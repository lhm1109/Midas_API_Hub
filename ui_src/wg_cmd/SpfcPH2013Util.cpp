#include "stdafx.h"
#include "SpfcPH2013Util.h"

CSpfcPH2013Util::CSpfcPH2013Util()
{
	m_nGroundType = 0;

	m_dPGA = 0.5;
	m_dSS = 1.1;
	m_dS1 = 0.6;
	m_dRMF = 1.0;
	m_dMaxPeriod = 6.0;
	m_strGT = _T("");

	m_dFpga = 0.0;
	m_dFa = 0.0;
	m_dFv = 0.0;
}

bool CSpfcPH2013Util::CheckValid()
{
	if (m_dPGA <= 0) { AfxMessageBox(_LS(IDS_CMD_SPECTRUM__FAILURE_PGA)); return FALSE; }
	if (m_dSS <= 0) { AfxMessageBox(_LS(IDS_CMD_SPECTRUM__FAILURE_SS)); return FALSE; }
	if (m_dS1 <= 0) { AfxMessageBox(_LS(IDS_CMD_SPECTRUM__FAILURE_S1)); return FALSE; }
	if (m_dRMF <= 0) { AfxMessageBox(_LS(IDS_CMD_SPECTRUM__FAILURE_RMF)); return FALSE; }

	if (m_dMaxPeriod <= 0) { AfxMessageBox(_LS(IDS_CMD_SPECTRUM__MAX_PERIOD_GREATER_0)); return FALSE; }

	return TRUE;
}

void CSpfcPH2013Util::MakeSpectrumData(bool bOnlyCalc)
{
	double st = PRD_ST;
	double ed = m_dMaxPeriod;
	int Step = PRD_NUM;
	double Tn, dt;
	double dSdT;

	// Pushover FEMA440에서 계산할 때(Period 2배, Step 4배)
	if (bOnlyCalc)
	{
		ed *= GetPeriodCoeff();
		Step *= GetStepCoeff();
	}

	dt = (ed - st) / Step;  // time interval	
	double dTol = dt / Step;  // Tolerance

	double dRange1 = ((m_dFv * m_dS1) / (m_dFa * m_dSS)) * 0.2;
	double dRange2 = ((m_dFv * m_dS1) / (m_dFa * m_dSS));

	InitPeriod();
	InitAccel();

	for (int i = 0; i <= Step; i++)
	{
		Tn = st + dt * i;

		if (Tn == 0.0) dSdT = m_dPGA * m_dFpga;
		else if (Tn > 0 && Tn < dRange1)
		{
			dSdT = (m_dPGA * m_dFpga) + (((m_dSS * m_dFa) - (m_dPGA * m_dFpga)) * (Tn / dRange1));
			dSdT = dSdT / m_dRMF;
		}
		else if (Tn >= dRange1 && Tn < dRange2)
		{
			dSdT = m_dSS * m_dFa;
			dSdT = dSdT / m_dRMF;
		}
		else
		{
			dSdT = (m_dS1 * m_dFv) / Tn;
			dSdT = dSdT / m_dRMF;
		}

		if (Tn > dRange1 + dTol && Tn < dRange1 + dt - dTol) // transition point		
		{
			AddPeriod(dRange1);
			AddAccel((m_dSS * m_dFa) / m_dRMF);
		}
		if (Tn > dRange2 + dTol && Tn < dRange2 + dt - dTol) // transition point		
		{
			AddPeriod(dRange2);
			AddAccel((m_dSS * m_dFa) / m_dRMF);
		}

		/*if (m_parPeriod)*/ AddPeriod(Tn);
		/*if (m_parAccel)*/ AddAccel(dSdT);
	}

	if (bOnlyCalc) return;

	wchar_t buffer[SpfcUtil::BUFFER_SIZE];
	int result = std::swprintf(buffer, SpfcUtil::BUFFER_SIZE, _LS(IDS_WG_CMD__ADD_DPWH_LRFD_2013_PROFILE), m_strGT, m_dPGA, m_dSS, m_dS1, m_dRMF);
	if (result >= 0)
	{
		SetDescript(buffer);
	}
	SetFuncName(L"DPWH-LRFD BSDS(2013)");
}

