#include "stdafx.h"
#include "RptRatingPSC_AASHTO_LRFR12.h"

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

CRptRatingPSC_AASHTO_LRFR12::CRptRatingPSC_AASHTO_LRFR12()
{
	m_pDoc = CDBDoc::GetDocPoint(); ASSERT(m_pDoc);
	m_bStopExecute = FALSE;

    m_pDoc->SetCivilCodeDgn(AASHTO_LRFD12_PSC_RATING);
	m_pDataCtrl = new CCRCDataCtrl();

	m_pForcCtrl = m_pDataCtrl->Get_ForceCtrlPointer();
	if(m_pForcCtrl==NULL) {ASSERT(0); return;}

}

CRptRatingPSC_AASHTO_LRFR12::~CRptRatingPSC_AASHTO_LRFR12()
{
	if(m_pDataCtrl)
	{
		delete m_pDataCtrl;
		m_pDataCtrl = NULL;
	}
}

BOOL CRptRatingPSC_AASHTO_LRFR12::Execute_RatingReport(int iDgnCode, CString strPath, int iPrintOpt, BOOL bSaveImage)
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

	T_BLRS_D BlrsD; BlrsD.Initialize();  
	m_pDoc->m_pAttrCtrl->GetBlrs(BlrsD);

	// Print항목이 없을 경우 강제로 ProgressBar를 닫도록 조치!!
	CArray<T_RKPO_K, T_RKPO_K> aPrintElemKey;  
	m_pDoc->m_pAttrCtrl->GetRkpoKeyList(aPrintElemKey);

	CArray<T_RKPV_K, T_RKPV_K> aPrintVBeamKey;
	m_pDoc->m_pAttrCtrl->GetRkpvKeyList(aPrintVBeamKey);

	int nPrintElemSize = aPrintElemKey.GetSize(); 
	int nPrintVBeamSize = aPrintVBeamKey.GetSize();

	ArrElemPairKey aPrintElemPairK;
	CDBLib::ConvertToElemPairKeyList(aPrintElemKey, aPrintVBeamKey, aPrintElemPairK);

	if(nPrintElemSize==0 && nPrintVBeamSize==0)
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
	//BOOL bComposite = 0;
	CDesignLoadRating *pCheckGirder = m_pDoc->m_pPostCtrl->GetLoadRating();

	try
	{
		RAT_LRAT_RPT RatResRpt;
		RatResRpt.Initialize();
		RatResRpt.nMeasureType =  BlrsD.iDiagnostic_Method;

		for(i=0; i< aPrintElemPairK.GetSize(); ++i) // Repeat Element
		{
			auto  ElemK = aPrintElemPairK[i];
			BOOL bIsCpt     = m_pDataCtrl->IsCompositePSCSect(ElemK);
			//if (bIsCpt) bComposite = TRUE;
			T_RKPO_D RkpoD; RkpoD.Initialize();  // 출력 위치
			if (ElemK.second == EN_EL_BEAM)
			{
				if (!m_pDoc->m_pAttrCtrl->GetRkpo(ElemK.first, RkpoD)) continue;

				T_ELEM_D ElemD; ElemD.Initialize();
				if (!m_pDoc->m_pAttrCtrl->GetElem(ElemK.first, ElemD)) continue;
			}
			else if (ElemK.second == EN_EL_VBEAM)
			{
				if (!m_pDoc->m_pAttrCtrl->GetRkpv(ElemK.first, RkpoD)) continue;
			}
			else ASSERT(0);
			

			BOOL bPrintI = (RkpoD.iPrintChk == 0 || RkpoD.iPrintChk == 2);
			BOOL bPrintJ = (RkpoD.iPrintChk == 1 || RkpoD.iPrintChk == 2);


			// Repeat I & J
			RAT_ELEM_RES RatElemR;
			RatElemR.nElem = ElemK.first;
			RatElemR.nPrtOpt = RkpoD.iPrintChk;

			for(int nIJ = 0; nIJ < 2; ++nIJ)
			{
				if(nIJ==0 && (!bPrintI)) continue;
				if(nIJ==1 && (!bPrintJ)) continue;

				T_BLRC_D TBlrcD;
				//////////////////////////////////////////////////////////////////////////
				// Flexure Rating Case Result
				//////////////////////////////////////////////////////////////////////////
				T_RTBC_CASE RtbcD;// Civil Structure
				//T_RTBC_BASE RtbcBaseD; // Civil Structure
				RAT_SFLE_BASE FlexDgnResD[2]; // DgnEngine Structure, 0 = Positive , 1 = Negative

				BOOL bDC_Max=TRUE,bDW_Max=TRUE,bTemp_Max=TRUE;
				int iConcurr=0;

				for(int iMaxMin = 1; iMaxMin < 3; ++iMaxMin) // 1 : Positive , 2 :  Negative
				{
					if(pAnalysisResult->GetFlexuralStrengthRatingData(ElemK, 0,iMaxMin,bDC_Max,bDW_Max,bTemp_Max,iConcurr,RtbcD))
					{
						TBlrcD.Initialize();
						m_pDoc->m_pAttrCtrl->GetBlrc(RtbcD.RtbcBase[nIJ].RCaseK, TBlrcD);          
						Convert_FlexureElemPos(TBlrcD.RatingCaseName, (RtbcD.RtbcBase[nIJ].nMoment == 1), RtbcD.RtbcBase[nIJ], FlexDgnResD[iMaxMin-1]);
					}
				}

				//////////////////////////////////////////////////////////////////////////
				// Shear Rating Case Result
				//////////////////////////////////////////////////////////////////////////
				T_RTSC_CASE RtscCaseD; // Civil Structure
				RAT_SSHR_BASE ShearDgnResD; // DgnEngine Structure
				int iMaxMin=1;
				if(pAnalysisResult->GetShearStrengthRatingData(ElemK, 0,iMaxMin,bDC_Max,bDW_Max,bTemp_Max,iConcurr,RtscCaseD))
				{
					TBlrcD.Initialize();
					m_pDoc->m_pAttrCtrl->GetBlrc(RtscCaseD.RtscBase[nIJ].RCaseK, TBlrcD);   
					Convert_ShearElemPos(TBlrcD.RatingCaseName, RtscCaseD.RtscBase[nIJ], ShearDgnResD);
				}

				//////////////////////////////////////////////////////////////////////////
				// Stress Rating Case Result
				//////////////////////////////////////////////////////////////////////////
				T_RTST_CASE RtstCaseD_G, RtstCaseD_S; // Girder & Slab
				RAT_SSTR_BASE StressDgnResD_G[2], StressDgnResD_S[2];
				bDC_Max=TRUE,bDW_Max=TRUE,bTemp_Max=TRUE,iConcurr=0;
				for(int iMaxMin = 1; iMaxMin < 3; ++iMaxMin) // 1 : Compression , 2 :  Tension
				{
					// Girder
					if(pAnalysisResult->GetStressRatingData(ElemK, 0,iMaxMin,bDC_Max,bDW_Max,bTemp_Max,iConcurr, RtstCaseD_G))
					{
						TBlrcD.Initialize();
						m_pDoc->m_pAttrCtrl->GetBlrc(RtstCaseD_G.RtstBase[nIJ].RCaseK, TBlrcD);          
						Convert_StressElemPos(TBlrcD.RatingCaseName, (iMaxMin == 1), RtstCaseD_G.RtstBase[nIJ], StressDgnResD_G[iMaxMin-1]);
					}

					// Slab
					if(bIsCpt)
					{
						if(pAnalysisResult->GetSlabStressRatingData(ElemK, 0,iMaxMin,bDC_Max,bDW_Max,bTemp_Max,iConcurr, RtstCaseD_S))
						{
							TBlrcD.Initialize();
							m_pDoc->m_pAttrCtrl->GetBlrc(RtstCaseD_S.RtstBase[nIJ].RCaseK, TBlrcD);          
							Convert_StressElemPos(TBlrcD.RatingCaseName, (iMaxMin == 1), RtstCaseD_S.RtstBase[nIJ], StressDgnResD_S[iMaxMin-1]);
						}
					}
				}
				//////////////////////////////////////////////////////////////////////////
				// Save Result
				//////////////////////////////////////////////////////////////////////////
				//Flexure
				for(int nPN = 0; nPN < 2; ++nPN) // Positive, Negative
				{
					RatElemR.FlexR.FlexB[nIJ * 2 + nPN] = FlexDgnResD[nPN];
				}
				if (RatElemR.FlexR.FlexB[0].bChk || RatElemR.FlexR.FlexB[1].bChk || RatElemR.FlexR.FlexB[2].bChk || RatElemR.FlexR.FlexB[3].bChk)
				{
					RatElemR.FlexR.bChk=TRUE;
				}
				//Shear 
				RatElemR.ShearR.ShrB[nIJ] = ShearDgnResD;
				if (RatElemR.ShearR.ShrB[0].bChk || RatElemR.ShearR.ShrB[1].bChk)
				{
					RatElemR.ShearR.bChk=TRUE;
				}
				//Stress for Girder
				for(int nCT = 0; nCT < 2; ++nCT) // Compression, Tension
				{
					RatElemR.StressR.StressB[nIJ * 2 + nCT] = StressDgnResD_G[nCT];
				}
				if (RatElemR.StressR.StressB[0].bChk || RatElemR.StressR.StressB[1].bChk || RatElemR.StressR.StressB[2].bChk || RatElemR.StressR.StressB[3].bChk)
				{
					RatElemR.StressR.bChk=TRUE;
				}
				//Stress for Slab
				for(int nCT = 0; nCT < 2; ++nCT) // Compression, Tension
				{
					RatElemR.StressR.StressB_Slab[nIJ * 2 + nCT] = StressDgnResD_S[nCT];
				}
				if (RatElemR.StressR.StressB_Slab[0].bChk || RatElemR.StressR.StressB_Slab[1].bChk || RatElemR.StressR.StressB_Slab[2].bChk || RatElemR.StressR.StressB_Slab[3].bChk)
				{
					RatElemR.StressR.bChk_Slab=TRUE;
				}
			}
			RatResRpt.aRatElemR.Add(RatElemR);
		}
		// Summary

		CArray<T_BLRC_K,T_BLRC_K> aBlrcKey;
		m_pDoc->m_pAttrCtrl->GetBlrcKeyList(aBlrcKey);
		int nBlrcKeySize = aBlrcKey.GetSize();

		for (i=0; i<nBlrcKeySize; ++i)
		{
			T_BLRC_K BlrcK = aBlrcKey[i];
			T_BLRC_D BlrcD; BlrcD.Initialize();
			if (!m_pDoc->m_pAttrCtrl->GetBlrc(BlrcK, BlrcD)) continue;
		 
			ArrElemPairKey arBlrDgnElement;			
			pAnalysisResult->GetBlrDgnElemData(arBlrDgnElement);
			int iTotalElem = arBlrDgnElement.GetSize();
			BOOL bIsCpt=0;
			for(int nTotalElem=0; nTotalElem<iTotalElem; nTotalElem++)
			{
				auto ElemK = arBlrDgnElement[nTotalElem];
				bIsCpt     = m_pDataCtrl->IsCompositePSCSect(ElemK);
			}
			CString strMvldName;
			GetMvldLoadCaseName(BlrcD.PrimaryVeh.LoadCaseKey, strMvldName); // ??

			RAT_LRSR_RCASE_B LrsrD;
            if ( BlrcD.iEvaluation == 2 ) BlrcD.iEvaluation = 1;
			LrsrD.nEvaluation  = BlrcD.iEvaluation; //RklcD.nEvaluation;
			LrsrD.nRCaseType   = BlrcD.iLimitState;            
			LrsrD.strRCaseName = BlrcD.RatingCaseName;
			LrsrD.strMVName    = strMvldName;
			LrsrD.nComposite   = bIsCpt; //0:PSC 1:PSC Composite

			for (j=0; j<2; ++j)
			{
				LrsrD.dFactorDCbe[j] = 0.0; //RklcD.RateLoad[0].dFactor[j];
				LrsrD.dFactorDC[j]   = BlrcD.RateLoad[0].dFactor[j];
				LrsrD.dFactorDW[j]   = BlrcD.RateLoad[1].dFactor[j];
			}
			LrsrD.dFactorTE = 0.0; //RlcsD.RateLoad[2].dFactor[0];
			LrsrD.dFactorTG = 0.0; //RlcsD.RateLoad[4].dFactor[0];
			LrsrD.dFactorP  = 0.0; //RlcsD.RateLoad[5].dFactor[0];
			LrsrD.dFactorSE = 0.0; //RlcsD.RateLoad[6].dFactor[0];
			LrsrD.dFactorUS = 0.0; //RlcsD.RateLoad[7].dFactor[0];
			LrsrD.dFactorPR = BlrcD.PrimaryVeh.Factor;
			LrsrD.dFactorAD = BlrcD.AdjacentVeh.Factor;


			if (BlrcD.iLimitState==1) // Strength
			{
				T_RTBC_BASE FlexStrnCR;
				T_RTSC_BASE ShearStrnCR;
				pAnalysisResult->GetFlexuralStrengthRatingMinRF_Data(BlrcK, FlexStrnCR);
				pAnalysisResult->GetShearStrengthRatingMinRF_Data(BlrcK, ShearStrnCR);
				Convert_FlexureElem4RCase(FlexStrnCR, LrsrD.FlexD);
				Convert_ShearElem4RCase(ShearStrnCR, LrsrD.ShearD);

				LrsrD.dWeight = FlexStrnCR.dWeight;
			}
			else if (BlrcD.iLimitState==0) // Service
			{
				T_RTST_BASE CompServCR_G, TensServCR_G;
				int nGirder=0;
				int nSlab=1;
				int nRFCTmin=0; // Comp : 0 , Tens : 1
				pAnalysisResult->GetStressRatingMinRF_Data(nGirder, BlrcK, CompServCR_G, TensServCR_G, nRFCTmin); //nPart = 0 : Girder 1: Slab
				if (nRFCTmin==0)
				{
					Convert_StressElem4RCase(CompServCR_G, LrsrD.StressD[nGirder], nRFCTmin);
					LrsrD.dWeight = CompServCR_G.dWeight;
				}
				else if (nRFCTmin==1)
				{
					Convert_StressElem4RCase(TensServCR_G, LrsrD.StressD[nGirder], nRFCTmin);
					LrsrD.dWeight = TensServCR_G.dWeight;
				}
				else ASSERT(0);

				if (bIsCpt) // Composite 단면 일때만 수행하도록!
				{
					T_RTST_BASE CompServCR_S, TensServCR_S;
					int nRFCTmin_Slab=0; // Comp : 0 , Tens : 1
					pAnalysisResult->GetStressRatingMinRF_Data(nSlab,   BlrcK, CompServCR_S, TensServCR_S, nRFCTmin_Slab); //nPart = 0 : Girder 1: Slab
					if (nRFCTmin_Slab==0)
					{
						Convert_StressElem4RCase(CompServCR_S, LrsrD.StressD[nSlab], nRFCTmin_Slab);
						LrsrD.dWeight = CompServCR_S.dWeight;
					}
					else if (nRFCTmin_Slab==1)
					{
						Convert_StressElem4RCase(TensServCR_S, LrsrD.StressD[nSlab], nRFCTmin_Slab);
						LrsrD.dWeight = TensServCR_S.dWeight;
					}
					else ASSERT(0);
				}

			}
			if      (BlrcD.iEvaluation==0) RatResRpt.LrsrD.DesignLR.Add(LrsrD);
			else if (BlrcD.iEvaluation==1) RatResRpt.LrsrD.LegalLR.Add(LrsrD);
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
		myReport.Print_PSCRatingAASHTO(strBasePath, strDataPath, strPath, RatResRpt);
		//myReport.Print_RFReport(strBasePath, strDataPath, strPath, LcomInfo, OptionData, arStrRptSectD, aRptData, aRptCsdInData);

	}
	catch(...)
	{
		bSuccess = FALSE;	
	}

return bSuccess;
}


