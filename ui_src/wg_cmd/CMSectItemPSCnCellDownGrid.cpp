// CMSectItemPSCnCellDownGrid.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMSectItemPSCnCellDownGrid.h"



#include "CMSectItemPSCnCell.h"
#include "CMSectItmePSCView.h"

#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_SectUtil.h"
#include "..\wg_db\wg_db_SectDB.h"
#include "..\wg_db\wg_db_NewSect.h"


#define D_TOTAL_ROW_NUM 17
#define D_OUTER_START_ROW 2
#define D_INNER_START_ROW  (D_OUTER_START_ROW+D_OUTER_ROW_SIZE+1)
#define D_OUTER_ROW_SIZE 6
#define D_INNER_ROW_SIZE 9

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCMSectItemPSCnCellDownGrid

CCMSectItemPSCnCellDownGrid::CCMSectItemPSCnCellDownGrid()
{
	m_pSect = 0;
	m_bInitDone = FALSE;
	m_nLeftRight=0;
	m_pParent=0;
}

CCMSectItemPSCnCellDownGrid::~CCMSectItemPSCnCellDownGrid()
{
}
void CCMSectItemPSCnCellDownGrid::InitGrid()
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
	SetScrollBarMode(SB_VERT, gxnEnabled | gxnEnhanced);

	// 표준 스타일 변경
	ChangeStandardStyle(CGXStyle()
			.SetFont(CGXFont_GC().SetSize(10))
			.SetVerticalAlignment(DT_BOTTOM)
			.SetAllowEnter(FALSE));

	LockUpdate(FALSE);
	Redraw();
}

void CCMSectItemPSCnCellDownGrid::SetStyleOnSizeCell()
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	int i;

	for (i = 0; i < D_OUTER_ROW_SIZE; i++)
	{
		SetStyleRange(CGXRange(D_OUTER_START_ROW+i, 2), CGXStyle()
			.SetFormat(GX_FMT_FIXED).SetPlaces(4));
		SetStyleRange(CGXRange(D_OUTER_START_ROW+i, 5), CGXStyle()
			.SetFormat(GX_FMT_FIXED).SetPlaces(4));
	}

	
	for (i = 0; i < D_INNER_ROW_SIZE; i++)
	{
		SetStyleRange(CGXRange(D_INNER_START_ROW+i, 2), CGXStyle()
			.SetFormat(GX_FMT_FIXED).SetPlaces(4));
		SetStyleRange(CGXRange(D_INNER_START_ROW+i, 5), CGXStyle()
			.SetFormat(GX_FMT_FIXED).SetPlaces(4));
	}

}
void CCMSectItemPSCnCellDownGrid::SetDataSource(T_SECT_D* pSect,CWnd* pParent)
{
	ASSERT(pSect);
	m_pSect = pSect;
	m_pParent=pParent;
	
	m_pnCell2.Set(*m_pSect,true,true);
	BOOL bOldLock = LockUpdate(TRUE);
	BOOL bOldReadOnlyLock = GetParam()->IsLockReadOnly();
	GetParam()->SetLockReadOnly(FALSE);
		
	SetRowCount(D_TOTAL_ROW_NUM);
	SetColCount(6);

	// set column width
	SetColWidthByDPI(1, 1, 50);
	SetColWidthByDPI(2, 2, 60);
	SetColWidthByDPI(3, 3, 50);
	SetColWidthByDPI(4, 4, 50);
	SetColWidthByDPI(5, 5, 60);
	SetColWidthByDPI(6, 6, 50);
	SetRowHeight(0, GetRowCount(), globalUtils.ScaleByDPI(20));

	SetStyleRange(CGXRange().SetCols(1, GetColCount()), CGXStyle()
		.SetVerticalAlignment(DT_VCENTER));


	SetCoveredCellsRowCol(D_OUTER_START_ROW-1, 1, D_OUTER_START_ROW-1, 6);
	SetStyleRange(CGXRange(D_OUTER_START_ROW-1, 1), CGXStyle()
		.SetInterior(COLORREF(RGB(0, 0, 255)))
		.SetTextColor(COLORREF(RGB(255, 255, 0)))
		.SetValue(_LS(IDS_CMD0417__Outer)));
	SetCoveredCellsRowCol(D_INNER_START_ROW-1, 1, D_INNER_START_ROW-1, 6);
	SetStyleRange(CGXRange(D_INNER_START_ROW-1, 1), CGXStyle()
		.SetInterior(COLORREF(RGB(0, 0, 255)))
		.SetTextColor(COLORREF(RGB(255, 255, 0)))
		.SetValue(_LS(IDS_CMD0417__Inner)));
	SetStyleRange(CGXRange().SetCols(1), CGXStyle()
		.SetReadOnly(TRUE)
		.SetEnabled(FALSE)
		.SetInterior(::GetSysColor(COLOR_BTNFACE)));
	SetStyleRange(CGXRange().SetCols(2), CGXStyle()
		.SetHorizontalAlignment(DT_RIGHT)
		.SetValueType(GX_VT_NUMERIC));
	SetStyleRange(CGXRange().SetCols(3), CGXStyle()
		.SetReadOnly(TRUE)
		.SetEnabled(FALSE)
		.SetInterior(::GetSysColor(COLOR_BTNFACE)));
	SetStyleRange(CGXRange().SetCols(4), CGXStyle()
		.SetReadOnly(TRUE)
		.SetEnabled(FALSE)
		.SetInterior(::GetSysColor(COLOR_BTNFACE)));
	SetStyleRange(CGXRange().SetCols(5), CGXStyle()
		.SetHorizontalAlignment(DT_RIGHT)
		.SetValueType(GX_VT_NUMERIC));
	SetStyleRange(CGXRange().SetCols(6), CGXStyle()
		.SetReadOnly(TRUE)
		.SetEnabled(FALSE)
		.SetInterior(::GetSysColor(COLOR_BTNFACE)));

	SetStyleOnSizeCell();
 
	CString aSizeTitle[] = {
		_T("HO1"),_T("HO2"),_T("HO2-1"),_T("HO3"),_T("HO4"),_T("BO1"),_T("BO1-1"),_T("BO2"),_T("BO3"),_T("BO4"),_T("BO5"),_T("HI1"),_T("HI2"),_T("HI2-1"),
			_T("HI3"),_T("HI4"),_T("HI5"),_T("HI6"),_T("R1"),_T("R2"),_T("BI1"),_T("BI2"),_T("BI2-1"),_T("BI3"),_T("BI4"),_T("BI5"),_T("BI6"),_T("BI7"),_T("BI8")
	};
	int aSizeUnit=CUnitCtrl::m_SECT_UNIT.SectBefore.SectI.Size;
	
	CString csUnit;
	CUnitCtrl::GetUnitSystem(aSizeUnit, csUnit);
	for (int i = 0; i < D_OUTER_ROW_SIZE-1; i++)
	{
		SetValueRange(CGXRange(D_OUTER_START_ROW+i, 1), aSizeTitle[i]);
		SetValueRange(CGXRange(D_OUTER_START_ROW+i, 3), csUnit);
	}
	for (int i = 0; i < D_OUTER_ROW_SIZE; i++)
	{
		SetValueRange(CGXRange(D_OUTER_START_ROW+i, 4), aSizeTitle[D_OUTER_ROW_SIZE-1+i]);
		SetValueRange(CGXRange(D_OUTER_START_ROW+i, 6), csUnit);
	}
	for (int i = 0; i < D_INNER_ROW_SIZE; i++)
	{
		SetValueRange(CGXRange(D_INNER_START_ROW+i, 1), aSizeTitle[2*D_OUTER_ROW_SIZE-1+i]);
		SetValueRange(CGXRange(D_INNER_START_ROW+i, 3), csUnit);
		SetValueRange(CGXRange(D_INNER_START_ROW+i, 4), aSizeTitle[2*D_OUTER_ROW_SIZE-1+D_INNER_ROW_SIZE+i]);
		SetValueRange(CGXRange(D_INNER_START_ROW+i, 6), csUnit);
	}

	GetParam()->SetLockReadOnly(bOldReadOnlyLock);

	LockUpdate(bOldLock);
	if (!bOldLock) Redraw();

	SetRightValidCell(1, 1);
}

