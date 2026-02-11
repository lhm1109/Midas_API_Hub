
#include "stdafx.h"
#include "wg_cmd.h"
#include "ETC_GDS_CH_JTG_T_2231_01_2020.h"

#include "SpfcCHJTG2020Util.h"
#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_base\wg_base_StrParser.h"
using namespace mit::lib;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



CETC_GDS_CH_JTG_T_2231_01_2020::CETC_GDS_CH_JTG_T_2231_01_2020(T_SPFC_D* pParamData, CWnd* pParent /*=NULL*/)
	: CMyChildDialog(CETC_GDS_CH_JTG_T_2231_01_2020::IDD, pParent, std::make_shared<CSpfcCHJTG2020Util>())
{
	m_aBridgeType.SetSize(4);
	m_aBridgeType[0] = IDC_ETC_GDS_CH_JTG_T_2231_01_2020_BTA_RDO;
	m_aBridgeType[1] = IDC_ETC_GDS_CH_JTG_T_2231_01_2020_BTB_RDO;
	m_aBridgeType[2] = IDC_ETC_GDS_CH_JTG_T_2231_01_2020_BTC_RDO;
	m_aBridgeType[3] = IDC_ETC_GDS_CH_JTG_T_2231_01_2020_BTD_RDO;

	m_aZoningMap.SetSize(3);
	m_aZoningMap[0] = IDC_ETC_GDS_CH_JTG_T_2231_01_2020_CZ035_RDO;
	m_aZoningMap[1] = IDC_ETC_GDS_CH_JTG_T_2231_01_2020_CZ040_RDO;
	m_aZoningMap[2] = IDC_ETC_GDS_CH_JTG_T_2231_01_2020_CZ045_RDO;

	m_aSiteTpye.SetSize(5);
	m_aSiteTpye[0] = IDC_ETC_GDS_CH_JTG_T_2231_01_2020_ST10_RDO;
	m_aSiteTpye[1] = IDC_ETC_GDS_CH_JTG_T_2231_01_2020_ST11_RDO;
	m_aSiteTpye[2] = IDC_ETC_GDS_CH_JTG_T_2231_01_2020_ST2_RDO;
	m_aSiteTpye[3] = IDC_ETC_GDS_CH_JTG_T_2231_01_2020_ST3_RDO;
	m_aSiteTpye[4] = IDC_ETC_GDS_CH_JTG_T_2231_01_2020_ST4_RDO;

	m_aSeismicIntensity.SetSize(6);
	m_aSeismicIntensity[0] = IDC_ETC_GDS_CH_JTG_T_2231_01_2020_SI05_RDO;
	m_aSeismicIntensity[1] = IDC_ETC_GDS_CH_JTG_T_2231_01_2020_SI10_RDO;
	m_aSeismicIntensity[2] = IDC_ETC_GDS_CH_JTG_T_2231_01_2020_SI15_RDO;
	m_aSeismicIntensity[3] = IDC_ETC_GDS_CH_JTG_T_2231_01_2020_SI20_RDO;
	m_aSeismicIntensity[4] = IDC_ETC_GDS_CH_JTG_T_2231_01_2020_SI30_RDO;
	m_aSeismicIntensity[5] = IDC_ETC_GDS_CH_JTG_T_2231_01_2020_SI40_RDO;

	m_aSeismicCriterion.SetSize(2);
	m_aSeismicCriterion[0] = IDC_ETC_GDS_CH_JTG_T_2231_01_2020_SCE1_RDO;
	m_aSeismicCriterion[1] = IDC_ETC_GDS_CH_JTG_T_2231_01_2020_SCE2_RDO;

	m_pParamData = pParamData;
	m_pSpfcUtil = std::dynamic_pointer_cast<CSpfcCHJTG2020Util>(m_pSpfcBaseUtil);

	m_bVertResSpec = FALSE;
}



