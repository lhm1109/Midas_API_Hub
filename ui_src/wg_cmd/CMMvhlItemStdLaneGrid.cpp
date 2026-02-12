// CMMvhlItemStdLaneGrid.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMMvhlItemStdLaneGrid.h"


#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_SectUtil.h"

#include "..\wg_db\wg_db_DBDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define D_SIZE_START_ROW 3
#define D_SIZE_END_ROW 6
#define D_SIZE_ROW_SIZE 4
/////////////////////////////////////////////////////////////////////////////
// CCMMvhlItemStdLaneGrid

CCMMvhlItemStdLaneGrid::CCMMvhlItemStdLaneGrid()
{
	m_pMvhl = 0;
	m_bInitDone = FALSE;
	m_pParent=0;
	m_bStd = TRUE;
}

CCMMvhlItemStdLaneGrid::~CCMMvhlItemStdLaneGrid()
{
}
void CCMMvhlItemStdLaneGrid::InitGrid()
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
			.SetVerticalAlignment(DT_BOTTOM)
			.SetAllowEnter(FALSE));

	LockUpdate(FALSE);
	Redraw();
}

void CCMMvhlItemStdLaneGrid::SetStyleOnSizeCell()
{

	int count;
	int i;
	count=4;
	
	for (i = 0; i <= count; i++)
	{
		SetStyleRange(CGXRange(i+3, 2), CGXStyle()
			.SetFormat(GX_FMT_FIXED).SetPlaces(2));
		SetStyleRange(CGXRange(i+3, 4), CGXStyle()
			.SetFormat(GX_FMT_FIXED).SetPlaces(2));
	}

}
void CCMMvhlItemStdLaneGrid::SetDataSource(T_MVHL_D* pMvhl,CWnd* pParent, BOOL bStd)
{
	ASSERT(pMvhl);
	m_pMvhl = pMvhl;
	m_pParent=pParent;
	m_bStd = bStd;

	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	CString strTemp,strUnit;
	BOOL bOldLock = LockUpdate(TRUE);
	BOOL bOldReadOnlyLock = GetParam()->IsLockReadOnly();
	GetParam()->SetLockReadOnly(FALSE);
	int count;
	count=4;
	SetRowCount(count+2);
	SetColCount(5);


	// set column width
	if(m_bStd)
	{
		SetColWidth(1, 1, globalUtils.ScaleByDPI(105));
		SetColWidth(2, 2 , globalUtils.ScaleByDPI(80));
		SetColWidth(3, 3,  globalUtils.ScaleByDPI(80));
		SetColWidth(4, 4 , globalUtils.ScaleByDPI(80));
		SetColWidth(5, 5, globalUtils.ScaleByDPI(105));
	}
	else
	{
		SetColWidth(1, 1, globalUtils.ScaleByDPI(105));
		SetColWidth(2, 2 , globalUtils.ScaleByDPI(75));
		SetColWidth(3, 3,  globalUtils.ScaleByDPI(80));
		SetColWidth(4, 4 , globalUtils.ScaleByDPI(75));
		SetColWidth(5, 5, globalUtils.ScaleByDPI(105));
	}
	
	int nHeight = GetRowHeight(1);

#ifdef _RUS
	SetRowHeight(2,2,nHeight*globalUtils.ScaleByDPI(4));
#else
	SetRowHeight(2,2,nHeight*globalUtils.ScaleByDPI(2));
#endif
	
	SetCoveredCellsRowCol(1, 1, 2, 1);
	SetStyleRange(CGXRange(1, 1), CGXStyle()
		.SetInterior(::GetSysColor(COLOR_BTNFACE))
		.SetHorizontalAlignment(DT_CENTER)
		.SetVerticalAlignment(DT_VCENTER)
		.SetValue(_LS(IDS_CMD_MVHL_LOCATION)));
	

	SetCoveredCellsRowCol(1, 2, 1, 3);
	SetStyleRange(CGXRange(1, 2), CGXStyle()
		.SetInterior(::GetSysColor(COLOR_BTNFACE))
		.SetEnabled(FALSE)
		.SetHorizontalAlignment(DT_CENTER)
		.SetVerticalAlignment(DT_VCENTER)
		.SetValue(_LS(IDS_CMD_MVHL_TANDEM)));

	SetCoveredCellsRowCol(1, 4, 1, 5);
	SetStyleRange(CGXRange(1, 4), CGXStyle()
		.SetInterior(::GetSysColor(COLOR_BTNFACE))
		.SetEnabled(FALSE)
		.SetHorizontalAlignment(DT_CENTER)
		.SetVerticalAlignment(DT_VCENTER)
		.SetValue(_LS(IDS_CMD_MVHL_UDL)));

	SetStyleRange(CGXRange(2, 2), CGXStyle()
		.SetInterior(::GetSysColor(COLOR_BTNFACE))
		.SetEnabled(FALSE)
		.SetHorizontalAlignment(DT_CENTER)
		.SetVerticalAlignment(DT_VCENTER)
		.SetValue(_LS(IDS_CMD_MVHL_TANDEM_ADJUST)));

	pDoc->m_pUnitCtrl->GetUnitSystem(CUnitCtrl::m_MVHL_UNIT.dTandemLoad, strUnit); 
	strTemp.Format(_T("%s (%s)"),_LS(IDS_CMD_MVHL_TANDEM_AXLE),strUnit);
 
	SetStyleRange(CGXRange(2, 3), CGXStyle()
		.SetInterior(::GetSysColor(COLOR_BTNFACE))
		.SetEnabled(FALSE)
		.SetHorizontalAlignment(DT_CENTER)
		.SetVerticalAlignment(DT_VCENTER)
		.SetValue(strTemp));
	SetStyleRange(CGXRange(2, 4), CGXStyle()
		.SetInterior(::GetSysColor(COLOR_BTNFACE))
		.SetEnabled(FALSE)
		.SetHorizontalAlignment(DT_CENTER)
		.SetVerticalAlignment(DT_VCENTER)
		.SetValue(_LS(IDS_CMD_MVHL_UDL_ADJUST)));
	pDoc->m_pUnitCtrl->GetUnitSystem(CUnitCtrl::m_MVHL_UNIT.dUDLLoad, strUnit); 
	strTemp.Format(_T("%s (%s)"),_LS(IDS_CMD_MVHL_UDL_UNIFORMLY),strUnit);
	SetStyleRange(CGXRange(2, 5), CGXStyle()
		.SetInterior(::GetSysColor(COLOR_BTNFACE))
		.SetEnabled(FALSE)
		.SetHorizontalAlignment(DT_CENTER)
		.SetVerticalAlignment(DT_VCENTER)
		.SetValue(strTemp));
	SetStyleRange(CGXRange().SetCols(1), CGXStyle()
		.SetReadOnly(FALSE)
		.SetEnabled(FALSE)
		.SetHorizontalAlignment(DT_LEFT)
		.SetVerticalAlignment(DT_VCENTER)
		.SetInterior(::GetSysColor(COLOR_BTNFACE)));

	SetStyleRange(CGXRange().SetCols(2,5), CGXStyle()
		.SetHorizontalAlignment(DT_RIGHT)
		.SetVerticalAlignment(DT_VCENTER)
		.SetValueType(GX_VT_NUMERIC)
		.SetFormat(GX_FMT_FIXED)
		.SetPlaces(2));

	if(m_bStd)
	{
		SetStyleRange(CGXRange().SetCols(3), CGXStyle()
			.SetReadOnly(FALSE)
			.SetEnabled(FALSE)
			.SetHorizontalAlignment(DT_RIGHT)
			.SetVerticalAlignment(DT_VCENTER)
			.SetInterior(::GetSysColor(COLOR_BTNFACE)));

		SetStyleRange(CGXRange().SetCols(5), CGXStyle()
			.SetReadOnly(FALSE)
			.SetEnabled(FALSE)
			.SetHorizontalAlignment(DT_RIGHT)
			.SetVerticalAlignment(DT_VCENTER)
			.SetInterior(::GetSysColor(COLOR_BTNFACE)));
		SetStyleRange(CGXRange(6, 2), CGXStyle()
			.SetReadOnly(FALSE)
			.SetEnabled(FALSE)
			.SetHorizontalAlignment(DT_RIGHT)
			.SetVerticalAlignment(DT_VCENTER)
			.SetInterior(::GetSysColor(COLOR_BTNFACE)));
	}
	else
	{
		SetStyleRange(CGXRange(6, 2, 6, 3), CGXStyle()
			.SetReadOnly(FALSE)
			.SetEnabled(FALSE)
			.SetHorizontalAlignment(DT_RIGHT)
			.SetVerticalAlignment(DT_VCENTER)
			.SetInterior(::GetSysColor(COLOR_BTNFACE)));
	}
 
	int i = 1;
	for( i=1;i<=3;i++)
	{
		strTemp.Format(_LS(IDS_CMD_MVHL_LANENUMBER),i);
		SetStyleRange(CGXRange(i+2, 1), CGXStyle()
			.SetValue(strTemp));
	}
	strTemp=_LS(IDS_CMD_MVHL_OTHERLANE);
		SetStyleRange(CGXRange(i+2, 1), CGXStyle()
			.SetValue(strTemp));
	

	GetParam()->SetLockReadOnly(bOldReadOnlyLock);

	LockUpdate(bOldLock);
	if (!bOldLock) Redraw();
	ShowData();
	SetRightValidCell(1, 1);
	
	SetRowHeight(3,3, globalUtils.ScaleByDPI(20));
	SetRowHeight(4,4, globalUtils.ScaleByDPI(20));
	SetRowHeight(5,5, globalUtils.ScaleByDPI(20));
	SetRowHeight(6,6, globalUtils.ScaleByDPI(34));
}

