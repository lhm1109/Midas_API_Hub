// TBBrowserWnd.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMLoadESEQTBGrid.h"

#include "..\wg_common\wg_common_TBGrid.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCMLoadESEQParam
CCMLoadESEQParam::CCMLoadESEQParam()
{
	m_bIsBrowseInit = FALSE;
	m_nCurrentRow = 0;
	m_nRecordCount = 0;
}


/////////////////////////////////////////////////////////////////////////////
// CTBBrowserArrowRowHeader

CCMLoadESEQArrowRowHeader::CCMLoadESEQArrowRowHeader(CCMLoadESEQTBGrid* pBrowserGrid)
	: CGXControl(pBrowserGrid)
{
	m_bmPencil.LoadBitmap(GX_IDB_PENCIL);
	m_bmStar.LoadBitmap(GX_IDB_STAR);
	m_bmArrow.LoadBitmap(GX_IDB_ARROW);

	m_pBrowserGrid = pBrowserGrid;

	m_bWin95Look = FALSE;
}

void CCMLoadESEQArrowRowHeader::Draw(CDC* pDC, CRect rect, ROWCOL nRow, ROWCOL nCol, const CGXStyle& style, const CGXStyle* pStandardStyle)
{
	// no arrow needed when printing
	if (Grid()->IsPrinting())
		return;

	// Is it the current row?
	BOOL bCurrentRow = nRow > 0 && Grid()->IsCurrentCell(nRow);

	// Is current row in edit mode?
	BOOL bPencil = m_pBrowserGrid->IsEdit(); //m_pBrowserGrid->m_nEditMode != CTBBrowserWnd::noMode;

	// Is it the append row?
	BOOL bAppendRow = m_pBrowserGrid->IsAppendRow(nRow);

	// Background
	DrawBackground(pDC, rect, style);

	// Draw button look (if not printing and not
	// explicitly specified as raised or sunken)
	if (!Grid()->IsPrinting()
		&& Grid()->GetParam()->GetProperties()->GetDisplay3dButtons()
		&& (style.GetIncludeDraw3dFrame() || style.GetDraw3dFrame() == gxFrameNormal))
	{

		if (m_bWin95Look)
		{
			// Draw Raised with better 3d-effect (looks better
			// if grid lines are turned off)
			GXDrawEdge(pDC, rect, BDR_RAISEDOUTER | BDR_RAISEDINNER);
		}
		else
		{
			COLORREF rgb3dDkShadow = RGB(0, 0, 0);
			COLORREF rgb3dHilight = GXGetSysData()->GetSysColor(COLOR_BTNHIGHLIGHT);

			// Draw Raised like a normal CGXHeader (looks better
			// when grid lines are turned on)
			GXDraw3dFrame(pDC, rect.left, rect.top, rect.right-1, rect.bottom-1, 1,
				rgb3dHilight, rgb3dDkShadow);
		}
	}

	// choose bitmap for the header
	CBitmap* pBitmap = NULL;

	if (bCurrentRow && bPencil)
		pBitmap = &m_bmPencil;
	else if (bAppendRow)
		pBitmap = &m_bmStar;
	else if (bCurrentRow)
		pBitmap = &m_bmArrow;

	if (pBitmap)
	{
		// Bitmap size
		BITMAP     bm;
		pBitmap->GetObject(sizeof(BITMAP), (LPTSTR)&bm);

		CPoint     ptSize;
		ptSize.x = bm.bmWidth;            // Get width of bitmap
		ptSize.y = bm.bmHeight;           // Get height of bitmap
		//pDC->DPtoLP(&ptSize, 1);      // Convert from device to logical points

		// Draw bitmap
		if (rect.Width() >= ptSize.x && rect.Height() >= ptSize.x)
		{
			// must have at least the first bitmap loaded before calling DrawItem
			ASSERT(pBitmap->m_hObject != NULL);     // required

			int x = rect.left + max(1, (rect.Width()-ptSize.x)/2);
			int y = rect.top + max(1, (rect.Height()-ptSize.y)/2);

			GXDrawTransparentBitmap(pDC, // The destination DC.
				pBitmap, // The bitmap to be drawn.
				x,        // X coordinate.
				y,        // Y coordinate.
				RGB(192,192,192)); // The color for transparent
								   // pixels (white grey).

		}
	}

	CGXControl::Draw(pDC, rect, nRow, nCol, style, pStandardStyle);
}

/////////////////////////////////////////////////////////////////////////////
// CCMLoadESEQTBGrid
IMPLEMENT_DYNAMIC(CCMLoadESEQTBGrid, CTBGrid);

CCMLoadESEQTBGrid::CCMLoadESEQTBGrid()
{
	m_bRedrawOnMovedCurrentCell = TRUE;
	m_nEditMode = noMode;  

	m_bDisplayOnly = FALSE;
	m_bLockEdit = FALSE;
	m_bCanInsertRowByPressingKey = TRUE;
	m_bCanDeleteRowByPressingKey = TRUE;
	m_bEnabledAppend = TRUE;
}

CCMLoadESEQTBGrid::~CCMLoadESEQTBGrid()
{
}

BOOL CCMLoadESEQTBGrid::IsNumeric(CString str, int& ecode)
{
	if (str.GetLength() < 1) return FALSE;

	TCHAR *num = new TCHAR[str.GetLength()+1];
	TCHAR *endp;
	double n;

	_stprintf(num, str);
	n = _tcstod(num, &endp);
	BOOL bAllScaned = endp == &num[str.GetLength()];
	delete[] num;

	if (n == HUGE_VAL || n == -HUGE_VAL) 
		ecode = 1;
	else if (!bAllScaned) 
		ecode = 2;
	else ecode = 0;

	if (ecode) return FALSE;
	else return TRUE;
}

BEGIN_MESSAGE_MAP(CCMLoadESEQTBGrid, CTBGrid)
	//{{AFX_MSG_MAP(CCMLoadESEQTBGrid)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CCMLoadESEQTBGrid message handlers

void CCMLoadESEQTBGrid::Initialize()
{
	CGXGridWnd::Initialize();
	
	// cell 바깥영역 배경색 지정 
	GetParam()->GetProperties()->SetColor(GX_COLOR_BACKGROUND, CTBGrid::m_Color.OuterBackground);
	
	// Header 배경색 지정 
	ChangeColHeaderStyle(CGXStyle().SetInterior(CTBGrid::m_Color.RowColHeaderBg)); 
	ChangeRowHeaderStyle(CGXStyle().SetInterior(CTBGrid::m_Color.RowColHeaderBg)); 
	
	// 기타 Cells 배경색 지정
	ChangeStandardStyle(CGXStyle( ).SetInterior(CTBGrid::m_Color.NormalModeBg));

	GetParam()->EnableUndo(FALSE);
	InitBrowserSettings();
	SetDrawingTechnique(gxDrawUsingMemDC);

	// Excel과 같은 셀 선택 : DoLButtonDblClk 참조
	GetParam()->SetActivateCellFlags(GX_CAFOCUS_DBLCLICKONCELL);

	/*
	ChangeStandardStyle(CGXStyle()
		.SetTextColor(m_Color.NormalModeFg)
		.SetInterior(m_Color.NormalModeBg));
	*/

	SetRowCount(1);
}

void CCMLoadESEQTBGrid::InitBrowserSettings()
{
	// Initialize pointer to some objects
	CGXGridParam* pParam = GetParam();
	ASSERT_VALID(pParam);

	// Disable direct copy and paste
	pParam->m_bDirectCopyPaste = FALSE;
	pParam->m_bDirectCutPaste = FALSE;
	m_nClipboardFlags &= ~GX_DNDSTYLES;

	CGXStylesMap* pStyMap = pParam->GetStylesMap();
	ASSERT_VALID(pStyMap);

	CGXProperties* pProp = pParam->GetProperties();
	ASSERT_VALID(pProp);

	CCMLoadESEQParam* pBrowseData = GetBrowseParam();

	// Register user-defined controls
	RegisterControl(
	  IDS_CMD_ESEQ_BROWSEROWHEADER,
	  new CCMLoadESEQArrowRowHeader(this)
		);  
	/*
	RegisterControl(
		IDS_TB_CTRL_BROWSECOLHEADER,
		new CTBSortColHeader(this)
		);
	*/
/*  RegisterControl(
		IDS_TB_CTRL_CHECKLIST_COMBOBOX,
		new CTBCheckListComboBox(this)
		);  */

	if (!pBrowseData->m_bIsBrowseInit)
	{
		BOOL bUndo = GetParam()->IsEnableUndo();
		GetParam()->EnableUndo(FALSE);

		// Following settings should only be initialized the first
		// time OnInitialUpdate is called. This is especially
		// usefull if the parameter-object is shared among
		// several views (MFC doc/view support) because OnInitialUpdate
		// is called for every new view.

		// Row and column headers
		ColHeaderStyle()
					.SetReadOnly(FALSE)
					.SetEnabled(FALSE) // Current cell cannot be moved to header
					//.SetControl(IDS_TB_CTRL_BROWSECOLHEADER)
					;

		RowHeaderStyle()
					.SetEnabled(FALSE)
					.SetControl(IDS_CMD_ESEQ_BROWSEROWHEADER); // use previously registered header control

		// Special grid settings
		pParam->EnableMoveRows(FALSE);                  // rows cannot be dragged
		pParam->EnableMoveCols(FALSE);                   // columns can be dragged
		pParam->EnableTrackRowHeight(GX_TRACK_ALL);     // all rows have the same height.
		pParam->EnableThumbTrack(FALSE);                // thumb-tracking is disabled

		pParam->SetNumberedRowHeaders(FALSE);
		pParam->SetNumberedColHeaders(FALSE);

		// Do not allow selecting range of cells in the table
		// pParam->EnableSelection(GX_SELTABLE | GX_SELROW | GX_SELCOL);
		pParam->EnableSelection(GX_SELROW | GX_SELCELL);

		// set focus to control when moving current cell
		// pParam->SetActivateCellFlags(GX_CAFOCUS_SETCURRENT);

		// removing rows is not undoable for this kind of grid
		pParam->SetRemoveColsFlags(FALSE);
		pParam->SetRemoveRowsFlags(FALSE);

		pProp->SetMarkColHeader(FALSE);                 // Turn off pressed button effect for column headers
		pProp->SetPrintRowHeaders(FALSE);               // Don't print column headers

		// set row header width to 22 pixels
		SetColWidthByDPI(0, 0, 22);

		pBrowseData->m_bIsBrowseInit = TRUE;
		GetParam()->EnableUndo(bUndo);
	}
}

