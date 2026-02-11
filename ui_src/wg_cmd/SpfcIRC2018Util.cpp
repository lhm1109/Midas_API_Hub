#include "stdafx.h"
#include "SpfcIRC2018Util.h"

CSpfcIRC2018Util::CSpfcIRC2018Util()
{
	m_nSoil = 0;
	m_nZone = 0;
	m_dMaxPeriod = PRD_ED;

	//SetCorrectRadButS(m_pParamData->CodeParam.IRCSP114.nSeisZone);
	//CString strI, strR;
	//m_wndIs02ImportanceCmb.GetWindowText(strI);
	//m_wndIs02RCmb.GetWindowText(strR);

	m_dI = 1.0;
	m_dR = 3.0;
	m_dDamping = 5.0;
	m_dDampingFactor = GetDampingFactorIS2002(m_dDamping);
	m_dUserDefSeisZoneFact = 0.36;

	arrZone = { 0.10, 0.16, 0.24, 0.36, m_dUserDefSeisZoneFact };
	m_dZ = arrZone[0];
}

bool CSpfcIRC2018Util::CheckValid()
{
	if (IsValid(m_dI, 0, 10) == false)
		return false;

	if (IsValid(m_dR, 0, 10) == false)
		return false;

	if (IsValid(m_dDamping, 0, 10) == false)
		return false;

	if (m_dMaxPeriod <= 0) { AfxMessageBox(_LS(IDS_CMD_SPECTRUM__MAX_PERIOD_GREATER_0)); return false; }
	return true;
}

void CSpfcIRC2018Util::MakeSpectrumData(bool bOnlyCalc)
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
	double Tn, dt, dSa, dAh;

	dt = (ed - st) / Step;  // time interval
	double dTol = dt / Step;  // Tolerance	

	double dSaFac[] = { 1.00, 1.36, 1.67 };
	double dTInter[] = { 0.40, 0.55, 0.67 };
	double dAftr4sVal[] = { 0.25,0.34,0.42 };
	double dTg = dTInter[m_nSoil];
	double dFac = dSaFac[m_nSoil];
	double dAftr4sVal1 = dAftr4sVal[m_nSoil];
	double dIR = min(m_dI / m_dR, 1.5); //IRC SP 114 Cl.4.5 and 4.2.4.Maximum value will be 1.5

	InitPeriod();
	InitAccel();

	for (int i = 0; i <= Step; i++)
	{
		Tn = st + dt * i;

		if (Tn >= 0 && Tn <= 0.1)
			dSa = 1. + 15 * Tn;
		else if (Tn > 0.1 && Tn < dTg)
			dSa = 2.5;
		else if (Tn >= dTg && Tn <= 4.0)
			dSa = dFac / Tn;
		else
			dSa = dAftr4sVal1;//dFac / 4.0;

		dSa *= m_dDampingFactor;
		dAh = dIR * dSa * m_dZ / 2.;

		if (Tn > 0.1 + dTol && Tn < 0.1 + dt - dTol) // transition point		
		{
			AddPeriod(0.1);
			AddAccel(2.5 * m_dDampingFactor * dIR * m_dZ / 2.);
		}
		if (Tn > dTg + dTol && Tn < dTg + dt - dTol) // transition point		
		{
			AddPeriod(dTg);
			AddAccel(2.5 * m_dDampingFactor * dIR * m_dZ / 2.);
		}
		if (Tn > 4.0 + dTol && Tn < 4.0 + dt - dTol)
		{
			AddPeriod(4.0);
			AddAccel((dFac / 4.0) * m_dDampingFactor * dIR * m_dZ / 2.);
		}
		AddPeriod(Tn);
		AddAccel(dAh);
	}

	if (bOnlyCalc) return;
	CString strTemp; strTemp.Format(_T("UD(%1.2f)"), m_dUserDefSeisZoneFact);
	CString strZone[] = { _T("II(0.10)"),_T("III(0.16)"),_T("IV(0.24)"),_T("V(0.36)"),strTemp };
	CString strSoil[] = { _T("I(Rock/Hard)"),_T("II(Medium)"),_T("III(Soft)") };

	wchar_t buffer[SpfcUtil::BUFFER_SIZE];
	int result = std::swprintf(buffer, SpfcUtil::BUFFER_SIZE, _T("IRC:SP:114-2018: Zone=%s, Soil= %s, Damping=%5.2f, I=%4.2f, R=%4.2f"), strZone[m_nZone], strSoil[m_nSoil], m_dDamping, m_dI, m_dR);
	if (result >= 0)
	{
		SetDescript(buffer);
	}
	SetFuncName(L"IRC:SP:114-2018");
}

