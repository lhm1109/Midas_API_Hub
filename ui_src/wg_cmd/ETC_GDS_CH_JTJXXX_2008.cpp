// ETC_GDS_CH_JTJXXX_2008.cpp : implementation file
//
/////////////////////////////////////////////////////////////////////////////
//
// JTJXXX는 아직 번호가 나오지 않아서 이렇게 기록할 수 밖에 없았다.
// 나중에 번호가 나오면 전부 수정하자...- MNET:3615-JHYUN-20080818
// 
// 이름이 나왔는데...JTG/T B02-01-2008 요건데...다 고치기 빡심....그냥 보이는 데만..수정..;;
//
/////////////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "wg_cmd.h"
#include "ETC_GDS_CH_JTJXXX_2008.h"

#include "SpfcCHJTJ2008Util.h"
#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_base\wg_base_StrParser.h"
using namespace mit::lib;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// ETC_GDS_CH_JTJXXX_2008 dialog

CETC_GDS_CH_JTJXXX_2008::CETC_GDS_CH_JTJXXX_2008(T_SPFC_D* pParamData, CWnd* pParent /*=NULL*/)
	: CMyChildDialog(CETC_GDS_CH_JTJXXX_2008::IDD, pParent, std::make_shared< CSpfcCHJTJ2008Util>())
{
	//{{AFX_DATA_INIT(CETC_GDS_CH_JTJXXX_2008)
	//}}AFX_DATA_INIT
	m_aBridgeType.SetSize(4);
	m_aBridgeType[0] = IDC_ETC_GDS_CH_JTJXXX_2008_BTA_RDO;
	m_aBridgeType[1] = IDC_ETC_GDS_CH_JTJXXX_2008_BTB_RDO;
	m_aBridgeType[2] = IDC_ETC_GDS_CH_JTJXXX_2008_BTC_RDO;
	m_aBridgeType[3] = IDC_ETC_GDS_CH_JTJXXX_2008_BTD_RDO;

	m_aZoningMap.SetSize(3);
	m_aZoningMap[0] = IDC_ETC_GDS_CH_JTJXXX_2008_CZ035_RDO;
	m_aZoningMap[1] = IDC_ETC_GDS_CH_JTJXXX_2008_CZ040_RDO;
	m_aZoningMap[2] = IDC_ETC_GDS_CH_JTJXXX_2008_CZ045_RDO;

	m_aSiteTpye.SetSize(4);
	m_aSiteTpye[0] = IDC_ETC_GDS_CH_JTJXXX_2008_ST1_RDO;
	m_aSiteTpye[1] = IDC_ETC_GDS_CH_JTJXXX_2008_ST2_RDO;
	m_aSiteTpye[2] = IDC_ETC_GDS_CH_JTJXXX_2008_ST3_RDO;
	m_aSiteTpye[3] = IDC_ETC_GDS_CH_JTJXXX_2008_ST4_RDO;

	m_aSeismicIntensity.SetSize(6);
	m_aSeismicIntensity[0] = IDC_ETC_GDS_CH_JTJXXX_2008_SI05_RDO;
	m_aSeismicIntensity[1] = IDC_ETC_GDS_CH_JTJXXX_2008_SI10_RDO;
	m_aSeismicIntensity[2] = IDC_ETC_GDS_CH_JTJXXX_2008_SI15_RDO;
	m_aSeismicIntensity[3] = IDC_ETC_GDS_CH_JTJXXX_2008_SI20_RDO;
	m_aSeismicIntensity[4] = IDC_ETC_GDS_CH_JTJXXX_2008_SI30_RDO;
	m_aSeismicIntensity[5] = IDC_ETC_GDS_CH_JTJXXX_2008_SI40_RDO;

	m_aSeismicCriterion.SetSize(2);
	m_aSeismicCriterion[0] = IDC_ETC_GDS_CH_JTJXXX_2008_SCE1_RDO;
	m_aSeismicCriterion[1] = IDC_ETC_GDS_CH_JTJXXX_2008_SCE2_RDO;

	m_pParamData = pParamData;
	m_pSpfcUtil = std::dynamic_pointer_cast<CSpfcCHJTJ2008Util>(m_pSpfcBaseUtil);

	m_nVertAccSpecType  = 0;
}