void CCMMvhlItemStdLaneGrid::ShowData()
{
	int count;
	count=4;
	CString str;
	for (int i = 0; i <count; i++)
	{
		if(i==count-1) str = _T("0");
		else           str.Format(_T("%g"),m_pMvhl->dTandemAdjust[i]);
		SetValueRange(CGXRange(i+3, 2),str);
		if(i==count-1) str = _T("0");
		else           str.Format(_T("%g"),m_pMvhl->dTandemLoad[i]);
		SetValueRange(CGXRange(i+3, 3),str);

		str.Format(_T("%g"),m_pMvhl->dUDLAdjust[i]);
		SetValueRange(CGXRange(i+3, 4),str);
		str.Format(_T("%g"),m_pMvhl->dUDLLoad[i]);
		SetValueRange(CGXRange(i+3, 5),str);
	}  
}
BOOL CCMMvhlItemStdLaneGrid::SaveData()
{
	CString csNum;
	double dNum;

	int count;
	count=4;
	for (int i = 0; i <count; i++)
	{
		csNum = GetEditingValue(i+3, 2);
		if (GetDoubleValue(csNum, dNum)) 
		{
			m_pMvhl->dTandemAdjust[i] = dNum;
		}
		else
		{
			m_pMvhl->dTandemAdjust[i] = 0.0;
		}
		csNum = GetEditingValue(i+3, 3);
		if (GetDoubleValue(csNum, dNum)) 
		{
			m_pMvhl->dTandemLoad[i] = dNum;
		}
		else
		{
			m_pMvhl->dTandemLoad[i] = 0.0;
		}
		csNum = GetEditingValue(i+3, 4);
		if (GetDoubleValue(csNum, dNum)) 
		{
			m_pMvhl->dUDLAdjust[i] = dNum;
		}
		else
		{
			m_pMvhl->dUDLAdjust[i] = 0.0;
		}
		csNum = GetEditingValue(i+3, 5);
		if (GetDoubleValue(csNum, dNum)) 
		{
			m_pMvhl->dUDLLoad[i] = dNum;
		}
		else
		{
			m_pMvhl->dUDLLoad[i] = 0.0;
		}
	}
	return TRUE;
}

