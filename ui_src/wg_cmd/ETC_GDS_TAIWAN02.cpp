#include "stdafx.h"
#include "wg_cmd.h"
#include "ETC_GDS_TAIWAN02.h"
#include "SeisTaiwan86PeriodDlg.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_base\SpinBtnExCtrl.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "SpfcTW2006Util.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CETC_GDS_TAIWAN02::CETC_GDS_TAIWAN02(T_SPFC_D* pParamData, CWnd* pParent)
	: CMyChildDialog(CETC_GDS_TAIWAN02::IDD, pParent)
{
	InitCtrlInfo();
	//m_pSpfcUtil->m_nSpecCategory = enSpectUsedDGN;
	m_pParamData = pParamData;
	m_pSpfcBaseUtil = std::make_shared<CSpfcTW2006Util>();
	m_pSpfcUtil = std::dynamic_pointer_cast<CSpfcTW2006Util>(m_pSpfcBaseUtil);
	m_pSpfcUtil->SetInitValueforPOSpectrum();
}

void CETC_GDS_TAIWAN02::InitCtrlInfo()
{
	m_aCtrlMethod0.RemoveAll();
	m_aCtrlMethod0.Add(IDC_CMD_TAIWAN02_RS_ACC_STC);
	m_aCtrlMethod0.Add(IDC_CMD_TAIWAN02_RS_SDS_STC);
	m_aCtrlMethod0.Add(IDC_CMD_TAIWAN02_RS_SD1_STC);
	m_aCtrlMethod0.Add(IDC_CMD_TAIWAN02_RS_SDS_CMB);
	m_aCtrlMethod0.Add(IDC_CMD_TAIWAN02_RS_SD1_CMB);
	m_aCtrlMethod0.Add(IDC_CMD_TAIWAN02_RS_SMS_CMB);
	m_aCtrlMethod0.Add(IDC_CMD_TAIWAN02_RS_SM1_CMB);
	m_aCtrlMethod0.Add(IDC_CMD_TAIWAN02_RS_MSF_STC);
	m_aCtrlMethod0.Add(IDC_CMD_TAIWAN02_RS_SOILTYPE_STC);
	m_aCtrlMethod0.Add(IDC_CMD_TAIWAN02_RS_SOILTYPE_CMB);
	m_aCtrlMethod0.Add(IDC_CMD_TAIWAN02_RS_SMFD_STC);
	m_aCtrlMethod0.Add(IDC_CMD_TAIWAN02_RS_SMFM_STC);
	m_aCtrlMethod0.Add(IDC_CMD_TAIWAN02_RS_FMA_EDT);
	m_aCtrlMethod0.Add(IDC_CMD_TAIWAN02_RS_FMV_EDT);
	m_aCtrlMethod0.Add(IDC_CMD_TAIWAN02_RS_FDA_EDT);
	m_aCtrlMethod0.Add(IDC_CMD_TAIWAN02_RS_FDV_EDT);
	m_aCtrlMethod0.Add(IDC_CMD_TAIWAN02_RS_FA_STC);
	m_aCtrlMethod0.Add(IDC_CMD_TAIWAN02_RS_FV_STC);
	m_aCtrlMethod0.Add(IDC_CMD_TAIWAN02_RS_G1_STC);
	m_aCtrlMethod0.Add(IDC_CMD_TAIWAN02_RS_G2_STC);
	m_aCtrlMethod0.Add(IDC_CMD_TAIWAN02_RS_0D_STC);
	m_aCtrlMethod0.Add(IDC_CMD_TAIWAN02_RS_0M_STC);

	m_aCtrlMethod1.RemoveAll();
	m_aCtrlMethod1.Add(IDC_CMD_TAIWAN02_RS_NSF_STC);
	m_aCtrlMethod1.Add(IDC_CMD_TAIWAN02_RS_NA_STC);
	m_aCtrlMethod1.Add(IDC_CMD_TAIWAN02_RS_NV_STC);
	m_aCtrlMethod1.Add(IDC_CMD_TAIWAN02_RS_NDA_EDT);
	m_aCtrlMethod1.Add(IDC_CMD_TAIWAN02_RS_NDV_EDT);
	m_aCtrlMethod1.Add(IDC_CMD_TAIWAN02_RS_NSFD_STC2);
	m_aCtrlMethod1.Add(IDC_CMD_TAIWAN02_RS_NSFM_STC2);
	m_aCtrlMethod1.Add(IDC_CMD_TAIWAN02_RS_NMV_EDT);
	m_aCtrlMethod1.Add(IDC_CMD_TAIWAN02_RS_NMA_EDT);

	m_aCtrlMethod2.RemoveAll();
	m_aCtrlMethod2.Add(IDC_CMD_TAIWAN02_RS_SUBZONE_STC);
	m_aCtrlMethod2.Add(IDC_CMD_TAIWAN02_RS_SUBZONE_CMB);
	m_aCtrlMethod2.Add(IDC_CMD_TAIWAN02_RS_2D_STC);
	m_aCtrlMethod2.Add(IDC_CMD_TAIWAN02_RS_2M_STC);
	m_aCtrlMethod2.Add(IDC_CMD_TAIWAN02_RS_ACC_STC2);
	m_aCtrlMethod2.Add(IDC_CMD_TAIWAN02_RS_SS_STC);
	m_aCtrlMethod2.Add(IDC_CMD_TAIWAN02_RS_SDS_EDT2);
	m_aCtrlMethod2.Add(IDC_CMD_TAIWAN02_RS_SMS_EDT2);
	m_aCtrlMethod2.Add(IDC_CMD_TAIWAN02_RS_G2_STC2);
	m_aCtrlMethod2.Add(IDC_CMD_TAIWAN02_RS_T0_STC);
	m_aCtrlMethod2.Add(IDC_CMD_TAIWAN02_RS_2D_STC2);
	m_aCtrlMethod2.Add(IDC_CMD_TAIWAN02_RS_2M_STC2);
	m_aCtrlMethod2.Add(IDC_CMD_TAIWAN02_RS_TD0_EDT);
	m_aCtrlMethod2.Add(IDC_CMD_TAIWAN02_RS_TM0_EDT);
	m_aCtrlMethod2.Add(IDC_CMD_TAIWAN02_RS_TP_STC);

	m_aCtrl2.RemoveAll();
	m_aCtrl2.Add(IDC_CMD_TAIWAN02_RS_SMS_CMB);
	m_aCtrl2.Add(IDC_CMD_TAIWAN02_RS_SM1_CMB);
	m_aCtrl2.Add(IDC_CMD_TAIWAN02_RS_NMA_EDT);
	m_aCtrl2.Add(IDC_CMD_TAIWAN02_RS_NMV_EDT);
	m_aCtrl2.Add(IDC_CMD_TAIWAN02_RS_FMA_EDT);
	m_aCtrl2.Add(IDC_CMD_TAIWAN02_RS_FMV_EDT);
	m_aCtrl2.Add(IDC_CMD_TAIWAN02_RS_SMS_EDT2);
	m_aCtrl2.Add(IDC_CMD_TAIWAN02_RS_TM0_EDT);

	m_aCtrl1.RemoveAll();
	m_aCtrl1.Add(IDC_CMD_TAIWAN02_RS_SDS_CMB);
	m_aCtrl1.Add(IDC_CMD_TAIWAN02_RS_SD1_CMB);
	m_aCtrl1.Add(IDC_CMD_TAIWAN02_RS_NDA_EDT);
	m_aCtrl1.Add(IDC_CMD_TAIWAN02_RS_NDV_EDT);
	m_aCtrl1.Add(IDC_CMD_TAIWAN02_RS_FDA_EDT);
	m_aCtrl1.Add(IDC_CMD_TAIWAN02_RS_FDV_EDT);
	m_aCtrl1.Add(IDC_CMD_TAIWAN02_RS_SDS_EDT2);
	m_aCtrl1.Add(IDC_CMD_TAIWAN02_RS_TD0_EDT);
}


