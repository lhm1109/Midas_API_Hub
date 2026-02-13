// Rating_RKMC_XLOut.cpp: implementation of the CRating_RKMC_XLOut class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "..\DgnEngine\Src\DgnCalcBase\DgnCalcBase_PSC_Struct.h"
#include "..\DgnEngine\Src\DgnCalcBase\DgnCalcBase_PSC_ResultStruct.h"

#include "Rating_RKMC_XLOut.h"
#include "DgnRating_KSCE_USD05.h"
#include "Dgn_PSCRptManager_LSD12_Comp.h"
#include "..\wg_base\Product.h"
#include "..\wg_base\wg_base_MsgDll.h"
#include "..\wg_base\TestEnvMgr.h"
#include "..\wg_dgn\wg_dgn_DgnProgressDlg.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl2.h"
#include "..\wg_db\wg_db_PostCtrl.h"
#include "..\wg_db\wg_db_AnalysisResult.h"
#include "..\wg_db\wg_db_UnitCtrl.h"

#include "..\..\DgnEngine\idesign\DGN_lib\ExcelGenerator.h"

using namespace dgn::lib;

#if _MSC_VER >= 1700  // vc++11.0 //sjhuh-2014-02-21
using namespace std;
#include "..\MIT_Lib\rptostream.h"	  // for rptwofstream
#else
#include <fstream.h>	// for rptwofstream
#endif

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define D_NUMBER_FORMAT_3  "0.000_ "

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRating_RKMC_XLOut::CRating_RKMC_XLOut() : CDgn_XLOut()
{

}

CRating_RKMC_XLOut::~CRating_RKMC_XLOut()
{
}

void CRating_RKMC_XLOut::SetDataCtrlPointer(CCRCDataCtrl* pDataCtrl)
{
	m_pDataCtrl = pDataCtrl;
}

void CRating_RKMC_XLOut::PrintReport(CString strPath, int iPrintOpt)
{
	// Code Set!!      
	T_RACD_D RacdD; RacdD.Initialize();
	m_pDoc->m_pAttrCtrl2->GetRacd(RacdD);
	m_iCode = RacdD.nRatingCode;

	// Data Set!!
	CArray<T_RKMC_D, T_RKMC_D&> arRKMCData;  arRKMCData.RemoveAll();
	CArray<T_RKST_D, T_RKST_D&> arRKSTData;  arRKSTData.RemoveAll();

	CArray<T_RKCE_K, T_RKCE_K> arKeyList; arKeyList.RemoveAll();
	m_pAttrCtrl->GetRkceKeyList(arKeyList);

	CArray<T_RKCV_K, T_RKCV_K> arKeyVBeam;
	m_pAttrCtrl->GetRkcvKeyList(arKeyVBeam);

	ArrElemPairKey aRptElemPairK;
	CDBLib::ConvertToElemPairKeyList(arKeyList, arKeyVBeam, aRptElemPairK);

	int nRkceCount = aRptElemPairK.GetSize();

	T_RKMC_D RkmcD; RkmcD.Initialize();
	T_RKST_D RkstD; RkstD.Initialize();

	T_RKCE_D RkceD; RkceD.Initialize();
	T_RAKR_D RakrD; RakrD.Initialize();

	for ( int i = 0; i < nRkceCount; i++ )
	{
		auto ElemKey = aRptElemPairK[i];
		m_pDoc->m_pAttrCtrl2->GetRakr(RakrD);  // for 강도/허용응력 설계법 구분!!!

		m_pDoc->m_pPostCtrl->GetAnalysisResult()->GetConcreteStressRating4TableKR05(ElemKey, RkstD);    // RKST
		m_pDoc->m_pPostCtrl->GetAnalysisResult()->GetFlexuralStrengthRating4TableKR05(ElemKey, RkmcD);  // RKMC

		if ( RakrD.bPrintParm[0] ) arRKMCData.Add(RkmcD);  // 강도설계법
		if ( RakrD.bPrintParm[1] ) arRKSTData.Add(RkstD);  // 허용응력설계법
	}

	T_UNIT_INDEX CodeUnit;
	CodeUnit.nBase_Force	= D_UNITSYS_FORCE_INDEX_N;
	CodeUnit.nBase_Length = D_UNITSYS_LENGTH_INDEX_MM;
	T_UNIT_INDEX CurrUnit;
	m_pDoc->m_pUnitCtrl->GetUnitIndexCurrent(CurrUnit);
	m_pDoc->m_pUnitCtrl->SetUnitIndexCurrent(CodeUnit);
	//
	if ( m_bTestMode )  PrintReportText(strPath, arRKMCData, arRKSTData);
	else             PrintReportExcel(strPath, iPrintOpt, arRKMCData, arRKSTData);
	//
	m_pDoc->m_pUnitCtrl->SetUnitIndexCurrent(CurrUnit);
}

void CRating_RKMC_XLOut::PrintReportExcel(CString strPath, int iPrintOpt, CArray<T_RKMC_D, T_RKMC_D&>& arRKMCData, CArray<T_RKST_D, T_RKST_D&>& arRKSTData)
{
	// Create Progress Dialog.
	CArray<T_RKPO_K, T_RKPO_K> arElemKeyList; arElemKeyList.RemoveAll();
	m_pDoc->m_pAttrCtrl->GetRkpoKeyList(arElemKeyList);

	CArray<T_RKPV_K, T_RKPV_K> aPrintVBeamKey;
	m_pDoc->m_pAttrCtrl->GetRkpvKeyList(aPrintVBeamKey);

	ArrElemPairKey aPrintElemPairK;
	CDBLib::ConvertToElemPairKeyList(arElemKeyList, aPrintVBeamKey, aPrintElemPairK);

	int nCountSelectData = aPrintElemPairK.GetSize();

	T_RKPO_D RkpoD; RkpoD.Initialize();
	T_RKMC_D RkmcD; RkmcD.Initialize();
	T_RKST_D RkstD; RkstD.Initialize();

	T_RKMC_BASE RkmcBaseI_Max, RkmcBaseI_Min;
	T_RKMC_BASE RkmcBaseJ_Max, RkmcBaseJ_Min;
	RkmcBaseI_Max.Initialize(); RkmcBaseI_Min.Initialize();
	RkmcBaseJ_Max.Initialize(); RkmcBaseJ_Min.Initialize();

	T_RKST_BASE RkstBaseI_1, RkstBaseI_2, RkstBaseI_3, RkstBaseI_4;
	T_RKST_BASE RkstBaseJ_1, RkstBaseJ_2, RkstBaseJ_3, RkstBaseJ_4;
	RkstBaseI_1.Initialize(); RkstBaseI_2.Initialize(); RkstBaseI_3.Initialize(); RkstBaseI_4.Initialize();
	RkstBaseJ_1.Initialize(); RkstBaseJ_2.Initialize(); RkstBaseJ_3.Initialize(); RkstBaseJ_4.Initialize();

	//(2008.04.02) Add by Unsang :: 출력할 수 있는 요소를 List Up
	m_arRkmcKeyList.RemoveAll(); m_arRkmcKeyList.SetSize(0);
	m_arRkstKeyList.RemoveAll(); m_arRkstKeyList.SetSize(0);

	BOOL bPrintChk = FALSE;
	//(2008.04.02) Add by Unsang :: 설계Data를 검토해서 출력 여부 확인함!!
	/// [RPMNX-442] 검토항목이 하나라도 있으면 출력하도록 수정. (검토위치의 모든 결과 TRUE이면 출력 -> 하나라도 TRUE이면 출력)
	for ( int i = 0; i < nCountSelectData; i++ )
	{
		// 강도설계법으로 적용하여 검토된 요소를 찾음.
		auto RkmcK = aPrintElemPairK[i];
		if ( m_pDoc->m_pPostCtrl->GetAnalysisResult()->GetFlexuralStrengthRating4TableKR05(RkmcK, RkmcD) )
		{
			RkpoD.Initialize(); 
			if (RkmcK.second == EN_EL_BEAM)
			{
				m_pDoc->m_pAttrCtrl->GetRkpo(RkmcK.first, RkpoD);
			}
			else if (RkmcK.second == EN_EL_VBEAM)
			{
				m_pDoc->m_pAttrCtrl->GetRkpv(RkmcK.first, RkpoD);
			}
			else ASSERT(0);
			
			if ( RkpoD.iPrintChk == 0 ) // I-End
			{
				RkmcBaseI_Max = RkmcD.RkmcBase[0];
				RkmcBaseI_Min = RkmcD.RkmcBase[1];
								
				if ( RkmcBaseI_Max.bCheck || RkmcBaseI_Min.bCheck ) bPrintChk = TRUE;
			}
			else if ( RkpoD.iPrintChk == 1 ) // J-End
			{
				RkmcBaseJ_Max = RkmcD.RkmcBase[2];
				RkmcBaseJ_Min = RkmcD.RkmcBase[3];
				if ( RkmcBaseJ_Max.bCheck || RkmcBaseJ_Min.bCheck ) bPrintChk = TRUE;
			}
			else if ( RkpoD.iPrintChk == 2 ) // I-End & J-End
			{
				RkmcBaseI_Max = RkmcD.RkmcBase[0];
				RkmcBaseI_Min = RkmcD.RkmcBase[1];
				RkmcBaseJ_Max = RkmcD.RkmcBase[2];
				RkmcBaseJ_Min = RkmcD.RkmcBase[3];

				if ( RkmcBaseI_Max.bCheck || RkmcBaseI_Min.bCheck || RkmcBaseJ_Max.bCheck || RkmcBaseJ_Min.bCheck ) bPrintChk = TRUE;
			}
			else
				ASSERT(0);

			if ( bPrintChk ) m_arRkmcKeyList.Add(RkmcK);

			bPrintChk = FALSE; // Initialize!!
		}

		// 허용응력설계법으로 적용하여 검토된 요소를 찾음.		
		auto RkstK = aPrintElemPairK[i];
		if ( m_pDoc->m_pPostCtrl->GetAnalysisResult()->GetConcreteStressRating4TableKR05(RkstK, RkstD) )
		{
			RkpoD.Initialize(); 
			if (RkstK.second == EN_EL_BEAM)
			{
				m_pDoc->m_pAttrCtrl->GetRkpo(RkstK.first, RkpoD);
			}
			else if (RkstK.second == EN_EL_VBEAM)
			{
				m_pDoc->m_pAttrCtrl->GetRkpv(RkstK.first, RkpoD);
			}
			else ASSERT(0);

			if ( RkpoD.iPrintChk == 0 )
			{
				RkstBaseI_1 = RkstD.BaseI[0]; 
				RkstBaseI_2 = RkstD.BaseI[1];
				RkstBaseI_3 = RkstD.BaseI[2]; 
				RkstBaseI_4 = RkstD.BaseI[3];
				bPrintChk = [RkstD]()
				{
					if (RkstD.BaseI[0].bCheck) return TRUE;
					if (RkstD.BaseI[1].bCheck) return TRUE;
					if (RkstD.BaseI[2].bCheck) return TRUE;
					if (RkstD.BaseI[3].bCheck) return TRUE;
					return FALSE;
				}();
			}
			else if ( RkpoD.iPrintChk == 1 )
			{
				RkstBaseJ_1 = RkstD.BaseJ[0]; 
				RkstBaseJ_2 = RkstD.BaseJ[1];
				RkstBaseJ_3 = RkstD.BaseJ[2]; 
				RkstBaseJ_4 = RkstD.BaseJ[3];
				bPrintChk = [RkstD]()
				{
					if (RkstD.BaseJ[0].bCheck) return TRUE;
					if (RkstD.BaseJ[1].bCheck) return TRUE;
					if (RkstD.BaseJ[2].bCheck) return TRUE;
					if (RkstD.BaseJ[3].bCheck) return TRUE;
					return FALSE;
				}();				
			}
			else if ( RkpoD.iPrintChk == 2 )
			{
				RkstBaseI_1 = RkstD.BaseI[0]; 
				RkstBaseI_2 = RkstD.BaseI[1];
				RkstBaseI_3 = RkstD.BaseI[2];
				RkstBaseI_4 = RkstD.BaseI[3];

				RkstBaseJ_1 = RkstD.BaseJ[0];
				RkstBaseJ_2 = RkstD.BaseJ[1];
				RkstBaseJ_3 = RkstD.BaseJ[2];
				RkstBaseJ_4 = RkstD.BaseJ[3];

				bPrintChk = [RkstD]()
				{
					if (RkstD.BaseI[0].bCheck) return TRUE;
					if (RkstD.BaseI[1].bCheck) return TRUE;
					if (RkstD.BaseI[2].bCheck) return TRUE;
					if (RkstD.BaseI[3].bCheck) return TRUE;
					if (RkstD.BaseJ[0].bCheck) return TRUE;
					if (RkstD.BaseJ[1].bCheck) return TRUE;
					if (RkstD.BaseJ[2].bCheck) return TRUE;
					if (RkstD.BaseJ[3].bCheck) return TRUE;
					return FALSE;
				}();
			}
			else
				ASSERT(0);

			if ( bPrintChk ) m_arRkstKeyList.Add(RkstK);

			bPrintChk = FALSE;
		}
	}

	int nCountRkmcData = m_arRkmcKeyList.GetSize();
	int nCountRkstData = m_arRkstKeyList.GetSize();
	CString strErrMessage = _T("");
	if ( nCountRkmcData == 0 && nCountRkstData == 0 )
	{
		if ( nCountSelectData == 0 )
			strErrMessage = _LS(IDS_DGN_PSC_RATING_NON_MEMBER);
		else
			strErrMessage = _LS(IDS_DGN_PSC_RATING_NOT_INPUT_MEMBER);

		AfxMessageBox(strErrMessage);
		return;
	}

	CDgnProgressDlg ProDlg;
	ProDlg.Create_ProgressDlg(PROGRESS_TYPE_PSC_RATING_XL_RPT);// PSC Rating XL Report -> 202

	m_strBasePath = m_pDoc->GetProgramPath() + _T("Excel Base File\\");
	//m_strBaseBook = _T("rating_base_ver01.dll");
	m_strBaseBook = _T("rating_base_ver01.xlsx");

	CExcelGenerator Excel(_T(""), XLTYPE::TYPE_LIBXL);
	CString strFileName = Excel.GetDLLFilePath(m_strBaseBook, FALSE);

	IExcel* pXL = Excel.Generate(strFileName);
	if(Excel.GetExcelType() == XLTYPE::TYPE_LIBXL)
	{
		m_strBaseBook = strFileName; // 경로 포함.
	}

	SetOutputUnitSystem();
	SetUnitConvertFactor();

	SetRKMCDetailSheetPage(pXL);
	SetRKSTDetailSheetPage(pXL);

	BOOL bStopCalc = FALSE;
	CArray<D_CELL, D_CELL&> arData;
	int stRowD = 0;

	// 강도설계법 & 허용응력설계법 검토방식 확인..
	BOOL bPrtRKMC = FALSE; // 강도    설계법 출력 여부 확인 :: TRUE->Prt / FALSE->Not
	BOOL bPrtRKST = FALSE; // 허용응력설계법 출력 여부 확인 :: TRUE->Prt / FALSE->Not
	T_RAKR_D RakrD; RakrD.Initialize();
	m_pDoc->m_pAttrCtrl2->GetRakr(RakrD);
	if ( RakrD.bPrintParm[0] )
		bPrtRKMC = TRUE;

	if ( RakrD.bPrintParm[1] )
		bPrtRKST = TRUE;

	if ( RakrD.bPrintParm[2] )
	{
		bPrtRKMC = TRUE;
		bPrtRKST = TRUE;
	}

	if ( bPrtRKMC && nCountRkmcData > 0 )
	{
		pXL->SetActiveSheet(m_strDetlSheet1);
		if ( m_iCode == KSCE_USD05_RATING )
		{
			// 1. 내하율(Rating Factor) 산정
				//1-1. Comment!!
			SetGetRkmcRatingFct(arRKMCData, arData);
			PrintGetRkmcRatingFct(pXL, stRowD, 1, arData, nCountRkmcData);
			//1-2. Result Table!!
			SetGetRkmcRatingFctTableHead(arRKMCData, arData);
			PrintGetRkmcRatingFctTableHead(pXL, stRowD, 1, arData, nCountRkmcData);
			//1-3. Result Table Body!!
			SetGetRkmcRatingFctTableBody(arRKMCData, arData);
			PrintGetRkmcRatingFctTableBody(pXL, stRowD, 1, arData, nCountRkmcData);

			// 2. 실측보정계수(Ks) 산정
				//2-1. Comment!!
			SetGetRkmcKsFct(arRKMCData, arData);
			PrintGetRkmcKsFct(pXL, stRowD, 1, arData, nCountRkmcData);
			//2-2. Result Table Head!!
			SetGetRkmcKsFctTableHead(arRKMCData, arData);
			PrintGetRkmcKsFctTableHead(pXL, stRowD, 1, arData, nCountRkmcData);
			//2-3. Result Table Body!!
			SetGetRkmcKsFctTableBody(arRKMCData, arData);
			PrintGetRkmcKsFctTableBody(pXL, stRowD, 1, arData, nCountRkmcData);

			// 3. 내하력(Capacity) 산정
				//3-1. Comment
			SetGetRkmcCapacity(arRKMCData, arData);
			PrintGetRkmcCapacity(pXL, stRowD, 1, arData, nCountRkmcData);
			//3-2. Result Table Head!!
			SetGetRkmcCapacityTableHead(arRKMCData, arData);
			PrintGetRkmcCapacityTableHead(pXL, stRowD, 1, arData, nCountRkmcData);
			//3-3. Result Table Body!!
			SetGetRkmcCapacityTableBody(arRKMCData, arData);
			PrintGetRkmcCapacityTableBody(pXL, stRowD, 1, arData, nCountRkmcData);

			if ( ProDlg.Update_ProgressDlg(nCountRkmcData+1, nCountRkmcData+nCountRkstData+1) )
			{
				bStopCalc = TRUE;
			}
		}
		else
			ASSERT(0);
	}

	// Sheet가 바뀌면 초기화.
	stRowD = 0;
	if ( bPrtRKST && nCountRkstData > 0 )
	{
		pXL->SetActiveSheet(m_strDetlSheet2);
		if ( m_iCode == KSCE_USD05_RATING )
		{
			// 1. 내하율(Rating Factor) 산정
				//1-1. Comment!!
			SetGetRkstRatingFct(arRKSTData, arData);
			PrintGetRkstRatingFct(pXL, stRowD, 1, arData, nCountRkstData);
			//1-2. Result Table Head!!
			SetGetRkstRatingFctTableHead(arRKSTData, arData);
			PrintGetRkstRatingFctTableHead(pXL, stRowD, 1, arData, nCountRkstData);
			//1-3. Result Table Bodyu!!
			SetGetRkstRatingFctTableBody(arRKSTData, arData);
			PrintGetRkstRatingFctTableBody(pXL, stRowD, 1, arData, nCountRkstData);

			// 2. 실측보정계수(Ks) 산정
				//2-1. Comment!!
			SetGetRkstKsFct(arRKSTData, arData);
			PrintGetRkstKsFct(pXL, stRowD, 1, arData, nCountRkstData);
			//2-2. Result Table Head!!
			SetGetRkstKsFctTableHead(arRKSTData, arData);
			PrintGetRkstKsFctTableHead(pXL, stRowD, 1, arData, nCountRkstData);
			//2-3. Result Table Body!!
			SetGetRkstKsFctTableBody(arRKSTData, arData);
			PrintGetRkstKsFctTableBody(pXL, stRowD, 1, arData, nCountRkstData);

			// 3. 내하력(Capacity) 산정
				//3-1. Comment
			SetGetRkstCapacity(arRKSTData, arData);
			PrintGetRkstCapacity(pXL, stRowD, 1, arData, nCountRkstData);
			//3-2. Result Table Head!!
			SetGetRkstCapacityTableHead(arRKSTData, arData);
			PrintGetRkstCapacityTableHead(pXL, stRowD, 1, arData, nCountRkstData);
			//3-3. Result Table Body!!
			SetGetRkstCapacityTableBody(arRKSTData, arData);
			PrintGetRkstCapacityTableBody(pXL, stRowD, 1, arData, nCountRkstData);

			if ( ProDlg.Update_ProgressDlg(nCountRkmcData+nCountRkstData+1, nCountRkmcData+nCountRkstData+1) )
			{
				bStopCalc = TRUE;
			}
		}
		else
			ASSERT(0);
	}

	if(pXL->GetExcelType() == XLTYPE::TYPE_LIBXL)
	{
		pXL->GetBook()->setActiveSheet(0);
	}
	pXL->EndExcel(strPath);
	delete pXL;

	ShellExecute(NULL, _T("open"), (strPath), NULL, NULL, SW_SHOWNORMAL);

	ProDlg.Delete_ProgressDlg();

	// ('06.06.28). For Regression test : After generating excel file, pop up AfxMessageBox.
	if ( m_bTestMode )
	{
		AfxMessageBox(_T("Finish generating excel file."), MB_OK);
	}
}

void CRating_RKMC_XLOut::PrintReportText(CString strPath, CArray<T_RKMC_D, T_RKMC_D&>& arRKMCData, CArray<T_RKST_D, T_RKST_D&>& arRKSTData)
{
	//*/PROFILING(_T("Print Report Text"));

	// Set File name for text output.
	CFileCtrl FileCtrl(strPath);
	m_strExcelTextFile = FileCtrl.GetFilePathAndNameWithoutExtension() + _T(".txt");

	rptwofstream fout1(m_strExcelTextFile, ios::trunc);
	fout1.close();

	rptwofstream fout(m_strExcelTextFile, ios::app);

	SetOutputUnitSystem();
	SetUnitConvertFactor();

	CString strValue = _T("");
	CArray<D_CELL, D_CELL&> arData;

	/*TEST*/ //(2008.02.26) Add by Unsang :: Add Code For Text_Print Work!!

	fout.close();

	// ('06.06.28). For Regression test : After generating excel file, pop up AfxMessageBox.
	if ( m_bTestMode )
	{
		AfxMessageBox(_T("Finish generating Text file."), MB_OK);
	}
}


////////////////////////////////////////////////////////////////////////////
// Sheet의 Page 설정.
////////////////////////////////////////////////////////////////////////////
void CRating_RKMC_XLOut::SetRKMCDetailSheetPage(IExcel* pExcel)
{
	m_strDetlSheet1 = _T("강도설계법");
	if ( pExcel->IsExistSheetName(m_strDetlSheet1) )
	{
		m_strDetlSheet1 = pExcel->GetMaximumSheetName(m_strDetlSheet1) + _T("_New");
	}
	pExcel->AddSheet2(m_strDetlSheet1); // AddSheet2 :: 맨뒤에 추가
	pExcel->SetActiveSheet(m_strDetlSheet1);

	SetDefaultSheetProperty(pExcel);
}

void CRating_RKMC_XLOut::SetRKSTDetailSheetPage(IExcel* pExcel)
{
	m_strDetlSheet2 = _T("허용응력설계법");
	if ( pExcel->IsExistSheetName(m_strDetlSheet2) )
	{
		m_strDetlSheet2 = pExcel->GetMaximumSheetName(m_strDetlSheet2) + _T("_New");
	}
	pExcel->AddSheet2(m_strDetlSheet2);
	pExcel->SetActiveSheet(m_strDetlSheet2);

	SetDefaultSheetProperty(pExcel);
}

// RKMC
void CRating_RKMC_XLOut::SetGetRkmcRatingFct(CArray<T_RKMC_D, T_RKMC_D&>& arRKMCData, CArray<D_CELL, D_CELL&>& arData)
{
	arData.RemoveAll();
}

void CRating_RKMC_XLOut::PrintGetRkmcRatingFct(IExcel* pExcel, int& stRow, int stCol, CArray<D_CELL, D_CELL&>& arData, int nCountSelectData)
{
	int rowCnt = 0;
	int colCnt = 0;

	CString strTableHead = _T("");
	if ( m_iCode == KSCE_USD05_RATING )
		strTableHead = _T("Str_Calc_RatingFct");
	else
	{
		ASSERT(0);
		return;
	}

	pExcel->CopyRange(stRow, stCol, m_strBaseBook, _T("Str_Rating"), strTableHead, rowCnt, colCnt);
	Print_Array_D_CELL(pExcel, arData, stRow+1);
	stRow += rowCnt + 1;
}

void CRating_RKMC_XLOut::SetGetRkmcRatingFctTableHead(CArray<T_RKMC_D, T_RKMC_D&>& arRKMCData, CArray<D_CELL, D_CELL&>& arData)
{
	arData.RemoveAll();
}

void CRating_RKMC_XLOut::PrintGetRkmcRatingFctTableHead(IExcel* pExcel, int& stRow, int stCol, CArray<D_CELL, D_CELL&>& arData, int nCountSelectData)
{
	int rowCnt = 0;
	int colCnt = 0;
	int nIncreaseRow = 0;

	CString strTableHead = _T("");
	if ( m_iCode == KSCE_USD05_RATING )
		strTableHead = _T("Str_Calc_RatingFct_Head");
	else
	{
		ASSERT(0);
		return;
	}

	pExcel->CopyRange(stRow, stCol, m_strBaseBook, _T("Str_Rating"), strTableHead, rowCnt, colCnt);
	Print_Array_D_CELL(pExcel, arData, stRow+1);
	stRow += rowCnt;
}

