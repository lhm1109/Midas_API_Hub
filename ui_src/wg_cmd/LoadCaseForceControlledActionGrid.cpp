// LoadCaseForceControlledActionGrid.cpp : implementation file
//

#include "stdafx.h" 
#include "wg_cmd.h"
#include "LoadCaseForceControlledActionGrid.h"

#include "..\wg_common\wg_common_TBCommon.h"
#include "..\wg_common\wg_common_TBGrid.h"
#include "..\wg_db\DBDoc.h"
#include "..\wg_db\AttrCtrl.h"
#include "..\wg_db\UnitCtrl.h"

enum ColumnIndex
{ 
	D_COL_KEY      =1,
	D_COL_LOADCASE , 
	D_COL_C_VALUE  , 
	D_COL_J_VALUE  , 

	D_COL_MAX = D_COL_J_VALUE
};

CLoadCaseForceControlledActionGrid::CLoadCaseForceControlledActionGrid()
{
	m_pDoc = CDBDoc::GetDocPoint();

	AddColInfo(_T("Key"),                      t_integer, 0);
	AddColInfo(_LS(IDS_CMD_BUCK_loadcase), t_string,  0);
	AddColInfo(_T("C")                   , t_real,    0);  
	AddColInfo(_T("J")                   , t_real,    0);  
}

CLoadCaseForceControlledActionGrid::~CLoadCaseForceControlledActionGrid()
{
}

///////////////////////////////////////////////////////////////////////////////
// User Defined Function
void CLoadCaseForceControlledActionGrid::SetData(CArray<T_LFCA_K, T_LFCA_K>& aLfcaK, CArray<T_LFCA_D, T_LFCA_D&>& aLfcaD, CArray<CString, CString&>& aName)
{
	BOOL bOldLock = LockUpdate(TRUE);

	int nNumKey = aLfcaK.GetSize();
	int nNumData = aLfcaD.GetSize();

	if(nNumKey < 0) return;
	if(nNumKey != nNumData) return;
	
	SetRowCount(nNumKey + 2);

	T_SPLC_D SplcD;

	for(int i=0; i<nNumKey; ++i)
	{
		CStringArray aValue;
		aValue.SetSize(D_COL_MAX);

		aValue[D_COL_KEY      - 1].Format(_T("%d"), aLfcaK[i].keymap);
		aValue[D_COL_LOADCASE - 1] = aName[i];
		aValue[D_COL_C_VALUE  - 1].Format(_T("%g"), aLfcaD[i].dC);
		aValue[D_COL_J_VALUE  - 1].Format(_T("%g"), aLfcaD[i].dJ);

		SetDataToRow(i+2, aValue);
		SetEnableDisableCells(i+2);
	}
	
	HideRows(nNumKey + 2, nNumKey + 2, TRUE);

	SetRowHeightByDPI(1, nNumKey, 18);

	LockUpdate(bOldLock);
	if(!bOldLock) Redraw();
} 

BOOL CLoadCaseForceControlledActionGrid::GetData(CArray<T_LFCA_K, T_LFCA_K>& raLfcaK, CArray<T_LFCA_D, T_LFCA_D&>& raLfcaD)
{
	raLfcaK.RemoveAll();
	raLfcaD.RemoveAll();

	int nDataCount = GetRowCount() - 2;
	if(nDataCount==0) return TRUE;

	raLfcaK.SetSize(nDataCount);
	raLfcaD.SetSize(nDataCount);

	CStringArray aValue;
	aValue.SetSize(D_COL_MAX);

	for(int i=0; i<nDataCount; ++i)
	{
		if(!GetRowToData(i+2, aValue)) return FALSE;

		raLfcaK[i].keymap = _ttoi(aValue[D_COL_KEY      - 1]);
		raLfcaD[i].dC     = _tstof(aValue[D_COL_C_VALUE  - 1]);
		raLfcaD[i].dJ     = _tstof(aValue[D_COL_J_VALUE  - 1]);
	}
	return TRUE;
}

void CLoadCaseForceControlledActionGrid::SetDataToRow(ROWCOL nRow, CStringArray& raValue)
{
	int nColCount = GetColCount();
	ASSERT(nColCount == raValue.GetSize());

	for(int i=0; i<nColCount; ++i)
		SetValueRange(CGXRange(nRow, i+1), raValue[i]);

	CString strTemp;
	strTemp.Format(_T("%d"), nRow - 1);
	SetValueRange(CGXRange(nRow, 0), strTemp);
}

