// BndrStoryDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_treemenu.h"
#include "BndrStoryDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBndrStoryDlg dialog


CBndrStoryDlg::CBndrStoryDlg(CWnd* pParent /*=NULL*/)
	: CMenuBarChildDlg(CBndrStoryDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CBndrStoryDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

void CBndrStoryDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBndrStoryDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	DDX_Control(pDX, IDC_BNDR_STORY_GROUP_TABLE, m_grid);
	//}}AFX_DATA_MAP
}


#define CDialog CMenuBarChildDlg
BEGIN_MESSAGE_MAP(CBndrStoryDlg, CDialog)
#undef CDialog
	//{{AFX_MSG_MAP(CBndrStoryDlg)
	ON_BN_CLICKED(IDC_TM_CLOSE, OnTmClose)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBndrStoryDlg message handlers

BOOL CBndrStoryDlg::OnInitDialog() 
{
	CMenuBarChildDlg::OnInitDialog();
	
	m_grid.Initialize();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CBndrStoryDlg::OnTmClose() 
{

	CloseDlg(); 
}
