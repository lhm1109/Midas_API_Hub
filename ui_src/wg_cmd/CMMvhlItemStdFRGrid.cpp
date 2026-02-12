#include "stdafx.h"
#include "wg_cmd.h"
#include "CMMvhlItemStdFRGrid.h"

#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_SectUtil.h"

#include "..\wg_db\wg_db_DBDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define D_SIZE_START_ROW 2
#define D_SIZE_END_ROW   4

#define D_COL_CLASS1 1
#define D_COL_CLASS2 2
#define D_COL_LANE1  3
#define D_COL_LANE2  4
#define D_COL_LANE3  5
#define D_COL_LANE4  6
#define D_COL_LANE5  7

/////////////////////////////////////////////////////////////////////////////
// CCMMvhlItemStdFRGrid

CCMMvhlItemStdFRGrid::CCMMvhlItemStdFRGrid()
{
	m_pMvhl = 0;
	m_bInitDone = FALSE;
	m_pParent = 0;
	m_bStd = TRUE;
	m_bBtType = FALSE;
}

CCMMvhlItemStdFRGrid::~CCMMvhlItemStdFRGrid()
{
}
void CCMMvhlItemStdFRGrid::InitGrid()
{
	if (!m_bInitDone) Initialize(); // 그리드를 초기화한다.
	else { ResetGrid(); Initialize(); }
	m_bInitDone = TRUE;

	GetParam()->EnableUndo(FALSE);  // undo를 금지시킨다.

	// cell의 width와 height 크기 변경을 금지한다.
	GetParam()->EnableTrackRowHeight(FALSE);
	GetParam()->EnableTrackColWidth(FALSE);

	// column과 row의 위치 이동을 금지시킨다.
	GetParam()->EnableMoveCols(FALSE);
	GetParam()->EnableMoveRows(FALSE);

	// double click시 셀 편집이 되도록 한다.
	GetParam()->SetActivateCellFlags(GX_CAFOCUS_DBLCLICKONCELL);

	// Text만 카피 되게 한다.
	//EnableOleDataSource(GX_DNDTEXT);
	m_nClipboardFlags &= ~GX_DNDSTYLES;

	// Row 헤더와 Column 헤더를 숨긴다.
	HideRows(0, 0);
	HideCols(0, 0);

	// always show vertical scrollbar
	SetScrollBarMode(SB_BOTH, gxnDisabled);

	// 표준 스타일 변경
	ChangeStandardStyle(CGXStyle()
		.SetFont(CGXFont_GC().SetSize(9))
		.SetVerticalAlignment(DT_VCENTER)
		.SetAllowEnter(FALSE));

	LockUpdate(FALSE);
	Redraw();
}

void CCMMvhlItemStdFRGrid::SetStyleOnSizeCell()
{
	ASSERT(0);
// 	int i;
// 	int count = 4;
// 
// 	for (i = 0; i <= count; i++)
// 	{
// 		SetStyleRange(CGXRange(i + 3, 2), CGXStyle()
// 			.SetFormat(GX_FMT_FIXED).SetPlaces(2));
// 		SetStyleRange(CGXRange(i + 3, 4), CGXStyle()
// 			.SetFormat(GX_FMT_FIXED).SetPlaces(2));
// 	}
}

