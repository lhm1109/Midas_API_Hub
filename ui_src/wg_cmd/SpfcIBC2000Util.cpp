#include "stdafx.h"
#include "SpfcIBC2000Util.h"

CSpfcIBC2000Util::CSpfcIBC2000Util()
{
	m_wndSC = 0;
	m_dMaxPeriod = PRD_ED;
	m_dI = 1.0;
	m_dSs = 0.25;
	m_dS1 = 0.1;
	m_dR = 1.0;

	SetValue();
}

bool CSpfcIBC2000Util::CheckValid()
{
	if (IsValid(m_dSs, 0, 10) == false) return false;
	if (IsValid(m_dS1, 0, 50) == false) return false;
	if (IsValid(m_dI, 0, 50) == false) return false;
	if (m_dMaxPeriod <= 0) { AfxMessageBox(_LS(IDS_CMD_SPECTRUM__MAX_PERIOD_GREATER_0)); return false; }
	return true;
}

void CSpfcIBC2000Util::MakeSpectrumData(bool bOnlyCalc /*= false*/)
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
	double dCs;

	m_dR = 1.0;
	m_dI = 1.0;

	if (m_wndSC == 4 && (m_dSs > 1.00 || m_dS1 > 0.4)) // E
	{
		GSaveHistoryFormatNF(_LS(IDS_CMD0417__Site_Specific_geotechnical_investigati));
		return;
	}

	dt = (ed - st) / Step;  // time interval	
	double dTol = dt / Step;  // Tolerance

	InitPeriod();
	InitAccel();

	for (int i = 0; i <= Step; i++)
	{
		Tn = st + dt * i;

		//		if ( Tn <= 1.0E-4 ) Tn=1.0E-4;

		if (Tn >= 0 && Tn <= m_dT0)				 dCs = (0.6 * (m_dSds / m_dT0) * Tn + 0.4 * m_dSds) * m_dI / m_dR;
		else if (Tn > m_dT0 && Tn <= m_dTs) dCs = m_dSds * m_dI / m_dR;
		else if (Tn > m_dTs)				 			   dCs = min((m_dSd1 / Tn) * m_dI / m_dR, m_dSds * m_dI / m_dR);

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
		AddAccel(dCs);
	}

	if (bOnlyCalc) return;

	CString strDgnCategory[] = { _T("A"),_T("B"),_T("C"),_T("D"),_T("E"),_T("F") };
	CString strSiteClass[] = { _T("A"),_T("B"),_T("C"),_T("D"),_T("E") };

	wchar_t buffer[SpfcUtil::BUFFER_SIZE];
	int result = std::swprintf(buffer, SpfcUtil::BUFFER_SIZE, _LS(IDS_WG_CMD__ADD_IBC2000_PROFILE), strSiteClass[m_wndSC], m_dSs, m_dS1);
	if (result >= 0)
	{
		SetDescript(buffer);
	}
	SetFuncName(L"IBC2000(ASCE7-98)");
}

void CSpfcIBC2000Util::MakePOSpectrumData(double dDamping, double SRa, double SRv, CArray <double, double>& aPeriod, CArray <double, double>& aAccel)
{
	aPeriod.RemoveAll();
	aAccel.RemoveAll();

	double st = PRD_ST;
	double ed = m_dMaxPeriod * 2;
	int Step = PRD_NUM * 4;
	double Tn, dt;
	double dCs;
	double dTs, dT0;

	m_dR = 1.0;
	m_dI = 1.0;

	if (dDamping >= 0)
	{
		SRa = (3.21 - 0.68 * log(dDamping)) / 2.12;
		SRv = (2.31 - 0.41 * log(dDamping)) / 1.65;
	}

	dt = (ed - st) / Step;  // time interval	
	double dTol = dt / Step;  // Tolerance

	dTs = m_dTs * SRv / SRa;
	dT0 = 0.2 * dTs;

	for (int i = 0; i <= Step; i++)
	{
		Tn = st + dt * i;

		//		if ( Tn <= 1.0E-4 ) Tn=1.0E-4;

		if (Tn >= 0 && Tn <= dT0)			 dCs = (0.4 + (SRa - 0.4) * Tn / dT0) * m_dSds * m_dI / m_dR;
		else if (Tn > dT0 && Tn <= dTs) dCs = (m_dSds * m_dI / m_dR) * SRa;
		else if (Tn > dTs)					 	   dCs = min((m_dSd1 / Tn) * (m_dI / m_dR) * SRv, (m_dSds * m_dI / m_dR) * SRa);

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
		aAccel.Add(dCs);
	}
}

