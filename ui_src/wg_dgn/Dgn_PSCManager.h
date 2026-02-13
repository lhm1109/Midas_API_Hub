// Dgn_DrawManager.h: interface for the CDgn_DrawManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(__DGN_PSCMANAGER_H__)
#define __DGN_PSCMANAGER_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DgnPscCommon.h"


//typedef BOOL (PDGNENGINE_FUNC)(void*, void*, size_t, size_t, BOOL&);

#include "Dgn_DllManager_Base.h"

struct _FYSG_PSC_LRFD_BASE;
struct _FYPC_AASHTO;
struct _FYCM_PSC_LRFD_BASE;
struct _FPMT_PSC_LRFD_BASE;
struct _TBAR_PSC_LRFD_BASE;
struct _BDCW_PSC_LRFD_BASE;
struct _STCM_AASHTO_BASE;
struct PSC_LRFD_STRS_CHK_POSI;
struct _TRAN_PSC_BASE;
struct _CRMS_AASHTO_BASE;
struct PSC_RUS_MEMB_RES_D;

struct PSC_FRCR_D;
struct PSC_FYSG_BASE;
struct PSC_FYCM_BASE;
struct PSC_FPSG_BASE;
struct PSC_FPMS_BASE;
struct PSC_FYPC_D;
struct PSC_CUMS_BASE;
struct PSC_CRMT_BASE;
struct PSC_BDCW_BASE;
struct PSC_FATG_BASE;
struct PSC_TNDN_CALD;
struct PSC_STRD_INFO;
struct PSC_FRCR_TNCD;

struct PSC_STRS_CHK_POSI;
struct PSC_BDCW_SNIP_BASE;
struct PSC_CUMS_SNIP_BASE;
struct PSC_FATI_SNIP_BASE;

struct PSC_FRCR_NEUT_AXIS_D;

class CDgn_PSCManager : public CDgn_DllManager_Base  
{
public:
	CDgn_PSCManager();
	virtual ~CDgn_PSCManager();

protected:
	// ※주의 : 아래의 함수는 CVL_DgnCalc_US 전용함수임 
	PDGNENGINE_FUNC* m_pBeforeLoss_Tens;
	PDGNENGINE_FUNC* m_pBeforeLoss_Comp;
	PDGNENGINE_FUNC2* m_pTendonStress;
	PDGNENGINE_FUNC* m_pAfterLoss_Tens;
	PDGNENGINE_FUNC* m_pAfterLoss_Comp;
	PDGNENGINE_FUNC* m_pPrincipalBeforeLoss;
	PDGNENGINE_FUNC* m_pPrincipalAfterLoss;  
	PDGNENGINE_FUNC* m_pFlexureStrength_Pos;
	PDGNENGINE_FUNC* m_pFlexureStrength_Neg;
	PDGNENGINE_FUNC* m_pTensionRebarReq_Top;
	PDGNENGINE_FUNC* m_pTensionRebarReq_Bot;
	PDGNENGINE_FUNC* m_pCrackCheck_Top;
	PDGNENGINE_FUNC* m_pCrackCheck_Bot;
	PDGNENGINE_FUNC* m_pShearDesign;
	PDGNENGINE_FUNC* m_pTorsionDesign;
	//
	PDGNENGINE_SETD* m_pSetMembRes;
	PDGNENGINE_FUNC* m_pMemberRes;
	PDGNENGINE_FUNC* m_pBeforeLoss;
	PDGNENGINE_FUNC* m_pTendonStressCheck;
	PDGNENGINE_FUNC* m_pAfterLoss;  
	PDGNENGINE_FUNC* m_pPrincipalStressBeforeLoss;
	PDGNENGINE_FUNC* m_pPrincipalStressAfterLoss;  
	PDGNENGINE_FUNC* m_pStressCheck;
	PDGNENGINE_FUNC* m_pFlexureStrength;
	PDGNENGINE_FUNC* m_pCrackCheck;
	PDGNENGINE_FUNC* m_pShearCheck;
	PDGNENGINE_FUNC* m_pTorsionCheck;
	PDGNENGINE_FUNC* m_pFatigueCheck;
    PDGNENGINE_FUNC* m_pTransferStress;

public:	
	BOOL InitialData(int iDgnType, int iDgnCode);
	BOOL Calc_AllowbleStressBeforeLoss(UINT nType, PSC_DATA_POSI& PosiD, _FYSG_PSC_LRFD_BASE& rData);
	BOOL Calc_AllowbleStressTendonLoss(int iCivilCode, PSC_TNDN_TDNA& PosiD, _FYPC_AASHTO& rData);
	BOOL Calc_AllowbleStressAfterLoss(UINT nType, PSC_DATA_POSI& PosiD, _FYCM_PSC_LRFD_BASE& rData);
	BOOL Calc_AllowblePrincipalStressBeforeLoss(UINT nType, PSC_DATA_POSI& PosiD, _FPMT_PSC_LRFD_BASE& rData);
	BOOL Calc_AllowblePrincipalStressAfterLoss(UINT nType, PSC_DATA_POSI& PosiD, _FPMT_PSC_LRFD_BASE& rData);
	BOOL Calc_FlexureStrength(UINT nType, PSC_DATA_POSI& PosiD, _BDCY_PSC_LRFD_BASE& rData);
	BOOL Calc_TensionRebarReq(UINT nType, PSC_DATA_POSI& PosiD, _TBAR_PSC_LRFD_BASE& rData);
	BOOL Calc_CrackCheck(UINT nType, PSC_DATA_POSI& PosiD, _BDCW_PSC_LRFD_BASE& rData);  
	BOOL Calc_ShearDesign(UINT nType, PSC_DATA_POSI& PosiD, _CUMS_AASHTO_BASE& rData);
	BOOL Calc_TorsionDesign(UINT nType, PSC_DATA_POSI& PosiD, _STCM_AASHTO_BASE& rData);
	//
	BOOL Calc_PscAllowbleStressBeforeLoss(UINT nType, PSC_DATA_POSI& PosiD, PSC_FYSG_BASE& rData);
	BOOL Calc_PscAllowbleStressAfterLoss(UINT nType, PSC_DATA_POSI& PosiD, PSC_FYCM_BASE& rData);
	BOOL Calc_PscAllowbleStressTendonLoss(PSC_DATA_TNDN& TndnD, PSC_FYPC_D& rData);  
	BOOL Calc_PscAllowblePrincipalStressBeforeLoss(UINT nType, PSC_DATA_POSI& PosiD, PSC_FPSG_BASE& rData);
	BOOL Calc_PscAllowblePrincipalStressAfterLoss(UINT nType, PSC_DATA_POSI& PosiD, PSC_FPMS_BASE& rData);
	BOOL Calc_PscCrackCheck(UINT nType, PSC_DATA_POSI& PosiD, PSC_BDCW_BASE& rData);  
	BOOL Calc_PscFlexureStrength(UINT nType, PSC_DATA_POSI& PosiD, PSC_FRCR_D& rData);  
	BOOL Calc_PscShearDesign(UINT nType, PSC_DATA_POSI& PosiD, PSC_CUMS_BASE& rData);
	BOOL Calc_PscTorsionDesign(UINT nType, PSC_DATA_POSI& PosiD, PSC_CRMT_BASE& rData);
	BOOL Calc_PscFatigeCheck(UINT nType, PSC_DATA_POSI& PosiD, PSC_FATG_BASE& rData);  

