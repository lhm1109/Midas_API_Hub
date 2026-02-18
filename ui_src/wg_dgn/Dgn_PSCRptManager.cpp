// Dgn_PSCRptManager.cpp: implementation of the Dgn_PSCRptManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "..\DgnEngine\Src\DgnCalcBase\DgnCalcBase_PSC_Struct.h"
#include "..\DgnEngine\Src\DgnCalcBase\DgnCalcBase_PSC_ResultStruct.h"
#include "..\DgnEngine\Src\CVL_DgnCalc_US\DgnCalc_US_PSC_Struct.h"
#include "Dgn_PSCRptManager.h"
#include "Dgn_PSCManager.h"

#include "..\wg_db\AttrCtrl.h"
#include "..\wg_db\PostCtrl.h"
#include "..\wg_db\PCDesign.h"

#include "..\dgnengine\src\DgnReportBase\XLStruct.h"
#include "..\wg_dbLock\LockMgr.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDgn_PSCRptManager::CDgn_PSCRptManager()
: m_bInitialized(FALSE)
{
	m_pInitNewReport = NULL;
	m_pTerminateReport = NULL;
	m_pAlwStressCS = NULL;
	m_pAlwStressCS_Comp = NULL;
	m_pAlwStressServComb = NULL;
	m_pAlwStressServComb_Comp = NULL;
	m_pAlwPrincipalStressCS = NULL;
	m_pAlwPrincipalStressShear = NULL;
	m_pAlwPrincipalStressTorsion = NULL;
	m_pTendon = NULL;  
	m_pCrack = NULL;
	m_pFlexure = NULL;
	m_pShear = NULL;
	m_pTorsion = NULL;	
	m_pDgnCondition= NULL;
	m_pDgnFlexure= NULL; 
	m_pDgnCrack = NULL;
	m_pDgnShear= NULL; 
	m_pDgnTorsion = NULL;
 
}

CDgn_PSCRptManager::~CDgn_PSCRptManager()
{
}

