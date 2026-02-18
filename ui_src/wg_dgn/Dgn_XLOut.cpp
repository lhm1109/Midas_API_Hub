// Rating_XLOut.cpp: implementation of the Az class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Dgn_XLOut.h"
#include <io.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

using namespace dgn::lib;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDgn_XLOut::CDgn_XLOut() : CXLOut()
{  
    m_pDoc = CDBDoc::GetDocPoint();
    m_pAttrCtrl = m_pDoc->m_pAttrCtrl;

    // Add by GAY.('06.07.19). Check Test Mode or not in constructor.
    // Because it takes long time to search in registry list.
    m_bTestMode = CDBLib::CheckTheRegistryForDesignTest(); //IsTestMode();

    m_bSeisEval = false;
}

CDgn_XLOut::~CDgn_XLOut()
{

}

CString CDgn_XLOut::GetText(LPCTSTR lpszFmt, ...)
{
	va_list args;
	va_start(args, lpszFmt);

	CString csText;
	csText.FormatV(lpszFmt, args);

	va_end(args);

	return csText;
}

void CDgn_XLOut::SetUnitConvertFactor()
{  
    double dPi = atan(1.0) * 4.0;

    m_dFactorForce  = 0.001;
    m_dFactorLength = 0.001;
    m_dFactorLengthEach = (m_bUnitSI)? 1.0 : 0.1;
    m_dFactorLengthMM   = 1000.0;
    m_dFactorAreamm2    = 1000.0*1000.0;
    m_dFactorMoment = 0.001 * 0.001;
    m_dFactorStress = 0.001;  
    m_dFactorRebarArea     = 1.0;
    m_dFactorInertiaMoment = pow(0.001, 4);
    m_dFactorInertiaMomentMM4 = pow(1.0, 4);
    m_dFactorAngle  = 180.0 / (dPi);

    m_strUnitForce.Format(_T("0.00 %s"), ((m_bUnitSI)? _T("kN") : _T("tonf")));
    m_strUnitLength.Format(_T("0.000 %s"), _T("m"));
    m_strUnitLengthEach.Format(_T("0.00 %s"), ((m_bUnitSI)? _T("mm") : _T("cm")));  
    m_strUnitLengthMM.Format(_T("0.00000 %s"), _T("mm"));
    m_strUnitMoment.Format(_T("0.00 %s"), ((m_bUnitSI)? _T("kN·m") : _T("tonf·m")));
    m_strUnitStress.Format(_T("0.00 %s"), ((m_bUnitSI)? _T("MPa") : _T("kgf/cm²")));  
    m_strUnitRebarArea.Format(_T("0.00 %s"), ((m_bUnitSI)? _T("mm²") : _T("cm²")));
    m_strUnitInertiaMoment.Format(_T("0.000000 %s"), _T("m⁴"));
    m_strUnitInertiaMomentMM4.Format(_T("0.00 %s"), _T("mm⁴"));
    m_strUnitDegree.Format(_T("0.00 %s"), _T("˚"));

    m_strNone3.Format(_T("0.000"));
    m_strNone4.Format(_T("0.0000"));
}
//
//
void CDgn_XLOut::SetDefaultSheetProperty(IExcel* pExcel)
{
    //(2008.03.14) Add by GAY. Excel2007에서는 <맑은고딕>이 Standard Font로 Setting되어서
    // 문서양식이 다른 관계로 아래와 같이 Sheet 설정함.
    pExcel->SetStandardFont(_T("돋움"));
    //pExcel->SetColumnWidth(0, 0, 0, 26, 2.0);
    //pExcel->SetRowHeight(15);
    pExcel->SetColumnWidth(0, 0, 0, 32, 1.50);
    pExcel->SetRowHeight(15);
}

void CDgn_XLOut::Update_ProgressInft(CString strTitle, CString strMessage, int iPercent)
{	
  if(m_pProgressInte->pDlgWnd == NULL) return;
  m_pProgressInte->strTitle   = strTitle;
  m_pProgressInte->strMessage =  strMessage;
  m_pProgressInte->iPercent   =  iPercent;
  m_pProgressInte->pDlgWnd->RedrawWindow();
}

