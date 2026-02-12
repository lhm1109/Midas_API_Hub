// ETC_GDS_DPWH_LRFD_2013.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "ETC_GDS_DPWH_LRFD_2013.h"

#include "SpfcPH2013Util.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CETC_GDS_DPWH_LRFD_2013 dialog

CETC_GDS_DPWH_LRFD_2013::CETC_GDS_DPWH_LRFD_2013(T_SPFC_D* pParamData, CWnd* pParent /*=NULL*/)
	: CMyChildDialog(CETC_GDS_DPWH_LRFD_2013::IDD, pParent, std::make_shared<CSpfcPH2013Util>())
{
	//{{AFX_DATA_INIT(CETC_GDS_DPWH_LRFD_2013)
	m_pParamData = pParamData;
	m_pSpfcUtil = std::dynamic_pointer_cast<CSpfcPH2013Util>(m_pSpfcBaseUtil);

	m_nGroundType = 0;

	//}}AFX_DATA_INIT
}

void CETC_GDS_DPWH_LRFD_2013::DoDataExchange(CDataExchange* pDX)
{
	CMyChildDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CETC_GDS_DPWH_LRFD_2013)
	DDX_Radio   (pDX, IDC_ETC_GDS_DPWH_LRFD_2013_GT_1_RDO, m_nGroundType);
	DDX_Control (pDX, IDC_ETC_GDS_DPWH_LRFD_2013_PGA_EDT,  m_edtPGA);
	DDX_Control (pDX, IDC_ETC_GDS_DPWH_LRFD_2013_SS_EDT,   m_edtSS);
	DDX_Control (pDX, IDC_ETC_GDS_DPWH_LRFD_2013_S1_EDT,   m_edtS1);
	DDX_Control (pDX, IDC_ETC_GDS_DPWH_LRFD_2013_R_CMB,    m_cmbRMF);
	DDX_Control (pDX, IDC_ETC_GDS_DPWH_LRFD_2013_MAX_PERIOD_EDT,   m_edtMaxPeriod);
	//}}AFX_DATA_MAP	
}


BEGIN_MESSAGE_MAP(CETC_GDS_DPWH_LRFD_2013, CMyChildDialog)
	//{{AFX_MSG_MAP(CETC_GDS_DPWH_LRFD_2013)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CETC_GDS_DPWH_LRFD_2013::InitUnit() 
{
	m_edtPGA.SetEditUnit(D_UNITSYS_NONE);
	m_edtSS.SetEditUnit(D_UNITSYS_NONE);
	m_edtS1.SetEditUnit(D_UNITSYS_NONE);
	m_edtMaxPeriod.SetEditUnit(D_UNITSYS_NONE);
}

