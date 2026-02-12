// CMSectItemPSCnCellUpGrid.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMSectItemPSCnCellUpGrid.h"
#include "CMSectItemPSCnCell.h"
#include "CMSectItmePSCView.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_SectUtil.h"
#include "..\wg_db\wg_db_SectDB.h"
#include "..\wg_db\wg_db_NewSect.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCMSectItemPSCnCellUpGrid

CCMSectItemPSCnCellUpGrid::CCMSectItemPSCnCellUpGrid()
{
	m_pSect = 0;
	m_bInitDone = FALSE;
	m_pParent=0;
	m_bOnStartEditing=FALSE;
	m_bRedraw=TRUE;
	m_nInsertNum=0;
}

CCMSectItemPSCnCellUpGrid::~CCMSectItemPSCnCellUpGrid()
{
}


void CCMSectItemPSCnCellUpGrid::InitGrid()
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

void CCMSectItemPSCnCellUpGrid::SetStyleOnSizeCell()
{

		SetStyleRange(CGXRange(2, 3), CGXStyle()
			.SetFormat(GX_FMT_GEN).SetPlaces(2));
		SetStyleRange(CGXRange(3, 3), CGXStyle()
			.SetFormat(GX_FMT_FIXED).SetPlaces(4));
 
}
void CCMSectItemPSCnCellUpGrid::SetDataSource(T_SECT_D* pSect,CWnd* pParent)
{
	ASSERT(pSect);
	m_pSect = pSect;
	m_pParent=pParent;
	
	m_NCell2.Set(*m_pSect,true,true);
	BOOL bOldLock = LockUpdate(TRUE);
	BOOL bOldReadOnlyLock = GetParam()->IsLockReadOnly();
	GetParam()->SetLockReadOnly(FALSE);
		
	SetRowCount(8);
	SetColCount(3);

	// set column width
	SetColWidthByDPI(1, 1, 120);
	SetColWidthByDPI(2, 2, 80);
	SetColWidthByDPI(3, 3, 120);
	SetRowHeight(0, GetRowCount(), globalUtils.ScaleByDPI(20));
	int aSizeUnit=CUnitCtrl::m_SECT_UNIT.SectBefore.SectI.Size; 
	CString csUnit;
	CUnitCtrl::GetUnitSystem(aSizeUnit, csUnit);

	SetStyleRange(CGXRange().SetCols(1, GetColCount()), CGXStyle()
		.SetVerticalAlignment(DT_VCENTER));


	SetCoveredCellsRowCol(1, 1, 1, 2);
	SetValueRange(CGXRange(1,1),_LS(IDS_CMD_PSC_GRID_Symmetry));

	SetStyleRange(CGXRange(1, 3), CGXStyle()
			.SetControl(GX_IDS_CTRL_CHECKBOX3D).SetHorizontalAlignment(DT_CENTER));
	
	SetCoveredCellsRowCol(2, 1, 2, 2);
	SetValueRange(CGXRange(2,1),_LS(IDS_CMD_SECT_PSC_No_Of_Cells));
	SetCoveredCellsRowCol(3, 1, 3, 2);
	SetValueRange(CGXRange(3,1),_LS(IDS_CMD_SECT_PSC_Slab_Width)+CString(_T("  ( "))+csUnit+CString(_T(" )")));

	
	
	

	SetCoveredCellsRowCol(4, 1, 4, 3);
	SetStyleRange(CGXRange(4, 1), CGXStyle()
		.SetInterior(COLORREF(RGB(0, 0, 255)))
		.SetTextColor(COLORREF(RGB(255, 255, 0)))
		.SetValue(_LS(IDS_CMD_SECT_PSC_Joint_OnOff)));

	SetCoveredCellsRowCol(5,1,5,2);
	SetValueRange(CGXRange(5,1),_LS(IDS_CMD_SECT_PSC_JO));
	SetStyleRange(CGXRange(5,3),CGXStyle()
		.SetControl(GX_IDS_CTRL_CHECKBOX3D).SetHorizontalAlignment(DT_CENTER));

	SetCoveredCellsRowCol(6,1,6,2);
	SetValueRange(CGXRange(6,1),_LS(IDS_CMD_SECT_PSC_JI));
	SetStyleRange(CGXRange(6,3),CGXStyle()
		.SetControl(GX_IDS_CTRL_CHECKBOX3D).SetHorizontalAlignment(DT_CENTER));

	SetCoveredCellsRowCol(7, 1, 7, 2);
	SetValueRange(CGXRange(7,1),_LS(IDS_CMD_SECT_PSC_Cell_Type));

	SetCoveredCellsRowCol(8, 1, 8, 2);
			SetValueRange(CGXRange(8,1),_LS(IDS_CMD_TAP_PSC_Side_Hole));
			SetStyleRange(CGXRange(8, 3), CGXStyle()
				.SetControl(GX_IDS_CTRL_CHECKBOX3D).SetHorizontalAlignment(DT_CENTER));


	
	SetStyleRange(CGXRange().SetCols(1), CGXStyle()
		.SetReadOnly(TRUE)
		.SetEnabled(FALSE)
		.SetInterior(::GetSysColor(COLOR_BTNFACE)));
	
	CString csCListCellType;
	csCListCellType.Format(_T("%s\n%s\n"), _LS(IDS_CMD_PSC_GRID_Polygon), _LS(IDS_CMD_PSC_GRID_Circle));
	SetCobxColumn(CGXRange(7,3),csCListCellType);
	SetValueRange(CGXRange(7, 3), m_NCell2.nCellType?_LS(IDS_CMD_PSC_GRID_Circle):_LS(IDS_CMD_PSC_GRID_Polygon));

	SetStyleRange(CGXRange(2,3), CGXStyle()
		.SetHorizontalAlignment(DT_CENTER)
		.SetValueType(GX_VT_NUMERIC));
	SetStyleRange(CGXRange(3,3), CGXStyle()
		.SetHorizontalAlignment(DT_CENTER)
		.SetValueType(GX_VT_NUMERIC));
	

	SetStyleOnSizeCell();
 
 
	GetParam()->SetLockReadOnly(bOldReadOnlyLock);

	LockUpdate(bOldLock);
	if (!bOldLock) Redraw();

	SetRightValidCell(1, 1);
}

