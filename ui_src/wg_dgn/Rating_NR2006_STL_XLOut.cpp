#include "stdafx.h"

#include "Rating_NR2006_STL_XLOut.h"

#include "RatingReportDataCtrl.h"
#include "Dgn_SteelRptManager_NR2006.h"

#include "..\wg_db\AttrCtrl.h"
#include "..\wg_db\AttrCtrl2.h"
#include "..\wg_db\PostCtrl.h"
#include "..\wg_db\AnalysisResult.h"
#include "..\wg_db\DB_ST_DN.h"

#include "..\..\DgnEngine\Src\DgnCalcBase\DgnCalcBase_CSG_Struct.h"
#include "..\..\DgnEngine\Src\CVL_DgnCalc_US\DgnCalc_CSG_CS457_Struct.h"
#include "..\..\DgnEngine\idesign\DGN_lib\ExcelGenerator.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define D_NUMBER_FORMAT_3  "0.000_ "
const double cMaxRF = 1.0E+70;
const double cMaxBSU = 1.0E+10;
#define DgnAbsDivide(a,b) fabs(b)>cDGN_Zero ? fabs(a/b) : cMaxRF

#define SYM_DASH _T("-")

using namespace dgn::lib;

CRating_NR2006_STL_XLOut::CRating_NR2006_STL_XLOut(void) : CDgn_XLOut()
{
	m_pDoc = CDBDoc::GetDocPoint();

	if(m_pDoc!=NULL)
	{
		m_pDgnResult = m_pDoc->m_pPostCtrl->GetAnalysisResult();
	}

}


CRating_NR2006_STL_XLOut::~CRating_NR2006_STL_XLOut(void)
{
}

void CRating_NR2006_STL_XLOut::PrintReport(CString szProgramDir, CString szProjectFileDir, CString strPath, int iPrintOpt, CArray<_T_NR2006_STL_XLOUT, _T_NR2006_STL_XLOUT&>& arXlout)
{
    m_strBasePath   = szProgramDir;
    m_strDataPath   = szProjectFileDir;

    CDgnEngineProgressDlg ProDlg;
    ProDlg.Create_ProgressDlg(PROGRESS_TYPE_STEEL_XL_RPT);// PSC Rating XL Report -> 202

    m_strBasePath = m_pDoc->GetProgramPath() + _T("Excel Base File\\");
    m_strBaseBook = _T("Steel Composite_NR2006.xlsx");

    BOOL bIsRegressionTest = CDBDoc::GetDocPoint()->IsRegTestMode();

    // Prevent Server Busy Dialog Box
    // https://docs.microsoft.com/ko-kr/cpp/mfc/reference/colemessagefilter-class?view=vs-2019
    COleMessageFilter *pFilter = AfxOleGetMessageFilter();
    //pFilter->SetMessagePendingDelay(); // Default는 5000ms입니다.
    pFilter->EnableNotRespondingDialog(FALSE);
    pFilter->EnableBusyDialog(FALSE);

	CExcelGenerator Excel(_T(""), XLTYPE::TYPE_LIBXL);
	CString strFileName = Excel.GetDLLFilePath(m_strBaseBook, FALSE);

	IExcel* pXL = Excel.Generate(strFileName);
	if(Excel.GetExcelType() == XLTYPE::TYPE_LIBXL)
	{
		m_strBaseBook = strFileName; // 경로 포함.
	}
    
    SetOutputUnitSystem();
    SetUnitConvertFactor();

    int nElem = arXlout.GetSize();
    int nProgressCount = 1;
    int nProgressTotal = 3*nElem + GetDetailCount(); // Assessment((Summary+IJ)*Element) + Detail
  
    if ( !bIsRegressionTest )
    {
        for ( int i=0; i<nElem; ++i )
        {
			// #1. Summary
			SetSummarySheetPage(pXL, arXlout[i].ElemK);
			GenerateSummarySheet(pXL, arXlout[i]);
			if(ProDlg.Update_ProgressDlg(nProgressCount++, nProgressTotal)) { pXL->Quit(); delete pXL; return; }

            for ( int j=0; j<2; ++j ) // Position
            {
                if ( !arXlout[i].bIJ[j] )
                {
                    if ( ProDlg.Update_ProgressDlg(nProgressCount++, nProgressTotal) ) { pXL->Quit(); delete pXL; return; }// #2. Assessment
                    continue;
                }

                // #2. Assessment
                SetAssessmentSheetPage(pXL, arXlout[i].ElemK, j);
                GenerateAssementSheet(pXL, j, arXlout[i]);

                if ( ProDlg.Update_ProgressDlg(nProgressCount++, nProgressTotal) ) { pXL->Quit(); delete pXL; return; }
            }
        }
    }
  
    // #3. Detail
    D_XL_PROGRESS_INTERFACE* pProInte = ProDlg.Get_InterfacePoint();
    pProInte->bUpdateTitle   = TRUE;
    pProInte->bUpdateMessage = TRUE;
    pProInte->bUpdatePercent = TRUE;
    pProInte->iCurStep       = nProgressCount-1;
    pProInte->iTotalStep     = nProgressTotal;
    GenerateDetailRpt(pXL, szProgramDir, szProjectFileDir, strPath, pProInte);
    if ( ProDlg.Get_InterfacePoint()->bStopExecute ) { pXL->Quit(); delete pXL; return; }

	if(!bIsRegressionTest)
	{
		//// Delete Default Sheet and Save.
		CString strDefaultSheet[3] = {"Sheet1", "Sheet2", "Sheet3"};
		for(int i=0; i<3; i++)
		{
			if(pXL->IsExistSheetName(strDefaultSheet[i]))
			{
				pXL->DeleteSheet(strDefaultSheet[i]);
			}
		}
		if(Excel.GetExcelType() == XLTYPE::TYPE_LIBXL)
		{
			pXL->GetBook()->setActiveSheet(0);
		}
		
		pXL->SaveAs(pXL->GetBookName(), strPath);//* 출력파일 저쟝..
	}
    pXL->Quit();
    delete pXL;

    ProDlg.Delete_ProgressDlg();

    if ( strPath.Find(_T(".txt")) == -1 ) // RT 이후 텍스트 파일 남기지 않기 위한 방법
    {
        ShellExecute(NULL, _T("open"), strPath, NULL, NULL, SW_SHOWNORMAL);
    }
}

void CRating_NR2006_STL_XLOut::SetDataCtrlPointer(CCRCDataCtrl* pDataCtrl)
{
	m_pDataCtrl = pDataCtrl;
}

int CRating_NR2006_STL_XLOut::GetDetailCount()
{
	CArray<T_RKPO_K, T_RKPO_K> aPrtElemK;
	m_pDoc->m_pAttrCtrl->GetRkpoKeyList(aPrtElemK);

	int nDedail = 0;
	INT_PTR nElem = aPrtElemK.GetSize();
	for(INT_PTR i=0; i<nElem; ++i)
	{
		T_ELEM_K  ElemK = aPrtElemK[i];

		// 출력 옵션 셋팅
		// 1. 출력 여부 결정
		T_ASOP_D AsopD; AsopD.Initialize();  // 설계 위치
		if(!m_pDoc->m_pAttrCtrl->GetAsop(ElemK, AsopD)) continue;
		T_RKPO_D RkpoD; RkpoD.Initialize();  // 출력 위치
		if(!m_pDoc->m_pAttrCtrl->GetRkpo(ElemK, RkpoD)) continue;

		if((AsopD.nPos == 0 || AsopD.nPos == 2) && (RkpoD.iPrintChk == 0 || RkpoD.iPrintChk == 2))
		{
			++nDedail;
		}
		if((AsopD.nPos == 1 || AsopD.nPos == 2) && (RkpoD.iPrintChk == 1 || RkpoD.iPrintChk == 2))
		{
			++nDedail;
		}
	}
	return nDedail;
}

