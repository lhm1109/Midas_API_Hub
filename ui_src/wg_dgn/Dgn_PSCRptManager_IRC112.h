// Dgn_PSCRptManager_IRC112.h: interface for the CDgn_PSCRptManager_IRC112 class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_Dgn_PSCRptManager_IRC112_H__320D0EDD_A144_432B_A6E6_1B91E6B8F83C__INCLUDED_)
#define AFX_Dgn_PSCRptManager_IRC112_H__320D0EDD_A144_432B_A6E6_1B91E6B8F83C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\wg_db\DBDoc.h"

#include "Dgn_DllManager_Base.h"
#include "DgnPscCommon.h"
#include "..\dgnengine\src\DgnBase\DgnBaseShape_Struct.h"
#include "..\dgnengine\src\DgnCalcBase\DgnCalcBase_MPhi_Struct.h"
#include "..\dgnengine\src\DgnReportBase\DgnReportBase_MPhi_Struct.h"
#include "..\dgnengine\src\CVL_DgnReport_US\DgnExcel_US_PSC_XLStruct.h"
#include "Dgn_PSCReportModule_EC.h"
#include "Dgn_PSCManager.h"

typedef BOOL (PSC_InitializeNewReport_EC)(void*, size_t, const BOOL & /*bRegression*/, BOOL&);
typedef BOOL (PSC_TerminateCurrentReport_EC)(const int /*iDgncode*/, BOOL /*bIsAborted*/, BOOL /*bIsRegression*/);
typedef BOOL (PSC_Print_Result_EC)(const int /*iDgncode*/, UINT /*iElemKey*/, UINT /*iIJ*/, BOOL /*bFinal*/, void* /*pFysgAashtoBase*/, size_t /*sizeFysgAashtobase*/, BOOL & /*bVersionOK*/ );
typedef BOOL (PSC_Print_Result_Tendon_EC)(const int /*iDgncode*/, UINT /*iTndn*/, BOOL /*bFinal*/, void* /*pFpmtAashto*/, size_t /*sizeFypcAashto*/, BOOL & /*bVersionOK*/);
typedef BOOL (PSC_Print_Detail_Condi_EC)(const int /*iDgncode*/, UINT /*iElemKey*/, UINT /*iIJ*/, void* /*pFysgAashtoBase*/, size_t /*sizeFysgAashtobase*/, BOOL & /*bVersionOK*/ );
typedef BOOL (PSC_Print_Detail_Design_EC)(const int /*iDgncode*/, UINT /*iElemKey*/, UINT /*iIJ*/, void* /*pFysgAashtoBase*/, size_t /*sizeFysgAashtobase*/, void* /*pFysgAashtoBase*/, size_t /*sizeFysgAashtobase*/, BOOL & /*bVersionOK*/ );

class CDgn_PSCRptManager_IRC112 : public CDgn_DllManager_Base       
{
public:
	CDgn_PSCRptManager_IRC112();
	virtual ~CDgn_PSCRptManager_IRC112();

protected:
	PSC_InitializeNewReport_EC*     m_pInitNewReport_EC;
	PSC_TerminateCurrentReport_EC*  m_pTerminateReport_EC;
	PSC_Print_Result_EC*            m_pAlwStressCS_EC;
	PSC_Print_Result_EC*            m_pAlwStressServ_EC;
	PSC_Print_Result_EC*            m_pAlwPrincipalStressCS_EC;
	PSC_Print_Result_EC*            m_pAlwPrincipalStressServ_EC;
	PSC_Print_Result_Tendon_EC*     m_pTendon_EC;  
	PSC_Print_Result_EC*            m_pCrack_EC;
	PSC_Print_Result_EC*            m_pFlexure_EC;
	PSC_Print_Result_EC*            m_pShear_EC;
	PSC_Print_Result_EC*            m_pTorsion_EC;
	//
	PSC_Print_Detail_Design_EC*     m_pDgnCondition;
	PSC_Print_Detail_Design_EC*     m_pFlexureDetail;
	PSC_Print_Detail_Design_EC*     m_pShearDetail;
	PSC_Print_Detail_Design_EC*     m_pTorsionDetail;
	PSC_Print_Detail_Design_EC*     m_pCrackDetail;
	PSC_Print_Detail_Design_EC*     m_pZerostress;
	PSC_Print_Detail_Design_EC*     m_pFatigDetail;
	PSC_Print_Detail_Design_EC*     m_pAlwStressCS_EC_Detail;
	PSC_Print_Detail_Design_EC*     m_pAlwStressSL_EC_Detail;
public:
	BOOL InitialData(int iNationType);
	BOOL Initialize(int iDgnCode, LPCTSTR szProgramDir, LPCTSTR szProjectFileDir, LPCTSTR szSaveFileName);
	BOOL Terminate(int iDgnCode, BOOL bIsAborted);
	

