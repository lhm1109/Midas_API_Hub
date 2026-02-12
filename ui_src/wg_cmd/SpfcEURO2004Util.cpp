#include "stdafx.h"
#include "SpfcEURO2004Util.h"
#include "../wg_db/wg_db_DBDoc.h"
#include "../wg_db/wg_db_AttrCtrl.h"

CSpfcEURO2004Util::CSpfcEURO2004Util()
{
	m_nGroundType = 0;
	m_nSpecType = 0;

	m_nParaTypeRdo = 0;
	m_dS = 1.2;
	m_dTb = 0.15;
	m_dTc = 0.5;
	m_dTd = 2.0;
	m_dAg = 0.08;
	m_dImportance = 1.0;
	m_dDamping = 5.0;
	m_dQ = 1.5;
	m_dBeta = 0.2;
	m_dMaxPeriod = PRD_ED;

	m_dTs = 0.5;
	m_nNAIndex = EN_SPFC_RECOMMENDED;
	m_nRegion = EN_SPFC_REGION_PENINSULAR;
}

bool CSpfcEURO2004Util::CheckValid()
{
	bool bElastic = false;
	//   if(m_bSingapore) nSpecType+=10;
	if (m_nSpecType == EN_HORI_ELASTIC || m_nSpecType == EN_VERT_ELASTIC || m_nSpecType == EN_HORI_ELASTIC_SIN) bElastic = true;

	if (m_dS <= 0) { AfxMessageBox(_T("S > 0")); return false; }
	if (m_dTb <= 0) { AfxMessageBox(_T("Tb > 0")); return false; }
	if (m_nNAIndex == EN_SPFC_SINGAPORE) //if(m_bSingapore)
	{
		if (m_dTb > m_dTc) { AfxMessageBox(_T("Tc >= Tb")); return false; }
		if (m_dTc > m_dTd) { AfxMessageBox(_T("Td >= Tc")); return false; }
	}
	else
	{
		if (m_dTb >= m_dTc) { AfxMessageBox(_T("Tc > Tb")); return false; }
		if (m_dTc >= m_dTd) { AfxMessageBox(_T("Td > Tc")); return false; }
	}
	if (m_dAg <= 0) { AfxMessageBox(_LS(IDS_CMD_SPECTRUM__GROUND_ACC)); return false; }
	if (bElastic)
	{
		if (m_nNAIndex == EN_SPFC_SINGAPORE) //if(m_bSingapore)
		{
			if (m_dTd > 10.4) { AfxMessageBox(_T("Td <= 10.4")); return false; }
		}
		else
		{
			if (m_dTd >= 4) { AfxMessageBox(_T("Td < 4")); return false; }
		}

		if (m_dDamping < 0) { AfxMessageBox(_LS(IDS_CMD_SPECTRUM__DAMPING_VALUE)); return false; }
	}
	else
	{
		if (m_dQ <= 0) { AfxMessageBox(_T("q > 0")); return false; }
		if (m_dBeta <= 0) { AfxMessageBox(_T("Beta > 0")); return false; }
		if (m_dImportance <= 0) { AfxMessageBox(_T("I > 0")); return false; }
	}
	if (m_dMaxPeriod <= 0) { AfxMessageBox(_LS(IDS_CMD_SPECTRUM__MAX_PERIOD_GREATER_0)); return false; }
	return true;
}

