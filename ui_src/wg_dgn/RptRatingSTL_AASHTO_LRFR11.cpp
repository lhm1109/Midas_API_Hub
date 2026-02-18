#include "stdafx.h"
#include "RptRatingSTL_AASHTO_LRFR11.h"

#include "Dgn_RatingRptManager.h"

//#include "..\wg_db\DBDoc.h"
#include "..\wg_db\FileCtrl.h"
#include "..\wg_db\AttrCtrl.h"
#include "..\wg_db\AttrCtrl2.h"
#include "..\wg_db\PostCtrl.h"
#include "..\wg_db\AnalysisResult.h"
#include "..\wg_db\DBLib.h"




//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRptRatingSTL_AASHTO_LRFR11::CRptRatingSTL_AASHTO_LRFR11()
{
	m_pDoc = CDBDoc::GetDocPoint(); ASSERT(m_pDoc);
	m_bStopExecute = FALSE;
	m_RpasD.Initialize();
	m_pDoc->m_pAttrCtrl2->GetRpas(m_RpasD); 
	
	m_pDoc->SetCivilCodeDgn(STL_AASHTO_LRFD12);
	m_pDataCtrl = new CCRCDataCtrl();
	
	m_pForcCtrl = m_pDataCtrl->Get_ForceCtrlPointer();
	if(m_pForcCtrl==NULL) {ASSERT(0); return;}
	
}

CRptRatingSTL_AASHTO_LRFR11::~CRptRatingSTL_AASHTO_LRFR11()
{
	if(m_pDataCtrl)
	{
		delete m_pDataCtrl;
		m_pDataCtrl = NULL;
	}
}

