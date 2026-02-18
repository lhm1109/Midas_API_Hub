// Dgn_OrthotropicDeckRptManager.cpp: implementation of the CDgn_OrthotropicDeckRptManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "..\DgnEngine\Src\DgnCalcBase\DgnCalcBase_PSC_Struct.h"
#include "..\DgnEngine\Src\DgnCalcBase\DgnCalcBase_PSC_ResultStruct.h"
#include "..\DgnEngine\Src\CVL_DgnCalc_US\DgnCalc_US_PSC_Struct.h"
#include "Dgn_OrthotropicDeckRptManager.h"
#include "DgnEngineProgressDlg.h"
#include "DgnPscCommon.h"
#include "..\wg_dbLock\LockMgr.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDgn_OrthotropicDeckRptManager::CDgn_OrthotropicDeckRptManager()
{
	m_pODDDetail_RUS  = NULL;
}

CDgn_OrthotropicDeckRptManager::~CDgn_OrthotropicDeckRptManager()
{
	
}

BOOL CDgn_OrthotropicDeckRptManager::InitialData(int iNationType)
{
	HINSTANCE hDllCalc = GetOrLoad_ReportDllHandle(iNationType);
	if(hDllCalc == NULL) return FALSE;
	
	if(iNationType != DGNENGINE_RS_DLL)
	{
		m_pODDDetail_RUS = NULL;
	}
	else 
	{	
		m_pODDDetail_RUS = (OrthotropicDeck_DetailReport_RUS*)GetProcAddress(hDllCalc, "Print_ODDetailReport_RUS");
		if(!m_pODDDetail_RUS) return FALSE;
	}
	
	return TRUE;
}

BOOL CDgn_OrthotropicDeckRptManager::Print_DetailReport(int iDgnCode,  LPCTSTR szProgramDir, LPCTSTR szBaseFileDirr, LPCTSTR szSaveFileNameDir, CArray<STL_MEMB_POSD_SNiP_OD, STL_MEMB_POSD_SNiP_OD&>& aInData,   CArray<STL_MEMB_RES_SNiP_OD,  STL_MEMB_RES_SNiP_OD&>& aRptData)
{
	VERIFY(SUCCEEDED(OleInitialize(NULL))); 
	
	if(!InitialData(DGNENGINE_RS_DLL)) return FALSE; 
	
	BOOL bVersionOK = FALSE;
	BOOL bCheck = FALSE;
	
	
	CDgnEngineProgressDlg ProDlg;
	ProDlg.Create_ProgressDlg(_T(""), _T(""), FALSE);	// Stop버튼이 보여지더라도 Progress창의 콘트롤이 안됨
	D_XL_PROGRESS_INTERFACE* pProInte = ProDlg.Get_InterfacePoint();
	pProInte->bUpdateTitle   = TRUE;
	pProInte->bUpdateMessage = TRUE;
	pProInte->bUpdatePercent = TRUE;
	
	size_t TSize_In   = sizeof(STL_MEMB_POSD_SNiP_OD);
	size_t TSize_Rpt  = sizeof(STL_MEMB_RES_SNiP_OD);
	
	CDgnPscCommon PscCommon;
	int nSelUnit =	CDBLib::GetReportSelUnit();
	int nDgnCode = PscCommon.GetConvertSodCodeCvlToEng(iDgnCode);
	BOOL bRegression = CLockMgr::Get_IsRegressionTest(); // Regression Test인지 확인

	CStringW strWProgramDir(szProgramDir);
	CStringW strWBaseFileDir(szBaseFileDirr);
	CStringW strWSaveFileName(szSaveFileNameDir);

	bCheck = (*m_pODDDetail_RUS)(nSelUnit, nDgnCode, strWProgramDir, strWBaseFileDir, strWSaveFileName, pProInte, 
		&aInData, &aRptData, TSize_In, TSize_Rpt, bVersionOK, bRegression);

	
	return (bVersionOK && bCheck);
}
