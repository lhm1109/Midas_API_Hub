

#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnSeisSetFactorH29AllGrid.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CDgnSeisSetFactorH29_RcPierGrid

IMPLEMENT_DYNAMIC(CDgnSeisSetFactorH29_RcPierGrid, CTBBrowserWnd)

CDgnSeisSetFactorH29_RcPierGrid::CDgnSeisSetFactorH29_RcPierGrid()
{
}

CDgnSeisSetFactorH29_RcPierGrid::~CDgnSeisSetFactorH29_RcPierGrid()
{
}

void CDgnSeisSetFactorH29_RcPierGrid::Initialize(const T_SIFA_H29_D& rData)
{
	CTBBrowserWnd::Initialize();

	// cell 바깥영역 배경색 지정 
	GetParam()->GetProperties()->SetColor(GX_COLOR_BACKGROUND, CTBGrid::m_Color.OuterBackground);

	// Header 배경색 지정 
	ChangeColHeaderStyle(CGXStyle().SetInterior(CTBGrid::m_Color.RowColHeaderBg));
	ChangeRowHeaderStyle(CGXStyle().SetInterior(CTBGrid::m_Color.RowColHeaderBg));

	// 기타 Cells 배경색 지정
	ChangeStandardStyle(CGXStyle().SetInterior(CTBGrid::m_Color.NormalModeBg));

	LockUpdate(TRUE);

	SetColCount(D_COMMON_COL_3);

	// Header
	SetFrozenCols(0, 0);

	// 표준 스타일 변경
	ChangeStandardStyle(CGXStyle()
		.SetFont(CGXFont_GC().SetSize(9))
		.SetVerticalAlignment(DT_VCENTER)
		.SetAllowEnter(FALSE));

	// 컬럼 헤더 스타일 변경
	ChangeColHeaderStyle(CGXStyle()
		.SetFont(CGXFont_GC().SetBold(FALSE)));

	// 컬럼 스타일 변경
	SetStyleRange(CGXRange().SetCols(D_COMMON_COL_INDENT), CGXStyle()
		.SetControl(GX_VT_STRING)
		.SetHorizontalAlignment(DT_CENTER)
		.SetEnabled(FALSE));

	SetStyleRange(CGXRange().SetCols(D_COMMON_COL_1, D_COMMON_COL_3), CGXStyle()
		.SetValueType(GX_VT_NUMERIC)
		.SetEnabled(TRUE)
		.SetHorizontalAlignment(DT_RIGHT));

	EnableDeleteRowByPressingKey(FALSE);
	EnableInsertRowByPressingKey(FALSE);
	EnableAppendRow(FALSE);

	SetRowHeightByDPI(0, 0, 20);

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
	SetScrollBarMode(SB_HORZ, gxnEnabled | gxnEnhanced);

	CGXProperties* pProp = GetParam()->GetProperties();
	pProp->SetMarkColHeader(FALSE); // Turn off pressed button effect for column headers
	pProp->SetMarkRowHeader(FALSE);
	pProp->SetUserProperty(GX_IDS_OUTLINECURRENTCELL, // 현재 셀의 border 설정 변경
		(CGXStyle)pProp->sInvertNoBorder);

	CArray<int, int> aWidth;
	aWidth.SetSize(D_COMMON_COL_3);
	aWidth[D_COMMON_COL_1-1] = 40;
	aWidth[D_COMMON_COL_2-1] = 40;
	aWidth[D_COMMON_COL_3-1] = 55;

	SetColWidthByDPI(0, 0, 100);
	for (int i = 0; i < D_COMMON_COL_3; i++)
	{
		SetColWidthByDPI(i+1, i+1, aWidth[i]);
	}
	SetValueRange(CGXRange(0, D_COMMON_COL_1), _LS(IDS_FACTOR_H29_COMMON_COL_1));
	SetValueRange(CGXRange(0, D_COMMON_COL_2), _LS(IDS_FACTOR_H29_COMMON_COL_2));
	SetValueRange(CGXRange(0, D_COMMON_COL_3), _LS(IDS_FACTOR_H29_COMMON_COL_3));

	// Row
	int nSize = D_COMMON_ROW_3 + 1;
	SetRowCount(nSize);
	HideRows(nSize, nSize);

	SetValueRange(CGXRange(D_COMMON_ROW_1, D_COMMON_COL_INDENT), _LS(IDS_FACTOR_H29_COMMON_ROW_1));
	SetValueRange(CGXRange(D_COMMON_ROW_2, D_COMMON_COL_INDENT), _LS(IDS_FACTOR_H29_COMMON_ROW_2));
	SetValueRange(CGXRange(D_COMMON_ROW_3, D_COMMON_COL_INDENT), _LS(IDS_FACTOR_H29_COMMON_ROW_3));

	SetValueRange(CGXRange(D_COMMON_ROW_1, D_COMMON_COL_1), rData.RcPierVal.Delta_yEd[T_SIFA_RC_Disp::kXi1]);
	SetValueRange(CGXRange(D_COMMON_ROW_2, D_COMMON_COL_1), rData.RcPierVal.Delta_ls2d[T_SIFA_RC_Disp::kXi1]);
	SetValueRange(CGXRange(D_COMMON_ROW_3, D_COMMON_COL_1), rData.RcPierVal.Delta_ls3d[T_SIFA_RC_Disp::kXi1]);

	SetValueRange(CGXRange(D_COMMON_ROW_1, D_COMMON_COL_2), rData.RcPierVal.Delta_yEd[T_SIFA_RC_Disp::kXi2]);
	SetValueRange(CGXRange(D_COMMON_ROW_2, D_COMMON_COL_2), rData.RcPierVal.Delta_ls2d[T_SIFA_RC_Disp::kXi2]);
	SetValueRange(CGXRange(D_COMMON_ROW_3, D_COMMON_COL_2), rData.RcPierVal.Delta_ls3d[T_SIFA_RC_Disp::kXi2]);

	SetValueRange(CGXRange(D_COMMON_ROW_1, D_COMMON_COL_3), rData.RcPierVal.Delta_yEd[T_SIFA_RC_Disp::kPhiS]);
	SetValueRange(CGXRange(D_COMMON_ROW_2, D_COMMON_COL_3), rData.RcPierVal.Delta_ls2d[T_SIFA_RC_Disp::kPhiS]);
	SetValueRange(CGXRange(D_COMMON_ROW_3, D_COMMON_COL_3), rData.RcPierVal.Delta_ls3d[T_SIFA_RC_Disp::kPhiS]);

	SetRowHeightByDPI(D_COMMON_ROW_1, D_COMMON_ROW_3, 20);
	LockUpdate(FALSE);
}

BOOL CDgnSeisSetFactorH29_RcPierGrid::GetStyleRowCol(ROWCOL nRow, ROWCOL nCol, CGXStyle& style, GXModifyType mt, int nType)
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
	else ncRow = GetRowCount() + 1; // 현재 셀이 없는 경우 : ncRow를 범위 밖으로 설정

	// 현재 셀이 이동되면 새로 그려준다.
	m_bRefreshOnSetCurrentCell = TRUE;

	// Row나 Col이 1보다 작으면 리턴한다.(Standard Row or Column Header)
	if (nRow < 1 || nCol < 1) return TRUE;

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
			.SetTextColor(m_Color.ReadOnlyModeFg);

		if (!bHeaderCol && bCurrentRow) SetCurrentRowStyle(style);

		return TRUE;
	}

	//// Disable안 되고 ReadOnly만 설정되었을 때
	//if (bReadOnly)
	//{
	//	if (bCurrentCell) style         // Current Cell
	//		.SetInterior(m_Color.ReadOnlyCurrentCellBg)
	//		.SetTextColor(m_Color.ReadOnlyCurrentCellFg);
	//	else if (bCurrentRow) style     // Current Row
	//		.SetInterior(m_Color.ReadOnlyCurrentRowBg)
	//		.SetTextColor(m_Color.ReadOnlyCurrentRowFg);
	//	else style                      // Readonly Cell
	//		.SetInterior(m_Color.ReadOnlyBg)
	//		.SetTextColor(m_Color.ReadOnlyFg);

	//	if (bCurrentRow) SetCurrentRowStyle(style);

	//	return TRUE;
	//}

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

		//if (m_setRedFont.find(std::pair<ROWCOL, ROWCOL>(nRow, nCol)) != m_setRedFont.end())
		//{
		//	style.SetTextColor(COLORREF(RGB(255, 0, 0)));
		//}

		style.SetControl(GX_IDS_CTRL_STATIC);  // 수정이 안되도록 control을 static으로 바꾼다.

		if (bCurrentRow) SetCurrentRowStyle(style);

		return TRUE;
	}

	// Disable 안되고, ReadOnly 아니고, Append가 Disable 되지 않은 경우
	// Current Cell or Current Row or Enabled Append Row인 경우이다.
	//if (bReadOnlyMode)
	//{
	//	if (bCurrentCell) style
	//		.SetInterior(m_Color.ReadOnlyCurrentCellBg)
	//		.SetTextColor(m_Color.ReadOnlyCurrentCellFg);
	//	else if (bCurrentRow) style
	//		.SetInterior(m_Color.ReadOnlyCurrentRowBg)
	//		.SetTextColor(m_Color.ReadOnlyCurrentRowFg);
	//	else if (bAppendRow) style
	//		.SetInterior(m_Color.ReadOnlyModeBg)
	//		.SetTextColor(m_Color.ReadOnlyModeFg);
	//	else if (bHeaderCol) style    // Enabled Header Col(Only Frozen)
	//		.SetInterior(m_Color.ReadOnlyModeBg)
	//		.SetTextColor(m_Color.ReadOnlyModeFg);

	//	if (bCurrentRow || bHeaderCol) SetCurrentRowStyle(style);

	//	return TRUE;
	//}
	//else
	//{
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
	//}

	return TRUE;
}

