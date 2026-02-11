// ElemCompactDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_treemenu.h"
#include "ElemCompactDlg.h"

#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_ViewCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CElemCompactDlg dialog


CElemCompactDlg::CElemCompactDlg(CWnd* pParent /*=NULL*/)
	: CMenuBarChildDlg(CElemCompactDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CElemCompactDlg)
	m_nRange = 0;
	m_nStartNum = 1;
	m_nEndNum = 1;
	//}}AFX_DATA_INIT
	m_ByNumCtrl.Add(IDC_TM_START_STATIC);
	m_ByNumCtrl.Add(IDC_TM_START_EDIT);
	m_ByNumCtrl.Add(IDC_TM_END_STATIC);
	m_ByNumCtrl.Add(IDC_TM_END_EDIT);
}


void CElemCompactDlg::DoDataExchange(CDataExchange* pDX)
{
	CMenuBarChildDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CElemCompactDlg)
	DDX_Radio(pDX, IDC_TM_SELECT_ALL, m_nRange);
	DDX_Text(pDX, IDC_TM_START_EDIT, m_nStartNum);
	DDX_Text(pDX, IDC_TM_END_EDIT, m_nEndNum);
	//}}AFX_DATA_MAP
}

#define CDialog CMenuBarChildDlg
BEGIN_MESSAGE_MAP(CElemCompactDlg, CDialog)
#undef CDialog
	//{{AFX_MSG_MAP(CElemCompactDlg)
	ON_BN_CLICKED(IDC_TM_SELECT_ALL, OnTmSelectAll)
	ON_BN_CLICKED(IDC_TM_SELECT_RANGE, OnTmSelectRange)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CElemCompactDlg message handlers
BOOL CElemCompactDlg::OnInitDialog() 
{
	CMenuBarChildDlg::OnInitDialog();
	
	// TODO: Add extra initialization here
	CDBDoc* pDoc = (CDBDoc*)CTreeMenuBarBase::GetDocST();
	m_nEndNum = pDoc->m_pAttrCtrl->GetLastNumElem();
	UpdateData(FALSE);

	CtrlEnableDisable(m_ByNumCtrl, m_nRange != 0);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CElemCompactDlg::OnTmSelectAll() 
{
	// TODO: Add your control notification handler code here
	CtrlEnableDisable(m_ByNumCtrl, FALSE);
}

void CElemCompactDlg::OnTmSelectRange() 
{
	// TODO: Add your control notification handler code here
	CtrlEnableDisable(m_ByNumCtrl, TRUE);	
}

void CElemCompactDlg::Execute()
{
	if (!UpdateData(TRUE)) return;

	CDBDoc* pDoc = (CDBDoc*)CTreeMenuBarBase::GetDocST();

	if (m_nRange == 0) 
	{
		m_nStartNum = 1;
		m_nEndNum = pDoc->m_pAttrCtrl->GetLastNumElem();
	}
	BOOL bSuccess = pDoc->m_pDataCtrl->CompactElem(m_nStartNum, m_nEndNum);
	if (bSuccess) pDoc->m_pViewCtrl->UnselectAll(NULL);
}
