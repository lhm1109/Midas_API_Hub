// StldPrtsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_treemenu.h"
#include "StldPrtsDlg.h"

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

/////////////////////////////////////////////////////////////////////////////
// CStldPrtsDlg dialog


CStldPrtsDlg::CStldPrtsDlg(CWnd* pParent /*=NULL*/)
	: CMenuBarChildDlg(CStldPrtsDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CStldPrtsDlg)
	m_nOption = 0;
	//}}AFX_DATA_INIT
	m_aDeleteCtrl.Add(IDC_TM_LOAD_VALUE);
}


void CStldPrtsDlg::DoDataExchange(CDataExchange* pDX)
{
	CMenuBarChildDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CStldPrtsDlg)
	DDX_Control(pDX, IDC_TM_LOAD_UNIT, m_wndTensionUnit);
	DDX_Control(pDX, IDC_TM_LOAD_VALUE, m_wndTension);
	DDX_Control(pDX, IDC_TM_CBO_LC, m_LoadCase);
	DDX_Radio(pDX, IDC_TM_OPT_ADD, m_nOption);
	DDX_Control(pDX, IDC_TM_LDGR_CBO_LC, m_wndLdgrCobx);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CStldPrtsDlg, CMenuBarChildDlg)
	//{{AFX_MSG_MAP(CStldPrtsDlg)
	ON_BN_CLICKED(IDC_TM_EXECUTE, OnTmExecute)
	ON_BN_CLICKED(IDC_TM_CLOSE, OnTmClose)
	ON_BN_CLICKED(IDC_TM_BTN_LC, OnClickLoadCaseBtn)
	ON_BN_CLICKED(IDC_TM_OPT_ADD, OnChangeOption)
	ON_BN_CLICKED(IDC_TM_OPT_DELETE, OnChangeOption)
	ON_BN_CLICKED(IDC_TM_OPT_REPLACE, OnChangeOption)
	ON_BN_CLICKED(IDC_TM_DEFINE_LDGR_BTN, OnTmDefineLdgrBtn)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStldPrtsDlg message handlers

BOOL CStldPrtsDlg::OnInitDialog() 
{
	m_pDoc = CDBDoc::GetDocPoint();
	ASSERT(m_pDoc);
	CMenuBarChildDlg::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	m_wndTension.SetUnitType(CUnitCtrl::m_PTNS_UNIT.dTension);
	m_wndTensionUnit.SetUnitType(CUnitCtrl::m_PTNS_UNIT.dTension);

	// Set Default Values
	m_nOption = 0;  // Default : Add

	// Enable/Disable/Show/Hide/Align controls

	UpdateData(FALSE);

	GetDlgItem(IDC_TM_EXECUTE)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_TM_CLOSE)->ShowWindow(SW_HIDE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CStldPrtsDlg::OnTmExecute() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	CArray<T_ELEM_K, T_ELEM_K> aSelKey;
	T_ELEM_D ElemData;
	I_GENModelBase * pIGM = CTreeMenuBarBase::GetI_GENModelST();  
	pIGM->GetSelectedElemKeyList(aSelKey);

	BOOL bTruss;
	for (int i = aSelKey.GetSize()-1; i >= 0; i--)
	{
		m_pDoc->m_pAttrCtrl->GetElem(aSelKey.GetAt(i), ElemData);
		bTruss =  m_pDoc->m_pAttrCtrl->IsTruss(ElemData.eltyp)
					 || m_pDoc->m_pAttrCtrl->IsTensTruss(ElemData.eltyp)
					 || m_pDoc->m_pAttrCtrl->IsCompTruss(ElemData.eltyp);
		if (!bTruss) aSelKey.RemoveAt(i);
	}

	if (aSelKey.GetSize() == 0) 
	{
		GSaveHistoryFormatNF(_LS(IDS_WG_TREEMENU_Error___No_selected__s_element_), _LS(IDS_WG_TREEMENU_Truss));
		return;
	}

	unsigned int nLoadCaseType, nLoadCaseKey;
	T_PTNS_D data;
	data.Initialize();

	m_LoadCase.GetSelectedLoad(nLoadCaseType, nLoadCaseKey);
	data.LoadCaseKey = nLoadCaseKey;

	data.dTension = m_wndTension.GetEditValue();

	BOOL bSuccess=FALSE;
	T_LDGR_K LdgrK;
	m_wndLdgrCobx.GetSelectedLdgr(LdgrK);

	if (m_nOption == 0 || m_nOption == 1) // Add or Replace
	{
		data.GroupKey = LdgrK;

		bSuccess = m_pDoc->m_pDataCtrl->AddPtns(aSelKey, data, (m_nOption==0)?FALSE:TRUE);	
	}
	else if (m_nOption == 2)  // delete
		bSuccess = m_pDoc->m_pDataCtrl->DelPtns(aSelKey, nLoadCaseKey,LdgrK);	

	if (bSuccess) m_pDoc->m_pViewCtrl->UnselectAll(NULL);
}

void CStldPrtsDlg::OnTmClose() 
{
	// TODO: Add your control notification handler code here
	CloseDlg();
}

void CStldPrtsDlg::OnClickLoadCaseBtn() 
{
	// TODO: Add your control notification handler code here
	AfxGetMainWnd()->SendMessage(WM_COMMAND, MAKEWPARAM(ID_STRUCT_STATIC_LOADCASE,0));	
}

void CStldPrtsDlg::OnChangeOption() 
{
	// TODO: Add your control notification handler code here
	const MSG* pMsg = GetCurrentMessage();
	switch(LOWORD(pMsg->wParam))
	{
	case IDC_TM_OPT_ADD: m_nOption = 0; break;
	case IDC_TM_OPT_REPLACE: m_nOption = 1; break;
	case IDC_TM_OPT_DELETE: m_nOption = 2; break;
	}

	if(m_nOption == 2)
	{ 
		this->m_wndLdgrCobx.ShowHideAll(TRUE);
	}
	else
	{
		this->m_wndLdgrCobx.ShowHideAll(FALSE);
	}

	CtrlEnableDisable(m_aDeleteCtrl, m_nOption != 2);
}

void CStldPrtsDlg::OnTmDefineLdgrBtn() 
{
	CreateOrActivateDlg(m_pDoc,CCMLoadGroupDlg::IDD);		
}