void CCMSectItemPSCnCellDownGrid::ShowData()
{
	ChangeStatus();
	double pSize[D_SECT_SIZE_NUM_MAX];
	m_pnCell2.GetSize(pSize);
	int i,plusNum;
	plusNum=m_nLeftRight*29;
	for (i = 0; i < D_OUTER_ROW_SIZE-1; i++)
		SetValueRange(CGXRange(D_OUTER_START_ROW+i, 2), pSize[i+1+plusNum]);
	for (i = 0; i < D_OUTER_ROW_SIZE; i++)
		SetValueRange(CGXRange(D_OUTER_START_ROW+i, 5), pSize[D_OUTER_ROW_SIZE-1+i+1+plusNum]);
	for (i = 0; i < D_INNER_ROW_SIZE-2; i++)
	{
		SetValueRange(CGXRange(D_INNER_START_ROW+i, 2), pSize[D_OUTER_ROW_SIZE*2-1+i+1+plusNum]);
		SetValueRange(CGXRange(D_INNER_START_ROW+i, 5), pSize[D_OUTER_ROW_SIZE*2-1+D_INNER_ROW_SIZE-2+i+1+plusNum]);
	}
	SetValueRange(CGXRange(D_INNER_START_ROW+D_INNER_ROW_SIZE-2, 2), pSize[28+plusNum]);
	SetValueRange(CGXRange(D_INNER_START_ROW+D_INNER_ROW_SIZE-1, 2), pSize[29+plusNum]);
	SetValueRange(CGXRange(D_INNER_START_ROW+D_INNER_ROW_SIZE-2, 5), pSize[26]);
	SetValueRange(CGXRange(D_INNER_START_ROW+D_INNER_ROW_SIZE-1, 5), pSize[27]);
	

	
}
BOOL CCMSectItemPSCnCellDownGrid::SaveData()
{
	CString csNum;
	double dNum;

	double pSize[D_SECT_SIZE_NUM_MAX];
	m_pnCell2.GetSize(pSize);
	int i,toRight,withRight;
	toRight=m_nLeftRight*29;
	if(!m_pnCell2.bSymmetric&&toRight==0)
	{
		withRight=0;
	}
	else
	{
		withRight=29;
	}
	for (i = 0; i < D_OUTER_ROW_SIZE-1; i++)
	{
		csNum = GetEditingValue(D_OUTER_START_ROW+i, 2);
		if (GetDoubleValue(csNum, dNum)) 
		{
			pSize[i+1+toRight] = dNum;
			pSize[i+1+withRight] = dNum;
		}
		else
		{
			pSize[i+1+toRight] = 0.0;
			pSize[i+1+withRight] = 0.0;
		}
	}
	for (i = 0; i < D_OUTER_ROW_SIZE; i++)
	{
		csNum = GetEditingValue(D_OUTER_START_ROW+i, 5);
		if (GetDoubleValue(csNum, dNum)) 
		{
			pSize[D_OUTER_ROW_SIZE-1+i+1+toRight] = dNum;
			pSize[D_OUTER_ROW_SIZE-1+i+1+withRight] = dNum;
		}
		else
		{
			pSize[D_OUTER_ROW_SIZE-1+i+1+toRight] = 0.0;
			pSize[D_OUTER_ROW_SIZE-1+i+1+withRight] = 0.0;
		}
	}
	for (i = 0; i < D_INNER_ROW_SIZE-2; i++)
	{
		csNum = GetEditingValue(D_INNER_START_ROW+i, 2);
		if (GetDoubleValue(csNum, dNum)) 
		{
			pSize[D_OUTER_ROW_SIZE*2-1+i+1+toRight] = dNum;
			pSize[D_OUTER_ROW_SIZE*2-1+i+1+withRight] = dNum;
		}
		else
		{
			pSize[D_OUTER_ROW_SIZE*2-1+i+1+toRight] = 0.0;
			pSize[D_OUTER_ROW_SIZE*2-1+i+1+withRight] = 0.0;
		}
		csNum = GetEditingValue(D_INNER_START_ROW+i, 5);
		if (GetDoubleValue(csNum, dNum)) 
		{
			pSize[D_OUTER_ROW_SIZE*2-1+D_INNER_ROW_SIZE-2+i+1+toRight] = dNum;
			pSize[D_OUTER_ROW_SIZE*2-1+D_INNER_ROW_SIZE-2+i+1+withRight] = dNum;
		}
		else
		{
			pSize[D_OUTER_ROW_SIZE*2-1+D_INNER_ROW_SIZE-2+i+1+toRight] = 0.0;
			pSize[D_OUTER_ROW_SIZE*2-1+D_INNER_ROW_SIZE-2+i+1+withRight] = 0.0;
		}
	
	}
	

	csNum = GetEditingValue(D_TOTAL_ROW_NUM-1, 2);
	if (GetDoubleValue(csNum, dNum)) 
	{
		pSize[28+toRight] = dNum;
		pSize[28+withRight] = dNum;
	}
	else
	{
		pSize[28+toRight] = 0.0;
		pSize[28+withRight] = 0.0;
	}
	csNum = GetEditingValue(D_TOTAL_ROW_NUM, 2);
	if (GetDoubleValue(csNum, dNum)) 
	{
		pSize[29+toRight] = dNum;
		pSize[29+withRight] = dNum;
	}
	else
	{
		pSize[29+toRight] = 0.0;
		pSize[29+withRight] = 0.0;
	}
	
	csNum = GetEditingValue(D_TOTAL_ROW_NUM-1, 5);
	if (GetDoubleValue(csNum, dNum)) 
		pSize[26] = dNum;
	else pSize[26] = 0.0;
	
	csNum = GetEditingValue(D_TOTAL_ROW_NUM, 5);
	if (GetDoubleValue(csNum, dNum)) 
		pSize[27] = dNum;
	else pSize[27] = 0.0;
	m_pnCell2.SetSize(pSize);
	m_pnCell2.Get(*m_pSect,true,true);
	
	return TRUE;
}

