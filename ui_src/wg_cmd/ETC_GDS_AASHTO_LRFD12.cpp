// ETC_GDS_AASHTO_LRFD12.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "ETC_GDS_AASHTO_LRFD12.h"
#include <math.h>

#include "SpfcAASHTOLRFD12Util.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CETC_GDS_AASHTO_LRFD12 dialog

CETC_GDS_AASHTO_LRFD12::CETC_GDS_AASHTO_LRFD12(T_SPFC_D* pParamData, CWnd* pParent /*=NULL*/)
	: CMyChildDialog(CETC_GDS_AASHTO_LRFD12::IDD, pParent, std::make_shared<CSpfcAASHTOLRFD12Util>())
{
	//{{AFX_DATA_INIT(CETC_GDS_AASHTO_LRFD12)
	m_pParamData = pParamData;
	m_pSpfcUtil = std::dynamic_pointer_cast<CSpfcAASHTOLRFD12Util>(m_pSpfcBaseUtil);

	m_nSiteClasee = 0;

	//m_dPGA = 0.0;
	//m_dSS = 0.0;
	//m_dS1 = 0.0;
	//m_dRMF = 0.0;
	//m_dMaxPeriod = 0.0;
	//m_strSoil = _T("");

	//m_dFpga = 0.0;
	//m_dFa = 0.0;
	//m_dFv = 0.0;
	//}}AFX_DATA_INIT
}

void CETC_GDS_AASHTO_LRFD12::DoDataExchange(CDataExchange* pDX)
{
	CMyChildDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CETC_GDS_AASHTO_LRFD12)
	DDX_Radio   (pDX, IDC_ETC_GDS_AASHTO_LRFD12_SITE_CLASS_A_RDO, m_nSiteClasee);
	DDX_Control (pDX, IDC_ETC_GDS_AASHTO_LRFD12_PGA_CMB, m_cmbPGA);
	DDX_Control (pDX, IDC_ETC_GDS_AASHTO_LRFD12_SS_CMB,  m_cmbSS);
	DDX_Control (pDX, IDC_ETC_GDS_AASHTO_LRFD12_S1_CMB,  m_cmbS1);
	DDX_Control (pDX, IDC_ETC_GDS_AASHTO_LRFD12_R_CMB,   m_cmbRMF);
	DDX_Control (pDX, IDC_ETC_GDS_AASHTO_LRFD12_MAX_PERIOD_EDT,   m_edtMaxPeriod);
	//}}AFX_DATA_MAP	
}


BEGIN_MESSAGE_MAP(CETC_GDS_AASHTO_LRFD12, CMyChildDialog)
	//{{AFX_MSG_MAP(CETC_GDS_AASHTO_LRFD12)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CETC_GDS_AASHTO_LRFD12::InitCombo() 
{
	// PGA
	m_cmbPGA.ResetContent();
	m_cmbPGA.AddString(_T("0.1"));
	m_cmbPGA.AddString(_T("0.2"));
	m_cmbPGA.AddString(_T("0.3"));
	m_cmbPGA.AddString(_T("0.4"));
	m_cmbPGA.AddString(_T("0.5"));
	m_cmbPGA.SetCurSel(0);
	
	// Ss
	m_cmbSS.ResetContent();
	m_cmbSS.AddString(_T("0.25"));
	m_cmbSS.AddString(_T("0.5"));
	m_cmbSS.AddString(_T("0.75"));
	m_cmbSS.AddString(_T("1.0"));
	m_cmbSS.AddString(_T("1.25"));
	m_cmbSS.SetCurSel(0);

	// S1
	m_cmbS1.ResetContent();
	m_cmbS1.AddString(_T("0.1"));
	m_cmbS1.AddString(_T("0.2"));
	m_cmbS1.AddString(_T("0.3"));
	m_cmbS1.AddString(_T("0.4"));
	m_cmbS1.AddString(_T("0.5"));
	m_cmbS1.SetCurSel(0);

	// RMF
	m_cmbRMF.ResetContent();
	m_cmbRMF.AddString(_T("0.8"));
	m_cmbRMF.AddString(_T("1.0"));
	m_cmbRMF.AddString(_T("1.5"));
	m_cmbRMF.AddString(_T("2.0"));
	m_cmbRMF.AddString(_T("3.0"));
	m_cmbRMF.AddString(_T("3.5"));
	m_cmbRMF.AddString(_T("5.0"));
	m_cmbRMF.SetCurSel(1);
}

/////////////////////////////////////////////////////////////////////////////
// CETC_GDS_AASHTO_LRFD12 message handlers