void CCMMvhlItemStdFRGrid::SetDataSource(T_MVHL_D* pMvhl, CWnd* pParent, BOOL bStd)
{
	ASSERT(pMvhl);
	m_pMvhl   = pMvhl;
	m_pParent = pParent;
	m_bStd    = bStd;
	if (m_bStd && m_pMvhl->VehicleTypeName == _T("Load System Bt"))
	{
		m_bBtType = TRUE;
	}

	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	CString strTemp, strUnit;
	BOOL bOldLock = LockUpdate(TRUE);
	BOOL bOldReadOnlyLock = GetParam()->IsLockReadOnly();
	GetParam()->SetLockReadOnly(FALSE);
	int count = 3;
	SetRowCount(count + 1);
	SetColCount(D_COL_LANE5);

	// set column width
	if (m_bStd)
	{
		SetColWidth(D_COL_CLASS1, D_COL_CLASS1, globalUtils.ScaleByDPI(95));
		SetColWidth(D_COL_CLASS2, D_COL_CLASS2, globalUtils.ScaleByDPI(110));
		SetColWidth(D_COL_LANE1 , D_COL_LANE1, globalUtils.ScaleByDPI(35));
		SetColWidth(D_COL_LANE2 , D_COL_LANE2, globalUtils.ScaleByDPI(35));
		SetColWidth(D_COL_LANE3 , D_COL_LANE3, globalUtils.ScaleByDPI(35));
		SetColWidth(D_COL_LANE4 , D_COL_LANE4, globalUtils.ScaleByDPI(35));
		SetColWidth(D_COL_LANE5 , D_COL_LANE5, globalUtils.ScaleByDPI(35));
	}
	else
	{
		ASSERT(0);
	}

	int nHeight = GetRowHeight(1);

	SetRowHeight(1, 1, globalUtils.ScaleByDPI(25));
	SetRowHeight(2, 4, globalUtils.ScaleByDPI(20));

	SetCoveredCellsRowCol(1, D_COL_CLASS1, 1, D_COL_CLASS2);    // nRow, nCol, nToRow, nToCol
	SetStyleRange(CGXRange(1, 1), CGXStyle()
		.SetInterior(::GetSysColor(COLOR_BTNFACE))
		.SetEnabled(FALSE)
		.SetHorizontalAlignment(DT_CENTER)
		.SetVerticalAlignment(DT_VCENTER)
		.SetValue(_LS(IDS_CMD_MVHL_NUM_LANE))); // "Number of Loaded Lanes"

	SetStyleRange(CGXRange(1, D_COL_LANE1, 1, D_COL_LANE5), CGXStyle()
		.SetInterior(::GetSysColor(COLOR_BTNFACE))
		.SetEnabled(FALSE)
		.SetHorizontalAlignment(DT_CENTER)
		.SetVerticalAlignment(DT_VCENTER));
	for (int i = D_COL_LANE1; i <= D_COL_LANE5; i++)
	{
		if(i == D_COL_LANE5)
			strTemp.Format(_LS(IDS_CMD_MVHL_NUM_LANE_GT5), i-2);  
		else
			strTemp.Format(_T("%d"), i - 2);
		SetStyleRange(CGXRange(1, i), CGXStyle()
			.SetValue(strTemp));
	}

	SetCoveredCellsRowCol(2, D_COL_CLASS1, 4, D_COL_CLASS1);
	SetStyleRange(CGXRange(2, D_COL_CLASS1), CGXStyle()
		.SetInterior(::GetSysColor(COLOR_BTNFACE))
		.SetEnabled(FALSE)
		.SetHorizontalAlignment(DT_CENTER)
		.SetVerticalAlignment(DT_VCENTER)
		.SetValue(_LS(IDS_CMD_MVHL_BRD_CALSS))); // Bridge\nClass

	//SetCoveredCellsRowCol(2, D_COL_CLASS2, 2, D_COL_CLASS2);
	SetStyleRange(CGXRange(2, D_COL_CLASS2), CGXStyle()
		.SetInterior(::GetSysColor(COLOR_BTNFACE))
		.SetEnabled(FALSE)
		.SetHorizontalAlignment(DT_CENTER)
		.SetVerticalAlignment(DT_VCENTER)
		.SetValue(_LS(IDS_CMD_MVHL_BRD_CALSS1))); // First Class

	//SetCoveredCellsRowCol(3, D_COL_CLASS2, 3, D_COL_CLASS2);
	SetStyleRange(CGXRange(3, D_COL_CLASS2), CGXStyle()
		.SetInterior(::GetSysColor(COLOR_BTNFACE))
		.SetEnabled(FALSE)
		.SetHorizontalAlignment(DT_CENTER)
		.SetVerticalAlignment(DT_VCENTER)
		.SetValue(_LS(IDS_CMD_MVHL_BRD_CALSS2)));

	//SetCoveredCellsRowCol(4, D_COL_CLASS2, 4, D_COL_CLASS2);
	SetStyleRange(CGXRange(4, D_COL_CLASS2), CGXStyle()
		.SetInterior(::GetSysColor(COLOR_BTNFACE))
		.SetEnabled(FALSE)
		.SetHorizontalAlignment(DT_CENTER)
		.SetVerticalAlignment(DT_VCENTER)
		.SetValue(_LS(IDS_CMD_MVHL_BRD_CALSS3)));

	SetStyleRange(CGXRange().SetCols(D_COL_LANE1, D_COL_LANE5), CGXStyle()
		.SetHorizontalAlignment(DT_RIGHT)
		.SetVerticalAlignment(DT_VCENTER)
		.SetValueType(GX_VT_NUMERIC)
		.SetFormat(GX_FMT_FIXED));
		//.SetPlaces(2));

	if (m_bStd)
	{
// 		// 데이터 활성화
// 		SetStyleRange(CGXRange(2, D_COL_LANE2, 4, D_COL_LANE5), CGXStyle()
// 			//.SetReadOnly(FALSE)
// 			.SetEnabled(FALSE));

		// 일부 데이터 비활성화
// 		SetStyleRange(CGXRange(3, D_COL_LANE3, 4, D_COL_LANE5), CGXStyle()
// 			.SetReadOnly(TRUE)
// 			.SetEnabled(TRUE)  // TRUE 일때 OnPasteFromClipboard 도 수정하기!
// 			.SetHorizontalAlignment(DT_RIGHT)
// 			.SetVerticalAlignment(DT_VCENTER)
// 			.SetInterior(::GetSysColor(COLOR_BTNFACE)));
	}
	else
	{
		ASSERT(0);
	}

	ShowData(m_bBtType);

	GetParam()->SetLockReadOnly(bOldReadOnlyLock);
	LockUpdate(bOldLock);
	if (!bOldLock) Redraw();

	SetRightValidCell(1, 1);
}