void CRating_RKMC_XLOut::SetGetRkmcRatingFctTableBody(CArray<T_RKMC_D, T_RKMC_D&>& arRKMCData, CArray<D_CELL, D_CELL&>& arData)
{
	int nIncreaseRow = 0;
	arData.RemoveAll();

	int nCountSelectData = m_arRkmcKeyList.GetSize();
	if ( nCountSelectData <= 0 ) return;


	T_RKPO_D RkpoD; RkpoD.Initialize();
	T_RKMC_D RkmcD; RkmcD.Initialize();

	T_RKMC_BASE RkmcBase_1, RkmcBase_2, RkmcBase_3, RkmcBase_4;
	RkmcBase_1.Initialize();  RkmcBase_2.Initialize();  RkmcBase_3.Initialize(); RkmcBase_4.Initialize();

	T_RKLC_K RklcKI_1, RklcKI_2, RklcKJ_1, RklcKJ_2;
	RklcKI_1 = RklcKI_2 = RklcKJ_1 = RklcKJ_2 = 0;

	T_RKLC_D RklcDI_1, RklcDI_2, RklcDJ_1, RklcDJ_2;
	RklcDI_1.Initialize();  RklcDI_2.Initialize();  RklcDJ_1.Initialize();  RklcDJ_2.Initialize();

	CString strLcomType_I_1 = _T("");
	CString strLcomType_I_2 = _T("");
	CString strLcomType_J_1 = _T("");
	CString strLcomType_J_2 = _T("");

	for ( int i = 0; i < nCountSelectData; i++ )
	{
		auto RkpoK = m_arRkmcKeyList[i];
		RkpoD.Initialize();
		if (RkpoK.second == EN_EL_BEAM)
		{
			m_pDoc->m_pAttrCtrl->GetRkpo(RkpoK.first, RkpoD);
		}
		else if (RkpoK.second == EN_EL_VBEAM)
		{
			m_pDoc->m_pAttrCtrl->GetRkpv(RkpoK.first, RkpoD);
		}
		else ASSERT(0);

		RkmcD.Initialize();
		m_pDoc->m_pPostCtrl->GetAnalysisResult()->GetFlexuralStrengthRating4TableKR05(RkpoK, RkmcD);

		if ( RkpoD.iPrintChk == 0 ) // I-End
		{
			RkmcBase_1.Initialize(); RkmcBase_1 = RkmcD.RkmcBase[0];
			RkmcBase_2.Initialize(); RkmcBase_2 = RkmcD.RkmcBase[1];

			// Element No
			arData.Add(D_CELL(nIncreaseRow, 1, GetOutputFormat(RkmcBase_1.ElemK, 0)));

			// Load Combination
			RklcKI_1 = RkmcBase_1.nRCaseK;
			RklcDI_1.Initialize(); m_pDoc->m_pAttrCtrl->GetRklc(RklcKI_1, RklcDI_1);

			RklcKI_2 = RkmcBase_2.nRCaseK;
			RklcDI_2.Initialize(); m_pDoc->m_pAttrCtrl->GetRklc(RklcKI_2, RklcDI_2);

			strLcomType_I_1 = RklcDI_1.strCaseName + _T("(") + GetLcomType(RkmcBase_1.nConcurrent) + _T(")");
			strLcomType_I_2 = RklcDI_2.strCaseName + _T("(") + GetLcomType(RkmcBase_2.nConcurrent) + _T(")");

			arData.Add(D_CELL(nIncreaseRow, 8, strLcomType_I_1));
			arData.Add(D_CELL(nIncreaseRow+1, 8, strLcomType_I_2));

			// Allow Stress
			arData.Add(D_CELL(nIncreaseRow, 11, GetOutputFormat(RkmcBase_1.dPhiMn * m_dFactorMoment, 3), D_NUMBER_FORMAT_3));
			arData.Add(D_CELL(nIncreaseRow+1, 11, GetOutputFormat(RkmcBase_2.dPhiMn * m_dFactorMoment, 3), D_NUMBER_FORMAT_3));

			// Stress By DeadLoad
			arData.Add(D_CELL(nIncreaseRow, 15, GetOutputFormat(RkmcBase_1.dMd_m * m_dFactorMoment, 3), D_NUMBER_FORMAT_3));
			arData.Add(D_CELL(nIncreaseRow+1, 15, GetOutputFormat(RkmcBase_2.dMd_m * m_dFactorMoment, 3), D_NUMBER_FORMAT_3));

			// Stress By LiveLoad
			arData.Add(D_CELL(nIncreaseRow, 19, GetOutputFormat(RkmcBase_1.dM_L * m_dFactorMoment, 3), D_NUMBER_FORMAT_3));
			arData.Add(D_CELL(nIncreaseRow+1, 19, GetOutputFormat(RkmcBase_2.dM_L * m_dFactorMoment, 3), D_NUMBER_FORMAT_3));

			// Impact Fct plus 1
			arData.Add(D_CELL(nIncreaseRow, 23, GetOutputFormat(RkmcBase_1.d1_plus_i_calc, 3), D_NUMBER_FORMAT_3));
			arData.Add(D_CELL(nIncreaseRow+1, 23, GetOutputFormat(RkmcBase_2.d1_plus_i_calc, 3), D_NUMBER_FORMAT_3));

			// Rating Fcf.
			arData.Add(D_CELL(nIncreaseRow, 27, GetOutputFormat(RkmcBase_1.dRF, 3), D_NUMBER_FORMAT_3));
			arData.Add(D_CELL(nIncreaseRow+1, 27, GetOutputFormat(RkmcBase_2.dRF, 3), D_NUMBER_FORMAT_3));

			nIncreaseRow = nIncreaseRow + 2;
		}
		else if ( RkpoD.iPrintChk == 1 ) // J -End
		{
			RkmcBase_3.Initialize(); RkmcBase_3 = RkmcD.RkmcBase[2];
			RkmcBase_4.Initialize(); RkmcBase_4 = RkmcD.RkmcBase[3];
			// Element No 
			arData.Add(D_CELL(nIncreaseRow, 1, GetOutputFormat(RkmcBase_3.ElemK, 0)));

			// Load Combination
			RklcKJ_1 = RkmcBase_3.nRCaseK;
			RklcDJ_1.Initialize(); m_pDoc->m_pAttrCtrl->GetRklc(RklcKJ_1, RklcDJ_1);

			RklcKJ_2 = RkmcBase_4.nRCaseK;
			RklcDJ_2.Initialize(); m_pDoc->m_pAttrCtrl->GetRklc(RklcKJ_2, RklcDJ_2);

			strLcomType_J_1 = RklcDI_1.strCaseName + _T("(") + GetLcomType(RkmcBase_3.nConcurrent) + _T(")");
			strLcomType_J_2 = RklcDI_2.strCaseName + _T("(") + GetLcomType(RkmcBase_4.nConcurrent) + _T(")");

			arData.Add(D_CELL(nIncreaseRow, 8, strLcomType_J_1));
			arData.Add(D_CELL(nIncreaseRow+1, 8, strLcomType_J_2));

			// Allow Stress
			arData.Add(D_CELL(nIncreaseRow, 11, GetOutputFormat(RkmcBase_3.dPhiMn * m_dFactorMoment, 3), D_NUMBER_FORMAT_3));
			arData.Add(D_CELL(nIncreaseRow+1, 11, GetOutputFormat(RkmcBase_4.dPhiMn * m_dFactorMoment, 3), D_NUMBER_FORMAT_3));

			// Stress By DeadLoad
			arData.Add(D_CELL(nIncreaseRow, 15, GetOutputFormat(RkmcBase_3.dMd_m * m_dFactorMoment, 3), D_NUMBER_FORMAT_3));
			arData.Add(D_CELL(nIncreaseRow+1, 15, GetOutputFormat(RkmcBase_4.dMd_m * m_dFactorMoment, 3), D_NUMBER_FORMAT_3));

			// Stress By LiveLoad
			arData.Add(D_CELL(nIncreaseRow, 19, GetOutputFormat(RkmcBase_3.dM_L * m_dFactorMoment, 3), D_NUMBER_FORMAT_3));
			arData.Add(D_CELL(nIncreaseRow+1, 19, GetOutputFormat(RkmcBase_4.dM_L * m_dFactorMoment, 3), D_NUMBER_FORMAT_3));

			// Impact Fct plus 1
			arData.Add(D_CELL(nIncreaseRow, 23, GetOutputFormat(RkmcBase_3.d1_plus_i_calc, 3), D_NUMBER_FORMAT_3));
			arData.Add(D_CELL(nIncreaseRow+1, 23, GetOutputFormat(RkmcBase_4.d1_plus_i_calc, 3), D_NUMBER_FORMAT_3));

			// Rating Fcf.
			arData.Add(D_CELL(nIncreaseRow, 27, GetOutputFormat(RkmcBase_3.dRF, 3), D_NUMBER_FORMAT_3));
			arData.Add(D_CELL(nIncreaseRow+1, 27, GetOutputFormat(RkmcBase_4.dRF, 3), D_NUMBER_FORMAT_3));

			nIncreaseRow = nIncreaseRow + 2;
		}
		else if ( RkpoD.iPrintChk == 2 ) // I-End & J-End
		{
			RkmcBase_1.Initialize(); RkmcBase_1 = RkmcD.RkmcBase[0];
			RkmcBase_2.Initialize(); RkmcBase_2 = RkmcD.RkmcBase[1];
			RkmcBase_3.Initialize(); RkmcBase_3 = RkmcD.RkmcBase[2];
			RkmcBase_4.Initialize(); RkmcBase_4 = RkmcD.RkmcBase[3];

			// Element No
			arData.Add(D_CELL(nIncreaseRow, 1, GetOutputFormat(RkmcBase_1.ElemK, 0)));

			// Load Combination
			RklcKI_1 = RkmcBase_1.nRCaseK;
			RklcDI_1.Initialize(); m_pDoc->m_pAttrCtrl->GetRklc(RklcKI_1, RklcDI_1);

			RklcKI_2 = RkmcBase_2.nRCaseK;
			RklcDI_2.Initialize(); m_pDoc->m_pAttrCtrl->GetRklc(RklcKI_2, RklcDI_2);

			RklcKJ_1 = RkmcBase_3.nRCaseK;
			RklcDJ_1.Initialize(); m_pDoc->m_pAttrCtrl->GetRklc(RklcKJ_1, RklcDJ_1);

			RklcKJ_2 = RkmcBase_4.nRCaseK;
			RklcDJ_2.Initialize(); m_pDoc->m_pAttrCtrl->GetRklc(RklcKJ_2, RklcDJ_2);

			strLcomType_I_1 = RklcDI_1.strCaseName + _T("(") + GetLcomType(RkmcBase_1.nConcurrent) + _T(")");
			strLcomType_I_2 = RklcDI_2.strCaseName + _T("(") + GetLcomType(RkmcBase_2.nConcurrent) + _T(")");
			strLcomType_J_1 = RklcDJ_1.strCaseName + _T("(") + GetLcomType(RkmcBase_3.nConcurrent) + _T(")");
			strLcomType_J_2 = RklcDJ_2.strCaseName + _T("(") + GetLcomType(RkmcBase_4.nConcurrent) + _T(")");


			arData.Add(D_CELL(nIncreaseRow, 8, strLcomType_I_1));
			arData.Add(D_CELL(nIncreaseRow+1, 8, strLcomType_I_2));
			arData.Add(D_CELL(nIncreaseRow+2, 8, strLcomType_J_1));
			arData.Add(D_CELL(nIncreaseRow+3, 8, strLcomType_J_2));

			// Allow Stress
			arData.Add(D_CELL(nIncreaseRow, 11, GetOutputFormat(RkmcBase_1.dPhiMn * m_dFactorMoment, 3), D_NUMBER_FORMAT_3));
			arData.Add(D_CELL(nIncreaseRow+1, 11, GetOutputFormat(RkmcBase_2.dPhiMn * m_dFactorMoment, 3), D_NUMBER_FORMAT_3));
			arData.Add(D_CELL(nIncreaseRow+2, 11, GetOutputFormat(RkmcBase_3.dPhiMn * m_dFactorMoment, 3), D_NUMBER_FORMAT_3));
			arData.Add(D_CELL(nIncreaseRow+3, 11, GetOutputFormat(RkmcBase_4.dPhiMn * m_dFactorMoment, 3), D_NUMBER_FORMAT_3));


			// Stress By DeadLoad
			arData.Add(D_CELL(nIncreaseRow, 15, GetOutputFormat(RkmcBase_1.dMd_m * m_dFactorMoment, 3), D_NUMBER_FORMAT_3));
			arData.Add(D_CELL(nIncreaseRow+1, 15, GetOutputFormat(RkmcBase_2.dMd_m * m_dFactorMoment, 3), D_NUMBER_FORMAT_3));
			arData.Add(D_CELL(nIncreaseRow+2, 15, GetOutputFormat(RkmcBase_3.dMd_m * m_dFactorMoment, 3), D_NUMBER_FORMAT_3));
			arData.Add(D_CELL(nIncreaseRow+3, 15, GetOutputFormat(RkmcBase_4.dMd_m * m_dFactorMoment, 3), D_NUMBER_FORMAT_3));

			// Stress By LiveLoad
			arData.Add(D_CELL(nIncreaseRow, 19, GetOutputFormat(RkmcBase_1.dM_L * m_dFactorMoment, 3), D_NUMBER_FORMAT_3));
			arData.Add(D_CELL(nIncreaseRow+1, 19, GetOutputFormat(RkmcBase_2.dM_L * m_dFactorMoment, 3), D_NUMBER_FORMAT_3));
			arData.Add(D_CELL(nIncreaseRow+2, 19, GetOutputFormat(RkmcBase_3.dM_L * m_dFactorMoment, 3), D_NUMBER_FORMAT_3));
			arData.Add(D_CELL(nIncreaseRow+3, 19, GetOutputFormat(RkmcBase_4.dM_L * m_dFactorMoment, 3), D_NUMBER_FORMAT_3));

			// Impact Fct plus 1
			arData.Add(D_CELL(nIncreaseRow, 23, GetOutputFormat(RkmcBase_1.d1_plus_i_calc, 3), D_NUMBER_FORMAT_3));
			arData.Add(D_CELL(nIncreaseRow+1, 23, GetOutputFormat(RkmcBase_2.d1_plus_i_calc, 3), D_NUMBER_FORMAT_3));
			arData.Add(D_CELL(nIncreaseRow+2, 23, GetOutputFormat(RkmcBase_3.d1_plus_i_calc, 3), D_NUMBER_FORMAT_3));
			arData.Add(D_CELL(nIncreaseRow+3, 23, GetOutputFormat(RkmcBase_4.d1_plus_i_calc, 3), D_NUMBER_FORMAT_3));

			// Rating Fcf.
			arData.Add(D_CELL(nIncreaseRow, 27, GetOutputFormat(RkmcBase_1.dRF, 3), D_NUMBER_FORMAT_3));
			arData.Add(D_CELL(nIncreaseRow+1, 27, GetOutputFormat(RkmcBase_2.dRF, 3), D_NUMBER_FORMAT_3));
			arData.Add(D_CELL(nIncreaseRow+2, 27, GetOutputFormat(RkmcBase_3.dRF, 3), D_NUMBER_FORMAT_3));
			arData.Add(D_CELL(nIncreaseRow+3, 27, GetOutputFormat(RkmcBase_4.dRF, 3), D_NUMBER_FORMAT_3));

			nIncreaseRow = nIncreaseRow + 4;
		}
		else
			ASSERT(0);
	}
}

void CRating_RKMC_XLOut::PrintGetRkmcRatingFctTableBody(IExcel* pExcel, int& stRow, int stCol, CArray<D_CELL, D_CELL&>& arData, int nCountSelectData)
{
	int rowCnt = 0;
	int colCnt = 0;
	int nIncreaseRow = 0;

	CString strTableBody = _T("");
	for ( int i = 0; i < nCountSelectData; i++ )
	{
		auto ElemKey = m_arRkmcKeyList[i];
		T_RKPO_D RkpoD; RkpoD.Initialize();
		if (ElemKey.second == EN_EL_BEAM)
		{
			m_pDoc->m_pAttrCtrl->GetRkpo(ElemKey.first, RkpoD);
		}
		else if (ElemKey.second == EN_EL_VBEAM)
		{
			m_pDoc->m_pAttrCtrl->GetRkpv(ElemKey.first, RkpoD);
		}
		else ASSERT(0);

		if ( i < nCountSelectData - 1 ) // End!!
		{
			if ( RkpoD.iPrintChk == 0 )      strTableBody = _T("Str_Calc_RatingFct_Body_I");  // I-End
			else if ( RkpoD.iPrintChk == 1 ) strTableBody = _T("Str_Calc_RatingFct_Body_J");  // J-End
			else if ( RkpoD.iPrintChk == 2 ) strTableBody = _T("Str_Calc_RatingFct_Body");    // I-End & J-End
			else
				ASSERT(0);

			pExcel->CopyRange(stRow + nIncreaseRow, stCol, m_strBaseBook, _T("Str_Rating"), strTableBody, rowCnt, colCnt);
			nIncreaseRow += (RkpoD.iPrintChk == 2)? 4 : 2;
		}
		else
		{
			if ( RkpoD.iPrintChk == 0 )      strTableBody = _T("Str_Calc_RatingFct_BodyEnd_I"); // I-End                         
			else if ( RkpoD.iPrintChk == 1 ) strTableBody = _T("Str_Calc_RatingFct_BodyEnd_J"); // J-End
			else if ( RkpoD.iPrintChk == 2 ) strTableBody = _T("Str_Calc_RatingFct_BodyEnd");   // I-End & J-End
			else
				ASSERT(0);

			pExcel->CopyRange(stRow + nIncreaseRow, stCol, m_strBaseBook, _T("Str_Rating"), strTableBody, rowCnt, colCnt);
			nIncreaseRow += (RkpoD.iPrintChk == 2)? 4 : 2;
		}
	}

	Print_Array_D_CELL(pExcel, arData, stRow);
	stRow += nIncreaseRow + 1;
}


void CRating_RKMC_XLOut::SetGetRkmcKsFct(CArray<T_RKMC_D, T_RKMC_D&>& arRKMCData, CArray<D_CELL, D_CELL&>& arData)
{
	arData.RemoveAll();
}

void CRating_RKMC_XLOut::PrintGetRkmcKsFct(IExcel* pExcel, int& stRow, int stCol, CArray<D_CELL, D_CELL&>& arData, int nCountSelectData)
{
	int rowCnt = 0;
	int colCnt = 0;

	CString strRange = _T("");
	if ( m_iCode == KSCE_USD05_RATING )
		strRange = _T("Str_Calc_KS_Factor");
	else
	{
		ASSERT(0);
		return;
	}

	pExcel->CopyRange(stRow, stCol, m_strBaseBook, _T("Str_Rating"), strRange, rowCnt, colCnt);
	Print_Array_D_CELL(pExcel, arData, stRow+1);
	stRow += rowCnt + 1;
}

void CRating_RKMC_XLOut::SetGetRkmcKsFctTableHead(CArray<T_RKMC_D, T_RKMC_D&>& arRKMCData, CArray<D_CELL, D_CELL&>& arData)
{
	arData.RemoveAll();
}

void CRating_RKMC_XLOut::PrintGetRkmcKsFctTableHead(IExcel* pExcel, int& stRow, int stCol, CArray<D_CELL, D_CELL&>& arData, int nCountSelectData)
{
	int rowCnt = 0;
	int colCnt = 0;
	int nIncreaseRow = 0;

	CString strTableHead = _T("");
	if ( m_iCode == KSCE_USD05_RATING )
		strTableHead = _T("Str_Calc_KS_Head");
	else
	{
		ASSERT(0);
		return;
	}

	pExcel->CopyRange(stRow, stCol, m_strBaseBook, _T("Str_Rating"), strTableHead, rowCnt, colCnt);
	Print_Array_D_CELL(pExcel, arData, stRow+1);
	stRow += rowCnt;
}

void CRating_RKMC_XLOut::SetGetRkmcKsFctTableBody(CArray<T_RKMC_D, T_RKMC_D&>& arRKMCData, CArray<D_CELL, D_CELL&>& arData)
{
	int nIncreaseRow = 0;
	arData.RemoveAll();

	int nCountSelectData = m_arRkmcKeyList.GetSize();
	if ( nCountSelectData <= 0 ) return;

	T_RKPO_K RkpoK; RkpoK = 0;
	T_RKPO_D RkpoD; RkpoD.Initialize();
	T_RKMC_D RkmcD; RkmcD.Initialize();

	T_RKMC_BASE RkmcBase_1, RkmcBase_2, RkmcBase_3, RkmcBase_4;
	RkmcBase_1.Initialize();  RkmcBase_2.Initialize();  RkmcBase_3.Initialize();  RkmcBase_4.Initialize();

	T_RKLC_K RklcKI_1, RklcKI_2, RklcKJ_1, RklcKJ_2;
	RklcKI_1 = RklcKI_2 = RklcKJ_1 = RklcKJ_2 = 0;

	T_RKLC_D RklcDI_1, RklcDI_2, RklcDJ_1, RklcDJ_2;
	RklcDI_1.Initialize();  RklcDI_1.Initialize();  RklcDJ_1.Initialize();  RklcDJ_2.Initialize();

	//(2008.04.03) Add by Unsang :: Lcom + Type
	CString strLcomType_I_1 = _T("");
	CString strLcomType_I_2 = _T("");
	CString strLcomType_J_1 = _T("");
	CString strLcomType_J_2 = _T("");

	for ( int i = 0; i < nCountSelectData; i++ )
	{
		auto RkpoK = m_arRkmcKeyList[i];
		RkpoD.Initialize();
		if (RkpoK.second == EN_EL_BEAM)
		{
			m_pDoc->m_pAttrCtrl->GetRkpo(RkpoK.first, RkpoD);
		}
		else if (RkpoK.second == EN_EL_VBEAM)
		{
			m_pDoc->m_pAttrCtrl->GetRkpv(RkpoK.first, RkpoD);
		}
		else ASSERT(0);

		RkmcD.Initialize();
		m_pDoc->m_pPostCtrl->GetAnalysisResult()->GetFlexuralStrengthRating4TableKR05(RkpoK, RkmcD);

		if ( RkpoD.iPrintChk == 0 ) // I-End
		{
			RkmcBase_1.Initialize(); RkmcBase_1 = RkmcD.RkmcBase[0];
			RkmcBase_2.Initialize(); RkmcBase_2 = RkmcD.RkmcBase[1];

			// Element No
			arData.Add(D_CELL(nIncreaseRow, 1, GetOutputFormat(RkmcBase_1.ElemK, 0)));

			// Load Combination
			RklcKI_1 = RkmcBase_1.nRCaseK;
			RklcDI_1.Initialize(); m_pDoc->m_pAttrCtrl->GetRklc(RklcKI_1, RklcDI_1);

			RklcKI_2 = RkmcBase_2.nRCaseK;
			RklcDI_2.Initialize(); m_pDoc->m_pAttrCtrl->GetRklc(RklcKI_2, RklcDI_2);

			strLcomType_I_1 = RklcDI_1.strCaseName + _T("(") + GetLcomType(RkmcBase_1.nConcurrent) + _T(")");
			strLcomType_I_2 = RklcDI_2.strCaseName + _T("(") + GetLcomType(RkmcBase_2.nConcurrent) + _T(")");

			arData.Add(D_CELL(nIncreaseRow, 7, strLcomType_I_1));
			arData.Add(D_CELL(nIncreaseRow+1, 7, strLcomType_I_2));

			// Displ.
			arData.Add(D_CELL(nIncreaseRow, 10, GetOutputFormat(RkmcBase_1.dCalc_defl * m_dFactorLengthEach, 3), D_NUMBER_FORMAT_3));  // Displ. By Calc
			arData.Add(D_CELL(nIncreaseRow+1, 10, GetOutputFormat(RkmcBase_2.dCalc_defl * m_dFactorLengthEach, 3), D_NUMBER_FORMAT_3));  // Displ. By Calc
			arData.Add(D_CELL(nIncreaseRow, 13, GetOutputFormat(RkmcBase_1.dRealDispZ * m_dFactorLengthEach, 3), D_NUMBER_FORMAT_3));  // Displ. By LoadTest
			arData.Add(D_CELL(nIncreaseRow+1, 13, GetOutputFormat(RkmcBase_2.dRealDispZ * m_dFactorLengthEach, 3), D_NUMBER_FORMAT_3));  // Displ. By LoadTest
			arData.Add(D_CELL(nIncreaseRow, 16, GetOutputFormat(RkmcBase_1.dDisp_rat, 3), D_NUMBER_FORMAT_3));  // Displ. Ratio (Calc/LoadTest)
			arData.Add(D_CELL(nIncreaseRow+1, 16, GetOutputFormat(RkmcBase_2.dDisp_rat, 3), D_NUMBER_FORMAT_3));  // Displ. Ratio (Calc/LoadTest)

			// Impact Fct.
			arData.Add(D_CELL(nIncreaseRow, 19, GetOutputFormat(RkmcBase_1.dImpFactor, 3), D_NUMBER_FORMAT_3)); // Impact Fct. By Standard
			arData.Add(D_CELL(nIncreaseRow+1, 19, GetOutputFormat(RkmcBase_2.dImpFactor, 3), D_NUMBER_FORMAT_3)); // Impact Fct. By Standard
			arData.Add(D_CELL(nIncreaseRow, 22, GetOutputFormat(RkmcBase_1.dRealImpFactor, 3), D_NUMBER_FORMAT_3)); // Impact Fct. By LoadTest
			arData.Add(D_CELL(nIncreaseRow+1, 22, GetOutputFormat(RkmcBase_2.dRealImpFactor, 3), D_NUMBER_FORMAT_3)); // Impact Fct. By LoadTest
			arData.Add(D_CELL(nIncreaseRow, 25, GetOutputFormat(RkmcBase_1.di_rat, 3), D_NUMBER_FORMAT_3)); // Impact Fct. Ratio (Standar/LoadTest)
			arData.Add(D_CELL(nIncreaseRow+1, 25, GetOutputFormat(RkmcBase_2.di_rat, 3), D_NUMBER_FORMAT_3)); // Impact Fct. Ratio (Standar/LoadTest)

			// DgnFct. Ks
			arData.Add(D_CELL(nIncreaseRow, 28, GetOutputFormat(RkmcBase_1.dKs, 3), D_NUMBER_FORMAT_3));
			arData.Add(D_CELL(nIncreaseRow+1, 28, GetOutputFormat(RkmcBase_2.dKs, 3), D_NUMBER_FORMAT_3));

			nIncreaseRow = nIncreaseRow + 2;
		}
		else if ( RkpoD.iPrintChk == 1 ) // J -End
		{
			RkmcBase_3.Initialize(); RkmcBase_3 = RkmcD.RkmcBase[2];
			RkmcBase_4.Initialize(); RkmcBase_4 = RkmcD.RkmcBase[3];
			// Element No
			arData.Add(D_CELL(nIncreaseRow, 1, GetOutputFormat(RkmcBase_3.ElemK, 0)));

			// Load Combination
			RklcKJ_1 = RkmcBase_3.nRCaseK;
			RklcDJ_1.Initialize(); m_pDoc->m_pAttrCtrl->GetRklc(RklcKJ_1, RklcDJ_1);

			RklcKJ_2 = RkmcBase_4.nRCaseK;
			RklcDJ_2.Initialize(); m_pDoc->m_pAttrCtrl->GetRklc(RklcKJ_2, RklcDJ_2);

			strLcomType_J_1 = RklcDJ_1.strCaseName + _T("(") + GetLcomType(RkmcBase_3.nConcurrent) + _T(")");
			strLcomType_J_2 = RklcDJ_2.strCaseName + _T("(") + GetLcomType(RkmcBase_4.nConcurrent) + _T(")");

			arData.Add(D_CELL(nIncreaseRow, 7, strLcomType_J_1));
			arData.Add(D_CELL(nIncreaseRow+1, 7, strLcomType_J_2));

			// Displ.
			arData.Add(D_CELL(nIncreaseRow, 10, GetOutputFormat(RkmcBase_3.dCalc_defl * m_dFactorLengthEach, 3), D_NUMBER_FORMAT_3));  // Displ. By Calc
			arData.Add(D_CELL(nIncreaseRow+1, 10, GetOutputFormat(RkmcBase_4.dCalc_defl * m_dFactorLengthEach, 3), D_NUMBER_FORMAT_3));  // Displ. By Calc
			arData.Add(D_CELL(nIncreaseRow, 13, GetOutputFormat(RkmcBase_3.dRealDispZ * m_dFactorLengthEach, 3), D_NUMBER_FORMAT_3));  // Displ. By LoadTest
			arData.Add(D_CELL(nIncreaseRow+1, 13, GetOutputFormat(RkmcBase_4.dRealDispZ * m_dFactorLengthEach, 3), D_NUMBER_FORMAT_3));  // Displ. By LoadTest
			arData.Add(D_CELL(nIncreaseRow, 16, GetOutputFormat(RkmcBase_3.dDisp_rat, 3), D_NUMBER_FORMAT_3));  // Displ. Ratio (Calc/LoadTest)
			arData.Add(D_CELL(nIncreaseRow+1, 16, GetOutputFormat(RkmcBase_4.dDisp_rat, 3), D_NUMBER_FORMAT_3));  // Displ. Ratio (Calc/LoadTest)

			// Impact Fct.
			arData.Add(D_CELL(nIncreaseRow, 19, GetOutputFormat(RkmcBase_3.dImpFactor, 3), D_NUMBER_FORMAT_3)); // Impact Fct. By Standard
			arData.Add(D_CELL(nIncreaseRow+1, 19, GetOutputFormat(RkmcBase_4.dImpFactor, 3), D_NUMBER_FORMAT_3)); // Impact Fct. By Standard
			arData.Add(D_CELL(nIncreaseRow, 22, GetOutputFormat(RkmcBase_3.dRealImpFactor, 3), D_NUMBER_FORMAT_3)); // Impact Fct. By LoadTest
			arData.Add(D_CELL(nIncreaseRow+1, 22, GetOutputFormat(RkmcBase_4.dRealImpFactor, 3), D_NUMBER_FORMAT_3)); // Impact Fct. By LoadTest
			arData.Add(D_CELL(nIncreaseRow, 25, GetOutputFormat(RkmcBase_3.di_rat, 3), D_NUMBER_FORMAT_3)); // Impact Fct. Ratio (Standar/LoadTest)
			arData.Add(D_CELL(nIncreaseRow+1, 25, GetOutputFormat(RkmcBase_4.di_rat, 3), D_NUMBER_FORMAT_3)); // Impact Fct. Ratio (Standar/LoadTest)

			// DgnFct. Ks
			arData.Add(D_CELL(nIncreaseRow, 28, GetOutputFormat(RkmcBase_3.dKs, 3), D_NUMBER_FORMAT_3));
			arData.Add(D_CELL(nIncreaseRow+1, 28, GetOutputFormat(RkmcBase_4.dKs, 3), D_NUMBER_FORMAT_3));

			nIncreaseRow = nIncreaseRow + 2;
		}
		else if ( RkpoD.iPrintChk == 2 ) // I-End & J-End
		{
			RkmcBase_1.Initialize(); RkmcBase_1 = RkmcD.RkmcBase[0];
			RkmcBase_2.Initialize(); RkmcBase_2 = RkmcD.RkmcBase[1];
			RkmcBase_3.Initialize(); RkmcBase_3 = RkmcD.RkmcBase[2];
			RkmcBase_4.Initialize(); RkmcBase_4 = RkmcD.RkmcBase[3];
			// Element No
			arData.Add(D_CELL(nIncreaseRow, 1, GetOutputFormat(RkmcBase_1.ElemK, 0)));

			// Load Combination
			RklcKI_1 = RkmcBase_1.nRCaseK;
			RklcDI_1.Initialize(); m_pDoc->m_pAttrCtrl->GetRklc(RklcKI_1, RklcDI_1);

			RklcKI_2 = RkmcBase_2.nRCaseK;
			RklcDI_2.Initialize(); m_pDoc->m_pAttrCtrl->GetRklc(RklcKI_2, RklcDI_2);

			RklcKJ_1 = RkmcBase_3.nRCaseK;
			RklcDJ_1.Initialize(); m_pDoc->m_pAttrCtrl->GetRklc(RklcKJ_1, RklcDJ_1);

			RklcKJ_2 = RkmcBase_4.nRCaseK;
			RklcDJ_2.Initialize(); m_pDoc->m_pAttrCtrl->GetRklc(RklcKJ_2, RklcDJ_2);

			strLcomType_I_1 = RklcDI_1.strCaseName + _T("(") + GetLcomType(RkmcBase_1.nConcurrent) + _T(")");
			strLcomType_I_2 = RklcDI_2.strCaseName + _T("(") + GetLcomType(RkmcBase_2.nConcurrent) + _T(")");
			strLcomType_J_1 = RklcDJ_1.strCaseName + _T("(") + GetLcomType(RkmcBase_3.nConcurrent) + _T(")");
			strLcomType_J_2 = RklcDJ_2.strCaseName + _T("(") + GetLcomType(RkmcBase_4.nConcurrent) + _T(")");

			arData.Add(D_CELL(nIncreaseRow, 7, strLcomType_I_1));
			arData.Add(D_CELL(nIncreaseRow+1, 7, strLcomType_I_2));
			arData.Add(D_CELL(nIncreaseRow+2, 7, strLcomType_J_1));
			arData.Add(D_CELL(nIncreaseRow+3, 7, strLcomType_J_2));

			// Displ.
			arData.Add(D_CELL(nIncreaseRow, 10, GetOutputFormat(RkmcBase_1.dCalc_defl * m_dFactorLengthEach, 3), D_NUMBER_FORMAT_3));  // Displ. By Calc
			arData.Add(D_CELL(nIncreaseRow+1, 10, GetOutputFormat(RkmcBase_2.dCalc_defl * m_dFactorLengthEach, 3), D_NUMBER_FORMAT_3));  // Displ. By Calc
			arData.Add(D_CELL(nIncreaseRow, 13, GetOutputFormat(RkmcBase_1.dRealDispZ * m_dFactorLengthEach, 3), D_NUMBER_FORMAT_3));  // Displ. By LoadTest
			arData.Add(D_CELL(nIncreaseRow+1, 13, GetOutputFormat(RkmcBase_2.dRealDispZ * m_dFactorLengthEach, 3), D_NUMBER_FORMAT_3));  // Displ. By LoadTest
			arData.Add(D_CELL(nIncreaseRow, 16, GetOutputFormat(RkmcBase_1.dDisp_rat, 3), D_NUMBER_FORMAT_3));  // Displ. Ratio (Calc/LoadTest)
			arData.Add(D_CELL(nIncreaseRow+1, 16, GetOutputFormat(RkmcBase_2.dDisp_rat, 3), D_NUMBER_FORMAT_3));  // Displ. Ratio (Calc/LoadTest)
			arData.Add(D_CELL(nIncreaseRow+2, 10, GetOutputFormat(RkmcBase_3.dCalc_defl * m_dFactorLengthEach, 3), D_NUMBER_FORMAT_3));  // Displ. By Calc
			arData.Add(D_CELL(nIncreaseRow+3, 10, GetOutputFormat(RkmcBase_4.dCalc_defl * m_dFactorLengthEach, 3), D_NUMBER_FORMAT_3));  // Displ. By Calc
			arData.Add(D_CELL(nIncreaseRow+2, 13, GetOutputFormat(RkmcBase_3.dRealDispZ * m_dFactorLengthEach, 3), D_NUMBER_FORMAT_3));  // Displ. By LoadTest
			arData.Add(D_CELL(nIncreaseRow+3, 13, GetOutputFormat(RkmcBase_4.dRealDispZ * m_dFactorLengthEach, 3), D_NUMBER_FORMAT_3));  // Displ. By LoadTest
			arData.Add(D_CELL(nIncreaseRow+2, 16, GetOutputFormat(RkmcBase_3.dDisp_rat, 3), D_NUMBER_FORMAT_3));  // Displ. Ratio (Calc/LoadTest)
			arData.Add(D_CELL(nIncreaseRow+3, 16, GetOutputFormat(RkmcBase_4.dDisp_rat, 3), D_NUMBER_FORMAT_3));  // Displ. Ratio (Calc/LoadTest)

			// Impact Fct.
			arData.Add(D_CELL(nIncreaseRow, 19, GetOutputFormat(RkmcBase_1.dImpFactor, 3), D_NUMBER_FORMAT_3)); // Impact Fct. By Standard
			arData.Add(D_CELL(nIncreaseRow+1, 19, GetOutputFormat(RkmcBase_2.dImpFactor, 3), D_NUMBER_FORMAT_3)); // Impact Fct. By Standard
			arData.Add(D_CELL(nIncreaseRow, 22, GetOutputFormat(RkmcBase_1.dRealImpFactor, 3), D_NUMBER_FORMAT_3)); // Impact Fct. By LoadTest
			arData.Add(D_CELL(nIncreaseRow+1, 22, GetOutputFormat(RkmcBase_2.dRealImpFactor, 3), D_NUMBER_FORMAT_3)); // Impact Fct. By LoadTest
			arData.Add(D_CELL(nIncreaseRow, 25, GetOutputFormat(RkmcBase_1.di_rat, 3), D_NUMBER_FORMAT_3)); // Impact Fct. Ratio (Standar/LoadTest)
			arData.Add(D_CELL(nIncreaseRow+1, 25, GetOutputFormat(RkmcBase_2.di_rat, 3), D_NUMBER_FORMAT_3)); // Impact Fct. Ratio (Standar/LoadTest)
			arData.Add(D_CELL(nIncreaseRow+2, 19, GetOutputFormat(RkmcBase_3.dImpFactor, 3), D_NUMBER_FORMAT_3)); // Impact Fct. By Standard
			arData.Add(D_CELL(nIncreaseRow+3, 19, GetOutputFormat(RkmcBase_4.dImpFactor, 3), D_NUMBER_FORMAT_3)); // Impact Fct. By Standard
			arData.Add(D_CELL(nIncreaseRow+2, 22, GetOutputFormat(RkmcBase_3.dRealImpFactor, 3), D_NUMBER_FORMAT_3)); // Impact Fct. By LoadTest
			arData.Add(D_CELL(nIncreaseRow+3, 22, GetOutputFormat(RkmcBase_4.dRealImpFactor, 3), D_NUMBER_FORMAT_3)); // Impact Fct. By LoadTest
			arData.Add(D_CELL(nIncreaseRow+2, 25, GetOutputFormat(RkmcBase_3.di_rat, 3), D_NUMBER_FORMAT_3)); // Impact Fct. Ratio (Standar/LoadTest)
			arData.Add(D_CELL(nIncreaseRow+3, 25, GetOutputFormat(RkmcBase_4.di_rat, 3), D_NUMBER_FORMAT_3)); // Impact Fct. Ratio (Standar/LoadTest)

			// DgnFct. Ks
			arData.Add(D_CELL(nIncreaseRow, 28, GetOutputFormat(RkmcBase_1.dKs, 3), D_NUMBER_FORMAT_3));
			arData.Add(D_CELL(nIncreaseRow+1, 28, GetOutputFormat(RkmcBase_2.dKs, 3), D_NUMBER_FORMAT_3));
			arData.Add(D_CELL(nIncreaseRow+2, 28, GetOutputFormat(RkmcBase_3.dKs, 3), D_NUMBER_FORMAT_3));
			arData.Add(D_CELL(nIncreaseRow+3, 28, GetOutputFormat(RkmcBase_4.dKs, 3), D_NUMBER_FORMAT_3));

			nIncreaseRow = nIncreaseRow + 4;
		}
		else
			ASSERT(0);
	}
}

