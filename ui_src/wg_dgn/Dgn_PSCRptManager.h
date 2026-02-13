// Dgn_PSCRptManager.h: interface for the Dgn_PSCRptManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(__DGN_PSCRPTMANAGER_H__)
#define __DGN_PSCRPTMANAGER_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\wg_db\DBDoc.h"

#include "Dgn_DllManager_Base.h"
#include "DgnPscCommon.h"
//#include "..\dgnengine\src\DgnCalcBase\DgnCalcBase_PSC_ResultStruct.h"
//#include "..\dgnengine\src\CVL_DgnCalc_US\DgnCalc_US_PSC_Struct.h"
#include "Dgn_PSCReportModule.h"
#include "Dgn_PSCManager.h"

//typedef BOOL (PDGNCALC)(void*, void*, size_t, size_t, BOOL&);

typedef BOOL (PSC_InitializeNewReport)(void*, size_t, const BOOL & /*bRegression*/, BOOL&);
typedef BOOL (PSC_TerminateCurrentReport)(int /*iDgnCode*/, BOOL /*bIsAborted*/, BOOL /*bRegression*/, int /*nType*/);
typedef BOOL (PSC_Print_Result)(UINT /*iElemKey*/, UINT /*iIJ*/, CStringW& /*LcomName*/, void* /*pFysgAashtoBase*/, size_t /*sizeFysgAashtobase*/, BOOL & /*bVersionOK*/ );
typedef BOOL (PSC_Print_Result2)(int /*iDgnCode*/, UINT /*iElemKey*/, UINT /*iIJ*/, bool /*bFinal*/, CStringW& /*LcomName*/, void* /*pFysgAashtoBase*/, size_t /*sizeFysgAashtobase*/, BOOL & /*bVersionOK*/ );
typedef BOOL (PSC_Print_Result_Comp)(int /*iDgnCode*/, UINT /*iElemKey*/, UINT /*iIJ*/, bool /*bFinal*/, CStringW& /*LcomName*/, void* /*pFysgAashtoBase*/, size_t /*sizeFysgAashtobase*/, BOOL & /*bVersionOK*/, BOOL /* bGirder*/ );
//typedef BOOL (PSC_Print_AllowableStresses_Conc_ServiceLoadCombination, UINT /*iElemKey*/, UINT /*iIJ*/, void* pFycmAashtoBase, size_t /*sizeFycmAashtobase*/, BOOL & /*bVersionOK*/ , _T("Print_AllowableStresses_Conc_ServiceLoadCombination"))
//typedef BOOL (PSC_Print_AllowablePrincipalStress_Conc_ConstructionStage, UINT /*iElemKey*/, UINT /*iIJ*/, void* /*pFpmtAashtoBase*/, size_t /*sizeFpmtAashtobase*/, BOOL & /*bVersionOK */, _T("Print_AllowablePrincipalStress_Conc_ConstructionStage"))
//typedef BOOL (PSC_Print_AllowablePrincipalStress_Conc_ServiceLoadCombination_MaxShear, UINT /*iElemKey*/, UINT /*iIJ*/, void* /*pFpmtAashtoBase*/, size_t /*sizeFpmtAashtobase*/, BOOL & /*bVersionOK*/ , _T("Print_AllowablePrincipalStress_Conc_ServiceLoadCombination_MaxShear"))
//ypedef BOOL (PSC_Print_AllowablePrincipalStress_Conc_ServiceLoadCombination_MaxTorsion, UINT/* iElemKey*/, UINT /*iIJ*/, void* /*pFpmtAashtoBase*/, size_t /*sizeFpmtAashtobase*/, BOOL & /*bVersionOK*/ , _T("Print_AllowablePrincipalStress_Conc_ServiceLoadCombination_MaxTorsion"))
typedef BOOL (PSC_Print_Result_Tendon)(int /*iDgnCode*/, UINT /*iTndn*/, bool /*bFinal*/, CStringW& /*TendonName*/, void* /*pFpmtAashto*/, size_t /*sizeFypcAashto*/, BOOL & /*bVersionOK*/);
//typedef BOOL (PSC_Print_TensionRebarRequired, UINT /*iElemKey*/, UINT /*iIJ*/, void* /*pTbarAashtoBase*/, size_t /*sizeTbarAashtoBase*/, BOOL & /*bVersionOK*/ , _T("Print_TensionRebarRequired"))
//typedef BOOL (PSC_Print_CrackCheck, UINT /*iElemKey*/, UINT /*iIJ*/, void* /*pBdcwAashtoBase*/, size_t /*sizeBdcwAashtoBase*/, BOOL & /*bVersionOK*/ , _T("Print_CrackCheck"))
typedef BOOL (PSC_Print_FlexureDesign)(UINT /*iElemKey*/, UINT /*iIJ*/, BOOL /*bCompatibility*/, CStringW& , void* /*pBdcyAashtoBase*/, size_t /*sizeBdcyAashtoBase*/, BOOL & /*bVersionOK*/);
//typedef BOOL (PSC_Print_ShearDesign, UINT /*iElemKey*/, UINT /*iIJ*/, void* /*pCumsAashtoBase*/, size_t /*sizeCumsAashtoBase*/, BOOL & /*bVersionOK*/ , _T("Print_ShearDesign"))
//typedef BOOL (PSC_Print_TorsionDesign, UINT /*iElemKey*/, UINT /*iIJ*/, void* /*pCrmsAashtoBase*/, size_t /*sizeCrmsAashtoBase*/, BOOL & /*bVersionOK*/ , _T("Print_TorsionDesign"))
																			 