void CCMSectItemPSCnCellDownGrid::OnSizeChange()
{
	// 임시(다시 그려주기)
	CCMSectItemPSCnCell *pParent = (CCMSectItemPSCnCell*)((CCMSectItemPSCnCellGridDlg*)m_pParent)->m_pWnd;
	if (pParent != 0)
	{
		pParent->SetData2Dlg();
		
		pParent->RedrawSection();
	} 
}

BOOL CCMSectItemPSCnCellDownGrid::SetLeftValidCell(ROWCOL nRow, ROWCOL nCol)
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
BOOL CCMSectItemPSCnCellDownGrid::SetRightValidCell(ROWCOL nRow, ROWCOL nCol)
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
BOOL CCMSectItemPSCnCellDownGrid::SetDownValidCell(ROWCOL nRow, ROWCOL nCol)
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
BOOL CCMSectItemPSCnCellDownGrid::SetUpValidCell(ROWCOL nRow, ROWCOL nCol)
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

CString CCMSectItemPSCnCellDownGrid::GetEditingValue(ROWCOL nRow, ROWCOL nCol)
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




BEGIN_MESSAGE_MAP(CCMSectItemPSCnCellDownGrid, CGXGridWnd)
	//{{AFX_MSG_MAP(CCMSectItemPSCnCellDownGrid)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CCMSectItemPSCnCellDownGrid message handlers
