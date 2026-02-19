// DgnSteelSPCSectJGrid.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnSteelSPCSectJGrid.h"

#include "..\wg_common\wg_common_TBCommon.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_DBLib.h"
#include "..\wg_db\wg_db_ViewCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_UnitCtrl.h"

#include "..\wg_base\wg_base_CompFunc.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDgnSteelSPCSectJGrid

CDgnSteelSPCSectJGrid::CDgnSteelSPCSectJGrid(int nElemCount)
{ 
	m_nOldCol = -1;

	m_pParent = 0;

	m_nColCount  = 6;
	m_nColHeader = nElemCount;   // extra column header(+default header 2)

	m_pDoc = CDBDoc::GetDocPoint();
	ASSERT(m_pDoc);	
}

CDgnSteelSPCSectJGrid::~CDgnSteelSPCSectJGrid()
{
}


#define CWnd CGXGridWnd
BEGIN_MESSAGE_MAP(CDgnSteelSPCSectJGrid, CWnd)
#undef CWnd
	//{{AFX_MSG_MAP(CDgnSteelSPCSectJGrid)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CDgnSteelSPCSectJGrid message handlers

void CDgnSteelSPCSectJGrid::OnModifyCell(ROWCOL nRow, ROWCOL nCol)
{
	CGXGridWnd::OnModifyCell(nRow, nCol);	
}

void CDgnSteelSPCSectJGrid::OnInitCurrentCell(ROWCOL nRow, ROWCOL nCol)
{
	CGXGridWnd::OnInitCurrentCell(nRow, nCol);
}

BOOL CDgnSteelSPCSectJGrid::OnPasteFromClipboard(const CGXRange &range)
{
	// 임시로 CGXGridWnd를 하나 만들어 Paste를 한 후 작업을 한다.
	CString csErrMsg;
	CRowColArray awRows, awCols;
	int nTotalRow, nTotalCol;

	CGXRange r;
	r.top = r.bottom = 1;
	r.left = r.right = 1;
	CGXGridWnd x;
	x.Create(WS_CHILD, CRect(0, 0, 1, 1), this, 0);
	x.Initialize();
	x.SetRowCount(0); x.SetColCount(1);
	x.OnPasteFromClipboard(r);

	r.top = 1; r.left = 1;
	r.bottom = x.GetRowCount(); r.right = x.GetColCount();
	int nPastedRows = r.bottom - r.top + 1;   
	int nPastedCols = r.right - r.left + 1;  

	if (nPastedCols > 3)  
	{
		csErrMsg = _LS(IDS_TM_STDG_GRID_ERR_MSG5);
		goto PASTE_ERROR;
	}

	nTotalRow = GetRowCount();
	nTotalCol = GetColCount();

	GetSelectedRows(awRows);
	GetSelectedCols(awCols);

	if (awRows.GetSize() > 1) // Select된 것이 2줄 이상
	{
		if (nPastedRows != 1)
		{
			if (awRows.GetSize() != nPastedRows)
			{
				csErrMsg = _LS(IDS_TM_STDG_GRID_ERR_MSG2);
				goto PASTE_ERROR;
			}
		}
	}
	else 
	{
		int nTgtRow;
		if (awRows.GetSize() == 0)  
		{
			ROWCOL ncRow, ncCol;
			if (!GetCurrentCell(ncRow, ncCol))  
			{
				csErrMsg = _LS(IDS_TM_STDG_GRID_ERR_MSG4);
				goto PASTE_ERROR;
			}
			nTgtRow = ncRow;
		}
		else nTgtRow = awRows[0];
		
		if (nTgtRow + nPastedRows - 1 > nTotalRow)
		{
			csErrMsg = _LS(IDS_TM_STDG_GRID_ERR_MSG5);
			goto PASTE_ERROR;
		}

		if(awCols[0] + nPastedCols - 1 > nTotalCol)
		{
			csErrMsg = _LS(IDS_TM_STDG_GRID_ERR_MSG5);
			goto PASTE_ERROR;
		}
	}
	
	x.DestroyWindow();

	CGXGridWnd::OnPasteFromClipboard(range);
	return TRUE;   

PASTE_ERROR:
	SetWarningText(csErrMsg);
	DisplayWarningText();
	x.DestroyWindow();
	return FALSE;
}

