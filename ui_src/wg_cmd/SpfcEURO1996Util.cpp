#include "stdafx.h"
#include "SpfcEURO1996Util.h"

CSpfcEURO1996Util::CSpfcEURO1996Util()
{
	m_nSc = 0;
	m_nKd = 0;
	m_nKr = 0;

	m_dQ0 = 5.0;
	m_dKw = 0.0;
	m_dAlpha = 0.0;
	m_dMaxPeriod = PRD_ED;
}

bool CSpfcEURO1996Util::CheckValid()
{
	if (IsValid(m_dQ0, 0, 10) == false)
		return false;

	if (m_dMaxPeriod <= 0) { AfxMessageBox(_LS(IDS_CMD_SPECTRUM__MAX_PERIOD_GREATER_0)); return false; }
	if (m_dKw <= 0) { AfxMessageBox(_LS(IDS_CMD_SPECTRUM__FAILURE_MODE_FACTOR)); return false; }
	if (m_dAlpha <= 0) { AfxMessageBox(_LS(IDS_CMD_SPECTRUM__GROUND_ACC_RATIO)); return false; }
	return true;
}

void CSpfcEURO1996Util::MakeSpectrumData(bool bOnlyCalc)
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
	double Qb;    // behavior factor
	double SdT;

	dt = (ed - st) / Step;  // time interval	
	double dTol = dt / Step;  // Tolerance
	Qb = m_dQ0 * m_dKd * m_dKr * m_dKw;
	if (Qb < 1.5) Qb = 1.5;

	InitPeriod();
	InitAccel();

	for (int i = 0; i <= Step; i++)
	{
		Tn = st + dt * i;

		//		if ( Tn <= 1.0E-4 ) Tn=1.0E-4;

		if (Tn >= 0 && Tn < m_dTb)			 SdT = m_dAlpha * m_dSc * (1 + (m_dBeta / Qb - 1) * (Tn / m_dTb));
		else if (Tn >= m_dTb && Tn < m_dTc) SdT = m_dAlpha * m_dSc * (m_dBeta / Qb);
		else if (Tn >= m_dTc && Tn < m_dTd)
		{
			SdT = m_dAlpha * m_dSc * (m_dBeta / Qb) * pow((m_dTc / Tn), m_dKd1);
			SdT = max(SdT, 0.2 * m_dAlpha);
		}
		else
		{
			SdT = m_dAlpha * m_dSc * (m_dBeta / Qb) * pow((m_dTc / m_dTd), m_dKd1) * pow((m_dTd / Tn), m_dKd2);
			SdT = max(SdT, 0.2 * m_dAlpha);
		}

		if (Tn > m_dTb + dTol && Tn < m_dTb + dt - dTol) // transition point		
		{
			AddPeriod(m_dTb);
			AddAccel(m_dAlpha * m_dSc * (m_dBeta / Qb));
		}
		if (Tn > m_dTc + dTol && Tn < m_dTc + dt - dTol) // transition point		
		{
			AddPeriod(m_dTc);
			AddAccel(m_dAlpha * m_dSc * (m_dBeta / Qb));
		}
		if (Tn > m_dTd + dTol && Tn < m_dTd + dt - dTol) // transition point		
		{
			AddPeriod(m_dTd);
			AddAccel(max(m_dAlpha * m_dSc * (m_dBeta / Qb) * pow((m_dTc / m_dTd), m_dKd1), 0.2 * m_dAlpha));
		}

		AddPeriod(Tn);
		AddAccel(SdT);
	}

	if (bOnlyCalc) return;

	wchar_t buffer[SpfcUtil::BUFFER_SIZE];
	int result = std::swprintf(buffer, SpfcUtil::BUFFER_SIZE, _LS(IDS_WG_CMD__ADD_EURO1996_PROFILE), m_sSoil, m_dQ0, m_dKd, m_dKr, m_dKw, m_dAlpha);
	if (result >= 0)
	{
		SetDescript(buffer);
	}
	SetFuncName(L"EURO1996 DESIGN");
}

