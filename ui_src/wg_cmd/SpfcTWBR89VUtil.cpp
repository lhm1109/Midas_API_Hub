#include "stdafx.h"
#include "SpfcTWBR89VUtil.h"

CSpfcTWBR89VUtil::CSpfcTWBR89VUtil()
{
	m_nSoil = 0;
	m_nZone = 0;

	m_dI = 1.0;
	m_dAlpha = 1.2;
	m_dMaxPeriod = PRD_ED;
	m_dRStar = 2.0;
	m_dDamping = 0.05;

	m_nR = 0;
	m_dR = 2.0 / 1.2;
	m_dZ = 0.33;
	m_bPOMode = FALSE;
}

bool CSpfcTWBR89VUtil::CheckValid()
{
	if (IsValid(m_dI, 0, 10) == false)
		return false;

	if (IsValid(m_dRStar, 0, 10) == false)
		return false;

	if (IsValid(m_dAlpha, 0, 10) == false)
		return false;

	if (m_dDamping < 0) { AfxMessageBox(_LS(IDS_CMD_SPECTRUM__DAMPING_VALUE)); return FALSE; }
	if (m_dMaxPeriod <= 0) { AfxMessageBox(_LS(IDS_CMD_SPECTRUM__MAX_PERIOD_GREATER_0)); return FALSE; }
	return TRUE;
}

