// Dgn_PSCManager.cpp: implementation of the CDgn_PSCManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Dgn_CSGManager_CS457_R1.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDgn_CSGManager_CS457_R1::CDgn_CSGManager_CS457_R1() : CDgn_CSGManager()
{
    m_bIsCalc = FALSE;
}

CDgn_CSGManager_CS457_R1::~CDgn_CSGManager_CS457_R1()
{

}

BOOL CDgn_CSGManager_CS457_R1::InitialData(int nDgnType, int nDgnCode)
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

    m_pConsRes = (PDGNENGINE_FUNC3*)GetProcAddress(hDllCalc, "Check_CsgConstructibilityCS457");
    if ( !m_pConsRes ) return FALSE;

    m_pStrnRes = (PDGNENGINE_FUNC3*)GetProcAddress(hDllCalc, "Check_CsgStrengthCS457");
    if ( !m_pStrnRes ) return FALSE;

    m_pServRes = (PDGNENGINE_FUNC3*)GetProcAddress(hDllCalc, "Check_CsgServiceCS457");
    if ( !m_pServRes ) return FALSE;

    m_pSconRes = (PDGNENGINE_FUNC3*)GetProcAddress(hDllCalc, "Check_CsgShearConnectorCS457");
    if ( !m_pSconRes ) return FALSE;

    if ( !IsCalc() )
    {
        CalcEffectiveBreadthRatio();
        CalcSlendernessFactor();
    }

    return TRUE;
}

BOOL CDgn_CSGManager_CS457_R1::Check_MemberResult(CSG_MEMB_POSD_KSCE_LSD &MembD, CSG_MEMB_RESULT_D &rData)
{
    size_t TSize_In    = sizeof(CSG_MEMB_POSD_KSCE_LSD);
    size_t TSize_Res   = 0;

    if ( m_pMemberRes == NULL ) return FALSE;
    if ( rData.pCS457==NULL ) return FALSE;

    int nChkPos = MembD.Memb.ChIf.nChkPos;
    if ( nChkPos<0 || nChkPos>1 ) return FALSE;

    rData.pCS457->MembR[nChkPos].pEffB = &m_EffBreadthR;
	rData.pCS457->MembR[nChkPos].pvSF  = &m_SlendernessFactorT;

    BOOL bVersionOK=FALSE;

    TSize_Res = sizeof(CSG_MEMB_RES_CS457_D);
    BOOL bCheck = (*m_pMemberRes)(&MembD, &rData.pCS457->MembR[nChkPos], TSize_In, TSize_Res, bVersionOK);
    
    if ( !bCheck || !bVersionOK ) return FALSE;
    return TRUE;
}


BOOL CDgn_CSGManager_CS457_R1::Check_ConstructibilityRes(CSG_MEMB_POSD_KSCE_LSD &MembD, CSG_MEMB_RESULT_D &MembRes, CSG_CHECK_RESULT_CODES &ChkRes)
{
    size_t TSize_In    = sizeof(CSG_MEMB_POSD_KSCE_LSD);
    size_t TSize_MembR = sizeof(CSG_MEMB_RES_CS457_D);
    size_t TSize_Res   = sizeof(CSG_CONS_RES_CS457);

    if ( m_pConsRes == NULL ) return FALSE;
    if ( MembRes.pCS457==NULL ) return FALSE;
    if ( ChkRes.pCS457==NULL ) return FALSE;

    int nChkPos = MembD.Memb.ChIf.nChkPos;
    if ( nChkPos<0 || nChkPos>1 ) return FALSE;

    BOOL bVersionOK=FALSE;
    BOOL bCheck = (*m_pConsRes)(&MembD, &MembRes.pCS457->MembR[nChkPos], ChkRes.pCS457->pConsD, TSize_In, TSize_MembR, TSize_Res, bVersionOK);

    if ( !bCheck || !bVersionOK ) return FALSE;
    return TRUE;

}

BOOL CDgn_CSGManager_CS457_R1::Check_StrengthRes(CSG_MEMB_POSD_KSCE_LSD &MembD, CSG_MEMB_RESULT_D &MembRes, CSG_CHECK_RESULT_CODES &ChkRes)
{
    size_t TSize_In    = sizeof(CSG_MEMB_POSD_KSCE_LSD);
    size_t TSize_MembR = sizeof(CSG_MEMB_RES_CS457_D);
    size_t TSize_Res   = sizeof(CSG_STRN_RES_CS457);

    if ( m_pStrnRes == NULL ) return FALSE;
    if ( MembRes.pCS457==NULL ) return FALSE;
    if ( ChkRes.pCS457==NULL ) return FALSE;

    int nChkPos = MembD.Memb.ChIf.nChkPos;
    if ( nChkPos<0 || nChkPos>1 ) return FALSE;

    BOOL bVersionOK=FALSE;
    BOOL bCheck = (*m_pStrnRes)(&MembD, &MembRes.pCS457->MembR[nChkPos], ChkRes.pCS457->pStrnD, TSize_In, TSize_MembR, TSize_Res, bVersionOK);

    if ( !bCheck || !bVersionOK ) return FALSE;
    return TRUE;
}

