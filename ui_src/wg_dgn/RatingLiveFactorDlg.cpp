// RatingLiveFactorDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "RatingLiveFactorDlg.h"

#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_base\CheckDialogOpen.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// RatingLiveFactorDlg dialog


RatingLiveFactorDlg::RatingLiveFactorDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(RatingLiveFactorDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(RatingLiveFactorDlg)
	m_iFactor = -1;
	//}}AFX_DATA_INIT

	m_dPrimary = 1.0;
	m_dAdjacent = 1.0;

	m_iLimitState = 0;
	m_nRatingType = 0;
}


void RatingLiveFactorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(RatingLiveFactorDlg)
	DDX_Radio(pDX, IDC_DGN_LIVE_FACTOR_RADIO1, m_iFactor);
	DDX_Control(pDX, IDC_DGN_LIVE_FACTOR_STATIC03, m_ctrlPrimary_P5);
	DDX_Control(pDX, IDC_DGN_LIVE_FACTOR_STATIC4, m_ctrlPrimary_P7);
	DDX_Control(pDX, IDC_DGN_LIVE_FACTOR_STATIC6, m_ctrlPrimary_P9);
	DDX_Control(pDX, IDC_DGN_LIVE_FACTOR_STATIC8,	m_ctrlPrimary_P11);
	DDX_Control(pDX, IDC_DGN_LIVE_FACTOR_STATIC10, m_ctrlPrimary_P13);
	DDX_Control(pDX, IDC_DGN_LIVE_FACTOR_STATIC12, m_ctrlPrimary_User);
	DDX_Control(pDX, IDC_DGN_LIVE_FACTOR_STATIC14, m_ctrlPrimary_P15);
	DDX_Control(pDX, IDC_DGN_LIVE_FACTOR_STATIC16, m_ctrlPrimary_HL93I);
	DDX_Control(pDX, IDC_DGN_LIVE_FACTOR_STATIC18, m_ctrlPrimary_HL93O);

	DDX_Control(pDX, IDC_DGN_LIVE_FACTOR_STATIC17, m_ctrlAdjacent_HL93I);
	DDX_Control(pDX, IDC_DGN_LIVE_FACTOR_STATIC19, m_ctrlAdjacent_HL93O);
	//}}AFX_DATA_MAP
}

