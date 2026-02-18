// Dgn_PSCManager.cpp: implementation of the CDgn_PSCManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DgnRUSStlCompManager.h"
#include "..\wg_db\FileCtrl.h"
#include "..\dgnengine\src\DgnReportBase\XLStruct.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDgnRUSStCompManager::CDgnRUSStCompManager() : CDgn_DllManager_Base()
{
	m_pGet_StlCompTableValue = NULL;
	m_pMake_StlCompBeforeBaseData = NULL;
	m_pCheck_StlCompBeforeStrength = NULL;
	m_pCheck_StlCompBeforeStability = NULL;
	m_pMake_StlCompAfterBaseData = NULL;
	m_pMake_StlCompShearConnBaseData = NULL;
	m_pCheck_StlCompKappaCheck = NULL;
	m_pCheck_StlCompAfterStrength = NULL;
	m_pCheck_StlCompAfterStability = NULL;
	m_pCheck_StlCompShearConnector = NULL;
	m_pCheck_StlCompFatigue = NULL;
	m_pCheck_StlCompServiseability = NULL;
	m_pPrint_StlCompDetailReport_RUS = NULL;
}

CDgnRUSStCompManager::~CDgnRUSStCompManager()
{

}

BOOL CDgnRUSStCompManager::InitialData(int nDgnType, int nDgnCode)
{
	HINSTANCE hDllCalc = GetOrLoad_CalcDllHandle(nDgnType, nDgnCode);

	if (hDllCalc == NULL) return FALSE;

	// Table Default Value Setting
	m_pGet_StlCompTableValue = (PDGNENGINE_SETD*)GetProcAddress(hDllCalc, "Get_StlCompTableValue");
	if (!m_pGet_StlCompTableValue) return FALSE;

	// Before Composite Data Ctrl - Design
	m_pMake_StlCompBeforeBaseData = (PDGNENGINE_FUNC*)GetProcAddress(hDllCalc, "Make_StlCompBeforeBaseData");
	if (!m_pMake_StlCompBeforeBaseData) return FALSE;
	m_pCheck_StlCompBeforeStrength = (PDGNENGINE_FUNC3*)GetProcAddress(hDllCalc, "Check_StlCompBeforeStrength");
	if (!m_pCheck_StlCompBeforeStrength) return FALSE;
	m_pCheck_StlCompBeforeStability = (PDGNENGINE_FUNC3*)GetProcAddress(hDllCalc, "Check_StlCompBeforeStability");
	if (!m_pCheck_StlCompBeforeStability) return FALSE;

	// After Composite Data Ctrl - Design
	m_pMake_StlCompAfterBaseData = (PDGNENGINE_FUNC*)GetProcAddress(hDllCalc, "Make_StlCompAfterBaseData");
	if (!m_pMake_StlCompAfterBaseData) return FALSE;
	m_pMake_StlCompShearConnBaseData = (PDGNENGINE_FUNC*)GetProcAddress(hDllCalc, "Make_StlCompShearConnBaseData");
	if (!m_pMake_StlCompShearConnBaseData) return FALSE;
	m_pCheck_StlCompKappaCheck = (PDGNENGINE_FUNC3*)GetProcAddress(hDllCalc, "Check_StlCompKappaCheck");
	if (!m_pCheck_StlCompKappaCheck) return FALSE;
	m_pCheck_StlCompAfterStrength = (PDGNENGINE_FUNC6*)GetProcAddress(hDllCalc, "Check_StlCompAfterStrength");
	if (!m_pCheck_StlCompAfterStrength) return FALSE;
	m_pCheck_StlCompAfterStability = (PDGNENGINE_FUNC6*)GetProcAddress(hDllCalc, "Check_StlCompAfterStability");
	if (!m_pCheck_StlCompAfterStability) return FALSE;
	m_pCheck_StlCompShearConnector = (PDGNENGINE_FUNC3*)GetProcAddress(hDllCalc, "Check_StlCompShearConnector");
	if (!m_pCheck_StlCompShearConnector) return FALSE;
	m_pCheck_StlCompFatigue = (PDGNENGINE_FUNC6*)GetProcAddress(hDllCalc, "Check_StlCompFatigue");
	if (!m_pCheck_StlCompFatigue) return FALSE;
	m_pCheck_StlCompServiseability = (PDGNENGINE_FUNC6*)GetProcAddress(hDllCalc, "Check_StlCompServiseability");
	if (!m_pCheck_StlCompServiseability) return FALSE;

	// Excel Report
	m_pPrint_StlCompDetailReport_RUS = (RUSStlCompGirder_DetailReport*)GetProcAddress(hDllCalc, "Print_StlCompDetailReport_RUS");
	if (!m_pPrint_StlCompDetailReport_RUS) return FALSE;

	return TRUE;
}

