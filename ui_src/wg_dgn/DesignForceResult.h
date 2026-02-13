#if !defined(AFX_DESIGNFORCERESULT_H__)
#define AFX_DESIGNFORCERESULT_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CRCDataCtrl.h"
#include "CRCForceCtrl.h"

// forward declarations ------------
class CPostCtrl;
class CDgnResultFileCtrl;
//class CDgnResultDataFilePosition;
class CResultDataFilePosition;

#include "HeaderPre.h"

class __MY_EXT_CLASS__ CDesignForceResult  
{
public:
	CDesignForceResult();
	CDesignForceResult(CCRCDataCtrl* pDataCtrl);
	virtual ~CDesignForceResult();

protected:  
	CResultDataFilePosition* m_pRdfp;
	CDgnResultFileCtrl* m_pResultFileCtrl;
	//CSeqIdInfo* m_pSeqIdInfo;
	
	CPostCtrl* m_pPostCtrl;
	
	CCRCDataCtrl* m_pDataCtrl;
	CCRCForceCtrl* m_pForceCtrl;

public:
	//
	void Initialize(CCRCDataCtrl* pDataCtrl);
	//
	BOOL GetForceLcomResult(ElemPairK ElemK, int iSerialLcom, _DGN_FORC_CRC& Force);
	BOOL GetForceLcomResult_Part1(ElemPairK ElemK, int iSerialLcom, _DGN_FORC_CRC& Force);
	BOOL GetForceLcomResult_Part2(ElemPairK ElemK, int iSerialLcom, _DGN_FORC_CRC& Force);
	BOOL GetForceLcomExTpResult(ElemPairK ElemK, int iSerialLcom, _DGN_FORC_CRC& Force);
	BOOL GetForceStageResult(ElemPairK ElemK, int iSerialLcom, _DGN_FORC_CRC& Force);
	BOOL GetShrsStageResult(ElemPairK ElemK, int iSerialLcom, _DGN_SHRS_ADD& ShrsAdd);
	BOOL GetForceTnprResult(ElemPairK ElemK, int iSerialLcom, _DGN_FORC_CRC& Force);
	BOOL GetForceTnscResult(ElemPairK ElemK, int iSerialLcom, _DGN_FORC_CRC& Force);
	BOOL GetStressAddResult(ElemPairK ElemK, int iSerialLcom, _DGN_STRS_ADD& StrsAdd);
	BOOL GetForceLTermResult(ElemPairK ElemK, int iSerialLcom, _DGN_FORC_CRC& Force);
	BOOL GetForceSTermResult(ElemPairK ElemK, int iSerialLcom, _DGN_FORC_CRC& Force);
	BOOL GetForceJtgTndnResult(ElemPairK ElemK, int iSerialLcom, _DGN_FORC_CRC& Force);
	BOOL GetStressLcomResult(ElemPairK ElemK, int iSerialLcom, _DGN_STRS_CRC& Stress);
	BOOL GetStressLcomResult_Part1(ElemPairK ElemK, int iSerialLcom, _DGN_STRS_CRC& Stress);
	BOOL GetStressLcomResult_Part2(ElemPairK ElemK, int iSerialLcom, _DGN_STRS_CRC& Stress);
    BOOL GetStressLcomLTerm_Part2(ElemPairK ElemK, int iSerialLcom, _DGN_STRS_CRC& Stress);
    BOOL GetStressLcomSTerm_Part2(ElemPairK ElemK, int iSerialLcom, _DGN_STRS_CRC& Stress);
    BOOL GetStressLcomResult_PartScfpCr(ElemPairK ElemK, int iSerialLcom, _DGN_SCFP_PART_LCASE& Stress);
	BOOL GetStressLcomResult_PartScfpSh(ElemPairK ElemK, int iSerialLcom, _DGN_SCFP_PART_LCASE& Stress);
	BOOL GetStressLcomResult_PartScfpTe(ElemPairK ElemK, int iSerialLcom, _DGN_SCFP_PART_LCASE& Stress);
	BOOL GetStressLcomResult_PartScfpRe(ElemPairK ElemK, int iSerialLcom, _DGN_SCFP_PART_LCASE& Stress);
	BOOL GetForceLcomResult_ScfpSum(ElemPairK ElemK, int iSerialLcom, _DGN_FORC_CRC& Force);
	BOOL GetForceLcomResult_ScfpCrp(ElemPairK ElemK, int iSerialLcom, _DGN_FORC_CRC& Force);
	BOOL GetForceLcomResult_ScfpShr(ElemPairK ElemK, int iSerialLcom, _DGN_FORC_CRC& Force);

