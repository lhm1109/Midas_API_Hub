// CCMMvhlItemUserLoadGrid.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMMvhlItemUserLoadGrid.h"


#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_SectUtil.h"
#include "..\wg_db\wg_db_SectDB.h"
#include "..\wg_db\wg_db_NewSect.h"
#include "..\wg_db\wg_db_DBDoc.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCMMvhlItemUserLoadGrid

CCMMvhlItemUserLoadGrid::CCMMvhlItemUserLoadGrid()
{
	m_pMvhl = 0;
	m_nIndex = 0;
	m_nFrom = 0;
	m_bInitDone = FALSE;
	m_bEnable =TRUE;
}

CCMMvhlItemUserLoadGrid::~CCMMvhlItemUserLoadGrid()
{
}
void CCMMvhlItemUserLoadGrid::InitGrid()
{
	if (!m_bInitDone) Initialize(); // 그리드를 초기화한다.
	else { ResetGrid(); Initialize(); }
	m_bInitDone = TRUE;

	GetParam()->EnableUndo(FALSE);  // undo를 금지시킨다.

	// cell의 width와 height 크기 변경을 금지한다.
	GetParam()->EnableTrackRowHeight(FALSE);
	//GetParam()->EnableTrackColWidth(FALSE);

	// column과 row의 위치 이동을 금지시킨다.
	GetParam()->EnableMoveCols(FALSE);
	GetParam()->EnableMoveRows(FALSE);

	// double click시 셀 편집이 되도록 한다.
	GetParam()->SetActivateCellFlags(GX_CAFOCUS_DBLCLICKONCELL);
	GetParam()->SetNumberedRowHeaders(TRUE);

	// Text만 카피 되게 한다.
	//EnableOleDataSource(GX_DNDTEXT);
	m_nClipboardFlags &= ~GX_DNDSTYLES;

	// Row 헤더와 Column 헤더를 숨긴다.
 
	// always show vertical scrollbar
	SetScrollBarMode(SB_VERT, gxnEnabled | gxnEnhanced);

	// 표준 스타일 변경
	ChangeStandardStyle(CGXStyle()
			.SetFont(CGXFont_GC().SetSize(10))
			.SetVerticalAlignment(DT_BOTTOM)
			.SetAllowEnter(FALSE));

	LockUpdate(FALSE);
	Redraw();
}

