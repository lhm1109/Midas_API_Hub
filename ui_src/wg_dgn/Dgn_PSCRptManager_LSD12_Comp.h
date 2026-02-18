// Dgn_PSCRptManager_LSD12_Comp.h: interface for the CDgn_PSCRptManager_LSD12_Comp class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_Dgn_PSCRptManager_LSD12_Comp_H__INCLUDED_)
#define AFX_Dgn_PSCRptManager_LSD12_Comp_H__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Dgn_DllManager_Base.h"

#include "..\dgnengine\src\DgnReportBase\IExcel.h"
#include "..\dgnengine\src\DgnReportBase\ExcelGenerator.h"
#include "..\dgnengine\src\DgnReportBase\MExcel_MS.h"


typedef BOOL (PSC_InitializeNewReport_LSD12_Comp)(void*, size_t, const BOOL & /*bRegression*/, BOOL&);
typedef BOOL (PSC_InitializeNewReport_LSD12_Rating)(const int /*iDgncode*/,const IExcel* /*pExcel*/, const CStringW & /*strBasePath*/, const CStringW & /*strDataPath*/, const CStringW & /*strPath*/, const CStringW & /*strPath*/, const BOOL & /*bRegression*/);
typedef BOOL (PSC_TerminateCurrentReport_LSD12_Comp)(const int /*iDgncode*/, BOOL /*bIsAborted*/, BOOL /*bRegression*/);
typedef BOOL (PSC_Print_Result_LSD12_Comp )(const int /*iDgncode*/, UINT /*iElemKey*/, UINT /*iIJ*/, BOOL /*bFinal*/, void* /*pFysgAashtoBase*/, size_t /*sizeFysgAashtobase*/, BOOL & /*bVersionOK*/ );
typedef BOOL (PSC_Print_Result_LSD12_Comp2)(const int /*iDgncode*/, UINT /*iElemKey*/, UINT /*iIJ*/, BOOL /*bFinal*/, BOOL /*bGirder*/, void* /*pFysgAashtoBase*/, size_t /*sizeFysgAashtobase*/, BOOL & /*bVersionOK*/ );
typedef BOOL (PSC_Print_Result_Tendon_LSD12_Comp)(const int /*iDgncode*/, UINT /*iTndn*/, BOOL /*bFinal*/, void* /*pFpmtAashto*/, size_t /*sizeFypcAashto*/, BOOL & /*bVersionOK*/);
typedef BOOL (PSC_Print_Detail_Design_LSD12_Comp)(const int /*iDgncode*/, UINT /*iElemKey*/, UINT /*iIJ*/, void* /*pFysgAashtoBase*/, size_t /*sizeFysgAashtobase*/, void* /*pFysgAashtoBase*/, size_t /*sizeFysgAashtobase*/, BOOL & /*bVersionOK*/ );

struct PSC_FYSG_BASE;
struct PSC_FYCM_BASE;
struct PSC_FPSG_BASE;
struct PSC_FPMS_BASE;
struct PSC_FYPC_D;
struct PSC_CUMS_BASE;
struct PSC_CRMT_BASE;
struct PSC_BDCW_BASE;
struct PSC_BDCW_BASE;
struct PSC_CUMS_BASE;
struct PSC_CRMT_BASE;
struct PSC_BDCW_BASE;

class CDgn_PSCRptManager_LSD12_Comp : public CDgn_DllManager_Base       
{
public:
	CDgn_PSCRptManager_LSD12_Comp();
	virtual ~CDgn_PSCRptManager_LSD12_Comp();
	
protected:
	PSC_InitializeNewReport_LSD12_Comp*     m_pInitNewReport_LSD12_Comp;
	PSC_InitializeNewReport_LSD12_Rating*   m_pInitNewReport_LSD12_Rating;
	PSC_TerminateCurrentReport_LSD12_Comp*  m_pTerminateReport_LSD12_Comp;
	PSC_Print_Result_LSD12_Comp2*            m_pAlwStressCS_LSD12_Comp;
	PSC_Print_Result_LSD12_Comp2*            m_pAlwStressServ_LSD12_Comp;
	PSC_Print_Result_LSD12_Comp2*            m_pAlwPrincipalStressCS_LSD12_Comp;
	PSC_Print_Result_LSD12_Comp2*            m_pAlwPrincipalStressServ_LSD12_Comp;
	PSC_Print_Result_Tendon_LSD12_Comp*     m_pTendon_LSD12_Comp;
	PSC_Print_Result_LSD12_Comp*            m_pCrack_LSD12_Comp;
	PSC_Print_Result_LSD12_Comp*            m_pFlexure_LSD12_Comp;
	PSC_Print_Result_LSD12_Comp*            m_pShear_LSD12_Comp;
	PSC_Print_Result_LSD12_Comp*            m_pTorsion_LSD12_Comp;
	
