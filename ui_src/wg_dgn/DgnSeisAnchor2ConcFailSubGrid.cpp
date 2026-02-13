// DgnSeisAnchor2ConcFailSubGrid.cpp : implementation file


#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnSeisAnchor2ConcFailSubGrid.h"
#include "DgnSeisAnchor2ConcFailSub.h"

#include "..\wg_common\wg_common_TBCommon.h"
#include "..\wg_common\wg_common_TBGrid.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_AttrCtrl2.h"

CDgnSeisAnchor2ConcFailSubGrid::CDgnSeisAnchor2ConcFailSubGrid(CWnd* pParent/* = NULL*/)
{
	m_pParent = (CDgnSeisAnchor2ConcFailSub*)pParent;

	m_pDoc = CDBDoc::GetDocPoint();

}

CDgnSeisAnchor2ConcFailSubGrid::~CDgnSeisAnchor2ConcFailSubGrid()
{
}

/////////////////////////////////////////////////////////////////////////////
// User Defined Finction
void CDgnSeisAnchor2ConcFailSubGrid::SetData2Grid(T_ANEV_CONC_FAIL_D* pConcFail, T_ANEV_D& AnevD)
{
	BOOL bOldLock = LockUpdate(TRUE);
	m_ConcFail = *pConcFail;
	m_AnevD = AnevD;

	int nRow = pConcFail->aFailSelLayer.GetSize();
	int nCol = 0;

	T_ANEV_FAIL_SEL_LAYER_D Layer;
	if (nRow > 0)
	{
		for (int i = 0; i < pConcFail->aFailSelLayer.GetSize(); i++)
		{
			Layer.Initialize();
			Layer = pConcFail->aFailSelLayer[i];
			nCol = max(nCol, Layer.aFailSel.GetSize());
		}
	}

	if (nRow == 0 || nCol == 0) return;

	SetColCount(nCol);
	SetRowCount(nRow);

	SetColWidth(0, 0, globalUtils.ScaleByDPI(50));
	SetColWidth(1, nCol, globalUtils.ScaleByDPI(90));

	SetRowHeight(0, 0, globalUtils.ScaleByDPI(20));
	SetRowHeight(1, nRow, globalUtils.ScaleByDPI(20));

	SetStyleRange(CGXRange().SetCols(1, nCol), CGXStyle()
		.SetControl(GX_IDS_CTRL_CHECKBOX3D)
		.SetVerticalAlignment(DT_VCENTER)
		.SetHorizontalAlignment(DT_CENTER)
		.SetPlaces(0)
		.SetValue(_T("0")));

	CString strColName;
	for (int i = 0; i < nCol; i++)
	{
		strColName.Format(_T("%d"), i + 1);
		SetValueRange(CGXRange(0, i + 1), strColName), CGXStyle()
			.SetVerticalAlignment(DT_VCENTER)
			.SetHorizontalAlignment(DT_CENTER);
	}

	CString strRowName;
	for (int i = 0; i < nRow; i++)
	{
		strRowName.Format(_T("%s%d"), _LS(IDS_DGN_SEIS_ANCHOR_SUPPPOT_LAYER), i + 1);
		SetValueRange(CGXRange(i + 1, 0), strRowName), CGXStyle()
			.SetVerticalAlignment(DT_VCENTER)
			.SetHorizontalAlignment(DT_CENTER);
	}


	if(nRow != m_AnevD.aLayer.GetSize())
	{
		ASSERT(0);
		return;
	}

	T_ANEV_FAIL_SEL_LAYER_D LayerOrg;
	for (int i = 0; i < nRow; i++)
	{
		Layer.Initialize();
		Layer = pConcFail->aFailSelLayer[i];

		LayerOrg.Initialize();
		LayerOrg = m_ConcFail.aFailSelLayer[i];

		CStringArray aValue;
		aValue.SetSize(nCol);

		for (int j = 0; j < nCol; j++)
		{
			CString value;
			if( j < Layer.aFailSel.GetSize())
				value.Format(_T("%d"), Layer.aFailSel[j]);
			else
				value.Format(_T("%d"), 0);
			aValue[j] = value;
		}

		SetDataToRow(i + 1, aValue, LayerOrg);
	}

	EnableDisableGrid(!AnevD.Fail.bUser[0]);

	LockUpdate(bOldLock);
	if (!bOldLock) Redraw();
}

