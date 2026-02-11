#include "stdafx.h"
#include "SpfcCHSH2003Util.h"

CSpfcCHSH2003Util::CSpfcCHSH2003Util()
{
	m_dMaxPeriod = PRD_ED;
	m_nER = 0;
	m_nSFI = 0;
	m_nSC = 0;
	m_dXi = 0.05;
	m_dTg = 0.65;
	m_dMaxEQE = 0.08;
}

bool CSpfcCHSH2003Util::CheckValid()
{
	if (IsValid(m_dTg, 0.1, 1.2) == false)
	{
		return false;
	}

	if (m_dMaxPeriod <= 0) { AfxMessageBox(_LS(IDS_CMD_SPECTRUM__MAX_PERIOD_GREATER_0)); return false; }
	if (m_dXi < 0) { AfxMessageBox(_LS(IDS_CMD_SPECTRUM__DAMPING_VALUE)); return false; }
	return TRUE;
}

void CSpfcCHSH2003Util::MakeSpectrumData(bool bOnlyCalc)
{
	double st = PRD_ST;
	double ed = m_dMaxPeriod;
	int Step = PRD_NUM;
	double Tn, dt, Alpha;

	// Pushover FEMA440에서 계산할 때(Period 2배, Step 4배)
	if (bOnlyCalc)
	{
		ed *= GetPeriodCoeff();
		Step *= GetStepCoeff();
	}

	// PMS4912 Gen 응답스펙트럼하중 alpha_max값 사용자 수정
	// JWKWON 2015-03-16 : MQC 4912-2 수정
	m_dAlphaMax = m_dMaxEQE;

	dt = (ed - st) / Step;  // time interval	
	double dTol = dt / Step;  // Tolerance

	InitPeriod();
	InitAccel();

	for (int i = 0; i <= Step; i++)
	{
		Tn = st + dt * i;

		//		if ( Tn <= 1.0E-4 ) Tn=1.0E-4;

		if (Tn >= 0 && Tn < 0.1)
			Alpha = 0.45 * m_dAlphaMax + Tn * (m_dEta2 - 0.45) * m_dAlphaMax / 0.1;
		else if (Tn >= 0.1 && Tn < m_dTg)
			Alpha = m_dEta2 * m_dAlphaMax;
		else if (Tn >= m_dTg && Tn < 5 * m_dTg)
			Alpha = m_dEta2 * m_dAlphaMax * pow((m_dTg / Tn), m_dGamma);
		else if (Tn >= 5 * m_dTg && Tn <= 10.0)
			Alpha = (m_dEta2 * pow(0.2, m_dGamma) - m_dEta1 * (Tn - 5.0 * m_dTg)) * m_dAlphaMax;
		else if (Tn > 10.0)
			Alpha = (m_dEta2 * pow(0.2, m_dGamma) - m_dEta1 * (10.0 - 5.0 * m_dTg)) * m_dAlphaMax;

		if (Tn > 0.1 + dTol && Tn < 0.1 + dt - dTol) // transition point		
		{
			AddPeriod(0.1);
			AddAccel(m_dEta2 * m_dAlphaMax);
		}
		if (Tn > m_dTg + dTol && Tn < m_dTg + dt - dTol) // transition point		
		{
			AddPeriod(m_dTg);
			AddAccel(m_dEta2 * m_dAlphaMax);
		}
		if (Tn > 5 * m_dTg + dTol && Tn < 5 * m_dTg + dt - dTol) // transition point		
		{
			AddPeriod(5 * m_dTg);
			AddAccel(m_dEta2 * m_dAlphaMax * pow(0.2, m_dGamma));
		}
		if (Tn > 10.0 + dTol && Tn < 10.0 + dt - dTol) // transition point		
		{
			AddPeriod(10.0);
			AddAccel((m_dEta2 * pow(0.2, m_dGamma) - m_dEta1 * (10.0 - 5.0 * m_dTg)) * m_dAlphaMax);
		}

		AddPeriod(Tn);
		AddAccel(Alpha);
	}
	if (bOnlyCalc) return;

	CString strSFI[] = { _T("6(0.05g)"),_T("7(0.10g)"),_T("8(0.20g)") };
	CString strSC[] = { _T("III"), _T("IV") };
	CString strER[] = { _LS(IDS_CMD0417__Frequent_E_Q_), _LS(IDS_CMD0417__Scarce_E_Q_) };

	wchar_t buffer[SpfcUtil::BUFFER_SIZE];
	int result = std::swprintf(buffer, SpfcUtil::BUFFER_SIZE, _LS(IDS_WG_CMD__ADD_CHSH2003_PROFILE), strSFI[m_nSFI], strSC[m_nSC], strER[m_nER], m_dTg, m_dXi);
	if (result >= 0)
	{
		SetDescript(buffer);
	}
	SetFuncName(L"China Shanghai(DGJ08-9-2003)");
}

