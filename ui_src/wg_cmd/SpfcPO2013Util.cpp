#include "stdafx.h"
#include "SpfcPO2013Util.h"
#include "../wg_db/MathFunc.h"

CSpfcPO2013Util::CSpfcPO2013Util()
{
	m_nSpecType = 0;
	m_dMaxPeriod = PRD_ED;
	m_nParaTypeRdo = D_SPFC_P100_2013_TC07S;

	m_dTb = 0.15;
	m_dTc = 0.5;
	m_dTd = 2.0;
	m_dAg = 0.3;
	m_dDamping = 5.0;
	m_dQ = 1.5;
	m_dImportance = 1.0;
}

bool CSpfcPO2013Util::CheckValid()
{
	bool bElastic = false;

	if (m_nSpecType == D_SPFC_P100_2013_SP_HE || m_nSpecType == D_SPFC_P100_2013_SP_VE) bElastic = true;

	if (m_dTb <= 0) { AfxMessageBox(_T("Tb > 0")); return false; }
	if (m_dTb >= m_dTc) { AfxMessageBox(_T("Tc > Tb")); return false; }
	if (m_dTc >= m_dTd) { AfxMessageBox(_T("Td > Tc")); return false; }
	if (m_dAg <= 0) { AfxMessageBox(_LS(IDS_CMD_SPECTRUM__GROUND_ACC)); return false; }
	if (bElastic)
	{
		if (m_dTd >= 4) { AfxMessageBox(_T("Td < 4")); return false; }
		if (m_dDamping < 0) { AfxMessageBox(_LS(IDS_CMD_SPECTRUM__DAMPING_VALUE)); return false; }
	}
	else
	{
		if (m_dQ <= 0) { AfxMessageBox(_T("q > 0")); return false; }
		if (m_dImportance < 0.8 || m_dImportance>1.4) { AfxMessageBox(_T("0.8 < I < 1.4")); return false; }
	}
	if (m_dMaxPeriod <= 0) { AfxMessageBox(_LS(IDS_CMD_SPECTRUM__MAX_PERIOD_GREATER_0)); return false; }
	return true;
}