void CETC_GDS_TAIWAN02::DoDataExchange(CDataExchange* pDX)
{
	CMyChildDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CMD_TAIWAN02_RS_CATEGORY_CMB, m_cmbSpecCategory);
	DDX_Control(pDX, IDC_CMD_TAIWAN02_RS_SMS_CMB, m_cmbSms);
	DDX_Control(pDX, IDC_CMD_TAIWAN02_RS_SM1_CMB, m_cmbSm1);
	DDX_Control(pDX, IDC_CMD_TAIWAN02_RS_NMV_EDT, m_edtNmv);
	DDX_Control(pDX, IDC_CMD_TAIWAN02_RS_NMA_EDT, m_edtNma);
	DDX_Control(pDX, IDC_CMD_TAIWAN02_RS_NDV_EDT, m_edtNdv);
	DDX_Control(pDX, IDC_CMD_TAIWAN02_RS_NDA_EDT, m_edtNda);
	DDX_Control(pDX, IDC_CMD_TAIWAN02_RS_FMV_EDT, m_edtFmv);
	DDX_Control(pDX, IDC_CMD_TAIWAN02_RS_FMA_EDT, m_edtFma);
	DDX_Control(pDX, IDC_CMD_TAIWAN02_RS_FDV_EDT, m_edtFdv);
	DDX_Control(pDX, IDC_CMD_TAIWAN02_RS_FDA_EDT, m_edtFda);
	DDX_Control(pDX, IDC_CMD_TAIWAN02_RS_TYPE_CMB, m_cmbSpecType);
	DDX_Control(pDX, IDC_CMD_TAIWAN02_RS_ZONE_CMB, m_cmbSeisZone);
	DDX_Control(pDX, IDC_CMD_TAIWAN02_RS_SDS_CMB, m_cmbSds);
	DDX_Control(pDX, IDC_CMD_TAIWAN02_RS_SD1_CMB, m_cmbSd1);
	DDX_Control(pDX, IDC_CMD_TAIWAN02_RS_SOILTYPE_CMB, m_cmbSoilType);
	DDX_Control(pDX, IDC_CMD_TAIWAN02_RS_SUBZONE_CMB, m_cmbSubZone);
	DDX_Control(pDX, IDC_CMD_TAIWAN02_RS_SDS_EDT2, m_edtSds_t);
	DDX_Control(pDX, IDC_CMD_TAIWAN02_RS_SMS_EDT2, m_edtSms_t);
	DDX_Control(pDX, IDC_CMD_TAIWAN02_RS_TM0_EDT, m_edtTm0);
	DDX_Control(pDX, IDC_CMD_TAIWAN02_RS_TD0_EDT, m_edtTd0);

	DDX_Control(pDX, IDC_CMD_TAIWAN02_RS_T1_EDT, m_edtT1);
	DDX_Control(pDX, IDC_CMD_IMPORTANCE_CMB, m_cmbI);
	DDX_Control(pDX, IDC_CMD_TAIWAN02_RS_ALPHA_CMB, m_cmbAlpha);
	DDX_Control(pDX, IDC_CMD_TAIWAN02_RS_R_CMB, m_cmbR);
	DDX_Control(pDX, IDC_CMD_TAIWAN02_RS_DAMP_EDT, m_edtDamping);
	DDX_Control(pDX, IDC_CMD_PERIOD_EDIT, m_edtMaxPeriod);
}

BEGIN_MESSAGE_MAP(CETC_GDS_TAIWAN02, CMyChildDialog)
	ON_CBN_SELCHANGE(IDC_CMD_TAIWAN02_RS_TYPE_CMB, OnSelChangeCmbSpecType)
	ON_CBN_SELCHANGE(IDC_CMD_TAIWAN02_RS_ZONE_CMB, OnSelChangeCmbSeisZone)
	ON_CBN_SELCHANGE(IDC_CMD_TAIWAN02_RS_SOILTYPE_CMB, OnSelChangeCmbSoilType)
	ON_CBN_EDITCHANGE(IDC_CMD_TAIWAN02_RS_SDS_CMB, OnEditChangeCmbSpecAcc)
	ON_CBN_EDITCHANGE(IDC_CMD_TAIWAN02_RS_SD1_CMB, OnEditChangeCmbSpecAcc)
	ON_CBN_SELCHANGE(IDC_CMD_TAIWAN02_RS_SDS_CMB, OnSelChangeCmbSpecAcc)
	ON_CBN_SELCHANGE(IDC_CMD_TAIWAN02_RS_SD1_CMB, OnSelChangeCmbSpecAcc)
	ON_CBN_EDITCHANGE(IDC_CMD_TAIWAN02_RS_SMS_CMB, OnEditChangeCmbSpecAcc)
	ON_CBN_EDITCHANGE(IDC_CMD_TAIWAN02_RS_SM1_CMB, OnEditChangeCmbSpecAcc)
	ON_CBN_SELCHANGE(IDC_CMD_TAIWAN02_RS_SMS_CMB, OnSelChangeCmbSpecAcc)
	ON_CBN_SELCHANGE(IDC_CMD_TAIWAN02_RS_SM1_CMB, OnSelChangeCmbSpecAcc)
	ON_EN_CHANGE(IDC_CMD_TAIWAN02_RS_NDA_EDT, OnEditChangeEdtNSFactor)
	ON_EN_CHANGE(IDC_CMD_TAIWAN02_RS_NDV_EDT, OnEditChangeEdtNSFactor)
	ON_EN_CHANGE(IDC_CMD_TAIWAN02_RS_NMA_EDT, OnEditChangeEdtNSFactor)
	ON_EN_CHANGE(IDC_CMD_TAIWAN02_RS_NMV_EDT, OnEditChangeEdtNSFactor)
	ON_EN_CHANGE(IDC_CMD_TAIWAN02_RS_FMV_EDT, OnEditChangeEdtSMFactor)
	ON_EN_CHANGE(IDC_CMD_TAIWAN02_RS_FMA_EDT, OnEditChangeEdtSMFactor)
	ON_EN_CHANGE(IDC_CMD_TAIWAN02_RS_FDV_EDT, OnEditChangeEdtSMFactor)
	ON_EN_CHANGE(IDC_CMD_TAIWAN02_RS_FDA_EDT, OnEditChangeEdtSMFactor)
	ON_EN_CHANGE(IDC_CMD_TAIWAN02_RS_SDS_EDT2, OnEditChangeEdtTaipei)
	ON_EN_CHANGE(IDC_CMD_TAIWAN02_RS_SMS_EDT2, OnEditChangeEdtTaipei)
	ON_EN_CHANGE(IDC_CMD_TAIWAN02_RS_TM0_EDT, OnEditChangeEdtTaipei)
	ON_EN_CHANGE(IDC_CMD_TAIWAN02_RS_TD0_EDT, OnEditChangeEdtTaipei)
	ON_EN_CHANGE(IDC_CMD_TAIWAN02_RS_T1_EDT, OnEditChangeEdtFundPeriod)
	ON_CBN_SELCHANGE(IDC_CMD_TAIWAN02_RS_SUBZONE_CMB, OnSelChangeCmbSubZone)
	ON_CBN_SELCHANGE(IDC_CMD_TAIWAN02_RS_CATEGORY_CMB, OnSelChangeCmbSpecCategory)
	ON_BN_CLICKED(IDC_CMD_TAIWAN02_RS_T1_BTN, OnBtnPeriodCalc)
