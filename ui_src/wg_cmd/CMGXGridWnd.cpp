// CMGXGridWnd.cpp : implementation file
// 2003. 6. 27   by TAE
//
// CGXGridWnd의 기본적인 기능에 Copy 기능 추가 
//////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMGXGridWnd.h"

#include "..\wg_common\wg_common_TBGrid.h"


///////////////////////////////////////////////////////////////////////////////
// CCMGXGridWnd

void CCMGXGridWnd::Initialize()
{
	CGXGridWnd::Initialize();
	
	// cell 바깥영역 배경색 지정 
	GetParam()->GetProperties()->SetColor(GX_COLOR_BACKGROUND, CTBGrid::m_Color.OuterBackground);
	
	// Header 배경색 지정 
	ChangeColHeaderStyle(CGXStyle().SetInterior(CTBGrid::m_Color.RowColHeaderBg)); 
	ChangeRowHeaderStyle(CGXStyle().SetInterior(CTBGrid::m_Color.RowColHeaderBg)); 
	
	// 기타 Cells 배경색 지정
	ChangeStandardStyle(CGXStyle( ).SetInterior(CTBGrid::m_Color.NormalModeBg));

	GetParam()->EnableMoveCols(FALSE);  // Disable moving column
	GetParam()->EnableMoveRows(FALSE);   // Disable moving row
	GetParam()->EnableTrackRowHeight(FALSE);  // Disable resizing row height
}

BOOL CCMGXGridWnd::Copy( )
{
	// 실제로 Selected Range는 Row, Col의 조합으로 저장된다는 사실에 착안 
	// ( 따라서 Multi Selection의 경우는 별도의 처리를 해주어야 함. )
	CRowColArray aSelectedRow, aSelectedCol;
	
	int nSelectedCol = GetSelectedCols(aSelectedCol, FALSE, FALSE);
	for(int i=0; i<nSelectedCol; i++)
	{
		ROWCOL nCol = aSelectedCol.GetAt(i);
		if(IsColHidden(nCol))
			SelectRange(CGXRange().SetCols(nCol), FALSE);
	}  

	int nSelectedRow = GetSelectedRows(aSelectedRow, FALSE, FALSE);
	for(int i=0; i<nSelectedRow; i++)
	{
		ROWCOL nRow = aSelectedRow.GetAt(i);
		if(IsRowHidden(nRow))
			SelectRange(CGXRange().SetRows(nRow), FALSE);
	}
	return CGXGridWnd::Copy();
}

