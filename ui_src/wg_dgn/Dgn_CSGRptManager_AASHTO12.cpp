// DGN_CSGRptManager_AASHTO12.cpp: implementation of the CDgn_CSGRptManager_AASHTO12 class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Dgn_CSGRptManager_AASHTO12.h"
#include "DgnEngineProgressDlg.h"
#include "..\wg_base\wg_base_MsgDll.h"
#include "..\wg_dbLock\LockMgr.h"
#include "..\dgnengine\src\DgnReportBase\XLStruct.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDgn_CSGRptManager_AASHTO12::CDgn_CSGRptManager_AASHTO12()
{
	m_pCSGDetail_AASHTO12  = NULL;
}

CDgn_CSGRptManager_AASHTO12::~CDgn_CSGRptManager_AASHTO12()
{
	
}

BOOL CDgn_CSGRptManager_AASHTO12::InitialData(int iNationType)
{
	HINSTANCE hDllCalc = GetOrLoad_ReportDllHandle(iNationType);
	if(hDllCalc == NULL) return FALSE;
	
	if(iNationType != DGNENGINE_US_DLL)
	{
		m_pCSGDetail_AASHTO12 = NULL;
	}
	else 
	{	
		m_pCSGDetail_AASHTO12 = (CSG_Print_DetailReport_AASHTO*)GetProcAddress(hDllCalc, "Print_CSGDetailReport_AASHTO");
		if(!m_pCSGDetail_AASHTO12) return FALSE;
	}
	
	return TRUE;
}

BOOL CDgn_CSGRptManager_AASHTO12::Print_DetailReport(int iDgnCode, LPCTSTR szProgramDir, LPCTSTR szProjectFileDir, LPCTSTR szSaveFileName,
																									CArray<CSG_MEMB_POSD_KSCE_LSD, CSG_MEMB_POSD_KSCE_LSD&>& aInData,   CArray<CSG_MEMB_RES_AASHTO_LRFD_D,  CSG_MEMB_RES_AASHTO_LRFD_D&>& aRptData)
{
	VERIFY(SUCCEEDED(OleInitialize(NULL))); 
	
	if(!InitialData(DGNENGINE_US_DLL)) return FALSE; 
		
	BOOL bVersionOK = FALSE;
	BOOL bCheck = FALSE;
	
	
	CDgnEngineProgressDlg ProDlg;
	//ProDlg.Create_ProgressDlg(_T(""), _T(""), FALSE);	// Stop버튼이 보여지더라도 Progress창의 콘트롤이 안됨
	ProDlg.Create_ProgressDlg(_LS(IDS_DGN_XL_REPORT_PRINTING), _LS(IDS_DGN_XL_REPORT_GENERATING), FALSE);	// 엔진에서 보여지는 ProgressDlg 의 Titile을 여기서 지정하였음. 엔진에서 외국어가 안되고 있음..
	D_XL_PROGRESS_INTERFACE* pProInte = ProDlg.Get_InterfacePoint();
	pProInte->bUpdateTitle   = TRUE;
	pProInte->bUpdateMessage = TRUE;
	pProInte->bUpdatePercent = TRUE;
	
	// 엔진에서 보여지는 ProgressDlg 의 Titile을 여기서 지정하였음. 엔진에서 외국어가 안되고 있음..
//   pProInte->strTitle   = _LS(IDS_DGN_XL_REPORT_PRINTING);  // _T("Printing Report")
//   pProInte->strMessage = _LS(IDS_DGN_XL_REPORT_PRINTING);  // _T("Generating Report")
//   pProInte->iPercent   =  0;

	size_t TSize_In   = sizeof(CArray<CSG_MEMB_POSD_KSCE_LSD, CSG_MEMB_POSD_KSCE_LSD&>);
	size_t TSize_Rpt  = sizeof(CArray<CSG_MEMB_RES_AASHTO_LRFD_D, CSG_MEMB_RES_AASHTO_LRFD_D&>);
	
	int nSelUnit =	CDBLib::GetReportSelUnit();
	int nDgnCode = iDgnCode;
	
	BOOL bRegression = CLockMgr::Get_IsRegressionTest(); // Regression Test인지 확인

	D_EXCEL_PRT_OPTION ROpt;
	GetRptOptionData(szProgramDir, szProjectFileDir, szSaveFileName, nDgnCode, ROpt);
    ROpt.nSelUnit = nSelUnit;

	bCheck = (*m_pCSGDetail_AASHTO12)(&ROpt, pProInte, 
		&aInData, &aRptData, TSize_In, TSize_Rpt,  bVersionOK, bRegression);
	
	return (bVersionOK && bCheck);
}
