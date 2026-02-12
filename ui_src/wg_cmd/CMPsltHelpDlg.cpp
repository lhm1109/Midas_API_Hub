// CMPsltHelpDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMPsltHelpDlg.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_AttrCtrl2.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCMPsltHelpDlg dialog

CCMPsltHelpDlg::CCMPsltHelpDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CCMPsltHelpDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCMPsltHelpDlg)
	//}}AFX_DATA_INIT
}

CCMPsltHelpDlg::~CCMPsltHelpDlg()
{
}

void CCMPsltHelpDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMPsltHelpDlg)
	//DDX_Control(pDX, IDC_CMD_PRES_ELTYPE_CMB, m_cmbElemType);
	DDX_Control(pDX, IDC_CMD_PICTURE, m_wndPicture);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CCMPsltHelpDlg, CDialogMove)
	//{{AFX_MSG_MAP(CCMPsltHelpDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMPsltHelpDlg message handlers

BOOL CCMPsltHelpDlg::OnInitDialog() 
{
	CDialogMove::OnInitDialog();

	SetIgnoreBottomBtnArea();

	SetBitmap();

	CString strTitle(CDBDoc::GetDocPoint()->m_pAttrCtrl2->GetPsltElemTypeStr(m_nElemType));
	this->SetWindowText(strTitle);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CCMPsltHelpDlg::SetElemType(int nElemType)
{
	m_nElemType = nElemType;
}


void CCMPsltHelpDlg::SetBitmap() 
{
	const int PLATESTRESS_FACE=0;
	const int PLATESTRESS_EDGE=1;
	const int SOLID_FACE      =2;
	const int PLAINSTRAIN_EDGE=3;
	const int AXISY_EDGE      =4;
	const int WALL_EDGE       =5;

	CString strSVG;
	switch(m_nElemType)
	{
	case PLATESTRESS_FACE:
		strSVG = _T("SVG\\Illustration\\Dialog\\tm_pres1.svg");//IDB_CMD_PRES1;
		break;
	case PLATESTRESS_EDGE:
	case WALL_EDGE :
		strSVG = _T("SVG\\Illustration\\Dialog\\tm_pres2.svg");//IDB_CMD_PRES2;
		break;
	case SOLID_FACE:
		strSVG = _T("SVG\\Illustration\\Dialog\\tm_pres3.svg");//IDB_CMD_PRES3;
		break;
	case PLAINSTRAIN_EDGE:
	case AXISY_EDGE:
		strSVG = _T("SVG\\Illustration\\Dialog\\tm_pres6.svg");//IDB_CMD_PRES6;
		break;
	}

	m_wndPicture.SetImage(strSVG);
	m_wndPicture.SetIgnoreLargeView();
}