	void Set_MemberResDataSNiP(PSC_RUS_MEMB_RES_D &rData);
	BOOL Calc_MemberResData(PSC_DATA_MEMB &MembD, PSC_RUS_MEMB_RES_D &rData);
	BOOL Calc_SecondGroupLSSNiP(PSC_DATA_POSI &PosiD, PSC_STRS_CHK_POSI &rData);
	BOOL Calc_PscCrackCheckSNiP(PSC_DATA_POSI &PosiD, PSC_BDCW_SNIP_BASE &rData);  
	BOOL Calc_PscShearDesignSNiP(PSC_DATA_POSI &PosiD, PSC_CUMS_SNIP_BASE &rData);
	//BOOL Calc_PscTorsionDesign(UINT nType, PSC_DATA_POSI &PosiD, PSC_CRMT_BASE& rData);
	BOOL Calc_PscFatigueDesignSNiP(PSC_DATA_POSI &PosiD, PSC_FATI_SNIP_BASE &rData);

	BOOL Calc_PscStressCheckAS(PSC_DATA_POSI &PosiD, PSC_LRFD_STRS_CHK_POSI &rData);
    BOOL Calc_PscTransferStressAS(PSC_DATA_POSI &PosiD, _TRAN_PSC_BASE& rData);
	BOOL Calc_PscCrackCheckAS(PSC_DATA_POSI &PosiD, _BDCW_PSC_LRFD_BASE &rData);
	BOOL Calc_PscFlexureStrengthAS(PSC_DATA_POSI &PosiD, _BDCY_PSC_LRFD_BASE &rData);
	BOOL Calc_PscShearTorsionStrengthAS(PSC_DATA_POSI &PosiD, _CUMS_AASHTO_BASE &rData);
	BOOL Calc_PscShearTorsionStrengthAS(PSC_DATA_POSI &PosiD, _STCM_AASHTO_BASE &rData);

    BOOL Calc_PscStressCheckBS(PSC_DATA_POSI &PosiD, PSC_STRS_CHK_POSI &rData);
    BOOL Calc_PscCrackCheckBS(PSC_DATA_POSI &PosiD, PSC_BDCW_BASE &rData);
    BOOL Calc_PscFlexureStrengthBS(PSC_DATA_POSI &PosiD, PSC_FRCR_D &rData);
    BOOL Calc_PscShearTorsionStrengthBS(PSC_DATA_POSI &PosiD, PSC_CUMS_BASE &rData);
    BOOL Calc_PscShearTorsionStrengthBS(PSC_DATA_POSI &PosiD, PSC_CRMT_BASE &rData);

    //////////////////////////////////////////////////////////////////////////
	BOOL ConvertFysg_FysgAASHTO(const T_FYSG_D& FysgD, T_FYSG_AASHTO& rData);
	BOOL ConvertFypcKSCE_FypcAASHTO(const T_FYPC_KSCE& FypcD, T_FYPC_AASHTO& rData);
	BOOL ConvertFycm_FycmAASHTO(const T_FYCM_D& FycmD, T_FYCM_AASHTO& rData);
	BOOL ConvertFysgD_FysgPSC(const T_FYSG_D& FysgD, T_FYSG_PSC& rData);
	//BOOL ConvertFypcKSCE_FypcAASHTO(const T_FYPC_KSCE& FypcD, T_FYPC_AASHTO& rData);
	BOOL ConvertFycmD_FycmPSC(const T_FYCM_D& FycmD, T_FYCM_PSC& rData);
	//BOOL ConvertFpsgD_FpsgPSC(const T_FPSG_JTG& InData, T_FPSG_PSC& rData);
	//BOOL ConvertFpmsD_FpmsPSC(const T_FPMS_JTG& InData, T_FPMS_PSC& rData);