void CCMSectItemPSCnCellUpGrid::ShowData()
{
	if(!m_bRedraw)
		return;
	ROWCOL nRow,nCol;
	GetCurrentCell(&nRow,&nCol);
	SetValueRange(CGXRange(1, 3), m_NCell2.bSymmetric?_T("1"):_T("0"));
	if(nRow!=2) SetValueRange(CGXRange(2, 3), (LONG)m_NCell2.nNumberOfCells);
	if(nRow!=3) SetValueRange(CGXRange(3, 3), m_NCell2.dSlabWidth);
	if(nRow!=5) SetValueRange(CGXRange(5, 3), m_NCell2.JO?_T("1"):_T("0"));
	if(nRow!=6) SetValueRange(CGXRange(6, 3), m_NCell2.JI?_T("1"):_T("0"));
	if(nRow!=8) SetValueRange(CGXRange(8,3), m_NCell2.bSmallHole?_T("1"):_T("0"));
	SetValueRange(CGXRange(7, 3), m_NCell2.nCellType?_LS(IDS_CMD_PSC_GRID_Circle):_LS(IDS_CMD_PSC_GRID_Polygon));

	 SetStyleRange(CGXRange(8,3), CGXStyle()
		 .SetEnabled(m_NCell2.nCellType>0).SetInterior(::GetSysColor((m_NCell2.nCellType>0)?COLOR_WINDOW:COLOR_BTNFACE)));
	 SetStyleRange(CGXRange(3,3), CGXStyle()
		 .SetEnabled(m_NCell2.nNumberOfCells>1).SetInterior(::GetSysColor((m_NCell2.nNumberOfCells>1)?COLOR_WINDOW:COLOR_BTNFACE)));


			
}
BOOL CCMSectItemPSCnCellUpGrid::SaveData()
{
	CString csNum;
	double dNum;
	csNum = GetEditingValue(1,3);
	if (GetDoubleValue(csNum, dNum)) 
		m_NCell2.bSymmetric = dNum;
	else m_NCell2.bSymmetric = 0;

	csNum = GetEditingValue(2,3);
	if (GetDoubleValue(csNum, dNum)) 
		m_NCell2.nNumberOfCells = dNum;
	else m_NCell2.nNumberOfCells=0;
	

	csNum = GetEditingValue(3,3);
	if (GetDoubleValue(csNum, dNum)) 
		m_NCell2.dSlabWidth = dNum;
	else m_NCell2.dSlabWidth=0.0;
	

	csNum = GetEditingValue(5,3);
	if (GetDoubleValue(csNum, dNum)) 
		m_NCell2.JO = dNum;
	else m_NCell2.JO = 0;

	csNum = GetEditingValue(6,3);
	if (GetDoubleValue(csNum, dNum)) 
		m_NCell2.JI = dNum;
	else m_NCell2.JI = 0;

	csNum = GetEditingValue(8,3);
	if (GetDoubleValue(csNum, dNum)) 
		m_NCell2.bSmallHole = dNum;
	else m_NCell2.bSmallHole = 0;
 
	m_NCell2.nCellType = 0;
	CString csType = GetEditingValue(7, 3);
	if (csType == _LS(IDS_CMD_PSC_GRID_Polygon)) m_NCell2.nCellType = 0;
	else if (csType == _LS(IDS_CMD_PSC_GRID_Circle)) m_NCell2.nCellType = 1;
	else ASSERT(0);

	m_NCell2.Get(*m_pSect,true,true);
		
	return TRUE;
}