CString CRating_NR2006_STL_XLOut::GetPartNameWithNode(T_ELEM_K ElemK, int iPosiNo)
{
	CString strPartNa=_T("");
	if(iPosiNo > 1)	{ ASSERT(0);	return strPartNa; }	// 0=1, 1=J.
	CString strPosiNa = (iPosiNo==0 ? _T("I") : _T("J"));
	T_ELEM_D ElemD;
	ElemD.Initialize();
	if(!m_pAttrCtrl->GetElem(ElemK, ElemD))	return strPosiNa;
	CString strNodeNo=_T("");
	strNodeNo.Format(_T("%d"), ElemD.elnod[iPosiNo]);
	strPartNa = strPosiNa + _T("(") + strNodeNo + _T(")");

	return strPartNa;
}

void CRating_NR2006_STL_XLOut::SetSheetPage(IExcel* pExcel, CString strNameCurrent)
{
	if(pExcel->IsExistSheetName(strNameCurrent))
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
	if(pExcel->GetExcelType() == XLTYPE::TYPE_LIBXL)
		pExcel->GetSheet()->setPrintFit(1, 0); // page fit
}

int CRating_NR2006_STL_XLOut::GetConcurrentSize()
{
	return 12;
}

void CRating_NR2006_STL_XLOut::SetSummarySheetPage(IExcel* pExcel, const T_ELEM_K ElemK)
{
	m_strBaseSheet = _T("Summary(Base)");
	CString strType = GetMembType(ElemK);
	CString strSheetName;
	strSheetName.Format(_T("%s_%d"), strType, ElemK);
	SetSheetPage(pExcel, strSheetName);
}

void CRating_NR2006_STL_XLOut::GenerateSummarySheet(IExcel* pExcel, const _T_NR2006_STL_XLOUT& XloutD)
{
	CString RptBlock;
	int stRow  = 0;
	int rowCnt = 0;

	// 1. Design Condition
	RptBlock = _T("Rating_Summary_Dgn_Condition");
	rowCnt = CopyRange(pExcel, RptBlock, stRow);

	CString strCode = CDBLib::GetStlRatCodeName(m_pDoc->GetCivilCodeDgn());
	CString strType = GetMembType(XloutD.ElemK);

	CArray<D_CELL, D_CELL&> arData;
	arData.RemoveAll();
	arData.Add(D_CELL(1, 1, strCode));
	arData.Add(D_CELL(1, 8, GetOutputFormat(XloutD.ElemK, 0)));
	arData.Add(D_CELL(1, 12, strType));
	Print_Array_D_CELL(pExcel, arData, stRow+1);
	stRow += rowCnt+1;

	GenerateSumBendingTable(pExcel, XloutD, stRow);
	GenerateSumShearTable(pExcel, XloutD, stRow);
	GenerateSumTransverseStifferTable(pExcel, XloutD, stRow, true);
	GenerateSumTransverseStifferTable(pExcel, XloutD, stRow, false);
}

void CRating_NR2006_STL_XLOut::GenerateSumBendingTable(IExcel* pExcel, const _T_NR2006_STL_XLOUT& XloutD, int& stRow)
{
	int nIncreaseRow = 0;

	CArray<D_CELL, D_CELL&> arData;
	arData.RemoveAll();

	ElemPairK EPairK(XloutD.ElemK, EN_EL_BEAM);

	T_ASFC_CSG_BS_CASE PosD, NegD;
	if(!m_pDoc->m_pPostCtrl->GetAnalysisResult()->GetFlexuralStrengthRating4CS454CSG(EPairK, 0, 1, 0, PosD)) PosD.Initialize();
	if(!m_pDoc->m_pPostCtrl->GetAnalysisResult()->GetFlexuralStrengthRating4CS454CSG(EPairK, 0, 2, 0, NegD)) NegD.Initialize();

	T_ASFC_CSG_BS_CASE AsfcCaseD;
	for(int j=0; j<2; ++j)
	{
		if(!XloutD.bIJ[j]) continue;

		const auto& AsfcB = [&]()
		{
			const auto& PAsfcB = PosD.AsfcB[j];
			const auto& NAsfcB = NegD.AsfcB[j];
			if(PAsfcB.ComD.bChk && NAsfcB.ComD.bChk)
			{
				return ME(PAsfcB.RatPa.dDCR, NAsfcB.RatPa.dDCR) ? PAsfcB : NAsfcB;
			}
			else if(NAsfcB.ComD.bChk)
			{
				return NAsfcB;
			}
			else
			{
				return PAsfcB;
			}
		}();

		if(AsfcB.ComD.bChk)
		{
			BOOL bChk = FALSE;
			XloutD.mBend.Lookup(AsfcB.ComD.RCaseK, bChk);
			if(!bChk) continue;

			T_ALCS_D AlcsD;
			if (!m_pDoc->m_pAttrCtrl2->GetAlcs(AsfcB.ComD.RCaseK, AlcsD)) ASSERT(0); // Rating case

			BOOL bIsWagon = IsWagon(AlcsD.nLiveType);

			BOOL bSReverseM = (AsfcB.dSd * AsfcB.dSst < 0) || EQ0(AsfcB.dSst);;
			BOOL bDReverseM = (AsfcB.dSd * AsfcB.dS < 0) || EQ0(AsfcB.dS);;

			arData.Add(D_CELL(nIncreaseRow,  1, GetPartNameWithNode(AsfcB.ComD.ElemK, AsfcB.ComD.unPosi)));
			arData.Add(D_CELL(nIncreaseRow,  3, m_pDoc->m_pPostCtrl->GetRatingCaseNameBD21(AsfcB.ComD.RCaseK, AsfcB.ComD.unConcurrent)));

			arData.Add(D_CELL(nIncreaseRow,  7, GetOutputFormat(AsfcB.dSd  *m_dFactorMoment, 3), D_NUMBER_FORMAT_3)); // loading Dead
			arData.Add(D_CELL(nIncreaseRow, 10, GetOutputFormat(AsfcB.dSst *m_dFactorMoment, 3), D_NUMBER_FORMAT_3)); // loading Live
			arData.Add(D_CELL(nIncreaseRow, 13, GetOutputFormat(AsfcB.dS   *m_dFactorMoment, 3), D_NUMBER_FORMAT_3)); // loading Dynamic

			if (bSReverseM && bDReverseM)
			{
				arData.Add(D_CELL(nIncreaseRow, 16, SYM_DASH));
			}
			else
			{
				arData.Add(D_CELL(nIncreaseRow, 16, GetOutputFormat(AsfcB.dRA * m_dFactorMoment, 3), D_NUMBER_FORMAT_3)); // Capacity
			}
			
			if (!bSReverseM)
			{
				arData.Add(D_CELL(nIncreaseRow, 19, bIsWagon ? SYM_DASH : GetOutputFormatWithInfinity(AsfcB.RatPa.dSBSU, 3, cMaxBSU, TRUE, 1), D_NUMBER_FORMAT_3)); // BSU Static
				arData.Add(D_CELL(nIncreaseRow, 25, bIsWagon ? SYM_DASH : GetRANumber(static_cast<int>(AsfcB.RatPa.dSRA)))); // RA Static
				arData.Add(D_CELL(nIncreaseRow, 31, GetOutputFormat(AsfcB.RatPa.dSUforRA, 3), D_NUMBER_FORMAT_3)); // U for RA8 Static
				arData.Add(D_CELL(nIncreaseRow, 37, GetAssessedCategory(AsfcB.RatPa.nSAC), _T(""), GetAssessedCategoryColor(AsfcB.RatPa.nSAC)));	// Assessed Category Static
			}
			else
			{
				arData.Add(D_CELL(nIncreaseRow, 19, SYM_DASH)); // BSU Static
				arData.Add(D_CELL(nIncreaseRow, 25, SYM_DASH)); // RA Static
				arData.Add(D_CELL(nIncreaseRow, 31, SYM_DASH)); // U for RA8 Static
				arData.Add(D_CELL(nIncreaseRow, 37, SYM_DASH));	// Assessed Category Static
			}

			if (!bDReverseM)
			{
				arData.Add(D_CELL(nIncreaseRow, 22, bIsWagon ? SYM_DASH : GetOutputFormatWithInfinity(AsfcB.RatPa.dDBSU, 3, cMaxBSU, TRUE, 1), D_NUMBER_FORMAT_3)); // BSU Dynamic
				arData.Add(D_CELL(nIncreaseRow, 28, bIsWagon ? SYM_DASH : GetRANumber(static_cast<int>(AsfcB.RatPa.dDRA)))); // RA Dynamic
				arData.Add(D_CELL(nIncreaseRow, 34, GetOutputFormat(AsfcB.RatPa.dDUforRA, 3), D_NUMBER_FORMAT_3)); // U for RA8 Dynamic
				arData.Add(D_CELL(nIncreaseRow, 40, GetAssessedCategory(AsfcB.RatPa.nDAC), _T(""), GetAssessedCategoryColor(AsfcB.RatPa.nDAC)));	// Assessed Category Dynamic
			}
			else
			{
				arData.Add(D_CELL(nIncreaseRow, 22, SYM_DASH)); // BSU Dynamic
				arData.Add(D_CELL(nIncreaseRow, 28, SYM_DASH)); // RA Dynamic
				arData.Add(D_CELL(nIncreaseRow, 34, SYM_DASH)); // U for RA8 Dynamic
				arData.Add(D_CELL(nIncreaseRow, 40, SYM_DASH));	// Assessed Category Dynamic
			}

			nIncreaseRow++;
		}
	}
	if(nIncreaseRow==0)
	{
		arData.Add(D_CELL(nIncreaseRow, 1, SYM_DASH));
		arData.Add(D_CELL(nIncreaseRow, 3, SYM_DASH));
		for(int k=0; k<12; ++k)
		{
			const int iCol = 7+(k*3);
			arData.Add(D_CELL(nIncreaseRow, iCol, SYM_DASH));
		}
		nIncreaseRow++;
	}

	// Write Data
	CString RptBlock;
	int rowCnt = 0;

	// Table Title
	RptBlock = _T("Rating_Summary_Bending");
	rowCnt = CopyRange(pExcel, RptBlock, stRow);
	stRow += rowCnt;
	// Table Head
	RptBlock = _T("Rating_Summary_Bend_Table_Head");
	rowCnt = CopyRange(pExcel, RptBlock, stRow);
	stRow += rowCnt;

	for(int i=0; i<nIncreaseRow; ++i)
	{
		if(i==(nIncreaseRow-1))
		{
			RptBlock = _T("Rating_Summary_Table_End");
		}
		else
		{
			RptBlock = _T("Rating_Summary_Table_Body");
		}
		rowCnt = CopyRange(pExcel, RptBlock, stRow+i);
	}

	Print_Array_D_CELL(pExcel, arData, stRow);
	stRow += (nIncreaseRow+1);
}

