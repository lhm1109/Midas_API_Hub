// ElemMergeDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_treemenu.h"
#include "ElemMergeDlg.h"

#include "..\wg_db\DB_ST_DT.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_ViewCtrl.h"
#include "..\wg_base\wg_base_DlgUtil.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CElemMergeDlg dialog
CElemMergeDlg::CElemMergeDlg(CWnd* pParent /*=NULL*/)
	: CMenuBarChildDlg(CElemMergeDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CElemMergeDlg)
	m_nElemType = 0;
	m_nSelectType = 1;
	m_bRemoveNode = TRUE;
	//}}AFX_DATA_INIT
	m_pDoc = CDBDoc::GetDocPoint();

	arForcedGroup.Add(IDC_TM_MERGE_TOLER_EDIT);
	//arForcedGroup.Add(IDC_TM_MERGE_TOLER_SPIN);
	arForcedGroup.Add(IDC_TM_MERGE_ELEM_EDIT);
}


void CElemMergeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CElemMergeDlg)
	DDX_Control(pDX, IDC_TM_MERGE_ELEM_EDIT, m_editElement);
	DDX_Radio(pDX, IDC_TM_ELEM_MERGE_TYPE_FRAME, m_nElemType);
	DDX_Radio(pDX, IDC_TM_ELEM_MERGE_TYPE_ALL, m_nSelectType);
	DDX_Check(pDX, IDC_TM_ELEM_MERGE_REMOVE, m_bRemoveNode);
	DDX_Control(pDX, IDC_TM_MERGE_TOLER_EDIT, m_wndTolerance);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CElemMergeDlg, CMenuBarChildDlg)
	//{{AFX_MSG_MAP(CElemMergeDlg)
	//ON_NOTIFY(UDN_DELTAPOS, IDC_TM_MERGE_TOLER_SPIN, OnDeltaposTmMergeTolerSpin)
	ON_BN_CLICKED(IDC_TM_ELEM_MERGE_TYPE_FRAME, OnTmElemMergeType)
	ON_BN_CLICKED(IDC_TM_ELEM_MERGE_TYPE_PLANE, OnTmElemMergeType)
	ON_BN_CLICKED(IDC_TM_ELEM_MERGE_TYPE_SOLID, OnTmElemMergeType)
	ON_BN_CLICKED(IDC_TM_ELEM_MERGE_TYPE_ALL, OnTmElemMergeType)
	ON_BN_CLICKED(IDC_TM_ELEM_MERGE_TYPE_FORCE, OnTmElemMergeType)
	ON_BN_CLICKED(IDC_TM_ELEM_MERGE_TYPE_SELECT, OnTmElemMergeType)
	//}}AFX_MSG_MAP
	ON_REGISTERED_MESSAGE(CMouseEdit::WM_LBUTTONDOWN_FROM_MVIEW, OnMViewLButtonDown)
	ON_REGISTERED_MESSAGE(CMouseEdit::WM_MOUSEEDITENTER,OnNodeListEnter)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CElemMergeDlg message handlers