void CDgn_XLOut::AddBook(IExcel* pXL, CString strPath, int iPrintOpt)
{

    int nCheck = access(CW2A(strPath), 0);

    if(iPrintOpt == 0 || nCheck!=0)
    {
        pXL->AddBook();
    }
    else
    {
        pXL->Open(strPath);    
        pXL->SetActiveBook(pXL->GetBookName());    
    }  
}
//
CString CDgn_XLOut::GetStringPosition(const int &nIndex)
{
    CString strTemp[3] = { _T("I"), _T("M"), _T("J") };
    return strTemp[nIndex];
}

CString CDgn_XLOut::GetStringPositionRating(const int &nIndex)
{
    CString strTemp[2] = { _T("I"),  _T("J") };
    return strTemp[nIndex];
}

CString CDgn_XLOut::SetStringEmpty2Hipun(CString &str)
{
    CString strTmp;
    if (str.IsEmpty() == TRUE)
    {
        strTmp = _T("-");
    }
    else
    {
        strTmp  = str;
    }

    return strTmp;
}


CString CDgn_XLOut::Check_Sign(int nType, double dVal1, double dVal2)
{
    CString strSign;

    if(nType ==0)    // if Left ≥ Right, it's OK; 
    {
        strSign = (dVal1 > dVal2 - cDGN_Zero)? _T("≥") : _T("<") ;
    }   
    else if(nType ==1) // if Left ≤ Right, it's OK;
    {
        strSign = (dVal1 < dVal2 + cDGN_Zero)? _T("≤") : _T(">") ;
    }
    else ASSERT(0);

    return strSign;
}

CString CDgn_XLOut::Chk_OK_Sign(int nType, double dVal1, double dVal2)
{
    CString strSign;

    if(nType ==0)    // if Left > Right, it's NG; 
    {
        strSign = (dVal1 > dVal2)? _LS(IDS_DGN_Msg_NG) : _LS(IDS_DGN_Msg_OK);
    }   
    else if(nType ==1) // if Left < Right, it's NG;
    {
        strSign = (dVal1 < dVal2)? _LS(IDS_DGN_Msg_NG) : _LS(IDS_DGN_Msg_OK);
    }
    else ASSERT(0);

    return strSign;
}

//
int CDgn_XLOut::CopyRange(IExcel* pExcel, CString& RptBlock, int stRow)
{
    int rowCnt=0, colCnt=0;
    pExcel->CopyRange(stRow, 0, m_strBaseBook, m_strBaseSheet, RptBlock, rowCnt, colCnt);
    return rowCnt;
}

int CDgn_XLOut::CopyRangeSubTitle(IExcel* pExcel, CString& RptBlock, int stRow)
{
    int rowCnt=0, colCnt=1;
    pExcel->CopyRange(stRow, 1, m_strBaseBook, m_strBaseSheet, RptBlock, rowCnt, colCnt);
    return rowCnt;
}

int CDgn_XLOut::CopyRangeOddTable(IExcel* pExcel, CString& RptBlock, int stRow)
{
    int rowCnt=0, colCnt=1;
    pExcel->CopyRange(stRow, 8, m_strBaseBook, m_strBaseSheet, RptBlock, rowCnt, colCnt);
    return rowCnt;
}

int CDgn_XLOut::CopyRangeEvenTable(IExcel* pExcel, CString& RptBlock, int stRow)
{
    int rowCnt=0, colCnt=1;
    pExcel->CopyRange(stRow, 18, m_strBaseBook, m_strBaseSheet, RptBlock, rowCnt, colCnt);
    return rowCnt;
}

int CDgn_XLOut::CopyRangeUnImportantTitle(IExcel* pExcel, CString& RptBlock, int stRow, int nColAdd)
{
    int rowCnt=0, colCnt=1;
    pExcel->CopyRange(stRow, 40+ nColAdd, m_strBaseBook, m_strBaseSheet, RptBlock, rowCnt, colCnt);
    return rowCnt;
}

