// CMWindPulsationModeGrid.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"

#include "CMWindPulsationDlg.h"
#include "CMWindPulsationModeGrid.h"

#include "..\wg_db\wg_db_SectUtil.h"
#include "..\wg_db\wg_db_SectDB.h"
#include "..\wg_db\wg_db_NewSect.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define D_START_ROW 1
#define IsSHIFTpressed() ( (GetKeyState(VK_SHIFT) & (1 << (sizeof(SHORT)*8-1))) != 0 ) 

/////////////////////////////////////////////////////////////////////////////
// CCMWindPulsationModeGrid

CCMWindPulsationModeGrid::CCMWindPulsationModeGrid()
{
	m_pData = 0;
	m_bSelect = TRUE;
	m_bInitDone = FALSE;
	m_nInsertNum=0;
	m_nModeNum=0;
	m_arModePercent.RemoveAll();
}

CCMWindPulsationModeGrid::~CCMWindPulsationModeGrid()
{
}
BEGIN_MESSAGE_MAP(CCMWindPulsationModeGrid, CGXGridWnd)
	//{{AFX_MSG_MAP(CCMWindPulsationModeGrid)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CCMWindPulsationModeGrid message handlers

////////////////////////////////외부에서 호출되는 함수////////////////////////
void CCMWindPulsationModeGrid::InitGrid(CCMWindPulsationDlg* pParent, int nDirection)
{
	m_pParent = pParent;
	m_nDirection = nDirection;
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
	//HideRows(0, 0);
	HideCols(0, 0);

	// always show vertical scrollbar
	SetScrollBarMode(SB_VERT, gxnEnabled | gxnEnhanced);
#ifdef _RUS
	SetScrollBarMode(SB_HORZ, gxnEnabled | gxnEnhanced);
#endif

	ColHeaderStyle()
					.SetReadOnly(TRUE)
					.SetEnabled(FALSE) // Current cell cannot be moved to header
					//.SetControl(IDS_TB_CTRL_BROWSECOLHEADER)
					;

	// 표준 스타일 변경
	ChangeStandardStyle(CGXStyle()
			.SetFont(CGXFont_GC().SetFaceName(_LS(IDS_CMD_FONT_Ms_Sans_Serif)).SetSize(9))
//      .SetFont(CGXFont_GC().SetSize(10))
			.SetVerticalAlignment(DT_BOTTOM)
			.SetAllowEnter(FALSE));
	//#if defined(_JP) || defined(_CH)
	ChangeColHeaderStyle(CGXStyle().SetFont(CGXFont_GC().SetBold(FALSE).SetFaceName(_LS(IDS_CMD_FONT_Ms_Sans_Serif)).SetSize(9)));
	//#endif
	LockUpdate(FALSE);
	Redraw();
}

void CCMWindPulsationModeGrid::SetDataSource(CArray<bool, bool>* pData, CArray<double, double>& arModePercent, BOOL bSelect)
{
	ASSERT(pData);
	m_pData = pData;
	m_arModePercent.Copy(arModePercent);
	m_bSelect = bSelect;
	m_nModeNum=m_pData->GetSize();
	SetStyleOnSizeCell();
	//ShowData();
	SetRightValidCell(1, 2);
}

