// ETC_GDS_IS2002.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "ETC_GDS_IS2002.h"

#include "SpfcIS2002Util.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CETC_GDS_IS2002 dialog


CETC_GDS_IS2002::CETC_GDS_IS2002(T_SPFC_D* pParamData, CWnd* pParent /*=NULL*/)
	: CMyChildDialog(CETC_GDS_IS2002::IDD, pParent, std::make_shared<CSpfcIS2002Util>())
{
	//{{AFX_DATA_INIT(CETC_GDS_IS2002)
	m_nSoil = 0;
	m_nZone = 0;
	m_dMaxPeriod = PRD_ED;
	//}}AFX_DATA_INIT  
	m_pParamData = pParamData;
	m_pSpfcUtil = std::dynamic_pointer_cast<CSpfcIS2002Util>(m_pSpfcBaseUtil);
}

void CETC_GDS_IS2002::DoDataExchange(CDataExchange* pDX)
{
	CMyChildDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CETC_GDS_IS2002)
	DDX_Radio(pDX, IDC_ETC_GDS_IS2002_ZONE_RDO, m_nZone);
	DDX_Radio(pDX, IDC_ETC_GDS_IS2002_SOIL_RDO, m_nSoil);
	DDX_Control(pDX, IDC_ETC_GDS_IS2002_DAMPING, m_wndIs02Damping);
	DDX_Control(pDX, IDC_ETC_GDS_IS2002_DAMPING_FACTOR, m_wndIs02DampingFactor);
	DDX_Control(pDX, IDC_ETC_GDS_IS2002_IMPORTANCE_CMB, m_wndIs02ImportanceCmb);
	DDX_Control(pDX, IDC_ETC_GDS_IS2002_RESPONSE_CMB, m_wndIs02RCmb);	
	DDX_Text(pDX, IDC_CMD_PERIOD_EDIT, m_dMaxPeriod);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CETC_GDS_IS2002, CMyChildDialog)
	//{{AFX_MSG_MAP(CETC_GDS_IS2002)
	ON_EN_CHANGE(IDC_ETC_GDS_IS2002_DAMPING, OnChangeEtcGdsIs2002Damping)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CETC_GDS_IS2002 message handlers
BOOL CETC_GDS_IS2002::OnInitDialog() 
{
	CMyChildDialog::OnInitDialog();
	  	
	m_wndIs02Damping.SetEditUnit(5.0);
	m_wndIs02DampingFactor.SetEditUnit(1.0); 
	
	CString strImportance[] = {_T("1.0"),_T("1.5")};  
	CString strResponse[]   = {_T("1.5"),_T("2.5"),_T("3.0"),_T("4.0"),_T("4.5"),_T("5.0")};
	for(int i = 0; i < 2; i++)  m_wndIs02ImportanceCmb.AddString(strImportance[i]);
	for(int i = 0; i < 6; i++)  m_wndIs02RCmb.AddString(strResponse[i]);
	m_wndIs02ImportanceCmb.SetCurSel(0);
	m_wndIs02RCmb.SetCurSel(2);
		 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CETC_GDS_IS2002::UpdateDampingWnd(CWnd* pDampingWnd)
{
	CString csDamping;
	csDamping.Format(_T("%g"), m_pSpfcUtil->GetDamping()/100.0);
	pDampingWnd->SetWindowText(csDamping);
}
	
void CETC_GDS_IS2002::End()
{
	UpdateData(TRUE);
	Dlg2UtilData();

	CMyChildDialog::End();
}

BOOL CETC_GDS_IS2002::ISVALID()
{  
	if(!IsValidCmb_d(&m_wndIs02ImportanceCmb,0,10,1.0)) return FALSE;
	if(!IsValidCmb_d(&m_wndIs02RCmb,0,10,1.0)) return FALSE;
	if(m_dMaxPeriod <= 0) {AfxMessageBox(_LS(IDS_CMD_SPECTRUM__MAX_PERIOD_GREATER_0)); return FALSE;}  
	return TRUE;  
}

void CETC_GDS_IS2002::OnChangeEtcGdsIs2002Damping() 
{
	double dDamping = m_wndIs02Damping.GetEditValue();
	m_wndIs02DampingFactor.SetEditUnit(m_pSpfcUtil->GetDampingFactorIS2002(dDamping));		
}

BOOL CETC_GDS_IS2002::Dlg2Data()
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
void CETC_GDS_IS2002::SetPushOverData(BOOL bInit/*=TRUE*/)
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

void CETC_GDS_IS2002::Dlg2UtilData()
{
	CString strI, strR;
	m_wndIs02ImportanceCmb.GetWindowText(strI);
	m_wndIs02RCmb.GetWindowText(strR);

	m_pSpfcUtil->SetZone(m_nZone);
	m_pSpfcUtil->SetSoil(m_nSoil);
	m_pSpfcUtil->SetDamping(m_wndIs02Damping.GetEditValue());
	m_pSpfcUtil->SetDampingFactor(m_pSpfcUtil->GetDampingFactorIS2002(m_pSpfcUtil->GetDamping()));
	m_pSpfcUtil->SetI(_tstof(strI));
	m_pSpfcUtil->SetR(_tstof(strR));
	m_pSpfcUtil->SetMaxPeriod(m_dMaxPeriod);
	m_pSpfcUtil->SetZ(IS2002::ZONE[m_nZone]);
}