void CSpfcTWBR89VUtil::MakeSpectrumData(bool bOnlyCalc)
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
	double Tn, dt, dSa, dSaA;

	dt = (ed - st) / Step;  // time interval
	double dTol = dt / Step;  // Tolerance	

	InitPeriod();
	InitAccel();

	for (int i = 0; i <= Step; i++)
	{
		Tn = st + dt * i;

		dSa = GetTaiwan86SpectralAcc(m_nSoil, m_dZ, m_dDamping, m_dI, m_dR, m_dAlpha, Tn);

		switch (m_nSoil)
		{
		case 0:  // Site I
			if (Tn > 0.03 + dTol && Tn < 0.03 + dt - dTol) // transition point		
			{
				AddPeriod(0.03);
				dSaA = GetTaiwan86SpectralAcc(m_nSoil, m_dZ, m_dDamping, m_dI, m_dR, m_dAlpha, 0.03);
				AddAccel(dSaA);
			}
			if (Tn > 0.10 + dTol && Tn < 0.10 + dt - dTol) // transition point		
			{
				AddPeriod(0.10);
				dSaA = GetTaiwan86SpectralAcc(m_nSoil, m_dZ, m_dDamping, m_dI, m_dR, m_dAlpha, 0.10);
				AddAccel(dSaA);
			}
			if (Tn > 0.15 + dTol && Tn < 0.15 + dt - dTol) // transition point		
			{
				AddPeriod(0.15);
				dSaA = GetTaiwan86SpectralAcc(m_nSoil, m_dZ, m_dDamping, m_dI, m_dR, m_dAlpha, 0.15);
				AddAccel(dSaA);
			}
			if (Tn > 0.242 + dTol && Tn < 0.242 + dt - dTol) // transition point		
			{
				AddPeriod(0.242);
				dSaA = GetTaiwan86SpectralAcc(m_nSoil, m_dZ, m_dDamping, m_dI, m_dR, m_dAlpha, 0.242);
				AddAccel(dSaA);
			}
			if (Tn > 0.288 + dTol && Tn < 0.288 + dt - dTol) // transition point		
			{
				AddPeriod(0.288);
				dSaA = GetTaiwan86SpectralAcc(m_nSoil, m_dZ, m_dDamping, m_dI, m_dR, m_dAlpha, 0.288);
				AddAccel(dSaA);
			}
			if (Tn > 0.333 + dTol && Tn < 0.333 + dt - dTol) // transition point		
			{
				AddPeriod(0.333);
				dSaA = GetTaiwan86SpectralAcc(m_nSoil, m_dZ, m_dDamping, m_dI, m_dR, m_dAlpha, 0.333);
				AddAccel(dSaA);
			}
			if (Tn > 1.139 + dTol && Tn < 1.139 + dt - dTol) // transition point		
			{
				AddPeriod(1.139);
				dSaA = GetTaiwan86SpectralAcc(m_nSoil, m_dZ, m_dDamping, m_dI, m_dR, m_dAlpha, 1.139);
				AddAccel(dSaA);
			}
			if (Tn > 1.315 + dTol && Tn < 1.315 + dt - dTol) // transition point		
			{
				AddPeriod(1.315);
				dSaA = GetTaiwan86SpectralAcc(m_nSoil, m_dZ, m_dDamping, m_dI, m_dR, m_dAlpha, 1.315);
				AddAccel(dSaA);
			}
			break;
		case 1:  // Site II
			if (Tn > 0.03 + dTol && Tn < 0.03 + dt - dTol) // transition point		
			{
				AddPeriod(0.03);
				dSaA = GetTaiwan86SpectralAcc(m_nSoil, m_dZ, m_dDamping, m_dI, m_dR, m_dAlpha, 0.03);
				AddAccel(dSaA);
			}
			if (Tn > 0.10 + dTol && Tn < 0.10 + dt - dTol) // transition point		
			{
				AddPeriod(0.10);
				dSaA = GetTaiwan86SpectralAcc(m_nSoil, m_dZ, m_dDamping, m_dI, m_dR, m_dAlpha, 0.10);
				AddAccel(dSaA);
			}
			if (Tn > 0.15 + dTol && Tn < 0.15 + dt - dTol) // transition point		
			{
				AddPeriod(0.15);
				dSaA = GetTaiwan86SpectralAcc(m_nSoil, m_dZ, m_dDamping, m_dI, m_dR, m_dAlpha, 0.15);
				AddAccel(dSaA);
			}
			if (Tn > 0.308 + dTol && Tn < 0.308 + dt - dTol) // transition point		
			{
				AddPeriod(0.308);
				dSaA = GetTaiwan86SpectralAcc(m_nSoil, m_dZ, m_dDamping, m_dI, m_dR, m_dAlpha, 0.308);
				AddAccel(dSaA);
			}
			if (Tn > 0.403 + dTol && Tn < 0.403 + dt - dTol) // transition point		
			{
				AddPeriod(0.403);
				dSaA = GetTaiwan86SpectralAcc(m_nSoil, m_dZ, m_dDamping, m_dI, m_dR, m_dAlpha, 0.403);
				AddAccel(dSaA);
			}
			if (Tn > 0.465 + dTol && Tn < 0.465 + dt - dTol) // transition point		
			{
				AddPeriod(0.465);
				dSaA = GetTaiwan86SpectralAcc(m_nSoil, m_dZ, m_dDamping, m_dI, m_dR, m_dAlpha, 0.465);
				AddAccel(dSaA);
			}
			if (Tn > 1.592 + dTol && Tn < 1.592 + dt - dTol) // transition point		
			{
				AddPeriod(1.592);
				dSaA = GetTaiwan86SpectralAcc(m_nSoil, m_dZ, m_dDamping, m_dI, m_dR, m_dAlpha, 1.592);
				AddAccel(dSaA);
			}
			break;
		case 2:  // Site III
			if (Tn > 0.03 + dTol && Tn < 0.03 + dt - dTol) // transition point		
			{
				AddPeriod(0.03);
				dSaA = GetTaiwan86SpectralAcc(m_nSoil, m_dZ, m_dDamping, m_dI, m_dR, m_dAlpha, 0.03);
				AddAccel(dSaA);
			}
			if (Tn > 0.10 + dTol && Tn < 0.10 + dt - dTol) // transition point		
			{
				AddPeriod(0.10);
				dSaA = GetTaiwan86SpectralAcc(m_nSoil, m_dZ, m_dDamping, m_dI, m_dR, m_dAlpha, 0.10);
				AddAccel(dSaA);
			}
			if (Tn > 0.20 + dTol && Tn < 0.20 + dt - dTol) // transition point		
			{
				AddPeriod(0.20);
				dSaA = GetTaiwan86SpectralAcc(m_nSoil, m_dZ, m_dDamping, m_dI, m_dR, m_dAlpha, 0.20);
				AddAccel(dSaA);
			}
			if (Tn > 0.406 + dTol && Tn < 0.406 + dt - dTol) // transition point		
			{
				AddPeriod(0.406);
				dSaA = GetTaiwan86SpectralAcc(m_nSoil, m_dZ, m_dDamping, m_dI, m_dR, m_dAlpha, 0.406);
				AddAccel(dSaA);
			}
			if (Tn > 0.530 + dTol && Tn < 0.530 + dt - dTol) // transition point		
			{
				AddPeriod(0.530);
				dSaA = GetTaiwan86SpectralAcc(m_nSoil, m_dZ, m_dDamping, m_dI, m_dR, m_dAlpha, 0.530);
				AddAccel(dSaA);
			}
			if (Tn > 0.611 + dTol && Tn < 0.611 + dt - dTol) // transition point		
			{
				AddPeriod(0.611);
				dSaA = GetTaiwan86SpectralAcc(m_nSoil, m_dZ, m_dDamping, m_dI, m_dR, m_dAlpha, 0.611);
				AddAccel(dSaA);
			}
			if (Tn > 2.093 + dTol && Tn < 2.093 + dt - dTol) // transition point		
			{
				AddPeriod(2.093);
				dSaA = GetTaiwan86SpectralAcc(m_nSoil, m_dZ, m_dDamping, m_dI, m_dR, m_dAlpha, 2.093);
				AddAccel(dSaA);
			}
			break;
		case 3:  // Taipei Basin
			if (Tn > 0.03 + dTol && Tn < 0.03 + dt - dTol) // transition point		
			{
				AddPeriod(0.03);
				dSaA = GetTaiwan86SpectralAcc(m_nSoil, m_dZ, m_dDamping, m_dI, m_dR, m_dAlpha, 0.03);
				AddAccel(dSaA);
			}
			if (Tn > 0.10 + dTol && Tn < 0.10 + dt - dTol) // transition point		
			{
				AddPeriod(0.10);
				dSaA = GetTaiwan86SpectralAcc(m_nSoil, m_dZ, m_dDamping, m_dI, m_dR, m_dAlpha, 0.10);
				AddAccel(dSaA);
			}
			if (Tn > 0.20 + dTol && Tn < 0.20 + dt - dTol) // transition point		
			{
				AddPeriod(0.20);
				dSaA = GetTaiwan86SpectralAcc(m_nSoil, m_dZ, m_dDamping, m_dI, m_dR, m_dAlpha, 0.20);
				AddAccel(dSaA);
			}
			if (Tn > 0.76 + dTol && Tn < 0.76 + dt - dTol) // transition point		
			{
				AddPeriod(0.76);
				dSaA = GetTaiwan86SpectralAcc(m_nSoil, m_dZ, m_dDamping, m_dI, m_dR, m_dAlpha, 0.76);
				AddAccel(dSaA);
			}
			if (Tn > 1.32 + dTol && Tn < 1.32 + dt - dTol) // transition point		
			{
				AddPeriod(1.32);
				dSaA = GetTaiwan86SpectralAcc(m_nSoil, m_dZ, m_dDamping, m_dI, m_dR, m_dAlpha, 1.32);
				AddAccel(dSaA);
			}
			if (Tn > 1.4 + dTol && Tn < 1.4 + dt - dTol) // transition point		
			{
				AddPeriod(1.4);
				dSaA = GetTaiwan86SpectralAcc(m_nSoil, m_dZ, m_dDamping, m_dI, m_dR, m_dAlpha, 1.4);
				AddAccel(dSaA);
			}
			if (Tn > 3.3 + dTol && Tn < 3.3 + dt - dTol) // transition point		
			{
				AddPeriod(3.3);
				dSaA = GetTaiwan86SpectralAcc(m_nSoil, m_dZ, m_dDamping, m_dI, m_dR, m_dAlpha, 3.3);
				AddAccel(dSaA);
			}
			break;
		}
		AddPeriod(Tn);
		AddAccel(dSa);
	}
	if (bOnlyCalc) return;

	CString strZone[] = { _T("I(0.22g)"),_T("II(0.153g)") };
	CString strSoil[] = { _T("I(Stiff)"),_T("II(Medium)"),_T("III(Soft)"),_T("Taipei Basin") };

	wchar_t buffer[SpfcUtil::BUFFER_SIZE];
	int result = std::swprintf(buffer, SpfcUtil::BUFFER_SIZE, _T("TaiwanBrg(89)V: Z=%s, S=%s, I=%4.2f, A=%4.2f,  R=%4.2f,Damp=%4.2f"), strZone[m_nZone], strSoil[m_nSoil], m_dI, m_dAlpha, m_dRStar, m_dDamping);
	if (result >= 0)
	{
		SetDescript(buffer);
	}
	SetFuncName(L"TaiwanBrg(89)V");
}

