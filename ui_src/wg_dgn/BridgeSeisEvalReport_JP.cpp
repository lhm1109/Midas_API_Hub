#include "stdafx.h"
#include <locale>
#include <string>
#include <fstream>
#include <codecvt>
#include "wg_dgn.h"
#include "BridgeSeisEvalReport_JP.h"

#include "..\..\Include\LibXL\libxl.h"
#include "..\..\DgnEngine\idesign\DGN_lib\ExcelGenerator.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl2.h"
#include "..\wg_db\DB_QUERY_SEIS_CVL_JP.h"

#include "BrdgSeisEval_JP.h"
#include "DgnExcelPrintDlg.h"
#include "WriteSeisEvalReport_JP.h"
#include "WriteSeisEvalGraph.h"

using namespace dgn::lib;

iBridgeSeisEvalReport_JP::iBridgeSeisEvalReport_JP()
{
	m_pWriteConditions = NULL;
	m_pWriteSummary = NULL;
	m_pWriteMaxDisp = NULL;
	m_pWriteResidualDisp = NULL;
	m_pWriteFlexure = NULL;
	m_pWriteShearSus = NULL;
	m_pWriteShearSusResult = NULL;
	m_pWriteShearSuc = NULL;
	m_pWriteShearSucResult = NULL;
	m_pWriteDispPlastRatio = NULL;
	m_pWriteCurvatureRC = NULL;
	m_pWriteCurvatureST = NULL;
	m_pWriteShearPs = NULL;
	m_pWriteShearPsResult = NULL;
	m_pWriteRebarReduction = NULL;
	m_pWritePierBottom = NULL;
	m_pWriteBearings = NULL;
}

iBridgeSeisEvalReport_JP::~iBridgeSeisEvalReport_JP()
{
	if (m_pWriteConditions != NULL) { delete m_pWriteConditions; m_pWriteConditions = NULL; }
	if (m_pWriteSummary != NULL) { delete m_pWriteSummary; m_pWriteSummary = NULL; }
	if (m_pWriteMaxDisp != NULL) { delete m_pWriteMaxDisp; m_pWriteMaxDisp = NULL; }
	if (m_pWriteResidualDisp != NULL) { delete m_pWriteResidualDisp; m_pWriteResidualDisp = NULL; }
	if (m_pWriteFlexure != NULL) { delete m_pWriteFlexure; m_pWriteFlexure = NULL; }
	if (m_pWriteShearSus != NULL) { delete m_pWriteShearSus; m_pWriteShearSus = NULL; }
	if (m_pWriteShearSusResult != NULL) { delete m_pWriteShearSusResult; m_pWriteShearSusResult = NULL; }
	if (m_pWriteShearSuc != NULL) { delete m_pWriteShearSuc; m_pWriteShearSuc = NULL; }
	if (m_pWriteShearSucResult != NULL) { delete m_pWriteShearSucResult; m_pWriteShearSucResult = NULL; }
	if (m_pWriteDispPlastRatio != NULL) { delete m_pWriteDispPlastRatio; m_pWriteDispPlastRatio = NULL; }
	if (m_pWriteCurvatureRC != NULL) { delete m_pWriteCurvatureRC; m_pWriteCurvatureRC = NULL; }
	if (m_pWriteCurvatureST != NULL) { delete m_pWriteCurvatureST; m_pWriteCurvatureST = NULL; }
	if (m_pWriteShearPs != NULL) { delete m_pWriteShearPs; m_pWriteShearPs = NULL; }
	if (m_pWriteShearPsResult != NULL) { delete m_pWriteShearPsResult; m_pWriteShearPsResult = NULL; }
	if (m_pWriteRebarReduction != NULL) { delete m_pWriteRebarReduction; m_pWriteRebarReduction = NULL; }
	if (m_pWritePierBottom != NULL) { delete m_pWritePierBottom; m_pWritePierBottom = NULL; }
	if (m_pWriteBearings != NULL) { delete m_pWriteBearings; m_pWriteBearings = NULL; }
}

#pragma region // CBridgeSeisEvalReport_JP
CBridgeSeisEvalReport_JP::CBridgeSeisEvalReport_JP(CDgnProgressDlg* pProgress, CString strPath)
{
	m_strPath = strPath;
	m_pProgress = pProgress;

	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	if (pDoc == NULL) return;

	T_SIDC_D dSidc;
	dSidc.Initialize();
	pDoc->m_pAttrCtrl2->GetQSidc()->Get(dSidc);

	CString strBasePath, strBaseBook;
	m_pXL = SetExcel(dSidc.nDesignCode, strBasePath, strBaseBook);
	if (m_pXL == nullptr) return;

	if (dSidc.nDesignCode == BrdgSE_CodeJP::JROAD5_H14)
		m_pReport = new CBridgeSeisEvalReport_JROAD5_H14(m_pXL, strBasePath, strBaseBook);
	else if (dSidc.nDesignCode == BrdgSE_CodeJP::JROAD5_H24)
		m_pReport = new CBridgeSeisEvalReport_JROAD5_H24(m_pXL, strBasePath, strBaseBook);
	else if (dSidc.nDesignCode == BrdgSE_CodeJP::JROAD5_H29)
		m_pReport = new CBridgeSeisEvalReport_JROAD5_H29(m_pXL, strBasePath, strBaseBook);

	m_pGraphCtrl = new CGraphControl(m_strPath);
}

CBridgeSeisEvalReport_JP::~CBridgeSeisEvalReport_JP(void)
{
	if (m_pReport != NULL) { delete m_pReport; m_pReport = NULL; }
	if (m_pGraphCtrl != NULL) {
		delete m_pGraphCtrl;
		m_pGraphCtrl = NULL;
	}
}

IExcel* CBridgeSeisEvalReport_JP::SetExcel(const int& nDesignCode, OUT CString& strBasePath, OUT CString& strBaseBook)
{
	strBasePath = m_pDoc->GetProgramPath() + _T("Excel Base File\\");
	if(nDesignCode == BrdgSE_CodeJP::JROAD5_H29)
		strBaseBook = _T("BrdgSeisEvalReportH29_JP.xlsx");
	else
		strBaseBook = _T("BrdgSeisEvalReportH14_JP.xlsx");

	CExcelGenerator Excel(_T(""), XLTYPE::TYPE_LIBXL);
	CString strFileName = Excel.GetDLLFilePath(strBaseBook, FALSE);

	IExcel* pXL = Excel.Generate(strFileName);
	strBaseBook = strFileName; // 경로 포함.

	return pXL;
}

void CBridgeSeisEvalReport_JP::WriteReport(const SDgnExcelPrintOpt* pPrintOpt)
{
	// Code 단위계 변환
	CUnitConversion unit(D_UNITSYS_FORCE_INDEX_KN, D_UNITSYS_LENGTH_INDEX_M);

	m_pReport->CreateInst(m_pProgress, pPrintOpt);
	m_pReport->AddStep(m_pProgress);

	if (m_pGraphCtrl->IsExcel())
	{
		m_pGraphCtrl->CreateGraphControl(m_pProgress);
		m_pGraphCtrl->AddStepGraph();
	}

	auto l_delete_XL = [&]()
		{
			if (m_pXL != nullptr)
			{
				delete m_pXL;
				m_pXL = nullptr;
			}
		};

	try
	{
		m_pReport->Write(m_pProgress);

		if (m_pGraphCtrl->IsGraph() == FALSE)
		{
			while (m_pProgress->m_nCurrentStep < m_pProgress->m_nTotalSteps) {
				if (m_pProgress->NextStep() == FALSE) throw FALSE;
			}
		}
	}
	catch (BOOL bStop)
	{
		ASSERT(bStop == FALSE);
		l_delete_XL();
		return;
	}
}

void CBridgeSeisEvalReport_JP::PrintOut(const SDgnExcelPrintOpt* pPrintOpt)
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	if (pDoc == NULL) return;
	if (pPrintOpt == NULL) return;

	WriteReport(pPrintOpt);

	m_pXL->EndExcel(m_strPath);

	if(m_pGraphCtrl->IsGraph())
		WriteGraph();

	ShellExecute(NULL, _T("open"), m_strPath, NULL, NULL, SW_SHOWNORMAL);
}

