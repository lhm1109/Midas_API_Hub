// Dgn_PSCManager.cpp: implementation of the CDgn_PSCManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Dgn_CSGManager.h"

// #include "..\wg_db\DBDoc.h"
// #include "..\wg_db\AttrCtrl.h"
// #include "..\wg_db\PostCtrl.h"
// #include "..\wg_db\PCDesign.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDgn_CSGManager::CDgn_CSGManager() : CDgn_DllManager_Base()
{
	m_pSetMembRes = NULL;
	m_pMemberRes  = NULL;
	m_pConsRes = NULL;
	m_pStrnRes = NULL;
	m_pServRes = NULL;
	m_pFatiRes = NULL;
	m_pCsdAsdRes = NULL;
    m_pGParam = NULL;

	m_nDgnCode = 0;
}

CDgn_CSGManager::~CDgn_CSGManager()
{

}

BOOL CDgn_CSGManager::InitialData(int nDgnType, int nDgnCode)
{
	HINSTANCE hDllCalc = GetOrLoad_CalcDllHandle(nDgnType, nDgnCode);

	if(hDllCalc == NULL) return FALSE;

	m_nDgnCode = nDgnCode;

	switch (nDgnCode)
	{
		case KSCE_ASD10_CSG:
		case KSCE_RAIL_ASD11_CSG:
		{		
			m_pCsdAsdRes = (PDGNENGINE_FUNC*)GetProcAddress(hDllCalc, "CSD_ASD_Execute_Chk");
			if (!m_pMemberRes) return FALSE;
			
			break;
		}
		default: break;
	}

	return TRUE;
}


void CDgn_CSGManager::Set_MemberResKSCE_LSD12(CSG_MEMB_RES_KSCE_LSD12_D &rData)
{
	if(m_pSetMembRes == NULL) return;

	size_t TSize_Res   = sizeof(CSG_MEMB_RES_KSCE_LSD12_D);

	BOOL bVersionOK=FALSE;
	BOOL bCheck = (*m_pSetMembRes)(&rData, TSize_Res, bVersionOK);

}

BOOL CDgn_CSGManager::Check_MemberResKSCE_LSD12(CSG_MEMB_POSD_KSCE_LSD &MembD, CSG_MEMB_RES_KSCE_LSD12_D &rData)
{
	size_t TSize_In    = sizeof(CSG_MEMB_POSD_KSCE_LSD);
	size_t TSize_Res   = sizeof(CSG_MEMB_RES_KSCE_LSD12_D);
	
	if(m_pMemberRes == NULL) return FALSE;	
	
	BOOL bVersionOK=FALSE;
	BOOL bCheck = (*m_pMemberRes)(&MembD, &rData, TSize_In, TSize_Res, bVersionOK);

	if(!bCheck || !bVersionOK) return FALSE;
	return TRUE;
}

BOOL CDgn_CSGManager::Check_ConstructibilityKSCE_LSD12(CSG_MEMB_POSD_KSCE_LSD &MembD, CSG_MEMB_RES_KSCE_LSD12_D &MembResD, CSG_CONS_RES_KSCE_LSD12 &rData)
{
	size_t TSize_In    = sizeof(CSG_MEMB_POSD_KSCE_LSD);
	size_t TSize_MembR = sizeof(CSG_MEMB_RES_KSCE_LSD12_D);
	size_t TSize_Res   = sizeof(CSG_CONS_RES_KSCE_LSD12);
	
	if(m_pConsRes == NULL) return FALSE;	
	
	BOOL bVersionOK=FALSE;
	BOOL bCheck = (*m_pConsRes)(&MembD, &MembResD, &rData, TSize_In, TSize_MembR, TSize_Res, bVersionOK);

	if(!bCheck || !bVersionOK) return FALSE;
	return TRUE;

}

