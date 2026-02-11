#include "stdafx.h"
#include "SpfcTAIWAN98Util.h"
#include "../wg_db/DBDoc.h"
#include "../wg_db/AttrCtrl2.h"

CSpfcTAIWAN98Util::CSpfcTAIWAN98Util()
{
	m_nSpecCategory = 0;       // MNET:2410-GSJI-20060829

	m_nSpecType = 0;     //Horizontal
	m_nSeisZone = 0;     //General Zone  

	m_dSds = 0.5;         m_dSd1 = 0.3;         m_dSms = 0.7;         m_dSm1 = 0.4;
	m_nSoilType = 0;     //Type1   
	m_dFda = 1.0;         m_dFdv = 1.0;         m_dFma = 1.0;         m_dFmv = 1.0;
	m_dNda = 1.0;         m_dNdv = 1.0;         m_dNma = 1.0;         m_dNmv = 1.0;
	m_dFda_1 = 1.0;       m_dFdv_1 = 1.0;
	m_nSubZone = 0;      //Taipei Basin I
	m_dSds_t = 0.6;       m_dSms_t = 0.8;       m_dTd0_t = 1.6;      m_dTm0_t = 1.6;
	m_dR = 1.6;
	m_dAy = 1.2;
	m_dI = 1.0;
	m_dDamping = 5.0;
	m_dMaxPeriod = PRD_ED;

	m_nSubZone = 0;
}

bool CSpfcTAIWAN98Util::CheckValid()
{
	//GetFinalValue();

	CString strMsg;
	//DWORD nItem;
	//CDlgUtil::CobxGetItemDataByCurSel(m_cmbSpecCategory, nItem);
	//m_nSpecCategory = nItem;
	if (m_nSpecCategory < 0 || m_nSpecCategory>2)  ASSERT(0);

	//m_nSeisZone = m_cmbSeisZone.GetItemData(m_cmbSeisZone.GetCurSel());
	if (m_nSeisZone == 0 || m_nSeisZone == 1)
	{
		if ((m_nSpecCategory != 2 && m_dSds <= 0.) || (m_nSpecCategory == 2 && m_dSms <= 0.))
		{
			strMsg.Format(_LS(IDS_CMD_ERROR_NAME_POSITIVE), _T("Ss"));
			AfxMessageBox(strMsg);
			return false;
		}
		if ((m_nSpecCategory != 2 && m_dSd1 <= 0.) || (m_nSpecCategory == 2 && m_dSm1 <= 0.))
		{
			strMsg.Format(_LS(IDS_CMD_ERROR_NAME_POSITIVE), _T("S1"));
			AfxMessageBox(strMsg);
			return false;
		}
		if ((m_nSpecCategory != 2 && m_dFda <= 0.) || (m_nSpecCategory == 2 && m_dFma <= 0.))
		{
			strMsg.Format(_LS(IDS_CMD_ERROR_NAME_POSITIVE), _T("Fa"));
			AfxMessageBox(strMsg);
			return false;
		}
		if ((m_nSpecCategory != 2 && m_dFdv <= 0.) || (m_nSpecCategory == 2 && m_dFmv <= 0.))
		{
			strMsg.Format(_LS(IDS_CMD_ERROR_NAME_POSITIVE), _T("Fv"));
			AfxMessageBox(strMsg);
			return false;
		}

		if (m_nSeisZone == 1)
		{
			//_T("Near Source Factor should be greater than or equal to 1.")
			if ((m_nSpecCategory != 2 && m_dNda < 1. || m_dNdv < 1.)
				|| (m_nSpecCategory == 2 && m_dNma < 1. || m_dNmv < 1.))
			{
				AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error__TAIWAN02_NSF));
				return false;
			}
		}
	}
	else
	{
		if (m_nSpecCategory != 2 && m_dSds_t <= 0.)
		{
			strMsg.Format(_LS(IDS_CMD_ERROR_NAME_POSITIVE), _T("Sds"));
			AfxMessageBox(strMsg);
			return false;
		}
		if (m_nSpecCategory == 2 && m_dSms_t <= 0.)
		{
			strMsg.Format(_LS(IDS_CMD_ERROR_NAME_POSITIVE), _T("Sms"));
			AfxMessageBox(strMsg);
			return false;
		}
		if ((m_nSpecCategory != 2 && m_dTd0_t <= 0.) || (m_nSpecCategory == 2 && m_dTm0_t <= 0.))
		{
			strMsg.Format(_LS(IDS_CMD_ERROR_NAME_POSITIVE), _T("Trans.Period"));
			AfxMessageBox(strMsg);
			return false;
		}
	}
	if (m_dI <= 0.)
	{
		strMsg.Format(_LS(IDS_CMD_ERROR_NAME_POSITIVE), _T("I"));
		AfxMessageBox(strMsg);
		return false;
	}
	if (m_dAy <= 0.)
	{
		strMsg.Format(_LS(IDS_CMD_ERROR_NAME_POSITIVE), _T("ay"));
		AfxMessageBox(strMsg);
		return false;
	}
	if (m_dR <= 0.)
	{
		strMsg.Format(_LS(IDS_CMD_ERROR_NAME_POSITIVE), _T("R"));
		AfxMessageBox(strMsg);
		return false;
	}
	if (m_dDamping <= 0.)
	{
		strMsg.Format(_LS(IDS_CMD_ERROR_NAME_POSITIVE), _T("Damping Ratio"));
		AfxMessageBox(strMsg);
		return false;
	}
	if (m_dMaxPeriod <= 0.) { AfxMessageBox(_LS(IDS_CMD_SPECTRUM__MAX_PERIOD_GREATER_0)); return false; }

	return true;
}

