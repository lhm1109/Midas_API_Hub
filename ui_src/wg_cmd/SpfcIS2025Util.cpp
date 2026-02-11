#include "stdafx.h"
#include "SpfcIS2025Util.h"
#include <math.h>

CSpfcIS2025Util::CSpfcIS2025Util()
{
	m_nSiteClass = 0;
	m_nEqZone = 0;
	m_dMaxPeriod = PRD_ED;

	m_dZoneFactor = 0.0375;

	m_dImpFact = 1.0;
	m_dRespReduFact = 1.5;
	m_dDamping = 5.0;
}

bool CSpfcIS2025Util::CheckValid()
{
	if(dgn::LE0(m_dImpFact))
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error__Importance_Fact));
		return false;
	}

	if(m_nSpecType == 0 && dgn::LE0(m_dRespReduFact))
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error__Response_Reduction_Fact));
		return false;
	}
	else if(dgn::LT0(m_dRespReduFact))
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error__Response_Reduction_Fact));
		return false;
	}

	if(dgn::LE(m_dMaxPeriod, 0.0)) { AfxMessageBox(_LS(IDS_CMD_SPECTRUM__MAX_PERIOD_GREATER_0)); return false; }
	return true;
}

void CSpfcIS2025Util::MakeSpectrumData(bool bOnlyCalc)
{
	double st = PRD_ST;
	double ed = m_dMaxPeriod;
	int Step = PRD_NUM;
	// Pushover FEMA440에서 계산할 때(Period 2배, Step 4배)
	if(bOnlyCalc)
	{
		ed *= GetPeriodCoeff();
		Step *= GetStepCoeff();
	}
	double Tn, dt, dSa, dAh;

	dt = ( ed - st ) / Step;  // time interval
	double dTol = dt / Step;  // Tolerance	

	double adHFacTgTo6[] = { 1.0, 1.0, 1.5, 2.0 };
	double adHFac6To10[] = { 6.0, 6.0, 9.0, 12.0 };
	double adTg[] = { 0.4, 0.4, 0.6, 0.8 };
	//double dTInter[] = { 0.40, 0.55, 0.67, 2.0 };
	//double dTg = dTInter[m_nSiteClass];
	double dHFacTgTo6 = adHFacTgTo6[m_nSiteClass];
	double dHFac6To10 = adHFac6To10[m_nSiteClass];
	double dIByR = m_nSpecType == EN_IS2025_SPEC_HOR ? min(dgn::SafeDiv(m_dImpFact, m_dRespReduFact), 1.0) : m_dImpFact;
	double dTg = adTg[m_nSiteClass];

	InitPeriod();
	InitAccel();

	for(int i = 0; i <= Step; i++)
	{
		Tn = st + dt * i;

		if(Tn >= 0.0 && Tn <= 0.01)
			dSa = 1.0;
		else if(Tn > 0.01 && Tn <= 0.1)
			dSa = 1.0 + ( ( 50.0 / 3.0 ) * ( Tn - 0.01 ) );
		else if(Tn > 0.1 && Tn <= dTg)
			dSa = 2.5;
		else if(Tn > dTg && Tn <= 6.0)
			dSa = dgn::SafeDiv(dHFacTgTo6, Tn);
		else
			dSa = dHFac6To10 / Tn / Tn;

		double dDeltaZeta = GetDeltaZeta(Tn);
		double dDeltaVert = GetDeltaVert(Tn);

		dSa *= 1.0;
		dAh = dIByR * dSa * m_dZoneFactor * dDeltaZeta * dDeltaVert;
		if(Tn > 0.01 + dTol && Tn < 0.01 + dt - dTol) // transition point		
		{
			AddPeriod(0.01);
			dDeltaZeta = GetDeltaZeta(0.01);
			dDeltaVert = GetDeltaVert(0.01);
			AddAccel(1.0 * dIByR * m_dZoneFactor * dDeltaZeta * dDeltaVert);
		}
		if(Tn > 0.1 + dTol && Tn < 0.1 + dt - dTol) // transition point		
		{
			AddPeriod(0.1);
			dDeltaZeta = GetDeltaZeta(0.1);
			dDeltaVert = GetDeltaVert(0.1);
			AddAccel(2.5 * dIByR * m_dZoneFactor * dDeltaZeta * dDeltaVert);
		}
		if(Tn > dTg + dTol && Tn < dTg + dt - dTol) // transition point		
		{
			AddPeriod(dTg);
			dDeltaZeta = GetDeltaZeta(dTg);
			dDeltaVert = GetDeltaVert(dTg);
			AddAccel(2.5 * dIByR * m_dZoneFactor * dDeltaZeta * dDeltaVert);
		}
		if(Tn > 6.0 + dTol && Tn < 6.0 + dt - dTol)
		{
			AddPeriod(6.0);
			dDeltaZeta = GetDeltaZeta(6.0);
			dDeltaVert = GetDeltaVert(6.0);
			AddAccel(( dHFacTgTo6 / 6.0 ) * dIByR * m_dZoneFactor * dDeltaZeta * dDeltaVert);
		}
		AddPeriod(Tn);
		AddAccel(dAh);
	}

	if(bOnlyCalc) return;

	CString strZone[] = { _T("II"),_T("III"),_T("IV"),_T("V"), _T("VI") };
	CString strSoil[] = { _T("A"),_T("B"),_T("C"), _T("D") };
	CString strSpecType[] = { _T("Horizontal"),_T("Vertical") };
	CString strRetPeriod[] = { _T("75"),_T("175"),_T("275"),_T("475"),_T("975"),_T("1275"),_T("2475"),_T("4975"),_T("9975") };

	wchar_t buffer[SpfcUtil::BUFFER_SIZE];
	int nResult = 0;
	if(m_nSpecType == 0)
	{
		nResult = std::swprintf(buffer, SpfcUtil::BUFFER_SIZE, _T("IS1893(2025): Type=%s, Zone=%s, Ret. Period=%s, Site Class= %s, Damping=%5.2f, I=%4.2f, R=%4.2f"),
		strSpecType[m_nSpecType], strZone[m_nEqZone], strRetPeriod[m_nRetPeriod], strSoil[m_nSiteClass], m_dDamping, m_dImpFact, m_dRespReduFact);
	}
	else
	{
		nResult = std::swprintf(buffer, SpfcUtil::BUFFER_SIZE, _T("IS1893(2025): Type=%s, Zone=%s, Ret. Period=%s, Site Class= %s, Damping=%5.2f, I=%4.2f"),
		strSpecType[m_nSpecType], strZone[m_nEqZone], strRetPeriod[m_nRetPeriod], strSoil[m_nSiteClass], m_dDamping, m_dImpFact);
	}
	if(nResult >= 0)
	{
		SetDescript(buffer);
	}
	if(m_nSpecType == 0)
	{
		SetFuncName(L"IS1893(2025)-H");
	}
	else if(m_nSpecType == 1)
	{
		SetFuncName(L"IS1893(2025)-V");
	}
	else
	{
		ASSERT(0);
		SetFuncName(L"IS1893(2025)");
	}
}

