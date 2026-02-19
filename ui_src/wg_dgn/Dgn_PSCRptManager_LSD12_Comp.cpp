// Dgn_PSCRptManager_LSD12_Comp.cpp: implementation of the CDgn_PSCRptManager_LSD12_Comp class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "..\DgnEngine\Src\DgnCalcBase\DgnCalcBase_PSC_Struct.h"
#include "..\DgnEngine\Src\DgnCalcBase\DgnCalcBase_PSC_ResultStruct.h"
#include "Dgn_PSCRptManager_LSD12_Comp.h"

//Dgn_PSCRptManager.cpp À¯¿ë.
//#include "DgnEurocode2_2_05.h"
#include "Dgn_PSCManager.h"

#include "..\wg_db\AttrCtrl.h"
#include "..\wg_db\PostCtrl.h"
#include "..\wg_db\PCDesign.h"

#include "DgnEngineProgressDlg.h"

#include "..\dgnengine\src\DgnReportBase\XLStruct.h"
#include "..\wg_dbLock\LockMgr.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDgn_PSCRptManager_LSD12_Comp::CDgn_PSCRptManager_LSD12_Comp()
: m_bInitialized(FALSE)
{
	m_pInitNewReport_LSD12_Comp          = NULL;
	m_pInitNewReport_LSD12_Rating        = NULL;
	m_pTerminateReport_LSD12_Comp        = NULL;
	//
	m_pFlexure_LSD12_Comp                = NULL;
	m_pShear_LSD12_Comp                  = NULL;
	m_pTorsion_LSD12_Comp                = NULL;
	//
	m_pAlwStressCS_LSD12_Comp            = NULL;
	m_pAlwStressServ_LSD12_Comp          = NULL;
	m_pAlwPrincipalStressCS_LSD12_Comp   = NULL;
	m_pAlwPrincipalStressServ_LSD12_Comp = NULL;
	m_pTendon_LSD12_Comp                 = NULL;  
	m_pCrack_LSD12_Comp                  = NULL;
	//
	m_pDgnCondition              = NULL;
	m_pFlexureDetail             = NULL;
	m_pShearDetail               = NULL;
	m_pTorsionDetail             = NULL;
	m_pCrackDetail               = NULL;
	m_pZeroStressDetail          = NULL;
}

CDgn_PSCRptManager_LSD12_Comp::~CDgn_PSCRptManager_LSD12_Comp()
{

}