void CSpfcTAIWAN98Util::MakeSpectrumData(bool bOnlyCalc)
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
	double dSa;

	dt = (ed - st) / Step;      // time interval	
	double dTol = dt / Step;  // Tolerance

	GetBFactorbyDamping(m_dDamping);                  // Calc dBs, dB1

	if (m_nSeisZone == 0)    Calc_GeneralZone(sCalc);
	else if (m_nSeisZone == 1)    Calc_NearFaultZone(sCalc);
	else                       Calc_TaipaiBasin(sCalc);

	double SRa = 1.0, SRv = 1.0;                      // Fixed for MakeSpectrumData() 
	sCalc.dTg_0 = sCalc.dTd0 * SRv / SRa;
	if (m_nSeisZone == 1)   sCalc.dTg_1 = sCalc.dTd0_1 * SRv / SRa;
	else                      sCalc.dTg_1 = sCalc.dTg_0;
	sCalc.dTg_2 = sCalc.dTm0 * SRv / SRa;

	for (int i = 0; i <= Step; i++)
	{
		Tn = st + dt * i;
		dSa = Calc_dSa_EveryZone(sCalc, Tn, SRa, SRv);

		// Transition point --------------------------------------------------------------------------------    
		double dTd0 = sCalc.dTd0, dTm0 = sCalc.dTm0;
		if (m_nSpecCategory != 2)
		{
			if (Tn > 0.2 * dTd0 + dTol && Tn < 0.2 * dTd0 + dt - dTol) TransPoint_EveryZone(sCalc, SRa, SRv, 0.2 * dTd0);
			if (Tn > 0.6 * dTd0 + dTol && Tn < 0.6 * dTd0 + dt - dTol) TransPoint_EveryZone(sCalc, SRa, SRv, 0.6 * dTd0); // curve가 잘 나오도록 추가함.
			if (Tn > 1.0 * dTd0 + dTol && Tn < 1.0 * dTd0 + dt - dTol) TransPoint_EveryZone(sCalc, SRa, SRv, 1.0 * dTd0);
			if (Tn > 2.5 * dTd0 + dTol && Tn < 2.5 * dTd0 + dt - dTol) TransPoint_EveryZone(sCalc, SRa, SRv, 2.5 * dTd0);
		}
		else
		{
			if (Tn > 0.2 * dTm0 + dTol && Tn < 0.2 * dTm0 + dt - dTol) TransPoint_EveryZone(sCalc, SRa, SRv, 0.2 * dTm0);
			if (Tn > 0.6 * dTm0 + dTol && Tn < 0.6 * dTm0 + dt - dTol) TransPoint_EveryZone(sCalc, SRa, SRv, 0.6 * dTm0);
			if (Tn > 1.0 * dTm0 + dTol && Tn < 1.0 * dTm0 + dt - dTol) TransPoint_EveryZone(sCalc, SRa, SRv, 1.0 * dTm0);
			if (Tn > 2.5 * dTm0 + dTol && Tn < 2.5 * dTm0 + dt - dTol) TransPoint_EveryZone(sCalc, SRa, SRv, 2.5 * dTm0);
		}
		//     if(dTm0==dTd0)
		//     {
		//       if(Tn> 0.2*dTd0 + dTol && Tn< 0.2*dTd0 + dt-dTol) TransPoint_EveryZone(sCalc, SRa, SRv, 0.2*dTd0);
		//       if(Tn> 1.0*dTd0 + dTol && Tn< 1.0*dTd0 + dt-dTol) TransPoint_EveryZone(sCalc, SRa, SRv, 1.0*dTd0);      
		//       if(Tn> 2.5*dTd0 + dTol && Tn< 2.5*dTd0 + dt-dTol) TransPoint_EveryZone(sCalc, SRa, SRv, 2.5*dTd0);      
		//     }
		//     else if(dTm0 > dTd0)
		//     {
		//       if(Tn> 0.2*dTd0 + dTol && Tn< 0.2*dTd0 + dt-dTol) TransPoint_EveryZone(sCalc, SRa, SRv, 0.2*dTd0);
		//       if(Tn> 0.2*dTm0 + dTol && Tn< 0.2*dTm0 + dt-dTol) TransPoint_EveryZone(sCalc, SRa, SRv, 0.2*dTm0);
		//       if(Tn> 1.0*dTd0 + dTol && Tn< 1.0*dTd0 + dt-dTol) TransPoint_EveryZone(sCalc, SRa, SRv, 1.0*dTd0); 
		//       if(Tn> 1.0*dTm0 + dTol && Tn< 1.0*dTm0 + dt-dTol) TransPoint_EveryZone(sCalc, SRa, SRv, 1.0*dTm0); 
		//       if(Tn> 2.5*dTd0 + dTol && Tn< 2.5*dTd0 + dt-dTol) TransPoint_EveryZone(sCalc, SRa, SRv, 2.5*dTd0);
		//       if(Tn> 2.5*dTm0 + dTol && Tn< 2.5*dTm0 + dt-dTol) TransPoint_EveryZone(sCalc, SRa, SRv, 2.5*dTm0);
		//     }
		//     else
		//     {
		//       if(Tn> 0.2*dTm0 + dTol && Tn< 0.2*dTm0 + dt-dTol) TransPoint_EveryZone(sCalc, SRa, SRv, 0.2*dTm0);
		//       if(Tn> 0.2*dTd0 + dTol && Tn< 0.2*dTd0 + dt-dTol) TransPoint_EveryZone(sCalc, SRa, SRv, 0.2*dTd0);
		//       if(Tn> 1.0*dTm0 + dTol && Tn< 1.0*dTm0 + dt-dTol) TransPoint_EveryZone(sCalc, SRa, SRv, 1.0*dTm0);
		//       if(Tn> 1.0*dTd0 + dTol && Tn< 1.0*dTd0 + dt-dTol) TransPoint_EveryZone(sCalc, SRa, SRv, 1.0*dTd0);
		//       if(Tn> 2.5*dTm0 + dTol && Tn< 2.5*dTm0 + dt-dTol) TransPoint_EveryZone(sCalc, SRa, SRv, 2.5*dTm0);
		//       if(Tn> 2.5*dTd0 + dTol && Tn< 2.5*dTd0 + dt-dTol) TransPoint_EveryZone(sCalc, SRa, SRv, 2.5*dTd0);
		//     }          
				// Transition point ------------------------------------------------------------------------------//

		
		AddPeriod(Tn);
		AddAccel(dSa);
	}

	if (bOnlyCalc) return;

	// Description   
	CString strSpecCategory[] = { _T("VD"),_T("VS"),_T("VM") };    // Description 제한때문에 약어 사용

	wchar_t buffer[SpfcUtil::BUFFER_SIZE];
	int result = 0;

	if (m_nSeisZone == 0 || m_nSeisZone == 1)
	{
		result = std::swprintf(buffer, SpfcUtil::BUFFER_SIZE, _LS(IDS_CMD__ADD_TAIWANBRG98_PROFILE1), GetSeismicZoneName(m_nSeisZone), strSpecCategory[m_nSpecCategory], GetSoilTypeName(m_nSoilType), m_dI, m_dAy, m_dR);
	}
	else
	{
		result = std::swprintf(buffer, SpfcUtil::BUFFER_SIZE, _LS(IDS_CMD__ADD_TAIWANBRG98_PROFILE2), GetSubZoneName(m_nSubZone), strSpecCategory[m_nSpecCategory], m_dI, m_dAy, m_dR);
	}

	if (result >= 0)
	{
		SetDescript(buffer);
	}

	SetFuncName(L"TaiwanBrg(98)");
}

