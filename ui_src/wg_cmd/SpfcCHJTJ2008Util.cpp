#include "stdafx.h"
#include "SpfcCHJTJ2008Util.h"

CSpfcCHJTJ2008Util::CSpfcCHJTJ2008Util()
{
	m_nBridgeType = 0;
	m_nZoningMap = 0;
	m_nSiteType = 0;
	m_nSeismicIntensity = 0;
	m_nSeismicCriterion = 0;

	m_bLargeBridge = FALSE;
	m_bVertAccSpec = FALSE;
	m_nVertAccSpecType = 0;

	SetOutputData();
	m_dMaxPeriod = PRD_ED;
}

bool CSpfcCHJTJ2008Util::CheckValid()
{
	if (IsValid(m_dDampingRatio, 0.) == false)
		return false;

	if (IsValid(m_dTg, 0.) == false)
		return false;

	if (IsValid(m_dCi, 0.) == false)
		return false;

	if (IsValid(m_dCs, 0.) == false)
		return false;

	if (IsValid(m_dCd, 0.) == false)
		return false;

	if (IsValid(m_dEPA, 0.) == false)
		return false;

	if (IsValid(m_dSMax, 0.) == false)
		return false;

	if (IsValid(m_dMaxPeriod, 0.) == false)
		return false;

	return true;
}

void CSpfcCHJTJ2008Util::MakeSpectrumData(bool bOnlyCalc)
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

	double Alpha = m_dSMax;
	double Beta = 0.0;

	InitPeriod();
	InitAccel();

	if (m_bVertAccSpec) // 수직
	{
		if (m_nVertAccSpecType == 0) // 염층
		{
			for (int i = 0; i <= Step; i++)
			{
				Tn = st + dt * i;
				if (Tn >= 0 && Tn < 0.1)
				{
					Beta = ((5.5 * Tn) + 0.45) * 0.65;
				}
				else if (Tn >= 0.1 && Tn < m_dTg)
				{
					Beta = 1 * 0.65;
				}
				else
				{
					Beta = (m_dTg / Tn) * 0.65;
				}

				if (Tn > 0.1 + dTol && Tn < 0.1 + dt - dTol) // transition point		
				{
					AddPeriod(0.1);
					AddAccel(Alpha * 0.65);
				}
				if (Tn > m_dTg + dTol && Tn < m_dTg + dt - dTol) // transition point		
				{
					AddPeriod(m_dTg);
					AddAccel(Alpha * 0.65);
				}
				AddPeriod(Tn);
				AddAccel(Alpha * Beta);
			}
		}
		else if (m_nVertAccSpecType == 1) // 토층
		{
			if (m_dTg < 0.3)
			{
				for (int i = 0; i <= Step; i++)
				{
					Tn = st + dt * i;
					if (Tn >= 0 && Tn < 0.1)
					{
						Beta = ((5.5 * Tn) + 0.45) * 1;
					}
					else if (Tn >= 0.1 && Tn < m_dTg)
					{
						Beta = 1 * (1.0 - 2.5 * (Tn - 0.1));
					}
					else if (Tn >= m_dTg && Tn < 0.3)
					{
						Beta = (m_dTg / Tn) * (1.0 - 2.5 * (Tn - 0.1));
					}
					else
					{
						Beta = (m_dTg / Tn) * 0.5;
					}

					if (Tn > 0.1 + dTol && Tn < 0.1 + dt - dTol) // transition point		
					{
						AddPeriod(0.1);
						AddAccel(Alpha);
					}
					if (Tn > m_dTg + dTol && Tn < m_dTg + dt - dTol) // transition point		
					{
						AddPeriod(m_dTg);
						AddAccel(Alpha * (1.0 - 2.5 * (Tn - 0.1)));
					}
					if (Tn > 0.3 + dTol && Tn < 0.3 + dt - dTol) // transition point		
					{
						AddPeriod(0.3);
						AddAccel(Alpha * (1.0 - 2.5 * (Tn - 0.1)));
					}

					AddPeriod(Tn);
					AddAccel(Alpha * Beta);
				}
			}
			else
			{
				for (int i = 0; i <= Step; i++)
				{
					Tn = st + dt * i;
					if (Tn >= 0 && Tn < 0.1)
					{
						Beta = ((5.5 * Tn) + 0.45) * 1;
					}
					else if (Tn >= 0.1 && Tn < 0.3)
					{
						Beta = 1 * (1.0 - 2.5 * (Tn - 0.1));
					}
					else if (Tn >= 0.3 && Tn < m_dTg)
					{
						Beta = 1 * 0.5;
					}
					else
					{
						Beta = (m_dTg / Tn) * 0.5;
					}

					if (Tn > 0.1 + dTol && Tn < 0.1 + dt - dTol) // transition point		
					{
						AddPeriod(0.1);
						AddAccel(Alpha);
					}
					if (Tn > 0.3 + dTol && Tn < 0.3 + dt - dTol) // transition point		
					{
						AddPeriod(0.1);
						AddAccel(Alpha * (1.0 - 2.5 * (Tn - 0.1)));
					}
					if (Tn > m_dTg + dTol && Tn < m_dTg + dt - dTol) // transition point		
					{
						AddPeriod(m_dTg);
						AddAccel(Alpha * 0.5);
					}
					AddPeriod(Tn);
					AddAccel(Alpha * Beta);
				}
			}
		}
		else
		{
			ASSERT(0);
		}
		CString funcName = _LS(IDS_CMD_SPECTRUM_JTJXXX_2008__FUNCNAME_VERT);
		SetFuncName(std::wstring(funcName));
	}
	else // 수평
	{
		for (int i = 0; i <= Step; i++)
		{
			Tn = st + dt * i;
			if (Tn >= 0 && Tn < 0.1)
			{
				Beta = ((5.5 * Tn) + 0.45);
			}
			else if (Tn >= 0.1 && Tn < m_dTg)
			{
				Beta = 1;
			}
			else
			{
				Beta = (m_dTg / Tn);
			}

			if (Tn > 0.1 + dTol && Tn < 0.1 + dt - dTol) // transition point		
			{
				AddPeriod(0.1);
				AddAccel(Alpha);
			}
			if (Tn > m_dTg + dTol && Tn < m_dTg + dt - dTol) // transition point		
			{
				AddPeriod(m_dTg);
				AddAccel(Alpha);
			}
			AddPeriod(Tn);
			AddAccel(Alpha * Beta);
		}

		CString funcName = (_LS(IDS_CMD_SPECTRUM_JTJXXX_2008__FUNCNAME));
		SetFuncName(std::wstring(funcName));
	}

	if (bOnlyCalc) return;

	wchar_t buffer[SpfcUtil::BUFFER_SIZE];
	int result = std::swprintf(buffer, SpfcUtil::BUFFER_SIZE, _LS(IDS_CMD_SPECTRUM_JTJXXX_2008__DESCRIPT), m_dDampingRatio, m_dTg, m_dCi, m_dCs, m_dCs, m_dEPA, m_dSMax);
	if (result >= 0)
	{
		SetDescript(buffer);
	}
}

