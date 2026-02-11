// CMThrdStepBeamDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMThrdStepWallDlg.h"
#include "CMThMainDlg.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCMThrdStepWallDlg dialog


CCMThrdStepWallDlg::CCMThrdStepWallDlg(CWnd* pParent /*=NULL*/)
	: CCMChildBarBase(CCMThrdStepWallDlg::IDD)
{
	//{{AFX_DATA_INIT(CCMThrdStepWallDlg)
	m_strName = _T("");
	m_nResultType = 0;
	//}}AFX_DATA_INIT
}


void CCMThrdStepWallDlg::DoDataExchange(CDataExchange* pDX)
{
	CCMChildBarBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMThrdStepWallDlg)
	DDX_Control(pDX, IDC_CMD_BAR_THLOADCASE, m_cmbLoadCase);
	DDX_Control(pDX, IDC_CMD_BAR_THVALUE, m_cmbValue);
	DDX_Control(pDX, IDC_CMD_BAR_THCOMP, m_cmbComponents);
	DDX_Control(pDX, IDC_CMD_BAR_THPOINT, m_cmbPoint);
	DDX_Control(pDX, IDC_CMD_BAR_THKYNDEL, m_ctrlElemNo);
	DDX_Text(pDX, IDC_CMD_BAR_THFUNCNAME, m_strName);
	DDX_Radio(pDX, IDC_CMD_BAR_THTYPE_FORCE, m_nResultType);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCMThrdStepWallDlg, CCMChildBarBase)
	//{{AFX_MSG_MAP(CCMThrdStepWallDlg)
	ON_BN_CLICKED(IDC_CMD_CANCEL, OnCmdCancel)
	ON_BN_CLICKED(IDC_CMD_OK, OnCmdOK)
	ON_BN_CLICKED(IDC_CMD_APPLY, OnCmdApply)
	ON_BN_CLICKED(IDC_CMD_BAR_THTYPE_FORCE, OnChangeType)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMThrdStepWallDlg message handlers

void CCMThrdStepWallDlg::OnCmdCancel()
{
	DataInit();
	((CCMThMainDlg*)(GetParent()->GetParent()))->SetThrdMain(1, 6);
}

void CCMThrdStepWallDlg::OnCmdOK()
{
	if (OnExecute())
	{
		DataInit();
		((CCMThMainDlg*)(GetParent()->GetParent()))->SetThrdMain(1, 6);
	}
}

void CCMThrdStepWallDlg::OnCmdApply()
{
	if (OnExecute())
	{
		DataInit();
		Data2Dlg();
	}
}

void CCMThrdStepWallDlg::DataInit()
{
	m_pDlgData.Initialize();
	m_pDlgData.nSubType = 1;
	m_pDlgData.nPosition = 1;
	m_pDlgData.nComponent = 1;
	
	m_bModify = FALSE;
	strOldName = _T("");
}

void CCMThrdStepWallDlg::Data2Dlg()
{
	m_strName = m_pDlgData.Name;
	
	CString str;

	if (m_pDlgData.nEntity == 0)
		str.Format(_T(""));
	else
		str.Format(_T("%d"), m_pDlgData.nEntity);

	m_ctrlElemNo.SetWindowText(str);
	m_nResultType = m_pDlgData.nSubType-1;

	MakeComponentCombo(m_pDlgData.nSubType);

	m_cmbPoint.SetCurSel(m_pDlgData.nPosition-1);
	m_cmbComponents.SetCurSel(m_pDlgData.nComponent-1);
	m_cmbValue.SetCurSel(m_pDlgData.nValueType);
	m_cmbLoadCase.ChangeSelect(D_LOADCASE_HISTORY, m_pDlgData.KeyThis);

	UpdateData(FALSE);
}

BOOL CCMThrdStepWallDlg::Dlg2Data()
{
	UpdateData();

	m_pDlgData.nGraphStep = 1;
	m_pDlgData.nStepFunction = 7;
	m_pDlgData.Name = m_strName;
	
	CString str;
	m_ctrlElemNo.GetWindowText(str);
	m_pDlgData.nEntity = _ttoi(str);

	m_pDlgData.nSubType = m_nResultType + 1;
	m_pDlgData.nPosition = m_cmbPoint.GetCurSel() + 1;
	m_pDlgData.nComponent = m_cmbComponents.GetCurSel()+1;
	m_pDlgData.nValueType = m_cmbValue.GetCurSel();

	UINT nLType,nLKey,nMnMxType;
	if(!m_cmbLoadCase.GetSelectedLoad(nLType, nLKey, &nMnMxType))
	{
		GSaveHistoryFormatNF(_LS(IDS_WG_CMD__ADDD__No_selected_time_history_loadcase));
		return FALSE;
	}
	m_pDlgData.KeyThis = nLKey; 
	return TRUE;
}

