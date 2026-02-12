// HydrHeatDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_treemenu.h"
#include "HydrHeatDlg.h"

#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_ViewCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"

#include "..\wg_main\wg_mainRes2.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CHydrHeatDlg dialog

CHydrHeatDlg::CHydrHeatDlg(CWnd* pParent /*=NULL*/)
	: CMenuBarChildDlg(CHydrHeatDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CHydrHeatDlg)
	m_option = -1;
	//}}AFX_DATA_INIT

	m_group.RemoveAll();
	m_group.Add(IDC_TM_HYDR_HEAT_COMBO);
	m_group.Add(IDC_TM_HYDR_HEAT_BTN);

	m_pDoc = CDBDoc::GetDocPoint();
}

void CHydrHeatDlg::DoDataExchange(CDataExchange* pDX)
{
	CMenuBarChildDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CHydrHeatDlg)
	DDX_Control(pDX, IDC_TM_HYDR_HEAT_COMBO, m_CobxHeatFunc);
	DDX_Radio(pDX, IDC_TM_HYDR_ADDREPL, m_option);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CHydrHeatDlg, CMenuBarChildDlg)
	//{{AFX_MSG_MAP(CHydrHeatDlg)
	ON_BN_CLICKED(IDC_TM_HYDR_ADDREPL, OnTmChangeOption)
	ON_BN_CLICKED(IDC_TM_HYDR_DEL, OnTmChangeOption)
	ON_BN_CLICKED(IDC_TM_HYDR_HEAT_BTN, OnTmHydrHeatBtn)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHydrHeatDlg message handlers

BOOL CHydrHeatDlg::OnInitDialog() 
{
	CMenuBarChildDlg::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_option = 0;

	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CHydrHeatDlg::Execute() 
{
	// TODO: Add your control notification handler code here
 	UpdateData(TRUE);
	CArray<T_ELEM_K, T_ELEM_K> aSelKey;
	I_GENModelBase * pIGM = CTreeMenuBarBase::GetI_GENModelST();  
	pIGM->GetSelectedElemKeyList(aSelKey);

	if (aSelKey.GetSize() == 0) 
	{
		GSaveHistoryFormatNF(_LS(IDS_WG_TREEMENU_Error___No_selected_solid_element_));
		return;
	}

	T_HAHS_D data;
	T_HSFC_K key;
	data.Initialize();
	m_CobxHeatFunc.GetSelectedHsfc(key);
	data.nFuncKey = key;

	BOOL bSuccess=FALSE;
	if (m_option == 0) // Add or Replace
		bSuccess = m_pDoc->m_pDataCtrl->AddHahs(aSelKey, data);
	else if (m_option == 1)  // delete
		bSuccess = m_pDoc->m_pDataCtrl->DelHahs(aSelKey);

	if (bSuccess) m_pDoc->m_pViewCtrl->UnselectAll(NULL);
}

void CHydrHeatDlg::OnTmChangeOption() 
{
	// TODO: Add your control notification handler code here
	UpdateData();

	if(m_option == 1)
		CtrlEnableDisable(m_group, FALSE);
	else
		CtrlEnableDisable(m_group, TRUE);
}

void CHydrHeatDlg::OnTmHydrHeatBtn() 
{
	// TODO: Add your control notification handler code here
	AfxGetMainWnd()->SendMessage(WM_COMMAND, MAKEWPARAM(ID_STRUCT_HYDRATION_HEATSOURCE_DEFINE,0));
}

BOOL CHydrHeatDlg::ExternalInit(UINT key)
{
	if(key == 0) return FALSE;

	T_HAHS_D data;
	m_pDoc->m_pAttrCtrl->GetHahs(key, data);

	m_CobxHeatFunc.ChangeSelect(data.nFuncKey);

	UpdateData(FALSE);
	return TRUE;
}

