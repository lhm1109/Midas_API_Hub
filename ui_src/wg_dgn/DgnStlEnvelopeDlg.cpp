// DgnStlEnvelopeDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnStlEnvelopeDlg.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"

#include "..\wg_db\wg_db_InitCtrl.h"
#include "..\wg_db\wg_db_DBLib.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_base\wg_base_LocaleLib.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDgnStlEnvelopeDlg dialog


CDgnStlEnvelopeDlg::CDgnStlEnvelopeDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CDgnStlEnvelopeDlg::IDD, pParent)
{
	m_EnvelopeData.Initialize();
	//{{AFX_DATA_INIT(CDgnStlEnvelopeDlg)	
	//}}AFX_DATA_INIT
}

void CDgnStlEnvelopeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDgnStlEnvelopeDlg)
	DDX_Control(pDX, IDC_DGN_CHECK1,	m_chkPmax);	
	DDX_Control(pDX, IDC_DGN_CHECK2,	m_chkPmin);
	DDX_Control(pDX, IDC_DGN_CHECK3,	m_chkVymax);
	DDX_Control(pDX, IDC_DGN_CHECK4,	m_chkVymin);
	DDX_Control(pDX, IDC_DGN_CHECK5,	m_chkVzmax);
	DDX_Control(pDX, IDC_DGN_CHECK6,	m_chkVzmin);
	DDX_Control(pDX, IDC_DGN_CHECK7,	m_chkTmax);
	DDX_Control(pDX, IDC_DGN_CHECK8,	m_chkTmin);
	DDX_Control(pDX, IDC_DGN_CHECK9,	m_chkMymax);
	DDX_Control(pDX, IDC_DGN_CHECK10, m_chkMymin);
	DDX_Control(pDX, IDC_DGN_CHECK11, m_chkMzmax);
	DDX_Control(pDX, IDC_DGN_CHECK12, m_chkMzmin);

	DDX_Control(pDX, IDC_DGN_SELECT_ALL_BTN, m_btnSelectAll);
	DDX_Control(pDX, IDC_DGN_UNSELECT_ALL_BTN, m_btnUnSelectAll);
	//}}AFX_DATA_MAP	
}

#define CDialog CDialogMove
BEGIN_MESSAGE_MAP(CDgnStlEnvelopeDlg, CInternationalDlg)
#undef CDialog
	//{{AFX_MSG_MAP(CDgnStlEnvelopeDlg)	
	ON_BN_CLICKED(IDC_DGN_SELECT_ALL_BTN, OnDgnSelectAllBtn)
	ON_BN_CLICKED(IDC_DGN_UNSELECT_ALL_BTN, OnDgnUnSelectAllBtn)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDgnStlEnvelopeDlg message handlers

BOOL CDgnStlEnvelopeDlg::OnInitDialog() 
{
	CDialogMove::OnInitDialog();

	m_chkPmax.SetCheck(m_EnvelopeData.bP_max);
	m_chkPmin.SetCheck(m_EnvelopeData.bP_min);
	m_chkVymax.SetCheck(m_EnvelopeData.bVy_max);
	m_chkVymin.SetCheck(m_EnvelopeData.bVy_min);
	m_chkVzmax.SetCheck(m_EnvelopeData.bVz_max);
	m_chkVzmin.SetCheck(m_EnvelopeData.bVz_min);
	m_chkTmax.SetCheck(m_EnvelopeData.bT_max);
	m_chkTmin.SetCheck(m_EnvelopeData.bT_min);
	m_chkMymax.SetCheck(m_EnvelopeData.bMy_max);
	m_chkMymin.SetCheck(m_EnvelopeData.bMy_min);
	m_chkMzmax.SetCheck(m_EnvelopeData.bMz_max);
	m_chkMzmin.SetCheck(m_EnvelopeData.bMz_min); 

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CDgnStlEnvelopeDlg::OnOK() 
{
	m_EnvelopeData.bP_max		= m_chkPmax.GetCheck();
	m_EnvelopeData.bP_min		= m_chkPmin.GetCheck();
	m_EnvelopeData.bVy_max	= m_chkVymax.GetCheck();
	m_EnvelopeData.bVy_min	= m_chkVymin.GetCheck();
	m_EnvelopeData.bVz_max	= m_chkVzmax.GetCheck();
	m_EnvelopeData.bVz_min	= m_chkVzmin.GetCheck();
	m_EnvelopeData.bT_max		= m_chkTmax.GetCheck();
	m_EnvelopeData.bT_min		= m_chkTmin.GetCheck();
	m_EnvelopeData.bMy_max	= m_chkMymax.GetCheck();
	m_EnvelopeData.bMy_min	= m_chkMymin.GetCheck();
	m_EnvelopeData.bMz_max	= m_chkMzmax.GetCheck();
	m_EnvelopeData.bMz_min	= m_chkMzmin.GetCheck(); 

	CDialogMove::OnOK();
}

void CDgnStlEnvelopeDlg::OnDgnSelectAllBtn() 
{
	m_chkPmax.SetCheck(TRUE);
	m_chkPmin.SetCheck(TRUE);
	m_chkVymax.SetCheck(TRUE);
	m_chkVymin.SetCheck(TRUE);
	m_chkVzmax.SetCheck(TRUE);
	m_chkVzmin.SetCheck(TRUE);
	m_chkTmax.SetCheck(TRUE);
	m_chkTmin.SetCheck(TRUE);
	m_chkMymax.SetCheck(TRUE);
	m_chkMymin.SetCheck(TRUE);
	m_chkMzmax.SetCheck(TRUE);
	m_chkMzmin.SetCheck(TRUE);
}

void CDgnStlEnvelopeDlg::OnDgnUnSelectAllBtn() 
{
	m_chkPmax.SetCheck(FALSE);
	m_chkPmin.SetCheck(FALSE);
	m_chkVymax.SetCheck(FALSE);
	m_chkVymin.SetCheck(FALSE);
	m_chkVzmax.SetCheck(FALSE);
	m_chkVzmin.SetCheck(FALSE);
	m_chkTmax.SetCheck(FALSE);
	m_chkTmin.SetCheck(FALSE);
	m_chkMymax.SetCheck(FALSE);
	m_chkMymin.SetCheck(FALSE);
	m_chkMzmax.SetCheck(FALSE);
	m_chkMzmin.SetCheck(FALSE);
}