void CRating_RKMC_XLOut::PrintGetRkmcKsFctTableBody(IExcel* pExcel, int& stRow, int stCol, CArray<D_CELL, D_CELL&>& arData, int nCountSelectData)
{
	int rowCnt = 0;
	int colCnt = 0;
	int nIncreaseRow = 0;

	CString strTableBody = _T("");
	for ( int i = 0; i < nCountSelectData; i++ )
	{
		auto ElemKey = m_arRkmcKeyList[i];
		T_RKPO_D RkpoD; RkpoD.Initialize();
		if (ElemKey.second == EN_EL_BEAM)
		{
			m_pDoc->m_pAttrCtrl->GetRkpo(ElemKey.first, RkpoD);
		}
		else if (ElemKey.second == EN_EL_VBEAM)
		{
			m_pDoc->m_pAttrCtrl->GetRkpv(ElemKey.first, RkpoD);
		}
		else ASSERT(0);

		if ( i < nCountSelectData - 1 ) // End!!
		{
			if ( RkpoD.iPrintChk == 0 )      strTableBody = _T("Str_Calc_KS_Body_I");  // I-End
			else if ( RkpoD.iPrintChk == 1 ) strTableBody = _T("Str_Calc_KS_Body_J");  // J-End
			else if ( RkpoD.iPrintChk == 2 ) strTableBody = _T("Str_Calc_KS_Body");    // I-End & J-End
			else
				ASSERT(0);

			pExcel->CopyRange(stRow + nIncreaseRow, stCol, m_strBaseBook, _T("Str_Rating"), strTableBody, rowCnt, colCnt);
			nIncreaseRow += (RkpoD.iPrintChk == 2)? 4 : 2;
		}
		else
		{
			if ( RkpoD.iPrintChk == 0 )      strTableBody = _T("Str_Calc_KS_BodyEnd_I"); // I-End                         
			else if ( RkpoD.iPrintChk == 1 ) strTableBody = _T("Str_Calc_KS_BodyEnd_J"); // J-End
			else if ( RkpoD.iPrintChk == 2 ) strTableBody = _T("Str_Calc_KS_BodyEnd");   // I-End & J-End
			else
				ASSERT(0);

			pExcel->CopyRange(stRow + nIncreaseRow, stCol, m_strBaseBook, _T("Str_Rating"), strTableBody, rowCnt, colCnt);
			nIncreaseRow += (RkpoD.iPrintChk == 2)? 4 : 2;
		}
	}

	Print_Array_D_CELL(pExcel, arData, stRow);
	stRow += nIncreaseRow + 1;
}

void CRating_RKMC_XLOut::SetGetRkmcCapacity(CArray<T_RKMC_D, T_RKMC_D&>& arRKMCData, CArray<D_CELL, D_CELL&>& arData)
{
	arData.RemoveAll();
}

void CRating_RKMC_XLOut::PrintGetRkmcCapacity(IExcel* pExcel, int& stRow, int stCol, CArray<D_CELL, D_CELL&>& arData, int nCountSelectData)
{
	int rowCnt = 0;
	int colCnt = 0;
	int nIncreaseRow = 0;

	CString strChapter = _T("");
	if ( m_iCode == KSCE_USD05_RATING )
		strChapter = _T("Str_Calc_LCC_Chapter");
	else
	{
		ASSERT(0);
		return;
	}
	pExcel->CopyRange(stRow, stCol, m_strBaseBook, _T("Str_Rating"), strChapter, rowCnt, colCnt);
	Print_Array_D_CELL(pExcel, arData, stRow+1);
	stRow += rowCnt;

	CString strComment1 = _T("");
	if ( m_iCode == KSCE_USD05_RATING )
		strComment1 = _T("Str_Calc_LCC_BaseLoad");
	else
	{
		ASSERT(0);
		return;
	}
	pExcel->CopyRange(stRow, stCol, m_strBaseBook, _T("Str_Rating"), strComment1, rowCnt, colCnt);
	Print_Array_D_CELL(pExcel, arData, stRow+1);
	stRow += rowCnt + 1;

	CString strComment2 = _T("");
	if ( m_iCode == KSCE_USD05_RATING )
		strComment2 = _T("Str_Calc_LCC_UsedLoad");
	else
	{
		ASSERT(0);
		return;
	}
	pExcel->CopyRange(stRow, stCol, m_strBaseBook, _T("Str_Rating"), strComment2, rowCnt, colCnt);
	Print_Array_D_CELL(pExcel, arData, stRow+1);
	stRow += rowCnt + 1;
}

void CRating_RKMC_XLOut::SetGetRkmcCapacityTableHead(CArray<T_RKMC_D, T_RKMC_D&>& arRKMCData, CArray<D_CELL, D_CELL&>& arData)
{
	arData.RemoveAll();
}

void CRating_RKMC_XLOut::PrintGetRkmcCapacityTableHead(IExcel* pExcel, int& stRow, int stCol, CArray<D_CELL, D_CELL&>& arData, int nCountSelectData)
{
	int rowCnt = 0;
	int colCnt = 0;
	int nIncreaseRow = 0;

	CString strTableHead = _T("");
	if ( m_iCode == KSCE_USD05_RATING )
		strTableHead = _T("Str_Calc_LCC_Head");
	else
	{
		ASSERT(0);
		return;
	}

	pExcel->CopyRange(stRow, stCol, m_strBaseBook, _T("Str_Rating"), strTableHead, rowCnt, colCnt);
	Print_Array_D_CELL(pExcel, arData, stRow+1);
	stRow += rowCnt;
}

void CRating_RKMC_XLOut::SetGetRkmcCapacityTableBody(CArray<T_RKMC_D, T_RKMC_D&>& arRKMCData, CArray<D_CELL, D_CELL&>& arData)
{
	int nIncreaseRow = 0;
	arData.RemoveAll();

	int nCountSelectData = m_arRkmcKeyList.GetSize();
	if ( nCountSelectData <= 0 ) return;

	T_RKMC_BASE RkmcBase_1, RkmcBase_2, RkmcBase_3, RkmcBase_4;
	RkmcBase_1.Initialize();  RkmcBase_2.Initialize();  RkmcBase_3.Initialize();  RkmcBase_4.Initialize();

	T_RKLC_K RklcKI_1, RklcKI_2, RklcKJ_1, RklcKJ_2;
	RklcKI_1 = RklcKI_2 = RklcKJ_1 = RklcKJ_2 = 0;

	T_RKLC_D RklcDI_1, RklcDI_2, RklcDJ_1, RklcDJ_2;
	RklcDI_1.Initialize();  RklcDI_2.Initialize();  RklcDJ_1.Initialize();  RklcDJ_2.Initialize();

	//(2008.04.03) Add by Unsang :: Lcom + Type
	CString strLcomType_I_1 = _T("");
	CString strLcomType_I_2 = _T("");
	CString strLcomType_J_1 = _T("");
	CString strLcomType_J_2 = _T("");

	for ( int i = 0; i < nCountSelectData; i++ )
	{
		auto RkpoK = m_arRkmcKeyList[i];
		T_RKPO_D RkpoD; RkpoD.Initialize();
		if (RkpoK.second == EN_EL_BEAM)
		{
			m_pDoc->m_pAttrCtrl->GetRkpo(RkpoK.first, RkpoD);
		}
		else if (RkpoK.second == EN_EL_VBEAM)
		{
			m_pDoc->m_pAttrCtrl->GetRkpv(RkpoK.first, RkpoD);
		}
		else ASSERT(0);

		T_RKMC_D RkmcD; RkmcD.Initialize();
		m_pDoc->m_pPostCtrl->GetAnalysisResult()->GetFlexuralStrengthRating4TableKR05(RkpoK, RkmcD);

		if ( RkpoD.iPrintChk == 0 ) // I-End
		{
			RkmcBase_1.Initialize(); RkmcBase_1 = RkmcD.RkmcBase[0];
			RkmcBase_2.Initialize(); RkmcBase_2 = RkmcD.RkmcBase[1];

			// Element No
			arData.Add(D_CELL(nIncreaseRow, 1, GetOutputFormat(RkmcBase_1.ElemK, 0)));

			// Load Combination
			RklcKI_1 = RkmcBase_1.nRCaseK;
			RklcDI_1.Initialize(); m_pDoc->m_pAttrCtrl->GetRklc(RklcKI_1, RklcDI_1);

			RklcKI_2 = RkmcBase_2.nRCaseK;
			RklcDI_2.Initialize(); m_pDoc->m_pAttrCtrl->GetRklc(RklcKI_2, RklcDI_2);

			strLcomType_I_1 = RklcDI_1.strCaseName + _T("(") + GetLcomType(RkmcBase_1.nConcurrent) + _T(")");
			strLcomType_I_2 = RklcDI_2.strCaseName + _T("(") + GetLcomType(RkmcBase_2.nConcurrent) + _T(")");

			arData.Add(D_CELL(nIncreaseRow, 7, strLcomType_I_1));
			arData.Add(D_CELL(nIncreaseRow+1, 7, strLcomType_I_2));

			// Rating Fct.
			arData.Add(D_CELL(nIncreaseRow, 10, GetOutputFormat(RkmcBase_1.dRF, 3), D_NUMBER_FORMAT_3));
			arData.Add(D_CELL(nIncreaseRow+1, 10, GetOutputFormat(RkmcBase_2.dRF, 3), D_NUMBER_FORMAT_3));

			// Base Capacity
			arData.Add(D_CELL(nIncreaseRow, 14, GetOutputFormat(RkmcBase_1.dP0, 3), D_NUMBER_FORMAT_3));
			arData.Add(D_CELL(nIncreaseRow+1, 14, GetOutputFormat(RkmcBase_2.dP0, 3), D_NUMBER_FORMAT_3));


			// Modify DgnFct.
			arData.Add(D_CELL(nIncreaseRow, 18, GetOutputFormat(RkmcBase_1.dKsPerKm, 3), D_NUMBER_FORMAT_3));
			arData.Add(D_CELL(nIncreaseRow+1, 18, GetOutputFormat(RkmcBase_2.dKsPerKm, 3), D_NUMBER_FORMAT_3));

			// Using Capacity
			arData.Add(D_CELL(nIncreaseRow, 22, GetOutputFormat(RkmcBase_1.dP0a, 3), D_NUMBER_FORMAT_3));
			arData.Add(D_CELL(nIncreaseRow+1, 22, GetOutputFormat(RkmcBase_2.dP0a, 3), D_NUMBER_FORMAT_3));

			// Dgn. LiveLoad
			arData.Add(D_CELL(nIncreaseRow, 26, GetOutputFormat(RkmcBase_1.dDgnLoad, 3)));

			// Comment
			arData.Add(D_CELL(nIncreaseRow, 29, (RkmcBase_1.bOK)? _T("OK") : _T("NG")));
			arData.Add(D_CELL(nIncreaseRow+1, 29, (RkmcBase_2.bOK)? _T("OK") : _T("NG")));

			nIncreaseRow = nIncreaseRow + 2;
		}
		else if ( RkpoD.iPrintChk == 1 ) // J -End
		{
			RkmcBase_3; RkmcBase_3.Initialize(); RkmcBase_3 = RkmcD.RkmcBase[2];
			RkmcBase_4; RkmcBase_4.Initialize(); RkmcBase_4 = RkmcD.RkmcBase[3];

			// Element No
			arData.Add(D_CELL(nIncreaseRow, 1, GetOutputFormat(RkmcBase_3.ElemK, 0)));

			// Load Combination
			RklcKJ_1 = RkmcBase_3.nRCaseK;
			RklcDJ_1.Initialize(); m_pDoc->m_pAttrCtrl->GetRklc(RklcKJ_1, RklcDJ_1);

			RklcKJ_2 = RkmcBase_4.nRCaseK;
			RklcDJ_2.Initialize(); m_pDoc->m_pAttrCtrl->GetRklc(RklcKJ_2, RklcDJ_2);

			strLcomType_J_1 = RklcDJ_1.strCaseName + _T("(") + GetLcomType(RkmcBase_3.nConcurrent) + _T(")");
			strLcomType_J_2 = RklcDJ_2.strCaseName + _T("(") + GetLcomType(RkmcBase_4.nConcurrent) + _T(")");

			arData.Add(D_CELL(nIncreaseRow, 7, strLcomType_J_1));
			arData.Add(D_CELL(nIncreaseRow+1, 7, strLcomType_J_2));

			// Rating Fct.
			arData.Add(D_CELL(nIncreaseRow, 10, GetOutputFormat(RkmcBase_3.dRF, 3), D_NUMBER_FORMAT_3));
			arData.Add(D_CELL(nIncreaseRow+1, 10, GetOutputFormat(RkmcBase_4.dRF, 3), D_NUMBER_FORMAT_3));

			// Base Capacity
			arData.Add(D_CELL(nIncreaseRow, 14, GetOutputFormat(RkmcBase_3.dP0, 3), D_NUMBER_FORMAT_3));
			arData.Add(D_CELL(nIncreaseRow+1, 14, GetOutputFormat(RkmcBase_4.dP0, 3), D_NUMBER_FORMAT_3));


			// Modify DgnFct.
			arData.Add(D_CELL(nIncreaseRow, 18, GetOutputFormat(RkmcBase_3.dKsPerKm, 3), D_NUMBER_FORMAT_3));
			arData.Add(D_CELL(nIncreaseRow+1, 18, GetOutputFormat(RkmcBase_4.dKsPerKm, 3), D_NUMBER_FORMAT_3));

			// Using Capacity
			arData.Add(D_CELL(nIncreaseRow, 22, GetOutputFormat(RkmcBase_3.dP0a, 3), D_NUMBER_FORMAT_3));
			arData.Add(D_CELL(nIncreaseRow+1, 22, GetOutputFormat(RkmcBase_4.dP0a, 3), D_NUMBER_FORMAT_3));

			// Dgn. LiveLoad
			arData.Add(D_CELL(nIncreaseRow, 26, GetOutputFormat(RkmcBase_3.dDgnLoad, 3)));

			// Comment
			arData.Add(D_CELL(nIncreaseRow, 29, (RkmcBase_3.bOK)? _T("OK") : _T("NG")));
			arData.Add(D_CELL(nIncreaseRow+1, 29, (RkmcBase_4.bOK)? _T("OK") : _T("NG")));

			nIncreaseRow = nIncreaseRow + 2;
		}
		else if ( RkpoD.iPrintChk == 2 ) // I-End & J-End
		{
			RkmcBase_1; RkmcBase_1.Initialize(); RkmcBase_1 = RkmcD.RkmcBase[0];
			RkmcBase_2; RkmcBase_2.Initialize(); RkmcBase_2 = RkmcD.RkmcBase[1];
			RkmcBase_3; RkmcBase_3.Initialize(); RkmcBase_3 = RkmcD.RkmcBase[2];
			RkmcBase_4; RkmcBase_4.Initialize(); RkmcBase_4 = RkmcD.RkmcBase[3];

			// Element No
			arData.Add(D_CELL(nIncreaseRow, 1, GetOutputFormat(RkmcBase_1.ElemK, 0)));

			// Load Combination
			RklcKI_1 = RkmcBase_1.nRCaseK;
			RklcDI_1.Initialize(); m_pDoc->m_pAttrCtrl->GetRklc(RklcKI_1, RklcDI_1);

			RklcKI_2 = RkmcBase_2.nRCaseK;
			RklcDI_2.Initialize(); m_pDoc->m_pAttrCtrl->GetRklc(RklcKI_2, RklcDI_2);

			RklcKJ_1 = RkmcBase_1.nRCaseK;
			RklcDJ_1.Initialize(); m_pDoc->m_pAttrCtrl->GetRklc(RklcKJ_1, RklcDJ_1);

			RklcKJ_2 = RkmcBase_2.nRCaseK;
			RklcDJ_2.Initialize(); m_pDoc->m_pAttrCtrl->GetRklc(RklcKJ_2, RklcDJ_2);

			strLcomType_I_1 = RklcDI_1.strCaseName + _T("(") + GetLcomType(RkmcBase_1.nConcurrent) + _T(")");
			strLcomType_I_2 = RklcDI_2.strCaseName + _T("(") + GetLcomType(RkmcBase_2.nConcurrent) + _T(")");
			strLcomType_J_1 = RklcDJ_1.strCaseName + _T("(") + GetLcomType(RkmcBase_3.nConcurrent) + _T(")");
			strLcomType_J_2 = RklcDJ_2.strCaseName + _T("(") + GetLcomType(RkmcBase_4.nConcurrent) + _T(")");


			arData.Add(D_CELL(nIncreaseRow, 7, strLcomType_I_1));
			arData.Add(D_CELL(nIncreaseRow+1, 7, strLcomType_I_2));
			arData.Add(D_CELL(nIncreaseRow+2, 7, strLcomType_J_1));
			arData.Add(D_CELL(nIncreaseRow+3, 7, strLcomType_J_2));

			// Rating Fct.
			arData.Add(D_CELL(nIncreaseRow, 10, GetOutputFormat(RkmcBase_1.dRF, 3), D_NUMBER_FORMAT_3));
			arData.Add(D_CELL(nIncreaseRow+1, 10, GetOutputFormat(RkmcBase_2.dRF, 3), D_NUMBER_FORMAT_3));
			arData.Add(D_CELL(nIncreaseRow+2, 10, GetOutputFormat(RkmcBase_3.dRF, 3), D_NUMBER_FORMAT_3));
			arData.Add(D_CELL(nIncreaseRow+3, 10, GetOutputFormat(RkmcBase_4.dRF, 3), D_NUMBER_FORMAT_3));

			// Base Capacity
			arData.Add(D_CELL(nIncreaseRow, 14, GetOutputFormat(RkmcBase_1.dP0, 3), D_NUMBER_FORMAT_3));
			arData.Add(D_CELL(nIncreaseRow+1, 14, GetOutputFormat(RkmcBase_2.dP0, 3), D_NUMBER_FORMAT_3));
			arData.Add(D_CELL(nIncreaseRow+2, 14, GetOutputFormat(RkmcBase_3.dP0, 3), D_NUMBER_FORMAT_3));
			arData.Add(D_CELL(nIncreaseRow+3, 14, GetOutputFormat(RkmcBase_4.dP0, 3), D_NUMBER_FORMAT_3));

			// Modify DgnFct.
			arData.Add(D_CELL(nIncreaseRow, 18, GetOutputFormat(RkmcBase_1.dKsPerKm, 3), D_NUMBER_FORMAT_3));
			arData.Add(D_CELL(nIncreaseRow+1, 18, GetOutputFormat(RkmcBase_2.dKsPerKm, 3), D_NUMBER_FORMAT_3));
			arData.Add(D_CELL(nIncreaseRow+2, 18, GetOutputFormat(RkmcBase_3.dKsPerKm, 3), D_NUMBER_FORMAT_3));
			arData.Add(D_CELL(nIncreaseRow+3, 18, GetOutputFormat(RkmcBase_4.dKsPerKm, 3), D_NUMBER_FORMAT_3));

			// Using Capacity
			arData.Add(D_CELL(nIncreaseRow, 22, GetOutputFormat(RkmcBase_1.dP0a, 3), D_NUMBER_FORMAT_3));
			arData.Add(D_CELL(nIncreaseRow+1, 22, GetOutputFormat(RkmcBase_2.dP0a, 3), D_NUMBER_FORMAT_3));
			arData.Add(D_CELL(nIncreaseRow+2, 22, GetOutputFormat(RkmcBase_3.dP0a, 3), D_NUMBER_FORMAT_3));
			arData.Add(D_CELL(nIncreaseRow+3, 22, GetOutputFormat(RkmcBase_4.dP0a, 3), D_NUMBER_FORMAT_3));

			// Dgn. LiveLoad
			arData.Add(D_CELL(nIncreaseRow, 26, GetOutputFormat(RkmcBase_1.dDgnLoad, 3)));

			// Comment
			arData.Add(D_CELL(nIncreaseRow, 29, (RkmcBase_1.bOK)? _T("OK") : _T("NG")));
			arData.Add(D_CELL(nIncreaseRow+1, 29, (RkmcBase_2.bOK)? _T("OK") : _T("NG")));
			arData.Add(D_CELL(nIncreaseRow+2, 29, (RkmcBase_3.bOK)? _T("OK") : _T("NG")));
			arData.Add(D_CELL(nIncreaseRow+3, 29, (RkmcBase_4.bOK)? _T("OK") : _T("NG")));

			nIncreaseRow = nIncreaseRow + 4;
		}
		else
			ASSERT(0);
	}
}

void CRating_RKMC_XLOut::PrintGetRkmcCapacityTableBody(IExcel* pExcel, int& stRow, int stCol, CArray<D_CELL, D_CELL&>& arData, int nCountSelectData)
{
	int rowCnt = 0;
	int colCnt = 0;
	int nIncreaseRow = 0;

	CString strTableBody = _T("");
	for ( int i = 0; i < nCountSelectData; i++ )
	{
		auto ElemKey = m_arRkmcKeyList[i];
		T_RKPO_D RkpoD; RkpoD.Initialize();
		if (ElemKey.second == EN_EL_BEAM)
		{
			m_pDoc->m_pAttrCtrl->GetRkpo(ElemKey.first, RkpoD);
		}
		else if (ElemKey.second == EN_EL_VBEAM)
		{
			m_pDoc->m_pAttrCtrl->GetRkpv(ElemKey.first, RkpoD);
		}
		else ASSERT(0);

		if ( i < nCountSelectData - 1 ) // End!!
		{
			if ( RkpoD.iPrintChk == 0 )      strTableBody = _T("Str_Calc_LCC_Body_I");  // I-End
			else if ( RkpoD.iPrintChk == 1 ) strTableBody = _T("Str_Calc_LCC_Body_J");  // J-End
			else if ( RkpoD.iPrintChk == 2 ) strTableBody = _T("Str_Calc_LCC_Body");    // I-End & J-End
			else
				ASSERT(0);

			pExcel->CopyRange(stRow + nIncreaseRow, stCol, m_strBaseBook, _T("Str_Rating"), strTableBody, rowCnt, colCnt);
			nIncreaseRow += (RkpoD.iPrintChk == 2)? 4 : 2;
		}
		else
		{
			if ( RkpoD.iPrintChk == 0 )      strTableBody = _T("Str_Calc_LCC_BodyEnd_I"); // I-End                         
			else if ( RkpoD.iPrintChk == 1 ) strTableBody = _T("Str_Calc_LCC_BodyEnd_J"); // J-End
			else if ( RkpoD.iPrintChk == 2 ) strTableBody = _T("Str_Calc_LCC_BodyEnd");   // I-End & J-End
			else
				ASSERT(0);

			pExcel->CopyRange(stRow + nIncreaseRow, stCol, m_strBaseBook, _T("Str_Rating"), strTableBody, rowCnt, colCnt);
			nIncreaseRow += (RkpoD.iPrintChk == 2)? 4 : 2;
		}
	}

	Print_Array_D_CELL(pExcel, arData, stRow);
	stRow += nIncreaseRow + 1;
}


// RKST
void CRating_RKMC_XLOut::SetGetRkstRatingFct(CArray<T_RKST_D, T_RKST_D&>& arRKSTData, CArray<D_CELL, D_CELL&>& arData)
{
	arData.RemoveAll();
	// Not Printing_Data
}

void CRating_RKMC_XLOut::PrintGetRkstRatingFct(IExcel* pExcel, int& stRow, int stCol, CArray<D_CELL, D_CELL&>& arData, int nCountSelectData)
{
	int rowCnt = 0;
	int colCnt = 0;

	CString strRange = _T("");
	if ( m_iCode == KSCE_USD05_RATING )
		strRange = _T("Ser_Calc_RatingFct");
	else
	{
		ASSERT(0);
		return;
	}

	pExcel->CopyRange(stRow, stCol, m_strBaseBook, _T("Ser_Rating"), strRange, rowCnt, colCnt);
	Print_Array_D_CELL(pExcel, arData, stRow+1);
	stRow += rowCnt + 1;
}

void CRating_RKMC_XLOut::SetGetRkstRatingFctTableHead(CArray<T_RKST_D, T_RKST_D&>& arRKSTData, CArray<D_CELL, D_CELL&>& arData)
{
	arData.RemoveAll();
	// Not Printing_Data
}

void CRating_RKMC_XLOut::PrintGetRkstRatingFctTableHead(IExcel* pExcel, int& stRow, int stCol, CArray<D_CELL, D_CELL&>& arData, int nCountSelectData)
{
	int rowCnt = 0;
	int colCnt = 0;

	CString strTableHead = _T("");
	if ( m_iCode == KSCE_USD05_RATING )
		strTableHead = _T("Ser_Calc_RatingFct_Head");
	else
	{
		ASSERT(0);
		return;
	}

	pExcel->CopyRange(stRow, stCol, m_strBaseBook, _T("Ser_Rating"), strTableHead, rowCnt, colCnt);
	Print_Array_D_CELL(pExcel, arData, stRow+1);
	stRow += rowCnt;
}

