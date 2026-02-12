#include "stdafx.h"
#include "SpfcJPBRG2002Util.h"
#include "../wg_db/wg_db_DBDoc.h"
#include "../wg_db/wg_db_UnitCtrl.h"
#include "../wg_db/wg_db_InitCtrl.h"

CSpfcJPBRG2002Util::CSpfcJPBRG2002Util()
{
	m_nMethod = 0;
	m_nSoil = 0;
	m_nZone = 0;
	m_dMaxPeriod = PRD_ED;
	m_dCz = 1.0;
	m_dDamping = 0.05;

	m_dCd = 0.;
}

bool CSpfcJPBRG2002Util::CheckValid()
{
	if (m_dDamping < 0) { AfxMessageBox(_LS(IDS_CMD_SPECTRUM__DAMPING_VALUE)); return false; }
	if (m_dMaxPeriod <= 0) { AfxMessageBox(_LS(IDS_CMD_SPECTRUM__MAX_PERIOD_GREATER_0)); return false; }
	return true;
}

void CSpfcJPBRG2002Util::MakeSpectrumData(bool bOnlyCalc)
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
	double Tn, dt, dSo;

	dt = (ed - st) / Step;  // time interval
	double dTol = dt / Step;  // Tolerance    
	double dFactor = m_dCz * m_dCd;

	double dT1, dT2, dT3, dSm, dS1, dS2, dS3, dE1, dE2;
	double dValue[7], dExp[2];

	GetSpectrumValue(dValue, dExp, m_nMethod);
	dT1 = dValue[0];  // 0.05
	dT2 = dValue[1];  // 0.1
	dT3 = dValue[2];  // 1.1
	dSm = dValue[3];  // 160.
	dS1 = dValue[4];  // 431.
	dS2 = dValue[5];  // 200.
	dS3 = dValue[6];  // 220.
	dE1 = dExp[0];    // 1./3.
	dE2 = dExp[1];    // 1.0

	InitPeriod();
	InitAccel();

	for (int i = 0; i <= Step; i++)
	{
		Tn = st + dt * i;

		if (Tn >= 0. && Tn < dT1)		  	 dSo = dSm;
		else if (Tn >= dT1 && Tn < dT2)	 dSo = max(dSm, dS1 * pow(Tn, dE1));
		else if (Tn >= dT2 && Tn < dT3)   dSo = dS2;
		else			                       dSo = dS3 / pow(Tn, dE2);

		if (Tn > dT1 + dTol && Tn < dT1 + dt - dTol) // transition point		
		{
			AddPeriod(dT1);
			AddAccel(dSm * dFactor);
		}
		if (Tn > dT2 + dTol && Tn < dT2 + dt - dTol) // transition point		
		{
			AddPeriod(dT2);
			AddAccel(dS2 * dFactor);
		}
		if (Tn > dT3 + dTol && Tn < dT3 + dt - dTol) // transition point		
		{
			AddPeriod(dT3);
			AddAccel(dS2 * dFactor);
		}

		AddPeriod(Tn);
		AddAccel(dSo * dFactor);
	}

	if (bOnlyCalc) return;

	CString strMethod[] = { _LS(IDC_CMD_GDS_JPBRG1), _LS(IDC_CMD_GDS_JPBRG2), _LS(IDC_CMD_GDS_JPBRG3) };
	CString strZone[] = { _LSX(A), _LSX(B), _LSX(C) };
	CString strSoil[] = { _LSX(I), _LSX(II), _LSX(III) };

	wchar_t buffer[SpfcUtil::BUFFER_SIZE];
	int result = std::swprintf(buffer, SpfcUtil::BUFFER_SIZE, _T("JP-Bridge02: Method=%s, Zone=%s, Soil=%s, Damp=%4.2f"), strMethod[m_nMethod], strZone[m_nZone], strSoil[m_nSoil], m_dDamping);

	if (result >= 0)
	{
		SetDescript(buffer);
	}

	SetFuncName(L"JP-Bridge02");
}

