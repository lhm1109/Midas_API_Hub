// XlBase.cpp: implementation of the CXlBase class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "wg_dgn.h"
#include "XlBase.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CXlBase::CXlBase()
{
	bExist = TRUE;
//    _Application oApp;
	vTrue = COleVariant((short)TRUE);
	vFalse = COleVariant((short)FALSE);
	vOpt = COleVariant((long)DISP_E_PARAMNOTFOUND, VT_ERROR);
/*Thread로 만들경우 엑셀작업 시작하는 곳에 삽입할 것
	if(!AfxOleInit())
	{
		AfxMessageBox(_T("Could not initialize COM dll"));
		return;
	}
//*/
	if(!oApp.CreateDispatch(_T("Excel.Application"), NULL))
	{
		bExist = FALSE;
		//AfxMessageBox(_T("Cannot start Excel"));
		return;
	}

		//Start a new workbook
	LPDISPATCH lpDisp;
	lpDisp = oApp.GetWorkbooks();
	ASSERT(lpDisp);
	oBooks.AttachDispatch(lpDisp);
	oApp.SetDisplayAlerts(FALSE);	//확인 대화상자를 열지않고 실행
}

CXlBase::~CXlBase()
{
	oApp.Quit();

}

BOOL CXlBase::bXlExist()
{
	return bExist;
}

void CXlBase::Quit()
{
//	oBook.Close(vFalse,vOpt,vOpt);
//	oApp.ReleaseDispatch();
	oApp.Quit();
}

void CXlBase::End()
{
	oApp = NULL;
}

//ex) sFilename - C:\\WORK\\test3.xls
void CXlBase::Open(CString sFilename)
{
	LPDISPATCH lpDisp;
	lpDisp = oBooks.Open(sFilename,vOpt,vOpt,vOpt,vOpt,vOpt,vOpt,vOpt,vOpt,vOpt,vOpt,vOpt,vOpt);
	ASSERT(lpDisp);
	oBook.AttachDispatch(lpDisp);
}

void CXlBase::Open(CString sFilename, CString sPassWord)
{
	LPDISPATCH lpDisp;
	lpDisp = oBooks.Open(sFilename,vOpt,vOpt,vOpt,COleVariant(sPassWord),vOpt,vOpt,vOpt,vOpt,vOpt,vOpt,vOpt,vOpt);
	ASSERT(lpDisp);
	oBook.AttachDispatch(lpDisp);
}

void CXlBase::SaveAs(CString sFilename)
{
	oBook.SetSaved(TRUE);

	try
	{
		oBook.SaveAs(COleVariant(sFilename),vOpt,vOpt,vOpt,vOpt,vOpt,0,vOpt,vOpt,vOpt,vOpt);
	}
	catch(CException* e)
	{
		CString sMsg;
		sMsg = sFilename + _T("로 저장합니다.");
//		AfxMessageBox(sMsg);

		oBook.SaveAs(COleVariant(sFilename),vOpt,vOpt,vOpt,vOpt,vOpt,0,vOpt,vOpt,vOpt,vOpt);

		e->Delete();
	}
	catch(...)
	{
	}

}

void CXlBase::SaveAs(CString sBookName, CString sFilename)
{
	oBook = oBooks.GetItem(COleVariant(sBookName));
	oBook.SetSaved(TRUE);

	try
	{
		oBook.SaveAs(COleVariant(sFilename),vOpt,vOpt,vOpt,vOpt,vOpt,0,vOpt,vOpt,vOpt,vOpt);
	}
	catch(CException* e)
	{
		CString sMsg;
		sMsg = sFilename + _T("로 저장합니다.");
//		AfxMessageBox(sMsg);

		oBook.SaveAs(COleVariant(sFilename),vOpt,vOpt,vOpt,vOpt,vOpt,0,vOpt,vOpt,vOpt,vOpt);

		e->Delete();
	}
	catch(...)
	{
	}

}

void CXlBase::SaveAs(CString sBookName, CString sFilename, CString sPassWord)
{
	oBook = oBooks.GetItem(COleVariant(sBookName));
	oBook.SetSaved(TRUE);

	try
	{
		oBook.SaveAs(COleVariant(sFilename),vOpt,COleVariant(sPassWord),vOpt,vOpt,vOpt,0,vOpt,vOpt,vOpt,vOpt);
	}
	catch(CException* e)
	{
		CString sMsg;
		sMsg = sFilename + _T("로 저장합니다.");
		oBook.SaveAs(COleVariant(sFilename),vOpt,COleVariant(sPassWord),vOpt,vOpt,vOpt,0,vOpt,vOpt,vOpt,vOpt);

		e->Delete();
	}
	catch(...)
	{
	}
}