void CDgn_XLOut::OnlyCopyRange(IExcel* pExcel, CString& RptBlock, int& stRow)
{
    int rowCnt = CopyRange(pExcel, RptBlock, stRow);
    stRow+=(rowCnt+1);
}



BOOL CDgn_XLOut::CalcCellID(int stRow, short Row, short Col, short Row_Ref, short Col_Ref, short &rRow_Calc, short &rCol_Calc)
{
    BOOL bChk = TRUE;

    rRow_Calc = stRow + Row - Row_Ref;	
    rCol_Calc = Col - Col_Ref + (IsSeisEval() ? 0 : 1);

    if(rRow_Calc < 0 || rCol_Calc < 0) bChk = FALSE;
    else                               bChk = TRUE;

    return bChk;
}

void CDgn_XLOut::PrintCellData(IExcel* pExcel, const CString strCellname, CString strData, short &rRow_Ref, short &rCol_Ref, int& stRow)
{
    short Row =0, Col=0;
    //  short Row_Ref=0, Col_Ref=0;
    short Row_Calc=0, Col_Calc=0;
    BOOL bChk = TRUE;

    pExcel->GetCellIDByName(strCellname, Row, Col);
    bChk = CalcCellID(stRow, Row, Col, rRow_Ref, rCol_Ref, Row_Calc, Col_Calc);
    if(bChk) pExcel->SetValue(Row_Calc, Col_Calc, strData);
}

void CDgn_XLOut::PrintCellDataMark(IExcel* pExcel, const CString strCellname, CString strData, short& rRow_Ref, short& rCol_Ref, int& stRow)
{
	short Row =0, Col=0;
	short Row_Calc=0, Col_Calc=0;
	BOOL bChk = TRUE;

	pExcel->GetCellIDByName(strCellname, Row, Col);
	bChk = CalcCellID(stRow, Row, Col, rRow_Ref, rCol_Ref, Row_Calc, Col_Calc);
	if(bChk) pExcel->SetValue(Row_Calc, Col_Calc, strData);

	if(pExcel->GetExcelType() == TYPE_LIBXL)
	{
		BOOL bNG = FALSE;
		if(strData.Find(_T("NG")) > -1)                bNG = TRUE;
		else if(strData.Find(_T("N.G")) > -1)          bNG = TRUE;
		else if(strData.Find(_T("УСЛ. НЕ ВЫП.")) > -1) bNG = TRUE;

		if(bNG)
		{
			pExcel->SetTextColor(Row_Calc, Col_Calc, Row_Calc, Col_Calc, RGB(255, 0, 0));  // Red
		}
	}
}

void CDgn_XLOut::PrintCellData(IExcel* pExcel, const CString strCellname, double dData, short &rRow_Ref, short &rCol_Ref, int& stRow)
{
    short Row=0, Col=0;
    //short Row_Ref=0, Col_Ref=0;
    short Row_Calc=0, Col_Calc=0;
    BOOL bChk = TRUE;

    pExcel->GetCellIDByName(strCellname, Row, Col);
    bChk = CalcCellID(stRow, Row, Col, rRow_Ref, rCol_Ref, Row_Calc, Col_Calc);
    if(bChk) pExcel->SetValue(Row_Calc, Col_Calc, dData);
}

void CDgn_XLOut::PrintCellTrashData(IExcel* pExcel, const CString strCellname, int nColAdd, double dData, short &rRow_Ref, short &rCol_Ref, int& stRow)
{
    short Row=0, Col=0;
    //short Row_Ref=0, Col_Ref=0;
    short Row_Calc=0, Col_Calc=0;
    BOOL bChk = TRUE;
    int iTrashCol = 39 + nColAdd;
    pExcel->GetCellIDByName(strCellname, Row, Col);
    bChk = CalcCellID(stRow, Row, Col, rRow_Ref, rCol_Ref, Row_Calc, Col_Calc);
    Col_Calc = Col_Calc + iTrashCol;
    if(bChk) pExcel->SetValue(Row_Calc, Col_Calc, dData);
}