void CSpfcTAIWAN98Util::MakePOSpectrumData(double dDamping, double SRa, double SRv, CArray<double, double>& aPeriod, CArray<double, double>& aAccel)
{
	// MNET:2689-GSJI-20070312 : 
	// Pushover Performance Curve를 작도할 때 Factor가 적용된 값을 사용하지 않는다. (대만대리점 요청)
	BOOL bPOcurve = true;

	aPeriod.RemoveAll();
	aAccel.RemoveAll();

	double st = PRD_ST;
	double ed = m_dMaxPeriod * 2;
	int Step = PRD_NUM * 4;
	double Tn, dt;
	double dSa;               // Normalized 5% damping spectrum

	dt = (ed - st) / Step;      // time interval	
	double dTol = dt / Step;  // Tolerance


	// taiwan1999 외 기타 코드와 동일한 방식인데, taiwan2006에는 부적합하다고 판단되어
	// 1로 값을 고정시킴.
	/*
	if(dDamping >= 0)
	{
		SRa = (3.21-0.68*log(dDamping))/2.12;
		SRv = (2.31-0.41*log(dDamping))/1.65;
	}
	*/
	SRa = 1.0;   SRv = 1.0;

	GetBFactorbyDamping(dDamping);     // Generate Spectrum Dlg의 입력치 대신 Pushover의 Damping 으로 계산
	m_dDamping = dDamping;

	if (m_nSeisZone == 0)    Calc_GeneralZone(sCalc);
	else if (m_nSeisZone == 1)    Calc_NearFaultZone(sCalc);
	else                       Calc_TaipaiBasin(sCalc);

	sCalc.dTg_0 = sCalc.dTd0 * SRv / SRa;
	if (m_nSeisZone == 1)   sCalc.dTg_1 = sCalc.dTd0_1 * SRv / SRa;
	else                      sCalc.dTg_1 = sCalc.dTg_0;
	sCalc.dTg_2 = sCalc.dTm0 * SRv / SRa;

	// MessageWindow에 결과 출력하여 확인할 때 사용
	//GSaveHistoryFormatNF(_T(" %g, %6.6f, %6.6f, %3.4f, %3.4f, %3.4f"), dDamping, SRa, SRv, sCalc.dTg_0, sCalc.dTg_1, sCalc.dTg_2);

	// MNET:2689-GSJI-20070312 : for Test
	//GSaveHistoryFormatNF(_T("Damping : %g"), dDamping);
	//GSaveHistoryFormatNF(_T("Tn   dSa*I"));

	for (int i = 0; i <= Step; i++)
	{
		Tn = st + dt * i;

		// Transition Point --------------------------------------------------------------------
		// dTg_1 은 Near Fault Zone에서만 유효하므로 add하지 않음. ( dTg_1 = sCalc.dTg_1  )
		// 아래 4개 주기에 대한 응답값을 dTol에 따라 추가함.
		double dTd0 = sCalc.dTd0, dTm0 = sCalc.dTm0;
		double dTg_0 = sCalc.dTg_0, dTg_2 = sCalc.dTg_2;
		if (m_nSpecCategory != 2)
		{
			TransPoint_POSpectrum(sCalc, SRa, SRv, dt, dTol, dSa, 0.2 * dTd0, Tn, aPeriod, aAccel, bPOcurve);
			TransPoint_POSpectrum(sCalc, SRa, SRv, dt, dTol, dSa, 0.6 * dTd0, Tn, aPeriod, aAccel, bPOcurve);
			TransPoint_POSpectrum(sCalc, SRa, SRv, dt, dTol, dSa, dTg_0, Tn, aPeriod, aAccel, bPOcurve);
			TransPoint_POSpectrum(sCalc, SRa, SRv, dt, dTol, dSa, 2.5 * dTd0, Tn, aPeriod, aAccel, bPOcurve);
		}
		else
		{
			TransPoint_POSpectrum(sCalc, SRa, SRv, dt, dTol, dSa, 0.2 * dTm0, Tn, aPeriod, aAccel, bPOcurve);
			TransPoint_POSpectrum(sCalc, SRa, SRv, dt, dTol, dSa, 0.6 * dTm0, Tn, aPeriod, aAccel, bPOcurve);
			TransPoint_POSpectrum(sCalc, SRa, SRv, dt, dTol, dSa, dTg_2, Tn, aPeriod, aAccel, bPOcurve);
			TransPoint_POSpectrum(sCalc, SRa, SRv, dt, dTol, dSa, 2.5 * dTm0, Tn, aPeriod, aAccel, bPOcurve);
		}
		//     if(dTd0 == dTm0)
		//     {
		//       TransPoint_POSpectrum(sCalc, SRa, SRv, dt, dTol, dSa, 0.2*dTd0, Tn, aPeriod, aAccel);            
		//       TransPoint_POSpectrum(sCalc, SRa, SRv, dt, dTol, dSa,    dTg_0, Tn, aPeriod, aAccel);
		//       TransPoint_POSpectrum(sCalc, SRa, SRv, dt, dTol, dSa, 2.5*dTd0, Tn, aPeriod, aAccel);      
		//     }
		//     else if(dTd0 > dTm0)
		//     {
		//       TransPoint_POSpectrum(sCalc, SRa, SRv, dt, dTol, dSa, 0.2*dTm0, Tn, aPeriod, aAccel);
		//       TransPoint_POSpectrum(sCalc, SRa, SRv, dt, dTol, dSa, 0.2*dTd0, Tn, aPeriod, aAccel);          
		//       if(0.2*dTd0 < dTg_0)
		//       {
		//         TransPoint_POSpectrum(sCalc, SRa, SRv, dt, dTol, dSa,    dTg_2, Tn, aPeriod, aAccel);
		//         TransPoint_POSpectrum(sCalc, SRa, SRv, dt, dTol, dSa,    dTg_0, Tn, aPeriod, aAccel);        
		//       }      
		//       TransPoint_POSpectrum(sCalc, SRa, SRv, dt, dTol, dSa, 2.5*dTm0, Tn, aPeriod, aAccel);
		//       TransPoint_POSpectrum(sCalc, SRa, SRv, dt, dTol, dSa, 2.5*dTd0, Tn, aPeriod, aAccel);
		//     }
		//     else
		//     {
		//       TransPoint_POSpectrum(sCalc, SRa, SRv, dt, dTol, dSa, 0.2*dTd0, Tn, aPeriod, aAccel);    
		//       TransPoint_POSpectrum(sCalc, SRa, SRv, dt, dTol, dSa, 0.2*dTm0, Tn, aPeriod, aAccel);
		//       if(0.2*dTd0 < dTg_0)
		//       {
		//         TransPoint_POSpectrum(sCalc, SRa, SRv, dt, dTol, dSa,    dTg_0, Tn, aPeriod, aAccel);      
		//         TransPoint_POSpectrum(sCalc, SRa, SRv, dt, dTol, dSa,    dTg_2, Tn, aPeriod, aAccel);
		//       }      
		//       TransPoint_POSpectrum(sCalc, SRa, SRv, dt, dTol, dSa, 2.5*dTd0, Tn, aPeriod, aAccel);
		//       TransPoint_POSpectrum(sCalc, SRa, SRv, dt, dTol, dSa, 2.5*dTm0, Tn, aPeriod, aAccel);
		//     }

				// transition Point --------------------------------------------------------------------//
		dSa = Calc_dSa_EveryZone(sCalc, Tn, SRa, SRv, bPOcurve);
		aPeriod.Add(Tn);
		aAccel.Add(dSa);
	}
}

