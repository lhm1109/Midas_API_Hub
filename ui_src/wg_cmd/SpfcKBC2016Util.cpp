#include "stdafx.h"
#include "SpfcKBC2016Util.h"
#include "..\wg_db\wg_db_UnitCtrl.h"

CSpfcKBC2016Util::CSpfcKBC2016Util()
{
	m_dDepthToMR = 0.;
	m_dFa = 1.46;
	m_dFv = 1.58;
	m_dSds = 0.53533;
	m_dSd1 = 0.23173;
	m_dR = 1.0;
	m_dI = 1.0;
	m_dMaxPeriod = PRD_ED;

	m_nZone = 0;
	m_nSite = 0;
	m_dZoneFactor = 0.14;

	m_dTs = m_dSd1 / m_dSds;
	m_dT0 = 0.2 * m_dTs;
}

bool CSpfcKBC2016Util::CheckValid()
{
	CString strMsg;
	if (m_dDepthToMR < 0.)
	{
		strMsg.Format(_LS(IDS_CMD_ERROR_NAME_POSITIVE), _T("Depth to MR"));
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
	if (m_dSds <= 0.)
	{
		strMsg.Format(_LS(IDS_CMD_ERROR_NAME_POSITIVE), _T("Sds"));
		AfxMessageBox(strMsg);
		return false;
	}
	if (m_dSd1 <= 0.)
	{
		strMsg.Format(_LS(IDS_CMD_ERROR_NAME_POSITIVE), _T("Sd1"));
		AfxMessageBox(strMsg);
		return false;
	}
	if (m_dI <= 0.)
	{
		strMsg.Format(_LS(IDS_CMD_ERROR_NAME_POSITIVE), _T("Ie"));
		AfxMessageBox(strMsg);
		return false;
	}
	if (m_dR <= 0.)
	{
		strMsg.Format(_LS(IDS_CMD_ERROR_NAME_POSITIVE), _T("R"));
		AfxMessageBox(strMsg);
		return false;
	}

	if (m_dMaxPeriod <= 0) { AfxMessageBox(_LS(IDS_CMD_SPECTRUM__MAX_PERIOD_GREATER_0)); return false; }
	return true;
}

void CSpfcKBC2016Util::MakeSpectrumData(bool bOnlyCalc)
{
	double st = PRD_ST;
	double ed = m_dMaxPeriod;
	int Step = PRD_NUM;
	// Pushover FEMA440에서 계산할 때(Period 2배, Step 4배)
	if (bOnlyCalc)
	{
		m_dT0 = 0.2 * m_dTs;
		ed *= GetPeriodCoeff();
		Step *= GetStepCoeff();
	}
	double Tn = 0.0;
	double dSa = 0.0;

	double dt = (ed - st) / Step;  // time interval	
	double dTol = dt / Step;  // Tolerance

	InitPeriod();
	InitAccel();

	for (int i = 0; i <= Step; i++)
	{
		Tn = st + dt * i;

		if (Tn >= 0 && Tn <= m_dT0)
		{
			dSa = (0.6 * (m_dSds / m_dT0) * Tn + 0.4 * m_dSds) * m_dI / m_dR;
		}
		else if (Tn > m_dT0 && Tn <= m_dTs)
		{
			dSa = m_dSds * m_dI / m_dR;
		}
		else if (Tn > m_dTs)
		{
			dSa = min((m_dSd1 / Tn) * m_dI / m_dR, m_dSds * m_dI / m_dR);
		}

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
	CString strSite[] = { _T("Sa"),_T("Sb"),_T("Sc"),_T("Sd"),_T("Se") };

	// Zone 은 0 = 1, 1 = 2
	// 수정하려면 CString strSite[]={_T("1"),_T("2")}; 이거 추가 한 후에 Msg 수정
	CString strUnit;
	wchar_t buffer[SpfcUtil::BUFFER_SIZE];

	if (!CUnitCtrl::GetUnitSystem(D_UNITSYS_BASE_LENGTH, strUnit)) strUnit = _T("");

	int result = std::swprintf(buffer, SpfcUtil::BUFFER_SIZE, _LS(IDS_WG_CMD__ADD_KBC2015_PROFILE), m_nZone + 1, m_dZoneFactor, strSite[m_nSite], m_dDepthToMR, strUnit, m_dFa, m_dFv, m_dSds, m_dSd1, m_dI, m_dR);

	if (result >= 0)
	{
		SetDescript(buffer);
	}

	SetFuncName(_T("KBC2016"));
}

void CSpfcKBC2016Util::MakePOSpectrumData(double dDamping, double SRa, double SRv, CArray<double, double>& aPeriod, CArray<double, double>& aAccel)
{
	aPeriod.RemoveAll();
	aAccel.RemoveAll();

	double st = PRD_ST;
	double ed = m_dMaxPeriod * 2;
	int Step = PRD_NUM * 4;
	double Tn = 0.0;
	double dSa = 0.0;

	if (dDamping >= 0)
	{
		SRa = (3.21 - 0.68 * log(dDamping)) / 2.12;
		SRv = (2.31 - 0.41 * log(dDamping)) / 1.65;
	}

	double dt = (ed - st) / Step;  // time interval	
	double dTol = dt / Step;  // Tolerance

	double dTs = m_dTs * SRv / SRa;
	double dT0 = 0.2 * dTs;

	for (int i = 0; i <= Step; i++)
	{
		Tn = st + dt * i;

		if (Tn >= 0 && Tn <= dT0)
		{
			dSa = (0.4 + (SRa - 0.4) * Tn / dT0) * m_dSds * m_dI / m_dR;
		}
		else if (Tn > dT0 && Tn <= dTs)
		{
			dSa = (m_dSds * m_dI / m_dR) * SRa;
		}
		else if (Tn > dTs)
		{
			dSa = min((m_dSd1 / Tn) * (m_dI / m_dR) * SRv, (m_dSds * m_dI / m_dR) * SRa);
		}

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

void CSpfcKBC2016Util::SetParamFromCode(const T_SPFC_CODE& codeParam)
{
	m_nZone = codeParam.KBC2016.nSeisZone;
	m_nSite = codeParam.KBC2016.nSiteClass;
	m_dZoneFactor = codeParam.KBC2016.dZoneFactor;
	m_dDepthToMR = codeParam.KBC2016.dDepthMR;
	m_dFa = codeParam.KBC2016.dFa;
	m_dFv = codeParam.KBC2016.dFv;
	m_dSds = codeParam.KBC2016.dSds;
	m_dSd1 = codeParam.KBC2016.dSd1;
	m_dI = codeParam.KBC2016.dIe;
	m_dR = codeParam.KBC2016.dCoef;
	m_dMaxPeriod = codeParam.KBC2016.dMaxPeriod;
}

void CSpfcKBC2016Util::SetParamFromCalc()
{
	m_dTs = m_dSd1 / m_dSds;
	m_dT0 = 0.2 * m_dTs;
}
