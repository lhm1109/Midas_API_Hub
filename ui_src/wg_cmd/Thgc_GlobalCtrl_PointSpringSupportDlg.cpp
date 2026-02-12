// Thgc_GlobalCtrl_PointSpringSupportDlg.cpp: implementation of the CThgc_GlobalCtrl_PointSpringSupportDlg class.
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "wg_cmd.h"
#include "Thgc_GlobalCtrl_PointSpringSupportDlg.h"

#include "..\wg_base\wg_base_DlgUtil.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CThgc_GlobalCtrl_PointSpringSupportDlg::CThgc_GlobalCtrl_PointSpringSupportDlg(CWnd* pParent /*=NULL*/)
	: CDlgChild(CThgc_GlobalCtrl_PointSpringSupportDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CThgc_GlobalCtrl_PointSpringSupportDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_nPointSpringSupport = 0;
	m_nElasticLink = 0;
	m_rectPosition.SetRectEmpty();

	m_aCtrlRdo.RemoveAll();
	m_aCtrlRdo.Add(IDC_PO_GLOBAL_POINT_SPRING_SUPPORT_RDO1);
	m_aCtrlRdo.Add(IDC_PO_GLOBAL_POINT_SPRING_SUPPORT_RDO2);
	m_aCtrlRdo.FreeExtra();

	m_aCtrlRdo2.RemoveAll();
	m_aCtrlRdo2.Add(IDC_PO_GLOBAL_ELASTIC_LINK_RDO1);
	m_aCtrlRdo2.Add(IDC_PO_GLOBAL_ELASTIC_LINK_RDO2);
	m_aCtrlRdo2.FreeExtra();
}

CThgc_GlobalCtrl_PointSpringSupportDlg::~CThgc_GlobalCtrl_PointSpringSupportDlg()
{
}

void CThgc_GlobalCtrl_PointSpringSupportDlg::DoDataExchange(CDataExchange* pDX)
{
	CDlgChild::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDgnLinkDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CThgc_GlobalCtrl_PointSpringSupportDlg, CDlgChild)
	//{{AFX_MSG_MAP(CDgnLinkDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CThgc_GlobalCtrl_PointSpringSupportDlg message handlers
BOOL CThgc_GlobalCtrl_PointSpringSupportDlg::OnInitDialog()
{
	CDlgChild::OnInitDialog();

	MoveDlg();

	Data2Dlg();
	return TRUE;
}

void CThgc_GlobalCtrl_PointSpringSupportDlg::OnOK()
{
	Dlg2Data();
	CDlgChild::OnOK();
}

void CThgc_GlobalCtrl_PointSpringSupportDlg::Data2Dlg()
{
	CDlgUtil::CtrlRadioSetCheck(this, m_aCtrlRdo, m_nPointSpringSupport);
	CDlgUtil::CtrlRadioSetCheck(this, m_aCtrlRdo2, m_nElasticLink);
}

void CThgc_GlobalCtrl_PointSpringSupportDlg::Dlg2Data()
{
	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlRdo, m_nPointSpringSupport);
	CDlgUtil::CtrlRadioGetCheck(this, m_aCtrlRdo2, m_nElasticLink);
}

void CThgc_GlobalCtrl_PointSpringSupportDlg::SetData(int nPointSpringSupport, int nElasticLink)
{
	m_nPointSpringSupport = nPointSpringSupport;
	m_nElasticLink = nElasticLink;
}

void CThgc_GlobalCtrl_PointSpringSupportDlg::GetData(int &nPointSpringSupport, int &nElasticLink)
{
	nPointSpringSupport = m_nPointSpringSupport;
	nElasticLink = m_nElasticLink;
}

void CThgc_GlobalCtrl_PointSpringSupportDlg::MoveDlg()
{
	CRect rectDlg; rectDlg.SetRectEmpty();
	GetWindowRect(&rectDlg);

	int y = m_rectPosition.top - rectDlg.Height() - m_rectPosition.Height() - globalUtils.ScaleByDPI(5);
	SetWindowPos(NULL, m_rectPosition.left, y,
		rectDlg.Width(), rectDlg.Height(), SWP_NOZORDER);
}