BOOL CDgn_PSCRptManager_LSD12_Comp::InitialData(int iNationType)
{
	HINSTANCE hDllCalc = GetOrLoad_ReportDllHandle(iNationType);
	if(hDllCalc == NULL) return FALSE;

	if(iNationType != DGNENGINE_US_DLL)
	{
		m_pInitNewReport_LSD12_Comp          = NULL;
		m_pInitNewReport_LSD12_Rating        = NULL;
		m_pTerminateReport_LSD12_Comp        = NULL;
		//
		m_pFlexure_LSD12_Comp                = NULL;
		m_pShear_LSD12_Comp                  = NULL;
		m_pTorsion_LSD12_Comp                = NULL;	    
		//
		m_pAlwStressCS_LSD12_Comp            = NULL;
		m_pAlwStressServ_LSD12_Comp          = NULL;
		m_pAlwPrincipalStressCS_LSD12_Comp   = NULL;
		m_pAlwPrincipalStressServ_LSD12_Comp = NULL;
		m_pTendon_LSD12_Comp                 = NULL;  
		m_pCrack_LSD12_Comp                  = NULL;
		//
		m_pDgnCondition              = NULL;
		m_pFlexureDetail             = NULL;
		m_pShearDetail               = NULL;
		m_pTorsionDetail             = NULL;
		m_pCrackDetail               = NULL;
		m_pZeroStressDetail          = NULL;

	}
	else 
	{	
		m_pInitNewReport_LSD12_Comp = (PSC_InitializeNewReport_LSD12_Comp*)GetProcAddress(hDllCalc, "InitializeNewReport_Compo");
		if(!m_pInitNewReport_LSD12_Comp) return FALSE;
		//
		m_pInitNewReport_LSD12_Rating = (PSC_InitializeNewReport_LSD12_Rating*)GetProcAddress(hDllCalc, "InitializeNewReport_Compo_Rating");
		if(!m_pInitNewReport_LSD12_Rating) return FALSE;
		//
		m_pTerminateReport_LSD12_Comp = (PSC_TerminateCurrentReport_LSD12_Comp*)GetProcAddress(hDllCalc, "TerminateCurrentReport_Compo");
		if(!m_pTerminateReport_LSD12_Comp) return FALSE;
		//
		m_pFlexure_LSD12_Comp = (PSC_Print_Result_LSD12_Comp*)GetProcAddress(hDllCalc, "Print_FlexureDesign_Compo");
		if(!m_pFlexure_LSD12_Comp) return FALSE;
		//
		m_pShear_LSD12_Comp = (PSC_Print_Result_LSD12_Comp*)GetProcAddress(hDllCalc, "Print_ShearDesign_Compo");
		if(!m_pShear_LSD12_Comp) return FALSE;
		//
		m_pTorsion_LSD12_Comp = (PSC_Print_Result_LSD12_Comp*)GetProcAddress(hDllCalc, "Print_TorsionDesign_Compo");
		if(!m_pTorsion_LSD12_Comp) return FALSE;
		//
		m_pAlwStressCS_LSD12_Comp = (PSC_Print_Result_LSD12_Comp2*)GetProcAddress(hDllCalc, "Print_AllowableStresses_Conc_ConstructionStage_Compo");
		if(!m_pAlwStressCS_LSD12_Comp) return FALSE;
		//
		m_pAlwStressServ_LSD12_Comp = (PSC_Print_Result_LSD12_Comp2*)GetProcAddress(hDllCalc, "Print_AllowableStresses_Conc_ServiceLoad_Compo");
		if(!m_pAlwStressServ_LSD12_Comp) return FALSE;
		//
		m_pAlwPrincipalStressCS_LSD12_Comp = (PSC_Print_Result_LSD12_Comp2*)GetProcAddress(hDllCalc, "Print_AllowablePrincipalStress_Conc_ConstructionStage_Compo");
		if(!m_pAlwPrincipalStressCS_LSD12_Comp) return FALSE;
		//
		m_pAlwPrincipalStressServ_LSD12_Comp = (PSC_Print_Result_LSD12_Comp2*)GetProcAddress(hDllCalc, "Print_AllowablePrincipalStress_Conc_ServiceLoad_Compo");
		if(!m_pAlwPrincipalStressServ_LSD12_Comp) return FALSE;
		//
		m_pTendon_LSD12_Comp = (PSC_Print_Result_Tendon_LSD12_Comp*)GetProcAddress(hDllCalc, "Print_AllowableStress_Tendon_Compo");
		if(!m_pTendon_LSD12_Comp) return FALSE;
		//
		m_pCrack_LSD12_Comp = (PSC_Print_Result_LSD12_Comp*)GetProcAddress(hDllCalc, "Print_CrackCheck_Compo");
		if(!m_pCrack_LSD12_Comp) return FALSE;

		// DETAIL-REPORT
		//
		m_pDgnCondition = (PSC_Print_Detail_Design_LSD12_Comp*)GetProcAddress(hDllCalc, "Print_DetailReport_DgnCondition_Compo");
		if(!m_pDgnCondition) return FALSE;
		//
		m_pFlexureDetail = (PSC_Print_Detail_Design_LSD12_Comp*)GetProcAddress(hDllCalc, "Print_DetailReport_FlexureDesign_Compo");
		if(!m_pFlexureDetail) return FALSE;
		//
		m_pShearDetail = (PSC_Print_Detail_Design_LSD12_Comp*)GetProcAddress(hDllCalc, "Print_DetailReport_ShearDesign_Compo");
		if(!m_pShearDetail) return FALSE;
		//
		m_pTorsionDetail = (PSC_Print_Detail_Design_LSD12_Comp*)GetProcAddress(hDllCalc, "Print_DetailReport_TorsionDesign_Compo");
		if(!m_pTorsionDetail) return FALSE;
		//
		m_pCrackDetail = (PSC_Print_Detail_Design_LSD12_Comp*)GetProcAddress(hDllCalc, "Print_DetailReport_CrackDesign_Compo");
		if(!m_pCrackDetail) return FALSE;
		//
		m_pZeroStressDetail = (PSC_Print_Detail_Design_LSD12_Comp*)GetProcAddress(hDllCalc, "Print_DetailReport_ZeroStressDesign_Compo");
		if(!m_pZeroStressDetail) return FALSE;

	}

	return TRUE;
}