BOOL CDgn_CSGManager::Check_StrengthResKSCE_LSD12(CSG_MEMB_POSD_KSCE_LSD &MembD, CSG_MEMB_RES_KSCE_LSD12_D &MembResD, CSG_STRN_RES_KSCE_LSD12 &rData)
{
	size_t TSize_In    = sizeof(CSG_MEMB_POSD_KSCE_LSD);
	size_t TSize_MembR = sizeof(CSG_MEMB_RES_KSCE_LSD12_D);
	size_t TSize_Res   = sizeof(CSG_STRN_RES_KSCE_LSD12);
	
	if(m_pStrnRes == NULL) return FALSE;	
	
	BOOL bVersionOK=FALSE;
	BOOL bCheck = (*m_pStrnRes)(&MembD, &MembResD, &rData, TSize_In, TSize_MembR, TSize_Res, bVersionOK);

	if(!bCheck || !bVersionOK) return FALSE;
	return TRUE;
}

BOOL CDgn_CSGManager::Check_StrengthResKSCE_ASD(int nDgnCode, CSG_MEMB_POSD_KSCE_LSD &MembDTemp, STL_KR_RES_D &rData)
{

	CSG_MEMB_POSD_KR_D MembD;
	MembD.strDgnCode = CDBLib::GetCsgCodeName(nDgnCode);
	MembD.nCalcType = MembDTemp.nCalcType; 
	MembD.nSelUnit  = MembDTemp.nSelUnit; 
	MembD.Memb      = MembDTemp.Memb;  
	MembD.aUlmf.Copy(MembDTemp.aUlmf);
	MembD.aChkFor.Copy(MembDTemp.aChkFor);

	size_t TSize_In    = sizeof(CSG_MEMB_POSD_KR_D);
	size_t TSize_Res   = sizeof(STL_KR_RES_D);
	
	if(m_pCsdAsdRes == NULL) return FALSE;	
	
	BOOL bVersionOK=FALSE;
	BOOL bCheck = (*m_pCsdAsdRes)(&MembD, &rData, TSize_In, TSize_Res, bVersionOK);
	
	if(!bCheck || !bVersionOK) return FALSE;
	return TRUE;
}


BOOL CDgn_CSGManager::Check_ServiceResKSCE_LSD12(CSG_MEMB_POSD_KSCE_LSD &MembD, CSG_MEMB_RES_KSCE_LSD12_D &MembResD, CSG_SERV_RES_KSCE_LSD12 &rData)
{
	size_t TSize_In    = sizeof(CSG_MEMB_POSD_KSCE_LSD);
	size_t TSize_MembR = sizeof(CSG_MEMB_RES_KSCE_LSD12_D);
	size_t TSize_Res   = sizeof(CSG_SERV_RES_KSCE_LSD12);
	
	if(m_pServRes == NULL) return FALSE;	
	
	BOOL bVersionOK=FALSE;
	BOOL bCheck = (*m_pServRes)(&MembD, &MembResD, &rData, TSize_In, TSize_MembR, TSize_Res, bVersionOK);

	if(!bCheck || !bVersionOK) return FALSE;
	return TRUE;
}

BOOL CDgn_CSGManager::Check_FatigueResKSCE_LSD12(CSG_MEMB_POSD_KSCE_LSD &MembD, CSG_MEMB_RES_KSCE_LSD12_D &MembResD, CSG_FATI_RES_KSCE_LSD12 &rData)
{
	size_t TSize_In    = sizeof(CSG_MEMB_POSD_KSCE_LSD);
	size_t TSize_MembR = sizeof(CSG_MEMB_RES_KSCE_LSD12_D);
	size_t TSize_Res   = sizeof(CSG_FATI_RES_KSCE_LSD12);
	
	if(m_pFatiRes== NULL) return FALSE;	
	
	BOOL bVersionOK=FALSE;
	BOOL bCheck = (*m_pFatiRes)(&MembD, &MembResD, &rData, TSize_In, TSize_MembR, TSize_Res, bVersionOK);

	if(!bCheck || !bVersionOK) return FALSE;
	return TRUE;
}

void CDgn_CSGManager::Set_MemberResKSSC_LSD2014(CSG_MEMB_RES_AASHTO_LRFD_D &rData)
{
	if(m_pSetMembRes == NULL) return;

	size_t TSize_Res   = sizeof(CSG_MEMB_RES_AASHTO_LRFD_D);

	BOOL bVersionOK=FALSE;
	BOOL bCheck = (*m_pSetMembRes)(&rData, TSize_Res, bVersionOK);

}

