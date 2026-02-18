// Dgn_PSCRptManager_EC2.cpp: implementation of the CDgn_PSCRptManager_IRC112 class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "..\DgnEngine\Src\DgnCalcBase\DgnCalcBase_PSC_Struct.h"
#include "..\DgnEngine\Src\DgnCalcBase\DgnCalcBase_PSC_ResultStruct.h"
#include "..\DgnEngine\Src\CVL_DgnCalc_US\DgnCalc_US_PSC_Struct.h"
#include "Dgn_PSCRptManager_IRC112.h"

//Dgn_PSCRptManager.cpp 유용.
#include "DgnEurocode2_2_05.h"
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

CDgn_PSCRptManager_IRC112::CDgn_PSCRptManager_IRC112()
	: m_bInitialized(FALSE)
{
	m_pInitNewReport_EC          = NULL;
	m_pTerminateReport_EC        = NULL;
	//
	m_pFlexure_EC                = NULL;
	m_pShear_EC                  = NULL;
	m_pTorsion_EC                = NULL;
	//
	m_pAlwStressCS_EC            = NULL;
	m_pAlwStressServ_EC          = NULL;
	m_pAlwPrincipalStressCS_EC   = NULL;
	m_pAlwPrincipalStressServ_EC = NULL;
	m_pTendon_EC                 = NULL;  
	m_pCrack_EC                  = NULL;
	//
	m_pDgnCondition              = NULL;
	m_pFlexureDetail             = NULL;
	m_pShearDetail               = NULL;
	m_pTorsionDetail             = NULL;
	m_pCrackDetail               = NULL;
	m_pZerostress                = NULL;
	m_pFatigDetail               = NULL;
}

CDgn_PSCRptManager_IRC112::~CDgn_PSCRptManager_IRC112()
{

}