void CSpfcIBC2000Util::SetParamFromCode(const T_SPFC_CODE& codeParam)
{
	m_wndSC = codeParam.IBC2000.nSc;
	m_dSs = codeParam.IBC2000.dSs;
	m_dS1 = codeParam.IBC2000.dS1;
	m_dMaxPeriod = codeParam.IBC2000.dMaxPeriod;
}

void CSpfcIBC2000Util::SetParamFromCalc()
{
	m_dI = 1.0;
	m_dR = 1.0;
	SetValue();
}

//-----------------------------------------------------------------------------------------------------
// INIT : 2003.09.20. by KYE-HONG
// FUNC : 계산알고리즘에 사용되는 값을 Setting함. 
//-----------------------------------------------------------------------------------------------------
void CSpfcIBC2000Util::SetValue()
{
	double dSms, dSm1;
	double Fa, Fv, Fa_i, Fa_j, Fv_i, Fv_j;
	double TBL_Fa[5][5] = { {0.8,0.8,0.8,0.8,0.8}, //Fa
														{1.0,1.0,1.0,1.0,1.0},
														{1.2,1.2,1.1,1.0,1.0},
														{1.6,1.4,1.2,1.1,1.0},
														{2.5,1.7,1.2,0.9,9.9} }; // 9.9 means impossibility

	double TBL_Fv[5][5] = { {0.8,0.8,0.8,0.8,0.8},  //Fv
														{1.0,1.0,1.0,1.0,1.0},
														{1.7,1.6,1.5,1.4,1.3},
														{2.4,2.0,1.8,1.6,1.5},
														{3.5,3.2,2.8,2.4,9.9} }; // 9.9 means impossibility

	double Ss_val[5] = { 0.25,0.50,0.75,1.00,1.25 };
	double S1_val[5] = { 0.1,0.2,0.3,0.4,0.5 };

	// determine Fa;
	if (m_dSs <= Ss_val[0])       Fa = TBL_Fa[m_wndSC][0];
	else if (m_dSs >= Ss_val[4])  Fa = TBL_Fa[m_wndSC][4];
	else
	{
		for (int I = 0; I < 4; I++)
		{
			if (m_dSs >= Ss_val[I] && m_dSs <= Ss_val[I + 1])
			{
				Fa_i = TBL_Fa[m_wndSC][I];
				Fa_j = TBL_Fa[m_wndSC][I + 1];
				Fa = Fa_i + (Fa_j - Fa_i) * (m_dSs - Ss_val[I]) / (Ss_val[I + 1] - Ss_val[I]);
			}
		}
	}

	// determine Fv;
	if (m_dS1 <= S1_val[0])       Fv = TBL_Fv[m_wndSC][0];
	else if (m_dS1 >= S1_val[4])  Fv = TBL_Fv[m_wndSC][4];
	else
	{
		for (int I = 0; I < 4; I++)
		{
			if (m_dS1 >= S1_val[I] && m_dS1 <= S1_val[I + 1])
			{
				Fv_i = TBL_Fv[m_wndSC][I];
				Fv_j = TBL_Fv[m_wndSC][I + 1];
				Fv = Fv_i + (Fv_j - Fv_i) * (m_dS1 - S1_val[I]) / (S1_val[I + 1] - S1_val[I]);
			}
		}
	}

	dSms = Fa * m_dSs;
	dSm1 = Fv * m_dS1;

	m_dSds = 2.0 / 3.0 * dSms;
	m_dSd1 = 2.0 / 3.0 * dSm1;

	m_dTs = m_dSd1 / m_dSds;
	m_dT0 = 0.2 * m_dTs;
}