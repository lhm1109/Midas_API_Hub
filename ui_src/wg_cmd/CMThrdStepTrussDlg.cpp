// CMThrdStepTrussDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMThrdStepTrussDlg.h"
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
// CCMThrdStepTrussDlg dialog


CCMThrdStepTrussDlg::CCMThrdStepTrussDlg(CWnd* pParent /*=NULL*/)
	: CCMChildBarBase(CCMThrdStepTrussDlg::IDD)
{
	//{{AFX_DATA_INIT(CCMThrdStepTrussDlg)
	m_strName = _T("");
	m_nResultType = 0;
	//}}AFX_DATA_INIT
}


void CCMThrdStepTrussDlg::DoDataExchange(CDataExchange* pDX)
{
	CCMChildBarBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMThrdStepTrussDlg)
	DDX_Control(pDX, IDC_CMD_BAR_THLOADCASE, m_cmbLoadCase);
	DDX_Control(pDX, IDC_CMD_BAR_THVALUE, m_cmbValue);
	DDX_Control(pDX, IDC_CMD_BAR_THPOINT, m_cmbPoint);
	DDX_Control(pDX, IDC_CMD_BAR_THKYNDEL, m_edtElemNo);
	DDX_Text(pDX, IDC_CMD_BAR_THFUNCNAME, m_strName);
	DDX_Radio(pDX, IDC_CMD_BAR_THTYPE_FORCE, m_nResultType);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCMThrdStepTrussDlg, CCMChildBarBase)
	//{{AFX_MSG_MAP(CCMThrdStepTrussDlg)
	ON_BN_CLICKED(IDC_CMD_CANCEL, OnCmdCancel)
	ON_BN_CLICKED(IDC_CMD_OK, OnCmdOK)
	ON_BN_CLICKED(IDC_CMD_APPLY, OnCmdApply)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMThrdStepTrussDlg message handlers

void CCMThrdStepTrussDlg::OnCmdCancel()
{
	DataInit();
	((CCMThMainDlg*)(GetParent()->GetParent()))->SetThrdMain(1, 2);
}

void CCMThrdStepTrussDlg::OnCmdOK()
{
	if (OnExecute())
	{
		DataInit();
		((CCMThMainDlg*)(GetParent()->GetParent()))->SetThrdMain(1, 2);
	}
}

void CCMThrdStepTrussDlg::OnCmdApply()
{
	if (OnExecute())
	{
		DataInit();
		Data2Dlg();
	}
}

void CCMThrdStepTrussDlg::DataInit()
{
	m_pDlgData.Initialize();
	m_pDlgData.nSubType = 1;
	m_pDlgData.nPosition = 1;
	
	m_bModify = FALSE;
	strOldName = _T("");
}

void CCMThrdStepTrussDlg::Data2Dlg()
{
	m_strName = m_pDlgData.Name;
	
	CString str;
	if (m_pDlgData.nEntity == 0)
		str.Format(_T(""));
	else
		str.Format(_T("%d"), m_pDlgData.nEntity);
	m_edtElemNo.SetWindowText(str);
	
	m_nResultType = m_pDlgData.nSubType - 1;
	m_cmbPoint.SetCurSel(m_pDlgData.nPosition-1);
	m_cmbValue.SetCurSel(m_pDlgData.nValueType);
	m_cmbLoadCase.ChangeSelect(D_LOADCASE_HISTORY, m_pDlgData.KeyThis);

	UpdateData(FALSE);
}

BOOL CCMThrdStepTrussDlg::Dlg2Data()
{
	UpdateData();

	m_pDlgData.nGraphStep = 1;
	m_pDlgData.nStepFunction = 3;
	m_pDlgData.Name = m_strName;
	CString str;
	m_edtElemNo.GetWindowText(str);
	m_pDlgData.nEntity = _ttoi(str);
	
	m_pDlgData.nSubType = m_nResultType + 1;
	m_pDlgData.nPosition = m_cmbPoint.GetCurSel() + 1;
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

BOOL CCMThrdStepTrussDlg::OnExecute()
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

BOOL CCMThrdStepTrussDlg::OnInitDialog() 
{
	CCMChildBarBase::OnInitDialog();

	m_bModify = FALSE;
	if (!m_bModify)
		DataInit();
	
	m_cmbLoadCase.SetLoadType(D_SELECTLOAD_HISTORY);

	m_edtElemNo.SetAttNodeList();
	m_edtElemNo.SetLButtonDownNotifyWindow(this);
	m_edtElemNo.SetEnterNotifyWindow(this);
	m_edtElemNo.SetModeToUse(MOUSEEDIT_USE_GET_ELEM_LIST);
	m_edtElemNo.SetMaxElemKeyNum(1);
	GotoDlgCtrl(&m_edtElemNo);

	MakeCombo();
	Data2Dlg();
	return TRUE;
}

void CCMThrdStepTrussDlg::MakeCombo()
{
	m_cmbPoint.AddString(_LS(IDS_WG_CMD_I_NODE));
	m_cmbPoint.AddString(_LS(IDS_WG_CMD_J_NODE));
	m_cmbPoint.AddString(_LS(IDS_WG_CMD_M_NODE));

	CString sValue[] = { _LS(IDS_WG_CMD_MIN), _LS(IDS_WG_CMD_MAX), _LS(IDS_WG_CMD_ABSMAX) };
	for (int i=0; i<3; i++)
		m_cmbValue.AddString(sValue[i]);
}

BOOL CCMThrdStepTrussDlg::ExternalInit(UINT key)
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