BOOL CCMLoadESEQTBGrid::SetRowCount(ROWCOL nRows, UINT flags)
{
	long nRowCount = GetBrowseParam()->m_nRecordCount;

	GetBrowseParam()->m_nRecordCount = nRows-1;
	if (CGXGridWnd::SetRowCount(nRows, flags)) return TRUE;
	GetBrowseParam()->m_nRecordCount = nRowCount;
	return FALSE;
}

BOOL CCMLoadESEQTBGrid::GetColumnNoFor(CString title, ROWCOL &nCol)
{
	ROWCOL nColCount = GetColCount();
	for (ROWCOL i = 0; i < nColCount; i++)
		if (GetValueRowCol(0, i+1) == title) 
		{
			nCol = i + 1;
			return TRUE;
		}
	return FALSE;
}

const CString& CCMLoadESEQTBGrid::GetValueRowCol(ROWCOL nRow, ROWCOL nCol)
{
	static CString value;
	value = _T(""); // 초기화 한다.

	// 요청된 셀이 현재 셀이 아닐 때
	ROWCOL ncRow, ncCol;
	VERIFY(GetCurrentCell(ncRow, ncCol));
	if (ncRow != nRow || ncCol != nCol)
		return CGXGridWnd::GetValueRowCol(nRow, nCol);

	// 셀이 Active 상태가 아니거나 수정되지 않으면 원래 함수 결과 리턴
	CGXControl *pControl = GetControl(nRow, nCol);
	if (!pControl || !pControl->IsActive() || !pControl->GetModify())
		return CGXGridWnd::GetValueRowCol(nRow, nCol);

	// 현재 셀이 Active 상태이고 수정이 되었으면 셀을 저장하고
	// 값을 가져온 후 셀을 이전 상태로 다시 복구한다.
	// 왜 이렇게 하냐 하면 Formula가 들어가면 셀에 저장되어야 값이
	// Evaluate되기 때문이다. 만약 Formula를 직접 Evaluate해 주면
	// 그냥 Control의 값을 리턴하면 된다. (땜빵 코드)
	// 그런데... One(Zero) based ComboBox의 경우는 저장되는 것이 숫자인 경우가
	// 있다. 이런 경우에는 저장/복구 과정을 통해야 한다.
	BOOL bCombo = pControl->IsKindOf(CONTROL_CLASS(CGXComboBox));
	if (GetSheetContext() == NULL && !bCombo) 
	{
		pControl->GetCurrentText(value);
		return value;
	}

	CString cur, prev;
	int nStart, nEnd;
	BOOL bEdit;

	bEdit = pControl->CGXControl::IsKindOf(CONTROL_CLASS(CGXEditControl));
	if (bEdit) ((CGXEditControl*)pControl)->GetSel(nStart, nEnd);

	// 현재 텍스트를 cur 변수에 저장한다.
	pControl->GetCurrentText(cur);
	// 이전 값을 prev 변수에 저장한다.
	prev = GetExpressionRowCol(nRow, nCol);
	// 현재 값을 셀에 저장. 저장이 안 되면 ""  리턴 ???
	if (!pControl->Store()) 
	{
		pControl->SetCurrentText(cur);
		pControl->SetActive(TRUE);
		return value;
	}
	// 저장된 값을 가져온다.
	value = CGXGridWnd::GetValueRowCol(nRow, nCol);
	// 이전 값을 셀에 저장하기 위해 Control에 값 재 설정 : version1
	// 에러 요소 : Side Effect로 OnModifyCell이 호출되어 결국은 
	// CCMLoadESEQTBGrid::Edit까지 호출
	/*
	pControl->SetCurrentText(prev);
	// 셀에 값 설정
	pControl->Store();
	*/
	// 새로운 방법: SetExpressionRowCol을 그냥 호출하면 이전 값을 그대로
	// 복구할 수 있다. 셀에서 ESC를 쳐서 편집 취소하면 이전값으로 복구된다.
	CGXGridWnd::SetExpressionRowCol(nRow, nCol, prev); // do nothing...

	// 편집중인 값을 컨트롤에 설정
	// pControl->SetCurrentText는 OnModifyCell을 호출하게 되므로 여기서
	// 다른 방법을 강구해야 한다.
	//pControl->SetCurrentText(cur);
	pControl->SetControlText(nRow, nCol, cur);
	pControl->SetActive(TRUE);
	if (bEdit) ((CGXEditControl*)pControl)->SetSel(nStart, nEnd);

	return value;
}

// 편집중인 셀의 편집 전 값을 가져오기 위해 사용
CString CCMLoadESEQTBGrid::GetValueRowColBeforeEdit(ROWCOL nRow, ROWCOL nCol)
{
	if (m_nEditMode == edit && nRow == GetBrowseParam()->m_nCurrentRow)
	{
		return m_EditRecordStyle.GetAt(nCol-1).GetValue();
		/*
		if (value.GetLength() > 0 && value.GetAt(0) == _T('='))
		{
			// 수식인 경우 결과 값을 리턴한다.
		}
		*/
		//return value;
	}
	return GetValueRowCol(nRow, nCol);
}

BOOL CCMLoadESEQTBGrid::ConvertFormulaToValue(CString& value)
{
	if (GetSheetContext() == NULL && value.GetLength() > 0 && value.GetAt(0) == _T('='))
	{
		// Formula engine이 꺼져 있는데 수식이 입력되어 있으면
		CGXFormulaSheet fs;
		CGXFormula fm;
		if (!fs.ParseExpression(value, fm)) 
		{
			SetWarningText(_LS(IDS_WG_CMD__ADDD__Error___Invalid_formula_));
			return FALSE;
		}
		if (!fs.EvaluateExpression(value, fm))
		{
			SetWarningText(_LS(IDS_WG_CMD__ADDD__Error___Can_t_evaluate_formula_));
			return FALSE;
		}
	}
	return TRUE;
}

BOOL CCMLoadESEQTBGrid::CanAppend()
{
	if (IsReadOnly()) return FALSE;
	if (GetBrowseParam()->m_nRecordCount == LONG_MAX) return FALSE;
	return TRUE;
}

BOOL CCMLoadESEQTBGrid::IsAppendRow(ROWCOL nRow)
{
	/*
	unsigned add = (m_nEditMode == addnew) ? 1 : 0;
	if (nRow == GetBrowseParam()->m_nRecordCount+1+add) return TRUE;
	*/
	if (nRow == GetRowCount()) return TRUE;
	return FALSE;
}

BOOL CCMLoadESEQTBGrid::GetStyleRowCol(ROWCOL nRow, ROWCOL nCol, CGXStyle& style, GXModifyType mt, int nType)
{
	BOOL bReadOnlyMode;
	BOOL bCurrentRow;
	BOOL bCurrentCell;
	BOOL bEnabled;
	BOOL bReadOnly;
	BOOL bAppendRow;
	BOOL bHeaderCol;

	ROWCOL ncRow, ncCol;    // 현재 Row, Col
	CGXGridWnd::GetStyleRowCol(nRow, nCol, style, mt, nType);

	// 현재 셀의 그리드 좌표(Row, Col 번호)를 얻어온다.
	if (GetCurrentCell(ncRow, ncCol)) GetBrowseParam()->m_nCurrentRow = ncRow;
	else ncRow = GetRowCount()+1; // 현재 셀이 없는 경우 : ncRow를 범위 밖으로 설정

	// 현재 셀이 이동되면 새로 그려준다.
	m_bRefreshOnSetCurrentCell = TRUE;

	// Row나 Col이 1보다 작으면 리턴한다.(Standard Row or Column Header)
	if (nRow < 3 || nCol < 1) return TRUE;

	// Get Current Table and Cell State
	bReadOnlyMode = IsLockEdit();
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
	else // Column style에서 enable 정보 알아냄
	{
		CGXStyle styleEx;
		GetColStyle(nCol, styleEx);
		bEnabled = (!styleEx.GetIncludeEnabled() || styleEx.GetEnabled());
	}

	if (style.GetIncludeReadOnly()) // 셀에 readonly정보 포함 여부 알아냄
	{
		bReadOnly = style.GetReadOnly();
	}
	else // Column style에서 readonly 정보 알아냄
	{
		CGXStyle styleEx;
		GetColStyle(nCol, styleEx);
		bReadOnly = (styleEx.GetIncludeReadOnly() && styleEx.GetReadOnly());
	}

	bAppendRow = IsAppendRow(nRow);
	bHeaderCol = IsFrozenCol(nCol);

	// Printing 모드이면 헤더 Column이 프린트 되기 위해 다음과 같이 리턴한다.
	if (IsPrinting())
	{
		if (bHeaderCol) style.SetControl(GX_IDS_CTRL_HEADER);
		return TRUE;
	}

	// 일반 셀이 가장 많으므로 빨리 검사하고 바로 리턴할 수 있게 코드를 앞으로 뺀다.
	if (bEnabled && !bReadOnly && !bCurrentCell && !bCurrentRow && !bAppendRow && !bHeaderCol)
	{
		if (bReadOnlyMode) style
			.SetInterior(m_Color.ReadOnlyModeBg)
			.SetTextColor(m_Color.ReadOnlyModeFg);
		else style
			.SetInterior(m_Color.NormalModeBg)
			.SetTextColor(m_Color.NormalModeFg);

		return TRUE;
	}

	// Disable되었을 때
	if (!bEnabled)  
	{
		if (bHeaderCol) style           // Row or Column Header
			.SetControl(GX_IDS_CTRL_HEADER)
			.SetInterior(m_Color.RowColHeaderBg)
			.SetTextColor(m_Color.NormalModeFg);
		else if (bCurrentRow) style     // Disabled Current Row
			.SetInterior(m_Color.DisabledBg)
			.SetTextColor(m_Color.ReadOnlyCurrentRowFg);
		else style                      // Disabled Cell
			.SetInterior(m_Color.DisabledBg)
			.SetTextColor(m_Color.DisabledFg);

		if (!bHeaderCol && bCurrentRow) SetCurrentRowStyle(style);

		return TRUE;
	}

	// Disable안 되고 ReadOnly만 설정되었을 때
	if (bReadOnly)
	{
		if (bCurrentCell) style         // Current Cell
			.SetInterior(m_Color.ReadOnlyCurrentCellBg)
			.SetTextColor(m_Color.ReadOnlyCurrentCellFg);
		else if (bCurrentRow) style     // Current Row
			.SetInterior(m_Color.ReadOnlyCurrentRowBg)
			.SetTextColor(m_Color.ReadOnlyCurrentRowFg);
		else style                      // Readonly Cell
			.SetInterior(m_Color.ReadOnlyBg)
			.SetTextColor(m_Color.ReadOnlyFg);

		if (bCurrentRow) SetCurrentRowStyle(style);

		return TRUE;
	}

	// Append Row가 Disable되었을 때(EnableAppendRow(FALSE)로 금지된 경우)
	if (bAppendRow && !m_bEnabledAppend)  // Disabled Apppend Row
	{
		if (bCurrentCell) style
			.SetInterior(m_Color.ReadOnlyCurrentCellBg)
			.SetTextColor(m_Color.ReadOnlyCurrentCellFg);
		else if (bCurrentRow) style
			.SetInterior(m_Color.DisabledAppendRowBg)
			.SetTextColor(m_Color.ReadOnlyCurrentRowFg);
		else style
			.SetInterior(m_Color.DisabledAppendRowBg)
			.SetTextColor(m_Color.ReadOnlyModeFg);

		style.SetControl(GX_IDS_CTRL_STATIC);  // 수정이 안되도록 control을 static으로 바꾼다.

		if (bCurrentRow) SetCurrentRowStyle(style);

		return TRUE;
	}
	
	// Disable 안되고, ReadOnly 아니고, Append가 Disable 되지 않은 경우
	// Current Cell or Current Row or Enabled Append Row인 경우이다.
	if (bReadOnlyMode)
	{
		if (bCurrentCell) style
			.SetInterior(m_Color.ReadOnlyCurrentCellBg)
			.SetTextColor(m_Color.ReadOnlyCurrentCellFg);
		else if (bCurrentRow) style
			.SetInterior(m_Color.ReadOnlyCurrentRowBg)
			.SetTextColor(m_Color.ReadOnlyCurrentRowFg);
		else if (bAppendRow) style
			.SetInterior(m_Color.ReadOnlyModeBg)
			.SetTextColor(m_Color.ReadOnlyModeFg);
		else if (bHeaderCol) style    // Enabled Header Col(Only Frozen)
			.SetInterior(m_Color.ReadOnlyModeBg)
			.SetTextColor(m_Color.ReadOnlyModeFg);

		if (bCurrentRow || bHeaderCol) SetCurrentRowStyle(style);

		return TRUE;
	}
	else
	{
		if (bCurrentCell) style
			.SetInterior(m_Color.NormalCurrentCellBg)
			.SetTextColor(m_Color.NormalCurrentCellFg);
		else if (bCurrentRow) style
			.SetInterior(m_Color.NormalCurrentRowBg)
			.SetTextColor(m_Color.NormalCurrentRowFg);
		else if (bAppendRow) style
			.SetInterior(m_Color.NormalModeBg)
			.SetTextColor(m_Color.NormalModeFg);
		else if (bHeaderCol) style    // Enabled Header Col(Only Frozen)
			.SetInterior(m_Color.NormalModeBg)
			.SetTextColor(m_Color.NormalModeFg);

		if (bCurrentRow || bHeaderCol) SetCurrentRowStyle(style);

		return TRUE;
	}

	return TRUE;
}