void CBridgeSeisEvalReport_JP::WriteGraph()
{
	Sleep(1000);

	// Graph
	m_pGraphCtrl->WriteGraph();

	while (m_pProgress->m_nCurrentStep < m_pProgress->m_nTotalSteps) {
		if (m_pProgress->NextStep() == FALSE) throw FALSE;
	}
}

std::string ConvertWCharToString(const wchar_t* wcharText) 
{
	std::wstring wideString(wcharText);

	int utf8Length = WideCharToMultiByte(CP_UTF8, 0, wideString.c_str(), -1, nullptr, 0, nullptr, nullptr);
	if (utf8Length == 0) {
		return "";
	}

	std::string utf8Str(utf8Length - 1, '\0');

	if (WideCharToMultiByte(CP_UTF8, 0, wideString.c_str(), -1, &utf8Str[0], utf8Length, nullptr, nullptr) == 0) {
		return "";
	}

	return utf8Str;
}

void CBridgeSeisEvalReport_JP::PrintOutAll(OUT CArray<CString, CString&>& arOutFileName, OUT CString& strResultMsg)
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	if (pDoc == NULL) return;

	SDgnExcelPrintOpt printOpt;
	pDoc->m_pAttrCtrl2->GetQSipa()->GetKeyList(printOpt.aPrintSipaK);

	WriteReport(&printOpt);

	CStringArray aSheets;
	m_pXL->GetSheetsName(aSheets);

	std::wofstream csvFile(m_strPath);
	csvFile.imbue(std::locale(std::locale::classic(), new std::codecvt_utf8<wchar_t>)); // UTF-8 설정

	for (int iSheet = 0; iSheet < aSheets.GetCount(); iSheet++)
	{
		const CString& strSheet = aSheets[iSheet];
		libxl::Sheet* pSheet = m_pXL->GetBook()->getSheet(iSheet);
		if (!pSheet) continue;

		// [SheetName]
		csvFile << L"[" << (LPCTSTR)strSheet << L"]\n";

		for (int row = 0; row < pSheet->lastRow(); ++row) {
			for (int col = 0; col < pSheet->lastCol(); ++col) {
				const wchar_t* cellValue = pSheet->readStr(row, col);
				if (cellValue) {
					csvFile << cellValue; // 유니코드 문자열 바로 출력
				}
				if (col < pSheet->lastCol() - 1) {
					csvFile << L","; // 쉼표 추가
				}
			}
			csvFile << L"\n"; // 줄바꿈 추가
		}
	}
	csvFile.close();
	m_pXL->Quit();

	arOutFileName.Add(m_strPath);
	strResultMsg = _T("SUCCESS");
}
#pragma endregion

#pragma region // CBridgeSeisEvalReport_JROAD5_H14
CBridgeSeisEvalReport_JROAD5_H14::CBridgeSeisEvalReport_JROAD5_H14(IExcel* pXL, const CString& strBasePath, const CString& strBaseBook)
{
	m_pXL = pXL;
	m_strBasePath = strBasePath;
	m_strBaseBook = strBaseBook;
}

CBridgeSeisEvalReport_JROAD5_H14::~CBridgeSeisEvalReport_JROAD5_H14(void)
{
}

void CBridgeSeisEvalReport_JROAD5_H14::CreateInst(CDgnProgressDlg* pProgress, const SDgnExcelPrintOpt* pPrintOpt)
{
	m_pWriteConditions		= new CSeisEvalReportConditions		(m_pXL, pProgress, m_strBasePath, m_strBaseBook, pPrintOpt);
// 	if (pPrintOpt->summarySize == SDgnExcelPrintOpt::kA3)
// 		m_pWriteSummary = new CSeisEvalReportSummary_A3(m_pXL, pProgress, m_strBasePath, m_strBaseBook, pPrintOpt);
// 	else if(pPrintOpt->summarySize == SDgnExcelPrintOpt::kA4)
// 		m_pWriteSummary = new CSeisEvalReportSummary_A4(m_pXL, pProgress, m_strBasePath, m_strBaseBook, pPrintOpt);
	m_pWriteSummary			= new CSeisEvalReportSummary_A3		(m_pXL, pProgress, m_strBasePath, m_strBaseBook, pPrintOpt);
	m_pWriteMaxDisp			= new CSeisEvalReportMaxDisp		(m_pXL, pProgress, m_strBasePath, m_strBaseBook, pPrintOpt);
	m_pWriteResidualDisp	= new CSeisEvalReportResidualDisp	(m_pXL, pProgress, m_strBasePath, m_strBaseBook, pPrintOpt);
	m_pWriteFlexure			= new CSeisEvalReportFlexure		(m_pXL, pProgress, m_strBasePath, m_strBaseBook, pPrintOpt);
	m_pWriteShearSus		= new CSeisEvalReportShearSus		(m_pXL, pProgress, m_strBasePath, m_strBaseBook, pPrintOpt);
	m_pWriteShearSusResult	= new CSeisEvalReportShearSusResult	(m_pXL, pProgress, m_strBasePath, m_strBaseBook, pPrintOpt);
	m_pWriteShearSuc		= new CSeisEvalReportShearSuc		(m_pXL, pProgress, m_strBasePath, m_strBaseBook, pPrintOpt);
	m_pWriteShearSucResult	= new CSeisEvalReportShearSucResult	(m_pXL, pProgress, m_strBasePath, m_strBaseBook, pPrintOpt);
	m_pWriteDispPlastRatio	= new CSeisEvalReportDispPlastRatio	(m_pXL, pProgress, m_strBasePath, m_strBaseBook, pPrintOpt);
	m_pWriteCurvatureRC		= new CSeisEvalReportCurvatureRC	(m_pXL, pProgress, m_strBasePath, m_strBaseBook, pPrintOpt);
	m_pWriteCurvatureST		= new CSeisEvalReportCurvatureST	(m_pXL, pProgress, m_strBasePath, m_strBaseBook, pPrintOpt);
	m_pWriteShearPs			= new CSeisEvalReportShearPs		(m_pXL, pProgress, m_strBasePath, m_strBaseBook, pPrintOpt);
	m_pWriteShearPsResult	= new CSeisEvalReportShearPsResult	(m_pXL, pProgress, m_strBasePath, m_strBaseBook, pPrintOpt);
	m_pWriteRebarReduction	= new CSeisEvalReportRebarReduction	(m_pXL, pProgress, m_strBasePath, m_strBaseBook, pPrintOpt);
	m_pWritePierBottom		= new CSeisEvalReportPierBottom		(m_pXL, pProgress, m_strBasePath, m_strBaseBook, pPrintOpt);
	m_pWriteBearings		= new CSeisEvalReportBearings		(m_pXL, pProgress, m_strBasePath, m_strBaseBook, pPrintOpt);
}

void CBridgeSeisEvalReport_JROAD5_H14::AddStep(CDgnProgressDlg* pProgress)
{
	pProgress->m_ProgressBar.SetState(PBST_PAUSED);
	if (m_pWriteConditions != NULL)		m_pWriteConditions->AddStep();
	if (m_pWriteSummary != NULL)		m_pWriteSummary->AddStep();
	//변위의 조사
	AddStep_Print_Displaceme();
	//상부구조의 조사
	AddStep_Print_UpperStruct();
	//하부구조의 조사
	AddStep_Print_LowerStruct();
	//교각하부의 단면력
	if (m_pWritePierBottom != NULL)		m_pWritePierBottom->AddStep();
	//지승단면력과 변위
	if (m_pWriteBearings != NULL)		m_pWriteBearings->AddStep();
	//사용자설정 그룹의 조사
	AddStep_Print_UserGroup();
}

void CBridgeSeisEvalReport_JROAD5_H14::AddStep_Print_Displaceme()
{
	if (m_pWriteMaxDisp != NULL)		m_pWriteMaxDisp->AddStep();
	if (m_pWriteResidualDisp != NULL)	m_pWriteResidualDisp->AddStep();
}

