// CMSectItemPSC3CellJointGrid.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMSectItemPSC3CellJointGrid.h"
#include "CMSectItemPSC3CellGridDlg.h"
#include "CMSectItemPSC3Cell.h"
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

#define D_JOINT_START_ROW 2
#define D_JOINT_END_ROW 8
#define D_JOINT_ROW_SIZE 7

/////////////////////////////////////////////////////////////////////////////
// CCMSectItemPSC3CellJointGrid

CCMSectItemPSC3CellJointGrid::CCMSectItemPSC3CellJointGrid()
{
	m_pSect = 0;
	m_bInitDone = FALSE;
	m_pParent=0;
	m_bOnStartEditing=FALSE;
	m_bRedraw=TRUE;
	m_nInsertNum=0;
}

CCMSectItemPSC3CellJointGrid::~CCMSectItemPSC3CellJointGrid()
{
}
BEGIN_MESSAGE_MAP(CCMSectItemPSC3CellJointGrid, CGXGridWnd)
	//{{AFX_MSG_MAP(CCMSectItemPSC3CellJointGrid)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CCMSectItemPSC3CellJointGrid message handlers

////////////////////////////////외부에서 호출되는 함수////////////////////////
void CCMSectItemPSC3CellJointGrid::InitGrid()
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

void CCMSectItemPSC3CellJointGrid::SetDataSource(T_SECT_D* pSect,CWnd* pParent)
{
	ASSERT(pSect);
	m_pSect = pSect;
	m_pParent=pParent;
	
	m_Cell.Set(*m_pSect,true,true);
	
	SetStyleOnSizeCell();
	
	SetRightValidCell(1, 1);
}

////////////////////////////////그리드내 컨트롤및 스타일을 정의하는 함수//////////////////
void CCMSectItemPSC3CellJointGrid::SetStyleOnSizeCell()
{

	
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	BOOL bOldLock = LockUpdate(TRUE);
	BOOL bOldReadOnlyLock = GetParam()->IsLockReadOnly();
	GetParam()->SetLockReadOnly(FALSE);
	
	SetRowCount(8);
	SetColCount(4);
	SetColWidthByDPI(1, 1, 70);
	SetColWidthByDPI(2, 2, 70);
	SetColWidthByDPI(3, 3, 70);
	SetColWidthByDPI(4, 4, 70);
	SetRowHeight(0, GetRowCount(), globalUtils.ScaleByDPI(20));

	SetStyleRange(CGXRange().SetCols(1, GetColCount()), CGXStyle()
		.SetVerticalAlignment(DT_VCENTER));


	int i;

	for (i = D_JOINT_START_ROW; i <= D_JOINT_END_ROW-1; i++)
	{
		SetStyleRange(CGXRange(i, 2), CGXStyle()
			.SetControl(GX_IDS_CTRL_CHECKBOX3D).SetHorizontalAlignment(DT_CENTER)); 
		SetStyleRange(CGXRange(i, 4), CGXStyle()
			.SetControl(GX_IDS_CTRL_CHECKBOX3D).SetHorizontalAlignment(DT_CENTER)); 
	}
	 SetStyleRange(CGXRange(D_JOINT_END_ROW, 2), CGXStyle()
			.SetControl(GX_IDS_CTRL_CHECKBOX3D).SetHorizontalAlignment(DT_CENTER));

	 CString aSizeTitleLeft[] = {
		_T("JO1"),_T("JI1"),_T("JI3"),_T("JI5"),
			_T("JI7"),_T("JI9"),_T("JI11"),
	};
	CString aSizeTitleRight[] = {
		_T("JO2"),_T("JI2"),_T("JI4"),_T("JI6"),_T("JI8"),_T("JI10"),
	};
	
	SetCoveredCellsRowCol(D_JOINT_START_ROW-1, 1, D_JOINT_START_ROW-1, 4);
	SetStyleRange(CGXRange(D_JOINT_START_ROW-1, 1), CGXStyle()
		.SetInterior(COLORREF(RGB(0, 0, 255)))
		.SetTextColor(COLORREF(RGB(255, 255, 0)))
		.SetValue(_LS(IDS_CMD_SECT_PSC_Joint_OnOff)));

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
		.SetHorizontalAlignment(DT_RIGHT)
		.SetValueType(GX_VT_NUMERIC));
	for(i=D_JOINT_START_ROW;i<=D_JOINT_END_ROW;i++)
	{
		SetValueRange(CGXRange(i,1),aSizeTitleLeft[i-D_JOINT_START_ROW]);
	}
	for(i=D_JOINT_START_ROW;i<=D_JOINT_END_ROW-1;i++)
	{
		SetValueRange(CGXRange(i,3),aSizeTitleRight[i-D_JOINT_START_ROW]);
	}
	SetStyleRange(CGXRange(D_JOINT_END_ROW,4), CGXStyle()
		.SetReadOnly(TRUE)
		.SetEnabled(FALSE)
		.SetInterior(::GetSysColor(COLOR_BTNFACE)));
	
	
	GetParam()->SetLockReadOnly(bOldReadOnlyLock);

	LockUpdate(bOldLock);
	if (!bOldLock) Redraw();
}