////////////////////////////////그리드내 컨트롤및 스타일을 정의하는 함수//////////////////
void CCMWindPulsationModeGrid::SetStyleOnSizeCell()
{

	
	BOOL bOldLock = LockUpdate(TRUE);
	BOOL bOldReadOnlyLock = GetParam()->IsLockReadOnly();
	GetParam()->SetLockReadOnly(FALSE);
	
	SetRowCount(m_nModeNum);
	SetColCount(3);
	SetColWidth(1, 1, 40);
#ifdef _RUS
	SetColWidth(2, 2, 60);
#else
	SetColWidth(2, 2, 35);
#endif
	SetColWidth(3, 3, 40);
	

	SetStyleRange(CGXRange().SetCols(1, 2), CGXStyle()
			.SetVerticalAlignment(DT_VCENTER)
			.SetHorizontalAlignment(DT_RIGHT)
			.SetValueType(GX_VT_NUMERIC)
			.SetFormat(GX_FMT_FIXED)
			.SetPlaces(4));
	//SetRowHeight(0, 0, 24);

	SetStyleRange(CGXRange().SetCols(2), CGXStyle()
			.SetControl(GX_IDS_CTRL_CHECKBOX3D)
			.SetHorizontalAlignment(DT_CENTER)
			.SetPlaces(0)
			.SetValue(_T("0")));


	SetStyleRange(CGXRange().SetCols(1), CGXStyle()
		.SetReadOnly(TRUE)
		.SetEnabled(FALSE)
		.SetInterior(::GetSysColor(COLOR_BTNFACE)) 
		.SetValueType(GX_VT_STRING)
		.SetFormat(GX_FMT_TEXT));


	SetStyleRange(CGXRange(0, 1), CGXStyle()
		.SetReadOnly(TRUE)
		.SetEnabled(FALSE)
		.SetInterior(::GetSysColor(COLOR_BTNFACE)) 
		.SetValue(_LS(IDS_WG_CMD__ADDD__Mode)));
	SetStyleRange(CGXRange(0, 2), CGXStyle()
		.SetReadOnly(TRUE)
		.SetEnabled(FALSE)
		.SetInterior(::GetSysColor(COLOR_BTNFACE)) 
		.SetValue(_LS(IDS_WG_CMD__ADDD__Use)));	
	
	CString strMassPer;
	strMassPer.Format(_T("%s(%%)"), _LS(IDS_TB_POSTDISP_MASS));
	SetStyleRange(CGXRange(0, 3), CGXStyle()
		.SetReadOnly(TRUE)
		.SetEnabled(FALSE)
		.SetInterior(::GetSysColor(COLOR_BTNFACE))
		.SetValue(strMassPer));
	
	int i;
	for (i = 0; i < m_nModeNum; i++)
	{
		CString str;
		str.Format(_T("%d"),i+1);
		SetValueRange(CGXRange(i+D_START_ROW,1), str);
		SetValueRange(CGXRange(i+D_START_ROW,2), _T("1"));
		SetValueRange(CGXRange(i + D_START_ROW, 3), _T("1.0"));
	}
	
		
	GetParam()->SetLockReadOnly(bOldReadOnlyLock);

	LockUpdate(bOldLock);
	if (!bOldLock) Redraw();
}

/////////////////////////데이터를 그리드에 보여주는 함수////////////////////
void CCMWindPulsationModeGrid::ShowData()
{
	ROWCOL nRow,nCol;
	GetCurrentCell(&nRow,&nCol);
	BOOL bOldLock = LockUpdate(TRUE);
	BOOL bOldReadOnlyLock = GetParam()->IsLockReadOnly();
	GetParam()->SetLockReadOnly(FALSE);
	double dMassSum = 0.0;
	for(int i=0;i<m_nModeNum;i++)
	{

		BOOL bUseMode=m_pData->GetAt(i);	
		double dMassPer = m_arModePercent[i];

		SetStyleRange(CGXRange(i + D_START_ROW, 3), CGXStyle()
			.SetReadOnly(TRUE)
			.SetEnabled(FALSE)
			.SetInterior(::GetSysColor(COLOR_BTNFACE))
			.SetValue(dMassPer));

		SetStyleRange(CGXRange(i+D_START_ROW, 2), CGXStyle()
				.SetReadOnly(!m_bSelect)
				.SetEnabled(m_bSelect)
				.SetInterior(::GetSysColor((m_bSelect)?COLOR_WINDOW:COLOR_BTNFACE)));
		//if(nRow==i+D_START_ROW&&nCol==2&&m_bOnStartEditing)  continue;
		SetValueRange(CGXRange(i+D_START_ROW,2),((short)bUseMode));
		if(bUseMode) dMassSum += dMassPer;
	}  
	GetParam()->SetLockReadOnly(bOldReadOnlyLock);

	LockUpdate(bOldLock);
	if (!bOldLock) Redraw();

	m_pParent->UpdatePercent(m_nDirection, dMassSum);
}


////////////////y///////////그리드의 데이터를 저장하는 함수////////////////////
BOOL CCMWindPulsationModeGrid::SaveData()
{
	CString csNum;
	double dNum;
	
	for(int i=0;i<m_nModeNum;i++)
	{
		csNum = GetEditingValue(i+D_START_ROW,2);
		if (GetDoubleValue(csNum, dNum)) 
			(*m_pData)[i] = (bool)dNum;
		else (*m_pData)[i] = true;
		csNum = GetEditingValue(i+D_START_ROW,3);
	}
	return TRUE;
}

//////////////////////////화살표를 입력 받았을 때 움직을 위치를 구하는 함수/////////////