BOOL CDgn_PSCRptManager::InitialData(int iNationType)
{
	HINSTANCE hDllCalc = GetOrLoad_ReportDllHandle(iNationType);
	if(hDllCalc == NULL) return FALSE;

	if(iNationType != DGNENGINE_US_DLL)
	{
		m_pInitNewReport = NULL;
		m_pTerminateReport = NULL;
		m_pAlwStressCS = NULL;
		m_pAlwStressCS_Comp = NULL;
		m_pAlwStressServComb = NULL;
		m_pAlwStressServComb_Comp = NULL;
		m_pAlwPrincipalStressCS = NULL;
		m_pAlwPrincipalStressShear = NULL;
		m_pAlwPrincipalStressTorsion = NULL;
		m_pTendon = NULL;  
		m_pCrack = NULL;
		m_pFlexure = NULL;
		m_pShear = NULL;
		m_pTorsion = NULL;	 
		m_pDgnCondition= NULL;
		m_pDgnFlexure= NULL; 
		m_pDgnCrack = NULL;
		m_pDgnShear= NULL; 
		m_pDgnTorsion = NULL;

	}
	else 
	{	
		m_pInitNewReport = (PSC_InitializeNewReport*)GetProcAddress(hDllCalc, "InitializeNewReport"); 
		if(!m_pInitNewReport) return FALSE;
		//    
		m_pTerminateReport = (PSC_TerminateCurrentReport*)GetProcAddress(hDllCalc, "TerminateCurrentReport"); 
		if(!m_pTerminateReport) return FALSE;
		//
		m_pAlwStressCS = (PSC_Print_Result2*)GetProcAddress(hDllCalc, "Print_AllowableStresses_Conc_ConstructionStage"); 
		if(!m_pAlwStressCS) return FALSE;
		//
		m_pAlwStressCS_Comp = (PSC_Print_Result_Comp*)GetProcAddress(hDllCalc, "Print_AllowableStresses_Conc_ConstructionStage_Comp"); 
		if(!m_pAlwStressCS_Comp) return FALSE;
		//
		m_pAlwStressServComb = (PSC_Print_Result2*)GetProcAddress(hDllCalc, "Print_AllowableStresses_Conc_ServiceLoadCombination"); 
		if(!m_pAlwStressServComb) return FALSE;
		//
		m_pAlwStressServComb_Comp = (PSC_Print_Result_Comp*)GetProcAddress(hDllCalc, "Print_AllowableStresses_Conc_ServiceLoadCombination_Comp"); 
		if(!m_pAlwStressServComb_Comp) return FALSE;
		//
		m_pAlwPrincipalStressCS = (PSC_Print_Result2*)GetProcAddress(hDllCalc, "Print_AllowablePrincipalStress_Conc_ConstructionStage"); 
		if(!m_pAlwPrincipalStressCS) return FALSE;
		//
		m_pAlwPrincipalStressShear = (PSC_Print_Result2*)GetProcAddress(hDllCalc, "Print_AllowablePrincipalStress_Conc_ServiceLoadCombination_MaxShear"); 
		if(!m_pAlwPrincipalStressShear) return FALSE;
		//
		m_pAlwPrincipalStressTorsion = (PSC_Print_Result2*)GetProcAddress(hDllCalc, "Print_AllowablePrincipalStress_Conc_ServiceLoadCombination_MaxTorsion"); 
		if(!m_pAlwPrincipalStressTorsion) return FALSE;
		//
		m_pTendon = (PSC_Print_Result_Tendon*)GetProcAddress(hDllCalc, "Print_AllowableStress_Tendon"); 
		if(!m_pTendon) return FALSE;
		//
		m_pCrack = (PSC_Print_Result*)GetProcAddress(hDllCalc, "Print_CrackCheck"); 
		if(!m_pCrack) return FALSE;
		//
		m_pFlexure = (PSC_Print_FlexureDesign*)GetProcAddress(hDllCalc, "Print_FlexureDesign"); 
		if(!m_pFlexure) return FALSE;
		//
		m_pShear = (PSC_Print_Result*)GetProcAddress(hDllCalc, "Print_ShearDesign"); 
		if(!m_pShear) return FALSE;
		//
		m_pTorsion = (PSC_Print_Result*)GetProcAddress(hDllCalc, "Print_TorsionDesign"); 
		if(!m_pTorsion) return FALSE;

		// Detail
		//
		m_pDgnCondition = (PSC_Print_AASHTO_Detail*)GetProcAddress(hDllCalc, "Print_Detail_Condition"); 
		if(!m_pDgnCondition) return FALSE;

		m_pDgnFlexure = (PSC_Print_AASHTO_Detail_Dgn*)GetProcAddress(hDllCalc, "Print_Detail_FlexureDesign"); 
		if(!m_pDgnFlexure) return FALSE;
		
		m_pDgnCrack = (PSC_Print_AASHTO_Detail_Dgn*)GetProcAddress(hDllCalc, "Print_Detail_CrackCheck"); 
		if(!m_pDgnCrack) return FALSE;

		m_pDgnShear = (PSC_Print_AASHTO_Detail_Dgn*)GetProcAddress(hDllCalc, "Print_Detail_ShearDesign"); 
		if(!m_pDgnShear) return FALSE;

		m_pDgnTorsion = (PSC_Print_AASHTO_Detail_Dgn*)GetProcAddress(hDllCalc, "Print_Detail_TorsionDesign"); 
		if(!m_pDgnTorsion) return FALSE;

		m_pDgnFysg = (PSC_Print_AASHTO_Detail_Alw*)GetProcAddress(hDllCalc, "Print_Detail_Allowable_Comp_Fysg"); 
		if(!m_pDgnFysg) return FALSE;

		m_pDgnFycm = (PSC_Print_AASHTO_Detail_Alw*)GetProcAddress(hDllCalc, "Print_Detail_Allowable_Comp_Fycm"); 
		if(!m_pDgnFycm) return FALSE;


	}

	return TRUE;
}