void CCMLoadESEQTBGrid::SetCurrentRowStyle(CGXStyle& style)
{
	if (m_bCurrentRowOutline) style
		.SetBorders(gxBorderTop, CGXPen().SetWidth(2).SetColor(m_Color.NormalModeFg))
		.SetBorders(gxBorderBottom, CGXPen().SetWidth(1).SetColor(m_Color.NormalModeFg));
}

/*
BOOL CCMLoadESEQTBGrid::GetStyleRowCol(ROWCOL nRow, ROWCOL nCol, CGXStyle& style, GXModifyType mt, int nType)
{
	CGXGridWnd::GetStyleRowCol(nRow, nCol, style, mt, nType);

	// 현재 셀이 이동되면 새로 그려준다.
	m_bRefreshOnSetCurrentCell = TRUE;

	BOOL bCurrentRow;
	// Update Current Row
	ROWCOL ncRow, ncCol;
	if (GetCurrentCell(ncRow, ncCol)) GetBrowseParam()->m_nCurrentRow = ncRow;

	if (nRow == GetBrowseParam()->m_nCurrentRow) bCurrentRow = TRUE;
	else bCurrentRow = FALSE;

	if (!IsPrinting() && nRow > 0 && nCol > 0 && bCurrentRow)
	{
		// 현재 셀이면 
		if (IsCurrentCell(nRow, nCol))
		{
		  // draw border in highlight around current cell
			style
				.SetInterior(m_Color.NormalCurrentCellBg)
				.SetTextColor(m_Color.NormalCurrentCellFg)
				//.SetBorders(gxBorderAll, CGXPen().SetWidth(1).SetColor(m_Color.CurrentCellBorderFg))
				;
		}
		else
		{
			// draw all other cells in current row with
			// hihglight color
			style
			.SetInterior(m_Color.NormalCurrentCellBg)
			.SetTextColor(m_Color.NormalCurrentCellFg)
			.SetBorders(gxBorderTop, CGXPen().SetStyle(PS_DOT))
			.SetBorders(gxBorderBottom, CGXPen().SetStyle(PS_DOT))
			;
		}
	}

	if (nRow < 1 || nCol < 1) return TRUE;

	// Cell이 Enable인지 Disable인지 알아본다. ComposeStyleRowCol을 못 쓰기 때문에
	// bSetInCell 변수 도입해서 Cell, Column 순으로 Enable Flag를 조사한다.
	// Cell에 Enable 정보가 있으면 bSetInCell이 True가 되고 Column의 style은 조사하지
	// 않게 된다.
	BOOL bSetInCell = FALSE;
	BOOL bEnabled=TRUE;
	if (style.GetIncludeEnabled())
	{
		bSetInCell = TRUE;
		if (style.GetEnabled()) bEnabled = TRUE;
		else bEnabled = FALSE;
	}

	if (!bSetInCell)  // Cell에 Enable정보가 없으면 Column의 스타일을 조사한다.
	{
		// ComposeStyleRowCol을 GetStyleRowCol에서 부르면 재귀 호출이 일어나게 된다.
		CGXStyle styleEx;
		GetColStyle(nCol, styleEx);
		if ((styleEx.GetIncludeEnabled() && !styleEx.GetEnabled()))
			bEnabled = FALSE;
		else bEnabled = TRUE;
	}
	
	if (!bEnabled)
	{
		if (IsFrozenCol(nCol))
		{
			style
				.SetControl(GX_IDS_CTRL_HEADER)
				.SetInterior(m_Color.ReadOnlyBg)
				.SetTextColor(m_Color.ReadOnlyFg)
				;
		}
		else if (!IsPrinting())
		{
			if (bCurrentRow)
				style
					.SetInterior(m_Color.ReadOnlyBg)
					.SetTextColor(m_Color.NormalCurrentCellFg)
					;
			else
				style
					.SetInterior(m_Color.ReadOnlyBg)
					.SetTextColor(m_Color.NormalCurrentCellFg)
					;
		}
	}
	return TRUE;
}
*/

BOOL CCMLoadESEQTBGrid::SetCurrentCell(ROWCOL nRow, ROWCOL nCol, UINT flags)
{
	//ROWCOL nCurrentRow = GetBrowseParam()->m_nCurrentRow;;
	// SetCurrentCell이 OnLeftCell을 부르게 되어 있다.
	if (CGXGridWnd::SetCurrentCell(nRow, nCol, flags)) 
	{
		GetBrowseParam()->m_nCurrentRow = nRow;
		// 셀 편집중 화살표키로 다른 셀로 이동했을 때 현재 셀이 다시 그려지게 한다.
		RedrawRowCol(CGXRange().SetRows(nRow));
		return TRUE;
	}
	//GetBrowseParam()->m_nCurrentRow = nCurrentRow;
	return FALSE;
}

BOOL CCMLoadESEQTBGrid::OnLeftCell(ROWCOL nRow, ROWCOL nCol, ROWCOL nNewRow, ROWCOL nNewCol)
{
	nCol, nNewCol;

	m_bRedrawOnMovedCurrentCell = FALSE;
	if (nNewRow > 0 && nNewRow != GetBrowseParam()->m_nCurrentRow)
	{
		ROWCOL nOldRow = GetBrowseParam()->m_nCurrentRow;
		GetBrowseParam()->m_nCurrentRow = nNewRow;
		
		RedrawRowCol(CGXRange().SetRows(nOldRow), GX_UPDATENOW, FALSE);

		// Next time, OnMoveCurrentCell is called, redraw the whole row
		m_bRedrawOnMovedCurrentCell = TRUE;
		
		// no further redrawing of current cell necessary
		return FALSE;
	}
	return TRUE;
}

void CCMLoadESEQTBGrid::OnMovedCurrentCell(ROWCOL nRow, ROWCOL nCol)
{
	// Redraw delayed when moved to a new row (see OnLeftCell)
	if (m_bRedrawOnMovedCurrentCell)
	{
		RedrawRowCol(CGXRange().SetRows(nRow), GX_INVALIDATE, FALSE);
	}

	// Store the new row
	if (nRow > 0) GetBrowseParam()->m_nCurrentRow = nRow;

	CGXGridWnd::OnMovedCurrentCell(nRow, nCol);
}

BOOL CCMLoadESEQTBGrid::OnStartEditing(ROWCOL nRow, ROWCOL nCol)
{
	// 이미 추가 모드이거나 편집 모드이면 무시
	if (m_nEditMode == addnew || m_nEditMode == edit)
		return CGXGridWnd::OnStartEditing(nRow, nCol);

	CGXControl* pControl = GetControl(nRow, nCol);
	if (!pControl) return CGXGridWnd::OnStartEditing(nRow, nCol);

	// ComboBox 종류는 무시해야 한다.
	// 문제는 이것이 콤보 박스인지 아닌지 어떻게 아느냐 하는 것이다.
	// style 정보에서 가져 오는 것 : 문제 있음.
	if (pControl->IsKindOf(RUNTIME_CLASS(CGXComboBoxWnd)))
		return CGXGridWnd::OnStartEditing(nRow, nCol);

	// edit control은 실제 고쳐질때 실행
	if (pControl->CGXControl::IsKindOf(CONTROL_CLASS(CGXEditControl)))
		return CGXGridWnd::OnStartEditing(nRow, nCol);

	// Button Type(radio , check box...)
	// 이 타입은 DECLARE_DYNAMIC이 선언 안 되어 있기 때문에 아래와 같이
	// DECLARE_CONTROL, CONTROL_CLASS를 이용한다. 
	BOOL bButtonType = FALSE;
	
	if (pControl->CGXControl::IsKindOf(CONTROL_CLASS(CGXCheckBox))
		|| pControl->CGXControl::IsKindOf(CONTROL_CLASS(CGXRadioButton))
		) bButtonType = TRUE;

	// check box는 무조건 실행(왜냐하면 클릭되면 값이 바뀌기 때문)
	if (!bButtonType) 
		return CGXGridWnd::OnStartEditing(nRow, nCol);

	// go into addnew or edit mode
	if (IsAppendRow(nRow)) AddNew(nRow);
	else Edit(nRow);
	return CGXGridWnd::OnStartEditing(nRow, nCol);
}