BOOL CLoadCaseForceControlledActionGrid::GetRowToData(ROWCOL nRow, CStringArray& raValue)
{
	int nColCount = GetColCount();
	ASSERT(nColCount == raValue.GetSize());

	for(int i=0; i<nColCount; ++i)
		raValue[i] = GetEditingValue(nRow, i+1);

	return TRUE;
}

CString CLoadCaseForceControlledActionGrid::GetEditingValue(ROWCOL nRow, ROWCOL nCol)
{
	CString strNum(_T(""));
	BOOL bGet = FALSE;
	ROWCOL ncRow, ncCol;
	if(GetCurrentCell(ncRow, ncCol))
	{
		if(ncRow==nRow && ncCol==nCol)
		{
			CGXControl* pControl = GetControl(nRow, nCol);
			if(pControl && pControl->IsActive())
				bGet = pControl->GetValue(strNum);
		}
	}
	if(!bGet) strNum = GetValueRowCol(nRow, nCol);

	return strNum;
}

BEGIN_MESSAGE_MAP(CLoadCaseForceControlledActionGrid, CTBCommon)
	//{{AFX_MSG_MAP(CLoadCaseForceControlledActionGrid)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLoadCaseForceControlledActionGrid message handlers

void CLoadCaseForceControlledActionGrid::Initialize()
{
	T_UNIT_SYSTEM UnitSystem;
	for (int i = 0; i < D_COL_MAX; i++)
	{
		UnitSystem.Index.Add(0);
	}
	CTBCommon::Initialize(m_pDoc, UnitSystem);

	SetColCount(D_COL_MAX);
	SetRowCount(1);
	
	// 표준 스타일 변경
	ChangeStandardStyle(CGXStyle()
		.SetFont(CGXFont_GC().SetSize(10))
		.SetVerticalAlignment(DT_BOTTOM)
		.SetAllowEnter(FALSE));

	SetFrozenRows(1,1,GX_UPDATENOW,gxDo);
	SetStyleRange(CGXRange().SetRows(1, 1), CGXStyle().SetEnabled(FALSE));

	SetCoveredCellsRowCol(0, 0, 1, 0);  
	SetCoveredCellsRowCol(0, D_COL_LOADCASE, 1, D_COL_LOADCASE);  
	SetCoveredCellsRowCol(0, D_COL_C_VALUE , 1, D_COL_C_VALUE ); 
	SetCoveredCellsRowCol(0, D_COL_J_VALUE , 1, D_COL_J_VALUE ); 

	// cell 바깥영역 배경색 지정 
	GetParam()->GetProperties()->SetColor(GX_COLOR_BACKGROUND, CTBGrid::m_Color.OuterBackground);
	/*
	// Header 배경색 지정 
	ChangeColHeaderStyle(CGXStyle().SetInterior(CTBGrid::m_Color.RowColHeaderBg)); 
	ChangeRowHeaderStyle(CGXStyle().SetInterior(CTBGrid::m_Color.RowColHeaderBg)); 

	// 기타 Cells 배경색 지정
	ChangeStandardStyle(CGXStyle( ).SetInterior(CTBGrid::m_Color.NormalModeBg));
	*/
	LockUpdate(TRUE);

	SetStyleRange(CGXRange().SetCols(D_COL_KEY, D_COL_MAX), CGXStyle()  	
		.SetValueType(GX_VT_NUMERIC)
		.SetHorizontalAlignment(DT_RIGHT)
		.SetVerticalAlignment(DT_VCENTER)
		.SetPlaces(5)
		);

	SetStyleRange(CGXRange().SetCols(D_COL_LOADCASE), CGXStyle()
		.SetValueType(GX_VT_STRING));

	SetRowHeightByDPI(0, 0, 20);

	// undo를 금지시킨다.
	GetParam()->EnableUndo(FALSE);  

	// cell의 Height 크기 변경을 금지한다.
	GetParam()->EnableTrackRowHeight(FALSE);
	//GetParam()->EnableTrackColWidth(FALSE);

	// Row와  Column의 이동을 금지시킨다.
	GetParam()->EnableMoveRows(FALSE);
	GetParam()->EnableMoveCols(FALSE);

	// double click시 셀 편집이 되도록 한다.
	GetParam()->SetActivateCellFlags(GX_CAFOCUS_DBLCLICKONCELL);

	// MemoryDC를 사용한다.
	SetDrawingTechnique(gxDrawUsingMemDC);

	// 헤더의 Current cell을 막는다.
	ColHeaderStyle().SetEnabled(FALSE);
	RowHeaderStyle().SetEnabled(FALSE);

	// always show vertical scrollbar
	SetScrollBarMode(SB_VERT, gxnEnabled | gxnEnhanced);

	CGXProperties* pProp = GetParam()->GetProperties();
	pProp->SetMarkColHeader(FALSE); // Turn off pressed button effect for column headers
	pProp->SetMarkRowHeader(FALSE);
	pProp->SetUserProperty(GX_IDS_OUTLINECURRENTCELL, // 현재 셀의 border 설정 변경
		(CGXStyle) pProp->sInvertNoBorder);

	// 표준 스타일 변경
#if defined(_JP) || defined(_CH)
	ChangeColHeaderStyle(CGXStyle().SetFont(CGXFont_GC().SetBold(FALSE)));
#endif

	CArray<int,int> aWidth;
	aWidth.SetSize(D_COL_MAX);	
	aWidth[D_COL_KEY - 1    ] = 0;  
	aWidth[D_COL_LOADCASE- 1] = 95;
	aWidth[D_COL_C_VALUE - 1] = 80;
	aWidth[D_COL_J_VALUE - 1] = 80;

	SetColWidthByDPI(0, 0, 20);
	for (int i = 0; i < D_COL_MAX; i++)
	{
		SetColWidthByDPI(i + 1, i + 1, aWidth[i]);
	}
	
	SetValueRange(CGXRange(0, D_COL_LOADCASE), _LS(IDS_CMD_BUCK_loadcase));  
	SetValueRange(CGXRange(0, D_COL_C_VALUE ), _T("C"));
	SetValueRange(CGXRange(0, D_COL_J_VALUE ), _T("J"));

	LockUpdate(FALSE);
}

