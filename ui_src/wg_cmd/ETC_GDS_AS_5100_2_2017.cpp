// ETC_GDS_AS_5100_2_2017.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "ETC_GDS_AS_5100_2_2017.h"

#include "SpfcAU2017Util.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CETC_GDS_AS_5100_2_2017 dialog

CETC_GDS_AS_5100_2_2017::CETC_GDS_AS_5100_2_2017(T_SPFC_D* pParamData, CWnd* pParent /*=NULL*/)
	: CMyChildDialog(CETC_GDS_AS_5100_2_2017::IDD, pParent, std::make_shared<CSpfcAU2017Util>())
{
	//{{AFX_DATA_INIT(CETC_GDS_AS_5100_2_2017)
	m_pParamData = pParamData;
	m_pSpfcUtil = std::dynamic_pointer_cast<CSpfcAU2017Util>(m_pSpfcBaseUtil);

	m_nSoilClass = 0;

	//}}AFX_DATA_INIT
}

void CETC_GDS_AS_5100_2_2017::DoDataExchange(CDataExchange* pDX)
{
	CMyChildDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CETC_GDS_AS_5100_2_2017)
	DDX_Radio   (pDX, IDC_ETC_GDS_AS_5100_2_2017_SOIL_CLASS_A_RDO, m_nSoilClass);
	DDX_Control (pDX, IDC_ETC_GDS_AS_5100_2_2017_KP_EDT,           m_edtkp);
	DDX_Control (pDX, IDC_ETC_GDS_AS_5100_2_2017_Z_EDT,            m_edtZ);
	DDX_Control (pDX, IDC_ETC_GDS_AS_5100_2_2017_MU_EDT,           m_edtMu);
	DDX_Control (pDX, IDC_ETC_GDS_AS_5100_2_2017_MAX_PERIOD_EDT,   m_edtMaxPeriod);
	//}}AFX_DATA_MAP	
}


BEGIN_MESSAGE_MAP(CETC_GDS_AS_5100_2_2017, CMyChildDialog)
	//{{AFX_MSG_MAP(CETC_GDS_AS_5100_2_2017)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CETC_GDS_AS_5100_2_2017::InitUnit() 
{
	m_edtkp.SetEditUnit(D_UNITSYS_NONE);
	m_edtZ.SetEditUnit(D_UNITSYS_NONE);
	m_edtMu.SetEditUnit(D_UNITSYS_NONE);
	m_edtMaxPeriod.SetEditUnit(D_UNITSYS_NONE);
}

/////////////////////////////////////////////////////////////////////////////
// CETC_GDS_AS_5100_2_2017 message handlers

BOOL CETC_GDS_AS_5100_2_2017::OnInitDialog() 
{
	CMyChildDialog::OnInitDialog();
	
	InitUnit();

	m_pParamData->CodeParam.AS510022017.Initialize();

	Data2Dlg();
		
	UpdateData(FALSE);

	GetDlgItem(IDC_ETC_GDS_AS_5100_2_2017_MU_STC)->SetWindowText(_LS(IDS_WG_CMD__ADD_AS_5100_2_2017_MU));
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
	
void CETC_GDS_AS_5100_2_2017::End()
{
	UpdateData(TRUE);

	Dlg2UtilData();
	SetValue();

	CMyChildDialog::End();
}

void CETC_GDS_AS_5100_2_2017::SetValue()
{
	m_pSpfcUtil->SetParamFromCalc();
}

BOOL CETC_GDS_AS_5100_2_2017::ISVALID()
{ 
	UpdateData(TRUE);

	const double dkp = m_edtkp.GetEditValue();
	const double dZ  = m_edtZ.GetEditValue();
	const double dMu = m_edtMu.GetEditValue();
	const double dMaxPeriod = m_edtMaxPeriod.GetEditValue();

	if(dkp <= 0)        {AfxMessageBox(_LS(IDS_CMD_SPECTRUM__FAILURE_AS510022017_KP)); return FALSE;}  
	if(dZ <= 0)         {AfxMessageBox(_LS(IDS_CMD_SPECTRUM__FAILURE_AS510022017_Z));  return FALSE;}  
	if(dMu <= 0)        {AfxMessageBox(_LS(IDS_CMD_SPECTRUM__FAILURE_AS510022017_Mu)); return FALSE;}  
	if(dMaxPeriod <= 0) {AfxMessageBox(_LS(IDS_CMD_SPECTRUM__MAX_PERIOD_GREATER_0));   return FALSE;}  
	
	return TRUE;  
}

BOOL CETC_GDS_AS_5100_2_2017::Dlg2Data()
{
	m_pParamData->CodeParam.AS510022017.nSoilClass = m_nSoilClass;
	m_pParamData->CodeParam.AS510022017.dkp        = m_pSpfcUtil->GetKp();
	m_pParamData->CodeParam.AS510022017.dZ         = m_pSpfcUtil->GetZ();
	m_pParamData->CodeParam.AS510022017.dMu        = m_pSpfcUtil->GetMu();
	m_pParamData->CodeParam.AS510022017.dMaxPeriod = m_pSpfcUtil->GetMaxPeriod();

	return TRUE;
}

void CETC_GDS_AS_5100_2_2017::Data2Dlg()
{
	SetPushOverData(TRUE);
}

void CETC_GDS_AS_5100_2_2017::SetPushOverData(BOOL bInit/*=TRUE*/)
{
	m_nSoilClass = m_pParamData->CodeParam.AS510022017.nSoilClass;
	m_pSpfcUtil->SetKp(m_pParamData->CodeParam.AS510022017.dkp);
	m_pSpfcUtil->SetZ(m_pParamData->CodeParam.AS510022017.dZ);
	m_pSpfcUtil->SetMu(m_pParamData->CodeParam.AS510022017.dMu);
	m_pSpfcUtil->SetMaxPeriod(m_pParamData->CodeParam.AS510022017.dMaxPeriod);

	SetValue();

	if(bInit)
	{			
		m_edtkp.SetEditUnit(m_pSpfcUtil->GetKp());
		m_edtZ.SetEditUnit(m_pSpfcUtil->GetZ());
		m_edtMu.SetEditUnit(m_pSpfcUtil->GetMu());

		m_edtMaxPeriod.SetEditUnit(m_pSpfcUtil->GetMaxPeriod());

		UpdateData(FALSE);
	}

	Dlg2UtilData();
}

void CETC_GDS_AS_5100_2_2017::Dlg2UtilData()
{
	m_pSpfcUtil->SetSoilClass(m_nSoilClass);
	m_pSpfcUtil->SetKp(m_edtkp.GetEditValue());
	m_pSpfcUtil->SetZ(m_edtZ.GetEditValue());
	m_pSpfcUtil->SetMu(m_edtMu.GetEditValue());
	m_pSpfcUtil->SetMaxPeriod(m_edtMaxPeriod.GetEditValue());
}