BOOL CDgnSeisSetFactorH29_RcPierGrid::OnEndEditing(ROWCOL nRow, ROWCOL nCol)
{
	CString data = GetValueRowCol(nRow, nCol);
	// double
	if (_ttof(data) == 0)
	{
		SetValueRange(CGXRange(nRow, nCol), _T("0"));
	}

	return CTBBrowserWnd::OnEndEditing(nRow, nCol);
}

void  CDgnSeisSetFactorH29_RcPierGrid::GetData(T_SIFA_H29_D& rData)
{
	double dDelta_yEd[T_SIFA_RC_Disp::kSize];
	for (int col = D_COMMON_COL_1; col <= D_COMMON_COL_3; col++)
	{
		dDelta_yEd[col - 1] = _ttof(GetValueRowCol(D_COMMON_ROW_1, col));
	}
	double dDelta_ls2d[T_SIFA_RC_Disp::kSize];
	for (int col = D_COMMON_COL_1; col <= D_COMMON_COL_3; col++)
	{
		dDelta_ls2d[col - 1] = _ttof(GetValueRowCol(D_COMMON_ROW_2, col));
	}
	double dDelta_ls3d[T_SIFA_RC_Disp::kSize];
	for (int col = D_COMMON_COL_1; col <= D_COMMON_COL_3; col++)
	{
		dDelta_ls3d[col - 1] = _ttof(GetValueRowCol(D_COMMON_ROW_3, col));
	}

	for (int i = 0; i < T_SIFA_RC_Disp::kSize; i++) rData.RcPierVal.Delta_yEd[i] = dDelta_yEd[i];
	for (int i = 0; i < T_SIFA_RC_Disp::kSize; i++) rData.RcPierVal.Delta_ls2d[i] = dDelta_ls2d[i];
	for (int i = 0; i < T_SIFA_RC_Disp::kSize; i++) rData.RcPierVal.Delta_ls3d[i] = dDelta_ls3d[i];
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CDgnSeisSetFactorH29_StlPierGrid

IMPLEMENT_DYNAMIC(CDgnSeisSetFactorH29_StlPierGrid, CTBBrowserWnd)

CDgnSeisSetFactorH29_StlPierGrid::CDgnSeisSetFactorH29_StlPierGrid()
{
}

CDgnSeisSetFactorH29_StlPierGrid::~CDgnSeisSetFactorH29_StlPierGrid()
{
}

void CDgnSeisSetFactorH29_StlPierGrid::Initialize(const T_SIFA_H29_D& rData)
{
	CTBBrowserWnd::Initialize();

	// cell 바깥영역 배경색 지정 
	GetParam()->GetProperties()->SetColor(GX_COLOR_BACKGROUND, CTBGrid::m_Color.OuterBackground);

	// Header 배경색 지정 
	ChangeColHeaderStyle(CGXStyle().SetInterior(CTBGrid::m_Color.RowColHeaderBg));
	ChangeRowHeaderStyle(CGXStyle().SetInterior(CTBGrid::m_Color.RowColHeaderBg));

	// 기타 Cells 배경색 지정
	ChangeStandardStyle(CGXStyle().SetInterior(CTBGrid::m_Color.NormalModeBg));

	LockUpdate(TRUE);

	SetColCount(D_COMMON_COL_3);

	// Header
	SetFrozenCols(0, 0);

	// 표준 스타일 변경
	ChangeStandardStyle(CGXStyle()
		.SetFont(CGXFont_GC().SetSize(9))
		.SetVerticalAlignment(DT_VCENTER)
		.SetAllowEnter(FALSE));

	// 컬럼 헤더 스타일 변경
	ChangeColHeaderStyle(CGXStyle()
		.SetFont(CGXFont_GC().SetBold(FALSE)));

	// 컬럼 스타일 변경
	SetStyleRange(CGXRange().SetCols(D_COMMON_COL_INDENT), CGXStyle()
		.SetControl(GX_VT_STRING)
		.SetHorizontalAlignment(DT_CENTER)
		.SetEnabled(FALSE));

	SetStyleRange(CGXRange().SetCols(D_COMMON_COL_1, D_COMMON_COL_3), CGXStyle()
		.SetValueType(GX_VT_NUMERIC)
		.SetEnabled(TRUE)
		.SetHorizontalAlignment(DT_RIGHT));

	EnableDeleteRowByPressingKey(FALSE);
	EnableInsertRowByPressingKey(FALSE);
	EnableAppendRow(FALSE);

	SetRowHeightByDPI(0, 0, 20);

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
	SetScrollBarMode(SB_HORZ, gxnEnabled | gxnEnhanced);

	CGXProperties* pProp = GetParam()->GetProperties();
	pProp->SetMarkColHeader(FALSE); // Turn off pressed button effect for column headers
	pProp->SetMarkRowHeader(FALSE);
	pProp->SetUserProperty(GX_IDS_OUTLINECURRENTCELL, // 현재 셀의 border 설정 변경
		(CGXStyle)pProp->sInvertNoBorder);

	CArray<int, int> aWidth;
	aWidth.SetSize(D_COMMON_COL_3);
	aWidth[D_COMMON_COL_1 - 1] = 40;
	aWidth[D_COMMON_COL_2 - 1] = 40;
	aWidth[D_COMMON_COL_3 - 1] = 55;

	SetColWidthByDPI(0, 0, 100);
	for (int i = 0; i < D_COMMON_COL_3; i++)
	{
		SetColWidthByDPI(i + 1, i + 1, aWidth[i]);
	}
	SetValueRange(CGXRange(0, D_COMMON_COL_1), _LS(IDS_FACTOR_H29_COMMON_COL_1));
	SetValueRange(CGXRange(0, D_COMMON_COL_2), _LS(IDS_FACTOR_H29_COMMON_COL_2));
	SetValueRange(CGXRange(0, D_COMMON_COL_3), _LS(IDS_FACTOR_H29_COMMON_COL_4));

	// Row
	int nSize = D_COMMON_ROW_1 + 1;
	SetRowCount(nSize);
	HideRows(nSize, nSize);

	SetValueRange(CGXRange(D_COMMON_ROW_1, D_COMMON_COL_INDENT), _LS(IDS_FACTOR_H29_COMMON_ROW_1));

	SetValueRange(CGXRange(D_COMMON_ROW_1, D_COMMON_COL_1), rData.StPierVal.Delta_ls2d[T_SIFA_ST_Disp::kXi1]);
	SetValueRange(CGXRange(D_COMMON_ROW_1, D_COMMON_COL_2), rData.StPierVal.Delta_ls2d[T_SIFA_ST_Disp::kXi2]);
	SetValueRange(CGXRange(D_COMMON_ROW_1, D_COMMON_COL_3), rData.StPierVal.Delta_ls2d[T_SIFA_ST_Disp::kPhiS]);

	SetRowHeightByDPI(D_COMMON_ROW_1, D_COMMON_ROW_3, 20);
	LockUpdate(FALSE);
}

BOOL CDgnSeisSetFactorH29_StlPierGrid::GetStyleRowCol(ROWCOL nRow, ROWCOL nCol, CGXStyle& style, GXModifyType mt, int nType)
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
	else ncRow = GetRowCount() + 1; // 현재 셀이 없는 경우 : ncRow를 범위 밖으로 설정

	// 현재 셀이 이동되면 새로 그려준다.
	m_bRefreshOnSetCurrentCell = TRUE;

	// Row나 Col이 1보다 작으면 리턴한다.(Standard Row or Column Header)
	if (nRow < 1 || nCol < 1) return TRUE;

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
			.SetTextColor(m_Color.ReadOnlyModeFg);

		if (!bHeaderCol && bCurrentRow) SetCurrentRowStyle(style);

		return TRUE;
	}

	//// Disable안 되고 ReadOnly만 설정되었을 때
	//if (bReadOnly)
	//{
	//	if (bCurrentCell) style         // Current Cell
	//		.SetInterior(m_Color.ReadOnlyCurrentCellBg)
	//		.SetTextColor(m_Color.ReadOnlyCurrentCellFg);
	//	else if (bCurrentRow) style     // Current Row
	//		.SetInterior(m_Color.ReadOnlyCurrentRowBg)
	//		.SetTextColor(m_Color.ReadOnlyCurrentRowFg);
	//	else style                      // Readonly Cell
	//		.SetInterior(m_Color.ReadOnlyBg)
	//		.SetTextColor(m_Color.ReadOnlyFg);

	//	if (bCurrentRow) SetCurrentRowStyle(style);

	//	return TRUE;
	//}

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

		//if (m_setRedFont.find(std::pair<ROWCOL, ROWCOL>(nRow, nCol)) != m_setRedFont.end())
		//{
		//	style.SetTextColor(COLORREF(RGB(255, 0, 0)));
		//}

		style.SetControl(GX_IDS_CTRL_STATIC);  // 수정이 안되도록 control을 static으로 바꾼다.

		if (bCurrentRow) SetCurrentRowStyle(style);

		return TRUE;
	}

	// Disable 안되고, ReadOnly 아니고, Append가 Disable 되지 않은 경우
	// Current Cell or Current Row or Enabled Append Row인 경우이다.
	//if (bReadOnlyMode)
	//{
	//	if (bCurrentCell) style
	//		.SetInterior(m_Color.ReadOnlyCurrentCellBg)
	//		.SetTextColor(m_Color.ReadOnlyCurrentCellFg);
	//	else if (bCurrentRow) style
	//		.SetInterior(m_Color.ReadOnlyCurrentRowBg)
	//		.SetTextColor(m_Color.ReadOnlyCurrentRowFg);
	//	else if (bAppendRow) style
	//		.SetInterior(m_Color.ReadOnlyModeBg)
	//		.SetTextColor(m_Color.ReadOnlyModeFg);
	//	else if (bHeaderCol) style    // Enabled Header Col(Only Frozen)
	//		.SetInterior(m_Color.ReadOnlyModeBg)
	//		.SetTextColor(m_Color.ReadOnlyModeFg);

	//	if (bCurrentRow || bHeaderCol) SetCurrentRowStyle(style);

	//	return TRUE;
	//}
	//else
	//{
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
	//}

	return TRUE;
}

