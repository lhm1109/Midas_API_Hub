// DgnEurocode2_2_05.h: interface for the CDgnPscDataCtrl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DGNPscDataCtrl__INCLUDED_)
#define AFX_DGNPscDataCtrl__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DgnPscCommon.h"

// struct _BDCY_AASHTO;
// struct _CUMS_AASHTO;
// struct _CRMS_AASHTO;
// struct _BDCW_AASHTO;
// struct _FYSG_AASHTO;
// struct _FYCM_AASHTO;
// struct _FPMT_AASHTO;
// struct PSC_RUS_MEMB_RES_D;

class CDgn_PSCManager;

class CDgnPscDataCtrl: public CDgnPscCommon
{
public:
	CDgnPscDataCtrl();
	virtual ~CDgnPscDataCtrl();

	void SetDataCtrlPointer(CCRCDataCtrl* pDataCtrl, int iDgnCode);
	BOOL InitialData(int iDgnCode);

    BOOL Get_FysgData(ElemPairK ElemK, PSC_DATA_MEMB &MembInD, T_FYSG_PSC& rData);
    BOOL Get_FypcData(T_TDNA_K TdnaK, T_FYPC_AASHTO& rData);
	BOOL Get_FycmData(ElemPairK ElemK, PSC_DATA_MEMB &MembInD, T_FYCM_PSC& rData);
    BOOL Get_FpsgData(ElemPairK ElemK, PSC_DATA_MEMB &MembInD, T_FPSG_PSC& rData);
    BOOL Get_FpmsData(ElemPairK ElemK, PSC_DATA_MEMB &MembInD, T_FPMS_PSC& rData);
	BOOL Get_BdcwData(ElemPairK ElemK, PSC_DATA_MEMB &MembInD, T_BDCW_PSC& rData, PSC_BDCW_D &rRes);
	BOOL Get_BdcrData(ElemPairK ElemK, PSC_DATA_MEMB &MembInD, T_BDCR_PSC& rData);
	BOOL Get_CumsData(ElemPairK ElemK, PSC_DATA_MEMB &MembInD, double adMr[4], T_CUMS_PSC& rData);
	BOOL Get_CrmtData(ElemPairK ElemK, PSC_DATA_MEMB &MembInD, T_CRMT_PSC& rData);
	//
	BOOL Get_FatgData(ElemPairK ElemK, PSC_DATA_MEMB &MembInD, T_FATG_PSC& rData);
	//
    BOOL Get_FysgData_Composite(ElemPairK ElemK, PSC_DATA_MEMB &MembInD, T_FYSG_PSC_COMP& rData);
    BOOL Get_FycmData_Composite(ElemPairK ElemK, PSC_DATA_MEMB &MembInD, T_FYCM_PSC_COMP& rData);
    BOOL Get_FpsgData_Composite(ElemPairK ElemK, PSC_DATA_MEMB &MembInD, T_FPSG_PSC_COMP& rData);
    BOOL Get_FpmsData_Composite(ElemPairK ElemK, PSC_DATA_MEMB &MembInD, T_FPMS_PSC_COMP& rData);
	BOOL Get_BdcwData_Composite(ElemPairK ElemK, PSC_DATA_MEMB &MembInD, T_BDCW_PSC_COMP& rData, PSC_BDCW_D &rRes);
	BOOL Get_BdcrData_Composite(ElemPairK ElemK, PSC_DATA_MEMB &MembInD, T_BDCR_PSC_COMP& rData);
	BOOL Get_CumsData_Composite(ElemPairK ElemK, PSC_DATA_MEMB &MembInD, double adMr[4], double adZ_sh[4], double adCc_gr[4], double adCc_sl[4],
																																			double adForce_T[4], double adTst_sl[4], T_CUMS_PSC_COMP& rData);
	BOOL Get_CrmtData_Composite(ElemPairK ElemK, PSC_DATA_MEMB &MembInD, double adZ_sh[6], double adCc_gr[6], double adCc_sl[6], T_CRMT_PSC_COMP& rData);

    BOOL CalcfpsInfoLpPosition();
	BOOL CalcReCalcDevelopmentLength(ElemPairK ElemK, int nPos, PSC_DATA_POSI& InD);

