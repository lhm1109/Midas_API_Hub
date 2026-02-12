#include "stdafx.h"
#include "SpfcSP2016Util.h"

CSpfcSP2016Util::CSpfcSP2016Util()
{
	m_nRegion = 0;
	m_nSoil = 0;
	m_dK1 = 0.25;
	m_dK2 = 1.0;
	m_dK3 = 1.0;
	m_dK4 = 1.0;
	m_dKPsi = 1.0;

	m_dMaxPeriod = PRD_ED;
}

bool CSpfcSP2016Util::CheckValid()
{
	CString strMsg;
	if (m_dK1 <= 0.)
	{
		strMsg.Format(_LS(IDS_CMD_ERROR_NAME_POSITIVE), _T("K1"));
		AfxMessageBox(strMsg);
		return false;
	}
	if (m_dK2 <= 0.)
	{
		strMsg.Format(_LS(IDS_CMD_ERROR_NAME_POSITIVE), _T("K2"));
		AfxMessageBox(strMsg);
		return false;
	}
	if (m_dK3 <= 0.)
	{
		strMsg.Format(_LS(IDS_CMD_ERROR_NAME_POSITIVE), _T("K3"));
		AfxMessageBox(strMsg);
		return false;
	}
	if (m_dK4 <= 0.)
	{
		strMsg.Format(_LS(IDS_CMD_ERROR_NAME_POSITIVE), _T("K4"));
		AfxMessageBox(strMsg);
		return false;
	}
	if (m_dKPsi <= 0.)
	{
		strMsg.Format(_LS(IDS_CMD_ERROR_NAME_POSITIVE), _T("KPsi"));
		AfxMessageBox(strMsg);
		return false;
	}

	if (m_dMaxPeriod <= 0) { AfxMessageBox(_LS(IDS_CMD_SPECTRUM__MAX_PERIOD_GREATER_0)); return false; }
	return true;
}