/////////////////////////////////////////////////////////////////////////////

BOOL CCMSectItemPSCnCellDownGrid::ProcessKeys(CWnd *pSender, UINT nMessage, UINT nChar, UINT nRepCnt, UINT flags)
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

BOOL CCMSectItemPSCnCellDownGrid::OnValidateCell(ROWCOL nRow, ROWCOL nCol)
{
	CString csNum = GetEditingValue(nRow, nCol);
	double dNum;
	if (GetDoubleValue(csNum, dNum)) return TRUE;
	return FALSE;
}

BOOL CCMSectItemPSCnCellDownGrid::OnActivateGrid(BOOL bActivate)
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

void CCMSectItemPSCnCellDownGrid::OnModifyCell(ROWCOL nRow, ROWCOL nCol)
{
	CGXGridWnd::OnModifyCell(nRow, nCol);


	CString csNum;
	double dNum;
	double pSize[D_SECT_SIZE_NUM_MAX];
	m_pnCell2.GetSize(pSize);
	
	csNum = GetEditingValue(nRow, nCol);
	if (!GetDoubleValue(csNum, dNum)) dNum = 0.0;
	int innerPlus,rightPlus,index;

	if(nRow<D_INNER_START_ROW)
	{
		innerPlus=0;
		rightPlus=(nCol<5)?0:D_OUTER_ROW_SIZE-1;
		index=nRow-D_OUTER_START_ROW+1+innerPlus+rightPlus;
	}
	else
	{
		innerPlus=2*D_OUTER_ROW_SIZE-1;
		rightPlus=(nCol<5)?0:D_INNER_ROW_SIZE-2;
		index=nRow-D_INNER_START_ROW+1+innerPlus+rightPlus;
	}
	if(nRow==D_TOTAL_ROW_NUM-1)
	{
		index=(nCol<5)?28:26;
	}
	else if(nRow==D_TOTAL_ROW_NUM)
	{
		index=(nCol<5)?29:27;
	}
	if(m_pnCell2.bSymmetric)
	{
		pSize[index]=dNum;
		pSize[index+29]=dNum;
	}
	else
	{
		if(m_nLeftRight)
			index+=29;
		pSize[index]=dNum;
	}
	
	pSize[index]=dNum;
	
	m_pnCell2.SetSize(pSize);
	m_pnCell2.Get(*m_pSect,true,true);
	OnSizeChange();

}

BOOL CCMSectItemPSCnCellDownGrid::GetDoubleValue(CString& csNum, double& dNum)
{
	int ecode;
	if (!IsValidDoubleNumber(csNum, ecode)) return InvalidDoubleNumber(ecode);
	ConvertFormulaToValue(csNum);
	dNum = _tstof(csNum);
	return TRUE;
}

// nErrCode : 1(수식 에러), 2(숫자 아님), 3(범위 초과)
BOOL CCMSectItemPSCnCellDownGrid::IsValidDoubleNumber(CString value, int &nErrCode)
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

BOOL CCMSectItemPSCnCellDownGrid::ConvertFormulaToValue(CString& value)
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

BOOL CCMSectItemPSCnCellDownGrid::IsNumeric(CString str, int& ecode)
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

BOOL CCMSectItemPSCnCellDownGrid::InvalidDoubleNumber(int nErrCode)
{
	switch(nErrCode)
	{
	case 1: return ValidateError(_LS(IDS_CMD0417__Formula_is_invalid_)); 
	case 2: return ValidateError(_LS(IDS_CMD0417__Character_exist_in_the_number_));
	case 3: return ValidateError(_LS(IDS_CMD0417__Out_of_value_range_));
	}
	return FALSE;
}

BOOL CCMSectItemPSCnCellDownGrid::ValidateError(LPCTSTR lpszError)
{
	SetWarningText(lpszError);
	return FALSE;
}

BOOL CCMSectItemPSCnCellDownGrid::OnEndEditing(ROWCOL nRow, ROWCOL nCol)
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

BOOL CCMSectItemPSCnCellDownGrid::OnPasteFromClipboard(const CGXRange &range)
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
		if(toi==D_OUTER_START_ROW-1||toi==D_INNER_START_ROW-1)
			continue;
		for(toj=toRange.left;toj<=toRange.right;toj++)
		{
			if(toj==1||toj==3||toj==4||toj==6)
				continue;
			if(toi!=D_OUTER_START_ROW+D_OUTER_ROW_SIZE-1||toj!=2)
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
	OnSizeChange();
	return TRUE;
}

