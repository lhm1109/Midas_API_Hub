#include "stdafx.h"
#include "SpfcTW2006Util.h"
#include "../wg_db/DBDoc.h"
#include "../wg_db/AttrCtrl.h"

CSpfcTW2006Util::CSpfcTW2006Util()
{
	m_nSpecCategory = enSpectUsedDGN;
	SetInitValueforPOSpectrum();
}

bool CSpfcTW2006Util::CheckValid()
{
	CString strMsg;
	if (m_nSpecCategory < enSpectUsedDGN || m_nSpecCategory > enSpectUsedMAX)  ASSERT(0);

	switch (m_nSeisZone)
	{
	case enSeisZoneGEN:
	case enSeisZoneNFZ:
	{
		CalcSiteMagnifyFactor();

		if ((m_nSpecCategory != enSpectUsedMAX && m_dSds <= 0.0) || (m_nSpecCategory == enSpectUsedMAX && m_dSms <= 0.0))
		{
			strMsg.Format(_LS(IDS_CMD_ERROR_NAME_POSITIVE), _T("Ss"));
			AfxMessageBox(strMsg);
			return FALSE;
		}
		if ((m_nSpecCategory != enSpectUsedMAX && m_dSd1 <= 0.0) || (m_nSpecCategory == enSpectUsedMAX && m_dSm1 <= 0.0))
		{
			strMsg.Format(_LS(IDS_CMD_ERROR_NAME_POSITIVE), _T("S1"));
			AfxMessageBox(strMsg);
			return FALSE;
		}
		if ((m_nSpecCategory != enSpectUsedMAX && m_dFda <= 0.0) || (m_nSpecCategory == enSpectUsedMAX && m_dFma <= 0.0))
		{
			strMsg.Format(_LS(IDS_CMD_ERROR_NAME_POSITIVE), _T("Fa"));
			AfxMessageBox(strMsg);
			return FALSE;
		}
		if ((m_nSpecCategory != enSpectUsedMAX && m_dFdv <= 0.0) || (m_nSpecCategory == enSpectUsedMAX && m_dFmv <= 0.0))
		{
			strMsg.Format(_LS(IDS_CMD_ERROR_NAME_POSITIVE), _T("Fv"));
			AfxMessageBox(strMsg);
			return FALSE;
		}

		CString csMsgNSF;
		if (!IsValidNearFaultFactor(csMsgNSF))
		{
			AfxMessageBox(csMsgNSF);
			return FALSE;
		}
	}
	break;
	case enSeisZoneTPB:
	{
		if (m_nSpecCategory != enSpectUsedMAX && m_dSds_t <= 0.0)
		{
			strMsg.Format(_LS(IDS_CMD_ERROR_NAME_POSITIVE), _T("Sds"));
			AfxMessageBox(strMsg);
			return FALSE;
		}
		if (m_nSpecCategory == enSpectUsedMAX && m_dSms_t <= 0.0)
		{
			strMsg.Format(_LS(IDS_CMD_ERROR_NAME_POSITIVE), _T("Sms"));
			AfxMessageBox(strMsg);
			return FALSE;
		}
		if ((m_nSpecCategory != enSpectUsedMAX && m_dTd0_t <= 0.0) || (m_nSpecCategory == enSpectUsedMAX && m_dTm0_t <= 0.0))
		{
			strMsg.Format(_LS(IDS_CMD_ERROR_NAME_POSITIVE), _T("Trans.Period"));
			AfxMessageBox(strMsg);
			return FALSE;
		}
	}
	break;
	default: ASSERT(0); break;
	}

	if (m_dI <= 0.0)
	{
		strMsg.Format(_LS(IDS_CMD_ERROR_NAME_POSITIVE), _T("I"));
		AfxMessageBox(strMsg);
		return FALSE;
	}
	if (m_dAy <= 0.0)
	{
		strMsg.Format(_LS(IDS_CMD_ERROR_NAME_POSITIVE), _T("ay"));
		AfxMessageBox(strMsg);
		return FALSE;
	}
	if (m_dR <= 0.0)
	{
		strMsg.Format(_LS(IDS_CMD_ERROR_NAME_POSITIVE), _T("R"));
		AfxMessageBox(strMsg);
		return FALSE;
	}
	if (m_dDamping <= 0.0)
	{
		strMsg.Format(_LS(IDS_CMD_ERROR_NAME_POSITIVE), _T("Damping Ratio"));
		AfxMessageBox(strMsg);
		return FALSE;
	}
	if (m_dMaxPeriod <= 0.0)
	{
		AfxMessageBox(_LS(IDS_CMD_SPECTRUM__MAX_PERIOD_GREATER_0));
		return FALSE;
	}
	if (IsFundamentalPeriodREQ())
	{
		if (m_dFundPeriod <= 0.0)
		{
			AfxMessageBox(_LS(IDS_CMD_SPECTRUM__MAX_FUNDAMENTAL_PERIOD_GREATER_0));
			return FALSE;
		}
	}
	return TRUE;
}