BOOL CDgnSeisSetFactorH29_StlPierGrid::OnEndEditing(ROWCOL nRow, ROWCOL nCol)
{
	CString data = GetValueRowCol(nRow, nCol);
	// double
	if (_ttof(data) == 0)
	{
		SetValueRange(CGXRange(nRow, nCol), _T("0"));
	}

	return CTBBrowserWnd::OnEndEditing(nRow, nCol);
}

void  CDgnSeisSetFactorH29_StlPierGrid::GetData(T_SIFA_H29_D& rData)
{
	double dDelta_ls2d[T_SIFA_ST_Disp::kSize];
	for (int col = D_COMMON_COL_1; col <= D_COMMON_COL_3; col++)
	{
		dDelta_ls2d[col - 1] = _ttof(GetValueRowCol(D_COMMON_ROW_1, col));
	}

	for (int i = 0; i < T_SIFA_ST_Disp::kSize; i++) rData.StPierVal.Delta_ls2d[i] = dDelta_ls2d[i];
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CDgnSeisSetFactorH29_CurvatureGrid

IMPLEMENT_DYNAMIC(CDgnSeisSetFactorH29_CurvatureGrid, CTBBrowserWnd)

CDgnSeisSetFactorH29_CurvatureGrid::CDgnSeisSetFactorH29_CurvatureGrid()
{
}

CDgnSeisSetFactorH29_CurvatureGrid::~CDgnSeisSetFactorH29_CurvatureGrid()
{
}

void CDgnSeisSetFactorH29_CurvatureGrid::Initialize(const T_SIFA_H29_D& rData)
{
	CTBBrowserWnd::Initialize();

	// cell 바깥영역 배경색 지정 
	GetParam()->GetProperties()->SetColor(GX_COLOR_BACKGROUND, CTBGrid::m_Color.OuterBackground);

	// Header 배경색 지정 
	ChangeColHeaderStyle(CGXStyle().SetInterior(CTBGrid::m_Color.RowColHeaderBg));
	ChangeRowHeaderStyle(CGXStyle().SetInterior(CTBGrid::m_Color.RowColHeaderBg));

	// 기타 Cells 배경색 지정
	ChangeStandardStyle(CGXStyle().SetInterior(CTBGrid::m_Color.NormalModeBg));

	LockUpdate(TRUE);

	SetColCount(D_COMMON_COL_4);

	// Header
	SetFrozenCols(0, 0);

	// 표준 스타일 변경
	ChangeStandardStyle(CGXStyle()
		.SetFont(CGXFont_GC().SetSize(9))
		.SetVerticalAlignment(DT_VCENTER)
		.SetAllowEnter(FALSE));

	// 컬럼 헤더 스타일 변경
	ChangeColHeaderStyle(CGXStyle()
		.SetFont(CGXFont_GC().SetBold(FALSE)));

	// 컬럼 스타일 변경
	SetStyleRange(CGXRange().SetCols(D_COMMON_COL_INDENT), CGXStyle()
		.SetControl(GX_VT_STRING)
		.SetHorizontalAlignment(DT_CENTER)
		.SetEnabled(FALSE));

	SetStyleRange(CGXRange().SetCols(D_COMMON_COL_1, D_COMMON_COL_4), CGXStyle()
		.SetValueType(GX_VT_NUMERIC)
		.SetEnabled(TRUE)
		.SetHorizontalAlignment(DT_RIGHT));

	EnableDeleteRowByPressingKey(FALSE);
	EnableInsertRowByPressingKey(FALSE);
	EnableAppendRow(FALSE);

	SetRowHeightByDPI(0, 0, 20);

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
	SetScrollBarMode(SB_HORZ, gxnEnabled | gxnEnhanced);

	CGXProperties* pProp = GetParam()->GetProperties();
	pProp->SetMarkColHeader(FALSE); // Turn off pressed button effect for column headers
	pProp->SetMarkRowHeader(FALSE);
	pProp->SetUserProperty(GX_IDS_OUTLINECURRENTCELL, // 현재 셀의 border 설정 변경
		(CGXStyle)pProp->sInvertNoBorder);

	CArray<int, int> aWidth;
	aWidth.SetSize(D_COMMON_COL_4);
	aWidth[D_COMMON_COL_1 - 1] = 40;
	aWidth[D_COMMON_COL_2 - 1] = 40;
	aWidth[D_COMMON_COL_3 - 1] = 45;
	aWidth[D_COMMON_COL_4 - 1] = 45;

	SetColWidthByDPI(0, 0, 100);
	for (int i = 0; i < D_COMMON_COL_4; i++)
	{
		SetColWidthByDPI(i + 1, i + 1, aWidth[i]);
	}
	SetValueRange(CGXRange(0, D_COMMON_COL_1), _LS(IDS_FACTOR_H29_COMMON_COL_1));
	SetValueRange(CGXRange(0, D_COMMON_COL_2), _LS(IDS_FACTOR_H29_COMMON_COL_2));
	SetValueRange(CGXRange(0, D_COMMON_COL_3), _LS(IDS_FACTOR_H29_COMMON_COL_4));
	SetValueRange(CGXRange(0, D_COMMON_COL_4), _LS(IDS_FACTOR_H29_COMMON_COL_5));

	// Row
	int nSize = D_COMMON_ROW_2 + 1;
	SetRowCount(nSize);
	HideRows(nSize, nSize);

	SetValueRange(CGXRange(D_COMMON_ROW_1, D_COMMON_COL_INDENT), _LS(IDS_FACTOR_H29_COMMON_ROW_2));
	SetValueRange(CGXRange(D_COMMON_ROW_2, D_COMMON_COL_INDENT), _LS(IDS_FACTOR_H29_COMMON_ROW_3));

	SetValueRange(CGXRange(D_COMMON_ROW_1, D_COMMON_COL_1), rData.RcCurvature.Phi_ls2d[T_SIFA_RC_Curv::kXi1]);
	SetValueRange(CGXRange(D_COMMON_ROW_2, D_COMMON_COL_1), rData.RcCurvature.Phi_ls3d[T_SIFA_RC_Curv::kXi1]);

	SetValueRange(CGXRange(D_COMMON_ROW_1, D_COMMON_COL_2), rData.RcCurvature.Phi_ls2d[T_SIFA_RC_Curv::kXi2]);
	SetValueRange(CGXRange(D_COMMON_ROW_2, D_COMMON_COL_2), rData.RcCurvature.Phi_ls3d[T_SIFA_RC_Curv::kXi2]);

	SetValueRange(CGXRange(D_COMMON_ROW_1, D_COMMON_COL_3), rData.RcCurvature.Phi_ls2d[T_SIFA_RC_Curv::kPhiS]);
	SetValueRange(CGXRange(D_COMMON_ROW_2, D_COMMON_COL_3), rData.RcCurvature.Phi_ls3d[T_SIFA_RC_Curv::kPhiS]);

	SetValueRange(CGXRange(D_COMMON_ROW_1, D_COMMON_COL_4), rData.RcCurvature.Phi_ls2d[T_SIFA_RC_Curv::kK]);
	SetValueRange(CGXRange(D_COMMON_ROW_2, D_COMMON_COL_4), rData.RcCurvature.Phi_ls3d[T_SIFA_RC_Curv::kK]);

	SetRowHeightByDPI(D_COMMON_ROW_1, D_COMMON_ROW_2, 20);
	LockUpdate(FALSE);
}

BOOL CDgnSeisSetFactorH29_CurvatureGrid::GetStyleRowCol(ROWCOL nRow, ROWCOL nCol, CGXStyle& style, GXModifyType mt, int nType)
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
	else ncRow = GetRowCount() + 1; // 현재 셀이 없는 경우 : ncRow를 범위 밖으로 설정

	// 현재 셀이 이동되면 새로 그려준다.
	m_bRefreshOnSetCurrentCell = TRUE;

	// Row나 Col이 1보다 작으면 리턴한다.(Standard Row or Column Header)
	if (nRow < 1 || nCol < 1) return TRUE;

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
			.SetTextColor(m_Color.ReadOnlyModeFg);

		if (!bHeaderCol && bCurrentRow) SetCurrentRowStyle(style);

		return TRUE;
	}

	//// Disable안 되고 ReadOnly만 설정되었을 때
	//if (bReadOnly)
	//{
	//	if (bCurrentCell) style         // Current Cell
	//		.SetInterior(m_Color.ReadOnlyCurrentCellBg)
	//		.SetTextColor(m_Color.ReadOnlyCurrentCellFg);
	//	else if (bCurrentRow) style     // Current Row
	//		.SetInterior(m_Color.ReadOnlyCurrentRowBg)
	//		.SetTextColor(m_Color.ReadOnlyCurrentRowFg);
	//	else style                      // Readonly Cell
	//		.SetInterior(m_Color.ReadOnlyBg)
	//		.SetTextColor(m_Color.ReadOnlyFg);

	//	if (bCurrentRow) SetCurrentRowStyle(style);

	//	return TRUE;
	//}

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

		//if (m_setRedFont.find(std::pair<ROWCOL, ROWCOL>(nRow, nCol)) != m_setRedFont.end())
		//{
		//	style.SetTextColor(COLORREF(RGB(255, 0, 0)));
		//}

		style.SetControl(GX_IDS_CTRL_STATIC);  // 수정이 안되도록 control을 static으로 바꾼다.

		if (bCurrentRow) SetCurrentRowStyle(style);

		return TRUE;
	}

	// Disable 안되고, ReadOnly 아니고, Append가 Disable 되지 않은 경우
	// Current Cell or Current Row or Enabled Append Row인 경우이다.
	//if (bReadOnlyMode)
	//{
	//	if (bCurrentCell) style
	//		.SetInterior(m_Color.ReadOnlyCurrentCellBg)
	//		.SetTextColor(m_Color.ReadOnlyCurrentCellFg);
	//	else if (bCurrentRow) style
	//		.SetInterior(m_Color.ReadOnlyCurrentRowBg)
	//		.SetTextColor(m_Color.ReadOnlyCurrentRowFg);
	//	else if (bAppendRow) style
	//		.SetInterior(m_Color.ReadOnlyModeBg)
	//		.SetTextColor(m_Color.ReadOnlyModeFg);
	//	else if (bHeaderCol) style    // Enabled Header Col(Only Frozen)
	//		.SetInterior(m_Color.ReadOnlyModeBg)
	//		.SetTextColor(m_Color.ReadOnlyModeFg);

	//	if (bCurrentRow || bHeaderCol) SetCurrentRowStyle(style);

	//	return TRUE;
	//}
	//else
	//{
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
	//}

	return TRUE;
}