BOOL CDgn_PSCRptManager_LSD12_Comp::Initialize(int iDgncode, LPCTSTR szProgramDir, LPCTSTR szProjectFileDir, LPCTSTR szSaveFileName)
{ 
	VERIFY(SUCCEEDED(OleInitialize(NULL)));

	if(!InitialData(DGNENGINE_US_DLL)) return FALSE;

	CDgnPscCommon PscCommon; 
	int nDgnCode = PscCommon.GetConvertCodeCvlToEng( iDgncode);
	BOOL bRegression = CLockMgr::Get_IsRegressionTest();

	BOOL bVersionOK = FALSE;

	D_EXCEL_PRT_OPTION ROpt;
	GetRptOptionData(szProgramDir, szProjectFileDir, szSaveFileName, nDgnCode, ROpt);

	BOOL bOK = (*m_pInitNewReport_LSD12_Comp)(&ROpt, sizeof(D_EXCEL_PRT_OPTION), bRegression, bVersionOK);  
	if(!bOK || !bVersionOK) return FALSE;

	m_bInitialized = TRUE;
	return TRUE;
}

BOOL CDgn_PSCRptManager_LSD12_Comp::Initialize_Rating(int iDgncode, IExcel* pExcel,
													LPCTSTR szProgramDir, 
													LPCTSTR szProjectFileDir, 
													LPCTSTR szSaveFileName, 
													LPCTSTR szSaveSheetName)
{ 
	VERIFY(SUCCEEDED(OleInitialize(NULL)));

	if(!InitialData(DGNENGINE_US_DLL)) return FALSE;

	CDgnPscCommon PscCommon; 
	int nDgnCode = PscCommon.GetConvertCodeCvlToEng( iDgncode);
	BOOL bRegression = CLockMgr::Get_IsRegressionTest();

	CStringW strWProgramDir(szProgramDir);
	CStringW strWProjectFileDir(szProjectFileDir);
	CStringW strWSaveFileName(szSaveFileName);
	CStringW strWSaveSheetName(szSaveSheetName);

	BOOL bOK = (*m_pInitNewReport_LSD12_Rating)(nDgnCode, pExcel, strWProgramDir, strWProjectFileDir, strWSaveFileName, strWSaveSheetName, bRegression);  
	if(!bOK) return FALSE;

	m_bInitialized = TRUE;
	return TRUE;
}

BOOL CDgn_PSCRptManager_LSD12_Comp::Terminate(int iDgnCode, BOOL bIsAborted)
{
	CDgnPscCommon PscCommon; 
	int nDgnCode = PscCommon.GetConvertCodeCvlToEng( iDgnCode);
	BOOL bRegression = CLockMgr::Get_IsRegressionTest();

	BOOL bTerminated = (*m_pTerminateReport_LSD12_Comp)(nDgnCode, bIsAborted, bRegression);		
	
	return bTerminated;
}

BOOL CDgn_PSCRptManager_LSD12_Comp::Print_AllowableStresses_Conc_ConstructionStage_LSD12_Comp(int iDgnCode, ElemPairK iElemKey, UINT iIJ, BOOL bFinal, BOOL bGirder, PSC_FYSG_BASE& FysgBase)
{
	if(!m_bInitialized) return FALSE;

	BOOL bVersionOK = FALSE;
	BOOL bCheck = FALSE;
	
	CDgnPscCommon PscCommon; 
	int nDgnCode = PscCommon.GetConvertCodeCvlToEng( iDgnCode);

	bCheck = (*m_pAlwStressCS_LSD12_Comp)(nDgnCode, iElemKey.first, iIJ, bFinal, bGirder, &FysgBase, sizeof(PSC_FYSG_BASE), bVersionOK);

	return (bVersionOK && bCheck);
}

