// Dgn_RatingManager.h: interface for the CDgn_RatingManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(__Dgn_RatingManager_H__)
#define __Dgn_RatingManager_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\dgnengine\src\CVL_DgnCalc_KR\DgnCalc_KR_STL_Struct.h"
#include "..\dgnengine\src\CVL_DgnCalc_KR\DgnCalc_KR_RCSC_BeamStruct.h"

#include "Dgn_DllManager_Base.h"

class CDgn_RatingManager : public CDgn_DllManager_Base  
{
public:
	CDgn_RatingManager();
	virtual ~CDgn_RatingManager();

	BOOL InitialData(int nDgnType, int nDgnCode);

public://UMD_STL_KR_D
	BOOL Check_MemberRes_KSCE_ASD_Rating         (UMD_STL_KR_D &MembD, STL_KR_UMDC_STLOPTION &InStlOpt, STL_KR_RES_D &rData);
	BOOL Check_MemberRes_KSCE_LSD_Rating         (UMD_STL_KR_D &MembD, STL_KR_UMDC_STLOPTION &InStlOpt, S_KR_LSD_RES_D &rData);
	BOOL Check_MemberRes_KSCE_USD_RCRating_Beam(_UMD_RC_BEAM_KCI  &InD, _UMD_RC_KR_BEAM_RES &ResD);
protected:
	
	PDGNENGINE_FUNC4  *m_pMemberRes;
	PDGNENGINE_FUNC  *m_pMemberResRCRatingBeam;
};

#endif // !defined(__Dgn_RatingManager_H__)