typedef BOOL (PSC_Print_AASHTO_Detail)(int /*iDgnCode*/, UINT, UINT, void* , size_t, void* , size_t, BOOL&);
typedef BOOL (PSC_Print_AASHTO_Detail_Dgn)(int /*iDgnCode*/, UINT, UINT, BOOL, CStringW&, void* , size_t, void* , size_t, BOOL&);
typedef BOOL (PSC_Print_AASHTO_Detail_Alw)(int /*iDgnCode*/, UINT, UINT, BOOL, CStringW,void* , size_t, void* , size_t, BOOL&, BOOL);                                     
																		 
class CDgn_PSCRptManager : public CDgn_DllManager_Base
{
public:
	CDgn_PSCRptManager();
	virtual ~CDgn_PSCRptManager();

protected:
	PSC_InitializeNewReport*    m_pInitNewReport;
	PSC_TerminateCurrentReport* m_pTerminateReport;
	PSC_Print_Result2* m_pAlwStressCS;
	PSC_Print_Result_Comp* m_pAlwStressCS_Comp;
 
	PSC_Print_Result2* m_pAlwStressServComb;
	PSC_Print_Result_Comp* m_pAlwStressServComb_Comp;

	PSC_Print_Result2* m_pAlwPrincipalStressCS;
	PSC_Print_Result2* m_pAlwPrincipalStressShear;
	PSC_Print_Result2* m_pAlwPrincipalStressTorsion;
	PSC_Print_Result_Tendon* m_pTendon;  
	PSC_Print_Result* m_pCrack;
	PSC_Print_FlexureDesign* m_pFlexure;
	PSC_Print_Result* m_pShear;
	PSC_Print_Result* m_pTorsion;