void CCMMvhlItemUserLoadGrid::SetStyleOnSizeCell()
{
	int count;
	int i;
	count=GetLoadSize();
	
	for (i = 1; i <= count; i++)
	{
		SetStyleRange(CGXRange(i,1), CGXStyle()
			.SetInterior(::GetSysColor((m_bEnable)?COLOR_WINDOW:COLOR_BTNFACE))
			.SetFormat(GX_FMT_FIXED).SetPlaces(2));
		SetStyleRange(CGXRange(i,2), CGXStyle()
			.SetInterior(::GetSysColor((m_bEnable)?COLOR_WINDOW:COLOR_BTNFACE))
			.SetEnabled(TRUE)
			.SetFormat(GX_FMT_FIXED).SetPlaces(2));     
		if(count == i)
		{ 
			SetStyleRange(CGXRange(i, 2), CGXStyle()
			.SetInterior(::GetSysColor((m_bEnable)?COLOR_WINDOW:COLOR_BTNFACE))
			.SetEnabled(FALSE)
			.SetFormat(GX_VT_STRING).SetPlaces(2));
			SetValueRange(CGXRange(i,2),_LS(IDS_WG_CMD__ADDD__end));
		}
	}
	
}
void CCMMvhlItemUserLoadGrid::SetDataSource(T_MVHL_D* pMvhl,int nIndex, int nFrom)
{
	ASSERT(pMvhl);
	m_pMvhl = pMvhl;
	m_nIndex = nIndex;
	m_nFrom = nFrom;
	CString strUnit,strTemp;
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	BOOL bOldLock = LockUpdate(TRUE);
	
	BOOL bOldReadOnlyLock = GetParam()->IsLockReadOnly();
	GetParam()->SetLockReadOnly(FALSE);
	int count;
	count=GetLoadSize();
	SetRowCount(count);
	SetColCount(2);

	// set column width
	SetColWidth(0, 0, globalUtils.ScaleByDPI(23));
	SetColWidth(1, 1, globalUtils.ScaleByDPI(50));
	SetColWidth(2, 2, globalUtils.ScaleByDPI(37));
	int nHeight = GetRowHeight(1);
	SetRowHeight(0,0,nHeight*globalUtils.ScaleByDPI(2));
	SetStyleRange(CGXRange(0, 0), CGXStyle()
		.SetHorizontalAlignment(DT_LEFT)
			.SetValue(_LS(IDS_CMD_MVHL_NO)));
 
	pDoc->m_pUnitCtrl->GetUnitSystem(CUnitCtrl::m_MVHL_UNIT.dPointLoad, strUnit); 
	strTemp.Format(_T("%s\n(%s)"),_LS(IDS_CMD_MVHL_P),strUnit);

	SetStyleRange(CGXRange(0, 1), CGXStyle()
		.SetHorizontalAlignment(DT_CENTER)
			.SetValue(strTemp));
	pDoc->m_pUnitCtrl->GetUnitSystem(CUnitCtrl::m_MVHL_UNIT.dPointDistance, strUnit); 
	strTemp.Format(_T("%s\n(%s)"),_LS(IDS_CMD_MVHL_D),strUnit);

	SetStyleRange(CGXRange(0, 2), CGXStyle()
		.SetHorizontalAlignment(DT_CENTER)
			.SetValue(strTemp));
 SetStyleRange(CGXRange().SetCols(0), CGXStyle()
		.SetHorizontalAlignment(DT_RIGHT));
	
	SetStyleRange(CGXRange().SetCols(1), CGXStyle()
		.SetHorizontalAlignment(DT_RIGHT)
		.SetValueType(GX_VT_NUMERIC)
		.SetFormat(GX_FMT_FIXED)
		.SetPlaces(2));
	SetStyleRange(CGXRange().SetCols(2), CGXStyle()
		.SetHorizontalAlignment(DT_RIGHT)
		.SetValueType(GX_VT_NUMERIC)
		.SetFormat(GX_FMT_FIXED)
		.SetPlaces(2));
	
	
	SetStyleOnSizeCell(); 
	SetStyleRange(CGXRange().SetCols(0), CGXStyle()
		.SetHorizontalAlignment(DT_LEFT));

	GetParam()->SetLockReadOnly(bOldReadOnlyLock);

	LockUpdate(bOldLock);
	if (!bOldLock) Redraw();
	ShowData();
	//SetRightValidCell(1, 0);
}

void CCMMvhlItemUserLoadGrid::ShowData()
{
	CString str;
	double* dLoad,*dSpacing;
	dLoad=m_pMvhl->dPointLoad3[m_nIndex];
	dSpacing=m_pMvhl->dPointDistance3[m_nIndex];

	for (int i = 0; i < GetLoadSize(); i++)
	{
		str.Format(_T("%g"),dLoad[i]);
		SetValueRange(CGXRange(i+1, 1),str);
		if(i == m_pMvhl->nNumLoad3[m_nIndex]-1)
			continue;
		str.Format(_T("%g"),dSpacing[i]);
		SetValueRange(CGXRange(i+1, 2),str);
	}
	
}
BOOL CCMMvhlItemUserLoadGrid::SaveData()
{
	CString csNum;
	double dNum;
	double* dLoad,*dSpacing;
	dLoad=m_pMvhl->dPointLoad3[m_nIndex];
	dSpacing=m_pMvhl->dPointDistance3[m_nIndex];
 
	for (int i = 0; i <GetLoadSize(); i++)
	{
		csNum = GetEditingValue(i+1, 1);
		if (GetDoubleValue(csNum, dNum)) 
		{
			dLoad[i] = dNum;
		}
		else
		{
			dLoad[i] = 0.0;
		}
		if(i == m_pMvhl->nNumLoad3[m_nIndex]-1)
		{
			dSpacing[i] = 0.0;
			continue;
		}
			
		csNum = GetEditingValue(i+1, 2);
		if (GetDoubleValue(csNum, dNum)) 
		{
			dSpacing[i] = dNum;
		}
		else
		{
			dSpacing[i] = 0.0;
		}
		
	}
	
	return TRUE;
}

