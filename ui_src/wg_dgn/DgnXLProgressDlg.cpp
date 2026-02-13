// CDgnXLProgressDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "..\DgnEngine\Src\DgnCalcBase\DgnCalcBase_PSC_Struct.h"
#include "..\DgnEngine\Src\DgnCalcBase\DgnCalcBase_PSC_ResultStruct.h"
#include "..\DgnEngine\Src\DGNe_RUS\DgnCalc_Rus_PSC_Struct.h"
#include "DgnXLProgressDlg.h"
#include "CRCExcelOutput.h"

#include "..\wg_base\wg_base_AppBase.h"
#include "..\wg_db\wg_db_DBLib.h"	// GetStlStatus(), GetSrcStatus().

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDgnXLProgressDlg dialog


CDgnXLProgressDlg::CDgnXLProgressDlg(CWnd* pParent /*=NULL*/)
	: CModelessMoveDlg(CDgnXLProgressDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDgnXLProgressDlg)
	m_strName = _LS(IDS_DGN_PSC_XL_REPORT_READY);
	m_strPercent = _T("0 %");
	//}}AFX_DATA_INIT

	m_pExcelOut = new CCRCExcelOutput;
	m_bStopExecute = FALSE;
}

CDgnXLProgressDlg::~CDgnXLProgressDlg()
{
	if(m_pExcelOut != NULL) delete m_pExcelOut;
	ShowWindow(SW_HIDE);
}

void CDgnXLProgressDlg::DoDataExchange(CDataExchange* pDX)
{
	CModelessMoveDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDgnXLProgressDlg)
	DDX_Control(pDX, IDC_DGN_PROGRESS_XL_BAR, m_ProgressBar);
	DDX_Text(pDX, IDC_DGN_PROGRESS_XL_STATIC, m_strName);
	DDX_Text(pDX, IDC_DGN_PERCENT_XL_STATIC, m_strPercent);
	//}}AFX_DATA_MAP
}

#define CDialog CModelessMoveDlg
BEGIN_MESSAGE_MAP(CDgnXLProgressDlg, CInternationalDlg)
#undef CDialog

	//{{AFX_MSG_MAP(CDgnXLProgressDlg)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_DGN_STOP_XL_BTN, OnDgnStopBtn)
	//}}AFX_MSG_MAP
	ON_MESSAGE (WM_PSC_XL_REPORT, OnMessage)	
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDgnXLProgressDlg message handlers

BOOL CDgnXLProgressDlg::Create(CWnd* pParentWnd) 
{
	// TODO: Add your specialized code here and/or call the base class
	return CModelessMoveDlg::Create(IDD, pParentWnd);
}

BOOL CDgnXLProgressDlg::OnInitDialog() 
{
	CModelessMoveDlg::OnInitDialog();
/*
	// TODO: Add extra initialization here
	// Set Progress Bar Range.
	m_ProgressBar.SetRange(0, 100);
	m_nPos = 0;
	// Set Progress Position.
	m_ProgressBar.SetPos(m_nPos);
	
	UpdateData(FALSE);
*/

	SetIgnoreBottomBtnArea();

	GetDlgItem(IDC_DGN_PROGRESS_XL_STATIC)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_DGN_PERCENT_XL_STATIC)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_DGN_PROGRESS_XL_BAR)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_DGN_STOP_XL_BTN)->ShowWindow(SW_SHOW);

	SetWindowText(_LS(IDS_DGN_PSC_XL_REPORT));

	Invalidate();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDgnXLProgressDlg::PostNcDestroy() 
{
	// TODO: Add your specialized code here and/or call the base class
	DestroyWindow();
	CModelessMoveDlg::PostNcDestroy();
}

void CDgnXLProgressDlg::OnDgnStopBtn() 
{
	// TODO: Add your control notification handler code here
	//m_bStopExecute = TRUE;
	m_pExcelOut->m_bStopExecute = TRUE;
}

void CDgnXLProgressDlg::Execute()
{
	m_pExcelOut->ThreadBegin(this, WM_PSC_XL_REPORT);
}

LRESULT CDgnXLProgressDlg::OnMessage(WPARAM wp, LPARAM lp)  
{
	switch(wp)
	{
		case 0:
			m_ProgressBar.SetPos(lp);
			break;

		case 1:
			m_strName = (LPCTSTR)lp;
			break;

		case 2:
			m_strPercent.Format(_T("%d %%"), lp);
			break;

		case 3:
			OnDestroy();
			break;
	}

	if(wp!=3)
		UpdateData(FALSE);

	return 0;
}

void CDgnXLProgressDlg::OnDestroy() 
{
	CModelessMoveDlg::OnDestroy();

	if(this != NULL)
		delete this;//크악~, 자살이라니!!!
}