void CSpfcTW2006Util::MakeSpectrumData(bool bOnlyCalc)
{
	double dPrdSt = PRD_ST;
	double dPrdEd = m_dMaxPeriod;
	int nStep = PRD_NUM;
	if (bOnlyCalc) // Pushover FEMA440에서 계산할 때(Period 2배, Step 4배)
	{
		dPrdEd *= GetPeriodCoeff();
		nStep *= GetStepCoeff();
	}
	double dInc = (dPrdEd - dPrdSt) / nStep; // time interval
	double dTol = dInc / nStep; // Tolerance

	stETC_GDS_TAIWAN02_CALC stCalc;
	GetBFactorbyDamping(stCalc, m_dDamping); // Calc dBs, dB1

	switch (m_nSeisZone)
	{
	case enSeisZoneGEN: CalcGeneralZone(stCalc); break;
	case enSeisZoneNFZ: CalcNearFaultZone(stCalc); break;
	case enSeisZoneTPB: CalcTaipeiBasin(stCalc); break;
	default: ASSERT(0); break;
	}

	double dSRa = 1.0;
	double dSRv = 1.0; // Fixed for MakeSpectrumData()

	stCalc.dTg_0 = stCalc.dTd0 * dSRv / dSRa;
	if (m_nSeisZone == enSeisZoneNFZ)
	{
		stCalc.dTg_1 = stCalc.dTd0_1 * dSRv / dSRa;
	}
	else
	{
		stCalc.dTg_1 = stCalc.dTg_0;
	}
	stCalc.dTg_2 = stCalc.dTm0 * dSRv / dSRa;

	const double dT1 = m_dFundPeriod;

	InitPeriod();
	InitAccel();

	for (int i = 0; i <= nStep; i++)
	{
		double dTn = dPrdSt + dInc * i;
		double dSa = CalcSaEveryZone(stCalc, dTn, dT1, dSRa, dSRv, FALSE);

		if (m_nSpecCategory != enSpectUsedMAX)
		{
			double dTd0 = stCalc.dTd0;
			if (dTn > 0.2 * dTd0 + dTol && dTn < 0.2 * dTd0 + dInc - dTol) TransPointEveryZone(stCalc, dSRa, dSRv, 0.2 * dTd0, dT1);
			if (dTn > 0.6 * dTd0 + dTol && dTn < 0.6 * dTd0 + dInc - dTol) TransPointEveryZone(stCalc, dSRa, dSRv, 0.6 * dTd0, dT1);
			if (dTn > 1.0 * dTd0 + dTol && dTn < 1.0 * dTd0 + dInc - dTol) TransPointEveryZone(stCalc, dSRa, dSRv, 1.0 * dTd0, dT1);
			if (dTn > 2.5 * dTd0 + dTol && dTn < 2.5 * dTd0 + dInc - dTol) TransPointEveryZone(stCalc, dSRa, dSRv, 2.5 * dTd0, dT1);
		}
		else
		{
			double dTm0 = stCalc.dTm0;
			if (dTn > 0.2 * dTm0 + dTol && dTn < 0.2 * dTm0 + dInc - dTol) TransPointEveryZone(stCalc, dSRa, dSRv, 0.2 * dTm0, dT1);
			if (dTn > 0.6 * dTm0 + dTol && dTn < 0.6 * dTm0 + dInc - dTol) TransPointEveryZone(stCalc, dSRa, dSRv, 0.6 * dTm0, dT1);
			if (dTn > 1.0 * dTm0 + dTol && dTn < 1.0 * dTm0 + dInc - dTol) TransPointEveryZone(stCalc, dSRa, dSRv, 1.0 * dTm0, dT1);
			if (dTn > 2.5 * dTm0 + dTol && dTn < 2.5 * dTm0 + dInc - dTol) TransPointEveryZone(stCalc, dSRa, dSRv, 2.5 * dTm0, dT1);
		}

		AddPeriod(dTn);
		AddAccel(dSa);
	}

	if (!bOnlyCalc)
	{
		SetDescript( std::wstring(GetFunctionDescription()) );
		SetFuncName( std::wstring(GetFunctionName()) );
	}
}

