#include "stdafx.h"
#include "wg_cmd.h"
#include "MPhiSectionImageDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CMPhiSectionImageDlg::CMPhiSectionImageDlg(CWnd* pParent /*=NULL*/) 
	: CChildDialog(CMPhiSectionImageDlg::IDD, pParent)
{

}

void CMPhiSectionImageDlg::DoDataExchange(CDataExchange* pDX)
{	
	CChildDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFCMWBTendonDlg)
	DDX_Control(pDX, IDC_SW_BITMAP_STATIC, m_KeepStrDown);
	//}}AFX_DATA_MAP
}

BOOL CMPhiSectionImageDlg::OnInitDialog() 
{
	CChildDialog::OnInitDialog();	
	SetIgnoreBottomBtnArea();
	m_KeepStrDown.SetImage(_T("SVG\\Illustration\\Dialog\\MphiHingeKeepStrDown.svg"));
	return TRUE;  // return TRUE unless you set the focus to a control	
}

BEGIN_MESSAGE_MAP(CMPhiSectionImageDlg, CChildDialog)
	//{{AFX_MSG_MAP(CFCMWBTendonDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()