// SeisNSR2010CalcPhiDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "SeisNSR2010CalcPhiDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSeisNSR2010CalcPhiDlg dialog


CSeisNSR2010CalcPhiDlg::CSeisNSR2010CalcPhiDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CSeisNSR2010CalcPhiDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSeisNSR2010CalcPhiDlg)
	m_dResultX = 1.0;
	m_dResultY = 1.0;
}

void CSeisNSR2010CalcPhiDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSeisNSR2010CalcPhiDlg)
	DDX_Control(pDX, IDC_ETC_SEIS_PHI_A_X_EDT, m_editA_X);
	DDX_Control(pDX, IDC_ETC_SEIS_PHI_A_Y_EDT, m_editA_Y);
	DDX_Control(pDX, IDC_ETC_SEIS_PHI_P_X_EDT, m_editP_X);
	DDX_Control(pDX, IDC_ETC_SEIS_PHI_P_Y_EDT, m_editP_Y);
	DDX_Control(pDX, IDC_ETC_SEIS_PHI_R_X_EDT, m_editR_X);
	DDX_Control(pDX, IDC_ETC_SEIS_PHI_R_Y_EDT, m_editR_Y);
	DDX_Control(pDX, IDC_ETC_SEIS_RESULT_X_EDT, m_editResult_X);
	DDX_Control(pDX, IDC_ETC_SEIS_RESULT_Y_EDT, m_editResult_Y);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSeisNSR2010CalcPhiDlg, CDialogMove)
	//{{AFX_MSG_MAP(CSeisNSR2010CalcPhiDlg)
	ON_BN_CLICKED(IDC_ETC_SEIS_CALC_BTN, OnCalcPhiBtn)	
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSeisNSR2010CalcPhiDlg message handlers
void CSeisNSR2010CalcPhiDlg::OnCalcPhiBtn() 
{
	CalcPhi();
}

void CSeisNSR2010CalcPhiDlg::OnOK() 
{
	CalcPhi();
	CDialogMove::OnOK();
}

BOOL CSeisNSR2010CalcPhiDlg::OnInitDialog() 
{
	CDialogMove::OnInitDialog();	

	m_editA_X.SetUnitType(D_UNITSYS_NONE);
	m_editA_Y.SetUnitType(D_UNITSYS_NONE);
	m_editP_X.SetUnitType(D_UNITSYS_NONE);
	m_editP_Y.SetUnitType(D_UNITSYS_NONE);
	m_editR_X.SetUnitType(D_UNITSYS_NONE);
	m_editR_Y.SetUnitType(D_UNITSYS_NONE);
	m_editResult_X.SetUnitType(D_UNITSYS_NONE);
	m_editResult_Y.SetUnitType(D_UNITSYS_NONE);
	m_editResult_X.SetReadOnly(TRUE);
	m_editResult_Y.SetReadOnly(TRUE);

	m_editA_X.SetEditUnit(1.0);
	m_editA_Y.SetEditUnit(1.0);
	m_editP_X.SetEditUnit(1.0);
	m_editP_Y.SetEditUnit(1.0);
	m_editR_X.SetEditUnit(1.0);
	m_editR_Y.SetEditUnit(1.0);

	CalcPhi();
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


BOOL CSeisNSR2010CalcPhiDlg::CalcPhi()
{  
	double dA_X = m_editA_X.GetEditValue();
	double dA_Y = m_editA_Y.GetEditValue();
	double dP_X = m_editP_X.GetEditValue();
	double dP_Y = m_editP_Y.GetEditValue();
	double dR_X = m_editR_X.GetEditValue();
	double dR_Y = m_editR_Y.GetEditValue();

	m_dResultX = dA_X*dP_X*dR_X;
	m_dResultY = dA_Y*dP_Y*dR_Y;
	m_editResult_X.SetEditUnit(m_dResultX);
	m_editResult_Y.SetEditUnit(m_dResultY);
	return TRUE;
}