	// AASHTO
	//BOOL Get_FysgData(T_ELEM_K ElemK, T_FYSG_AASHTO& rData);
	BOOL Get_FysgData(ElemPairK ElemK, T_FYSG_D& rData);
	BOOL Get_FypcData2(T_TDNA_K TdnaK, T_FYPC_AASHTO& rData);
	BOOL Get_FycmData(ElemPairK ElemK, T_FYCM_AASHTO& rData);
	BOOL Get_FssgData(ElemPairK ElemK, T_FPMT_JTG& rData);
	BOOL Get_FpmsData(ElemPairK ElemK, T_FPMS_JTG& rData);
	BOOL Get_TbarData(ElemPairK ElemK, T_TBAR_AASHTO& rData);
	BOOL Get_BdcyData(ElemPairK ElemK, T_BDCY_AASHTO& rData, T_PSCD_DGN_INFO& rDgnR, _BDCY_AASHTO* pRes, int nPrtUnit=0, BOOL IsRating=FALSE);
	BOOL Get_CumsData(ElemPairK ElemK, T_CUMS_AASHTO& rData, _CUMS_AASHTO* pRes, int nPrtUnit=0);
	BOOL Get_CrmtData(ElemPairK ElemK, T_CRMT_AASHTO& rData, _CRMS_AASHTO* pRes, int nPrtUnit=0);
	BOOL Get_BdcwData(ElemPairK ElemK, T_BDCW_AASHTO& rData, _BDCW_AASHTO* pRes, int nPrtUnit=0);
	BOOL Get_FysgData(ElemPairK ElemK, T_FYSG_AASHTO& rData, _FYSG_AASHTO* pRes, int nPrtUnit=0);
	BOOL Get_FycmData(ElemPairK ElemK, T_FYCM_AASHTO& rData, _FYCM_AASHTO* pRes, int nPrtUnit=0);
	//For Composite
	BOOL Get_FysgData(int nGirder, ElemPairK ElemK, T_FYSG_D& rData);
	BOOL Get_FycmData(int nGirder, ElemPairK ElemK, T_FYCM_AASHTO& rData);
    BOOL Get_PSCDgnData(ElemPairK ElemK, T_PSCD_DGN_INFO& rData);

	//IRC112
	BOOL Get_FysgData(ElemPairK ElemK, PSC_DATA_MEMB &MembInD, T_FYSG_PSC_IRC& rData);//IRC

	BOOL Get_FycmData(ElemPairK ElemK, PSC_DATA_MEMB &MembInD, T_FYCM_PSC_IRC& rData);//IRC
	BOOL Get_FpsgData(ElemPairK ElemK, PSC_DATA_MEMB &MembInD, T_FPSG_PSC_IRC& rData);//IRC
	BOOL Get_FpmsData(ElemPairK ElemK, PSC_DATA_MEMB &MembInD, T_FPMS_PSC_IRC& rData);//IRC
	BOOL Get_BdcwData(ElemPairK ElemK, PSC_DATA_MEMB &MembInD, T_BDCW_PSC_IRC& rData, PSC_BDCW_D &rRes);//IRC 
	BOOL Get_BdcrData(ElemPairK ElemK, PSC_DATA_MEMB &MembInD, T_BDCR_PSC_IRC& rData);//IRC
	BOOL Get_CumsData(ElemPairK ElemK, PSC_DATA_MEMB &MembInD, double adMr[4], T_CUMS_PSC_IRC& rData);//IRC
	BOOL Get_CrmtData(ElemPairK ElemK, PSC_DATA_MEMB &MembInD, T_CRMT_PSC_IRC& rData);//IRC
	//
	BOOL Get_FatgData(ElemPairK ElemK, PSC_DATA_MEMB &MembInD, T_FATG_PSC_IRC& rData);  //IRC
	//
	//End IRC112
	void SetMemberResDataSNiP(PSC_RUS_MEMB_RES_D &rData);
	BOOL MakeMemberResDataSNiP(ElemPairK ElemK, PSC_DATA_MEMB &PscMembD, PSC_RUS_MEMB_RES_D &rData);
	BOOL Get_FirstGroupLS(ElemPairK ElemK, PSC_DATA_MEMB &PscMembD, PSC_CUMS_SNIP_BASE *pCumsD);
	BOOL Get_FatiDataSNiP(ElemPairK ElemK, PSC_DATA_MEMB &PscMembD, PSC_FATI_SNIP_D &rData);
	BOOL Get_SecondGroupLS(ElemPairK ElemK, PSC_DATA_MEMB &PscMembD, T_FYSG_SNIP &rFysgD, T_FYCM_SNIP &FycmD, T_FPSG_SNIP &FpsgD, T_FPMS_SNIP &FpmsD);
	BOOL Get_BdcwDataSNiP(ElemPairK ElemK, PSC_DATA_MEMB &PscMembD, PSC_BDCW_SNIP_BASE *pBdcwD);

