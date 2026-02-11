// ETC_GDS_IRCSP114_2018.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "ETC_GDS_IRCSP114_2018.h"

#include "SpfcIRC2018Util.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CETC_GDS_IRCSP114_2018 dialog


CETC_GDS_IRCSP114_2018::CETC_GDS_IRCSP114_2018(T_SPFC_D* pParamData, CWnd* pParent /*=NULL*/)
	: CMyChildDialog(CETC_GDS_IRCSP114_2018::IDD, pParent, std::make_shared<CSpfcIRC2018Util>())
{
	//{{AFX_DATA_INIT(CETC_GDS_IRCSP114_2018)
	m_nSoil = 0;
	m_nZone = 0;
	m_dMaxPeriod = PRD_ED;
	//}}AFX_DATA_INIT  
	m_pParamData = pParamData;
	m_pSpfcUtil = std::dynamic_pointer_cast<CSpfcIRC2018Util>(m_pSpfcBaseUtil);
}

void CETC_GDS_IRCSP114_2018::DoDataExchange(CDataExchange* pDX)
{
	CMyChildDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CETC_GDS_IRCSP114_2018)
	DDX_Radio(pDX, IDC_ETC_GDS_IS2002_ZONE_RDO, m_nZone);
	DDX_Radio(pDX, IDC_ETC_GDS_IS2002_SOIL_RDO, m_nSoil);
	DDX_Control(pDX, IDC_ETC_GDS_IS2002_DAMPING, m_wndIs02Damping);
	DDX_Control(pDX, IDC_ETC_GDS_IS2002_DAMPING_FACTOR, m_wndIs02DampingFactor);
	DDX_Control(pDX, IDC_ETC_GDS_IS2002_IMPORTANCE_CMB, m_wndIs02ImportanceCmb);
	DDX_Control(pDX, IDC_ETC_GDS_IS2002_RESPONSE_CMB, m_wndIs02RCmb);
	DDX_Control(pDX, IDC_ETC_GDS_IS2002_DAMPING2, m_wndUserDefSeisZone);
	DDX_Text(pDX, IDC_CMD_PERIOD_EDIT, m_dMaxPeriod);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CETC_GDS_IRCSP114_2018, CMyChildDialog)
	//{{AFX_MSG_MAP(CETC_GDS_IRCSP114_2018)
	ON_EN_CHANGE(IDC_ETC_GDS_IS2002_DAMPING, OnChangeEtcGdsIs2002Damping)	
	ON_BN_CLICKED(IDC_ETC_GDS_IS2002_ZONE_RDO, OnBnClickedEtcGdsIs2002ZoneRdo5)
	ON_BN_CLICKED(IDC_ETC_GDS_IS2002_ZONE_RDO2, OnBnClickedEtcGdsIs2002ZoneRdo5)
	ON_BN_CLICKED(IDC_ETC_GDS_IS2002_ZONE_RDO3, OnBnClickedEtcGdsIs2002ZoneRdo5)
	ON_BN_CLICKED(IDC_ETC_GDS_IS2002_ZONE_RDO4, OnBnClickedEtcGdsIs2002ZoneRdo5)
	ON_BN_CLICKED(IDC_ETC_GDS_IS2002_ZONE_RDO5, OnBnClickedEtcGdsIs2002ZoneRdo5)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CETC_GDS_IRCSP114_2018 message handlers
