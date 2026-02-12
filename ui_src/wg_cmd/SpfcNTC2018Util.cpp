#include "stdafx.h"
#include "SpfcNTC2018Util.h"
#include "../wg_db/wg_db_DBDoc.h"
#include "../wg_db/wg_db_AttrCtrl.h"


CSpfcNTC2018Util::CSpfcNTC2018Util(int nCase)
	: m_nCase(nCase)
{
	m_nParaTypeRdo = 0;

	m_dS = 1.2;
	m_dTb = 0.15;
	m_dTc = 0.5;
	m_dTd = 2.0;
	m_dAg = 0.08;
	m_dFo = 2.5;
	m_dTcStar = 0.3;
	m_dDamping = 5.0;
	m_dQ = 1.5;

	m_nSpecType = 0;
	m_nGroundType = 1;
	m_dMaxPeriod = PRD_ED;
}

bool CSpfcNTC2018Util::CheckValid()
{
	bool bElastic = false;
	DWORD nSpecType = m_nSpecType;

	if (nSpecType == EN_HORI_ELASTIC || nSpecType == EN_VERT_ELASTIC) bElastic = TRUE;

	if (m_dS <= 0) { AfxMessageBox(_T("S > 0")); return false; }
	if (m_dTb <= 0) { AfxMessageBox(_T("Tb > 0")); return false; }
	if (m_dTb >= m_dTc) { AfxMessageBox(_T("Tc > Tb")); return false; }
	if (m_dTc >= m_dTd) { AfxMessageBox(_T("Td > Tc")); return false; }
	if (m_dAg <= 0) { AfxMessageBox(_LS(IDS_CMD_SPECTRUM__GROUND_ACC)); return false; }
	if (m_dFo <= 0) { AfxMessageBox(_T("Fo > 0")); return false; }
	if (m_dTcStar <= 0) { AfxMessageBox(_T("Tc* > 0")); return false; }
	if (bElastic)
	{
		if (m_dDamping < 0) { AfxMessageBox(_LS(IDS_CMD_SPECTRUM__DAMPING_VALUE)); return false; }
	}
	else
	{
		if (m_dQ <= 0) { AfxMessageBox(_T("q > 0")); return false; }
	}
	if (m_dMaxPeriod <= 0) { AfxMessageBox(_LS(IDS_CMD_SPECTRUM__MAX_PERIOD_GREATER_0)); return false; }
	return true;
}

void CSpfcNTC2018Util::MakeSpectrumData(bool bOnlyCalc /*= false*/)
{
	T_SPFC_NTC2018 NTC2018 = GetCodeFromParam();

	BOOL bPushover = FALSE;
	if (bOnlyCalc)
	{
		bPushover = TRUE;
	}

	double SRa = 1.0;
	double SRv = 1.0;

	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	CArray <double, double> arPeriod;
	CArray <double, double> arAccel;
	pDoc->m_pAttrCtrl->GetSpectralData_NTC2018(bPushover, SRa, SRv, &NTC2018, &arPeriod, &arAccel);

	InitPeriod();
	InitAccel();

	for (int i = 0; i < arPeriod.GetSize(); i++)
	{
		AddPeriod(arPeriod[i]);
	}

	for (int i = 0; i < arAccel.GetSize(); i++)
	{
		AddAccel(arAccel[i]);
	}

	if (bOnlyCalc) return;

	CString strSpecType[4] = { _T("NTC2018 H-ELASTIC"), _T("NTC2018 V-ELASTIC"), _T("NTC2018 H-DESIGN"), _T("NTC2018 V-DESIGN") };
	CString strGroundType[5] = { _T("A"), _T("B"), _T("C"), _T("D"), _T("E") };

	wchar_t buffer[SpfcUtil::BUFFER_SIZE];
	int result = -1;
	
	if (m_nSpecType == 0)
		result = std::swprintf(buffer, SpfcUtil::BUFFER_SIZE, _LS(IDS_CMD_NTC2018_H_ELA_PROFILE), strGroundType[m_nGroundType], m_dS, m_dTb, m_dTc, m_dTd, m_dAg, m_dFo, m_dTcStar, m_dDamping);
	else if (m_nSpecType == 1)
		result = std::swprintf(buffer, SpfcUtil::BUFFER_SIZE, _LS(IDS_CMD_NTC2018_V_ELA_PROFILE), strGroundType[m_nGroundType], m_dS, m_dTb, m_dTc, m_dTd, m_dAg, m_dFo, m_dTcStar, m_dDamping);
	else if (m_nSpecType == 2)
		result = std::swprintf(buffer, SpfcUtil::BUFFER_SIZE, _LS(IDS_CMD_NTC2018_H_DGN_PROFILE), strGroundType[m_nGroundType], m_dS, m_dTb, m_dTc, m_dTd, m_dAg, m_dFo, m_dTcStar, m_dQ);
	else if (m_nSpecType == 3)
		result = std::swprintf(buffer, SpfcUtil::BUFFER_SIZE, _LS(IDS_CMD_NTC2018_V_DGN_PROFILE), strGroundType[m_nGroundType], m_dS, m_dTb, m_dTc, m_dTd, m_dAg, m_dFo, m_dTcStar, m_dQ);

	if (result >= 0)
	{
		SetDescript(buffer);
	}

	SetFuncName(strSpecType[m_nSpecType].GetString());
}

