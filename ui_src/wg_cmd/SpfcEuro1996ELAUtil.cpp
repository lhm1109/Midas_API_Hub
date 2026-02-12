#include "stdafx.h"
#include "SpfcEURO1996ELAUtil.h"

CSpfcEURO1996ELAUtil::CSpfcEURO1996ELAUtil()
{
	m_nSc = 0;
	m_dAg = 0.2;
	m_dDamping = 5.0;
	m_dMaxPeriod = PRD_ED;
}

bool CSpfcEURO1996ELAUtil::CheckValid()
{
	if (m_dAg <= 0) { AfxMessageBox(_LS(IDS_CMD_SPECTRUM__GROUND_ACC)); return false; }
	if (m_dDamping < 0) { AfxMessageBox(_LS(IDS_CMD_SPECTRUM__DAMPING_VALUE)); return false; }
	if (m_dMaxPeriod <= 0) { AfxMessageBox(_LS(IDS_CMD_SPECTRUM__MAX_PERIOD_GREATER_0)); return false; }
	return true;
}

void CSpfcEURO1996ELAUtil::MakeSpectrumData(bool bOnlyCalc)
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

	double Tn, dt;
	double SeT;

	dt = (ed - st) / Step;  // time interval	
	double dTol = dt / Step;  // Tolerance
	double dEta = max(sqrt(7. / (2. + m_dDamping)), 0.7);

	InitPeriod();
	InitAccel();

	for (int i = 0; i <= Step; i++)
	{
		Tn = st + dt * i;

		if (Tn >= 0 && Tn < m_dTb)
			SeT = m_dAg * m_dSc * (1 + (Tn / m_dTb) * (dEta * m_dBeta - 1.));
		else if (Tn >= m_dTb && Tn < m_dTc)
			SeT = m_dAg * m_dSc * dEta * m_dBeta;
		else if (Tn >= m_dTc && Tn < m_dTd)
			SeT = m_dAg * m_dSc * dEta * m_dBeta * pow((m_dTc / Tn), m_dK1);
		else
			SeT = m_dAg * m_dSc * dEta * m_dBeta * pow((m_dTc / m_dTd), m_dK1) * pow((m_dTd / Tn), m_dK2);

		if (Tn > m_dTb + dTol && Tn < m_dTb + dt - dTol) // transition point		
		{
			AddPeriod(m_dTb);
			AddAccel(m_dAg * m_dSc * dEta * m_dBeta);
		}
		if (Tn > m_dTc + dTol && Tn < m_dTc + dt - dTol) // transition point		
		{
			AddPeriod(m_dTc);
			AddAccel(m_dAg * m_dSc * dEta * m_dBeta);
		}
		if (Tn > m_dTd + dTol && Tn < m_dTd + dt - dTol) // transition point		
		{
			AddPeriod(m_dTd);
			AddAccel(m_dAg * m_dSc * dEta * m_dBeta * pow((m_dTc / m_dTd), m_dK1));
		}

		AddPeriod(Tn);
		AddAccel(SeT);
	}

	if (bOnlyCalc) return;

	wchar_t buffer[SpfcUtil::BUFFER_SIZE];
	int result = std::swprintf(buffer, SpfcUtil::BUFFER_SIZE, _LS(IDS_WG_CMD__ADD_EURO1996_ELA_PROFILE), m_sSoil, m_dAg, m_dDamping);
	if (result >= 0)
	{
		SetDescript(buffer);
	}
	SetFuncName(L"EURO1996 ELASTIC");
}

void CSpfcEURO1996ELAUtil::MakePOSpectrumData(double dDamping, double SRa, double SRv, CArray<double, double>& aPeriod, CArray<double, double>& aAccel)
{

	aPeriod.RemoveAll();
	aAccel.RemoveAll();

	double st = PRD_ST;
	double ed = m_dMaxPeriod * 2;
	int Step = PRD_NUM * 4;
	double Tn, dt;
	double SeT;
	double Tc1;   // Tc의 변환점

	if (dDamping >= 0)
	{
		SRa = (3.21 - 0.68 * log(dDamping)) / 2.12;
		SRv = (2.31 - 0.41 * log(dDamping)) / 1.65;
	}

	dt = (ed - st) / Step;  // time interval	
	double dTol = dt / Step;  // Tolerance
	double dEta = max(sqrt(7. / (2. + m_dDamping)), 0.7);

	Tc1 = m_dTc * pow(SRv / SRa, 1.0 / m_dK1);  // modified Tg

	for (int i = 0; i <= Step; i++)
	{
		Tn = st + dt * i;

		if (Tn >= 0 && Tn < m_dTb)
			SeT = m_dAg * m_dSc * (1 + (Tn / m_dTb) * (SRa * dEta * m_dBeta - 1.));
		else if (Tn >= m_dTb && Tn <= Tc1)
			SeT = m_dAg * m_dSc * dEta * m_dBeta * SRa;
		else if (Tn > Tc1 && Tn <= m_dTd)
			SeT = m_dAg * m_dSc * dEta * m_dBeta * pow((m_dTc / Tn), m_dK1) * SRv;
		else if (Tn > m_dTd)
			SeT = m_dAg * m_dSc * dEta * m_dBeta * pow((m_dTc / m_dTd), m_dK1) * pow((m_dTd / Tn), m_dK2) * SRv;

		if (Tn > m_dTb + dTol && Tn < m_dTb + dt - dTol) // transition point		
		{
			aPeriod.Add(m_dTb);
			aAccel.Add(m_dAg * m_dSc * dEta * m_dBeta * SRa);
		}
		if (Tn > Tc1 + dTol && Tn < Tc1 + dt - dTol) // transition point		
		{
			aPeriod.Add(Tc1);
			aAccel.Add(m_dAg * m_dSc * dEta * m_dBeta * SRa);
		}
		if (Tn > m_dTd + dTol && Tn < m_dTd + dt - dTol) // transition point		
		{
			aPeriod.Add(m_dTd);
			aAccel.Add(m_dAg * m_dSc * dEta * m_dBeta * pow((m_dTc / m_dTd), m_dK1) * SRv);
		}
		aPeriod.Add(Tn);
		aAccel.Add(SeT);
	}
}

void CSpfcEURO1996ELAUtil::SetParamFromCode(const T_SPFC_CODE& codeParam)
{
	m_nSc = codeParam.EURO1996_ELA.nSc;
	m_dAg = codeParam.EURO1996_ELA.dAg;
	m_dDamping = codeParam.EURO1996_ELA.dDamping;
	m_dMaxPeriod = codeParam.EURO1996_ELA.dMaxPeriod;
}

void CSpfcEURO1996ELAUtil::SetParamFromCalc()
{
	switch (m_nSc)
	{
	case 0: m_dSc = 1.0, m_dBeta = 2.5, m_dK1 = 1., m_dK2 = 2., m_dTb = 0.10, m_dTc = 0.40, m_dTd = 3.0, m_sSoil = _T("A"); break;
	case 1: m_dSc = 1.0, m_dBeta = 2.5, m_dK1 = 1., m_dK2 = 2., m_dTb = 0.15, m_dTc = 0.60, m_dTd = 3.0, m_sSoil = _T("B"); break;
	case 2: m_dSc = 0.9, m_dBeta = 2.5, m_dK1 = 1., m_dK2 = 2., m_dTb = 0.20, m_dTc = 0.80, m_dTd = 3.0, m_sSoil = _T("C"); break;
	}
}