BOOL CDgnSeisSetFactorH29_CurvatureGrid::OnEndEditing(ROWCOL nRow, ROWCOL nCol)
{
	CString data = GetValueRowCol(nRow, nCol);
	// double
	if (_ttof(data) == 0)
	{
		SetValueRange(CGXRange(nRow, nCol), _T("0"));
	}

	return CTBBrowserWnd::OnEndEditing(nRow, nCol);
}

void  CDgnSeisSetFactorH29_CurvatureGrid::GetData(T_SIFA_H29_D& rData)
{
	double dPhi_ls2d[T_SIFA_RC_Curv::kSize];
	for (int col = D_COMMON_COL_1; col <= D_COMMON_COL_4; col++)
	{
		dPhi_ls2d[col - 1] = _ttof(GetValueRowCol(D_COMMON_ROW_1, col));
	}
	double dPhi_ls3d[T_SIFA_RC_Curv::kSize];
	for (int col = D_COMMON_COL_1; col <= D_COMMON_COL_4; col++)
	{
		dPhi_ls3d[col - 1] = _ttof(GetValueRowCol(D_COMMON_ROW_2, col));
	}

	for (int i = 0; i < T_SIFA_RC_Curv::kSize; i++) rData.RcCurvature.Phi_ls2d[i] = dPhi_ls2d[i];
	for (int i = 0; i < T_SIFA_RC_Curv::kSize; i++) rData.RcCurvature.Phi_ls3d[i] = dPhi_ls3d[i];
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CDgnSeisSetFactorH29_MomentMydGrid

IMPLEMENT_DYNAMIC(CDgnSeisSetFactorH29_MomentMydGrid, CTBBrowserWnd)

CDgnSeisSetFactorH29_MomentMydGrid::CDgnSeisSetFactorH29_MomentMydGrid()
{
}

CDgnSeisSetFactorH29_MomentMydGrid::~CDgnSeisSetFactorH29_MomentMydGrid()
{
}

void CDgnSeisSetFactorH29_MomentMydGrid::Initialize(const T_SIFA_H29_D& rData)
{
	CTBBrowserWnd::Initialize();

	// cell 바깥영역 배경색 지정 
	GetParam()->GetProperties()->SetColor(GX_COLOR_BACKGROUND, CTBGrid::m_Color.OuterBackground);

	// Header 배경색 지정 
	ChangeColHeaderStyle(CGXStyle().SetInterior(CTBGrid::m_Color.RowColHeaderBg));
	ChangeRowHeaderStyle(CGXStyle().SetInterior(CTBGrid::m_Color.RowColHeaderBg));

	// 기타 Cells 배경색 지정
	ChangeStandardStyle(CGXStyle().SetInterior(CTBGrid::m_Color.NormalModeBg));

	LockUpdate(TRUE);

	SetColCount(D_COMMON_COL_2);

	// Header
	SetFrozenCols(0, 0);

	// 표준 스타일 변경
	ChangeStandardStyle(CGXStyle()
		.SetFont(CGXFont_GC().SetSize(9))
		.SetVerticalAlignment(DT_VCENTER)
		.SetAllowEnter(FALSE));

	// 컬럼 헤더 스타일 변경
	ChangeColHeaderStyle(CGXStyle()
		.SetFont(CGXFont_GC().SetBold(FALSE)));

	// 컬럼 스타일 변경
	SetStyleRange(CGXRange().SetCols(D_COMMON_COL_INDENT), CGXStyle()
		.SetControl(GX_VT_STRING)
		.SetHorizontalAlignment(DT_CENTER)
		.SetEnabled(FALSE));

	SetStyleRange(CGXRange().SetCols(D_COMMON_COL_1, D_COMMON_COL_2), CGXStyle()
		.SetValueType(GX_VT_NUMERIC)
		.SetEnabled(TRUE)
		.SetHorizontalAlignment(DT_RIGHT));

	EnableDeleteRowByPressingKey(FALSE);
	EnableInsertRowByPressingKey(FALSE);
	EnableAppendRow(FALSE);

	SetRowHeightByDPI(0, 0, 20);

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
	SetScrollBarMode(SB_HORZ, gxnEnabled | gxnEnhanced);

	CGXProperties* pProp = GetParam()->GetProperties();
	pProp->SetMarkColHeader(FALSE); // Turn off pressed button effect for column headers
	pProp->SetMarkRowHeader(FALSE);
	pProp->SetUserProperty(GX_IDS_OUTLINECURRENTCELL, // 현재 셀의 border 설정 변경
		(CGXStyle)pProp->sInvertNoBorder);

	CArray<int, int> aWidth;
	aWidth.SetSize(D_COMMON_COL_2);
	aWidth[D_COMMON_COL_1 - 1] = 45;
	aWidth[D_COMMON_COL_2 - 1] = 45;

	SetColWidthByDPI(0, 0, 100);
	for (int i = 0; i < D_COMMON_COL_2; i++)
	{
		SetColWidthByDPI(i + 1, i + 1, aWidth[i]);
	}
	SetValueRange(CGXRange(0, D_COMMON_COL_1), _LS(IDS_FACTOR_H29_COMMON_COL_1));
	SetValueRange(CGXRange(0, D_COMMON_COL_2), _LS(IDS_FACTOR_H29_COMMON_COL_6));

	// Row
	int nSize = D_COMMON_ROW_2 + 1;
	SetRowCount(nSize);
	HideRows(nSize, nSize);

	SetValueRange(CGXRange(D_COMMON_ROW_1, D_COMMON_COL_INDENT), _LS(IDS_FACTOR_H29_COMMON_ROW_4));
	SetValueRange(CGXRange(D_COMMON_ROW_2, D_COMMON_COL_INDENT), _LS(IDS_FACTOR_H29_COMMON_ROW_5));

	SetValueRange(CGXRange(D_COMMON_ROW_1, D_COMMON_COL_1), rData.RcMomentMyd.No10[T_SIFA_RC_Myd::kXi1]);
	SetValueRange(CGXRange(D_COMMON_ROW_2, D_COMMON_COL_1), rData.RcMomentMyd.No11[T_SIFA_RC_Myd::kXi1]);

	SetValueRange(CGXRange(D_COMMON_ROW_1, D_COMMON_COL_2), rData.RcMomentMyd.No10[T_SIFA_RC_Myd::kPhiY]);
	SetValueRange(CGXRange(D_COMMON_ROW_2, D_COMMON_COL_2), rData.RcMomentMyd.No11[T_SIFA_RC_Myd::kPhiY]);

	SetRowHeightByDPI(D_COMMON_ROW_1, D_COMMON_ROW_2, 20);
	LockUpdate(FALSE);
}

BOOL CDgnSeisSetFactorH29_MomentMydGrid::GetStyleRowCol(ROWCOL nRow, ROWCOL nCol, CGXStyle& style, GXModifyType mt, int nType)
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
	else ncRow = GetRowCount() + 1; // 현재 셀이 없는 경우 : ncRow를 범위 밖으로 설정

	// 현재 셀이 이동되면 새로 그려준다.
	m_bRefreshOnSetCurrentCell = TRUE;

	// Row나 Col이 1보다 작으면 리턴한다.(Standard Row or Column Header)
	if (nRow < 1 || nCol < 1) return TRUE;

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
			.SetTextColor(m_Color.ReadOnlyModeFg);

		if (!bHeaderCol && bCurrentRow) SetCurrentRowStyle(style);

		return TRUE;
	}

	//// Disable안 되고 ReadOnly만 설정되었을 때
	//if (bReadOnly)
	//{
	//	if (bCurrentCell) style         // Current Cell
	//		.SetInterior(m_Color.ReadOnlyCurrentCellBg)
	//		.SetTextColor(m_Color.ReadOnlyCurrentCellFg);
	//	else if (bCurrentRow) style     // Current Row
	//		.SetInterior(m_Color.ReadOnlyCurrentRowBg)
	//		.SetTextColor(m_Color.ReadOnlyCurrentRowFg);
	//	else style                      // Readonly Cell
	//		.SetInterior(m_Color.ReadOnlyBg)
	//		.SetTextColor(m_Color.ReadOnlyFg);

	//	if (bCurrentRow) SetCurrentRowStyle(style);

	//	return TRUE;
	//}

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

		//if (m_setRedFont.find(std::pair<ROWCOL, ROWCOL>(nRow, nCol)) != m_setRedFont.end())
		//{
		//	style.SetTextColor(COLORREF(RGB(255, 0, 0)));
		//}

		style.SetControl(GX_IDS_CTRL_STATIC);  // 수정이 안되도록 control을 static으로 바꾼다.

		if (bCurrentRow) SetCurrentRowStyle(style);

		return TRUE;
	}

	// Disable 안되고, ReadOnly 아니고, Append가 Disable 되지 않은 경우
	// Current Cell or Current Row or Enabled Append Row인 경우이다.
	//if (bReadOnlyMode)
	//{
	//	if (bCurrentCell) style
	//		.SetInterior(m_Color.ReadOnlyCurrentCellBg)
	//		.SetTextColor(m_Color.ReadOnlyCurrentCellFg);
	//	else if (bCurrentRow) style
	//		.SetInterior(m_Color.ReadOnlyCurrentRowBg)
	//		.SetTextColor(m_Color.ReadOnlyCurrentRowFg);
	//	else if (bAppendRow) style
	//		.SetInterior(m_Color.ReadOnlyModeBg)
	//		.SetTextColor(m_Color.ReadOnlyModeFg);
	//	else if (bHeaderCol) style    // Enabled Header Col(Only Frozen)
	//		.SetInterior(m_Color.ReadOnlyModeBg)
	//		.SetTextColor(m_Color.ReadOnlyModeFg);

	//	if (bCurrentRow || bHeaderCol) SetCurrentRowStyle(style);

	//	return TRUE;
	//}
	//else
	//{
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
	//}

	return TRUE;
}