END_MESSAGE_MAP()

void CETC_GDS_TAIWAN02::AlignCtrl()
{
	CRect rRef, rMove;
	GetDlgItem(IDC_CMD_TAIWAN02_RS_ACC_STC)->GetWindowRect(rRef);
	GetDlgItem(IDC_CMD_TAIWAN02_RS_SUBZONE_STC)->GetWindowRect(rMove);
	int nDistY = rRef.top-rMove.top + globalUtils.ScaleByDPI(10);
	CDlgUtil::CtrlMoveDistY(this, m_aCtrlMethod2, nDistY);

	GetDlgItem(IDC_CMD_PERIOD_EDIT)->GetWindowRect(rRef);
	GetWindowRect(rMove);
	rMove.bottom = rRef.bottom + globalUtils.ScaleByDPI(12);
	GetParent()->ScreenToClient(rMove);
	MoveWindow(rMove);
}

void CETC_GDS_TAIWAN02::SetPushOverData(BOOL bInit)
{
	const T_SPFC_TAIWAN06& cData = m_pParamData->CodeParam.TAIWAN06;
	m_pSpfcUtil->m_nSpecType     = cData.nSpectrumType;
	m_pSpfcUtil->m_nSeisZone     = cData.nSeismicZone;
	m_pSpfcUtil->m_nSpecCategory = cData.nSpectrumUsed;
	m_pSpfcUtil->m_dSds          = cData.dSds;
	m_pSpfcUtil->m_dSd1          = cData.dSd1;
	m_pSpfcUtil->m_dSms          = cData.dSms;
	m_pSpfcUtil->m_dSm1          = cData.dSm1;
	m_pSpfcUtil->m_dNda          = cData.dNda;
	m_pSpfcUtil->m_dNdv          = cData.dNdv;
	m_pSpfcUtil->m_dNma          = cData.dNma;
	m_pSpfcUtil->m_dNmv          = cData.dNmv;

	m_pSpfcUtil->m_nSoilType = cData.nSoilType;
	m_pSpfcUtil->m_dFda      = cData.dFda;
	m_pSpfcUtil->m_dFdv      = cData.dFdv;
	m_pSpfcUtil->m_dFma      = cData.dFma;
	m_pSpfcUtil->m_dFmv      = cData.dFmv;

	m_pSpfcUtil->m_nSubZone = cData.nSubZone;
	m_pSpfcUtil->m_dSds_t   = cData.dSds_t;
	m_pSpfcUtil->m_dSms_t   = cData.dSms_t;
	m_pSpfcUtil->m_dTd0_t   = cData.dTd0_t;
	m_pSpfcUtil->m_dTm0_t   = cData.dTm0_t;

	m_pSpfcUtil->m_dI         = cData.dImportanceFactor;
	m_pSpfcUtil->m_dAy        = cData.dSeisMagnifyFactor;
	m_pSpfcUtil->m_dR         = cData.dResponseModifyFactor;
	m_pSpfcUtil->m_dDamping   = cData.dDampingRatio;
	m_pSpfcUtil->m_dMaxPeriod = cData.dMaxPeriod;

	if ( bInit )
	{ // pushover curve 에서 호출될때만 사용
		HWND hWnd = this->GetSafeHwnd();
		if ( !hWnd || !( ::IsWindow(hWnd) ) ) return;

		m_cmbSpecType.SetCurSel(m_pSpfcUtil->m_nSpecType);
		m_cmbSpecCategory.SetCurSel(m_pSpfcUtil->m_nSpecCategory);
		m_cmbSeisZone.SetCurSel(m_pSpfcUtil->m_nSeisZone);

		UpdateData(FALSE);

		SetComboData(m_cmbSds, m_pSpfcUtil->m_dSds);
		SetComboData(m_cmbSd1, m_pSpfcUtil->m_dSd1);
		SetComboData(m_cmbSms, m_pSpfcUtil->m_dSms);
		SetComboData(m_cmbSm1, m_pSpfcUtil->m_dSm1);

		m_edtNda.SetEditUnitAuto(m_pSpfcUtil->m_dNda);
		m_edtNdv.SetEditUnitAuto(m_pSpfcUtil->m_dNdv);
		m_edtNma.SetEditUnitAuto(m_pSpfcUtil->m_dNma);
		m_edtNmv.SetEditUnitAuto(m_pSpfcUtil->m_dNmv);

		m_cmbSoilType.SetCurSel(m_pSpfcUtil->m_nSoilType);
		m_edtFda.SetEditUnitAuto(m_pSpfcUtil->m_dFda);
		m_edtFdv.SetEditUnitAuto(m_pSpfcUtil->m_dFdv);
		m_edtFma.SetEditUnitAuto(m_pSpfcUtil->m_dFma);
		m_edtFmv.SetEditUnitAuto(m_pSpfcUtil->m_dFmv);

		m_cmbSubZone.SetCurSel(m_pSpfcUtil->m_nSubZone);
		m_edtSds_t.SetEditUnitAuto(m_pSpfcUtil->m_dSds_t);
		m_edtSms_t.SetEditUnitAuto(m_pSpfcUtil->m_dSms_t);
		m_edtTd0.SetEditUnitAuto(m_pSpfcUtil->m_dTd0_t);
		m_edtTm0.SetEditUnitAuto(m_pSpfcUtil->m_dTm0_t);

		m_edtT1.SetEditUnitAuto(m_pSpfcUtil->m_dFundPeriod);
		SetComboData(m_cmbI, m_pSpfcUtil->m_dI);
		SetComboData(m_cmbR, m_pSpfcUtil->m_dR);
		SetComboData(m_cmbAlpha, m_pSpfcUtil->m_dAy);
		m_edtDamping.SetEditUnitAuto(m_pSpfcUtil->m_dDamping);
		m_edtMaxPeriod.SetEditUnitAuto(m_pSpfcUtil->m_dMaxPeriod);

		// OK 또는 Cancel 한 이후 다시 대화상자를 열었을 때 메뉴 세팅
		CDlgUtil::CtrlShowHide(this, m_aCtrlMethod0, m_pSpfcUtil->m_nSeisZone != enSeisZoneTPB);
		CDlgUtil::CtrlShowHide(this, m_aCtrlMethod1, m_pSpfcUtil->m_nSeisZone == enSeisZoneNFZ);
		CDlgUtil::CtrlShowHide(this, m_aCtrlMethod2, m_pSpfcUtil->m_nSeisZone == enSeisZoneTPB);

		HideCtrlForPushOver();
		GetFinalValue();
		CtrlManager();
	}
	else
	{
		m_pSpfcUtil->CalcSiteMagnifyFactor();
	}
}