// Table Default Value Setting
BOOL CDgnRUSStCompManager::Get_StlCompTableValue(STL_SNiP_TB_D &rData)
{
	size_t TSize_Res = sizeof(STL_SNiP_TB_D);

	if (m_pGet_StlCompTableValue == NULL) return FALSE;

	BOOL bVersionOK = FALSE;
	BOOL bCheck = (*m_pGet_StlCompTableValue)(&rData, TSize_Res, bVersionOK);

	if (!bCheck || !bVersionOK) return FALSE;
	return TRUE;
}

// Before Composite Data Ctrl - Design
BOOL CDgnRUSStCompManager::Make_StlCompBeforeBaseData(STL_COMP_MEMB_POSD_SNiP &MembPosD, STL_COMP_RES_BEFORE_BASE_SNiP &BeforeMembBaseD)
{
	size_t TSize_In = sizeof(STL_COMP_MEMB_POSD_SNiP);
	size_t TSize_Res = sizeof(STL_COMP_RES_BEFORE_BASE_SNiP);

	if (m_pGet_StlCompTableValue == NULL) return FALSE;

	BOOL bVersionOK = FALSE;
	BOOL bCheck = (*m_pMake_StlCompBeforeBaseData)(&MembPosD, &BeforeMembBaseD, TSize_In, TSize_Res, bVersionOK);

	if (!bCheck || !bVersionOK) return FALSE;
	return TRUE;
}

BOOL  CDgnRUSStCompManager::Check_StlCompBeforeStrength(STL_COMP_MEMB_POSD_SNiP &MembPosD, STL_COMP_RES_BEFORE_BASE_SNiP &BeforeMembBaseD,
	STL_COMP_MEMB_RESD_SNiP_BEFORE_STR &BeforeStrResD)
{
	size_t TSize_In = sizeof(STL_COMP_MEMB_POSD_SNiP);
	size_t TSize_MembBaseIn = sizeof(STL_COMP_RES_BEFORE_BASE_SNiP);
	size_t TSize_Res = sizeof(STL_COMP_MEMB_RESD_SNiP_BEFORE_STR);

	if (m_pCheck_StlCompBeforeStrength == NULL) return FALSE;

	BOOL bVersionOK = FALSE;
	BOOL bCheck = (*m_pCheck_StlCompBeforeStrength)(&MembPosD, &BeforeMembBaseD, &BeforeStrResD, TSize_In, TSize_MembBaseIn, TSize_Res, bVersionOK);

	if (!bCheck || !bVersionOK) return FALSE;
	return TRUE;
}

BOOL  CDgnRUSStCompManager::Check_StlCompBeforeStability(STL_COMP_MEMB_POSD_SNiP &MembPosD, STL_COMP_RES_BEFORE_BASE_SNiP &BeforeMembBaseD,
	STL_COMP_MEMB_RESD_SNiP_BEFORE_STB &BeforeStaResD)
{
	size_t TSize_In = sizeof(STL_COMP_MEMB_POSD_SNiP);
	size_t TSize_MembBaseIn = sizeof(STL_COMP_RES_BEFORE_BASE_SNiP);
	size_t TSize_Res = sizeof(STL_COMP_MEMB_RESD_SNiP_BEFORE_STB);

	if (m_pCheck_StlCompBeforeStability == NULL) return FALSE;

	BOOL bVersionOK = FALSE;
	BOOL bCheck = (*m_pCheck_StlCompBeforeStability)(&MembPosD, &BeforeMembBaseD, &BeforeStaResD, TSize_In, TSize_MembBaseIn, TSize_Res, bVersionOK);

	if (!bCheck || !bVersionOK) return FALSE;
	return TRUE;
}