BOOL CDgnSeisSetFactorH29_MomentMydGrid::OnEndEditing(ROWCOL nRow, ROWCOL nCol)
{
	CString data = GetValueRowCol(nRow, nCol);
	// double
	if (_ttof(data) == 0)
	{
		SetValueRange(CGXRange(nRow, nCol), _T("0"));
	}

	return CTBBrowserWnd::OnEndEditing(nRow, nCol);
}

void  CDgnSeisSetFactorH29_MomentMydGrid::GetData(T_SIFA_H29_D& rData)
{
	double dNo10[T_SIFA_RC_Myd::kSize];
	for (int col = D_COMMON_COL_1; col <= D_COMMON_COL_2; col++)
	{
		dNo10[col - 1] = _ttof(GetValueRowCol(D_COMMON_ROW_1, col));
	}
	double dNo11[T_SIFA_RC_Myd::kSize];
	for (int col = D_COMMON_COL_1; col <= D_COMMON_COL_2; col++)
	{
		dNo11[col - 1] = _ttof(GetValueRowCol(D_COMMON_ROW_2, col));
	}

	for (int i = 0; i < T_SIFA_RC_Myd::kSize; i++) rData.RcMomentMyd.No10[i] = dNo10[i];
	for (int i = 0; i < T_SIFA_RC_Myd::kSize; i++) rData.RcMomentMyd.No11[i] = dNo11[i];
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CDgnSeisSetFactorH29_MomentMudGrid

IMPLEMENT_DYNAMIC(CDgnSeisSetFactorH29_MomentMudGrid, CTBBrowserWnd)

CDgnSeisSetFactorH29_MomentMudGrid::CDgnSeisSetFactorH29_MomentMudGrid()
{
}

CDgnSeisSetFactorH29_MomentMudGrid::~CDgnSeisSetFactorH29_MomentMudGrid()
{
}

void CDgnSeisSetFactorH29_MomentMudGrid::Initialize(const T_SIFA_H29_D& rData)
{
	CTBBrowserWnd::Initialize();

	// cell 바깥영역 배경색 지정 
	GetParam()->GetProperties()->SetColor(GX_COLOR_BACKGROUND, CTBGrid::m_Color.OuterBackground);

	// Header 배경색 지정 
	ChangeColHeaderStyle(CGXStyle().SetInterior(CTBGrid::m_Color.RowColHeaderBg));
	ChangeRowHeaderStyle(CGXStyle().SetInterior(CTBGrid::m_Color.RowColHeaderBg));

	// 기타 Cells 배경색 지정
	ChangeStandardStyle(CGXStyle().SetInterior(CTBGrid::m_Color.NormalModeBg));

	LockUpdate(TRUE);

	SetColCount(D_COMMON_COL_3);

	// Header
	SetFrozenCols(0, 0);

	// 표준 스타일 변경
	ChangeStandardStyle(CGXStyle()
		.SetFont(CGXFont_GC().SetSize(9))
		.SetVerticalAlignment(DT_VCENTER)
		.SetAllowEnter(FALSE));

	// 컬럼 헤더 스타일 변경
	ChangeColHeaderStyle(CGXStyle()
		.SetFont(CGXFont_GC().SetBold(FALSE)));

	// 컬럼 스타일 변경
	SetStyleRange(CGXRange().SetCols(D_COMMON_COL_INDENT), CGXStyle()
		.SetControl(GX_VT_STRING)
		.SetHorizontalAlignment(DT_CENTER)
		.SetEnabled(FALSE));

	SetStyleRange(CGXRange().SetCols(D_COMMON_COL_1, D_COMMON_COL_3), CGXStyle()
		.SetValueType(GX_VT_NUMERIC)
		.SetEnabled(TRUE)
		.SetHorizontalAlignment(DT_RIGHT));

	EnableDeleteRowByPressingKey(FALSE);
	EnableInsertRowByPressingKey(FALSE);
	EnableAppendRow(FALSE);

	SetRowHeightByDPI(0, 0, 20);

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
	SetScrollBarMode(SB_HORZ, gxnEnabled | gxnEnhanced);

	CGXProperties* pProp = GetParam()->GetProperties();
	pProp->SetMarkColHeader(FALSE); // Turn off pressed button effect for column headers
	pProp->SetMarkRowHeader(FALSE);
	pProp->SetUserProperty(GX_IDS_OUTLINECURRENTCELL, // 현재 셀의 border 설정 변경
		(CGXStyle)pProp->sInvertNoBorder);

	CArray<int, int> aWidth;
	aWidth.SetSize(D_COMMON_COL_3);
	aWidth[D_COMMON_COL_1 - 1] = 40;
	aWidth[D_COMMON_COL_2 - 1] = 40;
	aWidth[D_COMMON_COL_3 - 1] = 55;

	SetColWidthByDPI(0, 0, 100);
	for (int i = 0; i < D_COMMON_COL_3; i++)
	{
		SetColWidthByDPI(i + 1, i + 1, aWidth[i]);
	}
	SetValueRange(CGXRange(0, D_COMMON_COL_1), _LS(IDS_FACTOR_H29_COMMON_COL_1));
	SetValueRange(CGXRange(0, D_COMMON_COL_2), _LS(IDS_FACTOR_H29_COMMON_COL_2));
	SetValueRange(CGXRange(0, D_COMMON_COL_3), _LS(IDS_FACTOR_H29_COMMON_COL_7));

	// Row
	int nSize = D_COMMON_ROW_2 + 1;
	SetRowCount(nSize);
	HideRows(nSize, nSize);

	SetValueRange(CGXRange(D_COMMON_ROW_1, D_COMMON_COL_INDENT), _LS(IDS_FACTOR_H29_COMMON_ROW_4));
	SetValueRange(CGXRange(D_COMMON_ROW_2, D_COMMON_COL_INDENT), _LS(IDS_FACTOR_H29_COMMON_ROW_5));

	SetValueRange(CGXRange(D_COMMON_ROW_1, D_COMMON_COL_1), rData.RcMomentMud.No10[T_SIFA_RC_Mud::kXi1]);
	SetValueRange(CGXRange(D_COMMON_ROW_2, D_COMMON_COL_1), rData.RcMomentMud.No11[T_SIFA_RC_Mud::kXi1]);

	SetValueRange(CGXRange(D_COMMON_ROW_1, D_COMMON_COL_2), rData.RcMomentMud.No10[T_SIFA_RC_Mud::kXi2]);
	SetValueRange(CGXRange(D_COMMON_ROW_2, D_COMMON_COL_2), rData.RcMomentMud.No11[T_SIFA_RC_Mud::kXi2]);

	SetValueRange(CGXRange(D_COMMON_ROW_1, D_COMMON_COL_3), rData.RcMomentMud.No10[T_SIFA_RC_Mud::kPhiU]);
	SetValueRange(CGXRange(D_COMMON_ROW_2, D_COMMON_COL_3), rData.RcMomentMud.No11[T_SIFA_RC_Mud::kPhiU]);

	SetRowHeightByDPI(D_COMMON_ROW_1, D_COMMON_ROW_2, 20);
	LockUpdate(FALSE);
}

BOOL CDgnSeisSetFactorH29_MomentMudGrid::GetStyleRowCol(ROWCOL nRow, ROWCOL nCol, CGXStyle& style, GXModifyType mt, int nType)
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
	else ncRow = GetRowCount() + 1; // 현재 셀이 없는 경우 : ncRow를 범위 밖으로 설정

	// 현재 셀이 이동되면 새로 그려준다.
	m_bRefreshOnSetCurrentCell = TRUE;

	// Row나 Col이 1보다 작으면 리턴한다.(Standard Row or Column Header)
	if (nRow < 1 || nCol < 1) return TRUE;

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
			.SetTextColor(m_Color.ReadOnlyModeFg);

		if (!bHeaderCol && bCurrentRow) SetCurrentRowStyle(style);

		return TRUE;
	}

	//// Disable안 되고 ReadOnly만 설정되었을 때
	//if (bReadOnly)
	//{
	//	if (bCurrentCell) style         // Current Cell
	//		.SetInterior(m_Color.ReadOnlyCurrentCellBg)
	//		.SetTextColor(m_Color.ReadOnlyCurrentCellFg);
	//	else if (bCurrentRow) style     // Current Row
	//		.SetInterior(m_Color.ReadOnlyCurrentRowBg)
	//		.SetTextColor(m_Color.ReadOnlyCurrentRowFg);
	//	else style                      // Readonly Cell
	//		.SetInterior(m_Color.ReadOnlyBg)
	//		.SetTextColor(m_Color.ReadOnlyFg);

	//	if (bCurrentRow) SetCurrentRowStyle(style);

	//	return TRUE;
	//}

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

		//if (m_setRedFont.find(std::pair<ROWCOL, ROWCOL>(nRow, nCol)) != m_setRedFont.end())
		//{
		//	style.SetTextColor(COLORREF(RGB(255, 0, 0)));
		//}

		style.SetControl(GX_IDS_CTRL_STATIC);  // 수정이 안되도록 control을 static으로 바꾼다.

		if (bCurrentRow) SetCurrentRowStyle(style);

		return TRUE;
	}

	// Disable 안되고, ReadOnly 아니고, Append가 Disable 되지 않은 경우
	// Current Cell or Current Row or Enabled Append Row인 경우이다.
	//if (bReadOnlyMode)
	//{
	//	if (bCurrentCell) style
	//		.SetInterior(m_Color.ReadOnlyCurrentCellBg)
	//		.SetTextColor(m_Color.ReadOnlyCurrentCellFg);
	//	else if (bCurrentRow) style
	//		.SetInterior(m_Color.ReadOnlyCurrentRowBg)
	//		.SetTextColor(m_Color.ReadOnlyCurrentRowFg);
	//	else if (bAppendRow) style
	//		.SetInterior(m_Color.ReadOnlyModeBg)
	//		.SetTextColor(m_Color.ReadOnlyModeFg);
	//	else if (bHeaderCol) style    // Enabled Header Col(Only Frozen)
	//		.SetInterior(m_Color.ReadOnlyModeBg)
	//		.SetTextColor(m_Color.ReadOnlyModeFg);

	//	if (bCurrentRow || bHeaderCol) SetCurrentRowStyle(style);

	//	return TRUE;
	//}
	//else
	//{
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
	//}

	return TRUE;
}