void CXlBase::AddBook()
{
	LPDISPATCH lpDisp;

	lpDisp = oBooks.Add(vOpt);
	ASSERT(lpDisp);
	oBook.AttachDispatch(lpDisp);
}

void CXlBase::AddSheet(CString sSheetName)
{
	LPDISPATCH lpDisp;

	lpDisp = oBook.GetSheets();
	ASSERT(lpDisp);
		oSheets.AttachDispatch(lpDisp);

	long SheetSu = oSheets.GetCount();
	lpDisp = oSheets.GetItem(COleVariant((long)SheetSu));
		oSheet.AttachDispatch(lpDisp);
	CString sLastSheet = oSheet.GetName();

//	lpDisp = oSheets.Add(vOpt,COleVariant(sLastSheet),COleVariant((short)1),COleVariant((short)1));
	lpDisp = oSheets.Add(vOpt,vOpt,COleVariant((short)1),vOpt);//수정요망
		oSheet.AttachDispatch(lpDisp);
	oSheet.SetName(sSheetName);
}

void CXlBase::DeleteSheet(CString sSheetName)
{
	LPDISPATCH lpDisp;

	lpDisp = oBook.GetSheets();
	ASSERT(lpDisp);
	oSheets.AttachDispatch(lpDisp);

	lpDisp = oSheets.GetItem(COleVariant(sSheetName));
	ASSERT(lpDisp);
	oSheet.AttachDispatch(lpDisp);
	oSheet.Delete();
}

//sSheet를 sSheetAfter뒤로
void CXlBase::MoveSheet(CString sSheetAfter, CString sSheetName)
{
	LPDISPATCH lpDisp;
	Xl_Worksheet tempSheet;

	lpDisp = oBook.GetSheets();
	ASSERT(lpDisp);
		oSheets.AttachDispatch(lpDisp);

	long nSheetSu = oSheets.GetCount();
    short i = 0;
	for(i=1; i<=nSheetSu; i++)
	{
		lpDisp = oSheets.GetItem(COleVariant((short)i));
		ASSERT(lpDisp);
		tempSheet.AttachDispatch(lpDisp);
		CString cs = tempSheet.GetName();
		if(cs==sSheetAfter) break;
	}

	VARIANT Var;
	Var.vt = VT_DISPATCH;
	Var.pdispVal = lpDisp;
	oSheet.Move(vOpt,Var);

	lpDisp = oSheets.GetItem(COleVariant((short)i));
	oSheet.AttachDispatch(lpDisp);
	oSheet.SetName(sSheetName);
}

//sSheet를 sSheetAfter뒤로
void CXlBase::CopySheet(CString sSheetAfter, CString sSheetName)
{
	LPDISPATCH lpDisp;
	Xl_Worksheet tempSheet;

	lpDisp = oBook.GetSheets();
	ASSERT(lpDisp);
		oSheets.AttachDispatch(lpDisp);

	long nSheetSu = oSheets.GetCount();
    short i=0;
	for(i=1; i<=nSheetSu; i++)
	{
		lpDisp = oSheets.GetItem(COleVariant((short)i));
		ASSERT(lpDisp);
		tempSheet.AttachDispatch(lpDisp);
		CString cs = tempSheet.GetName();
		if(cs==sSheetAfter) break;
	}

	VARIANT Var;
	Var.vt = VT_DISPATCH;
	Var.pdispVal = lpDisp;
	oSheet.Copy(vOpt,Var);

	short j = i+1;
	lpDisp = oSheets.GetItem(COleVariant((short)j));
	oSheet.AttachDispatch(lpDisp);
	oSheet.SetName(sSheetName);
}

void CXlBase::SetVisible(BOOL bVis)
{
	oApp.SetVisible(bVis);
}

void CXlBase::SetActiveBook(CString sBookName)
{
	LPDISPATCH lpDisp;

	lpDisp = oBooks.GetItem(COleVariant(sBookName));
	ASSERT(lpDisp);
		oBook.AttachDispatch(lpDisp);
}