// After Composite Data Ctrl - Design
BOOL CDgnRUSStCompManager::Make_StlCompAfterBaseData(STL_COMP_MEMB_POSD_SNiP &MembPosD, STL_COMP_RES_AFTER_BASE_SNiP &AfterMembBaseD)
{
	size_t TSize_In = sizeof(STL_COMP_MEMB_POSD_SNiP);
	size_t TSize_Res = sizeof(STL_COMP_RES_AFTER_BASE_SNiP);

	if (m_pMake_StlCompAfterBaseData == NULL) return FALSE;

	BOOL bVersionOK = FALSE;
	BOOL bCheck = (*m_pMake_StlCompAfterBaseData)(&MembPosD, &AfterMembBaseD, TSize_In, TSize_Res, bVersionOK);

	if (!bCheck || !bVersionOK) return FALSE;
	return TRUE;
}

BOOL CDgnRUSStCompManager::Make_StlCompShearConnBaseData(STL_COMP_MEMB_POSD_SNiP &MembPosD, STL_COMP_AFTER_SHEAR_CONN_BASE_SNiP &AfterMembBaseD)
{
	size_t TSize_In = sizeof(STL_COMP_MEMB_POSD_SNiP);
	size_t TSize_Res = sizeof(STL_COMP_AFTER_SHEAR_CONN_BASE_SNiP);

	if (m_pMake_StlCompShearConnBaseData == NULL) return FALSE;

	BOOL bVersionOK = FALSE;
	BOOL bCheck = (*m_pMake_StlCompShearConnBaseData)(&MembPosD, &AfterMembBaseD, TSize_In, TSize_Res, bVersionOK);

	if (!bCheck || !bVersionOK) return FALSE;
	return TRUE;
}

BOOL  CDgnRUSStCompManager::Check_StlCompKappaCheck(STL_COMP_MEMB_POSD_SNiP &MembPosD, STL_COMP_RES_AFTER_BASE_SNiP &AfterMembBaseD,
	STL_COMP_MEMB_RESD_SNiP_KAPPA &KappaChkResD)
{
	size_t TSize_In = sizeof(STL_COMP_MEMB_POSD_SNiP);
	size_t TSize_MembBaseIn = sizeof(STL_COMP_RES_AFTER_BASE_SNiP);
	size_t TSize_Res = sizeof(STL_COMP_MEMB_RESD_SNiP_KAPPA);

	if (m_pCheck_StlCompKappaCheck == NULL) return FALSE;

	BOOL bVersionOK = FALSE;
	BOOL bCheck = (*m_pCheck_StlCompKappaCheck)(&MembPosD, &AfterMembBaseD, &KappaChkResD, TSize_In, TSize_MembBaseIn, TSize_Res, bVersionOK);

	if (!bCheck || !bVersionOK) return FALSE;
	return TRUE;
}

