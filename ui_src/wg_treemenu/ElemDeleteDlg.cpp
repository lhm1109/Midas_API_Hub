// ElemDeleteDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_treemenu.h"
#include "ElemDeleteDlg.h"

#include "..\wg_base\I_GENStateDefine.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_ViewCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CElemDeleteDlg dialog


CElemDeleteDlg::CElemDeleteDlg(CWnd* pParent /*=NULL*/)
	: CMenuBarChildDlg(CElemDeleteDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CElemDeleteDlg)
	m_nOption = 1;
	m_bFreeNode = TRUE;
	//}}AFX_DATA_INIT
	m_PickingCtrl.Add(IDC_TM_PICK_STATIC);
	m_PickingCtrl.Add(IDC_TM_PICK_EDIT);
}


void CElemDeleteDlg::DoDataExchange(CDataExchange* pDX)
{
	CMenuBarChildDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CElemDeleteDlg)
	DDX_Control(pDX, IDC_TM_PICK_EDIT, m_wndElemNumber);
	DDX_Radio(pDX, IDC_TM_PICK_RADIO, m_nOption);
	DDX_Check(pDX, IDC_TM_FREE_CHECK, m_bFreeNode);
	//}}AFX_DATA_MAP
}

#define CDialog CMenuBarChildDlg
BEGIN_MESSAGE_MAP(CElemDeleteDlg, CDialog)
#undef CDialog
	//{{AFX_MSG_MAP(CElemDeleteDlg)
	ON_BN_CLICKED(IDC_TM_PICK_RADIO, OnTmPickRadio)
	ON_BN_CLICKED(IDC_TM_SEL_RADIO, OnTmSelRadio)
	//}}AFX_MSG_MAP
	ON_REGISTERED_MESSAGE(CMouseEdit::WM_LBUTTONDOWN_FROM_MVIEW, OnMViewLButtonDown)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CElemDeleteDlg message handlers

BOOL CElemDeleteDlg::OnInitDialog() 
{
	CMenuBarChildDlg::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_wndElemNumber.SetAttElemList();
	m_wndElemNumber.SetLButtonDownNotifyWindow(this);
	m_wndElemNumber.SetEnterNotifyWindow(this);
	m_wndElemNumber.SetModeToUse(MOUSEEDIT_USE_GET_ELEM_LIST);
	m_wndElemNumber.SetMaxElemKeyNum(1);

	CtrlEnableDisable(m_PickingCtrl, m_nOption == 0);
	if (m_nOption == 0) GotoDlgCtrl(&m_wndElemNumber);

	return FALSE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CElemDeleteDlg::OnTmPickRadio() 
{
	// TODO: Add your control notification handler code here
	CtrlEnableDisable(m_PickingCtrl, TRUE);	
	GotoDlgCtrl(&m_wndElemNumber);
}

void CElemDeleteDlg::OnTmSelRadio() 
{
	// TODO: Add your control notification handler code here
	I_GENModelBase * pIGM = CTreeMenuBarBase::GetI_GENModelST();  
	CtrlEnableDisable(m_PickingCtrl, FALSE);		
	pIGM->SetModelingModeST(IG_STATE_CUROP_END);
}

BOOL CElemDeleteDlg::GetParameter(_ElemDelete& ED)
{
	if (!UpdateData(TRUE)) return FALSE;
	
	// Get Parameter
	ED.m_bFreeNode = m_bFreeNode;
	if (m_nOption == 0)
	{
		CString csElemNum;
		m_wndElemNumber.GetWindowText(csElemNum);
		if (!GetNodeList(csElemNum, ED.m_KeyList)) return FALSE;
	}
	else
	{
		I_GENModelBase * pIGM = CTreeMenuBarBase::GetI_GENModelST();  
		if (!pIGM->GetSelectedElemKeyList(ED.m_KeyList)) return FALSE;
	}
	return TRUE;
}

BOOL CElemDeleteDlg::DelElem(_ElemDelete& ED)
{
	CDBDoc* pDoc = (CDBDoc*)CTreeMenuBarBase::GetDocST();
	BOOL bSuccess = pDoc->m_pDataCtrl->DelElem(ED.m_KeyList, ED.m_bFreeNode);
	m_wndElemNumber.ClearElemKeyList();
	if (bSuccess) pDoc->m_pViewCtrl->UnselectAll(NULL);
	return TRUE;
}

void CElemDeleteDlg::Execute()
{
	_ElemDelete ED;
	if(GetParameter(ED))
	{
		DelElem(ED);
	}
	else
		GSaveHistoryNF(_LS(IDS_WG_TREEMENU_Error___Parameter_Error____));
}

LRESULT CElemDeleteDlg::OnMViewLButtonDown(WPARAM wParam, LPARAM lParam)
{
	if (m_wndElemNumber.IsKeyPressed()) return 0L;

	I_GENModelBase * pIGM = CTreeMenuBarBase::GetI_GENModelST();
	if(m_wndElemNumber.GetSafeHwnd() == (HWND)lParam)
	{
		if(pIGM->GetModelingModeST() == IG_STATE_GET_ELEM_LIST)
		{
			_ElemDelete ED;
			if(GetParameter(ED))
			{
				CButton* pExecButton = GetExecButton();
				if(pExecButton)
				{
				// Execute Button의 Push 효과를 낸다. 
				}
				DelElem(ED);
				//m_wndElemNumber.SetFocus();
			}
		}
	}
	return 0L;
}