void CSpfcTAIWAN98Util::SetParamFromCode(const T_SPFC_CODE& codeParam)
{
	m_nSpecType = codeParam.TAIWANBRG98.nSpectrumType;
	m_nSeisZone = codeParam.TAIWANBRG98.nSeismicZone;
	m_nSpecCategory = codeParam.TAIWANBRG98.nSpectrumUsed;

	m_dSds = codeParam.TAIWANBRG98.dSds;
	m_dSd1 = codeParam.TAIWANBRG98.dSd1;
	m_dSms = codeParam.TAIWANBRG98.dSms;
	m_dSm1 = codeParam.TAIWANBRG98.dSm1;

	m_dNda = codeParam.TAIWANBRG98.dNda;
	m_dNdv = codeParam.TAIWANBRG98.dNdv;
	m_dNma = codeParam.TAIWANBRG98.dNma;
	m_dNmv = codeParam.TAIWANBRG98.dNmv;

	m_nSoilType = codeParam.TAIWANBRG98.nSoilType;
	m_dFda = codeParam.TAIWANBRG98.dFda;
	m_dFdv = codeParam.TAIWANBRG98.dFdv;
	m_dFma = codeParam.TAIWANBRG98.dFma;
	m_dFmv = codeParam.TAIWANBRG98.dFmv;

	m_nSubZone = codeParam.TAIWANBRG98.nSubZone;
	m_dSds_t = codeParam.TAIWANBRG98.dSds_t;
	m_dSms_t = codeParam.TAIWANBRG98.dSms_t;
	m_dTd0_t = codeParam.TAIWANBRG98.dTd0_t;
	m_dTm0_t = codeParam.TAIWANBRG98.dTm0_t;

	m_dI = codeParam.TAIWANBRG98.dImportanceFactor;
	m_dAy = codeParam.TAIWANBRG98.dSeisMagnifyFactor;
	m_dR = codeParam.TAIWANBRG98.dResponseModifyFactor;
	m_dDamping = codeParam.TAIWANBRG98.dDampingRatio;
	m_dMaxPeriod = codeParam.TAIWANBRG98.dMaxPeriod;

	SetParamFromCalc();
}