void CETC_GDS_CH_JTG_T_2231_01_2020::DoDataExchange(CDataExchange* pDX)
{
	CMyChildDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ETC_GDS_CH_JTG_T_2231_01_2020_DAMPING_EDIT, m_edtDampingRatio);
	DDX_Control(pDX, IDC_ETC_GDS_CH_JTG_T_2231_01_2020_TG_EDIT, m_edtTg);
	DDX_Control(pDX, IDC_ETC_GDS_CH_JTG_T_2231_01_2020_CI_EDIT, m_edtCi);
	DDX_Control(pDX, IDC_ETC_GDS_CH_JTG_T_2231_01_2020_CS_EDIT, m_edtCs);
	DDX_Control(pDX, IDC_ETC_GDS_CH_JTG_T_2231_01_2020_CD_EDIT, m_edtCd);
	DDX_Control(pDX, IDC_ETC_GDS_CH_JTG_T_2231_01_2020_EPA_EDIT, m_edtA);
	DDX_Control(pDX, IDC_ETC_GDS_CH_JTG_T_2231_01_2020_SMAX_EDIT, m_edtSMax);
	DDX_Control(pDX, IDC_ETC_GDS_CH_JTG_T_2231_01_2020_STRUCTURE_MAX_EDIT, m_edtMaxPeriod);

	DDX_Control(pDX, IDC_ETC_GDS_CH_JTG_T_2231_01_2020_LARGEBRIDGE_CHK, m_chkLargeBridge);
	DDX_Control(pDX, IDC_ETC_GDS_CH_JTG_T_2231_01_2020_VERT_RES_SPEC_CHK, m_chkVertResSpec);
	DDX_Check(pDX, IDC_ETC_GDS_CH_JTG_T_2231_01_2020_VERT_RES_SPEC_CHK, m_bVertResSpec);
	DDX_Control(pDX, IDC_ETC_GDS_CH_JTG_T_2231_01_2020_BMP, m_ImgViewer);
}


BEGIN_MESSAGE_MAP(CETC_GDS_CH_JTG_T_2231_01_2020, CMyChildDialog)
	
	ON_BN_CLICKED(IDC_ETC_GDS_CH_JTG_T_2231_01_2020_BTA_RDO, OnCalcOutputData)
	ON_BN_CLICKED(IDC_ETC_GDS_CH_JTG_T_2231_01_2020_BTB_RDO, OnCalcOutputData)
	ON_BN_CLICKED(IDC_ETC_GDS_CH_JTG_T_2231_01_2020_BTC_RDO, OnCalcOutputData)
	ON_BN_CLICKED(IDC_ETC_GDS_CH_JTG_T_2231_01_2020_BTD_RDO, OnCalcOutputData)

	ON_BN_CLICKED(IDC_ETC_GDS_CH_JTG_T_2231_01_2020_CZ035_RDO, OnCalcOutputData)
	ON_BN_CLICKED(IDC_ETC_GDS_CH_JTG_T_2231_01_2020_CZ040_RDO, OnCalcOutputData)
	ON_BN_CLICKED(IDC_ETC_GDS_CH_JTG_T_2231_01_2020_CZ045_RDO, OnCalcOutputData)

	ON_BN_CLICKED(IDC_ETC_GDS_CH_JTG_T_2231_01_2020_ST10_RDO, OnCalcOutputData)
	ON_BN_CLICKED(IDC_ETC_GDS_CH_JTG_T_2231_01_2020_ST11_RDO, OnCalcOutputData)
	ON_BN_CLICKED(IDC_ETC_GDS_CH_JTG_T_2231_01_2020_ST2_RDO, OnCalcOutputData)
	ON_BN_CLICKED(IDC_ETC_GDS_CH_JTG_T_2231_01_2020_ST3_RDO, OnCalcOutputData)
	ON_BN_CLICKED(IDC_ETC_GDS_CH_JTG_T_2231_01_2020_ST4_RDO, OnCalcOutputData)

	ON_BN_CLICKED(IDC_ETC_GDS_CH_JTG_T_2231_01_2020_SI05_RDO, OnCalcOutputData)
	ON_BN_CLICKED(IDC_ETC_GDS_CH_JTG_T_2231_01_2020_SI10_RDO, OnCalcOutputData)
	ON_BN_CLICKED(IDC_ETC_GDS_CH_JTG_T_2231_01_2020_SI15_RDO, OnCalcOutputData)
	ON_BN_CLICKED(IDC_ETC_GDS_CH_JTG_T_2231_01_2020_SI20_RDO, OnCalcOutputData)
	ON_BN_CLICKED(IDC_ETC_GDS_CH_JTG_T_2231_01_2020_SI30_RDO, OnCalcOutputData)
	ON_BN_CLICKED(IDC_ETC_GDS_CH_JTG_T_2231_01_2020_SI40_RDO, OnCalcOutputData)

	ON_BN_CLICKED(IDC_ETC_GDS_CH_JTG_T_2231_01_2020_SCE1_RDO, OnCalcOutputData)
	ON_BN_CLICKED(IDC_ETC_GDS_CH_JTG_T_2231_01_2020_SCE2_RDO, OnCalcOutputData)

	ON_BN_CLICKED(IDC_ETC_GDS_CH_JTG_T_2231_01_2020_LARGEBRIDGE_CHK, OnClickLargeBridge)

	ON_BN_CLICKED(IDC_ETC_GDS_CH_JTG_T_2231_01_2020_VERT_RES_SPEC_CHK, OnClickVertResSpec)

	ON_EN_CHANGE(IDC_ETC_GDS_CH_JTG_T_2231_01_2020_DAMPING_EDIT, OnChaneInputData)

	ON_EN_CHANGE(IDC_ETC_GDS_CH_JTG_T_2231_01_2020_TG_EDIT, OnChaneOutputData)
	ON_EN_CHANGE(IDC_ETC_GDS_CH_JTG_T_2231_01_2020_CI_EDIT, OnChaneOutputData)
	ON_EN_CHANGE(IDC_ETC_GDS_CH_JTG_T_2231_01_2020_CS_EDIT, OnChaneOutputData)
	ON_EN_CHANGE(IDC_ETC_GDS_CH_JTG_T_2231_01_2020_CD_EDIT, OnChaneOutputData)
	ON_EN_CHANGE(IDC_ETC_GDS_CH_JTG_T_2231_01_2020_EPA_EDIT, OnChaneOutputData)
	ON_EN_CHANGE(IDC_ETC_GDS_CH_JTG_T_2231_01_2020_SMAX_EDIT, OnChaneOutputData)

	ON_EN_CHANGE(IDC_ETC_GDS_CH_JTG_T_2231_01_2020_STRUCTURE_MAX_EDIT, OnChaneOutputData)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