void CETC_GDS_TAIWAN02::SetComboData(MComboBox& cobx, const double& dValue)
{
	CString csValue;
	csValue.Format(_T("%g"), dValue);
	cobx.SetWindowText(csValue);
}

BOOL CETC_GDS_TAIWAN02::OnInitDialog()
{
	CMyChildDialog::OnInitDialog();

	for ( int i = 0; i < 3; ++i )
	{
		CDlgUtil::CobxAddItem(m_cmbSpecCategory, m_pSpfcUtil->GetSpecCategoryName(i), i);
	}
	m_cmbSpecCategory.SetCurSel(0);

	m_cmbSpecType.SetItemData(m_cmbSpecType.AddString(_LS(IDS_CMD_EURO2003_H_DESIGN_SPECTRUM)), 0); // _T("Horizontal Design Spectrum")
	m_cmbSpecType.SetItemData(m_cmbSpecType.AddString(_LS(IDS_CMD_EURO2003_V_DESIGN_SPECTRUM)), 1); // _T("Vertical Design Spectrum")
	m_cmbSpecType.SetCurSel(0);

	for ( int i = 0; i < 3; i++ )
	{
		CDlgUtil::CobxAddItem(m_cmbSeisZone, m_pSpfcUtil->GetSeismicZoneName(i), i);
	}
	m_cmbSeisZone.SetCurSel(0);
	
	AlignCtrl();
	CDlgUtil::CtrlShowHide(this, m_aCtrlMethod0, m_pSpfcUtil->m_nSeisZone != enSeisZoneTPB);
	CDlgUtil::CtrlShowHide(this, m_aCtrlMethod1, m_pSpfcUtil->m_nSeisZone == enSeisZoneNFZ);
	CDlgUtil::CtrlShowHide(this, m_aCtrlMethod2, m_pSpfcUtil->m_nSeisZone == enSeisZoneTPB);

	InitSpecAcc();
	InitSpecAccNearFaultZone();
	InitSiteMagnifyFactor();
	InitTaipei();
	InitIFactorCombo();
	InitAyFactorCombo();
	InitRCombo();

	m_edtDamping.SetEditUnitAuto(5.0); //5% 감쇠 기본
	m_pSpfcUtil->m_dDamping = m_edtDamping.GetEditValue();

	m_edtMaxPeriod.SetEditUnitAuto(PRD_ED);
	m_pSpfcUtil->m_dMaxPeriod = m_edtMaxPeriod.GetEditValue();

	m_pSpfcUtil->m_dFundPeriod = GetInitFundamentalPeriod();
	m_edtT1.SetEditUnitAuto(m_pSpfcUtil->m_dFundPeriod);

	GetFinalValue();
	CtrlManager();
	ChangeCaption();
	ChangeCaptionSmallMedium();
	EnableCtrlFundamentPeriod();

	return TRUE;
}

void CETC_GDS_TAIWAN02::OnSelChangeCmbSeisZone()
{
	m_pSpfcUtil->m_nSeisZone = m_cmbSeisZone.GetItemData(m_cmbSeisZone.GetCurSel());
	CDlgUtil::CtrlShowHide(this, m_aCtrlMethod0, m_pSpfcUtil->m_nSeisZone != enSeisZoneTPB);
	CDlgUtil::CtrlShowHide(this, m_aCtrlMethod1, m_pSpfcUtil->m_nSeisZone == enSeisZoneNFZ);
	CDlgUtil::CtrlShowHide(this, m_aCtrlMethod2, m_pSpfcUtil->m_nSeisZone == enSeisZoneTPB);
	SetSiteMagnifyFactor();
	CtrlManager();
}

void CETC_GDS_TAIWAN02::OnSelChangeCmbSpecType()
{
	m_pSpfcUtil->m_nSpecType = m_cmbSpecType.GetItemData(m_cmbSpecType.GetCurSel());
}

void CETC_GDS_TAIWAN02::OnEditChangeCmbSpecAcc()
{
	CtrlManager();
	SetSpectralData();
	SetSiteMagnifyFactor();
}

void CETC_GDS_TAIWAN02::OnSelChangeCmbSpecAcc()
{
	PostMessage(WM_COMMAND, MAKEWPARAM(IDC_CMD_TAIWAN02_RS_SDS_CMB, CBN_EDITCHANGE), ( LPARAM ) m_cmbSds.GetSafeHwnd());
	PostMessage(WM_COMMAND, MAKEWPARAM(IDC_CMD_TAIWAN02_RS_SD1_CMB, CBN_EDITCHANGE), ( LPARAM ) m_cmbSd1.GetSafeHwnd());
	PostMessage(WM_COMMAND, MAKEWPARAM(IDC_CMD_TAIWAN02_RS_SMS_CMB, CBN_EDITCHANGE), ( LPARAM ) m_cmbSms.GetSafeHwnd());
	PostMessage(WM_COMMAND, MAKEWPARAM(IDC_CMD_TAIWAN02_RS_SM1_CMB, CBN_EDITCHANGE), ( LPARAM ) m_cmbSm1.GetSafeHwnd());
}

