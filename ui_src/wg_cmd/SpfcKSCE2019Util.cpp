#include "stdafx.h"
#include "SpfcKSCE2019Util.h"
#include "..\wg_db\wg_db_UnitCtrl.h"

CSpfcKSCE2019Util::CSpfcKSCE2019Util()
{
	m_nZone = 0;
	m_nSite = 0;
	m_dI = 1.0;
	m_dMaxPeriod = 6.0;
	m_dFa = 0.0;
	m_dFv = 0.0;

	m_dZoneFactor = CalZonFactor(m_nZone, m_dI);
	m_dT0 = 0.06;
	m_dTs = 0.3;
	m_dTl = 3.0;
}

bool CSpfcKSCE2019Util::CheckValid()
{
	CString strMsg;

	if (m_nSite == 5)
	{
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
	}
	else
	{
		if (m_dFa < 0.)
		{
			strMsg.Format(_LS(IDS_CMD_ERROR_NAME_POSITIVE), _T("Fa"));
			AfxMessageBox(strMsg);
			return false;
		}
		if (m_dFv < 0.)
		{
			strMsg.Format(_LS(IDS_CMD_ERROR_NAME_POSITIVE), _T("Fv"));
			AfxMessageBox(strMsg);
			return false;
		}
	}


	if (m_dI <= 0.)
	{
		strMsg.Format(_LS(IDS_CMD_ERROR_NAME_POSITIVE), _T("I "));
		AfxMessageBox(strMsg);
		return false;
	}

	if (m_dMaxPeriod <= 0) { AfxMessageBox(_LS(IDS_CMD_SPECTRUM__MAX_PERIOD_GREATER_0)); return false; }
	return true;
}

void CSpfcKSCE2019Util::MakeSpectrumData(bool bOnlyCalc)
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
	double dSa = 0.0;
	double dSa_tmp = 0.0; // transition point 에서 사용

	dt = (ed - st) / Step;  // time interval	
	double dTol = dt / Step;  // Tolerance

	double dS = m_dZoneFactor;

	InitPeriod();
	InitAccel();

	for (int i = 0; i <= Step; i++)
	{
		Tn = st + dt * i;

		if (Tn >= 0 && Tn <= m_dT0)
		{
			if (m_nSite == 0) dSa = dS * (1 + 30.0 * Tn);
			else           dSa = m_dFa * dS * (1 + 1.5 * Tn / m_dT0);
		}
		else if (Tn > m_dT0 && Tn <= m_dTs)
		{
			if (m_nSite == 0) dSa = 2.8 * dS;
			else           dSa = m_dFa * 2.5 * dS;
		}
		else if (Tn > m_dTs && Tn <= m_dTl)
		{
			if (m_nSite == 0) dSa = 0.84 * dS / Tn;
			else           dSa = m_dFv * dS / Tn;
		}
		else if (Tn > m_dTl)
		{
			if (m_nSite == 0) dSa = 2.52 * dS / pow(Tn, 2.0);
			else           dSa = m_dFv * dS * m_dTl / pow(Tn, 2.0);
		}

		if (Tn > m_dT0 + dTol && Tn < m_dT0 + dt - dTol) // transition point		
		{
			if (m_nSite == 0) dSa_tmp = 2.8 * dS;
			else           dSa_tmp = m_dFa * 2.5 * dS;

			AddPeriod(m_dT0);
			AddAccel(dSa_tmp);
		}
		if (Tn > m_dTs + dTol && Tn < m_dTs + dt - dTol) // transition point		
		{
			if (m_nSite == 0) dSa_tmp = 2.8 * dS;
			else           dSa_tmp = m_dFa * 2.5 * dS;

			AddPeriod(m_dTs);
			AddAccel(dSa_tmp);
		}

		AddPeriod(Tn);
		AddAccel(dSa);
	}

	if (bOnlyCalc) return;

	CString strZone[] = { _T("ZoneI(0.11)"),_T("ZoneII(0.07)") };
	CString strSite[] = { _T("S1"), _T("S2"), _T("S3"), _T("S4"), _T("S5"), _T("S6") };


	// Zone 은 0 = 1, 1 = 2
	// 수정하려면 CString strSite[]={_T("1"),_T("2")}; 이거 추가 한 후에 Msg 수정
	CString strUnit;
	if (!CUnitCtrl::GetUnitSystem(D_UNITSYS_BASE_LENGTH, strUnit)) strUnit = _T("");
	wchar_t buffer[SpfcUtil::BUFFER_SIZE];

	int result = std::swprintf(buffer, SpfcUtil::BUFFER_SIZE, _LS(IDS_WG_CMD__ADD_KSCE2019_PROFILE), strZone[m_nZone], m_dI, strSite[m_nSite]/*,strUnit*/);

	if (result >= 0)
	{
		SetDescript(buffer);
	}

	SetFuncName(L"KDS(17-10-00:2018)");
}

