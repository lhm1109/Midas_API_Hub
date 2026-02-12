// ETC_GDS_CH_GBJ111_87.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "ETC_GDS_CH_GBJ111_87.h"

#include "SpfcCHGBJ1187Util.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CETC_GDS_CH_GBJ111_87 dialog


CETC_GDS_CH_GBJ111_87::CETC_GDS_CH_GBJ111_87(T_SPFC_D* pParamData, CWnd* pParent /*=NULL*/)
	: CMyChildDialog(CETC_GDS_CH_GBJ111_87::IDD, pParent, std::make_shared<CSpfcCHGBJ1187Util>())
{
	//{{AFX_DATA_INIT(CETC_GDS_CH_GBJ111_87)
	m_pParamData = pParamData;
	m_pSpfcUtil = std::dynamic_pointer_cast<CSpfcCHGBJ1187Util>(m_pSpfcBaseUtil);
	m_wndSFI = -1;
	m_wndSC = -1;	
	m_dMaxPeriod = PRD_ED;  
	//}}AFX_DATA_INIT
}


void CETC_GDS_CH_GBJ111_87::DoDataExchange(CDataExchange* pDX)
{
	CMyChildDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CETC_GDS_CH_GBJ111_87)
	DDX_Control(pDX, IDD_ETC_GDS_CH_GBJ111_87_KH_EDIT, m_wndKh);
	DDX_Control(pDX, IDD_ETC_GDS_CH_GBJ111_87_CZ_EDIT, m_wndCz);
	DDX_Text(pDX, IDC_CMD_PERIOD_EDIT, m_dMaxPeriod);
	DDX_Control(pDX, IDC_ETC_GDS_CH_GBJ111_87_TG1_EDIT, m_wndTg1);
	DDX_Control(pDX, IDC_ETC_GDS_CH_GBJ111_87_TG2_EDIT, m_wndTg2);
	DDX_Control(pDX, IDC_ETC_GDS_CH_GBJ111_87_TG1_EDIT, m_wndTg1);
	DDX_Control(pDX, IDC_ETC_GDS_CH_GBJ111_87_TG2_EDIT, m_wndTg2);
	DDX_Radio(pDX, IDD_ETC_GDS_CH_GBJ111_87_RDO_SFI1, m_wndSFI);
	DDX_Radio(pDX, IDD_ETC_GDS_CH_GBJ111_87_RDO_SITE1, m_wndSC);	
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CETC_GDS_CH_GBJ111_87, CMyChildDialog)
	//{{AFX_MSG_MAP(CETC_GDS_CH_GBJ111_87)
	ON_EN_CHANGE(IDC_ETC_GDS_CH_GBJ111_87_TG1_EDIT, OnChangeTg1)
	ON_BN_CLICKED(IDD_ETC_GDS_CH_GBJ111_87_RDO_SFI1, OnEtcGdsChGbj11189RdoSfi)
	ON_BN_CLICKED(IDD_ETC_GDS_CH_GBJ111_87_RDO_SFI2, OnEtcGdsChGbj11189RdoSfi)
	ON_BN_CLICKED(IDD_ETC_GDS_CH_GBJ111_87_RDO_SFI3, OnEtcGdsChGbj11189RdoSfi)
	ON_BN_CLICKED(IDD_ETC_GDS_CH_GBJ111_87_RDO_SITE1, OnEtcGdsChGbj11189RdoSite)
	ON_BN_CLICKED(IDD_ETC_GDS_CH_GBJ111_87_RDO_SITE2, OnEtcGdsChGbj11189RdoSite)
	ON_BN_CLICKED(IDD_ETC_GDS_CH_GBJ111_87_RDO_SITE3, OnEtcGdsChGbj11189RdoSite)	
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CETC_GDS_CH_GBJ111_87 message handlers