BOOL CDgn_CSGManager::Check_MemberResKSSC_LSD2014(CSG_MEMB_POSD_KSCE_LSD &MembD, CSG_MEMB_RES_AASHTO_LRFD_D &rData)
{
	size_t TSize_In    = sizeof(CSG_MEMB_POSD_KSCE_LSD);
	size_t TSize_Res   = sizeof(CSG_MEMB_RES_AASHTO_LRFD_D);

	if(m_pMemberRes == NULL) return FALSE;	

	BOOL bVersionOK=FALSE;
	BOOL bCheck = (*m_pMemberRes)(&MembD, &rData, TSize_In, TSize_Res, bVersionOK);

	if(!bCheck || !bVersionOK) return FALSE;
	return TRUE;
}

BOOL CDgn_CSGManager::Check_ConstructibilityKSSC_LSD2014(CSG_MEMB_POSD_KSCE_LSD &MembD, CSG_MEMB_RES_AASHTO_LRFD_D &MembResD, CSG_MEMB_RES_AASHTO_LRFD_CONSTRUCT &rData)
{
	size_t TSize_In    = sizeof(CSG_MEMB_POSD_KSCE_LSD);
	size_t TSize_MembR = sizeof(CSG_MEMB_RES_AASHTO_LRFD_D);
	size_t TSize_Res   = sizeof(CSG_MEMB_RES_AASHTO_LRFD_CONSTRUCT);

	if(m_pConsRes == NULL) return FALSE;	

	BOOL bVersionOK=FALSE;
	BOOL bCheck = (*m_pConsRes)(&MembD, &MembResD, &rData, TSize_In, TSize_MembR, TSize_Res, bVersionOK);

	if(!bCheck || !bVersionOK) return FALSE;
	return TRUE;

}

BOOL CDgn_CSGManager::Check_StrengthResKSSC_LSD2014(CSG_MEMB_POSD_KSCE_LSD &MembD, CSG_MEMB_RES_AASHTO_LRFD_D &MembResD, CSG_MEMB_RES_AASHTO_LRFD_STR_D     &rData)
{
	size_t TSize_In    = sizeof(CSG_MEMB_POSD_KSCE_LSD);
	size_t TSize_MembR = sizeof(CSG_MEMB_RES_AASHTO_LRFD_D);
	size_t TSize_Res   = sizeof(CSG_MEMB_RES_AASHTO_LRFD_STR_D);

	if(m_pStrnRes == NULL) return FALSE;	

	BOOL bVersionOK=FALSE;
	BOOL bCheck = (*m_pStrnRes)(&MembD, &MembResD, &rData, TSize_In, TSize_MembR, TSize_Res, bVersionOK);

	if(!bCheck || !bVersionOK) return FALSE;
	return TRUE;
}

BOOL CDgn_CSGManager::Check_ServiceResKSSC_LSD2014(CSG_MEMB_POSD_KSCE_LSD &MembD, CSG_MEMB_RES_AASHTO_LRFD_D &MembResD, CSG_MEMB_RES_AASHTO_LRFD_SER_D     &rData)
{
	size_t TSize_In    = sizeof(CSG_MEMB_POSD_KSCE_LSD);
	size_t TSize_MembR = sizeof(CSG_MEMB_RES_AASHTO_LRFD_D);
	size_t TSize_Res   = sizeof(CSG_MEMB_RES_AASHTO_LRFD_SER_D);

	if(m_pServRes == NULL) return FALSE;	

	BOOL bVersionOK=FALSE;
	BOOL bCheck = (*m_pServRes)(&MembD, &MembResD, &rData, TSize_In, TSize_MembR, TSize_Res, bVersionOK);

	if(!bCheck || !bVersionOK) return FALSE;
	return TRUE;
}

