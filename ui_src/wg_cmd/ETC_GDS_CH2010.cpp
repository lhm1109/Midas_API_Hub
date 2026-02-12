// ETC_GDS_CH2010.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "ETC_GDS_CH2010.h"

#include "SpfcCH2010Util.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CETC_GDS_CH2010 dialog

CETC_GDS_CH2010::CETC_GDS_CH2010(T_SPFC_D* pParamData, CWnd* pParent /*=NULL*/)
	: CMyChildDialog(CETC_GDS_CH2010::IDD, pParent, std::make_shared<CSpfcCH2010Util>())
{
	//{{AFX_DATA_INIT(CETC_GDS_CH2010)
	m_wndER = -1;
	m_wndSDC = -1;
	m_wndSFI = -1;
	m_wndSC = -1;
	m_dXi = 0.0;
	m_dMaxPeriod = PRD_ED;
	m_nLForce = 0;
	m_dMaxEQE = 0.08;
	//}}AFX_DATA_INIT
	/* // 아래와 같이 주소를 넘겨받으므로 불필요함. 다른 코드도 다 수정하면 지우자.
	m_wndER		= 0;
	m_wndSDC	= 0; 
	m_wndSFI	= 0;
	m_wndSC		= 0;
	m_dXi = 0.05;
	m_dTg = 0.25;
	*/
	m_pParamData = pParamData;    // MNET:2138-GSJI-20060530
	m_pSpfcUtil = std::dynamic_pointer_cast<CSpfcCH2010Util>(m_pSpfcBaseUtil);

	SetValue();
}