void CRptRatingPSC_AASHTO_LRFR12::Convert_FlexureElemPos(CString strRcaseName, BOOL bPositiveM, T_RTBC_BASE &RtbcB, RAT_SFLE_BASE &rData)
{
	double dCF = RtbcB.dPhi_CF;
	double dSF = RtbcB.dPhi_SF;
	double dCapacityF=dCF*dSF;
	if(dCapacityF<0.85) dCapacityF = 0.85;

 rData.bChk  = RtbcB.bChk;
 rData.nElem = RtbcB.ElemK;
 rData.nPart = RtbcB.nPosi;
 rData.RcaseD.bDCMax   = RtbcB.bDCmax;
 rData.RcaseD.bDWMax   = RtbcB.bDWmax;
 rData.RcaseD.bTEmax   = RtbcB.bTEmax;
 rData.RcaseD.nConcurr = RtbcB.nConcurrent;
 rData.bIsPositive  =bPositiveM;
 rData.strRcaseName = strRcaseName;
 rData.dpMn_CA = dCapacityF*RtbcB.dPhi_RE*RtbcB.dMn_CA;
 rData.dpFn_CA = 0.0;
 rData.dMu_DE  = RtbcB.dMu_SUM;
 rData.dfbu_DE = 0.0;
 rData.dMu_PR  = RtbcB.drPR*RtbcB.dMu_PR;
 rData.dfbu_PR = 0.0;
 rData.nCaseRes= 0;
 rData.dRF     = RtbcB.dRF;
 rData.dKRF    = RtbcB.dKRF;
 rData.dSF     = RtbcB.dRT;

 rData.DtrK.dKa    = RtbcB.DtkF.dKa;
 rData.DtrK.dKb    = RtbcB.DtkF.dKb;
 rData.DtrK.dK     = RtbcB.DtkF.dK;  
 rData.DtrK.dEpsiC = RtbcB.DtkF.dEpsiC;
 rData.DtrK.dEpsiT = RtbcB.DtkF.dEpsiT;

}

