// ReportProgressbarDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_treemenu.h"
#include "ReportProgressbarDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CReportProgressbarDlg dialog


CReportProgressbarDlg::CReportProgressbarDlg(CWnd* pParent /*=NULL*/)
	: CModelessMoveDlg(CReportProgressbarDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CReportProgressbarDlg)
	//}}AFX_DATA_INIT
	m_nIdx = 1;
}

void CReportProgressbarDlg::DoDataExchange(CDataExchange* pDX)
{
	CModelessMoveDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CReportProgressbarDlg)
	DDX_Control(pDX, IDC_TM_DYGEN_PROGRESS_BAR, m_ProgressBar);
	//}}AFX_DATA_MAP
}

#define CDialog CModelessMoveDlg
BEGIN_MESSAGE_MAP(CReportProgressbarDlg, CDialog)
#undef CDialog
	//{{AFX_MSG_MAP(CReportProgressbarDlg)
	ON_BN_CLICKED(IDCANCEL, OnCancel)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CReportProgressbarDlg message handlers

BOOL CReportProgressbarDlg::Create(CWnd* pParentWnd) 
{
	// TODO: Add your specialized code here and/or call the base class
	return CModelessMoveDlg::Create(IDD, pParentWnd);
}

BOOL CReportProgressbarDlg::OnInitDialog() 
{
	CModelessMoveDlg::OnInitDialog();

 	// TODO: Add extra initialization here
	m_ProgressBar.SetRange(0, 100); // Set Progress Bar Range.
	m_ProgressBar.SetPos(10);  // Set Progress Position.
	SetTimer(1,1000,NULL);

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CReportProgressbarDlg::PostNcDestroy() 
{
	// TODO: Add your specialized code here and/or call the base class
	DestroyWindow();
	CModelessMoveDlg::PostNcDestroy();
}

void CReportProgressbarDlg::OnCancel() 
{
	// TODO: Add your control notification handler code here
	m_ProgressBar.DestroyWindow();

	CModelessMoveDlg::OnCancel();
}

void CReportProgressbarDlg::OnTimer(UINT_PTR nIDEvent) 
{
	m_ProgressBar.OffsetPos(+10);
	++m_nIdx;

	if(m_nIdx==9)
	{
		this->OnCancel();
	}
}