	BOOL ConvertFysg_CivilToEngineAASHTO(const T_FYSG_BASE& InData, _FYSG_PSC_LRFD_BASE& rData);
	BOOL ConvertFysg_CivilToEngineAASHTO2(const T_FYSG_AASHTO_BASE& InData, _FYSG_PSC_LRFD_BASE& rData);
	BOOL ConvertFypc_CivilToEngineAASHTO(const T_FYPC_AASHTO& InData,      _FYPC_AASHTO& rData);
	BOOL ConvertFycm_CivilToEngineAASHTO(const T_FYCM_AASHTO_BASE& InData, _FYCM_PSC_LRFD_BASE& rData);
	BOOL ConvertFssg_CivilToEngineAASHTO(const T_FPMT_JTG_BASE& InData,    _FPMT_PSC_LRFD_BASE& rData);
	BOOL ConvertFpms_CivilToEngineAASHTO(const T_FPMS_JTG_BASE& InData,    _FPMT_PSC_LRFD_BASE& rData);
	BOOL ConvertBdcw_CivilToEngineAASHTO(const T_BDCW_AASHTO_BASE& InData, _BDCW_PSC_LRFD_BASE& rData);
	BOOL ConvertTbar_CivilToEngineAASHTO(const T_TBAR_AASHTO_BASE& InData, _TBAR_PSC_LRFD_BASE& rData);
	BOOL ConvertBdcy_CivilToEngineAASHTO(const T_BDCY_AASHTO_BASE& InData, _BDCY_PSC_LRFD_BASE& rData);
	BOOL ConvertCums_CivilToEngineAASHTO(const T_CUMS_AASHTO_BASE& InData, _CUMS_AASHTO_BASE& rData);
	BOOL ConvertCrmt_CivilToEngineAASHTO(const T_CRMT_AASHTO_BASE& InData, _STCM_AASHTO_BASE& rData);
    BOOL ConvertPscDgnBdcr_CivilToEngineAASHTO(const T_PSCD_DGN_B& InD, _BDCY_PSC_LRFD_BASE& rData);
    BOOL ConvertPscForce_CivilToEngine(const T_PSCD_FORCE& ForD, PSC_LCOM_FOR_B& rData);

	BOOL ConvertFysg_EngineToCVL(const _FYSG_PSC_LRFD_BASE& InData, T_FYSG_BASE& rData);
	BOOL ConvertFysg_EngineToCVL2(const _FYSG_PSC_LRFD_BASE& InData, T_FYSG_AASHTO_BASE& rData);
	BOOL ConvertFypc_EngineToCVL(const _FYPC_AASHTO& InData, T_FYPC_AASHTO& rData);
	BOOL ConvertFycm_EngineToCVL(const _FYCM_PSC_LRFD_BASE& InData, T_FYCM_AASHTO_BASE& rData);
	BOOL ConvertFssg_EngineToCVL(const _FPMT_PSC_LRFD_BASE& InData, T_FPMT_JTG_BASE& rData);
	BOOL ConvertFpms_EngineToCVL(const _FPMT_PSC_LRFD_BASE& InData, T_FPMS_JTG_BASE& rData);
	BOOL ConvertBdcw_EngineToCVL(const _BDCW_PSC_LRFD_BASE& InData, T_BDCW_AASHTO_BASE& rData);
	BOOL ConvertTbar_EngineToCVL(const _TBAR_PSC_LRFD_BASE& InData, T_TBAR_AASHTO_BASE& rData);
	BOOL ConvertBdcy_EngineToCVL(const _BDCY_PSC_LRFD_BASE& InData, T_BDCY_AASHTO_BASE& rData);
	BOOL ConvertCums_EngineToCVL(const _CUMS_AASHTO_BASE& InData, T_CUMS_AASHTO_BASE& rData);
	BOOL ConvertCrmt_EngineToCVL(const _STCM_AASHTO_BASE& InData, T_CRMT_AASHTO_BASE& rData);
    BOOL ConvertPscDgnBdcr_EngineToCVL(const PSC_DGNR_BDCR& InD, T_PSCD_DGN_B& rData);
    BOOL ConvertPscDgnTndn_EngineToCVL(const DGN_TNDN_CRC& InD, T_PSCD_TNDN_B& rData);

	BOOL ConvertBdcy_Unit(int nBase_Force, int nBase_Length, _BDCY_PSC_LRFD_BASE& InData);
	BOOL ConvertCums_Unit(int nBase_Force, int nBase_Length, _CUMS_AASHTO_BASE& InData);
	BOOL ConvertCrms_Unit(int nBase_Force, int nBase_Length, _CRMS_AASHTO_BASE& InData);
	BOOL ConvertBdcw_Unit(int nBase_Force, int nBase_Length, _BDCW_PSC_LRFD_BASE& InData);
    BOOL ConvertFysg_Unit(int nBase_Force, int nBase_Length, _FYSG_PSC_LRFD_BASE& InData);
    BOOL ConvertFycm_Unit(int nBase_Force, int nBase_Length, _FYCM_PSC_LRFD_BASE& InData);
	BOOL ConvertPSCIndata_Unit(int nBase_Force, int nBase_Length, PSC_DATA_POSI& InData);
	BOOL ConvertPSCIndata_MatlD_Unit(int nBase_Force, int nBase_Length, PSC_MATL_ELEM& InData);
	BOOL ConvertPSCIndata_SectD_Unit(int nBase_Force, int nBase_Length, PSC_SECT_POSI& InData);
	BOOL ConvertPSCIndata_SectgD_Unit(int nBase_Force, int nBase_Length, DGN_SPSC_SECT& InData);
	BOOL ConvertPSCIndata_RbarD_Unit(int nBase_Force, int nBase_Length, DGN_RBAR_POSI& InData);
	BOOL ConvertPSCIndata_RshrD_Unit(int nBase_Force, int nBase_Length, PSC_RSHR_POSI& InData);
	BOOL ConvertPSCIndata_TndnD_Unit(int nBase_Force, int nBase_Length, PSC_TNDN_POSI& InData);