void CETC_GDS_TAIWAN02::OnSelChangeCmbSoilType()
{
	CtrlManager();
	SetSiteMagnifyFactor();
}

void CETC_GDS_TAIWAN02::OnSelChangeCmbSubZone()
{
	CtrlManager();
	SetChangeTaipei();
}

void CETC_GDS_TAIWAN02::OnEditChangeEdtNSFactor()
{
	m_pSpfcUtil->m_dNda = m_edtNda.GetEditValue();
	m_pSpfcUtil->m_dNdv = m_edtNdv.GetEditValue();
	m_pSpfcUtil->m_dNma = m_edtNma.GetEditValue();
	m_pSpfcUtil->m_dNmv = m_edtNmv.GetEditValue();
	SetSiteMagnifyFactor();
}

void CETC_GDS_TAIWAN02::OnEditChangeEdtSMFactor()
{
	GetSiteMagnifyFactor();
}

void CETC_GDS_TAIWAN02::GetSiteMagnifyFactor()
{
	CFormulaEdit::GetEditValue(&m_edtFda, m_pSpfcUtil->m_dFda);
	CFormulaEdit::GetEditValue(&m_edtFdv, m_pSpfcUtil->m_dFdv);
	CFormulaEdit::GetEditValue(&m_edtFma, m_pSpfcUtil->m_dFma);
	CFormulaEdit::GetEditValue(&m_edtFmv, m_pSpfcUtil->m_dFmv);
}

void CETC_GDS_TAIWAN02::OnEditChangeEdtTaipei()
{
	CFormulaEdit::GetEditValue(&m_edtSds_t, m_pSpfcUtil->m_dSds_t);
	CFormulaEdit::GetEditValue(&m_edtSms_t, m_pSpfcUtil->m_dSms_t);
	CFormulaEdit::GetEditValue(&m_edtTd0, m_pSpfcUtil->m_dTd0_t);
	CFormulaEdit::GetEditValue(&m_edtTm0, m_pSpfcUtil->m_dTm0_t);
}

void CETC_GDS_TAIWAN02::OnEditChangeEdtFundPeriod()
{
	m_pSpfcUtil->m_dFundPeriod = m_edtT1.GetEditValue();
}

void CETC_GDS_TAIWAN02::OnBtnPeriodCalc()
{
	if ( !m_pSpfcUtil->IsFundamentalPeriodREQ() ) return;

	UINT nMode = EN_DLG_MODE_TAIWAN_2022;
	CSeisTaiwan86PeriodDlg PeriodDLG(nMode, true);
	PeriodDLG.SetInitPos(D_INIT_POS_RT);

	double dHn = GetBuildingHeight();
	PeriodDLG.m_nMethodX = 0;
	PeriodDLG.m_nMethodY = 0;
	PeriodDLG.m_dHnX = dHn;
	PeriodDLG.m_dHnY = dHn;
	PeriodDLG.m_dAcX = 0.0;
	PeriodDLG.m_dAcY = 0.0;

	if ( PeriodDLG.DoModal() == IDOK )
	{
		m_pSpfcUtil->m_dFundPeriod = PeriodDLG.m_dPeriodX;
		m_edtT1.SetEditUnitAuto(m_pSpfcUtil->m_dFundPeriod);
	}
}

double CETC_GDS_TAIWAN02::GetBuildingHeight()
{
	CCurUnitSaver UnitSaver(TRUE);

	CDBDoc* pDoc = (CDBDoc*)CDBDoc::GetDocPoint();
	if (pDoc == nullptr) { ASSERT(0); return 0.0; }

	CUnitCtrl* pUnitCtrl = pDoc->m_pUnitCtrl;
	if (pUnitCtrl == nullptr) { ASSERT(0); return 0.0; }

	T_UNIT_INDEX CodeIndex;
	pUnitCtrl->GetUnitIndexCurrent(CodeIndex);
	CodeIndex.nBase_Length = D_UNITSYS_LENGTH_INDEX_M;
	pUnitCtrl->SetUnitIndexCurrent(CodeIndex);

	CAttrCtrl* pAttrCtrl = pDoc->m_pAttrCtrl;
	if (pAttrCtrl == nullptr) { ASSERT(0); return 0.0; }

	CArray<T_STOR_K, T_STOR_K> aStorK;
	pAttrCtrl->GetStorKeyList(aStorK);
	int nStorNum = aStorK.GetSize();
	if (nStorNum < 1) return 0.0;

	T_STOR_D StorD;
	StorD.Initialize();
	pAttrCtrl->GetStor(aStorK[nStorNum - 1], StorD);

	double dHn = 0.0;
	if (pAttrCtrl->GetCountStor())
	{
		double dBaseLevel = pAttrCtrl->GetBaseLevel(FALSE);
		dHn = StorD.dStoryLevel - dBaseLevel;
	}
	return dHn;
}

