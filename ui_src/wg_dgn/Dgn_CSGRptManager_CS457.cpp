//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Dgn_CSGRptManager_CS457.h"
//#include "DgnEngineProgressDlg.h"
#include "..\wg_base\wg_base_MsgDll.h"
//#include "..\dgnengine\src\DgnReportBase\XLStruct.h"
#include "..\..\DgnEngine\Src\DgnCalcBase\DgnCalcBase_PSC_Struct.h"
#include "..\..\DgnEngine\idesign\DGN_lib\IExcel.h"

#include "DgnPscCommon.h"
#include "..\wg_dbLock\LockMgr.h"

using namespace dgn::lib;

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDgn_CSGRptManager_CS457::CDgn_CSGRptManager_CS457() : CDgn_DllManager_Base()
{
    m_pCS457Detail  = NULL;
}

CDgn_CSGRptManager_CS457::~CDgn_CSGRptManager_CS457()
{

}

BOOL CDgn_CSGRptManager_CS457::InitialData(int iNationType)
{
    HINSTANCE hDllCalc = GetOrLoad_ReportDllHandle(iNationType);
    if ( hDllCalc == NULL ) return FALSE;

    if ( iNationType != DGNENGINE_US_DLL )
    {
        m_pCS457Detail = NULL;
    }
    else
    {
        m_pCS457Detail = (PDGNENGINE_RPT_PROG*)GetProcAddress(hDllCalc, "Print_CSGDetailReport_CS457");
        if ( !m_pCS457Detail ) return FALSE;
    }

    return TRUE;
}

BOOL CDgn_CSGRptManager_CS457::Print_DetailReport(IExcel* pExcel, D_XL_PROGRESS_INTERFACE* pProgDlg, int iDgnCode, LPCTSTR szProgramDir, LPCTSTR szProjectFileDir, LPCTSTR szSaveFileName, CSG_RPT_CS457_D &rData)
{
    if ( !InitialData(DGNENGINE_US_DLL) ) return FALSE;

    BOOL bVersionOK = FALSE;
    BOOL bTemp = FALSE;

    size_t TSize_ROpt = sizeof(D_EXCEL_PRT_OPTION);
    size_t TSize_In   = 0;
    size_t TSize_Rpt  = sizeof(CSG_RPT_CS457_D);


    CDgnPscCommon PscCommon;
    int nDgnCode = PscCommon.GetConvertCSGCodeCvlToEng(iDgnCode);

    rData.nDgnCode = nDgnCode;

    BOOL bRegression = CLockMgr::Get_IsRegressionTest(); // Regression Test인지 확인

    D_EXCEL_PRT_OPTION ROpt;
    GetRptOptionData(szProgramDir, szProjectFileDir, szSaveFileName, nDgnCode, ROpt);
    if ( bRegression ) ROpt.strProject  = _T("");
    if ( bRegression ) ROpt.strSaveName  = szSaveFileName;

    BOOL bCheck = (*m_pCS457Detail)(nDgnCode, pExcel, pProgDlg, &ROpt, &rData, TSize_ROpt, TSize_In, TSize_Rpt, bVersionOK);

    return TRUE;
}