	BOOL ConvertCurUnit_TndnCalcD(int nBase_Force, int nBase_Length,  PSC_TNDN_CALD& TndnCalcD);
	BOOL ConvertCurUnit_CalcTndnInfo(int nBase_Force, int nBase_Length, CArray<PSC_FRCR_TNCD,PSC_FRCR_TNCD&>& arCalcTndnInfo);
	BOOL ConvertCurUnit_NeutAxisInfo(int nBase_Force, int nBase_Length, CArray<PSC_FRCR_NEUT_AXIS_D,PSC_FRCR_NEUT_AXIS_D&>& arNeutAxisInfo);

	//////////////////////////////////////////////////////////////////////////  
	BOOL ConvertFysg_CivilToEngine(ElemPairK ElemK, const T_FYSG_PSC_BASE& InData, PSC_FYSG_BASE& rData, BOOL bReport=FALSE);
	BOOL ConvertFypc_CivilToEngine(T_TDNA_K TdnaK, const T_FYPC_AASHTO& InData,   PSC_FYPC_D& rData, BOOL bReport=FALSE);
	BOOL ConvertFycm_CivilToEngine(ElemPairK ElemK, const T_FYCM_PSC_BASE& InData, PSC_FYCM_BASE& rData, BOOL bReport=FALSE);
	BOOL ConvertFpsg_CivilToEngine(ElemPairK ElemK, const T_FPSG_PSC_BASE& InData, PSC_FPSG_BASE& rData, BOOL bReport=FALSE);
	BOOL ConvertFpms_CivilToEngine(ElemPairK ElemK, const T_FPMS_PSC_BASE& InData, PSC_FPMS_BASE& rData, BOOL bReport=FALSE);
	BOOL ConvertBdcw_CivilToEngine(ElemPairK ElemK, const T_BDCW_PSC_BASE& InData, PSC_BDCW_BASE& rData, BOOL bReport=FALSE);  
	BOOL ConvertBdcr_CivilToEngine(ElemPairK ElemK, const T_BDCR_PSC_BASE& InData, PSC_FRCR_D& rData, BOOL bReport=FALSE);
	BOOL ConvertCums_CivilToEngine(ElemPairK ElemK, const T_CUMS_PSC_BASE& InData, PSC_CUMS_BASE& rData, BOOL bReport=FALSE);
	BOOL ConvertCrmt_CivilToEngine(ElemPairK ElemK, const T_CRMT_PSC_BASE& InData, PSC_CRMT_BASE& rData, BOOL bReport=FALSE);

	BOOL ConvertFatg_CivilToEngine(ElemPairK ElemK, const T_FATG_PSC_BASE& InData, PSC_FATG_BASE& rData, BOOL bReport=FALSE);

	// PSC Composite
	BOOL ConvertFysgComp_CivilToEngine(ElemPairK ElemK, const T_FYSG_PSC_COMP_BASE& InData, PSC_FYSG_BASE& rData, BOOL bReport=FALSE);
	BOOL ConvertFycmComp_CivilToEngine(ElemPairK ElemK, const T_FYCM_PSC_COMP_BASE& InData, PSC_FYCM_BASE& rData, BOOL bReport=FALSE);
	BOOL ConvertFpsgComp_CivilToEngine(ElemPairK ElemK, const T_FPSG_PSC_COMP_BASE& InData, PSC_FPSG_BASE& rData, BOOL bReport=FALSE);
	BOOL ConvertFpmsComp_CivilToEngine(ElemPairK ElemK, const T_FPMS_PSC_COMP_BASE& InData, PSC_FPMS_BASE& rData, BOOL bReport=FALSE);
	BOOL ConvertBdcwComp_CivilToEngine(ElemPairK ElemK, const T_BDCW_PSC_COMP_BASE& InData, PSC_BDCW_BASE& rData, BOOL bReport=FALSE);
	BOOL ConvertBdcrComp_CivilToEngine(ElemPairK ElemK, const T_BDCR_PSC_COMP_BASE& InData, PSC_FRCR_D& rData, BOOL bReport=FALSE);
	BOOL ConvertCumsComp_CivilToEngine(ElemPairK ElemK, const T_CUMS_PSC_COMP_BASE& InData, PSC_CUMS_BASE& rData, BOOL bReport=FALSE);
	BOOL ConvertCrmtComp_CivilToEngine(ElemPairK ElemK, const T_CRMT_PSC_COMP_BASE& InData, PSC_CRMT_BASE& rData, BOOL bReport=FALSE);