BOOL CDgnSeisSetFactorH29_MomentMudGrid::OnEndEditing(ROWCOL nRow, ROWCOL nCol)
{
	CString data = GetValueRowCol(nRow, nCol);
	// double
	if (_ttof(data) == 0)
	{
		SetValueRange(CGXRange(nRow, nCol), _T("0"));
	}

	return CTBBrowserWnd::OnEndEditing(nRow, nCol);
}

void  CDgnSeisSetFactorH29_MomentMudGrid::GetData(T_SIFA_H29_D& rData)
{
	double dNo10[T_SIFA_RC_Mud::kSize];
	for (int col = D_COMMON_COL_1; col <= D_COMMON_COL_3; col++)
	{
		dNo10[col - 1] = _ttof(GetValueRowCol(D_COMMON_ROW_1, col));
	}
	double dNo11[T_SIFA_RC_Mud::kSize];
	for (int col = D_COMMON_COL_1; col <= D_COMMON_COL_3; col++)
	{
		dNo11[col - 1] = _ttof(GetValueRowCol(D_COMMON_ROW_2, col));
	}

	for (int i = 0; i < T_SIFA_RC_Mud::kSize; i++) rData.RcMomentMud.No10[i] = dNo10[i];
	for (int i = 0; i < T_SIFA_RC_Mud::kSize; i++) rData.RcMomentMud.No11[i] = dNo11[i];
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CDgnSeisSetFactorH29_ShearSusdGrid

IMPLEMENT_DYNAMIC(CDgnSeisSetFactorH29_ShearSusdGrid, CTBBrowserWnd)

CDgnSeisSetFactorH29_ShearSusdGrid::CDgnSeisSetFactorH29_ShearSusdGrid()
{
}

CDgnSeisSetFactorH29_ShearSusdGrid::~CDgnSeisSetFactorH29_ShearSusdGrid()
{
}

void CDgnSeisSetFactorH29_ShearSusdGrid::Initialize(const T_SIFA_H29_D& rData)
{
	CTBBrowserWnd::Initialize();

	// cell 바깥영역 배경색 지정 
	GetParam()->GetProperties()->SetColor(GX_COLOR_BACKGROUND, CTBGrid::m_Color.OuterBackground);

	// Header 배경색 지정 
	ChangeColHeaderStyle(CGXStyle().SetInterior(CTBGrid::m_Color.RowColHeaderBg));
	ChangeRowHeaderStyle(CGXStyle().SetInterior(CTBGrid::m_Color.RowColHeaderBg));

	// 기타 Cells 배경색 지정
	ChangeStandardStyle(CGXStyle().SetInterior(CTBGrid::m_Color.NormalModeBg));

	LockUpdate(TRUE);

	SetColCount(D_COMMON_COL_6);

	// Header
	SetFrozenCols(0, 0);

	// 표준 스타일 변경
	ChangeStandardStyle(CGXStyle()
		.SetFont(CGXFont_GC().SetSize(9))
		.SetVerticalAlignment(DT_VCENTER)
		.SetAllowEnter(FALSE));

	// 컬럼 헤더 스타일 변경
	ChangeColHeaderStyle(CGXStyle()
		.SetFont(CGXFont_GC().SetBold(FALSE)));

	// 컬럼 스타일 변경
	SetStyleRange(CGXRange().SetCols(D_COMMON_COL_INDENT), CGXStyle()
		.SetControl(GX_VT_STRING)
		.SetHorizontalAlignment(DT_CENTER)
		.SetEnabled(FALSE));

	SetStyleRange(CGXRange().SetCols(D_COMMON_COL_1, D_COMMON_COL_6), CGXStyle()
		.SetValueType(GX_VT_NUMERIC)
		.SetEnabled(TRUE)
		.SetHorizontalAlignment(DT_RIGHT));

	EnableDeleteRowByPressingKey(FALSE);
	EnableInsertRowByPressingKey(FALSE);
	EnableAppendRow(FALSE);

	SetRowHeightByDPI(0, 0, 20);

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
	SetScrollBarMode(SB_HORZ, gxnEnabled | gxnEnhanced);

	CGXProperties* pProp = GetParam()->GetProperties();
	pProp->SetMarkColHeader(FALSE); // Turn off pressed button effect for column headers
	pProp->SetMarkRowHeader(FALSE);
	pProp->SetUserProperty(GX_IDS_OUTLINECURRENTCELL, // 현재 셀의 border 설정 변경
		(CGXStyle)pProp->sInvertNoBorder);

	CArray<int, int> aWidth;
	aWidth.SetSize(D_COMMON_COL_6);
	aWidth[D_COMMON_COL_1 - 1] = 40;
	aWidth[D_COMMON_COL_2 - 1] = 40;
	aWidth[D_COMMON_COL_3 - 1] = 65;
	aWidth[D_COMMON_COL_4 - 1] = 40;
	aWidth[D_COMMON_COL_5 - 1] = 55;
	aWidth[D_COMMON_COL_6 - 1] = 40;

	SetColWidthByDPI(0, 0, 100);
	for (int i = 0; i < D_COMMON_COL_6; i++)
	{
		SetColWidthByDPI(i + 1, i + 1, aWidth[i]);
	}
	SetValueRange(CGXRange(0, D_COMMON_COL_1), _LS(IDS_FACTOR_H29_COMMON_COL_1));
	SetValueRange(CGXRange(0, D_COMMON_COL_2), _LS(IDS_FACTOR_H29_COMMON_COL_2));
	SetValueRange(CGXRange(0, D_COMMON_COL_3), _LS(IDS_FACTOR_H29_COMMON_COL_8));
	SetValueRange(CGXRange(0, D_COMMON_COL_4), _LS(IDS_FACTOR_H29_COMMON_COL_1));
	SetValueRange(CGXRange(0, D_COMMON_COL_5), _LS(IDS_FACTOR_H29_COMMON_COL_9));
	SetValueRange(CGXRange(0, D_COMMON_COL_6), _LS(IDS_FACTOR_H29_COMMON_COL_5));

	// Row
	int nSize = D_COMMON_ROW_2 + 1;
	SetRowCount(nSize);
	HideRows(nSize, nSize);

	SetValueRange(CGXRange(D_COMMON_ROW_1, D_COMMON_COL_INDENT), _LS(IDS_FACTOR_H29_COMMON_ROW_4));
	SetValueRange(CGXRange(D_COMMON_ROW_2, D_COMMON_COL_INDENT), _LS(IDS_FACTOR_H29_COMMON_ROW_5));

	SetValueRange(CGXRange(D_COMMON_ROW_1, D_COMMON_COL_1), rData.RcShearSusd.No10[T_SIFA_RC_Susd::kXi1]);
	SetValueRange(CGXRange(D_COMMON_ROW_2, D_COMMON_COL_1), rData.RcShearSusd.No11[T_SIFA_RC_Susd::kXi1]);

	SetValueRange(CGXRange(D_COMMON_ROW_1, D_COMMON_COL_2), rData.RcShearSusd.No10[T_SIFA_RC_Susd::kXi2]);
	SetValueRange(CGXRange(D_COMMON_ROW_2, D_COMMON_COL_2), rData.RcShearSusd.No11[T_SIFA_RC_Susd::kXi2]);

	SetValueRange(CGXRange(D_COMMON_ROW_1, D_COMMON_COL_3), rData.RcShearSusd.No10[T_SIFA_RC_Susd::kPhi_uc_us]);
	SetValueRange(CGXRange(D_COMMON_ROW_2, D_COMMON_COL_3), rData.RcShearSusd.No11[T_SIFA_RC_Susd::kPhi_uc_us]);

	SetValueRange(CGXRange(D_COMMON_ROW_1, D_COMMON_COL_4), rData.RcShearSusd.No10[T_SIFA_RC_Susd::kXi1_p]);
	SetValueRange(CGXRange(D_COMMON_ROW_2, D_COMMON_COL_4), rData.RcShearSusd.No11[T_SIFA_RC_Susd::kXi1_p]);

	SetValueRange(CGXRange(D_COMMON_ROW_1, D_COMMON_COL_5), rData.RcShearSusd.No10[T_SIFA_RC_Susd::kXi2_Phi_up]);
	SetValueRange(CGXRange(D_COMMON_ROW_2, D_COMMON_COL_5), rData.RcShearSusd.No11[T_SIFA_RC_Susd::kXi2_Phi_up]);

	SetValueRange(CGXRange(D_COMMON_ROW_1, D_COMMON_COL_6), rData.RcShearSusd.No10[T_SIFA_RC_Susd::kK]);
	SetValueRange(CGXRange(D_COMMON_ROW_2, D_COMMON_COL_6), rData.RcShearSusd.No11[T_SIFA_RC_Susd::kK]);

	SetRowHeightByDPI(D_COMMON_ROW_1, D_COMMON_ROW_2, 20);
	LockUpdate(FALSE);
}

BOOL CDgnSeisSetFactorH29_ShearSusdGrid::GetStyleRowCol(ROWCOL nRow, ROWCOL nCol, CGXStyle& style, GXModifyType mt, int nType)
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
	else ncRow = GetRowCount() + 1; // 현재 셀이 없는 경우 : ncRow를 범위 밖으로 설정

	// 현재 셀이 이동되면 새로 그려준다.
	m_bRefreshOnSetCurrentCell = TRUE;

	// Row나 Col이 1보다 작으면 리턴한다.(Standard Row or Column Header)
	if (nRow < 1 || nCol < 1) return TRUE;

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
			.SetTextColor(m_Color.ReadOnlyModeFg);

		if (!bHeaderCol && bCurrentRow) SetCurrentRowStyle(style);

		return TRUE;
	}

	//// Disable안 되고 ReadOnly만 설정되었을 때
	//if (bReadOnly)
	//{
	//	if (bCurrentCell) style         // Current Cell
	//		.SetInterior(m_Color.ReadOnlyCurrentCellBg)
	//		.SetTextColor(m_Color.ReadOnlyCurrentCellFg);
	//	else if (bCurrentRow) style     // Current Row
	//		.SetInterior(m_Color.ReadOnlyCurrentRowBg)
	//		.SetTextColor(m_Color.ReadOnlyCurrentRowFg);
	//	else style                      // Readonly Cell
	//		.SetInterior(m_Color.ReadOnlyBg)
	//		.SetTextColor(m_Color.ReadOnlyFg);

	//	if (bCurrentRow) SetCurrentRowStyle(style);

	//	return TRUE;
	//}

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

		//if (m_setRedFont.find(std::pair<ROWCOL, ROWCOL>(nRow, nCol)) != m_setRedFont.end())
		//{
		//	style.SetTextColor(COLORREF(RGB(255, 0, 0)));
		//}

		style.SetControl(GX_IDS_CTRL_STATIC);  // 수정이 안되도록 control을 static으로 바꾼다.

		if (bCurrentRow) SetCurrentRowStyle(style);

		return TRUE;
	}

	// Disable 안되고, ReadOnly 아니고, Append가 Disable 되지 않은 경우
	// Current Cell or Current Row or Enabled Append Row인 경우이다.
	//if (bReadOnlyMode)
	//{
	//	if (bCurrentCell) style
	//		.SetInterior(m_Color.ReadOnlyCurrentCellBg)
	//		.SetTextColor(m_Color.ReadOnlyCurrentCellFg);
	//	else if (bCurrentRow) style
	//		.SetInterior(m_Color.ReadOnlyCurrentRowBg)
	//		.SetTextColor(m_Color.ReadOnlyCurrentRowFg);
	//	else if (bAppendRow) style
	//		.SetInterior(m_Color.ReadOnlyModeBg)
	//		.SetTextColor(m_Color.ReadOnlyModeFg);
	//	else if (bHeaderCol) style    // Enabled Header Col(Only Frozen)
	//		.SetInterior(m_Color.ReadOnlyModeBg)
	//		.SetTextColor(m_Color.ReadOnlyModeFg);

	//	if (bCurrentRow || bHeaderCol) SetCurrentRowStyle(style);

	//	return TRUE;
	//}
	//else
	//{
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
	//}

	return TRUE;
}

