#include "stdafx.h"
#include "wg_cmd.h"
#include "CMDTendonTemplateViewDlg.h"
#include "DgnTendonTemplateData.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCMDTendonTemplateViewDlg dialog

CCMDTendonTemplateViewDlg::CCMDTendonTemplateViewDlg(CWnd* pParent /*=NULL*/)
	: CCMDlgBase(CCMDTendonTemplateViewDlg::IDD, pParent)
{
	m_arResource[TNDN_TEMPLATE_TYPE_STRAIGHT]        = _T("tndnTemplate_01_straight.svg");
	m_arResource[TNDN_TEMPLATE_TYPE_STRAIGHT_BENT_1] = _T("tndnTemplate_02_bent1.svg");
	m_arResource[TNDN_TEMPLATE_TYPE_STRAIGHT_BENT_2] = _T("tndnTemplate_03_bent2.svg");
	m_arResource[TNDN_TEMPLATE_TYPE_CURVE_LINE]      = _T("tndnTemplate_01_straight.svg"); //일단 똑같이...
	m_arResource[TNDN_TEMPLATE_TYPE_CURVE_SIMPLE_1]  = _T("tndnTemplate_05_curve2.svg");
	m_arResource[TNDN_TEMPLATE_TYPE_CURVE_SIMPLE_2]  = _T("tndnTemplate_06_curve3.svg");
	m_arResource[TNDN_TEMPLATE_TYPE_CURVE_BENT_1]    = _T("tndnTemplate_07_curve4.svg");
	m_arResource[TNDN_TEMPLATE_TYPE_CURVE_ST]        = _T("tndnTemplate_01_straight.svg");

	m_nCurType = TNDN_TEMPLATE_TYPE_STRAIGHT;
}

CCMDTendonTemplateViewDlg::~CCMDTendonTemplateViewDlg()
{
}

void CCMDTendonTemplateViewDlg::DoDataExchange(CDataExchange* pDX)
{
	CCMDlgBase::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_WG_CMD_STATIC0, m_wndPicture1);
	DDX_Control(pDX, IDC_CMD_TENDON_TEMPLATE_VIEW_BMP, m_wndPicture2);
}

BEGIN_MESSAGE_MAP(CCMDTendonTemplateViewDlg, CCMDlgBase)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMDTendonTemplateViewDlg message handlers


/////////////////////////////////////////////////////////////////////////////
// Change Bitmap
//
BOOL CCMDTendonTemplateViewDlg::ChangeBitmap(UINT nType)
{
	BOOL bResult = FALSE;
	if (m_nCurType == nType)          return TRUE;  // 같은 bitmap이면 Load 안함
	if (nType > 7) return TRUE;

	CString strSVGPath = _T("SVG\\Illustration\\Dialog\\");
	m_wndPicture2.SetImage(strSVGPath + m_arResource[nType]);
	m_nCurType = nType; // 같은 bitmap이면 Load 안하기 위해 현재 Bitmap ID 저장
 
	return bResult;
}

BOOL CCMDTendonTemplateViewDlg::OnInitDialog() 
{
	CCMDlgBase::OnInitDialog();

	SetIgnoreBottomBtnArea();

	m_wndPicture1.SetImage(_T("SVG\\Illustration\\Dialog\\tndnTemplate_00_horz.svg"));
	m_wndPicture2.SetImage(_T("SVG\\Illustration\\Dialog\\tndnTemplate_01_straight.svg"));

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCMDTendonTemplateViewDlg::OnSysCommand(UINT nID, LPARAM lParam) 
{
	// Close를 막아준다.
	if(nID == SC_CLOSE)
	{
		return;
	}

	CCMDlgBase::OnSysCommand(nID, lParam);
}

void CCMDTendonTemplateViewDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	ChangeBitmap(m_nCurType);

	// Do not call CCMDlgBase::OnPaint() for painting messages
}

BOOL CCMDTendonTemplateViewDlg::DestroyWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CCMDlgBase::DestroyWindow();
}