	PSC_Print_Detail_Design_LSD12_Comp*     m_pDgnCondition;
	PSC_Print_Detail_Design_LSD12_Comp*     m_pFlexureDetail;
	PSC_Print_Detail_Design_LSD12_Comp*     m_pShearDetail;
	PSC_Print_Detail_Design_LSD12_Comp*     m_pTorsionDetail;
	PSC_Print_Detail_Design_LSD12_Comp*     m_pCrackDetail;
	PSC_Print_Detail_Design_LSD12_Comp*     m_pZeroStressDetail;
	
	
public:
	BOOL InitialData(int iNationType);
	BOOL Initialize(int iDgnCode, LPCTSTR szProgramDir, LPCTSTR szProjectFileDir, LPCTSTR szSaveFileName);
	BOOL Initialize_Rating(int iDgnCode, IExcel* pExcel,
												 LPCTSTR szProgramDir, 
												 LPCTSTR szProjectFileDir, 
												 LPCTSTR szSaveFileName, 
												 LPCTSTR szSaveSheetName);
	BOOL Terminate(int iDgnCode, BOOL bIsAborted);
	
	// RESULT-TABLE 
	BOOL Print_AllowableStresses_Conc_ConstructionStage_LSD12_Comp       (int iDgnCode, ElemPairK iElemKey, UINT iIJ, BOOL bFinal, BOOL bGirder, PSC_FYSG_BASE& FysgBase);
	BOOL Print_AllowableStresses_Conc_ServiceLoad_LSD12_Comp             (int iDgnCode, ElemPairK iElemKey, UINT iIJ, BOOL bFinal, BOOL bGirder, PSC_FYCM_BASE& FycmBase);
	BOOL Print_AllowablePrincipalStress_Conc_ConstructionStage_LSD12_Comp(int iDgnCode, ElemPairK iElemKey, UINT iIJ, BOOL bFinal, BOOL bGirder, PSC_FPSG_BASE& FpsgBase);
	BOOL Print_AllowablePrincipalStress_Conc_ServiceLoad_LSD12_Comp      (int iDgnCode, ElemPairK iElemKey, UINT iIJ, BOOL bFinal, BOOL bGirder, PSC_FPMS_BASE& FpmsBase);
	BOOL Print_AllowableStress_Tendon_LSD12_Comp                         (int iDgnCode, UINT iTndn, BOOL bFinal, PSC_FYPC_D& Fypc);

	// DETAIL-REPORT
	BOOL Print_DetailReport_DgnCondition (int iDgnCode, ElemPairK iElemKey, UINT iIJ, PSC_DATA_POSI& InData, PSC_FRCR_D& FrcrD);
	BOOL Print_DetailReport_FlexureDesign(int iDgnCode, ElemPairK iElemKey, UINT iIJ, PSC_DATA_POSI& InData, PSC_FRCR_D& FrcrD);
	BOOL Print_DetailReport_ShearDesign  (int iDgnCode, ElemPairK iElemKey, UINT iIJ, PSC_DATA_POSI& InData, PSC_CUMS_BASE& CumsBase);
	BOOL Print_DetailReport_TorsionDesign(int iDgnCode, ElemPairK iElemKey, UINT iIJ, PSC_DATA_POSI& InData, PSC_CRMT_BASE& CumsBase);
	BOOL Print_DetailReport_CrackDesign  (int iDgnCode, ElemPairK iElemKey, UINT iIJ, PSC_DATA_POSI& InData, PSC_BDCW_BASE& BdcwBase);
	BOOL Print_DetailReport_ZeroStressDesign  (int iDgnCode, ElemPairK iElemKey, UINT iIJ, PSC_DATA_POSI& InData, PSC_BDCW_BASE& BdcwBase);

	// SUMMART-REPROT
	BOOL Print_FlexureDesign_LSD12_Comp(int iDgnCode, ElemPairK iElemKey, UINT iIJ, BOOL bFinal, PSC_FRCR_D& BdcwBase);
	BOOL Print_ShearDesign_LSD12_Comp  (int iDgnCode, ElemPairK iElemKey, UINT iIJ, BOOL bFinal, PSC_CUMS_BASE& CumsBase);
	BOOL Print_TorsionDesign_LSD12_Comp(int iDgnCode, ElemPairK iElemKey, UINT iIJ, BOOL bFinal, PSC_CRMT_BASE& CrmtBase);
	BOOL Print_CrackCheck_LSD12_Comp   (int iDgnCode, ElemPairK iElemKey, UINT iIJ, BOOL bFinal, PSC_BDCW_BASE& BdcwBase);

	
private:
	//CEUROCODEPSCRptDllModule m_EurocodeReportModule;
	//CDgn_PSCManager m_pscManager;
	
	BOOL m_bInitialized;
	
	CString Get_StageName(T_STAG_K StagK);
	CString Get_LcomName(T_LCOM_K LcomK);

};
#endif // !defined(AFX_Dgn_PSCRptManager_LSD12_Comp_H__INCLUDED_)