	BOOL Get_FypcDataSNiP(T_TDNA_K TdnaK, T_FYPC_SNIP &rData);
	BOOL Get_BdcwDataSNiP(ElemPairK ElemK, T_BDCW_SNIP &rData);
	BOOL Get_BdcrDataSNiP(ElemPairK ElemK, T_BDCR_SNIP &rData);
	BOOL Get_CumsDataSNiP(ElemPairK ElemK, T_CUMS_SNIP &rData);
	BOOL Get_FatiDataSNiP(ElemPairK ElemK, T_FATI_SNIP &rData);

	void ConvertCumsSNiP(const PSC_CUMS_SNIP_BASE &InData, T_CUMS_SNIP_BASE &rData);
	void ConvertCrmtSNiP(const PSC_CUMS_SNIP_BASE &InData, T_CRMT_SNIP_BASE &rData);
	void ConvertFatiSNiP(const PSC_FATI_SNIP_BASE &InData, T_FATI_SNIP_BASE &rData);
	void ConvertBdcwSNiP(const PSC_BDCW_SNIP_BASE &InData, T_BDCW_SNIP_BASE &rData);

	// AS 5100:5
	BOOL Get_StressCheckCS (ElemPairK ElemK, PSC_DATA_MEMB &MembInD, T_FYSG_PSC_AS &FysgD, T_FPMT_JTG &FpsgD);
	BOOL Get_StressCheckSLS(ElemPairK ElemK, PSC_DATA_MEMB &MembInD, T_FYCM_PSC_AS &FycmD, T_FPMS_PSC &FpmsD);
    BOOL Get_TransferCheck(ElemPairK ElemK, PSC_DATA_MEMB &MembInD, T_TRAN_PSC_AS& TranD);
	//BOOL Get_FysgData(T_ELEM_K ElemK, PSC_DATA_MEMB &MembInD, T_FYSG_PSC_AS& rData);
	//BOOL Get_FypcData(T_TDNA_K TdnaK, T_FYPC_AASHTO& rData);
	//BOOL Get_FycmData(T_ELEM_K ElemK, PSC_DATA_MEMB &MembInD, T_FYCM_PSC_AS& rData);
	//BOOL Get_FpsgData(T_ELEM_K ElemK, PSC_DATA_MEMB &MembInD, T_FPSG_PSC& rData);
	//BOOL Get_FpmsData(T_ELEM_K ElemK, PSC_DATA_MEMB &MembInD, T_FPMS_PSC& rData);
	BOOL Get_BdcwDataAS(ElemPairK ElemK, PSC_DATA_MEMB &MembInD, T_BDCW_PSC_AS& rData);
	BOOL Get_BdcrDataAS(ElemPairK ElemK, BOOL bChkFlex[4], PSC_DATA_MEMB &MembInD, T_BDCR_PSC_AS& rData);
	BOOL Get_CumsDataAS(ElemPairK ElemK, PSC_DATA_MEMB &MembInD, T_SHRR_PSC_AS& rData);
	BOOL Get_CrmtDataAS(ElemPairK ElemK, PSC_DATA_MEMB &MembInD, T_TORR_PSC_AS& rData);
	BOOL Get_BdcwDataAS(ElemPairK ElemK, PSC_DATA_MEMB &MembInD, _BDCW_PSC_LRFD_BASE *pData);
	BOOL Get_CumsDataAS(ElemPairK ElemK, PSC_DATA_MEMB &MembInD, _CUMS_AASHTO_BASE *pData);
	BOOL Get_CrmtDataAS(ElemPairK ElemK, PSC_DATA_MEMB &MembInD, _STCM_AASHTO_BASE *pData);

