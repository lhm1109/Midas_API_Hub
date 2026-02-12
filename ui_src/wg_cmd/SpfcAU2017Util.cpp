#include "stdafx.h"
#include "SpfcAU2017Util.h"

CSpfcAU2017Util::CSpfcAU2017Util()
{
	m_nSoilClass = 0;
	m_dkp = 1.3;
	m_dZ = 0.08;
	m_dMu = 1.5;
	m_dMaxPeriod = 6.0;

	m_strSC = _T("");
}

bool CSpfcAU2017Util::CheckValid()
{
	if (m_dkp <= 0) { AfxMessageBox(_LS(IDS_CMD_SPECTRUM__FAILURE_AS510022017_KP)); return false; }
	if (m_dZ <= 0) { AfxMessageBox(_LS(IDS_CMD_SPECTRUM__FAILURE_AS510022017_Z));  return false; }
	if (m_dMu <= 0) { AfxMessageBox(_LS(IDS_CMD_SPECTRUM__FAILURE_AS510022017_Mu)); return false; }
	if (m_dMaxPeriod <= 0) { AfxMessageBox(_LS(IDS_CMD_SPECTRUM__MAX_PERIOD_GREATER_0));   return false; }

	return true;
}

void CSpfcAU2017Util::MakeSpectrumData(bool bOnlyCalc)
{
	double st = PRD_ST;
	double ed = m_dMaxPeriod;
	int Step = PRD_NUM;
	double Tn, dt;
	double dCdT;

	// Pushover FEMA440에서 계산할 때(Period 2배, Step 4배)
	if (bOnlyCalc)
	{
		ed *= GetPeriodCoeff();
		Step *= GetStepCoeff();
	}

	dt = (ed - st) / Step;  // time interval	
	double dTol = dt / Step;  // Tolerance

	double dRange1 = 0.1;
	double dRange2 = 1.5;

	InitPeriod();
	InitAccel();

	for (int i = 0; i <= Step; i++)
	{
		Tn = st + dt * i;
		dCdT = (m_dkp * m_dZ * GetChT(Tn)) / m_dMu;

		if (Tn > dRange1 + dTol && Tn < dRange1 + dt - dTol) // transition point		
		{
			dCdT = (m_dkp * m_dZ * GetChT(dRange1)) / m_dMu;

			AddPeriod(dRange1);
			AddAccel(dCdT);
		}
		if (Tn > dRange2 + dTol && Tn < dRange2 + dt - dTol) // transition point		
		{
			dCdT = (m_dkp * m_dZ * GetChT(dRange2)) / m_dMu;

			AddPeriod(dRange2);
			AddAccel(dCdT);
		}

		/*if (m_parPeriod) */ AddPeriod(Tn);
		/*if (m_parAccel) */  AddAccel(dCdT);
	}

	if (bOnlyCalc) return;

	wchar_t buffer[SpfcUtil::BUFFER_SIZE];
	int result = std::swprintf(buffer, SpfcUtil::BUFFER_SIZE, _LS(IDS_WG_CMD__ADD_AS_5100_2_2017_PROFILE), m_strSC, m_dkp, m_dZ, m_dMu);
	if (result >= 0)
	{
		SetDescript(buffer);
	}
	SetFuncName(L"AS 5100.2(2017)");
}