void CRating_NR2006_STL_XLOut::GenerateSumShearTable(IExcel* pExcel, const _T_NR2006_STL_XLOUT& XloutD, int& stRow)
{
	int nIncreaseRow = 0;

	CArray<D_CELL, D_CELL&> arData;
	arData.RemoveAll();

	T_ASSC_CSG_BS_CASE AsscCaseD;
	if(m_pDoc->m_pPostCtrl->GetAnalysisResult()->GetShearStrengthRating4CS454CSG(ElemPairK(XloutD.ElemK, EN_EL_BEAM), 0, 0, AsscCaseD))
	{
		for(int j=0; j<2; ++j)
		{
			if(!XloutD.bIJ[j]) continue;

			const auto& AsscB =  AsscCaseD.AsscB[j];
			if(AsscB.ComD.bChk)
			{
				BOOL bChk = FALSE;
				XloutD.mShear.Lookup(AsscB.ComD.RCaseK, bChk);
				if(!bChk) continue;

				T_ALCS_D AlcsD;
				if (!m_pDoc->m_pAttrCtrl2->GetAlcs(AsscB.ComD.RCaseK, AlcsD)) ASSERT(0); // Rating case

				BOOL bIsWagon = IsWagon(AlcsD.nLiveType);

				arData.Add(D_CELL(nIncreaseRow,  1, GetPartNameWithNode(AsscB.ComD.ElemK, AsscB.ComD.unPosi)));
				arData.Add(D_CELL(nIncreaseRow,  3, m_pDoc->m_pPostCtrl->GetRatingCaseNameBD21(AsscB.ComD.RCaseK, AsscB.ComD.unConcurrent)));

				arData.Add(D_CELL(nIncreaseRow,  7, GetOutputFormat(AsscB.dSd  *m_dFactorForce, 3), D_NUMBER_FORMAT_3)); // loading Dead
				arData.Add(D_CELL(nIncreaseRow, 10, GetOutputFormat(AsscB.dSst *m_dFactorForce, 3), D_NUMBER_FORMAT_3)); // loading Live
				arData.Add(D_CELL(nIncreaseRow, 13, GetOutputFormat(AsscB.dS   *m_dFactorForce, 3), D_NUMBER_FORMAT_3)); // loading Dynamic
				arData.Add(D_CELL(nIncreaseRow, 16, GetOutputFormat(AsscB.dRA  *m_dFactorForce, 3), D_NUMBER_FORMAT_3)); // Capacity

				arData.Add(D_CELL(nIncreaseRow, 19, bIsWagon ? SYM_DASH : GetOutputFormatWithInfinity(AsscB.RatPa.dSBSU, 3, cMaxBSU, TRUE, 1), D_NUMBER_FORMAT_3)); // BSU Static
				arData.Add(D_CELL(nIncreaseRow, 22, bIsWagon ? SYM_DASH : GetOutputFormatWithInfinity(AsscB.RatPa.dDBSU, 3, cMaxBSU, TRUE, 1), D_NUMBER_FORMAT_3)); // BSU Dynamic

				arData.Add(D_CELL(nIncreaseRow, 25, bIsWagon ? SYM_DASH : GetRANumber(static_cast<int>(AsscB.RatPa.dSRA)))); // RA Static
				arData.Add(D_CELL(nIncreaseRow, 28, bIsWagon ? SYM_DASH : GetRANumber(static_cast<int>(AsscB.RatPa.dDRA)))); // RA Dynamic

				arData.Add(D_CELL(nIncreaseRow, 31, GetOutputFormat(AsscB.RatPa.dSUforRA, 3), D_NUMBER_FORMAT_3)); // U for RA8 Static
				arData.Add(D_CELL(nIncreaseRow, 34, GetOutputFormat(AsscB.RatPa.dDUforRA, 3), D_NUMBER_FORMAT_3)); // U for RA8 Dynamic

				arData.Add(D_CELL(nIncreaseRow, 37, GetAssessedCategory(AsscB.RatPa.nSAC), _T(""), GetAssessedCategoryColor(AsscB.RatPa.nSAC)));	// Assessed Category Static
				arData.Add(D_CELL(nIncreaseRow, 40, GetAssessedCategory(AsscB.RatPa.nDAC), _T(""), GetAssessedCategoryColor(AsscB.RatPa.nDAC)));	// Assessed Category Dynamic

				nIncreaseRow++;
			}
		}
	}
	if(nIncreaseRow==0)
	{
		arData.Add(D_CELL(nIncreaseRow, 1, SYM_DASH));
		arData.Add(D_CELL(nIncreaseRow, 3, SYM_DASH));
		for(int k=0; k<12; ++k)
		{
			const int iCol = 7+(k*3);
			arData.Add(D_CELL(nIncreaseRow, iCol, SYM_DASH));
		}
		nIncreaseRow++;
	}

	// Write Data
	CString RptBlock;
	int rowCnt = 0;

	// Table Title
	RptBlock = _T("Rating_Summary_Shear");
	rowCnt = CopyRange(pExcel, RptBlock, stRow);
	stRow += rowCnt;
	// Table Head
	RptBlock = _T("Rating_Summary_Shear_Table_Head");
	rowCnt = CopyRange(pExcel, RptBlock, stRow);
	stRow += rowCnt;

	for(int i=0; i<nIncreaseRow; ++i)
	{
		if(i==(nIncreaseRow-1))
		{
			RptBlock = _T("Rating_Summary_Table_End");
		}
		else
		{
			RptBlock = _T("Rating_Summary_Table_Body");
		}
		rowCnt = CopyRange(pExcel, RptBlock, stRow+i);
	}

	Print_Array_D_CELL(pExcel, arData, stRow);
	stRow += (nIncreaseRow+1);
}

