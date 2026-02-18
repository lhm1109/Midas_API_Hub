// Dgn_DrawManager.h: interface for the CDgn_DrawManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(__Dgn_OrthotropicDeckManager_H__)
#define __Dgn_OrthotropicDeckManager_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include "DgnPscCommon.h"

#include "..\dgnengine\src\DGNe_RUS\DgnCalc_RUS_STL_OD_Struct.h"

//#include "..\dgnengine\src\DgnCalcBase\DgnCalcBase_CSG_Struct.h"
//#include "..\dgnengine\src\CVL_DgnCalc_US\DgnCalc_US_PSC_Struct.h"
//#include "..\dgnengine\src\DGNe_RUS\DgnCalc_RUS_PSC_Struct.h"

//typedef BOOL (PDGNENGINE_FUNC)(void*, void*, size_t, size_t, BOOL&);

#include "Dgn_DllManager_Base.h"

class CDgn_OrthotropicDeckManager : public CDgn_DllManager_Base  
{
public:
	CDgn_OrthotropicDeckManager();
	virtual ~CDgn_OrthotropicDeckManager();
	
	BOOL InitialData(int nDgnType, int nDgnCode);

public:
	void Set_MemberResStlOrthDeckSNiP(STL_RES_BASE_SNiP_OD_SNiP &rData);
	BOOL Check_MemberResSNiP(STL_MEMB_POSD_SNiP_OD &MembD, STL_RES_BASE_SNiP_OD_SNiP &rData);  
	BOOL Check_StlOrthDeckSNiP(STL_MEMB_POSD_SNiP_OD &MembD, STL_RES_BASE_SNiP_OD_SNiP &MembResD, STL_MEMB_RES_STR_STA_SNiP_OD &rData);
	BOOL Check_KappaF_StlOrthDeckSNiP(STL_MEMB_POSD_SNiP_OD &MembD, STL_RES_BASE_SNiP_OD_SNiP &MembResD, BOOL &bAllowKappaF, double &dRat_Max, aKAPAF_PonitInfo &aKappaFCheckRes);
	BOOL Check_StlOrthDeckFatigueSNiP(STL_MEMB_POSD_SNiP_OD &MembD, STL_RES_BASE_SNiP_OD_SNiP &MembResD, STL_MEMB_RES_SNiP_FAT &rData);
	BOOL GetTableValueSNiP(STL_SNiP_TB_D &rData);

protected:
	PDGNENGINE_FUNC  *m_pMemberRes;
	PDGNENGINE_FUNC3 *m_pStrnStabRes;
	PDGNENGINE_FUNC5 *m_pKapaaCheck;
	PDGNENGINE_FUNC3 *m_pFatiRes;
	PDGNENGINE_SETD  *m_pGetTBval;


};

#endif // !defined(__Dgn_OrthotropicDeckManager_H__)