void CETC_GDS_CH_JTJXXX_2008::DoDataExchange(CDataExchange* pDX)
{
	CMyChildDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CETC_GDS_CH_JTJXXX_2008)
	DDX_Control(pDX, IDC_ETC_GDS_CH_JTJXXX_2008_DAMPING_EDIT,       m_edtDampingRatio);
	DDX_Control(pDX, IDC_ETC_GDS_CH_JTJXXX_2008_TG_EDIT,            m_edtTg);
	DDX_Control(pDX, IDC_ETC_GDS_CH_JTJXXX_2008_CI_EDIT,            m_edtCi);
	DDX_Control(pDX, IDC_ETC_GDS_CH_JTJXXX_2008_CS_EDIT,            m_edtCs);
	DDX_Control(pDX, IDC_ETC_GDS_CH_JTJXXX_2008_CD_EDIT,            m_edtCd);
	DDX_Control(pDX, IDC_ETC_GDS_CH_JTJXXX_2008_EPA_EDIT,           m_edtEPA);
	DDX_Control(pDX, IDC_ETC_GDS_CH_JTJXXX_2008_SMAX_EDIT,          m_edtSMax);
	DDX_Control(pDX, IDC_ETC_GDS_CH_JTJXXX_2008_STRUCTURE_MAX_EDIT, m_edtMaxPeriod);

	DDX_Control(pDX, IDC_ETC_GDS_CH_JTJXXX_2008_LARGEBRIDGE_CHK,    m_chkLargeBridge);

	DDX_Check  (pDX, IDC_ETC_GDS_CH_JTJXXX_2008_VERT_ACC_SPEC_CHK,       m_bVertAccSpec);
	DDX_Radio  (pDX, IDC_ETC_GDS_CH_JTJXXX_2008_VERT_ACC_SPEC_TYPE_RDO1, m_nVertAccSpecType);
	DDX_Control(pDX, IDC_ETC_GDS_CH_JTJXXX_2008_BMP, m_ImgViewer);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CETC_GDS_CH_JTJXXX_2008, CMyChildDialog)
	//{{AFX_MSG_MAP(CETC_GDS_CH_JTJXXX_2008)
	
	ON_BN_CLICKED(IDC_ETC_GDS_CH_JTJXXX_2008_BTA_RDO, OnCalcOutputData)
 	ON_BN_CLICKED(IDC_ETC_GDS_CH_JTJXXX_2008_BTB_RDO, OnCalcOutputData)
	ON_BN_CLICKED(IDC_ETC_GDS_CH_JTJXXX_2008_BTC_RDO, OnCalcOutputData)
	ON_BN_CLICKED(IDC_ETC_GDS_CH_JTJXXX_2008_BTD_RDO, OnCalcOutputData)

	ON_BN_CLICKED(IDC_ETC_GDS_CH_JTJXXX_2008_CZ035_RDO, OnCalcOutputData)
 	ON_BN_CLICKED(IDC_ETC_GDS_CH_JTJXXX_2008_CZ040_RDO, OnCalcOutputData)
	ON_BN_CLICKED(IDC_ETC_GDS_CH_JTJXXX_2008_CZ045_RDO, OnCalcOutputData)

	ON_BN_CLICKED(IDC_ETC_GDS_CH_JTJXXX_2008_ST1_RDO, OnCalcOutputData)
 	ON_BN_CLICKED(IDC_ETC_GDS_CH_JTJXXX_2008_ST2_RDO, OnCalcOutputData)
	ON_BN_CLICKED(IDC_ETC_GDS_CH_JTJXXX_2008_ST3_RDO, OnCalcOutputData)
	ON_BN_CLICKED(IDC_ETC_GDS_CH_JTJXXX_2008_ST4_RDO, OnCalcOutputData)

	ON_BN_CLICKED(IDC_ETC_GDS_CH_JTJXXX_2008_SI05_RDO, OnCalcOutputData)
 	ON_BN_CLICKED(IDC_ETC_GDS_CH_JTJXXX_2008_SI10_RDO, OnCalcOutputData)
	ON_BN_CLICKED(IDC_ETC_GDS_CH_JTJXXX_2008_SI15_RDO, OnCalcOutputData)
	ON_BN_CLICKED(IDC_ETC_GDS_CH_JTJXXX_2008_SI20_RDO, OnCalcOutputData)
	ON_BN_CLICKED(IDC_ETC_GDS_CH_JTJXXX_2008_SI30_RDO, OnCalcOutputData)
	ON_BN_CLICKED(IDC_ETC_GDS_CH_JTJXXX_2008_SI40_RDO, OnCalcOutputData)

	ON_BN_CLICKED(IDC_ETC_GDS_CH_JTJXXX_2008_SCE1_RDO, OnCalcOutputData)
	ON_BN_CLICKED(IDC_ETC_GDS_CH_JTJXXX_2008_SCE2_RDO, OnCalcOutputData)

	ON_BN_CLICKED(IDC_ETC_GDS_CH_JTJXXX_2008_LARGEBRIDGE_CHK, OnClickLargeBridge)

	ON_BN_CLICKED(IDC_ETC_GDS_CH_JTJXXX_2008_VERT_ACC_SPEC_CHK,       OnClickVertAccSpec)
	ON_BN_CLICKED(IDC_ETC_GDS_CH_JTJXXX_2008_VERT_ACC_SPEC_TYPE_RDO1, OnClickVertAccSpecType)
	ON_BN_CLICKED(IDC_ETC_GDS_CH_JTJXXX_2008_VERT_ACC_SPEC_TYPE_RDO2, OnClickVertAccSpecType)

	ON_EN_CHANGE(IDC_ETC_GDS_CH_JTJXXX_2008_DAMPING_EDIT, OnChaneInputData)

	ON_EN_CHANGE(IDC_ETC_GDS_CH_JTJXXX_2008_TG_EDIT,   OnChaneOutputData)
	ON_EN_CHANGE(IDC_ETC_GDS_CH_JTJXXX_2008_CI_EDIT,   OnChaneOutputData)
	ON_EN_CHANGE(IDC_ETC_GDS_CH_JTJXXX_2008_CS_EDIT,   OnChaneOutputData)
	ON_EN_CHANGE(IDC_ETC_GDS_CH_JTJXXX_2008_CD_EDIT,   OnChaneOutputData)
	ON_EN_CHANGE(IDC_ETC_GDS_CH_JTJXXX_2008_EPA_EDIT,  OnChaneOutputData)
	ON_EN_CHANGE(IDC_ETC_GDS_CH_JTJXXX_2008_SMAX_EDIT, OnChaneOutputData)

	ON_EN_CHANGE(IDC_ETC_GDS_CH_JTJXXX_2008_STRUCTURE_MAX_EDIT, OnChaneOutputData)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CETC_GDS_CH_JTJXXX_2008 message handlers