void CSpfcKSCE2019Util::MakePOSpectrumData(double dDamping, double SRa, double SRv, CArray<double, double>& aPeriod, CArray<double, double>& aAccel)
{
	aPeriod.RemoveAll();
	aAccel.RemoveAll();

	double st = PRD_ST;
	double ed = m_dMaxPeriod * 2;
	int Step = PRD_NUM * 4;
	double Tn, dt;
	double dSa = 0.0;
	double dSa_tmp = 0.0; // transition point 에서 사용
	double dTs, dT0, dTl;

	// 이 식은 damping을 고려한 감소계수(이론값)임.
	// 설계기준에서 제시한 값이 있지만 다른 기준들 처럼 아래 식을 적용함.
	if (dDamping >= 0)
	{
		SRa = (3.21 - 0.68 * log(dDamping)) / 2.12;
		SRv = (2.31 - 0.41 * log(dDamping)) / 1.65;
	}

	dTs = m_dTs * SRv / SRa;
	dT0 = 0.2 * dTs;
	dTl = m_dTl;

	dt = (ed - st) / Step;  // time interval	
	double dTol = dt / Step;  // Tolerance

	double dS = m_dZoneFactor;

	for (int i = 0; i <= Step; i++)
	{
		Tn = st + dt * i;

		if (Tn >= 0 && Tn <= dT0)
		{
			if (m_nSite == 0) dSa = dS * (1 + 30.0 * Tn);
			else           dSa = m_dFa * dS * (1 + 1.5 * Tn / dT0);
		}
		else if (Tn > dT0 && Tn <= dTs)
		{
			if (m_nSite == 0) dSa = 2.8 * dS;
			else           dSa = m_dFa * 2.5 * dS;
			dSa *= SRa;
		}
		else if (Tn > dTs && Tn <= dTl)
		{
			if (m_nSite == 0) dSa = 0.84 * dS / Tn;
			else           dSa = m_dFv * dS / Tn;
			dSa *= SRv;
		}
		else if (Tn > dTl)
		{
			if (m_nSite == 0) dSa = 2.52 * dS / pow(Tn, 2.0);
			else           dSa = m_dFv * dS * dTl / pow(Tn, 2.0);
			dSa *= SRv;
		}

		if (Tn > dT0 + dTol && Tn < dT0 + dt - dTol) // transition point		
		{
			if (m_nSite == 0) dSa_tmp = 2.8 * dS;
			else           dSa_tmp = m_dFa * 2.5 * dS;
			dSa_tmp *= SRa;

			aPeriod.Add(dT0);
			aAccel.Add(dSa_tmp);
		}
		if (Tn > dTs + dTol && Tn < dTs + dt - dTol) // transition point		
		{
			if (m_nSite == 0) dSa_tmp = 2.8 * dS;
			else           dSa_tmp = m_dFa * 2.5 * dS;
			dSa_tmp *= SRa;

			aPeriod.Add(dTs);
			aAccel.Add(dSa_tmp);
		}

		aPeriod.Add(Tn);
		aAccel.Add(dSa);

	}
}

void CSpfcKSCE2019Util::SetParamFromCode(const T_SPFC_CODE& codeParam)
{
	m_nZone = codeParam.KSCE2019.nSeisZone;
	m_nSite = codeParam.KSCE2019.nSiteClass;
	m_dI = codeParam.KSCE2019.dIe;
	m_dMaxPeriod = codeParam.KSCE2019.dMaxPeriod;
	m_dFa = codeParam.KSCE2019.dFa;
	m_dFv = codeParam.KSCE2019.dFv;
}

void CSpfcKSCE2019Util::SetParamFromCalc()
{
	m_dZoneFactor = CalZonFactor(m_nZone, m_dI);

	if (m_nSite == 0) // S1
	{
		m_dT0 = 0.06;
		m_dTs = 0.3;
		m_dTl = 3.0;
	}
	else
	{
		m_dTs = (m_dFa == 0.0) ? 0.0 : m_dFv / (2.5 * m_dFa);
		m_dT0 = 0.2 * m_dTs;
		m_dTl = 3.0;
	}
}

double CSpfcKSCE2019Util::CalZonFactor(int nZone, double dI)
{
	double dZone = (nZone == 0) ? 0.11 : 0.07;

	double dZoneFactor = dZone * dI;

	return dZoneFactor;
}