BOOL CDgnSeisAnchor2ConcFailSubGrid::GetGrid2Data(T_ANEV_CONC_FAIL_D* pConcFail)
{
	int nRow = GetRowCount();
	int nCol = GetColCount();
	if (nRow == 0 || nCol == 0) return TRUE;

	CStringArray aValue;
	aValue.SetSize(nCol);

	T_ANEV_FAIL_SEL_LAYER_D Layer;

	for (int i = 0; i < nRow; i++)
	{
		Layer.Initialize();
		Layer.aFailSel.SetSize(nCol);

		if (!GetRowToData(i + 1, aValue)) return FALSE;

		for (int j = 0; j < nCol; j++)
		{
			Layer.aFailSel[j] = _ttoi(aValue[j]);
		}

		pConcFail->aFailSelLayer.Add(Layer);
	}

	return TRUE;
}

void CDgnSeisAnchor2ConcFailSubGrid::EnableDisableGrid(BOOL bEnable)
{
	int nRow = GetRowCount();
	int nCol = GetColCount();
	if (nRow == 0 || nCol == 0) return;

	int nSax = m_AnevD.aLayer[nRow - 1].aSax.GetSize() + 1;

	BOOL bEnable2 = TRUE;
	for (int i = 0; i < nRow; i++)
	{
		CStringArray aValue;
		aValue.SetSize(nCol);

		for (int j = 0; j < nCol; j++)
		{
			int nRow = i + 1;
			int nCol = j + 1;

			if (nCol == 1)
			{
				bEnable2 = TRUE;
			}
			else if (nCol > nSax)
			{
				bEnable2 = FALSE;
			}
			else
			{
				if (m_AnevD.aLayer[nRow-1].aSax[nCol-2] > 0.0) bEnable2 = TRUE;
				else                                           bEnable2 = FALSE;
			}
			if(bEnable)
			{
				SetStyleRange(CGXRange(i + 1, j + 1), CGXStyle().SetEnabled(bEnable2));
			}
			else
			{
				SetStyleRange(CGXRange(i + 1, j + 1), CGXStyle().SetEnabled(bEnable));
			}
		}
	}
}

void CDgnSeisAnchor2ConcFailSubGrid::SetDataToRow(ROWCOL nRow, CStringArray& raValue, T_ANEV_FAIL_SEL_LAYER_D& LayerOrg)
{
	int nColCount = GetColCount();
	ASSERT(nColCount == raValue.GetSize());

	int nSax = m_AnevD.aLayer[nRow-1].aSax.GetSize() + 1;

	BOOL bEnable = TRUE;
	for (int i = 1; i <= nColCount; ++i)
	{
		if (i == 1) 
		{
			bEnable = TRUE;
		}
		else if(i > nSax)
		{
			bEnable = FALSE;
		}
		else
		{
			if(m_AnevD.aLayer[nRow-1].aSax[i-2] > 0.0) bEnable = TRUE;
			else                                       bEnable = FALSE;
		}
		SetValueRange(CGXRange(nRow, i), raValue[i - 1]);
		SetStyleRange(CGXRange(nRow, i), CGXStyle().SetEnabled(bEnable));
	}
}

BOOL CDgnSeisAnchor2ConcFailSubGrid::GetRowToData(ROWCOL nRow, CStringArray& raValue)
{
	int nColCount = GetColCount();
	ASSERT(nColCount == raValue.GetSize());

	for (int i = 1; i <= nColCount; ++i)
		raValue[i - 1] = GetEditingValue(nRow, i);

	return TRUE;
}

CString CDgnSeisAnchor2ConcFailSubGrid::GetEditingValue(ROWCOL nRow, ROWCOL nCol)
{
	CString strNum(_T(""));
	BOOL bGet = FALSE;
	ROWCOL ncRow, ncCol;
	if (GetCurrentCell(ncRow, ncCol))
	{
		if (ncRow == nRow && ncCol == nCol)
		{
			CGXControl* pControl = GetControl(nRow, nCol);
			if (pControl && pControl->IsActive())
				bGet = pControl->GetValue(strNum);
		}
	}
	if (!bGet) strNum = GetValueRowCol(nRow, nCol);

	return strNum;
}

#define CWnd CGXGridWnd
BEGIN_MESSAGE_MAP(CDgnSeisAnchor2ConcFailSubGrid, CWnd)
#undef CWnd
	//{{AFX_MSG_MAP(CDgnSeisAnchor2ConcFailSubGrid)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

///////////////////////////////////////////////////////////////////////////
// CDgnSeisAnchor2ConcFailSubGrid message handlers