void CDgn_XLOut::PrintCellOddEvenData(IExcel* pExcel, const CString strCellname, int iOddEvenCol, double dData, short &rRow_Ref, short &rCol_Ref, int& stRow)
{
    short Row=0, Col=0;
    //short Row_Ref=0, Col_Ref=0;
    short Row_Calc=0, Col_Calc=0;
    BOOL bChk = TRUE;
    pExcel->GetCellIDByName(strCellname, Row, Col);
    bChk = CalcCellID(stRow, Row, Col, rRow_Ref, rCol_Ref, Row_Calc, Col_Calc);
    Col_Calc = Col_Calc + iOddEvenCol;
    if(bChk) pExcel->SetValue(Row_Calc, Col_Calc, dData);
}

void CDgn_XLOut::PrintCellOddEvenData(IExcel* pExcel, const CString strCellname, int iOddEvenCol, CString strData, short &rRow_Ref, short &rCol_Ref, int& stRow)
{
    short Row=0, Col=0;
    //short Row_Ref=0, Col_Ref=0;
    short Row_Calc=0, Col_Calc=0;
    BOOL bChk = TRUE;
    pExcel->GetCellIDByName(strCellname, Row, Col);
    bChk = CalcCellID(stRow, Row, Col, rRow_Ref, rCol_Ref, Row_Calc, Col_Calc);
    Col_Calc = Col_Calc + iOddEvenCol;
    if(bChk) pExcel->SetValue(Row_Calc, Col_Calc, strData);
}





void CDgn_XLOut::PrintCellData(IExcel* pExcel, const CString strBaseBook, const CString strSheetName,  const CString strCellname, CString strData, short &rRow_Ref, short &rCol_Ref, int& stRow)
{
    short Row =0, Col=0;
    //  short Row_Ref=0, Col_Ref=0;
    short Row_Calc=0, Col_Calc=0;
    BOOL bChk = TRUE;

    pExcel->GetCellIDByName(strBaseBook, strSheetName, strCellname, Row, Col);
    bChk = CalcCellID(stRow, Row, Col, rRow_Ref, rCol_Ref, Row_Calc, Col_Calc);
    if(bChk) pExcel->SetValue(Row_Calc, Col_Calc, strData);
}

void CDgn_XLOut::PrintCellData(IExcel* pExcel, const CString strBaseBook, const CString strSheetName, const CString strCellname, double dData, short &rRow_Ref, short &rCol_Ref, int& stRow)
{
    short Row=0, Col=0;
    //short Row_Ref=0, Col_Ref=0;
    short Row_Calc=0, Col_Calc=0;
    BOOL bChk = TRUE;

    pExcel->GetCellIDByName(strBaseBook, strSheetName, strCellname, Row, Col);
    bChk = CalcCellID(stRow, Row, Col, rRow_Ref, rCol_Ref, Row_Calc, Col_Calc);
    if(bChk) pExcel->SetValue(Row_Calc, Col_Calc, dData);
}

void CDgn_XLOut::InsertCellForRoof(IExcel* pExcel, CString strRefRowCellname, short &rRow_Ref, short &rCol_Ref, int& stRow)
{
    CString sCell1_St;
    CString sCell1_Ed;
    CString sCell2_St;
    CString sCell2_Ed;

    short Row=0, Col=0;
    short Row_Calc=0, Col_Calc=0;
    BOOL bChk = TRUE;

    pExcel->GetCellIDByName(strRefRowCellname, Row, Col);
    bChk = CalcCellID(stRow, Row, Col, rRow_Ref, rCol_Ref, Row_Calc, Col_Calc);

    Row_Calc = Row_Calc - 5;

    sCell1_St.Format(_T("A%d"), Row_Calc+1);
    //	sCell1_Ed.Format(_T("LZ%d"), Row_Calc+1); // 이쯤이면 되지 않을까... 이거 넘어가면 좀 심하네...
    sCell1_Ed.Format(_T("HZ%d"), Row_Calc+1);   // 이유는 모르겠지만 영역이 너무 크면 계산서 출력시 에러 발생되어 간격을 줄임.

    sCell2_St.Format(_T("A%d"), Row_Calc+2);
    //	sCell2_Ed.Format(_T("LZ%d"), Row_Calc+2); 
    sCell2_Ed.Format(_T("HZ%d"), Row_Calc+2); 

    pExcel->InsertCell(sCell2_St, sCell2_Ed, 3);
    pExcel->Copy(sCell1_St, sCell1_Ed, sCell2_St, sCell2_Ed);
}

