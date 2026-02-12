#include "stdafx.h"
#include "SpfcKSBRGUtil.h"
#include "../wg_db/DBDoc.h"

CSpfcKSBRGUtil::CSpfcKSBRGUtil()
{
	m_dRMF = 1.0;
	m_dIF = 1.0;
	m_dEA = 0.11;
	m_dSoil = 1.0;
	m_dMaxPeriod = PRD_ED;

	m_dTs = 0.33;
}

bool CSpfcKSBRGUtil::CheckValid()
{
	if (IsValid(m_dIF, 0, 10) == false)
	{
		return false;
	}

	if (IsValid(m_dRMF, 0, 50) == false)
	{
		return false;
	}

	if (m_dMaxPeriod <= 0)
	{
		const auto pDoc = CDBDoc::GetDocPoint();
		pDoc->DisplayHistoryMessage(_LS(IDS_CMD_SPECTRUM__MAX_PERIOD_GREATER_0));
		return false;
	}

	return true;
}

void CSpfcKSBRGUtil::MakeSpectrumData(bool bOnlyCalc/*=FALSE*/)
{
	//	// Here Area = EPA, Importance = EPV

	double st = PRD_ST;
	double ed = m_dMaxPeriod;
	int Step = PRD_NUM;
	// Pushover FEMA440에서 계산할 때(Period 2배, Step 4배)
	if (bOnlyCalc)
	{
		ed *= GetPeriodCoeff();
		Step *= GetStepCoeff();
	}

	double Tn;
	int n = 0;
	double Csm; // Modal Seismic Design Coefficient
	double critical;

	double dt = (ed - st) / Step;
	double dTol = dt / Step;  // Tolerance

	InitPeriod();
	InitAccel();

	for (int q = 0; q <= Step; q++)
	{
		Tn = st + dt * q;

		//		if ( Tn <= 1.0E-4 ) Tn=1.0E-4;

		if (Tn <= 4.0)
		{
			Csm = 1.2 * m_dEA * m_dIF * m_dSoil / (m_dRMF * pow(Tn, 2. / 3.));


			if (Csm > 2.5 * m_dEA * m_dIF)  Csm = 2.5 * m_dEA * m_dIF;

			critical = pow(1.2 * m_dSoil / (2.5 * m_dRMF), 3. / 2.);
			if (Tn > critical + dTol && Tn - dt < critical - dTol)		// transition point
			{
				AddPeriod(critical);
				AddAccel(1.2 * m_dEA * m_dIF * m_dSoil / (m_dRMF * pow(critical, 2. / 3.)));
			}
		}
		else Csm = 3. * m_dEA * m_dIF * m_dSoil / (m_dRMF * pow(Tn, 4. / 3.));

		AddPeriod(Tn);
		AddAccel(Csm);

	}

	if (bOnlyCalc) return;

	wchar_t buffer[SpfcUtil::BUFFER_SIZE];
	int result = std::swprintf(buffer, SpfcUtil::BUFFER_SIZE, _LS(IDS_WG_CMD__ADD2__KS_Bridge___Soil____4_2f__Area___), m_dSoil, m_dEA, m_dIF, m_dRMF);
	if (result >= 0)
	{
		SetDescript(buffer);
	}
	SetFuncName(L"KS-Bridge");

	////	m_strFuncName=_LS(IDS_WG_CMD__ADD2__KS_Bridge_MSG1);
	//	//SetAxisLabelText(1,_T("Csm/R"));		
	////  GSaveHistoryFormat(_T("KS Bridge : Soil = %3.1f  Area = %4.2f  I = %4.2f  R = %4.2f"),m_dSoil,m_dEA,m_dIF,m_dRMF);

}

void CSpfcKSBRGUtil::MakePOSpectrumData(double dDamping, double SRa, double SRv, CArray <double, double>& aPeriod, CArray <double, double>& aAccel)// for Pushover Curve
{
	//	// Here Area = EPA, Importance = EPV

	double st = PRD_ST;
	double ed = m_dMaxPeriod * 2;
	int Step = PRD_NUM * 4;

	if (dDamping >= 0)
	{
		SRa = (3.21 - 0.68 * log(dDamping)) / 2.12;
		SRv = (2.31 - 0.41 * log(dDamping)) / 1.65;
	}

	double Tn;
	int n = 0;
	double Csm; // Modal Seismic Design Coefficient
	double critical;

	double dt = (ed - st) / Step;
	double dTol = dt / Step;  // Tolerance

	for (int q = 0; q <= Step; q++)
	{
		Tn = st + dt * q;

		//		if ( Tn <= 1.0E-4 ) Tn=1.0E-4;

		if (Tn <= 4.0)
		{
			Csm = 1.2 * m_dEA * m_dIF * m_dSoil / (m_dRMF * pow(Tn, 2. / 3.)) * SRv;


			if (Csm > 2.5 * m_dEA * m_dIF * SRa)  Csm = 2.5 * m_dEA * m_dIF * SRa;

			critical = pow(1.2 * m_dSoil * SRv / (2.5 * SRa * m_dRMF), 3. / 2.);
			if (Tn > critical + dTol && Tn - dt < critical + dTol)		// transition point
			{
				aPeriod.Add(critical);
				aAccel.Add(1.2 * m_dEA * m_dIF * m_dSoil * SRv / (m_dRMF * pow(critical, 2. / 3.)));
			}
		}
		else Csm = 3. * m_dEA * m_dIF * m_dSoil / (m_dRMF * pow(Tn, 4. / 3.)) * SRv;

		aPeriod.Add(Tn);
		aAccel.Add(Csm);
	}
}

void CSpfcKSBRGUtil::SetParamFromCode(const T_SPFC_CODE& codeParam)
{
	m_dRMF = codeParam.KSBRG.dCoef;
	m_dIF = codeParam.KSBRG.dIe;
	m_dEA = codeParam.KSBRG.dEPA;
	m_dSoil = codeParam.KSBRG.dSoil;
	m_dMaxPeriod = codeParam.KSBRG.dMaxPeriod;
}

void CSpfcKSBRGUtil::SetParamFromCalc()
{
	if (m_dSoil == 1.0) m_dTs = 0.33;
	else if (m_dSoil == 1.2) m_dTs = 0.44;
	else if (m_dSoil == 1.5) m_dTs = 0.61;
	else if (m_dSoil == 2.0) m_dTs = 0.94;
}

double CSpfcKSBRGUtil::GetRMC() const
{
	return m_dRMF;
}

double CSpfcKSBRGUtil::GetOIF() const
{
	return m_dIF;
}

double CSpfcKSBRGUtil::GetEPA() const
{
	return m_dEA;
}

double CSpfcKSBRGUtil::GetSoil() const
{
	return m_dSoil;
}

double CSpfcKSBRGUtil::GetMaxPeriod() const
{
	return m_dMaxPeriod;
}

double CSpfcKSBRGUtil::GetTs() const
{
	return m_dTs;
}

void CSpfcKSBRGUtil::SetRMC(double rmc)
{
	m_dRMF = rmc;
}

void CSpfcKSBRGUtil::SetOIF(double oif)
{
	m_dIF = oif;
}

void CSpfcKSBRGUtil::SetEPA(double epa)
{
	m_dEA = epa;
}

void CSpfcKSBRGUtil::SetSoil(double soil)
{
	m_dSoil = soil;
}

void CSpfcKSBRGUtil::SetMaxPeriod(double maxPeriod)
{
	m_dMaxPeriod = maxPeriod;
}

void CSpfcKSBRGUtil::SetTs(double dTs)
{
	m_dTs = dTs;
}