void CRating_RKMC_XLOut::SetGetRkstRatingFctTableBody(CArray<T_RKST_D, T_RKST_D&>& arRKSTData, CArray<D_CELL, D_CELL&>& arData)
{
	int nIncreaseRow = 0;
	arData.RemoveAll();

	T_RKPO_D RkpoD; RkpoD.Initialize();
	T_RKST_D RkstD; RkstD.Initialize();

	T_RKST_BASE RkstBaseI_1, RkstBaseI_2, RkstBaseI_3, RkstBaseI_4;
	T_RKST_BASE RkstBaseJ_1, RkstBaseJ_2, RkstBaseJ_3, RkstBaseJ_4;
	RkstBaseI_1.Initialize(); RkstBaseI_2.Initialize(); RkstBaseI_3.Initialize(); RkstBaseI_4.Initialize();
	RkstBaseJ_1.Initialize(); RkstBaseJ_2.Initialize(); RkstBaseJ_3.Initialize(); RkstBaseJ_4.Initialize();

	T_RKLC_K RklcKI_1, RklcKI_2, RklcKI_3, RklcKI_4;
	T_RKLC_K RklcKJ_1, RklcKJ_2, RklcKJ_3, RklcKJ_4;
	RklcKI_1 = RklcKI_2 = RklcKI_3 = RklcKI_4 = 0;
	RklcKJ_1 = RklcKJ_2 = RklcKJ_3 = RklcKJ_4 = 0;

	T_RKLC_D RklcDI_1, RklcDI_2, RklcDI_3, RklcDI_4;
	T_RKLC_D RklcDJ_1, RklcDJ_2, RklcDJ_3, RklcDJ_4;
	RklcDI_1.Initialize();  RklcDI_2.Initialize();  RklcDI_3.Initialize();  RklcDI_4.Initialize();
	RklcDJ_1.Initialize();  RklcDJ_2.Initialize();  RklcDJ_3.Initialize();  RklcDJ_4.Initialize();


	//(2008.04.03) Add by Unsang :: Lcom + Type
	CString strLcomType_I_1 = _T("");
	CString strLcomType_I_2 = _T("");
	CString strLcomType_I_3 = _T("");
	CString strLcomType_I_4 = _T("");
	CString strLcomType_J_1 = _T("");
	CString strLcomType_J_2 = _T("");
	CString strLcomType_J_3 = _T("");
	CString strLcomType_J_4 = _T("");

	int nCountSelectData = m_arRkstKeyList.GetSize();
	for ( int i = 0; i < nCountSelectData; i++ )
	{
		auto RkpoK = m_arRkstKeyList[i];
		RkpoD.Initialize();
		if (RkpoK.second == EN_EL_BEAM)
		{
			m_pDoc->m_pAttrCtrl->GetRkpo(RkpoK.first, RkpoD);
		}
		else if (RkpoK.second == EN_EL_VBEAM)
		{
			m_pDoc->m_pAttrCtrl->GetRkpv(RkpoK.first, RkpoD);
		}
		else ASSERT(0);

		RkstD.Initialize();
		m_pDoc->m_pPostCtrl->GetAnalysisResult()->GetConcreteStressRating4TableKR05(RkpoK, RkstD);

		if ( RkpoD.iPrintChk == 0 )      // I-End
		{
			RkstBaseI_1.Initialize(); RkstBaseI_1 = RkstD.BaseI[0];  // 상연-인장
			RkstBaseI_2.Initialize(); RkstBaseI_2 = RkstD.BaseI[1];  // 상연-압축
			RkstBaseI_3.Initialize(); RkstBaseI_3 = RkstD.BaseI[2];  // 하연-인장
			RkstBaseI_4.Initialize(); RkstBaseI_4 = RkstD.BaseI[3];  // 하연-압축

			// Element No
			arData.Add(D_CELL(nIncreaseRow, 1, GetOutputFormat(RkstBaseI_1.ElemK, 0))); // 공통적용 값임..

			// Critical Load Combination 
			RklcKI_1; RklcKI_1 = RkstBaseI_1.nRCaseK;
			RklcDI_1; RklcDI_1.Initialize(); m_pDoc->m_pAttrCtrl->GetRklc(RklcKI_1, RklcDI_1);

			RklcKI_2; RklcKI_2 = RkstBaseI_2.nRCaseK;
			RklcDI_2; RklcDI_2.Initialize(); m_pDoc->m_pAttrCtrl->GetRklc(RklcKI_2, RklcDI_2);

			RklcKI_3; RklcKI_3 = RkstBaseI_3.nRCaseK;
			RklcDI_3; RklcDI_3.Initialize(); m_pDoc->m_pAttrCtrl->GetRklc(RklcKI_3, RklcDI_3);

			RklcKI_4; RklcKI_4 = RkstBaseI_4.nRCaseK;
			RklcDI_4; RklcDI_4.Initialize(); m_pDoc->m_pAttrCtrl->GetRklc(RklcKI_4, RklcDI_4);

			strLcomType_I_1 = RklcDI_1.strCaseName + _T("(") + GetLcomType(RkstBaseI_1.nConcurrent) + _T(")");
			strLcomType_I_2 = RklcDI_2.strCaseName + _T("(") + GetLcomType(RkstBaseI_2.nConcurrent) + _T(")");
			strLcomType_I_3 = RklcDI_3.strCaseName + _T("(") + GetLcomType(RkstBaseI_3.nConcurrent) + _T(")");
			strLcomType_I_4 = RklcDI_4.strCaseName + _T("(") + GetLcomType(RkstBaseI_4.nConcurrent) + _T(")");


			arData.Add(D_CELL(nIncreaseRow, 8, strLcomType_I_1)); // 공통적용 값임..
			arData.Add(D_CELL(nIncreaseRow+1, 8, strLcomType_I_2)); // 공통적용 값임..
			arData.Add(D_CELL(nIncreaseRow+2, 8, strLcomType_I_3)); // 공통적용 값임..
			arData.Add(D_CELL(nIncreaseRow+3, 8, strLcomType_I_4)); // 공통적용 값임..

			// Allowable Stress
			arData.Add(D_CELL(nIncreaseRow, 11, GetOutputFormat(RkstBaseI_1.dfa_p[RkstBaseI_1.iPosiResult] * m_dFactorStress, 3), D_NUMBER_FORMAT_3));
			arData.Add(D_CELL(nIncreaseRow+1, 11, GetOutputFormat(RkstBaseI_2.dfa_p[RkstBaseI_2.iPosiResult] * m_dFactorStress, 3), D_NUMBER_FORMAT_3));
			arData.Add(D_CELL(nIncreaseRow+2, 11, GetOutputFormat(RkstBaseI_3.dfa_p[RkstBaseI_3.iPosiResult] * m_dFactorStress, 3), D_NUMBER_FORMAT_3));
			arData.Add(D_CELL(nIncreaseRow+3, 11, GetOutputFormat(RkstBaseI_4.dfa_p[RkstBaseI_4.iPosiResult] * m_dFactorStress, 3), D_NUMBER_FORMAT_3));

			// Stress by Dead Load
			arData.Add(D_CELL(nIncreaseRow, 15, GetOutputFormat(RkstBaseI_1.dfdm_p[RkstBaseI_1.iPosiResult] * m_dFactorStress, 3), D_NUMBER_FORMAT_3));
			arData.Add(D_CELL(nIncreaseRow+1, 15, GetOutputFormat(RkstBaseI_2.dfdm_p[RkstBaseI_2.iPosiResult] * m_dFactorStress, 3), D_NUMBER_FORMAT_3));
			arData.Add(D_CELL(nIncreaseRow+2, 15, GetOutputFormat(RkstBaseI_3.dfdm_p[RkstBaseI_3.iPosiResult] * m_dFactorStress, 3), D_NUMBER_FORMAT_3));
			arData.Add(D_CELL(nIncreaseRow+3, 15, GetOutputFormat(RkstBaseI_4.dfdm_p[RkstBaseI_4.iPosiResult] * m_dFactorStress, 3), D_NUMBER_FORMAT_3));

			// Stress by Live Load
			arData.Add(D_CELL(nIncreaseRow, 19, GetOutputFormat(RkstBaseI_1.dfl_p[RkstBaseI_1.iPosiResult] * m_dFactorStress, 3), D_NUMBER_FORMAT_3));
			arData.Add(D_CELL(nIncreaseRow+1, 19, GetOutputFormat(RkstBaseI_2.dfl_p[RkstBaseI_2.iPosiResult] * m_dFactorStress, 3), D_NUMBER_FORMAT_3));
			arData.Add(D_CELL(nIncreaseRow+2, 19, GetOutputFormat(RkstBaseI_3.dfl_p[RkstBaseI_3.iPosiResult] * m_dFactorStress, 3), D_NUMBER_FORMAT_3));
			arData.Add(D_CELL(nIncreaseRow+3, 19, GetOutputFormat(RkstBaseI_4.dfl_p[RkstBaseI_4.iPosiResult] * m_dFactorStress, 3), D_NUMBER_FORMAT_3));

			// Calc. Impact Fct plus 1
			arData.Add(D_CELL(nIncreaseRow, 23, GetOutputFormat(RkstBaseI_1.d1_plus_i_calc, 3), D_NUMBER_FORMAT_3)); // 공통적용값임.

			// Rating Fct.
			arData.Add(D_CELL(nIncreaseRow, 27, GetOutputFormat(RkstBaseI_1.dRF_p[RkstBaseI_1.iPosiResult], 3), D_NUMBER_FORMAT_3));
			arData.Add(D_CELL(nIncreaseRow+1, 27, GetOutputFormat(RkstBaseI_2.dRF_p[RkstBaseI_2.iPosiResult], 3), D_NUMBER_FORMAT_3));
			arData.Add(D_CELL(nIncreaseRow+2, 27, GetOutputFormat(RkstBaseI_3.dRF_p[RkstBaseI_3.iPosiResult], 3), D_NUMBER_FORMAT_3));
			arData.Add(D_CELL(nIncreaseRow+3, 27, GetOutputFormat(RkstBaseI_4.dRF_p[RkstBaseI_4.iPosiResult], 3), D_NUMBER_FORMAT_3));

			nIncreaseRow = nIncreaseRow + 4;
		}
		else if ( RkpoD.iPrintChk == 1 ) // J-End
		{
			RkstBaseJ_1.Initialize(); RkstBaseJ_1 = RkstD.BaseJ[0];  // 상연-인장
			RkstBaseJ_2.Initialize(); RkstBaseJ_2 = RkstD.BaseJ[1];  // 상연-압축
			RkstBaseJ_3.Initialize(); RkstBaseJ_3 = RkstD.BaseJ[2];  // 하연-인장
			RkstBaseJ_4.Initialize(); RkstBaseJ_4 = RkstD.BaseJ[3];  // 하연-압축

			// Element No
			arData.Add(D_CELL(nIncreaseRow, 1, GetOutputFormat(RkstBaseJ_1.ElemK, 0))); // 공통적용 값임..

			// Critical Load Combination 
			RklcKJ_1 = RkstBaseJ_1.nRCaseK;
			RklcDJ_1.Initialize(); m_pDoc->m_pAttrCtrl->GetRklc(RklcKJ_1, RklcDJ_1);

			RklcKJ_2 = RkstBaseJ_2.nRCaseK;
			RklcDJ_2.Initialize(); m_pDoc->m_pAttrCtrl->GetRklc(RklcKJ_2, RklcDJ_2);

			RklcKJ_3 = RkstBaseJ_3.nRCaseK;
			RklcDJ_3.Initialize(); m_pDoc->m_pAttrCtrl->GetRklc(RklcKJ_3, RklcDJ_3);

			RklcKJ_4 = RkstBaseJ_4.nRCaseK;
			RklcDJ_4.Initialize(); m_pDoc->m_pAttrCtrl->GetRklc(RklcKJ_4, RklcDJ_4);

			strLcomType_J_1 = RklcDJ_1.strCaseName + _T("(") + GetLcomType(RkstBaseJ_1.nConcurrent) + _T(")");
			strLcomType_J_2 = RklcDJ_2.strCaseName + _T("(") + GetLcomType(RkstBaseJ_2.nConcurrent) + _T(")");
			strLcomType_J_3 = RklcDJ_3.strCaseName + _T("(") + GetLcomType(RkstBaseJ_3.nConcurrent) + _T(")");
			strLcomType_J_4 = RklcDJ_4.strCaseName + _T("(") + GetLcomType(RkstBaseJ_4.nConcurrent) + _T(")");

			arData.Add(D_CELL(nIncreaseRow, 8, strLcomType_J_1)); // 공통적용 값임..
			arData.Add(D_CELL(nIncreaseRow+1, 8, strLcomType_J_2)); // 공통적용 값임..
			arData.Add(D_CELL(nIncreaseRow+2, 8, strLcomType_J_3)); // 공통적용 값임..
			arData.Add(D_CELL(nIncreaseRow+3, 8, strLcomType_J_4)); // 공통적용 값임..

			// Allowable Stress
			arData.Add(D_CELL(nIncreaseRow, 11, GetOutputFormat(RkstBaseJ_1.dfa_p[RkstBaseJ_1.iPosiResult] * m_dFactorStress, 3), D_NUMBER_FORMAT_3));
			arData.Add(D_CELL(nIncreaseRow+1, 11, GetOutputFormat(RkstBaseJ_2.dfa_p[RkstBaseJ_2.iPosiResult] * m_dFactorStress, 3), D_NUMBER_FORMAT_3));
			arData.Add(D_CELL(nIncreaseRow+2, 11, GetOutputFormat(RkstBaseJ_3.dfa_p[RkstBaseJ_3.iPosiResult] * m_dFactorStress, 3), D_NUMBER_FORMAT_3));
			arData.Add(D_CELL(nIncreaseRow+3, 11, GetOutputFormat(RkstBaseJ_4.dfa_p[RkstBaseJ_4.iPosiResult] * m_dFactorStress, 3), D_NUMBER_FORMAT_3));

			// Stress by Dead Load
			arData.Add(D_CELL(nIncreaseRow, 15, GetOutputFormat(RkstBaseJ_1.dfdm_p[RkstBaseJ_1.iPosiResult] * m_dFactorStress, 3), D_NUMBER_FORMAT_3));
			arData.Add(D_CELL(nIncreaseRow+1, 15, GetOutputFormat(RkstBaseJ_2.dfdm_p[RkstBaseJ_2.iPosiResult] * m_dFactorStress, 3), D_NUMBER_FORMAT_3));
			arData.Add(D_CELL(nIncreaseRow+2, 15, GetOutputFormat(RkstBaseJ_3.dfdm_p[RkstBaseJ_3.iPosiResult] * m_dFactorStress, 3), D_NUMBER_FORMAT_3));
			arData.Add(D_CELL(nIncreaseRow+3, 15, GetOutputFormat(RkstBaseJ_4.dfdm_p[RkstBaseJ_4.iPosiResult] * m_dFactorStress, 3), D_NUMBER_FORMAT_3));

			// Stress by Live Load
			arData.Add(D_CELL(nIncreaseRow, 19, GetOutputFormat(RkstBaseJ_1.dfl_p[RkstBaseJ_1.iPosiResult] * m_dFactorStress, 3), D_NUMBER_FORMAT_3));
			arData.Add(D_CELL(nIncreaseRow+1, 19, GetOutputFormat(RkstBaseJ_2.dfl_p[RkstBaseJ_2.iPosiResult] * m_dFactorStress, 3), D_NUMBER_FORMAT_3));
			arData.Add(D_CELL(nIncreaseRow+2, 19, GetOutputFormat(RkstBaseJ_3.dfl_p[RkstBaseJ_3.iPosiResult] * m_dFactorStress, 3), D_NUMBER_FORMAT_3));
			arData.Add(D_CELL(nIncreaseRow+3, 19, GetOutputFormat(RkstBaseJ_4.dfl_p[RkstBaseJ_4.iPosiResult] * m_dFactorStress, 3), D_NUMBER_FORMAT_3));

			// Calc. Impact Fct plus 1
			arData.Add(D_CELL(nIncreaseRow, 23, GetOutputFormat(RkstBaseJ_1.d1_plus_i_calc, 3), D_NUMBER_FORMAT_3)); // 공통적용값임.

			// Rating Fct.
			arData.Add(D_CELL(nIncreaseRow, 27, GetOutputFormat(RkstBaseJ_1.dRF_p[RkstBaseJ_1.iPosiResult], 3), D_NUMBER_FORMAT_3));
			arData.Add(D_CELL(nIncreaseRow+1, 27, GetOutputFormat(RkstBaseJ_2.dRF_p[RkstBaseJ_2.iPosiResult], 3), D_NUMBER_FORMAT_3));
			arData.Add(D_CELL(nIncreaseRow+2, 27, GetOutputFormat(RkstBaseJ_3.dRF_p[RkstBaseJ_3.iPosiResult], 3), D_NUMBER_FORMAT_3));
			arData.Add(D_CELL(nIncreaseRow+3, 27, GetOutputFormat(RkstBaseJ_4.dRF_p[RkstBaseJ_4.iPosiResult], 3), D_NUMBER_FORMAT_3));


			nIncreaseRow = nIncreaseRow + 4;
		}
		else if ( RkpoD.iPrintChk == 2 ) // I-End & J-End
		{
			RkstBaseI_1.Initialize(); RkstBaseI_1 = RkstD.BaseI[0];  // 상연-인장
			RkstBaseI_2.Initialize(); RkstBaseI_2 = RkstD.BaseI[1];  // 상연-압축
			RkstBaseI_3.Initialize(); RkstBaseI_3 = RkstD.BaseI[2];  // 하연-인장
			RkstBaseI_4.Initialize(); RkstBaseI_4 = RkstD.BaseI[3];  // 하연-압축
			RkstBaseJ_1.Initialize(); RkstBaseJ_1 = RkstD.BaseJ[0];  // 상연-인장
			RkstBaseJ_2.Initialize(); RkstBaseJ_2 = RkstD.BaseJ[1];  // 상연-압축
			RkstBaseJ_3.Initialize(); RkstBaseJ_3 = RkstD.BaseJ[2];  // 하연-인장
			RkstBaseJ_4.Initialize(); RkstBaseJ_4 = RkstD.BaseJ[3];  // 하연-압축

			// Element No
			arData.Add(D_CELL(nIncreaseRow, 1, GetOutputFormat(RkstBaseI_1.ElemK, 0))); // 공통적용 값임..
			//arData.Add(D_CELL(nIncreaseRow+4, 1, GetOutputFormat(RkstBaseJ_1.ElemK, 0))); // 공통적용 값임..

			// Critical Load Combination 
			RklcKI_1 = RkstBaseI_1.nRCaseK;
			RklcDI_1.Initialize(); m_pDoc->m_pAttrCtrl->GetRklc(RklcKI_1, RklcDI_1);

			RklcKI_2 = RkstBaseI_2.nRCaseK;
			RklcDI_2.Initialize(); m_pDoc->m_pAttrCtrl->GetRklc(RklcKI_2, RklcDI_2);

			RklcKI_3 = RkstBaseI_3.nRCaseK;
			RklcDI_3.Initialize(); m_pDoc->m_pAttrCtrl->GetRklc(RklcKI_3, RklcDI_3);

			RklcKI_4 = RkstBaseI_4.nRCaseK;
			RklcDI_4.Initialize(); m_pDoc->m_pAttrCtrl->GetRklc(RklcKI_4, RklcDI_4);

			RklcKJ_1 = RkstBaseJ_1.nRCaseK;
			RklcDJ_1.Initialize(); m_pDoc->m_pAttrCtrl->GetRklc(RklcKJ_1, RklcDJ_1);

			RklcKJ_2 = RkstBaseJ_2.nRCaseK;
			RklcDJ_2.Initialize(); m_pDoc->m_pAttrCtrl->GetRklc(RklcKJ_2, RklcDJ_2);

			RklcKJ_3 = RkstBaseJ_3.nRCaseK;
			RklcDJ_3.Initialize(); m_pDoc->m_pAttrCtrl->GetRklc(RklcKJ_3, RklcDJ_3);

			RklcKJ_4 = RkstBaseJ_4.nRCaseK;
			RklcDJ_4.Initialize(); m_pDoc->m_pAttrCtrl->GetRklc(RklcKJ_4, RklcDJ_4);

			strLcomType_I_1 = RklcDI_1.strCaseName + _T("(") + GetLcomType(RkstBaseI_1.nConcurrent) + _T(")");
			strLcomType_I_2 = RklcDI_2.strCaseName + _T("(") + GetLcomType(RkstBaseI_2.nConcurrent) + _T(")");
			strLcomType_I_3 = RklcDI_3.strCaseName + _T("(") + GetLcomType(RkstBaseI_3.nConcurrent) + _T(")");
			strLcomType_I_4 = RklcDI_4.strCaseName + _T("(") + GetLcomType(RkstBaseI_4.nConcurrent) + _T(")");
			strLcomType_J_1 = RklcDJ_1.strCaseName + _T("(") + GetLcomType(RkstBaseJ_1.nConcurrent) + _T(")");
			strLcomType_J_2 = RklcDJ_2.strCaseName + _T("(") + GetLcomType(RkstBaseJ_2.nConcurrent) + _T(")");
			strLcomType_J_3 = RklcDJ_3.strCaseName + _T("(") + GetLcomType(RkstBaseJ_3.nConcurrent) + _T(")");
			strLcomType_J_4 = RklcDJ_4.strCaseName + _T("(") + GetLcomType(RkstBaseJ_4.nConcurrent) + _T(")");

			arData.Add(D_CELL(nIncreaseRow, 8, strLcomType_I_1)); // 공통적용 값임..
			arData.Add(D_CELL(nIncreaseRow+1, 8, strLcomType_I_2)); // 공통적용 값임..
			arData.Add(D_CELL(nIncreaseRow+2, 8, strLcomType_I_3)); // 공통적용 값임..
			arData.Add(D_CELL(nIncreaseRow+3, 8, strLcomType_I_4)); // 공통적용 값임..
			arData.Add(D_CELL(nIncreaseRow+4, 8, strLcomType_J_1)); // 공통적용 값임..
			arData.Add(D_CELL(nIncreaseRow+5, 8, strLcomType_J_2)); // 공통적용 값임..
			arData.Add(D_CELL(nIncreaseRow+6, 8, strLcomType_J_3)); // 공통적용 값임..
			arData.Add(D_CELL(nIncreaseRow+7, 8, strLcomType_J_4)); // 공통적용 값임..

			// Allowable Stress
			arData.Add(D_CELL(nIncreaseRow, 11, GetOutputFormat(RkstBaseI_1.dfa_p[RkstBaseI_1.iPosiResult] * m_dFactorStress, 3), D_NUMBER_FORMAT_3));
			arData.Add(D_CELL(nIncreaseRow+1, 11, GetOutputFormat(RkstBaseI_2.dfa_p[RkstBaseI_2.iPosiResult] * m_dFactorStress, 3), D_NUMBER_FORMAT_3));
			arData.Add(D_CELL(nIncreaseRow+2, 11, GetOutputFormat(RkstBaseI_3.dfa_p[RkstBaseI_3.iPosiResult] * m_dFactorStress, 3), D_NUMBER_FORMAT_3));
			arData.Add(D_CELL(nIncreaseRow+3, 11, GetOutputFormat(RkstBaseI_4.dfa_p[RkstBaseI_4.iPosiResult] * m_dFactorStress, 3), D_NUMBER_FORMAT_3));
			arData.Add(D_CELL(nIncreaseRow+4, 11, GetOutputFormat(RkstBaseJ_1.dfa_p[RkstBaseJ_1.iPosiResult] * m_dFactorStress, 3), D_NUMBER_FORMAT_3));
			arData.Add(D_CELL(nIncreaseRow+5, 11, GetOutputFormat(RkstBaseJ_2.dfa_p[RkstBaseJ_2.iPosiResult] * m_dFactorStress, 3), D_NUMBER_FORMAT_3));
			arData.Add(D_CELL(nIncreaseRow+6, 11, GetOutputFormat(RkstBaseJ_3.dfa_p[RkstBaseJ_3.iPosiResult] * m_dFactorStress, 3), D_NUMBER_FORMAT_3));
			arData.Add(D_CELL(nIncreaseRow+7, 11, GetOutputFormat(RkstBaseJ_4.dfa_p[RkstBaseJ_4.iPosiResult] * m_dFactorStress, 3), D_NUMBER_FORMAT_3));

			// Stress by Dead Load
			arData.Add(D_CELL(nIncreaseRow, 15, GetOutputFormat(RkstBaseI_1.dfdm_p[RkstBaseI_1.iPosiResult] * m_dFactorStress, 3), D_NUMBER_FORMAT_3));
			arData.Add(D_CELL(nIncreaseRow+1, 15, GetOutputFormat(RkstBaseI_2.dfdm_p[RkstBaseI_2.iPosiResult] * m_dFactorStress, 3), D_NUMBER_FORMAT_3));
			arData.Add(D_CELL(nIncreaseRow+2, 15, GetOutputFormat(RkstBaseI_3.dfdm_p[RkstBaseI_3.iPosiResult] * m_dFactorStress, 3), D_NUMBER_FORMAT_3));
			arData.Add(D_CELL(nIncreaseRow+3, 15, GetOutputFormat(RkstBaseI_4.dfdm_p[RkstBaseI_4.iPosiResult] * m_dFactorStress, 3), D_NUMBER_FORMAT_3));
			arData.Add(D_CELL(nIncreaseRow+4, 15, GetOutputFormat(RkstBaseJ_1.dfdm_p[RkstBaseJ_1.iPosiResult] * m_dFactorStress, 3), D_NUMBER_FORMAT_3));
			arData.Add(D_CELL(nIncreaseRow+5, 15, GetOutputFormat(RkstBaseJ_2.dfdm_p[RkstBaseJ_2.iPosiResult] * m_dFactorStress, 3), D_NUMBER_FORMAT_3));
			arData.Add(D_CELL(nIncreaseRow+6, 15, GetOutputFormat(RkstBaseJ_3.dfdm_p[RkstBaseJ_3.iPosiResult] * m_dFactorStress, 3), D_NUMBER_FORMAT_3));
			arData.Add(D_CELL(nIncreaseRow+7, 15, GetOutputFormat(RkstBaseJ_4.dfdm_p[RkstBaseJ_4.iPosiResult] * m_dFactorStress, 3), D_NUMBER_FORMAT_3));

			// Stress by Live Load
			arData.Add(D_CELL(nIncreaseRow, 19, GetOutputFormat(RkstBaseI_1.dfl_p[RkstBaseI_1.iPosiResult] * m_dFactorStress, 3), D_NUMBER_FORMAT_3));
			arData.Add(D_CELL(nIncreaseRow+1, 19, GetOutputFormat(RkstBaseI_2.dfl_p[RkstBaseI_2.iPosiResult] * m_dFactorStress, 3), D_NUMBER_FORMAT_3));
			arData.Add(D_CELL(nIncreaseRow+2, 19, GetOutputFormat(RkstBaseI_3.dfl_p[RkstBaseI_3.iPosiResult] * m_dFactorStress, 3), D_NUMBER_FORMAT_3));
			arData.Add(D_CELL(nIncreaseRow+3, 19, GetOutputFormat(RkstBaseI_4.dfl_p[RkstBaseI_4.iPosiResult] * m_dFactorStress, 3), D_NUMBER_FORMAT_3));
			arData.Add(D_CELL(nIncreaseRow+4, 19, GetOutputFormat(RkstBaseJ_1.dfl_p[RkstBaseJ_1.iPosiResult] * m_dFactorStress, 3), D_NUMBER_FORMAT_3));
			arData.Add(D_CELL(nIncreaseRow+5, 19, GetOutputFormat(RkstBaseJ_2.dfl_p[RkstBaseJ_2.iPosiResult] * m_dFactorStress, 3), D_NUMBER_FORMAT_3));
			arData.Add(D_CELL(nIncreaseRow+6, 19, GetOutputFormat(RkstBaseJ_3.dfl_p[RkstBaseJ_3.iPosiResult] * m_dFactorStress, 3), D_NUMBER_FORMAT_3));
			arData.Add(D_CELL(nIncreaseRow+7, 19, GetOutputFormat(RkstBaseJ_4.dfl_p[RkstBaseJ_4.iPosiResult] * m_dFactorStress, 3), D_NUMBER_FORMAT_3));

			// Calc. Impact Fct plus 1
			arData.Add(D_CELL(nIncreaseRow, 23, GetOutputFormat(RkstBaseI_1.d1_plus_i_calc, 3), D_NUMBER_FORMAT_3)); // 공통적용값임.
			//arData.Add(D_CELL(nIncreaseRow+4, 24, GetOutputFormat(RkstBaseJ_1.d1_plus_i_calc, 3))); // 공통적용값임.

			// Rating Fct.
			arData.Add(D_CELL(nIncreaseRow, 27, GetOutputFormat(RkstBaseI_1.dRF_p[RkstBaseI_1.iPosiResult], 3), D_NUMBER_FORMAT_3));
			arData.Add(D_CELL(nIncreaseRow+1, 27, GetOutputFormat(RkstBaseI_2.dRF_p[RkstBaseI_2.iPosiResult], 3), D_NUMBER_FORMAT_3));
			arData.Add(D_CELL(nIncreaseRow+2, 27, GetOutputFormat(RkstBaseI_3.dRF_p[RkstBaseI_3.iPosiResult], 3), D_NUMBER_FORMAT_3));
			arData.Add(D_CELL(nIncreaseRow+3, 27, GetOutputFormat(RkstBaseI_4.dRF_p[RkstBaseI_4.iPosiResult], 3), D_NUMBER_FORMAT_3));
			arData.Add(D_CELL(nIncreaseRow+4, 27, GetOutputFormat(RkstBaseJ_1.dRF_p[RkstBaseJ_1.iPosiResult], 3), D_NUMBER_FORMAT_3));
			arData.Add(D_CELL(nIncreaseRow+5, 27, GetOutputFormat(RkstBaseJ_2.dRF_p[RkstBaseJ_2.iPosiResult], 3), D_NUMBER_FORMAT_3));
			arData.Add(D_CELL(nIncreaseRow+6, 27, GetOutputFormat(RkstBaseJ_3.dRF_p[RkstBaseJ_3.iPosiResult], 3), D_NUMBER_FORMAT_3));
			arData.Add(D_CELL(nIncreaseRow+7, 27, GetOutputFormat(RkstBaseJ_4.dRF_p[RkstBaseJ_4.iPosiResult], 3), D_NUMBER_FORMAT_3));

			nIncreaseRow = nIncreaseRow + 8;
		}
		else
		{
			ASSERT(0);
			return;
		}
	}
}

