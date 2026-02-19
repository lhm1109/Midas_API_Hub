// Dgn_PSCReportModule_EC.h: Wraps the CVL_DgnReport_US.dll
//
//////////////////////////////////////////////////////////////////////

#if !defined(__DGN_PSCREPORTMODULE_EC_H__)
#define __DGN_PSCREPORTMODULE_EC_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Dgn_DLLModuleBase.h"

class CEUROCODEPSCRptDllModule : CDLLModuleBase
{
public:
	CEUROCODEPSCRptDllModule(){} 
	virtual ~CEUROCODEPSCRptDllModule(){}
	
	//Declare the functions exported in CVL_DgnReport_US.dll
	DECLARE_DLL_FUNCTION(BOOL, InitializeNewReport, (const CStringW & /*strBasePath*/, const CStringW & /*strDataPath*/, const CStringW & /*strPath*/))
	DECLARE_DLL_FUNCTION(BOOL, TerminateCurrentReport, (BOOL /*bIsAborted*/, BOOL/*bRegression*/))
	//DETAIL-REPORT
	DECLARE_DLL_FUNCTION(BOOL, Print_DetailReport_DgnCondition, (UINT /*iElemKey*/, UINT /*iIJ*/, void* /*pBdcyAashtoBase*/, size_t /*sizeBdcyAashtoBase*/, BOOL & /*bVersionOK*/ ))
	DECLARE_DLL_FUNCTION(BOOL, Print_DetailReport_FlexureDesign, (UINT /*iElemKey*/, UINT /*iIJ*/, void* /*pBdcyAashtoBase*/, size_t /*sizeBdcyAashtoBase*/, BOOL & /*bVersionOK*/ ))
	DECLARE_DLL_FUNCTION(BOOL, Print_DetailReport_ShearDesign, (UINT /*iElemKey*/, UINT /*iIJ*/, void* /*pCumsAashtoBase*/, size_t /*sizeCumsAashtoBase*/, BOOL & /*bVersionOK*/ ))
	DECLARE_DLL_FUNCTION(BOOL, Print_DetailReportorsionDesign, (UINT /*iElemKey*/, UINT /*iIJ*/, void* /*pCrmsAashtoBase*/, size_t /*sizeCrmsAashtoBase*/, BOOL & /*bVersionOK*/ ))
	DECLARE_DLL_FUNCTION(BOOL, Print_DetailReport_CrackDesign, (UINT /*iElemKey*/, UINT /*iIJ*/, void* /*pBdcwAashtoBase*/, size_t /*sizeBdcwAashtoBase*/, BOOL & /*bVersionOK*/ ))
	DECLARE_DLL_FUNCTION(BOOL, Print_DetailReport_ShearDesign_IRS, (UINT /*iElemKey*/, UINT /*iIJ*/, void* /*pCumsAashtoBase*/, size_t /*sizeCumsAashtoBase*/, size_t /*sizeBdcyAashtoBase*/,BOOL & /*bVersionOK*/ ))

	//RESULT-TABLE
	DECLARE_DLL_FUNCTION(BOOL, Print_AllowableStresses_Conc_ConstructionStage, (UINT /*iElemKey*/, UINT /*iIJ*/, void* /*pFysgAashtoBase*/, size_t /*sizeFysgAashtobase*/, BOOL & /*bVersionOK*/ ))
	DECLARE_DLL_FUNCTION(BOOL, Print_AllowableStresses_Conc_ServiceLoad, (UINT /*iElemKey*/, UINT /*iIJ*/, void* /*pFycmAashtoBase*/, size_t /*sizeFycmAashtobase*/, BOOL & /*bVersionOK*/ ))
	DECLARE_DLL_FUNCTION(BOOL, Print_AllowablePrincipalStress_Conc_ConstructionStage, (UINT /*iElemKey*/, UINT /*iIJ*/, void* /*pFpmtAashtoBase*/, size_t /*sizeFpmtAashtobase*/, BOOL & /*bVersionOK */))
	DECLARE_DLL_FUNCTION(BOOL, Print_AllowablePrincipalStress_Conc_ServiceLoad, (UINT /*iElemKey*/, UINT /*iIJ*/, void* /*pFpmtAashtoBase*/, size_t /*sizeFpmtAashtobase*/, BOOL & /*bVersionOK*/ ))
	DECLARE_DLL_FUNCTION(BOOL, Print_AllowableStressendon, (UINT /*iTndn*/, void* /*pFpmtAashto*/, size_t /*sizeFypcAashto*/, BOOL & /*bVersionOK*/))
	DECLARE_DLL_FUNCTION(BOOL, Print_CrackCheck, (UINT /*iElemKey*/, UINT /*iIJ*/, void* /*pBdcwAashtoBase*/, size_t /*sizeBdcwAashtoBase*/, BOOL & /*bVersionOK*/ ))
	DECLARE_DLL_FUNCTION(BOOL, Print_FlexureDesign, (UINT /*iElemKey*/, UINT /*iIJ*/, void* /*pBdcyAashtoBase*/, size_t /*sizeBdcyAashtoBase*/, BOOL & /*bVersionOK*/ ))
	DECLARE_DLL_FUNCTION(BOOL, Print_ShearDesign, (UINT /*iElemKey*/, UINT /*iIJ*/, void* /*pCumsAashtoBase*/, size_t /*sizeCumsAashtoBase*/, BOOL & /*bVersionOK*/ ))
	DECLARE_DLL_FUNCTION(BOOL, PrintorsionDesign, (UINT /*iElemKey*/, UINT /*iIJ*/, void* /*pCrmsAashtoBase*/, size_t /*sizeCrmsAashtoBase*/, BOOL & /*bVersionOK*/ ))
	