BOOL CLoadCaseForceControlledActionGrid::GetStyleRowCol(ROWCOL nRow, ROWCOL nCol, CGXStyle& style, GXModifyType mt, int nType)
{
	BOOL bCurrentRow;
	BOOL bCurrentCell;
	BOOL bEnabled;
	BOOL bHeaderCol;
	BOOL bHeaderRow;

	ROWCOL ncRow, ncCol;    // 현재 Row, Col
	CTBCommon::GetStyleRowCol(nRow, nCol, style, mt, nType);

	// 현재 셀의 그리드 좌표(Row, Col 번호)를 얻어온다.
	if (!GetCurrentCell(ncRow, ncCol)) 
		ncRow = GetRowCount()+1; // 현재 셀이 없는 경우 : ncRow를 범위 밖으로 설정

	// 현재 셀이 이동되면 새로 그려준다.
	m_bRefreshOnSetCurrentCell = TRUE;

	// Row나 Col이 1보다 작으면 리턴한다.(Standard Row or Column Header)
	if (nRow < 1 || nCol < 1) return TRUE;

	// Get Current Table and Cell State
	bCurrentRow = (nRow == ncRow);
	bCurrentCell = (nRow == ncRow && nCol == ncCol);

	// Enable, ReadOnly와 같은 Cell의 상태를 조사한다.
	// ComposeStyleRowCol은 호출해서는 안된다.
	// 그래서 여기서는 직접 조사한다.
	// 단 Cell Style, Column Style만 조사한다.
	if (style.GetIncludeEnabled())  // 셀에 Enable정보 포함 여부 알아냄
	{
		bEnabled = style.GetEnabled();
	}
	else // Column & Row style에서 enable 정보 알아냄
	{
		CGXStyle styleEx;
		GetColStyle(nCol, styleEx);
		//bEnabled = (!styleEx.GetIncludeEnabled() || styleEx.GetEnabled());
		GetRowStyle(nRow, styleEx);
		bEnabled = (!styleEx.GetIncludeEnabled() || styleEx.GetEnabled());
	}

	bHeaderCol = IsFrozenCol(nCol);
	bHeaderRow = IsFrozenRow(nRow);

	// Printing 모드이면 헤더 Column이 프린트 되기 위해 다음과 같이 리턴한다.
	if (IsPrinting())
	{
		if (bHeaderCol) style.SetControl(GX_IDS_CTRL_HEADER);
		return TRUE;
	}
	/*
	// 일반 셀이 가장 많으므로 빨리 검사하고 바로 리턴할 수 있게 코드를 앞으로 뺀다.
	if (bEnabled && !bCurrentCell && !bCurrentRow && !bHeaderCol)
	{
		style
			.SetInterior(COLORREF(RGB(228, 236, 247)))
			.SetTextColor(COLORREF(RGB(0, 0, 0)));

		return TRUE;
	}*/

	// Disable되었을 때
	if (!bEnabled)  
	{
		if (bHeaderCol || bHeaderRow) style           // Row or Column Header
			.SetControl(GX_IDS_CTRL_HEADER)
			.SetInterior(::GetSysColor(COLOR_BTNFACE))
			.SetTextColor(COLORREF(RGB(0, 0, 0)));
		else if (bCurrentRow) style     // Disabled Current Row
			.SetInterior(COLORREF(RGB(192, 192, 192)))
			.SetTextColor(COLORREF(RGB(0, 0, 0)));
		else style                      // Disabled Cell
			.SetInterior(COLORREF(RGB(192, 192, 192)))
			.SetTextColor(COLORREF(RGB(0, 0, 0)));

		return TRUE;
	}
	/*
	if (bCurrentCell) style
		.SetInterior(COLORREF(RGB(255, 251, 240)))
		.SetTextColor(COLORREF(RGB(0, 0, 0)));
	else if (bCurrentRow) style
		.SetInterior(COLORREF(RGB(228, 236, 247)))
		.SetTextColor(COLORREF(RGB(0, 0, 0)));
	else if (bHeaderCol || bHeaderRow) style    // Enabled Header Col(Only Frozen)
		.SetInterior(COLORREF(RGB(228, 236, 247)))
		.SetTextColor(COLORREF(RGB(0, 0, 0)));
		*/
	return TRUE;
}

