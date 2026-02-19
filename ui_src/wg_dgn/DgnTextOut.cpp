#include "stdafx.h"
#include "DgnTextOut.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\UnitCtrl.h"
#include "..\MIT_Lib\rptostream.h"
using namespace std;

const double cDgn_Zero = 1e-7;

CDgnTextOut::CDgnTextOut(void)
{
    
}

CDgnTextOut::~CDgnTextOut(void)
{
}

void CDgnTextOut::SetUnitFactor()
{
    CDBDoc* pDoc = CDBDoc::GetDocPoint();
    CUnitCtrl* pUnitCtrl = pDoc->m_pUnitCtrl;
    T_UNIT_INDEX CurIndex;
    pUnitCtrl->GetUnitIndexCurrent(CurIndex);

    T_UNIT_NAMEFACT CurUnitName;
    pUnitCtrl->GetUnitNameCurrent(CurUnitName);

    const CString& strCurUnitForc = CurUnitName.strForce[CurIndex.nBase_Force];
    const CString& strCurUnitLeng = CurUnitName.strLength[CurIndex.nBase_Length];
    
    m_strUserForUnit = strCurUnitForc;
    m_strUserLenUnit = strCurUnitLeng;

    m_dFlen = 1.0;
    m_dFln2 = 1.0;
    m_dFln3 = 1.0;
    m_dFln4 = 1.0;
    m_dFfor = 1.0;
    m_dFmot = 1.0;
    m_dFstr = 1.0;

    m_strLenUnit1 = strCurUnitLeng;
    m_strLenUnit2 = strCurUnitLeng;
	m_strAreaUnit = strCurUnitLeng + _T("^2");
	m_strLen3Unit = strCurUnitLeng + _T("^3");
	m_strLen4Unit = strCurUnitLeng + _T("^4");
	m_strLen6Unit = strCurUnitLeng + _T("^6");
	m_strLn_1Unit = strCurUnitLeng + _T("^3");
    m_strForUnit  = strCurUnitForc;
    m_strMomUnit  = strCurUnitForc;
	m_strStrUnit  = strCurUnitForc + _T("/") + strCurUnitLeng + _T("^2");
    m_strLen4ByFor2Unit = _T("");
    m_strFor1ByLen6Unit = _T("");
    m_strFor2Len4Unit   = _T("");

    // 모멘트 표기법 변경(2003.12.16, 미국요구사항)	
    if ( (CurIndex.nBase_Force  == D_UNITSYS_FORCE_INDEX_LBF || CurIndex.nBase_Force  == D_UNITSYS_FORCE_INDEX_KIP) &&
        (CurIndex.nBase_Length == D_UNITSYS_LENGTH_INDEX_IN || CurIndex.nBase_Length == D_UNITSYS_LENGTH_INDEX_FT) )
    {
        m_strMomUnit = strCurUnitLeng + _T("-") + strCurUnitForc;
    }
    else
    {
        m_strMomUnit = strCurUnitForc + _T("-") + strCurUnitLeng;
    }
}

void CDgnTextOut::SetFileName(const CString& csFileName)
{
    m_strFileName = csFileName;    
}

CString CDgnTextOut::GetFileName()
{
    return m_strFileName;
}

void CDgnTextOut::SetParagraphTotalLines(UINT nRows)
{
    if (m_bInsertPageBreak)
    {
        ///##Calc_Page(nRows);
    }
}

void CDgnTextOut::SetLineWidthForOutline1(const UINT nWidth/*=80*/)
{
    m_nLineWidthForOutline1 = nWidth;
}

void CDgnTextOut::WriteLine(const CString& strLineSymbol)
{
    if (m_nLineWidthForOutline1 == 0)
        m_nLineWidthForOutline1 = 80;

    const CString& csLine = GetLine(m_nLineWidthForOutline1, strLineSymbol);

    rptwofstream fout(m_strFileName, ios::app);
    fout<<endl
        <<_T(" ") <<csLine <<endl;
}

