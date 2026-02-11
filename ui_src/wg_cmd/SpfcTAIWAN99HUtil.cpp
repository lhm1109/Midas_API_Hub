#include "stdafx.h"
#include "SpfcTAIWAN99HUtil.h"

CSpfcTAIWAN99HUtil::CSpfcTAIWAN99HUtil()
{
	m_nSoil = 0;
	m_nZone = 0;
	m_dMaxPeriod = PRD_ED;

	m_dZ = 0.33;
	m_dI = 1.0;
	m_dR = 1.6;
	m_dAlpha = 1.2;
	m_dDamping = 0.05;
}

bool CSpfcTAIWAN99HUtil::CheckValid()
{
	if (IsValid(m_dI, 0, 10) == false)
		return false;

	if (IsValid(m_dR, 0, 10) == false)
		return false;

	if (IsValid(m_dAlpha, 0, 10) == false)
		return false;

	if (m_dDamping < 0) { AfxMessageBox(_LS(IDS_CMD_SPECTRUM__DAMPING_VALUE)); return FALSE; }
	if (m_dMaxPeriod <= 0) { AfxMessageBox(_LS(IDS_CMD_SPECTRUM__MAX_PERIOD_GREATER_0)); return FALSE; }
	return TRUE;
}

void CSpfcTAIWAN99HUtil::MakeSpectrumData(bool bOnlyCalc)
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
			if (Tn > 0.333 + dTol && Tn < 0.333 + dt - dTol) // transition point		
			{
				AddPeriod(0.333);
				dSaA = GetTaiwan86SpectralAcc(m_nSoil, m_dZ, m_dDamping, m_dI, m_dR, m_dAlpha, 0.333);
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
			if (Tn > 0.465 + dTol && Tn < 0.465 + dt - dTol) // transition point		
			{
				AddPeriod(0.465);
				dSaA = GetTaiwan86SpectralAcc(m_nSoil, m_dZ, m_dDamping, m_dI, m_dR, m_dAlpha, 0.465);
				AddAccel(dSaA);
			}
			if (Tn > 1.837 + dTol && Tn < 1.837 + dt - dTol) // transition point		
			{
				AddPeriod(1.837);
				dSaA = GetTaiwan86SpectralAcc(m_nSoil, m_dZ, m_dDamping, m_dI, m_dR, m_dAlpha, 1.837);
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
			if (Tn > 0.611 + dTol && Tn < 0.611 + dt - dTol) // transition point		
			{
				AddPeriod(0.611);
				dSaA = GetTaiwan86SpectralAcc(m_nSoil, m_dZ, m_dDamping, m_dI, m_dR, m_dAlpha, 0.611);
				AddAccel(dSaA);
			}
			if (Tn > 2.415 + dTol && Tn < 2.415 + dt - dTol) // transition point		
			{
				AddPeriod(2.415);
				dSaA = GetTaiwan86SpectralAcc(m_nSoil, m_dZ, m_dDamping, m_dI, m_dR, m_dAlpha, 2.415);
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
			if (Tn > 0.20 + dTol && Tn < 0.20 + dt - dTol) // transition point		
			{
				AddPeriod(0.20);
				dSaA = GetTaiwan86SpectralAcc(m_nSoil, m_dZ, m_dDamping, m_dI, m_dR, m_dAlpha, 0.20);
				AddAccel(dSaA);
			}
			if (Tn > 0.8 + dTol && Tn < 0.8 + dt - dTol) // transition point		
			{
				AddPeriod(0.8);
				dSaA = GetTaiwan86SpectralAcc(m_nSoil, m_dZ, m_dDamping, m_dI, m_dR, m_dAlpha, 0.8);
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

	CString strZone[] = { _T("I(0.33g)"),_T("II(0.23g)") };
	CString strSoil[] = { _T("I(Stiff)"),_T("II(Medium)"),_T("III(Soft)"),_T("Taipei Basin") };

	wchar_t buffer[SpfcUtil::BUFFER_SIZE];
	int result = std::swprintf(buffer, SpfcUtil::BUFFER_SIZE, _T("Taiwan(1999)H: Z=%s, S=%s, I=%4.2f, A=%4.2f, R=%4.2f, Damp=%4.2f"), strZone[m_nZone], strSoil[m_nSoil], m_dI, m_dAlpha, m_dR, m_dDamping);
	if (result >= 0)
	{
		SetDescript(buffer);
	}
	SetFuncName(L"Taiwan(1999)H");
}

void CSpfcTAIWAN99HUtil::MakePOSpectrumData(double dDamping, double SRa, double SRv, CArray<double, double>& aPeriod, CArray<double, double>& aAccel)
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
			if (Tn >= 0.15 && Tn < 0.15 + dt - dTol)
			{
				aPeriod.Add(0.15);
				aAccel.Add(SRa * GetTaiwan86POSpectralAcc(m_nSoil, m_dZ, m_dI, m_dR, m_dAlpha, 0.15, dTg)); // Tn 대신 0.03 (Transition 일때)
			}
			if (Tn > dTg + dTol && Tn < dTg + dt - dTol) // transition point		
			{
				aPeriod.Add(dTg);
				aAccel.Add(SRv * GetTaiwan86POSpectralAcc(m_nSoil, m_dZ, m_dI, m_dR, m_dAlpha, dTg, dTg)); // Tn 대신 dTg (Transition 일때)
			}
			if (Tn >= 1.315 && Tn < 1.315 + dt - dTol)
			{
				aPeriod.Add(1.315);
				aAccel.Add(SRv * GetTaiwan86POSpectralAcc(m_nSoil, m_dZ, m_dI, m_dR, m_dAlpha, 1.315, dTg)); // Tn 대신 0.03 (Transition 일때)
			}
			break;
		case 1:  // Site II
			if (Tn >= 0.03 && Tn < 0.03 + dt - dTol)
			{
				aPeriod.Add(0.03);
				aAccel.Add(SRa * GetTaiwan86POSpectralAcc(m_nSoil, m_dZ, m_dI, m_dR, m_dAlpha, 0.03, dTg)); // Tn 대신 0.03 (Transition 일때)
			}
			if (Tn >= 0.15 && Tn < 0.15 + dt - dTol)
			{
				aPeriod.Add(0.15);
				aAccel.Add(SRa * GetTaiwan86POSpectralAcc(m_nSoil, m_dZ, m_dI, m_dR, m_dAlpha, 0.15, dTg)); // Tn 대신 0.03 (Transition 일때)
			}
			if (Tn > dTg + dTol && Tn < dTg + dt - dTol) // transition point		
			{
				aPeriod.Add(dTg);
				aAccel.Add(SRv * GetTaiwan86POSpectralAcc(m_nSoil, m_dZ, m_dI, m_dR, m_dAlpha, dTg, dTg)); // Tn 대신 dTg (Transition 일때)
			}
			if (Tn >= 1.837 && Tn < 1.837 + dt - dTol)
			{
				aPeriod.Add(1.837);
				aAccel.Add(SRv * GetTaiwan86POSpectralAcc(m_nSoil, m_dZ, m_dI, m_dR, m_dAlpha, 1.837, dTg)); // Tn 대신 0.03 (Transition 일때)
			}
			break;
		case 2:  // Site III
			if (Tn >= 0.03 && Tn < 0.03 + dt - dTol)
			{
				aPeriod.Add(0.03);
				aAccel.Add(SRa * GetTaiwan86POSpectralAcc(m_nSoil, m_dZ, m_dI, m_dR, m_dAlpha, 0.03, dTg)); // Tn 대신 0.03 (Transition 일때)
			}
			if (Tn >= 0.20 && Tn < 0.20 + dt - dTol)
			{
				aPeriod.Add(0.20);
				aAccel.Add(SRa * GetTaiwan86POSpectralAcc(m_nSoil, m_dZ, m_dI, m_dR, m_dAlpha, 0.20, dTg)); // Tn 대신 0.03 (Transition 일때)
			}
			if (Tn > dTg + dTol && Tn < dTg + dt - dTol) // transition point		
			{
				aPeriod.Add(dTg);
				aAccel.Add(SRv * GetTaiwan86POSpectralAcc(m_nSoil, m_dZ, m_dI, m_dR, m_dAlpha, dTg, dTg)); // Tn 대신 dTg (Transition 일때)
			}
			if (Tn >= 2.415 && Tn < 2.415 + dt - dTol)
			{
				aPeriod.Add(2.415);
				aAccel.Add(SRv * GetTaiwan86POSpectralAcc(m_nSoil, m_dZ, m_dI, m_dR, m_dAlpha, 2.415, dTg)); // Tn 대신 0.03 (Transition 일때)
			}
			break;
		case 3:  // Site Taipei basin
			if (Tn >= 0.03 && Tn < 0.03 + dt - dTol)
			{
				aPeriod.Add(0.03);
				aAccel.Add(SRa * GetTaiwan86POSpectralAcc(m_nSoil, m_dZ, m_dI, m_dR, m_dAlpha, 0.03, dTg)); // Tn 대신 0.03 (Transition 일때)
			}
			if (Tn >= 0.20 && Tn < 0.20 + dt - dTol)
			{
				aPeriod.Add(0.20);
				aAccel.Add(SRa * GetTaiwan86POSpectralAcc(m_nSoil, m_dZ, m_dI, m_dR, m_dAlpha, 0.20, dTg)); // Tn 대신 0.03 (Transition 일때)
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

void CSpfcTAIWAN99HUtil::SetParamFromCode(const T_SPFC_CODE& codeParam)
{
	m_nZone = codeParam.TAIWAN99H.nSeisZone;
	m_nSoil = codeParam.TAIWAN99H.nSoilType;
	m_dI = codeParam.TAIWAN99H.dIe;
	m_dAlpha = codeParam.TAIWAN99H.dAlpha;
	m_dMaxPeriod = codeParam.TAIWAN99H.dMaxPeriod;
	m_dR = codeParam.TAIWAN99H.dCoef;
	m_dDamping = codeParam.TAIWAN99H.dDamping;
}

void CSpfcTAIWAN99HUtil::SetParamFromCalc()
{
	m_dZ = TW99H::ZONE[m_nZone];
}

double CSpfcTAIWAN99HUtil::GetTaiwan86CFactor(int nSoil, double dDamping, double dT)
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
		else if (dT >= 0.030 && dT < 0.150) dC = 12.5 * dT + 0.625;
		else if (dT >= 0.150 && dT < 0.333) dC = 2.5;
		else if (dT >= 0.333 && dT < 1.315) dC = 1.2 / pow(dT, 2. / 3.);
		else dC = 1.0;
	}
	break;
	case 1:  // Soil II
	{
		if (dT <= 0.030) dC = 1.0;
		else if (dT >= 0.030 && dT < 0.150) dC = 12.5 * dT + 0.625;
		else if (dT >= 0.150 && dT < 0.465) dC = 2.5;
		else if (dT >= 0.465 && dT < 1.837) dC = 1.5 / pow(dT, 2. / 3.);
		else dC = 1.0;
	}
	break;
	case 2:  // Soil III
	{
		if (dT <= 0.030) dC = 1.0;
		else if (dT >= 0.030 && dT < 0.200) dC = 8.824 * dT + 0.7352;
		else if (dT >= 0.200 && dT < 0.611) dC = 2.5;
		else if (dT >= 0.611 && dT < 2.415) dC = 1.8 / pow(dT, 2. / 3.);
		else dC = 1.0;
	}
	break;
	case 3:  // Taipei Basin
	{
		if (dT <= 0.030) dC = 1.0;
		else if (dT >= 0.030 && dT < 0.200) dC = 8.824 * dT + 0.7352;
		else if (dT >= 0.200 && dT < 1.320) dC = 2.5;
		else if (dT >= 1.320 && dT < 3.300) dC = 3.3 / dT;
		else dC = 1.0;
	}
	break;
	}
	return dC * dCd;
}

double CSpfcTAIWAN99HUtil::GetTaiwan86POCFactor(int nSoil, double dDamping, double dT, double dTg)
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
		if (dTg < 0.15) { dTg = 0.15; ASSERT(0); }
		if (dTg > 1.315) { dTg = 1.315; ASSERT(0); }

		if (dT <= 0.030) dC = 1.0;
		else if (dT >= 0.030 && dT < 0.150) dC = 12.5 * dT + 0.625;
		else if (dT >= 0.150 && dT < dTg) dC = 2.5;
		else if (dT >= dTg && dT < 1.315) dC = 1.2 / pow(dT, 2. / 3.);
		else dC = 1.0;
	}
	break;
	case 1:  // Soil II
	{
		if (dTg < 0.15) { dTg = 0.15; ASSERT(0); }
		if (dTg > 1.837) { dTg = 1.837; ASSERT(0); }

		if (dT <= 0.030) dC = 1.0;
		else if (dT >= 0.030 && dT < 0.150) dC = 12.5 * dT + 0.625;
		else if (dT >= 0.150 && dT < dTg) dC = 2.5;
		else if (dT >= dTg && dT < 1.837) dC = 1.5 / pow(dT, 2. / 3.);
		else dC = 1.0;
	}
	break;
	case 2:  // Soil III
	{
		if (dTg < 0.200) { dTg = 0.200; ASSERT(0); }
		if (dTg > 2.415) { dTg = 2.415; ASSERT(0); }

		if (dT <= 0.030) dC = 1.0;
		else if (dT >= 0.030 && dT < 0.200) dC = 8.824 * dT + 0.7352;
		else if (dT >= 0.200 && dT < dTg) dC = 2.5;
		else if (dT >= dTg && dT < 2.415) dC = 1.8 / pow(dT, 2. / 3.);
		else dC = 1.0;
	}
	break;
	case 3:  // Taipei Basin
	{
		if (dTg < 0.200) { dTg = 0.200; ASSERT(0); }
		if (dTg > 3.300) { dTg = 3.300; ASSERT(0); }

		if (dT <= 0.030) dC = 1.0;
		else if (dT >= 0.030 && dT < 0.200) dC = 8.824 * dT + 0.7352;
		else if (dT >= 0.200 && dT < dTg) dC = 2.5;
		else if (dT >= dTg && dT < 3.300) dC = 3.3 / dT;
		else dC = 1.0;
	}
	break;
	}
	return dC * dCd;
}

