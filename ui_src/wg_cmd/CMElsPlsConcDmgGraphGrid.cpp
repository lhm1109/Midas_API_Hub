// CMElsPlsConcDmgGraphGrid.cpp : implementation file
//

#include "stdafx.h" 
#include "wg_cmd.h"
#include "CMElsPlsConcDmgGraphGrid.h"

#include "..\wg_common\wg_common_TBCommon.h"
#include "..\wg_common\wg_common_TBGrid.h"
#include "..\wg_db\UnitCtrl.h"
#include "CMElsPlsConcDmgGraphDlg.h"

#define D_COL_INELASTIC_STRAIN 	1
#define D_COL_YIELD_STRESS 	2
#define D_COL_DAMAGE 	3
#define D_COL_NUM 	3

CCMElsPlsConcDmgGraphGrid::CCMElsPlsConcDmgGraphGrid(CWnd* pParent)
{
	m_pParent = pParent;
}

CCMElsPlsConcDmgGraphGrid::~CCMElsPlsConcDmgGraphGrid()
{
}

///////////////////////////////////////////////////////////////////////////////
// User Defined Function
void CCMElsPlsConcDmgGraphGrid::SetData(CArray<PMATL_CDMG_BEHAVIOR, PMATL_CDMG_BEHAVIOR> &aData)
{
	BOOL bOldLock = LockUpdate(TRUE);

	int nSize = aData.GetSize();

	if(nSize < 0) return;
	SetRowCount(nSize + 1);

	for(int i=0; i<nSize; ++i)
	{
		CStringArray aValue;
		aValue.SetSize(D_COL_NUM);

		aValue[D_COL_INELASTIC_STRAIN  - 1].Format(_T("%g"), aData[i].dInelasticStrain);
		aValue[D_COL_YIELD_STRESS  - 1].Format(_T("%g"), aData[i].dYieldStress);
		aValue[D_COL_DAMAGE  - 1].Format(_T("%g"), aData[i].dDamage);

		SetDataToRow(i+1, aValue);
	}

	SetRowHeightByDPI(1, nSize, 18);

	LockUpdate(bOldLock);
	if(!bOldLock) Redraw();
} 

BOOL CCMElsPlsConcDmgGraphGrid::GetData(CArray<PMATL_CDMG_BEHAVIOR, PMATL_CDMG_BEHAVIOR> &aData, BOOL bSort /*= TRUE*/)
{
	aData.RemoveAll();

	int nCount = GetRowCount();
	nCount--;
	if(nCount==0) return TRUE;

	aData.SetSize(nCount);

	CStringArray aValue;
	aValue.SetSize(D_COL_NUM);

	for(int i=0; i<nCount; ++i)
	{
		if(!GetRowToData(i+1, aValue)) return FALSE;

		aData[i].dInelasticStrain  = _tstof(aValue[D_COL_INELASTIC_STRAIN  - 1]);
		aData[i].dYieldStress  = _tstof(aValue[D_COL_YIELD_STRESS  - 1]);
		aData[i].dDamage  = _tstof(aValue[D_COL_DAMAGE  - 1]);
	}

	if(bSort)
	{
		std::vector<PMATL_CDMG_BEHAVIOR> vecData;
		vecData.resize(aData.GetSize());
		for (int i = 0; i < aData.GetSize(); i++)
		{
			vecData[i] = aData[i];
		}
		std::sort(vecData.begin(), vecData.end());
		for (int i = 0; i < aData.GetSize(); i++)
		{
			aData[i] = vecData[i];
		}
	}  

	return TRUE;
}

void CCMElsPlsConcDmgGraphGrid::SetDataToRow(ROWCOL nRow, CStringArray& raValue)
{
	int nColCount = GetColCount();
	ASSERT(nColCount == raValue.GetSize());

	for(int i=0; i<nColCount; ++i)
		SetValueRange(CGXRange(nRow, i+1), raValue[i]);
}

BOOL CCMElsPlsConcDmgGraphGrid::GetRowToData(ROWCOL nRow, CStringArray& raValue)
{
	int nColCount = GetColCount();
	ASSERT(nColCount == raValue.GetSize());

	for(int i=0; i<nColCount; ++i)
		raValue[i] = GetEditingValue(nRow, i+1);

	return TRUE;
}

CString CCMElsPlsConcDmgGraphGrid::GetEditingValue(ROWCOL nRow, ROWCOL nCol)
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

BEGIN_MESSAGE_MAP(CCMElsPlsConcDmgGraphGrid, CTBBrowserWnd)
	//{{AFX_MSG_MAP(CCMElsPlsConcDmgGraphGrid)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMElsPlsConcDmgGraphGrid message handlers