void CSpfcCHJTJ2008Util::MakePOSpectrumData(double dDamping, double SRa, double SRv, CArray <double, double>& aPeriod, CArray <double, double>& aAccel)
{
	// 현재 토목 기술팀에서 PushOver 에 대한 답을 듣지 못한 상황이므로 요렇게 처리...으흐흐흐~~~
	aPeriod.RemoveAll();
	aAccel.RemoveAll();

	double  st = PRD_ST;
	double  ed = m_dMaxPeriod * 2;
	int     Step = PRD_NUM * 4;
	double  Tn, dt;

	dt = (ed - st) / Step;      // time interval	
	double dTol = dt / Step;  // Tolerance

	double Alpha = m_dSMax;
	double Beta = 0.0;

	if (m_bVertAccSpec) // 수직
	{
		if (m_nVertAccSpecType == 0) // 염층
		{
			for (int i = 0; i <= Step; i++)
			{
				Tn = st + dt * i;
				if (Tn >= 0 && Tn < 0.1)
				{
					Beta = ((5.5 * Tn) + 0.45) * 0.65;
				}
				else if (Tn >= 0.1 && Tn < m_dTg)
				{
					Beta = 1 * 0.65;
				}
				else
				{
					Beta = (m_dTg / Tn) * 0.65;
				}

				if (Tn > 0.1 + dTol && Tn < 0.1 + dt - dTol) // transition point		
				{
					aPeriod.Add(0.1);
					aAccel.Add(Alpha * 0.65);
				}
				if (Tn > m_dTg + dTol && Tn < m_dTg + dt - dTol) // transition point		
				{
					aPeriod.Add(m_dTg);
					aAccel.Add(Alpha * 0.65);
				}
				aPeriod.Add(Tn);
				aAccel.Add(Alpha * Beta);
			}
		}
		else if (m_nVertAccSpecType == 1) // 토층
		{
			if (m_dTg < 0.3)
			{
				for (int i = 0; i <= Step; i++)
				{
					Tn = st + dt * i;
					if (Tn >= 0 && Tn < 0.1)
					{
						Beta = ((5.5 * Tn) + 0.45) * 1;
					}
					else if (Tn >= 0.1 && Tn < m_dTg)
					{
						Beta = 1 * (1.0 - 2.5 * (Tn - 0.1));
					}
					else if (Tn >= m_dTg && Tn < 0.3)
					{
						Beta = (m_dTg / Tn) * (1.0 - 2.5 * (Tn - 0.1));
					}
					else
					{
						Beta = (m_dTg / Tn) * 0.5;
					}

					if (Tn > 0.1 + dTol && Tn < 0.1 + dt - dTol) // transition point		
					{
						aPeriod.Add(0.1);
						aAccel.Add(Alpha);
					}
					if (Tn > m_dTg + dTol && Tn < m_dTg + dt - dTol) // transition point		
					{
						aPeriod.Add(m_dTg);
						aAccel.Add(Alpha * (1.0 - 2.5 * (Tn - 0.1)));
					}
					if (Tn > 0.3 + dTol && Tn < 0.3 + dt - dTol) // transition point		
					{
						aPeriod.Add(0.3);
						aAccel.Add(Alpha * (1.0 - 2.5 * (Tn - 0.1)));
					}

					aPeriod.Add(Tn);
					aAccel.Add(Alpha * Beta);
				}
			}
			else
			{
				for (int i = 0; i <= Step; i++)
				{
					Tn = st + dt * i;
					if (Tn >= 0 && Tn < 0.1)
					{
						Beta = ((5.5 * Tn) + 0.45) * 1;
					}
					else if (Tn >= 0.1 && Tn < 0.3)
					{
						Beta = 1 * (1.0 - 2.5 * (Tn - 0.1));
					}
					else if (Tn >= 0.3 && Tn < m_dTg)
					{
						Beta = 1 * 0.5;
					}
					else
					{
						Beta = (m_dTg / Tn) * 0.5;
					}

					if (Tn > 0.1 + dTol && Tn < 0.1 + dt - dTol) // transition point		
					{
						aPeriod.Add(0.1);
						aAccel.Add(Alpha);
					}
					if (Tn > 0.3 + dTol && Tn < 0.3 + dt - dTol) // transition point		
					{
						aPeriod.Add(0.1);
						aAccel.Add(Alpha * (1.0 - 2.5 * (Tn - 0.1)));
					}
					if (Tn > m_dTg + dTol && Tn < m_dTg + dt - dTol) // transition point		
					{
						aPeriod.Add(m_dTg);
						aAccel.Add(Alpha * 0.5);
					}
					aPeriod.Add(Tn);
					aAccel.Add(Alpha * Beta);
				}
			}
		}
		else
		{
			ASSERT(0);
		}

	}
	else // 수평
	{
		for (int i = 0; i <= Step; i++)
		{
			Tn = st + dt * i;
			if (Tn >= 0 && Tn < 0.1)
			{
				Beta = ((5.5 * Tn) + 0.45);
			}
			else if (Tn >= 0.1 && Tn < m_dTg)
			{
				Beta = 1;
			}
			else
			{
				Beta = (m_dTg / Tn);
			}

			if (Tn > 0.1 + dTol && Tn < 0.1 + dt - dTol) // transition point		
			{
				aPeriod.Add(0.1);
				aAccel.Add(Alpha);
			}
			if (Tn > m_dTg + dTol && Tn < m_dTg + dt - dTol) // transition point		
			{
				aPeriod.Add(m_dTg);
				aAccel.Add(Alpha);
			}
			aPeriod.Add(Tn);
			aAccel.Add(Alpha * Beta);
		}

	}
}

