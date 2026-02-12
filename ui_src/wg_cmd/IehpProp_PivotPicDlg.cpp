// IehpProp_PivotPicDlg.cpp : implementation file

#include "stdafx.h"
#include "wg_cmd.h"
#include "IehpProp_PivotPicDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCIehpProp_PivotPicDlg dialog


CIehpProp_PivotPicDlg::CIehpProp_PivotPicDlg(CWnd* pParent /*=NULL*/)
	: CCMDlgBase(CIehpProp_PivotPicDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CIehpProp_PivotPicDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	SetIgnoreBottomBtnArea();
}

CIehpProp_PivotPicDlg::~CIehpProp_PivotPicDlg()
{
}

void CIehpProp_PivotPicDlg::DoDataExchange(CDataExchange* pDX)
{
	CCMDlgBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CIehpProp_PivotPicDlg)
	DDX_Control(pDX, IDC_CMD_BITMAP1_FRM, m_wndPicture);
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CIehpProp_PivotPicDlg, CCMDlgBase)
	//{{AFX_MSG_MAP(CIehpProp_PivotPicDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
//
// Redraw - Update Viewer
//
BOOL CIehpProp_PivotPicDlg::OnInitDialog() 
{
	CCMDlgBase::OnInitDialog();
	m_wndPicture.SetImage(_T("SVG\\Illustration\\Dialog\\cmd_multiLinear_pivot.svg"));
	MoveWindow();
	// TODO: Add extra initialization here
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


BOOL CIehpProp_PivotPicDlg::DestroyWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	return CCMDlgBase::DestroyWindow();
}
void CIehpProp_PivotPicDlg::MoveWindow()
{
	CRect rect;
	GetClientRect(rect);
	m_nLeftMargin = -rect.Width();

	MoveDlgToInitPos();
}