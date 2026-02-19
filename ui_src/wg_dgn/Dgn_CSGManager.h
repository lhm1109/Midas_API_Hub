// Dgn_DrawManager.h: interface for the CDgn_DrawManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(__DGN_CsgMANAGER_H__)
#define __DGN_CsgMANAGER_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include "DgnPscCommon.h"

#include "..\dgnengine\src\CVL_DgnCalc_US\DgnCalc_CSG_Struct.h"
#include "..\dgnengine\src\CVL_DgnCalc_KR\\DgnCalc_KR_STL_Struct.h"

//#include "..\dgnengine\src\DgnCalcBase\DgnCalcBase_CSG_Struct.h"
//#include "..\dgnengine\src\CVL_DgnCalc_US\DgnCalc_US_PSC_Struct.h"
//#include "..\dgnengine\src\DGNe_RUS\DgnCalc_RUS_PSC_Struct.h"

//typedef BOOL (PDGNENGINE_FUNC)(void*, void*, size_t, size_t, BOOL&);

#include "Dgn_DllManager_Base.h"

class CDgn_CSGManager : public CDgn_DllManager_Base  
{
public:
	CDgn_CSGManager();
	virtual ~CDgn_CSGManager();

	virtual BOOL InitialData(int nDgnType, int nDgnCode);

public:
	void Set_MemberResKSCE_LSD12(CSG_MEMB_RES_KSCE_LSD12_D &rData);
	BOOL Check_MemberResKSCE_LSD12(CSG_MEMB_POSD_KSCE_LSD &MembD, CSG_MEMB_RES_KSCE_LSD12_D &rData);
	BOOL Check_ConstructibilityKSCE_LSD12(CSG_MEMB_POSD_KSCE_LSD &MembD, CSG_MEMB_RES_KSCE_LSD12_D &MembResD, CSG_CONS_RES_KSCE_LSD12 &rData);
	BOOL Check_StrengthResKSCE_LSD12     (CSG_MEMB_POSD_KSCE_LSD &MembD, CSG_MEMB_RES_KSCE_LSD12_D &MembResD, CSG_STRN_RES_KSCE_LSD12 &rData);
	BOOL Check_ServiceResKSCE_LSD12      (CSG_MEMB_POSD_KSCE_LSD &MembD, CSG_MEMB_RES_KSCE_LSD12_D &MembResD, CSG_SERV_RES_KSCE_LSD12 &rData);
	BOOL Check_FatigueResKSCE_LSD12      (CSG_MEMB_POSD_KSCE_LSD &MembD, CSG_MEMB_RES_KSCE_LSD12_D &MembResD, CSG_FATI_RES_KSCE_LSD12 &rData);

	void Set_MemberResKSSC_LSD2014(CSG_MEMB_RES_AASHTO_LRFD_D &rData);
	BOOL Check_MemberResKSSC_LSD2014(CSG_MEMB_POSD_KSCE_LSD &MembD, CSG_MEMB_RES_AASHTO_LRFD_D &rData);
	BOOL Check_ConstructibilityKSSC_LSD2014(CSG_MEMB_POSD_KSCE_LSD &MembD, CSG_MEMB_RES_AASHTO_LRFD_D &MembResD, CSG_MEMB_RES_AASHTO_LRFD_CONSTRUCT &rData);
	BOOL Check_StrengthResKSSC_LSD2014(CSG_MEMB_POSD_KSCE_LSD &MembD, CSG_MEMB_RES_AASHTO_LRFD_D &MembResD, CSG_MEMB_RES_AASHTO_LRFD_STR_D &rData);
	BOOL Check_ServiceResKSSC_LSD2014(CSG_MEMB_POSD_KSCE_LSD &MembD, CSG_MEMB_RES_AASHTO_LRFD_D &MembResD, CSG_MEMB_RES_AASHTO_LRFD_SER_D &rData);
	BOOL Check_FatigueResKSSC_LSD2014 (CSG_MEMB_POSD_KSCE_LSD &MembD, CSG_MEMB_RES_AASHTO_LRFD_D &MembResD, CSG_MEMB_RES_AASHTO_LRFD_FATIGUE_D &rData);
	BOOL Check_ShearConnResKSSC_LSD2014(CSG_MEMB_POSD_KSCE_LSD &MembD, CSG_MEMB_RES_AASHTO_LRFD_D &MembResD, CSG_MEMB_RES_AASHTO_LRFD_SHEAR_CONN_D &rData);
	BOOL Check_StiffenerResKSSC_LSD2014(CSG_MEMB_POSD_KSCE_LSD &MembD, CSG_MEMB_RES_AASHTO_LRFD_D &MembResD, CSG_MEMB_RES_AASHTO_LRFD_HS_LS_D &rData);
	