void CXlBase::SetActiveSheet(CString sSheetName)
{
	LPDISPATCH lpDisp;
	lpDisp = oBook.GetSheets();

	ASSERT(lpDisp);
	oSheets.AttachDispatch(lpDisp);

	lpDisp = oSheets.GetItem(COleVariant(sSheetName));
	ASSERT(lpDisp);
	oSheet.AttachDispatch(lpDisp);
}

void CXlBase::SetSheetName(CString sSheetNameOld, CString sSheetNameNew)
{
	LPDISPATCH lpDisp;

	lpDisp = oBook.GetSheets();
	ASSERT(lpDisp);
		oSheets.AttachDispatch(lpDisp);

	lpDisp = oSheets.GetItem(COleVariant(sSheetNameOld));
	ASSERT(lpDisp);
		oSheet.AttachDispatch(lpDisp);
	oSheet.SetName(sSheetNameNew);
}



CString CXlBase::GetBookName()
{
	CString sName = oBook.GetName();
	return sName;
}

double CXlBase::GetValue(CString sCell)
{
	LPDISPATCH lpDisp;
	Xl_Range oRange;
	lpDisp = oSheet.GetRange(COleVariant(sCell),vOpt);
	ASSERT(lpDisp);
	oRange.AttachDispatch(lpDisp);

	VARIANT var;
	var = oRange.GetValue();
	double dVal = (double)var.dblVal;

	return dVal;
}

void CXlBase::SetValue(CString sCell, CString sVal)
{
	LPDISPATCH lpDisp;
	Xl_Range oRange;
	lpDisp = oSheet.GetRange(COleVariant(sCell),COleVariant(sCell));
	ASSERT(lpDisp);
	oRange.AttachDispatch(lpDisp);
	oRange.SetValue(COleVariant(sVal));
}

void CXlBase::SetValue(CString sCell, short iVal)
{
	LPDISPATCH lpDisp;
	Xl_Range oRange;
	lpDisp = oSheet.GetRange(COleVariant(sCell),vOpt);
	ASSERT(lpDisp);
	oRange.AttachDispatch(lpDisp);
	oRange.SetValue(COleVariant((short) iVal));
}

void CXlBase::SetValue(CString sCell, long lVal)
{
	LPDISPATCH lpDisp;
	Xl_Range oRange;
	lpDisp = oSheet.GetRange(COleVariant(sCell),vOpt);
	ASSERT(lpDisp);
	oRange.AttachDispatch(lpDisp);
	oRange.SetValue(COleVariant((long) lVal));
}

void CXlBase::SetValue(CString sCell, double dVal)
{
	LPDISPATCH lpDisp;
	Xl_Range oRange;
	lpDisp = oSheet.GetRange(COleVariant(sCell),vOpt);
	ASSERT(lpDisp);
	oRange.AttachDispatch(lpDisp);
	oRange.SetValue(COleVariant((double) dVal));
}

void CXlBase::SetValue(short Row, short Col, CString sVal)
{
	LPDISPATCH lpDisp;
	Xl_Range oRange;
	CString sCell = GetCell(Row, Col);
	lpDisp = oSheet.GetRange(COleVariant(sCell),vOpt);
	ASSERT(lpDisp);
	oRange.AttachDispatch(lpDisp);
	oRange.SetValue(COleVariant(sVal));
}

void CXlBase::SetValue(short Row, short Col, short iVal)
{
	LPDISPATCH lpDisp;
	Xl_Range oRange;
	CString sCell = GetCell(Row, Col);
	lpDisp = oSheet.GetRange(COleVariant(sCell),vOpt);
	ASSERT(lpDisp);
	oRange.AttachDispatch(lpDisp);
	oRange.SetValue(COleVariant((short) iVal));
}

void CXlBase::SetValue(short Row, short Col, long lVal)
{
	LPDISPATCH lpDisp;
	Xl_Range oRange;
	CString sCell = GetCell(Row, Col);
	lpDisp = oSheet.GetRange(COleVariant(sCell),vOpt);
	ASSERT(lpDisp);
	oRange.AttachDispatch(lpDisp);
	oRange.SetValue(COleVariant((long) lVal));
}

void CXlBase::SetValue(short Row, short Col, double dVal)
{
	LPDISPATCH lpDisp;
	Xl_Range oRange;
	CString sCell = GetCell(Row, Col);
	lpDisp = oSheet.GetRange(COleVariant(sCell),vOpt);
	ASSERT(lpDisp);
	oRange.AttachDispatch(lpDisp);
	oRange.SetValue(COleVariant((double) dVal));
}