BOOL CDgn_PSCRptManager_LSD12_Comp::Print_AllowableStresses_Conc_ServiceLoad_LSD12_Comp(int iDgnCode, ElemPairK iElemKey, UINT iIJ, BOOL bFinal, BOOL bGirder, PSC_FYCM_BASE& FycmBase)
{
	if(!m_bInitialized) return FALSE;

	BOOL bVersionOK = FALSE;
	BOOL bCheck = FALSE;
	
	CDgnPscCommon PscCommon; 
	int nDgnCode = PscCommon.GetConvertCodeCvlToEng( iDgnCode);

	bCheck = (*m_pAlwStressServ_LSD12_Comp)(nDgnCode, iElemKey.first, iIJ, bFinal, bGirder, &FycmBase, sizeof(PSC_FYCM_BASE), bVersionOK);

	return (bVersionOK && bCheck);
}

BOOL CDgn_PSCRptManager_LSD12_Comp::Print_AllowablePrincipalStress_Conc_ConstructionStage_LSD12_Comp(int iDgnCode, ElemPairK iElemKey, UINT iIJ, BOOL bFinal, BOOL bGirder, PSC_FPSG_BASE& FpsgBase)
{
	if(!m_bInitialized) return FALSE;	

	BOOL bVersionOK = FALSE;
	BOOL bCheck = FALSE;
	
	CDgnPscCommon PscCommon; 
	int nDgnCode = PscCommon.GetConvertCodeCvlToEng( iDgnCode);

	bCheck = (*m_pAlwPrincipalStressCS_LSD12_Comp)(nDgnCode, iElemKey.first, iIJ, bFinal, bGirder, &FpsgBase, sizeof(PSC_FPSG_BASE), bVersionOK);

	return (bVersionOK && bCheck);
}

BOOL CDgn_PSCRptManager_LSD12_Comp::Print_AllowablePrincipalStress_Conc_ServiceLoad_LSD12_Comp(int iDgnCode, ElemPairK iElemKey, UINT iIJ, BOOL bFinal, BOOL bGirder, PSC_FPMS_BASE& FpmsBase)
{
	if(!m_bInitialized) return FALSE;

	BOOL bVersionOK = FALSE;
	BOOL bCheck = FALSE;
	
	CDgnPscCommon PscCommon; 
	int nDgnCode = PscCommon.GetConvertCodeCvlToEng( iDgnCode);

	bCheck = (*m_pAlwPrincipalStressServ_LSD12_Comp)(nDgnCode, iElemKey.first, iIJ, bFinal, bGirder, &FpmsBase, sizeof(PSC_FPMS_BASE), bVersionOK);

	return (bVersionOK && bCheck);
}

BOOL CDgn_PSCRptManager_LSD12_Comp::Print_AllowableStress_Tendon_LSD12_Comp(int iDgnCode, UINT iTndn, BOOL bFinal, PSC_FYPC_D& FypcBase)
{
	if(!m_bInitialized) return FALSE;

	BOOL bVersionOK = FALSE;
	BOOL bCheck = FALSE;

	CDgnPscCommon PscCommon; 
	int nDgnCode = PscCommon.GetConvertCodeCvlToEng( iDgnCode);

	bCheck = (*m_pTendon_LSD12_Comp)(nDgnCode, iTndn, bFinal, &FypcBase, sizeof(PSC_FYPC_D), bVersionOK);

	return (bVersionOK && bCheck);
}

// Summary Report!
BOOL CDgn_PSCRptManager_LSD12_Comp::Print_FlexureDesign_LSD12_Comp(int iDgnCode, ElemPairK iElemKey, UINT iIJ, BOOL bFinal, PSC_FRCR_D& BdcwBase)
{
	if(!m_bInitialized) return FALSE;

	BOOL bVersionOK = FALSE;
	BOOL bCheck = FALSE;

	CDgnPscCommon PscCommon; 
	int nDgnCode = PscCommon.GetConvertCodeCvlToEng( iDgnCode);

	bCheck = (*m_pFlexure_LSD12_Comp)(nDgnCode, iElemKey.first, iIJ, bFinal, &BdcwBase, sizeof(PSC_FRCR_D), bVersionOK);

	return (bVersionOK && bCheck);
}