void CSpfcPO2013Util::MakeSpectrumData(bool bOnlyCalc)
{
	const double st = PRD_ST;
	double ed = m_dMaxPeriod;
	int Step = PRD_NUM;
	// Pushover FEMA440에서 계산할 때(Period 2배, Step 4배)
	if (bOnlyCalc)
	{
		ed *= GetPeriodCoeff();
		Step *= GetStepCoeff();
	}
	double Tn, dt;
	double SeT;
	const double dMaxTn = 5.0;

	dt = (ed - st) / Step;  // time interval	
	double dTol = dt / Step;  // Tolerance

	int nSpecType = m_nSpecType;
	const BOOL bVertical = nSpecType == D_SPFC_P100_2013_SP_VE || nSpecType == D_SPFC_P100_2013_SP_VD;
	if (nSpecType == D_SPFC_P100_2013_SP_HE || nSpecType == D_SPFC_P100_2013_SP_VE)  // Elastic Spectrum : Horizontal(0), Vertical(1)
	{
		double dAg = bVertical ? m_dAg * 0.7 : m_dAg; // 수직일 때는 Avg = Ag*0.7 사용
		const double dEta = max(sqrt(10. / (5. + m_dDamping)), 0.55);  // Damping Correction Factor  
		const double dBeta_0 = nSpecType == D_SPFC_P100_2013_SP_HE ? 2.5 : 2.75;
		const BOOL bDampingIsNot5 = fabs(m_dDamping - 5.0) > CMathFunc::m_NormalZero;

		InitPeriod();
		InitAccel();

		for (int i = 0; i <= Step; i++)
		{
			Tn = st + dt * i;

			if (Tn >= 0 && Tn < m_dTb)
				SeT = dAg * (1 + (dBeta_0 - 1.0) * Tn / m_dTb) * m_dImportance;
			else if (Tn >= m_dTb && Tn < m_dTc)
				SeT = dAg * dBeta_0 * m_dImportance;
			else if (Tn >= m_dTc && Tn < m_dTd)
				SeT = dAg * dBeta_0 * m_dTc / Tn * m_dImportance;
			else if (Tn >= m_dTd && Tn < dMaxTn)
				SeT = dAg * dBeta_0 * m_dTc * m_dTd / (Tn * Tn) * m_dImportance;
			else if (Tn >= dMaxTn)
				SeT = dAg * dBeta_0 * m_dTc * m_dTd / (dMaxTn * dMaxTn) * m_dImportance;
			else
				ASSERT(false);

			// 감쇠력이 5%가 아닐 경우는 아래의 식을 통해 Se(T)값을 보정합니다.
			if (bDampingIsNot5)
			{
				if (Tn >= 0 && Tn < m_dTb)
				{
					SeT = (dAg + (SeT * dEta - dAg) * Tn / m_dTb) * m_dImportance;
				}
				else
				{
					SeT = SeT * dEta * m_dImportance;
				}
			}

			if (Tn > m_dTb + dTol && Tn < m_dTb + dt - dTol) // transition point : Tb
			{
				double dAcc = dAg * (1 + (dBeta_0 - 1.0) * m_dTb / m_dTb) * m_dImportance;
				if (bDampingIsNot5) dAcc *= dEta * m_dImportance;

				AddPeriod(m_dTb);
				AddAccel(dAcc);
			}
			if (Tn > m_dTc + dTol && Tn < m_dTc + dt - dTol) // transition point : Tc
			{
				double dAcc = dAg * dBeta_0 * m_dImportance;
				if (bDampingIsNot5) dAcc *= dEta * m_dImportance;

				AddPeriod(m_dTc);
				AddAccel(dAcc);
			}
			if (Tn > m_dTd + dTol && Tn < m_dTd + dt - dTol) // transition point : Td
			{
				double dAcc = dAg * dBeta_0 * m_dTc / m_dTd * m_dImportance;
				if (bDampingIsNot5) dAcc *= dEta * m_dImportance;

				AddPeriod(m_dTd);
				AddAccel(dAcc);
			}
			if (Tn > dMaxTn + dTol && Tn < dMaxTn + dt - dTol) // transition poin : MaxTn
			{
				double dAcc = dAg * dBeta_0 * m_dTc * m_dTd / (dMaxTn * dMaxTn) * m_dImportance;
				if (bDampingIsNot5) dAcc *= dEta * m_dImportance;

				AddPeriod(dMaxTn);
				AddAccel(dAcc);
			}
			AddPeriod(Tn);
			AddAccel(SeT);
		}
	}
	else if (nSpecType == D_SPFC_P100_2013_SP_HD || nSpecType == D_SPFC_P100_2013_SP_VD)  // Design Spectrum : Horizontal(2), Vertical(3)
	{
		double dAg = bVertical ? m_dAg * 0.7 : m_dAg; // 수직일 때는 Avg = Ag*0.7 사용
		const double dBeta_0 = nSpecType == D_SPFC_P100_2013_SP_HD ? 2.5 : 2.75;
		const double d02AvgI = 0.2 * dAg * m_dImportance;

		InitPeriod();
		InitAccel();

		for (int i = 0; i <= Step; i++)
		{
			Tn = st + dt * i;

			if (Tn >= 0 && Tn < m_dTb)
				SeT = dAg * (1 + (dBeta_0 / m_dQ - 1.0) * Tn / m_dTb) * m_dImportance;
			else if (Tn >= m_dTb && Tn < m_dTc)
				SeT = max(dAg * dBeta_0 / m_dQ * m_dImportance, d02AvgI);
			else if (Tn >= m_dTc && Tn < m_dTd)
				SeT = max(dAg * dBeta_0 / m_dQ * m_dTc / Tn * m_dImportance, d02AvgI);
			else if (Tn >= m_dTd && Tn < dMaxTn)
				SeT = max(dAg * dBeta_0 / m_dQ * m_dTc * m_dTd / (Tn * Tn) * m_dImportance, d02AvgI);
			else if (Tn >= dMaxTn)
				SeT = max(dAg * dBeta_0 / m_dQ * m_dTc * m_dTd / (dMaxTn * dMaxTn) * m_dImportance, d02AvgI);
			else
				ASSERT(false);

			// 감쇠력은 미고려함

			if (Tn > m_dTb + dTol && Tn < m_dTb + dt - dTol) // transition point : Tb
			{
				AddPeriod(m_dTb);
				AddAccel(dAg * (1 + (dBeta_0 / m_dQ - 1.0) * m_dTb / m_dTb) * m_dImportance);
			}
			if (Tn > m_dTc + dTol && Tn < m_dTc + dt - dTol) // transition point : Tc
			{
				AddPeriod(m_dTc);
				AddAccel(max(dAg * dBeta_0 / m_dQ * m_dImportance, d02AvgI));
			}
			if (Tn > m_dTd + dTol && Tn < m_dTd + dt - dTol) // transition point : Td
			{
				AddPeriod(m_dTd);
				AddAccel(max(dAg * dBeta_0 / m_dQ * m_dTc * m_dTd / (m_dTd * m_dTd) * m_dImportance, d02AvgI));
			}
			if (Tn > dMaxTn + dTol && Tn < dMaxTn + dt - dTol) // transition point : MaxTn
			{
				AddPeriod(dMaxTn);
				AddAccel(max(dAg * dBeta_0 / m_dQ * m_dTc * m_dTd / (dMaxTn * dMaxTn) * m_dImportance, d02AvgI));
			}
			AddPeriod(Tn);
			AddAccel(SeT);
		}
	}
	else ASSERT(false);

	if (bOnlyCalc) return;

	CString strSpecType[4] = { _T("P100-1(2013) H-ELASTIC"), _T("P100-1(2013) V-ELASTIC"), _T("P100-1(2013) H-DESIGN"), _T("P100-1(2013) V-DESIGN") };
	CString strFuncName = strSpecType[m_nSpecType];
	SetFuncName(std::wstring(strFuncName));

	wchar_t buffer[SpfcUtil::BUFFER_SIZE];
	int result = -1;

	if (m_nSpecType == D_SPFC_P100_2013_SP_HE)
		result = std::swprintf(buffer, SpfcUtil::BUFFER_SIZE, _LS(IDS_CMD_P100_2013_H_ELA_PROFILE), m_dTb, m_dTc, m_dTd, m_dAg, m_dImportance, m_dDamping);
	else if (m_nSpecType == D_SPFC_P100_2013_SP_VE)
		result = std::swprintf(buffer, SpfcUtil::BUFFER_SIZE, _LS(IDS_CMD_P100_2013_V_ELA_PROFILE), m_dTb, m_dTc, m_dTd, m_dAg, m_dImportance, m_dDamping);
	else if (m_nSpecType == D_SPFC_P100_2013_SP_HD)
		result = std::swprintf(buffer, SpfcUtil::BUFFER_SIZE, _LS(IDS_CMD_P100_2013_H_DGN_PROFILE), m_dTb, m_dTc, m_dTd, m_dAg, m_dImportance, m_dQ);
	else if (m_nSpecType == D_SPFC_P100_2013_SP_VD)
		result = std::swprintf(buffer, SpfcUtil::BUFFER_SIZE, _LS(IDS_CMD_P100_2013_V_DGN_PROFILE), m_dTb, m_dTc, m_dTd, m_dAg, m_dImportance, m_dQ);

	if (result >= 0)
	{
		SetDescript(buffer);
	}
}