void CSpfcTWBR89VUtil::MakePOSpectrumData(double dDamping, double SRa, double SRv, CArray<double, double>& aPeriod, CArray<double, double>& aAccel)
{
	aPeriod.RemoveAll();
	aAccel.RemoveAll();

	double st = PRD_ST;
	double ed = m_dMaxPeriod * 2;
	int Step = PRD_NUM * 4;
	double Tn, dt, dSa;

	if (dDamping >= 0)
	{
		SRa = (3.21 - 0.68 * log(dDamping)) / 2.12;
		SRv = (2.31 - 0.41 * log(dDamping)) / 1.65;
	}

	dt = (ed - st) / Step;  // time interval	
	double dTol = dt / Step;  // Tolerance

	double dTg = GetTaiwan86ModifiedTg(m_nSoil, m_dR, SRa, SRv);

	// MNET:2689-GSJI-20070405
	// 1.4 * m_dAlpha 로 나누지 않도록 처리(대만대리점 요청)
	for (int i = 0; i <= Step; i++)
	{
		Tn = st + dt * i;

		switch (m_nSoil)
		{
		case 0:  // Site I
			if (Tn >= 0.03 && Tn < 0.03 + dt - dTol)
			{
				aPeriod.Add(0.03);
				aAccel.Add(SRa * GetTaiwan86POSpectralAcc(m_nSoil, m_dZ, m_dI, m_dR, m_dAlpha, 0.03, dTg)); // Tn 대신 0.03 (Transition 일때)
			}
			if (Tn >= 0.10 && Tn < 0.10 + dt - dTol)
			{
				aPeriod.Add(0.10);
				aAccel.Add(SRa * GetTaiwan86POSpectralAcc(m_nSoil, m_dZ, m_dI, m_dR, m_dAlpha, 0.10, dTg)); // Tn 대신 0.03 (Transition 일때)
			}
			if (Tn > dTg + dTol && Tn < dTg + dt - dTol) // transition point		
			{
				aPeriod.Add(dTg);
				aAccel.Add(SRv * GetTaiwan86POSpectralAcc(m_nSoil, m_dZ, m_dI, m_dR, m_dAlpha, dTg, dTg)); // Tn 대신 dTg (Transition 일때)
			}
			if (Tn >= 1.139 && Tn < 1.139 + dt - dTol)
			{
				aPeriod.Add(1.139);
				aAccel.Add(SRv * GetTaiwan86POSpectralAcc(m_nSoil, m_dZ, m_dI, m_dR, m_dAlpha, 1.139, dTg)); // Tn 대신 0.03 (Transition 일때)
			}
			break;
		case 1:  // Site II
			if (Tn >= 0.03 && Tn < 0.03 + dt - dTol)
			{
				aPeriod.Add(0.03);
				aAccel.Add(SRa * GetTaiwan86POSpectralAcc(m_nSoil, m_dZ, m_dI, m_dR, m_dAlpha, 0.03, dTg)); // Tn 대신 0.03 (Transition 일때)
			}
			if (Tn >= 0.10 && Tn < 0.10 + dt - dTol)
			{
				aPeriod.Add(0.10);
				aAccel.Add(SRa * GetTaiwan86POSpectralAcc(m_nSoil, m_dZ, m_dI, m_dR, m_dAlpha, 0.10, dTg)); // Tn 대신 0.03 (Transition 일때)
			}
			if (Tn > dTg + dTol && Tn < dTg + dt - dTol) // transition point		
			{
				aPeriod.Add(dTg);
				aAccel.Add(SRv * GetTaiwan86POSpectralAcc(m_nSoil, m_dZ, m_dI, m_dR, m_dAlpha, dTg, dTg)); // Tn 대신 dTg (Transition 일때)
			}
			if (Tn >= 1.592 && Tn < 1.592 + dt - dTol)
			{
				aPeriod.Add(1.592);
				aAccel.Add(SRv * GetTaiwan86POSpectralAcc(m_nSoil, m_dZ, m_dI, m_dR, m_dAlpha, 1.592, dTg)); // Tn 대신 0.03 (Transition 일때)
			}
			break;
		case 2:  // Site III
			if (Tn >= 0.03 && Tn < 0.03 + dt - dTol)
			{
				aPeriod.Add(0.03);
				aAccel.Add(SRa * GetTaiwan86POSpectralAcc(m_nSoil, m_dZ, m_dI, m_dR, m_dAlpha, 0.03, dTg)); // Tn 대신 0.03 (Transition 일때)
			}
			if (Tn >= 0.10 && Tn < 0.10 + dt - dTol)
			{
				aPeriod.Add(0.10);
				aAccel.Add(SRa * GetTaiwan86POSpectralAcc(m_nSoil, m_dZ, m_dI, m_dR, m_dAlpha, 0.10, dTg)); // Tn 대신 0.03 (Transition 일때)
			}
			if (Tn > dTg + dTol && Tn < dTg + dt - dTol) // transition point		
			{
				aPeriod.Add(dTg);
				aAccel.Add(SRv * GetTaiwan86POSpectralAcc(m_nSoil, m_dZ, m_dI, m_dR, m_dAlpha, dTg, dTg)); // Tn 대신 dTg (Transition 일때)
			}
			if (Tn >= 2.093 && Tn < 2.093 + dt - dTol)
			{
				aPeriod.Add(2.093);
				aAccel.Add(SRv * GetTaiwan86POSpectralAcc(m_nSoil, m_dZ, m_dI, m_dR, m_dAlpha, 2.093, dTg)); // Tn 대신 0.03 (Transition 일때)
			}
			break;
		case 3:  // Site Taipei basin
			if (Tn >= 0.03 && Tn < 0.03 + dt - dTol)
			{
				aPeriod.Add(0.03);
				aAccel.Add(SRa * GetTaiwan86POSpectralAcc(m_nSoil, m_dZ, m_dI, m_dR, m_dAlpha, 0.03, dTg)); // Tn 대신 0.03 (Transition 일때)
			}
			if (Tn >= 0.10 && Tn < 0.10 + dt - dTol)
			{
				aPeriod.Add(0.10);
				aAccel.Add(SRa * GetTaiwan86POSpectralAcc(m_nSoil, m_dZ, m_dI, m_dR, m_dAlpha, 0.10, dTg)); // Tn 대신 0.03 (Transition 일때)
			}
			if (Tn > dTg + dTol && Tn < dTg + dt - dTol) // transition point		
			{
				aPeriod.Add(dTg);
				aAccel.Add(SRv * GetTaiwan86POSpectralAcc(m_nSoil, m_dZ, m_dI, m_dR, m_dAlpha, dTg, dTg)); // Tn 대신 dTg (Transition 일때)
			}
			if (Tn >= 3.3 && Tn < 3.3 + dt - dTol)
			{
				aPeriod.Add(3.3);
				aAccel.Add(SRv * GetTaiwan86POSpectralAcc(m_nSoil, m_dZ, m_dI, m_dR, m_dAlpha, 3.3, dTg)); // Tn 대신 0.03 (Transition 일때)
			}
			break;
		}

		if (Tn >= 0 && Tn <= dTg)
			dSa = SRa * GetTaiwan86POSpectralAcc(m_nSoil, m_dZ, m_dI, m_dR, m_dAlpha, Tn, dTg);
		else if (Tn > dTg)
			dSa = SRv * GetTaiwan86POSpectralAcc(m_nSoil, m_dZ, m_dI, m_dR, m_dAlpha, Tn, dTg);


		aPeriod.Add(Tn);
		aAccel.Add(dSa);
	}
}