	//IRC112
	BOOL ConvertFysg_CivilToEngine(ElemPairK ElemK, const T_FYSG_PSC_BASE_IRC& InData, PSC_FYSG_BASE& rData, BOOL bReport=FALSE);	//IRC
	//IRC
	BOOL ConvertFycm_CivilToEngine(ElemPairK ElemK, const T_FYCM_PSC_BASE_IRC& InData, PSC_FYCM_BASE& rData, BOOL bReport=FALSE);	//IRC
	BOOL ConvertFpsg_CivilToEngine(ElemPairK ElemK, const T_FPSG_PSC_BASE_IRC& InData, PSC_FPSG_BASE& rData, BOOL bReport=FALSE);	//IRC
	BOOL ConvertFpms_CivilToEngine(ElemPairK ElemK, const T_FPMS_PSC_BASE_IRC& InData, PSC_FPMS_BASE& rData, BOOL bReport=FALSE);	//IRC
	BOOL ConvertBdcw_CivilToEngine(ElemPairK ElemK, const T_BDCW_PSC_BASE_IRC& InData, PSC_BDCW_BASE& rData, BOOL bReport=FALSE);  //IRC
	BOOL ConvertBdcr_CivilToEngine(ElemPairK ElemK, const T_BDCR_PSC_BASE_IRC& InData, PSC_FRCR_D& rData, BOOL bReport=FALSE);		//IRC
	BOOL ConvertCums_CivilToEngine(ElemPairK ElemK, const T_CUMS_PSC_BASE_IRC& InData, PSC_CUMS_BASE& rData, BOOL bReport=FALSE);	//IRC
	BOOL ConvertCrmt_CivilToEngine(ElemPairK ElemK, const T_CRMT_PSC_BASE_IRC& InData, PSC_CRMT_BASE& rData, BOOL bReport=FALSE);	//IRC
	//IRC
	BOOL ConvertFatg_CivilToEngine(ElemPairK ElemK, const T_FATG_PSC_BASE_IRC& InData, PSC_FATG_BASE& rData, BOOL bReport=FALSE);	//IRC

	//End IRC112
	// SNiP
	BOOL ConvertFysg_CivilToEngineSNiP(ElemPairK ElemK, const T_FYSG_SNIP_BASE& InData, PSC_FYSG_BASE& rData, BOOL bReport=FALSE);
	BOOL ConvertFypc_CivilToEngineSNiP(T_TDNA_K TdnaK, const T_FYPC_SNIP& InData,   PSC_FYPC_D& rData, BOOL bReport=FALSE);
	BOOL ConvertFycm_CivilToEngineSNiP(ElemPairK ElemK, const T_FYCM_SNIP_BASE& InData, PSC_FYCM_BASE& rData, BOOL bReport=FALSE);
	BOOL ConvertFpsg_CivilToEngineSNiP(ElemPairK ElemK, const T_FPSG_SNIP_BASE& InData, PSC_FPSG_BASE& rData, BOOL bReport=FALSE);
	BOOL ConvertFpms_CivilToEngineSNiP(ElemPairK ElemK, const T_FPMS_SNIP_BASE& InData, PSC_FPMS_BASE& rData, BOOL bReport=FALSE);
	BOOL ConvertBdcw_CivilToEngineSNiP(ElemPairK ElemK, const T_BDCW_SNIP_BASE &InData, PSC_BDCW_SNIP_BASE &rData, BOOL bReport=FALSE);  
	BOOL ConvertBdcr_CivilToEngineSNiP(ElemPairK ElemK, const T_BDCR_SNIP_BASE &InData, PSC_FRCR_D &rData, BOOL bReport=FALSE);
	BOOL ConvertCums_CivilToEngineSNiP(ElemPairK ElemK, const T_CUMS_SNIP_BASE &InData, PSC_CUMS_SNIP_BASE &rData, BOOL bReport=FALSE);
	BOOL ConvertCrmt_CivilToEngineSNiP(ElemPairK ElemK, const T_CRMT_SNIP_BASE& InData, PSC_CUMS_SNIP_BASE &rData, BOOL bReport=FALSE);
	BOOL ConvertFati_CivilToEngineSNiP(ElemPairK ElemK, const T_FATI_SNIP_BASE &InData, PSC_FATI_SNIP_BASE &rData, BOOL bReport=FALSE);

	// AS 5100:5
	BOOL ConvertFysg_CivilToEngineAS(ElemPairK ElemK, const T_FYSG_PSC_AS_BASE& InData, _FYSG_PSC_LRFD_BASE& rData);
    BOOL ConvertTran_CivilToEngineAS(ElemPairK ElemK, const T_TRAN_PSC_AS_BASE& InData, _TRAN_PSC_BASE&      rData);
	BOOL ConvertFycm_CivilToEngineAS(ElemPairK ElemK, const T_FYCM_PSC_AS_BASE& InData, _FYCM_PSC_LRFD_BASE& rData);
	BOOL ConvertFpsg_CivilToEngineAS(ElemPairK ElemK, const T_FPMT_JTG_BASE&    InData, _FPMT_PSC_LRFD_BASE& rData);
	BOOL ConvertFpms_CivilToEngineAS(ElemPairK ElemK, const T_FPMS_PSC_BASE&    InData, _FPMT_PSC_LRFD_BASE& rData);
	BOOL ConvertFypc_CivilToEngineAS(T_TDNA_K TdnaK, const T_FYPC_PSC_AS&      InData,  _FYPC_AASHTO&       rData);
	BOOL ConvertBdcw_CivilToEngineAS(ElemPairK ElemK, const T_BDCW_PSC_AS_BASE& InData, _BDCW_PSC_LRFD_BASE& rData);
	BOOL ConvertBdcr_CivilToEngineAS(ElemPairK ElemK, const T_BDCR_PSC_AS_BASE& InData, _BDCY_PSC_LRFD_BASE& rData);
	BOOL ConvertCums_CivilToEngineAS(ElemPairK ElemK, const T_SHRR_PSC_AS_BASE& InData, _CUMS_AASHTO_BASE&   rData);
	BOOL ConvertCrmt_CivilToEngineAS(ElemPairK ElemK, const T_TORR_PSC_AS_BASE& InData, _STCM_AASHTO_BASE&   rData);