/////////////////////////데이터를 그리드에 보여주는 함수////////////////////
void CCMSectItemPSC3CellJointGrid::ShowData()
{
	if(!m_bRedraw)
		return;
	SetValueRange(CGXRange(D_JOINT_START_ROW,2), (short)m_Cell.JO1);
	SetValueRange(CGXRange(D_JOINT_START_ROW,4), (short)m_Cell.JO2);
	SetValueRange(CGXRange(D_JOINT_START_ROW+1,2), (short)m_Cell.JI1);
	SetValueRange(CGXRange(D_JOINT_START_ROW+1,4), (short)m_Cell.JI2);
	SetValueRange(CGXRange(D_JOINT_START_ROW+2,2), (short)m_Cell.JI3);
	SetValueRange(CGXRange(D_JOINT_START_ROW+2,4), (short)m_Cell.JI4);
	SetValueRange(CGXRange(D_JOINT_START_ROW+3,2), (short)m_Cell.JI5);
	SetValueRange(CGXRange(D_JOINT_START_ROW+3,4), (short)m_Cell.JI6);
	SetValueRange(CGXRange(D_JOINT_START_ROW+4,2), (short)m_Cell.JI7);
	SetValueRange(CGXRange(D_JOINT_START_ROW+4,4), (short)m_Cell.JI8);
	SetValueRange(CGXRange(D_JOINT_START_ROW+5,2), (short)m_Cell.JI9);
	SetValueRange(CGXRange(D_JOINT_START_ROW+5,4), (short)m_Cell.JI10);
	SetValueRange(CGXRange(D_JOINT_START_ROW+6,2), (short)m_Cell.JI11);
				
}

///////////////////////////그리드의 데이터를 저장하는 함수////////////////////
BOOL CCMSectItemPSC3CellJointGrid::SaveData()
{
	CString csNum;
	double dNum;

	csNum = GetEditingValue(D_JOINT_START_ROW,2);
	if (GetDoubleValue(csNum, dNum)) 
		m_Cell.JO1 = dNum;
	else m_Cell.JO1 = 0;

	csNum = GetEditingValue(D_JOINT_START_ROW,4);
	if (GetDoubleValue(csNum, dNum)) 
		m_Cell.JO2 = dNum;
	else m_Cell.JO2 = 0;
	
	csNum = GetEditingValue(D_JOINT_START_ROW+1,2);
	if (GetDoubleValue(csNum, dNum)) 
		m_Cell.JI1 = dNum;
	else m_Cell.JI1 = 0;
		
	csNum = GetEditingValue(D_JOINT_START_ROW+1,4);
	if (GetDoubleValue(csNum, dNum)) 
		m_Cell.JI2 = dNum;
	else m_Cell.JI2 = 0;
 
	csNum = GetEditingValue(D_JOINT_START_ROW+2,2);
	if (GetDoubleValue(csNum, dNum)) 
		m_Cell.JI3 = dNum;
	else m_Cell.JI3 = 0;
		
	csNum = GetEditingValue(D_JOINT_START_ROW+2,4);
	if (GetDoubleValue(csNum, dNum)) 
		m_Cell.JI4 = dNum;
	else m_Cell.JI4 = 0;
	 
	csNum = GetEditingValue(D_JOINT_START_ROW+3,2);
	if (GetDoubleValue(csNum, dNum)) 
		m_Cell.JI5 = dNum;
	else m_Cell.JI5 = 0;
		
	csNum = GetEditingValue(D_JOINT_START_ROW+3,4);
	if (GetDoubleValue(csNum, dNum)) 
		m_Cell.JI6 = dNum;
	else m_Cell.JI6 = 0;
	 
	csNum = GetEditingValue(D_JOINT_START_ROW+4,2);
	if (GetDoubleValue(csNum, dNum)) 
		m_Cell.JI7 = dNum;
	else m_Cell.JI7 = 0;
		
	csNum = GetEditingValue(D_JOINT_START_ROW+4,4);
	if (GetDoubleValue(csNum, dNum)) 
		m_Cell.JI8 = dNum;
	else m_Cell.JI8 = 0;
		
	csNum = GetEditingValue(D_JOINT_START_ROW+5,2);
	if (GetDoubleValue(csNum, dNum)) 
		m_Cell.JI9 = dNum;
	else m_Cell.JI9 = 0;

	csNum = GetEditingValue(D_JOINT_START_ROW+5,4);
	if (GetDoubleValue(csNum, dNum)) 
		m_Cell.JI10 = dNum;
	else m_Cell.JI10 = 0;

	csNum = GetEditingValue(D_JOINT_START_ROW+6,2);
	if (GetDoubleValue(csNum, dNum)) 
		m_Cell.JI11 = dNum;
	else m_Cell.JI11 = 0;


	m_Cell.Get(*m_pSect,true,true);
		
	return TRUE;
}
///////////////////////////그리드의 데이터가 바뀌었을 때 그리드 자신이나 외부에 알려주는 함수///////////