void CCMMvhlItemStdFRGrid::ShowData(BOOL bBtType)
{
	BOOL bOldLock = LockUpdate(TRUE);
	BOOL bOldReadOnlyLock = GetParam()->IsLockReadOnly();
	GetParam()->SetLockReadOnly(FALSE);

	CString str;
	int i = 0;
	//////////////////////////////////////////////////////////////////////////
	//
	i = D_COL_CLASS1;
	if (!bBtType)
		str = _LS(IDS_CMD_MVHL_NUM_LANE); 
	else
		str = _T("Coefficent");
	SetStyleRange(CGXRange(1, i), CGXStyle().SetValue(str));

	//
	i = D_COL_LANE1;
	if (!bBtType)
		str.Format(_T("%d"), i-2);
	else
		str = _T("bt");
	SetStyleRange(CGXRange(1, i), CGXStyle().SetValue(str));

	//
	HideCols(D_COL_LANE2, D_COL_LANE5, bBtType);

	//
	i = D_COL_LANE5;
	if (!bBtType)
		str.Format(_LS(IDS_CMD_MVHL_NUM_LANE_GT5), i-2);
	else
		str.Format(_T("%d"), i-2);
	SetStyleRange(CGXRange(1, i), CGXStyle().SetValue(str));

	//////////////////////////////////////////////////////////////////////////
	COLORREF color = (bBtType ? COLORREF(RGB(236, 233, 216)) : COLOR_BTNFACE);

	SetStyleRange(CGXRange(2, D_COL_LANE2, 4, D_COL_LANE5), CGXStyle()
		.SetInterior(::GetSysColor(color))
		.SetEnabled(bBtType)
		.SetHorizontalAlignment(DT_RIGHT)
		.SetVerticalAlignment(DT_VCENTER)
		.SetValueType(GX_VT_NUMERIC)
		.SetFormat(GX_FMT_FIXED));

	SetStyleRange(CGXRange(2, D_COL_LANE1, 3, D_COL_LANE1), CGXStyle()
		.SetInterior(::GetSysColor(color))
		.SetEnabled(bBtType)
		.SetHorizontalAlignment(DT_RIGHT)
		.SetVerticalAlignment(DT_VCENTER)
		.SetValueType(GX_VT_NUMERIC)
		.SetFormat(GX_FMT_FIXED));

	SetStyleRange(CGXRange(4, D_COL_LANE1, 4, D_COL_LANE1), CGXStyle()
		.SetInterior(::GetSysColor(COLOR_BTNFACE))
		.SetEnabled(FALSE));

	//////////////////////////////////////////////////////////////////////////
	int nRowcount = 3;
	int nCol  = 5;
	double dCoefficient[5];
	for (int i = 0; i < nRowcount; i++)   // row
	{
		if (i == 0)
		{
			memcpy(dCoefficient, m_pMvhl->dCoefficient01, sizeof(dCoefficient));

			for (int j = 0; j < nCol; j++)
			{
				if (bBtType && j > 1)
				{
					str = _T("-");
				}
				else
				{
					str.Format(_T("%g"), dCoefficient[j]);
				}
				SetValueRange(CGXRange(2 + i, D_COL_LANE1 + j), str);
			}
		}
		else if (i == 1)
		{
			memcpy(dCoefficient, m_pMvhl->dCoefficient02, sizeof(dCoefficient));

			for (int j = 0; j < nCol; j++)
			{
				if (bBtType && j > 1)
				{
					str = _T("-");
				}
				else
				{
					if (j > 1) str = _T("-");
					else       str.Format(_T("%g"), dCoefficient[j]);
				}

				SetValueRange(CGXRange(2 + i, D_COL_LANE1 + j), str);
			}
		}
		else if (i == 2)
		{
			memcpy(dCoefficient, m_pMvhl->dCoefficient03, sizeof(dCoefficient));

			for (int j = 0; j < nCol; j++)
			{
				if (bBtType)
				{
					str = _T("-");
				}
				else
				{
					if (j > 1) str = _T("-");
					else       str.Format(_T("%g"), dCoefficient[j]);
				}
				SetValueRange(CGXRange(2 + i, D_COL_LANE1 + j), str);
			}
		}
		else
			ASSERT(0);

	}

	GetParam()->SetLockReadOnly(bOldReadOnlyLock);
	LockUpdate(bOldLock);
	if (!bOldLock) Redraw();

}
BOOL CCMMvhlItemStdFRGrid::SaveData()
{
	CString csNum;
	double dNum;

	int nRowcount = 3;
	int nCol  = 5;

	for (int i = 0; i < nRowcount; i++)
	{
		if (i == 0)
		{
			for (int j = 0; j < nCol; j++)
			{
				csNum = GetEditingValue(2 + i, D_COL_LANE1 + j);
				if (GetDoubleValue(csNum, dNum)) 
					m_pMvhl->dCoefficient01[j] = dNum;
				else                             
					m_pMvhl->dCoefficient01[j] = 0.0;
			}
		}
		else if (i == 1)
		{
			for (int j = 0; j < nCol; j++)
			{
				csNum = GetEditingValue(2 + i, D_COL_LANE1 + j);
				if (GetDoubleValue(csNum, dNum)) 
					m_pMvhl->dCoefficient02[j] = dNum;
				else                             
					m_pMvhl->dCoefficient02[j] = 0.0;
			}
		}
		else if (i == 2)
		{
			for (int j = 0; j < nCol; j++)
			{
				csNum = GetEditingValue(2 + i, D_COL_LANE1 + j);
				if (GetDoubleValue(csNum, dNum)) 
					m_pMvhl->dCoefficient03[j] = dNum;
				else                             
					m_pMvhl->dCoefficient03[j] = 0.0;
			}
		}
		else
			ASSERT(0);
	}
	return TRUE;
}

