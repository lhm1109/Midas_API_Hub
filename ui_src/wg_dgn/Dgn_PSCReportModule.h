// Dgn_PSCReportModule.h: Wraps the CVL_DgnReport_US.dll
//
//////////////////////////////////////////////////////////////////////

#if !defined(__DGN_PSCREPORTMODULE_H__)
#define __DGN_PSCREPORTMODULE_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Dgn_DLLModuleBase.h"

class CAASHTOPSCRptDllModule : CDLLModuleBase
{
public:
	CAASHTOPSCRptDllModule(){} 
	virtual ~CAASHTOPSCRptDllModule(){}
	
	//Declare the functions exported in CVL_DgnReport_US.dll
	DECLARE_DLL_FUNCTION(BOOL, InitializeNewReport, (const CStringW & /*strBasePath*/, const CStringW & /*strDataPath*/, const CStringW & /*strPath*/))
	DECLARE_DLL_FUNCTION(BOOL, TerminateCurrentReport, (BOOL /*bIsAborted*/, BOOL/*bRegression*/))
	DECLARE_DLL_FUNCTION(BOOL, Print_AllowableStresses_Conc_ConstructionStage, (UINT /*iElemKey*/, UINT /*iIJ*/, void* /*pFysgAashtoBase*/, size_t /*sizeFysgAashtobase*/, BOOL & /*bVersionOK*/ ))
	DECLARE_DLL_FUNCTION(BOOL, Print_AllowableStresses_Conc_ConstructionStage_Comp, (UINT /*iElemKey*/, UINT /*iIJ*/, void* /*pFysgAashtoBase*/, size_t /*sizeFysgAashtobase*/, BOOL & /*bVersionOK*/ , BOOL /*bGirder*/))
	DECLARE_DLL_FUNCTION(BOOL, Print_AllowableStresses_Conc_ServiceLoadCombination, (UINT /*iElemKey*/, UINT /*iIJ*/, void* /*pFycmAashtoBase*/, size_t /*sizeFycmAashtobase*/, BOOL & /*bVersionOK*/ ))
	DECLARE_DLL_FUNCTION(BOOL, Print_AllowableStresses_Conc_ServiceLoadCombination_Comp, (UINT /*iElemKey*/, UINT /*iIJ*/, void* /*pFycmAashtoBase*/, size_t /*sizeFycmAashtobase*/, BOOL & /*bVersionOK*/ , BOOL /*bGirder*/))
	DECLARE_DLL_FUNCTION(BOOL, Print_AllowablePrincipalStress_Conc_ConstructionStage, (UINT /*iElemKey*/, UINT /*iIJ*/, void* /*pFpmtAashtoBase*/, size_t /*sizeFpmtAashtobase*/, BOOL & /*bVersionOK */))
	DECLARE_DLL_FUNCTION(BOOL, Print_AllowablePrincipalStress_Conc_ServiceLoadCombination_MaxShear, (UINT /*iElemKey*/, UINT /*iIJ*/, void* /*pFpmtAashtoBase*/, size_t /*sizeFpmtAashtobase*/, BOOL & /*bVersionOK*/ ))
	DECLARE_DLL_FUNCTION(BOOL, Print_AllowablePrincipalStress_Conc_ServiceLoadCombination_MaxTorsion, (UINT/* iElemKey*/, UINT /*iIJ*/, void* /*pFpmtAashtoBase*/, size_t /*sizeFpmtAashtobase*/, BOOL & /*bVersionOK*/ ))
	DECLARE_DLL_FUNCTION(BOOL, Print_AllowableStress_Tendon, (UINT /*iTndn*/, void* /*pFpmtAashto*/, size_t /*sizeFypcAashto*/, BOOL & /*bVersionOK*/))
	DECLARE_DLL_FUNCTION(BOOL, Print_TensionRebarRequired, (UINT /*iElemKey*/, UINT /*iIJ*/, void* /*pTbarAashtoBase*/, size_t /*sizeTbarAashtoBase*/, BOOL & /*bVersionOK*/ ))
	DECLARE_DLL_FUNCTION(BOOL, Print_CrackCheck, (UINT /*iElemKey*/, UINT /*iIJ*/, void* /*pBdcwAashtoBase*/, size_t /*sizeBdcwAashtoBase*/, BOOL & /*bVersionOK*/ ))
	DECLARE_DLL_FUNCTION(BOOL, Print_FlexureDesign, (UINT /*iElemKey*/, UINT /*iIJ*/, void* /*pBdcyAashtoBase*/, size_t /*sizeBdcyAashtoBase*/, BOOL & /*bVersionOK*/ ))
	DECLARE_DLL_FUNCTION(BOOL, Print_ShearDesign, (UINT /*iElemKey*/, UINT /*iIJ*/, void* /*pCumsAashtoBase*/, size_t /*sizeCumsAashtoBase*/, BOOL & /*bVersionOK*/ ))
	DECLARE_DLL_FUNCTION(BOOL, Print_TorsionDesign, (UINT /*iElemKey*/, UINT /*iIJ*/, void* /*pCrmsAashtoBase*/, size_t /*sizeCrmsAashtoBase*/, BOOL & /*bVersionOK*/ ))

