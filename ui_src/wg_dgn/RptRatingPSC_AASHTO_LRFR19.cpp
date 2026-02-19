#include "stdafx.h"
#include "RptRatingPSC_AASHTO_LRFR19.h"

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

CRptRatingPSC_AASHTO_LRFR19::CRptRatingPSC_AASHTO_LRFR19()
{
    m_pDoc = CDBDoc::GetDocPoint(); ASSERT(m_pDoc);
    m_bStopExecute = FALSE;

    m_pDoc->SetCivilCodeDgn(AASHTO_LRFD19_PSC_RATING);
    m_pDataCtrl = new CCRCDataCtrl();

    m_pForcCtrl = m_pDataCtrl->Get_ForceCtrlPointer();
    if ( m_pForcCtrl==NULL ) { ASSERT(0); return; }

}

CRptRatingPSC_AASHTO_LRFR19::~CRptRatingPSC_AASHTO_LRFR19()
{
    if ( m_pDataCtrl )
    {
        delete m_pDataCtrl;
        m_pDataCtrl = NULL;
    }
}

BOOL CRptRatingPSC_AASHTO_LRFR19::Execute_RatingReport(int iDgnCode, CString strPath, int iPrintOpt, BOOL bSaveImage)
{
	T_UNIT_INDEX CurrUnit;
	m_pDoc->m_pUnitCtrl->GetUnitIndexCurrent(CurrUnit);

	T_UNIT_INDEX CodeUnit = CurrUnit;
	CodeUnit.nBase_Length = D_UNITSYS_LENGTH_INDEX_IN;
    CodeUnit.nBase_Force  = D_UNITSYS_FORCE_INDEX_KIP;
    m_pDataCtrl->Set_UnitIndex(CodeUnit);

    if ( m_bStopExecute ) return FALSE;
    Progress(3, 0);
    int nProgressPercent = 0;

    if ( m_bStopExecute ) return FALSE;
    nProgressPercent +=5;
    Progress(0, nProgressPercent);
    Progress(2, nProgressPercent);

    if ( m_bStopExecute ) return FALSE;
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

    ArrElemPairKey aPrintElemPairK;
	CDBLib::ConvertToElemPairKeyList(aPrintElemKey, aPrintVBeamKey, aPrintElemPairK);

    int nPrintElemSize = aPrintElemPairK.GetSize();
    if ( nPrintElemSize==0 )
    {
        if ( m_bStopExecute ) return FALSE;
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

        for ( i=0; i<nPrintElemSize; ++i ) // Repeat Element
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

            BOOL bPrintI = (RkpoD.iPrintChk == 0 || RkpoD.iPrintChk == 2);
            BOOL bPrintJ = (RkpoD.iPrintChk == 1 || RkpoD.iPrintChk == 2);


            // Repeat I & J
            RAT_ELEM_RES RatElemR;
            RatElemR.nElem = ElemK.first;
			RatElemR.nBeamType = ElemK.second;
            RatElemR.nPrtOpt = RkpoD.iPrintChk;

            for ( int nIJ = 0; nIJ < 2; ++nIJ )
            {
                if ( nIJ==0 && (!bPrintI) ) continue;
                if ( nIJ==1 && (!bPrintJ) ) continue;

                T_BLRC_D TBlrcD;
                //////////////////////////////////////////////////////////////////////////
                // Flexure Rating Case Result
                //////////////////////////////////////////////////////////////////////////
                T_RTBC_CASE FlexCase;// Civil Structure

                //T_RTBC_BASE RtbcBaseD; // Civil Structure

                RAT_SFLE_BASE FlexDesign[2]; // DgnEngine Structure, 0 = Positive , 1 = Negative
                RAT_SFLE_BASE FlexLegal[2]; // DgnEngine Structure, 0 = Positive , 1 = Negative
                RAT_SFLE_BASE FlexPermit[2]; // DgnEngine Structure, 0 = Positive , 1 = Negative

                BOOL bDC_Max=TRUE, bDW_Max=TRUE, bTemp_Max=TRUE;
                int iConcurr=0;

                int iLegalPosNeg = 0;
                int iPermitPosNeg = 0;

                for ( int iMaxMin = 1; iMaxMin < 3; ++iMaxMin ) // 1 : Positive , 2 :  Negative
                {
                    if ( pAnalysisResult->GetFlexuralStrengthRatingData(ElemK, 0, iMaxMin, bDC_Max, bDW_Max, bTemp_Max, iConcurr, FlexCase) )
                    {
                        TBlrcD.Initialize();
                        m_pDoc->m_pAttrCtrl->GetBlrc(FlexCase.RtbcBase[nIJ].RCaseK, TBlrcD);
                        Convert_FlexureElemPos(TBlrcD.RatingCaseName, (FlexCase.RtbcBase[nIJ].nMoment == 1), FlexCase.RtbcBase[nIJ], FlexDesign[iMaxMin-1]);
                    }

                    if ( iMaxMin == 1 )
                    {
                        iLegalPosNeg = 3;
                        iPermitPosNeg = 5;
                    }
                    if ( iMaxMin == 2 )
                    {
                        iLegalPosNeg = 4;
                        iPermitPosNeg = 6;
                    }

                    if ( pAnalysisResult->GetFlexuralStrengthRatingData(ElemK, 0, iLegalPosNeg, bDC_Max, bDW_Max, bTemp_Max, iConcurr, FlexCase) )
                    {
                        TBlrcD.Initialize();
                        m_pDoc->m_pAttrCtrl->GetBlrc(FlexCase.RtbcBase[nIJ].RCaseK, TBlrcD);
                        Convert_FlexureElemPos(TBlrcD.RatingCaseName, (FlexCase.RtbcBase[nIJ].nMoment == 1), FlexCase.RtbcBase[nIJ], FlexLegal[iMaxMin-1]);
                    }

                    if ( pAnalysisResult->GetFlexuralStrengthRatingData(ElemK, 0, iPermitPosNeg, bDC_Max, bDW_Max, bTemp_Max, iConcurr, FlexCase) )
                    {
                        TBlrcD.Initialize();
                        m_pDoc->m_pAttrCtrl->GetBlrc(FlexCase.RtbcBase[nIJ].RCaseK, TBlrcD);
                        Convert_FlexureElemPos(TBlrcD.RatingCaseName, (FlexCase.RtbcBase[nIJ].nMoment == 1), FlexCase.RtbcBase[nIJ], FlexPermit[iMaxMin-1]);
                    }

                }

                //////////////////////////////////////////////////////////////////////////
                // Shear Rating Case Result
                //////////////////////////////////////////////////////////////////////////
                T_RTSC_CASE ShearCase; // Civil Structure

                RAT_SSHR_BASE ShearDesign; // DgnEngine Structure
                RAT_SSHR_BASE ShearLegal; // DgnEngine Structure
                RAT_SSHR_BASE ShearPermit; // DgnEngine Structure

                int iMaxMin=1;
                iLegalPosNeg=3;
                iPermitPosNeg=5;
                if ( pAnalysisResult->GetShearStrengthRatingData(ElemK, 0, iMaxMin, bDC_Max, bDW_Max, bTemp_Max, iConcurr, ShearCase) )
                {
                    TBlrcD.Initialize();
                    m_pDoc->m_pAttrCtrl->GetBlrc(ShearCase.RtscBase[nIJ].RCaseK, TBlrcD);
                    Convert_ShearElemPos(TBlrcD.RatingCaseName, ShearCase.RtscBase[nIJ], ShearDesign);
                }
                if ( pAnalysisResult->GetShearStrengthRatingData(ElemK, 0, iLegalPosNeg, bDC_Max, bDW_Max, bTemp_Max, iConcurr, ShearCase) )
                {
                    TBlrcD.Initialize();
                    m_pDoc->m_pAttrCtrl->GetBlrc(ShearCase.RtscBase[nIJ].RCaseK, TBlrcD);
                    Convert_ShearElemPos(TBlrcD.RatingCaseName, ShearCase.RtscBase[nIJ], ShearLegal);
                }
                if ( pAnalysisResult->GetShearStrengthRatingData(ElemK, 0, iPermitPosNeg, bDC_Max, bDW_Max, bTemp_Max, iConcurr, ShearCase) )
                {
                    TBlrcD.Initialize();
                    m_pDoc->m_pAttrCtrl->GetBlrc(ShearCase.RtscBase[nIJ].RCaseK, TBlrcD);
                    Convert_ShearElemPos(TBlrcD.RatingCaseName, ShearCase.RtscBase[nIJ], ShearPermit);
                }

                //////////////////////////////////////////////////////////////////////////
                // Stress Rating Case Result
                //////////////////////////////////////////////////////////////////////////
                T_RTST_CASE StressCase, StressCase_Slab; // Girder & Slab
                T_RTST_PERMIT_CASE StressCasePermit; // Girder 
                RAT_SSTR_BASE StressDesign[2], StressDesign_Slab[2];
                RAT_SSTR_BASE StressLegal[2], StressLegal_Slab[2];
                RAT_SSTR_PERMIT_BASE StressPermit;

                bDC_Max=TRUE, bDW_Max=TRUE, bTemp_Max=TRUE, iConcurr=0;

                int iLegalMaxMin = 0;
                int iPermitMaxMin = 5;

                for ( int iMaxMin = 1; iMaxMin < 3; ++iMaxMin ) // 1 : Compression , 2 :  Tension
                {
                    // Girder
                    if ( pAnalysisResult->GetStressRatingData(ElemK, 0, iMaxMin, bDC_Max, bDW_Max, bTemp_Max, iConcurr, StressCase) )
                    {
                        TBlrcD.Initialize();
                        m_pDoc->m_pAttrCtrl->GetBlrc(StressCase.RtstBase[nIJ].RCaseK, TBlrcD);
                        Convert_StressElemPos(TBlrcD.RatingCaseName, (iMaxMin == 1), StressCase.RtstBase[nIJ], StressDesign[iMaxMin-1]);
                    }

                    // Slab
                    if ( bIsCpt )
                    {
                        if ( pAnalysisResult->GetSlabStressRatingData(ElemK, 0, iMaxMin, bDC_Max, bDW_Max, bTemp_Max, iConcurr, StressCase_Slab) )
                        {
                            TBlrcD.Initialize();
                            m_pDoc->m_pAttrCtrl->GetBlrc(StressCase_Slab.RtstBase[nIJ].RCaseK, TBlrcD);
                            Convert_StressElemPos(TBlrcD.RatingCaseName, (iMaxMin == 1), StressCase_Slab.RtstBase[nIJ], StressDesign_Slab[iMaxMin-1]);
                        }
                    }

                    if ( iMaxMin == 1 ) iLegalMaxMin = 3;
                    if ( iMaxMin == 2 ) iLegalMaxMin = 4;
                    

                    // Girder
                    if ( pAnalysisResult->GetStressRatingData(ElemK, 0, iLegalMaxMin, bDC_Max, bDW_Max, bTemp_Max, iConcurr, StressCase) )
                    {
                        TBlrcD.Initialize();
                        m_pDoc->m_pAttrCtrl->GetBlrc(StressCase.RtstBase[nIJ].RCaseK, TBlrcD);
                        Convert_StressElemPos(TBlrcD.RatingCaseName, (iMaxMin == 1), StressCase.RtstBase[nIJ], StressLegal[iMaxMin-1]);
                    }

                    // Slab
                    if ( bIsCpt )
                    {
                        if ( pAnalysisResult->GetSlabStressRatingData(ElemK, 0, iLegalMaxMin, bDC_Max, bDW_Max, bTemp_Max, iConcurr, StressCase_Slab) )
                        {
                            TBlrcD.Initialize();
                            m_pDoc->m_pAttrCtrl->GetBlrc(StressCase_Slab.RtstBase[nIJ].RCaseK, TBlrcD);
                            Convert_StressElemPos(TBlrcD.RatingCaseName, (iMaxMin == 1), StressCase_Slab.RtstBase[nIJ], StressLegal_Slab[iMaxMin-1]);
                        }
                    }
                }

                if ( pAnalysisResult->GetStressRatingPermitData(ElemK, 0, iPermitMaxMin, bDC_Max, bDW_Max, bTemp_Max, iConcurr, StressCasePermit) )
                {
                    TBlrcD.Initialize();
                    m_pDoc->m_pAttrCtrl->GetBlrc(StressCasePermit.RtstPermitBase[nIJ].RCaseK, TBlrcD);
                    Convert_StressElemPosPermit(TBlrcD.RatingCaseName, StressCasePermit.RtstPermitBase[nIJ], StressPermit);
                }


                //////////////////////////////////////////////////////////////////////////
                // Save Result
                //////////////////////////////////////////////////////////////////////////
                //Flexure
                for ( int nPN = 0; nPN < 2; ++nPN ) // Positive, Negative
                {
                    RatElemR.FlexR.FlexDesign[nIJ * 2 + nPN] = FlexDesign[nPN];
                    RatElemR.FlexR.FlexLegal[nIJ * 2 + nPN] = FlexLegal[nPN];
                    RatElemR.FlexR.FlexPermit[nIJ * 2 + nPN] = FlexPermit[nPN];
                }

                CheckFlexReportData(BlrsD, RatElemR.FlexR);

                //Shear 
                RatElemR.ShearR.ShrDesign[nIJ] = ShearDesign;
                RatElemR.ShearR.ShrLegal[nIJ] = ShearLegal;
                RatElemR.ShearR.ShrPermit[nIJ] = ShearPermit;

                CheckShearReportData(BlrsD, RatElemR.ShearR);

                for ( int nCT = 0; nCT < 2; ++nCT ) // Compression, Tension
                {
                    //Stress for Girder
                    RatElemR.StressR.StressDesign[nIJ * 2 + nCT] = StressDesign[nCT];
                    RatElemR.StressR.StressLegal[nIJ * 2 + nCT] = StressLegal[nCT];

                    //Stress for Slab
                    RatElemR.StressR.StressDesign_Slab[nIJ * 2 + nCT] = StressDesign_Slab[nCT];
                    RatElemR.StressR.StressLegal_Slab[nIJ * 2 + nCT] = StressLegal_Slab[nCT];
                }


                RatElemR.StressR.StressPermitPSC[nIJ] = StressPermit;

                CheckSLSReportData(BlrsD, RatElemR.StressR);

            }
            RatResRpt.aRatElemR.Add(RatElemR);
        }
        // Summary

        CArray<T_BLRC_K, T_BLRC_K> aBlrcKey;
        m_pDoc->m_pAttrCtrl->GetBlrcKeyList(aBlrcKey);
        int nBlrcKeySize = aBlrcKey.GetSize();

        for ( i=0; i<nBlrcKeySize; ++i )
        {
            T_BLRC_K BlrcK = aBlrcKey[i];
            T_BLRC_D BlrcD; BlrcD.Initialize();
            if ( !m_pDoc->m_pAttrCtrl->GetBlrc(BlrcK, BlrcD) ) continue;

            ArrElemPairKey arBlrDgnElement;            
            pAnalysisResult->GetBlrDgnElemData(arBlrDgnElement);
            int iTotalElem = arBlrDgnElement.GetSize();
            BOOL bIsCpt=0;
            for ( int nTotalElem=0; nTotalElem<iTotalElem; nTotalElem++ )
            {
                auto ElemK = arBlrDgnElement[nTotalElem];
                bIsCpt     = m_pDataCtrl->IsCompositePSCSect(ElemK);
            }
            CString strMvldName;
            GetMvldLoadCaseName(BlrcD.PrimaryVeh.LoadCaseKey, strMvldName); // ??

            RAT_LRSR_RCASE_B LrsrD;
            LrsrD.nEvaluation  = BlrcD.iEvaluation; //RklcD.nEvaluation;
            LrsrD.nRCaseType   = BlrcD.iLimitState;
            LrsrD.strRCaseName = BlrcD.RatingCaseName;
            LrsrD.strMVName    = strMvldName;
            LrsrD.nComposite   = bIsCpt; //0:PSC 1:PSC Composite

            for ( j=0; j<2; ++j )
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

            if ( BlrcD.iLimitState==0 && !BlrsD.bService[0] && BlrcD.iEvaluation == ENUM_DESIGN ) continue;
            if ( BlrcD.iLimitState==0 && !BlrsD.bService[3] && BlrcD.iEvaluation == ENUM_LEGAL ) continue;
            if ( BlrcD.iLimitState==0 && !BlrsD.bService[5] && BlrcD.iEvaluation == ENUM_PERMIT ) continue;


            if ( BlrcD.iLimitState==1 ) // Strength
            {
                T_RTBC_BASE FlexStrnCR;
                T_RTSC_BASE ShearStrnCR;
                pAnalysisResult->GetFlexuralStrengthRatingMinRF_Data(BlrcK, FlexStrnCR);
                pAnalysisResult->GetShearStrengthRatingMinRF_Data(BlrcK, ShearStrnCR);
                Convert_FlexureElem4RCase(FlexStrnCR, LrsrD.FlexD);
                Convert_ShearElem4RCase(ShearStrnCR, LrsrD.ShearD);

                LrsrD.dWeight = FlexStrnCR.dWeight;
            }
            else if ( BlrcD.iLimitState==0 ) // Service
            {
                if ( BlrcD.iEvaluation != ENUM_PERMIT)
                {
                    T_RTST_BASE CompServCR_G, TensServCR_G;
                    int nGirder=0;
                    int nSlab=1;
                    int nRFCTmin=0; // Comp : 0 , Tens : 1
                    pAnalysisResult->GetStressRatingMinRF_Data(nGirder, BlrcK, CompServCR_G, TensServCR_G, nRFCTmin); //nPart = 0 : Girder 1: Slab
                    if ( nRFCTmin==0 )
                    {
                        Convert_StressElem4RCase(CompServCR_G, LrsrD.StressD[nGirder], nRFCTmin);
                        LrsrD.dWeight = CompServCR_G.dWeight;
                    }
                    else if ( nRFCTmin==1 )
                    {
                        Convert_StressElem4RCase(TensServCR_G, LrsrD.StressD[nGirder], nRFCTmin);
                        LrsrD.dWeight = TensServCR_G.dWeight;
                    }
                    else ASSERT(0);

                    if ( bIsCpt ) // Composite 단면 일때만 수행하도록!
                    {
                        T_RTST_BASE CompServCR_S, TensServCR_S;
                        int nRFCTmin_Slab=0; // Comp : 0 , Tens : 1
                        pAnalysisResult->GetStressRatingMinRF_Data(nSlab, BlrcK, CompServCR_S, TensServCR_S, nRFCTmin_Slab); //nPart = 0 : Girder 1: Slab
                        if ( nRFCTmin_Slab==0 )
                        {
                            Convert_StressElem4RCase(CompServCR_S, LrsrD.StressD[nSlab], nRFCTmin_Slab);
                            LrsrD.dWeight = CompServCR_S.dWeight;
                        }
                        else if ( nRFCTmin_Slab==1 )
                        {
                            Convert_StressElem4RCase(TensServCR_S, LrsrD.StressD[nSlab], nRFCTmin_Slab);
                            LrsrD.dWeight = TensServCR_S.dWeight;
                        }
                        else ASSERT(0);
                    }
                }
                else
                {
                    T_RTST_PERMIT_BASE TensServPermit;
                    pAnalysisResult->GetStressRatingPermitMinRF_Data(BlrcK, TensServPermit);
                    Convert_StressElem4RCasePermit(TensServPermit, LrsrD.PermitD);
                }
            }
            switch ( BlrcD.iEvaluation )
            {
            case ENUM_DESIGN: 
                RatResRpt.LrsrD.DesignLR.Add(LrsrD);
                break;
            case ENUM_LEGAL:
                RatResRpt.LrsrD.LegalLR.Add(LrsrD);
                break;
            case ENUM_PERMIT:
                RatResRpt.LrsrD.PermitLR.Add(LrsrD);
                break;
            default:
                ASSERT(0);
                break;
            }
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
    }
    catch ( ... )
    {
        bSuccess = FALSE;
    }

    return bSuccess;
}