BOOL CCMMvhlItemStdFRGrid::SetDownValidCell(ROWCOL nRow, ROWCOL nCol)
{
	ROWCOL i, j;
	i = nRow + 1;
	j = nCol;
	if (nRow > GetRowCount()) { i = 1; }

	while (i <= GetRowCount())
	{
		CGXStyle style;
		ComposeStyleRowCol(i, j, &style);
		if (!style.GetIncludeEnabled() || style.GetEnabled())
		{
			if (SetCurrentCell(i, j)) return TRUE;
		}
		i++;  // 이전 열을 조사

	}

	return FALSE;
}
BOOL CCMMvhlItemStdFRGrid::SetUpValidCell(ROWCOL nRow, ROWCOL nCol)
{
	ROWCOL i, j;
	i = nRow - 1;
	j = nCol;

	if (i < 1) { i = GetRowCount(); }

	while (i >= 1)
	{
		CGXStyle style;
		ComposeStyleRowCol(i, j, &style);
		if (!style.GetIncludeEnabled() || style.GetEnabled())
		{
			if (SetCurrentCell(i, j)) return TRUE;
		}

		i--;  // 다음 줄로 이동

	}

	return FALSE;
}

CString CCMMvhlItemStdFRGrid::GetEditingValue(ROWCOL nRow, ROWCOL nCol)
{
	CString csNum;
	BOOL bGet = FALSE;
	ROWCOL ncRow, ncCol;
	if (GetCurrentCell(ncRow, ncCol))
	{
		if (ncRow == nRow && ncCol == nCol)
		{
			CGXControl* pControl = GetControl(nRow, nCol);
			if (pControl && pControl->IsActive())
				bGet = pControl->GetValue(csNum);
		}
	}
	if (!bGet) csNum = GetValueRowCol(nRow, nCol);
	return csNum;
}


