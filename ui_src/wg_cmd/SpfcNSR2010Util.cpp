#include "stdafx.h"
#include "SpfcNSR2010Util.h"
#include "../wg_db/CalcNSR2010.h"

CSpfcNSR2010Util::CSpfcNSR2010Util()
{
	m_nSite = 0;

	m_dAa = 0.75;
	m_dAv = 0.3;
	m_dFa = 1.2;
	m_dFv = 1.8;
	m_dSdsUnUsed = 0.6;
	m_dSdUnUsed = 0.36;
	m_dTc = m_dSdUnUsed / m_dSdsUnUsed;
	m_dR = 4.0;
	m_dI = 1.0;
	m_dMaxPeriod = PRD_ED;
	m_dTLUnUsed = 4.0;
	m_dPhi = 1.0;
}

bool CSpfcNSR2010Util::CheckValid()
{
	CString strMsg;
	if (m_dAa <= 0.)
	{
		strMsg.Format(_LS(IDS_CMD_ERROR_NAME_POSITIVE), _T("Aa"));
		AfxMessageBox(strMsg);
		return false;
	}
	if (m_dAv <= 0.)
	{
		strMsg.Format(_LS(IDS_CMD_ERROR_NAME_POSITIVE), _T("Av"));
		AfxMessageBox(strMsg);
		return false;
	}
	if (m_dFa <= 0.)
	{
		strMsg.Format(_LS(IDS_CMD_ERROR_NAME_POSITIVE), _T("Fa"));
		AfxMessageBox(strMsg);
		return false;
	}
	if (m_dFv <= 0.)
	{
		strMsg.Format(_LS(IDS_CMD_ERROR_NAME_POSITIVE), _T("Fv"));
		AfxMessageBox(strMsg);
		return false;
	}
	if (m_dI <= 0.)
	{
		strMsg.Format(_LS(IDS_CMD_ERROR_NAME_POSITIVE), _T("I"));
		AfxMessageBox(strMsg);
		return false;
	}
	if (m_dR <= 0.)
	{
		strMsg.Format(_LS(IDS_CMD_ERROR_NAME_POSITIVE), _T("R"));
		AfxMessageBox(strMsg);
		return false;
	}

	if (m_dPhi <= 0.)
	{
		strMsg.Format(_LS(IDS_CMD_ERROR_NAME_POSITIVE), _T("Phi"));
		AfxMessageBox(strMsg);
		return false;
	}

	if (m_dMaxPeriod <= 0) { AfxMessageBox(_LS(IDS_CMD_SPECTRUM__MAX_PERIOD_GREATER_0)); return false; }

	if (m_dTLUnUsed <= m_dTc || m_dTLUnUsed >= m_dMaxPeriod)
	{
		AfxMessageBox(_LS(IDS_CMD_SPECTRUM_TL));
		return false;
	}

	return true;
}

void CSpfcNSR2010Util::MakeSpectrumData(bool bOnlyCalc)
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
	double Tx, dx;
	double dSa;

	dx = (ed - st) / Step;  // time interval	
	double dTol = dx / Step;  // Tolerance

	//double dI_R = m_dI / m_dR;

	double dAaFaI = m_dAa * m_dFa * m_dI;
	double dAvFvI = m_dAv * m_dFv * m_dI;

	InitPeriod();
	InitAccel();

	for (int i = 0; i <= Step; i++)
	{
		Tx = st + dx * i;

		if (Tx >= 0 && Tx < m_dT0)
			dSa = 2.5 * dAaFaI * (0.4 + 0.6 * Tx / m_dT0);
		else if (Tx >= m_dT0 && Tx < m_dTc)
			dSa = 2.5 * dAaFaI;
		else if (Tx >= m_dTc && Tx < m_dTL)
			dSa = 1.2 * dAvFvI / Tx;
		else if (Tx >= m_dTL)
			dSa = 1.2 * dAvFvI * m_dTL / (Tx * Tx);

		/*
		if(Tx > m_dT0  + dTol && Tx < m_dT0 + dx - dTol) // transition point
		{
			m_parPeriod->Add(m_dT0);
			m_parAccel->Add(m_dSdsUnUsed * dI_R);
		}
		if(Tx > m_dTc + dTol && Tx < m_dTc + dx - dTol) // transition point
		{
			m_parPeriod->Add(m_dTc);
			m_parAccel->Add(m_dSdsUnUsed * dI_R);
		}
		*/

		dSa /= (m_dR * m_dPhi);

		AddPeriod(Tx);
		AddAccel(dSa);
	}

	if (bOnlyCalc) return;

	CString strSite[] = { _T("A"),_T("B"),_T("C"),_T("D"),_T("E") };

	wchar_t buffer[SpfcUtil::BUFFER_SIZE];
	int result = std::swprintf(buffer, SpfcUtil::BUFFER_SIZE, _LS(IDS_WG_CMD__ADD_NSR2010_PROFILE), strSite[m_nSite], m_dAa, m_dAv, m_dFa, m_dFv, m_dI, m_dR * m_dPhi);
	if (result >= 0)
	{
		SetDescript(buffer);
	}
	SetFuncName(L"NSR-10");
}

