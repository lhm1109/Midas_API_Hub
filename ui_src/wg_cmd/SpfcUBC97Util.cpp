#include "stdafx.h"
#include "SpfcUBC97Util.h"

CSpfcUBC97Util::CSpfcUBC97Util()
{
    m_nOption = 0;
    m_nSoilType = 0;
    m_nSeisZone = 0;
    m_nSourceType = 0;
    m_dClosestDist = 10.0;
    m_dIF = 1.0;
    m_dNC = 1.0;
    m_dMaxPeriod = PRD_ED;

	AutoCalcSeisCoef();
}

bool CSpfcUBC97Util::CheckValid()
{
	CString strMsg;

	if (m_dCa <= 0)
	{
		strMsg.Format(_LS(IDS_CMD_ERROR_NAME_POSITIVE), _T("Seismic Coefficient(Ca)"));
		AfxMessageBox(strMsg);
		return false;
	}
	if (m_dCv <= 0)
	{
		strMsg.Format(_LS(IDS_CMD_ERROR_NAME_POSITIVE), _T("Seismic Coefficient(Cv)"));
		AfxMessageBox(strMsg);
		return false;
	}

	if (m_dMaxPeriod <= 0) { AfxMessageBox(_LS(IDS_CMD_SPECTRUM__MAX_PERIOD_GREATER_0)); return false; }
	return true;
}

void CSpfcUBC97Util::MakeSpectrumData(bool bOnlyCalc /*= false*/)
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
	double Tn, T0, Ts, SA, Value, dt;
	int n = 0;

	double Ca = m_dCa;
	double Cv = m_dCv;
	m_dIF = 1.0;
	m_dNC = 1.0;

	Ts = Cv / 2.5 / Ca;
	T0 = 0.2 * Ts;

	dt = (ed - st) / Step;
	double dTol = dt / Step;  // Tolerance		

	InitPeriod();
	InitAccel();

	for (int q = 0; q <= Step; q++)
	{
		Tn = st + dt * q;

		if (Tn >= 0 && Tn <= T0) SA = Ca + 1.5 * Ca / T0 * Tn;
		if (Tn > T0 + dTol && Tn - dt < T0 - dTol)
		{
			Value = 2.5 * Ca * m_dIF / m_dNC;
			AddPeriod(T0);
			AddAccel(Value);

		}
		if (Tn > T0 && Tn <= Ts) SA = 2.5 * Ca;
		if (Tn > Ts + dTol && Tn - dt < Ts - dTol)
		{
			Value = 2.5 * Ca * m_dIF / m_dNC;
			AddPeriod(Ts);
			AddAccel(Value);

		}
		if (Tn > Ts) SA = Cv / Tn;

		Value = SA * m_dIF / m_dNC;
		AddPeriod(Tn);
		AddAccel(Value);
	}
	if (bOnlyCalc) return;

	wchar_t buffer[SpfcUtil::BUFFER_SIZE];
	int result = std::swprintf(buffer, SpfcUtil::BUFFER_SIZE, _T("UBC1997 : Ca = %g  Cv = %g"), Ca, Cv);
	if (result >= 0)
	{
		SetDescript(buffer);
	}
	SetFuncName(L"UBC1997");
}

void CSpfcUBC97Util::MakePOSpectrumData(double dDamping, double SRa, double SRv, CArray <double, double>& aPeriod, CArray <double, double>& aAccel)
{

	double st = PRD_ST;
	double ed = m_dMaxPeriod * 2;
	int Step = PRD_NUM * 4;

	double Tn, T0, Ts, SA, Value, dt;
	int n = 0;
	double Ca = m_dCa;
	double Cv = m_dCv;

	m_dIF = 1.0;
	m_dNC = 1.0;

	if (dDamping >= 0)
	{
		SRa = (3.21 - 0.68 * log(dDamping)) / 2.12;
		SRv = (2.31 - 0.41 * log(dDamping)) / 1.65;
	}

	dt = (ed - st) / Step;
	double dTol = dt / Step;  // Tolerance

	Ts = Cv / 2.5 / Ca * SRv / SRa;
	T0 = 0.2 * Ts;


	for (int q = 0; q <= Step; q++)
	{
		Tn = st + dt * q;

		if (Tn >= 0 && Tn <= T0) 			SA = Ca * (1 + Tn * (2.5 * SRa - 1.0) / T0);
		if (Tn > T0 + dTol && Tn - dt < T0 - dTol)
		{
			Value = 2.5 * Ca * m_dIF / m_dNC * SRa;
			aPeriod.Add(T0);
			aAccel.Add(Value);
		}
		if (Tn > T0 && Tn <= Ts)
			SA = 2.5 * Ca * SRa;
		if (Tn > Ts + dTol && Tn - dt < Ts - dTol)
		{
			Value = 2.5 * Ca * m_dIF / m_dNC * SRa;
			aPeriod.Add(Ts);
			aAccel.Add(Value);

		}
		if (Tn > Ts) SA = Cv / Tn * SRv;

		Value = SA * m_dIF / m_dNC;
		aPeriod.Add(Tn);
		aAccel.Add(Value);
	}

}