double CSpfcTAIWAN99HUtil::GetTaiwan86FuFactor(int nSoil, double dR, double dT)
{
	// MNET:2689-GSJI-20070312 dR=1.0 이면 dFu=1.0으로 고정됨.
	double dFu;
	if (dR < 0.) dR = 0.;
	double dRa = 1.0 + (dR - 1) / 2.0;
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
		if (dT >= 1.40) dFu = dRa;
		else if (dT >= 0.80 && dT < 1.40) dFu = dSRa + (dRa - dSRa) * (dT - 0.80) / 0.60;
		else if (dT >= 0.20 && dT < 0.80) dFu = dSRa;
		else if (dT >= 0.03 && dT < 0.20) dFu = dSRa + (dSRa - 1.) * (dT - 0.20) / 0.17;
		else dFu = 1.0;
	}
	break;
	}

	return dFu;
}

double CSpfcTAIWAN99HUtil::GetTaiwan86SpectralAcc(int nSoil, double dZ, double dDamping, double dI, double dR, double dAlpha, double dT)
{
	double dC = GetTaiwan86CFactor(nSoil, dDamping, dT);
	double dFu = GetTaiwan86FuFactor(nSoil, dR, dT);
	double dCFum = min(1.0, dC / dFu);
	double dZd = max(dZ * dI / (1.4 * dAlpha), dZ * dI * dFu / (3.5 * dAlpha));
	double dSa = dZd * dCFum;

	return dSa;
}

double CSpfcTAIWAN99HUtil::GetTaiwan86ModifiedTg(int nSoil, double dR, double SRa, double SRv)
{
	double dRa = 1.0 + (dR - 1) / 2.0;
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

	return dTg;
}

double CSpfcTAIWAN99HUtil::GetTaiwan86POSpectralAcc(int nSoil, double dZ, double dI, double dR, double dAlpha, double dT, double dTg)
{
	//double dRa = 1.0 + (dR - 1) / 2.0;

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
