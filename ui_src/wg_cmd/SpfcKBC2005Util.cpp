#include "stdafx.h"
#include "SpfcKBC2005Util.h"

CSpfcKBC2005Util::CSpfcKBC2005Util()
{
	m_nAccMethod = 0;
	m_nZone = 0;
	m_nSite = 0;
	m_dSds = 0.52668;
	m_dSd1 = 0.33649;
	m_dR = 1.0;
	m_dI = 1.0;
	m_bRelieve = FALSE;
	m_dMaxPeriod = PRD_ED;
	m_dT0 = 0.2 * m_dTs;
	m_dTs = m_dSd1 / m_dSds;
}

bool CSpfcKBC2005Util::CheckValid()
{
	CString strMsg;
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
		strMsg.Format(_LS(IDS_CMD_ERROR_NAME_POSITIVE), _T("Ie"));
		AfxMessageBox(strMsg);
		return FALSE;
	}
	if (m_dR <= 0.)
	{
		strMsg.Format(_LS(IDS_CMD_ERROR_NAME_POSITIVE), _T("R"));
		AfxMessageBox(strMsg);
		return FALSE;
	}

	//  if(!IsValidCmb_d(&m_cmbIF,0,10,1.0)) return FALSE;
	//  if(!IsValidCmb_d(&m_cmbNC,0,50,1.0)) return FALSE;  
	if (m_dMaxPeriod <= 0) { AfxMessageBox(_LS(IDS_CMD_SPECTRUM__MAX_PERIOD_GREATER_0)); return FALSE; }
	return TRUE;
}

void CSpfcKBC2005Util::MakeSpectrumData(bool bOnlyCalc)
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
	double dSa;

	dt = (ed - st) / Step;  // time interval	
	double dTol = dt / Step;  // Tolerance

	BOOL bRelieve = FALSE;
	if (m_bRelieve && m_dT0 > 0.06 && m_dT0 < 0.3)
	{
		m_dT0 = 0.3;
		bRelieve = TRUE;
	}

	InitPeriod();
	InitAccel();

	for (int i = 0; i <= Step; i++)
	{
		Tn = st + dt * i;

		if (Tn >= 0 && Tn <= m_dT0)
		{
			if (bRelieve) dSa = (2 * m_dSds * Tn + 0.4 * m_dSds) * m_dI / m_dR;
			else         dSa = (0.6 * (m_dSds / m_dT0) * Tn + 0.4 * m_dSds) * m_dI / m_dR;
		}
		else if (Tn > m_dT0 && Tn <= m_dTs) dSa = m_dSds * m_dI / m_dR;
		else if (Tn > m_dTs)				 			   dSa = min((m_dSd1 / Tn) * m_dI / m_dR, m_dSds * m_dI / m_dR);

		if (Tn > m_dT0 + dTol && Tn < m_dT0 + dt - dTol) // transition point		
		{
			AddPeriod(m_dT0);
			AddAccel(m_dSds * m_dI / m_dR);
		}
		if (Tn > m_dTs + dTol && Tn < m_dTs + dt - dTol) // transition point		
		{
			AddPeriod(m_dTs);
			AddAccel(m_dSds * m_dI / m_dR);
		}

		AddPeriod(Tn);
		AddAccel(dSa);
	}

	if (bOnlyCalc) return;

	CString strZone[] = { _T("ZoneI(0.11)"),_T("ZoneII(0.07)") };
	CString strSite[] = { _T("Sa"),_T("Sb"),_T("Sc"),_T("Sd"),_T("Se") };
	CString strRelieve[] = { _T("No"), _T("Yes") };

	wchar_t buffer[SpfcUtil::BUFFER_SIZE];
	int result = 0;
	if (m_nAccMethod == 0)
	{
		result = std::swprintf(buffer, SpfcUtil::BUFFER_SIZE, _LS(IDS_WG_CMD__ADD_KBC2005_PROFILE1), strZone[m_nZone], strSite[m_nSite], m_dSds, m_dSd1, m_dI, m_dR);//, strRelieve[m_bRelieve]);
	}
	else
	{
		result = std::swprintf(buffer, SpfcUtil::BUFFER_SIZE, _LS(IDS_WG_CMD__ADD_KBC2005_PROFILE2), m_dSds, m_dSd1, m_dI, m_dR);//, strRelieve[m_bRelieve]);
	}

	if (result >= 0)
	{
		SetDescript(buffer);
	}

	SetFuncName(L"KBC2005");
}

void CSpfcKBC2005Util::MakePOSpectrumData(double dDamping, double SRa, double SRv, CArray<double, double>& aPeriod, CArray<double, double>& aAccel)
{
	aPeriod.RemoveAll();
	aAccel.RemoveAll();

	double st = PRD_ST;
	double ed = m_dMaxPeriod * 2;
	int Step = PRD_NUM * 4;
	double Tn, dt;
	double dSa;
	double dTs, dT0;

	if (dDamping >= 0)
	{
		SRa = (3.21 - 0.68 * log(dDamping)) / 2.12;
		SRv = (2.31 - 0.41 * log(dDamping)) / 1.65;
	}

	dt = (ed - st) / Step;  // time interval	
	double dTol = dt / Step;  // Tolerance

	dTs = m_dTs * SRv / SRa;
	dT0 = 0.2 * dTs;

	BOOL bRelieve = FALSE;
	if (m_bRelieve && dT0 > 0.06 && dT0 < 0.3)
	{
		dT0 = 0.3;
		bRelieve = TRUE;
	}

	for (int i = 0; i <= Step; i++)
	{
		Tn = st + dt * i;

		if (Tn >= 0 && Tn <= dT0)
		{
			if (bRelieve) dSa = (0.4 + (SRa - 0.4) * Tn / 0.3) * m_dSds * m_dI / m_dR;
			else         dSa = (0.4 + (SRa - 0.4) * Tn / dT0) * m_dSds * m_dI / m_dR;
		}
		else if (Tn > dT0 && Tn <= dTs) dSa = (m_dSds * m_dI / m_dR) * SRa;
		else if (Tn > dTs)					 	   dSa = min((m_dSd1 / Tn) * (m_dI / m_dR) * SRv, (m_dSds * m_dI / m_dR) * SRa);

		if (Tn > dT0 + dTol && Tn < dT0 + dt - dTol) // transition point		
		{
			aPeriod.Add(dT0);
			aAccel.Add((m_dSds * m_dI / m_dR) * SRa);
		}
		if (Tn > dTs + dTol && Tn < dTs + dt - dTol) // transition point		
		{
			aPeriod.Add(dTs);
			aAccel.Add((m_dSds * m_dI / m_dR) * SRa);
		}

		aPeriod.Add(Tn);
		aAccel.Add(dSa);
	}
}

void CSpfcKBC2005Util::SetParamFromCode(const T_SPFC_CODE& codeParam)
{
	m_nAccMethod = codeParam.KBC2005.nAccMethod;
	m_nZone = codeParam.KBC2005.nSeisZone;
	m_nSite = codeParam.KBC2005.nSiteClass;
	m_bRelieve = codeParam.KBC2005.bRelieve;
	m_dSds = codeParam.KBC2005.dSds;
	m_dSd1 = codeParam.KBC2005.dSd1;
	m_dI = codeParam.KBC2005.dIe;
	m_dR = codeParam.KBC2005.dCoef;
	m_dMaxPeriod = codeParam.KBC2005.dMaxPeriod;
}

void CSpfcKBC2005Util::SetParamFromCalc()
{
	m_dTs = m_dSd1 / m_dSds;
	m_dT0 = 0.2 * m_dTs;
}
