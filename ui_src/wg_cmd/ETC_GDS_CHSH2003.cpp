// ETC_GDS_CHSH2003.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "ETC_GDS_CHSH2003.h"

#include "SpfcCHSH2003Util.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CETC_GDS_CHSH2003 dialog


CETC_GDS_CHSH2003::CETC_GDS_CHSH2003(T_SPFC_D* pParamData, CWnd* pParent /*=NULL*/)
	: CMyChildDialog(CETC_GDS_CHSH2003::IDD, pParent, std::make_shared<CSpfcCHSH2003Util>())
{
	//{{AFX_DATA_INIT(CETC_GDS_CHSH2003)
	m_wndER = -1;	
	m_wndSFI = -1;
	m_wndSC = -1;
	m_dXi = 0.0;
	m_dMaxPeriod = PRD_ED;
	//}}AFX_DATA_INIT
	m_wndER		= 0;	
	m_wndSFI	= 0;
	m_wndSC		= 0;
	m_dXi = 0.05;
	m_dMaxEQE = 0.08;

	m_pParamData = pParamData;    // MNET:2138-GSJI-20060530
	m_pSpfcUtil = std::dynamic_pointer_cast<CSpfcCHSH2003Util>(m_pSpfcBaseUtil);

	SetValue();
}


void CETC_GDS_CHSH2003::DoDataExchange(CDataExchange* pDX)
{
	CMyChildDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CETC_GDS_CHSH2003)
	DDX_Radio(pDX, IDD_ETC_GDS_CH2002_RDO_ER1, m_wndER);	
	DDX_Radio(pDX, IDD_ETC_GDS_CH2002_RDO_SFI1, m_wndSFI);
	DDX_Radio(pDX, IDD_ETC_GDS_CH2002_RDO_Site1, m_wndSC);
	DDX_Control(pDX, IDD_ETC_GDS_CH2002_TG_EDIT, m_wndTg);
	DDX_Text(pDX, IDD_ETC_GDS_CH2002_XI_EDIT, m_dXi);
	DDX_Text(pDX, IDC_CMD_PERIOD_EDIT, m_dMaxPeriod);	
	DDX_Text(pDX,  IDD_ETC_GDS_CH2002_MAXEQE_EDIT, m_dMaxEQE);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CETC_GDS_CHSH2003, CMyChildDialog)
	//{{AFX_MSG_MAP(CETC_GDS_CHSH2003)
	ON_BN_CLICKED(IDD_ETC_GDS_CH2002_RDO_ER1, OnERClicked)
	ON_BN_CLICKED(IDD_ETC_GDS_CH2002_RDO_ER2, OnERClicked)
	ON_BN_CLICKED(IDD_ETC_GDS_CH2002_RDO_Site1, OnCalculateTg)
	ON_BN_CLICKED(IDD_ETC_GDS_CH2002_RDO_Site2, OnCalculateTg)
	ON_BN_CLICKED(IDD_ETC_GDS_CH2002_RDO_SFI1, OnSFIClicked)
	ON_BN_CLICKED(IDD_ETC_GDS_CH2002_RDO_SFI2, OnSFIClicked)
	ON_BN_CLICKED(IDD_ETC_GDS_CH2002_RDO_SFI3, OnSFIClicked)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CETC_GDS_CHSH2003 message handlers

