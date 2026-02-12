// ETC_GDS_CH_BRG89.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "ETC_GDS_CH_BRG89.h"

#include "SpfcCHBRG89Util.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CETC_GDS_CH_BRG89 dialog


CETC_GDS_CH_BRG89::CETC_GDS_CH_BRG89(T_SPFC_D* pParamData, CWnd* pParent /*=NULL*/)
	: CMyChildDialog(CETC_GDS_CH_BRG89::IDD, pParent, std::make_shared<CSpfcCHBRG89Util>())
{
	//{{AFX_DATA_INIT(CETC_GDS_CH_BRG89)
	m_pParamData = pParamData;

	m_pSpfcUtil = std::dynamic_pointer_cast<CSpfcCHBRG89Util>(m_pSpfcBaseUtil);

	m_wndSFI = -1;
	m_wndSC = -1;
	m_strCi = _T("");
	m_strCz = _T("");
//  m_dKh = 0.1;
//  m_dTg = 0.2;
//  m_dMu = 1.0;
 	m_dMaxPeriod = PRD_ED;
	//}}AFX_DATA_INIT

	SetDefaultValue();
}


void CETC_GDS_CH_BRG89::DoDataExchange(CDataExchange* pDX)
{
	CMyChildDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CETC_GDS_CH_BRG89)
	DDX_Control(pDX, IDD_ETC_GDS_CH_BRG89_CZ_CMB, m_wndCz);
	DDX_Control(pDX, IDD_ETC_GDS_CH_BRG89_CI_CMB, m_wndCi);
	DDX_Control(pDX, IDD_ETC_GDS_CH_BRG89_TG_EDIT, m_wndTg);
	DDX_Control(pDX, IDD_ETC_GDS_CH_BRG89_MU_EDIT, m_wndMu);
	DDX_Control(pDX, IDD_ETC_GDS_CH_BRG89_KH_EDIT, m_wndKh);
	DDX_Control(pDX, IDD_ETC_GDS_CH_BRG89_K_EDIT, m_wndK);
//	DDX_Text(pDX, IDD_ETC_GDS_CH_BRG89_KH_EDIT, m_dKh);
//  DDX_Text(pDX, IDD_ETC_GDS_CH_BRG89_TG_EDIT, m_dTg);
//  DDX_Text(pDX, IDD_ETC_GDS_CH_BRG89_MU_EDIT, m_dMu);
	DDX_Text(pDX, IDC_CMD_PERIOD_EDIT, m_dMaxPeriod);
	DDX_Radio(pDX, IDD_ETC_GDS_CH_BRG89_RDO_SFI1, m_wndSFI);
	DDX_Radio(pDX, IDD_ETC_GDS_CH_BRG89_RDO_SITE1, m_wndSC);
	DDX_CBString(pDX, IDD_ETC_GDS_CH_BRG89_CI_CMB, m_strCi);
	DDX_CBString(pDX, IDD_ETC_GDS_CH_BRG89_CZ_CMB, m_strCz);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CETC_GDS_CH_BRG89, CMyChildDialog)
	//{{AFX_MSG_MAP(CETC_GDS_CH_BRG89)
	ON_EN_CHANGE(IDD_ETC_GDS_CH_BRG89_TG_EDIT, OnChangeTg)
	ON_BN_CLICKED(IDD_ETC_GDS_CH_BRG89_RDO_SFI1, OnEtcGdsChBrg89RdoSfi)
	ON_BN_CLICKED(IDD_ETC_GDS_CH_BRG89_RDO_SFI2, OnEtcGdsChBrg89RdoSfi)
	ON_BN_CLICKED(IDD_ETC_GDS_CH_BRG89_RDO_SFI3, OnEtcGdsChBrg89RdoSfi)
	ON_BN_CLICKED(IDD_ETC_GDS_CH_BRG89_RDO_SITE1, OnEtcGdsChBrg89RdoSite)
	ON_BN_CLICKED(IDD_ETC_GDS_CH_BRG89_RDO_SITE2, OnEtcGdsChBrg89RdoSite)
	ON_BN_CLICKED(IDD_ETC_GDS_CH_BRG89_RDO_SITE3, OnEtcGdsChBrg89RdoSite)
	ON_BN_CLICKED(IDD_ETC_GDS_CH_BRG89_RDO_SITE4, OnEtcGdsChBrg89RdoSite)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CETC_GDS_CH_BRG89 message handlers

