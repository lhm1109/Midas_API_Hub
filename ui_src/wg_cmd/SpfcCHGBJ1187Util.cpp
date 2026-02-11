#include "stdafx.h"
#include "SpfcCHGBJ1187Util.h"

CSpfcCHGBJ1187Util::CSpfcCHGBJ1187Util()
{
	m_nSFI = 0;
	m_nSC = 0;
	m_dKh = 0.1;
	m_dTg1 = 0.2;
	m_dTg2 = 1.0;
	m_dCz = 0.2;
	m_dMaxPeriod = PRD_ED;

	m_dMu = 2.25 * m_dTg1;
}

bool CSpfcCHGBJ1187Util::CheckValid()
{
	if (IsValid(m_dTg1, 0.0, 100) == false)
	{
		return false;
	}

	if (m_dMaxPeriod <= 0) { AfxMessageBox(_LS(IDS_CMD_SPECTRUM__MAX_PERIOD_GREATER_0)); return false; }

	if (m_dCz <= 0) { AfxMessageBox(_LS(IDS_CMD_SPECTRUM__GENERAL_EFFECT_FACTOR)); return false; }
	return TRUE;
}

void CSpfcCHGBJ1187Util::MakeSpectrumData(bool bOnlyCalc)
{
	double st = PRD_ST;
	double ed = m_dMaxPeriod;
	int    Step = PRD_NUM;
	double Tn, dt, Beta, dFactor;

	// Pushover FEMA440에서 계산할 때(Period 2배, Step 4배)
	if (bOnlyCalc)
	{
		ed *= GetPeriodCoeff();
		Step *= GetStepCoeff();
	}

	dt = (ed - st) / Step;  // time interval
	double dTol = dt / Step;  // Tolerance
	dFactor = m_dCz * m_dKh;

	InitPeriod();
	InitAccel();

	for (int i = 0; i <= Step; i++)
	{
		Tn = st + dt * i;

		if (Tn >= 0 && Tn < m_dTg1)	Beta = 2.25;
		else if (Tn >= m_dTg1 && Tn < m_dTg2) Beta = m_dMu / Tn;
		else Beta = 0.45;

		if (Tn > m_dTg1 + dTol && Tn < m_dTg1 + dt - dTol) // transition point
		{
			AddPeriod(m_dTg1);
			AddAccel(2.25 * dFactor);
		}
		if (Tn > m_dTg2 + dTol && Tn < m_dTg2 + dt - dTol)
		{
			AddPeriod(m_dTg2);
			AddAccel(0.45 * dFactor);
		}

		AddPeriod(Tn);
		AddAccel(Beta * dFactor);
	}
	if (bOnlyCalc) return;

	CString strSFI[] = { _T("7"),_T("8"),_T("9") };
	CString strSC[] = { _T("I"), _T("II"), _T("III") };

	wchar_t buffer[SpfcUtil::BUFFER_SIZE];
	int result = std::swprintf(buffer, SpfcUtil::BUFFER_SIZE, _LS(IDS_WG_CMD__ADD_CH_GBJ111_87_PROFILE), strSFI[m_nSFI], m_dKh, strSC[m_nSC], m_dTg1, m_dCz);
	if (result >= 0)
	{
		SetDescript(buffer);
	}
	SetFuncName(L"China(GBJ11-87)");
}

void CSpfcCHGBJ1187Util::MakePOSpectrumData(double dDamping, double SRa, double SRv, CArray<double, double>& aPeriod, CArray<double, double>& aAccel)
{
	aPeriod.RemoveAll();
	aAccel.RemoveAll();

	double st = PRD_ST;
	double ed = m_dMaxPeriod * 2;
	int    Step = PRD_NUM * 4;
	double Tn, dt;
	double Beta, dFactor;
	double Tg1;   // Tc의 변환점

	dFactor = m_dCz * m_dKh;

	if (dDamping >= 0)
	{
		SRa = (3.21 - 0.68 * log(dDamping)) / 2.12;
		SRv = (2.31 - 0.41 * log(dDamping)) / 1.65;
	}

	dt = (ed - st) / Step;  // time interval	
	double dTol = dt / Step;  // Tolerance

	Tg1 = m_dTg1 * SRv / SRa;  // modified Tg

	for (int i = 0; i <= Step; i++)
	{
		Tn = st + dt * i;
		//		if ( Tn <= 1.0E-4 ) Tn=1.0E-4;

		if (Tn >= 0 && Tn < Tg1)	Beta = 2.25 * SRa;
		else if (Tn >= Tg1 && Tn < m_dTg2) Beta = (m_dMu / Tn) * SRv;
		else Beta = 0.45 * SRv;

		if (Tn > Tg1 + dTol && Tn < Tg1 + dt - dTol) // transition point		
		{
			aPeriod.Add(Tg1);
			aAccel.Add(2.25 * SRa * dFactor);
		}
		if (Tn > m_dTg2 + dTol && Tn < m_dTg2 + dt - dTol)
		{
			aPeriod.Add(m_dTg2);
			aAccel.Add(0.45 * SRv * dFactor);
		}

		aPeriod.Add(Tn);
		aAccel.Add(Beta * dFactor);
	}
}

void CSpfcCHGBJ1187Util::SetParamFromCode(const T_SPFC_CODE& codeParam)
{
	m_nSFI = codeParam.CHGBJ111_87.nSFI;
	m_dKh = codeParam.CHGBJ111_87.dKh;
	m_nSC = codeParam.CHGBJ111_87.nSc;
	m_dTg1 = codeParam.CHGBJ111_87.dTg1;
	m_dTg2 = codeParam.CHGBJ111_87.dTg2;
	m_dCz = codeParam.CHGBJ111_87.dCz;
	m_dMaxPeriod = codeParam.CHGBJ111_87.dMaxPeriod;
}

void CSpfcCHGBJ1187Util::SetParamFromCalc()
{
	m_dMu = 2.25 * m_dTg1;
}