void CSpfcIRC2018Util::MakePOSpectrumData(double dDamping, double SRa, double SRv, CArray<double, double>& aPeriod, CArray<double, double>& aAccel)
{
	aPeriod.RemoveAll();
	aAccel.RemoveAll();

	double st = PRD_ST;
	double ed = m_dMaxPeriod * 2;
	int Step = PRD_NUM * 4;
	double Tn, dt, dSa, dAh;

	double dSaFac[] = { 1.00, 1.36, 1.67 };
	double dTInter[] = { 0.40, 0.55, 0.67 };
	double dTg = dTInter[m_nSoil];
	double dFac = dSaFac[m_nSoil];
	double dIR = min(m_dI / m_dR, 1.0);

	if (dDamping >= 0)
	{
		SRa = (3.21 - 0.68 * log(dDamping)) / 2.12;
		SRv = (2.31 - 0.41 * log(dDamping)) / 1.65;
	}

	dt = (ed - st) / Step;  // time interval	
	double dTol = dt / Step;  // Tolerance

	double dTg1 = (dFac / 2.5) * (SRv / SRa);  // modified Tg

	for (int i = 0; i <= Step; i++)
	{
		Tn = st + dt * i;

		if (Tn >= 0 && Tn <= 0.1)
			dSa = 1. + (2.5 * SRa - 1.) * 10 * Tn;
		else if (Tn > 0.1 && Tn < dTg1)
			dSa = 2.5 * SRa;
		else if (Tn >= dTg1 && Tn <= 4.0)
			dSa = SRv * dFac / Tn;
		else
			dSa = SRv * dFac / 4.0;

		dSa *= m_dDampingFactor;
		dAh = dIR * dSa * m_dZ / 2.;

		if (Tn > 0.1 + dTol && Tn < 0.1 + dt - dTol) // transition point		
		{
			aPeriod.Add(0.1);
			aAccel.Add(SRa * 2.5 * m_dDampingFactor * dIR * m_dZ / 2.);
		}
		if (Tn > dTg1 + dTol && Tn < dTg1 + dt - dTol) // transition point		
		{
			aPeriod.Add(dTg1);
			aAccel.Add(SRa * 2.5 * m_dDampingFactor * dIR * m_dZ / 2.);
		}
		if (Tn > dTg1 && Tn > 4.0 + dTol && Tn < 4.0 + dt - dTol)
		{
			aPeriod.Add(4.0);
			aAccel.Add(SRv * (dFac / 4.0) * m_dDampingFactor * dIR * m_dZ / 2.);
		}

		aPeriod.Add(Tn);
		aAccel.Add(dAh);
	}
}

void CSpfcIRC2018Util::SetParamFromCode(const T_SPFC_CODE& codeParam)
{
	m_nZone = codeParam.IRCSP114.nSeisZone;
	m_nSoil = codeParam.IRCSP114.nSoilType;
	m_dDamping = codeParam.IRCSP114.dDamping;
	m_dDampingFactor = codeParam.IRCSP114.dDampingFactor;
	m_dI = codeParam.IRCSP114.dIe;
	m_dR = codeParam.IRCSP114.dCoef;
	m_dMaxPeriod = codeParam.IRCSP114.dMaxPeriod;
	m_dUserDefSeisZoneFact = codeParam.IRCSP114.dUsrDefSeismicZone;
}

void CSpfcIRC2018Util::SetParamFromCalc()
{
	arrZone.back() = m_dUserDefSeisZoneFact;
	m_dZ = arrZone[m_nZone];
}


double CSpfcIRC2018Util::GetDampingFactorIS2002(double dDamping)
{
	double dDampingFactor = 0.;
	double aDamping[] = { 0., 2., 5., 7., 10., 15., 20., 25., 30. };
	double aFactor[] = { 3.20, 1.40, 1.00, 0.90, 0.80, 0.70, 0.60, 0.55, 0.50 };

	if (dDamping < 0) dDamping = 0.;
	if (dDamping > 30) dDamping = 30.;
	int Index = 8;
	for (Index = 8; Index >= 0; Index--)
	{
		if (dDamping >= aDamping[Index]) break;
	}
	double dDifRatio = 0.;
	if (Index < 8)
	{
		dDifRatio = (dDamping - aDamping[Index]) / (aDamping[Index + 1] - aDamping[Index]);
		dDampingFactor = aFactor[Index] - (aFactor[Index] - aFactor[Index + 1]) * dDifRatio;
	}
	else dDampingFactor = 0.50;

	return dDampingFactor;
}