BOOL CRptRatingSTL_AASHTO_LRFR11::Execute_RatingReport(int iDgnCode, CString strPath, int iPrintOpt, BOOL bSaveImage)
{
	if(m_bStopExecute) return FALSE;
	Progress(3, 0);
	int nProgressPercent = 0;
	
	if(m_bStopExecute) return FALSE;
	nProgressPercent +=5;
	Progress(0, nProgressPercent);
	Progress(2, nProgressPercent);
	
	if(m_bStopExecute) return FALSE;
	nProgressPercent +=5;
	Progress(0, nProgressPercent);
	Progress(2, nProgressPercent);

	BOOL bSuccess = TRUE;

	CAnalysisResult *pAnalysisResult = m_pDoc->m_pPostCtrl->GetAnalysisResult();
	
	T_RPAS_D RpasD; RpasD.Initialize();  
	m_pDoc->m_pAttrCtrl2->GetRpas(RpasD);

	// Print항목이 없을 경우 강제로 ProgressBar를 닫도록 조치!!
	CArray<T_RPRS_K, T_RPRS_K> aPrintElemKey;  
	m_pDoc->m_pAttrCtrl2->GetRprsKeyList(aPrintElemKey);

	ArrElemPairKey aPrintElemPairKey;
	CDBLib::ConvertToElemPairKey(EN_EL_BEAM, aPrintElemKey, aPrintElemPairKey);

	CArray<T_RPRV_K, T_RPRV_K> aPrintVBeamKey;
	m_pDoc->m_pAttrCtrl2->GetRprvKeyList(aPrintVBeamKey);

	if (aPrintVBeamKey.GetSize() > 0)
	{
		ArrElemPairKey aPrintVBeamPairKey;
		CDBLib::ConvertToElemPairKey(EN_EL_VBEAM, aPrintVBeamKey, aPrintVBeamPairKey);
		aPrintElemPairKey.Append(aPrintVBeamPairKey);
	}
	
	int nPrintElemSize = aPrintElemPairKey.GetSize();
	if(nPrintElemSize==0)
	{
		if(m_bStopExecute) return FALSE;
		nProgressPercent = 100;
		Progress(0, nProgressPercent);
		Progress(2, nProgressPercent);
		
		CString strErrMessage = _LS(IDS_DGN_PSC_RATING_NOT_INPUT_MEMBER); // _T("출력할 요소의 설계정보가 없습니다.")
		AfxMessageBox(strErrMessage);
		
		return FALSE;
	}
	
	int nProgressIncrementPercent = nPrintElemSize == 0 ? 40 : 40/nPrintElemSize;
	
	int i=0; // 요소 
	int j=0; // I/J
	int k=0; // Positive / Negative
	int kk=0;
	int nIJ=0, nMaxMin=0;
	
	CDesignLoadRating *pCheckGirder = m_pDoc->m_pPostCtrl->GetLoadRating();

	try
	{
		RAT_LRAT_RPT RatResRpt;    
		RatResRpt.nMeasureType =  RpasD.nMeasurement;

		for(i=0; i<nPrintElemSize; ++i)
		{
			auto ElemK = aPrintElemPairKey[i];

			RAT_ELEM_RES RatElemR;
			RatElemR.nElem = ElemK.first;

			T_RPRS_D RkpoD; RkpoD.Initialize();  // 출력 위치
			T_SECT_D SectD; SectD.Initialize();

			if (ElemK.second == EN_EL_BEAM)
			{
				if (!m_pDoc->m_pAttrCtrl2->GetRprs(ElemK.first, RkpoD)) continue;
				T_ELEM_D ElemD; ElemD.Initialize();
				if (!m_pDoc->m_pAttrCtrl->GetElem(ElemK.first, ElemD)) continue;
				m_pDoc->m_pPostCtrl->GetSectPost(ElemD.elpro, SectD);
			}
			else if (ElemK.second == EN_EL_VBEAM)
			{
				T_RPRV_D RprvD; RprvD.Initialize();
				if (!m_pDoc->m_pAttrCtrl2->GetRprv(ElemK.first, RprvD)) continue;
				RkpoD=RprvD;

				T_VBEM_D VbemD; VbemD.Initialize();
				if (m_pDoc->m_pAttrCtrl2->GetVbem(ElemK.first, VbemD))
				{
					T_SECV_D SecvD; SecvD.Initialize();
					m_pDoc->m_pAttrCtrl2->GetSecv(ElemK.first, SecvD);
					SecvD.ConvertToSect(SectD);
				}
			}
			else ASSERT(0);
			

			BOOL bPrintI = (RkpoD.nPrintChk == 0 || RkpoD.nPrintChk == 2);
			BOOL bPrintJ = (RkpoD.nPrintChk == 1 || RkpoD.nPrintChk == 2);
			RatElemR.nPrtOpt = RkpoD.nPrintChk;

			double dVuFactor = 1.0;
			if (SectD.SectBefore.Shape == D_SECT_SHAPE_COMPO_B ||
				SectD.SectBefore.Shape == D_SECT_SHAPE_COMPO_TUB)
				dVuFactor = 0.5;

			T_RCFC_D RcfcD;
			T_RCSC_D RcscD;
			T_RCGS_D RcgsD;
			T_RCFA_D RcfaD;
			BOOL bIsFlexRes   = pAnalysisResult->GetStlRcfcResult(ElemK, RcfcD);
			BOOL bIsShearRes  = pAnalysisResult->GetStlRcscResult(ElemK, RcscD);
			BOOL bIsStressRes = pAnalysisResult->GetStlRcgsResult(ElemK, RcgsD);
			BOOL bIsFatiRes   = pAnalysisResult->GetStlRcfaResult(ElemK, RcfaD);

			if (!bIsFlexRes && !bIsShearRes && !bIsStressRes && !bIsFatiRes) continue;

			// 요소별..
			for(j=0; j<2; j++) // I,J
			{
				if(j==0 && (!bPrintI)) continue;
				if(j==1 && (!bPrintJ)) continue;

				if (bIsFlexRes==TRUE)
				{
					RatElemR.FlexR.bChk = TRUE;
					for (k=0; k<2; ++k)
					{
						int nIdx=j*2+k;
						T_RCCR_BASE &FlexKB = RcfcD.FlexR[nIdx];
						if (FlexKB.ElemK>0)
						{
							T_RLCS_KEY RlcsKey = m_pDoc->m_pPostCtrl->GetStlRlcsRatingKey(FlexKB.RCaseK, FlexKB.bDCmax, FlexKB.bDWmax, FlexKB.bTEmax, FlexKB.nConcurrent);
							
							int nEndIdx = j*2+k;
							BOOL bPositiveM = (k==0);
							T_RCST_CASE FlexC;
							pAnalysisResult->GetStlStrengthRating(ElemK, RlcsKey, FlexC);
							
							T_RLCS_D TRlcsD; TRlcsD.Initialize();
							m_pDoc->m_pAttrCtrl2->GetRlcs(FlexKB.RCaseK, TRlcsD);
							
							Convert_FlexureElemPos(TRlcsD.strCaseName, bPositiveM, FlexC.RcstB[j], RatElemR.FlexR.FlexB[nEndIdx]);
						}
					}
				}

				if (bIsShearRes==TRUE)
				{
					RatElemR.ShearR.bChk = TRUE;
					T_RCCR_BASE &ShearKB = RcscD.ShearR[j];
					if (ShearKB.ElemK>0)
					{
						T_RLCS_KEY RlcsKey = m_pDoc->m_pPostCtrl->GetStlRlcsRatingKey(ShearKB.RCaseK, ShearKB.bDCmax, ShearKB.bDWmax, ShearKB.bTEmax, ShearKB.nConcurrent);
						
						T_RCST_CASE ShearC;
						pAnalysisResult->GetStlStrengthRating(ElemK, RlcsKey, ShearC);
						
						T_RLCS_D TRlcsD; TRlcsD.Initialize();
						m_pDoc->m_pAttrCtrl2->GetRlcs(ShearKB.RCaseK, TRlcsD);
						
						Convert_ShearElemPos(dVuFactor, TRlcsD.strCaseName, ShearC.RcstB[j], RatElemR.ShearR.ShrB[j]);
					}
				}

				if (bIsStressRes==TRUE)
				{
					RatElemR.StressR.bChk = TRUE;
					for (k=0; k<2; ++k)
					{
						int nIdx=j*3+k+1;
						T_RCCR_BASE &StreKB = RcgsD.StressR[nIdx];
						if (StreKB.ElemK>0)
						{
							T_RLCS_KEY RlcsKey = m_pDoc->m_pPostCtrl->GetStlRlcsRatingKey(StreKB.RCaseK, StreKB.bDCmax, StreKB.bDWmax, StreKB.bTEmax, StreKB.nConcurrent);
							
							int nEndIdx = j*2+k;
							BOOL bIsComp = (k==0);
							
							T_RCSS_CASE StressC;
							pAnalysisResult->GetStlStressRating(ElemK, RlcsKey, StressC);
							
							T_RLCS_D TRlcsD; TRlcsD.Initialize();
							m_pDoc->m_pAttrCtrl2->GetRlcs(StreKB.RCaseK, TRlcsD);
							
							Convert_StressElemPos(TRlcsD.strCaseName, bIsComp, StressC.StreB[j], RatElemR.StressR.StressB[nEndIdx]);
						}
					}
				}

				if (bIsFatiRes==TRUE)
				{
					RatElemR.FatiR.bChk = TRUE;
					for (k=0; k<2; ++k)
					{
						int nIdx=j*2+k;
						T_RCCR_BASE &FatiKB = RcfaD.FatiR[nIdx];
						if (FatiKB.ElemK>0)
						{
							T_RLCS_KEY RlcsKey = m_pDoc->m_pPostCtrl->GetStlRlcsRatingKey(FatiKB.RCaseK, FatiKB.bDCmax, FatiKB.bDWmax, FatiKB.bTEmax, FatiKB.nConcurrent);
							
							
							BOOL bIsTop = (k==0);
							
							T_RCFA_CASE FatiC;
							pAnalysisResult->GetStlFatigueRating(ElemK, RlcsKey, FatiC);
							
							T_RLCS_D TRlcsD; TRlcsD.Initialize();
							m_pDoc->m_pAttrCtrl2->GetRlcs(FatiKB.RCaseK, TRlcsD);
							
							Convert_FatigueElemPos(TRlcsD.strCaseName, bIsTop, FatiC.FatiB[j], RatElemR.FatiR.FatChk[nIdx]);
						}
					}
				}        
				
			}

			RatResRpt.aRatElemR.Add(RatElemR);
		}

		// Summary
		
		CArray<T_RLCS_K,T_RLCS_K> aRlcsKey;
		m_pDoc->m_pAttrCtrl2->GetRlcsKeyList(aRlcsKey);
		int nRlcsKeySize = aRlcsKey.GetSize();

		for (i=0; i<nRlcsKeySize; ++i)
		{
			T_RLCS_K RlcsK = aRlcsKey[i];
			T_RLCS_D RlcsD; RlcsD.Initialize();
			if (!m_pDoc->m_pAttrCtrl2->GetRlcs(RlcsK, RlcsD)) continue;

			CString strMvldName;
			GetMvldLoadCaseName(RlcsD.PrimaryVeh.LoadCaseKey, strMvldName);

			RAT_LRSR_RCASE_B LrsrD;
			LrsrD.nEvaluation  = RlcsD.nEvaluation;
			LrsrD.nRCaseType   = RlcsD.nLimitState;            
			LrsrD.strRCaseName = RlcsD.strCaseName;
			LrsrD.strMVName    = strMvldName;


			for (j=0; j<2; ++j)
			{
				LrsrD.dFactorDCbe[j] = RlcsD.RateLoad[0].dFactor[j];
				LrsrD.dFactorDC[j]   = RlcsD.RateLoad[1].dFactor[j];
				LrsrD.dFactorDW[j]   = RlcsD.RateLoad[2].dFactor[j];
			}
			LrsrD.dFactorTE = RlcsD.RateLoad[3].dFactor[0];
			LrsrD.dFactorTG = RlcsD.RateLoad[4].dFactor[0];
			LrsrD.dFactorP  = RlcsD.RateLoad[5].dFactor[0];
			LrsrD.dFactorSE = RlcsD.RateLoad[6].dFactor[0];
			LrsrD.dFactorUS = RlcsD.RateLoad[7].dFactor[0];
			LrsrD.dFactorPR = RlcsD.PrimaryVeh.Factor;
			LrsrD.dFactorAD = RlcsD.AdjacentVeh.Factor;
			

			if (RlcsD.nLimitState==1) // Strength
			{
				T_RSCR_STRN StrnCR;
				pAnalysisResult->GetStlRCaseStrnElem(RlcsK, StrnCR);
			
				T_RLCS_KEY RlcsKey = m_pDoc->m_pPostCtrl->GetStlRlcsRatingKey(RlcsK, StrnCR.FlexR.bDCmax, StrnCR.FlexR.bDWmax, StrnCR.FlexR.bTEmax, StrnCR.FlexR.nConcurrent);
				
				T_RCST_CASE FlexC;
				ElemPairK EPairFlexK(StrnCR.FlexR.ElemK, StrnCR.FlexR.nBeamType);
				pAnalysisResult->GetStlStrengthRating(EPairFlexK, RlcsKey, FlexC);

				RlcsKey = m_pDoc->m_pPostCtrl->GetStlRlcsRatingKey(RlcsK, StrnCR.ShearR.bDCmax, StrnCR.ShearR.bDWmax, StrnCR.ShearR.bTEmax, StrnCR.ShearR.nConcurrent);

				T_RCST_CASE ShearC;
				ElemPairK EPairShearK(StrnCR.ShearR.ElemK, StrnCR.ShearR.nBeamType);
				pAnalysisResult->GetStlStrengthRating(EPairShearK, RlcsKey, ShearC);


				Convert_FlexureElem4RCase(FlexC.RcstB[StrnCR.FlexR.nPos], LrsrD.FlexD);
				//
				Convert_ShearElem4RCase(ShearC.RcstB[StrnCR.ShearR.nPos], LrsrD.ShearD);

				LrsrD.dWeight = FlexC.RcstB[StrnCR.FlexR.nPos].dRT;

			}
			else if (RlcsD.nLimitState==0) // Service
			{
				T_RSCR_SERV ServCR;
				pAnalysisResult->GetStlRCaseServElem(RlcsK, ServCR);

				T_RLCS_KEY RlcsKey = m_pDoc->m_pPostCtrl->GetStlRlcsRatingKey(RlcsK, ServCR.StressR.bDCmax, ServCR.StressR.bDWmax, ServCR.StressR.bTEmax, ServCR.StressR.nConcurrent);
				
				T_RCSS_CASE RcssC;
				ElemPairK EPairServK(ServCR.StressR.ElemK, ServCR.StressR.nBeamType);
				pAnalysisResult->GetStlStressRating(EPairServK, RlcsKey, RcssC);

				Convert_StressElem4RCase(RcssC.StreB[ServCR.StressR.nPos], LrsrD.StressD[0]);
				LrsrD.dWeight = RcssC.StreB[ServCR.StressR.nPos].dRT[0];

			}
			else if (RlcsD.nLimitState==2) // Fatigue
			{
				T_RSCR_FATI FatiCR;
				pAnalysisResult->GetStlRCaseFatiElem(RlcsK, FatiCR);

				T_RLCS_KEY RlcsKey = m_pDoc->m_pPostCtrl->GetStlRlcsRatingKey(RlcsK, FatiCR.FatiR.bDCmax, FatiCR.FatiR.bDWmax, FatiCR.FatiR.bTEmax, FatiCR.FatiR.nConcurrent);

				T_RCFA_CASE FatiC;
				ElemPairK EPairFatiK(FatiCR.FatiR.ElemK, FatiCR.FatiR.nBeamType);
				pAnalysisResult->GetStlFatigueRating(EPairFatiK, RlcsKey, FatiC);
								
				LrsrD.dWeight = FatiC.FatiB[FatiCR.FatiR.nPos].dRT;
			}

			if      (RlcsD.nEvaluation==0) RatResRpt.LrsrD.DesignLR.Add(LrsrD);
			else if (RlcsD.nEvaluation==1) RatResRpt.LrsrD.LegalLR.Add(LrsrD);
		}   
		

			
		// 계산서 출력
		CDgn_RatingRptManager myReport;
		
		CString strDataPath = m_pDoc->GetPathName();
		CString strBasePath = m_pDoc->GetProgramPath() + _T("Excel Base File\\");

		RatResRpt.nDgnCode = iDgnCode;
		RatResRpt.strBasePath = strBasePath;
		RatResRpt.strDataPath = strDataPath;
		RatResRpt.strPath     = strPath;
		RatResRpt.nPrintOpt   = iPrintOpt;
		
		myReport.Print_STLRatingAASHTO(strBasePath, strDataPath, strPath, RatResRpt);
		//myReport.Print_RFReport(strBasePath, strDataPath, strPath, LcomInfo, OptionData, arStrRptSectD, aRptData, aRptCsdInData);
	}
	catch(...)
	{
		bSuccess = FALSE;	
	}

	return bSuccess;
}