BOOL CCMGXGridWnd::OnPasteFromClipboard(const CGXRange &range)
{
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

	CString sErrMsg = _T("");
	ROWCOL nRow, nCol;
	VERIFY(GetCurrentCell(nRow, nCol));
	// 대상 범위가 지정되지 않은 경우
	// 현재 셀이 있는 위치를 기준으로 한다.
	if (range.GetHeight() == 1 && range.GetWidth() == 1)
	{
		// 대상 범위가 Column 한계를 넘어서면 에러
		if (nCol + r.GetWidth() - 1 > GetColCount())
		{
			//sErrMsg = _T("붙여 넣을 범위가 칼럼 범위를 초과합니다. 위치를 다시 지정하세요.");
			sErrMsg = _LS(IDS_CMD_PASTE_EXCEED_COLUMN);
			goto END_OF_PASTE;
		}
		// 대상 범위가 Row 한계를 넘어서면 에러
		if (nRow + r.GetHeight() - 1 > GetRowCount())
		{
			//sErrMsg = _T("붙여 넣을 범위가 로우 범위를 초과합니다. 위치를 다시 지정하세요.");
			sErrMsg = _LS(IDS_CMD_PASTE_EXCEED_ROW);
			goto END_OF_PASTE;
		}

	}
	// 대상 범위가 지정된 경우 : 원본과 같은 범위, Fill Mode(원본 1줄, 동일 칸수)
	else
	{
		// Fill 조건을 검사한다.
		// 원본이 1줄 이상이면 에러, 칸수가 달라도 에러가 된다.
		BOOL bFillMode = FALSE;
		if (range.GetHeight() != r.GetHeight() || range.GetWidth() != r.GetWidth())
		{
			// Fill Mode가 안 되는 조건
			if (r.GetHeight() > 1 || range.GetWidth() != r.GetWidth())
			{
				//sErrMsg = _T("대상 범위를 지정한 경우는 원본 범위의 크기와 같아야 합니다.");
				sErrMsg = _LS(IDS_CMD_PASTE_RANGE_MISMATCH);
				goto END_OF_PASTE;
			}
			// 대상 범위가 1줄 이상이면 Fill Mode, 아니면 Paste와 동일하다.
			if (range.GetHeight() > 1) bFillMode = TRUE;
		}

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

END_OF_PASTE:
	if (!sErrMsg.IsEmpty())
	{
		AfxMessageBox(sErrMsg);
		x.DestroyWindow();
		return FALSE;
	}

	x.DestroyWindow();
	return CGXGridWnd::OnPasteFromClipboard(range);
}

BOOL CCMGXGridWnd::ProcessKeys(CWnd *pSender, UINT nMessage, UINT nChar, UINT nRepCnt, UINT flags)
{
	BOOL bCtl = GetKeyState(VK_CONTROL) & 0x8000;
	BOOL bShift = GetKeyState(VK_SHIFT) & 0x8000;

	if (bCtl || bShift) 
		return CGXGridWnd::ProcessKeys(pSender, nMessage, nChar, nRepCnt, flags);

	if (nMessage == WM_KEYDOWN)
	{
		switch(nChar)
		{
		case VK_INSERT: 
		case VK_DELETE: 
			return TRUE;
		}
	}

	return CGXGridWnd::ProcessKeys(pSender, nMessage, nChar, nRepCnt, flags);
}

void CCMGXGridWnd::SetColWidthByDPI(ROWCOL nColSt, ROWCOL nColEd, int nWidth, int* anWidthArray, UINT flags, GXCmdType ctCmd)
{
	double dFactor = globalUtils.GetDPIScale();

	int nWidthDPI = nWidth;
	if (1.0 < dFactor && dFactor <= 1.25)
	{
		nWidthDPI = int(nWidth * dFactor);
	}
	else if (1.25 < dFactor && dFactor <= 1.5)
	{
		nWidthDPI = int(nWidth * dFactor * 0.95);
	}
	else
	{
		nWidthDPI = int(nWidth * dFactor);
	}

	SetColWidth(nColSt, nColEd, nWidthDPI, anWidthArray, flags, ctCmd);
}

void CCMGXGridWnd::SetRowHeightByDPI(ROWCOL nRowSt, ROWCOL nRowEd, int nHeight, int* anHeightArray, UINT flags, GXCmdType ctCmd)
{
	SetRowHeight(nRowSt, nRowEd, globalUtils.ScaleByDPI(nHeight), anHeightArray, flags, ctCmd);
}


/////////////////////////////////////////////////////////////////////////////
// CCMGXGridWnd2 

BOOL CCMGXGridWnd2::GetStyleRowCol(ROWCOL nRow, ROWCOL nCol, CGXStyle& style, GXModifyType mt, int nType)
{
	BOOL bCurrentRow;
	BOOL bCurrentCell;
	BOOL bEnabled;
	BOOL bHeaderCol;
	BOOL bHeaderRow;

	ROWCOL ncRow, ncCol;    // 현재 Row, Col
	CCMGXGridWnd::GetStyleRowCol(nRow, nCol, style, mt, nType);

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
			.SetInterior(CTBGrid::m_Color.NormalModeBg)
			.SetTextColor(CTBGrid::m_Color.NormalModeFg);

		return TRUE;
	}

	// Disable되었을 때
	if (!bEnabled)  
	{
		if (bHeaderCol || bHeaderRow) style           // Row or Column Header
			.SetControl(GX_IDS_CTRL_HEADER)
			.SetInterior(CTBGrid::m_Color.RowColHeaderBg)
			.SetTextColor(CTBGrid::m_Color.NormalModeFg);
		else if (bCurrentRow) style     // Disabled Current Row
			.SetInterior(CTBGrid::m_Color.DisabledBg)
			.SetTextColor(CTBGrid::m_Color.ReadOnlyCurrentRowFg);
		else style                      // Disabled Cell
			.SetInterior(CTBGrid::m_Color.DisabledBg)
			.SetTextColor(CTBGrid::m_Color.DisabledFg);

		return TRUE;
	}

	if (bCurrentCell) style
		.SetInterior(CTBGrid::m_Color.NormalCurrentCellBg)
		.SetTextColor(CTBGrid::m_Color.NormalCurrentCellFg);
	else if (bCurrentRow) style
		.SetInterior(CTBGrid::m_Color.NormalModeBg)
		.SetTextColor(CTBGrid::m_Color.NormalModeFg);
	else if (bHeaderCol || bHeaderRow) style    // Enabled Header Col(Only Frozen)
		.SetInterior(CTBGrid::m_Color.NormalModeBg)
		.SetTextColor(CTBGrid::m_Color.NormalModeFg);

	return TRUE;
}


