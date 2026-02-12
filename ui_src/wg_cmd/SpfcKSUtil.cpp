#include "stdafx.h"
#include "SpfcKSUtil.h"

CSpfcKSUtil::CSpfcKSUtil()
{
	m_dRMC = 1.0;
	m_dOIF = 1.0;
	m_dEPA = 0.08;
	m_dSoil = 1.0;
	m_dMaxPeriod = PRD_ED;
}

bool CSpfcKSUtil::CheckValid()
{
	if (IsValid(m_dOIF, 0, 10) == false)
	{
		return false;
	}

	if (IsValid(m_dRMC, 0, 50) == false)
	{
		return false;
	}

	if (m_dMaxPeriod <= 0)
	{
		AfxMessageBox(_LS(IDS_CMD_SPECTRUM__MAX_PERIOD_GREATER_0));
		return false;
	}

	return true;
}

void CSpfcKSUtil::MakeSpectrumData(bool bOnlyCalc/*=FALSE*/)
{
	double st = PRD_ST;
	double ed = m_dMaxPeriod;
	int Step = PRD_NUM;
	// Pushover FEMA440에서 계산할 때(Period 2배, Step 4배)
	if (bOnlyCalc)
	{
		ed *= GetPeriodCoeff();
		Step *= GetStepCoeff();
	}
	double Tn;
	int n = 0;
	double C, CS; // Modal Seismic Design Coefficient
	double critical;


	double dt = (ed - st) / Step;
	double dTol = dt / Step;  // Tolerance

	n = 0;

	InitPeriod();
	InitAccel();

	for (int q = 0; q <= Step; q++)
	{
		Tn = st + dt * q;

		//		if ( Tn <= 1.0E-4 ) Tn=1.0E-4;

		if (Tn <= 0.) C = 1.5;
		else C = 1 / (1.2 * sqrt(Tn));

		if (C > 1.5) C = 1.5;

		CS = C * m_dSoil;
		if (CS > 1.75) CS = 1.75;

		if (m_dSoil == 1.0)
		{
			critical = 0.30864;
			if (Tn > critical + dTol && Tn - dt < critical - dTol)		// transition point
			{
				AddPeriod(critical);
				AddAccel(1.5 * m_dSoil * m_dEPA * m_dOIF / m_dRMC);
			}
		}
		else
		{
			critical = (m_dSoil / 1.75 / 1.2) * (m_dSoil / 1.75 / 1.2);

			if (Tn > critical + dTol && Tn - dt < critical - dTol)		// transition point
			{
				AddPeriod(critical);
				AddAccel(1.75 * m_dEPA * m_dOIF / m_dRMC);
			}
		}

		AddPeriod(Tn);
		AddAccel(CS * m_dEPA * m_dOIF / m_dRMC);
	}

	if (bOnlyCalc) return;

	wchar_t buffer[SpfcUtil::BUFFER_SIZE];
	int result = std::swprintf(buffer, SpfcUtil::BUFFER_SIZE, _LS(IDS_WG_CMD__ADD2__KS___Soil____4_2f__EPA____5_3f__I), m_dSoil, m_dEPA, m_dOIF, m_dRMC);
	if (result >= 0)
	{
		SetDescript(buffer);
	}
	SetFuncName(L"KS1992");

	//// GSaveHistoryFormat(_T("KS : Soil = %3.1f  EPA = %4.2f  I = %4.2f  R = %4.2f"),m_dSoil,m_dEPA,m_dOIF,m_dRMC);
}

void CSpfcKSUtil::MakePOSpectrumData(double dDamping, double SRa, double SRv, CArray <double, double>& aPeriod, CArray <double, double>& aAccel)// for Pushover Curve
{

	double st = PRD_ST;
	double ed = m_dMaxPeriod * 2;
	int Step = PRD_NUM * 4;

	if (dDamping >= 0)
	{
		SRa = (3.21 - 0.68 * log(dDamping)) / 2.12;
		SRv = (2.31 - 0.41 * log(dDamping)) / 1.65;
	}

	double Tn;
	int n = 0;
	double C, CS; // Modal Seismic Design Coefficient
	double critical;


	double dt = (ed - st) / Step;
	double dTol = dt / Step;  // Tolerance

	n = 0;
	for (int q = 0; q <= Step; q++)
	{
		Tn = st + dt * q;

		//		if ( Tn <= 1.0E-4 ) Tn=1.0E-4;

		if (Tn <= 0.) C = 1.5 * SRa;
		else C = 1 / (1.2 * sqrt(Tn)) * SRv;
		if (C > 1.5 * SRa) C = 1.5 * SRa;

		CS = C * m_dSoil;
		if (CS > 1.75 * SRa) CS = 1.75 * SRa;

		if (m_dSoil == 1.0)
		{
			critical = (1.0 * SRv / (1.5 * SRa * 1.2)) * (1.0 * SRv / (1.5 * SRa * 1.2));
			if (Tn > critical + dTol && Tn < critical + dt - dTol)		// transition point
			{
				aPeriod.Add(critical);
				aAccel.Add(1.5 * SRa * m_dSoil * m_dEPA * m_dOIF / m_dRMC);
			}
		}
		else
		{
			critical = (m_dSoil * SRv / (1.75 * SRa * 1.2)) * (m_dSoil * SRv / (1.75 * SRa * 1.2));

			if (Tn > critical + dTol && Tn < critical + dt - dTol)		// transition point
			{
				aPeriod.Add(critical);
				aAccel.Add(1.75 * SRa * m_dEPA * m_dOIF / m_dRMC);
			}
		}

		aPeriod.Add(Tn);
		aAccel.Add(CS * m_dEPA * m_dOIF / m_dRMC);
	}
}

void CSpfcKSUtil::SetParamFromCode(const T_SPFC_CODE& codeParam)
{
	m_dRMC = codeParam.KS.dCoef;
	m_dOIF = codeParam.KS.dIe;
	m_dEPA = codeParam.KS.dEPA;
	m_dSoil = codeParam.KS.dSoil;
	m_dMaxPeriod = codeParam.KS.dMaxPeriod;
}

double CSpfcKSUtil::GetRMC() const
{
	return m_dRMC;
}

double CSpfcKSUtil::GetOIF() const
{
	return m_dOIF;
}

double CSpfcKSUtil::GetEPA() const
{
	return m_dEPA;
}

double CSpfcKSUtil::GetSoil() const
{
	return m_dSoil;
}

double CSpfcKSUtil::GetMaxPeriod() const
{
	return m_dMaxPeriod;
}

void CSpfcKSUtil::SetRMC(double rmc)
{
	m_dRMC = rmc;
}

void CSpfcKSUtil::SetOIF(double oif)
{
	m_dOIF = oif;
}

void CSpfcKSUtil::SetEPA(double epa)
{
	m_dEPA = epa;
}

void CSpfcKSUtil::SetSoil(double soil)
{
	m_dSoil = soil;
}

void CSpfcKSUtil::SetMaxPeriod(double maxPeriod)
{
	m_dMaxPeriod = maxPeriod;
}
 