void CRptRatingSTL_AASHTO_LRFR11::Convert_FlexureElemPos(CString strRcaseName, BOOL bPositiveM, T_RCST_BASE &RcstB, RAT_SFLE_BASE &rData)
{
	rData.bChk  = RcstB.bChk;
	rData.nElem = RcstB.ElemK;
	rData.nPart = RcstB.nPosi;
	rData.RcaseD.bDCMax   = RcstB.bDCmax;
	rData.RcaseD.bDWMax   = RcstB.bDWmax;
	rData.RcaseD.bTEmax   = RcstB.bTEmax;
	rData.RcaseD.nConcurr = RcstB.nConcurrent;
	rData.bIsPositive  =bPositiveM;
	rData.strRcaseName = strRcaseName;
	rData.dpMn_CA = RcstB.dpMn_CA;
	rData.dpFn_CA = RcstB.dpFn_CA;
	rData.dMu_DE  = RcstB.ForSUM.dForce[4];
	rData.dfbu_DE = RcstB.dfbu_SUM;
	rData.dMu_PR  = RcstB.ForPR.dForce[4]*RcstB.drPR;
	rData.dfbu_PR = RcstB.dfbu_PR;
	rData.nCaseRes= RcstB.nCaseRes;
	rData.dRF     = RcstB.dRF;
	rData.dKRF    = RcstB.dKRF;
	rData.dSF     = RcstB.dRT;
	
	rData.DtrK.dKa    = RcstB.DtkF.dKa;
	rData.DtrK.dKb    = RcstB.DtkF.dKb;
	rData.DtrK.dK     = RcstB.DtkF.dK;  
	rData.DtrK.dEpsiC = RcstB.DtkF.dEpsiC;
	rData.DtrK.dEpsiT = RcstB.DtkF.dEpsiT;

}