void CRating_RKMC_XLOut::PrintGetRkstRatingFctTableBody(IExcel* pExcel, int& stRow, int stCol, CArray<D_CELL, D_CELL&>& arData, int nCountSelectData)
{
	int rowCnt = 0;
	int colCnt = 0;
	int nIncreaseRow = 0;
	CString strTableBody = _T("");

	for ( int i = 0; i < nCountSelectData; i++ )
	{
		auto ElemKey = m_arRkstKeyList[i];
		T_RKPO_D RkpoD; RkpoD.Initialize();
		if (ElemKey.second == EN_EL_BEAM)
		{
			m_pDoc->m_pAttrCtrl->GetRkpo(ElemKey.first, RkpoD);
		}
		else if (ElemKey.second == EN_EL_VBEAM)
		{
			m_pDoc->m_pAttrCtrl->GetRkpv(ElemKey.first, RkpoD);
		}
		else ASSERT(0);

		if ( i < nCountSelectData - 1 ) // End!!
		{
			if ( RkpoD.iPrintChk == 0 )      strTableBody = _T("Ser_Calc_RatingFct_Body_I");  // I-End
			else if ( RkpoD.iPrintChk == 1 ) strTableBody = _T("Ser_Calc_RatingFct_Body_J");  // J-End
			else if ( RkpoD.iPrintChk == 2 ) strTableBody = _T("Ser_Calc_RatingFct_Body");    // I-End & J-End
			else
				ASSERT(0);

			pExcel->CopyRange(stRow + nIncreaseRow, stCol, m_strBaseBook, _T("Str_Rating"), strTableBody, rowCnt, colCnt);
			nIncreaseRow += (RkpoD.iPrintChk == 2)? 8 : 4;
		}
		else
		{
			if ( RkpoD.iPrintChk == 0 )      strTableBody = _T("Ser_Calc_RatingFct_BodyEnd_I"); // I-End                         
			else if ( RkpoD.iPrintChk == 1 ) strTableBody = _T("Ser_Calc_RatingFct_BodyEnd_J"); // J-End
			else if ( RkpoD.iPrintChk == 2 ) strTableBody = _T("Ser_Calc_RatingFct_BodyEnd");   // I-End & J-End
			else
				ASSERT(0);

			pExcel->CopyRange(stRow + nIncreaseRow, stCol, m_strBaseBook, _T("Str_Rating"), strTableBody, rowCnt, colCnt);
			nIncreaseRow += (RkpoD.iPrintChk == 2)? 8 : 4;
		}
	}

	Print_Array_D_CELL(pExcel, arData, stRow);
	stRow += nIncreaseRow + 1;
}

void CRating_RKMC_XLOut::SetGetRkstKsFct(CArray<T_RKST_D, T_RKST_D&>& arRKSTData, CArray<D_CELL, D_CELL&>& arData)
{
	arData.RemoveAll();
}

void CRating_RKMC_XLOut::PrintGetRkstKsFct(IExcel* pExcel, int& stRow, int stCol, CArray<D_CELL, D_CELL&>& arData, int nCountSelectData)
{
	int rowCnt = 0;
	int colCnt = 0;

	CString strRange = _T("");
	if ( m_iCode == KSCE_USD05_RATING )
		strRange = _T("Ser_Calc_KS_Factor");
	else
	{
		ASSERT(0);
		return;
	}

	pExcel->CopyRange(stRow, stCol, m_strBaseBook, _T("Ser_Rating"), strRange, rowCnt, colCnt);
	Print_Array_D_CELL(pExcel, arData, stRow+1);
	stRow += rowCnt + 1;
}

void CRating_RKMC_XLOut::SetGetRkstKsFctTableHead(CArray<T_RKST_D, T_RKST_D&>& arRKSTData, CArray<D_CELL, D_CELL&>& arData)
{
	arData.RemoveAll();
	// Not Printing_Data
}

void CRating_RKMC_XLOut::PrintGetRkstKsFctTableHead(IExcel* pExcel, int& stRow, int stCol, CArray<D_CELL, D_CELL&>& arData, int nCountSelectData)
{
	int rowCnt = 0;
	int colCnt = 0;
	int nIncreaseRow = 0;

	CString strTableHead = _T("");
	if ( m_iCode == KSCE_USD05_RATING )
		strTableHead = _T("Ser_Calc_KS_Head");
	else
	{
		ASSERT(0);
		return;
	}

	pExcel->CopyRange(stRow, stCol, m_strBaseBook, _T("Ser_Rating"), strTableHead, rowCnt, colCnt);
	Print_Array_D_CELL(pExcel, arData, stRow+1);
	stRow += rowCnt;
}

void CRating_RKMC_XLOut::SetGetRkstKsFctTableBody(CArray<T_RKST_D, T_RKST_D&>& arRKSTData, CArray<D_CELL, D_CELL&>& arData)
{
	int nIncreaseRow = 0;
	arData.RemoveAll();

	T_RKPO_D RkpoD; RkpoD.Initialize();
	T_RKST_D RkstD; RkstD.Initialize();

	T_RKST_BASE RkstBaseI_1, RkstBaseI_2, RkstBaseI_3, RkstBaseI_4;
	T_RKST_BASE RkstBaseJ_1, RkstBaseJ_2, RkstBaseJ_3, RkstBaseJ_4;
	RkstBaseI_1.Initialize(); RkstBaseI_2.Initialize(); RkstBaseI_3.Initialize(); RkstBaseI_4.Initialize();
	RkstBaseJ_1.Initialize(); RkstBaseJ_2.Initialize(); RkstBaseJ_3.Initialize(); RkstBaseJ_4.Initialize();

	T_RKLC_K RklcKI_1, RklcKI_2, RklcKI_3, RklcKI_4;
	T_RKLC_K RklcKJ_1, RklcKJ_2, RklcKJ_3, RklcKJ_4;
	RklcKI_1 = RklcKI_2 = RklcKI_3 = RklcKI_4 = 0;
	RklcKJ_1 = RklcKJ_2 = RklcKJ_3 = RklcKJ_4 = 0;

	T_RKLC_D RklcDI_1, RklcDI_2, RklcDI_3, RklcDI_4;
	T_RKLC_D RklcDJ_1, RklcDJ_2, RklcDJ_3, RklcDJ_4;
	RklcDI_1.Initialize();  RklcDI_1.Initialize();  RklcDI_1.Initialize();  RklcDI_1.Initialize();
	RklcDJ_1.Initialize();  RklcDJ_1.Initialize();  RklcDJ_1.Initialize();  RklcDJ_1.Initialize();

	//(2008.04.03) Add by Unsang :: Lcom + Type
	CString strLcomType_I_1 = _T("");
	CString strLcomType_I_2 = _T("");
	CString strLcomType_I_3 = _T("");
	CString strLcomType_I_4 = _T("");
	CString strLcomType_J_1 = _T("");
	CString strLcomType_J_2 = _T("");
	CString strLcomType_J_3 = _T("");
	CString strLcomType_J_4 = _T("");

	int nCountSelectData = m_arRkstKeyList.GetSize();
	for ( int i = 0; i < nCountSelectData; i++ )
	{
		auto RkpoK = m_arRkstKeyList[i];
		RkpoD.Initialize();
		if (RkpoK.second == EN_EL_BEAM)
		{
			m_pDoc->m_pAttrCtrl->GetRkpo(RkpoK.first, RkpoD);
		}
		else if (RkpoK.second == EN_EL_VBEAM)
		{
			m_pDoc->m_pAttrCtrl->GetRkpv(RkpoK.first, RkpoD);
		}
		else ASSERT(0);

		RkstD.Initialize();
		m_pDoc->m_pPostCtrl->GetAnalysisResult()->GetConcreteStressRating4TableKR05(RkpoK, RkstD);

		if ( RkpoD.iPrintChk == 0 )      // I-End
		{
			RkstBaseI_1; RkstBaseI_1.Initialize(); RkstBaseI_1 = RkstD.BaseI[0];  // 상연-인장
			RkstBaseI_2; RkstBaseI_2.Initialize(); RkstBaseI_2 = RkstD.BaseI[1];  // 상연-압축
			RkstBaseI_3; RkstBaseI_3.Initialize(); RkstBaseI_3 = RkstD.BaseI[2];  // 하연-인장
			RkstBaseI_4; RkstBaseI_4.Initialize(); RkstBaseI_4 = RkstD.BaseI[3];  // 하연-압축

			// Element No
			arData.Add(D_CELL(nIncreaseRow, 1, GetOutputFormat(RkstBaseI_1.ElemK, 0))); // 공통적용 값임..

			// Critical Load Combination 
			RklcKI_1 = RkstBaseI_1.nRCaseK;
			RklcDI_1.Initialize(); m_pDoc->m_pAttrCtrl->GetRklc(RklcKI_1, RklcDI_1);

			RklcKI_2 = RkstBaseI_2.nRCaseK;
			RklcDI_2.Initialize(); m_pDoc->m_pAttrCtrl->GetRklc(RklcKI_2, RklcDI_2);

			RklcKI_3 = RkstBaseI_3.nRCaseK;
			RklcDI_3.Initialize(); m_pDoc->m_pAttrCtrl->GetRklc(RklcKI_3, RklcDI_3);

			RklcKI_4 = RkstBaseI_4.nRCaseK;
			RklcDI_4.Initialize(); m_pDoc->m_pAttrCtrl->GetRklc(RklcKI_4, RklcDI_4);

			strLcomType_I_1 = RklcDI_1.strCaseName + _T("(") + GetLcomType(RkstBaseI_1.nConcurrent) + _T(")");
			strLcomType_I_2 = RklcDI_2.strCaseName + _T("(") + GetLcomType(RkstBaseI_2.nConcurrent) + _T(")");
			strLcomType_I_3 = RklcDI_3.strCaseName + _T("(") + GetLcomType(RkstBaseI_3.nConcurrent) + _T(")");
			strLcomType_I_4 = RklcDI_4.strCaseName + _T("(") + GetLcomType(RkstBaseI_4.nConcurrent) + _T(")");

			arData.Add(D_CELL(nIncreaseRow, 8, strLcomType_I_1)); // 공통적용 값임..
			arData.Add(D_CELL(nIncreaseRow+1, 8, strLcomType_I_2)); // 공통적용 값임..
			arData.Add(D_CELL(nIncreaseRow+2, 8, strLcomType_I_3)); // 공통적용 값임..
			arData.Add(D_CELL(nIncreaseRow+3, 8, strLcomType_I_4)); // 공통적용 값임..

			// Displ.
			arData.Add(D_CELL(nIncreaseRow, 11, GetOutputFormat(RkstBaseI_1.dCalc_defl * m_dFactorLengthEach, 3), D_NUMBER_FORMAT_3)); // Displ. By Calc.
			arData.Add(D_CELL(nIncreaseRow+1, 11, GetOutputFormat(RkstBaseI_2.dCalc_defl * m_dFactorLengthEach, 3), D_NUMBER_FORMAT_3)); // Displ. By Calc.
			arData.Add(D_CELL(nIncreaseRow+2, 11, GetOutputFormat(RkstBaseI_3.dCalc_defl * m_dFactorLengthEach, 3), D_NUMBER_FORMAT_3)); // Displ. By Calc.
			arData.Add(D_CELL(nIncreaseRow+3, 11, GetOutputFormat(RkstBaseI_4.dCalc_defl * m_dFactorLengthEach, 3), D_NUMBER_FORMAT_3)); // Displ. By Calc.
			arData.Add(D_CELL(nIncreaseRow, 14, GetOutputFormat(RkstBaseI_1.dRealDispZ * m_dFactorLengthEach, 3), D_NUMBER_FORMAT_3)); // Displ. By LoadTest
			arData.Add(D_CELL(nIncreaseRow+1, 14, GetOutputFormat(RkstBaseI_2.dRealDispZ * m_dFactorLengthEach, 3), D_NUMBER_FORMAT_3)); // Displ. By LoadTest
			arData.Add(D_CELL(nIncreaseRow+2, 14, GetOutputFormat(RkstBaseI_3.dRealDispZ * m_dFactorLengthEach, 3), D_NUMBER_FORMAT_3)); // Displ. By LoadTest
			arData.Add(D_CELL(nIncreaseRow+3, 14, GetOutputFormat(RkstBaseI_4.dRealDispZ * m_dFactorLengthEach, 3), D_NUMBER_FORMAT_3)); // Displ. By LoadTest
			arData.Add(D_CELL(nIncreaseRow, 17, GetOutputFormat(RkstBaseI_1.dDisp_rat, 3), D_NUMBER_FORMAT_3)); // Displ. Ratio (Calc/LoadTest)
			arData.Add(D_CELL(nIncreaseRow+1, 17, GetOutputFormat(RkstBaseI_2.dDisp_rat, 3), D_NUMBER_FORMAT_3)); // Displ. Ratio (Calc/LoadTest)
			arData.Add(D_CELL(nIncreaseRow+2, 17, GetOutputFormat(RkstBaseI_3.dDisp_rat, 3), D_NUMBER_FORMAT_3)); // Displ. Ratio (Calc/LoadTest)
			arData.Add(D_CELL(nIncreaseRow+3, 17, GetOutputFormat(RkstBaseI_4.dDisp_rat, 3), D_NUMBER_FORMAT_3)); // Displ. Ratio (Calc/LoadTest)

			// Impact Fct.
			arData.Add(D_CELL(nIncreaseRow, 20, GetOutputFormat(RkstBaseI_1.dImpFactor, 3), D_NUMBER_FORMAT_3)); // Impact Fct. By Standard
			arData.Add(D_CELL(nIncreaseRow, 23, GetOutputFormat(RkstBaseI_1.dRealImpFactor, 3), D_NUMBER_FORMAT_3)); // Impact Fct. By LoadTest
			arData.Add(D_CELL(nIncreaseRow, 26, GetOutputFormat(RkstBaseI_1.di_rat, 3), D_NUMBER_FORMAT_3)); // Impact Fct. Ratio (Standard/LoadTest)

			// Get Ks
			arData.Add(D_CELL(nIncreaseRow, 29, GetOutputFormat(RkstBaseI_1.dKs, 3), D_NUMBER_FORMAT_3));  // Fct. Ks
			arData.Add(D_CELL(nIncreaseRow+1, 29, GetOutputFormat(RkstBaseI_2.dKs, 3), D_NUMBER_FORMAT_3));  // Fct. Ks
			arData.Add(D_CELL(nIncreaseRow+2, 29, GetOutputFormat(RkstBaseI_3.dKs, 3), D_NUMBER_FORMAT_3));  // Fct. Ks
			arData.Add(D_CELL(nIncreaseRow+3, 29, GetOutputFormat(RkstBaseI_4.dKs, 3), D_NUMBER_FORMAT_3));  // Fct. Ks

			nIncreaseRow = nIncreaseRow + 4;
		}
		else if ( RkpoD.iPrintChk == 1 ) // J-End
		{
			RkstBaseJ_1; RkstBaseJ_1.Initialize(); RkstBaseJ_1 = RkstD.BaseJ[0];  // 상연-인장
			RkstBaseJ_2; RkstBaseJ_2.Initialize(); RkstBaseJ_2 = RkstD.BaseJ[1];  // 상연-압축
			RkstBaseJ_3; RkstBaseJ_3.Initialize(); RkstBaseJ_3 = RkstD.BaseJ[2];  // 하연-인장
			RkstBaseJ_4; RkstBaseJ_4.Initialize(); RkstBaseJ_4 = RkstD.BaseJ[3];  // 하연-압축

			// Element No
			arData.Add(D_CELL(nIncreaseRow, 1, GetOutputFormat(RkstBaseJ_1.ElemK, 0))); // 공통적용 값임..

			// Critical Load Combination 
			RklcKJ_1 = RkstBaseJ_1.nRCaseK;
			RklcDJ_1.Initialize(); m_pDoc->m_pAttrCtrl->GetRklc(RklcKJ_1, RklcDJ_1);

			RklcKJ_2 = RkstBaseJ_2.nRCaseK;
			RklcDJ_2.Initialize(); m_pDoc->m_pAttrCtrl->GetRklc(RklcKJ_2, RklcDJ_2);

			RklcKJ_3 = RkstBaseJ_3.nRCaseK;
			RklcDJ_3.Initialize(); m_pDoc->m_pAttrCtrl->GetRklc(RklcKJ_3, RklcDJ_3);

			RklcKJ_4 = RkstBaseJ_4.nRCaseK;
			RklcDJ_4.Initialize(); m_pDoc->m_pAttrCtrl->GetRklc(RklcKJ_4, RklcDJ_4);

			strLcomType_J_1 = RklcDJ_1.strCaseName + _T("(") + GetLcomType(RkstBaseJ_1.nConcurrent) + _T(")");
			strLcomType_J_2 = RklcDJ_2.strCaseName + _T("(") + GetLcomType(RkstBaseJ_2.nConcurrent) + _T(")");
			strLcomType_J_3 = RklcDJ_3.strCaseName + _T("(") + GetLcomType(RkstBaseJ_3.nConcurrent) + _T(")");
			strLcomType_J_4 = RklcDJ_4.strCaseName + _T("(") + GetLcomType(RkstBaseJ_4.nConcurrent) + _T(")");

			arData.Add(D_CELL(nIncreaseRow, 8, strLcomType_J_1)); // 공통적용 값임..
			arData.Add(D_CELL(nIncreaseRow+1, 8, strLcomType_J_2)); // 공통적용 값임..
			arData.Add(D_CELL(nIncreaseRow+2, 8, strLcomType_J_3)); // 공통적용 값임..
			arData.Add(D_CELL(nIncreaseRow+3, 8, strLcomType_J_4)); // 공통적용 값임..

			// Displ.
			arData.Add(D_CELL(nIncreaseRow, 11, GetOutputFormat(RkstBaseJ_1.dCalc_defl * m_dFactorLengthEach, 3), D_NUMBER_FORMAT_3)); // Displ. By Calc.
			arData.Add(D_CELL(nIncreaseRow+1, 11, GetOutputFormat(RkstBaseJ_2.dCalc_defl * m_dFactorLengthEach, 3), D_NUMBER_FORMAT_3)); // Displ. By Calc.
			arData.Add(D_CELL(nIncreaseRow+2, 11, GetOutputFormat(RkstBaseJ_3.dCalc_defl * m_dFactorLengthEach, 3), D_NUMBER_FORMAT_3)); // Displ. By Calc.
			arData.Add(D_CELL(nIncreaseRow+3, 11, GetOutputFormat(RkstBaseJ_4.dCalc_defl * m_dFactorLengthEach, 3), D_NUMBER_FORMAT_3)); // Displ. By Calc.
			arData.Add(D_CELL(nIncreaseRow, 14, GetOutputFormat(RkstBaseJ_1.dRealDispZ * m_dFactorLengthEach, 3), D_NUMBER_FORMAT_3)); // Displ. By LoadTest
			arData.Add(D_CELL(nIncreaseRow+1, 14, GetOutputFormat(RkstBaseJ_2.dRealDispZ * m_dFactorLengthEach, 3), D_NUMBER_FORMAT_3)); // Displ. By LoadTest
			arData.Add(D_CELL(nIncreaseRow+2, 14, GetOutputFormat(RkstBaseJ_3.dRealDispZ * m_dFactorLengthEach, 3), D_NUMBER_FORMAT_3)); // Displ. By LoadTest
			arData.Add(D_CELL(nIncreaseRow+3, 14, GetOutputFormat(RkstBaseJ_4.dRealDispZ * m_dFactorLengthEach, 3), D_NUMBER_FORMAT_3)); // Displ. By LoadTest
			arData.Add(D_CELL(nIncreaseRow, 17, GetOutputFormat(RkstBaseJ_1.dDisp_rat, 3), D_NUMBER_FORMAT_3)); // Displ. Ratio (Calc/LoadTest)
			arData.Add(D_CELL(nIncreaseRow+1, 17, GetOutputFormat(RkstBaseJ_2.dDisp_rat, 3), D_NUMBER_FORMAT_3)); // Displ. Ratio (Calc/LoadTest)
			arData.Add(D_CELL(nIncreaseRow+2, 17, GetOutputFormat(RkstBaseJ_3.dDisp_rat, 3), D_NUMBER_FORMAT_3)); // Displ. Ratio (Calc/LoadTest)
			arData.Add(D_CELL(nIncreaseRow+3, 17, GetOutputFormat(RkstBaseJ_4.dDisp_rat, 3), D_NUMBER_FORMAT_3)); // Displ. Ratio (Calc/LoadTest)

			// Impact Fct.
			arData.Add(D_CELL(nIncreaseRow, 20, GetOutputFormat(RkstBaseJ_1.dImpFactor, 3), D_NUMBER_FORMAT_3)); // Impact Fct. By Standard
			arData.Add(D_CELL(nIncreaseRow, 23, GetOutputFormat(RkstBaseJ_1.dRealImpFactor, 3), D_NUMBER_FORMAT_3)); // Impact Fct. By LoadTest
			arData.Add(D_CELL(nIncreaseRow, 26, GetOutputFormat(RkstBaseJ_1.di_rat, 3), D_NUMBER_FORMAT_3)); // Impact Fct. Ratio (Standard/LoadTest)

			// Get Ks
			arData.Add(D_CELL(nIncreaseRow, 29, GetOutputFormat(RkstBaseJ_1.dKs, 3), D_NUMBER_FORMAT_3));  // Fct. Ks
			arData.Add(D_CELL(nIncreaseRow+1, 29, GetOutputFormat(RkstBaseJ_2.dKs, 3), D_NUMBER_FORMAT_3));  // Fct. Ks
			arData.Add(D_CELL(nIncreaseRow+2, 29, GetOutputFormat(RkstBaseJ_3.dKs, 3), D_NUMBER_FORMAT_3));  // Fct. Ks
			arData.Add(D_CELL(nIncreaseRow+3, 29, GetOutputFormat(RkstBaseJ_4.dKs, 3), D_NUMBER_FORMAT_3));  // Fct. Ks

			nIncreaseRow = nIncreaseRow + 4;
		}
		else if ( RkpoD.iPrintChk == 2 ) // I-End & J-End
		{
			RkstBaseI_1; RkstBaseI_1.Initialize(); RkstBaseI_1 = RkstD.BaseI[0];  // 상연-인장
			RkstBaseI_2; RkstBaseI_2.Initialize(); RkstBaseI_2 = RkstD.BaseI[1];  // 상연-압축
			RkstBaseI_3; RkstBaseI_3.Initialize(); RkstBaseI_3 = RkstD.BaseI[2];  // 하연-인장
			RkstBaseI_4; RkstBaseI_4.Initialize(); RkstBaseI_4 = RkstD.BaseI[3];  // 하연-압축
			RkstBaseJ_1; RkstBaseJ_1.Initialize(); RkstBaseJ_1 = RkstD.BaseJ[0];  // 상연-인장
			RkstBaseJ_2; RkstBaseJ_2.Initialize(); RkstBaseJ_2 = RkstD.BaseJ[1];  // 상연-압축
			RkstBaseJ_3; RkstBaseJ_3.Initialize(); RkstBaseJ_3 = RkstD.BaseJ[2];  // 하연-인장
			RkstBaseJ_4; RkstBaseJ_4.Initialize(); RkstBaseJ_4 = RkstD.BaseJ[3];  // 하연-압축

			// Element No
			arData.Add(D_CELL(nIncreaseRow, 1, GetOutputFormat(RkstBaseI_1.ElemK, 0))); // 공통적용 값임..
			arData.Add(D_CELL(nIncreaseRow+4, 1, GetOutputFormat(RkstBaseJ_1.ElemK, 0))); // 공통적용 값임..

			// Critical Load Combination 
			RklcKI_1 = RkstBaseI_1.nRCaseK;
			RklcDI_1.Initialize(); m_pDoc->m_pAttrCtrl->GetRklc(RklcKI_1, RklcDI_1);

			RklcKI_2 = RkstBaseI_2.nRCaseK;
			RklcDI_2.Initialize(); m_pDoc->m_pAttrCtrl->GetRklc(RklcKI_2, RklcDI_2);

			RklcKI_3 = RkstBaseI_3.nRCaseK;
			RklcDI_3.Initialize(); m_pDoc->m_pAttrCtrl->GetRklc(RklcKI_3, RklcDI_3);

			RklcKI_4 = RkstBaseI_4.nRCaseK;
			RklcDI_4.Initialize(); m_pDoc->m_pAttrCtrl->GetRklc(RklcKI_4, RklcDI_4);

			RklcKJ_1 = RkstBaseJ_1.nRCaseK;
			RklcDJ_1.Initialize(); m_pDoc->m_pAttrCtrl->GetRklc(RklcKJ_1, RklcDJ_1);

			RklcKJ_2 = RkstBaseJ_2.nRCaseK;
			RklcDJ_2.Initialize(); m_pDoc->m_pAttrCtrl->GetRklc(RklcKJ_2, RklcDJ_2);

			RklcKJ_3 = RkstBaseJ_3.nRCaseK;
			RklcDJ_3.Initialize(); m_pDoc->m_pAttrCtrl->GetRklc(RklcKJ_3, RklcDJ_3);

			RklcKJ_4 = RkstBaseJ_4.nRCaseK;
			RklcDJ_4.Initialize(); m_pDoc->m_pAttrCtrl->GetRklc(RklcKJ_4, RklcDJ_4);

			strLcomType_I_1 = RklcDI_1.strCaseName + _T("(") + GetLcomType(RkstBaseI_1.nConcurrent) + _T(")");
			strLcomType_I_2 = RklcDI_2.strCaseName + _T("(") + GetLcomType(RkstBaseI_2.nConcurrent) + _T(")");
			strLcomType_I_3 = RklcDI_3.strCaseName + _T("(") + GetLcomType(RkstBaseI_3.nConcurrent) + _T(")");
			strLcomType_I_4 = RklcDI_4.strCaseName + _T("(") + GetLcomType(RkstBaseI_4.nConcurrent) + _T(")");
			strLcomType_J_1 = RklcDJ_1.strCaseName + _T("(") + GetLcomType(RkstBaseJ_1.nConcurrent) + _T(")");
			strLcomType_J_2 = RklcDJ_2.strCaseName + _T("(") + GetLcomType(RkstBaseJ_2.nConcurrent) + _T(")");
			strLcomType_J_3 = RklcDJ_3.strCaseName + _T("(") + GetLcomType(RkstBaseJ_3.nConcurrent) + _T(")");
			strLcomType_J_4 = RklcDJ_4.strCaseName + _T("(") + GetLcomType(RkstBaseJ_4.nConcurrent) + _T(")");

			arData.Add(D_CELL(nIncreaseRow, 8, strLcomType_I_1)); // 공통적용 값임..
			arData.Add(D_CELL(nIncreaseRow+1, 8, strLcomType_I_2)); // 공통적용 값임..
			arData.Add(D_CELL(nIncreaseRow+2, 8, strLcomType_I_3)); // 공통적용 값임..
			arData.Add(D_CELL(nIncreaseRow+3, 8, strLcomType_I_4)); // 공통적용 값임..
			arData.Add(D_CELL(nIncreaseRow+4, 8, strLcomType_J_1)); // 공통적용 값임..
			arData.Add(D_CELL(nIncreaseRow+5, 8, strLcomType_J_2)); // 공통적용 값임..
			arData.Add(D_CELL(nIncreaseRow+6, 8, strLcomType_J_3)); // 공통적용 값임..
			arData.Add(D_CELL(nIncreaseRow+7, 8, strLcomType_J_4)); // 공통적용 값임..

			// Displ.
			arData.Add(D_CELL(nIncreaseRow, 11, GetOutputFormat(RkstBaseI_1.dCalc_defl * m_dFactorLengthEach, 3), D_NUMBER_FORMAT_3)); // Displ. By Calc.
			arData.Add(D_CELL(nIncreaseRow+1, 11, GetOutputFormat(RkstBaseI_2.dCalc_defl * m_dFactorLengthEach, 3), D_NUMBER_FORMAT_3)); // Displ. By Calc.
			arData.Add(D_CELL(nIncreaseRow+2, 11, GetOutputFormat(RkstBaseI_3.dCalc_defl * m_dFactorLengthEach, 3), D_NUMBER_FORMAT_3)); // Displ. By Calc.
			arData.Add(D_CELL(nIncreaseRow+3, 11, GetOutputFormat(RkstBaseI_4.dCalc_defl * m_dFactorLengthEach, 3), D_NUMBER_FORMAT_3)); // Displ. By Calc.
			arData.Add(D_CELL(nIncreaseRow, 14, GetOutputFormat(RkstBaseI_1.dRealDispZ * m_dFactorLengthEach, 3), D_NUMBER_FORMAT_3)); // Displ. By LoadTest
			arData.Add(D_CELL(nIncreaseRow+1, 14, GetOutputFormat(RkstBaseI_2.dRealDispZ * m_dFactorLengthEach, 3), D_NUMBER_FORMAT_3)); // Displ. By LoadTest
			arData.Add(D_CELL(nIncreaseRow+2, 14, GetOutputFormat(RkstBaseI_3.dRealDispZ * m_dFactorLengthEach, 3), D_NUMBER_FORMAT_3)); // Displ. By LoadTest
			arData.Add(D_CELL(nIncreaseRow+3, 14, GetOutputFormat(RkstBaseI_4.dRealDispZ * m_dFactorLengthEach, 3), D_NUMBER_FORMAT_3)); // Displ. By LoadTest
			arData.Add(D_CELL(nIncreaseRow, 17, GetOutputFormat(RkstBaseI_1.dDisp_rat, 3), D_NUMBER_FORMAT_3)); // Displ. Ratio (Calc/LoadTest)
			arData.Add(D_CELL(nIncreaseRow+1, 17, GetOutputFormat(RkstBaseI_2.dDisp_rat, 3), D_NUMBER_FORMAT_3)); // Displ. Ratio (Calc/LoadTest)
			arData.Add(D_CELL(nIncreaseRow+2, 17, GetOutputFormat(RkstBaseI_3.dDisp_rat, 3), D_NUMBER_FORMAT_3)); // Displ. Ratio (Calc/LoadTest)
			arData.Add(D_CELL(nIncreaseRow+3, 17, GetOutputFormat(RkstBaseI_4.dDisp_rat, 3), D_NUMBER_FORMAT_3)); // Displ. Ratio (Calc/LoadTest)
			arData.Add(D_CELL(nIncreaseRow+4, 11, GetOutputFormat(RkstBaseJ_1.dCalc_defl * m_dFactorLengthEach, 3), D_NUMBER_FORMAT_3)); // Displ. By Calc.
			arData.Add(D_CELL(nIncreaseRow+5, 11, GetOutputFormat(RkstBaseJ_2.dCalc_defl * m_dFactorLengthEach, 3), D_NUMBER_FORMAT_3)); // Displ. By Calc.
			arData.Add(D_CELL(nIncreaseRow+6, 11, GetOutputFormat(RkstBaseJ_3.dCalc_defl * m_dFactorLengthEach, 3), D_NUMBER_FORMAT_3)); // Displ. By Calc.
			arData.Add(D_CELL(nIncreaseRow+7, 11, GetOutputFormat(RkstBaseJ_4.dCalc_defl * m_dFactorLengthEach, 3), D_NUMBER_FORMAT_3)); // Displ. By Calc.
			arData.Add(D_CELL(nIncreaseRow+4, 14, GetOutputFormat(RkstBaseJ_1.dRealDispZ * m_dFactorLengthEach, 3), D_NUMBER_FORMAT_3)); // Displ. By LoadTest
			arData.Add(D_CELL(nIncreaseRow+5, 14, GetOutputFormat(RkstBaseJ_2.dRealDispZ * m_dFactorLengthEach, 3), D_NUMBER_FORMAT_3)); // Displ. By LoadTest
			arData.Add(D_CELL(nIncreaseRow+6, 14, GetOutputFormat(RkstBaseJ_3.dRealDispZ * m_dFactorLengthEach, 3), D_NUMBER_FORMAT_3)); // Displ. By LoadTest
			arData.Add(D_CELL(nIncreaseRow+7, 14, GetOutputFormat(RkstBaseJ_4.dRealDispZ * m_dFactorLengthEach, 3), D_NUMBER_FORMAT_3)); // Displ. By LoadTest
			arData.Add(D_CELL(nIncreaseRow+4, 17, GetOutputFormat(RkstBaseJ_1.dDisp_rat, 3), D_NUMBER_FORMAT_3)); // Displ. Ratio (Calc/LoadTest)
			arData.Add(D_CELL(nIncreaseRow+5, 17, GetOutputFormat(RkstBaseJ_2.dDisp_rat, 3), D_NUMBER_FORMAT_3)); // Displ. Ratio (Calc/LoadTest)
			arData.Add(D_CELL(nIncreaseRow+6, 17, GetOutputFormat(RkstBaseJ_3.dDisp_rat, 3), D_NUMBER_FORMAT_3)); // Displ. Ratio (Calc/LoadTest)
			arData.Add(D_CELL(nIncreaseRow+7, 17, GetOutputFormat(RkstBaseJ_4.dDisp_rat, 3), D_NUMBER_FORMAT_3)); // Displ. Ratio (Calc/LoadTest)


			// Impact Fct.
			arData.Add(D_CELL(nIncreaseRow, 20, GetOutputFormat(RkstBaseI_1.dImpFactor, 3), D_NUMBER_FORMAT_3)); // Impact Fct. By Standard
			arData.Add(D_CELL(nIncreaseRow, 23, GetOutputFormat(RkstBaseI_1.dRealImpFactor, 3), D_NUMBER_FORMAT_3)); // Impact Fct. By LoadTest
			arData.Add(D_CELL(nIncreaseRow, 26, GetOutputFormat(RkstBaseI_1.di_rat, 3), D_NUMBER_FORMAT_3)); // Impact Fct. Ratio (Standard/LoadTest)
			arData.Add(D_CELL(nIncreaseRow+4, 20, GetOutputFormat(RkstBaseJ_1.dImpFactor, 3), D_NUMBER_FORMAT_3)); // Impact Fct. By Standard
			arData.Add(D_CELL(nIncreaseRow+4, 23, GetOutputFormat(RkstBaseJ_1.dRealImpFactor, 3), D_NUMBER_FORMAT_3)); // Impact Fct. By LoadTest
			arData.Add(D_CELL(nIncreaseRow+4, 26, GetOutputFormat(RkstBaseJ_1.di_rat, 3), D_NUMBER_FORMAT_3)); // Impact Fct. Ratio (Standard/LoadTest)

			// Get Ks
			arData.Add(D_CELL(nIncreaseRow, 29, GetOutputFormat(RkstBaseI_1.dKs, 3), D_NUMBER_FORMAT_3));  // Fct. Ks
			arData.Add(D_CELL(nIncreaseRow+1, 29, GetOutputFormat(RkstBaseI_2.dKs, 3), D_NUMBER_FORMAT_3));  // Fct. Ks
			arData.Add(D_CELL(nIncreaseRow+2, 29, GetOutputFormat(RkstBaseI_3.dKs, 3), D_NUMBER_FORMAT_3));  // Fct. Ks
			arData.Add(D_CELL(nIncreaseRow+3, 29, GetOutputFormat(RkstBaseI_4.dKs, 3), D_NUMBER_FORMAT_3));  // Fct. Ks
			arData.Add(D_CELL(nIncreaseRow+4, 29, GetOutputFormat(RkstBaseJ_1.dKs, 3), D_NUMBER_FORMAT_3));  // Fct. Ks
			arData.Add(D_CELL(nIncreaseRow+5, 29, GetOutputFormat(RkstBaseJ_2.dKs, 3), D_NUMBER_FORMAT_3));  // Fct. Ks
			arData.Add(D_CELL(nIncreaseRow+6, 29, GetOutputFormat(RkstBaseJ_3.dKs, 3), D_NUMBER_FORMAT_3));  // Fct. Ks
			arData.Add(D_CELL(nIncreaseRow+7, 29, GetOutputFormat(RkstBaseJ_4.dKs, 3), D_NUMBER_FORMAT_3));  // Fct. Ks

			nIncreaseRow = nIncreaseRow + 8;
		}
		else
		{
			ASSERT(0);
			return;
		}
	}

}

