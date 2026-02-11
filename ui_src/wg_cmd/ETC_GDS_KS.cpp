// ETC_GDS_KS.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "ETC_GDS_KS.h"

#include "SpfcKSUtil.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CETC_GDS_KS dialog


CETC_GDS_KS::CETC_GDS_KS(T_SPFC_D* pParamData, CWnd* pParent /*=NULL*/)
	: CMyChildDialog(CETC_GDS_KS::IDD, pParent, std::make_shared<CSpfcKSUtil>())
{
	//{{AFX_DATA_INIT(CETC_GDS_KS)
	m_rdoSOIL = 0;
	m_sOIF = _T("1.0");
	m_sRMC = _T("1.0");
	m_nEPA = 0;
	m_dMaxPeriod = PRD_ED;
	//}}AFX_DATA_INIT

	m_pSpfcUtil = std::dynamic_pointer_cast<CSpfcKSUtil>(m_pSpfcBaseUtil);
	m_pParamData = pParamData;
}

CETC_GDS_KS::~CETC_GDS_KS()
{
}



void CETC_GDS_KS::DoDataExchange(CDataExchange* pDX)
{
	CMyChildDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CETC_GDS_KS)
	DDX_Control(pDX, IDC_ETC_GDS_CMB_RMC, m_cmbRMC);
	DDX_Control(pDX, IDC_ETC_GDS_KS_CMB_OIF, m_cmbOIF);
	DDX_Radio(pDX, IDC_ETC_GDS_KS_RDO_SOIL, m_rdoSOIL);
	DDX_CBString(pDX, IDC_ETC_GDS_KS_CMB_OIF, m_sOIF);
	DDX_CBString(pDX, IDC_ETC_GDS_CMB_RMC, m_sRMC);
	DDX_Radio(pDX, IDC_ETC_GDS_KS_RDO_EPA, m_nEPA);
	DDX_Text(pDX, IDC_CMD_PERIOD_EDIT, m_dMaxPeriod);	
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CETC_GDS_KS, CMyChildDialog)
	//{{AFX_MSG_MAP(CETC_GDS_KS)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CETC_GDS_KS message handlers

BOOL CETC_GDS_KS::OnInitDialog() 
{
	CMyChildDialog::OnInitDialog();
	
	
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CETC_GDS_KS::End()
{
	UpdateData(TRUE);
	
	m_pSpfcUtil->SetOIF(_tstof(m_sOIF));
	m_pSpfcUtil->SetRMC(_tstof(m_sRMC));
	if (m_rdoSOIL == 0)
		m_pSpfcUtil->SetSoil(1.0);
	else if (m_rdoSOIL == 1)
		m_pSpfcUtil->SetSoil(1.2);
	else if (m_rdoSOIL == 2)
		m_pSpfcUtil->SetSoil(1.5);

	if (m_nEPA == 0)
		m_pSpfcUtil->SetEPA(0.08);
	else if (m_nEPA == 1)
		m_pSpfcUtil->SetEPA(0.12);

	m_pSpfcUtil->SetMaxPeriod(m_dMaxPeriod);


	CMyChildDialog::End();
}

BOOL CETC_GDS_KS::ISVALID()
{  
	if(!IsValidCmb_d(&m_cmbOIF,0,10,1.0)) return FALSE;
	if(!IsValidCmb_d(&m_cmbRMC,0,50,1.0)) return FALSE;
	if(m_dMaxPeriod <= 0) {AfxMessageBox(_LS(IDS_CMD_SPECTRUM__MAX_PERIOD_GREATER_0)); return FALSE;}  
	return TRUE;  
}

BOOL CETC_GDS_KS::Dlg2Data()
{
	m_pParamData->CodeParam.KS.dSoil = m_pSpfcUtil->GetSoil();
	m_pParamData->CodeParam.KS.dEPA  = m_pSpfcUtil->GetEPA();
	m_pParamData->CodeParam.KS.dIe   = m_pSpfcUtil->GetOIF();
	m_pParamData->CodeParam.KS.dCoef = m_pSpfcUtil->GetRMC();

	m_pParamData->CodeParam.KS.dMaxPeriod = m_dMaxPeriod;

	m_pParamData->dResModCoef = m_pSpfcUtil->GetRMC();
	return TRUE;
}

void CETC_GDS_KS::SetPushOverData(BOOL bInit/*=TRUE*/)
{
	m_pSpfcUtil->SetSoil(m_pParamData->CodeParam.KS.dSoil);
	m_pSpfcUtil->SetEPA(m_pParamData->CodeParam.KS.dEPA);
	m_pSpfcUtil->SetOIF(m_pParamData->CodeParam.KS.dIe);
	m_pSpfcUtil->SetRMC(m_pParamData->CodeParam.KS.dCoef);
	m_dMaxPeriod = m_pParamData->CodeParam.KS.dMaxPeriod;

	m_pSpfcUtil->SetMaxPeriod(m_dMaxPeriod);

	const double dSoil = m_pSpfcUtil->GetSoil();
	const double dEPA = m_pSpfcUtil->GetEPA();

	if (dSoil == 1.0) m_rdoSOIL = 0;
	else if (dSoil == 1.2) m_rdoSOIL = 1;
	else if (dSoil == 1.5) m_rdoSOIL = 2;
	else { ASSERT(0); m_rdoSOIL = 0; }

	if (dEPA == 0.08) m_nEPA = 0;
	else if (dEPA == 0.12) m_nEPA = 1;
	else { ASSERT(0); m_nEPA = 0; }

	if (bInit)
	{
		UpdateData(FALSE);

		CString strI, strR;
		strI.Format(_T("%g"), m_pSpfcUtil->GetOIF());
		strR.Format(_T("%g"), m_pSpfcUtil->GetRMC());
		m_cmbOIF.SetWindowText(strI);
		m_cmbRMC.SetWindowText(strR);
		m_cmbRMC.EnableWindow(FALSE);
	}
}