void CRptRatingSTL_AASHTO_LRFR11::Convert_ShearElemPos(double dVuFactor, CString strRcaseName, T_RCST_BASE &RcstB, RAT_SSHR_BASE &rData)
{
	rData.bChk  = RcstB.bChk;
	rData.nElem = RcstB.ElemK;
	rData.nPart = RcstB.nPosi;
	rData.RcaseD.bDCMax   = RcstB.bDCmax;
	rData.RcaseD.bDWMax   = RcstB.bDWmax;
	rData.RcaseD.bTEmax   = RcstB.bTEmax;
	rData.RcaseD.nConcurr = RcstB.nConcurrent;
	rData.strRcaseName = strRcaseName;
	rData.dVr_CA = RcstB.dVr_CA;
	rData.dVu_DE  = dVuFactor * RcstB.ForSUM.dForce[2];
	rData.dVu_PR  = dVuFactor * RcstB.ForPR.dForce[2]*RcstB.drPR;
	rData.dRF     = RcstB.dRFsh;
	rData.dKRF    = RcstB.dKRFsh;
	rData.dSF     = RcstB.dRT;
	
	rData.DtrK.dKa    = RcstB.DtkS.dKa;
	rData.DtrK.dKb    = RcstB.DtkS.dKb;
	rData.DtrK.dK     = RcstB.DtkS.dK;  
	rData.DtrK.dEpsiC = RcstB.DtkS.dEpsiC;
	rData.DtrK.dEpsiT = RcstB.DtkS.dEpsiT;
	
}

