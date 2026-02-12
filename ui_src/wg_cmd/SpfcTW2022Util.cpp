#include "StdAfx.h"
#include "SpfcTW2022Util.h"
#include "../wg_db/DBDoc.h"
#include "../wg_db/AttrCtrl.h"

void CSpfcTW2022Util::SetParamFromCode(const T_SPFC_CODE& codeParam)
{
	m_nSpecType = codeParam.TAIWAN22.nSpectrumType;
	m_nSeisZone = codeParam.TAIWAN22.nSeismicZone;
	m_nSpecCategory = codeParam.TAIWAN22.nSpectrumUsed;
	m_dSds = codeParam.TAIWAN22.dSds;
	m_dSd1 = codeParam.TAIWAN22.dSd1;
	m_dSms = codeParam.TAIWAN22.dSms;
	m_dSm1 = codeParam.TAIWAN22.dSm1;
	m_dNda = codeParam.TAIWAN22.dNda;
	m_dNdv = codeParam.TAIWAN22.dNdv;
	m_dNma = codeParam.TAIWAN22.dNma;
	m_dNmv = codeParam.TAIWAN22.dNmv;

	m_nSoilType = codeParam.TAIWAN22.nSoilType;
	m_dFda = codeParam.TAIWAN22.dFda;
	m_dFdv = codeParam.TAIWAN22.dFdv;
	m_dFma = codeParam.TAIWAN22.dFma;
	m_dFmv = codeParam.TAIWAN22.dFmv;

	m_nSubZone = codeParam.TAIWAN22.nSubZone;
	m_dSds_t = codeParam.TAIWAN22.dSds_t;
	m_dSms_t = codeParam.TAIWAN22.dSms_t;
	m_dTd0_t = codeParam.TAIWAN22.dTd0_t;
	m_dTm0_t = codeParam.TAIWAN22.dTm0_t;

	m_dI = codeParam.TAIWAN22.dImportanceFactor;
	m_dAy = codeParam.TAIWAN22.dSeisMagnifyFactor;
	m_dR = codeParam.TAIWAN22.dResponseModifyFactor;
	m_dDamping = codeParam.TAIWAN22.dDampingRatio;
	m_dMaxPeriod = codeParam.TAIWAN22.dMaxPeriod;
	m_dFundPeriod = codeParam.TAIWAN22.dFundamentPeriod;
}

void CSpfcTW2022Util::SetParamFromCalc()
{
	CalcSiteMagnifyFactor();
}

void CSpfcTW2022Util::SetInitValueforPOSpectrum()
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
	m_dNda = 0.80;
	m_dNdv = 0.45;
	m_dNma = 1.00;
	m_dNmv = 0.55;

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

void CSpfcTW2022Util::CalcSiteMagnifyFactor()
{
	CDBDoc* pDoc = (CDBDoc*)CDBDoc::GetDocPoint();
	if (pDoc == nullptr) { ASSERT(0); return; }

	if (m_nSeisZone != enSeisZoneGEN && m_nSeisZone != enSeisZoneNFZ) return;

	int nCalcSeisZone = m_nSeisZone;
	if (m_nSeisZone == enSeisZoneNFZ && m_nSpecCategory == enSpectUsedMED)
	{
		nCalcSeisZone = enSeisZoneGEN;
	}

	double dFna = 0.0, dFnv = 0.0;
	if (!pDoc->m_pAttrCtrl->CalcTaiwan22SiteMagnifyFactor(TAIWAN_E_2022, nCalcSeisZone, m_nSoilType,
		m_dSds, m_dSd1, m_dSms, m_dSm1, m_dNda, m_dNdv, m_dNma, m_dNmv,
		m_dFda, m_dFdv, m_dFma, m_dFmv, dFna, dFnv))
		return;

	if (m_nSeisZone == enSeisZoneNFZ) // 내부 계산용
	{
		double dNda_1 = 1.0, dNdv_1 = 1.0;
		if (!pDoc->m_pAttrCtrl->CalcTaiwan22SiteMagnifyFactor(TAIWAN_E_2022, nCalcSeisZone, m_nSoilType,
			m_dSds, m_dSd1, m_dSms, m_dSm1, dNda_1, dNdv_1, m_dNma, m_dNmv,
			m_dFda_1, m_dFdv_1, m_dFma, m_dFmv, dFna, dFnv))
			return;
	}
}