BOOL CETC_GDS_CH_JTJXXX_2008::OnInitDialog() 
{
	CMyChildDialog::OnInitDialog();

	CString strImgPath = _T("SVG\\illustration\\Dialog\\etc_gds_ch_JTJXXX2008.svg");

	m_ImgViewer.SetImage(strImgPath);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CETC_GDS_CH_JTJXXX_2008::Dlg2Data()
{
	// UpdateData(TRUE); // CETC_DESIGN_SPECT에서 Dlg2Data()호출 전에 이미 호출되었다.
	m_pParamData->CodeParam.JTJXXX_2008.Initialize();

	m_pParamData->nSpecCodeType = D_SPFC_CODE_JTJXXX_2008;

	// Input Data
	m_pParamData->CodeParam.JTJXXX_2008.nBridgeType       = m_pSpfcUtil->GetBridgeType();
	m_pParamData->CodeParam.JTJXXX_2008.nZoningMap        = m_pSpfcUtil->GetZoningMap();
	m_pParamData->CodeParam.JTJXXX_2008.nSiteType         = m_pSpfcUtil->GetSiteType();
	m_pParamData->CodeParam.JTJXXX_2008.nSeisminIntensity = m_pSpfcUtil->GetSeismicIntensity();
	m_pParamData->CodeParam.JTJXXX_2008.nSeisminCriterion = m_pSpfcUtil->GetSeismicCriterion();

	m_pParamData->CodeParam.JTJXXX_2008.dDampingRatio = m_edtDampingRatio.GetEditValue();
	
	m_pParamData->CodeParam.JTJXXX_2008.bLargeBridge = m_pSpfcUtil->GetLargeBridge();
	m_pParamData->CodeParam.JTJXXX_2008.bVertAccSpec = m_bVertAccSpec;
	m_pParamData->CodeParam.JTJXXX_2008.nVertAccSpecType = m_nVertAccSpecType;

	//Output Data
	m_pParamData->CodeParam.JTJXXX_2008.dTg        = m_edtTg.GetEditValue();
	m_pParamData->CodeParam.JTJXXX_2008.dCi        = m_edtCi.GetEditValue();
	m_pParamData->CodeParam.JTJXXX_2008.dCs        = m_edtCs.GetEditValue();
	m_pParamData->CodeParam.JTJXXX_2008.dCd        = m_edtCd.GetEditValue();
	m_pParamData->CodeParam.JTJXXX_2008.dEPA       = m_edtEPA.GetEditValue();
	m_pParamData->CodeParam.JTJXXX_2008.dSMax      = m_edtSMax.GetEditValue();
	m_pParamData->CodeParam.JTJXXX_2008.dMaxPeriod = m_edtMaxPeriod.GetEditValue();

	return TRUE;
}

void CETC_GDS_CH_JTJXXX_2008::Data2Dlg()
{  
	if(m_pParamData->nSpecCodeType!=D_SPFC_CODE_JTJXXX_2008)
	{
		m_pParamData->CodeParam.Initialize(D_SPFC_CODE_JTJXXX_2008);
	}

	// Input Data
	m_pSpfcUtil->SetBridgeType(m_pParamData->CodeParam.JTJXXX_2008.nBridgeType);
	m_pSpfcUtil->SetZoningMap(m_pParamData->CodeParam.JTJXXX_2008.nZoningMap);
	m_pSpfcUtil->SetSiteType(m_pParamData->CodeParam.JTJXXX_2008.nSiteType);
	m_pSpfcUtil->SetSeismicIntensity(m_pParamData->CodeParam.JTJXXX_2008.nSeisminIntensity);
	m_pSpfcUtil->SetSeismicCriterion(m_pParamData->CodeParam.JTJXXX_2008.nSeisminCriterion);

	UtilData2Button();

	m_edtDampingRatio.SetEditUnit(m_pParamData->CodeParam.JTJXXX_2008.dDampingRatio, 2);
	
	m_chkLargeBridge.SetCheck(m_pParamData->CodeParam.JTJXXX_2008.bLargeBridge);
	m_bVertAccSpec = m_pParamData->CodeParam.JTJXXX_2008.bVertAccSpec;
	m_nVertAccSpecType = m_pParamData->CodeParam.JTJXXX_2008.nVertAccSpecType;

	//Output Data
	m_edtTg       .SetEditUnit(m_pParamData->CodeParam.JTJXXX_2008.dTg, 2);
	m_edtCi       .SetEditUnit(m_pParamData->CodeParam.JTJXXX_2008.dCi, 2);
	m_edtCs       .SetEditUnit(m_pParamData->CodeParam.JTJXXX_2008.dCs, 2);
	m_edtCd       .SetEditUnit(m_pParamData->CodeParam.JTJXXX_2008.dCd, 2);
	m_edtEPA      .SetEditUnit(m_pParamData->CodeParam.JTJXXX_2008.dEPA, 2);
	m_edtSMax     .SetEditUnit(m_pParamData->CodeParam.JTJXXX_2008.dSMax, 2);
	m_edtMaxPeriod.SetEditUnit(m_pParamData->CodeParam.JTJXXX_2008.dMaxPeriod, 2);

	UpdateData(FALSE);

	OnCalcOutputData();
	OnClickLargeBridge();
	OnClickVertAccSpec();
	OnClickVertAccSpecType();
	OnChaneInputData();
	OnChaneOutputData();

	Dlg2UtilData();
}

void CETC_GDS_CH_JTJXXX_2008::Initialize() 
{
	m_pParamData->CodeParam.Initialize(D_SPFC_CODE_JTJXXX_2008);
}

void CETC_GDS_CH_JTJXXX_2008::ShowDlgData() 
{
	if(m_pParamData->nSpecCodeType == D_SPFC_CODE_JTJXXX_2008)
	{
		m_CodeD.JTJXXX_2008 = m_pParamData->CodeParam.JTJXXX_2008;
	}
	else 
	{
		m_pParamData->CodeParam.Initialize(D_SPFC_CODE_JTJXXX_2008);
	}

	// Input Data
	m_pSpfcUtil->SetBridgeType(m_pParamData->CodeParam.JTJXXX_2008.nBridgeType);
	m_pSpfcUtil->SetZoningMap(m_pParamData->CodeParam.JTJXXX_2008.nZoningMap);
	m_pSpfcUtil->SetSiteType(m_pParamData->CodeParam.JTJXXX_2008.nSiteType);
	m_pSpfcUtil->SetSeismicIntensity(m_pParamData->CodeParam.JTJXXX_2008.nSeisminIntensity);
	m_pSpfcUtil->SetSeismicCriterion(m_pParamData->CodeParam.JTJXXX_2008.nSeisminCriterion);
	
	UtilData2Button();

	m_edtDampingRatio.SetEditUnit(m_pParamData->CodeParam.JTJXXX_2008.dDampingRatio, 2);
	
	m_chkLargeBridge.SetCheck(m_pParamData->CodeParam.JTJXXX_2008.bLargeBridge);
	m_bVertAccSpec = m_pParamData->CodeParam.JTJXXX_2008.bVertAccSpec;
	m_nVertAccSpecType = m_pParamData->CodeParam.JTJXXX_2008.nVertAccSpecType;

	m_pSpfcUtil->SetDampingRatio(m_edtDampingRatio.GetEditValue());
	
	//Output Data
	m_edtTg       .SetEditUnit(m_pParamData->CodeParam.JTJXXX_2008.dTg, 2);
	m_edtCi       .SetEditUnit(m_pParamData->CodeParam.JTJXXX_2008.dCi, 2);
	m_edtCs       .SetEditUnit(m_pParamData->CodeParam.JTJXXX_2008.dCs, 2);
	m_edtCd       .SetEditUnit(m_pParamData->CodeParam.JTJXXX_2008.dCd, 2);
	m_edtEPA      .SetEditUnit(m_pParamData->CodeParam.JTJXXX_2008.dEPA, 2);
	m_edtSMax     .SetEditUnit(m_pParamData->CodeParam.JTJXXX_2008.dSMax, 2);
	m_edtMaxPeriod.SetEditUnit(m_pParamData->CodeParam.JTJXXX_2008.dMaxPeriod, 2);

	UpdateData(FALSE);

	OnCalcOutputData();
	OnClickLargeBridge();
	OnClickVertAccSpec();
	OnClickVertAccSpecType();
	OnChaneInputData();
	OnChaneOutputData();

	Dlg2UtilData();
}

void CETC_GDS_CH_JTJXXX_2008::Dlg2UtilData()
{
	m_pSpfcUtil->SetDampingRatio(m_edtDampingRatio.GetEditValue());

	m_pSpfcUtil->SetLargeBridge(m_chkLargeBridge.GetCheck());
	m_pSpfcUtil->SetVertAccSpec(m_bVertAccSpec);
	m_pSpfcUtil->SetVertAccSpecType(m_nVertAccSpecType);

	m_pSpfcUtil->SetTg(m_edtTg.GetEditValue());
	m_pSpfcUtil->SetCi(m_edtCi.GetEditValue());
	m_pSpfcUtil->SetCs(m_edtCs.GetEditValue());
	m_pSpfcUtil->SetCd(m_edtCd.GetEditValue());
	m_pSpfcUtil->SetEPA(m_edtEPA.GetEditValue());
	m_pSpfcUtil->SetSMax(m_edtSMax.GetEditValue());
	m_pSpfcUtil->SetMaxPeriod(m_edtMaxPeriod.GetEditValue());

}

void CETC_GDS_CH_JTJXXX_2008::Button2UtilData()
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

void CETC_GDS_CH_JTJXXX_2008::UtilData2Button()
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


void CETC_GDS_CH_JTJXXX_2008::End()
{
	UpdateData(TRUE);  
	
	// MakeSpectrumData할 때 맴버 변수 사용됨.
	Dlg2UtilData();

	CMyChildDialog::End();
}

void CETC_GDS_CH_JTJXXX_2008::OnCalcOutputData() 
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
	const double dEPA = m_pSpfcUtil->CalcEPAFromInputData();
	m_edtEPA.SetEditUnit(dEPA, 2);

	//Smax
	double dSMax = m_pSpfcUtil->CalcSMaxFromOutputData(dCi, dCs, dCd, dEPA);
	m_edtSMax.SetEditUnit(dSMax, 2);
}