BOOL CCMLoadESEQTBGrid::OnEndEditing(ROWCOL nRow, ROWCOL nCol)
{
	BOOL bModified = GetControl(nRow, nCol)->GetModify();

	BOOL bResult = CGXGridWnd::OnEndEditing(nRow, nCol);
	if (bModified)
	{
		CString value = GetValueRowCol(nRow, nCol);
		if (ConvertFormulaToValue(value)) SetExpressionRowCol(nRow, nCol, value);
	}
	return bResult;
}

void CCMLoadESEQTBGrid::OnModifyCell(ROWCOL nRow, ROWCOL nCol)
{
	if (m_nEditMode == addnew || m_nEditMode == edit)
	{
		CGXGridWnd::OnModifyCell(nRow, nCol);
		return;
	}

	// row 추가 또는 수정 모드로 들어간다.
	if (IsAppendRow(nRow)) AddNew(nRow);
	else Edit(nRow);

	CGXGridWnd::OnModifyCell(nRow, nCol);
}

BOOL CCMLoadESEQTBGrid::AddNew(ROWCOL nRow)
{
	m_nEditMode = addnew;

	// InsertRows를 수행할 때 Current Cell이 포커스를 잃는 것을 막기 위해
	// SetActive(FALSE)를 한 후 InsertRows()를 하고 SetActive(TRUE)를 한다.
	// 이것은 InsertRows의 내부에서 TransferCurrentCell을 호출하게 되는데
	// TransferCurrentCell은 현재 컨트롤이 Active상태이면 컨트롤의 내용을
	// 저장하고 컨트롤을 Deactive시키기 때문이다.
	ROWCOL ncRow, ncCol;
	GetCurrentCell(ncRow, ncCol);
	CGXControl* pControl = GetControl(ncRow, ncCol);
	BOOL bActive = FALSE;
	if (pControl && pControl->IsActive()) bActive = TRUE;
	if (bActive) pControl->SetActive(FALSE);

	InsertRows(nRow+1, 1);

	if (bActive) pControl->SetActive(TRUE);

	RedrawRowCol(nRow, 0);
	OnInsertEmptyRecord(nRow);
	return TRUE;
}

BOOL CCMLoadESEQTBGrid::Edit(ROWCOL nRow)
{
	m_nEditMode = edit;
	RedrawRowCol(nRow, 0);
	// m_EditRecord에 값을 저장한다. Editting을 cancel하였을 때 복구를 위해서 필요
	CString value;

	ROWCOL nColCount = GetColCount();
	for (unsigned int i = 0; i < nColCount; i++)
	{
		//value = GetValueRowCol(nRow, i+1);
		//GetStyleRowCol은 기존 style변수를 엎어쓰지 않고 GetInclude???가 
		// 1인 비트만 카피하는 성질이 있으므로 GetStyleRowCol을 호출하는
		// 경우에는 이에 해당하는 변수를 지역 변수로 사용해야 한다.
		CGXStyle style;
		value = GetExpressionRowCol(nRow, i+1);
		m_EditRecord.SetAtGrow(i, value);
		CGXGridWnd::GetStyleRowCol(nRow, i+1, style);
		m_EditRecordStyle.SetAtGrow(i, style);
	}
	return TRUE;
}

// 레코드 편집을 취소하였을 때 호출된다.
BOOL CCMLoadESEQTBGrid::CancelEdit()
{
	ROWCOL nRow, nCol;
	GetCurrentCell(nRow, nCol);
	BOOL bOldLock = LockUpdate(TRUE);
	int nCurrentRow = nRow; //GetBrowseParam()->m_nCurrentRow;
	if (m_nEditMode == addnew)
	{
		// 추가된 레코드를 없앤다.
		//CGXControl* pControl = GetControl(nRow, nCol);
		//pControl->SetActive(FALSE);
		if (!RemoveRows(nCurrentRow, nCurrentRow))
		{
			AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error__Can_t_remove_row___));
		}
		BOOL bSet = SetDownValidCell(nCurrentRow, nCol);
		if (!bSet) bSet = SetRightValidCell(nCurrentRow, nCol);
		if (!bSet) bSet = SetLeftValidCell(nCurrentRow, nCol);
		ASSERT(bSet);

		OnCanceledAddNew();
	}
	else // 편집 모드이면
	{
		// 이전 값을 복구한다.
		if ((unsigned)m_EditRecord.GetSize() < GetColCount()) 
		{
			TRACE(_LS(IDS_WG_CMD__ADDD__Number_of_fields_mismatch__n));
			return FALSE;
		}
		CString value;
		GetParam()->SetLockReadOnly(FALSE);
		CGXStyle style, curStyle;
		ROWCOL nColCount = GetColCount();
		for (unsigned i = 0; i < nColCount; i++)
		{
			//SetValueRange(CGXRange(GetBrowseParam()->m_nCurrentRow, i+1),
			//  m_EditRecord.GetAt(i));

			// SetStyleRange는 GetInclude???가 0이면 값을 바꾸지 않으므로
			// 저장된 style로 바로 복구할 수 없다.
			// 그래서 먼저 기존 style을 제거하고 저장된 style로 새로 설정한다.
			style = m_EditRecordStyle.GetAt(i);
			SetStyleRange(CGXRange(GetBrowseParam()->m_nCurrentRow, i+1), NULL, gxRemove);
			SetStyleRange(CGXRange(GetBrowseParam()->m_nCurrentRow, i+1), style);

			value = m_EditRecord.GetAt(i);
			if (value.GetLength() > 0 && value.GetAt(0) == _T('=')) // Expression이면 Expression을 설정한다.
			{
				SetExpressionRowCol(GetBrowseParam()->m_nCurrentRow, i+1, value);
			}
		}
		GetParam()->SetLockReadOnly(TRUE);
		OnCanceledModify();
	}
	LockUpdate(bOldLock);
	m_nEditMode = noMode;
	if (!bOldLock) Redraw();
	return TRUE;
}

// SetLeftValidCell, SetRightValidCell, SetDownValidCell은 side effect로서
// 현재 편집중인 셀이 저장되고 컨트롤은 deactivate된다.
// 현재 편집중인 레코드가 없으면 side effect는 없다.
// 컨트롤을 active로 복구하려면 Refresh를 호출해야 한다.
BOOL CCMLoadESEQTBGrid::SetLeftValidCell(ROWCOL nRow, ROWCOL nCol)
{

	ROWCOL i, j;

	i = nRow; j = nCol-1; // 현재 행 이전 열
	if (j < 1) { i--; j = GetColCount(); }  

	while (i >= 1)
	{
		while (j >= 1)
		{
			// 열이 선택될 수 있는지 조사
			if (SetCurrentCell(i, j)) return TRUE;
			j--;  // 이전 열을 조사
		}
		i--;  // 이전 줄로 이동
		j = GetColCount();
	}

	return FALSE;
}

BOOL CCMLoadESEQTBGrid::SetRightValidCell(ROWCOL nRow, ROWCOL nCol)
{
	ROWCOL i, j;
	i = nRow; j = nCol+1; // 현재 행 다음 열
	if (j > GetColCount()) { i++; j = 1; }

	while (i <= GetRowCount())
	{
		while (j <= GetColCount())
		{
			// 열이 선택될 수 있는지 조사
			if (SetCurrentCell(i, j)) return TRUE;
			j++;  // 다음 열을 조사
		}
		i++;  // 다음 줄로 이동
		j = 1;
	}
	return FALSE;
}

BOOL CCMLoadESEQTBGrid::SetDownValidCell(ROWCOL nRow, ROWCOL nCol)
{
	while(nRow <= GetRowCount())
	{
		if (SetCurrentCell(nRow, nCol)) return TRUE;
		nRow++;
	}
	return FALSE;
}

BOOL CCMLoadESEQTBGrid::CanSelectCurrentCell(BOOL bSelect, ROWCOL dwSelectRow, ROWCOL dwSelectCol, ROWCOL dwOldRow, ROWCOL dwOldCol)
{
	BOOL bValid;

	// CGXBrowserWnd::CanSelectCurrentCell()가 호출되면 Update가 되어 m_nEditMode = noMode가 된다.
	// 또한 addnew에서는 레코드가 기록되어 버린다.

	// 먼저 새 셀이 선택되어 질 수 있는지 본다.
	CGXStyle style;
	if (dwSelectRow < 3 || dwSelectCol < 1) return FALSE;

	// 원래는 ASSERT 처리였으나 TABLE중 새로 입력시 같은 Key가 입력되면 
	// 원래키에 중복하여 새로 한 레코드가 늘어나지 않는 경우가 있다.
	// 이 경우 ENTER키로 입력되었을 때 그다음 줄 첫번째 열을 current cell로
	// 설정하려고 하나 Row 갯수가 1개 적어져 dwSelectRow가 GetRowCount()보다
	// 큰 경우가 발생한다. 그래서 이 경우 그냥 FALSE를 리턴하도록 하였다.
	//ASSERT(dwSelectRow <= GetRowCount() && dwSelectCol <= GetColCount());
	if (dwSelectRow > GetRowCount() || dwSelectCol > GetColCount()) return FALSE;

	ComposeStyleRowCol(dwSelectRow, dwSelectCol, &style);
	if (style.GetIncludeEnabled() && !style.GetEnabled()) return FALSE;

	// 다른 row로 이동할 때
	if (dwOldRow != 0 && dwOldRow != GetRowCount() && dwOldRow != dwSelectRow)
	{
		int nEditMode = m_nEditMode;

		// 현재 레코드가 새 레코드가 아니고 수정된 레코드도 아니면 default action
		if (nEditMode != addnew && nEditMode != edit) 
			return CGXGridWnd::CanSelectCurrentCell(bSelect, dwSelectRow, dwSelectCol, dwOldRow, dwOldCol);

		// default action을 수행한다.
		bValid = CGXGridWnd::CanSelectCurrentCell(bSelect, dwSelectRow, dwSelectCol, dwOldRow, dwOldCol);
		if (!bValid) return FALSE; // default action에서 에러가 있으면 FALSE 리턴

		// 레코드 Validation을 수행한다.
		// 처음에는 레코드 Validation을 먼저 했는데 이 경우 Field Validation이 있기 전에
		// 레코드 Validation을 먼저 수행할 때가 있어 에러 소지가 있다.
		// (맨 끝 칼럼에서 Active Cell인 상태에서 Return키 칠 때)
		bValid = OnValidateRecord(dwOldRow);
		if (!bValid) return FALSE;

		if (nEditMode == addnew)
		{
			// 새 레코드 추가 모드이면
			if (!OnAddNewRecord(dwOldRow)) return FALSE;
			GetBrowseParam()->m_nRecordCount++;
			m_nEditMode = noMode;
		}
		else if (nEditMode == edit)
		{
			// 기존 레코드 수정 모드이면
			if (!OnModifyRecord(dwOldRow)) return FALSE;
			m_nEditMode = noMode;
		}
		
		return TRUE;
	}

	// 같은 줄이면 default action
	return CGXGridWnd::CanSelectCurrentCell(bSelect, dwSelectRow, dwSelectCol, dwOldRow, dwOldCol);
}