BOOL CDgn_PSCRptManager_IRC112::InitialData(int iNationType)
{
	HINSTANCE hDllCalc = GetOrLoad_ReportDllHandle(iNationType);
	if(hDllCalc == NULL) return FALSE;

	if(iNationType != DGNENGINE_US_DLL)
	{
		m_pInitNewReport_EC          = NULL;
		m_pTerminateReport_EC        = NULL;
		//
		m_pFlexure_EC                = NULL;
		m_pShear_EC                  = NULL;
		m_pTorsion_EC                = NULL;	    
		//
		m_pAlwStressCS_EC            = NULL;
		m_pAlwStressServ_EC          = NULL;
		m_pAlwPrincipalStressCS_EC   = NULL;
		m_pAlwPrincipalStressServ_EC = NULL;
		m_pTendon_EC                 = NULL;  
		m_pCrack_EC                  = NULL;
		//
		m_pDgnCondition              = NULL;
		m_pFlexureDetail             = NULL;
		m_pShearDetail               = NULL;
		m_pTorsionDetail             = NULL;
		m_pCrackDetail               = NULL;
		m_pZerostress                = NULL;
		m_pFatigDetail               = NULL;
		m_pAlwStressCS_EC_Detail     = NULL;
		m_pAlwStressSL_EC_Detail     = NULL;
	}
	else 
	{	
		m_pInitNewReport_EC = (PSC_InitializeNewReport_EC*)GetProcAddress(hDllCalc, "InitializeNewReport_EC");
		if(!m_pInitNewReport_EC) return FALSE;
		//    
		m_pTerminateReport_EC = (PSC_TerminateCurrentReport_EC*)GetProcAddress(hDllCalc, "TerminateCurrentReport_EC");
		if(!m_pTerminateReport_EC) return FALSE;
		//
		m_pFlexure_EC = (PSC_Print_Result_EC*)GetProcAddress(hDllCalc, "Print_FlexureDesign_EC");
		if(!m_pFlexure_EC) return FALSE;
		//
		m_pShear_EC = (PSC_Print_Result_EC*)GetProcAddress(hDllCalc, "Print_ShearDesign_EC");
		if(!m_pShear_EC) return FALSE;
		//
		m_pTorsion_EC = (PSC_Print_Result_EC*)GetProcAddress(hDllCalc, "Print_TorsionDesign_EC");
		if(!m_pTorsion_EC) return FALSE;
		//
		m_pAlwStressCS_EC = (PSC_Print_Result_EC*)GetProcAddress(hDllCalc, "Print_AllowableStresses_Conc_ConstructionStage_EC");
		if(!m_pAlwStressCS_EC) return FALSE;
		//
		m_pAlwStressServ_EC = (PSC_Print_Result_EC*)GetProcAddress(hDllCalc, "Print_AllowableStresses_Conc_ServiceLoad_EC");
		if(!m_pAlwStressServ_EC) return FALSE;
		//
		m_pAlwPrincipalStressCS_EC = (PSC_Print_Result_EC*)GetProcAddress(hDllCalc, "Print_AllowablePrincipalStress_Conc_ConstructionStage_EC");
		if(!m_pAlwPrincipalStressCS_EC) return FALSE;
		//
		m_pAlwPrincipalStressServ_EC = (PSC_Print_Result_EC*)GetProcAddress(hDllCalc, "Print_AllowablePrincipalStress_Conc_ServiceLoad_EC");
		if(!m_pAlwPrincipalStressServ_EC) return FALSE;
		//
		m_pTendon_EC = (PSC_Print_Result_Tendon_EC*)GetProcAddress(hDllCalc, "Print_AllowableStress_Tendon_EC");
		if(!m_pTendon_EC) return FALSE;
		//
		m_pCrack_EC = (PSC_Print_Result_EC*)GetProcAddress(hDllCalc, "Print_CrackCheck_EC");
		if(!m_pCrack_EC) return FALSE;

		// DETAIL-REPORT
		//
		m_pDgnCondition = (PSC_Print_Detail_Design_EC*)GetProcAddress(hDllCalc, "Print_DetailReport_DgnCondition");
		if(!m_pDgnCondition) return FALSE;
		//
		m_pFlexureDetail = (PSC_Print_Detail_Design_EC*)GetProcAddress(hDllCalc, "Print_DetailReport_FlexureDesign");
		if(!m_pFlexureDetail) return FALSE;
		//
		m_pShearDetail = (PSC_Print_Detail_Design_EC*)GetProcAddress(hDllCalc, "Print_DetailReport_ShearDesign");
		if(!m_pShearDetail) return FALSE;
		//
		m_pTorsionDetail = (PSC_Print_Detail_Design_EC*)GetProcAddress(hDllCalc, "Print_DetailReport_TorsionDesign");
		if(!m_pTorsionDetail) return FALSE;
		//
		m_pCrackDetail = (PSC_Print_Detail_Design_EC*)GetProcAddress(hDllCalc, "Print_DetailReport_CrackDesign");
		if(!m_pCrackDetail) return FALSE;
		//
		m_pZerostress = (PSC_Print_Detail_Design_EC*)GetProcAddress(hDllCalc, "Print_DetailReport_Zerostress");
		if(!m_pZerostress) return FALSE;
		//
		m_pFatigDetail = (PSC_Print_Detail_Design_EC*)GetProcAddress(hDllCalc, "Print_DetailReport_Fatigue");
		if(!m_pFatigDetail) return FALSE;
		//
		m_pAlwStressCS_EC_Detail = (PSC_Print_Detail_Design_EC*)GetProcAddress(hDllCalc, "Print_DetailReport_AlwStressCS_EC");
		if(!m_pAlwStressCS_EC_Detail) return FALSE;
		//
		m_pAlwStressSL_EC_Detail = (PSC_Print_Detail_Design_EC*)GetProcAddress(hDllCalc, "Print_DetailReport_AlwStressSL_EC");
		if(!m_pAlwStressSL_EC_Detail) return FALSE;

	}

	return TRUE;
}

