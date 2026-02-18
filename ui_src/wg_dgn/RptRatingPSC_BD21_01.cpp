#include "stdafx.h"
#include "RptRatingPSC_BD21_01.h"

#include "Dgn_RatingRptManager.h"
#include "Rating_BD21_XLOut.h"
#include "Rating_CS454_XLOut.h"
#include "Rating_CS454_STL_XLOut.h"

#include "..\wg_db\FileCtrl.h"
#include "..\wg_db\AttrCtrl.h"
#include "..\wg_db\AttrCtrl2.h"
#include "..\wg_db\PostCtrl.h"
#include "..\wg_db\AnalysisResult.h"
#include "..\wg_db\DBLib.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRptRatingPSC_BD21_01::CRptRatingPSC_BD21_01(int nDgnCode)
{
    m_pDoc = CDBDoc::GetDocPoint(); ASSERT(m_pDoc);
    m_bStopExecute = FALSE;

    m_pDoc->SetCivilCodeDgn(nDgnCode/*BD_21_01_PSC_RATING*/);
    m_pDataCtrl = new CCRCDataCtrl();

    m_pForcCtrl = m_pDataCtrl->Get_ForceCtrlPointer();
    if(m_pForcCtrl==NULL) {ASSERT(0); return;}
}

CRptRatingPSC_BD21_01::~CRptRatingPSC_BD21_01(void)
{
    if(m_pDataCtrl)
    {
        delete m_pDataCtrl;
        m_pDataCtrl = NULL;
    }
}

BOOL CRptRatingPSC_BD21_01::Execute_RatingReport(int iDgnCode, CString strPath, int iPrintOpt, BOOL bSaveImage)
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

    // Print항목이 없을 경우 강제로 ProgressBar를 닫도록 조치!!
    CArray<T_RKPO_K, T_RKPO_K> aPrintElemKey;  
    m_pDoc->m_pAttrCtrl->GetRkpoKeyList(aPrintElemKey);

    CArray<T_RKPV_K, T_RKPV_K> aPrintVBeamKey;
    m_pDoc->m_pAttrCtrl->GetRkpvKeyList(aPrintVBeamKey);

    ArrElemPairKey aPrintElemPairK;
    CDBLib::ConvertToElemPairKeyList(aPrintElemKey, aPrintVBeamKey, aPrintElemPairK);

    CArray<T_ALCS_K, T_ALCS_K> arAlcs; arAlcs.RemoveAll();
    m_pDoc->m_pAttrCtrl2->GetAlcsKeyList(arAlcs);
    int nAlcs = arAlcs.GetSize();
    if(nAlcs==0) return FALSE;

    int nPrintElemSize = aPrintElemPairK.GetSize();
    if(nPrintElemSize==0 || nAlcs==0)
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

    BOOL bSuccess = TRUE;
    CAnalysisResult *pAnalysisResult = m_pDoc->m_pPostCtrl->GetAnalysisResult();
    T_BLRS_D BlrsD; BlrsD.Initialize();  
    m_pDoc->m_pAttrCtrl->GetBlrs(BlrsD);

    try
    {
        // Parameter
        T_ASPM_D AspmD;
        if(!m_pDoc->m_pAttrCtrl->GetAspm(AspmD)) AspmD.Initialize();

        CArray<_T_BD21_XLOUT, _T_BD21_XLOUT&> arXlout;
        arXlout.RemoveAll();
        for(int i=0; i<nPrintElemSize; ++i)
        {
            auto  ElemK = aPrintElemPairK[i];

            // 출력 옵션 셋팅
            // 1. 출력 여부 결정
            T_ASOP_D AsopD; AsopD.Initialize();  // 설계 위치
            T_RKPO_D RkpoD; RkpoD.Initialize();  // 출력 위치

            if (ElemK.second == EN_EL_BEAM)
            {
                if (!m_pDoc->m_pAttrCtrl->GetAsop(ElemK.first, AsopD)) continue;
                if (!m_pDoc->m_pAttrCtrl->GetRkpo(ElemK.first, RkpoD)) continue;
            }
            else if (ElemK.second == EN_EL_VBEAM)
            {
                ASSERT(0); // Asov ?
                if (!m_pDoc->m_pAttrCtrl->GetAsop(ElemK.first, AsopD)) continue;
                if (!m_pDoc->m_pAttrCtrl->GetRkpv(ElemK.first, RkpoD)) continue;
            }
            else ASSERT(0);

            _T_BD21_XLOUT xlOutD;
            xlOutD.Initialize();
            xlOutD.ElemK = ElemK;
            xlOutD.bIJ[0] = ((AsopD.nPos == 0 || AsopD.nPos == 2) && (RkpoD.iPrintChk == 0 || RkpoD.iPrintChk == 2) );
            xlOutD.bIJ[1] = ((AsopD.nPos == 1 || AsopD.nPos == 2) && (RkpoD.iPrintChk == 1 || RkpoD.iPrintChk == 2) );

            for(int j=0; j<nAlcs; ++j)
            {
                T_ALCS_K AlcsK = arAlcs[j];
                T_ALCS_D AlcsD;
                if(!m_pDoc->m_pAttrCtrl2->GetAlcs(AlcsK, AlcsD)) continue; // Rating case
                BOOL bULS = (AspmD.bRptUltLim  && AlcsD.nLimitState == 0); // ULS
                BOOL bSLS = (AspmD.bRptServLim && AlcsD.nLimitState == 1); // SLS

                xlOutD.mPosiFlex.SetAt(AlcsK, (AspmD.bStrLimFlexure && bULS));
                xlOutD.mNegaFlex.SetAt(AlcsK, (AspmD.bStrLimFlexure && bULS));
                xlOutD.mShear.SetAt(AlcsK, (AspmD.bStrLimShear && bULS));
                xlOutD.mTorsion.SetAt(AlcsK, (AspmD.bStrLimTorsion && bULS));
                xlOutD.mSLS.SetAt(AlcsK, (AspmD.bSevLimStrsCrack && bSLS));
            }
            arXlout.Add(xlOutD);

            //////////////////////////////////////////////////////////////////////////
            if(m_bStopExecute)	return FALSE;
            nProgressPercent += nProgressIncrementPercent;
            Progress(0, nProgressPercent);
            Progress(2, nProgressPercent);
        }

        // 계산서 출력
        bSuccess = arXlout.GetSize() < 1 ? FALSE : TRUE;
        if(bSuccess)
        {
            // 계산서 출력	 	
            CString strDataPath = m_pDoc->GetPathName();
            CString strBasePath = m_pDoc->GetProgramPath() + _T("Excel Base File\\");

            CRating_BD21_XLOut *pmyReport;
            if (iDgnCode == CS_454_20_PSC_RATING )
            {
                pmyReport = new CRating_CS454_XLOut();
            }
            else
                ASSERT(0);

            pmyReport->SetDataCtrlPointer(m_pDataCtrl);
            pmyReport->PrintReport(strBasePath, strDataPath, strPath, iPrintOpt, arXlout);   // strPath : 경로+파일이름

            if (pmyReport)
                delete pmyReport;
        }
    }
    catch (...)
    {
        bSuccess = FALSE;	
    }

    return bSuccess;
}