	//Initialize the dll and create functions so that they can be used in a class.
	BEGIN_DLL_INIT()
		INIT_DLL_FUNCTION(BOOL, InitializeNewReport, (const CStringW & /*strBasePath*/, const CStringW & /*strDataPath*/, const CStringW & /*strPath*/), "InitializeNewReport")
		INIT_DLL_FUNCTION(BOOL, TerminateCurrentReport, (BOOL /*bIsAborted*/, BOOL/*bRegression*/), "TerminateCurrentReport")
		INIT_DLL_FUNCTION(BOOL, Print_AllowableStresses_Conc_ConstructionStage, (UINT /*iElemKey*/, UINT /*iIJ*/, void* /*pFysgAashtoBase*/, size_t /*sizeFysgAashtobase*/, BOOL & /*bVersionOK*/ ), "Print_AllowableStresses_Conc_ConstructionStage")
		INIT_DLL_FUNCTION(BOOL, Print_AllowableStresses_Conc_ConstructionStage_Comp, (UINT /*iElemKey*/, UINT /*iIJ*/, void* /*pFysgAashtoBase*/, size_t /*sizeFysgAashtobase*/, BOOL & /*bVersionOK*/ , BOOL /*bGirder*/), "Print_AllowableStresses_Conc_ConstructionStage_Composite")
		INIT_DLL_FUNCTION(BOOL, Print_AllowableStresses_Conc_ServiceLoadCombination, (UINT /*iElemKey*/, UINT /*iIJ*/, void* /*pFycmAashtoBase*/, size_t /*sizeFycmAashtobase*/, BOOL & /*bVersionOK*/ ), "Print_AllowableStresses_Conc_ServiceLoadCombination")
		INIT_DLL_FUNCTION(BOOL, Print_AllowableStresses_Conc_ServiceLoadCombination_Comp, (UINT /*iElemKey*/, UINT /*iIJ*/, void* /*pFycmAashtoBase*/, size_t /*sizeFycmAashtobase*/, BOOL & /*bVersionOK*/, BOOL /*bGirder*/ ), "Print_AllowableStresses_Conc_ServiceLoadCombination_Composite")
		INIT_DLL_FUNCTION(BOOL, Print_AllowablePrincipalStress_Conc_ConstructionStage, (UINT /*iElemKey*/, UINT /*iIJ*/, void* /*pFpmtAashtoBase*/, size_t /*sizeFpmtAashtobase*/, BOOL & /*bVersionOK */), "Print_AllowablePrincipalStress_Conc_ConstructionStage")
		INIT_DLL_FUNCTION(BOOL, Print_AllowablePrincipalStress_Conc_ServiceLoadCombination_MaxShear, (UINT /*iElemKey*/, UINT /*iIJ*/, void* /*pFpmtAashtoBase*/, size_t /*sizeFpmtAashtobase*/, BOOL & /*bVersionOK*/ ), "Print_AllowablePrincipalStress_Conc_ServiceLoadCombination_MaxShear")
		INIT_DLL_FUNCTION(BOOL, Print_AllowablePrincipalStress_Conc_ServiceLoadCombination_MaxTorsion, (UINT/* iElemKey*/, UINT /*iIJ*/, void* /*pFpmtAashtoBase*/, size_t /*sizeFpmtAashtobase*/, BOOL & /*bVersionOK*/ ), "Print_AllowablePrincipalStress_Conc_ServiceLoadCombination_MaxTorsion")
		INIT_DLL_FUNCTION(BOOL, Print_AllowableStress_Tendon, (UINT /*iTndn*/, void* /*pFpmtAashto*/, size_t /*sizeFypcAashto*/, BOOL & /*bVersionOK*/),  "Print_AllowableStress_Tendon")
		INIT_DLL_FUNCTION(BOOL, Print_TensionRebarRequired, (UINT /*iElemKey*/, UINT /*iIJ*/, void* /*pTbarAashtoBase*/, size_t /*sizeTbarAashtoBase*/, BOOL & /*bVersionOK*/ ), "Print_TensionRebarRequired")
		INIT_DLL_FUNCTION(BOOL, Print_CrackCheck, (UINT /*iElemKey*/, UINT /*iIJ*/, void* /*pBdcwAashtoBase*/, size_t /*sizeBdcwAashtoBase*/, BOOL & /*bVersionOK*/ ), "Print_CrackCheck")
		INIT_DLL_FUNCTION(BOOL, Print_FlexureDesign, (UINT /*iElemKey*/, UINT /*iIJ*/, void* /*pBdcyAashtoBase*/, size_t /*sizeBdcyAashtoBase*/, BOOL & /*bVersionOK*/ ), "Print_FlexureDesign")
		INIT_DLL_FUNCTION(BOOL, Print_ShearDesign, (UINT /*iElemKey*/, UINT /*iIJ*/, void* /*pCumsAashtoBase*/, size_t /*sizeCumsAashtoBase*/, BOOL & /*bVersionOK*/ ), "Print_ShearDesign")
		INIT_DLL_FUNCTION(BOOL, Print_TorsionDesign, (UINT /*iElemKey*/, UINT /*iIJ*/, void* /*pCrmsAashtoBase*/, size_t /*sizeCrmsAashtoBase*/, BOOL & /*bVersionOK*/ ), "Print_TorsionDesign")
	END_DLL_INIT()
};


#endif // !defined(__DGN_PSCREPORTMODULE_H__)