BEGIN_MESSAGE_MAP(CCMMvhlItemStdFRGrid, CGXGridWnd)
	//{{AFX_MSG_MAP(CCMMvhlItemStdFRGrid)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CCMMvhlItemStdFRGrid message handlers
/////////////////////////////////////////////////////////////////////////////

BOOL CCMMvhlItemStdFRGrid::ProcessKeys(CWnd *pSender, UINT nMessage, UINT nChar, UINT nRepCnt, UINT flags)
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
				if (SetLeftValidCell(nRow, nCol)) { Redraw(); return TRUE; }
			}

			if (!bShift)
			{
				if (SetRightValidCell(nRow, nCol)) { Redraw(); return TRUE; }
			}
		}
		break;
		case VK_RETURN: case VK_RIGHT:  // 다음 셀로 이동
			if (nRow < GetRowCount())
				return SetDownValidCell(nRow, nCol);
			break;
		case VK_DOWN:
			if (nRow < GetRowCount())
				return SetDownValidCell(nRow, nCol);
			break;
		case VK_LEFT:  // 이전 셀로 이동
			if (nRow > 1)
			{
				return SetUpValidCell(nRow, nCol);
			}
			break;
		case VK_UP:
			if (nRow > 1)
			{
				return SetUpValidCell(nRow, nCol);
			}
			break;

		}
	}
	return CGXGridWnd::ProcessKeys(pSender, nMessage, nChar, nRepCnt, flags);
}

BOOL CCMMvhlItemStdFRGrid::OnValidateCell(ROWCOL nRow, ROWCOL nCol)
{
	CString csNum = GetEditingValue(nRow, nCol);
	double dNum;
	if (GetDoubleValue(csNum, dNum)) return TRUE;
	return FALSE;
}

BOOL CCMMvhlItemStdFRGrid::OnActivateGrid(BOOL bActivate)
{
	if (bActivate) return CGXGridWnd::OnActivateGrid(bActivate);
	ROWCOL nRow, nCol;
	if (GetCurrentCell(nRow, nCol))
	{
		if (!OnValidateCell(nRow, nCol))
		{
			CGXControl* pControl = GetControl(nRow, nCol);
			if (pControl && pControl->IsActive())
			{
				pControl->OnCancelEditing();
				pControl->Init(nRow, nCol);
				pControl->Refresh();
				pControl->OnCanceledEditing();
			}
		}
	}
	return CGXGridWnd::OnActivateGrid(bActivate);
}

void CCMMvhlItemStdFRGrid::OnModifyCell(ROWCOL nRow, ROWCOL nCol)
{
	CGXGridWnd::OnModifyCell(nRow, nCol);
	SaveData();

}

BOOL CCMMvhlItemStdFRGrid::GetDoubleValue(CString& csNum, double& dNum)
{
	int ecode;
	if (!IsValidDoubleNumber(csNum, ecode)) return InvalidDoubleNumber(ecode);
	ConvertFormulaToValue(csNum);
	dNum = _tstof(csNum);
	return TRUE;
}

// nErrCode : 1(수식 에러), 2(숫자 아님), 3(범위 초과)
BOOL CCMMvhlItemStdFRGrid::IsValidDoubleNumber(CString value, int &nErrCode)
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

	TCHAR *num = new TCHAR[value.GetLength() + 1];
	TCHAR *endp;
	double n;

	_stprintf(num, value);
	n = _tcstod(num, &endp);
	delete[] num;

	// 실수 범위 검사
	if (n == HUGE_VAL || n == -HUGE_VAL) { nErrCode = 3; return FALSE; }

	return TRUE;
}

