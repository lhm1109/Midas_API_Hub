#include "stdafx.h"
#include "..\DgnEngine\Src\DgnCalcBase\DgnCalcBase_PSC_Struct.h"
#include "..\DgnEngine\Src\DgnCalcBase\DgnCalcBase_CSG_Struct.h"
#include "..\DgnEngine\Src\CVL_DgnCalc_US\DgnCalc_CSG_CS457_Struct.h"
#include "..\..\dgnengine\idesign\DGN_def\SectionPropertyDef.h"

#include "Rating_CS454_STL_XLOut.h"

#include "DgnDataCtrl.h"
#include "CRCDataCtrl.h"
#include "DgnCsgDataCtrl.h"
#include "DgnStlRatingDataCtrl.h"
#include "Dgn_CSGRptManager_CS457.h"
#include "RptSteelGirder_AASHTO12.h"

#include "..\wg_base\TestEnvMgr.h"
#include "..\wg_db\AttrCtrl.h"
#include "..\wg_db\AttrCtrl2.h"
#include "..\wg_db\PostCtrl.h"
#include "..\wg_db\AnalysisResult.h"

#include "..\..\DgnEngine\idesign\DGN_lib\ExcelGenerator.h"

using namespace dgn::lib;

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define D_NUMBER_FORMAT_3  "0.000_ "
const double cMaxRF = 1.0E+70;
#define DgnAbsDivide(a,b) fabs(b)>cDGN_Zero ? fabs(a/b) : cMaxRF

CRating_CS454_STL_XLOut::CRating_CS454_STL_XLOut(void) : CDgn_XLOut()// CRating_XLOut()
{
    m_pDoc = CDBDoc::GetDocPoint();

    if ( m_pDoc!=NULL )
    {
        m_pDgnResult = m_pDoc->m_pPostCtrl->GetAnalysisResult();
    }    

}


CRating_CS454_STL_XLOut::~CRating_CS454_STL_XLOut(void)
{
}


void CRating_CS454_STL_XLOut::SetDataCtrlPointer(CCRCDataCtrl* pDataCtrl)
{
    m_pDataCtrl = pDataCtrl;
}

