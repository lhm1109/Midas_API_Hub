#include "stdafx.h"
#include "SpfcCHGB2021Util.h"

CSpfcCHGB2021Util::CSpfcCHGB2021Util()
{
	m_nER = 0;
	m_nSDC = 0;
	m_nSFI = 0;
	m_nSC = 0;

	m_dXi = 0.0;
	m_dMaxPeriod = PRD_ED;
	m_nLForce = 0;
	m_dMaxEQE = 0.08;
}

bool CSpfcCHGB2021Util::CheckValid()
{
	if (IsValid(m_dTg, 0.1, 1.2) == false)
	{
		return false;
	}

	if (m_dMaxPeriod <= 0) { AfxMessageBox(_LS(IDS_CMD_SPECTRUM__MAX_PERIOD_GREATER_0)); return false; }
	if (m_dXi < 0) { AfxMessageBox(_LS(IDS_CMD_SPECTRUM__DAMPING_VALUE)); return false; }
	return true;
}

void CSpfcCHGB2021Util::MakeSpectrumData(bool bOnlyCalc)
{
	double st = PRD_ST;
	double ed = m_dMaxPeriod;
	int Step = PRD_NUM;
	double Tn, dt, Alpha;

	// Pushover FEMA440俊辑 拌魂且 锭(Period 2硅, Step 4硅)
	if (bOnlyCalc)
	{
		ed *= GetPeriodCoeff();
		Step *= GetStepCoeff();
	}
	// PMS4912 Gen 览翠胶棋飘烦窍吝 alpha_max蔼 荤侩磊 荐沥
	// JWKWON 2015-03-16 : MQC 4912-2 荐沥
	m_dAlphaMax = m_dMaxEQE;

	dt = (ed - st) / Step;      // time interval	
	double dTol = dt / Step;  // Tolerance

	InitPeriod();
	InitAccel();

	for (int i = 0; i <= Step; i++)
	{
		Tn = st + dt * i;
		//		if ( Tn <= 1.0E-4 ) Tn=1.0E-4;
		if (Tn >= 0 && Tn < 0.1)
			Alpha = 0.45 * m_dAlphaMax + Tn * (m_dEta - 0.45) * m_dAlphaMax / 0.1;
		else if (Tn >= 0.1 && Tn < m_dTg)
			Alpha = m_dEta * m_dAlphaMax;
		else if (Tn >= m_dTg)
			Alpha = m_dEta * m_dAlphaMax * pow((m_dTg / Tn), m_dGamma);

		if (Tn > 0.1 + dTol && Tn < 0.1 + dt - dTol) // transition point		
		{
			AddPeriod(0.1);
			AddAccel(m_dEta * m_dAlphaMax);
		}
		if (Tn > m_dTg + dTol && Tn < m_dTg + dt - dTol) // transition point		
		{
			AddPeriod(m_dTg);
			AddAccel(m_dEta * m_dAlphaMax);
		}
		if (Tn > 6.0 + dTol && Tn < 6.0 + dt - dTol) // transition point		
		{
			AddPeriod(6.0);
			AddAccel(m_dEta * m_dAlphaMax * pow(0.2, m_dGamma));
		}

		AddPeriod(Tn);
		AddAccel(Alpha);
	}

	if (bOnlyCalc) return;

	CString strSFI[] = { _T("6(0.05g)"),_T("7(0.10g)"),_T("7(0.15g)"),_T("8(0.20g)"),_T("8(0.30g)"),_T("9(0.40g)") };
	CString strSC[] = { _T("I0"), _T("I1"),  _T("II"), _T("III"), _T("IV") };
	CString strER[] = { _LS(IDS_CMD0417__Middle_E_Q_), _LS(IDS_CMD0417__Scarce_E_Q_), _LS(IDS_CMD0417__Very_Scarce_E_Q_) };

	wchar_t buffer[SpfcUtil::BUFFER_SIZE];
	int result = std::swprintf(buffer, SpfcUtil::BUFFER_SIZE, _LS(IDS_WG_CMD__ADD_CHGBT_PROFILE), m_nSDC + 1, strSFI[m_nSFI], strSC[m_nSC], strER[m_nER], m_dTg, m_dXi);
	if (result >= 0)
	{
		SetDescript(buffer);
	}
	SetFuncName(L"China(GB/T 51408-2021)");
}

