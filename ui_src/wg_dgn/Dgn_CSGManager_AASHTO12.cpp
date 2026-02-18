// Dgn_PSCManager.cpp: implementation of the CDgn_PSCManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Dgn_CSGManager_AASHTO12.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDgn_CSGManager_AASHTO12::CDgn_CSGManager_AASHTO12() : CDgn_CSGManager()
{
}

CDgn_CSGManager_AASHTO12::~CDgn_CSGManager_AASHTO12()
{

}

BOOL CDgn_CSGManager_AASHTO12::InitialData(int nDgnType, int nDgnCode)
{
	CDgn_CSGManager::InitialData(nDgnType, nDgnCode);

	HINSTANCE hDllCalc = GetOrLoad_CalcDllHandle(nDgnType, nDgnCode);

	if(hDllCalc == NULL) return FALSE;

	m_pMemberRes = (PDGNENGINE_FUNC*)GetProcAddress(hDllCalc, "Get_CsgMemberResultAASHTO_LRFD");
	if (!m_pMemberRes) return FALSE;

	m_pConsRes = (PDGNENGINE_FUNC3*)GetProcAddress(hDllCalc, "Check_CsgConstructibilityAASHTO_LRFD"); 
	if(!m_pConsRes) return FALSE;

	m_pStrnRes = (PDGNENGINE_FUNC3*)GetProcAddress(hDllCalc, "Check_CsgStrnengthAASHTO_LRFD"); 
	if(!m_pStrnRes) return FALSE;

	m_pServRes = (PDGNENGINE_FUNC3*)GetProcAddress(hDllCalc, "Check_CsgServiceAASHTO_LRFD"); 
	if(!m_pServRes) return FALSE;

	m_pFatiRes = (PDGNENGINE_FUNC3*)GetProcAddress(hDllCalc, "Check_CsgFatigueAASHTO_LRFD"); 
	if(!m_pFatiRes) return FALSE;

	m_pSconRes = (PDGNENGINE_FUNC3*)GetProcAddress(hDllCalc, "Check_CsgShearConnectorAASHTO_LRFD"); 
	if(!m_pSconRes) return FALSE;

	m_pStfnRes = (PDGNENGINE_FUNC3*)GetProcAddress(hDllCalc, "Check_CsgStiffenerAASHTO_LRFD"); 
	if(!m_pStfnRes) return FALSE;

	m_pBtfnRes = (PDGNENGINE_FUNC3*)GetProcAddress(hDllCalc, "Check_CsgBearingStiffenerAASHTO_LRFD"); 
	if(!m_pBtfnRes) return FALSE;

	return TRUE;
}

BOOL CDgn_CSGManager_AASHTO12::Check_MemberResult(CSG_MEMB_POSD_KSCE_LSD &MembD, CSG_MEMB_RESULT_D &rData)
{
	size_t TSize_In    = sizeof(CSG_MEMB_POSD_KSCE_LSD);
	size_t TSize_Res   = 0;

	if(m_pMemberRes == NULL) return FALSE;	
	if(rData.pAASHTO==NULL) return FALSE;

	int nChkPos = MembD.Memb.ChIf.nChkPos;
	if (nChkPos<0 || nChkPos>1) return FALSE;

	BOOL bVersionOK=FALSE;

	TSize_Res = sizeof(CSG_MEMB_RES_AASHTO_LRFD_D);
	BOOL bCheck = (*m_pMemberRes)(&MembD, &rData.pAASHTO->MembR[nChkPos], TSize_In, TSize_Res, bVersionOK);

	if(!bCheck || !bVersionOK) return FALSE;
	return TRUE;
}


BOOL CDgn_CSGManager_AASHTO12::Check_ConstructibilityRes(CSG_MEMB_POSD_KSCE_LSD &MembD, CSG_MEMB_RESULT_D &MembRes, CSG_CHECK_RESULT_CODES &ChkRes)
{
	size_t TSize_In    = sizeof(CSG_MEMB_POSD_KSCE_LSD);
	size_t TSize_MembR = sizeof(CSG_MEMB_RES_AASHTO_LRFD_D);
	size_t TSize_Res   = sizeof(CSG_MEMB_RES_AASHTO_LRFD_CONSTRUCT);

	if(m_pConsRes == NULL) return FALSE;	
	if (MembRes.pAASHTO==NULL) return FALSE;
	if (ChkRes.pAASHTO==NULL) return FALSE;

	int nChkPos = MembD.Memb.ChIf.nChkPos;
	if (nChkPos<0 || nChkPos>1) return FALSE;

	BOOL bVersionOK=FALSE;
	BOOL bCheck = (*m_pConsRes)(&MembD, &MembRes.pAASHTO->MembR[nChkPos], &ChkRes.pAASHTO->ConsD, TSize_In, TSize_MembR, TSize_Res, bVersionOK);

	if(!bCheck || !bVersionOK) return FALSE;
	return TRUE;

}