void CSpfcTAIWAN98Util::SetParamFromCalc()
{
	if (m_nSoilType != 3 && m_nSeisZone == 1)
	{
		double dNda_1 = 1.0;
		double dNdv_1 = 1.0;
		double dFma = m_dFma;
		double dFmv = m_dFmv;

		auto* pDoc = CDBDoc::GetDocPoint();
		pDoc->m_pAttrCtrl2->CalcTaiwan98SiteMagnifyFactor(m_nSeisZone, m_nSoilType, m_dSds, m_dSd1, m_dSms, m_dSm1, dNda_1, dNdv_1, m_dNma, m_dNmv, m_dFda_1, m_dFdv_1, dFma, dFmv);
	}
}

CString CSpfcTAIWAN98Util::GetSeismicZoneName(int nCode)
{
	if (nCode < 0 || nCode > 2) { ASSERT(0); return _T(""); }

	CString aItem[] = { _LS(IDS_CMD_SPFC_TAIWAN02__General_Zone),_LS(IDS_CMD_SPFC_TAIWAN02__Near_Fault_Zone),_LS(IDS_CMD_SPFC_TAIWAN02__Taipei_Basin) };
	return aItem[nCode];
}

CString CSpfcTAIWAN98Util::GetSoilTypeName(int nCode)
{
	if (nCode < 0 || nCode > 3) { ASSERT(0); return _T(""); }

	//CString aItem[] = {_T("Type 1"),_T("Type 2"),_T("Type 3"),_T("User Input")};
	CString aItem[] = { _LS(IDS_CMD_SPFC_TAIWAN02__Type1), _LS(IDS_CMD_SPFC_TAIWAN02__Type2), _LS(IDS_CMD_SPFC_TAIWAN02__Type3),_LS(IDS_CMD_SPFC_TAIWAN02__User) };
	return aItem[nCode];
}

CString CSpfcTAIWAN98Util::GetSubZoneName(int nCode)
{
	if (nCode < 0 || nCode > 3) { ASSERT(0); return _T(""); }

	//CString aItem[] = {_T("Taipei Basin I"),_T("Taipei Basin II"),_T("Taipei Basin III"),_T("Taipei Basin IV"),_T("User Input")};  
	CString aItem[] = { _LS(IDS_CMD_SPFC_TAIWAN02__TaipeiBasinI), _LS(IDS_CMD_SPFC_TAIWAN02__TaipeiBasinII),_LS(IDS_CMD_SPFC_TAIWAN02__TaipeiBasinIII), _LS(IDS_CMD_SPFC_TAIWAN02__User) };
	return aItem[nCode];
}

CString CSpfcTAIWAN98Util::GetSpecCategoryName(int nCode)   // MNET:2410-GSJI-20060829
{
	if (nCode < 0 || nCode > 3) { ASSERT(0); return _T(""); }

	CString aItem[] = { _LS(IDS_CMD_SPFC_TAIWAN02__Design_Spectrum), _LS(IDS_CMD_SPFC_TAIWAN02__Small_Medium_Spectrum), _LS(IDS_CMD_SPFC_TAIWAN02__Maximum_Spectrum) };
	return aItem[nCode];
}

bool CSpfcTAIWAN98Util::Calc_GeneralZone(stETC_GDS_TAIWAN02_CALC& sCalc)
{
	if (m_nSpecCategory != 2)
	{
		sCalc.dSDS = m_dSds * m_dFda;
		sCalc.dSD1 = m_dSd1 * m_dFdv;
		sCalc.dTd0 = sCalc.dSD1 / sCalc.dSDS * (sCalc.dBs / sCalc.dB1);
	}
	else
	{
		sCalc.dSMS = m_dSms * m_dFma;
		sCalc.dSM1 = m_dSm1 * m_dFmv;
		sCalc.dTm0 = sCalc.dSM1 / sCalc.dSMS * (sCalc.dBs / sCalc.dB1);
	}
	return TRUE;
}

bool CSpfcTAIWAN98Util::Calc_NearFaultZone(stETC_GDS_TAIWAN02_CALC& sCalc)
{
	if (m_nSpecCategory != 2)
	{
		sCalc.dSDS = m_dSds * m_dFda * m_dNda;       sCalc.dSD1 = m_dSd1 * m_dFdv * m_dNdv;
		sCalc.dSDS_1 = m_dSds * m_dFda_1 * 1.0;          sCalc.dSD1_1 = m_dSd1 * m_dFdv_1 * 1.0;      // Na=1, Nv=1  for V*
		sCalc.dTd0 = sCalc.dSD1 / sCalc.dSDS * (sCalc.dBs / sCalc.dB1);
		sCalc.dTd0_1 = sCalc.dSD1_1 / sCalc.dSDS_1 * (sCalc.dBs / sCalc.dB1);
	}
	else
	{
		sCalc.dSMS = m_dSms * m_dFma * m_dNma;       sCalc.dSM1 = m_dSm1 * m_dFmv * m_dNmv;
		sCalc.dTm0 = sCalc.dSM1 / sCalc.dSMS * (sCalc.dBs / sCalc.dB1);
	}
	return TRUE;
}

bool CSpfcTAIWAN98Util::Calc_TaipaiBasin(stETC_GDS_TAIWAN02_CALC& sCalc)
{
	if (m_nSpecCategory != 2)
	{
		sCalc.dSDS = m_dSds_t;
		sCalc.dTd0 = m_dTd0_t * (sCalc.dBs / sCalc.dB1);
	}
	else
	{
		sCalc.dSMS = m_dSms_t;
		sCalc.dTm0 = m_dTm0_t * (sCalc.dBs / sCalc.dB1);
	}
	return TRUE;
}