	// RESULT-TABLE
	BOOL Print_AllowableStresses_Conc_ConstructionStage_EC(int iDgnCode, ElemPairK iElemKey, UINT iIJ, BOOL bFinal, PSC_FYSG_BASE& FysgBase);
	BOOL Print_AllowableStresses_Conc_ServiceLoad_EC(int iDgnCode, ElemPairK iElemKey, UINT iIJ, BOOL bFinal, PSC_FYCM_BASE& FycmBase);
	BOOL Print_AllowablePrincipalStress_Conc_ConstructionStage_EC(int iDgnCode, ElemPairK iElemKey, UINT iIJ, BOOL bFinal, PSC_FPSG_BASE& FpsgBase);
	BOOL Print_AllowablePrincipalStress_Conc_ServiceLoad_EC(int iDgnCode, ElemPairK iElemKey, UINT iIJ, BOOL bFinal, PSC_FPMS_BASE& FpmsBase);
	BOOL Print_AllowableStress_Tendon_EC(int iDgnCode, UINT iTndn, BOOL bFinal, PSC_FYPC_D& Fypc);

	// DETAIL-REPORT
	BOOL Print_DetailReport_DgnCondition(int iDgnCode, ElemPairK iElemKey, UINT iIJ, PSC_DATA_POSI& InData, PSC_FRCR_D& FrcrD);
	BOOL Print_DetailReport_FlexureDesign(int iDgnCode, ElemPairK iElemKey, UINT iIJ, PSC_DATA_POSI& InData, PSC_FRCR_D& FrcrD);
	BOOL Print_DetailReport_ShearDesign(int iDgnCode, ElemPairK iElemKey, UINT iIJ, PSC_DATA_POSI& InData, PSC_CUMS_BASE& CumsBase);
	BOOL Print_DetailReport_TorsionDesign(int iDgnCode, ElemPairK iElemKey, UINT iIJ, PSC_DATA_POSI& InData, PSC_CRMT_BASE& CrmtBase);
	BOOL Print_DetailReport_CrackDesign(int iDgnCode, ElemPairK iElemKey, UINT iIJ, PSC_DATA_POSI& InData, PSC_BDCW_BASE& BdcwBase);
	BOOL Print_DetailReport_ZeroStressDesign(int iDgnCode, ElemPairK iElemKey, UINT iIJ, PSC_DATA_POSI& InData, PSC_BDCW_BASE& BdcwBase);
	BOOL Print_DetailReport_FatigueDesign(int iDgnCode, ElemPairK iElemKey, UINT iIJ, PSC_DATA_POSI& InData, PSC_FATG_BASE& FatgBase);
	BOOL Print_DetailReport_AllowableStresses_Fysg(int iDgnCode, ElemPairK iElemKey, UINT iIJ, PSC_DATA_POSI& InData, PSC_FYSG_BASE& FysgBase);
	BOOL Print_DetailReport_AllowableStresses_Fycm(int iDgnCode, ElemPairK iElemKey, UINT iIJ, PSC_DATA_POSI& InData, PSC_FYCM_BASE& FycmBase);

	// SUMMART-REPROT
	BOOL Print_FlexureDesign_EC(int iDgnCode, ElemPairK iElemKey, UINT iIJ, BOOL bFinal, PSC_FRCR_D& BdcwBase);
	BOOL Print_ShearDesign_EC(int iDgnCode, ElemPairK iElemKey, UINT iIJ, BOOL bFinal, PSC_CUMS_BASE& CumsBase);
	BOOL Print_TorsionDesign_EC(int iDgnCode, ElemPairK iElemKey, UINT iIJ, BOOL bFinal, PSC_CRMT_BASE& CrmtBase);
	BOOL Print_CrackCheck_EC(int iDgnCode, ElemPairK iElemKey, UINT iIJ, BOOL bFinal, PSC_BDCW_BASE& BdcwBase);

private:
	CEUROCODEPSCRptDllModule m_EurocodeReportModule;
	CDgn_PSCManager m_pscManager;
	
	BOOL m_bInitialized;

