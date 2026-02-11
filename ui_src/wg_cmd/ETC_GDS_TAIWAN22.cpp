#include "stdafx.h"
#include "wg_cmd.h"
#include "ETC_GDS_TAIWAN22.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_base\DlgUtil.h"
#include "..\wg_base\SpinBtnExCtrl.h"
#include "SpfcTW2022Util.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CETC_GDS_TAIWAN22::CETC_GDS_TAIWAN22(T_SPFC_D* pParamData, CWnd* pParent) : CETC_GDS_TAIWAN02(pParamData, pParent)
{
	InitCtrlInfo();
	m_pParamData = pParamData;
	m_pSpfcBaseUtil = std::make_shared<CSpfcTW2022Util>();
	m_pSpfcUtil = std::dynamic_pointer_cast<CSpfcTW2022Util>(m_pSpfcBaseUtil);
	m_pSpfcUtil->SetInitValueforPOSpectrum();
}

BOOL CETC_GDS_TAIWAN22::Dlg2Data()
{
	if ( m_parPeriod == nullptr ) { ASSERT(0); return FALSE; }

	m_pParamData->dResModCoef = m_pSpfcUtil->m_dR;

	T_SPFC_TAIWAN22& rData = m_pParamData->CodeParam.TAIWAN22;
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
	rData.dFundamentPeriod      = m_pSpfcUtil->m_dFundPeriod;
	return TRUE;
}

void CETC_GDS_TAIWAN22::SetPushOverData(BOOL bInit)
{
	const T_SPFC_TAIWAN22& cData = m_pParamData->CodeParam.TAIWAN22;
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

	m_pSpfcUtil->m_dI          = cData.dImportanceFactor;
	m_pSpfcUtil->m_dAy         = cData.dSeisMagnifyFactor;
	m_pSpfcUtil->m_dR          = cData.dResponseModifyFactor;
	m_pSpfcUtil->m_dDamping    = cData.dDampingRatio;
	m_pSpfcUtil->m_dMaxPeriod  = cData.dMaxPeriod;
	m_pSpfcUtil->m_dFundPeriod = cData.dFundamentPeriod;

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

void CETC_GDS_TAIWAN22::InitSpecAccNearFaultZone()
{
	m_edtNda.SetWindowText(_T("0.80"));
	m_edtNdv.SetWindowText(_T("0.45"));
	m_edtNma.SetWindowText(_T("1.00"));
	m_edtNmv.SetWindowText(_T("0.55"));
}

void CETC_GDS_TAIWAN22::ChangeCaption()
{
	ChangeCtrlText(IDC_CMD_TAIWAN02_RS_ACC_STC, _LSNew("IDS_CMD_TAIWAN_RS_HORIZONTAL"));//_T("Horizontal Spectral Accel. (Table 2-1)"));
	ChangeCtrlText(IDC_CMD_TAIWAN02_RS_NSF_STC, _LSNew("IDS_CMD_TAIWAN_RS_NSF"));//_T("Near Fault Seismic Effect (Table 2-3-1 ~ 2-3-4)"));
	ChangeCtrlText(IDC_CMD_TAIWAN02_RS_NA_STC, _LSNew("IDS_IDD_ETC_GDS_TAIWAN02_IDC_IDC_CMD_TAIWAN02_RS_NA_STC"));//_T("Short Period (Ss)"));
	ChangeCtrlText(IDC_CMD_TAIWAN02_RS_NV_STC, _LSNew("IDS_IDD_ETC_GDS_TAIWAN02_IDC_CMD_TAIWAN02_RS_NV_STC"));//_T("1sec Period (S1)"));
}

void CETC_GDS_TAIWAN22::ChangeCaptionSmallMedium()
{
	CString csFa = _LSNew("IDS_IDD_ETC_GDS_TAIWAN02_IDC_CMD_TAIWAN02_RS_FA_STC");//_T("Short Period (Fa)");
	CString csFv = _LSNew("IDS_IDD_ETC_GDS_TAIWAN02_IDC_CMD_TAIWAN02_RS_FV_STC");//_T("1sec Period (Fv)");
	if (m_pSpfcUtil->m_nSpecCategory == enSpectUsedMED )
	{
		csFa = _LSNew("IDS_CMD_TAIWAN02_RS_FA_S");
		csFv = _LSNew("IDS_CMD_TAIWAN02_RS_FV_S");
	}
	ChangeCtrlText(IDC_CMD_TAIWAN02_RS_FA_STC, csFa);
	ChangeCtrlText(IDC_CMD_TAIWAN02_RS_FV_STC, csFv);
}

double CETC_GDS_TAIWAN22::GetInitFundamentalPeriod()
{
	if ( m_pParamData->nSpecCodeType == D_SPFC_CODE_TAIWAN22 )
	{
		const T_SPFC_TAIWAN22& cData = m_pParamData->CodeParam.TAIWAN22;
		return cData.dFundamentPeriod;
	}
	return 0.0;
}

void CETC_GDS_TAIWAN22::CtrlManager()
{
	CETC_GDS_TAIWAN02::CtrlManager();

	if (m_pSpfcUtil->m_nSeisZone == enSeisZoneNFZ )
	{
		// Horizontal Spectral Acceleration
		m_cmbSds.EnableWindow(FALSE);
		m_cmbSd1.EnableWindow(FALSE);
		m_cmbSms.EnableWindow(FALSE);
		m_cmbSm1.EnableWindow(FALSE);
		// Near Fault Seismic Effect
		m_edtNdv.EnableWindow(FALSE);
		m_edtNda.EnableWindow(FALSE);
		m_edtNmv.EnableWindow(FALSE);
		m_edtNma.EnableWindow(FALSE);
		// Site Magnify Factor
		BOOL bEnable = (m_pSpfcUtil->m_nSoilType == enSoilTypeUSER ) ? TRUE : FALSE;
		m_edtFmv.EnableWindow(bEnable);
		m_edtFma.EnableWindow(bEnable);
		m_edtFdv.EnableWindow(bEnable);
		m_edtFda.EnableWindow(bEnable);

		switch (m_pSpfcUtil->m_nSpecCategory )
		{
		case enSpectUsedDGN:
			{
				m_edtNdv.EnableWindow(TRUE);
				m_edtNda.EnableWindow(TRUE);
			}
			break;
		case enSpectUsedMED:
			{
				m_cmbSds.EnableWindow(TRUE);
				m_cmbSd1.EnableWindow(TRUE);
			}
			break;
		case enSpectUsedMAX:
			{
				m_edtNmv.EnableWindow(TRUE);
				m_edtNma.EnableWindow(TRUE);
			}
			break;
		default: ASSERT(0); break;
		}
	}
}

void CETC_GDS_TAIWAN22::GetSiteMagnifyFactor()
{
	CFormulaEdit::GetEditValue(&m_edtFda, m_pSpfcUtil->m_dFda);
	CFormulaEdit::GetEditValue(&m_edtFdv, m_pSpfcUtil->m_dFdv);
	CFormulaEdit::GetEditValue(&m_edtFma, m_pSpfcUtil->m_dFma);
	CFormulaEdit::GetEditValue(&m_edtFmv, m_pSpfcUtil->m_dFmv);
	if (m_pSpfcUtil->m_nSeisZone == enSeisZoneNFZ )
	{
		m_pSpfcUtil->m_dFda_1 = m_pSpfcUtil->m_dFda;
		m_pSpfcUtil->m_dFdv_1 = m_pSpfcUtil->m_dFdv;
	}
}