BOOL CDgn_PSCRptManager_LSD12_Comp::Print_ShearDesign_LSD12_Comp(int iDgnCode, ElemPairK iElemKey, UINT iIJ, BOOL bFinal, PSC_CUMS_BASE& CumsBase)
{
	if(!m_bInitialized) return FALSE;

	BOOL bVersionOK = FALSE;
	BOOL bCheck = FALSE;

	CDgnPscCommon PscCommon; 
	int nDgnCode = PscCommon.GetConvertCodeCvlToEng( iDgnCode);

	bCheck = (*m_pShear_LSD12_Comp)(nDgnCode, iElemKey.first, iIJ, bFinal, &CumsBase, sizeof(PSC_CUMS_BASE), bVersionOK);

	return (bVersionOK && bCheck);
}

BOOL CDgn_PSCRptManager_LSD12_Comp::Print_TorsionDesign_LSD12_Comp(int iDgnCode, ElemPairK iElemKey, UINT iIJ, BOOL bFinal, PSC_CRMT_BASE& CrmtBase)
{
	if(!m_bInitialized) return FALSE;

	BOOL bVersionOK = FALSE;
	BOOL bCheck = FALSE;

	CDgnPscCommon PscCommon; 
	int nDgnCode = PscCommon.GetConvertCodeCvlToEng( iDgnCode);

	bCheck = (*m_pTorsion_LSD12_Comp)(nDgnCode, iElemKey.first, iIJ, bFinal, &CrmtBase, sizeof(PSC_CRMT_BASE), bVersionOK);

	return (bVersionOK && bCheck);
}

BOOL CDgn_PSCRptManager_LSD12_Comp::Print_CrackCheck_LSD12_Comp(int iDgnCode, ElemPairK iElemKey, UINT iIJ, BOOL bFinal, PSC_BDCW_BASE& BdcwBase)
{
	if(!m_bInitialized) return FALSE;

	BOOL bVersionOK = FALSE;
	BOOL bCheck = FALSE;

	CDgnPscCommon PscCommon; 
	int nDgnCode = PscCommon.GetConvertCodeCvlToEng( iDgnCode);

	bCheck = (*m_pCrack_LSD12_Comp)(nDgnCode, iElemKey.first, iIJ, bFinal, &BdcwBase, sizeof(PSC_BDCW_BASE), bVersionOK);

	return (bVersionOK && bCheck);
}

// DETAIL-REPORT!!

BOOL CDgn_PSCRptManager_LSD12_Comp::Print_DetailReport_DgnCondition(int iDgnCode, ElemPairK iElemKey, UINT iIJ, PSC_DATA_POSI& InData, PSC_FRCR_D& FrcrD)
{
	if(!m_bInitialized) return FALSE;

	BOOL bVersionOK = FALSE;
	BOOL bCheck = FALSE;

	CDgnPscCommon PscCommon; 
	int nDgnCode = PscCommon.GetConvertCodeCvlToEng( iDgnCode);

	bCheck = (*m_pDgnCondition)(nDgnCode, iElemKey.first, iIJ, &InData, sizeof(PSC_DATA_POSI), &FrcrD, sizeof(PSC_FRCR_D), bVersionOK);

	return (bVersionOK && bCheck);
}

BOOL CDgn_PSCRptManager_LSD12_Comp::Print_DetailReport_FlexureDesign(int iDgnCode, ElemPairK iElemKey, UINT iIJ, PSC_DATA_POSI& InData, PSC_FRCR_D& FrcrD)
{
	if(!m_bInitialized) return FALSE;

	BOOL bVersionOK = FALSE;
	BOOL bCheck = FALSE;

	CDgnPscCommon PscCommon; 
	int nDgnCode = PscCommon.GetConvertCodeCvlToEng( iDgnCode);

	bCheck = (*m_pFlexureDetail)(nDgnCode, iElemKey.first, iIJ, &InData, sizeof(PSC_DATA_POSI), &FrcrD, sizeof(PSC_FRCR_D), bVersionOK);

	return (bVersionOK && bCheck);
}