BOOL CDgn_CSGManager::Check_FatigueResKSSC_LSD2014(CSG_MEMB_POSD_KSCE_LSD &MembD, CSG_MEMB_RES_AASHTO_LRFD_D &MembResD, CSG_MEMB_RES_AASHTO_LRFD_FATIGUE_D &rData)
{
	size_t TSize_In    = sizeof(CSG_MEMB_POSD_KSCE_LSD);
	size_t TSize_MembR = sizeof(CSG_MEMB_RES_AASHTO_LRFD_D);
	size_t TSize_Res   = sizeof(CSG_MEMB_RES_AASHTO_LRFD_FATIGUE_D);

	if(m_pFatiRes == NULL) return FALSE;	

	BOOL bVersionOK=FALSE;
	BOOL bCheck = (*m_pFatiRes)(&MembD, &MembResD, &rData, TSize_In, TSize_MembR, TSize_Res, bVersionOK);

	if(!bCheck || !bVersionOK) return FALSE;
	return TRUE;
}

BOOL CDgn_CSGManager::Check_ShearConnResKSSC_LSD2014(CSG_MEMB_POSD_KSCE_LSD &MembD, CSG_MEMB_RES_AASHTO_LRFD_D &MembResD, CSG_MEMB_RES_AASHTO_LRFD_SHEAR_CONN_D &rData)
{
	size_t TSize_In    = sizeof(CSG_MEMB_POSD_KSCE_LSD);
	size_t TSize_MembR = sizeof(CSG_MEMB_RES_AASHTO_LRFD_D);
	size_t TSize_Res   = sizeof(CSG_MEMB_RES_AASHTO_LRFD_SHEAR_CONN_D);

	if(m_pSconRes == NULL) return FALSE;	

	BOOL bVersionOK=FALSE;
	BOOL bCheck = (*m_pSconRes)(&MembD, &MembResD, &rData, TSize_In, TSize_MembR, TSize_Res, bVersionOK);

	if(!bCheck || !bVersionOK) return FALSE;
	return TRUE;
}

BOOL CDgn_CSGManager::Check_StiffenerResKSSC_LSD2014(CSG_MEMB_POSD_KSCE_LSD &MembD, CSG_MEMB_RES_AASHTO_LRFD_D &MembResD, CSG_MEMB_RES_AASHTO_LRFD_HS_LS_D &rData)
{
	size_t TSize_In    = sizeof(CSG_MEMB_POSD_KSCE_LSD);
	size_t TSize_MembR = sizeof(CSG_MEMB_RES_AASHTO_LRFD_D);
	size_t TSize_Res   = sizeof(CSG_MEMB_RES_AASHTO_LRFD_HS_LS_D);

	if(m_pStfnRes == NULL) return FALSE;	

	BOOL bVersionOK=FALSE;
	BOOL bCheck = (*m_pStfnRes)(&MembD, &MembResD, &rData, TSize_In, TSize_MembR, TSize_Res, bVersionOK);

	if(!bCheck || !bVersionOK) return FALSE;
	return TRUE;
}

void CDgn_CSGManager::Set_MemberResAASHTO(CSG_MEMB_RES_AASHTO_LRFD_D &rData)
{
	if(m_pSetMembRes == NULL) return;

	size_t TSize_Res   = sizeof(CSG_MEMB_RES_AASHTO_LRFD_D);

	BOOL bVersionOK=FALSE;
	BOOL bCheck = (*m_pSetMembRes)(&rData, TSize_Res, bVersionOK);

}

BOOL CDgn_CSGManager::Check_MemberResAASHTO(CSG_MEMB_POSD_KSCE_LSD &MembD, CSG_MEMB_RES_AASHTO_LRFD_D &rData)
{
	size_t TSize_In    = sizeof(CSG_MEMB_POSD_KSCE_LSD);
	size_t TSize_Res   = sizeof(CSG_MEMB_RES_AASHTO_LRFD_D);
	
	if(m_pMemberRes == NULL) return FALSE;	
	
	BOOL bVersionOK=FALSE;
	BOOL bCheck = (*m_pMemberRes)(&MembD, &rData, TSize_In, TSize_Res, bVersionOK);
	
	if(!bCheck || !bVersionOK) return FALSE;
	return TRUE;
}

