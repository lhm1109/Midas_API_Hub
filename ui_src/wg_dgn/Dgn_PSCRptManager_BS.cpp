// Dgn_PSCRptManager_IRS.cpp: implementation of the CDgn_PSCRptManager_BS class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "..\DgnEngine\Src\DgnCalcBase\DgnCalcBase_PSC_Struct.h"
#include "..\DgnEngine\Src\DgnCalcBase\DgnCalcBase_PSC_ResultStruct.h"
#include "..\DgnEngine\Src\CVL_DgnCalc_US\DgnCalc_US_PSC_Struct.h"
#include "Dgn_PSCRptManager_BS.h"

//Dgn_PSCRptManager.cpp 유용.
#include "DgnEurocode2_2_05.h"
#include "Dgn_PSCManager.h"

#include "..\wg_db\AttrCtrl.h"
#include "..\wg_db\PostCtrl.h"
#include "..\wg_db\PCDesign.h"
#include "..\wg_base\MSExcel.h"
#include "DgnEngineProgressDlg.h"

//#include "..\dgnengine\src\DgnReportBase\XLStruct.h"
#include "..\DgnEngine\idesign\DGN_lib\IExcel.h"
#include "..\wg_dbLock\LockMgr.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDgn_PSCRptManager_BS::CDgn_PSCRptManager_BS()
    : m_bInitialized(FALSE)
{
    m_pDetail        = NULL;
}

CDgn_PSCRptManager_BS::~CDgn_PSCRptManager_BS()
{

}

BOOL CDgn_PSCRptManager_BS::InitialData(int iNationType)
{
    HINSTANCE hDllCalc = GetOrLoad_ReportDllHandle(iNationType);
    if ( hDllCalc == NULL ) return FALSE;

    if ( iNationType != DGNENGINE_US_DLL )
    {
        m_pDetail = NULL;
    }
    else
    {
        m_pDetail = (PDGNENGINE_RPT_PROG*)GetProcAddress(hDllCalc, "Print_PSCDetailReport_BS");
        if ( !m_pDetail ) return FALSE;

    }

    return TRUE;
}

BOOL CDgn_PSCRptManager_BS::Print_DetailReport_BS(int iDgnCode, LPCTSTR szProgramDir, LPCTSTR szProjectFileDir, LPCTSTR szSaveFileName, PSC_RPT_BS_D &rData)
{
    if ( !InitialData(DGNENGINE_US_DLL) ) return FALSE;

    BOOL bVersionOK = FALSE;

    size_t TSize_ROpt = sizeof(D_EXCEL_PRT_OPTION);
    size_t TSize_In   = sizeof(CArray<PSC_DATA_POSI, PSC_DATA_POSI&>);
    size_t TSize_Rpt  = sizeof(PSC_RPT_BS_D);

    CDgnPscCommon PscCommon;
    int nDgnCode = PscCommon.GetConvertCodeCvlToEng(iDgnCode);

    rData.nDgnCode = nDgnCode;

    BOOL bRegression = CLockMgr::Get_IsRegressionTest(); // Regression Test인지 확인

    D_EXCEL_PRT_OPTION ROpt;
    GetRptOptionData(szProgramDir, szProjectFileDir, szSaveFileName, nDgnCode, ROpt);

    CDgnEngineProgressDlg ProDlg;
    ProDlg.Create_ProgressDlg(PROGRESS_TYPE_PSC_DGN_XL_RPT);
        
    D_XL_PROGRESS_INTERFACE* pProInte = ProDlg.Get_InterfacePoint();
    pProInte->bUpdateTitle   = TRUE;
    pProInte->bUpdateMessage = TRUE;
    pProInte->bUpdatePercent = TRUE;
    pProInte->iTotalStep     = rData.aRptD.GetSize();

    //int nOutType;
    //BOOL bRunMS, bRowMark, bColumnMark;
    //CProduct::GetTestOutType(nOutType, bRunMS, bRowMark, bColumnMark);
    //CMSExcel* pExcel = new CMSExcel(nOutType, bRunMS, bRowMark, bColumnMark);

	//CString strBaseBook = _T("PSC_CS455_Report_base.xlsx");
    //CString strBaseBook = _T("PSC_CS455_Report_base.xls");

    // Prevent Server Busy Dialog Box
    // https://docs.microsoft.com/ko-kr/cpp/mfc/reference/colemessagefilter-class?view=vs-2019
    COleMessageFilter *pFilter = AfxOleGetMessageFilter();
    //pFilter->SetMessagePendingDelay(); // Default는 5000ms입니다.
    pFilter->EnableNotRespondingDialog(FALSE);
    pFilter->EnableBusyDialog(FALSE);

// 엔진에서 호출하도록 변경
//     pExcel->Open(m_pDoc->GetProgramPath() + strBaseBook);
// 
// 
// #if _DEBUG
//     pExcel->SetVisible(TRUE);
// #else
//     pExcel->SetVisible(FALSE);// Set Not_Visible
// #endif

    if ( bRegression ) ROpt.strProject  = _T("");

    dgn::lib::IExcel* pExcel = nullptr;
    BOOL bCheck = (*m_pDetail)(nDgnCode, pExcel, pProInte, &ROpt, &rData, TSize_ROpt, TSize_In, TSize_Rpt, bVersionOK);

    CString strPath = ROpt.strProject + ROpt.strSaveName;

    if ( strPath.Find(_T(".txt")) == -1 ) // RT 이후 텍스트 파일 남기지 않기 위한 방법
    {
        ShellExecute(NULL, _T("open"), strPath, NULL, NULL, SW_SHOWNORMAL);
    }

    return TRUE;

}