BOOL CCMWindPulsationModeGrid::SetLeftValidCell(ROWCOL nRow, ROWCOL nCol)
{
	ROWCOL i, j;
	i = nRow; j = nCol-1; // 현재 행 이전 열
	if (nCol == 0 || j < 1) { i--; j = GetColCount(); }  

	while (i >= 1)
	{
		while (j >= 1)
		{
			// 열이 선택될 수 있는지 조사
			//if (SetCurrentCell(i, j)) return TRUE;
			CGXStyle style;
			ComposeStyleRowCol(i, j, &style);
			if (!style.GetIncludeEnabled() || style.GetEnabled())
			{
				if (SetCurrentCell(i, j)) return TRUE;
			}
			j--;  // 이전 열을 조사
		}
		i--;  // 이전 줄로 이동
		j = GetColCount();
	}

	return FALSE;
}
BOOL CCMWindPulsationModeGrid::SetRightValidCell(ROWCOL nRow, ROWCOL nCol)
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
BOOL CCMWindPulsationModeGrid::SetDownValidCell(ROWCOL nRow, ROWCOL nCol)
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
BOOL CCMWindPulsationModeGrid::SetUpValidCell(ROWCOL nRow, ROWCOL nCol)
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
///////////////////////////복사 및 붙여넣기 했을 때 불리는 함수//////////////////////

BOOL CCMWindPulsationModeGrid::CopyRange(const CGXRangeList& selList)
{
	CGXRangeList mSelList;
	CGXRange* mRange;
	POSITION position=selList.GetHeadPosition();
	while(TRUE)
	{
		if(!position||position<0)
			break;
		mRange=new CGXRange();
		if(mRange->IntersectRange(CGXRange(D_START_ROW,2,D_START_ROW+m_nModeNum-1,3),selList.GetAt(position)))
		{
			mSelList.AddTail(mRange);
		}    
		else
			delete mRange;
		selList.GetNext(position);
	}
	return CGXGridWnd::CopyRange(mSelList);
	
}