void CSpfcCHGB2021Util::MakePOSpectrumData(double dDamping, double SRa, double SRv, CArray<double, double>& aPeriod, CArray<double, double>& aAccel)
{
	// 현재 토목 기술팀에서 PushOver 에 대한 답을 듣지 못한 상황이므로 요렇게 처리...으흐흐흐~~~
	aPeriod.RemoveAll();
	aAccel.RemoveAll();

	double st = PRD_ST;
	double ed = m_dMaxPeriod * 2;
	int Step = PRD_NUM * 4;
	double Tn, dt;
	double Alpha;
	double Tg1;   // Tc狼 函券痢

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
			Alpha = (0.45 * m_dAlphaMax + Tn * (m_dEta * SRa - 0.45) * m_dAlphaMax / 0.1);
		else if (Tn >= 0.1 && Tn < Tg1)
			Alpha = m_dEta * m_dAlphaMax * SRa;
		else if (Tn >= Tg1)
			Alpha = m_dEta * m_dAlphaMax * pow((m_dTg / Tn), m_dGamma) * SRv;

		if (Tn > 0.1 + dTol && Tn < 0.1 + dt - dTol) // transition point		
		{
			aPeriod.Add(0.1);
			aAccel.Add(m_dEta * m_dAlphaMax * SRa);
		}
		if (Tn > Tg1 + dTol && Tn < Tg1 + dt - dTol) // transition point		
		{
			aPeriod.Add(Tg1);
			aAccel.Add(m_dEta * m_dAlphaMax * SRa);
		}
		if (Tn > 6.0 + dTol && Tn < 6.0 + dt - dTol) // transition point		
		{
			aPeriod.Add(6.0);
			aAccel.Add(m_dEta * m_dAlphaMax * pow(0.2, m_dGamma) * SRv);
		}

		aPeriod.Add(Tn);
		aAccel.Add(Alpha);
	}
}

void CSpfcCHGB2021Util::SetParamFromCode(const T_SPFC_CODE& codeParam)
{
	m_nSDC = codeParam.GB_T_51408_2021.nSeisDgnCategory;
	m_nSFI = codeParam.GB_T_51408_2021.nSeisFortification;
	m_nSC = codeParam.GB_T_51408_2021.nSiteClass;
	m_nER = codeParam.GB_T_51408_2021.nEarthResponse;

	m_dTg = codeParam.GB_T_51408_2021.dTg;
	m_dXi = codeParam.GB_T_51408_2021.dDamping;
	m_nLForce = codeParam.GB_T_51408_2021.nLForce;
	m_dMaxEQE = codeParam.GB_T_51408_2021.dMaxEQE;
	m_dMaxPeriod = codeParam.GB_T_51408_2021.dMaxPeriod;
}

void CSpfcCHGB2021Util::SetParamFromCalc()
{
	m_dGamma = 0.9 + (0.05 - m_dXi) / (0.3 + 6.0 * m_dXi); // 邦急窍碍康开狼 皑尖拌荐.
	m_dEta = 1.0 + (0.05 - m_dXi) / (0.08 + 1.6 * m_dXi); // Damping Ratio 炼沥拌荐.
	if (m_dEta < 0.0) m_dEta = 0.0;
	// 0:0.05g, 1:0.10g, 2:0.15g, 3:0.20g, 4:0.30g, 5:0.40g
	switch (m_nER)
	{
	case 0: // Middle Earthquake.
		if (m_nSFI == 0) m_dAlphaMax = 0.12;
		else if (m_nSFI == 1) m_dAlphaMax = 0.23;
		else if (m_nSFI == 2) m_dAlphaMax = 0.34;
		else if (m_nSFI == 3) m_dAlphaMax = 0.45;
		else if (m_nSFI == 4) m_dAlphaMax = 0.68;
		else if (m_nSFI == 5) m_dAlphaMax = 0.90;
		else ASSERT(0);
		break;
	case 1: // Scarce Earthquake.
		if (m_nSFI == 0) m_dAlphaMax = 0.28;
		else if (m_nSFI == 1) m_dAlphaMax = 0.50;
		else if (m_nSFI == 2) m_dAlphaMax = 0.72;
		else if (m_nSFI == 3) m_dAlphaMax = 0.90;
		else if (m_nSFI == 4) m_dAlphaMax = 1.20;
		else if (m_nSFI == 5) m_dAlphaMax = 1.10;
		else ASSERT(0);
		break;
	case 2: // Rare Scarce Earthquake.
		if (m_nSFI == 0) m_dAlphaMax = 0.36;
		else if (m_nSFI == 1) m_dAlphaMax = 0.72;
		else if (m_nSFI == 2) m_dAlphaMax = 1.00;
		else if (m_nSFI == 3) m_dAlphaMax = 1.35;
		else if (m_nSFI == 4) m_dAlphaMax = 2.00;
		else if (m_nSFI == 5) m_dAlphaMax = 2.43;
		else ASSERT(0);
		break;
	}
}
