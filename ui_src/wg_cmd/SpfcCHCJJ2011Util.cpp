#include "stdafx.h"
#include "SpfcCHCJJ2011Util.h"

CSpfcCHCJJ2011Util::CSpfcCHCJJ2011Util()
{
	m_nSeisType = 0;
	m_nZoningMap = 0;
	m_nSiteType = 0;
	m_nSeismicIntensity = 0;
	m_nSeismicE = 0;

	m_bVertAccSpec = FALSE;
	m_bE2_005 = FALSE;

	SetOutputData();
}

bool CSpfcCHCJJ2011Util::CheckValid()
{
	if (IsValid(m_dDampingRatio, 0.) == false)
		return false;

	if (IsValid(m_dTg, 0.) == false)
		return false;

	if (IsValid(m_dEta1, 0.) == false)
		return false;

	if (IsValid(m_dEta2, 0.55) == false)
		return false;

	if (IsValid(m_dGamma, 0.) == false)
		return false;

	if (IsValid(m_dCi, 0.35) == false)
		return false;

	if (IsValid(m_dA, 0.) == false)
		return false;

	if (IsValid(m_dSMax, 0.) == false)
		return false;

	if (IsValid(m_dMaxPeriod, 0.) == false)
		return false;

	return TRUE;
}

void CSpfcCHCJJ2011Util::MakeSpectrumData(bool bOnlyCalc)
{
	double  st = PRD_ST;
	double  ed = m_dMaxPeriod;
	int     Step = PRD_NUM;
	double  Tn, dt;

	// Pushover FEMA440에서 계산할 때(Period 2배, Step 4배)
	if (bOnlyCalc)
	{
		ed *= GetPeriodCoeff();
		Step *= GetStepCoeff();
	}

	dt = (ed - st) / Step;      // time interval	
	double dTol = dt / Step;  // Tolerance

	double dS = 0.0;

	InitPeriod();
	InitAccel();

	//////////////////////////////////////////////////////////////////////////
	for (int i = 0; i <= Step; i++)
	{
		Tn = st + dt * i;
		if (Tn >= 0 && Tn < 0.1)
		{
			dS = (m_dEta2 - 0.45) * m_dSMax * Tn / 0.1 + 0.45 * m_dSMax;
		}
		else if (Tn >= 0.1 && Tn < m_dTg)
		{
			dS = m_dEta2 * m_dSMax;
		}
		else if (Tn >= m_dTg && Tn < 5 * m_dTg)
		{
			dS = m_dEta2 * m_dSMax * pow(m_dTg / Tn, m_dGamma);
		}
		else
		{
			dS = (m_dEta2 * pow(0.2, m_dGamma) - m_dEta1 * (Tn - 5 * m_dTg)) * m_dSMax;
		}

		CString strFuncName = (_LS(IDS_CMD_SPECTRUM_CJJ166_2011__FUNCNAME));

		if (m_bVertAccSpec)
		{
			dS = dS * 0.65;
			strFuncName = (_LS(IDS_CMD_SPECTRUM_CJJ166_2011__FUNCNAME_VERT));
		}
		SetFuncName(std::wstring(strFuncName));


		if (Tn > 0.1 + dTol && Tn < 0.1 + dt - dTol) // transition point		
		{
			AddPeriod(0.1);
			AddAccel(m_bVertAccSpec ? 0.65 * m_dEta2 * m_dSMax : m_dEta2 * m_dSMax);
		}
		if (Tn > m_dTg + dTol && Tn < m_dTg + dt - dTol) // transition point		
		{
			AddPeriod(m_dTg);
			AddAccel(m_bVertAccSpec ? 0.65 * m_dEta2 * m_dSMax : m_dEta2 * m_dSMax);
		}

		AddPeriod(Tn);
		AddAccel(dS);
	}
	if (bOnlyCalc) return;

	wchar_t buffer[SpfcUtil::BUFFER_SIZE];
	int result = std::swprintf(buffer, SpfcUtil::BUFFER_SIZE, _LS(IDS_CMD_SPECTRUM_CJJ166_2011__DESCRIPT), m_dDampingRatio, m_dTg, m_dEta1, m_dEta2, m_dGamma, m_dA, m_dSMax);
	if (result >= 0)
	{
		SetDescript(buffer);
	}
}

void CSpfcCHCJJ2011Util::MakePOSpectrumData(double dDamping, double SRa, double SRv, CArray<double, double>& aPeriod, CArray<double, double>& aAccel)
{
	/* nothing else this type */
	return;
}

void CSpfcCHCJJ2011Util::SetParamFromCode(const T_SPFC_CODE& codeParam)
{
	m_nSeisType = codeParam.CJJ166_2011.nSeisType;
	m_nZoningMap = codeParam.CJJ166_2011.nZoningMap;
	m_nSiteType = codeParam.CJJ166_2011.nSiteClass;
	m_nSeismicIntensity = codeParam.CJJ166_2011.nSeisFortification;
	m_nSeismicE = codeParam.CJJ166_2011.nSeisE;

	m_dDampingRatio = codeParam.CJJ166_2011.dDamping;
	m_bVertAccSpec = codeParam.CJJ166_2011.bVertAccSpec;
	m_bE2_005 = codeParam.CJJ166_2011.bE2_005;

	m_dTg = codeParam.CJJ166_2011.dTg;
	m_dEta1 = codeParam.CJJ166_2011.dEta1;
	m_dEta2 = codeParam.CJJ166_2011.dEta2;
	m_dGamma = codeParam.CJJ166_2011.dGamma;
	m_dCi = codeParam.CJJ166_2011.dCi;
	m_dA = codeParam.CJJ166_2011.dA;
	m_dSMax = codeParam.CJJ166_2011.dSMax;
	m_dMaxPeriod = codeParam.CJJ166_2011.dMaxPeriod;
}