void CSpfcPH2013Util::MakePOSpectrumData(double dDamping, double SRa, double SRv, CArray<double, double>& aPeriod, CArray<double, double>& aAccel)
{
	aPeriod.RemoveAll();
	aAccel.RemoveAll();

	double st = PRD_ST;
	double ed = m_dMaxPeriod * 2;
	int Step = PRD_NUM * 4;
	double Tn, dt;
	double dSdT;
	double dTs, dT0;

	if (dDamping >= 0)
	{
		SRa = (3.21 - 0.68 * log(dDamping)) / 2.12;
		SRv = (2.31 - 0.41 * log(dDamping)) / 1.65;
	}

	dt = (ed - st) / Step;  // time interval	
	double dTol = dt / Step;  // Tolerance

	double dRange1 = ((m_dFv * m_dS1) / (m_dFa * m_dSS)) * 0.2;
	double dRange2 = ((m_dFv * m_dS1) / (m_dFa * m_dSS));

	dTs = dRange2 * SRv / SRa;
	dT0 = 0.2 * dRange2;

	for (int i = 0; i <= Step; i++)
	{
		Tn = st + dt * i;

		if (Tn == 0.0) dSdT = m_dPGA * m_dFpga;
		else if (Tn > 0 && Tn < dT0)
		{
			dSdT = (m_dPGA * m_dFpga) + ((SRa - (m_dSS * m_dFa) - (m_dPGA * m_dFpga)) / (Tn / dT0));
			dSdT = dSdT / m_dRMF;
		}
		else if (Tn >= dT0 && Tn < dTs)
		{
			dSdT = m_dSS * m_dFa * SRa;
			dSdT = dSdT / m_dRMF;
		}
		else
		{
			dSdT = ((m_dS1 * m_dFv) / Tn) * SRv;
			dSdT = dSdT / m_dRMF;
		}

		if (Tn > dT0 + dTol && Tn < dT0 + dt - dTol) // transition point		
		{
			aPeriod.Add(dT0);
			aAccel.Add((m_dSS * m_dFa / m_dRMF) * SRa);
		}
		if (Tn > dTs + dTol && Tn < dTs + dt - dTol) // transition point		
		{
			aPeriod.Add(dTs);
			aAccel.Add((m_dSS * m_dFa / m_dRMF) * SRa);
		}

		aPeriod.Add(Tn);
		aAccel.Add(dSdT);
	}
}

void CSpfcPH2013Util::SetParamFromCode(const T_SPFC_CODE& codeParam)
{
	m_nGroundType = codeParam.DPWHLRFD2013.nGroundType;
	m_dPGA = codeParam.DPWHLRFD2013.dPGA;
	m_dSS = codeParam.DPWHLRFD2013.dSs;
	m_dS1 = codeParam.DPWHLRFD2013.dS1;
	m_dRMF = codeParam.DPWHLRFD2013.dR;
	m_dMaxPeriod = codeParam.DPWHLRFD2013.dMaxPeriod;
}

