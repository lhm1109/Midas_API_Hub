// NodeDeleteDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_treemenu.h"
#include "NodeDeleteDlg.h"

#include "..\wg_base\I_GENStateDefine.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_ViewCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNodeDeleteDlg dialog


CNodeDeleteDlg::CNodeDeleteDlg(CWnd* pParent /*=NULL*/)
	: CMenuBarChildDlg(CNodeDeleteDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CNodeDeleteDlg)
	m_nOption = 1;
	m_bFreeNode = TRUE;
	//}}AFX_DATA_INIT
	m_PickingCtrl.Add(IDC_TM_PICK_STATIC);
	m_PickingCtrl.Add(IDC_TM_PICK_EDIT);
}


void CNodeDeleteDlg::DoDataExchange(CDataExchange* pDX)
{
	CMenuBarChildDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CNodeDeleteDlg)
	DDX_Control(pDX, IDC_TM_PICK_EDIT, m_wndNodeNumber);
	DDX_Radio(pDX, IDC_TM_PICK_RADIO, m_nOption);
	DDX_Check(pDX, IDC_TM_FREE_CHECK, m_bFreeNode);
	//}}AFX_DATA_MAP
}

#define CDialog CMenuBarChildDlg
BEGIN_MESSAGE_MAP(CNodeDeleteDlg, CDialog)
#undef CDialog
	//{{AFX_MSG_MAP(CNodeDeleteDlg)
	ON_BN_CLICKED(IDC_TM_PICK_RADIO, OnTmPickRadio)
	ON_BN_CLICKED(IDC_TM_SEL_RADIO, OnTmSelRadio)
	//}}AFX_MSG_MAP
	ON_REGISTERED_MESSAGE(CMouseEdit::WM_LBUTTONDOWN_FROM_MVIEW, OnMViewLButtonDown)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNodeDeleteDlg message handlers

BOOL CNodeDeleteDlg::OnInitDialog() 
{
	CMenuBarChildDlg::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_wndNodeNumber.SetAttNodeList();
	m_wndNodeNumber.SetLButtonDownNotifyWindow(this);
	m_wndNodeNumber.SetEnterNotifyWindow(this);
	m_wndNodeNumber.SetModeToUse(MOUSEEDIT_USE_GET_NODE_LIST);
	m_wndNodeNumber.SetMaxNodeKeyNum(1);

	CtrlEnableDisable(m_PickingCtrl, m_nOption == 0);
	if (m_nOption == 0) GotoDlgCtrl(&m_wndNodeNumber);

	return FALSE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CNodeDeleteDlg::OnTmPickRadio() 
{
	// TODO: Add your control notification handler code here
	CtrlEnableDisable(m_PickingCtrl, TRUE);	
	GotoDlgCtrl(&m_wndNodeNumber);
}

void CNodeDeleteDlg::OnTmSelRadio() 
{
	// TODO: Add your control notification handler code here
	I_GENModelBase * pIGM = CTreeMenuBarBase::GetI_GENModelST();  
	CtrlEnableDisable(m_PickingCtrl, FALSE);		
	pIGM->SetModelingModeST(IG_STATE_CUROP_END);
}

BOOL CNodeDeleteDlg::GetParameter(_NodeDelete& ND)
{
	if (!UpdateData(TRUE)) return FALSE;
	
	// Get Parameter
	ND.m_bFreeNode = m_bFreeNode;
	if (m_nOption == 0)
	{
		CString csNodeNum;
		m_wndNodeNumber.GetWindowText(csNodeNum);
		if (!GetNodeList(csNodeNum, ND.m_KeyList)) return FALSE;
	}
	else
	{
		I_GENModelBase * pIGM = CTreeMenuBarBase::GetI_GENModelST();  
		if (!pIGM->GetSelectedNodeKeyList(ND.m_KeyList)) return FALSE;
	}
	return TRUE;
}

BOOL CNodeDeleteDlg::DelNode(_NodeDelete& ND)
{
	CDBDoc* pDoc = (CDBDoc*)CTreeMenuBarBase::GetDocST();
	BOOL bSuccess = pDoc->m_pDataCtrl->DelNode(ND.m_KeyList, ND.m_bFreeNode);
	m_wndNodeNumber.ClearNodeKeyList();
	if (bSuccess) pDoc->m_pViewCtrl->UnselectAll(NULL);
	return TRUE;
}

void CNodeDeleteDlg::Execute()
{
	_NodeDelete ND;
	if(GetParameter(ND))
	{
		DelNode(ND);
	}
	else
		GSaveHistoryNF(_LS(IDS_WG_TREEMENU_Error___Parameter_Error____));
}

LRESULT CNodeDeleteDlg::OnMViewLButtonDown(WPARAM wParam, LPARAM lParam)
{
	if (m_wndNodeNumber.IsKeyPressed()) return 0L;

	I_GENModelBase * pIGM = CTreeMenuBarBase::GetI_GENModelST();
	if(m_wndNodeNumber.GetSafeHwnd() == (HWND)lParam)
	{
		if(pIGM->GetModelingModeST() == IG_STATE_GET_NODE_LIST)
		{
			_NodeDelete ND;
			if(GetParameter(ND))
			{
				CButton* pExecButton = GetExecButton();
				if(pExecButton)
				{
				// Execute Button의 Push 효과를 낸다. 
				}
				DelNode(ND);
				//m_wndNodeNumber.SetFocus();
			}
		}
	}
	return 0L;
}