BOOL CCMMvhlItemStdFRGrid::ConvertFormulaToValue(CString& value)
{
	if (GetSheetContext() == NULL && value.GetLength() > 0 && value.GetAt(0) == _T('='))
	{
		// 대문자 E로 표시된 지수를 소문자 e로 변경
		value.Replace(_T("E"), _T("e"));

		// Formula engine이 꺼져 있는데 수식이 입력되어 있으면
		CGXFormulaSheet fs;
		CGXFormula fm;
		if (!fs.ParseExpression(value, fm))
		{
			SetWarningText(_LS(IDS_CMD0417__Error___Invalid_formula_));
			return FALSE;
		}
		if (!fs.EvaluateExpression(value, fm))
		{
			SetWarningText(_LS(IDS_CMD0417__Error___Can_t_evaluate_formula_));
			return FALSE;
		}
	}
	return TRUE;
}

BOOL CCMMvhlItemStdFRGrid::IsNumeric(CString str, int& ecode)
{
	if (str.GetLength() < 1) return FALSE;

	TCHAR *num = new TCHAR[str.GetLength() + 1];
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

BOOL CCMMvhlItemStdFRGrid::InvalidDoubleNumber(int nErrCode)
{
	switch (nErrCode)
	{
	case 1: return ValidateError(_LS(IDS_CMD0417__Formula_is_invalid_));
	case 2: return ValidateError(_LS(IDS_CMD0417__Character_exist_in_the_number_));
	case 3: return ValidateError(_LS(IDS_CMD0417__Out_of_value_range_));
	}
	return FALSE;
}

BOOL CCMMvhlItemStdFRGrid::ValidateError(LPCTSTR lpszError)
{
	SetWarningText(lpszError);
	return FALSE;
}

BOOL CCMMvhlItemStdFRGrid::OnEndEditing(ROWCOL nRow, ROWCOL nCol)
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
BOOL CCMMvhlItemStdFRGrid::SetRightValidCell(ROWCOL nRow, ROWCOL nCol)
{
	ROWCOL i, j;
	i = nRow; j = nCol + 1; // 현재 행 다음 열
	if (j > GetColCount()) { i++; j = 1; }

	while (i <= GetRowCount())
	{
		while (j <= GetColCount())
		{
			// 열이 선택될 수 있는지 조사
			//if (SetCurrentCell(i, j)) return TRUE;
			CGXStyle style;
			ComposeStyleRowCol(i, j, &style);
			if (!style.GetIncludeEnabled() || style.GetEnabled())
			{
				if (SetCurrentCell(i, j)) return TRUE;
			}
			j++;  // 다음 열을 조사
		}
		i++;  // 다음 줄로 이동
		j = 1;
	}

	return FALSE;
}
BOOL CCMMvhlItemStdFRGrid::SetLeftValidCell(ROWCOL nRow, ROWCOL nCol)
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

///////////////////////////복사 및 붙여넣기 했을 때 불리는 함수//////////////////////

BOOL CCMMvhlItemStdFRGrid::CopyRange(const CGXRangeList& selList)
{
	CGXRangeList mSelList;
	CGXRange* mRange;
	POSITION position = selList.GetHeadPosition();

	while (TRUE)
	{
		if (!position || position < 0)
			break;

		mRange = new CGXRange();
		if (m_bStd)
		{
			if (mRange->IntersectRange(CGXRange(D_SIZE_START_ROW, D_COL_LANE1, D_SIZE_END_ROW, D_COL_LANE5), selList.GetAt(position)))
			{
				mSelList.AddTail(mRange);
			}
			else
				delete mRange;
		}
		else
		{
			if (mRange->IntersectRange(CGXRange(D_SIZE_START_ROW, D_COL_LANE1, D_SIZE_END_ROW, D_COL_LANE5), selList.GetAt(position)))
			{
				mSelList.AddTail(mRange);
			}
			else
				delete mRange;
		}
		selList.GetNext(position);
	}

	return CGXGridWnd::CopyRange(mSelList);
}

BOOL CCMMvhlItemStdFRGrid::OnPasteFromClipboard(const CGXRange &range)
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
			continue; //전체 비활성화면 continue 활성화

			if(toi < D_SIZE_START_ROW) continue;
			if(toj < D_COL_LANE1) continue;
			
			if(m_bStd)
			{
				if (toi == 3 || toi == 4) // row
				{
					if (toj == D_COL_LANE3 || toj == D_COL_LANE4 || toj == D_COL_LANE5)
						continue;
				}
			}
			//BOOL bCondition = (toi == D_SIZE_END_ROW && toj == 1 && m_bStd);
			//BOOL bCondition = (toi == D_SIZE_END_ROW && toj == 1);
			//if (!bCondition)
			{
				SetValueRange(CGXRange(toi, toj), x.GetValueRowCol(fromi, fromj));
				if (!OnValidateCell(toi, toj))
				{
					DisplayWarningText();
					ShowData(m_bBtType);
					return FALSE;
				}
			}
			if (fromCols != 1) fromj++;
		}
		if (fromRows != 1)  fromi++;
	}

	SaveData();
	return TRUE;
}