BOOL CDgnSteelSPCSectJGrid::OnStartEditing(ROWCOL nRow, ROWCOL nCol)
{
	return CGXGridWnd::OnStartEditing(nRow, nCol); 
}


// Delete 를 막아줌 
BOOL CDgnSteelSPCSectJGrid::ProcessKeys(CWnd *pSender, UINT nMessage, UINT nChar, UINT nRepCnt, UINT flags)
{
	if((nMessage == WM_KEYDOWN) && (nChar == VK_DELETE)) return TRUE;
	 
	return CGXGridWnd::ProcessKeys(pSender, nMessage, nChar, nRepCnt, flags); 
}

BOOL CDgnSteelSPCSectJGrid::OnLButtonClickedRowCol(ROWCOL nRow, ROWCOL nCol, UINT nFlags, CPoint pt)
{
	if(nRow != 0)
		m_pParent->SelectLineRedraw(nRow);	

	return CTBBrowserWnd::OnLButtonClickedRowCol(nRow, nCol, nFlags, pt);
}

// Add
//--------------------------------------------------------------------------
// Interface
void CDgnSteelSPCSectJGrid::Initialize(CDgnStlSPCSectJTabDlg* pParent)
{
	 
	CTBBrowserWnd::Initialize();

	m_pParent = pParent;

	SetColInfo();

	GetParam()->EnableMoveCols(FALSE);  // Disable moving column
	GetParam()->EnableMoveRows(FALSE);   // Disable moving row
	GetParam()->EnableTrackRowHeight(FALSE);  // Disable resizing row height
 	//GetParam()->EnableSelection(GX_SELROW);                       // Row Selection
	GetParam()->EnableTrackRowHeight(GX_TRACK_NOTHEADER);         // Row Size 변경금지
	GetParam()->EnableTrackColWidth(GX_TRACK_NOTHEADER);          // Col Size 변경금지
	GetParam()->SetActivateCellFlags(GX_CAFOCUS_DBLCLICKONCELL);
	SetDrawingTechnique(gxDrawUsingMemDC);                        // MemDC 사용

	ColHeaderStyle().SetEnabled(FALSE);
	RowHeaderStyle().SetEnabled(FALSE);

	SetScrollBarMode(SB_VERT, gxnEnabled/* | gxnEnhanced*/);

	EnableInsertRowByPressingKey(FALSE);  // Disable inserting row by key
	MakeItemEx();
}

void CDgnSteelSPCSectJGrid::MakeItemEx()
{

}