BOOL CRptRatingPSC_BD21_01::Execute_StlRatingReport(int iDgnCode, CString strPath, int iPrintOpt, BOOL bSaveImage)
{
    //m_pDoc->SetCivilCodeDgn(CDBLib::GetRatCode());

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

    // Print항목이 없을 경우 강제로 ProgressBar를 닫도록 조치!!
    CArray<T_SRAR_K, T_SRAR_K> aPrintElemKey;
    m_pDoc->m_pAttrCtrl2->GetSrarKeyList(aPrintElemKey);
	ArrElemPairKey aPrintElemPairKey;
	CDBLib::ConvertToElemPairKey(EN_EL_BEAM, aPrintElemKey, aPrintElemPairKey);

	CArray<T_SRRV_K, T_SRRV_K> aPrintVBeamKey;
    m_pDoc->m_pAttrCtrl2->GetSrrvKeyList(aPrintVBeamKey);
    if (aPrintVBeamKey.GetSize() > 0)
    {
        ArrElemPairKey aPrtVBeamPairKey;
		CDBLib::ConvertToElemPairKey(EN_EL_VBEAM, aPrintVBeamKey, aPrtVBeamPairKey);
        aPrintElemPairKey.Append(aPrtVBeamPairKey);
    }

    CArray<T_ALCS_K, T_ALCS_K> arAlcs; arAlcs.RemoveAll();
    m_pDoc->m_pAttrCtrl2->GetAlcsSTLKeyList(arAlcs);
    int nAlcs = arAlcs.GetSize();
    if ( nAlcs==0 ) return FALSE;

    int nPrintElemSize = aPrintElemPairKey.GetSize();
    if ( nPrintElemSize==0 || nAlcs==0 )
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

    BOOL bSuccess = TRUE;
    CAnalysisResult *pAnalysisResult = m_pDoc->m_pPostCtrl->GetAnalysisResult();

    try
    {
        // Parameter
        T_RPSS_D RpssD;
        if ( !m_pDoc->m_pAttrCtrl2->GetRpss(RpssD) ) RpssD.Initialize();

        CArray<_T_CS454_STL_XLOUT, _T_CS454_STL_XLOUT&> arXlout;
        arXlout.RemoveAll();
        for ( int i=0; i<nPrintElemSize; ++i )
        {
            auto  ElemK = aPrintElemPairKey[i];

            // 출력 옵션 셋팅
            // 1. 출력 여부 결정
            T_SRAS_D SrasD; SrasD.Initialize();  // 설계 위치
            T_SRAR_D SrarD; SrarD.Initialize();  // 출력 위치

            if (ElemK.second == EN_EL_BEAM)
            {
                if (!m_pDoc->m_pAttrCtrl2->GetSras(ElemK.first, SrasD)) continue;
                if (!m_pDoc->m_pAttrCtrl2->GetSrar(ElemK.first, SrarD)) continue;

                T_ELEM_D ElemD; ElemD.Initialize();
                if (!m_pDoc->m_pPostCtrl->GetElemPost(ElemK.first, ElemD)) continue;
            }
            else if (ElemK.second == EN_EL_VBEAM)
            {
                T_SRCV_D SrcvD; SrcvD.Initialize();
                if (!m_pDoc->m_pAttrCtrl2->GetSrcv(ElemK.first, SrcvD)) continue;
                SrasD=SrcvD;
                //
                T_SRRV_D SrrvD; SrrvD.Initialize();
                if (!m_pDoc->m_pAttrCtrl2->GetSrrv(ElemK.first, SrrvD)) continue;
                SrarD=SrrvD;
            }
            else ASSERT(0);

            _T_CS454_STL_XLOUT xlOutD;
            xlOutD.Initialize();
            xlOutD.ElemK = ElemK.first;
			xlOutD.nBeamType = ElemK.second;
            xlOutD.bIJ[0] = ((SrasD.nPos == 0 || SrasD.nPos == 2) && (SrarD.iPrintChk == 0 || SrarD.iPrintChk == 2));
            xlOutD.bIJ[1] = ((SrasD.nPos == 1 || SrasD.nPos == 2) && (SrarD.iPrintChk == 1 || SrarD.iPrintChk == 2));

            for ( int j=0; j<nAlcs; ++j )
            {
                T_ALCS_K AlcsK = arAlcs[j];
                T_ALCS_D AlcsD;
                if ( !m_pDoc->m_pAttrCtrl2->GetAlcs(AlcsK, AlcsD) ) continue; // Rating case
                BOOL bULS = (RpssD.bRptUltLim  && AlcsD.nLimitState == 0); // ULS
                BOOL bSLS = (RpssD.bRptServLim && AlcsD.nLimitState == 1); // SLS

                xlOutD.mPosiFlex.SetAt(AlcsK, (RpssD.bUlsFlexure && bULS));
                xlOutD.mNegaFlex.SetAt(AlcsK, (RpssD.bUlsFlexure && bULS));
                xlOutD.mShear.SetAt(AlcsK, (RpssD.bUlsShear && bULS));
                xlOutD.mComb.SetAt(AlcsK, (RpssD.bUlsFST && bULS));
                xlOutD.mLShearULS.SetAt(AlcsK, (RpssD.bUlsLgShear && bULS));
                xlOutD.mFlangeBox.SetAt(AlcsK, (RpssD.bUlsFlexure && bULS));
                xlOutD.mFlangeStiff.SetAt(AlcsK, (RpssD.bUlsFlexure && bULS));
                xlOutD.mWebYielding.SetAt(AlcsK, (RpssD.bUlsFlexure && bULS));
                xlOutD.mWebBuckling.SetAt(AlcsK, (RpssD.bUlsFlexure && bULS));
                xlOutD.mWebStiff.SetAt(AlcsK, (RpssD.bUlsFlexure && bULS));

                //xlOutD.mTorsion.SetAt(AlcsK, (RpssD.bStrLimTorsion && bULS));
                xlOutD.mSLS.SetAt(AlcsK, (RpssD.bSlsStress && bSLS));
                xlOutD.mLShearSLS.SetAt(AlcsK, (RpssD.bSlsLgShear && bSLS));
            }
            arXlout.Add(xlOutD);

            //////////////////////////////////////////////////////////////////////////
            if ( m_bStopExecute )	return FALSE;
            nProgressPercent += nProgressIncrementPercent;
            Progress(0, nProgressPercent);
            Progress(2, nProgressPercent);
        }

        // 계산서 출력
        bSuccess = arXlout.GetSize() < 1 ? FALSE : TRUE;
        if ( bSuccess )
        {
            // 계산서 출력	 	
            CString strDataPath = m_pDoc->GetPathName();
            CString strBasePath = m_pDoc->GetProgramPath() + _T("Excel Base File\\");

            CRating_CS454_STL_XLOut *pmyReport;
            if ( iDgnCode == STL_CS454_20 )
            {
                pmyReport = new CRating_CS454_STL_XLOut();
            }
            else
                ASSERT(0);

            pmyReport->SetDataCtrlPointer(m_pDataCtrl);
            pmyReport->PrintReport(strBasePath, strDataPath, strPath, iPrintOpt, arXlout);   // strPath : 경로+파일이름

            if ( pmyReport )
                delete pmyReport;
        }
    }
    catch ( ... )
    {
        bSuccess = FALSE;
    }

    return bSuccess;
}