void CETC_GDS_CH_JTJXXX_2008::OnClickLargeBridge()
{
	UpdateData(TRUE);
	m_pSpfcUtil->SetLargeBridge(m_chkLargeBridge.GetCheck());

	OnCalcOutputData();
}

void CETC_GDS_CH_JTJXXX_2008::OnClickVertAccSpec()
{
	UpdateData(TRUE);

	if(m_bVertAccSpec)
	{
		GetDlgItem(IDC_ETC_GDS_CH_JTJXXX_2008_VERT_ACC_SPEC_TYPE_RDO1)->EnableWindow(TRUE);
		GetDlgItem(IDC_ETC_GDS_CH_JTJXXX_2008_VERT_ACC_SPEC_TYPE_RDO2)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_ETC_GDS_CH_JTJXXX_2008_VERT_ACC_SPEC_TYPE_RDO1)->EnableWindow(FALSE);
		GetDlgItem(IDC_ETC_GDS_CH_JTJXXX_2008_VERT_ACC_SPEC_TYPE_RDO2)->EnableWindow(FALSE);
	}
}

void CETC_GDS_CH_JTJXXX_2008::OnClickVertAccSpecType()
{
	UpdateData(TRUE);
}

void CETC_GDS_CH_JTJXXX_2008::OnChaneInputData()
{
	m_pSpfcUtil->SetDampingRatio(m_edtDampingRatio.GetEditValue());

	//Cd
	const double dCd = m_pSpfcUtil->CalcCdFromInputData();
	m_edtCd.SetEditUnit(dCd, 2);

	OnChaneOutputData();
}

