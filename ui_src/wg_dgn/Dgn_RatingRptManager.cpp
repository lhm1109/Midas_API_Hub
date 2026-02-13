// Dgn_RatingRptManager.cpp: implementation of the Dgn_RatingRptManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Dgn_RatingRptManager.h"
#include "..\wg_base\MSExcel.h"
#include "..\wg_dbLock\LockMgr.h"
#include "DgnEngineProgressDlg.h"

#include "..\dgnengine\src\DgnReportBase\XLStruct.h"

//#include "..\dgnengine\src\DgnReportBase\XLStruct.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDgn_RatingRptManager::CDgn_RatingRptManager()
{
	m_pRatingDetailRC  = NULL;
    m_pRatingDetailSTL  = NULL;
    m_pRatingDetailSTLAASHTO = NULL;
    m_pRatingDetailPSCAASHTO  = NULL;
	m_pRatingRF      = NULL;
	m_pRatingPgmSet  = NULL;
	m_pRatingRF_RC    = NULL;
}

CDgn_RatingRptManager::~CDgn_RatingRptManager()
{
	
}

BOOL CDgn_RatingRptManager::InitialData(int iNationType)
{
	HINSTANCE hDllCalc = GetOrLoad_ReportDllHandle(iNationType);
	if(hDllCalc == NULL) return FALSE;
	
	if (iNationType == DGNENGINE_KR_DLL)
	{	
        m_pRatingDetailSTL = (Rating_DetailReport*)GetProcAddress(hDllCalc, "STL_BeamCol_PrintReport");
		m_pRatingRF     = (Rating_RFReport*)GetProcAddress(hDllCalc, "Print_RatingDetailReport_KSCE_ASD");
		m_pRatingPgmSet = (Rating_SetPGMInfo*)GetProcAddress(hDllCalc, "Set_CallProgramInfo");
		m_pRatingRF_RC   = (Rating_RFReport_RC_KSCE_USD*)GetProcAddress(hDllCalc, "RCSC_Beam_Rating_KSCE_USD_PrintReport");
 
		if(!m_pRatingDetailSTL ) return FALSE;
		if(!m_pRatingRF)     return FALSE;
		if(!m_pRatingPgmSet) return FALSE;
		if(!m_pRatingRF_RC)   return FALSE;

	}
	else if (iNationType == DGNENGINE_US_DLL)
	{
        m_pRatingDetailSTLAASHTO = (Rating_DetailReportSTL*)GetProcAddress(hDllCalc, "Print_RatingReportSTL");
        m_pRatingDetailPSCAASHTO = (Rating_DetailReportPSC*)GetProcAddress(hDllCalc, "Print_RatingReportPSC");
		m_pRatingPgmSet = (Rating_SetPGMInfo*)GetProcAddress(hDllCalc, "Set_CallProgramInfo");
		m_pRatingCptDetail = (Rating_CptDetailReport*)GetProcAddress(hDllCalc, "Print_Rating_CptDetailReport");
		if (!m_pRatingDetailSTLAASHTO && !m_pRatingDetailPSCAASHTO ) return FALSE;

	}
	else
	{
        m_pRatingDetailSTLAASHTO = NULL;
        m_pRatingDetailPSCAASHTO = NULL;
		m_pRatingRF     = NULL;
		m_pRatingPgmSet = NULL;
		m_pRatingRF_RC   = NULL;
	}
	
	return TRUE;
}

void CDgn_RatingRptManager::SetDgnCode(int nDgnCode)
{
	m_nDgnCode = nDgnCode;
}

