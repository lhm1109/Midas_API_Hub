// CmdAutoLoadCombCvlHelpDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CmdAutoLoadCombCvlHelpDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define CDialog CDialogMove

/////////////////////////////////////////////////////////////////////////////
// CCmdAutoLoadCombCvlHelpDlg dialog


CCmdAutoLoadCombCvlHelpDlg::CCmdAutoLoadCombCvlHelpDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCmdAutoLoadCombCvlHelpDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCmdAutoLoadCombCvlHelpDlg)
	//}}AFX_DATA_INIT
	m_pGrid = NULL;
	m_pGrid = new CCmdAutoLoadCombCvlHelpGrid(this);
}

CCmdAutoLoadCombCvlHelpDlg::~CCmdAutoLoadCombCvlHelpDlg()
{
	if(m_pGrid) delete m_pGrid;
}

void CCmdAutoLoadCombCvlHelpDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCmdAutoLoadCombCvlHelpDlg)  
	DDX_Control(pDX, IDC_CMD_AASHTO_LRFD08_HELP_GRID, *m_pGrid);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCmdAutoLoadCombCvlHelpDlg, CDialog)
	//{{AFX_MSG_MAP(CCmdAutoLoadCombCvlHelpDlg)
	
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCmdAutoLoadCombCvlHelpDlg message handlers

BOOL CCmdAutoLoadCombCvlHelpDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	m_pGrid->Initialize();
	m_pGrid->SetText();
	
	return TRUE;
}