void CSpfcTWBR89VUtil::SetParamFromCode(const T_SPFC_CODE& codeParam)
{
	m_nZone = codeParam.TAIWAN89V.nSeisZone;
	m_nSoil = codeParam.TAIWAN89V.nSoilType;
	m_dI = codeParam.TAIWAN89V.dIe;
	m_dAlpha = codeParam.TAIWAN89V.dAlpha;
	m_dMaxPeriod = codeParam.TAIWAN89V.dMaxPeriod;
	m_dRStar = codeParam.TAIWAN89V.dCoef;
	m_dDamping = codeParam.TAIWAN89V.dDamping;
}

void CSpfcTWBR89VUtil::SetParamFromCalc()
{
	m_dZ = TWBR89V::ZONE[m_nZone];

	m_dR = m_dRStar / 1.2;
	if (m_bPOMode == true)
		m_dR = 1.0;

	// API에서 대응되지 않는 m_nR 값 정의
	SetNumRFromDoubleR();
}

double CSpfcTWBR89VUtil::GetTaiwan86CFactor(int nSoil, double dDamping, double dT)
{
	double dC;
	double dCd = 1.5 / (40.0 * dDamping + 1.0) + 0.5;
	if (dT < 0.03) dCd = 1.0;
	switch (nSoil)
	{
	case 0:
	case 1:
		if (dT >= 0.030 && dT < 0.150) dCd = 1.0 + dCd * (dT - 0.03) / 0.12;
		break;
	case 2:
	case 3:
		if (dT >= 0.030 && dT < 0.200) dCd = 1.0 + dCd * (dT - 0.03) / 0.17;
		break;
	}

	switch (nSoil)
	{
	case 0:  // Soil I
	{
		if (dT <= 0.030) dC = 1.0;
		else if (dT >= 0.030 && dT < 0.100) dC = 25 * dT + 0.25;
		else if (dT >= 0.100 && dT < 0.288) dC = 2.75;
		else if (dT >= 0.288 && dT < 1.139) dC = 1.2 / pow(dT, 2. / 3.);
		else dC = 1.1;
	}
	break;
	case 1:  // Soil II
	{
		if (dT <= 0.030) dC = 1.0;
		else if (dT >= 0.030 && dT < 0.100) dC = 25 * dT + 0.25;
		else if (dT >= 0.100 && dT < 0.403) dC = 2.75;
		else if (dT >= 0.403 && dT < 1.592) dC = 1.5 / pow(dT, 2. / 3.);
		else dC = 1.1;
	}
	break;
	case 2:  // Soil III
	{
		if (dT <= 0.030) dC = 1.0;
		else if (dT >= 0.030 && dT < 0.100) dC = 25 * dT + 0.25;
		else if (dT >= 0.100 && dT < 0.530) dC = 2.75;
		else if (dT >= 0.530 && dT < 2.093) dC = 1.8 / pow(dT, 2. / 3.);
		else dC = 1.1;
	}
	break;
	case 3:  // Taipei Basin
	{
		if (dT <= 0.030) dC = 1.0;
		else if (dT >= 0.030 && dT < 0.100) dC = 21.43 * dT + 0.357;
		else if (dT >= 0.100 && dT < 1.320) dC = 2.5;
		else if (dT >= 1.320 && dT < 3.300) dC = 3.3 / dT;
		else dC = 1.0;
	}
	break;
	}
	return dC * dCd;
}

