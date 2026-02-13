// Dgn_OrthotropicDeckManager.cpp: implementation of the CDgn_OrthotropicDeckManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Dgn_OrthotropicDeckManager.h"

// #include "..\wg_db\DBDoc.h"
// #include "..\wg_db\AttrCtrl.h"
// #include "..\wg_db\PostCtrl.h"
// #include "..\wg_db\PCDesign.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDgn_OrthotropicDeckManager::CDgn_OrthotropicDeckManager() : CDgn_DllManager_Base()
{
//   m_pSetMembRes = NULL;
	m_pMemberRes   = NULL;
	m_pStrnStabRes = NULL;
	m_pKapaaCheck  = NULL;
	m_pFatiRes     = NULL;
	m_pGetTBval    = NULL;
}

CDgn_OrthotropicDeckManager::~CDgn_OrthotropicDeckManager()
{

}

BOOL CDgn_OrthotropicDeckManager::InitialData(int nDgnType, int nDgnCode)
{
	HINSTANCE hDllCalc = GetOrLoad_CalcDllHandle(nDgnType, nDgnCode);
	if(hDllCalc == NULL) return FALSE;

	switch (nDgnCode)
	{
		case SNiP_20503_84_SOD:
		case SP_35_13330_11_SOD:
		{

			m_pMemberRes = (PDGNENGINE_FUNC*)GetProcAddress(hDllCalc, "Get_SodMemberResult");
			if (!m_pMemberRes) return FALSE;
			 
			m_pStrnStabRes = (PDGNENGINE_FUNC3*)GetProcAddress(hDllCalc, "Check_SodStrengthStability");
			if(!m_pStrnStabRes) return FALSE;

			m_pKapaaCheck = (PDGNENGINE_FUNC5*)GetProcAddress(hDllCalc, "Check_SodKappaCheck");
			if(!m_pKapaaCheck) return FALSE;

			m_pFatiRes = (PDGNENGINE_FUNC3*)GetProcAddress(hDllCalc, "Check_SodFatigue");
			if(!m_pFatiRes) return FALSE;

			m_pGetTBval = (PDGNENGINE_SETD*)GetProcAddress(hDllCalc, "Get_TableValue");

			break;
		}
		default: ASSERT(0); break;
	}

	return TRUE;
}

void CDgn_OrthotropicDeckManager::Set_MemberResStlOrthDeckSNiP(STL_RES_BASE_SNiP_OD_SNiP &rData)
{
//   if(m_pSetMembRes == NULL) return;
//   
//   size_t TSize_Res   = sizeof(STL_RES_BASE_SNiP_OD_SNiP);
//   
//   BOOL bVersionOK=FALSE;
//   BOOL bCheck = (*m_pSetMembRes)(&rData, TSize_Res, bVersionOK);

}

BOOL CDgn_OrthotropicDeckManager::Check_MemberResSNiP(STL_MEMB_POSD_SNiP_OD &MembD, STL_RES_BASE_SNiP_OD_SNiP &rData)
{
	size_t TSize_In    = sizeof(STL_MEMB_POSD_SNiP_OD);
	size_t TSize_Res   = sizeof(STL_RES_BASE_SNiP_OD_SNiP);
	
	if(m_pMemberRes == NULL) return FALSE;	
	
	BOOL bVersionOK=FALSE;
	BOOL bCheck = (*m_pMemberRes)(&MembD, &rData, TSize_In, TSize_Res, bVersionOK);
	
	if(!bCheck || !bVersionOK) return FALSE;
	return TRUE;
}

BOOL CDgn_OrthotropicDeckManager::Check_StlOrthDeckSNiP(STL_MEMB_POSD_SNiP_OD &MembD, STL_RES_BASE_SNiP_OD_SNiP &MembResD, STL_MEMB_RES_STR_STA_SNiP_OD &rData)
{
	size_t TSize_In    = sizeof(STL_MEMB_POSD_SNiP_OD);
	size_t TSize_MembR = sizeof(STL_RES_BASE_SNiP_OD_SNiP);
	size_t TSize_Res   = sizeof(STL_MEMB_RES_STR_STA_SNiP_OD);
	
	if(m_pStrnStabRes == NULL) return FALSE;	
	
	BOOL bVersionOK=FALSE;
	BOOL bCheck = (*m_pStrnStabRes)(&MembD, &MembResD, &rData, TSize_In, TSize_MembR, TSize_Res, bVersionOK);

	if(!bCheck || !bVersionOK) return FALSE;
	return TRUE;
}

BOOL CDgn_OrthotropicDeckManager::Check_KappaF_StlOrthDeckSNiP(STL_MEMB_POSD_SNiP_OD &MembD, STL_RES_BASE_SNiP_OD_SNiP &MembResD, BOOL &bAllowKappaF, double &dRat_Max, aKAPAF_PonitInfo &aKappaFCheckRes)
{
	size_t TSize_In           = sizeof(STL_MEMB_POSD_SNiP_OD);
	size_t TSize_MembR        = sizeof(STL_RES_BASE_SNiP_OD_SNiP);
	size_t TSize_Res          = sizeof(BOOL);
	size_t TSize_Res_Rat      = sizeof(double);
	size_t TSize_Res_KappaF   = sizeof(aKAPAF_PonitInfo);

	if(m_pKapaaCheck == NULL) return FALSE;	

	BOOL bVersionOK=FALSE;
	BOOL bCheck = (*m_pKapaaCheck)(&MembD, &MembResD, &bAllowKappaF, &dRat_Max, &aKappaFCheckRes, TSize_In, TSize_MembR, TSize_Res, TSize_Res_Rat, TSize_Res_KappaF, bVersionOK);

	if(!bCheck || !bVersionOK) return FALSE;
	return TRUE;
}

BOOL CDgn_OrthotropicDeckManager::Check_StlOrthDeckFatigueSNiP(STL_MEMB_POSD_SNiP_OD &MembD, STL_RES_BASE_SNiP_OD_SNiP &MembResD, STL_MEMB_RES_SNiP_FAT &rData)
{
	size_t TSize_In    = sizeof(STL_MEMB_POSD_SNiP_OD);
	size_t TSize_MembR = sizeof(STL_RES_BASE_SNiP_OD_SNiP);
	size_t TSize_Res   = sizeof(STL_MEMB_RES_SNiP_FAT);
	
	if(m_pStrnStabRes == NULL) return FALSE;	
	
	BOOL bVersionOK=FALSE;
	BOOL bCheck = (*m_pFatiRes)(&MembD, &MembResD, &rData, TSize_In, TSize_MembR, TSize_Res, bVersionOK);
	
	if(!bCheck || !bVersionOK) return FALSE;
	return TRUE;
}


BOOL CDgn_OrthotropicDeckManager::GetTableValueSNiP(STL_SNiP_TB_D &rData)
{
	size_t TSizeTB = sizeof(STL_SNiP_TB_D);

	if (m_pGetTBval == NULL) return FALSE;

	BOOL bVersionOK=FALSE;
	BOOL bCheck = (*m_pGetTBval)(&rData, TSizeTB, bVersionOK);

	if(!bCheck || !bVersionOK) return FALSE;
	return TRUE;
}
