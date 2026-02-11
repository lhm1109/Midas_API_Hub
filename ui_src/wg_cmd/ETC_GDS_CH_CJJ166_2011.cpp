// ETC_GDS_CH_CJJ166_2011.cpp : implementation file
//
/////////////////////////////////////////////////////////////////////////////
//CJJ166 2011   add by maxiao(maxiao@midasit.com)  2012-2-28
/////////////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "wg_cmd.h"
#include "ETC_GDS_CH_CJJ166_2011.h"

#include "SpfcCHCJJ2011Util.h"
#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_base\wg_base_StrParser.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// ETC_GDS_CH_CJJ166_2011 dialog

CETC_GDS_CH_CJJ166_2011::CETC_GDS_CH_CJJ166_2011(T_SPFC_D* pParamData, CWnd* pParent /*=NULL*/)
	: CMyChildDialog(CETC_GDS_CH_CJJ166_2011::IDD, pParent, std::make_shared<CSpfcCHCJJ2011Util>())
{
	//{{AFX_DATA_INIT(CETC_GDS_CH_CJJ166_2011)
	//}}AFX_DATA_INIT
	m_aSeisType.SetSize(3);
	m_aSeisType[0] = IDC_ETC_GDS_CH_CJJ166_2011_SEIS1_RDO;
	m_aSeisType[1] = IDC_ETC_GDS_CH_CJJ166_2011_SEIS2_RDO;
	m_aSeisType[2] = IDC_ETC_GDS_CH_CJJ166_2011_SEIS3_RDO;

	m_aZoningMap.SetSize(3);
	m_aZoningMap[0] = IDC_ETC_GDS_CH_CJJ166_2011_035_RDO;
	m_aZoningMap[1] = IDC_ETC_GDS_CH_CJJ166_2011_040_RDO;
	m_aZoningMap[2] = IDC_ETC_GDS_CH_CJJ166_2011_045_RDO;

	m_aSiteTpye.SetSize(4);
	m_aSiteTpye[0] = IDC_ETC_GDS_CH_CJJ166_2011_ST1_RDO;
	m_aSiteTpye[1] = IDC_ETC_GDS_CH_CJJ166_2011_ST2_RDO;
	m_aSiteTpye[2] = IDC_ETC_GDS_CH_CJJ166_2011_ST3_RDO;
	m_aSiteTpye[3] = IDC_ETC_GDS_CH_CJJ166_2011_ST4_RDO;

	m_aSeismicIntensity.SetSize(6);
	m_aSeismicIntensity[0] = IDC_ETC_GDS_CH_CJJ166_2011_SI05_RDO;
	m_aSeismicIntensity[1] = IDC_ETC_GDS_CH_CJJ166_2011_SI10_RDO;
	m_aSeismicIntensity[2] = IDC_ETC_GDS_CH_CJJ166_2011_SI15_RDO;
	m_aSeismicIntensity[3] = IDC_ETC_GDS_CH_CJJ166_2011_SI20_RDO;
	m_aSeismicIntensity[4] = IDC_ETC_GDS_CH_CJJ166_2011_SI30_RDO;
	m_aSeismicIntensity[5] = IDC_ETC_GDS_CH_CJJ166_2011_SI40_RDO;

	m_aSeismicE.SetSize(2);
	m_aSeismicE[0] = IDC_ETC_GDS_CH_CJJ166_2011_SCE1_RDO;
	m_aSeismicE[1] = IDC_ETC_GDS_CH_CJJ166_2011_SCE2_RDO;

	m_pParamData = pParamData;
	m_pSpfcUtil = std::dynamic_pointer_cast<CSpfcCHCJJ2011Util>(m_pSpfcBaseUtil);
}