BOOL CDgn_RatingRptManager::Print_RFReport(CString szProgramDir, CString szProjectFileDir, CString szSaveFileName, D_LCOM_INFO& InData,   DGNEXCEL_KR_STLC_OPTION &OptD, 
																					 CArray<DGNEXCEL_KR_STL_SECT,  DGNEXCEL_KR_STL_SECT&>& aRptData, 
																					 CArray<RF_RES_RF_KSCE_ASD_D,  RF_RES_RF_KSCE_ASD_D&>& aRFRptData, 
																					 CArray<CSG_MEMB_POSD_KR_D,  CSG_MEMB_POSD_KR_D&> &aRptCsdInData,
																					 CArray<S_KR_LSD_RES_D, S_KR_LSD_RES_D&> &arLSDResD)                      
{
	VERIFY(SUCCEEDED(OleInitialize(NULL))); 
	
	if(!InitialData(DGNENGINE_KR_DLL)) return FALSE; 
	
	BOOL bVersionOK = FALSE;
	BOOL bCheck = FALSE;
	
	
	CDgnEngineProgressDlg ProDlg;
	ProDlg.Create_ProgressDlg(_T(""), _T(""), FALSE);	// Stop버튼이 보여지더라도 Progress창의 콘트롤이 안됨
	D_XL_PROGRESS_INTERFACE* pProInte = ProDlg.Get_InterfacePoint();
	pProInte->bUpdateTitle   = TRUE;
	pProInte->bUpdateMessage = TRUE;
	pProInte->bUpdatePercent = TRUE;
	size_t Tsize_Prog = sizeof(D_XL_PROGRESS_INTERFACE);
	//DLLEXPORT BOOL Set_CallProgramInfo(CString strBasePath, CString strDataPath, void* pProgress, size_t Tsize, BOOL& bVersionOK)
	CStringW strWProgramDir(szProgramDir);
	CStringW strWProjectFileDir(szProjectFileDir);
	bCheck = (*m_pRatingPgmSet)(strWProgramDir, strWProjectFileDir, pProInte, Tsize_Prog, bVersionOK);
	
	size_t TSize_In   = sizeof(D_LCOM_INFO);
	size_t TSize_Opt  = sizeof(DGNEXCEL_KR_STLC_OPTION);
	size_t TSize_Rpt  = sizeof(CArray<DGNEXCEL_KR_STL_SECT, DGNEXCEL_KR_STL_SECT&>);
	size_t TSize_RptRF= sizeof(CArray<RF_RES_RF_KSCE_ASD_D, RF_RES_RF_KSCE_ASD_D&>);
	size_t TSize_CsdInRF = sizeof(CArray<CSG_MEMB_POSD_KR_D, CSG_MEMB_POSD_KR_D&>);
	size_t TSize_LSDR = sizeof(CArray<S_KR_LSD_RES_D, S_KR_LSD_RES_D&>);

	bCheck = (*m_pRatingRF)(&OptD, &InData, &aRptData, &aRFRptData, &aRptCsdInData, &arLSDResD, TSize_Opt, TSize_In, TSize_Rpt, TSize_RptRF, TSize_CsdInRF, TSize_LSDR, bVersionOK);
	
	return (bVersionOK && bCheck);
}