void CRptRatingPSC_AASHTO_LRFR12::Convert_ShearElemPos(CString strRcaseName, T_RTSC_BASE &RtscB, RAT_SSHR_BASE &rData)
{
	double dCF = RtscB.dPhi_CF;
	double dSF = RtscB.dPhi_SF;
	double dCapacityF=dCF*dSF;
	if(dCapacityF<0.85) dCapacityF = 0.85;
	
	rData.bChk  = RtscB.bChk;
	rData.nElem = RtscB.ElemK;
	rData.nPart = RtscB.nPosi;
	rData.RcaseD.bDCMax   = RtscB.bDCmax;
	rData.RcaseD.bDWMax   = RtscB.bDWmax;
	rData.RcaseD.bTEmax   = RtscB.bTEmax;
	rData.RcaseD.nConcurr = RtscB.nConcurrent;
	rData.strRcaseName = strRcaseName;
	rData.dVr_CA = dCapacityF*RtscB.dPhi_RE*RtscB.dVn_CA;
	rData.dVu_DE  = fabs(RtscB.dVu_SUM);
	rData.dVu_PR  = fabs(RtscB.drPR*RtscB.dVu_PR);
	rData.dRF     = RtscB.dRF;
	rData.dKRF    = RtscB.dKRF;
	rData.dSF     = RtscB.dRT;

	rData.DtrK.dKa    = RtscB.DtkS.dKa;
	rData.DtrK.dKb    = RtscB.DtkS.dKb;
	rData.DtrK.dK     = RtscB.DtkS.dK;  
	rData.DtrK.dEpsiC = RtscB.DtkS.dEpsiC;
	rData.DtrK.dEpsiT = RtscB.DtkS.dEpsiT;

}