void CSpfcTW2006Util::MakePOSpectrumData(double dDamping, double SRa, double SRv, CArray<double, double>& aPeriod, CArray<double, double>& aAccel)
{
	MakePOSpectrumData_sub(dDamping, SRa, SRv, aPeriod, aAccel);
}

void CSpfcTW2006Util::SetParamFromCode(const T_SPFC_CODE& codeParam)
{
	m_nSpecType = codeParam.TAIWAN06.nSpectrumType;
	m_nSeisZone = codeParam.TAIWAN06.nSeismicZone;
	m_nSpecCategory = codeParam.TAIWAN06.nSpectrumUsed;
	m_dSds = codeParam.TAIWAN06.dSds;
	m_dSd1 = codeParam.TAIWAN06.dSd1;
	m_dSms = codeParam.TAIWAN06.dSms;
	m_dSm1 = codeParam.TAIWAN06.dSm1;
	m_dNda = codeParam.TAIWAN06.dNda;
	m_dNdv = codeParam.TAIWAN06.dNdv;
	m_dNma = codeParam.TAIWAN06.dNma;
	m_dNmv = codeParam.TAIWAN06.dNmv;

	m_nSoilType = codeParam.TAIWAN06.nSoilType;
	m_dFda = codeParam.TAIWAN06.dFda;
	m_dFdv = codeParam.TAIWAN06.dFdv;
	m_dFma = codeParam.TAIWAN06.dFma;
	m_dFmv = codeParam.TAIWAN06.dFmv;

	m_nSubZone = codeParam.TAIWAN06.nSubZone;
	m_dSds_t = codeParam.TAIWAN06.dSds_t;
	m_dSms_t = codeParam.TAIWAN06.dSms_t;
	m_dTd0_t = codeParam.TAIWAN06.dTd0_t;
	m_dTm0_t = codeParam.TAIWAN06.dTm0_t;

	m_dI = codeParam.TAIWAN06.dImportanceFactor;
	m_dAy = codeParam.TAIWAN06.dSeisMagnifyFactor;
	m_dR = codeParam.TAIWAN06.dResponseModifyFactor;
	m_dDamping = codeParam.TAIWAN06.dDampingRatio;
	m_dMaxPeriod = codeParam.TAIWAN06.dMaxPeriod;
}

void CSpfcTW2006Util::SetParamFromCalc()
{
	CalcSiteMagnifyFactor();
	m_dFundPeriod = 0;
}

// Pushover 스펙트럼은 초기값 설정전에 호출되기 때문에 생성자에서 아래 함수를 반드시 먼저 불러야 한다.
void CSpfcTW2006Util::SetInitValueforPOSpectrum()
{
	m_nSpecCategory = enSpectUsedDGN;
	m_nSpecType = enSpectTypeHOR;
	m_nSeisZone = enSeisZoneGEN;

	m_dSds = 0.5;
	m_dSd1 = 0.3;
	m_dSms = 0.7;
	m_dSm1 = 0.4;

	m_nSoilType = enSoilTypeTYP1;

	m_dFda = 1.0;
	m_dFdv = 1.0;
	m_dFma = 1.0;
	m_dFmv = 1.0;
	m_dNda = 1.0;
	m_dNdv = 1.0;
	m_dNma = 1.0;
	m_dNmv = 1.0;
	
	m_dFda_1 = 1.0;
	m_dFdv_1 = 1.0;
	m_nSubZone = 0;

	m_dSds_t = 0.6;
	m_dSms_t = 0.8;
	m_dTd0_t = 1.6;
	m_dTm0_t = 1.6;
	m_dR = 1.6;
	m_dAy = 1.2;
	m_dI = 1.0;
	m_dDamping = 5.0;
	m_dMaxPeriod = PRD_ED;
	m_dFundPeriod = 1.0;
}

bool CSpfcTW2006Util::IsFundamentalPeriodREQ()
{
	return false;
}