//--------------------------------------------------------------------------
// Implementation
void CDgnSteelSPCSectJGrid::SetColInfo()
{
	//int nColNo = m_aColInfo.GetSize();

	BOOL bOldLock = LockUpdate(TRUE);

	SetRowCount(m_nColHeader);
	SetColCount(m_nColCount);

	// 표준 스타일 변경
	ChangeStandardStyle(CGXStyle()
			.SetFont(CGXFont_GC().SetSize(10))
			.SetVerticalAlignment(DT_BOTTOM)
			.SetAllowEnter(FALSE));

	// 표준 스타일 변경
	ChangeStandardStyle(CGXStyle()
			.SetFont(CGXFont_GC().SetSize(9))
			.SetVerticalAlignment(DT_BOTTOM)
			.SetAllowEnter(FALSE));
	ChangeColHeaderStyle(CGXStyle().SetFont(CGXFont_GC().SetBold(FALSE)));
	ChangeRowHeaderStyle(CGXStyle()
					.SetFont(CGXFont_GC().SetBold(FALSE))
					.SetHorizontalAlignment(DT_RIGHT)
					.SetControl(GX_IDS_CTRL_HEADER));
	GetParam()->SetNumberedRowHeaders(TRUE);

	// 컬럼 스타일 변경
	SetStyleRange(CGXRange().SetCols(1, m_nColCount), CGXStyle()
			.SetHorizontalAlignment(DT_RIGHT)
			.SetValueType(GX_VT_STRING));

	SetRowHeightByDPI(0, 0, 20);
	SetRowHeightByDPI(1, GetRowCount(), 18);
	SetColWidthByDPI(0, 0, 85);
	SetColWidthByDPI(1, 1, 70);
	SetColWidthByDPI(2, 2, 90);
	SetColWidthByDPI(3, 3, 70);
	SetColWidthByDPI(4, 4, 70);
	SetColWidthByDPI(5, 5, 70);
	SetColWidthByDPI(6, 6, 50);

	// Set Title
	SetHeaderTitle();
	
	//Group, sub Group, 구분
	SetStyleRange(CGXRange().SetCols(0, 2), CGXStyle()
			.SetControl(GX_IDS_CTRL_STATIC)    
			.SetHorizontalAlignment(DT_CENTER)
			.SetEnabled(FALSE)
			.SetValueType(GX_VT_STRING)
			);

	//길이
	SetStyleRange(CGXRange().SetCols(3), CGXStyle()
	    .SetControl(GX_IDS_CTRL_EDIT)    
			.SetValueType(GX_VT_NUMERIC)
			.SetEnabled(TRUE)
			.SetHorizontalAlignment(DT_CENTER)
			);

	//두께
	SetStyleRange(CGXRange().SetCols(4), CGXStyle()
	    .SetControl(GX_IDS_CTRL_EDIT)    
			.SetValueType(GX_VT_NUMERIC)
			.SetEnabled(TRUE)
			.SetHorizontalAlignment(DT_CENTER)
			);

	//패널수
	SetStyleRange(CGXRange().SetCols(5), CGXStyle()
	    .SetControl(GX_IDS_CTRL_EDIT)    
			.SetValueType(GX_VT_STRING)
			.SetEnabled(TRUE)
			.SetHorizontalAlignment(DT_CENTER)
			);

	//비고
	SetStyleRange(CGXRange().SetCols(6), CGXStyle()
	    .SetControl(GX_IDS_CTRL_CHECKBOX3D)    
			.SetValueType(GX_VT_STRING)
			.SetEnabled(TRUE)
			.SetHorizontalAlignment(DT_CENTER)
			);

	

	
	// 현재 셀의 border 설정 변경
	CGXProperties* pProp = GetParam()->GetProperties();
			pProp->SetUserProperty(GX_IDS_OUTLINECURRENTCELL,
		      (CGXStyle) pProp->sInvertNoBorder);

	// 현재 셀 설정
	SetRightValidCell(1, 1);
	GetParam()->SetHideCurrentCell(GX_HIDE_NEVER);

	LockUpdate(bOldLock);
	Redraw();
}

void CDgnSteelSPCSectJGrid::SetHeaderTitle()
{
	BOOL bOldLock = LockUpdate(TRUE);  

	CString title = _T("");

	title = _LS(IDS_DGN_GROUP);//_T("Group")
	SetCoveredCellsRowCol(0,0,0,0);
	SetStyleRange(CGXRange(0,0), CGXStyle().SetValue(title));

	title = _LS(IDS_DGN_SUB_GROUP);//_T("sub Group")
	SetCoveredCellsRowCol(0,1,0,1);
	SetStyleRange(CGXRange(0,1,0,1), CGXStyle().SetValue(title));

	title = _LS(IDS_DGN_CLASS);//_T("구분")
	SetCoveredCellsRowCol(0,2,0,2);
	SetStyleRange(CGXRange(0,2,0,2), CGXStyle().SetValue(title));

	title = _LS(IDS_DGN_LENGTH);//_T("길이(b)")
	SetCoveredCellsRowCol(0,3,0,3);
	SetStyleRange(CGXRange(0,3,0,3), CGXStyle().SetValue(title));

	title = _LS(IDS_DGN_THICKNESS);//_T("두께(t)")
	SetCoveredCellsRowCol(0,4,0,4);
	SetStyleRange(CGXRange(0,4,0,4), CGXStyle().SetValue(title));

	title = _LS(IDS_DGN_PANEL_NUM);//_T("패널수(n)")
	SetCoveredCellsRowCol(0,5,0,5);
	SetStyleRange(CGXRange(0,5,0,5), CGXStyle().SetValue(title));

	title = _LS(IDS_DGN_STEEL_CHECK_EXCEPTION);//_T("비고")
	SetCoveredCellsRowCol(0,6,0,6);
	SetStyleRange(CGXRange(0,6,0,6), CGXStyle().SetValue(title));
	
	LockUpdate(bOldLock);
	//if (!bOldLock) Redraw();
}

