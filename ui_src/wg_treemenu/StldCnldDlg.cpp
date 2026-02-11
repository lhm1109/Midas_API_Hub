// StldCnldDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_treemenu.h"
#include "StldCnldDlg.h"

#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_ViewCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"

#include "..\wg_main\wg_mainRes2.h"

#include "..\wg_cmd\wg_cmdAll.h"
#include "..\wg_cmd\wg_cmd_GroupDlgs.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CStldCnldDlg dialog


CStldCnldDlg::CStldCnldDlg(CWnd* pParent /*=NULL*/)
	: CMenuBarChildDlg(CStldCnldDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CStldCnldDlg)
	m_nOption = -1;
	//}}AFX_DATA_INIT
}


void CStldCnldDlg::DoDataExchange(CDataExchange* pDX)
{
	CMenuBarChildDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CStldCnldDlg)
	DDX_Control(pDX, IDC_TM_NL_CBO_LC, m_LoadCase);
	DDX_Radio(pDX, IDC_TM_NL_OPT_ADD, m_nOption);
	DDX_Control(pDX, IDC_TM_LDGR_CBO_LC, m_wndLdgrCobx);
	DDX_Control(pDX, IDC_WG_TREEMENU_STATIC9, m_wndPicture);
	//}}AFX_DATA_MAP
	UINT nID[6][2] = { 
		{IDC_TM_NL_FX, IDC_TM_NL_FX_UNIT},
		{IDC_TM_NL_FY, IDC_TM_NL_FY_UNIT},
		{IDC_TM_NL_FZ, IDC_TM_NL_FZ_UNIT},
		{IDC_TM_NL_MX, IDC_TM_NL_MX_UNIT},
		{IDC_TM_NL_MY, IDC_TM_NL_MY_UNIT},
		{IDC_TM_NL_MZ, IDC_TM_NL_MZ_UNIT},
	};
	for (int i = 0; i < 6; i++)
	{
	  DDX_Control(pDX, nID[i][0], m_Value[i]);
	  DDX_Control(pDX, nID[i][1], m_Unit[i]);
	}
}

BEGIN_MESSAGE_MAP(CStldCnldDlg, CMenuBarChildDlg)
	//{{AFX_MSG_MAP(CStldCnldDlg)
	ON_BN_CLICKED(IDC_TM_NL_BTN_LC, OnClickLoadCaseBtn)
	ON_BN_CLICKED(IDC_TM_NL_OPT_ADD, OnChangeOption)
	ON_BN_CLICKED(IDC_TM_NL_OPT_REPLACE, OnChangeOption)
	ON_BN_CLICKED(IDC_TM_NL_OPT_DELETE, OnChangeOption)
	ON_BN_CLICKED(IDC_TM_DEFINE_LDGR_BTN, OnTmDefineLdgrBtn)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStldCnldDlg message handlers

BOOL CStldCnldDlg::OnInitDialog() 
{
	MInitCombo initCombo;
	SetRedraw(FALSE);
	CMenuBarChildDlg::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_pDoc = CDBDoc::GetDocPoint();
	
	for (int i = 0; i < 6; i++)
	{
		m_Unit[i].SetUnitType(CUnitCtrl::m_CNLD_UNIT.Force[i]);
		m_Value[i].SetUnitType(CUnitCtrl::m_CNLD_UNIT.Force[i]);
	}
	m_nOption = 0;
	m_wndPicture.SetImage(_T("SVG\\illustration\\Dialog\\tm_cnld 1.svg"));
	GetDlgItem(IDC_TM_EXECUTE)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_TM_CLOSE)->ShowWindow(SW_HIDE);

	UpdateData(FALSE);

	SetRedraw(TRUE);
	RedrawWindow(NULL, NULL, RDW_ERASE | RDW_FRAME | RDW_INVALIDATE | RDW_ALLCHILDREN | RDW_UPDATENOW);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CStldCnldDlg::Execute()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	CArray<T_NODE_K, T_NODE_K> aSelKey;
	I_GENModelBase * pIGM = CTreeMenuBarBase::GetI_GENModelST();  
	pIGM->GetSelectedNodeKeyList(aSelKey);
	if (aSelKey.GetSize() == 0) 
	{
		GSaveHistoryFormatNF(_LS(IDS_WG_TREEMENU_Error___No_selected_node_));
		return;
	}
	T_CNLD_D data;
	unsigned int nLoadCaseType, nLoadCaseKey;
	data.Initialize();

	m_LoadCase.GetSelectedLoad(nLoadCaseType, nLoadCaseKey);
	data.LoadCaseKey = nLoadCaseKey;
	for (int i = 0; i < 6; i++)
		data.Force[i] = m_Value[i].GetEditValue();

	T_LDGR_K LdgrK;
	m_wndLdgrCobx.GetSelectedLdgr(LdgrK);

	BOOL bSuccess=FALSE;
	if (m_nOption == 0 || m_nOption == 1) // add or replace
	{
		
		data.GroupKey = LdgrK;
		bSuccess = m_pDoc->m_pDataCtrl->AddCnld(aSelKey, data, (m_nOption==0)?FALSE:TRUE);	
	}
	else if (m_nOption == 2)   // delete
		bSuccess = m_pDoc->m_pDataCtrl->DelCnld(aSelKey, nLoadCaseKey,LdgrK);

	if (bSuccess) m_pDoc->m_pViewCtrl->UnselectAll(NULL);
}

void CStldCnldDlg::OnClickLoadCaseBtn() 
{
	// TODO: Add your control notification handler code here
	AfxGetMainWnd()->SendMessage(WM_COMMAND, MAKEWPARAM(ID_STRUCT_STATIC_LOADCASE,0));
}

void CStldCnldDlg::OnChangeOption() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	if (m_nOption == 2) // delete 
	{
		EnableValueEdit(FALSE);
		this->m_wndLdgrCobx.ShowHideAll(TRUE);
	}
	else
	{
		EnableValueEdit(TRUE);
		this->m_wndLdgrCobx.ShowHideAll(FALSE);
	}
}

void CStldCnldDlg::EnableValueEdit(BOOL bEnable)
{
	UINT nID[6] = {
		IDC_TM_NL_FX, IDC_TM_NL_FY, IDC_TM_NL_FZ,
		IDC_TM_NL_MX, IDC_TM_NL_MY, IDC_TM_NL_MZ,
	};
	CWnd* pWnd;
	for (int i = 0; i < 6; i++)
	{
		pWnd = GetDlgItem(nID[i]);
		ASSERT(pWnd);
		pWnd->EnableWindow(bEnable);
	}
}

void CStldCnldDlg::OnTmDefineLdgrBtn() 
{
	// TODO: Add your control notification handler code here
	CreateOrActivateDlg(m_pDoc,CCMLoadGroupDlg::IDD);		
}

BOOL CStldCnldDlg::ExternalInit_by_SelectLabel(UINT key)
{
	return FALSE;
}