double CSpfcTWBR89VUtil::GetTaiwan86POCFactor(int nSoil, double dDamping, double dT, double dTg)
{
	double dC;
	double dCd = 1.5 / (40.0 * dDamping + 1.0) + 0.5;
	if (dT < 0.03) dCd = 1.0;
	/*
	switch(nSoil)
	{
	case 0:
	case 1:
		if(dT >= 0.030 && dT < 0.150) dCd = 1.0 + dCd * (dT - 0.03) / 0.12;
		break;
	case 2:
	case 3:
		if(dT >= 0.030 && dT < 0.200) dCd = 1.0 + dCd * (dT - 0.03) / 0.17;
		break;
	}
	*/

	switch (nSoil)
	{
	case 0:  // Soil I
	{
		if (dT <= 0.030) dC = 1.0;
		else if (dT >= 0.030 && dT < 0.100) dC = 25 * dT + 0.25;
		else if (dT >= 0.100 && dT < dTg) dC = 2.75;
		else if (dT >= dTg && dT < 1.139) dC = 1.2 / pow(dT, 2. / 3.);
		else dC = 1.1;
	}
	break;
	case 1:  // Soil II
	{
		if (dT <= 0.030) dC = 1.0;
		else if (dT >= 0.030 && dT < 0.100) dC = 25 * dT + 0.25;
		else if (dT >= 0.100 && dT < dTg) dC = 2.75;
		else if (dT >= dTg && dT < 1.592) dC = 1.5 / pow(dT, 2. / 3.);
		else dC = 1.1;
	}
	break;
	case 2:  // Soil III
	{
		if (dT <= 0.030) dC = 1.0;
		else if (dT >= 0.030 && dT < 0.100) dC = 25 * dT + 0.25;
		else if (dT >= 0.100 && dT < dTg) dC = 2.75;
		else if (dT >= dTg && dT < 2.093) dC = 1.8 / pow(dT, 2. / 3.);
		else dC = 1.1;
	}
	break;
	case 3:  // Taipei Basin
	{
		if (dT <= 0.030) dC = 1.0;
		else if (dT >= 0.030 && dT < 0.100) dC = 21.43 * dT + 0.357;
		else if (dT >= 0.100 && dT < dTg) dC = 2.5;
		else if (dT >= dTg && dT < 3.300) dC = 3.3 / dT;
		else dC = 1.0;
	}
	break;
	}
	return dC * dCd;
}