BOOL CCMMvhlItemUserLoadGrid::SetDownValidCell(ROWCOL nRow, ROWCOL nCol)
{
	ROWCOL i,j;
	i=nRow+1;
	j=nCol;
	if (nRow > GetRowCount()) { i=1; }  
	if (nCol == 2 && nRow ==GetRowCount()-1)
	{
		if (SetCurrentCell(nRow, nCol))
		return TRUE;
	}

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
BOOL CCMMvhlItemUserLoadGrid::SetUpValidCell(ROWCOL nRow, ROWCOL nCol)
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

CString CCMMvhlItemUserLoadGrid::GetEditingValue(ROWCOL nRow, ROWCOL nCol)
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




BEGIN_MESSAGE_MAP(CCMMvhlItemUserLoadGrid, CGXGridWnd)
	//{{AFX_MSG_MAP(CCMMvhlItemUserLoadGrid)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CCMMvhlItemUserLoadGrid message handlers
/////////////////////////////////////////////////////////////////////////////

BOOL CCMMvhlItemUserLoadGrid::ProcessKeys(CWnd *pSender, UINT nMessage, UINT nChar, UINT nRepCnt, UINT flags)
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
					if (nCol == GetColCount())
					{
						SetCurrentCell(nRow+1, nCol-1);
						return TRUE;
					} 
				}
				break;
			default:
				break;
			}
//     switch(nChar)
//     {
//     case VK_TAB: 
//       {
//         BOOL bShift = GetKeyState(VK_SHIFT) & 0x8000;
//         if (bShift && SetDownValidCell(nRow, nCol)) break;
//         if (!bShift && SetUpValidCell(nRow,nCol)) break;
// 
//         CWnd* pWnd = GetParent();
//         if (pWnd && pWnd->GetSafeHwnd())
//         {
//           //pWnd->PostMessage(nMessage, nChar, nRepCnt);
//           CWnd* pNextWnd = pWnd->GetNextDlgTabItem(this, bShift);
//           if (pNextWnd && pNextWnd->GetSafeHwnd()) pNextWnd->SetFocus();
//         }
//       }
//       break;
//     case VK_RETURN: case VK_RIGHT:  // 다음 셀로 이동
//       if (nRow < GetRowCount())
//         return SetDownValidCell(nRow, nCol);
//       break;
//     case VK_DOWN:
//       if (nRow < GetRowCount())
//         return SetDownValidCell(nRow, nCol);
//       break;
//     case VK_LEFT:  // 이전 셀로 이동
//       if (nRow > 1)
//       {
//         return SetUpValidCell(nRow, nCol);
//       }
//       break;
//     case VK_UP:
//       if(nRow>1)
//       {
//         return SetUpValidCell(nRow,nCol);
//       }
//       break;
// 
//     }
	}
	return CGXGridWnd::ProcessKeys(pSender, nMessage, nChar, nRepCnt, flags);
}

BOOL CCMMvhlItemUserLoadGrid::OnValidateCell(ROWCOL nRow, ROWCOL nCol)
{
	CString csNum = GetEditingValue(nRow, nCol);
	double dNum;
	if (GetDoubleValue(csNum, dNum)) return TRUE;
	return FALSE;
}

BOOL CCMMvhlItemUserLoadGrid::OnActivateGrid(BOOL bActivate)
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

void CCMMvhlItemUserLoadGrid::OnModifyCell(ROWCOL nRow, ROWCOL nCol)
{
	CGXGridWnd::OnModifyCell(nRow, nCol);
	SaveData();  

}