bool CSpfcTW2022Util::IsFundamentalPeriodREQ()
{
	return true;
}

bool CSpfcTW2022Util::IsValidNearFaultFactor(CString& rcsMsg)
{
	if (m_nSeisZone != enSeisZoneNFZ) return true;

	if ((m_nSpecCategory != enSpectUsedMAX && m_dNda < 0.0 || m_dNdv < 0.0) ||
		(m_nSpecCategory == enSpectUsedMAX && m_dNma < 0.0 || m_dNmv < 0.0))
	{
		rcsMsg = _LS(IDS_WG_CMD__ADDD__Error__TAIWAN22_NFSE);
		return false;
	}
	return true;
}

CString CSpfcTW2022Util::GetFunctionDescription()
{
	auto L_GetSpectUsed = [](UINT nSpectUsed) -> CString
	{
		switch (nSpectUsed)
		{
		case enSpectUsedDGN: return _T("Design");
		case enSpectUsedMED: return _T("Small-Medium");
		case enSpectUsedMAX: return _T("Maximum");
		}
		ASSERT(0); return _T("-");
	};
	auto L_GetSeisZone = [](UINT nSeisZone) -> CString
	{
		switch (nSeisZone)
		{
		case enSeisZoneGEN: return _T("General");
		case enSeisZoneNFZ: return _T("NearFault");
		case enSeisZoneTPB: return _T("Taipei");
		}
		ASSERT(0); return _T("-");
	};
	auto L_GetSoilType = [](UINT nSoilType) -> CString
	{
		switch (nSoilType)
		{
		case enSoilTypeTYP1: return _T("SoilType1");
		case enSoilTypeTYP2: return _T("SoilType2");
		case enSoilTypeTYP3: return _T("SoilType3");
		case enSoilTypeUSER: return _T("SoilUser");
		}
		ASSERT(0); return _T("-");
	};

	CString csDesc;
	switch (m_nSeisZone)
	{
	case enSeisZoneGEN:
	{
		double dSs = m_dSds;
		double dS1 = m_dSd1;
		if (m_nSpecCategory == enSpectUsedMAX)
		{
			dSs = m_dSms;
			dS1 = m_dSm1;
		}
		csDesc.Format(_T("TAIWAN(2022) : %s, %s, %s, I=%4.2f, ay=%4.2f, R=%4.2f, Ss=%4.2f, S1=%4.2f, T1=%4.2fs"),
			L_GetSeisZone(m_nSeisZone),
			L_GetSpectUsed(m_nSpecCategory),
			L_GetSoilType(m_nSoilType),
			m_dI, m_dAy, m_dR, dSs, dS1, m_dFundPeriod);
	}
	break;
	case enSeisZoneNFZ:
	{
		double dSs = m_dSds;
		double dS1 = m_dSd1;
		switch (m_nSpecCategory)
		{
		case enSpectUsedMAX:
		{
			dSs = m_dNma;
			dS1 = m_dNmv;
		}
		break;
		case enSpectUsedDGN:
		{
			dSs = m_dNda;
			dS1 = m_dNdv;
		}
		break;
		}
		csDesc.Format(_T("TAIWAN(2022) : %s, %s, %s, I=%4.2f, ay=%4.2f, R=%4.2f, Ss=%4.2f, S1=%4.2f, T1=%4.2fs"),
			L_GetSeisZone(m_nSeisZone),
			L_GetSpectUsed(m_nSpecCategory),
			L_GetSoilType(m_nSoilType),
			m_dI, m_dAy, m_dR, dSs, dS1, m_dFundPeriod);
	}
	break;
	case enSeisZoneTPB:
	{
		csDesc.Format(_LS(IDS_CMD__ADD_TAIWAN2022_PROFILE2),
			GetSubZoneName(m_nSubZone),
			L_GetSpectUsed(m_nSpecCategory),
			m_dI, m_dAy, m_dR, m_dFundPeriod);
	}
	break;
	default: ASSERT(0); break;
	}

	return csDesc;
}