    // BS5400:4, BD44/15
    BOOL Get_StressCheckCSBS (ElemPairK ElemK, PSC_DATA_MEMB &MembInD, T_FYSG_PSC_BS &FysgD, T_FPSG_PSC &FpsgD);
    BOOL Get_StressCheckSLSBS(ElemPairK ElemK, PSC_DATA_MEMB &MembInD, T_FYCM_PSC_BS &FycmD, T_FPMS_PSC &FpmsD);
    BOOL Get_BdcwDataBS(ElemPairK ElemK, PSC_DATA_MEMB &MembInD, T_BDCW_PSC_BS& rData);
    BOOL Get_BdcrDataBS(ElemPairK ElemK, BOOL bChkFlex[4], PSC_DATA_MEMB &MembInD, T_BDCR_PSC_BS& rData);
    BOOL Get_CumsDataBS(ElemPairK ElemK, PSC_DATA_MEMB &MembInD, T_SHRR_PSC_BS& rData);
    BOOL Get_CrmtDataBS(ElemPairK ElemK, PSC_DATA_MEMB &MembInD, T_TORR_PSC_BS& rData);
    BOOL Get_BdcwDataBS(ElemPairK ElemK, PSC_DATA_MEMB &MembInD, PSC_BDCW_BASE *pData);
    BOOL Get_BdcrDataBS(ElemPairK ElemK, PSC_DATA_MEMB &MembInD, PSC_FRCR_D    *pData);
    BOOL Get_CumsDataBS(ElemPairK ElemK, PSC_DATA_MEMB &MembInD, PSC_CUMS_BASE *pData);
    BOOL Get_CrmtDataBS(ElemPairK ElemK, PSC_DATA_MEMB &MembInD, PSC_CRMT_BASE *pData);
    BOOL Get_FypcDataBS(T_TDNA_K TdnaK, T_FYPC_PSC_BS& rData);
	//IRS
	BOOL Get_FysgData(ElemPairK ElemK, PSC_DATA_MEMB &MembInD, T_FYSG_PSC_IRS& rData);
	BOOL Get_FycmData(ElemPairK ElemK, PSC_DATA_MEMB &MembInD, T_FYCM_PSC_IRS& rData);
	BOOL Get_FpsgData(ElemPairK ElemK, PSC_DATA_MEMB &MembInD, T_FPSG_PSC_IRS& rData);
	BOOL Get_FpmsData(ElemPairK ElemK, PSC_DATA_MEMB &MembInD, T_FPMS_PSC_IRS& rData);
	BOOL Get_BdcwData(ElemPairK ElemK, PSC_DATA_MEMB &MembInD, T_BDCW_PSC_IRS& rData, PSC_BDCW_D &rRes);
	BOOL Get_BdcrData(ElemPairK ElemK, PSC_DATA_MEMB &MembInD, T_BDCR_PSC_IRS& rData);
	BOOL Get_CumsData(ElemPairK ElemK, PSC_DATA_MEMB &MembInD, double adMr[4], T_CUMS_PSC_IRS& rData, PSC_CUMS_BASE& TempData);
	BOOL Get_CrmtData(ElemPairK ElemK, PSC_DATA_MEMB &MembInD, T_CRMT_PSC_IRS& rData);
	BOOL Get_FatgData(ElemPairK ElemK, PSC_DATA_MEMB &MembInD, T_FATG_PSC_IRS& rData);
	//IRS END


	// For Report
	BOOL Get_FysgReportData(ElemPairK ElemK, const T_FYSG_PSC& InData, PSC_FYSG_D& rData);
	BOOL Get_FypcReportData(T_TDNA_K TdnaK, const T_FYPC_AASHTO& InData, PSC_FYPC_D& rData);
	BOOL Get_FycmReportData(ElemPairK ElemK, const T_FYCM_PSC& InData, PSC_FYCM_D& rData);
	BOOL Get_FpsgReportData(ElemPairK ElemK, const T_FPSG_PSC& InData, PSC_FPSG_D& rData);
	BOOL Get_FpmsReportData(ElemPairK ElemK, const T_FPMS_PSC& InData, PSC_FPMS_D& rData);
	BOOL Get_BdcwReportData(ElemPairK ElemK, int nIJ, int nMaxMin, const T_BDCW_PSC_BASE& BaseD, PSC_DATA_POSI& InData, PSC_BDCW_BASE& rData);
	BOOL Get_BdcrReportData(ElemPairK ElemK, int nIJ, int nMaxMin, const T_BDCR_PSC_BASE& BaseD, PSC_DATA_POSI& InData, PSC_FRCR_D& rData);
	BOOL Get_CumsReportData(ElemPairK ElemK, int nIJ, int nMaxMin, const T_CUMS_PSC_BASE& BaseD, PSC_DATA_POSI& InData, PSC_CUMS_BASE& rData);
	BOOL Get_CrmtReportData(ElemPairK ElemK, int nIJ, int nMaxMin, const T_CRMT_PSC_BASE& BaseD, PSC_DATA_POSI& InData, PSC_CRMT_BASE& rData);