void CXlBase::SetCellName(CString sCell, CString sCellName)
{
	LPDISPATCH lpDisp;
	Xl_Range oRange;
	lpDisp = oSheet.GetRange(COleVariant(sCell),vOpt);
	ASSERT(lpDisp);
	oRange.AttachDispatch(lpDisp);
	oRange.SetName(COleVariant(sCellName));
}

CString CXlBase::GetCellName(CString sCell)
{
	LPDISPATCH lpDisp;
	Xl_Range oRange;
	lpDisp = oSheet.GetRange(COleVariant(sCell),vOpt);
	ASSERT(lpDisp);
	oRange.AttachDispatch(lpDisp);

	VARIANT var;
	var = oRange.GetName();
	CString sCellName = (CString)var.bstrVal;

	return sCellName;
}

void CXlBase::SetColumnWidth(CString sCell1, CString sCell2, short newValue)
{
	LPDISPATCH lpDisp;
	Xl_Range oRange;
	lpDisp = oSheet.GetRange(COleVariant(sCell1),COleVariant(sCell2));
	ASSERT(lpDisp);
	oRange.AttachDispatch(lpDisp);
	oRange.SetColumnWidth(COleVariant((short)newValue));
}

void CXlBase::SetColumnWidth(short iRow1, short iCol1, short iRow2, short iCol2, short newValue)
{
	LPDISPATCH lpDisp;
	Xl_Range oRange;
	CString sCell1 = GetCell(iRow1, iCol1);
	CString sCell2 = GetCell(iRow2, iCol2);
	lpDisp = oSheet.GetRange(COleVariant(sCell1),COleVariant(sCell2));
	ASSERT(lpDisp);
	oRange.AttachDispatch(lpDisp);
	oRange.SetColumnWidth(COleVariant((short)newValue));
}

void CXlBase::SetRowHeight(CString sCell1, CString sCell2, short newValue)
{
	LPDISPATCH lpDisp;
	Xl_Range oRange;
	lpDisp = oSheet.GetRange(COleVariant(sCell1),COleVariant(sCell2));
	ASSERT(lpDisp);
	oRange.AttachDispatch(lpDisp);
	oRange.SetRowHeight(COleVariant((short)newValue));
}

void CXlBase::SetRowHeight(short iRow1, short iCol1, short iRow2, short iCol2, short newValue)
{
	LPDISPATCH lpDisp;
	Xl_Range oRange;
	CString sCell1 = GetCell(iRow1, iCol1);
	CString sCell2 = GetCell(iRow2, iCol2);
	lpDisp = oSheet.GetRange(COleVariant(sCell1),COleVariant(sCell2));
	ASSERT(lpDisp);
	oRange.AttachDispatch(lpDisp);
	oRange.SetRowHeight(COleVariant((short)newValue));
}

void CXlBase::SetMergeCell(CString sCell1, CString sCell2)
{
	LPDISPATCH lpDisp;
	Xl_Range oRange;
	lpDisp = oSheet.GetRange(COleVariant(sCell1),COleVariant(sCell2));
	ASSERT(lpDisp);
	oRange.AttachDispatch(lpDisp);
	VARIANT newValue = oRange.GetHeight();
	oRange.SetMergeCells(newValue);
}

void CXlBase::SetMergeCell(short iRow1, short iCol1, short iRow2, short iCol2)
{
	CString sCell1=_T(""), sCell2=_T("");
	sCell1 = GetCell(iRow1, iCol1);
	sCell2 = GetCell(iRow2, iCol2);
	SetMergeCell(sCell1, sCell2);
}

void CXlBase::SetHorAlign(CString sCell1, CString sCell2, short Align)
{
	LPDISPATCH lpDisp;
	Xl_Range oRange;
	lpDisp = oSheet.GetRange(COleVariant(sCell1),COleVariant(sCell2));
	ASSERT(lpDisp);
	oRange.AttachDispatch(lpDisp);
	oRange.SetHorizontalAlignment(COleVariant((short)Align));
}

void CXlBase::SetHorAlign(short iRow1, short iCol1, short iRow2, short iCol2, short Align)
{
	LPDISPATCH lpDisp;
	Xl_Range oRange;
	CString sCell1 = GetCell(iRow1, iCol1);
	CString sCell2 = GetCell(iRow2, iCol2);
	lpDisp = oSheet.GetRange(COleVariant(sCell1),COleVariant(sCell2));
	ASSERT(lpDisp);
	oRange.AttachDispatch(lpDisp);
	oRange.SetHorizontalAlignment(COleVariant((short)Align));
}