BOOL CCMMvhlItemStdLaneGrid::SetDownValidCell(ROWCOL nRow, ROWCOL nCol)
{
	ROWCOL i,j;
	i=nRow+1;
	j=nCol;
	if (nRow > GetRowCount()) { i=1; }  

	while (i <=GetRowCount())
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
BOOL CCMMvhlItemStdLaneGrid::SetUpValidCell(ROWCOL nRow, ROWCOL nCol)
{
	ROWCOL i,j;
	i = nRow-1;
	j = nCol;

	if (i < 1) { i= GetRowCount(); }

	while (i >=1)
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

CString CCMMvhlItemStdLaneGrid::GetEditingValue(ROWCOL nRow, ROWCOL nCol)
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




BEGIN_MESSAGE_MAP(CCMMvhlItemStdLaneGrid, CGXGridWnd)
	//{{AFX_MSG_MAP(CCMMvhlItemStdLaneGrid)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CCMMvhlItemStdLaneGrid message handlers
/////////////////////////////////////////////////////////////////////////////

BOOL CCMMvhlItemStdLaneGrid::ProcessKeys(CWnd *pSender, UINT nMessage, UINT nChar, UINT nRepCnt, UINT flags)
{
	CRowColArray awRows;
	ROWCOL nRow=1, nCol=1;
	//CGXControl* pControl;

	if (!GetCurrentCell(nRow, nCol)) // if no current cell, default action
		return CGXGridWnd::ProcessKeys(pSender, nMessage, nChar, nRepCnt, flags);

	//pControl = GetControl(nRow, nCol);
	if (nMessage == WM_KEYDOWN)
	{
		switch(nChar)
		{
		case VK_TAB: 
			{
				BOOL bShift = GetKeyState(VK_SHIFT) & 0x8000;
				if (bShift && SetDownValidCell(nRow, nCol)) break;
				if (!bShift && SetUpValidCell(nRow,nCol)) break;

				CWnd* pWnd = GetParent();
				if (pWnd && pWnd->GetSafeHwnd())
				{
					//pWnd->PostMessage(nMessage, nChar, nRepCnt);
					CWnd* pNextWnd = pWnd->GetNextDlgTabItem(this, bShift);
					if (pNextWnd && pNextWnd->GetSafeHwnd()) pNextWnd->SetFocus();
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
			if(nRow>1)
			{
				return SetUpValidCell(nRow,nCol);
			}
			break;

		}
	}
	return CGXGridWnd::ProcessKeys(pSender, nMessage, nChar, nRepCnt, flags);
}

BOOL CCMMvhlItemStdLaneGrid::OnValidateCell(ROWCOL nRow, ROWCOL nCol)
{
	CString csNum = GetEditingValue(nRow, nCol);
	double dNum;
	if (GetDoubleValue(csNum, dNum)) return TRUE;
	return FALSE;
}

BOOL CCMMvhlItemStdLaneGrid::OnActivateGrid(BOOL bActivate)
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

void CCMMvhlItemStdLaneGrid::OnModifyCell(ROWCOL nRow, ROWCOL nCol)
{
	CGXGridWnd::OnModifyCell(nRow, nCol);
	SaveData();  

}

BOOL CCMMvhlItemStdLaneGrid::GetDoubleValue(CString& csNum, double& dNum)
{
	int ecode;
	if (!IsValidDoubleNumber(csNum, ecode)) return InvalidDoubleNumber(ecode);
	ConvertFormulaToValue(csNum);
	dNum = _tstof(csNum);
	return TRUE;
}

// nErrCode : 1(수식 에러), 2(숫자 아님), 3(범위 초과)
BOOL CCMMvhlItemStdLaneGrid::IsValidDoubleNumber(CString value, int &nErrCode)
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

BOOL CCMMvhlItemStdLaneGrid::ConvertFormulaToValue(CString& value)
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

BOOL CCMMvhlItemStdLaneGrid::IsNumeric(CString str, int& ecode)
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

BOOL CCMMvhlItemStdLaneGrid::InvalidDoubleNumber(int nErrCode)
{
	switch(nErrCode)
	{
	case 1: return ValidateError(_LS(IDS_CMD0417__Formula_is_invalid_)); 
	case 2: return ValidateError(_LS(IDS_CMD0417__Character_exist_in_the_number_));
	case 3: return ValidateError(_LS(IDS_CMD0417__Out_of_value_range_));
	}
	return FALSE;
}

BOOL CCMMvhlItemStdLaneGrid::ValidateError(LPCTSTR lpszError)
{
	SetWarningText(lpszError);
	return FALSE;
}

BOOL CCMMvhlItemStdLaneGrid::OnEndEditing(ROWCOL nRow, ROWCOL nCol)
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
BOOL CCMMvhlItemStdLaneGrid::SetRightValidCell(ROWCOL nRow, ROWCOL nCol)
{
	ROWCOL i, j;
	i = nRow; j = nCol+1; // 현재 행 다음 열
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


///////////////////////////복사 및 붙여넣기 했을 때 불리는 함수//////////////////////

BOOL CCMMvhlItemStdLaneGrid::CopyRange(const CGXRangeList& selList)
{
	CGXRangeList mSelList;
	CGXRange* mRange;
	POSITION position=selList.GetHeadPosition();

	while(TRUE)
	{
		if(!position||position<0)
			break;
		
		mRange=new CGXRange();
		if(m_bStd)
		{
			if(mRange->IntersectRange(CGXRange(D_SIZE_START_ROW,2,D_SIZE_END_ROW-1,2),selList.GetAt(position)))
			{
				mSelList.AddTail(mRange);
				mRange=new CGXRange();
			}
			if(mRange->IntersectRange(CGXRange(D_SIZE_START_ROW,4,D_SIZE_END_ROW,4),selList.GetAt(position)))
			{
				mSelList.AddTail(mRange);
			}    
			else
				delete mRange;
		}
		else
		{
			if(mRange->IntersectRange(CGXRange(D_SIZE_START_ROW,2,D_SIZE_END_ROW,5),selList.GetAt(position)))
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

BOOL CCMMvhlItemStdLaneGrid::OnPasteFromClipboard(const CGXRange &range)
{
	CGXGridWnd x;
	CGXRange r;
	CGXRange toRange;
	toRange=range;
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

	if(!VerifyPasteRange(r,toRange))
	 return FALSE;

	int fromRows,fromCols,toCols,toRows;
	fromRows=r.bottom-r.top+1;
	fromCols=r.right-r.left+1;
	toRows=GetSelectedRowsCount(toRange.top,toRange.bottom);
	toCols=GetSelectedColsCount(toRange.left,toRange.right);
	int fromi,fromj,toi,toj;
	fromi=1;
	for(toi=toRange.top;toi<=toRange.bottom;toi++)
	{
		fromj=1;
		for(toj=toRange.left;toj<=toRange.right;toj++)
		{
			if((toj==3 || toj==5) && m_bStd)
				continue;

			BOOL bCondition = (toi==D_SIZE_END_ROW && toj==2 && m_bStd);

			if(!bCondition)
			{
				SetValueRange(CGXRange(toi,toj),x.GetValueRowCol(fromi,fromj));
				if(!OnValidateCell(toi,toj))
				{
					DisplayWarningText();
					ShowData();
					return FALSE;
				}
			}
			if(fromCols!=1) fromj++;
		}
		if(fromRows!=1)  fromi++;
	}
	
	SaveData();
	return TRUE;
}

BOOL CCMMvhlItemStdLaneGrid::VerifyPasteRange(CGXRange& from,CGXRange& to)
{
	
	int fromCols,fromRows,toCols,toRows;
	fromCols=from.right-from.left+1;
	fromRows=from.bottom-from.top+1;
	toRows=GetSelectedRowsCount(to.top,to.bottom);
	toCols=GetSelectedColsCount(to.left,to.right);
	
	if(toRows==1&&toCols==1)
	{
		int i;
		for(i=to.left;i<=5;i++)
		{
			if(GetSelectedColsCount(to.left,i)==fromCols)
			{
				to.right=i;
				break;
			}
		}
		if(fromCols!=1&&to.right==to.left)
		{
			MessageBox(_LS(IDS_CMD_PASTE_EXCEED_COLUMN));
			return FALSE;
		}
	
		for(i=to.top;i<=D_SIZE_END_ROW;i++)
		{
			if(GetSelectedRowsCount(to.top,i)==fromRows)
			{
				to.bottom=i;
				break;
			}
		}
		if(fromRows!=1&&to.bottom==to.top)
		{
			MessageBox(_LS(IDS_CMD_PASTE_EXCEED_ROW));
			return FALSE;
		}
		toRows=GetSelectedRowsCount(to.top,to.bottom);
		toCols=GetSelectedColsCount(to.left,to.right);
	}
	if(fromCols==1)
	{
		if(fromRows==1||fromRows==toRows) return TRUE;
	}
	else if(fromRows==1)
	{
		if(fromCols==1||fromCols==toCols)  return TRUE;
	}
	else if(fromRows==toRows&&fromCols==toCols)
		return TRUE;
	MessageBox(_LS(IDS_CMD_PASTE_RANGE_MISMATCH));
	return FALSE;
}

int CCMMvhlItemStdLaneGrid::GetSelectedRowsCount(int top,int bottom)
{
	int toRows;
	toRows=bottom-top+1;
	return toRows;
}

int CCMMvhlItemStdLaneGrid::GetSelectedColsCount(int left,int right)
{
	int toCols;
	toCols=0;
	if(m_bStd)
	{
		if(left<=2&&right>=2)
			toCols++;
		if(left<=4&&right>=4)
			toCols++;
		return toCols;
	}
	else
		return right - left + 1;
}