	void Set_MemberResAASHTO(CSG_MEMB_RES_AASHTO_LRFD_D &rData);
	BOOL Check_MemberResAASHTO(CSG_MEMB_POSD_KSCE_LSD &MembD, CSG_MEMB_RES_AASHTO_LRFD_D &rData);
	BOOL Check_ConstructibilityAASHTO(CSG_MEMB_POSD_KSCE_LSD &MembD, CSG_MEMB_RES_AASHTO_LRFD_D &MembResD, CSG_MEMB_RES_AASHTO_LRFD_CONSTRUCT &rData);
	BOOL Check_StrengthResAASHTO     (CSG_MEMB_POSD_KSCE_LSD &MembD, CSG_MEMB_RES_AASHTO_LRFD_D &MembResD, CSG_MEMB_RES_AASHTO_LRFD_STR_D &rData);
	BOOL Check_ServiceResAASHTO      (CSG_MEMB_POSD_KSCE_LSD &MembD, CSG_MEMB_RES_AASHTO_LRFD_D &MembResD, CSG_MEMB_RES_AASHTO_LRFD_SER_D &rData);
	BOOL Check_FatigueResAASHTO      (CSG_MEMB_POSD_KSCE_LSD &MembD, CSG_MEMB_RES_AASHTO_LRFD_D &MembResD, CSG_MEMB_RES_AASHTO_LRFD_FATIGUE_D &rData);
	BOOL Check_ShearConnResAASHTO    (CSG_MEMB_POSD_KSCE_LSD &MembD, CSG_MEMB_RES_AASHTO_LRFD_D &MembResD, CSG_MEMB_RES_AASHTO_LRFD_SHEAR_CONN_D &rData);
	BOOL Check_StiffenerResAASHTO    (CSG_MEMB_POSD_KSCE_LSD &MembD, CSG_MEMB_RES_AASHTO_LRFD_D &MembResD, CSG_MEMB_RES_AASHTO_LRFD_HS_LS_D &rData);
	BOOL Check_BearingStiffenerResAASHTO(CSG_MEMB_POSD_KSCE_LSD &MembD, CSG_MEMB_RES_AASHTO_LRFD_D &MembResD, CSG_MEMB_RES_AASHTO_LRFD_BEARING_STIFFENER_D &rData);

	void Set_MemberResCSA14(CSG_MEMB_RES_CSA_S6_D &rData);
	BOOL Check_MemberResCSA14    (CSG_MEMB_POSD_KSCE_LSD &MembD, CSG_MEMB_RES_CSA_S6_D &rData);


	BOOL Check_StrengthResKSCE_ASD   (int nDgnCode, CSG_MEMB_POSD_KSCE_LSD &MembD, STL_KR_RES_D &rData);

	void Set_MemberResult(CSG_MEMB_RESULT_D &rData);
	virtual BOOL Check_MemberResult(CSG_MEMB_POSD_KSCE_LSD &MembD, CSG_MEMB_RESULT_D &rData);
	BOOL Check_ConstructibilityCSA(CSG_MEMB_POSD_KSCE_LSD &MembD, CSG_MEMB_RES_CSA_S6_D &MembResD, CSG_CHECK_RES_CSA_S6 &rData);
	BOOL Check_StrengthResCSA     (CSG_MEMB_POSD_KSCE_LSD &MembD, CSG_MEMB_RES_CSA_S6_D &MembResD, CSG_CHECK_RES_CSA_S6 &rData);
	BOOL Check_ServiceResCSA      (CSG_MEMB_POSD_KSCE_LSD &MembD, CSG_MEMB_RES_CSA_S6_D &MembResD, CSG_CHECK_RES_CSA_S6 &rData);
	BOOL Check_FatigueResCAS      (CSG_MEMB_POSD_KSCE_LSD &MembD, CSG_MEMB_RES_CSA_S6_D &MembResD, CSG_CHECK_RES_CSA_S6 &rData);
	BOOL Check_ShearConnResCSA    (CSG_MEMB_POSD_KSCE_LSD &MembD, CSG_MEMB_RES_CSA_S6_D &MembResD, CSG_CHECK_RES_CSA_S6 &rData);


	virtual BOOL Check_ConstructibilityRes(CSG_MEMB_POSD_KSCE_LSD &MembD, CSG_MEMB_RESULT_D &MembRes, CSG_CHECK_RESULT_CODES &ChkRes);
	virtual BOOL Check_StrengthRes        (CSG_MEMB_POSD_KSCE_LSD &MembD, CSG_MEMB_RESULT_D &MembRes, CSG_CHECK_RESULT_CODES &ChkRes);
	virtual BOOL Check_ServiceRes         (CSG_MEMB_POSD_KSCE_LSD &MembD, CSG_MEMB_RESULT_D &MembRes, CSG_CHECK_RESULT_CODES &ChkRes);
	virtual BOOL Check_FatigueRes         (CSG_MEMB_POSD_KSCE_LSD &MembD, CSG_MEMB_RESULT_D &MembRes, CSG_CHECK_RESULT_CODES &ChkRes);
	virtual BOOL Check_ShearConnRes       (CSG_MEMB_POSD_KSCE_LSD &MembD, CSG_MEMB_RESULT_D &MembRes, CSG_CHECK_RESULT_CODES &ChkRes);
	virtual BOOL Check_StiffenerRes       (CSG_MEMB_POSD_KSCE_LSD &MembD, CSG_MEMB_RESULT_D &MembRes, CSG_CHECK_RESULT_CODES &ChkRes);
	virtual BOOL Check_BearingStiffenerRes(CSG_MEMB_POSD_KSCE_LSD &MembD, CSG_MEMB_RESULT_D &MembRes, CSG_CHECK_RESULT_CODES &ChkRes);

protected:
	
	PDGNENGINE_SETD  *m_pSetMembRes;
	PDGNENGINE_FUNC  *m_pMemberRes;
	PDGNENGINE_FUNC3 *m_pConsRes;
	PDGNENGINE_FUNC3 *m_pStrnRes;
	PDGNENGINE_FUNC3 *m_pServRes;
	PDGNENGINE_FUNC3 *m_pFatiRes;
	PDGNENGINE_FUNC3 *m_pSconRes;
	PDGNENGINE_FUNC3 *m_pStfnRes;
	PDGNENGINE_FUNC3 *m_pBtfnRes;
	PDGNENGINE_FUNC  *m_pCsdAsdRes;
    PDGNENGINE_SETD  *m_pGParam;
	PDGNENGINE_SETD  *m_pSFParam;

	int m_nDgnCode;

};

#endif // !defined(__DGN_CsgMANAGER_H__)