bool CSpfcTW2006Util::IsValidNearFaultFactor(CString& rcsMsg)
{
	if (m_nSeisZone != enSeisZoneNFZ) return true;

	if ((m_nSpecCategory != enSpectUsedMAX && m_dNda < 1.0 || m_dNdv < 1.0) ||
		(m_nSpecCategory == enSpectUsedMAX && m_dNma < 1.0 || m_dNmv < 1.0))
	{
		rcsMsg = _LS(IDS_WG_CMD__ADDD__Error__TAIWAN02_NSF);
		return false;
	}
	return true;
}

CString CSpfcTW2006Util::GetSeismicZoneName(int nCode)
{
	if (nCode < 0 || nCode > 2) { ASSERT(0); return _T(""); }

	CString aItem[] =
	{
		_LS(IDS_CMD_SPFC_TAIWAN02__General_Zone),
		_LS(IDS_CMD_SPFC_TAIWAN02__Near_Fault_Zone),
		_LS(IDS_CMD_SPFC_TAIWAN02__Taipei_Basin)
	};
	return aItem[nCode];
}

CString CSpfcTW2006Util::GetSoilTypeName(int nCode)
{
	if (nCode < 0 || nCode > 3) { ASSERT(0); return _T(""); }

	CString aItem[] =
	{
		_LS(IDS_CMD_SPFC_TAIWAN02__Type1),
		_LS(IDS_CMD_SPFC_TAIWAN02__Type2),
		_LS(IDS_CMD_SPFC_TAIWAN02__Type3),
		_LS(IDS_CMD_SPFC_TAIWAN02__User)
	};
	return aItem[nCode];
}

CString CSpfcTW2006Util::GetSubZoneName(int nCode)
{
	if (nCode < 0 || nCode > 4) { ASSERT(0); return _T(""); }

	CString aItem[] =
	{
		_LS(IDS_CMD_SPFC_TAIWAN02__TaipeiBasinI),
		_LS(IDS_CMD_SPFC_TAIWAN02__TaipeiBasinII),
		_LS(IDS_CMD_SPFC_TAIWAN02__TaipeiBasinIII),
		_LS(IDS_CMD_SPFC_TAIWAN02__TaipeiBasinIV),
		_LS(IDS_CMD_SPFC_TAIWAN02__User)
	};
	return aItem[nCode];
}

CString CSpfcTW2006Util::GetSpecCategoryName(int nCode)
{
	if (nCode < 0 || nCode > 3) { ASSERT(0); return _T(""); }

	CString aItem[] =
	{
		_LS(IDS_CMD_SPFC_TAIWAN02__Design_Spectrum),
		_LS(IDS_CMD_SPFC_TAIWAN02__Small_Medium_Spectrum),
		_LS(IDS_CMD_SPFC_TAIWAN02__Maximum_Spectrum)
	};
	return aItem[nCode];
}


void CSpfcTW2006Util::CalcSiteMagnifyFactor()
{
	CDBDoc* pDoc = (CDBDoc*)CDBDoc::GetDocPoint();
	if (m_nSeisZone != enSeisZoneGEN && m_nSeisZone != enSeisZoneNFZ) return;

	if (!pDoc->m_pAttrCtrl->CalcTaiwan02SiteMagnifyFactor(TAIWAN_E_2006, m_nSeisZone, m_nSoilType,
		m_dSds, m_dSd1, m_dSms, m_dSm1, m_dNda, m_dNdv, m_dNma, m_dNmv,
		m_dFda, m_dFdv, m_dFma, m_dFmv))
		return;

	if (m_nSeisZone == enSeisZoneNFZ) // 내부 계산용
	{
		double dNda_1 = 1.0, dNdv_1 = 1.0;
		if (!pDoc->m_pAttrCtrl->CalcTaiwan02SiteMagnifyFactor(TAIWAN_E_2006, m_nSeisZone, m_nSoilType,
			m_dSds, m_dSd1, m_dSms, m_dSm1, dNda_1, dNdv_1, m_dNma, m_dNmv,
			m_dFda_1, m_dFdv_1, m_dFma, m_dFmv))
			return;
	}
}