	BOOL GetStressTnprResult(ElemPairK ElemK, int iSerialLcom, _DGN_STRS_CRC& Stress);
	BOOL GetStressTnscResult(ElemPairK ElemK, int iSerialLcom, _DGN_STRS_CRC& Stress);
	BOOL GetStressLTermResult(ElemPairK ElemK, int iSerialLcom, _DGN_STRS_CRC& Stress);
	BOOL GetStressSTermResult(ElemPairK ElemK, int iSerialLcom, _DGN_STRS_CRC& Stress);
	BOOL GetStssLcomResult(ElemPairK ElemK, int iSerialLcom, _DGN_SHRS_ADD& ShrsAdd);
	BOOL GetStressTnprPart2Result(ElemPairK ElemK, int iSerialLcom, _DGN_STRS_CRC& Stress);
	BOOL GetStressTnscPart2Result(ElemPairK ElemK, int iSerialLcom, _DGN_STRS_CRC& Stress);
	// For TB 10002.3-2005
	BOOL GetForceDL_TB05Result(ElemPairK ElemK, int iSerialLcom, _DGN_FORC_CRC& Force);
	BOOL GetForceLL_TB05Result(ElemPairK ElemK, int iSerialLcom, _DGN_FORC_CRC& Force);
	BOOL GetStressDL_TB05Result(ElemPairK ElemK, int iSerialLcom, _DGN_STRS_CRC& Stress);
	BOOL GetStressLL_TB05Result(ElemPairK ElemK, int iSerialLcom, _DGN_STRS_CRC& Stress);
	BOOL GetForceCSTP_TB05Result(ElemPairK ElemK, int iSerialLcom, _DGN_FORC_CRC& Force);
	BOOL GetStressCSTP_TB05Result(ElemPairK ElemK, int iSerialLcom, _DGN_STRS_CRC& Stress);
	// For JTG/B02-01-2008
	BOOL GetForceEQE2_JTGResult(ElemPairK ElemK, int iSerialLcom, _DGN_FORC_CRC& Force);
	BOOL GetForceWeight_JTGResult(ElemPairK ElemK, int iSerialLcom, _DGN_FORC_CRC& Force);

	//////////////////////////////////////////////////////////////////////////
	// Expansion Force & Stress Position : 2 Positions -> 5 Positions
	//////////////////////////////////////////////////////////////////////////
	BOOL GetForceLcomResult_EX(ElemPairK ElemK, int iSerialLcom, _DGN_FORC_CRC_EX& Force);
	BOOL GetForceLcomResult_Part1_EX(ElemPairK ElemK, int iSerialLcom, _DGN_FORC_CRC_EX& Force);
	BOOL GetForceLcomResult_Part2_EX(ElemPairK ElemK, int iSerialLcom, _DGN_FORC_CRC_EX& Force);
	BOOL GetForceLcomExTpResult_EX(ElemPairK ElemK, int iSerialLcom, _DGN_FORC_CRC_EX& Force);
	BOOL GetForceStageResult_EX(ElemPairK ElemK, int iSerialLcom, _DGN_FORC_CRC_EX& Force);
	BOOL GetShrsStageResult_EX(ElemPairK ElemK, int iSerialLcom, _DGN_SHRS_ADD_EX& ShrsAdd);
	BOOL GetForceTnprResult_EX(ElemPairK ElemK, int iSerialLcom, _DGN_FORC_CRC_EX& Force);
	BOOL GetForceTnscResult_EX(ElemPairK ElemK, int iSerialLcom, _DGN_FORC_CRC_EX& Force);
	BOOL GetStressAddResult_EX(ElemPairK ElemK, int iSerialLcom, _DGN_STRS_ADD_EX& StrsAdd);
	BOOL GetForceLTermResult_EX(ElemPairK ElemK, int iSerialLcom, _DGN_FORC_CRC_EX& Force);
	BOOL GetForceJtgTndnResult_EX(ElemPairK ElemK, int iSerialLcom, _DGN_FORC_CRC_EX& Force);
	BOOL GetStressLcomResult_EX(ElemPairK ElemK, int iSerialLcom, _DGN_STRS_CRC_EX& Stress);
	BOOL GetStressLcomResult_Part1_EX(ElemPairK ElemK, int iSerialLcom, _DGN_STRS_CRC_EX& Stress);
	BOOL GetStressLcomResult_Part2_EX(ElemPairK ElemK, int iSerialLcom, _DGN_STRS_CRC_EX& Stress);
	BOOL GetStressLcomResult_PartScfpCr_EX(ElemPairK ElemK, int iSerialLcom, _DGN_SCFP_PART_LCASE_EX& Stress);
	BOOL GetStressLcomResult_PartScfpSh_EX(ElemPairK ElemK, int iSerialLcom, _DGN_SCFP_PART_LCASE_EX& Stress);
	BOOL GetStressLcomResult_PartScfpTe_EX(ElemPairK ElemK, int iSerialLcom, _DGN_SCFP_PART_LCASE_EX& Stress);
	BOOL GetStressLcomResult_PartScfpRe_EX(ElemPairK ElemK, int iSerialLcom, _DGN_SCFP_PART_LCASE_EX& Stress);
	BOOL GetForceLcomResult_ScfpSum_EX(ElemPairK ElemK, int iSerialLcom, _DGN_FORC_CRC_EX& Force);
	BOOL GetForceLcomResult_ScfpCrp_EX(ElemPairK ElemK, int iSerialLcom, _DGN_FORC_CRC_EX& Force);
	BOOL GetForceLcomResult_ScfpShr_EX(ElemPairK ElemK, int iSerialLcom, _DGN_FORC_CRC_EX& Force);