	//IRC112 Start
	// For Report
	BOOL Get_FysgReportData(ElemPairK ElemK, const T_FYSG_PSC_IRC& InData, PSC_FYSG_D& rData);

	BOOL Get_FycmReportData(ElemPairK ElemK, const T_FYCM_PSC_IRC& InData, PSC_FYCM_D& rData);
	BOOL Get_FpsgReportData(ElemPairK ElemK, const T_FPSG_PSC_IRC& InData, PSC_FPSG_D& rData);
	BOOL Get_FpmsReportData(ElemPairK ElemK, const T_FPMS_PSC_IRC& InData, PSC_FPMS_D& rData);
	BOOL Get_BdcwReportData(ElemPairK ElemK, int nIJ, int nMaxMin, const T_BDCW_PSC_BASE_IRC& BaseD, PSC_DATA_POSI& InData, PSC_BDCW_BASE& rData);
	BOOL Get_BdcrReportData(ElemPairK ElemK, int nIJ, int nMaxMin, const T_BDCR_PSC_BASE_IRC& BaseD, PSC_DATA_POSI& InData, PSC_FRCR_D& rData);
	BOOL Get_CumsReportData(ElemPairK ElemK, int nIJ, int nMaxMin, const T_CUMS_PSC_BASE_IRC& BaseD, PSC_DATA_POSI& InData, PSC_CUMS_BASE& rData);
	BOOL Get_CrmtReportData(ElemPairK ElemK, int nIJ, int nMaxMin, const T_CRMT_PSC_BASE_IRC& BaseD, PSC_DATA_POSI& InData, PSC_CRMT_BASE& rData);

	BOOL Get_FatgReportData    (ElemPairK ElemK, int nIJ, int nMaxMin, const T_FATG_PSC_BASE_IRC& BaseD, PSC_DATA_POSI& InData, PSC_FATG_BASE& rData);
    BOOL Get_FysgDetailReportData(ElemPairK ElemK, int nIJ, int nMaxMin, const T_FYSG_PSC_BASE_IRC& BaseD, PSC_DATA_POSI& InData, PSC_FYSG_BASE& rData);
    BOOL Get_FycmDetailReportData(ElemPairK ElemK, int nIJ, int nMaxMin, const T_FYCM_PSC_BASE_IRC& BaseD, PSC_DATA_POSI& InData, PSC_FYCM_BASE& rData);

	//IRC112 End


	// For Report : PSC Composite
	BOOL Get_FysgCompReportData(ElemPairK ElemK, const T_FYSG_PSC_COMP& InData, PSC_FYSG_D& rData, BOOL bGirder);
	BOOL Get_FycmCompReportData(ElemPairK ElemK, const T_FYCM_PSC_COMP& InData, PSC_FYCM_D& rData, BOOL bGirder);
	BOOL Get_FpsgCompReportData(ElemPairK ElemK, const T_FPSG_PSC_COMP& InData, PSC_FPSG_D& rData, BOOL bGirder);
	BOOL Get_FpmsCompReportData(ElemPairK ElemK, const T_FPMS_PSC_COMP& InData, PSC_FPMS_D& rData, BOOL bGirder);

	BOOL Get_BdcwCompReportData(ElemPairK ElemK, int nIJ, int nMaxMin, const T_BDCW_PSC_COMP_BASE& BaseD, PSC_DATA_POSI& InData, PSC_BDCW_BASE& rData);
	BOOL Get_BdcrCompReportData(ElemPairK ElemK, int nIJ, int nMaxMin, const T_BDCR_PSC_COMP_BASE& BaseD, PSC_DATA_POSI& InData, PSC_FRCR_D& rData);
	BOOL Get_CumsCompReportData(ElemPairK ElemK, int nIJ, int nMaxMin, const T_CUMS_PSC_COMP_BASE& BaseD, PSC_DATA_POSI& InData, PSC_CUMS_BASE& rData);
	BOOL Get_CrmtCompReportData(ElemPairK ElemK, int nIJ, int nMaxMin, const T_CRMT_PSC_COMP_BASE& BaseD, PSC_DATA_POSI& InData, PSC_CRMT_BASE& rData);