BOOL CETC_GDS_CH_BRG89::OnInitDialog() 
{
	CMyChildDialog::OnInitDialog();
		
	/*
	m_wndSFI = 0;
	m_wndSC	 = 0;

	m_dKh = 0.1;
	m_dTg = 0.2;
	m_dMu = 1.0;  
	m_dk  = 1.0;
	*/
	/*
	m_strCi = _T("1.0");
	m_strCz = _T("0.20");
	*/
	SetDefaultValue();
	
	m_wndCi.AddString(_T("0.6"));
	m_wndCi.AddString(_T("1.0"));
	m_wndCi.AddString(_T("1.3"));
	m_wndCi.AddString(_T("1.7"));
	
	m_wndCz.AddString(_T("0.20"));
	m_wndCz.AddString(_T("0.25"));
	m_wndCz.AddString(_T("0.30"));
	m_wndCz.AddString(_T("0.33"));
	m_wndCz.AddString(_T("0.35"));

		 
	m_wndKh.SetEditUnit(m_pSpfcUtil->GetKh());
	m_wndTg.SetEditUnit(m_pSpfcUtil->GetTg());
	m_wndMu.SetEditUnit(m_pSpfcUtil->GetMu());
	m_wndK.SetEditUnit(m_pSpfcUtil->GetK());

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
	
void CETC_GDS_CH_BRG89::End()
{
	m_pSpfcUtil->SetKh(m_wndKh.GetEditValue());
	m_pSpfcUtil->SetTg(m_wndTg.GetEditValue());
	m_pSpfcUtil->SetMu(m_wndMu.GetEditValue());
	m_pSpfcUtil->SetK(m_wndK.GetEditValue());
	m_pSpfcUtil->SetCi(_tstof(m_strCi));
	m_pSpfcUtil->SetCz(_tstof(m_strCz));

	m_pSpfcUtil->SetSFI(m_wndSFI);
	m_pSpfcUtil->SetSC(m_wndSC);
	m_pSpfcUtil->SetMaxPeriod(m_dMaxPeriod);
	
	CMyChildDialog::End();
}

void CETC_GDS_CH_BRG89::SetDefaultValue()
{
	m_wndSFI = 0;
	m_wndSC	 = 0;

	m_strCi = _T("1.0");
	m_strCz = _T("0.20");
}

void CETC_GDS_CH_BRG89::OnEtcGdsChBrg89RdoSfi() 
{
	UpdateData(TRUE);
	double dKh;
	switch(m_wndSFI)  // seismic fortification intensity(항진설방열도 : 지진구역)에 따른 수평지진계수 값 설정
	{
		case 0: dKh = 0.1; break;
		case 1: dKh = 0.2; break;
		case 2: dKh = 0.4; break;
	}
	m_wndKh.SetEditUnit(dKh);
}

void CETC_GDS_CH_BRG89::OnEtcGdsChBrg89RdoSite() 
{
	UpdateData(TRUE);
	double dTg;
	switch(m_wndSC)  // Site Class에 따른 Site 평가지수 값 설정
	{
		case 0: dTg = 0.2; break;
		case 1: dTg = 0.3; break;
		case 2: dTg = 0.45; break;
		case 3: dTg = 0.7; break;
	}  
	m_wndTg.SetEditUnit(dTg);
	OnChangeTg();
}

void CETC_GDS_CH_BRG89::OnChangeTg()
{
	UpdateData(TRUE);
	double dTg = m_wndTg.GetEditValue();
	double dMu = 2 * (0.7 - dTg);
	double dk  = 0.9 + 0.1 * dMu;  
	m_wndMu.SetEditUnit(dMu);
	m_wndK.SetEditUnit(dk);
}

BOOL CETC_GDS_CH_BRG89::ISVALID()
{  
	if(!IsValidCmb_d(&m_wndCi,0,10,1.0)) return FALSE;
	if(!IsValidCmb_d(&m_wndCz,0,10,1.0)) return FALSE;  
	if(!IsValid_d(&m_wndKh,0.0,100,0.1)) return FALSE;
	if(!IsValid_d(&m_wndTg,0.1,100,0.2)) return FALSE;
	if(m_dMaxPeriod <= 0) {AfxMessageBox(_LS(IDS_CMD_SPECTRUM__MAX_PERIOD_GREATER_0)); return FALSE;}  
	return TRUE;  
}

BOOL CETC_GDS_CH_BRG89::Dlg2Data()
{
	m_pParamData->CodeParam.CHBRG89.nSFI = m_wndSFI;
	m_pParamData->CodeParam.CHBRG89.dKh  = m_pSpfcUtil->GetKh();
	m_pParamData->CodeParam.CHBRG89.nSc  = m_wndSC;
	m_pParamData->CodeParam.CHBRG89.dTg  = m_pSpfcUtil->GetTg();
	m_pParamData->CodeParam.CHBRG89.dCi  = m_pSpfcUtil->GetCi();
	m_pParamData->CodeParam.CHBRG89.dCz  = m_pSpfcUtil->GetCz();
	m_pParamData->CodeParam.CHBRG89.dMaxPeriod = m_dMaxPeriod;

	return TRUE;
}
void CETC_GDS_CH_BRG89::SetPushOverData(BOOL bInit/*=TRUE*/)
{
	
	m_wndSFI = m_pParamData->CodeParam.CHBRG89.nSFI;
	m_pSpfcUtil->SetKh(m_pParamData->CodeParam.CHBRG89.dKh);
	m_wndSC = m_pParamData->CodeParam.CHBRG89.nSc;
	m_pSpfcUtil->SetTg(m_pParamData->CodeParam.CHBRG89.dTg);
	m_pSpfcUtil->SetCi(m_pParamData->CodeParam.CHBRG89.dCi);
	m_pSpfcUtil->SetCz(m_pParamData->CodeParam.CHBRG89.dCz);
	m_dMaxPeriod = m_pParamData->CodeParam.CHBRG89.dMaxPeriod;

	m_pSpfcUtil->SetSFI(m_wndSFI);
	m_pSpfcUtil->SetSC(m_wndSC);
	m_pSpfcUtil->SetMaxPeriod(m_dMaxPeriod);


	CString strCi, strCz;
	strCi.Format(_T("%g"), m_pSpfcUtil->GetCi());
	strCz.Format(_T("%g"), m_pSpfcUtil->GetCz());
	m_strCi = strCi;
	m_strCz = strCz;

	if(bInit)
	{
		UpdateData(FALSE);
		m_wndKh.SetEditUnit(m_pSpfcUtil->GetKh());
		m_wndTg.SetEditUnit(m_pSpfcUtil->GetTg());
		OnChangeTg();

		m_wndCz.SetWindowText(strCz);   
		m_wndCi.SetWindowText(strCi);
	}
}