void CSpfcAU2017Util::MakePOSpectrumData(double dDamping, double SRa, double SRv, CArray<double, double>& aPeriod, CArray<double, double>& aAccel)
{
	aPeriod.RemoveAll();
	aAccel.RemoveAll();

	double st = PRD_ST;
	double ed = m_dMaxPeriod * 2;
	int Step = PRD_NUM * 4;
	double Tn, dt;
	double dCdT;
	double dTs, dT0;

	if (dDamping >= 0)
	{
		SRa = (3.21 - 0.68 * log(dDamping)) / 2.12;
		SRv = (2.31 - 0.41 * log(dDamping)) / 1.65;
	}

	dt = (ed - st) / Step;  // time interval	
	double dTol = dt / Step;  // Tolerance

	double dRange1 = 0.1;
	double dRange2 = 1.5;

	dTs = dRange2 * SRv / SRa;
	dT0 = 0.2 * dRange2;

	for (int i = 0; i <= Step; i++)
	{
		Tn = st + dt * i;
		dCdT = (m_dkp * m_dZ * GetChT(Tn)) / m_dMu;

		if (Tn > dT0 + dTol && Tn < dT0 + dt - dTol) // transition point		
		{
			dCdT = (m_dkp * m_dZ * GetChT(dT0)) / m_dMu;

			aPeriod.Add(dT0);
			aAccel.Add(dCdT);
		}
		if (Tn > dTs + dTol && Tn < dTs + dt - dTol) // transition point		
		{
			dCdT = (m_dkp * m_dZ * GetChT(dTs)) / m_dMu;

			aPeriod.Add(dTs);
			aAccel.Add(dCdT);
		}

		aPeriod.Add(Tn);
		aAccel.Add(dCdT);
	}
}

void CSpfcAU2017Util::SetParamFromCode(const T_SPFC_CODE& codeParam)
{
	m_nSoilClass = codeParam.AS510022017.nSoilClass;
	m_dkp = codeParam.AS510022017.dkp;
	m_dZ = codeParam.AS510022017.dZ;
	m_dMu = codeParam.AS510022017.dMu;
	m_dMaxPeriod = codeParam.AS510022017.dMaxPeriod;
}

void CSpfcAU2017Util::SetParamFromCalc()
{
	m_strSC = _T("");

	if (m_nSoilClass == 0) m_strSC = _LSX(Ae);
	else if (m_nSoilClass == 1) m_strSC = _LSX(Be);
	else if (m_nSoilClass == 2) m_strSC = _LSX(Ce);
	else if (m_nSoilClass == 3) m_strSC = _LSX(De);
	else if (m_nSoilClass == 4) m_strSC = _LSX(Ee);
	else { ASSERT(0); }
}

double CSpfcAU2017Util::GetChT(double dTn)
{
	double dChT = 0.0;

	double dRange1 = 0.1;
	double dRange2 = 1.5;

	if (dTn <= dRange1)
	{
		if (m_nSoilClass == 0) dChT = 0.8 + (15.5 * dTn);
		else if (m_nSoilClass == 1) dChT = 1.0 + (19.4 * dTn);
		else if (m_nSoilClass == 2) dChT = 1.3 + (23.8 * dTn);
		else if (m_nSoilClass == 3) dChT = 1.1 + (25.8 * dTn);
		else if (m_nSoilClass == 4) dChT = 1.1 + (25.8 * dTn);
		else                       ASSERT(0);
	}
	else if (dTn > dRange1 && dTn <= dRange2)
	{
		if (m_nSoilClass == 0)
		{
			dChT = 0.704 / dTn;
			if (dChT > 2.35) dChT = 2.35;
		}
		else if (m_nSoilClass == 1)
		{
			dChT = 0.88 / dTn;
			if (dChT > 2.94) dChT = 2.94;
		}
		else if (m_nSoilClass == 2)
		{
			dChT = 1.25 / dTn;
			if (dChT > 3.68) dChT = 3.68;
		}
		else if (m_nSoilClass == 3)
		{
			dChT = 1.98 / dTn;
			if (dChT > 3.68) dChT = 3.68;
		}
		else if (m_nSoilClass == 4)
		{
			dChT = 3.08 / dTn;
			if (dChT > 3.68) dChT = 3.68;
		}
		else
		{
			ASSERT(0);
		}
	}
	else
	{
		if (m_nSoilClass == 0) dChT = 1.056 / (dTn * dTn);
		else if (m_nSoilClass == 1) dChT = 1.32 / (dTn * dTn);
		else if (m_nSoilClass == 2) dChT = 1.874 / (dTn * dTn);
		else if (m_nSoilClass == 3) dChT = 2.97 / (dTn * dTn);
		else if (m_nSoilClass == 4) dChT = 4.62 / (dTn * dTn);
		else                	   ASSERT(0);
	}

	return dChT;
}