void CBridgeSeisEvalReport_JROAD5_H14::AddStep_Print_UpperStruct()
{
	if (m_pWriteFlexure != NULL) {
		m_pWriteFlexure->m_subType = T_SIGR_D::kUpper;
		m_pWriteFlexure->AddStep();
	}
	if (m_pWriteCurvatureRC != NULL) {
		m_pWriteCurvatureRC->m_subType = T_SIGR_D::kUpper;
		m_pWriteCurvatureRC->AddStep();
	}
	if (m_pWriteCurvatureST != NULL) {
		m_pWriteCurvatureST->m_subType = T_SIGR_D::kUpper;
		m_pWriteCurvatureST->AddStep();
	}
	if (m_pWriteShearSus != NULL) {
		m_pWriteShearSus->m_subType = T_SIGR_D::kUpper;
		m_pWriteShearSus->AddStep();
	}
	if (m_pWriteShearSusResult != NULL) {
		m_pWriteShearSusResult->m_subType = T_SIGR_D::kUpper;
		m_pWriteShearSusResult->AddStep();
	}
	if (m_pWriteShearSuc != NULL) {
		m_pWriteShearSuc->m_subType = T_SIGR_D::kUpper;
		m_pWriteShearSuc->AddStep();
	}
	if (m_pWriteShearSucResult != NULL) {
		m_pWriteShearSucResult->m_subType = T_SIGR_D::kUpper;
		m_pWriteShearSucResult->AddStep();
	}
}

void CBridgeSeisEvalReport_JROAD5_H14::AddStep_Print_LowerStruct()
{
	if (m_pWriteDispPlastRatio != NULL) {
		m_pWriteDispPlastRatio->AddStep();
	}
	if (m_pWriteCurvatureRC != NULL) {
		m_pWriteCurvatureRC->m_subType = T_SIGR_D::kLower;
		m_pWriteCurvatureRC->AddStep();
	}
	if (m_pWriteCurvatureST != NULL) {
		m_pWriteCurvatureST->m_subType = T_SIGR_D::kLower;
		m_pWriteCurvatureST->AddStep();
	}
	if (m_pWriteShearPs != NULL) {
		m_pWriteShearPs->m_subType = T_SIGR_D::kLower;
		m_pWriteShearPs->AddStep();
	}
	if (m_pWriteShearPsResult != NULL) {
		m_pWriteShearPsResult->m_subType = T_SIGR_D::kLower;
		m_pWriteShearPsResult->AddStep();
	}
	if (m_pWriteRebarReduction != NULL) {
		m_pWriteRebarReduction->AddStep();
	}
}

void CBridgeSeisEvalReport_JROAD5_H14::AddStep_Print_UserGroup()
{
	if (m_pWriteFlexure != NULL) {
		m_pWriteFlexure->m_subType = T_SIGR_D::kUser;
		m_pWriteFlexure->AddStep();
	}
	if (m_pWriteCurvatureRC != NULL) {
		m_pWriteCurvatureRC->m_subType = T_SIGR_D::kUser;
		m_pWriteCurvatureRC->AddStep();
	}
	if (m_pWriteCurvatureST != NULL) {
		m_pWriteCurvatureST->m_subType = T_SIGR_D::kUser;
		m_pWriteCurvatureST->AddStep();
	}
	if (m_pWriteShearSus != NULL) {
		m_pWriteShearSus->m_subType = T_SIGR_D::kUser;
		m_pWriteShearSus->AddStep();
	}
	if (m_pWriteShearSusResult != NULL) {
		m_pWriteShearSusResult->m_subType = T_SIGR_D::kUser;
		m_pWriteShearSusResult->AddStep();
	}
	if (m_pWriteShearSuc != NULL) {
		m_pWriteShearSuc->m_subType = T_SIGR_D::kUser;
		m_pWriteShearSuc->AddStep();
	}
	if (m_pWriteShearSucResult != NULL) {
		m_pWriteShearSucResult->m_subType = T_SIGR_D::kUser;
		m_pWriteShearSucResult->AddStep();
	}
	if (m_pWriteShearPs != NULL) {
		m_pWriteShearPs->m_subType = T_SIGR_D::kUser;
		m_pWriteShearPs->AddStep();
	}
	if (m_pWriteShearPsResult != NULL) {
		m_pWriteShearPsResult->m_subType = T_SIGR_D::kUser;
		m_pWriteShearPsResult->AddStep();
	}
}

void CBridgeSeisEvalReport_JROAD5_H14::Write(CDgnProgressDlg* pProgress)
{
	pProgress->m_ProgressBar.SetState(PBST_NORMAL);
	if (m_pWriteConditions != NULL) m_pWriteConditions->Write();
	if (m_pWriteSummary != NULL) m_pWriteSummary->Write();
	int iParagraph1 = 0;
	int iParagraph2 = 0;
	//변위의 조사
	Print_Displacement(iParagraph1);
	//상부구조의 조사
	Print_UpperStruct(iParagraph1);
	//하부구조의 조사
	Print_LowerStruct(iParagraph1);
	//교각하부의 단면력
	if (m_pWritePierBottom != NULL) {
		m_pWritePierBottom->m_pParagraph1 = &iParagraph1;
		m_pWritePierBottom->m_pParagraph2 = &iParagraph2;
		m_pWritePierBottom->Write();
	}
	//지승단면력과 변위
	if (m_pWriteBearings != NULL) {
		m_pWriteBearings->m_pParagraph1 = &iParagraph1;
		m_pWriteBearings->m_pParagraph2 = &iParagraph2;
		m_pWriteBearings->Write();
	}
	//사용자설정 그룹의 조사
	Print_UserGroup(iParagraph1);
}

void CBridgeSeisEvalReport_JROAD5_H14::Print_Displacement(OUT int& iParagraph1)
{
//	1. 変位の照査 변위조사
	int iParagraph2 = 0;
//		1-1. 最大応答変位の照査 - 全体系 최대응답변위검토 - 전체시스템
//			1-1-1. 橋軸方向 교축방향
//			1-1-2. 橋軸直角方向 교축직각방향
	int nIndex1 = iParagraph1;
	if (m_pWriteMaxDisp != NULL) {
		m_pWriteMaxDisp->m_pParagraph1 = &nIndex1;
		m_pWriteMaxDisp->m_pParagraph2 = &iParagraph2;
		m_pWriteMaxDisp->Write();
	}
//		1-2. 残留変位の照査 - 全体系 잔류변위조사 - 전체시스템
//			1-2-1. 橋軸方向 교축방향
//			1-2-2. 橋軸直角方向 교축직각방향
//		1-3. 残留変位の照査 - 橋脚別 잔류변위조사 - 교각별 
//			1-3-1. 橋軸方向 교축방향
//			1-3-2. 橋軸直角方向 교축직각방향
	int nIndex2 = iParagraph1; 
	if (m_pWriteResidualDisp != NULL) {
		m_pWriteResidualDisp->m_pParagraph1 = &nIndex2;
		m_pWriteResidualDisp->m_pParagraph2 = &iParagraph2;
		m_pWriteResidualDisp->Write();
	}

	if ((std::max)({ nIndex1, nIndex2 }) > iParagraph1)
		iParagraph1++;
}