	BOOL GetStressTnprResult_EX(ElemPairK ElemK, int iSerialLcom, _DGN_STRS_CRC_EX& Stress);
	BOOL GetStressTnscResult_EX(ElemPairK ElemK, int iSerialLcom, _DGN_STRS_CRC_EX& Stress);
	BOOL GetStressLTermResult_EX(ElemPairK ElemK, int iSerialLcom, _DGN_STRS_CRC_EX& Stress);
	BOOL GetStssLcomResult_EX(ElemPairK ElemK, int iSerialLcom, _DGN_SHRS_ADD_EX& ShrsAdd);
	BOOL GetStressTnprPart2Result_EX(ElemPairK ElemK, int iSerialLcom, _DGN_STRS_CRC_EX& Stress);
	BOOL GetStressTnscPart2Result_EX(ElemPairK ElemK, int iSerialLcom, _DGN_STRS_CRC_EX& Stress);
	// For TB 10002.3-2005
	BOOL GetForceDL_TB05Result_EX(ElemPairK ElemK, int iSerialLcom, _DGN_FORC_CRC_EX& Force);
	BOOL GetForceLL_TB05Result_EX(ElemPairK ElemK, int iSerialLcom, _DGN_FORC_CRC_EX& Force);
	BOOL GetStressDL_TB05Result_EX(ElemPairK ElemK, int iSerialLcom, _DGN_STRS_CRC_EX& Stress);
	BOOL GetStressLL_TB05Result_EX(ElemPairK ElemK, int iSerialLcom, _DGN_STRS_CRC_EX& Stress);
	BOOL GetForceCSTP_TB05Result_EX(ElemPairK ElemK, int iSerialLcom, _DGN_FORC_CRC_EX& Force);
	BOOL GetStressCSTP_TB05Result_EX(ElemPairK ElemK, int iSerialLcom, _DGN_STRS_CRC_EX& Stress);
	// For JTG/B02-01-2008
	BOOL GetForceEQE2_JTGResult_EX(ElemPairK ElemK, int iSerialLcom, _DGN_FORC_CRC_EX& Force);
	BOOL GetForceWeight_JTGResult_EX(ElemPairK ElemK, int iSerialLcom, _DGN_FORC_CRC_EX& Force);