void CSpfcNSR2010Util::MakePOSpectrumData(double dDamping, double SRa, double SRv, CArray<double, double>& aPeriod, CArray<double, double>& aAccel)
{
	aPeriod.RemoveAll();
	aAccel.RemoveAll();

	double st = PRD_ST;
	double ed = m_dMaxPeriod * 2;
	int Step = PRD_NUM * 4;
	double Tx, dx;
	double dSa;
	double dTc, dT0, dTL;

	if (dDamping >= 0)
	{
		SRa = (3.21 - 0.68 * log(dDamping)) / 2.12;
		SRv = (2.31 - 0.41 * log(dDamping)) / 1.65;
	}

	dx = (ed - st) / Step;  // time interval	
	double dTol = dx / Step;  // Tolerance

	double dAaFaI = m_dAa * m_dFa * m_dI;
	double dAvFvI = m_dAv * m_dFv * m_dI;

	dTc = m_dTc * SRv / SRa;
	dTL = m_dTL * SRv / SRa;
	dT0 = 0.2 * dTc;

	for (int i = 0; i <= Step; i++)
	{
		Tx = st + dx * i;

		if (Tx >= 0 && Tx < dT0)
			dSa = 2.5 * dAaFaI * (0.4 + (SRa - 0.4) * Tx / dT0);
		else if (Tx >= dT0 && Tx < dTc)
			dSa = 2.5 * dAaFaI * SRa;
		else if (Tx >= dTc && Tx < dTL)
			dSa = 1.2 * dAvFvI / Tx * SRv;
		else if (Tx >= dTL)
			dSa = 1.2 * dAvFvI * dTL / (Tx * Tx) * SRv;

		dSa /= (m_dR * m_dPhi);

		aPeriod.Add(Tx);
		aAccel.Add(dSa);
	}
}
void CSpfcNSR2010Util::SetParamFromCode(const T_SPFC_CODE& codeParam)
{
	m_nSite = codeParam.NSR10.nSiteClass;
	m_dAa = codeParam.NSR10.dAa;
	m_dAv = codeParam.NSR10.dAv;
	m_dFa = codeParam.NSR10.dFa;
	m_dFv = codeParam.NSR10.dFv;
	m_dI = codeParam.NSR10.dIe;
	m_dR = codeParam.NSR10.dCoef;
	m_dPhi = codeParam.NSR10.dPhi;
	m_dMaxPeriod = codeParam.NSR10.dMaxPeriod;
}

void CSpfcNSR2010Util::SetParamFromCalc()
{
	m_dTc = CCalcNSR2010::CalcSpecturmTC(m_dAa, m_dAv, m_dFa, m_dFv);
	m_dT0 = CCalcNSR2010::CalcSpecturmT0(m_dAa, m_dAv, m_dFa, m_dFv);
	m_dTL = CCalcNSR2010::CalcSpectrumTL(m_dFv);
}
