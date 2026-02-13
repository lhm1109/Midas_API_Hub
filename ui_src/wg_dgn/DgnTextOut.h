#if !defined(AFX_DGNTEXTOUTT_H__INCLUDED_)
#define AFX_DGNTEXTOUTT_H__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\MIT_Lib\rptostream.h"
#include "DgnTextOutDef.h"

#include "HeaderPre.h"

class __MY_EXT_CLASS__ CDgnTextOut
{
public:
    CDgnTextOut(void);
    virtual ~CDgnTextOut(void);

public:
    void SetUnitFactor();
    void SetFileName(const CString &csFileName);
    CString GetFileName();

    void WriteOutline1Fmt(LPCTSTR Fmt, ...);
    void WriteOutline2Fmt(LPCTSTR Fmt, ...);
    void WriteOutline3Fmt(LPCTSTR Fmt, ...);
    void WriteOutline4Fmt(LPCTSTR Fmt, ...);
    
    
protected:
    void SetParagraphTotalLines(const UINT nRows);
    void SetLineWidthForOutline1(const UINT nWidth=80);

    void WriteLine(const CString& strLineSymbol);

    void WriteOutline1(CString csStr, BOOL bUpperCase=TRUE);
    void WriteOutline1(CString csStr1, CString csStr2, BOOL bUpperCase=TRUE);

    void WriteOutline2(CString csStr);

    void WriteOutline3(CString csStr);
    void WriteOutline3(CString csTitle, CString csInfo,   
        const EN_DIV_TYPE enDivType = EN_DIV_TYPE::DBDOT, UINT nTitleLen = DEFAULT_STRLEN);
    void WriteOutline3(CString csTitle, CString csInfo, CString csNote, 
        const EN_DIV_TYPE enDivType = EN_DIV_TYPE::DBDOT, UINT nTitleLen = DEFAULT_STRLEN);

    void WriteOutline4(CString csStr);
    void WriteOutline4(CString csTitle, CString csInfo,
        const EN_DIV_TYPE enDivType = EN_DIV_TYPE::DBDOT, UINT nTitleLen = DEFAULT_STRLEN);
    void WriteOutline4(CString csTitle, CString csInfo, CString csNote,
        const EN_DIV_TYPE nStrType = EN_DIV_TYPE::DBDOT, UINT nTitleLen = DEFAULT_STRLEN);

    void WriteCode(CString csStr);
    void WriteInfo(CString csTitle, CString csInfo, const EN_DIV_TYPE nStrType = EN_DIV_TYPE::DBDOT,
        UINT nTitleLen = DEFAULT_STRLEN);
    void WriteText(const UINT nSpace, CString csStr);
    void WriteBlank(UINT nRow=1);
    void WriteEquation(CString csStr, const UINT nBlank=0);
    void WriteGnrlInfo(CString csStr, const BOOL bSymbol=TRUE);

    void WriteComment(const CString& strComment);
    void WritePara(const CString& strVal);
    void WriteParaFmt(LPCTSTR Fmt, ...);

    void WriteTextFmt(const UINT unSpace, LPCTSTR Fmt, ...);
    
    void WriteTableRow(const CString& csStr);
    void RemoveDot(CString &rString);

protected:
    double GetCurUnitValue(double dValue, UINT nUnitType);

protected:
    CString GetUnitString(UINT nUnitType, BOOL bUnitOnly=FALSE);
    CString GetString(const CString& csString, UINT nTotalLength, const EN_ALIGN_TYPE enType = EN_ALIGN_TYPE::RIGHT, BOOL bAbb = FALSE);
    CString GetStringValue(double dValue, int iTotal, int iUnder);
    CString GetStringValue(double dValue, int iUnder);
    CString GetCurVal(double dValue, UINT nUnitType, BOOL bAddUnit=TRUE, UINT nTotalLen=DEFAULT_VALUE_LEN, UINT nUnderLen=DEFAULT_VALUE_DEC);
    CString GetCurStr(CString csStr, UINT nTotalLen=DEFAULT_VALUE_LEN); // GetCurVal와 동일한 글자 길이로 출력
    CString GetSpace(UINT nSpace);
    CString GetTableLine(UINT nTableWidth, BOOL bPlusAtEnd=FALSE);
    CString GetLine(UINT nLineLen, CString csText=_T("-"));
    CString GetRho(double dRho);
    CString GetFactor(double dFactor);
    CString GetRatio(double dRatio);
    CString GetStringWithNote(CString csSymbol, double dValue, UINT nUnitType, CString csNote=_T(""));
    CString GetStringWithNote(CString csSymbol, CString csValue, CString csNote=_T(""));
    CString GetStringOKNG(double dRatio, double dCriteria, BOOL bSmallOK=TRUE);
    CString GetGnrlInfo(const CString& csHead, const CString& csInfo, BOOL bUpperHead=TRUE, BOOL bUpperInfo=FALSE);
    CString GetGnrlInfo(const CString& csHead, UINT nInfo, BOOL bUpperHead=TRUE, BOOL bUpperInfo=FALSE);
       

private:
    CString GetDivideStr(const EN_DIV_TYPE enDivType);

private:
    CString m_strFileName;  // 출력 파일명
    CString m_strUserForUnit;
    CString m_strUserLenUnit;

    BOOL m_bInsertPageBreak;
    BOOL m_bBaseCodePrinted;
    UINT m_nLineWidthForOutline1;

    double  m_dFlen; // in   -> User selection unit
    double  m_dFln2; // in^2 -> User selection unit
    double  m_dFln3; // in^3 -> User selection unit
    double  m_dFln4; // in^4 -> User selection unit
    double  m_dFfor; // kips      -> User selection unit
    double  m_dFmot; // kips*in   -> User selection unit
    double  m_dFstr; // kips/in^2 -> User selection unit
    CString m_strLenUnit1;
    CString m_strLenUnit2;
    CString m_strAreaUnit;
    CString m_strLen3Unit;
    CString m_strLen4Unit;
    CString m_strLen5Unit;
    CString m_strLen6Unit;
    CString m_strLn_1Unit;
    CString m_strForUnit;
    CString m_strMomUnit;
    CString m_strStrUnit;
    CString m_strLen4ByFor2Unit;
    CString m_strFor1ByLen6Unit;
    CString m_strFor2Len4Unit;
    // Add by ZINU.('01.10.5). For Trial Mark for Design.
    CString m_strMarkDt, m_strMarkDts, m_strMarkS1, m_strMarkS2;
};

#include "HeaderPost.h"

#endif // !defined(AFX_DGNTEXTOUTT_H__INCLUDED_)