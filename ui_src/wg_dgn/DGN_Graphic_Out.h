#if !defined(__DGN_Graphic_Out_H__)
#define __DGN_Graphic_Out_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\choga10\wg_choga_ObjDB.h"

#include "HeaderPre.h"

class __MY_EXT_CLASS__ CDGN_Graphic_Out : public CBCGPStatic
{
public:
    CDGN_Graphic_Out(void);
    virtual ~CDGN_Graphic_Out(void);

public:
    CObjDataBase* GetObjDB() { return m_poDB; }
    void SetObjDataBase(CObjDataBase* poDB) { m_poDB = poDB; }

protected:
    void SetUnitFactor(int nBaseF, int nBaseL, int nCodeF, int nCodeL);

protected:
    void WriteChap(const CString& strVal);
    void WriteChap(LPCTSTR lpctSTR);
    void WriteChapFmt(TCHAR* fmt, ...);

protected:	

    void MoveTo(int nx, int ny);
    void SetTextColor(COLORREF color) { m_poDB->Set_TextColor(color); }
    void SetTextBold() { m_poDB->Set_Bold_Font(FW_BOLD); }
    void DrawHeadTable(int nstx, int nedx);

    void WriteText(const CString& strVal, BOOL bFixedFont=FALSE);
    void WriteText(LPCTSTR lpctSTR, BOOL bFixedFont=FALSE);
    void WriteTextFmt(TCHAR* fmt, ...);
    void WriteTextFmtFixedFont(TCHAR* fmt, ...);

    void WriteText(int nPosX, int nPosY, const CString& strVal, BOOL bFixedFont=FALSE);
    void WriteText(int nPosX, int nPosY, LPCTSTR lpctSTR, BOOL bFixedFont=FALSE);
    void WriteTextFmt(int nPosX, int nPosY, TCHAR* fmt, ...);
    void WriteTextFmtFixedFont(int nPosX, int nPosY, TCHAR* fmt, ...);

    void WriteTextPosX(int nPosX, const CString& strVal, BOOL bFixedFont=FALSE);
    void WriteTextPosX(int nPosX, LPCTSTR lpctSTR, BOOL bFixedFont=FALSE);
    void WriteTextPosXFmt(int nPosX, TCHAR* fmt, ...);
    void WriteTextPosXFmtFixedFont(int nPosX, TCHAR* fmt, ...);

    void WriteRatio(double dRatio, BOOL bColor=TRUE);
    void WriteRatio(int nMBarNum, double dRatio, BOOL bColor=TRUE);
    void WriteRatioPosX(int nPosX, double dRatio, const CString& strAddString=_T(""), BOOL bColor=TRUE);
    void WriteRatioPosX(int nPosX, double dRatio, double dRatioLim, const CString& strAddString=_T(""), BOOL bColor=TRUE);
    void WriteRatioPosX(int nPosX, int nMBarNum, double dRatio, BOOL bColor=TRUE);

    void WriteRatioWithEqualSign(double dRatio, BOOL bColor=TRUE);
    void WriteRatioWithEqualSign(int nMBarNum, double dRatio, BOOL bColor=TRUE);
    void WriteRatioWithEqualSignPosX(int nPosX, double dRatio, BOOL bColor=TRUE);
    void WriteRatioWithEqualSignPosX(int nPosX, int nMBarNum, double dRatio, BOOL bColor=TRUE);


    CString fTO7s(double dVal);
    CString GetRatioString(double dRatio);
    CString GetRatioString(double dRatio, double dRatioLim);
    CString GetRatioString(int nMBarNum, double dRatio);
    COLORREF GetRatioColor(double dRatio);
    COLORREF GetRatioColor(int nMBarNum, double dRatio);

    int GetPosX() { return m_poDB->m_iXpoint; }
    int GetPosY() { return m_poDB->m_iYpoint; }

    void SetPosX(int nx) { m_poDB->m_iXpoint = nx; }
    void SetPosY(int ny) { m_poDB->m_iYpoint = ny; }	
        
protected:
    //BOOL m_bViProp;
    double m_dFstr, m_dFmot, m_dFfor, m_dFlen, m_dFln1, m_dFln2, m_dFln3, m_dFln4, m_dFfpl;
    CString m_strMarkG1; // Add by ZINU.('03.01.24). For Trial and LAN Mark for Design.

    CObjDataBase* m_poDB;

protected:
    //{{AFX_MSG(CDGN_Graphic_Out)
    // NOTE - the ClassWizard will add and remove member functions here.
    //}}AFX_MSG

    DECLARE_MESSAGE_MAP()

};
#include "HeaderPost.h"

#endif // !defined(__DGN_Graphic_Out_H__)