void CRating_CS454_STL_XLOut::PrintReport(CString szProgramDir, CString szProjectFileDir, CString strPath, int iPrintOpt, CArray<_T_CS454_STL_XLOUT, _T_CS454_STL_XLOUT&>& arXlout)
{
    m_strBasePath   = szProgramDir;
    m_strDataPath   = szProjectFileDir;

    CDgnEngineProgressDlg ProDlg;
    ProDlg.Create_ProgressDlg(PROGRESS_TYPE_STEEL_XL_RPT);// PSC Rating XL Report -> 202

    m_strBasePath = m_pDoc->GetProgramPath() + _T("Excel Base File\\");
    m_strBaseBook = _T("Steel Composite_CS457.xlsx");

    // Prevent Server Busy Dialog Box
    // https://docs.microsoft.com/ko-kr/cpp/mfc/reference/colemessagefilter-class?view=vs-2019
    COleMessageFilter* pFilter = AfxOleGetMessageFilter();
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
    
    BOOL bTest = CDBLib::IsDesignTestOutputOK();

    SetOutputUnitSystem();
    SetUnitConvertFactor();

    int nElem = arXlout.GetSize();
    int nProgressCount = 1;
    int nProgressTotal = 1 + 2*nElem + GetDetailCount(); // Summary + Assessment(IJ*Element) + Detail

    BOOL bIsRegressionTest = CDBDoc::GetDocPoint()->IsRegTestMode();

    if ( !bIsRegressionTest )
    {
        // #1. Summary
        //SetSummarySheetPage(pXL);
        //GenerateSummarySheet(pXL, arXlout);
        if ( ProDlg.Update_ProgressDlg(nProgressCount++, nProgressTotal) ) { pXL->Quit(); delete pXL; return; }

        for ( int i=0; i<nElem; ++i )
        {
            for ( int j=0; j<2; ++j ) // Position
            {
                if ( !arXlout[i].bIJ[j] )
                {
                    if ( ProDlg.Update_ProgressDlg(nProgressCount++, nProgressTotal) ) { pXL->Quit(); delete pXL; return; }// #2. Assessment
                    continue;
                }

                if ( !bTest )
                {
                    // #2. Assessment
                    SetAssessmentSheetPage(pXL, arXlout[i].ElemK, j);
                    GenerateAssementSheet(pXL, j, arXlout[i]);
                }
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

    //// Delete Default Sheet and Save.
    if ( !bIsRegressionTest )
    {
        pXL->EndExcel(strPath);
        //pXL->DeleteDefaultSheet();
        //pXL->SaveAs(pXL->GetBookName(), strPath);//* 출력파일 저쟝..
        //pXL->Quit();
        //delete pXL;
    }
    if(pXL != nullptr)  delete pXL;

    ProDlg.Delete_ProgressDlg();

    if ( strPath.Find(_T(".txt")) == -1 ) // RT 이후 텍스트 파일 남기지 않기 위한 방법
    {
        ShellExecute(NULL, _T("open"), strPath, NULL, NULL, SW_SHOWNORMAL);
    }
}

CString CRating_CS454_STL_XLOut::GetFlexTableHeadName()
{
    return _T("Rating_Flex_Table_Head_CS454");
}

CString CRating_CS454_STL_XLOut::GetShearTableHeadName()
{
    return _T("Rating_Shear_Table_Head_CS454");
}

CString CRating_CS454_STL_XLOut::GetAssessFactorContentsName()
{
    return _T("Rating_Assessment_Factor_contents_CS454");
}


void CRating_CS454_STL_XLOut::SetSheetPage(IExcel* pExcel, CString strNameCurrent)
{
    if ( pExcel->IsExistSheetName(strNameCurrent) )
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

void CRating_CS454_STL_XLOut::SetSummarySheetPage(IExcel* pExcel)
{
    m_strBaseSheet = _T("Summary(Base)");
    SetSheetPage(pExcel, _T("Summary"));
}

void CRating_CS454_STL_XLOut::SetDetailSheetPage(IExcel* pExcel, const T_ELEM_K ElemK, const int& iPosi)
{
    m_strBaseSheet = _T("Detail(Base)");
    CString strSheetName;
    strSheetName.Format(_T("Detail_%d(%s)"), ElemK, (iPosi==0 ? _T("I") : _T("J")));
    SetSheetPage(pExcel, strSheetName);
}

void CRating_CS454_STL_XLOut::SetAssessmentSheetPage(IExcel* pExcel, const T_ELEM_K ElemK, const int& iPosi)
{
    m_strBaseSheet = _T("Assessment(Base)");
    CString strSheetName;
    strSheetName.Format(_T("Assessment_%d(%s)"), ElemK, (iPosi==0 ? _T("I") : _T("J")));
    SetSheetPage(pExcel, strSheetName);
}

void CRating_CS454_STL_XLOut::DelBaseSheetPage(IExcel* pExcel)
{
    //pExcel->DeleteSheet(_T("Summary(Base)"));
    pExcel->DeleteSheet(_T("Detail"));
    pExcel->DeleteSheet(_T("Assessment(Base)"));
}

void CRating_CS454_STL_XLOut::GenerateSummarySheet(IExcel* pExcel, CArray<_T_CS454_STL_XLOUT, _T_CS454_STL_XLOUT&>& arXlout)
{

}

void CRating_CS454_STL_XLOut::GenerateAssementSheet(IExcel* pExcel, const int& iPosi, const _T_CS454_STL_XLOUT& XloutD)
{
    CString RptBlock;
    int stRow  = 1;
    int rowCnt = 0;

    // 1. Design Condition
    RptBlock = _T("Rating_Title_Dgn_Condition");
    rowCnt = CopyRange(pExcel, RptBlock, stRow);

    CString strCode = CDBLib::GetStlRatCodeName(m_pDoc->GetCivilCodeDgn());
    CString strPosi = GetPartNameWithNode(XloutD.ElemK, iPosi);

    CArray<D_CELL, D_CELL &> arData;
    arData.RemoveAll();
    arData.Add(D_CELL(1, 1, strCode));
    arData.Add(D_CELL(1, 8, GetOutputFormat(XloutD.ElemK, 0)));
    arData.Add(D_CELL(1, 12, strPosi));
    Print_Array_D_CELL(pExcel, arData, stRow+1);
    stRow += rowCnt+1;

    // 2. Assessment factors
    RptBlock = _T("Rating_Title_Assessment_Factor");
    rowCnt = CopyRange(pExcel, RptBlock, stRow);
    stRow += rowCnt;

    RptBlock = GetAssessFactorContentsName();
    rowCnt = CopyRange(pExcel, RptBlock, stRow);
    stRow += rowCnt;

    GenerateAssFlexTable(pExcel, iPosi, XloutD, ++stRow);
    GenerateAssShearTable(pExcel, iPosi, XloutD, ++stRow);    
    GenerateAssMomentShearTable(pExcel, iPosi, XloutD, ++stRow);
    GenerateAssLongitudinalShearTable(pExcel, true, iPosi, XloutD, ++stRow);
    GenerateAssBoxFlangeTable(pExcel, iPosi, XloutD, ++stRow);
    GenerateAssBoxFlangeStiffenerTable(pExcel, iPosi, XloutD, ++stRow);
    GenerateAssWebYieldingTable(pExcel, iPosi, XloutD, ++stRow);
    GenerateAssWebBucklingTable(pExcel, iPosi, XloutD, ++stRow);
    GenerateAssWebStiffenerTable(pExcel, iPosi, XloutD, ++stRow);
    GenerateAssSLSTable(pExcel, iPosi, XloutD, ++stRow);
    //GenerateAssLongitudinalShearTable(pExcel, false, iPosi, XloutD, ++stRow);

}

void CRating_CS454_STL_XLOut::GenerateAssFlexTable(IExcel* pExcel, const int& iPosi, const _T_CS454_STL_XLOUT& XloutD, int& stRow)
{
    CArray<T_ALCS_K, T_ALCS_K> arAlcs; arAlcs.RemoveAll();
    m_pDoc->m_pAttrCtrl2->GetAlcsKeyList(arAlcs);
    int nAlcs = arAlcs.GetSize();
    if ( nAlcs==0 ) ASSERT(0);

    int nConcurrentSize = GetConcurrentSize();

    int nIncreaseRow = 0;
    // Make Data
    CArray<D_CELL, D_CELL &> arData;
    arData.RemoveAll();
    for ( int i=0; i<nAlcs; ++i )
    {
        T_ALCS_K AlcsK = arAlcs[i];
        BOOL bChk[2] = { FALSE, FALSE };
        XloutD.mPosiFlex.Lookup(AlcsK, bChk[0]);
        XloutD.mNegaFlex.Lookup(AlcsK, bChk[1]);

        if ( !bChk[0] && !bChk[1] ) continue;

        T_ALCS_D AlcsD;
        if ( !m_pDoc->m_pAttrCtrl2->GetAlcs(AlcsK, AlcsD) ) ASSERT(0); // Rating case

        for ( int j=0; j<nConcurrentSize; ++j ) // concurrent
        {
            int iConCurrent = j+1;
            T_ASFC_CSG_BS_CASE AsfcCaseD;
            if ( !m_pDoc->m_pPostCtrl->GetAnalysisResult()->GetFlexuralStrengthRating4CS454CSG(XloutD.GetElemPairK(), AlcsK, 0, iConCurrent, AsfcCaseD)) continue;

            const T_ASFC_CSG_BS_BASE& AsfcB =  AsfcCaseD.AsfcB[iPosi];
            int k = AsfcB.dSA > 0.0 ? 0 : 1; // Positive/Negative

            BOOL bAdd = FALSE;
            if ( k==0 && AsfcB.dSA > 0.0 ) bAdd = TRUE;
            else if ( k==1 && AsfcB.dSA <= 0.0 ) bAdd = TRUE;

            if ( bAdd == FALSE ) continue;

            if (AsfcB.ComD.IsStiffener()) continue;

            // Set Data
            arData.Add(D_CELL(nIncreaseRow, 1, m_pDoc->m_pPostCtrl->GetRatingCaseNameBD21(AlcsK, iConCurrent)));
            arData.Add(D_CELL(nIncreaseRow, 5, (k==0 ? _T("Positive") : _T("Negative"))));
            if ( AsfcB.ComD.bChk )
            {
                arData.Add(D_CELL(nIncreaseRow, 8, GetOutputFormat(AsfcB.dRA *m_dFactorMoment, 3), D_NUMBER_FORMAT_3));
                arData.Add(D_CELL(nIncreaseRow, 11, GetOutputFormat(AsfcB.dS  *m_dFactorMoment, 3), D_NUMBER_FORMAT_3));
                arData.Add(D_CELL(nIncreaseRow, 14, GetOutputFormat(AsfcB.dSd *m_dFactorMoment, 3), D_NUMBER_FORMAT_3));
                arData.Add(D_CELL(nIncreaseRow, 20, GetOutputFormat(AsfcB.dSA *m_dFactorMoment, 3), D_NUMBER_FORMAT_3));
                arData.Add(D_CELL(nIncreaseRow, 23, GetOutputFormatWithInfinity(AsfcB.dA, 3, cMaxRF, FALSE), D_NUMBER_FORMAT_3));

                // CIVIL-1063 khj0102
                if ( AlcsD.nMovType == 1 ) //Special인 경우
                {
                    arData.Add(D_CELL(nIncreaseRow, 17, _T("-")));
                    arData.Add(D_CELL(nIncreaseRow, 25, _T("-")));
                    if ( abs(AsfcB.dPsi_ast) - cDGN_Zero < 0.0 )
                    {
                        arData.Add(D_CELL(nIncreaseRow, 27, _T("-")));
                    }
                    else if ( AsfcB.dPsi_ast + cDGN_Zero > cMaxRF )
                    {
                        arData.Add(D_CELL(nIncreaseRow, 27, _T("-")));
                    }
                    else
                    {
                        arData.Add(D_CELL(nIncreaseRow, 27, GetOutputFormatWithInfinity(AsfcB.dPsi_ast, 3, cMaxRF, FALSE), D_NUMBER_FORMAT_3));
                    }
                }
                else //Combined인 경우
                {
                    arData.Add(D_CELL(nIncreaseRow, 17, GetOutputFormat(AsfcB.dSst*m_dFactorMoment, 3), D_NUMBER_FORMAT_3));
                    if ( abs(AsfcB.dPsi) - cDGN_Zero < 0.0 )
                    {
                        arData.Add(D_CELL(nIncreaseRow, 25, _T("-")));
                    }
                    else if ( AsfcB.dPsi + cDGN_Zero > cMaxRF )
                    {
                        arData.Add(D_CELL(nIncreaseRow, 25, _T("-")));
                    }
                    else
                    {
                        arData.Add(D_CELL(nIncreaseRow, 25, GetOutputFormatWithInfinity(AsfcB.dPsi, 3, cMaxRF, FALSE), D_NUMBER_FORMAT_3));
                    }

                    if ( abs(AsfcB.dPsi_ast) - cDGN_Zero < 0.0 )
                    {
                        arData.Add(D_CELL(nIncreaseRow, 27, _T("-")));
                    }
                    else if ( AsfcB.dPsi_ast + cDGN_Zero > cMaxRF )
                    {
                        arData.Add(D_CELL(nIncreaseRow, 27, _T("-")));
                    }
                    else
                    {
                        arData.Add(D_CELL(nIncreaseRow, 27, GetOutputFormatWithInfinity(AsfcB.dPsi_ast, 3, cMaxRF, FALSE), D_NUMBER_FORMAT_3));
                    }
                }
                arData.Add(D_CELL(nIncreaseRow, 29, (AsfcB.dRF < 1.0 ? _T("NG") : _T("OK"))));
            }
            else
            {
                arData.Add(D_CELL(nIncreaseRow, 8, _T("-")));
                arData.Add(D_CELL(nIncreaseRow, 11, _T("-")));
                arData.Add(D_CELL(nIncreaseRow, 14, _T("-")));
                arData.Add(D_CELL(nIncreaseRow, 17, _T("-")));
                arData.Add(D_CELL(nIncreaseRow, 20, _T("-")));
                arData.Add(D_CELL(nIncreaseRow, 23, _T("-")));
                arData.Add(D_CELL(nIncreaseRow, 25, _T("-")));
                arData.Add(D_CELL(nIncreaseRow, 27, _T("-")));
                arData.Add(D_CELL(nIncreaseRow, 29, _T("-")));
            }

            nIncreaseRow++;
        }
    }
    if ( nIncreaseRow==0 )
    {
        arData.Add(D_CELL(nIncreaseRow, 1, _T("-")));
        arData.Add(D_CELL(nIncreaseRow, 5, _T("-")));
        arData.Add(D_CELL(nIncreaseRow, 8, _T("-")));
        arData.Add(D_CELL(nIncreaseRow, 11, _T("-")));
        arData.Add(D_CELL(nIncreaseRow, 14, _T("-")));
        arData.Add(D_CELL(nIncreaseRow, 17, _T("-")));
        arData.Add(D_CELL(nIncreaseRow, 20, _T("-")));
        arData.Add(D_CELL(nIncreaseRow, 23, _T("-")));
        arData.Add(D_CELL(nIncreaseRow, 25, _T("-")));
        arData.Add(D_CELL(nIncreaseRow, 27, _T("-")));
        arData.Add(D_CELL(nIncreaseRow, 29, _T("-")));

        nIncreaseRow++;
    }

    // Write Data
    CString RptBlock;
    int rowCnt = 0;

    // Table Title
    RptBlock = _T("Rating_Title_Flex");
    rowCnt = CopyRange(pExcel, RptBlock, stRow);
    stRow += rowCnt;
    // Table Head
    RptBlock = GetFlexTableHeadName();
    rowCnt = CopyRange(pExcel, RptBlock, stRow);
    stRow += rowCnt;

    for ( int i=0; i<nIncreaseRow; ++i )
    {
        if ( i==(nIncreaseRow-1) )
        {
            RptBlock = _T("Rating_Flex_Table_End");
        }
        else
        {
            RptBlock = _T("Rating_Flex_Table_Body");
        }
        rowCnt = CopyRange(pExcel, RptBlock, stRow+i);
    }

    Print_Array_D_CELL(pExcel, arData, stRow);
    stRow += nIncreaseRow;
}

void CRating_CS454_STL_XLOut::GenerateAssShearTable(IExcel* pExcel, const int& iPosi, const _T_CS454_STL_XLOUT& XloutD, int& stRow)
{
    CArray<T_ALCS_K, T_ALCS_K> arAlcs; arAlcs.RemoveAll();
    m_pDoc->m_pAttrCtrl2->GetAlcsKeyList(arAlcs);
    int nAlcs = arAlcs.GetSize();
    if ( nAlcs==0 ) ASSERT(0);

    int nConcurrentSize = GetConcurrentSize();

    int nIncreaseRow = 0;
    // Make Data
    CArray<D_CELL, D_CELL &> arData;
    arData.RemoveAll();
    for ( int i=0; i<nAlcs; ++i )
    {
        T_ALCS_K AlcsK = arAlcs[i];
        BOOL bChk = FALSE;
        XloutD.mShear.Lookup(AlcsK, bChk);
        if ( !bChk ) continue;

        T_ALCS_D AlcsD;
        if ( !m_pDoc->m_pAttrCtrl2->GetAlcs(AlcsK, AlcsD) ) ASSERT(0); // Rating case

        for ( int j=0; j<nConcurrentSize; ++j ) // concurrent
        {
            int iConCurrent = j+1;
            T_ASSC_CSG_BS_CASE AsscCaseD;
            if ( !m_pDoc->m_pPostCtrl->GetAnalysisResult()->GetShearStrengthRating4CS454CSG(XloutD.GetElemPairK(), AlcsK, iConCurrent, AsscCaseD) ) continue;

            const T_ASSC_CSG_BS_BASE& AsscB =  AsscCaseD.AsscB[iPosi];

            if ( AsscB.ComD.IsStiffener() ) continue;

            // Set Data
            arData.Add(D_CELL(nIncreaseRow, 1, m_pDoc->m_pPostCtrl->GetRatingCaseNameBD21(AlcsK, iConCurrent)));
            if ( AsscB.ComD.bChk )
            {
                arData.Add(D_CELL(nIncreaseRow, 5, GetOutputFormat(AsscB.dRA *m_dFactorForce, 3), D_NUMBER_FORMAT_3));

                arData.Add(D_CELL(nIncreaseRow, 8, GetOutputFormat(AsscB.dS  *m_dFactorForce, 3), D_NUMBER_FORMAT_3));

                arData.Add(D_CELL(nIncreaseRow, 11, GetOutputFormat(AsscB.dSd *m_dFactorForce, 3), D_NUMBER_FORMAT_3));

                arData.Add(D_CELL(nIncreaseRow, 17, GetOutputFormat(AsscB.dSA *m_dFactorForce, 3), D_NUMBER_FORMAT_3));
                arData.Add(D_CELL(nIncreaseRow, 20, GetOutputFormatWithInfinity(AsscB.dA, 3, cMaxRF, FALSE), D_NUMBER_FORMAT_3));

                if ( AlcsD.nMovType == 1 ) //Special인 경우
                {
                    arData.Add(D_CELL(nIncreaseRow, 14, _T("-")));
                    arData.Add(D_CELL(nIncreaseRow, 22, _T("-")));
                    if ( abs(AsscB.dPsi_ast) - cDGN_Zero < 0.0 )
                    {
                        arData.Add(D_CELL(nIncreaseRow, 24, _T("-")));
                    }
                    else if ( AsscB.dPsi_ast + cDGN_Zero > cMaxRF )
                    {
                        arData.Add(D_CELL(nIncreaseRow, 24, _T("-")));
                    }
                    else
                    {
                        arData.Add(D_CELL(nIncreaseRow, 24, GetOutputFormatWithInfinity(AsscB.dPsi_ast, 3, cMaxRF, FALSE), D_NUMBER_FORMAT_3));
                    }
                }
                else //Combined인 경우
                {
                    arData.Add(D_CELL(nIncreaseRow, 14, GetOutputFormat(AsscB.dSst*m_dFactorForce, 3), D_NUMBER_FORMAT_3));
                    if ( abs(AsscB.dPsi) - cDGN_Zero < 0.0 )
                    {
                        arData.Add(D_CELL(nIncreaseRow, 22, _T("-")));
                    }
                    else if ( AsscB.dPsi + cDGN_Zero > cMaxRF )
                    {
                        arData.Add(D_CELL(nIncreaseRow, 22, _T("-")));
                    }
                    else
                    {
                        arData.Add(D_CELL(nIncreaseRow, 22, GetOutputFormatWithInfinity(AsscB.dPsi, 3, cMaxRF, FALSE), D_NUMBER_FORMAT_3));
                    }

                    if ( abs(AsscB.dPsi_ast) - cDGN_Zero < 0.0 )
                    {
                        arData.Add(D_CELL(nIncreaseRow, 24, _T("-")));
                    }
                    else if ( AsscB.dPsi_ast + cDGN_Zero > cMaxRF )
                    {
                        arData.Add(D_CELL(nIncreaseRow, 24, _T("-")));
                    }
                    else
                    {
                        arData.Add(D_CELL(nIncreaseRow, 24, GetOutputFormatWithInfinity(AsscB.dPsi_ast, 3, cMaxRF, FALSE), D_NUMBER_FORMAT_3));
                    }
                }
                arData.Add(D_CELL(nIncreaseRow, 26, (AsscB.dRF < 1.0 ? _T("NG") : _T("OK"))));
            }
            else
            {
                arData.Add(D_CELL(nIncreaseRow, 5, _T("-")));
                arData.Add(D_CELL(nIncreaseRow, 8, _T("-")));
                arData.Add(D_CELL(nIncreaseRow, 11, _T("-")));
                arData.Add(D_CELL(nIncreaseRow, 14, _T("-")));
                arData.Add(D_CELL(nIncreaseRow, 17, _T("-")));
                arData.Add(D_CELL(nIncreaseRow, 20, _T("-")));
                arData.Add(D_CELL(nIncreaseRow, 22, _T("-")));
                arData.Add(D_CELL(nIncreaseRow, 24, _T("-")));
                arData.Add(D_CELL(nIncreaseRow, 26, _T("-")));
            }

            nIncreaseRow++;
        }
    }
    if ( nIncreaseRow==0 )
    {
        arData.Add(D_CELL(nIncreaseRow, 1, _T("-")));
        arData.Add(D_CELL(nIncreaseRow, 5, _T("-")));
        arData.Add(D_CELL(nIncreaseRow, 8, _T("-")));
        arData.Add(D_CELL(nIncreaseRow, 11, _T("-")));
        arData.Add(D_CELL(nIncreaseRow, 14, _T("-")));
        arData.Add(D_CELL(nIncreaseRow, 17, _T("-")));
        arData.Add(D_CELL(nIncreaseRow, 20, _T("-")));
        arData.Add(D_CELL(nIncreaseRow, 22, _T("-")));
        arData.Add(D_CELL(nIncreaseRow, 24, _T("-")));
        arData.Add(D_CELL(nIncreaseRow, 26, _T("-")));

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
    RptBlock = GetShearTableHeadName();
    rowCnt = CopyRange(pExcel, RptBlock, stRow);
    stRow += rowCnt;

    for ( int i=0; i<nIncreaseRow; ++i )
    {
        if ( i==(nIncreaseRow-1) )
        {
            RptBlock = _T("Rating_Shear_Table_End");
        }
        else
        {
            RptBlock = _T("Rating_Shear_Table_Body");
        }
        rowCnt = CopyRange(pExcel, RptBlock, stRow+i);
    }

    Print_Array_D_CELL(pExcel, arData, stRow);
    stRow += nIncreaseRow;
}

void CRating_CS454_STL_XLOut::GenerateAssMomentShearTable(IExcel* pExcel, const int& iPosi, const _T_CS454_STL_XLOUT& XloutD, int& stRow)
{
    CArray<T_ALCS_K, T_ALCS_K> arAlcs; arAlcs.RemoveAll();
    m_pDoc->m_pAttrCtrl2->GetAlcsKeyList(arAlcs);
    INT_PTR nAlcs = arAlcs.GetSize();
    if ( nAlcs==0 ) return;

    int nConcurrentSize = GetConcurrentSize();

    int nIncreaseRow = 0;
    // Make Data
    CArray<D_CELL, D_CELL &> arData;
    arData.RemoveAll();
    for ( INT_PTR i=0; i<nAlcs; ++i )
    {
        T_ALCS_K AlcsK = arAlcs[i];
        BOOL bChk = FALSE;
        XloutD.mComb.Lookup(AlcsK, bChk);
        if ( !bChk ) continue;

        T_ALCS_D AlcsD;
        if ( !m_pDoc->m_pAttrCtrl2->GetAlcs(AlcsK, AlcsD) ) continue; // Rating case

        for ( int j=0; j<nConcurrentSize; ++j ) // concurrent
        {
            int iConCurrent = j+1;
            T_ASCB_CSG_BS_CASE AscbCaseD;
            if ( !m_pDoc->m_pPostCtrl->GetAnalysisResult()->GetCombinedMVStrength4CS454_CSGResult(XloutD.GetElemPairK(), AlcsK, iConCurrent, AscbCaseD) ) continue;

            const T_ASCB_CSG_BS_BASE& AscbB =  AscbCaseD.AscbB[iPosi];
            if ( AscbB.ComD.IsStiffener() ) continue;

            // Set Data
            arData.Add(D_CELL(nIncreaseRow, 1, m_pDoc->m_pPostCtrl->GetRatingCaseNameBD21(AlcsK, iConCurrent)));
            if ( AscbB.ComD.bChk )
            {
                arData.Add(D_CELL(nIncreaseRow, 5, GetOutputFormat(AscbB.dM_D *m_dFactorMoment, 3), D_NUMBER_FORMAT_3));
                arData.Add(D_CELL(nIncreaseRow, 8, GetOutputFormat(AscbB.dMf  *m_dFactorMoment, 3), D_NUMBER_FORMAT_3));
                arData.Add(D_CELL(nIncreaseRow, 11, GetOutputFormat(AscbB.dV_D *m_dFactorForce, 3), D_NUMBER_FORMAT_3));
                arData.Add(D_CELL(nIncreaseRow, 14, GetOutputFormat(AscbB.dV_R *m_dFactorForce, 3), D_NUMBER_FORMAT_3));
                arData.Add(D_CELL(nIncreaseRow, 17, GetOutputFormat(AscbB.dM_SV *m_dFactorMoment, 3), D_NUMBER_FORMAT_3));
                arData.Add(D_CELL(nIncreaseRow, 20, GetOutputFormat(AscbB.dV_SV *m_dFactorForce, 3), D_NUMBER_FORMAT_3));
                arData.Add(D_CELL(nIncreaseRow, 23, GetOutputFormat(AscbB.dM_DL *m_dFactorMoment, 3), D_NUMBER_FORMAT_3));
                arData.Add(D_CELL(nIncreaseRow, 26, GetOutputFormat(AscbB.dV_DL *m_dFactorForce, 3), D_NUMBER_FORMAT_3));
                arData.Add(D_CELL(nIncreaseRow, 29, GetOutputFormat(AscbB.dM_ST *m_dFactorMoment, 3), D_NUMBER_FORMAT_3));
                arData.Add(D_CELL(nIncreaseRow, 32, GetOutputFormat(AscbB.dV_ST *m_dFactorForce, 3), D_NUMBER_FORMAT_3));


                arData.Add(D_CELL(nIncreaseRow, 35, GetOutputFormatWithInfinity(AscbB.dA, 3, cMaxRF, FALSE), D_NUMBER_FORMAT_3));

                if ( AlcsD.nMovType == 1 ) //Special인 경우
                {
                    arData.Add(D_CELL(nIncreaseRow, 29, _T("-")));
                    arData.Add(D_CELL(nIncreaseRow, 32, _T("-")));
                    arData.Add(D_CELL(nIncreaseRow, 37, _T("-")));
                    if ( abs(AscbB.dPsi_ast) - cDGN_Zero < 0.0 )
                    {
                        arData.Add(D_CELL(nIncreaseRow, 39, _T("-")));
                    }
                    else if ( AscbB.dPsi_ast + cDGN_Zero > cMaxRF )
                    {
                        arData.Add(D_CELL(nIncreaseRow, 39, _T("-")));
                    }
                    else
                    {
                        arData.Add(D_CELL(nIncreaseRow, 39, GetOutputFormatWithInfinity(AscbB.dPsi_ast, 3, cMaxRF, FALSE), D_NUMBER_FORMAT_3));
                    }
                }
                else //Combined인 경우
                {
                    //arData.Add(D_CELL(nIncreaseRow, 14, GetOutputFormat(AsscB.dSst*m_dFactorForce, 3), D_NUMBER_FORMAT_3));
                    if ( abs(AscbB.dPsi) - cDGN_Zero < 0.0 )
                    {
                        arData.Add(D_CELL(nIncreaseRow, 37, _T("-")));
                    }
                    else if ( AscbB.dPsi + cDGN_Zero > cMaxRF )
                    {
                        arData.Add(D_CELL(nIncreaseRow, 37, _T("-")));
                    }
                    else
                    {
                        arData.Add(D_CELL(nIncreaseRow, 37, GetOutputFormatWithInfinity(AscbB.dPsi, 3, cMaxRF, FALSE), D_NUMBER_FORMAT_3));
                    }

                    if ( abs(AscbB.dPsi_ast) - cDGN_Zero < 0.0 )
                    {
                        arData.Add(D_CELL(nIncreaseRow, 24, _T("-")));
                    }
                    else if ( AscbB.dPsi_ast + cDGN_Zero > cMaxRF )
                    {
                        arData.Add(D_CELL(nIncreaseRow, 24, _T("-")));
                    }
                    else
                    {
                        arData.Add(D_CELL(nIncreaseRow, 24, GetOutputFormatWithInfinity(AscbB.dPsi_ast, 3, cMaxRF, FALSE), D_NUMBER_FORMAT_3));
                    }
                }
                arData.Add(D_CELL(nIncreaseRow, 26, (AscbB.dRF < 1.0 ? _T("NG") : _T("OK"))));
            }
            else
            {
                arData.Add(D_CELL(nIncreaseRow, 5, _T("-")));
                arData.Add(D_CELL(nIncreaseRow, 8, _T("-")));
                arData.Add(D_CELL(nIncreaseRow, 11, _T("-")));
                arData.Add(D_CELL(nIncreaseRow, 14, _T("-")));
                arData.Add(D_CELL(nIncreaseRow, 17, _T("-")));
                arData.Add(D_CELL(nIncreaseRow, 20, _T("-")));
                arData.Add(D_CELL(nIncreaseRow, 23, _T("-")));
                arData.Add(D_CELL(nIncreaseRow, 26, _T("-")));
                arData.Add(D_CELL(nIncreaseRow, 29, _T("-")));
                arData.Add(D_CELL(nIncreaseRow, 32, _T("-")));
                arData.Add(D_CELL(nIncreaseRow, 35, _T("-")));
                arData.Add(D_CELL(nIncreaseRow, 37, _T("-")));
                arData.Add(D_CELL(nIncreaseRow, 39, _T("-")));
            }

            nIncreaseRow++;
        }
    }
    if ( nIncreaseRow==0 )
    {
        arData.Add(D_CELL(nIncreaseRow, 1, _T("-")));
        arData.Add(D_CELL(nIncreaseRow, 5, _T("-")));
        arData.Add(D_CELL(nIncreaseRow, 8, _T("-")));
        arData.Add(D_CELL(nIncreaseRow, 11, _T("-")));
        arData.Add(D_CELL(nIncreaseRow, 14, _T("-")));
        arData.Add(D_CELL(nIncreaseRow, 17, _T("-")));
        arData.Add(D_CELL(nIncreaseRow, 20, _T("-")));
        arData.Add(D_CELL(nIncreaseRow, 23, _T("-")));
        arData.Add(D_CELL(nIncreaseRow, 26, _T("-")));
        arData.Add(D_CELL(nIncreaseRow, 29, _T("-")));
        arData.Add(D_CELL(nIncreaseRow, 32, _T("-")));
        arData.Add(D_CELL(nIncreaseRow, 35, _T("-")));
        arData.Add(D_CELL(nIncreaseRow, 37, _T("-")));
        arData.Add(D_CELL(nIncreaseRow, 39, _T("-")));

        nIncreaseRow++;
    }

    // Write Data
    CString RptBlock;
    int rowCnt = 0;

    // Table Title
    RptBlock = _T("Rating_Title_Combined_M_V");
    rowCnt = CopyRange(pExcel, RptBlock, stRow);
    stRow += rowCnt;
    // Table Head
    RptBlock = _T("Rating_Combined_Table_Head");
    rowCnt = CopyRange(pExcel, RptBlock, stRow);
    stRow += rowCnt;

    for ( int i=0; i<nIncreaseRow; ++i )
    {
        if ( i==(nIncreaseRow-1) )
        {
            RptBlock = _T("Rating_Combined_Table_End");
        }
        else
        {
            RptBlock = _T("Rating_Combined_Table_Body");
        }
        rowCnt = CopyRange(pExcel, RptBlock, stRow+i);
    }

    Print_Array_D_CELL(pExcel, arData, stRow);
    stRow += nIncreaseRow;
}

void CRating_CS454_STL_XLOut::GenerateAssTorsionTable(IExcel* pExcel, const int& iPosi, const _T_CS454_STL_XLOUT& XloutD, int& stRow)
{

}

void CRating_CS454_STL_XLOut::GenerateAssSLSTable(IExcel* pExcel, const int& iPosi, const _T_CS454_STL_XLOUT& XloutD, int& stRow)
{
    CArray<T_ALCS_K, T_ALCS_K> arAlcs; arAlcs.RemoveAll();
    m_pDoc->m_pAttrCtrl2->GetAlcsKeyList(arAlcs);
    int nAlcs = arAlcs.GetSize();
    if ( nAlcs==0 ) return;

    int nConcurrentSize = GetConcurrentSize();

    int nIncreaseRow = 0;
    // Make Data
    CArray<D_CELL, D_CELL &> arData;
    arData.RemoveAll();

    BOOL bExistLS = FALSE;
    for ( int i=0; i<nAlcs; ++i )
    {
        T_ALCS_K AlcsK = arAlcs[i];
        BOOL bChk = FALSE;
        XloutD.mSLS.Lookup(AlcsK, bChk);
        if ( !bChk ) continue;

        T_ALCS_D AlcsD;
        if ( !m_pDoc->m_pAttrCtrl2->GetAlcs(AlcsK, AlcsD) ) ASSERT(0); // Rating case

        for ( int j=0; j<nConcurrentSize; ++j ) // concurrent
        {
            int iConCurrent = j+1;
            T_ASSL_CSG_BS_CASE AsslCaseD;
            if ( !m_pDoc->m_pPostCtrl->GetAnalysisResult()->GetSLSRating4CS454CSG(XloutD.GetElemPairK(), AlcsK, 0, iConCurrent, AsslCaseD) ) continue;

            const T_ASSL_CSG_BS_BASE& AsslB =  AsslCaseD.AsslB[iPosi];
            int nPosiNega = (AsslB.ComD.ForD.ChkFor.ForLC.dForce[4] >= 0.0) ? 0 : 1;
            bExistLS = AsslB.ComD.bTopFlangeWithLS || AsslB.ComD.bBottomFlangeWithLS || AsslB.ComD.bWebWithLS;
            // Set Data
            arData.Add(D_CELL(nIncreaseRow, 1, m_pDoc->m_pPostCtrl->GetRatingCaseNameBD21(AlcsK, iConCurrent)));
            if ( AsslB.ComD.bChk )
            {
                double dSig_top = 0.0;
                double dSig_bot = 0.0;
                double dSig_alw_top = 0.0;
                double dSig_alw_bot = 0.0;
                if ( AsslB.bTopComp )
                {
                    dSig_top = AsslB.dsig_c;
                    dSig_bot = AsslB.dsig_t;
                    dSig_alw_top = AsslB.dsig_c_lim;
                    dSig_alw_bot = AsslB.dsig_t_lim;
                }
                else
                {
                    dSig_top = AsslB.dsig_t;
                    dSig_bot = AsslB.dsig_c;
                    dSig_alw_top = AsslB.dsig_t_lim;
                    dSig_alw_bot = AsslB.dsig_c_lim;
                }
                arData.Add(D_CELL(nIncreaseRow, 5, GetOutputFormat(AsslB.dsig_top[0]*m_dFactorStress, 3), D_NUMBER_FORMAT_3));
                arData.Add(D_CELL(nIncreaseRow, 8, GetOutputFormat(AsslB.dsig_top[1]*m_dFactorStress, 3), D_NUMBER_FORMAT_3));
                arData.Add(D_CELL(nIncreaseRow, 11, GetOutputFormat(dSig_alw_top  *m_dFactorStress, 3), D_NUMBER_FORMAT_3));
                arData.Add(D_CELL(nIncreaseRow, 14, GetOutputFormat(AsslB.dsig_bot[0]*m_dFactorStress, 3), D_NUMBER_FORMAT_3));
                arData.Add(D_CELL(nIncreaseRow, 17, GetOutputFormat(AsslB.dsig_bot[1]*m_dFactorStress, 3), D_NUMBER_FORMAT_3));
                arData.Add(D_CELL(nIncreaseRow, 20, GetOutputFormat(dSig_alw_bot *m_dFactorStress, 3), D_NUMBER_FORMAT_3));

                if ( nPosiNega == 1 ) // Negative
                {
                    arData.Add(D_CELL(nIncreaseRow, 23, GetOutputFormat(AsslB.dfr *m_dFactorStress, 3), D_NUMBER_FORMAT_3));
                    arData.Add(D_CELL(nIncreaseRow, 26, GetOutputFormat(AsslB.dfr_lim *m_dFactorStress, 3), D_NUMBER_FORMAT_3));
                }
                else
                {
                    arData.Add(D_CELL(nIncreaseRow, 23, _T("-")));
                    arData.Add(D_CELL(nIncreaseRow, 26, _T("-")));
                }

                if ( nPosiNega == 0 ) // Positive
                {
                    arData.Add(D_CELL(nIncreaseRow, 29, GetOutputFormat(AsslB.dft_s *m_dFactorStress, 3), D_NUMBER_FORMAT_3));
                    arData.Add(D_CELL(nIncreaseRow, 32, GetOutputFormat(AsslB.dft_s_lim *m_dFactorStress, 3), D_NUMBER_FORMAT_3));
                }
                else
                {
                    arData.Add(D_CELL(nIncreaseRow, 29, _T("-")));
                    arData.Add(D_CELL(nIncreaseRow, 32, _T("-")));
                }

                arData.Add(D_CELL(nIncreaseRow, 35, GetOutputFormatWithInfinity(AsslB.dA, 3, cMaxRF, FALSE), D_NUMBER_FORMAT_3));

            }
            else
            {
                arData.Add(D_CELL(nIncreaseRow, 5, _T("-")));
                arData.Add(D_CELL(nIncreaseRow, 8, _T("-")));
                arData.Add(D_CELL(nIncreaseRow, 11, _T("-")));
                arData.Add(D_CELL(nIncreaseRow, 14, _T("-")));
                arData.Add(D_CELL(nIncreaseRow, 17, _T("-")));
                arData.Add(D_CELL(nIncreaseRow, 20, _T("-")));
                arData.Add(D_CELL(nIncreaseRow, 23, _T("-")));
                arData.Add(D_CELL(nIncreaseRow, 26, _T("-")));
                arData.Add(D_CELL(nIncreaseRow, 29, _T("-")));
                arData.Add(D_CELL(nIncreaseRow, 32, _T("-")));
                arData.Add(D_CELL(nIncreaseRow, 35, _T("-")));
            }

            nIncreaseRow++;
        }
    }
    if ( nIncreaseRow==0 )
    {
        arData.Add(D_CELL(nIncreaseRow, 1, _T("-")));
        arData.Add(D_CELL(nIncreaseRow, 5, _T("-")));
        arData.Add(D_CELL(nIncreaseRow, 8, _T("-")));
        arData.Add(D_CELL(nIncreaseRow, 11, _T("-")));
        arData.Add(D_CELL(nIncreaseRow, 14, _T("-")));
        arData.Add(D_CELL(nIncreaseRow, 17, _T("-")));
        arData.Add(D_CELL(nIncreaseRow, 20, _T("-")));
        arData.Add(D_CELL(nIncreaseRow, 23, _T("-")));
        arData.Add(D_CELL(nIncreaseRow, 26, _T("-")));
        arData.Add(D_CELL(nIncreaseRow, 29, _T("-")));
        arData.Add(D_CELL(nIncreaseRow, 32, _T("-")));
        arData.Add(D_CELL(nIncreaseRow, 35, _T("-")));

        nIncreaseRow++;
    }

    // Write Data
    CString RptBlock;
    int rowCnt = 0;

    // Table Title
    RptBlock = bExistLS ? _T("Rating_Title_SLS_Stress") : _T("Rating_Title_SLS_Stress");
    rowCnt = CopyRange(pExcel, RptBlock, stRow);
    stRow += rowCnt;
    // Table Head
    RptBlock = _T("Rating_SLS_Stress_Table_Head");
    rowCnt = CopyRange(pExcel, RptBlock, stRow);
    stRow += rowCnt;

    for ( int i=0; i<nIncreaseRow; ++i )
    {
        if ( i==(nIncreaseRow-1) )
        {
            RptBlock = _T("Rating_SLS_Stress_Table_End");
        }
        else
        {
            RptBlock = _T("Rating_SLS_Stress_Table_Body");
        }
        rowCnt = CopyRange(pExcel, RptBlock, stRow+i);
    }

    Print_Array_D_CELL(pExcel, arData, stRow);
    stRow += nIncreaseRow;

}

void CRating_CS454_STL_XLOut::GenerateAssLongitudinalShearTable(IExcel* pExcel, const bool& bULS, const int& iPosi, const _T_CS454_STL_XLOUT& XloutD, int& stRow)
{
    CArray<T_ALCS_K, T_ALCS_K> arAlcs; arAlcs.RemoveAll();
    m_pDoc->m_pAttrCtrl2->GetAlcsKeyList(arAlcs);
    int nAlcs = arAlcs.GetSize();
    if ( nAlcs==0 ) return;

    int nConcurrentSize = GetConcurrentSize();

    ElemPairK EPairK = XloutD.GetElemPairK();

    int nIncreaseRow = 0;
    // Make Data
    CArray<D_CELL, D_CELL &> arData;
    arData.RemoveAll();

    BOOL bExistLS = FALSE;
    for ( int i=0; i<nAlcs; ++i )
    {
        T_ALCS_K AlcsK = arAlcs[i];
        //         BOOL bChk = FALSE;
        //         if ( bULS )
        //         {
        //             XloutD.mLShearULS.Lookup(AlcsK, bChk);
        //         }
        //         else
        //         {
        //             XloutD.mLShearSLS.Lookup(AlcsK, bChk);
        //         }
        //         
        //         if ( !bChk ) continue;

        T_ALCS_D AlcsD;
        if ( !m_pDoc->m_pAttrCtrl2->GetAlcs(AlcsK, AlcsD) ) ASSERT(0); // Rating case
        CString strState = (AlcsD.nLimitState==0) ? _T("ULS") : _T("SLS");

        for ( int j=0; j<nConcurrentSize; ++j ) // concurrent
        {
            int iConCurrent = j+1;
            T_ASLS_CSG_BS_CASE AslsCaseD;
            if ( AlcsD.nLimitState==0 )
            {
                if ( !m_pDoc->m_pPostCtrl->GetAnalysisResult()->GetLongShear4CS454_CSGResult(EPairK, AlcsK, iConCurrent, AslsCaseD) ) continue;
            }
            else
            {
                if ( !m_pDoc->m_pPostCtrl->GetAnalysisResult()->GetLongShearSLS4CS454_CSGResult(EPairK, AlcsK, iConCurrent, AslsCaseD) ) continue;
            }


            const T_ASLS_CSG_BS_BASE& AslsB =  AslsCaseD.AslsB[iPosi];
            bExistLS = AslsB.ComD.bTopFlangeWithLS || AslsB.ComD.bBottomFlangeWithLS || AslsB.ComD.bWebWithLS;
            // Set Data
            arData.Add(D_CELL(nIncreaseRow, 1, m_pDoc->m_pPostCtrl->GetRatingCaseNameBD21(AlcsK, iConCurrent)));
            if ( AslsB.ComD.bChk )
            {
                arData.Add(D_CELL(nIncreaseRow, 5, strState));
                arData.Add(D_CELL(nIncreaseRow, 8, GetOutputFormat(AslsB.dq *m_dFactorForce, 3), D_NUMBER_FORMAT_3));
                arData.Add(D_CELL(nIncreaseRow, 11, GetOutputFormat(AslsB.dPim  *m_dFactorForce, 3), D_NUMBER_FORMAT_3));
                arData.Add(D_CELL(nIncreaseRow, 14, GetOutputFormat(AslsB.dPa *m_dFactorForce, 3), D_NUMBER_FORMAT_3));
                arData.Add(D_CELL(nIncreaseRow, 17, GetOutputFormat(AslsB.dqr *m_dFactorForce, 3), D_NUMBER_FORMAT_3));
                arData.Add(D_CELL(nIncreaseRow, 20, GetOutputFormat(AslsB.dq_qr, 3), D_NUMBER_FORMAT_3));
                arData.Add(D_CELL(nIncreaseRow, 23, GetOutputFormat(AslsB.dq_qr_lim, 3), D_NUMBER_FORMAT_3));

                arData.Add(D_CELL(nIncreaseRow, 26, (AslsB.nOK == 1.0 ? _T("OK") : _T("NG"))));
            }
            else
            {
                arData.Add(D_CELL(nIncreaseRow, 5, _T("-")));
                arData.Add(D_CELL(nIncreaseRow, 8, _T("-")));
                arData.Add(D_CELL(nIncreaseRow, 11, _T("-")));
                arData.Add(D_CELL(nIncreaseRow, 14, _T("-")));
                arData.Add(D_CELL(nIncreaseRow, 17, _T("-")));
                arData.Add(D_CELL(nIncreaseRow, 20, _T("-")));
                arData.Add(D_CELL(nIncreaseRow, 23, _T("-")));
                arData.Add(D_CELL(nIncreaseRow, 26, _T("-")));
            }

            nIncreaseRow++;
        }
    }
    if ( nIncreaseRow==0 )
    {
        arData.Add(D_CELL(nIncreaseRow, 1, _T("-")));
        arData.Add(D_CELL(nIncreaseRow, 5, _T("-")));
        arData.Add(D_CELL(nIncreaseRow, 8, _T("-")));
        arData.Add(D_CELL(nIncreaseRow, 11, _T("-")));
        arData.Add(D_CELL(nIncreaseRow, 14, _T("-")));
        arData.Add(D_CELL(nIncreaseRow, 17, _T("-")));
        arData.Add(D_CELL(nIncreaseRow, 20, _T("-")));
        arData.Add(D_CELL(nIncreaseRow, 23, _T("-")));

        nIncreaseRow++;
    }

    // Write Data
    CString RptBlock;
    int rowCnt = 0;

    // Table Title
    RptBlock = bExistLS ? _T("Rating_Title_Long_Shear_with_stiffener") : _T("Rating_Title_Long_Shear");
    rowCnt = CopyRange(pExcel, RptBlock, stRow);
    stRow += rowCnt;
    // Table Head
    RptBlock = _T("Rating_Long_Shear_Table_Head");
    rowCnt = CopyRange(pExcel, RptBlock, stRow);
    stRow += rowCnt;

    for ( int i=0; i<nIncreaseRow; ++i )
    {
        if ( i==(nIncreaseRow-1) )
        {
            RptBlock = _T("Rating_Long_Shear_Table_End");
        }
        else
        {
            RptBlock = _T("Rating_Long_Shear_Table_Body");
        }
        rowCnt = CopyRange(pExcel, RptBlock, stRow+i);
    }

    Print_Array_D_CELL(pExcel, arData, stRow);
    stRow += nIncreaseRow;

}

void CRating_CS454_STL_XLOut::GenerateAssBoxFlangeTable(IExcel* pExcel, const int& iPosi, const _T_CS454_STL_XLOUT& XloutD, int& stRow)
{
    CArray<T_ALCS_K, T_ALCS_K> arAlcs; arAlcs.RemoveAll();
    m_pDoc->m_pAttrCtrl2->GetAlcsKeyList(arAlcs);
    int nAlcs = arAlcs.GetSize();
    if ( nAlcs==0 ) return;

    int nConcurrentSize = GetConcurrentSize();

    ElemPairK EPairK = XloutD.GetElemPairK();

    int nIncreaseRow = 0;
    // Make Data
    CArray<D_CELL, D_CELL &> arData;
    arData.RemoveAll();
    for ( int i=0; i<nAlcs; ++i )
    {
        T_ALCS_K AlcsK = arAlcs[i];
        BOOL bChk = FALSE;
        XloutD.mFlangeBox.Lookup(AlcsK, bChk);
        if ( !bChk ) continue;

        T_ALCS_D AlcsD;
        if ( !m_pDoc->m_pAttrCtrl2->GetAlcs(AlcsK, AlcsD) ) ASSERT(0); // Rating case

        for ( int j=0; j<nConcurrentSize; ++j ) // concurrent
        {
            int iConCurrent = j+1;
            T_ASFG_CSG_BS_D AsfgD;
            if ( !m_pDoc->m_pPostCtrl->GetAnalysisResult()->GetFlangeWithLStiffenerBoxShape4CS454_CSGResult(EPairK, AlcsK, 0, iConCurrent, AsfgD) ) continue;

            for ( int k=0; k<2; ++k )
            {
                int nIdx = iPosi*2 + k;
                const T_ASFG_CSG_BS_BASE& AsfgB =  AsfgD.AsfgB[nIdx]; // 압축?

                if ( !AsfgB.ComD.IsStiffener() ) continue;

                CString strCompTens = (k == 0) ? _T("Comp.") : _T("Tens.");
                // Set Data
                arData.Add(D_CELL(nIncreaseRow, 1, m_pDoc->m_pPostCtrl->GetRatingCaseNameBD21(AlcsK, iConCurrent)));
                if ( AsfgB.ComD.bChk )
                {
                    arData.Add(D_CELL(nIncreaseRow, 5, strCompTens));
                    arData.Add(D_CELL(nIncreaseRow, 8, GetOutputFormat(AsfgB.dSig_yf *m_dFactorStress, 3), D_NUMBER_FORMAT_3));
                    arData.Add(D_CELL(nIncreaseRow, 11, GetOutputFormat(AsfgB.dSig_f_SV  *m_dFactorStress, 3), D_NUMBER_FORMAT_3));
                    arData.Add(D_CELL(nIncreaseRow, 14, GetOutputFormat(AsfgB.dTau_SV *m_dFactorStress, 3), D_NUMBER_FORMAT_3));
                    arData.Add(D_CELL(nIncreaseRow, 17, GetOutputFormat(AsfgB.dSig_f_DL *m_dFactorStress, 3), D_NUMBER_FORMAT_3));
                    arData.Add(D_CELL(nIncreaseRow, 20, GetOutputFormat(AsfgB.dTau_DL *m_dFactorStress, 3), D_NUMBER_FORMAT_3));
                    arData.Add(D_CELL(nIncreaseRow, 23, GetOutputFormat(AsfgB.dSig_f_ST *m_dFactorStress, 3), D_NUMBER_FORMAT_3));
                    arData.Add(D_CELL(nIncreaseRow, 26, GetOutputFormat(AsfgB.dTau_ST *m_dFactorStress, 3), D_NUMBER_FORMAT_3));
                    arData.Add(D_CELL(nIncreaseRow, 29, GetOutputFormatWithInfinity(AsfgB.dA, 3, cMaxRF, FALSE), D_NUMBER_FORMAT_3));

                    if ( AlcsD.nMovType == 1 ) //Special인 경우
                    {
                        arData.Add(D_CELL(nIncreaseRow, 23, _T("-")));
                        arData.Add(D_CELL(nIncreaseRow, 26, _T("-")));
                        arData.Add(D_CELL(nIncreaseRow, 31, _T("-")));
                        if ( abs(AsfgB.dPsi_ast) - cDGN_Zero < 0.0 )
                        {
                            arData.Add(D_CELL(nIncreaseRow, 33, _T("-")));
                        }
                        else if ( AsfgB.dPsi_ast + cDGN_Zero > cMaxRF )
                        {
                            arData.Add(D_CELL(nIncreaseRow, 33, _T("-")));
                        }
                        else
                        {
                            arData.Add(D_CELL(nIncreaseRow, 33, GetOutputFormatWithInfinity(AsfgB.dPsi_ast, 3, cMaxRF, FALSE), D_NUMBER_FORMAT_3));
                        }
                    }
                    else //Combined인 경우
                    {
                        if ( abs(AsfgB.dPsi) - cDGN_Zero < 0.0 )
                        {
                            arData.Add(D_CELL(nIncreaseRow, 31, _T("-")));
                        }
                        else if ( AsfgB.dPsi + cDGN_Zero > cMaxRF )
                        {
                            arData.Add(D_CELL(nIncreaseRow, 31, _T("-")));
                        }
                        else
                        {
                            arData.Add(D_CELL(nIncreaseRow, 31, GetOutputFormatWithInfinity(AsfgB.dPsi, 3, cMaxRF, FALSE), D_NUMBER_FORMAT_3));
                        }

                        if ( abs(AsfgB.dPsi_ast) - cDGN_Zero < 0.0 )
                        {
                            arData.Add(D_CELL(nIncreaseRow, 33, _T("-")));
                        }
                        else if ( AsfgB.dPsi_ast + cDGN_Zero > cMaxRF )
                        {
                            arData.Add(D_CELL(nIncreaseRow, 33, _T("-")));
                        }
                        else
                        {
                            arData.Add(D_CELL(nIncreaseRow, 33, GetOutputFormatWithInfinity(AsfgB.dPsi_ast, 3, cMaxRF, FALSE), D_NUMBER_FORMAT_3));
                        }
                    }
                    arData.Add(D_CELL(nIncreaseRow, 35, (AsfgB.dRF < 1.0 ? _T("NG") : _T("OK"))));
                }
                else
                {
                    arData.Add(D_CELL(nIncreaseRow, 5, _T("-")));
                    arData.Add(D_CELL(nIncreaseRow, 8, _T("-")));
                    arData.Add(D_CELL(nIncreaseRow, 11, _T("-")));
                    arData.Add(D_CELL(nIncreaseRow, 14, _T("-")));
                    arData.Add(D_CELL(nIncreaseRow, 17, _T("-")));
                    arData.Add(D_CELL(nIncreaseRow, 20, _T("-")));
                    arData.Add(D_CELL(nIncreaseRow, 23, _T("-")));
                    arData.Add(D_CELL(nIncreaseRow, 26, _T("-")));
                    arData.Add(D_CELL(nIncreaseRow, 29, _T("-")));
                    arData.Add(D_CELL(nIncreaseRow, 31, _T("-")));
                    arData.Add(D_CELL(nIncreaseRow, 33, _T("-")));
                    arData.Add(D_CELL(nIncreaseRow, 35, _T("-")));
                }

                nIncreaseRow++;
            }

        }
    }
    if ( nIncreaseRow==0 )
    {
        arData.Add(D_CELL(nIncreaseRow, 1, _T("-")));
        arData.Add(D_CELL(nIncreaseRow, 5, _T("-")));
        arData.Add(D_CELL(nIncreaseRow, 8, _T("-")));
        arData.Add(D_CELL(nIncreaseRow, 11, _T("-")));
        arData.Add(D_CELL(nIncreaseRow, 14, _T("-")));
        arData.Add(D_CELL(nIncreaseRow, 17, _T("-")));
        arData.Add(D_CELL(nIncreaseRow, 20, _T("-")));
        arData.Add(D_CELL(nIncreaseRow, 23, _T("-")));
        arData.Add(D_CELL(nIncreaseRow, 26, _T("-")));
        arData.Add(D_CELL(nIncreaseRow, 29, _T("-")));
        arData.Add(D_CELL(nIncreaseRow, 31, _T("-")));
        arData.Add(D_CELL(nIncreaseRow, 33, _T("-")));
        arData.Add(D_CELL(nIncreaseRow, 35, _T("-")));

        nIncreaseRow++;
    }

    // Write Data
    CString RptBlock;
    int rowCnt = 0;

    // Table Title
    RptBlock = _T("Rating_Title_Box_Flange_Long_Stiffener");
    rowCnt = CopyRange(pExcel, RptBlock, stRow);
    stRow += rowCnt+1;

    RptBlock = _T("Rating_Title_Flange");
    rowCnt = CopyRange(pExcel, RptBlock, stRow);
    stRow += rowCnt;
    // Table Head
    RptBlock = _T("Rating_Box_Flange_Table_Head");
    rowCnt = CopyRange(pExcel, RptBlock, stRow);
    stRow += rowCnt;

    for ( int i=0; i<nIncreaseRow; ++i )
    {
        if ( i==(nIncreaseRow-1) )
        {
            RptBlock = _T("Rating_Box_Flange_Table_End");
        }
        else
        {
            RptBlock = _T("Rating_Box_Flange_Table_Body");
        }
        rowCnt = CopyRange(pExcel, RptBlock, stRow+i);
    }

    Print_Array_D_CELL(pExcel, arData, stRow);
    stRow += nIncreaseRow;
}

void CRating_CS454_STL_XLOut::GenerateAssBoxFlangeStiffenerTable(IExcel* pExcel, const int& iPosi, const _T_CS454_STL_XLOUT& XloutD, int& stRow)
{
    CArray<T_ALCS_K, T_ALCS_K> arAlcs; arAlcs.RemoveAll();
    m_pDoc->m_pAttrCtrl2->GetAlcsKeyList(arAlcs);
    int nAlcs = arAlcs.GetSize();
    if ( nAlcs==0 ) return;

    int nConcurrentSize = GetConcurrentSize();

    ElemPairK EPairK = XloutD.GetElemPairK();

    int nIncreaseRow = 0;
    // Make Data
    CArray<D_CELL, D_CELL &> arData;
    arData.RemoveAll();
    for ( int i=0; i<nAlcs; ++i )
    {
        T_ALCS_K AlcsK = arAlcs[i];
        BOOL bChk = FALSE;
        XloutD.mFlangeStiff.Lookup(AlcsK, bChk);
        if ( !bChk ) continue;

        T_ALCS_D AlcsD;
        if ( !m_pDoc->m_pAttrCtrl2->GetAlcs(AlcsK, AlcsD) ) ASSERT(0); // Rating case

        for ( int nP=0; nP<2; ++nP )
        {
            for ( int j=0; j<nConcurrentSize; ++j ) // concurrent
            {
                int iConCurrent = j+1;
                T_ASFS_CSG_BS_CASE AsfsCaseD;
                if ( !m_pDoc->m_pPostCtrl->GetAnalysisResult()->GetFlangeStiffenerWithLStiffenerBoxShape4CS454_CSGResult(EPairK, AlcsK, iConCurrent, AsfsCaseD) ) continue;

                const T_ASFS_CSG_BS_BASE& AsfsB =  (nP==0) ? AsfsCaseD.AsfsB[iPosi] : AsfsCaseD.AsfsFlgB[iPosi];

                if ( !AsfsB.ComD.IsStiffener() ) continue;

                CString strPartType = (nP==0) ? _T("Stiffener") : _T("Flange");;
                // Set Data
                arData.Add(D_CELL(nIncreaseRow, 1, m_pDoc->m_pPostCtrl->GetRatingCaseNameBD21(AlcsK, iConCurrent)));
                if ( AsfsB.ComD.bChk )
                {
                    arData.Add(D_CELL(nIncreaseRow, 5, strPartType));
                    arData.Add(D_CELL(nIncreaseRow, 8, GetOutputFormat(AsfsB.dk_l, 3), D_NUMBER_FORMAT_3));
                    arData.Add(D_CELL(nIncreaseRow, 11, GetOutputFormat(AsfsB.dSig_y *m_dFactorStress, 3), D_NUMBER_FORMAT_3));
                    arData.Add(D_CELL(nIncreaseRow, 14, GetOutputFormat(AsfsB.dSig_a_SV *m_dFactorStress, 3), D_NUMBER_FORMAT_3));
                    arData.Add(D_CELL(nIncreaseRow, 17, GetOutputFormat(AsfsB.dTau_1_SV *m_dFactorStress, 3), D_NUMBER_FORMAT_3));
                    arData.Add(D_CELL(nIncreaseRow, 20, GetOutputFormat(AsfsB.dSig_a_DL *m_dFactorStress, 3), D_NUMBER_FORMAT_3));
                    arData.Add(D_CELL(nIncreaseRow, 23, GetOutputFormat(AsfsB.dTau_1_DL *m_dFactorStress, 3), D_NUMBER_FORMAT_3));
                    arData.Add(D_CELL(nIncreaseRow, 26, GetOutputFormat(AsfsB.dSig_a_ST *m_dFactorStress, 3), D_NUMBER_FORMAT_3));
                    arData.Add(D_CELL(nIncreaseRow, 29, GetOutputFormat(AsfsB.dTau_1_ST *m_dFactorStress, 3), D_NUMBER_FORMAT_3));
                    arData.Add(D_CELL(nIncreaseRow, 32, GetOutputFormat(AsfsB.dk_s, 3), D_NUMBER_FORMAT_3));
                    arData.Add(D_CELL(nIncreaseRow, 35, GetOutputFormatWithInfinity(AsfsB.dA, 3, cMaxRF, FALSE), D_NUMBER_FORMAT_3));

                    if ( AlcsD.nMovType == 1 ) //Special인 경우
                    {
                        arData.Add(D_CELL(nIncreaseRow, 26, _T("-")));
                        arData.Add(D_CELL(nIncreaseRow, 29, _T("-")));
                        arData.Add(D_CELL(nIncreaseRow, 37, _T("-")));
                        if ( fabs(AsfsB.dPsi_ast) - cDGN_Zero < 0.0 )
                        {
                            arData.Add(D_CELL(nIncreaseRow, 39, _T("-")));
                        }
                        else if ( AsfsB.dPsi_ast + cDGN_Zero > cMaxRF )
                        {
                            arData.Add(D_CELL(nIncreaseRow, 39, _T("-")));
                        }
                        else
                        {
                            arData.Add(D_CELL(nIncreaseRow, 39, GetOutputFormatWithInfinity(AsfsB.dPsi_ast, 3, cMaxRF, FALSE), D_NUMBER_FORMAT_3));
                        }
                    }
                    else //Combined인 경우
                    {
                        if ( fabs(AsfsB.dPsi) - cDGN_Zero < 0.0 )
                        {
                            arData.Add(D_CELL(nIncreaseRow, 37, _T("-")));
                        }
                        else if ( AsfsB.dPsi + cDGN_Zero > cMaxRF )
                        {
                            arData.Add(D_CELL(nIncreaseRow, 37, _T("-")));
                        }
                        else
                        {
                            arData.Add(D_CELL(nIncreaseRow, 37, GetOutputFormatWithInfinity(AsfsB.dPsi, 3, cMaxRF, FALSE), D_NUMBER_FORMAT_3));
                        }

                        if ( fabs(AsfsB.dPsi_ast) - cDGN_Zero < 0.0 )
                        {
                            arData.Add(D_CELL(nIncreaseRow, 39, _T("-")));
                        }
                        else if ( AsfsB.dPsi_ast + cDGN_Zero > cMaxRF )
                        {
                            arData.Add(D_CELL(nIncreaseRow, 39, _T("-")));
                        }
                        else
                        {
                            arData.Add(D_CELL(nIncreaseRow, 39, GetOutputFormatWithInfinity(AsfsB.dPsi_ast, 3, cMaxRF, FALSE), D_NUMBER_FORMAT_3));
                        }
                    }
                    arData.Add(D_CELL(nIncreaseRow, 41, (AsfsB.dRF < 1.0 ? _T("NG") : _T("OK"))));
                }
                else
                {
                    arData.Add(D_CELL(nIncreaseRow, 5, _T("-")));
                    arData.Add(D_CELL(nIncreaseRow, 8, _T("-")));
                    arData.Add(D_CELL(nIncreaseRow, 11, _T("-")));
                    arData.Add(D_CELL(nIncreaseRow, 14, _T("-")));
                    arData.Add(D_CELL(nIncreaseRow, 17, _T("-")));
                    arData.Add(D_CELL(nIncreaseRow, 20, _T("-")));
                    arData.Add(D_CELL(nIncreaseRow, 23, _T("-")));
                    arData.Add(D_CELL(nIncreaseRow, 26, _T("-")));
                    arData.Add(D_CELL(nIncreaseRow, 29, _T("-")));
                    arData.Add(D_CELL(nIncreaseRow, 32, _T("-")));
                    arData.Add(D_CELL(nIncreaseRow, 34, _T("-")));
                    arData.Add(D_CELL(nIncreaseRow, 36, _T("-")));
                    arData.Add(D_CELL(nIncreaseRow, 38, _T("-")));
                    arData.Add(D_CELL(nIncreaseRow, 41, _T("-")));
                }

                nIncreaseRow++;
            }

        }
    }
    if ( nIncreaseRow==0 )
    {
        arData.Add(D_CELL(nIncreaseRow, 1, _T("-")));
        arData.Add(D_CELL(nIncreaseRow, 5, _T("-")));
        arData.Add(D_CELL(nIncreaseRow, 8, _T("-")));
        arData.Add(D_CELL(nIncreaseRow, 11, _T("-")));
        arData.Add(D_CELL(nIncreaseRow, 14, _T("-")));
        arData.Add(D_CELL(nIncreaseRow, 17, _T("-")));
        arData.Add(D_CELL(nIncreaseRow, 20, _T("-")));
        arData.Add(D_CELL(nIncreaseRow, 23, _T("-")));
        arData.Add(D_CELL(nIncreaseRow, 26, _T("-")));
        arData.Add(D_CELL(nIncreaseRow, 29, _T("-")));
        arData.Add(D_CELL(nIncreaseRow, 32, _T("-")));
        arData.Add(D_CELL(nIncreaseRow, 34, _T("-")));
        arData.Add(D_CELL(nIncreaseRow, 36, _T("-")));
        arData.Add(D_CELL(nIncreaseRow, 38, _T("-")));
        arData.Add(D_CELL(nIncreaseRow, 41, _T("-")));

        nIncreaseRow++;
    }

    // Write Data
    CString RptBlock;
    int rowCnt = 0;

    // Table Title
    RptBlock = _T("Rating_Title_Stiffener");
    rowCnt = CopyRange(pExcel, RptBlock, stRow);
    stRow += rowCnt;
    // Table Head
    RptBlock = _T("Rating_Box_Flange_Stiffener_Table_Head");
    rowCnt = CopyRange(pExcel, RptBlock, stRow);
    stRow += rowCnt;

    for ( int i=0; i<nIncreaseRow; ++i )
    {
        if ( i==(nIncreaseRow-1) )
        {
            RptBlock = _T("Rating_Box_Flange_Stiffener_Table_End");
        }
        else
        {
            RptBlock = _T("Rating_Box_Flange_Stiffener_Table_Body");
        }
        rowCnt = CopyRange(pExcel, RptBlock, stRow+i);
    }

    Print_Array_D_CELL(pExcel, arData, stRow);
    stRow += nIncreaseRow;
}

void CRating_CS454_STL_XLOut::GenerateAssWebYieldingTable(IExcel* pExcel, const int& iPosi, const _T_CS454_STL_XLOUT& XloutD, int& stRow)
{
    CArray<T_ALCS_K, T_ALCS_K> arAlcs; arAlcs.RemoveAll();
    m_pDoc->m_pAttrCtrl2->GetAlcsKeyList(arAlcs);
    int nAlcs = arAlcs.GetSize();
    if ( nAlcs==0 ) return;

    ElemPairK EPairK = XloutD.GetElemPairK();

    int nConcurrentSize = GetConcurrentSize();

    int nIncreaseRow = 0;
    // Make Data
    CArray<D_CELL, D_CELL &> arData;
    arData.RemoveAll();
    for ( int i=0; i<nAlcs; ++i )
    {
        T_ALCS_K AlcsK = arAlcs[i];
        BOOL bChk = FALSE;
        XloutD.mWebYielding.Lookup(AlcsK, bChk);
        if ( !bChk ) continue;

        T_ALCS_D AlcsD;
        if ( !m_pDoc->m_pAttrCtrl2->GetAlcs(AlcsK, AlcsD) ) ASSERT(0); // Rating case

        for ( int j=0; j<nConcurrentSize; ++j ) // concurrent
        {
            int iConCurrent = j+1;
            T_ASWY_CSG_BS_D AswyCaseD;
            if ( !m_pDoc->m_pPostCtrl->GetAnalysisResult()->GetWebYieldingWithLStiffenerBoxShape4CS454_CSGResult(EPairK, AlcsK, 0, iConCurrent, AswyCaseD) ) continue;

            const T_ASWY_CSG_BS_BASE& AswyB =  AswyCaseD.AswyB[iPosi];
            if ( !AswyB.ComD.bWebWithLS ) continue;

            // Set Data
            arData.Add(D_CELL(nIncreaseRow, 1, m_pDoc->m_pPostCtrl->GetRatingCaseNameBD21(AlcsK, iConCurrent)));
            if ( AswyB.ComD.bChk )
            {
                arData.Add(D_CELL(nIncreaseRow, 5, GetOutputFormat(AswyB.dSig_yw *m_dFactorStress, 3), D_NUMBER_FORMAT_3));
                arData.Add(D_CELL(nIncreaseRow, 8, GetOutputFormat(AswyB.dSig_1e_SV *m_dFactorStress, 3), D_NUMBER_FORMAT_3));
                arData.Add(D_CELL(nIncreaseRow, 11, GetOutputFormat(AswyB.dTau_SV *m_dFactorStress, 3), D_NUMBER_FORMAT_3));
                arData.Add(D_CELL(nIncreaseRow, 14, GetOutputFormat(AswyB.dSig_1e_DL *m_dFactorStress, 3), D_NUMBER_FORMAT_3));
                arData.Add(D_CELL(nIncreaseRow, 17, GetOutputFormat(AswyB.dTau_DL *m_dFactorStress, 3), D_NUMBER_FORMAT_3));
                arData.Add(D_CELL(nIncreaseRow, 20, GetOutputFormat(AswyB.dSig_1e_ST *m_dFactorStress, 3), D_NUMBER_FORMAT_3));
                arData.Add(D_CELL(nIncreaseRow, 23, GetOutputFormat(AswyB.dTau_ST *m_dFactorStress, 3), D_NUMBER_FORMAT_3));
                arData.Add(D_CELL(nIncreaseRow, 26, GetOutputFormatWithInfinity(AswyB.dA, 3, cMaxRF, FALSE), D_NUMBER_FORMAT_3));

                if ( AlcsD.nMovType == 1 ) //Special인 경우
                {
                    arData.Add(D_CELL(nIncreaseRow, 20, _T("-")));
                    arData.Add(D_CELL(nIncreaseRow, 23, _T("-")));
                    arData.Add(D_CELL(nIncreaseRow, 28, _T("-")));
                    if ( fabs(AswyB.dPsi_ast) - cDGN_Zero < 0.0 )
                    {
                        arData.Add(D_CELL(nIncreaseRow, 30, _T("-")));
                    }
                    else if ( AswyB.dPsi_ast + cDGN_Zero > cMaxRF )
                    {
                        arData.Add(D_CELL(nIncreaseRow, 30, _T("-")));
                    }
                    else
                    {
                        arData.Add(D_CELL(nIncreaseRow, 30, GetOutputFormatWithInfinity(AswyB.dPsi_ast, 3, cMaxRF, FALSE), D_NUMBER_FORMAT_3));
                    }
                }
                else //Combined인 경우
                {
                    if ( fabs(AswyB.dPsi) - cDGN_Zero < 0.0 )
                    {
                        arData.Add(D_CELL(nIncreaseRow, 28, _T("-")));
                    }
                    else if ( AswyB.dPsi + cDGN_Zero > cMaxRF )
                    {
                        arData.Add(D_CELL(nIncreaseRow, 28, _T("-")));
                    }
                    else
                    {
                        arData.Add(D_CELL(nIncreaseRow, 28, GetOutputFormatWithInfinity(AswyB.dPsi, 3, cMaxRF, FALSE), D_NUMBER_FORMAT_3));
                    }

                    if ( fabs(AswyB.dPsi_ast) - cDGN_Zero < 0.0 )
                    {
                        arData.Add(D_CELL(nIncreaseRow, 30, _T("-")));
                    }
                    else if ( AswyB.dPsi_ast + cDGN_Zero > cMaxRF )
                    {
                        arData.Add(D_CELL(nIncreaseRow, 30, _T("-")));
                    }
                    else
                    {
                        arData.Add(D_CELL(nIncreaseRow, 30, GetOutputFormatWithInfinity(AswyB.dPsi_ast, 3, cMaxRF, FALSE), D_NUMBER_FORMAT_3));
                    }
                }
                arData.Add(D_CELL(nIncreaseRow, 32, (AswyB.dRF < 1.0 ? _T("NG") : _T("OK"))));
            }
            else
            {
                arData.Add(D_CELL(nIncreaseRow, 5, _T("-")));
                arData.Add(D_CELL(nIncreaseRow, 8, _T("-")));
                arData.Add(D_CELL(nIncreaseRow, 11, _T("-")));
                arData.Add(D_CELL(nIncreaseRow, 14, _T("-")));
                arData.Add(D_CELL(nIncreaseRow, 17, _T("-")));
                arData.Add(D_CELL(nIncreaseRow, 20, _T("-")));
                arData.Add(D_CELL(nIncreaseRow, 23, _T("-")));
                arData.Add(D_CELL(nIncreaseRow, 26, _T("-")));
                arData.Add(D_CELL(nIncreaseRow, 28, _T("-")));
                arData.Add(D_CELL(nIncreaseRow, 30, _T("-")));
                arData.Add(D_CELL(nIncreaseRow, 32, _T("-")));
            }

            nIncreaseRow++;
        }
    }
    if ( nIncreaseRow==0 )
    {
        arData.Add(D_CELL(nIncreaseRow, 1, _T("-")));
        arData.Add(D_CELL(nIncreaseRow, 5, _T("-")));
        arData.Add(D_CELL(nIncreaseRow, 8, _T("-")));
        arData.Add(D_CELL(nIncreaseRow, 11, _T("-")));
        arData.Add(D_CELL(nIncreaseRow, 14, _T("-")));
        arData.Add(D_CELL(nIncreaseRow, 17, _T("-")));
        arData.Add(D_CELL(nIncreaseRow, 20, _T("-")));
        arData.Add(D_CELL(nIncreaseRow, 23, _T("-")));
        arData.Add(D_CELL(nIncreaseRow, 26, _T("-")));
        arData.Add(D_CELL(nIncreaseRow, 28, _T("-")));
        arData.Add(D_CELL(nIncreaseRow, 30, _T("-")));
        arData.Add(D_CELL(nIncreaseRow, 32, _T("-")));

        nIncreaseRow++;
    }

    // Write Data
    CString RptBlock;
    int rowCnt = 0;

    // Table Title
    RptBlock = _T("Rating_Title_Box_Web_Long_Stiffener");
    rowCnt = CopyRange(pExcel, RptBlock, stRow);
    stRow += rowCnt+1;

    RptBlock = _T("Rating_Title_Web_Yielding");
    rowCnt = CopyRange(pExcel, RptBlock, stRow);
    stRow += rowCnt;
    // Table Head
    RptBlock = _T("Rating_Box_Web_Yielding_Table_Head");
    rowCnt = CopyRange(pExcel, RptBlock, stRow);
    stRow += rowCnt;

    for ( int i=0; i<nIncreaseRow; ++i )
    {
        if ( i==(nIncreaseRow-1) )
        {
            RptBlock = _T("Rating_Box_Web_Yielding_Table_End");
        }
        else
        {
            RptBlock = _T("Rating_Box_Web_Yielding_Table_Body");
        }
        rowCnt = CopyRange(pExcel, RptBlock, stRow+i);
    }

    Print_Array_D_CELL(pExcel, arData, stRow);
    stRow += nIncreaseRow;
}

void CRating_CS454_STL_XLOut::GenerateAssWebBucklingTable(IExcel* pExcel, const int& iPosi, const _T_CS454_STL_XLOUT& XloutD, int& stRow)
{
    CArray<T_ALCS_K, T_ALCS_K> arAlcs; arAlcs.RemoveAll();
    m_pDoc->m_pAttrCtrl2->GetAlcsKeyList(arAlcs);
    int nAlcs = arAlcs.GetSize();
    if ( nAlcs==0 ) return;

    ElemPairK EPairK = XloutD.GetElemPairK();

    int nConcurrentSize = GetConcurrentSize();

    int nIncreaseRow = 0;
    // Make Data
    CArray<D_CELL, D_CELL &> arData;
    arData.RemoveAll();
    for ( int i=0; i<nAlcs; ++i )
    {
        T_ALCS_K AlcsK = arAlcs[i];
        BOOL bChk = FALSE;
        XloutD.mWebBuckling.Lookup(AlcsK, bChk);
        if ( !bChk ) continue;

        T_ALCS_D AlcsD;
        if ( !m_pDoc->m_pAttrCtrl2->GetAlcs(AlcsK, AlcsD) ) ASSERT(0); // Rating case

        for ( int j=0; j<nConcurrentSize; ++j ) // concurrent
        {
            int iConCurrent = j+1;
            T_ASWB_CSG_BS_D AswbCaseD;
            if ( !m_pDoc->m_pPostCtrl->GetAnalysisResult()->GetWebBucklingWithLStiffenerBoxShape4CS454_CSGResult(EPairK, AlcsK, 0, iConCurrent, AswbCaseD) ) continue;

            const T_ASWB_CSG_BS_BASE& AswbB =  AswbCaseD.AswbB[iPosi];

            if ( !AswbB.ComD.bWebWithLS ) continue;

            // Set Data
            arData.Add(D_CELL(nIncreaseRow, 1, m_pDoc->m_pPostCtrl->GetRatingCaseNameBD21(AlcsK, iConCurrent)));
            if ( AswbB.ComD.bChk )
            {
                arData.Add(D_CELL(nIncreaseRow, 5, GetOutputFormat(AswbB.dSig_yw *m_dFactorStress, 3), D_NUMBER_FORMAT_3));
                arData.Add(D_CELL(nIncreaseRow, 8, GetOutputFormat(AswbB.dK1 , 3), D_NUMBER_FORMAT_3));
                arData.Add(D_CELL(nIncreaseRow, 11, GetOutputFormat(AswbB.dKb, 3), D_NUMBER_FORMAT_3));
                arData.Add(D_CELL(nIncreaseRow, 14, GetOutputFormat(AswbB.dKq, 3), D_NUMBER_FORMAT_3));
                arData.Add(D_CELL(nIncreaseRow, 17, GetOutputFormat(AswbB.dSig_1_SV *m_dFactorStress, 3), D_NUMBER_FORMAT_3));
                arData.Add(D_CELL(nIncreaseRow, 20, GetOutputFormat(AswbB.dSig_b_SV *m_dFactorStress, 3), D_NUMBER_FORMAT_3));
                arData.Add(D_CELL(nIncreaseRow, 23, GetOutputFormat(AswbB.dTau_SV *m_dFactorStress, 3), D_NUMBER_FORMAT_3));
                arData.Add(D_CELL(nIncreaseRow, 26, GetOutputFormat(AswbB.dSig_1_DL *m_dFactorStress, 3), D_NUMBER_FORMAT_3));
                arData.Add(D_CELL(nIncreaseRow, 29, GetOutputFormat(AswbB.dSig_b_DL *m_dFactorStress, 3), D_NUMBER_FORMAT_3));
                arData.Add(D_CELL(nIncreaseRow, 32, GetOutputFormat(AswbB.dTau_DL *m_dFactorStress, 3), D_NUMBER_FORMAT_3));
                arData.Add(D_CELL(nIncreaseRow, 35, GetOutputFormat(AswbB.dSig_1_ST *m_dFactorStress, 3), D_NUMBER_FORMAT_3));
                arData.Add(D_CELL(nIncreaseRow, 38, GetOutputFormat(AswbB.dSig_b_ST *m_dFactorStress, 3), D_NUMBER_FORMAT_3));
                arData.Add(D_CELL(nIncreaseRow, 41, GetOutputFormat(AswbB.dTau_ST *m_dFactorStress, 3), D_NUMBER_FORMAT_3));                
                arData.Add(D_CELL(nIncreaseRow, 44, GetOutputFormatWithInfinity(AswbB.dA, 3, cMaxRF, FALSE), D_NUMBER_FORMAT_3));

                if ( AlcsD.nMovType == 1 ) //Special인 경우
                {
                    arData.Add(D_CELL(nIncreaseRow, 35, _T("-")));
                    arData.Add(D_CELL(nIncreaseRow, 38, _T("-")));
                    arData.Add(D_CELL(nIncreaseRow, 41, _T("-")));
                    arData.Add(D_CELL(nIncreaseRow, 46, _T("-")));
                    if ( fabs(AswbB.dPsi_ast) - cDGN_Zero < 0.0 )
                    {
                        arData.Add(D_CELL(nIncreaseRow, 48, _T("-")));
                    }
                    else if ( AswbB.dPsi_ast + cDGN_Zero > cMaxRF )
                    {
                        arData.Add(D_CELL(nIncreaseRow, 48, _T("-")));
                    }
                    else
                    {
                        arData.Add(D_CELL(nIncreaseRow, 48, GetOutputFormatWithInfinity(AswbB.dPsi_ast, 3, cMaxRF, FALSE), D_NUMBER_FORMAT_3));
                    }
                }
                else //Combined인 경우
                {
                    if ( fabs(AswbB.dPsi) - cDGN_Zero < 0.0 )
                    {
                        arData.Add(D_CELL(nIncreaseRow, 46, _T("-")));
                    }
                    else if ( AswbB.dPsi + cDGN_Zero > cMaxRF )
                    {
                        arData.Add(D_CELL(nIncreaseRow, 46, _T("-")));
                    }
                    else
                    {
                        arData.Add(D_CELL(nIncreaseRow, 46, GetOutputFormatWithInfinity(AswbB.dPsi, 3, cMaxRF, FALSE), D_NUMBER_FORMAT_3));
                    }

                    if ( fabs(AswbB.dPsi_ast) - cDGN_Zero < 0.0 )
                    {
                        arData.Add(D_CELL(nIncreaseRow, 48, _T("-")));
                    }
                    else if ( AswbB.dPsi_ast + cDGN_Zero > cMaxRF )
                    {
                        arData.Add(D_CELL(nIncreaseRow, 48, _T("-")));
                    }
                    else
                    {
                        arData.Add(D_CELL(nIncreaseRow, 48, GetOutputFormatWithInfinity(AswbB.dPsi_ast, 3, cMaxRF, FALSE), D_NUMBER_FORMAT_3));
                    }
                }
                arData.Add(D_CELL(nIncreaseRow, 50, (AswbB.dRF < 1.0 ? _T("NG") : _T("OK"))));
            }
            else
            {
                arData.Add(D_CELL(nIncreaseRow, 5, _T("-")));
                arData.Add(D_CELL(nIncreaseRow, 8, _T("-")));
                arData.Add(D_CELL(nIncreaseRow, 11, _T("-")));
                arData.Add(D_CELL(nIncreaseRow, 14, _T("-")));
                arData.Add(D_CELL(nIncreaseRow, 17, _T("-")));
                arData.Add(D_CELL(nIncreaseRow, 20, _T("-")));
                arData.Add(D_CELL(nIncreaseRow, 23, _T("-")));
                arData.Add(D_CELL(nIncreaseRow, 26, _T("-")));
                arData.Add(D_CELL(nIncreaseRow, 29, _T("-")));
                arData.Add(D_CELL(nIncreaseRow, 32, _T("-")));
                arData.Add(D_CELL(nIncreaseRow, 35, _T("-")));
                arData.Add(D_CELL(nIncreaseRow, 38, _T("-")));
                arData.Add(D_CELL(nIncreaseRow, 41, _T("-")));
                arData.Add(D_CELL(nIncreaseRow, 44, _T("-")));
                arData.Add(D_CELL(nIncreaseRow, 46, _T("-")));
                arData.Add(D_CELL(nIncreaseRow, 48, _T("-")));
                arData.Add(D_CELL(nIncreaseRow, 50, _T("-")));
            }

            nIncreaseRow++;
        }
    }
    if ( nIncreaseRow==0 )
    {
        arData.Add(D_CELL(nIncreaseRow, 1, _T("-")));
        arData.Add(D_CELL(nIncreaseRow, 5, _T("-")));
        arData.Add(D_CELL(nIncreaseRow, 8, _T("-")));
        arData.Add(D_CELL(nIncreaseRow, 11, _T("-")));
        arData.Add(D_CELL(nIncreaseRow, 14, _T("-")));
        arData.Add(D_CELL(nIncreaseRow, 17, _T("-")));
        arData.Add(D_CELL(nIncreaseRow, 20, _T("-")));
        arData.Add(D_CELL(nIncreaseRow, 23, _T("-")));
        arData.Add(D_CELL(nIncreaseRow, 26, _T("-")));
        arData.Add(D_CELL(nIncreaseRow, 29, _T("-")));
        arData.Add(D_CELL(nIncreaseRow, 32, _T("-")));
        arData.Add(D_CELL(nIncreaseRow, 35, _T("-")));
        arData.Add(D_CELL(nIncreaseRow, 38, _T("-")));
        arData.Add(D_CELL(nIncreaseRow, 41, _T("-")));
        arData.Add(D_CELL(nIncreaseRow, 44, _T("-")));
        arData.Add(D_CELL(nIncreaseRow, 46, _T("-")));
        arData.Add(D_CELL(nIncreaseRow, 48, _T("-")));
        arData.Add(D_CELL(nIncreaseRow, 50, _T("-")));

        nIncreaseRow++;
    }

    // Write Data
    CString RptBlock;
    int rowCnt = 0;

    // Table Title
    RptBlock = _T("Rating_Title_Web_Buckling");
    rowCnt = CopyRange(pExcel, RptBlock, stRow);
    stRow += rowCnt;
    // Table Head
    RptBlock = _T("Rating_Box_Web_Buckling_Table_Head");
    rowCnt = CopyRange(pExcel, RptBlock, stRow);
    stRow += rowCnt;

    for ( int i=0; i<nIncreaseRow; ++i )
    {
        if ( i==(nIncreaseRow-1) )
        {
            RptBlock = _T("Rating_Box_Web_Buckling_Table_End");
        }
        else
        {
            RptBlock = _T("Rating_Box_Web_Buckling_Table_Body");
        }
        rowCnt = CopyRange(pExcel, RptBlock, stRow+i);
    }

    Print_Array_D_CELL(pExcel, arData, stRow);
    stRow += nIncreaseRow;
}

void CRating_CS454_STL_XLOut::GenerateAssWebStiffenerTable(IExcel* pExcel, const int& iPosi, const _T_CS454_STL_XLOUT& XloutD, int& stRow)
{
    CArray<T_ALCS_K, T_ALCS_K> arAlcs; arAlcs.RemoveAll();
    m_pDoc->m_pAttrCtrl2->GetAlcsKeyList(arAlcs);
    int nAlcs = arAlcs.GetSize();
    if ( nAlcs==0 ) return;

    ElemPairK EPairK = XloutD.GetElemPairK();

    int nConcurrentSize = GetConcurrentSize();

    int nIncreaseRow = 0;
    // Make Data
    CArray<D_CELL, D_CELL &> arData;
    arData.RemoveAll();
    for ( int i=0; i<nAlcs; ++i )
    {
        T_ALCS_K AlcsK = arAlcs[i];
        BOOL bChk = FALSE;
        XloutD.mWebStiff.Lookup(AlcsK, bChk);
        if ( !bChk ) continue;

        T_ALCS_D AlcsD;
        if ( !m_pDoc->m_pAttrCtrl2->GetAlcs(AlcsK, AlcsD) ) ASSERT(0); // Rating case

        for ( int j=0; j<nConcurrentSize; ++j ) // concurrent
        {
            int iConCurrent = j+1;
            T_ASWS_CSG_BS_D AswsCaseD;
            if ( !m_pDoc->m_pPostCtrl->GetAnalysisResult()->GetWebStiffenerWithLStiffenerBoxShape4CS454_CSGResult(EPairK, AlcsK, 0, iConCurrent, AswsCaseD) ) continue;

            const T_ASWS_CSG_BS_BASE& AswsB =  AswsCaseD.AswsB[iPosi];

            if ( !AswsB.ComD.bWebWithLS ) continue;

            // Set Data
            arData.Add(D_CELL(nIncreaseRow, 1, m_pDoc->m_pPostCtrl->GetRatingCaseNameBD21(AlcsK, iConCurrent)));
            if ( AswsB.ComD.bChk )
            {
                arData.Add(D_CELL(nIncreaseRow, 5, GetOutputFormat(AswsB.dSig_ls *m_dFactorStress, 3), D_NUMBER_FORMAT_3));
                arData.Add(D_CELL(nIncreaseRow, 8, GetOutputFormat(AswsB.dSig_se_SV *m_dFactorStress, 3), D_NUMBER_FORMAT_3));
                arData.Add(D_CELL(nIncreaseRow, 11, GetOutputFormat(AswsB.dSig_se_DL *m_dFactorStress, 3), D_NUMBER_FORMAT_3));
                arData.Add(D_CELL(nIncreaseRow, 14, GetOutputFormat(AswsB.dSig_se_ST *m_dFactorStress, 3), D_NUMBER_FORMAT_3));                
                arData.Add(D_CELL(nIncreaseRow, 17, GetOutputFormatWithInfinity(AswsB.dA, 3, cMaxRF, FALSE), D_NUMBER_FORMAT_3));

                if ( AlcsD.nMovType == 1 ) //Special인 경우
                {
                    arData.Add(D_CELL(nIncreaseRow, 14, _T("-")));
                    arData.Add(D_CELL(nIncreaseRow, 19, _T("-")));
                    if ( fabs(AswsB.dPsi_ast) - cDGN_Zero < 0.0 )
                    {
                        arData.Add(D_CELL(nIncreaseRow, 21, _T("-")));
                    }
                    else if ( AswsB.dPsi_ast + cDGN_Zero > cMaxRF )
                    {
                        arData.Add(D_CELL(nIncreaseRow, 21, _T("-")));
                    }
                    else
                    {
                        arData.Add(D_CELL(nIncreaseRow, 21, GetOutputFormatWithInfinity(AswsB.dPsi_ast, 3, cMaxRF, FALSE), D_NUMBER_FORMAT_3));
                    }
                }
                else //Combined인 경우
                {
                    if ( fabs(AswsB.dPsi) - cDGN_Zero < 0.0 )
                    {
                        arData.Add(D_CELL(nIncreaseRow, 19, _T("-")));
                    }
                    else if ( AswsB.dPsi + cDGN_Zero > cMaxRF )
                    {
                        arData.Add(D_CELL(nIncreaseRow, 19, _T("-")));
                    }
                    else
                    {
                        arData.Add(D_CELL(nIncreaseRow, 19, GetOutputFormatWithInfinity(AswsB.dPsi, 3, cMaxRF, FALSE), D_NUMBER_FORMAT_3));
                    }

                    if ( fabs(AswsB.dPsi_ast) - cDGN_Zero < 0.0 )
                    {
                        arData.Add(D_CELL(nIncreaseRow, 21, _T("-")));
                    }
                    else if ( AswsB.dPsi_ast + cDGN_Zero > cMaxRF )
                    {
                        arData.Add(D_CELL(nIncreaseRow, 21, _T("-")));
                    }
                    else
                    {
                        arData.Add(D_CELL(nIncreaseRow, 21, GetOutputFormatWithInfinity(AswsB.dPsi_ast, 3, cMaxRF, FALSE), D_NUMBER_FORMAT_3));
                    }
                }
                arData.Add(D_CELL(nIncreaseRow, 23, (AswsB.dRF < 1.0 ? _T("NG") : _T("OK"))));
            }
            else
            {
                arData.Add(D_CELL(nIncreaseRow, 5, _T("-")));
                arData.Add(D_CELL(nIncreaseRow, 8, _T("-")));
                arData.Add(D_CELL(nIncreaseRow, 11, _T("-")));
                arData.Add(D_CELL(nIncreaseRow, 14, _T("-")));
                arData.Add(D_CELL(nIncreaseRow, 17, _T("-")));
                arData.Add(D_CELL(nIncreaseRow, 19, _T("-")));
                arData.Add(D_CELL(nIncreaseRow, 21, _T("-")));
                arData.Add(D_CELL(nIncreaseRow, 23, _T("-")));
            }

            nIncreaseRow++;
        }
    }
    if ( nIncreaseRow==0 )
    {
        arData.Add(D_CELL(nIncreaseRow, 1, _T("-")));
        arData.Add(D_CELL(nIncreaseRow, 5, _T("-")));
        arData.Add(D_CELL(nIncreaseRow, 8, _T("-")));
        arData.Add(D_CELL(nIncreaseRow, 11, _T("-")));
        arData.Add(D_CELL(nIncreaseRow, 14, _T("-")));
        arData.Add(D_CELL(nIncreaseRow, 17, _T("-")));
        arData.Add(D_CELL(nIncreaseRow, 19, _T("-")));
        arData.Add(D_CELL(nIncreaseRow, 21, _T("-")));
        arData.Add(D_CELL(nIncreaseRow, 23, _T("-")));

        nIncreaseRow++;
    }

    // Write Data
    CString RptBlock;
    int rowCnt = 0;

    // Table Title
    RptBlock = _T("Rating_Title_Stiffener");
    rowCnt = CopyRange(pExcel, RptBlock, stRow);
    stRow += rowCnt;
    // Table Head
    RptBlock = _T("Rating_Box_Web_Stiffener_Table_Head");
    rowCnt = CopyRange(pExcel, RptBlock, stRow);
    stRow += rowCnt;

    for ( int i=0; i<nIncreaseRow; ++i )
    {
        if ( i==(nIncreaseRow-1) )
        {
            RptBlock = _T("Rating_Box_Web_Stiffener_Table_End");
        }
        else
        {
            RptBlock = _T("Rating_Box_Web_Stiffener_Table_Body");
        }
        rowCnt = CopyRange(pExcel, RptBlock, stRow+i);
    }

    Print_Array_D_CELL(pExcel, arData, stRow);
    stRow += nIncreaseRow;
}


BOOL CRating_CS454_STL_XLOut::GenerateDetailRpt(IExcel* pExcel, CString szProgramDir, CString szProjectFileDir, CString strPath, D_XL_PROGRESS_INTERFACE* pProgDlg)
{
    ArrElemPairKey aPrintElemKey;
    GetReportElemPairKey(aPrintElemKey);    

    CSG_RPT_CS457_D RptD;
    GetDetailResult(aPrintElemKey, RptD);

    CDgn_CSGRptManager_CS457 RptMgr;

    int nDgnCode = CS457_R1_CSG;//STL_CS454_20; 
    RptMgr.Print_DetailReport(pExcel, pProgDlg, nDgnCode, szProgramDir, szProjectFileDir, strPath, RptD);

    return TRUE;
}

int CRating_CS454_STL_XLOut::GetDetailCount()
{
    ArrElemPairKey aPrtElemK;
    GetReportElemPairKey(aPrtElemK);

    int nDedail = 0;
    INT_PTR nElem = aPrtElemK.GetSize();
    for ( INT_PTR i=0; i<nElem; ++i )
    {
        auto  ElemK = aPrtElemK[i];

        // 출력 옵션 셋팅
        // 1. 출력 여부 결정
        T_SRAS_D SrasD; SrasD.Initialize();  // 설계 위치        
        T_SRAR_D SrarD; SrarD.Initialize();  // 출력 위치        
        GetPrintElemData(ElemK, SrasD, SrarD);

        if ( (SrasD.nPos == 0 || SrasD.nPos == 2) && (SrarD.iPrintChk == 0 || SrarD.iPrintChk == 2) )
        {
            ++nDedail;
        }
        if ( (SrasD.nPos == 1 || SrasD.nPos == 2) && (SrarD.iPrintChk == 1 || SrarD.iPrintChk == 2) )
        {
            ++nDedail;
        }
    }
    return nDedail;
}

BOOL CRating_CS454_STL_XLOut::GetDetailResult(const ArrElemPairKey& aPrtElemK, CSG_RPT_CS457_D &rData)
{
    CDgnStlRatingDataCtrl* pRatingCtrl = m_pDataCtrl->m_pStlRatingDataCtrl;
    if ( !pRatingCtrl ) return FALSE;

    if ( !pRatingCtrl->m_pCSGDataCtrl ) return FALSE;

    CRptSteelGirder_AASHTO12 AASHTORpt(CS457_R1_CSG);

    T_RPSS_D RpssD; RpssD.Initialize();
    m_pDoc->m_pAttrCtrl2->GetRpss(RpssD);

    pRatingCtrl->Make_RatingDgnData();

    CSG_EFF_BREADTH_RATIO_SPAN_D EffBreadthR;
    CSG_C4_VALUE_TABLE_CS457 C4Value;

    CSG_MEMB_RESULT_D      MembR;
    CSG_CHECK_RESULT_CODES ChkResD;

    MembR.pCS457 = new CSG_MEMBPOS_RES_CS457_D;

    ChkResD.pCS457 = new CSG_CHECK_RES_CS457;

    CArray<T_ELEM_K, T_ELEM_K&> aUblmElemK;
    BOOL bCantilever=FALSE;
    for ( INT_PTR i=0; i<aPrtElemK.GetSize(); ++i )
    {
        CSG_MEMB_POSD_KSCE_LSD MembLcomD;
        MembLcomD.aChkFor.SetSize(1);
        pRatingCtrl->ConvertRatingGlobalData(RpssD, MembLcomD.Memb.Csgd);

        auto  ElemK = aPrtElemK[i];        

        // 출력 옵션 셋팅
        // 1. 출력 여부 결정
        T_SRAS_D SrasD; SrasD.Initialize();  // 설계 위치        
        T_SRAR_D SrarD; SrarD.Initialize();  // 출력 위치        
        GetPrintElemData(ElemK, SrasD, SrarD);


        BOOL bPrtIJ[2] = { FALSE, FALSE };
        bPrtIJ[0] = ((SrasD.nPos == 0 || SrasD.nPos == 2) && (SrarD.iPrintChk == 0 || SrarD.iPrintChk == 2));
        bPrtIJ[1] = ((SrasD.nPos == 1 || SrasD.nPos == 2) && (SrarD.iPrintChk == 1 || SrarD.iPrintChk == 2));

        int nMatlK = m_pDataCtrl->GetMaterialKey4ElemPairK(ElemK);

        if ( !pRatingCtrl->m_pCSGDataCtrl->m_mCsgMatl.Lookup(nMatlK, MembLcomD.Memb.Matl) ) MembLcomD.Memb.Matl.Initialize();

        double dVuFactor = 1.0;
        pRatingCtrl->m_pCSGDataCtrl->GetCsgSectionSupport(ElemK, MembLcomD);
        if (MembLcomD.Memb.PosD[0].Sect.SectInfo.nStype == DGN_SECT_TYPE_COMPO_B ||
            MembLcomD.Memb.PosD[0].Sect.SectInfo.nStype == DGN_SECT_TYPE_COMPO_TUB)
        {
            dVuFactor = 0.5;
        }

        pRatingCtrl->m_pCSGDataCtrl->Get_CsgMembInData(ElemK, MembLcomD);

        T_SPAN_K CurSpanK=0;

        T_KFAC_D KFacD; KFacD.Initialize();
        BOOL bGetKfac = m_pDataCtrl->GetGenKfactor(ElemK, KFacD);
        pRatingCtrl->m_pCSGDataCtrl->Get_UnbracedLengthData(ElemK, bGetKfac, KFacD, MembLcomD.Memb.Ulen);
        //
        pRatingCtrl->m_pCSGDataCtrl->Get_CsgUlmSection(ElemK, MembLcomD);

        MembLcomD.bChkUlmf = true;

        pRatingCtrl->m_pCSGDataCtrl->MakeMemberResultData(ElemK, MembLcomD, MembR);
        MembR.pCS457->MembR[0].pC4 = &C4Value;
        MembR.pCS457->MembR[1].pC4 = &C4Value;

        for ( int j=0; j<2; ++j )
        {
            //PscMembD.Pos[j].CalcD.bTypeC = AsopD.bTypeC;
            //m_pDataCtrl->m_pPscDataCtrl->Get_PscDataPosi(ElemK, j, PscMembD.Pos[j], bComposite);
        }

        for ( int j=0; j<2; ++j )
        {
            if ( bPrtIJ[j] )
            {
                MembLcomD.Memb.ChIf.nChkPos = j;
                CSG_RPT_CS457_POS RptResD;
                RptResD.PosD = MembLcomD.Memb;
                CSG_MEMB_RES_AASHTO_LRFD_D AASHTOMemb;
                AASHTOMemb.Memb.dCzp_rb  = MembR.pCS457->MembR[j].Memb.dCzp_rb;
                AASHTOMemb.Memb.dCzp_rb3 = MembR.pCS457->MembR[j].Memb.dCzp_rb3;
                AASHTORpt.SetRptData(ElemK.first, j, MembLcomD.Memb, AASHTOMemb);
                MembR.pCS457->MembR[j].Memb = AASHTOMemb.Memb;
                RptResD.MembR = MembR.pCS457->MembR[j];
                for ( int k=0; k<2; ++k )
                {
                    int nPosNeg = k+1;
                    GetDetailFlex(ElemK, j, nPosNeg, MembLcomD, *MembR.pCS457, *ChkResD.pCS457, RptResD);
                }
                GetDetailShear(ElemK, j, MembLcomD, *MembR.pCS457, *ChkResD.pCS457, RptResD);
                GetDetailCombinedMV(ElemK, j, MembLcomD, *MembR.pCS457, *ChkResD.pCS457, RptResD);
                GetDetailLongShear(TRUE, ElemK, j, MembLcomD, *MembR.pCS457, *ChkResD.pCS457, RptResD);
                //GetDetailShrr(ElemK, PscMembD, j, RptResD);
                //GetDetailTorr(ElemK, PscMembD, j, RptResD);

                int nComTen = 1;
                GetDetailFlange4Box(ElemK, j, nComTen, MembLcomD, *MembR.pCS457, *ChkResD.pCS457, RptResD);
                GetDetailFlangeStiffener4Box(ElemK, j, MembLcomD, *MembR.pCS457, *ChkResD.pCS457, RptResD);
                for ( int k=0; k<2; ++k )
                {
                    int nPosNeg = k+1;
                    GetDetailWebYieldBuckling4Box(ElemK, j, nPosNeg, MembLcomD, *MembR.pCS457, *ChkResD.pCS457, RptResD);
                }


                for ( int k=0; k<2; ++k )
                {
                    nComTen = k+1;
                    GetDetailSLS(ElemK, j, nComTen, MembLcomD, *MembR.pCS457, *ChkResD.pCS457, RptResD);
                }

                GetDetailLongShear(FALSE, ElemK, j, MembLcomD, *MembR.pCS457, *ChkResD.pCS457, RptResD);

                rData.aRptD.Add(RptResD);
            }
        }
    }
    return TRUE;
}

BOOL CRating_CS454_STL_XLOut::GetDetailFlex(ElemPairK ElemK, const int& nPos, const int& nPosNeg, CSG_MEMB_POSD_KSCE_LSD& MembLcomD, CSG_MEMBPOS_RES_CS457_D& MembResD, CSG_CHECK_RES_CS457& ResD, CSG_RPT_CS457_POS &rData)
{                             
    T_ASFC_CSG_BS_CASE AsfcCaseD;
    int nPN = nPosNeg;
    m_pDgnResult->GetFlexuralStrengthRating4CS454CSG(ElemK, 0, nPN, 0, AsfcCaseD);
    if ( nPosNeg==1 )
    {
        rData.bFlexPos = AsfcCaseD.AsfcB[nPos].ComD.bChk;
        if ( !rData.bFlexPos ) return FALSE;
    }
    else
    {
        rData.bFlexNeg = AsfcCaseD.AsfcB[nPos].ComD.bChk;
        if ( !rData.bFlexNeg ) return FALSE;
    }    

    CDgnStlRatingDataCtrl* pRatingCtrl = m_pDataCtrl->m_pStlRatingDataCtrl;
    if ( !pRatingCtrl ) return FALSE;

    if ( !pRatingCtrl->m_pCSGDataCtrl ) return FALSE;

    CSG_MEMBPOS_RES_CS457_D TMembRes = MembResD;
    CSG_MEMB_RESULT_D TMembR;
    TMembR.pCS457 = &TMembRes;

    CSG_CHK_RES_DTR_PARAM  DtrR;

    CSG_STRN_RES_CS457 StrnD;
    CSG_CHECK_RES_CS457 TRes;
    TRes.pStrnD = &StrnD;
    CSG_CHECK_RESULT_CODES TChkResD;
    TChkResD.pCS457 = &TRes;
    CString strRCase = GetRatingCaseName(AsfcCaseD.AsfcB[nPos].ComD.RCaseK);
    CString strRCaseSubType = m_pDataCtrl->GetLcomType(AsfcCaseD.AsfcB[nPos].ComD.unConcurrent);

    MembLcomD.aChkFor[0].nLcomType = 0;
    MembLcomD.aUlmf.RemoveAll();
    MembLcomD.aUlmf.SetSize(6);


    ConvertCsgDgnForce(AsfcCaseD.AsfcB[nPos].ComD, MembLcomD);


    pRatingCtrl->m_pCSGDataCtrl->Check_Strength(ElemK, nPos, MembLcomD, TMembR, TChkResD, DtrR);
    TChkResD.pCS457->pStrnD->FlexD.ForStrD.strLcomName = CT2W(strRCase);
    TChkResD.pCS457->pStrnD->FlexD.ForStrD.strLcomType = CT2W(strRCaseSubType);

    int nPosNegId = nPosNeg==1 ? 0 : 1;
    rData.FlexD[nPosNegId] = TChkResD.pCS457->pStrnD->FlexD;
    rData.FlexCompFlg = TChkResD.pCS457->pStrnD->FlexD;

    return TRUE;
}

BOOL CRating_CS454_STL_XLOut::GetDetailShear(ElemPairK ElemK, const int& nPos, CSG_MEMB_POSD_KSCE_LSD& MembLcomD, CSG_MEMBPOS_RES_CS457_D& MembResD, CSG_CHECK_RES_CS457& ResD, CSG_RPT_CS457_POS &rData)
{
    T_ASSC_CSG_BS_CASE AsscCaseD;

    m_pDgnResult->GetShearStrengthRating4CS454CSG(ElemK, 0, 0, AsscCaseD);
    rData.bShear = AsscCaseD.AsscB[nPos].ComD.bChk;
    if ( !rData.bShear ) return FALSE;

    CDgnStlRatingDataCtrl* pRatingCtrl = m_pDataCtrl->m_pStlRatingDataCtrl;
    if ( !pRatingCtrl ) return FALSE;

    if ( !pRatingCtrl->m_pCSGDataCtrl ) return FALSE;

    CSG_MEMBPOS_RES_CS457_D TMembRes = MembResD;
    CSG_MEMB_RESULT_D TMembR;
    TMembR.pCS457 = &TMembRes;

    CSG_CHK_RES_DTR_PARAM  DtrR;

    CSG_STRN_RES_CS457 StrnD;
    CSG_CHECK_RES_CS457 TRes;
    TRes.pStrnD = &StrnD;
    CSG_CHECK_RESULT_CODES TChkResD;
    TChkResD.pCS457 = &TRes;
    CString strRCase = GetRatingCaseName(AsscCaseD.AsscB[nPos].ComD.RCaseK);
    CString strRCaseSubType = m_pDataCtrl->GetLcomType(AsscCaseD.AsscB[nPos].ComD.unConcurrent);

    MembLcomD.aChkFor[0].nLcomType = 0;
    MembLcomD.aUlmf.RemoveAll();
    MembLcomD.aUlmf.SetSize(6);

    ConvertCsgDgnForce(AsscCaseD.AsscB[nPos].ComD, MembLcomD);

    pRatingCtrl->m_pCSGDataCtrl->Check_Strength(ElemK, nPos, MembLcomD, TMembR, TChkResD, DtrR);
    TChkResD.pCS457->pStrnD->ShearD.ForStrD.strLcomName = CT2W(strRCase);
    TChkResD.pCS457->pStrnD->ShearD.ForStrD.strLcomType = CT2W(strRCaseSubType);

    rData.ShearD = TChkResD.pCS457->pStrnD->ShearD;

    return TRUE;
}

BOOL CRating_CS454_STL_XLOut::GetDetailCombinedMV(ElemPairK ElemK, const int& nPos, CSG_MEMB_POSD_KSCE_LSD& MembLcomD, CSG_MEMBPOS_RES_CS457_D& MembResD, CSG_CHECK_RES_CS457& ResD, CSG_RPT_CS457_POS &rData)
{
    T_ASCB_CSG_BS_CASE AscbCaseD;
    m_pDgnResult->GetCombinedMVStrength4CS454_CSGResult(ElemK, 0, 0, AscbCaseD);
    rData.bCombine = AscbCaseD.AscbB[nPos].ComD.bChk;
    if ( !rData.bCombine ) return FALSE;

    CDgnStlRatingDataCtrl* pRatingCtrl = m_pDataCtrl->m_pStlRatingDataCtrl;
    if ( !pRatingCtrl ) return FALSE;

    if ( !pRatingCtrl->m_pCSGDataCtrl ) return FALSE;

    CSG_MEMBPOS_RES_CS457_D TMembRes = MembResD;
    CSG_MEMB_RESULT_D TMembR;
    TMembR.pCS457 = &TMembRes;

    CSG_CHK_RES_DTR_PARAM  DtrR;

    CSG_STRN_RES_CS457 StrnD;
    CSG_CHECK_RES_CS457 TRes;
    TRes.pStrnD = &StrnD;
    CSG_CHECK_RESULT_CODES TChkResD;
    TChkResD.pCS457 = &TRes;
    CString strRCase = GetRatingCaseName(AscbCaseD.AscbB[nPos].ComD.RCaseK);
    CString strRCaseSubType = m_pDataCtrl->GetLcomType(AscbCaseD.AscbB[nPos].ComD.unConcurrent);

    MembLcomD.aChkFor[0].nLcomType = 0;
    MembLcomD.aUlmf.RemoveAll();
    MembLcomD.aUlmf.SetSize(6);

    ConvertCsgDgnForce(AscbCaseD.AscbB[nPos].ComD, MembLcomD);
    m_pDataCtrl->m_pCsgDataCtrl->ConvertCsgComfCvlToEng(AscbCaseD.AscbB[nPos].ForcePanelM, MembLcomD.PanelM);
    m_pDataCtrl->m_pCsgDataCtrl->ConvertCsgComfCvlToEng(AscbCaseD.AscbB[nPos].ForcePanelV, MembLcomD.PanelV);

    pRatingCtrl->m_pCSGDataCtrl->Check_Strength(ElemK, nPos, MembLcomD, TMembR, TChkResD, DtrR);
    TChkResD.pCS457->pStrnD->FlexD.ForStrD.strLcomName = CT2W(strRCase);
    TChkResD.pCS457->pStrnD->FlexD.ForStrD.strLcomType = CT2W(strRCaseSubType);
    TChkResD.pCS457->pStrnD->CombD.ForStrD.strLcomName = TChkResD.pCS457->pStrnD->FlexD.ForStrD.strLcomName;
    TChkResD.pCS457->pStrnD->CombD.ForStrD.strLcomType = TChkResD.pCS457->pStrnD->FlexD.ForStrD.strLcomType;

    rData.CombD = TChkResD.pCS457->pStrnD->CombD;
    rData.CombFlex = TChkResD.pCS457->pStrnD->FlexD;

    return TRUE;
}

BOOL CRating_CS454_STL_XLOut::GetDetailLongShear(BOOL bULS, ElemPairK ElemK, const int& nPos, CSG_MEMB_POSD_KSCE_LSD& MembLcomD, CSG_MEMBPOS_RES_CS457_D& MembResD, CSG_CHECK_RES_CS457& ResD, CSG_RPT_CS457_POS &rData)
{
    T_ASLS_CSG_BS_CASE AslsCaseD;
    if ( bULS )
    {
        m_pDgnResult->GetLongShear4CS454_CSGResult(ElemK, 0, 0, AslsCaseD);
        rData.bSCon = AslsCaseD.AslsB[nPos].ComD.bChk;
        if ( !rData.bSCon ) return FALSE;
    }
    else
    {
        m_pDgnResult->GetLongShearSLS4CS454_CSGResult(ElemK, 0, 0, AslsCaseD);
        rData.bSConServ = AslsCaseD.AslsB[nPos].ComD.bChk;
        if ( !rData.bSConServ ) return FALSE;
    }    

    CDgnStlRatingDataCtrl* pRatingCtrl = m_pDataCtrl->m_pStlRatingDataCtrl;
    if ( !pRatingCtrl ) return FALSE;

    if ( !pRatingCtrl->m_pCSGDataCtrl ) return FALSE;

    CSG_MEMBPOS_RES_CS457_D TMembRes = MembResD;
    CSG_MEMB_RESULT_D TMembR;
    TMembR.pCS457 = &TMembRes;

    CSG_CHK_RES_DTR_PARAM  DtrR;

    CSG_STRN_RES_CS457 StrnD;
    CSG_SCON_RES_CS457 SconD;
    CSG_SCON_RES_CS457 ServSconD;
    CSG_CHECK_RES_CS457 TRes;
    TRes.pStrnD = &StrnD;
    TRes.pSconD = &SconD;
    TRes.pServSconD = &ServSconD;
    CSG_CHECK_RESULT_CODES TChkResD;
    TChkResD.pCS457 = &TRes;

    CString strRCase = GetRatingCaseName(AslsCaseD.AslsB[nPos].ComD.RCaseK);
    CString strRCaseSubType = m_pDataCtrl->GetLcomType(AslsCaseD.AslsB[nPos].ComD.unConcurrent);

    if ( bULS )
    {
        MembLcomD.aChkFor[0].nLcomType = 0;
    }
    else
    {
        MembLcomD.aChkFor[0].nLcomType = 1;
    }

    MembLcomD.aUlmf.RemoveAll();
    MembLcomD.aUlmf.SetSize(6);


    ConvertCsgDgnForce(AslsCaseD.AslsB[nPos].ComD, MembLcomD);

    pRatingCtrl->m_pCSGDataCtrl->Check_ShearConnector(ElemK, nPos, MembLcomD, TMembR, TChkResD, DtrR);

    if ( bULS )
    {
        TChkResD.pCS457->pSconD->ForStrD.strLcomName = CT2W(strRCase);
        TChkResD.pCS457->pSconD->ForStrD.strLcomType = CT2W(strRCaseSubType);
        rData.SConD = *TChkResD.pCS457->pSconD;
    }
    else
    {
        TChkResD.pCS457->pServSconD->ForStrD.strLcomName = CT2W(strRCase);
        TChkResD.pCS457->pServSconD->ForStrD.strLcomType = CT2W(strRCaseSubType);
        rData.SConServD = *TChkResD.pCS457->pServSconD;
    }

    return TRUE;
}

BOOL CRating_CS454_STL_XLOut::GetDetailFlange4Box(ElemPairK ElemK, const int& nPos, const int& nComTen, CSG_MEMB_POSD_KSCE_LSD& MembLcomD, CSG_MEMBPOS_RES_CS457_D& MembResD, CSG_CHECK_RES_CS457& ResD, CSG_RPT_CS457_POS &rData)
{
    T_ASFG_CSG_BS_D AsfgD;
    int nCT = nComTen;
    m_pDgnResult->GetFlangeWithLStiffenerBoxShape4CS454_CSGResult(ElemK, 0, nCT, 0, AsfgD);
    T_ASFG_CSG_BS_BASE AsfgB = AsfgD.AsfgB[nPos*2]; // 압축
    rData.bFlexCompFlg = AsfgB.ComD.bChk;
    if ( !rData.bFlexCompFlg ) return FALSE;

    CDgnStlRatingDataCtrl* pRatingCtrl = m_pDataCtrl->m_pStlRatingDataCtrl;
    if ( !pRatingCtrl ) return FALSE;

    if ( !pRatingCtrl->m_pCSGDataCtrl ) return FALSE;

    CSG_MEMBPOS_RES_CS457_D TMembRes = MembResD;
    CSG_MEMB_RESULT_D TMembR;
    TMembR.pCS457 = &TMembRes;

    CSG_CHK_RES_DTR_PARAM  DtrR;

    CSG_STRN_RES_CS457 StrnD;
    CSG_CHECK_RES_CS457 TRes;
    TRes.pStrnD = &StrnD;
    CSG_CHECK_RESULT_CODES TChkResD;
    TChkResD.pCS457 = &TRes;
    CString strRCase = GetRatingCaseName(AsfgB.ComD.RCaseK);
    CString strRCaseSubType = m_pDataCtrl->GetLcomType(AsfgB.ComD.unConcurrent);

    MembLcomD.aChkFor[0].nLcomType = 0;
    MembLcomD.aUlmf.RemoveAll();
    MembLcomD.aUlmf.SetSize(6);

    ConvertCsgDgnForce(AsfgB.ComD, MembLcomD);

    pRatingCtrl->m_pCSGDataCtrl->Check_Strength(ElemK, nPos, MembLcomD, TMembR, TChkResD, DtrR);
    TChkResD.pCS457->pStrnD->FlexD.ForStrD.strLcomName = CT2W(strRCase);
    TChkResD.pCS457->pStrnD->FlexD.ForStrD.strLcomType = CT2W(strRCaseSubType);

    int nPosNegId = nComTen==1 ? 0 : 1;
    rData.FlexCompFlg = TChkResD.pCS457->pStrnD->FlexD;

    return TRUE;

}

BOOL CRating_CS454_STL_XLOut::GetDetailFlangeStiffener4Box(ElemPairK ElemK, const int& nPos, CSG_MEMB_POSD_KSCE_LSD& MembLcomD, CSG_MEMBPOS_RES_CS457_D& MembResD, CSG_CHECK_RES_CS457& ResD, CSG_RPT_CS457_POS &rData)
{
    T_ASFS_CSG_BS_CASE AsfsD;
    m_pDgnResult->GetFlangeStiffenerWithLStiffenerBoxShape4CS454_CSGResult(ElemK, 0, 0, AsfsD);
    rData.bFlexFlgStiff = AsfsD.AsfsB[nPos].ComD.bChk;
    if ( !rData.bFlexFlgStiff ) return FALSE;

    CDgnStlRatingDataCtrl* pRatingCtrl = m_pDataCtrl->m_pStlRatingDataCtrl;
    if ( !pRatingCtrl ) return FALSE;

    if ( !pRatingCtrl->m_pCSGDataCtrl ) return FALSE;

    CSG_MEMBPOS_RES_CS457_D TMembRes = MembResD;
    CSG_MEMB_RESULT_D TMembR;
    TMembR.pCS457 = &TMembRes;

    CSG_CHK_RES_DTR_PARAM  DtrR;

    CSG_STRN_RES_CS457 StrnD;
    CSG_CHECK_RES_CS457 TRes;
    TRes.pStrnD = &StrnD;
    CSG_CHECK_RESULT_CODES TChkResD;
    TChkResD.pCS457 = &TRes;
    CString strRCase = GetRatingCaseName(AsfsD.AsfsB[nPos].ComD.RCaseK);
    CString strRCaseSubType = m_pDataCtrl->GetLcomType(AsfsD.AsfsB[nPos].ComD.unConcurrent);

    MembLcomD.aChkFor[0].nLcomType = 0;
    MembLcomD.aUlmf.RemoveAll();
    MembLcomD.aUlmf.SetSize(6);

    ConvertCsgDgnForce(AsfsD.AsfsB[nPos].ComD, MembLcomD);

    pRatingCtrl->m_pCSGDataCtrl->Check_Strength(ElemK, nPos, MembLcomD, TMembR, TChkResD, DtrR);
    TChkResD.pCS457->pStrnD->FlexD.ForStrD.strLcomName = CT2W(strRCase);
    TChkResD.pCS457->pStrnD->FlexD.ForStrD.strLcomType = CT2W(strRCaseSubType);

    rData.FlexFlgStiff = TChkResD.pCS457->pStrnD->FlexD;

    return TRUE;
}

BOOL CRating_CS454_STL_XLOut::GetDetailWebYieldBuckling4Box(ElemPairK ElemK, const int& nPos, const int& nPosNeg, CSG_MEMB_POSD_KSCE_LSD& MembLcomD, CSG_MEMBPOS_RES_CS457_D& MembResD, CSG_CHECK_RES_CS457& ResD, CSG_RPT_CS457_POS &rData)
{
    T_ASWY_CSG_BS_D AswyD;
    int nPN = nPosNeg;
    m_pDgnResult->GetWebYieldingWithLStiffenerBoxShape4CS454_CSGResult(ElemK, 0, nPN, 0, AswyD);
    rData.bFlexWeb = AswyD.AswyB[nPos].ComD.bChk;
    if ( !rData.bFlexWeb ) return FALSE;

    CDgnStlRatingDataCtrl* pRatingCtrl = m_pDataCtrl->m_pStlRatingDataCtrl;
    if ( !pRatingCtrl ) return FALSE;

    if ( !pRatingCtrl->m_pCSGDataCtrl ) return FALSE;

    CSG_MEMBPOS_RES_CS457_D TMembRes = MembResD;
    CSG_MEMB_RESULT_D TMembR;
    TMembR.pCS457 = &TMembRes;

    CSG_CHK_RES_DTR_PARAM  DtrR;

    CSG_STRN_RES_CS457 StrnD;
    CSG_CHECK_RES_CS457 TRes;
    TRes.pStrnD = &StrnD;
    CSG_CHECK_RESULT_CODES TChkResD;
    TChkResD.pCS457 = &TRes;
    CString strRCase = GetRatingCaseName(AswyD.AswyB[nPos].ComD.RCaseK);
    CString strRCaseSubType = m_pDataCtrl->GetLcomType(AswyD.AswyB[nPos].ComD.unConcurrent);

    MembLcomD.aChkFor[0].nLcomType = 0;
    MembLcomD.aUlmf.RemoveAll();
    MembLcomD.aUlmf.SetSize(6);

    ConvertCsgDgnForce(AswyD.AswyB[nPos].ComD, MembLcomD);
    for ( int i=0; i<2; ++i )
    {
        m_pDataCtrl->m_pCsgDataCtrl->ConvertCsgComfCvlToEng(AswyD.AswyB[nPos].Panelij[i], MembLcomD.Panelij[i]);
    }    

    pRatingCtrl->m_pCSGDataCtrl->Check_Strength(ElemK, nPos, MembLcomD, TMembR, TChkResD, DtrR);
    TChkResD.pCS457->pStrnD->FlexD.ForStrD.strLcomName = CT2W(strRCase);
    TChkResD.pCS457->pStrnD->FlexD.ForStrD.strLcomType = CT2W(strRCaseSubType);

    int nPosNegId = nPosNeg==1 ? 0 : 1;
    rData.FlexWeb[nPosNegId] = TChkResD.pCS457->pStrnD->FlexD;

    return TRUE;
}

BOOL CRating_CS454_STL_XLOut::GetDetailSLS(ElemPairK ElemK, const int& nPos, const int& nComTen, CSG_MEMB_POSD_KSCE_LSD& MembLcomD, CSG_MEMBPOS_RES_CS457_D& MembResD, CSG_CHECK_RES_CS457& ResD, CSG_RPT_CS457_POS &rData)
{
    T_ASSL_CSG_BS_CASE AsslCaseD;
    int nCT = nComTen;
    m_pDgnResult->GetSLSRating4CS454CSG(ElemK, 0, nCT, 0, AsslCaseD);
    if ( nComTen==1 )
    {
        rData.bServStrComp = AsslCaseD.AsslB[nPos].ComD.bChk;
        if ( !rData.bServStrComp ) return FALSE;
    }
    else
    {
        rData.bServStrTens = AsslCaseD.AsslB[nPos].ComD.bChk;
        if ( !rData.bServStrTens ) return FALSE;
    }    

    CDgnStlRatingDataCtrl* pRatingCtrl = m_pDataCtrl->m_pStlRatingDataCtrl;
    if ( !pRatingCtrl ) return FALSE;

    if ( !pRatingCtrl->m_pCSGDataCtrl ) return FALSE;

    CSG_MEMBPOS_RES_CS457_D TMembRes = MembResD;
    CSG_MEMB_RESULT_D TMembR;
    TMembR.pCS457 = &TMembRes;

    CSG_CHK_RES_DTR_PARAM  DtrR;

    CSG_SERV_RES_CS457 ServD;
    CSG_CHECK_RES_CS457 TRes;
    TRes.pServD = &ServD;
    CSG_CHECK_RESULT_CODES TChkResD;
    TChkResD.pCS457 = &TRes;
    CString strRCase = GetRatingCaseName(AsslCaseD.AsslB[nPos].ComD.RCaseK);
    CString strRCaseSubType = m_pDataCtrl->GetLcomType(AsslCaseD.AsslB[nPos].ComD.unConcurrent);

    MembLcomD.aChkFor[0].nLcomType = 1;
    MembLcomD.aUlmf.RemoveAll();
    MembLcomD.aUlmf.SetSize(6);

    ConvertCsgDgnForce(AsslCaseD.AsslB[nPos].ComD, MembLcomD);


    pRatingCtrl->m_pCSGDataCtrl->Check_Service(ElemK, nPos, MembLcomD, TMembR, TChkResD, DtrR);
    ServD.ForStrD.strLcomName = CT2W(strRCase);
    ServD.ForStrD.strLcomType = CT2W(strRCaseSubType);

    int nChkIndex = nComTen==1 ? 0 : 1;
    rData.ServStr[nChkIndex] = *TChkResD.pCS457->pServD;

    return TRUE;
}

void CRating_CS454_STL_XLOut::ConvertCsgDgnForce(T_ASCD_CSG_BS_BASE& TComD, CSG_MEMB_POSD_KSCE_LSD& rData)
{
    CSG_FORCE_COMP& ForStrEng = rData.aChkFor[0];
    m_pDataCtrl->m_pCsgDataCtrl->ConvertCsgComfCvlToEng(TComD.ForD.ChkFor, ForStrEng);

    CSG_FORCE_COMP ForStrULEng;
    m_pDataCtrl->m_pCsgDataCtrl->ConvertCsgComfCvlToEng(TComD.ForD.MembFor[0], ForStrULEng);
    m_pDataCtrl->m_pCsgDataCtrl->ConvertCsgUlmForce(0, ForStrULEng, rData.aUlmf);
    ForStrULEng.Initialize();
    m_pDataCtrl->m_pCsgDataCtrl->ConvertCsgComfCvlToEng(TComD.ForD.MembFor[1], ForStrULEng);
    m_pDataCtrl->m_pCsgDataCtrl->ConvertCsgUlmForce(1, ForStrULEng, rData.aUlmf);
    ForStrULEng.Initialize();
    m_pDataCtrl->m_pCsgDataCtrl->ConvertCsgComfCvlToEng(TComD.ForD.MembFor[2], ForStrULEng);
    m_pDataCtrl->m_pCsgDataCtrl->ConvertCsgUlmForce(2, ForStrULEng, rData.aUlmf);

}

CString CRating_CS454_STL_XLOut::GetRatingCaseName(T_ALCS_K AlcsK)
{
    CString strCase = _T("");
    T_ALCS_D AlcsD;
    if ( m_pDoc->m_pAttrCtrl2->GetAlcs(AlcsK, AlcsD) )
    {
        strCase = AlcsD.strCaseName;
    }
    return strCase;
}

int CRating_CS454_STL_XLOut::GetConcurrentSize()
{
    return 12;
}

void CRating_CS454_STL_XLOut::GetReportElemPairKey(ArrElemPairKey& aElemPairKey)
{
    CArray<T_SRAR_K, T_SRAR_K> aPrintElemKey;
    m_pDoc->m_pAttrCtrl2->GetSrarKeyList(aPrintElemKey);
    if (aPrintElemKey.GetSize() > 0)
    {
        CDBLib::ConvertToElemPairKey(EN_EL_BEAM, aPrintElemKey, aElemPairKey);
    }    

    CArray<T_SRRV_K, T_SRRV_K> aPrintVBeamKey;
    m_pDoc->m_pAttrCtrl2->GetSrrvKeyList(aPrintVBeamKey);
    if (aPrintVBeamKey.GetSize() > 0)
    {
        ArrElemPairKey aPrtVBeamPairKey;
        CDBLib::ConvertToElemPairKey(EN_EL_VBEAM, aPrintVBeamKey, aPrtVBeamPairKey);
        aElemPairKey.Append(aPrtVBeamPairKey);
    }
}

BOOL CRating_CS454_STL_XLOut::GetPrintElemData(ElemPairK EPairK, T_SRAS_D& SrasD, T_SRAR_D& SrarD)
{
    // 출력 옵션 셋팅
    // 1. 출력 여부 결정
    SrasD.Initialize();  // 설계 위치
    SrarD.Initialize();  // 출력 위치

    if (EPairK.second == EN_EL_BEAM)
    {
        if (!m_pDoc->m_pAttrCtrl2->GetSras(EPairK.first, SrasD)) return FALSE;
        if (!m_pDoc->m_pAttrCtrl2->GetSrar(EPairK.first, SrarD)) return FALSE;
    }
    else if (EPairK.second == EN_EL_VBEAM)
    {
        T_SRCV_D SrcvD; SrcvD.Initialize();
        if (!m_pDoc->m_pAttrCtrl2->GetSrcv(EPairK.first, SrcvD)) return FALSE;
        SrasD=SrcvD;
        //
        T_SRRV_D SrrvD; SrrvD.Initialize();
        if (!m_pDoc->m_pAttrCtrl2->GetSrrv(EPairK.first, SrrvD)) return FALSE;
        SrarD=SrrvD;
    }
    else ASSERT(0);

    return TRUE;
}

