// IehpStiffnessOptionDlg.cpp: implementation of the CIehpStiffnessOptionDlg class.
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "wg_cmd.h"

#include "IehpStiffnessOptionDlg.h"
#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_db\wg_db_AttrCtrl2.h"
#include "..\wg_db\wg_db_DBDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CIehpStiffnessOptionDlg::CIehpStiffnessOptionDlg(BOOL bUseNonlinearShearY, BOOL bUseNonlinearShearZ, CWnd* pParent /*=NULL*/)
	: CDialogMove(CIehpStiffnessOptionDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CIehpStiffnessOptionDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_bUseNonlinearShearY = bUseNonlinearShearY;
	m_bUseNonlinearShearZ = bUseNonlinearShearZ;
	m_rectPosition.SetRectEmpty();
}

CIehpStiffnessOptionDlg::~CIehpStiffnessOptionDlg()
{
}

void CIehpStiffnessOptionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CIehpStiffnessOptionDlg)
	DDX_Check(pDX, IDC_CMD_NONLINEAR_SHAER_Y_CHK, m_bUseNonlinearShearY);
	DDX_Check(pDX, IDC_CMD_NONLINEAR_SHAER_Z_CHK, m_bUseNonlinearShearZ);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CIehpStiffnessOptionDlg, CDialogMove)
	//{{AFX_MSG_MAP(CIehpStiffnessOptionDlg)

	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CIehpStiffnessOptionDlg message handlers
BOOL CIehpStiffnessOptionDlg::OnInitDialog()
{
	CDialogMove::OnInitDialog();

	MoveDlg();
	return TRUE;
}

void CIehpStiffnessOptionDlg::OnOK()
{
	UpdateData(TRUE);
	CDialogMove::OnOK();
}

void CIehpStiffnessOptionDlg::MoveDlg()
{
	CRect rectDlg; rectDlg.SetRectEmpty();
	GetWindowRect(&rectDlg);

	SetWindowPos(NULL, m_rectPosition.left, m_rectPosition.top, rectDlg.Width(), rectDlg.Height(), SWP_NOZORDER);
}