void CDgnTextOut::WriteOutline1(CString csStr, BOOL bUpperCase/*=TRUE*/)
{
    CString csStrOut = csStr;
    if (bUpperCase)
    {
        csStrOut.MakeUpper();
    }

    if (m_bInsertPageBreak)
    {
        ///##Calc_Page(5);
    }

    if (m_nLineWidthForOutline1 == 0)
        m_nLineWidthForOutline1 = 80;

    CString csLine = GetLine(m_nLineWidthForOutline1, _T("="));

    rptwofstream fout(m_strFileName, ios::app);
    fout<<endl
        <<endl
        <<_T(" ") <<csLine <<endl
        <<_T("    ")<<m_strMarkDt<<_T("   ")<<csStr<<endl
        <<_T(" ") <<csLine <<endl;
}

void CDgnTextOut::WriteOutline1(CString csStr1, CString csStr2, BOOL bUpperCase/*=TRUE*/)
{
    if (bUpperCase)
    {
        csStr1.MakeUpper();
        csStr2.MakeUpper();
    }

    if (m_bInsertPageBreak)
    {
        ///##Calc_Page(6);
    }

    if (m_nLineWidthForOutline1 == 0)
        m_nLineWidthForOutline1 = 80;

    CString csLine  = GetLine(m_nLineWidthForOutline1, _T("="));
    CString csSpace = GetSpace(m_strMarkDt.GetLength());

    rptwofstream fout(m_strFileName, ios::app);
    fout<<endl
        <<endl
        <<_T(" ") <<csLine <<endl
        <<_T("    ") <<m_strMarkDt <<_T("   ") <<csStr1 <<endl
        <<_T("    ") <<csSpace     <<_T("   ") <<csStr2 <<endl
        <<_T(" ") <<csLine <<endl;
}

void CDgnTextOut::WriteOutline2(CString csStr)
{
    if (m_bInsertPageBreak)
    {
        ///##Calc_Page(2);
    }

    rptwofstream fout(m_strFileName, ios::app);

    if (!m_bBaseCodePrinted)
    {
        fout <<endl;
    }
    fout <<GetSpace(5) <<_T("( ). ") <<csStr <<endl;

    m_bBaseCodePrinted = FALSE;
}

void CDgnTextOut::WriteOutline1Fmt(LPCTSTR Fmt, ...)
{
    va_list args;
    va_start(args, Fmt);

    CString strVal = _T("");
    strVal.FormatV(Fmt, args);
    va_end(args);  

    WriteOutline1(strVal);
}

void CDgnTextOut::WriteOutline2Fmt(LPCTSTR Fmt, ...)
{
    va_list args;
    va_start(args, Fmt);

    CString strVal = _T("");
    strVal.FormatV(Fmt, args);
    va_end(args);  

    WriteOutline2(strVal);
}

void CDgnTextOut::WriteOutline3(CString csStr)
{
    if (m_bInsertPageBreak)
    {
        ///##Calc_Page(1);
    }

    rptwofstream fout(m_strFileName, ios::app);
    fout <<GetSpace(10) <<_T("-. ") <<csStr <<endl;
}

void CDgnTextOut::WriteOutline3Fmt(LPCTSTR Fmt, ...)
{
    va_list args;
    va_start(args, Fmt);

    CString strVal = _T("");
    strVal.FormatV(Fmt, args);
    va_end(args);  

    WriteOutline3(strVal);
}

void CDgnTextOut::WriteOutline3(CString csTitle, CString csInfo, const EN_DIV_TYPE enDivType/*=EN_DIV_TYPE::DBDOT*/, UINT nTitleLen/*=12*/)
{
    CString csFormat;
    csFormat.Format(_T("-. %%-%ds %s %%s"), nTitleLen, GetDivideStr(enDivType)); // (ex) _T("-. %-12s : %s")

    CString csOut;
    csOut.Format(csFormat, csTitle, csInfo);

    if (m_bInsertPageBreak)
    {
        ///##Calc_Page(1);
    }

    rptwofstream fout(m_strFileName, ios::app);
    fout <<GetSpace(10) <<csOut <<endl;
}