// 그리드 초기화 
void CDgnSeisAnchor2ConcFailSubGrid::Initialize()
{
	CGXGridWnd::Initialize();

	// cell 바깥영역 배경색 지정 
	GetParam()->GetProperties()->SetColor(GX_COLOR_BACKGROUND, CTBGrid::m_Color.OuterBackground);

	// Header 배경색 지정 
	ChangeColHeaderStyle(CGXStyle().SetInterior(CTBGrid::m_Color.RowColHeaderBg));
	ChangeRowHeaderStyle(CGXStyle().SetInterior(CTBGrid::m_Color.RowColHeaderBg));

	// 기타 Cells 배경색 지정
	ChangeStandardStyle(CGXStyle().SetInterior(CTBGrid::m_Color.NormalModeBg));

	LockUpdate(TRUE);

	SetScrollBarMode(SB_VERT, gxnEnabled | gxnEnhanced);
	SetScrollBarMode(SB_HORZ, gxnAutomatic | gxnEnhanced);

	// undo를 금지시킨다.
	GetParam()->EnableUndo(FALSE);

	// cell의 width와 height 크기 변경을 금지한다.
	GetParam()->EnableTrackRowHeight(FALSE);
	GetParam()->EnableTrackColWidth(FALSE);

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

	CGXProperties* pProp = GetParam()->GetProperties();
	pProp->SetMarkColHeader(FALSE); // Turn off pressed button effect for column headers
	pProp->SetMarkRowHeader(FALSE);
	pProp->SetUserProperty(GX_IDS_OUTLINECURRENTCELL, // 현재 셀의 border 설정 변경
		(CGXStyle)pProp->sInvertNoBorder);

	// 표준 스타일 변경
	ChangeColHeaderStyle(CGXStyle().SetFont(CGXFont_GC().SetBold(FALSE)));

	LockUpdate(FALSE);
}

BOOL CDgnSeisAnchor2ConcFailSubGrid::GetStyleRowCol(ROWCOL nRow, ROWCOL nCol, CGXStyle& style, GXModifyType mt, int nType)
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
		ncRow = GetRowCount() + 1; // 현재 셀이 없는 경우 : ncRow를 범위 밖으로 설정

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
			.SetInterior(COLORREF(RGB(187, 227, 255)))
			.SetTextColor(COLORREF(RGB(0, 0, 0)));

		return TRUE;
	}

	// Disable되었을 때
	if (!bEnabled)
	{
		if (bHeaderCol || bHeaderRow) style           // Row or Column Header
			.SetControl(GX_IDS_CTRL_HEADER)
			.SetInterior(CTBGrid::m_Color.RowColHeaderBg)
			.SetTextColor(CTBGrid::m_Color.NormalModeFg);
		else style                      // Disabled Cell
			.SetInterior(CTBGrid::m_Color.DisabledBg)
			.SetTextColor(CTBGrid::m_Color.DisabledFg);

		return TRUE;
	}

	if (bCurrentCell) style
		.SetInterior(COLORREF(RGB(42, 64, 128)))
		.SetTextColor(COLORREF(RGB(255, 255, 255)));
	else if (bCurrentRow) style
		.SetInterior(COLORREF(RGB(187, 227, 255)))
		.SetTextColor(COLORREF(RGB(0, 0, 0)));
	else if (bHeaderCol || bHeaderRow) style    // Enabled Header Col(Only Frozen)
		.SetInterior(COLORREF(RGB(187, 227, 255)))
		.SetTextColor(COLORREF(RGB(0, 0, 0)));

	return TRUE;
}

void CDgnSeisAnchor2ConcFailSubGrid::OnModifyCell(ROWCOL nRow, ROWCOL nCol)
{
	CGXGridWnd::OnModifyCell(nRow, nCol);

	//	SetViewData(); // ReDraw 동작은 바깥에서..
}

BOOL CDgnSeisAnchor2ConcFailSubGrid::OnLButtonClickedRowCol(ROWCOL nRow, ROWCOL nCol, UINT nFlags, CPoint pt)
{
	BOOL bRet = CGXGridWnd::OnLButtonClickedRowCol(nRow, nCol, nFlags, pt);

	CString value;
	value = GetValueRowCol(nRow, nCol);
	int iChk = _ttoi(GetValueRowCol(nRow, nCol));
	// 	if (iChk == 0) {} // Off -> On
	// 	else              // On  -> Off 

	//	SetViewData(); // ReDraw 동작은 바깥에서..

	return bRet;
}