void CETC_GDS_CH_JTJXXX_2008::OnChaneOutputData()
{
	const double dCi   = m_edtCi.GetEditValue();
	const double dCs   = m_edtCs.GetEditValue();
	const double dCd   = m_edtCd.GetEditValue();
	const double dEPA  = m_edtEPA.GetEditValue();
	const double dSmax = m_pSpfcUtil->CalcSMaxFromOutputData(dCi, dCs, dCd, dEPA);

	m_edtSMax.SetEditUnit(dSmax, 2);
}
	
BOOL CETC_GDS_CH_JTJXXX_2008::ISVALID()
{
	if(!IsValid_d(&m_edtDampingRatio)) return FALSE;

	if(!IsValid_d(&m_edtTg)) return FALSE;
	if(!IsValid_d(&m_edtCi)) return FALSE;
	if(!IsValid_d(&m_edtCs)) return FALSE;
	if(!IsValid_d(&m_edtCd)) return FALSE;
	if(!IsValid_d(&m_edtEPA)) return FALSE;
	if(!IsValid_d(&m_edtSMax)) return FALSE;
	if(!IsValid_d(&m_edtMaxPeriod)) return FALSE;

	return TRUE;  
}

BOOL CETC_GDS_CH_JTJXXX_2008::IsValid_d(CEdit* pEdt)
{
	MyParser parser;
	CString sValue,msg;
	double dValue;
	double dDefValue;
	double dMin;

	if(pEdt==&m_edtDampingRatio) {dDefValue=0.05;dMin=0.;}

	if(pEdt==&m_edtTg)        {dDefValue=0.25 ;dMin=0.;}
	if(pEdt==&m_edtCi)        {dDefValue=1.00 ;dMin=0.;}
	if(pEdt==&m_edtCs)        {dDefValue=1.20 ;dMin=0.;}
	if(pEdt==&m_edtCd)        {dDefValue=1.00  ;dMin=0.;}
	if(pEdt==&m_edtEPA)       {dDefValue=0.05 ;dMin=0.;}
	if(pEdt==&m_edtSMax)      {dDefValue=0.135;dMin=0.;}
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

