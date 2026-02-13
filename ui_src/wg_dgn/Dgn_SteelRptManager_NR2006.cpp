//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Dgn_SteelRptManager_NR2006.h"

#include "..\wg_base\wg_base_MsgDll.h"

#include "..\..\DgnEngine\Src\DgnCalcBase\DgnCalcBase_CSG_Struct.h"
#include "..\..\DgnEngine\Src\DgnCalcBase\DgnCalcBase_PSC_Struct.h"
#include "..\..\dgnengine\src\CVL_DgnCalc_US\DgnCalc_CSG_CS457_Struct.h"

#include "DgnPscCommon.h"
#include "..\wg_dbLock\LockMgr.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

using namespace dgn::lib;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDgn_SteelRptManager_NR2006::CDgn_SteelRptManager_NR2006() : CDgn_DllManager_Base()
{
	m_pNR2006Detail = nullptr;
}

CDgn_SteelRptManager_NR2006::~CDgn_SteelRptManager_NR2006()
{

}

BOOL CDgn_SteelRptManager_NR2006::InitialData(int iNationType)
{
	HINSTANCE hDllCalc = GetOrLoad_ReportDllHandle(iNationType);
	if(hDllCalc == nullptr) return FALSE;

	if(iNationType != DGNENGINE_US_DLL)
	{
		m_pNR2006Detail = nullptr;
	}
	else
	{
		m_pNR2006Detail = (PDGNENGINE_RPT_PROG*)GetProcAddress(hDllCalc, "Print_Rating_StlDetailReport_NR2006");
		if(!m_pNR2006Detail) return FALSE;
	}

	return TRUE;
}

BOOL CDgn_SteelRptManager_NR2006::Print_DetailReport(IExcel* pExcel, D_XL_PROGRESS_INTERFACE* pProgDlg, int iDgnCode, LPCTSTR szProgramDir, LPCTSTR szProjectFileDir, LPCTSTR szSaveFileName, CSG_RPT_CS457_D& rData)
{
	if(!InitialData(DGNENGINE_US_DLL)) return FALSE;

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
	if(bRegression) ROpt.strProject  = _T("");
	if(bRegression) ROpt.strSaveName  = szSaveFileName;

	BOOL bCheck = (*m_pNR2006Detail)(nDgnCode, pExcel, pProgDlg, &ROpt, &rData, TSize_ROpt, TSize_In, TSize_Rpt, bVersionOK);
	
	return TRUE;
}
