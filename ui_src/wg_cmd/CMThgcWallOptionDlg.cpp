#include "stdafx.h"

#include "wg_cmd.h"

#include "CMThgcWallOptionDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CMThgcWallOptionDlg::CMThgcWallOptionDlg(CWnd* pParent /*=NULL*/)
	: CDlgChild(CMThgcWallOptionDlg::IDD, pParent)
{
	m_rectPosition.SetRectEmpty();
	m_chkWallStiffnessConvert = TRUE;
}

CMThgcWallOptionDlg::~CMThgcWallOptionDlg()
{
}

void CMThgcWallOptionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDlgChild::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPO_GlobalCtrl_WallNodeConnectivityDlg)
	DDX_Check(pDX, IDC_THGC_GLOBAL_WALL_STIFF_CONVERT_CHK, m_chkWallStiffnessConvert);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CMThgcWallOptionDlg, CDlgChild)
	//{{AFX_MSG_MAP(CPO_GlobalCtrl_WallNodeConnectivityDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPO_GlobalCtrl_WallNodeConnectivityDlg message handlers
BOOL CMThgcWallOptionDlg::OnInitDialog()
{
	CDlgChild::OnInitDialog();

	MoveDlg();
	Data2Dlg();

	return TRUE;
}

void CMThgcWallOptionDlg::OnOK()
{
	Dlg2Data();
	CDlgChild::OnOK();
}

void CMThgcWallOptionDlg::Data2Dlg()
{
	UpdateData(FALSE);
}

void CMThgcWallOptionDlg::Dlg2Data()
{
	UpdateData();
}

void CMThgcWallOptionDlg::MoveDlg()
{
	CRect rectDlg; rectDlg.SetRectEmpty();
	GetWindowRect(&rectDlg);

	SetWindowPos(NULL, m_rectPosition.left, m_rectPosition.top - rectDlg.Height(),
		rectDlg.Width(), rectDlg.Height(), SWP_NOZORDER);
}

void CMThgcWallOptionDlg::SetData(BOOL bchkWallStiffnessConvert)
{
	m_chkWallStiffnessConvert = bchkWallStiffnessConvert;
}

void CMThgcWallOptionDlg::GetData(BOOL& bchkWallStiffnessConvert) const
{
	bchkWallStiffnessConvert = m_chkWallStiffnessConvert;
}