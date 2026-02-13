#include "stdafx.h"
#include "DGN_Graphic_Out.h"

#include "..\wg_db\wg_db_DBLib.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_UnitCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define INDENT_PARA01 55

CDGN_Graphic_Out::CDGN_Graphic_Out(void)
{
    m_dFstr = m_dFmot = m_dFfor = m_dFlen = 1.0;
    m_dFln1 = m_dFln2 = m_dFln3 = m_dFln4 = m_dFfpl = 1.0;

    // Add by ZINU.('03.01.22). For Trial and LAN Mark for Design.
    CDBLib::GetDgnMarkForTrial(m_strMarkG1);
}


CDGN_Graphic_Out::~CDGN_Graphic_Out(void)
{
}

BEGIN_MESSAGE_MAP(CDGN_Graphic_Out, CBCGPStatic)
    //{{AFX_MSG_MAP(CDGN_Graphic_Out)
    // NOTE - the ClassWizard will add and remove mapping macros here.
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CDGN_Graphic_Out::SetUnitFactor(int nBaseF, int nBaseL, int nCodeF, int nCodeL)
{
    /// 설계기준 단위계를 현재 단위계로 변환하여 값을 출력하기 위한 단위변환계수 설정.
    CDBDoc* pDoc = CDBDoc::GetDocPoint();
    m_dFlen = pDoc->m_pUnitCtrl->ConvertSrcUnitData2TgtUnit(nBaseF, nBaseL, nCodeF, nCodeL, D_UNITSYS_BASE_LENGTH, 1.0);
    m_dFfor = pDoc->m_pUnitCtrl->ConvertSrcUnitData2TgtUnit(nBaseF, nBaseL, nCodeF, nCodeL, D_UNITSYS_BASE_FORCE , 1.0);
    m_dFstr = m_dFfor/(m_dFlen*m_dFlen);
    m_dFmot = m_dFfor*m_dFlen;
    m_dFln1 = m_dFlen * CDBLib::GetAs2Factor(nBaseL);  // Add by ZINU.('05.02.23). Add Fln1 (in^2/ft).
    m_dFln2 = pow(m_dFlen, 2.0);
    m_dFln3 = pow(m_dFlen, 3.0);
    m_dFln4 = pow(m_dFlen, 4.0);
    m_dFfpl = m_dFfor/m_dFlen;
}

void CDGN_Graphic_Out::WriteChap(const CString& strVal)
{
    m_poDB->Write_Chapter_Str(strVal);     
}

void CDGN_Graphic_Out::WriteChap(LPCTSTR lpctSTR)
{
    m_poDB->Write_Chapter_Str(lpctSTR); 
}

void CDGN_Graphic_Out::WriteChapFmt(TCHAR* fmt, ...)
{
    va_list argptr;
    va_start(argptr,fmt);

    CString strTemp;
    strTemp.FormatV(fmt,argptr);
    va_end(argptr);

    WriteChap(strTemp); 
}

void CDGN_Graphic_Out::MoveTo(int nx, int ny) 
{
    m_poDB->Write_MoveTo(nx, ny); 
}

void CDGN_Graphic_Out::DrawHeadTable(int nstx, int nedx)
{
    m_poDB->Draw_Solid(55, m_poDB->m_iYpoint, 1600, m_poDB->m_iYpoint-40, 0, RGB(192,192,192));
    m_poDB->m_iYpoint -= 10;
}

void CDGN_Graphic_Out::WriteText(const CString& strVal, BOOL bFixedFont/*=FALSE*/)
{
    m_poDB->Write_Printf((LPTSTR)(LPCTSTR)strVal); 
    if (bFixedFont)
    {
        m_poDB->Chg_FixedFont(); 
    }
}

void CDGN_Graphic_Out::WriteText(LPCTSTR lpctSTR, BOOL bFixedFont/*=FALSE*/)
{
    m_poDB->Write_Printf((TCHAR*)lpctSTR); 
    if (bFixedFont)
    {
        m_poDB->Chg_FixedFont();
    }
}

void CDGN_Graphic_Out::WriteTextFmt(TCHAR* fmt, ...)
{
    va_list argptr;
    va_start(argptr,fmt);

    CString strTemp;
    strTemp.FormatV(fmt,argptr);
    va_end(argptr);

    WriteText(strTemp); 
}

void CDGN_Graphic_Out::WriteTextFmtFixedFont(TCHAR* fmt, ...)
{
    va_list argptr;
    va_start(argptr,fmt);

    CString strTemp;
    strTemp.FormatV(fmt,argptr);
    va_end(argptr);

    WriteText(strTemp); 
    m_poDB->Chg_FixedFont();
}