double CSpfcTW2006Util::GetSaFactor(stETC_GDS_TAIWAN02_CALC& sCalc, const double dT, int nMIN)
{
	double dT0 = 0.0, dSS = 0.0, dS1 = 0.0;
	switch (nMIN)
	{
	case  0: { dT0 = sCalc.dTd0;   dSS = sCalc.dSDS;   dS1 = sCalc.dSD1; } break;
	case  1: { dT0 = sCalc.dTd0_1; dSS = sCalc.dSDS_1; dS1 = sCalc.dSD1_1; } break;
	default: { dT0 = sCalc.dTm0;   dSS = sCalc.dSMS;   dS1 = sCalc.dSM1; } break;
	}

	if (dT0 == 0.0) return 0.0;

	double dSa = 0.0;
	if (dT <= 0.2 * dT0)
	{
		dSa = dSS * (0.4 + (1.0 / sCalc.dBs - 0.4) * dT / (0.2 * dT0));
	}
	else if (0.2 * dT0 < dT && dT <= dT0)
	{
		dSa = dSS / sCalc.dBs;
	}
	else if (dT0 < dT && dT <= 2.5 * dT0)
	{
		if (m_nSeisZone != enSeisZoneTPB)
		{
			dSa = dS1 / (sCalc.dB1 * dT);
		}
		else
		{
			dSa = dSS / (sCalc.dB1 * dT) * dT0; // Taipei
		}
	}
	else
	{
		dSa = 0.4 * dSS / sCalc.dB1;
	}

	return dSa;
}

double CSpfcTW2006Util::GetFuFactor(double& dT, const double dT0, int nMIN)
{
	double dRa;
	if (m_nSeisZone != enSeisZoneTPB)
	{
		dRa = 1.0 + (m_dR - 1.0) / 1.5;
	}
	else
	{
		dRa = 1.0 + (m_dR - 1.0) / 2.0;
	}
	if (nMIN == 2)
	{
		dRa = m_dR;
	}
	double dSQRT = 2.0 * dRa - 1.0;
	if (dSQRT < 0.0) { ASSERT(0); return 0.0; }

	double dRt = sqrt(dSQRT);

	if (dT >= dT0)
	{
		return dRa;
	}
	else if (dT >= 0.6 * dT0 && dT < dT0)
	{
		return dRt + (dRa - dRt) * (dT - 0.6 * dT0) / (0.4 * dT0);
	}
	else if (dT >= 0.2 * dT0 && dT < 0.6 * dT0)
	{
		return dRt;
	}
	else
	{
		return dRt + (dRt - 1.0) * (dT - 0.2 * dT0) / (0.2 * dT0);
	}
}

double CSpfcTW2006Util::GetModifiedSaFu(double dSa, double dFu)
{
	double dMdf = dSa / dFu;
	if (dMdf <= 0.3) return dMdf;
	if (dMdf > 0.3 && dMdf < 0.8) return 0.52 * dMdf + 0.144;
	return 0.7 * dMdf;
}

CString CSpfcTW2006Util::GetFunctionDescription()
{
	auto L_GetSpectUsed = [](UINT nSpectUsed) -> CString
	{ // Description 제한때문에 약어 사용
		switch (nSpectUsed)
		{
		case enSpectUsedDGN: return _T("VD");
		case enSpectUsedMED: return _T("VS");
		case enSpectUsedMAX: return _T("VM");
		}
		ASSERT(0); return _T("-");
	};

	CString csDesc;
	switch (m_nSeisZone)
	{
	case enSeisZoneGEN:
	case enSeisZoneNFZ:
	{
		csDesc.Format(_LS(IDS_CMD__ADD_TAIWAN2002_PROFILE1),
			GetSeismicZoneName(m_nSeisZone),
			L_GetSpectUsed(m_nSpecCategory),
			GetSoilTypeName(m_nSoilType),
			m_dI, m_dAy, m_dR,
			m_nSpecCategory == enSpectUsedMAX ? m_dSms : m_dSds,
			m_nSpecCategory == enSpectUsedMAX ? m_dSm1 : m_dSd1);
	}
	break;
	case enSeisZoneTPB:
	{
		csDesc.Format(_LS(IDS_CMD__ADD_TAIWAN2002_PROFILE2),
			GetSubZoneName(m_nSubZone),
			L_GetSpectUsed(m_nSpecCategory),
			m_dI, m_dAy, m_dR);
	}
	break;
	default: ASSERT(0); break;
	}

	return csDesc;
}

