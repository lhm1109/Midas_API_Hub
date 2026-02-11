#include "stdafx.h"
#include "SpfcIBC2012Util.h"
#include "../wg_db/Seismic_NSCP2024.h"

CSpfcIBC2012Util::CSpfcIBC2012Util(int nCode)
	: m_nCode(nCode)
{
	m_nSite = 3;
	m_dSs = 0.75;
	m_dS1 = 0.3;
	m_dFa = 1.2;
	m_dFv = 1.8;
	m_dSds = 0.6;
	m_dSd1 = 0.36;
	m_dI = 1.0;
	m_dR = 4.0;
	m_dTL = 4.0;
	m_dMaxPeriod = PRD_ED;

	m_dTs = m_dSd1 / m_dSds;
	m_dT0 = 0.2 * m_dTs;
}

bool CSpfcIBC2012Util::CheckValid()
{
	CString strMsg;
	if (m_dSs <= 0.)
	{
		strMsg.Format(_LS(IDS_CMD_ERROR_NAME_POSITIVE), _T("Ss"));
		AfxMessageBox(strMsg);
		return FALSE;
	}
	if (m_dS1 <= 0.)
	{
		strMsg.Format(_LS(IDS_CMD_ERROR_NAME_POSITIVE), _T("S1"));
		AfxMessageBox(strMsg);
		return FALSE;
	}
	if (m_dFa <= 0.)
	{
		strMsg.Format(_LS(IDS_CMD_ERROR_NAME_POSITIVE), _T("Fa"));
		AfxMessageBox(strMsg);
		return FALSE;
	}
	if (m_dFv <= 0.)
	{
		strMsg.Format(_LS(IDS_CMD_ERROR_NAME_POSITIVE), _T("Fv"));
		AfxMessageBox(strMsg);
		return FALSE;
	}
	if (m_dSds <= 0.)
	{
		strMsg.Format(_LS(IDS_CMD_ERROR_NAME_POSITIVE), _T("Sds"));
		AfxMessageBox(strMsg);
		return FALSE;
	}
	if (m_dSd1 <= 0.)
	{
		strMsg.Format(_LS(IDS_CMD_ERROR_NAME_POSITIVE), _T("Sd1"));
		AfxMessageBox(strMsg);
		return FALSE;
	}
	if (m_dI <= 0.)
	{
		strMsg.Format(_LS(IDS_CMD_ERROR_NAME_POSITIVE), _T("I"));
		AfxMessageBox(strMsg);
		return FALSE;
	}
	if (m_dR <= 0.)
	{
		strMsg.Format(_LS(IDS_CMD_ERROR_NAME_POSITIVE), _T("R"));
		AfxMessageBox(strMsg);
		return FALSE;
	}

	if (m_dMaxPeriod <= 0) { AfxMessageBox(_LS(IDS_CMD_SPECTRUM__MAX_PERIOD_GREATER_0)); return FALSE; }

	if (m_dTL <= m_dTs || m_dTL >= m_dMaxPeriod)
	{
		AfxMessageBox(_LS(IDS_CMD_SPECTRUM_TL));
		return FALSE;
	}

	return TRUE;
}

void CSpfcIBC2012Util::MakeSpectrumData(bool bOnlyCalc /*= false*/)
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

	switch (m_nCode)
	{
	case D_SPFC_CODE_IBC2009:
	case D_SPFC_CODE_IBC2012:
	{
		double Tn, dt;
		double dSa;

		dt = (ed - st) / Step;  // time interval	
		double dTol = dt / Step;  // Tolerance

		double dI_R = m_dI / m_dR;

		InitPeriod();
		InitAccel();

		for (int i = 0; i <= Step; i++)
		{
			Tn = st + dt * i;

			if (Tn >= 0 && Tn < m_dT0)
				dSa = m_dSds * (0.4 + 0.6 * Tn / m_dT0);
			else if (Tn >= m_dT0 && Tn < m_dTs)
				dSa = m_dSds;
			else if (Tn >= m_dTs && Tn < m_dTL)
				dSa = m_dSd1 / Tn;
			else if (Tn >= m_dTL)
				dSa = m_dSd1 * m_dTL / (Tn * Tn);

			if (Tn > m_dT0 + dTol && Tn < m_dT0 + dt - dTol) // transition point		
			{
				AddPeriod(m_dT0);
				AddAccel(m_dSds * dI_R);
			}
			if (Tn > m_dTs + dTol && Tn < m_dTs + dt - dTol) // transition point		
			{
				AddPeriod(m_dTs);
				AddAccel(m_dSds * dI_R);
			}

			dSa *= dI_R;

			AddPeriod(Tn);
			AddAccel(dSa);
		}
	}
	break;
	case D_SPFC_CODE_NSCP_2024:
	{
		CSeismic_NSCP2024 CodeCalc;
		CArray <double, double> arPeriod, arAccel;
		CodeCalc.MakeDesignResponseSpectrum(st, ed, Step, m_dT0, m_dTs, m_dTL, m_dSds, m_dSd1, m_dI, m_dR, &arPeriod, &arAccel);

		InitPeriod();
		InitAccel();

		for (int i = 0; i < arPeriod.GetSize(); ++i)
		{
			AddPeriod(arPeriod[i]);
		}

		for (int i = 0; i < arAccel.GetSize(); ++i)
		{
			AddAccel(arAccel[i]);
		}
	}
	break;
	default:
		ASSERT(0);
		break;
	}

	if (bOnlyCalc) return;

	CString strSite[] = { _T("A"),_T("B"),_T("C"),_T("D"),_T("E") };
	wchar_t buffer[SpfcUtil::BUFFER_SIZE];
	std::wstring funcName{};
	int result = -1;

	switch (m_nCode)
	{
	case D_SPFC_CODE_IBC2009:
	{
		result = std::swprintf(buffer, SpfcUtil::BUFFER_SIZE, _LS(IDS_WG_CMD__ADD_IBC2009_PROFILE), strSite[m_nSite], m_dSs, m_dS1, m_dFa, m_dFv, m_dSds, m_dSd1, m_dI, m_dR);
		funcName = _T("IBC2009(ASCE7-05)");
	}
	break;
	case D_SPFC_CODE_IBC2012:
	{
		result = std::swprintf(buffer, SpfcUtil::BUFFER_SIZE, _LS(IDS_WG_CMD__ADD_IBC2012_PROFILE), strSite[m_nSite], m_dSs, m_dS1, m_dFa, m_dFv, m_dSds, m_dSd1, m_dI, m_dR);
		funcName = _T("IBC2012(ASCE7-10)");
	}
	break;
	case D_SPFC_CODE_NSCP_2024:
	{
		result = std::swprintf(buffer, SpfcUtil::BUFFER_SIZE, _LS(IDS_WG_CMD__ADD_NSCP_2024_PROFILE), strSite[m_nSite], m_dSs, m_dS1, m_dFa, m_dFv, m_dSds, m_dSd1, m_dI, m_dR);
		funcName = _T("NSCP 2024");
	}
	break;
	default:
		ASSERT(0);
		break;
	}

	if (result >= 0)
	{
		SetDescript(buffer);
	}

	SetFuncName(funcName);
}