BOOL CDgn_PSCRptManager_LSD12_Comp::Print_DetailReport_ShearDesign(int iDgnCode, ElemPairK iElemKey, UINT iIJ, PSC_DATA_POSI& InData, PSC_CUMS_BASE& CumsBase)
{
	if(!m_bInitialized) return FALSE;

	BOOL bVersionOK = FALSE;
	BOOL bCheck = FALSE;

	CDgnPscCommon PscCommon; 
	int nDgnCode = PscCommon.GetConvertCodeCvlToEng( iDgnCode);

	bCheck = (*m_pShearDetail)(nDgnCode, iElemKey.first, iIJ, &InData, sizeof(PSC_DATA_POSI), &CumsBase, sizeof(PSC_CUMS_BASE), bVersionOK);

	return (bVersionOK && bCheck);
}

BOOL CDgn_PSCRptManager_LSD12_Comp::Print_DetailReport_TorsionDesign(int iDgnCode, ElemPairK iElemKey, UINT iIJ, PSC_DATA_POSI& InData, PSC_CRMT_BASE& CrmtBase)
{
	if(!m_bInitialized) return FALSE;

	BOOL bVersionOK = FALSE;
	BOOL bCheck = FALSE;

	CDgnPscCommon PscCommon; 
	int nDgnCode = PscCommon.GetConvertCodeCvlToEng( iDgnCode);

	bCheck = (*m_pTorsionDetail)(nDgnCode, iElemKey.first, iIJ, &InData, sizeof(PSC_DATA_POSI), &CrmtBase, sizeof(PSC_CRMT_BASE), bVersionOK);

	return (bVersionOK && bCheck);
}

BOOL CDgn_PSCRptManager_LSD12_Comp::Print_DetailReport_CrackDesign(int iDgnCode, ElemPairK iElemKey, UINT iIJ, PSC_DATA_POSI& InData, PSC_BDCW_BASE& BdcwBase)
{
	if(!m_bInitialized) return FALSE;

	BOOL bVersionOK = FALSE;
	BOOL bCheck = FALSE;

	CDgnPscCommon PscCommon; 
	int nDgnCode = PscCommon.GetConvertCodeCvlToEng( iDgnCode);

	bCheck = (*m_pCrackDetail)(nDgnCode, iElemKey.first, iIJ, &InData, sizeof(PSC_DATA_POSI), &BdcwBase, sizeof(PSC_BDCW_BASE), bVersionOK);

	return (bVersionOK && bCheck);
}

BOOL CDgn_PSCRptManager_LSD12_Comp::Print_DetailReport_ZeroStressDesign(int iDgnCode, ElemPairK iElemKey, UINT iIJ, PSC_DATA_POSI& InData, PSC_BDCW_BASE& BdcwBase)
{
	if(!m_bInitialized) return FALSE;
	
	BOOL bVersionOK = FALSE;
	BOOL bCheck = FALSE;
	
	CDgnPscCommon PscCommon; 
	int nDgnCode = PscCommon.GetConvertCodeCvlToEng( iDgnCode);
	
	bCheck = (*m_pZeroStressDetail)(nDgnCode, iElemKey.first, iIJ, &InData, sizeof(PSC_DATA_POSI), &BdcwBase, sizeof(PSC_BDCW_BASE), bVersionOK);
	
	return (bVersionOK && bCheck);
}


CString CDgn_PSCRptManager_LSD12_Comp::Get_StageName(T_STAG_K StagK)
{
	CString strStageName=_T("");
	T_STAG_D StagD;
	if (m_pDoc->m_pAttrCtrl->GetStag(StagK, StagD)) strStageName = StagD.StageName;

	return strStageName;
}

CString CDgn_PSCRptManager_LSD12_Comp::Get_LcomName(T_LCOM_K LcomK)
{
	CString strLcomName=_T("");
	T_LCOM_K OrgLcomK = 0;
	T_LCOM_D LcomData;
	if (m_pDoc->m_pPostCtrl->GetPCDesign()->GetOrgLcomK(LcomK, OrgLcomK))
	{
		if(m_pDoc->m_pAttrCtrl->GetLcom(D_LCOMTYPE_CONCRETE, OrgLcomK, LcomData)) strLcomName = LcomData.LoadCombName;    
	}  

	return strLcomName;
}