BOOL CETC_GDS_TAIWAN02::ISVALID()
{
	GetFinalValue();

	CString strMsg;
	DWORD nItem;
	CDlgUtil::CobxGetItemDataByCurSel(m_cmbSpecCategory, nItem);
	m_pSpfcUtil->m_nSpecCategory = nItem;
	if ( m_pSpfcUtil->m_nSpecCategory < enSpectUsedDGN || m_pSpfcUtil->m_nSpecCategory > enSpectUsedMAX )  ASSERT(0);

	m_pSpfcUtil->m_nSeisZone = m_cmbSeisZone.GetItemData(m_cmbSeisZone.GetCurSel());
	switch ( m_pSpfcUtil->m_nSeisZone )
	{
	case enSeisZoneGEN:
	case enSeisZoneNFZ:
		{
			if ( ( m_pSpfcUtil->m_nSpecCategory != enSpectUsedMAX && m_pSpfcUtil->m_dSds<= 0.0 ) || ( m_pSpfcUtil->m_nSpecCategory == enSpectUsedMAX && m_pSpfcUtil->m_dSms <= 0.0 ) )
			{
				strMsg.Format(_LS(IDS_CMD_ERROR_NAME_POSITIVE), _T("Ss"));
				AfxMessageBox(strMsg);
				return FALSE;
			}
			if ( ( m_pSpfcUtil->m_nSpecCategory != enSpectUsedMAX && m_pSpfcUtil->m_dSd1 <= 0.0 ) || ( m_pSpfcUtil->m_nSpecCategory == enSpectUsedMAX && m_pSpfcUtil->m_dSm1 <= 0.0 ) )
			{
				strMsg.Format(_LS(IDS_CMD_ERROR_NAME_POSITIVE), _T("S1"));
				AfxMessageBox(strMsg);
				return FALSE;
			}
			if ( ( m_pSpfcUtil->m_nSpecCategory != enSpectUsedMAX && m_pSpfcUtil->m_dFda <= 0.0 ) || ( m_pSpfcUtil->m_nSpecCategory == enSpectUsedMAX && m_pSpfcUtil->m_dFma <= 0.0 ) )
			{
				strMsg.Format(_LS(IDS_CMD_ERROR_NAME_POSITIVE), _T("Fa"));
				AfxMessageBox(strMsg);
				return FALSE;
			}
			if ( ( m_pSpfcUtil->m_nSpecCategory != enSpectUsedMAX && m_pSpfcUtil->m_dFdv <= 0.0 ) || ( m_pSpfcUtil->m_nSpecCategory == enSpectUsedMAX && m_pSpfcUtil->m_dFmv <= 0.0 ) )
			{
				strMsg.Format(_LS(IDS_CMD_ERROR_NAME_POSITIVE), _T("Fv"));
				AfxMessageBox(strMsg);
				return FALSE;
			}

			CString csMsgNSF;
			if ( !m_pSpfcUtil->IsValidNearFaultFactor(csMsgNSF) )
			{
				AfxMessageBox(csMsgNSF);
				return FALSE;
			}
		}
		break;
	case enSeisZoneTPB:
		{
			if ( m_pSpfcUtil->m_nSpecCategory != enSpectUsedMAX && m_pSpfcUtil->m_dSds_t <= 0.0 )
			{
				strMsg.Format(_LS(IDS_CMD_ERROR_NAME_POSITIVE), _T("Sds"));
				AfxMessageBox(strMsg);
				return FALSE;
			}
			if ( m_pSpfcUtil->m_nSpecCategory == enSpectUsedMAX && m_pSpfcUtil->m_dSms_t <= 0.0 )
			{
				strMsg.Format(_LS(IDS_CMD_ERROR_NAME_POSITIVE), _T("Sms"));
				AfxMessageBox(strMsg);
				return FALSE;
			}
			if ( ( m_pSpfcUtil->m_nSpecCategory != enSpectUsedMAX && m_pSpfcUtil->m_dTd0_t <= 0.0 ) || ( m_pSpfcUtil->m_nSpecCategory == enSpectUsedMAX && m_pSpfcUtil->m_dTm0_t <= 0.0 ) )
			{
				strMsg.Format(_LS(IDS_CMD_ERROR_NAME_POSITIVE), _T("Trans.Period"));
				AfxMessageBox(strMsg);
				return FALSE;
			}
		}
		break;
	default: ASSERT(0); break;
	}

	if ( m_pSpfcUtil->m_dI <= 0.0 )
	{
		strMsg.Format(_LS(IDS_CMD_ERROR_NAME_POSITIVE), _T("I"));
		AfxMessageBox(strMsg);
		return FALSE;
	}
	if ( m_pSpfcUtil->m_dAy <= 0.0 )
	{
		strMsg.Format(_LS(IDS_CMD_ERROR_NAME_POSITIVE), _T("ay"));
		AfxMessageBox(strMsg);
		return FALSE;
	}
	if ( m_pSpfcUtil->m_dR <= 0.0 )
	{
		strMsg.Format(_LS(IDS_CMD_ERROR_NAME_POSITIVE), _T("R"));
		AfxMessageBox(strMsg);
		return FALSE;
	}
	if ( m_pSpfcUtil->m_dDamping <= 0.0 )
	{
		strMsg.Format(_LS(IDS_CMD_ERROR_NAME_POSITIVE), _T("Damping Ratio"));
		AfxMessageBox(strMsg);
		return FALSE;
	}
	if ( m_pSpfcUtil->m_dMaxPeriod <= 0.0 )
	{
		AfxMessageBox(_LS(IDS_CMD_SPECTRUM__MAX_PERIOD_GREATER_0));
		return FALSE;
	}
	if ( m_pSpfcUtil->IsFundamentalPeriodREQ() )
	{
		if ( m_pSpfcUtil->m_dFundPeriod <= 0.0 )
		{
			AfxMessageBox(_LS(IDS_CMD_SPECTRUM__MAX_FUNDAMENTAL_PERIOD_GREATER_0));
			return FALSE;
		}
	}
	return TRUE;
}

void CETC_GDS_TAIWAN02::End()
{
	CMyChildDialog::End();
}

void CETC_GDS_TAIWAN02::InitSpecAcc()
{
	std::vector<CString> vcsItemSds;
	vcsItemSds.push_back(_T("0.50"));
	vcsItemSds.push_back(_T("0.60"));
	vcsItemSds.push_back(_T("0.70"));
	vcsItemSds.push_back(_T("0.80"));
	InitComboBox(m_cmbSds, vcsItemSds);

	std::vector<CString> vcsItemSd1;
	vcsItemSd1.push_back(_T("0.30"));
	vcsItemSd1.push_back(_T("0.35"));
	vcsItemSd1.push_back(_T("0.40"));
	vcsItemSd1.push_back(_T("0.45"));
	InitComboBox(m_cmbSd1, vcsItemSd1);

	std::vector<CString> vcsItemSms;
	vcsItemSms.push_back(_T("0.70"));
	vcsItemSms.push_back(_T("0.80"));
	vcsItemSms.push_back(_T("0.90"));
	vcsItemSms.push_back(_T("1.00"));
	InitComboBox(m_cmbSms, vcsItemSms);

	std::vector<CString> vcsItemSm1;
	vcsItemSm1.push_back(_T("0.40"));
	vcsItemSm1.push_back(_T("0.45"));
	vcsItemSm1.push_back(_T("0.50"));
	vcsItemSm1.push_back(_T("0.55"));
	InitComboBox(m_cmbSm1, vcsItemSm1);
}

void CETC_GDS_TAIWAN02::InitSpecAccNearFaultZone()
{
	m_edtNda.SetWindowText(_T("1.0"));
	m_edtNdv.SetWindowText(_T("1.0"));
	m_edtNma.SetWindowText(_T("1.0"));
	m_edtNmv.SetWindowText(_T("1.0"));
}

void CETC_GDS_TAIWAN02::InitSiteMagnifyFactor()
{
	for ( int i = 0; i < 4; i++ )
	{
		CDlgUtil::CobxAddItem(m_cmbSoilType, m_pSpfcUtil->GetSoilTypeName(i), i);
	}
	m_cmbSoilType.SetCurSel(0);
	m_edtFda.SetWindowText(_T("1.0"));
	m_edtFdv.SetWindowText(_T("1.0"));
	m_edtFma.SetWindowText(_T("1.0"));
	m_edtFmv.SetWindowText(_T("1.0"));
}