	PSC_Print_AASHTO_Detail* m_pDgnCondition; 
	PSC_Print_AASHTO_Detail_Dgn* m_pDgnFlexure; 
	PSC_Print_AASHTO_Detail_Dgn* m_pDgnCrack; 
	PSC_Print_AASHTO_Detail_Dgn* m_pDgnShear; 
	PSC_Print_AASHTO_Detail_Dgn* m_pDgnTorsion; 
	PSC_Print_AASHTO_Detail_Alw* m_pDgnFysg;
	PSC_Print_AASHTO_Detail_Alw* m_pDgnFycm;
	
public:
	BOOL InitialData(int iNationType);
	BOOL Initialize(int iDgnCode, int iRptUnit, LPCTSTR szProgramDir, LPCTSTR szProjectFileDir, LPCTSTR szSaveFileName);
	BOOL Terminate(int iDgnCode, BOOL bIsAborted, int nType);
	BOOL Print_AllowableStresses_Conc_ConstructionStage(int iDgnCode, ElemPairK iElemKey, UINT iIJ, bool bFinal, const T_FYSG_AASHTO_BASE & fysgBase);
	BOOL Print_AllowableStresses_Conc_ConstructionStage_Comp(int iDgnCode, ElemPairK iElemKey, UINT iIJ, bool bFinal, const T_FYSG_AASHTO_BASE & fysgBase, BOOL bGirder);
	BOOL Print_AllowableStresses_Conc_ServiceLoadCombination(int iDgnCode, ElemPairK iElemKey, UINT iIJ, bool bFinal, const T_FYCM_AASHTO_BASE & fycmBase);
	BOOL Print_AllowableStresses_Conc_ServiceLoadCombination_Comp(int iDgnCode, ElemPairK iElemKey, UINT iIJ, bool bFinal, const T_FYCM_AASHTO_BASE & fycmBase, BOOL bGirder);
	BOOL Print_AllowablePrincipalStress_Conc_ConstructionStage(int iDgnCode, ElemPairK iElemKey, UINT iIJ, bool bFinal, const T_FPMT_JTG_BASE & fpmtBase);
	BOOL Print_AllowablePrincipalStress_Conc_ServiceLoadCombination_MaxShear(int iDgnCode, ElemPairK iElemKey, UINT iIJ, bool bFinal, const T_FPMS_JTG_BASE & fpmtBase);
	BOOL Print_AllowablePrincipalStress_Conc_ServiceLoadCombination_MaxTorsion(int iDgnCode, ElemPairK iElemKey, UINT iIJ, bool bFinal, const T_FPMS_JTG_BASE & fpmtBase);
	BOOL Print_AllowableStress_Tendon(int iDgnCode, UINT iTndn, bool bFinal, const T_FYPC_AASHTO & fypc);
	BOOL Print_TensionRebarRequired(ElemPairK iElemKey, UINT iIJ, const T_TBAR_AASHTO_BASE & tbarBase);
	BOOL Print_CrackCheck(ElemPairK iElemKey, UINT iIJ, const T_BDCW_AASHTO_BASE & bdcwBase);
	BOOL Print_FlexureDesign(ElemPairK iElemKey, UINT iIJ, BOOL bCompatibility, const T_BDCY_AASHTO_BASE & bdcwBase);
	BOOL Print_ShearDesign(ElemPairK iElemKey, UINT iIJ, const T_CUMS_AASHTO_BASE & bdcwBase);
	BOOL Print_TorsionDesign(ElemPairK iElemKey, UINT iIJ, const T_CRMT_AASHTO_BASE & crmtBase);

	BOOL Print_Detail_Condition    (int iDgnCode, ElemPairK iElemKey, UINT iIJ, PSC_DATA_POSI& InData, _BDCY_PSC_LRFD_BASE& EngBdcyD);
	BOOL Print_Detail_FlexureDesign(int iDgnCode, ElemPairK iElemKey, UINT iIJ, BOOL bFirst, PSC_DATA_POSI& InData, _BDCY_PSC_LRFD_BASE& EngBdcyD);
	BOOL Print_Detail_CrackCheck   (int iDgnCode, ElemPairK ElemKey, UINT iIJ, BOOL bFirst, PSC_DATA_POSI& InData, _BDCW_PSC_LRFD_BASE & bdcwBase);
	BOOL Print_Detail_ShearDesign  (int iDgnCode, ElemPairK ElemKey, UINT iIJ, BOOL bFirst, PSC_DATA_POSI& InData, _CUMS_AASHTO_BASE & bdcwBase);
	BOOL Print_Detail_TorsionDesign(int iDgnCode, ElemPairK ElemKey, UINT iIJ, BOOL bFirst, PSC_DATA_POSI& InData, _CRMS_AASHTO_BASE & bdcwBase);
	BOOL Print_Detail_Allowable_Comp_Fysg(int iDgnCode, ElemPairK ElemKey, UINT iIJ, BOOL bFirst, PSC_DATA_POSI& InData, _FYSG_PSC_LRFD_BASE & dgnFysgBase, BOOL bGirder);
	BOOL Print_Detail_Allowable_Comp_Fycm(int iDgnCode, ElemPairK ElemKey, UINT iIJ, BOOL bFirst, PSC_DATA_POSI& InData, _FYCM_PSC_LRFD_BASE & dgnFycmBase, BOOL bGirder);

protected:
    CString Get_StageName(T_STAG_K StagK);
    CString Get_LcomName(T_LCOM_K LcomK, BOOL bIsOrgLcomK=FALSE);

private:
	CAASHTOPSCRptDllModule m_AASHTOReportModule;
	CDgn_PSCManager m_pscManager;
	
	BOOL m_bInitialized;
};

#endif // !defined(__DGN_PSCRPTMANAGER_H__)