BOOL CDgn_PSCRptManager::Initialize(int iDgnCode, int iRptUnit, LPCTSTR szProgramDir, LPCTSTR szProjectFileDir, LPCTSTR szSaveFileName)
{ 
	VERIFY(SUCCEEDED(OleInitialize(NULL))); 

	if(!InitialData(DGNENGINE_US_DLL)) return FALSE;

	CDgnPscCommon PscCommon; 
	int nDgnCode = PscCommon.GetConvertCodeCvlToEng( iDgnCode);
	BOOL bRegression = CLockMgr::Get_IsRegressionTest();

	BOOL bVersionOK = FALSE;
	
	D_EXCEL_PRT_OPTION ROpt;
	GetRptOptionData(szProgramDir, szProjectFileDir, szSaveFileName, nDgnCode, ROpt);
    ROpt.nSelUnit = iRptUnit;

	BOOL bOK = (*m_pInitNewReport)(&ROpt, sizeof(D_EXCEL_PRT_OPTION), bRegression, bVersionOK);  
	if(!bOK || !bVersionOK) return FALSE;

	m_bInitialized = TRUE;


	return TRUE;
}

BOOL CDgn_PSCRptManager::Terminate(int iDgnCode, BOOL bIsAborted, int nType)
{
	CDgnPscCommon PscCommon; 
	int nDgnCode = PscCommon.GetConvertCodeCvlToEng( iDgnCode);

	BOOL bRegression = CLockMgr::Get_IsRegressionTest();
	
	BOOL bTerminated = (*m_pTerminateReport)(nDgnCode, bIsAborted, bRegression, nType);		
	
	return bTerminated;
}

BOOL CDgn_PSCRptManager::Print_AllowableStresses_Conc_ConstructionStage(int iDgnCode, ElemPairK iElemKey, UINT iIJ, bool bFinal, const T_FYSG_AASHTO_BASE & fysgBase)
{
	if(!m_bInitialized) return FALSE;
	
	_FYSG_PSC_LRFD_BASE dgnFysgBase;
	dgnFysgBase.Initialize();
	m_pscManager.ConvertFysg_CivilToEngineAASHTO2(fysgBase, dgnFysgBase);

	CString strStageName = Get_StageName(fysgBase.StagK);
	CStringW strWStageName(strStageName);

	BOOL bVersionOK = FALSE;
	BOOL bCheck = FALSE;
	
	CDgnPscCommon PscCommon; 
	int nDgnCode = PscCommon.GetConvertCodeCvlToEng( iDgnCode);
	
	bCheck = (*m_pAlwStressCS)(nDgnCode, iElemKey.first, iIJ, bFinal, strWStageName, &dgnFysgBase, sizeof(_FYSG_PSC_LRFD_BASE), bVersionOK);

	return (bVersionOK && bCheck);
}

BOOL CDgn_PSCRptManager::Print_AllowableStresses_Conc_ConstructionStage_Comp(int iDgnCode, ElemPairK iElemKey, UINT iIJ, bool bFinal, const T_FYSG_AASHTO_BASE & fysgBase, BOOL bGirder)
{
	if(!m_bInitialized) return FALSE;

	_FYSG_PSC_LRFD_BASE dgnFysgBase;
	dgnFysgBase.Initialize();
	m_pscManager.ConvertFysg_CivilToEngineAASHTO2(fysgBase, dgnFysgBase);

	CString strStageName = Get_StageName(fysgBase.StagK);
	CStringW strWStageName(strStageName);

	BOOL bVersionOK = FALSE;
	BOOL bCheck = FALSE;

	CDgnPscCommon PscCommon; 
	int nDgnCode = PscCommon.GetConvertCodeCvlToEng( iDgnCode);

	bCheck = (*m_pAlwStressCS_Comp)(nDgnCode, iElemKey.first, iIJ, bFinal, strWStageName, &dgnFysgBase, sizeof(_FYSG_PSC_LRFD_BASE), bVersionOK, bGirder);

	return (bVersionOK && bCheck);
}