BOOL CLoadCaseForceControlledActionGrid::OnStartEditing(ROWCOL nRow, ROWCOL nCol)
{
	return CTBCommon::OnStartEditing(nRow, nCol);
}

void CLoadCaseForceControlledActionGrid::OnModifyCell(ROWCOL nRow, ROWCOL nCol)
{
	CTBCommon::OnModifyCell(nRow, nCol);
}

BOOL CLoadCaseForceControlledActionGrid::DoLButtonUp(UINT flags, CPoint point)
{
	BOOL bOldLock = LockUpdate(TRUE);

	BOOL bReturnVal = CTBCommon::DoLButtonUp(flags, point);

	ROWCOL nhRow, nhCol;
	int ht = HitTest(point, &nhRow, &nhCol);
	if (ht != GX_NOHIT && ht != GX_VERTLINE)
	{
		SetEnableDisableCells( nhRow );
	}

	LockUpdate(bOldLock);
	if (!bOldLock) Redraw();

	return bReturnVal;
}

BOOL CLoadCaseForceControlledActionGrid::OnEndEditing(ROWCOL nRow, ROWCOL nCol)
{
	return CTBCommon::OnEndEditing(nRow, nCol);

	
}

BOOL CLoadCaseForceControlledActionGrid::ProcessKeys(CWnd *pSender, UINT nMessage, UINT nChar, UINT nRepCnt, UINT flags)
{
	CRowColArray awRows;
	ROWCOL nRow=1, nCol=1;

	if(!GetCurrentCell(nRow, nCol)) // if no current cell, default action
		return CTBCommon::ProcessKeys(pSender, nMessage, nChar, nRepCnt, flags);

	if(nMessage == WM_KEYDOWN)
	{
		switch(nChar)
		{
		case VK_TAB: 
			{
				BOOL bShift = GetKeyState(VK_SHIFT) & 0x8000;
				if( bShift )
				{
					if( SetLeftValidCell(nRow,  nCol)) { goto GT_PROC_RETURN; }
				}

				if(!bShift )
				{
					if( SetRightValidCell(nRow, nCol)) { goto GT_PROC_RETURN; }
				}
			}
			break;
		default: 
			break;
		}
	}

	return CTBCommon::ProcessKeys(pSender, nMessage, nChar, nRepCnt, flags); 

GT_PROC_RETURN:
	if(nMessage==WM_KEYDOWN && (nChar==VK_TAB || nChar==VK_RETURN || nChar==VK_RIGHT || nChar==VK_LEFT))
	{
		Redraw();    
	}

	return TRUE;
}