BOOL CCMMvhlItemUserLoadGrid::GetDoubleValue(CString& csNum, double& dNum)
{
	int ecode;
	if (!IsValidDoubleNumber(csNum, ecode)) return InvalidDoubleNumber(ecode);
	ConvertFormulaToValue(csNum);
	dNum = _tstof(csNum);
	return TRUE;
}

// nErrCode : 1(수식 에러), 2(숫자 아님), 3(범위 초과)
BOOL CCMMvhlItemUserLoadGrid::IsValidDoubleNumber(CString value, int &nErrCode)
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

BOOL CCMMvhlItemUserLoadGrid::ConvertFormulaToValue(CString& value)
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

BOOL CCMMvhlItemUserLoadGrid::IsNumeric(CString str, int& ecode)
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

BOOL CCMMvhlItemUserLoadGrid::InvalidDoubleNumber(int nErrCode)
{
	switch(nErrCode)
	{
	case 1: return ValidateError(_LS(IDS_CMD0417__Formula_is_invalid_)); 
	case 2: return ValidateError(_LS(IDS_CMD0417__Character_exist_in_the_number_));
	case 3: return ValidateError(_LS(IDS_CMD0417__Out_of_value_range_));
	}
	return FALSE;
}

BOOL CCMMvhlItemUserLoadGrid::ValidateError(LPCTSTR lpszError)
{
	SetWarningText(lpszError);
	return FALSE;
}

BOOL CCMMvhlItemUserLoadGrid::OnEndEditing(ROWCOL nRow, ROWCOL nCol)
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
BOOL CCMMvhlItemUserLoadGrid::SetRightValidCell(ROWCOL nRow, ROWCOL nCol)
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
int  CCMMvhlItemUserLoadGrid::GetLoadSize()
{
	 return m_pMvhl->nNumLoad3[m_nIndex];
}
void CCMMvhlItemUserLoadGrid::SetEnable(BOOL bEnable)
{
	m_bEnable = bEnable;
	SetStyleOnSizeCell();
}

///////////////////////////복사 및 붙여넣기 했을 때 불리는 함수//////////////////////

BOOL CCMMvhlItemUserLoadGrid::CopyRange(const CGXRangeList& selList)
{
	CGXRangeList mSelList;
	CGXRange* mRange;
	POSITION position=selList.GetHeadPosition();

	while(TRUE)
	{
		if(!position||position<0)
			break;
		
		mRange=new CGXRange();
		if(mRange->IntersectRange(CGXRange(1,1,GetRowCount(),2),selList.GetAt(position)))
		{
			mSelList.AddTail(mRange);
		}
		else
			delete mRange;
		
		selList.GetNext(position);
	}
 
	return CGXGridWnd::CopyRange(mSelList);
}

BOOL CCMMvhlItemUserLoadGrid::OnPasteFromClipboard(const CGXRange &range)
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
			BOOL bCondition = FALSE;
			bCondition=(toi==GetRowCount() && toj==2);
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

BOOL CCMMvhlItemUserLoadGrid::VerifyPasteRange(CGXRange& from,CGXRange& to)
{
	
	int fromCols,fromRows,toCols,toRows;
	fromCols=from.right-from.left+1;
	fromRows=from.bottom-from.top+1;
	toRows=GetSelectedRowsCount(to.top,to.bottom);
	toCols=GetSelectedColsCount(to.left,to.right);
	
	if(toRows==1&&toCols==1)
	{
		int i;
		for(i=to.left;i<=2;i++)
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
	
		for(i=to.top;i<=GetRowCount();i++)
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

int CCMMvhlItemUserLoadGrid::GetSelectedRowsCount(int top,int bottom)
{
	int toRows;
	toRows=bottom-top+1;
	return toRows;
}

int CCMMvhlItemUserLoadGrid::GetSelectedColsCount(int left,int right)
{
	int toCols;
	toCols= right -left +1;
	return toCols;
}
