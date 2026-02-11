// ETC_GDS_IS1893_2016.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "ETC_GDS_IS1893_2016.h"

#include "SpfcIS2016Util.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CETC_GDS_IS1893_2016 dialog


CETC_GDS_IS1893_2016::CETC_GDS_IS1893_2016(T_SPFC_D* pParamData, CWnd* pParent /*=NULL*/)
	: CMyChildDialog(CETC_GDS_IS1893_2016::IDD, pParent, std::make_shared<CSpfcIS2016Util>())
{
	//{{AFX_DATA_INIT(CETC_GDS_IS1893_2016)
	m_nSoil = 0;
	m_nZone = 0;
	m_dMaxPeriod = PRD_ED;
	//}}AFX_DATA_INIT  
	m_pParamData = pParamData;
	m_pSpfcUtil = std::dynamic_pointer_cast<CSpfcIS2016Util>(m_pSpfcBaseUtil);
}

void CETC_GDS_IS1893_2016::DoDataExchange(CDataExchange* pDX)
{
	CMyChildDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CETC_GDS_IS1893_2016)
	DDX_Radio(pDX, IDC_ETC_GDS_IS2002_ZONE_RDO, m_nZone);
	DDX_Radio(pDX, IDC_ETC_GDS_IS2002_SOIL_RDO, m_nSoil);
	DDX_Control(pDX, IDC_ETC_GDS_IS2002_DAMPING, m_wndIs02Damping);
	DDX_Control(pDX, IDC_ETC_GDS_IS2002_DAMPING_FACTOR, m_wndIs02DampingFactor);
	DDX_Control(pDX, IDC_ETC_GDS_IS2002_IMPORTANCE_CMB, m_wndIs02ImportanceCmb);
	DDX_Control(pDX, IDC_ETC_GDS_IS2002_RESPONSE_CMB, m_wndIs02RCmb);	
	DDX_Text(pDX, IDC_CMD_PERIOD_EDIT, m_dMaxPeriod);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CETC_GDS_IS1893_2016, CMyChildDialog)
	//{{AFX_MSG_MAP(CETC_GDS_IS1893_2016)
	ON_EN_CHANGE(IDC_ETC_GDS_IS2002_DAMPING, OnChangeEtcGdsIS1893_2016Damping)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CETC_GDS_IS1893_2016 message handlers