BOOL CLoadCaseForceControlledActionGrid::SetRightValidCell(ROWCOL nRow, ROWCOL nCol)
{
	long i=nRow, j=nCol+1; // 현재 행 다음 열
	int nColCount = GetColCount();
	if(j>nColCount) { ++i; j=1; }

	BOOL bCovered;/* = FALSE;*/
	while(i<=GetRowCount())
	{
		bCovered = FALSE;

		while(j<=nColCount)
		{
			// 열이 선택될 수 있는지 조사
			CGXStyle style; style.Free();
			ComposeStyleRowCol(i, j, &style);

			// Is it a covered cell?
			CGXRange cvr;
			if(!bCovered && GetCoveredCellsRowCol(i, j, cvr))
			{
				if((cvr.top<=i && i<=cvr.bottom) && (cvr.left<=j && j<=cvr.right))
				{
					if (nRow == cvr.top) i = cvr.bottom;
					j = cvr.right+1;
					bCovered = TRUE;
					continue;
				}
			}

			if(IsRowHidden(i)) break;
			if(IsColHidden(j)) { ++j; continue; }

			BOOL bEnable = (!style.GetIncludeEnabled() || style.GetEnabled());
			BOOL bReadOnly = style.GetReadOnly();

			if(bEnable && !bReadOnly)
			{
				if(SetCurrentCell(i, j)) return TRUE;
				return FALSE;
			}
			//bCovered = TRUE;
			bCovered = FALSE;

			++j;  // 다음 열을 조사
		}
		if(j>nColCount) { ++i; j=1; }
	}
	return FALSE;
}

BOOL CLoadCaseForceControlledActionGrid::SetLeftValidCell(ROWCOL nRow, ROWCOL nCol)
{
	long i=nRow, j=nCol-1; // 현재 행 이전 열
	int nColCount = GetColCount();
	if(nCol==0 || j<1) { --i; j=nColCount; } 

	BOOL bCovered = FALSE;
	while(i>=1)
	{
		bCovered = FALSE;
		while(j>=1)
		{
			// 열이 선택될 수 있는지 조사
			CGXStyle style; style.Free();
			ComposeStyleRowCol(i, j, &style);

			// Is it a covered cell?
			CGXRange cvr;
			if(!bCovered && GetCoveredCellsRowCol(i, j, cvr))
			{
				if((cvr.top<=i && i<=cvr.bottom) && (cvr.left<=j && j<=cvr.right))
				{
					i = cvr.bottom;
					j = cvr.left-1;
					bCovered = TRUE;
					continue;
				}
			}

			if(IsRowHidden(i)) break;
			if(IsColHidden(j)) { --j; continue; }

			BOOL bEnable = !style.GetIncludeEnabled() || style.GetEnabled();
			BOOL bReadOnly = style.GetReadOnly();

			if(bEnable && !bReadOnly)
			{
				if(GetCoveredCellsRowCol(i, j, cvr))
				{
					if((cvr.top<=i && i<=cvr.bottom) && (cvr.left<=j && j<=cvr.right))
					{
						j = cvr.left;
					}
				}
				if(SetCurrentCell(i, j)) return TRUE;
				return FALSE;
			}
			bCovered = TRUE;
			--j;  // 이전 열을 조사
		}
		if(nCol==0 || j<1) { --i; j=nColCount; } 
	}
	return FALSE;
}

BOOL CLoadCaseForceControlledActionGrid::CopyRange(const CGXRangeList& selList)
{
	CGXRangeList mSelList;
	CGXRange* mRange;
	POSITION position=selList.GetHeadPosition();

	while(TRUE)
	{
		if(!position||position<0)
			break;

		mRange=new CGXRange();
		if(mRange->IntersectRange(CGXRange(1,1,GetRowCount(),GetColCount()),selList.GetAt(position)))
		{
			mSelList.AddTail(mRange);
		}
		else
			delete mRange;

		selList.GetNext(position);
	}

	return CTBCommon::CopyRange(mSelList);
}

