//
#include "stdafx.h"
#include "SeisCvlForceDirGrid.h"

#include "..\wg_common\wg_common_TBGrid.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_UnitCtrl.h"

#define D_COL_NUM 3
#define D_ROW_NUM 2

CSeisCvlForceDirGrid::CSeisCvlForceDirGrid()
{
	
}

CSeisCvlForceDirGrid::~CSeisCvlForceDirGrid()
{
}

///////////////////////////////////////////////////////////////////////////////
// User Defined Function
void CSeisCvlForceDirGrid::SetData2Grid(T_ELEM_K TopElemK, T_ELEM_K BotElemK, double dTopMy, double dBotMy)
{
	BOOL bOldLock = LockUpdate(TRUE);

	SetRowCount(D_ROW_NUM);

	for (int i = 0; i < D_ROW_NUM; i++)
	{
		CStringArray aValue;
		aValue.SetSize(D_COL_NUM);

		CString strElem;
		CString strMy;

		if(i==0)
		{
			strElem.Format(_T("%d"), TopElemK);
			strMy.Format(_T("%g"), dTopMy);

			aValue[0] = _LS(IDS_WG_TREEMENU_FORCE_DIR_TOP);
			aValue[1] = strElem;
			aValue[2] = strMy;
		}
		else
		{
			strElem.Format(_T("%d"), BotElemK);
			strMy.Format(_T("%g"), dBotMy);

			aValue[0] = _LS(IDS_WG_TREEMENU_FORCE_DIR_BOT);
			aValue[1] = strElem;
			aValue[2] = strMy;
		}
		
		SetDataToRow(i+1, aValue);
	}

	SetRowHeight(1, D_ROW_NUM, globalUtils.ScaleByDPI(20));

	LockUpdate(bOldLock);
	if(!bOldLock) Redraw();
}

void CSeisCvlForceDirGrid::SetDataToRow(ROWCOL nRow, CStringArray& raValue)
{
	int nColCount = GetColCount();
	ASSERT(nColCount == raValue.GetSize());

	for(int i=0; i<nColCount; ++i)
		SetValueRange(CGXRange(nRow, i+1), raValue[i]);
}


/////////////////////////////////////////////////////////////////////////////
// CSeisCvlForceDirGrid message handlers

void CSeisCvlForceDirGrid::Initialize(CWnd *pParent)
{
	CGXGridWnd::Initialize();
	  
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
	HideCols(0, 0);
	SetFrozenCols(1, 1);

	// State
	SetStyleRange(CGXRange().SetCols(1), CGXStyle()
		.SetFont(CGXFont_GC().SetSize(9))
		.SetValueType(GX_VT_STRING)
		.SetHorizontalAlignment(DT_CENTER)
		.SetVerticalAlignment(DT_VCENTER)
		.SetEnabled(FALSE)
		);

	// Elem
	SetStyleRange(CGXRange().SetCols(2), CGXStyle()
		.SetFont(CGXFont_GC().SetSize(9))
		.SetValueType(GX_VT_STRING)
		.SetHorizontalAlignment(DT_RIGHT)
		.SetVerticalAlignment(DT_VCENTER)
		.SetEnabled(FALSE)
		);

	// My.
	SetStyleRange(CGXRange().SetCols(3), CGXStyle()
		.SetFont(CGXFont_GC().SetSize(9))
		.SetValueType(GX_VT_STRING)
		.SetHorizontalAlignment(DT_RIGHT)
		.SetVerticalAlignment(DT_VCENTER)
		.SetEnabled(FALSE)
		);

	SetRowHeight(0, 0, globalUtils.ScaleByDPI(20));

	// undo를 금지시킨다.
	GetParam()->EnableUndo(FALSE);  

	// cell의 width와 height 크기 변경을 금지한다.
	GetParam()->EnableTrackRowHeight(FALSE);
	//GetParam()->EnableTrackColWidth(TRUE);

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

	int aWidth[] = {80, 64, 104};
	for (int i=0; i<D_COL_NUM; i++) SetColWidth(i+1, i+1, globalUtils.ScaleByDPI(aWidth[i]));

	CDBDoc* pDoc = CDBDoc::GetDocPoint();

	CString strMunit;
	pDoc->m_pUnitCtrl->GetUnitSystem(D_UNITSYS_BASE_MOMENT, strMunit);

	CString strM;
	strM.Format(_LS(IDS_WG_TREEMENU_FORCE_DIR_MY_s), strMunit);
	
	SetValueRange(CGXRange(0, 1), _LS(IDS_WG_TREEMENU_FORCE_DIR_STATE));
	SetValueRange(CGXRange(0, 2), _LS(IDS_WG_TREEMENU_FORCE_DIR_ELEM));
	SetValueRange(CGXRange(0, 3), strM);
	
	LockUpdate(FALSE);
}

BOOL CSeisCvlForceDirGrid::GetStyleRowCol(ROWCOL nRow, ROWCOL nCol, CGXStyle& style, GXModifyType mt, int nType)
{
	BOOL bCurrentRow;
	BOOL bCurrentCell;
	BOOL bEnabled;
	BOOL bHeaderCol;
	BOOL bHeaderRow;

	ROWCOL ncRow, ncCol;    // 현재 Row, Col
	CGXGridWnd::GetStyleRowCol(nRow, nCol, style, mt, nType);

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

	if (bHeaderCol || bHeaderRow) style           // Row or Column Header
		.SetControl(GX_IDS_CTRL_HEADER)
		.SetInterior(::GetSysColor(COLOR_BTNFACE))
		.SetTextColor(COLORREF(RGB(0, 0, 0)));
	else
		style
		.SetInterior(COLORREF(RGB(192, 220, 192)))
		.SetTextColor(COLORREF(RGB(0, 0, 0)));

	return TRUE;
}

BOOL CSeisCvlForceDirGrid::OnStartEditing(ROWCOL nRow, ROWCOL nCol)
{
	return CGXGridWnd::OnStartEditing(nRow, nCol);
}

void CSeisCvlForceDirGrid::OnModifyCell(ROWCOL nRow, ROWCOL nCol)
{
	CGXGridWnd::OnModifyCell(nRow, nCol);
}

BOOL CSeisCvlForceDirGrid::OnEndEditing(ROWCOL nRow, ROWCOL nCol)
{
	return CGXGridWnd::OnEndEditing(nRow, nCol);
}

BOOL CSeisCvlForceDirGrid::ProcessKeys(CWnd *pSender, UINT nMessage, UINT nChar, UINT nRepCnt, UINT flags)
{
	CRowColArray awRows;
	ROWCOL nRow=1, nCol=1;
	
	if(!GetCurrentCell(nRow, nCol)) // if no current cell, default action
		return CGXGridWnd::ProcessKeys(pSender, nMessage, nChar, nRepCnt, flags);
	
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

	return CGXGridWnd::ProcessKeys(pSender, nMessage, nChar, nRepCnt, flags); 
	
GT_PROC_RETURN:
	if(nMessage==WM_KEYDOWN && (nChar==VK_TAB || nChar==VK_RETURN || nChar==VK_RIGHT || nChar==VK_LEFT))
	{
		Redraw();    
	}
	
	return TRUE;
}

BOOL CSeisCvlForceDirGrid::SetRightValidCell(ROWCOL nRow, ROWCOL nCol)
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

BOOL CSeisCvlForceDirGrid::SetLeftValidCell(ROWCOL nRow, ROWCOL nCol)
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