void CRptRatingPSC_AASHTO_LRFR12::Convert_StressElemPos(CString strRcaseName, BOOL bIsComp, T_RTST_BASE &RtstB, RAT_SSTR_BASE &rData)
{
	rData.bChk  = RtstB.bChk;
	rData.nElem = RtstB.ElemK;
	rData.nPart = RtstB.nPosi;
	rData.RcaseD.bDCMax   = RtstB.bDCmax;
	rData.RcaseD.bDWMax   = RtstB.bDWmax;
	rData.RcaseD.bTEmax   = RtstB.bTEmax;
	rData.RcaseD.nConcurr = RtstB.nConcurrent;
	rData.strRcaseName = strRcaseName;  
	memcpy(rData.dStress_AL, RtstB.dStress_AL, sizeof(rData.dStress_AL));
	int nIsCompTens = (bIsComp ==TRUE) ? nIsCompTens = 0 : nIsCompTens = 1;
	rData.StrDE.dfcw    = 0.0; //RcssB.StrSUM.dfcw;
	rData.StrDE.dfcf    = RtstB.dStress_SUM[RtstB.nPoint[nIsCompTens]];
	rData.StrDE.dftf    = RtstB.dStress_SUM[RtstB.nPoint[nIsCompTens]];
	rData.StrDE.bIsTopC = 0.0; //RcssB.StrSUM.bIsTopC;
	rData.StrPR.dfcw    = 0.0; //RcssB.StrPR.dfcw;
	rData.StrPR.dfcf    = RtstB.dStress_PR[RtstB.nPoint[nIsCompTens]] * RtstB.drPR;
	rData.StrPR.dftf    = RtstB.dStress_PR[RtstB.nPoint[nIsCompTens]] * RtstB.drPR;
	rData.StrPR.bIsTopC = 0.0; //RcssB.StrPR.bIsTopC;
	rData.dRF     = (bIsComp==TRUE) ? RtstB.dRF[0] : RtstB.dRF[1];
	rData.dKRF    = (bIsComp==TRUE) ? RtstB.dKRF[0] : RtstB.dKRF[1];
	rData.dSF     = RtstB.dRT[0];

	T_RFDL_B &DtkS = (bIsComp==TRUE) ? RtstB.DtkS[0] : RtstB.DtkS[1];
	rData.DtrK.dKa    = DtkS.dKa;
	rData.DtrK.dKb    = DtkS.dKb;
	rData.DtrK.dK     = DtkS.dK;  
	rData.DtrK.dEpsiC = DtkS.dEpsiC;
	rData.DtrK.dEpsiT = DtkS.dEpsiT;

    rData.dCompFpb = RtstB.dCompFpb;
    rData.dTensFpb = RtstB.dTensFpb;
    rData.dAlwComp = RtstB.dAlwComp;
    rData.dAlwTens = RtstB.dAlwTens;
}



