#if !defined(__XLADVANCE_H__)
#define __XLADVANCE_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XlBase.h"
#include "HeaderPre.h"

struct T_CellLine
{
	int iDirection;
	int iCellLineStyle;
	int iCellLineThick;
	int iCellLineColor;

	void Init()
	{
		iDirection=1; 
		iCellLineStyle=1; 
		iCellLineThick=2; 
		iCellLineColor=1;
	}

	T_CellLine& operator=(T_CellLine& src)
	{
		iDirection=src.iDirection;
		iCellLineStyle=src.iCellLineStyle;
		iCellLineThick=src.iCellLineThick;
		iCellLineColor=src.iCellLineColor;
		return *this;
	}

	T_CellLine()
	{
	}

	T_CellLine(T_CellLine& src)
	{
		*this = src;
	}
};

class __MY_EXT_CLASS__ CXl_ACellBase
{
public:
	CXl_ACellBase();
	virtual ~CXl_ACellBase();
	CXl_ACellBase(CXl_ACellBase& src);
	CXl_ACellBase& operator=(CXl_ACellBase& src);

public:
	// Cell 형식
	BOOL m_bSetNumberFormat;
	CString m_NumberFormatLocal; //Text : @ , Number : 0.00_
	// 수직 정렬
	BOOL m_bSetVAlignment;
	int m_iVAlignment;
	//수평 정렬
	BOOL m_bSetHAlignment;
	int m_iHAlignment;
	// Cell Color
	BOOL m_bSetCellColor;
	int m_iCellColorIndex;

	BOOL m_bSetFont;
	int m_iFontHeight; long m_lFontColor; CString m_sFontType; BOOL m_bFontBold;

	CArray<T_CellLine, T_CellLine&> m_arCellLine;

	BOOL m_bSetBorder;
	int m_iBorderLineStyle, m_iBorderThick;

	BOOL m_bSetCellOutLine;
	int m_iCellOutLineStyle, m_iCellOutLineThick, m_iCellOutLineColor;

};

class __MY_EXT_CLASS__ CXl_ACell : public CXl_ACellBase
{
public:
	CXl_ACell();
	virtual ~CXl_ACell();
	CXl_ACell(CXl_ACell& src);
	CXl_ACell& operator=(CXl_ACell& src);
public:
	int m_iRow;
	int m_iCol;
protected:
	int     m_iValueType; //0:String, 1:double, 2:long, 3:short
	CString m_strValue;
	long    m_lValue;
	double  m_dValue;
	short   m_sValue;

public:
	void SetValue(double dValue);
	void SetValue(long lValue);
	void SetValue(short sValue);
	void SetValue(CString strValue);
	BOOL WriteCell(CXlBase* pXl);

};

class __MY_EXT_CLASS__ CXl_ACells : public CXl_ACellBase
{
public:
	CXl_ACells();
	virtual ~CXl_ACells();
	CXl_ACells(CXl_ACells& src);
	CXl_ACells& operator=(CXl_ACells& src);
protected:
	int     m_iValueType; //0:String, 1:double, 2:long, 3:short
	CString m_strValue;
	long    m_lValue;
	double  m_dValue;
	short   m_sValue;

public:
	int m_iSRow, m_iERow;
	int m_iSCol, m_iECol;
	BOOL m_bMerge;

	CArray<CXl_ACell, CXl_ACell&> m_arCell;

public:
	void SetValue(double dValue);
	void SetValue(long lValue);
	void SetValue(short sValue);
	void SetValue(CString strValue);
	BOOL WriteCells(CXlBase* pXl);

};

class __MY_EXT_CLASS__ CXl_ATable : public CXl_ACellBase
{
public:
	CXl_ATable();
	virtual ~CXl_ATable();
	CXl_ATable(CXl_ATable& src);
	CXl_ATable& operator=(CXl_ATable& src);

public:
	int m_iTableSRow, m_iTableERow;
	int m_iTableSCol, m_iTableECol;

	//Header Title
	CArray<CXl_ACells, CXl_ACells&>	m_arHeader;

	//Data
	CArray<CXl_ACells, CXl_ACells&>	m_arData;

public:
	BOOL WriteTable(CXlBase* pXl);

};

#include "HeaderPost.h"

#endif // !defined(AFX_XLADVANCE_H__3206071D_F617_4AEC_BA75_435F3F47EE93__INCLUDED_)