void CRating_NR2006_STL_XLOut::GenerateSumTransverseStifferTable(IExcel* pExcel, const _T_NR2006_STL_XLOUT& XloutD, int& stRow, bool bBearing)
{
	int nIncreaseRow[3] = {0, 0, 0};
	// Make Data
	CArray<D_CELL, D_CELL&> arData[enNR_TS_Check::EN_NR_TS_Buckling];
	for(int i=0; i<enNR_TS_Check::EN_NR_TS_Buckling; ++i)
	{
		arData[i].RemoveAll();
	}

	T_ELEM_D ElemD; ElemD.Initialize();
	if(!m_pDoc->m_pAttrCtrl->GetElem(XloutD.ElemK, ElemD)) {
		ASSERT(0);  return;
	}
	T_CGTS_SSM_D CgtsSSM;
	bool bExistCGTS = m_pDoc->m_pAttrCtrl2->GetCgtsSsm(ElemD.elpro, CgtsSSM);
	
	if(bExistCGTS)
	{
		for(int i=0; i<enNR_TS_Check::EN_NR_TS_Buckling; ++i)
		{
			T_ASBS_SG_BS_CASE AsbsCaseD;
			if(m_pDoc->m_pPostCtrl->GetAnalysisResult()->GetBearingStiffenerRating4NR_GN_CIV(ElemPairK(XloutD.ElemK, EN_EL_BEAM), 0, 0, static_cast<enNR_TS_Check>(i+1), AsbsCaseD))
			{
				for(int j=0; j<2; ++j) // I, J
				{
					if(!XloutD.bIJ[j]) continue;

					const auto& StiffD = CgtsSSM.bSameJ ? CgtsSSM.aStiffener[0] : CgtsSSM.aStiffener[j];
					if(bBearing)
					{
						if(!StiffD.bUseStifBearing) continue;
					}
					else
					{
						if(StiffD.bUseStifBearing) continue;
					}

					const auto AsbsB = AsbsCaseD.AsbsB[j];
					if(AsbsB.ComD.bChk)
					{
						BOOL bChk = FALSE;
						XloutD.mStiff.Lookup(AsbsB.ComD.RCaseK, bChk);
						if(!bChk) continue;
						
						T_ALCS_D AlcsD;
						if (!m_pDoc->m_pAttrCtrl2->GetAlcs(AsbsB.ComD.RCaseK, AlcsD)) ASSERT(0); // Rating case

						BOOL bIsWagon = IsWagon(AlcsD.nLiveType);

						double dDead=0.0, dStatic=0.0, dDynamic=0.0, dCapacity=0.0;
						T_ASCA_RAT_RAIL_BS_B RatPa;
						switch(i+1)
						{
							case enNR_TS_Check::EN_NR_TS_Web_Yielding:
							{
								dDead	  = AsbsB.YieldingWeb.dSigmaD*m_dFactorStress;
								dStatic   = AsbsB.YieldingWeb.dSigmaSt*m_dFactorStress;
								dDynamic  = AsbsB.YieldingWeb.dSigmaDy*m_dFactorStress;
								dCapacity = AsbsB.YieldingWeb.dSigmaRA*m_dFactorStress;
								RatPa     = AsbsB.YieldingWeb.RatPa;
							}
							break;
							case enNR_TS_Check::EN_NR_TS_Yielding:
							{
								dDead	  = AsbsB.YieldingStiffener.dSigmaD*m_dFactorStress;
								dStatic   = AsbsB.YieldingStiffener.dSigmaSt*m_dFactorStress;
								dDynamic  = AsbsB.YieldingStiffener.dSigmaDy*m_dFactorStress;
								dCapacity = AsbsB.YieldingStiffener.dSigmaRA*m_dFactorStress;
								RatPa     = AsbsB.YieldingStiffener.RatPa;
							}
							break;
							case enNR_TS_Check::EN_NR_TS_Buckling:
							{
								dDead	  = AsbsB.BucklingEffStiffener.dValD;
								dStatic   = AsbsB.BucklingEffStiffener.dValSt;
								dDynamic  = AsbsB.BucklingEffStiffener.dValDy;
								dCapacity = AsbsB.BucklingEffStiffener.dValRA;
								RatPa     = AsbsB.BucklingEffStiffener.RatPa;
							}
							break;
							default: ASSERT(0);
							break;
						}

						arData[i].Add(D_CELL(nIncreaseRow[i],  1, GetPartNameWithNode(AsbsB.ComD.ElemK, AsbsB.ComD.unPosi)));
						arData[i].Add(D_CELL(nIncreaseRow[i],  3, m_pDoc->m_pPostCtrl->GetRatingCaseNameBD21(AsbsB.ComD.RCaseK, AsbsB.ComD.unConcurrent)));
						
						arData[i].Add(D_CELL(nIncreaseRow[i],  7, GetOutputFormat(dDead    , 3), D_NUMBER_FORMAT_3));
						arData[i].Add(D_CELL(nIncreaseRow[i], 10, GetOutputFormat(dStatic  , 3), D_NUMBER_FORMAT_3));
						arData[i].Add(D_CELL(nIncreaseRow[i], 13, GetOutputFormat(dDynamic , 3), D_NUMBER_FORMAT_3));
						arData[i].Add(D_CELL(nIncreaseRow[i], 16, GetOutputFormat(dCapacity, 3), D_NUMBER_FORMAT_3));

						arData[i].Add(D_CELL(nIncreaseRow[i], 19, bIsWagon ? SYM_DASH : GetOutputFormatWithInfinity(RatPa.dSBSU, 3, cMaxBSU, TRUE, 1), D_NUMBER_FORMAT_3));
						arData[i].Add(D_CELL(nIncreaseRow[i], 22, bIsWagon ? SYM_DASH : GetOutputFormatWithInfinity(RatPa.dDBSU, 3, cMaxBSU, TRUE, 1), D_NUMBER_FORMAT_3));

						arData[i].Add(D_CELL(nIncreaseRow[i], 25, bIsWagon ? SYM_DASH : GetRANumber(static_cast<int>(RatPa.dSRA)))); // RA Static
						arData[i].Add(D_CELL(nIncreaseRow[i], 28, bIsWagon ? SYM_DASH : GetRANumber(static_cast<int>(RatPa.dDRA)))); // RA Dynamic

						arData[i].Add(D_CELL(nIncreaseRow[i], 31, GetOutputFormat(RatPa.dSUforRA, 3), D_NUMBER_FORMAT_3));
						arData[i].Add(D_CELL(nIncreaseRow[i], 34, GetOutputFormat(RatPa.dDUforRA, 3), D_NUMBER_FORMAT_3));

						arData[i].Add(D_CELL(nIncreaseRow[i], 37, GetAssessedCategory(RatPa.nSAC), _T(""), GetAssessedCategoryColor(RatPa.nSAC)));	// Assessed Category Static
						arData[i].Add(D_CELL(nIncreaseRow[i], 40, GetAssessedCategory(RatPa.nDAC), _T(""), GetAssessedCategoryColor(RatPa.nDAC)));	// Assessed Category Dynamic

						nIncreaseRow[i] += 1;
					}
				}
			}
		}
	}

	for(int i=0; i<3; ++i)
	{
		if(nIncreaseRow[i]==0)
		{
			arData[i].Add(D_CELL(nIncreaseRow[i], 1, SYM_DASH));
			arData[i].Add(D_CELL(nIncreaseRow[i], 3, SYM_DASH));
			for(int k=0; k<12; ++k)
			{
				const int iCol = 7+(k*3);
				arData[i].Add(D_CELL(nIncreaseRow[i], iCol, SYM_DASH));
			}
			nIncreaseRow[i] += 1;
		}
	}

	// Write Data
	CString RptBlock;
	int rowCnt = 0;

	// Table Title
	RptBlock = bBearing ? _T("Rating_Summary_Bearing_STIFF") : _T("Rating_Summary_Inter_STIFF");
	rowCnt = CopyRange(pExcel, RptBlock, stRow);
	stRow += rowCnt;
	for(int j=0; j<3; ++j)
	{
		// Table Title
		RptBlock = [](const int& iIdx)
		{
			switch(iIdx)
			{
				case 0: return _T("Rating_Summary_STIFF_Web_Yielding");
				case 1: return _T("Rating_Summary_STIFF_Yielding");
				case 2: return _T("Rating_Summary_STIFF_Buckling");
				default: ASSERT(0); return _T("");
			}
		}(j);
		rowCnt = CopyRange(pExcel, RptBlock, stRow);
		stRow += rowCnt;

		// Table Head
		RptBlock = RptBlock = [](const int& iIdx)
		{
			switch(iIdx)
			{
				case 0:
				case 1: return _T("Rating_Summary_STIFF_Yield_Table_Head");
				case 2: return _T("Rating_Summary_STIFF_Buck_Table_Head");
				default: ASSERT(0); return _T("");
			}
		}(j);
		rowCnt = CopyRange(pExcel, RptBlock, stRow);
		stRow += rowCnt;

		for(int i=0; i<nIncreaseRow[j]; ++i)
		{
			if(i==(nIncreaseRow[j]-1))
			{
				RptBlock = _T("Rating_Summary_Table_End");
			}
			else
			{
				RptBlock = _T("Rating_Summary_Table_Body");
			}
			rowCnt = CopyRange(pExcel, RptBlock, stRow+i);
		}

		Print_Array_D_CELL(pExcel, arData[j], stRow);
		stRow += nIncreaseRow[j]+1;		
	}
}