// nErrCode : 1(수식 에러), 2(숫자 아님), 3(범위 초과), 4(정수아님)
BOOL CCMGXGridWnd2::IsValidLongNumber(CString value, int &nErrCode)
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
	delete[] num; num=NULL;

	// 정수인지 실수인지 조사
	if (!bAllScaned) { nErrCode = 4; return FALSE; }
	// 정수 범위 검사
	if (n == LONG_MAX || n == LONG_MIN) { nErrCode = 3; return FALSE; }

	return TRUE;
}

// nErrCode : 1(수식 에러), 2(숫자 아님), 3(범위 초과)
BOOL CCMGXGridWnd2::IsValidDoubleNumber(CString value, int &nErrCode)
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

BOOL CCMGXGridWnd2::ConvertFormulaToValue(CString& value)
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
			SetWarningText(_LS(IDS_CMD_ERROR_INVALID_FORMULA));
			return FALSE;
		}
		if (!fs.EvaluateExpression(value, fm))
		{
			SetWarningText(_LS(IDS_CMD_ERROR_CANT_CALC_FORMULA));
			return FALSE;
		}
	}
	return TRUE;
}

BOOL CCMGXGridWnd2::IsNumeric(CString str, int& ecode)
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

BOOL CCMGXGridWnd2::ValidateError(LPCTSTR lpszError)
{
	AfxMessageBox(lpszError);
	return FALSE;
}

BOOL CCMGXGridWnd2::InvalidLongNumber(int nErrCode)
{
	switch(nErrCode)
	{
	case 1: return ValidateError(_LS(IDS_CMD_ERROR_FORMULA_INVALID));
	case 2: 
	case 4: return ValidateError(_LS(IDS_CMD_ERROR_REQUIRE_INTEGER));
	case 3: return ValidateError(_LS(IDS_CMD_ERROR_OUT_OF_RANGE));
	}
	return FALSE;
}

BOOL CCMGXGridWnd2::InvalidDoubleNumber(int nErrCode)
{
	switch(nErrCode)
	{
	case 1: return ValidateError(_LS(IDS_CMD_ERROR_FORMULA_INVALID)); 
	case 2: return ValidateError(_LS(IDS_CMD_ERROR_CHARACTER_EXIST));
	case 3: return ValidateError(_LS(IDS_CMD_ERROR_OUT_OF_RANGE));
	}
	return FALSE;
}

void CCMGXGridWnd2::SetColWidthByDPI(ROWCOL nColSt, ROWCOL nColEd, int nWidth, int* anWidthArray, UINT flags, GXCmdType ctCmd)
{
	double dFactor = globalUtils.GetDPIScale();

	int nWidthDPI = nWidth;
	if (1.0 < dFactor && dFactor <= 1.25)
	{
		nWidthDPI = int(nWidth * dFactor);
	}
	else if (1.25 < dFactor && dFactor <= 1.5)
	{
		nWidthDPI = int(nWidth * dFactor * 0.95);
	}
	else
	{
		nWidthDPI = int(nWidth * dFactor);
	}

	SetColWidth(nColSt, nColEd, nWidthDPI, anWidthArray, flags, ctCmd);
}

void CCMGXGridWnd2::SetRowHeightByDPI(ROWCOL nRowSt, ROWCOL nRowEd, int nHeight, int* anHeightArray, UINT flags, GXCmdType ctCmd)
{
	SetRowHeight(nRowSt, nRowEd, globalUtils.ScaleByDPI(nHeight), anHeightArray, flags, ctCmd);
}