void CETC_GDS_TAIWAN02::InitTaipei()
{
	for ( int i = 0; i < 5; i++ )
	{
		CDlgUtil::CobxAddItem(m_cmbSubZone, m_pSpfcUtil->GetSubZoneName(i), i);
	}
	m_cmbSubZone.SetCurSel(0);

	m_edtSds_t.SetEditUnitAuto(0.6);
	m_edtSms_t.SetEditUnitAuto(0.8);
	m_edtTd0.SetEditUnitAuto(1.6);
	m_edtTm0.SetEditUnitAuto(1.6);

	m_pSpfcUtil->m_dSds_t = m_edtSds_t.GetEditValue();
	m_pSpfcUtil->m_dSms_t = m_edtSms_t.GetEditValue();
	m_pSpfcUtil->m_dTd0_t = m_edtTd0.GetEditValue();
	m_pSpfcUtil->m_dTm0_t = m_edtTm0.GetEditValue();
}

void CETC_GDS_TAIWAN02::InitIFactorCombo()
{
	std::vector<CString> vcsItem;
	vcsItem.push_back(_T("1.00"));
	vcsItem.push_back(_T("1.25"));
	vcsItem.push_back(_T("1.50"));
	InitComboBox(m_cmbI, vcsItem);
}

void CETC_GDS_TAIWAN02::InitAyFactorCombo()
{
	std::vector<CString> vcsItem;
	vcsItem.push_back(_T("1.0"));
	vcsItem.push_back(_T("1.2"));
	vcsItem.push_back(_T("1.5"));
	InitComboBox(m_cmbAlpha, vcsItem);
}

void CETC_GDS_TAIWAN02::InitRCombo()
{
	std::vector<CString> vcsItem;
	vcsItem.push_back(_T("1.6"));
	vcsItem.push_back(_T("2.4"));
	vcsItem.push_back(_T("2.8"));
	vcsItem.push_back(_T("3.2"));
	vcsItem.push_back(_T("3.6"));
	vcsItem.push_back(_T("4.0"));
	vcsItem.push_back(_T("4.4"));
	vcsItem.push_back(_T("4.8"));
	InitComboBox(m_cmbR, vcsItem);
}

void CETC_GDS_TAIWAN02::InitComboBox(MComboBox& rCombo, std::vector<CString> vcsItem)
{
	rCombo.ResetContent();
	if ( vcsItem.empty() ) { ASSERT(0); return; }

	for ( const CString& csItem : vcsItem )
	{
		rCombo.AddString(csItem);
	}
	rCombo.SetCurSel(0);
}

void CETC_GDS_TAIWAN02::CtrlManager()
{
	CDlgUtil::CtrlEnableDisable(this, m_aCtrl1, m_pSpfcUtil->m_nSpecCategory != enSpectUsedMAX);
	CDlgUtil::CtrlEnableDisable(this, m_aCtrl2, m_pSpfcUtil->m_nSpecCategory == enSpectUsedMAX);

	// Site Magnify Factor
	m_pSpfcUtil->m_nSoilType = m_cmbSoilType.GetItemData(m_cmbSoilType.GetCurSel());
	if ( m_pSpfcUtil->m_nSpecCategory != enSpectUsedMAX )
	{
		m_edtFdv.SetReadOnly(m_pSpfcUtil->m_nSoilType != enSoilTypeUSER);
		m_edtFda.SetReadOnly(m_pSpfcUtil->m_nSoilType != enSoilTypeUSER);
	}
	else
	{
		m_edtFmv.SetReadOnly(m_pSpfcUtil->m_nSoilType != enSoilTypeUSER);
		m_edtFma.SetReadOnly(m_pSpfcUtil->m_nSoilType != enSoilTypeUSER);
	}

	// Taipei Basin only
	m_pSpfcUtil->m_nSubZone = m_cmbSubZone.GetItemData(m_cmbSubZone.GetCurSel());
	if ( m_pSpfcUtil->m_nSpecCategory != enSpectUsedMAX )
	{
		m_edtSds_t.SetReadOnly(m_pSpfcUtil->m_nSubZone != enSubZoneUSER);
		m_edtTd0.SetReadOnly(m_pSpfcUtil->m_nSubZone != enSubZoneUSER);
	}
	else
	{
		m_edtSms_t.SetReadOnly(m_pSpfcUtil->m_nSubZone != enSubZoneUSER);
		m_edtTm0.SetReadOnly(m_pSpfcUtil->m_nSubZone != enSubZoneUSER);
	}
}

void CETC_GDS_TAIWAN02::EnableCtrlFundamentPeriod()
{
	CArray<UINT, UINT> aCtrlID;
	aCtrlID.Add(IDC_CMD_TAIWAN02_RS_T1_STC);
	aCtrlID.Add(IDC_CMD_TAIWAN02_RS_T1_EDT);
	aCtrlID.Add(IDC_CMD_TAIWAN02_RS_T1_BTN);

	bool bEnable = m_pSpfcUtil->IsFundamentalPeriodREQ();
	CDlgUtil::CtrlEnableDisable(this, aCtrlID, bEnable);
}

void CETC_GDS_TAIWAN02::HideCtrlForPushOver()
{
	CArray<UINT, UINT> aCtrlID;
	aCtrlID.Add(IDC_ETC_GDS_TAIWAN_R_STC);
	aCtrlID.Add(IDC_ETC_GDS_TAIWAN_AY_STC);
	aCtrlID.Add(IDC_CMD_TAIWAN02_RS_R_CMB);
	aCtrlID.Add(IDC_CMD_TAIWAN02_RS_ALPHA_CMB);
	aCtrlID.Add(IDC_ETC_GDS_DAMPING_STATIC);
	aCtrlID.Add(IDC_CMD_TAIWAN02_RS_DAMP_EDT);
	CDlgUtil::CtrlShowHide(this, aCtrlID, FALSE);
}

void CETC_GDS_TAIWAN02::SetSpectralData()
{
	CFormulaEdit::GetEditValue(&m_cmbSds, m_pSpfcUtil->m_dSds);
	CFormulaEdit::GetEditValue(&m_cmbSd1, m_pSpfcUtil->m_dSd1);
	CFormulaEdit::GetEditValue(&m_cmbSms, m_pSpfcUtil->m_dSms);
	CFormulaEdit::GetEditValue(&m_cmbSm1, m_pSpfcUtil->m_dSm1);
}