void CDgnTextOut::WriteOutline3(CString csTitle, CString csInfo, CString csNote, const EN_DIV_TYPE enDivType/*=EN_DIV_TYPE::DBDOT*/, UINT nTitleLen/*=12*/)
{
    CString csFormat;
    csFormat.Format(_T("-. %%-%ds %s %%s"), nTitleLen, GetDivideStr(enDivType)); // (ex) _T("-. %-12s : %s")

    CString csOut;
    csOut.Format(csFormat, csTitle, csInfo);
    if (!csNote.IsEmpty())
    {
        CString csAdd;
        csAdd.Format(_T(" ( %s )"), csNote);
        csOut += csAdd;
    }

    if (m_bInsertPageBreak)
    {
        ///##Calc_Page(1);
    }

    rptwofstream fout(m_strFileName, ios::app);
    fout <<GetSpace(10) <<csOut <<endl;
}

void CDgnTextOut::WriteOutline4(CString csStr)
{
    if (m_bInsertPageBreak)
    {
        ///##Calc_Page(1);
    }

    rptwofstream fout(m_strFileName, ios::app);
    fout <<GetSpace(13) <<_T("*. ") <<csStr <<endl;
}

void CDgnTextOut::WriteOutline4Fmt(LPCTSTR Fmt, ...)
{
    va_list args;
    va_start(args, Fmt);

    CString strVal = _T("");
    strVal.FormatV(Fmt, args);
    va_end(args);  

    WriteOutline4(strVal);
}

void CDgnTextOut::WriteOutline4(CString csTitle, CString csInfo, const EN_DIV_TYPE enDivType/*=EN_DIV_TYPE::DBDOT*/, UINT nTitleLen/*=12*/)
{
    CString csFormat;
    csFormat.Format(_T("*. %%-%ds %s %%s"), nTitleLen, GetDivideStr(enDivType)); // (ex) _T("-. %-12s : %s")

    CString csOut;
    csOut.Format(csFormat, csTitle, csInfo);

    if (m_bInsertPageBreak)
    {
        ///##Calc_Page(1);
    }

    rptwofstream fout(m_strFileName, ios::app);
    fout <<GetSpace(13) <<csOut <<endl;
}

void CDgnTextOut::WriteEquation(CString csStr, UINT nBlank/*=0*/)
{
    if (nBlank == 0)
        nBlank = 16;
    WriteText(nBlank, csStr);
}

void CDgnTextOut::WriteOutline4(CString csTitle, CString csInfo, CString csNote, const EN_DIV_TYPE enDivType/*=EN_DIV_TYPE::DBDOT*/, UINT nTitleLen/*=12*/)
{
    CString csFormat;
    csFormat.Format(_T("*. %%-%ds %s %%s"), nTitleLen, GetDivideStr(enDivType)); // (ex) _T("-. %-12s : %s")

    CString csOut;
    csOut.Format(csFormat, csTitle, csInfo);
    if (!csNote.IsEmpty())
    {
        CString csAdd;
        csAdd.Format(_T(" ( %s )"), csNote);
        csOut += csAdd;
    }

    if (m_bInsertPageBreak)
    {
        ///##Calc_Page(1);
    }

    rptwofstream fout(m_strFileName, ios::app);
    fout <<GetSpace(13) <<csOut <<endl;
}

void CDgnTextOut::WriteTableRow(const CString& csStr)
{
    if (m_bInsertPageBreak)
    {
        ///##Calc_Page(1);
    }

    rptwofstream fout(m_strFileName, ios::app);
    fout <<GetSpace(4) <<csStr <<endl;
}

void CDgnTextOut::WriteCode(CString csStr)
{
    m_bBaseCodePrinted = TRUE;

    if (m_bInsertPageBreak)
    {
        ///##Calc_Page(1);
    }

    rptwofstream fout(m_strFileName, ios::app);

    UINT nCodeLen = m_nLineWidthForOutline1 + 1; // WriteOutline1() 출력시 공백 하나 출력함.
    CString csCode;
    csCode.Format(_T("[ %s ]"), csStr);
    csCode = GetString(csCode, nCodeLen, EN_ALIGN_TYPE::RIGHT);
    fout <<csCode <<endl;
    //fout <<GetSpace(10) <<_T("[ ") <<csStr <<_T(" ]") <<endl;
}

