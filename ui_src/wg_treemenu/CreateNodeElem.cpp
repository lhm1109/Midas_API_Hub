// CreateNodeElem.cpp : implementation file
//

#include "stdafx.h"
#include "wg_treemenu.h"
#include "CreateNodeElem.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CreateNodeElemDlg dialog

CreateNodeElemDlg::CreateNodeElemDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CreateNodeElemDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CreateNodeElemDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

void CreateNodeElemDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CreateNodeElemDlg)
	DDX_Control(pDX, IDC_TM_EDIT1, m_wndCoord);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CreateNodeElemDlg, CDialog)
	//{{AFX_MSG_MAP(CreateNodeElemDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CreateNodeElemDlg message handlers

BOOL CreateNodeElemDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_wndCoord.SetAttUcsPos();
 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