void CSpfcIBC2012Util::MakePOSpectrumData(double dDamping, double SRa, double SRv, CArray <double, double>& aPeriod, CArray <double, double>& aAccel)
{
	aPeriod.RemoveAll();
	aAccel.RemoveAll();

	double st = PRD_ST;
	double ed = m_dMaxPeriod * 2;
	int Step = PRD_NUM * 4;

	switch (m_nCode)
	{
	case D_SPFC_CODE_IBC2009:
	case D_SPFC_CODE_IBC2012:
	{
		double Tn, dt;
		double dSa;
		double dTs, dT0, dTL;

		if (dDamping >= 0)
		{
			SRa = (3.21 - 0.68 * log(dDamping)) / 2.12;
			SRv = (2.31 - 0.41 * log(dDamping)) / 1.65;
		}

		dt = (ed - st) / Step;  // time interval	
		double dTol = dt / Step;  // Tolerance

		double dI_R = m_dI / m_dR;

		dTs = m_dTs * SRv / SRa;
		dTL = m_dTL * SRv / SRa;
		dT0 = 0.2 * dTs;

		for (int i = 0; i <= Step; i++)
		{
			Tn = st + dt * i;

			if (Tn >= 0 && Tn < dT0)
				dSa = m_dSds * (0.4 + (SRa - 0.4) * Tn / dT0);
			else if (Tn >= dT0 && Tn < dTs)
				dSa = m_dSds * SRa;
			else if (Tn >= dTs && Tn < dTL)
				dSa = m_dSd1 / Tn * SRv;
			else if (Tn >= dTL)
				dSa = m_dSd1 * dTL / (Tn * Tn) * SRv;

			if (Tn > dT0 + dTol && Tn < dT0 + dt - dTol) // transition point		
			{
				aPeriod.Add(dT0);
				aAccel.Add(m_dSds * SRa * dI_R);
			}
			if (Tn > dTs + dTol && Tn < dTs + dt - dTol) // transition point		
			{
				aPeriod.Add(dTs);
				aAccel.Add(m_dSds * SRa * dI_R);
			}

			dSa *= dI_R;

			aPeriod.Add(Tn);
			aAccel.Add(dSa);
		}
	}
	break;
	case D_SPFC_CODE_NSCP_2024:
	{
		CSeismic_NSCP2024 CodeCalc;
		CodeCalc.MakePushoverSpectrum(dDamping, SRa, SRv, st, ed, Step, m_dTs, m_dTL, m_dSds, m_dSd1, m_dI, m_dR, aPeriod, aAccel);
	}
	break;
	default:
		ASSERT(0);
		break;
	}
}

void CSpfcIBC2012Util::SetParamFromCode(const T_SPFC_CODE& codeParam)
{
	m_nSite = codeParam.IBC2009.nSc;
	m_dSs = codeParam.IBC2009.dSs;
	m_dS1 = codeParam.IBC2009.dS1;
	m_dFa = codeParam.IBC2009.dFa;
	m_dFv = codeParam.IBC2009.dFv;
	m_dSds = codeParam.IBC2009.dSds;
	m_dSd1 = codeParam.IBC2009.dSd1;
	m_dI = codeParam.IBC2009.dIe;
	m_dR = codeParam.IBC2009.dCoef;
	m_dTL = codeParam.IBC2009.dTL;
	m_dMaxPeriod = codeParam.IBC2009.dMaxPeriod;
}

void CSpfcIBC2012Util::SetParamFromCalc()
{
	m_dTs = m_dSd1 / m_dSds;
	m_dT0 = 0.2 * m_dTs;
}