	//Initialize the dll and create functions so that they can be used in a class.
	BEGIN_DLL_INIT()
		INIT_DLL_FUNCTION(BOOL, InitializeNewReport, (const CStringW & /*strBasePath*/, const CStringW & /*strDataPath*/, const CStringW & /*strPath*/), ("InitializeNewReport"))
		INIT_DLL_FUNCTION(BOOL, TerminateCurrentReport, (BOOL /*bIsAborted*/, BOOL/*bRegression*/), ("TerminateCurrentReport"))
		//DETAIL-REPORT
		INIT_DLL_FUNCTION(BOOL, Print_DetailReport_DgnCondition, (UINT /*iElemKey*/, UINT /*iIJ*/, void* /*pBdcyAashtoBase*/, size_t /*sizeBdcyAashtoBase*/, BOOL & /*bVersionOK*/ ), ("Print_DetailReport_DgnCondition"))
	  INIT_DLL_FUNCTION(BOOL, Print_DetailReport_FlexureDesign, (UINT /*iElemKey*/, UINT /*iIJ*/, void* /*pBdcyAashtoBase*/, size_t /*sizeBdcyAashtoBase*/, BOOL & /*bVersionOK*/ ), ("Print_DetailReport_FlexureDesign"))
	  INIT_DLL_FUNCTION(BOOL, Print_DetailReport_ShearDesign, (UINT /*iElemKey*/, UINT /*iIJ*/, void* /*pCumsAashtoBase*/, size_t /*sizeCumsAashtoBase*/, BOOL & /*bVersionOK*/ ), ("Print_DetailReport_ShearDesign"))
	  INIT_DLL_FUNCTION(BOOL, Print_DetailReportorsionDesign, (UINT /*iElemKey*/, UINT /*iIJ*/, void* /*pCrmsAashtoBase*/, size_t /*sizeCrmsAashtoBase*/, BOOL & /*bVersionOK*/ ), ("Print_DetailReportorsionDesign"))
		INIT_DLL_FUNCTION(BOOL, Print_DetailReport_CrackDesign, (UINT /*iElemKey*/, UINT /*iIJ*/, void* /*pBdcwAashtoBase*/, size_t /*sizeBdcwAashtoBase*/, BOOL & /*bVersionOK*/ ), ("Print_DetailReport_CrackDesign"))
		//RESULT-TABLE
		INIT_DLL_FUNCTION(BOOL, Print_AllowableStresses_Conc_ConstructionStage, (UINT /*iElemKey*/, UINT /*iIJ*/, void* /*pFysgAashtoBase*/, size_t /*sizeFysgAashtobase*/, BOOL & /*bVersionOK*/ ), ("Print_AllowableStresses_Conc_ConstructionStage"))
		INIT_DLL_FUNCTION(BOOL, Print_AllowableStresses_Conc_ServiceLoad, (UINT /*iElemKey*/, UINT /*iIJ*/, void* /*pFycmAashtoBase*/, size_t /*sizeFycmAashtobase*/, BOOL & /*bVersionOK*/ ), ("Print_AllowableStresses_Conc_ServiceLoad"))
		INIT_DLL_FUNCTION(BOOL, Print_AllowablePrincipalStress_Conc_ConstructionStage, (UINT /*iElemKey*/, UINT /*iIJ*/, void* /*pFpmtAashtoBase*/, size_t /*sizeFpmtAashtobase*/, BOOL & /*bVersionOK */), ("Print_AllowablePrincipalStress_Conc_ConstructionStage"))
		INIT_DLL_FUNCTION(BOOL, Print_AllowablePrincipalStress_Conc_ServiceLoad, (UINT /*iElemKey*/, UINT /*iIJ*/, void* /*pFpmtAashtoBase*/, size_t /*sizeFpmtAashtobase*/, BOOL & /*bVersionOK*/ ), ("Print_AllowablePrincipalStress_Conc_ServiceLoad"))
		INIT_DLL_FUNCTION(BOOL, Print_AllowableStressendon, (UINT /*iTndn*/, void* /*pFpmtAashto*/, size_t /*sizeFypcAashto*/, BOOL & /*bVersionOK*/),  ("Print_AllowableStressendon"))
		INIT_DLL_FUNCTION(BOOL, Print_CrackCheck, (UINT /*iElemKey*/, UINT /*iIJ*/, void* /*pBdcwAashtoBase*/, size_t /*sizeBdcwAashtoBase*/, BOOL & /*bVersionOK*/ ), ("Print_CrackCheck"))
		INIT_DLL_FUNCTION(BOOL, Print_FlexureDesign, (UINT /*iElemKey*/, UINT /*iIJ*/, void* /*pBdcyAashtoBase*/, size_t /*sizeBdcyAashtoBase*/, BOOL & /*bVersionOK*/ ), ("Print_FlexureDesign"))
		INIT_DLL_FUNCTION(BOOL, Print_ShearDesign, (UINT /*iElemKey*/, UINT /*iIJ*/, void* /*pCumsAashtoBase*/, size_t /*sizeCumsAashtoBase*/, BOOL & /*bVersionOK*/ ), ("Print_ShearDesign"))
		INIT_DLL_FUNCTION(BOOL, PrintorsionDesign, (UINT /*iElemKey*/, UINT /*iIJ*/, void* /*pCrmsAashtoBase*/, size_t /*sizeCrmsAashtoBase*/, BOOL & /*bVersionOK*/ ), ("PrintorsionDesign"))
	END_DLL_INIT()
};


#endif // !defined(__DGN_PSCREPORTMODULE_EC2_H__)