	//////////////////////////////////////////////////////////////////////////
	// Load Case
	//////////////////////////////////////////////////////////////////////////
	BOOL GetLcaseForceStld(ElemPairK ElemK, T_STLD_K StldK, T_STLD_LCASE& StldLcase);
	BOOL GetLcaseForceSplc(ElemPairK ElemK, T_SPLC_K SplcK, T_STRB_LCASE& StrbLcase);
	BOOL GetLcaseForceEspc(ElemPairK ElemK, T_ESPC_K EspcK, T_STRB_LCASE& StrbLcase);
	BOOL GetLcaseForceThis(ElemPairK ElemK, T_THIS_K ThisK, T_STRB_LCASE& StrbLcase);
	BOOL GetLcaseForceSgld(ElemPairK ElemK, T_SGLD_K SgldK, T_STRB_LCASE& StrbLcase);
	BOOL GetLcaseForceStep(ElemPairK ElemK, T_STEP_K StepK, T_STRB_LCASE& StrbLcase);
	BOOL GetLcaseForceMvld(ElemPairK ElemK, T_MVLD_K MvldK, T_SBCF_LCASE& SbcfLcase);
	BOOL GetLcaseForceSmlc(ElemPairK ElemK, T_SMLC_K SmlcK, T_SBCF_LCASE& SbcfLcase);
	BOOL GetLcaseForceGilcStrb(ElemPairK ElemK, T_GILC_K GilcK, T_STRB_LCASE& StrbLcase);
	BOOL GetLcaseForceGilcSbcf(ElemPairK ElemK, T_GILC_K GilcK, T_SBCF_LCASE& SbcfLcase);
	BOOL GetLcaseForceLcomStrb(ElemPairK ElemK, T_LCOM_K LcomK, T_STRB_LCASE& StrbLcase);
	BOOL GetLcaseForceLcomSbcf(ElemPairK ElemK, T_LCOM_K LcomK, T_SBCF_LCASE& SbcfLcase);

	BOOL GetLoadCaseForceStld(ElemPairK ElemK, T_STLD_K StldK, _DGN_STRBFORC_LCASE &StldLcase);
	BOOL GetLoadCaseForceSplc(ElemPairK ElemK, T_SPLC_K SplcK, _DGN_STRBFORC_LCASE &StrbLcase);
	BOOL GetLoadCaseForceEspc(ElemPairK ElemK, T_ESPC_K EspcK, _DGN_STRBFORC_LCASE &StrbLcase);
	BOOL GetLoadCaseForceThis(ElemPairK ElemK, T_THIS_K ThisK, _DGN_STRBFORC_LCASE &StrbLcase);
	BOOL GetLoadCaseForceSgld(ElemPairK ElemK, T_SGLD_K SgldK, _DGN_STRBFORC_LCASE &StrbLcase);
	BOOL GetLoadCaseForceStep(ElemPairK ElemK, T_STEP_K StepK, _DGN_STRBFORC_LCASE &StrbLcase);
	BOOL GetLoadCaseForceMvld(ElemPairK ElemK, T_MVLD_K MvldK, _DGN_SBCFFORC_LCASE &SbcfLcase);
    BOOL GetLoadCaseForceMvldBS(ElemPairK ElemK, T_MVLD_K MvldK, _DGN_SBCFFORC_LCASE &SbcfLcase); // Civil v891 US - BS Assessment - CS454 Special Moving Load Case (CIVIL-1069)
	BOOL GetLoadCaseForceSmlc(ElemPairK ElemK, T_SMLC_K SmlcK, _DGN_SBCFFORC_LCASE &SbcfLcase);
	BOOL GetLoadCaseForceGilcStrb(ElemPairK ElemK, T_GILC_K GilcK, _DGN_STRBFORC_LCASE &StrbLcase);
	BOOL GetLoadCaseForceGilcSbcf(ElemPairK ElemK, T_GILC_K GilcK, _DGN_SBCFFORC_LCASE &SbcfLcase);
	BOOL GetLoadCaseForceLcomStrb(ElemPairK ElemK, T_LCOM_K LcomK, _DGN_STRBFORC_LCASE &StrbLcase);
	BOOL GetLoadCaseForceLcomStrbMin(ElemPairK ElemK, T_LCOM_K LcomK, _DGN_STRBFORC_LCASE &StrbLcase);
	BOOL GetLoadCaseForceLcomSbcf(ElemPairK ElemK, T_LCOM_K LcomK, _DGN_SBCFFORC_LCASE &SbcfLcase);
	//
	BOOL GetPartLoadCaseForceStld(ElemPairK ElemK, T_STLD_K StldK, _DGN_STRBFORC_PART_LCASE &StldLcase);
	BOOL GetPartLoadCaseForceSplc(ElemPairK ElemK, T_SPLC_K SplcK, _DGN_STRBFORC_PART_LCASE &StrbLcase);
	BOOL GetPartLoadCaseForceEspc(ElemPairK ElemK, T_ESPC_K EspcK, _DGN_STRBFORC_PART_LCASE &StrbLcase);
	BOOL GetPartLoadCaseForceThis(ElemPairK ElemK, T_THIS_K ThisK, _DGN_STRBFORC_PART_LCASE &StrbLcase);
	BOOL GetPartLoadCaseForceSgld(ElemPairK ElemK, T_SGLD_K SgldK, _DGN_STRBFORC_PART_LCASE &StrbLcase);
	BOOL GetPartLoadCaseForceStep(ElemPairK ElemK, T_STEP_K StepK, _DGN_STRBFORC_PART_LCASE &StrbLcase);
	BOOL GetPartLoadCaseForceMvld(ElemPairK ElemK, T_MVLD_K MvldK, _DGN_SBCFFORC_PART_LCASE &SbcfLcase);
    BOOL GetPartLoadCaseForceMvldBS(ElemPairK ElemK, T_MVLD_K MvldK, _DGN_SBCFFORC_PART_LCASE &SbcfLcase); // Civil v891 US - BS Assessment - CS454 Special Moving Load Case (CIVIL-1069)
	BOOL GetPartLoadCaseForceSmlc(ElemPairK ElemK, T_SMLC_K SmlcK, _DGN_SBCFFORC_PART_LCASE &SbcfLcase);
	BOOL GetPartLoadCaseForceGilcStrb(ElemPairK ElemK, T_GILC_K GilcK, _DGN_STRBFORC_PART_LCASE &StrbLcase);
	BOOL GetPartLoadCaseForceGilcSbcf(ElemPairK ElemK, T_GILC_K GilcK, _DGN_SBCFFORC_PART_LCASE &SbcfLcase);
	BOOL GetPartLoadCaseForceLcomStrb(ElemPairK ElemK, T_LCOM_K LcomK, _DGN_STRBFORC_PART_LCASE &StrbLcase);
	BOOL GetPartLoadCaseForceLcomStrbMin(ElemPairK ElemK, T_LCOM_K LcomK, _DGN_STRBFORC_PART_LCASE &StrbLcase);
	BOOL GetPartLoadCaseForceLcomSbcf(ElemPairK ElemK, T_LCOM_K LcomK, _DGN_SBCFFORC_PART_LCASE &SbcfLcase);
	BOOL GetPartLoadCaseForceStldScfp(ElemPairK ElemK, T_STLD_K StldK, _DGN_SCFP_PART_LCASE &ScfpLcase);
	BOOL GetPartLoadCaseForceSgldScfp(ElemPairK ElemK, T_SGLD_K SgldK, _DGN_SCFP_PART_LCASE &ScfpLcase);