BOOL CDgn_RatingRptManager::Print_RF_CptDetailReport(CString szProgramDir, CString szProjectFileDir, CString szSaveFileName, 
																										 DGNEXCEL_KR_STLC_OPTION &OptD, 
																										 CArray<DGNEXCEL_KR_STL_SECT,  DGNEXCEL_KR_STL_SECT&>& aRptData, 
																										 CArray<CSG_MEMB_POSD_KSCE_LSD,  CSG_MEMB_POSD_KSCE_LSD&> &aRptCsdInData,
																										 CArray<CSG_DRES_PRT,CSG_DRES_PRT&> &aCptLSDRes,
																										 CArray<CSG_MEMB_RES_AASHTO_LRFD_D,CSG_MEMB_RES_AASHTO_LRFD_D&> &aCptKSSCRes)
{
	VERIFY(SUCCEEDED(OleInitialize(NULL))); 

	if(!InitialData(DGNENGINE_US_DLL)) return FALSE; 

	BOOL bVersionOK = FALSE;
	BOOL bCheck = FALSE;

	BOOL bRegression = CLockMgr::Get_IsRegressionTest(); // Regression Test인지 확인

	int nDgnCode = 0;
	if (aRptCsdInData.GetSize()>0)
	{
		nDgnCode = aRptCsdInData[0].Memb.Csgd.nDgnCode;
	}

	D_EXCEL_PRT_OPTION ROpt;
	GetRptOptionData(szProgramDir, szProjectFileDir, szSaveFileName, nDgnCode, ROpt);
	ROpt.nPrintOpt = 1; // 1:Insert in an Exist File
	ROpt.bIsView = TRUE;

	CDgnEngineProgressDlg ProDlg;
	ProDlg.Create_ProgressDlg(_T(""), _T(""), FALSE);	// Stop버튼이 보여지더라도 Progress창의 콘트롤이 안됨
	D_XL_PROGRESS_INTERFACE* pProInte = ProDlg.Get_InterfacePoint();
	pProInte->bUpdateTitle   = TRUE;
	pProInte->bUpdateMessage = TRUE;
	pProInte->bUpdatePercent = TRUE;
	size_t Tsize_Prog = sizeof(D_XL_PROGRESS_INTERFACE);
	//DLLEXPORT BOOL Set_CallProgramInfo(CString strBasePath, CString strDataPath, void* pProgress, size_t Tsize, BOOL& bVersionOK)
	CStringW strWProgramDir(szProgramDir);
	CStringW strWProjectFileDir(szProjectFileDir);
	bCheck = (*m_pRatingPgmSet)(strWProgramDir, strWProjectFileDir, pProInte, Tsize_Prog, bVersionOK);

	//size_t TSize_In   = sizeof(D_LCOM_INFO);
	size_t TSize_Opt      = sizeof(D_EXCEL_PRT_OPTION);
	size_t TSize_Rpt      = sizeof(CArray<DGNEXCEL_KR_STL_SECT, DGNEXCEL_KR_STL_SECT&>);
	size_t TSize_CsdIn    = sizeof(CArray<CSG_MEMB_POSD_KSCE_LSD, CSG_MEMB_POSD_KSCE_LSD&>);
	size_t TSize_CptLSDR  = sizeof(CArray<CSG_DRES_PRT, CSG_DRES_PRT&>);
	size_t TSize_CptKSSCR = sizeof(CArray<CSG_MEMB_RES_AASHTO_LRFD_D, CSG_MEMB_RES_AASHTO_LRFD_D&>);

	bCheck = (*m_pRatingCptDetail)(&ROpt, &aRptCsdInData, &aCptLSDRes, &aCptKSSCRes, TSize_Opt, TSize_CsdIn, TSize_CptLSDR, TSize_CptKSSCR, bVersionOK, bRegression);

	return (bVersionOK && bCheck);

}


BOOL CDgn_RatingRptManager::Print_DetailReport(CString szProgramDir, CString szProjectFileDir, CString szSaveFileName, D_LCOM_INFO& InData, DGNEXCEL_KR_STLC_OPTION &OptD, CArray<DGNEXCEL_KR_STL_SECT,  DGNEXCEL_KR_STL_SECT&>& aRptData)
{
	VERIFY(SUCCEEDED(OleInitialize(NULL))); 
	
	if(!InitialData(DGNENGINE_KR_DLL)) return FALSE; 
	
	BOOL bVersionOK = FALSE;
	BOOL bCheck = FALSE;
	
	
	CDgnEngineProgressDlg ProDlg;
	ProDlg.Create_ProgressDlg(_T(""), _T(""), FALSE);	// Stop버튼이 보여지더라도 Progress창의 콘트롤이 안됨
	D_XL_PROGRESS_INTERFACE* pProInte = ProDlg.Get_InterfacePoint();
	pProInte->bUpdateTitle   = TRUE;
	pProInte->bUpdateMessage = TRUE;
	pProInte->bUpdatePercent = TRUE;
	size_t Tsize_Prog = sizeof(D_XL_PROGRESS_INTERFACE);
	//DLLEXPORT BOOL Set_CallProgramInfo(CString strBasePath, CString strDataPath, void* pProgress, size_t Tsize, BOOL& bVersionOK)
	CStringW strWProgramDir(szProgramDir);
	CStringW strWProjectFileDir(szProjectFileDir);
	bCheck = (*m_pRatingPgmSet)(strWProgramDir, strWProjectFileDir, pProInte, Tsize_Prog, bVersionOK);
	
	size_t TSize_In   = sizeof(D_LCOM_INFO);
	size_t TSize_Opt  = sizeof(DGNEXCEL_KR_STLC_OPTION);
	size_t TSize_Rpt  = sizeof(CArray<DGNEXCEL_KR_STL_SECT, DGNEXCEL_KR_STL_SECT&>);
	
	bCheck = (*m_pRatingDetailSTL)(&OptD, &InData, &aRptData, TSize_Opt, TSize_In, TSize_Rpt,  bVersionOK);
	
	return (bVersionOK && bCheck);
}