void CSpfcNTC2018Util::MakePOSpectrumData(double dDamping, double SRa, double SRv, CArray <double, double>& aPeriod, CArray <double, double>& aAccel)
{
	T_SPFC_NTC2018 NTC2018 = GetCodeFromParam();

	aPeriod.RemoveAll();
	aAccel.RemoveAll();

	if (dDamping >= 0)
	{
		SRa = (3.21 - 0.68 * log(dDamping)) / 2.12;
		SRv = (2.31 - 0.41 * log(dDamping)) / 1.65;
	}

	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	pDoc->m_pAttrCtrl->GetSpectralData_NTC2018(TRUE, SRa, SRv, &NTC2018, &aPeriod, &aAccel);
}

void CSpfcNTC2018Util::SetParamFromCode(const T_SPFC_CODE& codeParam)
{
	m_nGroundType = codeParam.NTC2018.nGroundType;
	m_nSpecType = codeParam.NTC2018.nSpectrumType;
	m_nParaTypeRdo = codeParam.NTC2018.nParameterType;
	m_dS = codeParam.NTC2018.dSoilFactor;
	m_dTb = codeParam.NTC2018.dTb;
	m_dTc = codeParam.NTC2018.dTc;
	m_dTd = codeParam.NTC2018.dTd;
	m_dAg = codeParam.NTC2018.dAg;
	m_dFo = codeParam.NTC2018.dFo;
	m_dTcStar = codeParam.NTC2018.dTcStar;
	m_dDamping = codeParam.NTC2018.dXi;
	m_dQ = codeParam.NTC2018.dQ;
	m_dMaxPeriod = codeParam.NTC2018.dMaxPeriod;
}

