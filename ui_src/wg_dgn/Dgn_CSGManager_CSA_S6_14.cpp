// Dgn_CSGManager_CSA_S6_14.cpp: implementation of the CDgn_CSGManager_CSA_S6_14 class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Dgn_CSGManager_CSA_S6_14.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDgn_CSGManager_CSA_S6_14::CDgn_CSGManager_CSA_S6_14() : CDgn_CSGManager()
{
}

CDgn_CSGManager_CSA_S6_14::~CDgn_CSGManager_CSA_S6_14()
{

}

BOOL CDgn_CSGManager_CSA_S6_14::InitialData(int nDgnType, int nDgnCode)
{
	CDgn_CSGManager::InitialData(nDgnType, nDgnCode);

	HINSTANCE hDllCalc = GetOrLoad_CalcDllHandle(nDgnType, nDgnCode);

	if(hDllCalc == NULL) return FALSE;

	m_pMemberRes = (PDGNENGINE_FUNC*)GetProcAddress(hDllCalc, "Get_CsgMemberResultCSA_S6");
	if (!m_pMemberRes) return FALSE;

	m_pConsRes = (PDGNENGINE_FUNC3*)GetProcAddress(hDllCalc, "Check_CsgConstructibilityCSA_S6"); 
	if(!m_pConsRes) return FALSE;

	m_pStrnRes = (PDGNENGINE_FUNC3*)GetProcAddress(hDllCalc, "Check_CsgStrnengthCSA_S6"); 
	if(!m_pStrnRes) return FALSE;

	m_pServRes = (PDGNENGINE_FUNC3*)GetProcAddress(hDllCalc, "Check_CsgServiceCSA_S6"); 
	if(!m_pServRes) return FALSE;

	m_pFatiRes = (PDGNENGINE_FUNC3*)GetProcAddress(hDllCalc, "Check_CsgFatigueCSA_S6"); 
	if(!m_pFatiRes) return FALSE;

	m_pSconRes = (PDGNENGINE_FUNC3*)GetProcAddress(hDllCalc, "Check_CsgShearConnectorCSA_S6"); 
	if(!m_pSconRes) return FALSE;

	m_pStfnRes = (PDGNENGINE_FUNC3*)GetProcAddress(hDllCalc, "Check_CsgStiffenerCSA_S6"); 
	if(!m_pStfnRes) return FALSE;

	m_pBtfnRes = (PDGNENGINE_FUNC3*)GetProcAddress(hDllCalc, "Check_CsgBearingStiffenerCSA_S6"); 
	if(!m_pBtfnRes) return FALSE;

	return TRUE;
}

BOOL CDgn_CSGManager_CSA_S6_14::Check_MemberResult(CSG_MEMB_POSD_KSCE_LSD &MembD, CSG_MEMB_RESULT_D &rData)
{	
	if(m_pMemberRes == NULL) return FALSE;	
	if (rData.pCSA==NULL) return FALSE;

	size_t TSize_In = sizeof(CSG_MEMB_POSD_KSCE_LSD);
	size_t TSize_Res = sizeof(CSG_MEMB_RES_CSA_S6_D);

	int nChkPos = MembD.Memb.ChIf.nChkPos;
	if (nChkPos<0 || nChkPos>1) return FALSE;

	BOOL bVersionOK=FALSE;
	BOOL bCheck = (*m_pMemberRes)(&MembD, &rData.pCSA->MembR[nChkPos], TSize_In, TSize_Res, bVersionOK);

	if(!bCheck || !bVersionOK) return FALSE;
	return TRUE;
}

BOOL CDgn_CSGManager_CSA_S6_14::Check_ConstructibilityRes(CSG_MEMB_POSD_KSCE_LSD &MembD, CSG_MEMB_RESULT_D &MembRes, CSG_CHECK_RESULT_CODES &ChkRes)
{
	if(m_pConsRes == NULL) return FALSE;	
	if (MembRes.pCSA==NULL) return FALSE;
	if (ChkRes.pCSA==NULL) return FALSE;

	size_t TSize_In = sizeof(CSG_MEMB_POSD_KSCE_LSD);
	size_t TSize_MembR = sizeof(CSG_MEMB_RES_CSA_S6_D);
	size_t TSize_Res = sizeof(CSG_CHECK_RES_CSA_S6);

	int nChkPos = MembD.Memb.ChIf.nChkPos;
	if (nChkPos<0 || nChkPos>1) return FALSE;

	BOOL bVersionOK=FALSE;
	BOOL bCheck = (*m_pConsRes)(&MembD, &MembRes.pCSA->MembR[nChkPos], ChkRes.pCSA, TSize_In, TSize_MembR, TSize_Res, bVersionOK);

	if(!bCheck || !bVersionOK) return FALSE;
	return TRUE;
}

BOOL CDgn_CSGManager_CSA_S6_14::Check_StrengthRes(CSG_MEMB_POSD_KSCE_LSD &MembD, CSG_MEMB_RESULT_D &MembRes, CSG_CHECK_RESULT_CODES &ChkRes)
{	
	if(m_pStrnRes == NULL) return FALSE;	
	if (MembRes.pCSA==NULL) return FALSE;
	if (ChkRes.pCSA==NULL) return FALSE;

	size_t TSize_In = sizeof(CSG_MEMB_POSD_KSCE_LSD);
	size_t TSize_MembR = sizeof(CSG_MEMB_RES_CSA_S6_D);
	size_t TSize_Res = sizeof(CSG_CHECK_RES_CSA_S6);

	int nChkPos = MembD.Memb.ChIf.nChkPos;
	if (nChkPos<0 || nChkPos>1) return FALSE;

	BOOL bVersionOK=FALSE;
	BOOL bCheck = (*m_pStrnRes)(&MembD, &MembRes.pCSA->MembR[nChkPos], ChkRes.pCSA, TSize_In, TSize_MembR, TSize_Res, bVersionOK);

	if(!bCheck || !bVersionOK) return FALSE;
	return TRUE;
}