void CXlBase::SetVerAlign(CString sCell1, CString sCell2, short Align)
{
	LPDISPATCH lpDisp;
	Xl_Range oRange;
	lpDisp = oSheet.GetRange(COleVariant(sCell1),COleVariant(sCell2));
	ASSERT(lpDisp);
	oRange.AttachDispatch(lpDisp);
	oRange.SetVerticalAlignment(COleVariant((short)Align));
}

void CXlBase::SetVerAlign(short iRow1, short iCol1, short iRow2, short iCol2, short Align)
{
	LPDISPATCH lpDisp;
	Xl_Range oRange;
	CString sCell1 = GetCell(iRow1, iCol1);
	CString sCell2 = GetCell(iRow2, iCol2);
	lpDisp = oSheet.GetRange(COleVariant(sCell1),COleVariant(sCell2));
	ASSERT(lpDisp);
	oRange.AttachDispatch(lpDisp);
	oRange.SetVerticalAlignment(COleVariant((short)Align));
}

void CXlBase::SetFont(CString sCell1, CString sCell2, short Height, long Color, CString sType, BOOL bBold)
{
	LPDISPATCH lpDisp;
	Xl_Range oRange;
	lpDisp = oSheet.GetRange(COleVariant(sCell1),COleVariant(sCell2));
	ASSERT(lpDisp);
	oRange.AttachDispatch(lpDisp);

	Xl_Font font = oRange.GetFont();
	font.SetSize(COleVariant((short)Height));
	font.SetColorIndex(COleVariant((long)Color));
	font.SetName(COleVariant(sType));

	if(bBold)
		font.SetBold(vTrue);
	else
		font.SetBold(vFalse);
}

void CXlBase::SetFont(short iRow1, short iCol1, short iRow2, short iCol2, short Height, long Color, CString sType, BOOL bBold)
{
	CString sCell1 = GetCell(iRow1, iCol1);
	CString sCell2 = GetCell(iRow2, iCol2);
	SetFont(sCell1, sCell2, Height, Color, sType, bBold);
}

void CXlBase::SetFont(CString sCell1, CString sCell2, short Height, long Color, BOOL bBold)
{
	LPDISPATCH lpDisp;
	Xl_Range oRange;
	lpDisp = oSheet.GetRange(COleVariant(sCell1),COleVariant(sCell2));
	ASSERT(lpDisp);
	oRange.AttachDispatch(lpDisp);

	Xl_Font font = oRange.GetFont();
	font.SetSize(COleVariant((short)Height));
	font.SetColorIndex(COleVariant((long)Color));

	if(bBold)
		font.SetBold(vTrue);
	else
		font.SetBold(vFalse);
}

void CXlBase::SetFont(short iRow1, short iCol1, short iRow2, short iCol2, short Height, long Color, BOOL bBold)
{
	CString sCell1 = GetCell(iRow1, iCol1);
	CString sCell2 = GetCell(iRow2, iCol2);
	SetFont(sCell1, sCell2, Height, Color, bBold);
}

void CXlBase::SetFont(CString sCell1, CString sCell2, short Height, CString sType)
{
	LPDISPATCH lpDisp;
	Xl_Range oRange;
	lpDisp = oSheet.GetRange(COleVariant(sCell1),COleVariant(sCell2));
	ASSERT(lpDisp);
	oRange.AttachDispatch(lpDisp);

	Xl_Font font = oRange.GetFont();
	font.SetSize(COleVariant((short)Height));
//	font.SetColorIndex(COleVariant((long)Color));
	font.SetName(COleVariant(sType));
/*
	if(bBold)
		font.SetBold(vTrue);
	else
		font.SetBold(vFalse);
*/
}

void CXlBase::SetBackColor(CString sCell1, CString sCell2, short Color)
{
	LPDISPATCH lpDisp;
	Xl_Range oRange;
	lpDisp = oSheet.GetRange(COleVariant(sCell1),COleVariant(sCell2));
	ASSERT(lpDisp);
	oRange.AttachDispatch(lpDisp);

	Xl_Interior interior = oRange.GetInterior();
	interior.SetColorIndex(COleVariant((short)Color));
}