void CRptRatingPSC_AASHTO_LRFR12::Convert_FlexureElem4RCase(T_RTBC_BASE &RcstB, RAT_LRSR_FLEX_RCASE &rData)
{
	rData.nElem          = RcstB.ElemK;
	rData.nPos           = RcstB.nPosi;
	rData.dScaleFac4Test = RcstB.DtkF.dK;
	rData.dSafeCapa      = RcstB.dRT;
	rData.dMuDe          = RcstB.dMu_SUM;//RcstB.ForSUM.dForce[4];    
	rData.dfbuDe         = RcstB.drPR*RcstB.dMu_PR;//RcstB.dfbu_SUM;
	rData.nCaseRes       = 0; //RcstB.nCaseRes;
	rData.dRF            = RcstB.dRF;
	rData.dKRF           = RcstB.dKRF;

}

void CRptRatingPSC_AASHTO_LRFR12::Convert_ShearElem4RCase(T_RTSC_BASE &RcstB, RAT_LRSR_SHEAR_RCASE &rData)
{
	rData.nElem          = RcstB.ElemK;
	rData.nPos           = RcstB.nPosi;
	rData.dScaleFac4Test = RcstB.DtkS.dK;
	rData.dSafeCapa      = RcstB.dRT;
	rData.dVuDe          = RcstB.dVu_SUM;//RcstB.ForSUM.dForce[2];
	rData.dRF            = RcstB.dRF;
	rData.dKRF           = RcstB.dKRF;
}

