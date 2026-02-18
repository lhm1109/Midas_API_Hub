// Dgn_PSCManager.cpp: implementation of the CDgn_PSCManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Dgn_CSGManager_NR_GN_CIV_025_06.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDgn_CSGManager_NR_GN_CIV_025_06::CDgn_CSGManager_NR_GN_CIV_025_06() : CDgn_CSGManager()
{
    m_bIsCalc = FALSE;
}

CDgn_CSGManager_NR_GN_CIV_025_06::~CDgn_CSGManager_NR_GN_CIV_025_06()
{

}

BOOL CDgn_CSGManager_NR_GN_CIV_025_06::InitialData(int nDgnType, int nDgnCode)
{
    CDgn_CSGManager::InitialData(nDgnType, nDgnCode);

    HINSTANCE hDllCalc = GetOrLoad_CalcDllHandle(nDgnType, nDgnCode);

    if ( hDllCalc == NULL ) return FALSE;

    m_pGParam = (PDGNENGINE_SETD*)GetProcAddress(hDllCalc, "GetCSGBS5400_PsiEffectiveBreadthRatio");
    if ( !m_pGParam ) return FALSE;

    m_pSFParam = (PDGNENGINE_SETD*)GetProcAddress(hDllCalc, "GetCSGBS5400_SlendernessFactor");
    if (!m_pSFParam) return FALSE;

    m_pMemberRes = (PDGNENGINE_FUNC*)GetProcAddress(hDllCalc, "Get_CsgMemberResultCS457");
    if ( !m_pMemberRes ) return FALSE;

    //m_pConsRes = (PDGNENGINE_FUNC3*)GetProcAddress(hDllCalc, "Check_CsgConstructibilityCS457");
    //if ( !m_pConsRes ) return FALSE;

    m_pStrnRes = (PDGNENGINE_FUNC3*)GetProcAddress(hDllCalc, "Check_CsgStrengthCS457");
    if ( !m_pStrnRes ) return FALSE;

    m_pServRes = (PDGNENGINE_FUNC3*)GetProcAddress(hDllCalc, "Check_CsgServiceCS457");
    if ( !m_pServRes ) return FALSE;

    //m_pSconRes = (PDGNENGINE_FUNC3*)GetProcAddress(hDllCalc, "Check_CsgShearConnectorCS457");
    //if ( !m_pSconRes ) return FALSE;

    if ( !IsCalc() )
    {
        CalcEffectiveBreadthRatio();
        CalcSlendernessFactor();
    }

    return TRUE;
}

BOOL CDgn_CSGManager_NR_GN_CIV_025_06::Check_MemberResult(CSG_MEMB_POSD_KSCE_LSD &MembD, CSG_MEMB_RESULT_D &rData)
{
    size_t TSize_In    = sizeof(CSG_MEMB_POSD_KSCE_LSD);
    size_t TSize_Res   = 0;

    if ( m_pMemberRes == NULL ) return FALSE;
    if ( rData.pNRGN==NULL ) return FALSE;

    int nChkPos = MembD.Memb.ChIf.nChkPos;
    if ( nChkPos<0 || nChkPos>1 ) return FALSE;

    rData.pNRGN->MembR[nChkPos].pEffB = &m_EffBreadthR;
    rData.pNRGN->MembR[nChkPos].pvSF  = &m_SlendernessFactorT;

    // --

    //CSG_TRST_POSD& Trst = MembD.Memb.PosD->Trst;
    //Trst.bUseStifWeb = true;
    //Trst.nStiffenerTypeWeb = 0;
    //Trst.nStifNumWeb = 2;
    //Trst.dPitchWeb = 300.0;
    //Trst.dSizeWeb[0] = 304.8;
    //Trst.dSizeWeb[1] = 15.9;
    //Trst.dFyWeb = 250.0;
    //
    //Trst.bUseStifBearing = true;
    //Trst.nBearingType = 0;
    //Trst.nBearingPairNum = 2;
    //Trst.dPitchBearing = 300.0;
    //Trst.dSizeBearing[0] = 304.8;
    //Trst.dSizeBearing[1] = 15.9;
    //Trst.dClip4Fillet = 20.0;
    //Trst.dFyBearing = 250.0;
    //Trst.dFuBearing = 300.0;
    //Trst.nNumBracing = 2;
    
    // --

    BOOL bVersionOK=FALSE;

    TSize_Res = sizeof(CSG_MEMB_RES_CS457_D);
    BOOL bCheck = (*m_pMemberRes)(&MembD, &rData.pNRGN->MembR[nChkPos], TSize_In, TSize_Res, bVersionOK);
    
    if ( !bCheck || !bVersionOK ) return FALSE;
    return TRUE;
}