void CRptRatingPSC_AASHTO_LRFR19::CheckFlexReportData(T_BLRS_D ParamD, RAT_SFLE_D &FlexD)
{
    for ( int i=0; i<4; i++ )
    {
        if ( !FlexD.bChkDesign )
        {
            FlexD.bChkDesign = FlexD.FlexDesign[i].bChk;
        }

        if ( !FlexD.bChkLegal )
        {
            FlexD.bChkLegal = FlexD.FlexLegal[i].bChk;
        }

        if ( !FlexD.bChkPermit )
        {
            FlexD.bChkPermit = FlexD.FlexPermit[i].bChk;
        }
    }

    if ( FlexD.bChkDesign || FlexD.bChkLegal || FlexD.bChkPermit ) FlexD.bChk=true;

    if ( !ParamD.bStrength[0] ) FlexD.bChk=false;
}

void CRptRatingPSC_AASHTO_LRFR19::CheckShearReportData(T_BLRS_D ParamD, RAT_SSHR_D &ShearD)
{
    for ( int i=0; i<2; i++ )
    {
        if ( !ShearD.bChkDesign )
        {
            ShearD.bChkDesign = ShearD.ShrDesign[i].bChk;
        }

        if ( !ShearD.bChkLegal )
        {
            ShearD.bChkLegal = ShearD.ShrLegal[i].bChk;
        }

        if ( !ShearD.bChkPermit )
        {
            ShearD.bChkPermit = ShearD.ShrPermit[i].bChk;
        }
    }

    if ( ShearD.bChkDesign || ShearD.bChkLegal || ShearD.bChkPermit ) ShearD.bChk = true;

    if ( !ParamD.bStrength[1] ) ShearD.bChk=false;
}