BOOL CETC_GDS_IRCSP114_2018::OnInitDialog() 
{
	CMyChildDialog::OnInitDialog();
  	
	m_wndIs02Damping.SetEditUnit(5.0);
	m_wndIs02DampingFactor.SetEditUnit(1.0); 
	m_wndUserDefSeisZone.SetEditUnit(0.36);
	m_wndUserDefSeisZone.EnableWindow(FALSE);
	CString strImportance[] = {_T("1.0"),_T("1.2"),_T("1.5")};  
	CString strResponse[]   = {_T("1.0"),_T("1.5"),_T("2.5"),_T("3.0"),_T("4.0"),_T("4.5"),_T("5.0")};
	for(int i = 0; i < 3; i++)  m_wndIs02ImportanceCmb.AddString(strImportance[i]);
	for(int i = 0; i < 7; i++)  m_wndIs02RCmb.AddString(strResponse[i]);
	m_wndIs02ImportanceCmb.SetCurSel(0);
	m_wndIs02RCmb.SetCurSel(2);
		 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CETC_GDS_IRCSP114_2018::UpdateDampingWnd(CWnd* pDampingWnd)
{
	CString csDamping;
	csDamping.Format(_T("%g"), m_pSpfcUtil->GetDamping() / 100.0);
	pDampingWnd->SetWindowText(csDamping);
}

void CETC_GDS_IRCSP114_2018::End()
{
	UpdateData(TRUE);
	Dlg2UtilData();
	CMyChildDialog::End();
}

void CETC_GDS_IRCSP114_2018::SetCorrectRadButS(int iZF)
{
	
	if (iZF == 1)
	{
		CButton* poButton;
		poButton = (CButton*)(GetDlgItem(IDC_ETC_GDS_IS2002_ZONE_RDO2));
		poButton->SetCheck(1);
		OnBnClickedEtcGdsIs2002ZoneRdo5();
	}
	else if (iZF == 2)
	{
		CButton* poButton;
		poButton = (CButton*)(GetDlgItem(IDC_ETC_GDS_IS2002_ZONE_RDO3));
		poButton->SetCheck(1);
		OnBnClickedEtcGdsIs2002ZoneRdo5();
	}
	else if (iZF == 3)
	{
		CButton* poButton;
		poButton = (CButton*)(GetDlgItem(IDC_ETC_GDS_IS2002_ZONE_RDO4));
		poButton->SetCheck(1);
		OnBnClickedEtcGdsIs2002ZoneRdo5();
	}
	else if (iZF == 4)
	{
		CButton* poButton;
		poButton = (CButton*)(GetDlgItem(IDC_ETC_GDS_IS2002_ZONE_RDO5));
		poButton->SetCheck(1);
		OnBnClickedEtcGdsIs2002ZoneRdo5();
	}
	
	
}
BOOL CETC_GDS_IRCSP114_2018::ISVALID()
{  
	if(!IsValidCmb_d(&m_wndIs02ImportanceCmb,0,10,1.0)) return FALSE;
	if(!IsValidCmb_d(&m_wndIs02RCmb,0,10,1.0)) return FALSE;
	if(!IsValid_d(&m_wndIs02Damping,0,10,1.0)) return FALSE;
	if(m_dMaxPeriod <= 0) {AfxMessageBox(_LS(IDS_CMD_SPECTRUM__MAX_PERIOD_GREATER_0)); return FALSE;}  
	return TRUE;  
}

void CETC_GDS_IRCSP114_2018::OnChangeEtcGdsIs2002Damping() 
{
	double dDamping = m_wndIs02Damping.GetEditValue();
	m_wndIs02DampingFactor.SetEditUnit(m_pSpfcUtil->GetDampingFactorIS2002(dDamping));		
}


BOOL CETC_GDS_IRCSP114_2018::Dlg2Data()
{
	m_pParamData->CodeParam.IRCSP114.nSeisZone      = m_nZone         ;
	m_pParamData->CodeParam.IRCSP114.nSoilType      = m_nSoil         ;
	m_pParamData->CodeParam.IRCSP114.dDamping       = m_pSpfcUtil->GetDamping();
	m_pParamData->CodeParam.IRCSP114.dDampingFactor = m_pSpfcUtil->GetDampingFactor();
	m_pParamData->CodeParam.IRCSP114.dIe = m_pSpfcUtil->GetI();
	m_pParamData->CodeParam.IRCSP114.dCoef = m_pSpfcUtil->GetR();
	m_pParamData->CodeParam.IRCSP114.dMaxPeriod     = m_dMaxPeriod    ;
	if (m_nZone == 4) m_pParamData->CodeParam.IRCSP114.dUsrDefSeismicZone = m_pSpfcUtil->GetUserDefSeisZoneFact();
	
	m_pParamData->dResModCoef = m_pSpfcUtil->GetR();
	return TRUE;
}
void CETC_GDS_IRCSP114_2018::SetPushOverData(BOOL bInit/*=TRUE*/)
{                  
	m_nZone          = m_pParamData->CodeParam.IRCSP114.nSeisZone;
	m_nSoil          = m_pParamData->CodeParam.IRCSP114.nSoilType ;
	m_pSpfcUtil->SetDamping(m_pParamData->CodeParam.IRCSP114.dDamping);
	m_pSpfcUtil->SetDampingFactor(m_pParamData->CodeParam.IRCSP114.dDampingFactor);
	m_pSpfcUtil->SetI(m_pParamData->CodeParam.IRCSP114.dIe);
	m_pSpfcUtil->SetR(m_pParamData->CodeParam.IRCSP114.dCoef );
	m_dMaxPeriod     = m_pParamData->CodeParam.IRCSP114.dMaxPeriod ;
	m_pSpfcUtil->SetUserDefSeisZoneFact(m_pParamData->CodeParam.IRCSP114.dUsrDefSeismicZone);

	if(bInit)
	{
		UpdateData(FALSE);

		m_wndIs02Damping.SetEditUnit(m_pSpfcUtil->GetDamping());
		m_wndIs02DampingFactor.SetEditUnit(m_pSpfcUtil->GetDampingFactor()); 
		m_wndUserDefSeisZone.SetEditUnit(m_pSpfcUtil->GetUserDefSeisZoneFact());

		CString strI, strR;
		strI.Format(_T("%g"), m_pSpfcUtil->GetI());
		strR.Format(_T("%g"), m_pSpfcUtil->GetR());

		m_wndIs02ImportanceCmb.SetWindowText(strI);   
		m_wndIs02RCmb.SetWindowText(strR);
	}

	Dlg2UtilData();
}

void CETC_GDS_IRCSP114_2018::Dlg2UtilData()
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
	m_pSpfcUtil->SetUserDefSeisZoneFact(m_wndUserDefSeisZone.GetEditValue());

	const double dZone = m_pSpfcUtil->GetZoneValue(m_nZone);
	m_pSpfcUtil->SetZ(dZone);

}

void CETC_GDS_IRCSP114_2018::OnBnClickedEtcGdsIs2002ZoneRdo5()
{
	// TODO: Add your control notification handler code here
	CButton* poButton;
	poButton = (CButton*)(GetDlgItem(IDC_ETC_GDS_IS2002_ZONE_RDO5));
	if (poButton->GetCheck()==1) m_wndUserDefSeisZone.EnableWindow(TRUE);
	else m_wndUserDefSeisZone.EnableWindow(FALSE);
	
}