BOOL CDgn_CSGManager_NR_GN_CIV_025_06::Check_StrengthRes(CSG_MEMB_POSD_KSCE_LSD &MembD, CSG_MEMB_RESULT_D &MembRes, CSG_CHECK_RESULT_CODES &ChkRes)
{
    size_t TSize_In    = sizeof(CSG_MEMB_POSD_KSCE_LSD);
    size_t TSize_MembR = sizeof(CSG_MEMB_RES_CS457_D);
    size_t TSize_Res   = sizeof(CSG_STRN_RES_CS457);

    if ( m_pStrnRes == NULL ) return FALSE;
    if ( MembRes.pNRGN ==NULL ) return FALSE;
    if ( ChkRes.pNRGN ==NULL ) return FALSE;

    int nChkPos = MembD.Memb.ChIf.nChkPos;
    if ( nChkPos<0 || nChkPos>1 ) return FALSE;

    BOOL bVersionOK=FALSE;
    BOOL bCheck = (*m_pStrnRes)(&MembD, &MembRes.pNRGN->MembR[nChkPos], ChkRes.pNRGN->pStrnD, TSize_In, TSize_MembR, TSize_Res, bVersionOK);

    if ( !bCheck || !bVersionOK ) return FALSE;
    return TRUE;
}

BOOL CDgn_CSGManager_NR_GN_CIV_025_06::Check_ServiceRes(CSG_MEMB_POSD_KSCE_LSD &MembD, CSG_MEMB_RESULT_D &MembRes, CSG_CHECK_RESULT_CODES &ChkRes)
{
    size_t TSize_In    = sizeof(CSG_MEMB_POSD_KSCE_LSD);
    size_t TSize_MembR = sizeof(CSG_MEMB_RES_CS457_D);
    size_t TSize_Res   = sizeof(CSG_SERV_RES_CS457);

    if ( m_pServRes == NULL ) return FALSE;
    if ( MembRes.pNRGN ==NULL ) return FALSE;
    if ( ChkRes.pNRGN ==NULL ) return FALSE;

    int nChkPos = MembD.Memb.ChIf.nChkPos;
    if ( nChkPos<0 || nChkPos>1 ) return FALSE;

    BOOL bVersionOK=FALSE;
    BOOL bCheck = (*m_pServRes)(&MembD, &MembRes.pNRGN->MembR[nChkPos], ChkRes.pNRGN->pServD, TSize_In, TSize_MembR, TSize_Res, bVersionOK);

    if ( !bCheck || !bVersionOK ) return FALSE;
    return TRUE;
}

BOOL CDgn_CSGManager_NR_GN_CIV_025_06::CalcEffectiveBreadthRatio()
{
    size_t TSize_Res   = sizeof(CSG_EFF_BREADTH_RATIO_SPAN_D);

    BOOL bVersionOK=FALSE;
    BOOL bCheck = (*m_pGParam)(&m_EffBreadthR, TSize_Res, bVersionOK);

    if ( !bCheck || !bVersionOK ) return FALSE;

    m_bIsCalc = TRUE;

    return TRUE;
}

BOOL CDgn_CSGManager_NR_GN_CIV_025_06::CalcSlendernessFactor()
{
    size_t TSize_Res = sizeof(CSG_v_SLENDERNESS_FACTOR_TABLE);

    BOOL bVersionOK = FALSE;
    BOOL bCheck = (*m_pSFParam)(&m_SlendernessFactorT, TSize_Res, bVersionOK);

    if (!bCheck || !bVersionOK) return FALSE;

    //m_bIsCalc = TRUE;

    return TRUE;
}


BOOL CDgn_CSGManager_NR_GN_CIV_025_06::GetEffBreadthRatio(CSG_EFF_BREADTH_RATIO_SPAN_D& rData)
{
    rData = m_EffBreadthR;
    return TRUE;
}

BOOL CDgn_CSGManager_NR_GN_CIV_025_06::GetSlendernessFactorTable(CSG_v_SLENDERNESS_FACTOR_TABLE& rData)
{
    rData = m_SlendernessFactorT;
    return TRUE;
}