BOOL CDgn_RatingRptManager::Print_STLRatingAASHTO(CString szProgramDir, CString szProjectFileDir, CString szSaveFileName, RAT_LRAT_RPT &LRatD)
{
	VERIFY(SUCCEEDED(OleInitialize(NULL))); 
	
	if(!InitialData(DGNENGINE_US_DLL)) return FALSE; 
	
    BOOL bCheck;

	BOOL bVersionOK = FALSE;

    size_t TSize_Opt  = sizeof(DGNEXCEL_KR_STLC_OPTION);
    size_t TSize_In   = sizeof(D_LCOM_INFO);
    size_t TSize_Rpt  = sizeof(RAT_LRAT_RPT);

    BOOL bRegression = CLockMgr::Get_IsRegressionTest(); // Regression Test인지 확인

    D_EXCEL_PRT_OPTION ROpt;
    GetRptOptionData(szProgramDir, szProjectFileDir, szSaveFileName, LRatD.nDgnCode, ROpt);

    if ( LRatD.nDgnCode == STL_AASHTO_LRFD19 )
    {
        ROpt.nCode = DGN_AASHTO_LRFD19_RATING;
    }
    else
    {
        ROpt.nCode = DGN_AASHTO_LRFD12_RATING;
    }

    CDgnEngineProgressDlg ProDlg;
    ProDlg.Create_ProgressDlg(_T(""), _T(""), FALSE);	// Stop버튼이 보여지더라도 Progress창의 콘트롤이 안됨

    D_XL_PROGRESS_INTERFACE* pProInte = ProDlg.Get_InterfacePoint();
    pProInte->bUpdateTitle   = TRUE;
    pProInte->bUpdateMessage = TRUE;
    pProInte->bUpdatePercent = TRUE;
    size_t Tsize_Prog = sizeof(D_XL_PROGRESS_INTERFACE);
    CStringW strWProgramDir(szProgramDir);
    CStringW strWProjectFileDir(szProjectFileDir);
    bCheck = (*m_pRatingPgmSet)(strWProgramDir, strWProjectFileDir, pProInte, Tsize_Prog, bVersionOK);

    int nOutType;
    BOOL bRunMS, bRowMark, bColumnMark;
    CProduct::GetTestOutType(nOutType, bRunMS, bRowMark, bColumnMark);

    // Prevent Server Busy Dialog Box
    // https://docs.microsoft.com/ko-kr/cpp/mfc/reference/colemessagefilter-class?view=vs-2019
    COleMessageFilter *pFilter = AfxOleGetMessageFilter();
    //pFilter->SetMessagePendingDelay(); // Default는 5000ms입니다.
    pFilter->EnableNotRespondingDialog(FALSE);
    pFilter->EnableBusyDialog(FALSE);

    if ( bRegression ) ROpt.strProject  = _T("");

    bCheck = (*m_pRatingDetailSTLAASHTO)(pProInte, &ROpt, &LRatD, TSize_Opt, TSize_In, TSize_Rpt,  bVersionOK);

    CString strPath = ROpt.strSaveName;

    if ( strPath.Find(_T(".txt")) == -1 ) // RT 이후 텍스트 파일 남기지 않기 위한 방법
    {
        ShellExecute(NULL, _T("open"), strPath, NULL, NULL, SW_SHOWNORMAL);
    }

    return TRUE;
}

