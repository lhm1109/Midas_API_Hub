#include "stdafx.h"
#include "SpfcNBC95Util.h"

CSpfcNBC95Util::CSpfcNBC95Util()
{
	m_dv = 0.05;
	m_dZa = 1.0;
	m_dZv = 1.0;
	m_dMaxPeriod = PRD_ED;
	m_dI = 1.0;
	m_dF = 1.0;
	m_dR = 1.0;
}

bool CSpfcNBC95Util::CheckValid()
{
	if (IsValid(m_dv, 0, 10) == false) return false;
	if (IsValid(m_dI, 0, 10) == false) return false;
	if (IsValid(m_dF, 0, 10) == false) return false;
	if (IsValid(m_dR, 0, 10) == false) return false;
	if (m_dMaxPeriod <= 0) { AfxMessageBox(_LS(IDS_CMD_SPECTRUM__MAX_PERIOD_GREATER_0)); return false; }
	return true;
}

void CSpfcNBC95Util::MakeSpectrumData(bool bOnlyCalc)
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
	double Tn = 0.0;

	double dt = (ed - st) / Step;  // time interval	
	double dTol = dt / Step;  // Tolerance

	double Sa = 0.0;  // Normalized 5% damping spectrum  
	double dSamax = 0.0;
	if (m_dZa > m_dZv)       dSamax = 4.2;
	else if (m_dZa == m_dZv) dSamax = 3.0;
	else if (m_dZa < m_dZv)  dSamax = 2.1;

	double Tg = 1.2814 / dSamax;

	InitPeriod();
	InitAccel();

	for (int i = 0; i <= Step; i++)
	{
		Tn = st + dt * i;
		if (Tn <= 0) Sa = dSamax;
		else Sa = 1.2814 / Tn;

		Sa = min(dSamax, Sa);
		if (Tn > Tg + dTol && Tn < Tg + dt - dTol)
		{
			AddPeriod(Tg);
			AddAccel(dSamax * m_dv);
		}

		AddPeriod(Tn);
		AddAccel(Sa * m_dv);
	}

	if (bOnlyCalc) return;

	wchar_t buffer[SpfcUtil::BUFFER_SIZE];
	int result = std::swprintf(buffer, SpfcUtil::BUFFER_SIZE, _LS(IDS_WG_CMD__ADD_NBC1995_PROFILE), m_dv, m_dZa, m_dZv);
	if (result >= 0)
	{
		SetDescript(buffer);
	}
	SetFuncName(L"NBC1995");
}

void CSpfcNBC95Util::MakePOSpectrumData(double dDamping, double SRa, double SRv, CArray<double, double>& aPeriod, CArray<double, double>& aAccel)
{
	aPeriod.RemoveAll();
	aAccel.RemoveAll();

	double st = PRD_ST;
	double ed = m_dMaxPeriod * 2;
	int Step = PRD_NUM * 4;
	double Tn = 0.0;

	double dt = (ed - st) / Step;  // time interval	
	double dTol = dt / Step;  // Tolerance

	double Sa = 0.0;  // Normalized 5% damping spectrum    
	double dSamax = 0.0;

	if (dDamping >= 0)
	{
		SRa = (3.21 - 0.68 * log(dDamping)) / 2.12;
		SRv = (2.31 - 0.41 * log(dDamping)) / 1.65;
	}

	if (m_dZa > m_dZv)       dSamax = 4.2;
	else if (m_dZa == m_dZv) dSamax = 3.0;
	else if (m_dZa < m_dZv)  dSamax = 2.1;

	double Tg = (1.2814 / dSamax) * SRv / SRa;

	for (int i = 0; i <= Step; i++)
	{
		Tn = st + dt * i;
		if (Tn <= 0) Sa = dSamax * SRa;
		else Sa = (1.2814 / Tn) * SRv;

		Sa = min(dSamax * SRa, Sa);

		if (Tn > Tg + dTol && Tn < Tg + dt - dTol)
		{
			aPeriod.Add(Tg);
			aAccel.Add(dSamax * SRa * m_dv);
		}

		aPeriod.Add(Tn);
		aAccel.Add(Sa * m_dv);
	}
}

void CSpfcNBC95Util::SetParamFromCode(const T_SPFC_CODE& codeParam)
{
	m_dv = codeParam.NBC95.dV;
	m_dZa = codeParam.NBC95.nZa;
	m_dZv = codeParam.NBC95.nZv;
	m_dMaxPeriod = codeParam.NBC95.dMaxPeriod;
}

void CSpfcNBC95Util::SetParamFromCalc()
{
	m_dI = 1.0;
	m_dF = 1.0;
	m_dR = 0.0;
}