void CBridgeSeisEvalReport_JROAD5_H14::Print_UpperStruct(OUT int& iParagraph1)
{
//	2. 上部構造の照査 상부구조조사
	int iParagraph2 = 0;
//		2-1. 曲げの照査 휨조사
//			2-1-1. 橋軸方向 교축방향
//				1) タイプⅠ
//				2) タイプⅡ
//			2-1-2. 橋軸直角方向 교축직각방향
//				1) タイプⅠ
//				2) タイプⅡ
	int nIndex1 = iParagraph1; 
	if (m_pWriteFlexure != NULL) {
		m_pWriteFlexure->m_pParagraph1 = &nIndex1;
		m_pWriteFlexure->m_pParagraph2 = &iParagraph2;
		m_pWriteFlexure->m_subType = T_SIGR_D::kUpper;
		m_pWriteFlexure->Write();
	}
	int nIndex2 = iParagraph1; 
	if (m_pWriteCurvatureRC != NULL) {
		m_pWriteCurvatureRC->m_pParagraph1 = &nIndex2;
		m_pWriteCurvatureRC->m_pParagraph2 = &iParagraph2;
		m_pWriteCurvatureRC->m_subType = T_SIGR_D::kUpper;
		m_pWriteCurvatureRC->Write();
	}
	int nIndex3 = iParagraph1;
	if (m_pWriteCurvatureST != NULL) {
		m_pWriteCurvatureST->m_pParagraph1 = &nIndex3;
		m_pWriteCurvatureST->m_pParagraph2 = &iParagraph2;
		m_pWriteCurvatureST->m_subType = T_SIGR_D::kUpper;
		m_pWriteCurvatureST->Write();
	}
//		2-2. せん断耐力 Sus
//			2-2-1. 橋軸方向 교축방향
//				1) タイプⅠ
//				2) タイプⅡ
//			2-2-2. 橋軸直角方向 교축직각방향
//				1) タイプⅠ
//				2) タイプⅡ
	int nIndex4 = iParagraph1; 
	if (m_pWriteShearSus != NULL) {
		m_pWriteShearSus->m_pParagraph1 = &nIndex4;
		m_pWriteShearSus->m_pParagraph2 = &iParagraph2;
		m_pWriteShearSus->m_subType = T_SIGR_D::kUpper;
		m_pWriteShearSus->Write();
	}
//		2-3. せん断の照査
//			2-3-1. 橋軸方向 교축방향
//				1) タイプⅠ
//				2) タイプⅡ
//			2-3-2. 橋軸直角方向 교축직각방향
//				1) タイプⅠ
//				2) タイプⅡ
	if (m_pWriteShearSusResult != NULL) {
		m_pWriteShearSusResult->m_pParagraph1 = &nIndex4;
		m_pWriteShearSusResult->m_pParagraph2 = &iParagraph2;
		m_pWriteShearSusResult->m_subType = T_SIGR_D::kUpper;
		m_pWriteShearSusResult->Write();
	}
//		2-4. せん断耐力 Suc
//			2-4-1. 橋軸方向 교축방향
//				1) タイプⅠ
//				2) タイプⅡ
//			2-4-2. 橋軸直角方向 교축직각방향
//			2-4-2. 橋軸直角方向 교축직각방향
//			2-4-2. 橋軸直角方向 교축직각방향
//				1) タイプⅠ
//				2) タイプⅡ
	int nIndex5 = iParagraph1; 
	if (m_pWriteShearSuc != NULL) {
		m_pWriteShearSuc->m_pParagraph1 = &nIndex5;
		m_pWriteShearSuc->m_pParagraph2 = &iParagraph2;
		m_pWriteShearSuc->m_subType = T_SIGR_D::kUpper;
		m_pWriteShearSuc->Write();
	}
//		2-5. せん断の照査
//			2-5-1. 橋軸方向 교축방향
//				1) タイプⅠ
//				2) タイプⅡ
//			2-5-2. 橋軸直角方向 교축직각방향
//				1) タイプⅠ
//				2) タイプⅡ
	if (m_pWriteShearSucResult != NULL) {
		m_pWriteShearSucResult->m_pParagraph1 = &nIndex5;
		m_pWriteShearSucResult->m_pParagraph2 = &iParagraph2;
		m_pWriteShearSucResult->m_subType = T_SIGR_D::kUpper;
		m_pWriteShearSucResult->Write();
	}

	if ((std::max)({ nIndex1, nIndex2, nIndex3, nIndex4, nIndex5 }) > iParagraph1)
		iParagraph1++;
}

void CBridgeSeisEvalReport_JROAD5_H14::Print_LowerStruct(OUT int& iParagraph1)
{
//	3. 下部構造の3降伏照査 하부구조항복조사
	int iParagraph2 = 0;
//		3-1. 塑性率の照査 소성율조사
//			3-1-1. 橋軸方向 교축방향
//			3-1-2. 橋軸直角方向 교축직각방향
	int nIndex1 = iParagraph1;	
	if (m_pWriteDispPlastRatio != NULL) {
		m_pWriteDispPlastRatio->m_pParagraph1 = &nIndex1;
		m_pWriteDispPlastRatio->m_pParagraph2 = &iParagraph2;
		m_pWriteDispPlastRatio->Write();
	}
//		3-2. 曲げの照査 곡률조사
//			3-2-1. 橋軸方向 교축방향
//				1) タイプⅠ
//					(1) 柱部の結果
//					(2) 梁部の結果
//				2) タイプⅡ
//					(1) 柱部の結果
//					(2) 梁部の結果
//			3-2-2. 橋軸直角方向 교축직각방향
//				1) タイプⅠ
//					(1) 柱部の結果
//					(2) 梁部の結果
//				2) タイプⅡ
//					(1) 柱部の結果
//					(2) 梁部の結果
	int nIndex2 = iParagraph1;	
	if (m_pWriteCurvatureRC != NULL) {
		m_pWriteCurvatureRC->m_pParagraph1 = &nIndex2;
		m_pWriteCurvatureRC->m_pParagraph2 = &iParagraph2;
		m_pWriteCurvatureRC->m_subType = T_SIGR_D::kLower;
		m_pWriteCurvatureRC->Write();
	}
	int nIndex3 = iParagraph1;	
	if (m_pWriteCurvatureST != NULL) {
		m_pWriteCurvatureST->m_pParagraph1 = &nIndex3;
		m_pWriteCurvatureST->m_pParagraph2 = &iParagraph2;
		m_pWriteCurvatureST->m_subType = T_SIGR_D::kLower;
		m_pWriteCurvatureST->Write();
	}
//		3-3. せん断耐力 Ps
//			3-3-1. 橋軸方向
//				1) タイプⅠ
//					(1) 柱部の結果
//					(2) 梁部の結果
//				2) タイプⅡ
//					(1) 柱部の結果
//					(2) 梁部の結果
//			3-3-2. 橋軸直角方向
//				1) タイプⅠ
//					(1) 柱部の結果
//					(2) 梁部の結果
//				2) タイプⅡ
//					(1) 柱部の結果
//					(2) 梁部の結果
	int nIndex4 = iParagraph1;	
	if (m_pWriteShearPs != NULL) {
		m_pWriteShearPs->m_pParagraph1 = &nIndex4;
		m_pWriteShearPs->m_pParagraph2 = &iParagraph2;
		m_pWriteShearPs->m_subType = T_SIGR_D::kLower;
		m_pWriteShearPs->Write();
	}
//		3-4. せん断の照査
//			3-4-1. 橋軸方向
//				1) タイプⅠ
//					(1) 柱部の結果
//					(2) 梁部の結果
//				2) タイプⅡ
//					(1) 柱部の結果
//					(2) 梁部の結果
//			3-4-2. 橋軸直角方向
//				1) タイプⅠ
//					(1) 柱部の結果
//					(2) 梁部の結果
//				2) タイプⅡ
//					(1) 柱部の結果
//					(2) 梁部の結果
	if (m_pWriteShearPsResult != NULL) {
		m_pWriteShearPsResult->m_pParagraph1 = &nIndex4;
		m_pWriteShearPsResult->m_pParagraph2 = &iParagraph2;
		m_pWriteShearPsResult->m_subType = T_SIGR_D::kLower;
		m_pWriteShearPsResult->Write();
	}
//		3-5. 段落し部の検討(3波平均) 단락부의 검토
//			3-5-1.曲げ耐力の照査 휨내력조사
//			3-5-2.せん断耐力の照査 전단내력조사
	int nIndex5 = iParagraph1;	
	if (m_pWriteRebarReduction != NULL) {
		m_pWriteRebarReduction->m_pParagraph1 = &nIndex5;
		m_pWriteRebarReduction->m_pParagraph2 = &iParagraph2;
		m_pWriteRebarReduction->Write();
	}

	if ((std::max)({ nIndex1, nIndex2, nIndex3, nIndex4, nIndex5 }) > iParagraph1)
		iParagraph1++;
}