	CString Get_StageName(T_STAG_K StagK);
	CString Get_LcomName(T_LCOM_K LcomK);
};
#endif // !defined(AFX_Dgn_PSCRptManager_EC2_H__320D0EDD_A144_432B_A6E6_1B91E6B8F83C__INCLUDED_)

	/*
	//
public:
	BOOL GPrint_Ec2_PscReport(DGNEXCEL_US_PSC_OPTION PrintOption, CArray<DGNEXCEL_US_PSC_SECT, DGNEXCEL_US_PSC_SECT&>& arResult);
	BOOL ConvertResult_UnitFrcr_UnitData(const PSC_FRCR_D& FrcrD, DGNEXCEL_PSC_FRCR_BASE& ExlData);
	BOOL ConvertResult_UnitFrcr_UnitTndn(const PSC_TNDN_CALD& FrcrD, DGNEXCEL_PSC_TNDN_CALD& ExlData);
	void Get_CalcTndnList_Size(const CArray<PSC_FRCR_TNCD,PSC_FRCR_TNCD&>& arData, int& iSize);
	void Set_CalcTndnList_Size(int iSize, CArray<DGNEXCEL_PSC_FRCR_TNCD,DGNEXCEL_PSC_FRCR_TNCD&>& arData);
	BOOL ConvertResult_CalcTndn(const PSC_FRCR_TNCD& FrcrD, DGNEXCEL_PSC_FRCR_TNCD& ExlData);
	void Get_NeutAxisInfo_Size(const CArray<PSC_FRCR_NEUT_AXIS_D,PSC_FRCR_NEUT_AXIS_D&>& arData, int& iSize);
	void Set_NeutAxisInfo_Size(int iSize, CArray<DGNEXCEL_PSC_FRCR_NEUT_AXIS_D,DGNEXCEL_PSC_FRCR_NEUT_AXIS_D&>& arData);
	BOOL ConvertResult_NeutAxisInfo(const PSC_FRCR_NEUT_AXIS_D& FrcrD, DGNEXCEL_PSC_FRCR_NEUT_AXIS_D& ExlData);
	BOOL ConvertResult_Frcr_ToReportDlLL(const PSC_FRCR_D& FrcrD, DGNEXCEL_PSC_FRCR_BASE& ExlData);
	//
	BOOL ConvertResult_UnitCums_ToReportDlLL(const PSC_CUMS_BASE& CumsD, DGNEXCEL_PSC_CUMS_BASE& ExlData);
	BOOL ConvertResult_Cums_ToReportDlLL(const PSC_CUMS_D& CumsD, DGNEXCEL_PSC_CUMS_D& ExlData);
	//
	BOOL ConvertResult_UnitCrmt_UnitData(const PSC_CRMT_BASE& CrmtD, DGNEXCEL_PSC_CRMT_BASE& ExlData);
	BOOL ConvertResult_UnitCrmt_TorPartD(const PSC_CRMT_PART_D& CrmtD, DGNEXCEL_PSC_CRMT_PART_D& ExlData);
	BOOL ConvertResult_UnitCrmt_ToReportDlLL(const PSC_CRMT_BASE& CrmtD, DGNEXCEL_PSC_CRMT_BASE& ExlData);
	BOOL ConvertResult_Crmt_ToReportDlLL(const PSC_CRMT_D& CrmtD, DGNEXCEL_PSC_CRMT_D& ExlData);
	//
	BOOL ConvertResult_UnitFysg_ToReportDlLL(int iElemKey, const T_FYSG_PSC_BASE& FysgD, DGNEXCEL_PSC_FYSG_BASE& ExlData);
	BOOL ConvertResult_Fysg_ToReportDlLL(int iElemKey, const T_FYSG_PSC& FysgD, DGNEXCEL_PSC_FYSG_D& ExlData);
	//
	BOOL ConvertResult_UnitFycm_ToReportDlLL(int iElemKey, const T_FYCM_PSC_BASE& FycmD, DGNEXCEL_PSC_FYCM_BASE& ExlData);
	BOOL ConvertResult_Fycm_ToReportDlLL(int iElemKey, const T_FYCM_PSC& FycmD, DGNEXCEL_PSC_FYCM_D& ExlData);
	//
	BOOL ConvertResult_UnitFpsg_ToReportDlLL(int iElemKey, const T_FPSG_PSC_BASE& FpsgD, DGNEXCEL_PSC_FPSG_BASE& ExlData);
	BOOL ConvertResult_Fpsg_ToReportDlLL(int iElemKey, const T_FPSG_PSC& FpsgD, DGNEXCEL_PSC_FPSG_D& ExlData);
	//
	BOOL ConvertResult_UnitFpms_ToReportDlLL(int iElemKey, const T_FPMS_PSC_BASE& FpmsD, DGNEXCEL_PSC_FPMS_BASE& ExlData);
	BOOL ConvertResult_Fpms_ToReportDlLL(int iElemKey, const T_FPMS_PSC& FpmsD, DGNEXCEL_PSC_FPMS_D& ExlData);
	//
	BOOL ConvertResult_UnitBdcw_ToReportDlLL(const PSC_BDCW_BASE& BdcwD, DGNEXCEL_PSC_BDCW_BASE& ExlData);  
	BOOL ConvertResult_Bdcw_ToReportDlLL(const PSC_BDCW_D& BdcwD, DGNEXCEL_PSC_BDCW_D& ExlData);
	*/