void CXlBase::SetBackColor(short Row1, short Col1, short Row2, short Col2, short Color)
{
	LPDISPATCH lpDisp;
	Xl_Range oRange;
	CString sCell1 = GetCell(Row1, Col1);
	CString sCell2 = GetCell(Row2, Col2);
	lpDisp = oSheet.GetRange(COleVariant(sCell1),COleVariant(sCell2));
	ASSERT(lpDisp);
	oRange.AttachDispatch(lpDisp);

	Xl_Interior interior = oRange.GetInterior();
	interior.SetColorIndex(COleVariant((short)Color));
}

void CXlBase::SetBorder(CString sCell1, CString sCell2, short LineStyle, short Thick)
{
	LPDISPATCH lpDisp;
	Xl_Range oRange;
	lpDisp = oSheet.GetRange(COleVariant(sCell1),COleVariant(sCell2));
	ASSERT(lpDisp);
	oRange.AttachDispatch(lpDisp);

	Xl_Borders borders = oRange.GetBorders();
	borders.SetLineStyle(COleVariant((short)LineStyle));
	borders.SetWeight(COleVariant((short)Thick));
}

void CXlBase::SetBorder(short Row1, short Col1, short Row2, short Col2, short LineStyle, short Thick)
{
	CString sCell1 = GetCell(Row1, Col1);
	CString sCell2 = GetCell(Row2, Col2);
	SetBorder(sCell1, sCell2, LineStyle, Thick);
}

void CXlBase::SetNumberFormat(CString sCell1, CString sCell2, CString sFormat)
{
	LPDISPATCH lpDisp;
	Xl_Range oRange;
	lpDisp = oSheet.GetRange(COleVariant(sCell1),COleVariant(sCell2));
	ASSERT(lpDisp);
	oRange.AttachDispatch(lpDisp);
	oRange.SetNumberFormat(COleVariant(sFormat));
}

void CXlBase::SetNumberFormat(short Row1, short Col1, short Row2, short Col2, CString sFormat)
{
	LPDISPATCH lpDisp;
	Xl_Range oRange;
	CString sCell1 = GetCell(Row1, Col1);
	CString sCell2 = GetCell(Row2, Col2);
	lpDisp = oSheet.GetRange(COleVariant(sCell1),COleVariant(sCell2));
	ASSERT(lpDisp);
	oRange.AttachDispatch(lpDisp);
	oRange.SetNumberFormat(COleVariant(sFormat));
}

//Direction  1: 모든셀의 좌측, 2: 모든셀의 우측, 3: 모든셀의 top, 4: 모든셀의 bottom, 5: 대각선(＼), 6: 대각선(／)
//           7: 선택영역의 맨 좌측, 8: 선택영역의 top, 9: 선택영역의 bottom, 10: 선택영역의 맨 우측, 11: 선택영역의 가운테 선만(가로), 12: 선택영역의 가운테 선만(세로)
void CXlBase::CellLine(CString sCell1, CString sCell2, short Direction, short LineStyle, short Thick, short LineColor)
{
	LPDISPATCH lpDisp;
	Xl_Range oRange;
	lpDisp = oSheet.GetRange(COleVariant(sCell1),COleVariant(sCell2));
	ASSERT(lpDisp);
	oRange.AttachDispatch(lpDisp);

	Xl_Border border;
	Xl_Borders borders;
	lpDisp = oRange.GetBorders();
	borders.AttachDispatch(lpDisp);

	lpDisp = borders.GetItem(Direction);
	border.AttachDispatch(lpDisp);
	border.SetLineStyle(COleVariant((short)LineStyle));
	border.SetWeight(COleVariant((short)Thick));
	border.SetColorIndex(COleVariant((short)LineColor));
}

void CXlBase::CellLine(short Row1, short Col1, short Row2, short Col2, short Direction, short LineStyle, short Thick, short LineColor)
{
	CString sCell1 = GetCell(Row1, Col1);
	CString sCell2 = GetCell(Row2, Col2);

	CellLine(sCell1, sCell2, Direction, LineStyle, Thick, LineColor);
}

void CXlBase::CellOutLine(CString sCell1, CString sCell2, short LineStyle, short Thick, short LineColor)
{
	LPDISPATCH lpDisp;
	Xl_Range oRange;
	lpDisp = oSheet.GetRange(COleVariant(sCell1),COleVariant(sCell2));
	ASSERT(lpDisp);
	oRange.AttachDispatch(lpDisp);

	oRange.BorderAround(COleVariant((short)LineStyle),Thick,LineColor,COleVariant((short)1));
}