// CETC_GDS_CH_JTG_T_2231_01_2020 메시지 처리기

BOOL CETC_GDS_CH_JTG_T_2231_01_2020::OnInitDialog()
{
	CMyChildDialog::OnInitDialog();

	CString strImgPath = _T("SVG\\illustration\\Dialog\\etc_gds_ch_JTG_T_2231_01_2020.svg");

	m_ImgViewer.SetImage(strImgPath);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CETC_GDS_CH_JTG_T_2231_01_2020::Dlg2Data()
{
	// UpdateData(TRUE); // CETC_DESIGN_SPECT에서 Dlg2Data()호출 전에 이미 호출되었다.
	m_pParamData->CodeParam.JTG_T_2231_01_2020.Initialize();

	m_pParamData->nSpecCodeType = D_SPFC_CODE_JTG_T_2231_01_2020;

	// Input Data
	m_pParamData->CodeParam.JTG_T_2231_01_2020.nBridgeType = m_pSpfcUtil->GetBridgeType();
	m_pParamData->CodeParam.JTG_T_2231_01_2020.nZoningMap = m_pSpfcUtil->GetZoningMap();
	m_pParamData->CodeParam.JTG_T_2231_01_2020.nSiteType = m_pSpfcUtil->GetSiteType();
	m_pParamData->CodeParam.JTG_T_2231_01_2020.nSeisminIntensity = m_pSpfcUtil->GetSeismicIntensity();
	m_pParamData->CodeParam.JTG_T_2231_01_2020.nSeisminCriterion = m_pSpfcUtil->GetSeismicCriterion();
							
	m_pParamData->CodeParam.JTG_T_2231_01_2020.dDampingRatio = m_edtDampingRatio.GetEditValue();
							
	m_pParamData->CodeParam.JTG_T_2231_01_2020.bLargeBridge = m_pSpfcUtil->GetLargeBridge();
	m_pParamData->CodeParam.JTG_T_2231_01_2020.bVertResSpec = m_bVertResSpec;
	

	m_pSpfcUtil->SetDampingRatio(m_edtDampingRatio.GetEditValue());

	//Output Data
	m_pParamData->CodeParam.JTG_T_2231_01_2020.dTg = m_edtTg.GetEditValue();
	m_pParamData->CodeParam.JTG_T_2231_01_2020.dCi = m_edtCi.GetEditValue();
	m_pParamData->CodeParam.JTG_T_2231_01_2020.dCs = m_edtCs.GetEditValue();
	m_pParamData->CodeParam.JTG_T_2231_01_2020.dCd = m_edtCd.GetEditValue();
	m_pParamData->CodeParam.JTG_T_2231_01_2020.dA = m_edtA.GetEditValue();
	m_pParamData->CodeParam.JTG_T_2231_01_2020.dSMax = m_edtSMax.GetEditValue();
	m_pParamData->CodeParam.JTG_T_2231_01_2020.dMaxPeriod = m_edtMaxPeriod.GetEditValue();

	m_pSpfcUtil->SetTg(m_edtTg.GetEditValue());
	m_pSpfcUtil->SetCi(m_edtCi.GetEditValue());
	m_pSpfcUtil->SetCs(m_edtCs.GetEditValue());
	m_pSpfcUtil->SetCd(m_edtCd.GetEditValue());
	m_pSpfcUtil->SetA(m_edtA.GetEditValue());
	m_pSpfcUtil->SetSMax(m_edtSMax.GetEditValue());
	m_pSpfcUtil->SetMaxPeriod(m_edtMaxPeriod.GetEditValue());

	return TRUE;
}


void CETC_GDS_CH_JTG_T_2231_01_2020::Data2Dlg()
{
	if (m_pParamData->nSpecCodeType != D_SPFC_CODE_JTG_T_2231_01_2020)
	{
		m_pParamData->CodeParam.Initialize(D_SPFC_CODE_JTG_T_2231_01_2020);
	}

	// Input Data
	m_pSpfcUtil->SetBridgeType(m_pParamData->CodeParam.JTG_T_2231_01_2020.nBridgeType);
	m_pSpfcUtil->SetZoningMap(m_pParamData->CodeParam.JTG_T_2231_01_2020.nZoningMap);
	m_pSpfcUtil->SetSiteType(m_pParamData->CodeParam.JTG_T_2231_01_2020.nSiteType);
	m_pSpfcUtil->SetSeismicIntensity(m_pParamData->CodeParam.JTG_T_2231_01_2020.nSeisminIntensity);
	m_pSpfcUtil->SetSeismicCriterion(m_pParamData->CodeParam.JTG_T_2231_01_2020.nSeisminCriterion);

	UtilData2Button();

	m_edtDampingRatio.SetEditUnit(m_pParamData->CodeParam.JTG_T_2231_01_2020.dDampingRatio, 2);

	m_chkLargeBridge.SetCheck(m_pParamData->CodeParam.JTG_T_2231_01_2020.bLargeBridge);
	m_bVertResSpec = m_pParamData->CodeParam.JTG_T_2231_01_2020.bVertResSpec;
	m_chkVertResSpec.SetCheck(m_bVertResSpec);

	//Output Data
	m_edtTg.SetEditUnit(m_pParamData->CodeParam.JTG_T_2231_01_2020.dTg, 2);
	m_edtCi.SetEditUnit(m_pParamData->CodeParam.JTG_T_2231_01_2020.dCi, 2);
	m_edtCs.SetEditUnit(m_pParamData->CodeParam.JTG_T_2231_01_2020.dCs, 2);
	m_edtCd.SetEditUnit(m_pParamData->CodeParam.JTG_T_2231_01_2020.dCd, 2);
	m_edtA.SetEditUnit(m_pParamData->CodeParam.JTG_T_2231_01_2020.dA, 2);
	m_edtSMax.SetEditUnit(m_pParamData->CodeParam.JTG_T_2231_01_2020.dSMax, 2);
	m_edtMaxPeriod.SetEditUnit(m_pParamData->CodeParam.JTG_T_2231_01_2020.dMaxPeriod, 2);

	Dlg2UtilData();
	UpdateData(FALSE);

	OnCalcOutputData();
	OnClickLargeBridge();
	OnClickVertResSpec();
	OnChaneInputData();
	OnChaneOutputData();
}


void CETC_GDS_CH_JTG_T_2231_01_2020::Initialize()
{
	m_pParamData->CodeParam.Initialize(D_SPFC_CODE_JTG_T_2231_01_2020);
}


void CETC_GDS_CH_JTG_T_2231_01_2020::ShowDlgData()
{
	if (m_pParamData->nSpecCodeType == D_SPFC_CODE_JTG_T_2231_01_2020)
	{
		m_CodeD.JTG_T_2231_01_2020= m_pParamData->CodeParam.JTG_T_2231_01_2020;
	}
	else
	{
		m_pParamData->CodeParam.Initialize(D_SPFC_CODE_JTG_T_2231_01_2020);
	}

	// Input Data
	m_pSpfcUtil->SetBridgeType(m_pParamData->CodeParam.JTG_T_2231_01_2020.nBridgeType);
	m_pSpfcUtil->SetZoningMap(m_pParamData->CodeParam.JTG_T_2231_01_2020.nZoningMap);
	m_pSpfcUtil->SetSiteType(m_pParamData->CodeParam.JTG_T_2231_01_2020.nSiteType);
	m_pSpfcUtil->SetSeismicIntensity(m_pParamData->CodeParam.JTG_T_2231_01_2020.nSeisminIntensity);
	m_pSpfcUtil->SetSeismicCriterion(m_pParamData->CodeParam.JTG_T_2231_01_2020.nSeisminCriterion);

	UtilData2Button();

	m_edtDampingRatio.SetEditUnit(m_pParamData->CodeParam.JTG_T_2231_01_2020.dDampingRatio, 2);

	m_chkLargeBridge.SetCheck(m_pParamData->CodeParam.JTG_T_2231_01_2020.bLargeBridge);
	m_bVertResSpec = m_pParamData->CodeParam.JTG_T_2231_01_2020.bVertResSpec;
	m_chkVertResSpec.SetCheck(m_bVertResSpec);

	//Output Data
	m_edtTg.SetEditUnit(m_pParamData->CodeParam.JTG_T_2231_01_2020.dTg, 2);
	m_edtCi.SetEditUnit(m_pParamData->CodeParam.JTG_T_2231_01_2020.dCi, 2);
	m_edtCs.SetEditUnit(m_pParamData->CodeParam.JTG_T_2231_01_2020.dCs, 2);
	m_edtCd.SetEditUnit(m_pParamData->CodeParam.JTG_T_2231_01_2020.dCd, 2);
	m_edtA.SetEditUnit(m_pParamData->CodeParam.JTG_T_2231_01_2020.dA, 2);
	m_edtSMax.SetEditUnit(m_pParamData->CodeParam.JTG_T_2231_01_2020.dSMax, 2);
	m_edtMaxPeriod.SetEditUnit(m_pParamData->CodeParam.JTG_T_2231_01_2020.dMaxPeriod, 2);

	Dlg2UtilData();
	UpdateData(FALSE);

	OnCalcOutputData();
	OnClickLargeBridge();
	OnClickVertResSpec();
	OnChaneInputData();
	OnChaneOutputData();
}

void CETC_GDS_CH_JTG_T_2231_01_2020::Dlg2UtilData()
{
	m_pSpfcUtil->SetDampingRatio(m_edtDampingRatio.GetEditValue());
	m_pSpfcUtil->SetLargeBridge(m_chkLargeBridge.GetCheck());
	m_pSpfcUtil->SetVertResSpec(m_bVertResSpec);

	m_pSpfcUtil->SetTg(m_edtTg.GetEditValue());
	m_pSpfcUtil->SetCi(m_edtCi.GetEditValue());
	m_pSpfcUtil->SetCs(m_edtCs.GetEditValue());
	m_pSpfcUtil->SetCd(m_edtCd.GetEditValue());
	m_pSpfcUtil->SetA(m_edtA.GetEditValue());
	m_pSpfcUtil->SetSMax(m_edtSMax.GetEditValue());
	m_pSpfcUtil->SetMaxPeriod(m_edtMaxPeriod.GetEditValue());
}

void CETC_GDS_CH_JTG_T_2231_01_2020::Button2UtilData()
{
	int nBridgeType = 0.0;
	int nZoningMap = 0.0;
	int nSiteType = 0.0;
	int nSeismicIntensity = 0.0;
	int nSeismicCriterion = 0.0;

	CDlgUtil::CtrlRadioGetCheck(this, m_aBridgeType, nBridgeType);
	CDlgUtil::CtrlRadioGetCheck(this, m_aZoningMap, nZoningMap);
	CDlgUtil::CtrlRadioGetCheck(this, m_aSiteTpye, nSiteType);
	CDlgUtil::CtrlRadioGetCheck(this, m_aSeismicIntensity, nSeismicIntensity);
	CDlgUtil::CtrlRadioGetCheck(this, m_aSeismicCriterion, nSeismicCriterion);

	m_pSpfcUtil->SetBridgeType(nBridgeType);
	m_pSpfcUtil->SetZoningMap(nZoningMap);
	m_pSpfcUtil->SetSiteType(nSiteType);
	m_pSpfcUtil->SetSeismicIntensity(nSeismicIntensity);
	m_pSpfcUtil->SetSeismicCriterion(nSeismicCriterion);
}

void CETC_GDS_CH_JTG_T_2231_01_2020::UtilData2Button()
{
	int nBridgeType = m_pSpfcUtil->GetBridgeType();
	int nZoningMap = m_pSpfcUtil->GetZoningMap();
	int nSiteType = m_pSpfcUtil->GetSiteType();
	int nSeismicIntensity = m_pSpfcUtil->GetSeismicIntensity();
	int nSeismicCriterion = m_pSpfcUtil->GetSeismicCriterion();

	CDlgUtil::CtrlRadioSetCheck(this, m_aBridgeType, nBridgeType);
	CDlgUtil::CtrlRadioSetCheck(this, m_aZoningMap, nZoningMap);
	CDlgUtil::CtrlRadioSetCheck(this, m_aSiteTpye, nSiteType);
	CDlgUtil::CtrlRadioSetCheck(this, m_aSeismicIntensity, nSeismicIntensity);
	CDlgUtil::CtrlRadioSetCheck(this, m_aSeismicCriterion, nSeismicCriterion);
}

void CETC_GDS_CH_JTG_T_2231_01_2020::End()
{
	UpdateData(TRUE);

	// MakeSpectrumData할 때 맴버 변수 사용됨.
	Dlg2UtilData();

	CMyChildDialog::End();
}

void CETC_GDS_CH_JTG_T_2231_01_2020::OnCalcOutputData()
{
	UpdateData(TRUE);

	Button2UtilData();

	//Tg
	const double dTg = m_pSpfcUtil->CalcTgFromInputData();
	m_edtTg.SetEditUnit(dTg, 2);

	//Ci
	const double dCi = m_pSpfcUtil->CalcCiFromInputData();
	m_edtCi.SetEditUnit(dCi, 2);

	//Cs
	const double dCs = m_pSpfcUtil->CalcCsFromInputData();
	m_edtCs.SetEditUnit(dCs, 2);

	//Cd
	const double dCd = m_pSpfcUtil->CalcCdFromInputData();
	m_edtCd.SetEditUnit(dCd, 2);

	//EPA
	const double dA = m_pSpfcUtil->CalcEPAFromInputData();
	m_edtA.SetEditUnit(dA, 2);

	//Smax
	const double dSMax = m_pSpfcUtil->CalcSMaxFromOutputData(dCi, dCs, dCd, dA);
	m_edtSMax.SetEditUnit(dSMax, 2);
}

void CETC_GDS_CH_JTG_T_2231_01_2020::OnClickLargeBridge()
{
	UpdateData(TRUE);
	m_pSpfcUtil->SetLargeBridge(m_chkLargeBridge.GetCheck());

	OnCalcOutputData();
}

void CETC_GDS_CH_JTG_T_2231_01_2020::OnClickVertResSpec()
{
	UpdateData(TRUE);
	m_bVertResSpec = m_chkVertResSpec.GetCheck();
	OnCalcOutputData();
}


void CETC_GDS_CH_JTG_T_2231_01_2020::OnChaneInputData()
{
	m_pSpfcUtil->SetDampingRatio(m_edtDampingRatio.GetEditValue());

	//Cd
	const double dCd = m_pSpfcUtil->CalcCdFromInputData();
	m_edtCd.SetEditUnit(dCd, 2);

	OnChaneOutputData();
}

void CETC_GDS_CH_JTG_T_2231_01_2020::OnChaneOutputData()
{
	const double dCi = m_edtCi.GetEditValue();
	const double dCs = m_edtCs.GetEditValue();
	const double dCd = m_edtCd.GetEditValue();
	const double dA = m_edtA.GetEditValue();

	//Smax
	const double dSMax = m_pSpfcUtil->CalcSMaxFromOutputData(dCi, dCs, dCd, dA);
	m_edtSMax.SetEditUnit(dSMax, 2);
}

BOOL CETC_GDS_CH_JTG_T_2231_01_2020::ISVALID()
{
	if (!IsValid_d(&m_edtDampingRatio)) return FALSE;

	if (!IsValid_d(&m_edtTg)) return FALSE;
	if (!IsValid_d(&m_edtCi)) return FALSE;
	if (!IsValid_d(&m_edtCs)) return FALSE;
	if (!IsValid_d(&m_edtCd)) return FALSE;
	if (!IsValid_d(&m_edtA)) return FALSE;
	if (!IsValid_d(&m_edtSMax)) return FALSE;
	if (!IsValid_d(&m_edtMaxPeriod)) return FALSE;

	return TRUE;
}

BOOL CETC_GDS_CH_JTG_T_2231_01_2020::IsValid_d(CEdit* pEdt)
{
	MyParser parser;
	CString sValue, msg;
	double dValue;
	double dDefValue;
	double dMin;

	if (pEdt == &m_edtDampingRatio) { dDefValue = 0.05; dMin = 0.; }

	if (pEdt == &m_edtTg) { dDefValue = 0.20; dMin = 0.; }
	if (pEdt == &m_edtCi) { dDefValue = 1.00; dMin = 0.; }
	if (pEdt == &m_edtCs) { dDefValue = 0.72; dMin = 0.; }
	if (pEdt == &m_edtCd) { dDefValue = 1.00; dMin = 0.; }
	if (pEdt == &m_edtA) { dDefValue = 0.05; dMin = 0.; }
	if (pEdt == &m_edtSMax) { dDefValue = 0.135; dMin = 0.; }
	if (pEdt == &m_edtMaxPeriod) { dDefValue = 6.00; dMin = 0.; }

	if (IsWindow(pEdt->m_hWnd))
	{
		pEdt->GetWindowText(sValue);
		sValue.TrimLeft();
		if (parser.ParsingNumberArg(sValue) && sValue != _T(""))
		{
			parser.GetNumberData(dValue);
			if (dValue < dMin)
			{
				((CEditUnit*)pEdt)->SetEditUnit(dDefValue);
				pEdt->SetFocus();
				pEdt->SetSel(0, -1);
				m_pWndError = pEdt;
				msg.Format(_LS(IDS_WG_CMD__ADDD__the_Value_must_be_greater_than__4), dMin);
				MessageBox(msg);
				return FALSE;
			}
			else
				return TRUE;
		}
		else
		{
			((CEditUnit*)pEdt)->SetEditUnit(dDefValue);
			pEdt->SetFocus();
			pEdt->SetSel(0, -1);
			m_pWndError = pEdt;
			MessageBox(_LS(IDS_WG_CMD__ADDD__Invalid_Input));
			return FALSE;
		}
	}
	return TRUE;
}