void CSpfcSP2016Util::MakeSpectrumData(bool bOnlyCalc)
{
	double st = PRD_ST;
	double ed = m_dMaxPeriod;
	int Step = PRD_NUM;
	// Pushover FEMA440에서 계산할 때(Period 2배, Step 4배)
	if (bOnlyCalc)
	{
		//m_dT0 = 0.2 * m_dTs;
		ed *= GetPeriodCoeff();
		Step *= GetStepCoeff();
	}
	double Tn = 0.0;
	double dSa = 0.0;

	int nRegion = m_nRegion;  // : “7”, “8”, “9”, “10”
	int nSoil = m_nSoil;    // : “I”, “II”, “III”
	double dK1 = m_dK1;
	double dK2 = m_dK2;
	double dK3 = m_dK3;
	double dK4 = m_dK4;
	double dKPsi = m_dKPsi;

	double T0 = 0.0;
	double T1 = 0.0;
	double T2 = 0.0;

	double dA = 0.0;
	if (nSoil == 0)  // “I”
	{
		T0 = 0.1;
		T1 = 0.3;
		T2 = 1.8;

		if (nRegion == 0)  dA = 0.1;
		else if (nRegion == 1)  dA = 0.2;
		else if (nRegion == 2)  dA = 0.4;
		else if (nRegion == 3)  dA = 0.8;
	}
	else if (nSoil == 1)  // “II”
	{
		T0 = 0.1;
		T1 = 0.5;
		T2 = 1.8;

		if (nRegion == 0)  dA = 0.1;
		else if (nRegion == 1)  dA = 0.2;
		else if (nRegion == 2)  dA = 0.4;
		else if (nRegion == 3)  dA = 0.8;
	}
	else if (nSoil == 2) // “III”
	{
		T0 = 0.1;
		T1 = 0.7;
		T2 = 1.8;

		if (nRegion == 0)  dA = 0.1;
		else if (nRegion == 1)  dA = 0.2;
		else if (nRegion == 2)  dA = 0.4;
		else if (nRegion == 3)  dA = 0.8;
	}
	else
		ASSERT(0);

	if (dA == 0.0) ASSERT(0);

	double dF1 = min(0.4, dK2 * dK3 * dK4 * dA);
	double dFactor = dK1 * dKPsi * dF1;

	double dt = (ed - st) / Step;  // time interval	
	double dTol = dt / Step;     // Tolerance

	double dBeta = 0.0;

	InitPeriod();
	InitAccel();

	for (int q = 0; q <= Step; q++)
	{
		Tn = st + dt * q;

		if (Tn >= 0 && Tn < T0)  dBeta = 1.0 + 15.0 * Tn;
		else if (Tn >= T0 && Tn < T1)  dBeta = 2.5;
		else if (Tn >= T1 && Tn < T2)
		{
			if (nSoil == 0)  dBeta = max(1.0, 0.75 / Tn);
			else if (nSoil == 1)  dBeta = max(1.0, 1.25 / Tn);
			else if (nSoil == 2)  dBeta = max(1.0, 1.75 / Tn);
		}
		else
		{
			if (nSoil == 0)  dBeta = max(1.0, 0.75 / Tn);
			else if (nSoil == 1)  dBeta = max(1.0, 1.25 / Tn);
			else if (nSoil == 2)  dBeta = max(1.0, 1.75 / Tn);
		}

		dSa = dFactor * dBeta;

		if (Tn > T0 + dTol && Tn < T0 + dt - dTol) // transition point		
		{
			AddPeriod(T0);
			AddAccel(dFactor * 2.5);
		}
		if (Tn > T1 + dTol && Tn < T1 + dt - dTol) // transition point		
		{
			AddPeriod(T1);
			AddAccel(dFactor * 2.5);
		}
		if (Tn > T2 + dTol && Tn < T2 + dt - dTol) // transition point
		{
			if (nSoil == 0)  dBeta = max(1.0, 0.75 / Tn);
			else if (nSoil == 1)  dBeta = max(1.0, 1.25 / Tn);
			else if (nSoil == 2)  dBeta = max(1.0, 1.75 / Tn);

			AddPeriod(T2);
			AddAccel(dFactor * dBeta);
		}

		AddPeriod(Tn);
		AddAccel(dSa);
	}

	if (bOnlyCalc) return;

	CString strRegion[] = { _T("7"), _T("8"), _T("9") , _T("10") };
	CString strSoil[] = { _T("S1"),_T("S2"),_T("S3"),_T("S4"),_T("S5"),_T("S6") };

	wchar_t buffer[SpfcUtil::BUFFER_SIZE];
	int result = std::swprintf(buffer, SpfcUtil::BUFFER_SIZE, _LS(IDS_WG_CMD__ADD_SP2016_PROFILE), strRegion[nRegion], strSoil[nSoil], dK1, dK2, dK3, dK4, dKPsi);
	if (result >= 0)
	{
		SetDescript(buffer);
	}
	SetFuncName(SPFC_RUS_SP_268_1325800);
}

void CSpfcSP2016Util::MakePOSpectrumData(double dDamping, double SRa, double SRv, CArray<double, double>& aPeriod, CArray<double, double>& aAccel)
{
	aPeriod.RemoveAll();
	aAccel.RemoveAll();

	ASSERT(false);
}

void CSpfcSP2016Util::SetParamFromCode(const T_SPFC_CODE& codeParam)
{
	m_nRegion = codeParam.SP2016.nRegion;
	m_nSoil = codeParam.SP2016.nSoil;
	m_dK1 = codeParam.SP2016.dK1;
	m_dK2 = codeParam.SP2016.dK2;
	m_dK3 = codeParam.SP2016.dK3;
	m_dK4 = codeParam.SP2016.dK4;
	m_dKPsi = codeParam.SP2016.dKPsi;
	m_dMaxPeriod = codeParam.SP2016.dMaxPeriod;
}
