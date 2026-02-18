// Dgn_CSGManager_AASHTO12.h: interface for the CDgn_CSGManager_AASHTO12 class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(__DGN_CsgManager_CS457_R1_H__)
#define __DGN_CsgManager_CS457_R1_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Dgn_CSGManager.h"

class CDgn_CSGManager_CS457_R1 : public CDgn_CSGManager
{
public:
    CDgn_CSGManager_CS457_R1();
    virtual ~CDgn_CSGManager_CS457_R1();

    virtual BOOL InitialData(int nDgnType, int nDgnCode) override;

    virtual BOOL Check_MemberResult(CSG_MEMB_POSD_KSCE_LSD &MembD, CSG_MEMB_RESULT_D &rData) override;

    virtual BOOL Check_ConstructibilityRes(CSG_MEMB_POSD_KSCE_LSD &MembD, CSG_MEMB_RESULT_D &MembRes, CSG_CHECK_RESULT_CODES &ChkRes) override;
    virtual BOOL Check_StrengthRes        (CSG_MEMB_POSD_KSCE_LSD &MembD, CSG_MEMB_RESULT_D &MembRes, CSG_CHECK_RESULT_CODES &ChkRes) override;
    virtual BOOL Check_ServiceRes         (CSG_MEMB_POSD_KSCE_LSD &MembD, CSG_MEMB_RESULT_D &MembRes, CSG_CHECK_RESULT_CODES &ChkRes) override;
    virtual BOOL Check_ShearConnRes       (CSG_MEMB_POSD_KSCE_LSD &MembD, CSG_MEMB_RESULT_D &MembRes, CSG_CHECK_RESULT_CODES &ChkRes) override;

protected: 
    BOOL CalcEffectiveBreadthRatio();
	BOOL CalcSlendernessFactor();

protected:
    BOOL GetEffBreadthRatio(CSG_EFF_BREADTH_RATIO_SPAN_D& rData);
	BOOL GetSlendernessFactorTable(CSG_v_SLENDERNESS_FACTOR_TABLE& rData);

    CSG_EFF_BREADTH_RATIO_SPAN_D m_EffBreadthR;
    CSG_v_SLENDERNESS_FACTOR_TABLE m_SlendernessFactorT;

private:
    BOOL IsCalc() { return m_bIsCalc; }
    BOOL m_bIsCalc;
};
#endif // !defined(__DGN_CsgManager_CS457_R1_H__)