BOOL CDgn_CSGManager::Check_ConstructibilityAASHTO(CSG_MEMB_POSD_KSCE_LSD &MembD, CSG_MEMB_RES_AASHTO_LRFD_D &MembResD, CSG_MEMB_RES_AASHTO_LRFD_CONSTRUCT &rData)
{
	size_t TSize_In    = sizeof(CSG_MEMB_POSD_KSCE_LSD);
	size_t TSize_MembR = sizeof(CSG_MEMB_RES_AASHTO_LRFD_D);
	size_t TSize_Res   = sizeof(CSG_MEMB_RES_AASHTO_LRFD_CONSTRUCT);
	
	if(m_pConsRes == NULL) return FALSE;	
	
	BOOL bVersionOK=FALSE;
	BOOL bCheck = (*m_pConsRes)(&MembD, &MembResD, &rData, TSize_In, TSize_MembR, TSize_Res, bVersionOK);
	
	if(!bCheck || !bVersionOK) return FALSE;
	return TRUE;
	
}

BOOL CDgn_CSGManager::Check_StrengthResAASHTO(CSG_MEMB_POSD_KSCE_LSD &MembD, CSG_MEMB_RES_AASHTO_LRFD_D &MembResD, CSG_MEMB_RES_AASHTO_LRFD_STR_D     &rData)
{
	size_t TSize_In    = sizeof(CSG_MEMB_POSD_KSCE_LSD);
	size_t TSize_MembR = sizeof(CSG_MEMB_RES_AASHTO_LRFD_D);
	size_t TSize_Res   = sizeof(CSG_MEMB_RES_AASHTO_LRFD_STR_D);
	
	if(m_pStrnRes == NULL) return FALSE;	
	
	BOOL bVersionOK=FALSE;
	BOOL bCheck = (*m_pStrnRes)(&MembD, &MembResD, &rData, TSize_In, TSize_MembR, TSize_Res, bVersionOK);
	
	if(!bCheck || !bVersionOK) return FALSE;
	return TRUE;
}

BOOL CDgn_CSGManager::Check_ServiceResAASHTO(CSG_MEMB_POSD_KSCE_LSD &MembD, CSG_MEMB_RES_AASHTO_LRFD_D &MembResD, CSG_MEMB_RES_AASHTO_LRFD_SER_D     &rData)
{
	size_t TSize_In    = sizeof(CSG_MEMB_POSD_KSCE_LSD);
	size_t TSize_MembR = sizeof(CSG_MEMB_RES_AASHTO_LRFD_D);
	size_t TSize_Res   = sizeof(CSG_MEMB_RES_AASHTO_LRFD_SER_D);
	
	if(m_pServRes == NULL) return FALSE;	
	
	BOOL bVersionOK=FALSE;
	BOOL bCheck = (*m_pServRes)(&MembD, &MembResD, &rData, TSize_In, TSize_MembR, TSize_Res, bVersionOK);
	
	if(!bCheck || !bVersionOK) return FALSE;
	return TRUE;
}

BOOL CDgn_CSGManager::Check_FatigueResAASHTO(CSG_MEMB_POSD_KSCE_LSD &MembD, CSG_MEMB_RES_AASHTO_LRFD_D &MembResD, CSG_MEMB_RES_AASHTO_LRFD_FATIGUE_D &rData)
{
	size_t TSize_In    = sizeof(CSG_MEMB_POSD_KSCE_LSD);
	size_t TSize_MembR = sizeof(CSG_MEMB_RES_AASHTO_LRFD_D);
	size_t TSize_Res   = sizeof(CSG_MEMB_RES_AASHTO_LRFD_FATIGUE_D);
	
	if(m_pFatiRes == NULL) return FALSE;	
	
	BOOL bVersionOK=FALSE;
	BOOL bCheck = (*m_pFatiRes)(&MembD, &MembResD, &rData, TSize_In, TSize_MembR, TSize_Res, bVersionOK);
	
	if(!bCheck || !bVersionOK) return FALSE;
	return TRUE;
}