BOOL CDgnSeisAnchor2ConcFailSubGrid::OnPasteFromClipboard(const CGXRange &range)
{
	CGXGridWnd x;
	CGXRange r;
	CGXRange toRange;
	toRange = range;
	r.top = r.bottom = 1;
	r.left = r.right = 1;
	x.Create(WS_CHILD, CRect(0, 0, 1, 1), GetParent(), 0);
	x.Initialize();
	x.GetParam()->EnableUndo(FALSE);
	x.LockUpdate(TRUE);
	x.SetRowCount(0); x.SetColCount(1);
	x.OnPasteFromClipboard(r);

	r.top = 1; r.left = 1;
	r.bottom = x.GetRowCount(); r.right = x.GetColCount();

	if (!VerifyPasteRange(r, toRange))
		return FALSE;

	int fromRows, fromCols, toCols, toRows;
	fromRows = r.bottom - r.top + 1;
	fromCols = r.right - r.left + 1;
	toRows = GetSelectedRowsCount(toRange.top, toRange.bottom);
	toCols = GetSelectedColsCount(toRange.left, toRange.right);
	int fromi, fromj, toi, toj;
	fromi = 1;
	for (toi = toRange.top; toi <= toRange.bottom; toi++)
	{
		fromj = 1;
		for (toj = toRange.left; toj <= toRange.right; toj++)
		{
			BOOL bCondition = FALSE;
			//bCondition = (toi == GetRowCount()/* && toj == 2*/);
			if (!bCondition)
			{
				SetValueRange(CGXRange(toi, toj), x.GetValueRowCol(fromi, fromj));
				if (!OnValidateCell(toi, toj))
				{
					DisplayWarningText();
					ShowData();

					x.DestroyWindow();
					return FALSE;
				}
			}
			if (fromCols != 1) fromj++;
		}
		if (fromRows != 1)  fromi++;

	}

	ShowData();
	//SetViewData(); // ReDraw 동작은 바깥에서.. 

	x.DestroyWindow();
	return TRUE;
}

BOOL CDgnSeisAnchor2ConcFailSubGrid::VerifyPasteRange(CGXRange& from, CGXRange& to)
{

	int fromCols, fromRows, toCols, toRows;
	fromCols = from.right - from.left + 1;
	fromRows = from.bottom - from.top + 1;
	toRows = GetSelectedRowsCount(to.top, to.bottom);
	toCols = GetSelectedColsCount(to.left, to.right);

	if (toRows == 1 && toCols == 1)
	{
		int i;
		for (i = to.left; i <= 2; i++)
		{
			if (GetSelectedColsCount(to.left, i) == fromCols)
			{
				to.right = i;
				break;
			}
		}
		if (fromCols != 1 && to.right == to.left)
		{
			MessageBox(_LS(IDS_CMD_PASTE_EXCEED_COLUMN));
			return FALSE;
		}

		for (i = to.top; i <= GetRowCount(); i++)
		{
			if (GetSelectedRowsCount(to.top, i) == fromRows)
			{
				to.bottom = i;
				break;
			}
		}
		if (fromRows != 1 && to.bottom == to.top)
		{
			MessageBox(_LS(IDS_CMD_PASTE_EXCEED_ROW));
			return FALSE;
		}
		toRows = GetSelectedRowsCount(to.top, to.bottom);
		toCols = GetSelectedColsCount(to.left, to.right);
	}
	if (fromCols == 1)
	{
		if (fromRows == 1 || fromRows == toRows) return TRUE;
	}
	else if (fromRows == 1)
	{
		if (fromCols == 1 || fromCols == toCols)  return TRUE;
	}
	else if (fromRows == toRows && fromCols == toCols)
		return TRUE;
	MessageBox(_LS(IDS_CMD_PASTE_RANGE_MISMATCH));
	return FALSE;
}


int CDgnSeisAnchor2ConcFailSubGrid::GetSelectedRowsCount(int top, int bottom)
{
	int toRows;
	toRows = bottom - top + 1;
	return toRows;
}

int CDgnSeisAnchor2ConcFailSubGrid::GetSelectedColsCount(int left, int right)
{
	int toCols;
	toCols = right - left + 1;
	return toCols;
}

void CDgnSeisAnchor2ConcFailSubGrid::ShowData()
{
	ROWCOL nRow, nCol;
	GetCurrentCell(&nRow, &nCol);
	BOOL bOldLock = LockUpdate(TRUE);

	LockUpdate(bOldLock);
	if (!bOldLock) Redraw();
}


//////////////////////////////////////////////////////////////////////////

void CDgnSeisAnchor2ConcFailSubGrid::SetViewData()
{
	m_pParent->SetViewData();
}