double CSpfcTAIWAN98Util::GetSaFactor(stETC_GDS_TAIWAN02_CALC& sCalc, const double dT, int nMIN)
{
	double dT0, dSS, dS1;
	if (nMIN == 0) { dT0 = sCalc.dTd0;         dSS = sCalc.dSDS;       dS1 = sCalc.dSD1; }
	else if (nMIN == 1) { dT0 = sCalc.dTd0_1;       dSS = sCalc.dSDS_1;     dS1 = sCalc.dSD1_1; }
	else { dT0 = sCalc.dTm0;         dSS = sCalc.dSMS;       dS1 = sCalc.dSM1; }

	double dSa;
	if (dT <= 0.2 * dT0)
		dSa = dSS * (0.4 + (1 / sCalc.dBs - 0.4) * dT / (0.2 * dT0));
	else if (0.2 * dT0 < dT && dT <= dT0)
		dSa = dSS / sCalc.dBs;
	else
	{
		if (m_nSeisZone != 2)  dSa = dS1 / (sCalc.dB1 * dT);
		else                dSa = dSS / (sCalc.dB1 * dT) * dT0;                // Taipei
	}

	return dSa;
}

bool CSpfcTAIWAN98Util::GetFuFactor(double& dT, const double dT0, int nMIN, double& dFu)
{
	double dRa;
	if (m_nSeisZone != 2)   dRa = 1 + (m_dR - 1) / 1.5;
	else                 dRa = 1 + (m_dR - 1) / 2.0;

	if (m_nSpecType == 0 && nMIN == 2) dRa = m_dR;                    // for FuM (V_M) : Calc. Fu by R (Not Ra), 수평지진력에만 해당함.

	if (2 * dRa - 1 < 0.0)     return FALSE;
	double dRt = sqrt(2 * dRa - 1);

	if (dT >= dT0)                      dFu = dRa;
	else if (dT >= 0.6 * dT0 && dT < dT0)      dFu = dRt + (dRa - dRt) * (dT - 0.6 * dT0) / (0.4 * dT0);
	else if (dT >= 0.2 * dT0 && dT < 0.6 * dT0)  dFu = dRt;
	else                                  dFu = dRt + (dRt - 1) * (dT - 0.2 * dT0) / (0.2 * dT0);

	return TRUE;
}

double CSpfcTAIWAN98Util::GetModifiedSaFu(double dSa, double dFu)     //  Calc. (SaD/Fu)m
{
	double dMdf = dSa / dFu;
	if (dMdf <= 0.3)            return dMdf;
	else if (dMdf > 0.3 && dMdf < 0.8) return (0.2 * dMdf + 0.24);
	else                            return (0.5 * dMdf);
}

double CSpfcTAIWAN98Util::GetModifiedSaFuAv(double dSa, double dFu, double dAv)     //  Calc. (av * SaD/Fu)m
{
	double dMdf = dAv * dSa / dFu;
	if (dMdf <= 0.3)            return dMdf;
	else if (dMdf > 0.3 && dMdf < 0.8) return (0.2 * dMdf + 0.24);
	else                            return (0.5 * dMdf);
}

double CSpfcTAIWAN98Util::GetModified0_4SDSFu(stETC_GDS_TAIWAN02_CALC& sCalc, double dFu, int nMIN)     //  Calc. (0.4*SDS/Fu)m
{
	double dSS;
	if (nMIN == 0)  dSS = sCalc.dSDS;
	else if (nMIN == 1)  dSS = sCalc.dSDS_1;
	else              dSS = sCalc.dSMS;

	double dMdf = 0.4 * dSS / dFu;
	if (dMdf <= 0.3)            return dMdf;
	else if (dMdf > 0.3 && dMdf < 0.8) return (0.2 * dMdf + 0.24);
	else                            return (0.5 * dMdf);
}


bool CSpfcTAIWAN98Util::GetBFactorbyDamping(const double dDamp)
{
	double dBs, dB1;
	if (dDamp <= 2)
	{
		dBs = 0.8;                                        dB1 = 0.8;
	}
	else if (dDamp <= 5 && dDamp > 2)
	{
		dBs = LinearInt(2.0, 5.0, 0.8, 1.0, dDamp);       dB1 = LinearInt(2.0, 5.0, 0.8, 1.0, dDamp);
	}
	else if (dDamp <= 10 && dDamp > 5)
	{
		dBs = LinearInt(5.0, 10.0, 1.00, 1.33, dDamp);    dB1 = LinearInt(5.0, 10.0, 1.00, 1.25, dDamp);
	}
	else if (dDamp <= 20 && dDamp > 10)
	{
		dBs = LinearInt(10.0, 20.0, 1.33, 1.60, dDamp);   dB1 = LinearInt(10.0, 20.0, 1.25, 1.50, dDamp);
	}
	else if (dDamp <= 30 && dDamp > 20)
	{
		dBs = LinearInt(20.0, 30.0, 1.60, 1.79, dDamp);   dB1 = LinearInt(20.0, 30.0, 1.50, 1.63, dDamp);
	}
	else if (dDamp <= 40 && dDamp > 30)
	{
		dBs = LinearInt(30.0, 40.0, 1.79, 1.87, dDamp);   dB1 = LinearInt(30.0, 40.0, 1.63, 1.70, dDamp);
	}
	else if (dDamp <= 50 && dDamp > 40)
	{
		dBs = LinearInt(40.0, 50.0, 1.87, 1.93, dDamp);   dB1 = LinearInt(40.0, 50.0, 1.70, 1.75, dDamp);
	}
	else
	{
		dBs = 1.93;                                       dB1 = 1.75;
	}
	sCalc.dBs = dBs;        sCalc.dB1 = dB1;
	return TRUE;
}
double CSpfcTAIWAN98Util::LinearInt(double dx1, double dx2, double dy1, double dy2, double dxx)
{
	//Linear Interpolation for dBs, dB1
	double dyy = 0.0;
	dyy = (dy2 - dy1) / (dx2 - dx1) * (dxx - dx1) + dy1;
	if (dyy == 0) return FALSE;
	return dyy;
}