BOOL CDgn_XLOut::CopyCellForRoof(IExcel* pExcel, CString strRefRowCellname, short &rRow_Ref, short &rCol_Ref, int& stRow)
{
    CString sCell1_St;
    CString sCell1_Ed;
    CString sCell2_St;
    CString sCell2_Ed;

    short Row=0, Col=0;
    short Row_Calc=0, Col_Calc=0;
    BOOL bChk = TRUE;

    pExcel->GetCellIDByName(strRefRowCellname, Row, Col);
    bChk = CalcCellID(stRow, Row, Col, rRow_Ref, rCol_Ref, Row_Calc, Col_Calc);
    if (!bChk) return FALSE;

    Row_Calc = Row_Calc - 5;

    sCell1_St.Format(_T("A%d"), Row_Calc+1);
    //	sCell1_Ed.Format(_T("LZ%d"), Row_Calc+1); // 이쯤이면 되지 않을까... 이거 넘어가면 좀 심하네...
    sCell1_Ed.Format(_T("AD%d"), Row_Calc+1);   // 이유는 모르겠지만 영역이 너무 크면 계산서 출력시 에러 발생되어 간격을 줄임.

    sCell2_St.Format(_T("A%d"), Row_Calc+2);
    //	sCell2_Ed.Format(_T("LZ%d"), Row_Calc+2); 
    sCell2_Ed.Format(_T("AD%d"), Row_Calc+2); 

    pExcel->Copy(sCell1_St, sCell1_Ed, sCell2_St, sCell2_Ed);

    return TRUE;
}

CString CDgn_XLOut::FormatOutput(int iValue)
{
	CString strReturn;
	strReturn.Format(_T("%d"), iValue);

	return strReturn;
}

CString CDgn_XLOut::FormatOutput(double dValue, int iDecimalPlaces)
{
	CString strResult, strOutput;

	strOutput.Format(_T("%s.%df"), _T("%"), iDecimalPlaces);

	strResult.Format(strOutput, dValue);

	return strResult;
}

CString CDgn_XLOut::GetPartNameWithNode(T_ELEM_K ElemK, int iPosiNo)
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

CString CDgn_XLOut::GetLcomType(int iLcomType)
{
	CString strLcomType=_T("");
	if(iLcomType==0)        strLcomType = _T("-");
	else if(iLcomType==1)   strLcomType = _T("Fx-max");
	else if(iLcomType==2)   strLcomType = _T("Fx-min");
	else if(iLcomType==3)   strLcomType = _T("Fy-max");
	else if(iLcomType==4)   strLcomType = _T("Fy-min");
	else if(iLcomType==5)   strLcomType = _T("Fz-max");
	else if(iLcomType==6)   strLcomType = _T("Fz-min");
	else if(iLcomType==7)   strLcomType = _T("Mx-max");
	else if(iLcomType==8)   strLcomType = _T("Mx-min");
	else if(iLcomType==9)   strLcomType = _T("My-max");
	else if(iLcomType==10)  strLcomType = _T("My-min");
	else if(iLcomType==11)  strLcomType = _T("Mz-max");
	else if(iLcomType==12)  strLcomType = _T("Mz-min");
	else                    strLcomType = _T("-");

	return strLcomType;
}

CString CDgn_XLOut::GetStringOKNG(const int& nOK)
{
	CString str;
	if(nOK == 1)
	{
		str = _T("OK");
	}
	else if(nOK == 2)
	{
		str = _T("NG");
	}
	else if(nOK == 0)
	{
		str = _T("NG");
	}
	else
	{
		ASSERT(0);
	}

	return str;
}