void CDgnTextOut::WriteInfo(CString csTitle, CString csInfo, const EN_DIV_TYPE enDivType/*=EN_DIV_TYPE::DBDOT*/, UINT nTitleLen/*=12*/)
{
    CString csFormat;
    csFormat.Format(_T("%%-%ds %s %%s"), nTitleLen, GetDivideStr(enDivType)); // (ex) %-12s : %s

    CString csOut;
    csOut.Format(csFormat, csTitle, csInfo);

    if (m_bInsertPageBreak)
    {
        ///##Calc_Page(1);
    }

    rptwofstream fout(m_strFileName, ios::app);
    fout <<GetSpace(10) <<csOut <<endl;
}

void CDgnTextOut::WriteText(UINT nSpace, CString csStr)
{
    if (nSpace > 0)
    {
        csStr.Insert(0, GetSpace(nSpace));
    }

    if (m_bInsertPageBreak)
    {
        ///##Calc_Page(1);
    }
	
    rptwofstream fout(m_strFileName, ios::app);
    fout <<GetSpace(10) <<csStr <<endl;
}

void CDgnTextOut::WriteBlank(UINT nRow/*=1*/)
{
    for (UINT i=0; i<nRow; ++i)
    {
        WriteText(0, _T(""));
    }
}

CString CDgnTextOut::GetRho(double dRho)
{
    return GetStringValue(dRho, 14, 6);
}

CString CDgnTextOut::GetFactor(double dFactor)
{
    return GetStringValue(dFactor, DEFAULT_VALUE_LEN, DEFAULT_VALUE_DEC);
}

CString CDgnTextOut::GetRatio(double dRatio)
{
    return GetStringValue(dRatio, 6, 3);
}

CString CDgnTextOut::GetStringValue(double dValue, int iTotal, int iUnder)
{
    BOOL bChange = FALSE;
    if     (fabs(dValue) >= 1.0*pow(10.0,iTotal-iUnder-1)) bChange = TRUE;
    else if(fabs(dValue) <  1.0*pow(10,-iUnder))	 bChange = TRUE;

    if (fabs(dValue) < pow(cDgn_Zero,3)) bChange = FALSE;

    CString strSymbol;
    if(bChange)	strSymbol.Format(_T("%%%d.%de"), iTotal, iUnder);
    else		strSymbol.Format(_T("%%%d.%df"), iTotal, iUnder);

    CString strValue;
    strValue.Format(strSymbol, dValue);

    return strValue;
}

CString CDgnTextOut::GetStringValue(double dValue, int iUnder)
{
    return GetStringValue(dValue, DEFAULT_VALUE_LEN, iUnder); // GetCurVal에서 사용하는 일반 단위계의 전체 자리수.
}

double CDgnTextOut::GetCurUnitValue(double dValue, UINT nUnitType)
{
    double dFlen2 = pow(m_dFlen, 2.0);
    double dFlen3 = pow(m_dFlen, 3.0);
    double dFlen4 = pow(m_dFlen, 4.0);
    double dFlen6 = pow(m_dFlen, 6.0);

    double dCurVal=0.0;
    switch (nUnitType)
    {
    case D_UNITSYS_NONE         : dCurVal = dValue; break;
    case D_UNITSYS_BASE_LENGTH  : dCurVal = dValue/m_dFlen; break;
    case D_UNITSYS_BASE_FORCE   : dCurVal = dValue/m_dFfor; break;
    case D_UNITSYS_BASE_MOMENT  : dCurVal = dValue/m_dFmot; break;
    case D_UNITSYS_BASE_STRESS  : dCurVal = dValue/m_dFstr; break;
    case D_UNITSYS_BASE_AREA    : dCurVal = dValue/dFlen2 ; break;
        //     case D_UNITSYS_BASE_UNITAREA: dCurVal = dValue/m_dFlen; break;
        //     case D_UNITSYS_BASE_UNITMOM : dCurVal = dValue/m_dFfor; break;
    case D_UNITSYS_BASE_VOLUME  : dCurVal = dValue/dFlen3 ; break;
    case D_UNITSYS_BASE_STIF    : dCurVal = dValue/dFlen4 ; break;
    case D_UNITSYS_BASE_L6      : dCurVal = dValue/dFlen6 ; break;
    default: { ASSERT(0); break; }
    }

    return dCurVal;
}