double CSpfcTWBR89VUtil::GetTaiwan86FuFactor(int nSoil, double dR, double dT)
{
	double dFu;
	if (dR < 0.) dR = 0.;
	double dRa = 1.0 + (dR - 1) / 1.5;
	double dSRa = sqrt(2 * dRa - 1.);
	switch (nSoil)
	{
	case 0:  // Soil I
	{
		if (dT >= 0.333) dFu = dRa;
		else if (dT >= 0.242 && dT < 0.333) dFu = dSRa + (dRa - dSRa) * (dT - 0.242) / 0.091;
		else if (dT >= 0.150 && dT < 0.242) dFu = dSRa;
		else if (dT >= 0.030 && dT < 0.150) dFu = dSRa + (dSRa - 1.) * (dT - 0.15) / 0.12;
		else dFu = 1.0;
	}
	break;
	case 1:  // Soil II
	{
		if (dT >= 0.465) dFu = dRa;
		else if (dT >= 0.308 && dT < 0.465) dFu = dSRa + (dRa - dSRa) * (dT - 0.308) / 0.157;
		else if (dT >= 0.150 && dT < 0.308) dFu = dSRa;
		else if (dT >= 0.030 && dT < 0.150) dFu = dSRa + (dSRa - 1.) * (dT - 0.15) / 0.12;
		else dFu = 1.0;
	}
	break;
	case 2:  // Soil III
	{
		if (dT >= 0.611) dFu = dRa;
		else if (dT >= 0.406 && dT < 0.611) dFu = dSRa + (dRa - dSRa) * (dT - 0.406) / 0.205;
		else if (dT >= 0.200 && dT < 0.406) dFu = dSRa;
		else if (dT >= 0.030 && dT < 0.200) dFu = dSRa + (dSRa - 1.) * (dT - 0.20) / 0.17;
		else dFu = 1.0;
	}
	break;
	case 3:  // Taipei Basin
	{
		if (dT >= 1.32) dFu = dRa;
		else if (dT >= 0.76 && dT < 1.32) dFu = dSRa + (dRa - dSRa) * (dT - 0.76) / 0.56;
		else if (dT >= 0.20 && dT < 0.76) dFu = dSRa;
		else if (dT >= 0.03 && dT < 0.20) dFu = dSRa + (dSRa - 1.) * (dT - 0.20) / 0.17;
		else dFu = 1.0;
	}
	break;
	}

	return dFu;
}