BOOL CDgnSeisSetFactorH29_ShearSusdGrid::OnEndEditing(ROWCOL nRow, ROWCOL nCol)
{
	CString data = GetValueRowCol(nRow, nCol);
	// double
	if (_ttof(data) == 0)
	{
		SetValueRange(CGXRange(nRow, nCol), _T("0"));
	}

	return CTBBrowserWnd::OnEndEditing(nRow, nCol);
}

void  CDgnSeisSetFactorH29_ShearSusdGrid::GetData(T_SIFA_H29_D& rData)
{
	double dNo10[T_SIFA_RC_Susd::kSize];
	for (int col = D_COMMON_COL_1; col <= D_COMMON_COL_6; col++)
	{
		dNo10[col - 1] = _ttof(GetValueRowCol(D_COMMON_ROW_1, col));
	}
	double dNo11[T_SIFA_RC_Susd::kSize];
	for (int col = D_COMMON_COL_1; col <= D_COMMON_COL_6; col++)
	{
		dNo11[col - 1] = _ttof(GetValueRowCol(D_COMMON_ROW_2, col));
	}

	for (int i = 0; i < T_SIFA_RC_Susd::kSize; i++) rData.RcShearSusd.No10[i] = dNo10[i];
	for (int i = 0; i < T_SIFA_RC_Susd::kSize; i++) rData.RcShearSusd.No11[i] = dNo11[i];
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CDgnSeisSetFactorH29_ShearSucdGrid

IMPLEMENT_DYNAMIC(CDgnSeisSetFactorH29_ShearSucdGrid, CTBBrowserWnd)

CDgnSeisSetFactorH29_ShearSucdGrid::CDgnSeisSetFactorH29_ShearSucdGrid()
{
}

CDgnSeisSetFactorH29_ShearSucdGrid::~CDgnSeisSetFactorH29_ShearSucdGrid()
{
}

void CDgnSeisSetFactorH29_ShearSucdGrid::Initialize(const T_SIFA_H29_D& rData)
{
	CTBBrowserWnd::Initialize();

	// cell 바깥영역 배경색 지정 
	GetParam()->GetProperties()->SetColor(GX_COLOR_BACKGROUND, CTBGrid::m_Color.OuterBackground);

	// Header 배경색 지정 
	ChangeColHeaderStyle(CGXStyle().SetInterior(CTBGrid::m_Color.RowColHeaderBg));
	ChangeRowHeaderStyle(CGXStyle().SetInterior(CTBGrid::m_Color.RowColHeaderBg));

	// 기타 Cells 배경색 지정
	ChangeStandardStyle(CGXStyle().SetInterior(CTBGrid::m_Color.NormalModeBg));

	LockUpdate(TRUE);

	SetColCount(D_COMMON_COL_2);

	// Header
	SetFrozenCols(0, 0);

	// 표준 스타일 변경
	ChangeStandardStyle(CGXStyle()
		.SetFont(CGXFont_GC().SetSize(9))
		.SetVerticalAlignment(DT_VCENTER)
		.SetAllowEnter(FALSE));

	// 컬럼 헤더 스타일 변경
	ChangeColHeaderStyle(CGXStyle()
		.SetFont(CGXFont_GC().SetBold(FALSE)));

	// 컬럼 스타일 변경
	SetStyleRange(CGXRange().SetCols(D_COMMON_COL_INDENT), CGXStyle()
		.SetControl(GX_VT_STRING)
		.SetHorizontalAlignment(DT_CENTER)
		.SetEnabled(FALSE));

	SetStyleRange(CGXRange().SetCols(D_COMMON_COL_1, D_COMMON_COL_2), CGXStyle()
		.SetValueType(GX_VT_NUMERIC)
		.SetEnabled(TRUE)
		.SetHorizontalAlignment(DT_RIGHT));

	EnableDeleteRowByPressingKey(FALSE);
	EnableInsertRowByPressingKey(FALSE);
	EnableAppendRow(FALSE);

	SetRowHeightByDPI(0, 0, 20);

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
	SetScrollBarMode(SB_HORZ, gxnEnabled | gxnEnhanced);

	CGXProperties* pProp = GetParam()->GetProperties();
	pProp->SetMarkColHeader(FALSE); // Turn off pressed button effect for column headers
	pProp->SetMarkRowHeader(FALSE);
	pProp->SetUserProperty(GX_IDS_OUTLINECURRENTCELL, // 현재 셀의 border 설정 변경
		(CGXStyle)pProp->sInvertNoBorder);

	CArray<int, int> aWidth;
	aWidth.SetSize(D_COMMON_COL_2);
	aWidth[D_COMMON_COL_1-1] = 40;
	aWidth[D_COMMON_COL_2-1] = 55;

	SetColWidthByDPI(0, 0, 100);
	for (int i = 0; i < D_COMMON_COL_2; i++)
	{
		SetColWidthByDPI(i+1, i+1, aWidth[i]);
	}
	SetValueRange(CGXRange(0, D_COMMON_COL_1), _LS(IDS_FACTOR_H29_COMMON_COL_1));
	SetValueRange(CGXRange(0, D_COMMON_COL_2), _LS(IDS_FACTOR_H29_COMMON_COL_10));

	// Row
	int nSize = D_COMMON_ROW_2 + 1;
	SetRowCount(nSize);
	HideRows(nSize, nSize);

	SetValueRange(CGXRange(D_COMMON_ROW_1, D_COMMON_COL_INDENT), _LS(IDS_FACTOR_H29_COMMON_ROW_4));
	SetValueRange(CGXRange(D_COMMON_ROW_2, D_COMMON_COL_INDENT), _LS(IDS_FACTOR_H29_COMMON_ROW_5));

	SetValueRange(CGXRange(D_COMMON_ROW_1, D_COMMON_COL_1), rData.RcShearSucd.No10[T_SIFA_RC_Sucd::kXi1]);
	SetValueRange(CGXRange(D_COMMON_ROW_2, D_COMMON_COL_1), rData.RcShearSucd.No10[T_SIFA_RC_Sucd::kXi2_Phi_ucw]);

	SetValueRange(CGXRange(D_COMMON_ROW_1, D_COMMON_COL_2), rData.RcShearSucd.No11[T_SIFA_RC_Sucd::kXi1]);
	SetValueRange(CGXRange(D_COMMON_ROW_2, D_COMMON_COL_2), rData.RcShearSucd.No11[T_SIFA_RC_Sucd::kXi2_Phi_ucw]);

	SetRowHeightByDPI(D_COMMON_ROW_1, D_COMMON_ROW_2, 20);
	LockUpdate(FALSE);
}

BOOL CDgnSeisSetFactorH29_ShearSucdGrid::GetStyleRowCol(ROWCOL nRow, ROWCOL nCol, CGXStyle& style, GXModifyType mt, int nType)
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
	else ncRow = GetRowCount() + 1; // 현재 셀이 없는 경우 : ncRow를 범위 밖으로 설정

	// 현재 셀이 이동되면 새로 그려준다.
	m_bRefreshOnSetCurrentCell = TRUE;

	// Row나 Col이 1보다 작으면 리턴한다.(Standard Row or Column Header)
	if (nRow < 1 || nCol < 1) return TRUE;

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
			.SetTextColor(m_Color.ReadOnlyModeFg);

		if (!bHeaderCol && bCurrentRow) SetCurrentRowStyle(style);

		return TRUE;
	}

	//// Disable안 되고 ReadOnly만 설정되었을 때
	//if (bReadOnly)
	//{
	//	if (bCurrentCell) style         // Current Cell
	//		.SetInterior(m_Color.ReadOnlyCurrentCellBg)
	//		.SetTextColor(m_Color.ReadOnlyCurrentCellFg);
	//	else if (bCurrentRow) style     // Current Row
	//		.SetInterior(m_Color.ReadOnlyCurrentRowBg)
	//		.SetTextColor(m_Color.ReadOnlyCurrentRowFg);
	//	else style                      // Readonly Cell
	//		.SetInterior(m_Color.ReadOnlyBg)
	//		.SetTextColor(m_Color.ReadOnlyFg);

	//	if (bCurrentRow) SetCurrentRowStyle(style);

	//	return TRUE;
	//}

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

		//if (m_setRedFont.find(std::pair<ROWCOL, ROWCOL>(nRow, nCol)) != m_setRedFont.end())
		//{
		//	style.SetTextColor(COLORREF(RGB(255, 0, 0)));
		//}

		style.SetControl(GX_IDS_CTRL_STATIC);  // 수정이 안되도록 control을 static으로 바꾼다.

		if (bCurrentRow) SetCurrentRowStyle(style);

		return TRUE;
	}

	// Disable 안되고, ReadOnly 아니고, Append가 Disable 되지 않은 경우
	// Current Cell or Current Row or Enabled Append Row인 경우이다.
	//if (bReadOnlyMode)
	//{
	//	if (bCurrentCell) style
	//		.SetInterior(m_Color.ReadOnlyCurrentCellBg)
	//		.SetTextColor(m_Color.ReadOnlyCurrentCellFg);
	//	else if (bCurrentRow) style
	//		.SetInterior(m_Color.ReadOnlyCurrentRowBg)
	//		.SetTextColor(m_Color.ReadOnlyCurrentRowFg);
	//	else if (bAppendRow) style
	//		.SetInterior(m_Color.ReadOnlyModeBg)
	//		.SetTextColor(m_Color.ReadOnlyModeFg);
	//	else if (bHeaderCol) style    // Enabled Header Col(Only Frozen)
	//		.SetInterior(m_Color.ReadOnlyModeBg)
	//		.SetTextColor(m_Color.ReadOnlyModeFg);

	//	if (bCurrentRow || bHeaderCol) SetCurrentRowStyle(style);

	//	return TRUE;
	//}
	//else
	//{
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
	//}

	return TRUE;
}