BOOL CDgn_PSCRptManager_IRC112::Initialize(int iDgncode, LPCTSTR szProgramDir, LPCTSTR szProjectFileDir, LPCTSTR szSaveFileName)
{ 
	VERIFY(SUCCEEDED(OleInitialize(NULL)));

	if(!InitialData(DGNENGINE_US_DLL)) return FALSE;

	CDgnPscCommon PscCommon; 
	int nDgnCode = PscCommon.GetConvertCodeCvlToEng( iDgncode);
	BOOL bRegression = CLockMgr::Get_IsRegressionTest();

	BOOL bVersionOK = FALSE;

	D_EXCEL_PRT_OPTION ROpt;
	GetRptOptionData(szProgramDir, szProjectFileDir, szSaveFileName, nDgnCode, ROpt);

	BOOL bOK = (*m_pInitNewReport_EC)(&ROpt, sizeof(D_EXCEL_PRT_OPTION), bRegression, bVersionOK);  
	if(!bOK || !bVersionOK) return FALSE;

	m_bInitialized = TRUE;
	return TRUE;
}

BOOL CDgn_PSCRptManager_IRC112::Terminate(int iDgnCode, BOOL bIsAborted)
{
	CDgnPscCommon PscCommon; 
	int nDgnCode = PscCommon.GetConvertCodeCvlToEng( iDgnCode);
	BOOL bRegression = CLockMgr::Get_IsRegressionTest(); // Regression Test인지 확인.

	BOOL bTerminated = (*m_pTerminateReport_EC)(nDgnCode, bIsAborted, bRegression);		

	return bTerminated;
}

BOOL CDgn_PSCRptManager_IRC112::Print_AllowableStresses_Conc_ConstructionStage_EC(int iDgnCode, ElemPairK iElemKey, UINT iIJ, BOOL bFinal, PSC_FYSG_BASE& FysgBase)
{
	if(!m_bInitialized) return FALSE;

	BOOL bVersionOK = FALSE;
	BOOL bCheck = FALSE;

	CDgnPscCommon PscCommon; 
	int nDgnCode = PscCommon.GetConvertCodeCvlToEng( iDgnCode);

	bCheck = (*m_pAlwStressCS_EC)(nDgnCode, iElemKey.first, iIJ, bFinal, &FysgBase, sizeof(PSC_FYSG_BASE), bVersionOK);

	return (bVersionOK && bCheck);
}

BOOL CDgn_PSCRptManager_IRC112::Print_AllowableStresses_Conc_ServiceLoad_EC(int iDgnCode, ElemPairK iElemKey, UINT iIJ, BOOL bFinal, PSC_FYCM_BASE& FycmBase)
{
	if(!m_bInitialized) return FALSE;

	BOOL bVersionOK = FALSE;
	BOOL bCheck = FALSE;

	CDgnPscCommon PscCommon; 
	int nDgnCode = PscCommon.GetConvertCodeCvlToEng( iDgnCode);

	bCheck = (*m_pAlwStressServ_EC)(nDgnCode, iElemKey.first, iIJ, bFinal, &FycmBase, sizeof(PSC_FYCM_BASE), bVersionOK);

	return (bVersionOK && bCheck);
}

BOOL CDgn_PSCRptManager_IRC112::Print_AllowablePrincipalStress_Conc_ConstructionStage_EC(int iDgnCode, ElemPairK iElemKey, UINT iIJ, BOOL bFinal, PSC_FPSG_BASE& FpsgBase)
{
	if(!m_bInitialized) return FALSE;	

	BOOL bVersionOK = FALSE;
	BOOL bCheck = FALSE;

	CDgnPscCommon PscCommon; 
	int nDgnCode = PscCommon.GetConvertCodeCvlToEng( iDgnCode);

	bCheck = (*m_pAlwPrincipalStressCS_EC)(nDgnCode, iElemKey.first, iIJ, bFinal, &FpsgBase, sizeof(PSC_FPSG_BASE), bVersionOK);

	return (bVersionOK && bCheck);
}