void CRating_RKMC_XLOut::PrintGetRkstKsFctTableBody(IExcel* pExcel, int& stRow, int stCol, CArray<D_CELL, D_CELL&>& arData, int nCountSelectData)
{
	int rowCnt = 0;
	int colCnt = 0;
	int nIncreaseRow = 0;

	CString strTableBody = _T("");
	for ( int i = 0; i < nCountSelectData; i++ )
	{
		auto ElemKey = m_arRkstKeyList[i];
		T_RKPO_D RkpoD; RkpoD.Initialize();
		if (ElemKey.second == EN_EL_BEAM)
		{
			m_pDoc->m_pAttrCtrl->GetRkpo(ElemKey.first, RkpoD);
		}
		else if (ElemKey.second == EN_EL_VBEAM)
		{
			m_pDoc->m_pAttrCtrl->GetRkpv(ElemKey.first, RkpoD);
		}
		else ASSERT(0);

		if ( i < nCountSelectData - 1 ) // End!!
		{
			if ( RkpoD.iPrintChk == 0 )      strTableBody = _T("Ser_Calc_KS_Body_I");  // I-End
			else if ( RkpoD.iPrintChk == 1 ) strTableBody = _T("Ser_Calc_KS_Body_J");  // J-End
			else if ( RkpoD.iPrintChk == 2 ) strTableBody = _T("Ser_Calc_KS_Body");    // I-End & J-End
			else
				ASSERT(0);

			pExcel->CopyRange(stRow + nIncreaseRow, stCol, m_strBaseBook, _T("Str_Rating"), strTableBody, rowCnt, colCnt);
			nIncreaseRow += (RkpoD.iPrintChk == 2)? 8 : 4;
		}
		else
		{
			if ( RkpoD.iPrintChk == 0 )      strTableBody = _T("Ser_Calc_KS_BodyEnd_I"); // I-End                         
			else if ( RkpoD.iPrintChk == 1 ) strTableBody = _T("Ser_Calc_KS_BodyEnd_J"); // J-End
			else if ( RkpoD.iPrintChk == 2 ) strTableBody = _T("Ser_Calc_KS_BodyEnd");   // I-End & J-End
			else
				ASSERT(0);

			pExcel->CopyRange(stRow + nIncreaseRow, stCol, m_strBaseBook, _T("Str_Rating"), strTableBody, rowCnt, colCnt);
			nIncreaseRow += (RkpoD.iPrintChk == 2)? 8 : 4;
		}
	}

	Print_Array_D_CELL(pExcel, arData, stRow);
	stRow += nIncreaseRow + 1;
}

void CRating_RKMC_XLOut::SetGetRkstCapacity(CArray<T_RKST_D, T_RKST_D&>& arRKSTData, CArray<D_CELL, D_CELL&>& arData)
{
	arData.RemoveAll();
}

void CRating_RKMC_XLOut::PrintGetRkstCapacity(IExcel* pExcel, int& stRow, int stCol, CArray<D_CELL, D_CELL&>& arData, int nCountSelectData)
{
	int rowCnt = 0;
	int colCnt = 0;

	CString strChapter = _T("");
	if ( m_iCode == KSCE_USD05_RATING )
		strChapter = _T("Ser_Calc_LCC_Chapter");
	else
	{
		ASSERT(0);
		return;
	}

	pExcel->CopyRange(stRow, stCol, m_strBaseBook, _T("Ser_Rating"), strChapter, rowCnt, colCnt);
	Print_Array_D_CELL(pExcel, arData, stRow+1);
	stRow += rowCnt;

	CString strComment1 = _T("");
	if ( m_iCode == KSCE_USD05_RATING )
		strComment1 = _T("Ser_Calc_LCC_BaseLoad");
	else
	{
		ASSERT(0);
		return;
	}

	pExcel->CopyRange(stRow, stCol, m_strBaseBook, _T("Ser_Rating"), strComment1, rowCnt, colCnt);
	Print_Array_D_CELL(pExcel, arData, stRow+1);
	stRow += rowCnt + 1;

	CString strComment2 = _T("");
	if ( m_iCode == KSCE_USD05_RATING )
		strComment2 = _T("Ser_Calc_LCC_UsedLoad");
	else
	{
		ASSERT(0);
		return;
	}

	pExcel->CopyRange(stRow, stCol, m_strBaseBook, _T("Ser_Rating"), strComment2, rowCnt, colCnt);
	Print_Array_D_CELL(pExcel, arData, stRow+1);
	stRow += rowCnt + 1;
}

void CRating_RKMC_XLOut::SetGetRkstCapacityTableHead(CArray<T_RKST_D, T_RKST_D&>& arRKSTData, CArray<D_CELL, D_CELL&>& arData)
{
	arData.RemoveAll();
}

void CRating_RKMC_XLOut::PrintGetRkstCapacityTableHead(IExcel* pExcel, int& stRow, int stCol, CArray<D_CELL, D_CELL&>& arData, int nCountSelectData)
{
	int rowCnt = 0;
	int colCnt = 0;
	int nIncreaseRow = 0;

	CString strTableHead = _T("");
	if ( m_iCode == KSCE_USD05_RATING )
		strTableHead = _T("Ser_Calc_LCC_Head");
	else
	{
		ASSERT(0);
		return;
	}

	pExcel->CopyRange(stRow, stCol, m_strBaseBook, _T("Ser_Rating"), strTableHead, rowCnt, colCnt);
	Print_Array_D_CELL(pExcel, arData, stRow+1);
	stRow += rowCnt;
}

void CRating_RKMC_XLOut::SetGetRkstCapacityTableBody(CArray<T_RKST_D, T_RKST_D&>& arRKSTData, CArray<D_CELL, D_CELL&>& arData)
{
	int nIncreaseRow = 0;
	arData.RemoveAll();

	T_RKPO_D RkpoD; RkpoD.Initialize();
	T_RKST_D RkstD; RkstD.Initialize();

	T_RKST_BASE RkstBaseI_1, RkstBaseI_2, RkstBaseI_3, RkstBaseI_4;
	T_RKST_BASE RkstBaseJ_1, RkstBaseJ_2, RkstBaseJ_3, RkstBaseJ_4;
	RkstBaseI_1.Initialize(); RkstBaseI_2.Initialize(); RkstBaseI_3.Initialize(); RkstBaseI_4.Initialize();
	RkstBaseJ_1.Initialize(); RkstBaseJ_2.Initialize(); RkstBaseJ_3.Initialize(); RkstBaseJ_4.Initialize();

	T_RKLC_K RklcKI_1, RklcKI_2, RklcKI_3, RklcKI_4;
	T_RKLC_K RklcKJ_1, RklcKJ_2, RklcKJ_3, RklcKJ_4;
	RklcKI_1 = RklcKI_2 = RklcKI_3 = RklcKI_4 = 0;
	RklcKJ_1 = RklcKJ_2 = RklcKJ_3 = RklcKJ_4 = 0;

	T_RKLC_D RklcDI_1, RklcDI_2, RklcDI_3, RklcDI_4;
	T_RKLC_D RklcDJ_1, RklcDJ_2, RklcDJ_3, RklcDJ_4;
	RklcDI_1.Initialize();  RklcDI_2.Initialize();  RklcDI_3.Initialize();  RklcDI_4.Initialize();
	RklcDJ_1.Initialize();  RklcDJ_2.Initialize();  RklcDJ_3.Initialize();  RklcDJ_4.Initialize();

	//(2008.04.03) Add by Unsang :: Lcom + Type
	CString strLcomType_I_1 = _T("");
	CString strLcomType_I_2 = _T("");
	CString strLcomType_I_3 = _T("");
	CString strLcomType_I_4 = _T("");
	CString strLcomType_J_1 = _T("");
	CString strLcomType_J_2 = _T("");
	CString strLcomType_J_3 = _T("");
	CString strLcomType_J_4 = _T("");

	int nCountSelectData = m_arRkstKeyList.GetSize();
	for ( int i = 0; i < nCountSelectData; i++ )
	{
		auto RkpoK = m_arRkstKeyList[i];
		RkpoD.Initialize();
		if (RkpoK.second == EN_EL_BEAM)
		{
			m_pDoc->m_pAttrCtrl->GetRkpo(RkpoK.first, RkpoD);
		}
		else if (RkpoK.second == EN_EL_VBEAM)
		{
			m_pDoc->m_pAttrCtrl->GetRkpv(RkpoK.first, RkpoD);
		}
		else ASSERT(0);

		RkstD.Initialize();
		m_pDoc->m_pPostCtrl->GetAnalysisResult()->GetConcreteStressRating4TableKR05(RkpoK, RkstD);

		if ( RkpoD.iPrintChk == 0 )      // I-End
		{
			RkstBaseI_1.Initialize(); RkstBaseI_1 = RkstD.BaseI[0];  // 상연-인장
			RkstBaseI_2.Initialize(); RkstBaseI_2 = RkstD.BaseI[1];  // 상연-압축
			RkstBaseI_3.Initialize(); RkstBaseI_3 = RkstD.BaseI[2];  // 하연-인장
			RkstBaseI_4.Initialize(); RkstBaseI_4 = RkstD.BaseI[3];  // 하연-압축

			// Element No
			arData.Add(D_CELL(nIncreaseRow, 1, GetOutputFormat(RkstBaseI_1.ElemK, 0))); // 공통적용 값임..

			// Critical Load Combination 
			RklcKI_1 = RkstBaseI_1.nRCaseK;
			RklcDI_1.Initialize(); m_pDoc->m_pAttrCtrl->GetRklc(RklcKI_1, RklcDI_1);

			RklcKI_2 = RkstBaseI_2.nRCaseK;
			RklcDI_2.Initialize(); m_pDoc->m_pAttrCtrl->GetRklc(RklcKI_2, RklcDI_2);

			RklcKI_3 = RkstBaseI_3.nRCaseK;
			RklcDI_3.Initialize(); m_pDoc->m_pAttrCtrl->GetRklc(RklcKI_3, RklcDI_3);

			RklcKI_4 = RkstBaseI_4.nRCaseK;
			RklcDI_4.Initialize(); m_pDoc->m_pAttrCtrl->GetRklc(RklcKI_4, RklcDI_4);

			strLcomType_I_1 = RklcDI_1.strCaseName + _T("(") + GetLcomType(RkstBaseI_1.nConcurrent) + _T(")");
			strLcomType_I_2 = RklcDI_2.strCaseName + _T("(") + GetLcomType(RkstBaseI_2.nConcurrent) + _T(")");
			strLcomType_I_3 = RklcDI_3.strCaseName + _T("(") + GetLcomType(RkstBaseI_3.nConcurrent) + _T(")");
			strLcomType_I_4 = RklcDI_4.strCaseName + _T("(") + GetLcomType(RkstBaseI_4.nConcurrent) + _T(")");

			arData.Add(D_CELL(nIncreaseRow, 8, strLcomType_I_1)); // 공통적용 값임..
			arData.Add(D_CELL(nIncreaseRow+1, 8, strLcomType_I_2)); // 공통적용 값임..
			arData.Add(D_CELL(nIncreaseRow+2, 8, strLcomType_I_3)); // 공통적용 값임..
			arData.Add(D_CELL(nIncreaseRow+3, 8, strLcomType_I_4)); // 공통적용 값임..

			// Rating Fct.
			arData.Add(D_CELL(nIncreaseRow, 11, GetOutputFormat(RkstBaseI_1.dRF_p[RkstBaseI_1.iPosiResult], 3), D_NUMBER_FORMAT_3));
			arData.Add(D_CELL(nIncreaseRow+1, 11, GetOutputFormat(RkstBaseI_2.dRF_p[RkstBaseI_2.iPosiResult], 3), D_NUMBER_FORMAT_3));
			arData.Add(D_CELL(nIncreaseRow+2, 11, GetOutputFormat(RkstBaseI_3.dRF_p[RkstBaseI_3.iPosiResult], 3), D_NUMBER_FORMAT_3));
			arData.Add(D_CELL(nIncreaseRow+3, 11, GetOutputFormat(RkstBaseI_4.dRF_p[RkstBaseI_4.iPosiResult], 3), D_NUMBER_FORMAT_3));

			// Base Capacity
			arData.Add(D_CELL(nIncreaseRow, 15, GetOutputFormat(RkstBaseI_1.dP0_p[RkstBaseI_1.iPosiResult], 3), D_NUMBER_FORMAT_3));
			arData.Add(D_CELL(nIncreaseRow+1, 15, GetOutputFormat(RkstBaseI_2.dP0_p[RkstBaseI_2.iPosiResult], 3), D_NUMBER_FORMAT_3));
			arData.Add(D_CELL(nIncreaseRow+2, 15, GetOutputFormat(RkstBaseI_3.dP0_p[RkstBaseI_3.iPosiResult], 3), D_NUMBER_FORMAT_3));
			arData.Add(D_CELL(nIncreaseRow+3, 15, GetOutputFormat(RkstBaseI_4.dP0_p[RkstBaseI_4.iPosiResult], 3), D_NUMBER_FORMAT_3));

			// Dgn Fct. (Ks x Kr x Kt)
			arData.Add(D_CELL(nIncreaseRow, 19, GetOutputFormat(RkstBaseI_1.dKsKrKt, 3), D_NUMBER_FORMAT_3));  // Fct. Ks
			arData.Add(D_CELL(nIncreaseRow+1, 19, GetOutputFormat(RkstBaseI_2.dKsKrKt, 3), D_NUMBER_FORMAT_3));  // Fct. Ks
			arData.Add(D_CELL(nIncreaseRow+2, 19, GetOutputFormat(RkstBaseI_3.dKsKrKt, 3), D_NUMBER_FORMAT_3));  // Fct. Ks
			arData.Add(D_CELL(nIncreaseRow+3, 19, GetOutputFormat(RkstBaseI_4.dKsKrKt, 3), D_NUMBER_FORMAT_3));  // Fct. Ks

			// Using Capacity
			arData.Add(D_CELL(nIncreaseRow, 23, GetOutputFormat(RkstBaseI_1.dP0a_p[RkstBaseI_1.iPosiResult], 3), D_NUMBER_FORMAT_3));  // Fct. Ks
			arData.Add(D_CELL(nIncreaseRow+1, 23, GetOutputFormat(RkstBaseI_2.dP0a_p[RkstBaseI_2.iPosiResult], 3), D_NUMBER_FORMAT_3));  // Fct. Ks
			arData.Add(D_CELL(nIncreaseRow+2, 23, GetOutputFormat(RkstBaseI_3.dP0a_p[RkstBaseI_3.iPosiResult], 3), D_NUMBER_FORMAT_3));  // Fct. Ks
			arData.Add(D_CELL(nIncreaseRow+3, 23, GetOutputFormat(RkstBaseI_4.dP0a_p[RkstBaseI_4.iPosiResult], 3), D_NUMBER_FORMAT_3));  // Fct. Ks

			// Dgn. Live Load
			arData.Add(D_CELL(nIncreaseRow, 27, GetOutputFormat(RkstBaseI_1.dDgnLoad, 3)));  // Fct. Ks

			// Check
			arData.Add(D_CELL(nIncreaseRow, 30, (RkstBaseI_1.bOK_p[RkstBaseI_1.iPosiResult])? _T("OK") : _T("NG")));  // Fct. Ks
			arData.Add(D_CELL(nIncreaseRow+1, 30, (RkstBaseI_2.bOK_p[RkstBaseI_2.iPosiResult])? _T("OK") : _T("NG")));  // Fct. Ks
			arData.Add(D_CELL(nIncreaseRow+2, 30, (RkstBaseI_3.bOK_p[RkstBaseI_3.iPosiResult])? _T("OK") : _T("NG")));  // Fct. Ks
			arData.Add(D_CELL(nIncreaseRow+3, 30, (RkstBaseI_4.bOK_p[RkstBaseI_4.iPosiResult])? _T("OK") : _T("NG")));  // Fct. Ks

			nIncreaseRow = nIncreaseRow + 4;
		}
		else if ( RkpoD.iPrintChk == 1 ) // J-End
		{
			RkstBaseJ_1; RkstBaseJ_1.Initialize(); RkstBaseJ_1 = RkstD.BaseJ[0];  // 상연-인장
			RkstBaseJ_2; RkstBaseJ_2.Initialize(); RkstBaseJ_2 = RkstD.BaseJ[1];  // 상연-압축
			RkstBaseJ_3; RkstBaseJ_3.Initialize(); RkstBaseJ_3 = RkstD.BaseJ[2];  // 하연-인장
			RkstBaseJ_4; RkstBaseJ_4.Initialize(); RkstBaseJ_4 = RkstD.BaseJ[3];  // 하연-압축

			// Element No
			arData.Add(D_CELL(nIncreaseRow, 1, GetOutputFormat(RkstBaseJ_1.ElemK, 0))); // 공통적용 값임..

			// Critical Load Combination 
			RklcKJ_1 = RkstBaseJ_1.nRCaseK;
			RklcDJ_1.Initialize(); m_pDoc->m_pAttrCtrl->GetRklc(RklcKJ_1, RklcDJ_1);

			RklcKJ_2 = RkstBaseJ_2.nRCaseK;
			RklcDJ_2.Initialize(); m_pDoc->m_pAttrCtrl->GetRklc(RklcKJ_2, RklcDJ_2);

			RklcKJ_3 = RkstBaseJ_3.nRCaseK;
			RklcDJ_3.Initialize(); m_pDoc->m_pAttrCtrl->GetRklc(RklcKJ_3, RklcDJ_3);

			RklcKJ_4 = RkstBaseJ_4.nRCaseK;
			RklcDJ_4.Initialize(); m_pDoc->m_pAttrCtrl->GetRklc(RklcKJ_4, RklcDJ_4);

			strLcomType_J_1 = RklcDJ_1.strCaseName + _T("(") + GetLcomType(RkstBaseJ_1.nConcurrent) + _T(")");
			strLcomType_J_2 = RklcDJ_2.strCaseName + _T("(") + GetLcomType(RkstBaseJ_2.nConcurrent) + _T(")");
			strLcomType_J_3 = RklcDJ_3.strCaseName + _T("(") + GetLcomType(RkstBaseJ_3.nConcurrent) + _T(")");
			strLcomType_J_4 = RklcDJ_4.strCaseName + _T("(") + GetLcomType(RkstBaseJ_4.nConcurrent) + _T(")");

			arData.Add(D_CELL(nIncreaseRow, 8, strLcomType_J_1)); // 공통적용 값임..
			arData.Add(D_CELL(nIncreaseRow+1, 8, strLcomType_J_2)); // 공통적용 값임..
			arData.Add(D_CELL(nIncreaseRow+2, 8, strLcomType_J_3)); // 공통적용 값임..
			arData.Add(D_CELL(nIncreaseRow+3, 8, strLcomType_J_4)); // 공통적용 값임..


			// Rating Fct.
			arData.Add(D_CELL(nIncreaseRow, 11, GetOutputFormat(RkstBaseJ_1.dRF_p[RkstBaseJ_1.iPosiResult], 3), D_NUMBER_FORMAT_3));
			arData.Add(D_CELL(nIncreaseRow+1, 11, GetOutputFormat(RkstBaseJ_2.dRF_p[RkstBaseJ_2.iPosiResult], 3), D_NUMBER_FORMAT_3));
			arData.Add(D_CELL(nIncreaseRow+2, 11, GetOutputFormat(RkstBaseJ_3.dRF_p[RkstBaseJ_3.iPosiResult], 3), D_NUMBER_FORMAT_3));
			arData.Add(D_CELL(nIncreaseRow+3, 11, GetOutputFormat(RkstBaseJ_4.dRF_p[RkstBaseJ_4.iPosiResult], 3), D_NUMBER_FORMAT_3));

			// Base Capacity
			arData.Add(D_CELL(nIncreaseRow, 15, GetOutputFormat(RkstBaseJ_1.dP0_p[RkstBaseJ_1.iPosiResult], 3), D_NUMBER_FORMAT_3));
			arData.Add(D_CELL(nIncreaseRow+1, 15, GetOutputFormat(RkstBaseJ_2.dP0_p[RkstBaseJ_2.iPosiResult], 3), D_NUMBER_FORMAT_3));
			arData.Add(D_CELL(nIncreaseRow+2, 15, GetOutputFormat(RkstBaseJ_3.dP0_p[RkstBaseJ_3.iPosiResult], 3), D_NUMBER_FORMAT_3));
			arData.Add(D_CELL(nIncreaseRow+3, 15, GetOutputFormat(RkstBaseJ_4.dP0_p[RkstBaseJ_4.iPosiResult], 3), D_NUMBER_FORMAT_3));

			// Dgn Fct. (Ks x Kr x Kt)
			arData.Add(D_CELL(nIncreaseRow, 19, GetOutputFormat(RkstBaseJ_1.dKsKrKt, 3), D_NUMBER_FORMAT_3));  // Fct. Ks
			arData.Add(D_CELL(nIncreaseRow+1, 19, GetOutputFormat(RkstBaseJ_2.dKsKrKt, 3), D_NUMBER_FORMAT_3));  // Fct. Ks
			arData.Add(D_CELL(nIncreaseRow+2, 19, GetOutputFormat(RkstBaseJ_3.dKsKrKt, 3), D_NUMBER_FORMAT_3));  // Fct. Ks
			arData.Add(D_CELL(nIncreaseRow+3, 19, GetOutputFormat(RkstBaseJ_4.dKsKrKt, 3), D_NUMBER_FORMAT_3));  // Fct. Ks

			// Using Capacity
			arData.Add(D_CELL(nIncreaseRow, 23, GetOutputFormat(RkstBaseJ_1.dP0a_p[RkstBaseJ_1.iPosiResult], 3), D_NUMBER_FORMAT_3));  // Fct. Ks
			arData.Add(D_CELL(nIncreaseRow+1, 23, GetOutputFormat(RkstBaseJ_2.dP0a_p[RkstBaseJ_2.iPosiResult], 3), D_NUMBER_FORMAT_3));  // Fct. Ks
			arData.Add(D_CELL(nIncreaseRow+2, 23, GetOutputFormat(RkstBaseJ_3.dP0a_p[RkstBaseJ_3.iPosiResult], 3), D_NUMBER_FORMAT_3));  // Fct. Ks
			arData.Add(D_CELL(nIncreaseRow+3, 23, GetOutputFormat(RkstBaseJ_4.dP0a_p[RkstBaseJ_4.iPosiResult], 3), D_NUMBER_FORMAT_3));  // Fct. Ks

			// Dgn. Live Load
			arData.Add(D_CELL(nIncreaseRow, 27, GetOutputFormat(RkstBaseJ_1.dDgnLoad, 3)));  // Fct. Ks

			// Check
			arData.Add(D_CELL(nIncreaseRow, 30, (RkstBaseJ_1.bOK_p[RkstBaseJ_1.iPosiResult])? _T("OK") : _T("NG")));  // Fct. Ks
			arData.Add(D_CELL(nIncreaseRow+1, 30, (RkstBaseJ_2.bOK_p[RkstBaseJ_2.iPosiResult])? _T("OK") : _T("NG")));  // Fct. Ks
			arData.Add(D_CELL(nIncreaseRow+2, 30, (RkstBaseJ_3.bOK_p[RkstBaseJ_3.iPosiResult])? _T("OK") : _T("NG")));  // Fct. Ks
			arData.Add(D_CELL(nIncreaseRow+3, 30, (RkstBaseJ_4.bOK_p[RkstBaseJ_4.iPosiResult])? _T("OK") : _T("NG")));  // Fct. Ks

			nIncreaseRow = nIncreaseRow + 4;
		}
		else if ( RkpoD.iPrintChk == 2 ) // I-End & J-End
		{
			RkstBaseI_1; RkstBaseI_1.Initialize(); RkstBaseI_1 = RkstD.BaseI[0];  // 상연-인장
			RkstBaseI_2; RkstBaseI_2.Initialize(); RkstBaseI_2 = RkstD.BaseI[1];  // 상연-압축
			RkstBaseI_3; RkstBaseI_3.Initialize(); RkstBaseI_3 = RkstD.BaseI[2];  // 하연-인장
			RkstBaseI_4; RkstBaseI_4.Initialize(); RkstBaseI_4 = RkstD.BaseI[3];  // 하연-압축
			RkstBaseJ_1; RkstBaseJ_1.Initialize(); RkstBaseJ_1 = RkstD.BaseJ[0];  // 상연-인장
			RkstBaseJ_2; RkstBaseJ_2.Initialize(); RkstBaseJ_2 = RkstD.BaseJ[1];  // 상연-압축
			RkstBaseJ_3; RkstBaseJ_3.Initialize(); RkstBaseJ_3 = RkstD.BaseJ[2];  // 하연-인장
			RkstBaseJ_4; RkstBaseJ_4.Initialize(); RkstBaseJ_4 = RkstD.BaseJ[3];  // 하연-압축

			// Element No
			arData.Add(D_CELL(nIncreaseRow, 1, GetOutputFormat(RkstBaseI_1.ElemK, 0))); // 공통적용 값임..

			// Critical Load Combination 
			RklcKI_1 = RkstBaseI_1.nRCaseK;
			RklcDI_1.Initialize(); m_pDoc->m_pAttrCtrl->GetRklc(RklcKI_1, RklcDI_1);

			RklcKI_2 = RkstBaseI_2.nRCaseK;
			RklcDI_2.Initialize(); m_pDoc->m_pAttrCtrl->GetRklc(RklcKI_2, RklcDI_2);

			RklcKI_3 = RkstBaseI_3.nRCaseK;
			RklcDI_3.Initialize(); m_pDoc->m_pAttrCtrl->GetRklc(RklcKI_3, RklcDI_3);

			RklcKI_4 = RkstBaseI_4.nRCaseK;
			RklcDI_4.Initialize(); m_pDoc->m_pAttrCtrl->GetRklc(RklcKI_4, RklcDI_4);

			RklcKJ_1 = RkstBaseJ_1.nRCaseK;
			RklcDJ_1.Initialize(); m_pDoc->m_pAttrCtrl->GetRklc(RklcKJ_1, RklcDJ_1);

			RklcKJ_2 = RkstBaseJ_2.nRCaseK;
			RklcDJ_2.Initialize(); m_pDoc->m_pAttrCtrl->GetRklc(RklcKJ_2, RklcDJ_2);

			RklcKJ_3 = RkstBaseJ_3.nRCaseK;
			RklcDJ_3.Initialize(); m_pDoc->m_pAttrCtrl->GetRklc(RklcKJ_3, RklcDJ_3);

			RklcKJ_4 = RkstBaseJ_4.nRCaseK;
			RklcDJ_4.Initialize(); m_pDoc->m_pAttrCtrl->GetRklc(RklcKJ_4, RklcDJ_4);

			strLcomType_I_1 = RklcDI_1.strCaseName + _T("(") + GetLcomType(RkstBaseI_1.nConcurrent) + _T(")");
			strLcomType_I_2 = RklcDI_2.strCaseName + _T("(") + GetLcomType(RkstBaseI_2.nConcurrent) + _T(")");
			strLcomType_I_3 = RklcDI_3.strCaseName + _T("(") + GetLcomType(RkstBaseI_3.nConcurrent) + _T(")");
			strLcomType_I_4 = RklcDI_4.strCaseName + _T("(") + GetLcomType(RkstBaseI_4.nConcurrent) + _T(")");
			strLcomType_J_1 = RklcDJ_1.strCaseName + _T("(") + GetLcomType(RkstBaseJ_1.nConcurrent) + _T(")");
			strLcomType_J_2 = RklcDJ_2.strCaseName + _T("(") + GetLcomType(RkstBaseJ_2.nConcurrent) + _T(")");
			strLcomType_J_3 = RklcDJ_3.strCaseName + _T("(") + GetLcomType(RkstBaseJ_3.nConcurrent) + _T(")");
			strLcomType_J_4 = RklcDJ_4.strCaseName + _T("(") + GetLcomType(RkstBaseJ_4.nConcurrent) + _T(")");

			arData.Add(D_CELL(nIncreaseRow, 8, strLcomType_I_1)); // 공통적용 값임..
			arData.Add(D_CELL(nIncreaseRow+1, 8, strLcomType_I_2)); // 공통적용 값임..
			arData.Add(D_CELL(nIncreaseRow+2, 8, strLcomType_I_3)); // 공통적용 값임..
			arData.Add(D_CELL(nIncreaseRow+3, 8, strLcomType_I_4)); // 공통적용 값임..
			arData.Add(D_CELL(nIncreaseRow+4, 8, strLcomType_J_1)); // 공통적용 값임..
			arData.Add(D_CELL(nIncreaseRow+5, 8, strLcomType_J_2)); // 공통적용 값임..
			arData.Add(D_CELL(nIncreaseRow+6, 8, strLcomType_J_3)); // 공통적용 값임..
			arData.Add(D_CELL(nIncreaseRow+7, 8, strLcomType_J_4)); // 공통적용 값임..

			// Rating Fct.
			arData.Add(D_CELL(nIncreaseRow, 11, GetOutputFormat(RkstBaseI_1.dRF_p[RkstBaseI_1.iPosiResult], 3), D_NUMBER_FORMAT_3));
			arData.Add(D_CELL(nIncreaseRow+1, 11, GetOutputFormat(RkstBaseI_2.dRF_p[RkstBaseI_2.iPosiResult], 3), D_NUMBER_FORMAT_3));
			arData.Add(D_CELL(nIncreaseRow+2, 11, GetOutputFormat(RkstBaseI_3.dRF_p[RkstBaseI_3.iPosiResult], 3), D_NUMBER_FORMAT_3));
			arData.Add(D_CELL(nIncreaseRow+3, 11, GetOutputFormat(RkstBaseI_4.dRF_p[RkstBaseI_4.iPosiResult], 3), D_NUMBER_FORMAT_3));
			arData.Add(D_CELL(nIncreaseRow+4, 11, GetOutputFormat(RkstBaseJ_1.dRF_p[RkstBaseJ_1.iPosiResult], 3), D_NUMBER_FORMAT_3));
			arData.Add(D_CELL(nIncreaseRow+5, 11, GetOutputFormat(RkstBaseJ_2.dRF_p[RkstBaseJ_2.iPosiResult], 3), D_NUMBER_FORMAT_3));
			arData.Add(D_CELL(nIncreaseRow+6, 11, GetOutputFormat(RkstBaseJ_3.dRF_p[RkstBaseJ_3.iPosiResult], 3), D_NUMBER_FORMAT_3));
			arData.Add(D_CELL(nIncreaseRow+7, 11, GetOutputFormat(RkstBaseJ_4.dRF_p[RkstBaseJ_4.iPosiResult], 3), D_NUMBER_FORMAT_3));


			// Base Capacity
			arData.Add(D_CELL(nIncreaseRow, 15, GetOutputFormat(RkstBaseI_1.dP0_p[RkstBaseI_1.iPosiResult], 3), D_NUMBER_FORMAT_3));
			arData.Add(D_CELL(nIncreaseRow+1, 15, GetOutputFormat(RkstBaseI_2.dP0_p[RkstBaseI_2.iPosiResult], 3), D_NUMBER_FORMAT_3));
			arData.Add(D_CELL(nIncreaseRow+2, 15, GetOutputFormat(RkstBaseI_3.dP0_p[RkstBaseI_3.iPosiResult], 3), D_NUMBER_FORMAT_3));
			arData.Add(D_CELL(nIncreaseRow+3, 15, GetOutputFormat(RkstBaseI_4.dP0_p[RkstBaseI_4.iPosiResult], 3), D_NUMBER_FORMAT_3));
			arData.Add(D_CELL(nIncreaseRow+4, 15, GetOutputFormat(RkstBaseJ_1.dP0_p[RkstBaseJ_1.iPosiResult], 3), D_NUMBER_FORMAT_3));
			arData.Add(D_CELL(nIncreaseRow+5, 15, GetOutputFormat(RkstBaseJ_2.dP0_p[RkstBaseJ_2.iPosiResult], 3), D_NUMBER_FORMAT_3));
			arData.Add(D_CELL(nIncreaseRow+6, 15, GetOutputFormat(RkstBaseJ_3.dP0_p[RkstBaseJ_3.iPosiResult], 3), D_NUMBER_FORMAT_3));
			arData.Add(D_CELL(nIncreaseRow+7, 15, GetOutputFormat(RkstBaseJ_4.dP0_p[RkstBaseJ_4.iPosiResult], 3), D_NUMBER_FORMAT_3));

			// Dgn Fct. (Ks x Kr x Kt)
			arData.Add(D_CELL(nIncreaseRow, 19, GetOutputFormat(RkstBaseI_1.dKsKrKt, 3), D_NUMBER_FORMAT_3));  // Fct. Ks
			arData.Add(D_CELL(nIncreaseRow+1, 19, GetOutputFormat(RkstBaseI_2.dKsKrKt, 3), D_NUMBER_FORMAT_3));  // Fct. Ks
			arData.Add(D_CELL(nIncreaseRow+2, 19, GetOutputFormat(RkstBaseI_3.dKsKrKt, 3), D_NUMBER_FORMAT_3));  // Fct. Ks
			arData.Add(D_CELL(nIncreaseRow+3, 19, GetOutputFormat(RkstBaseI_4.dKsKrKt, 3), D_NUMBER_FORMAT_3));  // Fct. Ks
			arData.Add(D_CELL(nIncreaseRow+4, 19, GetOutputFormat(RkstBaseJ_1.dKsKrKt, 3), D_NUMBER_FORMAT_3));  // Fct. Ks
			arData.Add(D_CELL(nIncreaseRow+5, 19, GetOutputFormat(RkstBaseJ_2.dKsKrKt, 3), D_NUMBER_FORMAT_3));  // Fct. Ks
			arData.Add(D_CELL(nIncreaseRow+6, 19, GetOutputFormat(RkstBaseJ_3.dKsKrKt, 3), D_NUMBER_FORMAT_3));  // Fct. Ks
			arData.Add(D_CELL(nIncreaseRow+7, 19, GetOutputFormat(RkstBaseJ_4.dKsKrKt, 3), D_NUMBER_FORMAT_3));  // Fct. Ks

			// Using Capacity
			arData.Add(D_CELL(nIncreaseRow, 23, GetOutputFormat(RkstBaseI_1.dP0a_p[RkstBaseI_1.iPosiResult], 3), D_NUMBER_FORMAT_3));  // Fct. Ks
			arData.Add(D_CELL(nIncreaseRow+1, 23, GetOutputFormat(RkstBaseI_2.dP0a_p[RkstBaseI_2.iPosiResult], 3), D_NUMBER_FORMAT_3));  // Fct. Ks
			arData.Add(D_CELL(nIncreaseRow+2, 23, GetOutputFormat(RkstBaseI_3.dP0a_p[RkstBaseI_3.iPosiResult], 3), D_NUMBER_FORMAT_3));  // Fct. Ks
			arData.Add(D_CELL(nIncreaseRow+3, 23, GetOutputFormat(RkstBaseI_4.dP0a_p[RkstBaseI_4.iPosiResult], 3), D_NUMBER_FORMAT_3));  // Fct. Ks
			arData.Add(D_CELL(nIncreaseRow+4, 23, GetOutputFormat(RkstBaseJ_1.dP0a_p[RkstBaseJ_1.iPosiResult], 3), D_NUMBER_FORMAT_3));  // Fct. Ks
			arData.Add(D_CELL(nIncreaseRow+5, 23, GetOutputFormat(RkstBaseJ_2.dP0a_p[RkstBaseJ_2.iPosiResult], 3), D_NUMBER_FORMAT_3));  // Fct. Ks
			arData.Add(D_CELL(nIncreaseRow+6, 23, GetOutputFormat(RkstBaseJ_3.dP0a_p[RkstBaseJ_3.iPosiResult], 3), D_NUMBER_FORMAT_3));  // Fct. Ks
			arData.Add(D_CELL(nIncreaseRow+7, 23, GetOutputFormat(RkstBaseJ_4.dP0a_p[RkstBaseJ_4.iPosiResult], 3), D_NUMBER_FORMAT_3));  // Fct. Ks

			// Dgn. Live Load
			arData.Add(D_CELL(nIncreaseRow, 27, GetOutputFormat(RkstBaseI_1.dDgnLoad, 3)));  // Fct. Ks

			// Check
			arData.Add(D_CELL(nIncreaseRow, 30, (RkstBaseI_1.bOK_p[RkstBaseI_1.iPosiResult])? _T("OK") : _T("NG")));  // Fct. Ks
			arData.Add(D_CELL(nIncreaseRow+1, 30, (RkstBaseI_2.bOK_p[RkstBaseI_2.iPosiResult])? _T("OK") : _T("NG")));  // Fct. Ks
			arData.Add(D_CELL(nIncreaseRow+2, 30, (RkstBaseI_3.bOK_p[RkstBaseI_3.iPosiResult])? _T("OK") : _T("NG")));  // Fct. Ks
			arData.Add(D_CELL(nIncreaseRow+3, 30, (RkstBaseI_4.bOK_p[RkstBaseI_4.iPosiResult])? _T("OK") : _T("NG")));  // Fct. Ks
			arData.Add(D_CELL(nIncreaseRow+4, 30, (RkstBaseJ_1.bOK_p[RkstBaseJ_1.iPosiResult])? _T("OK") : _T("NG")));  // Fct. Ks
			arData.Add(D_CELL(nIncreaseRow+5, 30, (RkstBaseJ_2.bOK_p[RkstBaseJ_2.iPosiResult])? _T("OK") : _T("NG")));  // Fct. Ks
			arData.Add(D_CELL(nIncreaseRow+6, 30, (RkstBaseJ_3.bOK_p[RkstBaseJ_3.iPosiResult])? _T("OK") : _T("NG")));  // Fct. Ks
			arData.Add(D_CELL(nIncreaseRow+7, 30, (RkstBaseJ_4.bOK_p[RkstBaseJ_4.iPosiResult])? _T("OK") : _T("NG")));  // Fct. Ks

			nIncreaseRow = nIncreaseRow + 8;
		}
		else
		{
			ASSERT(0);
			return;
		}
	}
}