double CSpfcTW2022Util::CalcSaEveryZone(stETC_GDS_TAIWAN02_CALC& sCalc, double dTn, double dT1, double dSRa, double dSRv, BOOL bPOcurve)
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
		double dSaDT1 = GetSaFactor(sCalc, dT1, 0);
		double dSaMT1 = GetSaFactor(sCalc, dT1, 2);
		double dFuT1 = GetFuFactor(dT1, sCalc.dTd0, 0);
		double dFuMT1 = GetFuFactor(dT1, sCalc.dTm0, 2);
		double dSFDT1 = GetModifiedSaFu(dSaDT1, dFuT1);
		double dSFMT1 = GetModifiedSaFu(dSaMT1, dFuMT1);
		double dSFD1T1 = 0.0;
		if (m_nSeisZone == enSeisZoneNFZ)
		{
			dSFD1T1 = GetModifiedSaFu(dSaDT1, dFuT1);
		}

		dV0 = dSaD * m_dI / (1.4 * m_dAy * dSaDT1) * dSFDT1;
		switch (m_nSeisZone)
		{
		case enSeisZoneGEN: dV1 = dSaD * m_dI / (4.2 * m_dAy * dSaDT1) * dSFDT1 * dFuT1; break;
		case enSeisZoneNFZ: dV1 = dSaD * m_dI / (4.2 * m_dAy * dSaDT1) * dSFD1T1 * dFuT1; break;
		case enSeisZoneTPB: dV1 = dSaD * m_dI / (3.5 * m_dAy * dSaDT1) * dSFDT1 * dFuT1; break;
		default: ASSERT(0); break;
		}
		dV2 = dSaM * m_dI / (1.4 * m_dAy * dSaMT1) * dSFMT1;
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

bool CSpfcTW2022Util::CalcGeneralZone(stETC_GDS_TAIWAN02_CALC& sCalc)
{
	sCalc.dSDS = m_dSds * m_dFda;
	sCalc.dSD1 = m_dSd1 * m_dFdv;
	sCalc.dTd0 = sCalc.dSD1 / sCalc.dSDS * (sCalc.dBs / sCalc.dB1);

	sCalc.dSMS = m_dSms * m_dFma;
	sCalc.dSM1 = m_dSm1 * m_dFmv;
	sCalc.dTm0 = sCalc.dSM1 / sCalc.dSMS * (sCalc.dBs / sCalc.dB1);
	return true;
}

bool CSpfcTW2022Util::CalcNearFaultZone(stETC_GDS_TAIWAN02_CALC& sCalc)
{
	const double dSds = m_dNda;
	const double dSd1 = m_dNdv;
	const double dSms = m_dNma;
	const double dSm1 = m_dNmv;

	sCalc.dSDS = dSds * m_dFda;
	sCalc.dSD1 = dSd1 * m_dFdv;
	sCalc.dSDS_1 = m_dSds * m_dFda_1;
	sCalc.dSD1_1 = m_dSd1 * m_dFdv_1;
	sCalc.dTd0 = sCalc.dSD1 / sCalc.dSDS * (sCalc.dBs / sCalc.dB1);
	sCalc.dTd0_1 = sCalc.dSD1_1 / sCalc.dSDS_1 * (sCalc.dBs / sCalc.dB1);

	sCalc.dSMS = dSms * m_dFma;
	sCalc.dSM1 = dSm1 * m_dFmv;
	sCalc.dTm0 = sCalc.dSM1 / sCalc.dSMS * (sCalc.dBs / sCalc.dB1);
	return true;
}

bool CSpfcTW2022Util::CalcTaipeiBasin(stETC_GDS_TAIWAN02_CALC& sCalc)
{
	sCalc.dSDS = m_dSds_t;
	sCalc.dTd0 = m_dTd0_t * (sCalc.dBs / sCalc.dB1);

	sCalc.dSMS = m_dSms_t;
	sCalc.dTm0 = m_dTm0_t * (sCalc.dBs / sCalc.dB1);
	return true;
}