void CCMSectItemPSCnCellDownGrid::ChangeStatusLeft(BOOL bSym, int nCellType, int CellNum, int nJoint, BOOL bSideHole)
{
	BOOL bJO = (nJoint & 0x0001) ? TRUE : FALSE;
	BOOL bJI = (nJoint & 0x0002) ? TRUE : FALSE;

	SetStyleRange(CGXRange(D_OUTER_START_ROW+2,2), CGXStyle()
			.SetEnabled(bJO).SetInterior(::GetSysColor((bJO)?COLOR_WINDOW:COLOR_BTNFACE)));
	SetStyleRange(CGXRange(D_OUTER_START_ROW+1,5), CGXStyle()
			.SetEnabled(bJO).SetInterior(::GetSysColor((bJO)?COLOR_WINDOW:COLOR_BTNFACE)));
 
	if (nCellType == 0)   // polygon
	{
		SetStyleRange(CGXRange(D_INNER_START_ROW+2,2), CGXStyle()
			.SetEnabled(bJI).SetInterior(::GetSysColor((bJI)?COLOR_WINDOW:COLOR_BTNFACE)));
		SetStyleRange(CGXRange(D_INNER_START_ROW+2,5), CGXStyle()
			.SetEnabled(bJI).SetInterior(::GetSysColor((bJI)?COLOR_WINDOW:COLOR_BTNFACE)));
		SetStyleRange(CGXRange(D_INNER_START_ROW+3,2), CGXStyle()
			.SetEnabled(CellNum > 1).SetInterior(::GetSysColor((CellNum>1)?COLOR_WINDOW:COLOR_BTNFACE)));
		SetStyleRange(CGXRange(D_INNER_START_ROW+4,2), CGXStyle()
			.SetEnabled(CellNum > 1).SetInterior(::GetSysColor((CellNum>1)?COLOR_WINDOW:COLOR_BTNFACE)));
		SetStyleRange(CGXRange(D_INNER_START_ROW+5,2), CGXStyle()
			.SetEnabled(TRUE).SetInterior(::GetSysColor(COLOR_WINDOW)));
		SetStyleRange(CGXRange(D_INNER_START_ROW+6,2), CGXStyle()
			.SetEnabled(TRUE).SetInterior(::GetSysColor(COLOR_WINDOW)));
		SetStyleRange(CGXRange(D_INNER_START_ROW,5), CGXStyle()
			.SetEnabled(TRUE).SetInterior(::GetSysColor(COLOR_WINDOW)));
		SetStyleRange(CGXRange(D_INNER_START_ROW+1,5), CGXStyle()
			.SetEnabled(TRUE).SetInterior(::GetSysColor(COLOR_WINDOW)));
		SetStyleRange(CGXRange(D_INNER_START_ROW+3,5), CGXStyle()
			.SetEnabled(CellNum > 1).SetInterior(::GetSysColor((CellNum>1)?COLOR_WINDOW:COLOR_BTNFACE)));
		SetStyleRange(CGXRange(D_INNER_START_ROW+4,5), CGXStyle()
			.SetEnabled(CellNum > 1).SetInterior(::GetSysColor((CellNum>1)?COLOR_WINDOW:COLOR_BTNFACE)));
		SetStyleRange(CGXRange(D_INNER_START_ROW+5,5), CGXStyle()
			.SetEnabled(TRUE).SetInterior(::GetSysColor(COLOR_WINDOW)));
		SetStyleRange(CGXRange(D_INNER_START_ROW+6,5), CGXStyle()
			.SetEnabled(CellNum > 1).SetInterior(::GetSysColor((CellNum>1)?COLOR_WINDOW:COLOR_BTNFACE)));
		SetStyleRange(CGXRange(D_INNER_START_ROW+7,5), CGXStyle()
			.SetEnabled(CellNum > 2).SetInterior(::GetSysColor((CellNum>2)?COLOR_WINDOW:COLOR_BTNFACE)));
		SetStyleRange(CGXRange(D_INNER_START_ROW+8,5), CGXStyle()
			.SetEnabled(CellNum > 2).SetInterior(::GetSysColor((CellNum>2)?COLOR_WINDOW:COLOR_BTNFACE)));
		
	
 }
	else if (nCellType == 1)    // circle
	{

		SetStyleRange(CGXRange(D_INNER_START_ROW+2,2), CGXStyle()
			.SetEnabled(FALSE).SetInterior(::GetSysColor(COLOR_BTNFACE)));
		SetStyleRange(CGXRange(D_INNER_START_ROW+2,5), CGXStyle()
			.SetEnabled(FALSE).SetInterior(::GetSysColor(COLOR_BTNFACE)));
		SetStyleRange(CGXRange(D_INNER_START_ROW+3,2), CGXStyle()
			.SetEnabled(bSideHole).SetInterior(::GetSysColor((bSideHole)?COLOR_WINDOW:COLOR_BTNFACE)));
		SetStyleRange(CGXRange(D_INNER_START_ROW+4,2), CGXStyle()
			.SetEnabled(FALSE).SetInterior(::GetSysColor(COLOR_BTNFACE)));
		SetStyleRange(CGXRange(D_INNER_START_ROW+5,2), CGXStyle()
			.SetEnabled(FALSE).SetInterior(::GetSysColor(COLOR_BTNFACE)));
		SetStyleRange(CGXRange(D_INNER_START_ROW+6,2), CGXStyle()
			.SetEnabled(FALSE).SetInterior(::GetSysColor(COLOR_BTNFACE)));
		SetStyleRange(CGXRange(D_INNER_START_ROW,5), CGXStyle()
			.SetEnabled(bSideHole).SetInterior(::GetSysColor((bSideHole)?COLOR_WINDOW:COLOR_BTNFACE)));
		SetStyleRange(CGXRange(D_INNER_START_ROW+1,5), CGXStyle()
			.SetEnabled(bSideHole).SetInterior(::GetSysColor((bSideHole)?COLOR_WINDOW:COLOR_BTNFACE)));
		SetStyleRange(CGXRange(D_INNER_START_ROW+3,5), CGXStyle()
			.SetEnabled(FALSE).SetInterior(::GetSysColor(COLOR_BTNFACE)));
		SetStyleRange(CGXRange(D_INNER_START_ROW+4,5), CGXStyle()
			.SetEnabled(FALSE).SetInterior(::GetSysColor(COLOR_BTNFACE)));
		SetStyleRange(CGXRange(D_INNER_START_ROW+5,5), CGXStyle()
			.SetEnabled(FALSE).SetInterior(::GetSysColor(COLOR_BTNFACE)));
		SetStyleRange(CGXRange(D_INNER_START_ROW+6,5), CGXStyle()
			.SetEnabled(FALSE).SetInterior(::GetSysColor(COLOR_BTNFACE)));
		SetStyleRange(CGXRange(D_INNER_START_ROW+7,5), CGXStyle()
			.SetEnabled(FALSE).SetInterior(::GetSysColor(COLOR_BTNFACE)));
		SetStyleRange(CGXRange(D_INNER_START_ROW+8,5), CGXStyle()
			.SetEnabled(FALSE).SetInterior(::GetSysColor(COLOR_BTNFACE)));
	
	}
	else ASSERT(0);
}