void CBridgeSeisEvalReport_JROAD5_H14::Print_UserGroup(OUT int& iParagraph1)
{
//	6. 使用者設定グループの照査
	int iParagraph2 = 0;
//		6-1. 曲げの照査 (・・・降伏曲率、許容曲率、降伏曲げモーメントの照査)
	int nIndex1 = iParagraph1;	
	if (m_pWriteFlexure != NULL) {
		m_pWriteFlexure->m_pParagraph1 = &nIndex1;
		m_pWriteFlexure->m_pParagraph2 = &iParagraph2;
		m_pWriteFlexure->m_subType = T_SIGR_D::kUser;
		m_pWriteFlexure->Write();
	}
	int nIndex2 = iParagraph1;	
	if (m_pWriteCurvatureRC != NULL) {
		m_pWriteCurvatureRC->m_pParagraph1 = &nIndex2;
		m_pWriteCurvatureRC->m_pParagraph2 = &iParagraph2;
		m_pWriteCurvatureRC->m_subType = T_SIGR_D::kUser;
		m_pWriteCurvatureRC->Write();
	}
	int nIndex3 = iParagraph1;	
	if (m_pWriteCurvatureST != NULL) {
		m_pWriteCurvatureST->m_pParagraph1 = &nIndex3;
		m_pWriteCurvatureST->m_pParagraph2 = &iParagraph2;
		m_pWriteCurvatureST->m_subType = T_SIGR_D::kUser;
		m_pWriteCurvatureST->Write();
	}
//		6-2. せん断耐力 Sus
	int nIndex4 = iParagraph1;	
	if (m_pWriteShearSus != NULL) {
		m_pWriteShearSus->m_pParagraph1 = &nIndex4;
		m_pWriteShearSus->m_pParagraph2 = &iParagraph2;
		m_pWriteShearSus->m_subType = T_SIGR_D::kUser;
		m_pWriteShearSus->Write();
	}
//		6-3. せん断の照査
	if (m_pWriteShearSusResult != NULL) {
		m_pWriteShearSusResult->m_pParagraph1 = &nIndex4;
		m_pWriteShearSusResult->m_pParagraph2 = &iParagraph2;
		m_pWriteShearSusResult->m_subType = T_SIGR_D::kUser;
		m_pWriteShearSusResult->Write();
	}
//		6-4. せん断耐力 Suc
	int nIndex5 = iParagraph1;	
	if (m_pWriteShearSuc != NULL) {
		m_pWriteShearSuc->m_pParagraph1 = &nIndex5;
		m_pWriteShearSuc->m_pParagraph2 = &iParagraph2;
		m_pWriteShearSuc->m_subType = T_SIGR_D::kUser;
		m_pWriteShearSuc->Write();
	}
//		6-5. せん断の照査
	if (m_pWriteShearSucResult != NULL) {
		m_pWriteShearSucResult->m_pParagraph1 = &nIndex5;
		m_pWriteShearSucResult->m_pParagraph2 = &iParagraph2;
		m_pWriteShearSucResult->m_subType = T_SIGR_D::kUser;
		m_pWriteShearSucResult->Write();
	}
//		6-6. せん断耐力 Ps
	int nIndex6 = iParagraph1;	
	if (m_pWriteShearPs != NULL) {
		m_pWriteShearPs->m_pParagraph1 = &nIndex6;
		m_pWriteShearPs->m_pParagraph2 = &iParagraph2;
		m_pWriteShearPs->m_subType = T_SIGR_D::kUser;
		m_pWriteShearPs->Write();
	}
//		6-7. せん断の照査
	if (m_pWriteShearPsResult != NULL) {
		m_pWriteShearPsResult->m_pParagraph1 = &nIndex6;
		m_pWriteShearPsResult->m_pParagraph2 = &iParagraph2;
		m_pWriteShearPsResult->m_subType = T_SIGR_D::kUser;
		m_pWriteShearPsResult->Write();
	}

	if ((std::max)({ nIndex1, nIndex2, nIndex3, nIndex4, nIndex5, nIndex6 }) > iParagraph1)
		iParagraph1++;
}
#pragma endregion

#pragma region // CBridgeSeisEvalReport_JROAD5_H24
CBridgeSeisEvalReport_JROAD5_H24::CBridgeSeisEvalReport_JROAD5_H24(IExcel* pXL, const CString& strBasePath, const CString& strBaseBook)
	: CBridgeSeisEvalReport_JROAD5_H14(pXL, strBasePath, strBaseBook)
{
}

CBridgeSeisEvalReport_JROAD5_H24::~CBridgeSeisEvalReport_JROAD5_H24(void)
{
}

void CBridgeSeisEvalReport_JROAD5_H24::CreateInst(CDgnProgressDlg* pProgress, const SDgnExcelPrintOpt* pPrintOpt)
{
	CBridgeSeisEvalReport_JROAD5_H14::CreateInst(pProgress, pPrintOpt);
}

void CBridgeSeisEvalReport_JROAD5_H24::AddStep(CDgnProgressDlg* pProgress)
{
	CBridgeSeisEvalReport_JROAD5_H14::AddStep(pProgress);
}

void CBridgeSeisEvalReport_JROAD5_H24::Write(CDgnProgressDlg* pProgress)
{
	CBridgeSeisEvalReport_JROAD5_H14::Write(pProgress);
}
#pragma endregion

#pragma region // CBridgeSeisEvalReport_JROAD5_H29
CBridgeSeisEvalReport_JROAD5_H29::CBridgeSeisEvalReport_JROAD5_H29(IExcel* pXL, const CString& strBasePath, const CString& strBaseBook)
{
	m_pXL = pXL;
	m_strBasePath = strBasePath;
	m_strBaseBook = strBaseBook;
}

CBridgeSeisEvalReport_JROAD5_H29::~CBridgeSeisEvalReport_JROAD5_H29(void)
{

}