BOOL CDgnSeisSetFactorH29_ShearSucdGrid::OnEndEditing(ROWCOL nRow, ROWCOL nCol)
{
	CString data = GetValueRowCol(nRow, nCol);
	// double
	if (_ttof(data) == 0)
	{
		SetValueRange(CGXRange(nRow, nCol), _T("0"));
	}

	return CTBBrowserWnd::OnEndEditing(nRow, nCol);
}

void  CDgnSeisSetFactorH29_ShearSucdGrid::GetData(T_SIFA_H29_D& rData)
{
	double dNo10[T_SIFA_RC_Sucd::kSize];
	for (int col = D_COMMON_COL_1; col <= D_COMMON_COL_2; col++)
	{
		dNo10[col - 1] = _ttof(GetValueRowCol(D_COMMON_ROW_1, col));
	}
	double dNo11[T_SIFA_RC_Sucd::kSize];
	for (int col = D_COMMON_COL_1; col <= D_COMMON_COL_2; col++)
	{
		dNo11[col - 1] = _ttof(GetValueRowCol(D_COMMON_ROW_2, col));
	}

	for (int i = 0; i < T_SIFA_RC_Sucd::kSize; i++) rData.RcShearSucd.No10[i] = dNo10[i];
	for (int i = 0; i < T_SIFA_RC_Sucd::kSize; i++) rData.RcShearSucd.No11[i] = dNo11[i];
}
