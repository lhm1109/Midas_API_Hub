// ETC_GDS_UBC88.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "ETC_GDS_UBC88.h"
#include "SpfcUBC88Util.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CETC_GDS_UBC88 dialog


CETC_GDS_UBC88::CETC_GDS_UBC88(T_SPFC_D* pParamData, CWnd* pParent /*=NULL*/)
	: CMyChildDialog(CETC_GDS_UBC88::IDD, pParent, std::make_shared<CSpfcUBC88Util>())
{
	//{{AFX_DATA_INIT(CETC_GDS_UBC88)
	m_pParamData = pParamData;
	m_pSpfcUtil = std::dynamic_pointer_cast<CSpfcUBC88Util>(m_pSpfcBaseUtil);

	m_sOIF = _T("1.0");
	m_sRw = _T("1.0");
	m_nSoil = 0;
	m_nSZF = 0;
	m_dMaxPeriod = PRD_ED;
	//}}AFX_DATA_INIT
	m_pSpfcUtil->SetSoil(1.0);
	m_pSpfcUtil->SetSZF(0.075);
}


void CETC_GDS_UBC88::DoDataExchange(CDataExchange* pDX)
{
	CMyChildDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CETC_GDS_UBC88)
	DDX_Control(pDX, IDC_ETC_GDS_CMB_RMC, m_cmbRw);
	DDX_Control(pDX, IDC_ETC_GDS_UBC88_CMB_OIF, m_cmbOIF);
	DDX_CBString(pDX, IDC_ETC_GDS_UBC88_CMB_OIF, m_sOIF);
	DDX_CBString(pDX, IDC_ETC_GDS_CMB_RMC, m_sRw);
	DDX_Radio(pDX, IDC_ETC_GDS_UBC88_RDO_SOIL1, m_nSoil);
	DDX_Radio(pDX, IDC_ETC_GDS_UBC88_RDO_SZF1, m_nSZF);
	DDX_Text(pDX, IDC_CMD_PERIOD_EDIT, m_dMaxPeriod);	
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CETC_GDS_UBC88, CMyChildDialog)
	//{{AFX_MSG_MAP(CETC_GDS_UBC88)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CETC_GDS_UBC88 message handlers

void CETC_GDS_UBC88::End()
{
	UpdateData(TRUE);
	
	m_pSpfcUtil->SetOIF(_tstof(m_sOIF));
	m_pSpfcUtil->SetRw(_tstof(m_sRw));

	if (m_nSoil == 0)
		m_pSpfcUtil->SetSoil(1.0);
	else if (m_nSoil == 1)
		m_pSpfcUtil->SetSoil(1.2);
	else if (m_nSoil == 2)
		m_pSpfcUtil->SetSoil(1.5);
	else if (m_nSoil == 3)
		m_pSpfcUtil->SetSoil(2.0);

	if (m_nSZF == 0)
		m_pSpfcUtil->SetSZF(0.075);
	else if (m_nSZF == 1)
		m_pSpfcUtil->SetSZF(0.15);
	else if (m_nSZF == 2)
		m_pSpfcUtil->SetSZF(0.20);
	else if (m_nSZF == 3)
		m_pSpfcUtil->SetSZF(0.30);
	else if (m_nSZF == 4)
		m_pSpfcUtil->SetSZF(0.40);
 
	m_pSpfcUtil->SetMaxPeriod(m_dMaxPeriod);

	CMyChildDialog::End();
}

BOOL CETC_GDS_UBC88::OnInitDialog() 
{
	CMyChildDialog::OnInitDialog();
	
	((CButton*)GetDlgItem(IDC_ETC_GDS_UBC88_I_STATIC))->ShowWindow(FALSE);
	((CButton*)GetDlgItem(IDC_ETC_GDS_UBC88_R_STATIC))->ShowWindow(FALSE);
	((CButton*)GetDlgItem(IDC_ETC_GDS_UBC88_CMB_OIF))->ShowWindow(FALSE);
	((CButton*)GetDlgItem(IDC_ETC_GDS_CMB_RMC))->ShowWindow(FALSE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
BOOL CETC_GDS_UBC88::ISVALID()
{
	if(!IsValidCmb_d(&m_cmbOIF,0,10,1.0)) return FALSE;
	if(!IsValidCmb_d(&m_cmbRw,0,50,1.0)) return FALSE;
	if(m_dMaxPeriod <= 0) {AfxMessageBox(_LS(IDS_CMD_SPECTRUM__MAX_PERIOD_GREATER_0)); return FALSE;}  
	return TRUE;  
}

BOOL CETC_GDS_UBC88::Dlg2Data()
{
	m_pParamData->CodeParam.UBC88.dSoil = m_pSpfcUtil->GetSoil();
	m_pParamData->CodeParam.UBC88.dSeis = m_pSpfcUtil->GetSZF();
	m_pParamData->CodeParam.UBC88.dIe   = m_pSpfcUtil->GetOIF();
	m_pParamData->CodeParam.UBC88.dCoef = m_pSpfcUtil->GetRw();
	m_pParamData->CodeParam.UBC88.dMaxPeriod = m_dMaxPeriod;

	return TRUE;
}

void CETC_GDS_UBC88::SetPushOverData(BOOL bInit/*=TRUE*/)
{
	m_pSpfcUtil->SetSoil(m_pParamData->CodeParam.UBC88.dSoil);
	m_pSpfcUtil->SetSZF(m_pParamData->CodeParam.UBC88.dSeis);
	m_pSpfcUtil->SetOIF(m_pParamData->CodeParam.UBC88.dIe);
	m_pSpfcUtil->SetRw(m_pParamData->CodeParam.UBC88.dCoef);
	m_dMaxPeriod  = m_pParamData->CodeParam.UBC88.dMaxPeriod;

	m_pSpfcUtil->SetMaxPeriod(m_dMaxPeriod);

	if     (m_pSpfcUtil->GetSoil() == 1.0) m_nSoil = 0;
	else if(m_pSpfcUtil->GetSoil() == 1.2) m_nSoil = 1;
	else if(m_pSpfcUtil->GetSoil() == 1.5) m_nSoil = 2;
	else { ASSERT(0); m_nSoil = 0; }

	if     (m_pSpfcUtil->GetSZF() == 0.075) m_nSZF = 0;
	else if(m_pSpfcUtil->GetSZF() == 0.15 ) m_nSZF = 1;
	else if(m_pSpfcUtil->GetSZF() == 0.20 ) m_nSZF = 2;
	else if(m_pSpfcUtil->GetSZF() == 0.30 ) m_nSZF = 3;
	else if(m_pSpfcUtil->GetSZF() == 0.40 ) m_nSZF = 4;
	else { ASSERT(0); m_nSZF = 0; }

	if(bInit)
	{
		UpdateData(FALSE);

		CString strI, strR;
		strI.Format(_T("%g"), m_pSpfcUtil->GetOIF());
		strR.Format(_T("%g"), m_pSpfcUtil->GetRw());
		m_cmbOIF.SetWindowText(strI);    
		m_cmbRw.SetWindowText(strR);
		m_cmbRw.EnableWindow(FALSE);
	}
}