void CRating_NR2006_STL_XLOut::SetAssessmentSheetPage(IExcel* pExcel, const T_ELEM_K ElemK, const int& iPosi)
{
	m_strBaseSheet = _T("Assessment(Base)");
	CString strSheetName;
	strSheetName.Format(_T("Asessment_%d(%s)"), ElemK, (iPosi==0 ? _T("I") : _T("J")));
	SetSheetPage(pExcel, strSheetName);	
}

void CRating_NR2006_STL_XLOut::GenerateAssementSheet(IExcel* pExcel, const int& iPosi, const _T_NR2006_STL_XLOUT& XloutD)
{
	CString RptBlock;
	int stRow  = 0;
	int rowCnt = 0;

	// 1. Design Condition
	RptBlock = _T("Rating_Title_Dgn_Condition");
	rowCnt = CopyRange(pExcel, RptBlock, stRow);

	CString strCode = CDBLib::GetStlRatCodeName(m_pDoc->GetCivilCodeDgn());
	CString strPosi = GetPartNameWithNode(XloutD.ElemK, iPosi);
	CString strType = GetMembType(XloutD.ElemK);

	CArray<D_CELL, D_CELL&> arData;
	arData.RemoveAll();
	arData.Add(D_CELL(1, 1, strCode));
	arData.Add(D_CELL(1, 8, GetOutputFormat(XloutD.ElemK, 0)));
	arData.Add(D_CELL(1, 12, strPosi));
	arData.Add(D_CELL(1, 16, strType));
	Print_Array_D_CELL(pExcel, arData, stRow+1);
	stRow += rowCnt+1;

	GenerateAssBendingTable(pExcel, iPosi, XloutD, ++stRow);
	GenerateAssShearTable(pExcel, iPosi, XloutD, ++stRow);
	GenerateAssTransverseStifferTable(pExcel, iPosi, XloutD, ++stRow);
}

void CRating_NR2006_STL_XLOut::GenerateAssBendingTable(IExcel* pExcel, const int& iPosi, const _T_NR2006_STL_XLOUT& XloutD, int& stRow)
{
	CArray<T_ALCS_K, T_ALCS_K> arAlcs; arAlcs.RemoveAll();
	m_pDoc->m_pAttrCtrl2->GetAlcsKeyList(arAlcs);
	int nAlcs = arAlcs.GetSize();
	if(nAlcs==0) ASSERT(0);

	int nConcurrentSize = GetConcurrentSize();

	int nIncreaseRow = 0;
	// Make Data
	CArray<D_CELL, D_CELL&> arData;
	arData.RemoveAll();
	for(int i=0; i<nAlcs; ++i)
	{
		T_ALCS_K AlcsK = arAlcs[i];
		BOOL bChk = FALSE;
		XloutD.mBend.Lookup(AlcsK, bChk);
		if(!bChk) continue;

		T_ALCS_D AlcsD;
		if(!m_pDoc->m_pAttrCtrl2->GetAlcs(AlcsK, AlcsD)) ASSERT(0); // Rating case

		BOOL bIsWagon = IsWagon(AlcsD.nLiveType);

		for(int j=0; j<nConcurrentSize; ++j) // concurrent
		{
			int iConCurrent = j+1;
			T_ASFC_CSG_BS_CASE AsfcCaseD;
			if(!m_pDoc->m_pPostCtrl->GetAnalysisResult()->GetFlexuralStrengthRating4CS454CSG(ElemPairK(XloutD.ElemK, EN_EL_BEAM), AlcsK, 0, iConCurrent, AsfcCaseD)) continue;

			const T_ASFC_CSG_BS_BASE& AsfcB =  AsfcCaseD.AsfcB[iPosi];

			// Set Data
			arData.Add(D_CELL(nIncreaseRow, 1, m_pDoc->m_pPostCtrl->GetRatingCaseNameBD21(AlcsK, iConCurrent)));
			if(AsfcB.ComD.bChk)
			{
				BOOL bSReverseM = (AsfcB.dSd * AsfcB.dSst < 0) || EQ0(AsfcB.dSst);
				BOOL bDReverseM = (AsfcB.dSd * AsfcB.dS < 0) || EQ0(AsfcB.dS);

				arData.Add(D_CELL(nIncreaseRow,  5, GetOutputFormat(AsfcB.dSd  *m_dFactorMoment, 3), D_NUMBER_FORMAT_3)); // loading Dead
				arData.Add(D_CELL(nIncreaseRow,  8, GetOutputFormat(AsfcB.dSst *m_dFactorMoment, 3), D_NUMBER_FORMAT_3)); // loading Live
				arData.Add(D_CELL(nIncreaseRow, 11, GetOutputFormat(AsfcB.dS   *m_dFactorMoment, 3), D_NUMBER_FORMAT_3)); // loading Dynamic
				
				if (bSReverseM && bDReverseM)
				{
					arData.Add(D_CELL(nIncreaseRow, 14, SYM_DASH));
				}
				else
				{
					arData.Add(D_CELL(nIncreaseRow, 14, GetOutputFormat(AsfcB.dRA * m_dFactorMoment, 3), D_NUMBER_FORMAT_3)); // Capacity
				}
				
				if (!bSReverseM)
				{
					arData.Add(D_CELL(nIncreaseRow, 17, bIsWagon ? SYM_DASH : GetOutputFormatWithInfinity(AsfcB.RatPa.dSBSU, 3, cMaxBSU, TRUE, 1), D_NUMBER_FORMAT_3)); // BSU Static
					arData.Add(D_CELL(nIncreaseRow, 23, bIsWagon ? SYM_DASH : GetRANumber(static_cast<int>(AsfcB.RatPa.dSRA)))); // RA Static
					arData.Add(D_CELL(nIncreaseRow, 29, GetOutputFormat(AsfcB.RatPa.dSUforRA, 3), D_NUMBER_FORMAT_3)); // U for RA8 Static
					arData.Add(D_CELL(nIncreaseRow, 35, GetAssessedCategory(AsfcB.RatPa.nSAC), _T(""), GetAssessedCategoryColor(AsfcB.RatPa.nSAC)));	// Assessed Category Static
				}
				else
				{
					arData.Add(D_CELL(nIncreaseRow, 17, SYM_DASH)); // BSU Static
					arData.Add(D_CELL(nIncreaseRow, 23, SYM_DASH)); // RA Static
					arData.Add(D_CELL(nIncreaseRow, 29, SYM_DASH)); // U for RA8 Static
					arData.Add(D_CELL(nIncreaseRow, 35, SYM_DASH));	// Assessed Category Static
				}

				if (!bDReverseM)
				{
					arData.Add(D_CELL(nIncreaseRow, 20, bIsWagon ? SYM_DASH : GetOutputFormatWithInfinity(AsfcB.RatPa.dDBSU, 3, cMaxBSU, TRUE, 1), D_NUMBER_FORMAT_3)); // BSU Dynamic
					arData.Add(D_CELL(nIncreaseRow, 26, bIsWagon ? SYM_DASH : GetRANumber(static_cast<int>(AsfcB.RatPa.dDRA)))); // RA Dynamic
					arData.Add(D_CELL(nIncreaseRow, 32, GetOutputFormat(AsfcB.RatPa.dDUforRA, 3), D_NUMBER_FORMAT_3)); // U for RA8 Dynamic
					arData.Add(D_CELL(nIncreaseRow, 38, GetAssessedCategory(AsfcB.RatPa.nDAC), _T(""), GetAssessedCategoryColor(AsfcB.RatPa.nDAC)));	// Assessed Category Dynamic
				}
				else
				{
					arData.Add(D_CELL(nIncreaseRow, 20, SYM_DASH)); // BSU Static
					arData.Add(D_CELL(nIncreaseRow, 26, SYM_DASH)); // RA Static
					arData.Add(D_CELL(nIncreaseRow, 32, SYM_DASH)); // U for RA8 Static
					arData.Add(D_CELL(nIncreaseRow, 38, SYM_DASH));	// Assessed Category Static
				}
			}
			else
			{
				for(int k=0; k<12; ++k)
				{
					const int iCol = 5+(k*3);
					arData.Add(D_CELL(nIncreaseRow, iCol, SYM_DASH));
				}
			}

			nIncreaseRow++;
		}
	}
	if(nIncreaseRow==0)
	{
		arData.Add(D_CELL(nIncreaseRow, 1, SYM_DASH));
		for(int k=0; k<12; ++k)
		{
			const int iCol = 5+(k*3);
			arData.Add(D_CELL(nIncreaseRow, iCol, SYM_DASH));
		}

		nIncreaseRow++;
	}

	// Write Data
	CString RptBlock;
	int rowCnt = 0;

	// Table Title
	RptBlock = _T("Rating_Title_Bending");
	rowCnt = CopyRange(pExcel, RptBlock, stRow);
	stRow += rowCnt;
	// Table Head
	RptBlock = _T("Rating_Bend_Table_Head");
	rowCnt = CopyRange(pExcel, RptBlock, stRow);
	stRow += rowCnt;

	for(int i=0; i<nIncreaseRow; ++i)
	{
		if(i==(nIncreaseRow-1))
		{
			RptBlock = _T("Rating_Table_End");
		}
		else
		{
			RptBlock = _T("Rating_Table_Body");
		}
		rowCnt = CopyRange(pExcel, RptBlock, stRow+i);
	}

	Print_Array_D_CELL(pExcel, arData, stRow);
	stRow += nIncreaseRow;
}