double CSpfcTWBR89VUtil::GetTaiwan86SpectralAcc(int nSoil, double dZ, double dDamping, double dI, double dR, double dAlpha, double dT)
{
	double dC = GetTaiwan86CFactor(nSoil, dDamping, dT);
	double dFu = GetTaiwan86FuFactor(nSoil, dR, dT);
	double dMax[] = { 1.2, 1.1, 1.0 };
	double dCFum = min(dMax[m_nR], dC / dFu);
	double dZd = max(dZ * dI / (1.2 * dAlpha), dZ * dI * dFu / (3.0 * dAlpha));
	double dSa = dZd * dCFum;

	return dSa;
}

double CSpfcTWBR89VUtil::GetTaiwan86ModifiedTg(int nSoil, double dR, double SRa, double SRv)
{
	double dRa = 1.0 + (dR - 1) / 1.5;
	double dTg;
	/*
		if(dRa > 2.5)
		{
			switch(nSoil)
			{
			case 0:
				dTg = pow((1.2 * SRv) / (2.5 * SRa), 1.5);
				break;
			case 1:
				dTg = pow((1.5 * SRv) / (2.5 * SRa), 1.5);
				break;
			case 2:
				dTg = pow((1.8 * SRv) / (2.5 * SRa), 1.5);
				break;
			case 3:
				dTg = pow((3.3 * SRv) / (2.5 * SRa), 1.0);
				break;
			}
		}
		else
		{
			switch(nSoil)
			{
			case 0:
				dTg = pow((1.2 * SRv) / (dRa * SRa), 1.5);
				break;
			case 1:
				dTg = pow((1.5 * SRv) / (dRa * SRa), 1.5);
				break;
			case 2:
				dTg = pow((1.8 * SRv) / (dRa * SRa), 1.5);
				break;
			case 3:
				dTg = pow((3.3 * SRv) / (dRa * SRa), 1.0);
				break;
			}
		}
	*/
	switch (nSoil)
	{
	case 0:
		dTg = pow((1.2 * SRv) / (2.75 * SRa), 1.5);
		break;
	case 1:
		dTg = pow((1.5 * SRv) / (2.75 * SRa), 1.5);
		break;
	case 2:
		dTg = pow((1.8 * SRv) / (2.75 * SRa), 1.5);
		break;
	case 3:
		dTg = pow((3.3 * SRv) / (2.5 * SRa), 1.0);
		break;
	}

	return dTg;
}