CString CDgnTextOut::GetUnitString(UINT nUnitType, BOOL bUnitOnly/*=FALSE*/)
{
    CString csUnit;
    switch (nUnitType)
    {
    case D_UNITSYS_NONE         : break;
    case D_UNITSYS_BASE_LENGTH  : csUnit = m_strLenUnit1; break;
    case D_UNITSYS_BASE_FORCE   : csUnit = m_strForUnit ; break;
    case D_UNITSYS_BASE_MOMENT  : csUnit = m_strMomUnit ; break;
    case D_UNITSYS_BASE_STRESS  : csUnit = m_strStrUnit ; break;
    case D_UNITSYS_BASE_AREA    : csUnit = m_strAreaUnit; break;
        //     case D_UNITSYS_BASE_UNITAREA: csUnit = m_strLenUnit2; break;
        //     case D_UNITSYS_BASE_UNITMOM : csUnit = m_strMomUnit + _T("/") + m_strLenUnit1; break;
    case D_UNITSYS_BASE_VOLUME  : csUnit = m_strLen3Unit; break;
    case D_UNITSYS_BASE_STIF    : csUnit = m_strLen4Unit; break;
    case D_UNITSYS_BASE_L6      : csUnit = m_strLen6Unit; break;
    default: ASSERT(0); break;
    }

    if (bUnitOnly)
    {
        csUnit.Trim();
        RemoveDot(csUnit);
    }

    return csUnit;
}

CString CDgnTextOut::GetCurStr(CString csStr, UINT nTotalLen/*=DEFAULT_VALUE_LEN*/)
{
    if (nTotalLen == 0)
    {
        nTotalLen = DEFAULT_VALUE_LEN;
    }

    CString csRet = GetString(csStr, nTotalLen, EN_ALIGN_TYPE::RIGHT, FALSE);

    return csRet;
}

CString CDgnTextOut::GetCurVal(double dValue, UINT nUnitType, BOOL bAddUnit/*=TRUE*/, UINT nTotalLen/*=DEFAULT_VALUE_LEN*/, UINT nUnderLen/*=DEFAULT_VALUE_DEC*/)
{
    if (nTotalLen + nUnderLen == 0)
    {
        nTotalLen = DEFAULT_VALUE_LEN;
        nUnderLen = DEFAULT_VALUE_DEC;
    }

    if (nUnitType == D_UNITSYS_NONE)
    {
        nTotalLen = 7;
        nUnderLen = 0;
    }

    double dCurVal = GetCurUnitValue(dValue, nUnitType);
    CString csVal;
    if (nUnderLen == 0)
    {
        int nValue = (int)ceilf(dCurVal);
        csVal.Format(_T("%d"), nValue);
        int nLength = csVal.GetLength();
        if (nLength < nTotalLen)
        {
            csVal.Insert(0, GetSpace(nTotalLen-nLength));
        }
    }
    else
    {
        csVal  = GetStringValue(dCurVal, nTotalLen, nUnderLen);
    }

    if (bAddUnit)
    {
        CString csUnit = GetUnitString(nUnitType, TRUE);
        csVal += _T(" ") + csUnit;
    }

    return csVal;
}

CString CDgnTextOut::GetSpace(UINT nSpace)
{
    CString csFormat;
    csFormat.Format(_T("%%%ds"), nSpace); // (ex) %3s

    CString csSpace;
    csSpace.Format(csFormat, _T(" "));

    return csSpace;
}

CString CDgnTextOut::GetLine(UINT nLineLen, CString csText/*=_T("-")*/)
{
    CString csFormat;
    csFormat.Format(_T("%%%ds"), nLineLen); // (ex) %3s

    CString csLine;
    csLine.Format(csFormat, _T(" "));
    if (!csLine.IsEmpty())
    {
        csLine.Replace(_T(" "), csText);
    }

    return csLine;
}