BOOL CETC_GDS_CHSH2003::OnInitDialog() 
{
	CMyChildDialog::OnInitDialog();
	
	m_wndTg.SetEditUnit(m_pSpfcUtil->GetTg());
	 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CETC_GDS_CHSH2003::Dlg2Data()
{
	// UpdateData(TRUE); // CETC_DESIGN_SPECT에서 Dlg2Data()호출 전에 이미 호출되었다.  
	m_pParamData->CodeParam.CHSH2003.Initialize();  
	m_pParamData->nSpecCodeType = D_SPFC_CODE_CHSH2003;    
	m_pParamData->CodeParam.CHSH2003.nSeisFortification = m_wndSFI;
	m_pParamData->CodeParam.CHSH2003.nSiteClass         = m_wndSC;   	 
	m_pParamData->CodeParam.CHSH2003.nEarthResponse     = m_wndER;  
	m_pParamData->CodeParam.CHSH2003.dTg                = m_wndTg.GetEditValue();               
	m_pParamData->CodeParam.CHSH2003.dDamping           = m_dXi;
	m_pParamData->CodeParam.CHSH2003.dMaxEQE            = m_dMaxEQE;
	m_pParamData->CodeParam.CHSH2003.dMaxPeriod = m_dMaxPeriod;
	
	return TRUE;
}

void CETC_GDS_CHSH2003::Data2Dlg()
{
	if(m_pParamData->nSpecCodeType!=D_SPFC_CODE_CHSH2003)
	{   
		m_pParamData->CodeParam.Initialize(D_SPFC_CODE_CHSH2003);    
	}
	m_wndSFI = m_pParamData->CodeParam.CHSH2003.nSeisFortification;
	m_wndSC = m_pParamData->CodeParam.CHSH2003.nSiteClass;
	m_wndER = m_pParamData->CodeParam.CHSH2003.nEarthResponse;
	m_wndTg.SetEditUnit(m_pParamData->CodeParam.CHSH2003.dTg);
	m_dXi = m_pParamData->CodeParam.CHSH2003.dDamping;
	m_dMaxEQE = m_pParamData->CodeParam.CHSH2003.dMaxEQE;
	m_dMaxPeriod = m_pParamData->CodeParam.CHSH2003.dMaxPeriod;

	Dlg2UtilData();
	UpdateData(FALSE);
}

void CETC_GDS_CHSH2003::Initialize()
{
	m_pParamData->CodeParam.Initialize(D_SPFC_CODE_CHSH2003);
}

void CETC_GDS_CHSH2003::ShowDlgData() 
{
	if(m_pParamData->nSpecCodeType == D_SPFC_CODE_CHSH2003)
		m_CodeD.CHSH2003 = m_pParamData->CodeParam.CHSH2003;
	else
		m_CodeD.CHSH2003.Initialize();
	m_wndSFI = m_CodeD.CHSH2003.nSeisFortification;
	m_wndSC  = m_CodeD.CHSH2003.nSiteClass        ;
	m_wndER  = m_CodeD.CHSH2003.nEarthResponse    ;
	m_wndTg.SetEditUnit(m_CodeD.CHSH2003.dTg);
	m_dXi    = m_CodeD.CHSH2003.dDamping          ;
	m_dMaxEQE= m_CodeD.CHSH2003.dMaxEQE           ;
	m_dMaxPeriod = m_pParamData->CodeParam.CHSH2003.dMaxPeriod;

	Dlg2UtilData();
	UpdateData(FALSE);
}

void CETC_GDS_CHSH2003::UpdateDampingWnd(CWnd* pDampingWnd)
{
	CString csDamping;
	csDamping.Format(_T("%g"), m_dXi);
	pDampingWnd->SetWindowText(csDamping);
}

void CETC_GDS_CHSH2003::End()
{
	UpdateData(TRUE);

	Dlg2UtilData();
	UpdateData(FALSE);
	SetValue();
		
	CMyChildDialog::End();
}

void CETC_GDS_CHSH2003::SetValue()
{
	m_pSpfcUtil->SetParamFromCalc();
}

void CETC_GDS_CHSH2003::OnCalculateTg() 
{
	UpdateData(TRUE);
	double dTg;

	switch(m_wndER)
	{
		case 0: // Frequent Earthquake
			if(m_wndSC == 0)      dTg = 0.65;
			else if(m_wndSC == 1) dTg = 0.90;        
		break;
		case 1: // Scarce Earthquake
			if(m_wndSC == 0)      dTg = 1.10;
			else if(m_wndSC == 1) dTg = 1.10;        
		break;      
	}

	m_wndTg.SetEditUnit(dTg);
}

BOOL CETC_GDS_CHSH2003::ISVALID()
{
	if(!IsValid_d(&m_wndTg,0.1,1.2,0.25)) return FALSE;
	if(m_dMaxPeriod <= 0) {AfxMessageBox(_LS(IDS_CMD_SPECTRUM__MAX_PERIOD_GREATER_0)); return FALSE;}  
	if(m_dXi < 0) {AfxMessageBox(_LS(IDS_CMD_SPECTRUM__DAMPING_VALUE)); return FALSE;}
	return TRUE;  
}

void CETC_GDS_CHSH2003::OnSFIClicked() 
{
	OnCalculateTg();
	UpdateMaxEQE();
}

void CETC_GDS_CHSH2003::OnERClicked() 
{
	OnCalculateTg();
	UpdateMaxEQE();
}

void CETC_GDS_CHSH2003::UpdateMaxEQE() 
{
	UpdateData(TRUE);

	double dAlpha0[3] = {0.04, 0.08, 0.16};
	double dAlpha1[3] = {0.0 , 0.45, 0.81};
	switch(m_wndER)
	{
	case 0: m_dMaxEQE = dAlpha0[m_wndSFI];    break;
	case 1: m_dMaxEQE = dAlpha1[m_wndSFI];    break;
	default: break;
	}
	UpdateData(FALSE);
}

void CETC_GDS_CHSH2003::SetPushOverData(BOOL bInit/*=TRUE*/)
{
	if(m_pParamData->nSpecCodeType!=D_SPFC_CODE_CHSH2003)
	{   
		m_pParamData->CodeParam.Initialize(D_SPFC_CODE_CHSH2003);    
	}
	m_wndSFI = m_pParamData->CodeParam.CHSH2003.nSeisFortification;
	m_wndSC  = m_pParamData->CodeParam.CHSH2003.nSiteClass;    
	m_wndER  = m_pParamData->CodeParam.CHSH2003.nEarthResponse;
	m_wndTg.SetEditUnit(m_pParamData->CodeParam.CHSH2003.dTg);
	m_dXi    = m_pParamData->CodeParam.CHSH2003.dDamping;  
	m_dMaxEQE= m_pParamData->CodeParam.CHSH2003.dMaxEQE;

	m_pSpfcUtil->SetSFI(m_wndSFI);
	m_pSpfcUtil->SetSC(m_wndSC);
	m_pSpfcUtil->SetER(m_wndER);
	m_pSpfcUtil->SetXi(m_dXi);
	m_pSpfcUtil->SetMaxEQE(m_dMaxEQE);
	m_pSpfcUtil->SetMaxPeriod(m_dMaxPeriod);
	m_pSpfcUtil->SetTg(m_wndTg.GetEditValue());

	const double dGamma = 0.9 + (0.05 - m_dXi) / (0.5 + 5 * m_dXi);
	double dEta1 = 0.02 * (1.0 + (0.05 - m_dXi) / (0.06 + 2.4 * m_dXi));
	double dEta2 = 1 + (0.05 - m_dXi) / (0.06 + 1.7 * m_dXi);
	if(dEta1 < 0.0) dEta1 = 0.0;
	if(dEta2 < 0.55) dEta2 = 0.55;

	m_pSpfcUtil->SetGamma(dGamma);
	m_pSpfcUtil->SetEta1(dEta1);
	m_pSpfcUtil->SetEta2(dEta2);

	if(bInit)
	{
		UpdateData(FALSE); // CETC_DESIGN_SPECT에서 Data2Dlg()호출 후 호출하려다 여기서 했다.
		m_wndTg.SetEditUnit(m_pSpfcUtil->GetTg());
	}
}

void CETC_GDS_CHSH2003::Dlg2UtilData()
{
	m_pSpfcUtil->SetSFI(m_wndSFI);
	m_pSpfcUtil->SetSC(m_wndSC);
	m_pSpfcUtil->SetER(m_wndER);
	m_pSpfcUtil->SetXi(m_dXi);
	m_pSpfcUtil->SetMaxEQE(m_dMaxEQE);
	m_pSpfcUtil->SetMaxPeriod(m_dMaxPeriod);

	m_pSpfcUtil->SetTg(m_wndTg.GetEditValue());
}