void CETC_GDS_CH_CJJ166_2011::DoDataExchange(CDataExchange* pDX)
{
	CMyChildDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CETC_GDS_CH_CJJ166_2011)
	DDX_Control(pDX, IDC_ETC_GDS_CH_CJJ166_2011_DAMPING_EDIT,       m_edtDampingRatio);
	DDX_Control(pDX, IDC_ETC_GDS_CH_CJJ166_2011_TG_EDIT,            m_edtTg);
	DDX_Control(pDX, IDC_ETC_GDS_CH_CJJ166_2011_ETA1_EDIT,          m_edtEta1);
	DDX_Control(pDX, IDC_ETC_GDS_CH_CJJ166_2011_ETA2_EDIT,          m_edtEta2);
	DDX_Control(pDX, IDC_ETC_GDS_CH_CJJ166_2011_GAMMA_EDIT,         m_edtGamma);
	DDX_Control(pDX, IDC_ETC_GDS_CH_CJJ166_2011_CI_EDIT,            m_edtCi);
	DDX_Control(pDX, IDC_ETC_GDS_CH_CJJ166_2011_A_EDIT,             m_edtA);
	DDX_Control(pDX, IDC_ETC_GDS_CH_CJJ166_2011_SMAX_EDIT,          m_edtSMax);
	DDX_Control(pDX, IDC_CMD_PERIOD_EDIT                 ,          m_edtMaxPeriod);

	DDX_Control(pDX, IDC_ETC_GDS_CH_CJJ166_2011_VERT_ACC_SPEC_CHK,  m_chkVertAccSpec);
	DDX_Control(pDX, IDC_ETC_GDS_CH_CJJ166_2011_E2_005_CHK,         m_chkE2_005);
	DDX_Control(pDX, IDC_ETC_GDS_CH_CJJ166_2011_BMP, m_ImgViewer);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CETC_GDS_CH_CJJ166_2011, CMyChildDialog)
	//{{AFX_MSG_MAP(CETC_GDS_CH_CJJ166_2011)
	
	ON_BN_CLICKED(IDC_ETC_GDS_CH_CJJ166_2011_SEIS1_RDO, OnCalcOutputData)
 	ON_BN_CLICKED(IDC_ETC_GDS_CH_CJJ166_2011_SEIS2_RDO, OnCalcOutputData)
	ON_BN_CLICKED(IDC_ETC_GDS_CH_CJJ166_2011_SEIS3_RDO, OnCalcOutputData)

	ON_BN_CLICKED(IDC_ETC_GDS_CH_CJJ166_2011_035_RDO, OnCalcOutputData)
 	ON_BN_CLICKED(IDC_ETC_GDS_CH_CJJ166_2011_040_RDO, OnCalcOutputData)
	ON_BN_CLICKED(IDC_ETC_GDS_CH_CJJ166_2011_045_RDO, OnCalcOutputData)

	ON_BN_CLICKED(IDC_ETC_GDS_CH_CJJ166_2011_ST1_RDO, OnCalcOutputData)
 	ON_BN_CLICKED(IDC_ETC_GDS_CH_CJJ166_2011_ST2_RDO, OnCalcOutputData)
	ON_BN_CLICKED(IDC_ETC_GDS_CH_CJJ166_2011_ST3_RDO, OnCalcOutputData)
	ON_BN_CLICKED(IDC_ETC_GDS_CH_CJJ166_2011_ST4_RDO, OnCalcOutputData)

	ON_BN_CLICKED(IDC_ETC_GDS_CH_CJJ166_2011_SI05_RDO, OnCalcOutputData)
 	ON_BN_CLICKED(IDC_ETC_GDS_CH_CJJ166_2011_SI10_RDO, OnCalcOutputData)
	ON_BN_CLICKED(IDC_ETC_GDS_CH_CJJ166_2011_SI15_RDO, OnCalcOutputData)
	ON_BN_CLICKED(IDC_ETC_GDS_CH_CJJ166_2011_SI20_RDO, OnCalcOutputData)
	ON_BN_CLICKED(IDC_ETC_GDS_CH_CJJ166_2011_SI30_RDO, OnCalcOutputData)
	ON_BN_CLICKED(IDC_ETC_GDS_CH_CJJ166_2011_SI40_RDO, OnCalcOutputData)

	ON_BN_CLICKED(IDC_ETC_GDS_CH_CJJ166_2011_SCE1_RDO, OnCalcOutputData)
	ON_BN_CLICKED(IDC_ETC_GDS_CH_CJJ166_2011_SCE2_RDO, OnCalcOutputData)

	ON_EN_CHANGE(IDC_ETC_GDS_CH_CJJ166_2011_DAMPING_EDIT, OnChaneInputData)

	ON_BN_CLICKED(IDC_ETC_GDS_CH_CJJ166_2011_VERT_ACC_SPEC_CHK, OnClickVertAccSpec)
	ON_BN_CLICKED(IDC_ETC_GDS_CH_CJJ166_2011_E2_005_CHK       , OnClickE2_005     )


	ON_EN_CHANGE(IDC_ETC_GDS_CH_CJJ166_2011_TG_EDIT,     OnChaneOutputData)
	ON_EN_CHANGE(IDC_ETC_GDS_CH_CJJ166_2011_ETA1_EDIT,   OnChaneOutputData)
	ON_EN_CHANGE(IDC_ETC_GDS_CH_CJJ166_2011_ETA2_EDIT,   OnChaneOutputData)
	ON_EN_CHANGE(IDC_ETC_GDS_CH_CJJ166_2011_GAMMA_EDIT,  OnChaneOutputData)
	ON_EN_CHANGE(IDC_ETC_GDS_CH_CJJ166_2011_CI_EDIT,     OnChaneOutputData)
	ON_EN_CHANGE(IDC_ETC_GDS_CH_CJJ166_2011_A_EDIT,      OnChaneOutputData)
	ON_EN_CHANGE(IDC_ETC_GDS_CH_CJJ166_2011_SMAX_EDIT,   OnChaneOutputData)

	ON_EN_CHANGE(IDC_CMD_PERIOD_EDIT                 ,   OnChaneOutputData)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CETC_GDS_CH_CJJ166_2011 message handlers