BOOL CCMLoadESEQTBGrid::ProcessKeys(CWnd *pSender, UINT nMessage, UINT nChar, UINT nRepCnt, UINT flags)
{
	CRowColArray awRows;
	ROWCOL nRow=3, nCol=1;
	CGXControl* pControl;
	ROWCOL i, j;
	int nSize;


	BOOL bCtl = GetKeyState(VK_CONTROL) & 0x8000;
	BOOL bShift = GetKeyState(VK_SHIFT) & 0x8000;

	if (bCtl || bShift) 
		return CGXGridWnd::ProcessKeys(pSender, nMessage, nChar, nRepCnt, flags);

	VERIFY(GetCurrentCell(nRow, nCol));
	pControl = GetControl(nRow, nCol);
	if (nMessage == WM_KEYDOWN)
	{
		// 셀이 ACTIVE인 상태에서 좌우 화살표 키가 눌러지면 셀 이동을 하지 않는다.
		if (nChar == VK_LEFT || nChar == VK_RIGHT)
		{
			ROWCOL nRow, nCol;
			if (GetCurrentCell(nRow, nCol))
			{
				CGXControl* pControl = GetControl(nRow, nCol);
				if (pControl && pControl->IsActive()) return TRUE;
			}
		}

		switch(nChar)
		{
		case VK_ESCAPE:
			if (m_nEditMode != noMode)  // 추가모드 또는 수정모드에서
			{
				if (!pControl || !pControl->IsActive()) // 편집중인 셀이 없으면
				{
					CancelEdit(); // 레코드 편집을 취소한다.
					return TRUE;
				}
			}
			// 편집모드 아닌 상태에서 편집중인 셀이 없으면 ESC는 무시한다.
			// 이렇게 함으로써 셀이 편집되지 않았을 때 OnCancelEditing과 
			// OnCanceledEditing이 호출 되는 것을 막는다.
			else if (!pControl || !pControl->IsActive()) 
			{
				// Select된 것을 해제한다.
				SelectRange(CGXRange().SetTable(), FALSE);
				return TRUE;
			}
			break;
		case VK_RETURN: case VK_RIGHT: case VK_TAB:
			// 열의 위치에 상관 없이 동작 : 두번째 버전
			// 마지막 셀이면 그냥 리턴
			if (IsAppendRow(nRow) && nCol == GetColCount()) return TRUE;

			i = nRow; j = nCol+1; // 현재 행 다음 열
			if (j > GetColCount()) { i++; j = 1; }

			m_bValidField = m_bValidRecord = TRUE; m_sWarningText.Empty();
			while (i <= GetRowCount())
			{
				while (j <= GetColCount())
				{
					// 열이 선택될 수 있는지 조사
					if (CanSelectCurrentCell(FALSE, i, j, nRow, nCol))
					{
						if (CanSelectCurrentCell(TRUE, i, j, nRow, nCol))
						{
							SetCurrentCell(i, j); return TRUE;
							return CGXGridWnd::ProcessKeys(pSender, nMessage, nChar, nRepCnt, flags);
						}
						// 이 경우는 레코드 추가시 같은 키의 중복에 의해 레코드가 합쳐진 경우
						// CanSelectCurrentCell(FALSE...)가 불리기 전보다 Row Count가 1 작아진다.
						// CTBOffs을 참고(SetCurrentCell 함수 부분)
						// 이 경우 새로 설정하려는 row가 Row Count보다 커지는 경우가 발생
						// 이걸 막기 위해서 Append Row(마지막 Row)에 SetCurrentCell을 한다.
						if (i > GetRowCount())
						{
							SetCurrentCell(GetRowCount(), j); return TRUE;
						}
					}
					if (!m_bValidField || !m_bValidRecord)
					{
						m_bValidField = m_bValidRecord = TRUE;
						if (m_sWarningText.GetLength() > 0) DisplayWarningText();
						
						// Edit Control이면 포커스를 다시 설정한다.
						pControl->Refresh();
						return TRUE;
					}
					j++;  // 다음 열을 조사
				}
				i++;  // 다음 줄로 이동
				j = 1;
			}

			// 일단 현재 셀이 Deactive 될 수 있는지 본다. 실패하면 에러 리턴 : version1
			/*
			if (pControl && pControl->IsActive() && 
				 !CanSelectCurrentCell(FALSE, i, j, nRow, nCol)) 
			{
				if (!m_bValidRecord && m_sWarningText.GetLength() > 0) 
					DisplayWarningText();
				if (nChar == VK_RIGHT) nChar = VK_RETURN;
				return CGXGridWnd::ProcessKeys(pSender, nMessage, nChar, nRepCnt, flags);
			}

			while (i <= GetRowCount())
			{
				while (j <= GetColCount())
				{
					// 열이 선택될 수 있는지 조사
					if (CanSelectCurrentCell(TRUE, i, j, nRow, nCol))
					{
						if (SetCurrentCell(i, j)) return TRUE;
						else AfxMessageBox(_T("Error to set current cell!"));
					}
					if (!m_bValidRecord)
					{
						m_bValidRecord = TRUE;
						if (m_sWarningText.GetLength() > 0) DisplayWarningText();
						return CGXGridWnd::ProcessKeys(pSender, nMessage, nChar, nRepCnt, flags);
					}
					j++;  // 다음 열을 조사
				}
				i++;  // 다음 줄로 이동
				j = 1;
			}
			*/
			break;
		case VK_LEFT:
			// 열의 위치에 상관 없이 동작 : 두번째 버전
			// 첫번째 셀이면 그냥 리턴
			if (nRow == 1 && nCol == 1) return TRUE;

			i = nRow; j = nCol-1; // 현재 행 이전 열
			if (j < 1) { i--; j = GetColCount(); }

			m_bValidField = m_bValidRecord = TRUE; m_sWarningText.Empty();

			while (i >= 1)
			{
				while (j >= 1)
				{
					// 열이 선택될 수 있는지 조사
					if (CanSelectCurrentCell(FALSE, i, j, nRow, nCol))
					{
						if (CanSelectCurrentCell(TRUE, i, j, nRow, nCol))
						{
							SetCurrentCell(i, j); return TRUE;
							return CGXGridWnd::ProcessKeys(pSender, nMessage, nChar, nRepCnt, flags);
						}
					}
					if (!m_bValidField || !m_bValidRecord)
					{
						m_bValidRecord = TRUE;
						if (m_sWarningText.GetLength() > 0) DisplayWarningText();

						pControl->Refresh();
						return TRUE;
					}
					j--;  // 이전 열을 조사
				}
				i--;  // 이전 줄로 이동
				j = GetColCount();
			}

			// 일단 현재 셀이 Deactive 될 수 있는지 본다. 실패하면 에러 리턴 : version1
			/*
			if (pControl && pControl->IsActive() && 
				 !CanSelectCurrentCell(FALSE, i, j, nRow, nCol)) 
			{
				if (!m_bValidRecord && m_sWarningText.GetLength() > 0)
					DisplayWarningText();
				return CGXGridWnd::ProcessKeys(pSender, nMessage, nChar, nRepCnt, flags);
			}

			while (i >= 1)
			{
				while (j >= 1)
				{
					// 열이 선택될 수 있는지 조사
					if (CanSelectCurrentCell(TRUE, i, j, nRow, nCol))
					{
						if (SetCurrentCell(i, j)) return TRUE;
						else AfxMessageBox(_T("Error to set current cell!"));
					}
					if (!m_bValidRecord)
					{
						m_bValidRecord = TRUE;
						if (m_sWarningText.GetLength() > 0) DisplayWarningText();
						return CGXGridWnd::ProcessKeys(pSender, nMessage, nChar, nRepCnt, flags);
					}
					j--;  // 이전 열을 조사
				}
				i--;  // 이전 줄로 이동
				j = GetColCount();
			}
			*/
			break;
		case VK_INSERT: // 편집중인 레코드 앞에 addnew 레코드 추가
			if (!CanInsertRowByPressingKey()) return TRUE;
			if (IsLockEdit()) return TRUE;  // display only mode면 do nothing!
			if (!InsertEmptyRecord(nRow)) FALSE;
			/*
			if (m_nEditMode != noMode)
			{
				SetWarningText(_T("편집중인 레코드가 있으면 새 레코드를 삽입할 수 없습니다."));
				DisplayWarningText();
				return FALSE;
			}
			bOldLock = LockUpdate(TRUE);
			InsertRows(nRow, 1);
			SetCurrentCell(nRow, 1);
			m_nEditMode = addnew;

			// 컨트롤을 Activate 시킨다.
			pControl = GetControl(nRow, 1);
			ASSERT(pControl);
			pControl->Init(nRow, 1);
			pControl->SetActive(TRUE);
			LockUpdate(bOldLock);
			Redraw();
			*/
			break;
		case VK_DELETE: // 편집중인 레코드 삭제
			if (!CanDeleteRowByPressingKey()) return TRUE;
			// 먼저 범위가 선택되어 있는지 본다. 범위가 우선한다.
			if (IsLockEdit()) return TRUE;  // display only mode면 do nothing!
			m_sWarningText.Empty();
			GetSelectedRows(awRows, TRUE, FALSE);
			nSize = awRows.GetSize();
			
			if (nSize > 0)
			{
				// Append Row는 뺀다.
				if (IsAppendRow(awRows.GetAt(nSize-1))) { nSize--; awRows.RemoveAt(nSize); }
				// Title Row도 뺀다.
				if (nSize > 0 && awRows.GetAt(0) == 0) { nSize--; awRows.RemoveAt(0); }
				if (nSize == 0) return TRUE;
				if (!OnDeleteRecords(awRows))
					if (m_sWarningText.GetLength() > 0) DisplayWarningText();
			}
			else  // 이 경우는 현재 레코드 삭제이다.
			{
				if (IsAppendRow(nRow)) return TRUE;
				if (!OnDeleteRecord(nRow))
					if (m_sWarningText.GetLength() > 0) DisplayWarningText();
			}
			return TRUE;
			break;
		case VK_DOWN:
			// Alt-Down 키를 이용하면 되므로 이 항목은 없앤다.
			/*
			if (pControl && pControl->IsActive() && pControl->IsKindOf(CONTROL_CLASS(CGXComboBox)))
			{
				// CGXComboBox::ShowDropDown을 호출하기 전에 반드시 UpdateWindow()를 
				// 호출해야 한다. 그렇지 않으면 에러가 발생하는 경우가 있다.
				UpdateWindow();
				((CGXComboBox*)pControl)->ShowDropDown(TRUE);
				return TRUE;
			}
			*/
			break;
		case VK_HOME:
			SetRightValidCell(nRow, 0);
			return TRUE;
			break;
		case VK_END:
			SetLeftValidCell(nRow, GetColCount()+1);
			return TRUE;
			break;
		}
	}
	else if (nMessage == WM_CHAR)
	{
		//VERIFY(GetCurrentCell(nRow, nCol));
		//pControl = GetControl(nRow, nCol);
		switch(nChar)
		{
		// 셀이 Inactive 상태에서 Edit 셀에 스페이스를 눌렀을 때 편집을 활성화 시킨다.
		case VK_SPACE:
			if (pControl && !pControl->IsActive() 
				&& pControl->CGXControl::IsKindOf(CONTROL_CLASS(CGXEditControl)))
			{
				if (!pControl->IsInit()) pControl->Init(nRow, nCol);
				CString value = GetExpressionRowCol(nRow, nCol);
				pControl->SetActive(TRUE);
				pControl->Refresh(); // 반드시 호출
				if (!pControl->IsKindOf(CONTROL_CLASS(CGXComboBox)))  // combo box가 아니면
					((CGXEditControl*)pControl)->SetSel(0, value.GetLength());
				return TRUE;
			}
			break;
		}
	}
	return CGXGridWnd::ProcessKeys(pSender, nMessage, nChar, nRepCnt, flags);
}