double CSpfcTWBR89VUtil::GetTaiwan86POSpectralAcc(int nSoil, double dZ, double dI, double dR, double dAlpha, double dT, double dTg)
{
	//double dRa = 1.0 + (dR - 1) / 1.5;

	// MNET:2689-GSJI-20070312
	//double dFactor = dZ * dI / (3.5 * dAlpha);  // before
	double dFactor = dZ * dI;  // after

	double dDamping = 0.05;
	double dSa = GetTaiwan86POCFactor(nSoil, dDamping, dT, dTg);
	/*
	if(dRa > 2.5)
	{
		dSa = GetTaiwan86CFactor(nSoil, dDamping, dT);
	}
	else
	{
		double dC  = GetTaiwan86CFactor(nSoil, dDamping, dT);
		double dFu = GetTaiwan86FuFactor(nSoil, dR, dT);
		dSa = 2.5 * dC / dFu;
	}*/
	dSa = dFactor * dSa;
	return dSa;
}


void CSpfcTWBR89VUtil::SetNumRFromDoubleR()
{
	if (m_dRStar == 2.0 && m_nR == 0)
		return;

	if (m_dRStar == 3.0 && m_nR == 1)
		return;

	if (m_dRStar == 5.0 && m_nR == 2)
		return;

	if (m_dRStar == 2.0)
		m_nR = 0;
	else if (m_dRStar == 3.0)
		m_nR = 1;
	else if (m_dRStar == 5.0)
		m_nR = 2;
	else
	{
		// TODO - API에서 2.0, 3.0, 5.0 아닌 값을 넣었을 경우 보완 처리 필요
		ASSERT(false);
		m_nR = 0;
	}
}
