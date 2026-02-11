
#include "stdafx.h"
#include "wg_cmd.h"
#include "ETC_GDS_CH_GB_T_51408_2021.h"

#include "SpfcCHGB2021Util.h"
#include "..\wg_base\wg_base_StrParser.h"
using namespace mit::lib;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



CETC_GDS_CH_GB_T_51408_2021::CETC_GDS_CH_GB_T_51408_2021(T_SPFC_D* pParamData, CWnd* pParent /*=NULL*/)
	: CMyChildDialog(CETC_GDS_CH_GB_T_51408_2021::IDD, pParent, std::make_shared<CSpfcCHGB2021Util>())
{
	m_wndER = -1;
	m_wndSDC = -1;
	m_wndSFI = -1;
	m_wndSC = -1;
	m_dXi = 0.0;
	m_dMaxPeriod = PRD_ED;
	m_nLForce = 0;
	m_dMaxEQE = 0.08;
	m_pParamData = pParamData;
	m_pSpfcUtil = std::dynamic_pointer_cast<CSpfcCHGB2021Util>(m_pSpfcBaseUtil);
	SetValue();
}

void CETC_GDS_CH_GB_T_51408_2021::DoDataExchange(CDataExchange* pDX)
{
	CMyChildDialog::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_ETC_GDS_CH2021_RDO_ER1, m_wndER);
	DDX_Radio(pDX, IDC_ETC_GDS_CH2021_RDO_SDC1, m_wndSDC);
	DDX_Radio(pDX, IDC_ETC_GDS_CH2021_RDO_SFI1, m_wndSFI);
	DDX_Radio(pDX, IDC_ETC_GDS_CH2021_RDO_Site1_0, m_wndSC);
	DDX_Control(pDX, IDC_ETC_GDS_CH2021_TG_EDIT, m_wndTg);
	DDX_Text(pDX, IDC_ETC_GDS_CH2021_XI_EDIT, m_dXi);
	DDX_Text(pDX, IDC_CMD_PERIOD_EDIT, m_dMaxPeriod);
	DDX_Radio(pDX, IDC_CMD_LFORCE1_RDO, m_nLForce);
	DDX_Text(pDX, IDC_ETC_GDS_CH2021_MAXEQE_EDIT, m_dMaxEQE);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CETC_GDS_CH_GB_T_51408_2021, CMyChildDialog)
	//{{AFX_MSG_MAP(CETC_GDS_CH_GB_T_51408_2021)
	ON_BN_CLICKED(IDC_ETC_GDS_CH2021_RDO_ER1, OnERClicked)
	ON_BN_CLICKED(IDC_ETC_GDS_CH2021_RDO_ER2, OnERClicked)
	ON_BN_CLICKED(IDC_ETC_GDS_CH2021_RDO_ER3, OnERClicked)
	ON_BN_CLICKED(IDC_ETC_GDS_CH2021_RDO_SDC1, OnCalculateTg)
	ON_BN_CLICKED(IDC_ETC_GDS_CH2021_RDO_SDC2, OnCalculateTg)
	ON_BN_CLICKED(IDC_ETC_GDS_CH2021_RDO_SDC3, OnCalculateTg)
	ON_BN_CLICKED(IDC_ETC_GDS_CH2021_RDO_SFI1, OnSFIClicked)
	ON_BN_CLICKED(IDC_ETC_GDS_CH2021_RDO_SFI2, OnSFIClicked)
	ON_BN_CLICKED(IDC_ETC_GDS_CH2021_RDO_SFI3, OnSFIClicked)
	ON_BN_CLICKED(IDC_ETC_GDS_CH2021_RDO_SFI4, OnSFIClicked)
	ON_BN_CLICKED(IDC_ETC_GDS_CH2021_RDO_SFI5, OnSFIClicked)
	ON_BN_CLICKED(IDC_ETC_GDS_CH2021_RDO_SFI6, OnSFIClicked)
	ON_BN_CLICKED(IDC_ETC_GDS_CH2021_RDO_Site1_0, OnCalculateTg)
	ON_BN_CLICKED(IDC_ETC_GDS_CH2021_RDO_Site1_1, OnCalculateTg)
	ON_BN_CLICKED(IDC_ETC_GDS_CH2021_RDO_Site2, OnCalculateTg)
	ON_BN_CLICKED(IDC_ETC_GDS_CH2021_RDO_Site3, OnCalculateTg)
	ON_BN_CLICKED(IDC_ETC_GDS_CH2021_RDO_Site4, OnCalculateTg)
	//}}AFX_MSG_MAP
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


// CETC_GDS_CH_GB_T_51408_2021 메시지 처리기