BOOL CCMLoadESEQTBGrid::DoRButtonDown(UINT flags, CPoint point)
{
	// not used
	flags;
	CWnd* pParent = GetParent();
	WPARAM wParam = MK_RBUTTON;    // key flags 
	LPARAM lParam;
	lParam = point.y << 16; // vertical position of cursor  
	lParam |= point.x;      // horizontal position of cursor 
	m_ptHit = point;        // store for later using

	if (pParent != NULL) pParent->SendMessage(WM_RBUTTONDOWN, wParam, lParam);
	return TRUE;
}

BOOL CCMLoadESEQTBGrid::DoLButtonDblClk(UINT flags, CPoint point)
{
	if (!CGXGridWnd::DoLButtonDblClk(flags, point)) return FALSE;

	// 엑셀처럼 셀 편집 상태가 되게 한다.
	// single click이면 셀 이동, double click이면 셀 편집
	// 더블 클릭시 내용이 선택 안 되게 한다.
	ROWCOL ncRow, ncCol;
	if (!GetCurrentCell(ncRow, ncCol)) return TRUE;
	CGXControl *pControl = GetControl(ncRow, ncCol);
	if (pControl->IsKindOf(CONTROL_CLASS(CGXEditControl)))
	{
		int nStart, nEnd;
		CGXEditControl* pEditCtrl = (CGXEditControl*)pControl;
		pEditCtrl->GetSel(nStart, nEnd);
		// 내용이 Selection 되어 있으면 Selection을 없앤다.
		if (nStart != nEnd) pEditCtrl->SetSel(0, 0);
	}
	return TRUE;
}

BOOL CCMLoadESEQTBGrid::OnValidateCell(ROWCOL nRow, ROWCOL nCol)
{
	if (CGXGridWnd::OnValidateCell(nRow, nCol))
		return OnValidateField(nRow, nCol);
	else return FALSE;
}

BOOL CCMLoadESEQTBGrid::CanCut()
{
	return FALSE; // Cut을 못하게 막는다.
}

// Paste를 지원하기 위해서는 이 함수를 각자 override해야 한다.
BOOL CCMLoadESEQTBGrid::OnPasteFromClipboard(const CGXRange &range)
{
	// not used
	range;
	return FALSE; // paste 동작을 막는다.

	/*
	// 임시로 CGXGridWnd를 하나 만들어 Paste를 한 후 작업을 한다.
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
	
	x.DestroyWindow();
	return FALSE;   // 실제 Paste는 하지 않는다.
	*/
}

// Selection이 깜빡거리는 현상을 막기 위해 사용한다.
// Copy-Paste 동작에서 Selection이 있는 경우 RemoveRow가 호출되면
// LockUpdate에 상관없이 Selection은 다시 그려진다. 이걸 막는 기능을 한다.
void CCMLoadESEQTBGrid::PrepareClearSelection(BOOL bCreateHint)
{
	BOOL bOldLock = LockUpdate(TRUE);
	if (bOldLock) return;
	LockUpdate(bOldLock);
	CGXGridWnd::PrepareClearSelection(bCreateHint);
	return;
}


//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
// New Overridable Functions
//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/

BOOL CCMLoadESEQTBGrid::CanDeleteRecords(CRowColArray& awRows)
{
	// 레코드 셋을 지울 수 있으면 TRUE, 아니면 FALSE 리턴.
	// 에러 메세지를 표시하려면 SetWarningText를 이용한다.
	// not used
	awRows;
	return TRUE;
}

BOOL CCMLoadESEQTBGrid::OnValidateField(ROWCOL nRow, ROWCOL nCol)
{
	// 필드가 valid하면 TRUE, 아니면 FALSE 리턴 : overridable
	// not used
	nRow; nCol;
	m_bValidField = TRUE;
	return m_bValidField;
}

BOOL CCMLoadESEQTBGrid::OnValidateRecord(ROWCOL nRow)
{
	// 레코드가 valid하면 m_bValidRecord를 TRUE로 아니면 FALSE로 설정
	// not used
	nRow;
	m_bValidRecord = TRUE;
	return m_bValidRecord;
}

BOOL CCMLoadESEQTBGrid::OnAddNewRecord(ROWCOL nRow)
{
	// 레코드를 DB에 넣는다. 성공하면 TRUE, 실패하면 ERROR 리턴
	// not used
	nRow;
	return TRUE;
}

BOOL CCMLoadESEQTBGrid::OnModifyRecord(ROWCOL nRow)
{
	// 레코드를 DB에서 수정한다. 성공하면 TRUE, 실패하면 ERROR 리턴
	// not used
	nRow;
	return TRUE;
}

BOOL CCMLoadESEQTBGrid::OnDeleteRecord(ROWCOL nRow)
{
	// 레코드를 DB에서 실제로 지운다. 성공하면 TRUE, 실패하면 ERROR 리턴
	if (!DeleteRecord(nRow))
	{
		SetWarningText(_LS(IDS_WG_CMD__ADDD__Error___Can_t_delete_data_));
		return FALSE;
	}
	return TRUE;
}

BOOL CCMLoadESEQTBGrid::OnDeleteRecords(CRowColArray &awRows)
{
	// 레코드(들)을 DB에서 실제로 지운다. 성공하면 TRUE, 실패하면 ERROR 리턴
	if (!DeleteRecord(awRows))
	{
		SetWarningText(_LS(IDS_WG_CMD__ADDD__Error___Can_t_delete_data_));
		return FALSE;
	}
	return TRUE;
}

BOOL CCMLoadESEQTBGrid::OnCanceledAddNew()
{
	return TRUE;
}

BOOL CCMLoadESEQTBGrid::OnCanceledModify()
{
	return TRUE;
}

BOOL CCMLoadESEQTBGrid::OnInsertEmptyRecord(ROWCOL nRow)
{
	// not used
	return TRUE;
}

BOOL CCMLoadESEQTBGrid::InsertEmptyRecord(ROWCOL nRow)
{
	BOOL bOldLock;
	ROWCOL nCol;
	CGXControl* pControl;
	if (m_nEditMode != noMode)
	{
		SetWarningText(_LS(IDS_WG_CMD__ADDD__Error___Can_t_add_new_data_while_));
		DisplayWarningText();
		return FALSE;
	}

	bOldLock = LockUpdate(TRUE);
	InsertRows(nRow, 1);
	SetRightValidCell(nRow, 0);
	m_nEditMode = addnew;


	VERIFY(GetCurrentCell(nRow, nCol));
	// 현재 셀이 안 보이면 스크롤 한다.
	ROWCOL nfRow, nfCol;
	GetTopLeftRowCol(&nfRow, &nfCol);
	ROWCOL nlRow = GetLastVisibleRow();
	ROWCOL nlCol = GetLastVisibleCol();
	if (nRow < nfRow) SetTopRow(nRow);
	else if (nRow > nlRow) SetTopRow(nfRow+(nRow-nlRow)+1);
	if (nCol < nfCol) SetLeftCol(nCol);
	else if (nCol > nlCol) SetLeftCol(nfCol+(nCol-nlCol)+1);

	// 컨트롤을 Activate 시킨다.
	pControl = GetControl(nRow, nCol);
	ASSERT(pControl);
	pControl->Init(nRow, nCol);
	pControl->SetActive(TRUE);

	// 삽입 직후 해야 할 일
	OnInsertEmptyRecord(nRow);
	LockUpdate(bOldLock);
	if (!bOldLock) Redraw();

	return TRUE;
}

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
// General Public Functions
//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/

BOOL CCMLoadESEQTBGrid::SetRecordCount(ROWCOL nRows, UINT flags)
{
	BOOL bOldLock = LockUpdate(TRUE);
	CGXGridWnd::SetRowCount(nRows+1, flags);
	GetBrowseParam()->m_nRecordCount = nRows;
	GetBrowseParam()->m_nCurrentRow = 1;
	m_nEditMode = noMode;
	SetRightValidCell(1, 0);
	LockUpdate(bOldLock);
	if (!bOldLock) Redraw();
	return TRUE;
}