void CCMSectItemPSCnCellUpGrid::OnSizeChange()
{
	// 임시(다시 그려주기)
	CCMSectItemPSCnCell *pParent = (CCMSectItemPSCnCell*)((CCMSectItemPSCnCellGridDlg*)m_pParent)->m_pWnd;
	if (pParent != 0)
	{
		pParent->SetData2Dlg(); 
		m_bRedraw=FALSE;
		((CCMSectItemPSCnCellGridDlg*)m_pParent)->ShowData();
		m_bRedraw=TRUE;
		pParent->RedrawSection();
	} 

}

BOOL CCMSectItemPSCnCellUpGrid::SetLeftValidCell(ROWCOL nRow, ROWCOL nCol)
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
BOOL CCMSectItemPSCnCellUpGrid::SetRightValidCell(ROWCOL nRow, ROWCOL nCol)
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

CString CCMSectItemPSCnCellUpGrid::GetEditingValue(ROWCOL nRow, ROWCOL nCol)
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
					if (m_bOnStartEditing)
					{
						if (csNum == _T("0")) csNum = _T("1");
						else csNum = _T("0");
					}
				}
			}
		}
	}
	if (!bGet) csNum = GetValueRowCol(nRow, nCol);
	return csNum;
}

BEGIN_MESSAGE_MAP(CCMSectItemPSCnCellUpGrid, CGXGridWnd)
	//{{AFX_MSG_MAP(CCMSectItemPSCnCellUpGrid)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CCMSectItemPSCnCellUpGrid message handlers

BOOL CCMSectItemPSCnCellUpGrid::ProcessKeys(CWnd *pSender, UINT nMessage, UINT nChar, UINT nRepCnt, UINT flags)
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
		case VK_RETURN: case VK_RIGHT: case VK_DOWN: // 다음 셀로 이동
			if (nRow < GetRowCount())
				SetRightValidCell(nRow, nCol+1);
			if (nChar == VK_DOWN) return TRUE;
			break;
		case VK_LEFT: case VK_UP: // 이전 셀로 이동
			if (nRow > 1)
				SetLeftValidCell(nRow, nCol-1);
			if (nChar == VK_UP) return TRUE;
			break;
		}
	}
	return CGXGridWnd::ProcessKeys(pSender, nMessage, nChar, nRepCnt, flags);
}

BOOL CCMSectItemPSCnCellUpGrid::OnValidateCell(ROWCOL nRow, ROWCOL nCol)
{
	CString csNum = GetEditingValue(nRow, nCol);
	double dNum;
	if (nRow == 7 && nCol == 3) 
	{
		if (csNum == _LS(IDS_CMD_PSC_GRID_Polygon)||csNum == _LS(IDS_CMD_PSC_GRID_Circle))
			return TRUE;
		return FALSE;
	}
	if (GetDoubleValue(csNum, dNum)) return TRUE;
	return FALSE;
}

BOOL CCMSectItemPSCnCellUpGrid::OnActivateGrid(BOOL bActivate)
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

void CCMSectItemPSCnCellUpGrid::OnModifyCell(ROWCOL nRow, ROWCOL nCol)
{
	CGXGridWnd::OnModifyCell(nRow, nCol);  
	SaveData();
	OnSizeChange();
	ShowData();
	
 
}

BOOL CCMSectItemPSCnCellUpGrid::GetDoubleValue(CString& csNum, double& dNum)
{
	int ecode;
	if (!IsValidDoubleNumber(csNum, ecode)) return InvalidDoubleNumber(ecode);
	ConvertFormulaToValue(csNum);
	dNum = _tstof(csNum);
	return TRUE;
}

// nErrCode : 1(수식 에러), 2(숫자 아님), 3(범위 초과)
BOOL CCMSectItemPSCnCellUpGrid::IsValidDoubleNumber(CString value, int &nErrCode)
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

BOOL CCMSectItemPSCnCellUpGrid::ConvertFormulaToValue(CString& value)
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

BOOL CCMSectItemPSCnCellUpGrid::IsNumeric(CString str, int& ecode)
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