void CBridgeSeisEvalReport_JROAD5_H29::CreateInst(CDgnProgressDlg* pProgress, const SDgnExcelPrintOpt* pPrintOpt)
{
	m_pWriteConditions = new CSeisEvalReportConditions_H29(m_pXL, pProgress, m_strBasePath, m_strBaseBook, pPrintOpt);
	m_pWriteSummary = new CSeisEvalReportSummary_A3(m_pXL, pProgress, m_strBasePath, m_strBaseBook, pPrintOpt);
	m_pWriteMaxDisp = new CSeisEvalReportMaxDisp(m_pXL, pProgress, m_strBasePath, m_strBaseBook, pPrintOpt);
	m_pWriteResidualDisp = new CSeisEvalReportResidualDisp(m_pXL, pProgress, m_strBasePath, m_strBaseBook, pPrintOpt);
	m_pWriteFlexure = new CSeisEvalReportFlexure_H29(m_pXL, pProgress, m_strBasePath, m_strBaseBook, pPrintOpt);
	m_pWriteCurvatureRC = new CSeisEvalReportCurvatureRC_H29(m_pXL, pProgress, m_strBasePath, m_strBaseBook, pPrintOpt);
	m_pWriteCurvatureST = new CSeisEvalReportCurvatureST(m_pXL, pProgress, m_strBasePath, m_strBaseBook, pPrintOpt);
	m_pWriteShearSus = new CSeisEvalReportShearSus_H29(m_pXL, pProgress, m_strBasePath, m_strBaseBook, pPrintOpt);
	m_pWriteShearSusResult = new CSeisEvalReportShearSusResult(m_pXL, pProgress, m_strBasePath, m_strBaseBook, pPrintOpt);
	m_pWriteShearSuc = new CSeisEvalReportShearSuc_H29(m_pXL, pProgress, m_strBasePath, m_strBaseBook, pPrintOpt);
	m_pWriteShearSucResult = new CSeisEvalReportShearSucResult(m_pXL, pProgress, m_strBasePath, m_strBaseBook, pPrintOpt);
	m_pWriteShearPs = new CSeisEvalReportShearPs_H29(m_pXL, pProgress, m_strBasePath, m_strBaseBook, pPrintOpt);
	m_pWriteShearPsResult = new CSeisEvalReportShearPsResult(m_pXL, pProgress, m_strBasePath, m_strBaseBook, pPrintOpt);
	m_pWriteRebarReduction = new CSeisEvalReportRebarReduction_H29(m_pXL, pProgress, m_strBasePath, m_strBaseBook, pPrintOpt);
	m_pWritePierBottom = new CSeisEvalReportPierBottom(m_pXL, pProgress, m_strBasePath, m_strBaseBook, pPrintOpt);
	m_pWriteBearings = new CSeisEvalReportBearings(m_pXL, pProgress, m_strBasePath, m_strBaseBook, pPrintOpt);

	T_SIFA_H29_D tSifaH29;
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	if (pDoc->m_pAttrCtrl2->GetQSifaH29()->Get(tSifaH29))
	{
		m_pWriteConditions->m_tSifaH29 = tSifaH29;
		m_pWriteMaxDisp->m_tSifaH29 = tSifaH29;
		m_pWriteResidualDisp->m_tSifaH29 = tSifaH29;
		m_pWriteFlexure->m_tSifaH29 = tSifaH29;
		m_pWriteCurvatureRC->m_tSifaH29 = tSifaH29;
		m_pWriteCurvatureST->m_tSifaH29 = tSifaH29;
		m_pWriteShearSus->m_tSifaH29 = tSifaH29;
		m_pWriteShearSusResult->m_tSifaH29 = tSifaH29;
		m_pWriteShearSuc->m_tSifaH29 = tSifaH29;
		m_pWriteShearSucResult->m_tSifaH29 = tSifaH29;
		m_pWriteShearPs->m_tSifaH29 = tSifaH29;
		m_pWriteShearPsResult->m_tSifaH29 = tSifaH29;
		m_pWriteRebarReduction->m_tSifaH29 = tSifaH29;
		m_pWritePierBottom->m_tSifaH29 = tSifaH29;
		m_pWriteBearings->m_tSifaH29 = tSifaH29;
	}
}

void CBridgeSeisEvalReport_JROAD5_H29::AddStep(CDgnProgressDlg* pProgress)
{
	pProgress->m_ProgressBar.SetState(PBST_PAUSED);
	if (m_pWriteConditions != NULL)		m_pWriteConditions->AddStep();
	if (m_pWriteSummary != NULL)		m_pWriteSummary->AddStep();
	//변위의 조사
	AddStep_Print_Displaceme();
	//상부구조의 조사
	AddStep_Print_UpperStruct();
	//하부구조의 조사
	AddStep_Print_LowerStruct();
	//교각하부의 단면력
	if (m_pWritePierBottom != NULL)		m_pWritePierBottom->AddStep();
	//지승단면력과 변위
	if (m_pWriteBearings != NULL)		m_pWriteBearings->AddStep();
	//사용자설정 그룹의 조사
	AddStep_Print_UserGroup();
}

void CBridgeSeisEvalReport_JROAD5_H29::AddStep_Print_Displaceme()
{
	if (m_pWriteMaxDisp != NULL)		m_pWriteMaxDisp->AddStep();
	if (m_pWriteResidualDisp != NULL)	m_pWriteResidualDisp->AddStep();
}

void CBridgeSeisEvalReport_JROAD5_H29::AddStep_Print_UpperStruct()
{
	if (m_pWriteFlexure != NULL) {
		m_pWriteFlexure->m_subType = T_SIGR_D::kUpper;
		m_pWriteFlexure->AddStep();
	}
	if (m_pWriteCurvatureRC != NULL) {
		m_pWriteCurvatureRC->m_subType = T_SIGR_D::kUpper;
		m_pWriteCurvatureRC->AddStep();
	}
	if (m_pWriteCurvatureST != NULL) {
		m_pWriteCurvatureST->m_subType = T_SIGR_D::kUpper;
		m_pWriteCurvatureST->AddStep();
	}
	if (m_pWriteShearSus != NULL) {
		m_pWriteShearSus->m_subType = T_SIGR_D::kUpper;
		m_pWriteShearSus->AddStep();
	}
	if (m_pWriteShearSusResult != NULL) {
		m_pWriteShearSusResult->m_subType = T_SIGR_D::kUpper;
		m_pWriteShearSusResult->AddStep();
	}
	if (m_pWriteShearSuc != NULL) {
		m_pWriteShearSuc->m_subType = T_SIGR_D::kUpper;
		m_pWriteShearSuc->AddStep();
	}
	if (m_pWriteShearSucResult != NULL) {
		m_pWriteShearSucResult->m_subType = T_SIGR_D::kUpper;
		m_pWriteShearSucResult->AddStep();
	}
}

void CBridgeSeisEvalReport_JROAD5_H29::AddStep_Print_LowerStruct()
{
	if (m_pWriteCurvatureRC != NULL) {
		m_pWriteCurvatureRC->m_subType = T_SIGR_D::kLower;
		m_pWriteCurvatureRC->AddStep();
	}
	if (m_pWriteCurvatureST != NULL) {
		m_pWriteCurvatureST->m_subType = T_SIGR_D::kLower;
		m_pWriteCurvatureST->AddStep();
	}
	if (m_pWriteShearPs != NULL) {
		m_pWriteShearPs->m_subType = T_SIGR_D::kLower;
		m_pWriteShearPs->AddStep();
	}
	if (m_pWriteShearPsResult != NULL) {
		m_pWriteShearPsResult->m_subType = T_SIGR_D::kLower;
		m_pWriteShearPsResult->AddStep();
	}
	if (m_pWriteRebarReduction != NULL) {
		m_pWriteRebarReduction->m_subType = T_SIGR_D::kLower;
		m_pWriteRebarReduction->AddStep();
	}
}

void CBridgeSeisEvalReport_JROAD5_H29::AddStep_Print_UserGroup()
{
	if (m_pWriteFlexure != NULL) {
		m_pWriteFlexure->m_subType = T_SIGR_D::kUser;
		m_pWriteFlexure->AddStep();
	}
	if (m_pWriteCurvatureRC != NULL) {
		m_pWriteCurvatureRC->m_subType = T_SIGR_D::kUser;
		m_pWriteCurvatureRC->AddStep();
	}
	if (m_pWriteCurvatureST != NULL) {
		m_pWriteCurvatureST->m_subType = T_SIGR_D::kUser;
		m_pWriteCurvatureST->AddStep();
	}
	if (m_pWriteShearSus != NULL) {
		m_pWriteShearSus->m_subType = T_SIGR_D::kUser;
		m_pWriteShearSus->AddStep();
	}
	if (m_pWriteShearSusResult != NULL) {
		m_pWriteShearSusResult->m_subType = T_SIGR_D::kUser;
		m_pWriteShearSusResult->AddStep();
	}
	if (m_pWriteShearSuc != NULL) {
		m_pWriteShearSuc->m_subType = T_SIGR_D::kUser;
		m_pWriteShearSuc->AddStep();
	}
	if (m_pWriteShearSucResult != NULL) {
		m_pWriteShearSucResult->m_subType = T_SIGR_D::kUser;
		m_pWriteShearSucResult->AddStep();
	}
	if (m_pWriteShearPs != NULL) {
		m_pWriteShearPs->m_subType = T_SIGR_D::kUser;
		m_pWriteShearPs->AddStep();
	}
	if (m_pWriteShearPsResult != NULL) {
		m_pWriteShearPsResult->m_subType = T_SIGR_D::kUser;
		m_pWriteShearPsResult->AddStep();
	}
}

