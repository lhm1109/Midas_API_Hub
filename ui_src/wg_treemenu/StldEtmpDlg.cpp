// StldEtmpDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_treemenu.h"
#include "StldEtmpDlg.h"

#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_ViewCtrl.h"

#include "..\wg_main\wg_mainRes2.h"

#include "..\wg_cmd\wg_cmdAll.h"
#include "..\wg_cmd\wg_cmd_GroupDlgs.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define CDialog CMenuBarChildDlg
/////////////////////////////////////////////////////////////////////////////
// CStldEtmpDlg dialog


CStldEtmpDlg::CStldEtmpDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CStldEtmpDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CStldEtmpDlg)
	m_nOption = 0;
	//}}AFX_DATA_INIT
	m_aDeleteCtrl.Add(IDC_TM_T1_TEMP_EDIT);
}


void CStldEtmpDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CStldEtmpDlg)
	DDX_Control(pDX, IDC_TM_T1_TEMP_UNIT, m_wndT1Unit);
	DDX_Control(pDX, IDC_TM_T1_TEMP_EDIT, m_wndT1Edit);
	DDX_Control(pDX, IDC_TM_T0_TEMP_UNIT, m_wndT0Unit);
	DDX_Control(pDX, IDC_TM_T0_TEMP_EDIT, m_wndT0Edit);
	DDX_Control(pDX, IDC_TM_CBO_LC, m_LoadCase);
	DDX_Radio(pDX, IDC_TM_OPT_ADD, m_nOption);
	DDX_Control(pDX, IDC_TM_LDGR_CBO_LC, m_wndLdgrCobx);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CStldEtmpDlg, CDialog)
	//{{AFX_MSG_MAP(CStldEtmpDlg)
	ON_BN_CLICKED(IDC_TM_EXECUTE, OnTmExecute)
	ON_BN_CLICKED(IDC_TM_CLOSE, OnTmClose)
	ON_BN_CLICKED(IDC_TM_BTN_LC, OnClickLoadCaseBtn)
	ON_BN_CLICKED(IDC_TM_OPT_ADD, OnChangeOption)
	ON_BN_CLICKED(IDC_TM_PROJ_CONF_BTN, OnTmProjConfBtn)
	ON_BN_CLICKED(IDC_TM_OPT_DELETE, OnChangeOption)
	ON_BN_CLICKED(IDC_TM_OPT_REPLACE, OnChangeOption)
	ON_BN_CLICKED(IDC_TM_DEFINE_LDGR_BTN, OnTmDefineLdgrBtn)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStldEtmpDlg message handlers
BOOL CStldEtmpDlg::OnInitDialog() 
{
	m_pDoc = CDBDoc::GetDocPoint();
	CMenuBarChildDlg::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_LoadCase.SetLoadType(D_LOADCASE_STATIC, TRUE);
	m_wndT0Unit.SetUnitType(CUnitCtrl::m_STMP_UNIT.dblSysTemp);
	m_wndT1Edit.SetUnitType(CUnitCtrl::m_STMP_UNIT.dblSysTemp);
	m_wndT1Unit.SetUnitType(CUnitCtrl::m_STMP_UNIT.dblSysTemp);
	
	// Set Default Values
	m_nOption = 0;  // Default : Add
	m_wndT1Edit.SetWindowText(_LS(IDS_WG_TREEMENU_0_0));

	// Enable/Disable/Show/Hide/Align controls
	m_wndT0Edit.SetReadOnly(TRUE);  // initial temperature´Â Read OnlyÇÑ´Ù.

	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CStldEtmpDlg::OnTmExecute() 
{
	
	// TODO: Add your control notification handler code here
	CArray<T_ELEM_K, T_ELEM_K> aSelKey;
	I_GENModelBase * pIGM = CTreeMenuBarBase::GetI_GENModelST();  
	pIGM->GetSelectedElemKeyList(aSelKey);
	if (aSelKey.GetSize() == 0) 
	{
		AfxMessageBox(_LS(IDS_WG_TREEMENU_Error___No_selected_element_));
		return;
	}

	UINT nLoadCaseType;
	T_ETMP_D data;
	if (!m_LoadCase.GetSelectedLoad(nLoadCaseType, data.LoadCaseKey))
	{
		AfxMessageBox(_LS(IDS_WG_TREEMENU_Error___Loadcase_is_not_selected_));
		return;
	}

	BOOL bSuccess;
	T_LDGR_K LdgrK;
	m_wndLdgrCobx.GetSelectedLdgr(LdgrK);

	if (m_nOption == 0 || m_nOption == 1)
	{
		CString csTemperature;
		
		data.GroupKey = LdgrK;

		m_wndT1Edit.GetWindowText(csTemperature);
		if(!GetFloatNumber(csTemperature, data.dblElemTemp)) return;
		bSuccess = m_pDoc->m_pDataCtrl->AddEtmp(aSelKey, data, (m_nOption == 1));
	}
	else
	{
		bSuccess = m_pDoc->m_pDataCtrl->DelEtmp(aSelKey, data.LoadCaseKey,LdgrK);
	}

	if (bSuccess) m_pDoc->m_pViewCtrl->UnselectAll(NULL);
	
}

void CStldEtmpDlg::OnTmClose() 
{
	// TODO: Add your control notification handler code here
	CloseDlg();
}

void CStldEtmpDlg::OnClickLoadCaseBtn() 
{
	// TODO: Add your control notification handler code here
	AfxGetMainWnd()->SendMessage(WM_COMMAND, MAKEWPARAM(ID_STRUCT_STATIC_LOADCASE,0));	
}

void CStldEtmpDlg::OnChangeOption() 
{
	// TODO: Add your control notification handler code here
	const MSG* pMsg = GetCurrentMessage();
	switch(LOWORD(pMsg->wParam))
	{
	case IDC_TM_OPT_ADD: m_nOption = 0; break;
	case IDC_TM_OPT_REPLACE: m_nOption = 1; break;
	case IDC_TM_OPT_DELETE: m_nOption = 2; break;
	}

	if (m_nOption == 2)  // Delete
	{
	  CtrlEnableDisable(m_aDeleteCtrl, FALSE);
		this->m_wndLdgrCobx.ShowHideAll(TRUE);
	}
	else 
	{
		CtrlEnableDisable(m_aDeleteCtrl, TRUE);
		this->m_wndLdgrCobx.ShowHideAll(FALSE);
	}
}

void CStldEtmpDlg::OnTmProjConfBtn() 
{
	AfxGetMainWnd()->SendMessage(WM_COMMAND, MAKEWPARAM(ID_STRUCT_CONFIG,0));
}

void CStldEtmpDlg::OnTmDefineLdgrBtn() 
{
	CreateOrActivateDlg(m_pDoc,CCMLoadGroupDlg::IDD);		
}