void CSpfcCHJTJ2008Util::SetParamFromCode(const T_SPFC_CODE& codeParam)
{
	m_nBridgeType = codeParam.JTJXXX_2008.nBridgeType;
	m_nZoningMap = codeParam.JTJXXX_2008.nZoningMap;
	m_nSiteType = codeParam.JTJXXX_2008.nSiteType;
	m_nSeismicIntensity = codeParam.JTJXXX_2008.nSeisminIntensity;
	m_nSeismicCriterion = codeParam.JTJXXX_2008.nSeisminCriterion;

	m_dDampingRatio = codeParam.JTJXXX_2008.dDampingRatio;	//(%df, 2)

	m_bLargeBridge = codeParam.JTJXXX_2008.bLargeBridge;
	m_bVertAccSpec = codeParam.JTJXXX_2008.bVertAccSpec;
	m_nVertAccSpecType = codeParam.JTJXXX_2008.nVertAccSpecType;

	//(%df, 2)
	m_dTg = codeParam.JTJXXX_2008.dTg;
	m_dCi = codeParam.JTJXXX_2008.dCi;
	m_dCs = codeParam.JTJXXX_2008.dCs;
	m_dCd = codeParam.JTJXXX_2008.dCd;
	m_dEPA = codeParam.JTJXXX_2008.dEPA;
	m_dSMax = codeParam.JTJXXX_2008.dSMax;
	m_dMaxPeriod = codeParam.JTJXXX_2008.dMaxPeriod;
}