BOOL CDgn_PSCRptManager_IRC112::Print_AllowablePrincipalStress_Conc_ServiceLoad_EC(int iDgnCode, ElemPairK iElemKey, UINT iIJ, BOOL bFinal, PSC_FPMS_BASE& FpmsBase)
{
	if(!m_bInitialized) return FALSE;

	BOOL bVersionOK = FALSE;
	BOOL bCheck = FALSE;

	CDgnPscCommon PscCommon; 
	int nDgnCode = PscCommon.GetConvertCodeCvlToEng( iDgnCode);

	bCheck = (*m_pAlwPrincipalStressServ_EC)(nDgnCode, iElemKey.first, iIJ, bFinal, &FpmsBase, sizeof(PSC_FPMS_BASE), bVersionOK);

	return (bVersionOK && bCheck);
}

BOOL CDgn_PSCRptManager_IRC112::Print_AllowableStress_Tendon_EC(int iDgnCode, UINT iTndn, BOOL bFinal, PSC_FYPC_D& FypcBase)
{
	if(!m_bInitialized) return FALSE;

	BOOL bVersionOK = FALSE;
	BOOL bCheck = FALSE;

	CDgnPscCommon PscCommon; 
	int nDgnCode = PscCommon.GetConvertCodeCvlToEng( iDgnCode);

	bCheck = (*m_pTendon_EC)(nDgnCode, iTndn, bFinal, &FypcBase, sizeof(PSC_FYPC_D), bVersionOK);

	return (bVersionOK && bCheck);
}

// Summary Report!
BOOL CDgn_PSCRptManager_IRC112::Print_FlexureDesign_EC(int iDgnCode, ElemPairK iElemKey, UINT iIJ, BOOL bFinal, PSC_FRCR_D& BdcwBase)
{
	if(!m_bInitialized) return FALSE;

	BOOL bVersionOK = FALSE;
	BOOL bCheck = FALSE;

	CDgnPscCommon PscCommon; 
	int nDgnCode = PscCommon.GetConvertCodeCvlToEng( iDgnCode);

	bCheck = (*m_pFlexure_EC)(nDgnCode, iElemKey.first, iIJ, bFinal, &BdcwBase, sizeof(PSC_FRCR_D), bVersionOK);

	return (bVersionOK && bCheck);
}

BOOL CDgn_PSCRptManager_IRC112::Print_ShearDesign_EC(int iDgnCode, ElemPairK iElemKey, UINT iIJ, BOOL bFinal, PSC_CUMS_BASE& CumsBase)
{
	if(!m_bInitialized) return FALSE;

	BOOL bVersionOK = FALSE;
	BOOL bCheck = FALSE;

	CDgnPscCommon PscCommon; 
	int nDgnCode = PscCommon.GetConvertCodeCvlToEng( iDgnCode);

	bCheck = (*m_pShear_EC)(nDgnCode, iElemKey.first, iIJ, bFinal, &CumsBase, sizeof(PSC_CUMS_BASE), bVersionOK);

	return (bVersionOK && bCheck);
}

BOOL CDgn_PSCRptManager_IRC112::Print_TorsionDesign_EC(int iDgnCode, ElemPairK iElemKey, UINT iIJ, BOOL bFinal, PSC_CRMT_BASE& CrmtBase)
{
	if(!m_bInitialized) return FALSE;

	BOOL bVersionOK = FALSE;
	BOOL bCheck = FALSE;

	CDgnPscCommon PscCommon; 
	int nDgnCode = PscCommon.GetConvertCodeCvlToEng( iDgnCode);

	bCheck = (*m_pTorsion_EC)(nDgnCode, iElemKey.first, iIJ, bFinal, &CrmtBase, sizeof(PSC_CRMT_BASE), bVersionOK);

	return (bVersionOK && bCheck);
}

BOOL CDgn_PSCRptManager_IRC112::Print_CrackCheck_EC(int iDgnCode, ElemPairK iElemKey, UINT iIJ, BOOL bFinal, PSC_BDCW_BASE& BdcwBase)
{
	if(!m_bInitialized) return FALSE;

	BOOL bVersionOK = FALSE;
	BOOL bCheck = FALSE;

	CDgnPscCommon PscCommon; 
	int nDgnCode = PscCommon.GetConvertCodeCvlToEng( iDgnCode);

	bCheck = (*m_pCrack_EC)(nDgnCode, iElemKey.first, iIJ, bFinal, &BdcwBase, sizeof(PSC_BDCW_BASE), bVersionOK);

	return (bVersionOK && bCheck);
}

