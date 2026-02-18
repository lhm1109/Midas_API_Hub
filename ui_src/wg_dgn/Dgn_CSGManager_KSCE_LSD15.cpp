// Dgn_CSGManager_KSCE_LSD15.cpp: implementation of the CDgn_CSGManager_KSCE_LSD15 class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Dgn_CSGManager_KSCE_LSD15.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDgn_CSGManager_KSCE_LSD15::CDgn_CSGManager_KSCE_LSD15() : CDgn_CSGManager()
{
}

CDgn_CSGManager_KSCE_LSD15::~CDgn_CSGManager_KSCE_LSD15()
{

}

BOOL CDgn_CSGManager_KSCE_LSD15::InitialData(int nDgnType, int nDgnCode)
{
	CDgn_CSGManager::InitialData(nDgnType, nDgnCode);

	HINSTANCE hDllCalc = GetOrLoad_CalcDllHandle(nDgnType, nDgnCode);

	if(hDllCalc == NULL) return FALSE;

	m_pMemberRes = (PDGNENGINE_FUNC*)GetProcAddress(hDllCalc, "Get_CsgMemberResultKSCE_LSD12");
	if (!m_pMemberRes) return FALSE;

	m_pConsRes = (PDGNENGINE_FUNC3*)GetProcAddress(hDllCalc, "Check_CsgConstructibilityKSCE_LSD12"); 
	if(!m_pConsRes) return FALSE;

	m_pStrnRes = (PDGNENGINE_FUNC3*)GetProcAddress(hDllCalc, "Check_CsgStrnengthKSCE_LSD12"); 
	if(!m_pStrnRes) return FALSE;

	m_pServRes = (PDGNENGINE_FUNC3*)GetProcAddress(hDllCalc, "Check_CsgServiceKSCE_LSD12"); 
	if(!m_pServRes) return FALSE;

	m_pFatiRes = (PDGNENGINE_FUNC3*)GetProcAddress(hDllCalc, "Check_CsgFatigueKSCE_LSD12"); 
	if(!m_pFatiRes) return FALSE;

	m_pCsdAsdRes = (PDGNENGINE_FUNC*)GetProcAddress(hDllCalc, "CSD_ASD_Execute_Chk");
	if (!m_pCsdAsdRes) return FALSE;

	return TRUE;
}

BOOL CDgn_CSGManager_KSCE_LSD15::Check_MemberResult(CSG_MEMB_POSD_KSCE_LSD &MembD, CSG_MEMB_RESULT_D &rData)
{
	size_t TSize_In    = sizeof(CSG_MEMB_POSD_KSCE_LSD);
	size_t TSize_Res   = 0;

	if(m_pMemberRes == NULL) return FALSE;	
	if(rData.pLSD==NULL) return FALSE;

	int nChkPos = MembD.Memb.ChIf.nChkPos;
	if (nChkPos<0 || nChkPos>1) return FALSE;

	BOOL bVersionOK=FALSE;

	TSize_Res = sizeof(CSG_MEMB_RES_KSCE_LSD12_D);
	BOOL bCheck = (*m_pMemberRes)(&MembD, &rData.pLSD->MembR[nChkPos], TSize_In, TSize_Res, bVersionOK);

	if(!bCheck || !bVersionOK) return FALSE;
	return TRUE;
}

BOOL CDgn_CSGManager_KSCE_LSD15::Check_ConstructibilityRes(CSG_MEMB_POSD_KSCE_LSD &MembD, CSG_MEMB_RESULT_D &MembRes, CSG_CHECK_RESULT_CODES &ChkRes)
{
	size_t TSize_In    = sizeof(CSG_MEMB_POSD_KSCE_LSD);
	size_t TSize_MembR = sizeof(CSG_MEMB_RES_KSCE_LSD12_D);
	size_t TSize_Res   = sizeof(CSG_CONS_RES_KSCE_LSD12);

	if(m_pConsRes == NULL) return FALSE;	
	if (MembRes.pLSD==NULL) return FALSE;
	if (ChkRes.pLSD==NULL) return FALSE;

	int nChkPos = MembD.Memb.ChIf.nChkPos;
	if (nChkPos<0 || nChkPos>1) return FALSE;

	BOOL bVersionOK=FALSE;
	BOOL bCheck = (*m_pConsRes)(&MembD, &MembRes.pLSD->MembR[nChkPos], &ChkRes.pLSD->ConsD, TSize_In, TSize_MembR, TSize_Res, bVersionOK);

	if(!bCheck || !bVersionOK) return FALSE;
	return TRUE;
}