void CRptRatingSTL_AASHTO_LRFR11::Convert_StressElemPos(CString strRcaseName, BOOL bIsComp, T_RCSS_BASE &RcssB, RAT_SSTR_BASE &rData)
{
	rData.bChk  = RcssB.bChk;
	rData.nElem = RcssB.ElemK;
	rData.nPart = RcssB.nPosi;
	rData.RcaseD.bDCMax   = RcssB.bDCmax;
	rData.RcaseD.bDWMax   = RcssB.bDWmax;
	rData.RcaseD.bTEmax   = RcssB.bTEmax;
	rData.RcaseD.nConcurr = RcssB.nConcurrent;
	rData.strRcaseName = strRcaseName;  
	memcpy(rData.dStress_AL, RcssB.dStress_AL, sizeof(rData.dStress_AL));
	rData.StrDE.dfcw    = RcssB.StrSUM.dfcw;
	rData.StrDE.dfcf    = RcssB.StrSUM.dfcf;
	rData.StrDE.dftf    = RcssB.StrSUM.dftf;
	rData.StrDE.bIsTopC = RcssB.StrSUM.bIsTopC;
	rData.StrPR.dfcw    = RcssB.StrPR.dfcw;
	rData.StrPR.dfcf    = RcssB.StrPR.dfcf;
	rData.StrPR.dftf    = RcssB.StrPR.dftf;
	rData.StrPR.bIsTopC = RcssB.StrPR.bIsTopC;
	rData.dRF     = (bIsComp==TRUE) ? RcssB.dRF[1] : RcssB.dRF[2];
	rData.dKRF    = (bIsComp==TRUE) ? RcssB.dKRF[1] : RcssB.dKRF[2];
	rData.dSF     = RcssB.dRT[0];
	
	T_RFDT_B &DtkS = (bIsComp==TRUE) ? RcssB.DtkS[1] : RcssB.DtkS[2];
	rData.DtrK.dKa    = DtkS.dKa;
	rData.DtrK.dKb    = DtkS.dKb;
	rData.DtrK.dK     = DtkS.dK;  
	rData.DtrK.dEpsiC = DtkS.dEpsiC;
	rData.DtrK.dEpsiT = DtkS.dEpsiT;
	
}