// DETAIL-REPORT!!

BOOL CDgn_PSCRptManager_IRC112::Print_DetailReport_DgnCondition(int iDgnCode, ElemPairK iElemKey, UINT iIJ, PSC_DATA_POSI& InData, PSC_FRCR_D& FrcrD)
{
	if(!m_bInitialized) return FALSE;

	BOOL bVersionOK = FALSE;
	BOOL bCheck = FALSE;

	CDgnPscCommon PscCommon; 
	int nDgnCode = PscCommon.GetConvertCodeCvlToEng( iDgnCode);

	bCheck = (*m_pDgnCondition)(nDgnCode, iElemKey.first, iIJ, &InData, sizeof(PSC_DATA_POSI), &FrcrD, sizeof(PSC_FRCR_D), bVersionOK);

	return (bVersionOK && bCheck);
}

BOOL CDgn_PSCRptManager_IRC112::Print_DetailReport_FlexureDesign(int iDgnCode, ElemPairK iElemKey, UINT iIJ, PSC_DATA_POSI& InData, PSC_FRCR_D& FrcrD)
{
	if(!m_bInitialized) return FALSE;

	BOOL bVersionOK = FALSE;
	BOOL bCheck = FALSE;

	CDgnPscCommon PscCommon; 
	int nDgnCode = PscCommon.GetConvertCodeCvlToEng( iDgnCode);

	bCheck = (*m_pFlexureDetail)(nDgnCode, iElemKey.first, iIJ, &InData, sizeof(PSC_DATA_POSI), &FrcrD, sizeof(PSC_FRCR_D), bVersionOK);

	return (bVersionOK && bCheck);
}

BOOL CDgn_PSCRptManager_IRC112::Print_DetailReport_ShearDesign(int iDgnCode, ElemPairK iElemKey, UINT iIJ, PSC_DATA_POSI& InData, PSC_CUMS_BASE& CumsBase)
{
	if(!m_bInitialized) return FALSE;

	BOOL bVersionOK = FALSE;
	BOOL bCheck = FALSE;

	CDgnPscCommon PscCommon; 
	int nDgnCode = PscCommon.GetConvertCodeCvlToEng( iDgnCode);

	bCheck = (*m_pShearDetail)(nDgnCode, iElemKey.first, iIJ, &InData, sizeof(PSC_DATA_POSI), &CumsBase, sizeof(PSC_CUMS_BASE), bVersionOK);

	return (bVersionOK && bCheck);
}

BOOL CDgn_PSCRptManager_IRC112::Print_DetailReport_TorsionDesign(int iDgnCode, ElemPairK iElemKey, UINT iIJ, PSC_DATA_POSI& InData, PSC_CRMT_BASE& CrmtBase)
{
	if(!m_bInitialized) return FALSE;

	BOOL bVersionOK = FALSE;
	BOOL bCheck = FALSE;

	CDgnPscCommon PscCommon; 
	int nDgnCode = PscCommon.GetConvertCodeCvlToEng( iDgnCode);

	bCheck = (*m_pTorsionDetail)(nDgnCode, iElemKey.first, iIJ, &InData, sizeof(PSC_DATA_POSI), &CrmtBase, sizeof(PSC_CRMT_BASE), bVersionOK);

	return (bVersionOK && bCheck);
}


BOOL CDgn_PSCRptManager_IRC112::Print_DetailReport_ZeroStressDesign(int iDgnCode, ElemPairK iElemKey, UINT iIJ, PSC_DATA_POSI& InData, PSC_BDCW_BASE& BdcwBase)
{
	if(!m_bInitialized) return FALSE;

	BOOL bVersionOK = FALSE;
	BOOL bCheck = FALSE;

	CDgnPscCommon PscCommon; 
	int nDgnCode = PscCommon.GetConvertCodeCvlToEng( iDgnCode);

	bCheck = (*m_pZerostress)(nDgnCode, iElemKey.first, iIJ, &InData, sizeof(PSC_DATA_POSI), &BdcwBase, sizeof(PSC_BDCW_BASE), bVersionOK);

	return (bVersionOK && bCheck);
}