void CCMElsPlsConcDmgGraphGrid::Initialize()
{
	CTBBrowserWnd::Initialize();

	// cell 바깥영역 배경색 지정 
	GetParam()->GetProperties()->SetColor(GX_COLOR_BACKGROUND, CTBGrid::m_Color.OuterBackground);

	// Header 배경색 지정 
	ChangeColHeaderStyle(CGXStyle().SetInterior(CTBGrid::m_Color.RowColHeaderBg)); 
	ChangeRowHeaderStyle(CGXStyle().SetInterior(CTBGrid::m_Color.RowColHeaderBg)); 

	// 기타 Cells 배경색 지정
	ChangeStandardStyle(CGXStyle( ).SetInterior(CTBGrid::m_Color.NormalModeBg));

	LockUpdate(TRUE);

	SetColCount(D_COL_NUM);

	// Header
	SetFrozenCols(0, 0);

	SetStyleRange(CGXRange().SetCols(D_COL_INELASTIC_STRAIN, D_COL_NUM), CGXStyle()
		.SetFont(CGXFont_GC().SetSize(9))
		.SetValueType(GX_VT_NUMERIC)
		.SetHorizontalAlignment(DT_RIGHT)
		.SetVerticalAlignment(DT_VCENTER)
		.SetPlaces(5)
		);

	SetRowHeightByDPI(0, 0, 37);

	// undo를 금지시킨다.
	GetParam()->EnableUndo(FALSE);  

	// cell의 width와 height 크기 변경을 금지한다.
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
	ChangeColHeaderStyle(CGXStyle().SetFont(CGXFont_GC().SetBold(FALSE)));

	CArray<int,int> aWidth;
	aWidth.SetSize(D_COL_NUM);	
	aWidth[D_COL_INELASTIC_STRAIN - 1] = 80;
	aWidth[D_COL_YIELD_STRESS - 1] = 80;
	aWidth[D_COL_DAMAGE - 1] = 80;

	SetColWidthByDPI(0, 0, 40);
	for (int i = 0; i < D_COL_NUM; i++)
	{
		SetColWidthByDPI(i + 1, i + 1, aWidth[i]);
	}
	
	CString csUnit;
	SetValueRange(CGXRange(0, D_COL_INELASTIC_STRAIN), _LS(IDS_CMD_EPCONC_INELASTIC_STRAIN));
	CUnitCtrl::GetUnitSystem(D_UNITSYS_BASE_STRESS, csUnit);
	
	SetValueRange(CGXRange(0, D_COL_YIELD_STRESS), _LS(IDS_CMD_EPCONC_YIELD_STRESS) + _T(" (") + csUnit + _T(")"));
	SetValueRange(CGXRange(0, D_COL_DAMAGE), _LS(IDS_CMD_EPCONC_DMG));

	SetRowCount(1);

	EnableDeleteRowByPressingKey(TRUE);

	LockUpdate(FALSE);
}

BOOL CCMElsPlsConcDmgGraphGrid::GetStyleRowCol(ROWCOL nRow, ROWCOL nCol, CGXStyle& style, GXModifyType mt, int nType)
{
	BOOL bCurrentRow;
	BOOL bCurrentCell;
	BOOL bEnabled;
	BOOL bHeaderCol;
	BOOL bHeaderRow;

	ROWCOL ncRow, ncCol;    // 현재 Row, Col
	CTBBrowserWnd::GetStyleRowCol(nRow, nCol, style, mt, nType);

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
		bEnabled = (!styleEx.GetIncludeEnabled() || styleEx.GetEnabled());
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

	// 일반 셀이 가장 많으므로 빨리 검사하고 바로 리턴할 수 있게 코드를 앞으로 뺀다.
	if (bEnabled && !bCurrentCell && !bCurrentRow && !bHeaderCol)
	{
		style
			.SetInterior(COLORREF(RGB(228, 236, 247)))
			.SetTextColor(COLORREF(RGB(0, 0, 0)));

		return TRUE;
	}

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

	if (bCurrentCell) style
		.SetInterior(COLORREF(RGB(255, 251, 240)))
		.SetTextColor(COLORREF(RGB(0, 0, 0)));
	else if (bCurrentRow) style
		.SetInterior(COLORREF(RGB(228, 236, 247)))
		.SetTextColor(COLORREF(RGB(0, 0, 0)));
	else if (bHeaderCol || bHeaderRow) style    // Enabled Header Col(Only Frozen)
		.SetInterior(COLORREF(RGB(228, 236, 247)))
		.SetTextColor(COLORREF(RGB(0, 0, 0)));

	return TRUE;
}