double CSpfcTAIWAN98Util::Calc_dSa_EveryZone(stETC_GDS_TAIWAN02_CALC& sCalc, double Tn, double SRa, double SRv, BOOL bPOcurve)
{
	int nMIN = 0;                                                        // Index for {V, V*, Vm}
	double dSaD = 0.0, dSaD_1 = 0.0, dSaM = 0.0;
	dSaD = GetSaFactor(sCalc, Tn, nMIN = 0);
	dSaM = GetSaFactor(sCalc, Tn, nMIN = 2);

	if (m_nSeisZone == 1)
		dSaD_1 = GetSaFactor(sCalc, Tn, nMIN = 1);

	double dSFD = 0.0, dSFD_1 = 0.0, dSFM = 0.0;
	double d0_4SFD = 0.0, d0_4SFD_1 = 0.0, d0_4SFM = 0.0;
	double dV0 = 0.0, dV1 = 0.0, dV2 = 0.0;
	double dV0_1 = 0.0, dV0_2 = 0.0;
	double dV1_1 = 0.0, dV1_2 = 0.0, dV1_min1 = 0.0, dV1_min2 = 0.0;
	double dV2_1 = 0.0, dV2_2 = 0.0, dV2_min1 = 0.0, dV2_min2 = 0.0;

	double dFu = 0.0, dFuM = 0.0;
	if (!bPOcurve)
	{
		//double dFu, dFuM;
		GetFuFactor(Tn, sCalc.dTd0, nMIN = 0, dFu);
		GetFuFactor(Tn, sCalc.dTm0, nMIN = 2, dFuM);

		dSFD = GetModifiedSaFu(dSaD, dFu);                                // dSaD/dFu  
		dSFM = GetModifiedSaFu(dSaM, dFuM);                               // dSaM/dFuM
		if (m_nSeisZone == 1)
			dSFD_1 = GetModifiedSaFu(dSaD_1, dFu);                          // dSaD_1/dFu

		d0_4SFD = GetModified0_4SDSFu(sCalc, dFu, nMIN = 0);                // 0.4*dSDS/dFu
		d0_4SFM = GetModified0_4SDSFu(sCalc, dFuM, nMIN = 2);               // 0.4*dSMS/dFuM
		if (m_nSeisZone == 1)
			d0_4SFD_1 = GetModified0_4SDSFu(sCalc, dFu, nMIN = 1);            // 0.4*dSMS_1/dFu

		// V : nMIN=0
		dV0_1 = m_dI / (1.2 * m_dAy) * dSFD;
		dV0_2 = m_dI / (1.2 * m_dAy) * d0_4SFD;
		dV0 = max(dV0_1, dV0_2);

		// V* : nMIN=1
		if (m_nSeisZone == 1)
		{
			dV1_1 = m_dI / (1.2 * m_dAy) * dSFD_1;
			dV1_2 = m_dI / (1.2 * m_dAy) * d0_4SFD_1;
			dV1_min1 = m_dI * dSaD_1 / (3.25 * m_dAy);
			dV1_min2 = 0.4 * m_dI * sCalc.dSDS_1 / (3.25 * m_dAy);
		}
		else
		{
			dV1_1 = m_dI / (1.2 * m_dAy) * dSFD;
			dV1_2 = m_dI / (1.2 * m_dAy) * d0_4SFD;
			dV1_min1 = m_dI * dSaD / (3.25 * m_dAy);
			dV1_min2 = 0.4 * m_dI * sCalc.dSDS / (3.25 * m_dAy);
		}
		dV1 = max(dV1_min1, dV1_min2);
		//     if(dV1_1 < dV1_2) dV1 = dV1_min2;
		//     else              dV1 = dV1_min1;

				// V_M : nMIN=2
		dV2_1 = m_dI / (1.2 * m_dAy) * dSFM;
		dV2_2 = m_dI / (1.2 * m_dAy) * d0_4SFM;
		//dV2 = max(dV2_1, dV2_2);
		dV2_min1 = m_dI / (1.2 * m_dAy) * d0_4SFM;
		dV2_min2 = m_dI / (1.2 * m_dAy) * dSFM;
		dV2 = max(dV2_min1, dV2_min2);

	}
	// MNET:2689-GSJI-20070312
	else  // Pushover Curve 작도시에는 Factor없이 SaD * I 만 사용 
				// m_dAy, m_dR  -> 입력받을 수 없음. 아예 무시        
	{
		dSFD = dSaD;
		dSFM = dSaM;
		if (m_nSeisZone == 1)
			dSFD_1 = dSaD_1;
		// Apply SRa, SRv   1 고정이므로 막음.
		/*
		if(Tn <= sCalc.dTg_0)     dV0 = dV0 * SRa;       else          dV0 = dV0 * SRv;
		if(Tn <= sCalc.dTg_1)     dV1 = dV1 * SRa;       else          dV1 = dV1 * SRv;
		if(Tn <= sCalc.dTg_2)     dV2 = dV2 * SRa;       else          dV2 = dV2 * SRv;
		*/
		dV0 = m_dI * dSFD;                                  //V   : nMIN=0
		if (m_nSeisZone == 0)     dV1 = m_dI * dSFD;    //V*  : nMIN=1
		else if (m_nSeisZone == 1)     dV1 = m_dI * dSFD_1;
		else                        dV1 = m_dI * dSFD;
		dV2 = m_dI * dSFM;                                  //V_M : nMIN=2

	}

	//double dSa = max(max(dV0, dV1), dV2);   // MNET:2410-GSJI-20060829
	//개발초기에는 dV0, dV1, dV2를 모두 계산해야 했는데,
	//대리점에서 잘못 알려준 것이어서 추후 옵션을 받아서 3개중 한개 골라서 씀.
	//기존에 검증을 끝낸 상태여서 아래와 같이만 처리함.
	double dSa = 0.0;
	if (m_nSpecCategory == 0)  dSa = dV0;
	else if (m_nSpecCategory == 1)  dSa = dV1;
	else if (m_nSpecCategory == 2)  dSa = dV2;
	else  ASSERT(0);

	// Factor for Vertical Spectrum
	double dVdv = 0.0, dAv = 0.0;
	if (m_nSeisZone == 1)  dAv = 2.0 / 3.0;
	else                dAv = 1.0 / 2.0;

	double dSFDAv = 0.0, dSFDAv_1 = 0.0, dSFMAv = 0.0;
	dSFDAv = GetModifiedSaFuAv(dSaD, dFu, dAv);                                // dSaD/dFu  
	dSFMAv = GetModifiedSaFuAv(dSaM, dFuM, dAv);                               // dSaM/dFuM
	if (m_nSeisZone == 1)
		dSFDAv_1 = GetModifiedSaFuAv(dSaD_1, dFu, dAv);                          // dSaD_1/dFu

	if (m_nSpecType == 1)  // 상부구조
	{
		if (m_nSeisZone == 1)
		{
			if (m_nSpecCategory == 0)  dVdv = m_dI * dSFDAv / (1.2 * m_dAy);
			else if (m_nSpecCategory == 1)  dVdv = m_dI * dSFDAv_1 / (1.2 * m_dAy);
			else                          dVdv = m_dI * dSFMAv / (1.2 * m_dAy);
		}
		else
		{
			if (m_nSpecCategory == 0 || m_nSpecCategory == 1)  dVdv = m_dI * dSFDAv / (1.2 * m_dAy);
			else                                          dVdv = m_dI * dSFMAv / (1.2 * m_dAy);
		}

		dSa = dVdv;
	}
	else if (m_nSpecType == 2) // 하부구조
	{
		if (m_nSeisZone == 1)
		{
			if (m_nSpecCategory == 0)  dVdv = 0.4 * m_dI * dAv * sCalc.dSDS / m_dAy;
			else if (m_nSpecCategory == 1)  dVdv = 0.4 * m_dI * dAv * sCalc.dSDS_1 / m_dAy;
			else                          dVdv = 0.4 * m_dI * dAv * sCalc.dSMS / m_dAy;
		}
		else
		{
			if (m_nSpecCategory == 0 || m_nSpecCategory == 1)  dVdv = 0.4 * m_dI * dAv * sCalc.dSDS / m_dAy;
			else                                          dVdv = 0.4 * m_dI * dAv * sCalc.dSMS / m_dAy;
		}

		dSa = dVdv;
	}

	// 중간값 테스트
	double dSaDTest = 0;
	if (m_nSpecCategory == 0) dSaDTest = dSaD;
	else if (m_nSpecCategory == 1)
	{
		if (m_nSeisZone == 1) dSaDTest = dSaD_1;
		else                dSaDTest = dSaD;
	}
	else                         dSaDTest = dSaM;

	//GSaveHistoryFormatNF(_T("T=%g, Sad=%g"), Tn, dSaDTest);

	double dFuTest = 0;
	if (m_nSpecCategory == 0) dFuTest = dFu;
	else if (m_nSpecCategory == 1) dFuTest = dFu;
	else                         dFuTest = dFuM;

	//GSaveHistoryFormatNF(_T("T=%g, Fu=%g"), Tn, dFuTest);

	double dSFDTest = 0;
	if (m_nSpecCategory == 0) dSFDTest = dSFD;
	else if (m_nSpecCategory == 1 && m_nSeisZone == 1) dSFDTest = dSFD_1;
	else                         dSFDTest = dSFM;

	//GSaveHistoryFormatNF(_T("T=%g, (SaD/FuD)m=%g"), Tn, dSFDTest);

	double d0_4SFDTest = 0;
	if (m_nSpecCategory == 0) d0_4SFDTest = d0_4SFD;
	else if (m_nSpecCategory == 1 && m_nSeisZone == 1) d0_4SFDTest = d0_4SFD_1;
	else                         d0_4SFDTest = d0_4SFM;

	//GSaveHistoryFormatNF(_T("T=%g, (0.4*SDS/FuD)m=%g"), Tn, d0_4SFDTest);

	double dSFDAvTest = 0;
	if (m_nSpecCategory == 0) dSFDAvTest = dSFDAv;
	else if (m_nSpecCategory == 1)
	{
		if (m_nSeisZone == 1) dSFDAvTest = dSFDAv_1;
		else                dSFDAvTest = dSFDAv;
	}
	else                         dSFDAvTest = dSFMAv;
	//GSaveHistoryFormatNF(_T("T=%g, dSFDAv=%g"), Tn, dSFDAvTest);

	//GSaveHistoryFormatNF(_T("T=%g, V=%g"), Tn, dSa);

	return dSa;
}