BOOL CETC_GDS_CH_GBJ111_87::OnInitDialog() 
{
	CMyChildDialog::OnInitDialog();

	m_wndSFI = 0;
	m_wndSC	 = 0;

	m_wndKh.SetEditUnit(m_pSpfcUtil->GetKh());
	m_wndTg1.SetEditUnit(m_pSpfcUtil->GetTg1());
	m_wndTg2.SetEditUnit(m_pSpfcUtil->GetTg2());
	m_wndCz.SetEditUnit(m_pSpfcUtil->GetCz());

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CETC_GDS_CH_GBJ111_87::End()
{ 
	m_pSpfcUtil->SetKh(m_wndKh.GetEditValue());
	m_pSpfcUtil->SetTg1(m_wndTg1.GetEditValue());
	m_pSpfcUtil->SetTg2(m_wndTg2.GetEditValue());
	m_pSpfcUtil->SetCz(m_wndCz.GetEditValue());

	const double mu = 2.25 * m_pSpfcUtil->GetTg1();
	m_pSpfcUtil->SetMu(mu);

	m_pSpfcUtil->SetSFI(m_wndSFI);
	m_pSpfcUtil->SetSC(m_wndSC);
	m_pSpfcUtil->SetMaxPeriod(m_dMaxPeriod);

	CMyChildDialog::End();
}

void CETC_GDS_CH_GBJ111_87::OnEtcGdsChGbj11189RdoSfi() 
{
	// TODO: Add your control notification handler code here
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

void CETC_GDS_CH_GBJ111_87::OnEtcGdsChGbj11189RdoSite() 
{
	UpdateData(TRUE);
	double dTg1;  
	switch(m_wndSC)  // Site Class에 따른 Site 평가지수 값 설정
	{
		case 0: dTg1 = 0.2; break;
		case 1: dTg1 = 0.3; break;
		case 2: dTg1 = 0.7; break;		
	}    
	m_wndTg1.SetEditUnit(dTg1);
	OnChangeTg1();
}

void CETC_GDS_CH_GBJ111_87::OnChangeTg1()
{
	UpdateData(TRUE);
	double dTg1 = m_wndTg1.GetEditValue();
	double dTg2 = dTg1 * 5.0;  
	m_wndTg2.SetEditUnit(dTg2);  
}

BOOL CETC_GDS_CH_GBJ111_87::ISVALID()
{
	if(!IsValid_d(&m_wndTg1,0.0,100,0.2)) return FALSE;
	if(m_dMaxPeriod <= 0) {AfxMessageBox(_LS(IDS_CMD_SPECTRUM__MAX_PERIOD_GREATER_0)); return FALSE;}

	m_pSpfcUtil->SetCz(m_wndCz.GetEditValue());
	if(m_pSpfcUtil->GetCz() <= 0) {AfxMessageBox(_LS(IDS_CMD_SPECTRUM__GENERAL_EFFECT_FACTOR)); return FALSE;}

	return TRUE;  
}
BOOL CETC_GDS_CH_GBJ111_87::Dlg2Data()
{
	m_pParamData->CodeParam.CHGBJ111_87.nSFI = m_wndSFI;
	m_pParamData->CodeParam.CHGBJ111_87.dKh  = m_pSpfcUtil->GetKh();
	m_pParamData->CodeParam.CHGBJ111_87.nSc  = m_wndSC;
	m_pParamData->CodeParam.CHGBJ111_87.dTg1 = m_pSpfcUtil->GetTg1();
	m_pParamData->CodeParam.CHGBJ111_87.dTg2 = m_pSpfcUtil->GetTg2();
	m_pParamData->CodeParam.CHGBJ111_87.dCz  = m_pSpfcUtil->GetCz();
	m_pParamData->CodeParam.CHGBJ111_87.dMaxPeriod = m_dMaxPeriod;

	return TRUE;
}
void CETC_GDS_CH_GBJ111_87::SetPushOverData(BOOL bInit/*=TRUE*/)
{
	m_wndSFI = m_pParamData->CodeParam.CHGBJ111_87.nSFI;
	m_pSpfcUtil->SetKh(m_pParamData->CodeParam.CHGBJ111_87.dKh);
	m_wndSC = m_pParamData->CodeParam.CHGBJ111_87.nSc;
	m_pSpfcUtil->SetTg1(m_pParamData->CodeParam.CHGBJ111_87.dTg1);
	m_pSpfcUtil->SetTg2(m_pParamData->CodeParam.CHGBJ111_87.dTg2);
	m_pSpfcUtil->SetCz(m_pParamData->CodeParam.CHGBJ111_87.dCz);
	m_dMaxPeriod = m_pParamData->CodeParam.CHGBJ111_87.dMaxPeriod;

	m_pSpfcUtil->SetSFI(m_wndSFI);
	m_pSpfcUtil->SetSC(m_wndSC);
	m_pSpfcUtil->SetMaxPeriod(m_dMaxPeriod);

	const double mu = 2.25 * m_pSpfcUtil->GetTg1();
	m_pSpfcUtil->SetMu(mu);
	if(bInit)
	{
		UpdateData(FALSE);
		m_wndKh.SetEditUnit(m_pSpfcUtil->GetKh());
		m_wndTg1.SetEditUnit(m_pSpfcUtil->GetTg1());
		OnChangeTg1();

		CString strCz;
		strCz.Format(_T("%g"), m_pSpfcUtil->GetCz());
		m_wndCz.SetWindowText(strCz);   
	}
}