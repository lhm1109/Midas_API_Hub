// HydrTemperDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_treemenu.h"
#include "HydrTemperDlg.h"
#include "..\wg_cmd\wg_cmd_GroupDlgs.h"

#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_ViewCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_cmd\wg_cmdAll.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CHydrTemperDlg dialog


CHydrTemperDlg::CHydrTemperDlg(CWnd* pParent /*=NULL*/)
	: CMenuBarChildDlg(CHydrTemperDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CHydrTemperDlg)
	m_option = 0;
	//}}AFX_DATA_INIT

	m_group.RemoveAll();
	m_group.Add(IDC_TM_STATIC2);
	m_group.Add(IDC_TM_HYDR_TEMPER);
	m_group.Add(IDC_TM_HYDR_TEMPER_UNIT);

	m_pDoc = CDBDoc::GetDocPoint();
}


void CHydrTemperDlg::DoDataExchange(CDataExchange* pDX)
{
	CMenuBarChildDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CHydrTemperDlg)
	DDX_Control(pDX, IDC_TM_GROUP_COMBO, m_wndGroupCombo);
	DDX_Control(pDX, IDC_TM_HYDR_TEMPER_UNIT, m_temperunit);
	DDX_Control(pDX, IDC_TM_HYDR_TEMPER, m_temper);
	DDX_Radio(pDX, IDC_TM_HYDR_ADD, m_option);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CHydrTemperDlg, CMenuBarChildDlg)
	//{{AFX_MSG_MAP(CHydrTemperDlg)
	//ON_BN_CLICKED(IDC_TM_CLOSE, OnTmClose)
	//ON_BN_CLICKED(IDC_TM_EXECUTE, OnTmExecute)
	ON_BN_CLICKED(IDC_TM_HYDR_ADD, OnTmChangeOption)
	ON_BN_CLICKED(IDC_TM_HYDR_DEL, OnTmChangeOption)
	ON_BN_CLICKED(IDC_TM_HYDR_REPLACE, OnTmChangeOption)
	ON_BN_CLICKED(IDC_TM_DEFINE_GROUP_BUTTON, OnTmDefineGroupButton)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHydrTemperDlg message handlers

BOOL CHydrTemperDlg::OnInitDialog() 
{
	CMenuBarChildDlg::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_temper.SetUnitType(CUnitCtrl::m_HSPT_UNIT.dTemper);
	m_temperunit.SetUnitType(CUnitCtrl::m_HSPT_UNIT.dTemper);

	CtrlEnableDisable(m_group, TRUE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CHydrTemperDlg::Execute()
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

	T_HSPT_D data;
	data.Initialize();

	m_wndGroupCombo.GetSelectedBngr(data.GroupKey);
	data.dTemper = m_temper.GetEditValue();

	BOOL bSuccess=FALSE;
	if (m_option == 0 || m_option == 1) // Add or Replace
		bSuccess = m_pDoc->m_pDataCtrl->AddHspt(aSelKey, data, (m_option==0)?FALSE:TRUE);	
	else if (m_option == 2)  // delete
		bSuccess = m_pDoc->m_pDataCtrl->DelHspt(aSelKey, data.GroupKey);

	if (bSuccess) m_pDoc->m_pViewCtrl->UnselectAll(NULL);
}

void CHydrTemperDlg::OnTmChangeOption() 
{
	UpdateData();

	if(m_option == 2)
	{
		CtrlEnableDisable(m_group, FALSE);
		m_wndGroupCombo.ShowHideAll(TRUE);
	}
	else
	{
		CtrlEnableDisable(m_group, TRUE);
		m_wndGroupCombo.ShowHideAll(FALSE);
	}
}

BOOL CHydrTemperDlg::ExternalInit(UINT key)
{
	if(key == 0) return FALSE;

	T_HSPT_D data;
	T_HSPT_K HsptK;
	T_BNGR_K BngrK;
	HsptK.keymap = key;

	//m_pDoc->m_pAttrCtrl->GetHspt((T_HSPT_K)key, data);
	if(!m_pDoc->m_pAttrCtrl->GetHspt(HsptK, data)) ASSERT(0);
	BngrK= data.GroupKey;
	m_temper.SetEditUnit(data.dTemper);
	if(!m_wndGroupCombo.ChangeSelect(BngrK)) ASSERT(0);
	m_option = 1;
	UpdateData(FALSE);
	return TRUE;

}

void CHydrTemperDlg::OnTmDefineGroupButton() 
{
	CreateOrActivateDlg(m_pDoc,CCMBndrGrupDefDlg::IDD);	
}