BOOL CCMWindPulsationModeGrid::OnPasteFromClipboard(const CGXRange &range)
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
	toCols=GetSelectedColsCount(r.left,r.right);
	int fromi,fromj,toi,toj;
	fromi=1;
	for(toi=toRange.top;toi<=toRange.bottom;toi++)
	{
		fromj=1;
		if(toi==D_START_ROW-1)
			continue;
		for(toj=toRange.left;toj<=toRange.right;toj++)
		{
			if(toj==1)
				continue;
			
			BOOL bCondition=0;
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
	ShowData();
	return TRUE;
}

BOOL CCMWindPulsationModeGrid::VerifyPasteRange(CGXRange& from,CGXRange& to)
{
	
	int fromCols,fromRows,toCols,toRows;
	fromCols=from.right-from.left+1;
	fromRows=from.bottom-from.top+1;
	toRows=GetSelectedRowsCount(to.top,to.bottom);
	toCols=GetSelectedColsCount(to.left,to.right);
	
	if(toRows==1&&toCols==1)
	{
		int i;
		for(i=to.left;i<=3;i++)
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
		for(i=to.top;i<=D_START_ROW+m_nModeNum-1;i++)
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

int CCMWindPulsationModeGrid::GetSelectedRowsCount(int top,int bottom)
{
	int toRows;
	toRows=bottom-top+1;
	if(top<=D_START_ROW-1)
		toRows--;
	return toRows;
}

int CCMWindPulsationModeGrid::GetSelectedColsCount(int left,int right)
{
	int toCols;
	toCols=0;
	if(left<=2&&right>=2)
		toCols++;
	if(left<=3&&right>=3)
		toCols++;

	return toCols;
}

////////////////////////////////////조건에 따라 각 셀을 보여줄지 말지를 결정하는 함수/////////


////////////////////////////////////////사용자가 데이터를 입력했을 때 불리는 함수/////////////////
void CCMWindPulsationModeGrid::OnModifyCell(ROWCOL nRow, ROWCOL nCol)
{
	CGXGridWnd::OnModifyCell(nRow, nCol);  
	SaveData();
	ShowData();
}

BOOL CCMWindPulsationModeGrid::OnStartEditing(ROWCOL nRow, ROWCOL nCol)
{
	if (!CGXGridWnd::OnStartEditing(nRow, nCol)) return FALSE;

	SaveData();
	ShowData();
	
	return TRUE;
}
////////////////////////////////////////안 바뀌는 부분//////////////////////////////
BOOL CCMWindPulsationModeGrid::OnEndEditing(ROWCOL nRow, ROWCOL nCol)
{
	BOOL bModified = GetControl(nRow, nCol)->GetModify();
	BOOL bResult = CGXGridWnd::OnEndEditing(nRow, nCol);
	if (bModified)
	{
		CString value = GetValueRowCol(nRow, nCol);
		if (ConvertFormulaToValue(value)) SetExpressionRowCol(nRow, nCol, value);
	}
	SaveData();
	ShowData();
	return bResult;
}

CString CCMWindPulsationModeGrid::GetEditingValue(ROWCOL nRow, ROWCOL nCol)
{
	CString csNum;
	BOOL bGet = FALSE;
	ROWCOL ncRow, ncCol;
	if (GetCurrentCell(ncRow, ncCol))
	{
		if (ncRow == nRow && ncCol == nCol)
		{
			CGXControl* pControl = GetControl(nRow, nCol);
			const CGXStyle style = LookupStyleRowCol(nRow, nCol);
			if (pControl)
			{
				if (pControl->IsActive())
					bGet = pControl->GetValue(csNum);
				else if (style.GetIncludeControl() && style.GetControl() == GX_IDS_CTRL_CHECKBOX3D)
				{
					bGet = pControl->GetValue(csNum);
				}
			}
		}
	}
	if (!bGet) csNum = GetValueRowCol(nRow, nCol);
	return csNum;
}

BOOL CCMWindPulsationModeGrid::ProcessKeys(CWnd *pSender, UINT nMessage, UINT nChar, UINT nRepCnt, UINT flags)
{
	CRowColArray awRows;
	ROWCOL nRow=1, nCol=1;
	//CGXControl* pControl;

	if (!GetCurrentCell(nRow, nCol)) // if no current cell, default action
		return CGXGridWnd::ProcessKeys(pSender, nMessage, nChar, nRepCnt, flags);

	//pControl = GetControl(nRow, nCol);
	if (nMessage == WM_KEYDOWN)
	{
		if(IsSHIFTpressed()) return CGXGridWnd::ProcessKeys(pSender, nMessage, nChar, nRepCnt, flags);
		switch(nChar)
		{
		case VK_TAB: 
			{
				BOOL bShift = GetKeyState(VK_SHIFT) & 0x8000;
				if (bShift && SetLeftValidCell(nRow, nCol-1)) break;
				if (!bShift && SetRightValidCell(nRow, nCol+1)) break;

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
				return SetRightValidCell(nRow, nCol+1);
			break;
		case VK_DOWN:
			if (nRow < GetRowCount())
				return SetDownValidCell(nRow, nCol);
			break;
		case VK_LEFT:  // 이전 셀로 이동
			if (nRow > 1)
			{
				return SetLeftValidCell(nRow, nCol-1);
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
BOOL CCMWindPulsationModeGrid::OnValidateCell(ROWCOL nRow, ROWCOL nCol)
{
	SaveData();
	ShowData();
	CString csNum = GetEditingValue(nRow, nCol);
	double dNum;
	if (GetDoubleValue(csNum, dNum)) return TRUE;
	return FALSE;
}

BOOL CCMWindPulsationModeGrid::OnActivateGrid(BOOL bActivate)
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
BOOL CCMWindPulsationModeGrid::GetDoubleValue(CString& csNum, double& dNum)
{
	int ecode;
	if (!IsValidDoubleNumber(csNum, ecode)) return InvalidDoubleNumber(ecode);
	ConvertFormulaToValue(csNum);
	dNum = _tstof(csNum);
	return TRUE;
}

// nErrCode : 1(수식 에러), 2(숫자 아님), 3(범위 초과)
BOOL CCMWindPulsationModeGrid::IsValidDoubleNumber(CString value, int &nErrCode)
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
BOOL CCMWindPulsationModeGrid::ConvertFormulaToValue(CString& value)
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

BOOL CCMWindPulsationModeGrid::IsNumeric(CString str, int& ecode)
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

BOOL CCMWindPulsationModeGrid::InvalidDoubleNumber(int nErrCode)
{
	switch(nErrCode)
	{
	case 1: return ValidateError(_LS(IDS_CMD0417__Formula_is_invalid_)); 
	case 2: return ValidateError(_LS(IDS_CMD0417__Character_exist_in_the_number_));
	case 3: return ValidateError(_LS(IDS_CMD0417__Out_of_value_range_));
	}
	return FALSE;
}



BOOL CCMWindPulsationModeGrid::ValidateError(LPCTSTR lpszError)
{
	SetWarningText(lpszError);
	return FALSE;
}