BOOL CDgnSteelSPCSectJGrid::PasteFromClipboard(const CGXRange &range, CString* aDefVal, int nKeyCol, 
																				 CGXGridWnd &x, int &nPasteMode, int &nPastedCols)
{
	CGXRange r;
	r.top = r.bottom = 1;
	r.left = r.right = 1;
	x.Create(WS_CHILD, CRect(0, 0, 1, 1), GetParent(), 0);
	x.Initialize();
	x.GetParam()->EnableUndo(FALSE);
	x.LockUpdate(TRUE);
	x.SetRowCount(0); x.SetColCount(1);
	x.OnPasteFromClipboard(r);

	// Paste된 범위를 구한다.
	r.top = 1; r.left = 1;
	r.bottom = x.GetRowCount(); r.right = x.GetColCount();

	// 규칙
	// 범위가 다르면 경고 메세지를 보내고 리턴
	// Paste된 내용이 레코드의 모든 필드를 만족하지 못하면 Default 값으로 채운다.
	// column 구분이 없기 때문에 칼럼 by 칼럼으로 Paste한다.
	// 즉 column의 위치가 변경되었으면 변경된 대로 Paste 한다.
	// 고의적으로 Hide된 칼럼은 Default 값으로 채운다.

	// 용어
	// 원본 범위 : Copy해서 Clipboard로 들어간 row, col 갯수
	// 대상 범위 : User가 마우스로 선택해 놓은 범위

	// 범위에 고의로 숨겨진 Column(HideByIntend)이 포함되어 있으면 이 칼럼을
	// 삽입하고 Default Value로 채운다.
	CArray<int, int> aHideCol;
	int nColCount = GetColCount();
	int nPasteColCount = r.GetWidth();
	for (int k = 1; k < nPasteColCount; k++)
	{
		if (range.left+k < nColCount && IsHiddenByIntend(range.left+k))
		{
			x.InsertCols(k+1, 1);
			aHideCol.Add(range.left+k);
			r.right++;
			nPasteColCount++;
		}
	}

	CString sErrMsg = _T("");
	ROWCOL nRow, nCol;
	int nLeftInsertWidth, nRightInsertWidth;
	VERIFY(GetCurrentCell(nRow, nCol));
	// 대상 범위가 지정되지 않은 경우
	// 현재 셀이 있는 위치를 기준으로 한다.
	if (range.GetHeight() == 1 && range.GetWidth() == 1)
	{
		// 대상 범위가 Append Row를 넘어서면 수정과 추가가 중복되므로 에러
		if (nRow != GetRowCount() && (nRow + r.GetHeight()) > GetRowCount())
		{
			//sErrMsg = _T("수정과 추가가 혼합되게 됩니다. 위치를 다시 지정하세요.");
			sErrMsg = _LS(IDS_TB_PASTE_ADD_MODIFY_MIX);
			goto END_OF_PASTE;
		}

		// 대상 범위가 Column 한계를 넘어서면 에러
		if (nCol + r.GetWidth() - 1 > GetColCount())
		{
			//sErrMsg = _T("붙여 넣을 범위가 칼럼 범위를 초과합니다. 위치를 다시 지정하세요.");
			sErrMsg = _LS(IDS_TB_PASTE_EXCEED_COLUMN);
			goto END_OF_PASTE;
		}

		// 편집 모드이고 원본 범위가 2줄 이상이면 에러
		if (IsEdit() && r.GetHeight() > 1)
		{
			//sErrMsg = _T("현재 편집중인 레코드가 복사 범위에 포함되어 있습니다.");
			sErrMsg = _LS(IDS_TB_PASTE_EDIT_DATA_INCLUDED);
			goto END_OF_PASTE;
		}

		// 현재 셀이 Append Row에 있으면 모두 추가(DB에 추가된다.)
		if (nRow == GetRowCount())
			nPasteMode = TB_PASTE_MODE_APPEND;
		// 레코드 수정상태를 유지(DB를 수정하지 않는다.)
		else if (IsEdit()) nPasteMode = TB_PASTE_MODE_ONMODIFY;
		// 모두 수정(DB에서 수정이 된다.)
		else nPasteMode = TB_PASTE_MODE_MODIFY;
	}
	// 대상 범위가 지정된 경우 : 원본과 같은 범위, Fill Mode(원본 1줄, 동일 칸수)
	else
	{
		// 편집 모드에서 편집 레코드가 대상 범위에 포함되어 있고 2줄 이상이면 에러 표시
		if (IsEdit() && nRow >= range.top && nRow <= range.bottom && range.GetHeight() > 1)
		{
			//sErrMsg = _T("현재 편집중인 레코드가 복사 범위에 포함되어 있습니다.");
			sErrMsg = _LS(IDS_TB_PASTE_EDIT_DATA_INCLUDED);
			goto END_OF_PASTE;
		}
		
		// Append Row가 범위에 포함된 경우 다른 Row도 범위에 포함되어 있으면 에러
		if (range.bottom == GetRowCount() && range.GetHeight() > 1)
		{
			//sErrMsg = _T("수정과 추가가 혼합되게 됩니다. 위치를 다시 지정하세요.");
			sErrMsg = _LS(IDS_TB_PASTE_ADD_MODIFY_MIX);
			goto END_OF_PASTE;
		}

		// Fill 조건을 검사한다.
		// 원본이 1줄 이상이면 에러, 칸수가 달라도 에러가 된다.
		BOOL bFillMode = FALSE;
		if (range.GetHeight() != r.GetHeight() || range.GetWidth() != r.GetWidth())
		{
			// Fill Mode가 안 되는 조건
			if (r.GetHeight() > 1 || range.GetWidth() != r.GetWidth())
			{
				//sErrMsg = _T("대상 범위를 지정한 경우는 원본 범위의 크기와 같아야 합니다.");
				sErrMsg = _LS(IDS_TB_PASTE_RANGE_MISMATCH);
				goto END_OF_PASTE;
			}
			// 대상 범위가 1줄 이상이면 Fill Mode, 아니면 Paste와 동일하다.
			if (range.GetHeight() > 1) bFillMode = TRUE;
		}

		// Append 단독 Row면 레코드 추가
		if (range.bottom == GetRowCount()) nPasteMode = TB_PASTE_MODE_APPEND; 
		// 레코드 수정 상태를 유지하며 DB에서 수정은 일어나지 않는다.
		else if (IsEdit() && range.top == nRow) nPasteMode = TB_PASTE_MODE_ONMODIFY;
		// 아니면 레코드 수정이 된다.
		else nPasteMode = TB_PASTE_MODE_MODIFY;

		// Fill Mode인지 검사한다.
		if (bFillMode)
		{
			// 대상 범위의 줄수 만큼 Row를 만들어 채운다.
			x.SetRowCount(range.GetHeight());
			CString value;
			for (int i = 0; i < r.GetWidth(); i++)  // Column Wise Copy...
			{
				value = x.GetValueRowCol(1, i+1);
				for (int j = 1; j < range.GetHeight(); j++)
					x.SetExpressionRowCol(j+1, i+1, value);
			}
			r.bottom = x.GetRowCount();
		}
	}

	// 레코드 또는 레코드 셋을 만든다.
	// 왼쪽과 오른쪽에 모자라는 칼럼수를 삽입한다.
	nPastedCols = r.GetWidth();
	nLeftInsertWidth = range.left-1;
	if (nLeftInsertWidth > 0) x.InsertCols(1, nLeftInsertWidth);
	nRightInsertWidth = GetColCount()-x.GetColCount();
	if (nRightInsertWidth > 0) x.InsertCols(x.GetColCount()+1, nRightInsertWidth);
	// 모자라는 값을 카피한다. (Append는 Default값, Modify는 기존 값)
	if (nPasteMode == TB_PASTE_MODE_APPEND)
	{
		int nID;
		CString value;
		// 속도 향상을 위해 column별로 복사
		// 왼쪽 칼럼 카피 
		for (int j = 1; j <= nLeftInsertWidth; j++)
		{
			nID = j-1;
			if (nID == nKeyCol) value = _T("");
			else value = aDefVal[nID-1];
			for (int i = 1; i <= r.GetHeight(); i++)
				x.SetExpressionRowCol(i, j, value);
		}
		// Copy된 영역중 고의로 숨겨진 칼럼에 Default값 채우기
		for (int j = 0; j < aHideCol.GetSize(); j++)
		{
			nID = aHideCol[j]-1;
			for (int i = 1; i <= r.GetHeight(); i++)
				x.SetExpressionRowCol(i, aHideCol[j], aDefVal[nID-1]);
		}
		// 오른쪽 칼럼 카피
		for (int j = nLeftInsertWidth+nPastedCols+1; j <= GetColCount(); j++)
		{
			nID = j-1;
			if (nID == nKeyCol) value = _T("");
			else value = aDefVal[nID-1];
			for (int i = 1; i <= r.GetHeight(); i++)
				x.SetExpressionRowCol(i, j, value);
		}
	}
	else if (nPasteMode == TB_PASTE_MODE_ONMODIFY || nPasteMode == TB_PASTE_MODE_MODIFY)
	{
		CString value;
		// 왼쪽 칼럼 카피 
		for (int j = 1; j <= nLeftInsertWidth; j++)
		{
			for (int i = 1; i <= r.GetHeight(); i++)
			{
				value = GetValueRowCol(range.top-1+i, j);
				x.SetExpressionRowCol(i, j, value);
			}
		}
		// Copy된 영역중 고의로 숨겨진 칼럼에 기존의 값 채우기
		for (int j = 0; j < aHideCol.GetSize(); j++)
		{
			for (int i = 1; i <= r.GetHeight(); i++)
			{
				value = GetValueRowCol(range.top-1+i, aHideCol[j]);
				x.SetExpressionRowCol(i, aHideCol[j], value);
			}
		}
		// 오른쪽 칼럼 카피
		for (int j = nLeftInsertWidth+nPastedCols+1; j <= GetColCount(); j++)
		{
			for (int i = 1; i <= r.GetHeight(); i++)
			{
				value = GetValueRowCol(range.top-1+i, j);
				x.SetExpressionRowCol(i, j, value);
			}
		}
	}

END_OF_PASTE:
	if (!sErrMsg.IsEmpty())
	{
		SetWarningText(sErrMsg);
		return FALSE;
	}
	return TRUE;
}

BOOL CDgnSteelSPCSectJGrid::OnValidateField(ROWCOL nRow, ROWCOL nCol)
{
	CString value;
	//int nColID;
	//VERIFY(GetColumnIDFor(nCol, nColID));
	value = GetValueRowCol(nRow, nCol);

	if (!ValidateField(value, nCol)) return SetFieldValidation(FALSE);
	return SetFieldValidation(TRUE);
}

BOOL CDgnSteelSPCSectJGrid::ValidateField(CString& value, int nColID)
{
	int nErrCode=0;
	if (nColID == 2 || nColID == 3 || nColID == 4) 
		IsValidDoubleNumber(value, nErrCode);  
	else
		return TRUE;

	switch(nErrCode)
	{
		case 1: SetWarningText(_LS(IDS_WG_DGN_INVALID_FORMULA)); break;
		case 2: SetWarningText(_LS(IDS_WG_DGN_CHARACTER_EXIST)); break;
		case 3: SetWarningText(_LS(IDS_WG_DGN_BIG_VALUE)); break;
		case 4: SetWarningText(_LS(IDS_WG_CMD__ADDD__Integer_Is_Required)); break;
	}
	if (nErrCode != 0)
		return FALSE;

	return TRUE;
}