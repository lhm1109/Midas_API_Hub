// Dgn_CSGManager_AASHTO12.h: interface for the CDgn_CSGManager_AASHTO12 class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(__DGN_CsgMANAGER_CSA_S6_14_H__)
#define __DGN_CsgMANAGER_CSA_S6_14_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Dgn_CSGManager.h"

class CDgn_CSGManager_CSA_S6_14 : public CDgn_CSGManager
{
public:
	CDgn_CSGManager_CSA_S6_14();
	virtual ~CDgn_CSGManager_CSA_S6_14();

	virtual BOOL InitialData(int nDgnType, int nDgnCode);

	virtual BOOL Check_MemberResult(CSG_MEMB_POSD_KSCE_LSD &MembD, CSG_MEMB_RESULT_D &rData);

	virtual BOOL Check_ConstructibilityRes(CSG_MEMB_POSD_KSCE_LSD &MembD, CSG_MEMB_RESULT_D &MembRes, CSG_CHECK_RESULT_CODES &ChkRes);
	virtual BOOL Check_StrengthRes        (CSG_MEMB_POSD_KSCE_LSD &MembD, CSG_MEMB_RESULT_D &MembRes, CSG_CHECK_RESULT_CODES &ChkRes);
	virtual BOOL Check_ServiceRes         (CSG_MEMB_POSD_KSCE_LSD &MembD, CSG_MEMB_RESULT_D &MembRes, CSG_CHECK_RESULT_CODES &ChkRes);
	virtual BOOL Check_FatigueRes         (CSG_MEMB_POSD_KSCE_LSD &MembD, CSG_MEMB_RESULT_D &MembRes, CSG_CHECK_RESULT_CODES &ChkRes);
	virtual BOOL Check_ShearConnRes       (CSG_MEMB_POSD_KSCE_LSD &MembD, CSG_MEMB_RESULT_D &MembRes, CSG_CHECK_RESULT_CODES &ChkRes);
	virtual BOOL Check_BearingStiffenerRes(CSG_MEMB_POSD_KSCE_LSD &MembD, CSG_MEMB_RESULT_D &MembRes, CSG_CHECK_RESULT_CODES &ChkRes);

protected:
};


#endif // !defined(__DGN_CsgMANAGER_CSA_S6_14_H__)