void CSpfcUBC97Util::SetParamFromCode(const T_SPFC_CODE& codeParam)
{
	m_nOption = codeParam.UBC97.nOption;
	m_nSoilType = codeParam.UBC97.nSoilType;
	m_nSeisZone = codeParam.UBC97.nSeisZone;
	m_nSourceType = codeParam.UBC97.nSourceType;
	m_dCa = codeParam.UBC97.dCa;
	m_dCv = codeParam.UBC97.dCv;
	m_dIF = codeParam.UBC97.dIe;
	m_dNC = codeParam.UBC97.dCoef;
	m_dMaxPeriod = codeParam.UBC97.dMaxPeriod;
	m_dClosestDist = codeParam.UBC97.dClosestDist;
}

void CSpfcUBC97Util::SetParamFromCalc()
{
	if (m_nOption == 0)
		AutoCalcSeisCoef();
}

void CSpfcUBC97Util::AutoCalcSeisCoef(bool isApiMode)
{
	int nSoilType = m_nSoilType;
	int nSeisZone = m_nSeisZone;
	int nSourceType = m_nSourceType;

	double Ca_Data[5][5] = { 0.06, 0.12, 0.16, 0.24, 0.32,
						  0.08, 0.15, 0.20, 0.30, 0.40,
						  0.09, 0.18, 0.24, 0.33, 0.40,
						  0.12, 0.22, 0.28, 0.36, 0.44,
						  0.19, 0.30, 0.34, 0.36, 0.44 };

	double Cv_Data[5][5] = { 0.06, 0.12, 0.16, 0.24, 0.32,
						  0.08, 0.15, 0.20, 0.30, 0.40,
						  0.13, 0.25, 0.32, 0.45, 0.56,
						  0.18, 0.32, 0.40, 0.54, 0.64,
						  0.26, 0.50, 0.64, 0.84, 0.96 };

	double Na_Data[3][3] = { 1.5, 1.2, 1.0,
						   1.3, 1.0, 1.0,
						   1.0, 1.0, 1.0 };

	double Nv_Data[3][4] = { 2.0, 1.6, 1.2, 1.0,
						   1.6, 1.2, 1.0, 1.0,
						   1.0, 1.0, 1.0, 1.0 };

	double Ca = Ca_Data[nSoilType][nSeisZone];
	double Cv = Cv_Data[nSoilType][nSeisZone];

	if (nSeisZone == 4)
	{
		double Na = 0.0, Nv = 0.0;
		double x1 = 0.0, x2 = 0.0;

		double dDist = m_dClosestDist;
		if (dDist < 0)
		{
			if (isApiMode == true)
			{
				CString strMsg;
				strMsg.Format(_LS(IDS_CMD_ERROR_NAME_NONE_NEGA), _T("Closest Distance to Known Seismic Source (km)"));
				if (dDist < 0) { AfxMessageBox(strMsg); return; }
			}

			return;
		}

		//Ca = Ca*Na 
		if (dDist <= 2.0) Na = Na_Data[nSourceType][0];
		else if (dDist >= 10.0) Na = Na_Data[nSourceType][2];
		else if (dDist > 2.0 && dDist <= 5.0)  // -> needs Linear interpolation
		{
			x1 = Na_Data[nSourceType][0];
			x2 = Na_Data[nSourceType][1];

			Na = x1 + (x2 - x1) * (dDist - 2.0) / (5.0 - 2.0);
		}
		else if (dDist > 5.0 && dDist <= 10.0)  // -> needs Linear interpolation
		{
			x1 = Na_Data[nSourceType][1];
			x2 = Na_Data[nSourceType][2];

			Na = x1 + (x2 - x1) * (dDist - 5.0) / (10.0 - 5.0);
		}

		Ca = Ca * Na;

		//Cv = Cv*Nv 
		if (dDist <= 2.0) Nv = Nv_Data[nSourceType][0];
		else if (dDist >= 15.0) Nv = Nv_Data[nSourceType][3];
		else if (dDist > 2.0 && dDist <= 5.0)  // -> needs Linear interpolation
		{
			x1 = Nv_Data[nSourceType][0];
			x2 = Nv_Data[nSourceType][1];

			Nv = x1 + (x2 - x1) * (dDist - 2.0) / (5.0 - 2.0);
		}
		else if (dDist > 5.0 && dDist <= 10.0)  // -> needs Linear interpolation
		{
			x1 = Nv_Data[nSourceType][1];
			x2 = Nv_Data[nSourceType][2];

			Nv = x1 + (x2 - x1) * (dDist - 5.0) / (10.0 - 5.0);
		}
		else if (dDist > 10.0 && dDist <= 15.0)  // -> needs Linear interpolation
		{
			x1 = Nv_Data[nSourceType][2];
			x2 = Nv_Data[nSourceType][3];

			Nv = x1 + (x2 - x1) * (dDist - 10.0) / (15.0 - 10.0);
		}

		Cv = Cv * Nv;
	}

	m_dCa = Ca;
	m_dCv = Cv;
}