	// 5 Position - Force
	BOOL GetLoadCaseForceStld_EX(ElemPairK ElemK, T_STLD_K StldK, _DGN_STRBFORC_LCASE_EX &StldLcase);
	BOOL GetLoadCaseForceSplc_EX(ElemPairK ElemK, T_SPLC_K SplcK, _DGN_STRBFORC_LCASE_EX &StrbLcase);
	BOOL GetLoadCaseForceEspc_EX(ElemPairK ElemK, T_ESPC_K EspcK, _DGN_STRBFORC_LCASE_EX &StrbLcase);
	BOOL GetLoadCaseForceThis_EX(ElemPairK ElemK, T_THIS_K ThisK, _DGN_STRBFORC_LCASE_EX &StrbLcase);
	BOOL GetLoadCaseForceSgld_EX(ElemPairK ElemK, T_SGLD_K SgldK, _DGN_STRBFORC_LCASE_EX &StrbLcase);
	BOOL GetLoadCaseForceStep_EX(ElemPairK ElemK, T_STEP_K StepK, _DGN_STRBFORC_LCASE_EX &StrbLcase);
	BOOL GetLoadCaseForceMvld_EX(ElemPairK ElemK, T_MVLD_K MvldK, _DGN_SBCFFORC_LCASE_EX &SbcfLcase);
    BOOL GetLoadCaseForceMvldBS_EX(ElemPairK ElemK, T_MVLD_K MvldK, _DGN_SBCFFORC_LCASE_EX &SbcfLcase); // Civil v891 US - BS Assessment - CS454 Special Moving Load Case (CIVIL-1069)
	BOOL GetLoadCaseForceSmlc_EX(ElemPairK ElemK, T_SMLC_K SmlcK, _DGN_SBCFFORC_LCASE_EX &SbcfLcase);
	BOOL GetLoadCaseForceGilcStrb_EX(ElemPairK ElemK, T_GILC_K GilcK, _DGN_STRBFORC_LCASE_EX &StrbLcase);
	BOOL GetLoadCaseForceGilcSbcf_EX(ElemPairK ElemK, T_GILC_K GilcK, _DGN_SBCFFORC_LCASE_EX &SbcfLcase);
	BOOL GetLoadCaseForceLcomStrb_EX(ElemPairK ElemK, T_LCOM_K LcomK, _DGN_STRBFORC_LCASE_EX &StrbLcase);
	BOOL GetLoadCaseForceLcomStrbMin_EX(ElemPairK ElemK, T_LCOM_K LcomK, _DGN_STRBFORC_LCASE_EX &StrbLcase);
	BOOL GetLoadCaseForceLcomSbcf_EX(ElemPairK ElemK, T_LCOM_K LcomK, _DGN_SBCFFORC_LCASE_EX &SbcfLcase);