BOOL CCMSectItemPSCnCellUpGrid::InvalidDoubleNumber(int nErrCode)
{
	switch(nErrCode)
	{
	case 1: return ValidateError(_LS(IDS_CMD0417__Formula_is_invalid_)); 
	case 2: return ValidateError(_LS(IDS_CMD0417__Character_exist_in_the_number_));
	case 3: return ValidateError(_LS(IDS_CMD0417__Out_of_value_range_));
	}
	return FALSE;
}

BOOL CCMSectItemPSCnCellUpGrid::ValidateError(LPCTSTR lpszError)
{
	SetWarningText(lpszError);
	return FALSE;
}

BOOL CCMSectItemPSCnCellUpGrid::OnEndEditing(ROWCOL nRow, ROWCOL nCol)
{
	BOOL bModified = GetControl(nRow, nCol)->GetModify();
	BOOL bResult = CGXGridWnd::OnEndEditing(nRow, nCol);
	if (bModified)
	{
		CString value = GetValueRowCol(nRow, nCol);
		if (ConvertFormulaToValue(value)) SetExpressionRowCol(nRow, nCol, value);
	}
	ShowData();
	return bResult;
}

BOOL CCMSectItemPSCnCellUpGrid::OnPasteFromClipboard(const CGXRange &range)
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
		if(toi==4)
			continue;
		for(toj=toRange.left;toj<=toRange.right;toj++)
		{
			if(toj==1||toj==2)
				continue;
			SetValueRange(CGXRange(toi,toj),x.GetValueRowCol(fromi,fromj));
			if(!OnValidateCell(toi,toj))
			{
				DisplayWarningText();
				ShowData();
				return FALSE;
			}
			if(fromCols!=1) fromj++;
		}
		if(fromRows!=1)  fromi++;
		
	}
	
	SaveData();
	OnSizeChange();
	ShowData();
	return TRUE;
}
BOOL CCMSectItemPSCnCellUpGrid::OnStartEditing(ROWCOL nRow, ROWCOL nCol)
{
	if (!CGXGridWnd::OnStartEditing(nRow, nCol)) return FALSE;

	if(nRow==1||nRow==5||nRow==6||nRow==8)
	{
		m_bOnStartEditing = TRUE; 
		SaveData();
		OnSizeChange();


		m_bOnStartEditing = FALSE;
	}

	

	return TRUE;
}
void CCMSectItemPSCnCellUpGrid::SetCobxColumn(CGXRange& rRange, LPCTSTR lpszChoiceList)
{
	SetStyleRange(rRange, CGXStyle()
			.SetControl(GX_IDS_CTRL_TABBED_COMBOBOX)
			.SetChoiceList(lpszChoiceList)
			.SetUserAttribute(GX_IDS_UA_TABLIST_KEYCOL, _T("0"))
			.SetUserAttribute(GX_IDS_UA_TABLIST_TEXTCOL, _T("0"))
			.SetUserAttribute(GX_IDS_UA_TABLIST_SHOWALLCOLS, _T("1"))
			.SetValueType(GX_VT_STRING)
			.SetHorizontalAlignment(DT_CENTER)
			.SetValue(_T(""))
			);
}

BOOL CCMSectItemPSCnCellUpGrid::CopyRange(const CGXRangeList& selList)
{
	CGXRangeList mSelList;
	CGXRange* mRange;
	POSITION position=selList.GetHeadPosition();
	while(TRUE)
	{
		if(!position||position<0)
			break;
		mRange=new CGXRange();
		if(mRange->IntersectRange(CGXRange(1,3,3,3),selList.GetAt(position)))
		{
			mSelList.AddTail(mRange);
			mRange=new CGXRange();
		}
		if(mRange->IntersectRange(CGXRange(5,3,8,3),selList.GetAt(position)))
		{
			mSelList.AddTail(mRange);
		}
		else
			delete mRange;
		selList.GetNext(position);
	}
 
	return CGXGridWnd::CopyRange(mSelList);
}
BOOL CCMSectItemPSCnCellUpGrid::VerifyPasteRange(CGXRange& from,CGXRange& to)
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
		for(i=to.top;i<=8;i++)
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
int CCMSectItemPSCnCellUpGrid::GetSelectedColsCount(int left,int right)
{
	int toCols;
	toCols=0;
	if(left<=3&&right>=3)
		toCols++;
	return toCols;
}
int CCMSectItemPSCnCellUpGrid::GetSelectedRowsCount(int top,int bottom)
{
	int toRows;
	toRows=bottom-top+1;
	if(top<=4&&bottom>=4)
		toRows--;
	return toRows;
}