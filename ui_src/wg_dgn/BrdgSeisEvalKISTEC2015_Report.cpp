#include "stdafx.h"

#include "BrdgSeisEvalKISTEC2015_Report.h"

#include "..\wg_base\Product.h"
#include "..\wg_base\wg_base_MsgDll.h"
#include "..\wg_dgn\DgnProgressDlg.h"

#include "..\wg_db\BrdgSeisEvalResult.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl2.h"
#include "..\wg_db\wg_db_PostCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_MatlDB.h"
#include "..\wg_db\wg_db_MathFunc.h"

#include "..\wg_db\wg_db_AnalysisResult.h"
#include "..\wg_db\VehlDB.h"
#include "..\wg_base\PL_StrParser.h"
#include "..\wg_cmd\CMMvcdDlg.h"

#include "..\wg_main\wg_main.h" 
#include "..\wg_base\ServiceProvider.h" 
#include "..\wg_gps\GPS_ServiceDef.h" 
#include "..\wg_base\TestEnvMgr.h"
#include "..\wg_base\wg_base_ArrayUtil.h"
#include "..\..\DgnEngine\idesign\DGN_lib\ExcelGenerator.h"

#include <thread>

#if _MSC_VER >= 1700  // vc++11.0 //sjhuh-2014-02-21
using namespace std;
#include "..\MIT_Lib\rptostream.h"	  // for rptwofstream
#else
#include <fstream.h>	// for rptwofstream
#endif


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

using namespace dgn::lib;

CBrdgSeisEvalKISTEC2015_Report::CBrdgSeisEvalKISTEC2015_Report(void) : CDgn_XLOut()
{
	m_pDoc = CDBDoc::GetDocPoint();
	for (int i = 0; i < 8; ++i)
	{
		m_bPrintItems[i] = TRUE;
	}

	SetSeisEval(true);
}

CBrdgSeisEvalKISTEC2015_Report::~CBrdgSeisEvalKISTEC2015_Report(void)
{

}

void CBrdgSeisEvalKISTEC2015_Report::Print_SeisEval(CString strPath, int iPrintOpt)
{
	// <Remember> Get Data by Code Units.
	CCurUnitSaver UnitSaver(TRUE);
	T_UNIT_INDEX CurIndex, CngIndex;
	m_pDoc->m_pUnitCtrl->GetUnitIndexCurrent(CurIndex);
	CngIndex.nBase_Length = D_UNITSYS_LENGTH_INDEX_M;
	CngIndex.nBase_Force = D_UNITSYS_FORCE_INDEX_KN;
	m_pDoc->m_pUnitCtrl->SetUnitIndexCurrent(CngIndex);
	//++++++++++++++++++++++++++++++++++++++++++++

	int nEvgp = m_pDoc->m_pAttrCtrl2->GetCountEvgp();
	if (nEvgp == 0)
	{
		AfxMessageBox(_T("[Error] 평가그룹(Evaluation Group)이 정의되어 있지 않습니다."));
		return;
	}

	arPairKey arEvgpColm; // Evgp Key and 다주식 Column Index
	CArray<UINT, UINT> arEvgpBearing;
	CArray<UINT, UINT> aBrev;
	INT_PTR nEvgpCol = GetPrintList(arEvgpColm);
	INT_PTR nEvgpBearing = GetBearingPrintList(arEvgpBearing, aBrev);
	if (nEvgpCol == 0 && nEvgpBearing == 0)
	{
		AfxMessageBox(_T("[Error] 내진성능평가 결과가 없습니다."));
		return;
	}

	CDgnProgressDlg ProDlg;
	ProDlg.Create_ProgressDlg(PROGRESS_TYPE_BRDG_SEIS_EVAL_XL_RPT);// PSC Rating XL Report -> 202
	int nProgressCount = 1;
	int nProgressTotal = 0;
	// #1.표지 Tab
	if (m_bPrintItems[0]) nProgressTotal += 1;
	// #2.결과요약 Tab
	if (m_bPrintItems[1]) nProgressTotal += 1;
	// #3.교량현황 Tab
	if (m_bPrintItems[2]) nProgressTotal += (1 + nEvgp * 2 + 1);
	// #4.구조해석 Tab
	if (m_bPrintItems[3]) nProgressTotal += (nEvgpCol * 2);
	// #5.탄성지진력 Tab
	if (m_bPrintItems[4]) nProgressTotal += 1;
	// #6.복합모드 Tab
	if (m_bPrintItems[5]) nProgressTotal += (nEvgpCol);
	// #7.검토조건 Tab, #8.내진성능평가 Tab
	if (m_bPrintItems[6]) nProgressTotal += (nEvgp + nEvgpCol * 6);
	// Bearing
	if (m_bPrintItems[7])
	{
		nProgressTotal += (nEvgpBearing * 5 + 2);

		T_BSED_D BsedD;
		if (!m_pDoc->m_pAttrCtrl2->GetDgnBsed(BsedD)) ASSERT(0);
		if (BsedD.nDesignCode == KISTEC_2019)     nProgressTotal += (5 * nEvgpBearing);
		else if (BsedD.nDesignCode == KALIS_2023) nProgressTotal += (5 * nEvgpBearing);
	}
	if (nProgressTotal == 0) return;

	// Prevent Server Busy Dialog Box
	// https://docs.microsoft.com/ko-kr/cpp/mfc/reference/colemessagefilter-class?view=vs-2019
	COleMessageFilter *pFilter = AfxOleGetMessageFilter();
	//pFilter->SetMessagePendingDelay(); // Default는 5000ms입니다. 
	pFilter->EnableNotRespondingDialog(FALSE);
	pFilter->EnableBusyDialog(FALSE);

	IExcel* pXL = SetExcel();
	auto l_delete_XL = [&]()
	{
		if(pXL != nullptr)
		{
			delete pXL;
			pXL = nullptr;
		}
	};

	int iSheetIdx = 0;

	// #1.표지 Tab
	if (m_bPrintItems[0])
	{
		if(!Print_CoverTab(pXL, &ProDlg, nProgressCount, nProgressTotal)) {
			l_delete_XL();  return;
		}
	}
	// #2.결과요약 Tab
	if (m_bPrintItems[1])
	{
		if(!Print_SummaryTab(pXL, &ProDlg, nProgressCount, nProgressTotal, iSheetIdx, arEvgpColm, arEvgpBearing, aBrev))  {
			l_delete_XL();  return;
		}
	}
	if (iSheetIdx == 0) iSheetIdx++;
	// #3.교량현황 Tab
	if (m_bPrintItems[2])
	{
		if(!Print_BrdgStateTab(pXL, &ProDlg, nProgressCount, nProgressTotal, iSheetIdx, arEvgpColm, arEvgpBearing))  {
			l_delete_XL();  return;
		}
	}
	// #4.구조해석 Tab
	if (m_bPrintItems[3])
	{
		if(!Print_AnalResTab(pXL, &ProDlg, nProgressCount, nProgressTotal, iSheetIdx, arEvgpColm))  {
			l_delete_XL();  return;
		}
	}

	//////////////////////////////////////////////////////////////////////////
	int nLoadType = 0;
	int nEvgpColCal = arEvgpColm.GetSize();
	for (int i = 0; i < nEvgpColCal; ++i)
	{
		PairKey pEvgpCol = arEvgpColm[0];
		T_BRDGEVAL_RESULT ResultD[2][2]; // Bottom/Top, Longitude/Transverse
		if (!GetResult(pEvgpCol, ResultD)) ASSERT(0);

		nLoadType = ResultD[0][0].ParamD.ForceD.nLoadType; // 0: RS  1; Time History
	}
	//////////////////////////////////////////////////////////////////////////

	// #5.탄성지진력 Tab
	if (m_bPrintItems[4])
	{
		if(!Print_ElSuppTab(pXL, &ProDlg, nProgressCount, nProgressTotal, iSheetIdx, nLoadType))  {
			l_delete_XL();  return;
		}
	}
	// #6.복합모드 Tab
	if (m_bPrintItems[5])
	{
		if (nEvgpColCal > 0)
		{
			if(!Print_ComplexModeTab(pXL, &ProDlg, nProgressCount, nProgressTotal, iSheetIdx, arEvgpColm, nLoadType))  {
				l_delete_XL();  return;
			}
		}
	}
	// #7.검토조건 Tab
	// #8.내진성능평가 Tab
	if (m_bPrintItems[6])
	{
		if(!Print_CndtNResTab(pXL, &ProDlg, nProgressCount, nProgressTotal, iSheetIdx, arEvgpColm))  {
			l_delete_XL();  return;
		}
	}
	// Bearing
	if (m_bPrintItems[7])
	{
		if (nEvgpBearing > 0)
		{
			if(!Print_BearingTab(pXL, &ProDlg, nProgressCount, nProgressTotal, iSheetIdx, arEvgpBearing, aBrev))  {
				l_delete_XL();  return;
			}
			if(!Print_BearingSuppLengTab(pXL, &ProDlg, nProgressCount, nProgressTotal, iSheetIdx, arEvgpBearing, aBrev))  {
				l_delete_XL();  return;
			}
		}
	}

	CDBLib::DeleteDirectoryFile(m_strPicPath);
	DelDefaultSheetPage(pXL);
	if(m_arDetailSheetName.GetSize() > 0)
	{
		pXL->SetActiveSheet(m_arDetailSheetName[0]);
	}
	pXL->SaveAs(pXL->GetBookName(), strPath);//* 출력파일 저쟝..
	pXL->Quit();
	
	l_delete_XL();

	ProDlg.Delete_ProgressDlg();

	if (strPath.Find(_T(".txt")) == -1) // RT 이후 텍스트 파일 남기지 않기 위한 방법
	{
		ShellExecute(NULL, _T("open"), strPath, NULL, NULL, SW_SHOWNORMAL);
	}
	// delete Picture folder
	if (::GetFileAttributes(m_strPicPath) != -1)
	{
		RemoveDirectory(m_strPicPath);
	}
}

IExcel* CBrdgSeisEvalKISTEC2015_Report::SetExcel()
{
	T_BSED_D BsedD;
	if(!m_pDoc->m_pAttrCtrl2->GetDgnBsed(BsedD)) ASSERT(0);

	m_strBasePath = m_pDoc->GetProgramPath() + _T("Excel Base File\\");
	if(BsedD.nDesignCode == KALIS_2023)
		m_strBaseBook = _T("CivilEarthquakeReport_2023.xlsx");
	else
		m_strBaseBook = _T("CivilEarthquakeReport.xlsx");

	// Set Picture Path.
	CString strDataPath = m_pDoc->GetPathName();
	CFileCtrl FileCtrl(strDataPath);
	m_strPicPath = FileCtrl.GetFilePath() + FileCtrl.GetFileNameWithoutExtension() + _T("_image\\");

	if (::GetFileAttributes(m_strPicPath) == -1)
	{
		CreateDirectory(m_strPicPath, NULL);
	}

	CExcelGenerator Excel(_T(""), XLTYPE::TYPE_LIBXL);
	CString strFileName = Excel.GetDLLFilePath(m_strBaseBook, FALSE);

	IExcel* pXL = Excel.Generate(strFileName);
	if (Excel.GetExcelType() == XLTYPE::TYPE_LIBXL)
	{
		m_strBaseBook = strFileName; // 경로 포함.
	}

	SetOutputUnitSystem();
	SetUnitConvertFactor();

	return pXL;
}

BOOL CBrdgSeisEvalKISTEC2015_Report::Print_CoverTab(IExcel* pXL, CDgnProgressDlg* pProDlg, int& nProCnt, const int& nProTot)
{
	// #1.표지 Tab 시작
	SetCoverSheetPage(pXL);
	
	int stRow = 0;
	CString RptBlock = _T("[0]Cover_Page");
	CopyRange(pXL, RptBlock, stRow);

	if (pProDlg->Update_ProgressDlg(nProCnt++, nProTot)) { pXL->Quit(); return FALSE; }
	pXL->SetHPageBreak(53, 46, 28);

	return TRUE;
}

BOOL CBrdgSeisEvalKISTEC2015_Report::Print_SummaryTab(IExcel* pXL, CDgnProgressDlg* pProDlg, int& nProCnt, const int& nProTot,
	int& iSheetIdx, arPairKey& arEvgpColm, CArray<UINT, UINT>& arEvgpBearing, CArray<UINT, UINT>& arBrev)
{
	struct PrtData
	{
		CArray<UINT, UINT> arColm;
		BOOL bBearing;

		void init()
		{
			arColm.RemoveAll();
			bBearing = FALSE;
		}

		PrtData& operator=(PrtData& src)
		{
			arColm.Copy(src.arColm);
			bBearing = src.bBearing;
			return *this;
		}
	};
	CMapEx<UINT, UINT, PrtData, PrtData&> mPrtKeys;
	mPrtKeys.RemoveAll();

	INT_PTR nEvgpCol = arEvgpColm.GetSize();
	for (INT_PTR i = 0; i < nEvgpCol; ++i)
	{
		PairKey pEvgpCol = arEvgpColm[i];

		PrtData PrtD;
		if (!mPrtKeys.Lookup(pEvgpCol.first, PrtD)) PrtD.init();
		PrtD.arColm.Add(pEvgpCol.second);
		mPrtKeys.SetAt(pEvgpCol.first, PrtD);
	}

	INT_PTR nEvgpBearing = arEvgpBearing.GetSize();
	INT_PTR nBearing = arBrev.GetSize();
	if (nEvgpBearing == nBearing)
	{
		for (INT_PTR i = 0; i < nBearing; ++i)
		{
			PrtData PrtD;
			if (!mPrtKeys.Lookup(arEvgpBearing[i], PrtD)) PrtD.init();
			PrtD.bBearing = TRUE;
			mPrtKeys.SetAt(arEvgpBearing[i], PrtD);
		}
	}

	CArray<UINT, UINT> arEvgp;
	mPrtKeys.GetKeyList(arEvgp);
	CArrayUtil::Sort(arEvgp);

	// #2.결과요약 Tab 시작 
	SetSummarySheetPage(pXL, iSheetIdx);
	int stRow = 1;
	GenerateSummarySheet_Info(pXL, stRow);

	INT_PTR nEvgp = arEvgp.GetSize();
	for (INT_PTR i = 0; i < nEvgp; ++i)
	{
		PrtData PrtD;
		if (!mPrtKeys.Lookup(arEvgp[i], PrtD)) continue;

		T_EVGP_D EvgpD;
		if (!m_pDoc->m_pAttrCtrl2->GetEvgp(arEvgp[i], EvgpD)) ASSERT(0);

		// Set Head
		GenerateSummaryPierTableHead(pXL, stRow, EvgpD.strName);

		INT_PTR nColm = PrtD.arColm.GetSize();
		for (INT_PTR j = 0; j < nColm; ++j)
		{
			PairKey pEvgpCol(arEvgp[i], PrtD.arColm[j]);
			CString strColm = EvgpD.aEvgpColm[PrtD.arColm[j]].strName;
			GenerateSummaryPierTableBody(pXL, stRow, pEvgpCol, strColm);
		}

		if (PrtD.bBearing)
		{
			for (INT_PTR j = 0; j < nEvgpBearing; ++j)
			{
				if (arEvgp[i] == arEvgpBearing[j])
				{
					GenerateSummaryBearingTable(pXL, stRow, arEvgp[i], arBrev[j]);
				}
			}
		}
		stRow++;
	}
	if (pProDlg->Update_ProgressDlg(nProCnt++, nProTot)) { pXL->Quit(); return FALSE; }

	pXL->SetHPageBreak(53, stRow, 28);

	return TRUE;
}

BOOL CBrdgSeisEvalKISTEC2015_Report::Print_BrdgStateTab(IExcel* pXL, CDgnProgressDlg* pProDlg, int& nProCnt, const int& nProTot,
	int& iSheetIdx, arPairKey& arEvgpColm, CArray<UINT, UINT>& arEvgpBearing)
{
	struct PrtData
	{
		double dfck;
		double dfy;
		double dEcs;
		double dEsr;

		PrtData()
		{
			dfck = 0.0;
			dfy = 0.0;
			dEcs = 0.0;
			dEsr = 0.0;
		}
		PrtData(double dVal1, double dVal2, double dVal3, double dVal4)
		{
			dfck = dVal1;
			dfy = dVal2;
			dEcs = dVal3;
			dEsr = dVal4;
		}
	};
	CMapEx<UINT, UINT, PrtData, PrtData&> mPrtPropD;
	mPrtPropD.RemoveAll();

	// #3.교량현황 Tab 시작 
	SetBrdgInfoSheetPage(pXL, iSheetIdx);

	T_SBRI_D BrdgInfo;
	if (!m_pDoc->m_pAttrCtrl2->GetSbri(BrdgInfo)) BrdgInfo.Initialize();

	int stRow = 1;
	GenerateBrdgInfoSheet(pXL, BrdgInfo, stRow);
	if (pProDlg->Update_ProgressDlg(nProCnt++, nProTot)) { pXL->Quit(); return FALSE; }

	int nEvgpCol = arEvgpColm.GetSize();
	UINT BeforeEvgpK = 0;
	for (int i = 0; i < nEvgpCol; ++i)
	{
		PairKey pEvgpCol = arEvgpColm[i];
		if (BeforeEvgpK == pEvgpCol.first) continue;

		T_EVGP_D EvgpD;
		if (!m_pDoc->m_pAttrCtrl2->GetEvgp(pEvgpCol.first, EvgpD)) ASSERT(0);

		T_MPCC_K MpccK = GetMpccKey(pEvgpCol.first, pEvgpCol.second, BOTTOM);
		if (MpccK == 0) MpccK = GetMpccKey(pEvgpCol.first, pEvgpCol.second, TOP);
		T_MPCC_D MpccD;
		if(MpccK == 0) MpccD.nEvgpPos = BOTTOM;
		else
		{
			if(!m_pDoc->m_pAttrCtrl2->GetMpcc(MpccK, MpccD)) ASSERT(0);
		}

		double dFck = Get_fck(pEvgpCol.first, pEvgpCol.second, (MpccD.nEvgpPos == 0 ? TOP : BOTTOM), LONGI);
		double dFy = Get_fyh(pEvgpCol.first, pEvgpCol.second, (MpccD.nEvgpPos == 0 ? TOP : BOTTOM));
		if (dFck < cDgn_Zero || dFy < cDgn_Zero)
		{
			T_ELEM_K ElemK = GetEvgpElemK(pEvgpCol.first, pEvgpCol.second, MpccD.nEvgpPos, EvgpD);

			T_MATD_D MatlD;
			if (!GetMatlData(ElemK, MatlD)) ASSERT(0);

			T_MATL_REBAR MatlRbarD;
			MatlRbarD.Initialize();
			if (!m_pDoc->m_pMatlDB->GetRebarData(MatlD.Rebar_CodeName, MatlD.MainRebar_RebarName, MatlRbarD))  ASSERT(0);

			if (dFck < cDgn_Zero) dFck = MatlD.Data1.Design.C_fc;
			if (dFy < cDgn_Zero) dFy = MatlRbarD.B_fy;
		}
		double dEc = Get_Ec(pEvgpCol.first, pEvgpCol.second, (MpccD.nEvgpPos == 0 ? TOP : BOTTOM), LONGI);
		double dEs = Get_Es(pEvgpCol.first, pEvgpCol.second, (MpccD.nEvgpPos == 0 ? TOP : BOTTOM));
		if (dEc < cDgn_Zero || dEs < cDgn_Zero)
		{
			T_ELEM_K ElemK = GetEvgpElemK(pEvgpCol.first, pEvgpCol.second, MpccD.nEvgpPos, EvgpD);

			T_MATD_D MatlD;
			MatlD.Initialize();
			if (!GetMatlData(ElemK, MatlD)) ASSERT(0);

			if (dEc < cDgn_Zero) dEc = MatlD.Data1.Analysis.Elast;
			if (dEs < cDgn_Zero)
			{
				T_MATL_REBAR MatlRbarD;
				MatlRbarD.Initialize();
				if (!m_pDoc->m_pMatlDB->GetRebarData(MatlD.Rebar_CodeName, MatlD.MainRebar_RebarName, MatlRbarD)) ASSERT(0);
				dEs = MatlRbarD.B_Elast;
			}
		}

		dFck = m_pDoc->m_pUnitCtrl->ConvertCurUnitData2TgtUnit(D_UNITSYS_FORCE_INDEX_N, D_UNITSYS_LENGTH_INDEX_MM, D_UNITSYS_BASE_STRESS, dFck);
		dFy = m_pDoc->m_pUnitCtrl->ConvertCurUnitData2TgtUnit(D_UNITSYS_FORCE_INDEX_N, D_UNITSYS_LENGTH_INDEX_MM, D_UNITSYS_BASE_STRESS, dFy);
		dEc = m_pDoc->m_pUnitCtrl->ConvertCurUnitData2TgtUnit(D_UNITSYS_FORCE_INDEX_N, D_UNITSYS_LENGTH_INDEX_MM, D_UNITSYS_BASE_STRESS, dEc);
		dEs = m_pDoc->m_pUnitCtrl->ConvertCurUnitData2TgtUnit(D_UNITSYS_FORCE_INDEX_N, D_UNITSYS_LENGTH_INDEX_MM, D_UNITSYS_BASE_STRESS, dEs);

		PrtData PrtD(dFck, dFy, dEc, dEs);
		mPrtPropD.SetAt(pEvgpCol.first, PrtD);
	}

	int nBearing = arEvgpBearing.GetSize();
	for (int i = 0; i < nBearing; ++i)
	{
		PrtData PrtD;
		if (mPrtPropD.Lookup(arEvgpBearing[i], PrtD)) continue;

		T_EVGP_D EvgpD;
		if (!m_pDoc->m_pAttrCtrl2->GetEvgp(arEvgpBearing[i], EvgpD)) ASSERT(0);
		if (!EvgpD.bPierCap) continue;
		if (EvgpD.aPierCap.GetSize() == 0) continue;

		T_ELEM_K ElemK = EvgpD.aPierCap[0];

		T_MATD_D MatlD;
		if (!GetMatlData(ElemK, MatlD)) ASSERT(0);

		T_MATL_REBAR MatlRbarD;
		MatlRbarD.Initialize();
		if (!m_pDoc->m_pMatlDB->GetRebarData(MatlD.Rebar_CodeName, MatlD.MainRebar_RebarName, MatlRbarD))  ASSERT(0);

		PrtD.dfck = MatlD.Data1.Design.C_fc;
		PrtD.dfy = MatlRbarD.B_fy;
		PrtD.dEcs = MatlD.Data1.Analysis.Elast;
		PrtD.dEsr = MatlRbarD.B_Elast;

		PrtD.dfck = m_pDoc->m_pUnitCtrl->ConvertCurUnitData2TgtUnit(D_UNITSYS_FORCE_INDEX_N, D_UNITSYS_LENGTH_INDEX_MM, D_UNITSYS_BASE_STRESS, PrtD.dfck);
		PrtD.dfy = m_pDoc->m_pUnitCtrl->ConvertCurUnitData2TgtUnit(D_UNITSYS_FORCE_INDEX_N, D_UNITSYS_LENGTH_INDEX_MM, D_UNITSYS_BASE_STRESS, PrtD.dfy);
		PrtD.dEcs = m_pDoc->m_pUnitCtrl->ConvertCurUnitData2TgtUnit(D_UNITSYS_FORCE_INDEX_N, D_UNITSYS_LENGTH_INDEX_MM, D_UNITSYS_BASE_STRESS, PrtD.dEcs);
		PrtD.dEsr = m_pDoc->m_pUnitCtrl->ConvertCurUnitData2TgtUnit(D_UNITSYS_FORCE_INDEX_N, D_UNITSYS_LENGTH_INDEX_MM, D_UNITSYS_BASE_STRESS, PrtD.dEsr);

		mPrtPropD.SetAt(arEvgpBearing[i], PrtD);
	}

	CArray<UINT, UINT> arEvgp;
	mPrtPropD.GetKeyList(arEvgp);
	CArrayUtil::Sort(arEvgp);
	int nEvgp = arEvgp.GetSize();

	for (int i = 0; i < nEvgp; ++i)
	{
		PrtData PrtD;
		mPrtPropD.Lookup(arEvgp[i], PrtD);

		T_EVGP_D EvgpD;
		if (!m_pDoc->m_pAttrCtrl2->GetEvgp(arEvgp[i], EvgpD)) ASSERT(0);
		GenerateBrdgStrengthInfoSheet(pXL, stRow, i, PrtD.dfck, PrtD.dfy, EvgpD.strName);

		if (pProDlg->Update_ProgressDlg(nProCnt++, nProTot)) { pXL->Quit(); return FALSE; }
	}

	for (int i = 0; i < nEvgp; ++i)
	{
		PrtData PrtD;
		mPrtPropD.Lookup(arEvgp[i], PrtD);

		T_EVGP_D EvgpD;
		if (!m_pDoc->m_pAttrCtrl2->GetEvgp(arEvgp[i], EvgpD)) ASSERT(0);
		GenerateBrdgElasticInfoSheet(pXL, stRow, i, PrtD.dEcs, PrtD.dEsr, EvgpD.strName);

		if (pProDlg->Update_ProgressDlg(nProCnt++, nProTot)) { pXL->Quit(); return FALSE; }
	}

	T_BSED_D BsedD;
	if (!m_pDoc->m_pAttrCtrl2->GetDgnBsed(BsedD)) ASSERT(0);

	T_FIMP_D FimpConcD1, FimpConcD3; // 비선형 재질
	T_FIMP_D FimpSteelD; // 비선형 재질
	if (nEvgpCol > 0)
	{
		PairKey pEvgpCol = arEvgpColm[0];
		T_MPCC_K MpccK = GetMpccKey(pEvgpCol.first, pEvgpCol.second, BOTTOM);
		if (MpccK == 0) MpccK = GetMpccKey(pEvgpCol.first, pEvgpCol.second, TOP);
		if(MpccK != 0)
		{
			T_MPCC_D MpccD;
			if(!m_pDoc->m_pAttrCtrl2->GetMpcc(MpccK, MpccD)) ASSERT(0);

			if(!m_pDoc->m_pAttrCtrl->GetFimp(MpccD.MatlConcK1, FimpConcD1))      ASSERT(0);
			if(!m_pDoc->m_pAttrCtrl->GetFimp(MpccD.MatlConcK1Trans, FimpConcD3)) ASSERT(0);
			if(!m_pDoc->m_pAttrCtrl->GetFimp(MpccD.MatlSteelK, FimpSteelD))      ASSERT(0);
		}
		else
		{
			FimpConcD1.nHysModelType = 99; // Plain concrete
			FimpConcD3.nHysModelType = 99;
			FimpSteelD.nHysModelType = -1;
		}
	}
	else
	{
		FimpConcD1.nHysModelType = -1;
		FimpConcD3.nHysModelType = -1;
		FimpSteelD.nHysModelType = -1;
	}
	GenerateBrdgElseInfoSheet(pXL, stRow, FimpConcD1, FimpConcD3, FimpSteelD, BsedD.nDesignCode);
	if (pProDlg->Update_ProgressDlg(nProCnt++, nProTot)) { pXL->Quit(); return FALSE; }

	pXL->SetHPageBreak(53, stRow, 28);

	return TRUE;
}

BOOL CBrdgSeisEvalKISTEC2015_Report::Print_AnalResTab(IExcel* pXL, CDgnProgressDlg* pProDlg, int& nProCnt, const int& nProTot,
	int& iSheetIdx, arPairKey& arEvgpColm)
{
	// #4.구조해석 Tab 시작
	SetStructAnalSheetPage(pXL, iSheetIdx);

	CMapEx<T_EVGP_K, T_EVGP_K, int, int> mEvgpColm;
	mEvgpColm.RemoveAll();

	int nEvgpCol = arEvgpColm.GetSize();
	int iMaxLink = 0;
	int BeforeEvgpK = 0;
	for (int i = 0; i < nEvgpCol; ++i)
	{
		PairKey pEvgpCol = arEvgpColm[i];
		int nColm = 0;
		if (!mEvgpColm.Lookup(pEvgpCol.first, nColm)) nColm = 0;
		nColm++;
		mEvgpColm.SetAt(pEvgpCol.first, nColm);

		if (BeforeEvgpK == pEvgpCol.first) continue;

		T_EVGP_D EvgpD;
		if (!m_pDoc->m_pAttrCtrl2->GetEvgp(pEvgpCol.first, EvgpD)) continue;

		BOOL bLink = TRUE;
		if (!EvgpD.bLink)                  bLink = FALSE;
		else if (EvgpD.aLink.GetSize() == 0) bLink = FALSE;

		CArray<T_ELNK_K, T_ELNK_K> aELinkK, aNLinkK;
		int nLink = bLink ? m_pDoc->m_pAttrCtrl2->GetLinkList(EvgpD.aLink, aELinkK, aNLinkK) : 0;
		if (nLink > iMaxLink) iMaxLink = nLink;
		BeforeEvgpK = pEvgpCol.first;
	}

	int stRow = 1;
	GenerateStructAnalCommonInfo(pXL, stRow);

	if (iMaxLink > 0)
	{
		int iEvgpCnt = 0;
		BeforeEvgpK = 0;
		for (int i = 0; i < nEvgpCol; ++i)
		{
			PairKey pEvgpCol = arEvgpColm[i];
			if (BeforeEvgpK == pEvgpCol.first)
			{
				if (pProDlg->Update_ProgressDlg(nProCnt++, nProTot)) { pXL->Quit(); return FALSE; }
				continue;
			}
			BeforeEvgpK = pEvgpCol.first;

			T_EVGP_D EvgpD;
			if (!m_pDoc->m_pAttrCtrl2->GetEvgp(pEvgpCol.first, EvgpD)) ASSERT(0);
			if (iMaxLink > 0) GenerateStructAnalPCSupportInfo(pXL, stRow, iEvgpCnt++, iMaxLink, EvgpD.aLink, pEvgpCol.first, pEvgpCol.second, EvgpD.strName);

			if (pProDlg->Update_ProgressDlg(nProCnt++, nProTot)) { pXL->Quit(); return FALSE; }
		}
		stRow++;
	}
	else
	{
		nProCnt += nEvgpCol;
		if (pProDlg->Update_ProgressDlg(nProCnt, nProTot)) { pXL->Quit(); return FALSE; }
	}

	// 다주식은 주각별로 출력
	int nPrtTitle = iMaxLink;
	for (int k = 0; k < 2; ++k)
	{
		T_EVGP_K BeforeEvgpK = 0;
		int iColCnt = 0;
		for (int i = 0; i < nEvgpCol; ++i)
		{
			PairKey pEvgpCol = arEvgpColm[i];
			int nColm = 0;
			mEvgpColm.Lookup(pEvgpCol.first, nColm);
			if (k == 0)
			{
				if (nColm > 1) continue; // 단주식
			}
			else
			{
				if (nColm == 1) continue; // 다주식
				if (BeforeEvgpK != pEvgpCol.first) iColCnt = 0;
			}

			T_EVGP_D EvgpD;
			if (!m_pDoc->m_pAttrCtrl2->GetEvgp(pEvgpCol.first, EvgpD)) ASSERT(0);

			T_BRDGEVAL_RESULT ResultD[2][2]; // Bottom/Top, Longitude/Transverse
			if (!GetResult(pEvgpCol, ResultD)) ASSERT(0);

			CString strName;
			strName.Format(_T("%s(%s)"), EvgpD.strName, EvgpD.aEvgpColm[pEvgpCol.second].strName);
			GenerateStructAnalPCBotInfo(pXL, stRow, ResultD, iColCnt, nPrtTitle, strName);
			iColCnt++;

			BeforeEvgpK = pEvgpCol.first;

			if (pProDlg->Update_ProgressDlg(nProCnt++, nProTot)) { pXL->Quit(); return FALSE; }
		}
	}

	pXL->SetHPageBreak(53, stRow, 28);

	return TRUE;
}

BOOL CBrdgSeisEvalKISTEC2015_Report::Print_ElSuppTab(IExcel* pXL, CDgnProgressDlg* pProDlg, int& nProCnt, const int& nProTot,
	int& iSheetIdx, int nLoadType)
{
	// #5.탄성지진력 Tab 시작
	SetNaturalFrequencySheetPage(pXL, iSheetIdx);
	if (nLoadType == 0)
		GenerateNaturalFrequencySheetRS(pXL);
	else
		GenerateNaturalFrequencySheetTH(pXL);
	if (pProDlg->Update_ProgressDlg(nProCnt++, nProTot)) { pXL->Quit(); return FALSE; }

	return TRUE;
}

BOOL CBrdgSeisEvalKISTEC2015_Report::Print_ComplexModeTab(IExcel* pXL, CDgnProgressDlg* pProDlg, int& nProCnt, const int& nProTot,
	int& iSheetIdx, arPairKey& arEvgpColm, int nLoadType)
{
	// #6.복합모드 부분 Tab
	SetMixedModeSheetPage(pXL, iSheetIdx);

	int stRow = 1;
	int nEvgpCol = arEvgpColm.GetSize();
	for (int i = 0; i < nEvgpCol; ++i)
	{
		PairKey pEvgpCol = arEvgpColm[i];

		T_EVGP_D EvgpD;
		if (!m_pDoc->m_pAttrCtrl2->GetEvgp(pEvgpCol.first, EvgpD)) ASSERT(0);

		T_BRDGEVAL_RESULT ResultD[2][2]; // Bottom/Top, Longitude/Transverse
		if (!GetResult(pEvgpCol, ResultD)) ASSERT(0);

		CMapEx<T_NODE_K, T_NODE_K, double, double&> mapERect, mapNRect;
		double dAxial = Calc_PierCapSuppEnvAxialForce(pEvgpCol.first, pEvgpCol.second, mapERect, mapNRect);

		CString strPier = Get_PierColmNameMixed(EvgpD, pEvgpCol.second);

		//////////////////////////////////////////////////////////////////////////
		CString RptBlock;
		int rowCnt = 0;
		short Row_Ref = 0, Col_Ref = 0;

		if (stRow == 1)
		{
			RptBlock = _T("[MM]Title_Mixed_Mode");
			rowCnt = CopyRange(pXL, RptBlock, stRow);
			stRow += rowCnt;
		}

		CString strMixedModeGroup;
		strMixedModeGroup.Format(_T("4.%d %s 복합모드 해석 결과"), /*iNumEVGP*/i + 1, strPier);

		RptBlock = _T("[MM]Mixed_Mode_Start_Group");
		rowCnt = CopyRangeSubTitle(pXL, RptBlock, stRow);
		GetCellIDByName(pXL, RptBlock, Row_Ref, Col_Ref);
		PrintCellData(pXL, _T("Mixed_Mode_Start_Group"), strMixedModeGroup, Row_Ref, Col_Ref, stRow);
		stRow += rowCnt;
		//////////////////////////////////////////////////////////////////////////

		if (nLoadType == 0)
		{
			GenerateMixedModeRSInfo(pXL, stRow, dAxial, pEvgpCol.first, pEvgpCol.second, i, ResultD, strPier);
			GenerateMixedModeCombInfo(pXL, stRow, dAxial, pEvgpCol.first, pEvgpCol.second, ResultD, strPier);
		}
		GenerateMixedModeEnvCombInfo(pXL, stRow, dAxial, pEvgpCol.first, pEvgpCol.second, ResultD, strPier, nLoadType);

		if (pProDlg->Update_ProgressDlg(nProCnt++, nProTot)) { pXL->Quit(); return FALSE; }
	}
	pXL->SetHPageBreak(53, stRow, 28);

	return TRUE;
}

BOOL CBrdgSeisEvalKISTEC2015_Report::Print_CndtNResTab(IExcel* pXL, CDgnProgressDlg* pProDlg, int& nProCnt, const int& nProTot,
	int& iSheetIdx, arPairKey& arEvgpColm)
{
	int nEvgpCol = arEvgpColm.GetSize();
	int iColIdx = 1;
	UINT BeforeEvgpK = 0;
	for (int i = 0; i < nEvgpCol; ++i)
	{
		PairKey pEvgpCol = arEvgpColm[i];

		T_BRDGEVAL_RESULT ResultD[2][2]; // Bottom/Top, Longitude/Transverse
		if (!GetResult(pEvgpCol, ResultD)) ASSERT(0);

		BOOL bCheck[2][2] = { FALSE, };
		for (int m = 0; m < 2; ++m)
		{
			for (int n = 0; n < 2; ++n) bCheck[m][n] = ResultD[m][n].EvalD.bCheck;
		}
		T_EVGP_D EvgpD;
		if (!m_pDoc->m_pAttrCtrl2->GetEvgp(pEvgpCol.first, EvgpD)) ASSERT(0);
		T_EVPM_D EvpmD;
		if (!GetEvpm(pEvgpCol.first, pEvgpCol.second, EvpmD)) ASSERT(0);
		EN_BEHAVIOR_TYPE enBehavior[2] = { (EvpmD.nBehavL == 0 ? RAHMEN : CANTILEVER),
			(EvpmD.nBehavT == 0 ? RAHMEN : CANTILEVER) };
		EN_PIER_TYPE	 enPierType[4] = { ResultD[0][0].CapaD.ShearCapaD.enPierType,
			ResultD[0][1].CapaD.ShearCapaD.enPierType,
			ResultD[1][0].CapaD.ShearCapaD.enPierType,
			ResultD[1][1].CapaD.ShearCapaD.enPierType };
		EN_FAILURE_MODE  enFailure[4] = { ResultD[0][0].EvalD.ModeD.enFalureMode,
			ResultD[0][1].EvalD.ModeD.enFalureMode,
			ResultD[1][0].EvalD.ModeD.enFalureMode,
			ResultD[1][1].EvalD.ModeD.enFalureMode };

		const bool bPlainConc = ResultD[0][0].ParamD.BarD.bPlainConc;

		// 비선형 재질 정보 가져요기
		T_MPCC_K MpccK1 = 0, MpccK2 = 0;
		T_MPCC_D MpccD, MpccD2;
		if(!bPlainConc)
		{
			if(enBehavior[0] == RAHMEN || enBehavior[1] == RAHMEN)
			{
				MpccK1 = GetMpccKey(pEvgpCol.first, pEvgpCol.second, BOTTOM);
				MpccK2 = GetMpccKey(pEvgpCol.first, pEvgpCol.second, TOP);
			}
			else if(enBehavior[0] == CANTILEVER && enBehavior[1] == CANTILEVER)
			{
				if(bCheck[0][0] && bCheck[0][1])
				{
					MpccK1 = GetMpccKey(pEvgpCol.first, pEvgpCol.second, BOTTOM);
				}
				if(bCheck[1][0] && bCheck[1][1])
				{
					MpccK2 = GetMpccKey(pEvgpCol.first, pEvgpCol.second, TOP);
				}
				else
				{
					MpccK1 = GetMpccKey(pEvgpCol.first, pEvgpCol.second, BOTTOM);
					MpccK2 = GetMpccKey(pEvgpCol.first, pEvgpCol.second, TOP);
				}
			}
			else if(enBehavior[0] == CANTILEVER)
			{
				if(bCheck[0][0]) MpccK1 = GetMpccKey(pEvgpCol.first, pEvgpCol.second, BOTTOM);
				else             MpccK2 = GetMpccKey(pEvgpCol.first, pEvgpCol.second, TOP);
			}
			else if(enBehavior[1] == CANTILEVER)
			{
				if(bCheck[0][1]) MpccK1 = GetMpccKey(pEvgpCol.first, pEvgpCol.second, BOTTOM);
				else             MpccK2 = GetMpccKey(pEvgpCol.first, pEvgpCol.second, TOP);
			}
			else ASSERT(0);
			if(MpccK1 == 0 && MpccK2 == 0) { ASSERT(0); continue; }

			if(MpccK1 != 0)
			{
				if(!m_pDoc->m_pAttrCtrl2->GetMpcc(MpccK1, MpccD)) ASSERT(0);
			}
			if(MpccK2 != 0)
			{
				if(!m_pDoc->m_pAttrCtrl2->GetMpcc(MpccK2, MpccD2)) ASSERT(0);
			}
		}
		else
		{
			T_ELEM_K ElemK = GetEvgpElemK(pEvgpCol.first, pEvgpCol.second, 0, EvgpD);
			T_SECT_K SectK = GetSectKey(ElemK);
			MpccD.SectK = SectK;

			ElemK = GetEvgpElemK(pEvgpCol.first, pEvgpCol.second, 1, EvgpD);
			SectK = GetSectKey(ElemK);
			MpccD2.SectK = SectK;
		}

		CString RptBlock;
		int stRow = 1;
		if (BeforeEvgpK != pEvgpCol.first)// Print only onetime for each Evgp
		{
			// 비선형 재질
			T_FIMP_D FimpConcD1, FimpConcD2, FimpConcD3, FimpConcD4;
			T_FIMP_D FimpConc2D1, FimpConc2D2, FimpConc2D3, FimpConc2D4;
			T_FIMP_D FimpSteelD, FimpSteelD2;

			// Rebar
			T_ELEM_K ElemK = GetEvgpElemK(pEvgpCol.first, pEvgpCol.second, (MpccK1 != 0 ? 1 : 0), EvgpD);
			T_SECT_K SectK = GetSectKey(ElemK);
			T_REBT_D GenRbarD;  GenRbarD.Initialize(); // 임의단면 기둥
			T_RCHK_D RchkData; RchkData.Initialize();

			if(!bPlainConc)
			{
				// 비선형 재질
				if(MpccK1 != 0)
				{
					if(!m_pDoc->m_pAttrCtrl->GetFimp(MpccD.MatlConcK1, FimpConcD1)) ASSERT(0);
					if(!m_pDoc->m_pAttrCtrl->GetFimp(MpccD.MatlConcK2, FimpConc2D1)) ASSERT(0);
					if(!m_pDoc->m_pAttrCtrl->GetFimp(MpccD.MatlSteelK, FimpSteelD)) ASSERT(0);
					if(!m_pDoc->m_pAttrCtrl->GetFimp(MpccD.MatlConcK1Trans, FimpConcD3)) ASSERT(0);
					if(!m_pDoc->m_pAttrCtrl->GetFimp(MpccD.MatlConcK2Trans, FimpConc2D3)) ASSERT(0);
				}
				if(MpccK2 != 0)
				{
					if(!m_pDoc->m_pAttrCtrl->GetFimp(MpccD2.MatlConcK1, FimpConcD2)) ASSERT(0);
					if(!m_pDoc->m_pAttrCtrl->GetFimp(MpccD2.MatlConcK2, FimpConc2D2)) ASSERT(0);
					if(!m_pDoc->m_pAttrCtrl->GetFimp(MpccD2.MatlSteelK, FimpSteelD2)) ASSERT(0);
					if(!m_pDoc->m_pAttrCtrl->GetFimp(MpccD2.MatlConcK1Trans, FimpConcD4)) ASSERT(0);
					if(!m_pDoc->m_pAttrCtrl->GetFimp(MpccD2.MatlConcK2Trans, FimpConc2D4)) ASSERT(0);
				}

				// Rebar
				if(ResultD[0][0].ParamD.SetcD.strShape == _T("GEN") || ResultD[1][0].ParamD.SetcD.strShape == _T("GEN"))
				{
					if(!m_pDoc->m_pAttrCtrl->GetRebt(SectK, GenRbarD)) ASSERT(0);
				}
				else
				{
					if(!m_pDoc->m_pAttrCtrl->GetRchk(SectK, RchkData)) ASSERT(0);
				}

				// Create Rebar
				m_pDoc->m_pMatlDB->Create_RebarData(m_CodeUnit.nBase_Length, m_CodeUnit.nBase_Force, TRUE);
			}

			double dB[4] = { ResultD[0][0].ParamD.SetcD.dB,
							   ResultD[0][1].ParamD.SetcD.dB,
							   ResultD[1][0].ParamD.SetcD.dB,
							   ResultD[1][1].ParamD.SetcD.dB };
			double dH[4] = { ResultD[0][0].ParamD.SetcD.dH,
							   ResultD[0][1].ParamD.SetcD.dH,
							   ResultD[1][0].ParamD.SetcD.dH,
							   ResultD[1][1].ParamD.SetcD.dH };
			double dT[2] = { max(ResultD[0][0].ParamD.OptD.dTs, ResultD[0][1].ParamD.OptD.dTs),
							   max(ResultD[1][0].ParamD.OptD.dTs, ResultD[1][1].ParamD.OptD.dTs) };

			// #7.검토조건 부분 Tab 시작
			if (BeforeEvgpK != 0) iSheetIdx++;
			SetConditionSheetPage(pXL, EvgpD.strName, iSheetIdx);
			if(bPlainConc)
			{
				if(enBehavior[0] == RAHMEN || enBehavior[1] == RAHMEN)
				{
					GenerateStructInfoBot(pXL, RptBlock, stRow, enBehavior, dB, dH, ResultD, &MpccD, RchkData, GenRbarD, EvgpD.strName, bPlainConc);
					GenerateStructInfoTop(pXL, RptBlock, stRow, enBehavior, dB, dH, ResultD, &MpccD2, RchkData, GenRbarD, EvgpD.strName, bPlainConc);
				}
				else
				{
					GenerateStructInfo(pXL, RptBlock, stRow, enBehavior, dB, dH, ResultD, &MpccD, RchkData, GenRbarD, EvgpD.strName, bPlainConc);
				}
			}
			else if (MpccK1 != 0 && MpccK2 != 0)
			{
				GenerateStructInfoBot(pXL, RptBlock, stRow, enBehavior, dB, dH, ResultD, &MpccD, RchkData, GenRbarD, EvgpD.strName);
				GenerateRebarBaseShapeInfo(pXL, RptBlock, stRow, dB, dH, ResultD, enPierType);
				GenerateMaterialModelInfo(pXL, RptBlock, stRow, FimpConcD1, FimpConc2D1, FimpConcD3, FimpConc2D3, FimpSteelD, ResultD, &MpccD);
				GenerateAssumptions(pXL, RptBlock, stRow, dT);

				if (GenerateStructInfoTop(pXL, RptBlock, stRow, enBehavior, dB, dH, ResultD, &MpccD2, RchkData, GenRbarD, EvgpD.strName))
				{
					GenerateRebarBaseShapeInfoTop(pXL, RptBlock, stRow, dB, dH, ResultD, enPierType);
					GenerateMaterialModelInfo(pXL, RptBlock, stRow, FimpConcD2, FimpConc2D2, FimpConcD4, FimpConc2D4, FimpSteelD2, ResultD, &MpccD2);
					GenerateAssumptionsTop(pXL, RptBlock, stRow, dT);
				}
			}
			else if (MpccK1 != 0)
			{
				GenerateStructInfo(pXL, RptBlock, stRow, enBehavior, dB, dH, ResultD, &MpccD, RchkData, GenRbarD, EvgpD.strName);
				GenerateRebarBaseShapeInfo(pXL, RptBlock, stRow, dB, dH, ResultD, enPierType);
				GenerateMaterialModelInfo(pXL, RptBlock, stRow, FimpConcD1, FimpConc2D1, FimpConcD3, FimpConc2D3, FimpSteelD, ResultD, &MpccD);
				GenerateAssumptions(pXL, RptBlock, stRow, dT);
			}
			else if (MpccK2 != 0)
			{
				GenerateStructInfo(pXL, RptBlock, stRow, enBehavior, dB, dH, ResultD, &MpccD2, RchkData, GenRbarD, EvgpD.strName);
				GenerateRebarBaseShapeInfo(pXL, RptBlock, stRow, dB, dH, ResultD, enPierType);
				GenerateMaterialModelInfo(pXL, RptBlock, stRow, FimpConcD2, FimpConc2D2, FimpConcD4, FimpConc2D4, FimpSteelD2, ResultD, &MpccD2);
				GenerateAssumptions(pXL, RptBlock, stRow, dT);
			}
			else ASSERT(0);
			if (pProDlg->Update_ProgressDlg(nProCnt++, nProTot)) { pXL->Quit(); return FALSE; }
			pXL->SetHPageBreak(53, stRow, 28);

			BeforeEvgpK = pEvgpCol.first;
			iColIdx = 1;
		}

		// #8.내진성능평가 부분 Tab 시작
		CString strPier = Get_PierColmName(EvgpD, iColIdx);
		SetEvaluationSheetPage(pXL, strPier, iSheetIdx, iColIdx);
		if(bPlainConc)
		{
			GenerateEvalPlainConcSheet(pXL, RptBlock, stRow, enBehavior, ResultD);
			GenerateEvalResultTable(pXL, RptBlock, stRow, enBehavior, enFailure, ResultD);
			pXL->SetHPageBreak(53, stRow, 28);
			if(pProDlg->Update_ProgressDlg(nProCnt+=6, nProTot)) { pXL->Quit(); return FALSE; }
		}
		else
		{
			double dVc2[4] = {ResultD[0][0].CapaD.ShearCapaD.dVc2,
							   ResultD[0][1].CapaD.ShearCapaD.dVc2,
							   ResultD[1][0].CapaD.ShearCapaD.dVc2,
							   ResultD[1][1].CapaD.ShearCapaD.dVc2};
			double dVc5[4] = {ResultD[0][0].CapaD.ShearCapaD.dVc5,
							   ResultD[0][1].CapaD.ShearCapaD.dVc5,
							   ResultD[1][0].CapaD.ShearCapaD.dVc5,
							   ResultD[1][1].CapaD.ShearCapaD.dVc5};
			double dVs[4] = {ResultD[0][0].CapaD.ShearCapaD.dVs,
							   ResultD[0][1].CapaD.ShearCapaD.dVs,
							   ResultD[1][0].CapaD.ShearCapaD.dVs,
							   ResultD[1][1].CapaD.ShearCapaD.dVs};
			double dVp[4] = {ResultD[0][0].CapaD.ShearCapaD.dVp,
							   ResultD[0][1].CapaD.ShearCapaD.dVp,
							   ResultD[1][0].CapaD.ShearCapaD.dVp,
							   ResultD[1][1].CapaD.ShearCapaD.dVp};
			
			GenerateEvalSheet(pXL, RptBlock, stRow, enPierType, ResultD);
			if(pProDlg->Update_ProgressDlg(nProCnt++, nProTot)) { pXL->Quit(); return FALSE; }

			GenerateIdealMomentPhi(pXL, RptBlock, stRow, enBehavior, ResultD, MpccD, MpccD2, MpccK1, MpccK2);
			if(pProDlg->Update_ProgressDlg(nProCnt++, nProTot)) { pXL->Quit(); return FALSE; }

			GenerateEffectiveYield(pXL, RptBlock, stRow, enBehavior, ResultD);
			GenerateEffectiveSect(pXL, RptBlock, stRow, enBehavior, ResultD);
			GenerateEffectiveStiffJudgement(pXL, RptBlock, stRow, enBehavior, ResultD);
			GenerateHe(pXL, RptBlock, stRow, EvpmD);
			GeneratePeriod(pXL, RptBlock, stRow, ResultD);
			if(pProDlg->Update_ProgressDlg(nProCnt++, nProTot)) { pXL->Quit(); return FALSE; }

			GenerateBendingStress(pXL, RptBlock, stRow, enBehavior, ResultD);
			if(ResultD[0][0].ParamD.OptD.iEvalCode == KISTEC_2015 ||
				ResultD[0][0].ParamD.OptD.iEvalCode == KISTEC_2019 ||
				ResultD[0][0].ParamD.OptD.iEvalCode == KALIS_2023)
			{
				GenerateBendingDuct2015(pXL, RptBlock, stRow, enBehavior, ResultD);
			}
			else if(ResultD[0][0].ParamD.OptD.iEvalCode == KEC_2012)
			{
				GenerateBendingDuct2012(pXL, RptBlock, stRow, enBehavior, ResultD);
			}

			T_UNIT_INDEX CurIndex3, CngIndex3;
			m_pDoc->m_pUnitCtrl->GetUnitIndexCurrent(CurIndex3);
			CngIndex3.nBase_Length = D_UNITSYS_LENGTH_INDEX_MM;
			CngIndex3.nBase_Force = D_UNITSYS_FORCE_INDEX_KN;
			m_pDoc->m_pUnitCtrl->SetUnitIndexCurrent(CngIndex3);

			GenerateBendingResult(pXL, RptBlock, stRow, enBehavior, ResultD, pEvgpCol.first, pEvgpCol.second, strPier);
			if(pProDlg->Update_ProgressDlg(nProCnt++, nProTot)) { 
				m_pDoc->m_pUnitCtrl->SetUnitIndexCurrent(CurIndex3);
				pXL->Quit(); return FALSE; }

			_DGN_SEISEVAL_SHEAR_OUT ShearOut(pEvgpCol.first, pEvgpCol.second, strPier, enPierType[0], enPierType[1], enBehavior[0], enBehavior[1],
				dVc2, dVc5, dVs, dVp, ResultD);
			GenerateShearResult(pXL, RptBlock, stRow, ShearOut);
			if(pProDlg->Update_ProgressDlg(nProCnt++, nProTot)) { 
				m_pDoc->m_pUnitCtrl->SetUnitIndexCurrent(CurIndex3);
				pXL->Quit(); return FALSE; }

			GenerateFailureBegin(pXL, RptBlock, stRow, enBehavior, enFailure, ResultD);
			GenerateFailureMode(pXL, RptBlock, stRow, enBehavior, enFailure, ResultD, (MpccK1 != 0 ? MpccD : MpccD2));

			GenerateEvalResult(pXL, RptBlock, stRow, enBehavior, enFailure, ResultD);
			GenerateEvalResultTable(pXL, RptBlock, stRow, enBehavior, enFailure, ResultD);
			pXL->SetHPageBreak(53, stRow, 28);
			if(pProDlg->Update_ProgressDlg(nProCnt++, nProTot)) { 
				m_pDoc->m_pUnitCtrl->SetUnitIndexCurrent(CurIndex3);
				pXL->Quit(); return FALSE; }

			m_pDoc->m_pUnitCtrl->SetUnitIndexCurrent(CurIndex3);
		}
	}
	iSheetIdx++;

	return TRUE;
}

void CBrdgSeisEvalKISTEC2015_Report::SetCoverSheetPage(IExcel* pExcel)
{
	m_strBaseSheet = _T("00.표지");
	CString strDetailSheetName = _T("00.표지");

	m_arDetailSheetName.Add(strDetailSheetName);
	SetDetailSheetPage(pExcel, strDetailSheetName);

}

void CBrdgSeisEvalKISTEC2015_Report::SetSummarySheetPage(IExcel* pExcel, int& iIdx)
{
	m_strBaseSheet = _T("0.결과요약(Base)");
	CString strDetailSheetName;
	strDetailSheetName.Format(_T("%d.결과요약"), iIdx);

	m_arDetailSheetName.Add(strDetailSheetName);
	SetDetailSheetPage(pExcel, strDetailSheetName);

	iIdx++;
}

void CBrdgSeisEvalKISTEC2015_Report::SetBrdgInfoSheetPage(IExcel* pExcel, int& iIdx)
{
	m_strBaseSheet = _T("1.교량현황(Base)");
	CString strDetailSheetName;
	strDetailSheetName.Format(_T("%d.교량현황"), iIdx);

	m_arDetailSheetName.Add(strDetailSheetName);
	SetDetailSheetPage(pExcel, strDetailSheetName);

	iIdx++;
}

void CBrdgSeisEvalKISTEC2015_Report::SetStructAnalSheetPage(IExcel* pExcel, int& iIdx)
{
	m_strBaseSheet = _T("2.구조해석(Base)");
	CString strDetailSheetName;
	strDetailSheetName.Format(_T("%d.구조해석"), iIdx);

	m_arDetailSheetName.Add(strDetailSheetName);
	SetDetailSheetPage(pExcel, strDetailSheetName);

	iIdx++;
}


void CBrdgSeisEvalKISTEC2015_Report::SetNaturalFrequencySheetPage(IExcel* pExcel, int& iIdx)
{
	m_strBaseSheet = _T("3.탄성지진력(Base)");
	CString strDetailSheetName;
	strDetailSheetName.Format(_T("%d.탄성지진력"), iIdx);

	m_arDetailSheetName.Add(strDetailSheetName);
	SetDetailSheetPage(pExcel, strDetailSheetName);

	iIdx++;
}

void CBrdgSeisEvalKISTEC2015_Report::SetMixedModeSheetPage(IExcel* pExcel, int& iIdx)
{
	m_strBaseSheet = _T("4.복합모드(Base)");
	CString strDetailSheetName;
	strDetailSheetName.Format(_T("%d.복합모드"), iIdx);

	m_arDetailSheetName.Add(strDetailSheetName);
	SetDetailSheetPage(pExcel, strDetailSheetName);

	iIdx++;
}

void CBrdgSeisEvalKISTEC2015_Report::SetConditionSheetPage(IExcel* pExcel, const CString& strName, int& iIdx)
{
	m_strBaseSheet = _T("5.검토조건(Base)");

	CString RptBlock;

	CString strDetailSheetName;
	strDetailSheetName.Format(_T("%d.검토조건(%s)"), iIdx, strName);

	m_arDetailSheetName.Add(strDetailSheetName);
	SetDetailSheetPage(pExcel, strDetailSheetName);

	iIdx++;
}

void CBrdgSeisEvalKISTEC2015_Report::SetEvaluationSheetPage(IExcel* pExcel, const CString& strName, const int& iIdx, int& iColIdx)
{
	m_strBaseSheet = _T("6.내진성능평가(Base)");

	CString RptBlock;

	CString strDetailSheetName;
	strDetailSheetName.Format(_T("%d-%d.교각%s 내진성능평가 "), iIdx, iColIdx, strName);

	m_arDetailSheetName.Add(strDetailSheetName);
	SetDetailSheetPage(pExcel, strDetailSheetName);

	iColIdx++;
}

void CBrdgSeisEvalKISTEC2015_Report::DelDefaultSheetPage(IExcel* pExcel)
{
	//pExcel->DeleteSheet(_T("0.결과요약(Base)"));
	//pExcel->DeleteSheet(_T("1.교량현황(Base)"));
	//pExcel->DeleteSheet(_T("2.구조해석(Base)"));
	//pExcel->DeleteSheet(_T("3.탄성지진력(Base)"));
	//pExcel->DeleteSheet(_T("4.복합모드(Base)"));
	//pExcel->DeleteSheet(_T("5.검토조건(Base)"));
	//pExcel->DeleteSheet(_T("6.내진성능평가(Base)"));
	//pExcel->DeleteSheet(_T("7.받침성능평가(Base)"));
	//pExcel->DeleteSheet(_T("8.받침지지길이평가(Base)"));

	if(pExcel->GetExcelType() == ExcelTYPE::TYPE_MSEXCEL)
	{
		CStringArray aSheetName;
		pExcel->GetSheetsName(aSheetName);
		const auto nSheet = aSheetName.GetSize();
		const auto nDet = m_arDetailSheetName.GetSize();
		for(int i = 0; i < nSheet; ++i)
		{
			if(![&](){
				for(int j = 0; j < nDet; ++j)
				{
					if(aSheetName[i] == m_arDetailSheetName[j])
						return true;
				}
				return false;
			}())
			{
				pExcel->DeleteSheet(aSheetName[i]);
			}
		}
	}
}

void CBrdgSeisEvalKISTEC2015_Report::SetDetailSheetPage(IExcel* pExcel, CString strNameCurrent)
{
	if (pExcel->IsExistSheetName(strNameCurrent))
	{
		pExcel->SetActiveSheet(strNameCurrent);

	}
	else
	{
		pExcel->AddSheet2(strNameCurrent);
		pExcel->SetActiveSheet(strNameCurrent);
	}

	pExcel->SetStandardFont(_T("맑은 고딕"));
	pExcel->SetMargin(13, 12, 20, 17);
	pExcel->SetColumnWidth(0, 0, 0, 60, 2.0);  // 28->74

	pExcel->SetRowHeight(16.25);
}

BOOL CBrdgSeisEvalKISTEC2015_Report::GenerateNaturalFrequencySheetRS(IExcel* pExcel)
{
	CString RptBlock;
	int rowCnt = 0;
	int stRow = 1;
	short Row_Ref = 0, Col_Ref = 0;

	RptBlock = _T("[NF]Title_Natural_Frequency");
	rowCnt = CopyRange(pExcel, RptBlock, stRow);
	//GetCellIDByName(pExcel, RptBlock, Row_Ref, Col_Ref);
	stRow += rowCnt;

	RptBlock = _T("[NF]Natural_Frequency_Graph_Head");
	rowCnt = CopyRangeSubTitle(pExcel, RptBlock, stRow);
	stRow += rowCnt;

	T_SPFC_K SpkcKey;
	T_SPFC_D SpkcData;
	CArray<T_SPFC_K, T_SPFC_K> aSpfcKeyList;
	m_pDoc->m_pAttrCtrl->GetSpfcKeyList(aSpfcKeyList);

	for (int i = 0; i < aSpfcKeyList.GetSize(); i++)
	{
		SpkcKey = aSpfcKeyList.GetAt(i);
	}

	m_pDoc->m_pAttrCtrl->GetSpfc(SpkcKey, SpkcData);

	if (SpkcData.SpecFuncName == _T("KS-Bridge"))
	{
		CString strSeisSoil;
		CString strSeisArea;

		if (abs(SpkcData.CodeParam.KSBRG.dSoil - 1.0) < cDGN_Zero) strSeisSoil = _T("S1 (1.0)");
		else if (abs(SpkcData.CodeParam.KSBRG.dSoil - 1.2) < cDGN_Zero) strSeisSoil = _T("S2 (1.2)");
		else if (abs(SpkcData.CodeParam.KSBRG.dSoil - 1.5) < cDGN_Zero) strSeisSoil = _T("S3 (1.5)");
		else if (abs(SpkcData.CodeParam.KSBRG.dSoil - 2.0) < cDGN_Zero) strSeisSoil = _T("S4 (2.0)");

		if (abs(SpkcData.CodeParam.KSBRG.dEPA - 0.11) < cDGN_Zero) strSeisArea = _T("Area I (0.11)");
		else if (abs(SpkcData.CodeParam.KSBRG.dEPA - 0.07) < cDGN_Zero) strSeisArea = _T("Area II (0.07)");


		RptBlock = _T("Natural_Frequency_KS");
		rowCnt = CopyRangeSubTitle(pExcel, RptBlock, stRow);
		GetCellIDByName(pExcel, RptBlock, Row_Ref, Col_Ref);

		PrintCellData(pExcel, _T("NF_KS_S"), strSeisSoil, Row_Ref, Col_Ref, stRow);
		PrintCellData(pExcel, _T("NF_KS_A"), strSeisArea, Row_Ref, Col_Ref, stRow);
		PrintCellData(pExcel, _T("NF_KS_I"), SpkcData.CodeParam.KSBRG.dIe, Row_Ref, Col_Ref, stRow);
		PrintCellData(pExcel, _T("NF_KS_R"), SpkcData.CodeParam.KSBRG.dCoef, Row_Ref, Col_Ref, stRow);
		PrintCellData(pExcel, _T("NF_KS_MaxPeriod"), SpkcData.CodeParam.KSBRG.dMaxPeriod, Row_Ref, Col_Ref, stRow);

	}
	else if (SpkcData.SpecFuncName == _T("KDS(17-10-00:2018)"))
	{
		CString strSeisZone;
		CString strSiteClass;

		if (SpkcData.CodeParam.KSCE2019.nSeisZone == 0) strSeisZone = _T("Zone1(0.11)");
		else                                            strSeisZone = _T("Zone2(0.07)");

		if (SpkcData.CodeParam.KSCE2019.nSiteClass == 0) strSiteClass = _T("S1");
		else if (SpkcData.CodeParam.KSCE2019.nSiteClass == 1) strSiteClass = _T("S2");
		else if (SpkcData.CodeParam.KSCE2019.nSiteClass == 2) strSiteClass = _T("S3");
		else if (SpkcData.CodeParam.KSCE2019.nSiteClass == 3) strSiteClass = _T("S4");
		else if (SpkcData.CodeParam.KSCE2019.nSiteClass == 4) strSiteClass = _T("S5");
		else if (SpkcData.CodeParam.KSCE2019.nSiteClass == 5) strSiteClass = _T("S6");


		if (SpkcData.CodeParam.KSCE2019.nSiteClass == 0)
		{
			RptBlock = _T("Natural_Frequency_KDS_0");
			rowCnt = CopyRangeSubTitle(pExcel, RptBlock, stRow);
			GetCellIDByName(pExcel, RptBlock, Row_Ref, Col_Ref);

			PrintCellData(pExcel, _T("NF_KDS_Z_0"), strSeisZone, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("NF_KDS_I_0"), SpkcData.CodeParam.KSCE2019.dIe, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("NF_KDS_S_0"), strSiteClass, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("NF_KDS_MaxPeriod_0"), SpkcData.CodeParam.KSCE2019.dMaxPeriod, Row_Ref, Col_Ref, stRow);

		}
		else
		{
			RptBlock = _T("Natural_Frequency_KDS");
			rowCnt = CopyRangeSubTitle(pExcel, RptBlock, stRow);
			GetCellIDByName(pExcel, RptBlock, Row_Ref, Col_Ref);

			PrintCellData(pExcel, _T("NF_KDS_Z"), strSeisZone, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("NF_KDS_I"), SpkcData.CodeParam.KSCE2019.dIe, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("NF_KDS_S"), strSiteClass, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("NF_KDS_Fa"), SpkcData.CodeParam.KSCE2019.dFa, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("NF_KDS_Fv"), SpkcData.CodeParam.KSCE2019.dFv, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("NF_KDS_MaxPeriod"), SpkcData.CodeParam.KSCE2019.dMaxPeriod, Row_Ref, Col_Ref, stRow);
		}

	}

	stRow += rowCnt;
	
	auto ChartItemD = [&]()
	{
		T_CHART_ITEM_D rData;
		rData.Initialize();

		///////////////////////////////////////////////////////////////////////////
		// BackGround
		rData.StyleD.BackGroundD.dwCompFillStyle = CX_SOLID_FILL;
		rData.StyleD.BackGroundD.colColor = CXCLR_WHITE;

		// Display
		rData.StyleD.DisplayD.dwGraphStyle = CX_GRAPH_XYSCATTERG_EX;
		rData.StyleD.DisplayD.dwAxisStyle = CX_AXIS_XYSCATTER;
		rData.StyleD.DisplayD.colDispColor = CXCLR_WHITE;
		rData.StyleD.DisplayD.colGridColor = CXCLR_GRAY40;
		rData.StyleD.DisplayD.dwCompFillStyle = CX_SOLID_FILL;
		rData.StyleD.DisplayD.dwCompBorderStyle = CX_NO_BORDER;
		rData.StyleD.DisplayD.nMeasurement = CX_PERCENT;
		rData.StyleD.DisplayD.nFontSize = 8;
		rData.StyleD.DisplayD.bSizeDefaults = TRUE;
		rData.StyleD.DisplayD.sFaceName = _LS(IDS_GPS_GRAPH_FONT_ARIAL);
		CRect rect(0.2, 0, 100, 97.0);
		rData.StyleD.DisplayD.rectGraph = rect;
		rData.StyleD.DisplayD.bUseEmptyGraph = TRUE;
		rData.StyleD.DisplayD.bUseMaxScale = FALSE;
		rData.StyleD.DisplayD.bAlwaysShowZero = TRUE;

		// X-Axis Label
		rData.StyleD.XAxisD.nMeasurement = SRGraphComponent::PERCENT;
		rData.StyleD.XAxisD.nLocationPoint = SRGraphLabel::TopCenter;
		rData.StyleD.XAxisD.nTextPositionX = 50.0;
		rData.StyleD.XAxisD.nTextPositionY = 95.0;
		rData.StyleD.XAxisD.bBlockMember = FALSE;
		rData.StyleD.XAxisD.nLabelStandard = SRGraphLabel::Normal;
		rData.StyleD.XAxisD.colLabel = CXCLR_WHITE;
		rData.StyleD.XAxisD.dFontSize = 9;
		rData.StyleD.XAxisD.sFaceName = _LS(IDS_GPS_GRAPH_FONT_ARIAL);

		// Y-Axis Label  
		rData.StyleD.YAxisD.nMeasurement = SRGraphComponent::PERCENT;
		rData.StyleD.YAxisD.nLocationPoint = SRGraphLabel::MidCenter;
		rData.StyleD.YAxisD.nTextPositionX = 2.0;
		rData.StyleD.YAxisD.nTextPositionY = 50.0;
		rData.StyleD.YAxisD.bBlockMember = FALSE;
		rData.StyleD.YAxisD.nLabelStandard = SRGraphLabel::Upward;
		rData.StyleD.YAxisD.colLabel = CXCLR_WHITE;
		rData.StyleD.YAxisD.dFontSize = 9;
		rData.StyleD.YAxisD.sFaceName = _LS(IDS_GPS_GRAPH_FONT_ARIAL);
		///////////////////////////////////////////////////////////////////////////

		rData.StyleD.DisplayD.sFaceName = _LS(IDS_GRAPH_FONT_ARIAL);
		rData.StyleD.DisplayD.bAlwaysShowZero = FALSE;
		rData.StyleD.XAxisD.nTextPositionY = 93.5;
		rData.StyleD.YAxisD.nTextPositionX = 1.5;

		rData.StyleD.XAxisD.sFaceName = _LS(IDS_GRAPH_FONT_ARIAL);
		rData.StyleD.YAxisD.sFaceName = _LS(IDS_GRAPH_FONT_ARIAL);
		rData.StyleD.XAxisD.sAxisLabelText = _LS(IDS_WG_CMD__ADDD__Period__sec_);
		rData.StyleD.YAxisD.sAxisLabelText = _LS(IDS_WG_CMD__ADDD__Spectral_Data) + _T("(g)");

		rData.StyleD.LegendD.bLegend = TRUE;

		// Legend Setting
		rData.StyleD.LegendD.dKeySize = 2.;
		rData.StyleD.LegendD.dXMargin = 0.5;
		rData.StyleD.LegendD.dYMargin = 0.5;
		rData.StyleD.LegendD.dwLegendKeyStyle = CX_KEY_BOX;
		rData.StyleD.LegendD.bLegend = TRUE;
		rData.StyleD.LegendD.nFontSize = 8;
		rData.StyleD.LegendD.dKeySize = 2.;
		rData.StyleD.LegendD.dXMargin = 0.5;
		rData.StyleD.LegendD.dYMargin = 0.5;
		rData.StyleD.LegendD.nJustification = 0;
		rData.StyleD.LegendD.nKeyAnnotationCharCount = 8;
		rData.StyleD.LegendD.nKeyAnnotationLineCount = 1;
		rData.StyleD.LegendD.nScopeImin = -1;
		rData.StyleD.LegendD.nScopeImax = -1;
		rData.StyleD.LegendD.nScopeGmin = 0;
		rData.StyleD.LegendD.bLegendKeyFirst = TRUE;

		CRect rectLegend(24., 94, 100., 100.);
		rData.StyleD.LegendD.rectLegend = rectLegend;

		rData.StyleD.LegendD.bLegendLockRC = TRUE;

		rData.StyleD.LegendD.bSizeDefaults = TRUE;

		rData.StyleD.LegendD.LabelD.nMeasurement = SRGraphComponent::PERCENT;
		rData.StyleD.LegendD.LabelD.nLocationPoint = SRGraphLabel::MidLeft;
		rData.StyleD.LegendD.LabelD.nTextPositionX = 7.0;
		rData.StyleD.LegendD.LabelD.nTextPositionY = 95.5;
		rData.StyleD.LegendD.LabelD.dFontSize = 9;
		rData.StyleD.LegendD.LabelD.bBlockMember = FALSE;
		rData.StyleD.LegendD.LabelD.nLabelStandard = SRGraphLabel::Normal;

		const CString strSpec = _T(" ") + SpkcData.SpecFuncName;
		rData.StyleD.LegendD.aLegendTitle.Add(strSpec);

		int nSize = SpkcData.arSpecFuncData.GetSize();
		double dMnMxXVal[2], dMnMxYVal[2];
		rData.aValues.SetSize(1);
		rData.aValues[0].aXList.SetSize(nSize);
		rData.aValues[0].aYList.SetSize(nSize);
		for(int i = 0; i < nSize; i++)
		{
			rData.aValues[0].aXList[i] = SpkcData.arSpecFuncData[i].dblPeriod;
			rData.aValues[0].aYList[i] = SpkcData.arSpecFuncData[i].dblValue;

			if(i == 0)
			{
				dMnMxXVal[0] = SpkcData.arSpecFuncData[i].dblPeriod;
				dMnMxXVal[1] = SpkcData.arSpecFuncData[i].dblPeriod;
				dMnMxYVal[0] = SpkcData.arSpecFuncData[i].dblValue;
				dMnMxYVal[1] = SpkcData.arSpecFuncData[i].dblValue;
				continue;
			}

			if(SpkcData.arSpecFuncData[i].dblPeriod < dMnMxXVal[0]) dMnMxXVal[0] = SpkcData.arSpecFuncData[i].dblPeriod;
			if(SpkcData.arSpecFuncData[i].dblPeriod > dMnMxXVal[1]) dMnMxXVal[1] = SpkcData.arSpecFuncData[i].dblPeriod;

			if(SpkcData.arSpecFuncData[i].dblValue < dMnMxYVal[0]) dMnMxYVal[0] = SpkcData.arSpecFuncData[i].dblValue;
			if(SpkcData.arSpecFuncData[i].dblValue > dMnMxYVal[1]) dMnMxYVal[1] = SpkcData.arSpecFuncData[i].dblValue;
		}

		double dX = (dMnMxXVal[1] - dMnMxXVal[0]) / 20.;
		double dY = (dMnMxYVal[1] - dMnMxYVal[0]) / 20.;

		rData.StyleD.DisplayD.bMinMaxRange = TRUE;
		rData.StyleD.DisplayD.dMaxRangeX = dMnMxXVal[1] + dX;
		rData.StyleD.DisplayD.dMaxRangeY = dMnMxYVal[1] + dY;
		rData.StyleD.DisplayD.dMinRangeX = 0.0;
		rData.StyleD.DisplayD.dMinRangeY = 0.0;

		rData.StyleD.DisplayD.bScaleLimit = FALSE;
		rData.StyleD.DisplayD.bUseMaxScale = TRUE;

		return rData;
	}();

	CString strFilePath;
	strFilePath.Format(_T("%sRS_Graph.bmp"), m_strPicPath);
	T_UCHT_D UchtD; UchtD.Initialize(D_TYPE_RS_FUNC);
	m_pDoc->MakeGraphAndBmpImage(ChartItemD, strFilePath, &UchtD);

	RptBlock = _T("[NF]Chart");
	rowCnt = CopyRangeSubTitle(pExcel, RptBlock, stRow);
	GetCellIDByName(pExcel, RptBlock, Row_Ref, Col_Ref);

	auto l_AddPicutre = [this](IExcel* pExcel, CString& strFileName, const int& stRow)
	{
		int nH = 16;
		int nW = 26;
		CString strPath = strFileName;
		if(WaitForFile(strPath))
		{
			pExcel->AddPicture_Count(stRow, 2, nH, nW, strPath);
		}
		return TRUE;
	};
	PrintCellData(pExcel, _T("NF_Chart_Image"), l_AddPicutre(pExcel, strFilePath, stRow), Row_Ref, Col_Ref, stRow);

	stRow += rowCnt+1;

	// MassTable
	GenerateNaturalFrequencyMassTable(pExcel, stRow);

	pExcel->SetHPageBreak(53, stRow, 28);

	return TRUE;

}

BOOL CBrdgSeisEvalKISTEC2015_Report::GenerateNaturalFrequencySheetTH(IExcel* pExcel)
{
	CString RptBlock;
	int rowCnt = 0;
	int stRow = 1;
	short Row_Ref = 0, Col_Ref = 0;

	RptBlock = _T("[NF]Title_Natural_Frequency");
	rowCnt = CopyRange(pExcel, RptBlock, stRow);
	//GetCellIDByName(pExcel, RptBlock, Row_Ref, Col_Ref);
	stRow += rowCnt;

	RptBlock = _T("[NF]Natural_Frequency_Graph_Head_TH");
	rowCnt = CopyRangeSubTitle(pExcel, RptBlock, stRow);
	stRow += rowCnt;

	//////////////////////////////////////////////////////////////////////////
	// ground acceleration
	CMapEx<T_THFC_K, T_THFC_K, T_THFC_D, T_THFC_D&> mThfcData;

	T_THFC_D DataThfc;
	T_THGA_K KeyThga;
	T_THGA_D DataThga;
	POSITION pos = m_pDoc->m_pAttrCtrl->GetStartThga();
	while (pos != NULL)
	{
		m_pDoc->m_pAttrCtrl->GetNextThga(pos, KeyThga, DataThga);

		for (int i = 0; i < 3; i++)
		{
			T_THFC_K ThfcKey = DataThga.nThisFuncKey[i];
			if (ThfcKey != 0)
			{
				m_pDoc->m_pAttrCtrl->GetThfc(ThfcKey, DataThfc);

				T_THFC_D DataThfcTemp;
				if (!mThfcData.Lookup(ThfcKey, DataThfcTemp))
				{
					mThfcData.SetAt(ThfcKey, DataThfc);
				}
			}
		}
	}
	//////////////////////////////////////////////////////////////////////////
	auto l_ChartItemD = [&](const T_THFC_D& ThfcD)
	{
		T_CHART_ITEM_D rData;
		rData.Initialize();

		///////////////////////////////////////////////////////////////////////////
		// BackGround
		rData.StyleD.BackGroundD.dwCompFillStyle = CX_SOLID_FILL;
		rData.StyleD.BackGroundD.colColor = CXCLR_WHITE;

		// Display
		rData.StyleD.DisplayD.dwGraphStyle = CX_GRAPH_XYSCATTERG_EX;
		rData.StyleD.DisplayD.dwAxisStyle = CX_AXIS_XYSCATTER;
		rData.StyleD.DisplayD.colDispColor = CXCLR_WHITE;
		rData.StyleD.DisplayD.colGridColor = CXCLR_GRAY40;
		rData.StyleD.DisplayD.dwCompFillStyle = CX_SOLID_FILL;
		rData.StyleD.DisplayD.dwCompBorderStyle = CX_NO_BORDER;
		rData.StyleD.DisplayD.nMeasurement = CX_PERCENT;
		rData.StyleD.DisplayD.nFontSize = 8;
		rData.StyleD.DisplayD.bSizeDefaults = TRUE;
		rData.StyleD.DisplayD.sFaceName = _LS(IDS_GPS_GRAPH_FONT_ARIAL);
		CRect rect(0.2, 0, 100, 97.0);
		rData.StyleD.DisplayD.rectGraph = rect;
		rData.StyleD.DisplayD.bUseEmptyGraph = TRUE;
		rData.StyleD.DisplayD.bUseMaxScale = FALSE;
		rData.StyleD.DisplayD.bAlwaysShowZero = TRUE;

		// X-Axis Label
		rData.StyleD.XAxisD.nMeasurement = SRGraphComponent::PERCENT;
		rData.StyleD.XAxisD.nLocationPoint = SRGraphLabel::TopCenter;
		rData.StyleD.XAxisD.nTextPositionX = 50.0;
		rData.StyleD.XAxisD.nTextPositionY = 95.0;
		rData.StyleD.XAxisD.bBlockMember = FALSE;
		rData.StyleD.XAxisD.nLabelStandard = SRGraphLabel::Normal;
		rData.StyleD.XAxisD.colLabel = CXCLR_WHITE;
		rData.StyleD.XAxisD.dFontSize = 9;
		rData.StyleD.XAxisD.sFaceName = _LS(IDS_GPS_GRAPH_FONT_ARIAL);

		// Y-Axis Label  
		rData.StyleD.YAxisD.nMeasurement = SRGraphComponent::PERCENT;
		rData.StyleD.YAxisD.nLocationPoint = SRGraphLabel::MidCenter;
		rData.StyleD.YAxisD.nTextPositionX = 2.0;
		rData.StyleD.YAxisD.nTextPositionY = 50.0;
		rData.StyleD.YAxisD.bBlockMember = FALSE;
		rData.StyleD.YAxisD.nLabelStandard = SRGraphLabel::Upward;
		rData.StyleD.YAxisD.colLabel = CXCLR_WHITE;
		rData.StyleD.YAxisD.dFontSize = 9;
		rData.StyleD.YAxisD.sFaceName = _LS(IDS_GPS_GRAPH_FONT_ARIAL);
		///////////////////////////////////////////////////////////////////////////

		rData.StyleD.DisplayD.sFaceName = _LS(IDS_GRAPH_FONT_ARIAL);
		rData.StyleD.DisplayD.bAlwaysShowZero = FALSE;
		rData.StyleD.XAxisD.nTextPositionY = 93.5;
		rData.StyleD.YAxisD.nTextPositionX = 1.5;

		rData.StyleD.XAxisD.sFaceName = _LS(IDS_GRAPH_FONT_ARIAL);
		rData.StyleD.YAxisD.sFaceName = _LS(IDS_GRAPH_FONT_ARIAL);
		rData.StyleD.XAxisD.sAxisLabelText = _T("Time (sec)"); // _LS(IDS_WG_CMD__ADDD__Period__sec_);
		rData.StyleD.YAxisD.sAxisLabelText = _T("Time History Data");// _LS(IDS_WG_CMD__ADDD__Spectral_Data) + _T("(g)");

		// Legend Setting
		rData.StyleD.LegendD.bLegend = FALSE;
		rData.StyleD.LegendD.dKeySize = 2.;
		rData.StyleD.LegendD.dXMargin = 0.5;
		rData.StyleD.LegendD.dYMargin = 0.5;
		rData.StyleD.LegendD.dwLegendKeyStyle = CX_KEY_BOX;
		rData.StyleD.LegendD.nFontSize = 8;
		rData.StyleD.LegendD.dKeySize = 2.;
		rData.StyleD.LegendD.dXMargin = 0.5;
		rData.StyleD.LegendD.dYMargin = 0.5;
		rData.StyleD.LegendD.nJustification = 0;
		rData.StyleD.LegendD.nKeyAnnotationCharCount = 8;
		rData.StyleD.LegendD.nKeyAnnotationLineCount = 1;
		rData.StyleD.LegendD.nScopeImin = -1;
		rData.StyleD.LegendD.nScopeImax = -1;
		rData.StyleD.LegendD.nScopeGmin = 0;
		rData.StyleD.LegendD.bLegendKeyFirst = TRUE;

		CRect rectLegend(24., 94, 100., 100.);
		rData.StyleD.LegendD.rectLegend = rectLegend;

		rData.StyleD.LegendD.bLegendLockRC = TRUE;

		rData.StyleD.LegendD.bSizeDefaults = TRUE;

		rData.StyleD.LegendD.LabelD.nMeasurement = SRGraphComponent::PERCENT;
		rData.StyleD.LegendD.LabelD.nLocationPoint = SRGraphLabel::MidLeft;
		rData.StyleD.LegendD.LabelD.nTextPositionX = 7.0;
		rData.StyleD.LegendD.LabelD.nTextPositionY = 95.5;
		rData.StyleD.LegendD.LabelD.dFontSize = 9;
		rData.StyleD.LegendD.LabelD.bBlockMember = FALSE;
		rData.StyleD.LegendD.LabelD.nLabelStandard = SRGraphLabel::Normal;

		const CString strSpec = _T(" ") + ThfcD.ThisFuncName;
		rData.StyleD.LegendD.aLegendTitle.Add(strSpec);

		int nSize = ThfcD.arThisFuncData.GetSize();

		double dMnMxXVal[2], dMnMxYVal[2];
		rData.aValues.SetSize(1);
		rData.aValues[0].aXList.SetSize(nSize);
		rData.aValues[0].aYList.SetSize(nSize);
		for(int i = 0; i < nSize; i++)
		{
			rData.aValues[0].aXList[i] = ThfcD.arThisFuncData[i].dblTime;
			rData.aValues[0].aYList[i] = ThfcD.arThisFuncData[i].dblValue;

			if(i == 0)
			{
				dMnMxXVal[0] = ThfcD.arThisFuncData[i].dblTime;
				dMnMxXVal[1] = ThfcD.arThisFuncData[i].dblTime;
				dMnMxYVal[0] = ThfcD.arThisFuncData[i].dblValue;
				dMnMxYVal[1] = ThfcD.arThisFuncData[i].dblValue;
				continue;
			}

			if(ThfcD.arThisFuncData[i].dblTime < dMnMxXVal[0]) dMnMxXVal[0] = ThfcD.arThisFuncData[i].dblTime;
			if(ThfcD.arThisFuncData[i].dblTime > dMnMxXVal[1]) dMnMxXVal[1] = ThfcD.arThisFuncData[i].dblTime;

			if(ThfcD.arThisFuncData[i].dblValue < dMnMxYVal[0]) dMnMxYVal[0] = ThfcD.arThisFuncData[i].dblValue;
			if(ThfcD.arThisFuncData[i].dblValue > dMnMxYVal[1]) dMnMxYVal[1] = ThfcD.arThisFuncData[i].dblValue;
		}

		double dX = (dMnMxXVal[1] - dMnMxXVal[0]) / 20.;
		double dY = (dMnMxYVal[1] - dMnMxYVal[0]) / 20.;

		//rData.StyleD.DisplayD.bMinMaxRange = TRUE;
		//rData.StyleD.DisplayD.dMaxRangeX = dMnMxXVal[1] + dX;
		//rData.StyleD.DisplayD.dMaxRangeY = dMnMxYVal[1] + dY;
		//rData.StyleD.DisplayD.dMinRangeX = 0.0;
		//rData.StyleD.DisplayD.dMinRangeY = dMnMxYVal[0] - dY;

		rData.StyleD.DisplayD.bScaleLimit = FALSE;
		rData.StyleD.DisplayD.bUseMaxScale = FALSE;
		rData.StyleD.DisplayD.bAlwaysShowZero = TRUE;

		return rData;
	};

	int nCnt = 0;
	int stRowFirst = stRow;
	int stRowCnt = stRowFirst;
	int stRowPre = 0;

	T_KEY_LIST aThfcK;
	mThfcData.GetKeyList(aThfcK);
	CArrayUtil::Sort(aThfcK);
	const auto nThfc = aThfcK.GetSize();
	for(int i=0; i<nThfc; ++i)
	{
		const auto ThfcK = aThfcK[i];
		if(!mThfcData.Lookup(ThfcK, DataThfc)) continue;

		// Add Name
		CString strName;
		if(DataThfc.Description.IsEmpty())
		{
			strName.Format(_T("[ %s %s ]"), _T("Time function"), DataThfc.ThisFuncName);
		}
		else
		{
			strName.Format(_T("[ %s %s ; %s ]"), _T("Time function"), DataThfc.ThisFuncName, DataThfc.Description);
		}
		pExcel->SetValue(stRow, 1, strName);
		stRow++;

		auto ChartItemD = l_ChartItemD(DataThfc);

		CString strFilePath;
		strFilePath.Format(_T("%s_%s_TH_Graph.bmp"), m_strPicPath, DataThfc.ThisFuncName);
		T_UCHT_D UchtD; UchtD.Initialize(D_TYPE_TH_FUNC);
		m_pDoc->MakeGraphAndBmpImage(ChartItemD, strFilePath, &UchtD);

		RptBlock = _T("[NF]Chart");
		rowCnt = CopyRangeSubTitle(pExcel, RptBlock, stRow);
		GetCellIDByName(pExcel, RptBlock, Row_Ref, Col_Ref);

		auto l_AddPicutre = [this](IExcel* pExcel, CString& strFileName, const int& stRow)
		{
			int nH = 16;
			int nW = 26;
			CString strPath = strFileName;
			if(WaitForFile(strPath))
			{
				pExcel->AddPicture_Count(stRow, 2, nH, nW, strPath);
			}
			return TRUE;
		};
		
		// Add picture
		PrintCellData(pExcel, _T("NF_Chart_Image"), l_AddPicutre(pExcel, strFilePath, stRow), Row_Ref, Col_Ref, stRow);

		stRow += rowCnt+1;

	}

	// Mass Table 
	GenerateNaturalFrequencyMassTable(pExcel, stRow);

	pExcel->SetHPageBreak(53, stRow, 28);

	return TRUE;

}

BOOL CBrdgSeisEvalKISTEC2015_Report::GenerateNaturalFrequencyMassTable(IExcel* pExcel, int& stRow)
{
	CString RptBlock;
	int rowCnt = 0;
	short Row_Ref = 0, Col_Ref = 0;

	int nModeNum = m_pDoc->m_pPostCtrl->GetNumEigvPost();
	if (nModeNum < 0)  ASSERT(0);

	RptBlock = _T("[NF]Mass_Table_Head");

	rowCnt = CopyRangeSubTitle(pExcel, RptBlock, stRow);
	stRow += rowCnt + 1;

	RptBlock = _T("[NF]Mass_Table");

	rowCnt = CopyRangeSubTitle(pExcel, RptBlock, stRow);
	GetCellIDByName(pExcel, RptBlock, Row_Ref, Col_Ref);
	stRow += rowCnt;

	for (int i = 0; i < nModeNum; ++i)
	{
		int iInsertRow = stRow + 2 + i;
		if(pExcel->GetExcelType() == XLTYPE::TYPE_MSEXCEL)
		{
			// 언제부터인가 테이블 행이 많아지면 엑셀 오류가 발생됨;; 
			// 그래서 10행 정도마다 Sleep 동작 적용함.
			if (i%11 == 10)  
			{
				::Sleep(200);
			}
		}

		if (i != nModeNum - 2)
		{
			if (!CopyCellForRoof(pExcel, _T("Mode"), Row_Ref, Col_Ref, iInsertRow))
				i--;
		}
	}

	T_FREQ_D FreqData;
	T_MPAM_D MassData;

	double dMaxUX = 0;
	double dMaxUY = 0;
	double dMaxUZ = 0;

	for (int i = 0; i < nModeNum; ++i)
	{
		m_pDoc->m_pPostCtrl->GetEiFreq(i + 1, FreqData);
		m_pDoc->m_pPostCtrl->GetEiMpam(i + 1, MassData);

		if (MassData.dblMpam[0] > dMaxUX) dMaxUX = MassData.dblMpam[0];
		if (MassData.dblMpam[1] > dMaxUY) dMaxUY = MassData.dblMpam[1];
		if (MassData.dblMpam[2] > dMaxUZ) dMaxUZ = MassData.dblMpam[2];
	}


	for (int i = 0; i < nModeNum; ++i)
	{
		m_pDoc->m_pPostCtrl->GetEiFreq(i + 1, FreqData);
		m_pDoc->m_pPostCtrl->GetEiMpam(i + 1, MassData);

		if (MassData.dblMpam[0] < cDGN_Zero) MassData.dblMpam[0] = 0;
		if (MassData.dblMpam[1] < cDGN_Zero) MassData.dblMpam[1] = 0;
		if (MassData.dblMpam[2] < cDGN_Zero) MassData.dblMpam[2] = 0;
		if (MassData.dblMpamSum[0] < cDGN_Zero) MassData.dblMpamSum[0] = 0;
		if (MassData.dblMpamSum[1] < cDGN_Zero) MassData.dblMpamSum[1] = 0;
		if (MassData.dblMpamSum[2] < cDGN_Zero) MassData.dblMpamSum[2] = 0;

		int iMergeRow = stRow - 4;
		PrintCellData(pExcel, _T("Mode"), i + 1, Row_Ref, Col_Ref, iMergeRow);

		PrintCellData(pExcel, _T("Period"), FreqData.dblFreq[2], Row_Ref, Col_Ref, iMergeRow);

		PrintCellData(pExcel, _T("UX"), MassData.dblMpam[0], Row_Ref, Col_Ref, iMergeRow);

		PrintCellData(pExcel, _T("UY"), MassData.dblMpam[1], Row_Ref, Col_Ref, iMergeRow);

		PrintCellData(pExcel, _T("UZ"), MassData.dblMpam[2], Row_Ref, Col_Ref, iMergeRow);

		if (abs(dMaxUX - MassData.dblMpam[0]) < cDGN_Zero) PrintCellData(pExcel, _T("MainMode"), _T("교축방향"), Row_Ref, Col_Ref, iMergeRow);
		if (abs(dMaxUY - MassData.dblMpam[1]) < cDGN_Zero) PrintCellData(pExcel, _T("MainMode"), _T("교축직각방향"), Row_Ref, Col_Ref, iMergeRow);
		if (abs(dMaxUZ - 0) > cDGN_Zero)
		{
			if (abs(dMaxUZ - MassData.dblMpam[2]) < cDGN_Zero) PrintCellData(pExcel, _T("MainMode"), _T("연직방향"), Row_Ref, Col_Ref, iMergeRow);
		}

		PrintCellData(pExcel, _T("SumUX"), MassData.dblMpamSum[0], Row_Ref, Col_Ref, iMergeRow);

		PrintCellData(pExcel, _T("SumUY"), MassData.dblMpamSum[1], Row_Ref, Col_Ref, iMergeRow);

		PrintCellData(pExcel, _T("SumUZ"), MassData.dblMpamSum[2], Row_Ref, Col_Ref, iMergeRow);

		if (i != nModeNum - 1) stRow++;
	}

	short Col_Mode, Col_UZ;

	RptBlock = _T("Mode");
	GetCellIDByName(pExcel, RptBlock, Row_Ref, Col_Mode);
	RptBlock = _T("SumUZ");
	GetCellIDByName(pExcel, RptBlock, Row_Ref, Col_UZ);

	RptBlock = _T("[NF]Mass_Graph_Head");
	rowCnt = CopyRangeSubTitle(pExcel, RptBlock, stRow);

	auto ChartItemD = [&]()
	{
		T_CHART_ITEM_D rData;
		rData.Initialize();

		//rData.bShowSymbol = TRUE;

		///////////////////////////////////////////////////////////////////////////
		// BackGround
		rData.StyleD.BackGroundD.dwCompFillStyle = CX_SOLID_FILL;
		rData.StyleD.BackGroundD.colColor = CXCLR_WHITE;

		// Display
		rData.StyleD.DisplayD.dwGraphStyle = CX_GRAPH_XYSCATTERG_EX;
		rData.StyleD.DisplayD.dwAxisStyle = CX_AXIS_XYSCATTER;
		rData.StyleD.DisplayD.colDispColor = CXCLR_WHITE;
		rData.StyleD.DisplayD.colGridColor = CXCLR_GRAY40;
		rData.StyleD.DisplayD.dwCompFillStyle = CX_SOLID_FILL;
		rData.StyleD.DisplayD.dwCompBorderStyle = CX_NO_BORDER;
		rData.StyleD.DisplayD.nMeasurement = CX_PERCENT;
		rData.StyleD.DisplayD.nFontSize = 8;
		rData.StyleD.DisplayD.bSizeDefaults = TRUE;
		rData.StyleD.DisplayD.sFaceName = _LS(IDS_GPS_GRAPH_FONT_ARIAL);
		CRect rect(0.2, 0, 100, 97.0);
		rData.StyleD.DisplayD.rectGraph = rect;
		rData.StyleD.DisplayD.bUseEmptyGraph = TRUE;
		rData.StyleD.DisplayD.bUseMaxScale = FALSE;
		rData.StyleD.DisplayD.bAlwaysShowZero = TRUE;

		// X-Axis Label
		rData.StyleD.XAxisD.nMeasurement = SRGraphComponent::PERCENT;
		rData.StyleD.XAxisD.nLocationPoint = SRGraphLabel::TopCenter;
		rData.StyleD.XAxisD.nTextPositionX = 50.0;
		rData.StyleD.XAxisD.nTextPositionY = 95.0;
		rData.StyleD.XAxisD.bBlockMember = FALSE;
		rData.StyleD.XAxisD.nLabelStandard = SRGraphLabel::Normal;
		rData.StyleD.XAxisD.colLabel = CXCLR_WHITE;
		rData.StyleD.XAxisD.dFontSize = 9;
		rData.StyleD.XAxisD.sFaceName = _LS(IDS_GPS_GRAPH_FONT_ARIAL);

		// Y-Axis Label  
		rData.StyleD.YAxisD.nMeasurement = SRGraphComponent::PERCENT;
		rData.StyleD.YAxisD.nLocationPoint = SRGraphLabel::MidCenter;
		rData.StyleD.YAxisD.nTextPositionX = 2.0;
		rData.StyleD.YAxisD.nTextPositionY = 50.0;
		rData.StyleD.YAxisD.bBlockMember = FALSE;
		rData.StyleD.YAxisD.nLabelStandard = SRGraphLabel::Upward;
		rData.StyleD.YAxisD.colLabel = CXCLR_WHITE;
		rData.StyleD.YAxisD.dFontSize = 9;
		rData.StyleD.YAxisD.sFaceName = _LS(IDS_GPS_GRAPH_FONT_ARIAL);
		///////////////////////////////////////////////////////////////////////////

		rData.StyleD.DisplayD.sFaceName = _LS(IDS_GRAPH_FONT_ARIAL);
		rData.StyleD.DisplayD.bAlwaysShowZero = FALSE;
		rData.StyleD.XAxisD.nTextPositionY = 93.5;
		rData.StyleD.YAxisD.nTextPositionX = 1.5;

		rData.StyleD.XAxisD.sFaceName = _LS(IDS_GRAPH_FONT_ARIAL);
		rData.StyleD.YAxisD.sFaceName = _LS(IDS_GRAPH_FONT_ARIAL);
		rData.StyleD.XAxisD.sAxisLabelText = _T("진동모드");
		rData.StyleD.YAxisD.sAxisLabelText = _T("누적질량참여율(%)");

		// Legend Setting
		rData.StyleD.LegendD.dKeySize = 2.;
		rData.StyleD.LegendD.dXMargin = 0.5;
		rData.StyleD.LegendD.dYMargin = 0.5;
		rData.StyleD.LegendD.dwLegendKeyStyle = CX_KEY_BOX;
		rData.StyleD.LegendD.bLegend = TRUE;
		rData.StyleD.LegendD.nFontSize = 8;
		rData.StyleD.LegendD.dKeySize = 2.;
		rData.StyleD.LegendD.dXMargin = 0.5;
		rData.StyleD.LegendD.dYMargin = 0.5;
		rData.StyleD.LegendD.nJustification = 0;
		rData.StyleD.LegendD.nKeyAnnotationCharCount = 8;
		rData.StyleD.LegendD.nKeyAnnotationLineCount = 1;
		rData.StyleD.LegendD.nScopeImin = -1;
		rData.StyleD.LegendD.nScopeImax = -1;
		rData.StyleD.LegendD.nScopeGmin = 0;
		rData.StyleD.LegendD.bLegendKeyFirst = TRUE;

		CRect rectLegend(24, 94, 100., 100.);
		rData.StyleD.LegendD.rectLegend = rectLegend;

		rData.StyleD.LegendD.bLegendLockRC = TRUE;

		rData.StyleD.LegendD.bSizeDefaults = TRUE;

		rData.StyleD.LegendD.LabelD.nMeasurement = SRGraphComponent::PERCENT;
		rData.StyleD.LegendD.LabelD.nLocationPoint = SRGraphLabel::MidLeft;
		rData.StyleD.LegendD.LabelD.nTextPositionX = 7.0;
		rData.StyleD.LegendD.LabelD.nTextPositionY = 95.5;
		rData.StyleD.LegendD.LabelD.dFontSize = 9;
		rData.StyleD.LegendD.LabelD.bBlockMember = FALSE;
		rData.StyleD.LegendD.LabelD.nLabelStandard = SRGraphLabel::Normal;

		rData.StyleD.LegendD.aLegendTitle.SetSize(3);
		rData.StyleD.LegendD.aLegendTitle[0] = _T(" UX");
		rData.StyleD.LegendD.aLegendTitle[1] = _T(" UY");
		rData.StyleD.LegendD.aLegendTitle[2] = _T(" UZ");

		rData.aValues.SetSize(3);
		for(int i = 0; i < 3; ++i)
		{
			rData.aValues[i].aXList.SetSize(nModeNum);
			rData.aValues[i].aYList.SetSize(nModeNum);
		}
		for(int nMode = 1; nMode <= nModeNum; ++nMode)
		{
			m_pDoc->m_pPostCtrl->GetEiMpam(nMode, MassData);
			const int iIdx = nMode - 1;
			for(int j = 0; j < 3; ++j)
			{
				rData.aValues[j].aXList[iIdx] = nMode;
				rData.aValues[j].aYList[iIdx] = max(MassData.dblMpamSum[j], 0.0);
			}
		}

		rData.StyleD.DisplayD.bMinMaxRange = TRUE;
		rData.StyleD.DisplayD.dMaxRangeX = nModeNum;
		rData.StyleD.DisplayD.dMaxRangeY = 100.0;
		rData.StyleD.DisplayD.dMinRangeX = 0.0;
		rData.StyleD.DisplayD.dMinRangeY = 0.0;

		rData.StyleD.DisplayD.bScaleLimit = FALSE;
		rData.StyleD.DisplayD.bUseMaxScale = TRUE;

		return rData;
	}();

	CString strFilePath;
	strFilePath.Format(_T("%sEigenMass_Graph.bmp"), m_strPicPath);
	T_UCHT_D UchtD; UchtD.Initialize(D_TYPE_RS_FUNC);
	m_pDoc->MakeGraphAndBmpImage(ChartItemD, strFilePath, &UchtD);

	RptBlock = _T("[NF]Chart");
	rowCnt = CopyRangeSubTitle(pExcel, RptBlock, stRow);
	GetCellIDByName(pExcel, RptBlock, Row_Ref, Col_Ref);

	auto l_AddPicutre = [this](IExcel* pExcel, CString& strFileName, const int& stRow)
	{
		int nH = 16;
		int nW = 26;
		CString strPath = strFileName;
		if(WaitForFile(strPath))
		{
			pExcel->AddPicture_Count(stRow, 2, nH, nW, strPath);
		}
		return TRUE;
	};

	PrintCellData(pExcel, _T("NF_Chart_Image"), l_AddPicutre(pExcel, strFilePath, stRow), Row_Ref, Col_Ref, stRow);

	stRow += rowCnt;

	/*
	ChartData chData;

	CString sMassStt = pExcel->GetCell(stRow - nModeNum - 1, Col_Mode);
	CString sMassEnd = pExcel->GetCell(stRow - 2, Col_UZ);
	CString MassSourceData = sMassStt + _T(":") + sMassEnd;

	chData.SourceData = MassSourceData;


	//chData.SourceData = _T("C116:W130");

	chData.bSetEachScatterData = TRUE;
	chData.aSeries.SetSize(3);
	chData.aSeries.SetAt(0, _T("UX"));
	chData.aSeries.SetAt(1, _T("UY"));
	chData.aSeries.SetAt(2, _T("UZ"));

	chData.xTitle = _T("진동 모드");
	chData.yTitle = _T("누적 질량 참여율(%)");
	chData.PlotBy = xlColumns;
	chData.bLegend = TRUE;
	chData.ChartType = xlXYScatterLines; //Chart Type : 꺾은선 xlLine, xlLineMarkers, 분산형 xlXYScatter, xlXYScatterSmooth, xlXYScatterSmoothNoMarkers, xlXYScatterLines, xlXYScatterLinesNoMarkers
	chData.color = xlNone;
	chData.dXmin = 0;

	int nModeForGraph = 0;
	double dTrashModeForGraph = nModeNum / 10.0;

	nModeForGraph = floor(dTrashModeForGraph);

	if (abs(dTrashModeForGraph - nModeForGraph) > cDGN_Zero)
	{
	nModeForGraph = floor(nModeNum / 10) * 10 + 10;
	}
	else
	{
	nModeForGraph = floor(nModeNum / 10) * 10;
	}

	chData.dXmax = nModeForGraph;
	chData.nXunit = nModeForGraph / 10;
	chData.dYmin = 0;
	chData.nYunit = 10;
	chData.dYmax = 100;

	double dMassGraphWidth  = 423.0;
	double dMassGraphHeight = 258.5;

	pExcel->AddChartScatter(chData, stRow + 2, 2, dMassGraphWidth, dMassGraphHeight);
	::Sleep(1000);
	stRow = stRow + 18;// int(floor(dMassGraphHeight / 12.0));
	*/
	return TRUE;
}

BOOL CBrdgSeisEvalKISTEC2015_Report::GenerateBrdgInfoSheet(IExcel* pExcel, T_SBRI_D& BrdgInfo, int& stRow)
{
	CString RptBlock;
	int rowCnt = 0;
	short Row_Ref = 0, Col_Ref = 0;

	CString strBrdgUpper;
	CString strBrdgAbutmentType;
	CString strBrdgAbutmentBase;
	CString strBrdgPierType;
	CString strBrdgPierBase;
	CString strBrdgSupportMatl;
	CString strBrdgPierSect;

	if (BrdgInfo.nUpper == 1)	strBrdgUpper = _LS(IDS_DGN_SEIS_PSC_I_GIRDER);
	else if (BrdgInfo.nUpper == 2)	strBrdgUpper = _LS(IDS_DGN_SEIS_PSC_BOX_GIRDER);
	else if (BrdgInfo.nUpper == 3)	strBrdgUpper = _LS(IDS_DGN_SEIS_RC_SLAB);
	else if (BrdgInfo.nUpper == 4)	strBrdgUpper = _LS(IDS_DGN_SEIS_STEEL_I_GIRDER);
	else if (BrdgInfo.nUpper == 5)	strBrdgUpper = _LS(IDS_DGN_SEIS_STEEL_BOX_GIRDER);

	if (BrdgInfo.nAbutmentType == 1)	strBrdgAbutmentType = _LS(IDS_DGN_SEIS_REV_T);
	else if (BrdgInfo.nAbutmentType == 2)	strBrdgAbutmentType = _LS(IDS_DGN_SEIS_GRAV);
	else if (BrdgInfo.nAbutmentType == 3)	strBrdgAbutmentType = _LS(IDS_DGN_SEIS_SUB_WALL);
	else if (BrdgInfo.nAbutmentType == 4)	strBrdgAbutmentType = _LS(IDS_DGN_SEIS_BOX);
	else if (BrdgInfo.nAbutmentType == 5)	strBrdgAbutmentType = _LS(IDS_DGN_SEIS_RAMAN);
	else if (BrdgInfo.nAbutmentType == 6)	strBrdgAbutmentType = _LS(IDS_DGN_SEIS_MID_CON);

	if (BrdgInfo.nAbutmentBase == 1)	strBrdgAbutmentBase = _LS(IDS_DGN_SEIS_BASE);
	else if (BrdgInfo.nAbutmentBase == 2)	strBrdgAbutmentBase = _LS(IDS_DGN_SEIS_MASS_BASE);
	else if (BrdgInfo.nAbutmentBase == 3)	strBrdgAbutmentBase = _LS(IDS_DGN_SEIS_PHC);
	else if (BrdgInfo.nAbutmentBase == 4)	strBrdgAbutmentBase = _LS(IDS_DGN_SEIS_STR);
	else if (BrdgInfo.nAbutmentBase == 5)	strBrdgAbutmentBase = _LS(IDS_DGN_SEIS_BASE_DT);
	else if (BrdgInfo.nAbutmentBase == 6)	strBrdgAbutmentBase = _LS(IDS_DGN_SEIS_WALL);
	else if (BrdgInfo.nAbutmentBase == 7)	strBrdgAbutmentBase = _LS(IDS_DGN_SEIS_ZOOM);

	if (BrdgInfo.nPierType == 1)	strBrdgPierType = _LS(IDS_DGN_SEIS_T);
	else if (BrdgInfo.nPierType == 2)	strBrdgPierType = _LS(IDS_DGN_SEIS_DOOR);
	else if (BrdgInfo.nPierType == 3)	strBrdgPierType = _LS(IDS_DGN_SEIS_TORCH);
	else if (BrdgInfo.nPierType == 4)	strBrdgPierType = _LS(IDS_DGN_SEIS_RAMAN);

	if (BrdgInfo.nPierBase == 1)	strBrdgPierBase = _LS(IDS_DGN_SEIS_BASE);
	else if (BrdgInfo.nPierBase == 2)	strBrdgPierBase = _LS(IDS_DGN_SEIS_MASS_BASE);
	else if (BrdgInfo.nPierBase == 3)	strBrdgPierBase = _LS(IDS_DGN_SEIS_PHC);
	else if (BrdgInfo.nPierBase == 4)	strBrdgPierBase = _LS(IDS_DGN_SEIS_STR);
	else if (BrdgInfo.nPierBase == 5)	strBrdgPierBase = _LS(IDS_DGN_SEIS_BASE_DT);
	else if (BrdgInfo.nPierBase == 6)	strBrdgPierBase = _LS(IDS_DGN_SEIS_WALL);
	else if (BrdgInfo.nPierBase == 7)	strBrdgPierBase = _LS(IDS_DGN_SEIS_ZOOM);

	if (BrdgInfo.nSupportMatl == 1)	strBrdgSupportMatl = _LS(IDS_DGN_SEIS_HIGH_STR);
	else if (BrdgInfo.nSupportMatl == 2)	strBrdgSupportMatl = _LS(IDS_DGN_SEIS_ELAST_BOT);
	else if (BrdgInfo.nSupportMatl == 3)	strBrdgSupportMatl = _LS(IDS_DGN_SEIS_POT_BOT);

	if (BrdgInfo.nPierSect == 1)	strBrdgPierSect = _LS(IDS_DGN_SEIS_POT_CIRCLE);
	else if (BrdgInfo.nPierSect == 2)	strBrdgPierSect = _LS(IDS_DGN_SEIS_POT_RECT);
	else if (BrdgInfo.nPierSect == 3)	strBrdgPierSect = _LS(IDS_DGN_SEIS_POT_TRACK);


	RptBlock = _T("[BI]Brdg_Info_Title");
	rowCnt = CopyRange(pExcel, RptBlock, stRow);
	stRow += rowCnt;

	RptBlock = _T("[BI]Brdg_Info_Basic");
	rowCnt = CopyRangeSubTitle(pExcel, RptBlock, stRow);
	GetCellIDByName(pExcel, RptBlock, Row_Ref, Col_Ref);

	PrintCellData(pExcel, _T("Brdg_Name"), BrdgInfo.strBrgName, Row_Ref, Col_Ref, stRow);
	PrintCellData(pExcel, _T("Brdg_Admin"), BrdgInfo.strManager, Row_Ref, Col_Ref, stRow);
	PrintCellData(pExcel, _T("Brdg_Location"), BrdgInfo.strLocation, Row_Ref, Col_Ref, stRow);
	PrintCellData(pExcel, _T("Brdg_Num"), BrdgInfo.strLineNum, Row_Ref, Col_Ref, stRow);
	PrintCellData(pExcel, _T("Brdg_Site"), BrdgInfo.strOrgLoc, Row_Ref, Col_Ref, stRow);
	PrintCellData(pExcel, _T("Brdg_Time"), BrdgInfo.nBuildYear, Row_Ref, Col_Ref, stRow);
	PrintCellData(pExcel, _T("Brdg_Total_Length"), BrdgInfo.dTotalLen, Row_Ref, Col_Ref, stRow);

	CString strSpan;
	strSpan = CStrParser::ConvtDistArr2DistStr(BrdgInfo.aSpan);

	PrintCellData(pExcel, _T("Brdg_Rapid_Length"), strSpan, Row_Ref, Col_Ref, stRow);
	PrintCellData(pExcel, _T("Brdg_Width"), BrdgInfo.dBrgWidth, Row_Ref, Col_Ref, stRow);

	if (BrdgInfo.nUpper == 0) PrintCellData(pExcel, _T("Brdg_Top_Form"), BrdgInfo.strUpper, Row_Ref, Col_Ref, stRow);
	else                      PrintCellData(pExcel, _T("Brdg_Top_Form"), strBrdgUpper, Row_Ref, Col_Ref, stRow);

	if (BrdgInfo.nAbutmentType == 0) PrintCellData(pExcel, _T("Brdg_Abutment_Form"), BrdgInfo.strAbutmentType, Row_Ref, Col_Ref, stRow);
	else                             PrintCellData(pExcel, _T("Brdg_Abutment_Form"), strBrdgAbutmentType, Row_Ref, Col_Ref, stRow);


	if (BrdgInfo.nAbutmentBase == 0) PrintCellData(pExcel, _T("Brdg_Abutment_Found"), BrdgInfo.strAbutmentBase, Row_Ref, Col_Ref, stRow);
	else                             PrintCellData(pExcel, _T("Brdg_Abutment_Found"), strBrdgAbutmentBase, Row_Ref, Col_Ref, stRow);

	if (BrdgInfo.nPierType == 0)     PrintCellData(pExcel, _T("Brdg_Pier_Form"), BrdgInfo.strPierType, Row_Ref, Col_Ref, stRow);
	else                             PrintCellData(pExcel, _T("Brdg_Pier_Form"), strBrdgPierType, Row_Ref, Col_Ref, stRow);

	if (BrdgInfo.nPierBase == 0)     PrintCellData(pExcel, _T("Brdg_Pier_Found"), BrdgInfo.strPierBase, Row_Ref, Col_Ref, stRow);
	else                             PrintCellData(pExcel, _T("Brdg_Pier_Found"), strBrdgPierBase, Row_Ref, Col_Ref, stRow);


	if (BrdgInfo.nSupportMatl == 0)  PrintCellData(pExcel, _T("Brdg_Support_Data"), BrdgInfo.strSupportMatl, Row_Ref, Col_Ref, stRow);
	else                             PrintCellData(pExcel, _T("Brdg_Support_Data"), strBrdgSupportMatl, Row_Ref, Col_Ref, stRow);

	PrintCellData(pExcel, _T("Brdg_Support_Capa_Abutment"), BrdgInfo.dSupportCapaAbut, Row_Ref, Col_Ref, stRow);
	PrintCellData(pExcel, _T("Brdg_Support_Capa_Pier"), BrdgInfo.dSupportCapaPier, Row_Ref, Col_Ref, stRow);


	if (BrdgInfo.nPierSect == 0) PrintCellData(pExcel, _T("Brdg_Pier_Shape"), BrdgInfo.strPierSect, Row_Ref, Col_Ref, stRow);
	else                         PrintCellData(pExcel, _T("Brdg_Pier_Shape"), strBrdgPierSect, Row_Ref, Col_Ref, stRow);

 	
	if (BrdgInfo.SpfcK == 0)
	{
		PrintCellData(pExcel, _T("Brdg_RS"), _LS(IDS_DGN__NONE), Row_Ref, Col_Ref, stRow);
	}
	else
	{
		T_SPFC_D SpfcD;
		if (!m_pDoc->m_pAttrCtrl->GetSpfc(BrdgInfo.SpfcK, SpfcD)) ASSERT(0);

		PrintCellData(pExcel, _T("Brdg_RS"), SpfcD.SpecFuncName, Row_Ref, Col_Ref, stRow);
	}
	
	stRow += rowCnt;

	return TRUE;

}

BOOL CBrdgSeisEvalKISTEC2015_Report::GenerateBrdgStrengthInfoSheet(IExcel* pExcel, int& stRow, const int& iStartStrength, double& dFck, double& dFy, const CString& strEvgp)
{
	CString RptBlock;
	int rowCnt = 0;
	short Row_Ref = 0, Col_Ref = 0;

	if (iStartStrength == 0)
	{
		RptBlock = _T("[BI]Brdg_Info_Strength_Head");
		rowCnt = CopyRangeSubTitle(pExcel, RptBlock, stRow);
		stRow += rowCnt;
	}

	RptBlock = _T("[BI]Brdg_Info_Strength");
	rowCnt = CopyRangeSubTitle(pExcel, RptBlock, stRow);
	GetCellIDByName(pExcel, RptBlock, Row_Ref, Col_Ref);

	PrintCellData(pExcel, _T("Brdg_Strength_Group"), strEvgp, Row_Ref, Col_Ref, stRow);
	PrintCellData(pExcel, _T("Brdg_Con_Strength"), dFck, Row_Ref, Col_Ref, stRow);
	PrintCellData(pExcel, _T("Brdg_Stl_Strength"), dFy, Row_Ref, Col_Ref, stRow);

	stRow += rowCnt;

	return TRUE;

}

BOOL CBrdgSeisEvalKISTEC2015_Report::GenerateBrdgElasticInfoSheet(IExcel* pExcel, int& stRow, const int& iStartStrength, double& dElastCon, double& dElastStl, const CString& strEvgp)
{
	CString RptBlock;
	int rowCnt = 0;
	short Row_Ref = 0, Col_Ref = 0;

	if (iStartStrength == 0)
	{
		RptBlock = _T("[BI]Brdg_Info_Elastic_Head");
		rowCnt = CopyRangeSubTitle(pExcel, RptBlock, stRow);
		stRow += rowCnt;
	}

	RptBlock = _T("[BI]Brdg_Info_Elastic");
	rowCnt = CopyRangeSubTitle(pExcel, RptBlock, stRow);
	GetCellIDByName(pExcel, RptBlock, Row_Ref, Col_Ref);

	PrintCellData(pExcel, _T("Brdg_Elastic_Group"), strEvgp, Row_Ref, Col_Ref, stRow);
	PrintCellData(pExcel, _T("Brdg_Con_Elastic"), dElastCon, Row_Ref, Col_Ref, stRow);
	PrintCellData(pExcel, _T("Brdg_Stl_Elastic"), dElastStl, Row_Ref, Col_Ref, stRow);


	stRow += rowCnt;


	return TRUE;

}

BOOL CBrdgSeisEvalKISTEC2015_Report::GenerateBrdgElseInfoSheet(IExcel* pExcel, int& stRow, const T_FIMP_D& FimpConcD1, const T_FIMP_D& FimpConcD3, const T_FIMP_D& FimpSteelD, const UINT& uncode)
{
	CString RptBlock;
	int rowCnt = 0;
	short Row_Ref = 0, Col_Ref = 0;

	RptBlock = _T("[BI]Brdg_Info_Program_Model");
	rowCnt = CopyRangeSubTitle(pExcel, RptBlock, stRow);
	GetCellIDByName(pExcel, RptBlock, Row_Ref, Col_Ref);

	if (FimpConcD1.nHysModelType == -1) PrintCellData(pExcel, _T("Brdg_Con_Model"), _T("-"), Row_Ref, Col_Ref, stRow);
	else if (FimpConcD1.nHysModelType == 0) PrintCellData(pExcel, _T("Brdg_Con_Model"), _LS(IDS_CMD2_FIMP_KENT_PARK), Row_Ref, Col_Ref, stRow);
	else if (FimpConcD1.nHysModelType == 1) PrintCellData(pExcel, _T("Brdg_Con_Model"), _LS(IDS_CMD2_FIMP_JCONCRETE), Row_Ref, Col_Ref, stRow);
	else if (FimpConcD1.nHysModelType == 2) PrintCellData(pExcel, _T("Brdg_Con_Model"), _LS(IDS_CMD2_FIMP_JBRIDGE), Row_Ref, Col_Ref, stRow);
	else if (FimpConcD1.nHysModelType == 3) PrintCellData(pExcel, _T("Brdg_Con_Model"), _LS(IDS_CMD2_FIMP_NAKOYA), Row_Ref, Col_Ref, stRow);
	else if (FimpConcD1.nHysModelType == 4) PrintCellData(pExcel, _T("Brdg_Con_Model"), _LS(IDS_CMD2_FIMP_CON_TRILIN), Row_Ref, Col_Ref, stRow);
	else if (FimpConcD1.nHysModelType == 5) PrintCellData(pExcel, _T("Brdg_Con_Model"), _LS(IDS_CMD2_CHINA_GB50010_02), Row_Ref, Col_Ref, stRow);
	else if (FimpConcD1.nHysModelType == 6) PrintCellData(pExcel, _T("Brdg_Con_Model"), _LS(IDS_CMD2_FIMP_MANDER), Row_Ref, Col_Ref, stRow);
	else if (FimpConcD1.nHysModelType == 7) PrintCellData(pExcel, _T("Brdg_Con_Model"), _LS(IDS_CMD2_FIMP_JBRIDGE_H24), Row_Ref, Col_Ref, stRow);
	else if (FimpConcD1.nHysModelType == 8) PrintCellData(pExcel, _T("Brdg_Con_Model"), _LS(IDS_CMD2_CHINA_GB50010_10), Row_Ref, Col_Ref, stRow);
	else if(FimpConcD1.nHysModelType == 99)
	{
		PrintCellData(pExcel, _T("Brdg_Sect_Str_Method"), _T("KDS 14 20 64"), Row_Ref, Col_Ref, stRow);
		PrintCellData(pExcel, _T("Brdg_Sect_Str_Code"), _LS(IDS_DGN_SEIS_PLAIN_CONCRETE_CODE), Row_Ref, Col_Ref, stRow);
		PrintCellData(pExcel, _T("Brdg_Con_Model"), _LS(IDS_DGN_SEIS_PLAIN_CONCRETE), Row_Ref, Col_Ref, stRow);
	}

	if (FimpConcD3.nHysModelType == -1) PrintCellData(pExcel, _T("Brdg_Con_Trans_Model"), _T("-"), Row_Ref, Col_Ref, stRow);
	else if (FimpConcD3.nHysModelType == 0) PrintCellData(pExcel, _T("Brdg_Con_Trans_Model"), _LS(IDS_CMD2_FIMP_KENT_PARK), Row_Ref, Col_Ref, stRow);
	else if (FimpConcD3.nHysModelType == 1) PrintCellData(pExcel, _T("Brdg_Con_Trans_Model"), _LS(IDS_CMD2_FIMP_JCONCRETE), Row_Ref, Col_Ref, stRow);
	else if (FimpConcD3.nHysModelType == 2) PrintCellData(pExcel, _T("Brdg_Con_Trans_Model"), _LS(IDS_CMD2_FIMP_JBRIDGE), Row_Ref, Col_Ref, stRow);
	else if (FimpConcD3.nHysModelType == 3) PrintCellData(pExcel, _T("Brdg_Con_Trans_Model"), _LS(IDS_CMD2_FIMP_NAKOYA), Row_Ref, Col_Ref, stRow);
	else if (FimpConcD3.nHysModelType == 4) PrintCellData(pExcel, _T("Brdg_Con_Trans_Model"), _LS(IDS_CMD2_FIMP_CON_TRILIN), Row_Ref, Col_Ref, stRow);
	else if (FimpConcD3.nHysModelType == 5) PrintCellData(pExcel, _T("Brdg_Con_Trans_Model"), _LS(IDS_CMD2_CHINA_GB50010_02), Row_Ref, Col_Ref, stRow);
	else if (FimpConcD3.nHysModelType == 6) PrintCellData(pExcel, _T("Brdg_Con_Trans_Model"), _LS(IDS_CMD2_FIMP_MANDER), Row_Ref, Col_Ref, stRow);
	else if (FimpConcD3.nHysModelType == 7) PrintCellData(pExcel, _T("Brdg_Con_Trans_Model"), _LS(IDS_CMD2_FIMP_JBRIDGE_H24), Row_Ref, Col_Ref, stRow);
	else if (FimpConcD3.nHysModelType == 8) PrintCellData(pExcel, _T("Brdg_Con_Trans_Model"), _LS(IDS_CMD2_CHINA_GB50010_10), Row_Ref, Col_Ref, stRow);
	else if (FimpConcD3.nHysModelType == 99) PrintCellData(pExcel, _T("Brdg_Con_Trans_Model"), _LS(IDS_DGN_SEIS_PLAIN_CONCRETE), Row_Ref, Col_Ref, stRow);

	if (FimpSteelD.nHysModelType == -1) PrintCellData(pExcel, _T("Brdg_Stl_Model"), _T("-"), Row_Ref, Col_Ref, stRow);
	else if (FimpSteelD.nHysModelType == 0) PrintCellData(pExcel, _T("Brdg_Stl_Model"), _LS(IDS_CMD2_FIMP_MENEGOTTO), Row_Ref, Col_Ref, stRow);
	else if (FimpSteelD.nHysModelType == 1) PrintCellData(pExcel, _T("Brdg_Stl_Model"), _LS(IDS_CMD2_FIMP_BILINEAR), Row_Ref, Col_Ref, stRow);
	else if (FimpSteelD.nHysModelType == 2) PrintCellData(pExcel, _T("Brdg_Stl_Model"), _LS(IDS_CMD2_FIMP_GEN_BILINEAR), Row_Ref, Col_Ref, stRow);
	else if (FimpSteelD.nHysModelType == 3) PrintCellData(pExcel, _T("Brdg_Stl_Model"), _LS(IDS_CMD2_FIMP_STL_TRILIN), Row_Ref, Col_Ref, stRow);
	else if (FimpSteelD.nHysModelType == 4) PrintCellData(pExcel, _T("Brdg_Stl_Model"), _LS(IDS_CMD2_FIMP_PARK_MODEL), Row_Ref, Col_Ref, stRow);
	else if (FimpSteelD.nHysModelType == 5) PrintCellData(pExcel, _T("Brdg_Stl_Model"), _LS(IDS_CMD2_FIMP_STL_JBRIDGE), Row_Ref, Col_Ref, stRow);
	else if (FimpSteelD.nHysModelType == 6) PrintCellData(pExcel, _T("Brdg_Stl_Model"), _LS(IDS_CMD2_CHINA_STL_GB50010_10), Row_Ref, Col_Ref, stRow);


	stRow += rowCnt;


	RptBlock = _T("[BI]Brdg_Info_Evaluation");
	rowCnt = CopyRangeSubTitle(pExcel, RptBlock, stRow);
	stRow += rowCnt;

	if (uncode == KISTEC_2015)
	{
		RptBlock = _T("[BI]Brdg_Info_Reference_KISTEC");
		rowCnt = CopyRangeSubTitle(pExcel, RptBlock, stRow);
		//GetCellIDByName(pExcel, RptBlock, Row_Ref, Col_Ref);
	}
	if (uncode == KISTEC_2019)
	{
		RptBlock = _T("[BI]Brdg_Info_Reference_KISTEC2019");
		rowCnt = CopyRangeSubTitle(pExcel, RptBlock, stRow);
		//GetCellIDByName(pExcel, RptBlock, Row_Ref, Col_Ref);
	}
	if(uncode == KALIS_2023)
	{
		RptBlock = _T("[BI]Brdg_Info_Reference_KALIS2023");
		rowCnt = CopyRangeSubTitle(pExcel, RptBlock, stRow);
		//GetCellIDByName(pExcel, RptBlock, Row_Ref, Col_Ref);
	}
	else
	{
		RptBlock = _T("[BI]Brdg_Info_Reference_KEC");
		rowCnt = CopyRangeSubTitle(pExcel, RptBlock, stRow);
		//GetCellIDByName(pExcel, RptBlock, Row_Ref, Col_Ref);
	}

	stRow += rowCnt;


	return TRUE;

}

BOOL CBrdgSeisEvalKISTEC2015_Report::GenerateSummarySheet_Info(IExcel* pExcel, int& stRow)
{
	CString RptBlock;
	int rowCnt = 0;
	short Row_Ref = 0, Col_Ref = 0;

	CString strCountry;
	CString strBrdgUpper;
	CString strBrdgAbutmentType;
	CString strBrdgAbutmentBase;
	CString strBrdgPierType;
	CString strBrdgPierBase;
	CString strBrdgSupportMatl;
	CString strBrdgPierSect;

	T_SBRI_D BrdgInfo;
	if (!m_pDoc->m_pAttrCtrl2->GetSbri(BrdgInfo)) BrdgInfo.Initialize();

	if (BrdgInfo.nUpper == 1)	strBrdgUpper = _LS(IDS_DGN_SEIS_PSC_I_GIRDER);
	else if (BrdgInfo.nUpper == 2)	strBrdgUpper = _LS(IDS_DGN_SEIS_PSC_BOX_GIRDER);
	else if (BrdgInfo.nUpper == 3)	strBrdgUpper = _LS(IDS_DGN_SEIS_RC_SLAB);
	else if (BrdgInfo.nUpper == 4)	strBrdgUpper = _LS(IDS_DGN_SEIS_STEEL_I_GIRDER);
	else if (BrdgInfo.nUpper == 5)	strBrdgUpper = _LS(IDS_DGN_SEIS_STEEL_BOX_GIRDER);

	if (BrdgInfo.nAbutmentType == 1)	strBrdgAbutmentType = _LS(IDS_DGN_SEIS_REV_T);
	else if (BrdgInfo.nAbutmentType == 2)	strBrdgAbutmentType = _LS(IDS_DGN_SEIS_GRAV);
	else if (BrdgInfo.nAbutmentType == 3)	strBrdgAbutmentType = _LS(IDS_DGN_SEIS_SUB_WALL);
	else if (BrdgInfo.nAbutmentType == 4)	strBrdgAbutmentType = _LS(IDS_DGN_SEIS_BOX);
	else if (BrdgInfo.nAbutmentType == 5)	strBrdgAbutmentType = _LS(IDS_DGN_SEIS_RAMAN);
	else if (BrdgInfo.nAbutmentType == 6)	strBrdgAbutmentType = _LS(IDS_DGN_SEIS_MID_CON);

	if (BrdgInfo.nAbutmentBase == 1)	strBrdgAbutmentBase = _LS(IDS_DGN_SEIS_BASE);
	else if (BrdgInfo.nAbutmentBase == 2)	strBrdgAbutmentBase = _LS(IDS_DGN_SEIS_MASS_BASE);
	else if (BrdgInfo.nAbutmentBase == 3)	strBrdgAbutmentBase = _LS(IDS_DGN_SEIS_PHC);
	else if (BrdgInfo.nAbutmentBase == 4)	strBrdgAbutmentBase = _LS(IDS_DGN_SEIS_STR);
	else if (BrdgInfo.nAbutmentBase == 5)	strBrdgAbutmentBase = _LS(IDS_DGN_SEIS_BASE_DT);
	else if (BrdgInfo.nAbutmentBase == 6)	strBrdgAbutmentBase = _LS(IDS_DGN_SEIS_WALL);
	else if (BrdgInfo.nAbutmentBase == 7)	strBrdgAbutmentBase = _LS(IDS_DGN_SEIS_ZOOM);

	if (BrdgInfo.nPierType == 1)	strBrdgPierType = _LS(IDS_DGN_SEIS_T);
	else if (BrdgInfo.nPierType == 2)	strBrdgPierType = _LS(IDS_DGN_SEIS_DOOR);
	else if (BrdgInfo.nPierType == 3)	strBrdgPierType = _LS(IDS_DGN_SEIS_TORCH);
	else if (BrdgInfo.nPierType == 4)	strBrdgPierType = _LS(IDS_DGN_SEIS_RAMAN);

	if (BrdgInfo.nPierBase == 1)	strBrdgPierBase = _LS(IDS_DGN_SEIS_BASE);
	else if (BrdgInfo.nPierBase == 2)	strBrdgPierBase = _LS(IDS_DGN_SEIS_MASS_BASE);
	else if (BrdgInfo.nPierBase == 3)	strBrdgPierBase = _LS(IDS_DGN_SEIS_PHC);
	else if (BrdgInfo.nPierBase == 4)	strBrdgPierBase = _LS(IDS_DGN_SEIS_STR);
	else if (BrdgInfo.nPierBase == 5)	strBrdgPierBase = _LS(IDS_DGN_SEIS_BASE_DT);
	else if (BrdgInfo.nPierBase == 6)	strBrdgPierBase = _LS(IDS_DGN_SEIS_WALL);
	else if (BrdgInfo.nPierBase == 7)	strBrdgPierBase = _LS(IDS_DGN_SEIS_ZOOM);

	if (BrdgInfo.nSupportMatl == 1)	strBrdgSupportMatl = _LS(IDS_DGN_SEIS_HIGH_STR);
	else if (BrdgInfo.nSupportMatl == 2)	strBrdgSupportMatl = _LS(IDS_DGN_SEIS_ELAST_BOT);
	else if (BrdgInfo.nSupportMatl == 3)	strBrdgSupportMatl = _LS(IDS_DGN_SEIS_POT_BOT);

	if (BrdgInfo.nPierSect == 1)	strBrdgPierSect = _LS(IDS_DGN_SEIS_POT_CIRCLE);
	else if (BrdgInfo.nPierSect == 2)	strBrdgPierSect = _LS(IDS_DGN_SEIS_POT_RECT);
	else if (BrdgInfo.nPierSect == 3)	strBrdgPierSect = _LS(IDS_DGN_SEIS_POT_TRACK);

	CCMMvcdDlg dlg;
	CStringArray      aCodeName;
	CArray<int, int>  aCodeData;
	dlg.GetCodeNameList(aCodeName);
	dlg.GetCodeDataList(aCodeData);

	CString strCurCodeName;
	for (int i = 0; i < aCodeData.GetSize(); i++)
	{
		if (aCodeData[i] == BrdgInfo.nMvCode)
		{
			strCurCodeName = aCodeName[i];
		}
	}

	CString strCurTypeName;

	if (BrdgInfo.nMvCode != D_MOVE_CODE_NONE)
	{
		if (BrdgInfo.nMvCode == D_MOVE_CODE_EURO_BS)
		{
			switch (BrdgInfo.nMvhlStd)
			{
			case 0:
				strCurTypeName = _LS(IDS_CMD_MVHL_EURO_BS_ROADBRIDGE);
				break;
			case 1:
				strCurTypeName = _LS(IDS_CMD_MVHL_EURO_BS_FOOTBRIDGE);
				break;
			case 2:
				strCurTypeName = _LS(IDS_CMD_MVHL_EURO_BS_ROADBRIDGE_FATIGUE);
				break;
			case 3:
				strCurTypeName = _LS(IDS_CMD_MVHL_EURO_BS_TRAIL_LOAD);
				break;
			default:
				ASSERT(0);
				break;
			}
		}
		else
		{
			if (BrdgInfo.nMvhlStd >= 0) {
				strCurTypeName = CVehlDB::GetTypeNameAr()[BrdgInfo.nMvhlStd];
			}
		}
	}

	RptBlock = _T("[S]Title_Summary");
	rowCnt = CopyRange(pExcel, RptBlock, stRow);
	stRow += rowCnt;
	RptBlock = _T("[S]Result_Brdg_Info");
	rowCnt = CopyRangeSubTitle(pExcel, RptBlock, stRow);
	GetCellIDByName(pExcel, RptBlock, Row_Ref, Col_Ref);

	PrintCellData(pExcel, _T("Result_Brdg_Name"), BrdgInfo.strBrgName, Row_Ref, Col_Ref, stRow);
	PrintCellData(pExcel, _T("Result_Brdg_Location"), BrdgInfo.strLocation, Row_Ref, Col_Ref, stRow);
	PrintCellData(pExcel, _T("Result_Brdg_Load_Country"), strCurCodeName, Row_Ref, Col_Ref, stRow);
	PrintCellData(pExcel, _T("Result_Brdg_Load_Standard"), strCurTypeName, Row_Ref, Col_Ref, stRow);
	PrintCellData(pExcel, _T("Result_Brdg_Load_Specific"), BrdgInfo.strMvhlType, Row_Ref, Col_Ref, stRow);
	PrintCellData(pExcel, _T("Result_Brdg_Load_Specific2"), BrdgInfo.strMvhlName, Row_Ref, Col_Ref, stRow);
	PrintCellData(pExcel, _T("Result_Brdg_Total_Length"), BrdgInfo.dTotalLen, Row_Ref, Col_Ref, stRow);

	if (BrdgInfo.nSeismicClass == 0)
	{
		PrintCellData(pExcel, _T("Result_Brdg_Seis_Class"), _T("특"), Row_Ref, Col_Ref, stRow);
	}
	else
	{
		PrintCellData(pExcel, _T("Result_Brdg_Seis_Class"), BrdgInfo.nSeismicClass, Row_Ref, Col_Ref, stRow);
	}

	PrintCellData(pExcel, _T("Result_Brdg_Seis_Section"), BrdgInfo.nEarthquake + 1, Row_Ref, Col_Ref, stRow);
	PrintCellData(pExcel, _T("Result_Brdg_Width"), BrdgInfo.dBrgWidth, Row_Ref, Col_Ref, stRow);

	if (BrdgInfo.nUpper == 0) PrintCellData(pExcel, _T("Result_Brdg_Top_Form"), BrdgInfo.strUpper, Row_Ref, Col_Ref, stRow);
	else                      PrintCellData(pExcel, _T("Result_Brdg_Top_Form"), strBrdgUpper, Row_Ref, Col_Ref, stRow);

	if (BrdgInfo.nAbutmentType == 0) PrintCellData(pExcel, _T("Result_Brdg_Abutment_Form"), BrdgInfo.strAbutmentType, Row_Ref, Col_Ref, stRow);
	else                             PrintCellData(pExcel, _T("Result_Brdg_Abutment_Form"), strBrdgAbutmentType, Row_Ref, Col_Ref, stRow);


	if (BrdgInfo.nAbutmentBase == 0) PrintCellData(pExcel, _T("Result_Brdg_Abutment_Found"), BrdgInfo.strAbutmentBase, Row_Ref, Col_Ref, stRow);
	else                             PrintCellData(pExcel, _T("Result_Brdg_Abutment_Found"), strBrdgAbutmentBase, Row_Ref, Col_Ref, stRow);

	if (BrdgInfo.nPierType == 0)     PrintCellData(pExcel, _T("Result_Brdg_Pier_Form"), BrdgInfo.strPierType, Row_Ref, Col_Ref, stRow);
	else                             PrintCellData(pExcel, _T("Result_Brdg_Pier_Form"), strBrdgPierType, Row_Ref, Col_Ref, stRow);

	if (BrdgInfo.nPierBase == 0)     PrintCellData(pExcel, _T("Result_Brdg_Pier_Found"), BrdgInfo.strPierBase, Row_Ref, Col_Ref, stRow);
	else                             PrintCellData(pExcel, _T("Result_Brdg_Pier_Found"), strBrdgPierBase, Row_Ref, Col_Ref, stRow);


	if (BrdgInfo.nSupportMatl == 0)  PrintCellData(pExcel, _T("Result_Brdg_Support_Data"), BrdgInfo.strSupportMatl, Row_Ref, Col_Ref, stRow);
	else                             PrintCellData(pExcel, _T("Result_Brdg_Support_Data"), strBrdgSupportMatl, Row_Ref, Col_Ref, stRow);

	PrintCellData(pExcel, _T("Result_Brdg_Support_Capa_Abutment"), BrdgInfo.dSupportCapaAbut, Row_Ref, Col_Ref, stRow);
	PrintCellData(pExcel, _T("Result_Brdg_Support_Capa_Pier"), BrdgInfo.dSupportCapaPier, Row_Ref, Col_Ref, stRow);

	stRow += rowCnt;

	return TRUE;

}

BOOL CBrdgSeisEvalKISTEC2015_Report::GenerateSummaryPierTableHead(IExcel* pExcel, int& stRow, const CString& strEvgp)
{
	CString RptBlock;
	int rowCnt = 0;
	short Row_Ref = 0, Col_Ref = 0;

	RptBlock = _T("[S]Result_Summary_Head");
	rowCnt = CopyRangeSubTitle(pExcel, RptBlock, stRow);
	GetCellIDByName(pExcel, RptBlock, Row_Ref, Col_Ref);
	PrintCellData(pExcel, _T("[S]Name_Pier"), strEvgp, Row_Ref, Col_Ref, stRow);
	stRow += rowCnt;

	RptBlock = _T("[S]Result_Summary_Col_Head");
	rowCnt = CopyRangeSubTitle(pExcel, RptBlock, stRow);
	stRow += rowCnt;

	return TRUE;
}

BOOL CBrdgSeisEvalKISTEC2015_Report::GenerateSummaryPierTableBody(IExcel* pExcel, int& stRow, const PairKey& pEvgpCol, const CString& strColm)
{
	T_EVPM_D EvpmD;
	if (!GetEvpm(pEvgpCol.first, pEvgpCol.second, EvpmD)) ASSERT(0);

	T_BRDGEVAL_RESULT ResultD[2][2]; // Bottom/Top, Longitude/Transverse
	if (!GetResult(pEvgpCol, ResultD)) ASSERT(0); // 설계결과가 없는 Evaluation Group은 Skip

	EN_FAILURE_MODE enFailure[4] = { ResultD[0][0].EvalD.ModeD.enFalureMode,
									 ResultD[0][1].EvalD.ModeD.enFalureMode,
									 ResultD[1][0].EvalD.ModeD.enFalureMode,
									 ResultD[1][1].EvalD.ModeD.enFalureMode };

	EN_BEHAVIOR_TYPE enBehavior[2] = { (EvpmD.nBehavL == 0 ? RAHMEN : CANTILEVER),
									   (EvpmD.nBehavT == 0 ? RAHMEN : CANTILEVER) };

	CString RptBlock;
	int rowCnt = 0;
	short Row_Ref = 0, Col_Ref = 0;

	auto lambda_GetFailStr = [this](const EN_FAILURE_MODE& enFailure)
	{
		CString strFailure;
		switch (enFailure)
		{
		case TYPE1_BEND:
			strFailure = _LS(IDS_DGN_SEIS_FAILURE_MODE_BEND);
			break;
		case TYPE2_BEND_SHR:
			strFailure = _LS(IDS_DGN_SEIS_FAILURE_MODE_BEND_SHR);
			break;
		case TYPE3_SHR:
			strFailure = _LS(IDS_DGN_SEIS_FAILURE_MODE_SHR);
			break;
		case TYPE4_PLAIN:
			strFailure = _LS(IDS_DGN_SEIS_PLAIN_CONCRETE);
			break;
		default:
			strFailure = _T("-");
			break;
		}

		return strFailure;
	};

	CString strFailureBotLongi = lambda_GetFailStr(enFailure[0]);
	CString strFailureTopLongi = lambda_GetFailStr(enFailure[2]);
	CString strFailureBotTrans = lambda_GetFailStr(enFailure[1]);
	CString strFailureTopTrans = lambda_GetFailStr(enFailure[3]);

	if (enBehavior[0] == CANTILEVER && enBehavior[1] == CANTILEVER)
	{
		RptBlock = _T("[S]Result_Summary_Col_Type1");
		rowCnt = CopyRangeSubTitle(pExcel, RptBlock, stRow);
		GetCellIDByName(pExcel, RptBlock, Row_Ref, Col_Ref);
		PrintCellData(pExcel, _T("[S]Name_Col_Type1"), strColm, Row_Ref, Col_Ref, stRow);

		BOOL bOK = TRUE;
		if (ResultD[0][0].EvalD.bCheck)
		{
			PrintCellData(pExcel, _T("[S]FailMode_L_Type1"), strFailureBotLongi, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("[S]FPC_L_Type1"), ResultD[0][0].EvalD.ModeD.dFpc, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("[S]FPD_L_Type1"), ResultD[0][0].EvalD.EvalD.dFpd, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("[S]FR_L_Type1"), ResultD[0][0].EvalD.dRat, Row_Ref, Col_Ref, stRow);
			PrintCellDataMark(pExcel, _T("[S]Result_L_Type1"), Chk_OK_Sign(0, 1.0, ResultD[0][0].EvalD.dRat), Row_Ref, Col_Ref, stRow);

			if (ResultD[0][0].EvalD.dRat < 1.0) bOK = FALSE;
		}

		if (ResultD[0][1].EvalD.bCheck)
		{
			PrintCellData(pExcel, _T("[S]FailMode_T_Type1"), strFailureBotTrans, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("[S]FPC_T_Type1"), ResultD[0][1].EvalD.ModeD.dFpc, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("[S]FPD_T_Type1"), ResultD[0][1].EvalD.EvalD.dFpd, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("[S]FR_T_Type1"), ResultD[0][1].EvalD.dRat, Row_Ref, Col_Ref, stRow);
			PrintCellDataMark(pExcel, _T("[S]Result_T_Type1"), Chk_OK_Sign(0, 1.0, ResultD[0][1].EvalD.dRat), Row_Ref, Col_Ref, stRow);

			if (ResultD[0][1].EvalD.dRat < 1.0) bOK = FALSE;
		}

		if (!bOK)
		{
			PrintCellDataMark(pExcel, _T("[S]Col_Result_Type1"), _LS(IDS_DGN_Msg_NG), Row_Ref, Col_Ref, stRow);
		}
		else
		{
			PrintCellData(pExcel, _T("[S]Col_Result_Type1"), _LS(IDS_DGN_Msg_OK), Row_Ref, Col_Ref, stRow);
		}

		stRow += rowCnt;
	}
	else if (enBehavior[0] == RAHMEN && enBehavior[1] == CANTILEVER)
	{

		RptBlock = _T("[S]Result_Summary_Col_Type2");
		rowCnt = CopyRangeSubTitle(pExcel, RptBlock, stRow);
		GetCellIDByName(pExcel, RptBlock, Row_Ref, Col_Ref);
		PrintCellData(pExcel, _T("[S]Name_Col_Type2"), strColm, Row_Ref, Col_Ref, stRow);

		BOOL bOK = TRUE;
		if (ResultD[0][0].EvalD.bCheck)
		{
			PrintCellData(pExcel, _T("[S]FailMode_L_B_Type2"), strFailureBotLongi, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("[S]FPC_L_B_Type2"), ResultD[0][0].EvalD.ModeD.dFpc, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("[S]FPD_L_B_Type2"), ResultD[0][0].EvalD.EvalD.dFpd, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("[S]FR_L_B_Type2"), ResultD[0][0].EvalD.dRat, Row_Ref, Col_Ref, stRow);
			PrintCellDataMark(pExcel, _T("[S]Result_L_B_Type2"), Chk_OK_Sign(0, 1.0, ResultD[0][0].EvalD.dRat), Row_Ref, Col_Ref, stRow);

			if (ResultD[0][0].EvalD.dRat < 1.0) bOK = FALSE;
		}

		if (ResultD[1][0].EvalD.bCheck)
		{
			PrintCellData(pExcel, _T("[S]FailMode_L_T_Type2"), strFailureTopLongi, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("[S]FPC_L_T_Type2"), ResultD[1][0].EvalD.ModeD.dFpc, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("[S]FPD_L_T_Type2"), ResultD[1][0].EvalD.EvalD.dFpd, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("[S]FR_L_T_Type2"), ResultD[1][0].EvalD.dRat, Row_Ref, Col_Ref, stRow);
			PrintCellDataMark(pExcel, _T("[S]Result_L_T_Type2"), Chk_OK_Sign(0, 1.0, ResultD[1][0].EvalD.dRat), Row_Ref, Col_Ref, stRow);

			if (ResultD[1][0].EvalD.dRat < 1.0) bOK = FALSE;
		}

		if (ResultD[0][1].EvalD.bCheck)
		{
			PrintCellData(pExcel, _T("[S]FailMode_T_Type2"), strFailureBotTrans, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("[S]FPC_T_Type2"), ResultD[0][1].EvalD.ModeD.dFpc, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("[S]FPD_T_Type2"), ResultD[0][1].EvalD.EvalD.dFpd, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("[S]FR_T_Type2"), ResultD[0][1].EvalD.dRat, Row_Ref, Col_Ref, stRow);
			PrintCellDataMark(pExcel, _T("[S]Result_T_Type2"), Chk_OK_Sign(0, 1.0, ResultD[0][1].EvalD.dRat), Row_Ref, Col_Ref, stRow);

			if (ResultD[0][1].EvalD.dRat < 1.0) bOK = FALSE;
		}

		if (!bOK)
		{
			PrintCellDataMark(pExcel, _T("[S]Col_Result_Type2"), _LS(IDS_DGN_Msg_NG), Row_Ref, Col_Ref, stRow);
		}
		else
		{
			PrintCellData(pExcel, _T("[S]Col_Result_Type2"), _LS(IDS_DGN_Msg_OK), Row_Ref, Col_Ref, stRow);
		}

		stRow += rowCnt;
	}
	else if (enBehavior[0] == CANTILEVER && enBehavior[1] == RAHMEN)
	{
		RptBlock = _T("[S]Result_Summary_Col_Type3");
		rowCnt = CopyRangeSubTitle(pExcel, RptBlock, stRow);
		GetCellIDByName(pExcel, RptBlock, Row_Ref, Col_Ref);
		PrintCellData(pExcel, _T("[S]Name_Col_Type3"), strColm, Row_Ref, Col_Ref, stRow);

		BOOL bOK = TRUE;
		if (ResultD[0][0].EvalD.bCheck)
		{
			PrintCellData(pExcel, _T("[S]FailMode_L_Type3"), strFailureBotLongi, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("[S]FPC_L_Type3"), ResultD[0][0].EvalD.ModeD.dFpc, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("[S]FPD_L_Type3"), ResultD[0][0].EvalD.EvalD.dFpd, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("[S]FR_L_Type3"), ResultD[0][0].EvalD.dRat, Row_Ref, Col_Ref, stRow);
			PrintCellDataMark(pExcel, _T("[S]Result_L_Type3"), Chk_OK_Sign(0, 1.0, ResultD[0][0].EvalD.dRat), Row_Ref, Col_Ref, stRow);

			if (ResultD[0][0].EvalD.dRat < 1.0) bOK = FALSE;
		}

		if (ResultD[0][1].EvalD.bCheck)
		{
			PrintCellData(pExcel, _T("[S]FailMode_T_B_Type3"), strFailureBotTrans, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("[S]FPC_T_B_Type3"), ResultD[0][1].EvalD.ModeD.dFpc, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("[S]FPD_T_B_Type3"), ResultD[0][1].EvalD.EvalD.dFpd, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("[S]FR_T_B_Type3"), ResultD[0][1].EvalD.dRat, Row_Ref, Col_Ref, stRow);
			PrintCellDataMark(pExcel, _T("[S]Result_T_B_Type3"), Chk_OK_Sign(0, 1.0, ResultD[0][1].EvalD.dRat), Row_Ref, Col_Ref, stRow);

			if (ResultD[0][1].EvalD.dRat < 1.0) bOK = FALSE;
		}

		if (ResultD[1][1].EvalD.bCheck)
		{
			PrintCellData(pExcel, _T("[S]FailMode_T_T_Type3"), strFailureTopTrans, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("[S]FPC_T_T_Type3"), ResultD[1][1].EvalD.ModeD.dFpc, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("[S]FPD_T_T_Type3"), ResultD[1][1].EvalD.EvalD.dFpd, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("[S]FR_T_T_Type3"), ResultD[1][1].EvalD.dRat, Row_Ref, Col_Ref, stRow);
			PrintCellDataMark(pExcel, _T("[S]Result_T_T_Type3"), Chk_OK_Sign(0, 1.0, ResultD[1][1].EvalD.dRat), Row_Ref, Col_Ref, stRow);

			if (ResultD[1][1].EvalD.dRat < 1.0) bOK = FALSE;
		}

		if (!bOK)
		{
			PrintCellDataMark(pExcel, _T("[S]Col_Result_Type3"), _LS(IDS_DGN_Msg_NG), Row_Ref, Col_Ref, stRow);
		}
		else
		{
			PrintCellData(pExcel, _T("[S]Col_Result_Type3"), _LS(IDS_DGN_Msg_OK), Row_Ref, Col_Ref, stRow);
		}

		stRow += rowCnt;
	}
	else if (enBehavior[0] == RAHMEN && enBehavior[1] == RAHMEN)
	{
		RptBlock = _T("[S]Result_Summary_Col_Type4");
		rowCnt = CopyRangeSubTitle(pExcel, RptBlock, stRow);
		GetCellIDByName(pExcel, RptBlock, Row_Ref, Col_Ref);
		PrintCellData(pExcel, _T("[S]Name_Col_Type4"), strColm, Row_Ref, Col_Ref, stRow);

		BOOL bOK = TRUE;
		if (ResultD[0][0].EvalD.bCheck)
		{

			PrintCellData(pExcel, _T("[S]FailMode_L_B_Type4"), strFailureBotLongi, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("[S]FPC_L_B_Type4"), ResultD[0][0].EvalD.ModeD.dFpc, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("[S]FPD_L_B_Type4"), ResultD[0][0].EvalD.EvalD.dFpd, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("[S]FR_L_B_Type4"), ResultD[0][0].EvalD.dRat, Row_Ref, Col_Ref, stRow);
			PrintCellDataMark(pExcel, _T("[S]Result_L_B_Type4"), Chk_OK_Sign(0, 1.0, ResultD[0][0].EvalD.dRat), Row_Ref, Col_Ref, stRow);

			if (ResultD[0][0].EvalD.dRat < 1.0) bOK = FALSE;
		}

		if (ResultD[1][0].EvalD.bCheck)
		{
			PrintCellData(pExcel, _T("[S]FailMode_L_T_Type4"), strFailureTopLongi, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("[S]FPC_L_T_Type4"), ResultD[1][0].EvalD.ModeD.dFpc, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("[S]FPD_L_T_Type4"), ResultD[1][0].EvalD.EvalD.dFpd, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("[S]FR_L_T_Type4"), ResultD[1][0].EvalD.dRat, Row_Ref, Col_Ref, stRow);
			PrintCellDataMark(pExcel, _T("[S]Result_L_T_Type4"), Chk_OK_Sign(0, 1.0, ResultD[1][0].EvalD.dRat), Row_Ref, Col_Ref, stRow);

			if (ResultD[1][0].EvalD.dRat < 1.0) bOK = FALSE;
		}

		if (ResultD[0][1].EvalD.bCheck)
		{
			PrintCellData(pExcel, _T("[S]FailMode_T_B_Type4"), strFailureBotTrans, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("[S]FPC_T_B_Type4"), ResultD[0][1].EvalD.ModeD.dFpc, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("[S]FPD_T_B_Type4"), ResultD[0][1].EvalD.EvalD.dFpd, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("[S]FR_T_B_Type4"), ResultD[0][1].EvalD.dRat, Row_Ref, Col_Ref, stRow);
			PrintCellDataMark(pExcel, _T("[S]Result_T_B_Type4"), Chk_OK_Sign(0, 1.0, ResultD[0][1].EvalD.dRat), Row_Ref, Col_Ref, stRow);

			if (ResultD[0][1].EvalD.dRat < 1.0) bOK = FALSE;
		}

		if (ResultD[1][1].EvalD.bCheck)
		{
			PrintCellData(pExcel, _T("[S]FailMode_T_T_Type4"), strFailureTopTrans, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("[S]FPC_T_T_Type4"), ResultD[1][1].EvalD.ModeD.dFpc, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("[S]FPD_T_T_Type4"), ResultD[1][1].EvalD.EvalD.dFpd, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("[S]FR_T_T_Type4"), ResultD[1][1].EvalD.dRat, Row_Ref, Col_Ref, stRow);
			PrintCellDataMark(pExcel, _T("[S]Result_T_T_Type4"), Chk_OK_Sign(0, 1.0, ResultD[1][1].EvalD.dRat), Row_Ref, Col_Ref, stRow);

			if (ResultD[1][1].EvalD.dRat < 1.0) bOK = FALSE;
		}

		if (!bOK)
		{
			PrintCellDataMark(pExcel, _T("[S]Col_Result_Type4"), _LS(IDS_DGN_Msg_NG), Row_Ref, Col_Ref, stRow);
		}
		else
		{
			PrintCellData(pExcel, _T("[S]Col_Result_Type4"), _LS(IDS_DGN_Msg_OK), Row_Ref, Col_Ref, stRow);
		}

		stRow += rowCnt;
	}
	else ASSERT(0);

	return TRUE;
}

BOOL CBrdgSeisEvalKISTEC2015_Report::GenerateSummaryBearingTable(IExcel* pExcel, int& stRow, const T_EVGP_K& EvgpK, const T_BREV_K& BrevK)
{
	CString RptBlock;
	int rowCnt = 0;
	short Row_Ref = 0, Col_Ref = 0;

	RptBlock = _T("[S]Result_Summary_Bearing");
	rowCnt = CopyRangeSubTitle(pExcel, RptBlock, stRow);

	if (BrevK == 0)
	{
		stRow += rowCnt;
		return TRUE;
	}
	CBrdgSeisEvalResult *pResult = m_pDoc->m_pPostCtrl->GetBrdgSeisEvalResult(); if (!pResult) { ASSERT(0); }
	T_BRDGEVAL_BEARING ResD;
	if (!pResult->ReadSeisEvalResultBearing(BrevK, 0, TRUE, ResD))
	{
		stRow += rowCnt;
		return TRUE;
	}

	GetCellIDByName(pExcel, RptBlock, Row_Ref, Col_Ref);

	T_BREV_D BrevD; BrevD.Initialize();
	if (!m_pDoc->m_pAttrCtrl2->GetBrev(BrevK, BrevD)) ASSERT(0);
	PrintCellData(pExcel, _T("[S]Name_Summary_Bearing"), BrevD.strName, Row_Ref, Col_Ref, stRow);

	// 받침본체
	BOOL bOK = TRUE;
	PrintCellData(pExcel, _T("[S]FBC_L"), ResD.BodyD[0].HorD.dFBC, Row_Ref, Col_Ref, stRow);
	PrintCellData(pExcel, _T("[S]FBD_L"), ResD.BodyD[0].HorD.dFBD, Row_Ref, Col_Ref, stRow);
	if (IsPrintMark(ResD.BodyD[0].HorD.dFBC, ResD.BodyD[0].HorD.dFBD))
	{
		PrintCellData(pExcel, _T("[S]FBR_L"), ResD.BodyD[0].HorD.dRat, Row_Ref, Col_Ref, stRow);
		PrintCellDataMark(pExcel, _T("[S]Res_FB_L"), Chk_OK_Sign(0, 1.0, ResD.BodyD[0].HorD.dRat), Row_Ref, Col_Ref, stRow);
		if (ResD.BodyD[0].HorD.dRat < 1.0) bOK = FALSE;
	}

	if (ResD.BodyD[0].DispD.bCheck)
	{
		PrintCellData(pExcel, _T("[S]FBC_SD_L"), ResD.BodyD[0].DispD.dFBC*m_dFactorLengthMM, Row_Ref, Col_Ref, stRow);
		PrintCellData(pExcel, _T("[S]FBD_SD_L"), ResD.BodyD[0].DispD.dFBD*m_dFactorLengthMM, Row_Ref, Col_Ref, stRow);
		if (IsPrintMark(ResD.BodyD[0].DispD.dFBC, ResD.BodyD[0].DispD.dFBD))
		{
			PrintCellData(pExcel, _T("[S]FBR_SD_L"), ResD.BodyD[0].DispD.dRat, Row_Ref, Col_Ref, stRow);
			PrintCellDataMark(pExcel, _T("[S]Res_FB_SD_L"), Chk_OK_Sign(0, 1.0, ResD.BodyD[0].DispD.dRat), Row_Ref, Col_Ref, stRow);
			if (ResD.BodyD[0].DispD.dRat < 1.0) bOK = FALSE;
		}
	}

	PrintCellData(pExcel, _T("[S]FBC_T"), ResD.BodyD[1].HorD.dFBC, Row_Ref, Col_Ref, stRow);
	PrintCellData(pExcel, _T("[S]FBD_T"), ResD.BodyD[1].HorD.dFBD, Row_Ref, Col_Ref, stRow);
	if (IsPrintMark(ResD.BodyD[1].HorD.dFBC, ResD.BodyD[1].HorD.dFBD))
	{
		PrintCellData(pExcel, _T("[S]FBR_T"), ResD.BodyD[1].HorD.dRat, Row_Ref, Col_Ref, stRow);
		PrintCellDataMark(pExcel, _T("[S]Res_FB_T"), Chk_OK_Sign(0, 1.0, ResD.BodyD[1].HorD.dRat), Row_Ref, Col_Ref, stRow);
		if (ResD.BodyD[1].HorD.dRat < 1.0) bOK = FALSE;
	}

	if (ResD.BodyD[1].DispD.bCheck)
	{
		PrintCellData(pExcel, _T("[S]FBC_SD_T"), ResD.BodyD[1].DispD.dFBC*m_dFactorLengthMM, Row_Ref, Col_Ref, stRow);
		PrintCellData(pExcel, _T("[S]FBD_SD_T"), ResD.BodyD[1].DispD.dFBD*m_dFactorLengthMM, Row_Ref, Col_Ref, stRow);
		if (IsPrintMark(ResD.BodyD[1].DispD.dFBC, ResD.BodyD[1].DispD.dFBD))
		{
			PrintCellData(pExcel, _T("[S]FBR_SD_T"), ResD.BodyD[1].DispD.dRat, Row_Ref, Col_Ref, stRow);
			PrintCellDataMark(pExcel, _T("[S]Res_FB_SD_T"), Chk_OK_Sign(0, 1.0, ResD.BodyD[1].DispD.dRat), Row_Ref, Col_Ref, stRow);
			if (ResD.BodyD[1].DispD.dRat < 1.0) bOK = FALSE;
		}
	}

	if (!bOK)
	{
		PrintCellDataMark(pExcel, _T("[S]Res_FB"), _LS(IDS_DGN_Msg_NG), Row_Ref, Col_Ref, stRow);
	}
	else
	{
		PrintCellData(pExcel, _T("[S]Res_FB"), _LS(IDS_DGN_Msg_OK), Row_Ref, Col_Ref, stRow);
	}

	// 앵커볼트
	bOK = TRUE;
	bool bAddPryoutCmt = false;
	PrintCellData(pExcel, _T("[S]FASC_L"), ResD.AnchorD[0].FASD.dFASC, Row_Ref, Col_Ref, stRow);
	PrintCellData(pExcel, _T("[S]FASD_L"), ResD.AnchorD[0].FASD.dFASD, Row_Ref, Col_Ref, stRow);
	if (IsPrintMark(ResD.AnchorD[0].FASD.dFASC, ResD.AnchorD[0].FASD.dFASD))
	{
		PrintCellData(pExcel, _T("[S]FASR_L"), ResD.AnchorD[0].FASD.dRat, Row_Ref, Col_Ref, stRow);
		PrintCellDataMark(pExcel, _T("[S]Res_FAS_L"), Chk_OK_Sign(0, 1.0, ResD.AnchorD[0].FASD.dRat), Row_Ref, Col_Ref, stRow);
		if (ResD.AnchorD[0].FASD.dRat < 1.0) bOK = FALSE;
	}
	PrintCellData(pExcel, _T("[S]FACC_L"), ResD.AnchorD[0].FACD.dFACC, Row_Ref, Col_Ref, stRow);
	PrintCellData(pExcel, _T("[S]FACD_L"), ResD.AnchorD[0].FACD.dFACD, Row_Ref, Col_Ref, stRow);
	if (IsPrintMark(ResD.AnchorD[0].FACD.dFACC, ResD.AnchorD[0].FACD.dFACD))
	{
		PrintCellData(pExcel, _T("[S]FACR_L"), ResD.AnchorD[0].FACD.dRat, Row_Ref, Col_Ref, stRow);
		PrintCellDataMark(pExcel, _T("[S]Res_FAC_L"), Chk_OK_Sign(0, 1.0, ResD.AnchorD[0].FACD.dRat), Row_Ref, Col_Ref, stRow);
		if (ResD.AnchorD[0].FACD.dRat < 1.0) bOK = FALSE;
	}
	PrintCellData(pExcel, _T("[S]FAPC_L"), ResD.AnchorD[0].FAPD.dFAPC, Row_Ref, Col_Ref, stRow);
	PrintCellData(pExcel, _T("[S]FAPD_L"), ResD.AnchorD[0].FAPD.dFAPD, Row_Ref, Col_Ref, stRow);
	if (IsPrintMark(ResD.AnchorD[0].FAPD.dFAPC, ResD.AnchorD[0].FAPD.dFAPD))
	{
		PrintCellData(pExcel, _T("[S]FAPR_L"), ResD.AnchorD[0].FAPD.dRat, Row_Ref, Col_Ref, stRow);
		if(ResD.ParamD.iEvalCode == KALIS_2023)
		{
			CString strSign = Chk_OK_Sign(0, 1.0, ResD.AnchorD[0].FAPD.dRat);
			if(ResD.AnchorD[0].FAPD.enIgnore == BEARING_PRYOUT_IGNORE_CONSIDER) strSign += _T("*");
			PrintCellDataMark(pExcel, _T("[S]Res_FAP_L"), strSign, Row_Ref, Col_Ref, stRow);
			bAddPryoutCmt = true;
		}
		if (ResD.AnchorD[0].FAPD.dRat < 1.0) bOK = FALSE;
	}
	PrintCellData(pExcel, _T("[S]FASC_T"), ResD.AnchorD[1].FASD.dFASC, Row_Ref, Col_Ref, stRow);
	PrintCellData(pExcel, _T("[S]FASD_T"), ResD.AnchorD[1].FASD.dFASD, Row_Ref, Col_Ref, stRow);
	if (IsPrintMark(ResD.AnchorD[1].FASD.dFASC, ResD.AnchorD[1].FASD.dFASD))
	{
		PrintCellData(pExcel, _T("[S]FASR_T"), ResD.AnchorD[1].FASD.dRat, Row_Ref, Col_Ref, stRow);
		PrintCellDataMark(pExcel, _T("[S]Res_FAS_T"), Chk_OK_Sign(0, 1.0, ResD.AnchorD[1].FASD.dRat), Row_Ref, Col_Ref, stRow);
		if (ResD.AnchorD[1].FASD.dRat < 1.0) bOK = FALSE;
	}
	PrintCellData(pExcel, _T("[S]FACC_T"), ResD.AnchorD[1].FACD.dFACC, Row_Ref, Col_Ref, stRow);
	PrintCellData(pExcel, _T("[S]FACD_T"), ResD.AnchorD[1].FACD.dFACD, Row_Ref, Col_Ref, stRow);
	if (IsPrintMark(ResD.AnchorD[1].FACD.dFACC, ResD.AnchorD[1].FACD.dFACD))
	{
		PrintCellData(pExcel, _T("[S]FACR_T"), ResD.AnchorD[1].FACD.dRat, Row_Ref, Col_Ref, stRow);
		PrintCellDataMark(pExcel, _T("[S]Res_FAC_T"), Chk_OK_Sign(0, 1.0, ResD.AnchorD[1].FACD.dRat), Row_Ref, Col_Ref, stRow);
		if (ResD.AnchorD[1].FACD.dRat < 1.0) bOK = FALSE;
	}
	PrintCellData(pExcel, _T("[S]FAPC_T"), ResD.AnchorD[1].FAPD.dFAPC, Row_Ref, Col_Ref, stRow);
	PrintCellData(pExcel, _T("[S]FAPD_T"), ResD.AnchorD[1].FAPD.dFAPD, Row_Ref, Col_Ref, stRow);
	if (IsPrintMark(ResD.AnchorD[1].FAPD.dFAPC, ResD.AnchorD[1].FAPD.dFAPD))
	{
		PrintCellData(pExcel, _T("[S]FAPR_T"), ResD.AnchorD[1].FAPD.dRat, Row_Ref, Col_Ref, stRow);
		if(ResD.ParamD.iEvalCode == KALIS_2023)
		{
			CString strSign = Chk_OK_Sign(0, 1.0, ResD.AnchorD[1].FAPD.dRat);
			if(ResD.AnchorD[1].FAPD.enIgnore == BEARING_PRYOUT_IGNORE_CONSIDER) strSign += _T("*");
			PrintCellDataMark(pExcel, _T("[S]Res_FAP_T"), strSign, Row_Ref, Col_Ref, stRow);
			bAddPryoutCmt = true;
		}
		if (ResD.AnchorD[1].FAPD.dRat < 1.0) bOK = FALSE;
	}

	if (!bOK)
	{
		PrintCellDataMark(pExcel, _T("[S]Res_FA"), _LS(IDS_DGN_Msg_NG), Row_Ref, Col_Ref, stRow);
	}
	else
	{
		PrintCellData(pExcel, _T("[S]Res_FA"), _LS(IDS_DGN_Msg_OK), Row_Ref, Col_Ref, stRow);
	}
	stRow += rowCnt;

	// 인장을 받는 앵커
	if (ResD.AnchorD[0].bCheckN || ResD.AnchorD[1].bCheckN)
	{
		RptBlock = _T("[S]Result_Summary_Bearing_Tension");
		rowCnt = CopyRangeSubTitle(pExcel, RptBlock, stRow);
		GetCellIDByName(pExcel, RptBlock, Row_Ref, Col_Ref);
		bOK = TRUE;
		if (ResD.AnchorD[0].bCheckN)
		{
			PrintCellData(pExcel, _T("[S]FNSC_L"), ResD.AnchorD[0].FNSD.dFNSC, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("[S]FNSD_L"), ResD.AnchorD[0].FNSD.dFNSD, Row_Ref, Col_Ref, stRow);
			if (IsPrintMark(ResD.AnchorD[0].FNSD.dFNSC, ResD.AnchorD[0].FNSD.dFNSD))
			{
				PrintCellData(pExcel, _T("[S]FNSR_L"), ResD.AnchorD[0].FNSD.dRat, Row_Ref, Col_Ref, stRow);
				PrintCellDataMark(pExcel, _T("[S]Res_FNS_L"), Chk_OK_Sign(0, 1.0, ResD.AnchorD[0].FNSD.dRat), Row_Ref, Col_Ref, stRow);
				if (ResD.AnchorD[0].FNSD.dRat < 1.0) bOK = FALSE;
			}
			PrintCellData(pExcel, _T("[S]FNCC_L"), ResD.AnchorD[0].FNCD.dFNCC, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("[S]FNCD_L"), ResD.AnchorD[0].FNCD.dFNCD, Row_Ref, Col_Ref, stRow);
			if (IsPrintMark(ResD.AnchorD[0].FNCD.dFNCC, ResD.AnchorD[0].FNCD.dFNCD))
			{
				PrintCellData(pExcel, _T("[S]FNCR_L"), ResD.AnchorD[0].FNCD.dRat, Row_Ref, Col_Ref, stRow);
				PrintCellDataMark(pExcel, _T("[S]Res_FNC_L"), Chk_OK_Sign(0, 1.0, ResD.AnchorD[0].FNCD.dRat), Row_Ref, Col_Ref, stRow);
				if (ResD.AnchorD[0].FNCD.dRat < 1.0) bOK = FALSE;
			}
			PrintCellData(pExcel, _T("[S]FNPC_L"), ResD.AnchorD[0].FNPD.dFNPC, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("[S]FNPD_L"), ResD.AnchorD[0].FNPD.dFNPD, Row_Ref, Col_Ref, stRow);
			if (IsPrintMark(ResD.AnchorD[0].FNPD.dFNPC, ResD.AnchorD[0].FNPD.dFNPD))
			{
				PrintCellData(pExcel, _T("[S]FNPR_L"), ResD.AnchorD[0].FNPD.dRat, Row_Ref, Col_Ref, stRow);
				PrintCellDataMark(pExcel, _T("[S]Res_FNP_L"), Chk_OK_Sign(0, 1.0, ResD.AnchorD[0].FNPD.dRat), Row_Ref, Col_Ref, stRow);
				if (ResD.AnchorD[0].FNPD.dRat < 1.0) bOK = FALSE;
			}
			if (ResD.AnchorD[0].FNRD.bCheck)
			{
				PrintCellData(pExcel, _T("[S]FNRC_L"), ResD.AnchorD[0].FNRD.dFNRC, Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("[S]FNRD_L"), ResD.AnchorD[0].FNRD.dFNRD, Row_Ref, Col_Ref, stRow);
				if (IsPrintMark(ResD.AnchorD[0].FNRD.dFNRC, ResD.AnchorD[0].FNRD.dFNRD))
				{
					PrintCellData(pExcel, _T("[S]FNRR_L"), ResD.AnchorD[0].FNRD.dRat, Row_Ref, Col_Ref, stRow);
					PrintCellDataMark(pExcel, _T("[S]Res_FNR_L"), Chk_OK_Sign(0, 1.0, ResD.AnchorD[0].FNRD.dRat), Row_Ref, Col_Ref, stRow);
					if (ResD.AnchorD[0].FNRD.dRat < 1.0) bOK = FALSE;
				}
			}

			double dRat = ResD.AnchorD[0].FNVD.dRat / ResD.AnchorD[0].FNVD.dRatLim;
			CString strGovTyp = _T("");
			switch (ResD.AnchorD[0].FNVD.nType)
			{
			case 1:
				strGovTyp = _T("교축; 인장지배");
				break;
			case 2:
				strGovTyp = _T("교축; 전단지배");
				break;
			case 3:
				strGovTyp = _T("교축; 상관작용");
				break;
			default:
				break;
			}
			if (strGovTyp != _T(""))
			{
				PrintCellData(pExcel, _T("[S]NV_Type_L"), strGovTyp, Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("[S]NVC_L"), ResD.AnchorD[0].FNVD.dRat, Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("[S]NVD_L"), ResD.AnchorD[0].FNVD.dRatLim, Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("[S]NV_Rat_L"), dRat, Row_Ref, Col_Ref, stRow);
				PrintCellDataMark(pExcel, _T("[S]Res_NV_L"), Chk_OK_Sign(0, 1.0, dRat), Row_Ref, Col_Ref, stRow);
			}
		}
		if (ResD.AnchorD[1].bCheckN)
		{
			PrintCellData(pExcel, _T("[S]FNSC_T"), ResD.AnchorD[1].FNSD.dFNSC, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("[S]FNSD_T"), ResD.AnchorD[1].FNSD.dFNSD, Row_Ref, Col_Ref, stRow);
			if (IsPrintMark(ResD.AnchorD[1].FNSD.dFNSC, ResD.AnchorD[1].FNSD.dFNSD))
			{
				PrintCellData(pExcel, _T("[S]FNSR_T"), ResD.AnchorD[1].FNSD.dRat, Row_Ref, Col_Ref, stRow);
				PrintCellDataMark(pExcel, _T("[S]Res_FNS_T"), Chk_OK_Sign(0, 1.0, ResD.AnchorD[1].FNSD.dRat), Row_Ref, Col_Ref, stRow);
				if (ResD.AnchorD[1].FNSD.dRat < 1.0) bOK = FALSE;
			}
			PrintCellData(pExcel, _T("[S]FNCC_T"), ResD.AnchorD[1].FNCD.dFNCC, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("[S]FNCD_T"), ResD.AnchorD[1].FNCD.dFNCD, Row_Ref, Col_Ref, stRow);
			if (IsPrintMark(ResD.AnchorD[1].FNCD.dFNCC, ResD.AnchorD[1].FNCD.dFNCD))
			{
				PrintCellData(pExcel, _T("[S]FNCR_T"), ResD.AnchorD[1].FNCD.dRat, Row_Ref, Col_Ref, stRow);
				PrintCellDataMark(pExcel, _T("[S]Res_FNC_T"), Chk_OK_Sign(0, 1.0, ResD.AnchorD[1].FNCD.dRat), Row_Ref, Col_Ref, stRow);
				if (ResD.AnchorD[1].FNCD.dRat < 1.0) bOK = FALSE;
			}
			PrintCellData(pExcel, _T("[S]FNPC_T"), ResD.AnchorD[1].FNPD.dFNPC, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("[S]FNPD_T"), ResD.AnchorD[1].FNPD.dFNPD, Row_Ref, Col_Ref, stRow);
			if (IsPrintMark(ResD.AnchorD[1].FNPD.dFNPC, ResD.AnchorD[1].FNPD.dFNPD))
			{
				PrintCellData(pExcel, _T("[S]FNPR_T"), ResD.AnchorD[1].FNPD.dRat, Row_Ref, Col_Ref, stRow);
				PrintCellDataMark(pExcel, _T("[S]Res_FNP_T"), Chk_OK_Sign(0, 1.0, ResD.AnchorD[1].FNPD.dRat), Row_Ref, Col_Ref, stRow);
				if (ResD.AnchorD[1].FNPD.dRat < 1.0) bOK = FALSE;
			}
			if (ResD.AnchorD[0].FNRD.bCheck)
			{
				PrintCellData(pExcel, _T("[S]FNRC_T"), ResD.AnchorD[1].FNRD.dFNRC, Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("[S]FNRD_T"), ResD.AnchorD[1].FNRD.dFNRD, Row_Ref, Col_Ref, stRow);
				if (IsPrintMark(ResD.AnchorD[1].FNRD.dFNRC, ResD.AnchorD[1].FNRD.dFNRD))
				{
					PrintCellData(pExcel, _T("[S]FNRR_T"), ResD.AnchorD[1].FNRD.dRat, Row_Ref, Col_Ref, stRow);
					PrintCellDataMark(pExcel, _T("[S]Res_FNR_T"), Chk_OK_Sign(0, 1.0, ResD.AnchorD[1].FNRD.dRat), Row_Ref, Col_Ref, stRow);
					if (ResD.AnchorD[1].FNRD.dRat < 1.0) bOK = FALSE;
				}
			}

			double dRat = ResD.AnchorD[1].FNVD.dRat / ResD.AnchorD[1].FNVD.dRatLim;
			CString strGovTyp = _T("");
			switch (ResD.AnchorD[1].FNVD.nType)
			{
			case 1:
				strGovTyp = _T("교축직각; 인장지배");
				break;
			case 2:
				strGovTyp = _T("교축직각; 전단지배");
				break;
			case 3:
				strGovTyp = _T("교축직각; 상관작용");
				break;
			default:
				break;
			}
			if (strGovTyp != _T(""))
			{
				PrintCellData(pExcel, _T("[S]NV_Type_T"), strGovTyp, Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("[S]NVC_T"), ResD.AnchorD[1].FNVD.dRat, Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("[S]NVD_T"), ResD.AnchorD[1].FNVD.dRatLim, Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("[S]NV_Rat_T"), dRat, Row_Ref, Col_Ref, stRow);
				PrintCellDataMark(pExcel, _T("[S]Res_NV_T"), Chk_OK_Sign(0, 1.0, dRat), Row_Ref, Col_Ref, stRow);
			}
		}
		if (!bOK)
		{
			PrintCellDataMark(pExcel, _T("[S]Res_FN"), _LS(IDS_DGN_Msg_NG), Row_Ref, Col_Ref, stRow);
		}
		else
		{
			PrintCellData(pExcel, _T("[S]Res_FN"), _LS(IDS_DGN_Msg_OK), Row_Ref, Col_Ref, stRow);
		}

		stRow += rowCnt;
	}

	// 받침지지길이
	RptBlock = _T("[S]Result_Summary_Bearing_SuppDisp");
	rowCnt = CopyRangeSubTitle(pExcel, RptBlock, stRow);
	GetCellIDByName(pExcel, RptBlock, Row_Ref, Col_Ref);
	PrintCellData(pExcel, _T("[S]Name_Summary_Bearing_SuppDisp"), BrevD.strName, Row_Ref, Col_Ref, stRow);
	if (ResD.SuppLengD.bCheck)
	{
		PrintCellData(pExcel, _T("[S]NC"), m_dFactorLengthMM*ResD.SuppLengD.dNC, Row_Ref, Col_Ref, stRow);
		PrintCellData(pExcel, _T("[S]ND"), m_dFactorLengthMM*ResD.SuppLengD.dND, Row_Ref, Col_Ref, stRow);
		if (IsPrintMark(ResD.SuppLengD.dNC, ResD.SuppLengD.dND))
		{
			PrintCellData(pExcel, _T("[S]NR"), ResD.SuppLengD.dRat, Row_Ref, Col_Ref, stRow);
			PrintCellDataMark(pExcel, _T("[S]Res_N"), Chk_OK_Sign(0, 1.0, ResD.SuppLengD.dRat), Row_Ref, Col_Ref, stRow);
		}
	}
	stRow += rowCnt;

	if(bAddPryoutCmt)
	{
		RptBlock = _T("[S]Pryout_Chk_Comment");
		rowCnt = CopyRangeSubTitle(pExcel, RptBlock, stRow);
		stRow += rowCnt;
	}

	return TRUE;

}

BOOL CBrdgSeisEvalKISTEC2015_Report::GenerateMixedModeRSInfo(IExcel* pExcel, int& stRow, const double& dAxial, const UINT& EvgpK, const UINT& iColm, int iNumEVGP, T_BRDGEVAL_RESULT ResultD[2][2], const CString& strEvgp)
{
	CString RptBlock;
	int rowCnt = 0;

	short Row_Ref = 0, Col_Ref = 0;

	RptBlock = _T("[MM]Mixed_Mode_RS_Head");
	rowCnt = CopyRangeSubTitle(pExcel, RptBlock, stRow);
	//GetCellIDByName(pExcel, RptBlock, Row_Ref, Col_Ref);
	stRow += rowCnt;

	T_BRDGEVAL_FORCE ForceD[2];

	double dDispLong = 0.0;
	double dDispTrans = 0.0;

	CArray<T_SPLC_K, T_SPLC_K> aXKey;
	CArray<T_SPLC_K, T_SPLC_K> aYKey;
	CArray<T_SPLC_K, T_SPLC_K> aZKey;

	int iRSNumTot = Get_RSListByDir(aXKey, aYKey, aZKey);

	int iRSNumX = aXKey.GetSize();
	int iRSNumY = aYKey.GetSize();
	int iRSNumZ = aZKey.GetSize();

	int nBridgeDir = 0;
	for (int i = 0; i < iRSNumX; i++)
	{
		T_SPLC_K keyX = aXKey[i];
		T_SPLC_D rDataX;
		m_pDoc->m_pAttrCtrl->GetSplc(keyX, rDataX);

		if (i == 0)
		{
			RptBlock = _T("[MM]Mixed_Mode_RS_X");
			rowCnt = CopyRangeSubTitle(pExcel, RptBlock, stRow);
			stRow += rowCnt;
		}

		if (i % 2 == 0)
		{
			RptBlock = _T("[MM]Mixed_Mode_X_RS_Table");
			rowCnt = CopyRangeSubTitle(pExcel, RptBlock, stRow);

			RptBlock = _T("[MM]Mixed_Mode_X_RS_Contents");
			rowCnt = CopyRangeOddTable(pExcel, RptBlock, stRow);
			GetCellIDByName(pExcel, RptBlock, Row_Ref, Col_Ref);
			int iOddNum = 7;

			PrintCellOddEvenData(pExcel, _T("Mixed_Mode_RS_Name"), iOddNum, rDataX.LoadCaseName, Row_Ref, Col_Ref, stRow);
			PrintCellOddEvenData(pExcel, _T("Mixed_Mode_RS_Load_Top"), iOddNum, dAxial, Row_Ref, Col_Ref, stRow);
			PrintCellOddEvenData(pExcel, _T("Mixed_Mode_RS_Load_Bot"), iOddNum, ResultD[0][0].ParamD.ForceD.dPcolbot, Row_Ref, Col_Ref, stRow);

			Calc_PierSeisRsDisp(EvgpK, iColm, keyX, dDispLong, dDispTrans);

			PrintCellOddEvenData(pExcel, _T("Mixed_Mode_RS_Disp_Longi"), iOddNum, 1000 * dDispLong, Row_Ref, Col_Ref, stRow);
			PrintCellOddEvenData(pExcel, _T("Mixed_Mode_RS_Disp_Trans"), iOddNum, 1000 * dDispTrans, Row_Ref, Col_Ref, stRow);

			Calc_PierSeisRsForce(EvgpK, iColm, keyX, ForceD);

			if (fabs(ForceD[1].dForce[2]) < cDGN_Zero)                                                  ForceD[1].dForce[2] = 0;
			else if (cDGN_Zero < ForceD[1].dForce[2] && ForceD[1].dForce[2] < cDGN_Zero * 10)      ForceD[1].dForce[2] = floor(ForceD[1].dForce[2] / cDGN_Zero + 0.5) * cDGN_Zero;
			else if (cDGN_Zero * 10 < ForceD[1].dForce[2] && ForceD[1].dForce[2] < cDGN_Zero * 100)     ForceD[1].dForce[2] = floor(ForceD[1].dForce[2] / cDGN_Zero / 10 + 0.5) * cDGN_Zero * 10;

			if (fabs(ForceD[1].dForce[1]) < cDGN_Zero)                                                  ForceD[1].dForce[1] = 0;
			else if (cDGN_Zero < ForceD[1].dForce[1] && ForceD[1].dForce[1] < cDGN_Zero * 10)      ForceD[1].dForce[1] = floor(ForceD[1].dForce[1] / cDGN_Zero + 0.5) * cDGN_Zero;
			else if (cDGN_Zero * 10 < ForceD[1].dForce[1] && ForceD[1].dForce[1] < cDGN_Zero * 100)     ForceD[1].dForce[1] = floor(ForceD[1].dForce[1] / cDGN_Zero / 10 + 0.5) * cDGN_Zero * 10;

			if (fabs(ForceD[1].dForce[4]) < cDGN_Zero)                                                  ForceD[1].dForce[4] = 0;
			else if (cDGN_Zero < ForceD[1].dForce[4] && ForceD[1].dForce[4] < cDGN_Zero * 10)      ForceD[1].dForce[4] = floor(ForceD[1].dForce[4] / cDGN_Zero + 0.5) * cDGN_Zero;
			else if (cDGN_Zero * 10 < ForceD[1].dForce[4] && ForceD[1].dForce[4] < cDGN_Zero * 100)     ForceD[1].dForce[4] = floor(ForceD[1].dForce[4] / cDGN_Zero / 10 + 0.5) * cDGN_Zero * 10;

			if (fabs(ForceD[1].dForce[5]) < cDGN_Zero)                                                  ForceD[1].dForce[5] = 0;
			else if (cDGN_Zero < ForceD[1].dForce[5] && ForceD[1].dForce[5] < cDGN_Zero * 10)      ForceD[1].dForce[5] = floor(ForceD[1].dForce[5] / cDGN_Zero + 0.5) * cDGN_Zero;
			else if (cDGN_Zero * 10 < ForceD[1].dForce[5] && ForceD[1].dForce[5] < cDGN_Zero * 100)     ForceD[1].dForce[5] = floor(ForceD[1].dForce[5] / cDGN_Zero / 10 + 0.5) * cDGN_Zero * 10;

			if (fabs(ForceD[0].dForce[2]) < cDGN_Zero)                                                  ForceD[0].dForce[2] = 0;
			else if (cDGN_Zero < ForceD[0].dForce[2] && ForceD[0].dForce[2] < cDGN_Zero * 10)      ForceD[0].dForce[2] = floor(ForceD[0].dForce[2] / cDGN_Zero + 0.5) * cDGN_Zero;
			else if (cDGN_Zero * 10 < ForceD[0].dForce[2] && ForceD[0].dForce[2] < cDGN_Zero * 100)     ForceD[0].dForce[2] = floor(ForceD[0].dForce[2] / cDGN_Zero / 10 + 0.5) * cDGN_Zero * 10;

			if (fabs(ForceD[0].dForce[1]) < cDGN_Zero)                                                  ForceD[0].dForce[1] = 0;
			else if (cDGN_Zero < ForceD[0].dForce[1] && ForceD[0].dForce[1] < cDGN_Zero * 10)      ForceD[0].dForce[1] = floor(ForceD[0].dForce[1] / cDGN_Zero + 0.5) * cDGN_Zero;
			else if (cDGN_Zero * 10 < ForceD[0].dForce[1] && ForceD[0].dForce[1] < cDGN_Zero * 100)     ForceD[0].dForce[1] = floor(ForceD[0].dForce[1] / cDGN_Zero / 10 + 0.5) * cDGN_Zero * 10;

			if (fabs(ForceD[0].dForce[4]) < cDGN_Zero)                                                  ForceD[0].dForce[4] = 0;
			else if (cDGN_Zero < ForceD[0].dForce[4] && ForceD[0].dForce[4] < cDGN_Zero * 10)      ForceD[0].dForce[4] = floor(ForceD[0].dForce[4] / cDGN_Zero + 0.5) * cDGN_Zero;
			else if (cDGN_Zero * 10 < ForceD[0].dForce[4] && ForceD[0].dForce[4] < cDGN_Zero * 100)     ForceD[0].dForce[4] = floor(ForceD[0].dForce[4] / cDGN_Zero / 10 + 0.5) * cDGN_Zero * 10;

			if (fabs(ForceD[0].dForce[5]) < cDGN_Zero)                                                  ForceD[0].dForce[5] = 0;
			else if (cDGN_Zero < ForceD[0].dForce[5] && ForceD[0].dForce[5] < cDGN_Zero * 10)      ForceD[0].dForce[5] = floor(ForceD[0].dForce[5] / cDGN_Zero + 0.5) * cDGN_Zero;
			else if (cDGN_Zero * 10 < ForceD[0].dForce[5] && ForceD[0].dForce[5] < cDGN_Zero * 100)     ForceD[0].dForce[5] = floor(ForceD[0].dForce[5] / cDGN_Zero / 10 + 0.5) * cDGN_Zero * 10;

			PrintCellOddEvenData(pExcel, _T("Mixed_Mode_RS_Shear_Longi_Top"), iOddNum, ForceD[1].dForce[2], Row_Ref, Col_Ref, stRow);
			PrintCellOddEvenData(pExcel, _T("Mixed_Mode_RS_Shear_Trans_Top"), iOddNum, ForceD[1].dForce[1], Row_Ref, Col_Ref, stRow);
			PrintCellOddEvenData(pExcel, _T("Mixed_Mode_RS_Moment_Longi_Top"), iOddNum, ForceD[1].dForce[4], Row_Ref, Col_Ref, stRow);
			PrintCellOddEvenData(pExcel, _T("Mixed_Mode_RS_Moment_Trans_Top"), iOddNum, ForceD[1].dForce[5], Row_Ref, Col_Ref, stRow);

			PrintCellOddEvenData(pExcel, _T("Mixed_Mode_RS_Shear_Longi_Bot"), iOddNum, ForceD[0].dForce[2], Row_Ref, Col_Ref, stRow);
			PrintCellOddEvenData(pExcel, _T("Mixed_Mode_RS_Shear_Trans_Bot"), iOddNum, ForceD[0].dForce[1], Row_Ref, Col_Ref, stRow);
			PrintCellOddEvenData(pExcel, _T("Mixed_Mode_RS_Moment_Longi_Bot"), iOddNum, ForceD[0].dForce[4], Row_Ref, Col_Ref, stRow);
			PrintCellOddEvenData(pExcel, _T("Mixed_Mode_RS_Moment_Trans_Bot"), iOddNum, ForceD[0].dForce[5], Row_Ref, Col_Ref, stRow);
		}
		else
		{
			RptBlock = _T("[MM]Mixed_Mode_X_RS_Contents");
			rowCnt = CopyRangeEvenTable(pExcel, RptBlock, stRow);
			GetCellIDByName(pExcel, RptBlock, Row_Ref, Col_Ref);
			int iEvenNum = 17;

			PrintCellOddEvenData(pExcel, _T("Mixed_Mode_RS_Name"), iEvenNum, rDataX.LoadCaseName, Row_Ref, Col_Ref, stRow);

			PrintCellOddEvenData(pExcel, _T("Mixed_Mode_RS_Load_Top"), iEvenNum, dAxial, Row_Ref, Col_Ref, stRow);
			PrintCellOddEvenData(pExcel, _T("Mixed_Mode_RS_Load_Bot"), iEvenNum, ResultD[0][0].ParamD.ForceD.dPcolbot, Row_Ref, Col_Ref, stRow);

			Calc_PierSeisRsDisp(EvgpK, iColm, keyX, dDispLong, dDispTrans);

			PrintCellOddEvenData(pExcel, _T("Mixed_Mode_RS_Disp_Longi"), iEvenNum, 1000 * dDispLong, Row_Ref, Col_Ref, stRow);
			PrintCellOddEvenData(pExcel, _T("Mixed_Mode_RS_Disp_Trans"), iEvenNum, 1000 * dDispTrans, Row_Ref, Col_Ref, stRow);

			Calc_PierSeisRsForce(EvgpK, iColm, keyX, ForceD);

			if (fabs(ForceD[1].dForce[2]) < cDGN_Zero)                                                  ForceD[1].dForce[2] = 0;
			else if (cDGN_Zero < ForceD[1].dForce[2] && ForceD[1].dForce[2] < cDGN_Zero * 10)      ForceD[1].dForce[2] = floor(ForceD[1].dForce[2] / cDGN_Zero + 0.5) * cDGN_Zero;
			else if (cDGN_Zero * 10 < ForceD[1].dForce[2] && ForceD[1].dForce[2] < cDGN_Zero * 100)     ForceD[1].dForce[2] = floor(ForceD[1].dForce[2] / cDGN_Zero / 10 + 0.5) * cDGN_Zero * 10;

			if (fabs(ForceD[1].dForce[1]) < cDGN_Zero)                                                  ForceD[1].dForce[1] = 0;
			else if (cDGN_Zero < ForceD[1].dForce[1] && ForceD[1].dForce[1] < cDGN_Zero * 10)      ForceD[1].dForce[1] = floor(ForceD[1].dForce[1] / cDGN_Zero + 0.5) * cDGN_Zero;
			else if (cDGN_Zero * 10 < ForceD[1].dForce[1] && ForceD[1].dForce[1] < cDGN_Zero * 100)     ForceD[1].dForce[1] = floor(ForceD[1].dForce[1] / cDGN_Zero / 10 + 0.5) * cDGN_Zero * 10;

			if (fabs(ForceD[1].dForce[4]) < cDGN_Zero)                                                  ForceD[1].dForce[4] = 0;
			else if (cDGN_Zero < ForceD[1].dForce[4] && ForceD[1].dForce[4] < cDGN_Zero * 10)      ForceD[1].dForce[4] = floor(ForceD[1].dForce[4] / cDGN_Zero + 0.5) * cDGN_Zero;
			else if (cDGN_Zero * 10 < ForceD[1].dForce[4] && ForceD[1].dForce[4] < cDGN_Zero * 100)     ForceD[1].dForce[4] = floor(ForceD[1].dForce[4] / cDGN_Zero / 10 + 0.5) * cDGN_Zero * 10;

			if (fabs(ForceD[1].dForce[5]) < cDGN_Zero)                                                  ForceD[1].dForce[5] = 0;
			else if (cDGN_Zero < ForceD[1].dForce[5] && ForceD[1].dForce[5] < cDGN_Zero * 10)      ForceD[1].dForce[5] = floor(ForceD[1].dForce[5] / cDGN_Zero + 0.5) * cDGN_Zero;
			else if (cDGN_Zero * 10 < ForceD[1].dForce[5] && ForceD[1].dForce[5] < cDGN_Zero * 100)     ForceD[1].dForce[5] = floor(ForceD[1].dForce[5] / cDGN_Zero / 10 + 0.5) * cDGN_Zero * 10;

			if (fabs(ForceD[0].dForce[2]) < cDGN_Zero)                                                  ForceD[0].dForce[2] = 0;
			else if (cDGN_Zero < ForceD[0].dForce[2] && ForceD[0].dForce[2] < cDGN_Zero * 10)      ForceD[0].dForce[2] = floor(ForceD[0].dForce[2] / cDGN_Zero + 0.5) * cDGN_Zero;
			else if (cDGN_Zero * 10 < ForceD[0].dForce[2] && ForceD[0].dForce[2] < cDGN_Zero * 100)     ForceD[0].dForce[2] = floor(ForceD[0].dForce[2] / cDGN_Zero / 10 + 0.5) * cDGN_Zero * 10;

			if (fabs(ForceD[0].dForce[1]) < cDGN_Zero)                                                  ForceD[0].dForce[1] = 0;
			else if (cDGN_Zero < ForceD[0].dForce[1] && ForceD[0].dForce[1] < cDGN_Zero * 10)      ForceD[0].dForce[1] = floor(ForceD[0].dForce[1] / cDGN_Zero + 0.5) * cDGN_Zero;
			else if (cDGN_Zero * 10 < ForceD[0].dForce[1] && ForceD[0].dForce[1] < cDGN_Zero * 100)     ForceD[0].dForce[1] = floor(ForceD[0].dForce[1] / cDGN_Zero / 10 + 0.5) * cDGN_Zero * 10;

			if (fabs(ForceD[0].dForce[4]) < cDGN_Zero)                                                  ForceD[0].dForce[4] = 0;
			else if (cDGN_Zero < ForceD[0].dForce[4] && ForceD[0].dForce[4] < cDGN_Zero * 10)      ForceD[0].dForce[4] = floor(ForceD[0].dForce[4] / cDGN_Zero + 0.5) * cDGN_Zero;
			else if (cDGN_Zero * 10 < ForceD[0].dForce[4] && ForceD[0].dForce[4] < cDGN_Zero * 100)     ForceD[0].dForce[4] = floor(ForceD[0].dForce[4] / cDGN_Zero / 10 + 0.5) * cDGN_Zero * 10;

			if (fabs(ForceD[0].dForce[5]) < cDGN_Zero)                                                  ForceD[0].dForce[5] = 0;
			else if (cDGN_Zero < ForceD[0].dForce[5] && ForceD[0].dForce[5] < cDGN_Zero * 10)      ForceD[0].dForce[5] = floor(ForceD[0].dForce[5] / cDGN_Zero + 0.5) * cDGN_Zero;
			else if (cDGN_Zero * 10 < ForceD[0].dForce[5] && ForceD[0].dForce[5] < cDGN_Zero * 100)     ForceD[0].dForce[5] = floor(ForceD[0].dForce[5] / cDGN_Zero / 10 + 0.5) * cDGN_Zero * 10;

			PrintCellOddEvenData(pExcel, _T("Mixed_Mode_RS_Shear_Longi_Top"), iEvenNum, ForceD[1].dForce[2], Row_Ref, Col_Ref, stRow);
			PrintCellOddEvenData(pExcel, _T("Mixed_Mode_RS_Shear_Trans_Top"), iEvenNum, ForceD[1].dForce[1], Row_Ref, Col_Ref, stRow);
			PrintCellOddEvenData(pExcel, _T("Mixed_Mode_RS_Moment_Longi_Top"), iEvenNum, ForceD[1].dForce[4], Row_Ref, Col_Ref, stRow);
			PrintCellOddEvenData(pExcel, _T("Mixed_Mode_RS_Moment_Trans_Top"), iEvenNum, ForceD[1].dForce[5], Row_Ref, Col_Ref, stRow);

			PrintCellOddEvenData(pExcel, _T("Mixed_Mode_RS_Shear_Longi_Bot"), iEvenNum, ForceD[0].dForce[2], Row_Ref, Col_Ref, stRow);
			PrintCellOddEvenData(pExcel, _T("Mixed_Mode_RS_Shear_Trans_Bot"), iEvenNum, ForceD[0].dForce[1], Row_Ref, Col_Ref, stRow);
			PrintCellOddEvenData(pExcel, _T("Mixed_Mode_RS_Moment_Longi_Bot"), iEvenNum, ForceD[0].dForce[4], Row_Ref, Col_Ref, stRow);
			PrintCellOddEvenData(pExcel, _T("Mixed_Mode_RS_Moment_Trans_Bot"), iEvenNum, ForceD[0].dForce[5], Row_Ref, Col_Ref, stRow);

			stRow += rowCnt;
		}
	}

	stRow += rowCnt;

	for (int j = 0; j < iRSNumY; j++)
	{
		T_SPLC_K keyY = aYKey[j];
		T_SPLC_D rDataY;
		m_pDoc->m_pAttrCtrl->GetSplc(keyY, rDataY);

		if (j == 0)
		{
			RptBlock = _T("[MM]Mixed_Mode_RS_Y");
			rowCnt = CopyRangeSubTitle(pExcel, RptBlock, stRow);
			stRow += rowCnt;
		}


		if (j % 2 == 0)
		{
			RptBlock = _T("[MM]Mixed_Mode_Y_RS_Table");
			rowCnt = CopyRangeSubTitle(pExcel, RptBlock, stRow);

			RptBlock = _T("[MM]Mixed_Mode_Y_RS_Contents");
			rowCnt = CopyRangeOddTable(pExcel, RptBlock, stRow);
			GetCellIDByName(pExcel, RptBlock, Row_Ref, Col_Ref);
			int iOddNum = 7;

			PrintCellOddEvenData(pExcel, _T("Mixed_Mode_Y_RS_Name"), iOddNum, rDataY.LoadCaseName, Row_Ref, Col_Ref, stRow);

			PrintCellOddEvenData(pExcel, _T("Mixed_Mode_Y_RS_Load_Top"), iOddNum, dAxial, Row_Ref, Col_Ref, stRow);
			PrintCellOddEvenData(pExcel, _T("Mixed_Mode_Y_RS_Load_Bot"), iOddNum, ResultD[0][0].ParamD.ForceD.dPcolbot, Row_Ref, Col_Ref, stRow);

			Calc_PierSeisRsDisp(EvgpK, iColm, keyY, dDispLong, dDispTrans);

			PrintCellOddEvenData(pExcel, _T("Mixed_Mode_Y_RS_Disp_Longi"), iOddNum, 1000 * dDispLong, Row_Ref, Col_Ref, stRow);
			PrintCellOddEvenData(pExcel, _T("Mixed_Mode_Y_RS_Disp_Trans"), iOddNum, 1000 * dDispTrans, Row_Ref, Col_Ref, stRow);

			Calc_PierSeisRsForce(EvgpK, iColm, keyY, ForceD);

			if (fabs(ForceD[1].dForce[2]) < cDGN_Zero)                                                  ForceD[1].dForce[2] = 0;
			else if (cDGN_Zero < ForceD[1].dForce[2] && ForceD[1].dForce[2] < cDGN_Zero * 10)      ForceD[1].dForce[2] = floor(ForceD[1].dForce[2] / cDGN_Zero + 0.5) * cDGN_Zero;
			else if (cDGN_Zero * 10 < ForceD[1].dForce[2] && ForceD[1].dForce[2] < cDGN_Zero * 100)     ForceD[1].dForce[2] = floor(ForceD[1].dForce[2] / cDGN_Zero / 10 + 0.5) * cDGN_Zero * 10;

			if (fabs(ForceD[1].dForce[1]) < cDGN_Zero)                                                  ForceD[1].dForce[1] = 0;
			else if (cDGN_Zero < ForceD[1].dForce[1] && ForceD[1].dForce[1] < cDGN_Zero * 10)      ForceD[1].dForce[1] = floor(ForceD[1].dForce[1] / cDGN_Zero + 0.5) * cDGN_Zero;
			else if (cDGN_Zero * 10 < ForceD[1].dForce[1] && ForceD[1].dForce[1] < cDGN_Zero * 100)     ForceD[1].dForce[1] = floor(ForceD[1].dForce[1] / cDGN_Zero / 10 + 0.5) * cDGN_Zero * 10;

			if (fabs(ForceD[1].dForce[4]) < cDGN_Zero)                                                  ForceD[1].dForce[4] = 0;
			else if (cDGN_Zero < ForceD[1].dForce[4] && ForceD[1].dForce[4] < cDGN_Zero * 10)      ForceD[1].dForce[4] = floor(ForceD[1].dForce[4] / cDGN_Zero + 0.5) * cDGN_Zero;
			else if (cDGN_Zero * 10 < ForceD[1].dForce[4] && ForceD[1].dForce[4] < cDGN_Zero * 100)     ForceD[1].dForce[4] = floor(ForceD[1].dForce[4] / cDGN_Zero / 10 + 0.5) * cDGN_Zero * 10;

			if (fabs(ForceD[1].dForce[5]) < cDGN_Zero)                                                  ForceD[1].dForce[5] = 0;
			else if (cDGN_Zero < ForceD[1].dForce[5] && ForceD[1].dForce[5] < cDGN_Zero * 10)      ForceD[1].dForce[5] = floor(ForceD[1].dForce[5] / cDGN_Zero + 0.5) * cDGN_Zero;
			else if (cDGN_Zero * 10 < ForceD[1].dForce[5] && ForceD[1].dForce[5] < cDGN_Zero * 100)     ForceD[1].dForce[5] = floor(ForceD[1].dForce[5] / cDGN_Zero / 10 + 0.5) * cDGN_Zero * 10;

			if (fabs(ForceD[0].dForce[2]) < cDGN_Zero)                                                  ForceD[0].dForce[2] = 0;
			else if (cDGN_Zero < ForceD[0].dForce[2] && ForceD[0].dForce[2] < cDGN_Zero * 10)      ForceD[0].dForce[2] = floor(ForceD[0].dForce[2] / cDGN_Zero + 0.5) * cDGN_Zero;
			else if (cDGN_Zero * 10 < ForceD[0].dForce[2] && ForceD[0].dForce[2] < cDGN_Zero * 100)     ForceD[0].dForce[2] = floor(ForceD[0].dForce[2] / cDGN_Zero / 10 + 0.5) * cDGN_Zero * 10;

			if (fabs(ForceD[0].dForce[1]) < cDGN_Zero)                                                  ForceD[0].dForce[1] = 0;
			else if (cDGN_Zero < ForceD[0].dForce[1] && ForceD[0].dForce[1] < cDGN_Zero * 10)      ForceD[0].dForce[1] = floor(ForceD[0].dForce[1] / cDGN_Zero + 0.5) * cDGN_Zero;
			else if (cDGN_Zero * 10 < ForceD[0].dForce[1] && ForceD[0].dForce[1] < cDGN_Zero * 100)     ForceD[0].dForce[1] = floor(ForceD[0].dForce[1] / cDGN_Zero / 10 + 0.5) * cDGN_Zero * 10;

			if (fabs(ForceD[0].dForce[4]) < cDGN_Zero)                                                  ForceD[0].dForce[4] = 0;
			else if (cDGN_Zero < ForceD[0].dForce[4] && ForceD[0].dForce[4] < cDGN_Zero * 10)      ForceD[0].dForce[4] = floor(ForceD[0].dForce[4] / cDGN_Zero + 0.5) * cDGN_Zero;
			else if (cDGN_Zero * 10 < ForceD[0].dForce[4] && ForceD[0].dForce[4] < cDGN_Zero * 100)     ForceD[0].dForce[4] = floor(ForceD[0].dForce[4] / cDGN_Zero / 10 + 0.5) * cDGN_Zero * 10;

			if (fabs(ForceD[0].dForce[5]) < cDGN_Zero)                                                  ForceD[0].dForce[5] = 0;
			else if (cDGN_Zero < ForceD[0].dForce[5] && ForceD[0].dForce[5] < cDGN_Zero * 10)      ForceD[0].dForce[5] = floor(ForceD[0].dForce[5] / cDGN_Zero + 0.5) * cDGN_Zero;
			else if (cDGN_Zero * 10 < ForceD[0].dForce[5] && ForceD[0].dForce[5] < cDGN_Zero * 100)     ForceD[0].dForce[5] = floor(ForceD[0].dForce[5] / cDGN_Zero / 10 + 0.5) * cDGN_Zero * 10;

			PrintCellOddEvenData(pExcel, _T("Mixed_Mode_Y_RS_Shear_Longi_Top"), iOddNum, ForceD[1].dForce[2], Row_Ref, Col_Ref, stRow);
			PrintCellOddEvenData(pExcel, _T("Mixed_Mode_Y_RS_Shear_Trans_Top"), iOddNum, ForceD[1].dForce[1], Row_Ref, Col_Ref, stRow);
			PrintCellOddEvenData(pExcel, _T("Mixed_Mode_Y_RS_Moment_Longi_Top"), iOddNum, ForceD[1].dForce[4], Row_Ref, Col_Ref, stRow);
			PrintCellOddEvenData(pExcel, _T("Mixed_Mode_Y_RS_Moment_Trans_Top"), iOddNum, ForceD[1].dForce[5], Row_Ref, Col_Ref, stRow);

			PrintCellOddEvenData(pExcel, _T("Mixed_Mode_Y_RS_Shear_Longi_Bot"), iOddNum, ForceD[0].dForce[2], Row_Ref, Col_Ref, stRow);
			PrintCellOddEvenData(pExcel, _T("Mixed_Mode_Y_RS_Shear_Trans_Bot"), iOddNum, ForceD[0].dForce[1], Row_Ref, Col_Ref, stRow);
			PrintCellOddEvenData(pExcel, _T("Mixed_Mode_Y_RS_Moment_Longi_Bot"), iOddNum, ForceD[0].dForce[4], Row_Ref, Col_Ref, stRow);
			PrintCellOddEvenData(pExcel, _T("Mixed_Mode_Y_RS_Moment_Trans_Bot"), iOddNum, ForceD[0].dForce[5], Row_Ref, Col_Ref, stRow);
		}
		else
		{
			RptBlock = _T("[MM]Mixed_Mode_Y_RS_Contents");

			rowCnt = CopyRangeEvenTable(pExcel, RptBlock, stRow);
			GetCellIDByName(pExcel, RptBlock, Row_Ref, Col_Ref);
			int iEvenNum = 17;

			PrintCellOddEvenData(pExcel, _T("Mixed_Mode_Y_RS_Name"), iEvenNum, rDataY.LoadCaseName, Row_Ref, Col_Ref, stRow);

			PrintCellOddEvenData(pExcel, _T("Mixed_Mode_Y_RS_Load_Top"), iEvenNum, dAxial, Row_Ref, Col_Ref, stRow);
			PrintCellOddEvenData(pExcel, _T("Mixed_Mode_Y_RS_Load_Bot"), iEvenNum, ResultD[0][0].ParamD.ForceD.dPcolbot, Row_Ref, Col_Ref, stRow);

			Calc_PierSeisRsDisp(EvgpK, iColm, keyY, dDispLong, dDispTrans);

			PrintCellOddEvenData(pExcel, _T("Mixed_Mode_Y_RS_Disp_Longi"), iEvenNum, 1000 * dDispLong, Row_Ref, Col_Ref, stRow);
			PrintCellOddEvenData(pExcel, _T("Mixed_Mode_Y_RS_Disp_Trans"), iEvenNum, 1000 * dDispTrans, Row_Ref, Col_Ref, stRow);

			Calc_PierSeisRsForce(EvgpK, iColm, keyY, ForceD);

			if (fabs(ForceD[1].dForce[2]) < cDGN_Zero)                                                        ForceD[1].dForce[2] = 0;
			else if (cDGN_Zero < ForceD[1].dForce[2] && ForceD[1].dForce[2] < cDGN_Zero * 10)      ForceD[1].dForce[2] = floor(ForceD[1].dForce[2] / cDGN_Zero + 0.5) * cDGN_Zero;
			else if (cDGN_Zero * 10 < ForceD[1].dForce[2] && ForceD[1].dForce[2] < cDGN_Zero * 100)     ForceD[1].dForce[2] = floor(ForceD[1].dForce[2] / cDGN_Zero / 10 + 0.5) * cDGN_Zero * 10;

			if (fabs(ForceD[1].dForce[1]) < cDGN_Zero)                                                        ForceD[1].dForce[1] = 0;
			else if (cDGN_Zero < ForceD[1].dForce[1] && ForceD[1].dForce[1] < cDGN_Zero * 10)      ForceD[1].dForce[1] = floor(ForceD[1].dForce[1] / cDGN_Zero + 0.5) * cDGN_Zero;
			else if (cDGN_Zero * 10 < ForceD[1].dForce[1] && ForceD[1].dForce[1] < cDGN_Zero * 100)     ForceD[1].dForce[1] = floor(ForceD[1].dForce[1] / cDGN_Zero / 10 + 0.5) * cDGN_Zero * 10;

			if (fabs(ForceD[1].dForce[4]) < cDGN_Zero)                                                        ForceD[1].dForce[4] = 0;
			else if (cDGN_Zero < ForceD[1].dForce[4] && ForceD[1].dForce[4] < cDGN_Zero * 10)      ForceD[1].dForce[4] = floor(ForceD[1].dForce[4] / cDGN_Zero + 0.5) * cDGN_Zero;
			else if (cDGN_Zero * 10 < ForceD[1].dForce[4] && ForceD[1].dForce[4] < cDGN_Zero * 100)     ForceD[1].dForce[4] = floor(ForceD[1].dForce[4] / cDGN_Zero / 10 + 0.5) * cDGN_Zero * 10;

			if (fabs(ForceD[1].dForce[5]) < cDGN_Zero)                                                        ForceD[1].dForce[5] = 0;
			else if (cDGN_Zero < ForceD[1].dForce[5] && ForceD[1].dForce[5] < cDGN_Zero * 10)      ForceD[1].dForce[5] = floor(ForceD[1].dForce[5] / cDGN_Zero + 0.5) * cDGN_Zero;
			else if (cDGN_Zero * 10 < ForceD[1].dForce[5] && ForceD[1].dForce[5] < cDGN_Zero * 100)     ForceD[1].dForce[5] = floor(ForceD[1].dForce[5] / cDGN_Zero / 10 + 0.5) * cDGN_Zero * 10;

			if (fabs(ForceD[0].dForce[2]) < cDGN_Zero)                                                        ForceD[0].dForce[2] = 0;
			else if (cDGN_Zero < ForceD[0].dForce[2] && ForceD[0].dForce[2] < cDGN_Zero * 10)      ForceD[0].dForce[2] = floor(ForceD[0].dForce[2] / cDGN_Zero + 0.5) * cDGN_Zero;
			else if (cDGN_Zero * 10 < ForceD[0].dForce[2] && ForceD[0].dForce[2] < cDGN_Zero * 100)     ForceD[0].dForce[2] = floor(ForceD[0].dForce[2] / cDGN_Zero / 10 + 0.5) * cDGN_Zero * 10;

			if (fabs(ForceD[0].dForce[1]) < cDGN_Zero)                                                        ForceD[0].dForce[1] = 0;
			else if (cDGN_Zero < ForceD[0].dForce[1] && ForceD[0].dForce[1] < cDGN_Zero * 10)      ForceD[0].dForce[1] = floor(ForceD[0].dForce[1] / cDGN_Zero + 0.5) * cDGN_Zero;
			else if (cDGN_Zero * 10 < ForceD[0].dForce[1] && ForceD[0].dForce[1] < cDGN_Zero * 100)     ForceD[0].dForce[1] = floor(ForceD[0].dForce[1] / cDGN_Zero / 10 + 0.5) * cDGN_Zero * 10;

			if (fabs(ForceD[0].dForce[4]) < cDGN_Zero)                                                        ForceD[0].dForce[4] = 0;
			else if (cDGN_Zero < ForceD[0].dForce[4] && ForceD[0].dForce[4] < cDGN_Zero * 10)      ForceD[0].dForce[4] = floor(ForceD[0].dForce[4] / cDGN_Zero + 0.5) * cDGN_Zero;
			else if (cDGN_Zero * 10 < ForceD[0].dForce[4] && ForceD[0].dForce[4] < cDGN_Zero * 100)     ForceD[0].dForce[4] = floor(ForceD[0].dForce[4] / cDGN_Zero / 10 + 0.5) * cDGN_Zero * 10;

			if (fabs(ForceD[0].dForce[5]) < cDGN_Zero)                                                        ForceD[0].dForce[5] = 0;
			else if (cDGN_Zero < ForceD[0].dForce[5] && ForceD[0].dForce[5] < cDGN_Zero * 10)      ForceD[0].dForce[5] = floor(ForceD[0].dForce[5] / cDGN_Zero + 0.5) * cDGN_Zero;
			else if (cDGN_Zero * 10 < ForceD[0].dForce[5] && ForceD[0].dForce[5] < cDGN_Zero * 100)     ForceD[0].dForce[5] = floor(ForceD[0].dForce[5] / cDGN_Zero / 10 + 0.5) * cDGN_Zero * 10;

			PrintCellOddEvenData(pExcel, _T("Mixed_Mode_Y_RS_Shear_Longi_Top"), iEvenNum, ForceD[1].dForce[2], Row_Ref, Col_Ref, stRow);
			PrintCellOddEvenData(pExcel, _T("Mixed_Mode_Y_RS_Shear_Trans_Top"), iEvenNum, ForceD[1].dForce[1], Row_Ref, Col_Ref, stRow);
			PrintCellOddEvenData(pExcel, _T("Mixed_Mode_Y_RS_Moment_Longi_Top"), iEvenNum, ForceD[1].dForce[4], Row_Ref, Col_Ref, stRow);
			PrintCellOddEvenData(pExcel, _T("Mixed_Mode_Y_RS_Moment_Trans_Top"), iEvenNum, ForceD[1].dForce[5], Row_Ref, Col_Ref, stRow);

			PrintCellOddEvenData(pExcel, _T("Mixed_Mode_Y_RS_Shear_Longi_Bot"), iEvenNum, ForceD[0].dForce[2], Row_Ref, Col_Ref, stRow);
			PrintCellOddEvenData(pExcel, _T("Mixed_Mode_Y_RS_Shear_Trans_Bot"), iEvenNum, ForceD[0].dForce[1], Row_Ref, Col_Ref, stRow);
			PrintCellOddEvenData(pExcel, _T("Mixed_Mode_Y_RS_Moment_Longi_Bot"), iEvenNum, ForceD[0].dForce[4], Row_Ref, Col_Ref, stRow);
			PrintCellOddEvenData(pExcel, _T("Mixed_Mode_Y_RS_Moment_Trans_Bot"), iEvenNum, ForceD[0].dForce[5], Row_Ref, Col_Ref, stRow);

			stRow += rowCnt;
		}
	}

	if (iRSNumZ > 0)
	{
		stRow += rowCnt;
	}

	for (int k = 0; k < iRSNumZ; k++)
	{
		T_SPLC_K keyZ = aZKey[k];
		T_SPLC_D rDataZ;
		m_pDoc->m_pAttrCtrl->GetSplc(keyZ, rDataZ);

		if (k == 0)
		{
			RptBlock = _T("[MM]Mixed_Mode_RS_Z");
			rowCnt = CopyRangeSubTitle(pExcel, RptBlock, stRow);
			stRow += rowCnt;
		}

		if (k % 2 == 0)
		{
			RptBlock = _T("[MM]Mixed_Mode_Z_RS_Table");
			rowCnt = CopyRangeSubTitle(pExcel, RptBlock, stRow);

			RptBlock = _T("[MM]Mixed_Mode_Z_RS_Contents");

			rowCnt = CopyRangeOddTable(pExcel, RptBlock, stRow);
			GetCellIDByName(pExcel, RptBlock, Row_Ref, Col_Ref);
			int iOddNum = 7;

			PrintCellOddEvenData(pExcel, _T("Mixed_Mode_Z_RS_Name"), iOddNum, rDataZ.LoadCaseName, Row_Ref, Col_Ref, stRow);

			PrintCellOddEvenData(pExcel, _T("Mixed_Mode_Z_RS_Load_Top"), iOddNum, dAxial, Row_Ref, Col_Ref, stRow);
			PrintCellOddEvenData(pExcel, _T("Mixed_Mode_Z_RS_Load_Bot"), iOddNum, ResultD[0][0].ParamD.ForceD.dPcolbot, Row_Ref, Col_Ref, stRow);

			Calc_PierSeisRsDisp(EvgpK, iColm, keyZ, dDispLong, dDispTrans);

			PrintCellOddEvenData(pExcel, _T("Mixed_Mode_Z_RS_Disp_Longi"), iOddNum, 1000 * dDispLong, Row_Ref, Col_Ref, stRow);
			PrintCellOddEvenData(pExcel, _T("Mixed_Mode_Z_RS_Disp_Trans"), iOddNum, 1000 * dDispTrans, Row_Ref, Col_Ref, stRow);

			Calc_PierSeisRsForce(EvgpK, iColm, keyZ, ForceD);

			if (fabs(ForceD[1].dForce[2]) < cDGN_Zero)                                                  ForceD[1].dForce[2] = 0;
			else if (cDGN_Zero < ForceD[1].dForce[2] && ForceD[1].dForce[2] < cDGN_Zero * 10)      ForceD[1].dForce[2] = floor(ForceD[1].dForce[2] / cDGN_Zero + 0.5) * cDGN_Zero;
			else if (cDGN_Zero * 10 < ForceD[1].dForce[2] && ForceD[1].dForce[2] < cDGN_Zero * 100)     ForceD[1].dForce[2] = floor(ForceD[1].dForce[2] / cDGN_Zero / 10 + 0.5) * cDGN_Zero * 10;

			if (fabs(ForceD[1].dForce[1]) < cDGN_Zero)                                                  ForceD[1].dForce[1] = 0;
			else if (cDGN_Zero < ForceD[1].dForce[1] && ForceD[1].dForce[1] < cDGN_Zero * 10)      ForceD[1].dForce[1] = floor(ForceD[1].dForce[1] / cDGN_Zero + 0.5) * cDGN_Zero;
			else if (cDGN_Zero * 10 < ForceD[1].dForce[1] && ForceD[1].dForce[1] < cDGN_Zero * 100)     ForceD[1].dForce[1] = floor(ForceD[1].dForce[1] / cDGN_Zero / 10 + 0.5) * cDGN_Zero * 10;

			if (fabs(ForceD[1].dForce[4]) < cDGN_Zero)                                                  ForceD[1].dForce[4] = 0;
			else if (cDGN_Zero < ForceD[1].dForce[4] && ForceD[1].dForce[4] < cDGN_Zero * 10)      ForceD[1].dForce[4] = floor(ForceD[1].dForce[4] / cDGN_Zero + 0.5) * cDGN_Zero;
			else if (cDGN_Zero * 10 < ForceD[1].dForce[4] && ForceD[1].dForce[4] < cDGN_Zero * 100)     ForceD[1].dForce[4] = floor(ForceD[1].dForce[4] / cDGN_Zero / 10 + 0.5) * cDGN_Zero * 10;

			if (fabs(ForceD[1].dForce[5]) < cDGN_Zero)                                                  ForceD[1].dForce[5] = 0;
			else if (cDGN_Zero < ForceD[1].dForce[5] && ForceD[1].dForce[5] < cDGN_Zero * 10)      ForceD[1].dForce[5] = floor(ForceD[1].dForce[5] / cDGN_Zero + 0.5) * cDGN_Zero;
			else if (cDGN_Zero * 10 < ForceD[1].dForce[5] && ForceD[1].dForce[5] < cDGN_Zero * 100)     ForceD[1].dForce[5] = floor(ForceD[1].dForce[5] / cDGN_Zero / 10 + 0.5) * cDGN_Zero * 10;

			if (fabs(ForceD[0].dForce[2]) < cDGN_Zero)                                                  ForceD[0].dForce[2] = 0;
			else if (cDGN_Zero < ForceD[0].dForce[2] && ForceD[0].dForce[2] < cDGN_Zero * 10)      ForceD[0].dForce[2] = floor(ForceD[0].dForce[2] / cDGN_Zero + 0.5) * cDGN_Zero;
			else if (cDGN_Zero * 10 < ForceD[0].dForce[2] && ForceD[0].dForce[2] < cDGN_Zero * 100)     ForceD[0].dForce[2] = floor(ForceD[0].dForce[2] / cDGN_Zero / 10 + 0.5) * cDGN_Zero * 10;

			if (fabs(ForceD[0].dForce[1]) < cDGN_Zero)                                                  ForceD[0].dForce[1] = 0;
			else if (cDGN_Zero < ForceD[0].dForce[1] && ForceD[0].dForce[1] < cDGN_Zero * 10)      ForceD[0].dForce[1] = floor(ForceD[0].dForce[1] / cDGN_Zero + 0.5) * cDGN_Zero;
			else if (cDGN_Zero * 10 < ForceD[0].dForce[1] && ForceD[0].dForce[1] < cDGN_Zero * 100)     ForceD[0].dForce[1] = floor(ForceD[0].dForce[1] / cDGN_Zero / 10 + 0.5) * cDGN_Zero * 10;

			if (fabs(ForceD[0].dForce[4]) < cDGN_Zero)                                                  ForceD[0].dForce[4] = 0;
			else if (cDGN_Zero < ForceD[0].dForce[4] && ForceD[0].dForce[4] < cDGN_Zero * 10)      ForceD[0].dForce[4] = floor(ForceD[0].dForce[4] / cDGN_Zero + 0.5) * cDGN_Zero;
			else if (cDGN_Zero * 10 < ForceD[0].dForce[4] && ForceD[0].dForce[4] < cDGN_Zero * 100)     ForceD[0].dForce[4] = floor(ForceD[0].dForce[4] / cDGN_Zero / 10 + 0.5) * cDGN_Zero * 10;

			if (fabs(ForceD[0].dForce[5]) < cDGN_Zero)                                                  ForceD[0].dForce[5] = 0;
			else if (cDGN_Zero < ForceD[0].dForce[5] && ForceD[0].dForce[5] < cDGN_Zero * 10)      ForceD[0].dForce[5] = floor(ForceD[0].dForce[5] / cDGN_Zero + 0.5) * cDGN_Zero;
			else if (cDGN_Zero * 10 < ForceD[0].dForce[5] && ForceD[0].dForce[5] < cDGN_Zero * 100)     ForceD[0].dForce[5] = floor(ForceD[0].dForce[5] / cDGN_Zero / 10 + 0.5) * cDGN_Zero * 10;

			PrintCellOddEvenData(pExcel, _T("Mixed_Mode_Z_RS_Shear_Longi_Top"), iOddNum, ForceD[1].dForce[2], Row_Ref, Col_Ref, stRow);
			PrintCellOddEvenData(pExcel, _T("Mixed_Mode_Z_RS_Shear_Trans_Top"), iOddNum, ForceD[1].dForce[1], Row_Ref, Col_Ref, stRow);
			PrintCellOddEvenData(pExcel, _T("Mixed_Mode_Z_RS_Moment_Longi_Top"), iOddNum, ForceD[1].dForce[4], Row_Ref, Col_Ref, stRow);
			PrintCellOddEvenData(pExcel, _T("Mixed_Mode_Z_RS_Moment_Trans_Top"), iOddNum, ForceD[1].dForce[5], Row_Ref, Col_Ref, stRow);

			PrintCellOddEvenData(pExcel, _T("Mixed_Mode_Z_RS_Shear_Longi_Bot"), iOddNum, ForceD[0].dForce[2], Row_Ref, Col_Ref, stRow);
			PrintCellOddEvenData(pExcel, _T("Mixed_Mode_Z_RS_Shear_Trans_Bot"), iOddNum, ForceD[0].dForce[1], Row_Ref, Col_Ref, stRow);
			PrintCellOddEvenData(pExcel, _T("Mixed_Mode_Z_RS_Moment_Longi_Bot"), iOddNum, ForceD[0].dForce[4], Row_Ref, Col_Ref, stRow);
			PrintCellOddEvenData(pExcel, _T("Mixed_Mode_Z_RS_Moment_Trans_Bot"), iOddNum, ForceD[0].dForce[5], Row_Ref, Col_Ref, stRow);
		}
		else
		{
			RptBlock = _T("[MM]Mixed_Mode_Z_RS_Contents");

			rowCnt = CopyRangeEvenTable(pExcel, RptBlock, stRow);
			GetCellIDByName(pExcel, RptBlock, Row_Ref, Col_Ref);
			int iEvenNum = 17;

			PrintCellOddEvenData(pExcel, _T("Mixed_Mode_Z_RS_Name"), iEvenNum, rDataZ.LoadCaseName, Row_Ref, Col_Ref, stRow);

			PrintCellOddEvenData(pExcel, _T("Mixed_Mode_Z_RS_Load_Top"), iEvenNum, dAxial, Row_Ref, Col_Ref, stRow);
			PrintCellOddEvenData(pExcel, _T("Mixed_Mode_Z_RS_Load_Bot"), iEvenNum, ResultD[0][0].ParamD.ForceD.dPcolbot, Row_Ref, Col_Ref, stRow);

			Calc_PierSeisRsDisp(EvgpK, iColm, keyZ, dDispLong, dDispTrans);

			PrintCellOddEvenData(pExcel, _T("Mixed_Mode_Z_RS_Disp_Longi"), iEvenNum, 1000 * dDispLong, Row_Ref, Col_Ref, stRow);
			PrintCellOddEvenData(pExcel, _T("Mixed_Mode_Z_RS_Disp_Trans"), iEvenNum, 1000 * dDispTrans, Row_Ref, Col_Ref, stRow);

			Calc_PierSeisRsForce(EvgpK, iColm, keyZ, ForceD);
			if (fabs(ForceD[1].dForce[2]) < cDGN_Zero)                                                  ForceD[1].dForce[2] = 0;
			else if (cDGN_Zero < ForceD[1].dForce[2] && ForceD[1].dForce[2] < cDGN_Zero * 10)      ForceD[1].dForce[2] = floor(ForceD[1].dForce[2] / cDGN_Zero + 0.5) * cDGN_Zero;
			else if (cDGN_Zero * 10 < ForceD[1].dForce[2] && ForceD[1].dForce[2] < cDGN_Zero * 100)     ForceD[1].dForce[2] = floor(ForceD[1].dForce[2] / cDGN_Zero / 10 + 0.5) * cDGN_Zero * 10;

			if (fabs(ForceD[1].dForce[1]) < cDGN_Zero)                                                  ForceD[1].dForce[1] = 0;
			else if (cDGN_Zero < ForceD[1].dForce[1] && ForceD[1].dForce[1] < cDGN_Zero * 10)      ForceD[1].dForce[1] = floor(ForceD[1].dForce[1] / cDGN_Zero + 0.5) * cDGN_Zero;
			else if (cDGN_Zero * 10 < ForceD[1].dForce[1] && ForceD[1].dForce[1] < cDGN_Zero * 100)     ForceD[1].dForce[1] = floor(ForceD[1].dForce[1] / cDGN_Zero / 10 + 0.5) * cDGN_Zero * 10;

			if (fabs(ForceD[1].dForce[4]) < cDGN_Zero)                                                  ForceD[1].dForce[4] = 0;
			else if (cDGN_Zero < ForceD[1].dForce[4] && ForceD[1].dForce[4] < cDGN_Zero * 10)      ForceD[1].dForce[4] = floor(ForceD[1].dForce[4] / cDGN_Zero + 0.5) * cDGN_Zero;
			else if (cDGN_Zero * 10 < ForceD[1].dForce[4] && ForceD[1].dForce[4] < cDGN_Zero * 100)     ForceD[1].dForce[4] = floor(ForceD[1].dForce[4] / cDGN_Zero / 10 + 0.5) * cDGN_Zero * 10;

			if (fabs(ForceD[1].dForce[5]) < cDGN_Zero)                                                  ForceD[1].dForce[5] = 0;
			else if (cDGN_Zero < ForceD[1].dForce[5] && ForceD[1].dForce[5] < cDGN_Zero * 10)      ForceD[1].dForce[5] = floor(ForceD[1].dForce[5] / cDGN_Zero + 0.5) * cDGN_Zero;
			else if (cDGN_Zero * 10 < ForceD[1].dForce[5] && ForceD[1].dForce[5] < cDGN_Zero * 100)     ForceD[1].dForce[5] = floor(ForceD[1].dForce[5] / cDGN_Zero / 10 + 0.5) * cDGN_Zero * 10;

			if (fabs(ForceD[0].dForce[2]) < cDGN_Zero)                                                  ForceD[0].dForce[2] = 0;
			else if (cDGN_Zero < ForceD[0].dForce[2] && ForceD[0].dForce[2] < cDGN_Zero * 10)      ForceD[0].dForce[2] = floor(ForceD[0].dForce[2] / cDGN_Zero + 0.5) * cDGN_Zero;
			else if (cDGN_Zero * 10 < ForceD[0].dForce[2] && ForceD[0].dForce[2] < cDGN_Zero * 100)     ForceD[0].dForce[2] = floor(ForceD[0].dForce[2] / cDGN_Zero / 10 + 0.5) * cDGN_Zero * 10;

			if (fabs(ForceD[0].dForce[1]) < cDGN_Zero)                                                  ForceD[0].dForce[1] = 0;
			else if (cDGN_Zero < ForceD[0].dForce[1] && ForceD[0].dForce[1] < cDGN_Zero * 10)      ForceD[0].dForce[1] = floor(ForceD[0].dForce[1] / cDGN_Zero + 0.5) * cDGN_Zero;
			else if (cDGN_Zero * 10 < ForceD[0].dForce[1] && ForceD[0].dForce[1] < cDGN_Zero * 100)     ForceD[0].dForce[1] = floor(ForceD[0].dForce[1] / cDGN_Zero / 10 + 0.5) * cDGN_Zero * 10;

			if (fabs(ForceD[0].dForce[4]) < cDGN_Zero)                                                  ForceD[0].dForce[4] = 0;
			else if (cDGN_Zero < ForceD[0].dForce[4] && ForceD[0].dForce[4] < cDGN_Zero * 10)      ForceD[0].dForce[4] = floor(ForceD[0].dForce[4] / cDGN_Zero + 0.5) * cDGN_Zero;
			else if (cDGN_Zero * 10 < ForceD[0].dForce[4] && ForceD[0].dForce[4] < cDGN_Zero * 100)     ForceD[0].dForce[4] = floor(ForceD[0].dForce[4] / cDGN_Zero / 10 + 0.5) * cDGN_Zero * 10;

			if (fabs(ForceD[0].dForce[5]) < cDGN_Zero)                                                  ForceD[0].dForce[5] = 0;
			else if (cDGN_Zero < ForceD[0].dForce[5] && ForceD[0].dForce[5] < cDGN_Zero * 10)      ForceD[0].dForce[5] = floor(ForceD[0].dForce[5] / cDGN_Zero + 0.5) * cDGN_Zero;
			else if (cDGN_Zero * 10 < ForceD[0].dForce[5] && ForceD[0].dForce[5] < cDGN_Zero * 100)     ForceD[0].dForce[5] = floor(ForceD[0].dForce[5] / cDGN_Zero / 10 + 0.5) * cDGN_Zero * 10;

			PrintCellOddEvenData(pExcel, _T("Mixed_Mode_Z_RS_Shear_Longi_Top"), iEvenNum, ForceD[1].dForce[2], Row_Ref, Col_Ref, stRow);
			PrintCellOddEvenData(pExcel, _T("Mixed_Mode_Z_RS_Shear_Trans_Top"), iEvenNum, ForceD[1].dForce[1], Row_Ref, Col_Ref, stRow);
			PrintCellOddEvenData(pExcel, _T("Mixed_Mode_Z_RS_Moment_Longi_Top"), iEvenNum, ForceD[1].dForce[4], Row_Ref, Col_Ref, stRow);
			PrintCellOddEvenData(pExcel, _T("Mixed_Mode_Z_RS_Moment_Trans_Top"), iEvenNum, ForceD[1].dForce[5], Row_Ref, Col_Ref, stRow);

			PrintCellOddEvenData(pExcel, _T("Mixed_Mode_Z_RS_Shear_Longi_Bot"), iEvenNum, ForceD[0].dForce[2], Row_Ref, Col_Ref, stRow);
			PrintCellOddEvenData(pExcel, _T("Mixed_Mode_Z_RS_Shear_Trans_Bot"), iEvenNum, ForceD[0].dForce[1], Row_Ref, Col_Ref, stRow);
			PrintCellOddEvenData(pExcel, _T("Mixed_Mode_Z_RS_Moment_Longi_Bot"), iEvenNum, ForceD[0].dForce[4], Row_Ref, Col_Ref, stRow);
			PrintCellOddEvenData(pExcel, _T("Mixed_Mode_Z_RS_Moment_Trans_Bot"), iEvenNum, ForceD[0].dForce[5], Row_Ref, Col_Ref, stRow);

			stRow += rowCnt;
		}
	}

	if (iRSNumZ > 0)
	{
		stRow += rowCnt;
		RptBlock = _T("[MM]Mixed_Mode_RS_Comment");
		rowCnt = CopyRangeSubTitle(pExcel, RptBlock, stRow);
	}
	else
	{
		stRow += rowCnt;
		RptBlock = _T("[MM]Mixed_Mode_RS_Comment_XY");
		rowCnt = CopyRangeSubTitle(pExcel, RptBlock, stRow);
	}

	stRow += rowCnt;

	return TRUE;
}

BOOL CBrdgSeisEvalKISTEC2015_Report::GenerateMixedModeCombInfo(IExcel* pExcel, int& stRow, const double& dAxial, const UINT& EvgpK, const UINT& iColm, T_BRDGEVAL_RESULT ResultD[2][2], const CString& strEvgp)
{
	auto l_ExistLcom = [&](const int& nType)
	{
		T_LCOM_K_LIST arLcomK;
		m_pDoc->m_pAttrCtrl->GetLcomKeyList(D_LCOMTYPE_SEISMIC, arLcomK);
		int nLcom = arLcomK.GetSize();
		for(int i = 0; i < nLcom; ++i)
		{
			T_LCOM_D LcomD;
			if(!m_pDoc->m_pAttrCtrl->GetLcom(D_LCOMTYPE_SEISMIC, arLcomK[i], LcomD)) continue;
			if(LcomD.nSeisLcomType == nType) return true;
		}
		return false;
	};

	CString RptBlock;
	int rowCnt = 0;

	short Row_Ref = 0, Col_Ref = 0;

	RptBlock = _T("[MM]Mixed_Mode_Comb_Head");
	rowCnt = CopyRangeSubTitle(pExcel, RptBlock, stRow);
	//GetCellIDByName(pExcel, RptBlock, Row_Ref, Col_Ref);
	stRow += rowCnt;

	T_BRDGEVAL_FORCE ForceD;
	double dMaxDisp = 0.0;
	UINT LinkK = 0;
	const bool bExistLongi = l_ExistLcom(1);
	const bool bExistTrans = l_ExistLcom(2);
	const bool bExistVerti = l_ExistLcom(3);

	if(bExistLongi)
	{
		RptBlock = _T("[MM]Mixed_Mode_Comb1_Table");
		rowCnt = CopyRangeSubTitle(pExcel, RptBlock, stRow);
		GetCellIDByName(pExcel, RptBlock, Row_Ref, Col_Ref);

		// 1번째; combination1의 하부
		PrintCellData(pExcel, _T("Mixed_Mode_Comb1_Load_Top"), dAxial, Row_Ref, Col_Ref, stRow);
		PrintCellData(pExcel, _T("Mixed_Mode_Comb1_Load_Bot"), ResultD[0][0].ParamD.ForceD.dPcolbot, Row_Ref, Col_Ref, stRow);
		
		Calc_PierSeisEnvDisp(EvgpK, iColm, LONGI, 1, dMaxDisp, LinkK);
		PrintCellData(pExcel, _T("Mixed_Mode_Comb1_Disp_Longi"), 1000 * dMaxDisp, Row_Ref, Col_Ref, stRow);
		Calc_PierSeisEnvDisp(EvgpK, iColm, TRANS, 1, dMaxDisp, LinkK);
		PrintCellData(pExcel, _T("Mixed_Mode_Comb1_Disp_Trans"), 1000 * dMaxDisp, Row_Ref, Col_Ref, stRow);

		Calc_PierSeisEnvForce(EvgpK, iColm, TOP, 1, ForceD);
		PrintCellData(pExcel, _T("Mixed_Mode_Comb1_Shear_Longi_Top"), ForceD.dForce[2], Row_Ref, Col_Ref, stRow);
		PrintCellData(pExcel, _T("Mixed_Mode_Comb1_Shear_Trans_Top"), ForceD.dForce[1], Row_Ref, Col_Ref, stRow);
		PrintCellData(pExcel, _T("Mixed_Mode_Comb1_Moment_Longi_Top"), ForceD.dForce[4], Row_Ref, Col_Ref, stRow);
		PrintCellData(pExcel, _T("Mixed_Mode_Comb1_Moment_Trans_Top"), ForceD.dForce[5], Row_Ref, Col_Ref, stRow);

		Calc_PierSeisEnvForce(EvgpK, iColm, BOTTOM, 1, ForceD);
		PrintCellData(pExcel, _T("Mixed_Mode_Comb1_Shear_Longi_Bot"), ForceD.dForce[2], Row_Ref, Col_Ref, stRow);
		PrintCellData(pExcel, _T("Mixed_Mode_Comb1_Shear_Trans_Bot"), ForceD.dForce[1], Row_Ref, Col_Ref, stRow);
		PrintCellData(pExcel, _T("Mixed_Mode_Comb1_Moment_Longi_Bot"), ForceD.dForce[4], Row_Ref, Col_Ref, stRow);
		PrintCellData(pExcel, _T("Mixed_Mode_Comb1_Moment_Trans_Bot"), ForceD.dForce[5], Row_Ref, Col_Ref, stRow);

		stRow += rowCnt;

	}

	if(bExistTrans)
	{
		RptBlock = _T("[MM]Mixed_Mode_Comb2_Table");
		rowCnt = CopyRangeSubTitle(pExcel, RptBlock, stRow);
		GetCellIDByName(pExcel, RptBlock, Row_Ref, Col_Ref);

		// 2번째; combination2의 하부
		PrintCellData(pExcel, _T("Mixed_Mode_Comb2_Load_Top"), dAxial, Row_Ref, Col_Ref, stRow);
		PrintCellData(pExcel, _T("Mixed_Mode_Comb2_Load_Bot"), ResultD[0][0].ParamD.ForceD.dPcolbot, Row_Ref, Col_Ref, stRow);

		Calc_PierSeisEnvDisp(EvgpK, iColm, LONGI, 2, dMaxDisp, LinkK);
		PrintCellData(pExcel, _T("Mixed_Mode_Comb2_Disp_Longi"), 1000 * dMaxDisp, Row_Ref, Col_Ref, stRow);
		Calc_PierSeisEnvDisp(EvgpK, iColm, TRANS, 2, dMaxDisp, LinkK);
		PrintCellData(pExcel, _T("Mixed_Mode_Comb2_Disp_Trans"), 1000 * dMaxDisp, Row_Ref, Col_Ref, stRow);

		Calc_PierSeisEnvForce(EvgpK, iColm, TOP, 2, ForceD);
		PrintCellData(pExcel, _T("Mixed_Mode_Comb2_Shear_Longi_Top"), ForceD.dForce[2], Row_Ref, Col_Ref, stRow);
		PrintCellData(pExcel, _T("Mixed_Mode_Comb2_Shear_Trans_Top"), ForceD.dForce[1], Row_Ref, Col_Ref, stRow);
		PrintCellData(pExcel, _T("Mixed_Mode_Comb2_Moment_Longi_Top"), ForceD.dForce[4], Row_Ref, Col_Ref, stRow);
		PrintCellData(pExcel, _T("Mixed_Mode_Comb2_Moment_Trans_Top"), ForceD.dForce[5], Row_Ref, Col_Ref, stRow);

		Calc_PierSeisEnvForce(EvgpK, iColm, BOTTOM, 2, ForceD);
		PrintCellData(pExcel, _T("Mixed_Mode_Comb2_Shear_Longi_Bot"), ForceD.dForce[2], Row_Ref, Col_Ref, stRow);
		PrintCellData(pExcel, _T("Mixed_Mode_Comb2_Shear_Trans_Bot"), ForceD.dForce[1], Row_Ref, Col_Ref, stRow);
		PrintCellData(pExcel, _T("Mixed_Mode_Comb2_Moment_Longi_Bot"), ForceD.dForce[4], Row_Ref, Col_Ref, stRow);
		PrintCellData(pExcel, _T("Mixed_Mode_Comb2_Moment_Trans_Bot"), ForceD.dForce[5], Row_Ref, Col_Ref, stRow);

		stRow += rowCnt;
	}

	
	if(bExistVerti)
	{
		RptBlock = _T("[MM]Mixed_Mode_Comb3_Table");
		rowCnt = CopyRangeSubTitle(pExcel, RptBlock, stRow);
		GetCellIDByName(pExcel, RptBlock, Row_Ref, Col_Ref);

		PrintCellData(pExcel, _T("Mixed_Mode_Comb3_Load_Top"), dAxial, Row_Ref, Col_Ref, stRow);
		PrintCellData(pExcel, _T("Mixed_Mode_Comb3_Load_Bot"), ResultD[0][0].ParamD.ForceD.dPcolbot, Row_Ref, Col_Ref, stRow);

		Calc_PierSeisEnvDisp(EvgpK, iColm, LONGI, 3, dMaxDisp, LinkK);
		PrintCellData(pExcel, _T("Mixed_Mode_Comb3_Disp_Longi"), 1000 * dMaxDisp, Row_Ref, Col_Ref, stRow);
		Calc_PierSeisEnvDisp(EvgpK, iColm, TRANS, 3, dMaxDisp, LinkK);
		PrintCellData(pExcel, _T("Mixed_Mode_Comb3_Disp_Trans"), 1000 * dMaxDisp, Row_Ref, Col_Ref, stRow);

		Calc_PierSeisEnvForce(EvgpK, iColm, TOP, 3, ForceD);
		PrintCellData(pExcel, _T("Mixed_Mode_Comb3_Shear_Longi_Top"), ForceD.dForce[2], Row_Ref, Col_Ref, stRow);
		PrintCellData(pExcel, _T("Mixed_Mode_Comb3_Shear_Trans_Top"), ForceD.dForce[1], Row_Ref, Col_Ref, stRow);
		PrintCellData(pExcel, _T("Mixed_Mode_Comb3_Moment_Longi_Top"), ForceD.dForce[4], Row_Ref, Col_Ref, stRow);
		PrintCellData(pExcel, _T("Mixed_Mode_Comb3_Moment_Trans_Top"), ForceD.dForce[5], Row_Ref, Col_Ref, stRow);

		Calc_PierSeisEnvForce(EvgpK, iColm, BOTTOM, 3, ForceD);
		PrintCellData(pExcel, _T("Mixed_Mode_Comb3_Shear_Longi_Bot"), ForceD.dForce[2], Row_Ref, Col_Ref, stRow);
		PrintCellData(pExcel, _T("Mixed_Mode_Comb3_Shear_Trans_Bot"), ForceD.dForce[1], Row_Ref, Col_Ref, stRow);
		PrintCellData(pExcel, _T("Mixed_Mode_Comb3_Moment_Longi_Bot"), ForceD.dForce[4], Row_Ref, Col_Ref, stRow);
		PrintCellData(pExcel, _T("Mixed_Mode_Comb3_Moment_Trans_Bot"), ForceD.dForce[5], Row_Ref, Col_Ref, stRow);

		stRow += rowCnt;
	}


	if (bExistVerti)
	{
		RptBlock = _T("[MM]Mixed_Mode_Comb_Comment_All");
		rowCnt = CopyRangeSubTitle(pExcel, RptBlock, stRow);
	}
	else
	{
		RptBlock = _T("[MM]Mixed_Mode_Comb_Comment");
		rowCnt = CopyRangeSubTitle(pExcel, RptBlock, stRow);
	}

	stRow += rowCnt;

	return TRUE;

}


BOOL CBrdgSeisEvalKISTEC2015_Report::GenerateMixedModeEnvCombInfo(IExcel* pExcel, int& stRow, const double& dAxial, const UINT& EvgpK, const UINT& iColm, T_BRDGEVAL_RESULT ResultD[2][2], const CString& strEvgp, const int nLoadType)
{
	CString RptBlock;
	int rowCnt = 0;

	int nCurRow = 0;
	CString strCurCell;

	short Row_Ref = 0, Col_Ref = 0;

	T_BRDGEVAL_FORCE ForceD;
	double dMaxDisp = 0.0;


	RptBlock = _T("[MM]Mixed_Mode_Env_Comb_Head");
	rowCnt = CopyRangeSubTitle(pExcel, RptBlock, stRow);
	//GetCellIDByName(pExcel, RptBlock, Row_Ref, Col_Ref);
	stRow += rowCnt;

	RptBlock = _T("[MM]Mixed_Mode_Env_Comb_Table");
	rowCnt = CopyRangeSubTitle(pExcel, RptBlock, stRow);
	GetCellIDByName(pExcel, RptBlock, Row_Ref, Col_Ref);

	// 이름 변경
	if (nLoadType == 1)
	{
		CString strName = _T("1)");
		CString strName2 = _T("지진력 및 변위 산정 결과");
		nCurRow = stRow;
		strCurCell.Format(_T("B%d"), nCurRow); pExcel->SetValue(strCurCell, strName);
		strCurCell.Format(_T("C%d"), nCurRow); pExcel->SetValue(strCurCell, strName2);

		nCurRow = stRow + 11;
		strCurCell.Format(_T("B%d"), nCurRow); pExcel->SetValue(strCurCell, _T(""));
		strCurCell.Format(_T("E%d"), nCurRow); pExcel->SetValue(strCurCell, _T(""));
		strCurCell.Format(_T("Q%d"), nCurRow); pExcel->SetValue(strCurCell, _T(""));
	}

	// 조합지진력 combination

	PrintCellData(pExcel, _T("Mixed_Mode_Env_Load_Top"), dAxial, Row_Ref, Col_Ref, stRow);
	PrintCellData(pExcel, _T("Mixed_Mode_Env_Load_Bot"), ResultD[0][0].ParamD.ForceD.dPcolbot, Row_Ref, Col_Ref, stRow);

	UINT LinkK;
	Calc_PierSeisEnvDisp(EvgpK, iColm, LONGI, 0, dMaxDisp, LinkK);

	PrintCellData(pExcel, _T("Mixed_Mode_Env_Disp_Longi"), 1000.0 * dMaxDisp, Row_Ref, Col_Ref, stRow);

	Calc_PierSeisEnvDisp(EvgpK, iColm, TRANS, 0, dMaxDisp, LinkK);

	PrintCellData(pExcel, _T("Mixed_Mode_Env_Disp_Trans"), 1000.0 * dMaxDisp, Row_Ref, Col_Ref, stRow);

	Calc_PierSeisEnvForce(EvgpK, iColm, TOP, 0, ForceD);
	PrintCellData(pExcel, _T("Mixed_Mode_Env_Shear_Longi_Top"), ForceD.dForce[2], Row_Ref, Col_Ref, stRow);
	PrintCellData(pExcel, _T("Mixed_Mode_Env_Shear_Trans_Top"), ForceD.dForce[1], Row_Ref, Col_Ref, stRow);
	PrintCellData(pExcel, _T("Mixed_Mode_Env_Moment_Longi_Top"), ForceD.dForce[4], Row_Ref, Col_Ref, stRow);
	PrintCellData(pExcel, _T("Mixed_Mode_Env_Moment_Trans_Top"), ForceD.dForce[5], Row_Ref, Col_Ref, stRow);

	Calc_PierSeisEnvForce(EvgpK, iColm, BOTTOM, 0, ForceD);
	PrintCellData(pExcel, _T("Mixed_Mode_Env_Shear_Longi_Bot"), ForceD.dForce[2], Row_Ref, Col_Ref, stRow);
	PrintCellData(pExcel, _T("Mixed_Mode_Env_Shear_Trans_Bot"), ForceD.dForce[1], Row_Ref, Col_Ref, stRow);
	PrintCellData(pExcel, _T("Mixed_Mode_Env_Moment_Longi_Bot"), ForceD.dForce[4], Row_Ref, Col_Ref, stRow);
	PrintCellData(pExcel, _T("Mixed_Mode_Env_Moment_Trans_Bot"), ForceD.dForce[5], Row_Ref, Col_Ref, stRow);

	stRow += rowCnt;

	return TRUE;

}

BOOL CBrdgSeisEvalKISTEC2015_Report::PrintModelView(IExcel* pExcel, CString& strModelViewName, int& stRow)
{

	int nH = 19;
	int nW = 25;

	CString strPath = strModelViewName;

	if (WaitForFile(strPath))
	{
		pExcel->AddPicture_Count(stRow + 3, 2, nH, nW, strPath);
	}

	return TRUE;
}

BOOL CBrdgSeisEvalKISTEC2015_Report::GenerateStructAnalCommonInfo(IExcel* pExcel, int& stRow)
{
	CString RptBlock;
	int rowCnt = 0;
	short Row_Ref = 0, Col_Ref = 0;

	RptBlock = _T("[A]Title_Struct_Analysis");
	rowCnt = CopyRange(pExcel, RptBlock, stRow);
	stRow += rowCnt;
	RptBlock = _T("[A]Brdg_Anal_Model");
	rowCnt = CopyRangeSubTitle(pExcel, RptBlock, stRow);
	stRow += rowCnt;
	RptBlock = _T("[A]Struct_Anal_Shape");
	rowCnt = CopyRangeSubTitle(pExcel, RptBlock, stRow);
	GetCellIDByName(pExcel, RptBlock, Row_Ref, Col_Ref);

	CString strModelViewFileName;
	strModelViewFileName.Format(_T("%s_Model.emf"), m_strPicPath);

	// Send Message로 처리한 것 명시적으로 처리 해야 할 듯. 
	// UI/UX 변경 이후 Command Routing이 비정상적으로 동작하는 경우가 있음.
	// 전체적으로 수정해야 할 듯. 
	AfxGetMainWnd()->SendMessage(WM_COMMAND, MAKEWPARAM(ID_VIEW_INITIALVIEW, 0));
	AfxGetMainWnd()->SendMessage(WM_COMMAND, MAKEWPARAM(ID_VIEW_VPOINT_ISO, 0), 0);  
	//위 두 함수를 호출하지 않아도 Line두께등 Rendering 설정이 바뀜. 
	//PrintModeView 함수 호출 내부에서 EMF 출력에서 영향 받는 듯. 
	//Export EMF, BMP 등등 바뀐 구조에 맞게 변경된 Export Graphics 루틴을 사용하도록 수정할 것 !! 

	CString strPath = strModelViewFileName;
	CServiceHeadOffice* pServiceHeadOffice = CServiceHeadOffice::Instance();
	ASSERT(pServiceHeadOffice);
	// 여기가 문제인듯. 
	pServiceHeadOffice->ReqService(_ULS(gps), D_GPS_AMR_FIGURE_IMG, (void*)&strPath);

	// 아래 PrintModelView함수에서 EMF파일 생성이 완료 될 때까지 기다리고 있는데, 내부에서 sleep 함수를 사용하고 있음??? Why???
	PrintCellData(pExcel, _T("Struct_Anal_Shape_Img"), PrintModelView(pExcel, strModelViewFileName, stRow), Row_Ref, Col_Ref, stRow); //이미지 출력

	stRow += rowCnt;


	return TRUE;

}

BOOL CBrdgSeisEvalKISTEC2015_Report::GenerateStructAnalPCSupportInfo(IExcel* pExcel, int& stRow, int iNumEVGP, const int& iMaxLink, const CArray<T_NODE_K, T_NODE_K>& aLink,
	const UINT& EvgpK, const UINT& iColm, const CString& strEvgp)
{
	CString RptBlock;
	int rowCnt = 0;
	short Row_Ref = 0, Col_Ref = 0;

	CString sInsertCellLink_St;
	CString sInsertCellLink_Ed;

	if (iNumEVGP == 0)
	{
		RptBlock = _T("[A]PCSupport_Node_Head");
		rowCnt = CopyRangeSubTitle(pExcel, RptBlock, stRow);
		stRow += rowCnt;
	}

	int iIdxEvgp = iNumEVGP % 5;
	if (iIdxEvgp == 0)  // Make Table
	{
		stRow++;

		RptBlock = _T("[A]PCSupport_Node_Table_Head");
		rowCnt = CopyRangeSubTitle(pExcel, RptBlock, stRow);
		GetCellIDByName(pExcel, RptBlock, Row_Ref, Col_Ref);
		stRow += rowCnt;

		for (int j = 0; j < iMaxLink; ++j)
		{
			RptBlock = _T("[A]PCSupport_Node_Table_Body");
			rowCnt = CopyRangeSubTitle(pExcel, RptBlock, stRow);
			stRow += rowCnt;
		}

		RptBlock = _T("[A]PCSupport_Node_Table_End");
		rowCnt = CopyRangeSubTitle(pExcel, RptBlock, stRow);
		stRow += rowCnt;

		stRow -= (iMaxLink + 2);
	}
	else
	{
		RptBlock = _T("[A]PCSupport_Node_Table_Head");
		GetCellIDByName(pExcel, RptBlock, Row_Ref, Col_Ref);
		stRow -= (iMaxLink + 2);
	}

	RptBlock.Format(_T("[A]Name%d"), iIdxEvgp + 1);
	PrintCellData(pExcel, RptBlock, strEvgp, Row_Ref, Col_Ref, stRow);
	stRow++;

	RptBlock = _T("[A]PCSupport_Node_Table_Body");
	GetCellIDByName(pExcel, RptBlock, Row_Ref, Col_Ref);

	CMapEx<UINT, UINT, double, double&> mapERect, mapNRect;
	double dAxial = Calc_PierCapSuppEnvAxialForce(EvgpK, iColm, mapERect, mapNRect, TRUE);
	CArray<UINT, UINT> aERectK, aNRectK;
	int nELink = mapERect.GetKeyList(aERectK);
	if (nELink > 1) CArrayUtil::Sort(aERectK);
	int nNLink = mapNRect.GetKeyList(aNRectK);
	if (nNLink > 1) CArrayUtil::Sort(aNRectK);
	int nLink = nELink + nNLink;
	for (int k = 0; k < nLink; ++k)
	{
		double dFx = 0.0;
		if (k < nELink)
		{
			if (!mapERect.Lookup(aERectK[k], dFx)) dFx = 0.0;
		}
		else
		{
			if (!mapNRect.Lookup(aNRectK[k - nELink], dFx)) dFx = 0.0;
		}

		if (iIdxEvgp == 0) PrintCellData(pExcel, _T("[A]Node_Num"), k + 1, Row_Ref, Col_Ref, stRow);

		RptBlock.Format(_T("[A]Axial%d"), iIdxEvgp + 1);
		PrintCellData(pExcel, RptBlock, dFx, Row_Ref, Col_Ref, stRow);
		stRow++;
	}
	if (nLink != iMaxLink) stRow += (iMaxLink - nLink);

	RptBlock = _T("[A]PCSupport_Node_Table_End");
	GetCellIDByName(pExcel, RptBlock, Row_Ref, Col_Ref);

	RptBlock.Format(_T("[A]AxialSum%d"), iIdxEvgp + 1);
	PrintCellData(pExcel, RptBlock, dAxial, Row_Ref, Col_Ref, stRow);
	stRow++;



	return TRUE;
}

BOOL CBrdgSeisEvalKISTEC2015_Report::GenerateStructAnalPCBotInfo(IExcel* pExcel, int& stRow, T_BRDGEVAL_RESULT ResultD[2][2], int iNumEVGP, int& nPrtTitle, const CString& strEvgp)
{
	CString RptBlock;
	int rowCnt = 0;
	short Row_Ref = 0, Col_Ref = 0;

	int iMergeRow = 0;
	int iNumEVGPLoop = 0;

	if (iNumEVGP == 0)
	{
		if (nPrtTitle > 0)
		{
			RptBlock = _T("[A]PCBot_Head");
			rowCnt = CopyRangeSubTitle(pExcel, RptBlock, stRow);
			stRow += rowCnt;
		}
		else if (nPrtTitle == 0)
		{
			RptBlock = _T("[A]PCBot_Head_No_Link");
			rowCnt = CopyRangeSubTitle(pExcel, RptBlock, stRow);
			stRow += rowCnt;
		}
		nPrtTitle = -1;
	}

	int iIdxEvgp = iNumEVGP % 5;

	RptBlock = _T("[A]PCBot_Table");
	if (iIdxEvgp == 0)
	{
		stRow++;
		rowCnt = CopyRangeSubTitle(pExcel, RptBlock, stRow);
	}
	else
	{
		stRow -= 2;
	}
	GetCellIDByName(pExcel, RptBlock, Row_Ref, Col_Ref);

	RptBlock.Format(_T("[A]NameBot%d"), iIdxEvgp + 1);
	PrintCellData(pExcel, RptBlock, strEvgp, Row_Ref, Col_Ref, stRow);
	RptBlock.Format(_T("[A]AxialBot%d"), iIdxEvgp + 1);
	PrintCellData(pExcel, RptBlock, ResultD[0][0].ParamD.ForceD.dPcolbot, Row_Ref, Col_Ref, stRow);
	stRow += 2;

	return TRUE;
}


BOOL CBrdgSeisEvalKISTEC2015_Report::GenerateStructInfo(IExcel* pExcel, CString& RptBlock, int& stRow, const EN_BEHAVIOR_TYPE* enBehavior, const double* dB, const double* dH,
	T_BRDGEVAL_RESULT ResultD[2][2], T_MPCC_D* pMpccD, const T_RCHK_D& RchkData, const T_REBT_D& GenRbarD, const CString& strName, bool bPlainConc)
{
	stRow = 1;

	if (enBehavior[0] == CANTILEVER)
	{
		RptBlock = _T("[1]Title_Check_Req_Cantil");
	}
	else
	{
		RptBlock = _T("[1]Title_Check_Req_Bot_Rahmen");
	}

	int rowCnt = CopyRange(pExcel, RptBlock, stRow);

	stRow += rowCnt;

	RptBlock = _T("[1]Struct_Info");

	rowCnt = CopyRangeSubTitle(pExcel, RptBlock, stRow);

	short Row_Ref = 0, Col_Ref = 0;
	GetCellIDByName(pExcel, RptBlock, Row_Ref, Col_Ref);

	PrintCellData(pExcel, _T("Name"), strName, Row_Ref, Col_Ref, stRow); //주각이름

	CString strSectFigFileName;
	strSectFigFileName.Format(_T("%s_%s_Sect.emf"), m_strPicPath, pMpccD->strName);
	Make_SectionFig(strSectFigFileName, pMpccD);
	m_pDoc->m_pMatlDB->Create_RebarData(m_CodeUnit.nBase_Length, m_CodeUnit.nBase_Force, TRUE);

	PrintCellData(pExcel, _T("[1]Sect_Image"), PrintSectionFig(pExcel, m_strPicPath, strSectFigFileName, stRow), Row_Ref, Col_Ref, stRow); //이미지 출력

	T_BRDGEVAL_RESULT PrtResultD;
	if (ResultD[0][0].EvalD.bCheck) PrtResultD = ResultD[0][0]; // 하부 교축
	else if (ResultD[1][0].EvalD.bCheck) PrtResultD = ResultD[1][0]; // 상부 교축
	else if (ResultD[0][1].EvalD.bCheck) PrtResultD = ResultD[0][1]; // 하부 교축직각
	else if (ResultD[1][1].EvalD.bCheck) PrtResultD = ResultD[1][1]; // 상부 교축직각

	int iIdx = PrtResultD.ParamD.OptD.bIend ? 0 : 2;

	if (PrtResultD.ParamD.SetcD.strShape == _T("P") || PrtResultD.ParamD.SetcD.strShape == _T("SR"))
	{
		PrintCellData(pExcel, _T("Shape"), _LS(IDS_DGN_SEIS_SHAPE_CIR), Row_Ref, Col_Ref, stRow);
		PrintCellData(pExcel, _T("Dia"), dH[0], Row_Ref, Col_Ref, stRow);
	}
	else if (PrtResultD.ParamD.SetcD.strShape == _T("B") ||
		PrtResultD.ParamD.SetcD.strShape == _T("SB") ||
		PrtResultD.ParamD.SetcD.strShape == _T("OCT") ||
		PrtResultD.ParamD.SetcD.strShape == _T("SOCT"))
	{
		PrintCellData(pExcel, _T("Shape"), _LS(IDS_DGN_SEIS_SHAPE_BOX), Row_Ref, Col_Ref, stRow);
		PrintCellData(pExcel, _T("Width"), dB[0], Row_Ref, Col_Ref, stRow);
		PrintCellData(pExcel, _T("Height"), dH[0], Row_Ref, Col_Ref, stRow);
	}
	else if (PrtResultD.ParamD.SetcD.strShape == _T("TRK") || PrtResultD.ParamD.SetcD.strShape == _T("STRK"))
	{
		PrintCellData(pExcel, _T("Shape"), _LS(IDS_DGN_SEIS_SHAPE_TRACK), Row_Ref, Col_Ref, stRow);
		PrintCellData(pExcel, _T("Width_Max"), dB[0], Row_Ref, Col_Ref, stRow);
		PrintCellData(pExcel, _T("Height"), dH[0], Row_Ref, Col_Ref, stRow);
	}
	else if (PrtResultD.ParamD.SetcD.strShape == _T("HTRK"))
	{
		PrintCellData(pExcel, _T("Shape"), _LS(IDS_DGN_SEIS_SHAPE_HTRACK), Row_Ref, Col_Ref, stRow);
		PrintCellData(pExcel, _T("Width_Max"), dB[0], Row_Ref, Col_Ref, stRow);
		PrintCellData(pExcel, _T("Height"), dH[0], Row_Ref, Col_Ref, stRow);
	}
	else if (PrtResultD.ParamD.SetcD.strShape == _T("GEN"))
	{
		PrintCellData(pExcel, _T("Shape"), _LS(IDS_DGN_SEIS_SHAPE_GEN), Row_Ref, Col_Ref, stRow);
		PrintCellData(pExcel, _T("Width"), dB[0], Row_Ref, Col_Ref, stRow);
		PrintCellData(pExcel, _T("Height"), dH[0], Row_Ref, Col_Ref, stRow);
	}
	else
	{
		ASSERT(0);
	}

	PrintCellData(pExcel, _T("Area"), PrtResultD.ParamD.SetcD.dArea, Row_Ref, Col_Ref, stRow);
	PrintCellData(pExcel, _T("Dc"), PrtResultD.ParamD.BarD.ddc, Row_Ref, Col_Ref, stRow);
	PrintCellData(pExcel, _T("Sy"), PrtResultD.ParamD.SetcD.dSy, Row_Ref, Col_Ref, stRow);
	PrintCellData(pExcel, _T("Sz"), PrtResultD.ParamD.SetcD.dSz, Row_Ref, Col_Ref, stRow);

	if(bPlainConc)
	{
		stRow += rowCnt;
		return TRUE;
	}

	PrintCellData(pExcel, _T("BDiaL"), 1 / m_dFactorLength * PrtResultD.ParamD.BarD.ddbl, Row_Ref, Col_Ref, stRow);

	if (PrtResultD.ParamD.SetcD.strShape == _T("GEN"))
	{
		int iIdxRbar = (PrtResultD.ParamD.OptD.bIend || GenRbarD.bIJBothMain) ? 0 : 2;
		T_REBT_MABR Mbar = GenRbarD.arMainRebar[iIdxRbar][0];
		PrintCellData(pExcel, _T("BNameL"), Mbar.strRebarName, Row_Ref, Col_Ref, stRow);
		PrintCellData(pExcel, _T("BAreaL"), m_pDoc->m_pMatlDB->Get_RebarArea(Mbar.strRebarName), Row_Ref, Col_Ref, stRow);
		PrintCellData(pExcel, _T("BRatioL"), (m_dFactorLength * m_dFactorLength) *  (m_pDoc->m_pMatlDB->Get_RebarArea(Mbar.strRebarName) * GenRbarD.arMainRebar[iIdxRbar].GetSize()) / PrtResultD.ParamD.SetcD.dArea, Row_Ref, Col_Ref, stRow);
	}
	else
	{
		PrintCellData(pExcel, _T("BNameL"), RchkData.COLM.strBarNa1[iIdx][0][0], Row_Ref, Col_Ref, stRow);
		PrintCellData(pExcel, _T("BAreaL"), m_pDoc->m_pMatlDB->Get_RebarArea(RchkData.COLM.strBarNa1[iIdx][0][0]), Row_Ref, Col_Ref, stRow);
		
		int iRbarIdx = PrtResultD.ParamD.OptD.bIend ? 0 : 2;
		double dAst = 0.0;
		for(int i = 0; i < 3; ++i)
        {
            for(int j = 0; j < T_RCHK_COLM_LAYER; ++j)
            {
                if(RchkData.COLM.iBarNum[iRbarIdx][i][j] == 0) continue;
                double dAs = m_pDoc->m_pMatlDB->Get_RebarArea(RchkData.COLM.strBarNa1[iRbarIdx][i][j]) * (m_dFactorLength * m_dFactorLength);
                if(!RchkData.COLM.strBarNa2[iRbarIdx][i][j].IsEmpty())
                {
                    dAs += (m_pDoc->m_pMatlDB->Get_RebarArea(RchkData.COLM.strBarNa2[iRbarIdx][i][j]) * (m_dFactorLength * m_dFactorLength));
                    dAs /= 2.0;
                }
                dAst += (dAs * 2.0 * RchkData.COLM.iBarNum[iRbarIdx][i][j]);
            }
        }
		const double drho = dAst / PrtResultD.ParamD.SetcD.dArea;
		PrintCellData(pExcel, _T("BRatioL"), drho, Row_Ref, Col_Ref, stRow);
	}

	PrintCellData(pExcel, _T("BDiaT"), 1 / m_dFactorLength * PrtResultD.ParamD.BarD.ddbs, Row_Ref, Col_Ref, stRow);

	if (PrtResultD.ParamD.SetcD.strShape == _T("GEN"))
	{
		int iIdxSbar = (PrtResultD.ParamD.OptD.bIend || GenRbarD.bIJBothShear) ? 0 : 2;
		T_REBT_SHBR SBar = GenRbarD.ShearRebar[iIdxSbar];

		PrintCellData(pExcel, _T("BNameT"), SBar.strSubRebarName, Row_Ref, Col_Ref, stRow);
		PrintCellData(pExcel, _T("BAreaT"), m_pDoc->m_pMatlDB->Get_RebarArea(SBar.strSubRebarName), Row_Ref, Col_Ref, stRow);
	}
	else
	{
		PrintCellData(pExcel, _T("BNameT"), RchkData.COLM.strSubBarNa[iIdx], Row_Ref, Col_Ref, stRow);
		PrintCellData(pExcel, _T("BAreaT"), m_pDoc->m_pMatlDB->Get_RebarArea(RchkData.COLM.strSubBarNa[iIdx]), Row_Ref, Col_Ref, stRow);
	}

	if (PrtResultD.ParamD.OptD.nSplice == 2)
	{
		PrintCellData(pExcel, _T("BRatioT"), PrtResultD.ParamD.BarD.drhosh, Row_Ref, Col_Ref, stRow);
	}
	else
	{
		PrintCellData(pExcel, _T("BRatioT"), _T("-"), Row_Ref, Col_Ref, stRow);
	}

	PrintCellData(pExcel, _T("BDisT"), PrtResultD.ParamD.BarD.ds, Row_Ref, Col_Ref, stRow);

	stRow += rowCnt;

	return TRUE;

}

BOOL CBrdgSeisEvalKISTEC2015_Report::GenerateRebarBaseShapeInfo(IExcel* pExcel, CString& RptBlock, int& stRow, const double* dB, const double* dH, T_BRDGEVAL_RESULT ResultD[2][2], const EN_PIER_TYPE* enPierType)
{
	short Row_Ref = 0, Col_Ref = 0;
	RptBlock = _T("[1]Rebar_Base_Shape_Chart");
	int rowCnt = CopyRangeSubTitle(pExcel, RptBlock, stRow + 1);
	//GetCellIDByName(pExcel, RptBlock, Row_Ref, Col_Ref);
	stRow += rowCnt + 1;

	T_BRDGEVAL_RESULT& PrtResD = ResultD[0][0].EvalD.bCheck ? ResultD[0][0] : ResultD[0][1];
	int iIdx = ResultD[0][0].EvalD.bCheck ? 0 : 1;

	double dBHratio = 0.0;
	if (PrtResD.ParamD.OptD.nSplice == 2)
	{
		if (PrtResD.ParamD.SetcD.strShape == _T("STRK") ||
			PrtResD.ParamD.SetcD.strShape == _T("HTRK"))
		{
			if (PrtResD.ParamD.OptD.nBHmethod == 0)
			{
				dBHratio = dB[iIdx] / dH[iIdx];

				RptBlock = _T("[1]Rebar_Base_General_BS_Ratio");
				rowCnt = CopyRangeSubTitle(pExcel, RptBlock, stRow);
				GetCellIDByName(pExcel, RptBlock, Row_Ref, Col_Ref);
				PrintCellData(pExcel, _T("Rebar_Base_General_Width2"), dB[iIdx], Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("Rebar_Base_General_Height2"), dH[iIdx], Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("Rebar_Base_General_BHRatio"), dBHratio, Row_Ref, Col_Ref, stRow);
				stRow += rowCnt + 1;
			}
			else
			{
				dBHratio = ((PI / 4 - 1) * dH[iIdx] + dB[iIdx]) / dH[iIdx];

				RptBlock = _T("[1]Rebar_Base_BS_Equation");
				rowCnt = CopyRangeSubTitle(pExcel, RptBlock, stRow);
				GetCellIDByName(pExcel, RptBlock, Row_Ref, Col_Ref);
				PrintCellData(pExcel, _T("Rebar_Base_Height1"), dH[iIdx], Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("Rebar_Base_Width_Max"), dB[iIdx], Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("Rebar_Base_Width1"), (PI / 4 - 1) * dH[iIdx] + dB[iIdx], Row_Ref, Col_Ref, stRow);
				stRow += rowCnt + 1;
				RptBlock = _T("[1]Rebar_Base_Recommend_BS_Ratio");
				rowCnt = CopyRangeSubTitle(pExcel, RptBlock, stRow);
				GetCellIDByName(pExcel, RptBlock, Row_Ref, Col_Ref);
				PrintCellData(pExcel, _T("Rebar_Base_Width2"), (PI / 4 - 1) * dH[iIdx] + dB[iIdx], Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("Rebar_Base_Height2"), dH[iIdx], Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("Rebar_Base_BHRatio"), dBHratio, Row_Ref, Col_Ref, stRow);
				stRow += rowCnt + 1;
			}
			if (dBHratio <= 1.5) RptBlock = _T("[1]Rebar_Base_Shape_General");
			else if (dBHratio < 4)   RptBlock = _T("[1]Rebar_Base_Shape_Transient");
			else                     RptBlock = _T("[1]Rebar_Base_Shape_Wall");

			//             if			(enPierType[iIdx] == WALL) 			RptBlock = _T("[1]Rebar_Base_Shape_Wall");
			//             else if (enPierType[iIdx] == TRANSIENT)	RptBlock = _T("[1]Rebar_Base_Shape_Transient");
			//             else if (enPierType[iIdx] == GENERAL)		RptBlock = _T("[1]Rebar_Base_Shape_General");

			rowCnt = CopyRangeSubTitle(pExcel, RptBlock, stRow);
			//GetCellIDByName(pExcel, RptBlock, Row_Ref, Col_Ref);
			stRow += rowCnt + 1;
		}
		else
		{
			if (PrtResD.ParamD.OptD.nBHmethod == 0)
			{
				dBHratio = dB[iIdx] / dH[iIdx];

				RptBlock = _T("[1]Rebar_Base_General_BS_Ratio");
				rowCnt = CopyRangeSubTitle(pExcel, RptBlock, stRow);
				GetCellIDByName(pExcel, RptBlock, Row_Ref, Col_Ref);
				PrintCellData(pExcel, _T("Rebar_Base_General_Width2"), dB[iIdx], Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("Rebar_Base_General_Height2"), dH[iIdx], Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("Rebar_Base_General_BHRatio"), dBHratio, Row_Ref, Col_Ref, stRow);
				stRow += rowCnt + 1;
			}
			else
			{
				dBHratio = PrtResD.ParamD.SetcD.dArea / dH[iIdx] / dH[iIdx];

				RptBlock = _T("[1]Rebar_Base_Else_BS_Equation");
				rowCnt = CopyRangeSubTitle(pExcel, RptBlock, stRow);
				GetCellIDByName(pExcel, RptBlock, Row_Ref, Col_Ref);
				PrintCellData(pExcel, _T("Rebar_Base_Else_Height2"), dH[iIdx], Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("Rebar_Base_Else_Ag"), PrtResD.ParamD.SetcD.dArea, Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("Rebar_Base_Else_Width2"), PrtResD.ParamD.SetcD.dArea / dH[iIdx], Row_Ref, Col_Ref, stRow);
				stRow += rowCnt + 1;
				RptBlock = _T("[1]Rebar_Base_Recommend_BS_Ratio");
				rowCnt = CopyRangeSubTitle(pExcel, RptBlock, stRow);
				GetCellIDByName(pExcel, RptBlock, Row_Ref, Col_Ref);
				PrintCellData(pExcel, _T("Rebar_Base_Width2"), PrtResD.ParamD.SetcD.dArea / dH[iIdx], Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("Rebar_Base_Height2"), dH[iIdx], Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("Rebar_Base_BHRatio"), dBHratio, Row_Ref, Col_Ref, stRow);
				stRow += rowCnt + 1;
			}

			if (dBHratio <= 1.5)     RptBlock = _T("[1]Rebar_Base_Shape_General");
			else if (dBHratio < 4)   RptBlock = _T("[1]Rebar_Base_Shape_Transient");
			else                     RptBlock = _T("[1]Rebar_Base_Shape_Wall");

			rowCnt = CopyRangeSubTitle(pExcel, RptBlock, stRow);
			//GetCellIDByName(pExcel, RptBlock, Row_Ref, Col_Ref);
			stRow += rowCnt + 1;
		}
	}

	return TRUE;

}

BOOL CBrdgSeisEvalKISTEC2015_Report::GenerateRebarBaseShapeInfoTop(IExcel* pExcel, CString& RptBlock, int& stRow, const double* dB, const double* dH, T_BRDGEVAL_RESULT ResultD[2][2], const EN_PIER_TYPE* enPierType)
{
	short Row_Ref = 0, Col_Ref = 0;
	RptBlock = _T("[1]Rebar_Base_Shape_Chart");
	int rowCnt = CopyRangeSubTitle(pExcel, RptBlock, stRow + 1);
	//GetCellIDByName(pExcel, RptBlock, Row_Ref, Col_Ref);
	stRow += rowCnt + 1;

	T_BRDGEVAL_RESULT& PrtResD = ResultD[1][0].EvalD.bCheck ? ResultD[1][0] : ResultD[1][1];
	int iIdx = ResultD[1][0].EvalD.bCheck ? 2 : 3;

	double dBHratio = 0.0;
	if (PrtResD.ParamD.OptD.nSplice == 2)
	{
		if (PrtResD.ParamD.SetcD.strShape == _T("STRK") ||
			PrtResD.ParamD.SetcD.strShape == _T("HTRK"))
		{
			if (PrtResD.ParamD.OptD.nBHmethod == 0)
			{
				dBHratio = dB[iIdx] / dH[iIdx];
				RptBlock = _T("[1]Rebar_Base_General_BS_Ratio");
				rowCnt = CopyRangeSubTitle(pExcel, RptBlock, stRow);
				GetCellIDByName(pExcel, RptBlock, Row_Ref, Col_Ref);
				PrintCellData(pExcel, _T("Rebar_Base_General_Width2"), dB[iIdx], Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("Rebar_Base_General_Height2"), dH[iIdx], Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("Rebar_Base_General_BHRatio"), dBHratio, Row_Ref, Col_Ref, stRow);
				stRow += rowCnt + 1;
			}
			else
			{
				dBHratio = ((PI / 4 - 1) * dH[iIdx] + dB[iIdx]) / dH[iIdx];
				RptBlock = _T("[1]Rebar_Base_BS_Equation");
				rowCnt = CopyRangeSubTitle(pExcel, RptBlock, stRow);
				GetCellIDByName(pExcel, RptBlock, Row_Ref, Col_Ref);
				PrintCellData(pExcel, _T("Rebar_Base_Height1"), dH[iIdx], Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("Rebar_Base_Width_Max"), dB[iIdx], Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("Rebar_Base_Width1"), (PI / 4 - 1) * dH[iIdx] + dB[iIdx], Row_Ref, Col_Ref, stRow);
				stRow += rowCnt + 1;
				RptBlock = _T("[1]Rebar_Base_Recommend_BS_Ratio");
				rowCnt = CopyRangeSubTitle(pExcel, RptBlock, stRow);
				GetCellIDByName(pExcel, RptBlock, Row_Ref, Col_Ref);
				PrintCellData(pExcel, _T("Rebar_Base_Width2"), (PI / 4 - 1) * dH[iIdx] + dB[iIdx], Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("Rebar_Base_Height2"), dH[0], Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("Rebar_Base_BHRatio"), dBHratio, Row_Ref, Col_Ref, stRow);
				stRow += rowCnt + 1;
			}

			if (dBHratio <= 1.5) RptBlock = _T("[1]Rebar_Base_Shape_General");
			else if (dBHratio < 4)    RptBlock = _T("[1]Rebar_Base_Shape_Transient");
			else                     RptBlock = _T("[1]Rebar_Base_Shape_Wall");

			rowCnt = CopyRangeSubTitle(pExcel, RptBlock, stRow);
			//GetCellIDByName(pExcel, RptBlock, Row_Ref, Col_Ref);
			stRow += rowCnt + 1;
		}
		else
		{
			if (PrtResD.ParamD.OptD.nBHmethod == 0)
			{
				dBHratio = dB[iIdx] / dH[iIdx];
				RptBlock = _T("[1]Rebar_Base_General_BS_Ratio");
				rowCnt = CopyRangeSubTitle(pExcel, RptBlock, stRow);
				GetCellIDByName(pExcel, RptBlock, Row_Ref, Col_Ref);
				PrintCellData(pExcel, _T("Rebar_Base_General_Width2"), dB[iIdx], Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("Rebar_Base_General_Height2"), dH[iIdx], Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("Rebar_Base_General_BHRatio"), dBHratio, Row_Ref, Col_Ref, stRow);
				stRow += rowCnt + 1;
			}
			else
			{
				dBHratio = PrtResD.ParamD.SetcD.dArea / dH[iIdx] / dH[iIdx];
				RptBlock = _T("[1]Rebar_Base_Else_BS_Equation");
				rowCnt = CopyRangeSubTitle(pExcel, RptBlock, stRow);
				GetCellIDByName(pExcel, RptBlock, Row_Ref, Col_Ref);
				PrintCellData(pExcel, _T("Rebar_Base_Else_Height2"), dH[iIdx], Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("Rebar_Base_Else_Ag"), PrtResD.ParamD.SetcD.dArea, Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("Rebar_Base_Else_Width2"), PrtResD.ParamD.SetcD.dArea / dH[iIdx], Row_Ref, Col_Ref, stRow);
				stRow += rowCnt + 1;
				RptBlock = _T("[1]Rebar_Base_Recommend_BS_Ratio");
				rowCnt = CopyRangeSubTitle(pExcel, RptBlock, stRow);
				GetCellIDByName(pExcel, RptBlock, Row_Ref, Col_Ref);
				PrintCellData(pExcel, _T("Rebar_Base_Width2"), PrtResD.ParamD.SetcD.dArea / dH[iIdx], Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("Rebar_Base_Height2"), dH[iIdx], Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("Rebar_Base_BHRatio"), dBHratio, Row_Ref, Col_Ref, stRow);
				stRow += rowCnt + 1;
			}

			if (dBHratio <= 1.5) RptBlock = _T("[1]Rebar_Base_Shape_General");
			else if (dBHratio < 4)    RptBlock = _T("[1]Rebar_Base_Shape_Transient");
			else                      RptBlock = _T("[1]Rebar_Base_Shape_Wall");

			rowCnt = CopyRangeSubTitle(pExcel, RptBlock, stRow);
			//GetCellIDByName(pExcel, RptBlock, Row_Ref, Col_Ref);
			stRow += rowCnt + 1;
		}
	}

	return TRUE;

}


BOOL CBrdgSeisEvalKISTEC2015_Report::PrintSectionFig(IExcel* pExcel, CString& strPicPath, CString& strSectFigFileName, int& stRow)
{

	int nH = 11;
	int nW = 13;

	CString strPath = strSectFigFileName;

	if (WaitForFile(strPath))
	{
		pExcel->AddPicture_Count(stRow + 8, 3, nH, nW, strPath);
	}

	return TRUE;
}


BOOL CBrdgSeisEvalKISTEC2015_Report::GenerateMaterialModelInfo(IExcel* pExcel, CString& RptBlock, int& stRow, const T_FIMP_D& FimpConcD1, const T_FIMP_D& FimpConc2D1, const T_FIMP_D& FimpConcD3, const T_FIMP_D& FimpConc2D3, const T_FIMP_D& FimpSteelD, T_BRDGEVAL_RESULT ResultD[2][2], T_MPCC_D* pMpccD)
{
	short Row_Ref = 0, Col_Ref = 0;
	RptBlock = _T("[1]Material_Model_Head");
	int rowCnt = CopyRangeSubTitle(pExcel, RptBlock, stRow + 1);
	//GetCellIDByName(pExcel, RptBlock, Row_Ref, Col_Ref);
	stRow += rowCnt;

	RptBlock = _T("[1]Material_Model_Conc");
	rowCnt = CopyRangeSubTitle(pExcel, RptBlock, stRow + 1);
	GetCellIDByName(pExcel, RptBlock, Row_Ref, Col_Ref);
	stRow = stRow + 1;

	T_ECUC_D EcucD;
	if (!m_pDoc->m_pAttrCtrl2->GetEcuc(EcucD)) EcucD.Initialize();

	if (FimpConcD1.nMaterialType == 0)
	{
		if (FimpConcD1.nHysModelType == 0)      PrintCellData(pExcel, _T("ConModelName"), _LS(IDS_CMD2_FIMP_KENT_PARK), Row_Ref, Col_Ref, stRow);
		else if (FimpConcD1.nHysModelType == 1) PrintCellData(pExcel, _T("ConModelName"), _LS(IDS_CMD2_FIMP_JCONCRETE), Row_Ref, Col_Ref, stRow);
		else if (FimpConcD1.nHysModelType == 2) PrintCellData(pExcel, _T("ConModelName"), _LS(IDS_CMD2_FIMP_JBRIDGE), Row_Ref, Col_Ref, stRow);
		else if (FimpConcD1.nHysModelType == 3) PrintCellData(pExcel, _T("ConModelName"), _LS(IDS_CMD2_FIMP_NAKOYA), Row_Ref, Col_Ref, stRow);
		else if (FimpConcD1.nHysModelType == 4) PrintCellData(pExcel, _T("ConModelName"), _LS(IDS_CMD2_FIMP_CON_TRILIN), Row_Ref, Col_Ref, stRow);
		else if (FimpConcD1.nHysModelType == 5) PrintCellData(pExcel, _T("ConModelName"), _LS(IDS_CMD2_CHINA_GB50010_02), Row_Ref, Col_Ref, stRow);
		else if (FimpConcD1.nHysModelType == 6)
		{
			PrintCellData(pExcel, _T("ConModelName"), _LS(IDS_CMD2_FIMP_MANDER), Row_Ref, Col_Ref, stRow);
			stRow = stRow + 1;
			RptBlock = _T("[1]Material_Model_Conc_Table");
			rowCnt = CopyRangeSubTitle(pExcel, RptBlock, stRow + 1);
			GetCellIDByName(pExcel, RptBlock, Row_Ref, Col_Ref);
			stRow = stRow + 1;
			PrintCellData(pExcel, _T("ConFCK"), m_dFactorStress * ResultD[0][0].ParamD.MatlD.dfck, Row_Ref, Col_Ref, stRow);

			PrintCellData(pExcel, _T("ConFCO"), m_dFactorStress * FimpConcD1.PROP.CON_MANDER.dConcDatafco, Row_Ref, Col_Ref, stRow);
			if (FimpConc2D1.nHysModelType == 6)
				PrintCellData(pExcel, _T("ConFCC"), m_dFactorStress * FimpConc2D1.PROP.CON_MANDER.dConfinedConcStrengthfcc, Row_Ref, Col_Ref, stRow);

			PrintCellData(pExcel, _T("ConECO"), FimpConcD1.PROP.CON_MANDER.dConcDataeco, Row_Ref, Col_Ref, stRow);
			if (FimpConc2D1.nHysModelType == 6)
			{
				PrintCellData(pExcel, _T("ConECC"), FimpConc2D1.PROP.CON_MANDER.dConfinedConcStrainecc, Row_Ref, Col_Ref, stRow);
				// MQC.26527 Mander 모델 극한변형률 출력 수정
				// CIVIL-806 수정
				if (EcucD.becu && EcucD.necuType == 1)
					PrintCellData(pExcel, _T("ConECU"), EcucD.decu_User, Row_Ref, Col_Ref, stRow);
				else if (FimpConc2D1.PROP.CON_MANDER.bUseUltStrUlt)
					PrintCellData(pExcel, _T("ConECU"), FimpConc2D1.PROP.CON_MANDER.dConfinedConcStrUltEcu, Row_Ref, Col_Ref, stRow);
				else
					PrintCellData(pExcel, _T("ConECU"), FimpConc2D1.PROP.CON_MANDER.dConfinedConcStrainecc, Row_Ref, Col_Ref, stRow);
			}
			else
				PrintCellData(pExcel, _T("ConECU"), FimpConcD1.PROP.CON_MANDER.dConcDataesp, Row_Ref, Col_Ref, stRow);

			PrintCellData(pExcel, _T("ConEC"), m_dFactorStress * FimpConcD1.PROP.CON_MANDER.dConcDataEc, Row_Ref, Col_Ref, stRow);
			stRow = stRow + 2;
		}
		else if (FimpConcD1.nHysModelType == 7) PrintCellData(pExcel, _T("ConModelName"), _LS(IDS_CMD2_FIMP_JBRIDGE_H24), Row_Ref, Col_Ref, stRow);
		else if (FimpConcD1.nHysModelType == 8) PrintCellData(pExcel, _T("ConModelName"), _LS(IDS_CMD2_CHINA_GB50010_10), Row_Ref, Col_Ref, stRow);
		stRow = stRow + 2;
		PrintConcreteGraph(pExcel, FimpConcD1.nHysModelType, stRow);
		RptBlock = _T("[1]Material_Model_Conc_Model_IDX");
		rowCnt = CopyRangeSubTitle(pExcel, RptBlock, stRow + 1);
		stRow = stRow + rowCnt;
	}

	{
		RptBlock = _T("[1]Material_Model_Trans_Conc");
		rowCnt = CopyRangeSubTitle(pExcel, RptBlock, stRow + 1);
		GetCellIDByName(pExcel, RptBlock, Row_Ref, Col_Ref);
		stRow = stRow + 1;
		if (FimpConcD3.nHysModelType == 0)      PrintCellData(pExcel, _T("TransConModelName"), _LS(IDS_CMD2_FIMP_KENT_PARK), Row_Ref, Col_Ref, stRow);
		else if (FimpConcD3.nHysModelType == 1) PrintCellData(pExcel, _T("TransConModelName"), _LS(IDS_CMD2_FIMP_JCONCRETE), Row_Ref, Col_Ref, stRow);
		else if (FimpConcD3.nHysModelType == 2) PrintCellData(pExcel, _T("TransConModelName"), _LS(IDS_CMD2_FIMP_JBRIDGE), Row_Ref, Col_Ref, stRow);
		else if (FimpConcD3.nHysModelType == 3) PrintCellData(pExcel, _T("TransConModelName"), _LS(IDS_CMD2_FIMP_NAKOYA), Row_Ref, Col_Ref, stRow);
		else if (FimpConcD3.nHysModelType == 4) PrintCellData(pExcel, _T("TransConModelName"), _LS(IDS_CMD2_FIMP_CON_TRILIN), Row_Ref, Col_Ref, stRow);
		else if (FimpConcD3.nHysModelType == 5) PrintCellData(pExcel, _T("TransConModelName"), _LS(IDS_CMD2_CHINA_GB50010_02), Row_Ref, Col_Ref, stRow);
		else if (FimpConcD3.nHysModelType == 6)
		{
			PrintCellData(pExcel, _T("TransConModelName"), _LS(IDS_CMD2_FIMP_MANDER), Row_Ref, Col_Ref, stRow);
			stRow = stRow + 1;
			RptBlock = _T("[1]Material_Model_Trans_Conc_Table");
			rowCnt = CopyRangeSubTitle(pExcel, RptBlock, stRow + 1);
			GetCellIDByName(pExcel, RptBlock, Row_Ref, Col_Ref);
			stRow = stRow + 1;
			PrintCellData(pExcel, _T("TransConFCK"), m_dFactorStress * ResultD[0][0].ParamD.MatlD.dfck, Row_Ref, Col_Ref, stRow);

			PrintCellData(pExcel, _T("TransConFCO"), m_dFactorStress * FimpConcD3.PROP.CON_MANDER.dConcDatafco, Row_Ref, Col_Ref, stRow);
			if (FimpConc2D3.nHysModelType == 6)
				PrintCellData(pExcel, _T("TransConFCC"), m_dFactorStress * FimpConc2D3.PROP.CON_MANDER.dConfinedConcStrengthfcc, Row_Ref, Col_Ref, stRow);

			PrintCellData(pExcel, _T("TransConECO"), FimpConcD3.PROP.CON_MANDER.dConcDataeco, Row_Ref, Col_Ref, stRow);

			if (FimpConc2D3.nHysModelType == 6)
			{
				PrintCellData(pExcel, _T("TransConECC"), FimpConc2D3.PROP.CON_MANDER.dConfinedConcStrainecc, Row_Ref, Col_Ref, stRow);
				// MQC.26527 Mander 모델 극한변형률 출력 수정
				// CIVIL-806 수정
				if (EcucD.becu && EcucD.necuType == 1)
					PrintCellData(pExcel, _T("TransConECU"), EcucD.decu_User, Row_Ref, Col_Ref, stRow);
				else if (FimpConc2D3.PROP.CON_MANDER.bUseUltStrUlt)
					PrintCellData(pExcel, _T("TransConECU"), FimpConc2D3.PROP.CON_MANDER.dConfinedConcStrUltEcu, Row_Ref, Col_Ref, stRow);
				else
					PrintCellData(pExcel, _T("TransConECU"), FimpConc2D3.PROP.CON_MANDER.dConfinedConcStrainecc, Row_Ref, Col_Ref, stRow);
			}
			else
				PrintCellData(pExcel, _T("TransConECU"), FimpConcD3.PROP.CON_MANDER.dConcDataesp, Row_Ref, Col_Ref, stRow);

			PrintCellData(pExcel, _T("TransConEC"), m_dFactorStress * FimpConcD3.PROP.CON_MANDER.dConcDataEc, Row_Ref, Col_Ref, stRow);
			stRow = stRow + 2;
		}
		else if (FimpConcD3.nHysModelType == 7) PrintCellData(pExcel, _T("TransConModelName"), _LS(IDS_CMD2_FIMP_JBRIDGE_H24), Row_Ref, Col_Ref, stRow);
		else if (FimpConcD3.nHysModelType == 8) PrintCellData(pExcel, _T("TransConModelName"), _LS(IDS_CMD2_CHINA_GB50010_10), Row_Ref, Col_Ref, stRow);
		stRow = stRow + 2;
		PrintConcreteGraph(pExcel, FimpConcD3.nHysModelType, stRow);
		RptBlock = _T("[1]Material_Model_Trans_Conc_Model_IDX");
		rowCnt = CopyRangeSubTitle(pExcel, RptBlock, stRow + 1);
		stRow = stRow + rowCnt;
	}

	RptBlock = _T("[1]Material_Model_Steel");
	rowCnt = CopyRangeSubTitle(pExcel, RptBlock, stRow + 1);
	GetCellIDByName(pExcel, RptBlock, Row_Ref, Col_Ref);
	stRow = stRow + 1;

	if (FimpSteelD.nMaterialType == 1)
	{
		if (FimpSteelD.nHysModelType == 0) PrintCellData(pExcel, _T("SteelModelName"), _LS(IDS_CMD2_FIMP_MENEGOTTO), Row_Ref, Col_Ref, stRow);
		else if (FimpSteelD.nHysModelType == 1) PrintCellData(pExcel, _T("SteelModelName"), _LS(IDS_CMD2_FIMP_BILINEAR), Row_Ref, Col_Ref, stRow);
		else if (FimpSteelD.nHysModelType == 2) PrintCellData(pExcel, _T("SteelModelName"), _LS(IDS_CMD2_FIMP_GEN_BILINEAR), Row_Ref, Col_Ref, stRow);
		else if (FimpSteelD.nHysModelType == 3) PrintCellData(pExcel, _T("SteelModelName"), _LS(IDS_CMD2_FIMP_STL_TRILIN), Row_Ref, Col_Ref, stRow);
		else if (FimpSteelD.nHysModelType == 4)
		{
			PrintCellData(pExcel, _T("SteelModelName"), _LS(IDS_CMD2_FIMP_PARK_MODEL), Row_Ref, Col_Ref, stRow);
			stRow = stRow + 1;
			RptBlock = _T("[1]Material_Model_Steel_Table");
			rowCnt = CopyRangeSubTitle(pExcel, RptBlock, stRow + 1);
			GetCellIDByName(pExcel, RptBlock, Row_Ref, Col_Ref);
			stRow = stRow + 1;
			PrintCellData(pExcel, _T("SteelFY"), m_dFactorStress * FimpSteelD.PROP.STL_PARK.dFy, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("SteelFSU"), m_dFactorStress * FimpSteelD.PROP.STL_PARK.dFu, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("SteelEY"), FimpSteelD.PROP.STL_PARK.dFy / FimpSteelD.PROP.STL_PARK.dEs, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("SteelESH"), FimpSteelD.PROP.STL_PARK.dEsh, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("SteelESU"), FimpSteelD.PROP.STL_PARK.dEsu, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("SteelEC"), m_dFactorStress * FimpSteelD.PROP.STL_PARK.dEs, Row_Ref, Col_Ref, stRow);
			stRow = stRow + 2;
		}
		else if (FimpSteelD.nHysModelType == 5) PrintCellData(pExcel, _T("SteelModelName"), _LS(IDS_CMD2_FIMP_STL_JBRIDGE), Row_Ref, Col_Ref, stRow);
		else if (FimpSteelD.nHysModelType == 6) PrintCellData(pExcel, _T("SteelModelName"), _LS(IDS_CMD2_CHINA_STL_GB50010_10), Row_Ref, Col_Ref, stRow);
		stRow = stRow + 2;
		PrintSteelGraph(pExcel, FimpSteelD.nHysModelType, stRow);
		RptBlock = _T("[1]Material_Model_Steel_Model_IDX");
		rowCnt = CopyRangeSubTitle(pExcel, RptBlock, stRow + 1);
		stRow = stRow + rowCnt;
	}

	stRow = stRow + 1;

	return TRUE;

}

BOOL CBrdgSeisEvalKISTEC2015_Report::GenerateAssumptions(IExcel* pExcel, CString& RptBlock, int& stRow, const double* dT)
{
	short Row_Ref = 0, Col_Ref = 0;
	RptBlock = _T("[1]Assumptions_Head");
	int rowCnt = CopyRangeSubTitle(pExcel, RptBlock, stRow);
	stRow += rowCnt;
	RptBlock = _T("[1]Assumptions_Detail");
	rowCnt = CopyRangeSubTitle(pExcel, RptBlock, stRow);
	GetCellIDByName(pExcel, RptBlock, Row_Ref, Col_Ref);
	PrintCellData(pExcel, _T("PeriodGeo"), dT[0], Row_Ref, Col_Ref, stRow);
	stRow += rowCnt;

	return TRUE;
}

BOOL CBrdgSeisEvalKISTEC2015_Report::GenerateAssumptionsTop(IExcel* pExcel, CString& RptBlock, int& stRow, const double* dT)
{
	short Row_Ref = 0, Col_Ref = 0;
	RptBlock = _T("[1]Assumptions_Head");
	int rowCnt = CopyRangeSubTitle(pExcel, RptBlock, stRow);
	stRow += rowCnt;
	RptBlock = _T("[1]Assumptions_Detail");
	rowCnt = CopyRangeSubTitle(pExcel, RptBlock, stRow);
	GetCellIDByName(pExcel, RptBlock, Row_Ref, Col_Ref);
	PrintCellData(pExcel, _T("PeriodGeo"), dT[1], Row_Ref, Col_Ref, stRow);
	stRow += rowCnt;

	return TRUE;
}

BOOL CBrdgSeisEvalKISTEC2015_Report::GenerateStructInfoBot(IExcel* pExcel, CString& RptBlock, int& stRow, const EN_BEHAVIOR_TYPE* enBehavior, const double* dB, const double* dH,
	T_BRDGEVAL_RESULT ResultD[2][2], T_MPCC_D* pMpccD, const T_RCHK_D& RchkData, const T_REBT_D& GenRbarD, const CString& strName, bool bPlainConc)
{
	stRow = 1;

	T_BRDGEVAL_RESULT& PrtResD  = ResultD[0][0].EvalD.bCheck ? ResultD[0][0] : ResultD[0][1];
	int iIdx = ResultD[0][0].EvalD.bCheck ? 0 : 1;

	RptBlock = _T("[1]Title_Check_Req_Bot_Rahmen");
	int rowCnt = CopyRange(pExcel, RptBlock, stRow);
	stRow += rowCnt;

	RptBlock = _T("[1]Struct_Info");
	rowCnt = CopyRangeSubTitle(pExcel, RptBlock, stRow);

	short Row_Ref = 0, Col_Ref = 0;
	GetCellIDByName(pExcel, RptBlock, Row_Ref, Col_Ref);
	PrintCellData(pExcel, _T("Name"), strName, Row_Ref, Col_Ref, stRow); //주각이름

	CString strSectFigFileName;
	strSectFigFileName.Format(_T("%s_%s_Sect.emf"), m_strPicPath, pMpccD->strName);
	Make_SectionFig(strSectFigFileName, pMpccD);
	m_pDoc->m_pMatlDB->Create_RebarData(m_CodeUnit.nBase_Length, m_CodeUnit.nBase_Force, TRUE);

	PrintCellData(pExcel, _T("[1]Sect_Image"), PrintSectionFig(pExcel, m_strPicPath, strSectFigFileName, stRow), Row_Ref, Col_Ref, stRow); //이미지 출력

	if (PrtResD.ParamD.SetcD.strShape == _T("P") || PrtResD.ParamD.SetcD.strShape == _T("SR"))
	{
		PrintCellData(pExcel, _T("Shape"), _LS(IDS_DGN_SEIS_SHAPE_CIR), Row_Ref, Col_Ref, stRow);
		PrintCellData(pExcel, _T("Dia"), dH[iIdx], Row_Ref, Col_Ref, stRow);
	}
	else if (PrtResD.ParamD.SetcD.strShape == _T("B") ||
		PrtResD.ParamD.SetcD.strShape == _T("SB") ||
		PrtResD.ParamD.SetcD.strShape == _T("OCT") ||
		PrtResD.ParamD.SetcD.strShape == _T("SOCT"))
	{
		PrintCellData(pExcel, _T("Shape"), _LS(IDS_DGN_SEIS_SHAPE_BOX), Row_Ref, Col_Ref, stRow);
		PrintCellData(pExcel, _T("Width"), dB[iIdx], Row_Ref, Col_Ref, stRow);
		PrintCellData(pExcel, _T("Height"), dH[iIdx], Row_Ref, Col_Ref, stRow);
	}
	else if (PrtResD.ParamD.SetcD.strShape == _T("TRK") ||
		PrtResD.ParamD.SetcD.strShape == _T("STRK"))
	{
		PrintCellData(pExcel, _T("Shape"), _LS(IDS_DGN_SEIS_SHAPE_TRACK), Row_Ref, Col_Ref, stRow);
		PrintCellData(pExcel, _T("Width_Max"), dB[iIdx], Row_Ref, Col_Ref, stRow);
		PrintCellData(pExcel, _T("Height"), dH[iIdx], Row_Ref, Col_Ref, stRow);
	}
	else if (PrtResD.ParamD.SetcD.strShape == _T("HTRK"))
	{
		PrintCellData(pExcel, _T("Shape"), _LS(IDS_DGN_SEIS_SHAPE_HTRACK), Row_Ref, Col_Ref, stRow);
		PrintCellData(pExcel, _T("Width_Max"), dB[iIdx], Row_Ref, Col_Ref, stRow);
		PrintCellData(pExcel, _T("Height"), dH[iIdx], Row_Ref, Col_Ref, stRow);
	}
	else if (PrtResD.ParamD.SetcD.strShape == _T("GEN"))
	{
		PrintCellData(pExcel, _T("Shape"), _LS(IDS_DGN_SEIS_SHAPE_GEN), Row_Ref, Col_Ref, stRow);
		PrintCellData(pExcel, _T("Width"), dB[iIdx], Row_Ref, Col_Ref, stRow);
		PrintCellData(pExcel, _T("Height"), dH[iIdx], Row_Ref, Col_Ref, stRow);
	}
	else
	{
		ASSERT(0);
	}

	PrintCellData(pExcel, _T("Area"), PrtResD.ParamD.SetcD.dArea, Row_Ref, Col_Ref, stRow);
	PrintCellData(pExcel, _T("Dc"), PrtResD.ParamD.BarD.ddc, Row_Ref, Col_Ref, stRow);
	PrintCellData(pExcel, _T("Sy"), PrtResD.ParamD.SetcD.dSy, Row_Ref, Col_Ref, stRow);
	PrintCellData(pExcel, _T("Sz"), PrtResD.ParamD.SetcD.dSz, Row_Ref, Col_Ref, stRow);

	if(bPlainConc)
	{
		stRow += rowCnt;
		return TRUE;
	}

	PrintCellData(pExcel, _T("BDiaL"), 1 / m_dFactorLength * PrtResD.ParamD.BarD.ddbl, Row_Ref, Col_Ref, stRow);

	int iRbarIdx = PrtResD.ParamD.OptD.bIend ? 0 : 2;

	if (PrtResD.ParamD.SetcD.strShape == _T("GEN"))
	{
		int iIdxRbar = (PrtResD.ParamD.OptD.bIend || GenRbarD.bIJBothMain) ? 0 : 2;
		T_REBT_MABR Mbar = GenRbarD.arMainRebar[iIdxRbar][0];
		PrintCellData(pExcel, _T("BNameL"), Mbar.strRebarName, Row_Ref, Col_Ref, stRow);
		PrintCellData(pExcel, _T("BAreaL"), m_pDoc->m_pMatlDB->Get_RebarArea(Mbar.strRebarName), Row_Ref, Col_Ref, stRow);
		PrintCellData(pExcel, _T("BRatioL"), (m_dFactorLength * m_dFactorLength) *  (m_pDoc->m_pMatlDB->Get_RebarArea(Mbar.strRebarName) * GenRbarD.arMainRebar[iIdxRbar].GetSize()) / PrtResD.ParamD.SetcD.dArea, Row_Ref, Col_Ref, stRow);
	}
	else
	{
		PrintCellData(pExcel, _T("BNameL"), RchkData.COLM.strBarNa1[iRbarIdx][0][0], Row_Ref, Col_Ref, stRow);
		PrintCellData(pExcel, _T("BAreaL"), m_pDoc->m_pMatlDB->Get_RebarArea(RchkData.COLM.strBarNa1[iRbarIdx][0][0]), Row_Ref, Col_Ref, stRow);

		double dAst = 0.0;
		for(int i = 0; i < 3; ++i)
        {
            for(int j = 0; j < T_RCHK_COLM_LAYER; ++j)
            {
                if(RchkData.COLM.iBarNum[iRbarIdx][i][j] == 0) continue;
                double dAs = m_pDoc->m_pMatlDB->Get_RebarArea(RchkData.COLM.strBarNa1[iRbarIdx][i][j]) * (m_dFactorLength * m_dFactorLength);
                if(!RchkData.COLM.strBarNa2[iRbarIdx][i][j].IsEmpty())
                {
                    dAs += (m_pDoc->m_pMatlDB->Get_RebarArea(RchkData.COLM.strBarNa2[iRbarIdx][i][j]) * (m_dFactorLength * m_dFactorLength));
                    dAs /= 2.0;
                }
				dAst += (dAs * 2.0 * RchkData.COLM.iBarNum[iRbarIdx][i][j]);
            }
        }
		const double drho = dAst / PrtResD.ParamD.SetcD.dArea;
		PrintCellData(pExcel, _T("BRatioL"), drho, Row_Ref, Col_Ref, stRow);
	}

	PrintCellData(pExcel, _T("BDiaT"), 1 / m_dFactorLength * PrtResD.ParamD.BarD.ddbs, Row_Ref, Col_Ref, stRow);

	if (PrtResD.ParamD.SetcD.strShape == _T("GEN"))
	{
		int iIdxSbar = (PrtResD.ParamD.OptD.bIend || GenRbarD.bIJBothShear) ? 0 : 2;
		T_REBT_SHBR SBar = GenRbarD.ShearRebar[iIdxSbar];

		PrintCellData(pExcel, _T("BNameT"), SBar.strSubRebarName, Row_Ref, Col_Ref, stRow);
		PrintCellData(pExcel, _T("BAreaT"), m_pDoc->m_pMatlDB->Get_RebarArea(SBar.strSubRebarName), Row_Ref, Col_Ref, stRow);
	}
	else
	{
		PrintCellData(pExcel, _T("BNameT"), RchkData.COLM.strSubBarNa[iRbarIdx], Row_Ref, Col_Ref, stRow);
		PrintCellData(pExcel, _T("BAreaT"), m_pDoc->m_pMatlDB->Get_RebarArea(RchkData.COLM.strSubBarNa[iRbarIdx]), Row_Ref, Col_Ref, stRow);
	}

	if (PrtResD.ParamD.OptD.nSplice == 2)
	{
		PrintCellData(pExcel, _T("BRatioT"), PrtResD.ParamD.BarD.drhosh, Row_Ref, Col_Ref, stRow);
	}
	else
	{
		PrintCellData(pExcel, _T("BRatioT"), _T("-"), Row_Ref, Col_Ref, stRow);
	}

	PrintCellData(pExcel, _T("BDisT"), PrtResD.ParamD.BarD.ds, Row_Ref, Col_Ref, stRow);

	stRow += rowCnt;

	return TRUE;

}

BOOL CBrdgSeisEvalKISTEC2015_Report::GenerateStructInfoTop(IExcel* pExcel, CString& RptBlock, int& stRow, const EN_BEHAVIOR_TYPE* enBehavior, const double* dB, const double* dH,
	T_BRDGEVAL_RESULT ResultD[2][2], T_MPCC_D* pMpccD, const T_RCHK_D& RchkData, const T_REBT_D& GenRbarD, const CString& strName, bool bPlainConc)
{
	T_BRDGEVAL_RESULT& PrtResD  = ResultD[1][0].EvalD.bCheck ? ResultD[1][0] : ResultD[1][1];
	if (!PrtResD.EvalD.bCheck) return FALSE;

	stRow++;
	int iIdx = ResultD[1][0].EvalD.bCheck ? 2 : 3;

	RptBlock = _T("[1]Title_Check_Req_Top_Rahmen");
	int rowCnt = CopyRange(pExcel, RptBlock, stRow);
	stRow += rowCnt;

	RptBlock = _T("[1]Struct_Info");
	rowCnt = CopyRangeSubTitle(pExcel, RptBlock, stRow);

	short Row_Ref = 0, Col_Ref = 0;
	GetCellIDByName(pExcel, RptBlock, Row_Ref, Col_Ref);
	PrintCellData(pExcel, _T("Name"), strName, Row_Ref, Col_Ref, stRow); //주각이름

	CString strSectFigFileName;
	strSectFigFileName.Format(_T("%s_%s_Sect.emf"), m_strPicPath, pMpccD->strName);
	Make_SectionFig(strSectFigFileName, pMpccD);
	m_pDoc->m_pMatlDB->Create_RebarData(m_CodeUnit.nBase_Length, m_CodeUnit.nBase_Force, TRUE);
	PrintCellData(pExcel, _T("[1]Sect_Image"), PrintSectionFig(pExcel, m_strPicPath, strSectFigFileName, stRow), Row_Ref, Col_Ref, stRow); //이미지 출력

	if (PrtResD.ParamD.SetcD.strShape == _T("P") || PrtResD.ParamD.SetcD.strShape == _T("SR"))
	{
		PrintCellData(pExcel, _T("Shape"), _LS(IDS_DGN_SEIS_SHAPE_CIR), Row_Ref, Col_Ref, stRow);
		PrintCellData(pExcel, _T("Dia"), dH[iIdx], Row_Ref, Col_Ref, stRow);
	}
	else if (PrtResD.ParamD.SetcD.strShape == _T("B") ||
		PrtResD.ParamD.SetcD.strShape == _T("SB") ||
		PrtResD.ParamD.SetcD.strShape == _T("OCT") ||
		PrtResD.ParamD.SetcD.strShape == _T("SOCT"))
	{
		PrintCellData(pExcel, _T("Shape"), _LS(IDS_DGN_SEIS_SHAPE_BOX), Row_Ref, Col_Ref, stRow);
		PrintCellData(pExcel, _T("Width"), dB[iIdx], Row_Ref, Col_Ref, stRow);
		PrintCellData(pExcel, _T("Height"), dH[iIdx], Row_Ref, Col_Ref, stRow);
	}
	else if (PrtResD.ParamD.SetcD.strShape == _T("TRK") ||
		PrtResD.ParamD.SetcD.strShape == _T("STRK"))
	{
		PrintCellData(pExcel, _T("Shape"), _LS(IDS_DGN_SEIS_SHAPE_TRACK), Row_Ref, Col_Ref, stRow);
		PrintCellData(pExcel, _T("Width_Max"), dB[iIdx], Row_Ref, Col_Ref, stRow);
		PrintCellData(pExcel, _T("Height"), dH[iIdx], Row_Ref, Col_Ref, stRow);
	}
	else if (PrtResD.ParamD.SetcD.strShape == _T("HTRK"))
	{
		PrintCellData(pExcel, _T("Shape"), _LS(IDS_DGN_SEIS_SHAPE_HTRACK), Row_Ref, Col_Ref, stRow);
		PrintCellData(pExcel, _T("Width_Max"), dB[iIdx], Row_Ref, Col_Ref, stRow);
		PrintCellData(pExcel, _T("Height"), dH[iIdx], Row_Ref, Col_Ref, stRow);
	}
	else if (PrtResD.ParamD.SetcD.strShape == _T("GEN"))
	{
		PrintCellData(pExcel, _T("Shape"), _LS(IDS_DGN_SEIS_SHAPE_GEN), Row_Ref, Col_Ref, stRow);
		PrintCellData(pExcel, _T("Width"), dB[iIdx], Row_Ref, Col_Ref, stRow);
		PrintCellData(pExcel, _T("Height"), dH[iIdx], Row_Ref, Col_Ref, stRow);
	}
	else
	{
		ASSERT(0);
	}

	PrintCellData(pExcel, _T("Area"), PrtResD.ParamD.SetcD.dArea, Row_Ref, Col_Ref, stRow);
	PrintCellData(pExcel, _T("Dc"), PrtResD.ParamD.BarD.ddc, Row_Ref, Col_Ref, stRow);
	PrintCellData(pExcel, _T("Sy"), PrtResD.ParamD.SetcD.dSy, Row_Ref, Col_Ref, stRow);
	PrintCellData(pExcel, _T("Sz"), PrtResD.ParamD.SetcD.dSz, Row_Ref, Col_Ref, stRow);

	if(bPlainConc)
	{
		stRow += rowCnt;
		return TRUE;
	}

	PrintCellData(pExcel, _T("BDiaL"), 1 / m_dFactorLength * PrtResD.ParamD.BarD.ddbl, Row_Ref, Col_Ref, stRow);

	int iRbarIdx = PrtResD.ParamD.OptD.bIend ? 0 : 2;

	if (PrtResD.ParamD.SetcD.strShape == _T("GEN"))
	{
		int iIdxRbar = (ResultD[0][0].ParamD.OptD.bIend || GenRbarD.bIJBothMain) ? 0 : 2;
		T_REBT_MABR Mbar = GenRbarD.arMainRebar[iIdxRbar][0];
		PrintCellData(pExcel, _T("BNameL"), Mbar.strRebarName, Row_Ref, Col_Ref, stRow);
		PrintCellData(pExcel, _T("BAreaL"), m_pDoc->m_pMatlDB->Get_RebarArea(Mbar.strRebarName), Row_Ref, Col_Ref, stRow);
		PrintCellData(pExcel, _T("BRatioL"), (m_dFactorLength * m_dFactorLength) *  (m_pDoc->m_pMatlDB->Get_RebarArea(Mbar.strRebarName) * GenRbarD.arMainRebar[iIdxRbar].GetSize()) / PrtResD.ParamD.SetcD.dArea, Row_Ref, Col_Ref, stRow);
	}
	else
	{
		PrintCellData(pExcel, _T("BNameL"), RchkData.COLM.strBarNa1[iRbarIdx][0][0], Row_Ref, Col_Ref, stRow);
		PrintCellData(pExcel, _T("BAreaL"), m_pDoc->m_pMatlDB->Get_RebarArea(RchkData.COLM.strBarNa1[iRbarIdx][0][0]), Row_Ref, Col_Ref, stRow);

		double dAst = 0.0;
		for(int i = 0; i < 3; ++i)
        {
            for(int j = 0; j < T_RCHK_COLM_LAYER; ++j)
            {
                if(RchkData.COLM.iBarNum[iRbarIdx][i][j] == 0) continue;
                double dAs = m_pDoc->m_pMatlDB->Get_RebarArea(RchkData.COLM.strBarNa1[iRbarIdx][i][j]) * (m_dFactorLength * m_dFactorLength);
                if(!RchkData.COLM.strBarNa2[iRbarIdx][i][j].IsEmpty())
                {
                    dAs += (m_pDoc->m_pMatlDB->Get_RebarArea(RchkData.COLM.strBarNa2[iRbarIdx][i][j]) * (m_dFactorLength * m_dFactorLength));
                    dAs /= 2.0;
                }
				dAst += (dAs * 2.0 * RchkData.COLM.iBarNum[iRbarIdx][i][j]);
            }
        }
		const double drho = dAst / PrtResD.ParamD.SetcD.dArea;
		PrintCellData(pExcel, _T("BRatioL"), drho, Row_Ref, Col_Ref, stRow);
	}

	PrintCellData(pExcel, _T("BDiaT"), 1 / m_dFactorLength * PrtResD.ParamD.BarD.ddbs, Row_Ref, Col_Ref, stRow);

	if (PrtResD.ParamD.SetcD.strShape == _T("GEN"))
	{
		int iIdxSbar = (PrtResD.ParamD.OptD.bIend || GenRbarD.bIJBothShear) ? 0 : 2;
		T_REBT_SHBR SBar = GenRbarD.ShearRebar[iIdxSbar];

		PrintCellData(pExcel, _T("BNameT"), SBar.strSubRebarName, Row_Ref, Col_Ref, stRow);
		PrintCellData(pExcel, _T("BAreaT"), m_pDoc->m_pMatlDB->Get_RebarArea(SBar.strSubRebarName), Row_Ref, Col_Ref, stRow);
	}
	else
	{
		PrintCellData(pExcel, _T("BNameT"), RchkData.COLM.strSubBarNa[iRbarIdx], Row_Ref, Col_Ref, stRow);
		PrintCellData(pExcel, _T("BAreaT"), m_pDoc->m_pMatlDB->Get_RebarArea(RchkData.COLM.strSubBarNa[iRbarIdx]), Row_Ref, Col_Ref, stRow);
	}

	if (PrtResD.ParamD.OptD.nSplice == 2)
	{
		PrintCellData(pExcel, _T("BRatioT"), PrtResD.ParamD.BarD.drhosh, Row_Ref, Col_Ref, stRow);
	}
	else
	{
		PrintCellData(pExcel, _T("BRatioT"), _T("-"), Row_Ref, Col_Ref, stRow);
	}

	PrintCellData(pExcel, _T("BDisT"), PrtResD.ParamD.BarD.ds, Row_Ref, Col_Ref, stRow);

	stRow += rowCnt;

	return TRUE;

}

BOOL CBrdgSeisEvalKISTEC2015_Report::PrintConcreteGraph(IExcel* pExcel, int nConcType, int& stRow)
{
	///*
	//IDS_CMD2_FIMP_KENT_PARK    0     //Kent & Park Model
	//IDS_CMD2_FIMP_JCONCRETE    1     //Japan Concrete Standard Specification Model
	//IDS_CMD2_FIMP_JBRIDGE      2     //Japan Roadway Specification(H.14) Model
	//IDS_CMD2_FIMP_NAKOYA			 3     //Nagoya Highway Corporation Model
	//IDS_CMD2_FIMP_CON_TRILIN   4     //Trilinear Concrete Model
	//IDS_CMD2_CHINA_GB50010_02  5     //China Concrete Code (GB50010-02)
	//IDS_CMD2_FIMP_MANDER			 6     //Mander Model
	//IDS_CMD2_FIMP_JBRIDGE_H24	 7     //Japan Roadway Specification(H.24) Model
	//IDS_CMD2_CHINA_GB50010_10  8     //China Concrete Code (GB/T50010-10)

	CString aResPath[9] = {
		_T("01_Conc_Kent_Park.bmp"),
		_T("02_Conc_Japan Concrete Standard Specification Model.bmp"),
		_T("03_Conc_Japan Roadway Specification(H.14) Model.bmp"),
		_T("04_Conc_Nagoya Highway Corporation Model.bmp"),
		_T("05_Conc_Trilinear Concrete Model.bmp"),
		_T("06_Conc_China Concrete Code (GB50010-02).bmp"),
		_T("07_Conc_Mander Model.bmp"),
		_T("08_Conc_Japan Roadway Specification(H.24) Model.bmp"),
		_T("09_Conc_China Concrete Code (GB/T50010-10).bmp"),
	};

	int nH = 12;
	int nW = 14;

	CString strPath = m_pDoc->GetProgramPath() + _T("\\Material Model\\Concrete\\") + aResPath[nConcType];

	if (WaitForFile(strPath))
	{
		pExcel->AddPicture_Count(stRow, 7, nH, nW, strPath);
	}

	stRow = stRow + nH;

	return TRUE;
}

BOOL CBrdgSeisEvalKISTEC2015_Report::PrintSteelGraph(IExcel* pExcel, int nStlType, int& stRow)
{
	//	/*
	//	IDS_CMD2_FIMP_MENEGOTTO				0			//Menegotto-Pinto Model
	//	IDS_CMD2_FIMP_BILINEAR				1			//Bilinear Model
	//	IDS_CMD2_FIMP_GEN_BILINEAR		2			//Asymmetrical Bilinear Steel Model
	//	IDS_CMD2_FIMP_STL_TRILIN			3			//Trilinear Steel Model
	//	IDS_CMD2_FIMP_PARK_MODEL			4			//Park Model
	//	IDS_CMD2_FIMP_STL_JBRIDGE			5			//Japan Roadway Specification Model
	//	IDS_CMD2_CHINA_STL_GB50010_10 6			//China Steel Code (GB50010-2010)
	//	*/

	CString aResPath[7] = {
		_T("01_Stl_Menegotto-Pinto Model.bmp"),
		_T("02_Stl_Bilinear Model.bmp"),
		_T("03_Stl_Unsymmetric Bilinear Steel Model.bmp"),
		_T("04_Stl_Trilinear Steel Model .bmp"),
		_T("05_Stl_Park Model.bmp"),
		_T("06_Stl_Japan Roadway Specification Model.bmp"),
		_T("07_Stl_China Steel Code(GB50010-2010) Model.bmp") };
	//
	int nH = 12;
	int nW = 14;
	//
	CString strPath = m_pDoc->GetProgramPath() + _T("\\Material Model\\Steel\\") + aResPath[nStlType];

	if (WaitForFile(strPath))
	{
		pExcel->AddPicture_Count(stRow, 7, nH, nW, strPath);
	}

	stRow = stRow + nH;

	return TRUE;
}

void CBrdgSeisEvalKISTEC2015_Report::GetMPhiGraphFileNames(CString aResName[3])
{
	aResName[0] = _T("option01.bmp");
	aResName[1] = _T("option02.bmp");
	aResName[2] = _T("option03.bmp");
}

BOOL CBrdgSeisEvalKISTEC2015_Report::PrintIdealGraph(IExcel* pExcel, int nOption, int& stRow)
{
	CString aResPath[3];
	GetMPhiGraphFileNames(aResPath);

	int nH = 13;
	int nW = 15;

	CString strPath = m_pDoc->GetProgramPath() + _T("\\Material Model\\Option\\") + aResPath[nOption];

	if (WaitForFile(strPath))
	{
		pExcel->AddPicture_Count(stRow + 5, 7, nH, nW, strPath);
	}

	return TRUE;
}

BOOL CBrdgSeisEvalKISTEC2015_Report::GenerateEvalSheet(IExcel* pExcel, CString& RptBlock, int& stRow, const EN_PIER_TYPE* enPierType, T_BRDGEVAL_RESULT ResultD[2][2])
{
	stRow = 1;


	if (enPierType[0] == WALL)
	{
		RptBlock = _T("[2]Title_Sect_Brick");
	}
	else
	{
		T_BRDGEVAL_RESULT PrtResultD;
		if (ResultD[0][0].EvalD.bCheck)      PrtResultD = ResultD[0][0]; // 하부 교축
		else if (ResultD[1][0].EvalD.bCheck) PrtResultD = ResultD[1][0]; // 상부 교축
		else if (ResultD[0][1].EvalD.bCheck) PrtResultD = ResultD[0][1]; // 하부 교축직각
		else if (ResultD[1][1].EvalD.bCheck) PrtResultD = ResultD[1][1]; // 상부 교축직각

		if (PrtResultD.ParamD.SetcD.strShape == _T("P") || PrtResultD.ParamD.SetcD.strShape == _T("SR"))
		{
			RptBlock = _T("[2]Title_Sect_Circle");
		}
		else if (PrtResultD.ParamD.SetcD.strShape == _T("B") ||
			PrtResultD.ParamD.SetcD.strShape == _T("SB") ||
			PrtResultD.ParamD.SetcD.strShape == _T("OCT") ||
			PrtResultD.ParamD.SetcD.strShape == _T("SOCT"))
		{
			RptBlock = _T("[2]Title_Sect_Box");
		}
		else if (PrtResultD.ParamD.SetcD.strShape == _T("TRK") || PrtResultD.ParamD.SetcD.strShape == _T("STRK"))
		{
			RptBlock = _T("[2]Title_Sect_Track");
		}
		else if (PrtResultD.ParamD.SetcD.strShape == _T("HTRK"))
		{
			RptBlock = _T("[2]Title_Sect_HalfTrack");
		}
		else if (PrtResultD.ParamD.SetcD.strShape == _T("GEN"))
		{
			RptBlock = _T("[2]Title_Sect_General");
		}
		else
		{
			ASSERT(0);
		}
	}
	int rowCnt = CopyRange(pExcel, RptBlock, stRow);
	stRow += rowCnt;

	return TRUE;
}

BOOL CBrdgSeisEvalKISTEC2015_Report::GenerateIdealMomentPhi(IExcel* pExcel, CString& RptBlock, int& stRow, const EN_BEHAVIOR_TYPE* enBehavior, T_BRDGEVAL_RESULT ResultD[2][2], T_MPCC_D& MpccD1, T_MPCC_D& MpccD2, T_MPCC_K MpccK1, T_MPCC_K MpccK2)
{
	short Row_Ref = 0, Col_Ref = 0;

	if (MpccK1 == 0) MpccD1.Initialize();

	RptBlock = _T("[2]Idealized_Option");
	int rowCnt = CopyRangeSubTitle(pExcel, RptBlock, stRow);
	if (MpccK1 != 0 && (MpccD1.nIdealOpt == 0 || MpccD1.nIdealOpt == 1 || MpccD1.nIdealOpt == 2))
		PrintIdealGraph(pExcel, MpccD1.nIdealOpt, stRow);
	else if (MpccK2 != 0 && (MpccD2.nIdealOpt == 0 || MpccD2.nIdealOpt == 1 || MpccD2.nIdealOpt == 2))
		PrintIdealGraph(pExcel, MpccD2.nIdealOpt, stRow);
	else
	{
		ASSERT(0);
		PrintIdealGraph(pExcel, 1, stRow);
	}

	stRow += rowCnt;

	RptBlock = _T("[2]Summary_Moment_Phi_Head");
	rowCnt = CopyRangeSubTitle(pExcel, RptBlock, stRow);
	stRow += rowCnt;

	CString strMPhiFileName;
	
	if (enBehavior[0] == CANTILEVER)
	{
		CDgnSeisMPhiCurveDlg dlg;
		strMPhiFileName.Format(_T("%s_%s_%s_M_Phi.emf"), m_strPicPath, MpccD1.strName, _T("LONGI"));
		dlg.SetParamData(MpccD1);

		dlg.m_nAngleDir = 0;
		dlg.MakeBmpAndClose(strMPhiFileName);
		dlg.DoModal();

		RptBlock = _T("[2]Summary_Moment_Phi_Cantil_Longi");
		rowCnt = CopyRangeSubTitle(pExcel, RptBlock, stRow);
		GetCellIDByName(pExcel, RptBlock, Row_Ref, Col_Ref);

		if (MpccK1 != 0)
		{
			PrintCellData(pExcel, _T("S1_Cur_Cr_Longi"), 1000 * ResultD[0][0].MPhiD.StateD.dCurvature[0], Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S1_Cur_Yinit_Longi"), 1000 * ResultD[0][0].MPhiD.StateD.dCurvature[1], Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S1_Cur_Y_Longi"), 1000 * ResultD[0][0].MPhiD.StateD.dCurvature[2], Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S1_Cur_Ult_Longi"), 1000 * ResultD[0][0].MPhiD.StateD.dCurvature[3], Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S1_Cur_Yideal_Longi"), 1000 * ResultD[0][0].MPhiD.StateD.dCurvature[4], Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S1_Mom_Cr_Longi"), ResultD[0][0].MPhiD.StateD.dMoment[0], Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S1_Mom_Yinit_Longi"), ResultD[0][0].MPhiD.StateD.dMoment[1], Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S1_Mom_Y_Longi"), ResultD[0][0].MPhiD.StateD.dMoment[2], Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S1_Mom_Ult_Longi"), ResultD[0][0].MPhiD.StateD.dMoment[3], Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S1_Mom_Yideal_Longi"), ResultD[0][0].MPhiD.StateD.dMoment[4], Row_Ref, Col_Ref, stRow);
		}
		PrintCellData(pExcel, _T("M_Phi_Img_Longi"), PrintMphiGraph(pExcel, strMPhiFileName, stRow - 1), Row_Ref, Col_Ref, stRow);

		stRow += rowCnt;
	}
	else
	{
		CDgnSeisMPhiCurveDlg dlg;
		strMPhiFileName.Format(_T("%s_%s_%s_%s_M_Phi.emf"), m_strPicPath, MpccD1.strName, _T("BOTTOM"), _T("LONGI"));
		dlg.SetParamData(MpccD1);
		dlg.m_nAngleDir = 0;
		dlg.MakeBmpAndClose(strMPhiFileName);
		dlg.DoModal();

		RptBlock = _T("[2]Summary_Moment_Phi_Rahmen_Longi");
		rowCnt = CopyRangeSubTitle(pExcel, RptBlock, stRow);
		GetCellIDByName(pExcel, RptBlock, Row_Ref, Col_Ref);

		if (MpccK1 != 0)
		{
			PrintCellData(pExcel, _T("S1_Cur_Cr_Bot_Longi"), 1000 * ResultD[0][0].MPhiD.StateD.dCurvature[0], Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S1_Cur_Yinit_Bot_Longi"), 1000 * ResultD[0][0].MPhiD.StateD.dCurvature[1], Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S1_Cur_Y_Bot_Longi"), 1000 * ResultD[0][0].MPhiD.StateD.dCurvature[2], Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S1_Cur_Ult_Bot_Longi"), 1000 * ResultD[0][0].MPhiD.StateD.dCurvature[3], Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S1_Cur_Yideal_Bot_Longi"), 1000 * ResultD[0][0].MPhiD.StateD.dCurvature[4], Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S1_Mom_Cr_Bot_Longi"), ResultD[0][0].MPhiD.StateD.dMoment[0], Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S1_Mom_Yinit_Bot_Longi"), ResultD[0][0].MPhiD.StateD.dMoment[1], Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S1_Mom_Y_Bot_Longi"), ResultD[0][0].MPhiD.StateD.dMoment[2], Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S1_Mom_Ult_Bot_Longi"), ResultD[0][0].MPhiD.StateD.dMoment[3], Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S1_Mom_Yideal_Bot_Longi"), ResultD[0][0].MPhiD.StateD.dMoment[4], Row_Ref, Col_Ref, stRow);
		}
		PrintCellData(pExcel, _T("M_Phi_Img_Bot_Longi"), PrintMphiGraph(pExcel, strMPhiFileName, stRow), Row_Ref, Col_Ref, stRow);

		CDgnSeisMPhiCurveDlg dlg2;
		strMPhiFileName.Format(_T("%s_%s_%s_%s_M_Phi.emf"), m_strPicPath, MpccD2.strName, _T("TOP"), _T("LONGI"));
		dlg2.SetParamData(MpccD2);
		dlg2.m_nAngleDir = 0;
		dlg2.MakeBmpAndClose(strMPhiFileName);
		dlg2.DoModal();

		if (MpccK2 != 0)
		{
			PrintCellData(pExcel, _T("S1_Cur_Cr_Top_Longi"), 1000 * ResultD[1][0].MPhiD.StateD.dCurvature[0], Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S1_Cur_Yinit_Top_Longi"), 1000 * ResultD[1][0].MPhiD.StateD.dCurvature[1], Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S1_Cur_Y_Top_Longi"), 1000 * ResultD[1][0].MPhiD.StateD.dCurvature[2], Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S1_Cur_Ult_Top_Longi"), 1000 * ResultD[1][0].MPhiD.StateD.dCurvature[3], Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S1_Cur_Yideal_Top_Longi"), 1000 * ResultD[1][0].MPhiD.StateD.dCurvature[4], Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S1_Mom_Cr_Top_Longi"), ResultD[1][0].MPhiD.StateD.dMoment[0], Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S1_Mom_Yinit_Top_Longi"), ResultD[1][0].MPhiD.StateD.dMoment[1], Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S1_Mom_Y_Top_Longi"), ResultD[1][0].MPhiD.StateD.dMoment[2], Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S1_Mom_Ult_Top_Longi"), ResultD[1][0].MPhiD.StateD.dMoment[3], Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S1_Mom_Yideal_Top_Longi"), ResultD[1][0].MPhiD.StateD.dMoment[4], Row_Ref, Col_Ref, stRow);
		}
		PrintCellData(pExcel, _T("M_Phi_Img_Top_Longi"), PrintMphiGraph(pExcel, strMPhiFileName, stRow + 25), Row_Ref, Col_Ref, stRow);

		stRow += rowCnt;
	}

	if (enBehavior[1] == CANTILEVER)
	{
		CDgnSeisMPhiCurveDlg dlg;
		strMPhiFileName.Format(_T("%s_%s_%s_M_Phi.emf"), m_strPicPath, MpccD1.strName, _T("TRANS"));
		dlg.SetParamData(MpccD1);

		dlg.m_nAngleDir = 1;
		dlg.MakeBmpAndClose(strMPhiFileName);
		dlg.DoModal();

		RptBlock = _T("[2]Summary_Moment_Phi_Cantil_Trans");
		rowCnt = CopyRangeSubTitle(pExcel, RptBlock, stRow);
		GetCellIDByName(pExcel, RptBlock, Row_Ref, Col_Ref);

		if (MpccK1 != 0)
		{
			PrintCellData(pExcel, _T("S1_Cur_Cr_Trans"), 1000 * ResultD[0][1].MPhiD.StateD.dCurvature[0], Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S1_Cur_Yinit_Trans"), 1000 * ResultD[0][1].MPhiD.StateD.dCurvature[1], Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S1_Cur_Y_Trans"), 1000 * ResultD[0][1].MPhiD.StateD.dCurvature[2], Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S1_Cur_Ult_Trans"), 1000 * ResultD[0][1].MPhiD.StateD.dCurvature[3], Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S1_Cur_Yideal_Trans"), 1000 * ResultD[0][1].MPhiD.StateD.dCurvature[4], Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S1_Mom_Cr_Trans"), ResultD[0][1].MPhiD.StateD.dMoment[0], Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S1_Mom_Yinit_Trans"), ResultD[0][1].MPhiD.StateD.dMoment[1], Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S1_Mom_Y_Trans"), ResultD[0][1].MPhiD.StateD.dMoment[2], Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S1_Mom_Ult_Trans"), ResultD[0][1].MPhiD.StateD.dMoment[3], Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S1_Mom_Yideal_Trans"), ResultD[0][1].MPhiD.StateD.dMoment[4], Row_Ref, Col_Ref, stRow);
		}
		PrintCellData(pExcel, _T("M_Phi_Img_Trans"), PrintMphiGraph(pExcel, strMPhiFileName, stRow - 1), Row_Ref, Col_Ref, stRow);

		stRow += rowCnt;
	}
	else
	{
		CDgnSeisMPhiCurveDlg dlg;
		strMPhiFileName.Format(_T("%s_%s_%s_%s_M_Phi.emf"), m_strPicPath, MpccD1.strName, _T("BOTTOM"), _T("TRANS"));
		dlg.SetParamData(MpccD1);
		dlg.m_nAngleDir = 1;
		dlg.MakeBmpAndClose(strMPhiFileName);
		dlg.DoModal();

		RptBlock = _T("[2]Summary_Moment_Phi_Rahmen_Trans");
		rowCnt = CopyRangeSubTitle(pExcel, RptBlock, stRow);
		GetCellIDByName(pExcel, RptBlock, Row_Ref, Col_Ref);

		if (MpccK1 != 0)
		{
			PrintCellData(pExcel, _T("S1_Cur_Cr_Bot_Trans"), 1000 * ResultD[0][1].MPhiD.StateD.dCurvature[0], Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S1_Cur_Yinit_Bot_Trans"), 1000 * ResultD[0][1].MPhiD.StateD.dCurvature[1], Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S1_Cur_Y_Bot_Trans"), 1000 * ResultD[0][1].MPhiD.StateD.dCurvature[2], Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S1_Cur_Ult_Bot_Trans"), 1000 * ResultD[0][1].MPhiD.StateD.dCurvature[3], Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S1_Cur_Yideal_Bot_Trans"), 1000 * ResultD[0][1].MPhiD.StateD.dCurvature[4], Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S1_Mom_Cr_Bot_Trans"), ResultD[0][1].MPhiD.StateD.dMoment[0], Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S1_Mom_Yinit_Bot_Trans"), ResultD[0][1].MPhiD.StateD.dMoment[1], Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S1_Mom_Y_Bot_Trans"), ResultD[0][1].MPhiD.StateD.dMoment[2], Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S1_Mom_Ult_Bot_Trans"), ResultD[0][1].MPhiD.StateD.dMoment[3], Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S1_Mom_Yideal_Bot_Trans"), ResultD[0][1].MPhiD.StateD.dMoment[4], Row_Ref, Col_Ref, stRow);
		}
		PrintCellData(pExcel, _T("M_Phi_Img_Bot_Trans"), PrintMphiGraph(pExcel, strMPhiFileName, stRow), Row_Ref, Col_Ref, stRow);

		CDgnSeisMPhiCurveDlg dlg2;
		strMPhiFileName.Format(_T("%s_%s_%s_%s_M_Phi.emf"), m_strPicPath, MpccD2.strName, _T("TOP"), _T("TRANS"));
		dlg2.SetParamData(MpccD2);
		dlg2.m_nAngleDir = 1;
		dlg2.MakeBmpAndClose(strMPhiFileName);
		dlg2.DoModal();

		if (MpccK2 != 0)
		{
			PrintCellData(pExcel, _T("S1_Cur_Cr_Top_Trans"), 1000 * ResultD[1][1].MPhiD.StateD.dCurvature[0], Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S1_Cur_Yinit_Top_Trans"), 1000 * ResultD[1][1].MPhiD.StateD.dCurvature[1], Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S1_Cur_Y_Top_Trans"), 1000 * ResultD[1][1].MPhiD.StateD.dCurvature[2], Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S1_Cur_Ult_Top_Trans"), 1000 * ResultD[1][1].MPhiD.StateD.dCurvature[3], Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S1_Cur_Yideal_Top_Trans"), 1000 * ResultD[1][1].MPhiD.StateD.dCurvature[4], Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S1_Mom_Cr_Top_Trans"), ResultD[1][1].MPhiD.StateD.dMoment[0], Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S1_Mom_Yinit_Top_Trans"), ResultD[1][1].MPhiD.StateD.dMoment[1], Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S1_Mom_Y_Top_Trans"), ResultD[1][1].MPhiD.StateD.dMoment[2], Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S1_Mom_Ult_Top_Trans"), ResultD[1][1].MPhiD.StateD.dMoment[3], Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S1_Mom_Yideal_Top_Trans"), ResultD[1][1].MPhiD.StateD.dMoment[4], Row_Ref, Col_Ref, stRow);
		}
		PrintCellData(pExcel, _T("M_Phi_Img_Top_Trans"), PrintMphiGraph(pExcel, strMPhiFileName, stRow + 25), Row_Ref, Col_Ref, stRow);

		stRow += rowCnt;
	}

	return TRUE;
}

BOOL CBrdgSeisEvalKISTEC2015_Report::PrintMphiGraph(IExcel* pExcel, CString& strMPhiFileName, const int& stRow)
{
	//
	int nH = 15;
	int nW = 17;
	//
	CString strPath = strMPhiFileName;
	if (WaitForFile(strPath))
	{
		pExcel->AddPicture_Count(stRow + 2, 2, nH, nW, strPath);
	}

	return TRUE;
}


BOOL CBrdgSeisEvalKISTEC2015_Report::GenerateHe(IExcel* pExcel, CString& RptBlock, int& stRow, const T_EVPM_D& EvmpD)
{

	RptBlock = _T("[2]Effect_Height_Head");
	int rowCnt = CopyRangeSubTitle(pExcel, RptBlock, stRow);
	stRow += rowCnt;
	short Row_Ref = 0, Col_Ref = 0;

	RptBlock = _T("[2]Effect_Height_Longi_Head");
	rowCnt = CopyRangeSubTitle(pExcel, RptBlock, stRow);
	stRow += rowCnt;

	if (EvmpD.nBehavL == 1) // CANTILEVER
	{
		RptBlock = _T("[2]Effect_Height_Cantil_Longi");
		rowCnt = CopyRangeSubTitle(pExcel, RptBlock, stRow);
		GetCellIDByName(pExcel, RptBlock, Row_Ref, Col_Ref);
		PrintCellData(pExcel, _T("S2_EHeight_Longi"), EvmpD.dHe1L, Row_Ref, Col_Ref, stRow);
		stRow += rowCnt + 1;
	}
	else
	{
		RptBlock = _T("[2]Effect_Height_Rahmen_Longi");
		rowCnt = CopyRangeSubTitle(pExcel, RptBlock, stRow);
		GetCellIDByName(pExcel, RptBlock, Row_Ref, Col_Ref);
		PrintCellData(pExcel, _T("S2_EHeight_Bot_Longi"), EvmpD.dHe1L, Row_Ref, Col_Ref, stRow);
		PrintCellData(pExcel, _T("S2_EHeight_Top_Longi"), EvmpD.dHe2L, Row_Ref, Col_Ref, stRow);
		stRow += rowCnt + 1;
	}

	RptBlock = _T("[2]Effect_Height_Trans_Head");
	rowCnt = CopyRangeSubTitle(pExcel, RptBlock, stRow);
	stRow += rowCnt;
	if (EvmpD.nBehavT == 1) // CANTILEVER
	{
		RptBlock = _T("[2]Effect_Height_Cantil_Trans");
		rowCnt = CopyRangeSubTitle(pExcel, RptBlock, stRow);
		GetCellIDByName(pExcel, RptBlock, Row_Ref, Col_Ref);
		PrintCellData(pExcel, _T("S2_EHeight_Trans"), EvmpD.dHe1T, Row_Ref, Col_Ref, stRow);
		stRow += rowCnt + 1;
	}
	else
	{
		RptBlock = _T("[2]Effect_Height_Rahmen_Trans");
		rowCnt = CopyRangeSubTitle(pExcel, RptBlock, stRow);
		GetCellIDByName(pExcel, RptBlock, Row_Ref, Col_Ref);
		PrintCellData(pExcel, _T("S2_EHeight_Bot_Trans"), EvmpD.dHe1T, Row_Ref, Col_Ref, stRow);
		PrintCellData(pExcel, _T("S2_EHeight_Top_Trans"), EvmpD.dHe2T, Row_Ref, Col_Ref, stRow);
		stRow += rowCnt + 1;
	}

	return TRUE;
}

BOOL CBrdgSeisEvalKISTEC2015_Report::GeneratePeriod(IExcel* pExcel, CString& RptBlock, int& stRow, T_BRDGEVAL_RESULT ResultD[2][2])
{
	short  Row_Ref, Col_Ref;
	RptBlock = _T("[2]Oscillation_Period_Info");
	int rowCnt = CopyRangeSubTitle(pExcel, RptBlock, stRow);
	GetCellIDByName(pExcel, RptBlock, Row_Ref, Col_Ref);

	double dTl = 0.0, dTt = 0.0;
	m_pDoc->m_pPostCtrl->GetEiPeriodByDirSeisEval(dTl, dTt);

	PrintCellData(pExcel, _T("S3_Operiod_Longi"), dTl, Row_Ref, Col_Ref, stRow);
	PrintCellData(pExcel, _T("S3_Operiod_Trans"), dTt, Row_Ref, Col_Ref, stRow);

	stRow += rowCnt;

	return TRUE;

}


BOOL CBrdgSeisEvalKISTEC2015_Report::GenerateEffectiveYield(IExcel* pExcel, CString& RptBlock, int& stRow, const EN_BEHAVIOR_TYPE* enBehavior, T_BRDGEVAL_RESULT ResultD[2][2])
{

	RptBlock = _T("[2]Effect_Stiff_Sect_Head");
	int rowCnt = CopyRangeSubTitle(pExcel, RptBlock, stRow);
	stRow += rowCnt;


	RptBlock = _T("[2]Effect_Stiff_Head");
	rowCnt = CopyRangeSubTitle(pExcel, RptBlock, stRow);
	stRow += rowCnt;

	short Row_Ref = 0, Col_Ref = 0;


	RptBlock = _T("[2]Effect_Stiff_Longi");
	rowCnt = CopyRangeSubTitle(pExcel, RptBlock, stRow);
	stRow += rowCnt;

	if (enBehavior[0] == CANTILEVER)
	{
		RptBlock = _T("[2]Effect_Stiff_Cantil_Longi");
		rowCnt = CopyRangeSubTitle(pExcel, RptBlock, stRow);
		GetCellIDByName(pExcel, RptBlock, Row_Ref, Col_Ref);

		if (ResultD[0][0].EvalD.bCheck)
		{
			PrintCellData(pExcel, _T("S4_1_Mom_Y_Longi"), ResultD[0][0].MPhiD.StateD.dMoment[4], Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S4_1_Cur_Y_Longi"), ResultD[0][0].MPhiD.StateD.dCurvature[4], Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S4_1_EIY_Longi"), ResultD[0][0].MPhiD.StateD.Get_EI(), Row_Ref, Col_Ref, stRow);
		}

		stRow += rowCnt + 1;
	}
	else
	{
		RptBlock = _T("[2]Effect_Stiff_Rahmen_Longi");
		rowCnt = CopyRangeSubTitle(pExcel, RptBlock, stRow);
		GetCellIDByName(pExcel, RptBlock, Row_Ref, Col_Ref);

		if (ResultD[0][0].EvalD.bCheck)
		{
			PrintCellData(pExcel, _T("S4_1_Mom_Y_Bot_Longi"), ResultD[0][0].MPhiD.StateD.dMoment[4], Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S4_1_Cur_Y_Bot_Longi"), ResultD[0][0].MPhiD.StateD.dCurvature[4], Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S4_1_EIY_Bot_Longi"), ResultD[0][0].MPhiD.StateD.Get_EI(), Row_Ref, Col_Ref, stRow);
		}

		if (ResultD[1][0].EvalD.bCheck)
		{
			PrintCellData(pExcel, _T("S4_1_Mom_Y_Top_Longi"), ResultD[1][0].MPhiD.StateD.dMoment[4], Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S4_1_Cur_Y_Top_Longi"), ResultD[1][0].MPhiD.StateD.dCurvature[4], Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S4_1_EIY_Top_Longi"), ResultD[1][0].MPhiD.StateD.Get_EI(), Row_Ref, Col_Ref, stRow);
		}

		stRow += rowCnt + 1;
	}


	RptBlock = _T("[2]Effect_Stiff_Trans");
	rowCnt = CopyRangeSubTitle(pExcel, RptBlock, stRow);
	stRow += rowCnt;

	if (enBehavior[1] == CANTILEVER)
	{
		RptBlock = _T("[2]Effect_Stiff_Cantil_Trans");
		rowCnt = CopyRangeSubTitle(pExcel, RptBlock, stRow);
		GetCellIDByName(pExcel, RptBlock, Row_Ref, Col_Ref);

		if (ResultD[0][1].EvalD.bCheck)
		{
			PrintCellData(pExcel, _T("S4_1_Mom_Y_Trans"), ResultD[0][1].MPhiD.StateD.dMoment[4], Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S4_1_Cur_Y_Trans"), ResultD[0][1].MPhiD.StateD.dCurvature[4], Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S4_1_EIY_Trans"), ResultD[0][1].MPhiD.StateD.Get_EI(), Row_Ref, Col_Ref, stRow);
		}

		stRow += rowCnt + 1;

	}
	else
	{
		RptBlock = _T("[2]Effect_Stiff_Rahmen_Trans");
		rowCnt = CopyRangeSubTitle(pExcel, RptBlock, stRow);
		GetCellIDByName(pExcel, RptBlock, Row_Ref, Col_Ref);

		if (ResultD[0][1].EvalD.bCheck)
		{
			PrintCellData(pExcel, _T("S4_1_Mom_Y_Bot_Trans"), ResultD[0][1].MPhiD.StateD.dMoment[4], Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S4_1_Cur_Y_Bot_Trans"), ResultD[0][1].MPhiD.StateD.dCurvature[4], Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S4_1_EIY_Bot_Trans"), ResultD[0][1].MPhiD.StateD.Get_EI(), Row_Ref, Col_Ref, stRow);
		}

		if (ResultD[1][1].EvalD.bCheck)
		{
			PrintCellData(pExcel, _T("S4_1_Mom_Y_Top_Trans"), ResultD[1][1].MPhiD.StateD.dMoment[4], Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S4_1_Cur_Y_Top_Trans"), ResultD[1][1].MPhiD.StateD.dCurvature[4], Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S4_1_EIY_Top_Trans"), ResultD[1][1].MPhiD.StateD.Get_EI(), Row_Ref, Col_Ref, stRow);
		}

		stRow += rowCnt + 1;

	}



	return TRUE;

}

BOOL CBrdgSeisEvalKISTEC2015_Report::GenerateEffectiveSect(IExcel* pExcel, CString& RptBlock, int& stRow, const EN_BEHAVIOR_TYPE* enBehavior, T_BRDGEVAL_RESULT ResultD[2][2])
{
	short Row_Ref = 0, Col_Ref = 0;

	RptBlock = _T("[2]Effect_Sect_Head");
	int rowCnt = CopyRangeSubTitle(pExcel, RptBlock, stRow);
	stRow += rowCnt;


	RptBlock = _T("[2]Effect_Sect_Longi");
	rowCnt = CopyRangeSubTitle(pExcel, RptBlock, stRow);
	stRow += rowCnt;

	double dEc[2] = {0.0};
	dEc[0] = ResultD[0][0].ParamD.MatlD.dEc;
	dEc[1] = ResultD[0][1].ParamD.MatlD.dEc;
	// MPa; 단위변환 필요 - 도로교설계기준(2010)

	if(enBehavior[0] == CANTILEVER)
	{
		RptBlock = _T("[2]Effect_Sect_Cantil_Longi");
		rowCnt = CopyRangeSubTitle(pExcel, RptBlock, stRow);
		GetCellIDByName(pExcel, RptBlock, Row_Ref, Col_Ref);

		if(ResultD[0][0].EvalD.bCheck)
		{
			double dEI = ResultD[0][0].MPhiD.StateD.Get_EI();
			double dEffStiff = m_dFactorStress * ResultD[0][0].ParamD.OptD.dEffStiff;
			double dEffStiffRat = ResultD[0][0].ParamD.OptD.dEffStiffRat;
			if(ResultD[0][0].ParamD.OptD.iEvalCode == KALIS_2023)
			{
				dEffStiff = dEI / dEc[0];
				dEffStiffRat = dEffStiff / ResultD[0][0].ParamD.SetcD.dIy;
			}
			PrintCellData(pExcel, _T("S4_2_EIY_Longi") , dEI, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S4_2_EC_Longi")  , dEc[0], Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S4_2_IY_Longi")  , dEffStiff, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S4_2_IY_LongiIg"), dEffStiffRat, Row_Ref, Col_Ref, stRow);
		}

		stRow += rowCnt + 1;
	}
	else
	{
		RptBlock = _T("[2]Effect_Sect_Rahmen_Longi");
		rowCnt = CopyRangeSubTitle(pExcel, RptBlock, stRow);
		GetCellIDByName(pExcel, RptBlock, Row_Ref, Col_Ref);
		if(ResultD[0][0].EvalD.bCheck)
		{
			double dEI = ResultD[0][0].MPhiD.StateD.Get_EI();
			double dEffStiff = m_dFactorStress * ResultD[0][0].ParamD.OptD.dEffStiff;
			double dEffStiffRat = ResultD[0][0].ParamD.OptD.dEffStiffRat;
			if(ResultD[0][0].ParamD.OptD.iEvalCode == KALIS_2023)
			{
				dEffStiff = dEI / dEc[0];
				dEffStiffRat = dEffStiff / ResultD[0][0].ParamD.SetcD.dIy;
			}
			PrintCellData(pExcel, _T("S4_2_EIY_Bot_Longi"), dEI, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S4_2_EC_Bot_Longi"), dEc[0], Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S4_2_IY_Bot_Longi"), dEffStiff, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S4_2_IY_Bot_LongiIg"), dEffStiffRat, Row_Ref, Col_Ref, stRow);
		}

		if(ResultD[1][0].EvalD.bCheck)
		{
			double dEI = ResultD[1][0].MPhiD.StateD.Get_EI();
			double dEffStiff = m_dFactorStress * ResultD[1][0].ParamD.OptD.dEffStiff;
			double dEffStiffRat = ResultD[1][0].ParamD.OptD.dEffStiffRat;
			if(ResultD[0][0].ParamD.OptD.iEvalCode == KALIS_2023)
			{
				dEffStiff = dEI / dEc[0];
				dEffStiffRat = dEffStiff / ResultD[1][0].ParamD.SetcD.dIy;
			}
			PrintCellData(pExcel, _T("S4_2_EIY_Top_Longi"), dEI, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S4_2_EC_Top_Longi"), dEc[0], Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S4_2_IY_Top_Longi"), dEffStiff, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S4_2_IY_Top_LongiIg"), dEffStiffRat, Row_Ref, Col_Ref, stRow);
		}

		stRow += rowCnt + 1;
	}


	RptBlock = _T("[2]Effect_Sect_Trans");
	rowCnt = CopyRangeSubTitle(pExcel, RptBlock, stRow);
	stRow += rowCnt;

	if(enBehavior[1] == CANTILEVER)
	{
		RptBlock = _T("[2]Effect_Sect_Cantil_Trans");
		rowCnt = CopyRangeSubTitle(pExcel, RptBlock, stRow);
		GetCellIDByName(pExcel, RptBlock, Row_Ref, Col_Ref);

		if(ResultD[0][1].EvalD.bCheck)
		{
			double dEI = ResultD[0][1].MPhiD.StateD.Get_EI();
			double dEffStiff = m_dFactorStress * ResultD[0][1].ParamD.OptD.dEffStiff;
			double dEffStiffRat = ResultD[0][1].ParamD.OptD.dEffStiffRat;
			if(ResultD[0][1].ParamD.OptD.iEvalCode == KALIS_2023)
			{
				dEffStiff = dEI / dEc[1];
				dEffStiffRat = dEffStiff / ResultD[0][1].ParamD.SetcD.dIz;
			}
			PrintCellData(pExcel, _T("S4_2_EIY_Trans"), dEI, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S4_2_EC_Trans"), dEc[1], Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S4_2_IY_Trans"), dEffStiff, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S4_2_IY_TransIg"), dEffStiffRat, Row_Ref, Col_Ref, stRow);
		}

		stRow += rowCnt + 1;
	}
	else
	{
		RptBlock = _T("[2]Effect_Sect_Rahmen_Trans");
		rowCnt = CopyRangeSubTitle(pExcel, RptBlock, stRow);
		GetCellIDByName(pExcel, RptBlock, Row_Ref, Col_Ref);
		if(ResultD[0][1].EvalD.bCheck)
		{
			double dEI = ResultD[0][1].MPhiD.StateD.Get_EI();
			double dEffStiff = m_dFactorStress * ResultD[0][1].ParamD.OptD.dEffStiff;
			double dEffStiffRat = ResultD[0][1].ParamD.OptD.dEffStiffRat;
			if(ResultD[0][1].ParamD.OptD.iEvalCode == KALIS_2023)
			{
				dEffStiff = dEI / dEc[1];
				dEffStiffRat = dEffStiff / ResultD[0][1].ParamD.SetcD.dIz;
			}
			PrintCellData(pExcel, _T("S4_2_EIY_Bot_Trans"), dEI, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S4_2_EC_Bot_Trans"), dEc[1], Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S4_2_IY_Bot_Trans"), dEffStiff, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S4_2_IY_Bot_TransIg"), dEffStiffRat, Row_Ref, Col_Ref, stRow);
		}

		if(ResultD[1][1].EvalD.bCheck)
		{
			double dEI = ResultD[1][1].MPhiD.StateD.Get_EI();
			double dEffStiff = m_dFactorStress * ResultD[1][1].ParamD.OptD.dEffStiff;
			double dEffStiffRat = ResultD[1][1].ParamD.OptD.dEffStiffRat;
			if(ResultD[0][1].ParamD.OptD.iEvalCode == KALIS_2023)
			{
				dEffStiff = dEI / dEc[1];
				dEffStiffRat = dEffStiff / ResultD[1][1].ParamD.SetcD.dIz;
			}
			PrintCellData(pExcel, _T("S4_2_EIY_Top_Trans"), dEI, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S4_2_EC_Top_Trans"), dEc[1], Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S4_2_IY_Top_Trans"), dEffStiff, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S4_2_IY_Top_TransIg"), dEffStiffRat, Row_Ref, Col_Ref, stRow);
		}

		stRow += rowCnt + 1;
	}
	return TRUE;
}

BOOL CBrdgSeisEvalKISTEC2015_Report::GenerateEffectiveStiffJudgement(IExcel* pExcel, CString& RptBlock, int& stRow, const EN_BEHAVIOR_TYPE* enBehavior, T_BRDGEVAL_RESULT ResultD[2][2])
{
	// Only KALIS2023
	return TRUE;
}

BOOL CBrdgSeisEvalKISTEC2015_Report::GenerateBendingStress(IExcel* pExcel, CString& RptBlock, int& stRow, const EN_BEHAVIOR_TYPE* enBehavior, T_BRDGEVAL_RESULT ResultD[2][2])
{
	RptBlock = _T("[2]Bend_Perform_Curvature_Head");
	int rowCnt = CopyRangeSubTitle(pExcel, RptBlock, stRow);
	stRow += rowCnt;

	short Row_Ref = 0, Col_Ref = 0;

	RptBlock = _T("[2]Bend_Stress_Head");
	rowCnt = CopyRangeSubTitle(pExcel, RptBlock, stRow);
	stRow += rowCnt;


	RptBlock = _T("[2]Bend_Stress_Longi_Head");
	rowCnt = CopyRangeSubTitle(pExcel, RptBlock, stRow);
	stRow += rowCnt;
	if (enBehavior[0] == CANTILEVER)
	{
		RptBlock = _T("[2]Bend_Stress_Cantil_Longi");
		rowCnt = CopyRangeSubTitle(pExcel, RptBlock, stRow);
		GetCellIDByName(pExcel, RptBlock, Row_Ref, Col_Ref);

		if (ResultD[0][0].EvalD.bCheck)
		{
			PrintCellData(pExcel, _T("S5_1_Mom_Y_Longi"), ResultD[0][0].MPhiD.StateD.dMoment[4], Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S5_1_EHeight_Longi1"), ResultD[0][0].ParamD.OptD.dHe, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S5_1_BSY_Longi"), ResultD[0][0].CapaD.BendCapaD.dFy, Row_Ref, Col_Ref, stRow);

			PrintCellData(pExcel, _T("S5_1_Mom_U_Longi"), ResultD[0][0].MPhiD.StateD.dMoment[5], Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S5_1_EHeight_Longi2"), ResultD[0][0].ParamD.OptD.dHe, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S5_1_BSU_Longi"), ResultD[0][0].CapaD.BendCapaD.dFu, Row_Ref, Col_Ref, stRow);
		}

		stRow += rowCnt + 1;
	}
	else
	{
		RptBlock = _T("[2]Bend_Stress_Rahmen_Longi");
		rowCnt = CopyRangeSubTitle(pExcel, RptBlock, stRow);
		GetCellIDByName(pExcel, RptBlock, Row_Ref, Col_Ref);

		if (ResultD[0][0].EvalD.bCheck)
		{
			PrintCellData(pExcel, _T("S5_1_Mom_Y_Bot_Longi"), ResultD[0][0].MPhiD.StateD.dMoment[4], Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S5_1_EHeight_Bot_Longi1"), ResultD[0][0].ParamD.OptD.dHe, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S5_1_BSY_Bot_Longi"), ResultD[0][0].CapaD.BendCapaD.dFy, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S5_1_Mom_U_Bot_Longi"), ResultD[0][0].MPhiD.StateD.dMoment[5], Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S5_1_EHeight_Bot_Longi2"), ResultD[0][0].ParamD.OptD.dHe, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S5_1_BSU_Bot_Longi"), ResultD[0][0].CapaD.BendCapaD.dFu, Row_Ref, Col_Ref, stRow);
		}

		if (ResultD[1][0].EvalD.bCheck)
		{
			PrintCellData(pExcel, _T("S5_1_Mom_Y_Top_Longi"), ResultD[1][0].MPhiD.StateD.dMoment[4], Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S5_1_EHeight_Top_Longi1"), ResultD[1][0].ParamD.OptD.dHe, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S5_1_BSY_Top_Longi"), ResultD[1][0].CapaD.BendCapaD.dFy, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S5_1_Mom_U_Top_Longi"), ResultD[1][0].MPhiD.StateD.dMoment[5], Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S5_1_EHeight_Top_Longi2"), ResultD[1][0].ParamD.OptD.dHe, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S5_1_BSU_Top_Longi"), ResultD[1][0].CapaD.BendCapaD.dFu, Row_Ref, Col_Ref, stRow);
		}

		stRow += rowCnt + 1;
	}

	RptBlock = _T("[2]Bend_Stress_Trans_Head");
	rowCnt = CopyRangeSubTitle(pExcel, RptBlock, stRow);
	stRow += rowCnt;
	if (enBehavior[1] == CANTILEVER)
	{
		RptBlock = _T("[2]Bend_Stress_Cantil_Trans");
		rowCnt = CopyRangeSubTitle(pExcel, RptBlock, stRow);
		GetCellIDByName(pExcel, RptBlock, Row_Ref, Col_Ref);

		if (ResultD[0][1].EvalD.bCheck)
		{
			PrintCellData(pExcel, _T("S5_1_Mom_Y_Trans"), ResultD[0][1].MPhiD.StateD.dMoment[4], Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S5_1_EHeight_Trans1"), ResultD[0][1].ParamD.OptD.dHe, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S5_1_BSY_Trans"), ResultD[0][1].CapaD.BendCapaD.dFy, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S5_1_Mom_U_Trans"), ResultD[0][1].MPhiD.StateD.dMoment[5], Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S5_1_EHeight_Trans2"), ResultD[0][1].ParamD.OptD.dHe, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S5_1_BSU_Trans"), ResultD[0][1].CapaD.BendCapaD.dFu, Row_Ref, Col_Ref, stRow);
		}
		stRow += rowCnt + 1;
	}
	else
	{
		RptBlock = _T("[2]Bend_Stress_Rahmen_Trans");
		rowCnt = CopyRangeSubTitle(pExcel, RptBlock, stRow);
		GetCellIDByName(pExcel, RptBlock, Row_Ref, Col_Ref);

		if (ResultD[0][1].EvalD.bCheck)
		{
			PrintCellData(pExcel, _T("S5_1_Mom_Y_Bot_Trans"), ResultD[0][1].MPhiD.StateD.dMoment[4], Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S5_1_EHeight_Bot_Trans1"), ResultD[0][1].ParamD.OptD.dHe, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S5_1_BSY_Bot_Trans"), ResultD[0][1].CapaD.BendCapaD.dFy, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S5_1_Mom_U_Bot_Trans"), ResultD[0][1].MPhiD.StateD.dMoment[5], Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S5_1_EHeight_Bot_Trans2"), ResultD[0][1].ParamD.OptD.dHe, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S5_1_BSU_Bot_Trans"), ResultD[0][1].CapaD.BendCapaD.dFu, Row_Ref, Col_Ref, stRow);
		}

		if (ResultD[1][1].EvalD.bCheck)
		{
			PrintCellData(pExcel, _T("S5_1_Mom_Y_Top_Trans"), ResultD[1][1].MPhiD.StateD.dMoment[4], Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S5_1_EHeight_Top_Trans1"), ResultD[1][1].ParamD.OptD.dHe, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S5_1_BSY_Top_Trans"), ResultD[1][1].CapaD.BendCapaD.dFy, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S5_1_Mom_U_Top_Trans"), ResultD[1][1].MPhiD.StateD.dMoment[5], Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S5_1_EHeight_Top_Trans2"), ResultD[1][1].ParamD.OptD.dHe, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S5_1_BSU_Top_Trans"), ResultD[1][1].CapaD.BendCapaD.dFu, Row_Ref, Col_Ref, stRow);
		}
		stRow += rowCnt + 1;
	}

	RptBlock = _T("[2]Bend_Explanation");
	rowCnt = CopyRangeSubTitle(pExcel, RptBlock, stRow);
	stRow += rowCnt + 1;


	return TRUE;

}

BOOL CBrdgSeisEvalKISTEC2015_Report::GenerateBendingDuct2015(IExcel* pExcel, CString& RptBlock, int& stRow, const EN_BEHAVIOR_TYPE* enBehavior, T_BRDGEVAL_RESULT ResultD[2][2])
{

	short Row_Ref = 0, Col_Ref = 0;


	RptBlock = _T("[2]Disp_Duct_Head");
	int rowCnt = CopyRangeSubTitle(pExcel, RptBlock, stRow);
	stRow += rowCnt;

	if (enBehavior[0] == CANTILEVER)
	{

		RptBlock = _T("[2]Disp_Duct_Cantil_Longi");
		rowCnt = CopyRangeSubTitle(pExcel, RptBlock, stRow);
		GetCellIDByName(pExcel, RptBlock, Row_Ref, Col_Ref);

		if (ResultD[0][0].EvalD.bCheck)
		{
			PrintCellData(pExcel, _T("S5_2_Cur_Y_Longi1"), ResultD[0][0].MPhiD.StateD.dCurvature[4], Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S5_2_EHeight_Longi1"), ResultD[0][0].ParamD.OptD.dHe, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S5_2_Dis_Y_Longi1"), ResultD[0][0].CapaD.DispDuctD.dDeltay, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S5_2_Dis_Y_Longi1mm"), 1000 * ResultD[0][0].CapaD.DispDuctD.dDeltay, Row_Ref, Col_Ref, stRow);

			PrintCellData(pExcel, _T("S5_2_EHeight_Longi2"), ResultD[0][0].ParamD.OptD.dHe, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S5_2_FY_Longi1"), 0.001 * ResultD[0][0].ParamD.MatlD.dfy, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S5_2_DB_Longi1"), ResultD[0][0].ParamD.BarD.ddbl, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S5_2_LP_CALC_Longi"), ResultD[0][0].CapaD.DispDuctD.dLp_cal, Row_Ref, Col_Ref, stRow);

			PrintCellData(pExcel, _T("S5_2_FY_Longi2"), 0.001 * ResultD[0][0].ParamD.MatlD.dfy, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S5_2_DB_Longi2"), ResultD[0][0].ParamD.BarD.ddbl, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S5_2_LP_MIN_Longi"), ResultD[0][0].CapaD.DispDuctD.dLp_lim, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S5_2_LP_COMP_Longi"), Check_Sign(0, ResultD[0][0].CapaD.DispDuctD.dLp_cal, ResultD[0][0].CapaD.DispDuctD.dLp_lim), Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S5_2_LP_FIN_Longi1"), ResultD[0][0].CapaD.DispDuctD.dLp, Row_Ref, Col_Ref, stRow);

			PrintCellData(pExcel, _T("S5_2_Cur_U_Longi"), ResultD[0][0].MPhiD.StateD.dCurvature[5], Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S5_2_Cur_Y_Longi2"), ResultD[0][0].MPhiD.StateD.dCurvature[4], Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S5_2_LP_FIN_Longi2"), ResultD[0][0].CapaD.DispDuctD.dLp, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S5_2_AngleP_Longi1"), ResultD[0][0].CapaD.DispDuctD.dThetap, Row_Ref, Col_Ref, stRow);

			PrintCellData(pExcel, _T("S5_2_Mom_U_Longi"), ResultD[0][0].MPhiD.StateD.dMoment[5], Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S5_2_Mom_Y_Longi"), ResultD[0][0].MPhiD.StateD.dMoment[4], Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S5_2_Dis_Y_Longi2"), ResultD[0][0].CapaD.DispDuctD.dDeltay, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S5_2_AngleP_Longi2"), ResultD[0][0].CapaD.DispDuctD.dThetap, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S5_2_EHeight_Longi3"), ResultD[0][0].ParamD.OptD.dHe, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S5_2_LP_FIN_Longi3"), ResultD[0][0].CapaD.DispDuctD.dLp, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S5_2_Dis_P_Longi1"), ResultD[0][0].CapaD.DispDuctD.dDeltap, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S5_2_Dis_P_Longi1mm"), 1000 * ResultD[0][0].CapaD.DispDuctD.dDeltap, Row_Ref, Col_Ref, stRow);

			PrintCellData(pExcel, _T("S5_2_Dis_Y_Longi3"), ResultD[0][0].CapaD.DispDuctD.dDeltay, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S5_2_Dis_P_Longi2"), ResultD[0][0].CapaD.DispDuctD.dDeltap, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S5_2_Dis_U_Longi1"), ResultD[0][0].CapaD.DispDuctD.dDeltau, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S5_2_Dis_U_Longi1mm"), 1000 * ResultD[0][0].CapaD.DispDuctD.dDeltau, Row_Ref, Col_Ref, stRow);

			PrintCellData(pExcel, _T("S5_2_Dis_U_Longi2"), ResultD[0][0].CapaD.DispDuctD.dDeltau, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S5_2_Dis_Y_Longi4"), ResultD[0][0].CapaD.DispDuctD.dDeltay, Row_Ref, Col_Ref, stRow);
			if (ResultD[0][0].CapaD.BendCapaD.dDeltac != 0.0)
			{
				PrintCellData(pExcel, _T("S5_2_D_MAX_Longi"), ResultD[0][0].CapaD.DispDuctD.dmu_DeltaMax, Row_Ref, Col_Ref, stRow);
			}
			else
			{
				PrintCellData(pExcel, _T("S5_2_D_MAX_Longi"), _T("-"), Row_Ref, Col_Ref, stRow);
			}
			PrintCellData(pExcel, _T("S5_2_D_FIN_Longi"), min(ResultD[0][0].CapaD.DispDuctD.dmu_Deltac, ResultD[0][0].CapaD.DispDuctD.dmu_DeltaMax), Row_Ref, Col_Ref, stRow);
		}

		stRow += rowCnt;

	}
	else
	{
		RptBlock = _T("[2]Disp_Duct_Rahmen_Longi");
		rowCnt = CopyRangeSubTitle(pExcel, RptBlock, stRow);
		GetCellIDByName(pExcel, RptBlock, Row_Ref, Col_Ref);

		if (ResultD[0][1].EvalD.bCheck)
		{
			PrintCellData(pExcel, _T("S5_2_Cur_Y_Bot_Longi1"), ResultD[0][0].MPhiD.StateD.dCurvature[4], Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S5_2_EHeight_Bot_Longi1"), ResultD[0][0].ParamD.OptD.dHe, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S5_2_Dis_Y_Bot_Longi1"), ResultD[0][0].CapaD.DispDuctD.dDeltay, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S5_2_Dis_Y_Bot_Longi1mm"), 1000 * ResultD[0][0].CapaD.DispDuctD.dDeltay, Row_Ref, Col_Ref, stRow);

			PrintCellData(pExcel, _T("S5_2_EHeight_Bot_Longi2"), ResultD[0][0].ParamD.OptD.dHe, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S5_2_FY_Bot_Longi1"), 0.001 * ResultD[0][0].ParamD.MatlD.dfy, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S5_2_DB_Bot_Longi1"), ResultD[0][0].ParamD.BarD.ddbl, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S5_2_LP_CALC_Bot_Longi"), ResultD[0][0].CapaD.DispDuctD.dLp_cal, Row_Ref, Col_Ref, stRow);

			PrintCellData(pExcel, _T("S5_2_FY_Bot_Longi2"), 0.001 * ResultD[0][0].ParamD.MatlD.dfy, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S5_2_DB_Bot_Longi2"), ResultD[0][0].ParamD.BarD.ddbl, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S5_2_LP_MIN_Bot_Longi"), ResultD[0][0].CapaD.DispDuctD.dLp_lim, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S5_2_LP_COMP_Bot_Longi"), Check_Sign(0, ResultD[0][0].CapaD.DispDuctD.dLp_cal, ResultD[0][0].CapaD.DispDuctD.dLp_lim), Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S5_2_LP_FIN_Bot_Longi1"), ResultD[0][0].CapaD.DispDuctD.dLp, Row_Ref, Col_Ref, stRow);

			PrintCellData(pExcel, _T("S5_2_Cur_U_Bot_Longi"), ResultD[0][0].MPhiD.StateD.dCurvature[5], Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S5_2_Cur_Y_Bot_Longi2"), ResultD[0][0].MPhiD.StateD.dCurvature[4], Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S5_2_LP_FIN_Bot_Longi2"), ResultD[0][0].CapaD.DispDuctD.dLp, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S5_2_AngleP_Bot_Longi1"), ResultD[0][0].CapaD.DispDuctD.dThetap, Row_Ref, Col_Ref, stRow);

			PrintCellData(pExcel, _T("S5_2_Mom_U_Bot_Longi"), ResultD[0][0].MPhiD.StateD.dMoment[5], Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S5_2_Mom_Y_Bot_Longi"), ResultD[0][0].MPhiD.StateD.dMoment[4], Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S5_2_Dis_Y_Bot_Longi2"), ResultD[0][0].CapaD.DispDuctD.dDeltay, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S5_2_AngleP_Bot_Longi2"), ResultD[0][0].CapaD.DispDuctD.dThetap, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S5_2_EHeight_Bot_Longi3"), ResultD[0][0].ParamD.OptD.dHe, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S5_2_LP_FIN_Bot_Longi3"), ResultD[0][0].CapaD.DispDuctD.dLp, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S5_2_Dis_P_Bot_Longi1"), ResultD[0][0].CapaD.DispDuctD.dDeltap, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S5_2_Dis_P_Bot_Longi1mm"), 1000 * ResultD[0][0].CapaD.DispDuctD.dDeltap, Row_Ref, Col_Ref, stRow);

			PrintCellData(pExcel, _T("S5_2_Dis_Y_Bot_Longi3"), ResultD[0][0].CapaD.DispDuctD.dDeltay, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S5_2_Dis_P_Bot_Longi2"), ResultD[0][0].CapaD.DispDuctD.dDeltap, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S5_2_Dis_U_Bot_Longi1"), ResultD[0][0].CapaD.DispDuctD.dDeltau, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S5_2_Dis_U_Bot_Longi1mm"), 1000 * ResultD[0][0].CapaD.DispDuctD.dDeltau, Row_Ref, Col_Ref, stRow);

			PrintCellData(pExcel, _T("S5_2_Dis_U_Bot_Longi2"), ResultD[0][0].CapaD.DispDuctD.dDeltau, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S5_2_Dis_Y_Bot_Longi4"), ResultD[0][0].CapaD.DispDuctD.dDeltay, Row_Ref, Col_Ref, stRow);
			if (ResultD[0][0].CapaD.BendCapaD.dDeltac != 0.0)
			{
				PrintCellData(pExcel, _T("S5_2_D_MAX_Bot_Longi"), ResultD[0][0].CapaD.DispDuctD.dmu_DeltaMax, Row_Ref, Col_Ref, stRow);
			}
			else
			{
				PrintCellData(pExcel, _T("S5_2_D_MAX_Bot_Longi"), _T("-"), Row_Ref, Col_Ref, stRow);
			}

			PrintCellData(pExcel, _T("S5_2_D_FIN_Bot_Longi"), min(ResultD[0][0].CapaD.DispDuctD.dmu_Deltac, ResultD[0][0].CapaD.DispDuctD.dmu_DeltaMax), Row_Ref, Col_Ref, stRow);
		}

		if (ResultD[1][0].EvalD.bCheck)
		{
			PrintCellData(pExcel, _T("S5_2_Cur_Y_Top_Longi1"), ResultD[1][0].MPhiD.StateD.dCurvature[4], Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S5_2_EHeight_Top_Longi1"), ResultD[1][0].ParamD.OptD.dHe, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S5_2_Dis_Y_Top_Longi1"), ResultD[1][0].CapaD.DispDuctD.dDeltay, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S5_2_Dis_Y_Top_Longi1mm"), 1000 * ResultD[1][0].CapaD.DispDuctD.dDeltay, Row_Ref, Col_Ref, stRow);

			PrintCellData(pExcel, _T("S5_2_EHeight_Top_Longi2"), ResultD[1][0].ParamD.OptD.dHe, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S5_2_FY_Top_Longi1"), 0.001 * ResultD[1][0].ParamD.MatlD.dfy, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S5_2_DB_Top_Longi1"), ResultD[1][0].ParamD.BarD.ddbl, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S5_2_LP_CALC_Top_Longi"), ResultD[1][0].CapaD.DispDuctD.dLp_cal, Row_Ref, Col_Ref, stRow);

			PrintCellData(pExcel, _T("S5_2_FY_Top_Longi2"), 0.001 * ResultD[1][0].ParamD.MatlD.dfy, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S5_2_DB_Top_Longi2"), ResultD[1][0].ParamD.BarD.ddbl, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S5_2_LP_MIN_Top_Longi"), ResultD[1][0].CapaD.DispDuctD.dLp_lim, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S5_2_LP_COMP_Top_Longi"), Check_Sign(0, ResultD[1][0].CapaD.DispDuctD.dLp_cal, ResultD[1][0].CapaD.DispDuctD.dLp_lim), Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S5_2_LP_FIN_Top_Longi1"), ResultD[1][0].CapaD.DispDuctD.dLp, Row_Ref, Col_Ref, stRow);

			PrintCellData(pExcel, _T("S5_2_Cur_U_Top_Longi"), ResultD[1][0].MPhiD.StateD.dCurvature[5], Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S5_2_Cur_Y_Top_Longi2"), ResultD[1][0].MPhiD.StateD.dCurvature[4], Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S5_2_LP_FIN_Top_Longi2"), ResultD[1][0].CapaD.DispDuctD.dLp, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S5_2_AngleP_Top_Longi1"), ResultD[1][0].CapaD.DispDuctD.dThetap, Row_Ref, Col_Ref, stRow);

			PrintCellData(pExcel, _T("S5_2_Mom_U_Top_Longi"), ResultD[1][0].MPhiD.StateD.dMoment[5], Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S5_2_Mom_Y_Top_Longi"), ResultD[1][0].MPhiD.StateD.dMoment[4], Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S5_2_Dis_Y_Top_Longi2"), ResultD[1][0].CapaD.DispDuctD.dDeltay, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S5_2_AngleP_Top_Longi2"), ResultD[1][0].CapaD.DispDuctD.dThetap, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S5_2_EHeight_Top_Longi3"), ResultD[1][0].ParamD.OptD.dHe, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S5_2_LP_FIN_Top_Longi3"), ResultD[1][0].CapaD.DispDuctD.dLp, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S5_2_Dis_P_Top_Longi1"), ResultD[1][0].CapaD.DispDuctD.dDeltap, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S5_2_Dis_P_Top_Longi1mm"), 1000 * ResultD[1][0].CapaD.DispDuctD.dDeltap, Row_Ref, Col_Ref, stRow);

			PrintCellData(pExcel, _T("S5_2_Dis_Y_Top_Longi3"), ResultD[1][0].CapaD.DispDuctD.dDeltay, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S5_2_Dis_P_Top_Longi2"), ResultD[1][0].CapaD.DispDuctD.dDeltap, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S5_2_Dis_U_Top_Longi1"), ResultD[1][0].CapaD.DispDuctD.dDeltau, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S5_2_Dis_U_Top_Longi1mm"), 1000 * ResultD[1][0].CapaD.DispDuctD.dDeltau, Row_Ref, Col_Ref, stRow);

			PrintCellData(pExcel, _T("S5_2_Dis_U_Top_Longi2"), ResultD[1][0].CapaD.DispDuctD.dDeltau, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S5_2_Dis_Y_Top_Longi4"), ResultD[1][0].CapaD.DispDuctD.dDeltay, Row_Ref, Col_Ref, stRow);

			if (ResultD[1][0].CapaD.BendCapaD.dDeltac != 0.0)
			{
				PrintCellData(pExcel, _T("S5_2_D_MAX_Top_Longi"), ResultD[1][0].CapaD.DispDuctD.dmu_DeltaMax, Row_Ref, Col_Ref, stRow);
			}
			else
			{
				PrintCellData(pExcel, _T("S5_2_D_MAX_Top_Longi"), _T("-"), Row_Ref, Col_Ref, stRow);
			}

			PrintCellData(pExcel, _T("S5_2_D_FIN_Top_Longi"), min(ResultD[1][0].CapaD.DispDuctD.dmu_Deltac, ResultD[1][0].CapaD.DispDuctD.dmu_DeltaMax), Row_Ref, Col_Ref, stRow);
		}

		stRow += rowCnt;
	}

	if (enBehavior[1] == CANTILEVER)
	{
		RptBlock = _T("[2]Disp_Duct_Cantil_Trans");
		rowCnt = CopyRangeSubTitle(pExcel, RptBlock, stRow);
		GetCellIDByName(pExcel, RptBlock, Row_Ref, Col_Ref);

		if (ResultD[0][1].EvalD.bCheck)
		{
			PrintCellData(pExcel, _T("S5_2_Cur_Y_Trans1"), ResultD[0][1].MPhiD.StateD.dCurvature[4], Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S5_2_EHeight_Trans1"), ResultD[0][1].ParamD.OptD.dHe, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S5_2_Dis_Y_Trans1"), ResultD[0][1].CapaD.DispDuctD.dDeltay, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S5_2_Dis_Y_Trans1mm"), 1000 * ResultD[0][1].CapaD.DispDuctD.dDeltay, Row_Ref, Col_Ref, stRow);

			PrintCellData(pExcel, _T("S5_2_EHeight_Trans2"), ResultD[0][1].ParamD.OptD.dHe, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S5_2_FY_Trans1"), 0.001 * ResultD[0][1].ParamD.MatlD.dfy, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S5_2_DB_Trans1"), ResultD[0][1].ParamD.BarD.ddbl, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S5_2_LP_CALC_Trans"), ResultD[0][1].CapaD.DispDuctD.dLp_cal, Row_Ref, Col_Ref, stRow);

			PrintCellData(pExcel, _T("S5_2_FY_Trans2"), 0.001 * ResultD[0][1].ParamD.MatlD.dfy, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S5_2_DB_Trans2"), ResultD[0][1].ParamD.BarD.ddbl, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S5_2_LP_MIN_Trans"), ResultD[0][1].CapaD.DispDuctD.dLp_lim, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S5_2_LP_COMP_Trans"), Check_Sign(0, ResultD[0][1].CapaD.DispDuctD.dLp_cal, ResultD[0][1].CapaD.DispDuctD.dLp_lim), Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S5_2_LP_FIN_Trans1"), ResultD[0][1].CapaD.DispDuctD.dLp, Row_Ref, Col_Ref, stRow);

			PrintCellData(pExcel, _T("S5_2_Cur_U_Trans"), ResultD[0][1].MPhiD.StateD.dCurvature[5], Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S5_2_Cur_Y_Trans2"), ResultD[0][1].MPhiD.StateD.dCurvature[4], Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S5_2_LP_FIN_Trans2"), ResultD[0][1].CapaD.DispDuctD.dLp, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S5_2_AngleP_Trans1"), ResultD[0][1].CapaD.DispDuctD.dThetap, Row_Ref, Col_Ref, stRow);

			PrintCellData(pExcel, _T("S5_2_Mom_U_Trans"), ResultD[0][1].MPhiD.StateD.dMoment[5], Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S5_2_Mom_Y_Trans"), ResultD[0][1].MPhiD.StateD.dMoment[4], Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S5_2_Dis_Y_Trans2"), ResultD[0][1].CapaD.DispDuctD.dDeltay, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S5_2_AngleP_Trans2"), ResultD[0][1].CapaD.DispDuctD.dThetap, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S5_2_EHeight_Trans3"), ResultD[0][1].ParamD.OptD.dHe, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S5_2_LP_FIN_Trans3"), ResultD[0][1].CapaD.DispDuctD.dLp, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S5_2_Dis_P_Trans1"), ResultD[0][1].CapaD.DispDuctD.dDeltap, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S5_2_Dis_P_Trans1mm"), 1000 * ResultD[0][1].CapaD.DispDuctD.dDeltap, Row_Ref, Col_Ref, stRow);

			PrintCellData(pExcel, _T("S5_2_Dis_Y_Trans3"), ResultD[0][1].CapaD.DispDuctD.dDeltay, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S5_2_Dis_P_Trans2"), ResultD[0][1].CapaD.DispDuctD.dDeltap, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S5_2_Dis_U_Trans1"), ResultD[0][1].CapaD.DispDuctD.dDeltau, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S5_2_Dis_U_Trans1mm"), 1000 * ResultD[0][1].CapaD.DispDuctD.dDeltau, Row_Ref, Col_Ref, stRow);

			PrintCellData(pExcel, _T("S5_2_Dis_U_Trans2"), ResultD[0][1].CapaD.DispDuctD.dDeltau, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S5_2_Dis_Y_Trans4"), ResultD[0][1].CapaD.DispDuctD.dDeltay, Row_Ref, Col_Ref, stRow);

			if (ResultD[0][1].CapaD.BendCapaD.dDeltac != 0.0)
			{
				PrintCellData(pExcel, _T("S5_2_D_MAX_Trans"), ResultD[0][1].CapaD.DispDuctD.dmu_DeltaMax, Row_Ref, Col_Ref, stRow);
			}
			else
			{
				PrintCellData(pExcel, _T("S5_2_D_MAX_Trans"), _T("-"), Row_Ref, Col_Ref, stRow);
			}

			PrintCellData(pExcel, _T("S5_2_D_FIN_Trans"), min(ResultD[0][1].CapaD.DispDuctD.dmu_Deltac, ResultD[0][1].CapaD.DispDuctD.dmu_DeltaMax), Row_Ref, Col_Ref, stRow);
		}

		stRow += rowCnt;

	}
	else
	{
		RptBlock = _T("[2]Disp_Duct_Rahmen_Trans");
		rowCnt = CopyRangeSubTitle(pExcel, RptBlock, stRow);
		GetCellIDByName(pExcel, RptBlock, Row_Ref, Col_Ref);

		if (ResultD[0][1].EvalD.bCheck)
		{
			PrintCellData(pExcel, _T("S5_2_Cur_Y_Bot_Trans1"), ResultD[0][1].MPhiD.StateD.dCurvature[4], Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S5_2_EHeight_Bot_Trans1"), ResultD[0][1].ParamD.OptD.dHe, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S5_2_Dis_Y_Bot_Trans1"), ResultD[0][1].CapaD.DispDuctD.dDeltay, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S5_2_Dis_Y_Bot_Trans1mm"), 1000 * ResultD[0][1].CapaD.DispDuctD.dDeltay, Row_Ref, Col_Ref, stRow);

			PrintCellData(pExcel, _T("S5_2_EHeight_Bot_Trans2"), ResultD[0][1].ParamD.OptD.dHe, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S5_2_FY_Bot_Trans1"), 0.001 * ResultD[0][1].ParamD.MatlD.dfy, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S5_2_DB_Bot_Trans1"), ResultD[0][1].ParamD.BarD.ddbl, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S5_2_LP_CALC_Bot_Trans"), ResultD[0][1].CapaD.DispDuctD.dLp_cal, Row_Ref, Col_Ref, stRow);

			PrintCellData(pExcel, _T("S5_2_FY_Bot_Trans2"), 0.001 * ResultD[0][1].ParamD.MatlD.dfy, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S5_2_DB_Bot_Trans2"), ResultD[0][1].ParamD.BarD.ddbl, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S5_2_LP_MIN_Bot_Trans"), ResultD[0][1].CapaD.DispDuctD.dLp_lim, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S5_2_LP_COMP_Bot_Trans"), Check_Sign(0, ResultD[0][1].CapaD.DispDuctD.dLp_cal, ResultD[0][1].CapaD.DispDuctD.dLp_lim), Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S5_2_LP_FIN_Bot_Trans1"), ResultD[0][1].CapaD.DispDuctD.dLp, Row_Ref, Col_Ref, stRow);

			PrintCellData(pExcel, _T("S5_2_Cur_U_Bot_Trans"), ResultD[0][1].MPhiD.StateD.dCurvature[5], Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S5_2_Cur_Y_Bot_Trans2"), ResultD[0][1].MPhiD.StateD.dCurvature[4], Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S5_2_LP_FIN_Bot_Trans2"), ResultD[0][1].CapaD.DispDuctD.dLp, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S5_2_AngleP_Bot_Trans1"), ResultD[0][1].CapaD.DispDuctD.dThetap, Row_Ref, Col_Ref, stRow);

			PrintCellData(pExcel, _T("S5_2_Mom_U_Bot_Trans"), ResultD[0][1].MPhiD.StateD.dMoment[5], Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S5_2_Mom_Y_Bot_Trans"), ResultD[0][1].MPhiD.StateD.dMoment[4], Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S5_2_Dis_Y_Bot_Trans2"), ResultD[0][1].CapaD.DispDuctD.dDeltay, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S5_2_AngleP_Bot_Trans2"), ResultD[0][1].CapaD.DispDuctD.dThetap, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S5_2_EHeight_Bot_Trans3"), ResultD[0][1].ParamD.OptD.dHe, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S5_2_LP_FIN_Bot_Trans3"), ResultD[0][1].CapaD.DispDuctD.dLp, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S5_2_Dis_P_Bot_Trans1"), ResultD[0][1].CapaD.DispDuctD.dDeltap, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S5_2_Dis_P_Bot_Trans1mm"), 1000 * ResultD[0][1].CapaD.DispDuctD.dDeltap, Row_Ref, Col_Ref, stRow);

			PrintCellData(pExcel, _T("S5_2_Dis_Y_Bot_Trans3"), ResultD[0][1].CapaD.DispDuctD.dDeltay, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S5_2_Dis_P_Bot_Trans2"), ResultD[0][1].CapaD.DispDuctD.dDeltap, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S5_2_Dis_U_Bot_Trans1"), ResultD[0][1].CapaD.DispDuctD.dDeltau, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S5_2_Dis_U_Bot_Trans1mm"), 1000 * ResultD[0][1].CapaD.DispDuctD.dDeltau, Row_Ref, Col_Ref, stRow);

			PrintCellData(pExcel, _T("S5_2_Dis_U_Bot_Trans2"), ResultD[0][1].CapaD.DispDuctD.dDeltau, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S5_2_Dis_Y_Bot_Trans4"), ResultD[0][1].CapaD.DispDuctD.dDeltay, Row_Ref, Col_Ref, stRow);

			if (ResultD[0][1].CapaD.BendCapaD.dDeltac != 0.0)
			{
				PrintCellData(pExcel, _T("S5_2_D_MAX_Bot_Trans"), ResultD[0][1].CapaD.DispDuctD.dmu_DeltaMax, Row_Ref, Col_Ref, stRow);
			}
			else
			{
				PrintCellData(pExcel, _T("S5_2_D_MAX_Bot_Trans"), _T("-"), Row_Ref, Col_Ref, stRow);
			}

			PrintCellData(pExcel, _T("S5_2_D_FIN_Bot_Trans"), min(ResultD[0][1].CapaD.DispDuctD.dmu_Deltac, ResultD[0][1].CapaD.DispDuctD.dmu_DeltaMax), Row_Ref, Col_Ref, stRow);
		}

		if (ResultD[1][1].EvalD.bCheck)
		{
			PrintCellData(pExcel, _T("S5_2_Cur_Y_Top_Trans1"), ResultD[1][1].MPhiD.StateD.dCurvature[4], Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S5_2_EHeight_Top_Trans1"), ResultD[1][1].ParamD.OptD.dHe, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S5_2_Dis_Y_Top_Trans1"), ResultD[1][1].CapaD.DispDuctD.dDeltay, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S5_2_Dis_Y_Top_Trans1mm"), 1000 * ResultD[1][1].CapaD.DispDuctD.dDeltay, Row_Ref, Col_Ref, stRow);

			PrintCellData(pExcel, _T("S5_2_EHeight_Top_Trans2"), ResultD[1][1].ParamD.OptD.dHe, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S5_2_FY_Top_Trans1"), 0.001 * ResultD[1][1].ParamD.MatlD.dfy, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S5_2_DB_Top_Trans1"), ResultD[1][1].ParamD.BarD.ddbl, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S5_2_LP_CALC_Top_Trans"), ResultD[1][1].CapaD.DispDuctD.dLp_cal, Row_Ref, Col_Ref, stRow);

			PrintCellData(pExcel, _T("S5_2_FY_Top_Trans2"), 0.001 * ResultD[1][1].ParamD.MatlD.dfy, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S5_2_DB_Top_Trans2"), ResultD[1][1].ParamD.BarD.ddbl, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S5_2_LP_MIN_Top_Trans"), ResultD[1][1].CapaD.DispDuctD.dLp_lim, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S5_2_LP_COMP_Top_Trans"), Check_Sign(0, ResultD[1][1].CapaD.DispDuctD.dLp_cal, ResultD[1][1].CapaD.DispDuctD.dLp_lim), Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S5_2_LP_FIN_Top_Trans1"), ResultD[1][1].CapaD.DispDuctD.dLp, Row_Ref, Col_Ref, stRow);

			PrintCellData(pExcel, _T("S5_2_Cur_U_Top_Trans"), ResultD[1][1].MPhiD.StateD.dCurvature[5], Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S5_2_Cur_Y_Top_Trans2"), ResultD[1][1].MPhiD.StateD.dCurvature[4], Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S5_2_LP_FIN_Top_Trans2"), ResultD[1][1].CapaD.DispDuctD.dLp, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S5_2_AngleP_Top_Trans1"), ResultD[1][1].CapaD.DispDuctD.dThetap, Row_Ref, Col_Ref, stRow);

			PrintCellData(pExcel, _T("S5_2_Mom_U_Top_Trans"), ResultD[1][1].MPhiD.StateD.dMoment[5], Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S5_2_Mom_Y_Top_Trans"), ResultD[1][1].MPhiD.StateD.dMoment[4], Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S5_2_Dis_Y_Top_Trans2"), ResultD[1][1].CapaD.DispDuctD.dDeltay, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S5_2_AngleP_Top_Trans2"), ResultD[1][1].CapaD.DispDuctD.dThetap, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S5_2_EHeight_Top_Trans3"), ResultD[1][1].ParamD.OptD.dHe, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S5_2_LP_FIN_Top_Trans3"), ResultD[1][1].CapaD.DispDuctD.dLp, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S5_2_Dis_P_Top_Trans1"), ResultD[1][1].CapaD.DispDuctD.dDeltap, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S5_2_Dis_P_Top_Trans1mm"), 1000 * ResultD[1][1].CapaD.DispDuctD.dDeltap, Row_Ref, Col_Ref, stRow);

			PrintCellData(pExcel, _T("S5_2_Dis_Y_Top_Trans3"), ResultD[1][1].CapaD.DispDuctD.dDeltay, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S5_2_Dis_P_Top_Trans2"), ResultD[1][1].CapaD.DispDuctD.dDeltap, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S5_2_Dis_U_Top_Trans1"), ResultD[1][1].CapaD.DispDuctD.dDeltau, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S5_2_Dis_U_Top_Trans1mm"), 1000 * ResultD[1][1].CapaD.DispDuctD.dDeltau, Row_Ref, Col_Ref, stRow);

			PrintCellData(pExcel, _T("S5_2_Dis_U_Top_Trans2"), ResultD[1][1].CapaD.DispDuctD.dDeltau, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S5_2_Dis_Y_Top_Trans4"), ResultD[1][1].CapaD.DispDuctD.dDeltay, Row_Ref, Col_Ref, stRow);

			if (ResultD[1][1].CapaD.BendCapaD.dDeltac != 0.0)
			{
				PrintCellData(pExcel, _T("S5_2_D_MAX_Top_Trans"), ResultD[1][1].CapaD.DispDuctD.dmu_DeltaMax, Row_Ref, Col_Ref, stRow);
			}
			else
			{
				PrintCellData(pExcel, _T("S5_2_D_MAX_Top_Trans"), _T("-"), Row_Ref, Col_Ref, stRow);
			}

			PrintCellData(pExcel, _T("S5_2_D_FIN_Top_Trans"), min(ResultD[1][1].CapaD.DispDuctD.dmu_Deltac, ResultD[1][1].CapaD.DispDuctD.dmu_DeltaMax), Row_Ref, Col_Ref, stRow);
		}

		stRow += rowCnt;

	}

	return TRUE;

}

BOOL CBrdgSeisEvalKISTEC2015_Report::GenerateBendingDuct2012(IExcel* pExcel, CString& RptBlock, int& stRow, const EN_BEHAVIOR_TYPE* enBehavior, T_BRDGEVAL_RESULT ResultD[2][2])
{

	short Row_Ref = 0, Col_Ref = 0;

	RptBlock = _T("[2]Disp_Duct_Head2");
	int rowCnt = CopyRangeSubTitle(pExcel, RptBlock, stRow);
	stRow += rowCnt;


	if (enBehavior[0] == CANTILEVER)
	{
		RptBlock = _T("[2]Disp_Duct_Cantil_Longi2");
		rowCnt = CopyRangeSubTitle(pExcel, RptBlock, stRow);
		GetCellIDByName(pExcel, RptBlock, Row_Ref, Col_Ref);

		if (ResultD[0][0].EvalD.bCheck)
		{
			PrintCellData(pExcel, _T("S5_2_2_Cur_Y_Longi1"), ResultD[0][0].MPhiD.StateD.dCurvature[4], Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S5_2_2_EHeight_Longi"), ResultD[0][0].ParamD.OptD.dHe, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S5_2_2_Dis_Y_Longi1"), ResultD[0][0].CapaD.DispDuctD.dDeltay, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S5_2_2_Dis_Y_Longi1mm"), 1000 * ResultD[0][0].CapaD.DispDuctD.dDeltay, Row_Ref, Col_Ref, stRow);

			PrintCellData(pExcel, _T("S5_2_2_Cur_U_Longi"), ResultD[0][0].MPhiD.StateD.dCurvature[5], Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S5_2_2_Cur_Y_Longi2"), ResultD[0][0].MPhiD.StateD.dCurvature[4], Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S5_2_2_Cur_RAT_Longi1"), ResultD[0][0].CapaD.BendCapaD.dmu_phi, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S5_2_2_D_Longi1"), ResultD[0][0].ParamD.SetcD.dH, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S5_2_2_L_Longi1"), ResultD[0][0].ParamD.OptD.dHe, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S5_2_2_D_Longi2"), ResultD[0][0].ParamD.SetcD.dH, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S5_2_2_L_Longi2"), ResultD[0][0].ParamD.OptD.dHe, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S5_2_2_Cur_RAT_Longi2"), ResultD[0][0].CapaD.BendCapaD.dmu_phi, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S5_2_2_Disp_Duct_Longi1"), ResultD[0][0].CapaD.BendCapaD.dmu_delta, Row_Ref, Col_Ref, stRow);

			PrintCellData(pExcel, _T("S5_2_2_Dis_Y_Longi2"), ResultD[0][0].CapaD.BendCapaD.dDeltay, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S5_2_2_DispDuct_Longi2"), ResultD[0][0].CapaD.BendCapaD.dmu_delta, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S5_2_2_Dis_U_Longi"), ResultD[0][0].CapaD.BendCapaD.dDeltau, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S5_2_2_Dis_U_Longimm"), 10000 * ResultD[0][0].CapaD.BendCapaD.dDeltau, Row_Ref, Col_Ref, stRow);
		}

		stRow += rowCnt;
	}
	else
	{
		RptBlock = _T("[2]Disp_Duct_Rahmen_Longi2");
		rowCnt = CopyRangeSubTitle(pExcel, RptBlock, stRow);
		GetCellIDByName(pExcel, RptBlock, Row_Ref, Col_Ref);

		if (ResultD[0][0].EvalD.bCheck)
		{
			PrintCellData(pExcel, _T("S5_2_2_Cur_Y_Bot_Longi1"), ResultD[0][0].MPhiD.StateD.dCurvature[4], Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S5_2_2_EHeight_Bot_Longi"), ResultD[0][0].ParamD.OptD.dHe, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S5_2_2_Dis_Y_Bot_Longi1"), ResultD[0][0].CapaD.DispDuctD.dDeltay, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S5_2_2_Dis_Y_Bot_Longi1mm"), 1000 * ResultD[0][0].CapaD.DispDuctD.dDeltay, Row_Ref, Col_Ref, stRow);

			PrintCellData(pExcel, _T("S5_2_2_Cur_U_Bot_Longi"), ResultD[0][0].MPhiD.StateD.dCurvature[5], Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S5_2_2_Cur_Y_Bot_Longi2"), ResultD[0][0].MPhiD.StateD.dCurvature[4], Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S5_2_2_Cur_RAT_Bot_Longi1"), ResultD[0][0].CapaD.BendCapaD.dmu_phi, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S5_2_2_D_Bot_Longi1"), ResultD[0][0].ParamD.SetcD.dH, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S5_2_2_L_Bot_Longi1"), ResultD[0][0].ParamD.OptD.dHe, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S5_2_2_D_Bot_Longi2"), ResultD[0][0].ParamD.SetcD.dH, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S5_2_2_L_Bot_Longi2"), ResultD[0][0].ParamD.OptD.dHe, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S5_2_2_Cur_RAT_Bot_Longi2"), ResultD[0][0].CapaD.BendCapaD.dmu_phi, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S5_2_2_Disp_Duct_Bot_Longi1"), ResultD[0][0].CapaD.BendCapaD.dmu_delta, Row_Ref, Col_Ref, stRow);

			PrintCellData(pExcel, _T("S5_2_2_Dis_Y_Bot_Longi2"), ResultD[0][0].CapaD.BendCapaD.dDeltay, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S5_2_2_DispDuct_Bot_Longi2"), ResultD[0][0].CapaD.BendCapaD.dmu_delta, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S5_2_2_Dis_U_Bot_Longi"), ResultD[0][0].CapaD.BendCapaD.dDeltau, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S5_2_2_Dis_U_Bot_Longimm"), 1000 * ResultD[0][0].CapaD.BendCapaD.dDeltau, Row_Ref, Col_Ref, stRow);
		}

		if (ResultD[1][0].EvalD.bCheck)
		{
			PrintCellData(pExcel, _T("S5_2_2_Cur_Y_Top_Longi1"), ResultD[1][0].MPhiD.StateD.dCurvature[4], Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S5_2_2_EHeight_Top_Longi"), ResultD[1][0].ParamD.OptD.dHe, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S5_2_2_Dis_Y_Top_Longi1"), ResultD[1][0].CapaD.DispDuctD.dDeltay, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S5_2_2_Dis_Y_Top_Longi1mm"), 1000 * ResultD[1][0].CapaD.DispDuctD.dDeltay, Row_Ref, Col_Ref, stRow);

			PrintCellData(pExcel, _T("S5_2_2_Cur_U_Top_Longi"), ResultD[1][0].MPhiD.StateD.dCurvature[5], Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S5_2_2_Cur_Y_Top_Longi2"), ResultD[1][0].MPhiD.StateD.dCurvature[4], Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S5_2_2_Cur_RAT_Top_Longi1"), ResultD[1][0].CapaD.BendCapaD.dmu_phi, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S5_2_2_D_Top_Longi1"), ResultD[1][0].ParamD.SetcD.dH, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S5_2_2_L_Top_Longi1"), ResultD[1][0].ParamD.OptD.dHe, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S5_2_2_D_Top_Longi2"), ResultD[1][0].ParamD.SetcD.dH, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S5_2_2_L_Top_Longi2"), ResultD[1][0].ParamD.OptD.dHe, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S5_2_2_Cur_RAT_Top_Longi2"), ResultD[1][0].CapaD.BendCapaD.dmu_phi, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S5_2_2_Disp_Duct_Top_Longi1"), ResultD[1][0].CapaD.BendCapaD.dmu_delta, Row_Ref, Col_Ref, stRow);

			PrintCellData(pExcel, _T("S5_2_2_Dis_Y_Top_Longi2"), ResultD[1][0].CapaD.BendCapaD.dDeltay, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S5_2_2_DispDuct_Top_Longi2"), ResultD[1][0].CapaD.BendCapaD.dmu_delta, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S5_2_2_Dis_U_Top_Longi"), ResultD[1][0].CapaD.BendCapaD.dDeltau, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S5_2_2_Dis_U_Top_Longimm"), 1000 * ResultD[1][0].CapaD.BendCapaD.dDeltau, Row_Ref, Col_Ref, stRow);
		}

		stRow += rowCnt;

	}

	if (enBehavior[1] == CANTILEVER)
	{
		RptBlock = _T("[2]Disp_Duct_Cantil_Trans2");
		rowCnt = CopyRangeSubTitle(pExcel, RptBlock, stRow);
		GetCellIDByName(pExcel, RptBlock, Row_Ref, Col_Ref);

		if (ResultD[0][1].EvalD.bCheck)
		{
			PrintCellData(pExcel, _T("S5_2_2_Cur_Y_Trans1"), ResultD[0][1].MPhiD.StateD.dCurvature[4], Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S5_2_2_EHeight_Trans"), ResultD[0][1].ParamD.OptD.dHe, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S5_2_2_Dis_Y_Trans1"), ResultD[0][1].CapaD.DispDuctD.dDeltay, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S5_2_2_Dis_Y_Trans1mm"), 1000 * ResultD[0][1].CapaD.DispDuctD.dDeltay, Row_Ref, Col_Ref, stRow);

			PrintCellData(pExcel, _T("S5_2_2_Cur_U_Trans"), ResultD[0][1].MPhiD.StateD.dCurvature[5], Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S5_2_2_Cur_Y_Trans2"), ResultD[0][1].MPhiD.StateD.dCurvature[4], Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S5_2_2_Cur_RAT_Trans1"), ResultD[0][1].CapaD.BendCapaD.dmu_phi, Row_Ref, Col_Ref, stRow);
			if (ResultD[0][1].ParamD.SetcD.strShape == _T("P") || ResultD[0][1].ParamD.SetcD.strShape == _T("SR"))
			{
				PrintCellData(pExcel, _T("S5_2_2_D_Trans1"), ResultD[0][1].ParamD.SetcD.dH, Row_Ref, Col_Ref, stRow);
			}
			else
			{
				PrintCellData(pExcel, _T("S5_2_2_D_Trans1"), ResultD[0][1].ParamD.SetcD.dB, Row_Ref, Col_Ref, stRow);
			}
			PrintCellData(pExcel, _T("S5_2_2_L_Trans1"), ResultD[0][1].ParamD.OptD.dHe, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S5_2_2_D_Trans2"), ResultD[0][1].ParamD.SetcD.dH, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S5_2_2_L_Trans2"), ResultD[0][1].ParamD.OptD.dHe, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S5_2_2_Cur_RAT_Trans2"), ResultD[0][1].CapaD.BendCapaD.dmu_phi, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S5_2_2_Disp_Duct_Trans1"), ResultD[0][1].CapaD.BendCapaD.dmu_delta, Row_Ref, Col_Ref, stRow);

			PrintCellData(pExcel, _T("S5_2_2_Dis_Y_Trans2"), ResultD[0][1].CapaD.BendCapaD.dDeltay, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S5_2_2_DispDuct_Trans2"), ResultD[0][1].CapaD.BendCapaD.dmu_delta, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S5_2_2_Dis_U_Trans"), ResultD[0][1].CapaD.BendCapaD.dDeltau, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S5_2_2_Dis_U_Transmm"), 1000 * ResultD[0][1].CapaD.BendCapaD.dDeltau, Row_Ref, Col_Ref, stRow);
		}

		stRow += rowCnt;
	}
	else
	{
		RptBlock = _T("[2]Disp_Duct_Rahmen_Trans2");
		rowCnt = CopyRangeSubTitle(pExcel, RptBlock, stRow);
		GetCellIDByName(pExcel, RptBlock, Row_Ref, Col_Ref);

		if (ResultD[0][1].EvalD.bCheck)
		{
			PrintCellData(pExcel, _T("S5_2_2_Cur_Y_Bot_Trans1"), ResultD[0][1].MPhiD.StateD.dCurvature[4], Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S5_2_2_EHeight_Bot_Trans"), ResultD[0][1].ParamD.OptD.dHe, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S5_2_2_Dis_Y_Bot_Trans1"), ResultD[0][1].CapaD.DispDuctD.dDeltay, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S5_2_2_Dis_Y_Bot_Trans1mm"), 1000 * ResultD[0][1].CapaD.DispDuctD.dDeltay, Row_Ref, Col_Ref, stRow);

			PrintCellData(pExcel, _T("S5_2_2_Cur_U_Bot_Trans"), ResultD[0][1].MPhiD.StateD.dCurvature[5], Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S5_2_2_Cur_Y_Bot_Trans2"), ResultD[0][1].MPhiD.StateD.dCurvature[4], Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S5_2_2_Cur_RAT_Bot_Trans1"), ResultD[0][1].CapaD.BendCapaD.dmu_phi, Row_Ref, Col_Ref, stRow);
			if (ResultD[0][1].ParamD.SetcD.strShape == _T("P") || ResultD[0][1].ParamD.SetcD.strShape == _T("SR"))
			{
				PrintCellData(pExcel, _T("S5_2_2_D_Bot_Trans1"), ResultD[0][1].ParamD.SetcD.dH, Row_Ref, Col_Ref, stRow);
			}
			else
			{
				PrintCellData(pExcel, _T("S5_2_2_D_Bot_Trans1"), ResultD[0][1].ParamD.SetcD.dB, Row_Ref, Col_Ref, stRow);
			}
			PrintCellData(pExcel, _T("S5_2_2_L_Bot_Trans1"), ResultD[0][1].ParamD.OptD.dHe, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S5_2_2_D_Bot_Trans2"), ResultD[0][1].ParamD.SetcD.dH, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S5_2_2_L_Bot_Trans2"), ResultD[0][1].ParamD.OptD.dHe, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S5_2_2_Cur_RAT_Bot_Trans2"), ResultD[0][1].CapaD.BendCapaD.dmu_phi, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S5_2_2_Disp_Duct_Bot_Trans1"), ResultD[0][1].CapaD.BendCapaD.dmu_delta, Row_Ref, Col_Ref, stRow);

			PrintCellData(pExcel, _T("S5_2_2_Dis_Y_Bot_Trans2"), ResultD[0][1].CapaD.BendCapaD.dDeltay, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S5_2_2_DispDuct_Bot_Trans2"), ResultD[0][1].CapaD.BendCapaD.dmu_delta, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S5_2_2_Dis_U_Bot_Trans"), ResultD[0][1].CapaD.BendCapaD.dDeltau, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S5_2_2_Dis_U_Bot_Transmm"), 1000 * ResultD[0][1].CapaD.BendCapaD.dDeltau, Row_Ref, Col_Ref, stRow);
		}

		if (ResultD[1][1].EvalD.bCheck)
		{
			PrintCellData(pExcel, _T("S5_2_2_Cur_Y_Top_Trans1"), ResultD[1][1].MPhiD.StateD.dCurvature[4], Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S5_2_2_EHeight_Top_Trans"), ResultD[1][1].ParamD.OptD.dHe, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S5_2_2_Dis_Y_Top_Trans1"), ResultD[1][1].CapaD.DispDuctD.dDeltay, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S5_2_2_Dis_Y_Top_Trans1mm"), 1000 * ResultD[1][1].CapaD.DispDuctD.dDeltay, Row_Ref, Col_Ref, stRow);

			PrintCellData(pExcel, _T("S5_2_2_Cur_U_Top_Trans"), ResultD[1][1].MPhiD.StateD.dCurvature[5], Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S5_2_2_Cur_Y_Top_Trans2"), ResultD[1][1].MPhiD.StateD.dCurvature[4], Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S5_2_2_Cur_RAT_Top_Trans1"), ResultD[1][1].CapaD.BendCapaD.dmu_phi, Row_Ref, Col_Ref, stRow);
			if (ResultD[1][1].ParamD.SetcD.strShape == _T("P") || ResultD[1][1].ParamD.SetcD.strShape == _T("SR"))
			{
				PrintCellData(pExcel, _T("S5_2_2_D_Top_Trans1"), ResultD[1][1].ParamD.SetcD.dH, Row_Ref, Col_Ref, stRow);
			}
			else
			{
				PrintCellData(pExcel, _T("S5_2_2_D_Top_Trans1"), ResultD[1][1].ParamD.SetcD.dB, Row_Ref, Col_Ref, stRow);
			}
			PrintCellData(pExcel, _T("S5_2_2_L_Top_Trans1"), ResultD[1][1].ParamD.OptD.dHe, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S5_2_2_D_Top_Trans2"), ResultD[1][1].ParamD.SetcD.dH, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S5_2_2_L_Top_Trans2"), ResultD[1][1].ParamD.OptD.dHe, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S5_2_2_Cur_RAT_Top_Trans2"), ResultD[1][1].CapaD.BendCapaD.dmu_phi, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S5_2_2_Disp_Duct_Top_Trans1"), ResultD[1][1].CapaD.BendCapaD.dmu_delta, Row_Ref, Col_Ref, stRow);

			PrintCellData(pExcel, _T("S5_2_2_Dis_Y_Top_Trans2"), ResultD[1][1].CapaD.BendCapaD.dDeltay, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S5_2_2_DispDuct_Top_Trans2"), ResultD[1][1].CapaD.BendCapaD.dmu_delta, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S5_2_2_Dis_U_Top_Trans"), ResultD[1][1].CapaD.BendCapaD.dDeltau, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S5_2_2_Dis_U_Top_Transmm"), 1000 * ResultD[1][1].CapaD.BendCapaD.dDeltau, Row_Ref, Col_Ref, stRow);
		}

		stRow += rowCnt;
	}

	return TRUE;
}



BOOL CBrdgSeisEvalKISTEC2015_Report::GenerateBendingResult(IExcel* pExcel, CString& RptBlock, int& stRow, const EN_BEHAVIOR_TYPE* enBehavior, T_BRDGEVAL_RESULT ResultD[2][2], const UINT& EvgpK, const UINT& iColm, const CString& strName)
{
	short Row_Ref = 0, Col_Ref = 0;

	CString strMomentFileName;

	RptBlock = _T("[2]Summary_Disp_Bend_Longi_Head");
	int rowCnt = CopyRangeSubTitle(pExcel, RptBlock, stRow);

	stRow += rowCnt;

	if (enBehavior[0] == CANTILEVER)
	{
		RptBlock = _T("[2]Summary_Disp_Bend_Cantil_Longi");
		rowCnt = CopyRangeSubTitle(pExcel, RptBlock, stRow);
		GetCellIDByName(pExcel, RptBlock, Row_Ref, Col_Ref);

		CDgnSeisFailureModeDlg dlg;
		strMomentFileName.Format(_T("%s_%s_%s_%s_Moment.emf"), m_strPicPath, strName, _T("LONGI"), _T("BOTTOM"));
		dlg.MakeEmfAndClose(EvgpK, iColm, LONGI, BOTTOM, TRUE, FALSE, strMomentFileName);
		dlg.DoModal();

		PrintCellData(pExcel, _T("Moment_Img_Longi"), PrintMomentShearGraph(pExcel, strMomentFileName, stRow, 3), Row_Ref, Col_Ref, stRow);

		if (ResultD[0][0].EvalD.bCheck)
		{
			PrintCellData(pExcel, _T("Tab_DisY_Longi"), 1000 * ResultD[0][0].CapaD.BendCapaD.dDeltay, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("Tab_BStressY_Longi"), ResultD[0][0].CapaD.BendCapaD.dFy, Row_Ref, Col_Ref, stRow);

			PrintCellData(pExcel, _T("Tab_DisU_Longi"), 1000 * ResultD[0][0].CapaD.BendCapaD.dDeltau, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("Tab_BStressU_Longi"), ResultD[0][0].CapaD.BendCapaD.dFu, Row_Ref, Col_Ref, stRow);

			if (ResultD[0][0].CapaD.BendCapaD.dDeltac != 0.0)
			{
				PrintCellData(pExcel, _T("Tab_DisC_Longi"), 1000 * ResultD[0][0].CapaD.BendCapaD.dDeltac, Row_Ref, Col_Ref, stRow);

				T_BRDGEVAL_ULT_DISP_DUCT DispDuctD = ResultD[0][0].CapaD.DispDuctD;
				if (DispDuctD.dmu_Deltac <= DispDuctD.dmu_DeltaMax)
					PrintCellData(pExcel, _T("Tab_BStressC_Longi"), _T("-"), Row_Ref, Col_Ref, stRow);
				else
					PrintCellData(pExcel, _T("Tab_BStressC_Longi"), ResultD[0][0].CapaD.BendCapaD.dFn, Row_Ref, Col_Ref, stRow);
			}
			else
			{
				PrintCellData(pExcel, _T("Tab_DisC_Longi"), _T("-"), Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("Tab_BStressC_Longi"), _T("-"), Row_Ref, Col_Ref, stRow);
			}
		}

		stRow += rowCnt;
	}
	else
	{
		RptBlock = _T("[2]Summary_Disp_Bend_Rahmen_Longi");
		rowCnt = CopyRangeSubTitle(pExcel, RptBlock, stRow);
		GetCellIDByName(pExcel, RptBlock, Row_Ref, Col_Ref);

		CDgnSeisFailureModeDlg dlg;
		strMomentFileName.Format(_T("%s_%s_%s_%s_Moment.emf"), m_strPicPath, strName, _T("LONGI"), _T("BOTTOM"));
		dlg.MakeEmfAndClose(EvgpK, iColm, LONGI, BOTTOM, TRUE, FALSE, strMomentFileName);
		dlg.DoModal();

		PrintCellData(pExcel, _T("Moment_Img_Bot_Longi"), PrintMomentShearGraph(pExcel, strMomentFileName, stRow + 1, 3), Row_Ref, Col_Ref, stRow);

		if (ResultD[0][0].EvalD.bCheck)
		{
			PrintCellData(pExcel, _T("Tab_DisY_Bot_Longi"), 1000 * ResultD[0][0].CapaD.BendCapaD.dDeltay, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("Tab_BStressY_Bot_Longi"), ResultD[0][0].CapaD.BendCapaD.dFy, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("Tab_DisU_Bot_Longi"), 1000 * ResultD[0][0].CapaD.BendCapaD.dDeltau, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("Tab_BStressU_Bot_Longi"), ResultD[0][0].CapaD.BendCapaD.dFu, Row_Ref, Col_Ref, stRow);
			if (ResultD[0][0].CapaD.BendCapaD.dDeltac != 0.0)
			{
				PrintCellData(pExcel, _T("Tab_DisC_Bot_Longi"), 1000 * ResultD[0][0].CapaD.BendCapaD.dDeltac, Row_Ref, Col_Ref, stRow);

				T_BRDGEVAL_ULT_DISP_DUCT DispDuctD = ResultD[0][0].CapaD.DispDuctD;
				if (DispDuctD.dmu_Deltac <= DispDuctD.dmu_DeltaMax)
					PrintCellData(pExcel, _T("Tab_BStressC_Bot_Longi"), _T("-"), Row_Ref, Col_Ref, stRow);
				else
					PrintCellData(pExcel, _T("Tab_BStressC_Bot_Longi"), ResultD[0][0].CapaD.BendCapaD.dFn, Row_Ref, Col_Ref, stRow);
			}
			else
			{
				PrintCellData(pExcel, _T("Tab_DisC_Bot_Longi"), _T("-"), Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("Tab_BStressC_Bot_Longi"), _T("-"), Row_Ref, Col_Ref, stRow);
			}
		}

		CDgnSeisFailureModeDlg dlg2;
		strMomentFileName.Format(_T("%s_%s_%s_%s_Moment.emf"), m_strPicPath, strName, _T("LONGI"), _T("TOP"));
		dlg2.MakeEmfAndClose(EvgpK, iColm, LONGI, TOP, TRUE, FALSE, strMomentFileName);
		dlg2.DoModal();

		PrintCellData(pExcel, _T("Moment_Img_Top_Longi"), PrintMomentShearGraph(pExcel, strMomentFileName, (stRow + 1) + 22, 3), Row_Ref, Col_Ref, stRow);

		if (ResultD[1][0].EvalD.bCheck)
		{
			PrintCellData(pExcel, _T("Tab_DisY_Top_Longi"), 1000 * ResultD[1][0].CapaD.BendCapaD.dDeltay, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("Tab_BStressY_Top_Longi"), ResultD[1][0].CapaD.BendCapaD.dFy, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("Tab_DisU_Top_Longi"), 1000 * ResultD[1][0].CapaD.BendCapaD.dDeltau, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("Tab_BStressU_Top_Longi"), ResultD[1][0].CapaD.BendCapaD.dFu, Row_Ref, Col_Ref, stRow);
			if (ResultD[1][0].CapaD.BendCapaD.dDeltac != 0.0)
			{
				PrintCellData(pExcel, _T("Tab_DisC_Top_Longi"), 1000 * ResultD[1][0].CapaD.BendCapaD.dDeltac, Row_Ref, Col_Ref, stRow);

				T_BRDGEVAL_ULT_DISP_DUCT DispDuctD = ResultD[1][0].CapaD.DispDuctD;
				if (DispDuctD.dmu_Deltac <= DispDuctD.dmu_DeltaMax)
					PrintCellData(pExcel, _T("Tab_BStressC_Top_Longi"), _T("-"), Row_Ref, Col_Ref, stRow);
				else
					PrintCellData(pExcel, _T("Tab_BStressC_Top_Longi"), ResultD[1][0].CapaD.BendCapaD.dFn, Row_Ref, Col_Ref, stRow);
			}
			else
			{
				PrintCellData(pExcel, _T("Tab_DisC_Top_Longi"), _T("-"), Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("Tab_BStressC_Top_Longi"), _T("-"), Row_Ref, Col_Ref, stRow);
			}
		}

		stRow += rowCnt;
	}

	RptBlock = _T("[2]Summary_Disp_Bend_Trans_Head");
	rowCnt = CopyRangeSubTitle(pExcel, RptBlock, stRow);

	stRow += rowCnt;

	if (enBehavior[1] == CANTILEVER)
	{
		RptBlock = _T("[2]Summary_Disp_Bend_Cantil_Trans");
		rowCnt = CopyRangeSubTitle(pExcel, RptBlock, stRow);
		GetCellIDByName(pExcel, RptBlock, Row_Ref, Col_Ref);

		CDgnSeisFailureModeDlg dlg;
		strMomentFileName.Format(_T("%s_%s_%s_%s_Moment.emf"), m_strPicPath, strName, _T("TRANS"), _T("BOTTOM"));
		dlg.MakeEmfAndClose(EvgpK, iColm, TRANS, BOTTOM, TRUE, FALSE, strMomentFileName);
		dlg.DoModal();

		PrintCellData(pExcel, _T("Moment_Img_Trans"), PrintMomentShearGraph(pExcel, strMomentFileName, stRow, 3), Row_Ref, Col_Ref, stRow);

		if (ResultD[0][1].EvalD.bCheck)
		{
			PrintCellData(pExcel, _T("Tab_DisY_Trans"), 1000 * ResultD[0][1].CapaD.BendCapaD.dDeltay, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("Tab_BStressY_Trans"), ResultD[0][1].CapaD.BendCapaD.dFy, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("Tab_DisU_Trans"), 1000 * ResultD[0][1].CapaD.BendCapaD.dDeltau, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("Tab_BStressU_Trans"), ResultD[0][1].CapaD.BendCapaD.dFu, Row_Ref, Col_Ref, stRow);
			if (ResultD[0][1].CapaD.BendCapaD.dDeltac != 0.0)
			{
				PrintCellData(pExcel, _T("Tab_DisC_Trans"), 1000 * ResultD[0][1].CapaD.BendCapaD.dDeltac, Row_Ref, Col_Ref, stRow);

				T_BRDGEVAL_ULT_DISP_DUCT DispDuctD = ResultD[0][1].CapaD.DispDuctD;
				if (DispDuctD.dmu_Deltac <= DispDuctD.dmu_DeltaMax)
					PrintCellData(pExcel, _T("Tab_BStressC_Trans"), _T("-"), Row_Ref, Col_Ref, stRow);
				else
					PrintCellData(pExcel, _T("Tab_BStressC_Trans"), ResultD[0][1].CapaD.BendCapaD.dFn, Row_Ref, Col_Ref, stRow);
			}
			else
			{
				PrintCellData(pExcel, _T("Tab_DisC_Trans"), _T("-"), Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("Tab_BStressC_Trans"), _T("-"), Row_Ref, Col_Ref, stRow);
			}
		}

		stRow += rowCnt;
	}
	else
	{
		CDgnSeisFailureModeDlg dlg;
		RptBlock = _T("[2]Summary_Disp_Bend_Rahmen_Trans");
		rowCnt = CopyRangeSubTitle(pExcel, RptBlock, stRow);
		GetCellIDByName(pExcel, RptBlock, Row_Ref, Col_Ref);

		strMomentFileName.Format(_T("%s_%s_%s_%s_Moment.emf"), m_strPicPath, strName, _T("TRANS"), _T("BOTTOM"));
		dlg.MakeEmfAndClose(EvgpK, iColm, TRANS, BOTTOM, TRUE, FALSE, strMomentFileName);
		dlg.DoModal();

		PrintCellData(pExcel, _T("Moment_Img_Bot_Trans"), PrintMomentShearGraph(pExcel, strMomentFileName, stRow + 1, 3), Row_Ref, Col_Ref, stRow);

		if (ResultD[0][1].EvalD.bCheck)
		{
			PrintCellData(pExcel, _T("Tab_DisY_Bot_Trans"), 1000 * ResultD[0][1].CapaD.BendCapaD.dDeltay, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("Tab_BStressY_Bot_Trans"), ResultD[0][1].CapaD.BendCapaD.dFy, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("Tab_DisU_Bot_Trans"), 1000 * ResultD[0][1].CapaD.BendCapaD.dDeltau, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("Tab_BStressU_Bot_Trans"), ResultD[0][1].CapaD.BendCapaD.dFu, Row_Ref, Col_Ref, stRow);
			if (ResultD[0][1].CapaD.BendCapaD.dDeltac != 0.0)
			{
				PrintCellData(pExcel, _T("Tab_DisC_Bot_Trans"), 1000 * ResultD[0][1].CapaD.BendCapaD.dDeltac, Row_Ref, Col_Ref, stRow);

				T_BRDGEVAL_ULT_DISP_DUCT DispDuctD = ResultD[0][1].CapaD.DispDuctD;
				if (DispDuctD.dmu_Deltac <= DispDuctD.dmu_DeltaMax)
					PrintCellData(pExcel, _T("Tab_BStressC_Bot_Trans"), _T("-"), Row_Ref, Col_Ref, stRow);
				else
					PrintCellData(pExcel, _T("Tab_BStressC_Bot_Trans"), ResultD[0][1].CapaD.BendCapaD.dFn, Row_Ref, Col_Ref, stRow);
			}
			else
			{
				PrintCellData(pExcel, _T("Tab_DisC_Bot_Trans"), _T("-"), Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("Tab_BStressC_Bot_Trans"), _T("-"), Row_Ref, Col_Ref, stRow);
			}
		}

		CDgnSeisFailureModeDlg dlg2;
		strMomentFileName.Format(_T("%s_%s_%s_%s_Moment.emf"), m_strPicPath, strName, _T("TRANS"), _T("TOP"));
		dlg2.MakeEmfAndClose(EvgpK, iColm, TRANS, TOP, TRUE, FALSE, strMomentFileName);
		dlg2.DoModal();

		PrintCellData(pExcel, _T("Moment_Img_Top_Trans"), PrintMomentShearGraph(pExcel, strMomentFileName, (stRow + 1) + 22, 3), Row_Ref, Col_Ref, stRow);

		if (ResultD[1][1].EvalD.bCheck)
		{
			PrintCellData(pExcel, _T("Tab_DisY_Top_Trans"), 1000 * ResultD[1][1].CapaD.BendCapaD.dDeltay, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("Tab_BStressY_Top_Trans"), ResultD[1][1].CapaD.BendCapaD.dFy, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("Tab_DisU_Top_Trans"), 1000 * ResultD[1][1].CapaD.BendCapaD.dDeltau, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("Tab_BStressU_Top_Trans"), ResultD[1][1].CapaD.BendCapaD.dFu, Row_Ref, Col_Ref, stRow);
			if (ResultD[1][1].CapaD.BendCapaD.dDeltac != 0.0)
			{
				PrintCellData(pExcel, _T("Tab_DisC_Top_Trans"), 1000 * ResultD[1][1].CapaD.BendCapaD.dDeltac, Row_Ref, Col_Ref, stRow);

				T_BRDGEVAL_ULT_DISP_DUCT DispDuctD = ResultD[1][1].CapaD.DispDuctD;
				if (DispDuctD.dmu_Deltac <= DispDuctD.dmu_DeltaMax)
					PrintCellData(pExcel, _T("Tab_BStressC_Top_Trans"), _T("-"), Row_Ref, Col_Ref, stRow);
				else
					PrintCellData(pExcel, _T("Tab_BStressC_Top_Trans"), ResultD[1][1].CapaD.BendCapaD.dFn, Row_Ref, Col_Ref, stRow);
			}
			else
			{
				PrintCellData(pExcel, _T("Tab_DisC_Top_Trans"), _T("-"), Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("Tab_BStressC_Top_Trans"), _T("-"), Row_Ref, Col_Ref, stRow);
			}
		}

		stRow += rowCnt;
	}

	return TRUE;

}

BOOL CBrdgSeisEvalKISTEC2015_Report::GenerateShearResult(IExcel* pExcel, CString& RptBlock, int& stRow, _DGN_SEISEVAL_SHEAR_OUT& ShearOut)
{
	// 6.7 교각의 전단성능 곡선
	RptBlock = _T("[2]Shear_Pier_Curvature_Head");
	int rowCnt = CopyRangeSubTitle(pExcel, RptBlock, stRow);
	stRow += rowCnt;

	BOOL bNotWall = (ShearOut.enPierType[0] != WALL && ShearOut.enPierType[1] != WALL);
	if (bNotWall)
	{
		RptBlock = _T("[2]Graph_Disp_Duct_Shear_Strength");
		rowCnt = CopyRangeSubTitle(pExcel, RptBlock, stRow);
		stRow += rowCnt;
	}

	// 	// 1) 교축방향
	// 	GenerateShearResult_Longi(pExcel, RptBlock, stRow, ShearOut);
	// 	// 2) 교축직각방향
	// 	GenerateShearResult_Trans(pExcel, RptBlock, stRow, ShearOut);
	// 	// 3) 공급변위연성도에 따른 교축방향 교각의 전단강도
	// 	GenerateShearResult_Longi_Strn(pExcel, RptBlock, stRow, ShearOut);
	// 	// 4) 공급변위연성도에 따른 교축직각방향 교각의 전단강도
	// 	GenerateShearResult_Trans_Strn(pExcel, RptBlock, stRow, ShearOut);

		// 1) 교축방향
	GenerateShearResult_Longi_Mod(pExcel, RptBlock, stRow, ShearOut);
	// 2) 교축직각방향
	GenerateShearResult_Trans_Mod(pExcel, RptBlock, stRow, ShearOut);
	// 3) 공급변위연성도에 따른 교축방향 교각의 전단강도
	GenerateShearResult_Longi_Strn(pExcel, RptBlock, stRow, ShearOut);
	// 4) 공급변위연성도에 따른 교축직각방향 교각의 전단강도
	GenerateShearResult_Trans_Strn(pExcel, RptBlock, stRow, ShearOut);

	return TRUE;
}

BOOL CBrdgSeisEvalKISTEC2015_Report::GenerateShearResult_Longi(IExcel* pExcel, CString& RptBlock, int& stRow, _DGN_SEISEVAL_SHEAR_OUT& ShearOut)
{
	// 1) 교축방향
	RptBlock = _T("[2]Shear_Longi");
	int rowCnt = CopyRangeSubTitle(pExcel, RptBlock, stRow);
	stRow += rowCnt;

	if (ShearOut.enPierType[0] == WALL)
	{
		if (ShearOut.ResultD[0][0].ParamD.OptD.iEvalCode == KISTEC_2019 ||
			ShearOut.ResultD[0][0].ParamD.OptD.iEvalCode == KALIS_2023)
			GenerateShearResult_Longi_Wall_2019(pExcel, RptBlock, stRow, ShearOut);
		else
			GenerateShearResult_Longi_Wall(pExcel, RptBlock, stRow, ShearOut);
	}
	else
	{
		GenerateShearResult_Longi_Else(pExcel, RptBlock, stRow, ShearOut);
	}

	return TRUE;
}

BOOL CBrdgSeisEvalKISTEC2015_Report::GenerateShearResult_Longi_Mod(IExcel* pExcel, CString& RptBlock, int& stRow, _DGN_SEISEVAL_SHEAR_OUT& ShearOut)
{
	// 1) 교축방향
	RptBlock = _T("[2]Shear_Longi");

	int rowCnt = CopyRangeSubTitle(pExcel, RptBlock, stRow);
	stRow += rowCnt;

	// 	if (ShearOut.enPierType[1] == WALL)
	// 	{
	// 		ASSERT(0);
	// 		GenerateShearResult_Trans_Wall(pExcel, RptBlock, stRow, ShearOut);
	// 	}
	// 	else
	// 	{
	// 		GenerateShearResult_Trans_Else(pExcel, RptBlock, stRow, ShearOut);
	// 	}

	if (ShearOut.enPierType[0] == WALL)
	{
		ASSERT(0);
		GenerateShearResult_Trans_Wall_byLongiData(pExcel, RptBlock, stRow, ShearOut);
	}
	else
	{
		GenerateShearResult_Trans_Else_byLongiData(pExcel, RptBlock, stRow, ShearOut);
	}

	return TRUE;
}


BOOL CBrdgSeisEvalKISTEC2015_Report::GenerateShearResult_Longi_Wall_byTranData(IExcel* pExcel, CString& RptBlock, int& stRow, _DGN_SEISEVAL_SHEAR_OUT& ShearOut)
{
	GenerateShearResult_Longi_Wall(pExcel, RptBlock, stRow, ShearOut, 1);

	return TRUE;
}

BOOL CBrdgSeisEvalKISTEC2015_Report::GenerateShearResult_Longi_Wall(IExcel* pExcel, CString& RptBlock, int& stRow, _DGN_SEISEVAL_SHEAR_OUT& ShearOut, int nLongTran/* = 0*/)
{
	RptBlock = _T("[2]Shear_Brick_Head");
	int rowCnt = CopyRangeSubTitle(pExcel, RptBlock, stRow);
	stRow += rowCnt;

	short Row_Ref = 0, Col_Ref = 0;
	RptBlock = _T("[2]Shear_Brick_Major");
	rowCnt = CopyRangeSubTitle(pExcel, RptBlock, stRow);
	GetCellIDByName(pExcel, RptBlock, Row_Ref, Col_Ref);

	int nLT = nLongTran;

	T_BRDGEVAL_RESULT PrtResultD; PrtResultD.initialize();
	if (ShearOut.ResultD[0][nLT].EvalD.bCheck) PrtResultD = ShearOut.ResultD[0][nLT];
	else if (ShearOut.ResultD[1][nLT].EvalD.bCheck) PrtResultD = ShearOut.ResultD[1][nLT];

	double dB = (nLT == 0) ? PrtResultD.ParamD.SetcD.dB : PrtResultD.ParamD.SetcD.dH;
	double dH = (nLT == 0) ? PrtResultD.ParamD.SetcD.dH : PrtResultD.ParamD.SetcD.dB;

	if (PrtResultD.EvalD.bCheck)
	{
		PrintCellData(pExcel, _T("S6_1_1_B_Longi1"), 1000 * dB, Row_Ref, Col_Ref, stRow);
		PrintCellData(pExcel, _T("S6_1_1_H_Longi"), 1000 * dH, Row_Ref, Col_Ref, stRow);
		PrintCellData(pExcel, _T("S6_1_1_DC_Longi"), 1000 * PrtResultD.ParamD.BarD.ddc, Row_Ref, Col_Ref, stRow);
		PrintCellData(pExcel, _T("S6_1_1_D_Longi1"), 1000 * dH - 1000 * PrtResultD.ParamD.BarD.ddc, Row_Ref, Col_Ref, stRow);

		PrintCellData(pExcel, _T("S6_1_1_FCK_Longi1"), m_dFactorStress * PrtResultD.ParamD.MatlD.dfck, Row_Ref, Col_Ref, stRow);
		PrintCellData(pExcel, _T("S6_1_1_B_Longi2"), 1000 * dB, Row_Ref, Col_Ref, stRow);
		PrintCellData(pExcel, _T("S6_1_1_D_Longi2"), 1000 * dH - 1000 * PrtResultD.ParamD.BarD.ddc, Row_Ref, Col_Ref, stRow);
		PrintCellData(pExcel, _T("S6_1_1_VN1_Longi1"), PrtResultD.CapaD.ShearCapaD.dVn1, Row_Ref, Col_Ref, stRow);

		PrintCellData(pExcel, _T("S6_1_1_FCK_Longi2"), m_dFactorStress * PrtResultD.ParamD.MatlD.dfck, Row_Ref, Col_Ref, stRow);
		PrintCellData(pExcel, _T("S6_1_1_PSH_Longi"), PrtResultD.ParamD.BarD.drhosh, Row_Ref, Col_Ref, stRow);
		PrintCellData(pExcel, _T("S6_1_1_FYH_Longi"), m_dFactorStress * PrtResultD.ParamD.MatlD.dfyh, Row_Ref, Col_Ref, stRow);
		PrintCellData(pExcel, _T("S6_1_1_B_Longi3"), 1000 * dB, Row_Ref, Col_Ref, stRow);
		PrintCellData(pExcel, _T("S6_1_1_D_Longi3"), 1000 * dH - 1000 * PrtResultD.ParamD.BarD.ddc, Row_Ref, Col_Ref, stRow);
		PrintCellData(pExcel, _T("S6_1_1_VN2_Longi1"), PrtResultD.CapaD.ShearCapaD.dVn2, Row_Ref, Col_Ref, stRow);

		PrintCellData(pExcel, _T("S6_1_1_VN1_Longi2"), PrtResultD.CapaD.ShearCapaD.dVn1, Row_Ref, Col_Ref, stRow);
		PrintCellData(pExcel, _T("S6_1_1_VN2_Longi2"), PrtResultD.CapaD.ShearCapaD.dVn2, Row_Ref, Col_Ref, stRow);
		PrintCellData(pExcel, _T("S6_1_1_VN_FIN_Longi"), min(PrtResultD.CapaD.ShearCapaD.dVn1, PrtResultD.CapaD.ShearCapaD.dVn2), Row_Ref, Col_Ref, stRow);
	}
	stRow += rowCnt;

	return TRUE;
}

BOOL CBrdgSeisEvalKISTEC2015_Report::GenerateShearResult_Longi_Else_byTranData(IExcel* pExcel, CString& RptBlock, int& stRow, _DGN_SEISEVAL_SHEAR_OUT& ShearOut)
{

	GenerateShearResult_Longi_Else(pExcel, RptBlock, stRow, ShearOut, 1);

	return TRUE;
}

BOOL CBrdgSeisEvalKISTEC2015_Report::GenerateShearResult_Longi_Else(IExcel* pExcel, CString& RptBlock, int& stRow, _DGN_SEISEVAL_SHEAR_OUT& ShearOut, int nLongTran/* = 0*/)
{
	BOOL bNotWall = (ShearOut.enPierType[0] != WALL && ShearOut.enPierType[1] != WALL);
	if (!bNotWall)
	{
		RptBlock = _T("[2]Graph_Disp_Duct_Shear_Strength");
		int rowCnt = CopyRangeSubTitle(pExcel, RptBlock, stRow);
		stRow += rowCnt;
	}

	int nLT = nLongTran;

	// 가) 콘크리트에 의한 전단강도
	if (nLT == 0) GenerateShearResult_Longi_Else_Conc(pExcel, RptBlock, stRow, ShearOut, nLT);
	else       GenerateShearResult_Trans_Else_Conc(pExcel, RptBlock, stRow, ShearOut, nLT);

	// 나) 전단철근에 의한 전단강도
	GenerateShearResult_Longi_Else_Rebar(pExcel, RptBlock, stRow, ShearOut, nLT);

	// 다) 축하중 작용에 의한 전단강도
	if (nLT == 0) GenerateShearResult_Longi_Else_Load(pExcel, RptBlock, stRow, ShearOut, nLT);
	else       GenerateShearResult_Trans_Else_Load(pExcel, RptBlock, stRow, ShearOut, nLT);

	return TRUE;
}

BOOL CBrdgSeisEvalKISTEC2015_Report::GenerateShearResult_Longi_Else_Conc(IExcel* pExcel, CString& RptBlock, int& stRow, _DGN_SEISEVAL_SHEAR_OUT& ShearOut, int nLongTran/* = 0*/)
{
	short Row_Ref = 0, Col_Ref = 0;
	RptBlock = _T("[2]Shear_Conc_Head");
	int rowCnt = CopyRangeSubTitle(pExcel, RptBlock, stRow);
	stRow += rowCnt;

	int nLT = nLongTran;

	if (ShearOut.enBehavior[nLT] == CANTILEVER)
	{
		RptBlock = _T("[2]Shear_Conc_Cantil_Longi");
		rowCnt = CopyRangeSubTitle(pExcel, RptBlock, stRow);
		GetCellIDByName(pExcel, RptBlock, Row_Ref, Col_Ref);

		if (ShearOut.ResultD[0][nLT].EvalD.bCheck)
		{
			PrintCellData(pExcel, _T("S6_1_FCK_Longi1"), m_dFactorStress	*	ShearOut.ResultD[0][nLT].ParamD.MatlD.dfck, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S6_1_AreaG_Longi1"), ShearOut.ResultD[0][nLT].ParamD.SetcD.dArea, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S6_1_AreaE_Longi1"), ShearOut.ResultD[0][nLT].ParamD.SetcD.dArea * ShearOut.ResultD[0][nLT].ParamD.OptD.dAeRat, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S6_1_FCK_Longi2"), m_dFactorStress	*	ShearOut.ResultD[0][nLT].ParamD.MatlD.dfck, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S6_1_AreaE_Longi2"), ShearOut.ResultD[0][nLT].ParamD.SetcD.dArea * ShearOut.ResultD[0][nLT].ParamD.OptD.dAeRat, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S6_1_ShearC_Longi"), ShearOut.ResultD[0][nLT].CapaD.ShearCapaD.dVc2, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S6_1_Dis_Y_Longi1"), ShearOut.ResultD[0][nLT].CapaD.DispDuctD.dDeltay, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S6_1_Dis_2_Longi"), 2 * ShearOut.ResultD[0][nLT].CapaD.DispDuctD.dDeltay, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S6_1_Dis_Y_Longi2"), ShearOut.ResultD[0][nLT].CapaD.DispDuctD.dDeltay, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S6_1_Dis_5_Longi"), 5 * ShearOut.ResultD[0][nLT].CapaD.DispDuctD.dDeltay, Row_Ref, Col_Ref, stRow);
		}

		stRow += rowCnt + 1;

	}
	else
	{
		RptBlock = _T("[2]Shear_Conc_Rahmen_Longi");
		rowCnt = CopyRangeSubTitle(pExcel, RptBlock, stRow);
		GetCellIDByName(pExcel, RptBlock, Row_Ref, Col_Ref);

		if (ShearOut.ResultD[0][nLT].EvalD.bCheck)
		{
			PrintCellData(pExcel, _T("S6_1_FCK_Bot_Longi1"), m_dFactorStress * ShearOut.ResultD[0][nLT].ParamD.MatlD.dfck, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S6_1_AreaG_Bot_Longi1"), ShearOut.ResultD[0][nLT].ParamD.SetcD.dArea, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S6_1_AreaE_Bot_Longi1"), ShearOut.ResultD[0][nLT].ParamD.SetcD.dArea * ShearOut.ResultD[0][nLT].ParamD.OptD.dAeRat, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S6_1_FCK_Bot_Longi2"), m_dFactorStress	* ShearOut.ResultD[0][nLT].ParamD.MatlD.dfck, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S6_1_AreaE_Bot_Longi2"), ShearOut.ResultD[0][nLT].ParamD.SetcD.dArea * ShearOut.ResultD[0][nLT].ParamD.OptD.dAeRat, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S6_1_ShearC_Bot_Longi"), ShearOut.ResultD[0][nLT].CapaD.ShearCapaD.dVc2, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S6_1_Dis_Y_Bot_Longi1"), ShearOut.ResultD[0][nLT].CapaD.DispDuctD.dDeltay, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S6_1_Dis_2_Bot_Longi"), 2 * ShearOut.ResultD[0][nLT].CapaD.DispDuctD.dDeltay, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S6_1_Dis_Y_Bot_Longi2"), ShearOut.ResultD[0][nLT].CapaD.DispDuctD.dDeltay, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S6_1_Dis_5_Bot_Longi"), 5 * ShearOut.ResultD[0][nLT].CapaD.DispDuctD.dDeltay, Row_Ref, Col_Ref, stRow);
		}

		if (ShearOut.ResultD[1][nLT].EvalD.bCheck)
		{
			PrintCellData(pExcel, _T("S6_1_FCK_Top_Longi1"), m_dFactorStress	* ShearOut.ResultD[1][nLT].ParamD.MatlD.dfck, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S6_1_AreaG_Top_Longi1"), ShearOut.ResultD[1][nLT].ParamD.SetcD.dArea, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S6_1_AreaE_Top_Longi1"), ShearOut.ResultD[1][nLT].ParamD.SetcD.dArea * ShearOut.ResultD[1][nLT].ParamD.OptD.dAeRat, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S6_1_FCK_Top_Longi2"), m_dFactorStress	* ShearOut.ResultD[1][nLT].ParamD.MatlD.dfck, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S6_1_AreaE_Top_Longi2"), ShearOut.ResultD[1][nLT].ParamD.SetcD.dArea * ShearOut.ResultD[1][nLT].ParamD.OptD.dAeRat, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S6_1_ShearC_Top_Longi"), ShearOut.ResultD[1][nLT].CapaD.ShearCapaD.dVc2, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S6_1_Dis_Y_Top_Longi1"), ShearOut.ResultD[1][nLT].CapaD.DispDuctD.dDeltay, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S6_1_Dis_2_Top_Longi"), 2 * ShearOut.ResultD[1][nLT].CapaD.DispDuctD.dDeltay, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S6_1_Dis_Y_Top_Longi2"), ShearOut.ResultD[1][nLT].CapaD.DispDuctD.dDeltay, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S6_1_Dis_5_Top_Longi"), 5 * ShearOut.ResultD[1][nLT].CapaD.DispDuctD.dDeltay, Row_Ref, Col_Ref, stRow);
		}

		stRow += rowCnt + 1;
	}

	return TRUE;
}

BOOL CBrdgSeisEvalKISTEC2015_Report::GenerateShearResult_Longi_Else_Rebar(IExcel* pExcel, CString& RptBlock, int& stRow, _DGN_SEISEVAL_SHEAR_OUT& ShearOut, int nLongTran/* = 0*/)
{
	short Row_Ref = 0, Col_Ref = 0;
	// 나) 전단철근에 의한 전단강도
	RptBlock = _T("[2]Shear_Rebar_Head");
	int rowCnt = CopyRangeSubTitle(pExcel, RptBlock, stRow);
	stRow += rowCnt;

	int nLT = nLongTran;

	CString strShape;
	if (ShearOut.ResultD[0][nLT].EvalD.bCheck) { strShape = ShearOut.ResultD[0][nLT].ParamD.SetcD.strShape; }// 하부 교축
	else if (ShearOut.ResultD[1][nLT].EvalD.bCheck) { strShape = ShearOut.ResultD[1][nLT].ParamD.SetcD.strShape; }// 상부 교축

	T_BRDGEVAL_RESULT PrtResultD1, PrtResultD2;
	PrtResultD1.initialize();
	PrtResultD2.initialize();
	if (ShearOut.ResultD[0][nLT].EvalD.bCheck) { PrtResultD1 = ShearOut.ResultD[0][nLT]; }// 하부 교축
	else if (ShearOut.ResultD[1][nLT].EvalD.bCheck) { PrtResultD1 = ShearOut.ResultD[1][nLT]; }// 상부 교축

	if (strShape == _T("P") || strShape == _T("SR"))
	{
		RptBlock = _T("[2]Shear_Rebar_Circle");
		rowCnt = CopyRangeSubTitle(pExcel, RptBlock, stRow);
		GetCellIDByName(pExcel, RptBlock, Row_Ref, Col_Ref);

		if (PrtResultD1.EvalD.bCheck)
		{
			PrintCellData(pExcel, _T("S6_2_CIR_AreaV_Longi1"), (1 / m_dFactorStress) * (1 / m_dFactorStress)	*	PrtResultD1.ParamD.BarD.dAsp, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S6_2_CIR_FYH_Longi1"), m_dFactorStress* PrtResultD1.ParamD.MatlD.dfyh, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S6_2_CIR_D1_Longi1"), 1000 * PrtResultD1.CapaD.ShearCapaD.dD1, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S6_2_CIR_S_Longi1"), 1000 * PrtResultD1.ParamD.BarD.ds, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S6_2_CIR_ShearR_Longi"), PrtResultD1.CapaD.ShearCapaD.dVs, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S6_2_CIR_AreaV_Longi2"), (1 / m_dFactorStress) * (1 / m_dFactorStress)	*	PrtResultD1.ParamD.BarD.dAsp, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S6_2_CIR_FYH_Longi2"), m_dFactorStress* PrtResultD1.ParamD.MatlD.dfyh, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S6_2_CIR_D1_Longi2"), 1000 * PrtResultD1.CapaD.ShearCapaD.dD1, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S6_2_CIR_S_Longi2"), 1000 * PrtResultD1.ParamD.BarD.ds, Row_Ref, Col_Ref, stRow);
		}

		stRow += rowCnt;
	}
	else if (strShape == _T("B") || strShape == _T("SB") ||
		strShape == _T("OCT") || strShape == _T("SOCT") ||
		strShape == _T("GEN"))
	{
		RptBlock = _T("[2]Shear_Rebar_Box_Longi");
		rowCnt = CopyRangeSubTitle(pExcel, RptBlock, stRow);
		GetCellIDByName(pExcel, RptBlock, Row_Ref, Col_Ref);

		if (PrtResultD1.EvalD.bCheck)
		{
			PrintCellData(pExcel, _T("S6_2_BOX_AreaV_Longi1"), (1 / m_dFactorStress) * (1 / m_dFactorStress)	*	PrtResultD1.ParamD.BarD.dAv, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S6_2_BOX_FYH_Longi1"), m_dFactorStress* PrtResultD1.ParamD.MatlD.dfyh, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S6_2_BOX_D1_Longi1"), 1000 * PrtResultD1.CapaD.ShearCapaD.dD1, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S6_2_BOX_S_Longi1"), 1000 * PrtResultD1.ParamD.BarD.ds, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S6_2_BOX_ShearR_Longi"), PrtResultD1.CapaD.ShearCapaD.dVs, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S6_2_BOX_AreaV_Longi2"), (1 / m_dFactorStress) * (1 / m_dFactorStress)	*	PrtResultD1.ParamD.BarD.dAv, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S6_2_BOX_FYH_Longi2"), m_dFactorStress* PrtResultD1.ParamD.MatlD.dfyh, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S6_2_BOX_D1_Longi2"), 1000 * PrtResultD1.CapaD.ShearCapaD.dD1, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S6_2_BOX_S_Longi2"), 1000 * PrtResultD1.ParamD.BarD.ds, Row_Ref, Col_Ref, stRow);
		}
		stRow += rowCnt;
	}
	else if (strShape == _T("TRK") || strShape == _T("STRK"))
	{
		if (nLT == 0) // 원래
		{
			RptBlock = _T("[2]Shear_Rebar_Track_Longi");
			rowCnt = CopyRangeSubTitle(pExcel, RptBlock, stRow);
			GetCellIDByName(pExcel, RptBlock, Row_Ref, Col_Ref);

			if (PrtResultD1.EvalD.bCheck)
			{
				PrintCellData(pExcel, _T("S6_2_TRACK_AreaV_Longi1"), (1 / m_dFactorStress) * (1 / m_dFactorStress)	*	PrtResultD2.ParamD.BarD.dAsp, Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S6_2_TRACK_FYH_Longi1"), m_dFactorStress* PrtResultD2.ParamD.MatlD.dfyh, Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S6_2_TRACK_D1_Longi1"), 1000 * PrtResultD2.CapaD.ShearCapaD.dD1, Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S6_2_TRACK_S_Longi1"), 1000 * PrtResultD2.ParamD.BarD.ds, Row_Ref, Col_Ref, stRow);

				PrintCellData(pExcel, _T("S6_2_TRACK_AreaV2_Longi1"), (1 / m_dFactorStress) * (1 / m_dFactorStress)	*	PrtResultD2.CapaD.ShearCapaD.dAvCal, Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S6_2_TRACK_FYH2_Longi1"), m_dFactorStress* PrtResultD2.ParamD.MatlD.dfyh, Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S6_2_TRACK_D2_Longi1"), 1000 * PrtResultD2.CapaD.ShearCapaD.dD2, Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S6_2_TRACK_S2_Longi1"), 1000 * PrtResultD2.ParamD.BarD.ds, Row_Ref, Col_Ref, stRow);

				PrintCellData(pExcel, _T("S6_2_TRACK_ShearR_Longi"), PrtResultD2.CapaD.ShearCapaD.dVs, Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S6_2_TRACK_AreaV_Longi2"), (1 / m_dFactorStress) * (1 / m_dFactorStress)	*	PrtResultD2.ParamD.BarD.dAsp, Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S6_2_TRACK_AreaV_Longi4"), (1 / m_dFactorStress) * (1 / m_dFactorStress)	*	PrtResultD2.CapaD.ShearCapaD.dAvCal, Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S6_2_TRACK_FYH_Longi2"), m_dFactorStress* PrtResultD2.ParamD.MatlD.dfyh, Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S6_2_TRACK_D1_Longi2"), 1000 * PrtResultD2.CapaD.ShearCapaD.dD1, Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S6_2_TRACK_D2_Longi2"), 1000 * PrtResultD2.CapaD.ShearCapaD.dD2, Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S6_2_TRACK_S_Longi2"), 1000 * PrtResultD2.ParamD.BarD.ds, Row_Ref, Col_Ref, stRow);
			}
		}
		else
		{
			RptBlock = _T("[2]Shear_Rebar_Track_Trans");
			rowCnt = CopyRangeSubTitle(pExcel, RptBlock, stRow);
			GetCellIDByName(pExcel, RptBlock, Row_Ref, Col_Ref);

			if (PrtResultD1.EvalD.bCheck)
			{
				PrtResultD2 = PrtResultD1;

				PrintCellData(pExcel, _T("S6_2_TRACK_AreaV_Trans1"), (1 / m_dFactorStress) * (1 / m_dFactorStress)	*	PrtResultD2.ParamD.BarD.dAsp, Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S6_2_TRACK_FYH_Trans1"), m_dFactorStress* PrtResultD2.ParamD.MatlD.dfyh, Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S6_2_TRACK_D1_Trans1"), 1000 * PrtResultD2.CapaD.ShearCapaD.dD1, Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S6_2_TRACK_S_Trans1"), 1000 * PrtResultD2.ParamD.BarD.ds, Row_Ref, Col_Ref, stRow);

				PrintCellData(pExcel, _T("S6_2_TRACK_AreaV_Trans2"), (1 / m_dFactorStress) * (1 / m_dFactorStress)	*	PrtResultD2.ParamD.BarD.dAv, Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S6_2_TRACK_FYH_Trans2"), m_dFactorStress* PrtResultD2.ParamD.MatlD.dfyh, Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S6_2_TRACK_D2_Trans1"), 1000 * PrtResultD2.CapaD.ShearCapaD.dD2, Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S6_2_TRACK_S_Trans2"), 1000 * PrtResultD2.ParamD.BarD.ds, Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S6_2_TRACK_ShearR_Trans"), PrtResultD2.CapaD.ShearCapaD.dVs, Row_Ref, Col_Ref, stRow);

				PrintCellData(pExcel, _T("S6_2_TRACK_AreaV_Trans3"), (1 / m_dFactorStress) * (1 / m_dFactorStress)	*	PrtResultD2.ParamD.BarD.dAsp, Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S6_2_TRACK_AreaV_Trans4"), (1 / m_dFactorStress) * (1 / m_dFactorStress)	*	PrtResultD2.ParamD.BarD.dAv, Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S6_2_TRACK_FYH_Trans3"), m_dFactorStress* PrtResultD2.ParamD.MatlD.dfyh, Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S6_2_TRACK_D1_Trans2"), 1000 * PrtResultD2.CapaD.ShearCapaD.dD1, Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S6_2_TRACK_D2_Trans2"), 1000 * PrtResultD2.CapaD.ShearCapaD.dD2, Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S6_2_TRACK_S_Trans3"), 1000 * PrtResultD2.ParamD.BarD.ds, Row_Ref, Col_Ref, stRow);
			}
		}
		stRow += rowCnt;
	}
	else if (strShape == _T("HTRK"))
	{
		if (nLT == 0) // 원래
		{
			RptBlock = _T("[2]Shear_Rebar_HalfTrack_Longi");
			rowCnt = CopyRangeSubTitle(pExcel, RptBlock, stRow);
			GetCellIDByName(pExcel, RptBlock, Row_Ref, Col_Ref);

			if (PrtResultD1.EvalD.bCheck)
			{
				PrintCellData(pExcel, _T("S6_2_HTRACK_AreaV_Longi1"), (1 / m_dFactorStress) * (1 / m_dFactorStress)	*	PrtResultD1.ParamD.BarD.dAsp, Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S6_2_HTRACK_FYH_Longi1"), m_dFactorStress* PrtResultD1.ParamD.MatlD.dfyh, Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S6_2_HTRACK_D1_Longi1"), 1000 * PrtResultD1.CapaD.ShearCapaD.dD1, Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S6_2_HTRACK_S_Longi1"), 1000 * PrtResultD1.ParamD.BarD.ds, Row_Ref, Col_Ref, stRow);

				PrintCellData(pExcel, _T("S6_2_HTRACK_AreaV_Longi2"), (1 / m_dFactorStress) * (1 / m_dFactorStress)	*	PrtResultD1.CapaD.ShearCapaD.dAvCal, Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S6_2_HTRACK_FYH_Longi2"), m_dFactorStress* PrtResultD1.ParamD.MatlD.dfyh, Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S6_2_HTRACK_D1_Longi2"), 1000 * PrtResultD1.CapaD.ShearCapaD.dD2, Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S6_2_HTRACK_S_Longi2"), 1000 * PrtResultD1.ParamD.BarD.ds, Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S6_2_HTRACK_ShearR_Longi"), PrtResultD1.CapaD.ShearCapaD.dVs, Row_Ref, Col_Ref, stRow);

				PrintCellData(pExcel, _T("S6_2_HTRACK_AreaV_Longi3"), (1 / m_dFactorStress) * (1 / m_dFactorStress)	*	PrtResultD1.ParamD.BarD.dAsp, Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S6_2_HTRACK_FYH_Longi3"), m_dFactorStress* PrtResultD1.ParamD.MatlD.dfyh, Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S6_2_HTRACK_D1_Longi3"), 1000 * PrtResultD1.CapaD.ShearCapaD.dD1, Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S6_2_HTRACK_S_Longi3"), 1000 * PrtResultD1.ParamD.BarD.ds, Row_Ref, Col_Ref, stRow);
			}
		}
		else
		{
			RptBlock = _T("[2]Shear_Rebar_HalfTrack_Trans");
			rowCnt = CopyRangeSubTitle(pExcel, RptBlock, stRow);
			GetCellIDByName(pExcel, RptBlock, Row_Ref, Col_Ref);

			if (PrtResultD1.EvalD.bCheck)
			{
				PrtResultD2 = PrtResultD1;

				PrintCellData(pExcel, _T("S6_2_HTRACK_AreaV_Trans1"), (1 / m_dFactorStress) * (1 / m_dFactorStress)	*	PrtResultD2.ParamD.BarD.dAsp, Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S6_2_HTRACK_FYH_Trans1"), m_dFactorStress* PrtResultD2.ParamD.MatlD.dfyh, Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S6_2_HTRACK_D1_Trans1"), 1000 * PrtResultD2.CapaD.ShearCapaD.dD1, Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S6_2_HTRACK_S_Trans1"), 1000 * PrtResultD2.ParamD.BarD.ds, Row_Ref, Col_Ref, stRow);

				PrintCellData(pExcel, _T("S6_2_HTRACK_AreaV_Trans2"), (1 / m_dFactorStress) * (1 / m_dFactorStress)	*	PrtResultD2.CapaD.ShearCapaD.dAvCal, Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S6_2_HTRACK_FYH_Trans2"), m_dFactorStress* PrtResultD2.ParamD.MatlD.dfyh, Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S6_2_HTRACK_D2_Trans1"), 1000 * PrtResultD2.CapaD.ShearCapaD.dD2, Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S6_2_HTRACK_S_Trans2"), 1000 * PrtResultD2.ParamD.BarD.ds, Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S6_2_HTRACK_ShearR_Trans"), PrtResultD2.CapaD.ShearCapaD.dVs, Row_Ref, Col_Ref, stRow);

				PrintCellData(pExcel, _T("S6_2_HTRACK_AreaV_Trans3"), (1 / m_dFactorStress) * (1 / m_dFactorStress)	*	PrtResultD2.ParamD.BarD.dAsp, Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S6_2_HTRACK_AreaV_Trans4"), (1 / m_dFactorStress) * (1 / m_dFactorStress)	*	PrtResultD2.CapaD.ShearCapaD.dAvCal, Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S6_2_HTRACK_FYH_Trans3"), m_dFactorStress* PrtResultD2.ParamD.MatlD.dfyh, Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S6_2_HTRACK_D1_Trans2"), 1000 * PrtResultD2.CapaD.ShearCapaD.dD1, Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S6_2_HTRACK_D2_Trans2"), 1000 * PrtResultD2.CapaD.ShearCapaD.dD2, Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S6_2_HTRACK_S_Trans3"), 1000 * PrtResultD2.ParamD.BarD.ds, Row_Ref, Col_Ref, stRow);
			}
		}

		stRow += rowCnt;
	}
	else
	{
		ASSERT(0);
	}

	return TRUE;
}

BOOL CBrdgSeisEvalKISTEC2015_Report::GenerateShearResult_Longi_Else_Load(IExcel* pExcel, CString& RptBlock, int& stRow, _DGN_SEISEVAL_SHEAR_OUT& ShearOut, int nLongTran/* = 0*/)
{
	short Row_Ref = 0, Col_Ref = 0;

	// 다) 축하중 작용에 의한 전단강도
	RptBlock = _T("[2]Shear_Load_Head");
	int rowCnt = CopyRangeSubTitle(pExcel, RptBlock, stRow);
	stRow += rowCnt;

	int nLT = nLongTran;

	if (ShearOut.enBehavior[nLT] == CANTILEVER)
	{
		RptBlock = _T("[2]Shear_Load_Cantil_Longi");
		rowCnt = CopyRangeSubTitle(pExcel, RptBlock, stRow);
		GetCellIDByName(pExcel, RptBlock, Row_Ref, Col_Ref);

		if (ShearOut.ResultD[0][nLT].EvalD.bCheck)
		{
			PrintCellData(pExcel, _T("S6_3_P_Longi1"), ShearOut.ResultD[0][nLT].ParamD.ForceD.dPtop, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S6_3_T_Longi1"), ShearOut.ResultD[0][nLT].ParamD.SetcD.dH, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S6_3_L_Longi1"), ShearOut.ResultD[0][nLT].ParamD.OptD.dHe, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S6_3_ShearP_Longi"), ShearOut.ResultD[0][nLT].CapaD.ShearCapaD.dVp, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S6_3_P_Longi2"), ShearOut.ResultD[0][nLT].ParamD.ForceD.dPtop, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S6_3_T_Longi2"), ShearOut.ResultD[0][nLT].ParamD.SetcD.dH, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S6_3_L_Longi2"), ShearOut.ResultD[0][nLT].ParamD.OptD.dHe, Row_Ref, Col_Ref, stRow);
		}

		stRow += rowCnt + 1;
	}
	else
	{
		RptBlock = _T("[2]Shear_Load_Rahmen_Longi");
		rowCnt = CopyRangeSubTitle(pExcel, RptBlock, stRow);
		GetCellIDByName(pExcel, RptBlock, Row_Ref, Col_Ref);

		if (ShearOut.ResultD[0][nLT].EvalD.bCheck)
		{
			PrintCellData(pExcel, _T("S6_3_P_Bot_Longi1"), ShearOut.ResultD[0][nLT].ParamD.ForceD.dPtop, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S6_3_T_Bot_Longi1"), ShearOut.ResultD[0][nLT].ParamD.SetcD.dH, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S6_3_L_Bot_Longi1"), ShearOut.ResultD[0][nLT].ParamD.OptD.dHe, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S6_3_ShearP_Bot_Longi"), ShearOut.ResultD[0][nLT].CapaD.ShearCapaD.dVp, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S6_3_P_Bot_Longi2"), ShearOut.ResultD[0][nLT].ParamD.ForceD.dPtop, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S6_3_T_Bot_Longi2"), ShearOut.ResultD[0][nLT].ParamD.SetcD.dH, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S6_3_L_Bot_Longi2"), ShearOut.ResultD[0][nLT].ParamD.OptD.dHe, Row_Ref, Col_Ref, stRow);
		}

		if (ShearOut.ResultD[1][0].EvalD.bCheck)
		{
			PrintCellData(pExcel, _T("S6_3_P_Top_Longi1"), ShearOut.ResultD[1][nLT].ParamD.ForceD.dPtop, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S6_3_T_Top_Longi1"), ShearOut.ResultD[1][nLT].ParamD.SetcD.dH, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S6_3_L_Top_Longi1"), ShearOut.ResultD[1][nLT].ParamD.OptD.dHe, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S6_3_ShearP_Top_Longi"), ShearOut.ResultD[1][nLT].CapaD.ShearCapaD.dVp, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S6_3_P_Top_Longi2"), ShearOut.ResultD[1][nLT].ParamD.ForceD.dPtop, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S6_3_T_Top_Longi2"), ShearOut.ResultD[1][nLT].ParamD.SetcD.dH, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S6_3_L_Top_Longi2"), ShearOut.ResultD[1][nLT].ParamD.OptD.dHe, Row_Ref, Col_Ref, stRow);
		}

		stRow += rowCnt + 1;
	}

	return TRUE;
}

BOOL CBrdgSeisEvalKISTEC2015_Report::GenerateShearResult_Trans(IExcel* pExcel, CString& RptBlock, int& stRow, _DGN_SEISEVAL_SHEAR_OUT& ShearOut)
{
	// 2) 교축직각방향
	RptBlock = _T("[2]Shear_Trans");

	int rowCnt = CopyRangeSubTitle(pExcel, RptBlock, stRow);
	stRow += rowCnt;
	if (ShearOut.enPierType[1] == WALL)
	{
		ASSERT(0);
		GenerateShearResult_Trans_Wall(pExcel, RptBlock, stRow, ShearOut);
	}
	else
	{
		GenerateShearResult_Trans_Else(pExcel, RptBlock, stRow, ShearOut);
	}

	return TRUE;
}

BOOL CBrdgSeisEvalKISTEC2015_Report::GenerateShearResult_Trans_Mod(IExcel* pExcel, CString& RptBlock, int& stRow, _DGN_SEISEVAL_SHEAR_OUT& ShearOut)
{
	// 2) 교축직각방향
	RptBlock = _T("[2]Shear_Trans");

	int rowCnt = CopyRangeSubTitle(pExcel, RptBlock, stRow);
	stRow += rowCnt;

	if (ShearOut.enPierType[1] == WALL)
	{
		if (ShearOut.ResultD[0][1].ParamD.OptD.iEvalCode == KISTEC_2019 ||
			ShearOut.ResultD[0][1].ParamD.OptD.iEvalCode == KALIS_2023)
			GenerateShearResult_Longi_Wall_2019_byTranData(pExcel, RptBlock, stRow, ShearOut);
		else
			GenerateShearResult_Longi_Wall_byTranData(pExcel, RptBlock, stRow, ShearOut);
	}
	else
	{
		GenerateShearResult_Longi_Else_byTranData(pExcel, RptBlock, stRow, ShearOut);
	}
	return TRUE;
}


BOOL CBrdgSeisEvalKISTEC2015_Report::GenerateShearResult_Trans_Wall(IExcel* pExcel, CString& RptBlock, int& stRow, _DGN_SEISEVAL_SHEAR_OUT& ShearOut, int nLongTran/* = 1*/)
{
	RptBlock = _T("[2]Shear_Brick_Head");
	int rowCnt = CopyRangeSubTitle(pExcel, RptBlock, stRow);
	stRow += rowCnt;

	short Row_Ref = 0, Col_Ref = 0;
	RptBlock = _T("[2]Shear_Brick_Minor");
	rowCnt = CopyRangeSubTitle(pExcel, RptBlock, stRow);
	GetCellIDByName(pExcel, RptBlock, Row_Ref, Col_Ref);

	int nLT = nLongTran;

	T_BRDGEVAL_RESULT PrtResultD; PrtResultD.initialize();
	if (ShearOut.ResultD[0][nLT].EvalD.bCheck) PrtResultD = ShearOut.ResultD[0][nLT];
	else if (ShearOut.ResultD[1][nLT].EvalD.bCheck) PrtResultD = ShearOut.ResultD[1][nLT];

	double dH = (nLT == 1) ? PrtResultD.ParamD.SetcD.dH : PrtResultD.ParamD.SetcD.dB;
	double dB = (nLT == 1) ? PrtResultD.ParamD.SetcD.dB : PrtResultD.ParamD.SetcD.dH;

	if (PrtResultD.EvalD.bCheck)
	{
		PrintCellData(pExcel, _T("S6_1_1_B_Trans1"), 1000 * dH, Row_Ref, Col_Ref, stRow);
		PrintCellData(pExcel, _T("S6_1_1_H_Trans"), 1000 * dB, Row_Ref, Col_Ref, stRow);
		PrintCellData(pExcel, _T("S6_1_1_DC_Trans"), 1000 * PrtResultD.ParamD.BarD.ddc, Row_Ref, Col_Ref, stRow);
		PrintCellData(pExcel, _T("S6_1_1_D_Trans1"), 1000 * dB - 1000 * PrtResultD.ParamD.BarD.ddc, Row_Ref, Col_Ref, stRow);

		PrintCellData(pExcel, _T("S6_1_1_FCK_Trans1"), m_dFactorStress * PrtResultD.ParamD.MatlD.dfck, Row_Ref, Col_Ref, stRow);
		PrintCellData(pExcel, _T("S6_1_1_B_Trans2"), 1000 * dH, Row_Ref, Col_Ref, stRow);
		PrintCellData(pExcel, _T("S6_1_1_D_Trans2"), 1000 * dB - 1000 * PrtResultD.ParamD.BarD.ddc, Row_Ref, Col_Ref, stRow);
		PrintCellData(pExcel, _T("S6_1_1_VN1_Trans1"), PrtResultD.CapaD.ShearCapaD.dVn1, Row_Ref, Col_Ref, stRow);

		PrintCellData(pExcel, _T("S6_1_1_FCK_Trans2"), m_dFactorStress * PrtResultD.ParamD.MatlD.dfck, Row_Ref, Col_Ref, stRow);
		PrintCellData(pExcel, _T("S6_1_1_PSH_Trans"), PrtResultD.ParamD.BarD.drhosh, Row_Ref, Col_Ref, stRow);
		PrintCellData(pExcel, _T("S6_1_1_FYH_Trans"), m_dFactorStress * PrtResultD.ParamD.MatlD.dfyh, Row_Ref, Col_Ref, stRow);
		PrintCellData(pExcel, _T("S6_1_1_B_Trans3"), 1000 * dH, Row_Ref, Col_Ref, stRow);
		PrintCellData(pExcel, _T("S6_1_1_D_Trans3"), 1000 * dB - 1000 * PrtResultD.ParamD.BarD.ddc, Row_Ref, Col_Ref, stRow);
		PrintCellData(pExcel, _T("S6_1_1_VN2_Trans1"), PrtResultD.CapaD.ShearCapaD.dVn2, Row_Ref, Col_Ref, stRow);

		PrintCellData(pExcel, _T("S6_1_1_VN1_Trans2"), PrtResultD.CapaD.ShearCapaD.dVn1, Row_Ref, Col_Ref, stRow);
		PrintCellData(pExcel, _T("S6_1_1_VN2_Trans2"), PrtResultD.CapaD.ShearCapaD.dVn2, Row_Ref, Col_Ref, stRow);
		PrintCellData(pExcel, _T("S6_1_1_VN_FIN_Trans"), min(PrtResultD.CapaD.ShearCapaD.dVn1, PrtResultD.CapaD.ShearCapaD.dVn2), Row_Ref, Col_Ref, stRow);
	}
	stRow += rowCnt;

	return TRUE;
}

BOOL CBrdgSeisEvalKISTEC2015_Report::GenerateShearResult_Trans_Wall_byLongiData(IExcel* pExcel, CString& RptBlock, int& stRow, _DGN_SEISEVAL_SHEAR_OUT& ShearOut)
{
	GenerateShearResult_Trans_Wall(pExcel, RptBlock, stRow, ShearOut, 0);

	return TRUE;
}

BOOL CBrdgSeisEvalKISTEC2015_Report::GenerateShearResult_Longi_Wall_2019_byTranData(IExcel* pExcel, CString& RptBlock, int& stRow, _DGN_SEISEVAL_SHEAR_OUT& ShearOut)
{
	GenerateShearResult_Longi_Wall_2019(pExcel, RptBlock, stRow, ShearOut, 1);

	return TRUE;
}

BOOL CBrdgSeisEvalKISTEC2015_Report::GenerateShearResult_Longi_Wall_2019(IExcel* pExcel, CString& RptBlock, int& stRow, _DGN_SEISEVAL_SHEAR_OUT& ShearOut, int nLongTran/* = 0*/)
{
	RptBlock = _T("[2]Shear_Brick_Head_2019");
	int rowCnt = CopyRangeSubTitle(pExcel, RptBlock, stRow);
	stRow += rowCnt;

	short Row_Ref = 0, Col_Ref = 0;
	RptBlock = _T("[2]Shear_Brick_Minor_2019");
	rowCnt = CopyRangeSubTitle(pExcel, RptBlock, stRow);
	GetCellIDByName(pExcel, RptBlock, Row_Ref, Col_Ref);

	int nLT = nLongTran;

	T_BRDGEVAL_RESULT PrtResultD; PrtResultD.initialize();
	if (ShearOut.ResultD[0][nLT].EvalD.bCheck) PrtResultD = ShearOut.ResultD[0][nLT];
	else if (ShearOut.ResultD[1][nLT].EvalD.bCheck) PrtResultD = ShearOut.ResultD[1][nLT];

	double dH = (nLT == 0) ? PrtResultD.ParamD.SetcD.dH : PrtResultD.ParamD.SetcD.dB;
	double dB = (nLT == 0) ? PrtResultD.ParamD.SetcD.dB : PrtResultD.ParamD.SetcD.dH;

	double dd = dH - PrtResultD.ParamD.BarD.ddc;
	if (PrtResultD.EvalD.bCheck)
	{
		PrintCellData(pExcel, _T("S6_1_1_B_Trans_2019"), 1000.0 * dB, Row_Ref, Col_Ref, stRow);
		PrintCellData(pExcel, _T("S6_1_1_H_Trans_2019"), 1000.0 * dH, Row_Ref, Col_Ref, stRow);
		PrintCellData(pExcel, _T("S6_1_1_DC_Trans_2019"), 1000.0 * PrtResultD.ParamD.BarD.ddc, Row_Ref, Col_Ref, stRow);
		PrintCellData(pExcel, _T("S6_1_1_D_Trans_2019"), 1000.0 * dd, Row_Ref, Col_Ref, stRow);
	}
	stRow += rowCnt;

	if (PrtResultD.CapaD.ShearCapaD.nWallShearType == 0)   //(PrtResultD.ParamD.BarD.ds > 0.0)
	{
		RptBlock = _T("[2]Shear_Brick_Minor_2019_w_s");
		rowCnt = CopyRangeSubTitle(pExcel, RptBlock, stRow);
		GetCellIDByName(pExcel, RptBlock, Row_Ref, Col_Ref);

		if (PrtResultD.EvalD.bCheck)
		{
			double dTan = tan(PrtResultD.CapaD.ShearCapaD.dtheta*CMathFunc::m_trrad);
			double dCot = 1.0 / dTan;
			PrintCellData(pExcel, _T("S6_1_2_Vn1_fyh_2019"), m_dFactorStress * PrtResultD.ParamD.MatlD.dfyh, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S6_1_2_Vn1_Av_2019"), m_dFactorAreamm2 * PrtResultD.ParamD.BarD.dAv, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S6_1_2_Vn1_z_2019"), 1000.0 * 0.9 * dd, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S6_1_2_Vn1_s_2019"), 1000.0 * PrtResultD.ParamD.BarD.ds, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S6_1_2_Vn1_cos_2019"), dCot, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S6_1_2_Vn1_2019"), PrtResultD.CapaD.ShearCapaD.dVn1, Row_Ref, Col_Ref, stRow);

			PrintCellData(pExcel, _T("S6_1_2_Vn2_v_2019"), PrtResultD.CapaD.ShearCapaD.dv, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S6_1_2_Vn2_fck_2019"), m_dFactorStress * PrtResultD.ParamD.MatlD.dfck_S, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S6_1_2_Vn2_b_2019"), 1000.0 * dB, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S6_1_2_Vn2_z_2019"), 1000.0 * 0.9 * dd, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S6_1_2_Vn2_cos_2019"), dCot, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S6_1_2_Vn2_tan_2019"), dTan, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S6_1_2_Vn2_2019"), PrtResultD.CapaD.ShearCapaD.dVn2, Row_Ref, Col_Ref, stRow);

			PrintCellData(pExcel, _T("S6_1_2_minVn1_2019"), PrtResultD.CapaD.ShearCapaD.dVn1, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S6_1_2_minVn2_2019"), PrtResultD.CapaD.ShearCapaD.dVn2, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S6_1_2_Vn_2019"), min(PrtResultD.CapaD.ShearCapaD.dVn1, PrtResultD.CapaD.ShearCapaD.dVn2), Row_Ref, Col_Ref, stRow);
		}
		stRow += rowCnt;
	}
	else
	{
		RptBlock = _T("[2]Shear_Brick_Minor_2019_wo_s");
		rowCnt = CopyRangeSubTitle(pExcel, RptBlock, stRow);
		GetCellIDByName(pExcel, RptBlock, Row_Ref, Col_Ref);

		if (PrtResultD.EvalD.bCheck)
		{
			PrintCellData(pExcel, _T("S6_1_1_Vn1_k_2019"), PrtResultD.CapaD.ShearCapaD.dk, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S6_1_1_Vn1_rho_2019"), PrtResultD.CapaD.ShearCapaD.drho_slt, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S6_1_1_Vn1_fck_2019"), m_dFactorStress * PrtResultD.ParamD.MatlD.dfck_S, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S6_1_1_Vn1_fn_2019"), PrtResultD.CapaD.ShearCapaD.dfn, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S6_1_1_Vn1_b_2019"), 1000.0 * dB, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S6_1_1_Vn1_d_2019"), 1000.0 * dd, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S6_1_1_Vn1_2019"), PrtResultD.CapaD.ShearCapaD.dVn1, Row_Ref, Col_Ref, stRow);

			PrintCellData(pExcel, _T("S6_1_1_Vn2_fctk_2019"), m_dFactorStress * PrtResultD.ParamD.MatlD.dfctk_S, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S6_1_1_Vn2_fn_2019"), PrtResultD.CapaD.ShearCapaD.dfn, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S6_1_1_Vn2_b_2019"), 1000.0 * dB, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S6_1_1_Vn2_d_2019"), 1000.0 * dd, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S6_1_1_Vn2_2019"), PrtResultD.CapaD.ShearCapaD.dVn2, Row_Ref, Col_Ref, stRow);

			PrintCellData(pExcel, _T("S6_1_1_maxVn1_2019"), PrtResultD.CapaD.ShearCapaD.dVn1, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S6_1_1_maxVn2_2019"), PrtResultD.CapaD.ShearCapaD.dVn2, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S6_1_1_Vn_2019"), max(PrtResultD.CapaD.ShearCapaD.dVn1, PrtResultD.CapaD.ShearCapaD.dVn2), Row_Ref, Col_Ref, stRow);
		}
		stRow += rowCnt;
	}

	return TRUE;
}
BOOL CBrdgSeisEvalKISTEC2015_Report::GenerateShearResult_Trans_Else(IExcel* pExcel, CString& RptBlock, int& stRow, _DGN_SEISEVAL_SHEAR_OUT& ShearOut, int nLongTran/* = 1*/)
{
	BOOL bNotWall = (ShearOut.enPierType[0] != WALL && ShearOut.enPierType[1] != WALL);
	if (!bNotWall)
	{
		RptBlock = _T("[2]Graph_Disp_Duct_Shear_Strength");
		int rowCnt = CopyRangeSubTitle(pExcel, RptBlock, stRow);
		stRow += rowCnt;
	}

	int nLT = nLongTran;

	// 가) 콘크리트에 의한 전단강도
	if (nLT == 0) GenerateShearResult_Longi_Else_Conc(pExcel, RptBlock, stRow, ShearOut, nLT);
	else       GenerateShearResult_Trans_Else_Conc(pExcel, RptBlock, stRow, ShearOut, nLT);

	// 나) 전단철근에 의한 전단강도
	GenerateShearResult_Trans_Else_Rebar(pExcel, RptBlock, stRow, ShearOut, nLT);

	// 다) 축하중 작용에 의한 전단강도
	if (nLT == 0) GenerateShearResult_Longi_Else_Load(pExcel, RptBlock, stRow, ShearOut, nLT);
	else          GenerateShearResult_Trans_Else_Load(pExcel, RptBlock, stRow, ShearOut, nLT);

	return TRUE;
}

BOOL CBrdgSeisEvalKISTEC2015_Report::GenerateShearResult_Trans_Else_byLongiData(IExcel* pExcel, CString& RptBlock, int& stRow, _DGN_SEISEVAL_SHEAR_OUT& ShearOut)
{
	GenerateShearResult_Trans_Else(pExcel, RptBlock, stRow, ShearOut, 0);

	return TRUE;
}


BOOL CBrdgSeisEvalKISTEC2015_Report::GenerateShearResult_Trans_Else_Conc(IExcel* pExcel, CString& RptBlock, int& stRow, _DGN_SEISEVAL_SHEAR_OUT& ShearOut, int nLongTran/* = 1*/)
{
	short Row_Ref = 0, Col_Ref = 0;
	// 가) 콘크리트에 의한 전단강도
	RptBlock = _T("[2]Shear_Conc_Head");
	int rowCnt = CopyRangeSubTitle(pExcel, RptBlock, stRow);
	stRow += rowCnt;

	int nLT = nLongTran;

	if (ShearOut.enBehavior[nLT] == CANTILEVER)
	{
		RptBlock = _T("[2]Shear_Conc_Cantil_Trans");
		rowCnt = CopyRangeSubTitle(pExcel, RptBlock, stRow);
		GetCellIDByName(pExcel, RptBlock, Row_Ref, Col_Ref);

		if (ShearOut.ResultD[0][1].EvalD.bCheck)
		{
			PrintCellData(pExcel, _T("S6_1_FCK_Trans1"), m_dFactorStress	*	ShearOut.ResultD[0][nLT].ParamD.MatlD.dfck, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S6_1_AreaG_Trans1"), ShearOut.ResultD[0][nLT].ParamD.SetcD.dArea, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S6_1_AreaE_Trans1"), ShearOut.ResultD[0][nLT].ParamD.SetcD.dArea * ShearOut.ResultD[0][nLT].ParamD.OptD.dAeRat, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S6_1_FCK_Trans2"), m_dFactorStress	*	ShearOut.ResultD[0][nLT].ParamD.MatlD.dfck, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S6_1_AreaE_Trans2"), ShearOut.ResultD[0][nLT].ParamD.SetcD.dArea * ShearOut.ResultD[0][nLT].ParamD.OptD.dAeRat, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S6_1_ShearC_Trans"), ShearOut.ResultD[0][nLT].CapaD.ShearCapaD.dVc2, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S6_1_Dis_Y_Trans1"), ShearOut.ResultD[0][nLT].CapaD.DispDuctD.dDeltay, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S6_1_Dis_2_Trans"), 2 * ShearOut.ResultD[0][nLT].CapaD.DispDuctD.dDeltay, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S6_1_Dis_Y_Trans2"), ShearOut.ResultD[0][nLT].CapaD.DispDuctD.dDeltay, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S6_1_Dis_5_Trans"), 5 * ShearOut.ResultD[0][nLT].CapaD.DispDuctD.dDeltay, Row_Ref, Col_Ref, stRow);
		}

		stRow += rowCnt + 1;
	}
	else
	{
		RptBlock = _T("[2]Shear_Conc_Rahmen_Trans");
		rowCnt = CopyRangeSubTitle(pExcel, RptBlock, stRow);
		GetCellIDByName(pExcel, RptBlock, Row_Ref, Col_Ref);

		if (ShearOut.ResultD[0][nLT].EvalD.bCheck)
		{
			PrintCellData(pExcel, _T("S6_1_FCK_Bot_Trans1"), m_dFactorStress * ShearOut.ResultD[0][nLT].ParamD.MatlD.dfck, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S6_1_AreaG_Bot_Trans1"), ShearOut.ResultD[0][nLT].ParamD.SetcD.dArea, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S6_1_AreaE_Bot_Trans1"), ShearOut.ResultD[0][nLT].ParamD.SetcD.dArea * ShearOut.ResultD[0][nLT].ParamD.OptD.dAeRat, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S6_1_FCK_Bot_Trans2"), m_dFactorStress * ShearOut.ResultD[0][nLT].ParamD.MatlD.dfck, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S6_1_AreaE_Bot_Trans2"), ShearOut.ResultD[0][nLT].ParamD.SetcD.dArea * ShearOut.ResultD[0][nLT].ParamD.OptD.dAeRat, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S6_1_ShearC_Bot_Trans"), ShearOut.ResultD[0][nLT].CapaD.ShearCapaD.dVc2, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S6_1_Dis_Y_Bot_Trans1"), ShearOut.ResultD[0][nLT].CapaD.DispDuctD.dDeltay, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S6_1_Dis_2_Bot_Trans"), 2 * ShearOut.ResultD[0][nLT].CapaD.DispDuctD.dDeltay, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S6_1_Dis_Y_Bot_Trans2"), ShearOut.ResultD[0][nLT].CapaD.DispDuctD.dDeltay, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S6_1_Dis_5_Bot_Trans"), 5 * ShearOut.ResultD[0][nLT].CapaD.DispDuctD.dDeltay, Row_Ref, Col_Ref, stRow);
		}

		if (ShearOut.ResultD[1][nLT].EvalD.bCheck)
		{
			PrintCellData(pExcel, _T("S6_1_FCK_Top_Trans1"), m_dFactorStress	* ShearOut.ResultD[1][nLT].ParamD.MatlD.dfck, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S6_1_AreaG_Top_Trans1"), ShearOut.ResultD[1][nLT].ParamD.SetcD.dArea, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S6_1_AreaE_Top_Trans1"), ShearOut.ResultD[1][nLT].ParamD.SetcD.dArea * ShearOut.ResultD[1][nLT].ParamD.OptD.dAeRat, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S6_1_FCK_Top_Trans2"), m_dFactorStress	* ShearOut.ResultD[1][nLT].ParamD.MatlD.dfck, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S6_1_AreaE_Top_Trans2"), ShearOut.ResultD[1][nLT].ParamD.SetcD.dArea * ShearOut.ResultD[1][nLT].ParamD.OptD.dAeRat, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S6_1_ShearC_Top_Trans"), ShearOut.ResultD[1][nLT].CapaD.ShearCapaD.dVc2, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S6_1_Dis_Y_Top_Trans1"), ShearOut.ResultD[1][nLT].CapaD.DispDuctD.dDeltay, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S6_1_Dis_2_Top_Trans"), 2 * ShearOut.ResultD[1][nLT].CapaD.DispDuctD.dDeltay, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S6_1_Dis_Y_Top_Trans2"), ShearOut.ResultD[1][nLT].CapaD.DispDuctD.dDeltay, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S6_1_Dis_5_Top_Trans"), 5 * ShearOut.ResultD[1][nLT].CapaD.DispDuctD.dDeltay, Row_Ref, Col_Ref, stRow);
		}

		stRow += rowCnt + 1;
	}

	return TRUE;
}


BOOL CBrdgSeisEvalKISTEC2015_Report::GenerateShearResult_Trans_Else_Rebar(IExcel* pExcel, CString& RptBlock, int& stRow, _DGN_SEISEVAL_SHEAR_OUT& ShearOut, int nLongTran/* = 1*/)
{
	short Row_Ref = 0, Col_Ref = 0;
	// 나) 전단철근에 의한 전단강도
	RptBlock = _T("[2]Shear_Rebar_Head");
	int rowCnt = CopyRangeSubTitle(pExcel, RptBlock, stRow);
	stRow += rowCnt;

	int nLT = nLongTran;

	CString strShape;
	if (ShearOut.ResultD[0][nLT].EvalD.bCheck) { strShape = ShearOut.ResultD[0][nLT].ParamD.SetcD.strShape; }// 하부 교축직각
	else if (ShearOut.ResultD[1][nLT].EvalD.bCheck) { strShape = ShearOut.ResultD[1][nLT].ParamD.SetcD.strShape; }// 상부 교축직각

	T_BRDGEVAL_RESULT PrtResultD1, PrtResultD2;
	PrtResultD1.initialize();
	PrtResultD2.initialize();
	if (ShearOut.ResultD[0][nLT].EvalD.bCheck) { PrtResultD2 = ShearOut.ResultD[0][nLT]; }// 하부 교축직각
	else if (ShearOut.ResultD[1][nLT].EvalD.bCheck) { PrtResultD2 = ShearOut.ResultD[1][nLT]; }// 상부 교축직각

	if (strShape == _T("P") || strShape == _T("SR"))
	{
		RptBlock = _T("[2]Shear_Rebar_Circle");
		rowCnt = CopyRangeSubTitle(pExcel, RptBlock, stRow);
		GetCellIDByName(pExcel, RptBlock, Row_Ref, Col_Ref);

		if (PrtResultD2.EvalD.bCheck)
		{
			PrintCellData(pExcel, _T("S6_2_CIR_AreaV_Longi1"), (1 / m_dFactorStress) * (1 / m_dFactorStress)	*	PrtResultD2.ParamD.BarD.dAsp, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S6_2_CIR_FYH_Longi1"), m_dFactorStress* PrtResultD2.ParamD.MatlD.dfyh, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S6_2_CIR_D1_Longi1"), 1000 * PrtResultD2.CapaD.ShearCapaD.dD1, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S6_2_CIR_S_Longi1"), 1000 * PrtResultD2.ParamD.BarD.ds, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S6_2_CIR_ShearR_Longi"), PrtResultD2.CapaD.ShearCapaD.dVs, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S6_2_CIR_AreaV_Longi2"), (1 / m_dFactorStress) * (1 / m_dFactorStress)	*	PrtResultD2.ParamD.BarD.dAsp, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S6_2_CIR_FYH_Longi2"), m_dFactorStress* PrtResultD2.ParamD.MatlD.dfyh, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S6_2_CIR_D1_Longi2"), 1000 * PrtResultD2.CapaD.ShearCapaD.dD1, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S6_2_CIR_S_Longi2"), 1000 * PrtResultD2.ParamD.BarD.ds, Row_Ref, Col_Ref, stRow);
		}

		stRow += rowCnt;
	}
	else if (strShape == _T("B") || strShape == _T("SB") ||
		strShape == _T("OCT") || strShape == _T("SOCT") ||
		strShape == _T("GEN"))
	{
		RptBlock = _T("[2]Shear_Rebar_Box_Trans");
		rowCnt = CopyRangeSubTitle(pExcel, RptBlock, stRow);
		GetCellIDByName(pExcel, RptBlock, Row_Ref, Col_Ref);

		if (PrtResultD2.EvalD.bCheck)
		{
			PrintCellData(pExcel, _T("S6_2_BOX_AreaV_Trans1"), (1 / m_dFactorStress) * (1 / m_dFactorStress)	*	PrtResultD2.ParamD.BarD.dAv, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S6_2_BOX_FYH_Trans1"), m_dFactorStress* PrtResultD2.ParamD.MatlD.dfyh, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S6_2_BOX_D1_Trans1"), 1000 * PrtResultD2.CapaD.ShearCapaD.dD1, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S6_2_BOX_S_Trans1"), 1000 * PrtResultD2.ParamD.BarD.ds, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S6_2_BOX_ShearR_Trans"), PrtResultD2.CapaD.ShearCapaD.dVs, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S6_2_BOX_AreaV_Trans2"), (1 / m_dFactorStress) * (1 / m_dFactorStress)	*	PrtResultD2.ParamD.BarD.dAv, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S6_2_BOX_FYH_Trans2"), m_dFactorStress* PrtResultD2.ParamD.MatlD.dfyh, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S6_2_BOX_D1_Trans2"), 1000 * PrtResultD2.CapaD.ShearCapaD.dD1, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S6_2_BOX_S_Trans2"), 1000 * PrtResultD2.ParamD.BarD.ds, Row_Ref, Col_Ref, stRow);
		}
		stRow += rowCnt;
	}
	else if (strShape == _T("TRK") || strShape == _T("STRK"))
	{
		if (nLT == 1) // 원래
		{
			RptBlock = _T("[2]Shear_Rebar_Track_Trans");
			rowCnt = CopyRangeSubTitle(pExcel, RptBlock, stRow);
			GetCellIDByName(pExcel, RptBlock, Row_Ref, Col_Ref);

			if (PrtResultD2.EvalD.bCheck)
			{
				PrintCellData(pExcel, _T("S6_2_TRACK_AreaV_Trans1"), (1 / m_dFactorStress) * (1 / m_dFactorStress)	*	PrtResultD2.ParamD.BarD.dAsp, Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S6_2_TRACK_FYH_Trans1"), m_dFactorStress* PrtResultD2.ParamD.MatlD.dfyh, Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S6_2_TRACK_D1_Trans1"), 1000 * PrtResultD2.CapaD.ShearCapaD.dD1, Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S6_2_TRACK_S_Trans1"), 1000 * PrtResultD2.ParamD.BarD.ds, Row_Ref, Col_Ref, stRow);

				PrintCellData(pExcel, _T("S6_2_TRACK_AreaV_Trans2"), (1 / m_dFactorStress) * (1 / m_dFactorStress)	*	PrtResultD2.CapaD.ShearCapaD.dAvCal, Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S6_2_TRACK_FYH_Trans2"), m_dFactorStress* PrtResultD2.ParamD.MatlD.dfyh, Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S6_2_TRACK_D2_Trans1"), 1000 * PrtResultD2.CapaD.ShearCapaD.dD2, Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S6_2_TRACK_S_Trans2"), 1000 * PrtResultD2.ParamD.BarD.ds, Row_Ref, Col_Ref, stRow);

				PrintCellData(pExcel, _T("S6_2_TRACK_ShearR_Trans"), PrtResultD2.CapaD.ShearCapaD.dVs, Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S6_2_TRACK_AreaV_Trans3"), (1 / m_dFactorStress) * (1 / m_dFactorStress)	*	PrtResultD2.ParamD.BarD.dAsp, Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S6_2_TRACK_AreaV_Trans4"), (1 / m_dFactorStress) * (1 / m_dFactorStress)	*	PrtResultD2.CapaD.ShearCapaD.dAvCal, Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S6_2_TRACK_FYH_Trans3"), m_dFactorStress* PrtResultD2.ParamD.MatlD.dfyh, Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S6_2_TRACK_D1_Trans2"), 1000 * PrtResultD2.CapaD.ShearCapaD.dD1, Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S6_2_TRACK_D2_Trans2"), 1000 * PrtResultD2.CapaD.ShearCapaD.dD2, Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S6_2_TRACK_S_Trans3"), 1000 * PrtResultD2.ParamD.BarD.ds, Row_Ref, Col_Ref, stRow);
			}
		}
		else  // nLT==0 : 교직 형태에 교축 데이터 출력하는 경우
		{
			RptBlock = _T("[2]Shear_Rebar_Track_Longi");
			rowCnt = CopyRangeSubTitle(pExcel, RptBlock, stRow);
			GetCellIDByName(pExcel, RptBlock, Row_Ref, Col_Ref);

			if (PrtResultD2.EvalD.bCheck)
			{
				PrintCellData(pExcel, _T("S6_2_TRACK_AreaV_Longi1"), (1 / m_dFactorStress) * (1 / m_dFactorStress)	*	PrtResultD2.ParamD.BarD.dAsp, Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S6_2_TRACK_FYH_Longi1"), m_dFactorStress* PrtResultD2.ParamD.MatlD.dfyh, Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S6_2_TRACK_D1_Longi1"), 1000 * PrtResultD2.CapaD.ShearCapaD.dD1, Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S6_2_TRACK_S_Longi1"), 1000 * PrtResultD2.ParamD.BarD.ds, Row_Ref, Col_Ref, stRow);

				PrintCellData(pExcel, _T("S6_2_TRACK_AreaV2_Longi1"), (1 / m_dFactorStress) * (1 / m_dFactorStress)	*	PrtResultD2.CapaD.ShearCapaD.dAvCal, Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S6_2_TRACK_FYH2_Longi1"), m_dFactorStress* PrtResultD2.ParamD.MatlD.dfyh, Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S6_2_TRACK_D2_Longi1"), 1000 * PrtResultD2.CapaD.ShearCapaD.dD2, Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S6_2_TRACK_S2_Longi1"), 1000 * PrtResultD2.ParamD.BarD.ds, Row_Ref, Col_Ref, stRow);

				PrintCellData(pExcel, _T("S6_2_TRACK_ShearR_Longi"), PrtResultD2.CapaD.ShearCapaD.dVs, Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S6_2_TRACK_AreaV_Longi2"), (1 / m_dFactorStress) * (1 / m_dFactorStress)	*	PrtResultD2.ParamD.BarD.dAsp, Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S6_2_TRACK_AreaV_Longi4"), (1 / m_dFactorStress) * (1 / m_dFactorStress)	*	PrtResultD2.CapaD.ShearCapaD.dAvCal, Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S6_2_TRACK_FYH_Longi2"), m_dFactorStress* PrtResultD2.ParamD.MatlD.dfyh, Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S6_2_TRACK_D1_Longi2"), 1000 * PrtResultD2.CapaD.ShearCapaD.dD1, Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S6_2_TRACK_D2_Longi2"), 1000 * PrtResultD2.CapaD.ShearCapaD.dD2, Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S6_2_TRACK_S_Longi2"), 1000 * PrtResultD2.ParamD.BarD.ds, Row_Ref, Col_Ref, stRow);
			}
		}

		stRow += rowCnt;
	}
	else if (strShape == _T("HTRK"))
	{
		if (nLT == 0)
		{
			RptBlock = _T("[2]Shear_Rebar_HalfTrack_Longi");
			rowCnt = CopyRangeSubTitle(pExcel, RptBlock, stRow);
			GetCellIDByName(pExcel, RptBlock, Row_Ref, Col_Ref);

			if (PrtResultD2.EvalD.bCheck)
			{
				PrtResultD1 = PrtResultD2;

				PrintCellData(pExcel, _T("S6_2_HTRACK_AreaV_Longi1"), (1 / m_dFactorStress) * (1 / m_dFactorStress)	*	PrtResultD1.ParamD.BarD.dAsp, Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S6_2_HTRACK_FYH_Longi1"), m_dFactorStress* PrtResultD1.ParamD.MatlD.dfyh, Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S6_2_HTRACK_D1_Longi1"), 1000 * PrtResultD1.CapaD.ShearCapaD.dD1, Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S6_2_HTRACK_S_Longi1"), 1000 * PrtResultD1.ParamD.BarD.ds, Row_Ref, Col_Ref, stRow);

				PrintCellData(pExcel, _T("S6_2_HTRACK_AreaV_Longi2"), (1 / m_dFactorStress) * (1 / m_dFactorStress)	*	PrtResultD1.CapaD.ShearCapaD.dAvCal, Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S6_2_HTRACK_FYH_Longi2"), m_dFactorStress* PrtResultD1.ParamD.MatlD.dfyh, Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S6_2_HTRACK_D1_Longi2"), 1000 * PrtResultD1.CapaD.ShearCapaD.dD2, Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S6_2_HTRACK_S_Longi2"), 1000 * PrtResultD1.ParamD.BarD.ds, Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S6_2_HTRACK_ShearR_Longi"), PrtResultD1.CapaD.ShearCapaD.dVs, Row_Ref, Col_Ref, stRow);

				PrintCellData(pExcel, _T("S6_2_HTRACK_AreaV_Longi3"), (1 / m_dFactorStress) * (1 / m_dFactorStress)	*	PrtResultD1.ParamD.BarD.dAsp, Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S6_2_HTRACK_FYH_Longi3"), m_dFactorStress* PrtResultD1.ParamD.MatlD.dfyh, Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S6_2_HTRACK_D1_Longi3"), 1000 * PrtResultD1.CapaD.ShearCapaD.dD1, Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S6_2_HTRACK_S_Longi3"), 1000 * PrtResultD1.ParamD.BarD.ds, Row_Ref, Col_Ref, stRow);
			}

		}
		else
		{
			RptBlock = _T("[2]Shear_Rebar_HalfTrack_Trans");
			rowCnt = CopyRangeSubTitle(pExcel, RptBlock, stRow);
			GetCellIDByName(pExcel, RptBlock, Row_Ref, Col_Ref);

			if (PrtResultD2.EvalD.bCheck)
			{
				PrintCellData(pExcel, _T("S6_2_HTRACK_AreaV_Trans1"), (1 / m_dFactorStress) * (1 / m_dFactorStress)	*	PrtResultD2.ParamD.BarD.dAsp, Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S6_2_HTRACK_FYH_Trans1"), m_dFactorStress* PrtResultD2.ParamD.MatlD.dfyh, Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S6_2_HTRACK_D1_Trans1"), 1000 * PrtResultD2.CapaD.ShearCapaD.dD1, Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S6_2_HTRACK_S_Trans1"), 1000 * PrtResultD2.ParamD.BarD.ds, Row_Ref, Col_Ref, stRow);

				PrintCellData(pExcel, _T("S6_2_HTRACK_AreaV_Trans2"), (1 / m_dFactorStress) * (1 / m_dFactorStress)	*	PrtResultD2.CapaD.ShearCapaD.dAvCal, Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S6_2_HTRACK_FYH_Trans2"), m_dFactorStress* PrtResultD2.ParamD.MatlD.dfyh, Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S6_2_HTRACK_D2_Trans1"), 1000 * PrtResultD2.CapaD.ShearCapaD.dD2, Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S6_2_HTRACK_S_Trans2"), 1000 * PrtResultD2.ParamD.BarD.ds, Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S6_2_HTRACK_ShearR_Trans"), PrtResultD2.CapaD.ShearCapaD.dVs, Row_Ref, Col_Ref, stRow);

				PrintCellData(pExcel, _T("S6_2_HTRACK_AreaV_Trans3"), (1 / m_dFactorStress) * (1 / m_dFactorStress)	*	PrtResultD2.ParamD.BarD.dAsp, Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S6_2_HTRACK_AreaV_Trans4"), (1 / m_dFactorStress) * (1 / m_dFactorStress)	*	PrtResultD2.CapaD.ShearCapaD.dAvCal, Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S6_2_HTRACK_FYH_Trans3"), m_dFactorStress* PrtResultD2.ParamD.MatlD.dfyh, Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S6_2_HTRACK_D1_Trans2"), 1000 * PrtResultD2.CapaD.ShearCapaD.dD1, Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S6_2_HTRACK_D2_Trans2"), 1000 * PrtResultD2.CapaD.ShearCapaD.dD2, Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S6_2_HTRACK_S_Trans3"), 1000 * PrtResultD2.ParamD.BarD.ds, Row_Ref, Col_Ref, stRow);
			}
		}

		stRow += rowCnt;
	}
	else
	{
		ASSERT(0);
	}

	return TRUE;
}

BOOL CBrdgSeisEvalKISTEC2015_Report::GenerateShearResult_Trans_Else_Load(IExcel* pExcel, CString& RptBlock, int& stRow, _DGN_SEISEVAL_SHEAR_OUT& ShearOut, int nLongTran/* = 1*/)
{
	short Row_Ref = 0, Col_Ref = 0;
	// 다) 축하중 작용에 의한 전단강도
	RptBlock = _T("[2]Shear_Load_Head");
	int rowCnt = CopyRangeSubTitle(pExcel, RptBlock, stRow);
	stRow += rowCnt;

	int nLT = nLongTran;

	if (ShearOut.enBehavior[nLT] == CANTILEVER)
	{
		RptBlock = _T("[2]Shear_Load_Cantil_Trans");
		rowCnt = CopyRangeSubTitle(pExcel, RptBlock, stRow);
		GetCellIDByName(pExcel, RptBlock, Row_Ref, Col_Ref);

		if (ShearOut.ResultD[0][nLT].EvalD.bCheck)
		{
			double dB = (nLT == 1) ? ShearOut.ResultD[0][nLT].ParamD.SetcD.dB : ShearOut.ResultD[0][nLT].ParamD.SetcD.dH;

			PrintCellData(pExcel, _T("S6_3_P_Trans1"), ShearOut.ResultD[0][nLT].ParamD.ForceD.dPtop, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S6_3_T_Trans1"), dB, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S6_3_L_Trans1"), ShearOut.ResultD[0][nLT].ParamD.OptD.dHe, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S6_3_ShearP_Trans"), ShearOut.ResultD[0][nLT].CapaD.ShearCapaD.dVp, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S6_3_P_Trans2"), ShearOut.ResultD[0][nLT].ParamD.ForceD.dPtop, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S6_3_T_Trans2"), dB, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S6_3_L_Trans2"), ShearOut.ResultD[0][nLT].ParamD.OptD.dHe, Row_Ref, Col_Ref, stRow);
		}

		stRow += rowCnt + 1;
	}
	else
	{
		RptBlock = _T("[2]Shear_Load_Rahmen_Trans");
		rowCnt = CopyRangeSubTitle(pExcel, RptBlock, stRow);
		GetCellIDByName(pExcel, RptBlock, Row_Ref, Col_Ref);

		if (ShearOut.ResultD[0][nLT].EvalD.bCheck)
		{
			double dB = (nLT == 1) ? ShearOut.ResultD[0][nLT].ParamD.SetcD.dB : ShearOut.ResultD[0][nLT].ParamD.SetcD.dH;

			PrintCellData(pExcel, _T("S6_3_P_Bot_Trans1"), ShearOut.ResultD[0][nLT].ParamD.ForceD.dPtop, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S6_3_T_Bot_Trans1"), dB, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S6_3_L_Bot_Trans1"), ShearOut.ResultD[0][nLT].ParamD.OptD.dHe, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S6_3_ShearP_Bot_Trans"), ShearOut.ResultD[0][nLT].CapaD.ShearCapaD.dVp, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S6_3_P_Bot_Trans2"), ShearOut.ResultD[0][nLT].ParamD.ForceD.dPtop, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S6_3_T_Bot_Trans2"), dB, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S6_3_L_Bot_Trans2"), ShearOut.ResultD[0][nLT].ParamD.OptD.dHe, Row_Ref, Col_Ref, stRow);
		}

		if (ShearOut.ResultD[1][nLT].EvalD.bCheck)
		{
			double dB = (nLT == 1) ? ShearOut.ResultD[1][nLT].ParamD.SetcD.dB : ShearOut.ResultD[1][nLT].ParamD.SetcD.dH;

			PrintCellData(pExcel, _T("S6_3_P_Top_Trans1"), ShearOut.ResultD[1][nLT].ParamD.ForceD.dPtop, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S6_3_T_Top_Trans1"), dB, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S6_3_L_Top_Trans1"), ShearOut.ResultD[1][nLT].ParamD.OptD.dHe, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S6_3_ShearP_Top_Trans"), ShearOut.ResultD[1][nLT].CapaD.ShearCapaD.dVp, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S6_3_P_Top_Trans2"), ShearOut.ResultD[1][nLT].ParamD.ForceD.dPtop, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S6_3_T_Top_Trans2"), dB, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("S6_3_L_Top_Trans2"), ShearOut.ResultD[1][nLT].ParamD.OptD.dHe, Row_Ref, Col_Ref, stRow);
		}

		stRow += rowCnt + 1;
	}

	return TRUE;
}

BOOL CBrdgSeisEvalKISTEC2015_Report::GenerateShearResult_Longi_Strn(IExcel* pExcel, CString& RptBlock, int& stRow, _DGN_SEISEVAL_SHEAR_OUT& ShearOut)
{
	// 3) 공급변위연성도에 따른 교축방향 교각의 전단강도
	if (ShearOut.enPierType[0] == WALL)
	{
		GenerateShearResult_Longi_Strn_Wall(pExcel, RptBlock, stRow, ShearOut);
	}
	else
	{
		GenerateShearResult_Longi_Strn_Else(pExcel, RptBlock, stRow, ShearOut);
	}

	return TRUE;
}


BOOL CBrdgSeisEvalKISTEC2015_Report::GenerateShearResult_Longi_Strn_Wall(IExcel* pExcel, CString& RptBlock, int& stRow, _DGN_SEISEVAL_SHEAR_OUT& ShearOut)
{
	short Row_Ref = 0, Col_Ref = 0;
	// 3) 공급변위연성도에 따른 교축방향 교각의 전단강도 - 벽식
	RptBlock = _T("[2]Summary_Disp_Shear_Wall_Longi_Head");
	int rowCnt = CopyRangeSubTitle(pExcel, RptBlock, stRow);
	stRow += rowCnt + 1;

	CString strShearFileName;
	strShearFileName.Format(_T("%s_%s_%s_%s_Shear.emf"), m_strPicPath, ShearOut.strEvgpName, _T("LONGI"), _T("BOTTOM"));
	CDgnSeisFailureModeDlg dlg;
	dlg.MakeEmfAndClose(ShearOut.EvgpK, ShearOut.iColm, LONGI, BOTTOM, FALSE, TRUE, strShearFileName);

	T_BRDGEVAL_RESULT PrtResultD; PrtResultD.initialize();
	if (ShearOut.ResultD[0][0].EvalD.bCheck)
	{
		PrtResultD = ShearOut.ResultD[0][0];
	}
	else if (ShearOut.ResultD[1][0].EvalD.bCheck)
	{
		PrtResultD = ShearOut.ResultD[1][0];

		strShearFileName.Format(_T("%s_%s_%s_%s_Shear.emf"), m_strPicPath, ShearOut.strEvgpName, _T("LONGI"), _T("TOP"));
		dlg.MakeEmfAndClose(ShearOut.EvgpK, ShearOut.iColm, LONGI, TOP, FALSE, TRUE, strShearFileName);
	}
	dlg.DoModal();

	RptBlock = _T("[2]Summary_Disp_Shear_Cantil_Longi");
	rowCnt = CopyRangeSubTitle(pExcel, RptBlock, stRow);
	GetCellIDByName(pExcel, RptBlock, Row_Ref, Col_Ref);

	PrintCellData(pExcel, _T("Shear_Img_Longi"), PrintMomentShearGraph(pExcel, strShearFileName, stRow, 3), Row_Ref, Col_Ref, stRow);

	if (PrtResultD.EvalD.bCheck)
	{
		PrintCellData(pExcel, _T("Tab_2_SDist_Longi"), 2000 * PrtResultD.CapaD.BendCapaD.dDeltay, Row_Ref, Col_Ref, stRow);
		PrintCellData(pExcel, _T("Tab_5_SDist_Longi"), 5000 * PrtResultD.CapaD.BendCapaD.dDeltay, Row_Ref, Col_Ref, stRow);
		PrintCellData(pExcel, _T("Tab_8_SDist_Longi"), 8000 * PrtResultD.CapaD.BendCapaD.dDeltay, Row_Ref, Col_Ref, stRow);

		PrintCellData(pExcel, _T("Tab_0_SStress_Longi"), min(PrtResultD.CapaD.ShearCapaD.dVn1, PrtResultD.CapaD.ShearCapaD.dVn2), Row_Ref, Col_Ref, stRow);
		PrintCellData(pExcel, _T("Tab_2_SStress_Longi"), min(PrtResultD.CapaD.ShearCapaD.dVn1, PrtResultD.CapaD.ShearCapaD.dVn2), Row_Ref, Col_Ref, stRow);
		PrintCellData(pExcel, _T("Tab_5_SStress_Longi"), min(PrtResultD.CapaD.ShearCapaD.dVn1, PrtResultD.CapaD.ShearCapaD.dVn2), Row_Ref, Col_Ref, stRow);
		PrintCellData(pExcel, _T("Tab_8_SStress_Longi"), min(PrtResultD.CapaD.ShearCapaD.dVn1, PrtResultD.CapaD.ShearCapaD.dVn2), Row_Ref, Col_Ref, stRow);
	}

	stRow += rowCnt;

	return TRUE;
}

BOOL CBrdgSeisEvalKISTEC2015_Report::GenerateShearResult_Longi_Strn_Else(IExcel* pExcel, CString& RptBlock, int& stRow, _DGN_SEISEVAL_SHEAR_OUT& ShearOut)
{
	short Row_Ref = 0, Col_Ref = 0;
	// 3) 공급변위연성도에 따른 교축방향 교각의 전단강도
	RptBlock = _T("[2]Summary_Disp_Shear_Longi_Head");
	int rowCnt = CopyRangeSubTitle(pExcel, RptBlock, stRow);
	stRow += rowCnt;

	if (ShearOut.enBehavior[0] == CANTILEVER)
	{
		RptBlock = _T("[2]Summary_Disp_Shear_Cantil_Longi");
		rowCnt = CopyRangeSubTitle(pExcel, RptBlock, stRow);
		GetCellIDByName(pExcel, RptBlock, Row_Ref, Col_Ref);

		CString strShearFileName;
		strShearFileName.Format(_T("%s_%s_%s_%s_Shear.emf"), m_strPicPath, ShearOut.strEvgpName, _T("LONGI"), _T("BOTTOM"));
		CDgnSeisFailureModeDlg dlg;
		dlg.MakeEmfAndClose(ShearOut.EvgpK, ShearOut.iColm, LONGI, BOTTOM, FALSE, TRUE, strShearFileName);
		dlg.DoModal();

		PrintCellData(pExcel, _T("Shear_Img_Longi"), PrintMomentShearGraph(pExcel, strShearFileName, stRow, 3), Row_Ref, Col_Ref, stRow);

		if (ShearOut.ResultD[0][0].EvalD.bCheck)
		{
			PrintCellData(pExcel, _T("Tab_2_SDist_Longi"), 2000 * ShearOut.ResultD[0][0].CapaD.BendCapaD.dDeltay, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("Tab_5_SDist_Longi"), 5000 * ShearOut.ResultD[0][0].CapaD.BendCapaD.dDeltay, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("Tab_8_SDist_Longi"), 8000 * ShearOut.ResultD[0][0].CapaD.BendCapaD.dDeltay, Row_Ref, Col_Ref, stRow);

			PrintCellData(pExcel, _T("Tab_0_SStress_Longi"), ShearOut.dVc2[0] + ShearOut.dVp[0] + ShearOut.dVs[0], Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("Tab_2_SStress_Longi"), ShearOut.dVc2[0] + ShearOut.dVp[0] + ShearOut.dVs[0], Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("Tab_5_SStress_Longi"), ShearOut.dVc5[0] + ShearOut.dVp[0] + ShearOut.dVs[0], Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("Tab_8_SStress_Longi"), ShearOut.dVc5[0] + ShearOut.dVp[0] + ShearOut.dVs[0], Row_Ref, Col_Ref, stRow);
		}

		stRow += rowCnt;
	}
	else
	{
		RptBlock = _T("[2]Summary_Disp_Shear_Rahmen_Longi");
		rowCnt = CopyRangeSubTitle(pExcel, RptBlock, stRow);
		GetCellIDByName(pExcel, RptBlock, Row_Ref, Col_Ref);

		CString strShearFileName;
		strShearFileName.Format(_T("%s_%s_%s_%s_Shear.emf"), m_strPicPath, ShearOut.strEvgpName, _T("LONGI"), _T("BOTTOM"));
		CDgnSeisFailureModeDlg dlg;
		dlg.MakeEmfAndClose(ShearOut.EvgpK, ShearOut.iColm, LONGI, BOTTOM, FALSE, TRUE, strShearFileName);
		dlg.DoModal();

		PrintCellData(pExcel, _T("Shear_Img_Bot_Longi"), PrintMomentShearGraph(pExcel, strShearFileName, stRow + 1, 3), Row_Ref, Col_Ref, stRow);

		if (ShearOut.ResultD[0][0].EvalD.bCheck)
		{
			PrintCellData(pExcel, _T("Tab_2_SDist_Bot_Longi"), 2000 * ShearOut.ResultD[0][0].CapaD.BendCapaD.dDeltay, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("Tab_5_SDist_Bot_Longi"), 5000 * ShearOut.ResultD[0][0].CapaD.BendCapaD.dDeltay, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("Tab_8_SDist_Bot_Longi"), 8000 * ShearOut.ResultD[0][0].CapaD.BendCapaD.dDeltay, Row_Ref, Col_Ref, stRow);

			PrintCellData(pExcel, _T("Tab_0_SStress_Bot_Longi"), ShearOut.dVc2[0] + ShearOut.dVp[0] + ShearOut.dVs[0], Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("Tab_2_SStress_Bot_Longi"), ShearOut.dVc2[0] + ShearOut.dVp[0] + ShearOut.dVs[0], Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("Tab_5_SStress_Bot_Longi"), ShearOut.dVc5[0] + ShearOut.dVp[0] + ShearOut.dVs[0], Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("Tab_8_SStress_Bot_Longi"), ShearOut.dVc5[0] + ShearOut.dVp[0] + ShearOut.dVs[0], Row_Ref, Col_Ref, stRow);
		}

		CDgnSeisFailureModeDlg dlg2;
		strShearFileName.Format(_T("%s_%s_%s_%s_Shear.emf"), m_strPicPath, ShearOut.strEvgpName, _T("LONGI"), _T("TOP"));
		dlg2.MakeEmfAndClose(ShearOut.EvgpK, ShearOut.iColm, LONGI, TOP, FALSE, TRUE, strShearFileName);
		dlg2.DoModal();

		PrintCellData(pExcel, _T("Shear_Img_Top_Longi"), PrintMomentShearGraph(pExcel, strShearFileName, (stRow + 1) + 22, 3), Row_Ref, Col_Ref, stRow);

		if (ShearOut.ResultD[1][0].EvalD.bCheck)
		{
			PrintCellData(pExcel, _T("Tab_2_SDist_Top_Longi"), 2000 * ShearOut.ResultD[1][0].CapaD.BendCapaD.dDeltay, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("Tab_5_SDist_Top_Longi"), 5000 * ShearOut.ResultD[1][0].CapaD.BendCapaD.dDeltay, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("Tab_8_SDist_Top_Longi"), 8000 * ShearOut.ResultD[1][0].CapaD.BendCapaD.dDeltay, Row_Ref, Col_Ref, stRow);

			PrintCellData(pExcel, _T("Tab_0_SStress_Top_Longi"), ShearOut.dVc2[2] + ShearOut.dVp[2] + ShearOut.dVs[2], Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("Tab_2_SStress_Top_Longi"), ShearOut.dVc2[2] + ShearOut.dVp[2] + ShearOut.dVs[2], Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("Tab_5_SStress_Top_Longi"), ShearOut.dVc5[2] + ShearOut.dVp[2] + ShearOut.dVs[2], Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("Tab_8_SStress_Top_Longi"), ShearOut.dVc5[2] + ShearOut.dVp[2] + ShearOut.dVs[2], Row_Ref, Col_Ref, stRow);
		}

		stRow += rowCnt;
	}

	return TRUE;
}

BOOL CBrdgSeisEvalKISTEC2015_Report::GenerateShearResult_Trans_Strn(IExcel* pExcel, CString& RptBlock, int& stRow, _DGN_SEISEVAL_SHEAR_OUT& ShearOut)
{
	// 3) 공급변위연성도에 따른 교축직각방향 교각의 전단강도
	if (ShearOut.enPierType[1] == WALL)
	{
		//ASSERT(0);
		GenerateShearResult_Trans_Strn_Wall(pExcel, RptBlock, stRow, ShearOut);
	}
	else
	{
		GenerateShearResult_Trans_Strn_Else(pExcel, RptBlock, stRow, ShearOut);
	}

	return TRUE;
}

BOOL CBrdgSeisEvalKISTEC2015_Report::GenerateShearResult_Trans_Strn_Wall(IExcel* pExcel, CString& RptBlock, int& stRow, _DGN_SEISEVAL_SHEAR_OUT& ShearOut)
{
	short Row_Ref = 0, Col_Ref = 0;
	// 4) 공급변위연성도에 따른 교축직각방향 교각의 전단강도 - 벽식
	RptBlock = _T("[2]Summary_Disp_Shear_Wall_Trans_Head");
	int rowCnt = CopyRangeSubTitle(pExcel, RptBlock, stRow);
	stRow += rowCnt + 1;

	CString strShearFileName;
	strShearFileName.Format(_T("%s_%s_%s_%s_Shear.emf"), m_strPicPath, ShearOut.strEvgpName, _T("TRANS"), _T("BOTTOM"));
	CDgnSeisFailureModeDlg dlg;
	dlg.MakeEmfAndClose(ShearOut.EvgpK, ShearOut.iColm, TRANS, BOTTOM, FALSE, TRUE, strShearFileName);

	T_BRDGEVAL_RESULT PrtResultD; PrtResultD.initialize();
	if (ShearOut.ResultD[0][1].EvalD.bCheck)
	{
		PrtResultD = ShearOut.ResultD[0][1];
	}
	else if (ShearOut.ResultD[1][1].EvalD.bCheck)
	{
		PrtResultD = ShearOut.ResultD[1][1];

		strShearFileName.Format(_T("%s_%s_%s_%s_Shear.emf"), m_strPicPath, ShearOut.strEvgpName, _T("TRANS"), _T("TOP"));
		dlg.MakeEmfAndClose(ShearOut.EvgpK, ShearOut.iColm, TRANS, TOP, FALSE, TRUE, strShearFileName);
	}
	dlg.DoModal();

	RptBlock = _T("[2]Summary_Disp_Shear_Cantil_Trans");
	rowCnt = CopyRangeSubTitle(pExcel, RptBlock, stRow);
	GetCellIDByName(pExcel, RptBlock, Row_Ref, Col_Ref);

	PrintCellData(pExcel, _T("Shear_Img_Trans"), PrintMomentShearGraph(pExcel, strShearFileName, stRow, 3), Row_Ref, Col_Ref, stRow);

	if (PrtResultD.EvalD.bCheck)
	{
		PrintCellData(pExcel, _T("Tab_2_SDist_Trans"), 2000 * PrtResultD.CapaD.BendCapaD.dDeltay, Row_Ref, Col_Ref, stRow);
		PrintCellData(pExcel, _T("Tab_5_SDist_Trans"), 5000 * PrtResultD.CapaD.BendCapaD.dDeltay, Row_Ref, Col_Ref, stRow);
		PrintCellData(pExcel, _T("Tab_8_SDist_Trans"), 8000 * PrtResultD.CapaD.BendCapaD.dDeltay, Row_Ref, Col_Ref, stRow);

		double dVn1_dVn2 = 0.0;
		if (ShearOut.ResultD[0][0].ParamD.OptD.iEvalCode == KISTEC_2019 ||
			ShearOut.ResultD[0][0].ParamD.OptD.iEvalCode == KALIS_2023)
			dVn1_dVn2 = max(PrtResultD.CapaD.ShearCapaD.dVn1, PrtResultD.CapaD.ShearCapaD.dVn2);
		else
			dVn1_dVn2 = min(PrtResultD.CapaD.ShearCapaD.dVn1, PrtResultD.CapaD.ShearCapaD.dVn2);

		PrintCellData(pExcel, _T("Tab_0_SStress_Trans"), dVn1_dVn2, Row_Ref, Col_Ref, stRow);
		PrintCellData(pExcel, _T("Tab_2_SStress_Trans"), dVn1_dVn2, Row_Ref, Col_Ref, stRow);
		PrintCellData(pExcel, _T("Tab_5_SStress_Trans"), dVn1_dVn2, Row_Ref, Col_Ref, stRow);
		PrintCellData(pExcel, _T("Tab_8_SStress_Trans"), dVn1_dVn2, Row_Ref, Col_Ref, stRow);
	}

	stRow += rowCnt;

	return TRUE;
}

BOOL CBrdgSeisEvalKISTEC2015_Report::GenerateShearResult_Trans_Strn_Else(IExcel* pExcel, CString& RptBlock, int& stRow, _DGN_SEISEVAL_SHEAR_OUT& ShearOut)
{
	short Row_Ref = 0, Col_Ref = 0;
	// 4) 공급변위연성도에 따른 교축직각방향 교각의 전단강도
	RptBlock = _T("[2]Summary_Disp_Shear_Trans_Head");
	int rowCnt = CopyRangeSubTitle(pExcel, RptBlock, stRow);
	stRow += rowCnt;

	if (ShearOut.enBehavior[1] == CANTILEVER)
	{
		RptBlock = _T("[2]Summary_Disp_Shear_Cantil_Trans");
		rowCnt = CopyRangeSubTitle(pExcel, RptBlock, stRow);
		GetCellIDByName(pExcel, RptBlock, Row_Ref, Col_Ref);

		CString strShearFileName;
		strShearFileName.Format(_T("%s_%s_%s_%s_Shear.emf"), m_strPicPath, ShearOut.strEvgpName, _T("TRANS"), _T("BOTTOM"));
		CDgnSeisFailureModeDlg dlg;
		dlg.MakeEmfAndClose(ShearOut.EvgpK, ShearOut.iColm, TRANS, BOTTOM, FALSE, TRUE, strShearFileName);
		dlg.DoModal();

		PrintCellData(pExcel, _T("Shear_Img_Trans"), PrintMomentShearGraph(pExcel, strShearFileName, stRow, 3), Row_Ref, Col_Ref, stRow);

		if (ShearOut.ResultD[0][1].EvalD.bCheck)
		{
			PrintCellData(pExcel, _T("Tab_2_SDist_Trans"), 2000 * ShearOut.ResultD[0][1].CapaD.BendCapaD.dDeltay, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("Tab_5_SDist_Trans"), 5000 * ShearOut.ResultD[0][1].CapaD.BendCapaD.dDeltay, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("Tab_8_SDist_Trans"), 8000 * ShearOut.ResultD[0][1].CapaD.BendCapaD.dDeltay, Row_Ref, Col_Ref, stRow);

			PrintCellData(pExcel, _T("Tab_0_SStress_Trans"), ShearOut.dVc2[1] + ShearOut.dVp[1] + ShearOut.dVs[1], Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("Tab_2_SStress_Trans"), ShearOut.dVc2[1] + ShearOut.dVp[1] + ShearOut.dVs[1], Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("Tab_5_SStress_Trans"), ShearOut.dVc5[1] + ShearOut.dVp[1] + ShearOut.dVs[1], Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("Tab_8_SStress_Trans"), ShearOut.dVc5[1] + ShearOut.dVp[1] + ShearOut.dVs[1], Row_Ref, Col_Ref, stRow);
		}

		stRow += rowCnt;
	}
	else
	{
		RptBlock = _T("[2]Summary_Disp_Shear_Rahmen_Trans");
		rowCnt = CopyRangeSubTitle(pExcel, RptBlock, stRow);
		GetCellIDByName(pExcel, RptBlock, Row_Ref, Col_Ref);

		CString strShearFileName;
		strShearFileName.Format(_T("%s_%s_%s_%s_Shear.emf"), m_strPicPath, ShearOut.strEvgpName, _T("TRANS"), _T("BOTTOM"));
		CDgnSeisFailureModeDlg dlg;
		dlg.MakeEmfAndClose(ShearOut.EvgpK, ShearOut.iColm, TRANS, BOTTOM, FALSE, TRUE, strShearFileName);
		dlg.DoModal();

		PrintCellData(pExcel, _T("Shear_Img_Bot_Trans"), PrintMomentShearGraph(pExcel, strShearFileName, stRow + 1, 3), Row_Ref, Col_Ref, stRow);

		if (ShearOut.ResultD[0][1].EvalD.bCheck)
		{
			PrintCellData(pExcel, _T("Tab_2_SDist_Bot_Trans"), 2000 * ShearOut.ResultD[0][1].CapaD.BendCapaD.dDeltay, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("Tab_5_SDist_Bot_Trans"), 5000 * ShearOut.ResultD[0][1].CapaD.BendCapaD.dDeltay, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("Tab_8_SDist_Bot_Trans"), 8000 * ShearOut.ResultD[0][1].CapaD.BendCapaD.dDeltay, Row_Ref, Col_Ref, stRow);

			PrintCellData(pExcel, _T("Tab_0_SStress_Bot_Trans"), ShearOut.dVc2[1] + ShearOut.dVp[1] + ShearOut.dVs[1], Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("Tab_2_SStress_Bot_Trans"), ShearOut.dVc2[1] + ShearOut.dVp[1] + ShearOut.dVs[1], Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("Tab_5_SStress_Bot_Trans"), ShearOut.dVc5[1] + ShearOut.dVp[1] + ShearOut.dVs[1], Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("Tab_8_SStress_Bot_Trans"), ShearOut.dVc5[1] + ShearOut.dVp[1] + ShearOut.dVs[1], Row_Ref, Col_Ref, stRow);
		}

		CDgnSeisFailureModeDlg dlg2;
		strShearFileName.Format(_T("%s_%s_%s_%s_Shear.emf"), m_strPicPath, ShearOut.strEvgpName, _T("TRANS"), _T("TOP"));
		dlg2.MakeEmfAndClose(ShearOut.EvgpK, ShearOut.iColm, TRANS, TOP, FALSE, TRUE, strShearFileName);
		dlg2.DoModal();

		PrintCellData(pExcel, _T("Shear_Img_Top_Trans"), PrintMomentShearGraph(pExcel, strShearFileName, (stRow + 1) + 22, 3), Row_Ref, Col_Ref, stRow);

		if (ShearOut.ResultD[1][1].EvalD.bCheck)
		{
			PrintCellData(pExcel, _T("Tab_2_SDist_Top_Trans"), 2000 * ShearOut.ResultD[1][1].CapaD.BendCapaD.dDeltay, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("Tab_5_SDist_Top_Trans"), 5000 * ShearOut.ResultD[1][1].CapaD.BendCapaD.dDeltay, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("Tab_8_SDist_Top_Trans"), 8000 * ShearOut.ResultD[1][1].CapaD.BendCapaD.dDeltay, Row_Ref, Col_Ref, stRow);

			PrintCellData(pExcel, _T("Tab_0_SStress_Top_Trans"), ShearOut.dVc2[3] + ShearOut.dVp[3] + ShearOut.dVs[3], Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("Tab_2_SStress_Top_Trans"), ShearOut.dVc2[3] + ShearOut.dVp[3] + ShearOut.dVs[3], Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("Tab_5_SStress_Top_Trans"), ShearOut.dVc5[3] + ShearOut.dVp[3] + ShearOut.dVs[3], Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("Tab_8_SStress_Top_Trans"), ShearOut.dVc5[3] + ShearOut.dVp[3] + ShearOut.dVs[3], Row_Ref, Col_Ref, stRow);
		}

		stRow += rowCnt;
	}

	return TRUE;
}

BOOL CBrdgSeisEvalKISTEC2015_Report::GenerateFailureBegin(IExcel* pExcel, CString& RptBlock, int& stRow, const EN_BEHAVIOR_TYPE* enBehavior, const EN_FAILURE_MODE* enFailure, T_BRDGEVAL_RESULT ResultD[2][2])
{
	RptBlock = _T("[2]Failure_Mode_Head");
	int rowCnt = CopyRangeSubTitle(pExcel, RptBlock, stRow);
	stRow += rowCnt;
	RptBlock = _T("[2]Failure_Mode_Type_Head");
	rowCnt = CopyRangeSubTitle(pExcel, RptBlock, stRow);
	stRow += rowCnt;

	auto lambda_GetFailStr = [this](const EN_FAILURE_MODE& enFailure)
	{
		CString strFailure;
		switch (enFailure)
		{
		case TYPE1_BEND:
			strFailure = _LS(IDS_DGN_SEIS_FAILURE_MODE_BEND);
			break;
		case TYPE2_BEND_SHR:
			strFailure = _LS(IDS_DGN_SEIS_FAILURE_MODE_BEND_SHR);
			break;
		case TYPE3_SHR:
			strFailure = _LS(IDS_DGN_SEIS_FAILURE_MODE_SHR);
			break;
		case TYPE4_PLAIN:
			strFailure = _LS(IDS_DGN_SEIS_PLAIN_CONCRETE);
			break;
		default:
			strFailure = _T("-");
			break;
		}

		return strFailure;
	};

	CString strFailureBotLongi = lambda_GetFailStr(enFailure[0]);
	CString strFailureTopLongi = lambda_GetFailStr(enFailure[2]);
	CString strFailureBotTrans = lambda_GetFailStr(enFailure[1]);
	CString strFailureTopTrans = lambda_GetFailStr(enFailure[3]);

	short Row_Ref = 0, Col_Ref = 0;
	if (enBehavior[0] == CANTILEVER)
	{
		RptBlock = _T("[2]Failure_Mode_Type_Cantil_Longi");
		rowCnt = CopyRangeSubTitle(pExcel, RptBlock, stRow);
		GetCellIDByName(pExcel, RptBlock, Row_Ref, Col_Ref);

		PrintCellData(pExcel, _T("FType_Longi"), strFailureBotLongi, Row_Ref, Col_Ref, stRow);
		stRow += rowCnt;

	}
	else
	{
		RptBlock = _T("[2]Failure_Mode_Type_Rahmen_Longi");
		rowCnt = CopyRangeSubTitle(pExcel, RptBlock, stRow);
		GetCellIDByName(pExcel, RptBlock, Row_Ref, Col_Ref);

		PrintCellData(pExcel, _T("FType_Bot_Longi"), strFailureBotLongi, Row_Ref, Col_Ref, stRow);
		PrintCellData(pExcel, _T("FType_Top_Longi"), strFailureTopLongi, Row_Ref, Col_Ref, stRow);
		stRow += rowCnt;
	}

	if (enBehavior[1] == CANTILEVER)
	{
		RptBlock = _T("[2]Failure_Mode_Type_Cantil_Trans");
		rowCnt = CopyRangeSubTitle(pExcel, RptBlock, stRow);
		GetCellIDByName(pExcel, RptBlock, Row_Ref, Col_Ref);

		PrintCellData(pExcel, _T("FType_Trans"), strFailureBotTrans, Row_Ref, Col_Ref, stRow);
		stRow += rowCnt;
	}
	else
	{
		RptBlock = _T("[2]Failure_Mode_Type_Rahmen_Trans");
		rowCnt = CopyRangeSubTitle(pExcel, RptBlock, stRow);
		GetCellIDByName(pExcel, RptBlock, Row_Ref, Col_Ref);

		PrintCellData(pExcel, _T("FType_Bot_Trans"), strFailureBotTrans, Row_Ref, Col_Ref, stRow);
		PrintCellData(pExcel, _T("FType_Top_Trans"), strFailureTopTrans, Row_Ref, Col_Ref, stRow);
		stRow += rowCnt;
	}

	return TRUE;

}



BOOL CBrdgSeisEvalKISTEC2015_Report::GenerateFailureMode(IExcel* pExcel, CString& RptBlock, int& stRow, const EN_BEHAVIOR_TYPE* enBehavior, const EN_FAILURE_MODE* enFailure, T_BRDGEVAL_RESULT ResultD[2][2], const T_MPCC_D& MpccD)
{
	short Row_Ref = 0, Col_Ref = 0;

	CString strMomentShearFileName;

	RptBlock = _T("[2]Seismic_Eval_Method_Head");
	int rowCnt = CopyRangeSubTitle(pExcel, RptBlock, stRow);
	stRow += rowCnt;


	RptBlock = _T("[2]Seismic_Eval_Method_Longi_Head");
	rowCnt = CopyRangeSubTitle(pExcel, RptBlock, stRow);
	stRow += rowCnt;

	T_EVGP_K EvgpK = MpccD.EvgpK;
	int      nEvgpColm = MpccD.nEvgpColm;

	if (enBehavior[0] == CANTILEVER)
	{
		CDgnSeisFailureModeDlg dlg;
		strMomentShearFileName.Format(_T("%s_%s_%s_%s_Moment_Shear.emf"), m_strPicPath, MpccD.strName, _T("LONGI"), _T("BOTTOM"));
		dlg.MakeEmfAndClose(EvgpK, nEvgpColm, LONGI, BOTTOM, TRUE, TRUE, strMomentShearFileName);
		dlg.DoModal();

		RptBlock = _T("[2]Graph_Seismic_Eval_Method_Cantil_Longi");
		rowCnt = CopyRangeSubTitle(pExcel, RptBlock, stRow);
		GetCellIDByName(pExcel, RptBlock, Row_Ref, Col_Ref);
		PrintCellData(pExcel, _T("Moment_Shear_Img_Longi"), PrintMomentShearFailureGraph(pExcel, strMomentShearFileName, stRow), Row_Ref, Col_Ref, stRow);
		stRow += rowCnt;

		if (enFailure[0] == TYPE1_BEND || enFailure[0] == TYPE0_NONE)
		{
			RptBlock = _T("[2]Seismic_Eval_Method_Cantil_Longi_T1");
			rowCnt = CopyRangeSubTitle(pExcel, RptBlock, stRow);
			GetCellIDByName(pExcel, RptBlock, Row_Ref, Col_Ref);

			if (ResultD[0][0].EvalD.bCheck)
			{
				PrintCellData(pExcel, _T("S7_Dis_U_Longi_T1"), ResultD[0][0].CapaD.DispDuctD.dDeltau, Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S7_Dis_Y_Longi_T1"), ResultD[0][0].CapaD.DispDuctD.dDeltay, Row_Ref, Col_Ref, stRow);
				if (ResultD[0][0].CapaD.BendCapaD.dDeltac > 0.0)
					PrintCellData(pExcel, _T("S7_Dis_Max_Longi_T1"), ResultD[0][0].CapaD.DispDuctD.dmu_DeltaMax, Row_Ref, Col_Ref, stRow);
				else
					PrintCellData(pExcel, _T("S7_Dis_Max_Longi_T1"), _T("-"), Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S7_FPC_Longi_T1"), ResultD[0][0].EvalD.ModeD.dFpc, Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S7_FN_Longi_T1"), ResultD[0][0].EvalD.ModeD.dFn, Row_Ref, Col_Ref, stRow);
			}

			stRow += rowCnt;

		}
		else if (enFailure[0] == TYPE2_BEND_SHR)
		{
			RptBlock = _T("[2]Seismic_Eval_Method_Cantil_Longi_T2");
			rowCnt = CopyRangeSubTitle(pExcel, RptBlock, stRow);
			GetCellIDByName(pExcel, RptBlock, Row_Ref, Col_Ref);

			if (ResultD[0][0].EvalD.bCheck)
			{
				PrintCellData(pExcel, _T("S7_Dis_C_Longi_T2"), ResultD[0][0].EvalD.ModeD.dDeltac, Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S7_Dis_Y_Longi_T2"), ResultD[0][0].CapaD.DispDuctD.dDeltay, Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S7_FPC_Longi_T2"), ResultD[0][0].EvalD.ModeD.dFpc, Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S7_FN_Longi_T2"), ResultD[0][0].EvalD.ModeD.dFn, Row_Ref, Col_Ref, stRow);
			}

			stRow += rowCnt;
		}
		else if (enFailure[0] == TYPE3_SHR)
		{
			RptBlock = _T("[2]Seismic_Eval_Method_Cantil_Longi_T3");
			rowCnt = CopyRangeSubTitle(pExcel, RptBlock, stRow);
			GetCellIDByName(pExcel, RptBlock, Row_Ref, Col_Ref);

			if (ResultD[0][0].EvalD.bCheck)
				PrintCellData(pExcel, _T("S7_FN_Longi_T3"), ResultD[0][0].EvalD.ModeD.dFn, Row_Ref, Col_Ref, stRow);

			stRow += rowCnt;
		}
		else
		{
			ASSERT(0);
		}
	}
	else
	{
		CDgnSeisFailureModeDlg dlg;
		strMomentShearFileName.Format(_T("%s_%s_%s_%s_Moment_Shear.emf"), m_strPicPath, MpccD.strName, _T("LONGI"), _T("BOTTOM"));
		dlg.MakeEmfAndClose(EvgpK, nEvgpColm, LONGI, BOTTOM, TRUE, TRUE, strMomentShearFileName);
		dlg.DoModal();

		RptBlock = _T("[2]Graph_Seismic_Eval_Method_Bot_Rahmen_Longi");
		rowCnt = CopyRangeSubTitle(pExcel, RptBlock, stRow);
		GetCellIDByName(pExcel, RptBlock, Row_Ref, Col_Ref);
		PrintCellData(pExcel, _T("Moment_Shear_Img_Bot_Longi"), PrintMomentShearFailureGraph(pExcel, strMomentShearFileName, stRow + 1), Row_Ref, Col_Ref, stRow);
		stRow += rowCnt;

		if (enFailure[0] == TYPE1_BEND || enFailure[0] == TYPE0_NONE)
		{
			RptBlock = _T("[2]Seismic_Eval_Method_Bot_Rahmen_Longi_T1");
			rowCnt = CopyRangeSubTitle(pExcel, RptBlock, stRow);
			GetCellIDByName(pExcel, RptBlock, Row_Ref, Col_Ref);

			if (ResultD[0][0].EvalD.bCheck)
			{
				PrintCellData(pExcel, _T("S7_Dis_U_Bot_Longi_T1"), ResultD[0][0].CapaD.DispDuctD.dDeltau, Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S7_Dis_Y_Bot_Longi_T1"), ResultD[0][0].CapaD.DispDuctD.dDeltay, Row_Ref, Col_Ref, stRow);
				if (ResultD[0][0].CapaD.BendCapaD.dDeltac > 0.0)
					PrintCellData(pExcel, _T("S7_Dis_Max_Bot_Longi_T1"), ResultD[0][0].CapaD.DispDuctD.dmu_DeltaMax, Row_Ref, Col_Ref, stRow);
				else
					PrintCellData(pExcel, _T("S7_Dis_Max_Bot_Longi_T1"), _T("-"), Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S7_FPC_Bot_Longi_T1"), ResultD[0][0].EvalD.ModeD.dFpc, Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S7_FN_Bot_Longi_T1"), ResultD[0][0].EvalD.ModeD.dFn, Row_Ref, Col_Ref, stRow);
			}

			stRow += rowCnt;

		}
		else if (enFailure[0] == TYPE2_BEND_SHR)
		{
			RptBlock = _T("[2]Seismic_Eval_Method_Bot_Rahmen_Longi_T2");
			rowCnt = CopyRangeSubTitle(pExcel, RptBlock, stRow);
			GetCellIDByName(pExcel, RptBlock, Row_Ref, Col_Ref);

			if (ResultD[0][0].EvalD.bCheck)
			{
				PrintCellData(pExcel, _T("S7_Dis_C_Bot_Longi_T2"), ResultD[0][0].EvalD.ModeD.dDeltac, Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S7_Dis_Y_Bot_Longi_T2"), ResultD[0][0].CapaD.DispDuctD.dDeltay, Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S7_FPC_Bot_Longi_T2"), ResultD[0][0].EvalD.ModeD.dFpc, Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S7_FN_Bot_Longi_T2"), ResultD[0][0].EvalD.ModeD.dFn, Row_Ref, Col_Ref, stRow);
			}

			stRow += rowCnt;
		}
		else if (enFailure[0] == TYPE3_SHR)
		{
			RptBlock = _T("[2]Seismic_Eval_Method_Bot_Rahmen_Longi_T3");
			rowCnt = CopyRangeSubTitle(pExcel, RptBlock, stRow);
			GetCellIDByName(pExcel, RptBlock, Row_Ref, Col_Ref);

			if (ResultD[0][0].EvalD.bCheck)
				PrintCellData(pExcel, _T("S7_FN_Bot_Longi_T3"), ResultD[0][0].EvalD.ModeD.dFn, Row_Ref, Col_Ref, stRow);

			stRow += rowCnt;
		}
		else
		{
			ASSERT(0);
		}

		CDgnSeisFailureModeDlg dlg2;
		strMomentShearFileName.Format(_T("%s_%s_%s_%s_Moment_Shear.emf"), m_strPicPath, MpccD.strName, _T("LONGI"), _T("TOP"));
		dlg2.MakeEmfAndClose(EvgpK, nEvgpColm, LONGI, TOP, TRUE, TRUE, strMomentShearFileName);
		dlg2.DoModal();

		RptBlock = _T("[2]Graph_Seismic_Eval_Method_Top_Rahmen_Longi");
		rowCnt = CopyRangeSubTitle(pExcel, RptBlock, stRow);
		GetCellIDByName(pExcel, RptBlock, Row_Ref, Col_Ref);
		PrintCellData(pExcel, _T("Moment_Shear_Img_Top_Longi"), PrintMomentShearFailureGraph(pExcel, strMomentShearFileName, stRow + 1), Row_Ref, Col_Ref, stRow);
		stRow += rowCnt;

		if (enFailure[2] == TYPE1_BEND || enFailure[2] == TYPE0_NONE)
		{
			RptBlock = _T("[2]Seismic_Eval_Method_Top_Rahmen_Longi_T1");
			rowCnt = CopyRangeSubTitle(pExcel, RptBlock, stRow);
			GetCellIDByName(pExcel, RptBlock, Row_Ref, Col_Ref);

			if (ResultD[1][0].EvalD.bCheck)
			{
				PrintCellData(pExcel, _T("S7_Dis_U_Top_Longi_T1"), ResultD[1][0].CapaD.DispDuctD.dDeltau, Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S7_Dis_Y_Top_Longi_T1"), ResultD[1][0].CapaD.DispDuctD.dDeltay, Row_Ref, Col_Ref, stRow);
				if (ResultD[1][0].CapaD.BendCapaD.dDeltac > 0.0)
					PrintCellData(pExcel, _T("S7_Dis_Max_Top_Longi_T1"), ResultD[1][0].CapaD.DispDuctD.dmu_DeltaMax, Row_Ref, Col_Ref, stRow);
				else
					PrintCellData(pExcel, _T("S7_Dis_Max_Top_Longi_T1"), _T("-"), Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S7_FPC_Top_Longi_T1"), ResultD[1][0].EvalD.ModeD.dFpc, Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S7_FN_Top_Longi_T1"), ResultD[1][0].EvalD.ModeD.dFn, Row_Ref, Col_Ref, stRow);
			}

			stRow += rowCnt;
		}
		else if (enFailure[2] == TYPE2_BEND_SHR)
		{
			RptBlock = _T("[2]Seismic_Eval_Method_Top_Rahmen_Longi_T2");
			rowCnt = CopyRangeSubTitle(pExcel, RptBlock, stRow);
			GetCellIDByName(pExcel, RptBlock, Row_Ref, Col_Ref);

			if (ResultD[1][0].EvalD.bCheck)
			{
				PrintCellData(pExcel, _T("S7_Dis_C_Top_Longi_T2"), ResultD[1][0].EvalD.ModeD.dDeltac, Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S7_Dis_Y_Top_Longi_T2"), ResultD[1][0].CapaD.DispDuctD.dDeltay, Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S7_FPC_Top_Longi_T2"), ResultD[1][0].EvalD.ModeD.dFpc, Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S7_FN_Top_Longi_T2"), ResultD[1][0].EvalD.ModeD.dFn, Row_Ref, Col_Ref, stRow);
			}

			stRow += rowCnt;

		}
		else if (enFailure[2] == TYPE3_SHR)
		{
			RptBlock = _T("[2]Seismic_Eval_Method_Top_Rahmen_Longi_T3");
			rowCnt = CopyRangeSubTitle(pExcel, RptBlock, stRow);
			GetCellIDByName(pExcel, RptBlock, Row_Ref, Col_Ref);

			if (ResultD[1][0].EvalD.bCheck)
				PrintCellData(pExcel, _T("S7_FN_Top_Longi_T3"), ResultD[1][0].EvalD.ModeD.dFn, Row_Ref, Col_Ref, stRow);

			stRow += rowCnt;
		}
		else
		{
			ASSERT(0);
		}
	}

	RptBlock = _T("[2]Seismic_Eval_Method_Trans_Head");
	rowCnt = CopyRangeSubTitle(pExcel, RptBlock, stRow);
	stRow += rowCnt;

	if (enBehavior[1] == CANTILEVER)
	{
		CDgnSeisFailureModeDlg dlg;
		strMomentShearFileName.Format(_T("%s_%s_%s_%s_Moment_Shear.emf"), m_strPicPath, MpccD.strName, _T("TRANS"), _T("BOTTOM"));
		dlg.MakeEmfAndClose(EvgpK, nEvgpColm, TRANS, BOTTOM, TRUE, TRUE, strMomentShearFileName);
		dlg.DoModal();

		RptBlock = _T("[2]Graph_Seismic_Eval_Method_Cantil_Trans");
		rowCnt = CopyRangeSubTitle(pExcel, RptBlock, stRow);
		GetCellIDByName(pExcel, RptBlock, Row_Ref, Col_Ref);
		PrintCellData(pExcel, _T("Moment_Shear_Img_Trans"), PrintMomentShearFailureGraph(pExcel, strMomentShearFileName, stRow), Row_Ref, Col_Ref, stRow);
		stRow += rowCnt;

		if (enFailure[1] == TYPE1_BEND || enFailure[1] == TYPE0_NONE)
		{
			RptBlock = _T("[2]Seismic_Eval_Method_Cantil_Trans_T1");
			rowCnt = CopyRangeSubTitle(pExcel, RptBlock, stRow);
			GetCellIDByName(pExcel, RptBlock, Row_Ref, Col_Ref);

			if (ResultD[0][1].EvalD.bCheck)
			{
				PrintCellData(pExcel, _T("S7_Dis_U_Trans_T1"), ResultD[0][1].CapaD.DispDuctD.dDeltau, Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S7_Dis_Y_Trans_T1"), ResultD[0][1].CapaD.DispDuctD.dDeltay, Row_Ref, Col_Ref, stRow);
				if (ResultD[0][1].CapaD.BendCapaD.dDeltac > 0.0)
					PrintCellData(pExcel, _T("S7_Dis_Max_Trans_T1"), ResultD[0][1].CapaD.DispDuctD.dmu_DeltaMax, Row_Ref, Col_Ref, stRow);
				else
					PrintCellData(pExcel, _T("S7_Dis_Max_Trans_T1"), _T("-"), Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S7_FPC_Trans_T1"), ResultD[0][1].EvalD.ModeD.dFpc, Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S7_FN_Trans_T1"), ResultD[0][1].EvalD.ModeD.dFn, Row_Ref, Col_Ref, stRow);
			}

			stRow += rowCnt;

		}
		else if (enFailure[1] == TYPE2_BEND_SHR)
		{
			RptBlock = _T("[2]Seismic_Eval_Method_Cantil_Trans_T2");
			rowCnt = CopyRangeSubTitle(pExcel, RptBlock, stRow);
			GetCellIDByName(pExcel, RptBlock, Row_Ref, Col_Ref);

			if (ResultD[0][1].EvalD.bCheck)
			{
				PrintCellData(pExcel, _T("S7_Dis_C_Trans_T2"), ResultD[0][1].EvalD.ModeD.dDeltac, Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S7_Dis_Y_Trans_T2"), ResultD[0][1].CapaD.DispDuctD.dDeltay, Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S7_FPC_Trans_T2"), ResultD[0][1].EvalD.ModeD.dFpc, Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S7_FN_Trans_T2"), ResultD[0][1].EvalD.ModeD.dFn, Row_Ref, Col_Ref, stRow);
			}

			stRow += rowCnt;

		}
		else if (enFailure[1] == TYPE3_SHR)
		{
			RptBlock = _T("[2]Seismic_Eval_Method_Cantil_Trans_T3");
			rowCnt = CopyRangeSubTitle(pExcel, RptBlock, stRow);
			GetCellIDByName(pExcel, RptBlock, Row_Ref, Col_Ref);

			if (ResultD[0][1].EvalD.bCheck)
				PrintCellData(pExcel, _T("S7_FN_Trans_T3"), ResultD[0][1].EvalD.ModeD.dFn, Row_Ref, Col_Ref, stRow);

			stRow += rowCnt;

		}
		else
		{
			ASSERT(0);
		}
	}
	else
	{
		CDgnSeisFailureModeDlg dlg;
		strMomentShearFileName.Format(_T("%s_%s_%s_%s_Moment_Shear.emf"), m_strPicPath, MpccD.strName, _T("TRANS"), _T("BOTTOM"));
		dlg.MakeEmfAndClose(EvgpK, nEvgpColm, TRANS, BOTTOM, TRUE, TRUE, strMomentShearFileName);
		dlg.DoModal();

		RptBlock = _T("[2]Graph_Seismic_Eval_Method_Bot_Rahmen_Trans");
		rowCnt = CopyRangeSubTitle(pExcel, RptBlock, stRow);
		GetCellIDByName(pExcel, RptBlock, Row_Ref, Col_Ref);
		PrintCellData(pExcel, _T("Moment_Shear_Img_Bot_Trans"), PrintMomentShearFailureGraph(pExcel, strMomentShearFileName, stRow + 1), Row_Ref, Col_Ref, stRow);

		stRow += rowCnt;

		if (enFailure[1] == TYPE1_BEND || enFailure[1] == TYPE0_NONE)
		{
			RptBlock = _T("[2]Seismic_Eval_Method_Bot_Rahmen_Trans_T1");
			rowCnt = CopyRangeSubTitle(pExcel, RptBlock, stRow);
			GetCellIDByName(pExcel, RptBlock, Row_Ref, Col_Ref);

			if (ResultD[0][1].EvalD.bCheck)
			{
				PrintCellData(pExcel, _T("S7_Dis_U_Bot_Trans_T1"), ResultD[0][1].CapaD.DispDuctD.dDeltau, Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S7_Dis_Y_Bot_Trans_T1"), ResultD[0][1].CapaD.DispDuctD.dDeltay, Row_Ref, Col_Ref, stRow);
				if (ResultD[0][1].CapaD.BendCapaD.dDeltac > 0.0)
					PrintCellData(pExcel, _T("S7_Dis_Max_Bot_Trans_T1"), ResultD[0][1].CapaD.DispDuctD.dmu_DeltaMax, Row_Ref, Col_Ref, stRow);
				else
					PrintCellData(pExcel, _T("S7_Dis_Max_Bot_Trans_T1"), _T("-"), Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S7_FPC_Bot_Trans_T1"), ResultD[0][1].EvalD.ModeD.dFpc, Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S7_FN_Bot_Trans_T1"), ResultD[0][1].EvalD.ModeD.dFn, Row_Ref, Col_Ref, stRow);
			}

			stRow += rowCnt;
		}
		else if (enFailure[1] == TYPE2_BEND_SHR)
		{
			RptBlock = _T("[2]Seismic_Eval_Method_Bot_Rahmen_Trans_T2");
			rowCnt = CopyRangeSubTitle(pExcel, RptBlock, stRow);
			GetCellIDByName(pExcel, RptBlock, Row_Ref, Col_Ref);

			if (ResultD[0][1].EvalD.bCheck)
			{
				PrintCellData(pExcel, _T("S7_Dis_C_Bot_Trans_T2"), ResultD[0][1].EvalD.ModeD.dDeltac, Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S7_Dis_Y_Bot_Trans_T2"), ResultD[0][1].CapaD.DispDuctD.dDeltay, Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S7_FPC_Bot_Trans_T2"), ResultD[0][1].EvalD.ModeD.dFpc, Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S7_FN_Bot_Trans_T2"), ResultD[0][1].EvalD.ModeD.dFn, Row_Ref, Col_Ref, stRow);
			}

			stRow += rowCnt;

		}
		else if (enFailure[1] == TYPE3_SHR)
		{
			RptBlock = _T("[2]Seismic_Eval_Method_Bot_Rahmen_Trans_T3");
			rowCnt = CopyRangeSubTitle(pExcel, RptBlock, stRow);
			GetCellIDByName(pExcel, RptBlock, Row_Ref, Col_Ref);

			if (ResultD[0][1].EvalD.bCheck)
				PrintCellData(pExcel, _T("S7_FN_Bot_Trans_T3"), ResultD[0][1].EvalD.ModeD.dFn, Row_Ref, Col_Ref, stRow);

			stRow += rowCnt;
		}
		else
		{
			ASSERT(0);
		}

		CDgnSeisFailureModeDlg dlg2;
		strMomentShearFileName.Format(_T("%s_%s_%s_%s_Moment_Shear.emf"), m_strPicPath, MpccD.strName, _T("TRANS"), _T("TOP"));
		dlg2.MakeEmfAndClose(EvgpK, nEvgpColm, TRANS, TOP, TRUE, TRUE, strMomentShearFileName);
		dlg2.DoModal();

		RptBlock = _T("[2]Graph_Seismic_Eval_Method_Top_Rahmen_Trans");
		rowCnt = CopyRangeSubTitle(pExcel, RptBlock, stRow);
		GetCellIDByName(pExcel, RptBlock, Row_Ref, Col_Ref);
		PrintCellData(pExcel, _T("Moment_Shear_Img_Top_Trans"), PrintMomentShearFailureGraph(pExcel, strMomentShearFileName, stRow + 1), Row_Ref, Col_Ref, stRow);
		stRow += rowCnt;

		if (enFailure[3] == TYPE1_BEND || enFailure[3] == TYPE0_NONE)
		{
			RptBlock = _T("[2]Seismic_Eval_Method_Top_Rahmen_Trans_T1");
			rowCnt = CopyRangeSubTitle(pExcel, RptBlock, stRow);
			GetCellIDByName(pExcel, RptBlock, Row_Ref, Col_Ref);

			if (ResultD[1][1].EvalD.bCheck)
			{
				PrintCellData(pExcel, _T("S7_Dis_U_Top_Trans_T1"), ResultD[1][1].CapaD.DispDuctD.dDeltau, Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S7_Dis_Y_Top_Trans_T1"), ResultD[1][1].CapaD.DispDuctD.dDeltay, Row_Ref, Col_Ref, stRow);
				if (ResultD[1][1].CapaD.BendCapaD.dDeltac > 0.0)
					PrintCellData(pExcel, _T("S7_Dis_Max_Top_Trans_T1"), ResultD[1][1].CapaD.DispDuctD.dmu_DeltaMax, Row_Ref, Col_Ref, stRow);
				else
					PrintCellData(pExcel, _T("S7_Dis_Max_Top_Trans_T1"), _T("-"), Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S7_FPC_Top_Trans_T1"), ResultD[1][1].EvalD.ModeD.dFpc, Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S7_FN_Top_Trans_T1"), ResultD[1][1].EvalD.ModeD.dFn, Row_Ref, Col_Ref, stRow);
			}

			stRow += rowCnt;
		}
		else if (enFailure[3] == TYPE2_BEND_SHR)
		{
			RptBlock = _T("[2]Seismic_Eval_Method_Top_Rahmen_Trans_T2");
			rowCnt = CopyRangeSubTitle(pExcel, RptBlock, stRow);
			GetCellIDByName(pExcel, RptBlock, Row_Ref, Col_Ref);

			if (ResultD[1][1].EvalD.bCheck)
			{
				PrintCellData(pExcel, _T("S7_Dis_C_Top_Trans_T2"), ResultD[1][1].EvalD.ModeD.dDeltac, Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S7_Dis_Y_Top_Trans_T2"), ResultD[1][1].CapaD.DispDuctD.dDeltay, Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S7_FPC_Top_Trans_T2"), ResultD[1][1].EvalD.ModeD.dFpc, Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S7_FN_Top_Trans_T2"), ResultD[1][1].EvalD.ModeD.dFn, Row_Ref, Col_Ref, stRow);
			}

			stRow += rowCnt;

		}
		else if (enFailure[3] == TYPE3_SHR)
		{
			RptBlock = _T("[2]Seismic_Eval_Method_Top_Rahmen_Trans_T3");
			rowCnt = CopyRangeSubTitle(pExcel, RptBlock, stRow);
			GetCellIDByName(pExcel, RptBlock, Row_Ref, Col_Ref);

			if (ResultD[1][1].EvalD.bCheck)
				PrintCellData(pExcel, _T("S7_FN_Top_Trans_T3"), ResultD[1][1].EvalD.ModeD.dFn, Row_Ref, Col_Ref, stRow);

			stRow += rowCnt;
		}
		else
		{
			ASSERT(0);
		}

	}

	return TRUE;

}

BOOL CBrdgSeisEvalKISTEC2015_Report::PrintMomentShearGraph(IExcel* pExcel, CString& strMomentShearFileName, const int& stRow, const int& stCol)
{

	int nH = 15;
	int nW = 14;
	//
	CString strPath = strMomentShearFileName;

	if (WaitForFile(strPath))
	{
		pExcel->AddPicture_Count(stRow, stCol, nH, nW, strPath);
	}

	return TRUE;
}

BOOL CBrdgSeisEvalKISTEC2015_Report::PrintMomentShearFailureGraph(IExcel* pExcel, CString& strMomentShearFileName, const int& stRow)
{

	int nH = 15;
	int nW = 14;
	//
	CString strPath = strMomentShearFileName;

	if (WaitForFile(strPath))
	{
		pExcel->AddPicture_Count(stRow, 4, nH, nW, strPath);
	}

	return TRUE;
}



BOOL CBrdgSeisEvalKISTEC2015_Report::GenerateEvalResult(IExcel* pExcel, CString& RptBlock, int& stRow, const EN_BEHAVIOR_TYPE* enBehavior, const EN_FAILURE_MODE* enFailure, T_BRDGEVAL_RESULT ResultD[2][2])
{
	RptBlock = _T("[2]Seismic_Eval_Pier_Head");
	int rowCnt = CopyRangeSubTitle(pExcel, RptBlock, stRow);
	stRow += rowCnt;

	short Row_Ref = 0, Col_Ref = 0;

	if (enBehavior[0] == CANTILEVER)
	{
		if (enFailure[0] == TYPE1_BEND || enFailure[0] == TYPE0_NONE)
		{
			RptBlock = _T("[2]Seismic_Eval_Cantil_Longi_T1T2");
			rowCnt = CopyRangeSubTitle(pExcel, RptBlock, stRow);
			GetCellIDByName(pExcel, RptBlock, Row_Ref, Col_Ref);

			if (ResultD[0][0].EvalD.bCheck)
			{
				PrintCellData(pExcel, _T("S8_FTYPE_Longi_T1T2"), _LS(IDS_DGN_SEIS_FAILURE_MODE_BEND), Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S8_FPC_Longi1_T1T2"), ResultD[0][0].EvalD.ModeD.dFpc, Row_Ref, Col_Ref, stRow);

				PrintCellData(pExcel, _T("S8_M_Longi_T1T2"), ResultD[0][0].EvalD.EvalD.dMe, Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S8_P_Longi_T1T2"), ResultD[0][0].EvalD.EvalD.dP, Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S8_D_Longi_T1T2"), ResultD[0][0].EvalD.EvalD.dDelta, Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S8_ME_Longi_T1T2"), ResultD[0][0].EvalD.EvalD.dMe_pd, Row_Ref, Col_Ref, stRow);

				PrintCellData(pExcel, _T("S8_ME_Longi2_T1T2"), ResultD[0][0].EvalD.EvalD.dMe_pd, Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S8_My_Longi_T1T2"), ResultD[0][0].MPhiD.StateD.dMoment[4], Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S8_M_COMP_Longi_T1T2"), Check_Sign(0, ResultD[0][0].MPhiD.StateD.dMoment[4], ResultD[0][0].EvalD.EvalD.dMe_pd), Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S8_RS_Longi_T1T2"), ResultD[0][0].EvalD.EvalD.dRs, Row_Ref, Col_Ref, stRow);

				PrintCellData(pExcel, _T("S8_TL_Longi_T1T2"), ResultD[0][0].EvalD.EvalD.dT, Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S8_TS_Longi_T1T2"), ResultD[0][0].EvalD.EvalD.dTs, Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S8_125TS_Longi_T1T2"), 1.25 *	ResultD[0][0].EvalD.EvalD.dTs, Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S8_T_COMP_Longi_T1T2"), Check_Sign(0, ResultD[0][0].EvalD.EvalD.dT, 1.25 *	ResultD[0][0].EvalD.EvalD.dTs), Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S8_LamDR_Longi_T1T2"), ResultD[0][0].EvalD.EvalD.dLadbdaDR, Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S8_FPD_Longi1_T1T2"), ResultD[0][0].EvalD.EvalD.dFpd, Row_Ref, Col_Ref, stRow);

				PrintCellData(pExcel, _T("S8_FPC_Longi2_T1T2"), ResultD[0][0].EvalD.ModeD.dFpc, Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S8_FPD_Longi2_T1T2"), ResultD[0][0].EvalD.EvalD.dFpd, Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S8_FR_Longi_T1T2"), ResultD[0][0].EvalD.dRat, Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S8_FR_COMP_Longi_T1T2"), Check_Sign(0, ResultD[0][0].EvalD.dRat, 1.0), Row_Ref, Col_Ref, stRow);
				PrintCellDataMark(pExcel, _T("S8_FR_Result_Longi_T1T2"), Chk_OK_Sign(0, 1.0, ResultD[0][0].EvalD.dRat), Row_Ref, Col_Ref, stRow);
			}
			else
			{
				CString sCell1 = pExcel->GetCell(stRow + 4, 0);
				CString sCell2 = pExcel->GetCell(stRow + 18, 0);
				pExcel->DeletCell(sCell1, sCell2);

				rowCnt -= (18 - 4);
			}

			stRow += rowCnt;
		}
		else if (enFailure[0] == TYPE2_BEND_SHR)
		{
			RptBlock = _T("[2]Seismic_Eval_Cantil_Longi_T1T2");
			rowCnt = CopyRangeSubTitle(pExcel, RptBlock, stRow);
			GetCellIDByName(pExcel, RptBlock, Row_Ref, Col_Ref);

			if (ResultD[0][0].EvalD.bCheck)
			{
				PrintCellData(pExcel, _T("S8_FTYPE_Longi_T1T2"), _LS(IDS_DGN_SEIS_FAILURE_MODE_BEND_SHR), Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S8_FPC_Longi1_T1T2"), ResultD[0][0].EvalD.ModeD.dFpc, Row_Ref, Col_Ref, stRow);

				PrintCellData(pExcel, _T("S8_M_Longi_T1T2"), ResultD[0][0].EvalD.EvalD.dMe, Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S8_P_Longi_T1T2"), ResultD[0][0].EvalD.EvalD.dP, Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S8_D_Longi_T1T2"), ResultD[0][0].EvalD.EvalD.dDelta, Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S8_ME_Longi_T1T2"), ResultD[0][0].EvalD.EvalD.dMe_pd, Row_Ref, Col_Ref, stRow);

				PrintCellData(pExcel, _T("S8_ME_Longi2_T1T2"), ResultD[0][0].EvalD.EvalD.dMe_pd, Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S8_My_Longi_T1T2"), ResultD[0][0].MPhiD.StateD.dMoment[4], Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S8_M_COMP_Longi_T1T2"), Check_Sign(0, ResultD[0][0].MPhiD.StateD.dMoment[4], ResultD[0][0].EvalD.EvalD.dMe_pd), Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S8_RS_Longi_T1T2"), ResultD[0][0].EvalD.EvalD.dRs, Row_Ref, Col_Ref, stRow);

				PrintCellData(pExcel, _T("S8_TL_Longi_T1T2"), ResultD[0][0].EvalD.EvalD.dT, Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S8_TS_Longi_T1T2"), ResultD[0][0].EvalD.EvalD.dTs, Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S8_125TS_Longi_T1T2"), 1.25 *	ResultD[0][0].EvalD.EvalD.dTs, Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S8_T_COMP_Longi_T1T2"), Check_Sign(0, ResultD[0][0].EvalD.EvalD.dT, 1.25 *	ResultD[0][0].EvalD.EvalD.dTs), Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S8_LamDR_Longi_T1T2"), ResultD[0][0].EvalD.EvalD.dLadbdaDR, Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S8_FPD_Longi1_T1T2"), ResultD[0][0].EvalD.EvalD.dFpd, Row_Ref, Col_Ref, stRow);

				PrintCellData(pExcel, _T("S8_FPC_Longi2_T1T2"), ResultD[0][0].EvalD.ModeD.dFpc, Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S8_FPD_Longi2_T1T2"), ResultD[0][0].EvalD.EvalD.dFpd, Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S8_FR_Longi_T1T2"), ResultD[0][0].EvalD.dRat, Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S8_FR_COMP_Longi_T1T2"), Check_Sign(0, ResultD[0][0].EvalD.dRat, 1.0), Row_Ref, Col_Ref, stRow);
				PrintCellDataMark(pExcel, _T("S8_FR_Result_Longi_T1T2"), Chk_OK_Sign(0, 1.0, ResultD[0][0].EvalD.dRat), Row_Ref, Col_Ref, stRow);
			}
			stRow += rowCnt;

		}
		else if (enFailure[0] == TYPE3_SHR)
		{
			RptBlock = _T("[2]Seismic_Eval_Cantil_Longi_T3");
			rowCnt = CopyRangeSubTitle(pExcel, RptBlock, stRow);
			GetCellIDByName(pExcel, RptBlock, Row_Ref, Col_Ref);

			if (ResultD[0][0].EvalD.bCheck)
			{
				PrintCellData(pExcel, _T("S8_FPC_Longi1_T3"), ResultD[0][0].EvalD.ModeD.dFpc, Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S8_FPD_Longi1_T3"), ResultD[0][0].EvalD.EvalD.dFpd, Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S8_FPC_Longi2_T3"), ResultD[0][0].EvalD.ModeD.dFpc, Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S8_FPD_Longi2_T3"), ResultD[0][0].EvalD.EvalD.dFpd, Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S8_FR_Longi_T3"), ResultD[0][0].EvalD.dRat, Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S8_FR_COMP_Longi_T3"), Check_Sign(0, ResultD[0][0].EvalD.dRat, 1.0), Row_Ref, Col_Ref, stRow);
				PrintCellDataMark(pExcel, _T("S8_FR_Result_Longi_T3"), Chk_OK_Sign(0, 1.0, ResultD[0][0].EvalD.dRat), Row_Ref, Col_Ref, stRow);
			}

			stRow += rowCnt;
		}
		else
		{
			ASSERT(0);
		}
	}
	else
	{
		if (enFailure[0] == TYPE1_BEND || enFailure[0] == TYPE0_NONE)
		{
			RptBlock = _T("[2]Seismic_Eval_Rahmen_Longi_T1T2");
			rowCnt = CopyRangeSubTitle(pExcel, RptBlock, stRow);
			stRow += rowCnt;
			RptBlock = _T("[2]Seismic_Eval_Bot_Rahmen_Longi_T1T2");
			rowCnt = CopyRangeSubTitle(pExcel, RptBlock, stRow);
			GetCellIDByName(pExcel, RptBlock, Row_Ref, Col_Ref);

			if (ResultD[0][0].EvalD.bCheck)
			{
				PrintCellData(pExcel, _T("S8_FTYPE_Bot_Longi_T1T2"), _LS(IDS_DGN_SEIS_FAILURE_MODE_BEND), Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S8_FPC_Bot_Longi1_T1T2"), ResultD[0][0].EvalD.ModeD.dFpc, Row_Ref, Col_Ref, stRow);

				PrintCellData(pExcel, _T("S8_M_Bot_Longi_T1T2"), ResultD[0][0].EvalD.EvalD.dMe, Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S8_P_Bot_Longi_T1T2"), ResultD[0][0].EvalD.EvalD.dP, Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S8_D_Bot_Longi_T1T2"), ResultD[0][0].EvalD.EvalD.dDelta, Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S8_ME_Bot_Longi_T1T2"), ResultD[0][0].EvalD.EvalD.dMe_pd, Row_Ref, Col_Ref, stRow);

				PrintCellData(pExcel, _T("S8_ME_Bot_Longi2_T1T2"), ResultD[0][0].EvalD.EvalD.dMe_pd, Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S8_My_Bot_Longi_T1T2"), ResultD[0][0].MPhiD.StateD.dMoment[4], Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S8_M_COMP_Bot_Longi_T1T2"), Check_Sign(0, ResultD[0][0].MPhiD.StateD.dMoment[4], ResultD[0][0].EvalD.EvalD.dMe_pd), Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S8_RS_Bot_Longi_T1T2"), ResultD[0][0].EvalD.EvalD.dRs, Row_Ref, Col_Ref, stRow);

				PrintCellData(pExcel, _T("S8_TL_Bot_Longi_T1T2"), ResultD[0][0].EvalD.EvalD.dT, Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S8_TS_Bot_Longi_T1T2"), ResultD[0][0].EvalD.EvalD.dTs, Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S8_125TS_Bot_Longi_T1T2"), 1.25 *	ResultD[0][0].EvalD.EvalD.dTs, Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S8_T_COMP_Bot_Longi_T1T2"), Check_Sign(0, ResultD[0][0].EvalD.EvalD.dT, 1.25 *	ResultD[0][0].EvalD.EvalD.dTs), Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S8_LamDR_Bot_Longi_T1T2"), ResultD[0][0].EvalD.EvalD.dLadbdaDR, Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S8_FPD_Bot_Longi1_T1T2"), ResultD[0][0].EvalD.EvalD.dFpd, Row_Ref, Col_Ref, stRow);

				PrintCellData(pExcel, _T("S8_FPC_Bot_Longi2_T1T2"), ResultD[0][0].EvalD.ModeD.dFpc, Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S8_FPD_Bot_Longi2_T1T2"), ResultD[0][0].EvalD.EvalD.dFpd, Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S8_FR_Bot_Longi_T1T2"), ResultD[0][0].EvalD.dRat, Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S8_FR_COMP_Bot_Longi_T1T2"), Check_Sign(0, ResultD[0][0].EvalD.dRat, 1.0), Row_Ref, Col_Ref, stRow);
				PrintCellDataMark(pExcel, _T("S8_FR_Result_Bot_Longi_T1T2"), Chk_OK_Sign(0, 1.0, ResultD[0][0].EvalD.dRat), Row_Ref, Col_Ref, stRow);
			}
			else
			{
				CString sCell1 = pExcel->GetCell(stRow + 4, 0);
				CString sCell2 = pExcel->GetCell(stRow + 20, 0);
				pExcel->DeletCell(sCell1, sCell2);

				rowCnt -= (20 - 4);
			}

			stRow += rowCnt;

		}
		else if (enFailure[0] == TYPE2_BEND_SHR)
		{
			RptBlock = _T("[2]Seismic_Eval_Rahmen_Longi_T1T2");
			rowCnt = CopyRangeSubTitle(pExcel, RptBlock, stRow);
			stRow += rowCnt;
			RptBlock = _T("[2]Seismic_Eval_Bot_Rahmen_Longi_T1T2");
			rowCnt = CopyRangeSubTitle(pExcel, RptBlock, stRow);
			GetCellIDByName(pExcel, RptBlock, Row_Ref, Col_Ref);

			if (ResultD[0][0].EvalD.bCheck)
			{
				PrintCellData(pExcel, _T("S8_FTYPE_Bot_Longi_T1T2"), _LS(IDS_DGN_SEIS_FAILURE_MODE_BEND_SHR), Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S8_FPC_Bot_Longi1_T1T2"), ResultD[0][0].EvalD.ModeD.dFpc, Row_Ref, Col_Ref, stRow);

				PrintCellData(pExcel, _T("S8_M_Bot_Longi_T1T2"), ResultD[0][0].EvalD.EvalD.dMe, Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S8_P_Bot_Longi_T1T2"), ResultD[0][0].EvalD.EvalD.dP, Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S8_D_Bot_Longi_T1T2"), ResultD[0][0].EvalD.EvalD.dDelta, Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S8_ME_Bot_Longi_T1T2"), ResultD[0][0].EvalD.EvalD.dMe_pd, Row_Ref, Col_Ref, stRow);

				PrintCellData(pExcel, _T("S8_ME_Bot_Longi2_T1T2"), ResultD[0][0].EvalD.EvalD.dMe_pd, Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S8_My_Bot_Longi_T1T2"), ResultD[0][0].MPhiD.StateD.dMoment[4], Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S8_M_COMP_Bot_Longi_T1T2"), Check_Sign(0, ResultD[0][0].MPhiD.StateD.dMoment[4], ResultD[0][0].EvalD.EvalD.dMe_pd), Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S8_RS_Bot_Longi_T1T2"), ResultD[0][0].EvalD.EvalD.dRs, Row_Ref, Col_Ref, stRow);

				PrintCellData(pExcel, _T("S8_TL_Bot_Longi_T1T2"), ResultD[0][0].EvalD.EvalD.dT, Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S8_TS_Bot_Longi_T1T2"), ResultD[0][0].EvalD.EvalD.dTs, Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S8_125TS_Bot_Longi_T1T2"), 1.25 *	ResultD[0][0].EvalD.EvalD.dTs, Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S8_T_COMP_Bot_Longi_T1T2"), Check_Sign(0, ResultD[0][0].EvalD.EvalD.dT, 1.25 *	ResultD[0][0].EvalD.EvalD.dTs), Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S8_LamDR_Bot_Longi_T1T2"), ResultD[0][0].EvalD.EvalD.dLadbdaDR, Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S8_FPD_Bot_Longi1_T1T2"), ResultD[0][0].EvalD.EvalD.dFpd, Row_Ref, Col_Ref, stRow);

				PrintCellData(pExcel, _T("S8_FPC_Bot_Longi2_T1T2"), ResultD[0][0].EvalD.ModeD.dFpc, Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S8_FPD_Bot_Longi2_T1T2"), ResultD[0][0].EvalD.EvalD.dFpd, Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S8_FR_Bot_Longi_T1T2"), ResultD[0][0].EvalD.dRat, Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S8_FR_COMP_Bot_Longi_T1T2"), Check_Sign(0, ResultD[0][0].EvalD.dRat, 1.0), Row_Ref, Col_Ref, stRow);
				PrintCellDataMark(pExcel, _T("S8_FR_Result_Bot_Longi_T1T2"), Chk_OK_Sign(0, 1.0, ResultD[0][0].EvalD.dRat), Row_Ref, Col_Ref, stRow);
			}

			stRow += rowCnt;
		}
		else if (enFailure[0] == TYPE3_SHR)
		{
			RptBlock = _T("[2]Seismic_Eval_Rahmen_Longi_T3");
			rowCnt = CopyRangeSubTitle(pExcel, RptBlock, stRow);
			stRow += rowCnt;
			RptBlock = _T("[2]Seismic_Eval_Bot_Rahmen_Longi_T3");
			rowCnt = CopyRangeSubTitle(pExcel, RptBlock, stRow);
			GetCellIDByName(pExcel, RptBlock, Row_Ref, Col_Ref);

			if (ResultD[0][0].EvalD.bCheck)
			{
				PrintCellData(pExcel, _T("S8_FPC_Bot_Longi1_T3"), ResultD[0][0].EvalD.ModeD.dFpc, Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S8_FPD_Bot_Longi1_T3"), ResultD[0][0].EvalD.EvalD.dFpd, Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S8_FPC_Bot_Longi2_T3"), ResultD[0][0].EvalD.ModeD.dFpc, Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S8_FPD_Bot_Longi2_T3"), ResultD[0][0].EvalD.EvalD.dFpd, Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S8_FR_Bot_Longi_T3"), ResultD[0][0].EvalD.dRat, Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S8_FR_COMP_Bot_Longi_T3"), Check_Sign(0, ResultD[0][0].EvalD.dRat, 1.0), Row_Ref, Col_Ref, stRow);
				PrintCellDataMark(pExcel, _T("S8_FR_Result_Bot_Longi_T3"), Chk_OK_Sign(0, 1.0, ResultD[0][0].EvalD.dRat), Row_Ref, Col_Ref, stRow);
			}

			stRow += rowCnt;
		}
		else
		{
			ASSERT(0);
		}

		if (enFailure[2] == TYPE1_BEND || enFailure[2] == TYPE0_NONE)
		{
			RptBlock = _T("[2]Seismic_Eval_Top_Rahmen_Longi_T1T2");
			rowCnt = CopyRangeSubTitle(pExcel, RptBlock, stRow);
			GetCellIDByName(pExcel, RptBlock, Row_Ref, Col_Ref);

			if (ResultD[1][0].EvalD.bCheck)
			{
				PrintCellData(pExcel, _T("S8_FTYPE_Top_Longi_T1T2"), _LS(IDS_DGN_SEIS_FAILURE_MODE_BEND), Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S8_FPC_Top_Longi1_T1T2"), ResultD[1][0].EvalD.ModeD.dFpc, Row_Ref, Col_Ref, stRow);

				PrintCellData(pExcel, _T("S8_M_Top_Longi_T1T2"), ResultD[1][0].EvalD.EvalD.dMe, Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S8_P_Top_Longi_T1T2"), ResultD[1][0].EvalD.EvalD.dP, Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S8_D_Top_Longi_T1T2"), ResultD[1][0].EvalD.EvalD.dDelta, Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S8_ME_Top_Longi_T1T2"), ResultD[1][0].EvalD.EvalD.dMe_pd, Row_Ref, Col_Ref, stRow);

				PrintCellData(pExcel, _T("S8_ME_Top_Longi2_T1T2"), ResultD[1][0].EvalD.EvalD.dMe_pd, Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S8_My_Top_Longi_T1T2"), ResultD[1][0].MPhiD.StateD.dMoment[4], Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S8_M_COMP_Top_Longi_T1T2"), Check_Sign(0, ResultD[1][0].MPhiD.StateD.dMoment[4], ResultD[1][0].EvalD.EvalD.dMe_pd), Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S8_RS_Top_Longi_T1T2"), ResultD[1][0].EvalD.EvalD.dRs, Row_Ref, Col_Ref, stRow);

				PrintCellData(pExcel, _T("S8_TL_Top_Longi_T1T2"), ResultD[1][0].EvalD.EvalD.dT, Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S8_TS_Top_Longi_T1T2"), ResultD[1][0].EvalD.EvalD.dTs, Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S8_125TS_Top_Longi_T1T2"), 1.25 *	ResultD[1][0].EvalD.EvalD.dTs, Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S8_T_COMP_Top_Longi_T1T2"), Check_Sign(0, ResultD[1][0].EvalD.EvalD.dT, 1.25 *	ResultD[1][0].EvalD.EvalD.dTs), Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S8_LamDR_Top_Longi_T1T2"), ResultD[1][0].EvalD.EvalD.dLadbdaDR, Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S8_FPD_Top_Longi1_T1T2"), ResultD[1][0].EvalD.EvalD.dFpd, Row_Ref, Col_Ref, stRow);

				PrintCellData(pExcel, _T("S8_FPC_Top_Longi2_T1T2"), ResultD[1][0].EvalD.ModeD.dFpc, Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S8_FPD_Top_Longi2_T1T2"), ResultD[1][0].EvalD.EvalD.dFpd, Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S8_FR_Top_Longi_T1T2"), ResultD[1][0].EvalD.dRat, Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S8_FR_COMP_Top_Longi_T1T2"), Check_Sign(0, ResultD[1][0].EvalD.dRat, 1.0), Row_Ref, Col_Ref, stRow);
				PrintCellDataMark(pExcel, _T("S8_FR_Result_Top_Longi_T1T2"), Chk_OK_Sign(0, 1.0, ResultD[1][0].EvalD.dRat), Row_Ref, Col_Ref, stRow);
			}
			else
			{
				CString sCell1 = pExcel->GetCell(stRow + 4, 0);
				CString sCell2 = pExcel->GetCell(stRow + 20, 0);
				pExcel->DeletCell(sCell1, sCell2);

				rowCnt -= (20 - 4);
			}

			stRow += rowCnt;
		}
		else if (enFailure[2] == TYPE2_BEND_SHR)
		{
			RptBlock = _T("[2]Seismic_Eval_Top_Rahmen_Longi_T1T2");
			rowCnt = CopyRangeSubTitle(pExcel, RptBlock, stRow);
			GetCellIDByName(pExcel, RptBlock, Row_Ref, Col_Ref);

			if (ResultD[1][0].EvalD.bCheck)
			{
				PrintCellData(pExcel, _T("S8_FTYPE_Top_Longi_T1T2"), _LS(IDS_DGN_SEIS_FAILURE_MODE_BEND_SHR), Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S8_FPC_Top_Longi1_T1T2"), ResultD[1][0].EvalD.ModeD.dFpc, Row_Ref, Col_Ref, stRow);

				PrintCellData(pExcel, _T("S8_M_Top_Longi_T1T2"), ResultD[1][0].EvalD.EvalD.dMe, Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S8_P_Top_Longi_T1T2"), ResultD[1][0].EvalD.EvalD.dP, Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S8_D_Top_Longi_T1T2"), ResultD[1][0].EvalD.EvalD.dDelta, Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S8_ME_Top_Longi_T1T2"), ResultD[1][0].EvalD.EvalD.dMe_pd, Row_Ref, Col_Ref, stRow);

				PrintCellData(pExcel, _T("S8_ME_Top_Longi2_T1T2"), ResultD[1][0].EvalD.EvalD.dMe_pd, Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S8_My_Top_Longi_T1T2"), ResultD[1][0].MPhiD.StateD.dMoment[4], Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S8_M_COMP_Top_Longi_T1T2"), Check_Sign(0, ResultD[1][0].MPhiD.StateD.dMoment[4], ResultD[1][0].EvalD.EvalD.dMe_pd), Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S8_RS_Top_Longi_T1T2"), ResultD[1][0].EvalD.EvalD.dRs, Row_Ref, Col_Ref, stRow);

				PrintCellData(pExcel, _T("S8_TL_Top_Longi_T1T2"), ResultD[1][0].EvalD.EvalD.dT, Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S8_TS_Top_Longi_T1T2"), ResultD[1][0].EvalD.EvalD.dTs, Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S8_125TS_Top_Longi_T1T2"), 1.25 *	ResultD[1][0].EvalD.EvalD.dTs, Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S8_T_COMP_Top_Longi_T1T2"), Check_Sign(0, ResultD[1][0].EvalD.EvalD.dT, 1.25 *	ResultD[1][0].EvalD.EvalD.dTs), Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S8_LamDR_Top_Longi_T1T2"), ResultD[1][0].EvalD.EvalD.dLadbdaDR, Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S8_FPD_Top_Longi1_T1T2"), ResultD[1][0].EvalD.EvalD.dFpd, Row_Ref, Col_Ref, stRow);

				PrintCellData(pExcel, _T("S8_FPC_Top_Longi2_T1T2"), ResultD[1][0].EvalD.ModeD.dFpc, Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S8_FPD_Top_Longi2_T1T2"), ResultD[1][0].EvalD.EvalD.dFpd, Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S8_FR_Top_Longi_T1T2"), ResultD[1][0].EvalD.dRat, Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S8_FR_COMP_Top_Longi_T1T2"), Check_Sign(0, ResultD[1][0].EvalD.dRat, 1.0), Row_Ref, Col_Ref, stRow);
				PrintCellDataMark(pExcel, _T("S8_FR_Result_Top_Longi_T1T2"), Chk_OK_Sign(0, 1.0, ResultD[1][0].EvalD.dRat), Row_Ref, Col_Ref, stRow);
			}

			stRow += rowCnt;
		}
		else if (enFailure[2] == TYPE3_SHR)
		{
			RptBlock = _T("[2]Seismic_Eval_Top_Rahmen_Longi_T3");
			rowCnt = CopyRangeSubTitle(pExcel, RptBlock, stRow);
			GetCellIDByName(pExcel, RptBlock, Row_Ref, Col_Ref);

			if (ResultD[1][0].EvalD.bCheck)
			{
				PrintCellData(pExcel, _T("S8_FPC_Top_Longi1_T3"), ResultD[1][0].EvalD.ModeD.dFpc, Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S8_FPD_Top_Longi1_T3"), ResultD[1][0].EvalD.EvalD.dFpd, Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S8_FPC_Top_Longi2_T3"), ResultD[1][0].EvalD.ModeD.dFpc, Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S8_FPD_Top_Longi2_T3"), ResultD[1][0].EvalD.EvalD.dFpd, Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S8_FR_Top_Longi_T3"), ResultD[1][0].EvalD.dRat, Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S8_FR_COMP_Top_Longi_T3"), Check_Sign(0, ResultD[1][0].EvalD.dRat, 1.0), Row_Ref, Col_Ref, stRow);
				PrintCellDataMark(pExcel, _T("S8_FR_Result_Top_Longi_T3"), Chk_OK_Sign(0, 1.0, ResultD[1][0].EvalD.dRat), Row_Ref, Col_Ref, stRow);
			}

			stRow += rowCnt;
		}
		else
		{
			ASSERT(0);
		}

	}

	if (enBehavior[1] == CANTILEVER)
	{
		if (enFailure[1] == TYPE1_BEND || enFailure[1] == TYPE0_NONE)
		{
			RptBlock = _T("[2]Seismic_Eval_Cantil_Trans_T1T2");
			rowCnt = CopyRangeSubTitle(pExcel, RptBlock, stRow);
			GetCellIDByName(pExcel, RptBlock, Row_Ref, Col_Ref);

			if (ResultD[0][1].EvalD.bCheck)
			{
				PrintCellData(pExcel, _T("S8_FTYPE_Trans_T1T2"), _LS(IDS_DGN_SEIS_FAILURE_MODE_BEND), Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S8_FPC_Trans1_T1T2"), ResultD[0][1].EvalD.ModeD.dFpc, Row_Ref, Col_Ref, stRow);

				PrintCellData(pExcel, _T("S8_M_Trans_T1T2"), ResultD[0][1].EvalD.EvalD.dMe, Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S8_P_Trans_T1T2"), ResultD[0][1].EvalD.EvalD.dP, Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S8_D_Trans_T1T2"), ResultD[0][1].EvalD.EvalD.dDelta, Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S8_ME_Trans_T1T2"), ResultD[0][1].EvalD.EvalD.dMe_pd, Row_Ref, Col_Ref, stRow);

				PrintCellData(pExcel, _T("S8_ME_Trans2_T1T2"), ResultD[0][1].EvalD.EvalD.dMe_pd, Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S8_My_Trans_T1T2"), ResultD[0][1].MPhiD.StateD.dMoment[4], Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S8_M_COMP_Trans_T1T2"), Check_Sign(0, ResultD[0][1].MPhiD.StateD.dMoment[4], ResultD[0][1].EvalD.EvalD.dMe_pd), Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S8_RS_Trans_T1T2"), ResultD[0][1].EvalD.EvalD.dRs, Row_Ref, Col_Ref, stRow);

				PrintCellData(pExcel, _T("S8_TL_Trans_T1T2"), ResultD[0][1].EvalD.EvalD.dT, Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S8_TS_Trans_T1T2"), ResultD[0][1].EvalD.EvalD.dTs, Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S8_125TS_Trans_T1T2"), 1.25 *	ResultD[0][1].EvalD.EvalD.dTs, Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S8_T_COMP_Trans_T1T2"), Check_Sign(0, ResultD[0][1].EvalD.EvalD.dT, 1.25 *	ResultD[0][1].EvalD.EvalD.dTs), Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S8_LamDR_Trans_T1T2"), ResultD[0][1].EvalD.EvalD.dLadbdaDR, Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S8_FPD_Trans1_T1T2"), ResultD[0][1].EvalD.EvalD.dFpd, Row_Ref, Col_Ref, stRow);

				PrintCellData(pExcel, _T("S8_FPC_Trans2_T1T2"), ResultD[0][1].EvalD.ModeD.dFpc, Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S8_FPD_Trans2_T1T2"), ResultD[0][1].EvalD.EvalD.dFpd, Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S8_FR_Trans_T1T2"), ResultD[0][1].EvalD.dRat, Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S8_FR_COMP_Trans_T1T2"), Check_Sign(0, ResultD[0][1].EvalD.dRat, 1.0), Row_Ref, Col_Ref, stRow);
				PrintCellDataMark(pExcel, _T("S8_FR_Result_Trans_T1T2"), Chk_OK_Sign(0, 1.0, ResultD[0][1].EvalD.dRat), Row_Ref, Col_Ref, stRow);
			}
			else
			{
				CString sCell1 = pExcel->GetCell(stRow + 4, 0);
				CString sCell2 = pExcel->GetCell(stRow + 18, 0);
				pExcel->DeletCell(sCell1, sCell2);

				rowCnt -= (18 - 4);
			}

			stRow += rowCnt;
		}
		else if (enFailure[1] == TYPE2_BEND_SHR)
		{
			RptBlock = _T("[2]Seismic_Eval_Cantil_Trans_T1T2");
			rowCnt = CopyRangeSubTitle(pExcel, RptBlock, stRow);
			GetCellIDByName(pExcel, RptBlock, Row_Ref, Col_Ref);

			if (ResultD[0][1].EvalD.bCheck)
			{
				PrintCellData(pExcel, _T("S8_FTYPE_Trans_T1T2"), _LS(IDS_DGN_SEIS_FAILURE_MODE_BEND_SHR), Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S8_FPC_Trans1_T1T2"), ResultD[0][1].EvalD.ModeD.dFpc, Row_Ref, Col_Ref, stRow);

				PrintCellData(pExcel, _T("S8_M_Trans_T1T2"), ResultD[0][1].EvalD.EvalD.dMe, Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S8_P_Trans_T1T2"), ResultD[0][1].EvalD.EvalD.dP, Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S8_D_Trans_T1T2"), ResultD[0][1].EvalD.EvalD.dDelta, Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S8_ME_Trans_T1T2"), ResultD[0][1].EvalD.EvalD.dMe_pd, Row_Ref, Col_Ref, stRow);

				PrintCellData(pExcel, _T("S8_ME_Trans2_T1T2"), ResultD[0][1].EvalD.EvalD.dMe_pd, Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S8_My_Trans_T1T2"), ResultD[0][1].MPhiD.StateD.dMoment[4], Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S8_M_COMP_Trans_T1T2"), Check_Sign(0, ResultD[0][1].MPhiD.StateD.dMoment[4], ResultD[0][1].EvalD.EvalD.dMe_pd), Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S8_RS_Trans_T1T2"), ResultD[0][1].EvalD.EvalD.dRs, Row_Ref, Col_Ref, stRow);

				PrintCellData(pExcel, _T("S8_TL_Trans_T1T2"), ResultD[0][1].EvalD.EvalD.dT, Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S8_TS_Trans_T1T2"), ResultD[0][1].EvalD.EvalD.dTs, Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S8_125TS_Trans_T1T2"), 1.25 *	ResultD[0][1].EvalD.EvalD.dTs, Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S8_T_COMP_Trans_T1T2"), Check_Sign(0, ResultD[0][1].EvalD.EvalD.dT, 1.25 *	ResultD[0][1].EvalD.EvalD.dTs), Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S8_LamDR_Trans_T1T2"), ResultD[0][1].EvalD.EvalD.dLadbdaDR, Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S8_FPD_Trans1_T1T2"), ResultD[0][1].EvalD.EvalD.dFpd, Row_Ref, Col_Ref, stRow);

				PrintCellData(pExcel, _T("S8_FPC_Trans2_T1T2"), ResultD[0][1].EvalD.ModeD.dFpc, Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S8_FPD_Trans2_T1T2"), ResultD[0][1].EvalD.EvalD.dFpd, Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S8_FR_Trans_T1T2"), ResultD[0][1].EvalD.dRat, Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S8_FR_COMP_Trans_T1T2"), Check_Sign(0, ResultD[0][1].EvalD.dRat, 1.0), Row_Ref, Col_Ref, stRow);
				PrintCellDataMark(pExcel, _T("S8_FR_Result_Trans_T1T2"), Chk_OK_Sign(0, 1.0, ResultD[0][1].EvalD.dRat), Row_Ref, Col_Ref, stRow);
			}

			stRow += rowCnt;

		}
		else if (enFailure[1] == TYPE3_SHR)
		{
			RptBlock = _T("[2]Seismic_Eval_Cantil_Trans_T3");
			rowCnt = CopyRangeSubTitle(pExcel, RptBlock, stRow);
			GetCellIDByName(pExcel, RptBlock, Row_Ref, Col_Ref);

			if (ResultD[0][1].EvalD.bCheck)
			{
				PrintCellData(pExcel, _T("S8_FPC_Trans1_T3"), ResultD[0][1].EvalD.ModeD.dFpc, Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S8_FPD_Trans1_T3"), ResultD[0][1].EvalD.EvalD.dFpd, Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S8_FPC_Trans2_T3"), ResultD[0][1].EvalD.ModeD.dFpc, Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S8_FPD_Trans2_T3"), ResultD[0][1].EvalD.EvalD.dFpd, Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S8_FR_Trans_T3"), ResultD[0][1].EvalD.dRat, Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S8_FR_COMP_Trans_T3"), Check_Sign(0, ResultD[0][1].EvalD.dRat, 1.0), Row_Ref, Col_Ref, stRow);
				PrintCellDataMark(pExcel, _T("S8_FR_Result_Trans_T3"), Chk_OK_Sign(0, 1.0, ResultD[0][1].EvalD.dRat), Row_Ref, Col_Ref, stRow);
			}

			stRow += rowCnt;
		}
		else
		{
			ASSERT(0);
		}
	}
	else
	{

		if (enFailure[1] == TYPE1_BEND || enFailure[1] == TYPE0_NONE)
		{
			RptBlock = _T("[2]Seismic_Eval_Rahmen_Trans_T1T2");
			rowCnt = CopyRangeSubTitle(pExcel, RptBlock, stRow);
			stRow += rowCnt;
			RptBlock = _T("[2]Seismic_Eval_Bot_Rahmen_Trans_T1T2");
			rowCnt = CopyRangeSubTitle(pExcel, RptBlock, stRow);
			GetCellIDByName(pExcel, RptBlock, Row_Ref, Col_Ref);

			if (ResultD[0][1].EvalD.bCheck)
			{
				PrintCellData(pExcel, _T("S8_FTYPE_Bot_Trans_T1T2"), _LS(IDS_DGN_SEIS_FAILURE_MODE_BEND), Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S8_FPC_Bot_Trans1_T1T2"), ResultD[0][1].EvalD.ModeD.dFpc, Row_Ref, Col_Ref, stRow);

				PrintCellData(pExcel, _T("S8_M_Bot_Trans_T1T2"), ResultD[0][1].EvalD.EvalD.dMe, Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S8_P_Bot_Trans_T1T2"), ResultD[0][1].EvalD.EvalD.dP, Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S8_D_Bot_Trans_T1T2"), ResultD[0][1].EvalD.EvalD.dDelta, Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S8_ME_Bot_Trans_T1T2"), ResultD[0][1].EvalD.EvalD.dMe_pd, Row_Ref, Col_Ref, stRow);

				PrintCellData(pExcel, _T("S8_ME_Bot_Trans2_T1T2"), ResultD[0][1].EvalD.EvalD.dMe_pd, Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S8_My_Bot_Trans_T1T2"), ResultD[0][1].MPhiD.StateD.dMoment[4], Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S8_M_COMP_Bot_Trans_T1T2"), Check_Sign(0, ResultD[0][1].MPhiD.StateD.dMoment[4], ResultD[0][1].EvalD.EvalD.dMe_pd), Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S8_RS_Bot_Trans_T1T2"), ResultD[0][1].EvalD.EvalD.dRs, Row_Ref, Col_Ref, stRow);

				PrintCellData(pExcel, _T("S8_TL_Bot_Trans_T1T2"), ResultD[0][1].EvalD.EvalD.dT, Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S8_TS_Bot_Trans_T1T2"), ResultD[0][1].EvalD.EvalD.dTs, Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S8_125TS_Bot_Trans_T1T2"), 1.25 *	ResultD[0][1].EvalD.EvalD.dTs, Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S8_T_COMP_Bot_Trans_T1T2"), Check_Sign(0, ResultD[0][1].EvalD.EvalD.dT, 1.25 *	ResultD[0][1].EvalD.EvalD.dTs), Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S8_LamDR_Bot_Trans_T1T2"), ResultD[0][1].EvalD.EvalD.dLadbdaDR, Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S8_FPD_Bot_Trans1_T1T2"), ResultD[0][1].EvalD.EvalD.dFpd, Row_Ref, Col_Ref, stRow);

				PrintCellData(pExcel, _T("S8_FPC_Bot_Trans2_T1T2"), ResultD[0][1].EvalD.ModeD.dFpc, Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S8_FPD_Bot_Trans2_T1T2"), ResultD[0][1].EvalD.EvalD.dFpd, Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S8_FR_Bot_Trans_T1T2"), ResultD[0][1].EvalD.dRat, Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S8_FR_COMP_Bot_Trans_T1T2"), Check_Sign(0, ResultD[0][1].EvalD.dRat, 1.0), Row_Ref, Col_Ref, stRow);
				PrintCellDataMark(pExcel, _T("S8_FR_Result_Bot_Trans_T1T2"), Chk_OK_Sign(0, 1.0, ResultD[0][1].EvalD.dRat), Row_Ref, Col_Ref, stRow);
			}
			else
			{
				CString sCell1 = pExcel->GetCell(stRow + 4, 0);
				CString sCell2 = pExcel->GetCell(stRow + 20, 0);
				pExcel->DeletCell(sCell1, sCell2);

				rowCnt -= (20 - 4);
			}

			stRow += rowCnt;

		}
		else if (enFailure[1] == TYPE2_BEND_SHR)
		{
			RptBlock = _T("[2]Seismic_Eval_Rahmen_Trans_T1T2");
			rowCnt = CopyRangeSubTitle(pExcel, RptBlock, stRow);
			stRow += rowCnt + 1;
			RptBlock = _T("[2]Seismic_Eval_Bot_Rahmen_Trans_T1T2");
			rowCnt = CopyRangeSubTitle(pExcel, RptBlock, stRow);
			GetCellIDByName(pExcel, RptBlock, Row_Ref, Col_Ref);

			if (ResultD[0][1].EvalD.bCheck)
			{
				PrintCellData(pExcel, _T("S8_FTYPE_Bot_Trans_T1T2"), _LS(IDS_DGN_SEIS_FAILURE_MODE_BEND_SHR), Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S8_FPC_Bot_Trans1_T1T2"), ResultD[0][1].EvalD.ModeD.dFpc, Row_Ref, Col_Ref, stRow);

				PrintCellData(pExcel, _T("S8_M_Bot_Trans_T1T2"), ResultD[0][1].EvalD.EvalD.dMe, Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S8_P_Bot_Trans_T1T2"), ResultD[0][1].EvalD.EvalD.dP, Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S8_D_Bot_Trans_T1T2"), ResultD[0][1].EvalD.EvalD.dDelta, Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S8_ME_Bot_Trans_T1T2"), ResultD[0][1].EvalD.EvalD.dMe_pd, Row_Ref, Col_Ref, stRow);

				PrintCellData(pExcel, _T("S8_ME_Bot_Trans2_T1T2"), ResultD[0][1].EvalD.EvalD.dMe_pd, Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S8_My_Bot_Trans_T1T2"), ResultD[0][1].MPhiD.StateD.dMoment[4], Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S8_M_COMP_Bot_Trans_T1T2"), Check_Sign(0, ResultD[0][1].MPhiD.StateD.dMoment[4], ResultD[0][1].EvalD.EvalD.dMe_pd), Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S8_RS_Bot_Trans_T1T2"), ResultD[0][1].EvalD.EvalD.dRs, Row_Ref, Col_Ref, stRow);

				PrintCellData(pExcel, _T("S8_TL_Bot_Trans_T1T2"), ResultD[0][1].EvalD.EvalD.dT, Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S8_TS_Bot_Trans_T1T2"), ResultD[0][1].EvalD.EvalD.dTs, Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S8_125TS_Bot_Trans_T1T2"), 1.25 *	ResultD[0][1].EvalD.EvalD.dTs, Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S8_T_COMP_Bot_Trans_T1T2"), Check_Sign(0, ResultD[0][1].EvalD.EvalD.dT, 1.25 *	ResultD[0][1].EvalD.EvalD.dTs), Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S8_LamDR_Bot_Trans_T1T2"), ResultD[0][1].EvalD.EvalD.dLadbdaDR, Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S8_FPD_Bot_Trans1_T1T2"), ResultD[0][1].EvalD.EvalD.dFpd, Row_Ref, Col_Ref, stRow);

				PrintCellData(pExcel, _T("S8_FPC_Bot_Trans2_T1T2"), ResultD[0][1].EvalD.ModeD.dFpc, Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S8_FPD_Bot_Trans2_T1T2"), ResultD[0][1].EvalD.EvalD.dFpd, Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S8_FR_Bot_Trans_T1T2"), ResultD[0][1].EvalD.dRat, Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S8_FR_COMP_Bot_Trans_T1T2"), Check_Sign(0, ResultD[0][1].EvalD.dRat, 1.0), Row_Ref, Col_Ref, stRow);
				PrintCellDataMark(pExcel, _T("S8_FR_Result_Bot_Trans_T1T2"), Chk_OK_Sign(0, 1.0, ResultD[0][1].EvalD.dRat), Row_Ref, Col_Ref, stRow);
			}

			stRow += rowCnt;
		}
		else if (enFailure[1] == TYPE3_SHR)
		{
			RptBlock = _T("[2]Seismic_Eval_Rahmen_Trans_T3");
			rowCnt = CopyRangeSubTitle(pExcel, RptBlock, stRow);
			stRow += rowCnt + 1;
			RptBlock = _T("[2]Seismic_Eval_Bot_Rahmen_Trans_T3");
			rowCnt = CopyRangeSubTitle(pExcel, RptBlock, stRow);
			GetCellIDByName(pExcel, RptBlock, Row_Ref, Col_Ref);

			if (ResultD[0][1].EvalD.bCheck)
			{
				PrintCellData(pExcel, _T("S8_FPC_Bot_Trans1_T3"), ResultD[0][1].EvalD.ModeD.dFpc, Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S8_FPD_Bot_Trans1_T3"), ResultD[0][1].EvalD.EvalD.dFpd, Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S8_FPC_Bot_Trans2_T3"), ResultD[0][1].EvalD.ModeD.dFpc, Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S8_FPD_Bot_Trans2_T3"), ResultD[0][1].EvalD.EvalD.dFpd, Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S8_FR_Bot_Trans_T3"), ResultD[0][1].EvalD.dRat, Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S8_FR_COMP_Bot_Trans_T3"), Check_Sign(0, ResultD[0][1].EvalD.dRat, 1.0), Row_Ref, Col_Ref, stRow);
				PrintCellDataMark(pExcel, _T("S8_FR_Result_Bot_Trans_T3"), Chk_OK_Sign(0, 1.0, ResultD[0][1].EvalD.dRat), Row_Ref, Col_Ref, stRow);
			}

			stRow += rowCnt;
		}
		else
		{
			ASSERT(0);
		}

		if (enFailure[3] == TYPE1_BEND || enFailure[3] == TYPE0_NONE)
		{
			RptBlock = _T("[2]Seismic_Eval_Top_Rahmen_Trans_T1T2");
			rowCnt = CopyRangeSubTitle(pExcel, RptBlock, stRow);
			GetCellIDByName(pExcel, RptBlock, Row_Ref, Col_Ref);

			if (ResultD[1][1].EvalD.bCheck)
			{
				PrintCellData(pExcel, _T("S8_FTYPE_Top_Trans_T1T2"), _LS(IDS_DGN_SEIS_FAILURE_MODE_BEND), Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S8_FPC_Top_Trans1_T1T2"), ResultD[1][1].EvalD.ModeD.dFpc, Row_Ref, Col_Ref, stRow);

				PrintCellData(pExcel, _T("S8_M_Top_Trans_T1T2"), ResultD[1][1].EvalD.EvalD.dMe, Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S8_P_Top_Trans_T1T2"), ResultD[1][1].EvalD.EvalD.dP, Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S8_D_Top_Trans_T1T2"), ResultD[1][1].EvalD.EvalD.dDelta, Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S8_ME_Top_Trans_T1T2"), ResultD[1][1].EvalD.EvalD.dMe_pd, Row_Ref, Col_Ref, stRow);

				PrintCellData(pExcel, _T("S8_ME_Top_Trans2_T1T2"), ResultD[1][1].EvalD.EvalD.dMe_pd, Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S8_My_Top_Trans_T1T2"), ResultD[1][1].MPhiD.StateD.dMoment[4], Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S8_M_COMP_Top_Trans_T1T2"), Check_Sign(0, ResultD[1][1].MPhiD.StateD.dMoment[4], ResultD[1][1].EvalD.EvalD.dMe_pd), Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S8_RS_Top_Trans_T1T2"), ResultD[1][1].EvalD.EvalD.dRs, Row_Ref, Col_Ref, stRow);

				PrintCellData(pExcel, _T("S8_TL_Top_Trans_T1T2"), ResultD[1][1].EvalD.EvalD.dT, Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S8_TS_Top_Trans_T1T2"), ResultD[1][1].EvalD.EvalD.dTs, Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S8_125TS_Top_Trans_T1T2"), 1.25 *	ResultD[1][1].EvalD.EvalD.dTs, Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S8_T_COMP_Top_Trans_T1T2"), Check_Sign(0, ResultD[1][1].EvalD.EvalD.dT, 1.25 *	ResultD[1][1].EvalD.EvalD.dTs), Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S8_LamDR_Top_Trans_T1T2"), ResultD[1][1].EvalD.EvalD.dLadbdaDR, Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S8_FPD_Top_Trans1_T1T2"), ResultD[1][1].EvalD.EvalD.dFpd, Row_Ref, Col_Ref, stRow);

				PrintCellData(pExcel, _T("S8_FPC_Top_Trans2_T1T2"), ResultD[1][1].EvalD.ModeD.dFpc, Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S8_FPD_Top_Trans2_T1T2"), ResultD[1][1].EvalD.EvalD.dFpd, Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S8_FR_Top_Trans_T1T2"), ResultD[1][1].EvalD.dRat, Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S8_FR_COMP_Top_Trans_T1T2"), Check_Sign(0, ResultD[1][1].EvalD.dRat, 1.0), Row_Ref, Col_Ref, stRow);
				PrintCellDataMark(pExcel, _T("S8_FR_Result_Top_Trans_T1T2"), Chk_OK_Sign(0, 1.0, ResultD[1][1].EvalD.dRat), Row_Ref, Col_Ref, stRow);
			}
			else
			{
				CString sCell1 = pExcel->GetCell(stRow + 4, 0);
				CString sCell2 = pExcel->GetCell(stRow + 20, 0);
				pExcel->DeletCell(sCell1, sCell2);

				rowCnt -= (20 - 4);
			}

			stRow += rowCnt;
		}
		else if (enFailure[3] == TYPE2_BEND_SHR)
		{
			RptBlock = _T("[2]Seismic_Eval_Top_Rahmen_Trans_T1T2");
			rowCnt = CopyRangeSubTitle(pExcel, RptBlock, stRow);
			GetCellIDByName(pExcel, RptBlock, Row_Ref, Col_Ref);

			if (ResultD[1][1].EvalD.bCheck)
			{
				PrintCellData(pExcel, _T("S8_FTYPE_Top_Trans_T1T2"), _LS(IDS_DGN_SEIS_FAILURE_MODE_BEND_SHR), Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S8_FPC_Top_Trans1_T1T2"), ResultD[1][1].EvalD.ModeD.dFpc, Row_Ref, Col_Ref, stRow);

				PrintCellData(pExcel, _T("S8_M_Top_Trans_T1T2"), ResultD[1][1].EvalD.EvalD.dMe, Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S8_P_Top_Trans_T1T2"), ResultD[1][1].EvalD.EvalD.dP, Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S8_D_Top_Trans_T1T2"), ResultD[1][1].EvalD.EvalD.dDelta, Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S8_ME_Top_Trans_T1T2"), ResultD[1][1].EvalD.EvalD.dMe_pd, Row_Ref, Col_Ref, stRow);

				PrintCellData(pExcel, _T("S8_ME_Top_Trans2_T1T2"), ResultD[1][1].EvalD.EvalD.dMe_pd, Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S8_My_Top_Trans_T1T2"), ResultD[1][1].MPhiD.StateD.dMoment[4], Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S8_M_COMP_Top_Trans_T1T2"), Check_Sign(0, ResultD[1][1].MPhiD.StateD.dMoment[4], ResultD[1][1].EvalD.EvalD.dMe_pd), Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S8_RS_Top_Trans_T1T2"), ResultD[1][1].EvalD.EvalD.dRs, Row_Ref, Col_Ref, stRow);

				PrintCellData(pExcel, _T("S8_TL_Top_Trans_T1T2"), ResultD[1][1].EvalD.EvalD.dT, Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S8_TS_Top_Trans_T1T2"), ResultD[1][1].EvalD.EvalD.dTs, Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S8_125TS_Top_Trans_T1T2"), 1.25 *	ResultD[1][1].EvalD.EvalD.dTs, Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S8_T_COMP_Top_Trans_T1T2"), Check_Sign(0, ResultD[1][1].EvalD.EvalD.dT, 1.25 *	ResultD[1][1].EvalD.EvalD.dTs), Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S8_LamDR_Top_Trans_T1T2"), ResultD[1][1].EvalD.EvalD.dLadbdaDR, Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S8_FPD_Top_Trans1_T1T2"), ResultD[1][1].EvalD.EvalD.dFpd, Row_Ref, Col_Ref, stRow);

				PrintCellData(pExcel, _T("S8_FPC_Top_Trans2_T1T2"), ResultD[1][1].EvalD.ModeD.dFpc, Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S8_FPD_Top_Trans2_T1T2"), ResultD[1][1].EvalD.EvalD.dFpd, Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S8_FR_Top_Trans_T1T2"), ResultD[1][1].EvalD.dRat, Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S8_FR_COMP_Top_Trans_T1T2"), Check_Sign(0, ResultD[1][1].EvalD.dRat, 1.0), Row_Ref, Col_Ref, stRow);
				PrintCellDataMark(pExcel, _T("S8_FR_Result_Top_Trans_T1T2"), Chk_OK_Sign(0, 1.0, ResultD[1][1].EvalD.dRat), Row_Ref, Col_Ref, stRow);
			}

			stRow += rowCnt;
		}
		else if (enFailure[3] == TYPE3_SHR)
		{
			RptBlock = _T("[2]Seismic_Eval_Top_Rahmen_Trans_T3");
			rowCnt = CopyRangeSubTitle(pExcel, RptBlock, stRow);
			GetCellIDByName(pExcel, RptBlock, Row_Ref, Col_Ref);

			if (ResultD[1][1].EvalD.bCheck)
			{
				PrintCellData(pExcel, _T("S8_FPC_Top_Trans1_T3"), ResultD[1][1].EvalD.ModeD.dFpc, Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S8_FPD_Top_Trans1_T3"), ResultD[1][1].EvalD.EvalD.dFpd, Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S8_FPC_Top_Trans2_T3"), ResultD[1][1].EvalD.ModeD.dFpc, Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S8_FPD_Top_Trans2_T3"), ResultD[1][1].EvalD.EvalD.dFpd, Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S8_FR_Top_Trans_T3"), ResultD[1][1].EvalD.dRat, Row_Ref, Col_Ref, stRow);
				PrintCellData(pExcel, _T("S8_FR_COMP_Top_Trans_T3"), Check_Sign(0, ResultD[1][1].EvalD.dRat, 1.0), Row_Ref, Col_Ref, stRow);
				PrintCellDataMark(pExcel, _T("S8_FR_Result_Top_Trans_T3"), Chk_OK_Sign(0, 1.0, ResultD[1][1].EvalD.dRat), Row_Ref, Col_Ref, stRow);
			}

			stRow += rowCnt;
		}
		else
		{
			ASSERT(0);
		}
	}


	return TRUE;
}

BOOL CBrdgSeisEvalKISTEC2015_Report::GenerateEvalResultTable(IExcel* pExcel, CString& RptBlock, int& stRow, const EN_BEHAVIOR_TYPE* enBehavior, const EN_FAILURE_MODE* enFailure, T_BRDGEVAL_RESULT ResultD[2][2])
{
	RptBlock = _T("[2]Seismic_Eval_Result_Head");
	int rowCnt = CopyRangeSubTitle(pExcel, RptBlock, stRow);
	stRow += rowCnt;

	CString strFailure[4];
	for (int i = 0; i < 4; ++i)
	{
		switch (enFailure[i])
		{
		case TYPE0_NONE:
			strFailure[i] = _T("-");
			break;
		case TYPE1_BEND:
			strFailure[i] = _LS(IDS_DGN_SEIS_FAILURE_MODE_BEND);
			break;
		case TYPE2_BEND_SHR:
			strFailure[i] = _LS(IDS_DGN_SEIS_FAILURE_MODE_BEND_SHR);
			break;
		case TYPE3_SHR:
			strFailure[i] = _LS(IDS_DGN_SEIS_FAILURE_MODE_SHR);
			break;
		case TYPE4_PLAIN:
			strFailure[i] = _LS(IDS_DGN_SEIS_PLAIN_CONCRETE);
			break;
		default:
			ASSERT(0);
			break;
		}
	}

	RptBlock = _T("[2]Seismic_Eval_Result_Longi");
	rowCnt = CopyRangeSubTitle(pExcel, RptBlock, stRow);
	stRow += rowCnt;

	short Row_Ref = 0, Col_Ref = 0;
	if (enBehavior[0] == CANTILEVER)
	{
		RptBlock = _T("[2]Seismic_Eval_Result_Cantil_Longi");
		rowCnt = CopyRangeSubTitle(pExcel, RptBlock, stRow);
		GetCellIDByName(pExcel, RptBlock, Row_Ref, Col_Ref);

		if (ResultD[0][0].EvalD.bCheck)
		{
			PrintCellData(pExcel, _T("Tab_FType_Longi"), strFailure[0], Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("Tab_FPC_Longi"), ResultD[0][0].EvalD.ModeD.dFpc, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("Tab_FPD_Longi"), ResultD[0][0].EvalD.EvalD.dFpd, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("Tab_FR_Longi"), ResultD[0][0].EvalD.dRat, Row_Ref, Col_Ref, stRow);
			PrintCellDataMark(pExcel, _T("Tab_FR_Result_Longi"), Chk_OK_Sign(0, 1.0, ResultD[0][0].EvalD.dRat), Row_Ref, Col_Ref, stRow);
		}

		stRow += rowCnt;
	}
	else
	{
		RptBlock = _T("[2]Seismic_Eval_Result_Rahmen_Longi");
		rowCnt = CopyRangeSubTitle(pExcel, RptBlock, stRow);
		GetCellIDByName(pExcel, RptBlock, Row_Ref, Col_Ref);

		if (ResultD[0][0].EvalD.bCheck)
		{
			PrintCellData(pExcel, _T("Tab_FType_Bot_Longi"), strFailure[0], Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("Tab_FPC_Bot_Longi"), ResultD[0][0].EvalD.ModeD.dFpc, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("Tab_FPD_Bot_Longi"), ResultD[0][0].EvalD.EvalD.dFpd, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("Tab_FR_Bot_Longi"), ResultD[0][0].EvalD.dRat, Row_Ref, Col_Ref, stRow);
			PrintCellDataMark(pExcel, _T("Tab_FR_Result_Bot_Longi"), Chk_OK_Sign(0, 1.0, ResultD[0][0].EvalD.dRat), Row_Ref, Col_Ref, stRow);
		}

		if (ResultD[1][0].EvalD.bCheck)
		{
			PrintCellData(pExcel, _T("Tab_FType_Top_Longi"), strFailure[2], Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("Tab_FPC_Top_Longi"), ResultD[1][0].EvalD.ModeD.dFpc, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("Tab_FPD_Top_Longi"), ResultD[1][0].EvalD.EvalD.dFpd, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("Tab_FR_Top_Longi"), ResultD[1][0].EvalD.dRat, Row_Ref, Col_Ref, stRow);
			PrintCellDataMark(pExcel, _T("Tab_FR_Result_Top_Longi"), Chk_OK_Sign(0, 1.0, ResultD[1][0].EvalD.dRat), Row_Ref, Col_Ref, stRow);
		}

		stRow += rowCnt;
	}

	RptBlock = _T("[2]Seismic_Eval_Result_Trans");
	rowCnt = CopyRangeSubTitle(pExcel, RptBlock, stRow);
	stRow += rowCnt;

	if (enBehavior[1] == CANTILEVER)
	{
		RptBlock = _T("[2]Seismic_Eval_Result_Cantil_Trans");
		rowCnt = CopyRangeSubTitle(pExcel, RptBlock, stRow);
		GetCellIDByName(pExcel, RptBlock, Row_Ref, Col_Ref);

		if (ResultD[0][1].EvalD.bCheck)
		{
			PrintCellData(pExcel, _T("Tab_FType_Trans"), strFailure[1], Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("Tab_FPC_Trans"), ResultD[0][1].EvalD.ModeD.dFpc, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("Tab_FPD_Trans"), ResultD[0][1].EvalD.EvalD.dFpd, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("Tab_FR_Trans"), ResultD[0][1].EvalD.dRat, Row_Ref, Col_Ref, stRow);
			PrintCellDataMark(pExcel, _T("Tab_FR_Result_Trans"), Chk_OK_Sign(0, 1.0, ResultD[0][1].EvalD.dRat), Row_Ref, Col_Ref, stRow);
		}

		stRow += rowCnt + 1;
	}
	else
	{
		RptBlock = _T("[2]Seismic_Eval_Result_Rahmen_Trans");
		rowCnt = CopyRangeSubTitle(pExcel, RptBlock, stRow);
		GetCellIDByName(pExcel, RptBlock, Row_Ref, Col_Ref);

		if (ResultD[0][1].EvalD.bCheck)
		{
			PrintCellData(pExcel, _T("Tab_FType_Bot_Trans"), strFailure[1], Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("Tab_FPC_Bot_Trans"), ResultD[0][1].EvalD.ModeD.dFpc, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("Tab_FPD_Bot_Trans"), ResultD[0][1].EvalD.EvalD.dFpd, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("Tab_FR_Bot_Trans"), ResultD[0][1].EvalD.dRat, Row_Ref, Col_Ref, stRow);
			PrintCellDataMark(pExcel, _T("Tab_FR_Result_Bot_Trans"), Chk_OK_Sign(0, 1.0, ResultD[0][1].EvalD.dRat), Row_Ref, Col_Ref, stRow);
		}

		if (ResultD[1][1].EvalD.bCheck)
		{
			PrintCellData(pExcel, _T("Tab_FType_Top_Trans"), strFailure[3], Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("Tab_FPC_Top_Trans"), ResultD[1][1].EvalD.ModeD.dFpc, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("Tab_FPD_Top_Trans"), ResultD[1][1].EvalD.EvalD.dFpd, Row_Ref, Col_Ref, stRow);
			PrintCellData(pExcel, _T("Tab_FR_Top_Trans"), ResultD[1][1].EvalD.dRat, Row_Ref, Col_Ref, stRow);
			PrintCellDataMark(pExcel, _T("Tab_FR_Result_Top_Trans"), Chk_OK_Sign(0, 1.0, ResultD[1][1].EvalD.dRat), Row_Ref, Col_Ref, stRow);
		}

		stRow += rowCnt + 1;
	}


	return TRUE;
}

BOOL CBrdgSeisEvalKISTEC2015_Report::Make_SectionFig(CString strFileName, T_MPCC_D* pMpccD)
{
	int HSize = 6000;
	int BSize = 8000;
	TCHAR szDesc[] = _T("MFS Enhanced MetaFile Application");
	CRect canvas(0, 0, BSize, HSize);

	// EMF image
	return m_MCCalc.Make_SectionFig(strFileName, canvas, pMpccD);
}


BOOL CBrdgSeisEvalKISTEC2015_Report::WaitForFile(CString strFileName)
{
	std::chrono::milliseconds interval = std::chrono::milliseconds(1000);

	while (true) 
	{
		std::ifstream file(strFileName);
		if (file.good()) {
			file.close();
			return true; // 파일이 존재함
		}
		file.close();
		std::this_thread::sleep_for(interval); // 일정 시간 동안 대기
	}

	AfxMessageBox(_T("WaitForFile_FALSE: " + strFileName));
	return FALSE;
}


BOOL CBrdgSeisEvalKISTEC2015_Report::GetResult(const PairKey& pEvgpColm, T_BRDGEVAL_RESULT rData[2][2])
{
	CBrdgSeisEvalResult* pResult = m_pDoc->m_pPostCtrl->GetBrdgSeisEvalResult();

	int nCheck = 0;
	for (int i = 0; i < 2; ++i)
	{
		EN_POSITION enPos = i == 0 ? BOTTOM : TOP;
		for (int j = 0; j < 2; ++j)
		{
			EN_LOCAL_DIR enDir = j == 0 ? LONGI : TRANS;
			if (!pResult->ReadSeisEvalResultMPhi(pEvgpColm.first, pEvgpColm.second, enDir, enPos, rData[i][j].MPhiD)) rData[i][j].MPhiD.initialize();
			if (!pResult->ReadSeisEvalResultParam(pEvgpColm.first, pEvgpColm.second, enDir, enPos, 0, TRUE, rData[i][j].ParamD)) rData[i][j].ParamD.initialize();
			if (!pResult->ReadSeisEvalResultCapa(pEvgpColm.first, pEvgpColm.second, enDir, enPos, 0, TRUE, rData[i][j].CapaD)) rData[i][j].CapaD.initialize();
			if (!pResult->ReadSeisEvalResultEval(pEvgpColm.first, pEvgpColm.second, enDir, enPos, 0, TRUE, rData[i][j].EvalD)) rData[i][j].EvalD.initialize();

			if (!rData[i][j].EvalD.bCheck) nCheck++;
		}
	}

	return nCheck == 4 ? FALSE : TRUE;
}

INT_PTR  CBrdgSeisEvalKISTEC2015_Report::GetPrintList(arPairKey& raEvgpColm)
{
	raEvgpColm.RemoveAll();

	CBrdgSeisEvalResult* pResult = m_pDoc->m_pPostCtrl->GetBrdgSeisEvalResult();
	if (!pResult->ExistResult()) return 0;

	T_EVGP_D EvgpD; EvgpD.Initialize();
	CArray<T_EVGP_K, T_EVGP_K> arEvgpK; // 검토 대상 Evaluation Group List
	m_pDoc->m_pAttrCtrl2->GetEvgpKeyList(arEvgpK);
	int nEvgp = arEvgpK.GetCount();
	if (nEvgp == 0) return 0;

	// check result file
	for (int i = 0; i < nEvgp; ++i)
	{
		T_EVGP_D EvgpD;
		if (!m_pDoc->m_pAttrCtrl2->GetEvgp(arEvgpK[i], EvgpD)) continue;
		if (EvgpD.nType != 0) continue; // only pier.

		INT_PTR nCol = EvgpD.aEvgpColm.GetSize();
		for (UINT iCol = 0; iCol < nCol; ++iCol)
		{
			PairKey pEvgpCol(arEvgpK[i], iCol);// = make_pair(arEvgpK[i], iCol);

			T_EVPM_D EvpmD;
			if (!GetEvpm(arEvgpK[i], iCol, EvpmD)) continue;

			T_BRDGEVAL_RESULT ResultD[2][2]; // Bottom/Top, Longitude/Transverse
			if (!GetResult(pEvgpCol, ResultD)) continue; // 설계결과가 없는 Evaluation Group은 Skip

			raEvgpColm.Add(pEvgpCol);
		}
	}
	return raEvgpColm.GetSize();
}

INT_PTR  CBrdgSeisEvalKISTEC2015_Report::GetBearingPrintList(CArray<UINT, UINT>& raEvgpBearing, CArray<UINT, UINT>& raBrev)
{
	raEvgpBearing.RemoveAll();
	raBrev.RemoveAll();

	CBrdgSeisEvalResult* pResult = m_pDoc->m_pPostCtrl->GetBrdgSeisEvalResult();
	if (!pResult->ExistBearingResult()) return 0;

	CArray<T_BREV_K, T_BREV_K> arBrevK; arBrevK.RemoveAll();
	m_pDoc->m_pAttrCtrl2->GetBrevKeyList(arBrevK);
	INT_PTR nBrek = arBrevK.GetSize();
	if (nBrek == 0) return 0;

	// check result file
	for (int i = 0; i < nBrek; ++i)
	{
		T_BREV_D BrevD;
		if (!m_pDoc->m_pAttrCtrl2->GetBrev(arBrevK[i], BrevD)) continue;

		T_BRDGEVAL_BEARING ResD;
		if (!pResult->ReadSeisEvalResultBearing(arBrevK[i], 0, TRUE, ResD)) continue;
		if (!ResD.bCheck) continue;

		raEvgpBearing.Add(BrevD.EvgpK); //??// 배열로 들어오면 첫번째걸로~~
		raBrev.Add(arBrevK[i]);
	}
	return raEvgpBearing.GetSize();
}

CString CBrdgSeisEvalKISTEC2015_Report::Get_PierColmName(const T_EVGP_D& EvgpD, const int& iColIdx)
{
	// iColIdx >= 1
	CString strPier = EvgpD.strName;
	if (EvgpD.aEvgpColm.GetSize() > (iColIdx - 1))
	{
		strPier.Format(_T("%s(%s)"), EvgpD.strName, EvgpD.aEvgpColm[iColIdx - 1].strName);
	}

	return strPier;
}

CString CBrdgSeisEvalKISTEC2015_Report::Get_PierColmNameMixed(const T_EVGP_D& EvgpD, const int& iColIdx)
{
	CString strPier = EvgpD.strName;
	if (EvgpD.aEvgpColm.GetSize() > (iColIdx))
	{
		strPier.Format(_T("%s(%s)"), EvgpD.strName, EvgpD.aEvgpColm[iColIdx].strName);
	}

	return strPier;
}


void CBrdgSeisEvalKISTEC2015_Report::SetPrintItems(BOOL bPrintItems[8])
{
	for (int i = 0; i < 8; ++i)
	{
		m_bPrintItems[i] = bPrintItems[i];
	}
}

BOOL CBrdgSeisEvalKISTEC2015_Report::GenerateEvalPlainConcSheet(IExcel* pExcel, CString& RptBlock, int& stRow, const EN_BEHAVIOR_TYPE* enBehavior, T_BRDGEVAL_RESULT pResultD[2][2])
{
	// Plain concrete Pier
	ASSERT(0);
	return TRUE;
}

void CBrdgSeisEvalKISTEC2015_Report::GetCellIDByName(IExcel* pExcel, CString sRangeName, short& rowCnt, short& colCnt)
{
	pExcel->GetCellIDByName(sRangeName, rowCnt, colCnt);
	if(colCnt>0) colCnt--; // First column in base Excel is not used.
}