BOOL CETC_GDS_IS1893_2016::OnInitDialog() 
{
	CMyChildDialog::OnInitDialog();
	  	
	m_wndIs02Damping.SetEditUnit(5.0);
	m_wndIs02DampingFactor.SetEditUnit(1.0); 
	
	CString strImportance[] = {_T("1.0"),_T("1.2"),_T("1.5")};  
	CString strResponse[]   = {_T("1.5"),_T("2.0"),_T("2.5"),_T("3.0"),_T("4.0"),_T("4.5"),_T("5.0")};
	for(int i = 0; i < 3; i++)  m_wndIs02ImportanceCmb.AddString(strImportance[i]);
	for(int i = 0; i < 7; i++)  m_wndIs02RCmb.AddString(strResponse[i]);  
	m_wndIs02ImportanceCmb.SetCurSel(0);
	m_wndIs02RCmb.SetCurSel(3);
		 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CETC_GDS_IS1893_2016::UpdateDampingWnd(CWnd* pDampingWnd)
{
	CString csDamping;
	csDamping.Format(_T("%g"), m_pSpfcUtil->GetDamping() / 100.0);
	pDampingWnd->SetWindowText(csDamping);
}
	
void CETC_GDS_IS1893_2016::End()
{
	UpdateData(TRUE);
	Dlg2UtilData();

	CMyChildDialog::End();
}

BOOL CETC_GDS_IS1893_2016::ISVALID()
{  
	if(!IsValidCmb_d(&m_wndIs02ImportanceCmb,0,10,1.0)) return FALSE;
	if(!IsValidCmb_d(&m_wndIs02RCmb,0,10,1.0)) return FALSE;
	if(m_dMaxPeriod <= 0) {AfxMessageBox(_LS(IDS_CMD_SPECTRUM__MAX_PERIOD_GREATER_0)); return FALSE;}  
	return TRUE;  
}

void CETC_GDS_IS1893_2016::OnChangeEtcGdsIS1893_2016Damping() 
{
	double dDamping = m_wndIs02Damping.GetEditValue();
	m_wndIs02DampingFactor.SetEditUnit(m_pSpfcUtil->GetDampingFactorIS1893_2016(dDamping));		
}

BOOL CETC_GDS_IS1893_2016::Dlg2Data()
{
	m_pParamData->CodeParam.IS1893.nSeisZone      = m_nZone         ;
	m_pParamData->CodeParam.IS1893.nSoilType      = m_nSoil         ;
	m_pParamData->CodeParam.IS1893.dDamping = m_pSpfcUtil->GetDamping();
	m_pParamData->CodeParam.IS1893.dDampingFactor = m_pSpfcUtil->GetDampingFactor();
	m_pParamData->CodeParam.IS1893.dIe = m_pSpfcUtil->GetI();
	m_pParamData->CodeParam.IS1893.dCoef = m_pSpfcUtil->GetR();
	m_pParamData->CodeParam.IS1893.dMaxPeriod     = m_dMaxPeriod    ;

	m_pParamData->dResModCoef = m_pSpfcUtil->GetR();
	return TRUE;
}
void CETC_GDS_IS1893_2016::SetPushOverData(BOOL bInit/*=TRUE*/)
{                  
	m_nZone          = m_pParamData->CodeParam.IS1893.nSeisZone;
	m_nSoil          = m_pParamData->CodeParam.IS1893.nSoilType ;
	m_pSpfcUtil->SetDamping(m_pParamData->CodeParam.IS1893.dDamping);
	m_pSpfcUtil->SetDampingFactor(m_pParamData->CodeParam.IS1893.dDampingFactor);
	m_pSpfcUtil->SetI(m_pParamData->CodeParam.IS1893.dIe);
	m_pSpfcUtil->SetR(m_pParamData->CodeParam.IS1893.dCoef);
	m_dMaxPeriod     = m_pParamData->CodeParam.IS1893.dMaxPeriod ;

	if(bInit)
	{
		UpdateData(FALSE);

		m_wndIs02Damping.SetEditUnit(m_pSpfcUtil->GetDamping());
		m_wndIs02DampingFactor.SetEditUnit(m_pSpfcUtil->GetDampingFactor());

		CString strI, strR;
		strI.Format(_T("%g"), m_pSpfcUtil->GetI());
		strR.Format(_T("%g"), m_pSpfcUtil->GetR());

		m_wndIs02ImportanceCmb.SetWindowText(strI);
		m_wndIs02RCmb.SetWindowText(strR);
	}

	Dlg2UtilData();
}


void CETC_GDS_IS1893_2016::Dlg2UtilData()
{
	CString strI, strR;
	m_wndIs02ImportanceCmb.GetWindowText(strI);
	m_wndIs02RCmb.GetWindowText(strR);

	m_pSpfcUtil->SetZone(m_nZone);
	m_pSpfcUtil->SetSoil(m_nSoil);
	m_pSpfcUtil->SetDamping(m_wndIs02Damping.GetEditValue());
	m_pSpfcUtil->SetDampingFactor(m_pSpfcUtil->GetDampingFactorIS1893_2016(m_pSpfcUtil->GetDamping()));
	m_pSpfcUtil->SetI(_tstof(strI));
	m_pSpfcUtil->SetR(_tstof(strR));
	m_pSpfcUtil->SetMaxPeriod(m_dMaxPeriod);
	m_pSpfcUtil->SetZ(IS2016::ZONE[m_nZone]);
}
