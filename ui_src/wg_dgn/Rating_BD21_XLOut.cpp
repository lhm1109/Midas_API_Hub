#include "stdafx.h"
#include "..\DgnEngine\Src\DgnCalcBase\DgnCalcBase_PSC_Struct.h"
#include "..\DgnEngine\Src\DgnCalcBase\DgnCalcBase_PSC_ResultStruct.h"
#include "..\DgnEngine\Src\CVL_DgnCalc_US\DgnCalc_US_PSC_Struct.h"
#include "Rating_BD21_XLOut.h"

#include "DgnPscDataCtrl.h"
#include "Dgn_PSCRptManager_EC2.h"

#include "..\wg_base\TestEnvMgr.h"
#include "..\wg_base\Product.h"
#include "..\wg_base\wg_base_MsgDll.h"
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
const double cMaxRF = 1.0E+70;
#define DgnAbsDivide(a,b) fabs(b)>cDGN_Zero ? fabs(a/b) : cMaxRF

CRating_BD21_XLOut::CRating_BD21_XLOut(void)
{
}

CRating_BD21_XLOut::~CRating_BD21_XLOut(void)
{
}

void CRating_BD21_XLOut::SetDataCtrlPointer(CCRCDataCtrl* pDataCtrl)
{
    m_pDataCtrl = pDataCtrl;
}