void CCMSectItemPSCnCellDownGrid::ChangeStatusRight(BOOL bSym, int nCellType, int CellNum, int nJoint, BOOL bSideHole)
{
	// if it's current tab
	int i;

	for(i=0;i<D_OUTER_ROW_SIZE;i++)
	{
		
		SetStyleRange(CGXRange(D_OUTER_START_ROW+i,5), CGXStyle()
			.SetEnabled(!bSym).SetInterior(::GetSysColor((bSym)?COLOR_BTNFACE:COLOR_WINDOW)));
		if(i==D_OUTER_ROW_SIZE-1)
			continue;
		SetStyleRange(CGXRange(D_OUTER_START_ROW+i,2), CGXStyle()
			.SetEnabled(!bSym).SetInterior(::GetSysColor((bSym)?COLOR_BTNFACE:COLOR_WINDOW)));
	}
	for(i=0;i<D_INNER_ROW_SIZE;i++)
	{
		SetStyleRange(CGXRange(D_INNER_START_ROW+i,2), CGXStyle()
			.SetEnabled(!bSym).SetInterior(::GetSysColor((bSym)?COLOR_BTNFACE:COLOR_WINDOW)));
		SetStyleRange(CGXRange(D_INNER_START_ROW+i,5), CGXStyle()
			.SetEnabled(!bSym).SetInterior(::GetSysColor((bSym)?COLOR_BTNFACE:COLOR_WINDOW)));
	}


	if (bSym) return;
	BOOL bJO = (nJoint & 0x0001) ? TRUE : FALSE;
	BOOL bJI = (nJoint & 0x0002) ? TRUE : FALSE;

	SetStyleRange(CGXRange(D_OUTER_START_ROW+2,2), CGXStyle()
			.SetEnabled(bJO).SetInterior(::GetSysColor((bJO)?COLOR_WINDOW:COLOR_BTNFACE)));
	SetStyleRange(CGXRange(D_OUTER_START_ROW+1,5), CGXStyle()
			.SetEnabled(bJO).SetInterior(::GetSysColor((bJO)?COLOR_WINDOW:COLOR_BTNFACE)));
	

	if (nCellType == 0)   // polygon
	{
		SetStyleRange(CGXRange(D_INNER_START_ROW+2,2), CGXStyle()
			.SetEnabled(bJI).SetInterior(::GetSysColor((bJI)?COLOR_WINDOW:COLOR_BTNFACE)));
		SetStyleRange(CGXRange(D_INNER_START_ROW+2,5), CGXStyle()
			.SetEnabled(bJI).SetInterior(::GetSysColor((bJI)?COLOR_WINDOW:COLOR_BTNFACE)));
		SetStyleRange(CGXRange(D_INNER_START_ROW+3,2), CGXStyle()
			.SetEnabled(FALSE).SetInterior(::GetSysColor(COLOR_BTNFACE)));
		SetStyleRange(CGXRange(D_INNER_START_ROW+4,2), CGXStyle()
			.SetEnabled(FALSE).SetInterior(::GetSysColor(COLOR_BTNFACE)));
		SetStyleRange(CGXRange(D_INNER_START_ROW+5,2), CGXStyle()
			.SetEnabled(TRUE).SetInterior(::GetSysColor(COLOR_WINDOW)));
		SetStyleRange(CGXRange(D_INNER_START_ROW+6,2), CGXStyle()
			.SetEnabled(FALSE).SetInterior(::GetSysColor(COLOR_BTNFACE)));
		SetStyleRange(CGXRange(D_INNER_START_ROW,5), CGXStyle()
			.SetEnabled(TRUE).SetInterior(::GetSysColor(COLOR_WINDOW)));
		SetStyleRange(CGXRange(D_INNER_START_ROW+1,5), CGXStyle()
			.SetEnabled(TRUE).SetInterior(::GetSysColor(COLOR_WINDOW)));
		SetStyleRange(CGXRange(D_INNER_START_ROW+3,5), CGXStyle()
			.SetEnabled(CellNum>1).SetInterior(::GetSysColor((CellNum>1)?COLOR_WINDOW:COLOR_BTNFACE)));
		SetStyleRange(CGXRange(D_INNER_START_ROW+4,5), CGXStyle()
			.SetEnabled(CellNum>1).SetInterior(::GetSysColor((CellNum>1)?COLOR_WINDOW:COLOR_BTNFACE)));
		SetStyleRange(CGXRange(D_INNER_START_ROW+5,5), CGXStyle()
			.SetEnabled(TRUE).SetInterior(::GetSysColor(COLOR_WINDOW)));
		SetStyleRange(CGXRange(D_INNER_START_ROW+6,5), CGXStyle()
			.SetEnabled(FALSE).SetInterior(::GetSysColor(COLOR_BTNFACE)));
		SetStyleRange(CGXRange(D_INNER_START_ROW+7,5), CGXStyle()
			.SetEnabled(FALSE).SetInterior(::GetSysColor(COLOR_BTNFACE)));
		SetStyleRange(CGXRange(D_INNER_START_ROW+8,5), CGXStyle()
			.SetEnabled(FALSE).SetInterior(::GetSysColor(COLOR_BTNFACE)));
		
		SetStyleRange(CGXRange(D_OUTER_START_ROW+4,2), CGXStyle()
			.SetEnabled(FALSE).SetInterior(::GetSysColor(COLOR_BTNFACE)));
		SetStyleRange(CGXRange(D_INNER_START_ROW,2), CGXStyle()
			.SetEnabled(FALSE).SetInterior(::GetSysColor(COLOR_BTNFACE)));
	 
 }
	else if (nCellType == 1)    // circle
	{
		SetStyleRange(CGXRange(D_INNER_START_ROW+2,2), CGXStyle()
			.SetEnabled(FALSE).SetInterior(::GetSysColor(COLOR_BTNFACE)));
		SetStyleRange(CGXRange(D_INNER_START_ROW+2,5), CGXStyle()
			.SetEnabled(FALSE).SetInterior(::GetSysColor(COLOR_BTNFACE)));
		SetStyleRange(CGXRange(D_INNER_START_ROW+3,2), CGXStyle()
			.SetEnabled(bSideHole).SetInterior(::GetSysColor((bSideHole)?COLOR_WINDOW:COLOR_BTNFACE)));
		SetStyleRange(CGXRange(D_INNER_START_ROW+4,2), CGXStyle()
			.SetEnabled(FALSE).SetInterior(::GetSysColor(COLOR_BTNFACE)));
		SetStyleRange(CGXRange(D_INNER_START_ROW+5,2), CGXStyle()
			.SetEnabled(FALSE).SetInterior(::GetSysColor(COLOR_BTNFACE)));
		SetStyleRange(CGXRange(D_INNER_START_ROW+6,2), CGXStyle()
			.SetEnabled(FALSE).SetInterior(::GetSysColor(COLOR_BTNFACE)));
		SetStyleRange(CGXRange(D_INNER_START_ROW,5), CGXStyle()
			.SetEnabled(bSideHole).SetInterior(::GetSysColor((bSideHole)?COLOR_WINDOW:COLOR_BTNFACE)));
		SetStyleRange(CGXRange(D_INNER_START_ROW+1,5), CGXStyle()
			.SetEnabled(bSideHole).SetInterior(::GetSysColor((bSideHole)?COLOR_WINDOW:COLOR_BTNFACE)));
		SetStyleRange(CGXRange(D_INNER_START_ROW+3,5), CGXStyle()
			.SetEnabled(FALSE).SetInterior(::GetSysColor(COLOR_BTNFACE)));
		SetStyleRange(CGXRange(D_INNER_START_ROW+4,5), CGXStyle()
			.SetEnabled(FALSE).SetInterior(::GetSysColor(COLOR_BTNFACE)));
		SetStyleRange(CGXRange(D_INNER_START_ROW+5,5), CGXStyle()
			.SetEnabled(FALSE).SetInterior(::GetSysColor(COLOR_BTNFACE)));
		SetStyleRange(CGXRange(D_INNER_START_ROW+6,5), CGXStyle()
			.SetEnabled(FALSE).SetInterior(::GetSysColor(COLOR_BTNFACE)));
		SetStyleRange(CGXRange(D_INNER_START_ROW+7,5), CGXStyle()
			.SetEnabled(FALSE).SetInterior(::GetSysColor(COLOR_BTNFACE)));
		SetStyleRange(CGXRange(D_INNER_START_ROW+8,5), CGXStyle()
			.SetEnabled(FALSE).SetInterior(::GetSysColor(COLOR_BTNFACE)));

		SetStyleRange(CGXRange(D_INNER_START_ROW,2), CGXStyle()
			.SetEnabled(FALSE).SetInterior(::GetSysColor(COLOR_BTNFACE)));
		SetStyleRange(CGXRange(D_INNER_START_ROW+1,2), CGXStyle()
			.SetEnabled(FALSE).SetInterior(::GetSysColor(COLOR_BTNFACE)));
		SetStyleRange(CGXRange(D_OUTER_START_ROW+4,2), CGXStyle()
			.SetEnabled(FALSE).SetInterior(::GetSysColor(COLOR_BTNFACE)));
	}
	else ASSERT(0);
}
void CCMSectItemPSCnCellDownGrid::ChangeStatus()
{
	m_pnCell2.Set(*m_pSect,true,true);
	m_pnCell2.Get(*m_pSect,true,true);
	int nJoint=0;
	int nJO=0;
	int nJI=0;
	nJO=(m_pnCell2.JO)?0x0001:0x0000;
	nJI=(m_pnCell2.JI)?0x0002:0x0000;
	nJoint=nJO|nJI;
	StatusReset();

	
	if(m_nLeftRight)
	{
		ChangeStatusRight(m_pnCell2.bSymmetric,m_pnCell2.nCellType,m_pnCell2.nNumberOfCells,nJoint,m_pnCell2.bSmallHole);
	}
	else
	{
		ChangeStatusLeft(m_pnCell2.bSymmetric,m_pnCell2.nCellType,m_pnCell2.nNumberOfCells,nJoint,m_pnCell2.bSmallHole);  
	}
	SetStyleRange(CGXRange(D_OUTER_START_ROW+5,2), CGXStyle()
			.SetEnabled(FALSE).SetInterior(::GetSysColor(COLOR_BTNFACE)));

}