	// 5 Position - Part Force
	BOOL GetPartLoadCaseForceStld_EX(ElemPairK ElemK, T_STLD_K StldK, _DGN_STRBFORC_PART_LCASE_EX &StldLcase);
	BOOL GetPartLoadCaseForceSplc_EX(ElemPairK ElemK, T_SPLC_K SplcK, _DGN_STRBFORC_PART_LCASE_EX &StrbLcase);
	BOOL GetPartLoadCaseForceEspc_EX(ElemPairK ElemK, T_ESPC_K EspcK, _DGN_STRBFORC_PART_LCASE_EX &StrbLcase);
	BOOL GetPartLoadCaseForceThis_EX(ElemPairK ElemK, T_THIS_K ThisK, _DGN_STRBFORC_PART_LCASE_EX &StrbLcase);
	BOOL GetPartLoadCaseForceSgld_EX(ElemPairK ElemK, T_SGLD_K SgldK, _DGN_STRBFORC_PART_LCASE_EX &StrbLcase);
	BOOL GetPartLoadCaseForceStep_EX(ElemPairK ElemK, T_STEP_K StepK, _DGN_STRBFORC_PART_LCASE_EX &StrbLcase);
	BOOL GetPartLoadCaseForceMvld_EX(ElemPairK ElemK, T_MVLD_K MvldK, _DGN_SBCFFORC_PART_LCASE_EX &SbcfLcase);
    BOOL GetPartLoadCaseForceMvldBS_EX(ElemPairK ElemK, T_MVLD_K MvldK, _DGN_SBCFFORC_PART_LCASE_EX &SbcfLcase); // Civil v891 US - BS Assessment - CS454 Special Moving Load Case (CIVIL-1069)
	BOOL GetPartLoadCaseForceSmlc_EX(ElemPairK ElemK, T_SMLC_K SmlcK, _DGN_SBCFFORC_PART_LCASE_EX &SbcfLcase);
	BOOL GetPartLoadCaseForceGilcStrb_EX(ElemPairK ElemK, T_GILC_K GilcK, _DGN_STRBFORC_PART_LCASE_EX &StrbLcase);
	BOOL GetPartLoadCaseForceGilcSbcf_EX(ElemPairK ElemK, T_GILC_K GilcK, _DGN_SBCFFORC_PART_LCASE_EX &SbcfLcase);
	BOOL GetPartLoadCaseForceLcomStrb_EX(ElemPairK ElemK, T_LCOM_K LcomK, _DGN_STRBFORC_PART_LCASE_EX &StrbLcase);
	BOOL GetPartLoadCaseForceLcomStrbMin_EX(ElemPairK ElemK, T_LCOM_K LcomK, _DGN_STRBFORC_PART_LCASE_EX &StrbLcase);
	BOOL GetPartLoadCaseForceLcomSbcf_EX(ElemPairK ElemK, T_LCOM_K LcomK, _DGN_SBCFFORC_PART_LCASE_EX &SbcfLcase);
	BOOL GetPartLoadCaseForceStldScfp_EX(ElemPairK ElemK, T_STLD_K StldK, _DGN_SCFP_PART_LCASE_EX &ScfpLcase);
	BOOL GetPartLoadCaseForceSgldScfp_EX(ElemPairK ElemK, T_SGLD_K SgldK, _DGN_SCFP_PART_LCASE_EX &ScfpLcase);

protected:
	void DisplayErrorMessage(LPCTSTR lpszErrorMsg, int nErrorCode);

};

#include "HeaderPost.h"

#endif // !defined(AFX_ANALYSISRESULT_H__15140B72_6DD3_4B80_8C71_0DA88A473054__INCLUDED_)

