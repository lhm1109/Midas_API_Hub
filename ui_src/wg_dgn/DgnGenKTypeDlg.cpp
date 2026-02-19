// DgnGenKTypeDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnGenKTypeDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDgnGenKTypeDlg dialog


CDgnGenKTypeDlg::CDgnGenKTypeDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CDgnGenKTypeDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDgnGenKTypeDlg)
	m_nKMode = -1;
	//}}AFX_DATA_INIT
}

void CDgnGenKTypeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDgnGenKTypeDlg)
	DDX_Radio(pDX, IDC_DGN_KINPUT_065, m_nKMode);
	DDX_Control(pDX, IDC_WG_DGN_STATIC0, m_wndPicture);
	//}}AFX_DATA_MAP
}

#define CDialog CDialogMove
BEGIN_MESSAGE_MAP(CDgnGenKTypeDlg, CInternationalDlg)
#undef CDialog

	//{{AFX_MSG_MAP(CDgnGenKTypeDlg)
	ON_BN_CLICKED(IDC_DGN_KINPUT_CLOSE, OnDgnKinputClose)
	ON_BN_CLICKED(IDC_DGN_KINPUT_OK, OnDgnKinputOk)
	ON_BN_CLICKED(IDC_DGN_KINPUT_065, OnDgnKinput065)
	ON_BN_CLICKED(IDC_DGN_KINPUT_08, OnDgnKinput08)
	ON_BN_CLICKED(IDC_DGN_KINPUT_10, OnDgnKinput10)
	ON_BN_CLICKED(IDC_DGN_KINPUT_12, OnDgnKinput12)
	ON_BN_CLICKED(IDC_DGN_KINPUT_20, OnDgnKinput20)
	ON_BN_CLICKED(IDC_DGN_KINPUT_21, OnDgnKinput21)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDgnGenKTypeDlg message handlers

BOOL CDgnGenKTypeDlg::OnInitDialog() 
{
	CDialogMove::OnInitDialog();

	Initial_Data();

	m_wndPicture.SetIgnoreLargeView();
	m_wndPicture.SetImage(_T("SVG\\Illustration\\Dialog\\dgn_k_factor.svg"));

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDgnGenKTypeDlg::Initial_Data()
{
	if(m_K == 0.65)     m_nKMode = 0;
	else if(m_K == 0.8)	m_nKMode = 1;
	else if(m_K == 1.0)	m_nKMode = 3;
	else if(m_K == 1.2)	m_nKMode = 2;
	else if(m_K == 2.0)	m_nKMode = 5;
	else if(m_K == 2.1)	m_nKMode = 4;
	else
	{
//		m_K = 1.0;
//		m_nKMode = 3;
	}

	UpdateData(FALSE);
}

void CDgnGenKTypeDlg::OnDgnKinputClose() 
{
	// TODO: Add your control notification handler code here
	CDialogMove::OnCancel();
}

void CDgnGenKTypeDlg::OnDgnKinputOk() 
{
	// TODO: Add your control notification handler code here
	CDialogMove::OnOK();
}

void CDgnGenKTypeDlg::OnDgnKinput065() 
{
	// TODO: Add your control notification handler code here
	m_K = 0.65;
	m_nKMode = 0;

	UpdateData(FALSE);
}

void CDgnGenKTypeDlg::OnDgnKinput08() 
{
	// TODO: Add your control notification handler code here
	m_K = 0.8;
	m_nKMode = 1;

	UpdateData(FALSE);
}

void CDgnGenKTypeDlg::OnDgnKinput10() 
{
	// TODO: Add your control notification handler code here
	m_K = 1.0;
	m_nKMode = 3;

	UpdateData(FALSE);
}

void CDgnGenKTypeDlg::OnDgnKinput12() 
{
	// TODO: Add your control notification handler code here
	m_K = 1.2;
	m_nKMode = 2;

	UpdateData(FALSE);
}

void CDgnGenKTypeDlg::OnDgnKinput20() 
{
	// TODO: Add your control notification handler code here
	m_K = 2.0;
	m_nKMode = 5;

	UpdateData(FALSE);
}

void CDgnGenKTypeDlg::OnDgnKinput21() 
{
	// TODO: Add your control notification handler code here
	m_K = 2.1;
	m_nKMode = 4;

	UpdateData(FALSE);
}