	BOOL Get_FatgReportData    (ElemPairK ElemK, int nIJ, int nMaxMin, const T_FATG_PSC_BASE& BaseD, PSC_DATA_POSI& InData, PSC_FATG_BASE& rData);

	//
	BOOL Get_FysgReportData_SNiP(ElemPairK ElemK, const T_FYSG_SNIP& InData, PSC_FYSG_D& rData);
	BOOL Get_FypcReportData_SNiP(T_TDNA_K TdnaK, const T_FYPC_SNIP& InData, PSC_FYPC_D& rData);
	BOOL Get_FycmReportData_SNiP(ElemPairK ElemK, const T_FYCM_SNIP& InData, PSC_FYCM_D& rData);
	BOOL Get_FpsgReportData_SNiP(ElemPairK ElemK, const T_FPSG_SNIP& InData, PSC_FPSG_D& rData);
	BOOL Get_FpmsReportData_SNiP(ElemPairK ElemK, const T_FPMS_SNIP& InData, PSC_FPMS_D& rData);
	BOOL Get_BdcrReportData_SNiP(ElemPairK ElemK, int nIJ, int nMaxMin, const T_BDCR_SNIP_BASE& BaseD, PSC_DATA_POSI& InData, PSC_FRCR_D& rData);
	BOOL Get_CumsReportData_SNiP(ElemPairK ElemK, int nIJ, int nMaxMin, const T_CUMS_SNIP_BASE& BaseD, PSC_DATA_POSI& InData, PSC_CUMS_SNIP_BASE& rData);
	BOOL Get_CrmtReportData_SNiP(ElemPairK ElemK, int nIJ, int nMaxMin, const T_CRMT_SNIP_BASE& BaseD, PSC_DATA_POSI& InData, PSC_CUMS_SNIP_BASE& rData);
	BOOL Get_FatiReportData_SNiP(ElemPairK ElemK, int nIJ, int nMaxMin, const T_FATI_SNIP_BASE& BaseD, PSC_DATA_POSI& InData, PSC_FATI_SNIP_BASE& rData);
	BOOL Get_BdcwReportData_SNiP(ElemPairK ElemK, int nIJ, int nMaxMin, const T_BDCW_SNIP_BASE& BaseD, PSC_DATA_POSI& InData, PSC_BDCW_SNIP_BASE& rData);

	BOOL Get_BdcyReportData(BOOL bLoadRating, ElemPairK ElemK, int nIJ, int nMaxMin, const T_BDCY_AASHTO_BASE& BaseD, PSC_DATA_POSI& InData, _BDCY_PSC_LRFD_BASE& rData);
	BOOL Get_DesignConditionData(BOOL bLoadRating, ElemPairK ElemK, int nIJ, int nMaxMin, PSC_DATA_POSI& InData);

	BOOL Get_ReportBlock4SNiP(BOOL bRptPrint[6], PSC_DATA_POSI &InData, PSC_DRES_RPT_SNIP &rData);
	BOOL Get_BdcwAsminReportData(ElemPairK ElemK, int nIJ, const T_BDCW_PSC_BASE& BaseD, PSC_BDCW_BASE& rData);
	BOOL Get_BdcwCompAsminReportData(ElemPairK ElemK, int nIJ, const T_BDCW_PSC_COMP_BASE& BaseD, PSC_BDCW_BASE& rData);
	BOOL Get_FysgDetailReportData(ElemPairK ElemK, int nIJ, int nMaxMin, const T_FYSG_PSC_BASE& BaseD, PSC_DATA_POSI& InData, PSC_FYSG_BASE& rData);
	BOOL Get_FycmDetailReportData(ElemPairK ElemK, int nIJ, int nMaxMin, const T_FYCM_PSC_BASE& BaseD, PSC_DATA_POSI& InData, PSC_FYCM_BASE& rData);
	BOOL Get_FysgCompDetailReportData(ElemPairK ElemK, int nIJ, int nMaxMin, const T_FYSG_PSC_COMP_BASE& BaseD, PSC_DATA_POSI& InData, PSC_FYSG_BASE& rData);
	BOOL Get_FycmCompDetailReportData(ElemPairK ElemK, int nIJ, int nMaxMin, const T_FYCM_PSC_COMP_BASE& BaseD, PSC_DATA_POSI& InData, PSC_FYCM_BASE& rData);