void CRating_NR2006_STL_XLOut::GenerateAssShearTable(IExcel* pExcel, const int& iPosi, const _T_NR2006_STL_XLOUT& XloutD, int& stRow)
{
	CArray<T_ALCS_K, T_ALCS_K> arAlcs; arAlcs.RemoveAll();
	m_pDoc->m_pAttrCtrl2->GetAlcsKeyList(arAlcs);
	int nAlcs = arAlcs.GetSize();
	if(nAlcs==0) ASSERT(0);

	int nConcurrentSize = GetConcurrentSize();

	int nIncreaseRow = 0;
	// Make Data
	CArray<D_CELL, D_CELL&> arData;
	arData.RemoveAll();
	for(int i=0; i<nAlcs; ++i)
	{
		T_ALCS_K AlcsK = arAlcs[i];
		BOOL bChk = FALSE;
		XloutD.mShear.Lookup(AlcsK, bChk);
		if ( !bChk ) continue;

		T_ALCS_D AlcsD;
		if(!m_pDoc->m_pAttrCtrl2->GetAlcs(AlcsK, AlcsD)) ASSERT(0); // Rating case

		BOOL bIsWagon = IsWagon(AlcsD.nLiveType);

		for(int j=0; j<nConcurrentSize; ++j) // concurrent
		{
			int iConCurrent = j+1;
			T_ASSC_CSG_BS_CASE AsscCaseD;
			if(!m_pDoc->m_pPostCtrl->GetAnalysisResult()->GetShearStrengthRating4CS454CSG(ElemPairK(XloutD.ElemK, EN_EL_BEAM), AlcsK, iConCurrent, AsscCaseD)) continue;

			// Set Data
			const auto AsscB = AsscCaseD.AsscB[iPosi];
			arData.Add(D_CELL(nIncreaseRow, 1, m_pDoc->m_pPostCtrl->GetRatingCaseNameBD21(AlcsK, iConCurrent)));
			if(AsscB.ComD.bChk)
			{
				arData.Add(D_CELL(nIncreaseRow,  5, GetOutputFormat(AsscB.dSd  *m_dFactorForce, 3), D_NUMBER_FORMAT_3)); // loading Dead
				arData.Add(D_CELL(nIncreaseRow,  8, GetOutputFormat(AsscB.dSst *m_dFactorForce, 3), D_NUMBER_FORMAT_3)); // loading Live
				arData.Add(D_CELL(nIncreaseRow, 11, GetOutputFormat(AsscB.dS   *m_dFactorForce, 3), D_NUMBER_FORMAT_3)); // loading Dynamic
				arData.Add(D_CELL(nIncreaseRow, 14, GetOutputFormat(AsscB.dRA  *m_dFactorForce, 3), D_NUMBER_FORMAT_3)); // Capacity

				arData.Add(D_CELL(nIncreaseRow, 17, bIsWagon ? SYM_DASH : GetOutputFormatWithInfinity(AsscB.RatPa.dSBSU, 3, cMaxBSU, TRUE, 1), D_NUMBER_FORMAT_3));
				arData.Add(D_CELL(nIncreaseRow, 20, bIsWagon ? SYM_DASH : GetOutputFormatWithInfinity(AsscB.RatPa.dDBSU, 3, cMaxBSU, TRUE, 1), D_NUMBER_FORMAT_3));

				arData.Add(D_CELL(nIncreaseRow, 23, bIsWagon ? SYM_DASH : GetRANumber(static_cast<int>(AsscB.RatPa.dSRA)))); // RA Static
				arData.Add(D_CELL(nIncreaseRow, 26, bIsWagon ? SYM_DASH : GetRANumber(static_cast<int>(AsscB.RatPa.dDRA)))); // RA Dynamic

				arData.Add(D_CELL(nIncreaseRow, 29, GetOutputFormat(AsscB.RatPa.dSUforRA, 3), D_NUMBER_FORMAT_3));
				arData.Add(D_CELL(nIncreaseRow, 32, GetOutputFormat(AsscB.RatPa.dDUforRA, 3), D_NUMBER_FORMAT_3));

				arData.Add(D_CELL(nIncreaseRow, 35, GetAssessedCategory(AsscB.RatPa.nSAC), _T(""), GetAssessedCategoryColor(AsscB.RatPa.nSAC)));	// Assessed Category Static
				arData.Add(D_CELL(nIncreaseRow, 38, GetAssessedCategory(AsscB.RatPa.nDAC), _T(""), GetAssessedCategoryColor(AsscB.RatPa.nDAC)));	// Assessed Category Dynamic
			}
			else
			{
				for(int k=0; k<12; ++k)
				{
					const int iCol = 5+(k*3);
					arData.Add(D_CELL(nIncreaseRow, iCol, SYM_DASH));
				}
			}

			nIncreaseRow++;
		}
	}
	if(nIncreaseRow==0)
	{
		arData.Add(D_CELL(nIncreaseRow, 1, SYM_DASH));
		for(int k=0; k<12; ++k)
		{
			const int iCol = 5+(k*3);
			arData.Add(D_CELL(nIncreaseRow, iCol, SYM_DASH));
		}

		nIncreaseRow++;
	}

	// Write Data
	CString RptBlock;
	int rowCnt = 0;

	// Table Title
	RptBlock = _T("Rating_Title_Shear");
	rowCnt = CopyRange(pExcel, RptBlock, stRow);
	stRow += rowCnt;
	// Table Head
	RptBlock = _T("Rating_Shear_Table_Head");
	rowCnt = CopyRange(pExcel, RptBlock, stRow);
	stRow += rowCnt;

	for(int i=0; i<nIncreaseRow; ++i)
	{
		if(i==(nIncreaseRow-1))
		{
			RptBlock = _T("Rating_Table_End");
		}
		else
		{
			RptBlock = _T("Rating_Table_Body");
		}
		rowCnt = CopyRange(pExcel, RptBlock, stRow+i);
	}

	Print_Array_D_CELL(pExcel, arData, stRow);
	stRow += nIncreaseRow;
}

