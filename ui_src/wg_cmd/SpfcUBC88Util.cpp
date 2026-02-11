#include "stdafx.h"
#include "SpfcUBC88Util.h"

CSpfcUBC88Util::CSpfcUBC88Util()
{
    m_dSoil = 0;
    m_dSZF = 0;
    m_dOIF = 1.0;
    m_dRw = 1.0;
    m_dMaxPeriod = PRD_ED;
}

bool CSpfcUBC88Util::CheckValid()
{
	if (IsValid(m_dOIF, 0, 10) == false)
	{
		return false;
	}

	if (IsValid(m_dRw, 0, 50) == false)
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

void CSpfcUBC88Util::MakeSpectrumData(bool bOnlyCalc /*= false*/)
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

	double Tn, SA, Value, dt;
	int n = 0;

	m_dOIF = 1.0;
	m_dRw = 1.0;

	dt = (ed - st) / Step;
	double dTol = dt / Step;  // Tolerance

	InitPeriod();
	InitAccel();

	for (int q = 0; q <= Step; q++)
	{
		Tn = st + dt * q;

		//		if ( Tn <= 1.0E-6 ) Tn=1.0E-6f;

		if (m_dSoil == 1.0)
		{
			if (Tn >= 0 && Tn <= 0.15) SA = 1.0 + 10. * Tn;
			if (Tn > 0.15 + dTol && Tn - dt < 0.15 - dTol)
			{
				Value = 2.5 * m_dSZF * m_dOIF / m_dRw;
				AddPeriod(0.15);
				AddAccel(Value);
			}
			if (Tn > 0.15 && Tn <= 0.39) SA = 2.5;
			if (Tn > 0.39 + dTol && Tn - dt < 0.39 - dTol)
			{
				Value = 2.5 * m_dSZF * m_dOIF / m_dRw;
				AddPeriod(0.39);
				AddAccel(Value);

			}
			if (Tn > 0.39) SA = 0.975 / Tn;
		}
		else if (m_dSoil == 1.2)
		{
			if (Tn >= 0 && Tn <= 0.15) SA = 1.0 + 10. * Tn;
			if (Tn > 0.15 + dTol && Tn - dt < 0.15 - dTol)
			{
				Value = 2.5 * m_dSZF * m_dOIF / m_dRw;
				AddPeriod(0.15);
				AddAccel(Value);

			}
			if (Tn > 0.15 && Tn <= 0.585) SA = 2.5;
			if (Tn > 0.585 + dTol && Tn - dt < 0.585 - dTol)
			{
				Value = 2.5 * m_dSZF * m_dOIF / m_dRw;
				AddPeriod(0.585);
				AddAccel(Value);

			}
			if (Tn > 0.585) SA = 1.463 / Tn;
		}
		else if (m_dSoil == 1.5)
		{
			if (Tn >= 0 && Tn <= 0.2) SA = 1.0 + 7.5 * Tn;
			if (Tn > 0.2 + dTol && Tn - dt < 0.2 - dTol)
			{
				Value = 2.5 * m_dSZF * m_dOIF / m_dRw;
				AddPeriod(0.2);
				AddAccel(Value);

			}
			if (Tn > 0.2 && Tn <= 0.915) SA = 2.5;
			if (Tn > 0.915 + dTol && Tn - dt < 0.915 - dTol)
			{
				Value = 2.5 * m_dSZF * m_dOIF / m_dRw;
				AddPeriod(0.915);
				AddAccel(Value);

			}
			if (Tn > 0.915) SA = 2.288 / Tn;
		}
		Value = SA * m_dSZF * m_dOIF / m_dRw;
		AddPeriod(Tn);
		AddAccel(Value);
	}

	if (bOnlyCalc) return;
	wchar_t buffer[SpfcUtil::BUFFER_SIZE];

	int result = std::swprintf(buffer, SpfcUtil::BUFFER_SIZE, _LS(IDS_WG_CMD__ADD2__UBC_88___Soil____4_2f__Z____5_3f_), m_dSoil, m_dSZF);
	if (result >= 0)
	{
		SetDescript(buffer);
	}
	SetFuncName(L"UBC1988");

	//	GSaveHistoryFormat(_T("UBC-88 : Soil = %3.1f  Z = %4.2f"),m_dSoil,m_dSZF);
}