void CSpfcCHJTJ2008Util::SetOutputData()
{
	m_dTg = CalcTgFromInputData();
	m_dCi = CalcCiFromInputData();
	m_dCs = CalcCsFromInputData();
	m_dCd = CalcCdFromInputData();
	m_dEPA = CalcEPAFromInputData();
	m_dSMax = CalcSMaxFromOutputData(m_dCi, m_dCs, m_dCd, m_dEPA);
}

double CSpfcCHJTJ2008Util::CalcTgFromInputData() const
{
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

	return dTg;
}

double CSpfcCHJTJ2008Util::CalcCiFromInputData() const
{
	double dCi = 0.;
	if (m_nBridgeType == 0)
	{
		if (m_nSeismicCriterion == 0) dCi = 1.00;
		else dCi = 1.70;
	}
	else if (m_nBridgeType == 1)
	{
		if (m_nSeismicCriterion == 0)
		{
			if (m_bLargeBridge) dCi = 0.50;
			else dCi = 0.43;
		}
		else
		{
			if (m_bLargeBridge) dCi = 1.70;
			else dCi = 1.30;
		}
	}
	else if (m_nBridgeType == 2)
	{
		if (m_nSeismicCriterion == 0) dCi = 0.34;
		else dCi = 1.00;
	}
	else
	{
		if (m_nSeismicCriterion == 0) dCi = 0.23;
		else dCi = 0.00;
	}

	return dCi;
}

double CSpfcCHJTJ2008Util::CalcCsFromInputData() const
{
	double dCs = 0.;
	if (m_nSiteType == 0)
	{
		if (m_nSeismicIntensity == 0) dCs = 1.20;
		else if (m_nSeismicIntensity == 1) dCs = 1.00;
		else if (m_nSeismicIntensity == 2) dCs = 0.90;
		else if (m_nSeismicIntensity == 3) dCs = 0.90;
		else if (m_nSeismicIntensity == 4) dCs = 0.90;
		else                               dCs = 0.90;
	}
	else if (m_nSiteType == 1)
	{
		if (m_nSeismicIntensity == 0) dCs = 1.00;
		else if (m_nSeismicIntensity == 1) dCs = 1.00;
		else if (m_nSeismicIntensity == 2) dCs = 1.00;
		else if (m_nSeismicIntensity == 3) dCs = 1.00;
		else if (m_nSeismicIntensity == 4) dCs = 1.00;
		else                               dCs = 1.00;
	}
	else if (m_nSiteType == 2)
	{
		if (m_nSeismicIntensity == 0) dCs = 1.10;
		else if (m_nSeismicIntensity == 1) dCs = 1.30;
		else if (m_nSeismicIntensity == 2) dCs = 1.20;
		else if (m_nSeismicIntensity == 3) dCs = 1.20;
		else if (m_nSeismicIntensity == 4) dCs = 1.00;
		else                               dCs = 1.00;
	}
	else
	{
		if (m_nSeismicIntensity == 0) dCs = 1.20;
		else if (m_nSeismicIntensity == 1) dCs = 1.40;
		else if (m_nSeismicIntensity == 2) dCs = 1.30;
		else if (m_nSeismicIntensity == 3) dCs = 1.30;
		else if (m_nSeismicIntensity == 4) dCs = 1.00;
		else                               dCs = 0.90;
	}
	return dCs;
}

double CSpfcCHJTJ2008Util::CalcCdFromInputData() const
{
	double dCd = 0.;

	dCd = 1 + ((0.05 - m_dDampingRatio) / (0.06 + (1.7 * m_dDampingRatio)));
	if (dCd < 0.55) dCd = 0.55;

	return dCd;
}

double CSpfcCHJTJ2008Util::CalcEPAFromInputData() const
{
	double dEPA = 0.;
	if (m_nSeismicIntensity == 0) dEPA = 0.05;
	else if (m_nSeismicIntensity == 1) dEPA = 0.10;
	else if (m_nSeismicIntensity == 2) dEPA = 0.15;
	else if (m_nSeismicIntensity == 3) dEPA = 0.20;
	else if (m_nSeismicIntensity == 4) dEPA = 0.30;
	else                               dEPA = 0.40;

	return dEPA;
}

double CSpfcCHJTJ2008Util::CalcSMaxFromOutputData(const double dCi, const double dCs, const double dCd, const double dEPA) const
{
	double dSMax = 0.;
	if (m_nBridgeType == 3 && m_nSeismicCriterion == 1)
	{
		dSMax = 2.25 * dCs * dCd * dEPA;
	}
	else
	{
		dSMax = 2.25 * dCi * dCs * dCd * dEPA;
	}

	return dSMax;
}