void CXlBase::CellOutLine(short Row1, short Col1, short Row2, short Col2, short LineStyle, short Thick, short LineColor)
{
	CString sCell1 = GetCell(Row1, Col1);
	CString sCell2 = GetCell(Row2, Col2);

	CellOutLine(sCell1, sCell2, LineStyle, Thick, LineColor);
}

void CXlBase::LineTo(float X1, float Y1, float X2, float Y2)
{
	LPDISPATCH lpDisp;
	Xl_Shapes shapes;
	lpDisp = oSheet.GetShapes();
	ASSERT(lpDisp);
	shapes.AttachDispatch(lpDisp);

	Xl_Line line;
	line = shapes.AddLine(X1,Y1,X2,Y2);
}

//copy:sCell1,sCell2   paste:sCell3,sCell4
void CXlBase::Copy(CString sCell1, CString sCell2, CString sCell3, CString sCell4)
{
	LPDISPATCH lpDisp, lpPaste;
	Xl_Range oRange;
	lpDisp = oSheet.GetRange(COleVariant(sCell1),COleVariant(sCell2));
	ASSERT(lpDisp);
	oRange.AttachDispatch(lpDisp);

	oRange.Copy(vOpt);

	lpPaste = oSheet.GetRange(COleVariant(sCell3),COleVariant(sCell4));
	ASSERT(lpPaste);
	oRange.AttachDispatch(lpPaste);

		VARIANT Var;
	Var.vt = VT_DISPATCH;
	Var.pdispVal = lpPaste; 

	oSheet.Paste(Var,vFalse);
}

void CXlBase::CopyRow(CString sCell1, CString sCell2)
{
	CString sOld1 = sCell1;
	CString sOld2 = sCell2;

	sCell1.Delete(0);
	sCell2.Delete(0);

	short nCell1 = _ttoi(sCell1);
	short nCell2 = _ttoi(sCell2);
	short nSub = nCell2 - nCell1;

	CString sCell3, sCell4;

	//일반화된 식으로 수정할 것.
	sOld2.Format(_T("AP%d"), nCell2);
	sCell3.Format(_T("H%d"), nCell2+1);
	sCell4.Format(_T("AP%d"), nCell2+nSub+1);

	InsertCell(sCell3, sCell4);
	Copy(sOld1, sOld2, sCell3, sCell4);
}

void CXlBase::CopyPicture(CString sCell, CString sBookName, CString sSheetName, CString sPicName)
{
	CString OldBookName = oBook.GetName();
	CString OldSheetName = oSheet.GetName();

	LPDISPATCH lpDisp;

	lpDisp = oBooks.GetItem(COleVariant(sBookName));
	ASSERT(lpDisp);
		oBook.AttachDispatch(lpDisp);

	lpDisp = oBook.GetSheets();
	ASSERT(lpDisp);
		oSheets.AttachDispatch(lpDisp);

	lpDisp = oSheets.GetItem(COleVariant(sSheetName));
	ASSERT(lpDisp);
		oSheet.AttachDispatch(lpDisp);

	VARIANT vRange;
	vRange = oSheet.Evaluate(COleVariant(sPicName));
	lpDisp = vRange.pdispVal;
	ASSERT(lpDisp);
	Xl_Range oRange(lpDisp);

//	oRange.AttachDispatch(lpDisp);
	oRange.CopyPicture(1,1);

	lpDisp = oBooks.GetItem(COleVariant(OldBookName));
	ASSERT(lpDisp);
		oBook.AttachDispatch(lpDisp);

	lpDisp = oBook.GetSheets();
	ASSERT(lpDisp);
		oSheets.AttachDispatch(lpDisp);

	lpDisp = oSheets.GetItem(COleVariant(OldSheetName));
	ASSERT(lpDisp);
		oSheet.AttachDispatch(lpDisp);

	lpDisp = oSheet.GetRange(COleVariant(sCell),COleVariant(sCell));

		VARIANT Var;
	Var.vt = VT_DISPATCH;
	Var.pdispVal = lpDisp; 

	oSheet.Paste(Var,vFalse);
}

//Left:X, Top:Y 시작좌표,  Width, Heith: 폭과 높이
//File 종류: bmp, jpg, emf, wmf 등
void CXlBase::AddPicture(CString sFilename, float Left, float Top, float Width, float Height)
{
	LPDISPATCH lpDisp;

	Xl_Shapes shapes;
	lpDisp = oSheet.GetShapes();
	ASSERT(lpDisp);
	shapes.AttachDispatch(lpDisp);

	lpDisp = shapes.AddPicture(sFilename, FALSE, TRUE, Left, Top, Width, Height);
	lpDisp->Release();
//	shapes.ReleaseDispatch();
}