// \\midasitdev\200_Planmaster\00 Support\00 SPEC(ING)\5300\5324 Gen 루마니아 RS function, Static Seismic Load 반영\5324 루마니아 Demand Spectrum_20170207 Spectrum.pptx 참고
void CSpfcPO2013Util::MakePOSpectrumData(double dDamping, double SRa, double SRv, CArray<double, double>& aPeriod, CArray<double, double>& aAccel)
{
	aPeriod.RemoveAll();
	aAccel.RemoveAll();

	double st = PRD_ST;
	double ed = m_dMaxPeriod * 2;
	int Step = PRD_NUM * 4;
	double Tn, dt;
	double SeT;
	double Tc1 = 0.;   // Tc의 변환점
	const double dMaxTn = 5.0;

	// SRa,SRv는 현재 모든 기준이 공통으로 사용하고 있음
	if (dDamping >= 0)
	{
		SRa = (3.21 - 0.68 * log(dDamping)) / 2.12;
		SRv = (2.31 - 0.41 * log(dDamping)) / 1.65;
	}

	dt = (ed - st) / Step;  // time interval	
	double dTol = dt / Step;  // Tolerance

	int nSpecType = m_nSpecType;
	if (nSpecType == D_SPFC_P100_2013_SP_HE || nSpecType == D_SPFC_P100_2013_SP_VE)
	{
		const double dEta = max(sqrt(10. / (5. + m_dDamping)), 0.55);     // Damping Correction Factor
		const double dBeta_0 = nSpecType == D_SPFC_P100_2013_SP_HE ? 2.5 : 2.75;
		const BOOL bDampingIsNot5 = fabs(m_dDamping - 5.0) > CMathFunc::m_NormalZero;
		if (bDampingIsNot5) ASSERT(0); // PO 커브에서 감쇠력은 5%일 때만 고려함

		Tc1 = m_dTc * SRv / SRa;  // modified Tg

		for (int i = 0; i <= Step; i++)
		{
			Tn = st + dt * i;

			if (Tn >= 0 && Tn < m_dTb)
				SeT = m_dAg * (1 + (dBeta_0 * SRa - 1.0) * Tn / m_dTb) * m_dImportance;
			else if (Tn >= m_dTb && Tn < Tc1)
				SeT = m_dAg * dBeta_0 * m_dImportance * SRa;
			else if (Tn >= Tc1 && Tn < m_dTd)
				SeT = m_dAg * dBeta_0 * m_dTc / Tn * m_dImportance * SRv;
			else if (Tn >= m_dTd && Tn < dMaxTn)
				SeT = m_dAg * dBeta_0 * m_dTc * m_dTd / (Tn * Tn) * m_dImportance * SRv;
			else if (Tn >= dMaxTn)
				SeT = m_dAg * dBeta_0 * m_dTc * m_dTd / (dMaxTn * dMaxTn) * m_dImportance * SRv;
			else
				ASSERT(false);

			//       // 감쇠력이 5%가 아닐 경우는 아래의 식을 통해 Se(T)값을 보정합니다.
			//       if(fabs(m_dDamping-5.0)>CMathFunc::m_NormalZero)
			//       {
			//         if(Tn >= 0 && Tn < m_dTb)
			//         {
			//           SeT = m_dAg + (SeT * dEta - m_dAg) * Tn / m_dTb;
			//         }
			//         else
			//         {
			//           SeT = SeT * dEta;
			//         }
			//       }

			if (Tn > m_dTb + dTol && Tn < m_dTb + dt - dTol) // transition point : Tb
			{
				aPeriod.Add(m_dTb);
				aAccel.Add(m_dAg * (1 + (dBeta_0 * SRa - 1.0) * m_dTb / m_dTb) * m_dImportance);
			}
			if (Tn > Tc1 + dTol && Tn < Tc1 + dt - dTol) // transition point : Tc
			{
				aPeriod.Add(Tc1);
				aAccel.Add(m_dAg * dBeta_0 * m_dImportance * SRa);
			}
			if (Tn > m_dTd + dTol && Tn < m_dTd + dt - dTol) // transition point : Td
			{
				aPeriod.Add(m_dTd);
				aAccel.Add(m_dAg * dBeta_0 * m_dTc / m_dTd * m_dImportance * SRv);
			}
			if (Tn > dMaxTn + dTol && Tn < dMaxTn + dt - dTol) // transition poin : MaxTn
			{
				aPeriod.Add(dMaxTn);
				aAccel.Add(m_dAg * dBeta_0 * m_dTc * m_dTd / (dMaxTn * dMaxTn) * m_dImportance * SRv);
			}
			aPeriod.Add(Tn);
			aAccel.Add(SeT);
		}
	}
	else if (nSpecType == D_SPFC_P100_2013_SP_HD || nSpecType == D_SPFC_P100_2013_SP_VD)  // Design Spectrum : Horizontal(2), Vertical(3)
	{
		const double dBeta_0 = nSpecType == D_SPFC_P100_2013_SP_HD ? 2.5 : 2.75;
		const double d02AvgI = 0.2 * m_dAg * m_dImportance * 0.7;
		Tc1 = m_dTc * SRv / SRa;  // modified Tg

		for (int i = 0; i <= Step; i++)
		{
			Tn = st + dt * i;

			if (Tn >= 0 && Tn < m_dTb)
				SeT = m_dAg * (1 + (dBeta_0 / m_dQ * SRa - 1.0) * Tn / m_dTb) * m_dImportance;
			else if (Tn >= m_dTb && Tn < Tc1)
				SeT = max(m_dAg * dBeta_0 / m_dQ * m_dImportance * SRa, d02AvgI * SRv);
			else if (Tn >= Tc1 && Tn < m_dTd)
				SeT = max(m_dAg * dBeta_0 / m_dQ * m_dTc / Tn * m_dImportance * SRv, d02AvgI * SRv);
			else if (Tn >= m_dTd && Tn < dMaxTn)
				SeT = max(m_dAg * dBeta_0 / m_dQ * m_dTc * m_dTd / (Tn * Tn) * m_dImportance * SRv, d02AvgI * SRv);
			else if (Tn >= dMaxTn)
				SeT = max(m_dAg * dBeta_0 / m_dQ * m_dTc * m_dTd / (dMaxTn * dMaxTn) * m_dImportance * SRv, d02AvgI * SRv);
			else
				ASSERT(false);

			// 감쇠력은 미고려함

			if (Tn > m_dTb + dTol && Tn < m_dTb + dt - dTol) // transition point : Tb
			{
				aPeriod.Add(m_dTb);
				aAccel.Add(m_dAg * (1 + (dBeta_0 / m_dQ * SRa - 1.0) * m_dTb / m_dTb) * m_dImportance);
			}
			if (Tn > Tc1 + dTol && Tn < Tc1 + dt - dTol) // transition point : Tc
			{
				aPeriod.Add(Tc1);
				aAccel.Add(max(m_dAg * dBeta_0 / m_dQ * m_dImportance * SRa, d02AvgI * SRa));
			}
			if (Tn > m_dTd + dTol && Tn < m_dTd + dt - dTol) // transition point : Td
			{
				aPeriod.Add(m_dTd);
				aAccel.Add(max(m_dAg * dBeta_0 / m_dQ * m_dTc * m_dTd / (m_dTd * m_dTd) * m_dImportance * SRv, d02AvgI * SRv));
			}
			if (Tn > dMaxTn + dTol && Tn < dMaxTn + dt - dTol) // transition point : MaxTn
			{
				aPeriod.Add(dMaxTn);
				aAccel.Add(max(m_dAg * dBeta_0 / m_dQ * m_dTc * m_dTd / (dMaxTn * dMaxTn) * m_dImportance * SRv, d02AvgI * SRv));
			}
			aPeriod.Add(Tn);
			aAccel.Add(SeT);
		}
	}
}

void CSpfcPO2013Util::SetParamFromCode(const T_SPFC_CODE& codeParam)
{
	m_nSpecType = codeParam.P100_2013.nSpectrumType;
	m_nParaTypeRdo = codeParam.P100_2013.nParameterType;
	m_dTb = codeParam.P100_2013.dTb;
	m_dTc = codeParam.P100_2013.dTc;
	m_dTd = codeParam.P100_2013.dTd;
	m_dAg = codeParam.P100_2013.dAgr;
	m_dImportance = codeParam.P100_2013.dI;
	m_dDamping = codeParam.P100_2013.dXi;
	m_dQ = codeParam.P100_2013.dQ;
	m_dMaxPeriod = codeParam.P100_2013.dMaxPeriod;
}

void CSpfcPO2013Util::SetParamFromCalc()
{
	// 계산으로부터 설정 코드
}