    // BS5400:4, BD44/15
    BOOL ConvertFysg_CivilToEngineBS(ElemPairK ElemK, const T_FYSG_PSC_BS_BASE& InData, PSC_FYSG_BASE& rData);
    BOOL ConvertFycm_CivilToEngineBS(ElemPairK ElemK, const T_FYCM_PSC_BS_BASE& InData, PSC_FYCM_BASE& rData);
    BOOL ConvertFpsg_CivilToEngineBS(ElemPairK ElemK, const T_FPSG_PSC_BASE&    InData, PSC_FPSG_BASE& rData);
    BOOL ConvertFpms_CivilToEngineBS(ElemPairK ElemK, const T_FPMS_PSC_BASE&    InData, PSC_FPMS_BASE& rData);
    BOOL ConvertBdcw_CivilToEngineBS(ElemPairK ElemK, const T_BDCW_PSC_BS_BASE& InData, PSC_BDCW_BASE& rData);
    BOOL ConvertBdcr_CivilToEngineBS(ElemPairK ElemK, const T_BDCR_PSC_BS_BASE& InData, PSC_FRCR_D& rData);
    BOOL ConvertCums_CivilToEngineBS(ElemPairK ElemK, const T_SHRR_PSC_BS_BASE& InData, PSC_CUMS_BASE& rData);
    BOOL ConvertCrmt_CivilToEngineBS(ElemPairK ElemK, const T_TORR_PSC_BS_BASE& InData, PSC_CRMT_BASE& rData);
    BOOL ConvertFypc_CivilToEngineBS(T_TDNA_K TdnaK, const T_FYPC_PSC_BS& InData, PSC_FYPC_D& rData);

	//
	//IRS
	BOOL ConvertFysg_CivilToEngine(ElemPairK ElemK, const T_FYSG_PSC_BASE_IRS& InData, PSC_FYSG_BASE& rData, BOOL bReport=FALSE);		
	BOOL ConvertFycm_CivilToEngine(ElemPairK ElemK, const T_FYCM_PSC_BASE_IRS& InData, PSC_FYCM_BASE& rData, BOOL bReport=FALSE);	
	BOOL ConvertFpsg_CivilToEngine(ElemPairK ElemK, const T_FPSG_PSC_BASE_IRS& InData, PSC_FPSG_BASE& rData, BOOL bReport=FALSE);	
	BOOL ConvertFpms_CivilToEngine(ElemPairK ElemK, const T_FPMS_PSC_BASE_IRS& InData, PSC_FPMS_BASE& rData, BOOL bReport=FALSE);	
	BOOL ConvertBdcw_CivilToEngine(ElemPairK ElemK, const T_BDCW_PSC_BASE_IRS& InData, PSC_BDCW_BASE& rData, BOOL bReport=FALSE);  
	BOOL ConvertBdcr_CivilToEngine(ElemPairK ElemK, const T_BDCR_PSC_BASE_IRS& InData, PSC_FRCR_D& rData, BOOL bReport=FALSE);		
	BOOL ConvertCums_CivilToEngine(ElemPairK ElemK, const T_CUMS_PSC_BASE_IRS& InData, PSC_CUMS_BASE& rData, BOOL bReport=FALSE);	
	BOOL ConvertCrmt_CivilToEngine(ElemPairK ElemK, const T_CRMT_PSC_BASE_IRS& InData, PSC_CRMT_BASE& rData, BOOL bReport=FALSE);		
	BOOL ConvertFatg_CivilToEngine(ElemPairK ElemK, const T_FATG_PSC_BASE_IRS& InData, PSC_FATG_BASE& rData, BOOL bReport=FALSE);	
	BOOL ConvertFypc_CivilToEngine(T_TDNA_K TdnaK, const T_FYPC_IRS& InData,   PSC_FYPC_D& rData, BOOL bReport=FALSE);

	//End IRS
	BOOL ConvertFysg_EngineToCivil(const PSC_FYSG_BASE& InData, T_FYSG_PSC_BASE& rData);
	BOOL ConvertFypc_EngineToCivil(const PSC_FYPC_D&    InData, T_FYPC_AASHTO&   rData);
	BOOL ConvertFycm_EngineToCivil(const PSC_FYCM_BASE& InData, T_FYCM_PSC_BASE& rData);
	BOOL ConvertFpsg_EngineToCivil(const PSC_FPSG_BASE& InData, T_FPSG_PSC_BASE& rData);
	BOOL ConvertFpms_EngineToCivil(const PSC_FPMS_BASE& InData, T_FPMS_PSC_BASE& rData);
	BOOL ConvertBdcw_EngineToCivil      (const PSC_BDCW_BASE& InData, BOOL bOk_Cr,    T_BDCW_PSC_BASE& rData);  
	BOOL ConvertBdcw_EngineToCivil_zero (const PSC_BDCW_BASE& InData, BOOL bOK_Zero,  T_BDCW_PSC_BASE& rData);  
	BOOL ConvertBdcw_EngineToCivil_Asmin(const PSC_BDCW_BASE& InData, BOOL bOK_Asmin, T_BDCW_PSC_BASE& rData); 