void CSpfcCHSH2003Util::MakePOSpectrumData(double dDamping, double SRa, double SRv, CArray<double, double>& aPeriod, CArray<double, double>& aAccel)
{

	aPeriod.RemoveAll();
	aAccel.RemoveAll();

	double st = PRD_ST;
	double ed = m_dMaxPeriod * 2;
	int Step = PRD_NUM * 4;
	double Tn, dt;
	double Alpha;
	double Tg1;   // Tc의 변환점

	if (dDamping >= 0)
	{
		SRa = (3.21 - 0.68 * log(dDamping)) / 2.12;
		SRv = (2.31 - 0.41 * log(dDamping)) / 1.65;
	}

	dt = (ed - st) / Step;  // time interval	
	double dTol = dt / Step;  // Tolerance

	Tg1 = m_dTg * pow(SRv / SRa, 1.0 / m_dGamma);  // modified Tg

	for (int i = 0; i <= Step; i++)
	{
		Tn = st + dt * i;

		//		if ( Tn <= 1.0E-4 ) Tn=1.0E-4;

		if (Tn >= 0 && Tn < 0.1)
			Alpha = (0.45 * m_dAlphaMax + Tn * (m_dEta2 * SRa - 0.45) * m_dAlphaMax / 0.1);
		else if (Tn >= 0.1 && Tn < Tg1)
			Alpha = m_dEta2 * m_dAlphaMax * SRa;
		else if (Tn >= Tg1 && Tn < 5 * m_dTg)
			Alpha = m_dEta2 * m_dAlphaMax * pow((m_dTg / Tn), m_dGamma) * SRv;
		else if (Tn >= 5 * m_dTg && Tn <= 10.0)
			Alpha = (m_dEta2 * pow(0.2, m_dGamma) - m_dEta1 * (Tn - 5 * m_dTg)) * m_dAlphaMax * SRv;
		else if (Tn > 10.0)
			Alpha = (m_dEta2 * pow(0.2, m_dGamma) - m_dEta1 * (10.0 - 5 * m_dTg)) * m_dAlphaMax * SRv;

		if (Tn > 0.1 + dTol && Tn < 0.1 + dt - dTol) // transition point		
		{
			aPeriod.Add(0.1);
			aAccel.Add(m_dEta2 * m_dAlphaMax * SRa);
		}
		if (Tn > Tg1 + dTol && Tn < Tg1 + dt - dTol) // transition point		
		{
			aPeriod.Add(Tg1);
			aAccel.Add(m_dEta2 * m_dAlphaMax * SRa);
		}
		if (Tn > 5 * m_dTg + dTol && Tn < 5 * m_dTg + dt - dTol) // transition point		
		{
			aPeriod.Add(5 * m_dTg);
			aAccel.Add(m_dEta2 * m_dAlphaMax * pow(0.2, m_dGamma) * SRv);
		}
		if (Tn > 10.0 + dTol && Tn < 10.0 + dt - dTol) // transition point		
		{
			aPeriod.Add(10.0);
			aAccel.Add((m_dEta2 * pow(0.2, m_dGamma) - m_dEta1 * (10.0 - 5.0 * m_dTg)) * m_dAlphaMax * SRv);
		}

		aPeriod.Add(Tn);
		aAccel.Add(Alpha);
	}
}

void CSpfcCHSH2003Util::SetParamFromCode(const T_SPFC_CODE& codeParam)
{
	m_nSFI = codeParam.CHSH2003.nSeisFortification;
	m_nSC = codeParam.CHSH2003.nSiteClass;
	m_nER = codeParam.CHSH2003.nEarthResponse;
	m_dTg = codeParam.CHSH2003.dTg;
	m_dXi = codeParam.CHSH2003.dDamping;
	m_dMaxEQE = codeParam.CHSH2003.dMaxEQE;
	m_dMaxPeriod = codeParam.CHSH2003.dMaxPeriod;
}

void CSpfcCHSH2003Util::SetParamFromCalc()
{
	m_dGamma = 0.9 + (0.05 - m_dXi) / (0.5 + 5 * m_dXi);
	m_dEta1 = 0.02 * (1.0 + (0.05 - m_dXi) / (0.06 + 2.4 * m_dXi));
	m_dEta2 = 1 + (0.05 - m_dXi) / (0.06 + 1.7 * m_dXi);
	if (m_dEta1 < 0.0) m_dEta1 = 0.0;
	if (m_dEta2 < 0.55) m_dEta2 = 0.55;

	switch (m_nER)
	{
	case 0:  // Frequent Earthquake
		if (m_nSFI == 0)      m_dAlphaMax = 0.04;
		else if (m_nSFI == 1) m_dAlphaMax = 0.08;
		else if (m_nSFI == 2) m_dAlphaMax = 0.16;
		break;
	case 1:  // Scarce Earthquake
		if (m_nSFI == 0)      m_dAlphaMax = 0.0;
		else if (m_nSFI == 1) m_dAlphaMax = 0.45;
		else if (m_nSFI == 2) m_dAlphaMax = 0.81;
		break;
	}
}