BOOL CDgn_CSGManager_AASHTO12::Check_StrengthRes(CSG_MEMB_POSD_KSCE_LSD &MembD, CSG_MEMB_RESULT_D &MembRes, CSG_CHECK_RESULT_CODES &ChkRes)
{
	size_t TSize_In    = sizeof(CSG_MEMB_POSD_KSCE_LSD);
	size_t TSize_MembR = sizeof(CSG_MEMB_RES_AASHTO_LRFD_D);
	size_t TSize_Res   = sizeof(CSG_MEMB_RES_AASHTO_LRFD_STR_D);

	if(m_pStrnRes == NULL) return FALSE;	
	if (MembRes.pAASHTO==NULL) return FALSE;
	if (ChkRes.pAASHTO==NULL) return FALSE;

	int nChkPos = MembD.Memb.ChIf.nChkPos;
	if (nChkPos<0 || nChkPos>1) return FALSE;

	BOOL bVersionOK=FALSE;
	BOOL bCheck = (*m_pStrnRes)(&MembD, &MembRes.pAASHTO->MembR[nChkPos], &ChkRes.pAASHTO->StrengthD, TSize_In, TSize_MembR, TSize_Res, bVersionOK);

	if(!bCheck || !bVersionOK) return FALSE;
	return TRUE;
}

BOOL CDgn_CSGManager_AASHTO12::Check_ServiceRes(CSG_MEMB_POSD_KSCE_LSD &MembD, CSG_MEMB_RESULT_D &MembRes, CSG_CHECK_RESULT_CODES &ChkRes)
{
	size_t TSize_In    = sizeof(CSG_MEMB_POSD_KSCE_LSD);
	size_t TSize_MembR = sizeof(CSG_MEMB_RES_AASHTO_LRFD_D);
	size_t TSize_Res   = sizeof(CSG_MEMB_RES_AASHTO_LRFD_SER_D);

	if(m_pServRes == NULL) return FALSE;	
	if (MembRes.pAASHTO==NULL) return FALSE;
	if (ChkRes.pAASHTO==NULL) return FALSE;

	int nChkPos = MembD.Memb.ChIf.nChkPos;
	if (nChkPos<0 || nChkPos>1) return FALSE;

	BOOL bVersionOK=FALSE;
	BOOL bCheck = (*m_pServRes)(&MembD, &MembRes.pAASHTO->MembR[nChkPos], &ChkRes.pAASHTO->ServiceD, TSize_In, TSize_MembR, TSize_Res, bVersionOK);

	if(!bCheck || !bVersionOK) return FALSE;
	return TRUE;
}

BOOL CDgn_CSGManager_AASHTO12::Check_FatigueRes(CSG_MEMB_POSD_KSCE_LSD &MembD, CSG_MEMB_RESULT_D &MembRes, CSG_CHECK_RESULT_CODES &ChkRes)
{
	size_t TSize_In    = sizeof(CSG_MEMB_POSD_KSCE_LSD);
	size_t TSize_MembR = sizeof(CSG_MEMB_RES_AASHTO_LRFD_D);
	size_t TSize_Res   = sizeof(CSG_MEMB_RES_AASHTO_LRFD_FATIGUE_D);

	if(m_pFatiRes == NULL) return FALSE;	
	if (MembRes.pAASHTO==NULL) return FALSE;
	if (ChkRes.pAASHTO==NULL) return FALSE;

	int nChkPos = MembD.Memb.ChIf.nChkPos;
	if (nChkPos<0 || nChkPos>1) return FALSE;

	BOOL bVersionOK=FALSE;
	BOOL bCheck = (*m_pFatiRes)(&MembD, &MembRes.pAASHTO->MembR[nChkPos], &ChkRes.pAASHTO->FatigueD, TSize_In, TSize_MembR, TSize_Res, bVersionOK);

	if(!bCheck || !bVersionOK) return FALSE;
	return TRUE;
}