	BOOL ConvertBdcr_EngineToCivil(const PSC_FRCR_D&    InData, T_BDCR_PSC_BASE& rData);
	BOOL ConvertCums_EngineToCivil(const PSC_CUMS_BASE& InData, T_CUMS_PSC_BASE& rData);
	BOOL ConvertCrmt_EngineToCivil(const PSC_CRMT_BASE& InData, T_CRMT_PSC_BASE& rData);

	BOOL ConvertFatg_EngineToCivil(const PSC_FATG_BASE& InData, T_FATG_PSC_BASE& rData);

	// PSC Composite
	BOOL ConvertFysgComp_EngineToCivil(const PSC_FYSG_BASE& InData, T_FYSG_PSC_COMP_BASE& rData);
	BOOL ConvertFycmComp_EngineToCivil(const PSC_FYCM_BASE& InData, T_FYCM_PSC_COMP_BASE& rData);
	BOOL ConvertFpsgComp_EngineToCivil(const PSC_FPSG_BASE& InData, T_FPSG_PSC_COMP_BASE& rData);
	BOOL ConvertFpmsComp_EngineToCivil(const PSC_FPMS_BASE& InData, T_FPMS_PSC_COMP_BASE& rData);
	BOOL ConvertBdcwComp_EngineToCivil     (const PSC_BDCW_BASE& InData, BOOL bOk_Cr,   T_BDCW_PSC_COMP_BASE& rData);  
	BOOL ConvertBdcwComp_EngineToCivil_zero(const PSC_BDCW_BASE& InData, BOOL bOK_Zero, T_BDCW_PSC_COMP_BASE& rData);
	BOOL ConvertBdcwComp_EngineToCivil_Asmin(const PSC_BDCW_BASE& InData, BOOL bOK_Asmin, T_BDCW_PSC_COMP_BASE& rData); 

	BOOL ConvertBdcrComp_EngineToCivil(const PSC_FRCR_D&    InData, T_BDCR_PSC_COMP_BASE& rData);  
	BOOL ConvertCumsComp_EngineToCivil(const PSC_CUMS_BASE& InData, T_CUMS_PSC_COMP_BASE& rData);
	BOOL ConvertCrmtComp_EngineToCivil(const PSC_CRMT_BASE& InData, T_CRMT_PSC_COMP_BASE& rData);

	//IRC112
	BOOL ConvertFysg_EngineToCivil(const PSC_FYSG_BASE& InData, T_FYSG_PSC_BASE_IRC& rData); //IRC
	//IRC
	BOOL ConvertFycm_EngineToCivil(const PSC_FYCM_BASE& InData, T_FYCM_PSC_BASE_IRC& rData); //IRC
	BOOL ConvertFpsg_EngineToCivil(const PSC_FPSG_BASE& InData, T_FPSG_PSC_BASE_IRC& rData); //IRC
	BOOL ConvertFpms_EngineToCivil(const PSC_FPMS_BASE& InData, T_FPMS_PSC_BASE_IRC& rData); //IRC
	BOOL ConvertBdcw_EngineToCivil     (const PSC_BDCW_BASE& InData, BOOL bOk_Cr,  T_BDCW_PSC_BASE_IRC& rData); //IRC 
	BOOL ConvertBdcw_EngineToCivil_zero(const PSC_BDCW_BASE& InData, BOOL bOK_Zero,T_BDCW_PSC_BASE_IRC& rData); //IRC 

	BOOL ConvertBdcr_EngineToCivil(const PSC_FRCR_D&    InData, T_BDCR_PSC_BASE_IRC& rData); //IRC
	BOOL ConvertCums_EngineToCivil(const PSC_CUMS_BASE& InData, T_CUMS_PSC_BASE_IRC& rData); //IRC
	BOOL ConvertCrmt_EngineToCivil(const PSC_CRMT_BASE& InData, T_CRMT_PSC_BASE_IRC& rData); //IRC
	//IRC
	BOOL ConvertFatg_EngineToCivil(const PSC_FATG_BASE& InData, T_FATG_PSC_BASE_IRC& rData); //IRC
	//End IRC112
	// SNiP
	BOOL ConvertFysg_EngineToCivilSNiP(const PSC_FYSG_BASE      &InData, T_FYSG_SNIP_BASE  &rData);
	BOOL ConvertFypc_EngineToCivilSNiP(const PSC_FYPC_D         &InData, T_FYPC_SNIP      &rData);
	BOOL ConvertFycm_EngineToCivilSNiP(const PSC_FYCM_BASE      &InData, T_FYCM_SNIP_BASE  &rData);
	BOOL ConvertFpsg_EngineToCivilSNiP(const PSC_FPSG_BASE      &InData, T_FPSG_SNIP_BASE  &rData);
	BOOL ConvertFpms_EngineToCivilSNiP(const PSC_FPMS_BASE      &InData, T_FPMS_SNIP_BASE  &rData);
	BOOL ConvertBdcw_EngineToCivilSNiP(const PSC_BDCW_SNIP_BASE &InData, T_BDCW_SNIP_BASE &rData);  
	BOOL ConvertBdcr_EngineToCivilSNiP(const PSC_FRCR_D         &InData, T_BDCR_SNIP_BASE &rData);
	BOOL ConvertCums_EngineToCivilSNiP(const PSC_CUMS_SNIP_BASE &InData, T_CUMS_SNIP_BASE &rData);
	BOOL ConvertCrmt_EngineToCivilSNiP(const PSC_CUMS_SNIP_BASE &InData, T_CRMT_SNIP_BASE &rData);
	BOOL ConvertFati_EngineToCivilSNiP(const PSC_FATI_SNIP_BASE &InData, T_FATI_SNIP_BASE &rData);