// 주어진 Row앞에 레코드를 삽입합니다.
// Insert키가 눌러진 것과는 조금 다르다.
// Insert키가 눌러지면 Addnew 상태가 되는 것이고 이 함수는 실제
// 함수가 호출되는 것이다.
BOOL CCMLoadESEQTBGrid::InsertRecord(ROWCOL nRow)
{
	// InsertRows를 수행할 때 Current Cell이 포커스를 잃는 것을 막기 위해
	// SetActive(FALSE)를 한 후 InsertRows()를 하고 SetActive(TRUE)를 한다.
	// 이것은 InsertRows의 내부에서 TransferCurrentCell을 호출하게 되는데
	// TransferCurrentCell은 현재 컨트롤이 Active상태이면 컨트롤의 내용을
	// 저장하고 컨트롤을 Deactive시키기 때문이다.

	if (nRow > GetRowCount()) return FALSE;
	ROWCOL ncRow, ncCol;
	GetCurrentCell(ncRow, ncCol);
	CGXControl* pControl = GetControl(ncRow, ncCol);

	BOOL bOldLock = LockUpdate(TRUE); // 수정이 종료된 후 다시 그린다.
	BOOL bActive = FALSE, bEdit = FALSE;
	CString val; int nStart, nEnd;  // 현재 셀의 텍스트와 caret 위치
	if (pControl && pControl->IsActive()) bActive = TRUE;
	if (bActive) 
	{
		bEdit = pControl->CGXControl::IsKindOf(CONTROL_CLASS(CGXEditControl));
		if (bEdit) ((CGXEditControl*)pControl)->GetSel(nStart, nEnd);
		pControl->GetCurrentText(val);
		pControl->SetActive(FALSE);
	}

	InsertRows(nRow, 1);
	//VERIFY(GetCurrentCell(ncRow, ncCol));
	//GetCurrentCell(ncRow, ncCol);
	//GetBrowseParam()->m_nCurrentRow = ncRow;
	GetBrowseParam()->m_nRecordCount++;
	// 현재 Row는 설정하지 않는다. 이것은 InsertRows가 되면
	// OnMovedCurrentCell이 호출되어 자동으로 Current Cell이 Update되기 때문이다.

	if (bActive) 
	{
		pControl->SetActive(TRUE);
		pControl->SetCurrentText(val);
		if (bEdit) ((CGXEditControl*)pControl)->SetSel(nStart, nEnd);
	}
	//RedrawRowCol(nRow, 0);
	LockUpdate(bOldLock);
	if (!bOldLock) Redraw();

	return TRUE;  
}

// 주어진 Row에 해당하는 레코드를 삭제합니다.
BOOL CCMLoadESEQTBGrid::DeleteRecord(ROWCOL nRow)
{
	if (nRow >= GetRowCount()) return FALSE;
	if (IsAppendRow(nRow)) return FALSE;

	ROWCOL ncRow, ncCol;
	GetCurrentCell(ncRow, ncCol);
	CGXControl* pControl = GetControl(ncRow, ncCol);
	if (nRow == ncRow && m_nEditMode == addnew) return CancelEdit(); //return FALSE;

	BOOL bOldLock = LockUpdate(TRUE);
	BOOL bActive = FALSE, bEdit = FALSE;
	int nStart, nEnd;
	CString val;
	if (pControl && pControl->IsActive()) bActive = TRUE;
	if (bActive && nRow != ncRow) // 셀이 Active이고 현재 Row가 아닌 다른 Row 삭제시
	{
		bEdit = pControl->CGXControl::IsKindOf(CONTROL_CLASS(CGXEditControl));
		if (bEdit) ((CGXEditControl*)pControl)->GetSel(nStart, nEnd);
		pControl->GetCurrentText(val);
		pControl->SetActive(FALSE);
	}

	RemoveRows(nRow, nRow);
	GetBrowseParam()->m_nRecordCount--;

	if (nRow == ncRow) // 현재 행을 지우면 새로 현재행을 설정한다.
	{
		// 편집중인 셀이 지워지므로 noMode로 변환된다.
		if (m_nEditMode == edit) m_nEditMode = noMode; 
		BOOL bSet = SetCurrentCell(ncRow, ncCol);
		if (!bSet) bSet = SetRightValidCell(ncRow, ncCol);
		if (!bSet) bSet = SetLeftValidCell(ncRow, ncCol);
		ASSERT(bSet);
	}
	if (ncRow > GetRowCount()) // 현재 셀이 마지막 행(Append row)에 있으면 현재 셀 다시 설정
	{
		// 아래 한줄은 OG70의 버그 때문에 삽입. Append Row의 바로 위가 Disable되어 있을
		// 경우 한줄이 지워지면 Append Row의 Cell이 Disable된 것 처럼 Style이 설정되어
		// 있다. 따라서 SetCurrentCell 호출전에 Enable을 해제시키는 방법을 사용했다.
		SetStyleRange(CGXRange(ncRow-1, ncCol), CGXStyle().SetIncludeEnabled(FALSE));
		BOOL bSet = SetCurrentCell(ncRow-1, ncCol); // 이 경우는 위쪽 행이 새로 Append Row가 되어 있으므로
																							 // 항상 선택 가능하다.
		if (!bSet) 
		{
//      GSaveHistoryFormatNF("Warning:SetCurrentCell(%d,%d) failed", ncRow-1, ncCol);
			bSet = SetRightValidCell(ncRow-1, ncCol);
			if (!bSet) bSet = SetLeftValidCell(ncRow-1, ncCol);
			ASSERT(bSet);
		}
	}

	if (bActive && nRow != ncRow)
	{
		pControl->SetActive(TRUE);
		pControl->SetCurrentText(val);
		if (bEdit) ((CGXEditControl*)pControl)->SetSel(nStart, nEnd);
	}
	LockUpdate(bOldLock);
	if (!bOldLock) Redraw();
	return TRUE;
}

BOOL CCMLoadESEQTBGrid::DeleteRecord(CRowColArray& awRows)
{
	ROWCOL nRow, nCol;

	// Redraw는 실제 지워진 row가 있을 때만 한다.
	ROWCOL nOldCount = GetRowCount();

	//레코드를 삭제한다. 선택된 레코드들은 항상 정렬되어 있다(오름차순)
	BOOL bOldLock = LockUpdate(TRUE);
	
	BOOL nPseudoRow = 0;  // addnew or append row는 pseudo row이다.
	// 편집중인 레코드가 제거 대상에 들어 있는지 조사
	ROWCOL row;
	for (int i = awRows.GetSize()-1; i >= 0; i--)
	{
		row = awRows.GetAt(i);
		if (IsAppendRow(row)) nPseudoRow++; // don't delete this row!
		else if (row == GetBrowseParam()->m_nCurrentRow) // 현재 row를 지울 때 
		{
			if (m_nEditMode == addnew) // 추가상태이면
			{
				// 편집 상태를 취소한다.
				nPseudoRow++;
				CancelEdit();
			}
			else
			{
				// edit or noMode이면 레코드를 제거하고 현재 셀을 다시 설정
				if (!GetCurrentCell(nRow, nCol)) nCol = 1;
				RemoveRows(row, row);
				BOOL bSet = SetDownValidCell(row, nCol);
				if (!bSet) bSet = SetRightValidCell(row, nCol);
				if (!bSet) bSet = SetLeftValidCell(row, nCol);
				ASSERT(bSet);
				m_nEditMode = noMode;
			}
		}
		else // 현재 row가 아닌 다른 row가 지워질 때
		{
			// current cell이 마지막에 있으면 RemoveRows에 의해 Current Cell이
			// 없어지므로 새로 current cell을 설정
			GetCurrentCell(nRow, nCol);
			if (IsAppendRow(nRow))
			{
				RemoveRows(row, row);
				SetCurrentCell(nRow-1, nCol);
			}
			else RemoveRows(row, row);
		}
	}
	LockUpdate(bOldLock);

	// Current Row를 재 설정하지 않는다. RemoveRows내에서
	// OnMovedCurrentCell이 호출되며 거기서 현재 셀은 자동으로 계산된다.
	// Row의 갯수를 변경한다.
	GetBrowseParam()->m_nRecordCount -= (awRows.GetSize()-nPseudoRow);
	if (GetRowCount() != nOldCount) Redraw();
	
	return TRUE;
}

BOOL CCMLoadESEQTBGrid::DeleteAllRecords()
{
	BOOL bOldLock = LockUpdate(TRUE);
	RemoveRows(1, GetRowCount());
	SetRowCount(1);
	GetBrowseParam()->m_nRecordCount = 0;
	GetBrowseParam()->m_nCurrentRow = 1;
	m_nEditMode = noMode;
	SetRightValidCell(1, 0);
	LockUpdate(bOldLock);
	if (!bOldLock) Redraw();
	return TRUE;
}

int CCMLoadESEQTBGrid::CompareKey( const void* arg1, const void* arg2 )
{
		return *(UINT*)arg1 - *(UINT*)arg2;
}

BOOL CCMLoadESEQTBGrid::ResizeColWidthsToFitSmart(long nCount)
{
	long nRow, nColCount, nRowCount;
	CArray<int, int> caWidth;
	nColCount = GetColCount();
	nRowCount = GetRowCount();

	if (nRowCount < nCount) 
		return ResizeColWidthsToFit(CGXRange().SetCols(1, nColCount));

	// nCount 갯수만큼만 추출해서 최대 width값을 구한다.
	BOOL bOldLock = LockUpdate(TRUE);

	ResizeColWidthsToFit(CGXRange(0, 1, 0, nColCount));
	for (long i = 0; i < nColCount; i++)
		caWidth.Add(GetColWidth(i+1));
	for (int i = 0; i < nCount; i++)
	{
		nRow = nRowCount/nCount*i+1;
		ResizeColWidthsToFit(CGXRange(nRow, 1, nRow, nColCount));
		for (long j = 0; j < nColCount; j++)
			if (GetColWidth(j+1) > caWidth[j]) caWidth[j] = GetColWidth(j+1);
	}

	// 새로 size 설정
	for (int i = 0; i < nColCount; i++)
		SetColWidth(i+1, i+1, caWidth[i]);

	LockUpdate(bOldLock);
	if (!bOldLock) Redraw();
	return TRUE;
}