void CSpfcNTC2018Util::CtrlManager()
{
	BOOL bElastic = false;
	BOOL bVertical = false;
	BOOL bUserDefined = false;
	BOOL bAutomatic = TRUE;

	DWORD nSpecType = m_nSpecType;
	if(m_nParaTypeRdo == 4) bUserDefined = TRUE;

	if (nSpecType == EN_HORI_ELASTIC || nSpecType == EN_VERT_ELASTIC) bElastic = TRUE;
	if (nSpecType == EN_VERT_ELASTIC || nSpecType == EN_VERT_DESIGN) bVertical = TRUE;
	if (m_nParaTypeRdo == 4) bUserDefined = TRUE;
	if (bUserDefined) bAutomatic = false;

	// Ground Type과 Spectrum Parameter Type에 따른 S, Tb, Tc, Td의 자동설정
	static double T_HORIZONTAL[4][5][7] = {
		{ // Type1
			// SS_min, SS_max, SS_1, SS_2, ST, CC_1, CC_2
			{1.00, 1.00, 1.00, 0.00, 1.00, 1.00,  0.00},  // A
			{1.00, 1.20, 1.40, 0.40, 1.00, 1.10, -0.20},  // B
			{1.00, 1.50, 1.70, 0.60, 1.00, 1.05, -0.33},  // C
			{0.90, 1.80, 2.40, 1.50, 1.00, 1.25, -0.50},  // D
			{1.00, 1.60, 2.00, 1.10, 1.00, 1.15, -0.40},  // E      
		},
		{ // Type2
			// SS_min, SS_max, SS_1, SS_2, ST, CC_1, CC_2
			{1.00, 1.00, 1.00, 0.00, 1.20, 1.00,  0.00},  // A
			{1.00, 1.20, 1.40, 0.40, 1.20, 1.10, -0.20},  // B
			{1.00, 1.50, 1.70, 0.60, 1.20, 1.05, -0.33},  // C
			{0.90, 1.80, 2.40, 1.50, 1.20, 1.25, -0.50},  // D
			{1.00, 1.60, 2.00, 1.10, 1.20, 1.15, -0.40},  // E      
		},
		{ // Type3
			// SS_min, SS_max, SS_1, SS_2, ST, CC_1, CC_2
			{1.00, 1.00, 1.00, 0.00, 1.20, 1.00,  0.00},  // A
			{1.00, 1.20, 1.40, 0.40, 1.20, 1.10, -0.20},  // B
			{1.00, 1.50, 1.70, 0.60, 1.20, 1.05, -0.33},  // C
			{0.90, 1.80, 2.40, 1.50, 1.20, 1.25, -0.50},  // D
			{1.00, 1.60, 2.00, 1.10, 1.20, 1.15, -0.40},  // E      
		},
		{ // Type4
			// SS_min, SS_max, SS_1, SS_2, ST, CC_1, CC_2
			{1.00, 1.00, 1.00, 0.00, 1.40, 1.00,  0.00},  // A
			{1.00, 1.20, 1.40, 0.40, 1.40, 1.10, -0.20},  // B
			{1.00, 1.50, 1.70, 0.60, 1.40, 1.05, -0.33},  // C
			{0.90, 1.80, 2.40, 1.50, 1.40, 1.25, -0.50},  // D
			{1.00, 1.60, 2.00, 1.10, 1.40, 1.15, -0.40},  // E      
		},
	};

	static double T_VERTICAL[5][4] = {
		// S    Tb    Tc    Td  
		{1.00, 0.05, 0.15, 1.0},  // A
		{1.00, 0.05, 0.15, 1.0},  // B
		{1.00, 0.05, 0.15, 1.0},  // C
		{1.00, 0.05, 0.15, 1.0},  // D
		{1.00, 0.05, 0.15, 1.0},  // E
	};

	double dAFactor = m_dFo; // the factor that quantifies the maximum spectral amplification
	if (nSpecType == EN_VERT_ELASTIC || nSpecType == EN_VERT_DESIGN) dAFactor = 1.35 * m_dFo * pow(m_dAg, 0.5);

	int nGroundType = m_nGroundType;
	if (bAutomatic)
	{
		if (bVertical)
		{
			const int nStSize = 4;
			double dSt[nStSize] = { 1.0, 1.2, 1.2, 1.4 };
			if (m_nParaTypeRdo <= nStSize)
				m_dS = T_VERTICAL[nGroundType][0] * dSt[m_nParaTypeRdo];
			else { ASSERT(0); m_dS = T_VERTICAL[nGroundType][0]; }

			m_dTb = T_VERTICAL[nGroundType][1];
			m_dTc = T_VERTICAL[nGroundType][2];
			m_dTd = T_VERTICAL[nGroundType][3];
		}
		else
		{
			double dSS_calc = T_HORIZONTAL[m_nParaTypeRdo][nGroundType][2] - T_HORIZONTAL[m_nParaTypeRdo][nGroundType][3] * dAFactor * m_dAg;
			double dCC = T_HORIZONTAL[m_nParaTypeRdo][nGroundType][5] * pow(m_dTcStar, T_HORIZONTAL[m_nParaTypeRdo][nGroundType][6]);
			double dSS = max(T_HORIZONTAL[m_nParaTypeRdo][nGroundType][0], min(T_HORIZONTAL[m_nParaTypeRdo][nGroundType][1], dSS_calc));
			m_dS = dSS * T_HORIZONTAL[m_nParaTypeRdo][nGroundType][4];
			m_dTc = dCC * m_dTcStar;
			m_dTb = m_dTc / 3;
			m_dTd = 4.0 * m_dAg + 1.6;
		}
	}
	else
	{
		if (bVertical)
		{
			m_dS = 1.0;
		}
	}
}

T_SPFC_NTC2018 CSpfcNTC2018Util::GetCodeFromParam() const
{
	T_SPFC_NTC2018 NTC2018;
	NTC2018.Initialize();

	NTC2018.nSpectrumType = m_nSpecType;
	NTC2018.nGroundType = m_nGroundType;

	// Input Data
	NTC2018.nParameterType = m_nParaTypeRdo;
	NTC2018.dSoilFactor = m_dS;
	NTC2018.dTb = m_dTb;
	NTC2018.dTc = m_dTc;
	NTC2018.dTd = m_dTd;
	NTC2018.dAg = m_dAg;
	NTC2018.dFo = m_dFo;
	NTC2018.dTcStar = m_dTcStar;
	NTC2018.dXi = m_dDamping;
	NTC2018.dQ = m_dQ;
	NTC2018.dMaxPeriod = m_dMaxPeriod;

	return NTC2018;
}