BOOL CETC_GDS_CH_GB_T_51408_2021::OnInitDialog()
{
	CMyChildDialog::OnInitDialog();

	m_wndTg.SetEditUnit(m_pSpfcUtil->GetTg());
	Data2Dlg();
	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CETC_GDS_CH_GB_T_51408_2021::Dlg2Data()
{
	// UpdateData(TRUE); // CETC_DESIGN_SPECT에서 Dlg2Data()호출 전에 이미 호출되었다.
	m_pParamData->CodeParam.GB_T_51408_2021.Initialize();
	m_pParamData->nSpecCodeType = D_SPFC_CODE_CH_GB_T_51408_2021;   
	m_pParamData->CodeParam.GB_T_51408_2021.nSeisDgnCategory = m_wndSDC;
	m_pParamData->CodeParam.GB_T_51408_2021.nSeisFortification = m_wndSFI;
	m_pParamData->CodeParam.GB_T_51408_2021.nSiteClass = m_wndSC;
	m_pParamData->CodeParam.GB_T_51408_2021.nEarthResponse = m_wndER;
	m_pParamData->CodeParam.GB_T_51408_2021.dTg = m_wndTg.GetEditValue();
	m_pParamData->CodeParam.GB_T_51408_2021.dDamping = m_dXi;
	m_pParamData->CodeParam.GB_T_51408_2021.nLForce = m_nLForce;
	m_pParamData->CodeParam.GB_T_51408_2021.dMaxEQE = m_dMaxEQE;
	m_pParamData->CodeParam.GB_T_51408_2021.dMaxPeriod = m_dMaxPeriod;

	return TRUE;
}


void CETC_GDS_CH_GB_T_51408_2021::Data2Dlg()
{
	if (m_pParamData->nSpecCodeType != D_SPFC_CODE_CH_GB_T_51408_2021)
	{
		m_pParamData->CodeParam.Initialize(D_SPFC_CODE_CH_GB_T_51408_2021);
	}

	// Input Data
	m_wndSDC = m_pParamData->CodeParam.GB_T_51408_2021.nSeisDgnCategory;
	m_wndSFI = m_pParamData->CodeParam.GB_T_51408_2021.nSeisFortification;
	m_wndSC = m_pParamData->CodeParam.GB_T_51408_2021.nSiteClass;
	m_wndER = m_pParamData->CodeParam.GB_T_51408_2021.nEarthResponse;
	m_wndTg.SetEditUnit(m_pParamData->CodeParam.GB_T_51408_2021.dTg);
	m_dXi = m_pParamData->CodeParam.GB_T_51408_2021.dDamping;
	m_nLForce = m_pParamData->CodeParam.GB_T_51408_2021.nLForce;
	m_dMaxEQE = m_pParamData->CodeParam.GB_T_51408_2021.dMaxEQE;
	m_dMaxPeriod = m_pParamData->CodeParam.GB_T_51408_2021.dMaxPeriod;

	UpdateData(FALSE); // CETC_DESIGN_SPECT俊辑 Data2Dlg()龋免 饶 龋免窍妨促 咯扁辑 沁促.
	Dlg2UtilData();
}


void CETC_GDS_CH_GB_T_51408_2021::Initialize()
{
	m_pParamData->CodeParam.Initialize(D_SPFC_CODE_CH_GB_T_51408_2021);
}


void CETC_GDS_CH_GB_T_51408_2021::ShowDlgData()
{
	if (m_pParamData->nSpecCodeType == D_SPFC_CODE_CH_GB_T_51408_2021)
	{
		m_CodeD.GB_T_51408_2021 = m_pParamData->CodeParam.GB_T_51408_2021;
	}
	else
	{
		m_CodeD.GB_T_51408_2021.Initialize();
	}

	// Input Data
	m_wndSDC = m_CodeD.GB_T_51408_2021.nSeisDgnCategory;
	m_wndSFI = m_CodeD.GB_T_51408_2021.nSeisFortification;
	m_wndSC = m_CodeD.GB_T_51408_2021.nSiteClass;
	m_wndER = m_CodeD.GB_T_51408_2021.nEarthResponse;
	m_wndTg.SetEditUnit(m_CodeD.GB_T_51408_2021.dTg);
	m_dXi = m_CodeD.GB_T_51408_2021.dDamping;
	m_nLForce = m_CodeD.GB_T_51408_2021.nLForce;
	m_dMaxEQE = m_CodeD.GB_T_51408_2021.dMaxEQE;
	m_dMaxPeriod = m_CodeD.GB_T_51408_2021.dMaxPeriod;

	UpdateData(FALSE);
	Dlg2UtilData();
}

void CETC_GDS_CH_GB_T_51408_2021::UpdateDampingWnd(CWnd* pDampingWnd)
{
	CString csDamping;
	csDamping.Format(_T("%g"), m_dXi);
	pDampingWnd->SetWindowText(csDamping);
}

void CETC_GDS_CH_GB_T_51408_2021::End()
{
	UpdateData(TRUE);

	Dlg2UtilData();
	SetValue();

	CMyChildDialog::End();
}

void CETC_GDS_CH_GB_T_51408_2021::SetValue()
{
	m_pSpfcUtil->SetParamFromCalc();
}

void CETC_GDS_CH_GB_T_51408_2021::OnCalculateTg()
{
	UpdateData(TRUE);
	T_SPFC_CH_GB_T_51408_2021 Spfc;
	double dTg = Spfc.GetTg(m_wndSDC, m_wndSC, m_wndER);

	m_wndTg.SetEditUnit(dTg);
}

BOOL CETC_GDS_CH_GB_T_51408_2021::ISVALID()
{
	if (!IsValid_d(&m_wndTg, 0.1, 1.2, 0.25)) return FALSE;
	if (m_dMaxPeriod <= 0) { AfxMessageBox(_LS(IDS_CMD_SPECTRUM__MAX_PERIOD_GREATER_0)); return FALSE; }
	if (m_dXi < 0) { AfxMessageBox(_LS(IDS_CMD_SPECTRUM__DAMPING_VALUE)); return FALSE; }
	return TRUE;
}

void CETC_GDS_CH_GB_T_51408_2021::OnSFIClicked()
{
	OnCalculateTg();
	UpdateMaxEQE();
}

void CETC_GDS_CH_GB_T_51408_2021::OnERClicked()
{
	OnCalculateTg();
	UpdateMaxEQE();
}

void CETC_GDS_CH_GB_T_51408_2021::UpdateMaxEQE()
{
	UpdateData(TRUE);

	double dAlpha0[6] = { 0.12, 0.23, 0.34, 0.45, 0.68, 0.90 };
	double dAlpha1[6] = { 0.28, 0.50, 0.72, 0.90, 1.20, 1.40 };
	double dAlpha2[6] = { 0.36, 0.72, 1.00, 1.35, 2.00, 2.43 };
	switch (m_wndER)
	{
	case 0: m_dMaxEQE = dAlpha0[m_wndSFI];    break;
	case 1: m_dMaxEQE = dAlpha1[m_wndSFI];    break;
	case 2: m_dMaxEQE = dAlpha2[m_wndSFI];    break;
	default: break;
	}
	UpdateData(FALSE);
}

void CETC_GDS_CH_GB_T_51408_2021::SetPushOverData(BOOL bInit/*=TRUE*/)
{
	if (m_pParamData->nSpecCodeType != D_SPFC_CODE_CH_GB_T_51408_2021)
	{
		//m_pParamData->nSpecCodeType=D_SPFC_CODE_CH_GB_T_51408_2021; 咯扁辑 捞吧 窍搁 救等促.    
		m_pParamData->CodeParam.Initialize(D_SPFC_CODE_CH_GB_T_51408_2021);
	}
	m_wndSDC = m_pParamData->CodeParam.GB_T_51408_2021.nSeisDgnCategory;
	m_wndSFI = m_pParamData->CodeParam.GB_T_51408_2021.nSeisFortification;
	m_wndSC = m_pParamData->CodeParam.GB_T_51408_2021.nSiteClass;
	m_wndER = m_pParamData->CodeParam.GB_T_51408_2021.nEarthResponse;
	m_wndTg.SetEditUnit(m_pParamData->CodeParam.GB_T_51408_2021.dTg);
	m_dXi = m_pParamData->CodeParam.GB_T_51408_2021.dDamping;
	m_nLForce = m_pParamData->CodeParam.GB_T_51408_2021.nLForce;
	m_dMaxEQE = m_pParamData->CodeParam.GB_T_51408_2021.dMaxEQE;

	const double dGamma = 0.9 + (0.05 - m_dXi) / (0.3 + 6.0 * m_dXi); // 邦急窍碍康开狼 皑尖拌荐.
	//m_dEta1 = 0.02 + (0.05 - m_dXi) / (4.0 + 32.0* m_dXi); // 流急窍碍康开狼 版荤炼沥拌荐.
	const double dEta = 1.0 + (0.05 - m_dXi) / (0.08 + 1.6 * m_dXi); // Damping Ratio 炼沥拌荐.

	m_pSpfcUtil->SetGamma(dGamma);
	m_pSpfcUtil->SetEta(dEta);

	Dlg2UtilData();

	if (bInit)
	{
		UpdateData(FALSE); // CETC_DESIGN_SPECT俊辑 Data2Dlg()龋免 饶 龋免窍妨促 咯扁辑 沁促.
		SetValue();
	}
}

void CETC_GDS_CH_GB_T_51408_2021::Dlg2UtilData()
{
	m_pSpfcUtil->SetSDC(m_wndSDC);
	m_pSpfcUtil->SetSFI(m_wndSFI);
	m_pSpfcUtil->SetSC(m_wndSC);
	m_pSpfcUtil->SetER(m_wndER);

	m_pSpfcUtil->SetTg(m_wndTg.GetEditValue());
	m_pSpfcUtil->SetXi(m_dXi);
	m_pSpfcUtil->SetLForce(m_nLForce);
	m_pSpfcUtil->SetMaxEQE(m_dMaxEQE);
	m_pSpfcUtil->SetMaxPeriod(m_dMaxPeriod);
}