void CRptRatingSTL_AASHTO_LRFR11::Convert_FatigueElemPos(CString strRcaseName, BOOL bIsTop, T_RCFA_BASE &RcfaB, RAT_FATI_BASE &rData)
{
	rData.bChk  = RcfaB.bChk;
	rData.nElem = RcfaB.ElemK;
	rData.nPart = RcfaB.nPosi;
	rData.RcaseD.bDCMax   = RcfaB.bDCmax;
	rData.RcaseD.bDWMax   = RcfaB.bDWmax;
	rData.RcaseD.bTEmax   = RcfaB.bTEmax;
	rData.RcaseD.nConcurr = RcfaB.nConcurrent;
	rData.strRcaseName = strRcaseName;  
	T_RCFA_LOAD &Load = (bIsTop) ? RcfaB.LoadR[0] : RcfaB.LoadR[1];
	rData.dDelF_n       = Load.dDelF_n;
	rData.dpDelF_n      = Load.dpDelF_n;
	rData.dfbu_DE       = Load.dfbuSum;
	rData.dGammaDelF_PR = Load.dGammaDelF;  
	rData.dRF           = (bIsTop) ? RcfaB.dRF_Load[0] : RcfaB.dRF_Load[1];
	rData.dKRF          = (bIsTop) ? RcfaB.dKRF_Load[0] : RcfaB.dKRF_Load[1];
	rData.dSF           = RcfaB.dRT;
	
	T_RFDT_B &DtkS = (bIsTop==TRUE) ? RcfaB.DtkS[0] : RcfaB.DtkS[1];
	rData.DtrK.dKa    = DtkS.dKa;
	rData.DtrK.dKb    = DtkS.dKb;
	rData.DtrK.dK     = DtkS.dK;  
	rData.DtrK.dEpsiC = DtkS.dEpsiC;
	rData.DtrK.dEpsiT = DtkS.dEpsiT;  

    //T_RATING_FATI_LIFE &FatiLife =  (bIsTop) ? RcfaB.FatiLife[0] : RcfaB.FatiLife[1];
    //rData.FatiLife.dDel_Fact_f_Tens;
    //rData.FatiLife.dDel_f_D_Comp;
    //rData.FatiLife.dDel_feff;
    //rData.FatiLife.dDel_fmax;
    //rData.FatiLife.dDel_FH;
    //rData.FatiLife.nFatiLifeCalcType;
    //rData.FatiLife.nCategory;
    //rData.FatiLife.nFatiLifeType;
    //rData.FatiLife.dRr;
    //rData.FatiLife.dA;
    //rData.FatiLife.dNav;
    //rData.FatiLife.dPresentLife;
    //rData.FatiLife.dADTT_SL_0;
    //rData.FatiLife.dADTT_SL_Present;
    //rData.FatiLife.dn;
    //rData.FatiLife.dN1;
    //rData.FatiLife.dg;
    //rData.FatiLife.bRemainLife;
    //rData.FatiLife.dY_Rem;
    //rData.FatiLife.dADTT_SL_Future;
    //rData.FatiLife.dADTT_SL_Limit;
    //rData.FatiLife.dY_ADTT_Limit;
    //rData.FatiLife.dY_Rem_Mod;
    //rData.FatiLife.dY;
}

