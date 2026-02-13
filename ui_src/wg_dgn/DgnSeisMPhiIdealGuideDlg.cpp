// DgnSeisMPhiIdealGuideDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnSeisMPhiIdealGuideDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDgnSeisMPhiIdealGuideDlg dialog

CDgnSeisMPhiIdealGuideDlg::CDgnSeisMPhiIdealGuideDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CDgnSeisMPhiIdealGuideDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDgnSeisMPhiIdealGuideDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_nIdealOpt = 0;
}

CDgnSeisMPhiIdealGuideDlg::~CDgnSeisMPhiIdealGuideDlg()
{
}

void CDgnSeisMPhiIdealGuideDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDgnSeisMPhiIdealGuideDlg)
	DDX_Radio  (pDX, IDC_DGN_SEIS_MPHI_IDEAL_OPT1_RDO,         m_nIdealOpt);
	DDX_Control(pDX, IDC_DGN_SEIS_MPHI_IDEAL_BMP, m_wndPicture);
	//}}AFX_DATA_MAP
}

void CDgnSeisMPhiIdealGuideDlg::ChangeBitmap()
{
	UpdateData(TRUE);

	CString bitmapPath[] = 
	{
		_T("SVG\\Illustration\\Dialog\\dgn_Seis_Mphi_Ideal_1.svg"),
		_T("SVG\\Illustration\\Dialog\\dgn_Seis_Mphi_Ideal_2.svg"),
		_T("SVG\\Illustration\\Dialog\\dgn_Seis_Mphi_Ideal_3.svg")
	};
	if (m_nIdealOpt == 0)   m_wndPicture.SetImage(bitmapPath[m_nIdealOpt]);
	else if (m_nIdealOpt == 1)   m_wndPicture.SetImage(bitmapPath[m_nIdealOpt]);
	else if (m_nIdealOpt == 2)   m_wndPicture.SetImage(bitmapPath[m_nIdealOpt]);
	else ASSERT(0);

}

/////////////////////////////////////////////////////////////////////////////
// CDgnSeisMPhiIdealGuideDlg implementation functions
BEGIN_MESSAGE_MAP(CDgnSeisMPhiIdealGuideDlg, CDialogMove)
	//{{AFX_MSG_MAP(CDgnSeisMPhiIdealGuideDlg)
	ON_BN_CLICKED(IDC_DGN_SEIS_MPHI_IDEAL_OPT1_RDO,       OnDgnSeisMPhiIdealOptRdo)
	ON_BN_CLICKED(IDC_DGN_SEIS_MPHI_IDEAL_OPT2_RDO,       OnDgnSeisMPhiIdealOptRdo)
	ON_BN_CLICKED(IDC_DGN_SEIS_MPHI_IDEAL_OPT3_RDO,       OnDgnSeisMPhiIdealOptRdo)
	ON_BN_CLICKED(IDC_DGN_SEIS_MPHI_IDEAL_CLOSE_BTN,      OnDgnSeisMPhiIdealCloseBtn)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDgnSeisMPhiIdealGuideDlg message handlers

BOOL CDgnSeisMPhiIdealGuideDlg::OnInitDialog() 
{
	CDialogMove::OnInitDialog();	

	OnDgnSeisMPhiIdealOptRdo();
		
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDgnSeisMPhiIdealGuideDlg::OnDgnSeisMPhiIdealOptRdo()
{
	ChangeBitmap();
}

void CDgnSeisMPhiIdealGuideDlg::OnDgnSeisMPhiIdealCloseBtn()
{
	CDialogMove::OnCancel();
}
