// XLHunter.h: interface for the CXLHunter class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(__XLBASE_H__)
#define __XLBASE_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define TAH_LEFT                      2
#define TAH_CENTER                    3
#define TAH_RIGHT                     4
#define TAH_RANGECENTER               7	//선택영역 가운데로

#define TAV_TOP                       1
#define TAV_CENTER                    2
#define TAV_BOTTOM                    3
#define TAV_BOTHCENTER                4	//양쪽맞춤

#define RGB_BLACK					  0X000000
#define RGB_WHITE					  0XFFFFFF
#define RGB_RED						  0X0000FF
#define RGB_GREEN					  0X00FF00
#define RGB_BLUE					  0XFF0000
#define RGB_YELLOW					  0X00FFFF
#define RGB_VIOLET					  0XFF00FF
#define RGB_CYAN					  0XFFFF00

#define PAL_BLACK					  1
#define PAL_WHITE					  2
#define PAL_RED						  3
#define PAL_GREEN					  4
#define PAL_BLUE					  5
#define PAL_YELLOW					  6
#define PAL_VIOLET					  7
#define PAL_CYAN					  8

#define XL_CONTINUOUS                 1
#define XL_DASH                       2

#include "Excel9.h"
#include "HeaderPre.h"

class __MY_EXT_CLASS__ CXlBase  // Copy From XLHunter
{
public:
	CXlBase();
	virtual ~CXlBase();

private:
	Xl_Application oApp;
	Xl_Workbooks oBooks;
	Xl_Workbook oBook;
	Xl_Worksheets oSheets;
	Xl_Worksheet oSheet;
	COleVariant vTrue;
	COleVariant vFalse;
	COleVariant vOpt;
	BOOL bExist;
public:
	BOOL bXlExist();
	void Quit();
	void End();
	void Open(CString sFilename, CString sPassWord);
	void Open(CString sFilename);
	void SaveAs(CString sFilename);
	void SaveAs(CString sBookName, CString sFilename);
	void SaveAs(CString sBookName, CString sFilename, CString sPassWord);
	void AddBook();
	CString GetBookName();
	void AddSheet(CString sSheetName);
	void DeleteSheet(CString sSheetName);
	void MoveSheet(CString sSheetAfter, CString sSheetName);
	void CopySheet(CString sSheetAfter, CString sSheetName);
	void SetVisible(BOOL bVis);
	void SetActiveBook(CString sBookName);
	void SetActiveSheet(CString sSheetName);
	void SetSheetName(CString sSheetNameOld, CString sSheetNameNew);

	double GetValue(CString sCell);

	void SetValue(CString sCell, CString sVal);
	void SetValue(CString sCell, short iVal);
	void SetValue(CString sCell, long lVal);
	void SetValue(CString sCell, double dVal);
	void SetValue(short Row, short Col, CString sVal);
	void SetValue(short Row, short Col, short iVal);
	void SetValue(short Row, short Col, long lVal);
	void SetValue(short Row, short Col, double dVal);

	void SetCellName(CString sCell, CString sCellName);
	CString GetCellName(CString sCell);

	void SetColumnWidth(CString sCell1, CString sCell2, short newValue);
	void SetColumnWidth(short iRow1, short iCol1, short iRow2, short iCol2, short newValue);
	void SetRowHeight(CString sCell1, CString sCell2, short newValue);
	void SetRowHeight(short iRow1, short iCol1, short iRow2, short iCol2, short newValue);
	void SetMergeCell(CString sCell1, CString sCell2);
	void SetMergeCell(short iRow1, short iCol1, short iRow2, short iCol2);
	void SetHorAlign(CString sCell1, CString sCell2, short Align);
	void SetHorAlign(short iRow1, short iCol1, short iRow2, short iCol2, short Align);
	void SetVerAlign(CString sCell1, CString sCell2, short Align);
	void SetVerAlign(short iRow1, short iCol1, short iRow2, short iCol2, short Align);
	void SetFont(CString sCell1, CString sCell2, short Height=9, long Color=0, CString sType=_T("굴림체"), BOOL bBold = FALSE);
	void SetFont(short iRow1, short iCol1, short iRow2, short iCol2, short Height=9, long Color=0, CString sType=_T("굴림체"), BOOL bBold = FALSE);
	void SetFont(CString sCell1, CString sCell2, short Height=9, long Color=0, BOOL bBold = FALSE);
	void SetFont(short iRow1, short iCol1, short iRow2, short iCol2, short Height=9, long Color=0, BOOL bBold = FALSE);
	void SetFont(CString sCell1, CString sCell2, short Height, CString sType);
	void SetBackColor(CString sCell1, CString sCell2, short Color);
	void SetBackColor(short Row1, short Col1, short Row2, short Col2, short Color);
	void SetBorder(CString sCell1, CString sCell2, short LineStyle=1, short Thick=2);
	void SetBorder(short Row1, short Col1, short Row2, short Col2, short LineStyle=1, short Thick=2);
	void SetNumberFormat(CString sCell1, CString sCell2, CString sFormat);
	void SetNumberFormat(short Row1, short Col1, short Row2, short Col2, CString sFormat);
	void CellLine(CString sCell1, CString sCell2, short Direction, short LineStyle=1, short Thick=2, short LineColor=1);
	void CellLine(short Row1, short Col1, short Row2, short Col2, short Direction, short LineStyle=1, short Thick=2, short LineColor=1);
	void CellOutLine(CString sCell1, CString sCell2, short LineStyle=1, short Thick=2, short LineColor=1);
	void CellOutLine(short Row1, short Col1, short Row2, short Col2, short LineStyle=1, short Thick=2, short LineColor=1);
	void LineTo(float X1, float Y1, float X2, float Y2);
	void Copy(CString sCell1, CString sCell2, CString sCell3, CString sCell4);
	void CopyRow(CString sCell1, CString sCell2);
	void CopyPicture(CString sCell, CString sBookName, CString sSheetName, CString sPicName);
	void AddPicture(CString sFilename, float Left, float Top, float Width, float Height);
	void AddPicture(CString sFilename, long Row, long Col, float Width, float Height, float scale=1);
	void InsertCell(CString sCell1, CString sCell2, short iOpt=3);
	void DeletCell(CString sCell1, CString sCell2, short iOpt=3);
	void Clear(CString sCell1, CString sCell2);
	void CellNumber(CString sCell, long &Row, long &Col);

	CString GetCell(short Row, short Col);

};

#include "HeaderPost.h"

#endif // !defined(AFX_XlBase_H__3206071D_F617_4AEC_BA75_435F3F47EE93__INCLUDED_)