void CRating_NR2006_STL_XLOut::GenerateAssTransverseStifferTable(IExcel* pExcel, const int& iPosi, const _T_NR2006_STL_XLOUT& XloutD, int& stRow)
{
	CArray<T_ALCS_K, T_ALCS_K> arAlcs; arAlcs.RemoveAll();
	m_pDoc->m_pAttrCtrl2->GetAlcsKeyList(arAlcs);
	int nAlcs = arAlcs.GetSize();
	if(nAlcs==0) ASSERT(0);

	int nConcurrentSize = GetConcurrentSize();

	int nTSType = [&]()
	{
		T_ELEM_D ElemD; ElemD.Initialize();
		if(!m_pDoc->m_pAttrCtrl->GetElem(XloutD.ElemK, ElemD)) {
			ASSERT(0);  return 0;
		}
		T_CGTS_SSM_D CgtsSSM;
		if(!m_pDoc->m_pAttrCtrl2->GetCgtsSsm(ElemD.elpro, CgtsSSM)) return 0;

		const auto& StiffD = CgtsSSM.bSameJ ? CgtsSSM.aStiffener[0] : CgtsSSM.aStiffener[iPosi];
		return StiffD.bUseStifBearing ? 2 : 1;
	}();

	int nIncreaseRow = 0;
	// Make Data
	CArray<D_CELL, D_CELL&> arData[3];
	arData[0].RemoveAll();
	arData[1].RemoveAll();
	arData[2].RemoveAll();
	if(nTSType > 0)
	{
		for(int i=0; i<nAlcs; ++i)
		{
			T_ALCS_K AlcsK = arAlcs[i];
			BOOL bChk = FALSE;
			XloutD.mStiff.Lookup(AlcsK, bChk);
			if(!bChk) continue;

			T_ALCS_D AlcsD;
			if(!m_pDoc->m_pAttrCtrl2->GetAlcs(AlcsK, AlcsD)) ASSERT(0); // Rating case

			BOOL bIsWagon = IsWagon(AlcsD.nLiveType);

			for(int j=0; j<nConcurrentSize; ++j) // concurrent
			{
				int iConCurrent = j+1;
				
				T_ASBS_SG_BS_BASE AsbsB[3];
				for(int k=0; k<enNR_TS_Check::EN_NR_TS_Buckling; ++k)
				{
					T_ASBS_SG_BS_CASE AsbsD;
					if(!m_pDoc->m_pPostCtrl->GetAnalysisResult()->GetBearingStiffenerRating4NR_GN_CIV(ElemPairK(XloutD.ElemK, EN_EL_BEAM), AlcsK, iConCurrent, static_cast<enNR_TS_Check>(k+1), AsbsD))  AsbsD.Initialize();
					AsbsB[k] = AsbsD.AsbsB[iPosi];
				}

				// Set Data
				if(AsbsB[0].ComD.bChk || AsbsB[1].ComD.bChk || AsbsB[2].ComD.bChk)
				{
					for(int k=0; k<enNR_TS_Check::EN_NR_TS_Buckling; ++k)
					{
						double dDead=0.0, dStatic=0.0, dDynamic=0.0, dCapacity=0.0;
						T_ASCA_RAT_RAIL_BS_B RatPa;
						switch(k+1)
						{
							case enNR_TS_Check::EN_NR_TS_Web_Yielding:
							{
								dDead	  = AsbsB[k].YieldingWeb.dSigmaD*m_dFactorStress;
								dStatic   = AsbsB[k].YieldingWeb.dSigmaSt*m_dFactorStress;
								dDynamic  = AsbsB[k].YieldingWeb.dSigmaDy*m_dFactorStress;
								dCapacity = AsbsB[k].YieldingWeb.dSigmaRA*m_dFactorStress;
								RatPa     = AsbsB[k].YieldingWeb.RatPa;
							}
							break;
							case enNR_TS_Check::EN_NR_TS_Yielding:
							{
								dDead	  = AsbsB[k].YieldingStiffener.dSigmaD*m_dFactorStress;
								dStatic   = AsbsB[k].YieldingStiffener.dSigmaSt*m_dFactorStress;
								dDynamic  = AsbsB[k].YieldingStiffener.dSigmaDy*m_dFactorStress;
								dCapacity = AsbsB[k].YieldingStiffener.dSigmaRA*m_dFactorStress;
								RatPa     = AsbsB[k].YieldingStiffener.RatPa;
							}
							break;
							case enNR_TS_Check::EN_NR_TS_Buckling:
							{
								dDead	  = AsbsB[k].BucklingEffStiffener.dValD;
								dStatic   = AsbsB[k].BucklingEffStiffener.dValSt;
								dDynamic  = AsbsB[k].BucklingEffStiffener.dValDy;
								dCapacity = AsbsB[k].BucklingEffStiffener.dValRA;
								RatPa     = AsbsB[k].BucklingEffStiffener.RatPa;
							}
							break;
							default: ASSERT(0); RatPa.Initialize();
							break;
						}

						arData[k].Add(D_CELL(nIncreaseRow,  1, m_pDoc->m_pPostCtrl->GetRatingCaseNameBD21(AlcsK, iConCurrent)));
						arData[k].Add(D_CELL(nIncreaseRow,  5, GetOutputFormat(dDead    , 3), D_NUMBER_FORMAT_3));
						arData[k].Add(D_CELL(nIncreaseRow,  8, GetOutputFormat(dStatic  , 3), D_NUMBER_FORMAT_3));
						arData[k].Add(D_CELL(nIncreaseRow, 11, GetOutputFormat(dDynamic , 3), D_NUMBER_FORMAT_3));
						arData[k].Add(D_CELL(nIncreaseRow, 14, GetOutputFormat(dCapacity, 3), D_NUMBER_FORMAT_3));

						arData[k].Add(D_CELL(nIncreaseRow, 17, bIsWagon ? SYM_DASH : GetOutputFormatWithInfinity(RatPa.dSBSU, 3, cMaxBSU, TRUE, 1), D_NUMBER_FORMAT_3));
						arData[k].Add(D_CELL(nIncreaseRow, 20, bIsWagon ? SYM_DASH : GetOutputFormatWithInfinity(RatPa.dDBSU, 3, cMaxBSU, TRUE, 1), D_NUMBER_FORMAT_3));

						arData[k].Add(D_CELL(nIncreaseRow, 23, bIsWagon ? SYM_DASH : GetRANumber(static_cast<int>(RatPa.dSRA)))); // RA Static
						arData[k].Add(D_CELL(nIncreaseRow, 26, bIsWagon ? SYM_DASH : GetRANumber(static_cast<int>(RatPa.dDRA)))); // RA Dynamic

						arData[k].Add(D_CELL(nIncreaseRow, 29, GetOutputFormat(RatPa.dSUforRA, 3), D_NUMBER_FORMAT_3));
						arData[k].Add(D_CELL(nIncreaseRow, 32, GetOutputFormat(RatPa.dDUforRA, 3), D_NUMBER_FORMAT_3));

						arData[k].Add(D_CELL(nIncreaseRow, 35, GetAssessedCategory(RatPa.nSAC), _T(""), GetAssessedCategoryColor(RatPa.nSAC)));	// Assessed Category Static
						arData[k].Add(D_CELL(nIncreaseRow, 38, GetAssessedCategory(RatPa.nDAC), _T(""), GetAssessedCategoryColor(RatPa.nDAC)));	// Assessed Category Dynamic
					}
					nIncreaseRow++;
				}
			}
		}
	}
	if(nIncreaseRow==0)
	{
		for(int i=0; i<3; ++i)
		{
			arData[i].Add(D_CELL(nIncreaseRow, 1, SYM_DASH));
			for(int k=0; k<12; ++k)
			{
				const int iCol = 5+(k*3);
				arData[i].Add(D_CELL(nIncreaseRow, iCol, SYM_DASH));
			}
		}
		nIncreaseRow++;
	}

	// Write Data
	CString RptBlock;
	int rowCnt = 0;

	// Table Title
	switch(nTSType)
	{
		case 0:	RptBlock = _T("Rating_Title_Trans_STIFF");		break;
		case 1:	RptBlock = _T("Rating_Title_Inter_STIFF");		break;
		case 2:	RptBlock = _T("Rating_Title_Bearing_STIFF");	break;
		default: ASSERT(0);
			    RptBlock = _T("Rating_Title_Trans_STIFF");		break;
	}
	rowCnt = CopyRange(pExcel, RptBlock, stRow);
	stRow += rowCnt;
	for(int j=0; j<3; ++j)
	{
		// Table Title
		RptBlock = [](const int& iIdx)
		{
			switch(iIdx)
			{
				case 0: return _T("Rating_Title_STIFF_Web_Yielding");
				case 1: return _T("Rating_Title_STIFF_Yielding");
				case 2: return _T("Rating_Title_STIFF_Buckling");
				default: ASSERT(0); return _T("");
			}
		}(j);
		rowCnt = CopyRange(pExcel, RptBlock, stRow);
		stRow += rowCnt;

		// Table Head
		RptBlock = RptBlock = [](const int& iIdx)
		{
			switch(iIdx)
			{
				case 0:
				case 1: return _T("Rating_STIFF_Yield_Table_Head");
				case 2: return _T("Rating_STIFF_Buck_Table_Head");
				default: ASSERT(0); return _T("");
			}
		}(j);
		rowCnt = CopyRange(pExcel, RptBlock, stRow);
		stRow += rowCnt;

		for(int i=0; i<nIncreaseRow; ++i)
		{
			if(i==(nIncreaseRow-1))
			{
				RptBlock = _T("Rating_Table_End");
			}
			else
			{
				RptBlock = _T("Rating_Table_Body");
			}
			rowCnt = CopyRange(pExcel, RptBlock, stRow+i);
		}

		Print_Array_D_CELL(pExcel, arData[j], stRow);
		stRow += nIncreaseRow;
		if(j<2)	stRow++;
	}
}