void CSpfcUBC88Util::MakePOSpectrumData(double dDamping, double SRa, double SRv, CArray <double, double>& aPeriod, CArray <double, double>& aAccel)
{

	double st = PRD_ST;
	double ed = m_dMaxPeriod * 2;
	int Step = PRD_NUM * 4;

	if (dDamping >= 0)
	{
		SRa = (3.21 - 0.68 * log(dDamping)) / 2.12;
		SRv = (2.31 - 0.41 * log(dDamping)) / 1.65;
	}

	m_dOIF = 1.0;
	m_dRw = 1.0;

	double Tn, SA, Value, dt;
	double T1;// 꺽이는 시점
	int n = 0;

	dt = (ed - st) / Step;
	double dTol = dt / Step;  // Tolerance


	for (int q = 0; q <= Step; q++)
	{
		Tn = st + dt * q;

		//		if ( Tn <= 1.0E-6 ) Tn=1.0E-6f;

		if (m_dSoil == 1.0)
		{
			if (Tn >= 0 && Tn <= 0.15) SA = 1.0 + Tn * (2.5 * SRa - 1.0) / 0.15;
			if (Tn > 0.15 + dTol && Tn - dt < 0.15 - dTol)
			{
				Value = 2.5 * m_dSZF * m_dOIF / m_dRw * SRa;
				aPeriod.Add(0.15);
				aAccel.Add(Value);
			}
			T1 = 0.39 * SRv / SRa;
			if (Tn > 0.15 && Tn <= T1) SA = 2.5 * SRa;
			if (Tn > T1 + dTol && Tn - dt < T1 - dTol)
			{
				Value = 2.5 * m_dSZF * m_dOIF / m_dRw * SRa;
				aPeriod.Add(T1);
				aAccel.Add(Value);

			}
			if (Tn > T1) SA = 0.975 / Tn * SRv;
		}
		else if (m_dSoil == 1.2)
		{
			if (Tn >= 0 && Tn <= 0.15) SA = 1.0 + Tn * (2.5 * SRa - 1.0) / 0.15;
			if (Tn > 0.15 + dTol && Tn - dt < 0.15 - dTol)
			{
				Value = 2.5 * m_dSZF * m_dOIF / m_dRw * SRa;
				aPeriod.Add(0.15);
				aAccel.Add(Value);

			}
			T1 = 0.585 * SRv / SRa;
			if (Tn > 0.15 && Tn <= T1) SA = 2.5 * SRa;
			if (Tn > T1 + dTol && Tn - dt < T1 - dTol)
			{
				Value = 2.5 * m_dSZF * m_dOIF / m_dRw * SRa;
				aPeriod.Add(T1);
				aAccel.Add(Value);

			}
			if (Tn > T1) SA = 1.463 / Tn * SRv;
		}
		else if (m_dSoil == 1.5)
		{
			if (Tn >= 0 && Tn <= 0.2) SA = 1.0 + Tn * (2.5 * SRa - 1.0) / 0.2;
			if (Tn > 0.2 + dTol && Tn - dt < 0.2 - dTol)
			{
				Value = 2.5 * m_dSZF * m_dOIF / m_dRw * SRa;
				aPeriod.Add(0.2);
				aAccel.Add(Value);

			}
			T1 = 0.915 * SRv / SRa;
			if (Tn > 0.2 && Tn <= T1) SA = 2.5 * SRa;
			if (Tn > T1 + dTol && Tn - dt < T1 - dTol)
			{
				Value = 2.5 * m_dSZF * m_dOIF / m_dRw * SRa;
				aPeriod.Add(T1);
				aAccel.Add(Value);

			}
			if (Tn > T1) SA = 2.288 / Tn * SRv;
		}
		Value = SA * m_dSZF * m_dOIF / m_dRw;
		aPeriod.Add(Tn);
		aAccel.Add(Value);
	}

}

void CSpfcUBC88Util::SetParamFromCode(const T_SPFC_CODE& codeParam)
{
	m_dSoil = codeParam.UBC88.dSoil;
	m_dSZF = codeParam.UBC88.dSeis;
	m_dOIF = codeParam.UBC88.dIe;
	m_dRw = codeParam.UBC88.dCoef;
	m_dMaxPeriod = codeParam.UBC88.dMaxPeriod;
}