BOOL CDgn_PSCRptManager::Print_AllowableStresses_Conc_ServiceLoadCombination(int iDgnCode, ElemPairK iElemKey, UINT iIJ, bool bFinal, const T_FYCM_AASHTO_BASE & fycmBase)
{
	if(!m_bInitialized) return FALSE;
	
	_FYCM_PSC_LRFD_BASE dgnFycmBase;
	dgnFycmBase.Initialize();
	m_pscManager.ConvertFycm_CivilToEngineAASHTO(fycmBase, dgnFycmBase);

	CString strLcomName = Get_LcomName(fycmBase.LcomK);
	CStringW strWLcomName(strLcomName);
	
	BOOL bVersionOK = FALSE;
	BOOL bCheck = FALSE;

	CDgnPscCommon PscCommon; 
	int nDgnCode = PscCommon.GetConvertCodeCvlToEng( iDgnCode);
	
	bCheck = (*m_pAlwStressServComb)(nDgnCode, iElemKey.first, iIJ, bFinal, strWLcomName, &dgnFycmBase, sizeof(_FYCM_PSC_LRFD_BASE), bVersionOK);

	return (bVersionOK && bCheck);
}

BOOL CDgn_PSCRptManager::Print_AllowableStresses_Conc_ServiceLoadCombination_Comp(int iDgnCode, ElemPairK iElemKey, UINT iIJ, bool bFinal, const T_FYCM_AASHTO_BASE & fycmBase, BOOL bGirder)
{
	if(!m_bInitialized) return FALSE;

	_FYCM_PSC_LRFD_BASE dgnFycmBase;
	dgnFycmBase.Initialize();
	m_pscManager.ConvertFycm_CivilToEngineAASHTO(fycmBase, dgnFycmBase);

	CString strLcomName = Get_LcomName(fycmBase.LcomK);
	CStringW strWLcomName(strLcomName);

	BOOL bVersionOK = FALSE;
	BOOL bCheck = FALSE;

	CDgnPscCommon PscCommon; 
	int nDgnCode = PscCommon.GetConvertCodeCvlToEng( iDgnCode);

	bCheck = (*m_pAlwStressServComb_Comp)(nDgnCode, iElemKey.first, iIJ, bFinal, strWLcomName, &dgnFycmBase, sizeof(_FYCM_PSC_LRFD_BASE), bVersionOK, bGirder);

	return (bVersionOK && bCheck);
}

BOOL CDgn_PSCRptManager::Print_AllowablePrincipalStress_Conc_ConstructionStage(int iDgnCode, ElemPairK iElemKey, UINT iIJ, bool bFinal, const T_FPMT_JTG_BASE & fpmtBase)
{
	if(!m_bInitialized) return FALSE;
	
	_FPMT_PSC_LRFD_BASE dgnFpmtBase;
	dgnFpmtBase.Initialize();
	m_pscManager.ConvertFssg_CivilToEngineAASHTO(fpmtBase, dgnFpmtBase);

	CString strStageName = Get_StageName(fpmtBase.LcomK);
	CStringW strWStageName(strStageName);

	BOOL bVersionOK = FALSE;
	BOOL bCheck = FALSE;

	CDgnPscCommon PscCommon; 
	int nDgnCode = PscCommon.GetConvertCodeCvlToEng( iDgnCode);
 
	bCheck = (*m_pAlwPrincipalStressCS)(nDgnCode, iElemKey.first, iIJ, bFinal, strWStageName, &dgnFpmtBase, sizeof(_FPMT_PSC_LRFD_BASE), bVersionOK);

	return (bVersionOK && bCheck);
}