BOOL CDgn_CSGManager_KSCE_LSD15::Check_StrengthRes(CSG_MEMB_POSD_KSCE_LSD &MembD, CSG_MEMB_RESULT_D &MembRes, CSG_CHECK_RESULT_CODES &ChkRes)
{
	size_t TSize_In    = sizeof(CSG_MEMB_POSD_KSCE_LSD);
	size_t TSize_MembR = sizeof(CSG_MEMB_RES_KSCE_LSD12_D);
	size_t TSize_Res   = sizeof(CSG_STRN_RES_KSCE_LSD12);

	if(m_pStrnRes == NULL) return FALSE;	
	if (MembRes.pLSD==NULL) return FALSE;
	if (ChkRes.pLSD==NULL) return FALSE;

	int nChkPos = MembD.Memb.ChIf.nChkPos;
	if (nChkPos<0 || nChkPos>1) return FALSE;

	BOOL bVersionOK=FALSE;
	BOOL bCheck = (*m_pStrnRes)(&MembD, &MembRes.pLSD->MembR[nChkPos], &ChkRes.pLSD->StrnD, TSize_In, TSize_MembR, TSize_Res, bVersionOK);

	if(!bCheck || !bVersionOK) return FALSE;
	return TRUE;
}

BOOL CDgn_CSGManager_KSCE_LSD15::Check_ServiceRes(CSG_MEMB_POSD_KSCE_LSD &MembD, CSG_MEMB_RESULT_D &MembRes, CSG_CHECK_RESULT_CODES &ChkRes)
{
	size_t TSize_In    = sizeof(CSG_MEMB_POSD_KSCE_LSD);
	size_t TSize_MembR = sizeof(CSG_MEMB_RES_KSCE_LSD12_D);
	size_t TSize_Res   = sizeof(CSG_SERV_RES_KSCE_LSD12);

	if(m_pServRes == NULL) return FALSE;	
	if (MembRes.pLSD==NULL) return FALSE;
	if (ChkRes.pLSD==NULL) return FALSE;

	int nChkPos = MembD.Memb.ChIf.nChkPos;
	if (nChkPos<0 || nChkPos>1) return FALSE;

	BOOL bVersionOK=FALSE;
	BOOL bCheck = (*m_pServRes)(&MembD, &MembRes.pLSD->MembR[nChkPos], &ChkRes.pLSD->ServD, TSize_In, TSize_MembR, TSize_Res, bVersionOK);

	if(!bCheck || !bVersionOK) return FALSE;
	return TRUE;
}

BOOL CDgn_CSGManager_KSCE_LSD15::Check_FatigueRes(CSG_MEMB_POSD_KSCE_LSD &MembD, CSG_MEMB_RESULT_D &MembRes, CSG_CHECK_RESULT_CODES &ChkRes)
{
	size_t TSize_In    = sizeof(CSG_MEMB_POSD_KSCE_LSD);
	size_t TSize_MembR = sizeof(CSG_MEMB_RES_KSCE_LSD12_D);
	size_t TSize_Res   = sizeof(CSG_FATI_RES_KSCE_LSD12);

	if(m_pFatiRes== NULL) return FALSE;	
	if (MembRes.pLSD==NULL) return FALSE;
	if (ChkRes.pLSD==NULL) return FALSE;

	int nChkPos = MembD.Memb.ChIf.nChkPos;
	if (nChkPos<0 || nChkPos>1) return FALSE;

	BOOL bVersionOK=FALSE;
	BOOL bCheck = (*m_pFatiRes)(&MembD, &MembRes.pLSD->MembR[nChkPos], &ChkRes.pLSD->FatiD, TSize_In, TSize_MembR, TSize_Res, bVersionOK);

	if(!bCheck || !bVersionOK) return FALSE;
	return TRUE;
}
