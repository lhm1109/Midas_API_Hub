#include "stdafx.h"
#include "SpfcJPN2000Util.h"

CSpfcJPN2000Util::CSpfcJPN2000Util()
{
	m_wndZ = 0;
	m_wndTc = 0;
	m_dC0 = 0.2;
	m_dMaxPeriod = PRD_ED;

	m_dZ = 1.0;
	m_dTc = 0.576;
	m_dSlope = 6.9120;
}

bool CSpfcJPN2000Util::CheckValid()
{
	if (m_dMaxPeriod <= 0) { AfxMessageBox(_LS(IDS_CMD_SPECTRUM__MAX_PERIOD_GREATER_0)); return false; }
	if (m_dC0 <= 0) { AfxMessageBox(_LS(IDS_CMD_SPECTRUM__BASE_SHEAR_GREATER_0)); return false; }
	return true;
}

void CSpfcJPN2000Util::MakeSpectrumData(bool bOnlyCalc)
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
	double Tn, dt, Sa;

	dt = (ed - st) / Step;  // time interval	
	double dTol = dt / Step;  // Tolerance

	InitPeriod();
	InitAccel();

	for (int i = 0; i <= Step; i++)
	{
		Tn = st + dt * i;
		//		if ( Tn <= 1.0E-4 ) Tn=1.0E-4;

		if (Tn >= 0 && Tn < 0.16)    			  Sa = m_dZ * m_dC0 * (4.8 + 45 * Tn) / 9.806;
		else if (Tn >= 0.16 && Tn < m_dTc) 	Sa = m_dZ * m_dC0 * 12.0 / 9.806;
		else if (Tn >= m_dTc)          			Sa = m_dZ * m_dC0 * m_dSlope / Tn / 9.806;

		if (Tn > 0.16 + dTol && Tn < 0.16 + dt - dTol) // transition point		
		{
			AddPeriod(0.16);
			AddAccel(m_dZ * m_dC0 * 12.0 / 9.806);
		}
		if (Tn > m_dTc + dTol && Tn < m_dTc + dt - dTol) // transition point
		{
			AddPeriod(m_dTc);
			AddAccel(m_dZ * m_dC0 * 12.0 / 9.806);
		}

		AddPeriod(Tn);
		AddAccel(Sa);
	}

	if (bOnlyCalc) return;
	CString strZ[] = { _T("1(1.0)"),_T("2(0.9)"),_T("3(0.8)"),_T("4(0.7)") };
	CString strSoil[] = { _T("I"), _T("II"), _T("III") };

	wchar_t buffer[SpfcUtil::BUFFER_SIZE];

	int result = std::swprintf(buffer, SpfcUtil::BUFFER_SIZE, _LS(IDS_WG_CMD__ADD_JPN2000_PROFILE), strZ[m_wndZ], strSoil[m_wndTc], m_dC0);

	if (result >= 0)
	{
		SetDescript(buffer);
	}

	SetFuncName(L"JP2000");
}

void CSpfcJPN2000Util::MakePOSpectrumData(double dDamping, double SRa, double SRv, CArray<double, double>& aPeriod, CArray<double, double>& aAccel)
{
	aPeriod.RemoveAll();
	aAccel.RemoveAll();

	double st = PRD_ST;
	double ed = m_dMaxPeriod * 2;
	int Step = PRD_NUM * 4;
	double Tn, dt, Sa;

	if (dDamping >= 0)
	{
		SRa = (3.21 - 0.68 * log(dDamping)) / 2.12;
		SRv = (2.31 - 0.41 * log(dDamping)) / 1.65;
	}

	dt = (ed - st) / Step;  // time interval	
	double dTol = dt / Step;  // Tolerance

	double Tg = m_dTc * SRv / SRa;

	for (int i = 0; i <= Step; i++)
	{
		Tn = st + dt * i;
		//		if ( Tn <= 1.0E-4 ) Tn=1.0E-4;

		if (Tn >= 0 && Tn < 0.16)		    Sa = m_dZ * m_dC0 * ((12.0 * SRa - 4.8) * Tn / 0.16 + 4.8) / 9.806;
		else if (Tn >= 0.16 && Tn <= Tg) Sa = m_dZ * m_dC0 * 12 * SRa / 9.806;
		else if (Tn > Tg)			          Sa = m_dZ * m_dC0 * m_dSlope * SRv / Tn / 9.806;

		if (Tn > 0.16 + dTol && Tn < 0.16 + dt - dTol) // transition point		
		{
			aPeriod.Add(0.16);
			aAccel.Add(m_dZ * m_dC0 * 12.0 * SRa / 9.806);
		}
		if (Tn > Tg + dTol && Tn < Tg + dt - dTol) // transition point		
		{
			aPeriod.Add(Tg);
			aAccel.Add(m_dZ * m_dC0 * 12.0 * SRa / 9.806);
		}

		aPeriod.Add(Tn);
		aAccel.Add(Sa);
	}

}

void CSpfcJPN2000Util::SetParamFromCode(const T_SPFC_CODE& codeParam)
{
	m_wndZ = codeParam.JP2000.nZ;
	m_wndTc = codeParam.JP2000.nSoilClass;
	m_dC0 = codeParam.JP2000.dCo;
	m_dMaxPeriod = codeParam.JP2000.dMaxPeriod;
}

void CSpfcJPN2000Util::SetParamFromCalc()
{
	switch (m_wndZ)   // Seismic zone factor Z
	{
	case 0: m_dZ = 1.0; break;
	case 1: m_dZ = 0.9; break;
	case 2: m_dZ = 0.8; break;
	case 3: m_dZ = 0.7; break;
	}

	switch (m_wndTc)  // soil period Tc
	{
	case 0: m_dTc = 0.576; m_dSlope = 6.9120; break;
	case 1: m_dTc = 0.864; m_dSlope = 10.368; break;
	case 2: m_dTc = 1.152; m_dSlope = 13.824; break;
	}
}
