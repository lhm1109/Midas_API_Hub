#include "stdafx.h"
#include "SpfcGB50111_2006Util.h"

CSpfcGB50111_2006Util::CSpfcGB50111_2006Util()
{
	m_nDivision = 0;
	m_nSiteClass = 0;
	m_nSFI = 0;
	m_nEarthEff = 0;
	m_dMaxPeriod = PRD_ED;
	m_dTg = 0.25;
	m_dG = 0.02;
	m_bImportantBridge = FALSE;
}

bool CSpfcGB50111_2006Util::CheckValid()
{
	if (IsValid(m_dTg, 0.) == false)
	{
		return false;
	}

	if (IsValid(m_dG, 0.) == false)
	{
		return false;
	}

	if ( m_dMaxPeriod <= 0 ) { AfxMessageBox(_LS(IDS_CMD_SPECTRUM__MAX_PERIOD_GREATER_0)); return false; }

	return true;
}

void CSpfcGB50111_2006Util::MakeSpectrumData(bool bOnlyCalc)
{
	double  st = PRD_ST;
	double  ed = m_dMaxPeriod;
	int     Step = PRD_NUM;
	double  Tn , dt;

	// Pushover FEMA440에서 계산할 때(Period 2배, Step 4배)
	if ( bOnlyCalc )
	{
		ed *= GetPeriodCoeff();
		Step *= GetStepCoeff();
	}

	dt = ( ed - st ) / Step;      // time interval	
	double dTol = dt / Step;  // Tolerance

	double Alpha = m_dG;
	double Beta = 0.0;

	InitPeriod();
	InitAccel();

	for ( int i = 0; i <= Step; i++ )
	{
		Tn = st + dt * i;
		//		if ( Tn <= 1.0E-4 ) Tn=1.0E-4;
		if ( Tn >= 0 && Tn < 0.1 )
			Beta = 1.0 + ( ( 2.25 - 1.0 ) * ( Tn * 10 ) );
		else if ( Tn >= 0.1 && Tn < m_dTg )
			Beta = 2.25;
		else if ( Tn >= m_dTg && Tn < 5 * m_dTg )
			Beta = 2.25 * ( m_dTg / Tn );
		else if ( Tn >= 5 * m_dTg )
			Beta = 0.45;

		if ( Tn > 0.1 + dTol && Tn < 0.1 + dt - dTol ) // transition point		
		{
			AddPeriod(0.1);
			AddAccel(Alpha * 2.25);
		}
		if ( Tn > m_dTg + dTol && Tn < m_dTg + dt - dTol ) // transition point		
		{
			AddPeriod(m_dTg);
			AddAccel(Alpha * 2.25);
		}
		if ( Tn > 5 * m_dTg + dTol && Tn < 5 * m_dTg + dt - dTol ) // transition point		
		{
			AddPeriod(5 * m_dTg);
			AddAccel(Alpha * 0.45);
		}

		AddPeriod(Tn);
		AddAccel(Alpha * Beta);
	}

	if ( bOnlyCalc ) return;

	CString strSFI [ ] = { _T("6(0.05g)"),_T("7(0.10g)"),_T("7(0.15g)"),_T("8(0.20g)"),_T("8(0.30g)"),_T("9(0.40g)") };
	CString strSC [ ] = { _T("I"), _T("II"), _T("III"), _T("IV") };
	CString strER [ ] = { _LS(IDS_CMD0417__Frequent_E_Q_), _LS(IDS_CMD0417__Scarce_E_Q_) };

	//m_sDescript.Format(_LS(IDS_WG_CMD__ADD_GB50111_2006_PROFILE), 	m_nDivision+1, strSFI[m_nSFI], strSC[m_nSiteClass], strER[m_nEarthEff], m_dTg, m_dXi);

	//wchar_t buffer [ SpfcUtil::BUFFER_SIZE ];
	//int result = std::swprintf(buffer , SpfcUtil::BUFFER_SIZE , _LS(IDS_WG_CMD__ADD_GB50111_2006_PROFILE) , m_nDivision + 1 , strSFI [ m_nSFI ] , strSC [ m_nSiteClass ] , strER [ m_nEarthEff ] , m_dTg , m_dXi);
	//if ( result >= 0 )
	//{
	//	SetDescript(buffer);
	//}

	SetFuncName(L"China(GB50111-06)");
}

void CSpfcGB50111_2006Util::MakePOSpectrumData(double dDamping , double SRa , double SRv , CArray<double , double>& aPeriod , CArray<double , double>& aAccel)
{
	aPeriod.RemoveAll();
	aAccel.RemoveAll();

	double st = PRD_ST;
	double ed = m_dMaxPeriod * 2;
	int Step = PRD_NUM * 4;
	double Tn , dt;
	//double Tg1;   // Tc의 변환점

	if ( dDamping >= 0 )
	{
		SRa = ( 3.21 - 0.68 * log(dDamping) ) / 2.12;
		SRv = ( 2.31 - 0.41 * log(dDamping) ) / 1.65;
	}

	dt = ( ed - st ) / Step;  // time interval	
	double dTol = dt / Step;  // Tolerance

	//Tg1 = m_dTg * pow(SRv / SRa, 1.0 / m_dGamma);  // modified Tg

	double Alpha = m_dG;
	double Beta = 0.0;

	for ( int i = 0; i <= Step; i++ )
	{
		Tn = st + dt * i;
		//		if ( Tn <= 1.0E-4 ) Tn=1.0E-4;

		if ( Tn >= 0 && Tn < 0.1 )
			Beta = 1.0 + ( ( 2.25 * SRa - 1.0 ) * ( Tn * 10 ) );
		else if ( Tn >= 0.1 && Tn < m_dTg )
			Beta = 2.25 * SRa;
		else if ( Tn >= m_dTg && Tn < 5 * m_dTg )
			Beta = 2.25 * ( m_dTg / Tn ) * SRv;
		else if ( Tn >= 5 * m_dTg )
			Beta = 0.45 * SRv;

		if ( Tn > 0.1 + dTol && Tn < 0.1 + dt - dTol ) // transition point		
		{
			aPeriod.Add(0.1);
			aAccel.Add(Alpha * 2.25 * SRa);
		}
		if ( Tn > m_dTg + dTol && Tn < m_dTg + dt - dTol ) // transition point		
		{
			aPeriod.Add(m_dTg);
			aAccel.Add(Alpha * 2.25 * SRa);
		}
		if ( Tn > 5 * m_dTg + dTol && Tn < 5 * m_dTg + dt - dTol ) // transition point		
		{
			aPeriod.Add(5 * m_dTg);
			aAccel.Add(Alpha * 0.45 * SRv);
		}

		aPeriod.Add(Tn);
		aAccel.Add(Alpha * Beta);
	}
}

void CSpfcGB50111_2006Util::SetParamFromCode(const T_SPFC_CODE& codeParam)
{
	m_nDivision = codeParam.GB50111_2006.nDivision;
	m_nSiteClass = codeParam.GB50111_2006.nSiteClass;
	m_nSFI = codeParam.GB50111_2006.nSeisFortification;
	m_nEarthEff = codeParam.GB50111_2006.nEarthResponse;
	m_dMaxPeriod = codeParam.GB50111_2006.dMaxPeriod;
	m_dTg = codeParam.GB50111_2006.dTg;
	m_dG = codeParam.GB50111_2006.dG;
	m_bImportantBridge = codeParam.GB50111_2006.bImportantBridge;
}