BOOL CDgn_CSGManager::Check_ShearConnResAASHTO(CSG_MEMB_POSD_KSCE_LSD &MembD, CSG_MEMB_RES_AASHTO_LRFD_D &MembResD, CSG_MEMB_RES_AASHTO_LRFD_SHEAR_CONN_D &rData)
{
	size_t TSize_In    = sizeof(CSG_MEMB_POSD_KSCE_LSD);
	size_t TSize_MembR = sizeof(CSG_MEMB_RES_AASHTO_LRFD_D);
	size_t TSize_Res   = sizeof(CSG_MEMB_RES_AASHTO_LRFD_SHEAR_CONN_D);
	
	if(m_pSconRes == NULL) return FALSE;	
	
	BOOL bVersionOK=FALSE;
	BOOL bCheck = (*m_pSconRes)(&MembD, &MembResD, &rData, TSize_In, TSize_MembR, TSize_Res, bVersionOK);
	
	if(!bCheck || !bVersionOK) return FALSE;
	return TRUE;
}

BOOL CDgn_CSGManager::Check_StiffenerResAASHTO(CSG_MEMB_POSD_KSCE_LSD &MembD, CSG_MEMB_RES_AASHTO_LRFD_D &MembResD, CSG_MEMB_RES_AASHTO_LRFD_HS_LS_D &rData)
{
	size_t TSize_In    = sizeof(CSG_MEMB_POSD_KSCE_LSD);
	size_t TSize_MembR = sizeof(CSG_MEMB_RES_AASHTO_LRFD_D);
	size_t TSize_Res   = sizeof(CSG_MEMB_RES_AASHTO_LRFD_HS_LS_D);
	
	if(m_pStfnRes == NULL) return FALSE;	
	
	BOOL bVersionOK=FALSE;
	BOOL bCheck = (*m_pStfnRes)(&MembD, &MembResD, &rData, TSize_In, TSize_MembR, TSize_Res, bVersionOK);
	
	if(!bCheck || !bVersionOK) return FALSE;
	return TRUE;
}

BOOL CDgn_CSGManager::Check_BearingStiffenerResAASHTO(CSG_MEMB_POSD_KSCE_LSD &MembD, CSG_MEMB_RES_AASHTO_LRFD_D &MembResD, CSG_MEMB_RES_AASHTO_LRFD_BEARING_STIFFENER_D &rData)
{
	size_t TSize_In    = sizeof(CSG_MEMB_POSD_KSCE_LSD);
	size_t TSize_MembR = sizeof(CSG_MEMB_RES_AASHTO_LRFD_D);
	size_t TSize_Res   = sizeof(CSG_MEMB_RES_AASHTO_LRFD_BEARING_STIFFENER_D);

	if(m_pBtfnRes == NULL) return FALSE;	

	BOOL bVersionOK=FALSE;
	BOOL bCheck = (*m_pBtfnRes)(&MembD, &MembResD, &rData, TSize_In, TSize_MembR, TSize_Res, bVersionOK);

	if(!bCheck || !bVersionOK) return FALSE;
	return TRUE;

}

void CDgn_CSGManager::Set_MemberResCSA14( CSG_MEMB_RES_CSA_S6_D &rData )
{
	if(m_pSetMembRes == NULL) return;

	size_t TSize_Res   = sizeof(CSG_MEMB_RES_CSA_S6_D);

	BOOL bVersionOK=FALSE;
	BOOL bCheck = (*m_pSetMembRes)(&rData, TSize_Res, bVersionOK);
}

BOOL CDgn_CSGManager::Check_MemberResCSA14( CSG_MEMB_POSD_KSCE_LSD &MembD, CSG_MEMB_RES_CSA_S6_D &rData )
{
	size_t TSize_In    = sizeof(CSG_MEMB_POSD_KSCE_LSD);
	size_t TSize_Res   = sizeof(CSG_MEMB_RES_CSA_S6_D);

	if(m_pMemberRes == NULL) return FALSE;	

	BOOL bVersionOK=FALSE;
	BOOL bCheck = (*m_pMemberRes)(&MembD, &rData, TSize_In, TSize_Res, bVersionOK);

	if(!bCheck || !bVersionOK) return FALSE;
	return TRUE;
}

void CDgn_CSGManager::Set_MemberResult(CSG_MEMB_RESULT_D &rData)
{
	if(m_pSetMembRes == NULL) return;

	size_t TSize_Res = 0;

	BOOL bVersionOK=FALSE;
	BOOL bCheck = FALSE;

	switch (m_nDgnCode)
	{
		case CSA_S6_19_CSG:
		case CSA_S6_14_CSG:
		case CSA_S6_10_CSG:			
		{
			TSize_Res = sizeof(CSG_MEMB_RES_CSA_S6_D);
			bCheck = (*m_pSetMembRes)(rData.pCSA, TSize_Res, bVersionOK);
			break;
		}
		default: 
			ASSERT(0);
			break;
	}
}

