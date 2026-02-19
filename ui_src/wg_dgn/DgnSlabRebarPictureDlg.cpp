
#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnSlabRebarPictureDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDgnSlabRebarPictureDlg dialog


CDgnSlabRebarPictureDlg::CDgnSlabRebarPictureDlg(CWnd* pParent /*=NULL*/)
	: CModelessMoveDlg(CDgnSlabRebarPictureDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDgnSlabRebarPictureDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	SetIgnoreBottomBtnArea();
}

CDgnSlabRebarPictureDlg::~CDgnSlabRebarPictureDlg()
{

}

void CDgnSlabRebarPictureDlg::DoDataExchange(CDataExchange* pDX)
{
	CModelessMoveDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDgnSlabRebarPictureDlg)
	DDX_Control(pDX, IDC_DGN_SLAB_REBAR_STC, m_wndPicture);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDgnSlabRebarPictureDlg, CModelessMoveDlg)
	//{{AFX_MSG_MAP(CDgnSlabRebarPictureDlg)
	ON_WM_SYSCOMMAND()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDgnSlabRebarPictureDlg message handlers

/////////////////////////////////////////////////////////////////////////////
BOOL CDgnSlabRebarPictureDlg::OnInitDialog() 
{
	CModelessMoveDlg::OnInitDialog();

#ifdef _CIVIL
	SetWindowText(_LS(IDS_DGN_SHELL));
#endif // _CIVIL

	m_wndPicture.SetImage(_T("SVG\\Illustration\\Dialog\\dgn_slab_rebar.svg"));
	m_wndPicture.SetIgnoreLargeView(TRUE);

	// TODO: Add extra initialization here

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDgnSlabRebarPictureDlg::OnSysCommand(UINT nID, LPARAM lParam) 
{
	// TODO: Add your message handler code here and/or call default
	// Close를 막아준다.
	if(nID == SC_CLOSE)
	{
		return;
	}

	CModelessMoveDlg::OnSysCommand(nID, lParam);
}

BOOL CDgnSlabRebarPictureDlg::DestroyWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CModelessMoveDlg::DestroyWindow();
}