void CCMSectItemPSCnCellDownGrid::StatusReset()
{
	int i;
	for(i=0;i<D_OUTER_ROW_SIZE;i++)
	{
		
		SetStyleRange(CGXRange(D_OUTER_START_ROW+i,5), CGXStyle()
			.SetEnabled(TRUE).SetInterior(::GetSysColor(COLOR_WINDOW)));
		if(i==D_OUTER_ROW_SIZE-1)
			continue;
		SetStyleRange(CGXRange(D_OUTER_START_ROW+i,2), CGXStyle()
			.SetEnabled(TRUE).SetInterior(::GetSysColor(COLOR_WINDOW)));
	}
	for(i=0;i<D_INNER_ROW_SIZE;i++)
	{
		SetStyleRange(CGXRange(D_INNER_START_ROW+i,2), CGXStyle()
			.SetEnabled(TRUE).SetInterior(::GetSysColor(COLOR_WINDOW)));
		SetStyleRange(CGXRange(D_INNER_START_ROW+i,5), CGXStyle()
			.SetEnabled(TRUE).SetInterior(::GetSysColor(COLOR_WINDOW)));
	}
}

BOOL CCMSectItemPSCnCellDownGrid::CopyRange(const CGXRangeList& selList)
{
	CGXRangeList mSelList;
	CGXRange* mRange;
	POSITION position=selList.GetHeadPosition();
	while(TRUE)
	{
		if(!position||position<0)
			break;
		mRange=new CGXRange();
		if(mRange->IntersectRange(CGXRange(D_OUTER_START_ROW,2,D_INNER_START_ROW-2,2),selList.GetAt(position)))
		{
			mSelList.AddTail(mRange);
			mRange=new CGXRange();
		}
		if(mRange->IntersectRange(CGXRange(D_OUTER_START_ROW,5,D_INNER_START_ROW-2,5),selList.GetAt(position)))
		{
			mSelList.AddTail(mRange);
			mRange=new CGXRange();
		}
		if(mRange->IntersectRange(CGXRange(D_INNER_START_ROW,2,D_TOTAL_ROW_NUM,2),selList.GetAt(position)))
		{
			mSelList.AddTail(mRange);
			mRange=new CGXRange();
		}
		if(mRange->IntersectRange(CGXRange(D_INNER_START_ROW,5,D_TOTAL_ROW_NUM,5),selList.GetAt(position)))
			mSelList.AddTail(mRange);
		else
			delete mRange;
		selList.GetNext(position);
	}
 
	return CGXGridWnd::CopyRange(mSelList);
}
BOOL CCMSectItemPSCnCellDownGrid::VerifyPasteRange(CGXRange& from,CGXRange& to)
{
	
	int fromCols,fromRows,toCols,toRows;
	fromCols=from.right-from.left+1;
	fromRows=from.bottom-from.top+1;
	toRows=GetSelectedRowsCount(to.top,to.bottom);
	toCols=GetSelectedColsCount(to.left,to.right);
	
	if(toRows==1&&toCols==1)
	{
		int i;
		for(i=to.left;i<=6;i++)
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
		for(i=to.top;i<=D_TOTAL_ROW_NUM;i++)
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
int CCMSectItemPSCnCellDownGrid::GetSelectedColsCount(int left,int right)
{
	int toCols;
	toCols=0;
	if(left<=2&&right>=2)
		toCols++;
	if(left<=5&&right>=5)
		toCols++;
	return toCols;
}
int CCMSectItemPSCnCellDownGrid::GetSelectedRowsCount(int top,int bottom)
{
	int toRows;
	toRows=bottom-top+1;
	if(top==D_OUTER_START_ROW-1)
		toRows--;
	if(top<=D_INNER_START_ROW-1&&bottom>=D_INNER_START_ROW-1)
		toRows--;
	return toRows;
}