void CSpfcCHCJJ2011Util::SetOutputData()
{
	m_dTg = CalcTgFromInputData();
	m_dEta1 = CalcEta1FromInputData();
	m_dEta2 = CalcEta2FromInputData();
	m_dGamma = CalcGammaFromInputData();
	m_dCi = CalcCiFromInputData();
	m_dA = CalcAFromOutputData(m_dCi);
	m_dSMax = CalcSMaxFromOutputData(m_dA);
}

double CSpfcCHCJJ2011Util::CalcTgFromInputData() const
{
	//Tg
	double dTg = 0.;
	if (m_nZoningMap == 0)
	{
		if (m_nSiteType == 0) dTg = 0.25;
		else if (m_nSiteType == 1) dTg = 0.35;
		else if (m_nSiteType == 2) dTg = 0.45;
		else                       dTg = 0.65;
	}
	else if (m_nZoningMap == 1)
	{
		if (m_nSiteType == 0) dTg = 0.30;
		else if (m_nSiteType == 1) dTg = 0.40;
		else if (m_nSiteType == 2) dTg = 0.55;
		else                       dTg = 0.75;
	}
	else
	{
		if (m_nSiteType == 0) dTg = 0.35;
		else if (m_nSiteType == 1) dTg = 0.45;
		else if (m_nSiteType == 2) dTg = 0.65;
		else                       dTg = 0.90;
	}
	if (m_bE2_005 && m_nSeismicE == 1) dTg += 0.05;

	return dTg;
}

double CSpfcCHCJJ2011Util::CalcEta1FromInputData() const
{
	//Eta1
	double dEta1 = 0.02;
	if (fabs(m_dDampingRatio - 0.05) > 1.0E-5)
	{
		dEta1 = 0.02 + (0.05 - m_dDampingRatio) / 8.0;
		if (dEta1 < 1.0E-5)  dEta1 = .0;
	}

	return dEta1;
}

double CSpfcCHCJJ2011Util::CalcEta2FromInputData() const
{
	//Eta2
	double dEta2 = 1.;
	if (fabs(m_dDampingRatio - 0.05) > 1.0E-5)
	{
		dEta2 = 1 + (0.05 - m_dDampingRatio) / (0.06 + 1.7 * m_dDampingRatio);
		if (dEta2 < 0.55) dEta2 = 0.55;
	}

	return dEta2;
}

double CSpfcCHCJJ2011Util::CalcGammaFromInputData() const
{
	//Gamma
	double dGamma = 0.9;
	if (fabs(m_dDampingRatio - 0.05) > 1.0E-5)
	{
		dGamma = 0.9 + (0.05 - m_dDampingRatio) / (0.5 + 5 * m_dDampingRatio);
	}

	return dGamma;
}

double CSpfcCHCJJ2011Util::CalcCiFromInputData() const
{
	//Ci Table3.2.2
	double dCi = 1;
	if (m_nSeismicE == 0)//E1
	{
		if (m_nSeisType == 0) dCi = 0.61;//乙
		else if (m_nSeisType == 1) dCi = 0.46;//丙
		else if (m_nSeisType == 2) dCi = 0.35;//丁
	}
	else//E2
	{
		if (m_nSeismicIntensity == 1) dCi = 2.20;//0.10
		else if (m_nSeismicIntensity == 2) dCi = 2.05;//0.15
		else if (m_nSeismicIntensity == 3) dCi = 2.00;//0.20
		else if (m_nSeismicIntensity == 4) dCi = 1.70;//0.30
		else if (m_nSeismicIntensity == 5) dCi = 1.55;//0.40

		if (m_nSeisType == 2) dCi = 1.0;//丁
	}

	return dCi;
}

double CSpfcCHCJJ2011Util::CalcAFromOutputData(const double dCi) const
{
	//A
	double dA = 0.;
	if (m_nSeismicIntensity == 0) dA = 0.05 * dCi;//0.05
	else if (m_nSeismicIntensity == 1) dA = 0.10 * dCi;//0.10
	else if (m_nSeismicIntensity == 2) dA = 0.15 * dCi;//0.15
	else if (m_nSeismicIntensity == 3) dA = 0.20 * dCi;//0.20
	else if (m_nSeismicIntensity == 4) dA = 0.30 * dCi;//0.30
	else if (m_nSeismicIntensity == 5) dA = 0.40 * dCi;//0.40

	return dA;
}

double CSpfcCHCJJ2011Util::CalcSMaxFromOutputData(const double dA) const
{
	return 2.25 * dA;
}