BOOL  CDgnRUSStCompManager::Check_StlCompAfterStrength(STL_COMP_MEMB_POSD_SNiP &MembPosD, STL_COMP_RES_AFTER_BASE_SNiP &AfterMembBaseD,
	STL_COMP_MEMB_RESD_SNiP_KAPPA &KappaChkResD, STL_COMP_MEMB_RESD_SNiP_AFTER_STR &AfterStrResD)
{
	size_t TSize_In = sizeof(STL_COMP_MEMB_POSD_SNiP);
	size_t TSize_MembBaseIn = sizeof(STL_COMP_RES_AFTER_BASE_SNiP);
	size_t TSize_KappaResIn = sizeof(STL_COMP_MEMB_RESD_SNiP_KAPPA);
	size_t TSize_Res = sizeof(STL_COMP_MEMB_RESD_SNiP_AFTER_STR);

	if (m_pCheck_StlCompAfterStrength == NULL) return FALSE;

	BOOL bVersionOK = FALSE;
	BOOL bCheck = (*m_pCheck_StlCompAfterStrength)(&MembPosD, &AfterMembBaseD, &KappaChkResD, &AfterStrResD, TSize_In, TSize_MembBaseIn, TSize_KappaResIn, TSize_Res, bVersionOK);

	if (!bCheck || !bVersionOK) return FALSE;
	return TRUE;
}


BOOL  CDgnRUSStCompManager::Check_StlCompAfterStability(STL_COMP_MEMB_POSD_SNiP &MembPosD, STL_COMP_RES_AFTER_BASE_SNiP &AfterMembBaseD,
	STL_COMP_MEMB_RESD_SNiP_KAPPA &KappaChkResD, STL_COMP_MEMB_RESD_SNiP_BEFORE_STB &AfterStaResD)
{
	size_t TSize_In = sizeof(STL_COMP_MEMB_POSD_SNiP);
	size_t TSize_MembBaseIn = sizeof(STL_COMP_RES_AFTER_BASE_SNiP);
	size_t TSize_KappaResIn = sizeof(STL_COMP_MEMB_RESD_SNiP_KAPPA);
	size_t TSize_Res = sizeof(STL_COMP_MEMB_RESD_SNiP_BEFORE_STB);

	if (m_pCheck_StlCompAfterStability == NULL) return FALSE;

	BOOL bVersionOK = FALSE;
	BOOL bCheck = (*m_pCheck_StlCompAfterStability)(&MembPosD, &AfterMembBaseD, &KappaChkResD, &AfterStaResD, TSize_In, TSize_MembBaseIn, TSize_KappaResIn, TSize_Res, bVersionOK);

	if (!bCheck || !bVersionOK) return FALSE;
	return TRUE;
}

BOOL  CDgnRUSStCompManager::Check_StlCompShearConnector(STL_COMP_MEMB_POSD_SNiP &MembPosD, STL_COMP_AFTER_SHEAR_CONN_BASE_SNiP &ShearConnBaseD,
	STL_COMP_MEMB_RESD_SNiP_SHEAR_CONN &ShearConnResD)
{
	size_t TSize_In = sizeof(STL_COMP_MEMB_POSD_SNiP);
	size_t TSize_MembBaseIn = sizeof(STL_COMP_AFTER_SHEAR_CONN_BASE_SNiP);
	size_t TSize_Res = sizeof(STL_COMP_MEMB_RESD_SNiP_SHEAR_CONN);

	if (m_pCheck_StlCompShearConnector == NULL) return FALSE;

	BOOL bVersionOK = FALSE;
	BOOL bCheck = (*m_pCheck_StlCompShearConnector)(&MembPosD, &ShearConnBaseD, &ShearConnResD, TSize_In, TSize_MembBaseIn, TSize_Res, bVersionOK);

	if (!bCheck || !bVersionOK) return FALSE;
	return TRUE;
}

BOOL  CDgnRUSStCompManager::Check_StlCompFatigue(STL_COMP_MEMB_POSD_SNiP &MembPosD, STL_COMP_RES_AFTER_BASE_SNiP &AfterMembBaseD,
	STL_COMP_MEMB_RESD_SNiP_KAPPA &KappaChkResD, STL_COMP_MEMB_RESD_SNiP_AFTER_FATI &AfterFatiResD)
{
	size_t TSize_In = sizeof(STL_COMP_MEMB_POSD_SNiP);
	size_t TSize_MembBaseIn = sizeof(STL_COMP_RES_AFTER_BASE_SNiP);
	size_t TSize_KappaResIn = sizeof(STL_COMP_MEMB_RESD_SNiP_KAPPA);
	size_t TSize_Res = sizeof(STL_COMP_MEMB_RESD_SNiP_AFTER_FATI);

	if (m_pCheck_StlCompFatigue == NULL) return FALSE;

	BOOL bVersionOK = FALSE;
	BOOL bCheck = (*m_pCheck_StlCompFatigue)(&MembPosD, &AfterMembBaseD, &KappaChkResD, &AfterFatiResD, TSize_In, TSize_MembBaseIn, TSize_KappaResIn, TSize_Res, bVersionOK);

	if (!bCheck || !bVersionOK) return FALSE;
	return TRUE;
}