BOOL RatingLiveFactorDlg::OnInitDialog() 
{
	CDialogMove::OnInitDialog();

	InitCtrl();
	SetEditCtrl();	

	CCheckDialogOpen::SetSoftwareRendering();


	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


BEGIN_MESSAGE_MAP(RatingLiveFactorDlg, CDialogMove)
	//{{AFX_MSG_MAP(RatingLiveFactorDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// RatingLiveFactorDlg message handlers

void RatingLiveFactorDlg::InitCtrl()
{
	if(m_nRatingType==0 && m_iLimitState == 2) // Steel & Fatigue : STL_AASHTO_LRFD19
	{
		// CTEXT           _T("Primary Vehicle Factor"),IDC_DGN_LIVE_FACTOR_STATIC01,72,7,85,16,SS_CENTERIMAGE | SS_SUNKEN
		// CTEXT           _T("Adjacent Vehicle Factor"),IDC_DGN_LIVE_FACTOR_STATIC02,157,7,85,16,SS_CENTERIMAGE | SS_SUNKEN
		// CONTROL         _T("P-5"),IDC_DGN_LIVE_FACTOR_RADIO1,_T("Button"),BS_AUTORADIOBUTTON | BS_VCENTER | BS_PUSHLIKE | WS_GROUP,4,23,68,16,WS_EX_STATICEDGE
		// CONTROL         _T("P-7"),IDC_DGN_LIVE_FACTOR_RADIO2,_T("Button"),BS_AUTORADIOBUTTON | BS_VCENTER | BS_PUSHLIKE,4,39,68,16,WS_EX_STATICEDGE
		// CTEXT           _T("1.6"),IDC_DGN_LIVE_FACTOR_STATIC03,72,23,85,16,SS_CENTERIMAGE | SS_SUNKEN
		// CTEXT           _T("1.0"),IDC_DGN_LIVE_FACTOR_STATIC04,157,23,85,16,SS_CENTERIMAGE | SS_SUNKEN
		// CTEXT           _T("1.6"),IDC_DGN_LIVE_FACTOR_STATIC4,72,39,85,16,SS_CENTERIMAGE | SS_SUNKEN
		// CTEXT           _T("1.0"),IDC_DGN_LIVE_FACTOR_STATIC5,157,39,85,16,SS_CENTERIMAGE | SS_SUNKEN

		GetDlgItem(IDC_DGN_LIVE_FACTOR_STATIC01)->SetWindowText(_T("LRFD"));
		GetDlgItem(IDC_DGN_LIVE_FACTOR_STATIC02)->SetWindowText(_T("Truck Survey"));
		GetDlgItem(IDC_DGN_LIVE_FACTOR_RADIO1)->SetWindowText(_T("Infinite Life"));
		GetDlgItem(IDC_DGN_LIVE_FACTOR_RADIO2)->SetWindowText(_T("Finite Life"));

		GetDlgItem(IDC_DGN_LIVE_FACTOR_RADIO3)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_LIVE_FACTOR_RADIO4)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_LIVE_FACTOR_RADIO5)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_LIVE_FACTOR_RADIO6)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_LIVE_FACTOR_RADIO7)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_LIVE_FACTOR_RADIO8)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_LIVE_FACTOR_RADIO9)->ShowWindow(SW_HIDE);
		
		GetDlgItem(IDC_DGN_LIVE_FACTOR_STATIC6 )->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_LIVE_FACTOR_STATIC7 )->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_LIVE_FACTOR_STATIC8 )->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_LIVE_FACTOR_STATIC9 )->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_LIVE_FACTOR_STATIC10)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_LIVE_FACTOR_STATIC11)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_LIVE_FACTOR_STATIC12)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_LIVE_FACTOR_STATIC13)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_LIVE_FACTOR_STATIC14)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_LIVE_FACTOR_STATIC15)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_LIVE_FACTOR_STATIC16)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_LIVE_FACTOR_STATIC17)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_LIVE_FACTOR_STATIC18)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_LIVE_FACTOR_STATIC19)->ShowWindow(SW_HIDE);

		// Move Ctrl
		CRect rRef;
		CRect rToMove;
		int nDistX, nDistY;
		CArray<UINT, UINT> aControls;

		aControls.RemoveAll();
		aControls.Add(IDOK);
		aControls.Add(IDCANCEL);
		GetDlgItem(IDC_DGN_LIVE_FACTOR_STATIC5)->GetWindowRect(rRef);
		GetDlgItem(IDOK                       )->GetWindowRect(rToMove);
		nDistX = 0.0;
		nDistY = rRef.bottom - rToMove.top + globalUtils.ScaleByDPI(10);
		CDlgUtil::CtrlMoveDistXY(this, aControls, nDistX, nDistY);

		// resize dialog
		UINT nLastCtrlID = IDCANCEL;
		CWnd* pWndLast = GetDlgItem(nLastCtrlID);
		CRect rectLast;
		pWndLast->GetWindowRect(rectLast);
		CRect r;
		GetWindowRect(r);
		r.bottom = rectLast.bottom+globalUtils.ScaleByDPI(16);

		MoveWindow(r);
	}
}