void CRptRatingSTL_AASHTO_LRFR11::Convert_FlexureElem4RCase(T_RCST_BASE &RcstB, RAT_LRSR_FLEX_RCASE &rData)
{
	rData.nElem          = RcstB.ElemK;
	rData.nPos           = RcstB.nPosi;
	rData.dScaleFac4Test = RcstB.DtkF.dK;
	rData.dSafeCapa      = RcstB.dSLCapa;
	rData.dMuDe          = RcstB.drPR*RcstB.ForPR.dForce[4];//RcstB.ForSUM.dForce[4];    
	rData.dfbuDe         = RcstB.dfbu_PR;//RcstB.dfbu_SUM;
	rData.nCaseRes       = RcstB.nCaseRes;
	rData.dRF            = RcstB.dRF;
	rData.dKRF           = RcstB.dKRF;

}

void CRptRatingSTL_AASHTO_LRFR11::Convert_ShearElem4RCase(T_RCST_BASE &RcstB, RAT_LRSR_SHEAR_RCASE &rData)
{
	rData.nElem          = RcstB.ElemK;
	rData.nPos           = RcstB.nPosi;
	rData.dScaleFac4Test = RcstB.DtkS.dK;
	rData.dSafeCapa      = RcstB.dSLCapaSh;
	rData.dVuDe          = RcstB.drPR*RcstB.ForPR.dForce[2];//RcstB.ForSUM.dForce[2];
	rData.dRF            = RcstB.dRFsh;
	rData.dKRF           = RcstB.dKRFsh;
}

