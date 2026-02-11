// StldSdspDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_treemenu.h"
#include "StldSdspDlg.h"

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
// CStldSdspDlg dialog


CStldSdspDlg::CStldSdspDlg(CWnd* pParent /*=NULL*/)
	: CMenuBarChildDlg(CStldSdspDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CStldSdspDlg)
	m_nOption = -1;
	//}}AFX_DATA_INIT
}


void CStldSdspDlg::DoDataExchange(CDataExchange* pDX)
{
	CMenuBarChildDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CStldSdspDlg)
	DDX_Control(pDX, IDC_TM_SD_CBO_LC, m_LoadCase);
	DDX_Radio(pDX, IDC_TM_SD_OPT_ADD, m_nOption);
	DDX_Control(pDX, IDC_TM_LDGR_CBO_LC, m_wndLdgrCobx);
	//}}AFX_DATA_MAP
	UINT nID[6][3] = { 
		{IDC_TM_SD_FLAG1, IDC_TM_SD_DX, IDC_TM_SD_DX_UNIT},
		{IDC_TM_SD_FLAG2, IDC_TM_SD_DY, IDC_TM_SD_DY_UNIT},
		{IDC_TM_SD_FLAG3, IDC_TM_SD_DZ, IDC_TM_SD_DZ_UNIT},
		{IDC_TM_SD_FLAG4, IDC_TM_SD_RX, IDC_TM_SD_RX_UNIT},
		{IDC_TM_SD_FLAG5, IDC_TM_SD_RY, IDC_TM_SD_RY_UNIT},
		{IDC_TM_SD_FLAG6, IDC_TM_SD_RZ, IDC_TM_SD_RZ_UNIT},
	};
	for (int i = 0; i < 6; i++)
	{
	  DDX_Control(pDX, nID[i][0], m_Flag[i]);
	  DDX_Control(pDX, nID[i][1], m_Value[i]);
	  DDX_Control(pDX, nID[i][2], m_Unit[i]);
	}
	DDX_Control(pDX, IDC_WG_TREEMENU_STATIC3, m_wndPicture);
}

BEGIN_MESSAGE_MAP(CStldSdspDlg, CMenuBarChildDlg)
	//{{AFX_MSG_MAP(CStldSdspDlg)
	ON_BN_CLICKED(IDC_TM_SD_BTN_LC, OnClickLoadCaseBtn)
	ON_BN_CLICKED(IDC_TM_SD_FLAG1, OnToggleFlag)
	ON_BN_CLICKED(IDC_TM_SD_OPT_ADD, OnChangeOption)
	ON_BN_CLICKED(IDC_TM_SD_FLAG2, OnToggleFlag)
	ON_BN_CLICKED(IDC_TM_SD_FLAG3, OnToggleFlag)
	ON_BN_CLICKED(IDC_TM_SD_FLAG4, OnToggleFlag)
	ON_BN_CLICKED(IDC_TM_SD_FLAG5, OnToggleFlag)
	ON_BN_CLICKED(IDC_TM_SD_FLAG6, OnToggleFlag)
	ON_BN_CLICKED(IDC_TM_SD_OPT_REPLACE, OnChangeOption)
	ON_BN_CLICKED(IDC_TM_SD_OPT_DELETE, OnChangeOption)
	ON_BN_CLICKED(IDC_TM_DEFINE_LDGR_BTN, OnTmDefineLdgrBtn)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStldSdspDlg message handlers