void CRating_BD21_XLOut::PrintReport(CString szProgramDir, CString szProjectFileDir, CString strPath, int iPrintOpt, CArray<_T_BD21_XLOUT, _T_BD21_XLOUT&>& arXlout)
{
    m_strBasePath   = szProgramDir;
    m_strDataPath   = szProjectFileDir;

    CDgnEngineProgressDlg ProDlg;
    ProDlg.Create_ProgressDlg(PROGRESS_TYPE_PSC_RATING_XL_RPT);// PSC Rating XL Report -> 202

    m_strBasePath = m_pDoc->GetProgramPath() + _T("Excel Base File\\");
	m_strBaseBook = _T("PSC_CS455_Report_base.xlsx");
	//m_strBaseBook = _T("PSC_CS455_Report_base.xls");

    CArray<unsigned int, unsigned int> arProcID;
    GetExistedExcelProcessID(arProcID);

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

    BOOL bIsRegressionTest = CDBDoc::GetDocPoint()->IsRegTestMode();
    
    SetOutputUnitSystem();
    SetUnitConvertFactor();

    int nElem = arXlout.GetSize();
    int nProgressCount = 1;
    int nProgressTotal = 1 + 2*nElem + GetDetailCount(); // Summary + Assessment(IJ*Element) + Detail

    if ( !bIsRegressionTest )
    {
        // #1. Summary
        SetSummarySheetPage(pXL);
        GenerateSummarySheet(pXL, arXlout);
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

void CRating_BD21_XLOut::GenerateSummarySheet(IExcel* pExcel, CArray<_T_BD21_XLOUT, _T_BD21_XLOUT&>& arXlout)
{
    CString RptBlock;
    int stRow  = 1;
    int rowCnt = 0;

    // Set Title
    RptBlock = _T("Title_Summary_Table");
    rowCnt = CopyRange(pExcel, RptBlock, stRow);
    stRow += rowCnt;

    // Set table Head
    RptBlock = _T("Summary_Table_Head");
    rowCnt = CopyRange(pExcel, RptBlock, stRow);
    stRow += rowCnt;

    CArray<T_ALCS_K, T_ALCS_K> arAlcs; arAlcs.RemoveAll();
    m_pDoc->m_pAttrCtrl2->GetAlcsKeyList(arAlcs);
    int nAlcs = arAlcs.GetSize();
    if ( nAlcs==0 ) ASSERT(0);

    int nIncreaseRow = 0;
    int nElem = arXlout.GetSize();
    // Make Data
    CArray<D_CELL, D_CELL&> arData;
    arData.RemoveAll();
    CArray<int, int> arMergeRow[2];
    arMergeRow[0].RemoveAll();
    arMergeRow[1].RemoveAll();

    T_ASPM_D AspmD;
    AspmD.Initialize();
    m_pAttrCtrl->GetAspm(AspmD);

    //T_ELEM_K BeforeElemK=0;
    for ( int i=0; i<nElem; ++i )
    {
        auto ElemK = arXlout[i].ElemK;
        for ( int k=0; k<2; ++k )
        {
            int nPosiRow=0;
            if ( !arXlout[i].bIJ[k] ) continue;

            for ( int j=0; j<nAlcs; ++j )
            {
                T_ALCS_K AlcsK = arAlcs[j];
                T_ALCS_D AlcsD;
                if ( !m_pDoc->m_pAttrCtrl2->GetAlcs(AlcsK, AlcsD) ) ASSERT(0); // Rating case

                double dRF[5] = { 0.0, 0.0, 0.0, 0.0, 0.0 };
                double dRFmin = 1.0/pow(cDGN_Zero, 10.0);

                BOOL bChk = FALSE;
                if ( AspmD.bStrLimFlexure )
                {
                    if ( arXlout[i].mPosiFlex.Lookup(AlcsK, bChk) && bChk )
                    {
                        T_ASFC_PSC_BS_CASE AsfcCaseD;
                        if ( m_pDoc->m_pPostCtrl->GetAnalysisResult()->GetFlexuralStrengthRating4BD21(ElemK, AlcsK, 1, 0, AsfcCaseD) )
                        {
                            if ( AsfcCaseD.AsfcBase[k].ComD.bChk ) dRF[0] = AsfcCaseD.AsfcBase[k].dRF;
                        }
                        if ( abs(dRF[0]) > 0.0 ) dRFmin = min(dRFmin, dRF[0]);
                    }
                }

                if ( AspmD.bStrLimFlexure )
                {
                    if ( arXlout[i].mNegaFlex.Lookup(AlcsK, bChk) && bChk )
                    {
                        T_ASFC_PSC_BS_CASE AsfcCaseD;
                        if ( m_pDoc->m_pPostCtrl->GetAnalysisResult()->GetFlexuralStrengthRating4BD21(ElemK, AlcsK, 2, 0, AsfcCaseD) )
                        {
                            if ( AsfcCaseD.AsfcBase[k].ComD.bChk ) dRF[1] = AsfcCaseD.AsfcBase[k].dRF;
                        }
                        if ( abs(dRF[1]) > 0.0 ) dRFmin = min(dRFmin, dRF[1]);
                    }
                }

                if ( AspmD.bStrLimShear )
                {
                    if ( arXlout[i].mShear.Lookup(AlcsK, bChk) && bChk )
                    {
                        T_ASSC_PSC_BS_CASE AsscCaseD;
                        if ( m_pDoc->m_pPostCtrl->GetAnalysisResult()->GetShearStrengthRating4BD21(ElemK, AlcsK, 0, AsscCaseD) )
                        {
                            if ( AsscCaseD.AsscBase[k].ComD.bChk ) dRF[2] = AsscCaseD.AsscBase[k].dRF;
                        }
                        if ( abs(dRF[2]) > 0.0 ) dRFmin = min(dRFmin, dRF[2]);
                    }
                }

                if ( AspmD.bStrLimTorsion )
                {
                    if ( arXlout[i].mTorsion.Lookup(AlcsK, bChk) && bChk )
                    {
                        T_ASTC_PSC_BS_CASE AstcCaseD;
                        if ( m_pDoc->m_pPostCtrl->GetAnalysisResult()->GetTorsionStrengthRating4BD21(ElemK, AlcsK, 0, AstcCaseD) )
                        {
                            if ( AstcCaseD.AstcBase[k].ComD.bChk )
                            {
                                if ( AstcCaseD.AstcBase[k].bBox )
                                {
                                    dRF[3] = AstcCaseD.AstcBase[k].dRF;
                                }
                            }
                        }
                        if ( abs(dRF[3]) > 0.0 ) dRFmin = min(dRFmin, dRF[3]);
                    }
                }

                if ( AspmD.bSevLimStrsCrack )
                {
                    if ( arXlout[i].mSLS.Lookup(AlcsK, bChk) && bChk )
                    {
                        T_ASSL_PSC_BS_CASE AsslCaseD;
                        if ( m_pDoc->m_pPostCtrl->GetAnalysisResult()->GetSLSRating4BD21(ElemK, AlcsK, 1, 0, AsslCaseD) )
                        {
                            if ( AsslCaseD.AsslBase[k].ComD.bChk ) dRF[4] = AsslCaseD.AsslBase[k].dRF;
                        }
                        if ( m_pDoc->m_pPostCtrl->GetAnalysisResult()->GetSLSRating4BD21(ElemK, AlcsK, 2, 0, AsslCaseD) )
                        {
                            if ( AsslCaseD.AsslBase[k].ComD.bChk )
                            {
                                if ( AsslCaseD.AsslBase[k].enClass == BS_ASSESS_SLS_CLASS1 )
                                {
                                    if ( AsslCaseD.AsslBase[k].dsig_t > 0.0 )
                                    {
                                        if ( abs(dRF[4]) < cDGN_Zero ) dRF[4] = AsslCaseD.AsslBase[k].dRF;
                                        else                        dRF[4] = min(dRF[4], AsslCaseD.AsslBase[k].dRF);
                                    }
                                }
                                else
                                {
                                    if ( abs(dRF[4]) < cDGN_Zero ) dRF[4] = AsslCaseD.AsslBase[k].dRF;
                                    else                        dRF[4] = min(dRF[4], AsslCaseD.AsslBase[k].dRF);
                                }
                            }
                        }
                        if ( abs(dRF[4]) > 0.0 ) dRFmin = min(dRFmin, dRF[4]);
                    }
                }

                if ( dRFmin > (1.0/pow(cDGN_Zero, 10.0) - cDGN_Zero) ) continue;

                arData.Add(D_CELL(nIncreaseRow, 2, GetOutputFormat(ElemK.first, 0)));
                arData.Add(D_CELL(nIncreaseRow, 5, GetPartNameWithNode(ElemK.first, k)));
                arData.Add(D_CELL(nIncreaseRow, 7, AlcsD.strCaseName));
                for ( int l=0; l<5; ++l )
                {
                    if ( abs(dRF[l]) >  cDGN_Zero )
                        arData.Add(D_CELL(nIncreaseRow, 12+3*l, GetOutputFormat(dRF[l], 3), D_NUMBER_FORMAT_3));
                    else
                        arData.Add(D_CELL(nIncreaseRow, 12+3*l, _T("-")));
                }
                arData.Add(D_CELL(nIncreaseRow, 27, (dRFmin<1.0 ? _T("NG") : _T("OK")), _T(""), (dRFmin<1.0 ? _T("RED") : _T("BLUE"))));

                ++nIncreaseRow;
                ++nPosiRow;
            }
            arMergeRow[k].Add(nPosiRow);
        }
        //BeforeElemK = ElemK;
    }
    if ( nIncreaseRow==0 )
    {
        arData.Add(D_CELL(nIncreaseRow, 2, _T("-")));
        arData.Add(D_CELL(nIncreaseRow, 5, _T("-")));
        arData.Add(D_CELL(nIncreaseRow, 7, _T("-")));
        for ( int l=0; l<5; ++l )
        {
            arData.Add(D_CELL(nIncreaseRow, 12+3*l, _T("-")));
        }
        arData.Add(D_CELL(nIncreaseRow, 27, _T("-")));

        nIncreaseRow++;
    }

    // Write Data
    for ( int i=0; i<nIncreaseRow; ++i )
    {
        if ( i==(nIncreaseRow-1) )
        {
            RptBlock = _T("Summary_Table_End");
        }
        else
        {
            RptBlock = _T("Summary_Table_Body");
        }
        rowCnt = CopyRange(pExcel, RptBlock, stRow+i);
    }

    Print_Array_D_CELL(pExcel, arData, stRow);

    // Merge Cells
    if(pExcel->GetExcelType() == XLTYPE::TYPE_MSEXCEL)
    {
        // LibXL은 이렇게 merge하면 안되는 듯, merge가 2번 되서 그런것 같음.
        int nIrow = arMergeRow[0].GetSize();
        int nJrow = arMergeRow[1].GetSize();
        if(nIrow == nJrow)
        {
            for(int i=0; i<nIrow; ++i)
            {
                int nI = arMergeRow[0].GetAt(i);
                int nJ = arMergeRow[1].GetAt(i);
                int nRow = nI+nJ;
                if(nRow > 0)
                {
                    pExcel->SetMergeCell(stRow, 2, stRow+(nRow-1), 4);
                }
                if(nI > 0)
                {
                    pExcel->SetMergeCell(stRow, 5, stRow+(nI-1), 6);
                    stRow += nI;
                }
                if(nJ > 0)
                {
                    pExcel->SetMergeCell(stRow, 5, stRow+(nJ-1), 6);
                    stRow += nJ;
                }
            }
        }
    }
}

void CRating_BD21_XLOut::GenerateAssementSheet(IExcel* pExcel, const int& iPosi, const _T_BD21_XLOUT& XloutD)
{
    CString RptBlock;
    int stRow  = 1;
    int rowCnt = 0;

    // 1. Design Condition
    RptBlock = _T("Rating_Title_Dgn_Condition");
    rowCnt = CopyRange(pExcel, RptBlock, stRow);

    CString strCode = CDBLib::GetPscRatCodeName(m_pDataCtrl->Get_CivilCode());
    CString strPosi = GetPartNameWithNode(XloutD.ElemK.first, iPosi);

    CArray<D_CELL, D_CELL &> arData;
    arData.RemoveAll();
    arData.Add(D_CELL(1, 1, strCode));
    arData.Add(D_CELL(1, 8, GetOutputFormat(XloutD.ElemK.first, 0)));
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
    GenerateAssTorsionTable(pExcel, iPosi, XloutD, ++stRow);
    GenerateAssSLSTable(pExcel, iPosi, XloutD, ++stRow);
}

void CRating_BD21_XLOut::GenerateAssFlexTable(IExcel* pExcel, const int& iPosi, const _T_BD21_XLOUT& XloutD, int& stRow)
{
    CArray<T_ALCS_K, T_ALCS_K> arAlcs; arAlcs.RemoveAll();
    m_pDoc->m_pAttrCtrl2->GetAlcsKeyList(arAlcs);
    int nAlcs = arAlcs.GetSize();
    if ( nAlcs==0 ) ASSERT(0);

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

        for ( int j=0; j<12; ++j ) // concurrent
        {
            int iConCurrent = j+1;
            T_ASFC_PSC_BS_CASE AsfcCaseD;
            if ( !m_pDoc->m_pPostCtrl->GetAnalysisResult()->GetFlexuralStrengthRating4BD21(XloutD.ElemK, AlcsK, 0, iConCurrent, AsfcCaseD) ) continue;

            int k = AsfcCaseD.AsfcBase[iPosi].dSA > 0.0 ? 0 : 1; // Positive/Negative

            BOOL bAdd = FALSE;
            if ( k==0 && AsfcCaseD.AsfcBase[iPosi].dSA > 0.0 ) bAdd = TRUE;
            else if ( k==1 && AsfcCaseD.AsfcBase[iPosi].dSA <= 0.0 ) bAdd = TRUE;

            if ( bAdd == FALSE ) continue;

            // Set Data
            arData.Add(D_CELL(nIncreaseRow, 1, m_pDoc->m_pPostCtrl->GetRatingCaseNameBD21(AlcsK, iConCurrent)));
            arData.Add(D_CELL(nIncreaseRow, 5, (k==0 ? _T("Positive") : _T("Negative"))));
            if ( AsfcCaseD.AsfcBase[iPosi].ComD.bChk )
            {
                arData.Add(D_CELL(nIncreaseRow, 8, GetOutputFormat(AsfcCaseD.AsfcBase[iPosi].dRA *m_dFactorMoment, 3), D_NUMBER_FORMAT_3));
                arData.Add(D_CELL(nIncreaseRow, 11, GetOutputFormat(AsfcCaseD.AsfcBase[iPosi].dS  *m_dFactorMoment, 3), D_NUMBER_FORMAT_3));
                arData.Add(D_CELL(nIncreaseRow, 14, GetOutputFormat(AsfcCaseD.AsfcBase[iPosi].dSd *m_dFactorMoment, 3), D_NUMBER_FORMAT_3));
                arData.Add(D_CELL(nIncreaseRow, 20, GetOutputFormat(AsfcCaseD.AsfcBase[iPosi].dSA *m_dFactorMoment, 3), D_NUMBER_FORMAT_3));
                arData.Add(D_CELL(nIncreaseRow, 23, GetOutputFormatWithInfinity(AsfcCaseD.AsfcBase[iPosi].dA, 3, cMaxRF, FALSE), D_NUMBER_FORMAT_3));

                // CIVIL-1063 khj0102
                if ( AlcsD.nMovType == 1 ) //Special인 경우
                {
                    arData.Add(D_CELL(nIncreaseRow, 17, _T("-")));
                    arData.Add(D_CELL(nIncreaseRow, 25, _T("-")));
                    if ( abs(AsfcCaseD.AsfcBase[iPosi].dPsi_ast) - cDGN_Zero < 0.0 )
                    {
                        arData.Add(D_CELL(nIncreaseRow, 27, _T("-")));
                    }
                    else if ( AsfcCaseD.AsfcBase[iPosi].dPsi_ast + cDGN_Zero > cMaxRF )
                    {
                        arData.Add(D_CELL(nIncreaseRow, 27, _T("-")));
                    }
                    else
                    {
                        arData.Add(D_CELL(nIncreaseRow, 27, GetOutputFormatWithInfinity(AsfcCaseD.AsfcBase[iPosi].dPsi_ast, 3, cMaxRF, FALSE), D_NUMBER_FORMAT_3));
                    }
                }
                else //Combined인 경우
                {
                    arData.Add(D_CELL(nIncreaseRow, 17, GetOutputFormat(AsfcCaseD.AsfcBase[iPosi].dSst*m_dFactorMoment, 3), D_NUMBER_FORMAT_3));
                    if ( abs(AsfcCaseD.AsfcBase[iPosi].dPsi) - cDGN_Zero < 0.0 )
                    {
                        arData.Add(D_CELL(nIncreaseRow, 25, _T("-")));
                    }
                    else if ( AsfcCaseD.AsfcBase[iPosi].dPsi + cDGN_Zero > cMaxRF )
                    {
                        arData.Add(D_CELL(nIncreaseRow, 25, _T("-")));
                    }
                    else
                    {
                        arData.Add(D_CELL(nIncreaseRow, 25, GetOutputFormatWithInfinity(AsfcCaseD.AsfcBase[iPosi].dPsi, 3, cMaxRF, FALSE), D_NUMBER_FORMAT_3));
                    }

                    if ( abs(AsfcCaseD.AsfcBase[iPosi].dPsi_ast) - cDGN_Zero < 0.0 )
                    {
                        arData.Add(D_CELL(nIncreaseRow, 27, _T("-")));
                    }
                    else if ( AsfcCaseD.AsfcBase[iPosi].dPsi_ast + cDGN_Zero > cMaxRF )
                    {
                        arData.Add(D_CELL(nIncreaseRow, 27, _T("-")));
                    }
                    else
                    {
                        arData.Add(D_CELL(nIncreaseRow, 27, GetOutputFormatWithInfinity(AsfcCaseD.AsfcBase[iPosi].dPsi_ast, 3, cMaxRF, FALSE), D_NUMBER_FORMAT_3));
                    }
                }
                arData.Add(D_CELL(nIncreaseRow, 29, (AsfcCaseD.AsfcBase[iPosi].dRF<1.0 ? _T("NG") : _T("OK")), _T(""), 
                    (AsfcCaseD.AsfcBase[iPosi].dRF<1.0 ? _T("RED") : _T("BLUE"))));
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

void CRating_BD21_XLOut::GenerateAssShearTable(IExcel* pExcel, const int& iPosi, const _T_BD21_XLOUT& XloutD, int& stRow)
{
    CArray<T_ALCS_K, T_ALCS_K> arAlcs; arAlcs.RemoveAll();
    m_pDoc->m_pAttrCtrl2->GetAlcsKeyList(arAlcs);
    int nAlcs = arAlcs.GetSize();
    if ( nAlcs==0 ) ASSERT(0);

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

        for ( int j=0; j<12; ++j ) // concurrent
        {
            int iConCurrent = j+1;
            T_ASSC_PSC_BS_CASE AsscCaseD;
            if ( !m_pDoc->m_pPostCtrl->GetAnalysisResult()->GetShearStrengthRating4BD21(XloutD.ElemK, AlcsK, iConCurrent, AsscCaseD) ) continue;

            // Set Data
            arData.Add(D_CELL(nIncreaseRow, 1, m_pDoc->m_pPostCtrl->GetRatingCaseNameBD21(AlcsK, iConCurrent)));
            if ( AsscCaseD.AsscBase[iPosi].ComD.bChk )
            {
                arData.Add(D_CELL(nIncreaseRow, 5, GetOutputFormat(AsscCaseD.AsscBase[iPosi].dRA *m_dFactorForce, 3), D_NUMBER_FORMAT_3));

                arData.Add(D_CELL(nIncreaseRow, 8, GetOutputFormat(AsscCaseD.AsscBase[iPosi].dS  *m_dFactorForce, 3), D_NUMBER_FORMAT_3));

                arData.Add(D_CELL(nIncreaseRow, 11, GetOutputFormat(AsscCaseD.AsscBase[iPosi].dSd *m_dFactorForce, 3), D_NUMBER_FORMAT_3));

                arData.Add(D_CELL(nIncreaseRow, 17, GetOutputFormat(AsscCaseD.AsscBase[iPosi].dSA *m_dFactorForce, 3), D_NUMBER_FORMAT_3));
                arData.Add(D_CELL(nIncreaseRow, 20, GetOutputFormatWithInfinity(AsscCaseD.AsscBase[iPosi].dA, 3, cMaxRF, FALSE), D_NUMBER_FORMAT_3));

                // CIVIL-1063 khj0102
                if ( AlcsD.nMovType == 1 ) //Special인 경우
                {
                    arData.Add(D_CELL(nIncreaseRow, 14, _T("-")));
                    arData.Add(D_CELL(nIncreaseRow, 22, _T("-")));
                    if ( abs(AsscCaseD.AsscBase[iPosi].dPsi_ast) - cDGN_Zero < 0.0 )
                    {
                        arData.Add(D_CELL(nIncreaseRow, 24, _T("-")));
                    }
                    else if ( AsscCaseD.AsscBase[iPosi].dPsi_ast + cDGN_Zero > cMaxRF )
                    {
                        arData.Add(D_CELL(nIncreaseRow, 24, _T("-")));
                    }
                    else
                    {
                        arData.Add(D_CELL(nIncreaseRow, 24, GetOutputFormatWithInfinity(AsscCaseD.AsscBase[iPosi].dPsi_ast, 3, cMaxRF, FALSE), D_NUMBER_FORMAT_3));
                    }
                }
                else //Combined인 경우
                {
                    arData.Add(D_CELL(nIncreaseRow, 14, GetOutputFormat(AsscCaseD.AsscBase[iPosi].dSst*m_dFactorForce, 3), D_NUMBER_FORMAT_3));
                    if ( abs(AsscCaseD.AsscBase[iPosi].dPsi) - cDGN_Zero < 0.0 )
                    {
                        arData.Add(D_CELL(nIncreaseRow, 22, _T("-")));
                    }
                    else if ( AsscCaseD.AsscBase[iPosi].dPsi + cDGN_Zero > cMaxRF )
                    {
                        arData.Add(D_CELL(nIncreaseRow, 22, _T("-")));
                    }
                    else
                    {
                        arData.Add(D_CELL(nIncreaseRow, 22, GetOutputFormatWithInfinity(AsscCaseD.AsscBase[iPosi].dPsi, 3, cMaxRF, FALSE), D_NUMBER_FORMAT_3));
                    }

                    if ( abs(AsscCaseD.AsscBase[iPosi].dPsi_ast) - cDGN_Zero < 0.0 )
                    {
                        arData.Add(D_CELL(nIncreaseRow, 24, _T("-")));
                    }
                    else if ( AsscCaseD.AsscBase[iPosi].dPsi_ast + cDGN_Zero > cMaxRF )
                    {
                        arData.Add(D_CELL(nIncreaseRow, 24, _T("-")));
                    }
                    else
                    {
                        arData.Add(D_CELL(nIncreaseRow, 24, GetOutputFormatWithInfinity(AsscCaseD.AsscBase[iPosi].dPsi_ast, 3, cMaxRF, FALSE), D_NUMBER_FORMAT_3));
                    }
                }
                arData.Add(D_CELL(nIncreaseRow, 26, (AsscCaseD.AsscBase[iPosi].dRF<1.0 ? _T("NG") : _T("OK")), _T(""), 
                    (AsscCaseD.AsscBase[iPosi].dRF<1.0 ? _T("RED") : _T("BLUE"))));
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

void CRating_BD21_XLOut::GenerateAssTorsionTable(IExcel* pExcel, const int& iPosi, const _T_BD21_XLOUT& XloutD, int& stRow)
{
    CArray<T_ALCS_K, T_ALCS_K> arAlcs; arAlcs.RemoveAll();
    m_pDoc->m_pAttrCtrl2->GetAlcsKeyList(arAlcs);
    int nAlcs = arAlcs.GetSize();
    if ( nAlcs==0 ) ASSERT(0);

    int nIncreaseRow = 0;
    // Make Data
    CArray<D_CELL, D_CELL &> arData;
    arData.RemoveAll();
    for ( int i=0; i<nAlcs; ++i )
    {
        T_ALCS_K AlcsK = arAlcs[i];
        BOOL bChk = FALSE;
        XloutD.mTorsion.Lookup(AlcsK, bChk);
        if ( !bChk ) continue;

        for ( int j=0; j<12; ++j ) // concurrent
        {
            int iConCurrent = j+1;
            T_ASTC_PSC_BS_CASE AstcCaseD;
            if ( !m_pDoc->m_pPostCtrl->GetAnalysisResult()->GetTorsionStrengthRating4BD21(XloutD.ElemK, AlcsK, iConCurrent, AstcCaseD) ) continue;

            // Set Data
            arData.Add(D_CELL(nIncreaseRow, 1, m_pDoc->m_pPostCtrl->GetRatingCaseNameBD21(AlcsK, iConCurrent)));
            if ( AstcCaseD.AstcBase[iPosi].ComD.bChk )
            {
                if ( !AstcCaseD.AstcBase[iPosi].bBox )
                {
                    arData.Add(D_CELL(nIncreaseRow, 5, _T("-")));
                    arData.Add(D_CELL(nIncreaseRow, 8, _T("-")));
                    arData.Add(D_CELL(nIncreaseRow, 11, GetOutputFormat(AstcCaseD.AstcBase[iPosi].dv, 3), D_NUMBER_FORMAT_3));
                    arData.Add(D_CELL(nIncreaseRow, 14, _T("-")));
                    arData.Add(D_CELL(nIncreaseRow, 17, _T("-")));
                    arData.Add(D_CELL(nIncreaseRow, 20, _T("-")));
                    arData.Add(D_CELL(nIncreaseRow, 23, _T("-")));
                    arData.Add(D_CELL(nIncreaseRow, 26, _T("-")));
                    arData.Add(D_CELL(nIncreaseRow, 29, _T("-")));
                    arData.Add(D_CELL(nIncreaseRow, 31, _T("-")));
                }
                else
                {
                    arData.Add(D_CELL(nIncreaseRow, 5, GetOutputFormat(AstcCaseD.AstcBase[iPosi].dvt, 3), D_NUMBER_FORMAT_3));
                    arData.Add(D_CELL(nIncreaseRow, 8, GetOutputFormat(AstcCaseD.AstcBase[iPosi].dvtmin, 3), D_NUMBER_FORMAT_3));
                    arData.Add(D_CELL(nIncreaseRow, 17, GetOutputFormat(AstcCaseD.AstcBase[iPosi].dy1, 3), D_NUMBER_FORMAT_3));

                    if ( AstcCaseD.AstcBase[iPosi].dvt > AstcCaseD.AstcBase[iPosi].dvtmin )
                    {
                        arData.Add(D_CELL(nIncreaseRow, 11, GetOutputFormat(AstcCaseD.AstcBase[iPosi].dv, 3), D_NUMBER_FORMAT_3));
                        arData.Add(D_CELL(nIncreaseRow, 14, GetOutputFormat(AstcCaseD.AstcBase[iPosi].dvtu1, 3), D_NUMBER_FORMAT_3));
                        arData.Add(D_CELL(nIncreaseRow, 23, GetOutputFormat(AstcCaseD.AstcBase[iPosi].dT * m_dFactorMoment, 3), D_NUMBER_FORMAT_3));
                        arData.Add(D_CELL(nIncreaseRow, 26, GetOutputFormat(AstcCaseD.AstcBase[iPosi].dTu* m_dFactorMoment, 3), D_NUMBER_FORMAT_3));
                    }
                    else
                    {
                        arData.Add(D_CELL(nIncreaseRow, 11, _T("-")));
                        arData.Add(D_CELL(nIncreaseRow, 14, _T("-")));
                        arData.Add(D_CELL(nIncreaseRow, 23, _T("-")));
                        arData.Add(D_CELL(nIncreaseRow, 26, _T("-")));
                    }

                    if ( AstcCaseD.AstcBase[iPosi].dy1 < 550.0 )
                    {
                        arData.Add(D_CELL(nIncreaseRow, 20, GetOutputFormat(AstcCaseD.AstcBase[iPosi].dvtu2, 3), D_NUMBER_FORMAT_3));
                    }
                    else
                    {
                        arData.Add(D_CELL(nIncreaseRow, 20, _T("-")));
                    }

                    arData.Add(D_CELL(nIncreaseRow, 29, GetOutputFormat(AstcCaseD.AstcBase[iPosi].dRF, 3), D_NUMBER_FORMAT_3));
                    arData.Add(D_CELL(nIncreaseRow, 31, (AstcCaseD.AstcBase[iPosi].dRF<1.0 ? _T("NG") : _T("OK")), _T(""), (AstcCaseD.AstcBase[iPosi].dRF<1.0 ? _T("RED") : _T("BLUE"))));
                }
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
        arData.Add(D_CELL(nIncreaseRow, 31, _T("-")));

        nIncreaseRow++;
    }

    // Write Data
    CString RptBlock;
    int rowCnt = 0;

    // Table Title
    RptBlock = _T("Rating_Title_Torsion");
    rowCnt = CopyRange(pExcel, RptBlock, stRow);
    stRow += rowCnt;
    // Table Head
    RptBlock = _T("Rating_Tor_Table_Head_Renew");
    rowCnt = CopyRange(pExcel, RptBlock, stRow);
    stRow += rowCnt;

    for ( int i=0; i<nIncreaseRow; ++i )
    {
        if ( i==(nIncreaseRow-1) )
        {
            RptBlock = _T("Rating_Tor_Table_End_Renew");
        }
        else
        {
            RptBlock = _T("Rating_Tor_Table_Body_Renew");
        }
        rowCnt = CopyRange(pExcel, RptBlock, stRow+i);
    }

    Print_Array_D_CELL(pExcel, arData, stRow);
    stRow += nIncreaseRow;
}

void CRating_BD21_XLOut::GenerateAssSLSTable(IExcel* pExcel, const int& iPosi, const _T_BD21_XLOUT& XloutD, int& stRow)
{
    CArray<T_ALCS_K, T_ALCS_K> arAlcs; arAlcs.RemoveAll();
    m_pDoc->m_pAttrCtrl2->GetAlcsKeyList(arAlcs);
    int nAlcs = arAlcs.GetSize();
    if ( nAlcs==0 ) ASSERT(0);

    int nIncreaseRow = 0;
    // Make Data
    CArray<D_CELL, D_CELL &> arData;
    arData.RemoveAll();
    for ( int i=0; i<nAlcs; ++i )
    {
        T_ALCS_K AlcsK = arAlcs[i];
        BOOL bChk = FALSE;
        XloutD.mSLS.Lookup(AlcsK, bChk);
        if ( !bChk ) continue;

        for ( int j=0; j<12; ++j ) // concurrent
        {
            int iConCurrent = j+1;
            T_ASSL_PSC_BS_CASE AsslCaseD;
            if ( !m_pDoc->m_pPostCtrl->GetAnalysisResult()->GetSLSRating4BD21(XloutD.ElemK, AlcsK, 0, iConCurrent, AsslCaseD) ) continue;

            int k = AsslCaseD.AsslBase[iPosi].dSA > 0.0 ? 0 : 1;// Positive/Negative
            // Set Data
            arData.Add(D_CELL(nIncreaseRow, 3, m_pDoc->m_pPostCtrl->GetRatingCaseNameBD21(AlcsK, iConCurrent)));
            arData.Add(D_CELL(nIncreaseRow, 7, (k==0 ? _T("Positive") : _T("Negative"))));

            const auto AsslBase = AsslCaseD.AsslBase[iPosi];

            if ( AsslBase.ComD.bChk )
            {
                arData.Add(D_CELL(nIncreaseRow, 1, GetOutputFormat(static_cast<int>(AsslBase.enClass), 0)));

                switch ( AsslBase.enClass )
                {
                case BS_ASSESS_SLS_CLASS1:
                    {
                        arData.Add(D_CELL(nIncreaseRow, 10, GetOutputFormat(AsslBase.dsig_c, 3), D_NUMBER_FORMAT_3));
                        arData.Add(D_CELL(nIncreaseRow, 13, GetOutputFormat(AsslBase.dsig_c_lim, 3), D_NUMBER_FORMAT_3));
                        arData.Add(D_CELL(nIncreaseRow, 16, GetOutputFormat(AsslBase.dsig_t, 3), D_NUMBER_FORMAT_3));
                        arData.Add(D_CELL(nIncreaseRow, 19, _T("0.0")));
                        arData.Add(D_CELL(nIncreaseRow, 22, _T("-")));
                        arData.Add(D_CELL(nIncreaseRow, 25, _T("-")));
                        if ( AsslBase.dsig_t < 0.0 )
                        {
                            arData.Add(D_CELL(nIncreaseRow, 30, _T("NG"), _T(""), _T("RED")));
                        }
                        else
                        {
                            arData.Add(D_CELL(nIncreaseRow, 30, (AsslBase.dRF<1.0 ? _T("NG") : _T("OK")), _T(""), (AsslBase.dRF<1.0 ? _T("RED") : _T("BLUE"))));
                        }
                    }
                    break;
                case BS_ASSESS_SLS_CLASS2:
                    {
                        arData.Add(D_CELL(nIncreaseRow, 10, GetOutputFormat(AsslBase.dsig_c, 3), D_NUMBER_FORMAT_3));
                        arData.Add(D_CELL(nIncreaseRow, 13, GetOutputFormat(AsslBase.dsig_c_lim, 3), D_NUMBER_FORMAT_3));
                        arData.Add(D_CELL(nIncreaseRow, 16, GetOutputFormat(AsslBase.dsig_t, 3), D_NUMBER_FORMAT_3));
                        arData.Add(D_CELL(nIncreaseRow, 19, GetOutputFormat(AsslBase.dsig_t_lim, 3), D_NUMBER_FORMAT_3));
                        arData.Add(D_CELL(nIncreaseRow, 22, _T("-")));
                        arData.Add(D_CELL(nIncreaseRow, 25, _T("-")));
                        arData.Add(D_CELL(nIncreaseRow, 30, (AsslBase.dRF<1.0 ? _T("NG") : _T("OK")), _T(""), (AsslBase.dRF<1.0 ? _T("RED") : _T("BLUE"))));
                    }
                    break;
                case BS_ASSESS_SLS_CLASS3:
                    if ( AsslBase.bBonded )
                    {
                        arData.Add(D_CELL(nIncreaseRow, 10, GetOutputFormat(AsslBase.dsig_c, 3), D_NUMBER_FORMAT_3));
                        arData.Add(D_CELL(nIncreaseRow, 13, GetOutputFormat(AsslBase.dsig_c_lim, 3), D_NUMBER_FORMAT_3));
                        arData.Add(D_CELL(nIncreaseRow, 16, GetOutputFormat(AsslBase.dsig_t, 3), D_NUMBER_FORMAT_3));
                        arData.Add(D_CELL(nIncreaseRow, 19, GetOutputFormat(AsslBase.dsig_t_lim, 3), D_NUMBER_FORMAT_3));
                        arData.Add(D_CELL(nIncreaseRow, 22, _T("-")));
                        arData.Add(D_CELL(nIncreaseRow, 25, _T("-")));
                        arData.Add(D_CELL(nIncreaseRow, 30, (AsslBase.dRF<1.0 ? _T("NG") : _T("OK")), _T(""), (AsslBase.dRF<1.0 ? _T("RED") : _T("BLUE"))));
                    }
                    else
                    {
                        arData.Add(D_CELL(nIncreaseRow, 10, _T("-")));
                        arData.Add(D_CELL(nIncreaseRow, 13, _T("-")));
                        arData.Add(D_CELL(nIncreaseRow, 16, _T("-")));
                        arData.Add(D_CELL(nIncreaseRow, 19, _T("-")));
                        arData.Add(D_CELL(nIncreaseRow, 22, GetOutputFormat(AsslBase.dw, 3), D_NUMBER_FORMAT_3));
                        arData.Add(D_CELL(nIncreaseRow, 25, GetOutputFormat(AsslBase.dw_lim, 3), D_NUMBER_FORMAT_3));
                        arData.Add(D_CELL(nIncreaseRow, 30, (AsslBase.dRF<1.0 ? _T("NG") : _T("OK")), _T(""), (AsslBase.dRF<1.0 ? _T("RED") : _T("BLUE"))));
                    }
                    break;
                default:
                    break;
                }

                arData.Add(D_CELL(nIncreaseRow, 28, GetOutputFormatWithInfinity(AsslBase.dRF, 3, cMaxRF, FALSE), D_NUMBER_FORMAT_3));
            }
            else
            {
                arData.Add(D_CELL(nIncreaseRow, 1, _T("-")));
                arData.Add(D_CELL(nIncreaseRow, 10, _T("-")));
                arData.Add(D_CELL(nIncreaseRow, 13, _T("-")));
                arData.Add(D_CELL(nIncreaseRow, 16, _T("-")));
                arData.Add(D_CELL(nIncreaseRow, 19, _T("-")));
                arData.Add(D_CELL(nIncreaseRow, 22, _T("-")));
                arData.Add(D_CELL(nIncreaseRow, 25, _T("-")));
                arData.Add(D_CELL(nIncreaseRow, 28, _T("-")));
                arData.Add(D_CELL(nIncreaseRow, 30, _T("-")));
            }

            nIncreaseRow++;
        }
    }
    if ( nIncreaseRow==0 )
    {
        arData.Add(D_CELL(nIncreaseRow, 1, _T("-")));
        arData.Add(D_CELL(nIncreaseRow, 3, _T("-")));
        arData.Add(D_CELL(nIncreaseRow, 7, _T("-")));
        arData.Add(D_CELL(nIncreaseRow, 10, _T("-")));
        arData.Add(D_CELL(nIncreaseRow, 13, _T("-")));
        arData.Add(D_CELL(nIncreaseRow, 16, _T("-")));
        arData.Add(D_CELL(nIncreaseRow, 19, _T("-")));
        arData.Add(D_CELL(nIncreaseRow, 22, _T("-")));
        arData.Add(D_CELL(nIncreaseRow, 25, _T("-")));
        arData.Add(D_CELL(nIncreaseRow, 28, _T("-")));
        arData.Add(D_CELL(nIncreaseRow, 30, _T("-")));

        nIncreaseRow++;
    }

    // Write Data
    CString RptBlock;
    int rowCnt = 0;

    // Table Title
    RptBlock = _T("Rating_Title_SLS");
    rowCnt = CopyRange(pExcel, RptBlock, stRow);
    stRow += rowCnt;
    // Table Head
    RptBlock = _T("Rating_SLS_Table_Head");
    rowCnt = CopyRange(pExcel, RptBlock, stRow);
    stRow += rowCnt;

    for ( int i=0; i<nIncreaseRow; ++i )
    {
        if ( i==(nIncreaseRow-1) )
        {
            RptBlock = _T("Rating_SLS_Table_End");
        }
        else
        {
            RptBlock = _T("Rating_SLS_Table_Body");
        }
        rowCnt = CopyRange(pExcel, RptBlock, stRow+i);
    }

    Print_Array_D_CELL(pExcel, arData, stRow);
    stRow += nIncreaseRow;
}

void CRating_BD21_XLOut::SetSheetPage(IExcel* pExcel, CString strNameCurrent)
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

void CRating_BD21_XLOut::SetSummarySheetPage(IExcel* pExcel)
{
    m_strBaseSheet = _T("Summary(Base)");
    SetSheetPage(pExcel, _T("Summary"));
}

void CRating_BD21_XLOut::SetDetailSheetPage(IExcel* pExcel, const ElemPairK ElemK, const int& iPosi)
{
    m_strBaseSheet = _T("Detail(Base)");
    CString strSheetName;
    strSheetName.Format(_T("Detail;%d(%s)"), ElemK, (iPosi==0 ? _T("I") : _T("J")));
    SetSheetPage(pExcel, strSheetName);
}

void CRating_BD21_XLOut::SetAssessmentSheetPage(IExcel* pExcel, const ElemPairK ElemK, const int& iPosi)
{
    m_strBaseSheet = _T("Assessment(Base)");
    CString strSheetName;
    strSheetName.Format(_T("Assessment;%d(%s)"), ElemK, (iPosi==0 ? _T("I") : _T("J")));
    SetSheetPage(pExcel, strSheetName);
}

void CRating_BD21_XLOut::DelBaseSheetPage(IExcel* pExcel)
{
    pExcel->DeleteSheet(_T("Summary(Base)"));
    pExcel->DeleteSheet(_T("Detail(Base)"));
    pExcel->DeleteSheet(_T("Assessment(Base)"));
    pExcel->DeleteSheet(_T("ConcreteSLS(Base)"));
    pExcel->DeleteSheet(_T("ConcreteCS(Base)"));
    pExcel->DeleteSheet(_T("PrincipalSLS(Base)"));
    pExcel->DeleteSheet(_T("PrincipalCS(Base)"));
    pExcel->DeleteSheet(_T("Tendon(Base)"));
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CRating_BD21_XLOut::GenerateDetailRpt(IExcel* pExcel, CString szProgramDir, CString szProjectFileDir, CString strPath, D_XL_PROGRESS_INTERFACE* pProgDlg)
{
    CArray<T_RKPO_K, T_RKPO_K> aPrintElemKey;
    m_pDoc->m_pAttrCtrl->GetRkpoKeyList(aPrintElemKey);

    CArray<T_RKPV_K, T_RKPV_K> aPrintVBeamKey;
    m_pDoc->m_pAttrCtrl->GetRkpvKeyList(aPrintVBeamKey);

    ArrElemPairKey aPrintElemPairK;
    CDBLib::ConvertToElemPairKeyList(aPrintElemKey, aPrintVBeamKey, aPrintElemPairK);

    PSC_RPT_BS_D PsdRptD;

    GetDetailResult(aPrintElemPairK, PsdRptD);

    CDgn_PSCRptManager_EC2 RptMgr;

    int nDgnCode = CS_455_PSC; //BD_44_15_PSC;
    RptMgr.Print_DetailReport_BS(pExcel, pProgDlg, nDgnCode, szProgramDir, szProjectFileDir, strPath, PsdRptD);

    return TRUE;
}

BOOL CRating_BD21_XLOut::GetDetailResult(const ArrElemPairKey& aPrtElemK, PSC_RPT_BS_D &rData)
{
    for ( INT_PTR i=0; i<aPrtElemK.GetSize(); ++i )
    {
        auto  ElemK = aPrtElemK[i];

        // 출력 옵션 셋팅
        // 1. 출력 여부 결정
        T_ASOP_D AsopD; AsopD.Initialize();  // 설계 위치
        T_RKPO_D RkpoD; RkpoD.Initialize();  // 출력 위치
        if (ElemK.second == EN_EL_BEAM)
        {
            if (!m_pDoc->m_pAttrCtrl->GetAsop(ElemK.first, AsopD)) continue;
            if (!m_pDoc->m_pAttrCtrl->GetRkpo(ElemK.first, RkpoD)) continue;
        }
        else if (ElemK.second == EN_EL_VBEAM)
        {
            ASSERT(0); // Asov ?
            if (!m_pDoc->m_pAttrCtrl->GetAsop(ElemK.first, AsopD)) continue;
            if (!m_pDoc->m_pAttrCtrl->GetRkpv(ElemK.first, RkpoD)) continue;
        }
        else ASSERT(0);

        BOOL bComposite = m_pDataCtrl->IsCompositePSCSect(ElemK);

        BOOL bPrtIJ[2] = { FALSE, FALSE };
        bPrtIJ[0] = ((AsopD.nPos == 0 || AsopD.nPos == 2) && (RkpoD.iPrintChk == 0 || RkpoD.iPrintChk == 2));
        bPrtIJ[1] = ((AsopD.nPos == 1 || AsopD.nPos == 2) && (RkpoD.iPrintChk == 1 || RkpoD.iPrintChk == 2));

        PSC_DATA_MEMB PscMembD;
        for ( int j=0; j<2; ++j )
        {
            PscMembD.Pos[j].CalcD.bTypeC = AsopD.bTypeC;
            m_pDataCtrl->m_pPscDataCtrl->Get_PscDataPosi(ElemK, j, PscMembD.Pos[j], bComposite);
        }

        for ( int j=0; j<2; ++j )
        {
            if ( bPrtIJ[j] )
            {
                PSC_RPT_BS_POS RptResD;
                RptResD.Pos = PscMembD.Pos[j];
                for ( int k=0; k<2; ++k )
                {
                    int nPosNeg = k+1;
                    GetDetailFlex(ElemK, PscMembD, j, nPosNeg, RptResD);
                }
                GetDetailShrr(ElemK, PscMembD, j, RptResD);
                GetDetailTorr(ElemK, PscMembD, j, RptResD);
                GetDetailStrs(ElemK, PscMembD, j, RptResD);
                GetDetailCrck(ElemK, PscMembD, j, RptResD);

                rData.aRptD.Add(RptResD);
            }
        }
    }
    return TRUE;
}

BOOL CRating_BD21_XLOut::GetDetailFlex(ElemPairK ElemK, PSC_DATA_MEMB& PscMembD, const int& nPos, int nPosNeg, PSC_RPT_BS_POS &rData)
{
    if ( nPos<0 || nPos>1 ) return FALSE;
    if ( nPosNeg<1 || nPosNeg>2 ) return FALSE;

    T_ASFC_PSC_BS_CASE AsfcCase;
    m_pDoc->m_pPostCtrl->GetAnalysisResult()->GetFlexuralStrengthRating4BD21(ElemK, 0, nPosNeg, 0, AsfcCase);

    const T_ASFC_PSC_BS_BASE& AsfcB = AsfcCase.AsfcBase[nPos];

    T_BDCR_PSC_BS_BASE BdcrB;
    BdcrB.bCHK = AsfcB.ComD.bChk;
    BdcrB.LcomK= AsfcB.ComD.RCaseK;
    BdcrB.nMax = AsfcB.ComD.unConcurrent;
    BdcrB.dMy  = AsfcB.dSA;

    T_ASPM_D AspmD;
    AspmD.Initialize();
    m_pAttrCtrl->GetAspm(AspmD);

    if ( !AspmD.bStrLimFlexure )
    {
        BdcrB.bCHK = FALSE;
    }

    int nRptPosNeg = nPosNeg-1;
    m_pDataCtrl->m_pPscDataCtrl->Get_BdcrReportData_CS454(ElemK, nPos, nRptPosNeg, BdcrB, PscMembD.Pos[nPos], rData.Fle[nRptPosNeg]);

    return TRUE;
}

BOOL CRating_BD21_XLOut::GetDetailShrr(ElemPairK ElemK, PSC_DATA_MEMB& PscMembD, const int& nPos, PSC_RPT_BS_POS &rData)
{
    if ( nPos<0 || nPos>1 ) return FALSE;

    T_ASSC_PSC_BS_CASE AsscCaseD;
    m_pDoc->m_pPostCtrl->GetAnalysisResult()->GetShearStrengthRating4BD21(ElemK, 0, 0, AsscCaseD);

	CCRCForceCtrl ForceCtrl;
	double dForceTpI[6] = { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 };
	double dForceTpJ[6] = { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 };

	ForceCtrl.Get_ForceByTnPr(ElemK, dForceTpI, dForceTpJ);
	
	const T_ASSC_PSC_BS_BASE& AsscB = AsscCaseD.AsscBase[nPos];

    T_SHRR_PSC_BS_BASE ShrrB;
    ShrrB.bCHK = AsscB.ComD.bChk;
    ShrrB.LcomK= AsscB.ComD.RCaseK;
    ShrrB.nMax = AsscB.ComD.unConcurrent;
    ShrrB.dFx  = AsscB.ComD.dFxx;
    ShrrB.dMy  = AsscB.ComD.dMuy;
    ShrrB.dVz  = AsscB.ComD.dFzz;
    ShrrB.dT   = AsscB.ComD.dMux;
	for ( int i = 0; i < 6; ++i ) ShrrB.dForceTP[i] = nPos == 0 ? fabs(dForceTpI[i]) : fabs(dForceTpJ[i]);

    T_ASPM_D AspmD;
    AspmD.Initialize();
    m_pAttrCtrl->GetAspm(AspmD);

    if ( !AspmD.bStrLimShear )
    {
        ShrrB.bCHK = FALSE;
    }

    m_pDataCtrl->m_pPscDataCtrl->Get_CumsReportData_CS454(ElemK, nPos, 0, ShrrB, PscMembD.Pos[nPos], rData.Shr);

    return TRUE;
}

BOOL CRating_BD21_XLOut::GetDetailTorr(ElemPairK ElemK, PSC_DATA_MEMB& PscMembD, const int& nPos, PSC_RPT_BS_POS &rData)
{
    if ( nPos<0 || nPos>1 ) return FALSE;

    T_ASTC_PSC_BS_CASE AstcCaseD;
    m_pDoc->m_pPostCtrl->GetAnalysisResult()->GetTorsionStrengthRating4BD21(ElemK, 0, 0, AstcCaseD);

    const T_ASTC_PSC_BS_BASE& AstcB = AstcCaseD.AstcBase[nPos];

    T_TORR_PSC_BS_BASE TorrB;
    TorrB.bCHK = AstcB.ComD.bChk;
    TorrB.LcomK= AstcB.ComD.RCaseK;
    TorrB.nMax = AstcB.ComD.unConcurrent;
    TorrB.dFx  = AstcB.ComD.dFxx;
    TorrB.dMy  = AstcB.ComD.dMuy;
    TorrB.dVz  = AstcB.ComD.dFzz;
    TorrB.dT   = AstcB.ComD.dMux;

    T_ASPM_D AspmD;
    AspmD.Initialize();
    m_pAttrCtrl->GetAspm(AspmD);

    if ( !AspmD.bStrLimTorsion )
    {
        TorrB.bCHK = FALSE;
    }

    m_pDataCtrl->m_pPscDataCtrl->Get_CrmtReportData_CS454(ElemK, nPos, 0, TorrB, PscMembD.Pos[nPos], rData.Tor);

    return TRUE;
}

BOOL CRating_BD21_XLOut::GetDetailStrs(ElemPairK ElemK, PSC_DATA_MEMB& PscMembD, const int& nPos, PSC_RPT_BS_POS &rData)
{
    if ( nPos<0 || nPos>1 ) return FALSE;

    T_ASSL_PSC_BS_CASE AsslCase;
    if ( !m_pDoc->m_pPostCtrl->GetAnalysisResult()->GetSLSRating4BD21(ElemK, 0, 1, 0, AsslCase) ) AsslCase.Initialize();
    T_ASSL_PSC_BS_BASE PAsslB = AsslCase.AsslBase[nPos];
    if ( !m_pDoc->m_pPostCtrl->GetAnalysisResult()->GetSLSRating4BD21(ElemK, 0, 2, 0, AsslCase) ) AsslCase.Initialize();
    T_ASSL_PSC_BS_BASE NAsslB = AsslCase.AsslBase[nPos];

    for ( int i=0; i<2; ++i )
    {
        double dRatP = i==0 ? (DgnAbsDivide(PAsslB.dsig_c_lim, PAsslB.dsig_c)) : (DgnAbsDivide(PAsslB.dsig_t_lim, PAsslB.dsig_t));
        double dRatN = i==0 ? (DgnAbsDivide(NAsslB.dsig_c_lim, NAsslB.dsig_c)) : (DgnAbsDivide(NAsslB.dsig_t_lim, NAsslB.dsig_t));

        T_FYCM_PSC_BS_BASE FycmB;
        FycmB.Initialize();
        if ( dRatN > dRatP )
        {
            FycmB.bCHK  = PAsslB.ComD.bChk;
            FycmB.LcomK = PAsslB.ComD.RCaseK;
            FycmB.nMax  = PAsslB.ComD.unConcurrent;
            FycmB.dFT   = PAsslB.ComD.dStr[0];
            FycmB.dFB   = PAsslB.ComD.dStr[1];
            FycmB.dFMAX = i==0 ? PAsslB.dsig_c : PAsslB.dsig_t;
            FycmB.dALW  = i==0 ? PAsslB.dsig_c_lim : PAsslB.dsig_t_lim;
            PscMembD.Pos[nPos].CalcD.iBdcrCalcType = 0;
        }
        else
        {
            FycmB.bCHK  = NAsslB.ComD.bChk;
            FycmB.LcomK = NAsslB.ComD.RCaseK;
            FycmB.nMax  = NAsslB.ComD.unConcurrent;
            FycmB.dFT   = NAsslB.ComD.dStr[0];
            FycmB.dFB   = NAsslB.ComD.dStr[1];
            FycmB.dFMAX = i==0 ? NAsslB.dsig_c : NAsslB.dsig_t;
            FycmB.dALW  = i==0 ? NAsslB.dsig_c_lim : NAsslB.dsig_t_lim;
            PscMembD.Pos[nPos].CalcD.iBdcrCalcType = 1;
        }

        T_ASPM_D AspmD;
        AspmD.Initialize();
        m_pAttrCtrl->GetAspm(AspmD);

        if ( !AspmD.bSevLimStrsCrack )
        {
            FycmB.bCHK = FALSE;
        }

        m_pDataCtrl->m_pPscDataCtrl->Get_ConcStressReportData_CS454(ElemK, nPos, i, FycmB, PscMembD.Pos[nPos], rData.Str[i]);
    }

    return TRUE;
}

BOOL CRating_BD21_XLOut::GetDetailCrck(ElemPairK ElemK, PSC_DATA_MEMB& PscMembD, const int& nPos, PSC_RPT_BS_POS &rData)
{
    if ( nPos<0 || nPos>1 ) return FALSE;

    T_ASSL_PSC_BS_CASE AsslCase;
    if ( !m_pDoc->m_pPostCtrl->GetAnalysisResult()->GetSLSRating4BD21(ElemK, 0, 1, 0, AsslCase) ) AsslCase.Initialize();
    T_ASSL_PSC_BS_BASE PAsslB = AsslCase.AsslBase[nPos];
    if ( !m_pDoc->m_pPostCtrl->GetAnalysisResult()->GetSLSRating4BD21(ElemK, 0, 2, 0, AsslCase) ) AsslCase.Initialize();
    T_ASSL_PSC_BS_BASE NAsslB = AsslCase.AsslBase[nPos];

    double dRatP = DgnAbsDivide(PAsslB.dw, PAsslB.dw_lim);
    double dRatN = DgnAbsDivide(NAsslB.dw, NAsslB.dw_lim);

    T_BDCW_PSC_BS_BASE BdcwB;
    BdcwB.Initialize();
    if ( dRatN > dRatP )
    {
        BdcwB.bCHK  = PAsslB.ComD.bChk;
        BdcwB.LcomK = PAsslB.ComD.RCaseK;
        BdcwB.iMax  = PAsslB.ComD.unConcurrent;
        BdcwB.dPu   = PAsslB.ComD.dFxx;
        BdcwB.dMu   = PAsslB.ComD.dMuy;
        BdcwB.dMg   = PAsslB.dMg;
        BdcwB.dMq   = PAsslB.dMq;
        BdcwB.dfb   = PAsslB.dfb;
        BdcwB.dft   = PAsslB.dft;
        BdcwB.dCW   = PAsslB.dw;
        BdcwB.dDgnCW= PAsslB.dw_lim;
        PscMembD.Pos[nPos].CalcD.iBdcwCalcType = 1;
    }
    else
    {
        BdcwB.bCHK  = NAsslB.ComD.bChk;
        BdcwB.LcomK = NAsslB.ComD.RCaseK;
        BdcwB.iMax  = NAsslB.ComD.unConcurrent;
        BdcwB.dPu   = PAsslB.ComD.dFxx;
        BdcwB.dMu   = NAsslB.ComD.dMuy;
        BdcwB.dMg   = NAsslB.dMg;
        BdcwB.dMq   = NAsslB.dMq;
        BdcwB.dfb   = NAsslB.dfb;
        BdcwB.dft   = NAsslB.dft;
        BdcwB.dCW   = NAsslB.dw;
        BdcwB.dDgnCW= NAsslB.dw_lim;
        PscMembD.Pos[nPos].CalcD.iBdcwCalcType = 0;
    }

    T_ASPM_D AspmD;
    AspmD.Initialize();
    m_pAttrCtrl->GetAspm(AspmD);

    if ( !AspmD.bSevLimStrsCrack )
    {
        BdcwB.bCHK = FALSE;
    }

    m_pDataCtrl->m_pPscDataCtrl->Get_BdcwReportData_BS(ElemK, BdcwB, PscMembD.Pos[nPos], rData.Crk);

    return TRUE;
}

int CRating_BD21_XLOut::GetDetailCount()
{
    CArray<T_RKPO_K, T_RKPO_K> aPrtElemK;
    m_pDoc->m_pAttrCtrl->GetRkpoKeyList(aPrtElemK);

    CArray<T_RKPV_K, T_RKPV_K> aPrtVBeamK;
    m_pDoc->m_pAttrCtrl->GetRkpvKeyList(aPrtVBeamK);

    ArrElemPairKey aPrintElemPairK;
    CDBLib::ConvertToElemPairKeyList(aPrtElemK, aPrtVBeamK, aPrintElemPairK);

    int nDedail = 0;
    INT_PTR nElem = aPrintElemPairK.GetSize();
    for ( INT_PTR i=0; i<nElem; ++i )
    {
        auto  ElemK = aPrintElemPairK[i];

        // 출력 옵션 셋팅
        // 1. 출력 여부 결정
        T_ASOP_D AsopD; AsopD.Initialize();  // 설계 위치
        T_RKPO_D RkpoD; RkpoD.Initialize();  // 출력 위치
        if (ElemK.second == EN_EL_BEAM)
        {
            if (!m_pDoc->m_pAttrCtrl->GetAsop(ElemK.first, AsopD)) continue;
            if (!m_pDoc->m_pAttrCtrl->GetRkpo(ElemK.first, RkpoD)) continue;
        }
        else if (ElemK.second == EN_EL_VBEAM)
        {
            ASSERT(0); // Asov ?
            if (!m_pDoc->m_pAttrCtrl->GetAsop(ElemK.first, AsopD)) continue;
            if (!m_pDoc->m_pAttrCtrl->GetRkpv(ElemK.first, RkpoD)) continue;
        }
        else ASSERT(0);

        if ( (AsopD.nPos == 0 || AsopD.nPos == 2) && (RkpoD.iPrintChk == 0 || RkpoD.iPrintChk == 2) )
        {
            ++nDedail;
        }
        if ( (AsopD.nPos == 1 || AsopD.nPos == 2) && (RkpoD.iPrintChk == 1 || RkpoD.iPrintChk == 2) )
        {
            ++nDedail;
        }
    }
    return nDedail;
}

CString CRating_BD21_XLOut::GetFlexTableHeadName()
{
    return _T("Rating_Flex_Table_Head");
}

CString CRating_BD21_XLOut::GetShearTableHeadName()
{
    return _T("Rating_Shear_Table_Head");
}

CString CRating_BD21_XLOut::GetAssessFactorContentsName()
{
    return _T("Rating_Assessment_Factor_contents");
}