void CSpfcPH2013Util::SetParamFromCalc()
{
	// Calc Fpga
	m_dFpga = 0.0;

	if (m_nGroundType == 0)
	{
		if (m_dPGA <= 0.2)
		{
			m_dFpga = 1.2;
		}
		else if (m_dPGA > 0.2 && m_dPGA <= 0.4)
		{
			m_dFpga = -m_dPGA + 1.4;
		}
		else
		{
			m_dFpga = 1.0;
		}
	}
	else if (m_nGroundType == 1)
	{
		if (m_dPGA <= 0.1)
		{
			m_dFpga = 1.6;
		}
		else if (m_dPGA > 0.1 && m_dPGA <= 0.4)
		{
			m_dFpga = (-2.0 * m_dPGA) + 1.8;
		}
		else if (m_dPGA > 0.4 && m_dPGA <= 0.5)
		{
			m_dFpga = -m_dPGA + 1.4;
		}
		else if (m_dPGA > 0.5 && m_dPGA <= 0.8)
		{
			m_dFpga = ((-1.0 / 6.0) * m_dPGA) + (59.0 / 60.0);
		}
		else
		{
			m_dFpga = 0.85;
		}
	}
	else if (m_nGroundType == 2)
	{
		if (m_dPGA <= 0.1)
		{
			m_dFpga = 2.5;
		}
		else if (m_dPGA > 0.1 && m_dPGA <= 0.2)
		{
			m_dFpga = (-8.0 * m_dPGA) + 3.3;
		}
		else if (m_dPGA > 0.2 && m_dPGA <= 0.3)
		{
			m_dFpga = (-5.0 * m_dPGA) + 2.7;
		}
		else if (m_dPGA > 0.3 && m_dPGA <= 0.4)
		{
			m_dFpga = (-3.0 * m_dPGA) + 2.1;
		}
		else if (m_dPGA > 0.4 && m_dPGA <= 0.5)
		{
			m_dFpga = -m_dPGA + 1.3;
		}
		else if (m_dPGA > 0.5 && m_dPGA <= 0.8)
		{
			m_dFpga = ((-1.0 / 6.0) * m_dPGA) + (53.0 / 60.0);
		}
		else
		{
			m_dFpga = 0.75;
		}
	}
	else
	{
		ASSERT(0);
	}

	// Calc Fa
	m_dFa = 0.0;

	if (m_nGroundType == 0)
	{
		if (m_dSS <= 0.5)
		{
			m_dFa = 1.2;
		}
		else if (m_dSS > 0.5 && m_dSS <= 1.0)
		{
			m_dFa = (-0.4 * m_dSS) + 1.4;
		}
		else
		{
			m_dFa = 1.0;
		}
	}
	else if (m_nGroundType == 1)
	{
		if (m_dSS <= 0.25)
		{
			m_dFa = 1.6;
		}
		else if (m_dSS > 0.25 && m_dSS <= 1.0)
		{
			m_dFa = (-0.8 * m_dSS) + 1.8;
		}
		else if (m_dSS > 1.0 && m_dSS <= 1.25)
		{
			m_dFa = (-0.4 * m_dSS) + 1.4;
		}
		else if (m_dSS > 1.25 && m_dSS <= 2.0)
		{
			m_dFa = ((-1.0 / 15.0) * m_dSS) + (59.0 / 60.0);
		}
		else
		{
			m_dFa = 0.85;
		}
	}
	else if (m_nGroundType == 2)
	{
		if (m_dSS <= 0.25)
		{
			m_dFa = 2.5;
		}
		else if (m_dSS > 0.25 && m_dSS <= 0.5)
		{
			m_dFa = (-3.2 * m_dSS) + 3.3;
		}
		else if (m_dSS > 0.5 && m_dSS <= 0.75)
		{
			m_dFa = (-2.0 * m_dSS) + 2.7;
		}
		else if (m_dSS > 0.75 && m_dSS <= 1.0)
		{
			m_dFa = (-1.2 * m_dSS) + 2.1;
		}
		else if (m_dSS > 1.0 && m_dSS <= 1.25)
		{
			m_dFa = (-0.4 * m_dSS) + 1.3;
		}
		else if (m_dSS > 1.25 && m_dSS <= 2.0)
		{
			m_dFa = ((-1.0 / 15.0) * m_dSS) + (53.0 / 60.0);
		}
		else
		{
			m_dFa = 0.75;
		}
	}
	else
	{
		ASSERT(0);
	}

	// Calc Fv
	m_dFv = 0.0;

	if (m_nGroundType == 0)
	{
		if (m_dS1 <= 0.1)
		{
			m_dFv = 1.7;
		}
		else if (m_dS1 > 0.1 && m_dS1 <= 0.4)
		{
			m_dFv = -m_dS1 + 1.8;
		}
		else
		{
			m_dFv = 1.4;
		}
	}
	else if (m_nGroundType == 1)
	{
		if (m_dS1 <= 0.1)
		{
			m_dFv = 2.4;
		}
		else if (m_dS1 > 0.1 && m_dS1 <= 0.2)
		{
			m_dFv = (-4.0 * m_dS1) + 2.8;
		}
		else if (m_dS1 > 0.2 && m_dS1 <= 0.4)
		{
			m_dFv = (-2.0 * m_dS1) + 2.4;
		}
		else if (m_dS1 > 0.4 && m_dS1 <= 0.5)
		{
			m_dFv = -m_dS1 + 2.0;
		}
		else
		{
			m_dFv = 1.5;
		}
	}
	else if (m_nGroundType == 2)
	{
		if (m_dS1 <= 0.1)
		{
			m_dFv = 3.5;
		}
		else if (m_dS1 > 0.1 && m_dS1 <= 0.2)
		{
			m_dFv = (-3.0 * m_dS1) + 3.8;
		}
		else if (m_dS1 > 0.2 && m_dS1 <= 0.4)
		{
			m_dFv = (-4.0 * m_dS1) + 4.0;
		}
		else if (m_dS1 > 0.4 && m_dS1 <= 0.5)
		{
			m_dFv = 2.4;
		}
		else if (m_dS1 > 0.5 && m_dS1 <= 0.8)
		{
			m_dFv = ((-4.0 / 3.0) * m_dS1) + (46.0 / 15.0);
		}
		else
		{
			m_dFv = 2.0;
		}
	}
	else
	{
		ASSERT(0);
	}

	m_strGT = _T("");

	if (m_nGroundType == 0) m_strGT = _LS(IDS_WG_CMD_DPWH_LRFD_2013_GT_1);
	else if (m_nGroundType == 1) m_strGT = _LS(IDS_WG_CMD_DPWH_LRFD_2013_GT_2);
	else if (m_nGroundType == 2) m_strGT = _LS(IDS_WG_CMD_DPWH_LRFD_2013_GT_3);
	else { ASSERT(0); }
}