BOOL CDgn_PSCRptManager::Print_AllowablePrincipalStress_Conc_ServiceLoadCombination_MaxShear(int iDgnCode, ElemPairK iElemKey, UINT iIJ, bool bFinal, const T_FPMS_JTG_BASE & fpmsBase)
{
	if(!m_bInitialized) return FALSE;
	
	_FPMT_PSC_LRFD_BASE dgnFpmtBase;
	dgnFpmtBase.Initialize();
	m_pscManager.ConvertFpms_CivilToEngineAASHTO(fpmsBase, dgnFpmtBase);

	CString strLcomName = Get_LcomName(fpmsBase.LcomK);
	CStringW strWLcomName(strLcomName);
	
	BOOL bVersionOK = FALSE;
	BOOL bCheck = FALSE;
	
	CDgnPscCommon PscCommon; 
	int nDgnCode = PscCommon.GetConvertCodeCvlToEng( iDgnCode);
	
	bCheck = (*m_pAlwPrincipalStressShear)(nDgnCode, iElemKey.first, iIJ, bFinal, strWLcomName, &dgnFpmtBase, sizeof(_FPMT_PSC_LRFD_BASE), bVersionOK);

	return (bVersionOK && bCheck);
}

BOOL CDgn_PSCRptManager::Print_AllowablePrincipalStress_Conc_ServiceLoadCombination_MaxTorsion(int iDgnCode, ElemPairK iElemKey, UINT iIJ, bool bFinal, const T_FPMS_JTG_BASE & fpmsBase)
{
	if(!m_bInitialized) return FALSE;
	
	_FPMT_PSC_LRFD_BASE dgnFpmtBase;
	dgnFpmtBase.Initialize();
	m_pscManager.ConvertFpms_CivilToEngineAASHTO(fpmsBase, dgnFpmtBase);

	CString strLcomName = Get_LcomName(fpmsBase.LcomK);
	CStringW strWLcomName(strLcomName);

	BOOL bVersionOK = FALSE;
	BOOL bCheck = FALSE;
	
	CDgnPscCommon PscCommon; 
	int nDgnCode = PscCommon.GetConvertCodeCvlToEng( iDgnCode);
	
	bCheck = (*m_pAlwPrincipalStressTorsion)(nDgnCode, iElemKey.first, iIJ, bFinal, strWLcomName, &dgnFpmtBase, sizeof(_FPMT_PSC_LRFD_BASE), bVersionOK);

	return (bVersionOK && bCheck);
}

BOOL CDgn_PSCRptManager::Print_AllowableStress_Tendon(int iDgnCode, UINT iTndn, bool bFinal, const T_FYPC_AASHTO & fypc)
{
	if(!m_bInitialized) return FALSE;
	
	_FYPC_AASHTO dgnFypc;
	dgnFypc.Initialize();
	m_pscManager.ConvertFypc_CivilToEngineAASHTO(fypc, dgnFypc);
 
	CString strTendonName = _T(""); 
	T_TDNA_D TdnaD;	
	if(m_pDoc->m_pAttrCtrl->GetTdna(iTndn,TdnaD)) strTendonName = TdnaD.TendonName;

	BOOL bVersionOK = FALSE;
	BOOL bCheck = FALSE;

	CDgnPscCommon PscCommon; 
	int nDgnCode = PscCommon.GetConvertCodeCvlToEng( iDgnCode);

	CStringW strWTendonName(strTendonName);
	bCheck = (*m_pTendon)(nDgnCode, iTndn, bFinal, strWTendonName, &dgnFypc, sizeof(_FYPC_AASHTO), bVersionOK);

	return (bVersionOK && bCheck);
}

BOOL CDgn_PSCRptManager::Print_TensionRebarRequired(ElemPairK iElemKey, UINT iIJ, const T_TBAR_AASHTO_BASE & tbarBase)
{
	if(!m_bInitialized) return FALSE;
	
	_TBAR_PSC_LRFD_BASE dgnTbarBase;
	dgnTbarBase.Initialize();
	m_pscManager.ConvertTbar_CivilToEngineAASHTO(tbarBase, dgnTbarBase);

	BOOL bVersionOK = FALSE;
	BOOL bCheck = FALSE;

	//bCheck =  m_AASHTOReportModule.Print_TensionRebarRequired(iElemKey, iIJ, &dgnTbarBase, sizeof(_TBAR_PSC_LRFD_BASE),bVersionOK);

	return (bVersionOK && bCheck);
}