double CSpfcIS2025Util::GetDeltaZeta(const double dTn)
{
	if(dgn::EQ(m_dDamping, 5.0))
	{
		return 1.0;
	}
	double dTAV = 0.0;
	switch(m_nSiteClass)
	{
	case EN_IS2025_SITE_A:
	case EN_IS2025_SITE_B:
	{
		dTAV = 0.4;
	}
	break;
	case EN_IS2025_SITE_C:
	{
		dTAV = 0.6;
	}
	break;
	case EN_IS2025_SITE_D:
	{
		dTAV = 0.8;
	}
	break;
	default:
		ASSERT(0);
		break;
	}
	double dEtaMax = 0.0;
	if(dgn::EQ0(m_dDamping)) { dEtaMax = 3.2; }
	else if(dgn::LE(m_dDamping, 0.5))
	{
		dEtaMax = 3.2 - ( 268.0 * m_dDamping / 100.0 );
	}
	else if(dgn::LE(m_dDamping, 5.0))
	{
		dEtaMax = pow(( 7.0 / ( 2.0 + m_dDamping ) ), 0.6);
	}
	else if(dgn::LE(m_dDamping, 30.0))
	{
		dEtaMax = pow(( 10.0 / ( 5.0 + m_dDamping ) ), 0.5);
	}
	else
	{
		dEtaMax = pow(( 10.0 / ( 5.0 + 30.0 ) ), 0.5);  //Corresponding to 30% damping
	}


	if(dgn::LE(dTn, 0.01))
	{
		return 1.0;
	}
	else if(dgn::LE(dTn, 0.1))
	{
		return 1.0 + ( ( ( dTn - 0.01 ) / 0.09 ) * ( dEtaMax - 1.0 ) );
	}
	else if(dgn::LE(dTn, dTAV))
	{
		return dEtaMax;
	}
	else if(dgn::LE(dTn, 6.0))
	{
		return dEtaMax - ( ( ( dTn - dTAV ) / ( 6.0 - dTAV ) ) * ( dEtaMax - 1.0 ) );
	}
	else
	{
		return 1.0;
	}
}

double CSpfcIS2025Util::GetDeltaVert(const double dTn)
{
	if(m_nSpecType == EN_IS2025_SPEC_HOR)
	{
		return 1.0;
	}

	double adFirstTerm[] = { 0.8, 0.8, 0.82, 0.85 };
	double adNumerator[] = { 200.0, 200.0, 213.0, 200.0 };
	double adDenominator[] = { 135.0, 135.0, 125.0, 100.0 };

	double dFirstTerm = adFirstTerm[m_nSiteClass];
	double dNumerator = adNumerator[m_nSiteClass];
	double dDenominator = adDenominator[m_nSiteClass];

	if(dgn::LE(dTn, 0.01))
	{
		return dFirstTerm;
	}
	else if(dgn::LE(dTn, 0.1))
	{
		return dFirstTerm - ( ( ( dTn - 0.01 ) * ( dNumerator / dDenominator ) ) );
	}
	else
	{
		return 0.67;
	}
}