BOOL CElemMergeDlg::OnInitDialog() 
{
	CMenuBarChildDlg::OnInitDialog();
	
	UpdateData(FALSE);

	// 임시로 기능이 안되어서 죽인다.
	((CButton*)GetDlgItem(IDC_TM_ELEM_MERGE_TYPE_PLANE))->EnableWindow(FALSE);
	((CButton*)GetDlgItem(IDC_TM_ELEM_MERGE_TYPE_SOLID))->EnableWindow(FALSE);

	// MouseEdit Setting
	m_editElement.SetAttElemList();
	m_editElement.SetLButtonDownNotifyWindow(this);
	m_editElement.SetEnterNotifyWindow(this);
	m_editElement.SetModeToUse(MOUSEEDIT_USE_GET_ELEM_LIST);
	m_editElement.SetMaxElemKeyNum(2);

	m_wndTolerance.SetRange(-360, 360);
	m_wndTolerance.SetValue(5);

	// Enable/Disable
	EnableDisableForced();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CElemMergeDlg::ExecuteForced()
{
	CArray<UINT, UINT> aKeyElem;
	CString csElemList = _T("");
	m_editElement.GetWindowText(csElemList);
	if (!GetNodeList(csElemList, aKeyElem) || aKeyElem.GetSize() != 2) return FALSE;

	UpdateData(TRUE);
	double dTolerance = m_wndTolerance.GetEditValue();
	return m_pDoc->m_pDataCtrl->MergeElement(aKeyElem[0], aKeyElem[1], dTolerance, m_bRemoveNode);
}

void CElemMergeDlg::Execute()
{
	UpdateData(TRUE);
	I_GENModelBase * pIGM = CTreeMenuBarBase::GetI_GENModelST();
	CArray<T_ELEM_K, T_ELEM_K> ElemKeyList; ElemKeyList.RemoveAll();
	
	if(m_nSelectType == 1)       // 1 for Select
	{
		if(!pIGM->GetSelectedElemKeyList(ElemKeyList,TRUE)) // 선택된 Elem KeyList 가져오기 
		{
			GSaveHistoryNF(_LS(IDS_WG_TREEMENU_Error___No_selected_element_));
			return; 
		}
		if(m_pDoc->m_pDataCtrl->MergeElement(ElemKeyList, m_nElemType+1, m_bRemoveNode))
			m_pDoc->m_pViewCtrl->UnselectAll(NULL);
	}
	else if(m_nSelectType == 0) // 0 for All
	{
		m_pDoc->m_pAttrCtrl->GetElemKeyList(ElemKeyList);   // 전체 Elem KeyList 가져오기 
	
		if(!ElemKeyList.GetSize())
		{
			GSaveHistoryNF(_LS(IDS_WG_TREEMENU_Error___No_selected_element_));
			return; 
		}
		if(m_pDoc->m_pDataCtrl->MergeElement(ElemKeyList, m_nElemType+1, m_bRemoveNode))
			m_pDoc->m_pViewCtrl->UnselectAll(NULL);
	}
	else if(m_nSelectType == 2) // 2 for Forced Merge
	{
		ExecuteForced();
	}
	else ASSERT(0);
}

LRESULT CElemMergeDlg::OnMViewLButtonDown(WPARAM wParam, LPARAM lParam)
{
	CString csElemList = _T("");
	if(m_editElement.GetSafeHwnd() == (HWND)lParam)
	{
		if(!ExecuteForced()) return 0L;
		
		m_editElement.GetWindowText(csElemList);
		m_editElement.ClearContents();
		m_editElement.GetWindowText(csElemList);
	}
	return 0L;  
}



LRESULT CElemMergeDlg::OnNodeListEnter(WPARAM wParam, LPARAM lParam)
{
	if(m_editElement.GetSafeHwnd() == (HWND)lParam)
	{
		ExecuteForced();
		m_editElement.ClearContents();
	}
	return 0L;
}

// Tolerance Spin
void CElemMergeDlg::OnDeltaposTmMergeTolerSpin(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	CDlgUtil::SetSpinValue(this, IDC_TM_MERGE_TOLER_EDIT, 0, 360, pNMHDR, pResult);
	*pResult = 0;
}

void CElemMergeDlg::EnableDisableForced()
{
	UpdateData(TRUE);

	if(m_nElemType == 0)  // Frame
	{
		GetDlgItem(IDC_TM_ELEM_MERGE_TYPE_FORCE)->EnableWindow(TRUE);

		if(m_nSelectType == 2)  //  Forced Merge
			CDlgUtil::CtrlEnableDisable(this, arForcedGroup, TRUE);
		else
			CDlgUtil::CtrlEnableDisable(this, arForcedGroup, FALSE);
	}
	else
	{
		if(m_nSelectType == 2) m_nSelectType = 1;
		UpdateData(FALSE);
		GetDlgItem(IDC_TM_ELEM_MERGE_TYPE_FORCE)->EnableWindow(FALSE);
		CDlgUtil::CtrlEnableDisable(this, arForcedGroup, FALSE);
	}
}

// 강제 Merge는 Frame 요소에만 적용
void CElemMergeDlg::OnTmElemMergeType() 
{
	EnableDisableForced();
}