void CSpfcEURO2004Util::MakeSpectrumData(bool bOnlyCalc)
{
	T_SPFC_EURO2004 EURO2004 = GetCodeFromParam().EURO2004;

	BOOL bPushover = false;
	if (bOnlyCalc)
	{
		bPushover = true;
	}

	const double SRa = 1.0;
	const double SRv = 1.0;

	CArray <double, double> arPeriod;
	CArray <double, double> arAccel;

	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	pDoc->m_pAttrCtrl->GetSpectralData_EURO2004(bPushover, SRa, SRv, &EURO2004, &arPeriod, &arAccel);

	InitPeriod();
	InitAccel();

	const auto periodSize = arPeriod.GetSize();
	for (auto idx = 0; idx < periodSize; idx++)
	{
		AddPeriod(arPeriod.GetAt(idx));
	}

	const auto accelSize = arAccel.GetSize();
	for (auto idx = 0; idx < accelSize; idx++)
	{
		AddAccel(arAccel.GetAt(idx));
	}

	if (bOnlyCalc) return;
	wchar_t buffer[SpfcUtil::BUFFER_SIZE];
	int result = -1;

	if (m_nNAIndex == EN_SPFC_SINGAPORE) //if(m_bSingapore)
	{
		CString strSpecType[2] = { _T("EURO2004 H-ELASTIC"), _T("EURO2004 H-DESIGN") };
		CString strGroundType[3] = { _T("C"), _T("D"), _T("S1") };

		int nGroundIx = EN_SPFC_GROUND_C;
		switch (m_nGroundType)
		{
		case EN_SPFC_GROUND_C: nGroundIx = 0; break;
		case EN_SPFC_GROUND_D: nGroundIx = 1; break;
		case EN_SPFC_GROUND_S1: nGroundIx = 2; break;
		default:
			ASSERT(0);
			nGroundIx = 0;
			break;
		}

		if (m_nSpecType == EN_HORI_ELASTIC_SIN)
			result = std::swprintf(buffer, SpfcUtil::BUFFER_SIZE, _LS(IDS_CMD_EURO2003_H_ELA_PROFILE), strGroundType[nGroundIx], m_dS, m_dTb, m_dTc, m_dTd, m_dAg, m_dImportance, m_dDamping);
		else if (m_nSpecType == EN_HORI_DESIGN_SIN)
			result = std::swprintf(buffer, SpfcUtil::BUFFER_SIZE, _LS(IDS_CMD_EURO2003_H_DGN_PROFILE), strGroundType[nGroundIx], m_dS, m_dTb, m_dTc, m_dTd, m_dAg, m_dImportance, m_dQ, m_dBeta);

		const auto strFuncName = strSpecType[m_nSpecType - 10];
		SetFuncName(std::wstring(strFuncName));
	}
	else
	{
		CString strSpecType[4] = { _T("EURO2004 H-ELASTIC"), _T("EURO2004 V-ELASTIC"), _T("EURO2004 H-DESIGN"), _T("EURO2004 V-DESIGN") };
		CString strGroundType[7] = { _T("A"), _T("B"), _T("C"), _T("D"), _T("E"), _T("S1"), _T("S2") };

		if (m_nSpecType == 0)
			result = std::swprintf(buffer, SpfcUtil::BUFFER_SIZE, _LS(IDS_CMD_EURO2003_H_ELA_PROFILE), strGroundType[m_nGroundType], m_dS, m_dTb, m_dTc, m_dTd, m_dAg, m_dImportance, m_dDamping);
		else if (m_nSpecType == 1)
			result = std::swprintf(buffer, SpfcUtil::BUFFER_SIZE, _LS(IDS_CMD_EURO2003_V_ELA_PROFILE), strGroundType[m_nGroundType], m_dS, m_dTb, m_dTc, m_dTd, m_dAg, m_dImportance, m_dDamping);
		else if (m_nSpecType == 2)
			result = std::swprintf(buffer, SpfcUtil::BUFFER_SIZE, _LS(IDS_CMD_EURO2003_H_DGN_PROFILE), strGroundType[m_nGroundType], m_dS, m_dTb, m_dTc, m_dTd, m_dAg, m_dImportance, m_dQ, m_dBeta);
		else if (m_nSpecType == 3)
			result = std::swprintf(buffer, SpfcUtil::BUFFER_SIZE, _LS(IDS_CMD_EURO2003_V_DGN_PROFILE), strGroundType[m_nGroundType], m_dS, m_dTb, m_dTc, m_dTd, m_dAg, m_dImportance, m_dQ, m_dBeta);

		const auto strFuncName = strSpecType[m_nSpecType];
		SetFuncName(std::wstring(strFuncName));
	}

	if (result >= 0)
	{
		SetDescript(buffer);
	}
}

void CSpfcEURO2004Util::MakePOSpectrumData(double dDamping, double SRa, double SRv, CArray<double, double>& aPeriod, CArray<double, double>& aAccel)
{
	T_SPFC_EURO2004 EURO2004 = GetCodeFromParam().EURO2004;

	aPeriod.RemoveAll();
	aAccel.RemoveAll();

	if (dDamping >= 0)
	{
		SRa = (3.21 - 0.68 * log(dDamping)) / 2.12;
		SRv = (2.31 - 0.41 * log(dDamping)) / 1.65;
	}

	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	pDoc->m_pAttrCtrl->GetSpectralData_EURO2004(true, SRa, SRv, &EURO2004, &aPeriod, &aAccel);
}

void CSpfcEURO2004Util::SetParamFromCode(const T_SPFC_CODE& codeParam)
{
	m_nGroundType = codeParam.EURO2004.nGroundType;
	m_nSpecType = codeParam.EURO2004.nSpectrumType;

	m_nParaTypeRdo = codeParam.EURO2004.nParameterType;
	m_dS = codeParam.EURO2004.dSoilFactor;
	m_dTb = codeParam.EURO2004.dTb;
	m_dTc = codeParam.EURO2004.dTc;
	m_dTd = codeParam.EURO2004.dTd;
	m_dAg = codeParam.EURO2004.dAgr;
	m_dImportance = codeParam.EURO2004.dI;
	m_dDamping = codeParam.EURO2004.dXi;
	m_dQ = codeParam.EURO2004.dQ;
	m_dBeta = codeParam.EURO2004.dB;
	m_dMaxPeriod = codeParam.EURO2004.dMaxPeriod;

	m_dTs = codeParam.EURO2004.dTs;
	m_nNAIndex = codeParam.EURO2004.nNationalAnnex;
	m_nRegion = codeParam.EURO2004.nResionType;
}

void CSpfcEURO2004Util::SetParamFromCalc()
{
	// 계산으로부터 설정 코드
}

T_SPFC_CODE CSpfcEURO2004Util::GetCodeFromParam() const
{
	T_SPFC_CODE codeParam;

	codeParam.EURO2004.nGroundType = m_nGroundType;
	codeParam.EURO2004.nSpectrumType = m_nSpecType;
	codeParam.EURO2004.nParameterType = m_nParaTypeRdo;
	codeParam.EURO2004.dSoilFactor = m_dS;
	codeParam.EURO2004.dTb = m_dTb;
	codeParam.EURO2004.dTc = m_dTc;
	codeParam.EURO2004.dTd = m_dTd;
	codeParam.EURO2004.dAgr = m_dAg;
	codeParam.EURO2004.dI = m_dImportance;
	codeParam.EURO2004.dXi = m_dDamping;
	codeParam.EURO2004.dQ = m_dQ;
	codeParam.EURO2004.dB = m_dBeta;
	codeParam.EURO2004.dMaxPeriod = m_dMaxPeriod;
	codeParam.EURO2004.dTs = m_dTs;
	codeParam.EURO2004.nNationalAnnex = m_nNAIndex;
	codeParam.EURO2004.nResionType = m_nRegion;

	return codeParam;
}