BOOL CDgn_PSCRptManager_IRC112::Print_DetailReport_CrackDesign(int iDgnCode, ElemPairK iElemKey, UINT iIJ, PSC_DATA_POSI& InData, PSC_BDCW_BASE& BdcwBase)
{
	if(!m_bInitialized) return FALSE;

	BOOL bVersionOK = FALSE;
	BOOL bCheck = FALSE;

	CDgnPscCommon PscCommon; 
	int nDgnCode = PscCommon.GetConvertCodeCvlToEng( iDgnCode);

	bCheck = (*m_pCrackDetail)(nDgnCode, iElemKey.first, iIJ, &InData, sizeof(PSC_DATA_POSI), &BdcwBase, sizeof(PSC_BDCW_BASE), bVersionOK);

	return (bVersionOK && bCheck);
}

BOOL CDgn_PSCRptManager_IRC112::Print_DetailReport_FatigueDesign(int iDgnCode, ElemPairK iElemKey, UINT iIJ, PSC_DATA_POSI& InData, PSC_FATG_BASE& FatgBase)
{
	if(!m_bInitialized) return FALSE;

	BOOL bVersionOK = FALSE;
	BOOL bCheck = FALSE;

	CDgnPscCommon PscCommon; 
	int nDgnCode = PscCommon.GetConvertCodeCvlToEng( iDgnCode);

	bCheck = (*m_pFatigDetail)(nDgnCode, iElemKey.first, iIJ, &InData, sizeof(PSC_DATA_POSI), &FatgBase, sizeof(PSC_FATG_BASE), bVersionOK);

	return (bVersionOK && bCheck);
} 



CString CDgn_PSCRptManager_IRC112::Get_StageName(T_STAG_K StagK)
{
	CString strStageName=_T("");
	T_STAG_D StagD;
	if (m_pDoc->m_pAttrCtrl->GetStag(StagK, StagD)) strStageName = StagD.StageName;

	return strStageName;
}

CString CDgn_PSCRptManager_IRC112::Get_LcomName(T_LCOM_K LcomK)
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

BOOL CDgn_PSCRptManager_IRC112::Print_DetailReport_AllowableStresses_Fysg(int iDgnCode, ElemPairK iElemKey, UINT iIJ, PSC_DATA_POSI& InData, PSC_FYSG_BASE& FysgBase)
{
	if(!m_bInitialized) return FALSE;

	BOOL bVersionOK = FALSE;
	BOOL bCheck = FALSE;

	CDgnPscCommon PscCommon; 
	int nDgnCode = PscCommon.GetConvertCodeCvlToEng( iDgnCode);

	bCheck = (*m_pAlwStressCS_EC_Detail)(nDgnCode, iElemKey.first, iIJ, &InData, sizeof(PSC_DATA_POSI), &FysgBase, sizeof(PSC_FYSG_BASE), bVersionOK);

	return (bVersionOK && bCheck);
}

BOOL CDgn_PSCRptManager_IRC112::Print_DetailReport_AllowableStresses_Fycm(int iDgnCode, ElemPairK iElemKey, UINT iIJ, PSC_DATA_POSI& InData, PSC_FYCM_BASE& FycmBase)
{
	if(!m_bInitialized) return FALSE;

	BOOL bVersionOK = FALSE;
	BOOL bCheck = FALSE;

	CDgnPscCommon PscCommon; 
	int nDgnCode = PscCommon.GetConvertCodeCvlToEng( iDgnCode);

	bCheck = (*m_pAlwStressSL_EC_Detail)(nDgnCode, iElemKey.first, iIJ, &InData, sizeof(PSC_DATA_POSI), &FycmBase, sizeof(PSC_FYCM_BASE), bVersionOK);

	return (bVersionOK && bCheck);
}