BOOL CDgn_PSCRptManager::Print_CrackCheck(ElemPairK iElemKey, UINT iIJ, const T_BDCW_AASHTO_BASE & bdcwBase)
{
	if(!m_bInitialized) return FALSE;
	
	_BDCW_PSC_LRFD_BASE dgnBdcwBase;
	dgnBdcwBase.Initialize();
	m_pscManager.ConvertBdcw_CivilToEngineAASHTO(bdcwBase, dgnBdcwBase);

	CString strLcomName = Get_LcomName(bdcwBase.LcomK);
	CStringW strWLcomName(strLcomName);
	
	BOOL bVersionOK = FALSE;
	BOOL bCheck = FALSE;

	bCheck = (*m_pCrack)(iElemKey.first, iIJ, strWLcomName, &dgnBdcwBase, sizeof(_BDCW_PSC_LRFD_BASE), bVersionOK);

	return (bVersionOK && bCheck);
}

BOOL CDgn_PSCRptManager::Print_FlexureDesign(ElemPairK iElemKey, UINT iIJ, BOOL bCompatibility, const T_BDCY_AASHTO_BASE & bdcyBase)
{
	if(!m_bInitialized) return FALSE;
	
	_BDCY_PSC_LRFD_BASE dgnBdcyBase;
	dgnBdcyBase.Initialize();
	m_pscManager.ConvertBdcy_CivilToEngineAASHTO(bdcyBase, dgnBdcyBase);

	CString strLcomName = Get_LcomName(bdcyBase.LcomK);
	CStringW strWLcomName(strLcomName);

	BOOL bVersionOK = FALSE;
	BOOL bCheck = FALSE;

	bCheck = (*m_pFlexure)(iElemKey.first, iIJ, bCompatibility, strWLcomName, &dgnBdcyBase, sizeof(_BDCY_PSC_LRFD_BASE),bVersionOK);

	return (bVersionOK && bCheck);
}

BOOL CDgn_PSCRptManager::Print_ShearDesign(ElemPairK iElemKey, UINT iIJ, const T_CUMS_AASHTO_BASE & cumsBase)
{
	if(!m_bInitialized) 
		return FALSE;
	
	BOOL bVersionOK = FALSE;
	BOOL bCheck = FALSE;

	_CUMS_AASHTO_BASE dgnCumsBase;
	
	dgnCumsBase.Initialize();
	m_pscManager.ConvertCums_CivilToEngineAASHTO(cumsBase, dgnCumsBase);

	CString strLcomName = Get_LcomName(cumsBase.LcomK);
	CStringW strWLcomName(strLcomName);
	
	bCheck = (*m_pShear)(iElemKey.first, iIJ, strWLcomName, &dgnCumsBase, sizeof(_CUMS_AASHTO_BASE),bVersionOK);

	return (bVersionOK && bCheck);
}

BOOL CDgn_PSCRptManager::Print_TorsionDesign(ElemPairK iElemKey, UINT iIJ, const T_CRMT_AASHTO_BASE & crmtBase)
{
	if(!m_bInitialized) return FALSE;
	BOOL bVersionOK = FALSE;
	BOOL bCheck = FALSE;

	_STCM_AASHTO_BASE dgnStcmBase;
	dgnStcmBase.Initialize();
	m_pscManager.ConvertCrmt_CivilToEngineAASHTO(crmtBase, dgnStcmBase);

	CString strLcomName = Get_LcomName(crmtBase.LcomK);
	CStringW strWLcomName(strLcomName);

	bCheck = (*m_pTorsion)(iElemKey.first, iIJ, strWLcomName, &dgnStcmBase.Tor, sizeof(_CRMS_AASHTO_BASE),bVersionOK);

	return (bVersionOK && bCheck);
}



CString CDgn_PSCRptManager::Get_StageName(T_STAG_K StagK)
{
	CString strStageName=_T("");
	T_STAG_D StagD;
	if (m_pDoc->m_pAttrCtrl->GetStag(StagK, StagD)) strStageName = StagD.StageName;

	return strStageName;
}