void CETC_GDS_TAIWAN02::SetSiteMagnifyFactor()
{
	if ( m_pSpfcUtil->m_nSoilType == enSoilTypeUSER )
	{
		OnEditChangeEdtSMFactor();
	}
	else // Type1 ~ 3
	{
		m_pSpfcUtil->CalcSiteMagnifyFactor();
		if ( m_pSpfcUtil->m_nSpecCategory != enSpectUsedMAX )
		{
			m_edtFda.SetEditUnitAuto(m_pSpfcUtil->m_dFda);
			m_edtFdv.SetEditUnitAuto(m_pSpfcUtil->m_dFdv);
		}
		else
		{
			m_edtFma.SetEditUnitAuto(m_pSpfcUtil->m_dFma);
			m_edtFmv.SetEditUnitAuto(m_pSpfcUtil->m_dFmv);
		}
	}
}

void CETC_GDS_TAIWAN02::SetChangeTaipei()
{
	m_edtSds_t.SetEditUnitAuto(0.6);
	m_edtSms_t.SetEditUnitAuto(0.8);

	m_pSpfcUtil->m_nSubZone = m_cmbSubZone.GetItemData(m_cmbSubZone.GetCurSel());

	auto L_SetT0 = [ ] (int nSubZone, CEditUnit& edtUnit)
	{
		switch ( nSubZone )
		{
		case enSubZoneTPB1: edtUnit.SetEditUnitAuto(1.60); break;
		case enSubZoneTPB2: edtUnit.SetEditUnitAuto(1.30); break;
		case enSubZoneTPB3: edtUnit.SetEditUnitAuto(1.05); break;
		case enSubZoneTPB4: edtUnit.SetEditUnitAuto(0.85); break;
		case enSubZoneUSER: edtUnit.SetEditUnitAuto(1.00); break;
		}
	};
	if ( m_pSpfcUtil->m_nSpecCategory != enSpectUsedMAX )
	{
		L_SetT0(m_pSpfcUtil->m_nSubZone, m_edtTd0);
	}
	else
	{
		L_SetT0(m_pSpfcUtil->m_nSubZone, m_edtTm0);
	}
}

void CETC_GDS_TAIWAN02::GetFinalValue()
{
	m_pSpfcUtil->m_nSpecType     = m_cmbSpecType.GetCurSel();
	m_pSpfcUtil->m_nSpecCategory = m_cmbSpecCategory.GetCurSel();
	m_pSpfcUtil->m_nSeisZone     = m_cmbSeisZone.GetCurSel();

	CDlgUtil::CobxGetCurSelItemData(m_cmbSpecCategory, m_pSpfcUtil->m_nSpecCategory);

	switch ( m_pSpfcUtil->m_nSeisZone )
	{
	case enSeisZoneGEN:
		{
			SetSpectralData();
			SetSiteMagnifyFactor();
		}
		break;
	case enSeisZoneNFZ:
		{
			SetSpectralData();
			SetSiteMagnifyFactor();
			OnEditChangeEdtNSFactor();
		}
		break;
	case enSeisZoneTPB:
		{
			OnEditChangeEdtTaipei();
		}
		break;
	default: ASSERT(0); break;
	}

	m_pSpfcUtil->m_dI  = CDlgUtil::GetComboData(m_cmbI);
	m_pSpfcUtil->m_dAy = CDlgUtil::GetComboData(m_cmbAlpha);
	m_pSpfcUtil->m_dR  = CDlgUtil::GetComboData(m_cmbR);
	m_pSpfcUtil->m_dDamping = m_edtDamping.GetEditValue();
	m_pSpfcUtil->m_dMaxPeriod = m_edtMaxPeriod.GetEditValue();
	m_pSpfcUtil->m_dFundPeriod = m_edtT1.GetEditValue();
}

void CETC_GDS_TAIWAN02::ChangeCaption()
{
	return;
}

void CETC_GDS_TAIWAN02::ChangeCaptionSmallMedium()
{
	return;
}

void CETC_GDS_TAIWAN02::ChangeCtrlText(UINT nCtrlID, const CString& csCaption)
{
	CWnd* pWnd = GetDlgItem(nCtrlID);
	if ( pWnd == nullptr ) { ASSERT(0); return; }
	pWnd->SetWindowText(csCaption);
}

double CETC_GDS_TAIWAN02::GetInitFundamentalPeriod()
{
	return 0.0;
}

void CETC_GDS_TAIWAN02::OnSelChangeCmbSpecCategory()
{
	DWORD nItem;
	CDlgUtil::CobxGetItemDataByCurSel(m_cmbSpecCategory, nItem);
	m_pSpfcUtil->m_nSpecCategory = nItem;

	CtrlManager();
	SetSiteMagnifyFactor();
	ChangeCaptionSmallMedium();
}

BOOL CETC_GDS_TAIWAN02::Dlg2Data()
{
	if ( m_parPeriod == nullptr ) { ASSERT(0); return FALSE; }

	m_pParamData->dResModCoef = m_pSpfcUtil->m_dR;

	T_SPFC_TAIWAN06& rData = m_pParamData->CodeParam.TAIWAN06;
	rData.nSpectrumType = m_pSpfcUtil->m_nSpecType;
	rData.nSeismicZone  = m_pSpfcUtil->m_nSeisZone;
	rData.nSpectrumUsed = m_pSpfcUtil->m_nSpecCategory;
	rData.dSds          = m_pSpfcUtil->m_dSds;
	rData.dSd1          = m_pSpfcUtil->m_dSd1;
	rData.dSms          = m_pSpfcUtil->m_dSms;
	rData.dSm1          = m_pSpfcUtil->m_dSm1;
	rData.dNda          = m_pSpfcUtil->m_dNda;
	rData.dNdv          = m_pSpfcUtil->m_dNdv;
	rData.dNma          = m_pSpfcUtil->m_dNma;
	rData.dNmv          = m_pSpfcUtil->m_dNmv;

	rData.nSoilType = m_pSpfcUtil->m_nSoilType;
	rData.dFda      = m_pSpfcUtil->m_dFda;
	rData.dFdv      = m_pSpfcUtil->m_dFdv;
	rData.dFma      = m_pSpfcUtil->m_dFma;
	rData.dFmv      = m_pSpfcUtil->m_dFmv;

	rData.nSubZone  = m_pSpfcUtil->m_nSubZone;
	rData.dSds_t    = m_pSpfcUtil->m_dSds_t;
	rData.dSms_t    = m_pSpfcUtil->m_dSms_t;
	rData.dTd0_t    = m_pSpfcUtil->m_dTd0_t;
	rData.dTm0_t    = m_pSpfcUtil->m_dTm0_t;

	rData.dImportanceFactor     = m_pSpfcUtil->m_dI;
	rData.dSeisMagnifyFactor    = m_pSpfcUtil->m_dAy;
	rData.dResponseModifyFactor = m_pSpfcUtil->m_dR;
	rData.dDampingRatio         = m_pSpfcUtil->m_dDamping;
	rData.dMaxPeriod            = m_pSpfcUtil->m_dMaxPeriod;
	return TRUE;
}