void CSpfcIS2025Util::MakePOSpectrumData(double dDamping, double SRa, double SRv, CArray<double, double>& aPeriod, CArray<double, double>& aAccel)
{
	aPeriod.RemoveAll();
	aAccel.RemoveAll();

	double st = PRD_ST;
	double ed = m_dMaxPeriod * 2;
	int Step = PRD_NUM * 4;
	double Tn, dt, dSa, dAh;

	dt = ( ed - st ) / Step;  // time interval
	double dTol = dt / Step;  // Tolerance	

	double adHFacTgTo6[] = { 1.0, 1.0, 1.5, 2.0 };
	double adHFac6To10[] = { 6.0, 6.0, 9.0, 12.0 };
	double adTg[] = { 0.4, 0.4, 0.6, 0.8 };
	//double dTInter[] = { 0.40, 0.55, 0.67, 2.0 };
	//double dTg = dTInter[m_nSiteClass];
	double dHFacTgTo6 = adHFacTgTo6[m_nSiteClass];
	double dHFac6To10 = adHFac6To10[m_nSiteClass];
	double dRespReduFact = m_nSpecType == EN_IS2025_SPEC_HOR ? m_dRespReduFact : 1.0;
	double dIByR = min(m_dImpFact / dRespReduFact, 1.0);
	double dTg = adTg[m_nSiteClass];

	for(int i = 0; i <= Step; i++)
	{
		Tn = st + dt * i;

		if(Tn >= 0.0 && Tn <= 0.01)
			dSa = 1.0;
		else if(Tn > 0.01 && Tn <= 0.1)
			dSa = 1.0 + ( ( 50.0 / 3.0 ) * ( Tn - 0.01 ) );
		else if(Tn > 0.1 && Tn <= dTg)
			dSa = 2.5;
		else if(Tn > dTg && Tn <= 6.0)
			dSa = dgn::SafeDiv(dHFacTgTo6, Tn);
		else
			dSa = dHFac6To10 / Tn / Tn;

		double dDeltaZeta = GetDeltaZeta(Tn);
		double dDeltaVert = GetDeltaVert(Tn);

		dSa *= 1.0;
		dAh = dIByR * dSa * m_dZoneFactor * dDeltaZeta * dDeltaVert;
		if(Tn > 0.01 + dTol && Tn < 0.01 + dt - dTol) // transition point		
		{
			aPeriod.Add(0.01);
			dDeltaZeta = GetDeltaZeta(0.01);
			dDeltaVert = GetDeltaVert(0.01);
			aAccel.Add(1.0 * dIByR * m_dZoneFactor * dDeltaZeta * dDeltaVert);
		}
		if(Tn > 0.1 + dTol && Tn < 0.1 + dt - dTol) // transition point		
		{
			aPeriod.Add(0.1);
			dDeltaZeta = GetDeltaZeta(0.1);
			dDeltaVert = GetDeltaVert(0.1);
			aAccel.Add(2.5 * dIByR * m_dZoneFactor * dDeltaZeta * dDeltaVert);
		}
		if(Tn > dTg + dTol && Tn < dTg + dt - dTol) // transition point		
		{
			aPeriod.Add(dTg);
			dDeltaZeta = GetDeltaZeta(dTg);
			dDeltaVert = GetDeltaVert(dTg);
			aAccel.Add(2.5 * dIByR * m_dZoneFactor * dDeltaZeta * dDeltaVert);
		}
		if(Tn > 6.0 + dTol && Tn < 6.0 + dt - dTol)
		{
			aPeriod.Add(6.0);
			dDeltaZeta = GetDeltaZeta(6.0);
			dDeltaVert = GetDeltaVert(6.0);
			aAccel.Add(( dHFacTgTo6 / 6.0 ) * dIByR * m_dZoneFactor * dDeltaZeta * dDeltaVert);
		}
		aPeriod.Add(Tn);
		aAccel.Add(dAh);
	}
}

void CSpfcIS2025Util::SetParamFromCode(const T_SPFC_CODE& codeParam)
{
	m_nSpecType = codeParam.IS2025.nSpecType;
	m_nEqZone = codeParam.IS2025.nEqZone;
	m_nRetPeriod = codeParam.IS2025.nReturnPeriod;
	m_dZoneFactor = codeParam.IS2025.dZoneFactor;
	m_nSiteClass = codeParam.IS2025.nSiteClass;
	m_dDamping = codeParam.IS2025.dDamping;
	m_dImpFact = codeParam.IS2025.dImpFactor;
	m_dRespReduFact = codeParam.IS2025.dRespRedFact;
	m_dMaxPeriod = codeParam.IS2025.dMaxPeriod;
}

void CSpfcIS2025Util::SetParamFromCalc()
{
	m_dZoneFactor = m_adZoneFacts[m_nEqZone][m_nRetPeriod];
}