BOOL CETC_GDS_CH_CJJ166_2011::OnInitDialog() 
{
	CMyChildDialog::OnInitDialog();

	CString strImgPath = _T("SVG\\illustration\\Dialog\\etc_gds_ch_cjj166_2011.svg");

	m_ImgViewer.SetImage(strImgPath);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CETC_GDS_CH_CJJ166_2011::Dlg2Data()
{
	// UpdateData(TRUE); // CETC_DESIGN_SPECT에서 Dlg2Data()호출 전에 이미 호출되었다.
	m_pParamData->CodeParam.CJJ166_2011.Initialize();

	m_pParamData->nSpecCodeType = D_SPFC_CODE_CJJ166_2011;

	// Input Data
	m_pParamData->CodeParam.CJJ166_2011.nSeisType          = m_pSpfcUtil->GetSeisType();
	m_pParamData->CodeParam.CJJ166_2011.nZoningMap         = m_pSpfcUtil->GetZoningMap();
	m_pParamData->CodeParam.CJJ166_2011.nSiteClass         = m_pSpfcUtil->GetSiteType();
	m_pParamData->CodeParam.CJJ166_2011.nSeisFortification = m_pSpfcUtil->GetSeismicIntensity();
	m_pParamData->CodeParam.CJJ166_2011.nSeisE             = m_pSpfcUtil->GetSeismicE();
	
	m_pParamData->CodeParam.CJJ166_2011.dDamping = m_edtDampingRatio.GetEditValue();
	
	m_pParamData->CodeParam.CJJ166_2011.bVertAccSpec = m_pSpfcUtil->GetVertAccSpec();
	m_pParamData->CodeParam.CJJ166_2011.bE2_005      = m_pSpfcUtil->GetE2_005();

	//Output Data
	m_pParamData->CodeParam.CJJ166_2011.dTg        = m_edtTg.GetEditValue();
	m_pParamData->CodeParam.CJJ166_2011.dEta1      = m_edtEta1.GetEditValue();
	m_pParamData->CodeParam.CJJ166_2011.dEta2      = m_edtEta2.GetEditValue();
	m_pParamData->CodeParam.CJJ166_2011.dGamma     = m_edtGamma.GetEditValue();
	m_pParamData->CodeParam.CJJ166_2011.dCi        = m_edtCi.GetEditValue();

	m_pParamData->CodeParam.CJJ166_2011.dA         = m_edtA.GetEditValue();
	m_pParamData->CodeParam.CJJ166_2011.dSMax      = m_edtSMax.GetEditValue();
	m_pParamData->CodeParam.CJJ166_2011.dMaxPeriod = m_edtMaxPeriod.GetEditValue();

	return TRUE;
}

void CETC_GDS_CH_CJJ166_2011::Data2Dlg()
{  
	if(m_pParamData->nSpecCodeType!=D_SPFC_CODE_CJJ166_2011)
	{
		m_pParamData->CodeParam.Initialize(D_SPFC_CODE_CJJ166_2011);
	}

	// Input Data
	m_pSpfcUtil->SetSeisType(m_pParamData->CodeParam.CJJ166_2011.nSeisType);
	m_pSpfcUtil->SetZoningMap(m_pParamData->CodeParam.CJJ166_2011.nZoningMap);
	m_pSpfcUtil->SetSiteType(m_pParamData->CodeParam.CJJ166_2011.nSiteClass);
	m_pSpfcUtil->SetSeismicIntensity(m_pParamData->CodeParam.CJJ166_2011.nSeisFortification);
	m_pSpfcUtil->SetSeismicE(m_pParamData->CodeParam.CJJ166_2011.nSeisE);

	UtilData2Button();

	m_edtDampingRatio.SetEditUnit(m_pParamData->CodeParam.CJJ166_2011.dDamping, 2);
	
	m_chkVertAccSpec.SetCheck(m_pParamData->CodeParam.CJJ166_2011.bVertAccSpec);
	m_chkE2_005.SetCheck(m_pParamData->CodeParam.CJJ166_2011.bE2_005);
	
	m_pSpfcUtil->SetVertAccSpec(m_pParamData->CodeParam.CJJ166_2011.bVertAccSpec);
	m_pSpfcUtil->SetE2_005(m_pParamData->CodeParam.CJJ166_2011.bE2_005);
	
	//Output Data
	m_edtTg.SetEditUnit(m_pParamData->CodeParam.CJJ166_2011.dTg, 2);
	m_edtEta1.SetEditUnit(m_pParamData->CodeParam.CJJ166_2011.dEta1, 2);
	m_edtEta2.SetEditUnit(m_pParamData->CodeParam.CJJ166_2011.dEta2, 2);
	m_edtGamma.SetEditUnit(m_pParamData->CodeParam.CJJ166_2011.dGamma, 2);
	m_edtCi.SetEditUnit(m_pParamData->CodeParam.CJJ166_2011.dCi, 2);
	m_edtA.SetEditUnit(m_pParamData->CodeParam.CJJ166_2011.dA, 2);
	m_edtSMax.SetEditUnit(m_pParamData->CodeParam.CJJ166_2011.dSMax, 2);
	m_edtMaxPeriod.SetEditUnit(m_pParamData->CodeParam.CJJ166_2011.dMaxPeriod, 2);

	Dlg2UtilData();
	UpdateData(FALSE);

	OnCalcOutputData();
	OnClickVertAccSpec();
	OnClickE2_005();
	OnChaneInputData();
	OnChaneOutputData();
}

void CETC_GDS_CH_CJJ166_2011::Initialize() 
{
	m_pParamData->CodeParam.Initialize(D_SPFC_CODE_CJJ166_2011);
}

void CETC_GDS_CH_CJJ166_2011::ShowDlgData() 
{
	if(m_pParamData->nSpecCodeType == D_SPFC_CODE_CJJ166_2011)
	{
		m_CodeD.CJJ166_2011 = m_pParamData->CodeParam.CJJ166_2011;
	}
	else 
	{
		m_pParamData->CodeParam.Initialize(D_SPFC_CODE_CJJ166_2011);
	}

	// Input Data
	m_pSpfcUtil->SetSeisType(m_pParamData->CodeParam.CJJ166_2011.nSeisType);
	m_pSpfcUtil->SetZoningMap(m_pParamData->CodeParam.CJJ166_2011.nZoningMap);
	m_pSpfcUtil->SetSiteType(m_pParamData->CodeParam.CJJ166_2011.nSiteClass);
	m_pSpfcUtil->SetSeismicIntensity(m_pParamData->CodeParam.CJJ166_2011.nSeisFortification);
	m_pSpfcUtil->SetSeismicE(m_pParamData->CodeParam.CJJ166_2011.nSeisE);

	UtilData2Button();

	m_edtDampingRatio.SetEditUnit(m_pParamData->CodeParam.CJJ166_2011.dDamping, 2);
	
	m_chkVertAccSpec.SetCheck(m_pParamData->CodeParam.CJJ166_2011.bVertAccSpec);
	m_chkE2_005.SetCheck(m_pParamData->CodeParam.CJJ166_2011.bE2_005);
	
	m_pSpfcUtil->SetVertAccSpec(m_pParamData->CodeParam.CJJ166_2011.bVertAccSpec);
	m_pSpfcUtil->SetE2_005(m_pParamData->CodeParam.CJJ166_2011.bE2_005);
	
	//Output Data
	m_edtTg.SetEditUnit(m_pParamData->CodeParam.CJJ166_2011.dTg, 2);
	m_edtEta1.SetEditUnit(m_pParamData->CodeParam.CJJ166_2011.dEta1, 2);
	m_edtEta2.SetEditUnit(m_pParamData->CodeParam.CJJ166_2011.dEta2, 2);
	m_edtGamma.SetEditUnit(m_pParamData->CodeParam.CJJ166_2011.dGamma, 2);
	m_edtCi.SetEditUnit(m_pParamData->CodeParam.CJJ166_2011.dCi, 2);
	m_edtA.SetEditUnit(m_pParamData->CodeParam.CJJ166_2011.dA, 2);
	m_edtSMax.SetEditUnit(m_pParamData->CodeParam.CJJ166_2011.dSMax, 2);
	m_edtMaxPeriod.SetEditUnit(m_pParamData->CodeParam.CJJ166_2011.dMaxPeriod, 2);

	Dlg2UtilData();

	UpdateData(FALSE);

	OnCalcOutputData();
	OnClickVertAccSpec();
	OnClickE2_005();
	OnChaneInputData();
	OnChaneOutputData();
}

void CETC_GDS_CH_CJJ166_2011::Dlg2UtilData()
{
	m_pSpfcUtil->SetDampingRatio(m_edtDampingRatio.GetEditValue());
	m_pSpfcUtil->SetVertAccSpec(m_chkVertAccSpec.GetCheck());
	m_pSpfcUtil->SetE2_005(m_chkE2_005.GetCheck());

	m_pSpfcUtil->SetTg(m_edtTg.GetEditValue());
	m_pSpfcUtil->SetEta1(m_edtEta1.GetEditValue());
	m_pSpfcUtil->SetEta2(m_edtEta2.GetEditValue());
	m_pSpfcUtil->SetGamma(m_edtGamma.GetEditValue());
	m_pSpfcUtil->SetCi(m_edtCi.GetEditValue());
	m_pSpfcUtil->SetA(m_edtA.GetEditValue());
	m_pSpfcUtil->SetSMax(m_edtSMax.GetEditValue());
	m_pSpfcUtil->SetMaxPeriod(m_edtMaxPeriod.GetEditValue());
}

void CETC_GDS_CH_CJJ166_2011::Button2UtilData()
{
	int nSeisType = 0.0;
	int nZoningMap = 0.0;
	int nSiteType = 0.0;
	int nSeismicIntensity = 0.0;
	int nSeismicE = 0.0;

	CDlgUtil::CtrlRadioGetCheck(this, m_aSeisType, nSeisType);
	CDlgUtil::CtrlRadioGetCheck(this, m_aZoningMap, nZoningMap);
	CDlgUtil::CtrlRadioGetCheck(this, m_aSiteTpye, nSiteType);
	CDlgUtil::CtrlRadioGetCheck(this, m_aSeismicIntensity, nSeismicIntensity);
	CDlgUtil::CtrlRadioGetCheck(this, m_aSeismicE, nSeismicE);

	m_pSpfcUtil->SetSeisType(nSeisType);
	m_pSpfcUtil->SetZoningMap(nZoningMap);
	m_pSpfcUtil->SetSiteType(nSiteType);
	m_pSpfcUtil->SetSeismicIntensity(nSeismicIntensity);
	m_pSpfcUtil->SetSeismicE(nSeismicE);
}

void CETC_GDS_CH_CJJ166_2011::UtilData2Button()
{
	int nSeisType = m_pSpfcUtil->GetSeisType();
	int nZoningMap = m_pSpfcUtil->GetZoningMap();
	int nSiteType = m_pSpfcUtil->GetSiteType();
	int nSeismicIntensity = m_pSpfcUtil->GetSeismicIntensity();
	int nSeismicE = m_pSpfcUtil->GetSeismicE();

	CDlgUtil::CtrlRadioSetCheck(this, m_aSeisType, nSeisType);
	CDlgUtil::CtrlRadioSetCheck(this, m_aZoningMap, nZoningMap);
	CDlgUtil::CtrlRadioSetCheck(this, m_aSiteTpye, nSiteType);
	CDlgUtil::CtrlRadioSetCheck(this, m_aSeismicIntensity, nSeismicIntensity);
	CDlgUtil::CtrlRadioSetCheck(this, m_aSeismicE, nSeismicE);
}

void CETC_GDS_CH_CJJ166_2011::End()
{
	UpdateData(TRUE);  
	
	Dlg2UtilData();
	
	CMyChildDialog::End();
}

void CETC_GDS_CH_CJJ166_2011::OnCalcOutputData() 
{
	UpdateData(TRUE);

	Button2UtilData();

	//Tg
	const double dTg = m_pSpfcUtil->CalcTgFromInputData();
	m_edtTg.SetEditUnit(dTg, 2);

	//Eta1
	const double dEta1 = m_pSpfcUtil->CalcEta1FromInputData();
	m_edtEta1.SetEditUnit(dEta1, 2);

	//Eta2
	const double dEta2 = m_pSpfcUtil->CalcEta2FromInputData();
	m_edtEta2.SetEditUnit(dEta2, 2);
	
	//Gamma
	const double dGamma = m_pSpfcUtil->CalcGammaFromInputData();
	m_edtGamma.SetEditUnit(dGamma, 2);

	//Ci Table3.2.2
	const double dCi = m_pSpfcUtil->CalcCiFromInputData();
	m_edtCi.SetEditUnit(dCi, 2);

	//A
	const double dA = m_pSpfcUtil->CalcAFromOutputData(dCi);
	m_edtA.SetEditUnit(dA, 4);

	//Smax
	const double dSMax = m_pSpfcUtil->CalcSMaxFromOutputData(dA);
	m_edtSMax.SetEditUnit(dSMax, 2);
}

void CETC_GDS_CH_CJJ166_2011::OnClickE2_005()
{
	UpdateData(TRUE);
	m_pSpfcUtil->SetE2_005(m_chkE2_005.GetCheck());
	OnCalcOutputData();
}

void CETC_GDS_CH_CJJ166_2011::OnClickVertAccSpec()
{
	UpdateData(TRUE);
	m_pSpfcUtil->SetVertAccSpec(m_chkVertAccSpec.GetCheck());
	OnCalcOutputData();
}


void CETC_GDS_CH_CJJ166_2011::OnChaneInputData()
{
	UpdateData(TRUE);
	m_pSpfcUtil->SetDampingRatio(m_edtDampingRatio.GetEditValue());
	OnCalcOutputData();
}

void CETC_GDS_CH_CJJ166_2011::OnChaneOutputData()
{
	m_pSpfcUtil->SetEta1(m_edtEta1.GetEditValue());
	m_pSpfcUtil->SetEta2(m_edtEta2.GetEditValue());
	m_pSpfcUtil->SetGamma(m_edtGamma.GetEditValue());
	m_pSpfcUtil->SetCi(m_edtCi.GetEditValue());
	m_pSpfcUtil->SetA(m_edtA.GetEditValue());

	//Smax
	const double dSMax = m_pSpfcUtil->CalcSMaxFromOutputData(m_pSpfcUtil->GetA());
	m_edtSMax.SetEditUnit(dSMax, 2);
}
	
BOOL CETC_GDS_CH_CJJ166_2011::ISVALID()
{
	if(!IsValid_d(&m_edtDampingRatio)) return FALSE;
	if(!IsValid_d(&m_edtTg)) return FALSE;
	if(!IsValid_d(&m_edtEta1)) return FALSE;
	if(!IsValid_d(&m_edtEta2)) return FALSE;
	if(!IsValid_d(&m_edtGamma)) return FALSE;
	if(!IsValid_d(&m_edtCi)) return FALSE;
	if(!IsValid_d(&m_edtA)) return FALSE;
	if(!IsValid_d(&m_edtSMax)) return FALSE;
	if(!IsValid_d(&m_edtMaxPeriod)) return FALSE;

	return TRUE;  
}

BOOL CETC_GDS_CH_CJJ166_2011::IsValid_d(CEdit* pEdt)
{
	MyParser parser;
	CString sValue,msg;
	double dValue;
	double dDefValue;
	double dMin;

	if(pEdt==&m_edtDampingRatio) {dDefValue=0.05;dMin=0.;}

	if(pEdt==&m_edtTg)        {dDefValue=0.25 ;dMin=0.;}
	if(pEdt==&m_edtEta1)      {dDefValue=0.02 ;dMin=0.;}
	if(pEdt==&m_edtEta2)      {dDefValue=1.00 ;dMin=0.55;}
	if(pEdt==&m_edtGamma)     {dDefValue=0.9  ;dMin=0.;}
	if(pEdt==&m_edtCi)        {dDefValue=2.2  ;dMin=0.35;}
	if(pEdt==&m_edtA)         {dDefValue=0.05 ;dMin=0.;}
	if(pEdt==&m_edtSMax)      {dDefValue=0.13 ;dMin=0.;}
	if(pEdt==&m_edtMaxPeriod) {dDefValue=6.00 ;dMin=0.;}

	if(IsWindow(pEdt->m_hWnd))
	{
		pEdt->GetWindowText(sValue);
		sValue.TrimLeft();
		if(parser.ParsingNumberArg(sValue) && sValue != _T(""))
		{
			parser.GetNumberData(dValue);
			if(dValue < dMin )
			{
				((CEditUnit*)pEdt)->SetEditUnit(dDefValue);
				pEdt->SetFocus();
				pEdt->SetSel(0,-1);
				m_pWndError = pEdt;
				msg.Format(_LS(IDS_WG_CMD__ADDD__the_Value_must_be_greater_than__4),dMin);
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
			pEdt->SetSel(0,-1);
			m_pWndError = pEdt;
			MessageBox(_LS(IDS_WG_CMD__ADDD__Invalid_Input));
			return FALSE;
		}
	}

	return TRUE;
}