BOOL CDgn_CSGManager_CS457_R1::Check_ServiceRes(CSG_MEMB_POSD_KSCE_LSD &MembD, CSG_MEMB_RESULT_D &MembRes, CSG_CHECK_RESULT_CODES &ChkRes)
{
    size_t TSize_In    = sizeof(CSG_MEMB_POSD_KSCE_LSD);
    size_t TSize_MembR = sizeof(CSG_MEMB_RES_CS457_D);
    size_t TSize_Res   = sizeof(CSG_SERV_RES_CS457);

    if ( m_pServRes == NULL ) return FALSE;
    if ( MembRes.pCS457==NULL ) return FALSE;
    if ( ChkRes.pCS457==NULL ) return FALSE;

    int nChkPos = MembD.Memb.ChIf.nChkPos;
    if ( nChkPos<0 || nChkPos>1 ) return FALSE;

    BOOL bVersionOK=FALSE;
    BOOL bCheck = (*m_pServRes)(&MembD, &MembRes.pCS457->MembR[nChkPos], ChkRes.pCS457->pServD, TSize_In, TSize_MembR, TSize_Res, bVersionOK);

    if ( !bCheck || !bVersionOK ) return FALSE;
    return TRUE;
}

BOOL CDgn_CSGManager_CS457_R1::Check_ShearConnRes(CSG_MEMB_POSD_KSCE_LSD &MembD, CSG_MEMB_RESULT_D &MembRes, CSG_CHECK_RESULT_CODES &ChkRes)
{
    size_t TSize_In    = sizeof(CSG_MEMB_POSD_KSCE_LSD);
    size_t TSize_MembR = sizeof(CSG_MEMB_RES_CS457_D);
    size_t TSize_Res   = sizeof(CSG_SCON_RES_CS457);

    if ( m_pSconRes == NULL ) return FALSE;
    if ( MembRes.pCS457==NULL ) return FALSE;
    if ( ChkRes.pCS457==NULL ) return FALSE;

    int nChkPos = MembD.Memb.ChIf.nChkPos;
    if ( nChkPos<0 || nChkPos>1 ) return FALSE;

    BOOL bVersionOK=FALSE;
    BOOL bCheck = FALSE;
    if ( MembD.aChkFor[0].nLcomType==0 )
    {
        bCheck = (*m_pSconRes)(&MembD, &MembRes.pCS457->MembR[nChkPos], ChkRes.pCS457->pSconD, TSize_In, TSize_MembR, TSize_Res, bVersionOK);
    }
    else
    {
        bCheck = (*m_pSconRes)(&MembD, &MembRes.pCS457->MembR[nChkPos], ChkRes.pCS457->pServSconD, TSize_In, TSize_MembR, TSize_Res, bVersionOK);
    }    

    if ( !bCheck || !bVersionOK ) return FALSE;
    return TRUE;
}

BOOL CDgn_CSGManager_CS457_R1::CalcEffectiveBreadthRatio()
{
    size_t TSize_Res   = sizeof(CSG_EFF_BREADTH_RATIO_SPAN_D);

    BOOL bVersionOK=FALSE;
    BOOL bCheck = (*m_pGParam)(&m_EffBreadthR, TSize_Res, bVersionOK);

    if ( !bCheck || !bVersionOK ) return FALSE;

    m_bIsCalc = TRUE;

    return TRUE;
}

BOOL CDgn_CSGManager_CS457_R1::CalcSlendernessFactor()
{
	size_t TSize_Res = sizeof(CSG_v_SLENDERNESS_FACTOR_TABLE);

	BOOL bVersionOK = FALSE;
	BOOL bCheck = (*m_pSFParam)(&m_SlendernessFactorT, TSize_Res, bVersionOK);

	if (!bCheck || !bVersionOK) return FALSE;

	//m_bIsCalc = TRUE;

	return TRUE;
}

BOOL CDgn_CSGManager_CS457_R1::GetEffBreadthRatio(CSG_EFF_BREADTH_RATIO_SPAN_D& rData)
{
    rData = m_EffBreadthR;
    return TRUE;
}

BOOL CDgn_CSGManager_CS457_R1::GetSlendernessFactorTable(CSG_v_SLENDERNESS_FACTOR_TABLE& rData)
{
	rData = m_SlendernessFactorT;
    return TRUE;
}
