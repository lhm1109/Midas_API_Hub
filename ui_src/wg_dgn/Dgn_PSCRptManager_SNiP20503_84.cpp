// Dgn_PSCRptManager_SNiP20503_84.cpp: implementation of the CDgn_PSCRptManager_SNiP20503_84 class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "..\DgnEngine\Src\DgnCalcBase\DgnCalcBase_PSC_Struct.h"
#include "..\DgnEngine\Src\DgnCalcBase\DgnCalcBase_PSC_ResultStruct.h"
#include "..\DgnEngine\Src\DGNe_RUS\DgnCalc_Rus_PSC_Struct.h"
#include "Dgn_PSCRptManager_SNiP20503_84.h"
#include "DgnEngineProgressDlg.h"

#include "..\dgnengine\src\DgnReportBase\XLStruct.h"
#include "..\wg_dbLock\LockMgr.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDgn_PSCRptManager_SNiP20503_84::CDgn_PSCRptManager_SNiP20503_84()
{
	m_pDetail        = NULL;
}

CDgn_PSCRptManager_SNiP20503_84::~CDgn_PSCRptManager_SNiP20503_84()
{

}

BOOL CDgn_PSCRptManager_SNiP20503_84::InitialData(int iNationType)
{
	HINSTANCE hDllCalc = GetOrLoad_ReportDllHandle(iNationType);
	if(hDllCalc == NULL) return FALSE;

	if(iNationType != DGNENGINE_RS_DLL)
	{
		m_pDetail = NULL;
	}
	else 
	{	
		m_pDetail = (PSC_Print_Detail_Design_SNiP*)GetProcAddress(hDllCalc, "Print_DetailReport");
		if(!m_pDetail) return FALSE;
	}

	return TRUE;
}

BOOL CDgn_PSCRptManager_SNiP20503_84::Print_DetailReport(int iDgnCode, LPCTSTR szProgramDir, LPCTSTR szProjectFileDir, LPCTSTR szSaveFileName,
																												 CArray<PSC_DATA_POSI, PSC_DATA_POSI&>& aInData,   CArray<PSC_DRES_RPT_SNIP, PSC_DRES_RPT_SNIP&>& aRptData,
																												 CArray<PSC_FYSG_BASE, PSC_FYSG_BASE&>& aFysgSNiP, CArray<PSC_FYCM_BASE, PSC_FYCM_BASE&>& aFycmSNiP,
																												 CArray<PSC_FPSG_BASE, PSC_FPSG_BASE&>& aFpsgSNiP, CArray<PSC_FPMS_BASE, PSC_FPMS_BASE&>& aFpmsSNiP,
																												 CArray<PSC_FYPC_D, PSC_FYPC_D&>& aFypcSNiP)
{
	VERIFY(SUCCEEDED(OleInitialize(NULL))); 
	
	if(!InitialData(DGNENGINE_RS_DLL)) return FALSE;

	BOOL bVersionOK = FALSE;
	BOOL bCheck = FALSE;

	size_t TSize_ROpt = sizeof(D_EXCEL_PRT_OPTION);
	size_t TSize_In   = sizeof(CArray<PSC_DATA_POSI, PSC_DATA_POSI&>);
	size_t TSize_Rpt  = sizeof(CArray<PSC_DRES_RPT_SNIP, PSC_DRES_RPT_SNIP&>);
	size_t TSize_Fysg = sizeof(CArray<PSC_FYSG_BASE, PSC_FYSG_BASE&>);
	size_t TSize_Fycm = sizeof(CArray<PSC_FYCM_BASE, PSC_FYCM_BASE&>);
	size_t TSize_Fpsg = sizeof(CArray<PSC_FPSG_BASE, PSC_FPSG_BASE&>);
	size_t TSize_Fpms = sizeof(CArray<PSC_FPMS_BASE, PSC_FPMS_BASE&>);
	size_t TSize_Fypc = sizeof(CArray<PSC_FYPC_D, PSC_FYPC_D&>);

	CDgnPscCommon PscCommon; 
	int nDgnCode = PscCommon.GetConvertCodeCvlToEng(iDgnCode);
	
	BOOL bRegression = CLockMgr::Get_IsRegressionTest(); // Regression Test인지 확인

	D_EXCEL_PRT_OPTION ROpt;
	GetRptOptionData(szProgramDir, szProjectFileDir, szSaveFileName, nDgnCode, ROpt);

	bCheck = (*m_pDetail)(&ROpt, TSize_ROpt,
												&aInData, &aRptData, &aFysgSNiP, &aFycmSNiP, &aFpsgSNiP, &aFpmsSNiP, &aFypcSNiP,
		                    TSize_In, TSize_Rpt, TSize_Fysg, TSize_Fycm, TSize_Fpsg, TSize_Fpms, TSize_Fypc, bVersionOK, bRegression);
	
	return (bVersionOK && bCheck);
}