BOOL  CDgnRUSStCompManager::Check_StlCompServiseability(STL_COMP_MEMB_POSD_SNiP &MembPosD, STL_COMP_RES_AFTER_BASE_SNiP &AfterMembBaseD,
	STL_COMP_MEMB_RESD_SNiP_KAPPA &KappaChkResD, STL_COMP_MEMB_RESD_SNiP_AFTER_SERV &AfterServResD)
{
	size_t TSize_In = sizeof(STL_COMP_MEMB_POSD_SNiP);
	size_t TSize_MembBaseIn = sizeof(STL_COMP_RES_AFTER_BASE_SNiP);
	size_t TSize_KappaResIn = sizeof(STL_COMP_MEMB_RESD_SNiP_KAPPA);
	size_t TSize_Res = sizeof(STL_COMP_MEMB_RESD_SNiP_AFTER_SERV);

	if (m_pCheck_StlCompServiseability == NULL) return FALSE;

	BOOL bVersionOK = FALSE;
	BOOL bCheck = (*m_pCheck_StlCompServiseability)(&MembPosD, &AfterMembBaseD, &KappaChkResD, &AfterServResD, TSize_In, TSize_MembBaseIn, TSize_KappaResIn, TSize_Res, bVersionOK);

	if (!bCheck || !bVersionOK) return FALSE;
	return TRUE;
}

// Excel Report
BOOL CDgnRUSStCompManager::Print_StlCompDetailReport_RUS(int nSelUnit, int nDgnCode, CString strProgramDir, CString strBasefileDir, CString strSaveFileName,
	STL_COMP_RPT_RES_TOTAL &RptResD, BOOL& bVersionOK, BOOL& bRegression)
{
	if (m_pPrint_StlCompDetailReport_RUS == NULL) return FALSE;

	size_t TSize_Res = sizeof(STL_COMP_RPT_RES_TOTAL);

	CDgnEngineProgressDlg ProDlg;
	//ProDlg.Create_ProgressDlg(_T(""), _T(""), FALSE);	// Stop버튼이 보여지더라도 Progress창의 콘트롤이 안됨
	ProDlg.Create_ProgressDlg(_LS(IDS_DGN_XL_REPORT_PRINTING), _LS(IDS_DGN_XL_REPORT_GENERATING), FALSE);	// 엔진에서 보여지는 ProgressDlg 의 Titile을 여기서 지정하였음. 엔진에서 외국어가 안되고 있음..
	D_XL_PROGRESS_INTERFACE* pProInte = ProDlg.Get_InterfacePoint();
	pProInte->bUpdateTitle = TRUE;
	pProInte->bUpdateMessage = TRUE;
	pProInte->bUpdatePercent = TRUE;

	CString strProjectDir = _T("");

	D_EXCEL_PRT_OPTION ROpt;
	GetRptOptionData(strProgramDir, strProjectDir, strSaveFileName, nDgnCode, ROpt);
	ROpt.strBasePath = CT2W(strBasefileDir);
	ROpt.nSelUnit = nSelUnit;

	BOOL bCheck = (*m_pPrint_StlCompDetailReport_RUS)(&ROpt, pProInte, &RptResD, TSize_Res, bVersionOK, bRegression);

	if (!bCheck || !bVersionOK) return FALSE;
	return TRUE;
}