void CCMSectItemPSC3CellJointGrid::OnSizeChange()
{
	// 임시(다시 그려주기)
	CCMSectItemPSC3Cell *pParent = (CCMSectItemPSC3Cell*)((CCMSectItemPSC3CellGridDlg*)m_pParent)->m_pWnd;
	if (pParent != 0)
	{
		pParent->SetData2Dlg(); 
		pParent->UpdateCtrls();
		pParent->RedrawSection();
		((CCMSectItemPSC3CellGridDlg*)m_pParent)->DecideEnable();
	} 

}


//////////////////////////화살표를 입력 받았을 때 움직을 위치를 구하는 함수/////////////

BOOL CCMSectItemPSC3CellJointGrid::SetLeftValidCell(ROWCOL nRow, ROWCOL nCol)
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
BOOL CCMSectItemPSC3CellJointGrid::SetRightValidCell(ROWCOL nRow, ROWCOL nCol)
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
BOOL CCMSectItemPSC3CellJointGrid::SetDownValidCell(ROWCOL nRow, ROWCOL nCol)
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
BOOL CCMSectItemPSC3CellJointGrid::SetUpValidCell(ROWCOL nRow, ROWCOL nCol)
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

BOOL CCMSectItemPSC3CellJointGrid::CopyRange(const CGXRangeList& selList)
{
	CGXRangeList mSelList;
	CGXRange* mRange;
	POSITION position=selList.GetHeadPosition();
	while(TRUE)
	{
		if(!position||position<0)
			break;
		mRange=new CGXRange();
		if(mRange->IntersectRange(CGXRange(D_JOINT_START_ROW,2,D_JOINT_END_ROW,2),selList.GetAt(position)))
		{
			mSelList.AddTail(mRange);
			mRange=new CGXRange();
		}
		if(mRange->IntersectRange(CGXRange(D_JOINT_START_ROW,4,D_JOINT_END_ROW-1,4),selList.GetAt(position)))
		{
			mSelList.AddTail(mRange);
		}    
		else
			delete mRange;
		selList.GetNext(position);
	}
 
	return CGXGridWnd::CopyRange(mSelList);
}

BOOL CCMSectItemPSC3CellJointGrid::OnPasteFromClipboard(const CGXRange &range)
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
			if(toj==1||toj==3)
				continue;
			BOOL bCondition=0;
			bCondition=(toi==D_JOINT_END_ROW&&toj==4);
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
	OnSizeChange();
	ShowData();
	return TRUE;
}