void CXlBase::AddPicture(CString sFilename, long Row, long Col, float Width, float Height, float scale)
{
	LPDISPATCH lpDisp;

	CString sCell;
	sCell.Format(_T("A%d"), Row);

	Xl_Range oRange;
	lpDisp = oSheet.GetRange(COleVariant(sCell),COleVariant(sCell));
	ASSERT(lpDisp);
	oRange.AttachDispatch(lpDisp);

	float Wi = (float)oRange.GetWidth().dblVal;
	float He = (float)oRange.GetHeight().dblVal;

	Xl_Shapes shapes;
	lpDisp = oSheet.GetShapes();	
	ASSERT(lpDisp);
	shapes.AttachDispatch(lpDisp);

	float Left = Wi * Col;
	float Top = He * Row;

	if(He*10<Height || Wi*28<Width)
	{
		scale = (He*10/Height) > (Wi*28/Width) ? Wi*28/Width : He*10/Height;
		Width = Width*scale;
		Height = Height*scale;
	}

	lpDisp = shapes.AddPicture(sFilename, FALSE, TRUE, Left, Top, Width, Height);
	lpDisp->Release();
}

/*iOpt - 1:오른쪽으로 이동 2:아래로 이동 3:행 삽입 4:열 삽입*/
void CXlBase::InsertCell(CString sCell1, CString sCell2, short iOpt)
{
	LPDISPATCH lpDisp;
	Xl_Range oRange;
	lpDisp = oSheet.GetRange(COleVariant(sCell1),COleVariant(sCell2));
	ASSERT(lpDisp);
	oRange.AttachDispatch(lpDisp);

	oRange.Insert(COleVariant((short)iOpt));
}

/*iOpt - 1:왼쪽으로 이동 2:위로이동 3:행 삭제 4:열 삭제*/
void CXlBase::DeletCell(CString sCell1, CString sCell2, short iOpt)
{
	LPDISPATCH lpDisp;
	Xl_Range oRange;
	lpDisp = oSheet.GetRange(COleVariant(sCell1),COleVariant(sCell2));
	ASSERT(lpDisp);
	oRange.AttachDispatch(lpDisp);

	oRange.Delete(COleVariant((short)iOpt));
}

void CXlBase::Clear(CString sCell1, CString sCell2)
{
	LPDISPATCH lpDisp;
	Xl_Range oRange;
	lpDisp = oSheet.GetRange(COleVariant(sCell1),COleVariant(sCell2));
	ASSERT(lpDisp);
	oRange.AttachDispatch(lpDisp);

	oRange.Clear();
}

//Cell번호를 주면 행,열로 반환
void CXlBase::CellNumber(CString sCell, long &Row, long &Col)
{
	short Len = sCell.GetLength();
	short CharLen = 0;
	short NumbLen = 0;

	for(short i=0; i<Len; i++)
	{
		TCHAR ch = sCell.GetAt(i);
		if(ch=='0'||ch=='1'||ch=='2'||ch=='3'||ch=='4'||ch=='5'||ch=='6'||ch=='7'||ch=='8'||ch=='9')
		{
			NumbLen = Len - CharLen;
			break;
		}
		CharLen++;
	}

	//열번호 계산과정 추가할 것
	CString sLeft = sCell.Left(CharLen);

	CString sRight = sCell.Right(NumbLen);
	Row = _ttoi(sRight);
}

CString CXlBase::GetCell(short Row, short Col)
{
	CString sCell=_T(""), sCol=_T(""), sRow=_T("");

	sRow.Format(_T("%d"), Row+1);

	if(Col < 26)
	{
		CString strTmp = (TCHAR)('A'+Col);
		sCol.Format(_T("%s"), strTmp);
	}
	else if(Col < 255)
	{
		short first, second;
		first = Col / 26 - 1;
		second = Col % 26;
		CString strTmp1 = (TCHAR)('A'+first);
		CString strTmp2 = (TCHAR)('A'+second);
		sCol.Format(_T("%s%s"), strTmp1, strTmp2);
	}

	sCell.Format(_T("%s%s"), sCol, sRow);

	return sCell;
}