void CRptRatingPSC_AASHTO_LRFR19::CheckSLSReportData(T_BLRS_D ParamD, RAT_SSTR_D &StressD)
{
    for ( int i=0; i<4; i++ )
    {
        if ( !StressD.bChkDesign )
        {
            StressD.bChkDesign = StressD.StressDesign[i].bChk;
        }

        if ( !StressD.bChkDesign_Slab )
        {
            StressD.bChkDesign_Slab = StressD.StressDesign_Slab[i].bChk;
        }

        if ( !StressD.bChkLegal )
        {
            StressD.bChkLegal = StressD.StressLegal[i].bChk;
        }

        if ( !StressD.bChkLegal_Slab )
        {
            StressD.bChkLegal_Slab = StressD.StressLegal_Slab[i].bChk;
        }
    }

    for ( int j=0; j<2; j++ )
    {
        if ( !StressD.bChkPermit )
        {
            StressD.bChkPermit = StressD.StressPermitPSC[j].bChk;
        }
    }

    if ( !ParamD.bService[0] )
    {
        StressD.bChkDesign=false;
        StressD.bChkDesign_Slab=false;
    }

    if ( !ParamD.bService[3] )
    {
        StressD.bChkLegal=false;
        StressD.bChkLegal_Slab=false;
    }

    if ( !ParamD.bService[5] )
    {
        StressD.bChkPermit=false;
    }

    if ( StressD.bChkDesign || StressD.bChkLegal || StressD.bChkPermit ) StressD.bChk=true;
    if ( StressD.bChkDesign_Slab || StressD.bChkLegal_Slab ) StressD.bChk_Slab=true;
}



