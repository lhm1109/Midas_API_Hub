#pragma once

#include "DgnStlRatingDataCtrl.h"


class CDgnStlRatingDataCtrlNR_GN_CIV_025 : public CDgnStlRatingDataCtrl
{
	// Member Functions.
public:
	CDgnStlRatingDataCtrlNR_GN_CIV_025();
	virtual ~CDgnStlRatingDataCtrlNR_GN_CIV_025();


public:
    virtual BOOL CheckStrengthRCaseAssessment(ElemPairK ElemK, int nPos, int nMovType, CSG_MEMB_POSD_KSCE_LSD& MembLcomD, CSG_MEMB_RESULT_D& MembResD, CSG_CHECK_RES_CS457& ResD,
        T_ASSM_ULS_CSG_BS_BASE& UlsB) override;
    virtual BOOL CheckServiceRCaseAssessment(ElemPairK ElemK, int nPos, int nMovType, CSG_MEMB_POSD_KSCE_LSD& MembLcomD, CSG_MEMB_RESULT_D& MembResD, CSG_CHECK_RES_CS457& ResD,
        T_ASSL_CSG_BS_BASE& AsslB) override;
    virtual BOOL CheckShearConnectorRCaseCS454(ElemPairK ElemK, int nPos, int nMovType, CSG_MEMB_POSD_KSCE_LSD& MembLcomD, CSG_MEMBPOS_RES_CS457_D& MembResD, CSG_CHECK_RES_CS457& ResD,
        T_ASLS_CSG_BS_BASE& AslsB) override;

protected:
    BOOL CheckFlexuralStrengthRCase(ElemPairK ElemK, int nPos, int nMovType, CSG_MEMB_POSD_KSCE_LSD& MembLcomD, CSG_MEMBPOS_RES_NR_GN_CIV_025_D& MembResD, CSG_CHECK_RES_NR_GN_CIV_025& ResD, T_ASSM_ULS_CSG_BS_BASE& UlsB);
    BOOL CheckShearStrengthRCase(ElemPairK ElemK, int nPos, int nMovType, CSG_MEMB_POSD_KSCE_LSD& MembLcomD, CSG_MEMBPOS_RES_NR_GN_CIV_025_D& MembResD, CSG_CHECK_RES_NR_GN_CIV_025& ResD, T_ASSM_ULS_CSG_BS_BASE& UlsB);
    BOOL CheckTransverseStiffenerStrengthRCase(ElemPairK ElemK, int nPos, int nMovType, CSG_MEMB_POSD_KSCE_LSD& MembLcomD, CSG_MEMBPOS_RES_NR_GN_CIV_025_D& MembResD, CSG_CHECK_RES_NR_GN_CIV_025& ResD, T_ASSM_ULS_CSG_BS_BASE& UlsB);
    BOOL CheckFastenerRCase(ElemPairK ElemK, int nPos, int nMovType, CSG_MEMB_POSD_KSCE_LSD& MembLcomD, CSG_MEMBPOS_RES_NR_GN_CIV_025_D& MembResD, CSG_CHECK_RES_NR_GN_CIV_025& ResD, T_ASSM_ULS_CSG_BS_BASE& UlsB);


    int ConvertAssessedCategory(enRailAssessedCategory AssCat);


};