void CSpfcJPBRG2002Util::MakePOSpectrumData(double dDamping, double SRa, double SRv, CArray<double, double>& aPeriod, CArray<double, double>& aAccel)
{
	aPeriod.RemoveAll();
	aAccel.RemoveAll();

	double st = PRD_ST;
	double ed = m_dMaxPeriod * 2;
	int Step = PRD_NUM * 4;
	double Tn, dt, dSo;
	double dFactor = m_dCz * m_dCd;
	double dTg;

	if (dDamping >= 0)
	{
		SRa = (3.21 - 0.68 * log(dDamping)) / 2.12;
		SRv = (2.31 - 0.41 * log(dDamping)) / 1.65;
	}

	dt = (ed - st) / Step;  // time interval	
	double dTol = dt / Step;  // Tolerance

	double dT1, dT2, dT3, dSm, dS1, dS2, dS3, dE1, dE2;
	double dValue[7], dExp[2];

	GetSpectrumValue(dValue, dExp, m_nMethod);
	dT1 = dValue[0];  // 0.05
	dT2 = dValue[1];  // 0.1
	dT3 = dValue[2];  // 1.1
	dSm = dValue[3];  // 160.
	dS1 = dValue[4];  // 431.
	dS2 = dValue[5];  // 200.
	dS3 = dValue[6];  // 220.
	dE1 = dExp[0];    // 1./3.
	dE2 = dExp[1];    // 1.0

	dTg = pow((SRv / SRa) * (dS3 / dS2), 1.0 / dE2);

	for (int i = 0; i <= Step; i++)
	{
		Tn = st + dt * i;

		if (Tn >= 0. && Tn < dT1)		  	 dSo = SRa * dSm;
		else if (Tn >= dT1 && Tn < dT2)   dSo = SRa * max(dSm, dS1 * pow(Tn, dE1));
		else if (Tn >= dT2 && Tn < dTg)   dSo = dS2 * SRa;
		else			                       dSo = dS3 / pow(Tn, dE2) * SRv;

		if (Tn > dT1 + dTol && Tn < dT1 + dt - dTol) // transition point		
		{
			aPeriod.Add(dT1);
			aAccel.Add(dSm * dFactor * SRa);
		}
		if (Tn > dT2 + dTol && Tn < dT2 + dt - dTol) // transition point		
		{
			aPeriod.Add(dT2);
			aAccel.Add(dS2 * dFactor * SRa);
		}
		if (Tn > dTg + dTol && Tn < dTg + dt - dTol) // transition point		
		{
			aPeriod.Add(dTg);
			aAccel.Add(dS2 * dFactor * SRa);
		}

		aPeriod.Add(Tn);
		aAccel.Add(dSo * dFactor);
	}
}

void CSpfcJPBRG2002Util::SetParamFromCode(const T_SPFC_CODE& codeParam)
{
	m_nMethod = codeParam.JPBRG2002.nMethod;
	m_nZone = codeParam.JPBRG2002.nSeisZone;
	m_nSoil = codeParam.JPBRG2002.nSoilClass;
	m_dCz = codeParam.JPBRG2002.dCz;
	m_dDamping = codeParam.JPBRG2002.dDamping;
	m_dMaxPeriod = codeParam.JPBRG2002.dMaxPeriod;
}

void CSpfcJPBRG2002Util::SetParamFromCalc()
{
	m_dCd = 1.5 / (40.0 * m_dDamping + 1.0) + 0.5;
}

void CSpfcJPBRG2002Util::GetSpectrumValue(double* dValue, double* dExp, int nMethod)
{
	double dTable1[3][7] = { {0.05, 0.1 , 1.1, 160.,  431.,  200.,  220.},
												{0.1,  0.2 , 1.3, 200.,  427.,  250.,  325.},
												{0.17, 0.34, 1.5, 240.,  430.,  300.,  450.} };
	double dTable2[3][7] = { {0.0,  0.0 , 1.4,   0.,    0.,  700.,  980.},
													{0.1,  0.18, 1.6, 700., 1505.,  850., 1360.},
													{0.1,  0.29, 2.0, 700., 1511., 1000., 2000.} };
	double dTable3[3][7] = { {0.0,  0.3 , 0.7,   0., 4463., 2000., 1104.},
													{0.0,  0.4 , 1.2,   0., 3224., 1750., 2371.},
													{0.0,  0.5 , 1.5,   0., 2381., 1500., 2948.} };
	int i;
	switch (nMethod)
	{
	case 0:
		for (i = 0; i < 7; i++) dValue[i] = dTable1[m_nSoil][i];
		dExp[0] = 1. / 3.;
		dExp[1] = 1.0;
		break;
	case 1:
		for (i = 0; i < 7; i++) dValue[i] = dTable2[m_nSoil][i];
		dExp[0] = 1. / 3.;
		dExp[1] = 1.0;
		break;
	case 2:
		for (i = 0; i < 7; i++) dValue[i] = dTable3[m_nSoil][i];
		dExp[0] = 2. / 3.;
		dExp[1] = 5. / 3.;
		break;
	}

	// cm/sec^2(gal)에서 g(중력가속도)로 단위 변환  	
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	T_UNIT_INDEX UnitIndex;
	double dGravity = pDoc->m_pInitCtrl->DefaultGravity();
	pDoc->m_pUnitCtrl->GetUnitIndexCurrent(UnitIndex);

	switch (UnitIndex.nBase_Length)
	{
	case D_UNITSYS_LENGTH_INDEX_MM:
		dGravity = dGravity * D_UNITSYS_LENGTH_FACTOR_MM;
		break;
	case D_UNITSYS_LENGTH_INDEX_CM:
		dGravity = dGravity * D_UNITSYS_LENGTH_FACTOR_CM;
		break;
	case D_UNITSYS_LENGTH_INDEX_M:
		dGravity = dGravity * D_UNITSYS_LENGTH_FACTOR_M;
		break;
	case D_UNITSYS_LENGTH_INDEX_IN:
		dGravity = dGravity * D_UNITSYS_LENGTH_FACTOR_IN;
		break;
	case D_UNITSYS_LENGTH_INDEX_FT:
		dGravity = dGravity * D_UNITSYS_LENGTH_FACTOR_FT;
		break;
	}
	if (dGravity == 0.) dGravity = 1.;
	for (i = 3; i < 7; i++) dValue[i] = 0.01 * dValue[i] / dGravity;
}