BOOL CETC_GDS_AASHTO_LRFD12::OnInitDialog() 
{
	CMyChildDialog::OnInitDialog();
	
	InitCombo();

	m_edtMaxPeriod.SetEditUnit(6.0);

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
	
void CETC_GDS_AASHTO_LRFD12::End()
{
	UpdateData(TRUE);

	CString str; 
	m_cmbPGA.GetWindowText(str);
	m_pSpfcUtil->SetPGA(_tstof(str));

	m_cmbSS.GetWindowText(str);
	m_pSpfcUtil->SetSS(_tstof(str));

	m_cmbS1.GetWindowText(str);
	m_pSpfcUtil->SetS1(_tstof(str));

	m_cmbRMF.GetWindowText(str);
	m_pSpfcUtil->SetRMF(_tstof(str));

	m_pSpfcUtil->SetMaxPeriod(m_edtMaxPeriod.GetEditValue());

	m_pSpfcUtil->SetSiteClass(m_nSiteClasee);
	m_pSpfcUtil->SetValue();

	CMyChildDialog::End();
}

BOOL CETC_GDS_AASHTO_LRFD12::ISVALID()
{ 
	UpdateData(TRUE);
	
	CString str; 
	m_cmbPGA.GetWindowText(str);
	m_pSpfcUtil->SetPGA(_tstof(str));

	m_cmbSS.GetWindowText(str);
	m_pSpfcUtil->SetSS(_tstof(str));

	m_cmbS1.GetWindowText(str);
	m_pSpfcUtil->SetS1(_tstof(str));

	m_cmbRMF.GetWindowText(str);
	m_pSpfcUtil->SetRMF(_tstof(str));

	m_pSpfcUtil->SetMaxPeriod(m_edtMaxPeriod.GetEditValue());

	return m_pSpfcUtil->CheckValid();
}

BOOL CETC_GDS_AASHTO_LRFD12::Dlg2Data()
{
	m_pParamData->CodeParam.AASHTO_LRFD12.nSiteClass = m_nSiteClasee;
	m_pParamData->CodeParam.AASHTO_LRFD12.dPGA       = m_pSpfcUtil->GetPGA();
	m_pParamData->CodeParam.AASHTO_LRFD12.dSs        = m_pSpfcUtil->GetSS();
	m_pParamData->CodeParam.AASHTO_LRFD12.dS1        = m_pSpfcUtil->GetS1();
	m_pParamData->CodeParam.AASHTO_LRFD12.dR         = m_pSpfcUtil->GetRMF();
	m_pParamData->CodeParam.AASHTO_LRFD12.dMaxPeriod = m_pSpfcUtil->GetMaxPeriod();

	return TRUE;
}

void CETC_GDS_AASHTO_LRFD12::SetPushOverData(BOOL bInit/*=TRUE*/)
{
	m_nSiteClasee  = m_pParamData->CodeParam.AASHTO_LRFD12.nSiteClass;
	m_pSpfcUtil->SetPGA(m_pParamData->CodeParam.AASHTO_LRFD12.dPGA);
	m_pSpfcUtil->SetSS(m_pParamData->CodeParam.AASHTO_LRFD12.dSs);
	m_pSpfcUtil->SetS1(m_pParamData->CodeParam.AASHTO_LRFD12.dS1);
	m_pSpfcUtil->SetRMF(m_pParamData->CodeParam.AASHTO_LRFD12.dR);
	m_pSpfcUtil->SetMaxPeriod(m_pParamData->CodeParam.AASHTO_LRFD12.dMaxPeriod);

	m_pSpfcUtil->SetSiteClass(m_nSiteClasee);

	m_pSpfcUtil->SetValue();
	if(bInit)
	{
		UpdateData(FALSE);

		CString strPGA, strSS, strS1, strRMF;
		strPGA.Format(_T("%g"), m_pSpfcUtil->GetPGA());
		strSS.Format(_T("%g"), m_pSpfcUtil->GetSS());
		strS1.Format(_T("%g"), m_pSpfcUtil->GetS1());
		strRMF.Format(_T("%g"), m_pSpfcUtil->GetRMF());

		m_cmbPGA.SetWindowText(strPGA);   
		m_cmbSS.SetWindowText(strSS);
		m_cmbS1.SetWindowText(strS1);   
		m_cmbRMF.SetWindowText(strRMF);
		m_edtMaxPeriod.SetEditUnit(m_pSpfcUtil->GetMaxPeriod());
	}
}