CString CDgnTextOut::GetTableLine(UINT nTableWidth, BOOL bPlusAtEnd/*=FALSE*/)
{
    if (bPlusAtEnd)
    {
        if ( nTableWidth <= 2 || nTableWidth == (UINT)-1 ) return _T("-");
        nTableWidth -= 2;
    }

    CString csDash = GetLine(nTableWidth);
    if (bPlusAtEnd)
    {
        CString csPlus = _T("+");
        csDash = csPlus + csDash + csPlus;
    }

    return csDash;
}

CString CDgnTextOut::GetDivideStr(const EN_DIV_TYPE enDivType)
{
    switch (enDivType)
    {
    case EN_DIV_TYPE::DBDOT: return  _T(":");
    case EN_DIV_TYPE::EQUAL: return  _T("=");
    case EN_DIV_TYPE::BLANK: return  _T(" ");
    case EN_DIV_TYPE::COMMA: return  _T(",");
    case EN_DIV_TYPE::COLON: return  _T(";");
    case EN_DIV_TYPE::EMPTY: return  _T("");
    }

    ASSERT(0);
    return _T("");
}

CString CDgnTextOut::GetString(const CString& csString, UINT nTotalLength, const EN_ALIGN_TYPE enType/*= EN_ALIGN_TYPE::RIGHT*/, BOOL bAbb/*=FALSE*/)
{
    if (nTotalLength < 1) { ASSERT(0); return _T(""); }

    UINT nString = csString.GetLength();
    if (nString > nTotalLength)
    {
        if (bAbb)
            return csString.Left(nTotalLength-1) + _T("~");
        else
            return csString;
    }

    CString csFormat, csReturn;
    CString csBlank = _T(" ");
    switch (enType)
    {
    case EN_ALIGN_TYPE::RIGHT:
        {
            csFormat.Format(_T("%%%ds"), nTotalLength);
            csReturn.Format(csFormat, csString);
            break;
        }
    case EN_ALIGN_TYPE::LEFT:
        {
            csFormat.Format(_T("%%-%ds"), nTotalLength);
            csReturn.Format(csFormat, csString);
            break;
        }
    case EN_ALIGN_TYPE::CENTER:
        {
            UINT nBlankA = nTotalLength - nString;
            UINT nBlankL = nBlankA/2;
            UINT nBlankR = nBlankL;
            if (nBlankL + nBlankR != nBlankA)
                nBlankR += 1;

            CString csBlankL; csBlankL.Empty();
            CString csBlankR; csBlankR.Empty();
            if (nBlankL > 0)
            {
                csFormat.Format(_T("%%%ds"), nBlankL);
                csBlankL.Format(csFormat, csBlank);
            }
            if (nBlankR > 0)
            {
                csFormat.Format(_T("%%%ds"), nBlankR);
                csBlankR.Format(csFormat, csBlank);
            }

            csReturn = csBlankL + csString + csBlankR;
            break;
        }
    default: ASSERT(0); break;
    }

    return csReturn;
}

CString CDgnTextOut::GetStringWithNote(CString csSymbol, double dValue, UINT nUnitType, CString csNote/*=_T("")*/)
{
    CString csValue = GetCurVal(dValue, nUnitType);
    return GetStringWithNote(csSymbol, csValue, csNote);
}

CString CDgnTextOut::GetStringWithNote(CString csSymbol, CString csValue, CString csNote/*=_T("")*/)
{
    UINT nUnit   = 7;
    UINT nSymbol = DEFAULT_STRLEN;
    UINT nValue  = DEFAULT_STRLEN + nUnit;

    CString strSymbol = GetString(csSymbol, nSymbol, EN_ALIGN_TYPE::LEFT);
    CString strValue  = GetString(csValue, nValue, EN_ALIGN_TYPE::LEFT);

    CString csStrNote;
    csStrNote.Format(_T("%s = %s"), strSymbol, strValue);

    if (!csNote.IsEmpty())
    {
        CString csAdd;
        csAdd.Format(_T(" ( %s )"), csNote);
        csStrNote += csAdd;
    }

    return csStrNote.Trim();
}

void CDgnTextOut::RemoveDot(CString &rString)
{
    if (rString.IsEmpty()) return;

    while (TRUE)
    {
        int nPos = rString.Find('.');
        if (nPos < 0) break;
        if (rString.IsEmpty()) break;

        rString.Delete(nPos);
    }
}

