#include "stdafx.h"
#include "wg_cmd.h"
#include "LoadCombCtrl.h"

#include "..\\wg_db\\wg_db_AttrCtrl.h"
#include "..\\wg_db\\wg_db_PostCtrl.h"
#include "..\wg_db\mvLoad.h"
#include "..\wg_db\VehlDef.h"

#if !defined D_STR
#define D_STR 0
#endif

#if !defined D_SER
#define D_SER 1
#endif

using namespace std;
//////////////////////////////////////////////////////////////////////////
#pragma region lcom_jtgd6004
// JTG D60-04 Áß±¹ Åä¸ñ½Ã¹æ¼­
BOOL CLoadCombCtrl::Set_CvlJTJD60_Permanent(ArINT& arDLoad, ArINT& arPsLoad, ArINT& arEvLoad, ArINT& arEhLoad,
	ArINT& arShLoad, ArINT& arCrLoad, ArINT& arBLoad, ArINT& arStlLoad, ArUNT& arSmLoad,
	int nType, CArray<T_LCOM_D, T_LCOM_D&>& arData)
{
	// nType  0 or 4: °è¼öÁ¶ÇÕ, 1 or 2: 1.0(settlement Á¦¿Ü), 3: 1.0(¸ðµÎ)
	arData.RemoveAll();

	if (m_nConstLoad == 0 && m_nDCount == 0 && m_nPsCount == 0 && m_nEvCount == 0 && m_nEhCount == 0 &&
		m_nShCount == 0 && m_nCrCount == 0 && m_nBCount == 0 && m_nStlCount == 0 && m_nSmCount == 0) return FALSE;

	int LoadCaseNo = 0;
	int nSmCount = max(m_nSmCount, 1);

	double dFact_1p2 = (nType == 0 || nType == 4) ? 1.2 : 1.;
	double dFact_1p4 = (nType == 0 || nType == 4) ? 1.4 : 1.;
	double dFact_0p5 = (nType == 3) ? 1.0 : 0.5;

	CString str1p2 = (nType == 0 || nType == 4) ? _T("1.2") : _T("1.0");
	CString str1p4 = (nType == 0 || nType == 4) ? _T("1.4") : _T("1.0");
	CString str0p5 = (nType == 3) ? _T("1.0") : _T("0.5");

	for (int sm = 0; sm < nSmCount; sm++)
	{
		T_LCOM_D LcomD; LcomD.Initialize();
		LoadCaseNo = 0;
		CString strPos = _T("");
		int i = 0, nDL = 0, nPS = 0, nEV = 0, nEH = 0, nSH = 0, nCR = 0, nBL = 0, nSTL = 0;
		for (i = 0; i < m_nDCount; i++) nDL += Set_STLCIncludeCS(arDLoad.GetAt(i), dFact_1p2, LoadCaseNo, LcomD, D_SGLD_DL_CVL);
		if (nDL > 0) { LcomD.Description += strPos + str1p2 + _LSX(D); strPos = _LSX(+); }
		for (i = 0; i < m_nPsCount; i++) nPS += Set_STLCIncludeCS(arPsLoad.GetAt(i), dFact_1p2, LoadCaseNo, LcomD, D_SGLD_TS_CVL);
		if (nPS > 0) { LcomD.Description += strPos + str1p2 + _LSX(PS); strPos = _LSX(+); }
		for (i = 0; i < m_nEvCount; i++) nEV += Set_STLCIncludeCS(arEvLoad.GetAt(i), dFact_1p2, LoadCaseNo, LcomD, 0);
		if (nEV > 0) { LcomD.Description += strPos + str1p2 + _LSX(EV); strPos = _LSX(+); }
		for (i = 0; i < m_nEhCount; i++) nEH += Set_STLCIncludeCS(arEhLoad.GetAt(i), dFact_1p4, LoadCaseNo, LcomD, 0);
		if (nEH > 0) { LcomD.Description += strPos + str1p4 + _LSX(EH); strPos = _LSX(+); }
		for (i = 0; i < m_nCrCount; i++) nCR += Set_STLCIncludeCS(arCrLoad.GetAt(i), 1., LoadCaseNo, LcomD, D_SGLD_CS_CVL);
		if (nCR > 0) { LcomD.Description += strPos + _T("1.0") + _LSX(CR); strPos = _LSX(+); }
		for (i = 0; i < m_nShCount; i++) nSH += Set_STLCIncludeCS(arShLoad.GetAt(i), 1., LoadCaseNo, LcomD, D_SGLD_SS_CVL);
		if (nSH > 0) { LcomD.Description += strPos + _T("1.0") + _LSX(SH); strPos = _LSX(+); }
		for (i = 0; i < m_nBCount; i++)  nBL += Set_STLCIncludeCS(arBLoad.GetAt(i), 1., LoadCaseNo, LcomD, 0);
		if (nBL > 0) { LcomD.Description += strPos + _T("1.0") + _LSX(B); strPos = _LSX(+); }
		for (i = 0; i < m_nStlCount; i++) nSTL += Set_STLCIncludeCS(arStlLoad.GetAt(i), dFact_0p5, LoadCaseNo, LcomD, 0);
		if (nSTL > 0) { LcomD.Description += strPos + str0p5 + _LSX(STL); strPos = _LSX(+); }
		if (m_nSmCount > 0)
		{
			Set_SettleCase(arSmLoad.GetAt(sm), dFact_0p5, LoadCaseNo, LcomD, TRUE); // SM
			CString strSM; strSM.Format(_T("%sSM[%d]"), str0p5, sm + 1);
			LcomD.Description += strPos + strSM; strPos = _LSX(+);
		}
		if (m_nConstLoad != 0) // CS or ST+CS
		{
			int n = 0;
			if (CheckCS_SgldK(D_SGLD_DL_CVL, n))
			{
				Set_CStageCase(m_aCSLoad.GetAt(n), dFact_1p2, LoadCaseNo, LcomD, TRUE);
				LcomD.Description += strPos + str1p2 + _T("(cD)"); strPos = _LSX(+);
			}

			CString strcEL;
			int nErection = 0;

			for (int nErec = 0; nErec < D_STCT_EREC_MAX_CVL; ++nErec)
			{
				strcEL = _T(""); nErection = 0;
				if (CheckCS_SgldK(m_CSEL_SgldK[nErec], n)) { strcEL = m_CSEL_Sgld_Desc[nErec]; nErection = m_CSEL_SgldK[nErec]; }



				if (CheckCSEL(nErection, _LSX(D)))
				{
					Set_CStageCase(m_aCSLoad.GetAt(n), dFact_1p2, LoadCaseNo, LcomD, TRUE);
					LcomD.Description += strPos + str1p2 + strcEL; strPos = _LSX(+);
				}
			}
			if (CheckCS_SgldK(D_SGLD_TP_CVL, n) && nType == 3) // tendon primary
			{
				double dPST = m_bLossFactor ? m_dLossFactorTransfer * dFact_1p2 : dFact_1p2;
				Set_CStageCase(m_aCSLoad.GetAt(n), dPST, LoadCaseNo, LcomD, TRUE);
				LcomD.Description += strPos + GetFactorString(dPST) + _T("(cTP)"); strPos = _LSX(+);
			}
			if (CheckCS_SgldK(D_SGLD_TS_CVL, n)) // tendon secondary
			{
				double dPSS = m_bLossFactor ? m_dLossFactorService * dFact_1p2 : dFact_1p2;
				Set_CStageCase(m_aCSLoad.GetAt(n), dPSS, LoadCaseNo, LcomD, TRUE);
				LcomD.Description += strPos + GetFactorString(dPSS) + _T("(cTS)"); strPos = _LSX(+);
			}
			if (CheckCS_SgldK(D_SGLD_CS_CVL, n)) // Creep secondary
			{
				Set_CStageCase(m_aCSLoad.GetAt(n), 1.0, LoadCaseNo, LcomD, TRUE);
				LcomD.Description += strPos + _T("1.0(cCR)"); strPos = _LSX(+);
			}
			if (CheckCS_SgldK(D_SGLD_SS_CVL, n)) // Shrinkage secondary
			{
				Set_CStageCase(m_aCSLoad.GetAt(n), 1.0, LoadCaseNo, LcomD, TRUE);
				LcomD.Description += strPos + _T("1.0(cSH)"); strPos = _LSX(+);
			}
		}
		arData.Add(LcomD);
	}

	if (arData.GetSize() < 1) return FALSE;

	arData.FreeExtra();

	return TRUE;
}

BOOL CLoadCombCtrl::SetCvlJTJD60_Capacity(const CArray<T_LCOM_D, T_LCOM_D&>& arData, ArINT& arLLoad,
	ArINT& arIlLoad, ArINT& arCfLoad, ArINT& arLsLoad, ArINT& arCrlLoad, ArINT& arWLoad,
	ArINT& arSfLoad, ArINT& arIpLoad, ArINT& arTLoad, ArINT& arTpgLoad, ArINT& arFrLoad,
	ArINT& arBrkLoad, int nType)
{
	// nType   0 or 4: °è¼öÁ¶ÇÕ, 1: 1.0(settlement Á¦¿Ü), 2: 1.0(¸ðµÎ), Live Load Á¦¿Ü
	CArray<T_LCOM_D, T_LCOM_D&> aLcomD; aLcomD.Copy(arData);
	int  nLcomSize = aLcomD.GetSize();
	int  nnn = 0;

	BOOL bErec = FALSE;
	for (int nErec = 0; nErec < D_STCT_EREC_MAX_CVL; ++nErec)
	{
		if (m_nConstLoad != 0 && CheckCS_SgldK(D_SGLD_ER1_CVL + nErec, nnn)) bErec = TRUE;
	}

	BOOL bAddCsLoad = bErec;
	//  BOOL bAddCsLoad = (m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER1_CVL,nnn)) || 
	// 		              (m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER2_CVL,nnn)) || 
	// 					  (m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER3_CVL,nnn)) ||
	// 					  (m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER4_CVL,nnn)) ||
	// 					  (m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER5_CVL,nnn)) ||
	// 					  (m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER6_CVL,nnn)) ||
	// 					  (m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER7_CVL,nnn)) ||
	// 					  (m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER8_CVL,nnn)) ||
	// 					  (m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER9_CVL,nnn)) ||
	// 					  (m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER10_CVL,nnn));// CS or ST+CS

		// ¿µ±¸ÇÏÁß¸¸ Á¶ÇÕ
	for (int lc = 0; lc < nLcomSize; lc++)
	{
		T_LCOM_D LcomD = aLcomD.GetAt(lc);
		LcomD.nActive = 1; // Active or Strength/Stress
		if (nType == 2 || nType == 4) m_aAcdnLcomD.Add(LcomD);
		else                     Set_LoadComb4Civil(LcomD, D_STR, 0, 101); // 101:±âº»Á¶ÇÕ(¿µ±¸ÇÏÁß)
	}
	if ((nType == 2 || nType == 4) && (m_nLsCount == 0 && m_nWCount == 0 && m_nSfCount == 0 &&
		m_nIpCount == 0 && m_nTCount == 0 && m_nTpgCount == 0 && m_nFrCount == 0))
	{
		for (int lc = 0; lc < nLcomSize; lc++) m_aAcdnLcomD.Add(aLcomD.GetAt(lc));
		return TRUE;
	}

	double dFactor = (nType == 0 || nType == 4) ? 1.4 : 1.0;
	double dWFactor = (nType == 0 || nType == 4) ? 1.1 : 1.0;

	int nMsgType = (nType == 0 || nType == 1) ? 103 : 104; // 103 : ±âº»Á¶ÇÕ, 104 : ¿ì¿¬Á¶ÇÕ
	// ¿µ±¸ÇÏÁß + 1°³ÀÇ °¡º¯ÇÏÁßÀÎ °æ¿ì
	CString strDesc = _T("");
	int LoadCaseNo = 0, nc = 0;
	for (int lc = 0; lc < nLcomSize; lc++)
	{
		T_LCOM_D LcomD = aLcomD.GetAt(lc); LcomD.nActive = 1; // Active or Strength/Stress
		LoadCaseNo = GetCountLcomCase(LcomD);
		if (nType != 2) // live load
		{
			BOOL bAddLcomD = FALSE;
			int nLL = 0, nIL = 0, nCF = 0;
			if (m_nLCount > 0 || (bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(L)))) // ST or ST+CS
			{
				for (nc = 0; nc < m_nLCount; nc++) nLL += Set_STLCIncludeCS(arLLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD, 0);
				if (nLL > 0) { strDesc.Format(_T("+%sL"), GetFactorString(dFactor)); LcomD.Description += strDesc; }
				if (bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(L)))
				{
					Set_CStageCase(m_aCSLoad.GetAt(nnn), dFactor, LoadCaseNo, LcomD, TRUE);
					strDesc.Format(_T("+%s(cEL)"), GetFactorString(dFactor)); LcomD.Description += strDesc;
					bAddLcomD = TRUE;
				}
				for (nc = 0; nc < m_nIlCount; nc++) nIL += Set_STLCIncludeCS(arIlLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD, 0);
				if (nIL > 0) { strDesc.Format(_T("+%sIL"), GetFactorString(dFactor)); LcomD.Description += strDesc; }
				for (nc = 0; nc < m_nCfCount; nc++) nCF += Set_STLCIncludeCS(arCfLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD, 0);
				if (nCF > 0) { strDesc.Format(_T("+%sCF"), GetFactorString(dFactor)); LcomD.Description += strDesc; }
				if (nLL + nIL + nCF > 0) bAddLcomD = TRUE;
			}
			if (bAddLcomD)
			{
				if (nType == 4) m_aAcdnLcomD.Add(LcomD);
				else         Set_LoadComb4Civil(LcomD, D_STR, 0, nMsgType);
			}
		}
		if (m_nLsCount > 0) // Live Load Surcharge
		{
			LcomD = aLcomD.GetAt(lc); LcomD.nActive = 1; LoadCaseNo = GetCountLcomCase(LcomD);
			int nLS = 0;
			for (nc = 0; nc < m_nLsCount; nc++) nLS += Set_STLCIncludeCS(arLsLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD, 0);
			if (nLS > 0)
			{
				strDesc.Format(_T("+%sLS"), GetFactorString(dFactor)); LcomD.Description += strDesc;
				if (nType == 2 || nType == 4) m_aAcdnLcomD.Add(LcomD);
				else                     Set_LoadComb4Civil(LcomD, D_STR, 0, nMsgType);
			}
		}
		if (nType != 2 && m_nCrlCount > 0) // Crowd Load
		{
			LcomD = aLcomD.GetAt(lc); LcomD.nActive = 1; LoadCaseNo = GetCountLcomCase(LcomD);
			int nCRL = 0;
			for (nc = 0; nc < m_nCrlCount; nc++) nCRL += Set_STLCIncludeCS(arCrlLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD, 0);
			if (nCRL > 0)
			{
				strDesc.Format(_T("+%sCRL"), GetFactorString(dFactor)); LcomD.Description += strDesc;
				if (nType == 2 || nType == 4) m_aAcdnLcomD.Add(LcomD);
				else                     Set_LoadComb4Civil(LcomD, D_STR, 0, nMsgType);
			}
		}

		int nWCount = m_nWCount * 2; // MNET:1614, KJH20050808
		if (m_nWCount == 0 && bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(W))) nWCount = 1;
		for (nc = 0; nc < nWCount; nc++)
		{
			LcomD = aLcomD.GetAt(lc); LcomD.nActive = 1; LoadCaseNo = GetCountLcomCase(LcomD);
			if (m_nWCount > 0)
			{
				double dWF = (nc % 2 == 0) ? dWFactor : -1. * dWFactor;
				if (Set_STLCIncludeCS(arWLoad.GetAt(nc / 2), dWF, LoadCaseNo, LcomD, 0) > 0)
				{
					CString strW = (dWF > 0.0) ? _T("+%sW") : _T("%sW");
					strDesc.Format(strW, GetFactorString(dWF)); LcomD.Description += strDesc;
					if (m_nWCount > 1) { strDesc.Format(_T("[%d]"), nc / 2 + 1); LcomD.Description += strDesc; }
				}
			}
			if (bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(W)))
			{
				Set_CStageCase(m_aCSLoad.GetAt(nnn), dWFactor, LoadCaseNo, LcomD, TRUE);
				strDesc.Format(_T("+%s(cEL)"), GetFactorString(dWFactor)); LcomD.Description += strDesc;
			}
			if (nType == 2 || nType == 4) m_aAcdnLcomD.Add(LcomD);
			else                     Set_LoadComb4Civil(LcomD, D_STR, 0, nMsgType);
		}
		if (m_nSfCount > 0) // Stream Flow Pressure
		{
			LcomD = aLcomD.GetAt(lc); LcomD.nActive = 1; LoadCaseNo = GetCountLcomCase(LcomD);
			int nSF = 0;
			for (nc = 0; nc < m_nSfCount; nc++) nSF += Set_STLCIncludeCS(arSfLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD, 0);
			if (nSF > 0)
			{
				strDesc.Format(_T("+%sSF"), GetFactorString(dFactor)); LcomD.Description += strDesc;
				if (nType == 2 || nType == 4) m_aAcdnLcomD.Add(LcomD);
				else                     Set_LoadComb4Civil(LcomD, D_STR, 0, nMsgType);
			}
		}
		if (m_nIpCount > 0) // Ice Pressure
		{
			LcomD = aLcomD.GetAt(lc); LcomD.nActive = 1; LoadCaseNo = GetCountLcomCase(LcomD);
			int nIP = 0;
			for (nc = 0; nc < m_nIpCount; nc++) nIP += Set_STLCIncludeCS(arIpLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD, 0);
			if (nIP > 0)
			{
				strDesc.Format(_T("+%sIP"), GetFactorString(dFactor)); LcomD.Description += strDesc;
				if (nType == 2 || nType == 4) m_aAcdnLcomD.Add(LcomD);
				else                     Set_LoadComb4Civil(LcomD, D_STR, 0, nMsgType);
			}
		}
		// Temeperature
		if ((m_nTCount > 0 || m_nTpgCount > 0) || (bAddCsLoad && (CheckCSEL(D_SGLD_ER1_CVL, _LSX(T)) || CheckCSEL(D_SGLD_ER1_CVL, _LSX(TPG)))))
		{
			LcomD = aLcomD.GetAt(lc); LcomD.nActive = 1;
			SetCvlJTJD60_Capacity_TPG(LcomD, arTLoad, arTpgLoad, arFrLoad, dFactor, dFactor, FALSE, nType == 2 || nType == 4, nMsgType);
		}
		if (m_nFrCount > 0) // Friction
		{
			LcomD = aLcomD.GetAt(lc); LcomD.nActive = 1; LoadCaseNo = GetCountLcomCase(LcomD);
			int nFR = 0;
			for (nc = 0; nc < m_nFrCount; nc++) nFR += Set_STLCIncludeCS(arFrLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD, 0);
			if (nFR > 0)
			{
				strDesc.Format(_T("+%sFR"), GetFactorString(dFactor)); LcomD.Description += strDesc;
				if (nType == 2 || nType == 4) m_aAcdnLcomD.Add(LcomD);
				else                     Set_LoadComb4Civil(LcomD, D_STR, 0, nMsgType);
			}
		}
	}

	//if((m_nConstLoad!=1 && m_nLCount>0) || (m_nConstLoad==1 && CheckCSEL(D_SGLD_ER1_CVL, _LSX(L))) || nType==2)
	{
		// ¿µ±¸ÇÏÁß + Â÷·®ÇÏÁß + 1°³ÀÇ ±âÅ¸ °¡º¯ÇÏÁßÀÎ °æ¿ì(8°³)
		SetCvlJTJD60_Var1(arData, arLLoad, arIlLoad, arCfLoad, arLsLoad, arCrlLoad,
			arWLoad, arSfLoad, arIpLoad, arTLoad, arTpgLoad, arFrLoad,
			arBrkLoad, nType, nMsgType);
		// ¿µ±¸ÇÏÁß + Â÷·®ÇÏÁß + 2°³ÀÇ ±âÅ¸ °¡º¯ÇÏÁßÀÎ °æ¿ì(24°³)
		SetCvlJTJD60_Var2(arData, arLLoad, arIlLoad, arCfLoad, arLsLoad, arCrlLoad,
			arWLoad, arSfLoad, arIpLoad, arTLoad, arTpgLoad, arFrLoad,
			arBrkLoad, nType, nMsgType);
		// ¿µ±¸ÇÏÁß + Â÷·®ÇÏÁß + 3°³ÀÇ ±âÅ¸ °¡º¯ÇÏÁßÀÎ °æ¿ì(36°³)
		SetCvlJTJD60_Var3(arData, arLLoad, arIlLoad, arCfLoad, arLsLoad, arCrlLoad,
			arWLoad, arSfLoad, arIpLoad, arTLoad, arTpgLoad, arFrLoad,
			arBrkLoad, nType, nMsgType);
		// ¿µ±¸ÇÏÁß + Â÷·®ÇÏÁß + 4°³ÀÇ ±âÅ¸ °¡º¯ÇÏÁßÀÎ °æ¿ì(29°³)
		SetCvlJTJD60_Var4(arData, arLLoad, arIlLoad, arCfLoad, arLsLoad, arCrlLoad,
			arWLoad, arSfLoad, arIpLoad, arTLoad, arTpgLoad, arFrLoad,
			arBrkLoad, nType, nMsgType);
		// ¿µ±¸ÇÏÁß + Â÷·®ÇÏÁß + 5°³ ¹× 6°³ÀÇ ±âÅ¸ °¡º¯ÇÏÁßÀÎ °æ¿ì(12°³ ¹× 2°³)
		SetCvlJTJD60_Var56(arData, arLLoad, arIlLoad, arCfLoad, arLsLoad, arCrlLoad,
			arWLoad, arSfLoad, arIpLoad, arTLoad, arTpgLoad, arFrLoad,
			arBrkLoad, nType, nMsgType);
	}

	return TRUE;
}
void CLoadCombCtrl::SetCvlJTJD60_Capacity_TPG(T_LCOM_D rData, ArINT& arTLoad, ArINT& arTpgLoad, ArINT& arFrLoad,
	double dTFact, double dTpgFact, BOOL bFr, BOOL bAcdn, int nMsgType, int nStrSer, int nType)
{
	if (bFr && m_nFrCount == 0) return;

	int LoadCaseNo = 0;
	CString strDesc = _T("");
	int  nnn = 0;

	BOOL bErec = FALSE;
	for (int nErec = 0; nErec < D_STCT_EREC_MAX_CVL; ++nErec)
	{
		if (m_nConstLoad != 0 && CheckCS_SgldK(D_SGLD_ER1_CVL + nErec, nnn)) bErec = TRUE;
	}

	BOOL bAddCsLoad = bErec;
	//  BOOL bAddCsLoad = (m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER1_CVL,nnn)) || 
	// 		              (m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER2_CVL,nnn)) || 
	// 					  (m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER3_CVL,nnn)) ||
	// 					  (m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER4_CVL,nnn)) ||
	// 					  (m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER5_CVL,nnn)) ||
	// 					  (m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER6_CVL,nnn)) ||
	// 					  (m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER7_CVL,nnn)) ||
	// 					  (m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER8_CVL,nnn)) ||
	// 					  (m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER9_CVL,nnn)) ||
	// 					  (m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER10_CVL,nnn)); // CS or ST+CS
	BOOL bAddLcomD = FALSE;

	int nTCount = max(m_nTCount, 1);
	for (int t = 0; t < nTCount; t++)
	{
		T_LCOM_D LcomD = rData;
		LoadCaseNo = GetCountLcomCase(LcomD);
		if (m_nTCount > 0)
		{
			int nTL = Set_STLCIncludeCS(arTLoad.GetAt(t), dTFact, LoadCaseNo, LcomD, 0);
			if (nTL > 0)
			{
				strDesc.Format(_T("+%sT"), GetFactorString(dTFact)); LcomD.Description += strDesc;
				if (m_nTCount > 1) { strDesc.Format(_T("[%d]"), t + 1); LcomD.Description += strDesc; }
			}
		}
		if (bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(T)))
		{
			Set_CStageCase(m_aCSLoad.GetAt(nnn), dTFact, LoadCaseNo, LcomD, TRUE);
			strDesc.Format(_T("+%s(cEL)"), GetFactorString(dTFact)); LcomD.Description += strDesc;
		}
		int nTpgCount = max(m_nTpgCount, 1);
		for (int tpg = 0; tpg < nTpgCount; tpg++)
		{
			T_LCOM_D LcomD_tpg = LcomD;
			LoadCaseNo = GetCountLcomCase(LcomD_tpg);
			if (m_nTpgCount > 0)
			{
				int nTPG = Set_STLCIncludeCS(arTpgLoad.GetAt(tpg), dTpgFact, LoadCaseNo, LcomD_tpg, 0);
				if (nTPG > 0)
				{
					strDesc.Format(_T("+%sTPG"), GetFactorString(dTpgFact)); LcomD_tpg.Description += strDesc;
					if (m_nTpgCount > 1) { strDesc.Format(_T("[%d]"), tpg + 1); LcomD_tpg.Description += strDesc; }
				}
			}
			if (bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(TPG)))
			{
				Set_CStageCase(m_aCSLoad.GetAt(nnn), dTpgFact, LoadCaseNo, LcomD_tpg, TRUE);
				strDesc.Format(_T("+%s(cEL)"), GetFactorString(dTpgFact)); LcomD_tpg.Description += strDesc;
			}

			if (bFr && m_nFrCount > 0)
			{
				int nFR = 0;
				for (int fr = 0; fr < m_nFrCount; fr++) nFR += Set_STLCIncludeCS(arFrLoad.GetAt(fr), dTFact, LoadCaseNo, LcomD_tpg, 0);
				if (nFR > 0) { strDesc.Format(_T("+%sFR"), GetFactorString(dTFact)); LcomD_tpg.Description += strDesc; }
			}

			if (bAcdn) m_aAcdnLcomD.Add(LcomD_tpg);
			else      Set_LoadComb4Civil(LcomD_tpg, nStrSer, nType, nMsgType);
		}
	}
}
BOOL CLoadCombCtrl::SetCvlJTJD60_Var1(const CArray<T_LCOM_D, T_LCOM_D&>& arData, ArINT& arLLoad,
	ArINT& arIlLoad, ArINT& arCfLoad, ArINT& arLsLoad, ArINT& arCrlLoad, ArINT& arWLoad,
	ArINT& arSfLoad, ArINT& arIpLoad, ArINT& arTLoad, ArINT& arTpgLoad, ArINT& arFrLoad,
	ArINT& arBrkLoad, int nType, int nMsgType)
{
	// nType == 0,4: °è¼öÁ¶ÇÕ, 1: 1.0(settlement Á¦¿Ü), 2: 1.0(¸ðµÎ), Live Load Á¦¿Ü
	CArray<T_LCOM_D, T_LCOM_D&> aLcomD; aLcomD.Copy(arData);
	int  nLcomSize = aLcomD.GetSize();
	int  nnn = 0;

	BOOL bErec = FALSE;
	for (int nErec = 0; nErec < D_STCT_EREC_MAX_CVL; ++nErec)
	{
		if (m_nConstLoad != 0 && CheckCS_SgldK(D_SGLD_ER1_CVL + nErec, nnn)) bErec = TRUE;
	}

	BOOL bAddCsLoad = bErec;
	//  BOOL bAddCsLoad = (m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER1_CVL,nnn)) || 
	// 		              (m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER2_CVL,nnn)) || 
	// 					  (m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER3_CVL,nnn)) ||
	// 					  (m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER4_CVL,nnn)) ||
	// 					  (m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER5_CVL,nnn)) ||
	// 					  (m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER6_CVL,nnn)) ||
	// 					  (m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER7_CVL,nnn)) ||
	// 					  (m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER8_CVL,nnn)) ||
	// 					  (m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER9_CVL,nnn)) ||
	// 					  (m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER10_CVL,nnn));// CS or ST+CS

	double dLFactor = (nType == 0 || nType == 4) ? 1.4 : 1.0;
	double dFactor = (nType == 0 || nType == 4) ? 1.4*0.8 : 1.0;
	double dWFactor = (nType == 0 || nType == 4) ? 1.1*0.8 : 1.0;

	// ¿µ±¸ÇÏÁß + Â÷·®ÇÏÁß + 1°³ÀÇ °¡º¯ÇÏÁßÀÎ °æ¿ì
	CString strDesc = _T("");
	int LoadCaseNo = 0, nc = 0;
	for (int lc = 0; lc < nLcomSize; lc++)
	{
		T_LCOM_D LcomD = aLcomD.GetAt(lc); LcomD.nActive = 1; // Active or Strength/Stress
		T_LCOM_D LcomD_cf = LcomD; // MNET:1614, KJH20050809, CF°¡ Á¦¿ÜµÈ ÇÏÁßÁ¶ÇÕ
		LoadCaseNo = GetCountLcomCase(LcomD);
		BOOL bCF = FALSE;
		if (nType != 2)
		{
			BOOL bAddLcomD = FALSE;
			if (m_nLCount > 0 || (bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(L)))) // ST or ST+CS
			{
				int nLL = 0, nIL = 0, nCF = 0;
				for (nc = 0; nc < m_nLCount; nc++) nLL += Set_STLCIncludeCS(arLLoad.GetAt(nc), dLFactor, LoadCaseNo, LcomD, 0);
				if (nLL > 0) { strDesc.Format(_T("+%sL"), GetFactorString(dLFactor)); LcomD.Description += strDesc; }
				if (bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(L)))
				{
					Set_CStageCase(m_aCSLoad.GetAt(nnn), dLFactor, LoadCaseNo, LcomD, TRUE);
					strDesc.Format(_T("+%s(cEL)"), GetFactorString(dLFactor)); LcomD.Description += strDesc;
					bAddLcomD = TRUE;
				}
				for (nc = 0; nc < m_nIlCount; nc++) nIL += Set_STLCIncludeCS(arIlLoad.GetAt(nc), dLFactor, LoadCaseNo, LcomD, 0);
				if (nIL > 0) { strDesc.Format(_T("+%sIL"), GetFactorString(dLFactor)); LcomD.Description += strDesc; }

				LcomD_cf = LcomD;
				for (nc = 0; nc < m_nCfCount; nc++) nCF += Set_STLCIncludeCS(arCfLoad.GetAt(nc), dLFactor, LoadCaseNo, LcomD, 0);
				if (nCF > 0) { strDesc.Format(_T("+%sCF"), GetFactorString(dLFactor)); LcomD.Description += strDesc; bCF = TRUE; }
				if (nLL + nIL + nCF > 0) bAddLcomD = TRUE;
			}
			if (bAddLcomD)
			{
				if (nType == 2 || nType == 4) m_aAcdnLcomD.Add(LcomD);
				else                     Set_LoadComb4Civil(LcomD, D_STR, 0, nMsgType);
			}
		}

		if (m_nLsCount > 0) // Live Load Surcharge
		{
			T_LCOM_D LcomD_l = LcomD; LoadCaseNo = GetCountLcomCase(LcomD_l);
			int nLS = 0;
			for (nc = 0; nc < m_nLsCount; nc++) nLS += Set_STLCIncludeCS(arLsLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD_l, 0);
			if (nLS > 0)
			{
				strDesc.Format(_T("+%sLS"), GetFactorString(dFactor)); LcomD_l.Description += strDesc;
				if (nType == 2 || nType == 4) m_aAcdnLcomD.Add(LcomD_l);
				else                     Set_LoadComb4Civil(LcomD_l, D_STR, 0, nMsgType);
			}
		}
		if (nType != 2 && m_nCrlCount > 0) // Crowd Load
		{
			T_LCOM_D LcomD_l = LcomD; LoadCaseNo = GetCountLcomCase(LcomD_l);
			int nCRL = 0;
			for (nc = 0; nc < m_nCrlCount; nc++) nCRL += Set_STLCIncludeCS(arCrlLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD_l, 0);
			if (nCRL > 0)
			{
				strDesc.Format(_T("+%sCRL"), GetFactorString(dFactor)); LcomD_l.Description += strDesc;
				if (nType == 2 || nType == 4) m_aAcdnLcomD.Add(LcomD_l);
				else                     Set_LoadComb4Civil(LcomD_l, D_STR, 0, nMsgType);
			}
		}
		if (nType != 2 && m_nBrkCount > 0 && m_nLCount > 0) // Brake Load
		{
			T_LCOM_D LcomD_l = LcomD; LoadCaseNo = GetCountLcomCase(LcomD_l);
			double dBrkF = (nType == 0 || nType == 4) ? dFactor * 0.7 : 1.0;
			int nBRK = 0;
			for (nc = 0; nc < m_nBrkCount; nc++) nBRK += Set_STLCIncludeCS(arBrkLoad.GetAt(nc), dBrkF, LoadCaseNo, LcomD_l, 0);
			if (nBRK > 0)
			{
				strDesc.Format(_T("+%sBRK"), GetFactorString(dBrkF)); LcomD_l.Description += strDesc;
				if (nType == 2 || nType == 4) m_aAcdnLcomD.Add(LcomD_l);
				else                     Set_LoadComb4Civil(LcomD_l, D_STR, 0, nMsgType);
			}
			if (bCF)
			{
				LcomD_l = LcomD_cf; LoadCaseNo = GetCountLcomCase(LcomD_l);
				double dBrkF = (nType == 0 || nType == 4) ? dFactor : 1.0;
				nBRK = 0;
				for (nc = 0; nc < m_nBrkCount; nc++) nBRK += Set_STLCIncludeCS(arBrkLoad.GetAt(nc), dBrkF, LoadCaseNo, LcomD_l, 0);
				if (nBRK > 0)
				{
					strDesc.Format(_T("+%sBRK"), GetFactorString(dBrkF)); LcomD_l.Description += strDesc;
					if (nType == 2 || nType == 4) m_aAcdnLcomD.Add(LcomD_l);
					else                     Set_LoadComb4Civil(LcomD_l, D_STR, 0, nMsgType);
				}
			}
		}
		int nWCount = m_nWCount * 2; // MNET:1614, KJH20050808
		if (m_nWCount == 0 && bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(W))) nWCount = 1;
		for (nc = 0; nc < nWCount; nc++)
		{
			T_LCOM_D LcomD_l = LcomD; LoadCaseNo = GetCountLcomCase(LcomD_l);
			if (m_nWCount > 0)
			{
				double dWF = (nc % 2 == 0) ? dWFactor : -1.*dWFactor;
				if (Set_STLCIncludeCS(arWLoad.GetAt(nc / 2), dWF, LoadCaseNo, LcomD_l, 0) > 0)
				{
					CString strW = (dWF > 0.0) ? _T("+%sW") : _T("%sW");
					strDesc.Format(strW, GetFactorString(dWF)); LcomD_l.Description += strDesc;
					if (m_nWCount > 1) { strDesc.Format(_T("[%d]"), nc / 2 + 1); LcomD_l.Description += strDesc; }
				}
			}
			if (bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(W)))
			{
				Set_CStageCase(m_aCSLoad.GetAt(nnn), dWFactor, LoadCaseNo, LcomD_l, TRUE);
				strDesc.Format(_T("+%s(cEL)"), GetFactorString(dWFactor)); LcomD_l.Description += strDesc;
			}
			if (nType == 2 || nType == 4) m_aAcdnLcomD.Add(LcomD_l);
			else                     Set_LoadComb4Civil(LcomD_l, D_STR, 0, nMsgType);
		}
		if (m_nSfCount > 0) // Stream Flow Pressure
		{
			T_LCOM_D LcomD_l = LcomD; LoadCaseNo = GetCountLcomCase(LcomD_l);
			int nSF = 0;
			for (nc = 0; nc < m_nSfCount; nc++) nSF += Set_STLCIncludeCS(arSfLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD_l, 0);
			if (nSF > 0)
			{
				strDesc.Format(_T("+%sSF"), GetFactorString(dFactor)); LcomD_l.Description += strDesc;
				if (nType == 2 || nType == 4) m_aAcdnLcomD.Add(LcomD_l);
				else                     Set_LoadComb4Civil(LcomD_l, D_STR, 0, nMsgType);
			}
		}
		if (m_nIpCount > 0) // Ice Pressure
		{
			T_LCOM_D LcomD_l = LcomD; LoadCaseNo = GetCountLcomCase(LcomD_l);
			int nIP = 0;
			for (nc = 0; nc < m_nIpCount; nc++) nIP += Set_STLCIncludeCS(arIpLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD_l, 0);
			if (nIP > 0)
			{
				strDesc.Format(_T("+%sIP"), GetFactorString(dFactor)); LcomD_l.Description += strDesc;
				if (nType == 2 || nType == 4) m_aAcdnLcomD.Add(LcomD_l);
				else                     Set_LoadComb4Civil(LcomD_l, D_STR, 0, nMsgType);
			}
		}
		// Temeperature
		if ((m_nTCount > 0 || m_nTpgCount > 0) || (bAddCsLoad && (CheckCSEL(D_SGLD_ER1_CVL, _LSX(T)) || CheckCSEL(D_SGLD_ER1_CVL, _LSX(TPG)))))
		{
			T_LCOM_D LcomD_l = LcomD;
			SetCvlJTJD60_Capacity_TPG(LcomD_l, arTLoad, arTpgLoad, arFrLoad, dFactor, dFactor, FALSE, nType == 2 || nType == 4, nMsgType);
		}
		if (m_nFrCount > 0) // Friction
		{
			T_LCOM_D LcomD_l = LcomD; LoadCaseNo = GetCountLcomCase(LcomD_l);
			int nFR = 0;
			for (nc = 0; nc < m_nFrCount; nc++) nFR += Set_STLCIncludeCS(arFrLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD_l, 0);
			if (nFR > 0)
			{
				strDesc.Format(_T("+%sFR"), GetFactorString(dFactor)); LcomD_l.Description += strDesc;
				if (nType == 2 || nType == 4) m_aAcdnLcomD.Add(LcomD_l);
				else                     Set_LoadComb4Civil(LcomD_l, D_STR, 0, nMsgType);
			}
		}
	}

	return TRUE;
}

BOOL CLoadCombCtrl::SetCvlJTJD60_Var2(const CArray<T_LCOM_D, T_LCOM_D&>& arData, ArINT& arLLoad,
	ArINT& arIlLoad, ArINT& arCfLoad, ArINT& arLsLoad, ArINT& arCrlLoad, ArINT& arWLoad,
	ArINT& arSfLoad, ArINT& arIpLoad, ArINT& arTLoad, ArINT& arTpgLoad, ArINT& arFrLoad,
	ArINT& arBrkLoad, int nType, int nMsgType)
{
	// nType == 0,4: °è¼öÁ¶ÇÕ, 1: 1.0(settlement Á¦¿Ü), 2: 1.0(¸ðµÎ), Live Load Á¦¿Ü
	CArray<T_LCOM_D, T_LCOM_D&> aLcomD; aLcomD.Copy(arData);
	int  nLcomSize = aLcomD.GetSize();
	int  nnn = 0;

	BOOL bErec = FALSE;
	for (int nErec = 0; nErec < D_STCT_EREC_MAX_CVL; ++nErec)
	{
		if (m_nConstLoad != 0 && CheckCS_SgldK(D_SGLD_ER1_CVL + nErec, nnn)) bErec = TRUE;
	}

	BOOL bAddCsLoad = bErec;
	//  BOOL bAddCsLoad = (m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER1_CVL,nnn)) || 
	// 		              (m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER2_CVL,nnn)) || 
	// 					  (m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER3_CVL,nnn)) ||
	// 					  (m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER4_CVL,nnn)) ||
	// 					  (m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER5_CVL,nnn)) ||
	// 					  (m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER6_CVL,nnn)) ||
	// 					  (m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER7_CVL,nnn)) ||
	// 					  (m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER8_CVL,nnn)) ||
	// 					  (m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER9_CVL,nnn)) ||
	// 					  (m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER10_CVL,nnn));// CS or ST+CS;

	double dLFactor = (nType == 0 || nType == 4) ? 1.4 : 1.0;
	double dFactor = (nType == 0 || nType == 4) ? 1.4*0.7 : 1.0;
	double dWFactor = (nType == 0 || nType == 4) ? 1.1*0.7 : 1.0;
	double dF = (nType == 0 || nType == 4) ? 0.7 : 1.0;

	// ¿µ±¸ÇÏÁß + Â÷·®ÇÏÁß + 2°³ÀÇ °¡º¯ÇÏÁßÀÎ °æ¿ì
	CString strDesc = _T("");
	int LoadCaseNo = 0, nc = 0;
	for (int lc = 0; lc < nLcomSize; lc++)
	{
		T_LCOM_D LcomD = aLcomD.GetAt(lc); LcomD.nActive = 1; // Active or Strength/Stress
		T_LCOM_D LcomD_cf = LcomD; // MNET:1614, KJH20050809, CF°¡ Á¦¿ÜµÈ ÇÏÁßÁ¶ÇÕ
		LoadCaseNo = GetCountLcomCase(LcomD);

		BOOL bLive = FALSE, bCF = FALSE;
		if (nType != 2)
		{
			bLive = m_nLCount > 0 || (bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(L)));
			if (bLive) // ST or ST+CS
			{
				int nLL = 0, nIL = 0, nCF = 0;
				for (nc = 0; nc < m_nLCount; nc++) nLL += Set_STLCIncludeCS(arLLoad.GetAt(nc), dLFactor, LoadCaseNo, LcomD, 0);
				if (nLL > 0) { strDesc.Format(_T("+%sL"), GetFactorString(dLFactor)); LcomD.Description += strDesc; }
				if (bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(L)))
				{
					Set_CStageCase(m_aCSLoad.GetAt(nnn), dLFactor, LoadCaseNo, LcomD, TRUE);
					strDesc.Format(_T("+%s(cEL)"), GetFactorString(dLFactor)); LcomD.Description += strDesc;
				}
				for (nc = 0; nc < m_nIlCount; nc++) nIL += Set_STLCIncludeCS(arIlLoad.GetAt(nc), dLFactor, LoadCaseNo, LcomD, 0);
				if (nIL > 0) { strDesc.Format(_T("+%sIL"), GetFactorString(dLFactor)); LcomD.Description += strDesc; }

				LcomD_cf = LcomD;
				for (nc = 0; nc < m_nCfCount; nc++) nCF += Set_STLCIncludeCS(arCfLoad.GetAt(nc), dLFactor, LoadCaseNo, LcomD, 0);
				if (nCF > 0) { strDesc.Format(_T("+%sCF"), GetFactorString(dLFactor)); LcomD.Description += strDesc; bCF = TRUE; }
			}
		}

		if (m_nLsCount > 0) // Live Load Surcharge
		{
			T_LCOM_D LcomD_l = LcomD; LoadCaseNo = GetCountLcomCase(LcomD_l);
			int nLS = 0;
			for (nc = 0; nc < m_nLsCount; nc++) nLS += Set_STLCIncludeCS(arLsLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD_l, 0);
			T_LCOM_D LcomD_tmp = LcomD_cf;
			if (bCF)
			{
				LoadCaseNo = GetCountLcomCase(LcomD_tmp);
				for (nc = 0; nc < m_nLsCount; nc++) Set_STLCIncludeCS(arLsLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD_tmp, 0);
			}
			if (nLS > 0)
			{
				strDesc.Format(_T("+%sLS"), GetFactorString(dFactor)); LcomD_l.Description += strDesc;
				if (bCF) LcomD_tmp.Description += strDesc;
				ArINT artmp;
				SetCvlJTJD60_ETC(LcomD_l, LcomD_tmp, bCF, arCrlLoad, artmp, nType != 2, arBrkLoad, bLive, arWLoad, TRUE, arSfLoad, TRUE,
					arIpLoad, TRUE, arTLoad, arTpgLoad, TRUE, arFrLoad, TRUE, 0, dF, nType == 2 || nType == 4, nMsgType);
			}
		}
		if (nType != 2 && m_nCrlCount > 0) // Crowd Load
		{
			T_LCOM_D LcomD_l = LcomD; LoadCaseNo = GetCountLcomCase(LcomD_l);
			int nCRL = 0;
			for (nc = 0; nc < m_nCrlCount; nc++) nCRL += Set_STLCIncludeCS(arCrlLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD_l, 0);
			T_LCOM_D LcomD_tmp = LcomD_cf;
			if (bCF)
			{
				LoadCaseNo = GetCountLcomCase(LcomD_tmp);
				for (nc = 0; nc < m_nCrlCount; nc++) Set_STLCIncludeCS(arCrlLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD_tmp, 0);
			}
			if (nCRL > 0)
			{
				strDesc.Format(_T("+%sCRL"), GetFactorString(dFactor)); LcomD_l.Description += strDesc;
				if (bCF) LcomD_tmp.Description += strDesc;
				ArINT artmp;
				SetCvlJTJD60_ETC(LcomD_l, LcomD_tmp, bCF, arCrlLoad, artmp, FALSE, arBrkLoad, m_nLCount > 0, arWLoad, TRUE, arSfLoad, TRUE, arIpLoad, TRUE,
					arTLoad, arTpgLoad, TRUE, arFrLoad, TRUE, 0, dF, nType == 2 || nType == 4, nMsgType);
			}
		}
		if (m_nBrkCount > 0 && bLive) // Brake Load
		{
			T_LCOM_D LcomD_l = LcomD; LoadCaseNo = GetCountLcomCase(LcomD_l);
			double dBrkF = (nType == 0 || nType == 4) ? dFactor * 0.7 : 1.0;
			int nBRK = 0;
			for (nc = 0; nc < m_nBrkCount; nc++) nBRK += Set_STLCIncludeCS(arBrkLoad.GetAt(nc), dBrkF, LoadCaseNo, LcomD_l, 0);
			if (nBRK > 0)
			{
				strDesc.Format(_T("+%sBRK"), GetFactorString(dBrkF)); LcomD_l.Description += strDesc;
				ArINT artmp;
				SetCvlJTJD60_ETC(LcomD_l, LcomD_cf, FALSE, arCrlLoad, artmp, FALSE, arBrkLoad, FALSE, arWLoad, TRUE, arSfLoad, FALSE, arIpLoad, FALSE,
					arTLoad, arTpgLoad, TRUE, arFrLoad, FALSE, 0, dF, nType == 2 || nType == 4, nMsgType);
			}
			if (bCF) // MNET:1614, KJH20050809, CF°¡ ¾ø´Â ÇÏÁßÁ¶ÇÕ ÇÑ¹ø´õ...
			{
				LcomD_l = LcomD_cf; LoadCaseNo = GetCountLcomCase(LcomD_l);
				dBrkF = (nType == 0 || nType == 4) ? dFactor : 1.0;
				nBRK = 0;
				for (nc = 0; nc < m_nBrkCount; nc++) nBRK += Set_STLCIncludeCS(arBrkLoad.GetAt(nc), dBrkF, LoadCaseNo, LcomD_l, 0);
				if (nBRK > 0)
				{
					strDesc.Format(_T("+%sBRK"), GetFactorString(dBrkF)); LcomD_l.Description += strDesc;
					ArINT artmp;
					SetCvlJTJD60_ETC(LcomD_l, LcomD_cf, FALSE, arCrlLoad, artmp, FALSE, arBrkLoad, FALSE, arWLoad, TRUE, arSfLoad, FALSE, arIpLoad, FALSE,
						arTLoad, arTpgLoad, TRUE, arFrLoad, FALSE, 0, dF, nType == 2 || nType == 4, nMsgType);
				}
			}
		}
		int nWCount = m_nWCount * 2; // MNET:1614, KJH20050808
		if (m_nWCount == 0 && bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(W))) nWCount = 1;
		for (nc = 0; nc < nWCount; nc++)
		{
			T_LCOM_D LcomD_l = LcomD; LoadCaseNo = GetCountLcomCase(LcomD_l);
			if (m_nWCount > 0)
			{
				double dWF = (nc % 2 == 0) ? dWFactor : -1.*dWFactor;
				if (Set_STLCIncludeCS(arWLoad.GetAt(nc / 2), dWF, LoadCaseNo, LcomD_l, 0) > 0)
				{
					CString strW = (dWF > 0.0) ? _T("+%sW") : _T("%sW");
					strDesc.Format(strW, GetFactorString(dWF)); LcomD_l.Description += strDesc;
					if (m_nWCount > 1) { strDesc.Format(_T("[%d]"), nc / 2 + 1); LcomD_l.Description += strDesc; }
				}
			}
			if (bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(W)))
			{
				Set_CStageCase(m_aCSLoad.GetAt(nnn), dWFactor, LoadCaseNo, LcomD_l, TRUE);
				strDesc.Format(_T("+%s(cEL)"), GetFactorString(dWFactor)); LcomD_l.Description += strDesc;
			}
			ArINT artmp;
			SetCvlJTJD60_ETC(LcomD_l, LcomD_cf, bCF, arCrlLoad, artmp, FALSE, arBrkLoad, FALSE, arWLoad, FALSE, arSfLoad, TRUE, arIpLoad, TRUE,
				arTLoad, arTpgLoad, TRUE, arFrLoad, TRUE, 0, dF, nType == 2 || nType == 4, nMsgType);
		}
		if (m_nSfCount > 0) // Stream Flow Pressure
		{
			T_LCOM_D LcomD_l = LcomD; LoadCaseNo = GetCountLcomCase(LcomD_l);
			int nSF = 0;
			for (nc = 0; nc < m_nSfCount; nc++) nSF += Set_STLCIncludeCS(arSfLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD_l, 0);
			if (nSF > 0)
			{
				ArINT artmp;
				strDesc.Format(_T("+%sSF"), GetFactorString(dFactor)); LcomD_l.Description += strDesc;
				SetCvlJTJD60_ETC(LcomD_l, LcomD_cf, bCF, arCrlLoad, artmp, FALSE, arBrkLoad, FALSE, arWLoad, FALSE, arSfLoad, FALSE, arIpLoad, FALSE,
					arTLoad, arTpgLoad, TRUE, arFrLoad, TRUE, 0, dF, nType == 2 || nType == 4, nMsgType);
			}
		}
		if (m_nIpCount > 0) // Ice Pressure
		{
			T_LCOM_D LcomD_l = LcomD; LoadCaseNo = GetCountLcomCase(LcomD_l);
			int nIP = 0;
			for (nc = 0; nc < m_nIpCount; nc++) nIP += Set_STLCIncludeCS(arIpLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD_l, 0);
			if (nIP > 0)
			{
				strDesc.Format(_T("+%sIP"), GetFactorString(dFactor)); LcomD_l.Description += strDesc;
				ArINT artmp;
				SetCvlJTJD60_ETC(LcomD_l, LcomD_cf, bCF, arCrlLoad, artmp, FALSE, arBrkLoad, FALSE, arWLoad, FALSE, arSfLoad, FALSE, arIpLoad, FALSE,
					arTLoad, arTpgLoad, TRUE, arFrLoad, TRUE, 0, dF, nType == 2 || nType == 4, nMsgType);
			}
		}
		if ((m_nTCount > 0 || m_nTpgCount > 0) || (bAddCsLoad && (CheckCSEL(D_SGLD_ER1_CVL, _LSX(T)) || CheckCSEL(D_SGLD_ER1_CVL, _LSX(TPG))))) // Temeperature
		{
			T_LCOM_D LcomD_l = LcomD;
			SetCvlJTJD60_Capacity_TPG(LcomD_l, arTLoad, arTpgLoad, arFrLoad, dFactor, dFactor, TRUE, nType == 2 || nType == 4, nMsgType);
		}
	}

	return TRUE;
}
BOOL CLoadCombCtrl::SetCvlJTJD60_Var3(const CArray<T_LCOM_D, T_LCOM_D&>& arData, ArINT& arLLoad,
	ArINT& arIlLoad, ArINT& arCfLoad, ArINT& arLsLoad, ArINT& arCrlLoad, ArINT& arWLoad,
	ArINT& arSfLoad, ArINT& arIpLoad, ArINT& arTLoad, ArINT& arTpgLoad, ArINT& arFrLoad,
	ArINT& arBrkLoad, int nType, int nMsgType)
{
	// nType == 0,4: °è¼öÁ¶ÇÕ, 1: 1.0(settlement Á¦¿Ü), 2: 1.0(¸ðµÎ), Live Load Á¦¿Ü
	CArray<T_LCOM_D, T_LCOM_D&> aLcomD; aLcomD.Copy(arData);
	int nLcomSize = aLcomD.GetSize();
	int  nnn = 0;

	BOOL bErec = FALSE;
	for (int nErec = 0; nErec < D_STCT_EREC_MAX_CVL; ++nErec)
	{
		if (m_nConstLoad != 0 && CheckCS_SgldK(D_SGLD_ER1_CVL + nErec, nnn)) bErec = TRUE;
	}

	BOOL bAddCsLoad = bErec;
	// 	BOOL bAddCsLoad =   (m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER1_CVL,nnn)) || 
	// 		    			(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER2_CVL,nnn)) || 
	// 						(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER3_CVL,nnn)) ||
	// 						(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER4_CVL,nnn)) ||
	// 						(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER5_CVL,nnn)) ||
	// 						(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER6_CVL,nnn)) ||
	// 						(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER7_CVL,nnn)) ||
	// 						(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER8_CVL,nnn)) ||
	// 						(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER9_CVL,nnn)) ||
	// 						(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER10_CVL,nnn));// CS or ST+CS;

	double dLFactor = (nType == 0 || nType == 4) ? 1.4 : 1.0;
	double dFactor = (nType == 0 || nType == 4) ? 1.4*0.6 : 1.0;
	double dWFactor = (nType == 0 || nType == 4) ? 1.1*0.6 : 1.0;
	double dF = (nType == 0 || nType == 4) ? 0.6 : 1.0;

	// ¿µ±¸ÇÏÁß + Â÷·®ÇÏÁß + 3°³ÀÇ °¡º¯ÇÏÁßÀÎ °æ¿ì
	CString strDesc = _T("");
	int LoadCaseNo = 0, nc = 0;
	for (int lc = 0; lc < nLcomSize; lc++)
	{
		T_LCOM_D LcomD = aLcomD.GetAt(lc); LcomD.nActive = 1; // Active or Strength/Stress
		T_LCOM_D LcomD_cf = LcomD; // MNET:1614, KJH20050809, CF°¡ ¾ø´Â ÇÏÁßÁ¶ÇÕ
		LoadCaseNo = GetCountLcomCase(LcomD);

		BOOL bLive = FALSE, bCF = FALSE;
		if (nType != 2)
		{
			bLive = m_nLCount > 0 || (bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(L)));
			if (bLive) // ST or ST+CS
			{
				int nLL = 0, nIL = 0, nCF = 0;
				for (nc = 0; nc < m_nLCount; nc++) nLL += Set_STLCIncludeCS(arLLoad.GetAt(nc), dLFactor, LoadCaseNo, LcomD, 0);
				if (nLL > 0) { strDesc.Format(_T("+%sL"), GetFactorString(dLFactor)); LcomD.Description += strDesc; }
				if (bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(L)))
				{
					Set_CStageCase(m_aCSLoad.GetAt(nnn), dLFactor, LoadCaseNo, LcomD, TRUE);
					strDesc.Format(_T("+%s(cEL)"), GetFactorString(dLFactor)); LcomD.Description += strDesc;
				}
				for (nc = 0; nc < m_nIlCount; nc++) nIL += Set_STLCIncludeCS(arIlLoad.GetAt(nc), dLFactor, LoadCaseNo, LcomD, 0);
				if (nIL > 0) { strDesc.Format(_T("+%sIL"), GetFactorString(dLFactor)); LcomD.Description += strDesc; }

				LcomD_cf = LcomD;
				for (nc = 0; nc < m_nCfCount; nc++) nCF += Set_STLCIncludeCS(arCfLoad.GetAt(nc), dLFactor, LoadCaseNo, LcomD, 0);
				if (nCF > 0) { strDesc.Format(_T("+%sCF"), GetFactorString(dLFactor)); LcomD.Description += strDesc; bCF = TRUE; }
			}
		}

		if (m_nLsCount > 0) // Live Load Surcharge
		{
			T_LCOM_D LcomD_l = LcomD; LoadCaseNo = GetCountLcomCase(LcomD_l);
			int nLS = 0;
			for (nc = 0; nc < m_nLsCount; nc++) nLS += Set_STLCIncludeCS(arLsLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD_l, 0);
			T_LCOM_D LcomD_tmp = LcomD_cf;
			if (bCF)
			{
				LoadCaseNo = GetCountLcomCase(LcomD_tmp);
				for (nc = 0; nc < m_nLsCount; nc++) Set_STLCIncludeCS(arLsLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD_tmp, 0);
			}
			if (nLS > 0)
			{
				strDesc.Format(_T("+%sLS"), GetFactorString(dFactor)); LcomD_l.Description += strDesc;
				if (bCF) LcomD_tmp.Description += strDesc;
				ArINT artmp;
				SetCvlJTJD60_ETC(LcomD_l, LcomD_tmp, bCF, arCrlLoad, artmp, nType != 2, arBrkLoad, bLive, arWLoad, TRUE, arSfLoad, TRUE, arIpLoad, TRUE,
					arTLoad, arTpgLoad, TRUE, arFrLoad, FALSE, 1, dF, nType == 2 || nType == 4, nMsgType);
			}
		}
		if (nType != 2 && m_nCrlCount > 0) // Crowd Load
		{
			T_LCOM_D LcomD_l = LcomD; LoadCaseNo = GetCountLcomCase(LcomD_l);
			int nCRL = 0;
			for (nc = 0; nc < m_nCrlCount; nc++) nCRL += Set_STLCIncludeCS(arCrlLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD_l, 0);
			T_LCOM_D LcomD_tmp = LcomD_cf;
			if (bCF)
			{
				LoadCaseNo = GetCountLcomCase(LcomD_tmp);
				for (nc = 0; nc < m_nCrlCount; nc++) Set_STLCIncludeCS(arCrlLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD_tmp, 0);
			}
			if (nCRL > 0)
			{
				strDesc.Format(_T("+%sCRL"), GetFactorString(dFactor)); LcomD_l.Description += strDesc;
				if (bCF) LcomD_tmp.Description += strDesc;
				ArINT artmp;
				SetCvlJTJD60_ETC(LcomD_l, LcomD_tmp, bCF, arCrlLoad, artmp, FALSE, arBrkLoad, bLive, arWLoad, TRUE, arSfLoad, TRUE, arIpLoad, TRUE,
					arTLoad, arTpgLoad, TRUE, arFrLoad, FALSE, 2, dF, nType == 2 || nType == 4, nMsgType);
			}
		}
		if (m_nBrkCount > 0 && bLive) // Brake Load
		{
			T_LCOM_D LcomD_l = LcomD; LoadCaseNo = GetCountLcomCase(LcomD_l);
			double dBrkF = (nType == 0 || nType == 4) ? dFactor * 0.7 : 1.0;
			int nBRK = 0;
			for (nc = 0; nc < m_nBrkCount; nc++) nBRK += Set_STLCIncludeCS(arBrkLoad.GetAt(nc), dBrkF, LoadCaseNo, LcomD_l, 0);
			if (nBRK > 0)
			{
				strDesc.Format(_T("+%sBRK"), GetFactorString(dBrkF)); LcomD_l.Description += strDesc;
				ArINT artmp;
				SetCvlJTJD60_ETC(LcomD_l, LcomD_cf, FALSE, arCrlLoad, artmp, FALSE, arBrkLoad, FALSE, arWLoad, TRUE, arSfLoad, FALSE, arIpLoad, FALSE,
					arTLoad, arTpgLoad, FALSE, arFrLoad, FALSE, 3, dF, nType == 2 || nType == 4, nMsgType);
			}
			if (bCF) // MNET:1614, KJH20050809, CF°¡ ¾ø´Â ÇÏÁßÁ¶ÇÕ ÇÑ¹ø´õ...
			{
				LcomD_l = LcomD_cf; LoadCaseNo = GetCountLcomCase(LcomD_l);
				dBrkF = (nType == 0 || nType == 4) ? dFactor : 1.0;
				nBRK = 0;
				for (nc = 0; nc < m_nBrkCount; nc++) nBRK += Set_STLCIncludeCS(arBrkLoad.GetAt(nc), dBrkF, LoadCaseNo, LcomD_l, 0);
				if (nBRK > 0)
				{
					strDesc.Format(_T("+%sBRK"), GetFactorString(dBrkF)); LcomD_l.Description += strDesc;
					ArINT artmp;
					SetCvlJTJD60_ETC(LcomD_l, LcomD_cf, FALSE, arCrlLoad, artmp, FALSE, arBrkLoad, FALSE, arWLoad, TRUE, arSfLoad, FALSE, arIpLoad, FALSE,
						arTLoad, arTpgLoad, FALSE, arFrLoad, FALSE, 3, dF, nType == 2 || nType == 4, nMsgType);
				}
			}
		}
		int nWCount = m_nWCount * 2; // MNET:1614, KJH20050808
		if (m_nWCount == 0 && bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(W))) nWCount = 1;
		for (nc = 0; nc < nWCount; nc++)
		{
			T_LCOM_D LcomD_l = LcomD; LoadCaseNo = GetCountLcomCase(LcomD_l);
			if (m_nWCount > 0)
			{
				double dWF = (nc % 2 == 0) ? dWFactor : -1.*dWFactor;
				if (Set_STLCIncludeCS(arWLoad.GetAt(nc / 2), dWF, LoadCaseNo, LcomD_l, 0) > 0)
				{
					CString strW = (dWF > 0.0) ? _T("+%sW") : _T("%sW");
					strDesc.Format(strW, GetFactorString(dWF)); LcomD_l.Description += strDesc;
					if (m_nWCount > 1) { strDesc.Format(_T("[%d]"), nc / 2 + 1); LcomD_l.Description += strDesc; }
				}
			}
			if (bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(W)))
			{
				Set_CStageCase(m_aCSLoad.GetAt(nnn), dWFactor, LoadCaseNo, LcomD_l, TRUE);
				strDesc.Format(_T("+%s(cEL)"), GetFactorString(dWFactor)); LcomD_l.Description += strDesc;
			}
			ArINT artmp;
			SetCvlJTJD60_ETC(LcomD_l, LcomD_cf, bCF, arCrlLoad, artmp, FALSE, arBrkLoad, FALSE, arWLoad, FALSE, arSfLoad, TRUE, arIpLoad, TRUE,
				arTLoad, arTpgLoad, TRUE, arFrLoad, FALSE, 4, dF, nType == 2 || nType == 4, nMsgType);
		}
		if (m_nSfCount > 0) // Stream Flow Pressure
		{
			T_LCOM_D LcomD_l = LcomD; LoadCaseNo = GetCountLcomCase(LcomD_l);
			int nSF = 0;
			for (nc = 0; nc < m_nSfCount; nc++) nSF += Set_STLCIncludeCS(arSfLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD_l, 0);
			if (nSF > 0)
			{
				strDesc.Format(_T("+%sSF"), GetFactorString(dFactor)); LcomD_l.Description += strDesc;
				ArINT artmp;
				SetCvlJTJD60_ETC(LcomD_l, LcomD_cf, bCF, arCrlLoad, artmp, FALSE, arBrkLoad, FALSE, arWLoad, FALSE, arSfLoad, FALSE, arIpLoad, FALSE,
					arTLoad, arTpgLoad, TRUE, arFrLoad, FALSE, 5, dF, nType == 2 || nType == 4, nMsgType);
			}
		}
		if (m_nIpCount > 0) // Ice Pressure
		{
			T_LCOM_D LcomD_l = LcomD; LoadCaseNo = GetCountLcomCase(LcomD_l);
			int nIP = 0;
			for (nc = 0; nc < m_nIpCount; nc++) nIP += Set_STLCIncludeCS(arIpLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD_l, 0);
			if (nIP > 0)
			{
				strDesc.Format(_T("+%sIP"), GetFactorString(dFactor)); LcomD_l.Description += strDesc;
				ArINT artmp;
				SetCvlJTJD60_ETC(LcomD_l, LcomD_cf, bCF, arCrlLoad, artmp, FALSE, arBrkLoad, FALSE, arWLoad, FALSE, arSfLoad, FALSE, arIpLoad, FALSE,
					arTLoad, arTpgLoad, TRUE, arFrLoad, FALSE, 5, dF, nType == 2 || nType == 4, nMsgType);
			}
		}
	}

	return TRUE;
}
BOOL CLoadCombCtrl::SetCvlJTJD60_Var4(const CArray<T_LCOM_D, T_LCOM_D&>& arData, ArINT& arLLoad,
	ArINT& arIlLoad, ArINT& arCfLoad, ArINT& arLsLoad, ArINT& arCrlLoad, ArINT& arWLoad,
	ArINT& arSfLoad, ArINT& arIpLoad, ArINT& arTLoad, ArINT& arTpgLoad, ArINT& arFrLoad,
	ArINT& arBrkLoad, int nType, int nMsgType)
{
	// nType == 0,4: °è¼öÁ¶ÇÕ, 1: 1.0(settlement Á¦¿Ü), 2: 1.0(¸ðµÎ), Live Load Á¦¿Ü
	CArray<T_LCOM_D, T_LCOM_D&> aLcomD; aLcomD.Copy(arData);
	int  nLcomSize = aLcomD.GetSize();
	int  nnn = 0;

	BOOL bErec = FALSE;
	for (int nErec = 0; nErec < D_STCT_EREC_MAX_CVL; ++nErec)
	{
		if (m_nConstLoad != 0 && CheckCS_SgldK(D_SGLD_ER1_CVL + nErec, nnn)) bErec = TRUE;
	}

	BOOL bAddCsLoad = bErec;
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER1_CVL,nnn)) || 
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER2_CVL,nnn)) || 
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER3_CVL,nnn)) ||
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER4_CVL,nnn)) ||
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER5_CVL,nnn)) ||
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER6_CVL,nnn)) ||
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER7_CVL,nnn)) ||
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER8_CVL,nnn)) ||
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER9_CVL,nnn)) ||
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER10_CVL,nnn));// CS or ST+CS;

	double dLFactor = (nType == 0 || nType == 4) ? 1.4 : 1.0;
	double dFactor = (nType == 0 || nType == 4) ? 1.4*0.5 : 1.0;
	double dWFactor = (nType == 0 || nType == 4) ? 1.1*0.5 : 1.0;
	double dF = (nType == 0 || nType == 4) ? 0.5 : 1.0;

	// ¿µ±¸ÇÏÁß + Â÷·®ÇÏÁß + 4°³ÀÇ °¡º¯ÇÏÁßÀÎ °æ¿ì
	CString strDesc = _T("");
	int LoadCaseNo = 0, nc = 0;
	for (int lc = 0; lc < nLcomSize; lc++)
	{
		T_LCOM_D LcomD = aLcomD.GetAt(lc); LcomD.nActive = 1; // Active or Strength/Stress
		T_LCOM_D LcomD_cf = LcomD; // MNET:1614, KJH20050809, CF°¡ ¾ø´Â ÇÏÁßÁ¶ÇÕ
		LoadCaseNo = GetCountLcomCase(LcomD);

		BOOL bLive = FALSE, bCF = FALSE;
		if (nType != 2)
		{
			bLive = m_nLCount > 0 || (bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(L)));
			if (bLive) // ST or ST+CS
			{
				int nLL = 0, nIL = 0, nCF = 0;
				for (nc = 0; nc < m_nLCount; nc++) nLL += Set_STLCIncludeCS(arLLoad.GetAt(nc), dLFactor, LoadCaseNo, LcomD, 0);
				if (nLL > 0) { strDesc.Format(_T("+%sL"), GetFactorString(dLFactor)); LcomD.Description += strDesc; }
				if (bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(L)))
				{
					Set_CStageCase(m_aCSLoad.GetAt(nnn), dLFactor, LoadCaseNo, LcomD, TRUE);
					strDesc.Format(_T("+%s(cEL)"), GetFactorString(dLFactor)); LcomD.Description += strDesc;
				}
				for (nc = 0; nc < m_nIlCount; nc++) nIL += Set_STLCIncludeCS(arIlLoad.GetAt(nc), dLFactor, LoadCaseNo, LcomD, 0);
				if (nIL > 0) { strDesc.Format(_T("+%sIL"), GetFactorString(dLFactor)); LcomD.Description += strDesc; }

				LcomD_cf = LcomD;
				for (nc = 0; nc < m_nCfCount; nc++) nCF += Set_STLCIncludeCS(arCfLoad.GetAt(nc), dLFactor, LoadCaseNo, LcomD, 0);
				if (nCF > 0) { strDesc.Format(_T("+%sCF"), GetFactorString(dLFactor)); LcomD.Description += strDesc; bCF = TRUE; }
			}
		}

		if (m_nLsCount > 0) // Live Load Surcharge
		{
			T_LCOM_D LcomD_l = LcomD; LoadCaseNo = GetCountLcomCase(LcomD_l);
			int nLS = 0;
			for (nc = 0; nc < m_nLsCount; nc++) nLS += Set_STLCIncludeCS(arLsLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD_l, 0);
			T_LCOM_D LcomD_tmp = LcomD_cf;
			if (bCF)
			{
				LoadCaseNo = GetCountLcomCase(LcomD_tmp);
				for (nc = 0; nc < m_nLsCount; nc++) Set_STLCIncludeCS(arLsLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD_tmp, 0);
			}
			if (nLS > 0)
			{
				strDesc.Format(_T("+%sLS"), GetFactorString(dFactor)); LcomD_l.Description += strDesc;
				if (bCF) LcomD_tmp.Description += strDesc;
				BOOL bInc = nType == 2 ? FALSE : TRUE;
				ArINT artmp;
				SetCvlJTJD60_ETC(LcomD_l, LcomD_tmp, bCF, arCrlLoad, artmp, nType != 2, arBrkLoad, bLive, arWLoad, TRUE, arSfLoad, TRUE, arIpLoad, TRUE,
					arTLoad, arTpgLoad, FALSE, arFrLoad, FALSE, 6, dF, nType == 2 || nType == 4, nMsgType);
			}
		}
		if (nType != 2 && m_nCrlCount > 0) // Crowd Load
		{
			T_LCOM_D LcomD_l = LcomD; LoadCaseNo = GetCountLcomCase(LcomD_l);
			int nCRL = 0;
			for (nc = 0; nc < m_nCrlCount; nc++) nCRL += Set_STLCIncludeCS(arCrlLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD_l, 0);
			T_LCOM_D LcomD_tmp = LcomD_cf;
			if (bCF)
			{
				LoadCaseNo = GetCountLcomCase(LcomD_tmp);
				for (nc = 0; nc < m_nCrlCount; nc++) Set_STLCIncludeCS(arCrlLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD_tmp, 0);
			}
			if (nCRL > 0)
			{
				strDesc.Format(_T("+%sCRL"), GetFactorString(dFactor)); LcomD_l.Description += strDesc;
				if (bCF) LcomD_tmp.Description += strDesc;
				ArINT artmp;
				SetCvlJTJD60_ETC(LcomD_l, LcomD_tmp, bCF, arCrlLoad, artmp, FALSE, arBrkLoad, bLive, arWLoad, TRUE, arSfLoad, TRUE, arIpLoad, TRUE,
					arTLoad, arTpgLoad, FALSE, arFrLoad, FALSE, 7, dF, nType == 2 || nType == 4, nMsgType);
			}
		}
		int nWCount = m_nWCount * 2; // MNET:1614, KJH20050808
		if (m_nWCount == 0 && bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(W))) nWCount = 1;
		for (nc = 0; nc < nWCount; nc++)
		{
			T_LCOM_D LcomD_l = LcomD; LoadCaseNo = GetCountLcomCase(LcomD_l);
			if (m_nWCount > 0)
			{
				double dWF = (nc % 2 == 0) ? dWFactor : -1.*dWFactor;
				if (Set_STLCIncludeCS(arWLoad.GetAt(nc / 2), dWF, LoadCaseNo, LcomD_l, 0) > 0)
				{
					CString strW = (dWF > 0.0) ? _T("+%sW") : _T("%sW");
					strDesc.Format(strW, GetFactorString(dWF)); LcomD_l.Description += strDesc;
					if (m_nWCount > 1) { strDesc.Format(_T("[%d]"), nc / 2 + 1); LcomD_l.Description += strDesc; }
				}
			}
			if (bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(W)))
			{
				Set_CStageCase(m_aCSLoad.GetAt(nnn), dWFactor, LoadCaseNo, LcomD_l, TRUE);
				strDesc.Format(_T("+%s(cEL)"), GetFactorString(dWFactor)); LcomD_l.Description += strDesc;
			}
			ArINT artmp;
			SetCvlJTJD60_ETC(LcomD_l, LcomD_cf, bCF, arCrlLoad, artmp, FALSE, arBrkLoad, FALSE, arWLoad, FALSE, arSfLoad, TRUE, arIpLoad, TRUE,
				arTLoad, arTpgLoad, FALSE, arFrLoad, FALSE, 7, dF, nType == 2 || nType == 4, nMsgType);
		}
	}

	return TRUE;
}
BOOL CLoadCombCtrl::SetCvlJTJD60_Var56(const CArray<T_LCOM_D, T_LCOM_D&>& arData, ArINT& arLLoad,
	ArINT& arIlLoad, ArINT& arCfLoad, ArINT& arLsLoad, ArINT& arCrlLoad, ArINT& arWLoad,
	ArINT& arSfLoad, ArINT& arIpLoad, ArINT& arTLoad, ArINT& arTpgLoad, ArINT& arFrLoad,
	ArINT& arBrkLoad, int nType, int nMsgType)
{
	// nType == 0,4: °è¼öÁ¶ÇÕ, 1: 1.0(settlement Á¦¿Ü), 2: 1.0(¸ðµÎ), Live Load Á¦¿Ü
	CArray<T_LCOM_D, T_LCOM_D&> aLcomD; aLcomD.Copy(arData);
	int  nLcomSize = aLcomD.GetSize();
	int  nnn = 0;

	BOOL bErec = FALSE;
	for (int nErec = 0; nErec < D_STCT_EREC_MAX_CVL; ++nErec)
	{
		if (m_nConstLoad != 0 && CheckCS_SgldK(D_SGLD_ER1_CVL + nErec, nnn)) bErec = TRUE;
	}

	BOOL bAddCsLoad = bErec;
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER1_CVL,nnn)) || 
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER2_CVL,nnn)) || 
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER3_CVL,nnn)) ||
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER4_CVL,nnn)) ||
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER5_CVL,nnn)) ||
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER6_CVL,nnn)) ||
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER7_CVL,nnn)) ||
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER8_CVL,nnn)) ||
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER9_CVL,nnn)) ||
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER10_CVL,nnn));// CS or ST+CS;

	double dLFactor = (nType == 0 || nType == 4) ? 1.4 : 1.0;
	double dFactor = (nType == 0 || nType == 4) ? 1.4*0.5 : 1.0;
	double dWFactor = (nType == 0 || nType == 4) ? 1.1*0.5 : 1.0;
	double dF = (nType == 0 || nType == 4) ? 0.5 : 1.0;

	// ¿µ±¸ÇÏÁß + Â÷·®ÇÏÁß + 5°³ÀÇ °¡º¯ÇÏÁßÀÎ °æ¿ì
	CString strDesc = _T("");
	int LoadCaseNo = 0, nc = 0;
	for (int lc = 0; lc < nLcomSize; lc++)
	{
		T_LCOM_D LcomD = aLcomD.GetAt(lc); LcomD.nActive = 1; // Active or Strength/Stress
		T_LCOM_D LcomD_cf = LcomD; // MNET:1614, KJH20050809, CF Á¦¿ÜÇÑ ÇÏÁßÁ¶ÇÕ
		LoadCaseNo = GetCountLcomCase(LcomD);

		BOOL bLive = FALSE, bCF = FALSE;
		if (nType != 2)
		{
			bLive = m_nLCount > 0 || (bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(L)));
			if (bLive) // ST or ST+CS
			{
				int nLL = 0, nIL = 0, nCF = 0;
				for (nc = 0; nc < m_nLCount; nc++) nLL += Set_STLCIncludeCS(arLLoad.GetAt(nc), dLFactor, LoadCaseNo, LcomD, 0);
				if (nLL > 0) { strDesc.Format(_T("+%sL"), GetFactorString(dLFactor)); LcomD.Description += strDesc; }
				if (bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(L)))
				{
					Set_CStageCase(m_aCSLoad.GetAt(nnn), dLFactor, LoadCaseNo, LcomD, TRUE);
					strDesc.Format(_T("+%s(cEL)"), GetFactorString(dLFactor)); LcomD.Description += strDesc;
				}
				for (nc = 0; nc < m_nIlCount; nc++) nIL += Set_STLCIncludeCS(arIlLoad.GetAt(nc), dLFactor, LoadCaseNo, LcomD, 0);
				if (nIL > 0) { strDesc.Format(_T("+%sIL"), GetFactorString(dLFactor)); LcomD.Description += strDesc; }

				LcomD_cf = LcomD;
				for (nc = 0; nc < m_nCfCount; nc++) nCF += Set_STLCIncludeCS(arCfLoad.GetAt(nc), dLFactor, LoadCaseNo, LcomD, 0);
				if (nCF > 0) { strDesc.Format(_T("+%sCF"), GetFactorString(dLFactor)); LcomD.Description += strDesc; bCF = TRUE; }
			}
		}

		if (m_nLsCount > 0) // Live Load Surcharge
		{
			T_LCOM_D LcomD_l = LcomD; LoadCaseNo = GetCountLcomCase(LcomD_l);
			int nLS = 0;
			for (nc = 0; nc < m_nLsCount; nc++) nLS += Set_STLCIncludeCS(arLsLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD_l, 0);
			T_LCOM_D LcomD_tmp = LcomD_cf;
			if (bCF)
			{
				LoadCaseNo = GetCountLcomCase(LcomD_tmp);
				for (nc = 0; nc < m_nLsCount; nc++) Set_STLCIncludeCS(arLsLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD_tmp, 0);
			}
			if (nLS > 0)
			{
				strDesc.Format(_T("+%sLS"), GetFactorString(dFactor)); LcomD_l.Description += strDesc;
				if (bCF) LcomD_tmp.Description += strDesc;
				ArINT artmp;
				SetCvlJTJD60_ETC(LcomD_l, LcomD_tmp, bCF, arCrlLoad, artmp, nType != 2, arBrkLoad, bLive, arWLoad, TRUE, arSfLoad, FALSE, arIpLoad, FALSE,
					arTLoad, arTpgLoad, FALSE, arFrLoad, FALSE, 8, dF, nType == 2 || nType == 4, nMsgType);
			}
		}
		if (nType != 2 && m_nCrlCount > 0) // Crowd Load
		{
			T_LCOM_D LcomD_l = LcomD; LoadCaseNo = GetCountLcomCase(LcomD_l);
			int nCRL = 0;
			for (nc = 0; nc < m_nCrlCount; nc++) nCRL += Set_STLCIncludeCS(arCrlLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD_l, 0);
			if (nCRL > 0)
			{
				strDesc.Format(_T("+%sCRL"), GetFactorString(dFactor)); LcomD_l.Description += strDesc;
				ArINT artmp;
				SetCvlJTJD60_ETC(LcomD_l, LcomD_cf, bCF, arCrlLoad, artmp, FALSE, arBrkLoad, FALSE, arWLoad, TRUE, arSfLoad, FALSE, arIpLoad, FALSE,
					arTLoad, arTpgLoad, FALSE, arFrLoad, FALSE, 9, dF, nType == 2 || nType == 4, nMsgType);
			}
		}
	}

	// ¿µ±¸ÇÏÁß + Â÷·®ÇÏÁß + 6°³ÀÇ °¡º¯ÇÏÁßÀÎ °æ¿ì
	for (int lc = 0; lc < nLcomSize; lc++)
	{
		T_LCOM_D LcomD = aLcomD.GetAt(lc); LcomD.nActive = 1; // Active or Strength/Stress
		LoadCaseNo = GetCountLcomCase(LcomD);

		if (nType != 2)
		{
			if (m_nLCount > 0 || (bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(L)))) // ST or ST+CS
			{
				int nLL = 0, nIL = 0, nCF = 0;
				for (nc = 0; nc < m_nLCount; nc++) nLL += Set_STLCIncludeCS(arLLoad.GetAt(nc), dLFactor, LoadCaseNo, LcomD, 0);
				if (nLL > 0) { strDesc.Format(_T("+%sL"), GetFactorString(dLFactor)); LcomD.Description += strDesc; }
				if (bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(L)))
				{
					Set_CStageCase(m_aCSLoad.GetAt(nnn), dLFactor, LoadCaseNo, LcomD, TRUE);
					strDesc.Format(_T("+%s(cEL)"), GetFactorString(dLFactor)); LcomD.Description += strDesc;
				}
				for (nc = 0; nc < m_nIlCount; nc++) nIL += Set_STLCIncludeCS(arIlLoad.GetAt(nc), dLFactor, LoadCaseNo, LcomD, 0);
				if (nIL > 0) { strDesc.Format(_T("+%sIL"), GetFactorString(dLFactor)); LcomD.Description += strDesc; }
				for (nc = 0; nc < m_nCfCount; nc++) nCF += Set_STLCIncludeCS(arCfLoad.GetAt(nc), dLFactor, LoadCaseNo, LcomD, 0);
				if (nCF > 0) { strDesc.Format(_T("+%sCF"), GetFactorString(dLFactor)); LcomD.Description += strDesc; }
			}
		}

		if (m_nLsCount > 0) // Live Load Surcharge
		{
			T_LCOM_D LcomD_l = LcomD; LoadCaseNo = GetCountLcomCase(LcomD_l);
			int nLS = 0;
			for (nc = 0; nc < m_nLsCount; nc++) nLS += Set_STLCIncludeCS(arLsLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD_l, 0);
			if (nLS > 0)
			{
				strDesc.Format(_T("+%sLS"), GetFactorString(dFactor)); LcomD_l.Description += strDesc;
				ArINT artmp;
				SetCvlJTJD60_ETC(LcomD_l, LcomD_l, FALSE, arCrlLoad, artmp, nType != 2, arBrkLoad, FALSE, arWLoad, FALSE, arSfLoad, FALSE, arIpLoad, FALSE,
					arTLoad, arTpgLoad, FALSE, arFrLoad, FALSE, 10, dF, nType == 2 || nType == 4, nMsgType);
			}
		}
	}

	return TRUE;
}
BOOL CLoadCombCtrl::SetCvlJTJD60_ETC(T_LCOM_D rData, T_LCOM_D Lcom_cf, BOOL bCF, ArINT& arCrlLoad, ArINT& arMCrlLoad, BOOL bCrl, ArINT& arBrkLoad, BOOL bBrk,
	ArINT& arWLoad, BOOL bW, ArINT& arSfLoad, BOOL bSf, ArINT& arIpLoad, BOOL bIp, ArINT& arTLoad,
	ArINT& arTpgLoad, BOOL bT, ArINT& arFrLoad, BOOL bFr, int nType, double dFactor, BOOL bAcdn, int nMsgType)
{
	int LoadCaseNo = 0, nc = 0, nnn = 0;
	CString strDesc = _T("");
	double dF = dFactor < 1.0 ? 1.4*dFactor : 1.0;
	double dWF = dFactor < 1.0 ? 1.1*dFactor : 1.0;

	BOOL bErec = FALSE;
	for (int nErec = 0; nErec < D_STCT_EREC_MAX_CVL; ++nErec)
	{
		if (m_nConstLoad != 0 && CheckCS_SgldK(D_SGLD_ER1_CVL + nErec, nnn)) bErec = TRUE;
	}

	BOOL bAddCsLoad = bErec;
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER1_CVL,nnn)) || 
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER2_CVL,nnn)) || 
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER3_CVL,nnn)) ||
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER4_CVL,nnn)) ||
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER5_CVL,nnn)) ||
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER6_CVL,nnn)) ||
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER7_CVL,nnn)) ||
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER8_CVL,nnn)) ||
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER9_CVL,nnn)) ||
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER10_CVL,nnn));// CS or ST+CS;

	if ((m_nMCrlCount || m_nCrlCount > 0) && bCrl)
	{
		T_LCOM_D LcomD = rData;
		LoadCaseNo = GetCountLcomCase(LcomD);
		int nCRL = 0;
		for (nc = 0; nc < m_nCrlCount; nc++) nCRL += Set_STLCIncludeCS(arCrlLoad.GetAt(nc), dF, LoadCaseNo, LcomD, 0);
		T_LCOM_D LcomD_tmp = Lcom_cf;
		if (bCF && bBrk)
		{
			LoadCaseNo = GetCountLcomCase(LcomD_tmp);
			for (nc = 0; nc < m_nCrlCount; nc++) Set_STLCIncludeCS(arCrlLoad.GetAt(nc), dF, LoadCaseNo, LcomD_tmp, 0);
		}
		if (nCRL > 0)
		{
			strDesc.Format(_T("+%sCRL"), GetFactorString(dF)); LcomD.Description += strDesc;
			if (bCF && bBrk) LcomD_tmp.Description += strDesc;
		}
		int nMCrlCount = (m_nMCrlCount > 0) ? m_nMCrlCount : ((nCRL > 0) ? 1 : 0);
		for (int crl = 0; crl < nMCrlCount; ++crl)
		{
			T_LCOM_D LcomD_crl = LcomD;
			T_LCOM_D LcomD_crltmp = LcomD_tmp;
			if (m_nMCrlCount > 0)
			{
				Set_MovingCase(arMCrlLoad.GetAt(crl), dF, LoadCaseNo, LcomD_crl, TRUE);
				strDesc.Format(_T("+%sMCRL"), GetFactorString(dF)); LcomD_crl.Description += strDesc; LcomD_crltmp.Description += strDesc;
				if (m_nMCrlCount > 1) { strDesc.Format(_T("[%d]"), crl + 1); LcomD_crl.Description += strDesc; LcomD_crltmp.Description += strDesc; }
			}
			if (nCRL > 0 || m_nMCrlCount > 0)
			{
				if (nType == 1) SetCvlJTJD60_ETC(LcomD_crl, LcomD_crltmp, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, bBrk, arWLoad, TRUE, arSfLoad, TRUE, arIpLoad, TRUE,
					arTLoad, arTpgLoad, TRUE, arFrLoad, TRUE, 0, dFactor, bAcdn, nMsgType);
				if (nType == 6) SetCvlJTJD60_ETC(LcomD_crl, LcomD_crltmp, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, bBrk, arWLoad, TRUE, arSfLoad, TRUE, arIpLoad, TRUE,
					arTLoad, arTpgLoad, TRUE, arFrLoad, FALSE, 2, dFactor, bAcdn, nMsgType);
				if (nType == 8) SetCvlJTJD60_ETC(LcomD_crl, LcomD_crltmp, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, bBrk, arWLoad, TRUE, arSfLoad, TRUE, arIpLoad, TRUE,
					arTLoad, arTpgLoad, FALSE, arFrLoad, FALSE, 6, dFactor, bAcdn, nMsgType);
				if (nType == 10) SetCvlJTJD60_ETC(LcomD_crl, LcomD_crltmp, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWLoad, TRUE, arSfLoad, FALSE, arIpLoad, FALSE,
					arTLoad, arTpgLoad, FALSE, arFrLoad, FALSE, 8, dFactor, bAcdn, nMsgType);
				if (nType == 101) SetCvlJTJD60_ETC(LcomD_crl, LcomD_crltmp, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, bBrk, arWLoad, FALSE, arSfLoad, FALSE, arIpLoad, FALSE,
					arTLoad, arTpgLoad, FALSE, arFrLoad, FALSE, 102, dFactor, bAcdn, nMsgType);
				if (nType == 103) SetCvlJTJD60_ETC(LcomD_crl, LcomD_crltmp, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, bBrk, arWLoad, TRUE, arSfLoad, FALSE, arIpLoad, FALSE,
					arTLoad, arTpgLoad, FALSE, arFrLoad, FALSE, 104, dFactor, bAcdn, nMsgType);
				if (nType == 0)
				{
					if (bAcdn) m_aAcdnLcomD.Add(LcomD_crl);
					else      Set_LoadComb4Civil(LcomD_crl, D_STR, 0, nMsgType);
				}
			}
		}
	}
	if (nType == 8) bBrk = FALSE;
	if (m_nBrkCount > 0 && bBrk)
	{
		for (int i = 0; i < 2; ++i)
		{
			if (i == 1 && !bCF) break;
			T_LCOM_D LcomD = (i == 0) ? rData : Lcom_cf;
			LoadCaseNo = GetCountLcomCase(LcomD);
			double dBrkF = (i == 0) ? dF * 0.7 : dF;
			int nBRK = 0;
			for (nc = 0; nc < m_nBrkCount; nc++) nBRK += Set_STLCIncludeCS(arBrkLoad.GetAt(nc), dBrkF, LoadCaseNo, LcomD, 0);
			if (nBRK > 0) { strDesc.Format(_T("+%sBRK"), GetFactorString(dBrkF)); LcomD.Description += strDesc; }
			if (nType == 1) SetCvlJTJD60_ETC(LcomD, Lcom_cf, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWLoad, TRUE, arSfLoad, FALSE, arIpLoad, FALSE,
				arTLoad, arTpgLoad, TRUE, arFrLoad, FALSE, 0, dFactor, bAcdn, nMsgType);
			if (nType == 2) SetCvlJTJD60_ETC(LcomD, Lcom_cf, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWLoad, TRUE, arSfLoad, FALSE, arIpLoad, FALSE,
				arTLoad, arTpgLoad, TRUE, arFrLoad, FALSE, 0, dFactor, bAcdn, nMsgType);
			if (nType == 6) SetCvlJTJD60_ETC(LcomD, Lcom_cf, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWLoad, TRUE, arSfLoad, FALSE, arIpLoad, FALSE,
				arTLoad, arTpgLoad, FALSE, arFrLoad, FALSE, 3, dFactor, bAcdn, nMsgType);
			if (nType == 7) SetCvlJTJD60_ETC(LcomD, Lcom_cf, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWLoad, TRUE, arSfLoad, FALSE, arIpLoad, FALSE,
				arTLoad, arTpgLoad, FALSE, arFrLoad, FALSE, 3, dFactor, bAcdn, nMsgType);
			if (nType == 8) SetCvlJTJD60_ETC(LcomD, Lcom_cf, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWLoad, TRUE, arSfLoad, FALSE, arIpLoad, FALSE,
				arTLoad, arTpgLoad, FALSE, arFrLoad, FALSE, 3, dFactor, bAcdn, nMsgType);
			if (nType == 101) SetCvlJTJD60_ETC(LcomD, Lcom_cf, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWLoad, TRUE, arSfLoad, FALSE, arIpLoad, FALSE,
				arTLoad, arTpgLoad, FALSE, arFrLoad, FALSE, 3, dFactor, bAcdn, nMsgType);
			if (nType == 102) SetCvlJTJD60_ETC(LcomD, Lcom_cf, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWLoad, TRUE, arSfLoad, FALSE, arIpLoad, FALSE,
				arTLoad, arTpgLoad, TRUE, arFrLoad, FALSE, 0, dFactor, bAcdn, nMsgType);
			if (nType == 103) SetCvlJTJD60_ETC(LcomD, Lcom_cf, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWLoad, TRUE, arSfLoad, FALSE, arIpLoad, FALSE,
				arTLoad, arTpgLoad, FALSE, arFrLoad, FALSE, 3, dFactor, bAcdn, nMsgType);
			if (nType == 104) SetCvlJTJD60_ETC(LcomD, Lcom_cf, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWLoad, TRUE, arSfLoad, FALSE, arIpLoad, FALSE,
				arTLoad, arTpgLoad, FALSE, arFrLoad, FALSE, 3, dFactor, bAcdn, nMsgType);
			if (nType == 0)
			{
				if (bAcdn) m_aAcdnLcomD.Add(LcomD);
				else      Set_LoadComb4Civil(LcomD, D_STR, 0, nMsgType);
			}
		}
	}
	if (bW)
	{
		int nWCount = m_nWCount * 2; // MNET:1614, KJH20050808
		if (m_nWCount == 0 && bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(W))) nWCount = 1;
		for (nc = 0; nc < nWCount; nc++)
		{
			T_LCOM_D LcomD = rData;
			LoadCaseNo = GetCountLcomCase(LcomD);
			int nWL = 0;
			if (m_nWCount > 0)
			{
				double dWF1 = (nc % 2 == 0) ? dWF : -1.*dWF;
				if (Set_STLCIncludeCS(arWLoad.GetAt(nc / 2), dWF1, LoadCaseNo, LcomD, 0) > 0)
				{
					CString strW = (dWF1 > 0.0) ? _T("+%sW") : _T("%sW");
					strDesc.Format(strW, GetFactorString(dWF1)); LcomD.Description += strDesc;
					if (m_nWCount > 1) { strDesc.Format(_T("[%d]"), nc / 2 + 1); LcomD.Description += strDesc; }
				}
			}
			if (bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(W)))
			{
				Set_CStageCase(m_aCSLoad.GetAt(nnn), dWF, LoadCaseNo, LcomD, TRUE);
				strDesc.Format(_T("+%s(cEL)"), GetFactorString(dWF)); LcomD.Description += strDesc;
			}
			if (nType == 1) SetCvlJTJD60_ETC(LcomD, Lcom_cf, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWLoad, FALSE, arSfLoad, TRUE, arIpLoad, TRUE,
				arTLoad, arTpgLoad, TRUE, arFrLoad, TRUE, 0, dFactor, bAcdn, nMsgType);
			if (nType == 2) SetCvlJTJD60_ETC(LcomD, Lcom_cf, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWLoad, FALSE, arSfLoad, TRUE, arIpLoad, TRUE,
				arTLoad, arTpgLoad, TRUE, arFrLoad, TRUE, 0, dFactor, bAcdn, nMsgType);
			if (nType == 3) SetCvlJTJD60_ETC(LcomD, Lcom_cf, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWLoad, FALSE, arSfLoad, FALSE, arIpLoad, FALSE,
				arTLoad, arTpgLoad, TRUE, arFrLoad, FALSE, 0, dFactor, bAcdn, nMsgType);
			if (nType == 6) SetCvlJTJD60_ETC(LcomD, Lcom_cf, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWLoad, FALSE, arSfLoad, TRUE, arIpLoad, TRUE,
				arTLoad, arTpgLoad, TRUE, arFrLoad, FALSE, 2, dFactor, bAcdn, nMsgType);
			if (nType == 7) SetCvlJTJD60_ETC(LcomD, Lcom_cf, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWLoad, FALSE, arSfLoad, TRUE, arIpLoad, TRUE,
				arTLoad, arTpgLoad, TRUE, arFrLoad, FALSE, 2, dFactor, bAcdn, nMsgType);
			if (nType == 8) SetCvlJTJD60_ETC(LcomD, Lcom_cf, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWLoad, FALSE, arSfLoad, TRUE, arIpLoad, TRUE,
				arTLoad, arTpgLoad, FALSE, arFrLoad, FALSE, 6, dFactor, bAcdn, nMsgType);
			if (nType == 9) SetCvlJTJD60_ETC(LcomD, Lcom_cf, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWLoad, FALSE, arSfLoad, TRUE, arIpLoad, TRUE,
				arTLoad, arTpgLoad, FALSE, arFrLoad, FALSE, 6, dFactor, bAcdn, nMsgType);
			if (nType == 0)
			{
				if (bAcdn) m_aAcdnLcomD.Add(LcomD);
				else      Set_LoadComb4Civil(LcomD, D_STR, 0, nMsgType);
			}
		}
	}
	if (m_nSfCount > 0 && bSf)
	{
		T_LCOM_D LcomD = rData;
		LoadCaseNo = GetCountLcomCase(LcomD);
		int nSF = 0;
		for (nc = 0; nc < m_nSfCount; nc++) nSF += Set_STLCIncludeCS(arSfLoad.GetAt(nc), dF, LoadCaseNo, LcomD, 0);
		if (nSF > 0) { strDesc.Format(_T("+%sSF"), GetFactorString(dF)); LcomD.Description += strDesc; }
		if (nType == 1) SetCvlJTJD60_ETC(LcomD, Lcom_cf, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWLoad, FALSE, arSfLoad, FALSE, arIpLoad, FALSE,
			arTLoad, arTpgLoad, TRUE, arFrLoad, TRUE, 0, dFactor, bAcdn, nMsgType);
		if (nType == 2) SetCvlJTJD60_ETC(LcomD, Lcom_cf, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWLoad, FALSE, arSfLoad, FALSE, arIpLoad, FALSE,
			arTLoad, arTpgLoad, TRUE, arFrLoad, TRUE, 0, dFactor, bAcdn, nMsgType);
		if (nType == 4) SetCvlJTJD60_ETC(LcomD, Lcom_cf, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWLoad, FALSE, arSfLoad, FALSE, arIpLoad, FALSE,
			arTLoad, arTpgLoad, TRUE, arFrLoad, TRUE, 0, dFactor, bAcdn, nMsgType);
		if (nType == 6) SetCvlJTJD60_ETC(LcomD, Lcom_cf, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWLoad, FALSE, arSfLoad, FALSE, arIpLoad, FALSE,
			arTLoad, arTpgLoad, TRUE, arFrLoad, FALSE, 2, dFactor, bAcdn, nMsgType);
		if (nType == 7) SetCvlJTJD60_ETC(LcomD, Lcom_cf, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWLoad, FALSE, arSfLoad, FALSE, arIpLoad, FALSE,
			arTLoad, arTpgLoad, TRUE, arFrLoad, FALSE, 2, dFactor, bAcdn, nMsgType);
		if (nType == 0)
		{
			if (bAcdn) m_aAcdnLcomD.Add(LcomD);
			else      Set_LoadComb4Civil(LcomD, D_STR, 0, nMsgType);
		}
	}
	if (m_nIpCount > 0 && bIp)
	{
		T_LCOM_D LcomD = rData;
		LoadCaseNo = GetCountLcomCase(LcomD);
		int nIP = 0;
		for (nc = 0; nc < m_nIpCount; nc++) nIP += Set_STLCIncludeCS(arIpLoad.GetAt(nc), dF, LoadCaseNo, LcomD, 0);
		if (nIP > 0) { strDesc.Format(_T("+%sIP"), GetFactorString(dF)); LcomD.Description += strDesc; }
		if (nType == 1) SetCvlJTJD60_ETC(LcomD, Lcom_cf, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWLoad, FALSE, arSfLoad, FALSE, arIpLoad, FALSE,
			arTLoad, arTpgLoad, TRUE, arFrLoad, TRUE, 0, dFactor, bAcdn, nMsgType);
		if (nType == 2) SetCvlJTJD60_ETC(LcomD, Lcom_cf, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWLoad, FALSE, arSfLoad, FALSE, arIpLoad, FALSE,
			arTLoad, arTpgLoad, TRUE, arFrLoad, TRUE, 0, dFactor, bAcdn, nMsgType);
		if (nType == 4) SetCvlJTJD60_ETC(LcomD, Lcom_cf, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWLoad, FALSE, arSfLoad, FALSE, arIpLoad, FALSE,
			arTLoad, arTpgLoad, TRUE, arFrLoad, TRUE, 0, dFactor, bAcdn, nMsgType);
		if (nType == 6) SetCvlJTJD60_ETC(LcomD, Lcom_cf, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWLoad, FALSE, arSfLoad, FALSE, arIpLoad, FALSE,
			arTLoad, arTpgLoad, TRUE, arFrLoad, FALSE, 2, dFactor, bAcdn, nMsgType);
		if (nType == 7) SetCvlJTJD60_ETC(LcomD, Lcom_cf, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWLoad, FALSE, arSfLoad, FALSE, arIpLoad, FALSE,
			arTLoad, arTpgLoad, TRUE, arFrLoad, FALSE, 2, dFactor, bAcdn, nMsgType);
		if (nType == 0)
		{
			if (bAcdn) m_aAcdnLcomD.Add(LcomD);
			else      Set_LoadComb4Civil(LcomD, D_STR, 0, nMsgType);
		}
	}
	if (bT && ((m_nTCount > 0 || m_nTpgCount > 0) || (bAddCsLoad && (CheckCSEL(D_SGLD_ER1_CVL, _LSX(T)) || CheckCSEL(D_SGLD_ER1_CVL, _LSX(TPG))))))
	{
		T_LCOM_D LcomD = rData;
		if (nType == 0) SetCvlJTJD60_Capacity_TPG(LcomD, arTLoad, arTpgLoad, arFrLoad, dF, dF, FALSE, bAcdn, nMsgType);
		else         SetCvlJTJD60_Capacity_TPG(LcomD, arTLoad, arTpgLoad, arFrLoad, dF, dF, TRUE, bAcdn, nMsgType);
	}
	if (m_nFrCount > 0 && bFr)
	{
		T_LCOM_D LcomD = rData;
		LoadCaseNo = GetCountLcomCase(LcomD);
		int nFR = 0;
		for (nc = 0; nc < m_nFrCount; nc++) nFR += Set_STLCIncludeCS(arFrLoad.GetAt(nc), dF, LoadCaseNo, LcomD, 0);
		if (nFR > 0)
		{
			strDesc.Format(_T("+%sFR"), GetFactorString(dF)); LcomD.Description += strDesc;
			if (nType == 0)
			{
				if (bAcdn) m_aAcdnLcomD.Add(LcomD);
				else      Set_LoadComb4Civil(LcomD, D_STR, 0, nMsgType);
			}
		}
	}
	return TRUE;
}
int CLoadCombCtrl::GetCountLcomCase(T_LCOM_D& rData)
{
	return rData.aCombination.GetSize();
	//   for(int i=0; i<D_LCOM_NUMCOMB; i++)
	//   {
	//     if(rData.Combination[i].LoadCaseKey == 0) return i;
	//   }
	//   ASSERT(0); // MNET:XXXX 20070618 mylee ¸øÃ£Àº °æ¿ì
	//   return D_LCOM_NUMCOMB;
}
BOOL CLoadCombCtrl::SetCvlJTJD60_Capacity_M(const CArray<T_LCOM_D, T_LCOM_D&>& arData, ArUNT& arMLoad,
	ArINT& arLLoad, ArINT& arIlLoad, ArINT& arCrlLoad, ArINT& arMCrlLoad,
	ArINT& arCfLoad, ArINT& arLsLoad, ArINT& arWLoad, ArINT& arSfLoad, ArINT& arIpLoad,
	ArINT& arTLoad, ArINT& arTpgLoad, ArINT& arFrLoad, ArINT& arBrkLoad, int nType)
{
	// nType == 0,4: °è¼öÁ¶ÇÕ, 1: 1.0(settlement Á¦¿Ü), 2: 1.0(¸ðµÎ), Live Load Á¦¿Ü
	CArray<T_LCOM_D, T_LCOM_D&> aLcomD; aLcomD.Copy(arData);
	int  nLcomSize = aLcomD.GetSize();
	int  nnn = 0;

	BOOL bErec = FALSE;
	for (int nErec = 0; nErec < D_STCT_EREC_MAX_CVL; ++nErec)
	{
		if (m_nConstLoad != 0 && CheckCS_SgldK(D_SGLD_ER1_CVL + nErec, nnn)) bErec = TRUE;
	}

	BOOL bAddCsLoad = bErec;
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER1_CVL,nnn)) || 
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER2_CVL,nnn)) || 
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER3_CVL,nnn)) ||
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER4_CVL,nnn)) ||
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER5_CVL,nnn)) ||
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER6_CVL,nnn)) ||
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER7_CVL,nnn)) ||
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER8_CVL,nnn)) ||
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER9_CVL,nnn)) ||
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER10_CVL,nnn));// CS or ST+CS;

		// ¿µ±¸ÇÏÁß¸¸ Á¶ÇÕ
	for (int lc = 0; lc < nLcomSize; lc++)
	{
		T_LCOM_D LcomD = aLcomD.GetAt(lc);
		LcomD.nActive = 1; // Active or Strength/Stress
		if (nType == 2 || nType == 4) m_aAcdnLcomD.Add(LcomD);
		else                     Set_LoadComb4Civil(LcomD, D_STR, 0, 101); // 101:±âº»Á¶ÇÕ(¿µ±¸ÇÏÁß)
	}

	double dFactor = (nType == 0 || nType == 4) ? 1.4 : 1.0;
	double dWFactor = (nType == 0 || nType == 4) ? 1.1 : 1.0;

	int nMsgType = (nType == 0 || nType == 1) ? 103 : 104; // 103:±âº»Á¶ÇÕ, 104:¿ì¿¬Á¶ÇÕ
	// ¿µ±¸ÇÏÁß + 1°³ÀÇ °¡º¯ÇÏÁßÀÎ °æ¿ì
	CString strDesc = _T("");
	int LoadCaseNo = 0, nc = 0;
	for (int lc = 0; lc < nLcomSize; lc++)
	{
		int nMCount = max(m_nMCount, 1);
		for (int m = 0; m < nMCount; m++)
		{
			T_LCOM_D LcomD = aLcomD.GetAt(lc); LcomD.nActive = 1; // Active or Strength/Stress
			LoadCaseNo = GetCountLcomCase(LcomD);

			if (nType != 2) // live load
			{
				BOOL bAddLcomD = FALSE;
				// moving load
				if (m_nMCount > 0)
				{
					Set_MovingCase(arMLoad.GetAt(m), dFactor, LoadCaseNo, LcomD, TRUE);
					strDesc.Format(_T("+%sM"), GetFactorString(dFactor)); LcomD.Description += strDesc;
					if (m_nMCount > 1) { strDesc.Format(_T("[%d]"), m + 1); LcomD.Description += strDesc; }
					bAddLcomD = TRUE;
				}
				int nLL = 0, nIL = 0, nCF = 0;
				if (m_nLCount > 0 || (bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(L)))) // ST or ST+CS
				{
					for (nc = 0; nc < m_nLCount; nc++) nLL += Set_STLCIncludeCS(arLLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD, 0);
					if (nLL > 0) { strDesc.Format(_T("+%sL"), GetFactorString(dFactor)); LcomD.Description += strDesc; }
					if (bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(L)))
					{
						Set_CStageCase(m_aCSLoad.GetAt(nnn), dFactor, LoadCaseNo, LcomD, TRUE);
						strDesc.Format(_T("+%s(cEL)"), GetFactorString(dFactor)); LcomD.Description += strDesc;
						bAddLcomD = TRUE;
					}
					for (nc = 0; nc < m_nIlCount; nc++) nIL += Set_STLCIncludeCS(arIlLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD, 0);
					if (nIL > 0) { strDesc.Format(_T("+%sIL"), GetFactorString(dFactor)); LcomD.Description += strDesc; }
					if (nLL + nIL > 0) bAddLcomD = TRUE;
				}
				if (m_nMCount > 0 || (m_nLCount > 0 || (bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(L)))))
				{
					for (nc = 0; nc < m_nCfCount; nc++) nCF += Set_STLCIncludeCS(arCfLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD, 0);
					if (nCF > 0) { strDesc.Format(_T("+%sCF"), GetFactorString(dFactor)); LcomD.Description += strDesc; }
					bAddLcomD = TRUE;
				}
				if (bAddLcomD)
				{
					if (nType == 4) m_aAcdnLcomD.Add(LcomD);
					else         Set_LoadComb4Civil(LcomD, D_STR, 0, nMsgType);
				}
			}
			if (m_nLsCount > 0) // Live Load Surcharge
			{
				LcomD = aLcomD.GetAt(lc); LcomD.nActive = 1; LoadCaseNo = GetCountLcomCase(LcomD);
				int nLS = 0;
				for (nc = 0; nc < m_nLsCount; nc++) nLS += Set_STLCIncludeCS(arLsLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD, 0);
				if (nLS > 0)
				{
					strDesc.Format(_T("+%sLS"), GetFactorString(dFactor)); LcomD.Description += strDesc;
					if (nType == 2 || nType == 4) m_aAcdnLcomD.Add(LcomD);
					else                     Set_LoadComb4Civil(LcomD, D_STR, 0, nMsgType);
				}
			}// MNET:XXXX-BJLEE(BBONG)-20060502
			if (nType != 2 && (m_nCrlCount > 0 || m_nMCrlCount > 0)) // Crowd Load
			{
				LcomD = aLcomD.GetAt(lc); LcomD.nActive = 1; LoadCaseNo = GetCountLcomCase(LcomD);
				int nCRL = 0;
				for (nc = 0; nc < m_nCrlCount; nc++) nCRL += Set_STLCIncludeCS(arCrlLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD, 0);
				if (nCRL > 0) { strDesc.Format(_T("+%sCRL"), GetFactorString(dFactor)); LcomD.Description += strDesc; }

				int nMCrlCount = (m_nMCrlCount > 0) ? m_nMCrlCount : ((nCRL > 0) ? 1 : 0);
				for (int crl = 0; crl < nMCrlCount; ++crl)
				{
					T_LCOM_D LcomD_crl = LcomD; LoadCaseNo = GetCountLcomCase(LcomD_crl);
					if (m_nMCrlCount > 0)
					{
						Set_MovingCase(arMCrlLoad.GetAt(crl), dFactor, LoadCaseNo, LcomD_crl, TRUE);
						strDesc.Format(_T("+%sMCRL"), GetFactorString(dFactor)); LcomD_crl.Description += strDesc;
						if (m_nMCrlCount > 1) { strDesc.Format(_T("[%d]"), crl + 1); LcomD_crl.Description += strDesc; }
					}
					if (nCRL > 0 || m_nMCrlCount > 0)
					{
						if (nType == 2 || nType == 4) m_aAcdnLcomD.Add(LcomD_crl);
						else                     Set_LoadComb4Civil(LcomD_crl, D_STR, 0, nMsgType);
					}
				}
			}
			int nWCount = m_nWCount * 2; // MNET:1614, KJH20050808
			if (m_nWCount == 0 && bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(W))) nWCount = 1;
			for (int nc = 0; nc < nWCount; nc++)
			{
				LcomD = aLcomD.GetAt(lc); LcomD.nActive = 1; LoadCaseNo = GetCountLcomCase(LcomD);
				if (m_nWCount > 0)
				{
					double dWF = (nc % 2 == 0) ? dWFactor : -1.*dWFactor;
					if (Set_STLCIncludeCS(arWLoad.GetAt(nc / 2), dWF, LoadCaseNo, LcomD, 0) > 0)
					{
						CString strW = (dWF > 0.0) ? _T("+%sW") : _T("%sW");
						strDesc.Format(strW, GetFactorString(dWF)); LcomD.Description += strDesc;
						if (m_nWCount > 1) { strDesc.Format(_T("[%d]"), nc / 2 + 1); LcomD.Description += strDesc; }
					}
				}
				if (bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(W)))
				{
					Set_CStageCase(m_aCSLoad.GetAt(nnn), dWFactor, LoadCaseNo, LcomD, TRUE);
					strDesc.Format(_T("+%s(cEL)"), GetFactorString(dWFactor)); LcomD.Description += strDesc;
				}
				if (nType == 2 || nType == 4) m_aAcdnLcomD.Add(LcomD);
				else                     Set_LoadComb4Civil(LcomD, D_STR, 0, nMsgType);
			}
			if (m_nSfCount > 0) // Stream Flow Pressure
			{
				LcomD = aLcomD.GetAt(lc); LcomD.nActive = 1; LoadCaseNo = GetCountLcomCase(LcomD);
				int nSF = 0;
				for (nc = 0; nc < m_nSfCount; nc++) nSF += Set_STLCIncludeCS(arSfLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD, 0);
				if (nSF > 0)
				{
					strDesc.Format(_T("+%sSF"), GetFactorString(dFactor)); LcomD.Description += strDesc;
					if (nType == 2 || nType == 4) m_aAcdnLcomD.Add(LcomD);
					else                     Set_LoadComb4Civil(LcomD, D_STR, 0, nMsgType);
				}
			}
			if (m_nIpCount > 0) // Ice Pressure
			{
				LcomD = aLcomD.GetAt(lc); LcomD.nActive = 1; LoadCaseNo = GetCountLcomCase(LcomD);
				int nIP = 0;
				for (nc = 0; nc < m_nIpCount; nc++) nIP += Set_STLCIncludeCS(arIpLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD, 0);
				if (nIP > 0)
				{
					strDesc.Format(_T("+%sIP"), GetFactorString(dFactor)); LcomD.Description += strDesc;
					if (nType == 2 || nType == 4) m_aAcdnLcomD.Add(LcomD);
					else                     Set_LoadComb4Civil(LcomD, D_STR, 0, nMsgType);
				}
			}
			// Temeperature
			if ((m_nTCount > 0 || m_nTpgCount > 0) || (bAddCsLoad && (CheckCSEL(D_SGLD_ER1_CVL, _LSX(T)) || CheckCSEL(D_SGLD_ER1_CVL, _LSX(TPG)))))
			{
				LcomD = aLcomD.GetAt(lc); LcomD.nActive = 1;
				SetCvlJTJD60_Capacity_TPG(LcomD, arTLoad, arTpgLoad, arFrLoad, dFactor, dFactor, FALSE, nType == 2 || nType == 4, nMsgType);
			}
			if (m_nFrCount > 0) // Friction
			{
				LcomD = aLcomD.GetAt(lc); LcomD.nActive = 1; LoadCaseNo = GetCountLcomCase(LcomD);
				int nFR = 0;
				for (nc = 0; nc < m_nFrCount; nc++) nFR += Set_STLCIncludeCS(arFrLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD, 0);
				if (nFR > 0)
				{
					strDesc.Format(_T("+%sFR"), GetFactorString(dFactor)); LcomD.Description += strDesc;
					if (nType == 2 || nType == 4) m_aAcdnLcomD.Add(LcomD);
					else                     Set_LoadComb4Civil(LcomD, D_STR, 0, nMsgType);
				}
			}
		} // next m
	} // next lc

	// ¿µ±¸ÇÏÁß + Â÷·®ÇÏÁß + 1°³ÀÇ ±âÅ¸ °¡º¯ÇÏÁßÀÎ °æ¿ì(8°³)
	SetCvlJTJD60_Var1_M(arData, arMLoad, arLLoad, arIlLoad, arCrlLoad, arMCrlLoad, arCfLoad, arLsLoad, arWLoad, arSfLoad, arIpLoad,
		arTLoad, arTpgLoad, arFrLoad, arBrkLoad, nType, nMsgType);
	// ¿µ±¸ÇÏÁß + Â÷·®ÇÏÁß + 2°³ÀÇ ±âÅ¸ °¡º¯ÇÏÁßÀÎ °æ¿ì(24°³)
	SetCvlJTJD60_Var2_M(arData, arMLoad, arLLoad, arIlLoad, arCrlLoad, arMCrlLoad, arCfLoad, arLsLoad, arWLoad, arSfLoad, arIpLoad,
		arTLoad, arTpgLoad, arFrLoad, arBrkLoad, nType, nMsgType);
	// ¿µ±¸ÇÏÁß + Â÷·®ÇÏÁß + 3°³ÀÇ ±âÅ¸ °¡º¯ÇÏÁßÀÎ °æ¿ì(36°³)
	SetCvlJTJD60_Var3_M(arData, arMLoad, arLLoad, arIlLoad, arCrlLoad, arMCrlLoad, arCfLoad, arLsLoad, arWLoad, arSfLoad, arIpLoad,
		arTLoad, arTpgLoad, arFrLoad, arBrkLoad, nType, nMsgType);
	// ¿µ±¸ÇÏÁß + Â÷·®ÇÏÁß + 4°³ÀÇ ±âÅ¸ °¡º¯ÇÏÁßÀÎ °æ¿ì(29°³)
	SetCvlJTJD60_Var4_M(arData, arMLoad, arLLoad, arIlLoad, arCrlLoad, arMCrlLoad, arCfLoad, arLsLoad, arWLoad, arSfLoad, arIpLoad,
		arTLoad, arTpgLoad, arFrLoad, arBrkLoad, nType, nMsgType);
	// ¿µ±¸ÇÏÁß + Â÷·®ÇÏÁß + 5°³ ¹× 6°³ÀÇ ±âÅ¸ °¡º¯ÇÏÁßÀÎ °æ¿ì(12°³ ¹× 2°³)
	SetCvlJTJD60_Var56_M(arData, arMLoad, arLLoad, arIlLoad, arCrlLoad, arMCrlLoad, arCfLoad, arLsLoad, arWLoad, arSfLoad, arIpLoad,
		arTLoad, arTpgLoad, arFrLoad, arBrkLoad, nType, nMsgType);

	return TRUE;
}
BOOL CLoadCombCtrl::SetCvlJTJD60_Var1_M(const CArray<T_LCOM_D, T_LCOM_D&>& arData, ArUNT& arMLoad,
	ArINT& arLLoad, ArINT& arIlLoad, ArINT& arCrlLoad, ArINT& arMCrlLoad,
	ArINT& arCfLoad, ArINT& arLsLoad, ArINT& arWLoad, ArINT& arSfLoad, ArINT& arIpLoad,
	ArINT& arTLoad, ArINT& arTpgLoad, ArINT& arFrLoad, ArINT& arBrkLoad, int nType, int nMsgType)
{
	// nType == 0,4: °è¼öÁ¶ÇÕ, 1: 1.0(settlement Á¦¿Ü), 2: 1.0(¸ðµÎ), Live Load Á¦¿Ü
	CArray<T_LCOM_D, T_LCOM_D&> aLcomD; aLcomD.Copy(arData);
	int  nLcomSize = aLcomD.GetSize();
	int  nnn = 0;

	BOOL bErec = FALSE;
	for (int nErec = 0; nErec < D_STCT_EREC_MAX_CVL; ++nErec)
	{
		if (m_nConstLoad != 0 && CheckCS_SgldK(D_SGLD_ER1_CVL + nErec, nnn)) bErec = TRUE;
	}

	BOOL bAddCsLoad = bErec;
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER1_CVL,nnn)) || 
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER2_CVL,nnn)) || 
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER3_CVL,nnn)) ||
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER4_CVL,nnn)) ||
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER5_CVL,nnn)) ||
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER6_CVL,nnn)) ||
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER7_CVL,nnn)) ||
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER8_CVL,nnn)) ||
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER9_CVL,nnn)) ||
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER10_CVL,nnn));// CS or ST+CS;

	double dLFactor = (nType == 0 || nType == 4) ? 1.4 : 1.0;
	double dFactor = (nType == 0 || nType == 4) ? 1.4*0.8 : 1.0;
	double dWFactor = (nType == 0 || nType == 4) ? 1.1*0.8 : 1.0;

	// ¿µ±¸ÇÏÁß + Â÷·®ÇÏÁß + 1°³ÀÇ °¡º¯ÇÏÁßÀÎ °æ¿ì
	CString strDesc = _T("");
	int LoadCaseNo = 0, nc = 0;
	for (int lc = 0; lc < nLcomSize; lc++)
	{
		int nMCount = max(m_nMCount, 1);
		for (int m = 0; m < nMCount; m++)
		{
			T_LCOM_D LcomD = aLcomD.GetAt(lc); LcomD.nActive = 1; // Active or Strength/Stress
			T_LCOM_D LcomD_cf = LcomD; // MNET:1614, KJH20050809, cf°¡ Á¦¿ÜµÈ load combination
			LoadCaseNo = GetCountLcomCase(LcomD);

			BOOL bLive = FALSE, bCF = FALSE;
			if (nType != 2)
			{
				BOOL bAddLcomD = FALSE;
				// moving load
				if (m_nMCount > 0)
				{
					Set_MovingCase(arMLoad.GetAt(m), dLFactor, LoadCaseNo, LcomD, TRUE);
					strDesc.Format(_T("+%sM"), GetFactorString(dLFactor)); LcomD.Description += strDesc;
					if (m_nMCount > 1) { strDesc.Format(_T("[%d]"), m + 1); LcomD.Description += strDesc; }
					bAddLcomD = TRUE;
				}
				int nLL = 0, nIL = 0, nCF = 0;
				if (m_nLCount > 0 || (bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(L)))) // ST or ST+CS
				{
					for (nc = 0; nc < m_nLCount; nc++) nLL += Set_STLCIncludeCS(arLLoad.GetAt(nc), dLFactor, LoadCaseNo, LcomD, 0);
					if (nLL > 0) { strDesc.Format(_T("+%sL"), GetFactorString(dLFactor)); LcomD.Description += strDesc; }
					if (bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(L)))
					{
						Set_CStageCase(m_aCSLoad.GetAt(nnn), dLFactor, LoadCaseNo, LcomD, TRUE);
						strDesc.Format(_T("+%s(cEL)"), GetFactorString(dLFactor)); LcomD.Description += strDesc;
						bAddLcomD = TRUE;
					}
					for (nc = 0; nc < m_nIlCount; nc++) nIL += Set_STLCIncludeCS(arIlLoad.GetAt(nc), dLFactor, LoadCaseNo, LcomD, 0);
					if (nIL > 0) { strDesc.Format(_T("+%sIL"), GetFactorString(dLFactor)); LcomD.Description += strDesc; }
					if (nLL + nIL > 0) bAddLcomD = TRUE;
				}
				bLive = m_nMCount > 0 || (m_nLCount > 0 || (bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(L))));
				if (bLive)
				{
					LcomD_cf = LcomD;
					for (nc = 0; nc < m_nCfCount; nc++) nCF += Set_STLCIncludeCS(arCfLoad.GetAt(nc), dLFactor, LoadCaseNo, LcomD, 0);
					if (nCF > 0) { strDesc.Format(_T("+%sCF"), GetFactorString(dLFactor)); LcomD.Description += strDesc; bCF = TRUE; }
					bAddLcomD = TRUE;
				}
				if (bAddLcomD)
				{
					if (nType == 4) m_aAcdnLcomD.Add(LcomD);
					else         Set_LoadComb4Civil(LcomD, D_STR, 0, nMsgType);
				}
			}

			if (m_nLsCount > 0) // Live Load Surcharge
			{
				T_LCOM_D LcomD_l = LcomD; LoadCaseNo = GetCountLcomCase(LcomD_l);
				int nLS = 0;
				for (nc = 0; nc < m_nLsCount; nc++) nLS += Set_STLCIncludeCS(arLsLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD_l, 0);
				if (nLS > 0)
				{
					strDesc.Format(_T("+%sLS"), GetFactorString(dFactor)); LcomD_l.Description += strDesc;
					if (nType == 2 || nType == 4) m_aAcdnLcomD.Add(LcomD_l);
					else                     Set_LoadComb4Civil(LcomD_l, D_STR, 0, nMsgType);
				}
			}
			// MNET:XXXX-BJLEE(BBONG)-20060502
			if (nType != 2 && (m_nCrlCount > 0 || m_nMCrlCount > 0)) // Crowd Load
			{
				T_LCOM_D LcomD_c = LcomD;
				LoadCaseNo = GetCountLcomCase(LcomD_c);
				int nCRL = 0;
				for (nc = 0; nc < m_nCrlCount; nc++) nCRL += Set_STLCIncludeCS(arCrlLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD_c, 0);
				if (nCRL > 0) { strDesc.Format(_T("+%sCRL"), GetFactorString(dFactor)); LcomD_c.Description += strDesc; }

				int nMCrlCount = (m_nMCrlCount > 0) ? m_nMCrlCount : ((nCRL > 0) ? 1 : 0);
				for (int crl = 0; crl < nMCrlCount; ++crl)
				{
					T_LCOM_D LcomD_crl = LcomD_c; LoadCaseNo = GetCountLcomCase(LcomD_crl);
					if (m_nMCrlCount > 0)
					{
						Set_MovingCase(arMCrlLoad.GetAt(crl), dFactor, LoadCaseNo, LcomD_crl, TRUE);
						strDesc.Format(_T("+%sMCRL"), GetFactorString(dFactor)); LcomD_crl.Description += strDesc;
						if (m_nMCrlCount > 1) { strDesc.Format(_T("[%d]"), crl + 1); LcomD_crl.Description += strDesc; }
					}
					if (nCRL > 0 || m_nMCrlCount > 0)
					{
						if (nType == 2 || nType == 4) m_aAcdnLcomD.Add(LcomD_crl);
						else                     Set_LoadComb4Civil(LcomD_crl, D_STR, 0, nMsgType);
					}
				}
			}

			if (m_nBrkCount > 0 && bLive) // Brake Load
			{
				T_LCOM_D LcomD_l = LcomD; LoadCaseNo = GetCountLcomCase(LcomD_l);
				double dBrkF = (nType == 0 || nType == 4) ? (bCF ? dFactor * 0.7 : dFactor) : 1.0;
				int nBRK = 0;
				for (nc = 0; nc < m_nBrkCount; nc++) nBRK += Set_STLCIncludeCS(arBrkLoad.GetAt(nc), dBrkF, LoadCaseNo, LcomD_l, 0);
				if (nBRK > 0)
				{
					strDesc.Format(_T("+%sBRK"), GetFactorString(dBrkF)); LcomD_l.Description += strDesc;
					if (nType == 2 || nType == 4) m_aAcdnLcomD.Add(LcomD_l);
					else                     Set_LoadComb4Civil(LcomD_l, D_STR, 0, nMsgType);
				}
				if (bCF)
				{
					LcomD_l = LcomD_cf; LoadCaseNo = GetCountLcomCase(LcomD_l);
					dBrkF = (nType == 0 || nType == 4) ? dFactor : 1.0;
					for (nc = 0; nc < m_nBrkCount; nc++) nBRK += Set_STLCIncludeCS(arBrkLoad.GetAt(nc), dBrkF, LoadCaseNo, LcomD_l, 0);
					if (nBRK > 0)
					{
						strDesc.Format(_T("+%sBRK"), GetFactorString(dBrkF)); LcomD_l.Description += strDesc;
						if (nType == 2 || nType == 4) m_aAcdnLcomD.Add(LcomD_l);
						else                     Set_LoadComb4Civil(LcomD_l, D_STR, 0, nMsgType);
					}
				}
			}
			int nWCount = m_nWCount * 2; // MNET:1614, KJH20050808
			if (m_nWCount == 0 && bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(W))) nWCount = 1;
			for (nc = 0; nc < nWCount; nc++)
			{
				T_LCOM_D LcomD_l = LcomD; LoadCaseNo = GetCountLcomCase(LcomD_l);
				if (m_nWCount > 0)
				{
					double dWF = (nc % 2 == 0) ? dWFactor : -1.*dWFactor;
					if (Set_STLCIncludeCS(arWLoad.GetAt(nc / 2), dWF, LoadCaseNo, LcomD_l, 0) > 0)
					{
						CString strW = (dWF > 0.0) ? _T("+%sW") : _T("%sW");
						strDesc.Format(strW, GetFactorString(dWF)); LcomD_l.Description += strDesc;
						if (m_nWCount > 1) { strDesc.Format(_T("[%d]"), nc / 2 + 1); LcomD_l.Description += strDesc; }
					}
				}
				if (bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(W)))
				{
					Set_CStageCase(m_aCSLoad.GetAt(nnn), dWFactor, LoadCaseNo, LcomD_l, TRUE);
					strDesc.Format(_T("+%s(cEL)"), GetFactorString(dWFactor)); LcomD_l.Description += strDesc;
				}
				if (nType == 2 || nType == 4) m_aAcdnLcomD.Add(LcomD_l);
				else                     Set_LoadComb4Civil(LcomD_l, D_STR, 0, nMsgType);
			}
			if (m_nSfCount > 0) // Stream Flow Pressure
			{
				T_LCOM_D LcomD_l = LcomD; LoadCaseNo = GetCountLcomCase(LcomD_l);
				int nSF = 0;
				for (nc = 0; nc < m_nSfCount; nc++) nSF += Set_STLCIncludeCS(arSfLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD_l, 0);
				if (nSF > 0)
				{
					strDesc.Format(_T("+%sSF"), GetFactorString(dFactor)); LcomD_l.Description += strDesc;
					if (nType == 2 || nType == 4) m_aAcdnLcomD.Add(LcomD_l);
					else                     Set_LoadComb4Civil(LcomD_l, D_STR, 0, nMsgType);
				}
			}
			if (m_nIpCount > 0) // Ice Pressure
			{
				T_LCOM_D LcomD_l = LcomD; LoadCaseNo = GetCountLcomCase(LcomD_l);
				int nIP = 0;
				for (nc = 0; nc < m_nIpCount; nc++) nIP += Set_STLCIncludeCS(arIpLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD_l, 0);
				if (nIP > 0)
				{
					strDesc.Format(_T("+%sIP"), GetFactorString(dFactor)); LcomD_l.Description += strDesc;
					if (nType == 2 || nType == 4) m_aAcdnLcomD.Add(LcomD_l);
					else                     Set_LoadComb4Civil(LcomD_l, D_STR, 0, nMsgType);
				}
			}
			// Temeperature
			if ((m_nTCount > 0 || m_nTpgCount > 0) || (bAddCsLoad && (CheckCSEL(D_SGLD_ER1_CVL, _LSX(T)) || CheckCSEL(D_SGLD_ER1_CVL, _LSX(TPG)))))
			{
				T_LCOM_D LcomD_l = LcomD;
				SetCvlJTJD60_Capacity_TPG(LcomD_l, arTLoad, arTpgLoad, arFrLoad, dFactor, dFactor, FALSE, nType == 2 || nType == 4, nMsgType);
			}
			if (m_nFrCount > 0) // Friction
			{
				T_LCOM_D LcomD_l = LcomD; LoadCaseNo = GetCountLcomCase(LcomD_l);
				int nFR = 0;
				for (nc = 0; nc < m_nFrCount; nc++) nFR += Set_STLCIncludeCS(arFrLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD_l, 0);
				if (nFR > 0)
				{
					strDesc.Format(_T("+%sFR"), GetFactorString(dFactor)); LcomD_l.Description += strDesc;
					if (nType == 2 || nType == 4) m_aAcdnLcomD.Add(LcomD_l);
					else                     Set_LoadComb4Civil(LcomD_l, D_STR, 0, nMsgType);
				}
			}
		} // next m
	} // next lc

	return TRUE;
}
BOOL CLoadCombCtrl::SetCvlJTJD60_Var2_M(const CArray<T_LCOM_D, T_LCOM_D&>& arData, ArUNT& arMLoad,
	ArINT& arLLoad, ArINT& arIlLoad, ArINT& arCrlLoad, ArINT& arMCrlLoad,
	ArINT& arCfLoad, ArINT& arLsLoad, ArINT& arWLoad, ArINT& arSfLoad, ArINT& arIpLoad,
	ArINT& arTLoad, ArINT& arTpgLoad, ArINT& arFrLoad, ArINT& arBrkLoad, int nType, int nMsgType)
{
	// nType == 0,4: °è¼öÁ¶ÇÕ, 1: 1.0(settlement Á¦¿Ü), 2: 1.0(¸ðµÎ), Live Load Á¦¿Ü
	CArray<T_LCOM_D, T_LCOM_D&> aLcomD; aLcomD.Copy(arData);
	int  nLcomSize = aLcomD.GetSize();
	int  nnn = 0;

	BOOL bErec = FALSE;
	for (int nErec = 0; nErec < D_STCT_EREC_MAX_CVL; ++nErec)
	{
		if (m_nConstLoad != 0 && CheckCS_SgldK(D_SGLD_ER1_CVL + nErec, nnn)) bErec = TRUE;
	}

	BOOL bAddCsLoad = bErec;
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER1_CVL,nnn)) || 
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER2_CVL,nnn)) || 
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER3_CVL,nnn)) ||
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER4_CVL,nnn)) ||
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER5_CVL,nnn)) ||
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER6_CVL,nnn)) ||
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER7_CVL,nnn)) ||
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER8_CVL,nnn)) ||
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER9_CVL,nnn)) ||
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER10_CVL,nnn));// CS or ST+CS;

	double dLFactor = (nType == 0 || nType == 4) ? 1.4 : 1.0;
	double dFactor = (nType == 0 || nType == 4) ? 1.4*0.7 : 1.0;
	double dWFactor = (nType == 0 || nType == 4) ? 1.1*0.7 : 1.0;
	double dF = (nType == 0 || nType == 4) ? 0.7 : 1.0;

	ArINT arTmp;

	// ¿µ±¸ÇÏÁß + Â÷·®ÇÏÁß + 2°³ÀÇ °¡º¯ÇÏÁßÀÎ °æ¿ì
	CString strDesc = _T("");
	int LoadCaseNo = 0, nc = 0;
	for (int lc = 0; lc < nLcomSize; lc++)
	{
		int nMCount = max(m_nMCount, 1);
		for (int m = 0; m < nMCount; m++)
		{
			T_LCOM_D LcomD = aLcomD.GetAt(lc); LcomD.nActive = 1; // Active or Strength/Stress
			T_LCOM_D LcomD_cf = LcomD; // MNET:1614, KJH20050809, CF°¡ Á¦¿ÜµÈ load combination
			LoadCaseNo = GetCountLcomCase(LcomD);

			BOOL bLive = FALSE, bCF = FALSE;
			if (nType != 2)
			{
				// moving load
				if (m_nMCount > 0)
				{
					Set_MovingCase(arMLoad.GetAt(m), dLFactor, LoadCaseNo, LcomD, TRUE);
					strDesc.Format(_T("+%sM"), GetFactorString(dLFactor)); LcomD.Description += strDesc;
					if (m_nMCount > 1) { strDesc.Format(_T("[%d]"), m + 1); LcomD.Description += strDesc; }
				}
				int nLL = 0, nIL = 0, nCF = 0;
				if (m_nLCount > 0 || (bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(L)))) // ST or ST+CS
				{
					for (nc = 0; nc < m_nLCount; nc++) nLL += Set_STLCIncludeCS(arLLoad.GetAt(nc), dLFactor, LoadCaseNo, LcomD, 0);
					if (nLL > 0) { strDesc.Format(_T("+%sL"), GetFactorString(dLFactor)); LcomD.Description += strDesc; }
					if (bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(L)))
					{
						Set_CStageCase(m_aCSLoad.GetAt(nnn), dLFactor, LoadCaseNo, LcomD, TRUE);
						strDesc.Format(_T("+%s(cEL)"), GetFactorString(dLFactor)); LcomD.Description += strDesc;
					}
					for (nc = 0; nc < m_nIlCount; nc++) nIL += Set_STLCIncludeCS(arIlLoad.GetAt(nc), dLFactor, LoadCaseNo, LcomD, 0);
					if (nIL > 0) { strDesc.Format(_T("+%sIL"), GetFactorString(dLFactor)); LcomD.Description += strDesc; }
				}
				bLive = m_nMCount > 0 || (m_nLCount > 0 || (bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(L))));
				if (bLive)
				{
					LcomD_cf = LcomD;
					for (nc = 0; nc < m_nCfCount; nc++) nCF += Set_STLCIncludeCS(arCfLoad.GetAt(nc), dLFactor, LoadCaseNo, LcomD, 0);
					if (nCF > 0) { strDesc.Format(_T("+%sCF"), GetFactorString(dLFactor)); LcomD.Description += strDesc; bCF = TRUE; }
				}
			}

			if (m_nLsCount > 0) // Live Load Surcharge
			{
				T_LCOM_D LcomD_l = LcomD; LoadCaseNo = GetCountLcomCase(LcomD_l);
				int nLS = 0;
				for (nc = 0; nc < m_nLsCount; nc++) nLS += Set_STLCIncludeCS(arLsLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD_l, 0);
				T_LCOM_D LcomD_tmp = LcomD_cf;
				if (bCF)
				{
					LoadCaseNo = GetCountLcomCase(LcomD_tmp);
					for (nc = 0; nc < m_nLsCount; nc++) Set_STLCIncludeCS(arLsLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD_tmp, 0);
				}
				if (nLS > 0)
				{
					strDesc.Format(_T("+%sLS"), GetFactorString(dFactor)); LcomD_l.Description += strDesc;
					if (bCF) LcomD_tmp.Description += strDesc;
					SetCvlJTJD60_ETC(LcomD_l, LcomD_tmp, bCF, arCrlLoad, arMCrlLoad, nType != 2, arBrkLoad, bLive, arWLoad, TRUE, arSfLoad, TRUE,
						arIpLoad, TRUE, arTLoad, arTpgLoad, TRUE, arFrLoad, TRUE, 0, dF, nType == 2 || nType == 4, nMsgType);
				}
			}
			if (nType != 2 && (m_nCrlCount > 0 || m_nMCrlCount > 0)) // Crowd Load
			{
				T_LCOM_D LcomD_l = LcomD; LoadCaseNo = GetCountLcomCase(LcomD_l);
				int nCRL = 0;
				for (nc = 0; nc < m_nCrlCount; nc++) nCRL += Set_STLCIncludeCS(arCrlLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD_l, 0);
				T_LCOM_D LcomD_tmp = LcomD_cf;
				if (bCF)
				{
					LoadCaseNo = GetCountLcomCase(LcomD_tmp);
					for (nc = 0; nc < m_nCrlCount; nc++) Set_STLCIncludeCS(arCrlLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD_tmp, 0);
				}
				// MNET:XXXX-BJLEE(BBONG)-20060502 
				if (nCRL > 0 || m_nMCrlCount > 0)
				{
					if (nCRL > 0)
					{
						strDesc.Format(_T("+%sCRL"), GetFactorString(dFactor)); LcomD_l.Description += strDesc;
						if (bCF) LcomD_tmp.Description += strDesc;
					}

					int nMCrlCount = (m_nMCrlCount > 0) ? m_nMCrlCount : ((nCRL > 0) ? 1 : 0);
					T_LCOM_D LcomD_tmp2;
					for (int crl = 0; crl < nMCrlCount; ++crl)
					{
						T_LCOM_D LcomD_crl = LcomD_l; LoadCaseNo = GetCountLcomCase(LcomD_crl);
						LcomD_tmp2.Initialize();
						if (m_nMCrlCount > 0)
						{
							Set_MovingCase(arMCrlLoad.GetAt(crl), dFactor, LoadCaseNo, LcomD_crl, TRUE);
							strDesc.Format(_T("+%sMCRL"), GetFactorString(dFactor)); LcomD_crl.Description += strDesc;
							if (m_nMCrlCount > 1) { strDesc.Format(_T("[%d]"), crl + 1); LcomD_crl.Description += strDesc; }

							if (bCF)
							{
								LcomD_tmp2 = LcomD_tmp; LoadCaseNo = GetCountLcomCase(LcomD_tmp2);
								Set_MovingCase(arMCrlLoad.GetAt(crl), dFactor, LoadCaseNo, LcomD_tmp2, TRUE);
								strDesc.Format(_T("+%sMCRL"), GetFactorString(dFactor)); LcomD_tmp2.Description += strDesc;
								if (m_nMCrlCount > 1) { strDesc.Format(_T("[%d]"), crl + 1); LcomD_tmp2.Description += strDesc; }
							}////////////////////////////////
						}
						//if((nCRL>0 || m_nMCrlCount>0) && bCF)
						//{
						SetCvlJTJD60_ETC(LcomD_crl, LcomD_tmp2, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, bLive, arWLoad, TRUE, arSfLoad, TRUE, arIpLoad, TRUE,
							arTLoad, arTpgLoad, TRUE, arFrLoad, TRUE, 0, dF, nType == 2 || nType == 4, nMsgType);
						//}
					}
				}
			}
			if (m_nBrkCount > 0 && bLive) // Brake Load
			{
				T_LCOM_D LcomD_l = LcomD; LoadCaseNo = GetCountLcomCase(LcomD_l);
				double dBrkF = (nType == 0 || nType == 4) ? (bCF ? dFactor * 0.7 : dFactor) : 1.0;
				int nBRK = 0;
				for (nc = 0; nc < m_nBrkCount; nc++) nBRK += Set_STLCIncludeCS(arBrkLoad.GetAt(nc), dBrkF, LoadCaseNo, LcomD_l, 0);
				if (nBRK > 0)
				{
					strDesc.Format(_T("+%sBRK"), GetFactorString(dBrkF)); LcomD_l.Description += strDesc;
					SetCvlJTJD60_ETC(LcomD_l, LcomD_cf, FALSE, arTmp, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWLoad, TRUE, arSfLoad, FALSE, arIpLoad, FALSE,
						arTLoad, arTpgLoad, TRUE, arFrLoad, FALSE, 0, dF, nType == 2 || nType == 4, nMsgType);
				}
				if (bCF) // MNET:1614, KJH20060809, ¾Õ¿¡¼­ CF°¡ Á¸ÀçÇßÀ¸¸é, CF°¡ Á¦¿ÜµÈ ÇÏÁßÁ¶ÇÕÀ¸·Î ÇÑ¹ø´õ
				{
					LcomD_l = LcomD_cf; LoadCaseNo = GetCountLcomCase(LcomD_l);
					dBrkF = (nType == 0 || nType == 4) ? dFactor : 1.0;
					nBRK = 0;
					for (nc = 0; nc < m_nBrkCount; nc++) nBRK += Set_STLCIncludeCS(arBrkLoad.GetAt(nc), dBrkF, LoadCaseNo, LcomD_l, 0);
					if (nBRK > 0)
					{
						strDesc.Format(_T("+%sBRK"), GetFactorString(dBrkF)); LcomD_l.Description += strDesc;
						SetCvlJTJD60_ETC(LcomD_l, LcomD_cf, FALSE, arTmp, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWLoad, TRUE, arSfLoad, FALSE, arIpLoad, FALSE,
							arTLoad, arTpgLoad, TRUE, arFrLoad, FALSE, 0, dF, nType == 2 || nType == 4, nMsgType);
					}
				}
			}
			int nWCount = m_nWCount * 2; // MNET:1614, KJH20050808
			if (m_nWCount == 0 && bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(W))) nWCount = 1;
			for (nc = 0; nc < nWCount; nc++)
			{
				T_LCOM_D LcomD_l = LcomD; LoadCaseNo = GetCountLcomCase(LcomD_l);
				if (m_nWCount > 0)
				{
					double dWF = (nc % 2 == 0) ? dWFactor : -1.*dWFactor;
					if (Set_STLCIncludeCS(arWLoad.GetAt(nc / 2), dWF, LoadCaseNo, LcomD_l, 0) > 0)
					{
						CString strW = (dWF > 0.0) ? _T("+%sW") : _T("%sW");
						strDesc.Format(strW, GetFactorString(dWF)); LcomD_l.Description += strDesc;
						if (m_nWCount > 1) { strDesc.Format(_T("[%d]"), nc / 2 + 1); LcomD_l.Description += strDesc; }
					}
				}
				if (bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(W)))
				{
					Set_CStageCase(m_aCSLoad.GetAt(nnn), dWFactor, LoadCaseNo, LcomD_l, TRUE);
					strDesc.Format(_T("+%s(cEL)"), GetFactorString(dWFactor)); LcomD_l.Description += strDesc;
				}
				SetCvlJTJD60_ETC(LcomD_l, LcomD_cf, bCF, arTmp, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWLoad, FALSE, arSfLoad, TRUE, arIpLoad, TRUE,
					arTLoad, arTpgLoad, TRUE, arFrLoad, TRUE, 0, dF, nType == 2 || nType == 4, nMsgType);
			}
			if (m_nSfCount > 0) // Stream Flow Pressure
			{
				T_LCOM_D LcomD_l = LcomD; LoadCaseNo = GetCountLcomCase(LcomD_l);
				int nSF = 0;
				for (nc = 0; nc < m_nSfCount; nc++) nSF += Set_STLCIncludeCS(arSfLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD_l, 0);
				if (nSF > 0)
				{
					strDesc.Format(_T("+%sSF"), GetFactorString(dFactor)); LcomD_l.Description += strDesc;
					SetCvlJTJD60_ETC(LcomD_l, LcomD_cf, bCF, arTmp, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWLoad, FALSE, arSfLoad, FALSE, arIpLoad, FALSE,
						arTLoad, arTpgLoad, TRUE, arFrLoad, TRUE, 0, dF, nType == 2 || nType == 4, nMsgType);
				}
			}
			if (m_nIpCount > 0) // Ice Pressure
			{
				T_LCOM_D LcomD_l = LcomD; LoadCaseNo = GetCountLcomCase(LcomD_l);
				int nIP = 0;
				for (nc = 0; nc < m_nIpCount; nc++) nIP += Set_STLCIncludeCS(arIpLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD_l, 0);
				if (nIP > 0)
				{
					strDesc.Format(_T("+%sIP"), GetFactorString(dFactor)); LcomD_l.Description += strDesc;
					SetCvlJTJD60_ETC(LcomD_l, LcomD_cf, bCF, arTmp, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWLoad, FALSE, arSfLoad, FALSE, arIpLoad, FALSE,
						arTLoad, arTpgLoad, TRUE, arFrLoad, TRUE, 0, dF, nType == 2 || nType == 4, nMsgType);
				}
			}
			if ((m_nTCount > 0 || m_nTpgCount > 0) || (bAddCsLoad && (CheckCSEL(D_SGLD_ER1_CVL, _LSX(T)) || CheckCSEL(D_SGLD_ER1_CVL, _LSX(TPG))))) // Temeperature
			{
				T_LCOM_D LcomD_l = LcomD;
				SetCvlJTJD60_Capacity_TPG(LcomD_l, arTLoad, arTpgLoad, arFrLoad, dFactor, dFactor, TRUE, nType == 2 || nType == 4, nMsgType);
			}
		} // next m
	} // next lc

	return TRUE;
}
BOOL CLoadCombCtrl::SetCvlJTJD60_Var3_M(const CArray<T_LCOM_D, T_LCOM_D&>& arData, ArUNT& arMLoad,
	ArINT& arLLoad, ArINT& arIlLoad, ArINT& arCrlLoad, ArINT& arMCrlLoad,
	ArINT& arCfLoad, ArINT& arLsLoad, ArINT& arWLoad, ArINT& arSfLoad, ArINT& arIpLoad,
	ArINT& arTLoad, ArINT& arTpgLoad, ArINT& arFrLoad, ArINT& arBrkLoad, int nType, int nMsgType)
{
	// nType == 0,4: °è¼öÁ¶ÇÕ, 1: 1.0(settlement Á¦¿Ü), 2: 1.0(¸ðµÎ), Live Load Á¦¿Ü
	CArray<T_LCOM_D, T_LCOM_D&> aLcomD; aLcomD.Copy(arData);
	int nLcomSize = aLcomD.GetSize();
	int  nnn = 0;

	BOOL bErec = FALSE;
	for (int nErec = 0; nErec < D_STCT_EREC_MAX_CVL; ++nErec)
	{
		if (m_nConstLoad != 0 && CheckCS_SgldK(D_SGLD_ER1_CVL + nErec, nnn)) bErec = TRUE;
	}

	BOOL bAddCsLoad = bErec;
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER1_CVL,nnn)) || 
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER2_CVL,nnn)) || 
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER3_CVL,nnn)) ||
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER4_CVL,nnn)) ||
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER5_CVL,nnn)) ||
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER6_CVL,nnn)) ||
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER7_CVL,nnn)) ||
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER8_CVL,nnn)) ||
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER9_CVL,nnn)) ||
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER10_CVL,nnn));// CS or ST+CS;

	double dLFactor = (nType == 0 || nType == 4) ? 1.4 : 1.0;
	double dFactor = (nType == 0 || nType == 4) ? 1.4*0.6 : 1.0;
	double dWFactor = (nType == 0 || nType == 4) ? 1.1*0.6 : 1.0;
	double dF = (nType == 0 || nType == 4) ? 0.6 : 1.0;

	ArINT arTmp;

	// ¿µ±¸ÇÏÁß + Â÷·®ÇÏÁß + 3°³ÀÇ °¡º¯ÇÏÁßÀÎ °æ¿ì
	CString strDesc = _T("");
	int LoadCaseNo = 0, nc = 0;
	for (int lc = 0; lc < nLcomSize; lc++)
	{
		int nMCount = max(m_nMCount, 1);
		for (int m = 0; m < nMCount; m++)
		{
			T_LCOM_D LcomD = aLcomD.GetAt(lc); LcomD.nActive = 1; // Active or Strength/Stress
			T_LCOM_D LcomD_cf = LcomD; // MNET:1614, KJH20050809, CF°¡ Á¦¿ÜµÈ load combination
			LoadCaseNo = GetCountLcomCase(LcomD);

			BOOL bLive = FALSE, bCF = FALSE;
			if (nType != 2)
			{
				// moving load
				if (m_nMCount > 0)
				{
					Set_MovingCase(arMLoad.GetAt(m), dLFactor, LoadCaseNo, LcomD, TRUE);
					strDesc.Format(_T("+%sM"), GetFactorString(dLFactor)); LcomD.Description += strDesc;
					if (m_nMCount > 1) { strDesc.Format(_T("[%d]"), m + 1); LcomD.Description += strDesc; }
				}
				int nLL = 0, nIL = 0, nCF = 0;
				if (m_nLCount > 0 || (bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(L)))) // ST or ST+CS
				{
					for (nc = 0; nc < m_nLCount; nc++) nLL += Set_STLCIncludeCS(arLLoad.GetAt(nc), dLFactor, LoadCaseNo, LcomD, 0);
					if (nLL > 0) { strDesc.Format(_T("+%sL"), GetFactorString(dLFactor)); LcomD.Description += strDesc; }
					if (bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(L)))
					{
						Set_CStageCase(m_aCSLoad.GetAt(nnn), dLFactor, LoadCaseNo, LcomD, TRUE);
						strDesc.Format(_T("+%s(cEL)"), GetFactorString(dLFactor)); LcomD.Description += strDesc;
					}
					for (nc = 0; nc < m_nIlCount; nc++) nIL += Set_STLCIncludeCS(arIlLoad.GetAt(nc), dLFactor, LoadCaseNo, LcomD, 0);
					if (nIL > 0) { strDesc.Format(_T("+%sIL"), GetFactorString(dLFactor)); LcomD.Description += strDesc; }
				}
				bLive = m_nMCount > 0 || (m_nLCount > 0 || (bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(L))));
				if (bLive)
				{
					LcomD_cf = LcomD;
					for (nc = 0; nc < m_nCfCount; nc++) nCF += Set_STLCIncludeCS(arCfLoad.GetAt(nc), dLFactor, LoadCaseNo, LcomD, 0);
					if (nCF > 0) { strDesc.Format(_T("+%sCF"), GetFactorString(dLFactor)); LcomD.Description += strDesc; bCF = TRUE; }
				}
			}

			if (m_nLsCount > 0) // Live Load Surcharge
			{
				T_LCOM_D LcomD_l = LcomD; LoadCaseNo = GetCountLcomCase(LcomD_l);
				int nLS = 0;
				for (nc = 0; nc < m_nLsCount; nc++) nLS += Set_STLCIncludeCS(arLsLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD_l, 0);
				T_LCOM_D LcomD_tmp = LcomD_cf;
				if (bCF)
				{
					LoadCaseNo = GetCountLcomCase(LcomD_tmp);
					for (nc = 0; nc < m_nLsCount; nc++) Set_STLCIncludeCS(arLsLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD_tmp, 0);
				}
				if (nLS > 0)
				{
					strDesc.Format(_T("+%sLS"), GetFactorString(dFactor)); LcomD_l.Description += strDesc;
					if (bCF) LcomD_tmp.Description += strDesc;
					SetCvlJTJD60_ETC(LcomD_l, LcomD_tmp, bCF, arCrlLoad, arMCrlLoad, nType != 2, arBrkLoad, bLive, arWLoad, TRUE, arSfLoad, TRUE, arIpLoad, TRUE,
						arTLoad, arTpgLoad, TRUE, arFrLoad, FALSE, 1, dF, nType == 2 || nType == 4, nMsgType);
				}
			}
			// MNET:XXXX-BJLEE(BBONG)-20060503
			if (nType != 2 && (m_nCrlCount > 0 || m_nMCrlCount > 0)) // Crowd Load
			{
				T_LCOM_D LcomD_l = LcomD; LoadCaseNo = GetCountLcomCase(LcomD_l);
				int nCRL = 0;
				for (nc = 0; nc < m_nCrlCount; nc++) nCRL += Set_STLCIncludeCS(arCrlLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD_l, 0);
				T_LCOM_D LcomD_tmp = LcomD_cf;
				if (bCF)
				{
					LoadCaseNo = GetCountLcomCase(LcomD_tmp);
					for (nc = 0; nc < m_nCrlCount; nc++) Set_STLCIncludeCS(arCrlLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD_tmp, 0);
				}
				// MNET:XXXX-BJLEE(BBONG)-20060502 
				if (nCRL > 0 || m_nMCrlCount > 0)
				{
					if (nCRL > 0)
					{
						strDesc.Format(_T("+%sCRL"), GetFactorString(dFactor)); LcomD_l.Description += strDesc;
						if (bCF) LcomD_tmp.Description += strDesc;
					}

					int nMCrlCount = (m_nMCrlCount > 0) ? m_nMCrlCount : ((nCRL > 0) ? 1 : 0);
					T_LCOM_D LcomD_tmp2;
					for (int crl = 0; crl < nMCrlCount; ++crl)
					{
						T_LCOM_D LcomD_crl = LcomD_l; LoadCaseNo = GetCountLcomCase(LcomD_crl);
						LcomD_tmp2.Initialize();
						if (m_nMCrlCount > 0)
						{
							Set_MovingCase(arMCrlLoad.GetAt(crl), dFactor, LoadCaseNo, LcomD_crl, TRUE);
							strDesc.Format(_T("+%sMCRL"), GetFactorString(dFactor)); LcomD_crl.Description += strDesc;
							if (m_nMCrlCount > 1) { strDesc.Format(_T("[%d]"), crl + 1); LcomD_crl.Description += strDesc; }

							if (bCF)
							{
								LcomD_tmp2 = LcomD_tmp; LoadCaseNo = GetCountLcomCase(LcomD_tmp2);
								Set_MovingCase(arMCrlLoad.GetAt(crl), dFactor, LoadCaseNo, LcomD_tmp2, TRUE);
								strDesc.Format(_T("+%sMCRL"), GetFactorString(dFactor)); LcomD_tmp2.Description += strDesc;
								if (m_nMCrlCount > 1) { strDesc.Format(_T("[%d]"), crl + 1); LcomD_tmp2.Description += strDesc; }
							}////////////////////////////////
						}

						//if((nCRL>0 || m_nMCrlCount>0) && bCF)
						//{
						SetCvlJTJD60_ETC(LcomD_crl, LcomD_tmp2, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, bLive, arWLoad, TRUE, arSfLoad, TRUE, arIpLoad, TRUE,
							arTLoad, arTpgLoad, TRUE, arFrLoad, FALSE, 2, dF, nType == 2 || nType == 4, nMsgType);
						//}


					}
				}
			}
			if (m_nBrkCount > 0 && bLive) // Brake Load
			{
				T_LCOM_D LcomD_l = LcomD; LoadCaseNo = GetCountLcomCase(LcomD_l);
				double dBrkF = (nType == 0 || nType == 4) ? (bCF ? dFactor * 0.7 : dFactor) : 1.0;
				int nBRK = 0;
				for (nc = 0; nc < m_nBrkCount; nc++) nBRK += Set_STLCIncludeCS(arBrkLoad.GetAt(nc), dBrkF, LoadCaseNo, LcomD_l, 0);
				if (nBRK > 0)
				{
					strDesc.Format(_T("+%sBRK"), GetFactorString(dBrkF)); LcomD_l.Description += strDesc;
					SetCvlJTJD60_ETC(LcomD_l, LcomD_cf, FALSE, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWLoad, TRUE, arSfLoad, FALSE, arIpLoad, FALSE,
						arTLoad, arTpgLoad, FALSE, arFrLoad, FALSE, 3, dF, nType == 2 || nType == 4, nMsgType);
				}
				if (bCF) // MNET:1614, KJH20050809, ¾Õ¿¡¼­ CF°¡ Á¸ÀçÇÏ¸é CF°¡ Á¦¿ÜµÈ ÇÏÁßÁ¶ÇÕ ÇÑ¹ø´õ...
				{
					LcomD_l = LcomD_cf; LoadCaseNo = GetCountLcomCase(LcomD_l);
					dBrkF = (nType == 0 || nType == 4) ? dFactor : 1.0;
					int nBRK = 0;
					for (nc = 0; nc < m_nBrkCount; nc++) nBRK += Set_STLCIncludeCS(arBrkLoad.GetAt(nc), dBrkF, LoadCaseNo, LcomD_l, 0);
					if (nBRK > 0)
					{
						strDesc.Format(_T("+%sBRK"), GetFactorString(dBrkF)); LcomD_l.Description += strDesc;
						SetCvlJTJD60_ETC(LcomD_l, LcomD_cf, FALSE, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWLoad, TRUE, arSfLoad, FALSE, arIpLoad, FALSE,
							arTLoad, arTpgLoad, FALSE, arFrLoad, FALSE, 3, dF, nType == 2 || nType == 4, nMsgType);
					}
				}
			}
			int nWCount = m_nWCount * 2; // MNET:1614, KJH20050808
			if (m_nWCount == 0 && bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(W))) nWCount = 1;
			for (nc = 0; nc < nWCount; nc++)
			{
				T_LCOM_D LcomD_l = LcomD; LoadCaseNo = GetCountLcomCase(LcomD_l);
				if (m_nWCount > 0)
				{
					double dWF = (nc % 2 == 0) ? dWFactor : -1.*dWFactor;
					if (Set_STLCIncludeCS(arWLoad.GetAt(nc / 2), dWF, LoadCaseNo, LcomD_l, 0) > 0)
					{
						CString strW = (dWF > 0.0) ? _T("+%sW") : _T("%sW");
						strDesc.Format(strW, GetFactorString(dWF)); LcomD_l.Description += strDesc;
						if (m_nWCount > 1) { strDesc.Format(_T("[%d]"), nc / 2 + 1); LcomD_l.Description += strDesc; }
					}
				}
				if (bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(W)))
				{
					Set_CStageCase(m_aCSLoad.GetAt(nnn), dWFactor, LoadCaseNo, LcomD_l, TRUE);
					strDesc.Format(_T("+%s(cEL)"), GetFactorString(dWFactor)); LcomD_l.Description += strDesc;
				}
				SetCvlJTJD60_ETC(LcomD_l, LcomD_cf, bCF, arTmp, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWLoad, FALSE, arSfLoad, TRUE, arIpLoad, TRUE,
					arTLoad, arTpgLoad, TRUE, arFrLoad, FALSE, 4, dF, nType == 2 || nType == 4, nMsgType);
			}
			if (m_nSfCount > 0) // Stream Flow Pressure
			{
				T_LCOM_D LcomD_l = LcomD; LoadCaseNo = GetCountLcomCase(LcomD_l);
				int nSF = 0;
				for (nc = 0; nc < m_nSfCount; nc++) nSF += Set_STLCIncludeCS(arSfLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD_l, 0);
				if (nSF > 0)
				{
					strDesc.Format(_T("+%sSF"), GetFactorString(dFactor)); LcomD_l.Description += strDesc;
					SetCvlJTJD60_ETC(LcomD_l, LcomD_cf, bCF, arTmp, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWLoad, FALSE, arSfLoad, FALSE, arIpLoad, FALSE,
						arTLoad, arTpgLoad, TRUE, arFrLoad, FALSE, 5, dF, nType == 2 || nType == 4, nMsgType);
				}
			}
			if (m_nIpCount > 0) // Ice Pressure
			{
				T_LCOM_D LcomD_l = LcomD; LoadCaseNo = GetCountLcomCase(LcomD_l);
				int nIP = 0;
				for (nc = 0; nc < m_nIpCount; nc++) nIP += Set_STLCIncludeCS(arIpLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD_l, 0);
				if (nIP > 0)
				{
					strDesc.Format(_T("+%sIP"), GetFactorString(dFactor)); LcomD_l.Description += strDesc;
					SetCvlJTJD60_ETC(LcomD_l, LcomD_cf, bCF, arTmp, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWLoad, FALSE, arSfLoad, FALSE, arIpLoad, FALSE,
						arTLoad, arTpgLoad, TRUE, arFrLoad, FALSE, 5, dF, nType == 2 || nType == 4, nMsgType);
				}
			}
		} // next m
	} // next lc

	return TRUE;
}
BOOL CLoadCombCtrl::SetCvlJTJD60_Var4_M(const CArray<T_LCOM_D, T_LCOM_D&>& arData, ArUNT& arMLoad,
	ArINT& arLLoad, ArINT& arIlLoad, ArINT& arCrlLoad, ArINT& arMCrlLoad,
	ArINT& arCfLoad, ArINT& arLsLoad, ArINT& arWLoad, ArINT& arSfLoad, ArINT& arIpLoad,
	ArINT& arTLoad, ArINT& arTpgLoad, ArINT& arFrLoad, ArINT& arBrkLoad, int nType, int nMsgType)
{
	// nType == 0,4: °è¼öÁ¶ÇÕ, 1: 1.0(settlement Á¦¿Ü), 2: 1.0(¸ðµÎ), Live Load Á¦¿Ü
	CArray<T_LCOM_D, T_LCOM_D&> aLcomD; aLcomD.Copy(arData);
	int  nLcomSize = aLcomD.GetSize();
	int  nnn = 0;

	BOOL bErec = FALSE;
	for (int nErec = 0; nErec < D_STCT_EREC_MAX_CVL; ++nErec)
	{
		if (m_nConstLoad != 0 && CheckCS_SgldK(D_SGLD_ER1_CVL + nErec, nnn)) bErec = TRUE;
	}

	BOOL bAddCsLoad = bErec;
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER1_CVL,nnn)) || 
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER2_CVL,nnn)) || 
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER3_CVL,nnn)) ||
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER4_CVL,nnn)) ||
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER5_CVL,nnn)) ||
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER6_CVL,nnn)) ||
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER7_CVL,nnn)) ||
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER8_CVL,nnn)) ||
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER9_CVL,nnn)) ||
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER10_CVL,nnn));// CS or ST+CS;

	double dLFactor = (nType == 0 || nType == 4) ? 1.4 : 1.0;
	double dFactor = (nType == 0 || nType == 4) ? 1.4*0.5 : 1.0;
	double dWFactor = (nType == 0 || nType == 4) ? 1.1*0.5 : 1.0;
	double dF = (nType == 0 || nType == 4) ? 0.5 : 1.0;

	ArINT arTmp;

	// ¿µ±¸ÇÏÁß + Â÷·®ÇÏÁß + 4°³ÀÇ °¡º¯ÇÏÁßÀÎ °æ¿ì
	CString strDesc = _T("");
	int LoadCaseNo = 0, nc = 0;
	for (int lc = 0; lc < nLcomSize; lc++)
	{
		int nMCount = max(m_nMCount, 1);
		for (int m = 0; m < nMCount; m++)
		{
			T_LCOM_D LcomD = aLcomD.GetAt(lc); LcomD.nActive = 1; // Active or Strength/Stress
			T_LCOM_D LcomD_cf = LcomD; // MNET:1614, KJH20050809, CF°¡ Á¦¿ÜµÈ ÇÏÁßÁ¶ÇÕ
			LoadCaseNo = GetCountLcomCase(LcomD);
			BOOL bLive = FALSE, bCF = FALSE;
			if (nType != 2)
			{
				// moving load
				if (m_nMCount > 0)
				{
					Set_MovingCase(arMLoad.GetAt(m), dLFactor, LoadCaseNo, LcomD, TRUE);
					strDesc.Format(_T("+%sM"), GetFactorString(dLFactor)); LcomD.Description += strDesc;
					if (m_nMCount > 1) { strDesc.Format(_T("[%d]"), m + 1); LcomD.Description += strDesc; }
				}
				int nLL = 0, nIL = 0, nCF = 0;
				if (m_nLCount > 0 || (bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(L)))) // ST or ST+CS
				{
					for (nc = 0; nc < m_nLCount; nc++) nLL += Set_STLCIncludeCS(arLLoad.GetAt(nc), dLFactor, LoadCaseNo, LcomD, 0);
					if (nLL > 0) { strDesc.Format(_T("+%sL"), GetFactorString(dLFactor)); LcomD.Description += strDesc; }
					if (bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(L)))
					{
						Set_CStageCase(m_aCSLoad.GetAt(nnn), dLFactor, LoadCaseNo, LcomD, TRUE);
						strDesc.Format(_T("+%s(cEL)"), GetFactorString(dLFactor)); LcomD.Description += strDesc;
					}
					for (nc = 0; nc < m_nIlCount; nc++) nIL += Set_STLCIncludeCS(arIlLoad.GetAt(nc), dLFactor, LoadCaseNo, LcomD, 0);
					if (nIL > 0) { strDesc.Format(_T("+%sIL"), GetFactorString(dLFactor)); LcomD.Description += strDesc; }
				}
				bLive = m_nMCount > 0 || (m_nLCount > 0 || (bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(L))));
				if (bLive)
				{
					LcomD_cf = LcomD;
					for (nc = 0; nc < m_nCfCount; nc++) nCF += Set_STLCIncludeCS(arCfLoad.GetAt(nc), dLFactor, LoadCaseNo, LcomD, 0);
					if (nCF > 0) { strDesc.Format(_T("+%sCF"), GetFactorString(dLFactor)); LcomD.Description += strDesc; bCF = TRUE; }
				}
			}

			if (m_nLsCount > 0) // Live Load Surcharge
			{
				T_LCOM_D LcomD_l = LcomD; LoadCaseNo = GetCountLcomCase(LcomD_l);
				int nLS = 0;
				for (nc = 0; nc < m_nLsCount; nc++) nLS += Set_STLCIncludeCS(arLsLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD_l, 0);
				T_LCOM_D LcomD_tmp = LcomD_cf;
				if (bCF)
				{
					LoadCaseNo = GetCountLcomCase(LcomD_tmp);
					for (nc = 0; nc < m_nLsCount; nc++) Set_STLCIncludeCS(arLsLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD_tmp, 0);
				}
				if (nLS > 0)
				{
					strDesc.Format(_T("+%sLS"), GetFactorString(dFactor)); LcomD_l.Description += strDesc;
					if (bCF) LcomD_tmp.Description += strDesc;
					SetCvlJTJD60_ETC(LcomD_l, LcomD_tmp, bCF, arCrlLoad, arMCrlLoad, nType != 2, arBrkLoad, bLive, arWLoad, TRUE, arSfLoad, TRUE, arIpLoad, TRUE,
						arTLoad, arTpgLoad, FALSE, arFrLoad, FALSE, 6, dF, nType == 2 || nType == 4, nMsgType);
				}
			}
			// MNET:XXXX-BJLEE(BBONG)-20060503
			if (nType != 2 && (m_nCrlCount > 0 || m_nMCrlCount > 0)) // Crowd Load
			{
				T_LCOM_D LcomD_l = LcomD; LoadCaseNo = GetCountLcomCase(LcomD_l);
				int nCRL = 0;
				for (nc = 0; nc < m_nCrlCount; nc++) nCRL += Set_STLCIncludeCS(arCrlLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD_l, 0);
				T_LCOM_D LcomD_tmp = LcomD_cf;
				if (bCF)
				{
					LoadCaseNo = GetCountLcomCase(LcomD_tmp);
					for (nc = 0; nc < m_nCrlCount; nc++) Set_STLCIncludeCS(arCrlLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD_tmp, 0);
				}
				// MNET:XXXX-BJLEE(BBONG)-20060502 
				if (nCRL > 0 || m_nMCrlCount > 0)
				{
					if (nCRL > 0)
					{
						strDesc.Format(_T("+%sCRL"), GetFactorString(dFactor)); LcomD_l.Description += strDesc;
						if (bCF) LcomD_tmp.Description += strDesc;
					}

					int nMCrlCount = (m_nMCrlCount > 0) ? m_nMCrlCount : ((nCRL > 0) ? 1 : 0);
					T_LCOM_D LcomD_tmp2;
					for (int crl = 0; crl < nMCrlCount; ++crl)
					{
						T_LCOM_D LcomD_crl = LcomD_l; LoadCaseNo = GetCountLcomCase(LcomD_crl);
						if (m_nMCrlCount > 0)
						{
							Set_MovingCase(arMCrlLoad.GetAt(crl), dFactor, LoadCaseNo, LcomD_crl, TRUE);
							strDesc.Format(_T("+%sMCRL"), GetFactorString(dFactor)); LcomD_crl.Description += strDesc;
							if (m_nMCrlCount > 1) { strDesc.Format(_T("[%d]"), crl + 1); LcomD_crl.Description += strDesc; }

							if (bCF)
							{
								LcomD_tmp2 = LcomD_tmp; LoadCaseNo = GetCountLcomCase(LcomD_tmp2);
								Set_MovingCase(arMCrlLoad.GetAt(crl), dFactor, LoadCaseNo, LcomD_tmp2, TRUE);
								strDesc.Format(_T("+%sMCRL"), GetFactorString(dFactor)); LcomD_tmp2.Description += strDesc;
								if (m_nMCrlCount > 1) { strDesc.Format(_T("[%d]"), crl + 1); LcomD_tmp2.Description += strDesc; }
							}
						}

						SetCvlJTJD60_ETC(LcomD_crl, LcomD_tmp2, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, bLive, arWLoad, TRUE, arSfLoad, TRUE, arIpLoad, TRUE,
							arTLoad, arTpgLoad, FALSE, arFrLoad, FALSE, 7, dF, nType == 2 || nType == 4, nMsgType);

					}
				}
			}

			int nWCount = m_nWCount * 2; // MNET:1614, KJH20050808
			if (m_nWCount == 0 && bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(W))) nWCount = 1;
			for (nc = 0; nc < nWCount; nc++)
			{
				T_LCOM_D LcomD_l = LcomD; LoadCaseNo = GetCountLcomCase(LcomD_l);
				if (m_nWCount > 0)
				{
					double dWF = (nc % 2 == 0) ? dWFactor : -1.*dWFactor;
					if (Set_STLCIncludeCS(arWLoad.GetAt(nc / 2), dWF, LoadCaseNo, LcomD_l, 0) > 0)
					{
						CString strW = (dWF > 0.0) ? _T("+%sW") : _T("%sW");
						strDesc.Format(strW, GetFactorString(dWF)); LcomD_l.Description += strDesc;
						if (m_nWCount > 1) { strDesc.Format(_T("[%d]"), nc / 2 + 1); LcomD_l.Description += strDesc; }
					}
				}
				if (bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(W)))
				{
					Set_CStageCase(m_aCSLoad.GetAt(nnn), dWFactor, LoadCaseNo, LcomD_l, TRUE);
					strDesc.Format(_T("+%s(cEL)"), GetFactorString(dWFactor)); LcomD_l.Description += strDesc;
				}
				SetCvlJTJD60_ETC(LcomD_l, LcomD_cf, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWLoad, FALSE, arSfLoad, TRUE, arIpLoad, TRUE,
					arTLoad, arTpgLoad, FALSE, arFrLoad, FALSE, 7, dF, nType == 2 || nType == 4, nMsgType);
			}
		} // next m
	} // next lc

	return TRUE;
}
BOOL CLoadCombCtrl::SetCvlJTJD60_Var56_M(const CArray<T_LCOM_D, T_LCOM_D&>& arData, ArUNT& arMLoad,
	ArINT& arLLoad, ArINT& arIlLoad, ArINT& arCrlLoad, ArINT& arMCrlLoad,
	ArINT& arCfLoad, ArINT& arLsLoad, ArINT& arWLoad, ArINT& arSfLoad, ArINT& arIpLoad,
	ArINT& arTLoad, ArINT& arTpgLoad, ArINT& arFrLoad, ArINT& arBrkLoad, int nType, int nMsgType)
{
	// nType == 0,4: °è¼öÁ¶ÇÕ, 1: 1.0(settlement Á¦¿Ü), 2: 1.0(¸ðµÎ), Live Load Á¦¿Ü
	CArray<T_LCOM_D, T_LCOM_D&> aLcomD; aLcomD.Copy(arData);
	int  nLcomSize = aLcomD.GetSize();
	int  nnn = 0;

	BOOL bErec = FALSE;
	for (int nErec = 0; nErec < D_STCT_EREC_MAX_CVL; ++nErec)
	{
		if (m_nConstLoad != 0 && CheckCS_SgldK(D_SGLD_ER1_CVL + nErec, nnn)) bErec = TRUE;
	}

	BOOL bAddCsLoad = bErec;
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER1_CVL,nnn)) || 
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER2_CVL,nnn)) || 
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER3_CVL,nnn)) ||
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER4_CVL,nnn)) ||
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER5_CVL,nnn)) ||
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER6_CVL,nnn)) ||
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER7_CVL,nnn)) ||
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER8_CVL,nnn)) ||
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER9_CVL,nnn)) ||
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER10_CVL,nnn));// CS or ST+CS;

	double dLFactor = (nType == 0 || nType == 4) ? 1.4 : 1.0;
	double dFactor = (nType == 0 || nType == 4) ? 1.4*0.5 : 1.0;
	double dWFactor = (nType == 0 || nType == 4) ? 1.1*0.5 : 1.0;
	double dF = (nType == 0 || nType == 4) ? 0.5 : 1.0;

	ArINT arTmp;
	// ¿µ±¸ÇÏÁß + Â÷·®ÇÏÁß + 5°³ÀÇ °¡º¯ÇÏÁßÀÎ °æ¿ì
	CString strDesc = _T("");
	int LoadCaseNo = 0, nc = 0;
	for (int lc = 0; lc < nLcomSize; lc++)
	{
		int nMCount = max(m_nMCount, 1);
		for (int m = 0; m < nMCount; m++)
		{
			T_LCOM_D LcomD = aLcomD.GetAt(lc); LcomD.nActive = 1; // Active or Strength/Stress
			T_LCOM_D LcomD_cf = LcomD; // MNET:1614, KJH20050809, CFÁ¦¿ÜÇÑ ÇÏÁßÁ¶ÇÕ
			LoadCaseNo = GetCountLcomCase(LcomD);
			BOOL bLive = FALSE, bCF = FALSE;
			if (nType != 2)
			{
				// moving load
				if (m_nMCount > 0)
				{
					Set_MovingCase(arMLoad.GetAt(m), dLFactor, LoadCaseNo, LcomD, TRUE);
					strDesc.Format(_T("+%sM"), GetFactorString(dLFactor)); LcomD.Description += strDesc;
					if (m_nMCount > 1) { strDesc.Format(_T("[%d]"), m + 1); LcomD.Description += strDesc; }
				}
				int nLL = 0, nIL = 0, nCF = 0;
				if (m_nLCount > 0 || (bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(L)))) // ST or ST+CS
				{
					for (nc = 0; nc < m_nLCount; nc++) nLL += Set_STLCIncludeCS(arLLoad.GetAt(nc), dLFactor, LoadCaseNo, LcomD, 0);
					if (nLL > 0) { strDesc.Format(_T("+%sL"), GetFactorString(dLFactor)); LcomD.Description += strDesc; }
					if (bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(L)))
					{
						Set_CStageCase(m_aCSLoad.GetAt(nnn), dLFactor, LoadCaseNo, LcomD, TRUE);
						strDesc.Format(_T("+%s(cEL)"), GetFactorString(dLFactor)); LcomD.Description += strDesc;
					}
					for (nc = 0; nc < m_nIlCount; nc++) nIL += Set_STLCIncludeCS(arIlLoad.GetAt(nc), dLFactor, LoadCaseNo, LcomD, 0);
					if (nIL > 0) { strDesc.Format(_T("+%sIL"), GetFactorString(dLFactor)); LcomD.Description += strDesc; }
				}
				bLive = m_nMCount > 0 || (m_nLCount > 0 || (bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(L))));
				if (bLive)
				{
					LcomD_cf = LcomD;
					for (nc = 0; nc < m_nCfCount; nc++) nCF += Set_STLCIncludeCS(arCfLoad.GetAt(nc), dLFactor, LoadCaseNo, LcomD, 0);
					if (nCF > 0) { strDesc.Format(_T("+%sCF"), GetFactorString(dLFactor)); LcomD.Description += strDesc; bCF = TRUE; }
				}
			}

			if (m_nLsCount > 0) // Live Load Surcharge
			{
				T_LCOM_D LcomD_l = LcomD; LoadCaseNo = GetCountLcomCase(LcomD_l);
				int nLS = 0;
				for (nc = 0; nc < m_nLsCount; nc++) nLS += Set_STLCIncludeCS(arLsLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD_l, 0);
				T_LCOM_D LcomD_tmp = LcomD_cf;
				if (bCF)
				{
					LoadCaseNo = GetCountLcomCase(LcomD_tmp);
					for (nc = 0; nc < m_nLsCount; nc++) Set_STLCIncludeCS(arLsLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD_tmp, 0);
				}
				if (nLS > 0)
				{
					strDesc.Format(_T("+%sLS"), GetFactorString(dFactor)); LcomD_l.Description += strDesc;
					if (bCF) LcomD_tmp.Description += strDesc;
					SetCvlJTJD60_ETC(LcomD_l, LcomD_tmp, bCF, arCrlLoad, arMCrlLoad, nType != 2, arBrkLoad, bLive, arWLoad, TRUE, arSfLoad, FALSE, arIpLoad, FALSE,
						arTLoad, arTpgLoad, FALSE, arFrLoad, FALSE, 8, dF, nType == 2 || nType == 4, nMsgType);
				}
			}
			// MNET:XXXX-BJLEE(BBONG)-20060503
			if (nType != 2 && (m_nCrlCount > 0 || m_nMCrlCount > 0)) // Crowd Load
			{
				T_LCOM_D LcomD_l = LcomD; LoadCaseNo = GetCountLcomCase(LcomD_l);
				int nCRL = 0;
				for (nc = 0; nc < m_nCrlCount; nc++) nCRL += Set_STLCIncludeCS(arCrlLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD_l, 0);
				T_LCOM_D LcomD_tmp = LcomD_cf;
				// MNET:XXXX-BJLEE(BBONG)-20060502 
				if (nCRL > 0 || m_nMCrlCount > 0)
				{
					if (nCRL > 0)
					{
						strDesc.Format(_T("+%sCRL"), GetFactorString(dFactor)); LcomD_l.Description += strDesc;
						if (bCF) LcomD_tmp.Description += strDesc;
					}

					int nMCrlCount = (m_nMCrlCount > 0) ? m_nMCrlCount : ((nCRL > 0) ? 1 : 0);
					T_LCOM_D LcomD_tmp2;
					for (int crl = 0; crl < nMCrlCount; ++crl)
					{
						T_LCOM_D LcomD_crl = LcomD_l; LoadCaseNo = GetCountLcomCase(LcomD_crl);
						if (m_nMCrlCount > 0)
						{
							Set_MovingCase(arMCrlLoad.GetAt(crl), dFactor, LoadCaseNo, LcomD_crl, TRUE);
							strDesc.Format(_T("+%sMCRL"), GetFactorString(dFactor)); LcomD_crl.Description += strDesc;
							if (m_nMCrlCount > 1) { strDesc.Format(_T("[%d]"), crl + 1); LcomD_crl.Description += strDesc; }

							if (bCF)
							{
								LcomD_tmp2 = LcomD_tmp; LoadCaseNo = GetCountLcomCase(LcomD_tmp2);
								Set_MovingCase(arMCrlLoad.GetAt(crl), dFactor, LoadCaseNo, LcomD_tmp2, TRUE);
								strDesc.Format(_T("+%sMCRL"), GetFactorString(dFactor)); LcomD_tmp2.Description += strDesc;
								if (m_nMCrlCount > 1) { strDesc.Format(_T("[%d]"), crl + 1); LcomD_tmp2.Description += strDesc; }
							}
						}
						//if((nCRL>0 || m_nMCrlCount>0))
						//{
						SetCvlJTJD60_ETC(LcomD_l, LcomD_cf, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWLoad, TRUE, arSfLoad, FALSE, arIpLoad, FALSE,
							arTLoad, arTpgLoad, FALSE, arFrLoad, FALSE, 9, dF, nType == 2 || nType == 4, nMsgType);
						//}
					}
				}
			}




		} // next m
	} // next lc

	// ¿µ±¸ÇÏÁß + Â÷·®ÇÏÁß + 6°³ÀÇ °¡º¯ÇÏÁßÀÎ °æ¿ì
	for (int lc = 0; lc < nLcomSize; lc++)
	{
		int nMCount = max(m_nMCount, 1);
		for (int m = 0; m < nMCount; m++)
		{
			T_LCOM_D LcomD = aLcomD.GetAt(lc); LcomD.nActive = 1; // Active or Strength/Stress
			LoadCaseNo = GetCountLcomCase(LcomD);

			if (nType != 2)
			{
				// moving load
				if (m_nMCount > 0)
				{
					Set_MovingCase(arMLoad.GetAt(m), dLFactor, LoadCaseNo, LcomD, TRUE);
					strDesc.Format(_T("+%sM"), GetFactorString(dLFactor)); LcomD.Description += strDesc;
					if (m_nMCount > 1) { strDesc.Format(_T("[%d]"), m + 1); LcomD.Description += strDesc; }
				}
				if (m_nLCount > 0 || (bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(L)))) // ST or ST+CS
				{
					int nLL = 0, nIL = 0, nCF = 0;
					for (nc = 0; nc < m_nLCount; nc++) nLL += Set_STLCIncludeCS(arLLoad.GetAt(nc), dLFactor, LoadCaseNo, LcomD, 0);
					if (nLL > 0) { strDesc.Format(_T("+%sL"), GetFactorString(dLFactor)); LcomD.Description += strDesc; }
					if (bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(L)))
					{
						Set_CStageCase(m_aCSLoad.GetAt(nnn), dLFactor, LoadCaseNo, LcomD, TRUE);
						strDesc.Format(_T("+%s(cEL)"), GetFactorString(dLFactor)); LcomD.Description += strDesc;
					}
					for (nc = 0; nc < m_nIlCount; nc++) nIL += Set_STLCIncludeCS(arIlLoad.GetAt(nc), dLFactor, LoadCaseNo, LcomD, 0);
					if (nIL > 0) { strDesc.Format(_T("+%sIL"), GetFactorString(dLFactor)); LcomD.Description += strDesc; }
					for (nc = 0; nc < m_nCfCount; nc++) nCF += Set_STLCIncludeCS(arCfLoad.GetAt(nc), dLFactor, LoadCaseNo, LcomD, 0);
					if (nCF > 0) { strDesc.Format(_T("+%sCF"), GetFactorString(dLFactor)); LcomD.Description += strDesc; }
				}
			}

			if (m_nLsCount > 0) // Live Load Surcharge
			{
				T_LCOM_D LcomD_l = LcomD; LoadCaseNo = GetCountLcomCase(LcomD_l);
				int nLS = 0;
				for (nc = 0; nc < m_nLsCount; nc++) nLS += Set_STLCIncludeCS(arLsLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD_l, 0);
				if (nLS > 0)
				{
					strDesc.Format(_T("+%sLS"), GetFactorString(dFactor)); LcomD_l.Description += strDesc;
					SetCvlJTJD60_ETC(LcomD_l, LcomD_l, FALSE, arCrlLoad, arMCrlLoad, nType != 2, arBrkLoad, FALSE, arWLoad, FALSE, arSfLoad, FALSE, arIpLoad, FALSE,
						arTLoad, arTpgLoad, FALSE, arFrLoad, FALSE, 10, dF, nType == 2 || nType == 4, nMsgType);
				}
			}
		}
	}

	return TRUE;
}
BOOL CLoadCombCtrl::SetCvlJTJD60_Accident_E(ArINT& arELoad)
{
	// seismic load
	int LoadCaseNo = 0;
	int  nnn = 0;

	BOOL bErec = FALSE;
	for (int nErec = 0; nErec < D_STCT_EREC_MAX_CVL; ++nErec)
	{
		if (m_nConstLoad != 0 && CheckCS_SgldK(D_SGLD_ER1_CVL + nErec, nnn)) bErec = TRUE;
	}

	BOOL bAddCsLoad = bErec;
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER1_CVL,nnn)) || 
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER2_CVL,nnn)) || 
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER3_CVL,nnn)) ||
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER4_CVL,nnn)) ||
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER5_CVL,nnn)) ||
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER6_CVL,nnn)) ||
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER7_CVL,nnn)) ||
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER8_CVL,nnn)) ||
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER9_CVL,nnn)) ||
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER10_CVL,nnn));// CS or ST+CS;
	CString strDesc = _T("");

	int nLcomSize = m_aAcdnLcomD.GetSize();
	for (int lc = 0; lc < nLcomSize; lc++)
	{
		T_LCOM_D LcomD = m_aAcdnLcomD.GetAt(lc); LoadCaseNo = GetCountLcomCase(LcomD);
		if (bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(E)))
		{
			Set_CStageCase(m_aCSLoad.GetAt(nnn), 1.0, LoadCaseNo, LcomD, TRUE);
			strDesc.Format(_T("+%s(cEL)"), GetFactorString(1.0)); LcomD.Description += strDesc;
		}
		int nECount = m_nECount * 2; // MNET:1614, KJH20050808
		for (int e = 0; e < nECount; e++)
		{
			T_LCOM_D LcomD_e = LcomD; LoadCaseNo = GetCountLcomCase(LcomD_e);
			if (m_nECount > 0)
			{
				double dEF = (e % 2 == 0) ? 1.0 : -1.0;
				if (Set_STLCIncludeCS(arELoad.GetAt(e / 2), dEF, LoadCaseNo, LcomD_e, 0) > 0)
				{
					CString strE = (dEF > 0.0) ? _T("+%sE") : _T("%sE");
					strDesc.Format(strE, GetFactorString(dEF)); LcomD_e.Description += strDesc;
					if (m_nECount > 1) { strDesc.Format(_T("[%d]"), e / 2 + 1); LcomD_e.Description += strDesc; }
				}
			}
			Set_LoadComb4Civil(LcomD_e, D_STR, 0, 104); // ¿ì¿¬Á¶ÇÕ
		}
	}
	return TRUE;
}
BOOL CLoadCombCtrl::SetCvlJTJD60_Accident_Esp(ArUNT& arEspLoad)
{
	// response spectrum Load
	int LoadCaseNo = 0;
	CString strDesc = _T("");

	int nLcomSize = m_aAcdnLcomD.GetSize();
	for (int lc = 0; lc < nLcomSize; lc++)
	{
		int nEspCount = m_nEspCount * 2; // MNET:1614, KJH20050808
		for (int e = 0; e < nEspCount; e++)
		{
			T_LCOM_D LcomD = m_aAcdnLcomD.GetAt(lc); LoadCaseNo = GetCountLcomCase(LcomD);

			double dESP = (e % 2 == 0) ? 1.0 : -1.0;
			Set_SpectrumCase(arEspLoad.GetAt(e / 2), dESP, LoadCaseNo, LcomD);
			CString strESP = (dESP > 0.0) ? _T("+%sESP") : _T("%sESP");
			strDesc.Format(strESP, GetFactorString(dESP)); LcomD.Description += strDesc;
			if (m_nEspCount > 1) { strDesc.Format(_T("[%d]"), e / 2 + 1); LcomD.Description += strDesc; }

			Set_LoadComb4Civil(LcomD, D_STR, 0, 104); // ¿ì¿¬Á¶ÇÕ
		}
	}
	return TRUE;
}
// response spectrum seismic Load by maxiao 2009-10-30
BOOL CLoadCombCtrl::SetCvlJTJD60_Accident_Esp(ArUNT& arEspLoad, ArINT& arDLoad, ArINT& arPsLoad, ArINT& arEhLoad, ArINT& arEvLoad,
	ArINT& arWpLoad, ArINT& arBLoad)
{
	// response spectrum Load
//  int n = m_aAcdnLcomD.GetSize();
//  if (n<=0) return FALSE;
//  int LoadCaseNo = GetCountLcomCase(m_aAcdnLcomD.GetAt(n-1));

	CString strDesc = _T("");
	T_LCOM_D ELcomD; ELcomD.Initialize();  //SRSS
	int nEspCount = m_nEspCount;

	//Get SRSS of response spectrum Load
	for (int e = 0; e < nEspCount; e++)
	{
		Set_SpectrumCase(arEspLoad.GetAt(e), 1.0, e, ELcomD);
	}
	ELcomD.Description += _LSX(E(SRSS));
	Set_LoadComb4CivilNew(ELcomD, D_STR, 3, 104);//SRSS 

	int LoadCaseNo = 0;
	//D,CS(2),Ps,EH,EV,WP,B +- SRSS
	if (m_nDCount == 0 && m_nConstLoad == 0 && m_nPsCount == 0 && m_nEhCount == 0 && m_nEvCount == 0 && m_nWpCount == 0 && m_nBCount == 0) return TRUE;
	//D,EH,EV,WP,B
	T_LCOM_D LcomD; LcomD.Initialize();
	int i = 0, nDL = 0, nEV = 0, nEH = 0, nWP = 0, nB = 0, nPS = 0;
	CString strPos = _T("");  CString str1p2 = _T("1.0");  double dFact_1p2 = 1.;
	//D
	for (i = 0; i < m_nDCount; i++) nDL += Set_STLCIncludeCS(arDLoad.GetAt(i), dFact_1p2, LoadCaseNo, LcomD, D_SGLD_DL_CVL);
	if (nDL > 0) { LcomD.Description += strPos + str1p2 + _LSX(D); strPos = _LSX(+); }
	//CS(2)
	if (m_nConstLoad != 0) // CS-Tendon Secondary(CS)
	{
		int n = 0;
		if (CheckCS_SgldK(D_SGLD_TS_CVL, n)) // tendon secondary
		{
			double dPSS = m_bLossFactor ? m_dLossFactorService * dFact_1p2 : dFact_1p2;
			Set_CStageCase(m_aCSLoad.GetAt(n), dPSS, LoadCaseNo, LcomD, TRUE);
			LcomD.Description += strPos + GetFactorString(dPSS) + _T("(cTS)"); strPos = _LSX(+);
		}
		if (CheckCS_SgldK(D_SGLD_DL_CVL, n)) // Dead Load(CS)
		{
			double dPSS = m_bLossFactor ? m_dLossFactorService * dFact_1p2 : dFact_1p2;
			Set_CStageCase(m_aCSLoad.GetAt(n), dPSS, LoadCaseNo, LcomD, TRUE);
			LcomD.Description += strPos + GetFactorString(dPSS) + _T("(cD)"); strPos = _LSX(+);
		}
	}
	//PS
	for (i = 0; i < m_nPsCount; i++) nPS += Set_STLCIncludeCS(arPsLoad.GetAt(i), dFact_1p2, LoadCaseNo, LcomD, 0);
	if (nPS > 0) { LcomD.Description += strPos + str1p2 + _LSX(PS); strPos = _LSX(+); }
	//EH
	for (i = 0; i < m_nEhCount; i++) nEH += Set_STLCIncludeCS(arEhLoad.GetAt(i), dFact_1p2, LoadCaseNo, LcomD, 0);

	if (nEH > 0) { LcomD.Description += strPos + str1p2 + _LSX(EH); strPos = _LSX(+); }
	//EV
	for (i = 0; i < m_nEvCount; i++) nEV += Set_STLCIncludeCS(arEvLoad.GetAt(i), dFact_1p2, LoadCaseNo, LcomD, 0);
	if (nEV > 0) { LcomD.Description += strPos + str1p2 + _LSX(EV); strPos = _LSX(+); }
	//WP
	for (i = 0; i < m_nWpCount; i++) nWP += Set_STLCIncludeCS(arWpLoad.GetAt(i), dFact_1p2, LoadCaseNo, LcomD, 0);
	if (nWP > 0) { LcomD.Description += strPos + str1p2 + _LSX(WP); strPos = _LSX(+); }

	//B
	for (i = 0; i < m_nBCount; i++) nB += Set_STLCIncludeCS(arBLoad.GetAt(i), dFact_1p2, LoadCaseNo, LcomD, 0);
	if (nB > 0) { LcomD.Description += strPos + str1p2 + _LSX(B); strPos = _LSX(+); }

	//+SRSS
	T_LCOM_D LcomD1 = LcomD;
	int ncaseNo = LoadCaseNo;
	LcomD1.Description += strPos + str1p2 + ELcomD.LoadCombName;//+SRSS
	//  Set_SrssCase(ELcomD.LoadCombId, dFact_1p2, LoadCaseNo, LcomD1, 0);
	T_LCOM_BASE LcomBase;
	if (m_nLcomType == 1) LcomBase.AnalType = D_LCOM_CB_GENERAL;
	else                 LcomBase.AnalType = D_LCOM_CB_CONCRETE;
	LcomBase.Factor = 1.0;
	LcomBase.LoadCaseKey = ELcomD.LoadCombId;
	if (LcomD1.aCombination.GetSize() > LoadCaseNo) LcomD1.aCombination[LoadCaseNo] = LcomBase;
	else LcomD1.aCombination.Add(LcomBase);

	//  if(bInc)  iCaseNo++;
	Set_LoadComb4Civil(LcomD1, D_STR, 0, 104);
	//-SRSS
	T_LCOM_D LcomD2 = LcomD;
	LcomD2.Description += _LSX(-1.0) + ELcomD.LoadCombName;//-SRSS
	//  Set_SrssCase(ELcomD.LoadCombId, -1., ncaseNo, LcomD2, 0);

	if (m_nLcomType == 1) LcomBase.AnalType = D_LCOM_CB_GENERAL;
	else                 LcomBase.AnalType = D_LCOM_CB_CONCRETE;
	LcomBase.Factor = -1.0;
	LcomBase.LoadCaseKey = ELcomD.LoadCombId;
	if (LcomD2.aCombination.GetSize() > LoadCaseNo) LcomD2.aCombination[LoadCaseNo] = LcomBase;
	else LcomD2.aCombination.Add(LcomBase);

	Set_LoadComb4Civil(LcomD2, D_STR, 0, 104);

	return TRUE;
}

BOOL CLoadCombCtrl::SetCvlCJJ166_2011_Accident_Esp(ArUNT& arEspLoad, ArINT& arDLoad, ArINT& arPsLoad, ArINT& arEhLoad, ArINT& arEvLoad, ArINT& arWpLoad, ArINT& arBLoad, ArINT& arTLoad)
{
	// response spectrum Load
	//  int n = m_aAcdnLcomD.GetSize();
	//  if (n<=0) return FALSE;
	//  int LoadCaseNo = GetCountLcomCase(m_aAcdnLcomD.GetAt(n-1));

	CString strDesc = _T("");
	T_LCOM_D ELcomD; ELcomD.Initialize();  //SRSS
	int nEspCount = m_nEspCount;

	//Get SRSS of response spectrum Load
	for (int e = 0; e < nEspCount; e++)
	{
		Set_SpectrumCase(arEspLoad.GetAt(e), 1.0, e, ELcomD);
	}
	ELcomD.Description += _LSX(E(SRSS));
	Set_LoadComb4CivilNew(ELcomD, D_STR, 3, 104);//SRSS 


	int LoadCaseNo = 0;
	//D,CS(2),Ps,EH,EV,WP,B +- SRSS
	if (m_nDCount == 0 && m_nConstLoad == 0 && m_nPsCount == 0 && m_nEhCount == 0 && m_nEvCount == 0 && m_nWpCount == 0 && m_nBCount == 0) return TRUE;
	//D,EH,EV,WP,B
	T_LCOM_D LcomD; LcomD.Initialize();
	int i = 0, nDL = 0, nEV = 0, nEH = 0, nWP = 0, nB = 0, nPS = 0, nT = 0;
	CString strPos = _T("");  CString str1p2 = _T("1.0");  double dFact_1p2 = 1.;

	//D
	for (i = 0; i < m_nDCount; i++) nDL += Set_STLCIncludeCS(arDLoad.GetAt(i), dFact_1p2, LoadCaseNo, LcomD, D_SGLD_DL_CVL);
	if (nDL > 0) { LcomD.Description += strPos + str1p2 + _LSX(D); strPos = _LSX(+); }
	//CS(2)
	if (m_nConstLoad != 0) // CS-Tendon Secondary(CS)
	{
		int n = 0;
		if (CheckCS_SgldK(D_SGLD_TS_CVL, n)) // tendon secondary
		{
			double dPSS = m_bLossFactor ? m_dLossFactorService * dFact_1p2 : dFact_1p2;
			Set_CStageCase(m_aCSLoad.GetAt(n), dPSS, LoadCaseNo, LcomD, TRUE);
			LcomD.Description += strPos + GetFactorString(dPSS) + _T("(cTS)"); strPos = _LSX(+);
		}
		if (CheckCS_SgldK(D_SGLD_DL_CVL, n)) // Dead Load(CS)
		{
			double dPSS = m_bLossFactor ? m_dLossFactorService * dFact_1p2 : dFact_1p2;
			Set_CStageCase(m_aCSLoad.GetAt(n), dPSS, LoadCaseNo, LcomD, TRUE);
			LcomD.Description += strPos + GetFactorString(dPSS) + _T("(cD)"); strPos = _LSX(+);
		}
	}
	//PS
	for (i = 0; i < m_nPsCount; i++) nPS += Set_STLCIncludeCS(arPsLoad.GetAt(i), dFact_1p2, LoadCaseNo, LcomD, 0);
	if (nPS > 0) { LcomD.Description += strPos + str1p2 + _LSX(PS); strPos = _LSX(+); }
	//EH
	for (i = 0; i < m_nEhCount; i++) nEH += Set_STLCIncludeCS(arEhLoad.GetAt(i), dFact_1p2, LoadCaseNo, LcomD, 0);

	if (nEH > 0) { LcomD.Description += strPos + str1p2 + _LSX(EH); strPos = _LSX(+); }
	//EV
	for (i = 0; i < m_nEvCount; i++) nEV += Set_STLCIncludeCS(arEvLoad.GetAt(i), dFact_1p2, LoadCaseNo, LcomD, 0);
	if (nEV > 0) { LcomD.Description += strPos + str1p2 + _LSX(EV); strPos = _LSX(+); }
	//WP
	for (i = 0; i < m_nWpCount; i++) nWP += Set_STLCIncludeCS(arWpLoad.GetAt(i), dFact_1p2, LoadCaseNo, LcomD, 0);
	if (nWP > 0) { LcomD.Description += strPos + str1p2 + _LSX(WP); strPos = _LSX(+); }

	//B
	for (i = 0; i < m_nBCount; i++) nB += Set_STLCIncludeCS(arBLoad.GetAt(i), dFact_1p2, LoadCaseNo, LcomD, 0);
	if (nB > 0) { LcomD.Description += strPos + str1p2 + _LSX(B); strPos = _LSX(+); }

	/* //T
	 for(int j=0; j<m_nTCount; j++)
	 {
		 T_LCOM_D LcomD0 = LcomD; LoadCaseNo = GetCountLcomCase(LcomD);
		 nT += Set_STLCIncludeCS(arTLoad.GetAt(j), 0.5, LoadCaseNo, LcomD0, 0);
		 if(nT>0)
		 {
			 if(m_nTCount>1)
			 {
				 strDesc.Format(_T("0.5T[%d]"), j+1);
				 LcomD0.Description += strPos + strDesc; strPos = _LSX(+);
			 }
			 else
			 {
				 LcomD0.Description += strPos + _LSX(0.5T); strPos = _LSX(+);
			 }
		 }
		 //+SRSS
		 T_LCOM_D LcomD1 = LcomD0;
		 LcomD1.Description += strPos + str1p2 + ELcomD.LoadCombName;//+SRSS
		 //  Set_SrssCase(ELcomD.LoadCombId, dFact_1p2, LoadCaseNo, LcomD1, 0);
		 T_LCOM_BASE LcomBase;
		 if(m_nLcomType == 1) LcomBase.AnalType = D_LCOM_CB_GENERAL;
		 else                 LcomBase.AnalType = D_LCOM_CB_CONCRETE;
		 LcomBase.Factor				= 1.0;
		 LcomBase.LoadCaseKey	= ELcomD.LoadCombId;
		 if(LcomD1.aCombination.GetSize() > LoadCaseNo) LcomD1.aCombination[LoadCaseNo] = LcomBase;
		 else LcomD1.aCombination.Add(LcomBase);

		 //  if(bInc)  iCaseNo++;
		 Set_LoadComb4Civil(LcomD1, D_STR, 0, 104);

		 //-SRSS
		 T_LCOM_D LcomD2 = LcomD0;
		 LcomD2.Description += _LSX(-1.0) + ELcomD.LoadCombName;//-SRSS
		 //  Set_SrssCase(ELcomD.LoadCombId, -1., ncaseNo, LcomD2, 0);

		 if(m_nLcomType == 1) LcomBase.AnalType = D_LCOM_CB_GENERAL;
		 else                 LcomBase.AnalType = D_LCOM_CB_CONCRETE;
		 LcomBase.Factor				= -1.0;
		 LcomBase.LoadCaseKey	= ELcomD.LoadCombId;
		 if(LcomD2.aCombination.GetSize() > LoadCaseNo) LcomD2.aCombination[LoadCaseNo] = LcomBase;
		 else LcomD2.aCombination.Add(LcomBase);

		 Set_LoadComb4Civil(LcomD2, D_STR, 0, 104);
	 }

	 if(m_nTCount == 0)*/
	{
		//+SRSS
		T_LCOM_D LcomD1 = LcomD;
		int ncaseNo = LoadCaseNo;
		LcomD1.Description += strPos + str1p2 + ELcomD.LoadCombName;//+SRSS
		//  Set_SrssCase(ELcomD.LoadCombId, dFact_1p2, LoadCaseNo, LcomD1, 0);
		T_LCOM_BASE LcomBase;
		if (m_nLcomType == 1) LcomBase.AnalType = D_LCOM_CB_GENERAL;
		else                 LcomBase.AnalType = D_LCOM_CB_CONCRETE;
		LcomBase.Factor = 1.0;
		LcomBase.LoadCaseKey = ELcomD.LoadCombId;
		if (LcomD1.aCombination.GetSize() > LoadCaseNo) LcomD1.aCombination[LoadCaseNo] = LcomBase;
		else LcomD1.aCombination.Add(LcomBase);

		//  if(bInc)  iCaseNo++;
		Set_LoadComb4Civil(LcomD1, D_STR, 0, 104);
		//-SRSS
		T_LCOM_D LcomD2 = LcomD;
		LcomD2.Description += _LSX(-1.0) + ELcomD.LoadCombName;//-SRSS
		//  Set_SrssCase(ELcomD.LoadCombId, -1., ncaseNo, LcomD2, 0);

		if (m_nLcomType == 1) LcomBase.AnalType = D_LCOM_CB_GENERAL;
		else                 LcomBase.AnalType = D_LCOM_CB_CONCRETE;
		LcomBase.Factor = -1.0;
		LcomBase.LoadCaseKey = ELcomD.LoadCombId;
		if (LcomD2.aCombination.GetSize() > LoadCaseNo) LcomD2.aCombination[LoadCaseNo] = LcomBase;
		else LcomD2.aCombination.Add(LcomBase);

		Set_LoadComb4Civil(LcomD2, D_STR, 0, 104);
	}

	return TRUE;
}

BOOL CLoadCombCtrl::SetCvlJTJD60_Accident_Ct(ArINT& arCtLoad)
{
	// vessel collision load(Â÷·®Ãæµ¹ÇÏÁß)
	int LoadCaseNo = 0;
	CString strDesc = _T("");

	int nLcomSize = m_aAcdnLcomD.GetSize();
	for (int lc = 0; lc < nLcomSize; lc++)
	{
		for (int c = 0; c < m_nCtCount; c++)
		{
			T_LCOM_D LcomD = m_aAcdnLcomD.GetAt(lc); LoadCaseNo = GetCountLcomCase(LcomD);
			int nCT = Set_STLCIncludeCS(arCtLoad.GetAt(c), 1.0, LoadCaseNo, LcomD, 0);
			if (nCT > 0)
			{
				strDesc.Format(_T("+%sCT"), GetFactorString(1.0)); LcomD.Description += strDesc;
				if (m_nCtCount > 1) { strDesc.Format(_T("[%d]"), c + 1); LcomD.Description += strDesc; }
				Set_LoadComb4Civil(LcomD, D_STR, 0, 104); // ¿ì¿¬Á¶ÇÕ
			}
		}
	}

	return TRUE;
}
BOOL CLoadCombCtrl::SetCvlJTJD60_Accident_Cv(ArINT& arCvLoad)
{
	// vehicle collision load(¼±¹ÚÃæµ¹ÇÏÁß)
	int LoadCaseNo = 0;
	CString strDesc = _T("");

	int nLcomSize = m_aAcdnLcomD.GetSize();
	for (int lc = 0; lc < nLcomSize; lc++)
	{
		for (int c = 0; c < m_nCvCount; c++)
		{
			T_LCOM_D LcomD = m_aAcdnLcomD.GetAt(lc); LoadCaseNo = GetCountLcomCase(LcomD);
			int nCV = Set_STLCIncludeCS(arCvLoad.GetAt(c), 1.0, LoadCaseNo, LcomD, 0);
			if (nCV > 0)
			{
				strDesc.Format(_T("+%sCV"), GetFactorString(1.0)); LcomD.Description += strDesc;
				if (m_nCvCount > 1) { strDesc.Format(_T("[%d]"), c + 1); LcomD.Description += strDesc; }
				Set_LoadComb4Civil(LcomD, D_STR, 0, 104); // ¿ì¿¬Á¶ÇÕ
			}
		}
	}

	return TRUE;
}

double CLoadCombCtrl::CalcMovingILFactor()
{
	// Modify by GAY. MNET:2404. ('06.08.22). Move this method to _T("wg_db\CMvLoad class"). (To Use at wg_dgn prj.) 
	CMvLoad mvLoad;
	return mvLoad.CalcMovingImpactFactor();
	/*
	double dIL = 0;
	T_MVCD_D MvcdD; MvcdD.Initialize();
	if(!m_pDoc->m_pAttrCtrl->GetMvcd(MvcdD)) MvcdD.Initialize();
	if(MvcdD.nCodeType==D_MOVE_CODE_CHINA)
	{
		// Áß±¹ moving load codeÀÌ¾î¾ß ÇÏ°í
		T_MVCTch_D MvctD; MvctD.Initialize();
		if(m_pDoc->m_pAttrCtrl->GetMvctch(MvctD))
		{ // moving load analysis case°¡ Á¸ÀçÇØ¾ß ÇÏ°í
			if(MvctD.nCodeType==0) // JTG D60-2004      // JTG D60-2004ÀÌ¾î¾ß ÇÑ´Ù.
			{
				double dPI = 4.0*atan(1.0);
				double dFreq = 0.;
				switch(MvctD.nFrequencyType) // JTG D60(µµ·Î±³ÇÔ¼³°è¹ü¿ë½Ã¹æ¼­) 4.3.2, ºÎ·Ï A.0.1
				{
				case 0: dFreq = MvctD.FreqUser.df; break; // user
				case 1: // simple beam
					{
						dFreq =   (dPI/(2.*pow(MvctD.FreqSbem.dL,2)))
										* pow((MvctD.FreqSbem.dE*MvctD.FreqSbem.dIc/MvctD.FreqSbem.dmc), 0.5);
						break;
					}
				case 2: // continuous beam
					{
						dFreq =   (MvctD.FreqCbem.da/(2.*dPI*pow(MvctD.FreqCbem.dL,2)))
										* pow((MvctD.FreqCbem.dE*MvctD.FreqCbem.dIc/MvctD.FreqCbem.dmc), 0.5);
						break;
					}
				case 3: // arch brg
					{
						double df = MvctD.FreqArch.df, dw=0.;
						if(MvctD.FreqArch.nArchType==0) // Arch with Uniform Section
						{
							dw = 105. * (5.4 + 50.*pow(df, 2.))/(16.45 + 334.*pow(df, 2.) + 1867.*pow(df, 4.));
						}
						else // Arch with Tapered Section
						{
							double dn = MvctD.FreqArch.dn;
							double dr1 = 3.7*dn + 1.7, dr2 = 34.3*dn + 15.7, dr3 = 16.3*dn + 0.15;
							double dr4 = 364.*dn - 30., dr5 = 1955.*dn - 88.;
							dw = 105.*(dr1 + dr2*pow(df,2.))/(dr3 + dr4*pow(df, 2.) + dr5*pow(df,4.));
						}
						dFreq =   (dw/(2.*dPI*pow(MvctD.FreqArch.dL,2)))
										* pow((MvctD.FreqArch.dE*MvctD.FreqArch.dIc/MvctD.FreqArch.dmc), 0.5);
						break;
					}
				case 4: // cable stayed Brg
					{
						dFreq = MvctD.FreqCabl.da/MvctD.FreqCabl.dL;
						break;
					}
				case 5: // suspension Brg
					{
						double dtmp1 = MvctD.FreqSusp.dE*MvctD.FreqSusp.dI*pow(2.*dPI/MvctD.FreqSusp.dL,2);
						double dtmp2 = (dtmp1 + 2.*MvctD.FreqSusp.dHg)/MvctD.FreqSusp.dm;
						dFreq = (1./MvctD.FreqSusp.dL) * pow(dtmp2,0.5);
						break;
					}
				default: ASSERT(0); return FALSE;
				}
				if(dFreq < m_dZeroLim) dIL = 0.;
				else if(dFreq-1.5 < -m_dZeroLim) dIL = 0.05;
				else if(1.5-dFreq < m_dZeroLim && dFreq-14. < m_dZeroLim) dIL = 0.1767 * log(dFreq) - 0.0157;
				else dIL = 0.45;
				if(dIL<m_dZeroLim) dIL = 0.;
			}
		}
	}

	return dIL;
	*/
}

BOOL CLoadCombCtrl::SetCvlJTJD60_Service(const CArray<T_LCOM_D, T_LCOM_D&>& arData, ArUNT& arMLoad,
	ArINT& arLLoad, ArINT& arIlLoad, ArINT& arCrlLoad,
	ArINT& arCfLoad, ArINT& arLsLoad, ArINT& arWLoad, ArINT& arSfLoad,
	ArINT& arIpLoad, ArINT& arTLoad, ArINT& arTpgLoad, ArINT& arFrLoad,
	ArINT& arBrkLoad, BOOL bElast)
{
	CArray<T_LCOM_D, T_LCOM_D&> aLcomD; aLcomD.Copy(arData);
	int  nLcomSize = aLcomD.GetSize();
	int  nnn = 0;

	BOOL bErec = FALSE;
	for (int nErec = 0; nErec < D_STCT_EREC_MAX_CVL; ++nErec)
	{
		if (m_nConstLoad != 0 && CheckCS_SgldK(D_SGLD_ER1_CVL + nErec, nnn)) bErec = TRUE;
	}

	BOOL bAddCsLoad = bErec;
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER1_CVL,nnn)) || 
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER2_CVL,nnn)) || 
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER3_CVL,nnn)) ||
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER4_CVL,nnn)) ||
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER5_CVL,nnn)) ||
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER6_CVL,nnn)) ||
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER7_CVL,nnn)) ||
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER8_CVL,nnn)) ||
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER9_CVL,nnn)) ||
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER10_CVL,nnn));// CS or ST+CS;

	int nServEnvType = bElast ? 1 : 0;
	// ¿µ±¸ÇÏÁß¸¸ Á¶ÇÕ(Åº¼º´Ü°è¿¡¼­´Â »«´Ù.)
	if (!bElast)
	{
		for (int lc = 0; lc < nLcomSize; lc++)
		{
			T_LCOM_D LcomD = aLcomD.GetAt(lc);
			LcomD.nActive = 1; // active
			if (m_nLcomType == D_LCOMTYPE_CONCRETE) LcomD.nActive = 2;  // serviceability
			if (bElast) LcomD.bElasticStage = TRUE;
			Set_LoadComb4Civil(LcomD, D_SER, nServEnvType, 105); // serviceablilty, 105:±ØÇÑÁ¶ÇÕ(¿µ±¸ÇÏÁß)
		}
	}

	// Á¤»ó»ç¿ë ±ØÇÑ»óÅÂ(Moving Load¿¡ impact load°¡ Æ÷ÇÔµÇ¾î ÀÖÀ¸¹Ç·Î »«´Ù.
	double dIL = (bElast) ? 0. : CalcMovingILFactor();

	double dWFactor = bElast ? 1.0 : 0.75;
	double dTpgFactor = bElast ? 1.0 : 0.8;

	CString strDesc = _T("");
	int LoadCaseNo = 0, nc = 0;
	for (int lc = 0; lc < nLcomSize; lc++)
	{
		for (int i = 0; i < 2; i++) // i==0: ÀÛ¿ë´Ü±âÈ¿ÀÀÁ¶ÇÕ, i==1: ÀÛ¿ëÀå±âÈ¿ÀÀÁ¶ÇÕ
		{
			T_LCOM_D LcomD = aLcomD.GetAt(lc); LcomD.nActive = 1; // active
			if (m_nLcomType == D_LCOMTYPE_CONCRETE) LcomD.nActive = 2;  // serviceability
			if (bElast) LcomD.bElasticStage = TRUE;
			LoadCaseNo = GetCountLcomCase(LcomD);

			double dLFactor = i == 0 ? 0.7 : 0.4;
			int nMsgType = 108;
			if (bElast)
			{
				dLFactor = 1.0;
				nMsgType = 108; // 108:Åº¼º´Ü°èÀÀ·Â°ËÅäÁ¶ÇÕ
				if (i == 1) break;
			}
			else nMsgType = (i == 0) ? 107 : 106; // 107:´Ü±âÁ¶ÇÕ, 106:Àå±âÁ¶ÇÕ

			int nMCount = max(m_nMCount, 1);
			for (int m = 0; m < nMCount; m++)
			{
				T_LCOM_D LcomD_m = LcomD; LoadCaseNo = GetCountLcomCase(LcomD_m);
				// moving load
				if (m_nMCount > 0)
				{
					Set_MovingCase(arMLoad.GetAt(m), dLFactor / (1. + dIL), LoadCaseNo, LcomD_m, TRUE);
					if (bElast || dIL < m_dZeroLim) strDesc.Format(_T("+%sM"), GetFactorString(dLFactor));
					else												 strDesc.Format(_T("+%s/(1+mu)M"), GetFactorString(dLFactor));
					LcomD_m.Description += strDesc;
					if (m_nMCount > 1) { strDesc.Format(_T("[%d]"), m + 1); LcomD_m.Description += strDesc; }
				}
				int nLL = 0, nIL = 0, nCF = 0;
				if (m_nLCount > 0 || (bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(L)))) // ST or ST+CS
				{
					for (nc = 0; nc < m_nLCount; nc++) nLL += Set_STLCIncludeCS(arLLoad.GetAt(nc), dLFactor, LoadCaseNo, LcomD_m, 0);
					if (nLL > 0) { strDesc.Format(_T("+%sL"), GetFactorString(dLFactor)); LcomD_m.Description += strDesc; }
					if (bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(L)))
					{
						Set_CStageCase(m_aCSLoad.GetAt(nnn), dLFactor, LoadCaseNo, LcomD_m, TRUE);
						strDesc.Format(_T("+%s(cEL)"), GetFactorString(dLFactor)); LcomD_m.Description += strDesc;
					}
					if (bElast && m_nIlCount > 0)
					{
						for (nc = 0; nc < m_nIlCount; nc++) nIL += Set_STLCIncludeCS(arIlLoad.GetAt(nc), dLFactor, LoadCaseNo, LcomD_m, 0);
						if (nIL > 0) { strDesc.Format(_T("+%sIL"), GetFactorString(dLFactor)); LcomD_m.Description += strDesc; }
					}
				}

				if (m_nMCount > 0 || (m_nLCount > 0 || (bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(L)))))
				{
					for (nc = 0; nc < m_nCfCount; nc++) nCF += Set_STLCIncludeCS(arCfLoad.GetAt(nc), dLFactor, LoadCaseNo, LcomD_m, 0);
					if (nCF > 0) { strDesc.Format(_T("+%sCF"), GetFactorString(dLFactor)); LcomD_m.Description += strDesc; }
				}

				if (m_nLsCount > 0) // Live Load Surcharge
				{
					int nLS = 0;
					for (nc = 0; nc < m_nLsCount; nc++) nLS += Set_STLCIncludeCS(arLsLoad.GetAt(nc), 1., LoadCaseNo, LcomD_m, 0);
					if (nLS > 0) LcomD_m.Description += _T("+1.0LS");
				}
				if (m_nCrlCount > 0) // Crowd Load
				{
					double dCrlFactor = i == 0 ? 1.0 : 0.4;
					int nCRL = 0;
					for (nc = 0; nc < m_nCrlCount; nc++) nCRL += Set_STLCIncludeCS(arCrlLoad.GetAt(nc), dCrlFactor, LoadCaseNo, LcomD_m, 0);
					if (nCRL > 0) { strDesc.Format(_T("+%sCRL"), GetFactorString(dCrlFactor)); LcomD_m.Description += strDesc; }
				}
				for (int j = 0; j < 2; j++)
				{
					T_LCOM_D LcomD_j = LcomD_m; LoadCaseNo = GetCountLcomCase(LcomD_j);
					if (j == 0) // Braking Load
					{
						int nBRK = 0;
						for (nc = 0; nc < m_nBrkCount; nc++) nBRK += Set_STLCIncludeCS(arBrkLoad.GetAt(nc), 1., LoadCaseNo, LcomD_j, 0);
						if (nBRK > 0) LcomD_j.Description += _T("+1.0BRK");

						int nWCount = m_nWCount * 2; // MNET:1614, KJH20050808
						if (m_nWCount == 0) nWCount = 1;
						for (int w = 0; w < nWCount; w++)
						{
							T_LCOM_D LcomD_w = LcomD_j; LoadCaseNo = GetCountLcomCase(LcomD_w);
							if (m_nWCount > 0)
							{
								double dWF = (w % 2 == 0) ? dWFactor : -1.*dWFactor;
								if (Set_STLCIncludeCS(arWLoad.GetAt(w / 2), dWF, LoadCaseNo, LcomD_w, 0) > 0)
								{
									CString strW = (dWF > 0.0) ? _T("+%sW") : _T("%sW");
									strDesc.Format(strW, GetFactorString(dWF)); LcomD_w.Description += strDesc;
									if (m_nWCount > 1) { strDesc.Format(_T("[%d]"), w / 2 + 1); LcomD_w.Description += strDesc; }
								}
							}
							if (bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(W)))
							{
								Set_CStageCase(m_aCSLoad.GetAt(nnn), dWFactor, LoadCaseNo, LcomD_w, TRUE);
								strDesc.Format(_T("+%s(cEL)"), GetFactorString(dWFactor)); LcomD_w.Description += strDesc;
							}
							SetCvlJTJD60_Capacity_TPG(LcomD_w, arTLoad, arTpgLoad, arFrLoad, 1.0, dTpgFactor, FALSE, FALSE, nMsgType, D_SER, nServEnvType);
						}
					}
					else if (j == 1) // Wind Load
					{
						int nWCount = m_nWCount * 2; // MNET:1614, KJH20050808
						if ((m_nWCount == 0 && bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(W))) || (m_nSfCount > 0 || m_nIpCount > 0)) nWCount = 1;
						for (int w = 0; w < nWCount; w++)
						{
							T_LCOM_D LcomD_w = LcomD_j; LoadCaseNo = GetCountLcomCase(LcomD_w);
							if (m_nWCount > 0)
							{
								double dWF = (w % 2 == 0) ? dWFactor : -1.*dWFactor;
								if (Set_STLCIncludeCS(arWLoad.GetAt(w / 2), dWF, LoadCaseNo, LcomD_w, 0) > 0)
								{
									CString strW = (dWF > 0.0) ? _T("+%sW") : _T("%sW");
									strDesc.Format(strW, GetFactorString(dWF)); LcomD_w.Description += strDesc;
									if (m_nWCount > 1) { strDesc.Format(_T("[%d]"), w / 2 + 1); LcomD_w.Description += strDesc; }
								}
							}
							if (bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(W)))
							{
								Set_CStageCase(m_aCSLoad.GetAt(nnn), dWFactor, LoadCaseNo, LcomD_w, TRUE);
								strDesc.Format(_T("+%s(cEL)"), GetFactorString(dWFactor)); LcomD_w.Description += strDesc;
							}
							for (int k = 0; k < 2; k++)
							{
								T_LCOM_D LcomD_k = LcomD_w; LoadCaseNo = GetCountLcomCase(LcomD_k);
								if (k == 0 && m_nSfCount > 0)
								{
									int nSF = 0;
									for (nc = 0; nc < m_nSfCount; nc++) nSF += Set_STLCIncludeCS(arSfLoad.GetAt(nc), 1., LoadCaseNo, LcomD_k, 0);
									LcomD_k.Description += _T("+1.0SF");
								}
								else if (k == 1 && m_nIpCount > 0)
								{
									int nIP = 0;
									for (nc = 0; nc < m_nIpCount; nc++) nIP += Set_STLCIncludeCS(arIpLoad.GetAt(nc), 1., LoadCaseNo, LcomD_k, 0);
									LcomD_k.Description += _T("+1.0IP");
								}
								SetCvlJTJD60_Capacity_TPG(LcomD_k, arTLoad, arTpgLoad, arFrLoad, 1.0, dTpgFactor, TRUE, FALSE, nMsgType, D_SER, nServEnvType);
							}
						}
					}
				} // next j
			} // next m
		} // next i
	} // next lc

	return TRUE;
}

BOOL CLoadCombCtrl::SetCvlJTJD60_Service_New(const CArray<T_LCOM_D, T_LCOM_D&>& arData, ArUNT& arMLoad,
	ArINT& arLLoad, ArINT& arIlLoad, ArINT& arCrlLoad, ArINT& arMCrlLoad,
	ArINT& arCfLoad, ArINT& arLsLoad, ArINT& arWLoad, ArINT& arSfLoad, ArINT& arIpLoad,
	ArINT& arTLoad, ArINT& arTpgLoad, ArINT& arFrLoad, ArINT& arBrkLoad, int nType)
{
	// nType == 10:Àå±âÁ¶ÇÕ(±ØÇÑ»óÅÂ), 11:´Ü±âÁ¶ÇÕ(±ØÇÑ»óÅÂ), 12:Åº¼º´Ü°èÁ¶ÇÕ
	CArray<T_LCOM_D, T_LCOM_D&> aLcomD; aLcomD.Copy(arData);
	int  nLcomSize = aLcomD.GetSize();
	int  nnn = 0;

	BOOL bErec = FALSE;
	for (int nErec = 0; nErec < D_STCT_EREC_MAX_CVL; ++nErec)
	{
		if (m_nConstLoad != 0 && CheckCS_SgldK(D_SGLD_ER1_CVL + nErec, nnn)) bErec = TRUE;
	}

	BOOL bAddCsLoad = bErec;
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER1_CVL,nnn)) || 
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER2_CVL,nnn)) || 
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER3_CVL,nnn)) ||
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER4_CVL,nnn)) ||
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER5_CVL,nnn)) ||
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER6_CVL,nnn)) ||
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER7_CVL,nnn)) ||
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER8_CVL,nnn)) ||
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER9_CVL,nnn)) ||
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER10_CVL,nnn));// CS or ST+CS;

	int nServEnvType = (nType == 12) ? 1 : 0;
	// ¿µ±¸ÇÏÁß¸¸ Á¶ÇÕ(±ØÇÑ»óÅÂÀÇ ´Ü±âÁ¶ÇÕ¸¸¿¡¼­¸¸..)
	if (nType == 11)
	{
		for (int lc = 0; lc < nLcomSize; lc++)
		{
			T_LCOM_D LcomD = aLcomD.GetAt(lc);
			LcomD.nActive = 1; // Active or Strength/Stress
			if (m_nLcomType == D_LCOMTYPE_CONCRETE) LcomD.nActive = 2;  // serviceability
			Set_LoadComb4Civil(LcomD, D_SER, nServEnvType, 105); // serviceablilty, 105:±ØÇÑÁ¶ÇÕ(¿µ±¸ÇÏÁß)
		}
	}

	double dLFactor = 1.0, dCRLFactor = 1.0;
	double dFactor = 1.0, dTPGFactor = 1.0;
	double dWFactor = 1.0;
	if (nType == 10) { dLFactor = 0.4, dWFactor = 0.75, dTPGFactor = 0.8, dCRLFactor = 0.4; }
	else if (nType == 11) { dLFactor = 0.7, dWFactor = 0.75, dTPGFactor = 0.8; }

	int nMsgType = 108; // Åº¼º´Ü°è
	if (nType == 10) nMsgType = 106; // Àå±âÁ¶ÇÕ
	else if (nType == 11) nMsgType = 107; // ´Ü±âÁ¶ÇÕ

	double dIL = (nType == 12) ? 0. : CalcMovingILFactor();

	// ¿µ±¸ÇÏÁß + 1°³ÀÇ °¡º¯ÇÏÁßÀÎ °æ¿ì
	CString strDesc = _T("");
	int LoadCaseNo = 0, nc = 0;
	for (int lc = 0; lc < nLcomSize; lc++)
	{
		int nMCount = max(m_nMCount, 1);
		for (int m = 0; m < nMCount; m++)
		{
			T_LCOM_D LcomD = aLcomD.GetAt(lc); LcomD.nActive = 1; // Active or Strength/Stress
			if (m_nLcomType == D_LCOMTYPE_CONCRETE) LcomD.nActive = 2;  // serviceability
			if (nType == 12) LcomD.bElasticStage = TRUE;
			LoadCaseNo = GetCountLcomCase(LcomD);

			BOOL bAddLcomD = FALSE;
			// moving load
			if (m_nMCount > 0)
			{
				Set_MovingCase(arMLoad.GetAt(m), dLFactor / (1. + dIL), LoadCaseNo, LcomD, TRUE);
				if (nType == 12 || dIL < m_dZeroLim) strDesc.Format(_T("+%sM"), GetFactorString(dLFactor));
				else                            strDesc.Format(_T("+%s/(1+mu)M"), GetFactorString(dLFactor));
				LcomD.Description += strDesc;
				if (m_nMCount > 1) { strDesc.Format(_T("[%d]"), m + 1); LcomD.Description += strDesc; }
				bAddLcomD = TRUE;
			}
			int nLL = 0, nIL = 0, nCF = 0;
			if (m_nLCount > 0 || (bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(L)))) // ST or ST+CS
			{
				for (nc = 0; nc < m_nLCount; nc++) nLL += Set_STLCIncludeCS(arLLoad.GetAt(nc), dLFactor, LoadCaseNo, LcomD, 0);
				if (nLL > 0) { strDesc.Format(_T("+%sL"), GetFactorString(dLFactor)); LcomD.Description += strDesc; }
				if (bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(L)))
				{
					Set_CStageCase(m_aCSLoad.GetAt(nnn), dLFactor, LoadCaseNo, LcomD, TRUE);
					strDesc.Format(_T("+%s(cEL)"), GetFactorString(dLFactor)); LcomD.Description += strDesc;
					bAddLcomD = TRUE;
				}
				for (nc = 0; nc < m_nIlCount; nc++) nIL += Set_STLCIncludeCS(arIlLoad.GetAt(nc), dLFactor, LoadCaseNo, LcomD, 0);
				if (nIL > 0) { strDesc.Format(_T("+%sIL"), GetFactorString(dLFactor)); LcomD.Description += strDesc; }
				if (nLL + nIL > 0) bAddLcomD = TRUE;
			}
			if (m_nMCount > 0 || (m_nLCount > 0 || (bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(L)))))
			{
				for (nc = 0; nc < m_nCfCount; nc++) nCF += Set_STLCIncludeCS(arCfLoad.GetAt(nc), dLFactor, LoadCaseNo, LcomD, 0);
				if (nCF > 0) { strDesc.Format(_T("+%sCF"), GetFactorString(dLFactor)); LcomD.Description += strDesc; }
				bAddLcomD = TRUE;
			}
			if (bAddLcomD) Set_LoadComb4Civil(LcomD, D_SER, nServEnvType, nMsgType);

			if (m_nLsCount > 0) // Live Load Surcharge
			{
				LcomD = aLcomD.GetAt(lc); LcomD.nActive = 1; LoadCaseNo = GetCountLcomCase(LcomD);
				if (m_nLcomType == D_LCOMTYPE_CONCRETE) LcomD.nActive = 2;  // serviceability
				if (nType == 12) LcomD.bElasticStage = TRUE;
				int nLS = 0;
				for (nc = 0; nc < m_nLsCount; nc++) nLS += Set_STLCIncludeCS(arLsLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD, 0);
				if (nLS > 0)
				{
					strDesc.Format(_T("+%sLS"), GetFactorString(dFactor)); LcomD.Description += strDesc;
					Set_LoadComb4Civil(LcomD, D_SER, nServEnvType, nMsgType);
				}
			}
			// MNET:XXXX-BJLEE(BBONG)-20060503
			if (m_nCrlCount > 0 || m_nMCrlCount > 0) // Crowd Load
			{
				LcomD = aLcomD.GetAt(lc); LcomD.nActive = 1; LoadCaseNo = GetCountLcomCase(LcomD);
				if (m_nLcomType == D_LCOMTYPE_CONCRETE) LcomD.nActive = 2;  // serviceability
				if (nType == 12) LcomD.bElasticStage = TRUE;
				int nCRL = 0;
				for (nc = 0; nc < m_nCrlCount; nc++) nCRL += Set_STLCIncludeCS(arCrlLoad.GetAt(nc), dCRLFactor, LoadCaseNo, LcomD, 0);
				if (nCRL > 0) { strDesc.Format(_T("+%sCRL"), GetFactorString(dCRLFactor)); LcomD.Description += strDesc; }

				int nMCrlCount = (m_nMCrlCount > 0) ? m_nMCrlCount : ((nCRL > 0) ? 1 : 0);
				for (int crl = 0; crl < nMCrlCount; ++crl)
				{
					T_LCOM_D LcomD_crl = LcomD; LoadCaseNo = GetCountLcomCase(LcomD_crl);
					if (m_nMCrlCount > 0)
					{
						Set_MovingCase(arMCrlLoad.GetAt(crl), dCRLFactor, LoadCaseNo, LcomD_crl, TRUE);
						strDesc.Format(_T("+%sMCRL"), GetFactorString(dCRLFactor)); LcomD_crl.Description += strDesc;
						if (m_nMCrlCount > 1) { strDesc.Format(_T("[%d]"), crl + 1); LcomD_crl.Description += strDesc; }
					}
					if (nCRL > 0 || m_nMCrlCount > 0)
					{
						if (nType == 2 || nType == 4) m_aAcdnLcomD.Add(LcomD_crl);
						else                     Set_LoadComb4Civil(LcomD_crl, D_SER, nServEnvType, nMsgType);
					}
				}
			}


			int nWCount = m_nWCount * 2; // MNET:1614, KJH20050808
			if (m_nWCount == 0 && bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(W))) nWCount = 1;
			for (nc = 0; nc < nWCount; nc++)
			{
				LcomD = aLcomD.GetAt(lc); LcomD.nActive = 1; LoadCaseNo = GetCountLcomCase(LcomD);
				if (m_nLcomType == D_LCOMTYPE_CONCRETE) LcomD.nActive = 2;  // serviceability
				if (nType == 12) LcomD.bElasticStage = TRUE;
				if (m_nWCount > 0)
				{
					double dWF = (nc % 2 == 0) ? dWFactor : -1.*dWFactor;
					if (Set_STLCIncludeCS(arWLoad.GetAt(nc / 2), dWF, LoadCaseNo, LcomD, 0) > 0)
					{
						CString strW = (dWF > 0.0) ? _T("+%sW") : _T("%sW");
						strDesc.Format(strW, GetFactorString(dWF)); LcomD.Description += strDesc;
						if (m_nWCount > 1) { strDesc.Format(_T("[%d]"), nc / 2 + 1); LcomD.Description += strDesc; }
					}
				}
				if (bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(W)))
				{
					Set_CStageCase(m_aCSLoad.GetAt(nnn), dWFactor, LoadCaseNo, LcomD, TRUE);
					strDesc.Format(_T("+%s(cEL)"), GetFactorString(dWFactor)); LcomD.Description += strDesc;
				}
				Set_LoadComb4Civil(LcomD, D_SER, nServEnvType, nMsgType);
			}
			if (m_nSfCount > 0) // Stream Flow Pressure
			{
				LcomD = aLcomD.GetAt(lc); LcomD.nActive = 1; LoadCaseNo = GetCountLcomCase(LcomD);
				if (m_nLcomType == D_LCOMTYPE_CONCRETE) LcomD.nActive = 2;  // serviceability
				if (nType == 12) LcomD.bElasticStage = TRUE;
				int nSF = 0;
				for (nc = 0; nc < m_nSfCount; nc++) nSF += Set_STLCIncludeCS(arSfLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD, 0);
				if (nSF > 0)
				{
					strDesc.Format(_T("+%sSF"), GetFactorString(dFactor)); LcomD.Description += strDesc;
					Set_LoadComb4Civil(LcomD, D_SER, nServEnvType, nMsgType);
				}
			}
			if (m_nIpCount > 0) // Ice Pressure
			{
				LcomD = aLcomD.GetAt(lc); LcomD.nActive = 1; LoadCaseNo = GetCountLcomCase(LcomD);
				if (m_nLcomType == D_LCOMTYPE_CONCRETE) LcomD.nActive = 2;  // serviceability
				if (nType == 12) LcomD.bElasticStage = TRUE;
				int nIP = 0;
				for (nc = 0; nc < m_nIpCount; nc++) nIP += Set_STLCIncludeCS(arIpLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD, 0);
				if (nIP > 0)
				{
					strDesc.Format(_T("+%sIP"), GetFactorString(dFactor)); LcomD.Description += strDesc;
					Set_LoadComb4Civil(LcomD, D_SER, nServEnvType, nMsgType);
				}
			}
			// Temeperature
			if ((m_nTCount > 0 || m_nTpgCount > 0) || (bAddCsLoad && (CheckCSEL(D_SGLD_ER1_CVL, _LSX(T)) || CheckCSEL(D_SGLD_ER1_CVL, _LSX(TPG)))))
			{
				LcomD = aLcomD.GetAt(lc); LcomD.nActive = 1;
				if (m_nLcomType == D_LCOMTYPE_CONCRETE) LcomD.nActive = 2;  // serviceability
				if (nType == 12) LcomD.bElasticStage = TRUE;
				SetCvlJTJD60_Capacity_TPG(LcomD, arTLoad, arTpgLoad, arFrLoad, dFactor, dTPGFactor, FALSE, FALSE, nMsgType, D_SER, nServEnvType);
			}
			if (m_nFrCount > 0) // Friction
			{
				LcomD = aLcomD.GetAt(lc); LcomD.nActive = 1; LoadCaseNo = GetCountLcomCase(LcomD);
				if (m_nLcomType == D_LCOMTYPE_CONCRETE) LcomD.nActive = 2;  // serviceability
				if (nType == 12) LcomD.bElasticStage = TRUE;
				int nFR = 0;
				for (nc = 0; nc < m_nFrCount; nc++) nFR += Set_STLCIncludeCS(arFrLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD, 0);
				if (nFR > 0)
				{
					strDesc.Format(_T("+%sFR"), GetFactorString(dFactor)); LcomD.Description += strDesc;
					Set_LoadComb4Civil(LcomD, D_SER, nServEnvType, nMsgType);
				}
			}
		} // next m
	} // next lc

	// ¿µ±¸ÇÏÁß + Â÷·®ÇÏÁß + 1°³ÀÇ ±âÅ¸ °¡º¯ÇÏÁßÀÎ °æ¿ì(8°³)
	SetCvlJTJD60_Var1_S(arData, arMLoad, dIL, arLLoad, arIlLoad, arCrlLoad, arMCrlLoad, arCfLoad, arLsLoad, arWLoad, arSfLoad, arIpLoad,
		arTLoad, arTpgLoad, arFrLoad, arBrkLoad, nType, nMsgType);
	// ¿µ±¸ÇÏÁß + Â÷·®ÇÏÁß + 2°³ÀÇ ±âÅ¸ °¡º¯ÇÏÁßÀÎ °æ¿ì(24°³)
	SetCvlJTJD60_Var2_S(arData, arMLoad, dIL, arLLoad, arIlLoad, arCrlLoad, arMCrlLoad, arCfLoad, arLsLoad, arWLoad, arSfLoad, arIpLoad,
		arTLoad, arTpgLoad, arFrLoad, arBrkLoad, nType, nMsgType);
	// ¿µ±¸ÇÏÁß + Â÷·®ÇÏÁß + 3°³ÀÇ ±âÅ¸ °¡º¯ÇÏÁßÀÎ °æ¿ì(36°³)
	SetCvlJTJD60_Var3_S(arData, arMLoad, dIL, arLLoad, arIlLoad, arCrlLoad, arMCrlLoad, arCfLoad, arLsLoad, arWLoad, arSfLoad, arIpLoad,
		arTLoad, arTpgLoad, arFrLoad, arBrkLoad, nType, nMsgType);
	// ¿µ±¸ÇÏÁß + Â÷·®ÇÏÁß + 4°³ÀÇ ±âÅ¸ °¡º¯ÇÏÁßÀÎ °æ¿ì(29°³)
	SetCvlJTJD60_Var4_S(arData, arMLoad, dIL, arLLoad, arIlLoad, arCrlLoad, arMCrlLoad, arCfLoad, arLsLoad, arWLoad, arSfLoad, arIpLoad,
		arTLoad, arTpgLoad, arFrLoad, arBrkLoad, nType, nMsgType);
	// ¿µ±¸ÇÏÁß + Â÷·®ÇÏÁß + 5°³ ¹× 6°³ÀÇ ±âÅ¸ °¡º¯ÇÏÁßÀÎ °æ¿ì(12°³ ¹× 2°³)
	SetCvlJTJD60_Var56_S(arData, arMLoad, dIL, arLLoad, arIlLoad, arCrlLoad, arMCrlLoad, arCfLoad, arLsLoad, arWLoad, arSfLoad, arIpLoad,
		arTLoad, arTpgLoad, arFrLoad, arBrkLoad, nType, nMsgType);

	return TRUE;
}

BOOL CLoadCombCtrl::SetCvlJTJD60_Var1_S(const CArray<T_LCOM_D, T_LCOM_D&>& arData, ArUNT& arMLoad, double dIL,
	ArINT& arLLoad, ArINT& arIlLoad, ArINT& arCrlLoad, ArINT& arMCrlLoad,
	ArINT& arCfLoad, ArINT& arLsLoad, ArINT& arWLoad, ArINT& arSfLoad, ArINT& arIpLoad,
	ArINT& arTLoad, ArINT& arTpgLoad, ArINT& arFrLoad, ArINT& arBrkLoad, int nType, int nMsgType)
{
	// nType == 10:Àå±âÁ¶ÇÕ(±ØÇÑ»óÅÂ), 11:´Ü±âÁ¶ÇÕ(±ØÇÑ»óÅÂ), 12:Åº¼º´Ü°èÁ¶ÇÕ
	CArray<T_LCOM_D, T_LCOM_D&> aLcomD; aLcomD.Copy(arData);
	int  nLcomSize = aLcomD.GetSize();
	int  nnn = 0;

	BOOL bErec = FALSE;
	for (int nErec = 0; nErec < D_STCT_EREC_MAX_CVL; ++nErec)
	{
		if (m_nConstLoad != 0 && CheckCS_SgldK(D_SGLD_ER1_CVL + nErec, nnn)) bErec = TRUE;
	}

	BOOL bAddCsLoad = bErec;
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER1_CVL,nnn)) || 
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER2_CVL,nnn)) || 
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER3_CVL,nnn)) ||
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER4_CVL,nnn)) ||
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER5_CVL,nnn)) ||
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER6_CVL,nnn)) ||
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER7_CVL,nnn)) ||
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER8_CVL,nnn)) ||
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER9_CVL,nnn)) ||
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER10_CVL,nnn));// CS or ST+CS;

	double dFactor = 1.0, dLFactor = 1.0, dWFactor = 1.0, dCRLFactor = 1.0, dTPGFactor = 1.0;
	if (nType == 10) { dLFactor = 0.4, dWFactor = 0.75, dTPGFactor = 0.8, dCRLFactor = 0.4; }
	else if (nType == 11) { dLFactor = 0.7, dWFactor = 0.75, dTPGFactor = 0.8; }

	int nServEnvType = (nType == 12) ? 1 : 0;
	// ¿µ±¸ÇÏÁß + Â÷·®ÇÏÁß + 1°³ÀÇ °¡º¯ÇÏÁßÀÎ °æ¿ì
	CString strDesc = _T("");
	int LoadCaseNo = 0, nc = 0;
	for (int lc = 0; lc < nLcomSize; lc++)
	{
		int nMCount = max(m_nMCount, 1);
		for (int m = 0; m < nMCount; m++)
		{
			T_LCOM_D LcomD = aLcomD.GetAt(lc); LcomD.nActive = 1; // Active or Strength/Stress
			if (m_nLcomType == D_LCOMTYPE_CONCRETE) LcomD.nActive = 2;  // serviceability
			if (nType == 12) LcomD.bElasticStage = TRUE;
			T_LCOM_D LcomD_cf = LcomD; // MNET:1614, KJH20050809, cf°¡ Á¦¿ÜµÈ load combination
			LoadCaseNo = GetCountLcomCase(LcomD);

			BOOL bLive = FALSE, bCF = FALSE;
			BOOL bAddLcomD = FALSE;
			// moving load
			if (m_nMCount > 0)
			{
				Set_MovingCase(arMLoad.GetAt(m), dLFactor / (1. + dIL), LoadCaseNo, LcomD, TRUE);
				if (nType == 12 || dIL < m_dZeroLim) strDesc.Format(_T("+%sM"), GetFactorString(dLFactor));
				else                            strDesc.Format(_T("+%s/(1+mu)M"), GetFactorString(dLFactor));
				LcomD.Description += strDesc;
				if (m_nMCount > 1) { strDesc.Format(_T("[%d]"), m + 1); LcomD.Description += strDesc; }
				bAddLcomD = TRUE;
			}
			int nLL = 0, nIL = 0, nCF = 0;
			if (m_nLCount > 0 || (bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(L)))) // ST or ST+CS
			{
				for (nc = 0; nc < m_nLCount; nc++) nLL += Set_STLCIncludeCS(arLLoad.GetAt(nc), dLFactor, LoadCaseNo, LcomD, 0);
				if (nLL > 0) { strDesc.Format(_T("+%sL"), GetFactorString(dLFactor)); LcomD.Description += strDesc; }
				if (bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(L)))
				{
					Set_CStageCase(m_aCSLoad.GetAt(nnn), dLFactor, LoadCaseNo, LcomD, TRUE);
					strDesc.Format(_T("+%s(cEL)"), GetFactorString(dLFactor)); LcomD.Description += strDesc;
					bAddLcomD = TRUE;
				}
				for (nc = 0; nc < m_nIlCount; nc++) nIL += Set_STLCIncludeCS(arIlLoad.GetAt(nc), dLFactor, LoadCaseNo, LcomD, 0);
				if (nIL > 0) { strDesc.Format(_T("+%sIL"), GetFactorString(dLFactor)); LcomD.Description += strDesc; }
				if (nLL + nIL > 0) bAddLcomD = TRUE;
			}
			bLive = m_nMCount > 0 || (m_nLCount > 0 || (bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(L))));
			if (bLive)
			{
				LcomD_cf = LcomD;
				for (nc = 0; nc < m_nCfCount; nc++) nCF += Set_STLCIncludeCS(arCfLoad.GetAt(nc), dLFactor, LoadCaseNo, LcomD, 0);
				if (nCF > 0) { strDesc.Format(_T("+%sCF"), GetFactorString(dLFactor)); LcomD.Description += strDesc; bCF = TRUE; }
				bAddLcomD = TRUE;
			}
			if (bAddLcomD) Set_LoadComb4Civil(LcomD, D_SER, nServEnvType, nMsgType);

			if (m_nLsCount > 0) // Live Load Surcharge
			{
				T_LCOM_D LcomD_l = LcomD; LoadCaseNo = GetCountLcomCase(LcomD_l);
				int nLS = 0;
				for (nc = 0; nc < m_nLsCount; nc++) nLS += Set_STLCIncludeCS(arLsLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD_l, 0);
				if (nLS > 0)
				{
					strDesc.Format(_T("+%sLS"), GetFactorString(dFactor)); LcomD_l.Description += strDesc;
					Set_LoadComb4Civil(LcomD_l, D_SER, nServEnvType, nMsgType);
				}
			}
			// MNET:XXXX-BJLEE(BBONG)-20060502
			if (m_nCrlCount > 0 || m_nMCrlCount > 0) // Crowd Load
			{
				T_LCOM_D LcomD_l = LcomD; LoadCaseNo = GetCountLcomCase(LcomD_l);
				int nCRL = 0;
				for (nc = 0; nc < m_nCrlCount; nc++) nCRL += Set_STLCIncludeCS(arCrlLoad.GetAt(nc), dCRLFactor, LoadCaseNo, LcomD_l, 0);
				if (nCRL > 0) { strDesc.Format(_T("+%sCRL"), GetFactorString(dCRLFactor)); LcomD_l.Description += strDesc; }

				int nMCrlCount = (m_nMCrlCount > 0) ? m_nMCrlCount : ((nCRL > 0) ? 1 : 0);
				for (int crl = 0; crl < nMCrlCount; ++crl)
				{
					T_LCOM_D LcomD_crl = LcomD_l; LoadCaseNo = GetCountLcomCase(LcomD_crl);
					if (m_nMCrlCount > 0)
					{
						Set_MovingCase(arMCrlLoad.GetAt(crl), dCRLFactor, LoadCaseNo, LcomD_crl, TRUE);
						strDesc.Format(_T("+%sMCRL"), GetFactorString(dCRLFactor)); LcomD_crl.Description += strDesc;
						if (m_nMCrlCount > 1) { strDesc.Format(_T("[%d]"), crl + 1); LcomD_crl.Description += strDesc; }
					}
					if (nCRL > 0 || m_nMCrlCount > 0)
					{
						if (nType == 2 || nType == 4) m_aAcdnLcomD.Add(LcomD_crl);
						else                     Set_LoadComb4Civil(LcomD_crl, D_SER, nServEnvType, nMsgType);
					}
				}
			}

			if (m_nBrkCount > 0 && bLive) // Brake Load
			{
				T_LCOM_D LcomD_l = LcomD; LoadCaseNo = GetCountLcomCase(LcomD_l);
				double dBrkF = dFactor * 0.7;
				int nBRK = 0;
				for (nc = 0; nc < m_nBrkCount; nc++) nBRK += Set_STLCIncludeCS(arBrkLoad.GetAt(nc), dBrkF, LoadCaseNo, LcomD_l, 0);
				if (nBRK > 0)
				{
					strDesc.Format(_T("+%sBRK"), GetFactorString(dBrkF)); LcomD_l.Description += strDesc;
					Set_LoadComb4Civil(LcomD_l, D_SER, nServEnvType, nMsgType);
				}
				if (bCF)
				{
					LcomD_l = LcomD_cf; LoadCaseNo = GetCountLcomCase(LcomD_l);
					dBrkF = dFactor;
					for (nc = 0; nc < m_nBrkCount; nc++) nBRK += Set_STLCIncludeCS(arBrkLoad.GetAt(nc), dBrkF, LoadCaseNo, LcomD_l, 0);
					if (nBRK > 0)
					{
						strDesc.Format(_T("+%sBRK"), GetFactorString(dBrkF)); LcomD_l.Description += strDesc;
						Set_LoadComb4Civil(LcomD_l, D_SER, nServEnvType, nMsgType);
					}
				}
			}
			int nWCount = m_nWCount * 2; // MNET:1614, KJH20050808
			if (m_nWCount == 0 && bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(W))) nWCount = 1;
			for (nc = 0; nc < nWCount; nc++)
			{
				T_LCOM_D LcomD_l = LcomD; LoadCaseNo = GetCountLcomCase(LcomD_l);
				if (m_nWCount > 0)
				{
					double dWF = (nc % 2 == 0) ? dWFactor : -1.*dWFactor;
					if (Set_STLCIncludeCS(arWLoad.GetAt(nc / 2), dWF, LoadCaseNo, LcomD_l, 0) > 0)
					{
						CString strW = (dWF > 0.0) ? _T("+%sW") : _T("%sW");
						strDesc.Format(strW, GetFactorString(dWF)); LcomD_l.Description += strDesc;
						if (m_nWCount > 1) { strDesc.Format(_T("[%d]"), nc / 2 + 1); LcomD_l.Description += strDesc; }
					}
				}
				if (bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(W)))
				{
					Set_CStageCase(m_aCSLoad.GetAt(nnn), dWFactor, LoadCaseNo, LcomD_l, TRUE);
					strDesc.Format(_T("+%s(cEL)"), GetFactorString(dWFactor)); LcomD_l.Description += strDesc;
				}
				Set_LoadComb4Civil(LcomD_l, D_SER, nServEnvType, nMsgType);
			}
			if (m_nSfCount > 0) // Stream Flow Pressure
			{
				T_LCOM_D LcomD_l = LcomD; LoadCaseNo = GetCountLcomCase(LcomD_l);
				int nSF = 0;
				for (nc = 0; nc < m_nSfCount; nc++) nSF += Set_STLCIncludeCS(arSfLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD_l, 0);
				if (nSF > 0)
				{
					strDesc.Format(_T("+%sSF"), GetFactorString(dFactor)); LcomD_l.Description += strDesc;
					Set_LoadComb4Civil(LcomD_l, D_SER, nServEnvType, nMsgType);
				}
			}
			if (m_nIpCount > 0) // Ice Pressure
			{
				T_LCOM_D LcomD_l = LcomD; LoadCaseNo = GetCountLcomCase(LcomD_l);
				int nIP = 0;
				for (nc = 0; nc < m_nIpCount; nc++) nIP += Set_STLCIncludeCS(arIpLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD_l, 0);
				if (nIP > 0)
				{
					strDesc.Format(_T("+%sIP"), GetFactorString(dFactor)); LcomD_l.Description += strDesc;
					Set_LoadComb4Civil(LcomD_l, D_SER, nServEnvType, nMsgType);
				}
			}
			// Temeperature
			if ((m_nTCount > 0 || m_nTpgCount > 0) || (bAddCsLoad && (CheckCSEL(D_SGLD_ER1_CVL, _LSX(T)) || CheckCSEL(D_SGLD_ER1_CVL, _LSX(TPG)))))
			{
				T_LCOM_D LcomD_l = LcomD;
				SetCvlJTJD60_Capacity_TPG(LcomD_l, arTLoad, arTpgLoad, arFrLoad, dFactor, dTPGFactor, FALSE, FALSE, nMsgType, D_SER, nServEnvType);
			}
			if (m_nFrCount > 0) // Friction
			{
				T_LCOM_D LcomD_l = LcomD; LoadCaseNo = GetCountLcomCase(LcomD_l);
				int nFR = 0;
				for (nc = 0; nc < m_nFrCount; nc++) nFR += Set_STLCIncludeCS(arFrLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD_l, 0);
				if (nFR > 0)
				{
					strDesc.Format(_T("+%sFR"), GetFactorString(dFactor)); LcomD_l.Description += strDesc;
					Set_LoadComb4Civil(LcomD_l, D_SER, nServEnvType, nMsgType);
				}
			}
		} // next m
	} // next lc

	return TRUE;
}
BOOL CLoadCombCtrl::SetCvlJTJD60_Var2_S(const CArray<T_LCOM_D, T_LCOM_D&>& arData, ArUNT& arMLoad, double dIL,
	ArINT& arLLoad, ArINT& arIlLoad, ArINT& arCrlLoad, ArINT& arMCrlLoad,
	ArINT& arCfLoad, ArINT& arLsLoad, ArINT& arWLoad, ArINT& arSfLoad, ArINT& arIpLoad,
	ArINT& arTLoad, ArINT& arTpgLoad, ArINT& arFrLoad, ArINT& arBrkLoad, int nType, int nMsgType)
{
	// nType == 10:Àå±âÁ¶ÇÕ(±ØÇÑ»óÅÂ), 11:´Ü±âÁ¶ÇÕ(±ØÇÑ»óÅÂ), 12:Åº¼º´Ü°èÁ¶ÇÕ
	CArray<T_LCOM_D, T_LCOM_D&> aLcomD; aLcomD.Copy(arData);
	int  nLcomSize = aLcomD.GetSize();
	int  nnn = 0;

	BOOL bErec = FALSE;
	for (int nErec = 0; nErec < D_STCT_EREC_MAX_CVL; ++nErec)
	{
		if (m_nConstLoad != 0 && CheckCS_SgldK(D_SGLD_ER1_CVL + nErec, nnn)) bErec = TRUE;
	}

	BOOL bAddCsLoad = bErec;
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER1_CVL,nnn)) || 
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER2_CVL,nnn)) || 
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER3_CVL,nnn)) ||
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER4_CVL,nnn)) ||
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER5_CVL,nnn)) ||
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER6_CVL,nnn)) ||
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER7_CVL,nnn)) ||
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER8_CVL,nnn)) ||
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER9_CVL,nnn)) ||
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER10_CVL,nnn));// CS or ST+CS;

	double dFactor = 1.0, dLFactor = 1.0, dWFactor = 1.0, dCRLFactor = 1.0, dTPGFactor = 1.0;
	if (nType == 10) { dLFactor = 0.4, dWFactor = 0.75, dTPGFactor = 0.8, dCRLFactor = 0.4; }
	else if (nType == 11) { dLFactor = 0.7, dWFactor = 0.75, dTPGFactor = 0.8; }

	ArINT arTmp;

	int nServEnvType = (nType == 12) ? 1 : 0;
	// ¿µ±¸ÇÏÁß + Â÷·®ÇÏÁß + 2°³ÀÇ °¡º¯ÇÏÁßÀÎ °æ¿ì
	CString strDesc = _T("");
	int LoadCaseNo = 0, nc = 0;
	for (int lc = 0; lc < nLcomSize; lc++)
	{
		int nMCount = max(m_nMCount, 1);
		for (int m = 0; m < nMCount; m++)
		{
			T_LCOM_D LcomD = aLcomD.GetAt(lc); LcomD.nActive = 1; // Active or Strength/Stress
			if (m_nLcomType == D_LCOMTYPE_CONCRETE) LcomD.nActive = 2;  // serviceability
			if (nType == 12) LcomD.bElasticStage = TRUE;
			T_LCOM_D LcomD_cf = LcomD; // MNET:1614, KJH20050809, CF°¡ Á¦¿ÜµÈ load combination
			LoadCaseNo = GetCountLcomCase(LcomD);

			BOOL bLive = FALSE, bCF = FALSE;
			// moving load
			if (m_nMCount > 0)
			{
				Set_MovingCase(arMLoad.GetAt(m), dLFactor / (1. + dIL), LoadCaseNo, LcomD, TRUE);
				if (nType == 12 || dIL < m_dZeroLim) strDesc.Format(_T("+%sM"), GetFactorString(dLFactor));
				else                            strDesc.Format(_T("+%s/(1+mu)M"), GetFactorString(dLFactor));
				LcomD.Description += strDesc;
				if (m_nMCount > 1) { strDesc.Format(_T("[%d]"), m + 1); LcomD.Description += strDesc; }
			}
			int nLL = 0, nIL = 0, nCF = 0;
			if (m_nLCount > 0 || (bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(L)))) // ST or ST+CS
			{
				for (nc = 0; nc < m_nLCount; nc++) nLL += Set_STLCIncludeCS(arLLoad.GetAt(nc), dLFactor, LoadCaseNo, LcomD, 0);
				if (nLL > 0) { strDesc.Format(_T("+%sL"), GetFactorString(dLFactor)); LcomD.Description += strDesc; }
				if (bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(L)))
				{
					Set_CStageCase(m_aCSLoad.GetAt(nnn), dLFactor, LoadCaseNo, LcomD, TRUE);
					strDesc.Format(_T("+%s(cEL)"), GetFactorString(dLFactor)); LcomD.Description += strDesc;
				}
				for (nc = 0; nc < m_nIlCount; nc++) nIL += Set_STLCIncludeCS(arIlLoad.GetAt(nc), dLFactor, LoadCaseNo, LcomD, 0);
				if (nIL > 0) { strDesc.Format(_T("+%sIL"), GetFactorString(dLFactor)); LcomD.Description += strDesc; }
			}
			bLive = m_nMCount > 0 || (m_nLCount > 0 || (bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(L))));
			if (bLive)
			{
				LcomD_cf = LcomD;
				for (nc = 0; nc < m_nCfCount; nc++) nCF += Set_STLCIncludeCS(arCfLoad.GetAt(nc), dLFactor, LoadCaseNo, LcomD, 0);
				if (nCF > 0) { strDesc.Format(_T("+%sCF"), GetFactorString(dLFactor)); LcomD.Description += strDesc; bCF = TRUE; }
			}

			if (m_nLsCount > 0) // Live Load Surcharge
			{
				T_LCOM_D LcomD_l = LcomD; LoadCaseNo = GetCountLcomCase(LcomD_l);
				int nLS = 0;
				for (nc = 0; nc < m_nLsCount; nc++) nLS += Set_STLCIncludeCS(arLsLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD_l, 0);
				T_LCOM_D LcomD_tmp = LcomD_cf;
				if (bCF)
				{
					LoadCaseNo = GetCountLcomCase(LcomD_tmp);
					for (nc = 0; nc < m_nLsCount; nc++) Set_STLCIncludeCS(arLsLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD_tmp, 0);
				}
				if (nLS > 0)
				{
					strDesc.Format(_T("+%sLS"), GetFactorString(dFactor)); LcomD_l.Description += strDesc;
					if (bCF) LcomD_tmp.Description += strDesc;
					SetCvlJTJD60_ETC_S(LcomD_l, LcomD_tmp, bCF, arCrlLoad, arMCrlLoad, TRUE, arBrkLoad, bLive, arWLoad, TRUE, arSfLoad, TRUE,
						arIpLoad, TRUE, arTLoad, arTpgLoad, TRUE, arFrLoad, TRUE, 0, nMsgType);
				}
			}
			if (m_nCrlCount > 0 || m_nMCrlCount > 0) // Crowd Load
			{
				T_LCOM_D LcomD_l = LcomD; LoadCaseNo = GetCountLcomCase(LcomD_l);
				int nCRL = 0;
				for (nc = 0; nc < m_nCrlCount; nc++) nCRL += Set_STLCIncludeCS(arCrlLoad.GetAt(nc), dCRLFactor, LoadCaseNo, LcomD_l, 0);
				T_LCOM_D LcomD_tmp = LcomD_cf;
				if (bCF)
				{
					LoadCaseNo = GetCountLcomCase(LcomD_tmp);
					for (nc = 0; nc < m_nCrlCount; nc++) Set_STLCIncludeCS(arCrlLoad.GetAt(nc), dCRLFactor, LoadCaseNo, LcomD_tmp, 0);
				}
				// MNET:XXXX-BJLEE(BBONG)-20060502 
				if (nCRL > 0 || m_nMCrlCount > 0)
				{
					if (nCRL > 0)
					{
						strDesc.Format(_T("+%sCRL"), GetFactorString(dCRLFactor)); LcomD_l.Description += strDesc;
						if (bCF) LcomD_tmp.Description += strDesc;
					}

					int nMCrlCount = (m_nMCrlCount > 0) ? m_nMCrlCount : ((nCRL > 0) ? 1 : 0);
					T_LCOM_D LcomD_tmp2;
					for (int crl = 0; crl < nMCrlCount; ++crl)
					{
						T_LCOM_D LcomD_crl = LcomD_l; LoadCaseNo = GetCountLcomCase(LcomD_crl);
						LcomD_tmp2.Initialize();
						if (m_nMCrlCount > 0)
						{
							Set_MovingCase(arMCrlLoad.GetAt(crl), dCRLFactor, LoadCaseNo, LcomD_crl, TRUE);
							strDesc.Format(_T("+%sMCRL"), GetFactorString(dCRLFactor)); LcomD_crl.Description += strDesc;
							if (m_nMCrlCount > 1) { strDesc.Format(_T("[%d]"), crl + 1); LcomD_crl.Description += strDesc; }

							if (bCF)
							{
								LcomD_tmp2 = LcomD_tmp; LoadCaseNo = GetCountLcomCase(LcomD_tmp2);
								Set_MovingCase(arMCrlLoad.GetAt(crl), dCRLFactor, LoadCaseNo, LcomD_tmp2, TRUE);
								strDesc.Format(_T("+%sMCRL"), GetFactorString(dCRLFactor)); LcomD_tmp2.Description += strDesc;
								if (m_nMCrlCount > 1) { strDesc.Format(_T("[%d]"), crl + 1); LcomD_tmp2.Description += strDesc; }
							}
						}
						//if((nCRL>0 || m_nMCrlCount>0) && bCF)
						//{
						SetCvlJTJD60_ETC_S(LcomD_crl, LcomD_tmp2, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, bLive, arWLoad, TRUE, arSfLoad, TRUE, arIpLoad, TRUE,
							arTLoad, arTpgLoad, TRUE, arFrLoad, TRUE, 0, nMsgType);
						//}
					}
				}
			}

			if (m_nBrkCount > 0 && bLive) // Brake Load
			{
				T_LCOM_D LcomD_l = LcomD; LoadCaseNo = GetCountLcomCase(LcomD_l);
				double dBrkF = dFactor * 0.7;
				int nBRK = 0;
				for (nc = 0; nc < m_nBrkCount; nc++) nBRK += Set_STLCIncludeCS(arBrkLoad.GetAt(nc), dBrkF, LoadCaseNo, LcomD_l, 0);
				if (nBRK > 0)
				{
					strDesc.Format(_T("+%sBRK"), GetFactorString(dBrkF)); LcomD_l.Description += strDesc;
					SetCvlJTJD60_ETC_S(LcomD_l, LcomD_cf, FALSE, arTmp, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWLoad, TRUE, arSfLoad, FALSE, arIpLoad, FALSE,
						arTLoad, arTpgLoad, TRUE, arFrLoad, FALSE, 0, nMsgType);
				}
				if (bCF) // MNET:1614, KJH20060809, ¾Õ¿¡¼­ CF°¡ Á¸ÀçÇßÀ¸¸é, CF°¡ Á¦¿ÜµÈ ÇÏÁßÁ¶ÇÕÀ¸·Î ÇÑ¹ø´õ
				{
					LcomD_l = LcomD_cf; LoadCaseNo = GetCountLcomCase(LcomD_l);
					dBrkF = dFactor;
					nBRK = 0;
					for (nc = 0; nc < m_nBrkCount; nc++) nBRK += Set_STLCIncludeCS(arBrkLoad.GetAt(nc), dBrkF, LoadCaseNo, LcomD_l, 0);
					if (nBRK > 0)
					{
						strDesc.Format(_T("+%sBRK"), GetFactorString(dBrkF)); LcomD_l.Description += strDesc;
						SetCvlJTJD60_ETC_S(LcomD_l, LcomD_cf, FALSE, arTmp, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWLoad, TRUE, arSfLoad, FALSE, arIpLoad, FALSE,
							arTLoad, arTpgLoad, TRUE, arFrLoad, FALSE, 0, nMsgType);
					}
				}
			}
			int nWCount = m_nWCount * 2; // MNET:1614, KJH20050808
			if (m_nWCount == 0 && bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(W))) nWCount = 1;
			for (nc = 0; nc < nWCount; nc++)
			{
				T_LCOM_D LcomD_l = LcomD; LoadCaseNo = GetCountLcomCase(LcomD_l);
				if (m_nWCount > 0)
				{
					double dWF = (nc % 2 == 0) ? dWFactor : -1.*dWFactor;
					if (Set_STLCIncludeCS(arWLoad.GetAt(nc / 2), dWF, LoadCaseNo, LcomD_l, 0) > 0)
					{
						CString strW = (dWF > 0.0) ? _T("+%sW") : _T("%sW");
						strDesc.Format(strW, GetFactorString(dWF)); LcomD_l.Description += strDesc;
						if (m_nWCount > 1) { strDesc.Format(_T("[%d]"), nc / 2 + 1); LcomD_l.Description += strDesc; }
					}
				}
				if (bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(W)))
				{
					Set_CStageCase(m_aCSLoad.GetAt(nnn), dWFactor, LoadCaseNo, LcomD_l, TRUE);
					strDesc.Format(_T("+%s(cEL)"), GetFactorString(dWFactor)); LcomD_l.Description += strDesc;
				}
				SetCvlJTJD60_ETC_S(LcomD_l, LcomD_cf, bCF, arTmp, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWLoad, FALSE, arSfLoad, TRUE, arIpLoad, TRUE,
					arTLoad, arTpgLoad, TRUE, arFrLoad, TRUE, 0, nMsgType);
			}
			if (m_nSfCount > 0) // Stream Flow Pressure
			{
				T_LCOM_D LcomD_l = LcomD; LoadCaseNo = GetCountLcomCase(LcomD_l);
				int nSF = 0;
				for (nc = 0; nc < m_nSfCount; nc++) nSF += Set_STLCIncludeCS(arSfLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD_l, 0);
				if (nSF > 0)
				{
					strDesc.Format(_T("+%sSF"), GetFactorString(dFactor)); LcomD_l.Description += strDesc;
					SetCvlJTJD60_ETC_S(LcomD_l, LcomD_cf, bCF, arTmp, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWLoad, FALSE, arSfLoad, FALSE, arIpLoad, FALSE,
						arTLoad, arTpgLoad, TRUE, arFrLoad, TRUE, 0, nMsgType);
				}
			}
			if (m_nIpCount > 0) // Ice Pressure
			{
				T_LCOM_D LcomD_l = LcomD; LoadCaseNo = GetCountLcomCase(LcomD_l);
				int nIP = 0;
				for (nc = 0; nc < m_nIpCount; nc++) nIP += Set_STLCIncludeCS(arIpLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD_l, 0);
				if (nIP > 0)
				{
					strDesc.Format(_T("+%sIP"), GetFactorString(dFactor)); LcomD_l.Description += strDesc;
					SetCvlJTJD60_ETC_S(LcomD_l, LcomD_cf, bCF, arTmp, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWLoad, FALSE, arSfLoad, FALSE, arIpLoad, FALSE,
						arTLoad, arTpgLoad, TRUE, arFrLoad, TRUE, 0, nMsgType);
				}
			}
			if ((m_nTCount > 0 || m_nTpgCount > 0) || (bAddCsLoad && (CheckCSEL(D_SGLD_ER1_CVL, _LSX(T)) || CheckCSEL(D_SGLD_ER1_CVL, _LSX(TPG))))) // Temeperature
			{
				T_LCOM_D LcomD_l = LcomD;
				SetCvlJTJD60_Capacity_TPG(LcomD_l, arTLoad, arTpgLoad, arFrLoad, dFactor, dTPGFactor, TRUE, FALSE, nMsgType, D_SER, nServEnvType);
			}
		} // next m
	} // next lc

	return TRUE;
}
BOOL CLoadCombCtrl::SetCvlJTJD60_Var3_S(const CArray<T_LCOM_D, T_LCOM_D&>& arData, ArUNT& arMLoad, double dIL,
	ArINT& arLLoad, ArINT& arIlLoad, ArINT& arCrlLoad, ArINT& arMCrlLoad,
	ArINT& arCfLoad, ArINT& arLsLoad, ArINT& arWLoad, ArINT& arSfLoad, ArINT& arIpLoad,
	ArINT& arTLoad, ArINT& arTpgLoad, ArINT& arFrLoad, ArINT& arBrkLoad, int nType, int nMsgType)
{
	// nType == 10:Àå±âÁ¶ÇÕ(±ØÇÑ»óÅÂ), 11:´Ü±âÁ¶ÇÕ(±ØÇÑ»óÅÂ), 12:Åº¼º´Ü°èÁ¶ÇÕ
	CArray<T_LCOM_D, T_LCOM_D&> aLcomD; aLcomD.Copy(arData);
	int nLcomSize = aLcomD.GetSize();
	int  nnn = 0;

	BOOL bErec = FALSE;
	for (int nErec = 0; nErec < D_STCT_EREC_MAX_CVL; ++nErec)
	{
		if (m_nConstLoad != 0 && CheckCS_SgldK(D_SGLD_ER1_CVL + nErec, nnn)) bErec = TRUE;
	}

	BOOL bAddCsLoad = bErec;
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER1_CVL,nnn)) || 
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER2_CVL,nnn)) || 
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER3_CVL,nnn)) ||
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER4_CVL,nnn)) ||
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER5_CVL,nnn)) ||
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER6_CVL,nnn)) ||
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER7_CVL,nnn)) ||
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER8_CVL,nnn)) ||
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER9_CVL,nnn)) ||
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER10_CVL,nnn));// CS or ST+CS;

	double dFactor = 1.0, dLFactor = 1.0, dWFactor = 1.0, dCRLFactor = 1.0, dTPGFactor = 1.0;
	if (nType == 10) { dLFactor = 0.4, dWFactor = 0.75, dTPGFactor = 0.8, dCRLFactor = 0.4; }
	else if (nType == 11) { dLFactor = 0.7, dWFactor = 0.75, dTPGFactor = 0.8; }

	ArINT arTmp;

	int nServEnvType = (nType == 12) ? 1 : 0;
	// ¿µ±¸ÇÏÁß + Â÷·®ÇÏÁß + 3°³ÀÇ °¡º¯ÇÏÁßÀÎ °æ¿ì
	CString strDesc = _T("");
	int LoadCaseNo = 0, nc = 0;
	for (int lc = 0; lc < nLcomSize; lc++)
	{
		int nMCount = max(m_nMCount, 1);
		for (int m = 0; m < nMCount; m++)
		{
			T_LCOM_D LcomD = aLcomD.GetAt(lc); LcomD.nActive = 1; // Active or Strength/Stress
			if (m_nLcomType == D_LCOMTYPE_CONCRETE) LcomD.nActive = 2;  // serviceability
			if (nType == 12) LcomD.bElasticStage = TRUE;
			T_LCOM_D LcomD_cf = LcomD; // MNET:1614, KJH20050809, CF°¡ Á¦¿ÜµÈ load combination
			LoadCaseNo = GetCountLcomCase(LcomD);

			BOOL bLive = FALSE, bCF = FALSE;
			// moving load
			if (m_nMCount > 0)
			{
				Set_MovingCase(arMLoad.GetAt(m), dLFactor / (1. + dIL), LoadCaseNo, LcomD, TRUE);
				if (nType == 12 || dIL < m_dZeroLim) strDesc.Format(_T("+%sM"), GetFactorString(dLFactor));
				else                            strDesc.Format(_T("+%s/(1+mu)M"), GetFactorString(dLFactor));
				LcomD.Description += strDesc;
				if (m_nMCount > 1) { strDesc.Format(_T("[%d]"), m + 1); LcomD.Description += strDesc; }
			}
			int nLL = 0, nIL = 0, nCF = 0;
			if (m_nLCount > 0 || (bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(L)))) // ST or ST+CS
			{
				for (nc = 0; nc < m_nLCount; nc++) nLL += Set_STLCIncludeCS(arLLoad.GetAt(nc), dLFactor, LoadCaseNo, LcomD, 0);
				if (nLL > 0) { strDesc.Format(_T("+%sL"), GetFactorString(dLFactor)); LcomD.Description += strDesc; }
				if (bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(L)))
				{
					Set_CStageCase(m_aCSLoad.GetAt(nnn), dLFactor, LoadCaseNo, LcomD, TRUE);
					strDesc.Format(_T("+%s(cEL)"), GetFactorString(dLFactor)); LcomD.Description += strDesc;
				}
				for (nc = 0; nc < m_nIlCount; nc++) nIL += Set_STLCIncludeCS(arIlLoad.GetAt(nc), dLFactor, LoadCaseNo, LcomD, 0);
				if (nIL > 0) { strDesc.Format(_T("+%sIL"), GetFactorString(dLFactor)); LcomD.Description += strDesc; }
			}
			bLive = m_nMCount > 0 || (m_nLCount > 0 || (bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(L))));
			if (bLive)
			{
				LcomD_cf = LcomD;
				for (nc = 0; nc < m_nCfCount; nc++) nCF += Set_STLCIncludeCS(arCfLoad.GetAt(nc), dLFactor, LoadCaseNo, LcomD, 0);
				if (nCF > 0) { strDesc.Format(_T("+%sCF"), GetFactorString(dLFactor)); LcomD.Description += strDesc; bCF = TRUE; }
			}

			if (m_nLsCount > 0) // Live Load Surcharge
			{
				T_LCOM_D LcomD_l = LcomD; LoadCaseNo = GetCountLcomCase(LcomD_l);
				int nLS = 0;
				for (nc = 0; nc < m_nLsCount; nc++) nLS += Set_STLCIncludeCS(arLsLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD_l, 0);
				T_LCOM_D LcomD_tmp = LcomD_cf; LoadCaseNo = GetCountLcomCase(LcomD_tmp);
				if (bCF)
				{
					for (nc = 0; nc < m_nLsCount; nc++) Set_STLCIncludeCS(arLsLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD_tmp, 0);
				}
				if (nLS > 0)
				{
					strDesc.Format(_T("+%sLS"), GetFactorString(dFactor)); LcomD_l.Description += strDesc;
					if (bCF) LcomD_tmp.Description += strDesc;
					SetCvlJTJD60_ETC_S(LcomD_l, LcomD_tmp, bCF, arCrlLoad, arMCrlLoad, TRUE, arBrkLoad, bLive, arWLoad, TRUE, arSfLoad, TRUE, arIpLoad, TRUE,
						arTLoad, arTpgLoad, TRUE, arFrLoad, FALSE, 1, nMsgType);
				}
			}

			// MNET:XXXX-BJLEE(BBONG)-20060503
			if (m_nCrlCount > 0 || m_nMCrlCount > 0) // Crowd Load
			{
				T_LCOM_D LcomD_l = LcomD; LoadCaseNo = GetCountLcomCase(LcomD_l);
				int nCRL = 0;
				for (nc = 0; nc < m_nCrlCount; nc++) nCRL += Set_STLCIncludeCS(arCrlLoad.GetAt(nc), dCRLFactor, LoadCaseNo, LcomD_l, 0);
				T_LCOM_D LcomD_tmp = LcomD_cf; LoadCaseNo = GetCountLcomCase(LcomD_tmp);
				if (bCF)
				{
					for (nc = 0; nc < m_nCrlCount; nc++) Set_STLCIncludeCS(arCrlLoad.GetAt(nc), dCRLFactor, LoadCaseNo, LcomD_tmp, 0);
				}
				// MNET:XXXX-BJLEE(BBONG)-20060502 
				if (nCRL > 0 || m_nMCrlCount > 0)
				{
					if (nCRL > 0)
					{
						strDesc.Format(_T("+%sCRL"), GetFactorString(dCRLFactor)); LcomD_l.Description += strDesc;
						if (bCF) LcomD_tmp.Description += strDesc;
					}

					int nMCrlCount = (m_nMCrlCount > 0) ? m_nMCrlCount : ((nCRL > 0) ? 1 : 0);
					T_LCOM_D LcomD_tmp2;
					for (int crl = 0; crl < nMCrlCount; ++crl)
					{
						T_LCOM_D LcomD_crl = LcomD_l; LoadCaseNo = GetCountLcomCase(LcomD_crl);
						LcomD_tmp2.Initialize();
						if (m_nMCrlCount > 0)
						{
							Set_MovingCase(arMCrlLoad.GetAt(crl), dCRLFactor, LoadCaseNo, LcomD_crl, TRUE);
							strDesc.Format(_T("+%sMCRL"), GetFactorString(dCRLFactor)); LcomD_crl.Description += strDesc;
							if (m_nMCrlCount > 1) { strDesc.Format(_T("[%d]"), crl + 1); LcomD_crl.Description += strDesc; }

							if (bCF)
							{
								LcomD_tmp2 = LcomD_tmp; LoadCaseNo = GetCountLcomCase(LcomD_tmp2);
								Set_MovingCase(arMCrlLoad.GetAt(crl), dCRLFactor, LoadCaseNo, LcomD_tmp2, TRUE);
								strDesc.Format(_T("+%sMCRL"), GetFactorString(dCRLFactor)); LcomD_tmp2.Description += strDesc;
								if (m_nMCrlCount > 1) { strDesc.Format(_T("[%d]"), crl + 1); LcomD_tmp2.Description += strDesc; }
							}
						}
						//if((nCRL>0 || m_nMCrlCount>0) && bCF)
						//{
						SetCvlJTJD60_ETC_S(LcomD_crl, LcomD_tmp2, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, bLive, arWLoad, TRUE, arSfLoad, TRUE, arIpLoad, TRUE,
							arTLoad, arTpgLoad, TRUE, arFrLoad, FALSE, 2, nMsgType);
						//}
					}
				}
			}


			if (m_nBrkCount > 0 && bLive) // Brake Load
			{
				T_LCOM_D LcomD_l = LcomD; LoadCaseNo = GetCountLcomCase(LcomD_l);
				double dBrkF = dFactor * 0.7;
				int nBRK = 0;
				for (nc = 0; nc < m_nBrkCount; nc++) nBRK += Set_STLCIncludeCS(arBrkLoad.GetAt(nc), dBrkF, LoadCaseNo, LcomD_l, 0);
				if (nBRK > 0)
				{
					strDesc.Format(_T("+%sBRK"), GetFactorString(dBrkF)); LcomD_l.Description += strDesc;
					SetCvlJTJD60_ETC_S(LcomD_l, LcomD_cf, FALSE, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWLoad, TRUE, arSfLoad, FALSE, arIpLoad, FALSE,
						arTLoad, arTpgLoad, FALSE, arFrLoad, FALSE, 3, nMsgType);
				}
				if (bCF) // MNET:1614, KJH20050809, ¾Õ¿¡¼­ CF°¡ Á¸ÀçÇÏ¸é CF°¡ Á¦¿ÜµÈ ÇÏÁßÁ¶ÇÕ ÇÑ¹ø´õ...
				{
					LcomD_l = LcomD_cf; LoadCaseNo = GetCountLcomCase(LcomD_l);
					dBrkF = dFactor;
					int nBRK = 0;
					for (nc = 0; nc < m_nBrkCount; nc++) nBRK += Set_STLCIncludeCS(arBrkLoad.GetAt(nc), dBrkF, LoadCaseNo, LcomD_l, 0);
					if (nBRK > 0)
					{
						strDesc.Format(_T("+%sBRK"), GetFactorString(dBrkF)); LcomD_l.Description += strDesc;
						SetCvlJTJD60_ETC_S(LcomD_l, LcomD_cf, FALSE, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWLoad, TRUE, arSfLoad, FALSE, arIpLoad, FALSE,
							arTLoad, arTpgLoad, FALSE, arFrLoad, FALSE, 3, nMsgType);
					}
				}
			}
			int nWCount = m_nWCount * 2; // MNET:1614, KJH20050808
			if (m_nWCount == 0 && bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(W))) nWCount = 1;
			for (nc = 0; nc < nWCount; nc++)
			{
				T_LCOM_D LcomD_l = LcomD; LoadCaseNo = GetCountLcomCase(LcomD_l);
				if (m_nWCount > 0)
				{
					double dWF = (nc % 2 == 0) ? dWFactor : -1.*dWFactor;
					if (Set_STLCIncludeCS(arWLoad.GetAt(nc / 2), dWF, LoadCaseNo, LcomD_l, 0) > 0)
					{
						CString strW = (dWF > 0.0) ? _T("+%sW") : _T("%sW");
						strDesc.Format(strW, GetFactorString(dWF)); LcomD_l.Description += strDesc;
						if (m_nWCount > 1) { strDesc.Format(_T("[%d]"), nc / 2 + 1); LcomD_l.Description += strDesc; }
					}
				}
				if (bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(W)))
				{
					Set_CStageCase(m_aCSLoad.GetAt(nnn), dWFactor, LoadCaseNo, LcomD_l, TRUE);
					strDesc.Format(_T("+%s(cEL)"), GetFactorString(dWFactor)); LcomD_l.Description += strDesc;
				}
				SetCvlJTJD60_ETC_S(LcomD_l, LcomD_cf, bCF, arTmp, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWLoad, FALSE, arSfLoad, TRUE, arIpLoad, TRUE,
					arTLoad, arTpgLoad, TRUE, arFrLoad, FALSE, 4, nMsgType);
			}
			if (m_nSfCount > 0) // Stream Flow Pressure
			{
				T_LCOM_D LcomD_l = LcomD; LoadCaseNo = GetCountLcomCase(LcomD_l);
				int nSF = 0;
				for (nc = 0; nc < m_nSfCount; nc++) nSF += Set_STLCIncludeCS(arSfLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD_l, 0);
				if (nSF > 0)
				{
					strDesc.Format(_T("+%sSF"), GetFactorString(dFactor)); LcomD_l.Description += strDesc;
					SetCvlJTJD60_ETC_S(LcomD_l, LcomD_cf, bCF, arTmp, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWLoad, FALSE, arSfLoad, FALSE, arIpLoad, FALSE,
						arTLoad, arTpgLoad, TRUE, arFrLoad, FALSE, 5, nMsgType);
				}
			}
			if (m_nIpCount > 0) // Ice Pressure
			{
				T_LCOM_D LcomD_l = LcomD; LoadCaseNo = GetCountLcomCase(LcomD_l);
				int nIP = 0;
				for (nc = 0; nc < m_nIpCount; nc++) nIP += Set_STLCIncludeCS(arIpLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD_l, 0);
				if (nIP > 0)
				{
					strDesc.Format(_T("+%sIP"), GetFactorString(dFactor)); LcomD_l.Description += strDesc;
					SetCvlJTJD60_ETC_S(LcomD_l, LcomD_cf, bCF, arTmp, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWLoad, FALSE, arSfLoad, FALSE, arIpLoad, FALSE,
						arTLoad, arTpgLoad, TRUE, arFrLoad, FALSE, 5, nMsgType);
				}
			}
		} // next m
	} // next lc

	return TRUE;
}
BOOL CLoadCombCtrl::SetCvlJTJD60_Var4_S(const CArray<T_LCOM_D, T_LCOM_D&>& arData, ArUNT& arMLoad, double dIL,
	ArINT& arLLoad, ArINT& arIlLoad, ArINT& arCrlLoad, ArINT& arMCrlLoad,
	ArINT& arCfLoad, ArINT& arLsLoad, ArINT& arWLoad, ArINT& arSfLoad, ArINT& arIpLoad,
	ArINT& arTLoad, ArINT& arTpgLoad, ArINT& arFrLoad, ArINT& arBrkLoad, int nType, int nMsgType)
{
	// nType == 10:Àå±âÁ¶ÇÕ(±ØÇÑ»óÅÂ), 11:´Ü±âÁ¶ÇÕ(±ØÇÑ»óÅÂ), 12:Åº¼º´Ü°èÁ¶ÇÕ
	CArray<T_LCOM_D, T_LCOM_D&> aLcomD; aLcomD.Copy(arData);
	int  nLcomSize = aLcomD.GetSize();
	int  nnn = 0;

	BOOL bErec = FALSE;
	for (int nErec = 0; nErec < D_STCT_EREC_MAX_CVL; ++nErec)
	{
		if (m_nConstLoad != 0 && CheckCS_SgldK(D_SGLD_ER1_CVL + nErec, nnn)) bErec = TRUE;
	}

	BOOL bAddCsLoad = bErec;
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER1_CVL,nnn)) || 
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER2_CVL,nnn)) || 
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER3_CVL,nnn)) ||
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER4_CVL,nnn)) ||
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER5_CVL,nnn)) ||
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER6_CVL,nnn)) ||
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER7_CVL,nnn)) ||
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER8_CVL,nnn)) ||
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER9_CVL,nnn)) ||
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER10_CVL,nnn));// CS or ST+CS;

	double dFactor = 1.0, dLFactor = 1.0, dWFactor = 1.0, dCRLFactor = 1.0, dTPGFactor = 1.0;
	if (nType == 10) { dLFactor = 0.4, dWFactor = 0.75, dTPGFactor = 0.8, dCRLFactor = 0.4; }
	else if (nType == 11) { dLFactor = 0.7, dWFactor = 0.75, dTPGFactor = 0.8; }

	ArINT arTmp;

	int nServEnvType = (nType == 12) ? 1 : 0;
	// ¿µ±¸ÇÏÁß + Â÷·®ÇÏÁß + 4°³ÀÇ °¡º¯ÇÏÁßÀÎ °æ¿ì
	CString strDesc = _T("");
	int LoadCaseNo = 0, nc = 0;
	for (int lc = 0; lc < nLcomSize; lc++)
	{
		int nMCount = max(m_nMCount, 1);
		for (int m = 0; m < nMCount; m++)
		{
			T_LCOM_D LcomD = aLcomD.GetAt(lc); LcomD.nActive = 1; // Active or Strength/Stress
			if (m_nLcomType == D_LCOMTYPE_CONCRETE) LcomD.nActive = 2;  // serviceability
			if (nType == 12) LcomD.bElasticStage = TRUE;
			T_LCOM_D LcomD_cf = LcomD; // MNET:1614, KJH20050809, CF°¡ Á¦¿ÜµÈ ÇÏÁßÁ¶ÇÕ
			LoadCaseNo = GetCountLcomCase(LcomD);
			BOOL bLive = FALSE, bCF = FALSE;
			// moving load
			if (m_nMCount > 0)
			{
				Set_MovingCase(arMLoad.GetAt(m), dLFactor / (1. + dIL), LoadCaseNo, LcomD, TRUE);
				if (nType == 12 || dIL < m_dZeroLim) strDesc.Format(_T("+%sM"), GetFactorString(dLFactor));
				else                            strDesc.Format(_T("+%s/(1+mu)M"), GetFactorString(dLFactor));
				LcomD.Description += strDesc;
				if (m_nMCount > 1) { strDesc.Format(_T("[%d]"), m + 1); LcomD.Description += strDesc; }
			}
			int nLL = 0, nIL = 0, nCF = 0;
			if (m_nLCount > 0 || (bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(L)))) // ST or ST+CS
			{
				for (nc = 0; nc < m_nLCount; nc++) nLL += Set_STLCIncludeCS(arLLoad.GetAt(nc), dLFactor, LoadCaseNo, LcomD, 0);
				if (nLL > 0) { strDesc.Format(_T("+%sL"), GetFactorString(dLFactor)); LcomD.Description += strDesc; }
				if (bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(L)))
				{
					Set_CStageCase(m_aCSLoad.GetAt(nnn), dLFactor, LoadCaseNo, LcomD, TRUE);
					strDesc.Format(_T("+%s(cEL)"), GetFactorString(dLFactor)); LcomD.Description += strDesc;
				}
				for (nc = 0; nc < m_nIlCount; nc++) nIL += Set_STLCIncludeCS(arIlLoad.GetAt(nc), dLFactor, LoadCaseNo, LcomD, 0);
				if (nIL > 0) { strDesc.Format(_T("+%sIL"), GetFactorString(dLFactor)); LcomD.Description += strDesc; }
			}
			bLive = m_nMCount > 0 || (m_nLCount > 0 || (bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(L))));
			if (bLive)
			{
				LcomD_cf = LcomD;
				for (nc = 0; nc < m_nCfCount; nc++) nCF += Set_STLCIncludeCS(arCfLoad.GetAt(nc), dLFactor, LoadCaseNo, LcomD, 0);
				if (nCF > 0) { strDesc.Format(_T("+%sCF"), GetFactorString(dLFactor)); LcomD.Description += strDesc; bCF = TRUE; }
			}

			if (m_nLsCount > 0) // Live Load Surcharge
			{
				T_LCOM_D LcomD_l = LcomD; LoadCaseNo = GetCountLcomCase(LcomD_l);
				int nLS = 0;
				for (nc = 0; nc < m_nLsCount; nc++) nLS += Set_STLCIncludeCS(arLsLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD_l, 0);
				T_LCOM_D LcomD_tmp = LcomD_cf; LoadCaseNo = GetCountLcomCase(LcomD_tmp);
				if (bCF)
				{
					for (nc = 0; nc < m_nLsCount; nc++) Set_STLCIncludeCS(arLsLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD_tmp, 0);
				}
				if (nLS > 0)
				{
					strDesc.Format(_T("+%sLS"), GetFactorString(dFactor)); LcomD_l.Description += strDesc;
					if (bCF) LcomD_tmp.Description += strDesc;
					SetCvlJTJD60_ETC_S(LcomD_l, LcomD_tmp, bCF, arCrlLoad, arMCrlLoad, TRUE, arBrkLoad, bLive, arWLoad, TRUE, arSfLoad, TRUE, arIpLoad, TRUE,
						arTLoad, arTpgLoad, FALSE, arFrLoad, FALSE, 6, nMsgType);
				}
				/*
								if(bCF)
								{
									LcomD_l = LcomD_cf; LoadCaseNo = GetCountLcomCase(LcomD_l);
									nLS = 0;
									for(nc=0; nc<m_nLsCount; nc++) nLS += Set_STLCIncludeCS(arLsLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD_l, 0);
									if(nLS>0)
									{
										strDesc.Format(_T("+%sLS"), GetFactorString(dFactor)); LcomD_l.Description += strDesc;
										SetCvlJTJD60_ETC_S(LcomD_l, LcomD_cf, bCF, arCrlLoad, TRUE, arBrkLoad, bLive, arWLoad, TRUE, arSfLoad, FALSE, arIpLoad, FALSE,
																		 arTLoad, arTpgLoad, FALSE, arFrLoad, FALSE, 101, nMsgType);
									}
								}
				*/
			}

			// MNET:XXXX-BJLEE(BBONG)-20060503
			if (m_nCrlCount > 0 || m_nMCrlCount > 0) // Crowd Load
			{
				T_LCOM_D LcomD_l = LcomD; LoadCaseNo = GetCountLcomCase(LcomD_l);
				int nCRL = 0;
				for (nc = 0; nc < m_nCrlCount; nc++) nCRL += Set_STLCIncludeCS(arCrlLoad.GetAt(nc), dCRLFactor, LoadCaseNo, LcomD_l, 0);
				T_LCOM_D LcomD_tmp = LcomD_cf; LoadCaseNo = GetCountLcomCase(LcomD_tmp);
				if (bCF)
				{
					for (nc = 0; nc < m_nCrlCount; nc++) Set_STLCIncludeCS(arCrlLoad.GetAt(nc), dCRLFactor, LoadCaseNo, LcomD_tmp, 0);
				}
				// MNET:XXXX-BJLEE(BBONG)-20060502 
				if (nCRL > 0 || m_nMCrlCount > 0)
				{
					if (nCRL > 0)
					{
						strDesc.Format(_T("+%sCRL"), GetFactorString(dCRLFactor)); LcomD_l.Description += strDesc;
						if (bCF) LcomD_tmp.Description += strDesc;
					}

					int nMCrlCount = (m_nMCrlCount > 0) ? m_nMCrlCount : ((nCRL > 0) ? 1 : 0);
					T_LCOM_D LcomD_tmp2;
					for (int crl = 0; crl < nMCrlCount; ++crl)
					{
						T_LCOM_D LcomD_crl = LcomD_l; LoadCaseNo = GetCountLcomCase(LcomD_crl);
						LcomD_tmp2.Initialize();
						if (m_nMCrlCount > 0)
						{
							Set_MovingCase(arMCrlLoad.GetAt(crl), dCRLFactor, LoadCaseNo, LcomD_crl, TRUE);
							strDesc.Format(_T("+%sMCRL"), GetFactorString(dCRLFactor)); LcomD_crl.Description += strDesc;
							if (m_nMCrlCount > 1) { strDesc.Format(_T("[%d]"), crl + 1); LcomD_crl.Description += strDesc; }

							if (bCF)
							{
								LcomD_tmp2 = LcomD_tmp; LoadCaseNo = GetCountLcomCase(LcomD_tmp2);
								Set_MovingCase(arMCrlLoad.GetAt(crl), dCRLFactor, LoadCaseNo, LcomD_tmp2, TRUE);
								strDesc.Format(_T("+%sMCRL"), GetFactorString(dCRLFactor)); LcomD_tmp2.Description += strDesc;
								if (m_nMCrlCount > 1) { strDesc.Format(_T("[%d]"), crl + 1); LcomD_tmp2.Description += strDesc; }
							}
						}
						//if((nCRL>0 || m_nMCrlCount>0) && bCF)
						//{
						SetCvlJTJD60_ETC_S(LcomD_crl, LcomD_tmp2, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, bLive, arWLoad, TRUE, arSfLoad, TRUE, arIpLoad, TRUE,
							arTLoad, arTpgLoad, FALSE, arFrLoad, FALSE, 7, nMsgType);
						//}
					}
				}
			}


			int nWCount = m_nWCount * 2; // MNET:1614, KJH20050808
			if (m_nWCount == 0 && bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(W))) nWCount = 1;
			for (nc = 0; nc < nWCount; nc++)
			{
				T_LCOM_D LcomD_l = LcomD; LoadCaseNo = GetCountLcomCase(LcomD_l);
				if (m_nWCount > 0)
				{
					double dWF = (nc % 2 == 0) ? dWFactor : -1.*dWFactor;
					if (Set_STLCIncludeCS(arWLoad.GetAt(nc / 2), dWF, LoadCaseNo, LcomD_l, 0) > 0)
					{
						CString strW = (dWF > 0.0) ? _T("+%sW") : _T("%sW");
						strDesc.Format(strW, GetFactorString(dWF)); LcomD_l.Description += strDesc;
						if (m_nWCount > 1) { strDesc.Format(_T("[%d]"), nc / 2 + 1); LcomD_l.Description += strDesc; }
					}
				}
				if (bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(W)))
				{
					Set_CStageCase(m_aCSLoad.GetAt(nnn), dWFactor, LoadCaseNo, LcomD_l, TRUE);
					strDesc.Format(_T("+%s(cEL)"), GetFactorString(dWFactor)); LcomD_l.Description += strDesc;
				}
				SetCvlJTJD60_ETC_S(LcomD_l, LcomD_cf, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWLoad, FALSE, arSfLoad, TRUE, arIpLoad, TRUE,
					arTLoad, arTpgLoad, FALSE, arFrLoad, FALSE, 7, nMsgType);
			}
		} // next m
	} // next lc

	return TRUE;
}
BOOL CLoadCombCtrl::SetCvlJTJD60_Var56_S(const CArray<T_LCOM_D, T_LCOM_D&>& arData, ArUNT& arMLoad, double dIL,
	ArINT& arLLoad, ArINT& arIlLoad, ArINT& arCrlLoad, ArINT& arMCrlLoad,
	ArINT& arCfLoad, ArINT& arLsLoad, ArINT& arWLoad, ArINT& arSfLoad, ArINT& arIpLoad,
	ArINT& arTLoad, ArINT& arTpgLoad, ArINT& arFrLoad, ArINT& arBrkLoad, int nType, int nMsgType)
{
	// nType == 10:Àå±âÁ¶ÇÕ(±ØÇÑ»óÅÂ), 11:´Ü±âÁ¶ÇÕ(±ØÇÑ»óÅÂ), 12:Åº¼º´Ü°èÁ¶ÇÕ
	CArray<T_LCOM_D, T_LCOM_D&> aLcomD; aLcomD.Copy(arData);
	int  nLcomSize = aLcomD.GetSize();
	int  nnn = 0;

	BOOL bErec = FALSE;
	for (int nErec = 0; nErec < D_STCT_EREC_MAX_CVL; ++nErec)
	{
		if (m_nConstLoad != 0 && CheckCS_SgldK(D_SGLD_ER1_CVL + nErec, nnn)) bErec = TRUE;
	}

	BOOL bAddCsLoad = bErec;
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER1_CVL,nnn)) || 
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER2_CVL,nnn)) || 
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER3_CVL,nnn)) ||
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER4_CVL,nnn)) ||
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER5_CVL,nnn)) ||
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER6_CVL,nnn)) ||
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER7_CVL,nnn)) ||
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER8_CVL,nnn)) ||
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER9_CVL,nnn)) ||
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER10_CVL,nnn));// CS or ST+CS;

	double dFactor = 1.0, dLFactor = 1.0, dWFactor = 1.0, dCRLFactor = 1.0, dTPGFactor = 1.0;
	if (nType == 10) { dLFactor = 0.4, dWFactor = 0.75, dTPGFactor = 0.8, dCRLFactor = 0.4; }
	else if (nType == 11) { dLFactor = 0.7, dWFactor = 0.75, dTPGFactor = 0.8; }

	ArINT arTmp;

	int nServEnvType = (nType == 12) ? 1 : 0;
	// ¿µ±¸ÇÏÁß + Â÷·®ÇÏÁß + 5°³ÀÇ °¡º¯ÇÏÁßÀÎ °æ¿ì
	CString strDesc = _T("");
	int LoadCaseNo = 0, nc = 0;
	for (int lc = 0; lc < nLcomSize; lc++)
	{
		int nMCount = max(m_nMCount, 1);
		for (int m = 0; m < nMCount; m++)
		{
			T_LCOM_D LcomD = aLcomD.GetAt(lc); LcomD.nActive = 1; // Active or Strength/Stress
			if (m_nLcomType == D_LCOMTYPE_CONCRETE) LcomD.nActive = 2;  // serviceability
			if (nType == 12) LcomD.bElasticStage = TRUE;
			T_LCOM_D LcomD_cf = LcomD; // MNET:1614, KJH20050809, CFÁ¦¿ÜÇÑ ÇÏÁßÁ¶ÇÕ
			LoadCaseNo = GetCountLcomCase(LcomD);
			BOOL bLive = FALSE, bCF = FALSE;
			// moving load
			if (m_nMCount > 0)
			{
				Set_MovingCase(arMLoad.GetAt(m), dLFactor / (1. + dIL), LoadCaseNo, LcomD, TRUE);
				if (nType == 12 || dIL < m_dZeroLim) strDesc.Format(_T("+%sM"), GetFactorString(dLFactor));
				else                            strDesc.Format(_T("+%s/(1+mu)M"), GetFactorString(dLFactor));
				LcomD.Description += strDesc;
				if (m_nMCount > 1) { strDesc.Format(_T("[%d]"), m + 1); LcomD.Description += strDesc; }
			}
			int nLL = 0, nIL = 0, nCF = 0;
			if (m_nLCount > 0 || (bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(L)))) // ST or ST+CS
			{
				for (nc = 0; nc < m_nLCount; nc++) nLL += Set_STLCIncludeCS(arLLoad.GetAt(nc), dLFactor, LoadCaseNo, LcomD, 0);
				if (nLL > 0) { strDesc.Format(_T("+%sL"), GetFactorString(dLFactor)); LcomD.Description += strDesc; }
				if (bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(L)))
				{
					Set_CStageCase(m_aCSLoad.GetAt(nnn), dLFactor, LoadCaseNo, LcomD, TRUE);
					strDesc.Format(_T("+%s(cEL)"), GetFactorString(dLFactor)); LcomD.Description += strDesc;
				}
				for (nc = 0; nc < m_nIlCount; nc++) nIL += Set_STLCIncludeCS(arIlLoad.GetAt(nc), dLFactor, LoadCaseNo, LcomD, 0);
				if (nIL > 0) { strDesc.Format(_T("+%sIL"), GetFactorString(dLFactor)); LcomD.Description += strDesc; }
			}
			bLive = m_nMCount > 0 || (m_nLCount > 0 || (bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(L))));
			if (bLive)
			{
				LcomD_cf = LcomD;
				for (nc = 0; nc < m_nCfCount; nc++) nCF += Set_STLCIncludeCS(arCfLoad.GetAt(nc), dLFactor, LoadCaseNo, LcomD, 0);
				if (nCF > 0) { strDesc.Format(_T("+%sCF"), GetFactorString(dLFactor)); LcomD.Description += strDesc; bCF = TRUE; }
			}

			if (m_nLsCount > 0) // Live Load Surcharge
			{
				T_LCOM_D LcomD_l = LcomD; LoadCaseNo = GetCountLcomCase(LcomD_l);
				int nLS = 0;
				for (nc = 0; nc < m_nLsCount; nc++) nLS += Set_STLCIncludeCS(arLsLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD_l, 0);
				T_LCOM_D LcomD_tmp = LcomD_cf; LoadCaseNo = GetCountLcomCase(LcomD_tmp);
				if (bCF)
				{
					for (nc = 0; nc < m_nLsCount; nc++) Set_STLCIncludeCS(arLsLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD_tmp, 0);
				}
				if (nLS > 0)
				{
					strDesc.Format(_T("+%sLS"), GetFactorString(dFactor)); LcomD_l.Description += strDesc;
					if (bCF) LcomD_tmp.Description += strDesc;
					SetCvlJTJD60_ETC_S(LcomD_l, LcomD_tmp, bCF, arCrlLoad, arMCrlLoad, TRUE, arBrkLoad, bLive, arWLoad, TRUE, arSfLoad, FALSE, arIpLoad, FALSE,
						arTLoad, arTpgLoad, FALSE, arFrLoad, FALSE, 8, nMsgType);
				}
			}
			// MNET:XXXX-BJLEE(BBONG)-20060503
			if (m_nCrlCount > 0 || m_nMCrlCount > 0) // Crowd Load
			{
				T_LCOM_D LcomD_l = LcomD; LoadCaseNo = GetCountLcomCase(LcomD_l);
				int nCRL = 0;
				for (nc = 0; nc < m_nCrlCount; nc++) nCRL += Set_STLCIncludeCS(arCrlLoad.GetAt(nc), dCRLFactor, LoadCaseNo, LcomD_l, 0);
				if (nCRL > 0 || m_nMCrlCount > 0)
				{
					if (nCRL > 0)
					{
						strDesc.Format(_T("+%sCRL"), GetFactorString(dCRLFactor)); LcomD_l.Description += strDesc;
					}
					int nMCrlCount = (m_nMCrlCount > 0) ? m_nMCrlCount : ((nCRL > 0) ? 1 : 0);

					for (int crl = 0; crl < nMCrlCount; ++crl)
					{
						T_LCOM_D LcomD_tmp = LcomD_l; LoadCaseNo = GetCountLcomCase(LcomD_tmp);
						if (m_nMCrlCount > 0)
						{
							Set_MovingCase(arMCrlLoad.GetAt(crl), dCRLFactor, LoadCaseNo, LcomD_tmp, TRUE);
							strDesc.Format(_T("+%sMCRL"), GetFactorString(dCRLFactor)); LcomD_tmp.Description += strDesc;
							if (m_nMCrlCount > 1) { strDesc.Format(_T("[%d]"), crl + 1); LcomD_tmp.Description += strDesc; }
						}
						if ((nCRL > 0 || m_nMCrlCount > 0))
						{
							SetCvlJTJD60_ETC_S(LcomD_tmp, LcomD_cf, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWLoad, TRUE, arSfLoad, FALSE, arIpLoad, FALSE,
								arTLoad, arTpgLoad, FALSE, arFrLoad, FALSE, 9, nMsgType);
						}
					}
				}
			}
		} // next m
	} // next lc

	// ¿µ±¸ÇÏÁß + Â÷·®ÇÏÁß + 6°³ÀÇ °¡º¯ÇÏÁßÀÎ °æ¿ì
	for (int lc = 0; lc < nLcomSize; lc++)
	{
		int nMCount = max(m_nMCount, 1);
		for (int m = 0; m < nMCount; m++)
		{
			T_LCOM_D LcomD = aLcomD.GetAt(lc); LcomD.nActive = 1; // Active or Strength/Stress
			if (m_nLcomType == D_LCOMTYPE_CONCRETE) LcomD.nActive = 2;  // serviceability
			if (nType == 12) LcomD.bElasticStage = TRUE;
			LoadCaseNo = GetCountLcomCase(LcomD);

			// moving load
			if (m_nMCount > 0)
			{
				Set_MovingCase(arMLoad.GetAt(m), dLFactor / (1. + dIL), LoadCaseNo, LcomD, TRUE);
				if (nType == 12 || dIL < m_dZeroLim) strDesc.Format(_T("+%sM"), GetFactorString(dLFactor));
				else                            strDesc.Format(_T("+%s/(1+mu)M"), GetFactorString(dLFactor));
				LcomD.Description += strDesc;
				if (m_nMCount > 1) { strDesc.Format(_T("[%d]"), m + 1); LcomD.Description += strDesc; }
			}
			if (m_nLCount > 0 || (bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(L)))) // ST or ST+CS
			{
				int nLL = 0, nIL = 0, nCF = 0;
				for (nc = 0; nc < m_nLCount; nc++) nLL += Set_STLCIncludeCS(arLLoad.GetAt(nc), dLFactor, LoadCaseNo, LcomD, 0);
				if (nLL > 0) { strDesc.Format(_T("+%sL"), GetFactorString(dLFactor)); LcomD.Description += strDesc; }
				if (bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(L)))
				{
					Set_CStageCase(m_aCSLoad.GetAt(nnn), dLFactor, LoadCaseNo, LcomD, TRUE);
					strDesc.Format(_T("+%s(cEL)"), GetFactorString(dLFactor)); LcomD.Description += strDesc;
				}
				for (nc = 0; nc < m_nIlCount; nc++) nIL += Set_STLCIncludeCS(arIlLoad.GetAt(nc), dLFactor, LoadCaseNo, LcomD, 0);
				if (nIL > 0) { strDesc.Format(_T("+%sIL"), GetFactorString(dLFactor)); LcomD.Description += strDesc; }
				for (nc = 0; nc < m_nCfCount; nc++) nCF += Set_STLCIncludeCS(arCfLoad.GetAt(nc), dLFactor, LoadCaseNo, LcomD, 0);
				if (nCF > 0) { strDesc.Format(_T("+%sCF"), GetFactorString(dLFactor)); LcomD.Description += strDesc; }
			}

			if (m_nLsCount > 0) // Live Load Surcharge
			{
				T_LCOM_D LcomD_l = LcomD; LoadCaseNo = GetCountLcomCase(LcomD_l);
				int nLS = 0;
				for (nc = 0; nc < m_nLsCount; nc++) nLS += Set_STLCIncludeCS(arLsLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD_l, 0);
				if (nLS > 0)
				{
					strDesc.Format(_T("+%sLS"), GetFactorString(dFactor)); LcomD_l.Description += strDesc;
					SetCvlJTJD60_ETC_S(LcomD_l, LcomD_l, FALSE, arCrlLoad, arMCrlLoad, TRUE, arBrkLoad, FALSE, arWLoad, FALSE, arSfLoad, FALSE, arIpLoad, FALSE,
						arTLoad, arTpgLoad, FALSE, arFrLoad, FALSE, 10, nMsgType);
				}
			}
		}
	}

	return TRUE;
}

BOOL CLoadCombCtrl::SetCvlJTJD60_ETC_S(T_LCOM_D rData, T_LCOM_D Lcom_cf, BOOL bCF, ArINT& arCrlLoad, ArINT& arMCrlLoad, BOOL bCrl, ArINT& arBrkLoad, BOOL bBrk,
	ArINT& arWLoad, BOOL bW, ArINT& arSfLoad, BOOL bSf, ArINT& arIpLoad, BOOL bIp, ArINT& arTLoad,
	ArINT& arTpgLoad, BOOL bT, ArINT& arFrLoad, BOOL bFr, int nType, int nMsgType)
{
	double dFactor = 1.0, dWFactor = 1.0, dCRLFactor = 1.0, dTPGFactor = 1.0;
	if (nMsgType == 106) { dWFactor = 0.75, dTPGFactor = 0.8, dCRLFactor = 0.4; } // Àå±â
	else if (nMsgType == 107) { dWFactor = 0.75, dTPGFactor = 0.8; } // ´Ü±â

	int LoadCaseNo = 0, nc = 0, nnn = 0;
	CString strDesc = _T("");

	BOOL bErec = FALSE;
	for (int nErec = 0; nErec < D_STCT_EREC_MAX_CVL; ++nErec)
	{
		if (m_nConstLoad != 0 && CheckCS_SgldK(D_SGLD_ER1_CVL + nErec, nnn)) bErec = TRUE;
	}

	int nServEnvType = (nMsgType == 108) ? 1 : 0;
	BOOL bAddCsLoad = bErec;
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER1_CVL,nnn)) || 
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER2_CVL,nnn)) || 
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER3_CVL,nnn)) ||
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER4_CVL,nnn)) ||
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER5_CVL,nnn)) ||
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER6_CVL,nnn)) ||
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER7_CVL,nnn)) ||
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER8_CVL,nnn)) ||
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER9_CVL,nnn)) ||
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER10_CVL,nnn));// CS or ST+CS;

	if ((m_nCrlCount > 0 || m_nMCrlCount > 0) && bCrl)
	{
		T_LCOM_D LcomD = rData;
		LoadCaseNo = GetCountLcomCase(LcomD);
		int nCRL = 0;
		for (nc = 0; nc < m_nCrlCount; nc++) nCRL += Set_STLCIncludeCS(arCrlLoad.GetAt(nc), dCRLFactor, LoadCaseNo, LcomD, 0);
		T_LCOM_D LcomD_tmp = Lcom_cf;
		if (bCF && bBrk)
		{
			LoadCaseNo = GetCountLcomCase(LcomD_tmp);
			for (nc = 0; nc < m_nCrlCount; nc++) Set_STLCIncludeCS(arCrlLoad.GetAt(nc), dCRLFactor, LoadCaseNo, LcomD_tmp, 0);
		}
		if (nCRL > 0)
		{
			strDesc.Format(_T("+%sCRL"), GetFactorString(dCRLFactor)); LcomD.Description += strDesc;
			if (bCF && bBrk) LcomD_tmp.Description += strDesc;
		}
		int nMCrlCount = (m_nMCrlCount > 0) ? m_nMCrlCount : ((nCRL > 0) ? 1 : 0);
		for (int crl = 0; crl < nMCrlCount; ++crl)
		{
			T_LCOM_D LcomD_crl = LcomD;
			T_LCOM_D LcomD_crltmp = LcomD_tmp;
			if (m_nMCrlCount > 0)
			{
				Set_MovingCase(arMCrlLoad.GetAt(crl), dCRLFactor, LoadCaseNo, LcomD_crl, TRUE);
				strDesc.Format(_T("+%sMCRL"), GetFactorString(dCRLFactor)); LcomD_crl.Description += strDesc; LcomD_crltmp.Description += strDesc;
				if (m_nMCrlCount > 1) { strDesc.Format(_T("[%d]"), crl + 1); LcomD_crl.Description += strDesc; LcomD_crltmp.Description += strDesc; }
			}
			if (nCRL > 0 || m_nMCrlCount > 0)
			{
				if (nType == 1) SetCvlJTJD60_ETC_S(LcomD_crl, LcomD_crltmp, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, bBrk, arWLoad, TRUE, arSfLoad, TRUE, arIpLoad, TRUE,
					arTLoad, arTpgLoad, TRUE, arFrLoad, TRUE, 0, nMsgType);
				if (nType == 6) SetCvlJTJD60_ETC_S(LcomD_crl, LcomD_crltmp, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, bBrk, arWLoad, TRUE, arSfLoad, TRUE, arIpLoad, TRUE,
					arTLoad, arTpgLoad, TRUE, arFrLoad, FALSE, 2, nMsgType);
				if (nType == 8) SetCvlJTJD60_ETC_S(LcomD_crl, LcomD_crltmp, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, bBrk, arWLoad, TRUE, arSfLoad, TRUE, arIpLoad, TRUE,
					arTLoad, arTpgLoad, FALSE, arFrLoad, FALSE, 6, nMsgType);
				if (nType == 10) SetCvlJTJD60_ETC_S(LcomD_crl, LcomD_crltmp, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWLoad, TRUE, arSfLoad, FALSE, arIpLoad, FALSE,
					arTLoad, arTpgLoad, FALSE, arFrLoad, FALSE, 8, nMsgType);
				if (nType == 101) SetCvlJTJD60_ETC_S(LcomD_crl, LcomD_crltmp, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, bBrk, arWLoad, FALSE, arSfLoad, FALSE, arIpLoad, FALSE,
					arTLoad, arTpgLoad, FALSE, arFrLoad, FALSE, 102, nMsgType);
				if (nType == 103) SetCvlJTJD60_ETC_S(LcomD_crl, LcomD_crltmp, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, bBrk, arWLoad, TRUE, arSfLoad, FALSE, arIpLoad, FALSE,
					arTLoad, arTpgLoad, FALSE, arFrLoad, FALSE, 104, nMsgType);

				if (nType == 0) Set_LoadComb4Civil(LcomD_crl, D_SER, nServEnvType, nMsgType);
			}
		}
	}
	if (nType == 8) bBrk = FALSE;
	if (m_nBrkCount > 0 && bBrk)
	{
		for (int i = 0; i < 2; ++i)
		{
			if (i == 1 && !bCF) break;
			T_LCOM_D LcomD = (i == 0) ? rData : Lcom_cf;
			LoadCaseNo = GetCountLcomCase(LcomD);
			double dBrkF = (i == 0) ? 0.7 : 1.0;
			int nBRK = 0;
			for (nc = 0; nc < m_nBrkCount; nc++) nBRK += Set_STLCIncludeCS(arBrkLoad.GetAt(nc), dBrkF, LoadCaseNo, LcomD, 0);
			if (nBRK > 0) { strDesc.Format(_T("+%sBRK"), GetFactorString(dBrkF)); LcomD.Description += strDesc; }
			if (nType == 1) SetCvlJTJD60_ETC_S(LcomD, Lcom_cf, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWLoad, TRUE, arSfLoad, FALSE, arIpLoad, FALSE,
				arTLoad, arTpgLoad, TRUE, arFrLoad, FALSE, 0, nMsgType);
			if (nType == 2) SetCvlJTJD60_ETC_S(LcomD, Lcom_cf, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWLoad, TRUE, arSfLoad, FALSE, arIpLoad, FALSE,
				arTLoad, arTpgLoad, TRUE, arFrLoad, FALSE, 0, nMsgType);
			if (nType == 6) SetCvlJTJD60_ETC_S(LcomD, Lcom_cf, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWLoad, TRUE, arSfLoad, FALSE, arIpLoad, FALSE,
				arTLoad, arTpgLoad, FALSE, arFrLoad, FALSE, 3, nMsgType);
			if (nType == 7) SetCvlJTJD60_ETC_S(LcomD, Lcom_cf, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWLoad, TRUE, arSfLoad, FALSE, arIpLoad, FALSE,
				arTLoad, arTpgLoad, FALSE, arFrLoad, FALSE, 3, nMsgType);
			if (nType == 8) SetCvlJTJD60_ETC_S(LcomD, Lcom_cf, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWLoad, TRUE, arSfLoad, FALSE, arIpLoad, FALSE,
				arTLoad, arTpgLoad, FALSE, arFrLoad, FALSE, 3, nMsgType);
			if (nType == 101) SetCvlJTJD60_ETC_S(LcomD, Lcom_cf, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWLoad, TRUE, arSfLoad, FALSE, arIpLoad, FALSE,
				arTLoad, arTpgLoad, FALSE, arFrLoad, FALSE, 3, nMsgType);
			if (nType == 102) SetCvlJTJD60_ETC_S(LcomD, Lcom_cf, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWLoad, TRUE, arSfLoad, FALSE, arIpLoad, FALSE,
				arTLoad, arTpgLoad, TRUE, arFrLoad, FALSE, 0, nMsgType);
			if (nType == 103) SetCvlJTJD60_ETC_S(LcomD, Lcom_cf, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWLoad, TRUE, arSfLoad, FALSE, arIpLoad, FALSE,
				arTLoad, arTpgLoad, FALSE, arFrLoad, FALSE, 3, nMsgType);
			if (nType == 104) SetCvlJTJD60_ETC_S(LcomD, Lcom_cf, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWLoad, TRUE, arSfLoad, FALSE, arIpLoad, FALSE,
				arTLoad, arTpgLoad, FALSE, arFrLoad, FALSE, 3, nMsgType);

			if (nType == 0) Set_LoadComb4Civil(LcomD, D_SER, nServEnvType, nMsgType);
		}
	}
	if (bW)
	{
		int nWCount = m_nWCount * 2; // MNET:1614, KJH20050808
		if (m_nWCount == 0 && bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(W))) nWCount = 1;
		for (nc = 0; nc < nWCount; nc++)
		{
			T_LCOM_D LcomD = rData;
			LoadCaseNo = GetCountLcomCase(LcomD);
			int nWL = 0;
			if (m_nWCount > 0)
			{
				double dWF1 = (nc % 2 == 0) ? dWFactor : -1.*dWFactor;
				if (Set_STLCIncludeCS(arWLoad.GetAt(nc / 2), dWF1, LoadCaseNo, LcomD, 0) > 0)
				{
					CString strW = (dWF1 > 0.0) ? _T("+%sW") : _T("%sW");
					strDesc.Format(strW, GetFactorString(dWF1)); LcomD.Description += strDesc;
					if (m_nWCount > 1) { strDesc.Format(_T("[%d]"), nc / 2 + 1); LcomD.Description += strDesc; }
				}
			}
			if (bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(W)))
			{
				Set_CStageCase(m_aCSLoad.GetAt(nnn), dWFactor, LoadCaseNo, LcomD, TRUE);
				strDesc.Format(_T("+%s(cEL)"), GetFactorString(dWFactor)); LcomD.Description += strDesc;
			}
			if (nType == 1) SetCvlJTJD60_ETC_S(LcomD, Lcom_cf, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWLoad, FALSE, arSfLoad, TRUE, arIpLoad, TRUE,
				arTLoad, arTpgLoad, TRUE, arFrLoad, TRUE, 0, nMsgType);
			if (nType == 2) SetCvlJTJD60_ETC_S(LcomD, Lcom_cf, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWLoad, FALSE, arSfLoad, TRUE, arIpLoad, TRUE,
				arTLoad, arTpgLoad, TRUE, arFrLoad, TRUE, 0, nMsgType);
			if (nType == 3) SetCvlJTJD60_ETC_S(LcomD, Lcom_cf, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWLoad, FALSE, arSfLoad, FALSE, arIpLoad, FALSE,
				arTLoad, arTpgLoad, TRUE, arFrLoad, FALSE, 0, nMsgType);
			if (nType == 6) SetCvlJTJD60_ETC_S(LcomD, Lcom_cf, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWLoad, FALSE, arSfLoad, TRUE, arIpLoad, TRUE,
				arTLoad, arTpgLoad, TRUE, arFrLoad, FALSE, 2, nMsgType);
			if (nType == 7) SetCvlJTJD60_ETC_S(LcomD, Lcom_cf, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWLoad, FALSE, arSfLoad, TRUE, arIpLoad, TRUE,
				arTLoad, arTpgLoad, TRUE, arFrLoad, FALSE, 2, nMsgType);
			if (nType == 8) SetCvlJTJD60_ETC_S(LcomD, Lcom_cf, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWLoad, FALSE, arSfLoad, TRUE, arIpLoad, TRUE,
				arTLoad, arTpgLoad, FALSE, arFrLoad, FALSE, 6, nMsgType);
			if (nType == 9) SetCvlJTJD60_ETC_S(LcomD, Lcom_cf, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWLoad, FALSE, arSfLoad, TRUE, arIpLoad, TRUE,
				arTLoad, arTpgLoad, FALSE, arFrLoad, FALSE, 6, nMsgType);

			if (nType == 0) Set_LoadComb4Civil(LcomD, D_SER, nServEnvType, nMsgType);
		}
	}
	if (m_nSfCount > 0 && bSf)
	{
		T_LCOM_D LcomD = rData;
		LoadCaseNo = GetCountLcomCase(LcomD);
		int nSF = 0;
		for (nc = 0; nc < m_nSfCount; nc++) nSF += Set_STLCIncludeCS(arSfLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD, 0);
		if (nSF > 0) { strDesc.Format(_T("+%sSF"), GetFactorString(dFactor)); LcomD.Description += strDesc; }
		if (nType == 1) SetCvlJTJD60_ETC_S(LcomD, Lcom_cf, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWLoad, FALSE, arSfLoad, FALSE, arIpLoad, FALSE,
			arTLoad, arTpgLoad, TRUE, arFrLoad, TRUE, 0, nMsgType);
		if (nType == 2) SetCvlJTJD60_ETC_S(LcomD, Lcom_cf, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWLoad, FALSE, arSfLoad, FALSE, arIpLoad, FALSE,
			arTLoad, arTpgLoad, TRUE, arFrLoad, TRUE, 0, nMsgType);
		if (nType == 4) SetCvlJTJD60_ETC_S(LcomD, Lcom_cf, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWLoad, FALSE, arSfLoad, FALSE, arIpLoad, FALSE,
			arTLoad, arTpgLoad, TRUE, arFrLoad, TRUE, 0, nMsgType);
		if (nType == 6) SetCvlJTJD60_ETC_S(LcomD, Lcom_cf, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWLoad, FALSE, arSfLoad, FALSE, arIpLoad, FALSE,
			arTLoad, arTpgLoad, TRUE, arFrLoad, FALSE, 2, nMsgType);
		if (nType == 7) SetCvlJTJD60_ETC_S(LcomD, Lcom_cf, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWLoad, FALSE, arSfLoad, FALSE, arIpLoad, FALSE,
			arTLoad, arTpgLoad, TRUE, arFrLoad, FALSE, 2, nMsgType);

		if (nType == 0) Set_LoadComb4Civil(LcomD, D_SER, nServEnvType, nMsgType);
	}
	if (m_nIpCount > 0 && bIp)
	{
		T_LCOM_D LcomD = rData;
		LoadCaseNo = GetCountLcomCase(LcomD);
		int nIP = 0;
		for (nc = 0; nc < m_nIpCount; nc++) nIP += Set_STLCIncludeCS(arIpLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD, 0);
		if (nIP > 0) { strDesc.Format(_T("+%sIP"), GetFactorString(dFactor)); LcomD.Description += strDesc; }
		if (nType == 1) SetCvlJTJD60_ETC_S(LcomD, Lcom_cf, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWLoad, FALSE, arSfLoad, FALSE, arIpLoad, FALSE,
			arTLoad, arTpgLoad, TRUE, arFrLoad, TRUE, 0, nMsgType);
		if (nType == 2) SetCvlJTJD60_ETC_S(LcomD, Lcom_cf, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWLoad, FALSE, arSfLoad, FALSE, arIpLoad, FALSE,
			arTLoad, arTpgLoad, TRUE, arFrLoad, TRUE, 0, nMsgType);
		if (nType == 4) SetCvlJTJD60_ETC_S(LcomD, Lcom_cf, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWLoad, FALSE, arSfLoad, FALSE, arIpLoad, FALSE,
			arTLoad, arTpgLoad, TRUE, arFrLoad, TRUE, 0, nMsgType);
		if (nType == 6) SetCvlJTJD60_ETC_S(LcomD, Lcom_cf, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWLoad, FALSE, arSfLoad, FALSE, arIpLoad, FALSE,
			arTLoad, arTpgLoad, TRUE, arFrLoad, FALSE, 2, nMsgType);
		if (nType == 7) SetCvlJTJD60_ETC_S(LcomD, Lcom_cf, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWLoad, FALSE, arSfLoad, FALSE, arIpLoad, FALSE,
			arTLoad, arTpgLoad, TRUE, arFrLoad, FALSE, 2, nMsgType);

		if (nType == 0) Set_LoadComb4Civil(LcomD, D_SER, nServEnvType, nMsgType);
	}
	if (bT && ((m_nTCount > 0 || m_nTpgCount > 0) || (bAddCsLoad && (CheckCSEL(D_SGLD_ER1_CVL, _LSX(T)) || CheckCSEL(D_SGLD_ER1_CVL, _LSX(TPG))))))
	{
		T_LCOM_D LcomD = rData;
		if (nType == 0) SetCvlJTJD60_Capacity_TPG(LcomD, arTLoad, arTpgLoad, arFrLoad, dFactor, dTPGFactor, FALSE, FALSE, nMsgType, D_SER, nServEnvType);
		else         SetCvlJTJD60_Capacity_TPG(LcomD, arTLoad, arTpgLoad, arFrLoad, dFactor, dTPGFactor, TRUE, FALSE, nMsgType, D_SER, nServEnvType);
	}
	if (m_nFrCount > 0 && bFr)
	{
		T_LCOM_D LcomD = rData;
		LoadCaseNo = GetCountLcomCase(LcomD);
		int nFR = 0;
		for (nc = 0; nc < m_nFrCount; nc++) nFR += Set_STLCIncludeCS(arFrLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD, 0);
		if (nFR > 0)
		{
			strDesc.Format(_T("+%sFR"), GetFactorString(dFactor)); LcomD.Description += strDesc;
			if (nType == 0) Set_LoadComb4Civil(LcomD, D_SER, nServEnvType, nMsgType);
		}
	}
	return TRUE;
}
#pragma endregion
//////////////////////////////////////////////////////////////////////////////
#pragma region lcom_jtgd6015
////////////////////////////////////////////////////////////////////////////// 
//JTG D60-2015
CString CLoadCombCtrl::GetJTGD602015MILoadName(const ArUNT& arMLoad, int m) const
{
	CString strJudgeMILFactorByLoadCase = _T("");
	if (m_nMCount > 0 && m < m_nMCount)
	{
		auto itrFindMVLcasType = m_mapJTGd6015MlLoad.find(arMLoad[m]);
		if (itrFindMVLcasType != m_mapJTGd6015MlLoad.end())
		{
			strJudgeMILFactorByLoadCase = itrFindMVLcasType->second;
		}//end if
	}//end if

	return strJudgeMILFactorByLoadCase;
}
BOOL CLoadCombCtrl::Set_CvlJTGD60_15_Permanent(ArINT& arDLoad, ArINT& arPsLoad, ArINT& arEvLoad, ArINT& arEhLoad, ArINT& arShLoad, ArINT& arCrLoad, ArINT& arBLoad, ArINT& arStlLoad, ArUNT& arSmLoad, int nType, CArray<T_LCOM_D, T_LCOM_D&>& arData)
{
	// nType  0 or 4:×éºÏÏµÊý, 1 or 2: 1.0(settlement ³ýÍâ), 3: 1.0(ÒÔ Ï¾?
	arData.RemoveAll();

	if (m_nConstLoad == 0 && m_nDCount == 0 && m_nPsCount == 0 && m_nEvCount == 0 && m_nEhCount == 0 &&
		m_nShCount == 0 && m_nCrCount == 0 && m_nBCount == 0 && m_nStlCount == 0 && m_nSmCount == 0) return FALSE;

	int LoadCaseNo = 0;
	int nSmCount = max(m_nSmCount, 1);

	double dFact_1p2 = (nType == 0) ? 1.2 : 1.;
	double dFact_1p4 = (nType == 0) ? 1.4 : 1.;
	double dFact_0p5 = (nType == 3 || nType == 4 || nType == 17) ? 1.0 : 0.5;
	if ((m_nLcomType == D_LCOMTYPE_STLCOMP || m_nLcomType == D_LCOMTYPE_SRC) && (nType == 0 || nType == 1))
	{
		dFact_0p5 = 1.0;
	}
	if (m_nDesignType == 0) dFact_0p5 = 1.0;

	CString str1p2 = (nType == 0) ? _T("1.2") : _T("1.0");
	CString str1p4 = (nType == 0) ? _T("1.4") : _T("1.0");
	CString str0p5 = (nType == 3) ? _T("1.0") : _T("0.5");
	if (m_nDesignType == 0) str0p5 = _T("1.0");

	for (int sm = 0; sm < nSmCount; sm++)
	{
		T_LCOM_D LcomD; LcomD.Initialize();
		LoadCaseNo = 0;
		CString strPos = _T("");
		int i = 0, nDL = 0, nPS = 0, nEV = 0, nEH = 0, nSH = 0, nCR = 0, nBL = 0, nSTL = 0;
		for (i = 0; i < m_nDCount; i++) nDL += Set_STLCIncludeCS(arDLoad.GetAt(i), dFact_1p2, LoadCaseNo, LcomD, D_SGLD_DL_CVL);
		if (nDL > 0) { LcomD.Description += strPos + str1p2 + _LSX(D); strPos = _LSX(+); }
		for (i = 0; i < m_nPsCount; i++) nPS += Set_STLCIncludeCS(arPsLoad.GetAt(i), dFact_1p2, LoadCaseNo, LcomD, D_SGLD_TS_CVL);
		if (nPS > 0) { LcomD.Description += strPos + str1p2 + _LSX(PS); strPos = _LSX(+); }
		for (i = 0; i < m_nEvCount; i++) nEV += Set_STLCIncludeCS(arEvLoad.GetAt(i), dFact_1p2, LoadCaseNo, LcomD, 0);
		if (nEV > 0) { LcomD.Description += strPos + str1p2 + _LSX(EV); strPos = _LSX(+); }
		for (i = 0; i < m_nEhCount; i++) nEH += Set_STLCIncludeCS(arEhLoad.GetAt(i), dFact_1p4, LoadCaseNo, LcomD, 0);
		if (nEH > 0) { LcomD.Description += strPos + str1p4 + _LSX(EH); strPos = _LSX(+); }
		for (i = 0; i < m_nCrCount; i++) nCR += Set_STLCIncludeCS(arCrLoad.GetAt(i), 1., LoadCaseNo, LcomD, D_SGLD_CS_CVL);
		if (nCR > 0) { LcomD.Description += strPos + _T("1.0") + _LSX(CR); strPos = _LSX(+); }
		for (i = 0; i < m_nShCount; i++) nSH += Set_STLCIncludeCS(arShLoad.GetAt(i), 1., LoadCaseNo, LcomD, D_SGLD_SS_CVL);
		if (nSH > 0) { LcomD.Description += strPos + _T("1.0") + _LSX(SH); strPos = _LSX(+); }
		for (i = 0; i < m_nBCount; i++)  nBL += Set_STLCIncludeCS(arBLoad.GetAt(i), 1., LoadCaseNo, LcomD, 0);
		if (nBL > 0) { LcomD.Description += strPos + _T("1.0") + _LSX(B); strPos = _LSX(+); }
		for (i = 0; i < m_nStlCount; i++) nSTL += Set_STLCIncludeCS(arStlLoad.GetAt(i), dFact_0p5, LoadCaseNo, LcomD, 0);
		if (nSTL > 0) { LcomD.Description += strPos + str0p5 + _LSX(STL); strPos = _LSX(+); }
		if (m_nSmCount > 0)
		{
			Set_SettleCase(arSmLoad.GetAt(sm), dFact_0p5, LoadCaseNo, LcomD, TRUE); // SM
			CString strSM; strSM.Format(_T("%sSM[%d]"), str0p5, sm + 1);
			LcomD.Description += strPos + strSM; strPos = _LSX(+);
		}
		if (m_nConstLoad != 0) // CS or ST+CS
		{
			int n = 0;
			if (CheckCS_SgldK(D_SGLD_DL_CVL, n))
			{
				Set_CStageCase(m_aCSLoad.GetAt(n), dFact_1p2, LoadCaseNo, LcomD, TRUE);
				LcomD.Description += strPos + str1p2 + _T("(cD)"); strPos = _LSX(+);
			}

			CString strcEL;
			int nErection = 0;

			for (int nErec = 0; nErec < D_STCT_EREC_MAX_CVL; ++nErec)
			{
				strcEL = _T(""); nErection = 0;
				if (CheckCS_SgldK(m_CSEL_SgldK[nErec], n)) { strcEL = m_CSEL_Sgld_Desc[nErec]; nErection = m_CSEL_SgldK[nErec]; }



				if (CheckCSEL(nErection, _LSX(D)))
				{
					Set_CStageCase(m_aCSLoad.GetAt(n), dFact_1p2, LoadCaseNo, LcomD, TRUE);
					LcomD.Description += strPos + str1p2 + strcEL; strPos = _LSX(+);
				}
			}
			if (CheckCS_SgldK(D_SGLD_TP_CVL, n) && nType == 3) // tendon primary
			{
				double dPST = m_bLossFactor ? m_dLossFactorTransfer * dFact_1p2 : dFact_1p2;
				Set_CStageCase(m_aCSLoad.GetAt(n), dPST, LoadCaseNo, LcomD, TRUE);
				LcomD.Description += strPos + GetFactorString(dPST) + _T("(cTP)"); strPos = _LSX(+);
			}
			if (CheckCS_SgldK(D_SGLD_TS_CVL, n)) // tendon secondary  ¸ÖÊø¶þ´Î
			{
				double dPSS = m_bLossFactor ? m_dLossFactorService * dFact_1p2 : dFact_1p2;
				Set_CStageCase(m_aCSLoad.GetAt(n), dPSS, LoadCaseNo, LcomD, TRUE);
				LcomD.Description += strPos + GetFactorString(dPSS) + _T("(cTS)"); strPos = _LSX(+);
			}
			if (CheckCS_SgldK(D_SGLD_CS_CVL, n)) // Creep secondary   Ðì±ä¶þ´Î
			{
				Set_CStageCase(m_aCSLoad.GetAt(n), 1.0, LoadCaseNo, LcomD, TRUE);
				LcomD.Description += strPos + _T("1.0(cCR)"); strPos = _LSX(+);
			}
			if (CheckCS_SgldK(D_SGLD_SS_CVL, n)) // Shrinkage secondary  ÊÕËõ¶þ´Î
			{
				Set_CStageCase(m_aCSLoad.GetAt(n), 1.0, LoadCaseNo, LcomD, TRUE);
				LcomD.Description += strPos + _T("1.0(cSH)"); strPos = _LSX(+);
			}
		}
		arData.Add(LcomD);
	}

	if (arData.GetSize() < 1) return FALSE;

	arData.FreeExtra();

	return TRUE;
}

BOOL CLoadCombCtrl::SetCvlJTGD60_15_Capacity_M(const CArray<T_LCOM_D, T_LCOM_D&>& arData, ArUNT& arMLoad, ArINT& arLLoad,
	ArINT& arIlLoad, ArINT& arCrlLoad, ArINT& arMCrlLoad, ArINT& arCfLoad, ArINT& arLsLoad,
	ArINT& arWLoad, ArINT& arWlLoad, ArINT& arSfLoad, ArINT& arIpLoad, ArINT& arTLoad, ArINT& arTpgLoad,
	ArINT& arFrLoad, ArINT& arBrkLoad, int nType)
{
	// nType == 0,4: °è¼öÁ¶ÇÕ, 1: 1.0(settlement Á¦¿Ü), 2: 1.0(¸ðµÎ), Live Load Á¦¿Ü
	CArray<T_LCOM_D, T_LCOM_D&> aLcomD; aLcomD.Copy(arData);
	int  nLcomSize = aLcomD.GetSize();
	int  nnn = 0, lc = 0;
	//BOOL bAddCsLoad = (m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER1_CVL,nnn)) || (m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER2_CVL,nnn)) || (m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER3_CVL,nnn)); // CS or ST+CS
	BOOL bAddCsLoad = m_nConstLoad != 0 && CheckCS_SgldK(D_SGLD_ER1_CVL, nnn); // °üº¬Ê©¹¤ºÉÔØÇÒ´æÔÚÊ©¹¤ºÉÔØ
	// ¿µ±¸ÇÏÁß¸¸ Á¶ÇÕ
	for (lc = 0; lc < nLcomSize; lc++)
	{
		T_LCOM_D LcomD = aLcomD.GetAt(lc);
		LcomD.nActive = 1; // Active or Strength/Stress
		if (nType == 2 || nType == 4) m_aAcdnLcomD.Add(LcomD);
		else                     Set_LoadComb4Civil(LcomD, D_STR, 0, 112); // 112:ÓÀ¾Ã×÷ÓÃ£¨ºãÔØ£©
	}

	double dFactor = (nType == 0 || nType == 4) ? 1.4 : 1.0;//³ýÆû³µºÉÔØ£¨º¬³å»÷Á¦¡¢ÀëÐÄÁ¦£©¡¢·çºÉÔØÍâ¿É±ä×÷ÓÃ·ÖÏîÏµÊý
	double dWFactor = (nType == 0 || nType == 4) ? 1.4 : 1.0;//ÔËÐÐ·çºÉÔØ·ÖÏîÏµÊý
	double dWlFactor = (nType == 0 || nType == 4) ? 1.1 : 1.0;//Ë²Ê±·çºÉÔØ·ÖÏîÏµÊý
	double dCJJMLFactor = (nType == 0 || nType == 4) ? 1.1 : 1.0;   // CJJ ÌØÖÖ³µÁ¾ ×éºÏÏµÊý
	double dIL = (nType == 12) ? 0. : CalcMovingILFactor();    // Æû³µºÉÔØ³å»÷ÏµÊý

	int nMsgType = (nType == 0 || nType == 1) ? 114 : 115; // 114:»ù±¾×éºÏ, 115:Å¼È»×éºÏ

	// ÓÀ¾Ã×÷ÓÃ + 1¸ö¿É±ä×÷ÓÃ
	if (m_bJTG15_Factor)
	{
		dFactor *= m_dJTG15_Factor;
		dWFactor *= m_dJTG15_Factor;
		dWlFactor *= m_dJTG15_Factor;
		dCJJMLFactor *= m_dJTG15_Factor;
	}

	CString strDesc = _T("");
	int LoadCaseNo = 0, nc = 0;
	for (lc = 0; lc < nLcomSize; lc++)
	{
		int nMCount = max(m_nMCount, 1);
		for (int m = 0; m < nMCount; m++)
		{
			T_LCOM_D LcomD = aLcomD.GetAt(lc); LcomD.nActive = 1; // Active or Strength/Stress
			LoadCaseNo = GetCountLcomCase(LcomD);

			LcomD.nActive = nType == 17 ? 4 : 1;  // Active or Strength/Stress			 
			if (m_nLcomType == D_LCOMTYPE_GENERAL) LcomD.nActive = 1;  // general
			double dMlFactor = 1.4; 
			// Æû³µºÉÔØ×éºÏÏµÊý
			CString strJudgeMILFactorByLoadCase = GetJTGD602015MILoadName(arMLoad, m);

			if (nType == 0 || nType == 1)         // ³ÐÔØÄÜÁ¦×´Ì¬  »ù±¾×éºÏ
			{
				if (strJudgeMILFactorByLoadCase == _T("CH-CL") || strJudgeMILFactorByLoadCase == _T("ÓÃ»§¶¨Òå"))
				{
					dMlFactor = 1.8;                  // ³µÁ¾ºÉÔØ  ÓÃ»§¶¨Òå   È¡1.8
				}
			}
			if (nType == 17)  dMlFactor = 1.0;
			if (nMsgType == 114 && m_bJTG15_Factor)
			{
				dMlFactor *= m_dJTG15_Factor;
			}

			if (nType != 2) // live load »îºÉÔØ
			{
				BOOL bAddLcomD = FALSE;
				// moving load ÒÆ¶¯ºÉÔØ
				if (m_nMCount > 0)
				{
					Set_MovingCase(arMLoad.GetAt(m), dMlFactor, LoadCaseNo, LcomD, TRUE);
					strDesc.Format(_T("+%sM"), GetFactorString(dMlFactor)); LcomD.Description += strDesc;
					if (m_nMCount > 1) { strDesc.Format(_T("[%d]"), m + 1); LcomD.Description += strDesc; }
					bAddLcomD = TRUE;
				}

				// »îÔØ  »îÔØ³å»÷
				int nLL = 0, nIL = 0, nCF = 0;
				if (m_nLCount > 0 || (bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(L)))) // ST or ST+CS
				{
					for (nc = 0; nc < m_nLCount; nc++)
						nLL += Set_STLCIncludeCS(arLLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD, 0);
					if (nLL > 0) { strDesc.Format(_T("+%sL"), GetFactorString(dFactor)); LcomD.Description = LcomD.Description + strDesc; }
					if (bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(L)))
					{
						Set_CStageCase(m_aCSLoad.GetAt(nnn), dFactor, LoadCaseNo, LcomD, TRUE);
						strDesc.Format(_T("+%s(cEL)"), GetFactorString(dFactor)); LcomD.Description = LcomD.Description + strDesc;
						bAddLcomD = TRUE;
					}
					for (nc = 0; nc < m_nIlCount; nc++)
						nIL += Set_STLCIncludeCS(arIlLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD, 0);
					if (nIL > 0) { strDesc.Format(_T("+%sIL"), GetFactorString(dFactor)); LcomD.Description = LcomD.Description + strDesc; }
					if (nLL + nIL > 0) bAddLcomD = TRUE;
				}

				// ÀëÐÄÁ¦
				if (m_nMCount > 0 || (m_nLCount > 0 || (bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(L)))))
				{
					for (nc = 0; nc < m_nCfCount; nc++)
						nCF += Set_STLCIncludeCS(arCfLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD, 0);
					if (nCF > 0) { strDesc.Format(_T("+%sCF"), GetFactorString(dFactor)); LcomD.Description += strDesc; }
					bAddLcomD = TRUE;
				}
				if (bAddLcomD)
				{
					if (nType == 4) m_aAcdnLcomD.Add(LcomD);
					else         Set_LoadComb4Civil(LcomD, D_STR, 0, nMsgType);
				}
			}

			if (m_nLsCount > 0) // Live Load Surcharge ¸½¼ÓµØÃæ»îºÉÔØ
			{
				LcomD = aLcomD.GetAt(lc); LcomD.nActive = 1; LoadCaseNo = GetCountLcomCase(LcomD);
				int nLS = 0;
				for (nc = 0; nc < m_nLsCount; nc++) nLS += Set_STLCIncludeCS(arLsLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD, 0);
				if (nLS > 0)
				{
					strDesc.Format(_T("+%sLS"), GetFactorString(dFactor)); LcomD.Description += strDesc;
					if (nType == 2 || nType == 4) m_aAcdnLcomD.Add(LcomD);
					else                     Set_LoadComb4Civil(LcomD, D_STR, 0, nMsgType);
				}
			}// MNET:XXXX-BJLEE(BBONG)-20060502

			// ²½ÐÐ»îºÉÔØ   ÈËÈººÉÔØ
			if (nType != 2 && (m_nCrlCount > 0 || m_nMCrlCount > 0)) // Crowd Load
			{
				LcomD = aLcomD.GetAt(lc);
				LcomD.nActive = nType == 17 ? 4 : 1;
				if (m_nLcomType == D_LCOMTYPE_GENERAL) LcomD.nActive = 1;  // general
				LoadCaseNo = GetCountLcomCase(LcomD);
				int nCRL = 0;
				for (nc = 0; nc < m_nCrlCount; nc++)
					nCRL += Set_STLCIncludeCS(arCrlLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD, 0);
				if (nCRL > 0) { strDesc.Format(_T("+%sCRL"), GetFactorString(dFactor)); LcomD.Description += strDesc; }

				int nMCrlCount = (m_nMCrlCount > 0) ? m_nMCrlCount : ((nCRL > 0) ? 1 : 0);
				for (int crl = 0; crl < nMCrlCount; ++crl)
				{
					T_LCOM_D LcomD_crl = LcomD; LoadCaseNo = GetCountLcomCase(LcomD_crl);
					if (m_nMCrlCount > 0)
					{
						Set_MovingCase(arMCrlLoad.GetAt(crl), dFactor, LoadCaseNo, LcomD_crl, TRUE);
						strDesc.Format(_T("+%sMCRL"), GetFactorString(dFactor));
						LcomD_crl.Description += strDesc;
						if (m_nMCrlCount > 1) { strDesc.Format(_T("[%d]"), crl + 1); LcomD_crl.Description += strDesc; }
					}
					if (nCRL > 0 || m_nMCrlCount > 0)
					{
						if (nType == 2 || nType == 4) m_aAcdnLcomD.Add(LcomD_crl);
						else                     Set_LoadComb4Civil(LcomD_crl, D_STR, 0, nMsgType);
					}
				}
			}

			{
				int nWCount = m_nWCount * 2; // ÔËÓª·çºÉÔØ
				if (m_nWCount == 0 && bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(W))) nWCount = 1;
				for (nc = 0; nc < nWCount; nc++)
				{
					LcomD = aLcomD.GetAt(lc);
					LcomD.nActive = nType == 17 ? 4 : 1;
					if (m_nLcomType == D_LCOMTYPE_GENERAL) LcomD.nActive = 1;  // general
					LoadCaseNo = GetCountLcomCase(LcomD);
					if (m_nWCount > 0)
					{
						double dWF = (nc % 2 == 0) ? dWFactor : -1.*dWFactor;
						if (Set_STLCIncludeCS(arWLoad.GetAt(nc / 2), dWF, LoadCaseNo, LcomD, 0) > 0)
						{
							CString strW = (dWF > 0.0) ? _T("+%sW") : _T("%sW");
							strDesc.Format(strW, GetFactorString(dWF)); LcomD.Description += strDesc;
							if (m_nWCount > 1) { strDesc.Format(_T("[%d]"), nc / 2 + 1); LcomD.Description += strDesc; }
						}
					}
					if (bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(W)))
					{
						Set_CStageCase(m_aCSLoad.GetAt(nnn), dWFactor, LoadCaseNo, LcomD, TRUE);
						strDesc.Format(_T("+%s(cEL)"), GetFactorString(dWFactor)); LcomD.Description += strDesc;
					}
					if (nType == 2 || nType == 4) m_aAcdnLcomD.Add(LcomD);
					else                     Set_LoadComb4Civil(LcomD, D_STR, 0, nMsgType);

					double dF = (nType == 0 || nType == 4) ? 0.75 : 1.0;
					SetCvlJTJD60_15_ETC_M(LcomD, LcomD, FALSE, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWLoad, FALSE, arWlLoad, FALSE, arSfLoad, TRUE, arIpLoad, TRUE,
						arTLoad, arTpgLoad, TRUE, arFrLoad, TRUE, 0, dF, nType == 2 || nType == 4, nMsgType);
				}
			}

			{
				int nWlCount = m_nWlCount * 2; // Ë²Ê±·çºÉÔØ
				if (m_nWlCount == 0 && bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(WL))) nWlCount = 1;
				for (nc = 0; nc < nWlCount; nc++)
				{
					LcomD = aLcomD.GetAt(lc);
					LcomD.nActive = nType == 17 ? 4 : 1;
					if (m_nLcomType == D_LCOMTYPE_GENERAL) LcomD.nActive = 1;  // general
					LoadCaseNo = GetCountLcomCase(LcomD);
					if (m_nWlCount > 0)
					{
						double dWF = (nc % 2 == 0) ? dWlFactor : -1.*dWlFactor;
						if (Set_STLCIncludeCS(arWlLoad.GetAt(nc / 2), dWF, LoadCaseNo, LcomD, 0) > 0)
						{
							CString strW = (dWF > 0.0) ? _T("+%sWL") : _T("%sWL");
							strDesc.Format(strW, GetFactorString(dWF)); LcomD.Description += strDesc;
							if (m_nWCount > 1) { strDesc.Format(_T("[%d]"), nc / 2 + 1); LcomD.Description += strDesc; }
						}
					}
					if (bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(WL)))
					{
						Set_CStageCase(m_aCSLoad.GetAt(nnn), dWlFactor, LoadCaseNo, LcomD, TRUE);
						strDesc.Format(_T("+%s(cEL)"), GetFactorString(dWlFactor)); LcomD.Description += strDesc;
					}
					if (nType == 2 || nType == 4) m_aAcdnLcomD.Add(LcomD);
					else                     Set_LoadComb4Civil(LcomD, D_STR, 0, nMsgType);

					double dF = (nType == 0 || nType == 4) ? 0.75 : 1.0;
					SetCvlJTJD60_15_ETC_M(LcomD, LcomD, FALSE, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWLoad, FALSE, arWlLoad, FALSE, arSfLoad, TRUE, arIpLoad, TRUE,
						arTLoad, arTpgLoad, TRUE, arFrLoad, TRUE, 0, dF, nType == 2 || nType == 4, nMsgType);
				}
			}

			if (m_nSfCount > 0) // Stream Flow Pressure Á÷Ë®Ñ¹Á¦
			{
				LcomD = aLcomD.GetAt(lc);
				LcomD.nActive = nType == 17 ? 4 : 1;
				if (m_nLcomType == D_LCOMTYPE_GENERAL) LcomD.nActive = 1;  // general
				LoadCaseNo = GetCountLcomCase(LcomD);
				int nSF = 0;
				for (nc = 0; nc < m_nSfCount; nc++) nSF += Set_STLCIncludeCS(arSfLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD, 0);
				if (nSF > 0)
				{
					strDesc.Format(_T("+%sSF"), GetFactorString(dFactor)); LcomD.Description += strDesc;
					if (nType == 2 || nType == 4) m_aAcdnLcomD.Add(LcomD);
					else                     Set_LoadComb4Civil(LcomD, D_STR, 0, nMsgType);
				}
			}

			if (m_nIpCount > 0) // Ice Pressure ±ùÑ¹Á¦
			{
				LcomD = aLcomD.GetAt(lc);
				LcomD.nActive = nType == 17 ? 4 : 1;
				if (m_nLcomType == D_LCOMTYPE_GENERAL) LcomD.nActive = 1;  // general
				LoadCaseNo = GetCountLcomCase(LcomD);
				int nIP = 0;
				for (nc = 0; nc < m_nIpCount; nc++) nIP += Set_STLCIncludeCS(arIpLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD, 0);
				if (nIP > 0)
				{
					strDesc.Format(_T("+%sIP"), GetFactorString(dFactor)); LcomD.Description += strDesc;
					if (nType == 2 || nType == 4) m_aAcdnLcomD.Add(LcomD);
					else                     Set_LoadComb4Civil(LcomD, D_STR, 0, nMsgType);
				}
			}

			// Temeperature ÎÂ¶ÈºÉÔØ
			if ((m_nTCount > 0 || m_nTpgCount > 0) || (bAddCsLoad && (CheckCSEL(D_SGLD_ER1_CVL, _LSX(T)) || CheckCSEL(D_SGLD_ER1_CVL, _LSX(TPG)))))
			{
				LcomD = aLcomD.GetAt(lc);
				LcomD.nActive = nType == 17 ? 4 : 1;
				if (m_nLcomType == D_LCOMTYPE_GENERAL) LcomD.nActive = 1;  // general
				SetCvlJTJD60_Capacity_TPG(LcomD, arTLoad, arTpgLoad, arFrLoad, dFactor, dFactor, FALSE, nType == 2 || nType == 4, nMsgType);
			}

			if (m_nFrCount > 0) // Friction  Ä¦²ÁÁ¦
			{
				LcomD = aLcomD.GetAt(lc);
				LcomD.nActive = nType == 17 ? 4 : 1;
				if (m_nLcomType == D_LCOMTYPE_GENERAL) LcomD.nActive = 1;  // general
				LoadCaseNo = GetCountLcomCase(LcomD);
				int nFR = 0;
				for (nc = 0; nc < m_nFrCount; nc++) nFR += Set_STLCIncludeCS(arFrLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD, 0);
				if (nFR > 0)
				{
					strDesc.Format(_T("+%sFR"), GetFactorString(dFactor)); LcomD.Description += strDesc;
					if (nType == 2 || nType == 4) m_aAcdnLcomD.Add(LcomD);
					else                     Set_LoadComb4Civil(LcomD, D_STR, 0, nMsgType);
				}
			}
		} // next m
	} // next lc

	// ¿µ±¸ÇÏÁß + Â÷·®ÇÏÁß + 1°³ÀÇ ±âÅ¸ °¡º¯ÇÏÁßÀÎ °æ¿ì(8°³)
	SetCvlJTGD60_15_Var1_M(arData, arMLoad, arLLoad, arIlLoad, arCrlLoad, arMCrlLoad, arCfLoad, arLsLoad, arWLoad, arWlLoad, arSfLoad, arIpLoad,
		arTLoad, arTpgLoad, arFrLoad, arBrkLoad, nType, nMsgType);
	// ¿µ±¸ÇÏÁß + Â÷·®ÇÏÁß + 2°³ÀÇ ±âÅ¸ °¡º¯ÇÏÁßÀÎ °æ¿ì(24°³)
	SetCvlJTGD60_15_Var2_M(arData, arMLoad, arLLoad, arIlLoad, arCrlLoad, arMCrlLoad, arCfLoad, arLsLoad, arWLoad, arWlLoad, arSfLoad, arIpLoad,
		arTLoad, arTpgLoad, arFrLoad, arBrkLoad, nType, nMsgType);
	// ¿µ±¸ÇÏÁß + Â÷·®ÇÏÁß + 3°³ÀÇ ±âÅ¸ °¡º¯ÇÏÁßÀÎ °æ¿ì(36°³)
	SetCvlJTGD60_15_Var3_M(arData, arMLoad, arLLoad, arIlLoad, arCrlLoad, arMCrlLoad, arCfLoad, arLsLoad, arWLoad, arWlLoad, arSfLoad, arIpLoad,
		arTLoad, arTpgLoad, arFrLoad, arBrkLoad, nType, nMsgType);
	// ¿µ±¸ÇÏÁß + Â÷·®ÇÏÁß + 4°³ÀÇ ±âÅ¸ °¡º¯ÇÏÁßÀÎ °æ¿ì(29°³)
	SetCvlJTGD60_15_Var4_M(arData, arMLoad, arLLoad, arIlLoad, arCrlLoad, arMCrlLoad, arCfLoad, arLsLoad, arWLoad, arWlLoad, arSfLoad, arIpLoad,
		arTLoad, arTpgLoad, arFrLoad, arBrkLoad, nType, nMsgType);
	// ¿µ±¸ÇÏÁß + Â÷·®ÇÏÁß + 5°³ ¹× 6°³ÀÇ ±âÅ¸ °¡º¯ÇÏÁßÀÎ °æ¿ì(12°³ ¹× 2°³)
	SetCvlJTGD60_15_Var56_M(arData, arMLoad, arLLoad, arIlLoad, arCrlLoad, arMCrlLoad, arCfLoad, arLsLoad, arWLoad, arWlLoad, arSfLoad, arIpLoad,
		arTLoad, arTpgLoad, arFrLoad, arBrkLoad, nType, nMsgType);

	return TRUE;
}


BOOL CLoadCombCtrl::SetCvlJTGD60_15_Capacity_A(const CArray<T_LCOM_D, T_LCOM_D&>& arData, ArINT& arLLoad, ArINT& arIlLoad,
	ArINT& arCfLoad, ArINT& arLsLoad, ArINT& arCrlLoad, ArINT& arWLoad, ArINT& arSfLoad,
	ArINT& arIpLoad, ArINT& arTLoad, ArINT& arTpgLoad, ArINT& arFrLoad, ArINT& arBrkLoad,
	ArUNT& arMLoad, ArINT& arMcrLoad, int nType, int nAcdType)
{
	// nType   0 or 4: °è¼öÁ¶ÇÕ, 1: 1.0(settlement Á¦¿Ü), 2: 1.0(¸ðµÎ), Live Load Á¦¿Ü
	CArray<T_LCOM_D, T_LCOM_D&> aLcomD; aLcomD.Copy(arData);
	int  nLcomSize = aLcomD.GetSize();
	int  nnn = 0, lc = 0;

	BOOL bErec = FALSE;
	for (int nErec = 0; nErec < D_STCT_EREC_MAX_CVL; ++nErec)
	{
		if (m_nConstLoad != 0 && CheckCS_SgldK(D_SGLD_ER1_CVL + nErec, nnn)) bErec = TRUE;
	}
	int nActive = m_nLcomType == D_LCOMTYPE_GENERAL ? 1 : 6;
	BOOL bAddCsLoad = bErec;
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER1_CVL,nnn)) || 
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER2_CVL,nnn)) || 
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER3_CVL,nnn)) ||
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER4_CVL,nnn)) ||
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER5_CVL,nnn)) ||
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER6_CVL,nnn)) ||
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER7_CVL,nnn)) ||
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER8_CVL,nnn)) ||
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER9_CVL,nnn)) ||
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER10_CVL,nnn)); // CS or ST+CS

		// ÓÀ¾Ã×÷ÓÃ×éºÏ
	for (lc = 0; lc < nLcomSize; lc++)
	{
		T_LCOM_D LcomD = aLcomD.GetAt(lc);
		LcomD.nActive = nActive; // Active or Strength/Stress
		if (nType == 2 || nType == 4) m_aAcdnLcomD.Add(LcomD);
		else                     Set_LoadComb4Civil(LcomD, D_STR, 0, 112); // »ù±¾×éºÏ£¨ÓÀ¾Ã×÷ÓÃ£©
	}
	if ((nType == 2 || nType == 4) && (m_nLsCount == 0 && m_nWCount == 0 && m_nSfCount == 0 &&
		m_nIpCount == 0 && m_nTCount == 0 && m_nTpgCount == 0 && m_nFrCount == 0))
	{
		for (lc = 0; lc < nLcomSize; lc++) m_aAcdnLcomD.Add(aLcomD.GetAt(lc));
		return TRUE;
	}

	double dFactor = 1.0;
	double dMLFactor = 1.0;       // Æû³µºÉÔØÏµÊý
	double dCrlFactor = 1.0;      // ÈËÈººÉÔØÏµÊý
	double dWFactor = 0.75;       // ·çºÉÔØÏµÊý
	double dTgpFactor = 0.8;      //ÎÂ¶ÈÌÝ¶ÈºÉÔØÏµÊý
	if (nAcdType == 0)//ÆµÓöÏµÊý modified this statement by gongxing at 2016-04-13
	{
		dMLFactor = 0.7;
		dCrlFactor = 1.0;
	}
	else if (nAcdType == 1)//×¼ÓÀ¾ÃÏµÊý
	{
		dMLFactor = 0.4;
		dCrlFactor = 0.4;
	}
	else if (nAcdType == 100)//»ù´¡ÑéËã-Å¼È»×éºÏ
	{
		dMLFactor  = 1.0;
		dCrlFactor = 1.0;
		dWFactor   = 1.0;
	}

	// 	double dFactor  = (nType==0 || nType==4) ? 1.4:1.0;
	// 	double dWFactor = (nType==0 || nType==4) ? 1.1:1.0;

	double dIL = (nType == 12) ? 0. : CalcMovingILFactor();//µ±²»Îª±ê×¼×éºÏÊ±£¬¼ÆËã³å»÷ÏµÊý
	if (nAcdType == 100)//»ù´¡ÑéËã-Å¼È»×éºÏ
	{
		dIL = 0.0;
	}

	int nMsgType = (nType == 0 || nType == 1) ? 114 : 115; // 114 : »ù±¾×éºÏ, 115 : Å¼È»×éºÏ
	// ÓÀ¾Ã×÷ÓÃ + 1¿É±ä×÷ÓÃ
	CString strDesc = _T("");
	int LoadCaseNo = 0, nc = 0;
	for (lc = 0; lc < nLcomSize; lc++)
	{
		int nMCount = max(m_nMCount + 1, 1);
		for (int m = 0;m < nMCount;m++)//Æû³µºÉÔØ
		{
			T_LCOM_D LcomD = aLcomD.GetAt(lc);
			LcomD.nActive = nActive;
			LoadCaseNo = GetCountLcomCase(LcomD);

			BOOL bAddLcomD = FALSE;
			if (m < m_nMCount)
			{
				Set_MovingCase(arMLoad.GetAt(m), dMLFactor, LoadCaseNo, LcomD, TRUE);
				strDesc.Format(_T("+%sM"), GetFactorString(dMLFactor)); LcomD.Description += strDesc;

				if (m_nMCount > 1) { strDesc.Format(_T("[%d]"), m + 1); LcomD.Description += strDesc; }
				if (nType == 2 || nType == 4) m_aAcdnLcomD.Add(LcomD);
				else                     Set_LoadComb4Civil(LcomD, D_STR, 0, nMsgType);
			}

			int nMcrCount = max(m_nMCrlCount + 1, 1);
			for (int nMcr = 0;nMcr < nMcrCount;nMcr++)//ÈËÈººÉÔØ
			{
				if (nMcr < m_nMCrlCount)
				{
					Set_MovingCase(arMcrLoad.GetAt(nMcr), dCrlFactor, LoadCaseNo, LcomD, TRUE);
					strDesc.Format(_T("+%sMcr"), GetFactorString(dCrlFactor)); LcomD.Description += strDesc;
					if (m_nMCrlCount > 1) { strDesc.Format(_T("[%d]"), nMcr + 1);LcomD.Description += strDesc; }
					if (nType == 2 || nType == 4) m_aAcdnLcomD.Add(LcomD);
					else                     Set_LoadComb4Civil(LcomD, D_STR, 0, nMsgType);
				}

				//»îºÉÔØ
				int nLL = 0, nIL = 0, nCF = 0;
				if (m_nLCount > 0 || (bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(L)))) // ST or ST+CS
				{
					for (nc = 0; nc < m_nLCount; nc++) nLL += Set_STLCIncludeCS(arLLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD, 0);
					if (nLL > 0) { strDesc.Format(_T("+%sL"), GetFactorString(dFactor)); LcomD.Description += strDesc; }
					if (bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(L)))
					{
						Set_CStageCase(m_aCSLoad.GetAt(nnn), dFactor, LoadCaseNo, LcomD, TRUE);
						strDesc.Format(_T("+%s(cEL)"), GetFactorString(dFactor)); LcomD.Description += strDesc;
						bAddLcomD = TRUE;
					}
					for (nc = 0; nc < m_nIlCount; nc++)
						nIL += Set_STLCIncludeCS(arIlLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD, 0);
					if (nIL > 0) { strDesc.Format(_T("+%sIL"), GetFactorString(dFactor)); LcomD.Description += strDesc; }
					if (nLL + nIL > 0)
					{
						if (nType == 2 || nType == 4) m_aAcdnLcomD.Add(LcomD);
						else                     Set_LoadComb4Civil(LcomD, D_STR, 0, nMsgType);
					}
				}

				// ???
				if (m_nMCount > 0 || (m_nLCount > 0 || (bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(L)))))
				{
					for (nc = 0; nc < m_nCfCount; nc++) nCF += Set_STLCIncludeCS(arCfLoad.GetAt(nc), dMLFactor, LoadCaseNo, LcomD, 0);
					if (nCF > 0) { strDesc.Format(_T("+%sCF"), GetFactorString(dMLFactor)); LcomD.Description += strDesc; }
					if (nType == 2 || nType == 4) m_aAcdnLcomD.Add(LcomD);
					else                     Set_LoadComb4Civil(LcomD, D_STR, 0, nMsgType);
				}

				if (m_nLsCount > 0) // Live Load Surcharge µØÃæ¸½¼Ó»îºÉÔØ
				{
					LcomD = aLcomD.GetAt(lc);
					LcomD.nActive = nActive;
					LoadCaseNo = GetCountLcomCase(LcomD);
					int nLS = 0;
					for (nc = 0; nc < m_nLsCount; nc++) nLS += Set_STLCIncludeCS(arLsLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD, 0);
					if (nLS > 0)
					{
						strDesc.Format(_T("+%sLS"), GetFactorString(dFactor)); LcomD.Description += strDesc;
						if (nType == 2 || nType == 4) m_aAcdnLcomD.Add(LcomD);
						else                     Set_LoadComb4Civil(LcomD, D_STR, 0, nMsgType);
					}
				}
				if (nType != 2 && m_nCrlCount > 0) // Crowd Load ÈËÈººÉÔØ
				{
					LcomD = aLcomD.GetAt(lc);
					LcomD.nActive = nActive;
					LoadCaseNo = GetCountLcomCase(LcomD);
					int nCRL = 0;
					for (nc = 0; nc < m_nCrlCount; nc++) nCRL += Set_STLCIncludeCS(arCrlLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD, 0);
					if (nCRL > 0)
					{
						strDesc.Format(_T("+%sCRL"), GetFactorString(dFactor)); LcomD.Description += strDesc;
						if (nType == 2 || nType == 4) m_aAcdnLcomD.Add(LcomD);
						else                     Set_LoadComb4Civil(LcomD, D_STR, 0, nMsgType);
					}
				}

				int nWCount = m_nWCount * 2; // MNET:1614, KJH20050808 ·çºÉÔØ
				if (m_nWCount == 0 && bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(W))) nWCount = 1;
				for (nc = 0; nc < nWCount; nc++)
				{
					LcomD = aLcomD.GetAt(lc);
					LcomD.nActive = nActive;
					LoadCaseNo = GetCountLcomCase(LcomD);
					if (m_nWCount > 0)
					{
						double dWF = (nc % 2 == 0) ? dWFactor : -1. * dWFactor;
						if (Set_STLCIncludeCS(arWLoad.GetAt(nc / 2), dWF, LoadCaseNo, LcomD, 0) > 0)
						{
							CString strW = (dWF > 0.0) ? _T("+%sW") : _T("%sW");
							strDesc.Format(strW, GetFactorString(dWF)); LcomD.Description += strDesc;
							if (m_nWCount > 1) { strDesc.Format(_T("[%d]"), nc / 2 + 1); LcomD.Description += strDesc; }
						}
					}
					if (bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(W)))
					{
						Set_CStageCase(m_aCSLoad.GetAt(nnn), dWFactor, LoadCaseNo, LcomD, TRUE);
						strDesc.Format(_T("+%s(cEL)"), GetFactorString(dWFactor)); LcomD.Description += strDesc;
					}
					if (nType == 2 || nType == 4) m_aAcdnLcomD.Add(LcomD);
					else                     Set_LoadComb4Civil(LcomD, D_STR, 0, nMsgType);
				}
				if (m_nSfCount > 0) // Stream Flow Pressure Á÷Ë®Ñ¹Á¦
				{
					LcomD = aLcomD.GetAt(lc);
					LcomD.nActive = nActive;
					LoadCaseNo = GetCountLcomCase(LcomD);
					int nSF = 0;
					for (nc = 0; nc < m_nSfCount; nc++) nSF += Set_STLCIncludeCS(arSfLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD, 0);
					if (nSF > 0)
					{
						strDesc.Format(_T("+%sSF"), GetFactorString(dFactor)); LcomD.Description += strDesc;
						if (nType == 2 || nType == 4) m_aAcdnLcomD.Add(LcomD);
						else                     Set_LoadComb4Civil(LcomD, D_STR, 0, nMsgType);
					}
				}
				if (m_nIpCount > 0) // Ice Pressure ±ùÑ¹Á¦
				{
					LcomD = aLcomD.GetAt(lc);
					LcomD.nActive = nActive;
					LoadCaseNo = GetCountLcomCase(LcomD);
					int nIP = 0;
					for (nc = 0; nc < m_nIpCount; nc++) nIP += Set_STLCIncludeCS(arIpLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD, 0);
					if (nIP > 0)
					{
						strDesc.Format(_T("+%sIP"), GetFactorString(dFactor)); LcomD.Description += strDesc;
						if (nType == 2 || nType == 4) m_aAcdnLcomD.Add(LcomD);
						else                     Set_LoadComb4Civil(LcomD, D_STR, 0, nMsgType);
					}
				}
				// Temeperature ÎÂ¶ÈºÉÔØ
				if ((m_nTCount > 0 || m_nTpgCount > 0) || (bAddCsLoad && (CheckCSEL(D_SGLD_ER1_CVL, _LSX(T)) || CheckCSEL(D_SGLD_ER1_CVL, _LSX(TPG)))))
				{
					LcomD = aLcomD.GetAt(lc);
					LcomD.nActive = nActive;
					SetCvlJTJD60_Capacity_TPG(LcomD, arTLoad, arTpgLoad, arFrLoad, dFactor, dFactor, FALSE, nType == 2 || nType == 4, nMsgType);
				}
				if (m_nFrCount > 0) // Friction Ä¦²ÁÁ¦
				{
					LcomD = aLcomD.GetAt(lc);
					LcomD.nActive = nActive;
					LoadCaseNo = GetCountLcomCase(LcomD);
					int nFR = 0;
					for (nc = 0; nc < m_nFrCount; nc++) nFR += Set_STLCIncludeCS(arFrLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD, 0);
					if (nFR > 0)
					{
						strDesc.Format(_T("+%sFR"), GetFactorString(dFactor)); LcomD.Description += strDesc;
						if (nType == 2 || nType == 4) m_aAcdnLcomD.Add(LcomD);
						else                     Set_LoadComb4Civil(LcomD, D_STR, 0, nMsgType);
					}
				}

			}
		}
	}

	//if((m_nConstLoad!=1 && m_nLCount>0) || (m_nConstLoad==1 && CheckCSEL(D_SGLD_ER1_CVL, _LSX(L))) || nType==2)
	{
		// ÓÀ¾Ã×÷ÓÃ + ÒÆ¶¯ºÉÔØ + 1¿É±ä×÷ÓÃ
		SetCvlJTJD60_15_Var1_A(arData, arLLoad, arIlLoad, arCfLoad, arLsLoad, arCrlLoad,
			arWLoad, arSfLoad, arIpLoad, arTLoad, arTpgLoad, arFrLoad, arBrkLoad, arMLoad, arMcrLoad, nType, nMsgType, nAcdType);//modified this line by gongxing at 2016-04-13
		// ÓÀ¾Ã×÷ÓÃ + ÒÆ¶¯ºÉÔØ + 2¿É±ä×÷ÓÃ
		SetCvlJTJD60_15_Var2_A(arData, arLLoad, arIlLoad, arCfLoad, arLsLoad, arCrlLoad,
			arWLoad, arSfLoad, arIpLoad, arTLoad, arTpgLoad, arFrLoad, arBrkLoad, arMLoad, arMcrLoad, nType, nMsgType, nAcdType);//modified this line by gongxing at 2016-04-13
		// ÓÀ¾Ã×÷ÓÃ + ÒÆ¶¯ºÉÔØ + 3¿É±ä×÷ÓÃ
		SetCvlJTJD60_15_Var3_A(arData, arLLoad, arIlLoad, arCfLoad, arLsLoad, arCrlLoad,
			arWLoad, arSfLoad, arIpLoad, arTLoad, arTpgLoad, arFrLoad, arBrkLoad, arMLoad, arMcrLoad, nType, nMsgType, nAcdType);//modified this line by gongxing at 2016-04-13
		// ÓÀ¾Ã×÷ÓÃ + ÒÆ¶¯ºÉÔØ + 4¿É±ä×÷ÓÃ
		SetCvlJTJD60_15_Var4_A(arData, arLLoad, arIlLoad, arCfLoad, arLsLoad, arCrlLoad,
			arWLoad, arSfLoad, arIpLoad, arTLoad, arTpgLoad, arFrLoad, arBrkLoad, arMLoad, arMcrLoad, nType, nMsgType, nAcdType);//modified this line by gongxing at 2016-04-13
		// ÓÀ¾Ã×÷ÓÃ + ÒÆ¶¯ºÉÔØ + 5.6¿É±ä×÷ÓÃ
		SetCvlJTJD60_15_Var56_A(arData, arLLoad, arIlLoad, arCfLoad, arLsLoad, arCrlLoad,
			arWLoad, arSfLoad, arIpLoad, arTLoad, arTpgLoad, arFrLoad, arBrkLoad, arMLoad, arMcrLoad, nType, nMsgType, nAcdType);//modified this line by gongxing at 2016-04-13
	}

	return TRUE;
}

BOOL CLoadCombCtrl::SetCvlJTGD60_15_Service_New(const CArray<T_LCOM_D, T_LCOM_D&>& arData, ArUNT& arMLoad,
	ArINT& arLLoad, ArINT& arIlLoad, ArINT& arCrlLoad, ArINT& arMCrlLoad,
	ArINT& arCfLoad, ArINT& arLsLoad, ArINT& arWLoad, ArINT& arWlLoad, ArINT& arSfLoad, ArINT& arIpLoad,
	ArINT& arTLoad, ArINT& arTpgLoad, ArINT& arFrLoad, ArINT& arBrkLoad, ArINT& arIlpLoad, ArINT& arCoLoad, int nType)
{
	// nType == 10:×¼ÓÀ¾Ã, 11:ÆµÓö 12:µ¯ÐÔ½×¶Î
	CArray<T_LCOM_D, T_LCOM_D&> aLcomD;
	aLcomD.Copy(arData);
	int  nLcomSize = aLcomD.GetSize();
	int  nnn = 0;

	BOOL bErec = FALSE;
	for (int nErec = 0; nErec < D_STCT_EREC_MAX_CVL; ++nErec)
	{
		if (m_nConstLoad != 0 && CheckCS_SgldK(D_SGLD_ER1_CVL + nErec, nnn)) bErec = TRUE;
	}

	BOOL bAddCsLoad = bErec;
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER1_CVL,nnn)) || 
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER2_CVL,nnn)) || 
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER3_CVL,nnn)) ||
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER4_CVL,nnn)) ||
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER5_CVL,nnn)) ||
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER6_CVL,nnn)) ||
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER7_CVL,nnn)) ||
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER8_CVL,nnn)) ||
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER9_CVL,nnn)) ||
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER10_CVL,nnn));// CS or ST+CS;

	int nServEnvType = (nType == 12) ? 1 : 0;
	// ¿µ±¸ÇÏÁß¸¸ Á¶ÇÕ(±ØÇÑ»óÅÂÀÇ ´Ü±âÁ¶ÇÕ¸¸¿¡¼­¸¸..)
	if (nType == 11 || nType == 100)//¶ÔÓ¦ÆµÓö×éºÏ/»ù´¡¹æ·¶-ÆµÓö×éºÏ
	{
		for (int lc = 0; lc < nLcomSize; lc++)
		{
			T_LCOM_D LcomD = aLcomD.GetAt(lc);
			LcomD.nActive = 1; // Active or Strength/Stress
			if (m_nLcomType == D_LCOMTYPE_CONCRETE || m_nLcomType == D_LCOMTYPE_STLCOMP || m_nLcomType == D_LCOMTYPE_SRC)  // serviceability
			{
				LcomD.nActive = 7;
			}
			//Set_LoadComb4Civil(LcomD, D_SER, nServEnvType, 116); // serviceablilty, 116:¼«¶Ë×éºÏ(ÓÀ¾Ã×÷ÓÃ)
		}
	}

	double dLFactor = 1.0, dCRLFactor = 1.0;
	double dFactor = 1.0, dTPGFactor = 1.0;
	double dWFactor = 1.0;
	double dWlFactor = 1.0;
	double dCfFactor = 1.0;         // ÀëÐÄÁ¦¡¢»îºÉÔØ³å»÷¡¢³¬ÔØ³å»÷Á¦¡¢³å»÷Á¦ ÏµÊý
	if (nType == 10) { dLFactor = 0.4, dWFactor = 0.75, dWlFactor = 1.0, dTPGFactor = 0.8, dCRLFactor = 0.4; }
	else if (nType == 11) { dLFactor = 0.7, dWFactor = 0.75, dWlFactor = 1.0, dTPGFactor = 0.8, dCfFactor = 0.7, dCRLFactor = 1.0; }
	else if (nType == 100){ dLFactor = 1.0, dWFactor = 1.0, dWlFactor = 1.0, dTPGFactor = 1.0, dCfFactor = 1.0, dCRLFactor = 1.0; }

	BOOL bUseWlLoad = TRUE;

	int nMsgType = 119; // µ¯ÐÔ½×¶ÎÓ¦Á¦
	if (nType == 10)      nMsgType = 117; // ×¼ÓÀ¾Ã
	else if (nType == 11) nMsgType = 118; // ÆµÓö
	else if (nType == 100)nMsgType = 200; // »ù´¡ÑéËã-ÆµÓö

	double dIL = (nType == 12 || nType == 100) ? 0. : CalcMovingILFactor();//Ó¦Á¦ÑéËãÊ±£¬ÐèÌáÈ¡³å»÷ÏµÊý
	
	// ÓÀ¾Ã×÷ÓÃ + 1¿É±ä×÷ÓÃ
	CString strDesc = _T("");
	int LoadCaseNo = 0, nc = 0;
	for (int lc = 0; lc < nLcomSize; lc++)
	{
		int nMCount = max(m_nMCount, 1);
		for (int m = 0; m < nMCount; m++)
		{
			T_LCOM_D LcomD = aLcomD.GetAt(lc); LcomD.nActive = 1; // Active or Strength/Stress
			if (m_nLcomType == D_LCOMTYPE_CONCRETE)
			{
				if (nType == 10) LcomD.nActive = 8;  //×¼ÓÀ¾Ã×éºÏ
				if (nType == 11 || nType == 100) LcomD.nActive = 7;  //ÆµÓö×éºÏ
				if (nType == 12)
				{
					LcomD.bElasticStage = TRUE;
					LcomD.nActive = 2;//±ê×¼×éºÏ
				}
			}
			else if (m_nLcomType == D_LCOMTYPE_STLCOMP || m_nLcomType == D_LCOMTYPE_SRC)
			{
				if (nType == 10) LcomD.nActive = 8;  //×¼ÓÀ¾Ã×éºÏ
				if (nType == 11 || nType == 100) LcomD.nActive = 7;  //ÆµÓö×éºÏ
			}
			LoadCaseNo = GetCountLcomCase(LcomD);

			BOOL bAddLcomD = FALSE;

			// moving load
			if (m_nMCount > 0)
			{
				Set_MovingCase(arMLoad.GetAt(m), dLFactor / (1. + dIL), LoadCaseNo, LcomD, TRUE);
				if (nType == 12 || dIL < m_dZeroLim) strDesc.Format(_T("+%sM"), GetFactorString(dLFactor));
				else                            strDesc.Format(_T("+%s/(1+mu)M"), GetFactorString(dLFactor));
				LcomD.Description += strDesc;
				if (m_nMCount > 1) { strDesc.Format(_T("[%d]"), m + 1); LcomD.Description += strDesc; }
				bAddLcomD = TRUE;
			}

			if (m_nLCount > 0 || (bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(L)))) // ST or ST+CS
			{
				int nLL = 0;
				for (nc = 0; nc < m_nLCount; nc++)
					nLL += Set_STLCIncludeCS(arLLoad.GetAt(nc), dLFactor, LoadCaseNo, LcomD, 0);
				if (nLL > 0) { strDesc.Format(_T("+%sL"), GetFactorString(dLFactor)); LcomD.Description = LcomD.Description + strDesc; }
				if (bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(L)))
				{
					Set_CStageCase(m_aCSLoad.GetAt(nnn), dLFactor, LoadCaseNo, LcomD, TRUE);
					strDesc.Format(_T("+%s(cEL)"), GetFactorString(dLFactor)); LcomD.Description = LcomD.Description + strDesc;
					bAddLcomD = TRUE;
				}
				int nIL = 0;
				for (nc = 0; nc < m_nIlCount; nc++)
					nIL += Set_STLCIncludeCS(arIlLoad.GetAt(nc), dLFactor, LoadCaseNo, LcomD, 0);
				if (nIL > 0) { strDesc.Format(_T("+%sIL"), GetFactorString(dLFactor)); LcomD.Description = LcomD.Description + strDesc; }
				if (nLL + nIL > 0) bAddLcomD = TRUE;
			}

			if (m_nMCount > 0 || (m_nLCount > 0 || (bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(L)))))
			{
				int nCF = 0, nIL = 0, nILP = 0, nCO = 0;
				for (nc = 0; nc < m_nCfCount; nc++)    // ÀëÐÄÁ¦
					nCF += Set_STLCIncludeCS(arCfLoad.GetAt(nc), dCfFactor, LoadCaseNo, LcomD, 0);
				if (nCF > 0) { strDesc.Format(_T("+%sCF"), GetFactorString(dCfFactor)); LcomD.Description = LcomD.Description + strDesc; }
				for (nc = 0; nc < m_nIlCount; nc++)    // »îºÉÔØ³å»÷
					nIL += Set_STLCIncludeCS(arIlLoad.GetAt(nc), dCfFactor, LoadCaseNo, LcomD, 0);
				if (nIL > 0) { strDesc.Format(_T("+%sIL"), GetFactorString(dCfFactor)); LcomD.Description = LcomD.Description + strDesc; }
				for (nc = 0; nc < m_nIlpCount; nc++)    // ³¬ÔØ³å»÷Á¦
					nILP += Set_STLCIncludeCS(arIlpLoad.GetAt(nc), dCfFactor, LoadCaseNo, LcomD, 0);
				if (nILP > 0) { strDesc.Format(_T("+%sCF"), GetFactorString(dCfFactor)); LcomD.Description = LcomD.Description + strDesc; }
				for (nc = 0; nc < m_nCoCount; nc++)    // ³å»÷Á¦
					nCO += Set_STLCIncludeCS(arCoLoad.GetAt(nc), dCfFactor, LoadCaseNo, LcomD, 0);
				if (nCO > 0) { strDesc.Format(_T("+%sCO"), GetFactorString(dCfFactor)); LcomD.Description = LcomD.Description + strDesc; }
				if (nCF + nIL + nILP + nCO > 0) bAddLcomD = TRUE;
			}
			//if(bAddLcomD) Set_LoadComb4Civil(LcomD, D_SER, nServEnvType, nMsgType);

			if (m_nLsCount > 0) // Live Load Surcharge µØÃæ¸½¼Ó»îºÉÔØ
			{
				LcomD = aLcomD.GetAt(lc); LcomD.nActive = 1; LoadCaseNo = GetCountLcomCase(LcomD);
				if (m_nLcomType == D_LCOMTYPE_CONCRETE)
				{
					if (nType == 10) LcomD.nActive = 8;  //×¼ÓÀ¾Ã×éºÏ
					if (nType == 11 || nType == 100) LcomD.nActive = 7;  //ÆµÓö×éºÏ
					if (nType == 12)
					{
						LcomD.bElasticStage = TRUE;
						LcomD.nActive = 2;//±ê×¼×éºÏ
					}
				}
				else if (m_nLcomType == D_LCOMTYPE_STLCOMP || m_nLcomType == D_LCOMTYPE_SRC)
				{
					if (nType == 10) LcomD.nActive = 8;  //×¼ÓÀ¾Ã×éºÏ
					if (nType == 11 || nType == 100) LcomD.nActive = 7;  //ÆµÓö×éºÏ
				}
				int nLS = 0;
				for (nc = 0; nc < m_nLsCount; nc++) nLS += Set_STLCIncludeCS(arLsLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD, 0);
				if (nLS > 0)
				{
					strDesc.Format(_T("+%sLS"), GetFactorString(dFactor)); LcomD.Description += strDesc;
					Set_LoadComb4Civil(LcomD, D_SER, nServEnvType, nMsgType);
				}
			}
			// MNET:XXXX-BJLEE(BBONG)-20060503

			if (m_nCrlCount > 0 || m_nMCrlCount > 0) // Crowd Load ÈËÈººÉÔØ
			{
				LcomD = aLcomD.GetAt(lc); LcomD.nActive = 1; LoadCaseNo = GetCountLcomCase(LcomD);
				if (m_nLcomType == D_LCOMTYPE_CONCRETE)
				{
					if (nType == 10) LcomD.nActive = 8;  //×¼ÓÀ¾Ã×éºÏ
					if (nType == 11 || nType == 100) LcomD.nActive = 7;  //ÆµÓö×éºÏ
					if (nType == 12)
					{
						LcomD.bElasticStage = TRUE;
						LcomD.nActive = 2;//±ê×¼×éºÏ
					}
				}
				else if (m_nLcomType == D_LCOMTYPE_STLCOMP || m_nLcomType == D_LCOMTYPE_SRC)
				{
					if (nType == 10) LcomD.nActive = 8;  //×¼ÓÀ¾Ã×éºÏ
					if (nType == 11 || nType == 100) LcomD.nActive = 7;  //ÆµÓö×éºÏ
				}
				int nCRL = 0;
				for (nc = 0; nc < m_nCrlCount; nc++) nCRL += Set_STLCIncludeCS(arCrlLoad.GetAt(nc), dCRLFactor, LoadCaseNo, LcomD, 0);
				if (nCRL > 0) { strDesc.Format(_T("+%sCRL"), GetFactorString(dCRLFactor)); LcomD.Description += strDesc; }

				int nMCrlCount = (m_nMCrlCount > 0) ? m_nMCrlCount : ((nCRL > 0) ? 1 : 0);
				for (int crl = 0; crl < nMCrlCount; ++crl)
				{
					T_LCOM_D LcomD_crl = LcomD; LoadCaseNo = GetCountLcomCase(LcomD_crl);
					if (m_nMCrlCount > 0)
					{
						Set_MovingCase(arMCrlLoad.GetAt(crl), dCRLFactor, LoadCaseNo, LcomD_crl, TRUE);
						strDesc.Format(_T("+%sMCRL"), GetFactorString(dCRLFactor)); LcomD_crl.Description += strDesc;
						if (m_nMCrlCount > 1) { strDesc.Format(_T("[%d]"), crl + 1); LcomD_crl.Description += strDesc; }
					}
					if (nCRL > 0 || m_nMCrlCount > 0)
					{
						if (nType == 2 || nType == 4) m_aAcdnLcomD.Add(LcomD_crl);
						else                     Set_LoadComb4Civil(LcomD_crl, D_SER, nServEnvType, nMsgType);
					}
				}
			}

			if (bUseWlLoad)
			{
				int nWCount = m_nWlCount * 2; // MNET:1614, KJH20050808 ·çºÉÔØ
				if (m_nWlCount == 0 && bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(WL))) nWCount = 1;
				for (nc = 0; nc < nWCount; nc++)
				{
					LcomD = aLcomD.GetAt(lc); LcomD.nActive = 1; LoadCaseNo = GetCountLcomCase(LcomD);
					if (m_nLcomType == D_LCOMTYPE_CONCRETE)
					{
						if (nType == 10) LcomD.nActive = 8;  //×¼ÓÀ¾Ã×éºÏ
						if (nType == 11 || nType == 100) LcomD.nActive = 7;  //ÆµÓö×éºÏ
						if (nType == 12)
						{
							LcomD.bElasticStage = TRUE;
							LcomD.nActive = 2;//±ê×¼×éºÏ
						}
					}
					else if (m_nLcomType == D_LCOMTYPE_STLCOMP || m_nLcomType == D_LCOMTYPE_SRC)
					{
						if (nType == 10) LcomD.nActive = 8;  //×¼ÓÀ¾Ã×éºÏ
						if (nType == 11 || nType == 100) LcomD.nActive = 7;  //ÆµÓö×éºÏ
					}
					if (m_nWlCount > 0)
					{
						double dWF = (nc % 2 == 0) ? dWlFactor : -1.*dWlFactor;
						if (Set_STLCIncludeCS(arWlLoad.GetAt(nc / 2), dWF, LoadCaseNo, LcomD, 0) > 0)
						{
							CString strW = (dWF > 0.0) ? _T("+%sWL") : _T("%sWL");
							strDesc.Format(strW, GetFactorString(dWF)); LcomD.Description += strDesc;
							if (m_nWlCount > 1) { strDesc.Format(_T("[%d]"), nc / 2 + 1); LcomD.Description += strDesc; }
						}
					}
					if (bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(WL)))
					{
						Set_CStageCase(m_aCSLoad.GetAt(nnn), dWlFactor, LoadCaseNo, LcomD, TRUE);
						strDesc.Format(_T("+%s(cEL)"), GetFactorString(dWlFactor)); LcomD.Description += strDesc;
					}
					Set_LoadComb4Civil(LcomD, D_SER, nServEnvType, nMsgType);
				}
			}
			else
			{
				int nWCount = m_nWCount * 2; // MNET:1614, KJH20050808 ·çºÉÔØ
				if (m_nWCount == 0 && bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(W))) nWCount = 1;
				for (nc = 0; nc < nWCount; nc++)
				{
					LcomD = aLcomD.GetAt(lc); LcomD.nActive = 1; LoadCaseNo = GetCountLcomCase(LcomD);
					if (m_nLcomType == D_LCOMTYPE_CONCRETE)
					{
						if (nType == 10) LcomD.nActive = 8;  //×¼ÓÀ¾Ã×éºÏ
						if (nType == 11 || nType == 100) LcomD.nActive = 7;  //ÆµÓö×éºÏ
						if (nType == 12)
						{
							LcomD.bElasticStage = TRUE;
							LcomD.nActive = 2;//±ê×¼×éºÏ
						}
					}
					else if (m_nLcomType == D_LCOMTYPE_STLCOMP || m_nLcomType == D_LCOMTYPE_SRC)
					{
						if (nType == 10) LcomD.nActive = 8;  //×¼ÓÀ¾Ã×éºÏ
						if (nType == 11 || nType == 100) LcomD.nActive = 7;  //ÆµÓö×éºÏ
					}
					if (m_nWCount > 0)
					{
						double dWF = (nc % 2 == 0) ? dWFactor : -1.*dWFactor;
						if (Set_STLCIncludeCS(arWLoad.GetAt(nc / 2), dWF, LoadCaseNo, LcomD, 0) > 0)
						{
							CString strW = (dWF > 0.0) ? _T("+%sW") : _T("%sW");
							strDesc.Format(strW, GetFactorString(dWF)); LcomD.Description += strDesc;
							if (m_nWCount > 1) { strDesc.Format(_T("[%d]"), nc / 2 + 1); LcomD.Description += strDesc; }
						}
					}
					if (bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(W)))
					{
						Set_CStageCase(m_aCSLoad.GetAt(nnn), dWFactor, LoadCaseNo, LcomD, TRUE);
						strDesc.Format(_T("+%s(cEL)"), GetFactorString(dWFactor)); LcomD.Description += strDesc;
					}
					Set_LoadComb4Civil(LcomD, D_SER, nServEnvType, nMsgType);
				}
			}
			if (m_nSfCount > 0) // Stream Flow Pressure Á÷Ë®Ñ¹Á¦
			{
				LcomD = aLcomD.GetAt(lc); LcomD.nActive = 1; LoadCaseNo = GetCountLcomCase(LcomD);
				if (m_nLcomType == D_LCOMTYPE_CONCRETE)
				{
					if (nType == 10) LcomD.nActive = 8;  //×¼ÓÀ¾Ã×éºÏ
					if (nType == 11 || nType == 100) LcomD.nActive = 7;  //ÆµÓö×éºÏ
					if (nType == 12)
					{
						LcomD.bElasticStage = TRUE;
						LcomD.nActive = 2;//±ê×¼×éºÏ
					}
				}
				else if (m_nLcomType == D_LCOMTYPE_STLCOMP || m_nLcomType == D_LCOMTYPE_SRC)
				{
					if (nType == 10) LcomD.nActive = 8;  //×¼ÓÀ¾Ã×éºÏ
					if (nType == 11) LcomD.nActive = 7;  //ÆµÓö×éºÏ
				}
				int nSF = 0;
				for (nc = 0; nc < m_nSfCount; nc++) nSF += Set_STLCIncludeCS(arSfLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD, 0);
				if (nSF > 0)
				{
					strDesc.Format(_T("+%sSF"), GetFactorString(dFactor)); LcomD.Description += strDesc;
					Set_LoadComb4Civil(LcomD, D_SER, nServEnvType, nMsgType);
				}
			}
			if (m_nIpCount > 0) // Ice Pressure ±ùÑ¹Á¦
			{
				LcomD = aLcomD.GetAt(lc); LcomD.nActive = 1; LoadCaseNo = GetCountLcomCase(LcomD);
				if (m_nLcomType == D_LCOMTYPE_CONCRETE)
				{
					if (nType == 10) LcomD.nActive = 8;  //×¼ÓÀ¾Ã×éºÏ
					if (nType == 11 || nType == 100) LcomD.nActive = 7;  //ÆµÓö×éºÏ
					if (nType == 12)
					{
						LcomD.bElasticStage = TRUE;
						LcomD.nActive = 2;//±ê×¼×éºÏ
					}
				}
				else if (m_nLcomType == D_LCOMTYPE_STLCOMP || m_nLcomType == D_LCOMTYPE_SRC)
				{
					if (nType == 10) LcomD.nActive = 8;  //×¼ÓÀ¾Ã×éºÏ
					if (nType == 11 || nType == 100) LcomD.nActive = 7;  //ÆµÓö×éºÏ
				}
				int nIP = 0;
				for (nc = 0; nc < m_nIpCount; nc++) nIP += Set_STLCIncludeCS(arIpLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD, 0);
				if (nIP > 0)
				{
					strDesc.Format(_T("+%sIP"), GetFactorString(dFactor)); LcomD.Description += strDesc;
					Set_LoadComb4Civil(LcomD, D_SER, nServEnvType, nMsgType);
				}
			}
			// Temeperature ÎÂ¶ÈºÉÔØ
			if ((m_nTCount > 0 || m_nTpgCount > 0) || (bAddCsLoad && (CheckCSEL(D_SGLD_ER1_CVL, _LSX(T)) || CheckCSEL(D_SGLD_ER1_CVL, _LSX(TPG)))))
			{
				LcomD = aLcomD.GetAt(lc); LcomD.nActive = 1;
				if (m_nLcomType == D_LCOMTYPE_CONCRETE)
				{
					if (nType == 10) LcomD.nActive = 8;  //×¼ÓÀ¾Ã×éºÏ
					if (nType == 11 || nType == 100) LcomD.nActive = 7;  //ÆµÓö×éºÏ
					if (nType == 12)
					{
						LcomD.bElasticStage = TRUE;
						LcomD.nActive = 2;//±ê×¼×éºÏ
					}
				}
				else if (m_nLcomType == D_LCOMTYPE_STLCOMP || m_nLcomType == D_LCOMTYPE_SRC)
				{
					if (nType == 10) LcomD.nActive = 8;  //×¼ÓÀ¾Ã×éºÏ
					if (nType == 11 || nType == 100) LcomD.nActive = 7;  //ÆµÓö×éºÏ
				}
				SetCvlJTJD60_Capacity_TPG(LcomD, arTLoad, arTpgLoad, arFrLoad, dFactor, dTPGFactor, FALSE, FALSE, nMsgType, D_SER, nServEnvType);
			}
			if (m_nFrCount > 0) // Friction Ä¦²ÁÁ¦
			{
				LcomD = aLcomD.GetAt(lc); LcomD.nActive = 1; LoadCaseNo = GetCountLcomCase(LcomD);
				if (m_nLcomType == D_LCOMTYPE_CONCRETE)
				{
					if (nType == 10) LcomD.nActive = 8;  //×¼ÓÀ¾Ã×éºÏ
					if (nType == 11 || nType == 100) LcomD.nActive = 7;  //ÆµÓö×éºÏ
					if (nType == 12)
					{
						LcomD.bElasticStage = TRUE;
						LcomD.nActive = 2;//±ê×¼×éºÏ
					}
				}
				else if (m_nLcomType == D_LCOMTYPE_STLCOMP || m_nLcomType == D_LCOMTYPE_SRC)
				{
					if (nType == 10) LcomD.nActive = 8;  //×¼ÓÀ¾Ã×éºÏ
					if (nType == 11 || nType == 100) LcomD.nActive = 7;  //ÆµÓö×éºÏ
				}
				int nFR = 0;
				for (nc = 0; nc < m_nFrCount; nc++) nFR += Set_STLCIncludeCS(arFrLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD, 0);
				if (nFR > 0)
				{
					strDesc.Format(_T("+%sFR"), GetFactorString(dFactor)); LcomD.Description += strDesc;
					Set_LoadComb4Civil(LcomD, D_SER, nServEnvType, nMsgType);
				}
			}
		} // next m
	} // next lc

	// ¿µ±¸ÇÏÁß + Â÷·®ÇÏÁß + 1°³ÀÇ ±âÅ¸ °¡º¯ÇÏÁßÀÎ °æ¿ì(8°³)
	SetCvlJTGD60_15_Var1_S(arData, arMLoad, dIL, arLLoad, arIlLoad, arCrlLoad, arMCrlLoad, arCfLoad, arLsLoad, arWLoad, arWlLoad, arSfLoad, arIpLoad,
		arTLoad, arTpgLoad, arFrLoad, arBrkLoad, nType, nMsgType);
	// ¿µ±¸ÇÏÁß + Â÷·®ÇÏÁß + 2°³ÀÇ ±âÅ¸ °¡º¯ÇÏÁßÀÎ °æ¿ì(24°³)
	SetCvlJTGD60_15_Var2_S(arData, arMLoad, dIL, arLLoad, arIlLoad, arCrlLoad, arMCrlLoad, arCfLoad, arLsLoad, arWLoad, arWlLoad, arSfLoad, arIpLoad,
		arTLoad, arTpgLoad, arFrLoad, arBrkLoad, nType, nMsgType);
	// ¿µ±¸ÇÏÁß + Â÷·®ÇÏÁß + 3°³ÀÇ ±âÅ¸ °¡º¯ÇÏÁßÀÎ °æ¿ì(36°³)
	SetCvlJTGD60_15_Var3_S(arData, arMLoad, dIL, arLLoad, arIlLoad, arCrlLoad, arMCrlLoad, arCfLoad, arLsLoad, arWLoad, arWlLoad, arSfLoad, arIpLoad,
		arTLoad, arTpgLoad, arFrLoad, arBrkLoad, nType, nMsgType);
	// ¿µ±¸ÇÏÁß + Â÷·®ÇÏÁß + 4°³ÀÇ ±âÅ¸ °¡º¯ÇÏÁßÀÎ °æ¿ì(29°³)
	SetCvlJTGD60_15_Var4_S(arData, arMLoad, dIL, arLLoad, arIlLoad, arCrlLoad, arMCrlLoad, arCfLoad, arLsLoad, arWLoad, arWlLoad, arSfLoad, arIpLoad,
		arTLoad, arTpgLoad, arFrLoad, arBrkLoad, nType, nMsgType);
	// ¿µ±¸ÇÏÁß + Â÷·®ÇÏÁß + 5°³ ¹× 6°³ÀÇ ±âÅ¸ °¡º¯ÇÏÁßÀÎ °æ¿ì(12°³ ¹× 2°³)
	SetCvlJTGD60_15_Var56_S(arData, arMLoad, dIL, arLLoad, arIlLoad, arCrlLoad, arMCrlLoad, arCfLoad, arLsLoad, arWLoad, arWlLoad, arSfLoad, arIpLoad,
		arTLoad, arTpgLoad, arFrLoad, arBrkLoad, nType, nMsgType);

	return TRUE;
}

BOOL CLoadCombCtrl::SetCvlJTJD60_15_Accident_Ct(ArINT& arCtLoad)//³µÁ¾×²»÷Á¦
{
	// vessel collision load(Â÷·®Ãæµ¹ÇÏÁß)
	int LoadCaseNo = 0;
	CString strDesc = _T("");

	int nLcomSize = m_aAcdnLcomD.GetSize();
	for (int lc = 0; lc < nLcomSize; lc++)
	{
		for (int c = 0; c < m_nCtCount; c++)
		{
			T_LCOM_D LcomD = m_aAcdnLcomD.GetAt(lc); LoadCaseNo = GetCountLcomCase(LcomD);
			int nCT = Set_STLCIncludeCS(arCtLoad.GetAt(c), 1.0, LoadCaseNo, LcomD, 0);
			if (nCT > 0)
			{
				strDesc.Format(_T("+%sCT"), GetFactorString(1.0)); LcomD.Description += strDesc;
				if (m_nCtCount > 1) { strDesc.Format(_T("[%d]"), c + 1); LcomD.Description += strDesc; }
				Set_LoadComb4Civil(LcomD, D_STR, 0, 104); // ¿ì¿¬Á¶ÇÕ
			}
		}
	}
	return TRUE;
}


BOOL CLoadCombCtrl::SetCvlJTJD60_15_Accident_Cv(ArINT& arCvLoad)//´¬Ö»»òÆ¯Á÷Îï×²»÷Á¦
{
	int LoadCaseNo = 0;
	CString strDesc = _T("");

	int nLcomSize = m_aAcdnLcomD.GetSize();
	for (int lc = 0; lc < nLcomSize; lc++)
	{
		for (int c = 0; c < m_nCvCount; c++)
		{
			T_LCOM_D LcomD = m_aAcdnLcomD.GetAt(lc); LoadCaseNo = GetCountLcomCase(LcomD);
			int nCV = Set_STLCIncludeCS(arCvLoad.GetAt(c), 1.0, LoadCaseNo, LcomD, 0);
			if (nCV > 0)
			{
				strDesc.Format(_T("+%sCV"), GetFactorString(1.0));  LcomD.Description += strDesc;
				if (m_nCvCount > 1) { strDesc.Format(_T("[%d]"), c + 1);  LcomD.Description += strDesc; }
				Set_LoadComb4Civil(LcomD, D_STR, 0, 104); // ¿ì¿¬Á¶ÇÕ
			}
		}
	}

	return TRUE;
}

BOOL CLoadCombCtrl::SetCvlJTGD60_15_Var1_M(const CArray<T_LCOM_D, T_LCOM_D&>& arData, ArUNT& arMLoad,
	ArINT& arLLoad, ArINT& arIlLoad, ArINT& arCrlLoad, ArINT& arMCrlLoad,
	ArINT& arCfLoad, ArINT& arLsLoad, ArINT& arWLoad, ArINT& arWlLoad, ArINT& arSfLoad, ArINT& arIpLoad,
	ArINT& arTLoad, ArINT& arTpgLoad, ArINT& arFrLoad, ArINT& arBrkLoad, int nType, int nMsgType)
{
	// nType == 0,4: °è¼öÁ¶ÇÕ, 1: 1.0(settlement Á¦¿Ü), 2: 1.0(¸ðµÎ), Live Load Á¦¿Ü
	CArray<T_LCOM_D, T_LCOM_D&> aLcomD;
	aLcomD.Copy(arData);
	int  nLcomSize = aLcomD.GetSize();
	int  nnn = 0;
	//BOOL bAddCsLoad = (m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER1_CVL,nnn)) || (m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER2_CVL,nnn)) || (m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER3_CVL,nnn)); // CS or ST+CS
	BOOL bAddCsLoad = m_nConstLoad != 0 && CheckCS_SgldK(D_SGLD_ER1_CVL, nnn); // °üº¬Ê©¹¤ºÉÔØÇÒ´æÔÚÊ©¹¤ºÉÔØ

	double dFactor = (nType == 0 || nType == 4) ? 1.4 * 0.75 : 1.0;// ¸½¼ÓµØÃæ»îºÉÔØ ÈËÈººÉÔØ Á÷Ë®Ñ¹Á¦ ±ùºÉÔØ ÎÂ¶ÈºÉÔØ Ä¦²ÁÁ¦
	double dWFactor = (nType == 0 || nType == 4) ? 1.4 * 1.0 : 1.0;// ·çºÉÔØ
	double dCJJMLFactor = (nType == 0 || nType == 4) ? 1.1 : 1.0;
	double dLFactor = (nType == 0 || nType == 4) ? 1.4 : 1.0;   // Ê©¹¤ºÉÔØ »îÔØ³å»÷
	double dIL = (nType == 12) ? 0. : CalcMovingILFactor();//³å»÷ÏµÊý

	double dWlFactor = (nType == 0 || nType == 4) ? 1.1 * 1.0 : 1.0;// ·çºÉÔØ
	BOOL bExsitMV = m_nMCount ? TRUE : FALSE;

	if (nMsgType == 114 && m_bJTG15_Factor)
	{
		dFactor *= m_dJTG15_Factor;
		dWFactor *= m_dJTG15_Factor;
		dWlFactor *= m_dJTG15_Factor;
		dCJJMLFactor *= m_dJTG15_Factor;
		dLFactor *= m_dJTG15_Factor;
	}

	// ÓÀ¾Ã×÷ÓÃ + ÒÆ¶¯ºÉÔØ + 1¿É±ä×÷ÓÃ µÄÇé¿ö
	CString strDesc = _T("");
	int LoadCaseNo = 0, nc = 0;
	for (int lc = 0; lc < nLcomSize; lc++)
	{
		int nMCount = max(m_nMCount, 1);
		for (int m = 0; m < nMCount; m++)
		{
			T_LCOM_D LcomD = aLcomD.GetAt(lc); LcomD.nActive = 1; // Active or Strength/Stress
			T_LCOM_D LcomD_cf = LcomD; // MNET:1614, KJH20050809, cf°¡ Á¦¿ÜµÈ load combination
			LoadCaseNo = GetCountLcomCase(LcomD);

			double dMlFactor = 1.4;               // Æû³µºÉÔØ×éºÏÏµÊý

			CString strJudgeMILFactorByLoadCase = GetJTGD602015MILoadName(arMLoad, m);

			if (nType == 0 || nType == 1)         // ³ÐÔØÄÜÁ¦×´Ì¬  »ù±¾×éºÏ
			{
				if (strJudgeMILFactorByLoadCase == _T("CH-CL") || strJudgeMILFactorByLoadCase == _T("ÓÃ»§¶¨Òå"))
				{
					dMlFactor = 1.8;                  // ³µÁ¾ºÉÔØ  ÓÃ»§¶¨Òå   È¡1.8
				}
			}
			if (nType == 17)  dMlFactor = 1.0;
			if (nMsgType == 114 && m_bJTG15_Factor)
			{
				dMlFactor *= m_dJTG15_Factor;
			}

			BOOL bLive = FALSE, bCF = FALSE;
			if (nType != 2)
			{
				BOOL bAddLcomD = FALSE;
				// moving load ÒÆ¶¯ºÉÔØ
				if (m_nMCount > 0)
				{
					Set_MovingCase(arMLoad.GetAt(m), dMlFactor, LoadCaseNo, LcomD, TRUE);
					strDesc.Format(_T("+%sM"), GetFactorString(dMlFactor)); LcomD.Description += strDesc;
					if (m_nMCount > 1) { strDesc.Format(_T("[%d]"), m + 1); LcomD.Description += strDesc; }
					bAddLcomD = TRUE;
				}

				if (m_nLCount > 0 || (bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(L)))) // ST or ST+CS
				{
					int nLL = 0, nIL = 0;
					for (nc = 0; nc < m_nLCount; nc++)
						nLL += Set_STLCIncludeCS(arLLoad.GetAt(nc), dLFactor, LoadCaseNo, LcomD, 0);
					if (nLL > 0) { strDesc.Format(_T("+%sL"), GetFactorString(dLFactor)); LcomD.Description += strDesc; }
					if (bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(L)))
					{
						Set_CStageCase(m_aCSLoad.GetAt(nnn), dLFactor, LoadCaseNo, LcomD, TRUE);
						strDesc.Format(_T("+%s(cEL)"), GetFactorString(dLFactor)); LcomD.Description += strDesc;
						bAddLcomD = TRUE;
					}
					for (nc = 0; nc < m_nIlCount; nc++)//³å»÷
						nIL += Set_STLCIncludeCS(arIlLoad.GetAt(nc), dLFactor, LoadCaseNo, LcomD, 0);
					if (nIL > 0) { strDesc.Format(_T("+%sIL"), GetFactorString(dLFactor)); LcomD.Description += strDesc; }
					if (nLL + nIL > 0) bAddLcomD = TRUE;
				}
				bLive = m_nMCount > 0 || (m_nLCount > 0 || (bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(L))));
				if (bLive)//ÀëÐÄÁ¦
				{
					LcomD_cf = LcomD;
					int nCF = 0;
					for (nc = 0; nc < m_nCfCount; nc++)
						nCF += Set_STLCIncludeCS(arCfLoad.GetAt(nc), dLFactor, LoadCaseNo, LcomD, 0);
					if (nCF > 0) { strDesc.Format(_T("+%sCF"), GetFactorString(dLFactor)); LcomD.Description += strDesc; bCF = TRUE; }
					bAddLcomD = TRUE;
				}
				if (bAddLcomD)
				{
					if (nType == 4) m_aAcdnLcomD.Add(LcomD);
					else         Set_LoadComb4Civil(LcomD, D_STR, 0, nMsgType);
				}
			}

			if (m_nLsCount > 0) // Live Load Surcharge  ¸½¼ÓµØÃæ»îºÉÔØ
			{
				T_LCOM_D LcomD_l = LcomD; LoadCaseNo = GetCountLcomCase(LcomD_l);
				int nLS = 0;
				for (nc = 0; nc < m_nLsCount; nc++) nLS += Set_STLCIncludeCS(arLsLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD_l, 0);
				if (nLS > 0)
				{
					strDesc.Format(_T("+%sLS"), GetFactorString(dFactor)); LcomD_l.Description += strDesc;
					if (nType == 2 || nType == 4) m_aAcdnLcomD.Add(LcomD_l);
					else                     Set_LoadComb4Civil(LcomD_l, D_STR, 0, nMsgType);
				}
			}

			if (nType != 2 && (m_nCrlCount > 0 || m_nMCrlCount > 0)) // Crowd Load ÈËÈººÉÔØ
			{
				T_LCOM_D LcomD_c = LcomD;
				LoadCaseNo = GetCountLcomCase(LcomD_c);
				int nCRL = 0;
				for (nc = 0; nc < m_nCrlCount; nc++) nCRL += Set_STLCIncludeCS(arCrlLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD_c, 0);
				if (nCRL > 0) { strDesc.Format(_T("+%sCRL"), GetFactorString(dFactor)); LcomD_c.Description += strDesc; }

				int nMCrlCount = (m_nMCrlCount > 0) ? m_nMCrlCount : ((nCRL > 0) ? 1 : 0);
				for (int crl = 0; crl < nMCrlCount; ++crl)
				{
					T_LCOM_D LcomD_crl = LcomD_c; LoadCaseNo = GetCountLcomCase(LcomD_crl);
					if (m_nMCrlCount > 0)
					{
						Set_MovingCase(arMCrlLoad.GetAt(crl), dFactor, LoadCaseNo, LcomD_crl, TRUE);
						strDesc.Format(_T("+%sMCRL"), GetFactorString(dFactor)); LcomD_crl.Description += strDesc;
						if (m_nMCrlCount > 1) { strDesc.Format(_T("[%d]"), crl + 1); LcomD_crl.Description += strDesc; }
					}
					if (nCRL > 0 || m_nMCrlCount > 0)
					{
						if (nType == 2 || nType == 4) m_aAcdnLcomD.Add(LcomD_crl);
						else                     Set_LoadComb4Civil(LcomD_crl, D_STR, 0, nMsgType);
					}
				}
			}

			// ÖÆ¶¯ºÉÔØ
			if (m_nBrkCount > 0 && bLive) // Brake Load 
			{
				T_LCOM_D LcomD_l = LcomD; LoadCaseNo = GetCountLcomCase(LcomD_l);
				double dBrkF = (nType == 0 || nType == 4) ? (bCF ? dFactor * 0.7 : dFactor) : 1.0;
				int nBRK = 0;
				for (nc = 0; nc < m_nBrkCount; nc++)
					nBRK += Set_STLCIncludeCS(arBrkLoad.GetAt(nc), dBrkF, LoadCaseNo, LcomD_l, 0);
				if (nBRK > 0)
				{
					strDesc.Format(_T("+%sBRK"), GetFactorString(dBrkF)); LcomD_l.Description += strDesc;
					if (nType == 2 || nType == 4) m_aAcdnLcomD.Add(LcomD_l);
					else                     Set_LoadComb4Civil(LcomD_l, D_STR, 0, nMsgType);
				}
				if (bCF)//´æÔÚÀëÐÄÁ¦
				{
					LcomD_l = LcomD_cf; LoadCaseNo = GetCountLcomCase(LcomD_l);
					dBrkF = (nType == 0 || nType == 4) ? dFactor : 1.0;
					for (nc = 0; nc < m_nBrkCount; nc++)
						nBRK += Set_STLCIncludeCS(arBrkLoad.GetAt(nc), dBrkF, LoadCaseNo, LcomD_l, 0);
					if (nBRK > 0)
					{
						strDesc.Format(_T("+%sBRK"), GetFactorString(dBrkF));
						LcomD_l.Description += strDesc;
						if (nType == 2 || nType == 4) m_aAcdnLcomD.Add(LcomD_l);
						else                     Set_LoadComb4Civil(LcomD_l, D_STR, 0, nMsgType);
					}
				}
			}

			{
				int nWlCount = m_nWlCount * 2; // ·çºÉÔØ
				if (m_nWlCount == 0 && bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(WL))) nWlCount = 1;
				for (nc = 0; nc < nWlCount; nc++)
				{
					T_LCOM_D LcomD_l = LcomD; LoadCaseNo = GetCountLcomCase(LcomD_l);
					if (m_nWlCount > 0)
					{
						double dWF = (nc % 2 == 0) ? dWlFactor : -1.0*dWlFactor;
						if (Set_STLCIncludeCS(arWlLoad.GetAt(nc / 2), dWF, LoadCaseNo, LcomD_l, 0) > 0)
						{
							CString strW = (dWF > 0.0) ? _T("+%sWL") : _T("%sWL");
							strDesc.Format(strW, GetFactorString(dWF)); LcomD_l.Description += strDesc;
							if (m_nWlCount > 1) { strDesc.Format(_T("[%d]"), nc / 2 + 1); LcomD_l.Description += strDesc; }
						}
					}
					if (bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(WL)))
					{
						Set_CStageCase(m_aCSLoad.GetAt(nnn), dWlFactor, LoadCaseNo, LcomD_l, TRUE);
						strDesc.Format(_T("+%s(cEL)"), GetFactorString(dWlFactor)); LcomD_l.Description += strDesc;
					}
					if (nType == 2 || nType == 4) m_aAcdnLcomD.Add(LcomD_l);
					else                     Set_LoadComb4Civil(LcomD_l, D_STR, 0, nMsgType);
				}
			}

			if (!bExsitMV)
			{
				int nWCount = m_nWCount * 2; // ·çºÉÔØ
				if (m_nWCount == 0 && bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(W))) nWCount = 1;
				for (nc = 0; nc < nWCount; nc++)
				{
					T_LCOM_D LcomD_l = LcomD; LoadCaseNo = GetCountLcomCase(LcomD_l);
					if (m_nWCount > 0)
					{
						double dWF = (nc % 2 == 0) ? dWFactor : -1.*dWFactor;
						if (Set_STLCIncludeCS(arWLoad.GetAt(nc / 2), dWF, LoadCaseNo, LcomD_l, 0) > 0)
						{
							CString strW = (dWF > 0.0) ? _T("+%sW") : _T("%sW");
							strDesc.Format(strW, GetFactorString(dWF)); LcomD_l.Description += strDesc;
							if (m_nWCount > 1) { strDesc.Format(_T("[%d]"), nc / 2 + 1); LcomD_l.Description += strDesc; }
						}
					}
					if (bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(W)))
					{
						Set_CStageCase(m_aCSLoad.GetAt(nnn), dWFactor, LoadCaseNo, LcomD_l, TRUE);
						strDesc.Format(_T("+%s(cEL)"), GetFactorString(dWFactor)); LcomD_l.Description += strDesc;
					}
					if (nType == 2 || nType == 4) m_aAcdnLcomD.Add(LcomD_l);
					else                     Set_LoadComb4Civil(LcomD_l, D_STR, 0, nMsgType);
				}
			}


			// Á÷Ë®Ñ¹Á¦
			if (m_nSfCount > 0) // Stream Flow Pressure
			{
				T_LCOM_D LcomD_l = LcomD; LoadCaseNo = GetCountLcomCase(LcomD_l);
				int nSF = 0;
				for (nc = 0; nc < m_nSfCount; nc++)
					nSF += Set_STLCIncludeCS(arSfLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD_l, 0);
				if (nSF > 0)
				{
					strDesc.Format(_T("+%sSF"), GetFactorString(dFactor)); LcomD_l.Description = LcomD_l.Description + strDesc;
					if (nType == 2 || nType == 4) m_aAcdnLcomD.Add(LcomD_l);
					else                     Set_LoadComb4Civil(LcomD_l, D_STR, 0, nMsgType);
				}
			}

			// ±ùºÉÔØ
			if (m_nIpCount > 0) // Ice Pressure
			{
				T_LCOM_D LcomD_l = LcomD; LoadCaseNo = GetCountLcomCase(LcomD_l);
				int nIP = 0;
				for (nc = 0; nc < m_nIpCount; nc++)
					nIP += Set_STLCIncludeCS(arIpLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD_l, 0);
				if (nIP > 0)
				{
					strDesc.Format(_T("+%sIP"), GetFactorString(dFactor)); LcomD_l.Description = LcomD_l.Description + strDesc;
					if (nType == 2 || nType == 4) m_aAcdnLcomD.Add(LcomD_l);
					else                     Set_LoadComb4Civil(LcomD_l, D_STR, 0, nMsgType);
				}
			}

			// Temeperature ÎÂ¶ÈºÉÔØ
			if ((m_nTCount > 0 || m_nTpgCount > 0) || (bAddCsLoad && (CheckCSEL(D_SGLD_ER1_CVL, _LSX(T)) || CheckCSEL(D_SGLD_ER1_CVL, _LSX(TPG)))))
			{
				T_LCOM_D LcomD_l = LcomD;
				SetCvlJTJD60_Capacity_TPG(LcomD_l, arTLoad, arTpgLoad, arFrLoad, dFactor, dFactor, FALSE, nType == 2 || nType == 4, nMsgType);
			}

			if (m_nFrCount > 0) // Friction Ä¦²ÁÁ¦
			{
				T_LCOM_D LcomD_l = LcomD; LoadCaseNo = GetCountLcomCase(LcomD_l);
				int nFR = 0;
				for (nc = 0; nc < m_nFrCount; nc++) nFR += Set_STLCIncludeCS(arFrLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD_l, 0);
				if (nFR > 0)
				{
					strDesc.Format(_T("+%sFR"), GetFactorString(dFactor)); LcomD_l.Description += strDesc;
					if (nType == 2 || nType == 4) m_aAcdnLcomD.Add(LcomD_l);
					else                     Set_LoadComb4Civil(LcomD_l, D_STR, 0, nMsgType);
				}
			}
		} // next m
	} // next lc

	return TRUE;
}

BOOL CLoadCombCtrl::SetCvlJTGD60_15_Var2_M(const CArray<T_LCOM_D, T_LCOM_D&>& arData, ArUNT& arMLoad,
	ArINT& arLLoad, ArINT& arIlLoad, ArINT& arCrlLoad, ArINT& arMCrlLoad,
	ArINT& arCfLoad, ArINT& arLsLoad, ArINT& arWLoad, ArINT& arWlLoad, ArINT& arSfLoad, ArINT& arIpLoad,
	ArINT& arTLoad, ArINT& arTpgLoad, ArINT& arFrLoad, ArINT& arBrkLoad, int nType, int nMsgType)
{
	// nType == 0,4: °è¼öÁ¶ÇÕ, 1: 1.0(settlement Á¦¿Ü), 2: 1.0(¸ðµÎ), Live Load Á¦¿Ü
	CArray<T_LCOM_D, T_LCOM_D&> aLcomD;
	aLcomD.Copy(arData);
	int  nLcomSize = aLcomD.GetSize();
	int  nnn = 0;
	//BOOL bAddCsLoad = (m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER1_CVL,nnn)) || (m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER2_CVL,nnn)) || (m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER3_CVL,nnn)); // CS or ST+CS
	BOOL bAddCsLoad = m_nConstLoad != 0 && CheckCS_SgldK(D_SGLD_ER1_CVL, nnn); // °üº¬Ê©¹¤ºÉÔØÇÒ´æÔÚÊ©¹¤ºÉÔØ

	double dFactor = (nType == 0 || nType == 4) ? 1.4*0.75 : 1.0;
	double dWFactor = (nType == 0 || nType == 4) ? 1.4*1.0 : 1.0;
	double dLFactor = (nType == 0 || nType == 4) ? 1.4 : 1.0;
	double dF = (nType == 0 || nType == 4) ? 0.75 : 1.0;
	double dCJJMLFactor = (nType == 0 || nType == 4) ? 1.1 : 1.0;
	double dIL = (nType == 12) ? 0. : CalcMovingILFactor();

	double dWlFactor = (nType == 0 || nType == 4) ? 1.1 * 1.0 : 1.0;// ·çºÉÔØ
	BOOL bExsitMV = m_nMCount ? TRUE : FALSE;

	if (nMsgType == 114 && m_bJTG15_Factor)
	{
		dFactor *= m_dJTG15_Factor;
		dWFactor *= m_dJTG15_Factor;
		dWlFactor *= m_dJTG15_Factor;
		dCJJMLFactor *= m_dJTG15_Factor;
		dLFactor *= m_dJTG15_Factor;
		dF *= m_dJTG15_Factor;
	}

	ArINT arTmp;

	// ÓÀ¾Ã×÷ÓÃ + Æû³µºÉÔØ + 2¿É±ä×÷ÓÃ
	CString strDesc = _T("");
	int LoadCaseNo = 0, nc = 0;
	for (int lc = 0; lc < nLcomSize; lc++)
	{
		int nMCount = max(m_nMCount, 1);
		for (int m = 0; m < nMCount; m++)
		{
			T_LCOM_D LcomD = aLcomD.GetAt(lc); LcomD.nActive = 1; // Active or Strength/Stress
			T_LCOM_D LcomD_cf = LcomD; // MNET:1614, KJH20050809, CF°¡ Á¦¿ÜµÈ load combination
			LoadCaseNo = GetCountLcomCase(LcomD);

			double dMlFactor = 1.4;               // Æû³µºÉÔØ×éºÏÏµÊý

			CString strJudgeMILFactorByLoadCase = GetJTGD602015MILoadName(arMLoad, m);

			if (nType == 0 || nType == 1)         // ³ÐÔØÄÜÁ¦×´Ì¬  »ù±¾×éºÏ
			{
				if (strJudgeMILFactorByLoadCase == _T("CH-CL") || strJudgeMILFactorByLoadCase == _T("ÓÃ»§¶¨Òå"))
				{
					dMlFactor = 1.8;                  // ³µÁ¾ºÉÔØ  ÓÃ»§¶¨Òå   È¡1.8
				}
			}
			if (nType == 17)  dMlFactor = 1.0;
			if (nMsgType == 114 && m_bJTG15_Factor)
			{
				dMlFactor *= m_dJTG15_Factor;
			}

			BOOL bLive = FALSE, bCF = FALSE;
			//BOOL bFindCJJLeadingMLoad = FALSE;
			if (nType != 2)
			{
				// moving load ÒÆ¶¯ºÉÔØ
				if (m_nMCount > 0)
				{
					Set_MovingCase(arMLoad.GetAt(m), dMlFactor, LoadCaseNo, LcomD, TRUE);
					strDesc.Format(_T("+%sM"), GetFactorString(dMlFactor)); LcomD.Description += strDesc;
					if (m_nMCount > 1) { strDesc.Format(_T("[%d]"), m + 1); LcomD.Description += strDesc; }
				}

				int nLL = 0, nIL = 0, nCF = 0;
				if (m_nLCount > 0 || (bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(L)))) // ST or ST+CS
				{
					for (nc = 0; nc < m_nLCount; nc++) nLL += Set_STLCIncludeCS(arLLoad.GetAt(nc), dLFactor, LoadCaseNo, LcomD, 0);
					if (nLL > 0) { strDesc.Format(_T("+%sL"), GetFactorString(dLFactor)); LcomD.Description += strDesc; }
					if (bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(L)))
					{
						Set_CStageCase(m_aCSLoad.GetAt(nnn), dLFactor, LoadCaseNo, LcomD, TRUE);
						strDesc.Format(_T("+%s(cEL)"), GetFactorString(dLFactor)); LcomD.Description += strDesc;
					}
					for (nc = 0; nc < m_nIlCount; nc++) nIL += Set_STLCIncludeCS(arIlLoad.GetAt(nc), dLFactor, LoadCaseNo, LcomD, 0);
					if (nIL > 0) { strDesc.Format(_T("+%sIL"), GetFactorString(dLFactor)); LcomD.Description += strDesc; }
				}
				bLive = m_nMCount > 0 || (m_nLCount > 0 || (bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(L))));
				if (bLive)
				{
					LcomD_cf = LcomD;
					for (nc = 0; nc < m_nCfCount; nc++) nCF += Set_STLCIncludeCS(arCfLoad.GetAt(nc), dLFactor, LoadCaseNo, LcomD, 0);
					if (nCF > 0) { strDesc.Format(_T("+%sCF"), GetFactorString(dLFactor)); LcomD.Description += strDesc; bCF = TRUE; }
				}
			}

			if (m_nLsCount > 0) // Live Load Surcharge
			{
				T_LCOM_D LcomD_l = LcomD; LoadCaseNo = GetCountLcomCase(LcomD_l);
				int nLS = 0;
				for (nc = 0; nc < m_nLsCount; nc++) nLS += Set_STLCIncludeCS(arLsLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD_l, 0);
				T_LCOM_D LcomD_tmp = LcomD_cf;
				if (bCF)
				{
					LoadCaseNo = GetCountLcomCase(LcomD_tmp);
					for (nc = 0; nc < m_nLsCount; nc++) Set_STLCIncludeCS(arLsLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD_tmp, 0);
				}
				if (nLS > 0)
				{
					strDesc.Format(_T("+%sLS"), GetFactorString(dFactor)); LcomD_l.Description += strDesc;
					if (bCF) LcomD_tmp.Description += strDesc;
					SetCvlJTJD60_15_ETC_M(LcomD_l, LcomD_tmp, bCF, arCrlLoad, arMCrlLoad, nType != 2, arBrkLoad, bLive, arWLoad, FALSE, arWlLoad, TRUE, arSfLoad, TRUE,
						arIpLoad, TRUE, arTLoad, arTpgLoad, TRUE, arFrLoad, TRUE, 0, dF, nType == 2 || nType == 4, nMsgType);
				}
			}
			if (nType != 2 && (m_nCrlCount > 0 || m_nMCrlCount > 0)) // Crowd Load
			{
				T_LCOM_D LcomD_l = LcomD; LoadCaseNo = GetCountLcomCase(LcomD_l);
				int nCRL = 0;
				for (nc = 0; nc < m_nCrlCount; nc++) nCRL += Set_STLCIncludeCS(arCrlLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD_l, 0);
				T_LCOM_D LcomD_tmp = LcomD_cf;
				if (bCF)
				{
					LoadCaseNo = GetCountLcomCase(LcomD_tmp);
					for (nc = 0; nc < m_nCrlCount; nc++) Set_STLCIncludeCS(arCrlLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD_tmp, 0);
				}
				// MNET:XXXX-BJLEE(BBONG)-20060502 
				if (nCRL > 0 || m_nMCrlCount > 0)
				{
					if (nCRL > 0)
					{
						strDesc.Format(_T("+%sCRL"), GetFactorString(dFactor)); LcomD_l.Description += strDesc;
						if (bCF) LcomD_tmp.Description += strDesc;
					}

					int nMCrlCount = (m_nMCrlCount > 0) ? m_nMCrlCount : ((nCRL > 0) ? 1 : 0);
					T_LCOM_D LcomD_tmp2;
					for (int crl = 0; crl < nMCrlCount; ++crl)
					{
						T_LCOM_D LcomD_crl = LcomD_l; LoadCaseNo = GetCountLcomCase(LcomD_crl);
						LcomD_tmp2.Initialize();
						if (m_nMCrlCount > 0)
						{
							Set_MovingCase(arMCrlLoad.GetAt(crl), dFactor, LoadCaseNo, LcomD_crl, TRUE);
							strDesc.Format(_T("+%sMCRL"), GetFactorString(dFactor)); LcomD_crl.Description += strDesc;
							if (m_nMCrlCount > 1) { strDesc.Format(_T("[%d]"), crl + 1); LcomD_crl.Description += strDesc; }

							if (bCF)
							{
								LcomD_tmp2 = LcomD_tmp; LoadCaseNo = GetCountLcomCase(LcomD_tmp2);
								Set_MovingCase(arMCrlLoad.GetAt(crl), dFactor, LoadCaseNo, LcomD_tmp2, TRUE);
								strDesc.Format(_T("+%sMCRL"), GetFactorString(dFactor)); LcomD_tmp2.Description += strDesc;
								if (m_nMCrlCount > 1) { strDesc.Format(_T("[%d]"), crl + 1); LcomD_tmp2.Description += strDesc; }
							}////////////////////////////////
						}
						//if((nCRL>0 || m_nMCrlCount>0) && bCF)
						//{
						SetCvlJTJD60_15_ETC_M(LcomD_crl, LcomD_tmp2, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, bLive, arWLoad, FALSE, arWlLoad, TRUE, arSfLoad, TRUE, arIpLoad, TRUE,
							arTLoad, arTpgLoad, TRUE, arFrLoad, TRUE, 0, dF, nType == 2 || nType == 4, nMsgType);
						//}
					}
				}
			}

			if (m_nBrkCount > 0 && bLive) // Brake Load ÖÆ¶¯Á¦
			{
				T_LCOM_D LcomD_l = LcomD; LoadCaseNo = GetCountLcomCase(LcomD_l);
				double dBrkF = (nType == 0 || nType == 4) ? (bCF ? dFactor * 0.7 : dFactor) : 1.0;
				int nBRK = 0;
				for (nc = 0; nc < m_nBrkCount; nc++) nBRK += Set_STLCIncludeCS(arBrkLoad.GetAt(nc), dBrkF, LoadCaseNo, LcomD_l, 0);
				if (nBRK > 0)
				{
					strDesc.Format(_T("+%sBRK"), GetFactorString(dBrkF)); LcomD_l.Description += strDesc;
					SetCvlJTJD60_15_ETC_M(LcomD_l, LcomD_cf, FALSE, arTmp, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWLoad, FALSE, arWlLoad, TRUE, arSfLoad, FALSE, arIpLoad, FALSE,
						arTLoad, arTpgLoad, TRUE, arFrLoad, FALSE, 0, dF, nType == 2 || nType == 4, nMsgType);
				}
				if (bCF) // MNET:1614, KJH20060809, ¾Õ¿¡¼­ CF°¡ Á¸ÀçÇßÀ¸¸é, CF°¡ Á¦¿ÜµÈ ÇÏÁßÁ¶ÇÕÀ¸·Î ÇÑ¹ø´õ
				{
					LcomD_l = LcomD_cf; LoadCaseNo = GetCountLcomCase(LcomD_l);
					dBrkF = (nType == 0 || nType == 4) ? dFactor : 1.0;
					nBRK = 0;
					for (nc = 0; nc < m_nBrkCount; nc++) nBRK += Set_STLCIncludeCS(arBrkLoad.GetAt(nc), dBrkF, LoadCaseNo, LcomD_l, 0);
					if (nBRK > 0)
					{
						strDesc.Format(_T("+%sBRK"), GetFactorString(dBrkF)); LcomD_l.Description += strDesc;
						SetCvlJTJD60_15_ETC_M(LcomD_l, LcomD_cf, FALSE, arTmp, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWLoad, FALSE, arWlLoad, TRUE, arSfLoad, FALSE, arIpLoad, FALSE,
							arTLoad, arTpgLoad, TRUE, arFrLoad, FALSE, 0, dF, nType == 2 || nType == 4, nMsgType);
					}
				}
			}

			{
				int nWlCount = m_nWlCount * 2; // MNET:1614, KJH20050808
				if (m_nWlCount == 0 && bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(WL))) nWlCount = 1;
				for (nc = 0; nc < nWlCount; nc++)
				{
					T_LCOM_D LcomD_l = LcomD; LoadCaseNo = GetCountLcomCase(LcomD_l);
					if (m_nWlCount > 0)
					{
						double dWF = (nc % 2 == 0) ? dWlFactor : -1.0*dWlFactor;
						if (Set_STLCIncludeCS(arWlLoad.GetAt(nc / 2), dWF, LoadCaseNo, LcomD_l, 0) > 0)
						{
							CString strW = (dWF > 0.0) ? _T("+%sWL") : _T("%sWL");
							strDesc.Format(strW, GetFactorString(dWF)); LcomD_l.Description += strDesc;
							if (m_nWlCount > 1) { strDesc.Format(_T("[%d]"), nc / 2 + 1); LcomD_l.Description += strDesc; }
						}
					}
					if (bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(WL)))
					{
						Set_CStageCase(m_aCSLoad.GetAt(nnn), dWlFactor, LoadCaseNo, LcomD_l, TRUE);
						strDesc.Format(_T("+%s(cEL)"), GetFactorString(dWlFactor)); LcomD_l.Description += strDesc;
					}
					SetCvlJTJD60_15_ETC_M(LcomD_l, LcomD_cf, bCF, arTmp, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWLoad, FALSE, arWlLoad, FALSE, arSfLoad, TRUE, arIpLoad, TRUE,
						arTLoad, arTpgLoad, TRUE, arFrLoad, TRUE, 0, dF, nType == 2 || nType == 4, nMsgType);
				}
			}
			if (!bExsitMV)
			{
				int nWCount = m_nWCount * 2; // MNET:1614, KJH20050808
				if (m_nWCount == 0 && bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(W))) nWCount = 1;
				for (nc = 0; nc < nWCount; nc++)
				{
					T_LCOM_D LcomD_l = LcomD; LoadCaseNo = GetCountLcomCase(LcomD_l);
					if (m_nWCount > 0)
					{
						double dWF = (nc % 2 == 0) ? dWFactor : -1.*dWFactor;
						if (Set_STLCIncludeCS(arWLoad.GetAt(nc / 2), dWF, LoadCaseNo, LcomD_l, 0) > 0)
						{
							CString strW = (dWF > 0.0) ? _T("+%sW") : _T("%sW");
							strDesc.Format(strW, GetFactorString(dWF)); LcomD_l.Description += strDesc;
							if (m_nWCount > 1) { strDesc.Format(_T("[%d]"), nc / 2 + 1); LcomD_l.Description += strDesc; }
						}
					}
					if (bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(W)))
					{
						Set_CStageCase(m_aCSLoad.GetAt(nnn), dWFactor, LoadCaseNo, LcomD_l, TRUE);
						strDesc.Format(_T("+%s(cEL)"), GetFactorString(dWFactor)); LcomD_l.Description += strDesc;
					}
					SetCvlJTJD60_15_ETC_M(LcomD_l, LcomD_cf, bCF, arTmp, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWLoad, FALSE, arWlLoad, FALSE, arSfLoad, TRUE, arIpLoad, TRUE,
						arTLoad, arTpgLoad, TRUE, arFrLoad, TRUE, 0, dF, nType == 2 || nType == 4, nMsgType);
				}
			}
			if (m_nSfCount > 0) // Stream Flow Pressure
			{
				T_LCOM_D LcomD_l = LcomD; LoadCaseNo = GetCountLcomCase(LcomD_l);
				int nSF = 0;
				for (nc = 0; nc < m_nSfCount; nc++) nSF += Set_STLCIncludeCS(arSfLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD_l, 0);
				if (nSF > 0)
				{
					strDesc.Format(_T("+%sSF"), GetFactorString(dFactor)); LcomD_l.Description = LcomD_l.Description + strDesc;
					SetCvlJTJD60_15_ETC_M(LcomD_l, LcomD_cf, bCF, arTmp, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWLoad, FALSE, arWlLoad, FALSE, arSfLoad, FALSE, arIpLoad, FALSE,
						arTLoad, arTpgLoad, TRUE, arFrLoad, TRUE, 0, dF, nType == 2 || nType == 4, nMsgType);
				}
			}

			if (m_nIpCount > 0) // Ice Pressure
			{
				T_LCOM_D LcomD_l = LcomD; LoadCaseNo = GetCountLcomCase(LcomD_l);
				int nIP = 0;
				for (nc = 0; nc < m_nIpCount; nc++) nIP += Set_STLCIncludeCS(arIpLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD_l, 0);
				if (nIP > 0)
				{
					strDesc.Format(_T("+%sIP"), GetFactorString(dFactor)); LcomD_l.Description = LcomD_l.Description + strDesc;
					SetCvlJTJD60_15_ETC_M(LcomD_l, LcomD_cf, bCF, arTmp, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWLoad, FALSE, arWlLoad, FALSE, arSfLoad, FALSE, arIpLoad, FALSE,
						arTLoad, arTpgLoad, TRUE, arFrLoad, TRUE, 0, dF, nType == 2 || nType == 4, nMsgType);
				}
			}

			if ((m_nTCount > 0 || m_nTpgCount > 0) || (bAddCsLoad && (CheckCSEL(D_SGLD_ER1_CVL, _LSX(T)) || CheckCSEL(D_SGLD_ER1_CVL, _LSX(TPG))))) // Temeperature
			{
				T_LCOM_D LcomD_l = LcomD;
				SetCvlJTJD60_Capacity_TPG(LcomD_l, arTLoad, arTpgLoad, arFrLoad, dFactor, dFactor, TRUE, nType == 2 || nType == 4, nMsgType);
			}
		} // next m
	} // next lc

	return TRUE;
}

BOOL CLoadCombCtrl::SetCvlJTGD60_15_Var3_M(const CArray<T_LCOM_D, T_LCOM_D&>& arData, ArUNT& arMLoad, ArINT& arLLoad,
	ArINT& arIlLoad, ArINT& arCrlLoad, ArINT& arMCrlLoad, ArINT& arCfLoad, ArINT& arLsLoad,
	ArINT& arWLoad, ArINT& arWlLoad, ArINT& arSfLoad, ArINT& arIpLoad, ArINT& arTLoad, ArINT& arTpgLoad,
	ArINT& arFrLoad, ArINT& arBrkLoad, int nType, int nMsgType)
{
	// nType == 0,4: °è¼öÁ¶ÇÕ, 1: 1.0(settlement Á¦¿Ü), 2: 1.0(¸ðµÎ), Live Load Á¦¿Ü
	CArray<T_LCOM_D, T_LCOM_D&> aLcomD; aLcomD.Copy(arData);
	int nLcomSize = aLcomD.GetSize();
	int  nnn = 0;
	//BOOL bAddCsLoad = (m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER1_CVL,nnn)) || (m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER2_CVL,nnn)) || (m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER3_CVL,nnn)); // CS or ST+CS
	BOOL bAddCsLoad = m_nConstLoad != 0 && CheckCS_SgldK(D_SGLD_ER1_CVL, nnn); // °üº¬Ê©¹¤ºÉÔØÇÒ´æÔÚÊ©¹¤ºÉÔØ

	double dFactor = (nType == 0 || nType == 4) ? 1.4*0.75 : 1.0;
	double dWFactor = (nType == 0 || nType == 4) ? 1.4*1.0 : 1.0;
	double dLFactor = (nType == 0 || nType == 4) ? 1.4 : 1.0;
	double dF = (nType == 0 || nType == 4) ? 0.75 : 1.0;
	double dMLFactor = (nType == 0 || nType == 4) ? 1.1 : 1.0;
	double dIL = (nType == 12) ? 0. : CalcMovingILFactor();

	double dWlFactor = (nType == 0 || nType == 4) ? 1.1 * 1.0 : 1.0;// ·çºÉÔØ
	BOOL bExsitMV = m_nMCount ? TRUE : FALSE;

	if (nMsgType == 114 && m_bJTG15_Factor)
	{
		dFactor *= m_dJTG15_Factor;
		dWFactor *= m_dJTG15_Factor;
		dWlFactor *= m_dJTG15_Factor;
		dLFactor *= m_dJTG15_Factor;
		dF *= m_dJTG15_Factor;
		dMLFactor *= m_dJTG15_Factor;
	}

	ArINT arTmp;

	// ÓÀ¾Ã×÷ÓÃ + Æû³µºÉÔØ + 3¿É±ä×÷ÓÃ
	CString strDesc = _T("");
	int LoadCaseNo = 0, nc = 0;
	for (int lc = 0; lc < nLcomSize; lc++)
	{
		int nMCount = max(m_nMCount, 1);
		for (int m = 0; m < nMCount; m++)
		{
			T_LCOM_D LcomD = aLcomD.GetAt(lc); LcomD.nActive = 1; // Active or Strength/Stress
			T_LCOM_D LcomD_cf = LcomD; // MNET:1614, KJH20050809, CF°¡ Á¦¿ÜµÈ load combination
			LoadCaseNo = GetCountLcomCase(LcomD);

			double dMlFactor = 1.4;               // Æû³µºÉÔØ×éºÏÏµÊý
			CString strJudgeMILFactorByLoadCase = GetJTGD602015MILoadName(arMLoad, m);
			if (nType == 0 || nType == 1)         // ³ÐÔØÄÜÁ¦×´Ì¬  »ù±¾×éºÏ
			{
				if (strJudgeMILFactorByLoadCase == _T("CH-CL") || strJudgeMILFactorByLoadCase == _T("ÓÃ»§¶¨Òå"))
				{
					dMlFactor = 1.8;                  // ³µÁ¾ºÉÔØ  ÓÃ»§¶¨Òå   È¡1.8
				}
			}
			if (nType == 17)  dMlFactor = 1.0;
			if (nMsgType == 114 && m_bJTG15_Factor)
			{
				dMlFactor *= m_dJTG15_Factor;
			}

			BOOL bLive = FALSE, bCF = FALSE;

			if (nType != 2)
			{
				// moving load
				if (m_nMCount > 0)
				{
					Set_MovingCase(arMLoad.GetAt(m), dMlFactor, LoadCaseNo, LcomD, TRUE);
					strDesc.Format(_T("+%sM"), GetFactorString(dMlFactor)); LcomD.Description += strDesc;
					if (m_nMCount > 1) { strDesc.Format(_T("[%d]"), m + 1); LcomD.Description += strDesc; }
				}

				int nLL = 0, nIL = 0, nCF = 0;
				if (m_nLCount > 0 || (bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(L)))) // ST or ST+CS
				{
					for (nc = 0; nc < m_nLCount;nc++) nLL += Set_STLCIncludeCS(arLLoad.GetAt(nc), dLFactor, LoadCaseNo, LcomD, 0);
					if (nLL > 0) { strDesc.Format(_T("+%sL"), GetFactorString(dLFactor)); LcomD.Description += strDesc; }
					if (bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(L)))
					{
						Set_CStageCase(m_aCSLoad.GetAt(nnn), dLFactor, LoadCaseNo, LcomD, TRUE);
						strDesc.Format(_T("+%s(cEL)"), GetFactorString(dLFactor)); LcomD.Description += strDesc;
					}
					for (nc = 0; nc < m_nIlCount; nc++) nIL += Set_STLCIncludeCS(arIlLoad.GetAt(nc), dLFactor, LoadCaseNo, LcomD, 0);
					if (nIL > 0) { strDesc.Format(_T("+%sIL"), GetFactorString(dLFactor));  LcomD.Description += strDesc; }
				}
				bLive = m_nMCount > 0 || (m_nLCount > 0 || (bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(L))));
				if (bLive)
				{
					LcomD_cf = LcomD;
					for (nc = 0; nc < m_nCfCount; nc++) nCF += Set_STLCIncludeCS(arCfLoad.GetAt(nc), dLFactor, LoadCaseNo, LcomD, 0);
					if (nCF > 0) { strDesc.Format(_T("+%sCF"), GetFactorString(dLFactor));  LcomD.Description += strDesc; bCF = TRUE; }
				}
			}

			if (m_nLsCount > 0) // Live Load Surcharge
			{
				T_LCOM_D LcomD_l = LcomD; LoadCaseNo = GetCountLcomCase(LcomD_l);
				int nLS = 0;
				for (nc = 0; nc < m_nLsCount; nc++) nLS += Set_STLCIncludeCS(arLsLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD_l, 0);
				T_LCOM_D LcomD_tmp = LcomD_cf;
				if (bCF)
				{
					LoadCaseNo = GetCountLcomCase(LcomD_tmp);
					for (nc = 0; nc < m_nLsCount; nc++) Set_STLCIncludeCS(arLsLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD_tmp, 0);
				}
				if (nLS > 0)
				{
					strDesc.Format(_T("+%sLS"), GetFactorString(dFactor)); LcomD_l.Description += strDesc;
					if (bCF) LcomD_tmp.Description += strDesc;
					SetCvlJTJD60_15_ETC_M(LcomD_l, LcomD_tmp, bCF, arCrlLoad, arMCrlLoad, nType != 2, arBrkLoad, bLive, arWLoad, FALSE, arWlLoad, TRUE, arSfLoad, TRUE, arIpLoad, TRUE,
						arTLoad, arTpgLoad, TRUE, arFrLoad, FALSE, 1, dF, nType == 2 || nType == 4, nMsgType);
				}
			}
			// MNET:XXXX-BJLEE(BBONG)-20060503
			if (nType != 2 && (m_nCrlCount > 0 || m_nMCrlCount > 0)) // Crowd Load
			{
				T_LCOM_D LcomD_l = LcomD; LoadCaseNo = GetCountLcomCase(LcomD_l);
				int nCRL = 0;
				for (nc = 0; nc < m_nCrlCount; nc++) nCRL += Set_STLCIncludeCS(arCrlLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD_l, 0);
				T_LCOM_D LcomD_tmp = LcomD_cf;
				if (bCF)
				{
					LoadCaseNo = GetCountLcomCase(LcomD_tmp);
					for (nc = 0; nc < m_nCrlCount; nc++) Set_STLCIncludeCS(arCrlLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD_tmp, 0);
				}
				// MNET:XXXX-BJLEE(BBONG)-20060502 
				if (nCRL > 0 || m_nMCrlCount > 0)
				{
					if (nCRL > 0)
					{
						strDesc.Format(_T("+%sCRL"), GetFactorString(dFactor)); LcomD_l.Description += strDesc;
						if (bCF) LcomD_tmp.Description += strDesc;
					}

					int nMCrlCount = (m_nMCrlCount > 0) ? m_nMCrlCount : ((nCRL > 0) ? 1 : 0);
					T_LCOM_D LcomD_tmp2;
					for (int crl = 0; crl < nMCrlCount; ++crl)
					{
						T_LCOM_D LcomD_crl = LcomD_l; LoadCaseNo = GetCountLcomCase(LcomD_crl);
						LcomD_tmp2.Initialize();
						if (m_nMCrlCount > 0)
						{
							Set_MovingCase(arMCrlLoad.GetAt(crl), dFactor, LoadCaseNo, LcomD_crl, TRUE);
							strDesc.Format(_T("+%sMCRL"), GetFactorString(dFactor)); LcomD_crl.Description += strDesc;
							if (m_nMCrlCount > 1) { strDesc.Format(_T("[%d]"), crl + 1); LcomD_crl.Description += strDesc; }

							if (bCF)
							{
								LcomD_tmp2 = LcomD_tmp; LoadCaseNo = GetCountLcomCase(LcomD_tmp2);
								Set_MovingCase(arMCrlLoad.GetAt(crl), dFactor, LoadCaseNo, LcomD_tmp2, TRUE);
								strDesc.Format(_T("+%sMCRL"), GetFactorString(dFactor)); LcomD_tmp2.Description += strDesc;
								if (m_nMCrlCount > 1) { strDesc.Format(_T("[%d]"), crl + 1); LcomD_tmp2.Description += strDesc; }
							}////////////////////////////////
						}

						//if((nCRL>0 || m_nMCrlCount>0) && bCF)
						//{
						SetCvlJTJD60_15_ETC_M(LcomD_crl, LcomD_tmp2, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, bLive, arWLoad, FALSE, arWlLoad, TRUE, arSfLoad, TRUE, arIpLoad, TRUE,
							arTLoad, arTpgLoad, TRUE, arFrLoad, FALSE, 2, dF, nType == 2 || nType == 4, nMsgType);
						//}
					}
				}
			}

			if (m_nBrkCount > 0 && bLive) // Brake Load
			{
				T_LCOM_D LcomD_l = LcomD; LoadCaseNo = GetCountLcomCase(LcomD_l);
				double dBrkF = (nType == 0 || nType == 4) ? (bCF ? dFactor * 0.7 : dFactor) : 1.0;
				int nBRK = 0;
				for (nc = 0; nc < m_nBrkCount; nc++) nBRK += Set_STLCIncludeCS(arBrkLoad.GetAt(nc), dBrkF, LoadCaseNo, LcomD_l, 0);
				if (nBRK > 0)
				{
					strDesc.Format(_T("+%sBRK"), GetFactorString(dBrkF)); LcomD_l.Description += strDesc;
					SetCvlJTJD60_15_ETC_M(LcomD_l, LcomD_cf, FALSE, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWLoad, FALSE, arWlLoad, TRUE, arSfLoad, FALSE, arIpLoad, FALSE,
						arTLoad, arTpgLoad, FALSE, arFrLoad, FALSE, 3, dF, nType == 2 || nType == 4, nMsgType);
				}
				if (bCF) // MNET:1614, KJH20050809, ¾Õ¿¡¼­ CF°¡ Á¸ÀçÇÏ¸é CF°¡ Á¦¿ÜµÈ ÇÏÁßÁ¶ÇÕ ÇÑ¹ø´õ...
				{
					LcomD_l = LcomD_cf; LoadCaseNo = GetCountLcomCase(LcomD_l);
					dBrkF = (nType == 0 || nType == 4) ? dFactor : 1.0;
					int nBRK = 0;
					for (nc = 0; nc < m_nBrkCount; nc++) nBRK += Set_STLCIncludeCS(arBrkLoad.GetAt(nc), dBrkF, LoadCaseNo, LcomD_l, 0);
					if (nBRK > 0)
					{
						strDesc.Format(_T("+%sBRK"), GetFactorString(dBrkF)); LcomD_l.Description += strDesc;
						SetCvlJTJD60_15_ETC_M(LcomD_l, LcomD_cf, FALSE, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWLoad, FALSE, arWlLoad, TRUE, arSfLoad, FALSE, arIpLoad, FALSE,
							arTLoad, arTpgLoad, FALSE, arFrLoad, FALSE, 3, dF, nType == 2 || nType == 4, nMsgType);
					}
				}
			}

			{
				int nWlCount = m_nWlCount * 2; // MNET:1614, KJH20050808
				if (m_nWlCount == 0 && bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(WL))) nWlCount = 1;
				for (nc = 0; nc < nWlCount; nc++)
				{
					T_LCOM_D LcomD_l = LcomD; LoadCaseNo = GetCountLcomCase(LcomD_l);
					if (m_nWlCount > 0)
					{
						double dWF = (nc % 2 == 0) ? dWlFactor : -1.0*dWlFactor;
						if (Set_STLCIncludeCS(arWlLoad.GetAt(nc / 2), dWF, LoadCaseNo, LcomD_l, 0) > 0)
						{
							CString strW = (dWF > 0.0) ? _T("+%sWL") : _T("%sWL");
							strDesc.Format(strW, GetFactorString(dWF)); LcomD_l.Description += strDesc;
							if (m_nWlCount > 1) { strDesc.Format(_T("[%d]"), nc / 2 + 1); LcomD_l.Description += strDesc; }
						}
					}
					if (bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(WL)))
					{
						Set_CStageCase(m_aCSLoad.GetAt(nnn), dWlFactor, LoadCaseNo, LcomD_l, TRUE);
						strDesc.Format(_T("+%s(cEL)"), GetFactorString(dWlFactor)); LcomD_l.Description += strDesc;
					}
					SetCvlJTJD60_15_ETC_M(LcomD_l, LcomD_cf, bCF, arTmp, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWLoad, FALSE, arWlLoad, FALSE, arSfLoad, TRUE, arIpLoad, TRUE,
						arTLoad, arTpgLoad, TRUE, arFrLoad, FALSE, 4, dF, nType == 2 || nType == 4, nMsgType);
				}
			}
			if (!bExsitMV)
			{
				int nWCount = m_nWCount * 2; // MNET:1614, KJH20050808
				if (m_nWCount == 0 && bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(W))) nWCount = 1;
				for (nc = 0; nc < nWCount; nc++)
				{
					T_LCOM_D LcomD_l = LcomD; LoadCaseNo = GetCountLcomCase(LcomD_l);
					if (m_nWCount > 0)
					{
						double dWF = (nc % 2 == 0) ? dWFactor : -1.*dWFactor;
						if (Set_STLCIncludeCS(arWLoad.GetAt(nc / 2), dWF, LoadCaseNo, LcomD_l, 0) > 0)
						{
							CString strW = (dWF > 0.0) ? _T("+%sW") : _T("%sW");
							strDesc.Format(strW, GetFactorString(dWF)); LcomD_l.Description += strDesc;
							if (m_nWCount > 1) { strDesc.Format(_T("[%d]"), nc / 2 + 1); LcomD_l.Description += strDesc; }
						}
					}
					if (bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(W)))
					{
						Set_CStageCase(m_aCSLoad.GetAt(nnn), dWFactor, LoadCaseNo, LcomD_l, TRUE);
						strDesc.Format(_T("+%s(cEL)"), GetFactorString(dWFactor)); LcomD_l.Description += strDesc;
					}
					SetCvlJTJD60_15_ETC_M(LcomD_l, LcomD_cf, bCF, arTmp, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWLoad, FALSE, arWlLoad, FALSE, arSfLoad, TRUE, arIpLoad, TRUE,
						arTLoad, arTpgLoad, TRUE, arFrLoad, FALSE, 4, dF, nType == 2 || nType == 4, nMsgType);
				}
			}
			if (m_nSfCount > 0) // Stream Flow Pressure
			{
				T_LCOM_D LcomD_l = LcomD; LoadCaseNo = GetCountLcomCase(LcomD_l);
				int nSF = 0;
				for (nc = 0; nc < m_nSfCount; nc++) nSF += Set_STLCIncludeCS(arSfLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD_l, 0);
				if (nSF > 0)
				{
					strDesc.Format(_T("+%sSF"), GetFactorString(dFactor)); LcomD_l.Description = LcomD_l.Description + strDesc;
					SetCvlJTJD60_15_ETC_M(LcomD_l, LcomD_cf, bCF, arTmp, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWLoad, FALSE, arWlLoad, FALSE, arSfLoad, FALSE, arIpLoad, FALSE,
						arTLoad, arTpgLoad, TRUE, arFrLoad, FALSE, 5, dF, nType == 2 || nType == 4, nMsgType);
				}
			}

			if (m_nIpCount > 0) // Ice Pressure
			{
				T_LCOM_D LcomD_l = LcomD; LoadCaseNo = GetCountLcomCase(LcomD_l);
				int nIP = 0;
				for (nc = 0; nc < m_nIpCount; nc++) nIP += Set_STLCIncludeCS(arIpLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD_l, 0);
				if (nIP > 0)
				{
					strDesc.Format(_T("+%sIP"), GetFactorString(dFactor)); LcomD_l.Description = LcomD_l.Description + strDesc;
					SetCvlJTJD60_15_ETC_M(LcomD_l, LcomD_cf, bCF, arTmp, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWLoad, FALSE, arWlLoad, FALSE, arSfLoad, FALSE, arIpLoad, FALSE,
						arTLoad, arTpgLoad, TRUE, arFrLoad, FALSE, 5, dF, nType == 2 || nType == 4, nMsgType);
				}
			}

		} // next m
	} // next lc

	return TRUE;
}

BOOL CLoadCombCtrl::SetCvlJTGD60_15_Var4_M(const CArray<T_LCOM_D, T_LCOM_D&>& arData, ArUNT& arMLoad,
	ArINT& arLLoad, ArINT& arIlLoad, ArINT& arCrlLoad, ArINT& arMCrlLoad,
	ArINT& arCfLoad, ArINT& arLsLoad, ArINT& arWLoad, ArINT& arWlLoad, ArINT& arSfLoad, ArINT& arIpLoad,
	ArINT& arTLoad, ArINT& arTpgLoad, ArINT& arFrLoad, ArINT& arBrkLoad, int nType, int nMsgType)
{
	// nType == 0,4: °è¼öÁ¶ÇÕ, 1: 1.0(settlement Á¦¿Ü), 2: 1.0(¸ðµÎ), Live Load Á¦¿Ü
	CArray<T_LCOM_D, T_LCOM_D&> aLcomD; aLcomD.Copy(arData);
	int  nLcomSize = aLcomD.GetSize();
	int  nnn = 0;
	//BOOL bAddCsLoad = (m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER1_CVL,nnn)) || (m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER2_CVL,nnn)) || (m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER3_CVL,nnn)); // CS or ST+CS
	BOOL bAddCsLoad = m_nConstLoad != 0 && CheckCS_SgldK(D_SGLD_ER1_CVL, nnn); // °üº¬Ê©¹¤ºÉÔØÇÒ´æÔÚÊ©¹¤ºÉÔØ

	double dFactor = (nType == 0 || nType == 4) ? 1.4*0.75 : 1.0;
	double dWFactor = (nType == 0 || nType == 4) ? 1.4*1.0 : 1.0;
	double dLFactor = (nType == 0 || nType == 4) ? 1.4 : 1.0;
	double dF = (nType == 0 || nType == 4) ? 0.75 : 1.0;
	double dMLFactor = (nType == 0 || nType == 4) ? 1.1 : 1.0;
	double dIL = (nType == 12) ? 0. : CalcMovingILFactor();

	double dWlFactor = (nType == 0 || nType == 4) ? 1.1 * 1.0 : 1.0;// ·çºÉÔØ
	BOOL bExsitMV = m_nMCount ? TRUE : FALSE;

	if (nMsgType == 114 && m_bJTG15_Factor)
	{
		dFactor *= m_dJTG15_Factor;
		dWFactor *= m_dJTG15_Factor;
		dWlFactor *= m_dJTG15_Factor;
		dLFactor *= m_dJTG15_Factor;
		dF *= m_dJTG15_Factor;
		dMLFactor *= m_dJTG15_Factor;
	}

	ArINT arTmp;

	// ÓÀ¾Ã×÷ÓÃ + Æû³µºÉÔØ + 4¿É±ä×÷ÓÃ
	CString strDesc = _T("");
	int LoadCaseNo = 0, nc = 0;
	for (int lc = 0; lc < nLcomSize; lc++)
	{
		int nMCount = max(m_nMCount, 1);
		for (int m = 0; m < nMCount; m++)
		{
			T_LCOM_D LcomD = aLcomD.GetAt(lc); LcomD.nActive = 1; // Active or Strength/Stress
			T_LCOM_D LcomD_cf = LcomD; // MNET:1614, KJH20050809, CF°¡ Á¦¿ÜµÈ ÇÏÁßÁ¶ÇÕ
			LoadCaseNo = GetCountLcomCase(LcomD);

			double dMlFactor = 1.4;       // Æû³µºÉÔØ×éºÏÏµÊý
			CString strJudgeMILFactorByLoadCase = GetJTGD602015MILoadName(arMLoad, m);
			if (nType == 0 || nType == 1)         // ³ÐÔØÄÜÁ¦×´Ì¬  »ù±¾×éºÏ
			{
				if (strJudgeMILFactorByLoadCase == _T("CH-CL") || strJudgeMILFactorByLoadCase == _T("ÓÃ»§¶¨Òå"))
				{
					dMlFactor = 1.8;                  // ³µÁ¾ºÉÔØ  ÓÃ»§¶¨Òå   È¡1.8
				}
			}
			if (nType == 17)  dMlFactor = 1.0;
			if (nMsgType == 114 && m_bJTG15_Factor)
			{
				dMlFactor *= m_dJTG15_Factor;
			}

			BOOL bLive = FALSE, bCF = FALSE;

			if (nType != 2)
			{
				// moving load
				if (m_nMCount > 0)
				{
					Set_MovingCase(arMLoad.GetAt(m), dMlFactor, LoadCaseNo, LcomD, TRUE);
					strDesc.Format(_T("+%sM"), GetFactorString(dMlFactor)); LcomD.Description += strDesc;
					if (m_nMCount > 1) { strDesc.Format(_T("[%d]"), m + 1); LcomD.Description += strDesc; }
				}

				int nLL = 0, nIL = 0, nCF = 0;
				if (m_nLCount > 0 || (bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(L)))) // ST or ST+CS
				{
					for (nc = 0; nc < m_nLCount; nc++) nLL += Set_STLCIncludeCS(arLLoad.GetAt(nc), dLFactor, LoadCaseNo, LcomD, 0);
					if (nLL > 0) { strDesc.Format(_T("+%sL"), GetFactorString(dLFactor)); LcomD.Description += strDesc; }
					if (bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(L)))
					{
						Set_CStageCase(m_aCSLoad.GetAt(nnn), dLFactor, LoadCaseNo, LcomD, TRUE);
						strDesc.Format(_T("+%s(cEL)"), GetFactorString(dLFactor));  LcomD.Description += strDesc;
					}
					for (nc = 0; nc < m_nIlCount; nc++) nIL += Set_STLCIncludeCS(arIlLoad.GetAt(nc), dLFactor, LoadCaseNo, LcomD, 0);
					if (nIL > 0) { strDesc.Format(_T("+%sIL"), GetFactorString(dLFactor));  LcomD.Description += strDesc; }
				}
				bLive = m_nMCount > 0 || (m_nLCount > 0 || (bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(L))));
				if (bLive)
				{
					LcomD_cf = LcomD;
					for (nc = 0; nc < m_nCfCount; nc++) nCF += Set_STLCIncludeCS(arCfLoad.GetAt(nc), dLFactor, LoadCaseNo, LcomD, 0);
					if (nCF > 0) { strDesc.Format(_T("+%sCF"), GetFactorString(dLFactor));  LcomD.Description += strDesc; bCF = TRUE; }
				}
			}

			if (m_nLsCount > 0) // Live Load Surcharge
			{
				T_LCOM_D LcomD_l = LcomD; LoadCaseNo = GetCountLcomCase(LcomD_l);
				int nLS = 0;
				for (nc = 0; nc < m_nLsCount; nc++) nLS += Set_STLCIncludeCS(arLsLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD_l, 0);
				T_LCOM_D LcomD_tmp = LcomD_cf;
				if (bCF)
				{
					LoadCaseNo = GetCountLcomCase(LcomD_tmp);
					for (nc = 0; nc < m_nLsCount; nc++) Set_STLCIncludeCS(arLsLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD_tmp, 0);
				}
				if (nLS > 0)
				{
					strDesc.Format(_T("+%sLS"), GetFactorString(dFactor)); LcomD_l.Description += strDesc;
					if (bCF) LcomD_tmp.Description += strDesc;
					SetCvlJTJD60_15_ETC_M(LcomD_l, LcomD_tmp, bCF, arCrlLoad, arMCrlLoad, nType != 2, arBrkLoad, bLive, arWLoad, FALSE, arWlLoad, TRUE, arSfLoad, TRUE, arIpLoad, TRUE,
						arTLoad, arTpgLoad, FALSE, arFrLoad, FALSE, 6, dF, nType == 2 || nType == 4, nMsgType);
				}
			}

			if (nType != 2 && (m_nCrlCount > 0 || m_nMCrlCount > 0)) // Crowd Load
			{
				T_LCOM_D LcomD_l = LcomD; LoadCaseNo = GetCountLcomCase(LcomD_l);
				int nCRL = 0;
				for (nc = 0; nc < m_nCrlCount; nc++) nCRL += Set_STLCIncludeCS(arCrlLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD_l, 0);
				T_LCOM_D LcomD_tmp = LcomD_cf;
				if (bCF)
				{
					LoadCaseNo = GetCountLcomCase(LcomD_tmp);
					for (nc = 0; nc < m_nCrlCount; nc++) Set_STLCIncludeCS(arCrlLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD_tmp, 0);
				}
				if (nCRL > 0 || m_nMCrlCount > 0)
				{
					if (nCRL > 0)
					{
						strDesc.Format(_T("+%sCRL"), GetFactorString(dFactor)); LcomD_l.Description += strDesc;
						if (bCF) LcomD_tmp.Description += strDesc;
					}

					int nMCrlCount = (m_nMCrlCount > 0) ? m_nMCrlCount : ((nCRL > 0) ? 1 : 0);
					T_LCOM_D LcomD_tmp2;
					for (int crl = 0; crl < nMCrlCount; ++crl)
					{
						T_LCOM_D LcomD_crl = LcomD_l; LoadCaseNo = GetCountLcomCase(LcomD_crl);
						if (m_nMCrlCount > 0)
						{
							Set_MovingCase(arMCrlLoad.GetAt(crl), dFactor, LoadCaseNo, LcomD_crl, TRUE);
							strDesc.Format(_T("+%sMCRL"), GetFactorString(dFactor)); LcomD_crl.Description += strDesc;
							if (m_nMCrlCount > 1) { strDesc.Format(_T("[%d]"), crl + 1); LcomD_crl.Description += strDesc; }

							if (bCF)
							{
								LcomD_tmp2 = LcomD_tmp; LoadCaseNo = GetCountLcomCase(LcomD_tmp2);
								Set_MovingCase(arMCrlLoad.GetAt(crl), dFactor, LoadCaseNo, LcomD_tmp2, TRUE);
								strDesc.Format(_T("+%sMCRL"), GetFactorString(dFactor)); LcomD_tmp2.Description += strDesc;
								if (m_nMCrlCount > 1) { strDesc.Format(_T("[%d]"), crl + 1); LcomD_tmp2.Description += strDesc; }
							}
						}

						SetCvlJTJD60_15_ETC_M(LcomD_crl, LcomD_tmp2, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, bLive, arWLoad, FALSE, arWlLoad, TRUE, arSfLoad, TRUE, arIpLoad, TRUE,
							arTLoad, arTpgLoad, FALSE, arFrLoad, FALSE, 7, dF, nType == 2 || nType == 4, nMsgType);

					}
				}
			}

			{
				int nWlCount = m_nWlCount * 2; // wind
				if (m_nWlCount == 0 && bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(WL))) nWlCount = 1;
				for (nc = 0; nc < nWlCount; nc++)
				{
					T_LCOM_D LcomD_l = LcomD; LoadCaseNo = GetCountLcomCase(LcomD_l);
					if (m_nWCount > 0)
					{
						double dWF = (nc % 2 == 0) ? dWlFactor : -1.0*dWlFactor;
						if (Set_STLCIncludeCS(arWlLoad.GetAt(nc / 2), dWF, LoadCaseNo, LcomD_l, 0) > 0)
						{
							CString strW = (dWF > 0.0) ? _T("+%sWL") : _T("%sWL");
							strDesc.Format(strW, GetFactorString(dWF)); LcomD_l.Description += strDesc;
							if (m_nWlCount > 1) { strDesc.Format(_T("[%d]"), nc / 2 + 1); LcomD_l.Description += strDesc; }
						}
					}
					if (bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(WL)))
					{
						Set_CStageCase(m_aCSLoad.GetAt(nnn), dWlFactor, LoadCaseNo, LcomD_l, TRUE);
						strDesc.Format(_T("+%s(cEL)"), GetFactorString(dWlFactor)); LcomD_l.Description += strDesc;
					}
					SetCvlJTJD60_15_ETC_M(LcomD_l, LcomD_cf, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWLoad, FALSE, arWlLoad, FALSE, arSfLoad, TRUE, arIpLoad, TRUE,
						arTLoad, arTpgLoad, FALSE, arFrLoad, FALSE, 7, dF, nType == 2 || nType == 4, nMsgType);
				}
			}
			if (!bExsitMV)
			{
				int nWCount = m_nWCount * 2; // wind
				if (m_nWCount == 0 && bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(W))) nWCount = 1;
				for (nc = 0; nc < nWCount; nc++)
				{
					T_LCOM_D LcomD_l = LcomD; LoadCaseNo = GetCountLcomCase(LcomD_l);
					if (m_nWCount > 0)
					{
						double dWF = (nc % 2 == 0) ? dWFactor : -1.*dWFactor;
						if (Set_STLCIncludeCS(arWLoad.GetAt(nc / 2), dWF, LoadCaseNo, LcomD_l, 0) > 0)
						{
							CString strW = (dWF > 0.0) ? _T("+%sW") : _T("%sW");
							strDesc.Format(strW, GetFactorString(dWF)); LcomD_l.Description += strDesc;
							if (m_nWCount > 1) { strDesc.Format(_T("[%d]"), nc / 2 + 1); LcomD_l.Description += strDesc; }
						}
					}
					if (bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(W)))
					{
						Set_CStageCase(m_aCSLoad.GetAt(nnn), dWFactor, LoadCaseNo, LcomD_l, TRUE);
						strDesc.Format(_T("+%s(cEL)"), GetFactorString(dWFactor)); LcomD_l.Description += strDesc;
					}
					SetCvlJTJD60_15_ETC_M(LcomD_l, LcomD_cf, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWLoad, FALSE, arWlLoad, FALSE, arSfLoad, TRUE, arIpLoad, TRUE,
						arTLoad, arTpgLoad, FALSE, arFrLoad, FALSE, 7, dF, nType == 2 || nType == 4, nMsgType);
				}
			}
		} // next m
	} // next lc

	return TRUE;
}

BOOL CLoadCombCtrl::SetCvlJTGD60_15_Var56_M(const CArray<T_LCOM_D, T_LCOM_D&>& arData, ArUNT& arMLoad, ArINT& arLLoad,
	ArINT& arIlLoad, ArINT& arCrlLoad, ArINT& arMCrlLoad, ArINT& arCfLoad, ArINT& arLsLoad,
	ArINT& arWLoad, ArINT& arWlLoad, ArINT& arSfLoad, ArINT& arIpLoad, ArINT& arTLoad, ArINT& arTpgLoad,
	ArINT& arFrLoad, ArINT& arBrkLoad, int nType, int nMsgType)
{
	// nType == 0,4: °è¼öÁ¶ÇÕ, 1: 1.0(settlement Á¦¿Ü), 2: 1.0(¸ðµÎ), Live Load Á¦¿Ü
	CArray<T_LCOM_D, T_LCOM_D&> aLcomD; aLcomD.Copy(arData);
	int  nLcomSize = aLcomD.GetSize();
	int  nnn = 0;
	//BOOL bAddCsLoad = (m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER1_CVL,nnn)) || (m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER2_CVL,nnn)) || (m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER3_CVL,nnn)); // CS or ST+CS
	BOOL bAddCsLoad = m_nConstLoad != 0 && CheckCS_SgldK(D_SGLD_ER1_CVL, nnn); // °üº¬Ê©¹¤ºÉÔØÇÒ´æÔÚÊ©¹¤ºÉÔØ

	double dFactor = (nType == 0 || nType == 4) ? 1.4*0.75 : 1.0;
	double dWFactor = (nType == 0 || nType == 4) ? 1.4*1.0 : 1.0;
	double dLFactor = (nType == 0 || nType == 4) ? 1.4 : 1.0;
	double dF = (nType == 0 || nType == 4) ? 0.75 : 1.0;
	double dMLFactor = (nType == 0 || nType == 4) ? 1.1 : 1.0;
	double dIL = (nType == 12) ? 0. : CalcMovingILFactor();

	double dWlFactor = (nType == 0 || nType == 4) ? 1.1 * 1.0 : 1.0;// ·çºÉÔØ
	BOOL bUseWlLoad = m_nMCount ? TRUE : FALSE;

	if (nMsgType == 114 && m_bJTG15_Factor)
	{
		dFactor *= m_dJTG15_Factor;
		dWFactor *= m_dJTG15_Factor;
		dWlFactor *= m_dJTG15_Factor;
		dLFactor *= m_dJTG15_Factor;
		dF *= m_dJTG15_Factor;
		dMLFactor *= m_dJTG15_Factor;
	}

	ArINT arTmp;
	// ¿µ±¸ÇÏÁß + Â÷·®ÇÏÁß + 5°³ÀÇ °¡º¯ÇÏÁßÀÎ °æ¿ì
	CString strDesc = _T("");
	int LoadCaseNo = 0, nc = 0;
	for (int lc = 0; lc < nLcomSize; lc++)
	{
		int nMCount = max(m_nMCount, 1);
		for (int m = 0; m < nMCount; m++)
		{
			T_LCOM_D LcomD = aLcomD.GetAt(lc); LcomD.nActive = 1; // Active or Strength/Stress
			T_LCOM_D LcomD_cf = LcomD; // MNET:1614, KJH20050809, CFÁ¦¿ÜÇÑ ÇÏÁßÁ¶ÇÕ
			LoadCaseNo = GetCountLcomCase(LcomD);

			double dMlFactor = 1.4;               // Æû³µºÉÔØ×éºÏÏµÊý
			CString strJudgeMILFactorByLoadCase = GetJTGD602015MILoadName(arMLoad, m);
			if (nType == 0 || nType == 1)         // ³ÐÔØÄÜÁ¦×´Ì¬  »ù±¾×éºÏ
			{
				if (strJudgeMILFactorByLoadCase == _T("CH-CL") || strJudgeMILFactorByLoadCase == _T("ÓÃ»§¶¨Òå"))
				{
					dMlFactor = 1.8;                  // ³µÁ¾ºÉÔØ  ÓÃ»§¶¨Òå   È¡1.8
				}
			}
			if (nType == 17)  dMlFactor = 1.0;
			if (nMsgType == 114 && m_bJTG15_Factor)
			{
				dMlFactor *= m_dJTG15_Factor;
			}

			BOOL bLive = FALSE, bCF = FALSE;
			//BOOL bFindCJJLeadingMLoad = FALSE;
			if (nType != 2)
			{
				// moving load
				if (m_nMCount > 0)
				{
					Set_MovingCase(arMLoad.GetAt(m), dMlFactor, LoadCaseNo, LcomD, TRUE);
					strDesc.Format(_T("+%sM"), GetFactorString(dMlFactor)); LcomD.Description += strDesc;
					if (m_nMCount > 1) { strDesc.Format(_T("[%d]"), m + 1); LcomD.Description += strDesc; }
				}

				int nLL = 0, nIL = 0, nCF = 0;
				if (m_nLCount > 0 || (bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(L)))) // ST or ST+CS
				{
					for (nc = 0; nc < m_nLCount; nc++) nLL += Set_STLCIncludeCS(arLLoad.GetAt(nc), dLFactor, LoadCaseNo, LcomD, 0);
					if (nLL > 0) { strDesc.Format(_T("+%sL"), GetFactorString(dLFactor)); LcomD.Description += strDesc; }
					if (bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(L)))
					{
						Set_CStageCase(m_aCSLoad.GetAt(nnn), dLFactor, LoadCaseNo, LcomD, TRUE);
						strDesc.Format(_T("+%s(cEL)"), GetFactorString(dLFactor)); LcomD.Description += strDesc;
					}
					for (nc = 0; nc < m_nIlCount; nc++) nIL += Set_STLCIncludeCS(arIlLoad.GetAt(nc), dLFactor, LoadCaseNo, LcomD, 0);
					if (nIL > 0) { strDesc.Format(_T("+%sIL"), GetFactorString(dLFactor)); LcomD.Description += strDesc; }
				}
				bLive = m_nMCount > 0 || (m_nLCount > 0 || (bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(L))));
				if (bLive)
				{
					LcomD_cf = LcomD;
					for (nc = 0; nc < m_nCfCount; nc++) nCF += Set_STLCIncludeCS(arCfLoad.GetAt(nc), dLFactor, LoadCaseNo, LcomD, 0);
					if (nCF > 0) { strDesc.Format(_T("+%sCF"), GetFactorString(dLFactor)); LcomD.Description += strDesc; bCF = TRUE; }
				}
			}

			if (m_nLsCount > 0) // Live Load Surcharge
			{
				T_LCOM_D LcomD_l = LcomD; LoadCaseNo = GetCountLcomCase(LcomD_l);
				int nLS = 0;
				for (nc = 0; nc < m_nLsCount; nc++) nLS += Set_STLCIncludeCS(arLsLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD_l, 0);
				T_LCOM_D LcomD_tmp = LcomD_cf;
				if (bCF)
				{
					LoadCaseNo = GetCountLcomCase(LcomD_tmp);
					for (nc = 0; nc < m_nLsCount; nc++) Set_STLCIncludeCS(arLsLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD_tmp, 0);
				}
				if (nLS > 0)
				{
					strDesc.Format(_T("+%sLS"), GetFactorString(dFactor)); LcomD_l.Description += strDesc;
					if (bCF) LcomD_tmp.Description += strDesc;
					SetCvlJTJD60_15_ETC_M(LcomD_l, LcomD_tmp, bCF, arCrlLoad, arMCrlLoad, nType != 2, arBrkLoad, bLive, arWLoad, FALSE, arWlLoad, TRUE, arSfLoad, FALSE, arIpLoad, FALSE,
						arTLoad, arTpgLoad, FALSE, arFrLoad, FALSE, 8, dF, nType == 2 || nType == 4, nMsgType);
				}
			}
			// MNET:XXXX-BJLEE(BBONG)-20060503
			if (nType != 2 && (m_nCrlCount > 0 || m_nMCrlCount > 0)) // Crowd Load
			{
				T_LCOM_D LcomD_l = LcomD; LoadCaseNo = GetCountLcomCase(LcomD_l);
				int nCRL = 0;
				for (nc = 0; nc < m_nCrlCount; nc++) nCRL += Set_STLCIncludeCS(arCrlLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD_l, 0);
				T_LCOM_D LcomD_tmp = LcomD_cf;
				// MNET:XXXX-BJLEE(BBONG)-20060502 
				if (nCRL > 0 || m_nMCrlCount > 0)
				{
					if (nCRL > 0)
					{
						strDesc.Format(_T("+%sCRL"), GetFactorString(dFactor)); LcomD_l.Description += strDesc;
						if (bCF) LcomD_tmp.Description += strDesc;
					}

					int nMCrlCount = (m_nMCrlCount > 0) ? m_nMCrlCount : ((nCRL > 0) ? 1 : 0);
					T_LCOM_D LcomD_tmp2;
					for (int crl = 0; crl < nMCrlCount; ++crl)
					{
						T_LCOM_D LcomD_crl = LcomD_l; LoadCaseNo = GetCountLcomCase(LcomD_crl);
						if (m_nMCrlCount > 0)
						{
							Set_MovingCase(arMCrlLoad.GetAt(crl), dFactor, LoadCaseNo, LcomD_crl, TRUE);
							strDesc.Format(_T("+%sMCRL"), GetFactorString(dFactor)); LcomD_crl.Description += strDesc;
							if (m_nMCrlCount > 1) { strDesc.Format(_T("[%d]"), crl + 1); LcomD_crl.Description += strDesc; }

							if (bCF)
							{
								LcomD_tmp2 = LcomD_tmp; LoadCaseNo = GetCountLcomCase(LcomD_tmp2);
								Set_MovingCase(arMCrlLoad.GetAt(crl), dFactor, LoadCaseNo, LcomD_tmp2, TRUE);
								strDesc.Format(_T("+%sMCRL"), GetFactorString(dFactor)); LcomD_tmp2.Description += strDesc;
								if (m_nMCrlCount > 1) { strDesc.Format(_T("[%d]"), crl + 1); LcomD_tmp2.Description += strDesc; }
							}
						}
						//if((nCRL>0 || m_nMCrlCount>0))
						//{
						SetCvlJTJD60_15_ETC_M(LcomD_l, LcomD_cf, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWLoad, FALSE, arWlLoad, TRUE, arSfLoad, FALSE, arIpLoad, FALSE,
							arTLoad, arTpgLoad, FALSE, arFrLoad, FALSE, 9, dF, nType == 2 || nType == 4, nMsgType);
						//}
					}
				}
			}
		} // next m
	} // next lc

	// ¿µ±¸ÇÏÁß + Â÷·®ÇÏÁß + 6°³ÀÇ °¡º¯ÇÏÁßÀÎ °æ¿ì
	for (int lc = 0; lc < nLcomSize; lc++)
	{
		int nMCount = max(m_nMCount, 1);
		for (int m = 0; m < nMCount; m++)
		{
			T_LCOM_D LcomD = aLcomD.GetAt(lc); LcomD.nActive = 1; // Active or Strength/Stress
			LoadCaseNo = GetCountLcomCase(LcomD);

			if (nType != 2)
			{
				// moving load
				if (m_nMCount > 0)
				{
					Set_MovingCase(arMLoad.GetAt(m), dLFactor, LoadCaseNo, LcomD, TRUE);
					strDesc.Format(_T("+%sM"), GetFactorString(dLFactor)); LcomD.Description += strDesc;
					if (m_nMCount > 1) { strDesc.Format(_T("[%d]"), m + 1); LcomD.Description += strDesc; }
				}
				if (m_nLCount > 0 || (bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(L)))) // ST or ST+CS
				{
					int nLL = 0, nIL = 0, nCF = 0;
					for (nc = 0; nc < m_nLCount; nc++) nLL += Set_STLCIncludeCS(arLLoad.GetAt(nc), dLFactor, LoadCaseNo, LcomD, 0);
					if (nLL > 0) { strDesc.Format(_T("+%sL"), GetFactorString(dLFactor)); LcomD.Description += strDesc; }
					if (bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(L)))
					{
						Set_CStageCase(m_aCSLoad.GetAt(nnn), dLFactor, LoadCaseNo, LcomD, TRUE);
						strDesc.Format(_T("+%s(cEL)"), GetFactorString(dLFactor)); LcomD.Description += strDesc;
					}
					for (nc = 0; nc < m_nIlCount; nc++) nIL += Set_STLCIncludeCS(arIlLoad.GetAt(nc), dLFactor, LoadCaseNo, LcomD, 0);
					if (nIL > 0) { strDesc.Format(_T("+%sIL"), GetFactorString(dLFactor)); LcomD.Description += strDesc; }
					for (nc = 0; nc < m_nCfCount; nc++) nCF += Set_STLCIncludeCS(arCfLoad.GetAt(nc), dLFactor, LoadCaseNo, LcomD, 0);
					if (nCF > 0) { strDesc.Format(_T("+%sCF"), GetFactorString(dLFactor)); LcomD.Description += strDesc; }
				}
			}

			if (m_nLsCount > 0) // Live Load Surcharge
			{
				T_LCOM_D LcomD_l = LcomD; LoadCaseNo = GetCountLcomCase(LcomD_l);
				int nLS = 0;
				for (nc = 0; nc < m_nLsCount; nc++) nLS += Set_STLCIncludeCS(arLsLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD_l, 0);
				if (nLS > 0)
				{
					strDesc.Format(_T("+%sLS"), GetFactorString(dFactor)); LcomD_l.Description += strDesc;
					SetCvlJTJD60_15_ETC_M(LcomD_l, LcomD_l, FALSE, arCrlLoad, arMCrlLoad, nType != 2, arBrkLoad, FALSE, arWLoad, FALSE, arWlLoad, FALSE, arSfLoad, FALSE, arIpLoad, FALSE,
						arTLoad, arTpgLoad, FALSE, arFrLoad, FALSE, 10, dF, nType == 2 || nType == 4, nMsgType);
				}
			}
		}
	}

	return TRUE;
}

BOOL CLoadCombCtrl::SetCvlJTJD60_15_ETC_M(T_LCOM_D rData, T_LCOM_D Lcom_cf, BOOL bCF, ArINT& arCrlLoad, ArINT& arMCrlLoad, BOOL bCrl, ArINT& arBrkLoad, BOOL bBrk,
	ArINT& arWLoad, BOOL bW, ArINT& arWlLoad, BOOL bWl, ArINT& arSfLoad, BOOL bSf, ArINT& arIpLoad, BOOL bIp, ArINT& arTLoad, ArINT& arTpgLoad,
	BOOL bT, ArINT& arFrLoad, BOOL bFr, int nType, double dFactor, BOOL bAcdn, int nMsgType)
{
	int LoadCaseNo = 0, nc = 0, nnn = 0;
	CString strDesc = _T("");
	bool bChange = m_bJTG15_Factor ? (dFactor / m_dJTG15_Factor < 1.0) : (dFactor < 1.0);
	double dF = bChange ? 1.4*dFactor : 1.0;
	double dTepFcator = nMsgType == 115 ? 0.8 : dF;//nMsgType == 115 ´ú±íÅ¼È»×éºÏ
	double dTFactor = nMsgType == 115 ? 1.0 : dTepFcator;
	//double dTepF = nType == 0 ? 1.4*0.75 : 1.4;
	double dWF = bChange ? 1.4*1.0 : 1.0;
	double dWlF = bChange ? 1.1*1.0 : 1.0;

	BOOL bAddCsLoad = m_nConstLoad != 0 && CheckCS_SgldK(D_SGLD_ER1_CVL, nnn); // CS or ST+CS

	if ((m_nMCrlCount || m_nCrlCount > 0) && bCrl)
	{
		T_LCOM_D LcomD = rData;
		LoadCaseNo = GetCountLcomCase(LcomD);
		int nCRL = 0;
		for (nc = 0; nc < m_nCrlCount; nc++) nCRL += Set_STLCIncludeCS(arCrlLoad.GetAt(nc), dF, LoadCaseNo, LcomD, 0);
		T_LCOM_D LcomD_tmp = Lcom_cf;
		if (bCF && bBrk)
		{
			LoadCaseNo = GetCountLcomCase(LcomD_tmp);
			for (nc = 0; nc < m_nCrlCount; nc++) Set_STLCIncludeCS(arCrlLoad.GetAt(nc), dF, LoadCaseNo, LcomD_tmp, 0);
		}
		if (nCRL > 0)
		{
			strDesc.Format(_T("+%sCRL"), GetFactorString(dF)); LcomD.Description = LcomD.Description + strDesc;
			if (bCF && bBrk) LcomD_tmp.Description = LcomD_tmp.Description + strDesc;
		}
		int nMCrlCount = (m_nMCrlCount > 0) ? m_nMCrlCount : ((nCRL > 0) ? 1 : 0);
		for (int crl = 0; crl < nMCrlCount; ++crl)
		{
			T_LCOM_D LcomD_crl = LcomD;
			T_LCOM_D LcomD_crltmp = LcomD_tmp;
			if (m_nMCrlCount > 0)
			{
				Set_MovingCase(arMCrlLoad.GetAt(crl), dF, LoadCaseNo, LcomD_crl, TRUE);
				strDesc.Format(_T("+%sMCRL"), GetFactorString(dF)); LcomD_crl.Description = LcomD_crl.Description + strDesc; LcomD_crltmp.Description = LcomD_crltmp.Description + strDesc;
				if (m_nMCrlCount > 1) { strDesc.Format(_T("[%d]"), crl + 1); LcomD_crl.Description = LcomD_crl.Description + strDesc; LcomD_crltmp.Description = LcomD_crltmp.Description + strDesc; }
			}
			if (nCRL > 0 || m_nMCrlCount > 0)
			{
				if (nType == 1) SetCvlJTJD60_15_ETC_M(LcomD_crl, LcomD_crltmp, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, bBrk, arWLoad, FALSE, arWlLoad, TRUE, arSfLoad, TRUE, arIpLoad, TRUE,
					arTLoad, arTpgLoad, TRUE, arFrLoad, TRUE, 0, dFactor, bAcdn, nMsgType);
				if (nType == 6) SetCvlJTJD60_15_ETC_M(LcomD_crl, LcomD_crltmp, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, bBrk, arWLoad, FALSE, arWlLoad, TRUE, arSfLoad, TRUE, arIpLoad, TRUE,
					arTLoad, arTpgLoad, TRUE, arFrLoad, FALSE, 2, dFactor, bAcdn, nMsgType);
				if (nType == 8) SetCvlJTJD60_15_ETC_M(LcomD_crl, LcomD_crltmp, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, bBrk, arWLoad, FALSE, arWlLoad, TRUE, arSfLoad, TRUE, arIpLoad, TRUE,
					arTLoad, arTpgLoad, FALSE, arFrLoad, FALSE, 6, dFactor, bAcdn, nMsgType);
				if (nType == 10) SetCvlJTJD60_15_ETC_M(LcomD_crl, LcomD_crltmp, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWLoad, FALSE, arWlLoad, TRUE, arSfLoad, FALSE, arIpLoad, FALSE,
					arTLoad, arTpgLoad, FALSE, arFrLoad, FALSE, 8, dFactor, bAcdn, nMsgType);
				if (nType == 101) SetCvlJTJD60_15_ETC_M(LcomD_crl, LcomD_crltmp, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, bBrk, arWLoad, FALSE, arWlLoad, FALSE, arSfLoad, FALSE, arIpLoad, FALSE,
					arTLoad, arTpgLoad, FALSE, arFrLoad, FALSE, 102, dFactor, bAcdn, nMsgType);
				if (nType == 103) SetCvlJTJD60_15_ETC_M(LcomD_crl, LcomD_crltmp, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, bBrk, arWLoad, FALSE, arWlLoad, TRUE, arSfLoad, FALSE, arIpLoad, FALSE,
					arTLoad, arTpgLoad, FALSE, arFrLoad, FALSE, 104, dFactor, bAcdn, nMsgType);
				if (nType == 0)
				{
					if (bAcdn) m_aAcdnLcomD.Add(LcomD_crl);
					else      Set_LoadComb4Civil(LcomD_crl, D_STR, 0, nMsgType);
				}
			}
		}
	}

	if (nType == 8) bBrk = FALSE;
	if (m_nBrkCount > 0 && bBrk)
	{
		for (int i = 0; i < 2; ++i)
		{
			if (i == 1 && !bCF) break;
			T_LCOM_D LcomD = (i == 0) ? rData : Lcom_cf;
			LoadCaseNo = GetCountLcomCase(LcomD);
			double dBrkF = (i == 0) ? (bCF ? dF * 0.7 : dF) : dF;
			int nBRK = 0;
			for (nc = 0; nc < m_nBrkCount; nc++) nBRK += Set_STLCIncludeCS(arBrkLoad.GetAt(nc), dBrkF, LoadCaseNo, LcomD, 0);
			if (nBRK > 0) { strDesc.Format(_T("+%sBRK"), GetFactorString(dBrkF)); LcomD.Description = LcomD.Description + strDesc; }
			if (nType == 1) SetCvlJTJD60_15_ETC_M(LcomD, Lcom_cf, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWLoad, FALSE, arWlLoad, TRUE, arSfLoad, FALSE, arIpLoad, FALSE,
				arTLoad, arTpgLoad, TRUE, arFrLoad, FALSE, 0, dFactor, bAcdn, nMsgType);
			if (nType == 2) SetCvlJTJD60_15_ETC_M(LcomD, Lcom_cf, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWLoad, FALSE, arWlLoad, TRUE, arSfLoad, FALSE, arIpLoad, FALSE,
				arTLoad, arTpgLoad, TRUE, arFrLoad, FALSE, 0, dFactor, bAcdn, nMsgType);
			if (nType == 6) SetCvlJTJD60_15_ETC_M(LcomD, Lcom_cf, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWLoad, FALSE, arWlLoad, TRUE, arSfLoad, FALSE, arIpLoad, FALSE,
				arTLoad, arTpgLoad, FALSE, arFrLoad, FALSE, 3, dFactor, bAcdn, nMsgType);
			if (nType == 7) SetCvlJTJD60_15_ETC_M(LcomD, Lcom_cf, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWLoad, FALSE, arWlLoad, TRUE, arSfLoad, FALSE, arIpLoad, FALSE,
				arTLoad, arTpgLoad, FALSE, arFrLoad, FALSE, 3, dFactor, bAcdn, nMsgType);
			if (nType == 8) SetCvlJTJD60_15_ETC_M(LcomD, Lcom_cf, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWLoad, FALSE, arWlLoad, TRUE, arSfLoad, FALSE, arIpLoad, FALSE,
				arTLoad, arTpgLoad, FALSE, arFrLoad, FALSE, 3, dFactor, bAcdn, nMsgType);
			if (nType == 101) SetCvlJTJD60_15_ETC_M(LcomD, Lcom_cf, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWLoad, FALSE, arWlLoad, TRUE, arSfLoad, FALSE, arIpLoad, FALSE,
				arTLoad, arTpgLoad, FALSE, arFrLoad, FALSE, 3, dFactor, bAcdn, nMsgType);
			if (nType == 102) SetCvlJTJD60_15_ETC_M(LcomD, Lcom_cf, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWLoad, FALSE, arWlLoad, TRUE, arSfLoad, FALSE, arIpLoad, FALSE,
				arTLoad, arTpgLoad, TRUE, arFrLoad, FALSE, 0, dFactor, bAcdn, nMsgType);
			if (nType == 103) SetCvlJTJD60_15_ETC_M(LcomD, Lcom_cf, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWLoad, FALSE, arWlLoad, TRUE, arSfLoad, FALSE, arIpLoad, FALSE,
				arTLoad, arTpgLoad, FALSE, arFrLoad, FALSE, 3, dFactor, bAcdn, nMsgType);
			if (nType == 104) SetCvlJTJD60_15_ETC_M(LcomD, Lcom_cf, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWLoad, FALSE, arWlLoad, TRUE, arSfLoad, FALSE, arIpLoad, FALSE,
				arTLoad, arTpgLoad, FALSE, arFrLoad, FALSE, 3, dFactor, bAcdn, nMsgType);
			if (nType == 0)
			{
				if (bAcdn) m_aAcdnLcomD.Add(LcomD);
				else      Set_LoadComb4Civil(LcomD, D_STR, 0, nMsgType);
			}
		}
	}

	if (bW)
	{
		int nWCount = m_nWCount * 2; // Wind
		if (m_nWCount == 0 && bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(W))) nWCount = 1;
		for (nc = 0; nc < nWCount; nc++)
		{
			T_LCOM_D LcomD = rData;
			LoadCaseNo = GetCountLcomCase(LcomD);
			int nWL = 0;
			if (m_nWCount > 0)
			{
				double dWF1 = (nc % 2 == 0) ? dWF : -1.*dWF;
				if (Set_STLCIncludeCS(arWLoad.GetAt(nc / 2), dWF1, LoadCaseNo, LcomD, 0) > 0)
				{
					CString strW = (dWF1 > 0.0) ? _T("+%sW") : _T("%sW");
					strDesc.Format(strW, GetFactorString(dWF1)); LcomD.Description = LcomD.Description + strDesc;
					if (m_nWCount > 1) { strDesc.Format(_T("[%d]"), nc / 2 + 1); LcomD.Description = LcomD.Description + strDesc; }
				}
			}
			if (bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(W)))
			{
				Set_CStageCase(m_aCSLoad.GetAt(nnn), dWF, LoadCaseNo, LcomD, TRUE);
				strDesc.Format(_T("+%s(cEL)"), GetFactorString(dWF)); LcomD.Description = LcomD.Description + strDesc;
			}
			if (nType == 1) SetCvlJTJD60_15_ETC_M(LcomD, Lcom_cf, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWLoad, FALSE, arWlLoad, FALSE, arSfLoad, TRUE, arIpLoad, TRUE,
				arTLoad, arTpgLoad, TRUE, arFrLoad, TRUE, 0, dFactor, bAcdn, nMsgType);
			if (nType == 2) SetCvlJTJD60_15_ETC_M(LcomD, Lcom_cf, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWLoad, FALSE, arWlLoad, FALSE, arSfLoad, TRUE, arIpLoad, TRUE,
				arTLoad, arTpgLoad, TRUE, arFrLoad, TRUE, 0, dFactor, bAcdn, nMsgType);
			if (nType == 3) SetCvlJTJD60_15_ETC_M(LcomD, Lcom_cf, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWLoad, FALSE, arWlLoad, FALSE, arSfLoad, FALSE, arIpLoad, FALSE,
				arTLoad, arTpgLoad, TRUE, arFrLoad, FALSE, 0, dFactor, bAcdn, nMsgType);
			if (nType == 6) SetCvlJTJD60_15_ETC_M(LcomD, Lcom_cf, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWLoad, FALSE, arWlLoad, FALSE, arSfLoad, TRUE, arIpLoad, TRUE,
				arTLoad, arTpgLoad, TRUE, arFrLoad, FALSE, 2, dFactor, bAcdn, nMsgType);
			if (nType == 7) SetCvlJTJD60_15_ETC_M(LcomD, Lcom_cf, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWLoad, FALSE, arWlLoad, FALSE, arSfLoad, TRUE, arIpLoad, TRUE,
				arTLoad, arTpgLoad, TRUE, arFrLoad, FALSE, 2, dFactor, bAcdn, nMsgType);
			if (nType == 8) SetCvlJTJD60_15_ETC_M(LcomD, Lcom_cf, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWLoad, FALSE, arWlLoad, FALSE, arSfLoad, TRUE, arIpLoad, TRUE,
				arTLoad, arTpgLoad, FALSE, arFrLoad, FALSE, 6, dFactor, bAcdn, nMsgType);
			if (nType == 9) SetCvlJTJD60_15_ETC_M(LcomD, Lcom_cf, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWLoad, FALSE, arWlLoad, FALSE, arSfLoad, TRUE, arIpLoad, TRUE,
				arTLoad, arTpgLoad, FALSE, arFrLoad, FALSE, 6, dFactor, bAcdn, nMsgType);
			if (nType == 0)
			{
				if (bAcdn) m_aAcdnLcomD.Add(LcomD);
				else      Set_LoadComb4Civil(LcomD, D_STR, 0, nMsgType);
			}
		}
	}

	if (bWl)
	{
		int nWlCount = m_nWlCount * 2; // Wind
		if (m_nWlCount == 0 && bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(WL))) nWlCount = 1;
		for (nc = 0; nc < nWlCount; nc++)
		{
			T_LCOM_D LcomD = rData;
			LoadCaseNo = GetCountLcomCase(LcomD);
			int nWL = 0;
			if (m_nWlCount > 0)
			{
				double dWF1 = (nc % 2 == 0) ? dWlF : -1.*dWlF;
				if (Set_STLCIncludeCS(arWlLoad.GetAt(nc / 2), dWF1, LoadCaseNo, LcomD, 0) > 0)
				{
					CString strW = (dWF1 > 0.0) ? _T("+%sWL") : _T("%sWL");
					strDesc.Format(strW, GetFactorString(dWF1)); LcomD.Description = LcomD.Description + strDesc;
					if (m_nWlCount > 1) { strDesc.Format(_T("[%d]"), nc / 2 + 1); LcomD.Description = LcomD.Description + strDesc; }
				}
			}
			if (bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(WL)))
			{
				Set_CStageCase(m_aCSLoad.GetAt(nnn), dWlF, LoadCaseNo, LcomD, TRUE);
				strDesc.Format(_T("+%s(cEL)"), GetFactorString(dWlF)); LcomD.Description = LcomD.Description + strDesc;
			}
			if (nType == 1) SetCvlJTJD60_15_ETC_M(LcomD, Lcom_cf, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWLoad, FALSE, arWlLoad, FALSE, arSfLoad, TRUE, arIpLoad, TRUE,
				arTLoad, arTpgLoad, TRUE, arFrLoad, TRUE, 0, dFactor, bAcdn, nMsgType);
			if (nType == 2) SetCvlJTJD60_15_ETC_M(LcomD, Lcom_cf, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWLoad, FALSE, arWlLoad, FALSE, arSfLoad, TRUE, arIpLoad, TRUE,
				arTLoad, arTpgLoad, TRUE, arFrLoad, TRUE, 0, dFactor, bAcdn, nMsgType);
			if (nType == 3) SetCvlJTJD60_15_ETC_M(LcomD, Lcom_cf, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWLoad, FALSE, arWlLoad, FALSE, arSfLoad, FALSE, arIpLoad, FALSE,
				arTLoad, arTpgLoad, TRUE, arFrLoad, FALSE, 0, dFactor, bAcdn, nMsgType);
			if (nType == 6) SetCvlJTJD60_15_ETC_M(LcomD, Lcom_cf, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWLoad, FALSE, arWlLoad, FALSE, arSfLoad, TRUE, arIpLoad, TRUE,
				arTLoad, arTpgLoad, TRUE, arFrLoad, FALSE, 2, dFactor, bAcdn, nMsgType);
			if (nType == 7) SetCvlJTJD60_15_ETC_M(LcomD, Lcom_cf, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWLoad, FALSE, arWlLoad, FALSE, arSfLoad, TRUE, arIpLoad, TRUE,
				arTLoad, arTpgLoad, TRUE, arFrLoad, FALSE, 2, dFactor, bAcdn, nMsgType);
			if (nType == 8) SetCvlJTJD60_15_ETC_M(LcomD, Lcom_cf, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWLoad, FALSE, arWlLoad, FALSE, arSfLoad, TRUE, arIpLoad, TRUE,
				arTLoad, arTpgLoad, FALSE, arFrLoad, FALSE, 6, dFactor, bAcdn, nMsgType);
			if (nType == 9) SetCvlJTJD60_15_ETC_M(LcomD, Lcom_cf, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWLoad, FALSE, arWlLoad, FALSE, arSfLoad, TRUE, arIpLoad, TRUE,
				arTLoad, arTpgLoad, FALSE, arFrLoad, FALSE, 6, dFactor, bAcdn, nMsgType);
			if (nType == 0)
			{
				if (bAcdn) m_aAcdnLcomD.Add(LcomD);
				else      Set_LoadComb4Civil(LcomD, D_STR, 0, nMsgType);
			}
		}
	}

	if (m_nSfCount > 0 && bSf)
	{
		T_LCOM_D LcomD = rData;
		LoadCaseNo = GetCountLcomCase(LcomD);
		int nSF = 0;
		for (nc = 0; nc < m_nSfCount; nc++) nSF += Set_STLCIncludeCS(arSfLoad.GetAt(nc), dF, LoadCaseNo, LcomD, 0);
		if (nSF > 0) { strDesc.Format(_T("+%sSF"), GetFactorString(dF)); LcomD.Description = LcomD.Description + strDesc; }
		if (nType == 1) SetCvlJTJD60_15_ETC_M(LcomD, Lcom_cf, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWLoad, FALSE, arWlLoad, FALSE, arSfLoad, FALSE, arIpLoad, FALSE,
			arTLoad, arTpgLoad, TRUE, arFrLoad, TRUE, 0, dFactor, bAcdn, nMsgType);
		if (nType == 2) SetCvlJTJD60_15_ETC_M(LcomD, Lcom_cf, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWLoad, FALSE, arWlLoad, FALSE, arSfLoad, FALSE, arIpLoad, FALSE,
			arTLoad, arTpgLoad, TRUE, arFrLoad, TRUE, 0, dFactor, bAcdn, nMsgType);
		if (nType == 4) SetCvlJTJD60_15_ETC_M(LcomD, Lcom_cf, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWLoad, FALSE, arWlLoad, FALSE, arSfLoad, FALSE, arIpLoad, FALSE,
			arTLoad, arTpgLoad, TRUE, arFrLoad, TRUE, 0, dFactor, bAcdn, nMsgType);
		if (nType == 6) SetCvlJTJD60_15_ETC_M(LcomD, Lcom_cf, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWLoad, FALSE, arWlLoad, FALSE, arSfLoad, FALSE, arIpLoad, FALSE,
			arTLoad, arTpgLoad, TRUE, arFrLoad, FALSE, 2, dFactor, bAcdn, nMsgType);
		if (nType == 7) SetCvlJTJD60_15_ETC_M(LcomD, Lcom_cf, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWLoad, FALSE, arWlLoad, FALSE, arSfLoad, FALSE, arIpLoad, FALSE,
			arTLoad, arTpgLoad, TRUE, arFrLoad, FALSE, 2, dFactor, bAcdn, nMsgType);
		if (nType == 0)
		{
			if (bAcdn) m_aAcdnLcomD.Add(LcomD);
			else      Set_LoadComb4Civil(LcomD, D_STR, 0, nMsgType);
		}
	}

	if (m_nIpCount > 0 && bIp)
	{
		T_LCOM_D LcomD = rData;
		LoadCaseNo = GetCountLcomCase(LcomD);
		int nIP = 0;
		for (nc = 0; nc < m_nIpCount; nc++) nIP += Set_STLCIncludeCS(arIpLoad.GetAt(nc), dF, LoadCaseNo, LcomD, 0);
		if (nIP > 0) { strDesc.Format(_T("+%sIP"), GetFactorString(dF)); LcomD.Description = LcomD.Description + strDesc; }
		if (nType == 1) SetCvlJTJD60_15_ETC_M(LcomD, Lcom_cf, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWLoad, FALSE, arWlLoad, FALSE, arSfLoad, FALSE, arIpLoad, FALSE,
			arTLoad, arTpgLoad, TRUE, arFrLoad, TRUE, 0, dFactor, bAcdn, nMsgType);
		if (nType == 2) SetCvlJTJD60_15_ETC_M(LcomD, Lcom_cf, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWLoad, FALSE, arWlLoad, FALSE, arSfLoad, FALSE, arIpLoad, FALSE,
			arTLoad, arTpgLoad, TRUE, arFrLoad, TRUE, 0, dFactor, bAcdn, nMsgType);
		if (nType == 4) SetCvlJTJD60_15_ETC_M(LcomD, Lcom_cf, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWLoad, FALSE, arWlLoad, FALSE, arSfLoad, FALSE, arIpLoad, FALSE,
			arTLoad, arTpgLoad, TRUE, arFrLoad, TRUE, 0, dFactor, bAcdn, nMsgType);
		if (nType == 6) SetCvlJTJD60_15_ETC_M(LcomD, Lcom_cf, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWLoad, FALSE, arWlLoad, FALSE, arSfLoad, FALSE, arIpLoad, FALSE,
			arTLoad, arTpgLoad, TRUE, arFrLoad, FALSE, 2, dFactor, bAcdn, nMsgType);
		if (nType == 7) SetCvlJTJD60_15_ETC_M(LcomD, Lcom_cf, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWLoad, FALSE, arWlLoad, FALSE, arSfLoad, FALSE, arIpLoad, FALSE,
			arTLoad, arTpgLoad, TRUE, arFrLoad, FALSE, 2, dFactor, bAcdn, nMsgType);
		if (nType == 0)
		{
			if (bAcdn) m_aAcdnLcomD.Add(LcomD);
			else      Set_LoadComb4Civil(LcomD, D_STR, 0, nMsgType);
		}
	}

	if (bT && ((m_nTCount > 0 || m_nTpgCount > 0) || (bAddCsLoad && (CheckCSEL(D_SGLD_ER1_CVL, _LSX(T)) || CheckCSEL(D_SGLD_ER1_CVL, _LSX(TPG))))))
	{
		T_LCOM_D LcomD = rData;
		if (nType == 0) SetCvlJTJD60_Capacity_TPG(LcomD, arTLoad, arTpgLoad, arFrLoad, dTFactor, dTepFcator, FALSE, bAcdn, nMsgType);
		else         SetCvlJTJD60_Capacity_TPG(LcomD, arTLoad, arTpgLoad, arFrLoad, dTFactor, dTepFcator, TRUE, bAcdn, nMsgType);
	}

	if (m_nFrCount > 0 && bFr)
	{
		T_LCOM_D LcomD = rData;
		LoadCaseNo = GetCountLcomCase(LcomD);
		int nFR = 0;
		for (nc = 0; nc < m_nFrCount; nc++) nFR += Set_STLCIncludeCS(arFrLoad.GetAt(nc), dF, LoadCaseNo, LcomD, 0);
		if (nFR > 0)
		{
			strDesc.Format(_T("+%sFR"), GetFactorString(dF)); LcomD.Description = LcomD.Description + strDesc;
			if (nType == 0)
			{
				if (bAcdn) m_aAcdnLcomD.Add(LcomD);
				else      Set_LoadComb4Civil(LcomD, D_STR, 0, nMsgType);
			}
		}
	}

	return TRUE;
}
BOOL CLoadCombCtrl::SetCvlJTJD60_15_ETC(T_LCOM_D rData, T_LCOM_D Lcom_cf, BOOL bCF, ArINT& arCrlLoad, ArINT& arMCrlLoad, BOOL bCrl, ArINT& arBrkLoad, BOOL bBrk,
	ArINT& arWLoad, BOOL bW, ArINT& arSfLoad, BOOL bSf, ArINT& arIpLoad, BOOL bIp, ArINT& arTLoad, ArINT& arTpgLoad,
	BOOL bT, ArINT& arFrLoad, BOOL bFr, int nType, double dFactor, BOOL bAcdn, int nMsgType)
{
	int LoadCaseNo = 0, nc = 0, nnn = 0;
	CString strDesc = _T("");
	bool bChange = m_bJTG15_Factor ? (dFactor / m_dJTG15_Factor < 1.0) : (dFactor < 1.0);
	double dF = bChange ? 1.4*dFactor : 1.0;
	double dTepFcator = nMsgType == 115 ? 0.8 : dF;//nMsgType == 115 ´ú±íÅ¼È»×éºÏ
	double dTFactor = nMsgType == 115 ? 1.0 : dTepFcator;
	//double dTepF = nType == 0 ? 1.4*0.75 : 1.4;
	double dWF = bChange ? 1.1*dFactor : 1.0;

	BOOL bAddCsLoad = m_nConstLoad != 0 && CheckCS_SgldK(D_SGLD_ER1_CVL, nnn); // CS or ST+CS

	if ((m_nMCrlCount || m_nCrlCount > 0) && bCrl)
	{
		T_LCOM_D LcomD = rData;
		LoadCaseNo = GetCountLcomCase(LcomD);
		int nCRL = 0;
		for (nc = 0; nc < m_nCrlCount; nc++) nCRL += Set_STLCIncludeCS(arCrlLoad.GetAt(nc), dF, LoadCaseNo, LcomD, 0);
		T_LCOM_D LcomD_tmp = Lcom_cf;
		if (bCF && bBrk)
		{
			LoadCaseNo = GetCountLcomCase(LcomD_tmp);
			for (nc = 0; nc < m_nCrlCount; nc++) Set_STLCIncludeCS(arCrlLoad.GetAt(nc), dF, LoadCaseNo, LcomD_tmp, 0);
		}
		if (nCRL > 0)
		{
			strDesc.Format(_T("+%sCRL"), GetFactorString(dF)); LcomD.Description = LcomD.Description + strDesc;
			if (bCF && bBrk) LcomD_tmp.Description = LcomD_tmp.Description + strDesc;
		}
		int nMCrlCount = (m_nMCrlCount > 0) ? m_nMCrlCount : ((nCRL > 0) ? 1 : 0);
		for (int crl = 0; crl < nMCrlCount; ++crl)
		{
			T_LCOM_D LcomD_crl = LcomD;
			T_LCOM_D LcomD_crltmp = LcomD_tmp;
			if (m_nMCrlCount > 0)
			{
				Set_MovingCase(arMCrlLoad.GetAt(crl), dF, LoadCaseNo, LcomD_crl, TRUE);
				strDesc.Format(_T("+%sMCRL"), GetFactorString(dF)); LcomD_crl.Description = LcomD_crl.Description + strDesc; LcomD_crltmp.Description = LcomD_crltmp.Description + strDesc;
				if (m_nMCrlCount > 1) { strDesc.Format(_T("[%d]"), crl + 1); LcomD_crl.Description = LcomD_crl.Description + strDesc; LcomD_crltmp.Description = LcomD_crltmp.Description + strDesc; }
			}
			if (nCRL > 0 || m_nMCrlCount > 0)
			{
				if (nType == 1) SetCvlJTJD60_15_ETC(LcomD_crl, LcomD_crltmp, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, bBrk, arWLoad, TRUE, arSfLoad, TRUE, arIpLoad, TRUE,
					arTLoad, arTpgLoad, TRUE, arFrLoad, TRUE, 0, dFactor, bAcdn, nMsgType);
				if (nType == 6) SetCvlJTJD60_15_ETC(LcomD_crl, LcomD_crltmp, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, bBrk, arWLoad, TRUE, arSfLoad, TRUE, arIpLoad, TRUE,
					arTLoad, arTpgLoad, TRUE, arFrLoad, FALSE, 2, dFactor, bAcdn, nMsgType);
				if (nType == 8) SetCvlJTJD60_15_ETC(LcomD_crl, LcomD_crltmp, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, bBrk, arWLoad, TRUE, arSfLoad, TRUE, arIpLoad, TRUE,
					arTLoad, arTpgLoad, FALSE, arFrLoad, FALSE, 6, dFactor, bAcdn, nMsgType);
				if (nType == 10) SetCvlJTJD60_15_ETC(LcomD_crl, LcomD_crltmp, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWLoad, TRUE, arSfLoad, FALSE, arIpLoad, FALSE,
					arTLoad, arTpgLoad, FALSE, arFrLoad, FALSE, 8, dFactor, bAcdn, nMsgType);
				if (nType == 101) SetCvlJTJD60_15_ETC(LcomD_crl, LcomD_crltmp, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, bBrk, arWLoad, FALSE, arSfLoad, FALSE, arIpLoad, FALSE,
					arTLoad, arTpgLoad, FALSE, arFrLoad, FALSE, 102, dFactor, bAcdn, nMsgType);
				if (nType == 103) SetCvlJTJD60_15_ETC(LcomD_crl, LcomD_crltmp, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, bBrk, arWLoad, TRUE, arSfLoad, FALSE, arIpLoad, FALSE,
					arTLoad, arTpgLoad, FALSE, arFrLoad, FALSE, 104, dFactor, bAcdn, nMsgType);
				if (nType == 0)
				{
					if (bAcdn) m_aAcdnLcomD.Add(LcomD_crl);
					else      Set_LoadComb4Civil(LcomD_crl, D_STR, 0, nMsgType);
				}
			}
		}
	}

	if (nType == 8) bBrk = FALSE;
	if (m_nBrkCount > 0 && bBrk)
	{
		for (int i = 0; i < 2; ++i)
		{
			if (i == 1 && !bCF) break;
			T_LCOM_D LcomD = (i == 0) ? rData : Lcom_cf;
			LoadCaseNo = GetCountLcomCase(LcomD);
			double dBrkF = (i == 0) ? (bCF ? dF * 0.7 : dF) : dF;
			int nBRK = 0;
			for (nc = 0; nc < m_nBrkCount; nc++) nBRK += Set_STLCIncludeCS(arBrkLoad.GetAt(nc), dBrkF, LoadCaseNo, LcomD, 0);
			if (nBRK > 0) { strDesc.Format(_T("+%sBRK"), GetFactorString(dBrkF)); LcomD.Description = LcomD.Description + strDesc; }
			if (nType == 1) SetCvlJTJD60_15_ETC(LcomD, Lcom_cf, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWLoad, TRUE, arSfLoad, FALSE, arIpLoad, FALSE,
				arTLoad, arTpgLoad, TRUE, arFrLoad, FALSE, 0, dFactor, bAcdn, nMsgType);
			if (nType == 2) SetCvlJTJD60_15_ETC(LcomD, Lcom_cf, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWLoad, TRUE, arSfLoad, FALSE, arIpLoad, FALSE,
				arTLoad, arTpgLoad, TRUE, arFrLoad, FALSE, 0, dFactor, bAcdn, nMsgType);
			if (nType == 6) SetCvlJTJD60_15_ETC(LcomD, Lcom_cf, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWLoad, TRUE, arSfLoad, FALSE, arIpLoad, FALSE,
				arTLoad, arTpgLoad, FALSE, arFrLoad, FALSE, 3, dFactor, bAcdn, nMsgType);
			if (nType == 7) SetCvlJTJD60_15_ETC(LcomD, Lcom_cf, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWLoad, TRUE, arSfLoad, FALSE, arIpLoad, FALSE,
				arTLoad, arTpgLoad, FALSE, arFrLoad, FALSE, 3, dFactor, bAcdn, nMsgType);
			if (nType == 8) SetCvlJTJD60_15_ETC(LcomD, Lcom_cf, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWLoad, TRUE, arSfLoad, FALSE, arIpLoad, FALSE,
				arTLoad, arTpgLoad, FALSE, arFrLoad, FALSE, 3, dFactor, bAcdn, nMsgType);
			if (nType == 101) SetCvlJTJD60_15_ETC(LcomD, Lcom_cf, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWLoad, TRUE, arSfLoad, FALSE, arIpLoad, FALSE,
				arTLoad, arTpgLoad, FALSE, arFrLoad, FALSE, 3, dFactor, bAcdn, nMsgType);
			if (nType == 102) SetCvlJTJD60_15_ETC(LcomD, Lcom_cf, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWLoad, TRUE, arSfLoad, FALSE, arIpLoad, FALSE,
				arTLoad, arTpgLoad, TRUE, arFrLoad, FALSE, 0, dFactor, bAcdn, nMsgType);
			if (nType == 103) SetCvlJTJD60_15_ETC(LcomD, Lcom_cf, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWLoad, TRUE, arSfLoad, FALSE, arIpLoad, FALSE,
				arTLoad, arTpgLoad, FALSE, arFrLoad, FALSE, 3, dFactor, bAcdn, nMsgType);
			if (nType == 104) SetCvlJTJD60_15_ETC(LcomD, Lcom_cf, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWLoad, TRUE, arSfLoad, FALSE, arIpLoad, FALSE,
				arTLoad, arTpgLoad, FALSE, arFrLoad, FALSE, 3, dFactor, bAcdn, nMsgType);
			if (nType == 0)
			{
				if (bAcdn) m_aAcdnLcomD.Add(LcomD);
				else      Set_LoadComb4Civil(LcomD, D_STR, 0, nMsgType);
			}
		}
	}
	if (bW)
	{
		int nWCount = m_nWCount * 2; // Wind
		if (m_nWCount == 0 && bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(W))) nWCount = 1;
		for (nc = 0; nc < nWCount; nc++)
		{
			T_LCOM_D LcomD = rData;
			LoadCaseNo = GetCountLcomCase(LcomD);
			int nWL = 0;
			if (m_nWCount > 0)
			{
				double dWF1 = (nc % 2 == 0) ? dWF : -1.*dWF;
				if (Set_STLCIncludeCS(arWLoad.GetAt(nc / 2), dWF1, LoadCaseNo, LcomD, 0) > 0)
				{
					CString strW = (dWF1 > 0.0) ? _T("+%sW") : _T("%sW");
					strDesc.Format(strW, GetFactorString(dWF1)); LcomD.Description = LcomD.Description + strDesc;
					if (m_nWCount > 1) { strDesc.Format(_T("[%d]"), nc / 2 + 1); LcomD.Description = LcomD.Description + strDesc; }
				}
			}
			if (bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(W)))
			{
				Set_CStageCase(m_aCSLoad.GetAt(nnn), dWF, LoadCaseNo, LcomD, TRUE);
				strDesc.Format(_T("+%s(cEL)"), GetFactorString(dWF)); LcomD.Description = LcomD.Description + strDesc;
			}
			if (nType == 1) SetCvlJTJD60_15_ETC(LcomD, Lcom_cf, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWLoad, FALSE, arSfLoad, TRUE, arIpLoad, TRUE,
				arTLoad, arTpgLoad, TRUE, arFrLoad, TRUE, 0, dFactor, bAcdn, nMsgType);
			if (nType == 2) SetCvlJTJD60_15_ETC(LcomD, Lcom_cf, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWLoad, FALSE, arSfLoad, TRUE, arIpLoad, TRUE,
				arTLoad, arTpgLoad, TRUE, arFrLoad, TRUE, 0, dFactor, bAcdn, nMsgType);
			if (nType == 3) SetCvlJTJD60_15_ETC(LcomD, Lcom_cf, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWLoad, FALSE, arSfLoad, FALSE, arIpLoad, FALSE,
				arTLoad, arTpgLoad, TRUE, arFrLoad, FALSE, 0, dFactor, bAcdn, nMsgType);
			if (nType == 6) SetCvlJTJD60_15_ETC(LcomD, Lcom_cf, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWLoad, FALSE, arSfLoad, TRUE, arIpLoad, TRUE,
				arTLoad, arTpgLoad, TRUE, arFrLoad, FALSE, 2, dFactor, bAcdn, nMsgType);
			if (nType == 7) SetCvlJTJD60_15_ETC(LcomD, Lcom_cf, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWLoad, FALSE, arSfLoad, TRUE, arIpLoad, TRUE,
				arTLoad, arTpgLoad, TRUE, arFrLoad, FALSE, 2, dFactor, bAcdn, nMsgType);
			if (nType == 8) SetCvlJTJD60_15_ETC(LcomD, Lcom_cf, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWLoad, FALSE, arSfLoad, TRUE, arIpLoad, TRUE,
				arTLoad, arTpgLoad, FALSE, arFrLoad, FALSE, 6, dFactor, bAcdn, nMsgType);
			if (nType == 9) SetCvlJTJD60_15_ETC(LcomD, Lcom_cf, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWLoad, FALSE, arSfLoad, TRUE, arIpLoad, TRUE,
				arTLoad, arTpgLoad, FALSE, arFrLoad, FALSE, 6, dFactor, bAcdn, nMsgType);
			if (nType == 0)
			{
				if (bAcdn) m_aAcdnLcomD.Add(LcomD);
				else      Set_LoadComb4Civil(LcomD, D_STR, 0, nMsgType);
			}
		}
	}

	if (m_nSfCount > 0 && bSf)
	{
		T_LCOM_D LcomD = rData;
		LoadCaseNo = GetCountLcomCase(LcomD);
		int nSF = 0;
		for (nc = 0; nc < m_nSfCount; nc++) nSF += Set_STLCIncludeCS(arSfLoad.GetAt(nc), dF, LoadCaseNo, LcomD, 0);
		if (nSF > 0) { strDesc.Format(_T("+%sSF"), GetFactorString(dF)); LcomD.Description = LcomD.Description + strDesc; }
		if (nType == 1) SetCvlJTJD60_15_ETC(LcomD, Lcom_cf, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWLoad, FALSE, arSfLoad, FALSE, arIpLoad, FALSE,
			arTLoad, arTpgLoad, TRUE, arFrLoad, TRUE, 0, dFactor, bAcdn, nMsgType);
		if (nType == 2) SetCvlJTJD60_15_ETC(LcomD, Lcom_cf, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWLoad, FALSE, arSfLoad, FALSE, arIpLoad, FALSE,
			arTLoad, arTpgLoad, TRUE, arFrLoad, TRUE, 0, dFactor, bAcdn, nMsgType);
		if (nType == 4) SetCvlJTJD60_15_ETC(LcomD, Lcom_cf, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWLoad, FALSE, arSfLoad, FALSE, arIpLoad, FALSE,
			arTLoad, arTpgLoad, TRUE, arFrLoad, TRUE, 0, dFactor, bAcdn, nMsgType);
		if (nType == 6) SetCvlJTJD60_15_ETC(LcomD, Lcom_cf, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWLoad, FALSE, arSfLoad, FALSE, arIpLoad, FALSE,
			arTLoad, arTpgLoad, TRUE, arFrLoad, FALSE, 2, dFactor, bAcdn, nMsgType);
		if (nType == 7) SetCvlJTJD60_15_ETC(LcomD, Lcom_cf, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWLoad, FALSE, arSfLoad, FALSE, arIpLoad, FALSE,
			arTLoad, arTpgLoad, TRUE, arFrLoad, FALSE, 2, dFactor, bAcdn, nMsgType);
		if (nType == 0)
		{
			if (bAcdn) m_aAcdnLcomD.Add(LcomD);
			else      Set_LoadComb4Civil(LcomD, D_STR, 0, nMsgType);
		}
	}

	if (m_nIpCount > 0 && bIp)
	{
		T_LCOM_D LcomD = rData;
		LoadCaseNo = GetCountLcomCase(LcomD);
		int nIP = 0;
		for (nc = 0; nc < m_nIpCount; nc++) nIP += Set_STLCIncludeCS(arIpLoad.GetAt(nc), dF, LoadCaseNo, LcomD, 0);
		if (nIP > 0) { strDesc.Format(_T("+%sIP"), GetFactorString(dF)); LcomD.Description = LcomD.Description + strDesc; }
		if (nType == 1) SetCvlJTJD60_15_ETC(LcomD, Lcom_cf, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWLoad, FALSE, arSfLoad, FALSE, arIpLoad, FALSE,
			arTLoad, arTpgLoad, TRUE, arFrLoad, TRUE, 0, dFactor, bAcdn, nMsgType);
		if (nType == 2) SetCvlJTJD60_15_ETC(LcomD, Lcom_cf, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWLoad, FALSE, arSfLoad, FALSE, arIpLoad, FALSE,
			arTLoad, arTpgLoad, TRUE, arFrLoad, TRUE, 0, dFactor, bAcdn, nMsgType);
		if (nType == 4) SetCvlJTJD60_15_ETC(LcomD, Lcom_cf, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWLoad, FALSE, arSfLoad, FALSE, arIpLoad, FALSE,
			arTLoad, arTpgLoad, TRUE, arFrLoad, TRUE, 0, dFactor, bAcdn, nMsgType);
		if (nType == 6) SetCvlJTJD60_15_ETC(LcomD, Lcom_cf, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWLoad, FALSE, arSfLoad, FALSE, arIpLoad, FALSE,
			arTLoad, arTpgLoad, TRUE, arFrLoad, FALSE, 2, dFactor, bAcdn, nMsgType);
		if (nType == 7) SetCvlJTJD60_15_ETC(LcomD, Lcom_cf, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWLoad, FALSE, arSfLoad, FALSE, arIpLoad, FALSE,
			arTLoad, arTpgLoad, TRUE, arFrLoad, FALSE, 2, dFactor, bAcdn, nMsgType);
		if (nType == 0)
		{
			if (bAcdn) m_aAcdnLcomD.Add(LcomD);
			else      Set_LoadComb4Civil(LcomD, D_STR, 0, nMsgType);
		}
	}

	if (bT && ((m_nTCount > 0 || m_nTpgCount > 0) || (bAddCsLoad && (CheckCSEL(D_SGLD_ER1_CVL, _LSX(T)) || CheckCSEL(D_SGLD_ER1_CVL, _LSX(TPG))))))
	{
		T_LCOM_D LcomD = rData;
		if (nType == 0) SetCvlJTJD60_Capacity_TPG(LcomD, arTLoad, arTpgLoad, arFrLoad, dTFactor, dTepFcator, FALSE, bAcdn, nMsgType);
		else         SetCvlJTJD60_Capacity_TPG(LcomD, arTLoad, arTpgLoad, arFrLoad, dTFactor, dTepFcator, TRUE, bAcdn, nMsgType);
	}

	if (m_nFrCount > 0 && bFr)
	{
		T_LCOM_D LcomD = rData;
		LoadCaseNo = GetCountLcomCase(LcomD);
		int nFR = 0;
		for (nc = 0; nc < m_nFrCount; nc++) nFR += Set_STLCIncludeCS(arFrLoad.GetAt(nc), dF, LoadCaseNo, LcomD, 0);
		if (nFR > 0)
		{
			strDesc.Format(_T("+%sFR"), GetFactorString(dF)); LcomD.Description = LcomD.Description + strDesc;
			if (nType == 0)
			{
				if (bAcdn) m_aAcdnLcomD.Add(LcomD);
				else      Set_LoadComb4Civil(LcomD, D_STR, 0, nMsgType);
			}
		}
	}
	return TRUE;
}

BOOL CLoadCombCtrl::SetCvlJTGD60_15_Var1_S(const CArray<T_LCOM_D, T_LCOM_D&>& arData, ArUNT& arMLoad, double dIL, ArINT& arLLoad, ArINT& arIlLoad, ArINT& arCrlLoad, ArINT& arMCrlLoad, ArINT& arCfLoad, ArINT& arLsLoad, ArINT& arWLoad, ArINT& arWlLoad, ArINT& arSfLoad, ArINT& arIpLoad, ArINT& arTLoad, ArINT& arTpgLoad, ArINT& arFrLoad, ArINT& arBrkLoad, int nType, int nMsgType)
{
	// nType == 10:×¼ÓÀ¾Ã, 11:ÆµÓö, 12:±ê×¼
	CArray<T_LCOM_D, T_LCOM_D&> aLcomD; aLcomD.Copy(arData);
	int  nLcomSize = aLcomD.GetSize();
	int  nnn = 0;

	BOOL bErec = FALSE;
	for (int nErec = 0; nErec < D_STCT_EREC_MAX_CVL; ++nErec)
	{
		if (m_nConstLoad != 0 && CheckCS_SgldK(D_SGLD_ER1_CVL + nErec, nnn)) bErec = TRUE;
	}

	BOOL bAddCsLoad = bErec;
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER1_CVL,nnn)) || 
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER2_CVL,nnn)) || 
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER3_CVL,nnn)) ||
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER4_CVL,nnn)) ||
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER5_CVL,nnn)) ||
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER6_CVL,nnn)) ||
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER7_CVL,nnn)) ||
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER8_CVL,nnn)) ||
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER9_CVL,nnn)) ||
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER10_CVL,nnn));// CS or ST+CS;

	double dFactor = 1.0, dLFactor = 1.0, dWFactor = 1.0, dWlFactor = 1.0, dCRLFactor = 1.0, dTPGFactor = 1.0;
	if (nType == 10) { dLFactor = 0.4, dWFactor = 0.75, dWlFactor = 1.0, dTPGFactor = 0.8, dCRLFactor = 0.4; }
	else if (nType == 11) { dLFactor = 0.7, dWFactor = 0.75, dWlFactor = 1.0, dTPGFactor = 0.8, dCRLFactor = 1.0; }
	else if (nType == 100) { dLFactor = 1.0, dWFactor = 1.0, dWlFactor = 1.0, dTPGFactor = 1.0, dCRLFactor = 1.0; }
	

	BOOL bUseWlLoad = TRUE;

	int nServEnvType = (nType == 12) ? 1 : 0;
	// ÓÀ¾Ã×÷ÓÃ+ÒÆ¶¯ºÉÔØ + 1¿É±ä×÷ÓÃ
	CString strDesc = _T("");
	int LoadCaseNo = 0, nc = 0;
	for (int lc = 0; lc < nLcomSize; lc++)
	{
		int nMCount = max(m_nMCount, 1);
		for (int m = 0; m < nMCount; m++)
		{
			T_LCOM_D LcomD = aLcomD.GetAt(lc); LcomD.nActive = 1; // Active or Strength/Stress
			if (m_nLcomType == D_LCOMTYPE_CONCRETE)
			{
				if (nType == 10)   LcomD.nActive = 8;  // ×¼ÓÀ¾Ã×éºÏ
				if (nType == 11 || nType == 100)   LcomD.nActive = 7;  // ÆµÓö×éºÏ
				if (nType == 12)
				{
					LcomD.bElasticStage = TRUE;
					LcomD.nActive = 2;
				}
			}
			else if (m_nLcomType == D_LCOMTYPE_STLCOMP || m_nLcomType == D_LCOMTYPE_SRC)
			{
				if (nType == 10) LcomD.nActive = 8;  //×¼ÓÀ¾Ã×éºÏ
				if (nType == 11 || nType == 100) LcomD.nActive = 7;  //ÆµÓö×éºÏ
			}

			T_LCOM_D LcomD_cf = LcomD; // MNET:1614, KJH20050809, cf°¡ Á¦¿ÜµÈ load combination
			LoadCaseNo = GetCountLcomCase(LcomD);

			BOOL bLive = FALSE, bCF = FALSE;
			BOOL bAddLcomD = FALSE;
			//BOOL bFindCJJLeadingMLoad = FALSE;
			// moving load ÒÆ¶¯ºÉÔØ
			if (m_nMCount > 0)
			{
				if (nType == 11) dCRLFactor = 0.4;
				Set_MovingCase(arMLoad.GetAt(m), dLFactor / (1. + dIL), LoadCaseNo, LcomD, TRUE);
				if (nType == 12 || dIL < m_dZeroLim) strDesc.Format(_T("+%sM"), GetFactorString(dLFactor));
				else                            strDesc.Format(_T("+%s/(1+mu)M"), GetFactorString(dLFactor));
				LcomD.Description += strDesc;
				if (m_nMCount > 1) { strDesc.Format(_T("[%d]"), m + 1); LcomD.Description += strDesc; }
				bAddLcomD = TRUE;
			}
			int nLL = 0, nIL = 0, nCF = 0;
			if (m_nLCount > 0 || (bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(L)))) // ST or ST+CS
			{
				for (nc = 0; nc < m_nLCount; nc++) nLL += Set_STLCIncludeCS(arLLoad.GetAt(nc), dLFactor, LoadCaseNo, LcomD, 0);
				if (nLL > 0) { strDesc.Format(_T("+%sL"), GetFactorString(dLFactor)); LcomD.Description += strDesc; }
				if (bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(L)))
				{
					Set_CStageCase(m_aCSLoad.GetAt(nnn), dLFactor, LoadCaseNo, LcomD, TRUE);
					strDesc.Format(_T("+%s(cEL)"), GetFactorString(dLFactor)); LcomD.Description += strDesc;
					bAddLcomD = TRUE;
				}
				for (nc = 0; nc < m_nIlCount; nc++) nIL += Set_STLCIncludeCS(arIlLoad.GetAt(nc), dLFactor, LoadCaseNo, LcomD, 0);
				if (nIL > 0) { strDesc.Format(_T("+%sIL"), GetFactorString(dLFactor)); LcomD.Description += strDesc; }
				if (nLL + nIL > 0) bAddLcomD = TRUE;
			}
			bLive = m_nMCount > 0 || (m_nLCount > 0 || (bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(L))));
			if (bLive)
			{
				LcomD_cf = LcomD;
				for (nc = 0; nc < m_nCfCount; nc++) nCF += Set_STLCIncludeCS(arCfLoad.GetAt(nc), dLFactor, LoadCaseNo, LcomD, 0);
				if (nCF > 0) { strDesc.Format(_T("+%sCF"), GetFactorString(dLFactor)); LcomD.Description += strDesc; bCF = TRUE; }
				bAddLcomD = TRUE;
			}
			if (bAddLcomD) Set_LoadComb4Civil(LcomD, D_SER, nServEnvType, nMsgType);

			if (m_nLsCount > 0) // Live Load Surcharge µØÃæ¸½¼Ó»îºÉÔØ
			{
				T_LCOM_D LcomD_l = LcomD; LoadCaseNo = GetCountLcomCase(LcomD_l);
				int nLS = 0;
				for (nc = 0; nc < m_nLsCount; nc++) nLS += Set_STLCIncludeCS(arLsLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD_l, 0);
				if (nLS > 0)
				{
					strDesc.Format(_T("+%sLS"), GetFactorString(dFactor)); LcomD_l.Description += strDesc;
					Set_LoadComb4Civil(LcomD_l, D_SER, nServEnvType, nMsgType);
				}
			}
			// MNET:XXXX-BJLEE(BBONG)-20060502
			if (m_nCrlCount > 0 || m_nMCrlCount > 0) // Crowd Load ÈËÈººÉÔØ
			{
				T_LCOM_D LcomD_l = LcomD; LoadCaseNo = GetCountLcomCase(LcomD_l);
				int nCRL = 0;
				for (nc = 0; nc < m_nCrlCount; nc++) nCRL += Set_STLCIncludeCS(arCrlLoad.GetAt(nc), dCRLFactor, LoadCaseNo, LcomD_l, 0);
				if (nCRL > 0) { strDesc.Format(_T("+%sCRL"), GetFactorString(dCRLFactor)); LcomD_l.Description += strDesc; }

				int nMCrlCount = (m_nMCrlCount > 0) ? m_nMCrlCount : ((nCRL > 0) ? 1 : 0);
				for (int crl = 0; crl < nMCrlCount; ++crl)
				{
					T_LCOM_D LcomD_crl = LcomD_l; LoadCaseNo = GetCountLcomCase(LcomD_crl);
					if (m_nMCrlCount > 0)
					{
						Set_MovingCase(arMCrlLoad.GetAt(crl), dCRLFactor, LoadCaseNo, LcomD_crl, TRUE);
						strDesc.Format(_T("+%sMCRL"), GetFactorString(dCRLFactor)); LcomD_crl.Description += strDesc;
						if (m_nMCrlCount > 1) { strDesc.Format(_T("[%d]"), crl + 1); LcomD_crl.Description += strDesc; }
					}
					if (nCRL > 0 || m_nMCrlCount > 0)
					{
						if (nType == 2 || nType == 4) m_aAcdnLcomD.Add(LcomD_crl);
						else                     Set_LoadComb4Civil(LcomD_crl, D_SER, nServEnvType, nMsgType);
					}
				}
			}

			if (m_nBrkCount > 0 && bLive) // Brake Load ÖÆ¶¯Á¦
			{
				T_LCOM_D LcomD_l = LcomD; LoadCaseNo = GetCountLcomCase(LcomD_l);
				double dBrkF = bCF ? dFactor * 0.7 : dFactor;
				int nBRK = 0;
				for (nc = 0; nc < m_nBrkCount; nc++) nBRK += Set_STLCIncludeCS(arBrkLoad.GetAt(nc), dBrkF, LoadCaseNo, LcomD_l, 0);
				if (nBRK > 0)
				{
					strDesc.Format(_T("+%sBRK"), GetFactorString(dBrkF)); LcomD_l.Description += strDesc;
					Set_LoadComb4Civil(LcomD_l, D_SER, nServEnvType, nMsgType);
				}
				if (bCF)
				{
					LcomD_l = LcomD_cf; LoadCaseNo = GetCountLcomCase(LcomD_l);
					dBrkF = dFactor;
					for (nc = 0; nc < m_nBrkCount; nc++) nBRK += Set_STLCIncludeCS(arBrkLoad.GetAt(nc), dBrkF, LoadCaseNo, LcomD_l, 0);
					if (nBRK > 0)
					{
						strDesc.Format(_T("+%sBRK"), GetFactorString(dBrkF)); LcomD_l.Description += strDesc;
						Set_LoadComb4Civil(LcomD_l, D_SER, nServEnvType, nMsgType);
					}
				}
			}

			if (bUseWlLoad)
			{
				int nWCount = m_nWlCount * 2; // MNET:1614, KJH20050808 ·çºÉÔØ
				if (m_nWlCount == 0 && bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(WL))) nWCount = 1;
				for (nc = 0; nc < nWCount; nc++)
				{
					T_LCOM_D LcomD_l = LcomD; LoadCaseNo = GetCountLcomCase(LcomD_l);
					if (m_nWlCount > 0)
					{
						double dWF = (nc % 2 == 0) ? dWlFactor : -1.*dWlFactor;
						if (Set_STLCIncludeCS(arWlLoad.GetAt(nc / 2), dWF, LoadCaseNo, LcomD_l, 0) > 0)
						{
							CString strW = (dWF > 0.0) ? _T("+%sWL") : _T("%sWL");
							strDesc.Format(strW, GetFactorString(dWF)); LcomD_l.Description += strDesc;
							if (m_nWlCount > 1) { strDesc.Format(_T("[%d]"), nc / 2 + 1); LcomD_l.Description += strDesc; }
						}
					}
					if (bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(WL)))
					{
						Set_CStageCase(m_aCSLoad.GetAt(nnn), dWlFactor, LoadCaseNo, LcomD_l, TRUE);
						strDesc.Format(_T("+%s(cEL)"), GetFactorString(dWlFactor)); LcomD_l.Description += strDesc;
					}
					Set_LoadComb4Civil(LcomD_l, D_SER, nServEnvType, nMsgType);
				}
			}
			else
			{
				int nWCount = m_nWCount * 2; // MNET:1614, KJH20050808 ·çºÉÔØ
				if (m_nWCount == 0 && bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(W))) nWCount = 1;
				for (nc = 0; nc < nWCount; nc++)
				{
					T_LCOM_D LcomD_l = LcomD; LoadCaseNo = GetCountLcomCase(LcomD_l);
					if (m_nWCount > 0)
					{
						double dWF = (nc % 2 == 0) ? dWFactor : -1.*dWFactor;
						if (Set_STLCIncludeCS(arWLoad.GetAt(nc / 2), dWF, LoadCaseNo, LcomD_l, 0) > 0)
						{
							CString strW = (dWF > 0.0) ? _T("+%sW") : _T("%sW");
							strDesc.Format(strW, GetFactorString(dWF)); LcomD_l.Description += strDesc;
							if (m_nWCount > 1) { strDesc.Format(_T("[%d]"), nc / 2 + 1); LcomD_l.Description += strDesc; }
						}
					}
					if (bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(W)))
					{
						Set_CStageCase(m_aCSLoad.GetAt(nnn), dWFactor, LoadCaseNo, LcomD_l, TRUE);
						strDesc.Format(_T("+%s(cEL)"), GetFactorString(dWFactor)); LcomD_l.Description += strDesc;
					}
					Set_LoadComb4Civil(LcomD_l, D_SER, nServEnvType, nMsgType);
				}
			}
			if (m_nSfCount > 0) // Stream Flow Pressure Á÷Ë®Ñ¹Á¦
			{
				T_LCOM_D LcomD_l = LcomD; LoadCaseNo = GetCountLcomCase(LcomD_l);
				int nSF = 0;
				for (nc = 0; nc < m_nSfCount; nc++) nSF += Set_STLCIncludeCS(arSfLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD_l, 0);
				if (nSF > 0)
				{
					strDesc.Format(_T("+%sSF"), GetFactorString(dFactor)); LcomD_l.Description += strDesc;
					Set_LoadComb4Civil(LcomD_l, D_SER, nServEnvType, nMsgType);
				}
			}
			if (m_nIpCount > 0) // Ice Pressure ±ùÑ¹Á¦
			{
				T_LCOM_D LcomD_l = LcomD; LoadCaseNo = GetCountLcomCase(LcomD_l);
				int nIP = 0;
				for (nc = 0; nc < m_nIpCount; nc++) nIP += Set_STLCIncludeCS(arIpLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD_l, 0);
				if (nIP > 0)
				{
					strDesc.Format(_T("+%sIP"), GetFactorString(dFactor)); LcomD_l.Description += strDesc;
					Set_LoadComb4Civil(LcomD_l, D_SER, nServEnvType, nMsgType);
				}
			}
			// Temeperature ÎÂ¶ÈºÉÔØ
			if ((m_nTCount > 0 || m_nTpgCount > 0) || (bAddCsLoad && (CheckCSEL(D_SGLD_ER1_CVL, _LSX(T)) || CheckCSEL(D_SGLD_ER1_CVL, _LSX(TPG)))))
			{
				T_LCOM_D LcomD_l = LcomD;
				SetCvlJTJD60_Capacity_TPG(LcomD_l, arTLoad, arTpgLoad, arFrLoad, dFactor, dTPGFactor, FALSE, FALSE, nMsgType, D_SER, nServEnvType);
			}
			if (m_nFrCount > 0) // Friction Ä¦²ÁÁ¦
			{
				T_LCOM_D LcomD_l = LcomD; LoadCaseNo = GetCountLcomCase(LcomD_l);
				int nFR = 0;
				for (nc = 0; nc < m_nFrCount; nc++) nFR += Set_STLCIncludeCS(arFrLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD_l, 0);
				if (nFR > 0)
				{
					strDesc.Format(_T("+%sFR"), GetFactorString(dFactor)); LcomD_l.Description += strDesc;
					Set_LoadComb4Civil(LcomD_l, D_SER, nServEnvType, nMsgType);
				}
			}
		} // next m
	} // next lc

	return TRUE;
}
BOOL CLoadCombCtrl::SetCvlJTGD60_15_Var2_S(const CArray<T_LCOM_D, T_LCOM_D&>& arData, ArUNT& arMLoad, double dIL, ArINT& arLLoad, ArINT& arIlLoad, ArINT& arCrlLoad, ArINT& arMCrlLoad, ArINT& arCfLoad, ArINT& arLsLoad, ArINT& arWLoad, ArINT& arWlLoad, ArINT& arSfLoad, ArINT& arIpLoad, ArINT& arTLoad, ArINT& arTpgLoad, ArINT& arFrLoad, ArINT& arBrkLoad, int nType, int nMsgType)
{
	// nType == 10:×¼ÓÀ¾Ã×éºÏ, 11:ÆµÓö×éºÏ, 12:±ê×¼×éºÏ
	CArray<T_LCOM_D, T_LCOM_D&> aLcomD; aLcomD.Copy(arData);
	int  nLcomSize = aLcomD.GetSize();
	int  nnn = 0;

	BOOL bErec = FALSE;
	for (int nErec = 0; nErec < D_STCT_EREC_MAX_CVL; ++nErec)
	{
		if (m_nConstLoad != 0 && CheckCS_SgldK(D_SGLD_ER1_CVL + nErec, nnn)) bErec = TRUE;
	}

	BOOL bAddCsLoad = bErec;
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER1_CVL,nnn)) || 
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER2_CVL,nnn)) || 
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER3_CVL,nnn)) ||
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER4_CVL,nnn)) ||
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER5_CVL,nnn)) ||
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER6_CVL,nnn)) ||
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER7_CVL,nnn)) ||
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER8_CVL,nnn)) ||
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER9_CVL,nnn)) ||
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER10_CVL,nnn));// CS or ST+CS;

	double dFactor = 1.0, dLFactor = 1.0, dWFactor = 1.0, dWlFactor = 1.0, dCRLFactor = 1.0, dTPGFactor = 1.0;
	if (nType == 10) { dLFactor = 0.4, dWFactor = 0.75, dWlFactor = 1.0, dTPGFactor = 0.8, dCRLFactor = 0.4; }
	else if (nType == 11) { dLFactor = 0.7, dWFactor = 0.75, dWlFactor = 1.0, dTPGFactor = 0.8, dCRLFactor = 1.0; }//modified by gongxing at 2016-01-22
	else if (nType == 100) { dLFactor = 1.0, dWFactor = 1.0, dWlFactor = 1.0, dTPGFactor = 1.0, dCRLFactor = 1.0; }

	BOOL bUseWlLoad = TRUE;

	ArINT arTmp;

	int nServEnvType = (nType == 12) ? 1 : 0;
	// ÓÀ¾Ã×÷ÓÃ + ÒÆ¶¯ºÉÔØ + 2¿É±ä×÷ÓÃ
	CString strDesc = _T("");
	int LoadCaseNo = 0, nc = 0;
	for (int lc = 0; lc < nLcomSize; lc++)
	{
		int nMCount = max(m_nMCount, 1);
		for (int m = 0; m < nMCount; m++)
		{
			T_LCOM_D LcomD = aLcomD.GetAt(lc); LcomD.nActive = 1; // Active or Strength/Stress
			if (m_nLcomType == D_LCOMTYPE_CONCRETE)
			{
				if (nType == 10)   LcomD.nActive = 8;  // ×¼ÓÀ¾Ã×éºÏ
				if (nType == 11 || nType == 100)   LcomD.nActive = 7;  // ÆµÓö×éºÏ
				if (nType == 12)
				{
					LcomD.bElasticStage = TRUE;
					LcomD.nActive = 2;
				}
			}
			else if (m_nLcomType == D_LCOMTYPE_STLCOMP || m_nLcomType == D_LCOMTYPE_SRC)
			{
				if (nType == 10) LcomD.nActive = 8;  //×¼ÓÀ¾Ã×éºÏ
				if (nType == 11 || nType == 100) LcomD.nActive = 7;  //ÆµÓö×éºÏ
			}
			T_LCOM_D LcomD_cf = LcomD; // MNET:1614, KJH20050809, CF°¡ Á¦¿ÜµÈ load combination
			LoadCaseNo = GetCountLcomCase(LcomD);

			BOOL bLive = FALSE, bCF = FALSE;
			//BOOL bFindCJJLeadingMLoad = FALSE;
			// moving load
			if (m_nMCount > 0)
			{
				if (nType == 11) dCRLFactor = 0.4;
				Set_MovingCase(arMLoad.GetAt(m), dLFactor / (1. + dIL), LoadCaseNo, LcomD, TRUE);
				if (nType == 12 || dIL < m_dZeroLim) strDesc.Format(_T("+%sM"), GetFactorString(dLFactor));
				else                            strDesc.Format(_T("+%s/(1+mu)M"), GetFactorString(dLFactor));
				LcomD.Description += strDesc;
				if (m_nMCount > 1) { strDesc.Format(_T("[%d]"), m + 1); LcomD.Description += strDesc; }
			}
			int nLL = 0, nIL = 0, nCF = 0;
			if (m_nLCount > 0 || (bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(L)))) // ST or ST+CS
			{
				for (nc = 0; nc < m_nLCount; nc++) nLL += Set_STLCIncludeCS(arLLoad.GetAt(nc), dLFactor, LoadCaseNo, LcomD, 0);
				if (nLL > 0) { strDesc.Format(_T("+%sL"), GetFactorString(dLFactor)); LcomD.Description += strDesc; }
				if (bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(L)))
				{
					Set_CStageCase(m_aCSLoad.GetAt(nnn), dLFactor, LoadCaseNo, LcomD, TRUE);
					strDesc.Format(_T("+%s(cEL)"), GetFactorString(dLFactor)); LcomD.Description += strDesc;
				}
				for (nc = 0; nc < m_nIlCount; nc++) nIL += Set_STLCIncludeCS(arIlLoad.GetAt(nc), dLFactor, LoadCaseNo, LcomD, 0);
				if (nIL > 0) { strDesc.Format(_T("+%sIL"), GetFactorString(dLFactor)); LcomD.Description += strDesc; }
			}
			bLive = m_nMCount > 0 || (m_nLCount > 0 || (bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(L))));
			if (bLive)
			{
				LcomD_cf = LcomD;
				for (nc = 0; nc < m_nCfCount; nc++) nCF += Set_STLCIncludeCS(arCfLoad.GetAt(nc), dLFactor, LoadCaseNo, LcomD, 0);
				if (nCF > 0) { strDesc.Format(_T("+%sCF"), GetFactorString(dLFactor)); LcomD.Description += strDesc; bCF = TRUE; }
			}

			if (m_nLsCount > 0) // Live Load Surcharge µØÃæ¸½¼Ó»îºÉÔØ
			{
				T_LCOM_D LcomD_l = LcomD; LoadCaseNo = GetCountLcomCase(LcomD_l);
				int nLS = 0;
				for (nc = 0; nc < m_nLsCount; nc++) nLS += Set_STLCIncludeCS(arLsLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD_l, 0);
				T_LCOM_D LcomD_tmp = LcomD_cf;
				if (bCF)
				{
					LoadCaseNo = GetCountLcomCase(LcomD_tmp);
					for (nc = 0; nc < m_nLsCount; nc++) Set_STLCIncludeCS(arLsLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD_tmp, 0);
				}
				if (nLS > 0)
				{
					strDesc.Format(_T("+%sLS"), GetFactorString(dFactor)); LcomD_l.Description += strDesc;
					if (bCF) LcomD_tmp.Description += strDesc;
					SetCvlJTJD60_15_ETC_S(LcomD_l, LcomD_tmp, bCF, arCrlLoad, arMCrlLoad, TRUE, arBrkLoad, bLive, arWlLoad, TRUE, arSfLoad, TRUE,
						arIpLoad, TRUE, arTLoad, arTpgLoad, TRUE, arFrLoad, TRUE, 0, nMsgType);
				}
			}
			if (m_nCrlCount > 0 || m_nMCrlCount > 0) // Crowd Load ÈËÈººÉÔØ
			{
				T_LCOM_D LcomD_l = LcomD; LoadCaseNo = GetCountLcomCase(LcomD_l);
				int nCRL = 0;
				for (nc = 0; nc < m_nCrlCount; nc++) nCRL += Set_STLCIncludeCS(arCrlLoad.GetAt(nc), dCRLFactor, LoadCaseNo, LcomD_l, 0);
				T_LCOM_D LcomD_tmp = LcomD_cf;
				if (bCF)
				{
					LoadCaseNo = GetCountLcomCase(LcomD_tmp);
					for (nc = 0; nc < m_nCrlCount; nc++) Set_STLCIncludeCS(arCrlLoad.GetAt(nc), dCRLFactor, LoadCaseNo, LcomD_tmp, 0);
				}
				// MNET:XXXX-BJLEE(BBONG)-20060502 
				if (nCRL > 0 || m_nMCrlCount > 0)
				{
					if (nCRL > 0)
					{
						strDesc.Format(_T("+%sCRL"), GetFactorString(dCRLFactor)); LcomD_l.Description += strDesc;
						if (bCF) LcomD_tmp.Description += strDesc;
					}

					int nMCrlCount = (m_nMCrlCount > 0) ? m_nMCrlCount : ((nCRL > 0) ? 1 : 0);
					T_LCOM_D LcomD_tmp2;
					for (int crl = 0; crl < nMCrlCount; ++crl)
					{
						T_LCOM_D LcomD_crl = LcomD_l; LoadCaseNo = GetCountLcomCase(LcomD_crl);
						LcomD_tmp2.Initialize();
						if (m_nMCrlCount > 0)
						{
							Set_MovingCase(arMCrlLoad.GetAt(crl), dCRLFactor, LoadCaseNo, LcomD_crl, TRUE);
							strDesc.Format(_T("+%sMCRL"), GetFactorString(dCRLFactor)); LcomD_crl.Description += strDesc;
							if (m_nMCrlCount > 1) { strDesc.Format(_T("[%d]"), crl + 1); LcomD_crl.Description += strDesc; }

							if (bCF)
							{
								LcomD_tmp2 = LcomD_tmp; LoadCaseNo = GetCountLcomCase(LcomD_tmp2);
								Set_MovingCase(arMCrlLoad.GetAt(crl), dCRLFactor, LoadCaseNo, LcomD_tmp2, TRUE);
								strDesc.Format(_T("+%sMCRL"), GetFactorString(dCRLFactor)); LcomD_tmp2.Description += strDesc;
								if (m_nMCrlCount > 1) { strDesc.Format(_T("[%d]"), crl + 1); LcomD_tmp2.Description += strDesc; }
							}
						}
						//if((nCRL>0 || m_nMCrlCount>0) && bCF)
						//{
						SetCvlJTJD60_15_ETC_S(LcomD_crl, LcomD_tmp2, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, bLive, arWlLoad, TRUE, arSfLoad, TRUE, arIpLoad, TRUE,
							arTLoad, arTpgLoad, TRUE, arFrLoad, TRUE, 0, nMsgType);
						//}
					}
				}
			}

			if (m_nBrkCount > 0 && bLive) // Brake Load ÖÆ¶¯Á¦
			{
				T_LCOM_D LcomD_l = LcomD; LoadCaseNo = GetCountLcomCase(LcomD_l);
				double dBrkF = bCF ? dFactor * 0.7 : dFactor;
				int nBRK = 0;
				for (nc = 0; nc < m_nBrkCount; nc++) nBRK += Set_STLCIncludeCS(arBrkLoad.GetAt(nc), dBrkF, LoadCaseNo, LcomD_l, 0);
				if (nBRK > 0)
				{
					strDesc.Format(_T("+%sBRK"), GetFactorString(dBrkF)); LcomD_l.Description += strDesc;
					SetCvlJTJD60_15_ETC_S(LcomD_l, LcomD_cf, FALSE, arTmp, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWlLoad, TRUE, arSfLoad, FALSE, arIpLoad, FALSE,
						arTLoad, arTpgLoad, TRUE, arFrLoad, FALSE, 0, nMsgType);
				}
				if (bCF) // MNET:1614, KJH20060809, ¾Õ¿¡¼­ CF°¡ Á¸ÀçÇßÀ¸¸é, CF°¡ Á¦¿ÜµÈ ÇÏÁßÁ¶ÇÕÀ¸·Î ÇÑ¹ø´õ
				{
					LcomD_l = LcomD_cf; LoadCaseNo = GetCountLcomCase(LcomD_l);
					dBrkF = dFactor;
					nBRK = 0;
					for (nc = 0; nc < m_nBrkCount; nc++) nBRK += Set_STLCIncludeCS(arBrkLoad.GetAt(nc), dBrkF, LoadCaseNo, LcomD_l, 0);
					if (nBRK > 0)
					{
						strDesc.Format(_T("+%sBRK"), GetFactorString(dBrkF)); LcomD_l.Description += strDesc;
						SetCvlJTJD60_15_ETC_S(LcomD_l, LcomD_cf, FALSE, arTmp, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWlLoad, TRUE, arSfLoad, FALSE, arIpLoad, FALSE,
							arTLoad, arTpgLoad, TRUE, arFrLoad, FALSE, 0, nMsgType);
					}
				}
			}

			if (bUseWlLoad)
			{
				int nWCount = m_nWlCount * 2; // MNET:1614, KJH20050808 ·çºÉÔØ
				if (m_nWlCount == 0 && bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(WL))) nWCount = 1;
				for (nc = 0; nc < nWCount; nc++)
				{
					T_LCOM_D LcomD_l = LcomD; LoadCaseNo = GetCountLcomCase(LcomD_l);
					if (m_nWlCount > 0)
					{
						double dWF = (nc % 2 == 0) ? dWlFactor : -1.*dWlFactor;
						if (Set_STLCIncludeCS(arWlLoad.GetAt(nc / 2), dWF, LoadCaseNo, LcomD_l, 0) > 0)
						{
							CString strW = (dWF > 0.0) ? _T("+%sWL") : _T("%sWL");
							strDesc.Format(strW, GetFactorString(dWF)); LcomD_l.Description += strDesc;
							if (m_nWlCount > 1) { strDesc.Format(_T("[%d]"), nc / 2 + 1); LcomD_l.Description += strDesc; }
						}
					}
					if (bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(WL)))
					{
						Set_CStageCase(m_aCSLoad.GetAt(nnn), dWlFactor, LoadCaseNo, LcomD_l, TRUE);
						strDesc.Format(_T("+%s(cEL)"), GetFactorString(dWlFactor)); LcomD_l.Description += strDesc;
					}
					SetCvlJTJD60_15_ETC_S(LcomD_l, LcomD_cf, bCF, arTmp, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWlLoad, FALSE, arSfLoad, TRUE, arIpLoad, TRUE,
						arTLoad, arTpgLoad, TRUE, arFrLoad, TRUE, 0, nMsgType);
				}
			}
			else
			{
				int nWCount = m_nWCount * 2; // MNET:1614, KJH20050808 ·çºÉÔØ
				if (m_nWCount == 0 && bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(W))) nWCount = 1;
				for (nc = 0; nc < nWCount; nc++)
				{
					T_LCOM_D LcomD_l = LcomD; LoadCaseNo = GetCountLcomCase(LcomD_l);
					if (m_nWCount > 0)
					{
						double dWF = (nc % 2 == 0) ? dWFactor : -1.*dWFactor;
						if (Set_STLCIncludeCS(arWLoad.GetAt(nc / 2), dWF, LoadCaseNo, LcomD_l, 0) > 0)
						{
							CString strW = (dWF > 0.0) ? _T("+%sW") : _T("%sW");
							strDesc.Format(strW, GetFactorString(dWF)); LcomD_l.Description += strDesc;
							if (m_nWCount > 1) { strDesc.Format(_T("[%d]"), nc / 2 + 1); LcomD_l.Description += strDesc; }
						}
					}
					if (bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(W)))
					{
						Set_CStageCase(m_aCSLoad.GetAt(nnn), dWFactor, LoadCaseNo, LcomD_l, TRUE);
						strDesc.Format(_T("+%s(cEL)"), GetFactorString(dWFactor)); LcomD_l.Description += strDesc;
					}
					SetCvlJTJD60_15_ETC_S(LcomD_l, LcomD_cf, bCF, arTmp, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWlLoad, FALSE, arSfLoad, TRUE, arIpLoad, TRUE,
						arTLoad, arTpgLoad, TRUE, arFrLoad, TRUE, 0, nMsgType);
				}
			}
			if (m_nSfCount > 0) // Stream Flow Pressure Á÷Ë®Ñ¹Á¦
			{
				T_LCOM_D LcomD_l = LcomD; LoadCaseNo = GetCountLcomCase(LcomD_l);
				int nSF = 0;
				for (nc = 0; nc < m_nSfCount; nc++) nSF += Set_STLCIncludeCS(arSfLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD_l, 0);
				if (nSF > 0)
				{
					strDesc.Format(_T("+%sSF"), GetFactorString(dFactor)); LcomD_l.Description += strDesc;
					SetCvlJTJD60_15_ETC_S(LcomD_l, LcomD_cf, bCF, arTmp, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWlLoad, FALSE, arSfLoad, FALSE, arIpLoad, FALSE,
						arTLoad, arTpgLoad, TRUE, arFrLoad, TRUE, 0, nMsgType);
				}
			}
			if (m_nIpCount > 0) // Ice Pressure ±ùÑ¹Á¦
			{
				T_LCOM_D LcomD_l = LcomD; LoadCaseNo = GetCountLcomCase(LcomD_l);
				int nIP = 0;
				for (nc = 0; nc < m_nIpCount; nc++) nIP += Set_STLCIncludeCS(arIpLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD_l, 0);
				if (nIP > 0)
				{
					strDesc.Format(_T("+%sIP"), GetFactorString(dFactor)); LcomD_l.Description += strDesc;
					SetCvlJTJD60_15_ETC_S(LcomD_l, LcomD_cf, bCF, arTmp, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWlLoad, FALSE, arSfLoad, FALSE, arIpLoad, FALSE,
						arTLoad, arTpgLoad, TRUE, arFrLoad, TRUE, 0, nMsgType);
				}
			}
			if ((m_nTCount > 0 || m_nTpgCount > 0) || (bAddCsLoad && (CheckCSEL(D_SGLD_ER1_CVL, _LSX(T)) || CheckCSEL(D_SGLD_ER1_CVL, _LSX(TPG))))) // Temeperature ÎÂ¶ÈºÉÔØ
			{
				T_LCOM_D LcomD_l = LcomD;
				SetCvlJTJD60_Capacity_TPG(LcomD_l, arTLoad, arTpgLoad, arFrLoad, dFactor, dTPGFactor, TRUE, FALSE, nMsgType, D_SER, nServEnvType);
			}
		} // next m
	} // next lc

	return TRUE;
}
BOOL CLoadCombCtrl::SetCvlJTGD60_15_Var3_S(const CArray<T_LCOM_D, T_LCOM_D&>& arData, ArUNT& arMLoad, double dIL, ArINT& arLLoad, ArINT& arIlLoad, ArINT& arCrlLoad, ArINT& arMCrlLoad, ArINT& arCfLoad, ArINT& arLsLoad, ArINT& arWLoad, ArINT& arWlLoad, ArINT& arSfLoad, ArINT& arIpLoad, ArINT& arTLoad, ArINT& arTpgLoad, ArINT& arFrLoad, ArINT& arBrkLoad, int nType, int nMsgType)
{
	// nType == 10:×¼ÓÀ¾Ã×éºÏ 11:ÆµÓö×éºÏ 12:±ê×¼×éºÏ
	CArray<T_LCOM_D, T_LCOM_D&> aLcomD; aLcomD.Copy(arData);
	int nLcomSize = aLcomD.GetSize();
	int  nnn = 0;

	BOOL bErec = FALSE;
	for (int nErec = 0; nErec < D_STCT_EREC_MAX_CVL; ++nErec)
	{
		if (m_nConstLoad != 0 && CheckCS_SgldK(D_SGLD_ER1_CVL + nErec, nnn)) bErec = TRUE;
	}

	BOOL bAddCsLoad = bErec;
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER1_CVL,nnn)) || 
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER2_CVL,nnn)) || 
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER3_CVL,nnn)) ||
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER4_CVL,nnn)) ||
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER5_CVL,nnn)) ||
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER6_CVL,nnn)) ||
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER7_CVL,nnn)) ||
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER8_CVL,nnn)) ||
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER9_CVL,nnn)) ||
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER10_CVL,nnn));// CS or ST+CS;

	double dFactor = 1.0, dLFactor = 1.0, dWFactor = 1.0, dWlFactor = 1.0, dCRLFactor = 1.0, dTPGFactor = 1.0;
	if (nType == 10) { dLFactor = 0.4, dWFactor = 0.75, dWlFactor = 1.0, dTPGFactor = 0.8, dCRLFactor = 0.4; }
	else if (nType == 11) { dLFactor = 0.7, dWFactor = 0.75, dWlFactor = 1.0, dTPGFactor = 0.8, dCRLFactor = 1.0; }//modified by gongxing at 2016-01-22
	else if (nType == 100) { dLFactor = 1.0, dWFactor = 1.0, dWlFactor = 1.0, dTPGFactor = 1.0, dCRLFactor = 1.0; }

	BOOL bUseWlLoad = TRUE;

	ArINT arTmp;

	int nServEnvType = (nType == 12) ? 1 : 0;
	// ÓÀ¾Ã×÷ÓÃ + ÒÆ¶¯ºÉÔØ + 3¿É±ä×÷ÓÃ
	CString strDesc = _T("");
	int LoadCaseNo = 0, nc = 0;
	for (int lc = 0; lc < nLcomSize; lc++)
	{
		int nMCount = max(m_nMCount, 1);
		for (int m = 0; m < nMCount; m++)
		{
			T_LCOM_D LcomD = aLcomD.GetAt(lc); LcomD.nActive = 1; // Active or Strength/Stress
			if (m_nLcomType == D_LCOMTYPE_CONCRETE)
			{
				if (nType == 10)   LcomD.nActive = 8;  // ×¼ÓÀ¾Ã×éºÏ
				if (nType == 11 || nType == 100)   LcomD.nActive = 7;  // ÆµÓö×éºÏ
				if (nType == 12)
				{
					LcomD.bElasticStage = TRUE;
					LcomD.nActive = 2;
				}
			}
			else if (m_nLcomType == D_LCOMTYPE_STLCOMP || m_nLcomType == D_LCOMTYPE_SRC)
			{
				if (nType == 10) LcomD.nActive = 8;  //×¼ÓÀ¾Ã×éºÏ
				if (nType == 11 || nType == 100) LcomD.nActive = 7;  //ÆµÓö×éºÏ
			}
			T_LCOM_D LcomD_cf = LcomD; // MNET:1614, KJH20050809, CF°¡ Á¦¿ÜµÈ load combination
			LoadCaseNo = GetCountLcomCase(LcomD);

			BOOL bLive = FALSE, bCF = FALSE;
			//BOOL bFindCJJLeadingMLoad = FALSE;
			// moving load
			if (m_nMCount > 0)
			{
				if (nType == 11) dCRLFactor = 0.4;
				Set_MovingCase(arMLoad.GetAt(m), dLFactor / (1. + dIL), LoadCaseNo, LcomD, TRUE);
				if (nType == 12 || dIL < m_dZeroLim) strDesc.Format(_T("+%sM"), GetFactorString(dLFactor));
				else                            strDesc.Format(_T("+%s/(1+mu)M"), GetFactorString(dLFactor));
				LcomD.Description += strDesc;
				if (m_nMCount > 1) { strDesc.Format(_T("[%d]"), m + 1); LcomD.Description += strDesc; }
			}
			int nLL = 0, nIL = 0, nCF = 0;
			if (m_nLCount > 0 || (bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(L)))) // ST or ST+CS
			{
				for (nc = 0; nc < m_nLCount; nc++) nLL += Set_STLCIncludeCS(arLLoad.GetAt(nc), dLFactor, LoadCaseNo, LcomD, 0);
				if (nLL > 0) { strDesc.Format(_T("+%sL"), GetFactorString(dLFactor)); LcomD.Description += strDesc; }
				if (bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(L)))
				{
					Set_CStageCase(m_aCSLoad.GetAt(nnn), dLFactor, LoadCaseNo, LcomD, TRUE);
					strDesc.Format(_T("+%s(cEL)"), GetFactorString(dLFactor)); LcomD.Description += strDesc;
				}
				for (nc = 0; nc < m_nIlCount; nc++) nIL += Set_STLCIncludeCS(arIlLoad.GetAt(nc), dLFactor, LoadCaseNo, LcomD, 0);
				if (nIL > 0) { strDesc.Format(_T("+%sIL"), GetFactorString(dLFactor)); LcomD.Description += strDesc; }
			}
			bLive = m_nMCount > 0 || (m_nLCount > 0 || (bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(L))));
			if (bLive)
			{
				LcomD_cf = LcomD;
				for (nc = 0; nc < m_nCfCount; nc++) nCF += Set_STLCIncludeCS(arCfLoad.GetAt(nc), dLFactor, LoadCaseNo, LcomD, 0);
				if (nCF > 0) { strDesc.Format(_T("+%sCF"), GetFactorString(dLFactor)); LcomD.Description += strDesc; bCF = TRUE; }
			}

			if (m_nLsCount > 0) // Live Load Surcharge µØÃæ¸½¼Ó»îºÉÔØ
			{
				T_LCOM_D LcomD_l = LcomD; LoadCaseNo = GetCountLcomCase(LcomD_l);
				int nLS = 0;
				for (nc = 0; nc < m_nLsCount; nc++) nLS += Set_STLCIncludeCS(arLsLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD_l, 0);
				T_LCOM_D LcomD_tmp = LcomD_cf; LoadCaseNo = GetCountLcomCase(LcomD_tmp);
				if (bCF)
				{
					for (nc = 0; nc < m_nLsCount; nc++) Set_STLCIncludeCS(arLsLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD_tmp, 0);
				}
				if (nLS > 0)
				{
					strDesc.Format(_T("+%sLS"), GetFactorString(dFactor)); LcomD_l.Description += strDesc;
					if (bCF) LcomD_tmp.Description += strDesc;
					SetCvlJTJD60_15_ETC_S(LcomD_l, LcomD_tmp, bCF, arCrlLoad, arMCrlLoad, TRUE, arBrkLoad, bLive, arWlLoad, TRUE, arSfLoad, TRUE, arIpLoad, TRUE,
						arTLoad, arTpgLoad, TRUE, arFrLoad, FALSE, 1, nMsgType);
				}
			}

			// MNET:XXXX-BJLEE(BBONG)-20060503
			if (m_nCrlCount > 0 || m_nMCrlCount > 0) // Crowd Load ÈËÈººÉÔØ
			{
				T_LCOM_D LcomD_l = LcomD; LoadCaseNo = GetCountLcomCase(LcomD_l);
				int nCRL = 0;
				for (nc = 0; nc < m_nCrlCount; nc++) nCRL += Set_STLCIncludeCS(arCrlLoad.GetAt(nc), dCRLFactor, LoadCaseNo, LcomD_l, 0);
				T_LCOM_D LcomD_tmp = LcomD_cf; LoadCaseNo = GetCountLcomCase(LcomD_tmp);
				if (bCF)
				{
					for (nc = 0; nc < m_nCrlCount; nc++) Set_STLCIncludeCS(arCrlLoad.GetAt(nc), dCRLFactor, LoadCaseNo, LcomD_tmp, 0);
				}
				// MNET:XXXX-BJLEE(BBONG)-20060502 
				if (nCRL > 0 || m_nMCrlCount > 0)
				{
					if (nCRL > 0)
					{
						strDesc.Format(_T("+%sCRL"), GetFactorString(dCRLFactor)); LcomD_l.Description += strDesc;
						if (bCF) LcomD_tmp.Description += strDesc;
					}

					int nMCrlCount = (m_nMCrlCount > 0) ? m_nMCrlCount : ((nCRL > 0) ? 1 : 0);
					T_LCOM_D LcomD_tmp2;
					for (int crl = 0; crl < nMCrlCount; ++crl)
					{
						T_LCOM_D LcomD_crl = LcomD_l; LoadCaseNo = GetCountLcomCase(LcomD_crl);
						LcomD_tmp2.Initialize();
						if (m_nMCrlCount > 0)
						{
							Set_MovingCase(arMCrlLoad.GetAt(crl), dCRLFactor, LoadCaseNo, LcomD_crl, TRUE);
							strDesc.Format(_T("+%sMCRL"), GetFactorString(dCRLFactor)); LcomD_crl.Description += strDesc;
							if (m_nMCrlCount > 1) { strDesc.Format(_T("[%d]"), crl + 1); LcomD_crl.Description += strDesc; }

							if (bCF)
							{
								LcomD_tmp2 = LcomD_tmp; LoadCaseNo = GetCountLcomCase(LcomD_tmp2);
								Set_MovingCase(arMCrlLoad.GetAt(crl), dCRLFactor, LoadCaseNo, LcomD_tmp2, TRUE);
								strDesc.Format(_T("+%sMCRL"), GetFactorString(dCRLFactor)); LcomD_tmp2.Description += strDesc;
								if (m_nMCrlCount > 1) { strDesc.Format(_T("[%d]"), crl + 1); LcomD_tmp2.Description += strDesc; }
							}
						}
						//if((nCRL>0 || m_nMCrlCount>0) && bCF)
						//{
						SetCvlJTJD60_15_ETC_S(LcomD_crl, LcomD_tmp2, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, bLive, arWlLoad, TRUE, arSfLoad, TRUE, arIpLoad, TRUE,
							arTLoad, arTpgLoad, TRUE, arFrLoad, FALSE, 2, nMsgType);
						//}
					}
				}
			}


			if (m_nBrkCount > 0 && bLive) // Brake Load ÖÆ¶¯Á¦
			{
				T_LCOM_D LcomD_l = LcomD; LoadCaseNo = GetCountLcomCase(LcomD_l);
				double dBrkF = bCF ? dFactor * 0.7 : dFactor;
				int nBRK = 0;
				for (nc = 0; nc < m_nBrkCount; nc++) nBRK += Set_STLCIncludeCS(arBrkLoad.GetAt(nc), dBrkF, LoadCaseNo, LcomD_l, 0);
				if (nBRK > 0)
				{
					strDesc.Format(_T("+%sBRK"), GetFactorString(dBrkF)); LcomD_l.Description += strDesc;
					SetCvlJTJD60_15_ETC_S(LcomD_l, LcomD_cf, FALSE, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWlLoad, TRUE, arSfLoad, FALSE, arIpLoad, FALSE,
						arTLoad, arTpgLoad, FALSE, arFrLoad, FALSE, 3, nMsgType);
				}
				if (bCF) // MNET:1614, KJH20050809, ¾Õ¿¡¼­ CF°¡ Á¸ÀçÇÏ¸é CF°¡ Á¦¿ÜµÈ ÇÏÁßÁ¶ÇÕ ÇÑ¹ø´õ...
				{
					LcomD_l = LcomD_cf; LoadCaseNo = GetCountLcomCase(LcomD_l);
					dBrkF = dFactor;
					int nBRK = 0;
					for (nc = 0; nc < m_nBrkCount; nc++) nBRK += Set_STLCIncludeCS(arBrkLoad.GetAt(nc), dBrkF, LoadCaseNo, LcomD_l, 0);
					if (nBRK > 0)
					{
						strDesc.Format(_T("+%sBRK"), GetFactorString(dBrkF)); LcomD_l.Description += strDesc;
						SetCvlJTJD60_15_ETC_S(LcomD_l, LcomD_cf, FALSE, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWlLoad, TRUE, arSfLoad, FALSE, arIpLoad, FALSE,
							arTLoad, arTpgLoad, FALSE, arFrLoad, FALSE, 3, nMsgType);
					}
				}
			}

			if (bUseWlLoad)
			{
				int nWCount = m_nWlCount * 2; // MNET:1614, KJH20050808 ·çºÉÔØ
				if (m_nWlCount == 0 && bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(WL))) nWCount = 1;
				for (nc = 0; nc < nWCount; nc++)
				{
					T_LCOM_D LcomD_l = LcomD; LoadCaseNo = GetCountLcomCase(LcomD_l);
					if (m_nWlCount > 0)
					{
						double dWF = (nc % 2 == 0) ? dWlFactor : -1.*dWlFactor;
						if (Set_STLCIncludeCS(arWlLoad.GetAt(nc / 2), dWF, LoadCaseNo, LcomD_l, 0) > 0)
						{
							CString strW = (dWF > 0.0) ? _T("+%sWL") : _T("%sWL");
							strDesc.Format(strW, GetFactorString(dWF)); LcomD_l.Description += strDesc;
							if (m_nWlCount > 1) { strDesc.Format(_T("[%d]"), nc / 2 + 1); LcomD_l.Description += strDesc; }
						}
					}
					if (bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(WL)))
					{
						Set_CStageCase(m_aCSLoad.GetAt(nnn), dWlFactor, LoadCaseNo, LcomD_l, TRUE);
						strDesc.Format(_T("+%s(cEL)"), GetFactorString(dWlFactor)); LcomD_l.Description += strDesc;
					}
					SetCvlJTJD60_15_ETC_S(LcomD_l, LcomD_cf, bCF, arTmp, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWlLoad, FALSE, arSfLoad, TRUE, arIpLoad, TRUE,
						arTLoad, arTpgLoad, TRUE, arFrLoad, FALSE, 4, nMsgType);
				}
			}
			else
			{
				int nWCount = m_nWCount * 2; // MNET:1614, KJH20050808 ·çºÉÔØ
				if (m_nWCount == 0 && bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(W))) nWCount = 1;
				for (nc = 0; nc < nWCount; nc++)
				{
					T_LCOM_D LcomD_l = LcomD; LoadCaseNo = GetCountLcomCase(LcomD_l);
					if (m_nWCount > 0)
					{
						double dWF = (nc % 2 == 0) ? dWFactor : -1.*dWFactor;
						if (Set_STLCIncludeCS(arWLoad.GetAt(nc / 2), dWF, LoadCaseNo, LcomD_l, 0) > 0)
						{
							CString strW = (dWF > 0.0) ? _T("+%sW") : _T("%sW");
							strDesc.Format(strW, GetFactorString(dWF)); LcomD_l.Description += strDesc;
							if (m_nWCount > 1) { strDesc.Format(_T("[%d]"), nc / 2 + 1); LcomD_l.Description += strDesc; }
						}
					}
					if (bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(W)))
					{
						Set_CStageCase(m_aCSLoad.GetAt(nnn), dWFactor, LoadCaseNo, LcomD_l, TRUE);
						strDesc.Format(_T("+%s(cEL)"), GetFactorString(dWFactor)); LcomD_l.Description += strDesc;
					}
					SetCvlJTJD60_15_ETC_S(LcomD_l, LcomD_cf, bCF, arTmp, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWlLoad, FALSE, arSfLoad, TRUE, arIpLoad, TRUE,
						arTLoad, arTpgLoad, TRUE, arFrLoad, FALSE, 4, nMsgType);
				}
			}
			if (m_nSfCount > 0) // Stream Flow Pressure Á÷Ë®Ñ¹Á¦
			{
				T_LCOM_D LcomD_l = LcomD; LoadCaseNo = GetCountLcomCase(LcomD_l);
				int nSF = 0;
				for (nc = 0; nc < m_nSfCount; nc++) nSF += Set_STLCIncludeCS(arSfLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD_l, 0);
				if (nSF > 0)
				{
					strDesc.Format(_T("+%sSF"), GetFactorString(dFactor)); LcomD_l.Description += strDesc;
					SetCvlJTJD60_15_ETC_S(LcomD_l, LcomD_cf, bCF, arTmp, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWlLoad, FALSE, arSfLoad, FALSE, arIpLoad, FALSE,
						arTLoad, arTpgLoad, TRUE, arFrLoad, FALSE, 5, nMsgType);
				}
			}
			if (m_nIpCount > 0) // Ice Pressure ±ùÑ¹Á¦
			{
				T_LCOM_D LcomD_l = LcomD; LoadCaseNo = GetCountLcomCase(LcomD_l);
				int nIP = 0;
				for (nc = 0; nc < m_nIpCount; nc++) nIP += Set_STLCIncludeCS(arIpLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD_l, 0);
				if (nIP > 0)
				{
					strDesc.Format(_T("+%sIP"), GetFactorString(dFactor)); LcomD_l.Description += strDesc;
					SetCvlJTJD60_15_ETC_S(LcomD_l, LcomD_cf, bCF, arTmp, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWlLoad, FALSE, arSfLoad, FALSE, arIpLoad, FALSE,
						arTLoad, arTpgLoad, TRUE, arFrLoad, FALSE, 5, nMsgType);
				}
			}
		} // next m
	} // next lc

	return TRUE;
}
BOOL CLoadCombCtrl::SetCvlJTGD60_15_Var4_S(const CArray<T_LCOM_D, T_LCOM_D&>& arData, ArUNT& arMLoad, double dIL, ArINT& arLLoad, ArINT& arIlLoad, ArINT& arCrlLoad, ArINT& arMCrlLoad, ArINT& arCfLoad, ArINT& arLsLoad, ArINT& arWLoad, ArINT& arWlLoad, ArINT& arSfLoad, ArINT& arIpLoad, ArINT& arTLoad, ArINT& arTpgLoad, ArINT& arFrLoad, ArINT& arBrkLoad, int nType, int nMsgType)
{
	// nType == 10:×¼ÓÀ¾Ã×éºÏ, 11:ÆµÓö×éºÏ 12:±ê×¼×éºÏ
	CArray<T_LCOM_D, T_LCOM_D&> aLcomD; aLcomD.Copy(arData);
	int  nLcomSize = aLcomD.GetSize();
	int  nnn = 0;

	BOOL bErec = FALSE;
	for (int nErec = 0; nErec < D_STCT_EREC_MAX_CVL; ++nErec)
	{
		if (m_nConstLoad != 0 && CheckCS_SgldK(D_SGLD_ER1_CVL + nErec, nnn)) bErec = TRUE;
	}

	BOOL bAddCsLoad = bErec;
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER1_CVL,nnn)) || 
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER2_CVL,nnn)) || 
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER3_CVL,nnn)) ||
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER4_CVL,nnn)) ||
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER5_CVL,nnn)) ||
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER6_CVL,nnn)) ||
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER7_CVL,nnn)) ||
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER8_CVL,nnn)) ||
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER9_CVL,nnn)) ||
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER10_CVL,nnn));// CS or ST+CS;

	double dFactor = 1.0, dLFactor = 1.0, dWFactor = 1.0, dWlFactor = 1.0, dCRLFactor = 1.0, dTPGFactor = 1.0;
	if (nType == 10) { dLFactor = 0.4, dWFactor = 0.75, dWlFactor = 1.0, dTPGFactor = 0.8, dCRLFactor = 0.4; }
	else if (nType == 11) { dLFactor = 0.7, dWFactor = 0.75, dWlFactor = 1.0, dTPGFactor = 0.8, dCRLFactor = 1.0; }//modified by gongxing at 2016-01-22
	else if (nType == 100) { dLFactor = 1.0, dWFactor = 1.0, dWlFactor = 1.0, dTPGFactor = 1.0, dCRLFactor = 1.0; }

	BOOL bUseWlLoad = TRUE;


	ArINT arTmp;

	int nServEnvType = (nType == 12) ? 1 : 0;
	// ÓÀ¾Ã×÷ÓÃ + ÆµÓö×éºÏ+ 4¿É±ä×÷ÓÃ
	CString strDesc = _T("");
	int LoadCaseNo = 0, nc = 0;
	for (int lc = 0; lc < nLcomSize; lc++)
	{
		int nMCount = max(m_nMCount, 1);
		for (int m = 0; m < nMCount; m++)
		{
			T_LCOM_D LcomD = aLcomD.GetAt(lc); LcomD.nActive = 1; // Active or Strength/Stress
			if (m_nLcomType == D_LCOMTYPE_CONCRETE)
			{
				if (nType == 10)   LcomD.nActive = 8;  // ×¼ÓÀ¾Ã×éºÏ
				if (nType == 11 || nType == 100)   LcomD.nActive = 7;  // ÆµÓö×éºÏ
				if (nType == 12)
				{
					LcomD.bElasticStage = TRUE;
					LcomD.nActive = 2;
				}
			}
			else if (m_nLcomType == D_LCOMTYPE_STLCOMP || m_nLcomType == D_LCOMTYPE_SRC)
			{
				if (nType == 10) LcomD.nActive = 8;  //×¼ÓÀ¾Ã×éºÏ
				if (nType == 11 || nType == 100) LcomD.nActive = 7;  //ÆµÓö×éºÏ
			}
			T_LCOM_D LcomD_cf = LcomD; // MNET:1614, KJH20050809, CF°¡ Á¦¿ÜµÈ ÇÏÁßÁ¶ÇÕ
			LoadCaseNo = GetCountLcomCase(LcomD);
			BOOL bLive = FALSE, bCF = FALSE;
			//BOOL bFindCJJLeadingMLoad = FALSE;
			// moving load
			if (m_nMCount > 0)
			{
				if (nType == 11) dCRLFactor = 0.4;
				Set_MovingCase(arMLoad.GetAt(m), dLFactor / (1. + dIL), LoadCaseNo, LcomD, TRUE);
				if (nType == 12 || dIL < m_dZeroLim) strDesc.Format(_T("+%sM"), GetFactorString(dLFactor));
				else                            strDesc.Format(_T("+%s/(1+mu)M"), GetFactorString(dLFactor));
				LcomD.Description += strDesc;
				if (m_nMCount > 1) { strDesc.Format(_T("[%d]"), m + 1); LcomD.Description += strDesc; }
			}
			int nLL = 0, nIL = 0, nCF = 0;
			if (m_nLCount > 0 || (bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(L)))) // ST or ST+CS
			{
				for (nc = 0; nc < m_nLCount; nc++) nLL += Set_STLCIncludeCS(arLLoad.GetAt(nc), dLFactor, LoadCaseNo, LcomD, 0);
				if (nLL > 0) { strDesc.Format(_T("+%sL"), GetFactorString(dLFactor)); LcomD.Description += strDesc; }
				if (bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(L)))
				{
					Set_CStageCase(m_aCSLoad.GetAt(nnn), dLFactor, LoadCaseNo, LcomD, TRUE);
					strDesc.Format(_T("+%s(cEL)"), GetFactorString(dLFactor)); LcomD.Description += strDesc;
				}
				for (nc = 0; nc < m_nIlCount; nc++) nIL += Set_STLCIncludeCS(arIlLoad.GetAt(nc), dLFactor, LoadCaseNo, LcomD, 0);
				if (nIL > 0) { strDesc.Format(_T("+%sIL"), GetFactorString(dLFactor)); LcomD.Description += strDesc; }
			}
			bLive = m_nMCount > 0 || (m_nLCount > 0 || (bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(L))));
			if (bLive)
			{
				LcomD_cf = LcomD;
				for (nc = 0; nc < m_nCfCount; nc++) nCF += Set_STLCIncludeCS(arCfLoad.GetAt(nc), dLFactor, LoadCaseNo, LcomD, 0);
				if (nCF > 0) { strDesc.Format(_T("+%sCF"), GetFactorString(dLFactor)); LcomD.Description += strDesc; bCF = TRUE; }
			}

			if (m_nLsCount > 0) // Live Load Surcharge µØÃæ¸½¼Ó»îºÉÔØ
			{
				T_LCOM_D LcomD_l = LcomD; LoadCaseNo = GetCountLcomCase(LcomD_l);
				int nLS = 0;
				for (nc = 0; nc < m_nLsCount; nc++) nLS += Set_STLCIncludeCS(arLsLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD_l, 0);
				T_LCOM_D LcomD_tmp = LcomD_cf; LoadCaseNo = GetCountLcomCase(LcomD_tmp);
				if (bCF)
				{
					for (nc = 0; nc < m_nLsCount; nc++) Set_STLCIncludeCS(arLsLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD_tmp, 0);
				}
				if (nLS > 0)
				{
					strDesc.Format(_T("+%sLS"), GetFactorString(dFactor)); LcomD_l.Description += strDesc;
					if (bCF) LcomD_tmp.Description += strDesc;
					SetCvlJTJD60_15_ETC_S(LcomD_l, LcomD_tmp, bCF, arCrlLoad, arMCrlLoad, TRUE, arBrkLoad, bLive, arWlLoad, TRUE, arSfLoad, TRUE, arIpLoad, TRUE,
						arTLoad, arTpgLoad, FALSE, arFrLoad, FALSE, 6, nMsgType);
				}
				/*
								if(bCF)
								{
									LcomD_l = LcomD_cf; LoadCaseNo = GetCountLcomCase(LcomD_l);
									nLS = 0;
									for(nc=0; nc<m_nLsCount; nc++) nLS += Set_STLCIncludeCS(arLsLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD_l, 0);
									if(nLS>0)
									{
										strDesc.Format(_T("+%sLS"), GetFactorString(dFactor)); LcomD_l.Description += strDesc;
										SetCvlJTJD60_15_ETC_S(LcomD_l, LcomD_cf, bCF, arCrlLoad, TRUE, arBrkLoad, bLive, arWLoad, TRUE, arSfLoad, FALSE, arIpLoad, FALSE,
																		 arTLoad, arTpgLoad, FALSE, arFrLoad, FALSE, 101, nMsgType);
									}
								}
				*/
			}

			// MNET:XXXX-BJLEE(BBONG)-20060503
			if (m_nCrlCount > 0 || m_nMCrlCount > 0) // Crowd Load ÈËÈººÉÔØ
			{
				T_LCOM_D LcomD_l = LcomD; LoadCaseNo = GetCountLcomCase(LcomD_l);
				int nCRL = 0;
				for (nc = 0; nc < m_nCrlCount; nc++) nCRL += Set_STLCIncludeCS(arCrlLoad.GetAt(nc), dCRLFactor, LoadCaseNo, LcomD_l, 0);
				T_LCOM_D LcomD_tmp = LcomD_cf; LoadCaseNo = GetCountLcomCase(LcomD_tmp);
				if (bCF)
				{
					for (nc = 0; nc < m_nCrlCount; nc++) Set_STLCIncludeCS(arCrlLoad.GetAt(nc), dCRLFactor, LoadCaseNo, LcomD_tmp, 0);
				}
				// MNET:XXXX-BJLEE(BBONG)-20060502 
				if (nCRL > 0 || m_nMCrlCount > 0)
				{
					if (nCRL > 0)
					{
						strDesc.Format(_T("+%sCRL"), GetFactorString(dCRLFactor)); LcomD_l.Description += strDesc;
						if (bCF) LcomD_tmp.Description += strDesc;
					}

					int nMCrlCount = (m_nMCrlCount > 0) ? m_nMCrlCount : ((nCRL > 0) ? 1 : 0);
					T_LCOM_D LcomD_tmp2;
					for (int crl = 0; crl < nMCrlCount; ++crl)
					{
						T_LCOM_D LcomD_crl = LcomD_l; LoadCaseNo = GetCountLcomCase(LcomD_crl);
						LcomD_tmp2.Initialize();
						if (m_nMCrlCount > 0)
						{
							Set_MovingCase(arMCrlLoad.GetAt(crl), dCRLFactor, LoadCaseNo, LcomD_crl, TRUE);
							strDesc.Format(_T("+%sMCRL"), GetFactorString(dCRLFactor)); LcomD_crl.Description += strDesc;
							if (m_nMCrlCount > 1) { strDesc.Format(_T("[%d]"), crl + 1); LcomD_crl.Description += strDesc; }

							if (bCF)
							{
								LcomD_tmp2 = LcomD_tmp; LoadCaseNo = GetCountLcomCase(LcomD_tmp2);
								Set_MovingCase(arMCrlLoad.GetAt(crl), dCRLFactor, LoadCaseNo, LcomD_tmp2, TRUE);
								strDesc.Format(_T("+%sMCRL"), GetFactorString(dCRLFactor)); LcomD_tmp2.Description += strDesc;
								if (m_nMCrlCount > 1) { strDesc.Format(_T("[%d]"), crl + 1); LcomD_tmp2.Description += strDesc; }
							}
						}
						//if((nCRL>0 || m_nMCrlCount>0) && bCF)
						//{
						SetCvlJTJD60_15_ETC_S(LcomD_crl, LcomD_tmp2, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, bLive, arWlLoad, TRUE, arSfLoad, TRUE, arIpLoad, TRUE,
							arTLoad, arTpgLoad, FALSE, arFrLoad, FALSE, 7, nMsgType);
						//}
					}
				}
			}

			if (bUseWlLoad)
			{
				int nWCount = m_nWlCount * 2; // MNET:1614, KJH20050808 ·çºÉÔØ
				if (m_nWlCount == 0 && bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(WL))) nWCount = 1;
				for (nc = 0; nc < nWCount; nc++)
				{
					T_LCOM_D LcomD_l = LcomD; LoadCaseNo = GetCountLcomCase(LcomD_l);
					if (m_nWlCount > 0)
					{
						double dWF = (nc % 2 == 0) ? dWlFactor : -1.*dWlFactor;
						if (Set_STLCIncludeCS(arWlLoad.GetAt(nc / 2), dWF, LoadCaseNo, LcomD_l, 0) > 0)
						{
							CString strW = (dWF > 0.0) ? _T("+%sWL") : _T("%sWL");
							strDesc.Format(strW, GetFactorString(dWF)); LcomD_l.Description += strDesc;
							if (m_nWlCount > 1) { strDesc.Format(_T("[%d]"), nc / 2 + 1); LcomD_l.Description += strDesc; }
						}
					}
					if (bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(WL)))
					{
						Set_CStageCase(m_aCSLoad.GetAt(nnn), dWlFactor, LoadCaseNo, LcomD_l, TRUE);
						strDesc.Format(_T("+%s(cEL)"), GetFactorString(dWlFactor)); LcomD_l.Description += strDesc;
					}
					SetCvlJTJD60_15_ETC_S(LcomD_l, LcomD_cf, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWlLoad, FALSE, arSfLoad, TRUE, arIpLoad, TRUE,
						arTLoad, arTpgLoad, FALSE, arFrLoad, FALSE, 7, nMsgType);
				}
			}
			else
			{
				int nWCount = m_nWCount * 2; // MNET:1614, KJH20050808 ·çºÉÔØ
				if (m_nWCount == 0 && bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(W))) nWCount = 1;
				for (nc = 0; nc < nWCount; nc++)
				{
					T_LCOM_D LcomD_l = LcomD; LoadCaseNo = GetCountLcomCase(LcomD_l);
					if (m_nWCount > 0)
					{
						double dWF = (nc % 2 == 0) ? dWFactor : -1.*dWFactor;
						if (Set_STLCIncludeCS(arWLoad.GetAt(nc / 2), dWF, LoadCaseNo, LcomD_l, 0) > 0)
						{
							CString strW = (dWF > 0.0) ? _T("+%sW") : _T("%sW");
							strDesc.Format(strW, GetFactorString(dWF)); LcomD_l.Description += strDesc;
							if (m_nWCount > 1) { strDesc.Format(_T("[%d]"), nc / 2 + 1); LcomD_l.Description += strDesc; }
						}
					}
					if (bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(W)))
					{
						Set_CStageCase(m_aCSLoad.GetAt(nnn), dWFactor, LoadCaseNo, LcomD_l, TRUE);
						strDesc.Format(_T("+%s(cEL)"), GetFactorString(dWFactor)); LcomD_l.Description += strDesc;
					}
					SetCvlJTJD60_15_ETC_S(LcomD_l, LcomD_cf, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWlLoad, FALSE, arSfLoad, TRUE, arIpLoad, TRUE,
						arTLoad, arTpgLoad, FALSE, arFrLoad, FALSE, 7, nMsgType);
				}
			}
		} // next m
	} // next lc

	return TRUE;
}
BOOL CLoadCombCtrl::SetCvlJTGD60_15_Var56_S(const CArray<T_LCOM_D, T_LCOM_D&>& arData, ArUNT& arMLoad, double dIL, ArINT& arLLoad, ArINT& arIlLoad, ArINT& arCrlLoad, ArINT& arMCrlLoad, ArINT& arCfLoad, ArINT& arLsLoad, ArINT& arWLoad, ArINT& arWlLoad, ArINT& arSfLoad, ArINT& arIpLoad, ArINT& arTLoad, ArINT& arTpgLoad, ArINT& arFrLoad, ArINT& arBrkLoad, int nType, int nMsgType)
{
	// nType == 10:×¼ÓÀ¾Ã×éºÏ 11:ÆµÓö×éºÏ, 12:±ê×¼×éºÏ
	CArray<T_LCOM_D, T_LCOM_D&> aLcomD; aLcomD.Copy(arData);
	int  nLcomSize = aLcomD.GetSize();
	int  nnn = 0;

	BOOL bErec = FALSE;
	for (int nErec = 0; nErec < D_STCT_EREC_MAX_CVL; ++nErec)
	{
		if (m_nConstLoad != 0 && CheckCS_SgldK(D_SGLD_ER1_CVL + nErec, nnn)) bErec = TRUE;
	}

	BOOL bAddCsLoad = bErec;
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER1_CVL,nnn)) || 
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER2_CVL,nnn)) || 
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER3_CVL,nnn)) ||
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER4_CVL,nnn)) ||
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER5_CVL,nnn)) ||
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER6_CVL,nnn)) ||
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER7_CVL,nnn)) ||
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER8_CVL,nnn)) ||
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER9_CVL,nnn)) ||
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER10_CVL,nnn));// CS or ST+CS;

	double dFactor = 1.0, dLFactor = 1.0, dWFactor = 1.0, dCRLFactor = 1.0, dTPGFactor = 1.0;
	if (nType == 10) { dLFactor = 0.4, dWFactor = 0.75, dTPGFactor = 0.8, dCRLFactor = 0.4; }
	else if (nType == 11) { dLFactor = 0.7, dWFactor = 0.75, dTPGFactor = 0.8, dCRLFactor = 1.0; }//modified by gongxing at 2016-01-22
	else if (nType == 100) { dLFactor = 1.0, dWFactor = 1.0, dTPGFactor = 1.0, dCRLFactor = 1.0; }

	ArINT arTmp;

	int nServEnvType = (nType == 12) ? 1 : 0;
	// ÓÀ¾Ã×÷ÓÃ + ÒÆ¶¯ºÉÔØ + 5¿É±ä×÷ÓÃ
	CString strDesc = _T("");
	int LoadCaseNo = 0, nc = 0;
	for (int lc = 0; lc < nLcomSize; lc++)
	{
		int nMCount = max(m_nMCount, 1);
		for (int m = 0; m < nMCount; m++)
		{
			T_LCOM_D LcomD = aLcomD.GetAt(lc); LcomD.nActive = 1; // Active or Strength/Stress
			if (m_nLcomType == D_LCOMTYPE_CONCRETE)
			{
				if (nType == 10)   LcomD.nActive = 8;  // ×¼ÓÀ¾Ã×éºÏ
				if (nType == 11 || nType == 100)   LcomD.nActive = 7;  // ÆµÓö×éºÏ
				if (nType == 12)
				{
					LcomD.bElasticStage = TRUE;
					LcomD.nActive = 2;
				}
			}
			else if (m_nLcomType == D_LCOMTYPE_STLCOMP || m_nLcomType == D_LCOMTYPE_SRC)
			{
				if (nType == 10) LcomD.nActive = 8;  //×¼ÓÀ¾Ã×éºÏ
				if (nType == 11 || nType == 100) LcomD.nActive = 7;  //ÆµÓö×éºÏ
			}
			T_LCOM_D LcomD_cf = LcomD; // MNET:1614, KJH20050809, CFÁ¦¿ÜÇÑ ÇÏÁßÁ¶ÇÕ
			LoadCaseNo = GetCountLcomCase(LcomD);
			BOOL bLive = FALSE, bCF = FALSE;
			//BOOL bFindCJJLeadingMLoad = FALSE;
			// moving load
			if (m_nMCount > 0)
			{
				if (nType == 11) dCRLFactor = 0.4;
				Set_MovingCase(arMLoad.GetAt(m), dLFactor / (1. + dIL), LoadCaseNo, LcomD, TRUE);
				if (nType == 12 || dIL < m_dZeroLim) strDesc.Format(_T("+%sM"), GetFactorString(dLFactor));
				else                            strDesc.Format(_T("+%s/(1+mu)M"), GetFactorString(dLFactor));
				LcomD.Description += strDesc;
				if (m_nMCount > 1) { strDesc.Format(_T("[%d]"), m + 1); LcomD.Description += strDesc; }
			}
			int nLL = 0, nIL = 0, nCF = 0;
			if (m_nLCount > 0 || (bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(L)))) // ST or ST+CS
			{
				for (nc = 0; nc < m_nLCount; nc++) nLL += Set_STLCIncludeCS(arLLoad.GetAt(nc), dLFactor, LoadCaseNo, LcomD, 0);
				if (nLL > 0) { strDesc.Format(_T("+%sL"), GetFactorString(dLFactor)); LcomD.Description += strDesc; }
				if (bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(L)))
				{
					Set_CStageCase(m_aCSLoad.GetAt(nnn), dLFactor, LoadCaseNo, LcomD, TRUE);
					strDesc.Format(_T("+%s(cEL)"), GetFactorString(dLFactor)); LcomD.Description += strDesc;
				}
				for (nc = 0; nc < m_nIlCount; nc++) nIL += Set_STLCIncludeCS(arIlLoad.GetAt(nc), dLFactor, LoadCaseNo, LcomD, 0);
				if (nIL > 0) { strDesc.Format(_T("+%sIL"), GetFactorString(dLFactor)); LcomD.Description += strDesc; }
			}
			bLive = m_nMCount > 0 || (m_nLCount > 0 || (bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(L))));
			if (bLive)
			{
				LcomD_cf = LcomD;
				for (nc = 0; nc < m_nCfCount; nc++) nCF += Set_STLCIncludeCS(arCfLoad.GetAt(nc), dLFactor, LoadCaseNo, LcomD, 0);
				if (nCF > 0) { strDesc.Format(_T("+%sCF"), GetFactorString(dLFactor)); LcomD.Description += strDesc; bCF = TRUE; }
			}

			if (m_nLsCount > 0) // Live Load Surcharge µØÃæ¸½¼Ó»îºÉÔØ
			{
				T_LCOM_D LcomD_l = LcomD; LoadCaseNo = GetCountLcomCase(LcomD_l);
				int nLS = 0;
				for (nc = 0; nc < m_nLsCount; nc++) nLS += Set_STLCIncludeCS(arLsLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD_l, 0);
				T_LCOM_D LcomD_tmp = LcomD_cf; LoadCaseNo = GetCountLcomCase(LcomD_tmp);
				if (bCF)
				{
					for (nc = 0; nc < m_nLsCount; nc++) Set_STLCIncludeCS(arLsLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD_tmp, 0);
				}
				if (nLS > 0)
				{
					strDesc.Format(_T("+%sLS"), GetFactorString(dFactor)); LcomD_l.Description += strDesc;
					if (bCF) LcomD_tmp.Description += strDesc;
					SetCvlJTJD60_15_ETC_S(LcomD_l, LcomD_tmp, bCF, arCrlLoad, arMCrlLoad, TRUE, arBrkLoad, bLive, arWlLoad, TRUE, arSfLoad, FALSE, arIpLoad, FALSE,
						arTLoad, arTpgLoad, FALSE, arFrLoad, FALSE, 8, nMsgType);
				}
			}
			// MNET:XXXX-BJLEE(BBONG)-20060503
			if (m_nCrlCount > 0 || m_nMCrlCount > 0) // Crowd Load ÈËÈººÉÔØ
			{
				T_LCOM_D LcomD_l = LcomD; LoadCaseNo = GetCountLcomCase(LcomD_l);
				int nCRL = 0;
				for (nc = 0; nc < m_nCrlCount; nc++) nCRL += Set_STLCIncludeCS(arCrlLoad.GetAt(nc), dCRLFactor, LoadCaseNo, LcomD_l, 0);
				if (nCRL > 0 || m_nMCrlCount > 0)
				{
					if (nCRL > 0)
					{
						strDesc.Format(_T("+%sCRL"), GetFactorString(dCRLFactor)); LcomD_l.Description += strDesc;
					}
					int nMCrlCount = (m_nMCrlCount > 0) ? m_nMCrlCount : ((nCRL > 0) ? 1 : 0);

					for (int crl = 0; crl < nMCrlCount; ++crl)
					{
						T_LCOM_D LcomD_tmp = LcomD_l; LoadCaseNo = GetCountLcomCase(LcomD_tmp);
						if (m_nMCrlCount > 0)
						{
							Set_MovingCase(arMCrlLoad.GetAt(crl), dCRLFactor, LoadCaseNo, LcomD_tmp, TRUE);
							strDesc.Format(_T("+%sMCRL"), GetFactorString(dCRLFactor)); LcomD_tmp.Description += strDesc;
							if (m_nMCrlCount > 1) { strDesc.Format(_T("[%d]"), crl + 1); LcomD_tmp.Description += strDesc; }
						}
						if ((nCRL > 0 || m_nMCrlCount > 0))
						{
							SetCvlJTJD60_15_ETC_S(LcomD_tmp, LcomD_cf, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWlLoad, TRUE, arSfLoad, FALSE, arIpLoad, FALSE,
								arTLoad, arTpgLoad, FALSE, arFrLoad, FALSE, 9, nMsgType);
						}
					}
				}
			}
		} // next m
	} // next lc

	// ¿µ±¸ÇÏÁß + Â÷·®ÇÏÁß + 6°³ÀÇ °¡º¯ÇÏÁßÀÎ °æ¿ì
	for (int lc = 0; lc < nLcomSize; lc++)
	{
		int nMCount = max(m_nMCount, 1);
		for (int m = 0; m < nMCount; m++)
		{
			T_LCOM_D LcomD = aLcomD.GetAt(lc); LcomD.nActive = 1; // Active or Strength/Stress
			if (m_nLcomType == D_LCOMTYPE_CONCRETE)
			{
				if (nType == 10) LcomD.nActive = 8;  // ×¼ÓÀ¾Ã×éºÏ
				if (nType == 11 || nType == 100) LcomD.nActive = 7;  // ÆµÓö×éºÏ
				if (nType == 12)
				{
					LcomD.bElasticStage = TRUE;
					LcomD.nActive = 2;
				}
			}
			else if (m_nLcomType == D_LCOMTYPE_STLCOMP || m_nLcomType == D_LCOMTYPE_SRC)
			{
				if (nType == 10) LcomD.nActive = 8;  //×¼ÓÀ¾Ã×éºÏ
				if (nType == 11 || nType == 100) LcomD.nActive = 7;  //ÆµÓö×éºÏ
			}
			LoadCaseNo = GetCountLcomCase(LcomD);

			// moving load
			if (m_nMCount > 0)
			{
				Set_MovingCase(arMLoad.GetAt(m), dLFactor / (1. + dIL), LoadCaseNo, LcomD, TRUE);
				if (nType == 12 || dIL < m_dZeroLim) strDesc.Format(_T("+%sM"), GetFactorString(dLFactor));
				else                            strDesc.Format(_T("+%s/(1+mu)M"), GetFactorString(dLFactor));
				LcomD.Description += strDesc;
				if (m_nMCount > 1) { strDesc.Format(_T("[%d]"), m + 1); LcomD.Description += strDesc; }
			}
			if (m_nLCount > 0 || (bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(L)))) // ST or ST+CS
			{
				int nLL = 0, nIL = 0, nCF = 0;
				for (nc = 0; nc < m_nLCount; nc++) nLL += Set_STLCIncludeCS(arLLoad.GetAt(nc), dLFactor, LoadCaseNo, LcomD, 0);
				if (nLL > 0) { strDesc.Format(_T("+%sL"), GetFactorString(dLFactor)); LcomD.Description += strDesc; }
				if (bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(L)))
				{
					Set_CStageCase(m_aCSLoad.GetAt(nnn), dLFactor, LoadCaseNo, LcomD, TRUE);
					strDesc.Format(_T("+%s(cEL)"), GetFactorString(dLFactor)); LcomD.Description += strDesc;
				}
				for (nc = 0; nc < m_nIlCount; nc++) nIL += Set_STLCIncludeCS(arIlLoad.GetAt(nc), dLFactor, LoadCaseNo, LcomD, 0);
				if (nIL > 0) { strDesc.Format(_T("+%sIL"), GetFactorString(dLFactor)); LcomD.Description += strDesc; }
				for (nc = 0; nc < m_nCfCount; nc++) nCF += Set_STLCIncludeCS(arCfLoad.GetAt(nc), dLFactor, LoadCaseNo, LcomD, 0);
				if (nCF > 0) { strDesc.Format(_T("+%sCF"), GetFactorString(dLFactor)); LcomD.Description += strDesc; }
			}

			if (m_nLsCount > 0) // Live Load Surcharge µØÃæ¸½¼Ó»îºÉÔØ
			{
				T_LCOM_D LcomD_l = LcomD; LoadCaseNo = GetCountLcomCase(LcomD_l);
				int nLS = 0;
				for (nc = 0; nc < m_nLsCount; nc++) nLS += Set_STLCIncludeCS(arLsLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD_l, 0);
				if (nLS > 0)
				{
					strDesc.Format(_T("+%sLS"), GetFactorString(dFactor)); LcomD_l.Description += strDesc;
					SetCvlJTJD60_15_ETC_S(LcomD_l, LcomD_l, FALSE, arCrlLoad, arMCrlLoad, TRUE, arBrkLoad, FALSE, arWlLoad, FALSE, arSfLoad, FALSE, arIpLoad, FALSE,
						arTLoad, arTpgLoad, FALSE, arFrLoad, FALSE, 10, nMsgType);
				}
			}
		}
	}

	return TRUE;
}

BOOL CLoadCombCtrl::SetCvlJTJD60_15_ETC_S(T_LCOM_D rData, T_LCOM_D Lcom_cf, BOOL bCF, ArINT& arCrlLoad, ArINT& arMCrlLoad, BOOL bCrl, ArINT& arBrkLoad, BOOL bBrk,
	ArINT& arWlLoad, BOOL bWl, ArINT& arSfLoad, BOOL bSf, ArINT& arIpLoad, BOOL bIp, ArINT& arTLoad,
	ArINT& arTpgLoad, BOOL bT, ArINT& arFrLoad, BOOL bFr, int nType, int nMsgType)
{
	double dFactor = 1.0, dWlFactor = 1.0, dCRLFactor = 1.0, dTPGFactor = 1.0;
	if (nMsgType == 117) { dWlFactor = 1.0, dTPGFactor = 0.8, dCRLFactor = 0.4; }     // ×¼ÓÀ¾Ã
	else if (nMsgType == 118) { dWlFactor = 1.0, dTPGFactor = 0.8, dCRLFactor = 0.4; }// ÆµÓö
	else if (nMsgType == 200) { dWlFactor = 1.0, dTPGFactor = 1.0, dCRLFactor = 1.0; }//»ù´¡ÑéËã-ÆµÓö

	int LoadCaseNo = 0, nc = 0, nnn = 0;
	CString strDesc = _T("");

	BOOL bErec = FALSE;
	for (int nErec = 0; nErec < D_STCT_EREC_MAX_CVL; ++nErec)
	{
		if (m_nConstLoad != 0 && CheckCS_SgldK(D_SGLD_ER1_CVL + nErec, nnn)) bErec = TRUE;
	}

	int nServEnvType = (nMsgType == 119) ? 1 : 0;//±ê×¼
	BOOL bAddCsLoad = bErec;
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER1_CVL,nnn)) || 
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER2_CVL,nnn)) || 
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER3_CVL,nnn)) ||
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER4_CVL,nnn)) ||
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER5_CVL,nnn)) ||
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER6_CVL,nnn)) ||
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER7_CVL,nnn)) ||
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER8_CVL,nnn)) ||
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER9_CVL,nnn)) ||
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER10_CVL,nnn));// CS or ST+CS;

	if ((m_nCrlCount > 0 || m_nMCrlCount > 0) && bCrl)
	{
		T_LCOM_D LcomD = rData;
		LoadCaseNo = GetCountLcomCase(LcomD);
		int nCRL = 0;
		for (nc = 0; nc < m_nCrlCount; nc++) nCRL += Set_STLCIncludeCS(arCrlLoad.GetAt(nc), dCRLFactor, LoadCaseNo, LcomD, 0);
		T_LCOM_D LcomD_tmp = Lcom_cf;
		if (bCF && bBrk)
		{
			LoadCaseNo = GetCountLcomCase(LcomD_tmp);
			for (nc = 0; nc < m_nCrlCount; nc++) Set_STLCIncludeCS(arCrlLoad.GetAt(nc), dCRLFactor, LoadCaseNo, LcomD_tmp, 0);
		}
		if (nCRL > 0)
		{
			strDesc.Format(_T("+%sCRL"), GetFactorString(dCRLFactor)); LcomD.Description += strDesc;
			if (bCF && bBrk) LcomD_tmp.Description += strDesc;
		}
		int nMCrlCount = (m_nMCrlCount > 0) ? m_nMCrlCount : ((nCRL > 0) ? 1 : 0);
		for (int crl = 0; crl < nMCrlCount; ++crl)
		{
			T_LCOM_D LcomD_crl = LcomD;
			T_LCOM_D LcomD_crltmp = LcomD_tmp;
			if (m_nMCrlCount > 0)
			{
				Set_MovingCase(arMCrlLoad.GetAt(crl), dCRLFactor, LoadCaseNo, LcomD_crl, TRUE);
				strDesc.Format(_T("+%sMCRL"), GetFactorString(dCRLFactor)); LcomD_crl.Description += strDesc; LcomD_crltmp.Description += strDesc;
				if (m_nMCrlCount > 1) { strDesc.Format(_T("[%d]"), crl + 1); LcomD_crl.Description += strDesc; LcomD_crltmp.Description += strDesc; }
			}
			if (nCRL > 0 || m_nMCrlCount > 0)
			{
				if (nType == 1) SetCvlJTJD60_15_ETC_S(LcomD_crl, LcomD_crltmp, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, bBrk, arWlLoad, TRUE, arSfLoad, TRUE, arIpLoad, TRUE,
					arTLoad, arTpgLoad, TRUE, arFrLoad, TRUE, 0, nMsgType);
				if (nType == 6) SetCvlJTJD60_15_ETC_S(LcomD_crl, LcomD_crltmp, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, bBrk, arWlLoad, TRUE, arSfLoad, TRUE, arIpLoad, TRUE,
					arTLoad, arTpgLoad, TRUE, arFrLoad, FALSE, 2, nMsgType);
				if (nType == 8) SetCvlJTJD60_15_ETC_S(LcomD_crl, LcomD_crltmp, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, bBrk, arWlLoad, TRUE, arSfLoad, TRUE, arIpLoad, TRUE,
					arTLoad, arTpgLoad, FALSE, arFrLoad, FALSE, 6, nMsgType);
				if (nType == 10) SetCvlJTJD60_15_ETC_S(LcomD_crl, LcomD_crltmp, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWlLoad, TRUE, arSfLoad, FALSE, arIpLoad, FALSE,
					arTLoad, arTpgLoad, FALSE, arFrLoad, FALSE, 8, nMsgType);
				if (nType == 101) SetCvlJTJD60_15_ETC_S(LcomD_crl, LcomD_crltmp, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, bBrk, arWlLoad, FALSE, arSfLoad, FALSE, arIpLoad, FALSE,
					arTLoad, arTpgLoad, FALSE, arFrLoad, FALSE, 102, nMsgType);
				if (nType == 103) SetCvlJTJD60_15_ETC_S(LcomD_crl, LcomD_crltmp, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, bBrk, arWlLoad, TRUE, arSfLoad, FALSE, arIpLoad, FALSE,
					arTLoad, arTpgLoad, FALSE, arFrLoad, FALSE, 104, nMsgType);

				if (nType == 0) Set_LoadComb4Civil(LcomD_crl, D_SER, nServEnvType, nMsgType);
			}
		}
	}
	if (nType == 8) bBrk = FALSE;
	if (m_nBrkCount > 0 && bBrk)
	{
		for (int i = 0; i < 2; ++i)
		{
			if (i == 1 && !bCF) break;
			T_LCOM_D LcomD = (i == 0) ? rData : Lcom_cf;
			LoadCaseNo = GetCountLcomCase(LcomD);
			double dBrkF = (i == 0) ? (bCF ? 0.7 : 1.0) : 1.0;
			int nBRK = 0;
			for (nc = 0; nc < m_nBrkCount; nc++) nBRK += Set_STLCIncludeCS(arBrkLoad.GetAt(nc), dBrkF, LoadCaseNo, LcomD, 0);
			if (nBRK > 0) { strDesc.Format(_T("+%sBRK"), GetFactorString(dBrkF)); LcomD.Description += strDesc; }
			if (nType == 1) SetCvlJTJD60_15_ETC_S(LcomD, Lcom_cf, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWlLoad, TRUE, arSfLoad, FALSE, arIpLoad, FALSE,
				arTLoad, arTpgLoad, TRUE, arFrLoad, FALSE, 0, nMsgType);
			if (nType == 2) SetCvlJTJD60_15_ETC_S(LcomD, Lcom_cf, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWlLoad, TRUE, arSfLoad, FALSE, arIpLoad, FALSE,
				arTLoad, arTpgLoad, TRUE, arFrLoad, FALSE, 0, nMsgType);
			if (nType == 6) SetCvlJTJD60_15_ETC_S(LcomD, Lcom_cf, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWlLoad, TRUE, arSfLoad, FALSE, arIpLoad, FALSE,
				arTLoad, arTpgLoad, FALSE, arFrLoad, FALSE, 3, nMsgType);
			if (nType == 7) SetCvlJTJD60_15_ETC_S(LcomD, Lcom_cf, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWlLoad, TRUE, arSfLoad, FALSE, arIpLoad, FALSE,
				arTLoad, arTpgLoad, FALSE, arFrLoad, FALSE, 3, nMsgType);
			if (nType == 8) SetCvlJTJD60_15_ETC_S(LcomD, Lcom_cf, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWlLoad, TRUE, arSfLoad, FALSE, arIpLoad, FALSE,
				arTLoad, arTpgLoad, FALSE, arFrLoad, FALSE, 3, nMsgType);
			if (nType == 101) SetCvlJTJD60_15_ETC_S(LcomD, Lcom_cf, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWlLoad, TRUE, arSfLoad, FALSE, arIpLoad, FALSE,
				arTLoad, arTpgLoad, FALSE, arFrLoad, FALSE, 3, nMsgType);
			if (nType == 102) SetCvlJTJD60_15_ETC_S(LcomD, Lcom_cf, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWlLoad, TRUE, arSfLoad, FALSE, arIpLoad, FALSE,
				arTLoad, arTpgLoad, TRUE, arFrLoad, FALSE, 0, nMsgType);
			if (nType == 103) SetCvlJTJD60_15_ETC_S(LcomD, Lcom_cf, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWlLoad, TRUE, arSfLoad, FALSE, arIpLoad, FALSE,
				arTLoad, arTpgLoad, FALSE, arFrLoad, FALSE, 3, nMsgType);
			if (nType == 104) SetCvlJTJD60_15_ETC_S(LcomD, Lcom_cf, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWlLoad, TRUE, arSfLoad, FALSE, arIpLoad, FALSE,
				arTLoad, arTpgLoad, FALSE, arFrLoad, FALSE, 3, nMsgType);

			if (nType == 0) Set_LoadComb4Civil(LcomD, D_SER, nServEnvType, nMsgType);
		}
	}
	if (bWl)
	{
		int nWlCount = m_nWlCount * 2; // MNET:1614, KJH20050808
		if (m_nWlCount == 0 && bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(WL))) nWlCount = 1;
		for (nc = 0; nc < nWlCount; nc++)
		{
			T_LCOM_D LcomD = rData;
			LoadCaseNo = GetCountLcomCase(LcomD);
			int nWL = 0;
			if (m_nWlCount > 0)
			{
				double dWF1 = (nc % 2 == 0) ? dWlFactor : -1.*dWlFactor;
				if (Set_STLCIncludeCS(arWlLoad.GetAt(nc / 2), dWF1, LoadCaseNo, LcomD, 0) > 0)
				{
					CString strW = (dWF1 > 0.0) ? _T("+%sWL") : _T("%sWL");
					strDesc.Format(strW, GetFactorString(dWF1)); LcomD.Description += strDesc;
					if (m_nWlCount > 1) { strDesc.Format(_T("[%d]"), nc / 2 + 1); LcomD.Description += strDesc; }
				}
			}
			if (bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(WL)))
			{
				Set_CStageCase(m_aCSLoad.GetAt(nnn), dWlFactor, LoadCaseNo, LcomD, TRUE);
				strDesc.Format(_T("+%s(cEL)"), GetFactorString(dWlFactor)); LcomD.Description += strDesc;
			}
			if (nType == 1) SetCvlJTJD60_15_ETC_S(LcomD, Lcom_cf, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWlLoad, FALSE, arSfLoad, TRUE, arIpLoad, TRUE,
				arTLoad, arTpgLoad, TRUE, arFrLoad, TRUE, 0, nMsgType);
			if (nType == 2) SetCvlJTJD60_15_ETC_S(LcomD, Lcom_cf, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWlLoad, FALSE, arSfLoad, TRUE, arIpLoad, TRUE,
				arTLoad, arTpgLoad, TRUE, arFrLoad, TRUE, 0, nMsgType);
			if (nType == 3) SetCvlJTJD60_15_ETC_S(LcomD, Lcom_cf, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWlLoad, FALSE, arSfLoad, FALSE, arIpLoad, FALSE,
				arTLoad, arTpgLoad, TRUE, arFrLoad, FALSE, 0, nMsgType);
			if (nType == 6) SetCvlJTJD60_15_ETC_S(LcomD, Lcom_cf, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWlLoad, FALSE, arSfLoad, TRUE, arIpLoad, TRUE,
				arTLoad, arTpgLoad, TRUE, arFrLoad, FALSE, 2, nMsgType);
			if (nType == 7) SetCvlJTJD60_15_ETC_S(LcomD, Lcom_cf, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWlLoad, FALSE, arSfLoad, TRUE, arIpLoad, TRUE,
				arTLoad, arTpgLoad, TRUE, arFrLoad, FALSE, 2, nMsgType);
			if (nType == 8) SetCvlJTJD60_15_ETC_S(LcomD, Lcom_cf, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWlLoad, FALSE, arSfLoad, TRUE, arIpLoad, TRUE,
				arTLoad, arTpgLoad, FALSE, arFrLoad, FALSE, 6, nMsgType);
			if (nType == 9) SetCvlJTJD60_15_ETC_S(LcomD, Lcom_cf, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWlLoad, FALSE, arSfLoad, TRUE, arIpLoad, TRUE,
				arTLoad, arTpgLoad, FALSE, arFrLoad, FALSE, 6, nMsgType);

			if (nType == 0) Set_LoadComb4Civil(LcomD, D_SER, nServEnvType, nMsgType);
		}
	}
	if (m_nSfCount > 0 && bSf)
	{
		T_LCOM_D LcomD = rData;
		LoadCaseNo = GetCountLcomCase(LcomD);
		int nSF = 0;
		for (nc = 0; nc < m_nSfCount; nc++) nSF += Set_STLCIncludeCS(arSfLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD, 0);
		if (nSF > 0) { strDesc.Format(_T("+%sSF"), GetFactorString(dFactor)); LcomD.Description += strDesc; }
		if (nType == 1) SetCvlJTJD60_15_ETC_S(LcomD, Lcom_cf, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWlLoad, FALSE, arSfLoad, FALSE, arIpLoad, FALSE,
			arTLoad, arTpgLoad, TRUE, arFrLoad, TRUE, 0, nMsgType);
		if (nType == 2) SetCvlJTJD60_15_ETC_S(LcomD, Lcom_cf, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWlLoad, FALSE, arSfLoad, FALSE, arIpLoad, FALSE,
			arTLoad, arTpgLoad, TRUE, arFrLoad, TRUE, 0, nMsgType);
		if (nType == 4) SetCvlJTJD60_15_ETC_S(LcomD, Lcom_cf, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWlLoad, FALSE, arSfLoad, FALSE, arIpLoad, FALSE,
			arTLoad, arTpgLoad, TRUE, arFrLoad, TRUE, 0, nMsgType);
		if (nType == 6) SetCvlJTJD60_15_ETC_S(LcomD, Lcom_cf, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWlLoad, FALSE, arSfLoad, FALSE, arIpLoad, FALSE,
			arTLoad, arTpgLoad, TRUE, arFrLoad, FALSE, 2, nMsgType);
		if (nType == 7) SetCvlJTJD60_15_ETC_S(LcomD, Lcom_cf, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWlLoad, FALSE, arSfLoad, FALSE, arIpLoad, FALSE,
			arTLoad, arTpgLoad, TRUE, arFrLoad, FALSE, 2, nMsgType);

		if (nType == 0) Set_LoadComb4Civil(LcomD, D_SER, nServEnvType, nMsgType);
	}
	if (m_nIpCount > 0 && bIp)
	{
		T_LCOM_D LcomD = rData;
		LoadCaseNo = GetCountLcomCase(LcomD);
		int nIP = 0;
		for (nc = 0; nc < m_nIpCount; nc++) nIP += Set_STLCIncludeCS(arIpLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD, 0);
		if (nIP > 0) { strDesc.Format(_T("+%sIP"), GetFactorString(dFactor)); LcomD.Description += strDesc; }
		if (nType == 1) SetCvlJTJD60_15_ETC_S(LcomD, Lcom_cf, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWlLoad, FALSE, arSfLoad, FALSE, arIpLoad, FALSE,
			arTLoad, arTpgLoad, TRUE, arFrLoad, TRUE, 0, nMsgType);
		if (nType == 2) SetCvlJTJD60_15_ETC_S(LcomD, Lcom_cf, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWlLoad, FALSE, arSfLoad, FALSE, arIpLoad, FALSE,
			arTLoad, arTpgLoad, TRUE, arFrLoad, TRUE, 0, nMsgType);
		if (nType == 4) SetCvlJTJD60_15_ETC_S(LcomD, Lcom_cf, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWlLoad, FALSE, arSfLoad, FALSE, arIpLoad, FALSE,
			arTLoad, arTpgLoad, TRUE, arFrLoad, TRUE, 0, nMsgType);
		if (nType == 6) SetCvlJTJD60_15_ETC_S(LcomD, Lcom_cf, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWlLoad, FALSE, arSfLoad, FALSE, arIpLoad, FALSE,
			arTLoad, arTpgLoad, TRUE, arFrLoad, FALSE, 2, nMsgType);
		if (nType == 7) SetCvlJTJD60_15_ETC_S(LcomD, Lcom_cf, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWlLoad, FALSE, arSfLoad, FALSE, arIpLoad, FALSE,
			arTLoad, arTpgLoad, TRUE, arFrLoad, FALSE, 2, nMsgType);

		if (nType == 0) Set_LoadComb4Civil(LcomD, D_SER, nServEnvType, nMsgType);
	}
	if (bT && ((m_nTCount > 0 || m_nTpgCount > 0) || (bAddCsLoad && (CheckCSEL(D_SGLD_ER1_CVL, _LSX(T)) || CheckCSEL(D_SGLD_ER1_CVL, _LSX(TPG))))))
	{
		T_LCOM_D LcomD = rData;
		if (nType == 0) SetCvlJTJD60_Capacity_TPG(LcomD, arTLoad, arTpgLoad, arFrLoad, dFactor, dTPGFactor, FALSE, FALSE, nMsgType, D_SER, nServEnvType);
		else         SetCvlJTJD60_Capacity_TPG(LcomD, arTLoad, arTpgLoad, arFrLoad, dFactor, dTPGFactor, TRUE, FALSE, nMsgType, D_SER, nServEnvType);
	}
	if (m_nFrCount > 0 && bFr)
	{
		T_LCOM_D LcomD = rData;
		LoadCaseNo = GetCountLcomCase(LcomD);
		int nFR = 0;
		for (nc = 0; nc < m_nFrCount; nc++) nFR += Set_STLCIncludeCS(arFrLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD, 0);
		if (nFR > 0)
		{
			strDesc.Format(_T("+%sFR"), GetFactorString(dFactor)); LcomD.Description += strDesc;
			if (nType == 0) Set_LoadComb4Civil(LcomD, D_SER, nServEnvType, nMsgType);
		}
	}
	return TRUE;
}

BOOL CLoadCombCtrl::SetCvlJTJD60_15_Var1_A(const CArray<T_LCOM_D, T_LCOM_D&>& arData, ArINT& arLLoad,
	ArINT& arIlLoad, ArINT& arCfLoad, ArINT& arLsLoad, ArINT& arCrlLoad, ArINT& arWLoad,
	ArINT& arSfLoad, ArINT& arIpLoad, ArINT& arTLoad, ArINT& arTpgLoad, ArINT& arFrLoad,
	ArINT& arBrkLoad, ArUNT& arMLoad, ArINT& arMcrLoad, int nType, int nMsgType, int nAcdType)
{
	// nType == 0,4: °è¼öÁ¶ÇÕ, 1: 1.0(settlement Á¦¿Ü), 2: 1.0(¸ðµÎ), Live Load Á¦¿Ü
	CArray<T_LCOM_D, T_LCOM_D&> aLcomD; aLcomD.Copy(arData);
	int  nLcomSize = aLcomD.GetSize();
	int  nnn = 0;
	BOOL bAddCsLoad = m_nConstLoad != 0 && CheckCS_SgldK(D_SGLD_ER1_CVL, nnn); // CS or ST+CS

	double dFactor = 1.0;
	double dMLFactor = 1.0;       // Æû³µºÉÔØÏµÊý
	double dCrlFactor = 1.0;      // ÈËÈººÉÔØÏµÊý
	double dWFactor = 0.75;       // ·çºÉÔØÏµÊý
	double dTgpFactor = 0.8;      //ÎÂ¶ÈÌÝ¶ÈºÉÔØÏµÊý
	if (nAcdType == 0)//ÆµÓöÏµÊý modified this statement by gongxing at 2016-04-13
	{
		dMLFactor = 0.7;
		dCrlFactor = 1.0;
	}
	else if (nAcdType == 1)//×¼ÓÀ¾ÃÏµÊý
	{
		dMLFactor = 0.4;
		dCrlFactor = 0.4;
	}
	else if (nAcdType == 100)//»ù´¡ÑéËã-Å¼È»×éºÏ
	{
		dMLFactor  = 1.0;
		dCrlFactor = 1.0;
		dWFactor   = 1.0;
	}
	int nActive = m_nLcomType == D_LCOMTYPE_GENERAL ? 1 : 6;
	double dIL = (nType == 12) ? 0. : CalcMovingILFactor();
	if (nAcdType == 100)//»ù´¡ÑéËã-Å¼È»×éºÏ
	{
		dIL = 0.0;
	}
	// ¿µ±¸ÇÏÁß + Â÷·®ÇÏÁß + 1°³ÀÇ °¡º¯ ÇÏÁßÀÎ °æ¿ì
	CString strDesc = _T("");
	int LoadCaseNo = 0, nc = 0;
	for (int lc = 0; lc < nLcomSize; lc++)
	{
		T_LCOM_D LcomD = aLcomD.GetAt(lc);
		LcomD.nActive = nActive;
		LoadCaseNo = GetCountLcomCase(LcomD);

		int nMCount = max(m_nMCount + 1, 1);
		for (int m = 0; m < nMCount; m++)
		{
			if (m < m_nMCount)
			{

				Set_MovingCase(arMLoad.GetAt(m), dMLFactor, LoadCaseNo, LcomD, TRUE);  //????
				strDesc.Format(_T("+%sM"), GetFactorString(dMLFactor)); LcomD.Description += strDesc;
				if (m_nMCount > 1) { strDesc.Format(_T("[%d]"), m + 1); LcomD.Description += strDesc; }
				if (nType == 2 || nType == 4) m_aAcdnLcomD.Add(LcomD);
				else                     Set_LoadComb4Civil(LcomD, D_STR, 0, nMsgType);
			}
			int nMcrCount = max(m_nMCrlCount + 1, 1);
			for (int nMcr = 0; nMcr < nMcrCount; ++nMcr)
			{
				if (m < m_nMCount && nMcr < m_nMCrlCount)
				{
					Set_MovingCase(arMcrLoad.GetAt(nMcr), dCrlFactor, LoadCaseNo, LcomD, TRUE);
					strDesc.Format(_T("+%sMcr"), GetFactorString(dCrlFactor));LcomD.Description += strDesc;
					if (m_nMCrlCount > 1) { strDesc.Format(_T("[%d]"), nMcr + 1); LcomD.Description += strDesc; }
					if (nType == 2 || nType == 4) m_aAcdnLcomD.Add(LcomD);
					else                     Set_LoadComb4Civil(LcomD, D_STR, 0, nMsgType);
				}

				BOOL bCF = FALSE;
				T_LCOM_D LcomD_cf = LcomD; // MNET:1614, KJH20050809, CF°¡ Á¦¿ÜµÈ ÇÏÁßÁ¶ÇÕ
				if (nType != 2)
				{
					BOOL bAddLcomD = FALSE;
					if (m_nLCount > 0 || (bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(L)))) // ST or ST+CS
					{
						int nLL = 0, nIL = 0, nCF = 0;
						for (nc = 0; nc < m_nLCount; nc++) nLL += Set_STLCIncludeCS(arLLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD, 0);
						if (nLL > 0) { strDesc.Format(_T("+%sL"), GetFactorString(dFactor)); LcomD_cf.Description += strDesc; }
						if (bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(L)))
						{
							Set_CStageCase(m_aCSLoad.GetAt(nnn), dFactor, LoadCaseNo, LcomD, TRUE);
							strDesc.Format(_T("+%s(cEL)"), GetFactorString(dFactor)); LcomD_cf.Description += strDesc;
							bAddLcomD = TRUE;
						}
						for (nc = 0; nc < m_nIlCount; nc++) nIL += Set_STLCIncludeCS(arIlLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD, 0);
						if (nIL > 0) { strDesc.Format(_T("+%sIL"), GetFactorString(dFactor));  LcomD_cf.Description += strDesc; }

						LcomD_cf = LcomD;
						for (nc = 0; nc < m_nCfCount; nc++) nCF += Set_STLCIncludeCS(arCfLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD, 0);
						if (nCF > 0) { strDesc.Format(_T("+%sCF"), GetFactorString(dFactor));  LcomD_cf.Description += strDesc; bCF = TRUE; }
						if (nLL + nIL + nCF > 0) bAddLcomD = TRUE;
					}
					if (bAddLcomD)
					{
						if (nType == 2 || nType == 4) m_aAcdnLcomD.Add(LcomD);
						else                     Set_LoadComb4Civil(LcomD, D_STR, 0, nMsgType);
					}
				}

				if (m_nLsCount > 0) // Live Load Surcharge
				{
					T_LCOM_D LcomD_l = LcomD; LoadCaseNo = GetCountLcomCase(LcomD_l);
					int nLS = 0;
					for (nc = 0; nc < m_nLsCount; nc++) nLS += Set_STLCIncludeCS(arLsLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD_l, 0);
					if (nLS > 0)
					{
						strDesc.Format(_T("+%sLS"), GetFactorString(dFactor));  LcomD_l.Description += strDesc;
						if (nType == 2 || nType == 4) m_aAcdnLcomD.Add(LcomD_l);
						else                     Set_LoadComb4Civil(LcomD_l, D_STR, 0, nMsgType);
					}
				}
				if (nType != 2 && m_nCrlCount > 0) // Crowd Load
				{
					T_LCOM_D LcomD_l = LcomD; LoadCaseNo = GetCountLcomCase(LcomD_l);
					int nCRL = 0;
					for (nc = 0; nc < m_nCrlCount; nc++) nCRL += Set_STLCIncludeCS(arCrlLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD_l, 0);
					if (nCRL > 0)
					{
						strDesc.Format(_T("+%sCRL"), GetFactorString(dFactor));  LcomD_l.Description += strDesc;
						if (nType == 2 || nType == 4) m_aAcdnLcomD.Add(LcomD_l);
						else                     Set_LoadComb4Civil(LcomD_l, D_STR, 0, nMsgType);
					}
				}
				if (nType != 2 && m_nBrkCount > 0 && m_nLCount > 0) // Brake Load
				{
					T_LCOM_D LcomD_l = LcomD; LoadCaseNo = GetCountLcomCase(LcomD_l);
					double dBrkF = (nType == 0 || nType == 4) ? dFactor * 0.7 : 1.0;
					int nBRK = 0;
					for (nc = 0; nc < m_nBrkCount; nc++) nBRK += Set_STLCIncludeCS(arBrkLoad.GetAt(nc), dBrkF, LoadCaseNo, LcomD_l, 0);
					if (nBRK > 0)
					{
						strDesc.Format(_T("+%sBRK"), GetFactorString(dBrkF));  LcomD_l.Description += strDesc;
						if (nType == 2 || nType == 4) m_aAcdnLcomD.Add(LcomD_l);
						else                     Set_LoadComb4Civil(LcomD_l, D_STR, 0, nMsgType);
					}
					if (bCF)
					{
						LcomD_l = LcomD_cf; LoadCaseNo = GetCountLcomCase(LcomD_l);
						double dBrkF = (nType == 0 || nType == 4) ? dFactor : 1.0;
						nBRK = 0;
						for (nc = 0; nc < m_nBrkCount; nc++) nBRK += Set_STLCIncludeCS(arBrkLoad.GetAt(nc), dBrkF, LoadCaseNo, LcomD_l, 0);
						if (nBRK > 0)
						{
							strDesc.Format(_T("+%sBRK"), GetFactorString(dBrkF));  LcomD_l.Description += strDesc;
							if (nType == 2 || nType == 4) m_aAcdnLcomD.Add(LcomD_l);
							else                     Set_LoadComb4Civil(LcomD_l, D_STR, 0, nMsgType);
						}
					}
				}
				int nWCount = m_nWCount * 2; // MNET:1614, KJH20050808
				if (m_nWCount == 0 && bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(W))) nWCount = 1;
				for (nc = 0; nc < nWCount; nc++)
				{
					T_LCOM_D LcomD_l = LcomD; LoadCaseNo = GetCountLcomCase(LcomD_l);
					if (m_nWCount > 0)
					{
						double dWF = (nc % 2 == 0) ? dWFactor : -1.*dWFactor;
						if (Set_STLCIncludeCS(arWLoad.GetAt(nc / 2), dWF, LoadCaseNo, LcomD_l, 0) > 0)
						{
							CString strW = (dWF > 0.0) ? _T("+%sW") : _T("%sW");
							strDesc.Format(strW, GetFactorString(dWF)); LcomD_l.Description += strDesc;
							if (m_nWCount > 1) { strDesc.Format(_T("[%d]"), nc / 2 + 1);  LcomD_l.Description += strDesc; }
						}
					}
					if (bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(W)))
					{
						Set_CStageCase(m_aCSLoad.GetAt(nnn), dWFactor, LoadCaseNo, LcomD_l, TRUE);
						strDesc.Format(_T("+%s(cEL)"), GetFactorString(dWFactor)); LcomD_l.Description += strDesc;
					}
					if (nType == 2 || nType == 4) m_aAcdnLcomD.Add(LcomD_l);
					else                     Set_LoadComb4Civil(LcomD_l, D_STR, 0, nMsgType);
				}
				if (m_nSfCount > 0) // Stream Flow Pressure
				{
					T_LCOM_D LcomD_l = LcomD; LoadCaseNo = GetCountLcomCase(LcomD_l);
					int nSF = 0;
					for (nc = 0; nc < m_nSfCount; nc++) nSF += Set_STLCIncludeCS(arSfLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD_l, 0);
					if (nSF > 0)
					{
						strDesc.Format(_T("+%sSF"), GetFactorString(dFactor));  LcomD_l.Description += strDesc;
						if (nType == 2 || nType == 4) m_aAcdnLcomD.Add(LcomD_l);
						else                     Set_LoadComb4Civil(LcomD_l, D_STR, 0, nMsgType);
					}
				}
				if (m_nIpCount > 0) // Ice Pressure
				{
					T_LCOM_D LcomD_l = LcomD; LoadCaseNo = GetCountLcomCase(LcomD_l);
					int nIP = 0;
					for (nc = 0; nc < m_nIpCount; nc++) nIP += Set_STLCIncludeCS(arIpLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD_l, 0);
					if (nIP > 0)
					{
						strDesc.Format(_T("+%sIP"), GetFactorString(dFactor)); LcomD_l.Description += strDesc;
						if (nType == 2 || nType == 4) m_aAcdnLcomD.Add(LcomD_l);
						else                     Set_LoadComb4Civil(LcomD_l, D_STR, 0, nMsgType);
					}
				}
				// Temeperature
				if ((m_nTCount > 0 || m_nTpgCount > 0) || (bAddCsLoad && (CheckCSEL(D_SGLD_ER1_CVL, _LSX(T)) || CheckCSEL(D_SGLD_ER1_CVL, _LSX(TPG)))))
				{
					T_LCOM_D LcomD_l = LcomD;
					SetCvlJTJD60_Capacity_TPG(LcomD_l, arTLoad, arTpgLoad, arFrLoad, dFactor, dTgpFactor, FALSE, nType == 2 || nType == 4, nMsgType);
				}
				if (m_nFrCount > 0) // Friction
				{
					T_LCOM_D LcomD_l = LcomD; LoadCaseNo = GetCountLcomCase(LcomD_l);
					int nFR = 0;
					for (nc = 0; nc < m_nFrCount; nc++) nFR += Set_STLCIncludeCS(arFrLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD_l, 0);
					if (nFR > 0)
					{
						strDesc.Format(_T("+%sFR"), GetFactorString(dFactor));  LcomD_l.Description += strDesc;
						if (nType == 2 || nType == 4) m_aAcdnLcomD.Add(LcomD_l);
						else                     Set_LoadComb4Civil(LcomD_l, D_STR, 0, nMsgType);
					}
				}
			}
		}
	}

	return TRUE;
}
BOOL CLoadCombCtrl::SetCvlJTJD60_15_Var2_A(const CArray<T_LCOM_D, T_LCOM_D&>& arData, ArINT& arLLoad,
	ArINT& arIlLoad, ArINT& arCfLoad, ArINT& arLsLoad, ArINT& arCrlLoad, ArINT& arWLoad,
	ArINT& arSfLoad, ArINT& arIpLoad, ArINT& arTLoad, ArINT& arTpgLoad, ArINT& arFrLoad,
	ArINT& arBrkLoad, ArUNT& arMLoad, ArINT& arMcrLoad, int nType, int nMsgType, int nAcdType)
{
	// nType == 0,4: °è¼öÁ¶ÇÕ, 1: 1.0(settlement Á¦¿Ü), 2: 1.0(¸ðµÎ), Live Load Á¦¿Ü
	CArray<T_LCOM_D, T_LCOM_D&>aLcomD; aLcomD.Copy(arData);
	int  nLcomSize = aLcomD.GetSize();
	int  nnn = 0;

	BOOL bAddCsLoad = m_nConstLoad != 0 && CheckCS_SgldK(D_SGLD_ER1_CVL, nnn); // CS or ST+CS

	double dFactor = 1.0;
	double dMLFactor = 1.0;       // Æû³µºÉÔØÏµÊý
	double dCrlFactor = 1.0;      // ÈËÈººÉÔØÏµÊý
	double dWFactor = 0.75;       // ·çºÉÔØÏµÊý
	double dTgpFactor = 0.8;      //ÎÂ¶ÈÌÝ¶ÈºÉÔØÏµÊý
	if (nAcdType == 0)//ÆµÓöÏµÊý modified this statement by gongxing at 2016-04-13
	{
		dMLFactor = 0.7;
		dCrlFactor = 1.0;
	}
	else if (nAcdType == 1)//×¼ÓÀ¾ÃÏµÊý
	{
		dMLFactor = 0.4;
		dCrlFactor = 0.4;
	}
	else if (nAcdType == 100)//»ù´¡ÑéËã-Å¼È»×éºÏ
	{
		dMLFactor = 1.0;
		dCrlFactor = 1.0;
		dWFactor = 1.0;
	}
	int nActive = m_nLcomType == D_LCOMTYPE_GENERAL ? 1 : 6;
	double dF = (nType == 0 || nType == 4) ? 0.75 : 1.0;
	double dIL = (nType == 12) ? 0. : CalcMovingILFactor();
	if (nAcdType == 100)//»ù´¡ÑéËã-Å¼È»×éºÏ
	{
		dIL = 0.0;
	}
	// ¿µ±¸ÇÏÁß + Â÷·®ÇÏÁß + 2°³ÀÇ °¡º¯ÇÏÁßÀÎ °æ¿ì
	CString strDesc = _T("");
	int LoadCaseNo = 0, nc = 0;
	for (int lc = 0; lc < nLcomSize; lc++)
	{
		T_LCOM_D LcomD = aLcomD.GetAt(lc);
		LcomD.nActive = nActive;
		LoadCaseNo = GetCountLcomCase(LcomD);

		int nMCount = max(m_nMCount + 1, 1);
		for (int m = 0; m < nMCount; m++)
		{
			if (m < m_nMCount)
			{

				Set_MovingCase(arMLoad.GetAt(m), dMLFactor, LoadCaseNo, LcomD, TRUE);  //????
				strDesc.Format(_T("+%sM"), GetFactorString(dMLFactor)); LcomD.Description += strDesc;
				if (m_nMCount > 1) { strDesc.Format(_T("[%d]"), m + 1); LcomD.Description += strDesc; }
				if (nType == 2 || nType == 4) m_aAcdnLcomD.Add(LcomD);
				else                     Set_LoadComb4Civil(LcomD, D_STR, 0, nMsgType);
			}

			int nMcrCount = max(m_nMCrlCount + 1, 1);
			for (int nMcr = 0; nMcr < nMcrCount; ++nMcr)
			{
				if (m < m_nMCount && nMcr < m_nMCrlCount)
				{
					Set_MovingCase(arMcrLoad.GetAt(nMcr), dCrlFactor, LoadCaseNo, LcomD, TRUE);
					strDesc.Format(_T("+%sMcr"), GetFactorString(dCrlFactor));LcomD.Description += strDesc;;
					if (m_nMCrlCount > 1) { strDesc.Format(_T("[%d]"), nMcr + 1); LcomD.Description += strDesc; }
					if (nType == 2 || nType == 4) m_aAcdnLcomD.Add(LcomD);
					else                     Set_LoadComb4Civil(LcomD, D_STR, 0, nMsgType);
				}

				T_LCOM_D LcomD_cf = LcomD; // MNET:1614, KJH20050809, CF°¡ Á¦¿ÜµÈ ÇÏÁßÁ¶ÇÕ
				BOOL bLive = FALSE, bCF = FALSE;
				if (nType != 2)
				{
					bLive = m_nLCount > 0 || (bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(L)));
					if (bLive) // ST or ST+CS
					{
						int nLL = 0, nIL = 0, nCF = 0;
						for (nc = 0; nc < m_nLCount; nc++) nLL += Set_STLCIncludeCS(arLLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD, 0);
						if (nLL > 0) { strDesc.Format(_T("+%sL"), GetFactorString(dFactor)); LcomD.Description += strDesc; }
						if (bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(L)))
						{
							Set_CStageCase(m_aCSLoad.GetAt(nnn), dFactor, LoadCaseNo, LcomD, TRUE);
							strDesc.Format(_T("+%s(cEL)"), GetFactorString(dFactor)); LcomD.Description += strDesc;;
						}
						for (nc = 0; nc < m_nIlCount; nc++) nIL += Set_STLCIncludeCS(arIlLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD, 0);
						if (nIL > 0) { strDesc.Format(_T("+%sIL"), GetFactorString(dFactor)); LcomD.Description += strDesc; }

						LcomD_cf = LcomD;
						for (nc = 0; nc < m_nCfCount; nc++) nCF += Set_STLCIncludeCS(arCfLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD, 0);
						if (nCF > 0) { strDesc.Format(_T("+%sCF"), GetFactorString(dFactor)); LcomD.Description += strDesc; bCF = TRUE; }
					}
				}

				if (m_nLsCount > 0) // Live Load Surcharge
				{
					T_LCOM_D LcomD_l = LcomD; LoadCaseNo = GetCountLcomCase(LcomD_l);
					int nLS = 0;
					for (nc = 0; nc < m_nLsCount; nc++) nLS += Set_STLCIncludeCS(arLsLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD_l, 0);
					T_LCOM_D LcomD_tmp = LcomD_cf;
					if (bCF)
					{
						LoadCaseNo = GetCountLcomCase(LcomD_tmp);
						for (nc = 0; nc < m_nLsCount; nc++) Set_STLCIncludeCS(arLsLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD_tmp, 0);
					}
					if (nLS > 0)
					{
						strDesc.Format(_T("+%sLS"), GetFactorString(dFactor)); LcomD_l.Description += strDesc;
						if (bCF) LcomD_tmp.Description += strDesc;
						ArINT artmp;
						SetCvlJTJD60_15_ETC(LcomD_l, LcomD_tmp, bCF, arCrlLoad, artmp, nType != 2, arBrkLoad, bLive, arWLoad, TRUE, arSfLoad, TRUE,
							arIpLoad, TRUE, arTLoad, arTpgLoad, TRUE, arFrLoad, TRUE, 0, dF, nType == 2 || nType == 4, nMsgType);
					}
				}
				if (nType != 2 && m_nCrlCount > 0) // Crowd Load
				{
					T_LCOM_D LcomD_l = LcomD; LoadCaseNo = GetCountLcomCase(LcomD_l);
					int nCRL = 0;
					for (nc = 0; nc < m_nCrlCount; nc++) nCRL += Set_STLCIncludeCS(arCrlLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD_l, 0);
					T_LCOM_D LcomD_tmp = LcomD_cf;
					if (bCF)
					{
						LoadCaseNo = GetCountLcomCase(LcomD_tmp);
						for (nc = 0; nc < m_nCrlCount; nc++) Set_STLCIncludeCS(arCrlLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD_tmp, 0);
					}
					if (nCRL > 0)
					{
						strDesc.Format(_T("+%sCRL"), GetFactorString(dFactor)); LcomD_l.Description += strDesc;
						if (bCF) LcomD_tmp.Description += strDesc;
						ArINT artmp;
						SetCvlJTJD60_15_ETC(LcomD_l, LcomD_tmp, bCF, arCrlLoad, artmp, FALSE, arBrkLoad, m_nLCount > 0, arWLoad, TRUE, arSfLoad, TRUE, arIpLoad, TRUE,
							arTLoad, arTpgLoad, TRUE, arFrLoad, TRUE, 0, dF, nType == 2 || nType == 4, nMsgType);
					}
				}
				if (m_nBrkCount > 0 && bLive) // Brake Load
				{
					T_LCOM_D LcomD_l = LcomD; LoadCaseNo = GetCountLcomCase(LcomD_l);
					double dBrkF = (nType == 0 || nType == 4) ? dFactor * 0.7 : 1.0;
					int nBRK = 0;
					for (nc = 0; nc < m_nBrkCount; nc++) nBRK += Set_STLCIncludeCS(arBrkLoad.GetAt(nc), dBrkF, LoadCaseNo, LcomD_l, 0);
					if (nBRK > 0)
					{
						strDesc.Format(_T("+%sBRK"), GetFactorString(dBrkF)); LcomD_l.Description += strDesc;
						ArINT artmp;
						SetCvlJTJD60_15_ETC(LcomD_l, LcomD_cf, FALSE, arCrlLoad, artmp, FALSE, arBrkLoad, FALSE, arWLoad, TRUE, arSfLoad, FALSE, arIpLoad, FALSE,
							arTLoad, arTpgLoad, TRUE, arFrLoad, FALSE, 0, dF, nType == 2 || nType == 4, nMsgType);
					}
					if (bCF) // MNET:1614, KJH20050809, CF°¡ ¾ø´Â ÇÏÁßÁ¶ÇÕ ÇÑ¹ø´õ...
					{
						LcomD_l = LcomD_cf; LoadCaseNo = GetCountLcomCase(LcomD_l);
						dBrkF = (nType == 0 || nType == 4) ? dFactor : 1.0;
						nBRK = 0;
						for (nc = 0; nc < m_nBrkCount; nc++) nBRK += Set_STLCIncludeCS(arBrkLoad.GetAt(nc), dBrkF, LoadCaseNo, LcomD_l, 0);
						if (nBRK > 0)
						{
							strDesc.Format(_T("+%sBRK"), GetFactorString(dBrkF)); LcomD_l.Description += strDesc;
							ArINT artmp;
							SetCvlJTJD60_15_ETC(LcomD_l, LcomD_cf, FALSE, arCrlLoad, artmp, FALSE, arBrkLoad, FALSE, arWLoad, TRUE, arSfLoad, FALSE, arIpLoad, FALSE,
								arTLoad, arTpgLoad, TRUE, arFrLoad, FALSE, 0, dF, nType == 2 || nType == 4, nMsgType);
						}
					}
				}
				int nWCount = m_nWCount * 2; // MNET:1614, KJH20050808
				if (m_nWCount == 0 && bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(W))) nWCount = 1;
				for (nc = 0; nc < nWCount; nc++)
				{
					T_LCOM_D LcomD_l = LcomD; LoadCaseNo = GetCountLcomCase(LcomD_l);
					if (m_nWCount > 0)
					{
						double dWF = (nc % 2 == 0) ? dWFactor : -1.*dWFactor;
						if (Set_STLCIncludeCS(arWLoad.GetAt(nc / 2), dWF, LoadCaseNo, LcomD_l, 0) > 0)
						{
							CString strW = (dWF > 0.0) ? _T("+%sW") : _T("%sW");
							strDesc.Format(strW, GetFactorString(dWF)); LcomD_l.Description += strDesc;
							if (m_nWCount > 1) { strDesc.Format(_T("[%d]"), nc / 2 + 1); LcomD_l.Description += strDesc; }
						}
					}
					if (bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(W)))
					{
						Set_CStageCase(m_aCSLoad.GetAt(nnn), dWFactor, LoadCaseNo, LcomD_l, TRUE);
						strDesc.Format(_T("+%s(cEL)"), GetFactorString(dWFactor)); LcomD_l.Description += strDesc;
					}
					ArINT artmp;
					SetCvlJTJD60_15_ETC(LcomD_l, LcomD_cf, bCF, arCrlLoad, artmp, FALSE, arBrkLoad, FALSE, arWLoad, FALSE, arSfLoad, TRUE, arIpLoad, TRUE,
						arTLoad, arTpgLoad, TRUE, arFrLoad, TRUE, 0, dF, nType == 2 || nType == 4, nMsgType);
				}
				if (m_nSfCount > 0) // Stream Flow Pressure
				{
					T_LCOM_D LcomD_l = LcomD; LoadCaseNo = GetCountLcomCase(LcomD_l);
					int nSF = 0;
					for (nc = 0; nc < m_nSfCount; nc++) nSF += Set_STLCIncludeCS(arSfLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD_l, 0);
					if (nSF > 0)
					{
						ArINT artmp;
						strDesc.Format(_T("+%sSF"), GetFactorString(dFactor)); LcomD_l.Description += strDesc;
						SetCvlJTJD60_15_ETC(LcomD_l, LcomD_cf, bCF, arCrlLoad, artmp, FALSE, arBrkLoad, FALSE, arWLoad, FALSE, arSfLoad, FALSE, arIpLoad, FALSE,
							arTLoad, arTpgLoad, TRUE, arFrLoad, TRUE, 0, dF, nType == 2 || nType == 4, nMsgType);
					}
				}
				if (m_nIpCount > 0) // Ice Pressure
				{
					T_LCOM_D LcomD_l = LcomD; LoadCaseNo = GetCountLcomCase(LcomD_l);
					int nIP = 0;
					for (nc = 0; nc < m_nIpCount; nc++) nIP += Set_STLCIncludeCS(arIpLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD_l, 0);
					if (nIP > 0)
					{
						strDesc.Format(_T("+%sIP"), GetFactorString(dFactor)); LcomD_l.Description += strDesc;
						ArINT artmp;
						SetCvlJTJD60_15_ETC(LcomD_l, LcomD_cf, bCF, arCrlLoad, artmp, FALSE, arBrkLoad, FALSE, arWLoad, FALSE, arSfLoad, FALSE, arIpLoad, FALSE,
							arTLoad, arTpgLoad, TRUE, arFrLoad, TRUE, 0, dF, nType == 2 || nType == 4, nMsgType);
					}
				}
				if ((m_nTCount > 0 || m_nTpgCount > 0) || (bAddCsLoad && (CheckCSEL(D_SGLD_ER1_CVL, _LSX(T)) || CheckCSEL(D_SGLD_ER1_CVL, _LSX(TPG))))) // Temeperature
				{
					T_LCOM_D LcomD_l = LcomD;
					SetCvlJTJD60_Capacity_TPG(LcomD_l, arTLoad, arTpgLoad, arFrLoad, dFactor, dTgpFactor, TRUE, nType == 2 || nType == 4, nMsgType);
				}
			}
		}
	}

	return TRUE;
}
BOOL CLoadCombCtrl::SetCvlJTJD60_15_Var3_A(const CArray<T_LCOM_D, T_LCOM_D&>& arData, ArINT& arLLoad,
	ArINT& arIlLoad, ArINT& arCfLoad, ArINT& arLsLoad, ArINT& arCrlLoad, ArINT& arWLoad,
	ArINT& arSfLoad, ArINT& arIpLoad, ArINT& arTLoad, ArINT& arTpgLoad, ArINT& arFrLoad,
	ArINT& arBrkLoad, ArUNT& arMLoad, ArINT& arMcrLoad, int nType, int nMsgType, int nAcdType)
{
	// nType == 0,4: °è¼öÁ¶ÇÕ, 1: 1.0(settlement Á¦¿Ü), 2: 1.0(¸ðµÎ), Live Load Á¦¿Ü
	CArray<T_LCOM_D, T_LCOM_D&> aLcomD; aLcomD.Copy(arData);
	int nLcomSize = aLcomD.GetSize();
	int  nnn = 0;
	BOOL bAddCsLoad = m_nConstLoad != 0 && CheckCS_SgldK(D_SGLD_ER1_CVL, nnn); // CS or ST+CS

	double dFactor = 1.0;
	double dMLFactor = 1.0;     // ???äü??
	double dCrlFactor = 1.0;    // ???äü??
	double dWFactor = 0.75;      // ??äü??
	double dTgpFactor = 0.8;    // ???äü??
	if (nAcdType == 0)//ÆµÓöÏµÊý modified this statement by gongxing at 2016-04-13
	{
		dMLFactor = 0.7;
		dCrlFactor = 1.0;
	}
	else if (nAcdType == 1)//×¼ÓÀ¾ÃÏµÊý
	{
		dMLFactor = 0.4;
		dCrlFactor = 0.4;
	}
	else if (nAcdType == 100)//»ù´¡ÑéËã-Å¼È»×éºÏ
	{
		dMLFactor = 1.0;
		dCrlFactor = 1.0;
		dWFactor = 1.0;
	}
	int nActive = m_nLcomType == D_LCOMTYPE_GENERAL ? 1 : 6;
	double dF = (nType == 0 || nType == 4) ? 0.75 : 1.0;
	double dIL = (nType == 12) ? 0. : CalcMovingILFactor();
	if (nAcdType == 100)//»ù´¡ÑéËã-Å¼È»×éºÏ
	{
		dIL = 0.0;
	}
	// ¿µ±¸ÇÏÁß + Â÷·®ÇÏÁß + 3°³ÀÇ °¡º¯ÇÏÁßÀÎ °æ¿ì
	CString strDesc = _T("");
	int LoadCaseNo = 0, nc = 0;
	for (int lc = 0; lc < nLcomSize; lc++)
	{
		T_LCOM_D LcomD = aLcomD.GetAt(lc);
		LcomD.nActive = nActive;
		LoadCaseNo = GetCountLcomCase(LcomD);

		int nMCount = max(m_nMCount + 1, 1);
		for (int m = 0; m < nMCount; m++)
		{
			if (m < m_nMCount)
			{

				Set_MovingCase(arMLoad.GetAt(m), dMLFactor, LoadCaseNo, LcomD, TRUE);  //????
				strDesc.Format(_T("+%sM"), GetFactorString(dMLFactor)); LcomD.Description = LcomD.Description + strDesc;
				if (m_nMCount > 1) { strDesc.Format(_T("[%d]"), m + 1); LcomD.Description = LcomD.Description + strDesc; }
				if (nType == 2 || nType == 4) m_aAcdnLcomD.Add(LcomD);
				else                     Set_LoadComb4Civil(LcomD, D_STR, 0, nMsgType);
			}
			int nMcrCount = max(m_nMCrlCount + 1, 1);
			for (int nMcr = 0; nMcr < nMcrCount; ++nMcr)
			{
				if (m < m_nMCount && nMcr < m_nMCrlCount)
				{
					Set_MovingCase(arMcrLoad.GetAt(nMcr), dCrlFactor, LoadCaseNo, LcomD, TRUE);
					strDesc.Format(_T("+%sMcr"), GetFactorString(dCrlFactor)); LcomD.Description = LcomD.Description + strDesc;
					if (m_nMCrlCount > 1) { strDesc.Format(_T("[%d]"), nMcr + 1); LcomD.Description = LcomD.Description + strDesc; }
					if (nType == 2 || nType == 4) m_aAcdnLcomD.Add(LcomD);
					else                     Set_LoadComb4Civil(LcomD, D_STR, 0, nMsgType);
				}

				T_LCOM_D LcomD_cf = LcomD; // MNET:1614, KJH20050809, CF°¡ ¾ø´Â ÇÏÁßÁ¶ÇÕ
				BOOL bLive = FALSE, bCF = FALSE;
				if (nType != 2)
				{
					bLive = m_nLCount > 0 || (bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(L)));
					if (bLive) // ST or ST+CS
					{
						int nLL = 0, nIL = 0, nCF = 0;
						for (nc = 0; nc < m_nLCount; nc++) nLL += Set_STLCIncludeCS(arLLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD, 0);
						if (nLL > 0) { strDesc.Format(_T("+%sL"), GetFactorString(dFactor)); LcomD.Description = LcomD.Description + strDesc; }
						if (bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(L)))
						{
							Set_CStageCase(m_aCSLoad.GetAt(nnn), dFactor, LoadCaseNo, LcomD, TRUE);
							strDesc.Format(_T("+%s(cEL)"), GetFactorString(dFactor)); LcomD.Description = LcomD.Description + strDesc;
						}
						for (nc = 0; nc < m_nIlCount; nc++) nIL += Set_STLCIncludeCS(arIlLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD, 0);
						if (nIL > 0) { strDesc.Format(_T("+%sIL"), GetFactorString(dFactor)); LcomD.Description = LcomD.Description + strDesc; }

						LcomD_cf = LcomD;
						for (nc = 0; nc < m_nCfCount; nc++) nCF += Set_STLCIncludeCS(arCfLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD, 0);
						if (nCF > 0) { strDesc.Format(_T("+%sCF"), GetFactorString(dFactor)); LcomD.Description = LcomD.Description + strDesc; bCF = TRUE; }
					}
				}

				if (m_nLsCount > 0) // Live Load Surcharge
				{
					T_LCOM_D LcomD_l = LcomD; LoadCaseNo = GetCountLcomCase(LcomD_l);
					int nLS = 0;
					for (nc = 0; nc < m_nLsCount; nc++) nLS += Set_STLCIncludeCS(arLsLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD_l, 0);
					T_LCOM_D LcomD_tmp = LcomD_cf;
					if (bCF)
					{
						LoadCaseNo = GetCountLcomCase(LcomD_tmp);
						for (nc = 0; nc < m_nLsCount; nc++) Set_STLCIncludeCS(arLsLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD_tmp, 0);
					}
					if (nLS > 0)
					{
						strDesc.Format(_T("+%sLS"), GetFactorString(dFactor)); LcomD_l.Description = LcomD_l.Description + strDesc;
						if (bCF) LcomD_tmp.Description = LcomD_tmp.Description + strDesc;
						ArINT artmp;
						SetCvlJTJD60_15_ETC(LcomD_l, LcomD_tmp, bCF, arCrlLoad, artmp, nType != 2, arBrkLoad, bLive, arWLoad, TRUE, arSfLoad, TRUE, arIpLoad, TRUE,
							arTLoad, arTpgLoad, TRUE, arFrLoad, FALSE, 1, dF, nType == 2 || nType == 4, nMsgType);
					}
				}
				if (nType != 2 && m_nCrlCount > 0) // Crowd Load
				{
					T_LCOM_D LcomD_l = LcomD; LoadCaseNo = GetCountLcomCase(LcomD_l);
					int nCRL = 0;
					for (nc = 0; nc < m_nCrlCount; nc++) nCRL += Set_STLCIncludeCS(arCrlLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD_l, 0);
					T_LCOM_D LcomD_tmp = LcomD_cf;
					if (bCF)
					{
						LoadCaseNo = GetCountLcomCase(LcomD_tmp);
						for (nc = 0; nc < m_nCrlCount; nc++) Set_STLCIncludeCS(arCrlLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD_tmp, 0);
					}
					if (nCRL > 0)
					{
						strDesc.Format(_T("+%sCRL"), GetFactorString(dFactor)); LcomD_l.Description = LcomD_l.Description + strDesc;
						if (bCF) LcomD_tmp.Description = LcomD_tmp.Description + strDesc;
						ArINT artmp;
						SetCvlJTJD60_15_ETC(LcomD_l, LcomD_tmp, bCF, arCrlLoad, artmp, FALSE, arBrkLoad, bLive, arWLoad, TRUE, arSfLoad, TRUE, arIpLoad, TRUE,
							arTLoad, arTpgLoad, TRUE, arFrLoad, FALSE, 2, dF, nType == 2 || nType == 4, nMsgType);
					}
				}
				if (m_nBrkCount > 0 && bLive) // Brake Load
				{
					T_LCOM_D LcomD_l = LcomD; LoadCaseNo = GetCountLcomCase(LcomD_l);
					double dBrkF = (nType == 0 || nType == 4) ? dFactor * 0.7 : 1.0;
					int nBRK = 0;
					for (nc = 0; nc < m_nBrkCount; nc++) nBRK += Set_STLCIncludeCS(arBrkLoad.GetAt(nc), dBrkF, LoadCaseNo, LcomD_l, 0);
					if (nBRK > 0)
					{
						strDesc.Format(_T("+%sBRK"), GetFactorString(dBrkF)); LcomD_l.Description = LcomD_l.Description + strDesc;
						ArINT artmp;
						SetCvlJTJD60_15_ETC(LcomD_l, LcomD_cf, FALSE, arCrlLoad, artmp, FALSE, arBrkLoad, FALSE, arWLoad, TRUE, arSfLoad, FALSE, arIpLoad, FALSE,
							arTLoad, arTpgLoad, FALSE, arFrLoad, FALSE, 3, dF, nType == 2 || nType == 4, nMsgType);
					}
					if (bCF) // MNET:1614, KJH20050809, CF°¡ ¾ø´Â ÇÏÁßÁ¶ÇÕ ÇÑ¹ø´õ...
					{
						LcomD_l = LcomD_cf; LoadCaseNo = GetCountLcomCase(LcomD_l);
						dBrkF = (nType == 0 || nType == 4) ? dFactor : 1.0;
						nBRK = 0;
						for (nc = 0; nc < m_nBrkCount; nc++) nBRK += Set_STLCIncludeCS(arBrkLoad.GetAt(nc), dBrkF, LoadCaseNo, LcomD_l, 0);
						if (nBRK > 0)
						{
							strDesc.Format(_T("+%sBRK"), GetFactorString(dBrkF)); LcomD_l.Description = LcomD_l.Description + strDesc;
							ArINT artmp;
							SetCvlJTJD60_15_ETC(LcomD_l, LcomD_cf, FALSE, arCrlLoad, artmp, FALSE, arBrkLoad, FALSE, arWLoad, TRUE, arSfLoad, FALSE, arIpLoad, FALSE,
								arTLoad, arTpgLoad, FALSE, arFrLoad, FALSE, 3, dF, nType == 2 || nType == 4, nMsgType);
						}
					}
				}
				int nWCount = m_nWCount * 2; // MNET:1614, KJH20050808
				if (m_nWCount == 0 && bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(W))) nWCount = 1;
				for (nc = 0; nc < nWCount; nc++)
				{
					T_LCOM_D LcomD_l = LcomD; LoadCaseNo = GetCountLcomCase(LcomD_l);
					if (m_nWCount > 0)
					{
						double dWF = (nc % 2 == 0) ? dWFactor : -1.*dWFactor;
						if (Set_STLCIncludeCS(arWLoad.GetAt(nc / 2), dWF, LoadCaseNo, LcomD_l, 0) > 0)
						{
							CString strW = (dWF > 0.0) ? _T("+%sW") : _T("%sW");
							strDesc.Format(strW, GetFactorString(dWF)); LcomD_l.Description = LcomD_l.Description + strDesc;
							if (m_nWCount > 1) { strDesc.Format(_T("[%d]"), nc / 2 + 1); LcomD_l.Description = LcomD_l.Description + strDesc; }
						}
					}
					if (bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(W)))
					{
						Set_CStageCase(m_aCSLoad.GetAt(nnn), dWFactor, LoadCaseNo, LcomD_l, TRUE);
						strDesc.Format(_T("+%s(cEL)"), GetFactorString(dWFactor)); LcomD_l.Description = LcomD_l.Description + strDesc;
					}
					ArINT artmp;
					SetCvlJTJD60_15_ETC(LcomD_l, LcomD_cf, bCF, arCrlLoad, artmp, FALSE, arBrkLoad, FALSE, arWLoad, FALSE, arSfLoad, TRUE, arIpLoad, TRUE,
						arTLoad, arTpgLoad, TRUE, arFrLoad, FALSE, 4, dF, nType == 2 || nType == 4, nMsgType);
				}
				if (m_nSfCount > 0) // Stream Flow Pressure
				{
					T_LCOM_D LcomD_l = LcomD; LoadCaseNo = GetCountLcomCase(LcomD_l);
					int nSF = 0;
					for (nc = 0; nc < m_nSfCount; nc++) nSF += Set_STLCIncludeCS(arSfLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD_l, 0);
					if (nSF > 0)
					{
						strDesc.Format(_T("+%sSF"), GetFactorString(dFactor)); LcomD_l.Description = LcomD_l.Description + strDesc;
						ArINT artmp;
						SetCvlJTJD60_15_ETC(LcomD_l, LcomD_cf, bCF, arCrlLoad, artmp, FALSE, arBrkLoad, FALSE, arWLoad, FALSE, arSfLoad, FALSE, arIpLoad, FALSE,
							arTLoad, arTpgLoad, TRUE, arFrLoad, FALSE, 5, dF, nType == 2 || nType == 4, nMsgType);
					}
				}
				if (m_nIpCount > 0) // Ice Pressure
				{
					T_LCOM_D LcomD_l = LcomD; LoadCaseNo = GetCountLcomCase(LcomD_l);
					int nIP = 0;
					for (nc = 0; nc < m_nIpCount; nc++) nIP += Set_STLCIncludeCS(arIpLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD_l, 0);
					if (nIP > 0)
					{
						strDesc.Format(_T("+%sIP"), GetFactorString(dFactor)); LcomD_l.Description = LcomD_l.Description + strDesc;
						ArINT artmp;
						SetCvlJTJD60_15_ETC(LcomD_l, LcomD_cf, bCF, arCrlLoad, artmp, FALSE, arBrkLoad, FALSE, arWLoad, FALSE, arSfLoad, FALSE, arIpLoad, FALSE,
							arTLoad, arTpgLoad, TRUE, arFrLoad, FALSE, 5, dF, nType == 2 || nType == 4, nMsgType);
					}
				}
			}
		}
	}

	return TRUE;
}
BOOL CLoadCombCtrl::SetCvlJTJD60_15_Var4_A(const CArray<T_LCOM_D, T_LCOM_D&>& arData, ArINT& arLLoad,
	ArINT& arIlLoad, ArINT& arCfLoad, ArINT& arLsLoad, ArINT& arCrlLoad, ArINT& arWLoad,
	ArINT& arSfLoad, ArINT& arIpLoad, ArINT& arTLoad, ArINT& arTpgLoad, ArINT& arFrLoad,
	ArINT& arBrkLoad, ArUNT& arMLoad, ArINT& arMcrLoad, int nType, int nMsgType, int nAcdType)
{
	// nType == 0,4: °è¼öÁ¶ÇÕ, 1: 1.0(settlement Á¦¿Ü), 2: 1.0(¸ðµÎ), Live Load Á¦¿Ü
	CArray<T_LCOM_D, T_LCOM_D&>aLcomD; aLcomD.Copy(arData);
	int  nLcomSize = aLcomD.GetSize();
	int  nnn = 0;
	BOOL bAddCsLoad = m_nConstLoad != 0 && CheckCS_SgldK(D_SGLD_ER1_CVL, nnn); // CS or ST+CS

	double dFactor = 1.0;
	double dMLFactor = 1.0;     // ???äü??
	double dCrlFactor = 1.0;    // ???äü??
	double dWFactor = 0.75;      // ??äü??
	double dTgpFactor = 0.8;    // ???äü??

	if (nAcdType == 0)//ÆµÓöÏµÊý modified this statement by gongxing at 2016-04-13
	{
		dMLFactor = 0.7;
		dCrlFactor = 1.0;
	}
	else if (nAcdType == 1)//×¼ÓÀ¾ÃÏµÊý
	{
		dMLFactor = 0.4;
		dCrlFactor = 0.4;
	}
	else if (nAcdType == 100)//»ù´¡ÑéËã-Å¼È»×éºÏ
	{
		dMLFactor = 1.0;
		dCrlFactor = 1.0;
		dWFactor = 1.0;
	}
	int nActive = m_nLcomType == D_LCOMTYPE_GENERAL ? 1 : 6;
	double dF = (nType == 0 || nType == 4) ? 0.75 : 1.0;
	double dIL = (nType == 12) ? 0. : CalcMovingILFactor();
	if (nAcdType == 100)//»ù´¡ÑéËã-Å¼È»×éºÏ
	{
		dIL = 0.0;
	}
	// ¿µ±¸ÇÏÁß + Â÷·®ÇÏÁß + 4°³ÀÇ °¡º¯ÇÏÁßÀÎ °æ¿ì
	CString strDesc = _T("");
	int LoadCaseNo = 0, nc = 0;
	for (int lc = 0; lc < nLcomSize; lc++)
	{
		T_LCOM_D LcomD = aLcomD.GetAt(lc);
		LcomD.nActive = nActive;
		LoadCaseNo = GetCountLcomCase(LcomD);

		int nMCount = max(m_nMCount + 1, 1);
		for (int m = 0; m < nMCount; m++)
		{
			if (m < m_nMCount)
			{

				Set_MovingCase(arMLoad.GetAt(m), dMLFactor, LoadCaseNo, LcomD, TRUE);  //????
				strDesc.Format(_T("+%sM"), GetFactorString(dMLFactor)); LcomD.Description = LcomD.Description + strDesc;
				if (m_nMCount > 1) { strDesc.Format(_T("[%d]"), m + 1); LcomD.Description = LcomD.Description + strDesc; }
				if (nType == 2 || nType == 4) m_aAcdnLcomD.Add(LcomD);
				else                     Set_LoadComb4Civil(LcomD, D_STR, 0, nMsgType);
			}

			int nMcrCount = max(m_nMCrlCount + 1, 1);
			for (int nMcr = 0; nMcr < nMcrCount; ++nMcr)
			{
				if (m < m_nMCount && nMcr < m_nMCrlCount)
				{
					Set_MovingCase(arMcrLoad.GetAt(nMcr), dCrlFactor, LoadCaseNo, LcomD, TRUE);
					strDesc.Format(_T("+%sMcr"), GetFactorString(dCrlFactor)); LcomD.Description = LcomD.Description + strDesc;
					if (m_nMCrlCount > 1) { strDesc.Format(_T("[%d]"), nMcr + 1); LcomD.Description = LcomD.Description + strDesc; }
					if (nType == 2 || nType == 4) m_aAcdnLcomD.Add(LcomD);
					else                     Set_LoadComb4Civil(LcomD, D_STR, 0, nMsgType);
				}

				T_LCOM_D LcomD_cf = LcomD; // MNET:1614, KJH20050809, CF°¡ ¾ø´Â ÇÏÁßÁ¶ÇÕ
				BOOL bLive = FALSE, bCF = FALSE;
				if (nType != 2)
				{
					bLive = m_nLCount > 0 || (bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(L)));
					if (bLive) // ST or ST+CS
					{
						int nLL = 0, nIL = 0, nCF = 0;
						for (nc = 0; nc < m_nLCount; nc++) nLL += Set_STLCIncludeCS(arLLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD, 0);
						if (nLL > 0) { strDesc.Format(_T("+%sL"), GetFactorString(dFactor)); LcomD.Description = LcomD.Description + strDesc; }
						if (bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(L)))
						{
							Set_CStageCase(m_aCSLoad.GetAt(nnn), dFactor, LoadCaseNo, LcomD, TRUE);
							strDesc.Format(_T("+%s(cEL)"), GetFactorString(dFactor)); LcomD.Description = LcomD.Description + strDesc;
						}
						for (nc = 0; nc < m_nIlCount; nc++) nIL += Set_STLCIncludeCS(arIlLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD, 0);
						if (nIL > 0) { strDesc.Format(_T("+%sIL"), GetFactorString(dFactor)); LcomD.Description = LcomD.Description + strDesc; }

						LcomD_cf = LcomD;
						for (nc = 0; nc < m_nCfCount; nc++) nCF += Set_STLCIncludeCS(arCfLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD, 0);
						if (nCF > 0) { strDesc.Format(_T("+%sCF"), GetFactorString(dFactor)); LcomD.Description = LcomD.Description + strDesc; bCF = TRUE; }
					}
				}

				if (m_nLsCount > 0) // Live Load Surcharge
				{
					T_LCOM_D LcomD_l = LcomD; LoadCaseNo = GetCountLcomCase(LcomD_l);
					int nLS = 0;
					for (nc = 0; nc < m_nLsCount; nc++) nLS += Set_STLCIncludeCS(arLsLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD_l, 0);
					T_LCOM_D LcomD_tmp = LcomD_cf;
					if (bCF)
					{
						LoadCaseNo = GetCountLcomCase(LcomD_tmp);
						for (nc = 0; nc < m_nLsCount; nc++) Set_STLCIncludeCS(arLsLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD_tmp, 0);
					}
					if (nLS > 0)
					{
						strDesc.Format(_T("+%sLS"), GetFactorString(dFactor)); LcomD_l.Description = LcomD_l.Description + strDesc;
						if (bCF) LcomD_tmp.Description = LcomD_tmp.Description + strDesc;
						BOOL bInc = nType == 2 ? FALSE : TRUE;
						ArINT artmp;
						SetCvlJTJD60_15_ETC(LcomD_l, LcomD_tmp, bCF, arCrlLoad, artmp, nType != 2, arBrkLoad, bLive, arWLoad, TRUE, arSfLoad, TRUE, arIpLoad, TRUE,
							arTLoad, arTpgLoad, FALSE, arFrLoad, FALSE, 6, dF, nType == 2 || nType == 4, nMsgType);
					}
				}
				if (nType != 2 && m_nCrlCount > 0) // Crowd Load
				{
					T_LCOM_D LcomD_l = LcomD; LoadCaseNo = GetCountLcomCase(LcomD_l);
					int nCRL = 0;
					for (nc = 0; nc < m_nCrlCount; nc++) nCRL += Set_STLCIncludeCS(arCrlLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD_l, 0);
					T_LCOM_D LcomD_tmp = LcomD_cf;
					if (bCF)
					{
						LoadCaseNo = GetCountLcomCase(LcomD_tmp);
						for (nc = 0; nc < m_nCrlCount; nc++) Set_STLCIncludeCS(arCrlLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD_tmp, 0);
					}
					if (nCRL > 0)
					{
						strDesc.Format(_T("+%sCRL"), GetFactorString(dFactor)); LcomD_l.Description = LcomD_l.Description + strDesc;
						if (bCF) LcomD_tmp.Description = LcomD_tmp.Description + strDesc;
						ArINT artmp;
						SetCvlJTJD60_15_ETC(LcomD_l, LcomD_tmp, bCF, arCrlLoad, artmp, FALSE, arBrkLoad, bLive, arWLoad, TRUE, arSfLoad, TRUE, arIpLoad, TRUE,
							arTLoad, arTpgLoad, FALSE, arFrLoad, FALSE, 7, dF, nType == 2 || nType == 4, nMsgType);
					}
				}
				int nWCount = m_nWCount * 2; // MNET:1614, KJH20050808
				if (m_nWCount == 0 && bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(W))) nWCount = 1;
				for (nc = 0; nc < nWCount; nc++)
				{
					T_LCOM_D LcomD_l = LcomD; LoadCaseNo = GetCountLcomCase(LcomD_l);
					if (m_nWCount > 0)
					{
						double dWF = (nc % 2 == 0) ? dWFactor : -1.*dWFactor;
						if (Set_STLCIncludeCS(arWLoad.GetAt(nc / 2), dWF, LoadCaseNo, LcomD_l, 0) > 0)
						{
							CString strW = (dWF > 0.0) ? _T("+%sW") : _T("%sW");
							strDesc.Format(strW, GetFactorString(dWF)); LcomD_l.Description = LcomD_l.Description + strDesc;
							if (m_nWCount > 1) { strDesc.Format(_T("[%d]"), nc / 2 + 1); LcomD_l.Description = LcomD_l.Description + strDesc; }
						}
					}
					if (bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(W)))
					{
						Set_CStageCase(m_aCSLoad.GetAt(nnn), dWFactor, LoadCaseNo, LcomD_l, TRUE);
						strDesc.Format(_T("+%s(cEL)"), GetFactorString(dWFactor)); LcomD_l.Description = LcomD_l.Description + strDesc;
					}
					ArINT artmp;
					SetCvlJTJD60_15_ETC(LcomD_l, LcomD_cf, bCF, arCrlLoad, artmp, FALSE, arBrkLoad, FALSE, arWLoad, FALSE, arSfLoad, TRUE, arIpLoad, TRUE,
						arTLoad, arTpgLoad, FALSE, arFrLoad, FALSE, 7, dF, nType == 2 || nType == 4, nMsgType);
				}
			}
		}
	}

	return TRUE;
}
BOOL CLoadCombCtrl::SetCvlJTJD60_15_Var56_A(const CArray<T_LCOM_D, T_LCOM_D&>& arData, ArINT& arLLoad,
	ArINT& arIlLoad, ArINT& arCfLoad, ArINT& arLsLoad, ArINT& arCrlLoad, ArINT& arWLoad,
	ArINT& arSfLoad, ArINT& arIpLoad, ArINT& arTLoad, ArINT& arTpgLoad, ArINT& arFrLoad,
	ArINT& arBrkLoad, ArUNT& arMLoad, ArINT& arMcrLoad, int nType, int nMsgType, int nAcdType)
{
	// nType == 0,4: °è¼öÁ¶ÇÕ, 1: 1.0(settlement Á¦¿Ü), 2: 1.0(¸ðµÎ), Live Load Á¦¿Ü
	CArray<T_LCOM_D, T_LCOM_D&> aLcomD; aLcomD.Copy(arData);
	int  nLcomSize = aLcomD.GetSize();
	int  nnn = 0;
	BOOL bAddCsLoad = m_nConstLoad != 0 && CheckCS_SgldK(D_SGLD_ER1_CVL, nnn); // CS or ST+CS

	double dFactor = 1.0;
	double dMLFactor = 1.0;     // ???äü??
	double dCrlFactor = 1.0;    // ???äü??
	double dWFactor = 0.75;      // ??äü??
	double dTgpFactor = 0.8;    // ???äü??

	if (nAcdType == 0)//ÆµÓöÏµÊý modified this statement by gongxing at 2016-04-13
	{
		dMLFactor = 0.7;
		dCrlFactor = 1.0;
	}
	else if (nAcdType == 1)//×¼ÓÀ¾ÃÏµÊý
	{
		dMLFactor = 0.4;
		dCrlFactor = 0.4;
	}
	else if (nAcdType == 100)//»ù´¡ÑéËã-Å¼È»×éºÏ
	{
		dMLFactor = 1.0;
		dCrlFactor = 1.0;
		dWFactor = 1.0;
	}
	int nActive = m_nLcomType == D_LCOMTYPE_GENERAL ? 1 : 6;
	double dF = (nType == 0 || nType == 4) ? 0.5 : 1.0;
	double dIL = (nType == 12) ? 0. : CalcMovingILFactor();
	if (nAcdType == 100)//»ù´¡ÑéËã-Å¼È»×éºÏ
	{
		dIL = 0.0;
	}
	CString strDesc = _T("");
	int LoadCaseNo = 0, nc = 0;
	int lc = 0;
	for (lc = 0; lc < nLcomSize; lc++)
	{
		T_LCOM_D LcomD = aLcomD.GetAt(lc);
		LcomD.nActive = nActive;
		LoadCaseNo = GetCountLcomCase(LcomD);

		int nMCount = max(m_nMCount + 1, 1);
		for (int m = 0; m < nMCount; m++)
		{
			if (m < m_nMCount)
			{
				Set_MovingCase(arMLoad.GetAt(m), dMLFactor, LoadCaseNo, LcomD, TRUE);  //????
				strDesc.Format(_T("+%sM"), GetFactorString(dMLFactor)); LcomD.Description = LcomD.Description + strDesc;
				if (m_nMCount > 1) { strDesc.Format(_T("[%d]"), m + 1); LcomD.Description = LcomD.Description + strDesc; }
				if (nType == 2 || nType == 4) m_aAcdnLcomD.Add(LcomD);
				else                     Set_LoadComb4Civil(LcomD, D_STR, 0, nMsgType);
			}

			int nMcrCount = max(m_nMCrlCount + 1, 1);
			for (int nMcr = 0; nMcr < nMcrCount; ++nMcr)
			{
				if (m < m_nMCount&& nMcr < m_nMCrlCount)
				{
					Set_MovingCase(arMcrLoad.GetAt(nMcr), dCrlFactor, LoadCaseNo, LcomD, TRUE);
					strDesc.Format(_T("+%sMcr"), GetFactorString(dCrlFactor)); LcomD.Description = LcomD.Description + strDesc;
					if (m_nMCrlCount > 1) { strDesc.Format(_T("[%d]"), nMcr + 1); LcomD.Description = LcomD.Description + strDesc; }
					if (nType == 2 || nType == 4) m_aAcdnLcomD.Add(LcomD);
					else                     Set_LoadComb4Civil(LcomD, D_STR, 0, nMsgType);
				}

				T_LCOM_D LcomD_cf = LcomD; // MNET:1614, KJH20050809, CF Á¦¿ÜÇÑ ÇÏÁßÁ¶ÇÕ
				BOOL bLive = FALSE, bCF = FALSE;
				if (nType != 2)
				{
					bLive = m_nLCount > 0 || (bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(L)));
					if (bLive) // ST or ST+CS
					{
						int nLL = 0, nIL = 0, nCF = 0;
						for (nc = 0; nc < m_nLCount; nc++) nLL += Set_STLCIncludeCS(arLLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD, 0);
						if (nLL > 0) { strDesc.Format(_T("+%sL"), GetFactorString(dFactor)); LcomD.Description = LcomD.Description + strDesc; }
						if (bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(L)))
						{
							Set_CStageCase(m_aCSLoad.GetAt(nnn), dFactor, LoadCaseNo, LcomD, TRUE);
							strDesc.Format(_T("+%s(cEL)"), GetFactorString(dFactor)); LcomD.Description = LcomD.Description + strDesc;
						}
						for (nc = 0; nc < m_nIlCount; nc++) nIL += Set_STLCIncludeCS(arIlLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD, 0);
						if (nIL > 0) { strDesc.Format(_T("+%sIL"), GetFactorString(dFactor)); LcomD.Description = LcomD.Description + strDesc; }

						LcomD_cf = LcomD;
						for (nc = 0; nc < m_nCfCount; nc++) nCF += Set_STLCIncludeCS(arCfLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD, 0);
						if (nCF > 0) { strDesc.Format(_T("+%sCF"), GetFactorString(dFactor)); LcomD.Description = LcomD.Description + strDesc; bCF = TRUE; }
					}
				}

				if (m_nLsCount > 0) // Live Load Surcharge
				{
					T_LCOM_D LcomD_l = LcomD; LoadCaseNo = GetCountLcomCase(LcomD_l);
					int nLS = 0;
					for (nc = 0; nc < m_nLsCount; nc++) nLS += Set_STLCIncludeCS(arLsLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD_l, 0);
					T_LCOM_D LcomD_tmp = LcomD_cf;
					if (bCF)
					{
						LoadCaseNo = GetCountLcomCase(LcomD_tmp);
						for (nc = 0; nc < m_nLsCount; nc++) Set_STLCIncludeCS(arLsLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD_tmp, 0);
					}
					if (nLS > 0)
					{
						strDesc.Format(_T("+%sLS"), GetFactorString(dFactor)); LcomD_l.Description = LcomD_l.Description + strDesc;
						if (bCF) LcomD_tmp.Description = LcomD_tmp.Description + strDesc;
						ArINT artmp;
						SetCvlJTJD60_15_ETC(LcomD_l, LcomD_tmp, bCF, arCrlLoad, artmp, nType != 2, arBrkLoad, bLive, arWLoad, TRUE, arSfLoad, FALSE, arIpLoad, FALSE,
							arTLoad, arTpgLoad, FALSE, arFrLoad, FALSE, 8, dF, nType == 2 || nType == 4, nMsgType);
					}
				}
				if (nType != 2 && m_nCrlCount > 0) // Crowd Load
				{
					T_LCOM_D LcomD_l = LcomD; LoadCaseNo = GetCountLcomCase(LcomD_l);
					int nCRL = 0;
					for (nc = 0; nc < m_nCrlCount; nc++) nCRL += Set_STLCIncludeCS(arCrlLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD_l, 0);
					if (nCRL > 0)
					{
						strDesc.Format(_T("+%sCRL"), GetFactorString(dFactor)); LcomD_l.Description = LcomD_l.Description + strDesc;
						ArINT artmp;
						SetCvlJTJD60_15_ETC(LcomD_l, LcomD_cf, bCF, arCrlLoad, artmp, FALSE, arBrkLoad, FALSE, arWLoad, TRUE, arSfLoad, FALSE, arIpLoad, FALSE,
							arTLoad, arTpgLoad, FALSE, arFrLoad, FALSE, 9, dF, nType == 2 || nType == 4, nMsgType);
					}
				}
			}
		}
	}

	// ¿µ±¸ÇÏÁß + Â÷·®ÇÏÁß + 6°³ÀÇ °¡º¯ÇÏÁßÀÎ °æ¿ì
	for (lc = 0; lc < nLcomSize; lc++)
	{
		T_LCOM_D LcomD = aLcomD.GetAt(lc); LcomD.nActive = nActive;
		LoadCaseNo = GetCountLcomCase(LcomD);

		if (nType != 2)
		{
			if (m_nLCount > 0 || (bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(L)))) // ST or ST+CS
			{
				int nLL = 0, nIL = 0, nCF = 0;
				for (nc = 0; nc < m_nLCount; nc++) nLL += Set_STLCIncludeCS(arLLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD, 0);
				if (nLL > 0) { strDesc.Format(_T("+%sL"), GetFactorString(dFactor)); LcomD.Description = LcomD.Description + strDesc; }
				if (bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(L)))
				{
					Set_CStageCase(m_aCSLoad.GetAt(nnn), dFactor, LoadCaseNo, LcomD, TRUE);
					strDesc.Format(_T("+%s(cEL)"), GetFactorString(dFactor)); LcomD.Description = LcomD.Description + strDesc;
				}
				for (nc = 0; nc < m_nIlCount; nc++) nIL += Set_STLCIncludeCS(arIlLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD, 0);
				if (nIL > 0) { strDesc.Format(_T("+%sIL"), GetFactorString(dFactor)); LcomD.Description = LcomD.Description + strDesc; }
				for (nc = 0; nc < m_nCfCount; nc++) nCF += Set_STLCIncludeCS(arCfLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD, 0);
				if (nCF > 0) { strDesc.Format(_T("+%sCF"), GetFactorString(dFactor)); LcomD.Description = LcomD.Description + strDesc; }
			}
		}

		if (m_nLsCount > 0) // Live Load Surcharge
		{
			T_LCOM_D LcomD_l = LcomD; LoadCaseNo = GetCountLcomCase(LcomD_l);
			int nLS = 0;
			for (nc = 0; nc < m_nLsCount; nc++) nLS += Set_STLCIncludeCS(arLsLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD_l, 0);
			if (nLS > 0)
			{
				strDesc.Format(_T("+%sLS"), GetFactorString(dFactor)); LcomD_l.Description = LcomD_l.Description + strDesc;
				ArINT artmp;
				SetCvlJTJD60_15_ETC(LcomD_l, LcomD_l, FALSE, arCrlLoad, artmp, nType != 2, arBrkLoad, FALSE, arWLoad, FALSE, arSfLoad, FALSE, arIpLoad, FALSE,
					arTLoad, arTpgLoad, FALSE, arFrLoad, FALSE, 10, dF, nType == 2 || nType == 4, nMsgType);
			}
		}
	}

	return TRUE;
}

////////////////////////////////////////////////////////////////////////////// 
#pragma endregion
//////////////////////////////////////////////////////////////////////////////
#pragma region lcom_EngneeringStructureCommonCode_GB19
////////////////////////////////////////////////////////////////////////////// 
//JTG D60-2015
BOOL CLoadCombCtrl::Set_CvlESCGB19_Permanent(ArINT& arDLoad, ArINT& arPsLoad, ArINT& arEvLoad, ArINT& arEhLoad, ArINT& arShLoad, ArINT& arCrLoad, ArINT& arBLoad, ArINT& arStlLoad, ArUNT& arSmLoad, int nType, CArray<T_LCOM_D, T_LCOM_D&>& arData)
{
	// nType  0 or 4:×éºÏÏµÊý, 1 or 2: 1.0(settlement ³ýÍâ), 3: 1.0(ÒÔ Ï¾?
	arData.RemoveAll();

	if (m_nConstLoad == 0 && m_nDCount == 0 && m_nPsCount == 0 && m_nEvCount == 0 && m_nEhCount == 0 &&
		m_nShCount == 0 && m_nCrCount == 0 && m_nBCount == 0 && m_nStlCount == 0 && m_nSmCount == 0) return FALSE;

	int LoadCaseNo = 0;
	int nSmCount = max(m_nSmCount, 1);

	double dFact_1p2 = (nType == 0 || nType == 4) ? 1.2 : 1.0;
	double dFact_1p4 = (nType == 0 || nType == 4) ? 1.4 : 1.0;
	double dFact_0p5 = (nType == 3) ? 1.0 : 0.5;
	if ((m_nLcomType == D_LCOMTYPE_STLCOMP || m_nLcomType == D_LCOMTYPE_SRC) && (nType == 0 || nType == 1))
	{
		dFact_0p5 = 1.0;
	}
	if (m_nDesignType == 0) dFact_0p5 = 1.0;

	CString str1p2 = (nType == 0 || nType == 4) ? _T("1.2") : _T("1.0");
	CString str1p4 = (nType == 0 || nType == 4) ? _T("1.4") : _T("1.0");
	CString str0p5 = (nType == 3) ? _T("1.0") : _T("0.5");
	if (m_nDesignType == 0) str0p5 = _T("1.0");

	for (int sm = 0; sm < nSmCount; sm++)
	{
		T_LCOM_D LcomD; LcomD.Initialize();
		LoadCaseNo = 0;
		CString strPos = _T("");
		int i = 0, nDL = 0, nPS = 0, nEV = 0, nEH = 0, nSH = 0, nCR = 0, nBL = 0, nSTL = 0;
		for (i = 0; i < m_nDCount; i++) nDL += Set_STLCIncludeCS(arDLoad.GetAt(i), dFact_1p2, LoadCaseNo, LcomD, D_SGLD_DL_CVL);
		if (nDL > 0) { LcomD.Description += strPos + str1p2 + _LSX(D); strPos = _LSX(+); }
		for (i = 0; i < m_nPsCount; i++) nPS += Set_STLCIncludeCS(arPsLoad.GetAt(i), dFact_1p2, LoadCaseNo, LcomD, D_SGLD_TS_CVL);
		if (nPS > 0) { LcomD.Description += strPos + str1p2 + _LSX(PS); strPos = _LSX(+); }
		for (i = 0; i < m_nEvCount; i++) nEV += Set_STLCIncludeCS(arEvLoad.GetAt(i), dFact_1p2, LoadCaseNo, LcomD, 0);
		if (nEV > 0) { LcomD.Description += strPos + str1p2 + _LSX(EV); strPos = _LSX(+); }
		for (i = 0; i < m_nEhCount; i++) nEH += Set_STLCIncludeCS(arEhLoad.GetAt(i), dFact_1p4, LoadCaseNo, LcomD, 0);
		if (nEH > 0) { LcomD.Description += strPos + str1p4 + _LSX(EH); strPos = _LSX(+); }
		for (i = 0; i < m_nCrCount; i++) nCR += Set_STLCIncludeCS(arCrLoad.GetAt(i), 1., LoadCaseNo, LcomD, D_SGLD_CS_CVL);
		if (nCR > 0) { LcomD.Description += strPos + _T("1.0") + _LSX(CR); strPos = _LSX(+); }
		for (i = 0; i < m_nShCount; i++) nSH += Set_STLCIncludeCS(arShLoad.GetAt(i), 1., LoadCaseNo, LcomD, D_SGLD_SS_CVL);
		if (nSH > 0) { LcomD.Description += strPos + _T("1.0") + _LSX(SH); strPos = _LSX(+); }
		for (i = 0; i < m_nBCount; i++)  nBL += Set_STLCIncludeCS(arBLoad.GetAt(i), 1., LoadCaseNo, LcomD, 0);
		if (nBL > 0) { LcomD.Description += strPos + _T("1.0") + _LSX(B); strPos = _LSX(+); }
		for (i = 0; i < m_nStlCount; i++) nSTL += Set_STLCIncludeCS(arStlLoad.GetAt(i), dFact_0p5, LoadCaseNo, LcomD, 0);
		if (nSTL > 0) { LcomD.Description += strPos + str0p5 + _LSX(STL); strPos = _LSX(+); }
		if (m_nSmCount > 0)
		{
			Set_SettleCase(arSmLoad.GetAt(sm), dFact_0p5, LoadCaseNo, LcomD, TRUE); // SM
			CString strSM; strSM.Format(_T("%sSM[%d]"), str0p5, sm + 1);
			LcomD.Description += strPos + strSM; strPos = _LSX(+);
		}
		if (m_nConstLoad != 0) // CS or ST+CS
		{
			int n = 0;
			if (CheckCS_SgldK(D_SGLD_DL_CVL, n))
			{
				Set_CStageCase(m_aCSLoad.GetAt(n), dFact_1p2, LoadCaseNo, LcomD, TRUE);
				LcomD.Description += strPos + str1p2 + _T("(cD)"); strPos = _LSX(+);
			}

			CString strcEL;
			int nErection = 0;

			for (int nErec = 0; nErec < D_STCT_EREC_MAX_CVL; ++nErec)
			{
				strcEL = _T(""); nErection = 0;
				if (CheckCS_SgldK(m_CSEL_SgldK[nErec], n)) { strcEL = m_CSEL_Sgld_Desc[nErec]; nErection = m_CSEL_SgldK[nErec]; }



				if (CheckCSEL(nErection, _LSX(D)))
				{
					Set_CStageCase(m_aCSLoad.GetAt(n), dFact_1p2, LoadCaseNo, LcomD, TRUE);
					LcomD.Description += strPos + str1p2 + strcEL; strPos = _LSX(+);
				}
			}
			if (CheckCS_SgldK(D_SGLD_TP_CVL, n) && nType == 3) // tendon primary
			{
				double dPST = m_bLossFactor ? m_dLossFactorTransfer * dFact_1p2 : dFact_1p2;
				Set_CStageCase(m_aCSLoad.GetAt(n), dPST, LoadCaseNo, LcomD, TRUE);
				LcomD.Description += strPos + GetFactorString(dPST) + _T("(cTP)"); strPos = _LSX(+);
			}
			if (CheckCS_SgldK(D_SGLD_TS_CVL, n)) // tendon secondary  ¸ÖÊø¶þ´Î
			{
				double dPSS = m_bLossFactor ? m_dLossFactorService * dFact_1p2 : dFact_1p2;
				Set_CStageCase(m_aCSLoad.GetAt(n), dPSS, LoadCaseNo, LcomD, TRUE);
				LcomD.Description += strPos + GetFactorString(dPSS) + _T("(cTS)"); strPos = _LSX(+);
			}
			if (CheckCS_SgldK(D_SGLD_CS_CVL, n)) // Creep secondary   Ðì±ä¶þ´Î
			{
				Set_CStageCase(m_aCSLoad.GetAt(n), 1.0, LoadCaseNo, LcomD, TRUE);
				LcomD.Description += strPos + _T("1.0(cCR)"); strPos = _LSX(+);
			}
			if (CheckCS_SgldK(D_SGLD_SS_CVL, n)) // Shrinkage secondary  ÊÕËõ¶þ´Î
			{
				Set_CStageCase(m_aCSLoad.GetAt(n), 1.0, LoadCaseNo, LcomD, TRUE);
				LcomD.Description += strPos + _T("1.0(cSH)"); strPos = _LSX(+);
			}
		}
		arData.Add(LcomD);
	}

	if (arData.GetSize() < 1) return FALSE;

	arData.FreeExtra();

	return TRUE;
}

BOOL CLoadCombCtrl::SetCvlESCGB19_Capacity_M(const CArray<T_LCOM_D, T_LCOM_D&>& arData, ArUNT& arMLoad, ArINT& arLLoad,
	ArINT& arIlLoad, ArINT& arCrlLoad, ArINT& arMCrlLoad, ArINT& arCfLoad, ArINT& arLsLoad,
	ArINT& arWLoad, ArINT& arWlLoad, ArINT& arSfLoad, ArINT& arIpLoad, ArINT& arTLoad, ArINT& arTpgLoad,
	ArINT& arFrLoad, ArINT& arBrkLoad, int nType)
{
	// nType == 0,4: °è¼öÁ¶ÇÕ, 1: 1.0(settlement Á¦¿Ü), 2: 1.0(¸ðµÎ), Live Load Á¦¿Ü
	CArray<T_LCOM_D, T_LCOM_D&> aLcomD; aLcomD.Copy(arData);
	int  nLcomSize = aLcomD.GetSize();
	int  nnn = 0, lc = 0;
	//BOOL bAddCsLoad = (m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER1_CVL,nnn)) || (m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER2_CVL,nnn)) || (m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER3_CVL,nnn)); // CS or ST+CS
	BOOL bAddCsLoad = m_nConstLoad != 0 && CheckCS_SgldK(D_SGLD_ER1_CVL, nnn); // °üº¬Ê©¹¤ºÉÔØÇÒ´æÔÚÊ©¹¤ºÉÔØ
	// ¿µ±¸ÇÏÁß¸¸ Á¶ÇÕ
	for (lc = 0; lc < nLcomSize; lc++)
	{
		T_LCOM_D LcomD = aLcomD.GetAt(lc);
		LcomD.nActive = 1; // Active or Strength/Stress
		if (nType == 2 || nType == 4) m_aAcdnLcomD.Add(LcomD);
		else                     Set_LoadComb4Civil(LcomD, D_STR, 0, 112); // 112:ÓÀ¾Ã×÷ÓÃ£¨ºãÔØ£©
	}

	double dFactor = (nType == 0 || nType == 4) ? 1.4 : 1.0;//³ýÆû³µºÉÔØ£¨º¬³å»÷Á¦¡¢ÀëÐÄÁ¦£©¡¢·çºÉÔØÍâ¿É±ä×÷ÓÃ·ÖÏîÏµÊý
	double dWFactor = (nType == 0 || nType == 4) ? 1.4 : 1.0;//ÔËÐÐ·çºÉÔØ·ÖÏîÏµÊý
	double dWlFactor = (nType == 0 || nType == 4) ? 1.1 : 1.0;//Ë²Ê±·çºÉÔØ·ÖÏîÏµÊý
	double dCJJMLFactor = (nType == 0 || nType == 4) ? 1.1 : 1.0;   // CJJ ÌØÖÖ³µÁ¾ ×éºÏÏµÊý
	double dIL = (nType == 12) ? 0. : CalcMovingILFactor();    // Æû³µºÉÔØ³å»÷ÏµÊý

	int nMsgType = (nType == 0 || nType == 1) ? 114 : 115; // 114:»ù±¾×éºÏ, 115:Å¼È»×éºÏ

	// ÓÀ¾Ã×÷ÓÃ + 1¸ö¿É±ä×÷ÓÃ
	if (m_bJTG15_Factor)
	{
		dFactor *= m_dJTG15_Factor;
		dWFactor *= m_dJTG15_Factor;
		dWlFactor *= m_dJTG15_Factor;
		dCJJMLFactor *= m_dJTG15_Factor;
	}

	CString strDesc = _T("");
	int LoadCaseNo = 0, nc = 0;
	for (lc = 0; lc < nLcomSize; lc++)
	{
		int nMCount = max(m_nMCount, 1);
		for (int m = 0; m < nMCount; m++)
		{
			T_LCOM_D LcomD = aLcomD.GetAt(lc); LcomD.nActive = 1; // Active or Strength/Stress
			LoadCaseNo = GetCountLcomCase(LcomD);

			LcomD.nActive = nType == 17 ? 4 : 1;  // Active or Strength/Stress			 
			if (m_nLcomType == D_LCOMTYPE_GENERAL) LcomD.nActive = 1;  // general
			double dMlFactor = 1.4;               // Æû³µºÉÔØ×éºÏÏµÊý
			CString strJudgeMILFactorByLoadCase = GetJTGD602015MILoadName(arMLoad, m);
			if (nType == 0 || nType == 1)         // ³ÐÔØÄÜÁ¦×´Ì¬  »ù±¾×éºÏ
			{
				if (strJudgeMILFactorByLoadCase == _T("CH-CL") || strJudgeMILFactorByLoadCase == _T("ÓÃ»§¶¨Òå"))
				{
					dMlFactor = 1.8;                  // ³µÁ¾ºÉÔØ  ÓÃ»§¶¨Òå   È¡1.8
				}
			}
			if (nType == 17)  dMlFactor = 1.0;
			if (nMsgType == 114 && m_bJTG15_Factor)
			{
				dMlFactor *= m_dJTG15_Factor;
			}

			if (nType != 2) // live load »îºÉÔØ
			{
				BOOL bAddLcomD = FALSE;
				// moving load ÒÆ¶¯ºÉÔØ
				if (m_nMCount > 0)
				{
					Set_MovingCase(arMLoad.GetAt(m), dMlFactor, LoadCaseNo, LcomD, TRUE);
					strDesc.Format(_T("+%sM"), GetFactorString(dMlFactor)); LcomD.Description += strDesc;
					if (m_nMCount > 1) { strDesc.Format(_T("[%d]"), m + 1); LcomD.Description += strDesc; }
					bAddLcomD = TRUE;
				}

				// »îÔØ  »îÔØ³å»÷
				int nLL = 0, nIL = 0, nCF = 0;
				if (m_nLCount > 0 || (bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(L)))) // ST or ST+CS
				{
					for (nc = 0; nc < m_nLCount; nc++)
						nLL += Set_STLCIncludeCS(arLLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD, 0);
					if (nLL > 0) { strDesc.Format(_T("+%sL"), GetFactorString(dFactor)); LcomD.Description = LcomD.Description + strDesc; }
					if (bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(L)))
					{
						Set_CStageCase(m_aCSLoad.GetAt(nnn), dFactor, LoadCaseNo, LcomD, TRUE);
						strDesc.Format(_T("+%s(cEL)"), GetFactorString(dFactor)); LcomD.Description = LcomD.Description + strDesc;
						bAddLcomD = TRUE;
					}
					for (nc = 0; nc < m_nIlCount; nc++)
						nIL += Set_STLCIncludeCS(arIlLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD, 0);
					if (nIL > 0) { strDesc.Format(_T("+%sIL"), GetFactorString(dFactor)); LcomD.Description = LcomD.Description + strDesc; }
					if (nLL + nIL > 0) bAddLcomD = TRUE;
				}

				// ÀëÐÄÁ¦
				if (m_nMCount > 0 || (m_nLCount > 0 || (bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(L)))))
				{
					for (nc = 0; nc < m_nCfCount; nc++)
						nCF += Set_STLCIncludeCS(arCfLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD, 0);
					if (nCF > 0) { strDesc.Format(_T("+%sCF"), GetFactorString(dFactor)); LcomD.Description += strDesc; }
					bAddLcomD = TRUE;
				}
				if (bAddLcomD)
				{
					if (nType == 4) m_aAcdnLcomD.Add(LcomD);
					else         Set_LoadComb4Civil(LcomD, D_STR, 0, nMsgType);
				}
			}

			if (m_nLsCount > 0) // Live Load Surcharge ¸½¼ÓµØÃæ»îºÉÔØ
			{
				LcomD = aLcomD.GetAt(lc); LcomD.nActive = 1; LoadCaseNo = GetCountLcomCase(LcomD);
				int nLS = 0;
				for (nc = 0; nc < m_nLsCount; nc++) nLS += Set_STLCIncludeCS(arLsLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD, 0);
				if (nLS > 0)
				{
					strDesc.Format(_T("+%sLS"), GetFactorString(dFactor)); LcomD.Description += strDesc;
					if (nType == 2 || nType == 4) m_aAcdnLcomD.Add(LcomD);
					else                     Set_LoadComb4Civil(LcomD, D_STR, 0, nMsgType);
				}
			}// MNET:XXXX-BJLEE(BBONG)-20060502

			// ²½ÐÐ»îºÉÔØ   ÈËÈººÉÔØ
			if (nType != 2 && (m_nCrlCount > 0 || m_nMCrlCount > 0)) // Crowd Load
			{
				LcomD = aLcomD.GetAt(lc);
				LcomD.nActive = nType == 17 ? 4 : 1;
				if (m_nLcomType == D_LCOMTYPE_GENERAL) LcomD.nActive = 1;  // general
				LoadCaseNo = GetCountLcomCase(LcomD);
				int nCRL = 0;
				for (nc = 0; nc < m_nCrlCount; nc++)
					nCRL += Set_STLCIncludeCS(arCrlLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD, 0);
				if (nCRL > 0) { strDesc.Format(_T("+%sCRL"), GetFactorString(dFactor)); LcomD.Description += strDesc; }

				int nMCrlCount = (m_nMCrlCount > 0) ? m_nMCrlCount : ((nCRL > 0) ? 1 : 0);
				for (int crl = 0; crl < nMCrlCount; ++crl)
				{
					T_LCOM_D LcomD_crl = LcomD; LoadCaseNo = GetCountLcomCase(LcomD_crl);
					if (m_nMCrlCount > 0)
					{
						Set_MovingCase(arMCrlLoad.GetAt(crl), dFactor, LoadCaseNo, LcomD_crl, TRUE);
						strDesc.Format(_T("+%sMCRL"), GetFactorString(dFactor));
						LcomD_crl.Description += strDesc;
						if (m_nMCrlCount > 1) { strDesc.Format(_T("[%d]"), crl + 1); LcomD_crl.Description += strDesc; }
					}
					if (nCRL > 0 || m_nMCrlCount > 0)
					{
						if (nType == 2 || nType == 4) m_aAcdnLcomD.Add(LcomD_crl);
						else                     Set_LoadComb4Civil(LcomD_crl, D_STR, 0, nMsgType);
					}
				}
			}

			{
				int nWCount = m_nWCount * 2; // ÔËÓª·çºÉÔØ
				if (m_nWCount == 0 && bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(W))) nWCount = 1;
				for (nc = 0; nc < nWCount; nc++)
				{
					LcomD = aLcomD.GetAt(lc);
					LcomD.nActive = nType == 17 ? 4 : 1;
					if (m_nLcomType == D_LCOMTYPE_GENERAL) LcomD.nActive = 1;  // general
					LoadCaseNo = GetCountLcomCase(LcomD);
					if (m_nWCount > 0)
					{
						double dWF = (nc % 2 == 0) ? dWFactor : -1.*dWFactor;
						if (Set_STLCIncludeCS(arWLoad.GetAt(nc / 2), dWF, LoadCaseNo, LcomD, 0) > 0)
						{
							CString strW = (dWF > 0.0) ? _T("+%sW") : _T("%sW");
							strDesc.Format(strW, GetFactorString(dWF)); LcomD.Description += strDesc;
							if (m_nWCount > 1) { strDesc.Format(_T("[%d]"), nc / 2 + 1); LcomD.Description += strDesc; }
						}
					}
					if (bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(W)))
					{
						Set_CStageCase(m_aCSLoad.GetAt(nnn), dWFactor, LoadCaseNo, LcomD, TRUE);
						strDesc.Format(_T("+%s(cEL)"), GetFactorString(dWFactor)); LcomD.Description += strDesc;
					}
					if (nType == 2 || nType == 4) m_aAcdnLcomD.Add(LcomD);
					else                     Set_LoadComb4Civil(LcomD, D_STR, 0, nMsgType);

					double dF = (nType == 0 || nType == 4) ? 0.75 : 1.0;
					SetCvlESCGB19_ETC_M(LcomD, LcomD, FALSE, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWLoad, FALSE, arWlLoad, FALSE, arSfLoad, TRUE, arIpLoad, TRUE,
						arTLoad, arTpgLoad, TRUE, arFrLoad, TRUE, 0, dF, nType == 2 || nType == 4, nMsgType);
				}
			}

			{
				int nWlCount = m_nWlCount * 2; // Ë²Ê±·çºÉÔØ
				if (m_nWlCount == 0 && bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(WL))) nWlCount = 1;
				for (nc = 0; nc < nWlCount; nc++)
				{
					LcomD = aLcomD.GetAt(lc);
					LcomD.nActive = nType == 17 ? 4 : 1;
					if (m_nLcomType == D_LCOMTYPE_GENERAL) LcomD.nActive = 1;  // general
					LoadCaseNo = GetCountLcomCase(LcomD);
					if (m_nWlCount > 0)
					{
						double dWF = (nc % 2 == 0) ? dWlFactor : -1.*dWlFactor;
						if (Set_STLCIncludeCS(arWlLoad.GetAt(nc / 2), dWF, LoadCaseNo, LcomD, 0) > 0)
						{
							CString strW = (dWF > 0.0) ? _T("+%sWL") : _T("%sWL");
							strDesc.Format(strW, GetFactorString(dWF)); LcomD.Description += strDesc;
							if (m_nWCount > 1) { strDesc.Format(_T("[%d]"), nc / 2 + 1); LcomD.Description += strDesc; }
						}
					}
					if (bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(WL)))
					{
						Set_CStageCase(m_aCSLoad.GetAt(nnn), dWlFactor, LoadCaseNo, LcomD, TRUE);
						strDesc.Format(_T("+%s(cEL)"), GetFactorString(dWlFactor)); LcomD.Description += strDesc;
					}
					if (nType == 2 || nType == 4) m_aAcdnLcomD.Add(LcomD);
					else                     Set_LoadComb4Civil(LcomD, D_STR, 0, nMsgType);

					double dF = (nType == 0 || nType == 4) ? 0.75 : 1.0;
					SetCvlESCGB19_ETC_M(LcomD, LcomD, FALSE, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWLoad, FALSE, arWlLoad, FALSE, arSfLoad, TRUE, arIpLoad, TRUE,
						arTLoad, arTpgLoad, TRUE, arFrLoad, TRUE, 0, dF, nType == 2 || nType == 4, nMsgType);
				}
			}

			if (m_nSfCount > 0) // Stream Flow Pressure Á÷Ë®Ñ¹Á¦
			{
				LcomD = aLcomD.GetAt(lc);
				LcomD.nActive = nType == 17 ? 4 : 1;
				if (m_nLcomType == D_LCOMTYPE_GENERAL) LcomD.nActive = 1;  // general
				LoadCaseNo = GetCountLcomCase(LcomD);
				int nSF = 0;
				for (nc = 0; nc < m_nSfCount; nc++) nSF += Set_STLCIncludeCS(arSfLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD, 0);
				if (nSF > 0)
				{
					strDesc.Format(_T("+%sSF"), GetFactorString(dFactor)); LcomD.Description += strDesc;
					if (nType == 2 || nType == 4) m_aAcdnLcomD.Add(LcomD);
					else                     Set_LoadComb4Civil(LcomD, D_STR, 0, nMsgType);
				}
			}

			if (m_nIpCount > 0) // Ice Pressure ±ùÑ¹Á¦
			{
				LcomD = aLcomD.GetAt(lc);
				LcomD.nActive = nType == 17 ? 4 : 1;
				if (m_nLcomType == D_LCOMTYPE_GENERAL) LcomD.nActive = 1;  // general
				LoadCaseNo = GetCountLcomCase(LcomD);
				int nIP = 0;
				for (nc = 0; nc < m_nIpCount; nc++) nIP += Set_STLCIncludeCS(arIpLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD, 0);
				if (nIP > 0)
				{
					strDesc.Format(_T("+%sIP"), GetFactorString(dFactor)); LcomD.Description += strDesc;
					if (nType == 2 || nType == 4) m_aAcdnLcomD.Add(LcomD);
					else                     Set_LoadComb4Civil(LcomD, D_STR, 0, nMsgType);
				}
			}

			// Temeperature ÎÂ¶ÈºÉÔØ
			if ((m_nTCount > 0 || m_nTpgCount > 0) || (bAddCsLoad && (CheckCSEL(D_SGLD_ER1_CVL, _LSX(T)) || CheckCSEL(D_SGLD_ER1_CVL, _LSX(TPG)))))
			{
				LcomD = aLcomD.GetAt(lc);
				LcomD.nActive = nType == 17 ? 4 : 1;
				if (m_nLcomType == D_LCOMTYPE_GENERAL) LcomD.nActive = 1;  // general
				SetCvlJTJD60_Capacity_TPG(LcomD, arTLoad, arTpgLoad, arFrLoad, dFactor, dFactor, FALSE, nType == 2 || nType == 4, nMsgType);
			}

			if (m_nFrCount > 0) // Friction  Ä¦²ÁÁ¦
			{
				LcomD = aLcomD.GetAt(lc);
				LcomD.nActive = nType == 17 ? 4 : 1;
				if (m_nLcomType == D_LCOMTYPE_GENERAL) LcomD.nActive = 1;  // general
				LoadCaseNo = GetCountLcomCase(LcomD);
				int nFR = 0;
				for (nc = 0; nc < m_nFrCount; nc++) nFR += Set_STLCIncludeCS(arFrLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD, 0);
				if (nFR > 0)
				{
					strDesc.Format(_T("+%sFR"), GetFactorString(dFactor)); LcomD.Description += strDesc;
					if (nType == 2 || nType == 4) m_aAcdnLcomD.Add(LcomD);
					else                     Set_LoadComb4Civil(LcomD, D_STR, 0, nMsgType);
				}
			}
		} // next m
	} // next lc

	// ¿µ±¸ÇÏÁß + Â÷·®ÇÏÁß + 1°³ÀÇ ±âÅ¸ °¡º¯ÇÏÁßÀÎ °æ¿ì(8°³)
	SetCvlESCGB19_Var1_M(arData, arMLoad, arLLoad, arIlLoad, arCrlLoad, arMCrlLoad, arCfLoad, arLsLoad, arWLoad, arWlLoad, arSfLoad, arIpLoad,
		arTLoad, arTpgLoad, arFrLoad, arBrkLoad, nType, nMsgType);
	// ¿µ±¸ÇÏÁß + Â÷·®ÇÏÁß + 2°³ÀÇ ±âÅ¸ °¡º¯ÇÏÁßÀÎ °æ¿ì(24°³)
	SetCvlESCGB19_Var2_M(arData, arMLoad, arLLoad, arIlLoad, arCrlLoad, arMCrlLoad, arCfLoad, arLsLoad, arWLoad, arWlLoad, arSfLoad, arIpLoad,
		arTLoad, arTpgLoad, arFrLoad, arBrkLoad, nType, nMsgType);
	// ¿µ±¸ÇÏÁß + Â÷·®ÇÏÁß + 3°³ÀÇ ±âÅ¸ °¡º¯ÇÏÁßÀÎ °æ¿ì(36°³)
	SetCvlESCGB19_Var3_M(arData, arMLoad, arLLoad, arIlLoad, arCrlLoad, arMCrlLoad, arCfLoad, arLsLoad, arWLoad, arWlLoad, arSfLoad, arIpLoad,
		arTLoad, arTpgLoad, arFrLoad, arBrkLoad, nType, nMsgType);
	// ¿µ±¸ÇÏÁß + Â÷·®ÇÏÁß + 4°³ÀÇ ±âÅ¸ °¡º¯ÇÏÁßÀÎ °æ¿ì(29°³)
	SetCvlESCGB19_Var4_M(arData, arMLoad, arLLoad, arIlLoad, arCrlLoad, arMCrlLoad, arCfLoad, arLsLoad, arWLoad, arWlLoad, arSfLoad, arIpLoad,
		arTLoad, arTpgLoad, arFrLoad, arBrkLoad, nType, nMsgType);
	// ¿µ±¸ÇÏÁß + Â÷·®ÇÏÁß + 5°³ ¹× 6°³ÀÇ ±âÅ¸ °¡º¯ÇÏÁßÀÎ °æ¿ì(12°³ ¹× 2°³)
	SetCvlESCGB19_Var56_M(arData, arMLoad, arLLoad, arIlLoad, arCrlLoad, arMCrlLoad, arCfLoad, arLsLoad, arWLoad, arWlLoad, arSfLoad, arIpLoad,
		arTLoad, arTpgLoad, arFrLoad, arBrkLoad, nType, nMsgType);

	return TRUE;
}


BOOL CLoadCombCtrl::SetCvlESCGB19_Capacity_A(const CArray<T_LCOM_D, T_LCOM_D&>& arData, ArINT& arLLoad, ArINT& arIlLoad,
	ArINT& arCfLoad, ArINT& arLsLoad, ArINT& arCrlLoad, ArINT& arWLoad, ArINT& arSfLoad,
	ArINT& arIpLoad, ArINT& arTLoad, ArINT& arTpgLoad, ArINT& arFrLoad, ArINT& arBrkLoad,
	ArUNT& arMLoad, ArINT& arMcrLoad, int nType, int nAcdType)
{
	// nType   0 or 4: °è¼öÁ¶ÇÕ, 1: 1.0(settlement Á¦¿Ü), 2: 1.0(¸ðµÎ), Live Load Á¦¿Ü
	CArray<T_LCOM_D, T_LCOM_D&> aLcomD; aLcomD.Copy(arData);
	int  nLcomSize = aLcomD.GetSize();
	int  nnn = 0, lc = 0;

	BOOL bErec = FALSE;
	for (int nErec = 0; nErec < D_STCT_EREC_MAX_CVL; ++nErec)
	{
		if (m_nConstLoad != 0 && CheckCS_SgldK(D_SGLD_ER1_CVL + nErec, nnn)) bErec = TRUE;
	}

	BOOL bAddCsLoad = bErec;
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER1_CVL,nnn)) || 
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER2_CVL,nnn)) || 
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER3_CVL,nnn)) ||
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER4_CVL,nnn)) ||
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER5_CVL,nnn)) ||
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER6_CVL,nnn)) ||
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER7_CVL,nnn)) ||
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER8_CVL,nnn)) ||
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER9_CVL,nnn)) ||
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER10_CVL,nnn)); // CS or ST+CS

		// ÓÀ¾Ã×÷ÓÃ×éºÏ
	for (lc = 0; lc < nLcomSize; lc++)
	{
		T_LCOM_D LcomD = aLcomD.GetAt(lc);
		LcomD.nActive = 6; // Active or Strength/Stress
		if (m_nLcomType == D_LCOMTYPE_GENERAL) LcomD.nActive = 1;  // general
		if (nType == 2 || nType == 4) m_aAcdnLcomD.Add(LcomD);
		else                     Set_LoadComb4Civil(LcomD, D_STR, 0, 112); // »ù±¾×éºÏ£¨ÓÀ¾Ã×÷ÓÃ£©
	}
	if ((nType == 2 || nType == 4) && (m_nLsCount == 0 && m_nWCount == 0 && m_nSfCount == 0 &&
		m_nIpCount == 0 && m_nTCount == 0 && m_nTpgCount == 0 && m_nFrCount == 0))
	{
		for (lc = 0; lc < nLcomSize; lc++) m_aAcdnLcomD.Add(aLcomD.GetAt(lc));
		return TRUE;
	}

	double dFactor = 1.0;
	double dMLFactor = 1.0;       // Æû³µºÉÔØÏµÊý
	double dCrlFactor = 1.0;      // ÈËÈººÉÔØÏµÊý
	double dWFactor = 0.75;       // ·çºÉÔØÏµÊý
	double dTgpFactor = 0.8;      //ÎÂ¶ÈÌÝ¶ÈºÉÔØÏµÊý
	if (nAcdType == 0)//ÆµÓöÏµÊý modified this statement by gongxing at 2016-04-13
	{
		dMLFactor = 0.7;
		dCrlFactor = 1.0;
	}
	else if (nAcdType == 1)//×¼ÓÀ¾ÃÏµÊý
	{
		dMLFactor = 0.4;
		dCrlFactor = 0.4;
	}

	// 	double dFactor  = (nType==0 || nType==4) ? 1.4:1.0;
	// 	double dWFactor = (nType==0 || nType==4) ? 1.1:1.0;

	double dIL = (nType == 12) ? 0. : CalcMovingILFactor();//µ±²»Îª±ê×¼×éºÏÊ±£¬¼ÆËã³å»÷ÏµÊý


	int nMsgType = (nType == 0 || nType == 1) ? 114 : 115; // 114 : »ù±¾×éºÏ, 115 : Å¼È»×éºÏ
	// ÓÀ¾Ã×÷ÓÃ + 1¿É±ä×÷ÓÃ
	CString strDesc = _T("");
	int LoadCaseNo = 0, nc = 0;
	for (lc = 0; lc < nLcomSize; lc++)
	{
		int nMCount = max(m_nMCount + 1, 1);
		for (int m = 0;m < nMCount;m++)//Æû³µºÉÔØ
		{
			T_LCOM_D LcomD = aLcomD.GetAt(lc);
			LcomD.nActive = 6; // Active or Strength/Stress
			if (m_nLcomType == D_LCOMTYPE_GENERAL) LcomD.nActive = 1;  // general
			LoadCaseNo = GetCountLcomCase(LcomD);

			BOOL bAddLcomD = FALSE;
			if (m < m_nMCount)
			{
				Set_MovingCase(arMLoad.GetAt(m), dMLFactor, LoadCaseNo, LcomD, TRUE);
				strDesc.Format(_T("+%sM"), GetFactorString(dMLFactor)); LcomD.Description += strDesc;

				if (m_nMCount > 1) { strDesc.Format(_T("[%d]"), m + 1); LcomD.Description += strDesc; }
				if (nType == 2 || nType == 4) m_aAcdnLcomD.Add(LcomD);
				else                     Set_LoadComb4Civil(LcomD, D_STR, 0, nMsgType);
			}

			int nMcrCount = max(m_nMCrlCount + 1, 1);
			for (int nMcr = 0;nMcr < nMcrCount;nMcr++)//ÈËÈººÉÔØ
			{
				if (nMcr < m_nMCrlCount)
				{
					Set_MovingCase(arMcrLoad.GetAt(nMcr), dCrlFactor, LoadCaseNo, LcomD, TRUE);
					strDesc.Format(_T("+%sMcr"), GetFactorString(dCrlFactor)); LcomD.Description += strDesc;
					if (m_nMCrlCount > 1) { strDesc.Format(_T("[%d]"), nMcr + 1);LcomD.Description += strDesc; }
					if (nType == 2 || nType == 4) m_aAcdnLcomD.Add(LcomD);
					else                     Set_LoadComb4Civil(LcomD, D_STR, 0, nMsgType);
				}

				//»îºÉÔØ
				int nLL = 0, nIL = 0, nCF = 0;
				if (m_nLCount > 0 || (bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(L)))) // ST or ST+CS
				{
					for (nc = 0; nc < m_nLCount; nc++) nLL += Set_STLCIncludeCS(arLLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD, 0);
					if (nLL > 0) { strDesc.Format(_T("+%sL"), GetFactorString(dFactor)); LcomD.Description += strDesc; }
					if (bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(L)))
					{
						Set_CStageCase(m_aCSLoad.GetAt(nnn), dFactor, LoadCaseNo, LcomD, TRUE);
						strDesc.Format(_T("+%s(cEL)"), GetFactorString(dFactor)); LcomD.Description += strDesc;
						bAddLcomD = TRUE;
					}
					for (nc = 0; nc < m_nIlCount; nc++)
						nIL += Set_STLCIncludeCS(arIlLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD, 0);
					if (nIL > 0) { strDesc.Format(_T("+%sIL"), GetFactorString(dFactor)); LcomD.Description += strDesc; }
					if (nLL + nIL > 0)
					{
						if (nType == 2 || nType == 4) m_aAcdnLcomD.Add(LcomD);
						else                     Set_LoadComb4Civil(LcomD, D_STR, 0, nMsgType);
					}
				}

				// ???
				if (m_nMCount > 0 || (m_nLCount > 0 || (bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(L)))))
				{
					for (nc = 0; nc < m_nCfCount; nc++) nCF += Set_STLCIncludeCS(arCfLoad.GetAt(nc), dMLFactor, LoadCaseNo, LcomD, 0);
					if (nCF > 0) { strDesc.Format(_T("+%sCF"), GetFactorString(dMLFactor)); LcomD.Description += strDesc; }
					if (nType == 2 || nType == 4) m_aAcdnLcomD.Add(LcomD);
					else                     Set_LoadComb4Civil(LcomD, D_STR, 0, nMsgType);
				}

				if (m_nLsCount > 0) // Live Load Surcharge µØÃæ¸½¼Ó»îºÉÔØ
				{
					LcomD = aLcomD.GetAt(lc);
					LcomD.nActive = 6;
					if (m_nLcomType == D_LCOMTYPE_GENERAL) LcomD.nActive = 1;  // general
					LoadCaseNo = GetCountLcomCase(LcomD);
					int nLS = 0;
					for (nc = 0; nc < m_nLsCount; nc++) nLS += Set_STLCIncludeCS(arLsLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD, 0);
					if (nLS > 0)
					{
						strDesc.Format(_T("+%sLS"), GetFactorString(dFactor)); LcomD.Description += strDesc;
						if (nType == 2 || nType == 4) m_aAcdnLcomD.Add(LcomD);
						else                     Set_LoadComb4Civil(LcomD, D_STR, 0, nMsgType);
					}
				}
				if (nType != 2 && m_nCrlCount > 0) // Crowd Load ÈËÈººÉÔØ
				{
					LcomD = aLcomD.GetAt(lc);
					LcomD.nActive = 6;
					if (m_nLcomType == D_LCOMTYPE_GENERAL) LcomD.nActive = 1;  // general
					LoadCaseNo = GetCountLcomCase(LcomD);
					int nCRL = 0;
					for (nc = 0; nc < m_nCrlCount; nc++) nCRL += Set_STLCIncludeCS(arCrlLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD, 0);
					if (nCRL > 0)
					{
						strDesc.Format(_T("+%sCRL"), GetFactorString(dFactor)); LcomD.Description += strDesc;
						if (nType == 2 || nType == 4) m_aAcdnLcomD.Add(LcomD);
						else                     Set_LoadComb4Civil(LcomD, D_STR, 0, nMsgType);
					}
				}

				int nWCount = m_nWCount * 2; // MNET:1614, KJH20050808 ·çºÉÔØ
				if (m_nWCount == 0 && bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(W))) nWCount = 1;
				for (nc = 0; nc < nWCount; nc++)
				{
					LcomD = aLcomD.GetAt(lc);
					LcomD.nActive = 6;
					if (m_nLcomType == D_LCOMTYPE_GENERAL) LcomD.nActive = 1;  // general
					LoadCaseNo = GetCountLcomCase(LcomD);
					if (m_nWCount > 0)
					{
						double dWF = (nc % 2 == 0) ? dWFactor : -1. * dWFactor;
						if (Set_STLCIncludeCS(arWLoad.GetAt(nc / 2), dWF, LoadCaseNo, LcomD, 0) > 0)
						{
							CString strW = (dWF > 0.0) ? _T("+%sW") : _T("%sW");
							strDesc.Format(strW, GetFactorString(dWF)); LcomD.Description += strDesc;
							if (m_nWCount > 1) { strDesc.Format(_T("[%d]"), nc / 2 + 1); LcomD.Description += strDesc; }
						}
					}
					if (bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(W)))
					{
						Set_CStageCase(m_aCSLoad.GetAt(nnn), dWFactor, LoadCaseNo, LcomD, TRUE);
						strDesc.Format(_T("+%s(cEL)"), GetFactorString(dWFactor)); LcomD.Description += strDesc;
					}
					if (nType == 2 || nType == 4) m_aAcdnLcomD.Add(LcomD);
					else                     Set_LoadComb4Civil(LcomD, D_STR, 0, nMsgType);
				}
				if (m_nSfCount > 0) // Stream Flow Pressure Á÷Ë®Ñ¹Á¦
				{
					LcomD = aLcomD.GetAt(lc);
					LcomD.nActive = 6;
					if (m_nLcomType == D_LCOMTYPE_GENERAL) LcomD.nActive = 1;  // general
					LoadCaseNo = GetCountLcomCase(LcomD);
					int nSF = 0;
					for (nc = 0; nc < m_nSfCount; nc++) nSF += Set_STLCIncludeCS(arSfLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD, 0);
					if (nSF > 0)
					{
						strDesc.Format(_T("+%sSF"), GetFactorString(dFactor)); LcomD.Description += strDesc;
						if (nType == 2 || nType == 4) m_aAcdnLcomD.Add(LcomD);
						else                     Set_LoadComb4Civil(LcomD, D_STR, 0, nMsgType);
					}
				}
				if (m_nIpCount > 0) // Ice Pressure ±ùÑ¹Á¦
				{
					LcomD = aLcomD.GetAt(lc);
					LcomD.nActive = 6;
					if (m_nLcomType == D_LCOMTYPE_GENERAL) LcomD.nActive = 1;  // general
					LoadCaseNo = GetCountLcomCase(LcomD);
					int nIP = 0;
					for (nc = 0; nc < m_nIpCount; nc++) nIP += Set_STLCIncludeCS(arIpLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD, 0);
					if (nIP > 0)
					{
						strDesc.Format(_T("+%sIP"), GetFactorString(dFactor)); LcomD.Description += strDesc;
						if (nType == 2 || nType == 4) m_aAcdnLcomD.Add(LcomD);
						else                     Set_LoadComb4Civil(LcomD, D_STR, 0, nMsgType);
					}
				}
				// Temeperature ÎÂ¶ÈºÉÔØ
				if ((m_nTCount > 0 || m_nTpgCount > 0) || (bAddCsLoad && (CheckCSEL(D_SGLD_ER1_CVL, _LSX(T)) || CheckCSEL(D_SGLD_ER1_CVL, _LSX(TPG)))))
				{
					LcomD = aLcomD.GetAt(lc);
					LcomD.nActive = 6;
					if (m_nLcomType == D_LCOMTYPE_GENERAL) LcomD.nActive = 1;  // general
					SetCvlJTJD60_Capacity_TPG(LcomD, arTLoad, arTpgLoad, arFrLoad, dFactor, dFactor, FALSE, nType == 2 || nType == 4, nMsgType);
				}
				if (m_nFrCount > 0) // Friction Ä¦²ÁÁ¦
				{
					LcomD = aLcomD.GetAt(lc);
					LcomD.nActive = 6;
					if (m_nLcomType == D_LCOMTYPE_GENERAL) LcomD.nActive = 1;  // general
					LoadCaseNo = GetCountLcomCase(LcomD);
					int nFR = 0;
					for (nc = 0; nc < m_nFrCount; nc++) nFR += Set_STLCIncludeCS(arFrLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD, 0);
					if (nFR > 0)
					{
						strDesc.Format(_T("+%sFR"), GetFactorString(dFactor)); LcomD.Description += strDesc;
						if (nType == 2 || nType == 4) m_aAcdnLcomD.Add(LcomD);
						else                     Set_LoadComb4Civil(LcomD, D_STR, 0, nMsgType);
					}
				}

			}
		}
	}

	//if((m_nConstLoad!=1 && m_nLCount>0) || (m_nConstLoad==1 && CheckCSEL(D_SGLD_ER1_CVL, _LSX(L))) || nType==2)
	{
		// ÓÀ¾Ã×÷ÓÃ + ÒÆ¶¯ºÉÔØ + 1¿É±ä×÷ÓÃ
		SetCvlESCGB19_Var1_A(arData, arLLoad, arIlLoad, arCfLoad, arLsLoad, arCrlLoad,
			arWLoad, arSfLoad, arIpLoad, arTLoad, arTpgLoad, arFrLoad, arBrkLoad, arMLoad, arMcrLoad, nType, nMsgType, nAcdType);//modified this line by gongxing at 2016-04-13
		// ÓÀ¾Ã×÷ÓÃ + ÒÆ¶¯ºÉÔØ + 2¿É±ä×÷ÓÃ
		SetCvlESCGB19_Var2_A(arData, arLLoad, arIlLoad, arCfLoad, arLsLoad, arCrlLoad,
			arWLoad, arSfLoad, arIpLoad, arTLoad, arTpgLoad, arFrLoad, arBrkLoad, arMLoad, arMcrLoad, nType, nMsgType, nAcdType);//modified this line by gongxing at 2016-04-13
		// ÓÀ¾Ã×÷ÓÃ + ÒÆ¶¯ºÉÔØ + 3¿É±ä×÷ÓÃ
		SetCvlESCGB19_Var3_A(arData, arLLoad, arIlLoad, arCfLoad, arLsLoad, arCrlLoad,
			arWLoad, arSfLoad, arIpLoad, arTLoad, arTpgLoad, arFrLoad, arBrkLoad, arMLoad, arMcrLoad, nType, nMsgType, nAcdType);//modified this line by gongxing at 2016-04-13
		// ÓÀ¾Ã×÷ÓÃ + ÒÆ¶¯ºÉÔØ + 4¿É±ä×÷ÓÃ
		SetCvlESCGB19_Var4_A(arData, arLLoad, arIlLoad, arCfLoad, arLsLoad, arCrlLoad,
			arWLoad, arSfLoad, arIpLoad, arTLoad, arTpgLoad, arFrLoad, arBrkLoad, arMLoad, arMcrLoad, nType, nMsgType, nAcdType);//modified this line by gongxing at 2016-04-13
		// ÓÀ¾Ã×÷ÓÃ + ÒÆ¶¯ºÉÔØ + 5.6¿É±ä×÷ÓÃ
		SetCvlESCGB19_Var56_A(arData, arLLoad, arIlLoad, arCfLoad, arLsLoad, arCrlLoad,
			arWLoad, arSfLoad, arIpLoad, arTLoad, arTpgLoad, arFrLoad, arBrkLoad, arMLoad, arMcrLoad, nType, nMsgType, nAcdType);//modified this line by gongxing at 2016-04-13
	}

	return TRUE;
}

BOOL CLoadCombCtrl::SetCvlESCGB19_Service_New(const CArray<T_LCOM_D, T_LCOM_D&>& arData, ArUNT& arMLoad,
	ArINT& arLLoad, ArINT& arIlLoad, ArINT& arCrlLoad, ArINT& arMCrlLoad,
	ArINT& arCfLoad, ArINT& arLsLoad, ArINT& arWLoad, ArINT& arWlLoad, ArINT& arSfLoad, ArINT& arIpLoad,
	ArINT& arTLoad, ArINT& arTpgLoad, ArINT& arFrLoad, ArINT& arBrkLoad, ArINT& arIlpLoad, ArINT& arCoLoad, int nType)
{
	// nType == 10:×¼ÓÀ¾Ã, 11:ÆµÓö 12:µ¯ÐÔ½×¶Î
	CArray<T_LCOM_D, T_LCOM_D&> aLcomD;
	aLcomD.Copy(arData);
	int  nLcomSize = aLcomD.GetSize();
	int  nnn = 0;

	BOOL bErec = FALSE;
	for (int nErec = 0; nErec < D_STCT_EREC_MAX_CVL; ++nErec)
	{
		if (m_nConstLoad != 0 && CheckCS_SgldK(D_SGLD_ER1_CVL + nErec, nnn)) bErec = TRUE;
	}

	BOOL bAddCsLoad = bErec;
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER1_CVL,nnn)) || 
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER2_CVL,nnn)) || 
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER3_CVL,nnn)) ||
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER4_CVL,nnn)) ||
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER5_CVL,nnn)) ||
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER6_CVL,nnn)) ||
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER7_CVL,nnn)) ||
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER8_CVL,nnn)) ||
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER9_CVL,nnn)) ||
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER10_CVL,nnn));// CS or ST+CS;

	int nServEnvType = (nType == 12) ? 1 : 0;
	// ¿µ±¸ÇÏÁß¸¸ Á¶ÇÕ(±ØÇÑ»óÅÂÀÇ ´Ü±âÁ¶ÇÕ¸¸¿¡¼­¸¸..)
	if (nType == 11)//¶ÔÓ¦ÆµÓö×éºÏ
	{
		for (int lc = 0; lc < nLcomSize; lc++)
		{
			T_LCOM_D LcomD = aLcomD.GetAt(lc);
			LcomD.nActive = 1; // Active or Strength/Stress
			if (m_nLcomType == D_LCOMTYPE_CONCRETE || m_nLcomType == D_LCOMTYPE_STLCOMP || m_nLcomType == D_LCOMTYPE_SRC)  // serviceability
			{
				LcomD.nActive = 7;
			}
			//Set_LoadComb4Civil(LcomD, D_SER, nServEnvType, 116); // serviceablilty, 116:¼«¶Ë×éºÏ(ÓÀ¾Ã×÷ÓÃ)
		}
	}

	double dLFactor = 1.0, dCRLFactor = 1.0;
	double dFactor = 1.0, dTPGFactor = 1.0;
	double dWFactor = 1.0;
	double dWlFactor = 1.0;
	double dCfFactor = 1.0;         // ÀëÐÄÁ¦¡¢»îºÉÔØ³å»÷¡¢³¬ÔØ³å»÷Á¦¡¢³å»÷Á¦ ÏµÊý
	if (nType == 10) { dLFactor = 0.4, dWFactor = 0.75, dWlFactor = 1.0, dTPGFactor = 0.8, dCRLFactor = 0.4; }
	else if (nType == 11) { dLFactor = 0.7, dWFactor = 0.75, dWlFactor = 1.0, dTPGFactor = 0.8, dCfFactor = 0.7, dCRLFactor = 1.0; }//modified by gongxing at 2016-01-22

	BOOL bUseWlLoad = TRUE;

	int nMsgType = 119; // µ¯ÐÔ½×¶ÎÓ¦Á¦
	if (nType == 10) nMsgType = 117; // ×¼ÓÀ¾Ã
	else if (nType == 11) nMsgType = 118; // ÆµÓö

	double dIL = (nType == 12) ? 0. : CalcMovingILFactor();//Ó¦Á¦ÑéËãÊ±£¬ÐèÌáÈ¡³å»÷ÏµÊý

	// ÓÀ¾Ã×÷ÓÃ + 1¿É±ä×÷ÓÃ
	CString strDesc = _T("");
	int LoadCaseNo = 0, nc = 0;
	for (int lc = 0; lc < nLcomSize; lc++)
	{
		int nMCount = max(m_nMCount, 1);
		for (int m = 0; m < nMCount; m++)
		{
			T_LCOM_D LcomD = aLcomD.GetAt(lc); LcomD.nActive = 1; // Active or Strength/Stress
			if (m_nLcomType == D_LCOMTYPE_CONCRETE)
			{
				if (nType == 10) LcomD.nActive = 8;  //×¼ÓÀ¾Ã×éºÏ
				if (nType == 11) LcomD.nActive = 7;  //ÆµÓö×éºÏ
				if (nType == 12)
				{
					LcomD.bElasticStage = TRUE;
					LcomD.nActive = 2;//±ê×¼×éºÏ
				}
			}
			else if (m_nLcomType == D_LCOMTYPE_STLCOMP || m_nLcomType == D_LCOMTYPE_SRC)
			{
				if (nType == 10) LcomD.nActive = 8;  //×¼ÓÀ¾Ã×éºÏ
				if (nType == 11) LcomD.nActive = 7;  //ÆµÓö×éºÏ
			}
			LoadCaseNo = GetCountLcomCase(LcomD);

			BOOL bAddLcomD = FALSE;

			// moving load
			if (m_nMCount > 0)
			{
				Set_MovingCase(arMLoad.GetAt(m), dLFactor / (1. + dIL), LoadCaseNo, LcomD, TRUE);
				if (nType == 12 || dIL < m_dZeroLim) strDesc.Format(_T("+%sM"), GetFactorString(dLFactor));
				else                            strDesc.Format(_T("+%s/(1+mu)M"), GetFactorString(dLFactor));
				LcomD.Description += strDesc;
				if (m_nMCount > 1) { strDesc.Format(_T("[%d]"), m + 1); LcomD.Description += strDesc; }
				bAddLcomD = TRUE;
			}

			if (m_nLCount > 0 || (bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(L)))) // ST or ST+CS
			{
				int nLL = 0;
				for (nc = 0; nc < m_nLCount; nc++)
					nLL += Set_STLCIncludeCS(arLLoad.GetAt(nc), dLFactor, LoadCaseNo, LcomD, 0);
				if (nLL > 0) { strDesc.Format(_T("+%sL"), GetFactorString(dLFactor)); LcomD.Description = LcomD.Description + strDesc; }
				if (bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(L)))
				{
					Set_CStageCase(m_aCSLoad.GetAt(nnn), dLFactor, LoadCaseNo, LcomD, TRUE);
					strDesc.Format(_T("+%s(cEL)"), GetFactorString(dLFactor)); LcomD.Description = LcomD.Description + strDesc;
					bAddLcomD = TRUE;
				}
				int nIL = 0;
				for (nc = 0; nc < m_nIlCount; nc++)
					nIL += Set_STLCIncludeCS(arIlLoad.GetAt(nc), dLFactor, LoadCaseNo, LcomD, 0);
				if (nIL > 0) { strDesc.Format(_T("+%sIL"), GetFactorString(dLFactor)); LcomD.Description = LcomD.Description + strDesc; }
				if (nLL + nIL > 0) bAddLcomD = TRUE;
			}

			if (m_nMCount > 0 || (m_nLCount > 0 || (bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(L)))))
			{
				int nCF = 0, nIL = 0, nILP = 0, nCO = 0;
				for (nc = 0; nc < m_nCfCount; nc++)    // ÀëÐÄÁ¦
					nCF += Set_STLCIncludeCS(arCfLoad.GetAt(nc), dCfFactor, LoadCaseNo, LcomD, 0);
				if (nCF > 0) { strDesc.Format(_T("+%sCF"), GetFactorString(dCfFactor)); LcomD.Description = LcomD.Description + strDesc; }
				for (nc = 0; nc < m_nIlCount; nc++)    // »îºÉÔØ³å»÷
					nIL += Set_STLCIncludeCS(arIlLoad.GetAt(nc), dCfFactor, LoadCaseNo, LcomD, 0);
				if (nIL > 0) { strDesc.Format(_T("+%sIL"), GetFactorString(dCfFactor)); LcomD.Description = LcomD.Description + strDesc; }
				for (nc = 0; nc < m_nIlpCount; nc++)    // ³¬ÔØ³å»÷Á¦
					nILP += Set_STLCIncludeCS(arIlpLoad.GetAt(nc), dCfFactor, LoadCaseNo, LcomD, 0);
				if (nILP > 0) { strDesc.Format(_T("+%sCF"), GetFactorString(dCfFactor)); LcomD.Description = LcomD.Description + strDesc; }
				for (nc = 0; nc < m_nCoCount; nc++)    // ³å»÷Á¦
					nCO += Set_STLCIncludeCS(arCoLoad.GetAt(nc), dCfFactor, LoadCaseNo, LcomD, 0);
				if (nCO > 0) { strDesc.Format(_T("+%sCO"), GetFactorString(dCfFactor)); LcomD.Description = LcomD.Description + strDesc; }
				if (nCF + nIL + nILP + nCO > 0) bAddLcomD = TRUE;
			}
			//if(bAddLcomD) Set_LoadComb4Civil(LcomD, D_SER, nServEnvType, nMsgType);

			if (m_nLsCount > 0) // Live Load Surcharge µØÃæ¸½¼Ó»îºÉÔØ
			{
				LcomD = aLcomD.GetAt(lc); LcomD.nActive = 1; LoadCaseNo = GetCountLcomCase(LcomD);
				if (m_nLcomType == D_LCOMTYPE_CONCRETE)
				{
					if (nType == 10) LcomD.nActive = 8;  //×¼ÓÀ¾Ã×éºÏ
					if (nType == 11) LcomD.nActive = 7;  //ÆµÓö×éºÏ
					if (nType == 12)
					{
						LcomD.bElasticStage = TRUE;
						LcomD.nActive = 2;//±ê×¼×éºÏ
					}
				}
				else if (m_nLcomType == D_LCOMTYPE_STLCOMP || m_nLcomType == D_LCOMTYPE_SRC)
				{
					if (nType == 10) LcomD.nActive = 8;  //×¼ÓÀ¾Ã×éºÏ
					if (nType == 11) LcomD.nActive = 7;  //ÆµÓö×éºÏ
				}
				int nLS = 0;
				for (nc = 0; nc < m_nLsCount; nc++) nLS += Set_STLCIncludeCS(arLsLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD, 0);
				if (nLS > 0)
				{
					strDesc.Format(_T("+%sLS"), GetFactorString(dFactor)); LcomD.Description += strDesc;
					Set_LoadComb4Civil(LcomD, D_SER, nServEnvType, nMsgType);
				}
			}
			// MNET:XXXX-BJLEE(BBONG)-20060503

			if (m_nCrlCount > 0 || m_nMCrlCount > 0) // Crowd Load ÈËÈººÉÔØ
			{
				LcomD = aLcomD.GetAt(lc); LcomD.nActive = 1; LoadCaseNo = GetCountLcomCase(LcomD);
				if (m_nLcomType == D_LCOMTYPE_CONCRETE)
				{
					if (nType == 10) LcomD.nActive = 8;  //×¼ÓÀ¾Ã×éºÏ
					if (nType == 11) LcomD.nActive = 7;  //ÆµÓö×éºÏ
					if (nType == 12)
					{
						LcomD.bElasticStage = TRUE;
						LcomD.nActive = 2;//±ê×¼×éºÏ
					}
				}
				else if (m_nLcomType == D_LCOMTYPE_STLCOMP || m_nLcomType == D_LCOMTYPE_SRC)
				{
					if (nType == 10) LcomD.nActive = 8;  //×¼ÓÀ¾Ã×éºÏ
					if (nType == 11) LcomD.nActive = 7;  //ÆµÓö×éºÏ
				}
				int nCRL = 0;
				for (nc = 0; nc < m_nCrlCount; nc++) nCRL += Set_STLCIncludeCS(arCrlLoad.GetAt(nc), dCRLFactor, LoadCaseNo, LcomD, 0);
				if (nCRL > 0) { strDesc.Format(_T("+%sCRL"), GetFactorString(dCRLFactor)); LcomD.Description += strDesc; }

				int nMCrlCount = (m_nMCrlCount > 0) ? m_nMCrlCount : ((nCRL > 0) ? 1 : 0);
				for (int crl = 0; crl < nMCrlCount; ++crl)
				{
					T_LCOM_D LcomD_crl = LcomD; LoadCaseNo = GetCountLcomCase(LcomD_crl);
					if (m_nMCrlCount > 0)
					{
						Set_MovingCase(arMCrlLoad.GetAt(crl), dCRLFactor, LoadCaseNo, LcomD_crl, TRUE);
						strDesc.Format(_T("+%sMCRL"), GetFactorString(dCRLFactor)); LcomD_crl.Description += strDesc;
						if (m_nMCrlCount > 1) { strDesc.Format(_T("[%d]"), crl + 1); LcomD_crl.Description += strDesc; }
					}
					if (nCRL > 0 || m_nMCrlCount > 0)
					{
						if (nType == 2 || nType == 4) m_aAcdnLcomD.Add(LcomD_crl);
						else                     Set_LoadComb4Civil(LcomD_crl, D_SER, nServEnvType, nMsgType);
					}
				}
			}

			if (bUseWlLoad)
			{
				int nWCount = m_nWlCount * 2; // MNET:1614, KJH20050808 ·çºÉÔØ
				if (m_nWlCount == 0 && bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(WL))) nWCount = 1;
				for (nc = 0; nc < nWCount; nc++)
				{
					LcomD = aLcomD.GetAt(lc); LcomD.nActive = 1; LoadCaseNo = GetCountLcomCase(LcomD);
					if (m_nLcomType == D_LCOMTYPE_CONCRETE)
					{
						if (nType == 10) LcomD.nActive = 8;  //×¼ÓÀ¾Ã×éºÏ
						if (nType == 11) LcomD.nActive = 7;  //ÆµÓö×éºÏ
						if (nType == 12)
						{
							LcomD.bElasticStage = TRUE;
							LcomD.nActive = 2;//±ê×¼×éºÏ
						}
					}
					else if (m_nLcomType == D_LCOMTYPE_STLCOMP || m_nLcomType == D_LCOMTYPE_SRC)
					{
						if (nType == 10) LcomD.nActive = 8;  //×¼ÓÀ¾Ã×éºÏ
						if (nType == 11) LcomD.nActive = 7;  //ÆµÓö×éºÏ
					}
					if (m_nWlCount > 0)
					{
						double dWF = (nc % 2 == 0) ? dWlFactor : -1.*dWlFactor;
						if (Set_STLCIncludeCS(arWlLoad.GetAt(nc / 2), dWF, LoadCaseNo, LcomD, 0) > 0)
						{
							CString strW = (dWF > 0.0) ? _T("+%sWL") : _T("%sWL");
							strDesc.Format(strW, GetFactorString(dWF)); LcomD.Description += strDesc;
							if (m_nWlCount > 1) { strDesc.Format(_T("[%d]"), nc / 2 + 1); LcomD.Description += strDesc; }
						}
					}
					if (bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(WL)))
					{
						Set_CStageCase(m_aCSLoad.GetAt(nnn), dWlFactor, LoadCaseNo, LcomD, TRUE);
						strDesc.Format(_T("+%s(cEL)"), GetFactorString(dWlFactor)); LcomD.Description += strDesc;
					}
					Set_LoadComb4Civil(LcomD, D_SER, nServEnvType, nMsgType);
				}
			}
			else
			{
				int nWCount = m_nWCount * 2; // MNET:1614, KJH20050808 ·çºÉÔØ
				if (m_nWCount == 0 && bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(W))) nWCount = 1;
				for (nc = 0; nc < nWCount; nc++)
				{
					LcomD = aLcomD.GetAt(lc); LcomD.nActive = 1; LoadCaseNo = GetCountLcomCase(LcomD);
					if (m_nLcomType == D_LCOMTYPE_CONCRETE)
					{
						if (nType == 10) LcomD.nActive = 8;  //×¼ÓÀ¾Ã×éºÏ
						if (nType == 11) LcomD.nActive = 7;  //ÆµÓö×éºÏ
						if (nType == 12)
						{
							LcomD.bElasticStage = TRUE;
							LcomD.nActive = 2;//±ê×¼×éºÏ
						}
					}
					else if (m_nLcomType == D_LCOMTYPE_STLCOMP || m_nLcomType == D_LCOMTYPE_SRC)
					{
						if (nType == 10) LcomD.nActive = 8;  //×¼ÓÀ¾Ã×éºÏ
						if (nType == 11) LcomD.nActive = 7;  //ÆµÓö×éºÏ
					}
					if (m_nWCount > 0)
					{
						double dWF = (nc % 2 == 0) ? dWFactor : -1.*dWFactor;
						if (Set_STLCIncludeCS(arWLoad.GetAt(nc / 2), dWF, LoadCaseNo, LcomD, 0) > 0)
						{
							CString strW = (dWF > 0.0) ? _T("+%sW") : _T("%sW");
							strDesc.Format(strW, GetFactorString(dWF)); LcomD.Description += strDesc;
							if (m_nWCount > 1) { strDesc.Format(_T("[%d]"), nc / 2 + 1); LcomD.Description += strDesc; }
						}
					}
					if (bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(W)))
					{
						Set_CStageCase(m_aCSLoad.GetAt(nnn), dWFactor, LoadCaseNo, LcomD, TRUE);
						strDesc.Format(_T("+%s(cEL)"), GetFactorString(dWFactor)); LcomD.Description += strDesc;
					}
					Set_LoadComb4Civil(LcomD, D_SER, nServEnvType, nMsgType);
				}
			}
			if (m_nSfCount > 0) // Stream Flow Pressure Á÷Ë®Ñ¹Á¦
			{
				LcomD = aLcomD.GetAt(lc); LcomD.nActive = 1; LoadCaseNo = GetCountLcomCase(LcomD);
				if (m_nLcomType == D_LCOMTYPE_CONCRETE)
				{
					if (nType == 10) LcomD.nActive = 8;  //×¼ÓÀ¾Ã×éºÏ
					if (nType == 11) LcomD.nActive = 7;  //ÆµÓö×éºÏ
					if (nType == 12)
					{
						LcomD.bElasticStage = TRUE;
						LcomD.nActive = 2;//±ê×¼×éºÏ
					}
				}
				else if (m_nLcomType == D_LCOMTYPE_STLCOMP || m_nLcomType == D_LCOMTYPE_SRC)
				{
					if (nType == 10) LcomD.nActive = 8;  //×¼ÓÀ¾Ã×éºÏ
					if (nType == 11) LcomD.nActive = 7;  //ÆµÓö×éºÏ
				}
				int nSF = 0;
				for (nc = 0; nc < m_nSfCount; nc++) nSF += Set_STLCIncludeCS(arSfLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD, 0);
				if (nSF > 0)
				{
					strDesc.Format(_T("+%sSF"), GetFactorString(dFactor)); LcomD.Description += strDesc;
					Set_LoadComb4Civil(LcomD, D_SER, nServEnvType, nMsgType);
				}
			}
			if (m_nIpCount > 0) // Ice Pressure ±ùÑ¹Á¦
			{
				LcomD = aLcomD.GetAt(lc); LcomD.nActive = 1; LoadCaseNo = GetCountLcomCase(LcomD);
				if (m_nLcomType == D_LCOMTYPE_CONCRETE)
				{
					if (nType == 10) LcomD.nActive = 8;  //×¼ÓÀ¾Ã×éºÏ
					if (nType == 11) LcomD.nActive = 7;  //ÆµÓö×éºÏ
					if (nType == 12)
					{
						LcomD.bElasticStage = TRUE;
						LcomD.nActive = 2;//±ê×¼×éºÏ
					}
				}
				else if (m_nLcomType == D_LCOMTYPE_STLCOMP || m_nLcomType == D_LCOMTYPE_SRC)
				{
					if (nType == 10) LcomD.nActive = 8;  //×¼ÓÀ¾Ã×éºÏ
					if (nType == 11) LcomD.nActive = 7;  //ÆµÓö×éºÏ
				}
				int nIP = 0;
				for (nc = 0; nc < m_nIpCount; nc++) nIP += Set_STLCIncludeCS(arIpLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD, 0);
				if (nIP > 0)
				{
					strDesc.Format(_T("+%sIP"), GetFactorString(dFactor)); LcomD.Description += strDesc;
					Set_LoadComb4Civil(LcomD, D_SER, nServEnvType, nMsgType);
				}
			}
			// Temeperature ÎÂ¶ÈºÉÔØ
			if ((m_nTCount > 0 || m_nTpgCount > 0) || (bAddCsLoad && (CheckCSEL(D_SGLD_ER1_CVL, _LSX(T)) || CheckCSEL(D_SGLD_ER1_CVL, _LSX(TPG)))))
			{
				LcomD = aLcomD.GetAt(lc); LcomD.nActive = 1;
				if (m_nLcomType == D_LCOMTYPE_CONCRETE)
				{
					if (nType == 10) LcomD.nActive = 8;  //×¼ÓÀ¾Ã×éºÏ
					if (nType == 11) LcomD.nActive = 7;  //ÆµÓö×éºÏ
					if (nType == 12)
					{
						LcomD.bElasticStage = TRUE;
						LcomD.nActive = 2;//±ê×¼×éºÏ
					}
				}
				else if (m_nLcomType == D_LCOMTYPE_STLCOMP || m_nLcomType == D_LCOMTYPE_SRC)
				{
					if (nType == 10) LcomD.nActive = 8;  //×¼ÓÀ¾Ã×éºÏ
					if (nType == 11) LcomD.nActive = 7;  //ÆµÓö×éºÏ
				}
				SetCvlJTJD60_Capacity_TPG(LcomD, arTLoad, arTpgLoad, arFrLoad, dFactor, dTPGFactor, FALSE, FALSE, nMsgType, D_SER, nServEnvType);
			}
			if (m_nFrCount > 0) // Friction Ä¦²ÁÁ¦
			{
				LcomD = aLcomD.GetAt(lc); LcomD.nActive = 1; LoadCaseNo = GetCountLcomCase(LcomD);
				if (m_nLcomType == D_LCOMTYPE_CONCRETE)
				{
					if (nType == 10) LcomD.nActive = 8;  //×¼ÓÀ¾Ã×éºÏ
					if (nType == 11) LcomD.nActive = 7;  //ÆµÓö×éºÏ
					if (nType == 12)
					{
						LcomD.bElasticStage = TRUE;
						LcomD.nActive = 2;//±ê×¼×éºÏ
					}
				}
				else if (m_nLcomType == D_LCOMTYPE_STLCOMP || m_nLcomType == D_LCOMTYPE_SRC)
				{
					if (nType == 10) LcomD.nActive = 8;  //×¼ÓÀ¾Ã×éºÏ
					if (nType == 11) LcomD.nActive = 7;  //ÆµÓö×éºÏ
				}
				int nFR = 0;
				for (nc = 0; nc < m_nFrCount; nc++) nFR += Set_STLCIncludeCS(arFrLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD, 0);
				if (nFR > 0)
				{
					strDesc.Format(_T("+%sFR"), GetFactorString(dFactor)); LcomD.Description += strDesc;
					Set_LoadComb4Civil(LcomD, D_SER, nServEnvType, nMsgType);
				}
			}
		} // next m
	} // next lc

	// ¿µ±¸ÇÏÁß + Â÷·®ÇÏÁß + 1°³ÀÇ ±âÅ¸ °¡º¯ÇÏÁßÀÎ °æ¿ì(8°³)
	SetCvlESCGB19_Var1_S(arData, arMLoad, dIL, arLLoad, arIlLoad, arCrlLoad, arMCrlLoad, arCfLoad, arLsLoad, arWLoad, arWlLoad, arSfLoad, arIpLoad,
		arTLoad, arTpgLoad, arFrLoad, arBrkLoad, nType, nMsgType);
	// ¿µ±¸ÇÏÁß + Â÷·®ÇÏÁß + 2°³ÀÇ ±âÅ¸ °¡º¯ÇÏÁßÀÎ °æ¿ì(24°³)
	SetCvlESCGB19_Var2_S(arData, arMLoad, dIL, arLLoad, arIlLoad, arCrlLoad, arMCrlLoad, arCfLoad, arLsLoad, arWLoad, arWlLoad, arSfLoad, arIpLoad,
		arTLoad, arTpgLoad, arFrLoad, arBrkLoad, nType, nMsgType);
	// ¿µ±¸ÇÏÁß + Â÷·®ÇÏÁß + 3°³ÀÇ ±âÅ¸ °¡º¯ÇÏÁßÀÎ °æ¿ì(36°³)
	SetCvlESCGB19_Var3_S(arData, arMLoad, dIL, arLLoad, arIlLoad, arCrlLoad, arMCrlLoad, arCfLoad, arLsLoad, arWLoad, arWlLoad, arSfLoad, arIpLoad,
		arTLoad, arTpgLoad, arFrLoad, arBrkLoad, nType, nMsgType);
	// ¿µ±¸ÇÏÁß + Â÷·®ÇÏÁß + 4°³ÀÇ ±âÅ¸ °¡º¯ÇÏÁßÀÎ °æ¿ì(29°³)
	SetCvlESCGB19_Var4_S(arData, arMLoad, dIL, arLLoad, arIlLoad, arCrlLoad, arMCrlLoad, arCfLoad, arLsLoad, arWLoad, arWlLoad, arSfLoad, arIpLoad,
		arTLoad, arTpgLoad, arFrLoad, arBrkLoad, nType, nMsgType);
	// ¿µ±¸ÇÏÁß + Â÷·®ÇÏÁß + 5°³ ¹× 6°³ÀÇ ±âÅ¸ °¡º¯ÇÏÁßÀÎ °æ¿ì(12°³ ¹× 2°³)
	SetCvlESCGB19_Var56_S(arData, arMLoad, dIL, arLLoad, arIlLoad, arCrlLoad, arMCrlLoad, arCfLoad, arLsLoad, arWLoad, arWlLoad, arSfLoad, arIpLoad,
		arTLoad, arTpgLoad, arFrLoad, arBrkLoad, nType, nMsgType);

	return TRUE;
}

BOOL CLoadCombCtrl::SetCvlESCGB19_Accident_Ct(ArINT& arCtLoad)//³µÁ¾×²»÷Á¦
{
	// vessel collision load(Â÷·®Ãæµ¹ÇÏÁß)
	int LoadCaseNo = 0;
	CString strDesc = _T("");

	int nLcomSize = m_aAcdnLcomD.GetSize();
	for (int lc = 0; lc < nLcomSize; lc++)
	{
		for (int c = 0; c < m_nCtCount; c++)
		{
			T_LCOM_D LcomD = m_aAcdnLcomD.GetAt(lc); LoadCaseNo = GetCountLcomCase(LcomD);
			int nCT = Set_STLCIncludeCS(arCtLoad.GetAt(c), 1.0, LoadCaseNo, LcomD, 0);
			if (nCT > 0)
			{
				strDesc.Format(_T("+%sCT"), GetFactorString(1.0)); LcomD.Description += strDesc;
				if (m_nCtCount > 1) { strDesc.Format(_T("[%d]"), c + 1); LcomD.Description += strDesc; }
				Set_LoadComb4Civil(LcomD, D_STR, 0, 104); // ¿ì¿¬Á¶ÇÕ
			}
		}
	}
	return TRUE;
}


BOOL CLoadCombCtrl::SetCvlESCGB19_Accident_Cv(ArINT& arCvLoad)//´¬Ö»»òÆ¯Á÷Îï×²»÷Á¦
{
	int LoadCaseNo = 0;
	CString strDesc = _T("");

	int nLcomSize = m_aAcdnLcomD.GetSize();
	for (int lc = 0; lc < nLcomSize; lc++)
	{
		for (int c = 0; c < m_nCvCount; c++)
		{
			T_LCOM_D LcomD = m_aAcdnLcomD.GetAt(lc); LoadCaseNo = GetCountLcomCase(LcomD);
			int nCV = Set_STLCIncludeCS(arCvLoad.GetAt(c), 1.0, LoadCaseNo, LcomD, 0);
			if (nCV > 0)
			{
				strDesc.Format(_T("+%sCV"), GetFactorString(1.0));  LcomD.Description += strDesc;
				if (m_nCvCount > 1) { strDesc.Format(_T("[%d]"), c + 1);  LcomD.Description += strDesc; }
				Set_LoadComb4Civil(LcomD, D_STR, 0, 104); // ¿ì¿¬Á¶ÇÕ
			}
		}
	}

	return TRUE;
}

BOOL CLoadCombCtrl::SetCvlESCGB19_Var1_M(const CArray<T_LCOM_D, T_LCOM_D&>& arData, ArUNT& arMLoad,
	ArINT& arLLoad, ArINT& arIlLoad, ArINT& arCrlLoad, ArINT& arMCrlLoad,
	ArINT& arCfLoad, ArINT& arLsLoad, ArINT& arWLoad, ArINT& arWlLoad, ArINT& arSfLoad, ArINT& arIpLoad,
	ArINT& arTLoad, ArINT& arTpgLoad, ArINT& arFrLoad, ArINT& arBrkLoad, int nType, int nMsgType)
{
	// nType == 0,4: °è¼öÁ¶ÇÕ, 1: 1.0(settlement Á¦¿Ü), 2: 1.0(¸ðµÎ), Live Load Á¦¿Ü
	CArray<T_LCOM_D, T_LCOM_D&> aLcomD;
	aLcomD.Copy(arData);
	int  nLcomSize = aLcomD.GetSize();
	int  nnn = 0;
	//BOOL bAddCsLoad = (m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER1_CVL,nnn)) || (m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER2_CVL,nnn)) || (m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER3_CVL,nnn)); // CS or ST+CS
	BOOL bAddCsLoad = m_nConstLoad != 0 && CheckCS_SgldK(D_SGLD_ER1_CVL, nnn); // °üº¬Ê©¹¤ºÉÔØÇÒ´æÔÚÊ©¹¤ºÉÔØ

	double dFactor = (nType == 0 || nType == 4) ? 1.4 * 0.75 : 1.0;// ¸½¼ÓµØÃæ»îºÉÔØ ÈËÈººÉÔØ Á÷Ë®Ñ¹Á¦ ±ùºÉÔØ ÎÂ¶ÈºÉÔØ Ä¦²ÁÁ¦
	double dWFactor = (nType == 0 || nType == 4) ? 1.4 * 1.0 : 1.0;// ·çºÉÔØ
	double dCJJMLFactor = (nType == 0 || nType == 4) ? 1.1 : 1.0;
	double dLFactor = (nType == 0 || nType == 4) ? 1.4 : 1.0;   // Ê©¹¤ºÉÔØ »îÔØ³å»÷
	double dIL = (nType == 12) ? 0. : CalcMovingILFactor();//³å»÷ÏµÊý

	double dWlFactor = (nType == 0 || nType == 4) ? 1.1 * 1.0 : 1.0;// ·çºÉÔØ
	BOOL bExsitMV = m_nMCount ? TRUE : FALSE;

	if (nMsgType == 114 && m_bJTG15_Factor)
	{
		dFactor *= m_dJTG15_Factor;
		dWFactor *= m_dJTG15_Factor;
		dWlFactor *= m_dJTG15_Factor;
		dCJJMLFactor *= m_dJTG15_Factor;
		dLFactor *= m_dJTG15_Factor;
	}

	// ÓÀ¾Ã×÷ÓÃ + ÒÆ¶¯ºÉÔØ + 1¿É±ä×÷ÓÃ µÄÇé¿ö
	CString strDesc = _T("");
	int LoadCaseNo = 0, nc = 0;
	for (int lc = 0; lc < nLcomSize; lc++)
	{
		int nMCount = max(m_nMCount, 1);
		for (int m = 0; m < nMCount; m++)
		{
			T_LCOM_D LcomD = aLcomD.GetAt(lc); LcomD.nActive = 1; // Active or Strength/Stress
			T_LCOM_D LcomD_cf = LcomD; // MNET:1614, KJH20050809, cf°¡ Á¦¿ÜµÈ load combination
			LoadCaseNo = GetCountLcomCase(LcomD);

			double dMlFactor = 1.4;               // Æû³µºÉÔØ×éºÏÏµÊý
			CString strJudgeMILFactorByLoadCase = GetJTGD602015MILoadName(arMLoad, m);
			if (nType == 0 || nType == 1)         // ³ÐÔØÄÜÁ¦×´Ì¬  »ù±¾×éºÏ
			{
				if (strJudgeMILFactorByLoadCase == _T("CH-CL") || strJudgeMILFactorByLoadCase == _T("ÓÃ»§¶¨Òå"))
				{
					dMlFactor = 1.8;                  // ³µÁ¾ºÉÔØ  ÓÃ»§¶¨Òå   È¡1.8
				}
			}
			if (nType == 17)  dMlFactor = 1.0;
			if (nMsgType == 114 && m_bJTG15_Factor)
			{
				dMlFactor *= m_dJTG15_Factor;
			}

			BOOL bLive = FALSE, bCF = FALSE;
			if (nType != 2)
			{
				BOOL bAddLcomD = FALSE;
				// moving load ÒÆ¶¯ºÉÔØ
				if (m_nMCount > 0)
				{
					Set_MovingCase(arMLoad.GetAt(m), dMlFactor, LoadCaseNo, LcomD, TRUE);
					strDesc.Format(_T("+%sM"), GetFactorString(dMlFactor)); LcomD.Description += strDesc;
					if (m_nMCount > 1) { strDesc.Format(_T("[%d]"), m + 1); LcomD.Description += strDesc; }
					bAddLcomD = TRUE;
				}

				if (m_nLCount > 0 || (bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(L)))) // ST or ST+CS
				{
					int nLL = 0, nIL = 0;
					for (nc = 0; nc < m_nLCount; nc++)
						nLL += Set_STLCIncludeCS(arLLoad.GetAt(nc), dLFactor, LoadCaseNo, LcomD, 0);
					if (nLL > 0) { strDesc.Format(_T("+%sL"), GetFactorString(dLFactor)); LcomD.Description += strDesc; }
					if (bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(L)))
					{
						Set_CStageCase(m_aCSLoad.GetAt(nnn), dLFactor, LoadCaseNo, LcomD, TRUE);
						strDesc.Format(_T("+%s(cEL)"), GetFactorString(dLFactor)); LcomD.Description += strDesc;
						bAddLcomD = TRUE;
					}
					for (nc = 0; nc < m_nIlCount; nc++)//³å»÷
						nIL += Set_STLCIncludeCS(arIlLoad.GetAt(nc), dLFactor, LoadCaseNo, LcomD, 0);
					if (nIL > 0) { strDesc.Format(_T("+%sIL"), GetFactorString(dLFactor)); LcomD.Description += strDesc; }
					if (nLL + nIL > 0) bAddLcomD = TRUE;
				}
				bLive = m_nMCount > 0 || (m_nLCount > 0 || (bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(L))));
				if (bLive)//ÀëÐÄÁ¦
				{
					LcomD_cf = LcomD;
					int nCF = 0;
					for (nc = 0; nc < m_nCfCount; nc++)
						nCF += Set_STLCIncludeCS(arCfLoad.GetAt(nc), dLFactor, LoadCaseNo, LcomD, 0);
					if (nCF > 0) { strDesc.Format(_T("+%sCF"), GetFactorString(dLFactor)); LcomD.Description += strDesc; bCF = TRUE; }
					bAddLcomD = TRUE;
				}
				if (bAddLcomD)
				{
					if (nType == 4) m_aAcdnLcomD.Add(LcomD);
					else         Set_LoadComb4Civil(LcomD, D_STR, 0, nMsgType);
				}
			}

			if (m_nLsCount > 0) // Live Load Surcharge  ¸½¼ÓµØÃæ»îºÉÔØ
			{
				T_LCOM_D LcomD_l = LcomD; LoadCaseNo = GetCountLcomCase(LcomD_l);
				int nLS = 0;
				for (nc = 0; nc < m_nLsCount; nc++) nLS += Set_STLCIncludeCS(arLsLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD_l, 0);
				if (nLS > 0)
				{
					strDesc.Format(_T("+%sLS"), GetFactorString(dFactor)); LcomD_l.Description += strDesc;
					if (nType == 2 || nType == 4) m_aAcdnLcomD.Add(LcomD_l);
					else                     Set_LoadComb4Civil(LcomD_l, D_STR, 0, nMsgType);
				}
			}

			if (nType != 2 && (m_nCrlCount > 0 || m_nMCrlCount > 0)) // Crowd Load ÈËÈººÉÔØ
			{
				T_LCOM_D LcomD_c = LcomD;
				LoadCaseNo = GetCountLcomCase(LcomD_c);
				int nCRL = 0;
				for (nc = 0; nc < m_nCrlCount; nc++) nCRL += Set_STLCIncludeCS(arCrlLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD_c, 0);
				if (nCRL > 0) { strDesc.Format(_T("+%sCRL"), GetFactorString(dFactor)); LcomD_c.Description += strDesc; }

				int nMCrlCount = (m_nMCrlCount > 0) ? m_nMCrlCount : ((nCRL > 0) ? 1 : 0);
				for (int crl = 0; crl < nMCrlCount; ++crl)
				{
					T_LCOM_D LcomD_crl = LcomD_c; LoadCaseNo = GetCountLcomCase(LcomD_crl);
					if (m_nMCrlCount > 0)
					{
						Set_MovingCase(arMCrlLoad.GetAt(crl), dFactor, LoadCaseNo, LcomD_crl, TRUE);
						strDesc.Format(_T("+%sMCRL"), GetFactorString(dFactor)); LcomD_crl.Description += strDesc;
						if (m_nMCrlCount > 1) { strDesc.Format(_T("[%d]"), crl + 1); LcomD_crl.Description += strDesc; }
					}
					if (nCRL > 0 || m_nMCrlCount > 0)
					{
						if (nType == 2 || nType == 4) m_aAcdnLcomD.Add(LcomD_crl);
						else                     Set_LoadComb4Civil(LcomD_crl, D_STR, 0, nMsgType);
					}
				}
			}

			// ÖÆ¶¯ºÉÔØ
			if (m_nBrkCount > 0 && bLive) // Brake Load 
			{
				T_LCOM_D LcomD_l = LcomD; LoadCaseNo = GetCountLcomCase(LcomD_l);
				double dBrkF = (nType == 0 || nType == 4) ? (bCF ? dFactor * 0.7 : dFactor) : 1.0;
				int nBRK = 0;
				for (nc = 0; nc < m_nBrkCount; nc++)
					nBRK += Set_STLCIncludeCS(arBrkLoad.GetAt(nc), dBrkF, LoadCaseNo, LcomD_l, 0);
				if (nBRK > 0)
				{
					strDesc.Format(_T("+%sBRK"), GetFactorString(dBrkF)); LcomD_l.Description += strDesc;
					if (nType == 2 || nType == 4) m_aAcdnLcomD.Add(LcomD_l);
					else                     Set_LoadComb4Civil(LcomD_l, D_STR, 0, nMsgType);
				}
				if (bCF)//´æÔÚÀëÐÄÁ¦
				{
					LcomD_l = LcomD_cf; LoadCaseNo = GetCountLcomCase(LcomD_l);
					dBrkF = (nType == 0 || nType == 4) ? dFactor : 1.0;
					for (nc = 0; nc < m_nBrkCount; nc++)
						nBRK += Set_STLCIncludeCS(arBrkLoad.GetAt(nc), dBrkF, LoadCaseNo, LcomD_l, 0);
					if (nBRK > 0)
					{
						strDesc.Format(_T("+%sBRK"), GetFactorString(dBrkF));
						LcomD_l.Description += strDesc;
						if (nType == 2 || nType == 4) m_aAcdnLcomD.Add(LcomD_l);
						else                     Set_LoadComb4Civil(LcomD_l, D_STR, 0, nMsgType);
					}
				}
			}

			{
				int nWlCount = m_nWlCount * 2; // ·çºÉÔØ
				if (m_nWlCount == 0 && bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(WL))) nWlCount = 1;
				for (nc = 0; nc < nWlCount; nc++)
				{
					T_LCOM_D LcomD_l = LcomD; LoadCaseNo = GetCountLcomCase(LcomD_l);
					if (m_nWlCount > 0)
					{
						double dWF = (nc % 2 == 0) ? dWlFactor : -1.0*dWlFactor;
						if (Set_STLCIncludeCS(arWlLoad.GetAt(nc / 2), dWF, LoadCaseNo, LcomD_l, 0) > 0)
						{
							CString strW = (dWF > 0.0) ? _T("+%sWL") : _T("%sWL");
							strDesc.Format(strW, GetFactorString(dWF)); LcomD_l.Description += strDesc;
							if (m_nWlCount > 1) { strDesc.Format(_T("[%d]"), nc / 2 + 1); LcomD_l.Description += strDesc; }
						}
					}
					if (bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(WL)))
					{
						Set_CStageCase(m_aCSLoad.GetAt(nnn), dWlFactor, LoadCaseNo, LcomD_l, TRUE);
						strDesc.Format(_T("+%s(cEL)"), GetFactorString(dWlFactor)); LcomD_l.Description += strDesc;
					}
					if (nType == 2 || nType == 4) m_aAcdnLcomD.Add(LcomD_l);
					else                     Set_LoadComb4Civil(LcomD_l, D_STR, 0, nMsgType);
				}
			}

			if (!bExsitMV)
			{
				int nWCount = m_nWCount * 2; // ·çºÉÔØ
				if (m_nWCount == 0 && bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(W))) nWCount = 1;
				for (nc = 0; nc < nWCount; nc++)
				{
					T_LCOM_D LcomD_l = LcomD; LoadCaseNo = GetCountLcomCase(LcomD_l);
					if (m_nWCount > 0)
					{
						double dWF = (nc % 2 == 0) ? dWFactor : -1.*dWFactor;
						if (Set_STLCIncludeCS(arWLoad.GetAt(nc / 2), dWF, LoadCaseNo, LcomD_l, 0) > 0)
						{
							CString strW = (dWF > 0.0) ? _T("+%sW") : _T("%sW");
							strDesc.Format(strW, GetFactorString(dWF)); LcomD_l.Description += strDesc;
							if (m_nWCount > 1) { strDesc.Format(_T("[%d]"), nc / 2 + 1); LcomD_l.Description += strDesc; }
						}
					}
					if (bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(W)))
					{
						Set_CStageCase(m_aCSLoad.GetAt(nnn), dWFactor, LoadCaseNo, LcomD_l, TRUE);
						strDesc.Format(_T("+%s(cEL)"), GetFactorString(dWFactor)); LcomD_l.Description += strDesc;
					}
					if (nType == 2 || nType == 4) m_aAcdnLcomD.Add(LcomD_l);
					else                     Set_LoadComb4Civil(LcomD_l, D_STR, 0, nMsgType);
				}
			}


			// Á÷Ë®Ñ¹Á¦
			if (m_nSfCount > 0) // Stream Flow Pressure
			{
				T_LCOM_D LcomD_l = LcomD; LoadCaseNo = GetCountLcomCase(LcomD_l);
				int nSF = 0;
				for (nc = 0; nc < m_nSfCount; nc++)
					nSF += Set_STLCIncludeCS(arSfLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD_l, 0);
				if (nSF > 0)
				{
					strDesc.Format(_T("+%sSF"), GetFactorString(dFactor)); LcomD_l.Description = LcomD_l.Description + strDesc;
					if (nType == 2 || nType == 4) m_aAcdnLcomD.Add(LcomD_l);
					else                     Set_LoadComb4Civil(LcomD_l, D_STR, 0, nMsgType);
				}
			}

			// ±ùºÉÔØ
			if (m_nIpCount > 0) // Ice Pressure
			{
				T_LCOM_D LcomD_l = LcomD; LoadCaseNo = GetCountLcomCase(LcomD_l);
				int nIP = 0;
				for (nc = 0; nc < m_nIpCount; nc++)
					nIP += Set_STLCIncludeCS(arIpLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD_l, 0);
				if (nIP > 0)
				{
					strDesc.Format(_T("+%sIP"), GetFactorString(dFactor)); LcomD_l.Description = LcomD_l.Description + strDesc;
					if (nType == 2 || nType == 4) m_aAcdnLcomD.Add(LcomD_l);
					else                     Set_LoadComb4Civil(LcomD_l, D_STR, 0, nMsgType);
				}
			}

			// Temeperature ÎÂ¶ÈºÉÔØ
			if ((m_nTCount > 0 || m_nTpgCount > 0) || (bAddCsLoad && (CheckCSEL(D_SGLD_ER1_CVL, _LSX(T)) || CheckCSEL(D_SGLD_ER1_CVL, _LSX(TPG)))))
			{
				T_LCOM_D LcomD_l = LcomD;
				SetCvlJTJD60_Capacity_TPG(LcomD_l, arTLoad, arTpgLoad, arFrLoad, dFactor, dFactor, FALSE, nType == 2 || nType == 4, nMsgType);
			}

			if (m_nFrCount > 0) // Friction Ä¦²ÁÁ¦
			{
				T_LCOM_D LcomD_l = LcomD; LoadCaseNo = GetCountLcomCase(LcomD_l);
				int nFR = 0;
				for (nc = 0; nc < m_nFrCount; nc++) nFR += Set_STLCIncludeCS(arFrLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD_l, 0);
				if (nFR > 0)
				{
					strDesc.Format(_T("+%sFR"), GetFactorString(dFactor)); LcomD_l.Description += strDesc;
					if (nType == 2 || nType == 4) m_aAcdnLcomD.Add(LcomD_l);
					else                     Set_LoadComb4Civil(LcomD_l, D_STR, 0, nMsgType);
				}
			}
		} // next m
	} // next lc

	return TRUE;
}

BOOL CLoadCombCtrl::SetCvlESCGB19_Var2_M(const CArray<T_LCOM_D, T_LCOM_D&>& arData, ArUNT& arMLoad,
	ArINT& arLLoad, ArINT& arIlLoad, ArINT& arCrlLoad, ArINT& arMCrlLoad,
	ArINT& arCfLoad, ArINT& arLsLoad, ArINT& arWLoad, ArINT& arWlLoad, ArINT& arSfLoad, ArINT& arIpLoad,
	ArINT& arTLoad, ArINT& arTpgLoad, ArINT& arFrLoad, ArINT& arBrkLoad, int nType, int nMsgType)
{
	// nType == 0,4: °è¼öÁ¶ÇÕ, 1: 1.0(settlement Á¦¿Ü), 2: 1.0(¸ðµÎ), Live Load Á¦¿Ü
	CArray<T_LCOM_D, T_LCOM_D&> aLcomD;
	aLcomD.Copy(arData);
	int  nLcomSize = aLcomD.GetSize();
	int  nnn = 0;
	//BOOL bAddCsLoad = (m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER1_CVL,nnn)) || (m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER2_CVL,nnn)) || (m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER3_CVL,nnn)); // CS or ST+CS
	BOOL bAddCsLoad = m_nConstLoad != 0 && CheckCS_SgldK(D_SGLD_ER1_CVL, nnn); // °üº¬Ê©¹¤ºÉÔØÇÒ´æÔÚÊ©¹¤ºÉÔØ

	double dFactor = (nType == 0 || nType == 4) ? 1.4*0.75 : 1.0;
	double dWFactor = (nType == 0 || nType == 4) ? 1.4*1.0 : 1.0;
	double dLFactor = (nType == 0 || nType == 4) ? 1.4 : 1.0;
	double dF = (nType == 0 || nType == 4) ? 0.75 : 1.0;
	double dCJJMLFactor = (nType == 0 || nType == 4) ? 1.1 : 1.0;
	double dIL = (nType == 12) ? 0. : CalcMovingILFactor();

	double dWlFactor = (nType == 0 || nType == 4) ? 1.1 * 1.0 : 1.0;// ·çºÉÔØ
	BOOL bExsitMV = m_nMCount ? TRUE : FALSE;

	if (nMsgType == 114 && m_bJTG15_Factor)
	{
		dFactor *= m_dJTG15_Factor;
		dWFactor *= m_dJTG15_Factor;
		dWlFactor *= m_dJTG15_Factor;
		dCJJMLFactor *= m_dJTG15_Factor;
		dLFactor *= m_dJTG15_Factor;
		dF *= m_dJTG15_Factor;
	}

	ArINT arTmp;

	// ÓÀ¾Ã×÷ÓÃ + Æû³µºÉÔØ + 2¿É±ä×÷ÓÃ
	CString strDesc = _T("");
	int LoadCaseNo = 0, nc = 0;
	for (int lc = 0; lc < nLcomSize; lc++)
	{
		int nMCount = max(m_nMCount, 1);
		for (int m = 0; m < nMCount; m++)
		{
			T_LCOM_D LcomD = aLcomD.GetAt(lc); LcomD.nActive = 1; // Active or Strength/Stress
			T_LCOM_D LcomD_cf = LcomD; // MNET:1614, KJH20050809, CF°¡ Á¦¿ÜµÈ load combination
			LoadCaseNo = GetCountLcomCase(LcomD);

			double dMlFactor = 1.4;               // Æû³µºÉÔØ×éºÏÏµÊý
			CString strJudgeMILFactorByLoadCase = GetJTGD602015MILoadName(arMLoad, m);
			if (nType == 0 || nType == 1)         // ³ÐÔØÄÜÁ¦×´Ì¬  »ù±¾×éºÏ
			{
				if (strJudgeMILFactorByLoadCase == _T("CH-CL") || strJudgeMILFactorByLoadCase == _T("ÓÃ»§¶¨Òå"))
				{
					dMlFactor = 1.8;                  // ³µÁ¾ºÉÔØ  ÓÃ»§¶¨Òå   È¡1.8
				}
			}
			if (nType == 17)  dMlFactor = 1.0;
			if (nMsgType == 114 && m_bJTG15_Factor)
			{
				dMlFactor *= m_dJTG15_Factor;
			}

			BOOL bLive = FALSE, bCF = FALSE;
			//BOOL bFindCJJLeadingMLoad = FALSE;
			if (nType != 2)
			{
				// moving load ÒÆ¶¯ºÉÔØ
				if (m_nMCount > 0)
				{
					Set_MovingCase(arMLoad.GetAt(m), dMlFactor, LoadCaseNo, LcomD, TRUE);
					strDesc.Format(_T("+%sM"), GetFactorString(dMlFactor)); LcomD.Description += strDesc;
					if (m_nMCount > 1) { strDesc.Format(_T("[%d]"), m + 1); LcomD.Description += strDesc; }
				}

				int nLL = 0, nIL = 0, nCF = 0;
				if (m_nLCount > 0 || (bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(L)))) // ST or ST+CS
				{
					for (nc = 0; nc < m_nLCount; nc++) nLL += Set_STLCIncludeCS(arLLoad.GetAt(nc), dLFactor, LoadCaseNo, LcomD, 0);
					if (nLL > 0) { strDesc.Format(_T("+%sL"), GetFactorString(dLFactor)); LcomD.Description += strDesc; }
					if (bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(L)))
					{
						Set_CStageCase(m_aCSLoad.GetAt(nnn), dLFactor, LoadCaseNo, LcomD, TRUE);
						strDesc.Format(_T("+%s(cEL)"), GetFactorString(dLFactor)); LcomD.Description += strDesc;
					}
					for (nc = 0; nc < m_nIlCount; nc++) nIL += Set_STLCIncludeCS(arIlLoad.GetAt(nc), dLFactor, LoadCaseNo, LcomD, 0);
					if (nIL > 0) { strDesc.Format(_T("+%sIL"), GetFactorString(dLFactor)); LcomD.Description += strDesc; }
				}
				bLive = m_nMCount > 0 || (m_nLCount > 0 || (bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(L))));
				if (bLive)
				{
					LcomD_cf = LcomD;
					for (nc = 0; nc < m_nCfCount; nc++) nCF += Set_STLCIncludeCS(arCfLoad.GetAt(nc), dLFactor, LoadCaseNo, LcomD, 0);
					if (nCF > 0) { strDesc.Format(_T("+%sCF"), GetFactorString(dLFactor)); LcomD.Description += strDesc; bCF = TRUE; }
				}
			}

			if (m_nLsCount > 0) // Live Load Surcharge
			{
				T_LCOM_D LcomD_l = LcomD; LoadCaseNo = GetCountLcomCase(LcomD_l);
				int nLS = 0;
				for (nc = 0; nc < m_nLsCount; nc++) nLS += Set_STLCIncludeCS(arLsLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD_l, 0);
				T_LCOM_D LcomD_tmp = LcomD_cf;
				if (bCF)
				{
					LoadCaseNo = GetCountLcomCase(LcomD_tmp);
					for (nc = 0; nc < m_nLsCount; nc++) Set_STLCIncludeCS(arLsLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD_tmp, 0);
				}
				if (nLS > 0)
				{
					strDesc.Format(_T("+%sLS"), GetFactorString(dFactor)); LcomD_l.Description += strDesc;
					if (bCF) LcomD_tmp.Description += strDesc;
					SetCvlESCGB19_ETC_M(LcomD_l, LcomD_tmp, bCF, arCrlLoad, arMCrlLoad, nType != 2, arBrkLoad, bLive, arWLoad, FALSE, arWlLoad, TRUE, arSfLoad, TRUE,
						arIpLoad, TRUE, arTLoad, arTpgLoad, TRUE, arFrLoad, TRUE, 0, dF, nType == 2 || nType == 4, nMsgType);
				}
			}
			if (nType != 2 && (m_nCrlCount > 0 || m_nMCrlCount > 0)) // Crowd Load
			{
				T_LCOM_D LcomD_l = LcomD; LoadCaseNo = GetCountLcomCase(LcomD_l);
				int nCRL = 0;
				for (nc = 0; nc < m_nCrlCount; nc++) nCRL += Set_STLCIncludeCS(arCrlLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD_l, 0);
				T_LCOM_D LcomD_tmp = LcomD_cf;
				if (bCF)
				{
					LoadCaseNo = GetCountLcomCase(LcomD_tmp);
					for (nc = 0; nc < m_nCrlCount; nc++) Set_STLCIncludeCS(arCrlLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD_tmp, 0);
				}
				// MNET:XXXX-BJLEE(BBONG)-20060502 
				if (nCRL > 0 || m_nMCrlCount > 0)
				{
					if (nCRL > 0)
					{
						strDesc.Format(_T("+%sCRL"), GetFactorString(dFactor)); LcomD_l.Description += strDesc;
						if (bCF) LcomD_tmp.Description += strDesc;
					}

					int nMCrlCount = (m_nMCrlCount > 0) ? m_nMCrlCount : ((nCRL > 0) ? 1 : 0);
					T_LCOM_D LcomD_tmp2;
					for (int crl = 0; crl < nMCrlCount; ++crl)
					{
						T_LCOM_D LcomD_crl = LcomD_l; LoadCaseNo = GetCountLcomCase(LcomD_crl);
						LcomD_tmp2.Initialize();
						if (m_nMCrlCount > 0)
						{
							Set_MovingCase(arMCrlLoad.GetAt(crl), dFactor, LoadCaseNo, LcomD_crl, TRUE);
							strDesc.Format(_T("+%sMCRL"), GetFactorString(dFactor)); LcomD_crl.Description += strDesc;
							if (m_nMCrlCount > 1) { strDesc.Format(_T("[%d]"), crl + 1); LcomD_crl.Description += strDesc; }

							if (bCF)
							{
								LcomD_tmp2 = LcomD_tmp; LoadCaseNo = GetCountLcomCase(LcomD_tmp2);
								Set_MovingCase(arMCrlLoad.GetAt(crl), dFactor, LoadCaseNo, LcomD_tmp2, TRUE);
								strDesc.Format(_T("+%sMCRL"), GetFactorString(dFactor)); LcomD_tmp2.Description += strDesc;
								if (m_nMCrlCount > 1) { strDesc.Format(_T("[%d]"), crl + 1); LcomD_tmp2.Description += strDesc; }
							}////////////////////////////////
						}
						//if((nCRL>0 || m_nMCrlCount>0) && bCF)
						//{
						SetCvlESCGB19_ETC_M(LcomD_crl, LcomD_tmp2, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, bLive, arWLoad, FALSE, arWlLoad, TRUE, arSfLoad, TRUE, arIpLoad, TRUE,
							arTLoad, arTpgLoad, TRUE, arFrLoad, TRUE, 0, dF, nType == 2 || nType == 4, nMsgType);
						//}
					}
				}
			}

			if (m_nBrkCount > 0 && bLive) // Brake Load ÖÆ¶¯Á¦
			{
				T_LCOM_D LcomD_l = LcomD; LoadCaseNo = GetCountLcomCase(LcomD_l);
				double dBrkF = (nType == 0 || nType == 4) ? (bCF ? dFactor * 0.7 : dFactor) : 1.0;
				int nBRK = 0;
				for (nc = 0; nc < m_nBrkCount; nc++) nBRK += Set_STLCIncludeCS(arBrkLoad.GetAt(nc), dBrkF, LoadCaseNo, LcomD_l, 0);
				if (nBRK > 0)
				{
					strDesc.Format(_T("+%sBRK"), GetFactorString(dBrkF)); LcomD_l.Description += strDesc;
					SetCvlESCGB19_ETC_M(LcomD_l, LcomD_cf, FALSE, arTmp, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWLoad, FALSE, arWlLoad, TRUE, arSfLoad, FALSE, arIpLoad, FALSE,
						arTLoad, arTpgLoad, TRUE, arFrLoad, FALSE, 0, dF, nType == 2 || nType == 4, nMsgType);
				}
				if (bCF) // MNET:1614, KJH20060809, ¾Õ¿¡¼­ CF°¡ Á¸ÀçÇßÀ¸¸é, CF°¡ Á¦¿ÜµÈ ÇÏÁßÁ¶ÇÕÀ¸·Î ÇÑ¹ø´õ
				{
					LcomD_l = LcomD_cf; LoadCaseNo = GetCountLcomCase(LcomD_l);
					dBrkF = (nType == 0 || nType == 4) ? dFactor : 1.0;
					nBRK = 0;
					for (nc = 0; nc < m_nBrkCount; nc++) nBRK += Set_STLCIncludeCS(arBrkLoad.GetAt(nc), dBrkF, LoadCaseNo, LcomD_l, 0);
					if (nBRK > 0)
					{
						strDesc.Format(_T("+%sBRK"), GetFactorString(dBrkF)); LcomD_l.Description += strDesc;
						SetCvlESCGB19_ETC_M(LcomD_l, LcomD_cf, FALSE, arTmp, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWLoad, FALSE, arWlLoad, TRUE, arSfLoad, FALSE, arIpLoad, FALSE,
							arTLoad, arTpgLoad, TRUE, arFrLoad, FALSE, 0, dF, nType == 2 || nType == 4, nMsgType);
					}
				}
			}

			{
				int nWlCount = m_nWlCount * 2; // MNET:1614, KJH20050808
				if (m_nWlCount == 0 && bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(WL))) nWlCount = 1;
				for (nc = 0; nc < nWlCount; nc++)
				{
					T_LCOM_D LcomD_l = LcomD; LoadCaseNo = GetCountLcomCase(LcomD_l);
					if (m_nWlCount > 0)
					{
						double dWF = (nc % 2 == 0) ? dWlFactor : -1.0*dWlFactor;
						if (Set_STLCIncludeCS(arWlLoad.GetAt(nc / 2), dWF, LoadCaseNo, LcomD_l, 0) > 0)
						{
							CString strW = (dWF > 0.0) ? _T("+%sWL") : _T("%sWL");
							strDesc.Format(strW, GetFactorString(dWF)); LcomD_l.Description += strDesc;
							if (m_nWlCount > 1) { strDesc.Format(_T("[%d]"), nc / 2 + 1); LcomD_l.Description += strDesc; }
						}
					}
					if (bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(WL)))
					{
						Set_CStageCase(m_aCSLoad.GetAt(nnn), dWlFactor, LoadCaseNo, LcomD_l, TRUE);
						strDesc.Format(_T("+%s(cEL)"), GetFactorString(dWlFactor)); LcomD_l.Description += strDesc;
					}
					SetCvlESCGB19_ETC_M(LcomD_l, LcomD_cf, bCF, arTmp, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWLoad, FALSE, arWlLoad, FALSE, arSfLoad, TRUE, arIpLoad, TRUE,
						arTLoad, arTpgLoad, TRUE, arFrLoad, TRUE, 0, dF, nType == 2 || nType == 4, nMsgType);
				}
			}
			if (!bExsitMV)
			{
				int nWCount = m_nWCount * 2; // MNET:1614, KJH20050808
				if (m_nWCount == 0 && bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(W))) nWCount = 1;
				for (nc = 0; nc < nWCount; nc++)
				{
					T_LCOM_D LcomD_l = LcomD; LoadCaseNo = GetCountLcomCase(LcomD_l);
					if (m_nWCount > 0)
					{
						double dWF = (nc % 2 == 0) ? dWFactor : -1.*dWFactor;
						if (Set_STLCIncludeCS(arWLoad.GetAt(nc / 2), dWF, LoadCaseNo, LcomD_l, 0) > 0)
						{
							CString strW = (dWF > 0.0) ? _T("+%sW") : _T("%sW");
							strDesc.Format(strW, GetFactorString(dWF)); LcomD_l.Description += strDesc;
							if (m_nWCount > 1) { strDesc.Format(_T("[%d]"), nc / 2 + 1); LcomD_l.Description += strDesc; }
						}
					}
					if (bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(W)))
					{
						Set_CStageCase(m_aCSLoad.GetAt(nnn), dWFactor, LoadCaseNo, LcomD_l, TRUE);
						strDesc.Format(_T("+%s(cEL)"), GetFactorString(dWFactor)); LcomD_l.Description += strDesc;
					}
					SetCvlESCGB19_ETC_M(LcomD_l, LcomD_cf, bCF, arTmp, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWLoad, FALSE, arWlLoad, FALSE, arSfLoad, TRUE, arIpLoad, TRUE,
						arTLoad, arTpgLoad, TRUE, arFrLoad, TRUE, 0, dF, nType == 2 || nType == 4, nMsgType);
				}
			}
			if (m_nSfCount > 0) // Stream Flow Pressure
			{
				T_LCOM_D LcomD_l = LcomD; LoadCaseNo = GetCountLcomCase(LcomD_l);
				int nSF = 0;
				for (nc = 0; nc < m_nSfCount; nc++) nSF += Set_STLCIncludeCS(arSfLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD_l, 0);
				if (nSF > 0)
				{
					strDesc.Format(_T("+%sSF"), GetFactorString(dFactor)); LcomD_l.Description = LcomD_l.Description + strDesc;
					SetCvlESCGB19_ETC_M(LcomD_l, LcomD_cf, bCF, arTmp, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWLoad, FALSE, arWlLoad, FALSE, arSfLoad, FALSE, arIpLoad, FALSE,
						arTLoad, arTpgLoad, TRUE, arFrLoad, TRUE, 0, dF, nType == 2 || nType == 4, nMsgType);
				}
			}

			if (m_nIpCount > 0) // Ice Pressure
			{
				T_LCOM_D LcomD_l = LcomD; LoadCaseNo = GetCountLcomCase(LcomD_l);
				int nIP = 0;
				for (nc = 0; nc < m_nIpCount; nc++) nIP += Set_STLCIncludeCS(arIpLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD_l, 0);
				if (nIP > 0)
				{
					strDesc.Format(_T("+%sIP"), GetFactorString(dFactor)); LcomD_l.Description = LcomD_l.Description + strDesc;
					SetCvlESCGB19_ETC_M(LcomD_l, LcomD_cf, bCF, arTmp, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWLoad, FALSE, arWlLoad, FALSE, arSfLoad, FALSE, arIpLoad, FALSE,
						arTLoad, arTpgLoad, TRUE, arFrLoad, TRUE, 0, dF, nType == 2 || nType == 4, nMsgType);
				}
			}

			if ((m_nTCount > 0 || m_nTpgCount > 0) || (bAddCsLoad && (CheckCSEL(D_SGLD_ER1_CVL, _LSX(T)) || CheckCSEL(D_SGLD_ER1_CVL, _LSX(TPG))))) // Temeperature
			{
				T_LCOM_D LcomD_l = LcomD;
				SetCvlJTJD60_Capacity_TPG(LcomD_l, arTLoad, arTpgLoad, arFrLoad, dFactor, dFactor, TRUE, nType == 2 || nType == 4, nMsgType);
			}
		} // next m
	} // next lc

	return TRUE;
}

BOOL CLoadCombCtrl::SetCvlESCGB19_Var3_M(const CArray<T_LCOM_D, T_LCOM_D&>& arData, ArUNT& arMLoad, ArINT& arLLoad,
	ArINT& arIlLoad, ArINT& arCrlLoad, ArINT& arMCrlLoad, ArINT& arCfLoad, ArINT& arLsLoad,
	ArINT& arWLoad, ArINT& arWlLoad, ArINT& arSfLoad, ArINT& arIpLoad, ArINT& arTLoad, ArINT& arTpgLoad,
	ArINT& arFrLoad, ArINT& arBrkLoad, int nType, int nMsgType)
{
	// nType == 0,4: °è¼öÁ¶ÇÕ, 1: 1.0(settlement Á¦¿Ü), 2: 1.0(¸ðµÎ), Live Load Á¦¿Ü
	CArray<T_LCOM_D, T_LCOM_D&> aLcomD; aLcomD.Copy(arData);
	int nLcomSize = aLcomD.GetSize();
	int  nnn = 0;
	//BOOL bAddCsLoad = (m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER1_CVL,nnn)) || (m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER2_CVL,nnn)) || (m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER3_CVL,nnn)); // CS or ST+CS
	BOOL bAddCsLoad = m_nConstLoad != 0 && CheckCS_SgldK(D_SGLD_ER1_CVL, nnn); // °üº¬Ê©¹¤ºÉÔØÇÒ´æÔÚÊ©¹¤ºÉÔØ

	double dFactor = (nType == 0 || nType == 4) ? 1.4*0.75 : 1.0;
	double dWFactor = (nType == 0 || nType == 4) ? 1.4*1.0 : 1.0;
	double dLFactor = (nType == 0 || nType == 4) ? 1.4 : 1.0;
	double dF = (nType == 0 || nType == 4) ? 0.75 : 1.0;
	double dMLFactor = (nType == 0 || nType == 4) ? 1.1 : 1.0;
	double dIL = (nType == 12) ? 0. : CalcMovingILFactor();

	double dWlFactor = (nType == 0 || nType == 4) ? 1.1 * 1.0 : 1.0;// ·çºÉÔØ
	BOOL bExsitMV = m_nMCount ? TRUE : FALSE;

	if (nMsgType == 114 && m_bJTG15_Factor)
	{
		dFactor *= m_dJTG15_Factor;
		dWFactor *= m_dJTG15_Factor;
		dWlFactor *= m_dJTG15_Factor;
		dLFactor *= m_dJTG15_Factor;
		dF *= m_dJTG15_Factor;
		dMLFactor *= m_dJTG15_Factor;
	}

	ArINT arTmp;

	// ÓÀ¾Ã×÷ÓÃ + Æû³µºÉÔØ + 3¿É±ä×÷ÓÃ
	CString strDesc = _T("");
	int LoadCaseNo = 0, nc = 0;
	for (int lc = 0; lc < nLcomSize; lc++)
	{
		int nMCount = max(m_nMCount, 1);
		for (int m = 0; m < nMCount; m++)
		{
			T_LCOM_D LcomD = aLcomD.GetAt(lc); LcomD.nActive = 1; // Active or Strength/Stress
			T_LCOM_D LcomD_cf = LcomD; // MNET:1614, KJH20050809, CF°¡ Á¦¿ÜµÈ load combination
			LoadCaseNo = GetCountLcomCase(LcomD);

			double dMlFactor = 1.4;               // Æû³µºÉÔØ×éºÏÏµÊý
			CString strJudgeMILFactorByLoadCase = GetJTGD602015MILoadName(arMLoad, m);
			if (nType == 0 || nType == 1)         // ³ÐÔØÄÜÁ¦×´Ì¬  »ù±¾×éºÏ
			{
				if (strJudgeMILFactorByLoadCase == _T("CH-CL") || strJudgeMILFactorByLoadCase == _T("ÓÃ»§¶¨Òå"))
				{
					dMlFactor = 1.8;                  // ³µÁ¾ºÉÔØ  ÓÃ»§¶¨Òå   È¡1.8
				}
			}
			if (nType == 17)  dMlFactor = 1.0;
			if (nMsgType == 114 && m_bJTG15_Factor)
			{
				dMlFactor *= m_dJTG15_Factor;
			}

			BOOL bLive = FALSE, bCF = FALSE;

			if (nType != 2)
			{
				// moving load
				if (m_nMCount > 0)
				{
					Set_MovingCase(arMLoad.GetAt(m), dMlFactor, LoadCaseNo, LcomD, TRUE);
					strDesc.Format(_T("+%sM"), GetFactorString(dMlFactor)); LcomD.Description += strDesc;
					if (m_nMCount > 1) { strDesc.Format(_T("[%d]"), m + 1); LcomD.Description += strDesc; }
				}

				int nLL = 0, nIL = 0, nCF = 0;
				if (m_nLCount > 0 || (bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(L)))) // ST or ST+CS
				{
					for (nc = 0; nc < m_nLCount;nc++) nLL += Set_STLCIncludeCS(arLLoad.GetAt(nc), dLFactor, LoadCaseNo, LcomD, 0);
					if (nLL > 0) { strDesc.Format(_T("+%sL"), GetFactorString(dLFactor)); LcomD.Description += strDesc; }
					if (bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(L)))
					{
						Set_CStageCase(m_aCSLoad.GetAt(nnn), dLFactor, LoadCaseNo, LcomD, TRUE);
						strDesc.Format(_T("+%s(cEL)"), GetFactorString(dLFactor)); LcomD.Description += strDesc;
					}
					for (nc = 0; nc < m_nIlCount; nc++) nIL += Set_STLCIncludeCS(arIlLoad.GetAt(nc), dLFactor, LoadCaseNo, LcomD, 0);
					if (nIL > 0) { strDesc.Format(_T("+%sIL"), GetFactorString(dLFactor));  LcomD.Description += strDesc; }
				}
				bLive = m_nMCount > 0 || (m_nLCount > 0 || (bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(L))));
				if (bLive)
				{
					LcomD_cf = LcomD;
					for (nc = 0; nc < m_nCfCount; nc++) nCF += Set_STLCIncludeCS(arCfLoad.GetAt(nc), dLFactor, LoadCaseNo, LcomD, 0);
					if (nCF > 0) { strDesc.Format(_T("+%sCF"), GetFactorString(dLFactor));  LcomD.Description += strDesc; bCF = TRUE; }
				}
			}

			if (m_nLsCount > 0) // Live Load Surcharge
			{
				T_LCOM_D LcomD_l = LcomD; LoadCaseNo = GetCountLcomCase(LcomD_l);
				int nLS = 0;
				for (nc = 0; nc < m_nLsCount; nc++) nLS += Set_STLCIncludeCS(arLsLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD_l, 0);
				T_LCOM_D LcomD_tmp = LcomD_cf;
				if (bCF)
				{
					LoadCaseNo = GetCountLcomCase(LcomD_tmp);
					for (nc = 0; nc < m_nLsCount; nc++) Set_STLCIncludeCS(arLsLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD_tmp, 0);
				}
				if (nLS > 0)
				{
					strDesc.Format(_T("+%sLS"), GetFactorString(dFactor)); LcomD_l.Description += strDesc;
					if (bCF) LcomD_tmp.Description += strDesc;
					SetCvlESCGB19_ETC_M(LcomD_l, LcomD_tmp, bCF, arCrlLoad, arMCrlLoad, nType != 2, arBrkLoad, bLive, arWLoad, FALSE, arWlLoad, TRUE, arSfLoad, TRUE, arIpLoad, TRUE,
						arTLoad, arTpgLoad, TRUE, arFrLoad, FALSE, 1, dF, nType == 2 || nType == 4, nMsgType);
				}
			}
			// MNET:XXXX-BJLEE(BBONG)-20060503
			if (nType != 2 && (m_nCrlCount > 0 || m_nMCrlCount > 0)) // Crowd Load
			{
				T_LCOM_D LcomD_l = LcomD; LoadCaseNo = GetCountLcomCase(LcomD_l);
				int nCRL = 0;
				for (nc = 0; nc < m_nCrlCount; nc++) nCRL += Set_STLCIncludeCS(arCrlLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD_l, 0);
				T_LCOM_D LcomD_tmp = LcomD_cf;
				if (bCF)
				{
					LoadCaseNo = GetCountLcomCase(LcomD_tmp);
					for (nc = 0; nc < m_nCrlCount; nc++) Set_STLCIncludeCS(arCrlLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD_tmp, 0);
				}
				// MNET:XXXX-BJLEE(BBONG)-20060502 
				if (nCRL > 0 || m_nMCrlCount > 0)
				{
					if (nCRL > 0)
					{
						strDesc.Format(_T("+%sCRL"), GetFactorString(dFactor)); LcomD_l.Description += strDesc;
						if (bCF) LcomD_tmp.Description += strDesc;
					}

					int nMCrlCount = (m_nMCrlCount > 0) ? m_nMCrlCount : ((nCRL > 0) ? 1 : 0);
					T_LCOM_D LcomD_tmp2;
					for (int crl = 0; crl < nMCrlCount; ++crl)
					{
						T_LCOM_D LcomD_crl = LcomD_l; LoadCaseNo = GetCountLcomCase(LcomD_crl);
						LcomD_tmp2.Initialize();
						if (m_nMCrlCount > 0)
						{
							Set_MovingCase(arMCrlLoad.GetAt(crl), dFactor, LoadCaseNo, LcomD_crl, TRUE);
							strDesc.Format(_T("+%sMCRL"), GetFactorString(dFactor)); LcomD_crl.Description += strDesc;
							if (m_nMCrlCount > 1) { strDesc.Format(_T("[%d]"), crl + 1); LcomD_crl.Description += strDesc; }

							if (bCF)
							{
								LcomD_tmp2 = LcomD_tmp; LoadCaseNo = GetCountLcomCase(LcomD_tmp2);
								Set_MovingCase(arMCrlLoad.GetAt(crl), dFactor, LoadCaseNo, LcomD_tmp2, TRUE);
								strDesc.Format(_T("+%sMCRL"), GetFactorString(dFactor)); LcomD_tmp2.Description += strDesc;
								if (m_nMCrlCount > 1) { strDesc.Format(_T("[%d]"), crl + 1); LcomD_tmp2.Description += strDesc; }
							}////////////////////////////////
						}

						//if((nCRL>0 || m_nMCrlCount>0) && bCF)
						//{
						SetCvlESCGB19_ETC_M(LcomD_crl, LcomD_tmp2, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, bLive, arWLoad, FALSE, arWlLoad, TRUE, arSfLoad, TRUE, arIpLoad, TRUE,
							arTLoad, arTpgLoad, TRUE, arFrLoad, FALSE, 2, dF, nType == 2 || nType == 4, nMsgType);
						//}
					}
				}
			}

			if (m_nBrkCount > 0 && bLive) // Brake Load
			{
				T_LCOM_D LcomD_l = LcomD; LoadCaseNo = GetCountLcomCase(LcomD_l);
				double dBrkF = (nType == 0 || nType == 4) ? (bCF ? dFactor * 0.7 : dFactor) : 1.0;
				int nBRK = 0;
				for (nc = 0; nc < m_nBrkCount; nc++) nBRK += Set_STLCIncludeCS(arBrkLoad.GetAt(nc), dBrkF, LoadCaseNo, LcomD_l, 0);
				if (nBRK > 0)
				{
					strDesc.Format(_T("+%sBRK"), GetFactorString(dBrkF)); LcomD_l.Description += strDesc;
					SetCvlESCGB19_ETC_M(LcomD_l, LcomD_cf, FALSE, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWLoad, FALSE, arWlLoad, TRUE, arSfLoad, FALSE, arIpLoad, FALSE,
						arTLoad, arTpgLoad, FALSE, arFrLoad, FALSE, 3, dF, nType == 2 || nType == 4, nMsgType);
				}
				if (bCF) // MNET:1614, KJH20050809, ¾Õ¿¡¼­ CF°¡ Á¸ÀçÇÏ¸é CF°¡ Á¦¿ÜµÈ ÇÏÁßÁ¶ÇÕ ÇÑ¹ø´õ...
				{
					LcomD_l = LcomD_cf; LoadCaseNo = GetCountLcomCase(LcomD_l);
					dBrkF = (nType == 0 || nType == 4) ? dFactor : 1.0;
					int nBRK = 0;
					for (nc = 0; nc < m_nBrkCount; nc++) nBRK += Set_STLCIncludeCS(arBrkLoad.GetAt(nc), dBrkF, LoadCaseNo, LcomD_l, 0);
					if (nBRK > 0)
					{
						strDesc.Format(_T("+%sBRK"), GetFactorString(dBrkF)); LcomD_l.Description += strDesc;
						SetCvlESCGB19_ETC_M(LcomD_l, LcomD_cf, FALSE, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWLoad, FALSE, arWlLoad, TRUE, arSfLoad, FALSE, arIpLoad, FALSE,
							arTLoad, arTpgLoad, FALSE, arFrLoad, FALSE, 3, dF, nType == 2 || nType == 4, nMsgType);
					}
				}
			}

			{
				int nWlCount = m_nWlCount * 2; // MNET:1614, KJH20050808
				if (m_nWlCount == 0 && bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(WL))) nWlCount = 1;
				for (nc = 0; nc < nWlCount; nc++)
				{
					T_LCOM_D LcomD_l = LcomD; LoadCaseNo = GetCountLcomCase(LcomD_l);
					if (m_nWlCount > 0)
					{
						double dWF = (nc % 2 == 0) ? dWlFactor : -1.0*dWlFactor;
						if (Set_STLCIncludeCS(arWlLoad.GetAt(nc / 2), dWF, LoadCaseNo, LcomD_l, 0) > 0)
						{
							CString strW = (dWF > 0.0) ? _T("+%sWL") : _T("%sWL");
							strDesc.Format(strW, GetFactorString(dWF)); LcomD_l.Description += strDesc;
							if (m_nWlCount > 1) { strDesc.Format(_T("[%d]"), nc / 2 + 1); LcomD_l.Description += strDesc; }
						}
					}
					if (bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(WL)))
					{
						Set_CStageCase(m_aCSLoad.GetAt(nnn), dWlFactor, LoadCaseNo, LcomD_l, TRUE);
						strDesc.Format(_T("+%s(cEL)"), GetFactorString(dWlFactor)); LcomD_l.Description += strDesc;
					}
					SetCvlESCGB19_ETC_M(LcomD_l, LcomD_cf, bCF, arTmp, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWLoad, FALSE, arWlLoad, FALSE, arSfLoad, TRUE, arIpLoad, TRUE,
						arTLoad, arTpgLoad, TRUE, arFrLoad, FALSE, 4, dF, nType == 2 || nType == 4, nMsgType);
				}
			}
			if (!bExsitMV)
			{
				int nWCount = m_nWCount * 2; // MNET:1614, KJH20050808
				if (m_nWCount == 0 && bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(W))) nWCount = 1;
				for (nc = 0; nc < nWCount; nc++)
				{
					T_LCOM_D LcomD_l = LcomD; LoadCaseNo = GetCountLcomCase(LcomD_l);
					if (m_nWCount > 0)
					{
						double dWF = (nc % 2 == 0) ? dWFactor : -1.*dWFactor;
						if (Set_STLCIncludeCS(arWLoad.GetAt(nc / 2), dWF, LoadCaseNo, LcomD_l, 0) > 0)
						{
							CString strW = (dWF > 0.0) ? _T("+%sW") : _T("%sW");
							strDesc.Format(strW, GetFactorString(dWF)); LcomD_l.Description += strDesc;
							if (m_nWCount > 1) { strDesc.Format(_T("[%d]"), nc / 2 + 1); LcomD_l.Description += strDesc; }
						}
					}
					if (bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(W)))
					{
						Set_CStageCase(m_aCSLoad.GetAt(nnn), dWFactor, LoadCaseNo, LcomD_l, TRUE);
						strDesc.Format(_T("+%s(cEL)"), GetFactorString(dWFactor)); LcomD_l.Description += strDesc;
					}
					SetCvlESCGB19_ETC_M(LcomD_l, LcomD_cf, bCF, arTmp, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWLoad, FALSE, arWlLoad, FALSE, arSfLoad, TRUE, arIpLoad, TRUE,
						arTLoad, arTpgLoad, TRUE, arFrLoad, FALSE, 4, dF, nType == 2 || nType == 4, nMsgType);
				}
			}
			if (m_nSfCount > 0) // Stream Flow Pressure
			{
				T_LCOM_D LcomD_l = LcomD; LoadCaseNo = GetCountLcomCase(LcomD_l);
				int nSF = 0;
				for (nc = 0; nc < m_nSfCount; nc++) nSF += Set_STLCIncludeCS(arSfLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD_l, 0);
				if (nSF > 0)
				{
					strDesc.Format(_T("+%sSF"), GetFactorString(dFactor)); LcomD_l.Description = LcomD_l.Description + strDesc;
					SetCvlESCGB19_ETC_M(LcomD_l, LcomD_cf, bCF, arTmp, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWLoad, FALSE, arWlLoad, FALSE, arSfLoad, FALSE, arIpLoad, FALSE,
						arTLoad, arTpgLoad, TRUE, arFrLoad, FALSE, 5, dF, nType == 2 || nType == 4, nMsgType);
				}
			}

			if (m_nIpCount > 0) // Ice Pressure
			{
				T_LCOM_D LcomD_l = LcomD; LoadCaseNo = GetCountLcomCase(LcomD_l);
				int nIP = 0;
				for (nc = 0; nc < m_nIpCount; nc++) nIP += Set_STLCIncludeCS(arIpLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD_l, 0);
				if (nIP > 0)
				{
					strDesc.Format(_T("+%sIP"), GetFactorString(dFactor)); LcomD_l.Description = LcomD_l.Description + strDesc;
					SetCvlESCGB19_ETC_M(LcomD_l, LcomD_cf, bCF, arTmp, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWLoad, FALSE, arWlLoad, FALSE, arSfLoad, FALSE, arIpLoad, FALSE,
						arTLoad, arTpgLoad, TRUE, arFrLoad, FALSE, 5, dF, nType == 2 || nType == 4, nMsgType);
				}
			}

		} // next m
	} // next lc

	return TRUE;
}

BOOL CLoadCombCtrl::SetCvlESCGB19_Var4_M(const CArray<T_LCOM_D, T_LCOM_D&>& arData, ArUNT& arMLoad,
	ArINT& arLLoad, ArINT& arIlLoad, ArINT& arCrlLoad, ArINT& arMCrlLoad,
	ArINT& arCfLoad, ArINT& arLsLoad, ArINT& arWLoad, ArINT& arWlLoad, ArINT& arSfLoad, ArINT& arIpLoad,
	ArINT& arTLoad, ArINT& arTpgLoad, ArINT& arFrLoad, ArINT& arBrkLoad, int nType, int nMsgType)
{
	// nType == 0,4: °è¼öÁ¶ÇÕ, 1: 1.0(settlement Á¦¿Ü), 2: 1.0(¸ðµÎ), Live Load Á¦¿Ü
	CArray<T_LCOM_D, T_LCOM_D&> aLcomD; aLcomD.Copy(arData);
	int  nLcomSize = aLcomD.GetSize();
	int  nnn = 0;
	//BOOL bAddCsLoad = (m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER1_CVL,nnn)) || (m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER2_CVL,nnn)) || (m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER3_CVL,nnn)); // CS or ST+CS
	BOOL bAddCsLoad = m_nConstLoad != 0 && CheckCS_SgldK(D_SGLD_ER1_CVL, nnn); // °üº¬Ê©¹¤ºÉÔØÇÒ´æÔÚÊ©¹¤ºÉÔØ

	double dFactor = (nType == 0 || nType == 4) ? 1.4*0.75 : 1.0;
	double dWFactor = (nType == 0 || nType == 4) ? 1.4*1.0 : 1.0;
	double dLFactor = (nType == 0 || nType == 4) ? 1.4 : 1.0;
	double dF = (nType == 0 || nType == 4) ? 0.75 : 1.0;
	double dMLFactor = (nType == 0 || nType == 4) ? 1.1 : 1.0;
	double dIL = (nType == 12) ? 0. : CalcMovingILFactor();

	double dWlFactor = (nType == 0 || nType == 4) ? 1.1 * 1.0 : 1.0;// ·çºÉÔØ
	BOOL bExsitMV = m_nMCount ? TRUE : FALSE;

	if (nMsgType == 114 && m_bJTG15_Factor)
	{
		dFactor *= m_dJTG15_Factor;
		dWFactor *= m_dJTG15_Factor;
		dWlFactor *= m_dJTG15_Factor;
		dLFactor *= m_dJTG15_Factor;
		dF *= m_dJTG15_Factor;
		dMLFactor *= m_dJTG15_Factor;
	}

	ArINT arTmp;

	// ÓÀ¾Ã×÷ÓÃ + Æû³µºÉÔØ + 4¿É±ä×÷ÓÃ
	CString strDesc = _T("");
	int LoadCaseNo = 0, nc = 0;
	for (int lc = 0; lc < nLcomSize; lc++)
	{
		int nMCount = max(m_nMCount, 1);
		for (int m = 0; m < nMCount; m++)
		{
			T_LCOM_D LcomD = aLcomD.GetAt(lc); LcomD.nActive = 1; // Active or Strength/Stress
			T_LCOM_D LcomD_cf = LcomD; // MNET:1614, KJH20050809, CF°¡ Á¦¿ÜµÈ ÇÏÁßÁ¶ÇÕ
			LoadCaseNo = GetCountLcomCase(LcomD);

			double dMlFactor = 1.4;       // Æû³µºÉÔØ×éºÏÏµÊý
			CString strJudgeMILFactorByLoadCase = GetJTGD602015MILoadName(arMLoad, m);
			if (nType == 0 || nType == 1)         // ³ÐÔØÄÜÁ¦×´Ì¬  »ù±¾×éºÏ
			{
				if (strJudgeMILFactorByLoadCase == _T("CH-CL") || strJudgeMILFactorByLoadCase == _T("ÓÃ»§¶¨Òå"))
				{
					dMlFactor = 1.8;                  // ³µÁ¾ºÉÔØ  ÓÃ»§¶¨Òå   È¡1.8
				}
			}
			if (nType == 17)  dMlFactor = 1.0;
			if (nMsgType == 114 && m_bJTG15_Factor)
			{
				dMlFactor *= m_dJTG15_Factor;
			}

			BOOL bLive = FALSE, bCF = FALSE;

			if (nType != 2)
			{
				// moving load
				if (m_nMCount > 0)
				{
					Set_MovingCase(arMLoad.GetAt(m), dMlFactor, LoadCaseNo, LcomD, TRUE);
					strDesc.Format(_T("+%sM"), GetFactorString(dMlFactor)); LcomD.Description += strDesc;
					if (m_nMCount > 1) { strDesc.Format(_T("[%d]"), m + 1); LcomD.Description += strDesc; }
				}

				int nLL = 0, nIL = 0, nCF = 0;
				if (m_nLCount > 0 || (bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(L)))) // ST or ST+CS
				{
					for (nc = 0; nc < m_nLCount; nc++) nLL += Set_STLCIncludeCS(arLLoad.GetAt(nc), dLFactor, LoadCaseNo, LcomD, 0);
					if (nLL > 0) { strDesc.Format(_T("+%sL"), GetFactorString(dLFactor)); LcomD.Description += strDesc; }
					if (bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(L)))
					{
						Set_CStageCase(m_aCSLoad.GetAt(nnn), dLFactor, LoadCaseNo, LcomD, TRUE);
						strDesc.Format(_T("+%s(cEL)"), GetFactorString(dLFactor));  LcomD.Description += strDesc;
					}
					for (nc = 0; nc < m_nIlCount; nc++) nIL += Set_STLCIncludeCS(arIlLoad.GetAt(nc), dLFactor, LoadCaseNo, LcomD, 0);
					if (nIL > 0) { strDesc.Format(_T("+%sIL"), GetFactorString(dLFactor));  LcomD.Description += strDesc; }
				}
				bLive = m_nMCount > 0 || (m_nLCount > 0 || (bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(L))));
				if (bLive)
				{
					LcomD_cf = LcomD;
					for (nc = 0; nc < m_nCfCount; nc++) nCF += Set_STLCIncludeCS(arCfLoad.GetAt(nc), dLFactor, LoadCaseNo, LcomD, 0);
					if (nCF > 0) { strDesc.Format(_T("+%sCF"), GetFactorString(dLFactor));  LcomD.Description += strDesc; bCF = TRUE; }
				}
			}

			if (m_nLsCount > 0) // Live Load Surcharge
			{
				T_LCOM_D LcomD_l = LcomD; LoadCaseNo = GetCountLcomCase(LcomD_l);
				int nLS = 0;
				for (nc = 0; nc < m_nLsCount; nc++) nLS += Set_STLCIncludeCS(arLsLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD_l, 0);
				T_LCOM_D LcomD_tmp = LcomD_cf;
				if (bCF)
				{
					LoadCaseNo = GetCountLcomCase(LcomD_tmp);
					for (nc = 0; nc < m_nLsCount; nc++) Set_STLCIncludeCS(arLsLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD_tmp, 0);
				}
				if (nLS > 0)
				{
					strDesc.Format(_T("+%sLS"), GetFactorString(dFactor)); LcomD_l.Description += strDesc;
					if (bCF) LcomD_tmp.Description += strDesc;
					SetCvlESCGB19_ETC_M(LcomD_l, LcomD_tmp, bCF, arCrlLoad, arMCrlLoad, nType != 2, arBrkLoad, bLive, arWLoad, FALSE, arWlLoad, TRUE, arSfLoad, TRUE, arIpLoad, TRUE,
						arTLoad, arTpgLoad, FALSE, arFrLoad, FALSE, 6, dF, nType == 2 || nType == 4, nMsgType);
				}
			}

			if (nType != 2 && (m_nCrlCount > 0 || m_nMCrlCount > 0)) // Crowd Load
			{
				T_LCOM_D LcomD_l = LcomD; LoadCaseNo = GetCountLcomCase(LcomD_l);
				int nCRL = 0;
				for (nc = 0; nc < m_nCrlCount; nc++) nCRL += Set_STLCIncludeCS(arCrlLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD_l, 0);
				T_LCOM_D LcomD_tmp = LcomD_cf;
				if (bCF)
				{
					LoadCaseNo = GetCountLcomCase(LcomD_tmp);
					for (nc = 0; nc < m_nCrlCount; nc++) Set_STLCIncludeCS(arCrlLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD_tmp, 0);
				}
				if (nCRL > 0 || m_nMCrlCount > 0)
				{
					if (nCRL > 0)
					{
						strDesc.Format(_T("+%sCRL"), GetFactorString(dFactor)); LcomD_l.Description += strDesc;
						if (bCF) LcomD_tmp.Description += strDesc;
					}

					int nMCrlCount = (m_nMCrlCount > 0) ? m_nMCrlCount : ((nCRL > 0) ? 1 : 0);
					T_LCOM_D LcomD_tmp2;
					for (int crl = 0; crl < nMCrlCount; ++crl)
					{
						T_LCOM_D LcomD_crl = LcomD_l; LoadCaseNo = GetCountLcomCase(LcomD_crl);
						if (m_nMCrlCount > 0)
						{
							Set_MovingCase(arMCrlLoad.GetAt(crl), dFactor, LoadCaseNo, LcomD_crl, TRUE);
							strDesc.Format(_T("+%sMCRL"), GetFactorString(dFactor)); LcomD_crl.Description += strDesc;
							if (m_nMCrlCount > 1) { strDesc.Format(_T("[%d]"), crl + 1); LcomD_crl.Description += strDesc; }

							if (bCF)
							{
								LcomD_tmp2 = LcomD_tmp; LoadCaseNo = GetCountLcomCase(LcomD_tmp2);
								Set_MovingCase(arMCrlLoad.GetAt(crl), dFactor, LoadCaseNo, LcomD_tmp2, TRUE);
								strDesc.Format(_T("+%sMCRL"), GetFactorString(dFactor)); LcomD_tmp2.Description += strDesc;
								if (m_nMCrlCount > 1) { strDesc.Format(_T("[%d]"), crl + 1); LcomD_tmp2.Description += strDesc; }
							}
						}

						SetCvlESCGB19_ETC_M(LcomD_crl, LcomD_tmp2, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, bLive, arWLoad, FALSE, arWlLoad, TRUE, arSfLoad, TRUE, arIpLoad, TRUE,
							arTLoad, arTpgLoad, FALSE, arFrLoad, FALSE, 7, dF, nType == 2 || nType == 4, nMsgType);

					}
				}
			}

			{
				int nWlCount = m_nWlCount * 2; // wind
				if (m_nWlCount == 0 && bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(WL))) nWlCount = 1;
				for (nc = 0; nc < nWlCount; nc++)
				{
					T_LCOM_D LcomD_l = LcomD; LoadCaseNo = GetCountLcomCase(LcomD_l);
					if (m_nWCount > 0)
					{
						double dWF = (nc % 2 == 0) ? dWlFactor : -1.0*dWlFactor;
						if (Set_STLCIncludeCS(arWlLoad.GetAt(nc / 2), dWF, LoadCaseNo, LcomD_l, 0) > 0)
						{
							CString strW = (dWF > 0.0) ? _T("+%sWL") : _T("%sWL");
							strDesc.Format(strW, GetFactorString(dWF)); LcomD_l.Description += strDesc;
							if (m_nWlCount > 1) { strDesc.Format(_T("[%d]"), nc / 2 + 1); LcomD_l.Description += strDesc; }
						}
					}
					if (bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(WL)))
					{
						Set_CStageCase(m_aCSLoad.GetAt(nnn), dWlFactor, LoadCaseNo, LcomD_l, TRUE);
						strDesc.Format(_T("+%s(cEL)"), GetFactorString(dWlFactor)); LcomD_l.Description += strDesc;
					}
					SetCvlESCGB19_ETC_M(LcomD_l, LcomD_cf, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWLoad, FALSE, arWlLoad, FALSE, arSfLoad, TRUE, arIpLoad, TRUE,
						arTLoad, arTpgLoad, FALSE, arFrLoad, FALSE, 7, dF, nType == 2 || nType == 4, nMsgType);
				}
			}
			if (!bExsitMV)
			{
				int nWCount = m_nWCount * 2; // wind
				if (m_nWCount == 0 && bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(W))) nWCount = 1;
				for (nc = 0; nc < nWCount; nc++)
				{
					T_LCOM_D LcomD_l = LcomD; LoadCaseNo = GetCountLcomCase(LcomD_l);
					if (m_nWCount > 0)
					{
						double dWF = (nc % 2 == 0) ? dWFactor : -1.*dWFactor;
						if (Set_STLCIncludeCS(arWLoad.GetAt(nc / 2), dWF, LoadCaseNo, LcomD_l, 0) > 0)
						{
							CString strW = (dWF > 0.0) ? _T("+%sW") : _T("%sW");
							strDesc.Format(strW, GetFactorString(dWF)); LcomD_l.Description += strDesc;
							if (m_nWCount > 1) { strDesc.Format(_T("[%d]"), nc / 2 + 1); LcomD_l.Description += strDesc; }
						}
					}
					if (bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(W)))
					{
						Set_CStageCase(m_aCSLoad.GetAt(nnn), dWFactor, LoadCaseNo, LcomD_l, TRUE);
						strDesc.Format(_T("+%s(cEL)"), GetFactorString(dWFactor)); LcomD_l.Description += strDesc;
					}
					SetCvlESCGB19_ETC_M(LcomD_l, LcomD_cf, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWLoad, FALSE, arWlLoad, FALSE, arSfLoad, TRUE, arIpLoad, TRUE,
						arTLoad, arTpgLoad, FALSE, arFrLoad, FALSE, 7, dF, nType == 2 || nType == 4, nMsgType);
				}
			}
		} // next m
	} // next lc

	return TRUE;
}

BOOL CLoadCombCtrl::SetCvlESCGB19_Var56_M(const CArray<T_LCOM_D, T_LCOM_D&>& arData, ArUNT& arMLoad, ArINT& arLLoad,
	ArINT& arIlLoad, ArINT& arCrlLoad, ArINT& arMCrlLoad, ArINT& arCfLoad, ArINT& arLsLoad,
	ArINT& arWLoad, ArINT& arWlLoad, ArINT& arSfLoad, ArINT& arIpLoad, ArINT& arTLoad, ArINT& arTpgLoad,
	ArINT& arFrLoad, ArINT& arBrkLoad, int nType, int nMsgType)
{
	// nType == 0,4: °è¼öÁ¶ÇÕ, 1: 1.0(settlement Á¦¿Ü), 2: 1.0(¸ðµÎ), Live Load Á¦¿Ü
	CArray<T_LCOM_D, T_LCOM_D&> aLcomD; aLcomD.Copy(arData);
	int  nLcomSize = aLcomD.GetSize();
	int  nnn = 0;
	//BOOL bAddCsLoad = (m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER1_CVL,nnn)) || (m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER2_CVL,nnn)) || (m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER3_CVL,nnn)); // CS or ST+CS
	BOOL bAddCsLoad = m_nConstLoad != 0 && CheckCS_SgldK(D_SGLD_ER1_CVL, nnn); // °üº¬Ê©¹¤ºÉÔØÇÒ´æÔÚÊ©¹¤ºÉÔØ

	double dFactor = (nType == 0 || nType == 4) ? 1.4*0.75 : 1.0;
	double dWFactor = (nType == 0 || nType == 4) ? 1.4*1.0 : 1.0;
	double dLFactor = (nType == 0 || nType == 4) ? 1.4 : 1.0;
	double dF = (nType == 0 || nType == 4) ? 0.75 : 1.0;
	double dMLFactor = (nType == 0 || nType == 4) ? 1.1 : 1.0;
	double dIL = (nType == 12) ? 0. : CalcMovingILFactor();

	double dWlFactor = (nType == 0 || nType == 4) ? 1.1 * 1.0 : 1.0;// ·çºÉÔØ
	BOOL bUseWlLoad = m_nMCount ? TRUE : FALSE;

	if (nMsgType == 114 && m_bJTG15_Factor)
	{
		dFactor *= m_dJTG15_Factor;
		dWFactor *= m_dJTG15_Factor;
		dWlFactor *= m_dJTG15_Factor;
		dLFactor *= m_dJTG15_Factor;
		dF *= m_dJTG15_Factor;
		dMLFactor *= m_dJTG15_Factor;
	}

	ArINT arTmp;
	// ¿µ±¸ÇÏÁß + Â÷·®ÇÏÁß + 5°³ÀÇ °¡º¯ÇÏÁßÀÎ °æ¿ì
	CString strDesc = _T("");
	int LoadCaseNo = 0, nc = 0;
	for (int lc = 0; lc < nLcomSize; lc++)
	{
		int nMCount = max(m_nMCount, 1);
		for (int m = 0; m < nMCount; m++)
		{
			T_LCOM_D LcomD = aLcomD.GetAt(lc); LcomD.nActive = 1; // Active or Strength/Stress
			T_LCOM_D LcomD_cf = LcomD; // MNET:1614, KJH20050809, CFÁ¦¿ÜÇÑ ÇÏÁßÁ¶ÇÕ
			LoadCaseNo = GetCountLcomCase(LcomD);

			double dMlFactor = 1.4;               // Æû³µºÉÔØ×éºÏÏµÊý
			CString strJudgeMILFactorByLoadCase = GetJTGD602015MILoadName(arMLoad, m);
			if (nType == 0 || nType == 1)         // ³ÐÔØÄÜÁ¦×´Ì¬  »ù±¾×éºÏ
			{
				if (strJudgeMILFactorByLoadCase == _T("CH-CL") || strJudgeMILFactorByLoadCase == _T("ÓÃ»§¶¨Òå"))
				{
					dMlFactor = 1.8;                  // ³µÁ¾ºÉÔØ  ÓÃ»§¶¨Òå   È¡1.8
				}
			}
			if (nType == 17)  dMlFactor = 1.0;
			if (nMsgType == 114 && m_bJTG15_Factor)
			{
				dMlFactor *= m_dJTG15_Factor;
			}

			BOOL bLive = FALSE, bCF = FALSE;
			//BOOL bFindCJJLeadingMLoad = FALSE;
			if (nType != 2)
			{
				// moving load
				if (m_nMCount > 0)
				{
					Set_MovingCase(arMLoad.GetAt(m), dMlFactor, LoadCaseNo, LcomD, TRUE);
					strDesc.Format(_T("+%sM"), GetFactorString(dMlFactor)); LcomD.Description += strDesc;
					if (m_nMCount > 1) { strDesc.Format(_T("[%d]"), m + 1); LcomD.Description += strDesc; }
				}

				int nLL = 0, nIL = 0, nCF = 0;
				if (m_nLCount > 0 || (bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(L)))) // ST or ST+CS
				{
					for (nc = 0; nc < m_nLCount; nc++) nLL += Set_STLCIncludeCS(arLLoad.GetAt(nc), dLFactor, LoadCaseNo, LcomD, 0);
					if (nLL > 0) { strDesc.Format(_T("+%sL"), GetFactorString(dLFactor)); LcomD.Description += strDesc; }
					if (bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(L)))
					{
						Set_CStageCase(m_aCSLoad.GetAt(nnn), dLFactor, LoadCaseNo, LcomD, TRUE);
						strDesc.Format(_T("+%s(cEL)"), GetFactorString(dLFactor)); LcomD.Description += strDesc;
					}
					for (nc = 0; nc < m_nIlCount; nc++) nIL += Set_STLCIncludeCS(arIlLoad.GetAt(nc), dLFactor, LoadCaseNo, LcomD, 0);
					if (nIL > 0) { strDesc.Format(_T("+%sIL"), GetFactorString(dLFactor)); LcomD.Description += strDesc; }
				}
				bLive = m_nMCount > 0 || (m_nLCount > 0 || (bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(L))));
				if (bLive)
				{
					LcomD_cf = LcomD;
					for (nc = 0; nc < m_nCfCount; nc++) nCF += Set_STLCIncludeCS(arCfLoad.GetAt(nc), dLFactor, LoadCaseNo, LcomD, 0);
					if (nCF > 0) { strDesc.Format(_T("+%sCF"), GetFactorString(dLFactor)); LcomD.Description += strDesc; bCF = TRUE; }
				}
			}

			if (m_nLsCount > 0) // Live Load Surcharge
			{
				T_LCOM_D LcomD_l = LcomD; LoadCaseNo = GetCountLcomCase(LcomD_l);
				int nLS = 0;
				for (nc = 0; nc < m_nLsCount; nc++) nLS += Set_STLCIncludeCS(arLsLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD_l, 0);
				T_LCOM_D LcomD_tmp = LcomD_cf;
				if (bCF)
				{
					LoadCaseNo = GetCountLcomCase(LcomD_tmp);
					for (nc = 0; nc < m_nLsCount; nc++) Set_STLCIncludeCS(arLsLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD_tmp, 0);
				}
				if (nLS > 0)
				{
					strDesc.Format(_T("+%sLS"), GetFactorString(dFactor)); LcomD_l.Description += strDesc;
					if (bCF) LcomD_tmp.Description += strDesc;
					SetCvlESCGB19_ETC_M(LcomD_l, LcomD_tmp, bCF, arCrlLoad, arMCrlLoad, nType != 2, arBrkLoad, bLive, arWLoad, FALSE, arWlLoad, TRUE, arSfLoad, FALSE, arIpLoad, FALSE,
						arTLoad, arTpgLoad, FALSE, arFrLoad, FALSE, 8, dF, nType == 2 || nType == 4, nMsgType);
				}
			}
			// MNET:XXXX-BJLEE(BBONG)-20060503
			if (nType != 2 && (m_nCrlCount > 0 || m_nMCrlCount > 0)) // Crowd Load
			{
				T_LCOM_D LcomD_l = LcomD; LoadCaseNo = GetCountLcomCase(LcomD_l);
				int nCRL = 0;
				for (nc = 0; nc < m_nCrlCount; nc++) nCRL += Set_STLCIncludeCS(arCrlLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD_l, 0);
				T_LCOM_D LcomD_tmp = LcomD_cf;
				// MNET:XXXX-BJLEE(BBONG)-20060502 
				if (nCRL > 0 || m_nMCrlCount > 0)
				{
					if (nCRL > 0)
					{
						strDesc.Format(_T("+%sCRL"), GetFactorString(dFactor)); LcomD_l.Description += strDesc;
						if (bCF) LcomD_tmp.Description += strDesc;
					}

					int nMCrlCount = (m_nMCrlCount > 0) ? m_nMCrlCount : ((nCRL > 0) ? 1 : 0);
					T_LCOM_D LcomD_tmp2;
					for (int crl = 0; crl < nMCrlCount; ++crl)
					{
						T_LCOM_D LcomD_crl = LcomD_l; LoadCaseNo = GetCountLcomCase(LcomD_crl);
						if (m_nMCrlCount > 0)
						{
							Set_MovingCase(arMCrlLoad.GetAt(crl), dFactor, LoadCaseNo, LcomD_crl, TRUE);
							strDesc.Format(_T("+%sMCRL"), GetFactorString(dFactor)); LcomD_crl.Description += strDesc;
							if (m_nMCrlCount > 1) { strDesc.Format(_T("[%d]"), crl + 1); LcomD_crl.Description += strDesc; }

							if (bCF)
							{
								LcomD_tmp2 = LcomD_tmp; LoadCaseNo = GetCountLcomCase(LcomD_tmp2);
								Set_MovingCase(arMCrlLoad.GetAt(crl), dFactor, LoadCaseNo, LcomD_tmp2, TRUE);
								strDesc.Format(_T("+%sMCRL"), GetFactorString(dFactor)); LcomD_tmp2.Description += strDesc;
								if (m_nMCrlCount > 1) { strDesc.Format(_T("[%d]"), crl + 1); LcomD_tmp2.Description += strDesc; }
							}
						}
						//if((nCRL>0 || m_nMCrlCount>0))
						//{
						SetCvlESCGB19_ETC_M(LcomD_l, LcomD_cf, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWLoad, FALSE, arWlLoad, TRUE, arSfLoad, FALSE, arIpLoad, FALSE,
							arTLoad, arTpgLoad, FALSE, arFrLoad, FALSE, 9, dF, nType == 2 || nType == 4, nMsgType);
						//}
					}
				}
			}
		} // next m
	} // next lc

	// ¿µ±¸ÇÏÁß + Â÷·®ÇÏÁß + 6°³ÀÇ °¡º¯ÇÏÁßÀÎ °æ¿ì
	for (int lc = 0; lc < nLcomSize; lc++)
	{
		int nMCount = max(m_nMCount, 1);
		for (int m = 0; m < nMCount; m++)
		{
			T_LCOM_D LcomD = aLcomD.GetAt(lc); LcomD.nActive = 1; // Active or Strength/Stress
			LoadCaseNo = GetCountLcomCase(LcomD);

			if (nType != 2)
			{
				// moving load
				if (m_nMCount > 0)
				{
					Set_MovingCase(arMLoad.GetAt(m), dLFactor, LoadCaseNo, LcomD, TRUE);
					strDesc.Format(_T("+%sM"), GetFactorString(dLFactor)); LcomD.Description += strDesc;
					if (m_nMCount > 1) { strDesc.Format(_T("[%d]"), m + 1); LcomD.Description += strDesc; }
				}
				if (m_nLCount > 0 || (bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(L)))) // ST or ST+CS
				{
					int nLL = 0, nIL = 0, nCF = 0;
					for (nc = 0; nc < m_nLCount; nc++) nLL += Set_STLCIncludeCS(arLLoad.GetAt(nc), dLFactor, LoadCaseNo, LcomD, 0);
					if (nLL > 0) { strDesc.Format(_T("+%sL"), GetFactorString(dLFactor)); LcomD.Description += strDesc; }
					if (bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(L)))
					{
						Set_CStageCase(m_aCSLoad.GetAt(nnn), dLFactor, LoadCaseNo, LcomD, TRUE);
						strDesc.Format(_T("+%s(cEL)"), GetFactorString(dLFactor)); LcomD.Description += strDesc;
					}
					for (nc = 0; nc < m_nIlCount; nc++) nIL += Set_STLCIncludeCS(arIlLoad.GetAt(nc), dLFactor, LoadCaseNo, LcomD, 0);
					if (nIL > 0) { strDesc.Format(_T("+%sIL"), GetFactorString(dLFactor)); LcomD.Description += strDesc; }
					for (nc = 0; nc < m_nCfCount; nc++) nCF += Set_STLCIncludeCS(arCfLoad.GetAt(nc), dLFactor, LoadCaseNo, LcomD, 0);
					if (nCF > 0) { strDesc.Format(_T("+%sCF"), GetFactorString(dLFactor)); LcomD.Description += strDesc; }
				}
			}

			if (m_nLsCount > 0) // Live Load Surcharge
			{
				T_LCOM_D LcomD_l = LcomD; LoadCaseNo = GetCountLcomCase(LcomD_l);
				int nLS = 0;
				for (nc = 0; nc < m_nLsCount; nc++) nLS += Set_STLCIncludeCS(arLsLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD_l, 0);
				if (nLS > 0)
				{
					strDesc.Format(_T("+%sLS"), GetFactorString(dFactor)); LcomD_l.Description += strDesc;
					SetCvlESCGB19_ETC_M(LcomD_l, LcomD_l, FALSE, arCrlLoad, arMCrlLoad, nType != 2, arBrkLoad, FALSE, arWLoad, FALSE, arWlLoad, FALSE, arSfLoad, FALSE, arIpLoad, FALSE,
						arTLoad, arTpgLoad, FALSE, arFrLoad, FALSE, 10, dF, nType == 2 || nType == 4, nMsgType);
				}
			}
		}
	}

	return TRUE;
}

BOOL CLoadCombCtrl::SetCvlESCGB19_ETC_M(T_LCOM_D rData, T_LCOM_D Lcom_cf, BOOL bCF, ArINT& arCrlLoad, ArINT& arMCrlLoad, BOOL bCrl, ArINT& arBrkLoad, BOOL bBrk,
	ArINT& arWLoad, BOOL bW, ArINT& arWlLoad, BOOL bWl, ArINT& arSfLoad, BOOL bSf, ArINT& arIpLoad, BOOL bIp, ArINT& arTLoad, ArINT& arTpgLoad,
	BOOL bT, ArINT& arFrLoad, BOOL bFr, int nType, double dFactor, BOOL bAcdn, int nMsgType)
{
	int LoadCaseNo = 0, nc = 0, nnn = 0;
	CString strDesc = _T("");
	bool bChange = m_bJTG15_Factor ? (dFactor / m_dJTG15_Factor < 1.0) : (dFactor < 1.0);
	double dF = bChange ? 1.4*dFactor : 1.0;
	double dTepFcator = nMsgType == 115 ? 0.8 : dF;//nMsgType == 115 ´ú±íÅ¼È»×éºÏ
	double dTFactor = nMsgType == 115 ? 1.0 : dTepFcator;
	//double dTepF = nType == 0 ? 1.4*0.75 : 1.4;
	double dWF = bChange ? 1.4*1.0 : 1.0;
	double dWlF = bChange ? 1.1*1.0 : 1.0;

	BOOL bAddCsLoad = m_nConstLoad != 0 && CheckCS_SgldK(D_SGLD_ER1_CVL, nnn); // CS or ST+CS

	if ((m_nMCrlCount || m_nCrlCount > 0) && bCrl)
	{
		T_LCOM_D LcomD = rData;
		LoadCaseNo = GetCountLcomCase(LcomD);
		int nCRL = 0;
		for (nc = 0; nc < m_nCrlCount; nc++) nCRL += Set_STLCIncludeCS(arCrlLoad.GetAt(nc), dF, LoadCaseNo, LcomD, 0);
		T_LCOM_D LcomD_tmp = Lcom_cf;
		if (bCF && bBrk)
		{
			LoadCaseNo = GetCountLcomCase(LcomD_tmp);
			for (nc = 0; nc < m_nCrlCount; nc++) Set_STLCIncludeCS(arCrlLoad.GetAt(nc), dF, LoadCaseNo, LcomD_tmp, 0);
		}
		if (nCRL > 0)
		{
			strDesc.Format(_T("+%sCRL"), GetFactorString(dF)); LcomD.Description = LcomD.Description + strDesc;
			if (bCF && bBrk) LcomD_tmp.Description = LcomD_tmp.Description + strDesc;
		}
		int nMCrlCount = (m_nMCrlCount > 0) ? m_nMCrlCount : ((nCRL > 0) ? 1 : 0);
		for (int crl = 0; crl < nMCrlCount; ++crl)
		{
			T_LCOM_D LcomD_crl = LcomD;
			T_LCOM_D LcomD_crltmp = LcomD_tmp;
			if (m_nMCrlCount > 0)
			{
				Set_MovingCase(arMCrlLoad.GetAt(crl), dF, LoadCaseNo, LcomD_crl, TRUE);
				strDesc.Format(_T("+%sMCRL"), GetFactorString(dF)); LcomD_crl.Description = LcomD_crl.Description + strDesc; LcomD_crltmp.Description = LcomD_crltmp.Description + strDesc;
				if (m_nMCrlCount > 1) { strDesc.Format(_T("[%d]"), crl + 1); LcomD_crl.Description = LcomD_crl.Description + strDesc; LcomD_crltmp.Description = LcomD_crltmp.Description + strDesc; }
			}
			if (nCRL > 0 || m_nMCrlCount > 0)
			{
				if (nType == 1) SetCvlESCGB19_ETC_M(LcomD_crl, LcomD_crltmp, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, bBrk, arWLoad, FALSE, arWlLoad, TRUE, arSfLoad, TRUE, arIpLoad, TRUE,
					arTLoad, arTpgLoad, TRUE, arFrLoad, TRUE, 0, dFactor, bAcdn, nMsgType);
				if (nType == 6) SetCvlESCGB19_ETC_M(LcomD_crl, LcomD_crltmp, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, bBrk, arWLoad, FALSE, arWlLoad, TRUE, arSfLoad, TRUE, arIpLoad, TRUE,
					arTLoad, arTpgLoad, TRUE, arFrLoad, FALSE, 2, dFactor, bAcdn, nMsgType);
				if (nType == 8) SetCvlESCGB19_ETC_M(LcomD_crl, LcomD_crltmp, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, bBrk, arWLoad, FALSE, arWlLoad, TRUE, arSfLoad, TRUE, arIpLoad, TRUE,
					arTLoad, arTpgLoad, FALSE, arFrLoad, FALSE, 6, dFactor, bAcdn, nMsgType);
				if (nType == 10) SetCvlESCGB19_ETC_M(LcomD_crl, LcomD_crltmp, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWLoad, FALSE, arWlLoad, TRUE, arSfLoad, FALSE, arIpLoad, FALSE,
					arTLoad, arTpgLoad, FALSE, arFrLoad, FALSE, 8, dFactor, bAcdn, nMsgType);
				if (nType == 101) SetCvlESCGB19_ETC_M(LcomD_crl, LcomD_crltmp, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, bBrk, arWLoad, FALSE, arWlLoad, FALSE, arSfLoad, FALSE, arIpLoad, FALSE,
					arTLoad, arTpgLoad, FALSE, arFrLoad, FALSE, 102, dFactor, bAcdn, nMsgType);
				if (nType == 103) SetCvlESCGB19_ETC_M(LcomD_crl, LcomD_crltmp, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, bBrk, arWLoad, FALSE, arWlLoad, TRUE, arSfLoad, FALSE, arIpLoad, FALSE,
					arTLoad, arTpgLoad, FALSE, arFrLoad, FALSE, 104, dFactor, bAcdn, nMsgType);
				if (nType == 0)
				{
					if (bAcdn) m_aAcdnLcomD.Add(LcomD_crl);
					else      Set_LoadComb4Civil(LcomD_crl, D_STR, 0, nMsgType);
				}
			}
		}
	}

	if (nType == 8) bBrk = FALSE;
	if (m_nBrkCount > 0 && bBrk)
	{
		for (int i = 0; i < 2; ++i)
		{
			if (i == 1 && !bCF) break;
			T_LCOM_D LcomD = (i == 0) ? rData : Lcom_cf;
			LoadCaseNo = GetCountLcomCase(LcomD);
			double dBrkF = (i == 0) ? (bCF ? dF * 0.7 : dF) : dF;
			int nBRK = 0;
			for (nc = 0; nc < m_nBrkCount; nc++) nBRK += Set_STLCIncludeCS(arBrkLoad.GetAt(nc), dBrkF, LoadCaseNo, LcomD, 0);
			if (nBRK > 0) { strDesc.Format(_T("+%sBRK"), GetFactorString(dBrkF)); LcomD.Description = LcomD.Description + strDesc; }
			if (nType == 1) SetCvlESCGB19_ETC_M(LcomD, Lcom_cf, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWLoad, FALSE, arWlLoad, TRUE, arSfLoad, FALSE, arIpLoad, FALSE,
				arTLoad, arTpgLoad, TRUE, arFrLoad, FALSE, 0, dFactor, bAcdn, nMsgType);
			if (nType == 2) SetCvlESCGB19_ETC_M(LcomD, Lcom_cf, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWLoad, FALSE, arWlLoad, TRUE, arSfLoad, FALSE, arIpLoad, FALSE,
				arTLoad, arTpgLoad, TRUE, arFrLoad, FALSE, 0, dFactor, bAcdn, nMsgType);
			if (nType == 6) SetCvlESCGB19_ETC_M(LcomD, Lcom_cf, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWLoad, FALSE, arWlLoad, TRUE, arSfLoad, FALSE, arIpLoad, FALSE,
				arTLoad, arTpgLoad, FALSE, arFrLoad, FALSE, 3, dFactor, bAcdn, nMsgType);
			if (nType == 7) SetCvlESCGB19_ETC_M(LcomD, Lcom_cf, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWLoad, FALSE, arWlLoad, TRUE, arSfLoad, FALSE, arIpLoad, FALSE,
				arTLoad, arTpgLoad, FALSE, arFrLoad, FALSE, 3, dFactor, bAcdn, nMsgType);
			if (nType == 8) SetCvlESCGB19_ETC_M(LcomD, Lcom_cf, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWLoad, FALSE, arWlLoad, TRUE, arSfLoad, FALSE, arIpLoad, FALSE,
				arTLoad, arTpgLoad, FALSE, arFrLoad, FALSE, 3, dFactor, bAcdn, nMsgType);
			if (nType == 101) SetCvlESCGB19_ETC_M(LcomD, Lcom_cf, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWLoad, FALSE, arWlLoad, TRUE, arSfLoad, FALSE, arIpLoad, FALSE,
				arTLoad, arTpgLoad, FALSE, arFrLoad, FALSE, 3, dFactor, bAcdn, nMsgType);
			if (nType == 102) SetCvlESCGB19_ETC_M(LcomD, Lcom_cf, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWLoad, FALSE, arWlLoad, TRUE, arSfLoad, FALSE, arIpLoad, FALSE,
				arTLoad, arTpgLoad, TRUE, arFrLoad, FALSE, 0, dFactor, bAcdn, nMsgType);
			if (nType == 103) SetCvlESCGB19_ETC_M(LcomD, Lcom_cf, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWLoad, FALSE, arWlLoad, TRUE, arSfLoad, FALSE, arIpLoad, FALSE,
				arTLoad, arTpgLoad, FALSE, arFrLoad, FALSE, 3, dFactor, bAcdn, nMsgType);
			if (nType == 104) SetCvlESCGB19_ETC_M(LcomD, Lcom_cf, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWLoad, FALSE, arWlLoad, TRUE, arSfLoad, FALSE, arIpLoad, FALSE,
				arTLoad, arTpgLoad, FALSE, arFrLoad, FALSE, 3, dFactor, bAcdn, nMsgType);
			if (nType == 0)
			{
				if (bAcdn) m_aAcdnLcomD.Add(LcomD);
				else      Set_LoadComb4Civil(LcomD, D_STR, 0, nMsgType);
			}
		}
	}

	if (bW)
	{
		int nWCount = m_nWCount * 2; // Wind
		if (m_nWCount == 0 && bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(W))) nWCount = 1;
		for (nc = 0; nc < nWCount; nc++)
		{
			T_LCOM_D LcomD = rData;
			LoadCaseNo = GetCountLcomCase(LcomD);
			int nWL = 0;
			if (m_nWCount > 0)
			{
				double dWF1 = (nc % 2 == 0) ? dWF : -1.*dWF;
				if (Set_STLCIncludeCS(arWLoad.GetAt(nc / 2), dWF1, LoadCaseNo, LcomD, 0) > 0)
				{
					CString strW = (dWF1 > 0.0) ? _T("+%sW") : _T("%sW");
					strDesc.Format(strW, GetFactorString(dWF1)); LcomD.Description = LcomD.Description + strDesc;
					if (m_nWCount > 1) { strDesc.Format(_T("[%d]"), nc / 2 + 1); LcomD.Description = LcomD.Description + strDesc; }
				}
			}
			if (bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(W)))
			{
				Set_CStageCase(m_aCSLoad.GetAt(nnn), dWF, LoadCaseNo, LcomD, TRUE);
				strDesc.Format(_T("+%s(cEL)"), GetFactorString(dWF)); LcomD.Description = LcomD.Description + strDesc;
			}
			if (nType == 1) SetCvlESCGB19_ETC_M(LcomD, Lcom_cf, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWLoad, FALSE, arWlLoad, FALSE, arSfLoad, TRUE, arIpLoad, TRUE,
				arTLoad, arTpgLoad, TRUE, arFrLoad, TRUE, 0, dFactor, bAcdn, nMsgType);
			if (nType == 2) SetCvlESCGB19_ETC_M(LcomD, Lcom_cf, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWLoad, FALSE, arWlLoad, FALSE, arSfLoad, TRUE, arIpLoad, TRUE,
				arTLoad, arTpgLoad, TRUE, arFrLoad, TRUE, 0, dFactor, bAcdn, nMsgType);
			if (nType == 3) SetCvlESCGB19_ETC_M(LcomD, Lcom_cf, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWLoad, FALSE, arWlLoad, FALSE, arSfLoad, FALSE, arIpLoad, FALSE,
				arTLoad, arTpgLoad, TRUE, arFrLoad, FALSE, 0, dFactor, bAcdn, nMsgType);
			if (nType == 6) SetCvlESCGB19_ETC_M(LcomD, Lcom_cf, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWLoad, FALSE, arWlLoad, FALSE, arSfLoad, TRUE, arIpLoad, TRUE,
				arTLoad, arTpgLoad, TRUE, arFrLoad, FALSE, 2, dFactor, bAcdn, nMsgType);
			if (nType == 7) SetCvlESCGB19_ETC_M(LcomD, Lcom_cf, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWLoad, FALSE, arWlLoad, FALSE, arSfLoad, TRUE, arIpLoad, TRUE,
				arTLoad, arTpgLoad, TRUE, arFrLoad, FALSE, 2, dFactor, bAcdn, nMsgType);
			if (nType == 8) SetCvlESCGB19_ETC_M(LcomD, Lcom_cf, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWLoad, FALSE, arWlLoad, FALSE, arSfLoad, TRUE, arIpLoad, TRUE,
				arTLoad, arTpgLoad, FALSE, arFrLoad, FALSE, 6, dFactor, bAcdn, nMsgType);
			if (nType == 9) SetCvlESCGB19_ETC_M(LcomD, Lcom_cf, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWLoad, FALSE, arWlLoad, FALSE, arSfLoad, TRUE, arIpLoad, TRUE,
				arTLoad, arTpgLoad, FALSE, arFrLoad, FALSE, 6, dFactor, bAcdn, nMsgType);
			if (nType == 0)
			{
				if (bAcdn) m_aAcdnLcomD.Add(LcomD);
				else      Set_LoadComb4Civil(LcomD, D_STR, 0, nMsgType);
			}
		}
	}

	if (bWl)
	{
		int nWlCount = m_nWlCount * 2; // Wind
		if (m_nWlCount == 0 && bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(WL))) nWlCount = 1;
		for (nc = 0; nc < nWlCount; nc++)
		{
			T_LCOM_D LcomD = rData;
			LoadCaseNo = GetCountLcomCase(LcomD);
			int nWL = 0;
			if (m_nWlCount > 0)
			{
				double dWF1 = (nc % 2 == 0) ? dWlF : -1.*dWlF;
				if (Set_STLCIncludeCS(arWlLoad.GetAt(nc / 2), dWF1, LoadCaseNo, LcomD, 0) > 0)
				{
					CString strW = (dWF1 > 0.0) ? _T("+%sWL") : _T("%sWL");
					strDesc.Format(strW, GetFactorString(dWF1)); LcomD.Description = LcomD.Description + strDesc;
					if (m_nWlCount > 1) { strDesc.Format(_T("[%d]"), nc / 2 + 1); LcomD.Description = LcomD.Description + strDesc; }
				}
			}
			if (bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(WL)))
			{
				Set_CStageCase(m_aCSLoad.GetAt(nnn), dWlF, LoadCaseNo, LcomD, TRUE);
				strDesc.Format(_T("+%s(cEL)"), GetFactorString(dWlF)); LcomD.Description = LcomD.Description + strDesc;
			}
			if (nType == 1) SetCvlESCGB19_ETC_M(LcomD, Lcom_cf, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWLoad, FALSE, arWlLoad, FALSE, arSfLoad, TRUE, arIpLoad, TRUE,
				arTLoad, arTpgLoad, TRUE, arFrLoad, TRUE, 0, dFactor, bAcdn, nMsgType);
			if (nType == 2) SetCvlESCGB19_ETC_M(LcomD, Lcom_cf, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWLoad, FALSE, arWlLoad, FALSE, arSfLoad, TRUE, arIpLoad, TRUE,
				arTLoad, arTpgLoad, TRUE, arFrLoad, TRUE, 0, dFactor, bAcdn, nMsgType);
			if (nType == 3) SetCvlESCGB19_ETC_M(LcomD, Lcom_cf, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWLoad, FALSE, arWlLoad, FALSE, arSfLoad, FALSE, arIpLoad, FALSE,
				arTLoad, arTpgLoad, TRUE, arFrLoad, FALSE, 0, dFactor, bAcdn, nMsgType);
			if (nType == 6) SetCvlESCGB19_ETC_M(LcomD, Lcom_cf, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWLoad, FALSE, arWlLoad, FALSE, arSfLoad, TRUE, arIpLoad, TRUE,
				arTLoad, arTpgLoad, TRUE, arFrLoad, FALSE, 2, dFactor, bAcdn, nMsgType);
			if (nType == 7) SetCvlESCGB19_ETC_M(LcomD, Lcom_cf, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWLoad, FALSE, arWlLoad, FALSE, arSfLoad, TRUE, arIpLoad, TRUE,
				arTLoad, arTpgLoad, TRUE, arFrLoad, FALSE, 2, dFactor, bAcdn, nMsgType);
			if (nType == 8) SetCvlESCGB19_ETC_M(LcomD, Lcom_cf, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWLoad, FALSE, arWlLoad, FALSE, arSfLoad, TRUE, arIpLoad, TRUE,
				arTLoad, arTpgLoad, FALSE, arFrLoad, FALSE, 6, dFactor, bAcdn, nMsgType);
			if (nType == 9) SetCvlESCGB19_ETC_M(LcomD, Lcom_cf, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWLoad, FALSE, arWlLoad, FALSE, arSfLoad, TRUE, arIpLoad, TRUE,
				arTLoad, arTpgLoad, FALSE, arFrLoad, FALSE, 6, dFactor, bAcdn, nMsgType);
			if (nType == 0)
			{
				if (bAcdn) m_aAcdnLcomD.Add(LcomD);
				else      Set_LoadComb4Civil(LcomD, D_STR, 0, nMsgType);
			}
		}
	}

	if (m_nSfCount > 0 && bSf)
	{
		T_LCOM_D LcomD = rData;
		LoadCaseNo = GetCountLcomCase(LcomD);
		int nSF = 0;
		for (nc = 0; nc < m_nSfCount; nc++) nSF += Set_STLCIncludeCS(arSfLoad.GetAt(nc), dF, LoadCaseNo, LcomD, 0);
		if (nSF > 0) { strDesc.Format(_T("+%sSF"), GetFactorString(dF)); LcomD.Description = LcomD.Description + strDesc; }
		if (nType == 1) SetCvlESCGB19_ETC_M(LcomD, Lcom_cf, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWLoad, FALSE, arWlLoad, FALSE, arSfLoad, FALSE, arIpLoad, FALSE,
			arTLoad, arTpgLoad, TRUE, arFrLoad, TRUE, 0, dFactor, bAcdn, nMsgType);
		if (nType == 2) SetCvlESCGB19_ETC_M(LcomD, Lcom_cf, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWLoad, FALSE, arWlLoad, FALSE, arSfLoad, FALSE, arIpLoad, FALSE,
			arTLoad, arTpgLoad, TRUE, arFrLoad, TRUE, 0, dFactor, bAcdn, nMsgType);
		if (nType == 4) SetCvlESCGB19_ETC_M(LcomD, Lcom_cf, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWLoad, FALSE, arWlLoad, FALSE, arSfLoad, FALSE, arIpLoad, FALSE,
			arTLoad, arTpgLoad, TRUE, arFrLoad, TRUE, 0, dFactor, bAcdn, nMsgType);
		if (nType == 6) SetCvlESCGB19_ETC_M(LcomD, Lcom_cf, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWLoad, FALSE, arWlLoad, FALSE, arSfLoad, FALSE, arIpLoad, FALSE,
			arTLoad, arTpgLoad, TRUE, arFrLoad, FALSE, 2, dFactor, bAcdn, nMsgType);
		if (nType == 7) SetCvlESCGB19_ETC_M(LcomD, Lcom_cf, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWLoad, FALSE, arWlLoad, FALSE, arSfLoad, FALSE, arIpLoad, FALSE,
			arTLoad, arTpgLoad, TRUE, arFrLoad, FALSE, 2, dFactor, bAcdn, nMsgType);
		if (nType == 0)
		{
			if (bAcdn) m_aAcdnLcomD.Add(LcomD);
			else      Set_LoadComb4Civil(LcomD, D_STR, 0, nMsgType);
		}
	}

	if (m_nIpCount > 0 && bIp)
	{
		T_LCOM_D LcomD = rData;
		LoadCaseNo = GetCountLcomCase(LcomD);
		int nIP = 0;
		for (nc = 0; nc < m_nIpCount; nc++) nIP += Set_STLCIncludeCS(arIpLoad.GetAt(nc), dF, LoadCaseNo, LcomD, 0);
		if (nIP > 0) { strDesc.Format(_T("+%sIP"), GetFactorString(dF)); LcomD.Description = LcomD.Description + strDesc; }
		if (nType == 1) SetCvlESCGB19_ETC_M(LcomD, Lcom_cf, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWLoad, FALSE, arWlLoad, FALSE, arSfLoad, FALSE, arIpLoad, FALSE,
			arTLoad, arTpgLoad, TRUE, arFrLoad, TRUE, 0, dFactor, bAcdn, nMsgType);
		if (nType == 2) SetCvlESCGB19_ETC_M(LcomD, Lcom_cf, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWLoad, FALSE, arWlLoad, FALSE, arSfLoad, FALSE, arIpLoad, FALSE,
			arTLoad, arTpgLoad, TRUE, arFrLoad, TRUE, 0, dFactor, bAcdn, nMsgType);
		if (nType == 4) SetCvlESCGB19_ETC_M(LcomD, Lcom_cf, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWLoad, FALSE, arWlLoad, FALSE, arSfLoad, FALSE, arIpLoad, FALSE,
			arTLoad, arTpgLoad, TRUE, arFrLoad, TRUE, 0, dFactor, bAcdn, nMsgType);
		if (nType == 6) SetCvlESCGB19_ETC_M(LcomD, Lcom_cf, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWLoad, FALSE, arWlLoad, FALSE, arSfLoad, FALSE, arIpLoad, FALSE,
			arTLoad, arTpgLoad, TRUE, arFrLoad, FALSE, 2, dFactor, bAcdn, nMsgType);
		if (nType == 7) SetCvlESCGB19_ETC_M(LcomD, Lcom_cf, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWLoad, FALSE, arWlLoad, FALSE, arSfLoad, FALSE, arIpLoad, FALSE,
			arTLoad, arTpgLoad, TRUE, arFrLoad, FALSE, 2, dFactor, bAcdn, nMsgType);
		if (nType == 0)
		{
			if (bAcdn) m_aAcdnLcomD.Add(LcomD);
			else      Set_LoadComb4Civil(LcomD, D_STR, 0, nMsgType);
		}
	}

	if (bT && ((m_nTCount > 0 || m_nTpgCount > 0) || (bAddCsLoad && (CheckCSEL(D_SGLD_ER1_CVL, _LSX(T)) || CheckCSEL(D_SGLD_ER1_CVL, _LSX(TPG))))))
	{
		T_LCOM_D LcomD = rData;
		if (nType == 0) SetCvlJTJD60_Capacity_TPG(LcomD, arTLoad, arTpgLoad, arFrLoad, dTFactor, dTepFcator, FALSE, bAcdn, nMsgType);
		else         SetCvlJTJD60_Capacity_TPG(LcomD, arTLoad, arTpgLoad, arFrLoad, dTFactor, dTepFcator, TRUE, bAcdn, nMsgType);
	}

	if (m_nFrCount > 0 && bFr)
	{
		T_LCOM_D LcomD = rData;
		LoadCaseNo = GetCountLcomCase(LcomD);
		int nFR = 0;
		for (nc = 0; nc < m_nFrCount; nc++) nFR += Set_STLCIncludeCS(arFrLoad.GetAt(nc), dF, LoadCaseNo, LcomD, 0);
		if (nFR > 0)
		{
			strDesc.Format(_T("+%sFR"), GetFactorString(dF)); LcomD.Description = LcomD.Description + strDesc;
			if (nType == 0)
			{
				if (bAcdn) m_aAcdnLcomD.Add(LcomD);
				else      Set_LoadComb4Civil(LcomD, D_STR, 0, nMsgType);
			}
		}
	}

	return TRUE;
}
BOOL CLoadCombCtrl::SetCvlESCGB19_ETC(T_LCOM_D rData, T_LCOM_D Lcom_cf, BOOL bCF, ArINT& arCrlLoad, ArINT& arMCrlLoad, BOOL bCrl, ArINT& arBrkLoad, BOOL bBrk,
	ArINT& arWLoad, BOOL bW, ArINT& arSfLoad, BOOL bSf, ArINT& arIpLoad, BOOL bIp, ArINT& arTLoad, ArINT& arTpgLoad,
	BOOL bT, ArINT& arFrLoad, BOOL bFr, int nType, double dFactor, BOOL bAcdn, int nMsgType)
{
	int LoadCaseNo = 0, nc = 0, nnn = 0;
	CString strDesc = _T("");
	bool bChange = m_bJTG15_Factor ? (dFactor / m_dJTG15_Factor < 1.0) : (dFactor < 1.0);
	double dF = bChange ? 1.4*dFactor : 1.0;
	double dTepFcator = nMsgType == 115 ? 0.8 : dF;//nMsgType == 115 ´ú±íÅ¼È»×éºÏ
	double dTFactor = nMsgType == 115 ? 1.0 : dTepFcator;
	//double dTepF = nType == 0 ? 1.4*0.75 : 1.4;
	double dWF = bChange ? 1.1*dFactor : 1.0;

	BOOL bAddCsLoad = m_nConstLoad != 0 && CheckCS_SgldK(D_SGLD_ER1_CVL, nnn); // CS or ST+CS

	if ((m_nMCrlCount || m_nCrlCount > 0) && bCrl)
	{
		T_LCOM_D LcomD = rData;
		LoadCaseNo = GetCountLcomCase(LcomD);
		int nCRL = 0;
		for (nc = 0; nc < m_nCrlCount; nc++) nCRL += Set_STLCIncludeCS(arCrlLoad.GetAt(nc), dF, LoadCaseNo, LcomD, 0);
		T_LCOM_D LcomD_tmp = Lcom_cf;
		if (bCF && bBrk)
		{
			LoadCaseNo = GetCountLcomCase(LcomD_tmp);
			for (nc = 0; nc < m_nCrlCount; nc++) Set_STLCIncludeCS(arCrlLoad.GetAt(nc), dF, LoadCaseNo, LcomD_tmp, 0);
		}
		if (nCRL > 0)
		{
			strDesc.Format(_T("+%sCRL"), GetFactorString(dF)); LcomD.Description = LcomD.Description + strDesc;
			if (bCF && bBrk) LcomD_tmp.Description = LcomD_tmp.Description + strDesc;
		}
		int nMCrlCount = (m_nMCrlCount > 0) ? m_nMCrlCount : ((nCRL > 0) ? 1 : 0);
		for (int crl = 0; crl < nMCrlCount; ++crl)
		{
			T_LCOM_D LcomD_crl = LcomD;
			T_LCOM_D LcomD_crltmp = LcomD_tmp;
			if (m_nMCrlCount > 0)
			{
				Set_MovingCase(arMCrlLoad.GetAt(crl), dF, LoadCaseNo, LcomD_crl, TRUE);
				strDesc.Format(_T("+%sMCRL"), GetFactorString(dF)); LcomD_crl.Description = LcomD_crl.Description + strDesc; LcomD_crltmp.Description = LcomD_crltmp.Description + strDesc;
				if (m_nMCrlCount > 1) { strDesc.Format(_T("[%d]"), crl + 1); LcomD_crl.Description = LcomD_crl.Description + strDesc; LcomD_crltmp.Description = LcomD_crltmp.Description + strDesc; }
			}
			if (nCRL > 0 || m_nMCrlCount > 0)
			{
				if (nType == 1) SetCvlESCGB19_ETC(LcomD_crl, LcomD_crltmp, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, bBrk, arWLoad, TRUE, arSfLoad, TRUE, arIpLoad, TRUE,
					arTLoad, arTpgLoad, TRUE, arFrLoad, TRUE, 0, dFactor, bAcdn, nMsgType);
				if (nType == 6) SetCvlESCGB19_ETC(LcomD_crl, LcomD_crltmp, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, bBrk, arWLoad, TRUE, arSfLoad, TRUE, arIpLoad, TRUE,
					arTLoad, arTpgLoad, TRUE, arFrLoad, FALSE, 2, dFactor, bAcdn, nMsgType);
				if (nType == 8) SetCvlESCGB19_ETC(LcomD_crl, LcomD_crltmp, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, bBrk, arWLoad, TRUE, arSfLoad, TRUE, arIpLoad, TRUE,
					arTLoad, arTpgLoad, FALSE, arFrLoad, FALSE, 6, dFactor, bAcdn, nMsgType);
				if (nType == 10) SetCvlESCGB19_ETC(LcomD_crl, LcomD_crltmp, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWLoad, TRUE, arSfLoad, FALSE, arIpLoad, FALSE,
					arTLoad, arTpgLoad, FALSE, arFrLoad, FALSE, 8, dFactor, bAcdn, nMsgType);
				if (nType == 101) SetCvlESCGB19_ETC(LcomD_crl, LcomD_crltmp, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, bBrk, arWLoad, FALSE, arSfLoad, FALSE, arIpLoad, FALSE,
					arTLoad, arTpgLoad, FALSE, arFrLoad, FALSE, 102, dFactor, bAcdn, nMsgType);
				if (nType == 103) SetCvlESCGB19_ETC(LcomD_crl, LcomD_crltmp, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, bBrk, arWLoad, TRUE, arSfLoad, FALSE, arIpLoad, FALSE,
					arTLoad, arTpgLoad, FALSE, arFrLoad, FALSE, 104, dFactor, bAcdn, nMsgType);
				if (nType == 0)
				{
					if (bAcdn) m_aAcdnLcomD.Add(LcomD_crl);
					else      Set_LoadComb4Civil(LcomD_crl, D_STR, 0, nMsgType);
				}
			}
		}
	}

	if (nType == 8) bBrk = FALSE;
	if (m_nBrkCount > 0 && bBrk)
	{
		for (int i = 0; i < 2; ++i)
		{
			if (i == 1 && !bCF) break;
			T_LCOM_D LcomD = (i == 0) ? rData : Lcom_cf;
			LoadCaseNo = GetCountLcomCase(LcomD);
			double dBrkF = (i == 0) ? (bCF ? dF * 0.7 : dF) : dF;
			int nBRK = 0;
			for (nc = 0; nc < m_nBrkCount; nc++) nBRK += Set_STLCIncludeCS(arBrkLoad.GetAt(nc), dBrkF, LoadCaseNo, LcomD, 0);
			if (nBRK > 0) { strDesc.Format(_T("+%sBRK"), GetFactorString(dBrkF)); LcomD.Description = LcomD.Description + strDesc; }
			if (nType == 1) SetCvlESCGB19_ETC(LcomD, Lcom_cf, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWLoad, TRUE, arSfLoad, FALSE, arIpLoad, FALSE,
				arTLoad, arTpgLoad, TRUE, arFrLoad, FALSE, 0, dFactor, bAcdn, nMsgType);
			if (nType == 2) SetCvlESCGB19_ETC(LcomD, Lcom_cf, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWLoad, TRUE, arSfLoad, FALSE, arIpLoad, FALSE,
				arTLoad, arTpgLoad, TRUE, arFrLoad, FALSE, 0, dFactor, bAcdn, nMsgType);
			if (nType == 6) SetCvlESCGB19_ETC(LcomD, Lcom_cf, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWLoad, TRUE, arSfLoad, FALSE, arIpLoad, FALSE,
				arTLoad, arTpgLoad, FALSE, arFrLoad, FALSE, 3, dFactor, bAcdn, nMsgType);
			if (nType == 7) SetCvlESCGB19_ETC(LcomD, Lcom_cf, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWLoad, TRUE, arSfLoad, FALSE, arIpLoad, FALSE,
				arTLoad, arTpgLoad, FALSE, arFrLoad, FALSE, 3, dFactor, bAcdn, nMsgType);
			if (nType == 8) SetCvlESCGB19_ETC(LcomD, Lcom_cf, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWLoad, TRUE, arSfLoad, FALSE, arIpLoad, FALSE,
				arTLoad, arTpgLoad, FALSE, arFrLoad, FALSE, 3, dFactor, bAcdn, nMsgType);
			if (nType == 101) SetCvlESCGB19_ETC(LcomD, Lcom_cf, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWLoad, TRUE, arSfLoad, FALSE, arIpLoad, FALSE,
				arTLoad, arTpgLoad, FALSE, arFrLoad, FALSE, 3, dFactor, bAcdn, nMsgType);
			if (nType == 102) SetCvlESCGB19_ETC(LcomD, Lcom_cf, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWLoad, TRUE, arSfLoad, FALSE, arIpLoad, FALSE,
				arTLoad, arTpgLoad, TRUE, arFrLoad, FALSE, 0, dFactor, bAcdn, nMsgType);
			if (nType == 103) SetCvlESCGB19_ETC(LcomD, Lcom_cf, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWLoad, TRUE, arSfLoad, FALSE, arIpLoad, FALSE,
				arTLoad, arTpgLoad, FALSE, arFrLoad, FALSE, 3, dFactor, bAcdn, nMsgType);
			if (nType == 104) SetCvlESCGB19_ETC(LcomD, Lcom_cf, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWLoad, TRUE, arSfLoad, FALSE, arIpLoad, FALSE,
				arTLoad, arTpgLoad, FALSE, arFrLoad, FALSE, 3, dFactor, bAcdn, nMsgType);
			if (nType == 0)
			{
				if (bAcdn) m_aAcdnLcomD.Add(LcomD);
				else      Set_LoadComb4Civil(LcomD, D_STR, 0, nMsgType);
			}
		}
	}
	if (bW)
	{
		int nWCount = m_nWCount * 2; // Wind
		if (m_nWCount == 0 && bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(W))) nWCount = 1;
		for (nc = 0; nc < nWCount; nc++)
		{
			T_LCOM_D LcomD = rData;
			LoadCaseNo = GetCountLcomCase(LcomD);
			int nWL = 0;
			if (m_nWCount > 0)
			{
				double dWF1 = (nc % 2 == 0) ? dWF : -1.*dWF;
				if (Set_STLCIncludeCS(arWLoad.GetAt(nc / 2), dWF1, LoadCaseNo, LcomD, 0) > 0)
				{
					CString strW = (dWF1 > 0.0) ? _T("+%sW") : _T("%sW");
					strDesc.Format(strW, GetFactorString(dWF1)); LcomD.Description = LcomD.Description + strDesc;
					if (m_nWCount > 1) { strDesc.Format(_T("[%d]"), nc / 2 + 1); LcomD.Description = LcomD.Description + strDesc; }
				}
			}
			if (bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(W)))
			{
				Set_CStageCase(m_aCSLoad.GetAt(nnn), dWF, LoadCaseNo, LcomD, TRUE);
				strDesc.Format(_T("+%s(cEL)"), GetFactorString(dWF)); LcomD.Description = LcomD.Description + strDesc;
			}
			if (nType == 1) SetCvlESCGB19_ETC(LcomD, Lcom_cf, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWLoad, FALSE, arSfLoad, TRUE, arIpLoad, TRUE,
				arTLoad, arTpgLoad, TRUE, arFrLoad, TRUE, 0, dFactor, bAcdn, nMsgType);
			if (nType == 2) SetCvlESCGB19_ETC(LcomD, Lcom_cf, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWLoad, FALSE, arSfLoad, TRUE, arIpLoad, TRUE,
				arTLoad, arTpgLoad, TRUE, arFrLoad, TRUE, 0, dFactor, bAcdn, nMsgType);
			if (nType == 3) SetCvlESCGB19_ETC(LcomD, Lcom_cf, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWLoad, FALSE, arSfLoad, FALSE, arIpLoad, FALSE,
				arTLoad, arTpgLoad, TRUE, arFrLoad, FALSE, 0, dFactor, bAcdn, nMsgType);
			if (nType == 6) SetCvlESCGB19_ETC(LcomD, Lcom_cf, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWLoad, FALSE, arSfLoad, TRUE, arIpLoad, TRUE,
				arTLoad, arTpgLoad, TRUE, arFrLoad, FALSE, 2, dFactor, bAcdn, nMsgType);
			if (nType == 7) SetCvlESCGB19_ETC(LcomD, Lcom_cf, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWLoad, FALSE, arSfLoad, TRUE, arIpLoad, TRUE,
				arTLoad, arTpgLoad, TRUE, arFrLoad, FALSE, 2, dFactor, bAcdn, nMsgType);
			if (nType == 8) SetCvlESCGB19_ETC(LcomD, Lcom_cf, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWLoad, FALSE, arSfLoad, TRUE, arIpLoad, TRUE,
				arTLoad, arTpgLoad, FALSE, arFrLoad, FALSE, 6, dFactor, bAcdn, nMsgType);
			if (nType == 9) SetCvlESCGB19_ETC(LcomD, Lcom_cf, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWLoad, FALSE, arSfLoad, TRUE, arIpLoad, TRUE,
				arTLoad, arTpgLoad, FALSE, arFrLoad, FALSE, 6, dFactor, bAcdn, nMsgType);
			if (nType == 0)
			{
				if (bAcdn) m_aAcdnLcomD.Add(LcomD);
				else      Set_LoadComb4Civil(LcomD, D_STR, 0, nMsgType);
			}
		}
	}

	if (m_nSfCount > 0 && bSf)
	{
		T_LCOM_D LcomD = rData;
		LoadCaseNo = GetCountLcomCase(LcomD);
		int nSF = 0;
		for (nc = 0; nc < m_nSfCount; nc++) nSF += Set_STLCIncludeCS(arSfLoad.GetAt(nc), dF, LoadCaseNo, LcomD, 0);
		if (nSF > 0) { strDesc.Format(_T("+%sSF"), GetFactorString(dF)); LcomD.Description = LcomD.Description + strDesc; }
		if (nType == 1) SetCvlESCGB19_ETC(LcomD, Lcom_cf, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWLoad, FALSE, arSfLoad, FALSE, arIpLoad, FALSE,
			arTLoad, arTpgLoad, TRUE, arFrLoad, TRUE, 0, dFactor, bAcdn, nMsgType);
		if (nType == 2) SetCvlESCGB19_ETC(LcomD, Lcom_cf, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWLoad, FALSE, arSfLoad, FALSE, arIpLoad, FALSE,
			arTLoad, arTpgLoad, TRUE, arFrLoad, TRUE, 0, dFactor, bAcdn, nMsgType);
		if (nType == 4) SetCvlESCGB19_ETC(LcomD, Lcom_cf, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWLoad, FALSE, arSfLoad, FALSE, arIpLoad, FALSE,
			arTLoad, arTpgLoad, TRUE, arFrLoad, TRUE, 0, dFactor, bAcdn, nMsgType);
		if (nType == 6) SetCvlESCGB19_ETC(LcomD, Lcom_cf, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWLoad, FALSE, arSfLoad, FALSE, arIpLoad, FALSE,
			arTLoad, arTpgLoad, TRUE, arFrLoad, FALSE, 2, dFactor, bAcdn, nMsgType);
		if (nType == 7) SetCvlESCGB19_ETC(LcomD, Lcom_cf, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWLoad, FALSE, arSfLoad, FALSE, arIpLoad, FALSE,
			arTLoad, arTpgLoad, TRUE, arFrLoad, FALSE, 2, dFactor, bAcdn, nMsgType);
		if (nType == 0)
		{
			if (bAcdn) m_aAcdnLcomD.Add(LcomD);
			else      Set_LoadComb4Civil(LcomD, D_STR, 0, nMsgType);
		}
	}

	if (m_nIpCount > 0 && bIp)
	{
		T_LCOM_D LcomD = rData;
		LoadCaseNo = GetCountLcomCase(LcomD);
		int nIP = 0;
		for (nc = 0; nc < m_nIpCount; nc++) nIP += Set_STLCIncludeCS(arIpLoad.GetAt(nc), dF, LoadCaseNo, LcomD, 0);
		if (nIP > 0) { strDesc.Format(_T("+%sIP"), GetFactorString(dF)); LcomD.Description = LcomD.Description + strDesc; }
		if (nType == 1) SetCvlESCGB19_ETC(LcomD, Lcom_cf, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWLoad, FALSE, arSfLoad, FALSE, arIpLoad, FALSE,
			arTLoad, arTpgLoad, TRUE, arFrLoad, TRUE, 0, dFactor, bAcdn, nMsgType);
		if (nType == 2) SetCvlESCGB19_ETC(LcomD, Lcom_cf, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWLoad, FALSE, arSfLoad, FALSE, arIpLoad, FALSE,
			arTLoad, arTpgLoad, TRUE, arFrLoad, TRUE, 0, dFactor, bAcdn, nMsgType);
		if (nType == 4) SetCvlESCGB19_ETC(LcomD, Lcom_cf, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWLoad, FALSE, arSfLoad, FALSE, arIpLoad, FALSE,
			arTLoad, arTpgLoad, TRUE, arFrLoad, TRUE, 0, dFactor, bAcdn, nMsgType);
		if (nType == 6) SetCvlESCGB19_ETC(LcomD, Lcom_cf, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWLoad, FALSE, arSfLoad, FALSE, arIpLoad, FALSE,
			arTLoad, arTpgLoad, TRUE, arFrLoad, FALSE, 2, dFactor, bAcdn, nMsgType);
		if (nType == 7) SetCvlESCGB19_ETC(LcomD, Lcom_cf, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWLoad, FALSE, arSfLoad, FALSE, arIpLoad, FALSE,
			arTLoad, arTpgLoad, TRUE, arFrLoad, FALSE, 2, dFactor, bAcdn, nMsgType);
		if (nType == 0)
		{
			if (bAcdn) m_aAcdnLcomD.Add(LcomD);
			else      Set_LoadComb4Civil(LcomD, D_STR, 0, nMsgType);
		}
	}

	if (bT && ((m_nTCount > 0 || m_nTpgCount > 0) || (bAddCsLoad && (CheckCSEL(D_SGLD_ER1_CVL, _LSX(T)) || CheckCSEL(D_SGLD_ER1_CVL, _LSX(TPG))))))
	{
		T_LCOM_D LcomD = rData;
		if (nType == 0) SetCvlJTJD60_Capacity_TPG(LcomD, arTLoad, arTpgLoad, arFrLoad, dTFactor, dTepFcator, FALSE, bAcdn, nMsgType);
		else         SetCvlJTJD60_Capacity_TPG(LcomD, arTLoad, arTpgLoad, arFrLoad, dTFactor, dTepFcator, TRUE, bAcdn, nMsgType);
	}

	if (m_nFrCount > 0 && bFr)
	{
		T_LCOM_D LcomD = rData;
		LoadCaseNo = GetCountLcomCase(LcomD);
		int nFR = 0;
		for (nc = 0; nc < m_nFrCount; nc++) nFR += Set_STLCIncludeCS(arFrLoad.GetAt(nc), dF, LoadCaseNo, LcomD, 0);
		if (nFR > 0)
		{
			strDesc.Format(_T("+%sFR"), GetFactorString(dF)); LcomD.Description = LcomD.Description + strDesc;
			if (nType == 0)
			{
				if (bAcdn) m_aAcdnLcomD.Add(LcomD);
				else      Set_LoadComb4Civil(LcomD, D_STR, 0, nMsgType);
			}
		}
	}
	return TRUE;
}

BOOL CLoadCombCtrl::SetCvlESCGB19_Var1_S(const CArray<T_LCOM_D, T_LCOM_D&>& arData, ArUNT& arMLoad, double dIL, ArINT& arLLoad, ArINT& arIlLoad, ArINT& arCrlLoad, ArINT& arMCrlLoad, ArINT& arCfLoad, ArINT& arLsLoad, ArINT& arWLoad, ArINT& arWlLoad, ArINT& arSfLoad, ArINT& arIpLoad, ArINT& arTLoad, ArINT& arTpgLoad, ArINT& arFrLoad, ArINT& arBrkLoad, int nType, int nMsgType)
{
	// nType == 10:×¼ÓÀ¾Ã, 11:ÆµÓö, 12:±ê×¼
	CArray<T_LCOM_D, T_LCOM_D&> aLcomD; aLcomD.Copy(arData);
	int  nLcomSize = aLcomD.GetSize();
	int  nnn = 0;

	BOOL bErec = FALSE;
	for (int nErec = 0; nErec < D_STCT_EREC_MAX_CVL; ++nErec)
	{
		if (m_nConstLoad != 0 && CheckCS_SgldK(D_SGLD_ER1_CVL + nErec, nnn)) bErec = TRUE;
	}

	BOOL bAddCsLoad = bErec;
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER1_CVL,nnn)) || 
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER2_CVL,nnn)) || 
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER3_CVL,nnn)) ||
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER4_CVL,nnn)) ||
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER5_CVL,nnn)) ||
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER6_CVL,nnn)) ||
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER7_CVL,nnn)) ||
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER8_CVL,nnn)) ||
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER9_CVL,nnn)) ||
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER10_CVL,nnn));// CS or ST+CS;

	double dFactor = 1.0, dLFactor = 1.0, dWFactor = 1.0, dWlFactor = 1.0, dCRLFactor = 1.0, dTPGFactor = 1.0;
	if (nType == 10) { dLFactor = 0.4, dWFactor = 0.75, dWlFactor = 1.0, dTPGFactor = 0.8, dCRLFactor = 0.4; }
	else if (nType == 11) { dLFactor = 0.7, dWFactor = 0.75, dWlFactor = 1.0, dTPGFactor = 0.8, dCRLFactor = 1.0; }//modified by gongxing at 2016-01-22

	BOOL bUseWlLoad = TRUE;

	int nServEnvType = (nType == 12) ? 1 : 0;
	// ÓÀ¾Ã×÷ÓÃ+ÒÆ¶¯ºÉÔØ + 1¿É±ä×÷ÓÃ
	CString strDesc = _T("");
	int LoadCaseNo = 0, nc = 0;
	for (int lc = 0; lc < nLcomSize; lc++)
	{
		int nMCount = max(m_nMCount, 1);
		for (int m = 0; m < nMCount; m++)
		{
			T_LCOM_D LcomD = aLcomD.GetAt(lc); LcomD.nActive = 1; // Active or Strength/Stress
			if (m_nLcomType == D_LCOMTYPE_CONCRETE)
			{
				if (nType == 10)   LcomD.nActive = 8;  // ×¼ÓÀ¾Ã×éºÏ
				if (nType == 11)   LcomD.nActive = 7;  // ÆµÓö×éºÏ
				if (nType == 12)
				{
					LcomD.bElasticStage = TRUE;
					LcomD.nActive = 2;
				}
			}
			else if (m_nLcomType == D_LCOMTYPE_STLCOMP || m_nLcomType == D_LCOMTYPE_SRC)
			{
				if (nType == 10) LcomD.nActive = 8;  //×¼ÓÀ¾Ã×éºÏ
				if (nType == 11) LcomD.nActive = 7;  //ÆµÓö×éºÏ
			}

			T_LCOM_D LcomD_cf = LcomD; // MNET:1614, KJH20050809, cf°¡ Á¦¿ÜµÈ load combination
			LoadCaseNo = GetCountLcomCase(LcomD);

			BOOL bLive = FALSE, bCF = FALSE;
			BOOL bAddLcomD = FALSE;
			//BOOL bFindCJJLeadingMLoad = FALSE;
			// moving load ÒÆ¶¯ºÉÔØ
			if (m_nMCount > 0)
			{
				Set_MovingCase(arMLoad.GetAt(m), dLFactor / (1. + dIL), LoadCaseNo, LcomD, TRUE);
				if (nType == 12 || dIL < m_dZeroLim) strDesc.Format(_T("+%sM"), GetFactorString(dLFactor));
				else                            strDesc.Format(_T("+%s/(1+mu)M"), GetFactorString(dLFactor));
				LcomD.Description += strDesc;
				if (m_nMCount > 1) { strDesc.Format(_T("[%d]"), m + 1); LcomD.Description += strDesc; }
				bAddLcomD = TRUE;
			}
			int nLL = 0, nIL = 0, nCF = 0;
			if (m_nLCount > 0 || (bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(L)))) // ST or ST+CS
			{
				for (nc = 0; nc < m_nLCount; nc++) nLL += Set_STLCIncludeCS(arLLoad.GetAt(nc), dLFactor, LoadCaseNo, LcomD, 0);
				if (nLL > 0) { strDesc.Format(_T("+%sL"), GetFactorString(dLFactor)); LcomD.Description += strDesc; }
				if (bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(L)))
				{
					Set_CStageCase(m_aCSLoad.GetAt(nnn), dLFactor, LoadCaseNo, LcomD, TRUE);
					strDesc.Format(_T("+%s(cEL)"), GetFactorString(dLFactor)); LcomD.Description += strDesc;
					bAddLcomD = TRUE;
				}
				for (nc = 0; nc < m_nIlCount; nc++) nIL += Set_STLCIncludeCS(arIlLoad.GetAt(nc), dLFactor, LoadCaseNo, LcomD, 0);
				if (nIL > 0) { strDesc.Format(_T("+%sIL"), GetFactorString(dLFactor)); LcomD.Description += strDesc; }
				if (nLL + nIL > 0) bAddLcomD = TRUE;
			}
			bLive = m_nMCount > 0 || (m_nLCount > 0 || (bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(L))));
			if (bLive)
			{
				LcomD_cf = LcomD;
				for (nc = 0; nc < m_nCfCount; nc++) nCF += Set_STLCIncludeCS(arCfLoad.GetAt(nc), dLFactor, LoadCaseNo, LcomD, 0);
				if (nCF > 0) { strDesc.Format(_T("+%sCF"), GetFactorString(dLFactor)); LcomD.Description += strDesc; bCF = TRUE; }
				bAddLcomD = TRUE;
			}
			if (bAddLcomD) Set_LoadComb4Civil(LcomD, D_SER, nServEnvType, nMsgType);

			if (m_nLsCount > 0) // Live Load Surcharge µØÃæ¸½¼Ó»îºÉÔØ
			{
				T_LCOM_D LcomD_l = LcomD; LoadCaseNo = GetCountLcomCase(LcomD_l);
				int nLS = 0;
				for (nc = 0; nc < m_nLsCount; nc++) nLS += Set_STLCIncludeCS(arLsLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD_l, 0);
				if (nLS > 0)
				{
					strDesc.Format(_T("+%sLS"), GetFactorString(dFactor)); LcomD_l.Description += strDesc;
					Set_LoadComb4Civil(LcomD_l, D_SER, nServEnvType, nMsgType);
				}
			}
			// MNET:XXXX-BJLEE(BBONG)-20060502
			if (m_nCrlCount > 0 || m_nMCrlCount > 0) // Crowd Load ÈËÈººÉÔØ
			{
				T_LCOM_D LcomD_l = LcomD; LoadCaseNo = GetCountLcomCase(LcomD_l);
				int nCRL = 0;
				for (nc = 0; nc < m_nCrlCount; nc++) nCRL += Set_STLCIncludeCS(arCrlLoad.GetAt(nc), dCRLFactor, LoadCaseNo, LcomD_l, 0);
				if (nCRL > 0) { strDesc.Format(_T("+%sCRL"), GetFactorString(dCRLFactor)); LcomD_l.Description += strDesc; }

				int nMCrlCount = (m_nMCrlCount > 0) ? m_nMCrlCount : ((nCRL > 0) ? 1 : 0);
				for (int crl = 0; crl < nMCrlCount; ++crl)
				{
					T_LCOM_D LcomD_crl = LcomD_l; LoadCaseNo = GetCountLcomCase(LcomD_crl);
					if (m_nMCrlCount > 0)
					{
						Set_MovingCase(arMCrlLoad.GetAt(crl), dCRLFactor, LoadCaseNo, LcomD_crl, TRUE);
						strDesc.Format(_T("+%sMCRL"), GetFactorString(dCRLFactor)); LcomD_crl.Description += strDesc;
						if (m_nMCrlCount > 1) { strDesc.Format(_T("[%d]"), crl + 1); LcomD_crl.Description += strDesc; }
					}
					if (nCRL > 0 || m_nMCrlCount > 0)
					{
						if (nType == 2 || nType == 4) m_aAcdnLcomD.Add(LcomD_crl);
						else                     Set_LoadComb4Civil(LcomD_crl, D_SER, nServEnvType, nMsgType);
					}
				}
			}

			if (m_nBrkCount > 0 && bLive) // Brake Load ÖÆ¶¯Á¦
			{
				T_LCOM_D LcomD_l = LcomD; LoadCaseNo = GetCountLcomCase(LcomD_l);
				double dBrkF = bCF ? dFactor * 0.7 : dFactor;
				int nBRK = 0;
				for (nc = 0; nc < m_nBrkCount; nc++) nBRK += Set_STLCIncludeCS(arBrkLoad.GetAt(nc), dBrkF, LoadCaseNo, LcomD_l, 0);
				if (nBRK > 0)
				{
					strDesc.Format(_T("+%sBRK"), GetFactorString(dBrkF)); LcomD_l.Description += strDesc;
					Set_LoadComb4Civil(LcomD_l, D_SER, nServEnvType, nMsgType);
				}
				if (bCF)
				{
					LcomD_l = LcomD_cf; LoadCaseNo = GetCountLcomCase(LcomD_l);
					dBrkF = dFactor;
					for (nc = 0; nc < m_nBrkCount; nc++) nBRK += Set_STLCIncludeCS(arBrkLoad.GetAt(nc), dBrkF, LoadCaseNo, LcomD_l, 0);
					if (nBRK > 0)
					{
						strDesc.Format(_T("+%sBRK"), GetFactorString(dBrkF)); LcomD_l.Description += strDesc;
						Set_LoadComb4Civil(LcomD_l, D_SER, nServEnvType, nMsgType);
					}
				}
			}

			if (bUseWlLoad)
			{
				int nWCount = m_nWlCount * 2; // MNET:1614, KJH20050808 ·çºÉÔØ
				if (m_nWlCount == 0 && bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(WL))) nWCount = 1;
				for (nc = 0; nc < nWCount; nc++)
				{
					T_LCOM_D LcomD_l = LcomD; LoadCaseNo = GetCountLcomCase(LcomD_l);
					if (m_nWlCount > 0)
					{
						double dWF = (nc % 2 == 0) ? dWlFactor : -1.*dWlFactor;
						if (Set_STLCIncludeCS(arWlLoad.GetAt(nc / 2), dWF, LoadCaseNo, LcomD_l, 0) > 0)
						{
							CString strW = (dWF > 0.0) ? _T("+%sWL") : _T("%sWL");
							strDesc.Format(strW, GetFactorString(dWF)); LcomD_l.Description += strDesc;
							if (m_nWlCount > 1) { strDesc.Format(_T("[%d]"), nc / 2 + 1); LcomD_l.Description += strDesc; }
						}
					}
					if (bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(WL)))
					{
						Set_CStageCase(m_aCSLoad.GetAt(nnn), dWlFactor, LoadCaseNo, LcomD_l, TRUE);
						strDesc.Format(_T("+%s(cEL)"), GetFactorString(dWlFactor)); LcomD_l.Description += strDesc;
					}
					Set_LoadComb4Civil(LcomD_l, D_SER, nServEnvType, nMsgType);
				}
			}
			else
			{
				int nWCount = m_nWCount * 2; // MNET:1614, KJH20050808 ·çºÉÔØ
				if (m_nWCount == 0 && bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(W))) nWCount = 1;
				for (nc = 0; nc < nWCount; nc++)
				{
					T_LCOM_D LcomD_l = LcomD; LoadCaseNo = GetCountLcomCase(LcomD_l);
					if (m_nWCount > 0)
					{
						double dWF = (nc % 2 == 0) ? dWFactor : -1.*dWFactor;
						if (Set_STLCIncludeCS(arWLoad.GetAt(nc / 2), dWF, LoadCaseNo, LcomD_l, 0) > 0)
						{
							CString strW = (dWF > 0.0) ? _T("+%sW") : _T("%sW");
							strDesc.Format(strW, GetFactorString(dWF)); LcomD_l.Description += strDesc;
							if (m_nWCount > 1) { strDesc.Format(_T("[%d]"), nc / 2 + 1); LcomD_l.Description += strDesc; }
						}
					}
					if (bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(W)))
					{
						Set_CStageCase(m_aCSLoad.GetAt(nnn), dWFactor, LoadCaseNo, LcomD_l, TRUE);
						strDesc.Format(_T("+%s(cEL)"), GetFactorString(dWFactor)); LcomD_l.Description += strDesc;
					}
					Set_LoadComb4Civil(LcomD_l, D_SER, nServEnvType, nMsgType);
				}
			}
			if (m_nSfCount > 0) // Stream Flow Pressure Á÷Ë®Ñ¹Á¦
			{
				T_LCOM_D LcomD_l = LcomD; LoadCaseNo = GetCountLcomCase(LcomD_l);
				int nSF = 0;
				for (nc = 0; nc < m_nSfCount; nc++) nSF += Set_STLCIncludeCS(arSfLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD_l, 0);
				if (nSF > 0)
				{
					strDesc.Format(_T("+%sSF"), GetFactorString(dFactor)); LcomD_l.Description += strDesc;
					Set_LoadComb4Civil(LcomD_l, D_SER, nServEnvType, nMsgType);
				}
			}
			if (m_nIpCount > 0) // Ice Pressure ±ùÑ¹Á¦
			{
				T_LCOM_D LcomD_l = LcomD; LoadCaseNo = GetCountLcomCase(LcomD_l);
				int nIP = 0;
				for (nc = 0; nc < m_nIpCount; nc++) nIP += Set_STLCIncludeCS(arIpLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD_l, 0);
				if (nIP > 0)
				{
					strDesc.Format(_T("+%sIP"), GetFactorString(dFactor)); LcomD_l.Description += strDesc;
					Set_LoadComb4Civil(LcomD_l, D_SER, nServEnvType, nMsgType);
				}
			}
			// Temeperature ÎÂ¶ÈºÉÔØ
			if ((m_nTCount > 0 || m_nTpgCount > 0) || (bAddCsLoad && (CheckCSEL(D_SGLD_ER1_CVL, _LSX(T)) || CheckCSEL(D_SGLD_ER1_CVL, _LSX(TPG)))))
			{
				T_LCOM_D LcomD_l = LcomD;
				SetCvlJTJD60_Capacity_TPG(LcomD_l, arTLoad, arTpgLoad, arFrLoad, dFactor, dTPGFactor, FALSE, FALSE, nMsgType, D_SER, nServEnvType);
			}
			if (m_nFrCount > 0) // Friction Ä¦²ÁÁ¦
			{
				T_LCOM_D LcomD_l = LcomD; LoadCaseNo = GetCountLcomCase(LcomD_l);
				int nFR = 0;
				for (nc = 0; nc < m_nFrCount; nc++) nFR += Set_STLCIncludeCS(arFrLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD_l, 0);
				if (nFR > 0)
				{
					strDesc.Format(_T("+%sFR"), GetFactorString(dFactor)); LcomD_l.Description += strDesc;
					Set_LoadComb4Civil(LcomD_l, D_SER, nServEnvType, nMsgType);
				}
			}
		} // next m
	} // next lc

	return TRUE;
}

BOOL CLoadCombCtrl::SetCvlESCGB19_Var2_S(const CArray<T_LCOM_D, T_LCOM_D&>& arData, ArUNT& arMLoad, double dIL, ArINT& arLLoad, ArINT& arIlLoad, ArINT& arCrlLoad, ArINT& arMCrlLoad, ArINT& arCfLoad, ArINT& arLsLoad, ArINT& arWLoad, ArINT& arWlLoad, ArINT& arSfLoad, ArINT& arIpLoad, ArINT& arTLoad, ArINT& arTpgLoad, ArINT& arFrLoad, ArINT& arBrkLoad, int nType, int nMsgType)
{
	// nType == 10:×¼ÓÀ¾Ã×éºÏ, 11:ÆµÓö×éºÏ, 12:±ê×¼×éºÏ
	CArray<T_LCOM_D, T_LCOM_D&> aLcomD; aLcomD.Copy(arData);
	int  nLcomSize = aLcomD.GetSize();
	int  nnn = 0;

	BOOL bErec = FALSE;
	for (int nErec = 0; nErec < D_STCT_EREC_MAX_CVL; ++nErec)
	{
		if (m_nConstLoad != 0 && CheckCS_SgldK(D_SGLD_ER1_CVL + nErec, nnn)) bErec = TRUE;
	}

	BOOL bAddCsLoad = bErec;
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER1_CVL,nnn)) || 
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER2_CVL,nnn)) || 
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER3_CVL,nnn)) ||
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER4_CVL,nnn)) ||
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER5_CVL,nnn)) ||
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER6_CVL,nnn)) ||
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER7_CVL,nnn)) ||
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER8_CVL,nnn)) ||
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER9_CVL,nnn)) ||
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER10_CVL,nnn));// CS or ST+CS;

	double dFactor = 1.0, dLFactor = 1.0, dWFactor = 1.0, dWlFactor = 1.0, dCRLFactor = 1.0, dTPGFactor = 1.0;
	if (nType == 10) { dLFactor = 0.4, dWFactor = 0.75, dWlFactor = 1.0, dTPGFactor = 0.8, dCRLFactor = 0.4; }
	else if (nType == 11) { dLFactor = 0.7, dWFactor = 0.75, dWlFactor = 1.0, dTPGFactor = 0.8, dCRLFactor = 1.0; }//modified by gongxing at 2016-01-22

	BOOL bUseWlLoad = TRUE;

	ArINT arTmp;

	int nServEnvType = (nType == 12) ? 1 : 0;
	// ÓÀ¾Ã×÷ÓÃ + ÒÆ¶¯ºÉÔØ + 2¿É±ä×÷ÓÃ
	CString strDesc = _T("");
	int LoadCaseNo = 0, nc = 0;
	for (int lc = 0; lc < nLcomSize; lc++)
	{
		int nMCount = max(m_nMCount, 1);
		for (int m = 0; m < nMCount; m++)
		{
			T_LCOM_D LcomD = aLcomD.GetAt(lc); LcomD.nActive = 1; // Active or Strength/Stress
			if (m_nLcomType == D_LCOMTYPE_CONCRETE)
			{
				if (nType == 10)   LcomD.nActive = 8;  // ×¼ÓÀ¾Ã×éºÏ
				if (nType == 11)   LcomD.nActive = 7;  // ÆµÓö×éºÏ
				if (nType == 12)
				{
					LcomD.bElasticStage = TRUE;
					LcomD.nActive = 2;
				}
			}
			else if (m_nLcomType == D_LCOMTYPE_STLCOMP || m_nLcomType == D_LCOMTYPE_SRC)
			{
				if (nType == 10) LcomD.nActive = 8;  //×¼ÓÀ¾Ã×éºÏ
				if (nType == 11) LcomD.nActive = 7;  //ÆµÓö×éºÏ
			}
			T_LCOM_D LcomD_cf = LcomD; // MNET:1614, KJH20050809, CF°¡ Á¦¿ÜµÈ load combination
			LoadCaseNo = GetCountLcomCase(LcomD);

			BOOL bLive = FALSE, bCF = FALSE;
			//BOOL bFindCJJLeadingMLoad = FALSE;
			// moving load
			if (m_nMCount > 0)
			{
				Set_MovingCase(arMLoad.GetAt(m), dLFactor / (1. + dIL), LoadCaseNo, LcomD, TRUE);
				if (nType == 12 || dIL < m_dZeroLim) strDesc.Format(_T("+%sM"), GetFactorString(dLFactor));
				else                            strDesc.Format(_T("+%s/(1+mu)M"), GetFactorString(dLFactor));
				LcomD.Description += strDesc;
				if (m_nMCount > 1) { strDesc.Format(_T("[%d]"), m + 1); LcomD.Description += strDesc; }
			}
			int nLL = 0, nIL = 0, nCF = 0;
			if (m_nLCount > 0 || (bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(L)))) // ST or ST+CS
			{
				for (nc = 0; nc < m_nLCount; nc++) nLL += Set_STLCIncludeCS(arLLoad.GetAt(nc), dLFactor, LoadCaseNo, LcomD, 0);
				if (nLL > 0) { strDesc.Format(_T("+%sL"), GetFactorString(dLFactor)); LcomD.Description += strDesc; }
				if (bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(L)))
				{
					Set_CStageCase(m_aCSLoad.GetAt(nnn), dLFactor, LoadCaseNo, LcomD, TRUE);
					strDesc.Format(_T("+%s(cEL)"), GetFactorString(dLFactor)); LcomD.Description += strDesc;
				}
				for (nc = 0; nc < m_nIlCount; nc++) nIL += Set_STLCIncludeCS(arIlLoad.GetAt(nc), dLFactor, LoadCaseNo, LcomD, 0);
				if (nIL > 0) { strDesc.Format(_T("+%sIL"), GetFactorString(dLFactor)); LcomD.Description += strDesc; }
			}
			bLive = m_nMCount > 0 || (m_nLCount > 0 || (bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(L))));
			if (bLive)
			{
				LcomD_cf = LcomD;
				for (nc = 0; nc < m_nCfCount; nc++) nCF += Set_STLCIncludeCS(arCfLoad.GetAt(nc), dLFactor, LoadCaseNo, LcomD, 0);
				if (nCF > 0) { strDesc.Format(_T("+%sCF"), GetFactorString(dLFactor)); LcomD.Description += strDesc; bCF = TRUE; }
			}

			if (m_nLsCount > 0) // Live Load Surcharge µØÃæ¸½¼Ó»îºÉÔØ
			{
				T_LCOM_D LcomD_l = LcomD; LoadCaseNo = GetCountLcomCase(LcomD_l);
				int nLS = 0;
				for (nc = 0; nc < m_nLsCount; nc++) nLS += Set_STLCIncludeCS(arLsLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD_l, 0);
				T_LCOM_D LcomD_tmp = LcomD_cf;
				if (bCF)
				{
					LoadCaseNo = GetCountLcomCase(LcomD_tmp);
					for (nc = 0; nc < m_nLsCount; nc++) Set_STLCIncludeCS(arLsLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD_tmp, 0);
				}
				if (nLS > 0)
				{
					strDesc.Format(_T("+%sLS"), GetFactorString(dFactor)); LcomD_l.Description += strDesc;
					if (bCF) LcomD_tmp.Description += strDesc;
					SetCvlESCGB19_ETC_S(LcomD_l, LcomD_tmp, bCF, arCrlLoad, arMCrlLoad, TRUE, arBrkLoad, bLive, arWlLoad, TRUE, arSfLoad, TRUE,
						arIpLoad, TRUE, arTLoad, arTpgLoad, TRUE, arFrLoad, TRUE, 0, nMsgType);
				}
			}
			if (m_nCrlCount > 0 || m_nMCrlCount > 0) // Crowd Load ÈËÈººÉÔØ
			{
				T_LCOM_D LcomD_l = LcomD; LoadCaseNo = GetCountLcomCase(LcomD_l);
				int nCRL = 0;
				for (nc = 0; nc < m_nCrlCount; nc++) nCRL += Set_STLCIncludeCS(arCrlLoad.GetAt(nc), dCRLFactor, LoadCaseNo, LcomD_l, 0);
				T_LCOM_D LcomD_tmp = LcomD_cf;
				if (bCF)
				{
					LoadCaseNo = GetCountLcomCase(LcomD_tmp);
					for (nc = 0; nc < m_nCrlCount; nc++) Set_STLCIncludeCS(arCrlLoad.GetAt(nc), dCRLFactor, LoadCaseNo, LcomD_tmp, 0);
				}
				// MNET:XXXX-BJLEE(BBONG)-20060502 
				if (nCRL > 0 || m_nMCrlCount > 0)
				{
					if (nCRL > 0)
					{
						strDesc.Format(_T("+%sCRL"), GetFactorString(dCRLFactor)); LcomD_l.Description += strDesc;
						if (bCF) LcomD_tmp.Description += strDesc;
					}

					int nMCrlCount = (m_nMCrlCount > 0) ? m_nMCrlCount : ((nCRL > 0) ? 1 : 0);
					T_LCOM_D LcomD_tmp2;
					for (int crl = 0; crl < nMCrlCount; ++crl)
					{
						T_LCOM_D LcomD_crl = LcomD_l; LoadCaseNo = GetCountLcomCase(LcomD_crl);
						LcomD_tmp2.Initialize();
						if (m_nMCrlCount > 0)
						{
							Set_MovingCase(arMCrlLoad.GetAt(crl), dCRLFactor, LoadCaseNo, LcomD_crl, TRUE);
							strDesc.Format(_T("+%sMCRL"), GetFactorString(dCRLFactor)); LcomD_crl.Description += strDesc;
							if (m_nMCrlCount > 1) { strDesc.Format(_T("[%d]"), crl + 1); LcomD_crl.Description += strDesc; }

							if (bCF)
							{
								LcomD_tmp2 = LcomD_tmp; LoadCaseNo = GetCountLcomCase(LcomD_tmp2);
								Set_MovingCase(arMCrlLoad.GetAt(crl), dCRLFactor, LoadCaseNo, LcomD_tmp2, TRUE);
								strDesc.Format(_T("+%sMCRL"), GetFactorString(dCRLFactor)); LcomD_tmp2.Description += strDesc;
								if (m_nMCrlCount > 1) { strDesc.Format(_T("[%d]"), crl + 1); LcomD_tmp2.Description += strDesc; }
							}
						}
						//if((nCRL>0 || m_nMCrlCount>0) && bCF)
						//{
						SetCvlESCGB19_ETC_S(LcomD_crl, LcomD_tmp2, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, bLive, arWlLoad, TRUE, arSfLoad, TRUE, arIpLoad, TRUE,
							arTLoad, arTpgLoad, TRUE, arFrLoad, TRUE, 0, nMsgType);
						//}
					}
				}
			}

			if (m_nBrkCount > 0 && bLive) // Brake Load ÖÆ¶¯Á¦
			{
				T_LCOM_D LcomD_l = LcomD; LoadCaseNo = GetCountLcomCase(LcomD_l);
				double dBrkF = bCF ? dFactor * 0.7 : dFactor;
				int nBRK = 0;
				for (nc = 0; nc < m_nBrkCount; nc++) nBRK += Set_STLCIncludeCS(arBrkLoad.GetAt(nc), dBrkF, LoadCaseNo, LcomD_l, 0);
				if (nBRK > 0)
				{
					strDesc.Format(_T("+%sBRK"), GetFactorString(dBrkF)); LcomD_l.Description += strDesc;
					SetCvlESCGB19_ETC_S(LcomD_l, LcomD_cf, FALSE, arTmp, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWlLoad, TRUE, arSfLoad, FALSE, arIpLoad, FALSE,
						arTLoad, arTpgLoad, TRUE, arFrLoad, FALSE, 0, nMsgType);
				}
				if (bCF) // MNET:1614, KJH20060809, ¾Õ¿¡¼­ CF°¡ Á¸ÀçÇßÀ¸¸é, CF°¡ Á¦¿ÜµÈ ÇÏÁßÁ¶ÇÕÀ¸·Î ÇÑ¹ø´õ
				{
					LcomD_l = LcomD_cf; LoadCaseNo = GetCountLcomCase(LcomD_l);
					dBrkF = dFactor;
					nBRK = 0;
					for (nc = 0; nc < m_nBrkCount; nc++) nBRK += Set_STLCIncludeCS(arBrkLoad.GetAt(nc), dBrkF, LoadCaseNo, LcomD_l, 0);
					if (nBRK > 0)
					{
						strDesc.Format(_T("+%sBRK"), GetFactorString(dBrkF)); LcomD_l.Description += strDesc;
						SetCvlESCGB19_ETC_S(LcomD_l, LcomD_cf, FALSE, arTmp, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWlLoad, TRUE, arSfLoad, FALSE, arIpLoad, FALSE,
							arTLoad, arTpgLoad, TRUE, arFrLoad, FALSE, 0, nMsgType);
					}
				}
			}

			if (bUseWlLoad)
			{
				int nWCount = m_nWlCount * 2; // MNET:1614, KJH20050808 ·çºÉÔØ
				if (m_nWlCount == 0 && bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(WL))) nWCount = 1;
				for (nc = 0; nc < nWCount; nc++)
				{
					T_LCOM_D LcomD_l = LcomD; LoadCaseNo = GetCountLcomCase(LcomD_l);
					if (m_nWlCount > 0)
					{
						double dWF = (nc % 2 == 0) ? dWlFactor : -1.*dWlFactor;
						if (Set_STLCIncludeCS(arWlLoad.GetAt(nc / 2), dWF, LoadCaseNo, LcomD_l, 0) > 0)
						{
							CString strW = (dWF > 0.0) ? _T("+%sWL") : _T("%sWL");
							strDesc.Format(strW, GetFactorString(dWF)); LcomD_l.Description += strDesc;
							if (m_nWlCount > 1) { strDesc.Format(_T("[%d]"), nc / 2 + 1); LcomD_l.Description += strDesc; }
						}
					}
					if (bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(WL)))
					{
						Set_CStageCase(m_aCSLoad.GetAt(nnn), dWlFactor, LoadCaseNo, LcomD_l, TRUE);
						strDesc.Format(_T("+%s(cEL)"), GetFactorString(dWlFactor)); LcomD_l.Description += strDesc;
					}
					SetCvlESCGB19_ETC_S(LcomD_l, LcomD_cf, bCF, arTmp, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWlLoad, FALSE, arSfLoad, TRUE, arIpLoad, TRUE,
						arTLoad, arTpgLoad, TRUE, arFrLoad, TRUE, 0, nMsgType);
				}
			}
			else
			{
				int nWCount = m_nWCount * 2; // MNET:1614, KJH20050808 ·çºÉÔØ
				if (m_nWCount == 0 && bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(W))) nWCount = 1;
				for (nc = 0; nc < nWCount; nc++)
				{
					T_LCOM_D LcomD_l = LcomD; LoadCaseNo = GetCountLcomCase(LcomD_l);
					if (m_nWCount > 0)
					{
						double dWF = (nc % 2 == 0) ? dWFactor : -1.*dWFactor;
						if (Set_STLCIncludeCS(arWLoad.GetAt(nc / 2), dWF, LoadCaseNo, LcomD_l, 0) > 0)
						{
							CString strW = (dWF > 0.0) ? _T("+%sW") : _T("%sW");
							strDesc.Format(strW, GetFactorString(dWF)); LcomD_l.Description += strDesc;
							if (m_nWCount > 1) { strDesc.Format(_T("[%d]"), nc / 2 + 1); LcomD_l.Description += strDesc; }
						}
					}
					if (bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(W)))
					{
						Set_CStageCase(m_aCSLoad.GetAt(nnn), dWFactor, LoadCaseNo, LcomD_l, TRUE);
						strDesc.Format(_T("+%s(cEL)"), GetFactorString(dWFactor)); LcomD_l.Description += strDesc;
					}
					SetCvlESCGB19_ETC_S(LcomD_l, LcomD_cf, bCF, arTmp, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWlLoad, FALSE, arSfLoad, TRUE, arIpLoad, TRUE,
						arTLoad, arTpgLoad, TRUE, arFrLoad, TRUE, 0, nMsgType);
				}
			}
			if (m_nSfCount > 0) // Stream Flow Pressure Á÷Ë®Ñ¹Á¦
			{
				T_LCOM_D LcomD_l = LcomD; LoadCaseNo = GetCountLcomCase(LcomD_l);
				int nSF = 0;
				for (nc = 0; nc < m_nSfCount; nc++) nSF += Set_STLCIncludeCS(arSfLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD_l, 0);
				if (nSF > 0)
				{
					strDesc.Format(_T("+%sSF"), GetFactorString(dFactor)); LcomD_l.Description += strDesc;
					SetCvlESCGB19_ETC_S(LcomD_l, LcomD_cf, bCF, arTmp, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWlLoad, FALSE, arSfLoad, FALSE, arIpLoad, FALSE,
						arTLoad, arTpgLoad, TRUE, arFrLoad, TRUE, 0, nMsgType);
				}
			}
			if (m_nIpCount > 0) // Ice Pressure ±ùÑ¹Á¦
			{
				T_LCOM_D LcomD_l = LcomD; LoadCaseNo = GetCountLcomCase(LcomD_l);
				int nIP = 0;
				for (nc = 0; nc < m_nIpCount; nc++) nIP += Set_STLCIncludeCS(arIpLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD_l, 0);
				if (nIP > 0)
				{
					strDesc.Format(_T("+%sIP"), GetFactorString(dFactor)); LcomD_l.Description += strDesc;
					SetCvlESCGB19_ETC_S(LcomD_l, LcomD_cf, bCF, arTmp, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWlLoad, FALSE, arSfLoad, FALSE, arIpLoad, FALSE,
						arTLoad, arTpgLoad, TRUE, arFrLoad, TRUE, 0, nMsgType);
				}
			}
			if ((m_nTCount > 0 || m_nTpgCount > 0) || (bAddCsLoad && (CheckCSEL(D_SGLD_ER1_CVL, _LSX(T)) || CheckCSEL(D_SGLD_ER1_CVL, _LSX(TPG))))) // Temeperature ÎÂ¶ÈºÉÔØ
			{
				T_LCOM_D LcomD_l = LcomD;
				SetCvlJTJD60_Capacity_TPG(LcomD_l, arTLoad, arTpgLoad, arFrLoad, dFactor, dTPGFactor, TRUE, FALSE, nMsgType, D_SER, nServEnvType);
			}
		} // next m
	} // next lc

	return TRUE;
}


BOOL CLoadCombCtrl::SetCvlESCGB19_Var3_S(const CArray<T_LCOM_D, T_LCOM_D&>& arData, ArUNT& arMLoad, double dIL, ArINT& arLLoad, ArINT& arIlLoad, ArINT& arCrlLoad, ArINT& arMCrlLoad, ArINT& arCfLoad, ArINT& arLsLoad, ArINT& arWLoad, ArINT& arWlLoad, ArINT& arSfLoad, ArINT& arIpLoad, ArINT& arTLoad, ArINT& arTpgLoad, ArINT& arFrLoad, ArINT& arBrkLoad, int nType, int nMsgType)
{
	// nType == 10:×¼ÓÀ¾Ã×éºÏ 11:ÆµÓö×éºÏ 12:±ê×¼×éºÏ
	CArray<T_LCOM_D, T_LCOM_D&> aLcomD; aLcomD.Copy(arData);
	int nLcomSize = aLcomD.GetSize();
	int  nnn = 0;

	BOOL bErec = FALSE;
	for (int nErec = 0; nErec < D_STCT_EREC_MAX_CVL; ++nErec)
	{
		if (m_nConstLoad != 0 && CheckCS_SgldK(D_SGLD_ER1_CVL + nErec, nnn)) bErec = TRUE;
	}

	BOOL bAddCsLoad = bErec;
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER1_CVL,nnn)) || 
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER2_CVL,nnn)) || 
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER3_CVL,nnn)) ||
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER4_CVL,nnn)) ||
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER5_CVL,nnn)) ||
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER6_CVL,nnn)) ||
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER7_CVL,nnn)) ||
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER8_CVL,nnn)) ||
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER9_CVL,nnn)) ||
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER10_CVL,nnn));// CS or ST+CS;

	double dFactor = 1.0, dLFactor = 1.0, dWFactor = 1.0, dWlFactor = 1.0, dCRLFactor = 1.0, dTPGFactor = 1.0;
	if (nType == 10) { dLFactor = 0.4, dWFactor = 0.75, dWlFactor = 1.0, dTPGFactor = 0.8, dCRLFactor = 0.4; }
	else if (nType == 11) { dLFactor = 0.7, dWFactor = 0.75, dWlFactor = 1.0, dTPGFactor = 0.8, dCRLFactor = 1.0; }//modified by gongxing at 2016-01-22

	BOOL bUseWlLoad = TRUE;

	ArINT arTmp;

	int nServEnvType = (nType == 12) ? 1 : 0;
	// ÓÀ¾Ã×÷ÓÃ + ÒÆ¶¯ºÉÔØ + 3¿É±ä×÷ÓÃ
	CString strDesc = _T("");
	int LoadCaseNo = 0, nc = 0;
	for (int lc = 0; lc < nLcomSize; lc++)
	{
		int nMCount = max(m_nMCount, 1);
		for (int m = 0; m < nMCount; m++)
		{
			T_LCOM_D LcomD = aLcomD.GetAt(lc); LcomD.nActive = 1; // Active or Strength/Stress
			if (m_nLcomType == D_LCOMTYPE_CONCRETE)
			{
				if (nType == 10)   LcomD.nActive = 8;  // ×¼ÓÀ¾Ã×éºÏ
				if (nType == 11)   LcomD.nActive = 7;  // ÆµÓö×éºÏ
				if (nType == 12)
				{
					LcomD.bElasticStage = TRUE;
					LcomD.nActive = 2;
				}
			}
			else if (m_nLcomType == D_LCOMTYPE_STLCOMP || m_nLcomType == D_LCOMTYPE_SRC)
			{
				if (nType == 10) LcomD.nActive = 8;  //×¼ÓÀ¾Ã×éºÏ
				if (nType == 11) LcomD.nActive = 7;  //ÆµÓö×éºÏ
			}
			T_LCOM_D LcomD_cf = LcomD; // MNET:1614, KJH20050809, CF°¡ Á¦¿ÜµÈ load combination
			LoadCaseNo = GetCountLcomCase(LcomD);

			BOOL bLive = FALSE, bCF = FALSE;
			//BOOL bFindCJJLeadingMLoad = FALSE;
			// moving load
			if (m_nMCount > 0)
			{
				Set_MovingCase(arMLoad.GetAt(m), dLFactor / (1. + dIL), LoadCaseNo, LcomD, TRUE);
				if (nType == 12 || dIL < m_dZeroLim) strDesc.Format(_T("+%sM"), GetFactorString(dLFactor));
				else                            strDesc.Format(_T("+%s/(1+mu)M"), GetFactorString(dLFactor));
				LcomD.Description += strDesc;
				if (m_nMCount > 1) { strDesc.Format(_T("[%d]"), m + 1); LcomD.Description += strDesc; }
			}
			int nLL = 0, nIL = 0, nCF = 0;
			if (m_nLCount > 0 || (bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(L)))) // ST or ST+CS
			{
				for (nc = 0; nc < m_nLCount; nc++) nLL += Set_STLCIncludeCS(arLLoad.GetAt(nc), dLFactor, LoadCaseNo, LcomD, 0);
				if (nLL > 0) { strDesc.Format(_T("+%sL"), GetFactorString(dLFactor)); LcomD.Description += strDesc; }
				if (bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(L)))
				{
					Set_CStageCase(m_aCSLoad.GetAt(nnn), dLFactor, LoadCaseNo, LcomD, TRUE);
					strDesc.Format(_T("+%s(cEL)"), GetFactorString(dLFactor)); LcomD.Description += strDesc;
				}
				for (nc = 0; nc < m_nIlCount; nc++) nIL += Set_STLCIncludeCS(arIlLoad.GetAt(nc), dLFactor, LoadCaseNo, LcomD, 0);
				if (nIL > 0) { strDesc.Format(_T("+%sIL"), GetFactorString(dLFactor)); LcomD.Description += strDesc; }
			}
			bLive = m_nMCount > 0 || (m_nLCount > 0 || (bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(L))));
			if (bLive)
			{
				LcomD_cf = LcomD;
				for (nc = 0; nc < m_nCfCount; nc++) nCF += Set_STLCIncludeCS(arCfLoad.GetAt(nc), dLFactor, LoadCaseNo, LcomD, 0);
				if (nCF > 0) { strDesc.Format(_T("+%sCF"), GetFactorString(dLFactor)); LcomD.Description += strDesc; bCF = TRUE; }
			}

			if (m_nLsCount > 0) // Live Load Surcharge µØÃæ¸½¼Ó»îºÉÔØ
			{
				T_LCOM_D LcomD_l = LcomD; LoadCaseNo = GetCountLcomCase(LcomD_l);
				int nLS = 0;
				for (nc = 0; nc < m_nLsCount; nc++) nLS += Set_STLCIncludeCS(arLsLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD_l, 0);
				T_LCOM_D LcomD_tmp = LcomD_cf; LoadCaseNo = GetCountLcomCase(LcomD_tmp);
				if (bCF)
				{
					for (nc = 0; nc < m_nLsCount; nc++) Set_STLCIncludeCS(arLsLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD_tmp, 0);
				}
				if (nLS > 0)
				{
					strDesc.Format(_T("+%sLS"), GetFactorString(dFactor)); LcomD_l.Description += strDesc;
					if (bCF) LcomD_tmp.Description += strDesc;
					SetCvlESCGB19_ETC_S(LcomD_l, LcomD_tmp, bCF, arCrlLoad, arMCrlLoad, TRUE, arBrkLoad, bLive, arWlLoad, TRUE, arSfLoad, TRUE, arIpLoad, TRUE,
						arTLoad, arTpgLoad, TRUE, arFrLoad, FALSE, 1, nMsgType);
				}
			}

			// MNET:XXXX-BJLEE(BBONG)-20060503
			if (m_nCrlCount > 0 || m_nMCrlCount > 0) // Crowd Load ÈËÈººÉÔØ
			{
				T_LCOM_D LcomD_l = LcomD; LoadCaseNo = GetCountLcomCase(LcomD_l);
				int nCRL = 0;
				for (nc = 0; nc < m_nCrlCount; nc++) nCRL += Set_STLCIncludeCS(arCrlLoad.GetAt(nc), dCRLFactor, LoadCaseNo, LcomD_l, 0);
				T_LCOM_D LcomD_tmp = LcomD_cf; LoadCaseNo = GetCountLcomCase(LcomD_tmp);
				if (bCF)
				{
					for (nc = 0; nc < m_nCrlCount; nc++) Set_STLCIncludeCS(arCrlLoad.GetAt(nc), dCRLFactor, LoadCaseNo, LcomD_tmp, 0);
				}
				// MNET:XXXX-BJLEE(BBONG)-20060502 
				if (nCRL > 0 || m_nMCrlCount > 0)
				{
					if (nCRL > 0)
					{
						strDesc.Format(_T("+%sCRL"), GetFactorString(dCRLFactor)); LcomD_l.Description += strDesc;
						if (bCF) LcomD_tmp.Description += strDesc;
					}

					int nMCrlCount = (m_nMCrlCount > 0) ? m_nMCrlCount : ((nCRL > 0) ? 1 : 0);
					T_LCOM_D LcomD_tmp2;
					for (int crl = 0; crl < nMCrlCount; ++crl)
					{
						T_LCOM_D LcomD_crl = LcomD_l; LoadCaseNo = GetCountLcomCase(LcomD_crl);
						LcomD_tmp2.Initialize();
						if (m_nMCrlCount > 0)
						{
							Set_MovingCase(arMCrlLoad.GetAt(crl), dCRLFactor, LoadCaseNo, LcomD_crl, TRUE);
							strDesc.Format(_T("+%sMCRL"), GetFactorString(dCRLFactor)); LcomD_crl.Description += strDesc;
							if (m_nMCrlCount > 1) { strDesc.Format(_T("[%d]"), crl + 1); LcomD_crl.Description += strDesc; }

							if (bCF)
							{
								LcomD_tmp2 = LcomD_tmp; LoadCaseNo = GetCountLcomCase(LcomD_tmp2);
								Set_MovingCase(arMCrlLoad.GetAt(crl), dCRLFactor, LoadCaseNo, LcomD_tmp2, TRUE);
								strDesc.Format(_T("+%sMCRL"), GetFactorString(dCRLFactor)); LcomD_tmp2.Description += strDesc;
								if (m_nMCrlCount > 1) { strDesc.Format(_T("[%d]"), crl + 1); LcomD_tmp2.Description += strDesc; }
							}
						}
						//if((nCRL>0 || m_nMCrlCount>0) && bCF)
						//{
						SetCvlESCGB19_ETC_S(LcomD_crl, LcomD_tmp2, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, bLive, arWlLoad, TRUE, arSfLoad, TRUE, arIpLoad, TRUE,
							arTLoad, arTpgLoad, TRUE, arFrLoad, FALSE, 2, nMsgType);
						//}
					}
				}
			}


			if (m_nBrkCount > 0 && bLive) // Brake Load ÖÆ¶¯Á¦
			{
				T_LCOM_D LcomD_l = LcomD; LoadCaseNo = GetCountLcomCase(LcomD_l);
				double dBrkF = bCF ? dFactor * 0.7 : dFactor;
				int nBRK = 0;
				for (nc = 0; nc < m_nBrkCount; nc++) nBRK += Set_STLCIncludeCS(arBrkLoad.GetAt(nc), dBrkF, LoadCaseNo, LcomD_l, 0);
				if (nBRK > 0)
				{
					strDesc.Format(_T("+%sBRK"), GetFactorString(dBrkF)); LcomD_l.Description += strDesc;
					SetCvlESCGB19_ETC_S(LcomD_l, LcomD_cf, FALSE, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWlLoad, TRUE, arSfLoad, FALSE, arIpLoad, FALSE,
						arTLoad, arTpgLoad, FALSE, arFrLoad, FALSE, 3, nMsgType);
				}
				if (bCF) // MNET:1614, KJH20050809, ¾Õ¿¡¼­ CF°¡ Á¸ÀçÇÏ¸é CF°¡ Á¦¿ÜµÈ ÇÏÁßÁ¶ÇÕ ÇÑ¹ø´õ...
				{
					LcomD_l = LcomD_cf; LoadCaseNo = GetCountLcomCase(LcomD_l);
					dBrkF = dFactor;
					int nBRK = 0;
					for (nc = 0; nc < m_nBrkCount; nc++) nBRK += Set_STLCIncludeCS(arBrkLoad.GetAt(nc), dBrkF, LoadCaseNo, LcomD_l, 0);
					if (nBRK > 0)
					{
						strDesc.Format(_T("+%sBRK"), GetFactorString(dBrkF)); LcomD_l.Description += strDesc;
						SetCvlESCGB19_ETC_S(LcomD_l, LcomD_cf, FALSE, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWlLoad, TRUE, arSfLoad, FALSE, arIpLoad, FALSE,
							arTLoad, arTpgLoad, FALSE, arFrLoad, FALSE, 3, nMsgType);
					}
				}
			}

			if (bUseWlLoad)
			{
				int nWCount = m_nWlCount * 2; // MNET:1614, KJH20050808 ·çºÉÔØ
				if (m_nWlCount == 0 && bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(WL))) nWCount = 1;
				for (nc = 0; nc < nWCount; nc++)
				{
					T_LCOM_D LcomD_l = LcomD; LoadCaseNo = GetCountLcomCase(LcomD_l);
					if (m_nWlCount > 0)
					{
						double dWF = (nc % 2 == 0) ? dWlFactor : -1.*dWlFactor;
						if (Set_STLCIncludeCS(arWlLoad.GetAt(nc / 2), dWF, LoadCaseNo, LcomD_l, 0) > 0)
						{
							CString strW = (dWF > 0.0) ? _T("+%sWL") : _T("%sWL");
							strDesc.Format(strW, GetFactorString(dWF)); LcomD_l.Description += strDesc;
							if (m_nWlCount > 1) { strDesc.Format(_T("[%d]"), nc / 2 + 1); LcomD_l.Description += strDesc; }
						}
					}
					if (bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(WL)))
					{
						Set_CStageCase(m_aCSLoad.GetAt(nnn), dWlFactor, LoadCaseNo, LcomD_l, TRUE);
						strDesc.Format(_T("+%s(cEL)"), GetFactorString(dWlFactor)); LcomD_l.Description += strDesc;
					}
					SetCvlESCGB19_ETC_S(LcomD_l, LcomD_cf, bCF, arTmp, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWlLoad, FALSE, arSfLoad, TRUE, arIpLoad, TRUE,
						arTLoad, arTpgLoad, TRUE, arFrLoad, FALSE, 4, nMsgType);
				}
			}
			else
			{
				int nWCount = m_nWCount * 2; // MNET:1614, KJH20050808 ·çºÉÔØ
				if (m_nWCount == 0 && bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(W))) nWCount = 1;
				for (nc = 0; nc < nWCount; nc++)
				{
					T_LCOM_D LcomD_l = LcomD; LoadCaseNo = GetCountLcomCase(LcomD_l);
					if (m_nWCount > 0)
					{
						double dWF = (nc % 2 == 0) ? dWFactor : -1.*dWFactor;
						if (Set_STLCIncludeCS(arWLoad.GetAt(nc / 2), dWF, LoadCaseNo, LcomD_l, 0) > 0)
						{
							CString strW = (dWF > 0.0) ? _T("+%sW") : _T("%sW");
							strDesc.Format(strW, GetFactorString(dWF)); LcomD_l.Description += strDesc;
							if (m_nWCount > 1) { strDesc.Format(_T("[%d]"), nc / 2 + 1); LcomD_l.Description += strDesc; }
						}
					}
					if (bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(W)))
					{
						Set_CStageCase(m_aCSLoad.GetAt(nnn), dWFactor, LoadCaseNo, LcomD_l, TRUE);
						strDesc.Format(_T("+%s(cEL)"), GetFactorString(dWFactor)); LcomD_l.Description += strDesc;
					}
					SetCvlESCGB19_ETC_S(LcomD_l, LcomD_cf, bCF, arTmp, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWlLoad, FALSE, arSfLoad, TRUE, arIpLoad, TRUE,
						arTLoad, arTpgLoad, TRUE, arFrLoad, FALSE, 4, nMsgType);
				}
			}
			if (m_nSfCount > 0) // Stream Flow Pressure Á÷Ë®Ñ¹Á¦
			{
				T_LCOM_D LcomD_l = LcomD; LoadCaseNo = GetCountLcomCase(LcomD_l);
				int nSF = 0;
				for (nc = 0; nc < m_nSfCount; nc++) nSF += Set_STLCIncludeCS(arSfLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD_l, 0);
				if (nSF > 0)
				{
					strDesc.Format(_T("+%sSF"), GetFactorString(dFactor)); LcomD_l.Description += strDesc;
					SetCvlESCGB19_ETC_S(LcomD_l, LcomD_cf, bCF, arTmp, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWlLoad, FALSE, arSfLoad, FALSE, arIpLoad, FALSE,
						arTLoad, arTpgLoad, TRUE, arFrLoad, FALSE, 5, nMsgType);
				}
			}
			if (m_nIpCount > 0) // Ice Pressure ±ùÑ¹Á¦
			{
				T_LCOM_D LcomD_l = LcomD; LoadCaseNo = GetCountLcomCase(LcomD_l);
				int nIP = 0;
				for (nc = 0; nc < m_nIpCount; nc++) nIP += Set_STLCIncludeCS(arIpLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD_l, 0);
				if (nIP > 0)
				{
					strDesc.Format(_T("+%sIP"), GetFactorString(dFactor)); LcomD_l.Description += strDesc;
					SetCvlESCGB19_ETC_S(LcomD_l, LcomD_cf, bCF, arTmp, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWlLoad, FALSE, arSfLoad, FALSE, arIpLoad, FALSE,
						arTLoad, arTpgLoad, TRUE, arFrLoad, FALSE, 5, nMsgType);
				}
			}
		} // next m
	} // next lc

	return TRUE;
}
BOOL CLoadCombCtrl::SetCvlESCGB19_Var4_S(const CArray<T_LCOM_D, T_LCOM_D&>& arData, ArUNT& arMLoad, double dIL, ArINT& arLLoad, ArINT& arIlLoad, ArINT& arCrlLoad, ArINT& arMCrlLoad, ArINT& arCfLoad, ArINT& arLsLoad, ArINT& arWLoad, ArINT& arWlLoad, ArINT& arSfLoad, ArINT& arIpLoad, ArINT& arTLoad, ArINT& arTpgLoad, ArINT& arFrLoad, ArINT& arBrkLoad, int nType, int nMsgType)
{
	// nType == 10:×¼ÓÀ¾Ã×éºÏ, 11:ÆµÓö×éºÏ 12:±ê×¼×éºÏ
	CArray<T_LCOM_D, T_LCOM_D&> aLcomD; aLcomD.Copy(arData);
	int  nLcomSize = aLcomD.GetSize();
	int  nnn = 0;

	BOOL bErec = FALSE;
	for (int nErec = 0; nErec < D_STCT_EREC_MAX_CVL; ++nErec)
	{
		if (m_nConstLoad != 0 && CheckCS_SgldK(D_SGLD_ER1_CVL + nErec, nnn)) bErec = TRUE;
	}

	BOOL bAddCsLoad = bErec;
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER1_CVL,nnn)) || 
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER2_CVL,nnn)) || 
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER3_CVL,nnn)) ||
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER4_CVL,nnn)) ||
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER5_CVL,nnn)) ||
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER6_CVL,nnn)) ||
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER7_CVL,nnn)) ||
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER8_CVL,nnn)) ||
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER9_CVL,nnn)) ||
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER10_CVL,nnn));// CS or ST+CS;

	double dFactor = 1.0, dLFactor = 1.0, dWFactor = 1.0, dWlFactor = 1.0, dCRLFactor = 1.0, dTPGFactor = 1.0;
	if (nType == 10) { dLFactor = 0.4, dWFactor = 0.75, dWlFactor = 1.0, dTPGFactor = 0.8, dCRLFactor = 0.4; }
	else if (nType == 11) { dLFactor = 0.7, dWFactor = 0.75, dWlFactor = 1.0, dTPGFactor = 0.8, dCRLFactor = 1.0; }//modified by gongxing at 2016-01-22

	BOOL bUseWlLoad = TRUE;


	ArINT arTmp;

	int nServEnvType = (nType == 12) ? 1 : 0;
	// ÓÀ¾Ã×÷ÓÃ + ÆµÓö×éºÏ+ 4¿É±ä×÷ÓÃ
	CString strDesc = _T("");
	int LoadCaseNo = 0, nc = 0;
	for (int lc = 0; lc < nLcomSize; lc++)
	{
		int nMCount = max(m_nMCount, 1);
		for (int m = 0; m < nMCount; m++)
		{
			T_LCOM_D LcomD = aLcomD.GetAt(lc); LcomD.nActive = 1; // Active or Strength/Stress
			if (m_nLcomType == D_LCOMTYPE_CONCRETE)
			{
				if (nType == 10)   LcomD.nActive = 8;  // ×¼ÓÀ¾Ã×éºÏ
				if (nType == 11)   LcomD.nActive = 7;  // ÆµÓö×éºÏ
				if (nType == 12)
				{
					LcomD.bElasticStage = TRUE;
					LcomD.nActive = 2;
				}
			}
			else if (m_nLcomType == D_LCOMTYPE_STLCOMP || m_nLcomType == D_LCOMTYPE_SRC)
			{
				if (nType == 10) LcomD.nActive = 8;  //×¼ÓÀ¾Ã×éºÏ
				if (nType == 11) LcomD.nActive = 7;  //ÆµÓö×éºÏ
			}
			T_LCOM_D LcomD_cf = LcomD; // MNET:1614, KJH20050809, CF°¡ Á¦¿ÜµÈ ÇÏÁßÁ¶ÇÕ
			LoadCaseNo = GetCountLcomCase(LcomD);
			BOOL bLive = FALSE, bCF = FALSE;
			//BOOL bFindCJJLeadingMLoad = FALSE;
			// moving load
			if (m_nMCount > 0)
			{
				Set_MovingCase(arMLoad.GetAt(m), dLFactor / (1. + dIL), LoadCaseNo, LcomD, TRUE);
				if (nType == 12 || dIL < m_dZeroLim) strDesc.Format(_T("+%sM"), GetFactorString(dLFactor));
				else                            strDesc.Format(_T("+%s/(1+mu)M"), GetFactorString(dLFactor));
				LcomD.Description += strDesc;
				if (m_nMCount > 1) { strDesc.Format(_T("[%d]"), m + 1); LcomD.Description += strDesc; }
			}
			int nLL = 0, nIL = 0, nCF = 0;
			if (m_nLCount > 0 || (bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(L)))) // ST or ST+CS
			{
				for (nc = 0; nc < m_nLCount; nc++) nLL += Set_STLCIncludeCS(arLLoad.GetAt(nc), dLFactor, LoadCaseNo, LcomD, 0);
				if (nLL > 0) { strDesc.Format(_T("+%sL"), GetFactorString(dLFactor)); LcomD.Description += strDesc; }
				if (bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(L)))
				{
					Set_CStageCase(m_aCSLoad.GetAt(nnn), dLFactor, LoadCaseNo, LcomD, TRUE);
					strDesc.Format(_T("+%s(cEL)"), GetFactorString(dLFactor)); LcomD.Description += strDesc;
				}
				for (nc = 0; nc < m_nIlCount; nc++) nIL += Set_STLCIncludeCS(arIlLoad.GetAt(nc), dLFactor, LoadCaseNo, LcomD, 0);
				if (nIL > 0) { strDesc.Format(_T("+%sIL"), GetFactorString(dLFactor)); LcomD.Description += strDesc; }
			}
			bLive = m_nMCount > 0 || (m_nLCount > 0 || (bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(L))));
			if (bLive)
			{
				LcomD_cf = LcomD;
				for (nc = 0; nc < m_nCfCount; nc++) nCF += Set_STLCIncludeCS(arCfLoad.GetAt(nc), dLFactor, LoadCaseNo, LcomD, 0);
				if (nCF > 0) { strDesc.Format(_T("+%sCF"), GetFactorString(dLFactor)); LcomD.Description += strDesc; bCF = TRUE; }
			}

			if (m_nLsCount > 0) // Live Load Surcharge µØÃæ¸½¼Ó»îºÉÔØ
			{
				T_LCOM_D LcomD_l = LcomD; LoadCaseNo = GetCountLcomCase(LcomD_l);
				int nLS = 0;
				for (nc = 0; nc < m_nLsCount; nc++) nLS += Set_STLCIncludeCS(arLsLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD_l, 0);
				T_LCOM_D LcomD_tmp = LcomD_cf; LoadCaseNo = GetCountLcomCase(LcomD_tmp);
				if (bCF)
				{
					for (nc = 0; nc < m_nLsCount; nc++) Set_STLCIncludeCS(arLsLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD_tmp, 0);
				}
				if (nLS > 0)
				{
					strDesc.Format(_T("+%sLS"), GetFactorString(dFactor)); LcomD_l.Description += strDesc;
					if (bCF) LcomD_tmp.Description += strDesc;
					SetCvlESCGB19_ETC_S(LcomD_l, LcomD_tmp, bCF, arCrlLoad, arMCrlLoad, TRUE, arBrkLoad, bLive, arWlLoad, TRUE, arSfLoad, TRUE, arIpLoad, TRUE,
						arTLoad, arTpgLoad, FALSE, arFrLoad, FALSE, 6, nMsgType);
				}
				/*
								if(bCF)
								{
									LcomD_l = LcomD_cf; LoadCaseNo = GetCountLcomCase(LcomD_l);
									nLS = 0;
									for(nc=0; nc<m_nLsCount; nc++) nLS += Set_STLCIncludeCS(arLsLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD_l, 0);
									if(nLS>0)
									{
										strDesc.Format(_T("+%sLS"), GetFactorString(dFactor)); LcomD_l.Description += strDesc;
										SetCvlESCGB19_ETC_S(LcomD_l, LcomD_cf, bCF, arCrlLoad, TRUE, arBrkLoad, bLive, arWLoad, TRUE, arSfLoad, FALSE, arIpLoad, FALSE,
																		 arTLoad, arTpgLoad, FALSE, arFrLoad, FALSE, 101, nMsgType);
									}
								}
				*/
			}

			// MNET:XXXX-BJLEE(BBONG)-20060503
			if (m_nCrlCount > 0 || m_nMCrlCount > 0) // Crowd Load ÈËÈººÉÔØ
			{
				T_LCOM_D LcomD_l = LcomD; LoadCaseNo = GetCountLcomCase(LcomD_l);
				int nCRL = 0;
				for (nc = 0; nc < m_nCrlCount; nc++) nCRL += Set_STLCIncludeCS(arCrlLoad.GetAt(nc), dCRLFactor, LoadCaseNo, LcomD_l, 0);
				T_LCOM_D LcomD_tmp = LcomD_cf; LoadCaseNo = GetCountLcomCase(LcomD_tmp);
				if (bCF)
				{
					for (nc = 0; nc < m_nCrlCount; nc++) Set_STLCIncludeCS(arCrlLoad.GetAt(nc), dCRLFactor, LoadCaseNo, LcomD_tmp, 0);
				}
				// MNET:XXXX-BJLEE(BBONG)-20060502 
				if (nCRL > 0 || m_nMCrlCount > 0)
				{
					if (nCRL > 0)
					{
						strDesc.Format(_T("+%sCRL"), GetFactorString(dCRLFactor)); LcomD_l.Description += strDesc;
						if (bCF) LcomD_tmp.Description += strDesc;
					}

					int nMCrlCount = (m_nMCrlCount > 0) ? m_nMCrlCount : ((nCRL > 0) ? 1 : 0);
					T_LCOM_D LcomD_tmp2;
					for (int crl = 0; crl < nMCrlCount; ++crl)
					{
						T_LCOM_D LcomD_crl = LcomD_l; LoadCaseNo = GetCountLcomCase(LcomD_crl);
						LcomD_tmp2.Initialize();
						if (m_nMCrlCount > 0)
						{
							Set_MovingCase(arMCrlLoad.GetAt(crl), dCRLFactor, LoadCaseNo, LcomD_crl, TRUE);
							strDesc.Format(_T("+%sMCRL"), GetFactorString(dCRLFactor)); LcomD_crl.Description += strDesc;
							if (m_nMCrlCount > 1) { strDesc.Format(_T("[%d]"), crl + 1); LcomD_crl.Description += strDesc; }

							if (bCF)
							{
								LcomD_tmp2 = LcomD_tmp; LoadCaseNo = GetCountLcomCase(LcomD_tmp2);
								Set_MovingCase(arMCrlLoad.GetAt(crl), dCRLFactor, LoadCaseNo, LcomD_tmp2, TRUE);
								strDesc.Format(_T("+%sMCRL"), GetFactorString(dCRLFactor)); LcomD_tmp2.Description += strDesc;
								if (m_nMCrlCount > 1) { strDesc.Format(_T("[%d]"), crl + 1); LcomD_tmp2.Description += strDesc; }
							}
						}
						//if((nCRL>0 || m_nMCrlCount>0) && bCF)
						//{
						SetCvlESCGB19_ETC_S(LcomD_crl, LcomD_tmp2, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, bLive, arWlLoad, TRUE, arSfLoad, TRUE, arIpLoad, TRUE,
							arTLoad, arTpgLoad, FALSE, arFrLoad, FALSE, 7, nMsgType);
						//}
					}
				}
			}

			if (bUseWlLoad)
			{
				int nWCount = m_nWlCount * 2; // MNET:1614, KJH20050808 ·çºÉÔØ
				if (m_nWlCount == 0 && bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(WL))) nWCount = 1;
				for (nc = 0; nc < nWCount; nc++)
				{
					T_LCOM_D LcomD_l = LcomD; LoadCaseNo = GetCountLcomCase(LcomD_l);
					if (m_nWlCount > 0)
					{
						double dWF = (nc % 2 == 0) ? dWlFactor : -1.*dWlFactor;
						if (Set_STLCIncludeCS(arWlLoad.GetAt(nc / 2), dWF, LoadCaseNo, LcomD_l, 0) > 0)
						{
							CString strW = (dWF > 0.0) ? _T("+%sWL") : _T("%sWL");
							strDesc.Format(strW, GetFactorString(dWF)); LcomD_l.Description += strDesc;
							if (m_nWlCount > 1) { strDesc.Format(_T("[%d]"), nc / 2 + 1); LcomD_l.Description += strDesc; }
						}
					}
					if (bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(WL)))
					{
						Set_CStageCase(m_aCSLoad.GetAt(nnn), dWlFactor, LoadCaseNo, LcomD_l, TRUE);
						strDesc.Format(_T("+%s(cEL)"), GetFactorString(dWlFactor)); LcomD_l.Description += strDesc;
					}
					SetCvlESCGB19_ETC_S(LcomD_l, LcomD_cf, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWlLoad, FALSE, arSfLoad, TRUE, arIpLoad, TRUE,
						arTLoad, arTpgLoad, FALSE, arFrLoad, FALSE, 7, nMsgType);
				}
			}
			else
			{
				int nWCount = m_nWCount * 2; // MNET:1614, KJH20050808 ·çºÉÔØ
				if (m_nWCount == 0 && bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(W))) nWCount = 1;
				for (nc = 0; nc < nWCount; nc++)
				{
					T_LCOM_D LcomD_l = LcomD; LoadCaseNo = GetCountLcomCase(LcomD_l);
					if (m_nWCount > 0)
					{
						double dWF = (nc % 2 == 0) ? dWFactor : -1.*dWFactor;
						if (Set_STLCIncludeCS(arWLoad.GetAt(nc / 2), dWF, LoadCaseNo, LcomD_l, 0) > 0)
						{
							CString strW = (dWF > 0.0) ? _T("+%sW") : _T("%sW");
							strDesc.Format(strW, GetFactorString(dWF)); LcomD_l.Description += strDesc;
							if (m_nWCount > 1) { strDesc.Format(_T("[%d]"), nc / 2 + 1); LcomD_l.Description += strDesc; }
						}
					}
					if (bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(W)))
					{
						Set_CStageCase(m_aCSLoad.GetAt(nnn), dWFactor, LoadCaseNo, LcomD_l, TRUE);
						strDesc.Format(_T("+%s(cEL)"), GetFactorString(dWFactor)); LcomD_l.Description += strDesc;
					}
					SetCvlESCGB19_ETC_S(LcomD_l, LcomD_cf, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWlLoad, FALSE, arSfLoad, TRUE, arIpLoad, TRUE,
						arTLoad, arTpgLoad, FALSE, arFrLoad, FALSE, 7, nMsgType);
				}
			}
		} // next m
	} // next lc

	return TRUE;
}
BOOL CLoadCombCtrl::SetCvlESCGB19_Var56_S(const CArray<T_LCOM_D, T_LCOM_D&>& arData, ArUNT& arMLoad, double dIL, ArINT& arLLoad, ArINT& arIlLoad, ArINT& arCrlLoad, ArINT& arMCrlLoad, ArINT& arCfLoad, ArINT& arLsLoad, ArINT& arWLoad, ArINT& arWlLoad, ArINT& arSfLoad, ArINT& arIpLoad, ArINT& arTLoad, ArINT& arTpgLoad, ArINT& arFrLoad, ArINT& arBrkLoad, int nType, int nMsgType)
{
	// nType == 10:×¼ÓÀ¾Ã×éºÏ 11:ÆµÓö×éºÏ, 12:±ê×¼×éºÏ
	CArray<T_LCOM_D, T_LCOM_D&> aLcomD; aLcomD.Copy(arData);
	int  nLcomSize = aLcomD.GetSize();
	int  nnn = 0;

	BOOL bErec = FALSE;
	for (int nErec = 0; nErec < D_STCT_EREC_MAX_CVL; ++nErec)
	{
		if (m_nConstLoad != 0 && CheckCS_SgldK(D_SGLD_ER1_CVL + nErec, nnn)) bErec = TRUE;
	}

	BOOL bAddCsLoad = bErec;
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER1_CVL,nnn)) || 
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER2_CVL,nnn)) || 
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER3_CVL,nnn)) ||
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER4_CVL,nnn)) ||
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER5_CVL,nnn)) ||
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER6_CVL,nnn)) ||
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER7_CVL,nnn)) ||
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER8_CVL,nnn)) ||
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER9_CVL,nnn)) ||
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER10_CVL,nnn));// CS or ST+CS;

	double dFactor = 1.0, dLFactor = 1.0, dWFactor = 1.0, dCRLFactor = 1.0, dTPGFactor = 1.0;
	if (nType == 10) { dLFactor = 0.4, dWFactor = 0.75, dTPGFactor = 0.8, dCRLFactor = 0.4; }
	else if (nType == 11) { dLFactor = 0.7, dWFactor = 0.75, dTPGFactor = 0.8, dCRLFactor = 1.0; }//modified by gongxing at 2016-01-22

	ArINT arTmp;

	int nServEnvType = (nType == 12) ? 1 : 0;
	// ÓÀ¾Ã×÷ÓÃ + ÒÆ¶¯ºÉÔØ + 5¿É±ä×÷ÓÃ
	CString strDesc = _T("");
	int LoadCaseNo = 0, nc = 0;
	for (int lc = 0; lc < nLcomSize; lc++)
	{
		int nMCount = max(m_nMCount, 1);
		for (int m = 0; m < nMCount; m++)
		{
			T_LCOM_D LcomD = aLcomD.GetAt(lc); LcomD.nActive = 1; // Active or Strength/Stress
			if (m_nLcomType == D_LCOMTYPE_CONCRETE)
			{
				if (nType == 10)   LcomD.nActive = 8;  // ×¼ÓÀ¾Ã×éºÏ
				if (nType == 11)   LcomD.nActive = 7;  // ÆµÓö×éºÏ
				if (nType == 12)
				{
					LcomD.bElasticStage = TRUE;
					LcomD.nActive = 2;
				}
			}
			else if (m_nLcomType == D_LCOMTYPE_STLCOMP || m_nLcomType == D_LCOMTYPE_SRC)
			{
				if (nType == 10) LcomD.nActive = 8;  //×¼ÓÀ¾Ã×éºÏ
				if (nType == 11) LcomD.nActive = 7;  //ÆµÓö×éºÏ
			}
			T_LCOM_D LcomD_cf = LcomD; // MNET:1614, KJH20050809, CFÁ¦¿ÜÇÑ ÇÏÁßÁ¶ÇÕ
			LoadCaseNo = GetCountLcomCase(LcomD);
			BOOL bLive = FALSE, bCF = FALSE;
			//BOOL bFindCJJLeadingMLoad = FALSE;
			// moving load
			if (m_nMCount > 0)
			{
				Set_MovingCase(arMLoad.GetAt(m), dLFactor / (1. + dIL), LoadCaseNo, LcomD, TRUE);
				if (nType == 12 || dIL < m_dZeroLim) strDesc.Format(_T("+%sM"), GetFactorString(dLFactor));
				else                            strDesc.Format(_T("+%s/(1+mu)M"), GetFactorString(dLFactor));
				LcomD.Description += strDesc;
				if (m_nMCount > 1) { strDesc.Format(_T("[%d]"), m + 1); LcomD.Description += strDesc; }
			}
			int nLL = 0, nIL = 0, nCF = 0;
			if (m_nLCount > 0 || (bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(L)))) // ST or ST+CS
			{
				for (nc = 0; nc < m_nLCount; nc++) nLL += Set_STLCIncludeCS(arLLoad.GetAt(nc), dLFactor, LoadCaseNo, LcomD, 0);
				if (nLL > 0) { strDesc.Format(_T("+%sL"), GetFactorString(dLFactor)); LcomD.Description += strDesc; }
				if (bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(L)))
				{
					Set_CStageCase(m_aCSLoad.GetAt(nnn), dLFactor, LoadCaseNo, LcomD, TRUE);
					strDesc.Format(_T("+%s(cEL)"), GetFactorString(dLFactor)); LcomD.Description += strDesc;
				}
				for (nc = 0; nc < m_nIlCount; nc++) nIL += Set_STLCIncludeCS(arIlLoad.GetAt(nc), dLFactor, LoadCaseNo, LcomD, 0);
				if (nIL > 0) { strDesc.Format(_T("+%sIL"), GetFactorString(dLFactor)); LcomD.Description += strDesc; }
			}
			bLive = m_nMCount > 0 || (m_nLCount > 0 || (bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(L))));
			if (bLive)
			{
				LcomD_cf = LcomD;
				for (nc = 0; nc < m_nCfCount; nc++) nCF += Set_STLCIncludeCS(arCfLoad.GetAt(nc), dLFactor, LoadCaseNo, LcomD, 0);
				if (nCF > 0) { strDesc.Format(_T("+%sCF"), GetFactorString(dLFactor)); LcomD.Description += strDesc; bCF = TRUE; }
			}

			if (m_nLsCount > 0) // Live Load Surcharge µØÃæ¸½¼Ó»îºÉÔØ
			{
				T_LCOM_D LcomD_l = LcomD; LoadCaseNo = GetCountLcomCase(LcomD_l);
				int nLS = 0;
				for (nc = 0; nc < m_nLsCount; nc++) nLS += Set_STLCIncludeCS(arLsLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD_l, 0);
				T_LCOM_D LcomD_tmp = LcomD_cf; LoadCaseNo = GetCountLcomCase(LcomD_tmp);
				if (bCF)
				{
					for (nc = 0; nc < m_nLsCount; nc++) Set_STLCIncludeCS(arLsLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD_tmp, 0);
				}
				if (nLS > 0)
				{
					strDesc.Format(_T("+%sLS"), GetFactorString(dFactor)); LcomD_l.Description += strDesc;
					if (bCF) LcomD_tmp.Description += strDesc;
					SetCvlESCGB19_ETC_S(LcomD_l, LcomD_tmp, bCF, arCrlLoad, arMCrlLoad, TRUE, arBrkLoad, bLive, arWlLoad, TRUE, arSfLoad, FALSE, arIpLoad, FALSE,
						arTLoad, arTpgLoad, FALSE, arFrLoad, FALSE, 8, nMsgType);
				}
			}
			// MNET:XXXX-BJLEE(BBONG)-20060503
			if (m_nCrlCount > 0 || m_nMCrlCount > 0) // Crowd Load ÈËÈººÉÔØ
			{
				T_LCOM_D LcomD_l = LcomD; LoadCaseNo = GetCountLcomCase(LcomD_l);
				int nCRL = 0;
				for (nc = 0; nc < m_nCrlCount; nc++) nCRL += Set_STLCIncludeCS(arCrlLoad.GetAt(nc), dCRLFactor, LoadCaseNo, LcomD_l, 0);
				if (nCRL > 0 || m_nMCrlCount > 0)
				{
					if (nCRL > 0)
					{
						strDesc.Format(_T("+%sCRL"), GetFactorString(dCRLFactor)); LcomD_l.Description += strDesc;
					}
					int nMCrlCount = (m_nMCrlCount > 0) ? m_nMCrlCount : ((nCRL > 0) ? 1 : 0);

					for (int crl = 0; crl < nMCrlCount; ++crl)
					{
						T_LCOM_D LcomD_tmp = LcomD_l; LoadCaseNo = GetCountLcomCase(LcomD_tmp);
						if (m_nMCrlCount > 0)
						{
							Set_MovingCase(arMCrlLoad.GetAt(crl), dCRLFactor, LoadCaseNo, LcomD_tmp, TRUE);
							strDesc.Format(_T("+%sMCRL"), GetFactorString(dCRLFactor)); LcomD_tmp.Description += strDesc;
							if (m_nMCrlCount > 1) { strDesc.Format(_T("[%d]"), crl + 1); LcomD_tmp.Description += strDesc; }
						}
						if ((nCRL > 0 || m_nMCrlCount > 0))
						{
							SetCvlESCGB19_ETC_S(LcomD_tmp, LcomD_cf, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWlLoad, TRUE, arSfLoad, FALSE, arIpLoad, FALSE,
								arTLoad, arTpgLoad, FALSE, arFrLoad, FALSE, 9, nMsgType);
						}
					}
				}
			}
		} // next m
	} // next lc

	// ¿µ±¸ÇÏÁß + Â÷·®ÇÏÁß + 6°³ÀÇ °¡º¯ÇÏÁßÀÎ °æ¿ì
	for (int lc = 0; lc < nLcomSize; lc++)
	{
		int nMCount = max(m_nMCount, 1);
		for (int m = 0; m < nMCount; m++)
		{
			T_LCOM_D LcomD = aLcomD.GetAt(lc); LcomD.nActive = 1; // Active or Strength/Stress
			if (m_nLcomType == D_LCOMTYPE_CONCRETE)
			{
				if (nType == 10) LcomD.nActive = 8;  // ×¼ÓÀ¾Ã×éºÏ
				if (nType == 11) LcomD.nActive = 7;  // ÆµÓö×éºÏ
				if (nType == 12)
				{
					LcomD.bElasticStage = TRUE;
					LcomD.nActive = 2;
				}
			}
			else if (m_nLcomType == D_LCOMTYPE_STLCOMP || m_nLcomType == D_LCOMTYPE_SRC)
			{
				if (nType == 10) LcomD.nActive = 8;  //×¼ÓÀ¾Ã×éºÏ
				if (nType == 11) LcomD.nActive = 7;  //ÆµÓö×éºÏ
			}
			LoadCaseNo = GetCountLcomCase(LcomD);

			// moving load
			if (m_nMCount > 0)
			{
				Set_MovingCase(arMLoad.GetAt(m), dLFactor / (1. + dIL), LoadCaseNo, LcomD, TRUE);
				if (nType == 12 || dIL < m_dZeroLim) strDesc.Format(_T("+%sM"), GetFactorString(dLFactor));
				else                            strDesc.Format(_T("+%s/(1+mu)M"), GetFactorString(dLFactor));
				LcomD.Description += strDesc;
				if (m_nMCount > 1) { strDesc.Format(_T("[%d]"), m + 1); LcomD.Description += strDesc; }
			}
			if (m_nLCount > 0 || (bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(L)))) // ST or ST+CS
			{
				int nLL = 0, nIL = 0, nCF = 0;
				for (nc = 0; nc < m_nLCount; nc++) nLL += Set_STLCIncludeCS(arLLoad.GetAt(nc), dLFactor, LoadCaseNo, LcomD, 0);
				if (nLL > 0) { strDesc.Format(_T("+%sL"), GetFactorString(dLFactor)); LcomD.Description += strDesc; }
				if (bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(L)))
				{
					Set_CStageCase(m_aCSLoad.GetAt(nnn), dLFactor, LoadCaseNo, LcomD, TRUE);
					strDesc.Format(_T("+%s(cEL)"), GetFactorString(dLFactor)); LcomD.Description += strDesc;
				}
				for (nc = 0; nc < m_nIlCount; nc++) nIL += Set_STLCIncludeCS(arIlLoad.GetAt(nc), dLFactor, LoadCaseNo, LcomD, 0);
				if (nIL > 0) { strDesc.Format(_T("+%sIL"), GetFactorString(dLFactor)); LcomD.Description += strDesc; }
				for (nc = 0; nc < m_nCfCount; nc++) nCF += Set_STLCIncludeCS(arCfLoad.GetAt(nc), dLFactor, LoadCaseNo, LcomD, 0);
				if (nCF > 0) { strDesc.Format(_T("+%sCF"), GetFactorString(dLFactor)); LcomD.Description += strDesc; }
			}

			if (m_nLsCount > 0) // Live Load Surcharge µØÃæ¸½¼Ó»îºÉÔØ
			{
				T_LCOM_D LcomD_l = LcomD; LoadCaseNo = GetCountLcomCase(LcomD_l);
				int nLS = 0;
				for (nc = 0; nc < m_nLsCount; nc++) nLS += Set_STLCIncludeCS(arLsLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD_l, 0);
				if (nLS > 0)
				{
					strDesc.Format(_T("+%sLS"), GetFactorString(dFactor)); LcomD_l.Description += strDesc;
					SetCvlESCGB19_ETC_S(LcomD_l, LcomD_l, FALSE, arCrlLoad, arMCrlLoad, TRUE, arBrkLoad, FALSE, arWlLoad, FALSE, arSfLoad, FALSE, arIpLoad, FALSE,
						arTLoad, arTpgLoad, FALSE, arFrLoad, FALSE, 10, nMsgType);
				}
			}
		}
	}

	return TRUE;
}

BOOL CLoadCombCtrl::SetCvlESCGB19_ETC_S(T_LCOM_D rData, T_LCOM_D Lcom_cf, BOOL bCF, ArINT& arCrlLoad, ArINT& arMCrlLoad, BOOL bCrl, ArINT& arBrkLoad, BOOL bBrk,
	ArINT& arWlLoad, BOOL bWl, ArINT& arSfLoad, BOOL bSf, ArINT& arIpLoad, BOOL bIp, ArINT& arTLoad,
	ArINT& arTpgLoad, BOOL bT, ArINT& arFrLoad, BOOL bFr, int nType, int nMsgType)
{
	double dFactor = 1.0, dWlFactor = 1.0, dCRLFactor = 1.0, dTPGFactor = 1.0;
	if (nMsgType == 117) { dWlFactor = 1.0, dTPGFactor = 0.8, dCRLFactor = 0.4; } // ×¼ÓÀ¾Ã
	else if (nMsgType == 118) { dWlFactor = 1.0, dTPGFactor = 0.8, dCRLFactor = 0.4; }//modified by gongxing at 2016-01-22 // ÆµÓö

	int LoadCaseNo = 0, nc = 0, nnn = 0;
	CString strDesc = _T("");

	BOOL bErec = FALSE;
	for (int nErec = 0; nErec < D_STCT_EREC_MAX_CVL; ++nErec)
	{
		if (m_nConstLoad != 0 && CheckCS_SgldK(D_SGLD_ER1_CVL + nErec, nnn)) bErec = TRUE;
	}

	int nServEnvType = (nMsgType == 119) ? 1 : 0;//±ê×¼
	BOOL bAddCsLoad = bErec;
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER1_CVL,nnn)) || 
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER2_CVL,nnn)) || 
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER3_CVL,nnn)) ||
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER4_CVL,nnn)) ||
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER5_CVL,nnn)) ||
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER6_CVL,nnn)) ||
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER7_CVL,nnn)) ||
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER8_CVL,nnn)) ||
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER9_CVL,nnn)) ||
	// 		(m_nConstLoad!=0 && CheckCS_SgldK(D_SGLD_ER10_CVL,nnn));// CS or ST+CS;

	if ((m_nCrlCount > 0 || m_nMCrlCount > 0) && bCrl)
	{
		T_LCOM_D LcomD = rData;
		LoadCaseNo = GetCountLcomCase(LcomD);
		int nCRL = 0;
		for (nc = 0; nc < m_nCrlCount; nc++) nCRL += Set_STLCIncludeCS(arCrlLoad.GetAt(nc), dCRLFactor, LoadCaseNo, LcomD, 0);
		T_LCOM_D LcomD_tmp = Lcom_cf;
		if (bCF && bBrk)
		{
			LoadCaseNo = GetCountLcomCase(LcomD_tmp);
			for (nc = 0; nc < m_nCrlCount; nc++) Set_STLCIncludeCS(arCrlLoad.GetAt(nc), dCRLFactor, LoadCaseNo, LcomD_tmp, 0);
		}
		if (nCRL > 0)
		{
			strDesc.Format(_T("+%sCRL"), GetFactorString(dCRLFactor)); LcomD.Description += strDesc;
			if (bCF && bBrk) LcomD_tmp.Description += strDesc;
		}
		int nMCrlCount = (m_nMCrlCount > 0) ? m_nMCrlCount : ((nCRL > 0) ? 1 : 0);
		for (int crl = 0; crl < nMCrlCount; ++crl)
		{
			T_LCOM_D LcomD_crl = LcomD;
			T_LCOM_D LcomD_crltmp = LcomD_tmp;
			if (m_nMCrlCount > 0)
			{
				Set_MovingCase(arMCrlLoad.GetAt(crl), dCRLFactor, LoadCaseNo, LcomD_crl, TRUE);
				strDesc.Format(_T("+%sMCRL"), GetFactorString(dCRLFactor)); LcomD_crl.Description += strDesc; LcomD_crltmp.Description += strDesc;
				if (m_nMCrlCount > 1) { strDesc.Format(_T("[%d]"), crl + 1); LcomD_crl.Description += strDesc; LcomD_crltmp.Description += strDesc; }
			}
			if (nCRL > 0 || m_nMCrlCount > 0)
			{
				if (nType == 1) SetCvlESCGB19_ETC_S(LcomD_crl, LcomD_crltmp, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, bBrk, arWlLoad, TRUE, arSfLoad, TRUE, arIpLoad, TRUE,
					arTLoad, arTpgLoad, TRUE, arFrLoad, TRUE, 0, nMsgType);
				if (nType == 6) SetCvlESCGB19_ETC_S(LcomD_crl, LcomD_crltmp, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, bBrk, arWlLoad, TRUE, arSfLoad, TRUE, arIpLoad, TRUE,
					arTLoad, arTpgLoad, TRUE, arFrLoad, FALSE, 2, nMsgType);
				if (nType == 8) SetCvlESCGB19_ETC_S(LcomD_crl, LcomD_crltmp, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, bBrk, arWlLoad, TRUE, arSfLoad, TRUE, arIpLoad, TRUE,
					arTLoad, arTpgLoad, FALSE, arFrLoad, FALSE, 6, nMsgType);
				if (nType == 10) SetCvlESCGB19_ETC_S(LcomD_crl, LcomD_crltmp, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWlLoad, TRUE, arSfLoad, FALSE, arIpLoad, FALSE,
					arTLoad, arTpgLoad, FALSE, arFrLoad, FALSE, 8, nMsgType);
				if (nType == 101) SetCvlESCGB19_ETC_S(LcomD_crl, LcomD_crltmp, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, bBrk, arWlLoad, FALSE, arSfLoad, FALSE, arIpLoad, FALSE,
					arTLoad, arTpgLoad, FALSE, arFrLoad, FALSE, 102, nMsgType);
				if (nType == 103) SetCvlESCGB19_ETC_S(LcomD_crl, LcomD_crltmp, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, bBrk, arWlLoad, TRUE, arSfLoad, FALSE, arIpLoad, FALSE,
					arTLoad, arTpgLoad, FALSE, arFrLoad, FALSE, 104, nMsgType);

				if (nType == 0) Set_LoadComb4Civil(LcomD_crl, D_SER, nServEnvType, nMsgType);
			}
		}
	}
	if (nType == 8) bBrk = FALSE;
	if (m_nBrkCount > 0 && bBrk)
	{
		for (int i = 0; i < 2; ++i)
		{
			if (i == 1 && !bCF) break;
			T_LCOM_D LcomD = (i == 0) ? rData : Lcom_cf;
			LoadCaseNo = GetCountLcomCase(LcomD);
			double dBrkF = (i == 0) ? (bCF ? 0.7 : 1.0) : 1.0;
			int nBRK = 0;
			for (nc = 0; nc < m_nBrkCount; nc++) nBRK += Set_STLCIncludeCS(arBrkLoad.GetAt(nc), dBrkF, LoadCaseNo, LcomD, 0);
			if (nBRK > 0) { strDesc.Format(_T("+%sBRK"), GetFactorString(dBrkF)); LcomD.Description += strDesc; }
			if (nType == 1) SetCvlESCGB19_ETC_S(LcomD, Lcom_cf, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWlLoad, TRUE, arSfLoad, FALSE, arIpLoad, FALSE,
				arTLoad, arTpgLoad, TRUE, arFrLoad, FALSE, 0, nMsgType);
			if (nType == 2) SetCvlESCGB19_ETC_S(LcomD, Lcom_cf, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWlLoad, TRUE, arSfLoad, FALSE, arIpLoad, FALSE,
				arTLoad, arTpgLoad, TRUE, arFrLoad, FALSE, 0, nMsgType);
			if (nType == 6) SetCvlESCGB19_ETC_S(LcomD, Lcom_cf, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWlLoad, TRUE, arSfLoad, FALSE, arIpLoad, FALSE,
				arTLoad, arTpgLoad, FALSE, arFrLoad, FALSE, 3, nMsgType);
			if (nType == 7) SetCvlESCGB19_ETC_S(LcomD, Lcom_cf, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWlLoad, TRUE, arSfLoad, FALSE, arIpLoad, FALSE,
				arTLoad, arTpgLoad, FALSE, arFrLoad, FALSE, 3, nMsgType);
			if (nType == 8) SetCvlESCGB19_ETC_S(LcomD, Lcom_cf, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWlLoad, TRUE, arSfLoad, FALSE, arIpLoad, FALSE,
				arTLoad, arTpgLoad, FALSE, arFrLoad, FALSE, 3, nMsgType);
			if (nType == 101) SetCvlESCGB19_ETC_S(LcomD, Lcom_cf, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWlLoad, TRUE, arSfLoad, FALSE, arIpLoad, FALSE,
				arTLoad, arTpgLoad, FALSE, arFrLoad, FALSE, 3, nMsgType);
			if (nType == 102) SetCvlESCGB19_ETC_S(LcomD, Lcom_cf, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWlLoad, TRUE, arSfLoad, FALSE, arIpLoad, FALSE,
				arTLoad, arTpgLoad, TRUE, arFrLoad, FALSE, 0, nMsgType);
			if (nType == 103) SetCvlESCGB19_ETC_S(LcomD, Lcom_cf, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWlLoad, TRUE, arSfLoad, FALSE, arIpLoad, FALSE,
				arTLoad, arTpgLoad, FALSE, arFrLoad, FALSE, 3, nMsgType);
			if (nType == 104) SetCvlESCGB19_ETC_S(LcomD, Lcom_cf, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWlLoad, TRUE, arSfLoad, FALSE, arIpLoad, FALSE,
				arTLoad, arTpgLoad, FALSE, arFrLoad, FALSE, 3, nMsgType);

			if (nType == 0) Set_LoadComb4Civil(LcomD, D_SER, nServEnvType, nMsgType);
		}
	}
	if (bWl)
	{
		int nWlCount = m_nWlCount * 2; // MNET:1614, KJH20050808
		if (m_nWlCount == 0 && bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(WL))) nWlCount = 1;
		for (nc = 0; nc < nWlCount; nc++)
		{
			T_LCOM_D LcomD = rData;
			LoadCaseNo = GetCountLcomCase(LcomD);
			int nWL = 0;
			if (m_nWlCount > 0)
			{
				double dWF1 = (nc % 2 == 0) ? dWlFactor : -1.*dWlFactor;
				if (Set_STLCIncludeCS(arWlLoad.GetAt(nc / 2), dWF1, LoadCaseNo, LcomD, 0) > 0)
				{
					CString strW = (dWF1 > 0.0) ? _T("+%sWL") : _T("%sWL");
					strDesc.Format(strW, GetFactorString(dWF1)); LcomD.Description += strDesc;
					if (m_nWlCount > 1) { strDesc.Format(_T("[%d]"), nc / 2 + 1); LcomD.Description += strDesc; }
				}
			}
			if (bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(WL)))
			{
				Set_CStageCase(m_aCSLoad.GetAt(nnn), dWlFactor, LoadCaseNo, LcomD, TRUE);
				strDesc.Format(_T("+%s(cEL)"), GetFactorString(dWlFactor)); LcomD.Description += strDesc;
			}
			if (nType == 1) SetCvlESCGB19_ETC_S(LcomD, Lcom_cf, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWlLoad, FALSE, arSfLoad, TRUE, arIpLoad, TRUE,
				arTLoad, arTpgLoad, TRUE, arFrLoad, TRUE, 0, nMsgType);
			if (nType == 2) SetCvlESCGB19_ETC_S(LcomD, Lcom_cf, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWlLoad, FALSE, arSfLoad, TRUE, arIpLoad, TRUE,
				arTLoad, arTpgLoad, TRUE, arFrLoad, TRUE, 0, nMsgType);
			if (nType == 3) SetCvlESCGB19_ETC_S(LcomD, Lcom_cf, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWlLoad, FALSE, arSfLoad, FALSE, arIpLoad, FALSE,
				arTLoad, arTpgLoad, TRUE, arFrLoad, FALSE, 0, nMsgType);
			if (nType == 6) SetCvlESCGB19_ETC_S(LcomD, Lcom_cf, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWlLoad, FALSE, arSfLoad, TRUE, arIpLoad, TRUE,
				arTLoad, arTpgLoad, TRUE, arFrLoad, FALSE, 2, nMsgType);
			if (nType == 7) SetCvlESCGB19_ETC_S(LcomD, Lcom_cf, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWlLoad, FALSE, arSfLoad, TRUE, arIpLoad, TRUE,
				arTLoad, arTpgLoad, TRUE, arFrLoad, FALSE, 2, nMsgType);
			if (nType == 8) SetCvlESCGB19_ETC_S(LcomD, Lcom_cf, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWlLoad, FALSE, arSfLoad, TRUE, arIpLoad, TRUE,
				arTLoad, arTpgLoad, FALSE, arFrLoad, FALSE, 6, nMsgType);
			if (nType == 9) SetCvlESCGB19_ETC_S(LcomD, Lcom_cf, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWlLoad, FALSE, arSfLoad, TRUE, arIpLoad, TRUE,
				arTLoad, arTpgLoad, FALSE, arFrLoad, FALSE, 6, nMsgType);

			if (nType == 0) Set_LoadComb4Civil(LcomD, D_SER, nServEnvType, nMsgType);
		}
	}
	if (m_nSfCount > 0 && bSf)
	{
		T_LCOM_D LcomD = rData;
		LoadCaseNo = GetCountLcomCase(LcomD);
		int nSF = 0;
		for (nc = 0; nc < m_nSfCount; nc++) nSF += Set_STLCIncludeCS(arSfLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD, 0);
		if (nSF > 0) { strDesc.Format(_T("+%sSF"), GetFactorString(dFactor)); LcomD.Description += strDesc; }
		if (nType == 1) SetCvlESCGB19_ETC_S(LcomD, Lcom_cf, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWlLoad, FALSE, arSfLoad, FALSE, arIpLoad, FALSE,
			arTLoad, arTpgLoad, TRUE, arFrLoad, TRUE, 0, nMsgType);
		if (nType == 2) SetCvlESCGB19_ETC_S(LcomD, Lcom_cf, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWlLoad, FALSE, arSfLoad, FALSE, arIpLoad, FALSE,
			arTLoad, arTpgLoad, TRUE, arFrLoad, TRUE, 0, nMsgType);
		if (nType == 4) SetCvlESCGB19_ETC_S(LcomD, Lcom_cf, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWlLoad, FALSE, arSfLoad, FALSE, arIpLoad, FALSE,
			arTLoad, arTpgLoad, TRUE, arFrLoad, TRUE, 0, nMsgType);
		if (nType == 6) SetCvlESCGB19_ETC_S(LcomD, Lcom_cf, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWlLoad, FALSE, arSfLoad, FALSE, arIpLoad, FALSE,
			arTLoad, arTpgLoad, TRUE, arFrLoad, FALSE, 2, nMsgType);
		if (nType == 7) SetCvlESCGB19_ETC_S(LcomD, Lcom_cf, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWlLoad, FALSE, arSfLoad, FALSE, arIpLoad, FALSE,
			arTLoad, arTpgLoad, TRUE, arFrLoad, FALSE, 2, nMsgType);

		if (nType == 0) Set_LoadComb4Civil(LcomD, D_SER, nServEnvType, nMsgType);
	}
	if (m_nIpCount > 0 && bIp)
	{
		T_LCOM_D LcomD = rData;
		LoadCaseNo = GetCountLcomCase(LcomD);
		int nIP = 0;
		for (nc = 0; nc < m_nIpCount; nc++) nIP += Set_STLCIncludeCS(arIpLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD, 0);
		if (nIP > 0) { strDesc.Format(_T("+%sIP"), GetFactorString(dFactor)); LcomD.Description += strDesc; }
		if (nType == 1) SetCvlESCGB19_ETC_S(LcomD, Lcom_cf, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWlLoad, FALSE, arSfLoad, FALSE, arIpLoad, FALSE,
			arTLoad, arTpgLoad, TRUE, arFrLoad, TRUE, 0, nMsgType);
		if (nType == 2) SetCvlESCGB19_ETC_S(LcomD, Lcom_cf, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWlLoad, FALSE, arSfLoad, FALSE, arIpLoad, FALSE,
			arTLoad, arTpgLoad, TRUE, arFrLoad, TRUE, 0, nMsgType);
		if (nType == 4) SetCvlESCGB19_ETC_S(LcomD, Lcom_cf, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWlLoad, FALSE, arSfLoad, FALSE, arIpLoad, FALSE,
			arTLoad, arTpgLoad, TRUE, arFrLoad, TRUE, 0, nMsgType);
		if (nType == 6) SetCvlESCGB19_ETC_S(LcomD, Lcom_cf, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWlLoad, FALSE, arSfLoad, FALSE, arIpLoad, FALSE,
			arTLoad, arTpgLoad, TRUE, arFrLoad, FALSE, 2, nMsgType);
		if (nType == 7) SetCvlESCGB19_ETC_S(LcomD, Lcom_cf, bCF, arCrlLoad, arMCrlLoad, FALSE, arBrkLoad, FALSE, arWlLoad, FALSE, arSfLoad, FALSE, arIpLoad, FALSE,
			arTLoad, arTpgLoad, TRUE, arFrLoad, FALSE, 2, nMsgType);

		if (nType == 0) Set_LoadComb4Civil(LcomD, D_SER, nServEnvType, nMsgType);
	}
	if (bT && ((m_nTCount > 0 || m_nTpgCount > 0) || (bAddCsLoad && (CheckCSEL(D_SGLD_ER1_CVL, _LSX(T)) || CheckCSEL(D_SGLD_ER1_CVL, _LSX(TPG))))))
	{
		T_LCOM_D LcomD = rData;
		if (nType == 0) SetCvlJTJD60_Capacity_TPG(LcomD, arTLoad, arTpgLoad, arFrLoad, dFactor, dTPGFactor, FALSE, FALSE, nMsgType, D_SER, nServEnvType);
		else         SetCvlJTJD60_Capacity_TPG(LcomD, arTLoad, arTpgLoad, arFrLoad, dFactor, dTPGFactor, TRUE, FALSE, nMsgType, D_SER, nServEnvType);
	}
	if (m_nFrCount > 0 && bFr)
	{
		T_LCOM_D LcomD = rData;
		LoadCaseNo = GetCountLcomCase(LcomD);
		int nFR = 0;
		for (nc = 0; nc < m_nFrCount; nc++) nFR += Set_STLCIncludeCS(arFrLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD, 0);
		if (nFR > 0)
		{
			strDesc.Format(_T("+%sFR"), GetFactorString(dFactor)); LcomD.Description += strDesc;
			if (nType == 0) Set_LoadComb4Civil(LcomD, D_SER, nServEnvType, nMsgType);
		}
	}
	return TRUE;
}

BOOL CLoadCombCtrl::SetCvlESCGB19_Var1_A(const CArray<T_LCOM_D, T_LCOM_D&>& arData, ArINT& arLLoad,
	ArINT& arIlLoad, ArINT& arCfLoad, ArINT& arLsLoad, ArINT& arCrlLoad, ArINT& arWLoad,
	ArINT& arSfLoad, ArINT& arIpLoad, ArINT& arTLoad, ArINT& arTpgLoad, ArINT& arFrLoad,
	ArINT& arBrkLoad, ArUNT& arMLoad, ArINT& arMcrLoad, int nType, int nMsgType, int nAcdType)
{
	// nType == 0,4: °è¼öÁ¶ÇÕ, 1: 1.0(settlement Á¦¿Ü), 2: 1.0(¸ðµÎ), Live Load Á¦¿Ü
	CArray<T_LCOM_D, T_LCOM_D&> aLcomD; aLcomD.Copy(arData);
	int  nLcomSize = aLcomD.GetSize();
	int  nnn = 0;
	BOOL bAddCsLoad = m_nConstLoad != 0 && CheckCS_SgldK(D_SGLD_ER1_CVL, nnn); // CS or ST+CS

	double dFactor = 1.0;
	double dMLFactor = 1.0;       // Æû³µºÉÔØÏµÊý
	double dCrlFactor = 1.0;      // ÈËÈººÉÔØÏµÊý
	double dWFactor = 0.75;       // ·çºÉÔØÏµÊý
	double dTgpFactor = 0.8;      //ÎÂ¶ÈÌÝ¶ÈºÉÔØÏµÊý
	if (nAcdType == 0)//ÆµÓöÏµÊý modified this statement by gongxing at 2016-04-13
	{
		dMLFactor = 0.7;
		dCrlFactor = 1.0;
	}
	else if (nAcdType == 1)//×¼ÓÀ¾ÃÏµÊý
	{
		dMLFactor = 0.4;
		dCrlFactor = 0.4;
	}
	double dIL = (nType == 12) ? 0. : CalcMovingILFactor();

	// ¿µ±¸ÇÏÁß + Â÷·®ÇÏÁß + 1°³ÀÇ °¡º¯ ÇÏÁßÀÎ °æ¿ì
	CString strDesc = _T("");
	int LoadCaseNo = 0, nc = 0;
	for (int lc = 0; lc < nLcomSize; lc++)
	{
		T_LCOM_D LcomD = aLcomD.GetAt(lc);
		LcomD.nActive = 6; // Active or Strength/Stress
		if (m_nLcomType == D_LCOMTYPE_GENERAL) LcomD.nActive = 1;  // general
		LoadCaseNo = GetCountLcomCase(LcomD);

		int nMCount = max(m_nMCount + 1, 1);
		for (int m = 0; m < nMCount; m++)
		{
			if (m < m_nMCount)
			{

				Set_MovingCase(arMLoad.GetAt(m), dMLFactor, LoadCaseNo, LcomD, TRUE);  //????
				strDesc.Format(_T("+%sM"), GetFactorString(dMLFactor)); LcomD.Description += strDesc;
				if (m_nMCount > 1) { strDesc.Format(_T("[%d]"), m + 1); LcomD.Description += strDesc; }
				if (nType == 2 || nType == 4) m_aAcdnLcomD.Add(LcomD);
				else                     Set_LoadComb4Civil(LcomD, D_STR, 0, nMsgType);
			}
			int nMcrCount = max(m_nMCrlCount + 1, 1);
			for (int nMcr = 0; nMcr < nMcrCount; ++nMcr)
			{
				if (m < m_nMCount && nMcr < m_nMCrlCount)
				{
					Set_MovingCase(arMcrLoad.GetAt(nMcr), dCrlFactor, LoadCaseNo, LcomD, TRUE);
					strDesc.Format(_T("+%sMcr"), GetFactorString(dCrlFactor));LcomD.Description += strDesc;
					if (m_nMCrlCount > 1) { strDesc.Format(_T("[%d]"), nMcr + 1); LcomD.Description += strDesc; }
					if (nType == 2 || nType == 4) m_aAcdnLcomD.Add(LcomD);
					else                     Set_LoadComb4Civil(LcomD, D_STR, 0, nMsgType);
				}

				BOOL bCF = FALSE;
				T_LCOM_D LcomD_cf = LcomD; // MNET:1614, KJH20050809, CF°¡ Á¦¿ÜµÈ ÇÏÁßÁ¶ÇÕ
				if (nType != 2)
				{
					BOOL bAddLcomD = FALSE;
					if (m_nLCount > 0 || (bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(L)))) // ST or ST+CS
					{
						int nLL = 0, nIL = 0, nCF = 0;
						for (nc = 0; nc < m_nLCount; nc++) nLL += Set_STLCIncludeCS(arLLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD, 0);
						if (nLL > 0) { strDesc.Format(_T("+%sL"), GetFactorString(dFactor)); LcomD_cf.Description += strDesc; }
						if (bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(L)))
						{
							Set_CStageCase(m_aCSLoad.GetAt(nnn), dFactor, LoadCaseNo, LcomD, TRUE);
							strDesc.Format(_T("+%s(cEL)"), GetFactorString(dFactor)); LcomD_cf.Description += strDesc;
							bAddLcomD = TRUE;
						}
						for (nc = 0; nc < m_nIlCount; nc++) nIL += Set_STLCIncludeCS(arIlLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD, 0);
						if (nIL > 0) { strDesc.Format(_T("+%sIL"), GetFactorString(dFactor));  LcomD_cf.Description += strDesc; }

						LcomD_cf = LcomD;
						for (nc = 0; nc < m_nCfCount; nc++) nCF += Set_STLCIncludeCS(arCfLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD, 0);
						if (nCF > 0) { strDesc.Format(_T("+%sCF"), GetFactorString(dFactor));  LcomD_cf.Description += strDesc; bCF = TRUE; }
						if (nLL + nIL + nCF > 0) bAddLcomD = TRUE;
					}
					if (bAddLcomD)
					{
						if (nType == 2 || nType == 4) m_aAcdnLcomD.Add(LcomD);
						else                     Set_LoadComb4Civil(LcomD, D_STR, 0, nMsgType);
					}
				}

				if (m_nLsCount > 0) // Live Load Surcharge
				{
					T_LCOM_D LcomD_l = LcomD; LoadCaseNo = GetCountLcomCase(LcomD_l);
					int nLS = 0;
					for (nc = 0; nc < m_nLsCount; nc++) nLS += Set_STLCIncludeCS(arLsLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD_l, 0);
					if (nLS > 0)
					{
						strDesc.Format(_T("+%sLS"), GetFactorString(dFactor));  LcomD_l.Description += strDesc;
						if (nType == 2 || nType == 4) m_aAcdnLcomD.Add(LcomD_l);
						else                     Set_LoadComb4Civil(LcomD_l, D_STR, 0, nMsgType);
					}
				}
				if (nType != 2 && m_nCrlCount > 0) // Crowd Load
				{
					T_LCOM_D LcomD_l = LcomD; LoadCaseNo = GetCountLcomCase(LcomD_l);
					int nCRL = 0;
					for (nc = 0; nc < m_nCrlCount; nc++) nCRL += Set_STLCIncludeCS(arCrlLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD_l, 0);
					if (nCRL > 0)
					{
						strDesc.Format(_T("+%sCRL"), GetFactorString(dFactor));  LcomD_l.Description += strDesc;
						if (nType == 2 || nType == 4) m_aAcdnLcomD.Add(LcomD_l);
						else                     Set_LoadComb4Civil(LcomD_l, D_STR, 0, nMsgType);
					}
				}
				if (nType != 2 && m_nBrkCount > 0 && m_nLCount > 0) // Brake Load
				{
					T_LCOM_D LcomD_l = LcomD; LoadCaseNo = GetCountLcomCase(LcomD_l);
					double dBrkF = (nType == 0 || nType == 4) ? dFactor * 0.7 : 1.0;
					int nBRK = 0;
					for (nc = 0; nc < m_nBrkCount; nc++) nBRK += Set_STLCIncludeCS(arBrkLoad.GetAt(nc), dBrkF, LoadCaseNo, LcomD_l, 0);
					if (nBRK > 0)
					{
						strDesc.Format(_T("+%sBRK"), GetFactorString(dBrkF));  LcomD_l.Description += strDesc;
						if (nType == 2 || nType == 4) m_aAcdnLcomD.Add(LcomD_l);
						else                     Set_LoadComb4Civil(LcomD_l, D_STR, 0, nMsgType);
					}
					if (bCF)
					{
						LcomD_l = LcomD_cf; LoadCaseNo = GetCountLcomCase(LcomD_l);
						double dBrkF = (nType == 0 || nType == 4) ? dFactor : 1.0;
						nBRK = 0;
						for (nc = 0; nc < m_nBrkCount; nc++) nBRK += Set_STLCIncludeCS(arBrkLoad.GetAt(nc), dBrkF, LoadCaseNo, LcomD_l, 0);
						if (nBRK > 0)
						{
							strDesc.Format(_T("+%sBRK"), GetFactorString(dBrkF));  LcomD_l.Description += strDesc;
							if (nType == 2 || nType == 4) m_aAcdnLcomD.Add(LcomD_l);
							else                     Set_LoadComb4Civil(LcomD_l, D_STR, 0, nMsgType);
						}
					}
				}
				int nWCount = m_nWCount * 2; // MNET:1614, KJH20050808
				if (m_nWCount == 0 && bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(W))) nWCount = 1;
				for (nc = 0; nc < nWCount; nc++)
				{
					T_LCOM_D LcomD_l = LcomD; LoadCaseNo = GetCountLcomCase(LcomD_l);
					if (m_nWCount > 0)
					{
						double dWF = (nc % 2 == 0) ? dWFactor : -1.*dWFactor;
						if (Set_STLCIncludeCS(arWLoad.GetAt(nc / 2), dWF, LoadCaseNo, LcomD_l, 0) > 0)
						{
							CString strW = (dWF > 0.0) ? _T("+%sW") : _T("%sW");
							strDesc.Format(strW, GetFactorString(dWF)); LcomD_l.Description += strDesc;
							if (m_nWCount > 1) { strDesc.Format(_T("[%d]"), nc / 2 + 1);  LcomD_l.Description += strDesc; }
						}
					}
					if (bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(W)))
					{
						Set_CStageCase(m_aCSLoad.GetAt(nnn), dWFactor, LoadCaseNo, LcomD_l, TRUE);
						strDesc.Format(_T("+%s(cEL)"), GetFactorString(dWFactor)); LcomD_l.Description += strDesc;
					}
					if (nType == 2 || nType == 4) m_aAcdnLcomD.Add(LcomD_l);
					else                     Set_LoadComb4Civil(LcomD_l, D_STR, 0, nMsgType);
				}
				if (m_nSfCount > 0) // Stream Flow Pressure
				{
					T_LCOM_D LcomD_l = LcomD; LoadCaseNo = GetCountLcomCase(LcomD_l);
					int nSF = 0;
					for (nc = 0; nc < m_nSfCount; nc++) nSF += Set_STLCIncludeCS(arSfLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD_l, 0);
					if (nSF > 0)
					{
						strDesc.Format(_T("+%sSF"), GetFactorString(dFactor));  LcomD_l.Description += strDesc;
						if (nType == 2 || nType == 4) m_aAcdnLcomD.Add(LcomD_l);
						else                     Set_LoadComb4Civil(LcomD_l, D_STR, 0, nMsgType);
					}
				}
				if (m_nIpCount > 0) // Ice Pressure
				{
					T_LCOM_D LcomD_l = LcomD; LoadCaseNo = GetCountLcomCase(LcomD_l);
					int nIP = 0;
					for (nc = 0; nc < m_nIpCount; nc++) nIP += Set_STLCIncludeCS(arIpLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD_l, 0);
					if (nIP > 0)
					{
						strDesc.Format(_T("+%sIP"), GetFactorString(dFactor)); LcomD_l.Description += strDesc;
						if (nType == 2 || nType == 4) m_aAcdnLcomD.Add(LcomD_l);
						else                     Set_LoadComb4Civil(LcomD_l, D_STR, 0, nMsgType);
					}
				}
				// Temeperature
				if ((m_nTCount > 0 || m_nTpgCount > 0) || (bAddCsLoad && (CheckCSEL(D_SGLD_ER1_CVL, _LSX(T)) || CheckCSEL(D_SGLD_ER1_CVL, _LSX(TPG)))))
				{
					T_LCOM_D LcomD_l = LcomD;
					SetCvlJTJD60_Capacity_TPG(LcomD_l, arTLoad, arTpgLoad, arFrLoad, dFactor, dTgpFactor, FALSE, nType == 2 || nType == 4, nMsgType);
				}
				if (m_nFrCount > 0) // Friction
				{
					T_LCOM_D LcomD_l = LcomD; LoadCaseNo = GetCountLcomCase(LcomD_l);
					int nFR = 0;
					for (nc = 0; nc < m_nFrCount; nc++) nFR += Set_STLCIncludeCS(arFrLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD_l, 0);
					if (nFR > 0)
					{
						strDesc.Format(_T("+%sFR"), GetFactorString(dFactor));  LcomD_l.Description += strDesc;
						if (nType == 2 || nType == 4) m_aAcdnLcomD.Add(LcomD_l);
						else                     Set_LoadComb4Civil(LcomD_l, D_STR, 0, nMsgType);
					}
				}
			}
		}
	}

	return TRUE;
}
BOOL CLoadCombCtrl::SetCvlESCGB19_Var2_A(const CArray<T_LCOM_D, T_LCOM_D&>& arData, ArINT& arLLoad,
	ArINT& arIlLoad, ArINT& arCfLoad, ArINT& arLsLoad, ArINT& arCrlLoad, ArINT& arWLoad,
	ArINT& arSfLoad, ArINT& arIpLoad, ArINT& arTLoad, ArINT& arTpgLoad, ArINT& arFrLoad,
	ArINT& arBrkLoad, ArUNT& arMLoad, ArINT& arMcrLoad, int nType, int nMsgType, int nAcdType)
{
	// nType == 0,4: °è¼öÁ¶ÇÕ, 1: 1.0(settlement Á¦¿Ü), 2: 1.0(¸ðµÎ), Live Load Á¦¿Ü
	CArray<T_LCOM_D, T_LCOM_D&>aLcomD; aLcomD.Copy(arData);
	int  nLcomSize = aLcomD.GetSize();
	int  nnn = 0;

	BOOL bAddCsLoad = m_nConstLoad != 0 && CheckCS_SgldK(D_SGLD_ER1_CVL, nnn); // CS or ST+CS

	double dFactor = 1.0;
	double dMLFactor = 1.0;       // Æû³µºÉÔØÏµÊý
	double dCrlFactor = 1.0;      // ÈËÈººÉÔØÏµÊý
	double dWFactor = 0.75;       // ·çºÉÔØÏµÊý
	double dTgpFactor = 0.8;      //ÎÂ¶ÈÌÝ¶ÈºÉÔØÏµÊý
	if (nAcdType == 0)//ÆµÓöÏµÊý modified this statement by gongxing at 2016-04-13
	{
		dMLFactor = 0.7;
		dCrlFactor = 1.0;
	}
	else if (nAcdType == 1)//×¼ÓÀ¾ÃÏµÊý
	{
		dMLFactor = 0.4;
		dCrlFactor = 0.4;
	}
	double dF = (nType == 0 || nType == 4) ? 0.75 : 1.0;
	double dIL = (nType == 12) ? 0. : CalcMovingILFactor();

	// ¿µ±¸ÇÏÁß + Â÷·®ÇÏÁß + 2°³ÀÇ °¡º¯ÇÏÁßÀÎ °æ¿ì
	CString strDesc = _T("");
	int LoadCaseNo = 0, nc = 0;
	for (int lc = 0; lc < nLcomSize; lc++)
	{
		T_LCOM_D LcomD = aLcomD.GetAt(lc);
		LcomD.nActive = 6; // Active or Strength/Stress
		if (m_nLcomType == D_LCOMTYPE_GENERAL) LcomD.nActive = 1;  // general
		LoadCaseNo = GetCountLcomCase(LcomD);

		int nMCount = max(m_nMCount + 1, 1);
		for (int m = 0; m < nMCount; m++)
		{
			if (m < m_nMCount)
			{

				Set_MovingCase(arMLoad.GetAt(m), dMLFactor, LoadCaseNo, LcomD, TRUE);  //????
				strDesc.Format(_T("+%sM"), GetFactorString(dMLFactor)); LcomD.Description += strDesc;
				if (m_nMCount > 1) { strDesc.Format(_T("[%d]"), m + 1); LcomD.Description += strDesc; }
				if (nType == 2 || nType == 4) m_aAcdnLcomD.Add(LcomD);
				else                     Set_LoadComb4Civil(LcomD, D_STR, 0, nMsgType);
			}

			int nMcrCount = max(m_nMCrlCount + 1, 1);
			for (int nMcr = 0; nMcr < nMcrCount; ++nMcr)
			{
				if (m < m_nMCount && nMcr < m_nMCrlCount)
				{
					Set_MovingCase(arMcrLoad.GetAt(nMcr), dCrlFactor, LoadCaseNo, LcomD, TRUE);
					strDesc.Format(_T("+%sMcr"), GetFactorString(dCrlFactor));LcomD.Description += strDesc;;
					if (m_nMCrlCount > 1) { strDesc.Format(_T("[%d]"), nMcr + 1); LcomD.Description += strDesc; }
					if (nType == 2 || nType == 4) m_aAcdnLcomD.Add(LcomD);
					else                     Set_LoadComb4Civil(LcomD, D_STR, 0, nMsgType);
				}

				T_LCOM_D LcomD_cf = LcomD; // MNET:1614, KJH20050809, CF°¡ Á¦¿ÜµÈ ÇÏÁßÁ¶ÇÕ
				BOOL bLive = FALSE, bCF = FALSE;
				if (nType != 2)
				{
					bLive = m_nLCount > 0 || (bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(L)));
					if (bLive) // ST or ST+CS
					{
						int nLL = 0, nIL = 0, nCF = 0;
						for (nc = 0; nc < m_nLCount; nc++) nLL += Set_STLCIncludeCS(arLLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD, 0);
						if (nLL > 0) { strDesc.Format(_T("+%sL"), GetFactorString(dFactor)); LcomD.Description += strDesc; }
						if (bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(L)))
						{
							Set_CStageCase(m_aCSLoad.GetAt(nnn), dFactor, LoadCaseNo, LcomD, TRUE);
							strDesc.Format(_T("+%s(cEL)"), GetFactorString(dFactor)); LcomD.Description += strDesc;;
						}
						for (nc = 0; nc < m_nIlCount; nc++) nIL += Set_STLCIncludeCS(arIlLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD, 0);
						if (nIL > 0) { strDesc.Format(_T("+%sIL"), GetFactorString(dFactor)); LcomD.Description += strDesc; }

						LcomD_cf = LcomD;
						for (nc = 0; nc < m_nCfCount; nc++) nCF += Set_STLCIncludeCS(arCfLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD, 0);
						if (nCF > 0) { strDesc.Format(_T("+%sCF"), GetFactorString(dFactor)); LcomD.Description += strDesc; bCF = TRUE; }
					}
				}

				if (m_nLsCount > 0) // Live Load Surcharge
				{
					T_LCOM_D LcomD_l = LcomD; LoadCaseNo = GetCountLcomCase(LcomD_l);
					int nLS = 0;
					for (nc = 0; nc < m_nLsCount; nc++) nLS += Set_STLCIncludeCS(arLsLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD_l, 0);
					T_LCOM_D LcomD_tmp = LcomD_cf;
					if (bCF)
					{
						LoadCaseNo = GetCountLcomCase(LcomD_tmp);
						for (nc = 0; nc < m_nLsCount; nc++) Set_STLCIncludeCS(arLsLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD_tmp, 0);
					}
					if (nLS > 0)
					{
						strDesc.Format(_T("+%sLS"), GetFactorString(dFactor)); LcomD_l.Description += strDesc;
						if (bCF) LcomD_tmp.Description += strDesc;
						ArINT artmp;
						SetCvlESCGB19_ETC(LcomD_l, LcomD_tmp, bCF, arCrlLoad, artmp, nType != 2, arBrkLoad, bLive, arWLoad, TRUE, arSfLoad, TRUE,
							arIpLoad, TRUE, arTLoad, arTpgLoad, TRUE, arFrLoad, TRUE, 0, dF, nType == 2 || nType == 4, nMsgType);
					}
				}
				if (nType != 2 && m_nCrlCount > 0) // Crowd Load
				{
					T_LCOM_D LcomD_l = LcomD; LoadCaseNo = GetCountLcomCase(LcomD_l);
					int nCRL = 0;
					for (nc = 0; nc < m_nCrlCount; nc++) nCRL += Set_STLCIncludeCS(arCrlLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD_l, 0);
					T_LCOM_D LcomD_tmp = LcomD_cf;
					if (bCF)
					{
						LoadCaseNo = GetCountLcomCase(LcomD_tmp);
						for (nc = 0; nc < m_nCrlCount; nc++) Set_STLCIncludeCS(arCrlLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD_tmp, 0);
					}
					if (nCRL > 0)
					{
						strDesc.Format(_T("+%sCRL"), GetFactorString(dFactor)); LcomD_l.Description += strDesc;
						if (bCF) LcomD_tmp.Description += strDesc;
						ArINT artmp;
						SetCvlESCGB19_ETC(LcomD_l, LcomD_tmp, bCF, arCrlLoad, artmp, FALSE, arBrkLoad, m_nLCount > 0, arWLoad, TRUE, arSfLoad, TRUE, arIpLoad, TRUE,
							arTLoad, arTpgLoad, TRUE, arFrLoad, TRUE, 0, dF, nType == 2 || nType == 4, nMsgType);
					}
				}
				if (m_nBrkCount > 0 && bLive) // Brake Load
				{
					T_LCOM_D LcomD_l = LcomD; LoadCaseNo = GetCountLcomCase(LcomD_l);
					double dBrkF = (nType == 0 || nType == 4) ? dFactor * 0.7 : 1.0;
					int nBRK = 0;
					for (nc = 0; nc < m_nBrkCount; nc++) nBRK += Set_STLCIncludeCS(arBrkLoad.GetAt(nc), dBrkF, LoadCaseNo, LcomD_l, 0);
					if (nBRK > 0)
					{
						strDesc.Format(_T("+%sBRK"), GetFactorString(dBrkF)); LcomD_l.Description += strDesc;
						ArINT artmp;
						SetCvlESCGB19_ETC(LcomD_l, LcomD_cf, FALSE, arCrlLoad, artmp, FALSE, arBrkLoad, FALSE, arWLoad, TRUE, arSfLoad, FALSE, arIpLoad, FALSE,
							arTLoad, arTpgLoad, TRUE, arFrLoad, FALSE, 0, dF, nType == 2 || nType == 4, nMsgType);
					}
					if (bCF) // MNET:1614, KJH20050809, CF°¡ ¾ø´Â ÇÏÁßÁ¶ÇÕ ÇÑ¹ø´õ...
					{
						LcomD_l = LcomD_cf; LoadCaseNo = GetCountLcomCase(LcomD_l);
						dBrkF = (nType == 0 || nType == 4) ? dFactor : 1.0;
						nBRK = 0;
						for (nc = 0; nc < m_nBrkCount; nc++) nBRK += Set_STLCIncludeCS(arBrkLoad.GetAt(nc), dBrkF, LoadCaseNo, LcomD_l, 0);
						if (nBRK > 0)
						{
							strDesc.Format(_T("+%sBRK"), GetFactorString(dBrkF)); LcomD_l.Description += strDesc;
							ArINT artmp;
							SetCvlESCGB19_ETC(LcomD_l, LcomD_cf, FALSE, arCrlLoad, artmp, FALSE, arBrkLoad, FALSE, arWLoad, TRUE, arSfLoad, FALSE, arIpLoad, FALSE,
								arTLoad, arTpgLoad, TRUE, arFrLoad, FALSE, 0, dF, nType == 2 || nType == 4, nMsgType);
						}
					}
				}
				int nWCount = m_nWCount * 2; // MNET:1614, KJH20050808
				if (m_nWCount == 0 && bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(W))) nWCount = 1;
				for (nc = 0; nc < nWCount; nc++)
				{
					T_LCOM_D LcomD_l = LcomD; LoadCaseNo = GetCountLcomCase(LcomD_l);
					if (m_nWCount > 0)
					{
						double dWF = (nc % 2 == 0) ? dWFactor : -1.*dWFactor;
						if (Set_STLCIncludeCS(arWLoad.GetAt(nc / 2), dWF, LoadCaseNo, LcomD_l, 0) > 0)
						{
							CString strW = (dWF > 0.0) ? _T("+%sW") : _T("%sW");
							strDesc.Format(strW, GetFactorString(dWF)); LcomD_l.Description += strDesc;
							if (m_nWCount > 1) { strDesc.Format(_T("[%d]"), nc / 2 + 1); LcomD_l.Description += strDesc; }
						}
					}
					if (bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(W)))
					{
						Set_CStageCase(m_aCSLoad.GetAt(nnn), dWFactor, LoadCaseNo, LcomD_l, TRUE);
						strDesc.Format(_T("+%s(cEL)"), GetFactorString(dWFactor)); LcomD_l.Description += strDesc;
					}
					ArINT artmp;
					SetCvlESCGB19_ETC(LcomD_l, LcomD_cf, bCF, arCrlLoad, artmp, FALSE, arBrkLoad, FALSE, arWLoad, FALSE, arSfLoad, TRUE, arIpLoad, TRUE,
						arTLoad, arTpgLoad, TRUE, arFrLoad, TRUE, 0, dF, nType == 2 || nType == 4, nMsgType);
				}
				if (m_nSfCount > 0) // Stream Flow Pressure
				{
					T_LCOM_D LcomD_l = LcomD; LoadCaseNo = GetCountLcomCase(LcomD_l);
					int nSF = 0;
					for (nc = 0; nc < m_nSfCount; nc++) nSF += Set_STLCIncludeCS(arSfLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD_l, 0);
					if (nSF > 0)
					{
						ArINT artmp;
						strDesc.Format(_T("+%sSF"), GetFactorString(dFactor)); LcomD_l.Description += strDesc;
						SetCvlESCGB19_ETC(LcomD_l, LcomD_cf, bCF, arCrlLoad, artmp, FALSE, arBrkLoad, FALSE, arWLoad, FALSE, arSfLoad, FALSE, arIpLoad, FALSE,
							arTLoad, arTpgLoad, TRUE, arFrLoad, TRUE, 0, dF, nType == 2 || nType == 4, nMsgType);
					}
				}
				if (m_nIpCount > 0) // Ice Pressure
				{
					T_LCOM_D LcomD_l = LcomD; LoadCaseNo = GetCountLcomCase(LcomD_l);
					int nIP = 0;
					for (nc = 0; nc < m_nIpCount; nc++) nIP += Set_STLCIncludeCS(arIpLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD_l, 0);
					if (nIP > 0)
					{
						strDesc.Format(_T("+%sIP"), GetFactorString(dFactor)); LcomD_l.Description += strDesc;
						ArINT artmp;
						SetCvlESCGB19_ETC(LcomD_l, LcomD_cf, bCF, arCrlLoad, artmp, FALSE, arBrkLoad, FALSE, arWLoad, FALSE, arSfLoad, FALSE, arIpLoad, FALSE,
							arTLoad, arTpgLoad, TRUE, arFrLoad, TRUE, 0, dF, nType == 2 || nType == 4, nMsgType);
					}
				}
				if ((m_nTCount > 0 || m_nTpgCount > 0) || (bAddCsLoad && (CheckCSEL(D_SGLD_ER1_CVL, _LSX(T)) || CheckCSEL(D_SGLD_ER1_CVL, _LSX(TPG))))) // Temeperature
				{
					T_LCOM_D LcomD_l = LcomD;
					SetCvlJTJD60_Capacity_TPG(LcomD_l, arTLoad, arTpgLoad, arFrLoad, dFactor, dTgpFactor, TRUE, nType == 2 || nType == 4, nMsgType);
				}
			}
		}
	}

	return TRUE;
}
BOOL CLoadCombCtrl::SetCvlESCGB19_Var3_A(const CArray<T_LCOM_D, T_LCOM_D&>& arData, ArINT& arLLoad,
	ArINT& arIlLoad, ArINT& arCfLoad, ArINT& arLsLoad, ArINT& arCrlLoad, ArINT& arWLoad,
	ArINT& arSfLoad, ArINT& arIpLoad, ArINT& arTLoad, ArINT& arTpgLoad, ArINT& arFrLoad,
	ArINT& arBrkLoad, ArUNT& arMLoad, ArINT& arMcrLoad, int nType, int nMsgType, int nAcdType)
{
	// nType == 0,4: °è¼öÁ¶ÇÕ, 1: 1.0(settlement Á¦¿Ü), 2: 1.0(¸ðµÎ), Live Load Á¦¿Ü
	CArray<T_LCOM_D, T_LCOM_D&> aLcomD; aLcomD.Copy(arData);
	int nLcomSize = aLcomD.GetSize();
	int  nnn = 0;
	BOOL bAddCsLoad = m_nConstLoad != 0 && CheckCS_SgldK(D_SGLD_ER1_CVL, nnn); // CS or ST+CS

	double dFactor = 1.0;
	double dMLFactor = 1.0;     // ???äü??
	double dCrlFactor = 1.0;    // ???äü??
	double dWFactor = 0.75;      // ??äü??
	double dTgpFactor = 0.8;    // ???äü??
	if (nAcdType == 0)//ÆµÓöÏµÊý modified this statement by gongxing at 2016-04-13
	{
		dMLFactor = 0.7;
		dCrlFactor = 1.0;
	}
	else if (nAcdType == 1)//×¼ÓÀ¾ÃÏµÊý
	{
		dMLFactor = 0.4;
		dCrlFactor = 0.4;
	}

	double dF = (nType == 0 || nType == 4) ? 0.75 : 1.0;
	double dIL = (nType == 12) ? 0. : CalcMovingILFactor();

	// ¿µ±¸ÇÏÁß + Â÷·®ÇÏÁß + 3°³ÀÇ °¡º¯ÇÏÁßÀÎ °æ¿ì
	CString strDesc = _T("");
	int LoadCaseNo = 0, nc = 0;
	for (int lc = 0; lc < nLcomSize; lc++)
	{
		T_LCOM_D LcomD = aLcomD.GetAt(lc);
		LcomD.nActive = 6; // Active or Strength/Stress
		if (m_nLcomType == D_LCOMTYPE_GENERAL) LcomD.nActive = 1;  // general
		LoadCaseNo = GetCountLcomCase(LcomD);

		int nMCount = max(m_nMCount + 1, 1);
		for (int m = 0; m < nMCount; m++)
		{
			if (m < m_nMCount)
			{

				Set_MovingCase(arMLoad.GetAt(m), dMLFactor, LoadCaseNo, LcomD, TRUE);  //????
				strDesc.Format(_T("+%sM"), GetFactorString(dMLFactor)); LcomD.Description = LcomD.Description + strDesc;
				if (m_nMCount > 1) { strDesc.Format(_T("[%d]"), m + 1); LcomD.Description = LcomD.Description + strDesc; }
				if (nType == 2 || nType == 4) m_aAcdnLcomD.Add(LcomD);
				else                     Set_LoadComb4Civil(LcomD, D_STR, 0, nMsgType);
			}
			int nMcrCount = max(m_nMCrlCount + 1, 1);
			for (int nMcr = 0; nMcr < nMcrCount; ++nMcr)
			{
				if (m < m_nMCount && nMcr < m_nMCrlCount)
				{
					Set_MovingCase(arMcrLoad.GetAt(nMcr), dCrlFactor, LoadCaseNo, LcomD, TRUE);
					strDesc.Format(_T("+%sMcr"), GetFactorString(dCrlFactor)); LcomD.Description = LcomD.Description + strDesc;
					if (m_nMCrlCount > 1) { strDesc.Format(_T("[%d]"), nMcr + 1); LcomD.Description = LcomD.Description + strDesc; }
					if (nType == 2 || nType == 4) m_aAcdnLcomD.Add(LcomD);
					else                     Set_LoadComb4Civil(LcomD, D_STR, 0, nMsgType);
				}

				T_LCOM_D LcomD_cf = LcomD; // MNET:1614, KJH20050809, CF°¡ ¾ø´Â ÇÏÁßÁ¶ÇÕ
				BOOL bLive = FALSE, bCF = FALSE;
				if (nType != 2)
				{
					bLive = m_nLCount > 0 || (bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(L)));
					if (bLive) // ST or ST+CS
					{
						int nLL = 0, nIL = 0, nCF = 0;
						for (nc = 0; nc < m_nLCount; nc++) nLL += Set_STLCIncludeCS(arLLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD, 0);
						if (nLL > 0) { strDesc.Format(_T("+%sL"), GetFactorString(dFactor)); LcomD.Description = LcomD.Description + strDesc; }
						if (bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(L)))
						{
							Set_CStageCase(m_aCSLoad.GetAt(nnn), dFactor, LoadCaseNo, LcomD, TRUE);
							strDesc.Format(_T("+%s(cEL)"), GetFactorString(dFactor)); LcomD.Description = LcomD.Description + strDesc;
						}
						for (nc = 0; nc < m_nIlCount; nc++) nIL += Set_STLCIncludeCS(arIlLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD, 0);
						if (nIL > 0) { strDesc.Format(_T("+%sIL"), GetFactorString(dFactor)); LcomD.Description = LcomD.Description + strDesc; }

						LcomD_cf = LcomD;
						for (nc = 0; nc < m_nCfCount; nc++) nCF += Set_STLCIncludeCS(arCfLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD, 0);
						if (nCF > 0) { strDesc.Format(_T("+%sCF"), GetFactorString(dFactor)); LcomD.Description = LcomD.Description + strDesc; bCF = TRUE; }
					}
				}

				if (m_nLsCount > 0) // Live Load Surcharge
				{
					T_LCOM_D LcomD_l = LcomD; LoadCaseNo = GetCountLcomCase(LcomD_l);
					int nLS = 0;
					for (nc = 0; nc < m_nLsCount; nc++) nLS += Set_STLCIncludeCS(arLsLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD_l, 0);
					T_LCOM_D LcomD_tmp = LcomD_cf;
					if (bCF)
					{
						LoadCaseNo = GetCountLcomCase(LcomD_tmp);
						for (nc = 0; nc < m_nLsCount; nc++) Set_STLCIncludeCS(arLsLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD_tmp, 0);
					}
					if (nLS > 0)
					{
						strDesc.Format(_T("+%sLS"), GetFactorString(dFactor)); LcomD_l.Description = LcomD_l.Description + strDesc;
						if (bCF) LcomD_tmp.Description = LcomD_tmp.Description + strDesc;
						ArINT artmp;
						SetCvlESCGB19_ETC(LcomD_l, LcomD_tmp, bCF, arCrlLoad, artmp, nType != 2, arBrkLoad, bLive, arWLoad, TRUE, arSfLoad, TRUE, arIpLoad, TRUE,
							arTLoad, arTpgLoad, TRUE, arFrLoad, FALSE, 1, dF, nType == 2 || nType == 4, nMsgType);
					}
				}
				if (nType != 2 && m_nCrlCount > 0) // Crowd Load
				{
					T_LCOM_D LcomD_l = LcomD; LoadCaseNo = GetCountLcomCase(LcomD_l);
					int nCRL = 0;
					for (nc = 0; nc < m_nCrlCount; nc++) nCRL += Set_STLCIncludeCS(arCrlLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD_l, 0);
					T_LCOM_D LcomD_tmp = LcomD_cf;
					if (bCF)
					{
						LoadCaseNo = GetCountLcomCase(LcomD_tmp);
						for (nc = 0; nc < m_nCrlCount; nc++) Set_STLCIncludeCS(arCrlLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD_tmp, 0);
					}
					if (nCRL > 0)
					{
						strDesc.Format(_T("+%sCRL"), GetFactorString(dFactor)); LcomD_l.Description = LcomD_l.Description + strDesc;
						if (bCF) LcomD_tmp.Description = LcomD_tmp.Description + strDesc;
						ArINT artmp;
						SetCvlESCGB19_ETC(LcomD_l, LcomD_tmp, bCF, arCrlLoad, artmp, FALSE, arBrkLoad, bLive, arWLoad, TRUE, arSfLoad, TRUE, arIpLoad, TRUE,
							arTLoad, arTpgLoad, TRUE, arFrLoad, FALSE, 2, dF, nType == 2 || nType == 4, nMsgType);
					}
				}
				if (m_nBrkCount > 0 && bLive) // Brake Load
				{
					T_LCOM_D LcomD_l = LcomD; LoadCaseNo = GetCountLcomCase(LcomD_l);
					double dBrkF = (nType == 0 || nType == 4) ? dFactor * 0.7 : 1.0;
					int nBRK = 0;
					for (nc = 0; nc < m_nBrkCount; nc++) nBRK += Set_STLCIncludeCS(arBrkLoad.GetAt(nc), dBrkF, LoadCaseNo, LcomD_l, 0);
					if (nBRK > 0)
					{
						strDesc.Format(_T("+%sBRK"), GetFactorString(dBrkF)); LcomD_l.Description = LcomD_l.Description + strDesc;
						ArINT artmp;
						SetCvlESCGB19_ETC(LcomD_l, LcomD_cf, FALSE, arCrlLoad, artmp, FALSE, arBrkLoad, FALSE, arWLoad, TRUE, arSfLoad, FALSE, arIpLoad, FALSE,
							arTLoad, arTpgLoad, FALSE, arFrLoad, FALSE, 3, dF, nType == 2 || nType == 4, nMsgType);
					}
					if (bCF) // MNET:1614, KJH20050809, CF°¡ ¾ø´Â ÇÏÁßÁ¶ÇÕ ÇÑ¹ø´õ...
					{
						LcomD_l = LcomD_cf; LoadCaseNo = GetCountLcomCase(LcomD_l);
						dBrkF = (nType == 0 || nType == 4) ? dFactor : 1.0;
						nBRK = 0;
						for (nc = 0; nc < m_nBrkCount; nc++) nBRK += Set_STLCIncludeCS(arBrkLoad.GetAt(nc), dBrkF, LoadCaseNo, LcomD_l, 0);
						if (nBRK > 0)
						{
							strDesc.Format(_T("+%sBRK"), GetFactorString(dBrkF)); LcomD_l.Description = LcomD_l.Description + strDesc;
							ArINT artmp;
							SetCvlESCGB19_ETC(LcomD_l, LcomD_cf, FALSE, arCrlLoad, artmp, FALSE, arBrkLoad, FALSE, arWLoad, TRUE, arSfLoad, FALSE, arIpLoad, FALSE,
								arTLoad, arTpgLoad, FALSE, arFrLoad, FALSE, 3, dF, nType == 2 || nType == 4, nMsgType);
						}
					}
				}
				int nWCount = m_nWCount * 2; // MNET:1614, KJH20050808
				if (m_nWCount == 0 && bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(W))) nWCount = 1;
				for (nc = 0; nc < nWCount; nc++)
				{
					T_LCOM_D LcomD_l = LcomD; LoadCaseNo = GetCountLcomCase(LcomD_l);
					if (m_nWCount > 0)
					{
						double dWF = (nc % 2 == 0) ? dWFactor : -1.*dWFactor;
						if (Set_STLCIncludeCS(arWLoad.GetAt(nc / 2), dWF, LoadCaseNo, LcomD_l, 0) > 0)
						{
							CString strW = (dWF > 0.0) ? _T("+%sW") : _T("%sW");
							strDesc.Format(strW, GetFactorString(dWF)); LcomD_l.Description = LcomD_l.Description + strDesc;
							if (m_nWCount > 1) { strDesc.Format(_T("[%d]"), nc / 2 + 1); LcomD_l.Description = LcomD_l.Description + strDesc; }
						}
					}
					if (bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(W)))
					{
						Set_CStageCase(m_aCSLoad.GetAt(nnn), dWFactor, LoadCaseNo, LcomD_l, TRUE);
						strDesc.Format(_T("+%s(cEL)"), GetFactorString(dWFactor)); LcomD_l.Description = LcomD_l.Description + strDesc;
					}
					ArINT artmp;
					SetCvlESCGB19_ETC(LcomD_l, LcomD_cf, bCF, arCrlLoad, artmp, FALSE, arBrkLoad, FALSE, arWLoad, FALSE, arSfLoad, TRUE, arIpLoad, TRUE,
						arTLoad, arTpgLoad, TRUE, arFrLoad, FALSE, 4, dF, nType == 2 || nType == 4, nMsgType);
				}
				if (m_nSfCount > 0) // Stream Flow Pressure
				{
					T_LCOM_D LcomD_l = LcomD; LoadCaseNo = GetCountLcomCase(LcomD_l);
					int nSF = 0;
					for (nc = 0; nc < m_nSfCount; nc++) nSF += Set_STLCIncludeCS(arSfLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD_l, 0);
					if (nSF > 0)
					{
						strDesc.Format(_T("+%sSF"), GetFactorString(dFactor)); LcomD_l.Description = LcomD_l.Description + strDesc;
						ArINT artmp;
						SetCvlESCGB19_ETC(LcomD_l, LcomD_cf, bCF, arCrlLoad, artmp, FALSE, arBrkLoad, FALSE, arWLoad, FALSE, arSfLoad, FALSE, arIpLoad, FALSE,
							arTLoad, arTpgLoad, TRUE, arFrLoad, FALSE, 5, dF, nType == 2 || nType == 4, nMsgType);
					}
				}
				if (m_nIpCount > 0) // Ice Pressure
				{
					T_LCOM_D LcomD_l = LcomD; LoadCaseNo = GetCountLcomCase(LcomD_l);
					int nIP = 0;
					for (nc = 0; nc < m_nIpCount; nc++) nIP += Set_STLCIncludeCS(arIpLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD_l, 0);
					if (nIP > 0)
					{
						strDesc.Format(_T("+%sIP"), GetFactorString(dFactor)); LcomD_l.Description = LcomD_l.Description + strDesc;
						ArINT artmp;
						SetCvlESCGB19_ETC(LcomD_l, LcomD_cf, bCF, arCrlLoad, artmp, FALSE, arBrkLoad, FALSE, arWLoad, FALSE, arSfLoad, FALSE, arIpLoad, FALSE,
							arTLoad, arTpgLoad, TRUE, arFrLoad, FALSE, 5, dF, nType == 2 || nType == 4, nMsgType);
					}
				}
			}
		}
	}

	return TRUE;
}
BOOL CLoadCombCtrl::SetCvlESCGB19_Var4_A(const CArray<T_LCOM_D, T_LCOM_D&>& arData, ArINT& arLLoad,
	ArINT& arIlLoad, ArINT& arCfLoad, ArINT& arLsLoad, ArINT& arCrlLoad, ArINT& arWLoad,
	ArINT& arSfLoad, ArINT& arIpLoad, ArINT& arTLoad, ArINT& arTpgLoad, ArINT& arFrLoad,
	ArINT& arBrkLoad, ArUNT& arMLoad, ArINT& arMcrLoad, int nType, int nMsgType, int nAcdType)
{
	// nType == 0,4: °è¼öÁ¶ÇÕ, 1: 1.0(settlement Á¦¿Ü), 2: 1.0(¸ðµÎ), Live Load Á¦¿Ü
	CArray<T_LCOM_D, T_LCOM_D&>aLcomD; aLcomD.Copy(arData);
	int  nLcomSize = aLcomD.GetSize();
	int  nnn = 0;
	BOOL bAddCsLoad = m_nConstLoad != 0 && CheckCS_SgldK(D_SGLD_ER1_CVL, nnn); // CS or ST+CS

	double dFactor = 1.0;
	double dMLFactor = 1.0;     // ???äü??
	double dCrlFactor = 1.0;    // ???äü??
	double dWFactor = 0.75;      // ??äü??
	double dTgpFactor = 0.8;    // ???äü??

	if (nAcdType == 0)//ÆµÓöÏµÊý modified this statement by gongxing at 2016-04-13
	{
		dMLFactor = 0.7;
		dCrlFactor = 1.0;
	}
	else if (nAcdType == 1)//×¼ÓÀ¾ÃÏµÊý
	{
		dMLFactor = 0.4;
		dCrlFactor = 0.4;
	}

	double dF = (nType == 0 || nType == 4) ? 0.75 : 1.0;
	double dIL = (nType == 12) ? 0. : CalcMovingILFactor();

	// ¿µ±¸ÇÏÁß + Â÷·®ÇÏÁß + 4°³ÀÇ °¡º¯ÇÏÁßÀÎ °æ¿ì
	CString strDesc = _T("");
	int LoadCaseNo = 0, nc = 0;
	for (int lc = 0; lc < nLcomSize; lc++)
	{
		T_LCOM_D LcomD = aLcomD.GetAt(lc);
		LcomD.nActive = 6; // Active or Strength/Stress
		if (m_nLcomType == D_LCOMTYPE_GENERAL) LcomD.nActive = 1;  // general
		LoadCaseNo = GetCountLcomCase(LcomD);

		int nMCount = max(m_nMCount + 1, 1);
		for (int m = 0; m < nMCount; m++)
		{
			if (m < m_nMCount)
			{

				Set_MovingCase(arMLoad.GetAt(m), dMLFactor, LoadCaseNo, LcomD, TRUE);  //????
				strDesc.Format(_T("+%sM"), GetFactorString(dMLFactor)); LcomD.Description = LcomD.Description + strDesc;
				if (m_nMCount > 1) { strDesc.Format(_T("[%d]"), m + 1); LcomD.Description = LcomD.Description + strDesc; }
				if (nType == 2 || nType == 4) m_aAcdnLcomD.Add(LcomD);
				else                     Set_LoadComb4Civil(LcomD, D_STR, 0, nMsgType);
			}

			int nMcrCount = max(m_nMCrlCount + 1, 1);
			for (int nMcr = 0; nMcr < nMcrCount; ++nMcr)
			{
				if (m < m_nMCount && nMcr < m_nMCrlCount)
				{
					Set_MovingCase(arMcrLoad.GetAt(nMcr), dCrlFactor, LoadCaseNo, LcomD, TRUE);
					strDesc.Format(_T("+%sMcr"), GetFactorString(dCrlFactor)); LcomD.Description = LcomD.Description + strDesc;
					if (m_nMCrlCount > 1) { strDesc.Format(_T("[%d]"), nMcr + 1); LcomD.Description = LcomD.Description + strDesc; }
					if (nType == 2 || nType == 4) m_aAcdnLcomD.Add(LcomD);
					else                     Set_LoadComb4Civil(LcomD, D_STR, 0, nMsgType);
				}

				T_LCOM_D LcomD_cf = LcomD; // MNET:1614, KJH20050809, CF°¡ ¾ø´Â ÇÏÁßÁ¶ÇÕ
				BOOL bLive = FALSE, bCF = FALSE;
				if (nType != 2)
				{
					bLive = m_nLCount > 0 || (bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(L)));
					if (bLive) // ST or ST+CS
					{
						int nLL = 0, nIL = 0, nCF = 0;
						for (nc = 0; nc < m_nLCount; nc++) nLL += Set_STLCIncludeCS(arLLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD, 0);
						if (nLL > 0) { strDesc.Format(_T("+%sL"), GetFactorString(dFactor)); LcomD.Description = LcomD.Description + strDesc; }
						if (bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(L)))
						{
							Set_CStageCase(m_aCSLoad.GetAt(nnn), dFactor, LoadCaseNo, LcomD, TRUE);
							strDesc.Format(_T("+%s(cEL)"), GetFactorString(dFactor)); LcomD.Description = LcomD.Description + strDesc;
						}
						for (nc = 0; nc < m_nIlCount; nc++) nIL += Set_STLCIncludeCS(arIlLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD, 0);
						if (nIL > 0) { strDesc.Format(_T("+%sIL"), GetFactorString(dFactor)); LcomD.Description = LcomD.Description + strDesc; }

						LcomD_cf = LcomD;
						for (nc = 0; nc < m_nCfCount; nc++) nCF += Set_STLCIncludeCS(arCfLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD, 0);
						if (nCF > 0) { strDesc.Format(_T("+%sCF"), GetFactorString(dFactor)); LcomD.Description = LcomD.Description + strDesc; bCF = TRUE; }
					}
				}

				if (m_nLsCount > 0) // Live Load Surcharge
				{
					T_LCOM_D LcomD_l = LcomD; LoadCaseNo = GetCountLcomCase(LcomD_l);
					int nLS = 0;
					for (nc = 0; nc < m_nLsCount; nc++) nLS += Set_STLCIncludeCS(arLsLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD_l, 0);
					T_LCOM_D LcomD_tmp = LcomD_cf;
					if (bCF)
					{
						LoadCaseNo = GetCountLcomCase(LcomD_tmp);
						for (nc = 0; nc < m_nLsCount; nc++) Set_STLCIncludeCS(arLsLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD_tmp, 0);
					}
					if (nLS > 0)
					{
						strDesc.Format(_T("+%sLS"), GetFactorString(dFactor)); LcomD_l.Description = LcomD_l.Description + strDesc;
						if (bCF) LcomD_tmp.Description = LcomD_tmp.Description + strDesc;
						BOOL bInc = nType == 2 ? FALSE : TRUE;
						ArINT artmp;
						SetCvlESCGB19_ETC(LcomD_l, LcomD_tmp, bCF, arCrlLoad, artmp, nType != 2, arBrkLoad, bLive, arWLoad, TRUE, arSfLoad, TRUE, arIpLoad, TRUE,
							arTLoad, arTpgLoad, FALSE, arFrLoad, FALSE, 6, dF, nType == 2 || nType == 4, nMsgType);
					}
				}
				if (nType != 2 && m_nCrlCount > 0) // Crowd Load
				{
					T_LCOM_D LcomD_l = LcomD; LoadCaseNo = GetCountLcomCase(LcomD_l);
					int nCRL = 0;
					for (nc = 0; nc < m_nCrlCount; nc++) nCRL += Set_STLCIncludeCS(arCrlLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD_l, 0);
					T_LCOM_D LcomD_tmp = LcomD_cf;
					if (bCF)
					{
						LoadCaseNo = GetCountLcomCase(LcomD_tmp);
						for (nc = 0; nc < m_nCrlCount; nc++) Set_STLCIncludeCS(arCrlLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD_tmp, 0);
					}
					if (nCRL > 0)
					{
						strDesc.Format(_T("+%sCRL"), GetFactorString(dFactor)); LcomD_l.Description = LcomD_l.Description + strDesc;
						if (bCF) LcomD_tmp.Description = LcomD_tmp.Description + strDesc;
						ArINT artmp;
						SetCvlESCGB19_ETC(LcomD_l, LcomD_tmp, bCF, arCrlLoad, artmp, FALSE, arBrkLoad, bLive, arWLoad, TRUE, arSfLoad, TRUE, arIpLoad, TRUE,
							arTLoad, arTpgLoad, FALSE, arFrLoad, FALSE, 7, dF, nType == 2 || nType == 4, nMsgType);
					}
				}
				int nWCount = m_nWCount * 2; // MNET:1614, KJH20050808
				if (m_nWCount == 0 && bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(W))) nWCount = 1;
				for (nc = 0; nc < nWCount; nc++)
				{
					T_LCOM_D LcomD_l = LcomD; LoadCaseNo = GetCountLcomCase(LcomD_l);
					if (m_nWCount > 0)
					{
						double dWF = (nc % 2 == 0) ? dWFactor : -1.*dWFactor;
						if (Set_STLCIncludeCS(arWLoad.GetAt(nc / 2), dWF, LoadCaseNo, LcomD_l, 0) > 0)
						{
							CString strW = (dWF > 0.0) ? _T("+%sW") : _T("%sW");
							strDesc.Format(strW, GetFactorString(dWF)); LcomD_l.Description = LcomD_l.Description + strDesc;
							if (m_nWCount > 1) { strDesc.Format(_T("[%d]"), nc / 2 + 1); LcomD_l.Description = LcomD_l.Description + strDesc; }
						}
					}
					if (bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(W)))
					{
						Set_CStageCase(m_aCSLoad.GetAt(nnn), dWFactor, LoadCaseNo, LcomD_l, TRUE);
						strDesc.Format(_T("+%s(cEL)"), GetFactorString(dWFactor)); LcomD_l.Description = LcomD_l.Description + strDesc;
					}
					ArINT artmp;
					SetCvlESCGB19_ETC(LcomD_l, LcomD_cf, bCF, arCrlLoad, artmp, FALSE, arBrkLoad, FALSE, arWLoad, FALSE, arSfLoad, TRUE, arIpLoad, TRUE,
						arTLoad, arTpgLoad, FALSE, arFrLoad, FALSE, 7, dF, nType == 2 || nType == 4, nMsgType);
				}
			}
		}
	}

	return TRUE;
}
BOOL CLoadCombCtrl::SetCvlESCGB19_Var56_A(const CArray<T_LCOM_D, T_LCOM_D&>& arData, ArINT& arLLoad,
	ArINT& arIlLoad, ArINT& arCfLoad, ArINT& arLsLoad, ArINT& arCrlLoad, ArINT& arWLoad,
	ArINT& arSfLoad, ArINT& arIpLoad, ArINT& arTLoad, ArINT& arTpgLoad, ArINT& arFrLoad,
	ArINT& arBrkLoad, ArUNT& arMLoad, ArINT& arMcrLoad, int nType, int nMsgType, int nAcdType)
{
	// nType == 0,4: °è¼öÁ¶ÇÕ, 1: 1.0(settlement Á¦¿Ü), 2: 1.0(¸ðµÎ), Live Load Á¦¿Ü
	CArray<T_LCOM_D, T_LCOM_D&> aLcomD; aLcomD.Copy(arData);
	int  nLcomSize = aLcomD.GetSize();
	int  nnn = 0;
	BOOL bAddCsLoad = m_nConstLoad != 0 && CheckCS_SgldK(D_SGLD_ER1_CVL, nnn); // CS or ST+CS

	double dFactor = 1.0;
	double dMLFactor = 1.0;     // ???äü??
	double dCrlFactor = 1.0;    // ???äü??
	double dWFactor = 0.75;      // ??äü??
	double dTgpFactor = 0.8;    // ???äü??

	if (nAcdType == 0)//ÆµÓöÏµÊý modified this statement by gongxing at 2016-04-13
	{
		dMLFactor = 0.7;
		dCrlFactor = 1.0;
	}
	else if (nAcdType == 1)//×¼ÓÀ¾ÃÏµÊý
	{
		dMLFactor = 0.4;
		dCrlFactor = 0.4;
	}

	double dF = (nType == 0 || nType == 4) ? 0.5 : 1.0;
	double dIL = (nType == 12) ? 0. : CalcMovingILFactor();

	CString strDesc = _T("");
	int LoadCaseNo = 0, nc = 0;
	int lc = 0;
	for (lc = 0; lc < nLcomSize; lc++)
	{
		T_LCOM_D LcomD = aLcomD.GetAt(lc);
		LcomD.nActive = 6; // Active or Strength/Stress
		if (m_nLcomType == D_LCOMTYPE_GENERAL) LcomD.nActive = 1;  // general
		LoadCaseNo = GetCountLcomCase(LcomD);

		int nMCount = max(m_nMCount + 1, 1);
		for (int m = 0; m < nMCount; m++)
		{
			if (m < m_nMCount)
			{
				Set_MovingCase(arMLoad.GetAt(m), dMLFactor, LoadCaseNo, LcomD, TRUE);  //????
				strDesc.Format(_T("+%sM"), GetFactorString(dMLFactor)); LcomD.Description = LcomD.Description + strDesc;
				if (m_nMCount > 1) { strDesc.Format(_T("[%d]"), m + 1); LcomD.Description = LcomD.Description + strDesc; }
				if (nType == 2 || nType == 4) m_aAcdnLcomD.Add(LcomD);
				else                     Set_LoadComb4Civil(LcomD, D_STR, 0, nMsgType);
			}

			int nMcrCount = max(m_nMCrlCount + 1, 1);
			for (int nMcr = 0; nMcr < nMcrCount; ++nMcr)
			{
				if (m < m_nMCount&& nMcr < m_nMCrlCount)
				{
					Set_MovingCase(arMcrLoad.GetAt(nMcr), dCrlFactor, LoadCaseNo, LcomD, TRUE);
					strDesc.Format(_T("+%sMcr"), GetFactorString(dCrlFactor)); LcomD.Description = LcomD.Description + strDesc;
					if (m_nMCrlCount > 1) { strDesc.Format(_T("[%d]"), nMcr + 1); LcomD.Description = LcomD.Description + strDesc; }
					if (nType == 2 || nType == 4) m_aAcdnLcomD.Add(LcomD);
					else                     Set_LoadComb4Civil(LcomD, D_STR, 0, nMsgType);
				}

				T_LCOM_D LcomD_cf = LcomD; // MNET:1614, KJH20050809, CF Á¦¿ÜÇÑ ÇÏÁßÁ¶ÇÕ
				BOOL bLive = FALSE, bCF = FALSE;
				if (nType != 2)
				{
					bLive = m_nLCount > 0 || (bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(L)));
					if (bLive) // ST or ST+CS
					{
						int nLL = 0, nIL = 0, nCF = 0;
						for (nc = 0; nc < m_nLCount; nc++) nLL += Set_STLCIncludeCS(arLLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD, 0);
						if (nLL > 0) { strDesc.Format(_T("+%sL"), GetFactorString(dFactor)); LcomD.Description = LcomD.Description + strDesc; }
						if (bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(L)))
						{
							Set_CStageCase(m_aCSLoad.GetAt(nnn), dFactor, LoadCaseNo, LcomD, TRUE);
							strDesc.Format(_T("+%s(cEL)"), GetFactorString(dFactor)); LcomD.Description = LcomD.Description + strDesc;
						}
						for (nc = 0; nc < m_nIlCount; nc++) nIL += Set_STLCIncludeCS(arIlLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD, 0);
						if (nIL > 0) { strDesc.Format(_T("+%sIL"), GetFactorString(dFactor)); LcomD.Description = LcomD.Description + strDesc; }

						LcomD_cf = LcomD;
						for (nc = 0; nc < m_nCfCount; nc++) nCF += Set_STLCIncludeCS(arCfLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD, 0);
						if (nCF > 0) { strDesc.Format(_T("+%sCF"), GetFactorString(dFactor)); LcomD.Description = LcomD.Description + strDesc; bCF = TRUE; }
					}
				}

				if (m_nLsCount > 0) // Live Load Surcharge
				{
					T_LCOM_D LcomD_l = LcomD; LoadCaseNo = GetCountLcomCase(LcomD_l);
					int nLS = 0;
					for (nc = 0; nc < m_nLsCount; nc++) nLS += Set_STLCIncludeCS(arLsLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD_l, 0);
					T_LCOM_D LcomD_tmp = LcomD_cf;
					if (bCF)
					{
						LoadCaseNo = GetCountLcomCase(LcomD_tmp);
						for (nc = 0; nc < m_nLsCount; nc++) Set_STLCIncludeCS(arLsLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD_tmp, 0);
					}
					if (nLS > 0)
					{
						strDesc.Format(_T("+%sLS"), GetFactorString(dFactor)); LcomD_l.Description = LcomD_l.Description + strDesc;
						if (bCF) LcomD_tmp.Description = LcomD_tmp.Description + strDesc;
						ArINT artmp;
						SetCvlESCGB19_ETC(LcomD_l, LcomD_tmp, bCF, arCrlLoad, artmp, nType != 2, arBrkLoad, bLive, arWLoad, TRUE, arSfLoad, FALSE, arIpLoad, FALSE,
							arTLoad, arTpgLoad, FALSE, arFrLoad, FALSE, 8, dF, nType == 2 || nType == 4, nMsgType);
					}
				}
				if (nType != 2 && m_nCrlCount > 0) // Crowd Load
				{
					T_LCOM_D LcomD_l = LcomD; LoadCaseNo = GetCountLcomCase(LcomD_l);
					int nCRL = 0;
					for (nc = 0; nc < m_nCrlCount; nc++) nCRL += Set_STLCIncludeCS(arCrlLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD_l, 0);
					if (nCRL > 0)
					{
						strDesc.Format(_T("+%sCRL"), GetFactorString(dFactor)); LcomD_l.Description = LcomD_l.Description + strDesc;
						ArINT artmp;
						SetCvlESCGB19_ETC(LcomD_l, LcomD_cf, bCF, arCrlLoad, artmp, FALSE, arBrkLoad, FALSE, arWLoad, TRUE, arSfLoad, FALSE, arIpLoad, FALSE,
							arTLoad, arTpgLoad, FALSE, arFrLoad, FALSE, 9, dF, nType == 2 || nType == 4, nMsgType);
					}
				}
			}
		}
	}

	// ¿µ±¸ÇÏÁß + Â÷·®ÇÏÁß + 6°³ÀÇ °¡º¯ÇÏÁßÀÎ °æ¿ì
	for (lc = 0; lc < nLcomSize; lc++)
	{
		T_LCOM_D LcomD = aLcomD.GetAt(lc); LcomD.nActive = 1; // Active or Strength/Stress
		LoadCaseNo = GetCountLcomCase(LcomD);

		if (nType != 2)
		{
			if (m_nLCount > 0 || (bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(L)))) // ST or ST+CS
			{
				int nLL = 0, nIL = 0, nCF = 0;
				for (nc = 0; nc < m_nLCount; nc++) nLL += Set_STLCIncludeCS(arLLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD, 0);
				if (nLL > 0) { strDesc.Format(_T("+%sL"), GetFactorString(dFactor)); LcomD.Description = LcomD.Description + strDesc; }
				if (bAddCsLoad && CheckCSEL(D_SGLD_ER1_CVL, _LSX(L)))
				{
					Set_CStageCase(m_aCSLoad.GetAt(nnn), dFactor, LoadCaseNo, LcomD, TRUE);
					strDesc.Format(_T("+%s(cEL)"), GetFactorString(dFactor)); LcomD.Description = LcomD.Description + strDesc;
				}
				for (nc = 0; nc < m_nIlCount; nc++) nIL += Set_STLCIncludeCS(arIlLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD, 0);
				if (nIL > 0) { strDesc.Format(_T("+%sIL"), GetFactorString(dFactor)); LcomD.Description = LcomD.Description + strDesc; }
				for (nc = 0; nc < m_nCfCount; nc++) nCF += Set_STLCIncludeCS(arCfLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD, 0);
				if (nCF > 0) { strDesc.Format(_T("+%sCF"), GetFactorString(dFactor)); LcomD.Description = LcomD.Description + strDesc; }
			}
		}

		if (m_nLsCount > 0) // Live Load Surcharge
		{
			T_LCOM_D LcomD_l = LcomD; LoadCaseNo = GetCountLcomCase(LcomD_l);
			int nLS = 0;
			for (nc = 0; nc < m_nLsCount; nc++) nLS += Set_STLCIncludeCS(arLsLoad.GetAt(nc), dFactor, LoadCaseNo, LcomD_l, 0);
			if (nLS > 0)
			{
				strDesc.Format(_T("+%sLS"), GetFactorString(dFactor)); LcomD_l.Description = LcomD_l.Description + strDesc;
				ArINT artmp;
				SetCvlESCGB19_ETC(LcomD_l, LcomD_l, FALSE, arCrlLoad, artmp, nType != 2, arBrkLoad, FALSE, arWLoad, FALSE, arSfLoad, FALSE, arIpLoad, FALSE,
					arTLoad, arTpgLoad, FALSE, arFrLoad, FALSE, 10, dF, nType == 2 || nType == 4, nMsgType);
			}
		}
	}

	return TRUE;
}

////////////////////////////////////////////////////////////////////////////// 
#pragma endregion
//////////////////////////////////////////////////////////////////////////