void CSpfcTAIWAN98Util::TransPoint_EveryZone(stETC_GDS_TAIWAN02_CALC sCalc, double SRa, double SRv, double dTn)
{
	double dSa;
	dSa = Calc_dSa_EveryZone(sCalc, dTn, SRa, SRv);

	InitPeriod();
	InitAccel();

	AddPeriod(dTn);
	AddAccel(dSa);
}

void CSpfcTAIWAN98Util::TransPoint_POSpectrum(stETC_GDS_TAIWAN02_CALC sCalc, double SRa,
	double SRv, double& dt, double& dTol, double& dSa, double dT_crt, double& Tn, CArray <double, double>& aPeriod, CArray <double, double>& aAccel, BOOL bPOcurve)
{
	// Tn이 Critical 을 넘었을 때 Critical 주기의 데이터를 추가한다.
	// Critical : 0.2Td0, 0.2Tm0, dTg_0, dTg_2, 2.5Td0, 2.5Tm0  (6개,  dTg_1는 제외)
	if (Tn > dT_crt + dTol && Tn < dT_crt + dt - dTol)
	{
		dSa = Calc_dSa_EveryZone(sCalc, dT_crt, SRa, SRv, bPOcurve);
		aPeriod.Add(dT_crt);           aAccel.Add(dSa);
	}
}