void CRptRatingSTL_AASHTO_LRFR11::Convert_StressElem4RCase(T_RCSS_BASE &RcssB, RAT_LRSR_STRESS_RCASE &rData)
{
	rData.nElem          = RcssB.ElemK;
	rData.nPos           = RcssB.nPosi;
	rData.dScaleFac4Test = RcssB.DtkS[RcssB.nCrMinType].dK;
	rData.dSafeCapa      = RcssB.dSLCapa[RcssB.nCrMinType];

	double dStrDe=0.0;
//   if     (RcssB.nCrMinType==0) dStrDe = RcssB.StrSUM.dfcw;
//   else if(RcssB.nCrMinType==1) dStrDe = RcssB.StrSUM.dfcf;
//   else if(RcssB.nCrMinType==2) dStrDe = RcssB.StrSUM.dftf;
	if     (RcssB.nCrMinType==0) dStrDe = RcssB.StrPR.dfcw;
	else if(RcssB.nCrMinType==1) dStrDe = RcssB.StrPR.dfcf;
	else if(RcssB.nCrMinType==2) dStrDe = RcssB.StrPR.dftf;
	rData.dStressDe      = dStrDe;
	rData.dRF            = RcssB.dRF[RcssB.nCrMinType];
	rData.dKRF           = RcssB.dKRF[RcssB.nCrMinType];

}

BOOL CRptRatingSTL_AASHTO_LRFR11::GetMvldLoadCaseName(T_MVLD_K MvldK, CString &strName)
{
	T_MVCD_D MvcdD;
	if(!m_pDoc->m_pAttrCtrl->GetMvcd(MvcdD)) return FALSE;
	switch(MvcdD.nCodeType)
	{
	case D_MOVE_CODE_NONE:
		break;
	case D_MOVE_CODE_KOREA:
	case D_MOVE_CODE_AASHTO_STAN:
	case D_MOVE_CODE_AASHTO_LRFD:
	case D_MOVE_CODE_TAIWAN:
	case D_MOVE_CODE_CANADA:
	case D_MOVE_CODE_PENDOT:
	case D_MOVE_CODE_EURO_BS:
	case D_MOVE_CODE_RUSSIA:
	case D_MOVE_CODE_KOREA_LRFD_2011:
	case D_MOVE_CODE_AUSTRALIA:
	case D_MOVE_CODE_POLAND:
	case D_MOVE_CODE_SOUTH_AFRICA:
	case D_MOVE_CODE_NEWZEALAND:
	case D_MOVE_CODE_BRAZIL:
		{
			T_MVLD_D MvldD;
			if(!m_pDoc->m_pAttrCtrl->GetMvld(MvldK, MvldD)) break;
			strName = MvldD.LoadCaseName;
		}
		break; 
	case D_MOVE_CODE_JAPAN:
		{
			T_MVLDjp_D MvldJPD;
			m_pDoc->m_pAttrCtrl->InitializeMvldjp(MvldJPD);            
			if(!m_pDoc->m_pAttrCtrl->GetMvldjp(MvldK, MvldJPD)) break;
			strName = MvldJPD.LoadCaseName;
		}
		break;
	case D_MOVE_CODE_CHINA:
		{
			T_MVLDch_D MvldCHD;				  
			if(!m_pDoc->m_pAttrCtrl->GetMvldch(MvldK, MvldCHD)) break;
			strName = MvldCHD.LoadCaseName;
		}
		break;
	case D_MOVE_CODE_INDIA:
		{
			T_MVLDid_D MvldIDD;
			if(!m_pDoc->m_pAttrCtrl->GetMvldid(MvldK, MvldIDD)) break;
			strName = MvldIDD.LoadCaseName;				  
		}
		break;
	case D_MOVE_CODE_BS:
		{
			T_MVLDbs_D MvldBSD;				  
			if(!m_pDoc->m_pAttrCtrl->GetMvldbs(MvldK, MvldBSD)) break;
		  strName = MvldBSD.LoadCaseName;
		}
		break;
	case D_MOVE_CODE_FRANCE:
		{
			T_MVLDfr_D MvldfrD;				  
			if(!m_pDoc->m_pAttrCtrl->GetMvldfr(MvldK, MvldfrD)) break;
		  strName = MvldfrD.LoadCaseName;
		}
		break;
	case D_MOVE_CODE_TRANS:
		{
			T_MVLDtr_D MvldTRD;				  
			if(!m_pDoc->m_pAttrCtrl->GetMvldtr(MvldK, MvldTRD)) break;
		  strName = MvldTRD.LoadCaseName;
		}
		break;
	default: 
		ASSERT(0); break;
	}

	return TRUE;
}