// for Detail Report
BOOL CRating_NR2006_STL_XLOut::GenerateDetailRpt(IExcel* pExcel, CString szProgramDir, CString szProjectFileDir, CString strPath, D_XL_PROGRESS_INTERFACE* pProgDlg)
{
	CArray<T_SRAR_K, T_SRAR_K> aPrintElemKey;
	m_pDoc->m_pAttrCtrl2->GetSrarKeyList(aPrintElemKey);
	ArrElemPairKey aPrintElemPairKey;
	CDBLib::ConvertToElemPairKey(EN_EL_BEAM, aPrintElemKey, aPrintElemPairKey);

	const int nDgnCode = NR_GN_CIV_025_06_CSG;
	CSG_RPT_CS457_D RptD;
	CRatingReportDataCtrl ReportDataCtrl(m_pDataCtrl, nDgnCode);
	ReportDataCtrl.Get_CS457_DetailResult(aPrintElemPairKey, RptD);

	CDgn_SteelRptManager_NR2006 RptMgr;
	RptMgr.Print_DetailReport(pExcel, pProgDlg, nDgnCode, szProgramDir, szProjectFileDir, strPath, RptD);

	return TRUE;
}

CString CRating_NR2006_STL_XLOut::GetMembType(const int& iElem)
{
	T_UFMT_D UfmtD;
	if(!m_pDoc->m_pAttrCtrl->GetUfmt(iElem, UfmtD)) UfmtD.Initialize();
	const int nType = UfmtD.nMemberType;  // by iElem
	switch(nType)// 0:Main Girder, 1:Cross Girder, 2:Rail Bearer
	{
		case 0: return _T("Main Girder");
		case 1: return _T("Cross Girder");
		case 2: return _T("Rail Bearer");
		default:
		break;
	}
	ASSERT(0);
	return _T("Main Girder");
}

CString CRating_NR2006_STL_XLOut::GetRANumber(const int& iRA)
{
	CString strRA;
	strRA.Format(_T("RA %d"), iRA);
	return strRA;
}

CString CRating_NR2006_STL_XLOut::GetAssessedCategory(const int& iAssCat)
{
	switch(iAssCat)
	{
		case D_NR_RAIL_ASSESSED_CATEGORY_A1:   return _T("A1");
		case D_NR_RAIL_ASSESSED_CATEGORY_A2:   return _T("A2");
		case D_NR_RAIL_ASSESSED_CATEGORY_A3:   return _T("A3");
		case D_NR_RAIL_ASSESSED_CATEGORY_B :   return _T("B");
		case D_NR_RAIL_ASSESSED_CATEGORY_C :   return _T("C");
		case D_NR_RAIL_ASSESSED_CATEGORY_D :   return _T("D");
		case D_NR_RAIL_ASSESSED_CATEGORY_E :   return _T("E");
		case D_NR_RAIL_ASSESSED_CATEGORY_F :   return _T("F");
		default:
		break;
	}
	//ASSERT(0);
	return _T("A1");
}

CString CRating_NR2006_STL_XLOut::GetAssessedCategoryColor(const int& iAssCat)
{
	// COLORREF CLibXLFormatManager::GetColorRgb(const CString& sColor)
	switch(iAssCat)
	{
		case D_NR_RAIL_ASSESSED_CATEGORY_A1:   
		case D_NR_RAIL_ASSESSED_CATEGORY_A2:  return _T("EMERALD_GREEN");
		case D_NR_RAIL_ASSESSED_CATEGORY_A3:  
		case D_NR_RAIL_ASSESSED_CATEGORY_B:   
		case D_NR_RAIL_ASSESSED_CATEGORY_C:   return _T("DEEP_ORANGE");
		case D_NR_RAIL_ASSESSED_CATEGORY_D:   
		case D_NR_RAIL_ASSESSED_CATEGORY_E:   
		case D_NR_RAIL_ASSESSED_CATEGORY_F:   return _T("BURGUNDY");
		default:
		break;
	}
	//ASSERT(0);
	return _T("BLUE");
}

BOOL CRating_NR2006_STL_XLOut::IsWagon(int nLiveType)
{
	return (nLiveType == EN_NR_LIVE_LOAD_Wagon);
}