double CSpfcTW2006Util::CalcSaEveryZone(stETC_GDS_TAIWAN02_CALC& sCalc, double dTn, double dT1, double dSRa, double dSRv, BOOL bPOcurve)
{
	double dSaD = GetSaFactor(sCalc, dTn, 0);
	double dSaM = GetSaFactor(sCalc, dTn, 2);
	double dSaD1 = 0.0;
	if (m_nSeisZone == enSeisZoneNFZ)
	{
		dSaD1 = GetSaFactor(sCalc, dTn, 1);
	}

	double dV0 = 0.0, dV1 = 0.0, dV2 = 0.0;
	if (!bPOcurve)
	{
		double dFu = GetFuFactor(dTn, sCalc.dTd0, 0);
		double dFuM = GetFuFactor(dTn, sCalc.dTm0, 2);
		double dSFD = GetModifiedSaFu(dSaD, dFu);
		double dSFM = GetModifiedSaFu(dSaM, dFuM);
		double dSFD1 = 0.0;
		if (m_nSeisZone == enSeisZoneNFZ)
		{
			dSFD1 = GetModifiedSaFu(dSaD1, dFu);
		}

		dV0 = m_dI / (1.4 * m_dAy) * dSFD;
		switch (m_nSeisZone)
		{
		case enSeisZoneGEN: dV1 = m_dI / (4.2 * m_dAy) * dSFD * dFu; break;
		case enSeisZoneNFZ: dV1 = m_dI / (4.2 * m_dAy) * dSFD1 * dFu; break;
		case enSeisZoneTPB: dV1 = m_dI / (3.5 * m_dAy) * dSFD * dFu; break;
		default: ASSERT(0); break;
		}
		dV2 = m_dI / (1.4 * m_dAy) * dSFM;
	}
	else
	{
		double dSFD = dSaD;
		double dSFM = dSaM;
		double dSFD1 = 0.0;
		if (m_nSeisZone == enSeisZoneNFZ)
		{
			dSFD1 = dSaD1;
		}

		dV0 = m_dI * dSFD;
		switch (m_nSeisZone)
		{
		case enSeisZoneGEN: dV1 = m_dI * dSFD; break;
		case enSeisZoneNFZ: dV1 = m_dI * dSFD1; break;
		case enSeisZoneTPB: dV1 = m_dI * dSFD; break;
		default: ASSERT(0); break;
		}
		dV2 = m_dI * dSFM;
	}

	double dSa = 0.0;
	switch (m_nSpecCategory)
	{
	case enSpectUsedDGN: dSa = dV0; break;
	case enSpectUsedMED: dSa = dV1; break;
	case enSpectUsedMAX: dSa = dV2; break;
	default: ASSERT(0); break;
	}
	if (m_nSpecType == enSpectTypeVER)
	{
		if (m_nSeisZone != enSeisZoneTPB)
		{
			dSa = dSa * 1.0 / 2.0;
		}
		else
		{
			dSa = dSa * 2.0 / 3.0;
		}
	}
	return dSa;
}

double CSpfcTW2006Util::GetInitFundamentalPeriod()
{
	return 0.0;
}

bool CSpfcTW2006Util::CalcGeneralZone(stETC_GDS_TAIWAN02_CALC& sCalc)
{
	sCalc.dSDS = m_dSds * m_dFda;
	sCalc.dSD1 = m_dSd1 * m_dFdv;
	sCalc.dTd0 = sCalc.dSD1 / sCalc.dSDS * (sCalc.dBs / sCalc.dB1);

	sCalc.dSMS = m_dSms * m_dFma;
	sCalc.dSM1 = m_dSm1 * m_dFmv;
	sCalc.dTm0 = sCalc.dSM1 / sCalc.dSMS * (sCalc.dBs / sCalc.dB1);
	return true;
}

bool CSpfcTW2006Util::CalcNearFaultZone(stETC_GDS_TAIWAN02_CALC& sCalc)
{
	sCalc.dSDS = m_dSds * m_dFda * m_dNda;
	sCalc.dSD1 = m_dSd1 * m_dFdv * m_dNdv;
	sCalc.dSDS_1 = m_dSds * m_dFda_1 * 1.0;
	sCalc.dSD1_1 = m_dSd1 * m_dFdv_1 * 1.0;
	sCalc.dTd0 = sCalc.dSD1 / sCalc.dSDS * (sCalc.dBs / sCalc.dB1);
	sCalc.dTd0_1 = sCalc.dSD1_1 / sCalc.dSDS_1 * (sCalc.dBs / sCalc.dB1);

	sCalc.dSMS = m_dSms * m_dFma * m_dNma;
	sCalc.dSM1 = m_dSm1 * m_dFmv * m_dNmv;
	sCalc.dTm0 = sCalc.dSM1 / sCalc.dSMS * (sCalc.dBs / sCalc.dB1);
	return true;
}

