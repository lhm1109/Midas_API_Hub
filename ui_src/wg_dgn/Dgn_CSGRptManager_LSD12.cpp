// DGN_CSGRptManager_LSD12.cpp: implementation of the CDgn_CSGRptManager_LSD12 class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "..\DgnEngine\Src\DgnCalcBase\DgnCalcBase_PSC_Struct.h"
#include "Dgn_CSGRptManager_LSD12.h"
#include "DgnEngineProgressDlg.h"
#include "..\wg_base\wg_base_MsgDll.h"
#include "..\wg_dbLock\LockMgr.h"
#include "..\dgnengine\src\DgnReportBase\XLStruct.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDgn_CSGRptManager_LSD12::CDgn_CSGRptManager_LSD12()
{
	m_pCSGDetail_LSD12  = NULL;
}

CDgn_CSGRptManager_LSD12::~CDgn_CSGRptManager_LSD12()
{
	
}

BOOL CDgn_CSGRptManager_LSD12::InitialData(int iNationType)
{
	HINSTANCE hDllCalc = GetOrLoad_ReportDllHandle(iNationType);
	if(hDllCalc == NULL) return FALSE;
	
	if(iNationType != DGNENGINE_US_DLL)
	{
		m_pCSGDetail_LSD12 = NULL;
	}
	else 
	{	
		m_pCSGDetail_LSD12 = (CSG_Print_DetailReport_LSD*)GetProcAddress(hDllCalc, "Print_CSGDetailReport_LSD");
		if(!m_pCSGDetail_LSD12) return FALSE;
	}
	
	return TRUE;
}

BOOL CDgn_CSGRptManager_LSD12::Print_DetailReport(int iDgnCode, LPCTSTR szProgramDir, LPCTSTR szProjectFileDir, LPCTSTR szSaveFileName,
																									CArray<CSG_INDATA_SPAN, CSG_INDATA_SPAN&>& aSpInData,  CArray<CSG_DRES_SPAN_PRT, CSG_DRES_SPAN_PRT&>& aSpRptData,
																									CArray<CSG_MEMB_POSD_KSCE_LSD, CSG_MEMB_POSD_KSCE_LSD&>& aInData,   CArray<CSG_DRES_PRT,  CSG_DRES_PRT&>& aRptData)
//                                                          CArray<PSC_FYSG_BASE, PSC_FYSG_BASE&>& aFysgSNiP, CArray<PSC_FYCM_BASE, PSC_FYCM_BASE&>& aFycmSNiP,
//                                                          CArray<PSC_FPSG_BASE, PSC_FPSG_BASE&>& aFpsgSNiP, CArray<PSC_FPMS_BASE, PSC_FPMS_BASE&>& aFpmsSNiP,
//                                                          CArray<PSC_FYPC_D, PSC_FYPC_D&>& aFypcSNiP)
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

	size_t TSize_In   = sizeof(CArray<CSG_MEMB_POSD_KSCE_LSD, CSG_MEMB_POSD_KSCE_LSD&>);
	size_t TSize_Rpt  = sizeof(CArray<CSG_DRES_PRT, CSG_DRES_PRT&>);
//   size_t TSize_Fysg = sizeof(CArray<PSC_FYSG_BASE, PSC_FYSG_BASE&>);
//   size_t TSize_Fycm = sizeof(CArray<PSC_FYCM_BASE, PSC_FYCM_BASE&>);
//   size_t TSize_Fpsg = sizeof(CArray<PSC_FPSG_BASE, PSC_FPSG_BASE&>);
//   size_t TSize_Fpms = sizeof(CArray<PSC_FPMS_BASE, PSC_FPMS_BASE&>);
//   size_t TSize_Fypc = sizeof(CArray<PSC_FYPC_D, PSC_FYPC_D&>);
	
	CDgnPscCommon PscCommon; 
	int nDgnCode = 0; //PscCommon.GetConvertCodeCvlToEng(iDgnCode);
	BOOL bRegression = CLockMgr::Get_IsRegressionTest(); // Regression Test인지 확인

	D_EXCEL_PRT_OPTION ROpt;
	GetRptOptionData(szProgramDir, szProjectFileDir, szSaveFileName, nDgnCode, ROpt);


	 bCheck = (*m_pCSGDetail_LSD12)(&ROpt, pProInte,
		 &aSpInData, &aSpRptData, &aInData, &aRptData, TSize_In, TSize_Rpt, bVersionOK, bRegression);

	return (bVersionOK && bCheck);
}