void CETC_GDS_CH2010::DoDataExchange(CDataExchange* pDX)
{
	CMyChildDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CETC_GDS_CH2010)
	DDX_Radio(pDX, IDD_ETC_GDS_CH2010_RDO_ER1, m_wndER);
	DDX_Radio(pDX, IDD_ETC_GDS_CH2010_RDO_SDC1, m_wndSDC);
	DDX_Radio(pDX, IDD_ETC_GDS_CH2010_RDO_SFI1, m_wndSFI);
	DDX_Radio(pDX, IDD_ETC_GDS_CH2010_RDO_Site1_0, m_wndSC);
	DDX_Control(pDX, IDD_ETC_GDS_CH2010_TG_EDIT, m_wndTg);
	DDX_Text(pDX, IDD_ETC_GDS_CH2010_XI_EDIT, m_dXi);
	DDX_Text(pDX, IDC_CMD_PERIOD_EDIT, m_dMaxPeriod);	
	DDX_Radio(pDX, IDC_CMD_LFORCE1_RDO, m_nLForce);
	DDX_Text(pDX,  IDC_ETC_GDS_CH2010_MAXEQE_EDIT, m_dMaxEQE);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CETC_GDS_CH2010, CMyChildDialog)
	//{{AFX_MSG_MAP(CETC_GDS_CH2010)
	ON_BN_CLICKED(IDD_ETC_GDS_CH2010_RDO_ER1, OnERClicked)
	ON_BN_CLICKED(IDD_ETC_GDS_CH2010_RDO_ER2, OnERClicked)
	ON_BN_CLICKED(IDD_ETC_GDS_CH2010_RDO_ER3, OnERClicked)
	ON_BN_CLICKED(IDD_ETC_GDS_CH2010_RDO_SDC1, OnCalculateTg)
	ON_BN_CLICKED(IDD_ETC_GDS_CH2010_RDO_SDC2, OnCalculateTg)
	ON_BN_CLICKED(IDD_ETC_GDS_CH2010_RDO_SDC3, OnCalculateTg)
	ON_BN_CLICKED(IDD_ETC_GDS_CH2010_RDO_SFI1, OnSFIClicked)
	ON_BN_CLICKED(IDD_ETC_GDS_CH2010_RDO_SFI2, OnSFIClicked)
	ON_BN_CLICKED(IDD_ETC_GDS_CH2010_RDO_SFI3, OnSFIClicked)
	ON_BN_CLICKED(IDD_ETC_GDS_CH2010_RDO_SFI4, OnSFIClicked)
	ON_BN_CLICKED(IDD_ETC_GDS_CH2010_RDO_SFI5, OnSFIClicked)
	ON_BN_CLICKED(IDD_ETC_GDS_CH2010_RDO_SFI6, OnSFIClicked)
	ON_BN_CLICKED(IDD_ETC_GDS_CH2010_RDO_Site1_0, OnCalculateTg)
	ON_BN_CLICKED(IDD_ETC_GDS_CH2010_RDO_Site1_1, OnCalculateTg)
	ON_BN_CLICKED(IDD_ETC_GDS_CH2010_RDO_Site2, OnCalculateTg)
	ON_BN_CLICKED(IDD_ETC_GDS_CH2010_RDO_Site3, OnCalculateTg)
	ON_BN_CLICKED(IDD_ETC_GDS_CH2010_RDO_Site4, OnCalculateTg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CETC_GDS_CH2010 message handlers

BOOL CETC_GDS_CH2010::OnInitDialog() 
{
	CMyChildDialog::OnInitDialog();
	
	m_wndTg.SetEditUnit(m_pSpfcUtil->GetTg());
	 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CETC_GDS_CH2010::Dlg2Data()
{
	// UpdateData(TRUE); // CETC_DESIGN_SPECT에서 Dlg2Data()호출 전에 이미 호출되었다.
	m_pParamData->CodeParam.CH2010.Initialize();  
	m_pParamData->nSpecCodeType = D_SPFC_CODE_CH2010;   //확인사살~  
	m_pParamData->CodeParam.CH2010.nSeisDgnCategory   = m_wndSDC;  
	m_pParamData->CodeParam.CH2010.nSeisFortification = m_wndSFI;
	m_pParamData->CodeParam.CH2010.nSiteClass         = m_wndSC;   	 
	m_pParamData->CodeParam.CH2010.nEarthResponse     = m_wndER;  
	m_pParamData->CodeParam.CH2010.dTg                = m_wndTg.GetEditValue();               
	m_pParamData->CodeParam.CH2010.dDamping           = m_dXi;
	m_pParamData->CodeParam.CH2010.nLForce						= m_nLForce;
	m_pParamData->CodeParam.CH2010.dMaxEQE            = m_dMaxEQE;
	m_pParamData->CodeParam.CH2010.dMaxPeriod = m_dMaxPeriod;

	return TRUE;
}

void CETC_GDS_CH2010::Data2Dlg()
{  
	if(m_pParamData->nSpecCodeType!=D_SPFC_CODE_CH2010)
	{
		//m_pParamData->nSpecCodeType=D_SPFC_CODE_CH2010; 여기서 이걸 하면 안된다.    
		m_pParamData->CodeParam.Initialize(D_SPFC_CODE_CH2010);    
	}
	m_wndSDC = m_pParamData->CodeParam.CH2010.nSeisDgnCategory;
	m_wndSFI = m_pParamData->CodeParam.CH2010.nSeisFortification;
	m_wndSC  = m_pParamData->CodeParam.CH2010.nSiteClass;    
	m_wndER  = m_pParamData->CodeParam.CH2010.nEarthResponse;
	m_pSpfcUtil->SetTg(m_pParamData->CodeParam.CH2010.dTg);
	m_dXi    = m_pParamData->CodeParam.CH2010.dDamping;  
	m_nLForce= m_pParamData->CodeParam.CH2010.nLForce;
	m_dMaxEQE= m_pParamData->CodeParam.CH2010.dMaxEQE;
	m_dMaxPeriod = m_pParamData->CodeParam.CH2010.dMaxPeriod;

	Dlg2UtilData();

	UpdateData(FALSE); // CETC_DESIGN_SPECT에서 Data2Dlg()호출 후 호출하려다 여기서 했다.
	m_wndTg.SetEditUnit(m_pSpfcUtil->GetTg());  
}

void CETC_GDS_CH2010::Initialize() 
{
	m_pParamData->CodeParam.Initialize(D_SPFC_CODE_CH2010);
}

void CETC_GDS_CH2010::ShowDlgData() 
{
	if(m_pParamData->nSpecCodeType == D_SPFC_CODE_CH2010)
		m_CodeD.CH2010 = m_pParamData->CodeParam.CH2010;
	else
		m_CodeD.CH2010.Initialize();
	
	m_wndSDC = m_CodeD.CH2010.nSeisDgnCategory  ;
	m_wndSFI = m_CodeD.CH2010.nSeisFortification;
	m_wndSC  = m_CodeD.CH2010.nSiteClass        ;
	m_wndER  = m_CodeD.CH2010.nEarthResponse    ;
	m_pSpfcUtil->SetTg(m_CodeD.CH2010.dTg);
	m_dXi    = m_CodeD.CH2010.dDamping          ;  
	m_nLForce= m_CodeD.CH2010.nLForce			;
	m_dMaxEQE= m_CodeD.CH2010.dMaxEQE           ;
	m_dMaxPeriod = m_pParamData->CodeParam.CH2010.dMaxPeriod;

	Dlg2UtilData();

	UpdateData(FALSE);
	m_wndTg.SetEditUnit(m_pSpfcUtil->GetTg());
}

void CETC_GDS_CH2010::UpdateDampingWnd(CWnd* pDampingWnd)
{
	CString csDamping;
	csDamping.Format(_T("%g"), m_dXi);
	pDampingWnd->SetWindowText(csDamping);
}

void CETC_GDS_CH2010::End()
{
	UpdateData(TRUE);  
	
	// MNET:XXXX-JBSEON-20061030 MakeSpectrumData할 때 m_dTg 변수 사용됨
	// 아래 문장 다시 복구
	m_pSpfcUtil->SetTg(m_wndTg.GetEditValue());//tmp

	Dlg2UtilData();
	SetValue();

	CMyChildDialog::End();
}

void CETC_GDS_CH2010::SetValue()
{
	m_pSpfcUtil->SetParamFromCalc();
}

void CETC_GDS_CH2010::OnCalculateTg() 
{
	UpdateData(TRUE);

	T_SPFC_CH2010 Spfc;
	double dTg = Spfc.GetTg(m_wndSDC, m_wndSC, m_wndER);

	m_wndTg.SetEditUnit(dTg);
}

BOOL CETC_GDS_CH2010::ISVALID()
{
	if(!IsValid_d(&m_wndTg,0.1,1.2,0.25)) return FALSE;
	if(m_dMaxPeriod <= 0) {AfxMessageBox(_LS(IDS_CMD_SPECTRUM__MAX_PERIOD_GREATER_0)); return FALSE;}  
	if(m_dXi < 0) {AfxMessageBox(_LS(IDS_CMD_SPECTRUM__DAMPING_VALUE)); return FALSE;}
	return TRUE;  
}

void CETC_GDS_CH2010::OnSFIClicked() 
{
	OnCalculateTg();
	UpdateMaxEQE();
}

void CETC_GDS_CH2010::OnERClicked() 
{
	OnCalculateTg();
	UpdateMaxEQE();
}

void CETC_GDS_CH2010::UpdateMaxEQE() 
{
	UpdateData(TRUE);

	double dAlpha0[6] = {0.04, 0.08, 0.12, 0.16, 0.24, 0.32};
	double dAlpha1[6] = {0.12, 0.23, 0.34, 0.45, 0.68, 0.90};
	double dAlpha2[6] = {0.28, 0.50, 0.72, 0.90, 1.20, 1.40};
	switch(m_wndER)
	{
	case 0: m_dMaxEQE = dAlpha0[m_wndSFI];    break;
	case 1: m_dMaxEQE = dAlpha1[m_wndSFI];    break;
	case 2: m_dMaxEQE = dAlpha2[m_wndSFI];    break;
	default: break;
	}
	UpdateData(FALSE);
}

void CETC_GDS_CH2010::SetPushOverData(BOOL bInit/*=TRUE*/)
{
	if(m_pParamData->nSpecCodeType!=D_SPFC_CODE_CH2010)
	{
		//m_pParamData->nSpecCodeType=D_SPFC_CODE_CH2010; 여기서 이걸 하면 안된다.    
		m_pParamData->CodeParam.Initialize(D_SPFC_CODE_CH2010);    
	}
	m_wndSDC = m_pParamData->CodeParam.CH2010.nSeisDgnCategory;
	m_wndSFI = m_pParamData->CodeParam.CH2010.nSeisFortification;
	m_wndSC  = m_pParamData->CodeParam.CH2010.nSiteClass;    
	m_wndER  = m_pParamData->CodeParam.CH2010.nEarthResponse;
	m_pSpfcUtil->SetTg(m_pParamData->CodeParam.CH2010.dTg);
	m_dXi    = m_pParamData->CodeParam.CH2010.dDamping;  
	m_nLForce= m_pParamData->CodeParam.CH2010.nLForce;
	m_dMaxEQE= m_pParamData->CodeParam.CH2010.dMaxEQE;

	Dlg2UtilData();
	m_pSpfcUtil->CalcEarthquakeCoefFromPushOver();

	if(bInit)
	{
		UpdateData(FALSE); // CETC_DESIGN_SPECT에서 Data2Dlg()호출 후 호출하려다 여기서 했다.
		m_wndTg.SetEditUnit(m_pSpfcUtil->GetTg());  
		SetValue();
	}
}

void CETC_GDS_CH2010::Dlg2UtilData()
{
	m_pSpfcUtil->SetSDC(m_wndSDC);
	m_pSpfcUtil->SetSFI(m_wndSFI);
	m_pSpfcUtil->SetSC(m_wndSC);
	m_pSpfcUtil->SetER(m_wndER);

	m_pSpfcUtil->SetXi(m_dXi);
	m_pSpfcUtil->SetLForce(m_nLForce);
	m_pSpfcUtil->SetMaxEQE(m_dMaxEQE);
	m_pSpfcUtil->SetMaxPeriod(m_dMaxPeriod);
}