BOOL CLoadCaseForceControlledActionGrid::OnPasteFromClipboard(const CGXRange &range)
{
	OpenClipboard(); // open clipboard
	HGLOBAL handle = GetClipboardData(CF_UNICODETEXT);
	LPTSTR psz = (LPTSTR)GlobalLock(handle);
	TCHAR* pTChar = (TCHAR*)psz;
	DWORD size = (DWORD)GlobalSize(handle);
	GlobalUnlock(handle);
	CloseClipboard(); // close the clipboard

	char	strMultibyte[1024] = {0,};
	WideCharToMultiByte(CP_ACP, 0, (LPCWSTR)psz, -1, strMultibyte, 1024 ,NULL ,NULL);

	CString  sub = _T(""), szData = strMultibyte;
	LPTSTR   buf = szData.GetBuffer(1);
	int      pos = 0, endpos, len = szData.GetLength();
	TCHAR    endchar = L' ';
	int		 col = range.left;
	int  	 row = range.top;	

	BOOL bPasteToAllRange = IsPasteToAllRange(range, szData);

	BOOL bOldLock = LockUpdate(TRUE);

	BOOL bFirstDone = FALSE;
	CString szSubFirst = _T("");

	while(pos < len)
	{
		if(buf[pos] == _T('\t') || buf[pos] == _T('\r') || buf[pos] == _T('\n'))
		{
			endchar = buf[pos];
			endpos = pos;
		}		
		else // find the end of the item then copy the item to the cell
		{
			endpos = pos + 1;
			while(endpos < len)
			{
				endchar = buf[endpos];
				if(endchar == _T('\n') || endchar == _T('\r') || endchar == _T('\t')) break;
				endpos++;
			}

			if(col <= (int)GetColCount() && row <= (int)GetRowCount())
			{	
				sub = szData.Mid(pos, endpos - pos);
				if(bFirstDone == FALSE)
				{
					szSubFirst = sub;
				}
				SetValueRange(CGXRange(row, col), sub);
			}
		}

		if(endchar == _T('\t')) col++;
		if(endchar == _T('\r') || endchar == _T('\n'))
		{
			col = range.left;
			row++;
			if(buf[endpos] == _T('\r') && buf[endpos + 1] == _T('\n')) endpos++;
		}
		pos = endpos + 1;
	}

	if(bPasteToAllRange && !szSubFirst.IsEmpty())
	{
		while(row <= (int)range.bottom)
		{
			SetValueRange(CGXRange(row, col), szSubFirst);

			row++;
		}
	}

	szData.ReleaseBuffer();

	LockUpdate(bOldLock);
	if (!bOldLock) Redraw();

	return TRUE;	
}

BOOL CLoadCaseForceControlledActionGrid::IsPasteToAllRange(const CGXRange &range, CString szData) // copy한 Cell이 1개 이고, 동일한 행의 cell 여러개로 붙여넣은 상황일 때 TRUE.. 이 경우엔 붙여넣을 Range의 모든 Cell에 복사한 Cell의 값을 붙여 넣어 줘야 함
{
	// 붙여넣을 Cell이 모두 동일한 행에 있는지 확인
	if(szData.Find(_T('\t')) != -1 || szData.Find(_T('\r')) != -1 || szData.Find(_T('\n')) != -1 )
	{
		return FALSE;
	}

	// 복사한 cell이 하나인지 확인
	if(range.left != range.right)
	{
		return FALSE;
	}

	return TRUE;
}

void CLoadCaseForceControlledActionGrid::SetEnableDisableCells(ROWCOL nRow)
{
	if(nRow <= 1) return;
	SetStyleRange(CGXRange().SetCells(nRow, D_COL_LOADCASE), CGXStyle().SetReadOnly(TRUE));
}

BOOL CLoadCaseForceControlledActionGrid::AddToDB( ROWCOL nRow )
{
	// DB는 안쓰고 기본적인 기능만쓰고싶은데.. ADDToDB가 끝나면 행하나를 지워버리기 떄문에 미리 하나 더 복사해서 만들어 줘야함.. ㅠㅠ  
	InsertRecord(nRow);

	CString strVal;
	for (int i = 1; i <= D_COL_MAX; i++)
	{
		strVal = GetValueRowCol(nRow - 1, i);
		SetValueRange(CGXRange(nRow, i), strVal);
	}

	return TRUE;
}