CString CDgn_PSCRptManager::Get_LcomName(T_LCOM_K LcomK, BOOL bIsOrgLcomK)
{
	CString strLcomName=_T("");
	T_LCOM_K OrgLcomK = 0;
	T_LCOM_D LcomData;
    if ( bIsOrgLcomK )
    {
        if ( m_pDoc->m_pAttrCtrl->GetLcom(D_LCOMTYPE_CONCRETE, LcomK, LcomData) ) strLcomName = LcomData.LoadCombName;
    }
    else
    {
        if ( m_pDoc->m_pPostCtrl->GetPCDesign()->GetOrgLcomK(LcomK, OrgLcomK) )
        {
            if ( m_pDoc->m_pAttrCtrl->GetLcom(D_LCOMTYPE_CONCRETE, OrgLcomK, LcomData) ) strLcomName = LcomData.LoadCombName;
        }
    }

	return strLcomName;
}

BOOL CDgn_PSCRptManager::Print_Detail_Condition(int iDgnCode, ElemPairK iElemKey, UINT iIJ, PSC_DATA_POSI& InData, _BDCY_PSC_LRFD_BASE& EngBdcyD)
{
	if(!m_bInitialized) return FALSE;
	
	BOOL bVersionOK = FALSE;
	BOOL bCheck = FALSE;
	
	CDgnPscCommon PscCommon; 
	int nDgnCode = PscCommon.GetConvertCodeCvlToEng( iDgnCode);

	bCheck = (*m_pDgnCondition)(nDgnCode, iElemKey.first, iIJ, &InData, sizeof(PSC_DATA_POSI), &EngBdcyD, sizeof(_BDCY_PSC_LRFD_BASE), bVersionOK);
	
	return (bVersionOK && bCheck);
}


BOOL CDgn_PSCRptManager::Print_Detail_FlexureDesign(int iDgnCode, ElemPairK iElemKey, UINT iIJ, BOOL bFirst, PSC_DATA_POSI& InData, _BDCY_PSC_LRFD_BASE& EngBdcyD)
{
	if(!m_bInitialized) return FALSE;
	
	BOOL bVersionOK = FALSE;
	BOOL bCheck = FALSE;
	CString strLcomName = Get_LcomName(EngBdcyD.LcomK);
	CStringW strWLcomName(strLcomName);
	
	CDgnPscCommon PscCommon; 
	int nDgnCode = PscCommon.GetConvertCodeCvlToEng( iDgnCode);

	bCheck = (*m_pDgnFlexure)(nDgnCode, iElemKey.first, iIJ, bFirst, strWLcomName, &InData, sizeof(PSC_DATA_POSI), &EngBdcyD, sizeof(_BDCY_PSC_LRFD_BASE), bVersionOK);
	
	return (bVersionOK && bCheck);
}


BOOL CDgn_PSCRptManager::Print_Detail_CrackCheck(int iDgnCode, ElemPairK iElemKey, UINT iIJ, BOOL bFirst, PSC_DATA_POSI& InData, _BDCW_PSC_LRFD_BASE & dgnBdcwBase)
{
	if(!m_bInitialized) return FALSE;
		
	BOOL bVersionOK = FALSE;
	BOOL bCheck = FALSE;
	CString strLcomName = Get_LcomName(dgnBdcwBase.LInf.LcomK);
	CStringW strWLcomName(strLcomName);

	CDgnPscCommon PscCommon; 
	int nDgnCode = PscCommon.GetConvertCodeCvlToEng( iDgnCode);

	bCheck = (*m_pDgnCrack)(nDgnCode, iElemKey.first, iIJ, bFirst, strWLcomName, &InData, sizeof(PSC_DATA_POSI), &dgnBdcwBase, sizeof(_BDCW_PSC_LRFD_BASE), bVersionOK);
	
	return (bVersionOK && bCheck);

}