void CRating_RKMC_XLOut::PrintGetRkstCapacityTableBody(IExcel* pExcel, int& stRow, int stCol, CArray<D_CELL, D_CELL&>& arData, int nCountSelectData)
{
	int rowCnt = 0;
	int colCnt = 0;
	int nIncreaseRow = 0;

	CString strTableBody = _T("");
	for ( int i = 0; i < nCountSelectData; i++ )
	{
		auto  ElemKey = m_arRkstKeyList[i];
		T_RKPO_D RkpoD; RkpoD.Initialize();
		if (ElemKey.second == EN_EL_BEAM)
		{
			m_pDoc->m_pAttrCtrl->GetRkpo(ElemKey.first, RkpoD);
		}
		else if (ElemKey.second == EN_EL_VBEAM)
		{
			m_pDoc->m_pAttrCtrl->GetRkpv(ElemKey.first, RkpoD);
		}
		else ASSERT(0);

		if ( i < nCountSelectData - 1 ) // End!!
		{
			if ( RkpoD.iPrintChk == 0 )      strTableBody = _T("Ser_Calc_LCC_Body_I");  // I-End
			else if ( RkpoD.iPrintChk == 1 ) strTableBody = _T("Ser_Calc_LCC_Body_J");  // J-End
			else if ( RkpoD.iPrintChk == 2 ) strTableBody = _T("Ser_Calc_LCC_Body");    // I-End & J-End
			else
				ASSERT(0);

			pExcel->CopyRange(stRow + nIncreaseRow, stCol, m_strBaseBook, _T("Str_Rating"), strTableBody, rowCnt, colCnt);
			nIncreaseRow += (RkpoD.iPrintChk == 2)? 8 : 4;
		}
		else
		{
			if ( RkpoD.iPrintChk == 0 )      strTableBody = _T("Ser_Calc_LCC_BodyEnd_I"); // I-End                         
			else if ( RkpoD.iPrintChk == 1 ) strTableBody = _T("Ser_Calc_LCC_BodyEnd_J"); // J-End
			else if ( RkpoD.iPrintChk == 2 ) strTableBody = _T("Ser_Calc_LCC_BodyEnd");   // I-End & J-End
			else
				ASSERT(0);

			pExcel->CopyRange(stRow + nIncreaseRow, stCol, m_strBaseBook, _T("Str_Rating"), strTableBody, rowCnt, colCnt);
			nIncreaseRow += (RkpoD.iPrintChk == 2)? 8 : 4;
		}
	}

	Print_Array_D_CELL(pExcel, arData, stRow);
	stRow += nIncreaseRow + 1;
}


void CRating_RKMC_XLOut::Print_RFReport(CString szProgramDir, CString szProjectFileDir, CString strPath, int iPrintOpt, CArray<RF_RES_RF_KSCE_PSC_D, RF_RES_RF_KSCE_PSC_D&>& aRFRptData)
{

	CDgnEngineProgressDlg ProDlg;
	ProDlg.Create_ProgressDlg(_T(""), _T(""), FALSE);	// Stop버튼이 보여지더라도 Progress창의 콘트롤이 안됨
	m_pProgressInte = ProDlg.Get_InterfacePoint();
	m_pProgressInte->bUpdateTitle   = TRUE;
	m_pProgressInte->bUpdateMessage = TRUE;
	m_pProgressInte->bUpdatePercent = TRUE;

	m_strBasePath   = szProgramDir;
	m_strDataPath   = szProjectFileDir;

	if ( m_pProgressInte == NULL || m_strBasePath==_T("") || m_strDataPath==_T("") ) return;

	const int nCodeNum = m_pDataCtrl->Get_Code();
	m_pDoc->SetCivilCodeDgn(nCodeNum);
	if (nCodeNum==KSCE_USD10_RATING || nCodeNum==KSCE_RAIL_USD11_RATING)
	{
		Print_RFReport_PSCRating(strPath, iPrintOpt, aRFRptData);
	}
	else if ( nCodeNum==KSCE_LSD15_RATING )
	{
		Print_RFReport_PSCRating_LSD(strPath, iPrintOpt, aRFRptData);
	}
	else
	{
		ASSERT(0);
		Print_RFReport_PSCRating(strPath, iPrintOpt, aRFRptData);
	}

}


void CRating_RKMC_XLOut::Print_RFReport_PSCRating(CString strPath, int iPrintOpt, CArray<RF_RES_RF_KSCE_PSC_D, RF_RES_RF_KSCE_PSC_D&>& aRFRptData)
{
	// Update Progress Dialog.
	Update_ProgressInft(_STR(_T("PSC 내하력 계산서 출력중...")), _STR(_T("PSC 내하력 계산서 생성중...")), 0);

	// PSC Detail 계산서
	const int nCodeNum = m_pDataCtrl->Get_Code();
	m_strBaseBook = SetGetPSCReportBaseFile(nCodeNum);

	CArray<unsigned int, unsigned int> arProcID;
	GetExistedExcelProcessID(arProcID);

	int nOutType;
	BOOL bRunMS, bRowMark, bColumnMark;
	CProduct::GetTestOutType(nOutType, bRunMS, bRowMark, bColumnMark);
	// CDgnRating_KSCE_USD05 이거부터 LibXL로 대체해야 여기를 LibXL로 변경할 수 있음.
	CMSExcel* pXL = new CMSExcel(nOutType, bRunMS, bRowMark, bColumnMark);
	IExcel* IXL = new MExcel_MS(pXL);

#if defined (_DEBUG)
	pXL->SetVisible(TRUE);
#endif
	if (CTestEnvMgr::IsTestEnvST(_T("ExcelVisible"), _T("yes")))
		pXL->SetVisible(TRUE);
	else
		pXL->SetVisible(FALSE);

	if ( !pXL->Open(m_strBasePath + m_strBaseBook) ) return;  //* base1 열기

	AddBook(IXL, strPath, iPrintOpt); //* 출력할 새 파일 열기

	SetOutputUnitSystem();
	SetUnitConvertFactor();

	T_RAKR_D RakrD;   RakrD.Initialize();
	m_pDoc->m_pAttrCtrl2->GetRakr(RakrD);

	// ******************************
	// 세부 계산서 출력 - 휨검토
	// ******************************
	int nCodePSC = 0;
	if ( nCodeNum==KSCE_USD10_RATING )			 nCodePSC =KSCE_USD10;
	else if ( nCodeNum==KSCE_RAIL_USD11_RATING ) nCodePSC =KSCE_RAIL_USD11;
	else if ( nCodeNum==KSCE_LSD15_RATING )      nCodePSC =KSCE_LSD15_PSC;
	else ASSERT(0);

	CDgnRating_KSCE_USD05 RatingKS05;
	RatingKS05.SetDataCtrlPointer4Rating(nCodePSC, m_pDataCtrl);

	T_RKMC_CASE RkmcCaseD;
	CString strSheetName;

	int iCountProgressBar = 0;
	int nTotalCount = aRFRptData.GetSize() * 2.0;
	int iPercent = (int)(100*(0)/nTotalCount);
	Update_ProgressInft(_T(""), _T(""), iPercent);

	int nChapterNum = 5;
	int i =0;
	for ( i=0; i<aRFRptData.GetSize(); ++i )  // 각 절점별 출력.
	{
		if ( !RakrD.bPrintParm[1] ) continue;

		RF_RES_RF_KSCE_PSC_D &ResData = aRFRptData[i];
		ElemPairK nElem(ResData.nElem, EN_EL_BEAM);
		int nIJ   = ResData.nIJ;
		BOOL bCompoSect = m_pDoc->m_pPostCtrl->IsElem4CompositePSCSect(nElem);

		T_RKMC_D RkmcD; RkmcD.Initialize();
		m_pDoc->m_pPostCtrl->GetAnalysisResult()->GetFlexuralStrengthRating4TableKR05(nElem, RkmcD);

		RkmcCaseD.RkmcBase[0] = (nIJ==0)? RkmcD.RkmcBase[0] : RkmcD.RkmcBase[2];
		RkmcCaseD.RkmcBase[1] = (nIJ==0)? RkmcD.RkmcBase[1] : RkmcD.RkmcBase[3];

		strSheetName.Format(_T("%d.단면설계(%d_%s)"), nChapterNum, ResData.nElem, GetStringPositionRating(ResData.nIJ));

		if ( pXL->IsExistSheetName(strSheetName) )
		{
			strSheetName = pXL->GetMaximumSheetName(strSheetName) + _T("_New");
		}

		pXL->AddSheet2(strSheetName);
		pXL->SetStandardFont(_T("맑은 고딕"));
		pXL->SetColumnWidth(0, 0, 0, 31, 1.8);  // 28->74
		pXL->SetRowHeight(15);
		pXL->SetMargin(13, 12, 20, 17);

		RatingKS05.Set_CMSExcel_KSCE10(pXL, m_strBaseBook, nCodePSC);
		if ( !RatingKS05.Get_RatingFlexuralStrength_Print(pXL, nElem, nIJ, bCompoSect, strSheetName, RakrD, RkmcCaseD) ) continue;   // 절점별 min/max 휨검토 출력 

		iCountProgressBar++;
		iPercent = (int)(100*(iCountProgressBar)/nTotalCount);
		Update_ProgressInft(_T(""), _T(""), iPercent);
		if ( m_pProgressInte->bStopExecute ) { break; }

	}

	pXL->SaveAs(pXL->GetBookName(), strPath);//* 출력파일 저쟝..

	// ******************************
	// Rating 계산서 출력
	// ******************************
	m_strBaseBook = _T("Rating Design_Base.xlsx");

	if ( !pXL->Open(m_strBasePath + m_strBaseBook) ) return;  //* base2 열기
	pXL->Open(strPath);                         //* 저장된 출력파일 열기
	pXL->SetActiveBook(pXL->GetBookName());

	nChapterNum += 1;
	for ( i=0; i<aRFRptData.GetSize(); ++i )  // 각 절점별 출력.
	{
		CString strSheetName;
		RF_RES_RF_KSCE_PSC_D &ResData = aRFRptData[i];

		strSheetName.Format(_T("%d.내하력평가(%d_%s)"), nChapterNum, ResData.nElem, GetStringPositionRating(ResData.nIJ));

		if ( pXL->IsExistSheetName(strSheetName) )
		{
			strSheetName = pXL->GetMaximumSheetName(strSheetName) + _T("_New");
		}

		pXL->AddSheet2(strSheetName);
		pXL->SetStandardFont(_T("맑은 고딕"));
		pXL->SetColumnWidth(0, 0, 0, 31, 1.8);  // 28->74
		pXL->SetRowHeight(15);
		pXL->SetMargin(13, 12, 20, 17);

		// 1. Rating 검토
		m_strBaseSheet = _T("RC & PSC Rating_Base");
		Print_Detail_PSCRating(IXL, ResData, strSheetName);

		// 2. force 검토 - max,min 출력
		if ( RakrD.bPrintParm[0] )
		{
			strSheetName.Format(_T("참조>단면력(%d_%s)"), ResData.nElem, GetStringPositionRating(ResData.nIJ));

			if ( pXL->IsExistSheetName(strSheetName) )
			{
				strSheetName = pXL->GetMaximumSheetName(strSheetName) + _T("_New");
			}

			pXL->AddSheet2(strSheetName);
			pXL->SetStandardFont(_T("맑은 고딕"));
			pXL->SetColumnWidth(0, 0, 0, 31, 1.8);  // 28->74
			pXL->SetRowHeight(15);
			pXL->SetMargin(13, 12, 20, 17);


			m_strBaseSheet = _T("Force");
			Print_Detail_Force(IXL, ResData, strSheetName);
		}

		iCountProgressBar++;
		iPercent = (int)(100*(iCountProgressBar)/nTotalCount);
		Update_ProgressInft(_T(""), _T(""), iPercent);
		if ( m_pProgressInte->bStopExecute ) { break; }

	}

	// 정리
	Update_ProgressInft(_T(""), _T(""), 100);

	pXL->DeleteDefaultSheet();
	pXL->SaveAs(pXL->GetBookName(), strPath);
	pXL->Quit();
	delete pXL;
	delete IXL;

	KillExcelProcess(arProcID);


	// 파일 보여주기
	ShellExecute(NULL, _T("open"), (strPath), NULL, NULL, SW_SHOWNORMAL);
}

void CRating_RKMC_XLOut::Print_RFReport_PSCRating_LSD(CString strPath, int iPrintOpt, CArray<RF_RES_RF_KSCE_PSC_D, RF_RES_RF_KSCE_PSC_D&>& aRFRptData)
{
	// Update Progress Dialog.
	Update_ProgressInft(_STR(_T("PSC 내하력 계산서 출력중...")), _STR(_T("PSC 내하력 계산서 생성중...")), 0);

	// PSC Detail 계산서
	const int nCodeNum = m_pDataCtrl->Get_Code();
	m_strBaseBook = SetGetPSCReportBaseFile(nCodeNum);

	CExcelGenerator Excel(_T(""), XLTYPE::TYPE_LIBXL);
	CString strFileName = Excel.GetDLLFilePath(m_strBaseBook, FALSE);

	IExcel* pXL = Excel.Generate(strFileName);
	if(Excel.GetExcelType() == XLTYPE::TYPE_LIBXL)
	{
		m_strBaseBook = strFileName; // 경로 포함.
	}

	SetOutputUnitSystem();
	SetUnitConvertFactor();

	T_RAKR_D RakrD;   RakrD.Initialize();
	m_pDoc->m_pAttrCtrl2->GetRakr(RakrD);

	// ******************************
	// 세부 계산서 출력 - 휨검토
	// ******************************
	int nCodePSC = 0;
	if ( nCodeNum==KSCE_USD10_RATING )			 nCodePSC =KSCE_USD10;
	else if ( nCodeNum==KSCE_RAIL_USD11_RATING ) nCodePSC =KSCE_RAIL_USD11;
	else if ( nCodeNum==KSCE_LSD15_RATING )      nCodePSC =KSCE_LSD15_PSC;
	else ASSERT(0);

	BOOL bSuccess = TRUE;
	try
	{
		T_RKMC_CASE RkmcCaseD;
		CString strSheetName;

		int iCountProgressBar = 0;
		int nTotalCount = aRFRptData.GetSize() * 2.0;
		int iPercent = (int)(100*(0)/nTotalCount);
		Update_ProgressInft(_T(""), _T(""), iPercent);

		int nChapterNum = 5;
		int i =0;
		for ( i=0; i<aRFRptData.GetSize(); ++i )  // 각 절점별 출력.
		{
			if ( !RakrD.bPrintParm[1] ) continue;

			RF_RES_RF_KSCE_PSC_D &ResData = aRFRptData[i];
			ElemPairK nElem(ResData.nElem, EN_EL_BEAM);
			int nIJ   = ResData.nIJ;
			BOOL bCompoSect = m_pDoc->m_pPostCtrl->IsElem4CompositePSCSect(nElem);

			T_RKMC_D RkmcD; RkmcD.Initialize();
			m_pDoc->m_pPostCtrl->GetAnalysisResult()->GetFlexuralStrengthRating4TableKR05(nElem, RkmcD);

			RkmcCaseD.RkmcBase[0] = (nIJ==0)? RkmcD.RkmcBase[0] : RkmcD.RkmcBase[2];
			RkmcCaseD.RkmcBase[1] = (nIJ==0)? RkmcD.RkmcBase[1] : RkmcD.RkmcBase[3];

			strSheetName.Format(_T("%d.단면설계(%d_%s)"), nChapterNum, ResData.nElem, GetStringPositionRating(ResData.nIJ));

			if ( pXL->IsExistSheetName(strSheetName) )
			{
				strSheetName = pXL->GetMaximumSheetName(strSheetName) + _T("_New");
			}

			pXL->AddSheet2(strSheetName);
			pXL->SetStandardFont(_T("맑은 고딕"));
			pXL->SetRowHeight(15);
			pXL->SetMargin(13, 12, 20, 17);
			pXL->SetColumnWidth(0, 0, 0, 31, 1.8);  // 28->74

			CString strPathName=m_pDoc->GetPathName();
			CFileCtrl FileCtrl(strPathName);
			CString strFileName = FileCtrl.GetFilePath();
			FileCtrl.SetFilePathAndName(strPath);
			CString strSaveFileName = FileCtrl.GetFileName();

			CDgn_PSCRptManager_LSD12_Comp myCompositePSCReport;
			if ( !myCompositePSCReport.Initialize_Rating(nCodePSC, pXL/*pIExcel*/, m_pDoc->GetProgramPath() + _T("Excel Base File\\"), strFileName, m_strBaseBook, strSheetName) ) continue;
			PSC_DATA_POSI InData;
			PSC_FRCR_D EngBdcrD;
			BOOL bFirst = TRUE;
			try
			{
				for ( int i = 0; i < 2; ++i )
				{
					// Positive, Negative
					InData.Initialize();
					EngBdcrD.Initialize();
					if ( !m_pDataCtrl->Get_RatingFlexuralStrengthKR_Print(nElem, bCompoSect, nIJ, i, InData, RakrD, RkmcCaseD, EngBdcrD) ) continue;   // 절점별 min/max 휨검토 출력 
					if ( bFirst )
					{
						if ( !myCompositePSCReport.Print_DetailReport_DgnCondition(nCodePSC, nElem, nIJ, InData, EngBdcrD) ) continue;
						bFirst = FALSE;
					}
					// Positive, Negative
					int kk = nIJ*2 + i;
					if ( !myCompositePSCReport.Print_DetailReport_FlexureDesign(nCodePSC, nElem, kk, InData, EngBdcrD) ) continue;
				}
			}
			catch ( ... )
			{
				bSuccess = FALSE;
				VERIFY(myCompositePSCReport.Terminate(nCodePSC, !bSuccess));
				throw FALSE;
			}


			iCountProgressBar++;
			iPercent = (int)(100*(iCountProgressBar)/nTotalCount);
			Update_ProgressInft(_T(""), _T(""), iPercent);
			if ( m_pProgressInte->bStopExecute ) { break; }
		}

	  // ******************************
	  // Rating 계산서 출력
	  // ******************************
		m_strBaseBook = _T("Rating Design_Base.xlsx");
		if(pXL->GetExcelType() == XLTYPE::TYPE_LIBXL)
		{
			m_strBaseBook = Excel.GetDLLFilePath(m_strBaseBook, FALSE);
		}
		else
		{
			pXL->SaveAs(pXL->GetBookName(), strPath);//* 출력파일 저쟝..
			if(!pXL->Open(m_strBasePath + m_strBaseBook)) return;  //* base2 열기
			pXL->Open(strPath);                         //* 저장된 출력파일 열기
			pXL->SetActiveBook(pXL->GetBookName());
		}

		nChapterNum += 1;
		for ( i=0; i<aRFRptData.GetSize(); ++i )  // 각 절점별 출력.
		{
			CString strSheetName;
			RF_RES_RF_KSCE_PSC_D &ResData = aRFRptData[i];

			strSheetName.Format(_T("%d.내하력평가(%d_%s)"), nChapterNum, ResData.nElem, GetStringPositionRating(ResData.nIJ));

			if ( pXL->IsExistSheetName(strSheetName) )
			{
				strSheetName = pXL->GetMaximumSheetName(strSheetName) + _T("_New");
			}

			pXL->AddSheet2(strSheetName);
			pXL->SetActiveSheet(strSheetName);
			pXL->SetStandardFont(_T("맑은 고딕"));
			pXL->SetRowHeight(15);
			pXL->SetMargin(13, 12, 20, 17);
			pXL->SetColumnWidth(0, 0, 0, 31, 1.9);  // 28->74
			
			// 1. Rating 검토
			m_strBaseSheet = _T("RC & PSC Rating_Base");
			Print_Detail_PSCRating(pXL, ResData, strSheetName);

			// 2. force 검토 - max,min 출력
			if ( RakrD.bPrintParm[0] )
			{
				strSheetName.Format(_T("참조>단면력(%d_%s)"), ResData.nElem, GetStringPositionRating(ResData.nIJ));

				if ( pXL->IsExistSheetName(strSheetName) )
				{
					strSheetName = pXL->GetMaximumSheetName(strSheetName) + _T("_New");
				}

				pXL->AddSheet2(strSheetName);
				pXL->SetActiveSheet(strSheetName);
				pXL->SetStandardFont(_T("맑은 고딕"));
				pXL->SetRowHeight(15);
				pXL->SetMargin(13, 12, 20, 17);
				pXL->SetColumnWidth(0, 0, 0, 31, 1.9);  // 28->74

				m_strBaseSheet = _T("Force");
				Print_Detail_Force(pXL, ResData, strSheetName);
			}

			iCountProgressBar++;
			iPercent = (int)(100*(iCountProgressBar)/nTotalCount);
			Update_ProgressInft(_T(""), _T(""), iPercent);
			if ( m_pProgressInte->bStopExecute ) { break; }
		}
	}
	catch ( ... )
	{
		bSuccess = FALSE;
		delete pXL;
		Update_ProgressInft(_T(""), _T(""), 100);
		AfxMessageBox(_LS(IDS_DGN_PSC_XLPRINT_CALC_REPORT)); // _T("Could not write calculation report")
		return;
	}
	// 정리
	Update_ProgressInft(_T(""), _T(""), 100);
	if(pXL->GetExcelType() == XLTYPE::TYPE_LIBXL)
	{
		pXL->GetBook()->setActiveSheet(0);
	}
	pXL->EndExcel(strPath);
	delete pXL;

	// 파일 보여주기
	ShellExecute(NULL, _T("open"), (strPath), NULL, NULL, SW_SHOWNORMAL);
}

