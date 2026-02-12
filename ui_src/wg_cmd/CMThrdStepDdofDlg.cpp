// CMThrdStepDdofDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMThrdStepDdofDlg.h"
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
// CCMThrdStepDdofDlg dialog


CCMThrdStepDdofDlg::CCMThrdStepDdofDlg(CWnd* pParent /*=NULL*/)
	: CCMChildBarBase(CCMThrdStepDdofDlg::IDD)
{
	//{{AFX_DATA_INIT(CCMThrdStepDdofDlg)
	m_strName = _T("");
	//}}AFX_DATA_INIT
}


void CCMThrdStepDdofDlg::DoDataExchange(CDataExchange* pDX)
{
	CCMChildBarBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMThrdStepDdofDlg)
	DDX_Control(pDX, IDC_CMD_BAR_THLOADCASE, m_cmbLoadCase);
	DDX_Control(pDX, IDC_CMD_BAR_THVALUE2, m_cmbValue);
	DDX_Control(pDX, IDC_CMD_BAR_THCOMP, m_cmbComponents);
	DDX_Control(pDX, IDC_CMD_BAR_THPOINT, m_cmbPoint);
	DDX_Text(pDX, IDC_CMD_BAR_THFUNCNAME, m_strName);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCMThrdStepDdofDlg, CCMChildBarBase)
	//{{AFX_MSG_MAP(CCMThrdStepDdofDlg)
	ON_BN_CLICKED(IDC_CMD_CANCEL, OnCmdCancel)
	ON_BN_CLICKED(IDC_CMD_OK, OnCmdOK)
	ON_BN_CLICKED(IDC_CMD_APPLY, OnCmdApply)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMThrdStepDdofDlg message handlers

void CCMThrdStepDdofDlg::OnCmdCancel()
{
	DataInit();
	((CCMThMainDlg*)(GetParent()->GetParent()))->SetThrdMain(1, 4);
}

void CCMThrdStepDdofDlg::OnCmdOK()
{
	if (OnExecute())
	{
		DataInit();
		((CCMThMainDlg*)(GetParent()->GetParent()))->SetThrdMain(1, 4);
	}
}

void CCMThrdStepDdofDlg::OnCmdApply()
{
	if (OnExecute())
	{
		DataInit();
		Data2Dlg();
	}
}

void CCMThrdStepDdofDlg::DataInit()
{
	m_pDlgData.Initialize();
	m_pDlgData.nPosition = 1;
	m_pDlgData.nComponent = 1;
	
	m_bModify = FALSE;
	strOldName = _T("");
}

void CCMThrdStepDdofDlg::Data2Dlg()
{
	m_strName = m_pDlgData.Name;
	m_cmbPoint.SetCurSel(m_pDlgData.nPosition-1);
	m_cmbComponents.SetCurSel(m_pDlgData.nComponent-1);
	m_cmbValue.SetCurSel(m_pDlgData.nValueType);
	m_cmbLoadCase.ChangeSelect(D_LOADCASE_HISTORY, m_pDlgData.KeyThis);

	UpdateData(FALSE);
}

BOOL CCMThrdStepDdofDlg::Dlg2Data()
{
	UpdateData();

	m_pDlgData.nGraphStep = 1;
	m_pDlgData.nStepFunction = 5;
	m_pDlgData.Name = m_strName;
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

BOOL CCMThrdStepDdofDlg::OnExecute()
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

BOOL CCMThrdStepDdofDlg::OnInitDialog() 
{
	CCMChildBarBase::OnInitDialog();

	m_bModify = FALSE;
	if (!m_bModify)
		DataInit();
	
	m_cmbLoadCase.SetLoadType(D_SELECTLOAD_HISTORY);

	MakeCombo();
	Data2Dlg();
	return TRUE;
}

void CCMThrdStepDdofDlg::MakeCombo()
{
	m_cmbPoint.AddString(_LS(IDS_WG_CMD_I_NODE));
	m_cmbPoint.AddString(_LS(IDS_WG_CMD_J_NODE));
	m_cmbPoint.AddString(_LS(IDS_WG_CMD_M_NODE));
	m_cmbPoint.SetCurSel(0);

	CString sCompo[]={_LSX(Fx), _LSX(Fy), _LSX(Fz), _LSX(Mx), _LSX(My), _LSX(Mz)};
	for (int i=0; i<6; i++)
		m_cmbComponents.AddString(sCompo[i]);
	m_cmbComponents.SetCurSel(0);

	CString sValue[] = { _LS(IDS_WG_CMD_MIN), _LS(IDS_WG_CMD_MAX), _LS(IDS_WG_CMD_ABSMAX) };
	for (int i=0; i<3; i++)
		m_cmbValue.AddString(sValue[i]);
	m_cmbValue.SetCurSel(0);
}

BOOL CCMThrdStepDdofDlg::ExternalInit(UINT key)
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
