// DGN_DgnPrint.h: interface for the CDGN_DgnPrint class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DGN_DGNPRINT_H__F81E26F8_6C71_4AFD_B6F8_3021874B822C__INCLUDED_)
#define AFX_DGN_DGNPRINT_H__F81E26F8_6C71_4AFD_B6F8_3021874B822C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////
#include "HeaderPre.h"      /////////
/////////////////////////////////////

class __MY_EXT_CLASS__ CDGN_DgnPrint  
{
public:
	CDGN_DgnPrint();
	virtual ~CDGN_DgnPrint();

	static int GetInt(double dValue);
	// S는 부호가 있는 경우, 숫자는 자리수, _1e에서 1은 소수점 이하 자리수
	// Change Scientific format from value.
	static CString Change_S7_1e_S1(double dValue);
	static CString Change_7_1e_S2(double dValue);
	static CString Change_6_1e_S1(double dValue);
	static CString Change_ScientificForm(double dValue);

	static CString Get_S7_1e_S1(double dValue, double dRatio);
	static CString Get_S7_1e_S1_RC(double dValue, double dRatio);
	static CString Get_7_1e_S2(double dValue, double dRatio);
	static CString Get_6_1e_S1(double dValue, double dRatio);
	static CString Get_ScientificForm(int iIndex, int iCount, double dValue, BOOL bSign=FALSE);

    static double Get_ScientificValue(int nDecimal, double dValue);

	static CString GetPrintString(int nLength, const CString& strValue);
};

/////////////////////////////////////
#include "HeaderPost.h"      ////////
/////////////////////////////////////

#endif // !defined(AFX_DGN_DGNPRINT_H__F81E26F8_6C71_4AFD_B6F8_3021874B822C__INCLUDED_)