// 테이블 자체가 ReadOnly만 허용하는 경우(예:섹션의 All 테이블)
// 지속적으로 Edit Lock이 걸린다.
void CCMLoadESEQTBGrid::SetModeDisplayOnly(BOOL bReadOnly)
{
	if (m_bDisplayOnly && bReadOnly) return;
	if (!m_bDisplayOnly && !bReadOnly) return;

	if (!bReadOnly) m_bDisplayOnly = bReadOnly;
	LockEditMode(bReadOnly);
	if (bReadOnly) m_bDisplayOnly = bReadOnly;
}

// 테이블에 잠시 Edit Lock을 건다. 
// (예:후처리 모드로 가면 테이블이 수정 안됨)
void CCMLoadESEQTBGrid::LockEditMode(BOOL bLock)
{
	if (m_bDisplayOnly) return;
	if (m_bLockEdit && bLock) return;
	if (!m_bLockEdit && !bLock) return;

	if (m_bLockEdit && !bLock) 
	{
		m_bLockEdit = bLock;
		RestorePrevColStyle();
		// Paste를 허용한다.
		EnableOleDataSource();  // enable copy & paste
		// 이 부분은 EnableOleDataSource 소스 참고
		m_bHitTestSelEdge = FALSE;  
		m_nClipboardFlags = 56 & (~GX_DNDSTYLES);
		return;
	}

	m_bLockEdit = bLock;
	SaveCurrentColStyle();
	SetStyleRange(CGXRange().SetCols(1, GetColCount()), CGXStyle()
		//.SetControl(GX_IDS_CTRL_STATIC)
		.SetReadOnly(TRUE)
		);

	// Paste를 금지시킨다
	// 그래서 이 부분 말고 OnPaste에서 막도록 코드를 수정한다.
	EnableOleDataSource(GX_DNDDISABLED);  // disable copy & paste
	// clipboard로 복사는 할 수 있게 한다.
	// 따라서 readonly라서 엑셀의 내용을 복사할 순 없지만 grid의 내용을
	// 엑셀로 복사할 수는 있다.
	m_nClipboardFlags = 56 & (~GX_DNDSTYLES); // 이부분에서 EnableOleDataSource한게 무용지물이 된다.
	m_bHitTestSelEdge = FALSE;  // edge에 가면 마우스에 사각형이 나타나는 것을 막는다.
}

void CCMLoadESEQTBGrid::SaveCurrentColStyle()
{
	ASSERT(m_aColStyle.GetSize() == 0);
	ROWCOL nColCount = GetColCount();
	for (int i = 0; i < nColCount; i++)
	{
		CGXStyle style;
		GetColStyle(i+1, style);
		m_aColStyle.Add(style);
	}
}

void CCMLoadESEQTBGrid::RestorePrevColStyle()
{
	ASSERT(m_aColStyle.GetSize() == GetColCount());
	int nSize = m_aColStyle.GetSize();
	for (int i = 0; i < nSize; i++)
		SetStyleRange(CGXRange().SetCols(i+1), m_aColStyle.GetAt(i), gxCopy);
	m_aColStyle.RemoveAll();  // 저장했던 것을 제거한다.
}


// nErrCode : 1(수식 에러), 2(숫자 아님), 3(범위 초과), 4(정수아님)
BOOL CCMLoadESEQTBGrid::IsValidLongNumber(CString value, int &nErrCode)
{
	value.TrimLeft();
	value.TrimRight();
	if (value == _T("")) return TRUE; // 공백은 0으로 생각하여 TRUE 리턴
	// 수식이면 변환, 수식이 아니면 TRUE 반환(수식은 첫자가 '=' 임)
	if (!ConvertFormulaToValue(value)) { nErrCode = 1; return FALSE; }

	int ecode;
	// 숫자인지 문자열인지 검사
	if (!IsNumeric(value, ecode)) 
	{
		if (ecode == 1) nErrCode = 3;
		else nErrCode = 2;
		return FALSE;
	}

	TCHAR *num = new TCHAR[value.GetLength()+1];
	TCHAR *endp;
	long n;

	_stprintf(num, value);
	n = _tcstol(num, &endp, 10);
	BOOL bAllScaned = endp == &num[value.GetLength()];
	delete[] num;

	// 정수인지 실수인지 조사
	if (!bAllScaned) { nErrCode = 4; return FALSE; }
	// 정수 범위 검사
	if (n == LONG_MAX || n == LONG_MIN) { nErrCode = 3; return FALSE; }

	return TRUE;
}

// nErrCode : 1(수식 에러), 2(숫자 아님), 3(범위 초과)
BOOL CCMLoadESEQTBGrid::IsValidDoubleNumber(CString value, int &nErrCode)
{
	value.TrimLeft();
	value.TrimRight();
	if (value == _T("")) return TRUE; // 공백은 0으로 생각하여 TRUE 리턴
	// 수식이면 변환, 수식이 아니면 TRUE 반환(수식은 첫자가 '=' 임)
	if (!ConvertFormulaToValue(value)) { nErrCode = 1; return FALSE; }

	int ecode;
	// 숫자인지 문자열인지 검사
	if (!IsNumeric(value, ecode))
	{
		if (ecode == 1) nErrCode = 3;
		else nErrCode = 2;
		return FALSE;
	}

	TCHAR *num = new TCHAR[value.GetLength()+1];
	TCHAR *endp;
	double n;

	_stprintf(num, value);
	n = _tcstod(num, &endp);
	delete[] num;

	// 실수 범위 검사
	if (n == HUGE_VAL || n == -HUGE_VAL) { nErrCode = 3; return FALSE; }

	return TRUE;
}

//
// 다이얼로그나 뷰 종료시 저장되지 않은 레코드에 대한 처리를 담당하는 함수
//
BOOL CCMLoadESEQTBGrid::OnTerminate(int nOption)
{
	if (!IsEdit()) return TRUE;
	if (nOption == D_TB_SAVE_NEVER) return TRUE;

	if (nOption == D_TB_SAVE_IF_YES)
	{
		CString strTemp = (char*)(LPCTSTR)_LS(IDS_WG_CMD__ADDD__Current_item_is_modified_but_not_);
		strTemp += (char*)(LPCTSTR)_LS(IDS_WG_CMD__ADDD__Would_you_like_to_save_it_);
		int nAnswer = AfxMessageBox(strTemp,MB_YESNO);
		if (nAnswer == IDNO) 
		{
			CancelEdit();
			return TRUE;
		}
	}
/*  if (CDBDoc::GetDocPoint() == 0) 
	{
		AfxMessageBox(_T("Can't store modified data.\nModification will be discarded."));
		return FALSE;
	}  */
	BOOL bSaved = SaveCurrentRecord();
	if (!bSaved) DisplayWarningText();
	return bSaved;
}

BOOL CCMLoadESEQTBGrid::SaveCurrentRecord()
{
	ASSERT(IsEdit());
	ROWCOL nRow, nCol;
	if (!GetCurrentCell(nRow, nCol)) return FALSE;

	// 레코드 Validation을 수행한다.
	// 처음에는 레코드 Validation을 먼저 했는데 이 경우 Field Validation이 있기 전에
	// 레코드 Validation을 먼저 수행할 때가 있어 에러 소지가 있다.
	// (맨 끝 칼럼에서 Active Cell인 상태에서 Return키 칠 때)
	BOOL bValid = OnValidateRecord(nRow);
	if (!bValid) return FALSE;

	if (m_nEditMode == addnew)
	{
		// 새 레코드 추가 모드이면
		if (!OnAddNewRecord(nRow)) return FALSE;
		GetBrowseParam()->m_nRecordCount++;
		m_nEditMode = noMode;
	}
	else if (m_nEditMode == edit)
	{
		// 기존 레코드 수정 모드이면
		if (!OnModifyRecord(nRow)) return FALSE;
		m_nEditMode = noMode;
	}
	return TRUE;
}

/* OnKillFocus로는 해결할 수 없다.
void CCMLoadESEQTBGrid::OnKillFocus(CWnd* pNewWnd) 
{
	CGXGridWnd::OnKillFocus(pNewWnd);

	// TODO: Add your message handler code here

	static BOOL bCalled = FALSE;
	if (bCalled) return;  // OnKillFocus가 호출된 경우는 그냥 통과
	if (pNewWnd->GetParent() == this) return; // child가 activate되는 경우는 그냥 통과
	if (!IsEdit()) return;    // editing mode가 아니면 그냥 통과

	// Validate하다가 Message Box가 떠서 OnKillFocus가 호출되는 경우가 있다
	// 이런 경우는 그냥 리턴되야 한다.
	// 어떻게 메세지 박스인지 알 수 있는가? -> 없다.
	// 1. 다음 포커스가 this와 같으면 메세지 박스로 생각하면 되지 않는가?
	// -> 다음 포커스 윈도우를 알 수 있는 방법이 없다.
	{
		int nResponse = AfxMessageBox(
			_T("Current record is editing!\nDo you want to save this record?"), 
			MB_YESNO);
		switch(nResponse)
		{
		case IDYES:
			{
				ROWCOL ncRow, ncCol;
				GetCurrentCell(ncRow, ncCol);
				if (!SetRightValidCell(ncRow+1, 0)) SetFocus();
			}
			break;
		case IDNO:
			CancelEdit();
			break;
		}
	}
	bCalled = FALSE;
}
*/

ROWCOL CCMLoadESEQTBGrid::GetAppendRowNo()
{
	return GetRowCount();
}

// Enable/Disable Insert Row by Pressing "insert" key
BOOL CCMLoadESEQTBGrid::EnableInsertRowByPressingKey(BOOL bEnable)
{ 
	BOOL bOld = m_bCanInsertRowByPressingKey; 
	m_bCanInsertRowByPressingKey = bEnable; 
	return bOld; 
}

// Enable/Disable Delete Row by Pressing "delete" key
BOOL CCMLoadESEQTBGrid::EnableDeleteRowByPressingKey(BOOL bEnable)
{ 
	BOOL bOld = m_bCanDeleteRowByPressingKey; 
	m_bCanDeleteRowByPressingKey = bEnable; 
	return bOld; 
}

// Enable/Disalbe append row
BOOL CCMLoadESEQTBGrid::EnableAppendRow(BOOL bEnable)
{ 
	BOOL bOld = m_bEnabledAppend; 
	m_bEnabledAppend = bEnable; 
	ROWCOL nAppendRow= GetAppendRowNo();
	RedrawRowCol(CGXRange().SetRows(nAppendRow, nAppendRow));

	return bOld; 
}

