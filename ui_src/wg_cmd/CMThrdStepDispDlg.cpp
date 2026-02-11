// CMThrdStepDispDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMThrdStepDispDlg.h"
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
// CCMThrdStepDispDlg dialog


CCMThrdStepDispDlg::CCMThrdStepDispDlg(CWnd* pParent /*=NULL*/)
	: CCMChildBarBase(CCMThrdStepDispDlg::IDD)
{
	//{{AFX_DATA_INIT(CCMThrdStepDispDlg)
	m_strName = _T("");
	m_nResultType = 0;
	//}}AFX_DATA_INIT
}


void CCMThrdStepDispDlg::DoDataExchange(CDataExchange* pDX)
{
	CCMChildBarBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMThrdStepDispDlg)
	DDX_Control(pDX, IDC_CMD_BAR_THLOADCASE, m_cmbLoadCase);
	DDX_Control(pDX, IDC_CMD_BAR_THVALUE, m_cmbValue);
	DDX_Control(pDX, IDC_CMD_BAR_THCOMP, m_cmbComponents);
	DDX_Control(pDX, IDC_CMD_BAR_THKYNDEL, m_NodeNum);
	DDX_Text(pDX, IDC_CMD_BAR_THFUNCNAME, m_strName);
	DDX_Radio(pDX, IDC_CMD_BAR_THTYPE_DISP, m_nResultType);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCMThrdStepDispDlg, CCMChildBarBase)
	//{{AFX_MSG_MAP(CCMThrdStepDispDlg)
	ON_BN_CLICKED(IDC_CMD_CANCEL, OnCmdCancel)
	ON_BN_CLICKED(IDC_CMD_OK, OnCmdOK)
	ON_BN_CLICKED(IDC_CMD_APPLY, OnCmdApply)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMThrdStepDispDlg message handlers

void CCMThrdStepDispDlg::OnCmdCancel()
{
	DataInit();
	((CCMThMainDlg*)(GetParent()->GetParent()))->SetThrdMain(1, 1);
}

void CCMThrdStepDispDlg::OnCmdOK()
{
	if (OnExecute())
	{
		DataInit();
		((CCMThMainDlg*)(GetParent()->GetParent()))->SetThrdMain(1, 1);
	}
}

void CCMThrdStepDispDlg::OnCmdApply()
{
	if (OnExecute())
	{
		DataInit();
		Data2Dlg();
	}
}

void CCMThrdStepDispDlg::DataInit()
{
	m_pDlgData.Initialize();
	m_pDlgData.nSubType = 1;
	m_pDlgData.nComponent = 1;
	
	m_bModify = FALSE;
	strOldName = _T("");
}

void CCMThrdStepDispDlg::Data2Dlg()
{
	m_strName = m_pDlgData.Name;
	if(m_pDlgData.nEntity)
	{
		CString str;
		str.Format(_T("%d"), m_pDlgData.nEntity);
		m_NodeNum.SetWindowText(str);
	}
	m_nResultType = m_pDlgData.nSubType - 1;
	m_cmbComponents.SetCurSel(m_pDlgData.nComponent-1);
	m_cmbValue.SetCurSel(m_pDlgData.nValueType);
	m_cmbLoadCase.ChangeSelect(D_LOADCASE_HISTORY,m_pDlgData.KeyThis);

	UpdateData(FALSE);
}

BOOL CCMThrdStepDispDlg::Dlg2Data()
{
	UpdateData();

	m_pDlgData.nGraphStep = 1;
	m_pDlgData.nStepFunction = 2;
	m_pDlgData.Name = m_strName;

	CString str;
	m_NodeNum.GetWindowText(str);
	m_pDlgData.nEntity = _ttoi(str);

	m_pDlgData.nSubType = m_nResultType + 1;
	m_pDlgData.nComponent = m_cmbComponents.GetCurSel() + 1;
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

BOOL CCMThrdStepDispDlg::OnExecute()
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

BOOL CCMThrdStepDispDlg::OnInitDialog() 
{
	CCMChildBarBase::OnInitDialog();

	m_bModify = FALSE;
	if (!m_bModify)
		DataInit();
	
	m_cmbLoadCase.SetLoadType(D_SELECTLOAD_HISTORY);

	m_NodeNum.SetAttNodeList();
	m_NodeNum.SetLButtonDownNotifyWindow(this);
	m_NodeNum.SetEnterNotifyWindow(this);
	m_NodeNum.SetModeToUse(MOUSEEDIT_USE_GET_NODE_LIST);
	m_NodeNum.SetMaxNodeKeyNum(1);
	GotoDlgCtrl(&m_NodeNum);

	MakeCombo();
	Data2Dlg();
	return TRUE;
}

void CCMThrdStepDispDlg::MakeCombo()
{
	CString sComp[]={ _LSX(DX), _LSX(DY), _LSX(DZ), _LSX(RX), _LSX(RY), _LSX(RZ) };
	for(int i=0; i<6; i++)
		m_cmbComponents.AddString(sComp[i]);

	CString sValue[] = { _LS(IDS_WG_CMD_MIN), _LS(IDS_WG_CMD_MAX), _LS(IDS_WG_CMD_ABSMAX) };
	for (int i=0; i<3; i++)
		m_cmbValue.AddString(sValue[i]);
}

BOOL CCMThrdStepDispDlg::ExternalInit(UINT key)
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