void CETC_GDS_DPWH_LRFD_2013::InitCombo() 
{
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
// CETC_GDS_DPWH_LRFD_2013 message handlers

BOOL CETC_GDS_DPWH_LRFD_2013::OnInitDialog() 
{
	CMyChildDialog::OnInitDialog();
	
	InitUnit();
	InitCombo();

	Data2Dlg();
	
	UpdateData(FALSE);

	GetDlgItem(IDC_ETC_GDS_DPWH_LRFD_2013_GT_1_RDO)->SetWindowText(_LS(IDS_WG_CMD__ADD_DPWH_LRFD_2013_GROUP_TYPE_1));
	GetDlgItem(IDC_ETC_GDS_DPWH_LRFD_2013_GT_2_RDO)->SetWindowText(_LS(IDS_WG_CMD__ADD_DPWH_LRFD_2013_GROUP_TYPE_2));
	GetDlgItem(IDC_ETC_GDS_DPWH_LRFD_2013_GT_3_RDO)->SetWindowText(_LS(IDS_WG_CMD__ADD_DPWH_LRFD_2013_GROUP_TYPE_3));
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CETC_GDS_DPWH_LRFD_2013::End()
{
	UpdateData(TRUE);
	Dlg2UtilData();
	SetValue();

	CMyChildDialog::End();
}

void CETC_GDS_DPWH_LRFD_2013::SetValue()
{
	m_pSpfcUtil->SetParamFromCalc();
}

BOOL CETC_GDS_DPWH_LRFD_2013::ISVALID()
{ 
	UpdateData(TRUE);

	m_pSpfcUtil->SetPGA(m_edtPGA.GetEditValue());
	m_pSpfcUtil->SetSS(m_edtSS.GetEditValue());
	m_pSpfcUtil->SetS1(m_edtS1.GetEditValue());

	CString str; 
	m_cmbRMF.GetWindowText(str);
	m_pSpfcUtil->SetRMF(_tstof(str));

	m_pSpfcUtil->SetMaxPeriod(m_edtMaxPeriod.GetEditValue());

	if(m_pSpfcUtil->GetPGA() <= 0)    {AfxMessageBox(_LS(IDS_CMD_SPECTRUM__FAILURE_PGA)); return FALSE;}  
	if(m_pSpfcUtil->GetSS() <= 0)     {AfxMessageBox(_LS(IDS_CMD_SPECTRUM__FAILURE_SS)); return FALSE;}  
	if(m_pSpfcUtil->GetS1() <= 0)     {AfxMessageBox(_LS(IDS_CMD_SPECTRUM__FAILURE_S1)); return FALSE;}  
	if(m_pSpfcUtil->GetRMF() <= 0)    {AfxMessageBox(_LS(IDS_CMD_SPECTRUM__FAILURE_RMF)); return FALSE;}  

	if(m_pSpfcUtil->GetMaxPeriod() <= 0) {AfxMessageBox(_LS(IDS_CMD_SPECTRUM__MAX_PERIOD_GREATER_0)); return FALSE;}  

	return TRUE;  
}

BOOL CETC_GDS_DPWH_LRFD_2013::Dlg2Data()
{
	m_pParamData->CodeParam.DPWHLRFD2013.Initialize();
	m_pParamData->nSpecCodeType = D_SPFC_CODE_DPWH_LRFD_2013;   //확인사살~  
	m_pParamData->CodeParam.DPWHLRFD2013.nGroundType = m_nGroundType;
	m_pParamData->CodeParam.DPWHLRFD2013.dPGA        = m_pSpfcUtil->GetPGA();
	m_pParamData->CodeParam.DPWHLRFD2013.dSs         = m_pSpfcUtil->GetSS();
	m_pParamData->CodeParam.DPWHLRFD2013.dS1         = m_pSpfcUtil->GetS1();
	m_pParamData->CodeParam.DPWHLRFD2013.dR          = m_pSpfcUtil->GetRMF();
	m_pParamData->CodeParam.DPWHLRFD2013.dMaxPeriod  = m_pSpfcUtil->GetMaxPeriod();

	return TRUE;
}

void CETC_GDS_DPWH_LRFD_2013::Data2Dlg()
{
	SetPushOverData(TRUE);
}

void CETC_GDS_DPWH_LRFD_2013::SetPushOverData(BOOL bInit/*=TRUE*/)
{
	if (m_pParamData->nSpecCodeType != D_SPFC_CODE_DPWH_LRFD_2013)
	{
		//m_pParamData->nSpecCodeType=D_SPFC_CODE_DPWH_LRFD_2013; 여기서 이걸 하면 안된다.    
		m_pParamData->CodeParam.Initialize(D_SPFC_CODE_DPWH_LRFD_2013);
	}
	m_nGroundType = m_pParamData->CodeParam.DPWHLRFD2013.nGroundType;
	m_pSpfcUtil->SetGroundType(m_nGroundType);
	m_pSpfcUtil->SetPGA(m_pParamData->CodeParam.DPWHLRFD2013.dPGA);
	m_pSpfcUtil->SetSS(m_pParamData->CodeParam.DPWHLRFD2013.dSs);
	m_pSpfcUtil->SetS1(m_pParamData->CodeParam.DPWHLRFD2013.dS1);
	m_pSpfcUtil->SetRMF(m_pParamData->CodeParam.DPWHLRFD2013.dR);
	m_pSpfcUtil->SetMaxPeriod(m_pParamData->CodeParam.DPWHLRFD2013.dMaxPeriod);

 	SetValue();

	if(bInit)
	{			
		m_edtPGA.SetEditUnit(m_pSpfcUtil->GetPGA());
		m_edtSS.SetEditUnit(m_pSpfcUtil->GetSS());
		m_edtS1.SetEditUnit(m_pSpfcUtil->GetS1());

		InitCombo();
		CString strRMF;
		strRMF.Format(_T("%g"), m_pSpfcUtil->GetRMF());
		m_cmbRMF.SetWindowText(strRMF);

		m_edtMaxPeriod.SetEditUnit(m_pSpfcUtil->GetMaxPeriod());

		Dlg2UtilData();
		UpdateData(FALSE);
	}
}

void CETC_GDS_DPWH_LRFD_2013::Dlg2UtilData()
{
	CString str;
	m_cmbRMF.GetWindowText(str);

	m_pSpfcUtil->SetGroundType(m_nGroundType);
	m_pSpfcUtil->SetPGA(m_edtPGA.GetEditValue());
	m_pSpfcUtil->SetSS(m_edtSS.GetEditValue());
	m_pSpfcUtil->SetS1(m_edtS1.GetEditValue());
	m_pSpfcUtil->SetRMF(_tstof(str));
	m_pSpfcUtil->SetMaxPeriod(m_edtMaxPeriod.GetEditValue());
}