BOOL CRating_RKMC_XLOut::Print_Detail_PSCRating(IExcel* pExcel, RF_RES_RF_KSCE_PSC_D& ResData, CString strTitle)
{
	int     stRow = 1;
	CString RptBlock;
	// 내하율 산정 
	RptBlock = _T("RF_GROUP_1") ;   SetValue_RF_GROUP_1(pExcel, RptBlock, ResData, strTitle, stRow);
	if ( ResData.nEstimatedMethod == 0 )
	{
		RptBlock = _T("RF_GROUP_2") ;   SetValue_RF_GROUP_2(pExcel, RptBlock, ResData, stRow);
	}
	else
	{
		RptBlock = _T("RF_GROUP_3") ;   SetValue_RF_GROUP_3(pExcel, RptBlock, ResData, stRow);
	}
	RptBlock = _T("RF_GROUP_4") ;   SetValue_RF_GROUP_4(pExcel, RptBlock, ResData, stRow);

	return TRUE;
}


void CRating_RKMC_XLOut::SetValue_RF_GROUP_1(IExcel* pExcel, CString& RptBlock, RF_RES_RF_KSCE_PSC_D& RptData, CString strTitle, int& stRow)
{
	int rowCnt = CopyRange(pExcel, RptBlock, stRow);
	short Row_Ref = 0, Col_Ref = 0;
	pExcel->GetCellIDByName(m_strBaseBook, m_strBaseSheet, RptBlock, Row_Ref, Col_Ref);

	PrintCellData(pExcel, m_strBaseBook, m_strBaseSheet, _T("RF_Title"), strTitle, Row_Ref, Col_Ref, stRow);

	PrintCellData(pExcel, m_strBaseBook, m_strBaseSheet, _T("RF_ELEM_RF"), RptData.nElem, Row_Ref, Col_Ref, stRow);
	PrintCellData(pExcel, m_strBaseBook, m_strBaseSheet, _T("RF_PART_RF"), GetStringPositionRating(RptData.nIJ), Row_Ref, Col_Ref, stRow);

	CString strLcom0 = RptData.Force[0].strLcom;
	CString strLcom1 = RptData.Force[1].strLcom;
	PrintCellData(pExcel, m_strBaseBook, m_strBaseSheet, _T("RF_LCOM_TT"), SetStringEmpty2Hipun(strLcom0), Row_Ref, Col_Ref, stRow);
	PrintCellData(pExcel, m_strBaseBook, m_strBaseSheet, _T("RF_LCOM_BC"), SetStringEmpty2Hipun(strLcom1), Row_Ref, Col_Ref, stRow);

	PrintCellData(pExcel, m_strBaseBook, m_strBaseSheet, _T("RF_RF_Fa_TT"), RptData.Force[0].dphiMn * m_dFactorMoment, Row_Ref, Col_Ref, stRow);
	PrintCellData(pExcel, m_strBaseBook, m_strBaseSheet, _T("RF_RF_Fa_BC"), RptData.Force[1].dphiMn * m_dFactorMoment, Row_Ref, Col_Ref, stRow);

	PrintCellData(pExcel, m_strBaseBook, m_strBaseSheet, _T("RF_Fd_TT"), RptData.Force[0].dMd * m_dFactorMoment, Row_Ref, Col_Ref, stRow);
	PrintCellData(pExcel, m_strBaseBook, m_strBaseSheet, _T("RF_Fd_BC"), RptData.Force[1].dMd * m_dFactorMoment, Row_Ref, Col_Ref, stRow);

	PrintCellData(pExcel, m_strBaseBook, m_strBaseSheet, _T("RF_Fl_TT"), RptData.Force[0].dMl * m_dFactorMoment, Row_Ref, Col_Ref, stRow);
	PrintCellData(pExcel, m_strBaseBook, m_strBaseSheet, _T("RF_Fl_BC"), RptData.Force[1].dMl * m_dFactorMoment, Row_Ref, Col_Ref, stRow);

	PrintCellData(pExcel, m_strBaseBook, m_strBaseSheet, _T("RF_1i_TT"), RptData.Force[0].dImpactFactor, Row_Ref, Col_Ref, stRow);
	PrintCellData(pExcel, m_strBaseBook, m_strBaseSheet, _T("RF_1i_BC"), RptData.Force[1].dImpactFactor, Row_Ref, Col_Ref, stRow);

	PrintCellData(pExcel, m_strBaseBook, m_strBaseSheet, _T("RF_RF_RF_TT"), RptData.Force[0].dRF, Row_Ref, Col_Ref, stRow);
	PrintCellData(pExcel, m_strBaseBook, m_strBaseSheet, _T("RF_RF_RF_BC"), RptData.Force[1].dRF, Row_Ref, Col_Ref, stRow);

	stRow+=(rowCnt+1);
}


void CRating_RKMC_XLOut::SetValue_RF_GROUP_2(IExcel* pExcel, CString& RptBlock, RF_RES_RF_KSCE_PSC_D& RptData, int& stRow)
{
	int rowCnt = CopyRange(pExcel, RptBlock, stRow);
	short Row_Ref = 0, Col_Ref = 0;
	pExcel->GetCellIDByName(m_strBaseBook, m_strBaseSheet, RptBlock, Row_Ref, Col_Ref);

	PrintCellData(pExcel, m_strBaseBook, m_strBaseSheet, _T("RF_ELEM_Ks"), RptData.nElem, Row_Ref, Col_Ref, stRow);
	PrintCellData(pExcel, m_strBaseBook, m_strBaseSheet, _T("RF_PART_Ks"), GetStringPositionRating(RptData.nIJ), Row_Ref, Col_Ref, stRow);

	CString strLcom0 = RptData.Delta[0].strLcom;
	CString strLcom1 = RptData.Delta[1].strLcom;
	PrintCellData(pExcel, m_strBaseBook, m_strBaseSheet, _T("RF_LCOM_Ks_TT"), SetStringEmpty2Hipun(strLcom0), Row_Ref, Col_Ref, stRow);
	PrintCellData(pExcel, m_strBaseBook, m_strBaseSheet, _T("RF_LCOM_Ks_BC"), SetStringEmpty2Hipun(strLcom1), Row_Ref, Col_Ref, stRow);

	PrintCellData(pExcel, m_strBaseBook, m_strBaseSheet, _T("RF_Calc_Del_Ks_TT"), RptData.Delta[0].dCalcDelta, Row_Ref, Col_Ref, stRow);
	PrintCellData(pExcel, m_strBaseBook, m_strBaseSheet, _T("RF_Calc_Del_LCOM_Ks_BC"), RptData.Delta[1].dCalcDelta, Row_Ref, Col_Ref, stRow);

	PrintCellData(pExcel, m_strBaseBook, m_strBaseSheet, _T("RF_Esti_Del_LCOM_Ks_TT"), RptData.Delta[0].dEstiDelta, Row_Ref, Col_Ref, stRow);
	PrintCellData(pExcel, m_strBaseBook, m_strBaseSheet, _T("RF_Esti_Del_LCOM_Ks_BC"), RptData.Delta[1].dEstiDelta, Row_Ref, Col_Ref, stRow);

	PrintCellData(pExcel, m_strBaseBook, m_strBaseSheet, _T("RF_Del_Ratio_TT"), RptData.Delta[0].dRatioDelta, Row_Ref, Col_Ref, stRow);
	PrintCellData(pExcel, m_strBaseBook, m_strBaseSheet, _T("RF_Del_Ratio_BC"), RptData.Delta[1].dRatioDelta, Row_Ref, Col_Ref, stRow);

	PrintCellData(pExcel, m_strBaseBook, m_strBaseSheet, _T("RF_Ks_Calc_i_TT"), RptData.Delta[0].dCalci, Row_Ref, Col_Ref, stRow);
	PrintCellData(pExcel, m_strBaseBook, m_strBaseSheet, _T("RF_Ks_Calc_i_BC"), RptData.Delta[1].dCalci, Row_Ref, Col_Ref, stRow);

	PrintCellData(pExcel, m_strBaseBook, m_strBaseSheet, _T("RF_Ks_Esti_i_TT"), RptData.Delta[0].dEstii, Row_Ref, Col_Ref, stRow);
	PrintCellData(pExcel, m_strBaseBook, m_strBaseSheet, _T("RF_Ks_Esti_i_BC"), RptData.Delta[1].dEstii, Row_Ref, Col_Ref, stRow);

	PrintCellData(pExcel, m_strBaseBook, m_strBaseSheet, _T("RF_Delta_i_Ratio_TT"), RptData.Delta[0].dRatioi, Row_Ref, Col_Ref, stRow);
	PrintCellData(pExcel, m_strBaseBook, m_strBaseSheet, _T("RF_Delta_i_Ratio_BC"), RptData.Delta[1].dRatioi, Row_Ref, Col_Ref, stRow);

	PrintCellData(pExcel, m_strBaseBook, m_strBaseSheet, _T("RF_Del_Ks_TT"), RptData.Delta[0].dKs, Row_Ref, Col_Ref, stRow);
	PrintCellData(pExcel, m_strBaseBook, m_strBaseSheet, _T("RF_Del_Ks_BC"), RptData.Delta[1].dKs, Row_Ref, Col_Ref, stRow);

	stRow+=(rowCnt+1);
}


void CRating_RKMC_XLOut::SetValue_RF_GROUP_3(IExcel* pExcel, CString& RptBlock, RF_RES_RF_KSCE_PSC_D& RptData, int& stRow)
{
	int rowCnt = CopyRange(pExcel, RptBlock, stRow);
	short Row_Ref = 0, Col_Ref = 0;
	pExcel->GetCellIDByName(m_strBaseBook, m_strBaseSheet, RptBlock, Row_Ref, Col_Ref);

	PrintCellData(pExcel, m_strBaseBook, m_strBaseSheet, _T("RF_ELEM_E_Ks"), RptData.nElem, Row_Ref, Col_Ref, stRow);
	PrintCellData(pExcel, m_strBaseBook, m_strBaseSheet, _T("RF_PART_E_Ks"), GetStringPositionRating(RptData.nIJ), Row_Ref, Col_Ref, stRow);

	CString strLcom0 = RptData.Ewips[0].strLcom;
	CString strLcom1 = RptData.Ewips[1].strLcom;
	PrintCellData(pExcel, m_strBaseBook, m_strBaseSheet, _T("RF_LCOM_E_Ks_TT"), SetStringEmpty2Hipun(strLcom0), Row_Ref, Col_Ref, stRow);
	PrintCellData(pExcel, m_strBaseBook, m_strBaseSheet, _T("RF_LCOM_E_Ks_BC"), SetStringEmpty2Hipun(strLcom1), Row_Ref, Col_Ref, stRow);

	PrintCellData(pExcel, m_strBaseBook, m_strBaseSheet, _T("RF_E_Calc_Ks_TT"), RptData.Ewips[0].dCalcEwips, Row_Ref, Col_Ref, stRow);
	PrintCellData(pExcel, m_strBaseBook, m_strBaseSheet, _T("RF_E_Calc_LCOM_Ks_BC"), RptData.Ewips[1].dCalcEwips, Row_Ref, Col_Ref, stRow);

	PrintCellData(pExcel, m_strBaseBook, m_strBaseSheet, _T("RF_E_Esti_Ks_TT"), RptData.Ewips[0].dEstiEwips, Row_Ref, Col_Ref, stRow);
	PrintCellData(pExcel, m_strBaseBook, m_strBaseSheet, _T("RF_E_Esti_Ks_BC"), RptData.Ewips[1].dEstiEwips, Row_Ref, Col_Ref, stRow);

	PrintCellData(pExcel, m_strBaseBook, m_strBaseSheet, _T("RF_E_Ratio_TT"), RptData.Ewips[0].dRatioEwips, Row_Ref, Col_Ref, stRow);
	PrintCellData(pExcel, m_strBaseBook, m_strBaseSheet, _T("RF_E_Ratio_BC"), RptData.Ewips[1].dRatioEwips, Row_Ref, Col_Ref, stRow);

	PrintCellData(pExcel, m_strBaseBook, m_strBaseSheet, _T("RF_E_Ks_Calc_i_TT"), RptData.Ewips[0].dCalci, Row_Ref, Col_Ref, stRow);
	PrintCellData(pExcel, m_strBaseBook, m_strBaseSheet, _T("RF_E_Ks_Calc_i_BC"), RptData.Ewips[1].dCalci, Row_Ref, Col_Ref, stRow);

	PrintCellData(pExcel, m_strBaseBook, m_strBaseSheet, _T("RF_E_Ks_Esti_i_TT"), RptData.Ewips[0].dEstii, Row_Ref, Col_Ref, stRow);
	PrintCellData(pExcel, m_strBaseBook, m_strBaseSheet, _T("RF_E_Ks_Esti_i_BC"), RptData.Ewips[1].dEstii, Row_Ref, Col_Ref, stRow);

	PrintCellData(pExcel, m_strBaseBook, m_strBaseSheet, _T("RF_E_i_Ratio_TT"), RptData.Ewips[0].dRatioi, Row_Ref, Col_Ref, stRow);
	PrintCellData(pExcel, m_strBaseBook, m_strBaseSheet, _T("RF_E_i_Ratio_BC"), RptData.Ewips[1].dRatioi, Row_Ref, Col_Ref, stRow);

	PrintCellData(pExcel, m_strBaseBook, m_strBaseSheet, _T("RF_E_Ks_TT"), RptData.Ewips[0].dKs, Row_Ref, Col_Ref, stRow);
	PrintCellData(pExcel, m_strBaseBook, m_strBaseSheet, _T("RF_E_Ks_BC"), RptData.Ewips[1].dKs, Row_Ref, Col_Ref, stRow);

	stRow+=(rowCnt+1);
}


void CRating_RKMC_XLOut::SetValue_RF_GROUP_4(IExcel* pExcel, CString& RptBlock, RF_RES_RF_KSCE_PSC_D& RptData, int& stRow)
{
	int rowCnt = CopyRange(pExcel, RptBlock, stRow);
	short Row_Ref = 0, Col_Ref = 0;
	pExcel->GetCellIDByName(m_strBaseBook, m_strBaseSheet, RptBlock, Row_Ref, Col_Ref);

	// 공용내하력 산정
	PrintCellData(pExcel, m_strBaseBook, m_strBaseSheet, _T("RF_ELEM_P"), RptData.nElem, Row_Ref, Col_Ref, stRow);
	PrintCellData(pExcel, m_strBaseBook, m_strBaseSheet, _T("RF_PART_P"), GetStringPositionRating(RptData.nIJ), Row_Ref, Col_Ref, stRow);

	CString strLcom0 = RptData.LoadP[0].strLcom;
	CString strLcom1 = RptData.LoadP[1].strLcom;
	PrintCellData(pExcel, m_strBaseBook, m_strBaseSheet, _T("RF_LCOM_P_TT"), SetStringEmpty2Hipun(strLcom0), Row_Ref, Col_Ref, stRow);
	PrintCellData(pExcel, m_strBaseBook, m_strBaseSheet, _T("RF_LCOM_P_BC"), SetStringEmpty2Hipun(strLcom1), Row_Ref, Col_Ref, stRow);

	PrintCellData(pExcel, m_strBaseBook, m_strBaseSheet, _T("RF_PF_P_TT"), RptData.Force[0].dRF, Row_Ref, Col_Ref, stRow);
	PrintCellData(pExcel, m_strBaseBook, m_strBaseSheet, _T("RF_PF_P_BC"), RptData.Force[1].dRF, Row_Ref, Col_Ref, stRow);


	CString strTmp;
	CString str;
	for ( int i = 0; i<2; ++i )
	{
		if ( RptData.LoadP[i].strDesignPr.IsEmpty() == FALSE )
		{
			CString strDesignPr = RptData.LoadP[i].strDesignPr;

			int nPosi = strDesignPr.Find(_T("-"));
			str = RptData.LoadP[i].strDesignPr.Left(nPosi);
			PrintCellData(pExcel, m_strBaseBook, m_strBaseSheet, _T("RF_PR"), SetStringEmpty2Hipun(strDesignPr), Row_Ref, Col_Ref, stRow);
			break;
		}
	}

	CString strP00 = RptData.LoadP[0].strP0;
	CString strP01 = RptData.LoadP[1].strP0;
	PrintCellData(pExcel, m_strBaseBook, m_strBaseSheet, _T("RF_P0_TT"), SetStringEmpty2Hipun(strP00), Row_Ref, Col_Ref, stRow);
	PrintCellData(pExcel, m_strBaseBook, m_strBaseSheet, _T("RF_P0_BC"), SetStringEmpty2Hipun(strP01), Row_Ref, Col_Ref, stRow);


	PrintCellData(pExcel, m_strBaseBook, m_strBaseSheet, _T("RF_Ks_TT"), RptData.LoadP[0].dKs, Row_Ref, Col_Ref, stRow);
	PrintCellData(pExcel, m_strBaseBook, m_strBaseSheet, _T("RF_Ks_BC"), RptData.LoadP[1].dKs, Row_Ref, Col_Ref, stRow);


	if ( RptData.LoadP[0].dP != 0.0 ) strTmp.Format(_T("%s-%.3f"), str, RptData.LoadP[0].dP);
	else                            strTmp = _T("-");
	PrintCellData(pExcel, m_strBaseBook, m_strBaseSheet, _T("RF_P_TT"), strTmp, Row_Ref, Col_Ref, stRow);

	if ( RptData.LoadP[3].dP != 0.0 ) strTmp.Format(_T("%s-%.3f"), str, RptData.LoadP[1].dP);
	else                            strTmp = _T("-");
	PrintCellData(pExcel, m_strBaseBook, m_strBaseSheet, _T("RF_P_BC"), strTmp, Row_Ref, Col_Ref, stRow);

	PrintCellData(pExcel, m_strBaseBook, m_strBaseSheet, _T("RF_P_OK_TT"), GetStringOKNG(RptData.LoadP[0].nOK), Row_Ref, Col_Ref, stRow);
	PrintCellData(pExcel, m_strBaseBook, m_strBaseSheet, _T("RF_P_OK_BC"), GetStringOKNG(RptData.LoadP[1].nOK), Row_Ref, Col_Ref, stRow);


	// 안전성 평가 등급
	PrintCellData(pExcel, m_strBaseBook, m_strBaseSheet, _T("RF_Fa_TT"), RptData.Level[0].dfa * m_dFactorMoment, Row_Ref, Col_Ref, stRow);
	PrintCellData(pExcel, m_strBaseBook, m_strBaseSheet, _T("RF_Fa_BC"), RptData.Level[1].dfa * m_dFactorMoment, Row_Ref, Col_Ref, stRow);

	PrintCellData(pExcel, m_strBaseBook, m_strBaseSheet, _T("RF_Fdl_TT"), RptData.Level[0].dfdl * m_dFactorMoment, Row_Ref, Col_Ref, stRow);
	PrintCellData(pExcel, m_strBaseBook, m_strBaseSheet, _T("RF_Fdl_BC"), RptData.Level[1].dfdl * m_dFactorMoment, Row_Ref, Col_Ref, stRow);

	PrintCellData(pExcel, m_strBaseBook, m_strBaseSheet, _T("RF_FaFdl_TT"), RptData.Level[0].dRatio, Row_Ref, Col_Ref, stRow);
	PrintCellData(pExcel, m_strBaseBook, m_strBaseSheet, _T("RF_FaFdl_BC"), RptData.Level[1].dRatio, Row_Ref, Col_Ref, stRow);

	CString strLevel0 = RptData.Level[0].strLevel;
	CString strLevel1 = RptData.Level[1].strLevel;
	PrintCellData(pExcel, m_strBaseBook, m_strBaseSheet, _T("RF_LEVEL_TT"), SetStringEmpty2Hipun(strLevel0), Row_Ref, Col_Ref, stRow);
	PrintCellData(pExcel, m_strBaseBook, m_strBaseSheet, _T("RF_LEVEL_BC"), SetStringEmpty2Hipun(strLevel1), Row_Ref, Col_Ref, stRow);

	stRow+=(rowCnt+1);
}



BOOL CRating_RKMC_XLOut::Print_Detail_Force(IExcel* pExcel, RF_RES_RF_KSCE_PSC_D& ResData, CString strTitle)
{
	int stRow = 1;
	CString RptBlock;

	// Print title
	RptBlock = _T("Title_Force_Rating");
	int rowCnt = CopyRange(pExcel, RptBlock, stRow);
	short Row_Ref = 0, Col_Ref = 0;
	pExcel->GetCellIDByName(m_strBaseBook, m_strBaseSheet, RptBlock, Row_Ref, Col_Ref);
	PrintCellData(pExcel, m_strBaseBook, m_strBaseSheet, _T("RF_Title"), strTitle, Row_Ref, Col_Ref, stRow);
	stRow+=rowCnt;
	//

	RptBlock = _T("RF_FORCE_TITLE");   OnlyCopyRange(pExcel, RptBlock, stRow); // Title
	RptBlock = _T("RF_FORCE_GROUP");   OnlyCopyRange(pExcel, RptBlock, stRow); // 테이블 이름
	--stRow;
	for ( int i = 0; i<ResData.arForce.GetSize(); ++i )
	{
		RptBlock = _T("RF_FORCE_COMP");   SetValue_RF_FORCE_COMP(pExcel, RptBlock, ResData, ResData.arForce[i], stRow);
	}

	return TRUE;
}


void CRating_RKMC_XLOut::SetValue_RF_FORCE_COMP(IExcel* pExcel, CString& RptBlock, RF_RES_RF_KSCE_PSC_D &RptBaseD, RF_RES_KS_FORCE_D &RptData, int& stRow)
{
	int rowCnt = CopyRange(pExcel, RptBlock, stRow);
	short Row_Ref = 0, Col_Ref = 0;
	pExcel->GetCellIDByName(m_strBaseBook, m_strBaseSheet, RptBlock, Row_Ref, Col_Ref);

	PrintCellData(pExcel, m_strBaseBook, m_strBaseSheet, _T("F_N"), RptBaseD.nElem, Row_Ref, Col_Ref, stRow);
	PrintCellData(pExcel, m_strBaseBook, m_strBaseSheet, _T("F_IJ"), GetStringPositionRating(RptBaseD.nIJ), Row_Ref, Col_Ref, stRow);
	CString strName = RptData.ForceLL.strName;
	PrintCellData(pExcel, m_strBaseBook, m_strBaseSheet, _T("F_LCB"), strName, Row_Ref, Col_Ref, stRow);

	PrintCellData(pExcel, m_strBaseBook, m_strBaseSheet, _T("F_DL_FX"), RptData.ForceDL.dForce[0] * m_dFactorForce, Row_Ref, Col_Ref, stRow);
	PrintCellData(pExcel, m_strBaseBook, m_strBaseSheet, _T("F_DL_FY"), RptData.ForceDL.dForce[1] * m_dFactorForce, Row_Ref, Col_Ref, stRow);
	PrintCellData(pExcel, m_strBaseBook, m_strBaseSheet, _T("F_DL_FZ"), RptData.ForceDL.dForce[2] * m_dFactorForce, Row_Ref, Col_Ref, stRow);
	PrintCellData(pExcel, m_strBaseBook, m_strBaseSheet, _T("F_DL_MX"), RptData.ForceDL.dForce[3] * m_dFactorMoment, Row_Ref, Col_Ref, stRow);
	PrintCellData(pExcel, m_strBaseBook, m_strBaseSheet, _T("F_DL_MY"), RptData.ForceDL.dForce[4] * m_dFactorMoment, Row_Ref, Col_Ref, stRow);
	PrintCellData(pExcel, m_strBaseBook, m_strBaseSheet, _T("F_DL_MZ"), RptData.ForceDL.dForce[5] * m_dFactorMoment, Row_Ref, Col_Ref, stRow);

	PrintCellData(pExcel, m_strBaseBook, m_strBaseSheet, _T("F_LL_FX"), RptData.ForceLL.dForce[0] * m_dFactorForce, Row_Ref, Col_Ref, stRow);
	PrintCellData(pExcel, m_strBaseBook, m_strBaseSheet, _T("F_LL_FY"), RptData.ForceLL.dForce[1] * m_dFactorForce, Row_Ref, Col_Ref, stRow);
	PrintCellData(pExcel, m_strBaseBook, m_strBaseSheet, _T("F_LL_FZ"), RptData.ForceLL.dForce[2] * m_dFactorForce, Row_Ref, Col_Ref, stRow);
	PrintCellData(pExcel, m_strBaseBook, m_strBaseSheet, _T("F_LL_MX"), RptData.ForceLL.dForce[3] * m_dFactorMoment, Row_Ref, Col_Ref, stRow);
	PrintCellData(pExcel, m_strBaseBook, m_strBaseSheet, _T("F_LL_MY"), RptData.ForceLL.dForce[4] * m_dFactorMoment, Row_Ref, Col_Ref, stRow);
	PrintCellData(pExcel, m_strBaseBook, m_strBaseSheet, _T("F_LL_MZ"), RptData.ForceLL.dForce[5] * m_dFactorMoment, Row_Ref, Col_Ref, stRow);

	PrintCellData(pExcel, m_strBaseBook, m_strBaseSheet, _T("F_CF"), GetLcomType(RptData.ForceLL.nType), Row_Ref, Col_Ref, stRow);

	stRow+=(rowCnt);
}

CString CRating_RKMC_XLOut::SetGetPSCReportBaseFile(int nDgnCode)
{
	CString strBaseBook;
	switch ( nDgnCode )
	{
	case KSCE_USD10_RATING:
	case KSCE_RAIL_USD11_RATING:
		strBaseBook = _T("PSC_KSCE_10_base_ver01.dll");
		break;
	case KSCE_LSD15_RATING:
		strBaseBook = _T("PSC_KR_LSD_Base.xlsx"); // _T("PSC_COMPO_KSCE_LSD12_base_ver01.dll");
		break;
	default:
		strBaseBook = _T("PSC_KSCE_10_base_ver01.dll");
		ASSERT(0);
		break;
	}
	return strBaseBook;
}