BOOL CDgn_CSGManager_CSA_S6_14::Check_ServiceRes(CSG_MEMB_POSD_KSCE_LSD &MembD, CSG_MEMB_RESULT_D &MembRes, CSG_CHECK_RESULT_CODES &ChkRes)
{
	if(m_pServRes == NULL) return FALSE;	
	if (MembRes.pCSA==NULL) return FALSE;
	if (ChkRes.pCSA==NULL) return FALSE;

	size_t TSize_In = sizeof(CSG_MEMB_POSD_KSCE_LSD);
	size_t TSize_MembR = sizeof(CSG_MEMB_RES_CSA_S6_D);
	size_t TSize_Res = sizeof(CSG_CHECK_RES_CSA_S6);

	int nChkPos = MembD.Memb.ChIf.nChkPos;
	if (nChkPos<0 || nChkPos>1) return FALSE;

	BOOL bVersionOK=FALSE;
	BOOL bCheck = (*m_pServRes)(&MembD, &MembRes.pCSA->MembR[nChkPos], ChkRes.pCSA, TSize_In, TSize_MembR, TSize_Res, bVersionOK); // Check_CsgServiceCSA_S6

	if(!bCheck || !bVersionOK) return FALSE;
	return TRUE;
}

BOOL CDgn_CSGManager_CSA_S6_14::Check_FatigueRes(CSG_MEMB_POSD_KSCE_LSD &MembD, CSG_MEMB_RESULT_D &MembRes, CSG_CHECK_RESULT_CODES &ChkRes)
{
	if(m_pFatiRes == NULL) return FALSE;	
	if (MembRes.pCSA==NULL) return FALSE;
	if (ChkRes.pCSA==NULL) return FALSE;

	size_t TSize_In = sizeof(CSG_MEMB_POSD_KSCE_LSD);
	size_t TSize_MembR = sizeof(CSG_MEMB_RES_CSA_S6_D);
	size_t TSize_Res = sizeof(CSG_CHECK_RES_CSA_S6);

	int nChkPos = MembD.Memb.ChIf.nChkPos;
	if (nChkPos<0 || nChkPos>1) return FALSE;

	BOOL bVersionOK=FALSE;
	BOOL bCheck = (*m_pFatiRes)(&MembD, &MembRes.pCSA->MembR[nChkPos], ChkRes.pCSA, TSize_In, TSize_MembR, TSize_Res, bVersionOK);   // Check_CsgFatigueCSA_S6

	if(!bCheck || !bVersionOK) return FALSE;
	return TRUE;
}

BOOL CDgn_CSGManager_CSA_S6_14::Check_ShearConnRes(CSG_MEMB_POSD_KSCE_LSD &MembD, CSG_MEMB_RESULT_D &MembRes, CSG_CHECK_RESULT_CODES &ChkRes)
{
	if (m_pSconRes == NULL) return FALSE;
	if (MembRes.pCSA == NULL) return FALSE;
	if (ChkRes.pCSA == NULL) return FALSE;

	int nChkPos = MembD.Memb.ChIf.nChkPos;
	if (nChkPos < 0 || nChkPos>1) return FALSE;

	size_t TSize_In = sizeof(CSG_MEMB_POSD_KSCE_LSD);
	size_t TSize_MembR = sizeof(CSG_MEMB_RES_CSA_S6_D);
	size_t TSize_Res = sizeof(CSG_CHECK_RES_CSA_S6);	

	BOOL bVersionOK = FALSE;
	BOOL bCheck = (*m_pSconRes)(&MembD, &MembRes.pCSA->MembR[nChkPos], ChkRes.pCSA, TSize_In, TSize_MembR, TSize_Res, bVersionOK);   // Check_CsgShearConnectorCSA_S6

	if(!bCheck || !bVersionOK) return FALSE;
	return TRUE;
}

BOOL CDgn_CSGManager_CSA_S6_14::Check_BearingStiffenerRes(CSG_MEMB_POSD_KSCE_LSD& MembD, CSG_MEMB_RESULT_D& MembRes, CSG_CHECK_RESULT_CODES& ChkRes)
{
	if (m_pBtfnRes == NULL) return FALSE;
	if (MembRes.pCSA == NULL) return FALSE;
	if (ChkRes.pCSA == NULL) return FALSE;

	const int nChkPos = MembD.Memb.ChIf.nChkPos;
	if (nChkPos < 0 || nChkPos>1) return FALSE;

	size_t TSize_In = sizeof(CSG_MEMB_POSD_KSCE_LSD);
	size_t TSize_MembR = sizeof(CSG_MEMB_RES_CSA_S6_D);
	size_t TSize_Res = sizeof(CSG_CHECK_RES_CSA_S6);
		
	BOOL bVersionOK = FALSE;
	BOOL bCheck = (*m_pBtfnRes)(&MembD, &MembRes.pCSA->MembR[nChkPos], ChkRes.pCSA, TSize_In, TSize_MembR, TSize_Res, bVersionOK);

	if (!bCheck || !bVersionOK) return FALSE;
	return TRUE;
}
