#pragma once

#include "Dgn_CSGManager.h"

class CDgn_CSGManager_NR_GN_CIV_025_06 : public CDgn_CSGManager
{
public:
    CDgn_CSGManager_NR_GN_CIV_025_06();
    virtual ~CDgn_CSGManager_NR_GN_CIV_025_06();

    virtual BOOL InitialData(int nDgnType, int nDgnCode) override;

    virtual BOOL Check_MemberResult(CSG_MEMB_POSD_KSCE_LSD& MembD, CSG_MEMB_RESULT_D& rData) override;

    //virtual BOOL Check_ConstructibilityRes(CSG_MEMB_POSD_KSCE_LSD& MembD, CSG_MEMB_RESULT_D& MembRes, CSG_CHECK_RESULT_CODES& ChkRes) override;
    virtual BOOL Check_StrengthRes(CSG_MEMB_POSD_KSCE_LSD& MembD, CSG_MEMB_RESULT_D& MembRes, CSG_CHECK_RESULT_CODES& ChkRes) override;
    virtual BOOL Check_ServiceRes(CSG_MEMB_POSD_KSCE_LSD& MembD, CSG_MEMB_RESULT_D& MembRes, CSG_CHECK_RESULT_CODES& ChkRes) override;
    //virtual BOOL Check_ShearConnRes(CSG_MEMB_POSD_KSCE_LSD& MembD, CSG_MEMB_RESULT_D& MembRes, CSG_CHECK_RESULT_CODES& ChkRes) override;

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