	// AS 5100:5
	BOOL ConvertFysg_EngineToCivilAS(const _FYSG_PSC_LRFD_BASE &InData, T_FYSG_PSC_AS_BASE &rData);
    BOOL ConvertTran_EngineToCivilAS(const _TRAN_PSC_BASE      &InData, T_TRAN_PSC_AS_BASE &rData);
	BOOL ConvertFycm_EngineToCivilAS(const _FYCM_PSC_LRFD_BASE &InData, T_FYCM_PSC_AS_BASE &rData);
	BOOL ConvertFpsg_EngineToCivilAS(const _FPMT_PSC_LRFD_BASE &InData, T_FPMT_JTG_BASE    &rData);
	BOOL ConvertFpms_EngineToCivilAS(const _FPMT_PSC_LRFD_BASE &InData, T_FPMS_PSC_BASE    &rData);
	BOOL ConvertFypc_EngineToCivilAS(const _FYPC_AASHTO        &InData, T_FYPC_PSC_AS    &rData);
	BOOL ConvertBdcw_EngineToCivilAS(const _BDCW_PSC_LRFD_BASE &InData, T_BDCW_PSC_AS_BASE &rData);
	BOOL ConvertBdcr_EngineToCivilAS(const _BDCY_PSC_LRFD_BASE &InData, T_BDCR_PSC_AS_BASE &rData);
	BOOL ConvertCums_EngineToCivilAS(const _CUMS_AASHTO_BASE   &InData, T_SHRR_PSC_AS_BASE &rData);
	BOOL ConvertCrmt_EngineToCivilAS(const _STCM_AASHTO_BASE   &InData, T_TORR_PSC_AS_BASE &rData);

    // BS5400:4, BD44/15
    BOOL ConvertFysg_EngineToCivilBS(const PSC_FYSG_BASE &InData, T_FYSG_PSC_BS_BASE &rData);
    BOOL ConvertFycm_EngineToCivilBS(const PSC_FYCM_BASE &InData, T_FYCM_PSC_BS_BASE &rData);
    BOOL ConvertFpsg_EngineToCivilBS(const PSC_FPSG_BASE &InData, T_FPSG_PSC_BASE    &rData);
    BOOL ConvertFpms_EngineToCivilBS(const PSC_FPMS_BASE &InData, T_FPMS_PSC_BASE    &rData);
    BOOL ConvertBdcw_EngineToCivilBS(const PSC_BDCW_BASE &InData, T_BDCW_PSC_BS_BASE &rData);
    BOOL ConvertBdcr_EngineToCivilBS(const PSC_FRCR_D    &InData, T_BDCR_PSC_BS_BASE &rData);
    BOOL ConvertCums_EngineToCivilBS(const PSC_CUMS_BASE &InData, T_SHRR_PSC_BS_BASE &rData);
    BOOL ConvertCrmt_EngineToCivilBS(const PSC_CRMT_BASE &InData, T_TORR_PSC_BS_BASE &rData);
    BOOL ConvertFypc_EngineToCivilBS(const PSC_FYPC_D    &InData, T_FYPC_PSC_BS      &rData);

	//IRS
	BOOL ConvertFysg_EngineToCivil(const PSC_FYSG_BASE& InData, T_FYSG_PSC_BASE_IRS& rData);	
	BOOL ConvertFycm_EngineToCivil(const PSC_FYCM_BASE& InData, T_FYCM_PSC_BASE_IRS& rData);
	BOOL ConvertFpsg_EngineToCivil(const PSC_FPSG_BASE& InData, T_FPSG_PSC_BASE_IRS& rData);
	BOOL ConvertFpms_EngineToCivil(const PSC_FPMS_BASE& InData, T_FPMS_PSC_BASE_IRS& rData);
	BOOL ConvertBdcw_EngineToCivil     (const PSC_BDCW_BASE& InData, BOOL bOk_Cr,  T_BDCW_PSC_BASE_IRS& rData); 
	BOOL ConvertBdcw_EngineToCivil_zero(const PSC_BDCW_BASE& InData, BOOL bOK_Zero,T_BDCW_PSC_BASE_IRS& rData); 
	BOOL ConvertBdcr_EngineToCivil(const PSC_FRCR_D&    InData, T_BDCR_PSC_BASE_IRS& rData); 
	BOOL ConvertCums_EngineToCivil(const PSC_CUMS_BASE& InData, T_CUMS_PSC_BASE_IRS& rData); 
	BOOL ConvertCrmt_EngineToCivil(const PSC_CRMT_BASE& InData, T_CRMT_PSC_BASE_IRS& rData); 
	BOOL ConvertFatg_EngineToCivil(const PSC_FATG_BASE& InData, T_FATG_PSC_BASE_IRS& rData); 
	//End IRS
	//////////////////////////////////////////////////////////////////////////  
	BOOL ConvertFycm_ToEngineAASHTO(const T_FYCM_D& InData, T_FYCM_AASHTO& rData);

	BOOL ConvertFycm_ToCivilData(const T_FYCM_AASHTO& InData, T_FYCM_D& rData);

	CString Get_SatgeName4PSC(int StagK);
	CStringW Get_LcomName4PSC(int iDgnLcomK,  BOOL bOriginalLcomKey = FALSE);
	BOOL Get_TndnInfo4PSC(T_TDNA_K TdnaK, CString& strTendonName, int& nLoadType);
};

#endif // !defined(__DGN_PSCMANAGER_H__)