	// AS
	BOOL Get_FysgReportData_AS(ElemPairK ElemK, const T_FYSG_PSC_AS& InData, _FYSG_AASHTO& FysgGr, _FYSG_AASHTO& FysgSl);
    BOOL Get_TranReportData_AS(ElemPairK ElemK, int nIJ, int nMaxMin, const T_TRAN_PSC_AS_BASE& BaseD, PSC_DATA_POSI& InData, _TRAN_PSC_BASE& TranD);
	//BOOL Get_FypcReportData_AS(T_TDNA_K TdnaK, const T_FYPC_SNIP& InData, PSC_FYPC_D& rData);
	BOOL Get_FycmReportData_AS(ElemPairK ElemK, const T_FYCM_PSC_AS& InData, _FYCM_AASHTO& FycmGr, _FYCM_AASHTO& FycmSl);
	BOOL Get_FpsgReportData_AS(ElemPairK ElemK, const T_FPMT_JTG& InData, _FPMT_AASHTO& rData);
	BOOL Get_FpmsReportData_AS(ElemPairK ElemK, const T_FPMS_PSC& InData, _FPMT_AASHTO& rData);
	BOOL Get_BdcrReportData_AS(ElemPairK ElemK, int nIJ, int nMaxMin, const T_BDCR_PSC_AS_BASE& BaseD, PSC_DATA_POSI& InData, _BDCY_PSC_LRFD_BASE& rData);
	BOOL Get_CumsReportData_AS(ElemPairK ElemK, int nIJ, int nMaxMin, const T_SHRR_PSC_AS_BASE& BaseD, PSC_DATA_POSI& InData, _CUMS_AASHTO_BASE& rData);
	BOOL Get_CrmtReportData_AS(ElemPairK ElemK, int nIJ, int nMaxMin, const T_TORR_PSC_AS_BASE& BaseD, PSC_DATA_POSI& InData, _STCM_AASHTO_BASE& rData);
	//BOOL Get_FatiReportData_AS(T_ELEM_K ElemK, int nIJ, int nMaxMin, const T_FATI_SNIP_BASE& BaseD, PSC_DATA_POSI& InData, PSC_FATI_SNIP_BASE& rData);
	BOOL Get_BdcwReportData_AS(ElemPairK ElemK, int nIJ, int nMaxMin, const T_BDCW_PSC_AS_BASE& BaseD, PSC_DATA_POSI& InData, _BDCW_PSC_LRFD_BASE& rData);

	//IRS Start
	// For Report
	BOOL Get_FysgReportData(ElemPairK ElemK, const T_FYSG_PSC_IRS& InData, PSC_FYSG_D& rData);
	BOOL Get_FycmReportData(ElemPairK ElemK, const T_FYCM_PSC_IRS& InData, PSC_FYCM_D& rData);
	BOOL Get_FpsgReportData(ElemPairK ElemK, const T_FPSG_PSC_IRS& InData, PSC_FPSG_D& rData);
	BOOL Get_FpmsReportData(ElemPairK ElemK, const T_FPMS_PSC_IRS& InData, PSC_FPMS_D& rData);
	BOOL Get_BdcwReportData(ElemPairK ElemK, int nIJ, int nMaxMin, const T_BDCW_PSC_BASE_IRS& BaseD, PSC_DATA_POSI& InData, PSC_BDCW_BASE& rData);
	BOOL Get_BdcrReportData(ElemPairK ElemK, int nIJ, int nMaxMin, const T_BDCR_PSC_BASE_IRS& BaseD, PSC_DATA_POSI& InData, PSC_FRCR_D& rData);
	BOOL Get_CumsReportData(ElemPairK ElemK, int nIJ, int nMaxMin, const T_CUMS_PSC_BASE_IRS& BaseD, PSC_DATA_POSI& InData, PSC_CUMS_BASE& rData);
	BOOL Get_CrmtReportData(ElemPairK ElemK, int nIJ, int nMaxMin, const T_CRMT_PSC_BASE_IRS& BaseD, PSC_DATA_POSI& InData, PSC_CRMT_BASE& rData);
	BOOL Get_FatgReportData    (ElemPairK ElemK, int nIJ, int nMaxMin, const T_FATG_PSC_BASE_IRS& BaseD, PSC_DATA_POSI& InData, PSC_FATG_BASE& rData);
	BOOL Get_FypcReportData(T_TDNA_K TdnaK, const T_FYPC_IRS& InData, PSC_FYPC_D& rData);
	//IRS End

