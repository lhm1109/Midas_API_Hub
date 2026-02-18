// DgnRebarCpgDlg.cpp : implementation file
//
// Add by sshan. ('20061227)
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "wg_dgn.h"


#include "DgnRebarCpgDlg.h"
#include "DgnRebarCpgMomentMain.h"
#include "DgnRebarCpgStiffMain.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDgnRebarCpgDlg dialog


CDgnRebarCpgDlg::CDgnRebarCpgDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CDgnRebarCpgDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDgnRebarPscDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

/*
void CDgnRebarCpgDlg::SetParamDataPsc(T_RPSC_K RpscK)
{
	m_KeyPsc = RpscK;
}*/

void CDgnRebarCpgDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDgnRebarCpgDlg)
	DDX_Control(pDX, IDC_DGN_TAB, m_ctrlTab);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDgnRebarCpgDlg, CDialogMove)
	//{{AFX_MSG_MAP(CDgnRebarCpgDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDgnRebarCpgDlg message handlers

BOOL CDgnRebarCpgDlg::OnInitDialog() 
{
	CDialogMove::OnInitDialog();
		
	// TODO: Add extra initialization here
	this->LayoutChildDlg();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDgnRebarCpgDlg::OnCancel()
{
	EndDialog(IDCANCEL);
}

/////////////////////////////////////////////////////////////////////////////
// User Defined Function

void CDgnRebarCpgDlg::LayoutChildDlg()
{
	//CDgnRebarCpgMomentMain* pMomentDlg;
	CDgnRebarCpgStiffMain* pStiffDlg;

	CWnd* pPlaceHolder = GetDlgItem(IDC_DGN_PLACEHOLDER);
	ASSERT(pPlaceHolder);
	m_ctrlTab.SetPlaceHolder(pPlaceHolder);
	
	int nInitShow = 0;

	//pMomentDlg = new CDgnRebarCpgMomentMain(this);
	pStiffDlg = new CDgnRebarCpgStiffMain(this);

	/*
	if (m_KeyPsc != 0) 
	{
		m_pPscDlg->SetParamData(m_KeyPsc);
	}*/

	//m_ctrlTab.AddTab(pMomentDlg, _LS(IDS_DGN_CPG_MAIN_REBAR), CDgnRebarCpgMomentMain::IDD, TRUE);
	m_ctrlTab.AddTab(pStiffDlg, _LS(IDS_DGN_CPG_STIFF_REBAR), CDgnRebarCpgStiffMain::IDD, TRUE);
	
	m_ctrlTab.ShowTab(nInitShow);
}