BOOL CDgn_PSCRptManager::Print_Detail_ShearDesign(int iDgnCode, ElemPairK ElemKey, UINT iIJ, BOOL bFirst, PSC_DATA_POSI& InData, _CUMS_AASHTO_BASE & dgnCumsBase)
{
	if(!m_bInitialized) return FALSE;
 
	BOOL bVersionOK = FALSE;
	BOOL bCheck = FALSE;
	CString strLcomName = Get_LcomName(dgnCumsBase.LcomK);
	CStringW strWLcomName(strLcomName);

	CDgnPscCommon PscCommon; 
	int nDgnCode = PscCommon.GetConvertCodeCvlToEng( iDgnCode);

	bCheck = (*m_pDgnShear)(nDgnCode, ElemKey.first, iIJ, bFirst, strWLcomName, &InData, sizeof(PSC_DATA_POSI), &dgnCumsBase, sizeof(_CUMS_AASHTO_BASE), bVersionOK);
	 
	return (bVersionOK && bCheck);
}


BOOL CDgn_PSCRptManager::Print_Detail_TorsionDesign(int iDgnCode, ElemPairK ElemKey, UINT iIJ, BOOL bFirst, PSC_DATA_POSI& InData, _CRMS_AASHTO_BASE & dgnCrmsBase)
{
	if(!m_bInitialized) return FALSE;
	BOOL bVersionOK = FALSE;
	BOOL bCheck = FALSE;  
	CString strLcomName = Get_LcomName(dgnCrmsBase.LcomK);
	CStringW strWLcomName(strLcomName);
	 
	CDgnPscCommon PscCommon; 
	int nDgnCode = PscCommon.GetConvertCodeCvlToEng( iDgnCode);

	bCheck = (*m_pDgnTorsion)(nDgnCode, ElemKey.first, iIJ, bFirst, strWLcomName, &InData, sizeof(PSC_DATA_POSI), &dgnCrmsBase, sizeof(_CRMS_AASHTO_BASE), bVersionOK);

	return (bVersionOK && bCheck);
}

	BOOL CDgn_PSCRptManager::Print_Detail_Allowable_Comp_Fysg(int iDgnCode, ElemPairK ElemKey, UINT iIJ, BOOL bFirst, PSC_DATA_POSI& InData, _FYSG_PSC_LRFD_BASE & dgnFysgBase, BOOL bGirder)
{
	if(!m_bInitialized) return FALSE;
	BOOL bVersionOK = FALSE;
	BOOL bCheck = FALSE;  

	CString strStageName = Get_StageName(dgnFysgBase.StagK);
	CStringW strWStageName(strStageName);

	CDgnPscCommon PscCommon; 
	int nDgnCode = PscCommon.GetConvertCodeCvlToEng( iDgnCode);

	bCheck = (*m_pDgnFysg)(nDgnCode, ElemKey.first, iIJ, bFirst, strWStageName, &InData, sizeof(PSC_DATA_POSI), &dgnFysgBase, sizeof(_FYSG_PSC_LRFD_BASE), bVersionOK, bGirder);

	return (bVersionOK && bCheck);
}

	BOOL CDgn_PSCRptManager::Print_Detail_Allowable_Comp_Fycm(int iDgnCode, ElemPairK ElemKey, UINT iIJ, BOOL bFirst, PSC_DATA_POSI& InData, _FYCM_PSC_LRFD_BASE & dgnFycmBase, BOOL bGirder)
	{
		if(!m_bInitialized) return FALSE;
		BOOL bVersionOK = FALSE;
		BOOL bCheck = FALSE;  

		CString strLcomName = Get_LcomName(dgnFycmBase.LcomK);
		CStringW strWLcomName(strLcomName);

		CDgnPscCommon PscCommon; 
		int nDgnCode = PscCommon.GetConvertCodeCvlToEng( iDgnCode);

		bCheck = (*m_pDgnFycm)(nDgnCode, ElemKey.first, iIJ, bFirst, strWLcomName, &InData, sizeof(PSC_DATA_POSI), &dgnFycmBase, sizeof(_FYCM_PSC_LRFD_BASE), bVersionOK, bGirder);

		return (bVersionOK && bCheck);
	}