    BOOL Get_BdcrReportData_BS(ElemPairK ElemK, int nIJ, int nMaxMin, const T_BDCR_PSC_BS_BASE& BaseD, PSC_DATA_POSI& InData, PSC_FRCR_D& rData);
    BOOL Get_CumsReportData_BS(ElemPairK ElemK, int nIJ, int nMaxMin, const T_SHRR_PSC_BS_BASE& BaseD, PSC_DATA_POSI& InData, PSC_CUMS_BASE &rData);
    BOOL Get_CrmtReportData_BS(ElemPairK ElemK, int nIJ, int nMaxMin, const T_TORR_PSC_BS_BASE& BaseD, PSC_DATA_POSI& InData, PSC_CRMT_BASE &rData);
    BOOL Get_FycmDetailReportData_BS(ElemPairK ElemK, int nIJ, int nMaxMin, const T_FYCM_PSC_BS_BASE& BaseD, PSC_DATA_POSI& InData, PSC_FYCM_BASE &rData);
    BOOL Get_FysgDetailReportData_BS(ElemPairK ElemK, int nIJ, int nMaxMin, const T_FYSG_PSC_BS_BASE& BaseD, PSC_DATA_POSI& InData, PSC_FYSG_BASE &rData);
    BOOL Get_BdcwReportData_BS(ElemPairK ElemK, const T_BDCW_PSC_BS_BASE& BaseD, PSC_DATA_POSI& InData, PSC_BDCW_BASE &rData);
    
    BOOL Get_FysgReportData_BS(ElemPairK ElemK, const T_FYSG_PSC_BS& InData, PSC_FYSG_D& FysgGr, PSC_FYSG_D& FysgSl);
    BOOL Get_FycmReportData_BS(ElemPairK ElemK, const T_FYCM_PSC_BS& InData, PSC_FYCM_D& FycmGr, PSC_FYCM_D& FycmSl);
    BOOL Get_FpsgReportData_BS(ElemPairK ElemK, const T_FPSG_PSC& InData, PSC_FPSG_D& rData);
    BOOL Get_FpmsReportData_BS(ElemPairK ElemK, const T_FPMS_PSC& InData, PSC_FPMS_D& rData);
    BOOL Get_FypcReportData_BS(T_TDNA_K TdnaK, const T_FYPC_PSC_BS& BaseD, PSC_FYPC_D& rData);

	// Rating
	BOOL Get_BdcrRatingData(ElemPairK ElemK, int nIJ, int nMaxMin, T_BDCR_PSC_BASE& BaseD, PSC_DATA_POSI& InData, PSC_FRCR_D& rData);
	BOOL Get_BdcrCompRatingData(ElemPairK ElemK, int nIJ, int nMaxMin, T_BDCR_PSC_COMP_BASE& BaseD, PSC_DATA_POSI& InData, PSC_FRCR_D& rData);
    
    //Rating PSC CS454
    BOOL Get_BdcrReportData_CS454(ElemPairK ElemK, int nIJ, int nMaxMin, const T_BDCR_PSC_BS_BASE& BaseD, PSC_DATA_POSI& InData, PSC_FRCR_D& rData);
    BOOL Get_CumsReportData_CS454(ElemPairK ElemK, int nIJ, int nMaxMin, const T_SHRR_PSC_BS_BASE& BaseD, PSC_DATA_POSI& InData, PSC_CUMS_BASE &rData);
    BOOL Get_CrmtReportData_CS454(ElemPairK ElemK, int nIJ, int nMaxMin, const T_TORR_PSC_BS_BASE& BaseD, PSC_DATA_POSI& InData, PSC_CRMT_BASE &rData);
    BOOL Get_ConcStressReportData_CS454(ElemPairK ElemK, int nIJ, int nMaxMin, const T_FYCM_PSC_BS_BASE& BaseD, PSC_DATA_POSI& InData, PSC_FYCM_BASE &rData);

protected:
	BOOL Check_Wmax_4Crack_Grade(int iExposureClass, int iLcomType, BOOL bPost,  BOOL bBonded, BOOL& bOK_Zero);
    CString GetRCaseName(T_ALCS_K AlcsK);
    double  GetGammaF(T_ALCS_K AlcsK);

protected:

	int m_iDgnCode;
	CDgn_PSCManager* m_pPscManager; 

	PSC_RUS_MEMB_RES_D *m_pMembResSNiP;

};

#endif // !defined(AFX_DGNPscDataCtrl__INCLUDED_)