bool CSpfcTW2006Util::CalcTaipeiBasin(stETC_GDS_TAIWAN02_CALC& sCalc)
{
	sCalc.dSDS = m_dSds_t;
	sCalc.dTd0 = m_dTd0_t * (sCalc.dBs / sCalc.dB1);

	sCalc.dSMS = m_dSms_t;
	sCalc.dTm0 = m_dTm0_t * (sCalc.dBs / sCalc.dB1);
	return true;
}

bool CSpfcTW2006Util::GetBFactorbyDamping(stETC_GDS_TAIWAN02_CALC& rstCalc, const double& dDamp)
{
	double dBs, dB1;
	if (dDamp <= 2.0)
	{
		dBs = 0.8;
		dB1 = 0.8;
	}
	else if (dDamp <= 5.0 && dDamp > 2.0)
	{
		dBs = LinearInt(2.0, 5.0, 0.8, 1.0, dDamp);
		dB1 = LinearInt(2.0, 5.0, 0.8, 1.0, dDamp);
	}
	else if (dDamp <= 10.0 && dDamp > 5.0)
	{
		dBs = LinearInt(5.0, 10.0, 1.00, 1.33, dDamp);
		dB1 = LinearInt(5.0, 10.0, 1.00, 1.25, dDamp);
	}
	else if (dDamp <= 20.0 && dDamp > 10.0)
	{
		dBs = LinearInt(10.0, 20.0, 1.33, 1.60, dDamp);
		dB1 = LinearInt(10.0, 20.0, 1.25, 1.50, dDamp);
	}
	else if (dDamp <= 30.0 && dDamp > 20.0)
	{
		dBs = LinearInt(20.0, 30.0, 1.60, 1.79, dDamp);
		dB1 = LinearInt(20.0, 30.0, 1.50, 1.63, dDamp);
	}
	else if (dDamp <= 40.0 && dDamp > 30.0)
	{
		dBs = LinearInt(30.0, 40.0, 1.79, 1.87, dDamp);
		dB1 = LinearInt(30.0, 40.0, 1.63, 1.70, dDamp);
	}
	else if (dDamp <= 50.0 && dDamp > 40.0)
	{
		dBs = LinearInt(40.0, 50.0, 1.87, 1.93, dDamp);
		dB1 = LinearInt(40.0, 50.0, 1.70, 1.75, dDamp);
	}
	else
	{
		dBs = 1.93;
		dB1 = 1.75;
	}

	rstCalc.dBs = dBs;
	rstCalc.dB1 = dB1;
	return TRUE;
}

void CSpfcTW2006Util::TransPointEveryZone(stETC_GDS_TAIWAN02_CALC sCalc, double dSRa, double dSRv, double dTn, double dT1)
{
	double dSa = CalcSaEveryZone(sCalc, dTn, dT1, dSRa, dSRv, FALSE);
	AddPeriod(dTn);
	AddAccel(dSa);
}

void CSpfcTW2006Util::TransPointPOSpectrum(stETC_GDS_TAIWAN02_CALC sCalc, double dSRa, double dSRv, const double& dt, const double& dTol, double& rdSa, double dTx, const double& dTn, const double& dT1, SeisDblArray& radPeriod, SeisDblArray& radAccel, BOOL bPOcurve)
{
	// Tn이 Critical 을 넘었을 때 Critical 주기의 데이터를 추가한다.
// Critical : 0.2Td0, 0.2Tm0, dTg_0, dTg_2, 2.5Td0, 2.5Tm0  (6개,  dTg_1는 제외)
	if (dTn > dTx + dTol && dTn < dTx + dt - dTol)
	{
		rdSa = CalcSaEveryZone(sCalc, dTx, dT1, dSRa, dSRv, bPOcurve);
		radPeriod.Add(dTx);
		radAccel.Add(rdSa);
	}
}

double CSpfcTW2006Util::LinearInt(double dx1, double dx2, double dy1, double dy2, double dxx)
{
	double dyy = (dy2 - dy1) / (dx2 - dx1) * (dxx - dx1) + dy1;
	return dyy;
}