void CBridgeSeisEvalReport_JROAD5_H29::Write(CDgnProgressDlg* pProgress)
{
	pProgress->m_ProgressBar.SetState(PBST_NORMAL);
	if (m_pWriteConditions != NULL) m_pWriteConditions->Write();
	if (m_pWriteSummary != NULL)	m_pWriteSummary->Write();
	int iParagraph1 = 0;
	int iParagraph2 = 0;
	//변위의 조사
	Print_Displacement(iParagraph1);
	//상부구조의 조사
	Print_UpperStruct(iParagraph1);
	//하부구조의 조사
	Print_LowerStruct(iParagraph1);
	//교각하부의 단면력
	if (m_pWritePierBottom != NULL) {
		m_pWritePierBottom->m_pParagraph1 = &iParagraph1;
		m_pWritePierBottom->m_pParagraph2 = &iParagraph2;
		m_pWritePierBottom->Write();
	}
	//지승단면력과 변위
	if (m_pWriteBearings != NULL) {
		m_pWriteBearings->m_pParagraph1 = &iParagraph1;
		m_pWriteBearings->m_pParagraph2 = &iParagraph2;
		m_pWriteBearings->Write();
	}
	//사용자설정 그룹의 조사
	Print_UserGroup(iParagraph1);
}

void CBridgeSeisEvalReport_JROAD5_H29::Print_Displacement(OUT int& iParagraph1)
{
//	1. 変位の照査 변위조사
	int iParagraph2 = 0;
//		1-1. 最大応答変位の照査 - 全体系 최대응답변위검토 - 전체시스템
//			1-1-1. 橋軸方向 교축방향
//			1-1-2. 橋軸直角方向 교축직각방향
	int nIndex1 = iParagraph1;
	if (m_pWriteMaxDisp != NULL) {
		m_pWriteMaxDisp->m_pParagraph1 = &nIndex1;
		m_pWriteMaxDisp->m_pParagraph2 = &iParagraph2;
		m_pWriteMaxDisp->Write();
	}
//		1-2. 残留変位の照査 - 全体系 잔류변위조사 - 전체시스템
//			1-2-1. 橋軸方向 교축방향
//			1-2-2. 橋軸直角方向 교축직각방향
//		1-3. 残留変位の照査 - 橋脚別 잔류변위조사 - 교각별 
//			1-3-1. 橋軸方向 교축방향
//			1-3-2. 橋軸直角方向 교축직각방향
	int nIndex2 = iParagraph1;
	if (m_pWriteResidualDisp != NULL) {
		m_pWriteResidualDisp->m_pParagraph1 = &nIndex2;
		m_pWriteResidualDisp->m_pParagraph2 = &iParagraph2;
		m_pWriteResidualDisp->Write();
	}

	if ((std::max)({ nIndex1, nIndex2 }) > iParagraph1)
		iParagraph1++;
}

void CBridgeSeisEvalReport_JROAD5_H29::Print_UpperStruct(OUT int& iParagraph1)
{
//	2. 上部構造の照査 상부구조조사
	int iParagraph2 = 0;
//		2-1. 曲げの照査 휨조사
//			2-1-1. 橋軸方向 교축방향
//				1) タイプⅠ
//				2) タイプⅡ
//			2-1-2. 橋軸直角方向 교축직각방향
//				1) タイプⅠ
//				2) タイプⅡ
	int nIndex1 = iParagraph1;
	if (m_pWriteFlexure != NULL) {
		m_pWriteFlexure->m_pParagraph1 = &nIndex1;
		m_pWriteFlexure->m_pParagraph2 = &iParagraph2;
		m_pWriteFlexure->m_subType = T_SIGR_D::kUpper;
		m_pWriteFlexure->Write();
	}
	int nIndex2 = iParagraph1;
	if (m_pWriteCurvatureRC != NULL) {
		m_pWriteCurvatureRC->m_pParagraph1 = &nIndex2;
		m_pWriteCurvatureRC->m_pParagraph2 = &iParagraph2;
		m_pWriteCurvatureRC->m_subType = T_SIGR_D::kUpper;
		m_pWriteCurvatureRC->Write();
	}
	int nIndex3 = iParagraph1;
	if (m_pWriteCurvatureST != NULL) {
		m_pWriteCurvatureST->m_pParagraph1 = &nIndex3;
		m_pWriteCurvatureST->m_pParagraph2 = &iParagraph2;
		m_pWriteCurvatureST->m_subType = T_SIGR_D::kUpper;
		m_pWriteCurvatureST->Write();
	}
//		2-2. せん断耐力 Sus
//			2-2-1. 橋軸方向 교축방향
//				1) タイプⅠ
//				2) タイプⅡ
//			2-2-2. 橋軸直角方向 교축직각방향
//				1) タイプⅠ
//				2) タイプⅡ
	int nIndex4 = iParagraph1;
	if (m_pWriteShearSus != NULL) {
		m_pWriteShearSus->m_pParagraph1 = &nIndex4;
		m_pWriteShearSus->m_pParagraph2 = &iParagraph2;
		m_pWriteShearSus->m_subType = T_SIGR_D::kUpper;
		m_pWriteShearSus->Write();
	}
//		2-3. せん断の照査
//			2-3-1. 橋軸方向 교축방향
//				1) タイプⅠ
//				2) タイプⅡ
//			2-3-2. 橋軸直角方向 교축직각방향
//				1) タイプⅠ
//				2) タイプⅡ
	if (m_pWriteShearSusResult != NULL) {
		m_pWriteShearSusResult->m_pParagraph1 = &nIndex4;
		m_pWriteShearSusResult->m_pParagraph2 = &iParagraph2;
		m_pWriteShearSusResult->m_subType = T_SIGR_D::kUpper;
		m_pWriteShearSusResult->Write();
	}
//		2-4. せん断耐力 Suc
//			2-4-1. 橋軸方向 교축방향
//				1) タイプⅠ
//				2) タイプⅡ
//			2-4-2. 橋軸直角方向 교축직각방향
//			2-4-2. 橋軸直角方向 교축직각방향
//			2-4-2. 橋軸直角方向 교축직각방향
//				1) タイプⅠ
//				2) タイプⅡ
	int nIndex5 = iParagraph1;
	if (m_pWriteShearSuc != NULL) {
		m_pWriteShearSuc->m_pParagraph1 = &nIndex5;
		m_pWriteShearSuc->m_pParagraph2 = &iParagraph2;
		m_pWriteShearSuc->m_subType = T_SIGR_D::kUpper;
		m_pWriteShearSuc->Write();
	}
//		2-5. せん断の照査
//			2-5-1. 橋軸方向 교축방향
//				1) タイプⅠ
//				2) タイプⅡ
//			2-5-2. 橋軸直角方向 교축직각방향
//				1) タイプⅠ
//				2) タイプⅡ
	if (m_pWriteShearSucResult != NULL) {
		m_pWriteShearSucResult->m_pParagraph1 = &nIndex5;
		m_pWriteShearSucResult->m_pParagraph2 = &iParagraph2;
		m_pWriteShearSucResult->m_subType = T_SIGR_D::kUpper;
		m_pWriteShearSucResult->Write();
	}

	if ((std::max)({ nIndex1, nIndex2, nIndex3, nIndex4, nIndex5 }) > iParagraph1)
		iParagraph1++;
}