BOOL CDgn_CSGManager::Check_MemberResult(CSG_MEMB_POSD_KSCE_LSD &MembD, CSG_MEMB_RESULT_D &rData)
{
	size_t TSize_In    = sizeof(CSG_MEMB_POSD_KSCE_LSD);
	size_t TSize_Res   = 0;

	if(m_pMemberRes == NULL) return FALSE;	

	int nChkPos = MembD.Memb.ChIf.nChkPos;
	if (nChkPos<0 || nChkPos>1) return FALSE;

	BOOL bVersionOK=FALSE;
	BOOL bCheck = FALSE;

	switch (m_nDgnCode)
	{
	case CSA_S6_19_CSG:
	case CSA_S6_14_CSG:
	case CSA_S6_10_CSG:
		{
			TSize_Res = sizeof(CSG_MEMB_RES_CSA_S6_D);
			bCheck = (*m_pMemberRes)(&MembD, &rData.pCSA->MembR[nChkPos], TSize_In, TSize_Res, bVersionOK);
			break;
		}
	default: 
		ASSERT(0);
		break;
	}

	if(!bCheck || !bVersionOK) return FALSE;
	return TRUE;

}

BOOL CDgn_CSGManager::Check_ConstructibilityCSA(CSG_MEMB_POSD_KSCE_LSD &MembD, CSG_MEMB_RES_CSA_S6_D &MembResD, CSG_CHECK_RES_CSA_S6 &rData)
{
	size_t TSize_In    = sizeof(CSG_MEMB_POSD_KSCE_LSD);
	size_t TSize_MembR = sizeof(CSG_MEMB_RES_CSA_S6_D);
	size_t TSize_Res   = sizeof(CSG_CHECK_RES_CSA_S6);

	if(m_pConsRes == NULL) return FALSE;	

	BOOL bVersionOK=FALSE;
	BOOL bCheck = (*m_pConsRes)(&MembD, &MembResD, &rData, TSize_In, TSize_MembR, TSize_Res, bVersionOK);

	if(!bCheck || !bVersionOK) return FALSE;
	return TRUE;
}

BOOL CDgn_CSGManager::Check_StrengthResCSA(CSG_MEMB_POSD_KSCE_LSD &MembD, CSG_MEMB_RES_CSA_S6_D &MembResD, CSG_CHECK_RES_CSA_S6 &rData)
{
	size_t TSize_In    = sizeof(CSG_MEMB_POSD_KSCE_LSD);
	size_t TSize_MembR = sizeof(CSG_MEMB_RES_CSA_S6_D);
	size_t TSize_Res   = sizeof(CSG_CHECK_RES_CSA_S6);

	if(m_pStrnRes == NULL) return FALSE;	

	BOOL bVersionOK=FALSE;
	BOOL bCheck = (*m_pStrnRes)(&MembD, &MembResD, &rData, TSize_In, TSize_MembR, TSize_Res, bVersionOK); // Check_CsgStrnengthCSA_S6

	if(!bCheck || !bVersionOK) return FALSE;
	return TRUE;
}

BOOL CDgn_CSGManager::Check_ServiceResCSA(CSG_MEMB_POSD_KSCE_LSD &MembD, CSG_MEMB_RES_CSA_S6_D &MembResD, CSG_CHECK_RES_CSA_S6 &rData)
{
	size_t TSize_In    = sizeof(CSG_MEMB_POSD_KSCE_LSD);
	size_t TSize_MembR = sizeof(CSG_MEMB_RES_CSA_S6_D);
	size_t TSize_Res   = sizeof(CSG_CHECK_RES_CSA_S6);

	if(m_pServRes == NULL) return FALSE;	

	BOOL bVersionOK=FALSE;
	BOOL bCheck = (*m_pServRes)(&MembD, &MembResD, &rData, TSize_In, TSize_MembR, TSize_Res, bVersionOK); // Check_CsgServiceCSA_S6

	if(!bCheck || !bVersionOK) return FALSE;
	return TRUE;
}

