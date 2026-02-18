// DgnConPhiDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "RatingPhiRCLSD15Dlg.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_InitCtrl.h"
#include "..\wg_base\wg_base_LocaleLib.h"
#include "..\wg_base\wg_base_MsgDll.h"

#include "..\wg_main\wg_mainres2.h"	// For ID_DGN_CON_DCON_SRED
#include "DgnDataCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDgnConPhiDlg dialog


CRatingPhiRCLSD15Dlg::CRatingPhiRCLSD15Dlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CRatingPhiRCLSD15Dlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDgnConPhiDlg)
	//
	//}}AFX_DATA_INIT
}

void CRatingPhiRCLSD15Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDgnConPhiDlg)
	DDX_Control(pDX, IDC_DGN_RATING_CON_ULT_EDT,				m_dConPhi_U);
	DDX_Control(pDX, IDC_DGN_RATING_CON_EXT_EDT,				m_dConPhi_E);
	DDX_Control(pDX, IDC_DGN_RATING_CON_SERVICE_EDT,		m_dConPhi_S);
	DDX_Control(pDX, IDC_DGN_RATING_REBAR_ULT_EDT,			m_dRebarPhi_U);
	DDX_Control(pDX, IDC_DGN_RATING_REBAR_EXT_EDT,			m_dRebarPhi_E);
	DDX_Control(pDX, IDC_DGN_RATING_REBAR_SERVICE_EDT,	m_dRebarPhi_S);
	DDX_Control(pDX, IDC_DGN_RATING_RC_COF_CHECK,				m_bAlphacc);	
	DDX_Control(pDX, IDC_DGN_RATING_RC_ALPHACC_EDT,			m_dAlphacc_CVL);

}

#define CDialog CDialogMove
BEGIN_MESSAGE_MAP(CRatingPhiRCLSD15Dlg, CInternationalDlg)
#undef CDialog
	//{{AFX_MSG_MAP(CDgnConPhiDlg)
	ON_BN_CLICKED(IDC_DGN_RATING_RC_COF_CHECK,  OnCheckAlphaCCData)
	ON_BN_CLICKED(IDC_DGN_RATING_PHI_UPDATE_BTN, OnDgnPhiUpdateBtn)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDgnConPhiDlg message handlers

BOOL CRatingPhiRCLSD15Dlg::OnInitDialog() 
{
	m_pDoc = CDBDoc::GetDocPoint();
	CDialogMove::OnInitDialog();

	Initial_Data();


	return TRUE;
}

void CRatingPhiRCLSD15Dlg::OnCheckAlphaCCData()
{
	BOOL bChk = FALSE;
	bChk = m_bAlphacc.GetCheck();

	GetDlgItem(IDC_DGN_RC_RATING_ALPHACC_STR)	 ->	EnableWindow(bChk);
	GetDlgItem(IDC_DGN_RC_RATING_ALPHACC_CLONE)->	EnableWindow(bChk);
	GetDlgItem(IDC_DGN_RATING_RC_ALPHACC_EDT)	 ->	EnableWindow(bChk);
}

void CRatingPhiRCLSD15Dlg::Initial_Data()
{
	if(m_Data.bCheckCofLongTerm)//사용자 입력
	{
		m_bAlphacc.SetCheck(TRUE);
	}
	else
	{
		m_bAlphacc.SetCheck(FALSE);
	}
	GetDlgItem(IDC_DGN_RC_RATING_ALPHACC_STR)	 ->	EnableWindow(m_Data.bCheckCofLongTerm);
	GetDlgItem(IDC_DGN_RC_RATING_ALPHACC_CLONE)->	EnableWindow(m_Data.bCheckCofLongTerm);
	GetDlgItem(IDC_DGN_RATING_RC_ALPHACC_EDT)	 ->	EnableWindow(m_Data.bCheckCofLongTerm);

	// SET UNIT!!
	m_dConPhi_U.SetUnitType(D_UNITSYS_NONE);
	m_dConPhi_E.SetUnitType(D_UNITSYS_NONE);
	m_dConPhi_S.SetUnitType(D_UNITSYS_NONE);
	m_dRebarPhi_U.SetUnitType(D_UNITSYS_NONE);
	m_dRebarPhi_E.SetUnitType(D_UNITSYS_NONE);
	m_dRebarPhi_S.SetUnitType(D_UNITSYS_NONE);
	m_dAlphacc_CVL.SetUnitType(D_UNITSYS_NONE);

	// SET Default DATA by ANNEX!!
	T_RPAC_D Data; Data.Initialize();
	//GetDefaultDatabyAnnex(ParamData, m_nAnnex);
	Data.Set_RC_Rating_AnnexData_LSD();

	m_dConPhi_U.SetEditUnit		(m_Data.dPhi_c[0]==0.0?  Data.dPhi_c[0]		: m_Data.dPhi_c[0]);
	m_dConPhi_E.SetEditUnit		(m_Data.dPhi_c[1]==0.0?  Data.dPhi_c[1]		: m_Data.dPhi_c[1]);
	m_dConPhi_S.SetEditUnit		(m_Data.dPhi_c[2]==0.0?  Data.dPhi_c[2]		: m_Data.dPhi_c[2]);
	m_dRebarPhi_U.SetEditUnit (m_Data.dPhi_s[0]==0.0?  Data.dPhi_s[0]		: m_Data.dPhi_s[0]);
	m_dRebarPhi_E.SetEditUnit (m_Data.dPhi_s[1]==0.0?  Data.dPhi_s[1]		: m_Data.dPhi_s[1]);
	m_dRebarPhi_S.SetEditUnit	(m_Data.dPhi_s[2]==0.0?  Data.dPhi_s[2]		: m_Data.dPhi_s[2]);
	m_dAlphacc_CVL.SetEditUnit(m_Data.dAlphacc==0.0?   Data.dAlphacc		: m_Data.dAlphacc);

}

void CRatingPhiRCLSD15Dlg::OnCancel() 
{
	CDialogMove::OnCancel();
}

void CRatingPhiRCLSD15Dlg::OnOK() 
{
	UpdateData(TRUE);

	Dlg2Data();

	if(!m_pDoc->m_pDataCtrl->AddRpac(m_Data))
		return;

	CDialogMove::OnOK();		

}

void CRatingPhiRCLSD15Dlg::Dlg2Data()
{              
	///*m_Data.Initialize();*/
	//m_Data.Set_RC_Rating_AnnexData_LSD();

	if(m_bAlphacc.GetCheck())//사용자
		m_Data.bCheckCofLongTerm = TRUE;
	else if(!m_bAlphacc.GetCheck())//사용자
		m_Data.bCheckCofLongTerm = FALSE;
	else
		ASSERT(0);


	m_Data.dPhi_c[0]  = m_dConPhi_U.GetEditValue();
	m_Data.dPhi_c[1]  = m_dConPhi_E.GetEditValue();
	m_Data.dPhi_c[2]	= m_dConPhi_S.GetEditValue();
	m_Data.dPhi_s[0]  = m_dRebarPhi_U.GetEditValue();
	m_Data.dPhi_s[1]  = m_dRebarPhi_E.GetEditValue();
	m_Data.dPhi_s[2]  = m_dRebarPhi_S.GetEditValue();
	m_Data.dAlphacc		= m_dAlphacc_CVL.GetEditValue();

}

void CRatingPhiRCLSD15Dlg::OnDgnPhiUpdateBtn() 
{
	m_Data.Set_RC_Rating_AnnexData_LSD();
	Initial_Data();
}