void CDGN_Graphic_Out::WriteText(int nx, int ny, const CString& strVal, BOOL bFixedFont/*=FALSE*/) 
{
    m_poDB->Write_Printfxy(nx, ny, (LPTSTR)(LPCTSTR)strVal); 
    if (bFixedFont)
    {
        m_poDB->Chg_FixedFont();
    }
}

void CDGN_Graphic_Out::WriteText(int nx, int ny, LPCTSTR lpctSTR, BOOL bFixedFont/*=FALSE*/)
{ 
    m_poDB->Write_Printfxy(nx, ny, (TCHAR*)lpctSTR);
    if (bFixedFont)
    {
        m_poDB->Chg_FixedFont();
    }
}

void CDGN_Graphic_Out::WriteTextFmt(int nx, int ny, TCHAR* fmt, ...)
{ 
    va_list argptr;
    va_start(argptr,fmt);

    CString strTemp;
    strTemp.FormatV(fmt,argptr);
    va_end(argptr);

    m_poDB->Write_Printfxy(nx, ny, strTemp); 
}

void CDGN_Graphic_Out::WriteTextFmtFixedFont(int nx, int ny, TCHAR* fmt, ...)
{ 
    va_list argptr;
    va_start(argptr,fmt);

    CString strTemp;
    strTemp.FormatV(fmt,argptr);
    va_end(argptr);

    m_poDB->Write_Printfxy(nx, ny, strTemp); 
    m_poDB->Chg_FixedFont();
}

void CDGN_Graphic_Out::WriteTextPosX(int nx, const CString& strVal, BOOL bFixedFont/*=FALSE*/)
{ 
    m_poDB->Write_Printfxy(nx, m_poDB->m_iYpoint, (LPTSTR)(LPCTSTR)strVal);
    if (bFixedFont)
    {
        m_poDB->Chg_FixedFont();
    }
} 

void CDGN_Graphic_Out::WriteTextPosX(int nx, LPCTSTR lpctSTR, BOOL bFixedFont/*=FALSE*/)
{ 
    m_poDB->Write_Printfxy(nx, m_poDB->m_iYpoint, (TCHAR*)lpctSTR);
    if (bFixedFont)
    {
        m_poDB->Chg_FixedFont();
    }
}

void CDGN_Graphic_Out::WriteTextPosXFmt(int nx, TCHAR* fmt, ...)
{ 
    va_list argptr;
    va_start(argptr,fmt);

    CString strTemp;
    strTemp.FormatV(fmt,argptr);
    va_end(argptr);

    WriteTextPosX(nx, strTemp);
}

void CDGN_Graphic_Out::WriteTextPosXFmtFixedFont(int nx, TCHAR* fmt, ...)
{ 
    va_list argptr;
    va_start(argptr,fmt);

    CString strTemp;
    strTemp.FormatV(fmt,argptr);
    va_end(argptr);

    WriteTextPosX(nx, strTemp);
    m_poDB->Chg_FixedFont();
}

void CDGN_Graphic_Out::WriteRatio(double dRatio, BOOL bColor/*=TRUE*/)
{
    WriteText(GetRatioString(dRatio));
    if (bColor) { m_poDB->Set_TextColor(GetRatioColor(dRatio)); }
}

void CDGN_Graphic_Out::WriteRatio(int nMBarNum, double dRatio, BOOL bColor/*=TRUE*/)
{
    WriteText(GetRatioString(nMBarNum, dRatio));
    if (bColor) { m_poDB->Set_TextColor(GetRatioColor(dRatio)); }
}

void CDGN_Graphic_Out::WriteRatioPosX(int nPosX, double dRatio, const CString& strAddString, BOOL bColor/*=TRUE*/)
{
    WriteText(nPosX, m_poDB->m_iYpoint, GetRatioString(dRatio) + strAddString);
    if (bColor) { m_poDB->Set_TextColor(GetRatioColor(dRatio)); }
}

void CDGN_Graphic_Out::WriteRatioPosX(int nPosX, double dRatio, double dRatioLim, const CString& strAddString, BOOL bColor/*=TRUE*/)
{
    WriteText(nPosX, m_poDB->m_iYpoint, GetRatioString(dRatio, dRatioLim) + strAddString);
    const double dRat = fabs(dRatioLim) < DGN_ZERO ? 0.0 : dRatio/dRatioLim;
    if (bColor) { m_poDB->Set_TextColor(GetRatioColor(dRat)); }
}

