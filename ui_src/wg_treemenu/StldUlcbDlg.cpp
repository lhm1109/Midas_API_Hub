// StldUlcbDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_treemenu.h"
#include "StldUlcbDlg.h"

#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_ViewCtrl.h"

#include "..\wg_cmd\wg_cmdAll.h"
#include "..\wg_cmd\wg_cmd_GroupDlgs.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CStldUlcbDlg dialog


CStldUlcbDlg::CStldUlcbDlg(CWnd* pParent /*=NULL*/)
	: CMenuBarChildDlg(CStldUlcbDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CStldUlcbDlg)
	m_nOption = 0;
	//}}AFX_DATA_INIT
	m_aDeleteCtrl.Add(IDC_TM_LOAD_VALUE);
}


void CStldUlcbDlg::DoDataExchange(CDataExchange* pDX)
{
	CMenuBarChildDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CStldUlcbDlg)
	DDX_Control(pDX, IDC_TM_ULCB_LOAD_UNIT, m_untLength);
	DDX_Control(pDX, IDC_TM_ULCB_LOAD_VALUE, m_edtLength);
	DDX_Radio(pDX, IDC_TM_ULCB_OPT_ADD, m_nOption);
	DDX_Control(pDX, IDC_TM_LDGR_CBO_LC, m_cmbLdgr);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CStldUlcbDlg, CMenuBarChildDlg)
	//{{AFX_MSG_MAP(CStldUlcbDlg)
	ON_BN_CLICKED(IDC_TM_EXECUTE, OnTmExecute)
	ON_BN_CLICKED(IDC_TM_CLOSE, OnTmClose)
	ON_BN_CLICKED(IDC_TM_ULCB_OPT_ADD, OnChangeOption)
	ON_BN_CLICKED(IDC_TM_ULCB_OPT_DELETE, OnChangeOption)
	ON_BN_CLICKED(IDC_TM_ULCB_OPT_REPLACE, OnChangeOption)
	ON_BN_CLICKED(IDC_TM_DEFINE_LDGR_BTN, OnTmDefineLdgrBtn)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStldUlcbDlg message handlers

BOOL CStldUlcbDlg::OnInitDialog() 
{
	m_pDoc = CDBDoc::GetDocPoint();
	ASSERT(m_pDoc);
	CMenuBarChildDlg::OnInitDialog();

	// TODO: Add extra initialization here

	m_edtLength.SetUnitType(CUnitCtrl::m_ULCB_UNIT.dLength);
	m_untLength.SetUnitType(CUnitCtrl::m_ULCB_UNIT.dLength);

	// Set Default Values
	m_nOption = 0;  // Default : Add

	// Enable/Disable/Show/Hide/Align controls
	GetDlgItem(IDC_TM_EXECUTE)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_TM_CLOSE)->ShowWindow(SW_HIDE);

	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CStldUlcbDlg::OnTmExecute() 
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
		GSaveHistoryFormatNF(_LS(IDS_WG_TREEMENU_Error___No_selected__s_element_), _LS(IDS_WG_TREEMENU_Cable));
		return;
	}

	T_ULCB_D data;
	data.Initialize();

	data.dLength = m_edtLength.GetEditValue();

	BOOL bSuccess=FALSE;
	T_LDGR_K LdgrK;
	m_cmbLdgr.GetSelectedLdgr(LdgrK);

	if (m_nOption == 0 || m_nOption == 1) // Add or Replace
	{
		data.LoadGrpKey = LdgrK;
		bSuccess = m_pDoc->m_pDataCtrl->AddUlcb(aSelKey, data, (m_nOption==0)?FALSE:TRUE);	
	}
	else if (m_nOption == 2)  // delete
		bSuccess = m_pDoc->m_pDataCtrl->DelUlcb(aSelKey);	

	if (bSuccess) m_pDoc->m_pViewCtrl->UnselectAll(NULL);
}

void CStldUlcbDlg::OnTmClose() 
{
	// TODO: Add your control notification handler code here
	CloseDlg();
}

void CStldUlcbDlg::OnChangeOption() 
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
		this->m_cmbLdgr.ShowHideAll(TRUE);
	}
	else
	{
		this->m_cmbLdgr.ShowHideAll(FALSE);
	}

	CtrlEnableDisable(m_aDeleteCtrl, m_nOption != 2);
}

void CStldUlcbDlg::OnTmDefineLdgrBtn() 
{
	CreateOrActivateDlg(m_pDoc,CCMLoadGroupDlg::IDD);		
}