BOOL CDgnSeisAnchor2ConcFailSubGrid::ProcessKeys(CWnd *pSender, UINT nMessage, UINT nChar, UINT nRepCnt, UINT flags)
{
	CRowColArray awRows;
	ROWCOL nRow = 1, nCol = 1;

	if (!GetCurrentCell(nRow, nCol)) // if no current cell, default action
		return CGXGridWnd::ProcessKeys(pSender, nMessage, nChar, nRepCnt, flags);

	if (nMessage == WM_KEYDOWN)
	{
		switch (nChar)
		{
		case VK_TAB:
		{
			BOOL bShift = GetKeyState(VK_SHIFT) & 0x8000;
			if (bShift)
			{
				if (SetLeftValidCell(nRow, nCol)) { goto GT_PROC_RETURN; }
			}

			if (!bShift)
			{
				if (SetRightValidCell(nRow, nCol)) { goto GT_PROC_RETURN; }
			}

		}
		break;
		case VK_DELETE:
		{
			goto GT_PROC_RETURN;
		}
		break;
		default:
			break;
		}
	}

	return CGXGridWnd::ProcessKeys(pSender, nMessage, nChar, nRepCnt, flags);

GT_PROC_RETURN:
	if (nMessage == WM_KEYDOWN && (nChar == VK_TAB || nChar == VK_RETURN || nChar == VK_RIGHT || nChar == VK_LEFT))
	{
		Redraw();
	}

	return TRUE;
}

BOOL CDgnSeisAnchor2ConcFailSubGrid::SetRightValidCell(ROWCOL nRow, ROWCOL nCol)
{

	long i = nRow, j = nCol + 1; // 현재 행 다음 열
	int nColCount = GetColCount();
	if (j > nColCount) { ++i; j = 1; }

	BOOL bCovered;/* = FALSE;*/
	while (i <= GetRowCount())
	{
		bCovered = FALSE;

		while (j <= nColCount)
		{
			// 열이 선택될 수 있는지 조사
			CGXStyle style; style.Free();
			ComposeStyleRowCol(i, j, &style);

			// Is it a covered cell?
			CGXRange cvr;
			if (!bCovered && GetCoveredCellsRowCol(i, j, cvr))
			{
				if ((cvr.top <= i && i <= cvr.bottom) && (cvr.left <= j && j <= cvr.right))
				{
					if (nRow == cvr.top) i = cvr.bottom;
					j = cvr.right + 1;
					bCovered = TRUE;
					continue;
				}
			}

			if (IsRowHidden(i)) break;
			if (IsColHidden(j)) { ++j; continue; }

			BOOL bEnable = (!style.GetIncludeEnabled() || style.GetEnabled());
			BOOL bReadOnly = style.GetReadOnly();

			if (bEnable && !bReadOnly)
			{
				if (SetCurrentCell(i, j)) return TRUE;
				return FALSE;
			}
			//bCovered = TRUE;
			bCovered = FALSE;

			++j;  // 다음 열을 조사
		}
		if (j > nColCount) { ++i; j = 1; }
	}
	return FALSE;
}


BOOL CDgnSeisAnchor2ConcFailSubGrid::SetLeftValidCell(ROWCOL nRow, ROWCOL nCol)
{
	long i = nRow, j = nCol - 1; // 현재 행 이전 열
	int nColCount = GetColCount();
	if (nCol == 0 || j < 1) { --i; j = nColCount; }

	BOOL bCovered = FALSE;
	while (i >= 1)
	{
		bCovered = FALSE;
		while (j >= 1)
		{
			// 열이 선택될 수 있는지 조사
			CGXStyle style; style.Free();
			ComposeStyleRowCol(i, j, &style);

			// Is it a covered cell?
			CGXRange cvr;
			if (!bCovered && GetCoveredCellsRowCol(i, j, cvr))
			{
				if ((cvr.top <= i && i <= cvr.bottom) && (cvr.left <= j && j <= cvr.right))
				{
					i = cvr.bottom;
					j = cvr.left - 1;
					bCovered = TRUE;
					continue;
				}
			}

			if (IsRowHidden(i)) break;
			if (IsColHidden(j)) { --j; continue; }

			BOOL bEnable = !style.GetIncludeEnabled() || style.GetEnabled();
			BOOL bReadOnly = style.GetReadOnly();

			if (bEnable && !bReadOnly)
			{
				if (GetCoveredCellsRowCol(i, j, cvr))
				{
					if ((cvr.top <= i && i <= cvr.bottom) && (cvr.left <= j && j <= cvr.right))
					{
						j = cvr.left;
					}
				}
				if (SetCurrentCell(i, j)) return TRUE;
				return FALSE;
			}
			bCovered = TRUE;
			--j;  // 이전 열을 조사
		}
		if (nCol == 0 || j < 1) { --i; j = nColCount; }
	}
	return FALSE;
}