void CDGN_Graphic_Out::WriteRatioPosX(int nPosX, int nMBarNum, double dRatio, BOOL bColor/*=TRUE*/)
{
    WriteText(nPosX, m_poDB->m_iYpoint, GetRatioString(nMBarNum, dRatio));
    if (bColor) { m_poDB->Set_TextColor(GetRatioColor(dRatio)); }
}

void CDGN_Graphic_Out::WriteRatioWithEqualSign(double dRatio, BOOL bColor/*=TRUE*/)
{
    WriteText(_T("= ") + GetRatioString(dRatio));
    if (bColor) { m_poDB->Set_TextColor(GetRatioColor(dRatio)); }
}

void CDGN_Graphic_Out::WriteRatioWithEqualSign(int nMBarNum, double dRatio, BOOL bColor/*=TRUE*/)
{
    WriteText(_T("= ") + GetRatioString(nMBarNum, dRatio));
    if (bColor) { m_poDB->Set_TextColor(GetRatioColor(dRatio)); }
}

void CDGN_Graphic_Out::WriteRatioWithEqualSignPosX(int nPosX, double dRatio, BOOL bColor/*=TRUE*/)
{
    WriteText(nPosX, m_poDB->m_iYpoint, _T("= ") + GetRatioString(dRatio));
    if (bColor) { m_poDB->Set_TextColor(GetRatioColor(dRatio)); }
}

void CDGN_Graphic_Out::WriteRatioWithEqualSignPosX(int nPosX, int nMBarNum, double dRatio, BOOL bColor/*=TRUE*/)
{
    WriteText(nPosX, m_poDB->m_iYpoint,  _T("= ") + GetRatioString(nMBarNum, dRatio));
    if (bColor) { m_poDB->Set_TextColor(GetRatioColor(dRatio)); }
}

CString CDGN_Graphic_Out::fTO7s(double dVal)
{
    CString strVal = _T("");

    if      (dVal <= -10000.0) { strVal.Format(_T("%7.0f"), dVal); }
    else if (dVal <= -1000.0)  { strVal.Format(_T("%7.1f"), dVal); }
    else if (dVal <= -100.0)   { strVal.Format(_T("%7.2f"), dVal); }
    else if (dVal <= -10.0)    { strVal.Format(_T("%7.3f"), dVal); }
    else if (dVal < 0.0)	   { strVal.Format(_T("%7.4f"), dVal); }
    else if (dVal < 10.0)	   { strVal.Format(_T("%7.5f"), dVal); }
    else if (dVal <  100.0)    { strVal.Format(_T("%7.4f"), dVal); }
    else if (dVal <  1000.0)   { strVal.Format(_T("%7.3f"), dVal); }
    else if (dVal <  10000.0)  { strVal.Format(_T("%7.2f"), dVal); }
    else if (dVal <  100000.0) { strVal.Format(_T("%7.1f"), dVal); }
    else                       { strVal.Format(_T("%7.0f"), dVal); }
    //else						{strValue = Change_6_1e_S1(dValue/Ratio);}

    return strVal;
}

CString CDGN_Graphic_Out::GetRatioString(double dRatio)
{
    CString strRatio = _T("");
    if (dRatio > 1.0) { strRatio.Format(_T("%5.3f > 1.000 ....... N.G"), dRatio); }
    else              { strRatio.Format(_T("%5.3f < 1.000 ....... O.K"), dRatio); }	
    return strRatio;
}

CString CDGN_Graphic_Out::GetRatioString(double dRatio, double dRatioLim)
{
    CString strRatio = _T("-");
    if (dRatioLim > 0.0)
    {
        if (dRatio > dRatioLim) { strRatio.Format(_T("%5.3f > %5.3f ....... N.G"), dRatio, dRatioLim); }
        else                    { strRatio.Format(_T("%5.3f < %5.3f ....... O.K"), dRatio, dRatioLim); }
    }
    return strRatio;
}

CString CDGN_Graphic_Out::GetRatioString(int nMBarNum, double dRatio)
{
    if (nMBarNum > 0) { return GetRatioString(dRatio); }

    return _T("***** > 1.0 ....... N.G");
}

COLORREF CDGN_Graphic_Out::GetRatioColor(double dRatio)
{
    return (dRatio > 1.0) ?  RED : BLUE;
}

COLORREF CDGN_Graphic_Out::GetRatioColor(int nMBarNum, double dRatio)
{
    if ( nMBarNum < 1 ) return RED;
    if ( dRatio > 1.0 ) return RED;

    return BLUE;
}