BOOL CDgn_CSGManager_AASHTO12::Check_ShearConnRes(CSG_MEMB_POSD_KSCE_LSD &MembD, CSG_MEMB_RESULT_D &MembRes, CSG_CHECK_RESULT_CODES &ChkRes)
{
	size_t TSize_In    = sizeof(CSG_MEMB_POSD_KSCE_LSD);
	size_t TSize_MembR = sizeof(CSG_MEMB_RES_AASHTO_LRFD_D);
	size_t TSize_Res   = sizeof(CSG_MEMB_RES_AASHTO_LRFD_SHEAR_CONN_D);

	if(m_pSconRes == NULL) return FALSE;	
	if (MembRes.pAASHTO==NULL) return FALSE;
	if (ChkRes.pAASHTO==NULL) return FALSE;

	int nChkPos = MembD.Memb.ChIf.nChkPos;
	if (nChkPos<0 || nChkPos>1) return FALSE;

	BOOL bVersionOK=FALSE;
	BOOL bCheck = (*m_pSconRes)(&MembD, &MembRes.pAASHTO->MembR[nChkPos], &ChkRes.pAASHTO->ShearConnD, TSize_In, TSize_MembR, TSize_Res, bVersionOK);

	if(!bCheck || !bVersionOK) return FALSE;
	return TRUE;
}

BOOL CDgn_CSGManager_AASHTO12::Check_StiffenerRes(CSG_MEMB_POSD_KSCE_LSD &MembD, CSG_MEMB_RESULT_D &MembRes, CSG_CHECK_RESULT_CODES &ChkRes)
{
	size_t TSize_In    = sizeof(CSG_MEMB_POSD_KSCE_LSD);
	size_t TSize_MembR = sizeof(CSG_MEMB_RES_AASHTO_LRFD_D);
	size_t TSize_Res   = sizeof(CSG_MEMB_RES_AASHTO_LRFD_LSTIFF_D);

	if(m_pStfnRes == NULL) return FALSE;	
	if (MembRes.pAASHTO==NULL) return FALSE;
	if (ChkRes.pAASHTO==NULL) return FALSE;

	int nChkPos = MembD.Memb.ChIf.nChkPos;
	if (nChkPos<0 || nChkPos>1) return FALSE;

	BOOL bVersionOK=FALSE;
	BOOL bCheck = (*m_pStfnRes)(&MembD, &MembRes.pAASHTO->MembR[nChkPos], &ChkRes.pAASHTO->HS_D, TSize_In, TSize_MembR, TSize_Res, bVersionOK);

	if(!bCheck || !bVersionOK) return FALSE;
	return TRUE;
}

BOOL CDgn_CSGManager_AASHTO12::Check_BearingStiffenerRes(CSG_MEMB_POSD_KSCE_LSD &MembD, CSG_MEMB_RESULT_D &MembRes, CSG_CHECK_RESULT_CODES &ChkRes)
{
	size_t TSize_In    = sizeof(CSG_MEMB_POSD_KSCE_LSD);
	size_t TSize_MembR = sizeof(CSG_MEMB_RES_AASHTO_LRFD_D);
	size_t TSize_Res   = sizeof(CSG_MEMB_RES_AASHTO_LRFD_BEARING_STIFFENER_D);

	if(m_pBtfnRes == NULL) return FALSE;	
	if (MembRes.pAASHTO==NULL) return FALSE;
	if (ChkRes.pAASHTO==NULL) return FALSE;

	int nChkPos = MembD.Memb.ChIf.nChkPos;
	if (nChkPos<0 || nChkPos>1) return FALSE;

	BOOL bVersionOK=FALSE;
	BOOL bCheck = (*m_pBtfnRes)(&MembD, &MembRes.pAASHTO->MembR[nChkPos], &ChkRes.pAASHTO->BS_D, TSize_In, TSize_MembR, TSize_Res, bVersionOK);

	if(!bCheck || !bVersionOK) return FALSE;
	return TRUE;
}



//////////////////////////////////////////////////////////////////////////

CDgn_CSGManager_AASHTO07::CDgn_CSGManager_AASHTO07() : CDgn_CSGManager_AASHTO12()
{
}

CDgn_CSGManager_AASHTO07::~CDgn_CSGManager_AASHTO07()
{

}