CString CDgnTextOut::GetStringOKNG(double dRatio, double dCriteria, BOOL bSmallOK/*=TRUE*/)
{
    if (bSmallOK)
    {
        BOOL bOK = (dRatio > dCriteria) ? FALSE : TRUE;
        CString csRatio;
        csRatio.Format(_T("%s %s %s %s"), GetRatio(dRatio), (bOK ? STR_SMALL : STR_BIGGR), GetRatio(dCriteria), (bOK ? STR_OK : STR_NG));
        return csRatio;
    }
    else
    {
        BOOL bOK = (dRatio > dCriteria) ? TRUE : FALSE;
        CString csRatio;
        csRatio.Format(_T("%s %s %s %s"), GetRatio(dRatio), (bOK ? STR_BIGGR : STR_SMALL), GetRatio(dCriteria), (bOK ? STR_OK : STR_NG));
        return csRatio;
    }
}

void CDgnTextOut::WriteGnrlInfo(CString csStr, BOOL bSymbol/*=TRUE*/)
{
    if (m_bInsertPageBreak)
    {
        ///##Calc_Page(1);
    }

    CString csSymbol; csSymbol.Empty();
    UINT nIndent = 8;
    if (bSymbol)
    {
        csSymbol = _T("*. ");
        nIndent -= csSymbol.GetLength();
    }
    CString csIndent = GetSpace(nIndent);

    rptwofstream fout(m_strFileName, ios::app);
    fout <<csIndent <<csSymbol <<csStr <<endl;
}

void CDgnTextOut::WriteComment(const CString& strComment)
{
    if (m_bInsertPageBreak)
    {
        ///##Calc_Page(1);
    }

    CString csOut = _T("");
    csOut.Format(_T("[ %s ]"), strComment);

    rptwofstream fout(m_strFileName, ios::app);
    fout <<GetSpace(12) <<csOut <<endl;
}

void CDgnTextOut::WritePara(const CString& strVal)
{
    if (m_bInsertPageBreak)
    {
        ///##Calc_Page(1);
    }

    rptwofstream fout(m_strFileName, ios::app);
    fout <<GetSpace(12) <<strVal <<endl;
}


void CDgnTextOut::WriteParaFmt(LPCTSTR Fmt, ...)
{
    va_list args;
    va_start(args, Fmt);

    CString strVal = _T("");
    strVal.FormatV(Fmt, args);
    va_end(args);  

    if (m_bInsertPageBreak)
    {
        ///##Calc_Page(1);
    }

    rptwofstream fout(m_strFileName, ios::app);
    fout <<GetSpace(12) <<strVal <<endl;
}

void CDgnTextOut::WriteTextFmt(const UINT unSpace, LPCTSTR Fmt, ...)
{
    va_list args;
    va_start(args, Fmt);

    CString strVal = _T("");
    strVal.FormatV(Fmt, args);
    va_end(args);
    
    WriteText(unSpace, strVal);
}

CString CDgnTextOut::GetGnrlInfo(const CString& csHead, UINT nInfo, BOOL bUpperHead/*=TRUE*/, BOOL bUpperInfo/*=FALSE*/)
{
    CString csInfo;
    csInfo.Format(_T("%d"), nInfo);
    return GetGnrlInfo(csHead, csInfo, bUpperHead, bUpperInfo);
}

CString CDgnTextOut::GetGnrlInfo(const CString& csHead, const CString& csInfo, BOOL bUpperHead/*=TRUE*/, BOOL bUpperInfo/*=FALSE*/)
{
    UINT nHead = DEFAULT_STRLEN;
    UINT nInfo = 6/*DEFAULT_STRLEN*/;

    CString strHead = GetString(csHead, nHead, EN_ALIGN_TYPE::LEFT );
    CString strInfo = GetString(csInfo, nInfo, EN_ALIGN_TYPE::RIGHT);

    if (bUpperHead) strHead.MakeUpper();
    if (bUpperInfo) strInfo.MakeUpper();

    CString csHeadInfo;
    csHeadInfo.Format(_T("%s : %s"), strHead, strInfo);

    return csHeadInfo;
}

