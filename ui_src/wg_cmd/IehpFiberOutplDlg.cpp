// IehpFiberOutplFiberDlg.cpp: implementation of the CIehpFiberOutplDlg class.
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "wg_cmd.h"

#include "IehpFiberOutplDlg.h"
#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_db\wg_db_AttrCtrl2.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_DBDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CIehpFiberOutplDlg::CIehpFiberOutplDlg(int nUseIehc, BOOL bConsOutpl, BOOL bPushover, CWnd* pParent /*=NULL*/ )
	: CDialogMove(CIehpFiberOutplDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CIehpFiberOutplDlg)
	// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_nUseIehc = nUseIehc;
	m_bConsOutpl = bConsOutpl;
	m_bPushover = bPushover;
	m_rectPosition.SetRectEmpty();
}

CIehpFiberOutplDlg::~CIehpFiberOutplDlg()
{  
}

void CIehpFiberOutplDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CIehpFiberOutplDlg)
	DDX_Radio(pDX, IDC_ETC_IEHP_OUTPL_FIBER_IEHC_RDO , m_nUseIehc);
	DDX_Check(pDX, IDC_ETC_IEHP_OUTPL_FIBER_CONS_NONL_CHK, m_bConsOutpl);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CIehpFiberOutplDlg, CDialogMove)
	//{{AFX_MSG_MAP(CIehpFiberOutplDlg)
	ON_BN_CLICKED(IDC_ETC_IEHP_OUTPL_FIBER_IEHC_RDO, CtrlManager)
	ON_BN_CLICKED(IDC_ETC_IEHP_OUTPL_FIBER_USER_RDO, CtrlManager)
	ON_BN_CLICKED(IDC_ETC_IEHP_OUTPL_FIBER_CONS_NONL_CHK, CtrlManager)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CIehpFiberOutplDlg message handlers
BOOL CIehpFiberOutplDlg::OnInitDialog() 
{
	CDialogMove::OnInitDialog();

	MoveDlg();
	CtrlManager();

	if(m_bPushover)
	{
		GetDlgItem(IDC_ETC_IEHP_OUTPL_FIBER_IEHC_RDO)->SetWindowText(_LS(IDS_CMD_PHGT_OUTPL_POGD_R));
	}

	return TRUE;
}


void CIehpFiberOutplDlg::OnOK() 
{
	UpdateData(TRUE);
	CDialogMove::OnOK();  
}


void CIehpFiberOutplDlg::MoveDlg()
{
	CRect rectDlg; rectDlg.SetRectEmpty();
	GetWindowRect(&rectDlg);

	SetWindowPos(NULL, m_rectPosition.left, m_rectPosition.top, rectDlg.Width(), rectDlg.Height(), SWP_NOZORDER);
}

void CIehpFiberOutplDlg::CtrlManager()
{
	UpdateData(TRUE);

	const BOOL bAuto = m_nUseIehc==0;
	if(bAuto)
	{
		if(!m_bPushover)
		{
			T_IEHC_D IehcD;
			if(!CDBDoc::GetDocPoint()->m_pAttrCtrl2->GetIehc(IehcD)) IehcD.Initialize();
			m_bConsOutpl = IehcD.bWallConsOut;
			UpdateData(FALSE);
		}
		else
		{
			T_POGD_D PogdD;
			if(!CDBDoc::GetDocPoint()->m_pAttrCtrl->GetPogd(PogdD)) PogdD.Initialize();
			m_bConsOutpl = PogdD.bWallConsOut;
			UpdateData(FALSE);
		}
	}
}