BOOL CCMSectItemPSC3CellJointGrid::VerifyPasteRange(CGXRange& from,CGXRange& to)
{
	
	int fromCols,fromRows,toCols,toRows;
	fromCols=from.right-from.left+1;
	fromRows=from.bottom-from.top+1;
	toRows=GetSelectedRowsCount(to.top,to.bottom);
	toCols=GetSelectedColsCount(to.left,to.right);
	
	if(toRows==1&&toCols==1)
	{
		int i;
		for(i=to.left;i<=4;i++)
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
		for(i=to.top;i<=D_JOINT_END_ROW;i++)
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

int CCMSectItemPSC3CellJointGrid::GetSelectedRowsCount(int top,int bottom)
{
	int toRows;
	toRows=bottom-top+1;
	if(top==D_JOINT_START_ROW-1)
		toRows--;
	return toRows;
}

int CCMSectItemPSC3CellJointGrid::GetSelectedColsCount(int left,int right)
{
	int toCols;
	toCols=0;
	if(left<=2&&right>=2)
		toCols++;
	if(left<=4&&right>=4)
		toCols++;
	return toCols;
}

////////////////////////////////////조건에 따라 각 셀을 보여줄지 말지를 결정하는 함수/////////
void CCMSectItemPSC3CellJointGrid::DecideEnable()
{

}


////////////////////////////////////////사용자가 데이터를 입력했을 때 불리는 함수/////////////////
void CCMSectItemPSC3CellJointGrid::OnModifyCell(ROWCOL nRow, ROWCOL nCol)
{
	CGXGridWnd::OnModifyCell(nRow, nCol);  
	SaveData();
	OnSizeChange();
	//ShowData();
}

BOOL CCMSectItemPSC3CellJointGrid::OnStartEditing(ROWCOL nRow, ROWCOL nCol)
{
	if (!CGXGridWnd::OnStartEditing(nRow, nCol)) return FALSE;
	
	if(nRow>=D_JOINT_START_ROW && nRow<=D_JOINT_END_ROW)
	{
		m_bOnStartEditing = TRUE; 
		SaveData();
		OnSizeChange();
		m_bOnStartEditing = FALSE;
	}
	return TRUE;
}
////////////////////////////////////////안 바뀌는 부분//////////////////////////////
BOOL CCMSectItemPSC3CellJointGrid::OnEndEditing(ROWCOL nRow, ROWCOL nCol)
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

CString CCMSectItemPSC3CellJointGrid::GetEditingValue(ROWCOL nRow, ROWCOL nCol)
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

BOOL CCMSectItemPSC3CellJointGrid::ProcessKeys(CWnd *pSender, UINT nMessage, UINT nChar, UINT nRepCnt, UINT flags)
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
BOOL CCMSectItemPSC3CellJointGrid::OnValidateCell(ROWCOL nRow, ROWCOL nCol)
{
	CString csNum = GetEditingValue(nRow, nCol);
	double dNum;
	if (GetDoubleValue(csNum, dNum)) return TRUE;
	return FALSE;
}

BOOL CCMSectItemPSC3CellJointGrid::OnActivateGrid(BOOL bActivate)
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
BOOL CCMSectItemPSC3CellJointGrid::GetDoubleValue(CString& csNum, double& dNum)
{
	int ecode;
	if (!IsValidDoubleNumber(csNum, ecode)) return InvalidDoubleNumber(ecode);
	ConvertFormulaToValue(csNum);
	dNum = _tstof(csNum);
	return TRUE;
}

// nErrCode : 1(수식 에러), 2(숫자 아님), 3(범위 초과)
BOOL CCMSectItemPSC3CellJointGrid::IsValidDoubleNumber(CString value, int &nErrCode)
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
BOOL CCMSectItemPSC3CellJointGrid::ConvertFormulaToValue(CString& value)
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

BOOL CCMSectItemPSC3CellJointGrid::IsNumeric(CString str, int& ecode)
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

BOOL CCMSectItemPSC3CellJointGrid::InvalidDoubleNumber(int nErrCode)
{
	switch(nErrCode)
	{
	case 1: return ValidateError(_LS(IDS_CMD0417__Formula_is_invalid_)); 
	case 2: return ValidateError(_LS(IDS_CMD0417__Character_exist_in_the_number_));
	case 3: return ValidateError(_LS(IDS_CMD0417__Out_of_value_range_));
	}
	return FALSE;
}



BOOL CCMSectItemPSC3CellJointGrid::ValidateError(LPCTSTR lpszError)
{
	SetWarningText(lpszError);
	return FALSE;
}