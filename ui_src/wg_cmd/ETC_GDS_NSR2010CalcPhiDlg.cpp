// ETC_GDS_NSR2010CalcPhiDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "ETC_GDS_NSR2010CalcPhiDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CETC_GDS_NSR2010CalcPhiDlg dialog


CETC_GDS_NSR2010CalcPhiDlg::CETC_GDS_NSR2010CalcPhiDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CETC_GDS_NSR2010CalcPhiDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CETC_GDS_NSR2010CalcPhiDlg)
	m_dResultX = 1.0;
}

void CETC_GDS_NSR2010CalcPhiDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CETC_GDS_NSR2010CalcPhiDlg)
	DDX_Control(pDX, IDC_ETC_SEIS_PHI_A_X_EDT, m_editA_X);
	DDX_Control(pDX, IDC_ETC_SEIS_PHI_P_X_EDT, m_editP_X);
	DDX_Control(pDX, IDC_ETC_SEIS_PHI_R_X_EDT, m_editR_X);
	DDX_Control(pDX, IDC_ETC_SEIS_RESULT_X_EDT, m_editResult_X);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CETC_GDS_NSR2010CalcPhiDlg, CDialogMove)
	//{{AFX_MSG_MAP(CETC_GDS_NSR2010CalcPhiDlg)
	ON_BN_CLICKED(IDC_ETC_SEIS_CALC_BTN, OnCalcPhiBtn)	
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CETC_GDS_NSR2010CalcPhiDlg message handlers
void CETC_GDS_NSR2010CalcPhiDlg::OnCalcPhiBtn() 
{
	CalcPhi();
}

void CETC_GDS_NSR2010CalcPhiDlg::OnOK() 
{
	CalcPhi();
	CDialogMove::OnOK();
}

BOOL CETC_GDS_NSR2010CalcPhiDlg::OnInitDialog() 
{
	CDialogMove::OnInitDialog();
#ifdef _RUS
	this->SetWindowText(_LS(IDS_IDD_ETC_GDS_CALCPHI_NSR2010_IDD_ETC_GDS_CALCPHI_NSR2010));
	GetDlgItem(IDC_WG_CMD_STATIC0)->SetWindowText(_LS(IDS_IDD_ETC_GDS_CALCPHI_NSR2010_IDC_WG_CMD_STATIC0));
	GetDlgItem(IDC_WG_CMD_STATIC1)->SetWindowText(_LS(IDS_IDD_ETC_GDS_CALCPHI_NSR2010_IDC_WG_CMD_STATIC1));
	GetDlgItem(IDC_WG_CMD_STATIC2)->SetWindowText(_LS(IDS_IDD_ETC_GDS_CALCPHI_NSR2010_IDC_WG_CMD_STATIC2));
	GetDlgItem(IDC_WG_CMD_STATIC3)->SetWindowText(_LS(IDS_IDD_ETC_GDS_CALCPHI_NSR2010_IDC_WG_CMD_STATIC3));
	GetDlgItem(IDC_WG_CMD_STATIC6)->SetWindowText(_LS(IDS_IDD_ETC_GDS_CALCPHI_NSR2010_IDC_WG_CMD_STATIC6));
	GetDlgItem(IDC_ETC_SEIS_CALC_BTN)->SetWindowText(_LS(IDS_IDD_ETC_GDS_CALCPHI_NSR2010_IDC_ETC_SEIS_CALC_BTN));
	GetDlgItem(IDOK)->SetWindowText(_LS(IDS_IDD_ETC_GDS_CALCPHI_NSR2010_IDOK));
	GetDlgItem(IDCANCEL)->SetWindowText(_LS(IDS_IDD_ETC_GDS_CALCPHI_NSR2010_IDCANCEL));
#endif
	m_editA_X.SetUnitType(D_UNITSYS_NONE);
	m_editP_X.SetUnitType(D_UNITSYS_NONE);
	m_editR_X.SetUnitType(D_UNITSYS_NONE);
	m_editResult_X.SetUnitType(D_UNITSYS_NONE);
	m_editResult_X.SetReadOnly(TRUE);

	m_editA_X.SetEditUnit(1.0);
	m_editP_X.SetEditUnit(1.0);
	m_editR_X.SetEditUnit(1.0);

	CalcPhi();
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


BOOL CETC_GDS_NSR2010CalcPhiDlg::CalcPhi()
{  
	double dA_X = m_editA_X.GetEditValue();
	double dP_X = m_editP_X.GetEditValue();
	double dR_X = m_editR_X.GetEditValue();

	m_dResultX = dA_X*dP_X*dR_X;
	m_editResult_X.SetEditUnit(m_dResultX);
	return TRUE;
}