BOOL CCMMvhlItemStdFRGrid::VerifyPasteRange(CGXRange& from, CGXRange& to)
{

	int fromCols, fromRows, toCols, toRows;
	fromCols = from.right - from.left + 1;
	fromRows = from.bottom - from.top + 1;
	toRows = GetSelectedRowsCount(to.top, to.bottom);
	toCols = GetSelectedColsCount(to.left, to.right);

	if (toRows == 1 && toCols == 1)
	{
		int i;
		for (i = to.left; i <= D_COL_LANE5; i++)
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

		for (i = to.top; i <= D_SIZE_END_ROW; i++)
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

int CCMMvhlItemStdFRGrid::GetSelectedRowsCount(int top, int bottom)
{
	int toRows;
	toRows = bottom - top + 1;
	return toRows;
}

int CCMMvhlItemStdFRGrid::GetSelectedColsCount(int left, int right)
{
	int toCols;
	toCols = 0;
// 	if (m_bStd)
// 	{
// 		if (left <= 2 && right >= 2)
// 			toCols++;
// 		if (left <= 4 && right >= 4)
// 			toCols++;
// 		return toCols;
// 	}
// 	else
		return right - left + 1;
}

BOOL CCMMvhlItemStdFRGrid::GetStyleRowCol(ROWCOL nRow, ROWCOL nCol, CGXStyle& style, GXModifyType mt, int nType)
{
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
	BOOL bCurrentRow = (nRow == ncRow);
	BOOL bCurrentCell = (nRow == ncRow && nCol == ncCol);

	// Enable, ReadOnly와 같은 Cell의 상태를 조사한다.
	// ComposeStyleRowCol은 호출해서는 안된다.
	// 그래서 여기서는 직접 조사한다.
	// 단 Cell Style, Column Style만 조사한다.

	BOOL bEnabled = TRUE;
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

	BOOL bHeaderCol = IsFrozenCol(nCol);
	BOOL bHeaderRow = IsFrozenRow(nRow);

	// Printing 모드이면 헤더 Column이 프린트 되기 위해 다음과 같이 리턴한다.
	if (IsPrinting())
	{
		if (bHeaderCol) style.SetControl(GX_IDS_CTRL_HEADER);
		return TRUE;
	}
	// [현재 대화창에 맞게 수정]
	// 일반 셀이 가장 많으므로 빨리 검사하고 바로 리턴할 수 있게 코드를 앞으로 뺀다.
	if (bEnabled && !bCurrentCell && !bCurrentRow && !bHeaderCol)
	{
		style.SetInterior(COLORREF(RGB(255, 255, 255)))
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
			.SetInterior(GetSysColor(COLOR_BTNFACE))
			.SetTextColor(COLORREF(RGB(0, 0, 0)));
		else style                      // Disabled Cell
			.SetInterior(GetSysColor(COLOR_BTNFACE))
			.SetTextColor(COLORREF(RGB(0, 0, 0)));

		return TRUE;
	}

	if (bCurrentCell) style
		.SetInterior(COLORREF(RGB(255, 255, 255)))
		.SetTextColor(COLORREF(RGB(0, 0, 0)));
	else if (bCurrentRow) style
		.SetInterior(COLORREF(RGB(255, 255, 255)))
		.SetTextColor(COLORREF(RGB(0, 0, 0)));
	else if (bHeaderCol || bHeaderRow) style    // Enabled Header Col(Only Frozen)
		.SetInterior(::GetSysColor(COLOR_BTNFACE))
		.SetTextColor(COLORREF(RGB(0, 0, 0)));


	return TRUE;
}