BOOL CDgn_RatingRptManager::Print_PSCRatingAASHTO(CString szProgramDir, CString szProjectFileDir, CString szSaveFileName, RAT_LRAT_RPT &LRatD)
{
    VERIFY(SUCCEEDED(OleInitialize(NULL)));

    if ( !InitialData(DGNENGINE_US_DLL) ) return FALSE;

    BOOL bCheck;

    BOOL bVersionOK = FALSE;

    size_t TSize_Opt  = sizeof(DGNEXCEL_KR_STLC_OPTION);
    size_t TSize_In   = sizeof(D_LCOM_INFO);
    size_t TSize_Rpt  = sizeof(RAT_LRAT_RPT);

    BOOL bRegression = CLockMgr::Get_IsRegressionTest(); // Regression Test인지 확인

    D_EXCEL_PRT_OPTION ROpt;
    GetRptOptionData(szProgramDir, szProjectFileDir, szSaveFileName, LRatD.nDgnCode, ROpt);

    if ( LRatD.nDgnCode == AASHTO_LRFD19_PSC_RATING)
    {
        ROpt.nCode = DGN_AASHTO_LRFD19_PSC_RATING;
    }
    else
    {
        ROpt.nCode = DGN_AASHTO_LRFD12_PSC_RATING;
    }

    CDgnEngineProgressDlg ProDlg;
    ProDlg.Create_ProgressDlg(_T(""), _T(""), FALSE);	// Stop버튼이 보여지더라도 Progress창의 콘트롤이 안됨

    D_XL_PROGRESS_INTERFACE* pProInte = ProDlg.Get_InterfacePoint();
    pProInte->bUpdateTitle   = TRUE;
    pProInte->bUpdateMessage = TRUE;
    pProInte->bUpdatePercent = TRUE;
    size_t Tsize_Prog = sizeof(D_XL_PROGRESS_INTERFACE);
    CStringW strWProgramDir(szProgramDir);
    CStringW strWProjectFileDir(szProjectFileDir);
    bCheck = (*m_pRatingPgmSet)(strWProgramDir, strWProjectFileDir, pProInte, Tsize_Prog, bVersionOK);

    int nOutType;
    BOOL bRunMS, bRowMark, bColumnMark;
    CProduct::GetTestOutType(nOutType, bRunMS, bRowMark, bColumnMark);
    
    // Prevent Server Busy Dialog Box
    // https://docs.microsoft.com/ko-kr/cpp/mfc/reference/colemessagefilter-class?view=vs-2019
    COleMessageFilter *pFilter = AfxOleGetMessageFilter();
    //pFilter->SetMessagePendingDelay(); // Default는 5000ms입니다.
    pFilter->EnableNotRespondingDialog(FALSE);
    pFilter->EnableBusyDialog(FALSE);

    if ( bRegression ) ROpt.strProject  = _T("");

    bCheck = (*m_pRatingDetailPSCAASHTO)(pProInte, &ROpt, &LRatD, TSize_Opt, TSize_In, TSize_Rpt, bVersionOK);

    CString strPath = ROpt.strSaveName;

    if ( strPath.Find(_T(".txt")) == -1 ) // RT 이후 텍스트 파일 남기지 않기 위한 방법
    {
        ShellExecute(NULL, _T("open"), strPath, NULL, NULL, SW_SHOWNORMAL);
    }

    return TRUE;
}

// BOOL CDgn_RatingRptManager::Set_CallProgramInfo(CString strBasePath, CString strDataPath, void* pProgress, size_t Tsize, BOOL& bVersionOK)
// {
//   VERIFY(SUCCEEDED(OleInitialize(NULL))); 
//   
//   if(!InitialData(DGNENGINE_KR_DLL)) return FALSE; 
//   
//   BOOL bVersionOK = FALSE;
//   BOOL bCheck = FALSE;
//   
//   
//   CDgnEngineProgressDlg ProDlg;
//   ProDlg.Create_ProgressDlg(_T(""), _T(""), FALSE);	// Stop버튼이 보여지더라도 Progress창의 콘트롤이 안됨
//   D_XL_PROGRESS_INTERFACE* pProInte = ProDlg.Get_InterfacePoint();
//   pProInte->bUpdateTitle   = TRUE;
//   pProInte->bUpdateMessage = TRUE;
//   pProInte->bUpdatePercent = TRUE;
//   
//   size_t TSize_In   = sizeof(CArray<UMD_STL_KR_D, UMD_STL_KR_D&>);
//   size_t TSize_Opt  = sizeof(STL_KR_UMDC_STLOPTION);
//   size_t TSize_Rpt  = sizeof(CArray<STL_KR_RES_D, STL_KR_RES_D&>);
//   
//   bCheck = (*m_pRatingPgmSet)(&strBasePath, &strDataPath, &pProgress, TSize_In, bVersionOK);
//   
//   return (bVersionOK && bCheck);
// }