BOOL CStldSdspDlg::OnInitDialog() 
{
	CMenuBarChildDlg::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_pDoc = CDBDoc::GetDocPoint();
	
	for (int i = 0; i < 6; i++)
	{
		m_Unit[i].SetUnitType(CUnitCtrl::m_SDSP_UNIT.Displacement[i]);
		m_Value[i].SetUnitType(CUnitCtrl::m_SDSP_UNIT.Displacement[i]);
	}
	m_nOption = 0;
	for(int i=0; i < 6; i++) 
	{
		m_Flag[i].SetCheck(FALSE);
		m_Value[i].EnableWindow(FALSE);
	}
	m_wndPicture.SetImage(_T("SVG\\illustration\\Dialog\\tm_sdsp 1.svg"));
	GetDlgItem(IDC_TM_EXECUTE)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_TM_CLOSE)->ShowWindow(SW_HIDE);

	UpdateData(FALSE);	
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CStldSdspDlg::Execute()
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
	T_SDSP_D data;
	unsigned int nLoadCaseType, nLoadCaseKey;
	data.Initialize();

	m_LoadCase.GetSelectedLoad(nLoadCaseType, nLoadCaseKey);
	data.LoadCaseKey = nLoadCaseKey;
	for (int i = 0; i < 6; i++)
	{
		data.Flag[i] = (m_Flag[i].GetCheck()) ? '1' : '0';
		if (m_Flag[i].GetCheck())
			data.Displacement[i] = m_Value[i].GetEditValue();
	}

	BOOL bSuccess=FALSE;
	T_LDGR_K LdgrK;
	m_wndLdgrCobx.GetSelectedLdgr(LdgrK);
	if (m_nOption == 0 || m_nOption == 1) // Add or Replace
	{
		
		data.GroupKey = LdgrK;

		bSuccess = m_pDoc->m_pDataCtrl->AddSdsp(aSelKey, data, (m_nOption==0)?FALSE:TRUE);	
	}
	else if (m_nOption == 2)  // delete
		bSuccess = m_pDoc->m_pDataCtrl->DelSdsp(aSelKey, nLoadCaseKey,LdgrK);	

	if (bSuccess) m_pDoc->m_pViewCtrl->UnselectAll(NULL);
}

void CStldSdspDlg::OnClickLoadCaseBtn() 
{
	// TODO: Add your control notification handler code here
	AfxGetMainWnd()->SendMessage(WM_COMMAND, MAKEWPARAM(ID_STRUCT_STATIC_LOADCASE,0));
}

void CStldSdspDlg::OnToggleFlag() 
{
	// TODO: Add your control notification handler code here
	BOOL bEnable;
	for (int i = 0; i < 6; i++)
	{
		bEnable = m_Flag[i].GetCheck() ? TRUE : FALSE;
		m_Value[i].EnableWindow(bEnable);
	}
}

void CStldSdspDlg::OnChangeOption() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	if (m_nOption == 2) // delete
	{
		EnableValueEdit(FALSE); // disable all value edit
		m_wndLdgrCobx.ShowHideAll(TRUE);
	}
	else
	{
		EnableValueEdit(TRUE);
		m_wndLdgrCobx.ShowHideAll(FALSE);
	}
}

void CStldSdspDlg::EnableValueEdit(BOOL bEnable)
{
	UINT nID[6][3] = { 
		{IDC_TM_SD_FLAG1, IDC_TM_SD_DX, IDC_TM_SD_DX_UNIT},
		{IDC_TM_SD_FLAG2, IDC_TM_SD_DY, IDC_TM_SD_DY_UNIT},
		{IDC_TM_SD_FLAG3, IDC_TM_SD_DZ, IDC_TM_SD_DZ_UNIT},
		{IDC_TM_SD_FLAG4, IDC_TM_SD_RX, IDC_TM_SD_RX_UNIT},
		{IDC_TM_SD_FLAG5, IDC_TM_SD_RY, IDC_TM_SD_RY_UNIT},
		{IDC_TM_SD_FLAG6, IDC_TM_SD_RZ, IDC_TM_SD_RZ_UNIT},
	};
	CWnd* pWnd;
	for (int i = 0; i < 6; i++)
	{
		pWnd = GetDlgItem(nID[i][0]);
		ASSERT(pWnd);
		pWnd->EnableWindow(bEnable);
		if (bEnable) OnToggleFlag();
		else
		{
			pWnd = GetDlgItem(nID[i][1]);
			ASSERT(pWnd);
			pWnd->EnableWindow(bEnable);
		}
	}
}

void CStldSdspDlg::OnTmDefineLdgrBtn() 
{
	CreateOrActivateDlg(m_pDoc,CCMLoadGroupDlg::IDD);		
}