void CSpfcEURO1996Util::MakePOSpectrumData(double dDamping, double SRa, double SRv, CArray<double, double>& aPeriod, CArray<double, double>& aAccel)
{
	aPeriod.RemoveAll();
	aAccel.RemoveAll();

	double st = PRD_ST;
	double ed = m_dMaxPeriod * 2;
	int Step = PRD_NUM * 4;
	double Tn, dt;
	double Qb;    // behavior factor
	double SdT;
	double Tc1;   // Tc의 변환점

	if (dDamping >= 0)
	{
		SRa = (3.21 - 0.68 * log(dDamping)) / 2.12;
		SRv = (2.31 - 0.41 * log(dDamping)) / 1.65;
	}

	dt = (ed - st) / Step;  // time interval	
	double dTol = dt / Step;  // Tolerance
	Qb = m_dQ0 * m_dKd * m_dKr * m_dKw;
	if (Qb > 1.5) Qb = 1.5;   // restriction of Qb : 규준에서는 Qb >= 1.5 라고 되어 있는데 
	// Qb >= 1.5 가 맞는 것 같음(인쇄가 잘못 된 것 같음)                    
	Tc1 = m_dTc * pow(SRv / SRa, 1.0 / m_dKd1);  // modified Tg

	for (int i = 0; i <= Step; i++)
	{
		Tn = st + dt * i;

		//		if (Tn <= 1.0E-4) Tn=1.0E-4;	

		if (Tn >= 0 && Tn < m_dTb)
			SdT = m_dAlpha * m_dSc * (1 + (SRa * m_dBeta / Qb - 1) * (Tn / m_dTb));
		else if (Tn >= m_dTb && Tn <= Tc1)
			SdT = m_dAlpha * m_dSc * (m_dBeta / Qb) * SRa;
		else if (Tn > Tc1 && Tn <= m_dTd)
		{
			SdT = m_dAlpha * m_dSc * (m_dBeta / Qb) * pow((m_dTc / Tn), m_dKd1) * SRv;
			SdT = max(SdT, 0.2 * m_dAlpha * SRv);
		}
		else if (Tn > m_dTd)
		{
			SdT = m_dAlpha * m_dSc * (m_dBeta / Qb) * pow((m_dTc / m_dTd), m_dKd1) * pow((m_dTd / Tn), m_dKd2) * SRv;
			SdT = max(SdT, 0.2 * m_dAlpha * SRv);
		}

		if (Tn > m_dTb + dTol && Tn < m_dTb + dt - dTol) // transition point		
		{
			aPeriod.Add(m_dTb);
			aAccel.Add(m_dAlpha * m_dSc * (m_dBeta / Qb) * SRa);
		}
		if (Tn > Tc1 + dTol && Tn < Tc1 + dt - dTol) // transition point		
		{
			aPeriod.Add(Tc1);
			aAccel.Add(m_dAlpha * m_dSc * (m_dBeta / Qb) * SRa);
		}
		if (Tn > m_dTd + dTol && Tn < m_dTd + dt - dTol) // transition point		
		{
			aPeriod.Add(m_dTd);
			aAccel.Add(max(m_dAlpha * m_dSc * (m_dBeta / Qb) * pow((m_dTc / m_dTd), m_dKd1) * SRv, 0.2 * m_dAlpha * SRv));
		}
		aPeriod.Add(Tn);  //
		aAccel.Add(SdT);  //
	}
}

void CSpfcEURO1996Util::SetParamFromCode(const T_SPFC_CODE& codeParam)
{
	m_nSc = codeParam.EURO1996.nSc;
	m_nKd = codeParam.EURO1996.nKd;
	m_nKr = codeParam.EURO1996.nKr;
	m_dQ0 = codeParam.EURO1996.dQ0;
	m_dKw = codeParam.EURO1996.dKw;
	m_dAlpha = codeParam.EURO1996.dAlpha;
	m_dMaxPeriod = codeParam.EURO1996.dMaxPeriod;
}

void CSpfcEURO1996Util::SetParamFromCalc()
{
	switch (m_nSc)
	{
	case 0: m_dSc = 1.0, m_dBeta = 2.5, m_dKd1 = 2. / 3., m_dKd2 = 5. / 3., m_dTb = 0.10, m_dTc = 0.40, m_dTd = 3.0, m_sSoil = _T("A"); break;
	case 1: m_dSc = 1.0, m_dBeta = 2.5, m_dKd1 = 2. / 3., m_dKd2 = 5. / 3., m_dTb = 0.15, m_dTc = 0.60, m_dTd = 3.0, m_sSoil = _T("B"); break;
	case 2: m_dSc = 0.9, m_dBeta = 2.5, m_dKd1 = 2. / 3., m_dKd2 = 5. / 3., m_dTb = 0.20, m_dTc = 0.80, m_dTd = 3.0, m_sSoil = _T("C"); break;
	}

	switch (m_nKd)
	{
	case 0: m_dKd = 1.00; break;
	case 1: m_dKd = 0.75; break;
	case 2: m_dKd = 0.50; break;
	}

	switch (m_nKr)
	{
	case 0: m_dKr = 1.0; break;
	case 1: m_dKr = 0.8; break;
	}
}