void CRptRatingPSC_AASHTO_LRFR12::Convert_StressElem4RCase(T_RTST_BASE &RcssB, RAT_LRSR_STRESS_RCASE &rData, int nRFCTmin)
{
	rData.nElem          = RcssB.ElemK;
	rData.nPos           = RcssB.nPosi;
	rData.dScaleFac4Test = RcssB.DtkS[nRFCTmin].dK;
	rData.dSafeCapa      = RcssB.dRT[nRFCTmin];

	//double dStrDe=0.0;
	//   if     (RcssB.nCrMinType==0) dStrDe = RcssB.StrSUM.dfcw;
	//   else if(RcssB.nCrMinType==1) dStrDe = RcssB.StrSUM.dfcf;
	//   else if(RcssB.nCrMinType==2) dStrDe = RcssB.StrSUM.dftf;
	//if     (RcssB.nCrMinType==0) dStrDe = RcssB.StrPR.dfcw;
	//else if(RcssB.nCrMinType==1) dStrDe = RcssB.StrPR.dfcf;
	//else if(RcssB.nCrMinType==2) dStrDe = RcssB.StrPR.dftf;
	rData.dStressDe      = RcssB.dStress_PR[RcssB.nPoint[nRFCTmin]];
	rData.dRF            = RcssB.dRF[nRFCTmin];
	rData.dKRF           = RcssB.dKRF[nRFCTmin];

}

BOOL CRptRatingPSC_AASHTO_LRFR12::GetMvldLoadCaseName(T_MVLD_K MvldK, CString &strName)
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