BOOL CDgn_CSGManager::Check_FatigueResCAS(CSG_MEMB_POSD_KSCE_LSD &MembD, CSG_MEMB_RES_CSA_S6_D &MembResD, CSG_CHECK_RES_CSA_S6 &rData)
{
	size_t TSize_In    = sizeof(CSG_MEMB_POSD_KSCE_LSD);
	size_t TSize_MembR = sizeof(CSG_MEMB_RES_CSA_S6_D);
	size_t TSize_Res   = sizeof(CSG_CHECK_RES_CSA_S6);

	if(m_pFatiRes == NULL) return FALSE;	

	BOOL bVersionOK=FALSE;
	BOOL bCheck = (*m_pFatiRes)(&MembD, &MembResD, &rData, TSize_In, TSize_MembR, TSize_Res, bVersionOK);   // Check_CsgFatigueCSA_S6

	if(!bCheck || !bVersionOK) return FALSE;
	return TRUE;
}

BOOL CDgn_CSGManager::Check_ShearConnResCSA(CSG_MEMB_POSD_KSCE_LSD &MembD, CSG_MEMB_RES_CSA_S6_D &MembResD, CSG_CHECK_RES_CSA_S6 &rData)
{
	size_t TSize_In    = sizeof(CSG_MEMB_POSD_KSCE_LSD);
	size_t TSize_MembR = sizeof(CSG_MEMB_RES_CSA_S6_D);
	size_t TSize_Res   = sizeof(CSG_CHECK_RES_CSA_S6);

	if(m_pSconRes == NULL) return FALSE;	

	BOOL bVersionOK=FALSE;
	BOOL bCheck = (*m_pSconRes)(&MembD, &MembResD, &rData, TSize_In, TSize_MembR, TSize_Res, bVersionOK);   // Check_CsgShearConnectorCSA_S6

	if(!bCheck || !bVersionOK) return FALSE;
	return TRUE;
}

BOOL CDgn_CSGManager::Check_ConstructibilityRes(CSG_MEMB_POSD_KSCE_LSD &MembD, CSG_MEMB_RESULT_D &MembRes, CSG_CHECK_RESULT_CODES &ChkRes)
{
	ASSERT(0);
	return TRUE;
}

BOOL CDgn_CSGManager::Check_StrengthRes(CSG_MEMB_POSD_KSCE_LSD &MembD, CSG_MEMB_RESULT_D &MembRes, CSG_CHECK_RESULT_CODES &ChkRes)
{
	ASSERT(0);
	return TRUE;
}

BOOL CDgn_CSGManager::Check_ServiceRes(CSG_MEMB_POSD_KSCE_LSD &MembD, CSG_MEMB_RESULT_D &MembRes, CSG_CHECK_RESULT_CODES &ChkRes)
{
	ASSERT(0);
	return TRUE;
}

BOOL CDgn_CSGManager::Check_FatigueRes(CSG_MEMB_POSD_KSCE_LSD &MembD, CSG_MEMB_RESULT_D &MembRes, CSG_CHECK_RESULT_CODES &ChkRes)
{
	ASSERT(0);
	return TRUE;
}

BOOL CDgn_CSGManager::Check_ShearConnRes(CSG_MEMB_POSD_KSCE_LSD &MembD, CSG_MEMB_RESULT_D &MembRes, CSG_CHECK_RESULT_CODES &ChkRes)
{
	ASSERT(0);
	return TRUE;
}

BOOL CDgn_CSGManager::Check_StiffenerRes(CSG_MEMB_POSD_KSCE_LSD &MembD, CSG_MEMB_RESULT_D &MembRes, CSG_CHECK_RESULT_CODES &ChkRes)
{
	ASSERT(0);
	return TRUE;
}

BOOL CDgn_CSGManager::Check_BearingStiffenerRes(CSG_MEMB_POSD_KSCE_LSD &MembD, CSG_MEMB_RESULT_D &MembRes, CSG_CHECK_RESULT_CODES &ChkRes)
{
	ASSERT(0);
	return TRUE;
}