void CBridgeSeisEvalReport_JROAD5_H29::Print_LowerStruct(OUT int& iParagraph1)
{
//	3. 下部構造の3降伏照査 하부구조항복조사
	int iParagraph2 = 0;
//		3-1. 曲げの照査 곡률조사
//			3-1-1. 橋軸方向 교축방향
//				1) タイプⅠ
//					(1) 柱部の結果
//					(2) 梁部の結果
//				2) タイプⅡ
//					(1) 柱部の結果
//					(2) 梁部の結果
//			3-1-2. 橋軸直角方向 교축직각방향
//				1) タイプⅠ
//					(1) 柱部の結果
//					(2) 梁部の結果
//				2) タイプⅡ
//					(1) 柱部の結果
//					(2) 梁部の結果
	int nIndex1 = iParagraph1;
	if (m_pWriteCurvatureRC != NULL) {
		m_pWriteCurvatureRC->m_pParagraph1 = &nIndex1;
		m_pWriteCurvatureRC->m_pParagraph2 = &iParagraph2;
		m_pWriteCurvatureRC->m_subType = T_SIGR_D::kLower;
		m_pWriteCurvatureRC->Write();
	}
	int nIndex2 = iParagraph1;
	if (m_pWriteCurvatureST != NULL) {
		m_pWriteCurvatureST->m_pParagraph1 = &nIndex2;
		m_pWriteCurvatureST->m_pParagraph2 = &iParagraph2;
		m_pWriteCurvatureST->m_subType = T_SIGR_D::kLower;
		m_pWriteCurvatureST->Write();
	}
	//		2-2. せん断耐力 Sus
//			2-2-1. 橋軸方向 교축방향
//				1) タイプⅠ
//				2) タイプⅡ
//			2-2-2. 橋軸直角方向 교축직각방향
//				1) タイプⅠ
//				2) タイプⅡ
	int nIndex3 = iParagraph1;
	if (m_pWriteShearSus != NULL) {
		m_pWriteShearSus->m_pParagraph1 = &nIndex3;
		m_pWriteShearSus->m_pParagraph2 = &iParagraph2;
		m_pWriteShearSus->m_subType = T_SIGR_D::kLower;
		m_pWriteShearSus->Write();
	}
	//		2-3. せん断の照査
	//			2-3-1. 橋軸方向 교축방향
	//				1) タイプⅠ
	//				2) タイプⅡ
	//			2-3-2. 橋軸直角方向 교축직각방향
	//				1) タイプⅠ
	//				2) タイプⅡ
	if (m_pWriteShearSusResult != NULL) {
		m_pWriteShearSusResult->m_pParagraph1 = &nIndex3;
		m_pWriteShearSusResult->m_pParagraph2 = &iParagraph2;
		m_pWriteShearSusResult->m_subType = T_SIGR_D::kLower;
		m_pWriteShearSusResult->Write();
	}
	//		2-4. せん断耐力 Suc
	//			2-4-1. 橋軸方向 교축방향
	//				1) タイプⅠ
	//				2) タイプⅡ
	//			2-4-2. 橋軸直角方向 교축직각방향
	//			2-4-2. 橋軸直角方向 교축직각방향
	//			2-4-2. 橋軸直角方向 교축직각방향
	//				1) タイプⅠ
	//				2) タイプⅡ
	int nIndex4 = iParagraph1;
	if (m_pWriteShearSuc != NULL) {
		m_pWriteShearSuc->m_pParagraph1 = &nIndex4;
		m_pWriteShearSuc->m_pParagraph2 = &iParagraph2;
		m_pWriteShearSuc->m_subType = T_SIGR_D::kLower;
		m_pWriteShearSuc->Write();
	}
	//		2-5. せん断の照査
	//			2-5-1. 橋軸方向 교축방향
	//				1) タイプⅠ
	//				2) タイプⅡ
	//			2-5-2. 橋軸直角方向 교축직각방향
	//				1) タイプⅠ
	//				2) タイプⅡ
	if (m_pWriteShearSucResult != NULL) {
		m_pWriteShearSucResult->m_pParagraph1 = &nIndex4;
		m_pWriteShearSucResult->m_pParagraph2 = &iParagraph2;
		m_pWriteShearSucResult->m_subType = T_SIGR_D::kLower;
		m_pWriteShearSucResult->Write();
	}
//		3-4. 段落し部の検討(3波平均) 단락부의 검토
//			3-4-1.曲げ耐力の照査 휨내력조사
//			3-4-2.せん断耐力の照査 전단내력조사
	int nIndex5 = iParagraph1;
	if (m_pWriteRebarReduction != NULL) {
		m_pWriteRebarReduction->m_pParagraph1 = &nIndex5;
		m_pWriteRebarReduction->m_pParagraph2 = &iParagraph2;
		m_pWriteRebarReduction->m_subType = T_SIGR_D::kLower;
		m_pWriteRebarReduction->Write();
	}

	if ((std::max)({ nIndex1, nIndex2, nIndex3, nIndex4, nIndex5 }) > iParagraph1)
		iParagraph1++;
}

void CBridgeSeisEvalReport_JROAD5_H29::Print_UserGroup(OUT int& iParagraph1)
{
//	6. 使用者設定グループの照査
	int iParagraph2 = 0;
//		6-1. 曲げの照査 (・・・降伏曲率、許容曲率、降伏曲げモーメントの照査)
	int nIndex1 = iParagraph1;
	if (m_pWriteFlexure != NULL) {
		m_pWriteFlexure->m_pParagraph1 = &nIndex1;
		m_pWriteFlexure->m_pParagraph2 = &iParagraph2;
		m_pWriteFlexure->m_subType = T_SIGR_D::kUser;
		m_pWriteFlexure->Write();
	}
	int nIndex2 = iParagraph1;
	if (m_pWriteCurvatureRC != NULL) {
		m_pWriteCurvatureRC->m_pParagraph1 = &nIndex2;
		m_pWriteCurvatureRC->m_pParagraph2 = &iParagraph2;
		m_pWriteCurvatureRC->m_subType = T_SIGR_D::kUser;
		m_pWriteCurvatureRC->Write();
	}
	int nIndex3 = iParagraph1;
	if (m_pWriteCurvatureST != NULL) {
		m_pWriteCurvatureST->m_pParagraph1 = &nIndex3;
		m_pWriteCurvatureST->m_pParagraph2 = &iParagraph2;
		m_pWriteCurvatureST->m_subType = T_SIGR_D::kUser;
		m_pWriteCurvatureST->Write();
	}
//		6-2. せん断耐力 Sus
	int nIndex4 = iParagraph1;
	if (m_pWriteShearSus != NULL) {
		m_pWriteShearSus->m_pParagraph1 = &nIndex4;
		m_pWriteShearSus->m_pParagraph2 = &iParagraph2;
		m_pWriteShearSus->m_subType = T_SIGR_D::kUser;
		m_pWriteShearSus->Write();
	}
//		6-3. せん断の照査
	if (m_pWriteShearSusResult != NULL) {
		m_pWriteShearSusResult->m_pParagraph1 = &nIndex4;
		m_pWriteShearSusResult->m_pParagraph2 = &iParagraph2;
		m_pWriteShearSusResult->m_subType = T_SIGR_D::kUser;
		m_pWriteShearSusResult->Write();
	}
//		6-4. せん断耐力 Suc
	int nIndex5 = iParagraph1;
	if (m_pWriteShearSuc != NULL) {
		m_pWriteShearSuc->m_pParagraph1 = &nIndex5;
		m_pWriteShearSuc->m_pParagraph2 = &iParagraph2;
		m_pWriteShearSuc->m_subType = T_SIGR_D::kUser;
		m_pWriteShearSuc->Write();
	}
//		6-5. せん断の照査
	if (m_pWriteShearSucResult != NULL) {
		m_pWriteShearSucResult->m_pParagraph1 = &nIndex5;
		m_pWriteShearSucResult->m_pParagraph2 = &iParagraph2;
		m_pWriteShearSucResult->m_subType = T_SIGR_D::kUser;
		m_pWriteShearSucResult->Write();
	}
//		6-6. せん断耐力 Ps
	int nIndex6 = iParagraph1;
	if (m_pWriteShearPs != NULL) {
		m_pWriteShearPs->m_pParagraph1 = &nIndex6;
		m_pWriteShearPs->m_pParagraph2 = &iParagraph2;
		m_pWriteShearPs->m_subType = T_SIGR_D::kUser;
		m_pWriteShearPs->Write();
	}
//		6-7. せん断の照査
	if (m_pWriteShearPsResult != NULL) {
		m_pWriteShearPsResult->m_pParagraph1 = &nIndex6;
		m_pWriteShearPsResult->m_pParagraph2 = &iParagraph2;
		m_pWriteShearPsResult->m_subType = T_SIGR_D::kUser;
		m_pWriteShearPsResult->Write();
	}

	if ((std::max)({ nIndex1, nIndex2, nIndex3, nIndex4, nIndex5, nIndex6 }) > iParagraph1)
		iParagraph1++;
}
#pragma endregion