BOOL CCMThrdStepWallDlg::OnExecute()
{
	if (!Dlg2Data())
		return FALSE;

	CDBDoc *pDoc = CDBDoc::GetDocPoint();
	if (m_bModify)
	{
		if (!pDoc->m_pDataCtrl->ModifyThrd(strOldName, m_pDlgData))
			return FALSE;
	}
	else
	{
		if (!pDoc->m_pDataCtrl->AddThrd(m_pDlgData))
			return FALSE;
	}
	return TRUE;
}

BOOL CCMThrdStepWallDlg::OnInitDialog() 
{
	CCMChildBarBase::OnInitDialog();

	m_bModify = FALSE;
	if (!m_bModify)
		DataInit();
	
	m_cmbLoadCase.SetLoadType(D_SELECTLOAD_HISTORY);

	m_ctrlElemNo.SetAttNodeList();
	m_ctrlElemNo.SetLButtonDownNotifyWindow(this);
	m_ctrlElemNo.SetEnterNotifyWindow(this);
	m_ctrlElemNo.SetModeToUse(MOUSEEDIT_USE_GET_ELEM_LIST);
	m_ctrlElemNo.SetMaxElemKeyNum(1);
	GotoDlgCtrl(&m_ctrlElemNo);

	MakeCombo();
	Data2Dlg();
	return TRUE;
}

void CCMThrdStepWallDlg::MakeCombo()
{
	m_cmbPoint.AddString(_LS(IDS_WG_CMD_I_NODE));
	m_cmbPoint.AddString(_LS(IDS_WG_CMD_J_NODE));

	CString sValue[] = { _LS(IDS_WG_CMD_MIN), _LS(IDS_WG_CMD_MAX), _LS(IDS_WG_CMD_ABSMAX) };
	for (int i=0; i<3; i++)
		m_cmbValue.AddString(sValue[i]);
}

void CCMThrdStepWallDlg::MakeComponentCombo(int nType)
{
	m_cmbComponents.ResetContent();
	if (nType == 1)
	{
		//Force
		m_cmbComponents.AddString(_LS(IDS_WG_CMD_AXIAL));
		m_cmbComponents.AddString(_LS(IDS_WG_CMD_SHEAR_Y));
		m_cmbComponents.AddString(_LS(IDS_WG_CMD_SHEAR_Z));
		m_cmbComponents.AddString(_LS(IDS_WG_CMD_TORSION));
		m_cmbComponents.AddString(_LS(IDS_WG_CMD_MOMENT_Y));
		m_cmbComponents.AddString(_LS(IDS_WG_CMD_MOMENT_Z));
		m_cmbComponents.SetCurSel(2);
	}
	else
	{
		//Stress
		m_cmbComponents.AddString(_LS(IDS_WG_CMD_AXIAL));
		m_cmbComponents.AddString(_LS(IDS_WG_CMD_SHEAR_STRESS_Y));
		m_cmbComponents.AddString(_LS(IDS_WG_CMD_SHEAR_STRESS_Z));
		m_cmbComponents.AddString(_LS(IDS_WG_CMD_BEND__Y_));
		m_cmbComponents.AddString(_LS(IDS_WG_CMD_BEND__Y__MSG1));
		m_cmbComponents.AddString(_LS(IDS_WG_CMD_BEND__Z_));
		m_cmbComponents.AddString(_LS(IDS_WG_CMD_BEND__Z__MSG2));
		m_cmbComponents.AddString(_LS(IDS_WG_CMD_COMBINED_MSG1));
		m_cmbComponents.AddString(_LS(IDS_WG_CMD_COMBINED_MSG2));
		m_cmbComponents.AddString(_LS(IDS_WG_CMD_COMBINED_MSG3));
		m_cmbComponents.AddString(_LS(IDS_WG_CMD_COMBINED_MSG4));
		m_cmbComponents.SetCurSel(2);
	}
}

void CCMThrdStepWallDlg::OnChangeType()
{
	UpdateData();
	MakeComponentCombo(m_nResultType+1);
}

BOOL CCMThrdStepWallDlg::ExternalInit(UINT key)
{
	if(key == 0) return FALSE;

	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	T_THRD_D ThrdD;
	pDoc->m_pAttrCtrl->GetThrd(key, ThrdD);
	m_pDlgData = ThrdD;
	strOldName = ThrdD.Name;
	m_bModify = TRUE;
	GetDlgItem(IDC_CMD_APPLY)->EnableWindow(FALSE);
	
	Data2Dlg();
	return TRUE;
}