void CRptRatingPSC_AASHTO_LRFR19::Convert_FlexureElemPos(CString strRcaseName, BOOL bPositiveM, T_RTBC_BASE &RtbcB, RAT_SFLE_BASE &rData)
{
    double dCF = RtbcB.dPhi_CF;
    double dSF = RtbcB.dPhi_SF;
    double dCapacityF=dCF*dSF;
    if ( dCapacityF<0.85 ) dCapacityF = 0.85;

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

void CRptRatingPSC_AASHTO_LRFR19::Convert_ShearElemPos(CString strRcaseName, T_RTSC_BASE &RtscB, RAT_SSHR_BASE &rData)
{
    double dCF = RtscB.dPhi_CF;
    double dSF = RtscB.dPhi_SF;
    double dCapacityF=dCF*dSF;
    if ( dCapacityF<0.85 ) dCapacityF = 0.85;

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

void CRptRatingPSC_AASHTO_LRFR19::Convert_StressElemPos(CString strRcaseName, BOOL bIsComp, T_RTST_BASE &RtstB, RAT_SSTR_BASE &rData)
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
    rData.dCompFpb = RtstB.dCompFpb;
    rData.dTensFpb = RtstB.dTensFpb;
    rData.dAlwComp = RtstB.dAlwComp;
    rData.dAlwTens = RtstB.dAlwTens;

    T_RFDL_B &DtkS = (bIsComp==TRUE) ? RtstB.DtkS[0] : RtstB.DtkS[1];
    rData.DtrK.dKa    = DtkS.dKa;
    rData.DtrK.dKb    = DtkS.dKb;
    rData.DtrK.dK     = DtkS.dK;
    rData.DtrK.dEpsiC = DtkS.dEpsiC;
    rData.DtrK.dEpsiT = DtkS.dEpsiT;
}

void CRptRatingPSC_AASHTO_LRFR19::Convert_StressElemPosPermit(CString strRcaseName, T_RTST_PERMIT_BASE &RtstB, RAT_SSTR_PERMIT_BASE &rData)
{
    rData.strRcaseName = strRcaseName;
    rData.bChk  = RtstB.bChk;
    rData.nElem = RtstB.ElemK;
    rData.nPart = RtstB.nPosi;
    rData.iMax = RtstB.nConcurrent;

    rData.bCrack = RtstB.bCrack;
    rData.nTendonRebar = RtstB.nTendonRebar;

    rData.dAllowStress = RtstB.dAllowStress;
    rData.dAppliedStress = RtstB.dAppliedStress;

    rData.dEffective_Stress = RtstB.dEffectStress;
    rData.dIcr = RtstB.dIcr;
    rData.dNeu = RtstB.dNeu;

    rData.dM_Crack = RtstB.dMcr;
    rData.dM_Dead  = RtstB.dM_Dead;
    rData.dM_Live  = RtstB.dM_Live;
    rData.dM_Sum   = RtstB.dM_Sum;

    rData.dStress_Dead = RtstB.dStress_Dead;
    rData.dStress_Live = RtstB.dStress_Live;
    rData.dStress_Mcr  = RtstB.dStress_Mcr;
    rData.dStress_AfterCr = RtstB.dStress_AfterCr;
    rData.dStress_Sum = RtstB.dAppliedStress;
    rData.dStressRatio = RtstB.dStressRatio;
    rData.dElast = RtstB.dElast;

}

void CRptRatingPSC_AASHTO_LRFR19::Convert_FlexureElem4RCase(T_RTBC_BASE &RcstB, RAT_LRSR_FLEX_RCASE &rData)
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

void CRptRatingPSC_AASHTO_LRFR19::Convert_ShearElem4RCase(T_RTSC_BASE &RcstB, RAT_LRSR_SHEAR_RCASE &rData)
{
    rData.nElem          = RcstB.ElemK;
    rData.nPos           = RcstB.nPosi;
    rData.dScaleFac4Test = RcstB.DtkS.dK;
    rData.dSafeCapa      = RcstB.dRT;
    rData.dVuDe          = RcstB.dVu_SUM;//RcstB.ForSUM.dForce[2];
    rData.dRF            = RcstB.dRF;
    rData.dKRF           = RcstB.dKRF;
}

void CRptRatingPSC_AASHTO_LRFR19::Convert_StressElem4RCase(T_RTST_BASE &RcssB, RAT_LRSR_STRESS_RCASE &rData, int nRFCTmin)
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

void CRptRatingPSC_AASHTO_LRFR19::Convert_StressElem4RCasePermit(T_RTST_PERMIT_BASE &RcssB, RAT_LRSR_STRESS_PERMIT_RCASE &rData)
{
    rData.nElem         = RcssB.ElemK;
    rData.nPos          = RcssB.nPosi;
    rData.nTendonRebar  = RcssB.nTendonRebar;
    rData.dEffectStress = RcssB.dEffectStress;
    rData.dStress_Dead  = RcssB.dStress_Dead;
    rData.dStress_Live  = RcssB.dStress_Live;
    rData.dStress_Mcr   = RcssB.dStress_Mcr;
    rData.dStress_AfterCr = RcssB.dStress_AfterCr;
    rData.dAllowStress  = RcssB.dAllowStress;
    rData.dAppliedStress= RcssB.dAppliedStress;
    rData.dStressRatio  = RcssB.dStressRatio;
}

BOOL CRptRatingPSC_AASHTO_LRFR19::GetMvldLoadCaseName(T_MVLD_K MvldK, CString &strName)
{
    T_MVCD_D MvcdD;
    if ( !m_pDoc->m_pAttrCtrl->GetMvcd(MvcdD) ) return FALSE;
    switch ( MvcdD.nCodeType )
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
            if ( !m_pDoc->m_pAttrCtrl->GetMvld(MvldK, MvldD) ) break;
            strName = MvldD.LoadCaseName;
        }
        break;
    case D_MOVE_CODE_JAPAN:
        {
            T_MVLDjp_D MvldJPD;
            m_pDoc->m_pAttrCtrl->InitializeMvldjp(MvldJPD);
            if ( !m_pDoc->m_pAttrCtrl->GetMvldjp(MvldK, MvldJPD) ) break;
            strName = MvldJPD.LoadCaseName;
        }
        break;
    case D_MOVE_CODE_CHINA:
        {
            T_MVLDch_D MvldCHD;
            if ( !m_pDoc->m_pAttrCtrl->GetMvldch(MvldK, MvldCHD) ) break;
            strName = MvldCHD.LoadCaseName;
        }
        break;
    case D_MOVE_CODE_INDIA:
        {
            T_MVLDid_D MvldIDD;
            if ( !m_pDoc->m_pAttrCtrl->GetMvldid(MvldK, MvldIDD) ) break;
            strName = MvldIDD.LoadCaseName;
        }
        break;
    case D_MOVE_CODE_BS:
        {
            T_MVLDbs_D MvldBSD;
            if ( !m_pDoc->m_pAttrCtrl->GetMvldbs(MvldK, MvldBSD) ) break;
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
            if ( !m_pDoc->m_pAttrCtrl->GetMvldtr(MvldK, MvldTRD) ) break;
            strName = MvldTRD.LoadCaseName;
        }
        break;
    default:
        ASSERT(0); break;
    }

    return TRUE;
}