void RatingLiveFactorDlg::SetEditCtrl() 
{
	if(m_nRatingType==0 && m_iLimitState == 2) // Steel & Fatigue : STL_AASHTO_LRFD19
	{
		GetDlgItem(IDC_DGN_LIVE_FACTOR_STATIC03)->SetWindowText(_T("1.75"));
		GetDlgItem(IDC_DGN_LIVE_FACTOR_STATIC04)->SetWindowText(_T("2.2"));
		GetDlgItem(IDC_DGN_LIVE_FACTOR_STATIC4)->SetWindowText(_T("0.8"));
		GetDlgItem(IDC_DGN_LIVE_FACTOR_STATIC5)->SetWindowText(_T("1.0"));
	}
	else
	{
		if (m_iLimitState == 0 || m_iLimitState == 2) // Service Limit State, Fatigue Limit State
		{
			m_ctrlPrimary_P5.SetWindowText(_T("1.0"));
			m_ctrlPrimary_P7.SetWindowText(_T("1.0"));
			m_ctrlPrimary_P9.SetWindowText(_T("1.0"));
			m_ctrlPrimary_P11.SetWindowText(_T("1.0"));
			m_ctrlPrimary_P13.SetWindowText(_T("1.0"));
			m_ctrlPrimary_User.SetWindowText(_T("1.0"));
			m_ctrlPrimary_P15.SetWindowText(_T("1.0"));
			if (m_nRatingType == 0 && m_iLimitState == 0)  // Steel & Service Limit
				m_ctrlPrimary_HL93I.SetWindowText(_T("1.3"));
			else
				m_ctrlPrimary_HL93I.SetWindowText(_T("1.0"));
			m_ctrlPrimary_HL93O.SetWindowText(_T("1.0"));
		}

		if (m_nRatingType == 0) // Steel
		{
			m_ctrlAdjacent_HL93I.SetWindowText(_T("0.0"));
			m_ctrlAdjacent_HL93O.SetWindowText(_T("0.0"));
		}
		else if (m_nRatingType == 1) // Psc.
		{
			if (m_iLimitState == 0)  // Service Limit
			{
				m_ctrlPrimary_HL93I.SetWindowText(_T("0.8"));
				m_ctrlPrimary_HL93O.SetWindowText(_T("0.0"));

				m_ctrlAdjacent_HL93I.SetWindowText(_T("0.0"));
				m_ctrlAdjacent_HL93O.SetWindowText(_T("0.0"));
			}

		}
	}

}


void RatingLiveFactorDlg::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData(TRUE);

	if(m_nRatingType==0 && m_iLimitState == 2) // Steel & Fatigue : STL_AASHTO_LRFD19
	{
		//m_dPrimary = 1.0;
		//m_dAdjacent = 1.0;
	}
	else if(m_iLimitState == 0 || m_iLimitState == 2) // Service Limit State, Fatigue Limit State
	{
		m_dPrimary = 1.0;
		m_dAdjacent = 1.0;
	}
	else // Strength Limit State
	{
		switch(m_iFactor)
		{
			case 0 :
				m_dPrimary = 1.6;
				m_dAdjacent = 1.0;
				break;
			case 1 :
				m_dPrimary = 1.6;
				m_dAdjacent = 1.0;
				break;
			case 2 :
				m_dPrimary = 1.2;
				m_dAdjacent = 1.0;
				break;
			case 3 :
				m_dPrimary = 1.2;
				m_dAdjacent = 1.0;
				break;
			case 4 :
				m_dPrimary = 1.2;
				m_dAdjacent = 1.0;
				break;
			case 5 :
				m_dPrimary = 1.15;
				m_dAdjacent = 1.0;
				break;
			case 6 :
				m_dPrimary = 1.35;
				m_dAdjacent = 1.0;
				break;
			case 7 :
				m_dPrimary = 1.75;
				if(m_nRatingType == 0) m_dAdjacent = 0.0; 
				else                   m_dAdjacent = 1.0;
				break;
			case 8 :
				m_dPrimary = 1.35;
				if(m_nRatingType == 0) m_dAdjacent = 0.0; 
				else                   m_dAdjacent = 1.0;
				break;
			default:
				m_dPrimary = 1.0;
				m_dAdjacent = 1.0;
				break;
		}
	}

	if(m_iFactor == -1)
		CDialogMove::OnCancel();
	else
		CDialogMove::OnOK();
}

void RatingLiveFactorDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialogMove::OnCancel();
}