void CSpfcTW2006Util::MakePOSpectrumData_sub(double dDamping, double dSRa, double dSRv, SeisDblArray& radPeriod, SeisDblArray& radAccel, BOOL bMakeSpectrum, BOOL bOnlyCalc)
{
	// MNET:2689-GSJI-20070312 :
	// Pushover Performance Curve를 작도할 때 Factor가 적용된 값을 사용하지 않는다. (대만대리점 요청)
	BOOL bPOcurve = TRUE;
	radPeriod.RemoveAll();
	radAccel.RemoveAll();

	double dPrdSt = PRD_ST;
	double dPrdEd = m_dMaxPeriod * 2.0;
	int    nStep = PRD_NUM * 4;
	if (bMakeSpectrum)
	{
		dPrdEd = m_dMaxPeriod;
		nStep = PRD_NUM;
		if (bOnlyCalc)
		{
			dPrdEd *= GetPeriodCoeff();
			nStep *= GetStepCoeff();
		}
	}
	double dInc = (dPrdEd - dPrdSt) / nStep; // time interval
	double dTol = dInc / nStep; // Tolerance

	stETC_GDS_TAIWAN02_CALC stCalc;
	GetBFactorbyDamping(stCalc, dDamping); // Generate Spectrum 대화상자의 입력치 대신 Pushover Damping 으로 계산
	m_dDamping = dDamping;

	switch (m_nSeisZone)
	{
	case enSeisZoneGEN: CalcGeneralZone(stCalc); break;
	case enSeisZoneNFZ: CalcNearFaultZone(stCalc); break;
	case enSeisZoneTPB: CalcTaipeiBasin(stCalc); break;
	default: ASSERT(0); break;
	}

	dSRa = 1.0;
	dSRv = 1.0;
	stCalc.dTg_0 = stCalc.dTd0 * dSRv / dSRa;
	if (m_nSeisZone == enSeisZoneNFZ)
	{
		stCalc.dTg_1 = stCalc.dTd0_1 * dSRv / dSRa;
	}
	else
	{
		stCalc.dTg_1 = stCalc.dTg_0;
	}
	stCalc.dTg_2 = stCalc.dTm0 * dSRv / dSRa;

	const double dT1 = m_dFundPeriod;
	for (int i = 0; i <= nStep; i++)
	{
		double Tn = dPrdSt + dInc * i;
		double dSa = 0.0; // Normalized 5% damping spectrum
		if (m_nSpecCategory != enSpectUsedMAX)
		{
			double dTd0 = stCalc.dTd0;
			double dTg0 = stCalc.dTg_0;
			TransPointPOSpectrum(stCalc, dSRa, dSRv, dInc, dTol, dSa, 0.2 * dTd0, Tn, dT1, radPeriod, radAccel, bPOcurve);
			TransPointPOSpectrum(stCalc, dSRa, dSRv, dInc, dTol, dSa, 0.6 * dTd0, Tn, dT1, radPeriod, radAccel, bPOcurve);
			TransPointPOSpectrum(stCalc, dSRa, dSRv, dInc, dTol, dSa, 1.0 * dTg0, Tn, dT1, radPeriod, radAccel, bPOcurve);
			TransPointPOSpectrum(stCalc, dSRa, dSRv, dInc, dTol, dSa, 2.5 * dTd0, Tn, dT1, radPeriod, radAccel, bPOcurve);
		}
		else
		{
			double dTm0 = stCalc.dTm0;
			double dTg2 = stCalc.dTg_2;
			TransPointPOSpectrum(stCalc, dSRa, dSRv, dInc, dTol, dSa, 0.2 * dTm0, Tn, dT1, radPeriod, radAccel, bPOcurve);
			TransPointPOSpectrum(stCalc, dSRa, dSRv, dInc, dTol, dSa, 0.6 * dTm0, Tn, dT1, radPeriod, radAccel, bPOcurve);
			TransPointPOSpectrum(stCalc, dSRa, dSRv, dInc, dTol, dSa, 1.0 * dTg2, Tn, dT1, radPeriod, radAccel, bPOcurve);
			TransPointPOSpectrum(stCalc, dSRa, dSRv, dInc, dTol, dSa, 2.5 * dTm0, Tn, dT1, radPeriod, radAccel, bPOcurve);
		}

		dSa = CalcSaEveryZone(stCalc, Tn, dT1, dSRa, dSRv, bPOcurve);
		radPeriod.Add(Tn);
		radAccel.Add(dSa);
	}
}
