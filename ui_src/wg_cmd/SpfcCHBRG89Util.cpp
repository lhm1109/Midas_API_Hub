#include "stdafx.h"
#include "SpfcCHBRG89Util.h"

CSpfcCHBRG89Util::CSpfcCHBRG89Util()
{
	m_nSFI = 0;
	m_nSC = 0;

	m_dKh = 0.1;
	m_dTg = 0.2;
	m_dMu = 1.0;
	m_dk = 1.0;

	m_dCi = 0.6;
	m_dCz = 0.2;

	m_dMaxPeriod = PRD_ED;
}

bool CSpfcCHBRG89Util::CheckValid()
{
	if (IsValid(m_dCi, 0, 10) == false)
	{
		return false;
	}

	if (IsValid(m_dCz, 0, 10) == false)
	{
		return false;
	}

	if (IsValid(m_dKh, 0.1, 100) == false)
	{
		return false;
	}

	if (IsValid(m_dTg, 0.1, 100) == false)
	{
		return false;
	}

	if (m_dMaxPeriod <= 0) { AfxMessageBox(_LS(IDS_CMD_SPECTRUM__MAX_PERIOD_GREATER_0)); return false; }
	return TRUE;
}

void CSpfcCHBRG89Util::MakeSpectrumData(bool bOnlyCalc)
{
	double st = PRD_ST;
	double ed = m_dMaxPeriod;
	int Step = PRD_NUM;
	double Tn, dt, Beta, dFactor;
	double dT2;  // minimum level transition point

	// Pushover FEMA440에서 계산할 때(Period 2배, Step 4배)
	if (bOnlyCalc)
	{
		ed *= GetPeriodCoeff();
		Step *= GetStepCoeff();
	}

	dt = (ed - st) / Step;  // time interval
	double dTol = dt / Step;  // Tolerance
	dFactor = m_dCi * m_dCz * m_dKh;
	dT2 = m_dTg / pow((0.3 / 2.25), (1.0 / m_dk));

	InitPeriod();
	InitAccel();

	for (int i = 0; i <= Step; i++)
	{
		Tn = st + dt * i;
		//		if ( Tn <= 1.0E-4 ) Tn=1.0E-4;

		if (Tn >= 0 && Tn < 0.1)		 			 Beta = 1.0 + 12.5 * Tn;
		else if (Tn >= 0.1 && Tn < m_dTg) Beta = 2.25;
		else			                       Beta = 2.25 * pow((m_dTg / Tn), m_dk);

		if (Tn > 0.1 + dTol && Tn < 0.1 + dt - dTol) // transition point		
		{
			AddPeriod(0.1);
			AddAccel(2.25 * dFactor);
		}
		if (Tn > m_dTg + dTol && Tn < m_dTg + dt - dTol) // transition point		
		{
			AddPeriod(m_dTg);
			AddAccel(2.25 * dFactor);
		}
		if (Tn > dT2 + dTol && Tn < dT2 + dt - dTol)
		{
			AddPeriod(dT2);
			AddAccel(0.3 * dFactor);
		}

		if (Beta < 0.3) Beta = 0.3;

		AddPeriod(Tn);
		AddAccel(Beta * dFactor);
	}

	if (bOnlyCalc) return;
	CString strSFI[] = { _T("7"),_T("8"),_T("9") };
	CString strSC[] = { _T("I"), _T("II"), _T("III"), _T("IV") };

	wchar_t buffer[SpfcUtil::BUFFER_SIZE];
	int result = std::swprintf(buffer, SpfcUtil::BUFFER_SIZE, _LS(IDS_WG_CMD__ADD_CH_BRG89_PROFILE), strSFI[m_nSFI], m_dKh, strSC[m_nSC], m_dTg, m_dCi, m_dCz);

	if (result >= 0)
	{
		SetDescript(buffer);
	}
	SetFuncName(L"China(JTJ004-89)");
}

void CSpfcCHBRG89Util::MakePOSpectrumData(double dDamping, double SRa, double SRv, CArray<double, double>& aPeriod, CArray<double, double>& aAccel)
{
	aPeriod.RemoveAll();
	aAccel.RemoveAll();

	double st = PRD_ST;
	double ed = m_dMaxPeriod * 2;
	int Step = PRD_NUM * 4;
	double Tn, dt;
	double Beta, dFactor;
	double Tg1;   // Tc의 변환점
	double dT2;   // minimum level transition point

	dT2 = m_dTg / pow((0.3 / 2.25), (1.0 / m_dk));
	dFactor = m_dCi * m_dCz * m_dKh;

	if (dDamping >= 0)
	{
		SRa = (3.21 - 0.68 * log(dDamping)) / 2.12;
		SRv = (2.31 - 0.41 * log(dDamping)) / 1.65;
	}

	dt = (ed - st) / Step;  // time interval	
	double dTol = dt / Step;  // Tolerance

	Tg1 = m_dTg * pow(SRv / SRa, 1.0 / m_dk);  // modified Tg

	for (int i = 0; i <= Step; i++)
	{
		Tn = st + dt * i;
		//		if ( Tn <= 1.0E-4 ) Tn=1.0E-4;

		if (Tn >= 0 && Tn < 0.1)			   Beta = 1.0 + Tn * (2.25 * SRa - 1.0) / 0.1;
		else if (Tn >= 0.1 && Tn < Tg1) Beta = 2.25 * SRa;
		else                        	 Beta = 2.25 * pow((m_dTg / Tn), m_dk) * SRv;

		if (Tn > 0.1 + dTol && Tn < 0.1 + dt - dTol) // transition point		
		{
			aPeriod.Add(0.1);
			aAccel.Add(2.25 * SRa * dFactor);
		}
		if (Tn > Tg1 + dTol && Tn < Tg1 + dt - dTol) // transition point		
		{
			aPeriod.Add(Tg1);
			aAccel.Add(2.25 * SRa * dFactor);
		}
		if (Tn > dT2 + dTol && Tn < dT2 + dt - dTol)  // transition point
		{
			aPeriod.Add(dT2);
			aAccel.Add(0.3 * SRv * dFactor);
		}

		if (Beta < 0.3 * SRv) Beta = 0.3 * SRv;

		aPeriod.Add(Tn);
		aAccel.Add(Beta * dFactor);
	}
}

void CSpfcCHBRG89Util::SetParamFromCode(const T_SPFC_CODE& codeParam)
{
	m_nSFI = codeParam.CHBRG89.nSFI;
	m_dKh = codeParam.CHBRG89.dKh;
	m_nSC = codeParam.CHBRG89.nSc;
	m_dTg = codeParam.CHBRG89.dTg;
	m_dCi = codeParam.CHBRG89.dCi;
	m_dCz = codeParam.CHBRG89.dCz;
	m_dMaxPeriod = codeParam.CHBRG89.dMaxPeriod;
}

void CSpfcCHBRG89Util::SetParamFromCalc()
{
	m_dMu = 2 * (0.7 - m_dTg);
	m_dk = 0.9 + 0.1 * m_dMu;
}
