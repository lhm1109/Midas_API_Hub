// Dgn_RatingManager.cpp: implementation of the Dgn_RatingManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Dgn_RatingManager.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDgn_RatingManager::CDgn_RatingManager() : CDgn_DllManager_Base()
{
	m_pMemberRes  = NULL;
	m_pMemberResRCRatingBeam = NULL;
}

CDgn_RatingManager::~CDgn_RatingManager()
{

}

BOOL CDgn_RatingManager::InitialData(int nDgnType, int nDgnCode)
{
	HINSTANCE hDllCalc = GetOrLoad_CalcDllHandle(nDgnType, nDgnCode);
	if(hDllCalc == NULL) return FALSE;

	switch (nDgnCode)
	{
		case STL_KSCE_ASD05:
		case STL_KSCE_ASD10:
		case STL_KSCE_RAIL_ASD04:
		case STL_KSCE_RAIL_ASD11:
		case STL_KSCE_LSD15:
		{
			m_pMemberRes = (PDGNENGINE_FUNC4*)GetProcAddress(hDllCalc, "UMD_STL_Execute_Chk");
			if (!m_pMemberRes) return FALSE;

			break;
		}
		case STL_AASHTO_LRFD12:
        case STL_AASHTO_LRFD19:
        case STL_CS454_20:
		case STL_NR_GN_CIV_025_06:
			break;
		case KSCE_USD10_RC_RATING:
		case KSCE_RAIL_USD11_RC_RATING:
		case KSCE_LSD15_RC_RATING:
			{
				m_pMemberResRCRatingBeam = (PDGNENGINE_FUNC*)GetProcAddress(hDllCalc, "UMD_RC_Beam_Execute_Chk");
				if (!m_pMemberResRCRatingBeam) return FALSE;
				break;
			}
		default: ASSERT(0); break;
	}

	return TRUE;
}

BOOL CDgn_RatingManager::Check_MemberRes_KSCE_ASD_Rating(UMD_STL_KR_D &MembD, STL_KR_UMDC_STLOPTION &InStlOpt, STL_KR_RES_D &rData)
{
	size_t TSize_In    = sizeof(UMD_STL_KR_D);
	size_t TSize_Res   = sizeof(STL_KR_RES_D);
	
	if(m_pMemberRes == NULL) return FALSE;	

	//[Check before Release...!!] BSC-20140313 함수돌리기 이해 채워야할 항목
	size_t TSize_InOpt = sizeof(STL_KR_UMDC_STLOPTION);
	CStringW strFilePath;
	//
	
	BOOL bVersionOK=FALSE;
	BOOL bCheck = (*m_pMemberRes)(&MembD, &InStlOpt, &rData, strFilePath, TSize_In, TSize_InOpt, TSize_Res, bVersionOK);
	//BOOL bCheck = (*m_pMemberRes)(&MembD, &rData, TSize_In, TSize_Res, bVersionOK);

	if(!bCheck || !bVersionOK) return FALSE;
	return TRUE;
}

BOOL CDgn_RatingManager::Check_MemberRes_KSCE_LSD_Rating(UMD_STL_KR_D &MembD, STL_KR_UMDC_STLOPTION &InStlOpt, S_KR_LSD_RES_D &rData)
{
	size_t TSize_In    = sizeof(UMD_STL_KR_D);
	size_t TSize_Res   = sizeof(S_KR_LSD_RES_D);

	if(m_pMemberRes == NULL) return FALSE;	

	size_t TSize_InOpt = sizeof(STL_KR_UMDC_STLOPTION);
	CStringW strFilePath;
	//

	BOOL bVersionOK=FALSE;
	BOOL bCheck = (*m_pMemberRes)(&MembD, &InStlOpt, &rData, strFilePath, TSize_In, TSize_InOpt, TSize_Res, bVersionOK);
	
	if(!bCheck || !bVersionOK) return FALSE;
	return TRUE;
}

	BOOL CDgn_RatingManager::Check_MemberRes_KSCE_USD_RCRating_Beam(_UMD_RC_BEAM_KCI  &InD, _UMD_RC_KR_BEAM_RES &ResD)
	{
		size_t TSize_In    = sizeof(_UMD_RC_BEAM_KCI);
		size_t TSize_Res   = sizeof(_UMD_RC_KR_BEAM_RES);

		if(m_pMemberResRCRatingBeam == NULL) return FALSE;	

		BOOL bVersionOK=FALSE;
		BOOL bCheck = (*m_pMemberResRCRatingBeam)(&InD, &ResD, TSize_In, TSize_Res, bVersionOK);

		if(!bCheck || !bVersionOK) return FALSE;
		return TRUE;
	}