BOOL CCMElsPlsConcDmgGraphGrid::OnStartEditing(ROWCOL nRow, ROWCOL nCol)
{
	return CTBBrowserWnd::OnStartEditing(nRow, nCol);
}

void CCMElsPlsConcDmgGraphGrid::OnModifyCell(ROWCOL nRow, ROWCOL nCol)
{
	CTBBrowserWnd::OnModifyCell(nRow, nCol);
}

BOOL CCMElsPlsConcDmgGraphGrid::OnEndEditing(ROWCOL nRow, ROWCOL nCol)
{
	CCMElsPlsConcDmgGraphDlg* pParent = (CCMElsPlsConcDmgGraphDlg*)m_pParent;
	pParent->RedrawGraph();

	return CTBBrowserWnd::OnEndEditing(nRow, nCol);
}

BOOL CCMElsPlsConcDmgGraphGrid::ProcessKeys(CWnd *pSender, UINT nMessage, UINT nChar, UINT nRepCnt, UINT flags)
{
	CRowColArray awRows;
	ROWCOL nRow=1, nCol=1;

	if(!GetCurrentCell(nRow, nCol)) // if no current cell, default action
		return CTBBrowserWnd::ProcessKeys(pSender, nMessage, nChar, nRepCnt, flags);

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

	return CTBBrowserWnd::ProcessKeys(pSender, nMessage, nChar, nRepCnt, flags); 

GT_PROC_RETURN:
	if(nMessage==WM_KEYDOWN && (nChar==VK_TAB || nChar==VK_RETURN || nChar==VK_RIGHT || nChar==VK_LEFT))
	{
		Redraw();    
	}

	return TRUE;
}

BOOL CCMElsPlsConcDmgGraphGrid::SetRightValidCell(ROWCOL nRow, ROWCOL nCol)
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

BOOL CCMElsPlsConcDmgGraphGrid::SetLeftValidCell(ROWCOL nRow, ROWCOL nCol)
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

BOOL CCMElsPlsConcDmgGraphGrid::CopyRange(const CGXRangeList& selList)
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

	return CTBBrowserWnd::CopyRange(mSelList);
}

BOOL CCMElsPlsConcDmgGraphGrid::OnPasteFromClipboard(const CGXRange &range)
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

			if(row >= (int)GetRowCount())
			{
				SetRowCount(row + 1);
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

	SortGrid();

	LockUpdate(bOldLock);
	if (!bOldLock) Redraw();

	return TRUE;	
}

BOOL CCMElsPlsConcDmgGraphGrid::IsPasteToAllRange(const CGXRange &range, CString szData) // copy한 Cell이 1개 이고, 동일한 행의 cell 여러개로 붙여넣은 상황일 때 TRUE.. 이 경우엔 붙여넣을 Range의 모든 Cell에 복사한 Cell의 값을 붙여 넣어 줘야 함
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

BOOL CCMElsPlsConcDmgGraphGrid::OnAddNewRecord(ROWCOL nRow)
{
	SortGrid();

	return TRUE;
}
BOOL CCMElsPlsConcDmgGraphGrid::OnModifyRecord(ROWCOL nRow)
{
	SortGrid();

	return TRUE;
}

BOOL CCMElsPlsConcDmgGraphGrid::OnDeleteRecord(ROWCOL nRow)
{
	BOOL bReturnVal;
	bReturnVal = CTBBrowserWnd::OnDeleteRecord(nRow);

	CCMElsPlsConcDmgGraphDlg* pParent = (CCMElsPlsConcDmgGraphDlg*)m_pParent;
	pParent->RedrawGraph();

	return bReturnVal;
}
BOOL CCMElsPlsConcDmgGraphGrid::OnDeleteRecords(CRowColArray &awRows)
{
	BOOL bReturnVal;
	bReturnVal = CTBBrowserWnd::OnDeleteRecords(awRows);

	CCMElsPlsConcDmgGraphDlg* pParent = (CCMElsPlsConcDmgGraphDlg*)m_pParent;
	pParent->RedrawGraph();

	return bReturnVal;
}

void CCMElsPlsConcDmgGraphGrid::SortGrid()
{
	CArray<PMATL_CDMG_BEHAVIOR, PMATL_CDMG_BEHAVIOR> aData;
	GetData(aData, TRUE); // Sorting 해서 가지고 온다음..

	SetData(aData); // 바로 세팅
}