BOOL CDgn_RatingRptManager::Print_RC_RFReport(BOOL bBeam, CString szProgramDir, CString szProjectFileDir, CString szSaveFileName, 
																							BOOL bPrintParm[3], DGNEXCEL_KR_RCSC_OPTION& OptD,  D_LCOM_INFO& LoadD, 
																							CArray<DGNEXCEL_KR_BEAM_SECT, DGNEXCEL_KR_BEAM_SECT&>& arRptResD,
																							CArray<RF_RES_RF_KSCE_RC_D, RF_RES_RF_KSCE_RC_D&>& aRptData,
																							CArray<_UMD_RC_BEAM_KCI, _UMD_RC_BEAM_KCI&>& arStrInD,
																							CArray<_UMD_RC_KR_BEAM_RES, _UMD_RC_KR_BEAM_RES&>& arStrResD)
{

 // VERIFY(SUCCEEDED(OleInitialize(NULL))); 

	if(!InitialData(DGNENGINE_KR_DLL)) return FALSE; 

	BOOL bVersionOK = FALSE;
	BOOL bCheck = FALSE;


	CDgnEngineProgressDlg ProDlg;
	ProDlg.Create_ProgressDlg(_T(""), _T(""), FALSE);	// Stop버튼이 보여지더라도 Progress창의 콘트롤이 안됨
	D_XL_PROGRESS_INTERFACE* pProInte = ProDlg.Get_InterfacePoint();
	pProInte->bUpdateTitle   = TRUE;
	pProInte->bUpdateMessage = TRUE;
	pProInte->bUpdatePercent = TRUE;
	size_t Tsize_Prog = sizeof(D_XL_PROGRESS_INTERFACE);
	//DLLEXPORT BOOL Set_CallProgramInfo(CString strBasePath, CString strDataPath, void* pProgress, size_t Tsize, BOOL& bVersionOK)

	CStringW cwszProgramDir(szProgramDir);
	CStringW cwszProjectDir(szProjectFileDir);
	CStringW cwszSaveFileName(szSaveFileName);

	bCheck = (*m_pRatingPgmSet)(cwszProgramDir, cwszProjectDir, pProInte, Tsize_Prog, bVersionOK);

//   CString strBasePath = m_pDoc->GetProgramPath();
//   CString strDataPath = m_pDoc->GetPathName();
//   CString szSaveFileName = GetSaveFileNameEx();
	RF_RPAC_D RPAC;
	for(int i=0; i<3; ++i)
	{
		RPAC.bPrintParm[i] = bPrintParm[i];
	}

	CString szProjectDir =  szProgramDir;
	CStringW szwProjectDir(szProjectDir);
	BOOL bRegression =  CLockMgr::Get_IsRegressionTest();

	size_t SizeIn_RpacD        = sizeof(RPAC);
	size_t SizeIn_OptD         = sizeof(OptD);
	size_t SizeIn_LcomD        = sizeof(LoadD); 
	size_t SizeIn_StrengthRptD = sizeof(arRptResD);
	size_t SizeIn_RatingRptD   = sizeof(aRptData);
	size_t SizeIn_StrInD       = sizeof(arStrInD);
	size_t SizeIn_StrResD      = sizeof(arStrResD);

	bVersionOK = (*m_pRatingRF_RC)(bBeam, &RPAC, &OptD, &LoadD, &arRptResD, &aRptData, &arStrInD, &arStrResD,
																SizeIn_RpacD, SizeIn_OptD, SizeIn_LcomD, SizeIn_StrengthRptD, SizeIn_RatingRptD, SizeIn_StrInD, SizeIn_StrResD, bVersionOK, bRegression);

	return (bVersionOK && bCheck);
}
