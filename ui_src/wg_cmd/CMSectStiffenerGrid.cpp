// CMSectStiffenerGrid.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMSectStiffenerGrid.h"
#include "CMSectStiffenerDlg.h"

#include "..\wg_common\wg_common_TBCommon.h"
#include "..\wg_common\wg_common_TBGrid.h"

#include "..\wg_db\wg_db_UnitCtrl.h"


#define D_COL_NUM 3

CCMSectStiffenerGrid::CCMSectStiffenerGrid()
{
}

CCMSectStiffenerGrid::~CCMSectStiffenerGrid()
{
}

///////////////////////////////////////////////////////////////////////////////
// User Defined Function
void CCMSectStiffenerGrid::SetData2Grid(CArray<T_STIFF_SHAPE_D, T_STIFF_SHAPE_D&> &aShape, CArray<T_STIFF_PROFILE_D, T_STIFF_PROFILE_D&> &aStiffener, int nNum)
{
	BOOL bOldLock = LockUpdate(TRUE);

	SetRowCount(nNum);

	for(int i=0; i<nNum; i++)
	{
		CStringArray aValue;
		aValue.SetSize(D_COL_NUM);

		aValue[0].Format(_T("%d"), aStiffener[i].bCalc);
		aValue[1].Format(_T("%g"), aStiffener[i].dSpacing);

		if(aShape.GetSize()==0)
		{
			aValue[2] = _T("");
		}
		else
		{
			int nShape = aStiffener[i].nShape;

			if( (nShape == -1) || (nShape > aShape.GetSize()-1) )
			{
				aValue[2] = _T("");
			}
			else
			{
				aValue[2] = aShape[aStiffener[i].nShape].strName;
			}
		}

		SetDataToRow(i+1, aValue);
	}

	SetRowHeight(1, nNum, globalUtils.ScaleByDPI(18));

	LockUpdate(bOldLock);
	if(!bOldLock) Redraw();
}

BOOL CCMSectStiffenerGrid::GetGrid2Data(CArray<T_STIFF_SHAPE_D, T_STIFF_SHAPE_D&> &aShape, CArray<T_STIFF_PROFILE_D, T_STIFF_PROFILE_D&> &aStiffener)
{
	int nCount = GetRowCount();
	if(nCount==0) return TRUE;

	CStringArray aValue;
	aValue.SetSize(D_COL_NUM);
	aStiffener.RemoveAll();

	T_STIFF_PROFILE_D Stiffener; 
	Stiffener.Initialize();

	for(int i=0; i<nCount; ++i)
	{
		Stiffener.Initialize();	
		if(!GetRowToData(i+1, aValue)) return FALSE;

		Stiffener.bCalc = _ttoi(aValue[0]);
		Stiffener.dSpacing = _tstof(aValue[1]);

		for(int j=0; j<aShape.GetSize(); ++j)
		{
			if(aValue[2] == aShape[j].strName)
			{
				Stiffener.nShape = j;
				break;
			}
		}

		if(m_bSetCalc && m_nRow==i+1)
		{
			if(Stiffener.bCalc) Stiffener.bCalc = FALSE;
			else                Stiffener.bCalc = TRUE;
			m_bSetCalc = FALSE;
			m_nRow = 0;
		}

		aStiffener.Add(Stiffener);
	}
	return TRUE;
}

BOOL CCMSectStiffenerGrid::EnableDiableGrid(BOOL bEnable)
{
	int nRowCount = GetRowCount();

	for(int i=0; i<nRowCount; ++i)
	{
		SetStyleRange(CGXRange(i+1, 1), CGXStyle().SetEnabled(bEnable));
		SetStyleRange(CGXRange(i+1, 2), CGXStyle().SetEnabled(bEnable));
		SetStyleRange(CGXRange(i+1, 3), CGXStyle().SetEnabled(bEnable));
	}

	return TRUE;
}

void CCMSectStiffenerGrid::SetDataToRow(ROWCOL nRow, CStringArray& raValue)
{
	int nColCount = GetColCount();
	ASSERT(nColCount == raValue.GetSize());

	for(int i=0; i<nColCount; ++i)
		SetValueRange(CGXRange(nRow, i+1), raValue[i]);
}

BOOL CCMSectStiffenerGrid::GetRowToData(ROWCOL nRow, CStringArray& raValue)
{
	int nColCount = GetColCount();
	ASSERT(nColCount == raValue.GetSize());

	for(int i=0; i<nColCount; ++i)
		raValue[i] = GetEditingValue(nRow, i+1);

	return TRUE;
}

CString CCMSectStiffenerGrid::GetEditingValue(ROWCOL nRow, ROWCOL nCol)
{
	CString strNum(_T(""));
	BOOL bGet = FALSE;
	ROWCOL ncRow, ncCol;
	if(GetCurrentCell(ncRow, ncCol))
	{
		if(ncRow==nRow && ncCol==nCol)
		{
			CGXControl* pControl = GetControl(nRow, nCol);
			if(pControl && pControl->IsActive())
				bGet = pControl->GetValue(strNum);
		}
	}
	if(!bGet) strNum = GetValueRowCol(nRow, nCol);

	return strNum;
}

BEGIN_MESSAGE_MAP(CCMSectStiffenerGrid, CGXGridWnd)
	//{{AFX_MSG_MAP(CCMSectStiffenerGrid)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMSectStiffenerGrid message handlers

void CCMSectStiffenerGrid::Initialize(CCMSectStiffenerDlg *pParent, int nStiffPos)
{
	CGXGridWnd::Initialize();

	m_pParent = pParent;
	m_nStiffPos = nStiffPos;
	
	// cell 바깥영역 배경색 지정 
	GetParam()->GetProperties()->SetColor(GX_COLOR_BACKGROUND, CTBGrid::m_Color.OuterBackground);
	
	// Header 배경색 지정 
	ChangeColHeaderStyle(CGXStyle().SetInterior(CTBGrid::m_Color.RowColHeaderBg)); 
	ChangeRowHeaderStyle(CGXStyle().SetInterior(CTBGrid::m_Color.RowColHeaderBg)); 
	
	// 기타 Cells 배경색 지정
	ChangeStandardStyle(CGXStyle( ).SetInterior(CTBGrid::m_Color.NormalModeBg));

	LockUpdate(TRUE);

	SetColCount(D_COL_NUM);
	
	// Header
	HideCols(0, 0);
	SetFrozenCols(0, 0);

	SetStyleRange(CGXRange().SetCols(1), CGXStyle()
		.SetFont(CGXFont_GC().SetSize(9))
		.SetControl(GX_IDS_CTRL_CHECKBOX3D)
		.SetValueType(GX_VT_NUMERIC)
		.SetHorizontalAlignment(DT_CENTER)
		.SetVerticalAlignment(DT_VCENTER)
		.SetEnabled(TRUE)
		);

	SetStyleRange(CGXRange().SetCols(2), CGXStyle()
		.SetFont(CGXFont_GC().SetSize(9))
		.SetValueType(GX_VT_NUMERIC)
		.SetHorizontalAlignment(DT_RIGHT)
		.SetVerticalAlignment(DT_VCENTER)
		.SetEnabled(TRUE)
		);

	// Initialize 할땐 빈값 채워주고 뒤에서 새로그린다.
	CArray<T_STIFF_SHAPE_D, T_STIFF_SHAPE_D&> aShape; aShape.RemoveAll();
	InitStiffenerCol(aShape);

	SetRowHeight(0, 0, globalUtils.ScaleByDPI(25));

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

	// always show vertical scrollbar
	SetScrollBarMode(SB_VERT, gxnEnabled | gxnEnhanced);

	CGXProperties* pProp = GetParam()->GetProperties();
	pProp->SetMarkColHeader(FALSE); // Turn off pressed button effect for column headers
	pProp->SetMarkRowHeader(FALSE);
	pProp->SetUserProperty(GX_IDS_OUTLINECURRENTCELL, // 현재 셀의 border 설정 변경
												(CGXStyle) pProp->sInvertNoBorder);

	// 표준 스타일 변경
	ChangeColHeaderStyle(CGXStyle().SetFont(CGXFont_GC().SetBold(FALSE)));

	int aWidth[] = {25, 45, 60};
	for (int i=0; i<D_COL_NUM; i++) SetColWidthByDPI(i+1, i+1, aWidth[i]);

	CString csUnit;
	CDBDoc *pDoc = CDBDoc::GetDocPoint();
	pDoc->m_pUnitCtrl->GetUnitSystem(D_UNITSYS_BASE_LENGTH, csUnit);

	SetValueRange(CGXRange(0, 1), _T("C"));
	SetValueRange(CGXRange(0, 2), _T("d (") + csUnit + _T(")"));
	SetValueRange(CGXRange(0, 3), _LS(IDS_CMD_SECT_STIFFENER_TITLE));

	LockUpdate(FALSE);
}

void CCMSectStiffenerGrid::InitStiffenerCol(CArray<T_STIFF_SHAPE_D, T_STIFF_SHAPE_D&> &aShape)
{
	CString csChoiceList;

	int nSize = aShape.GetSize();
	for(int i=0; i<nSize; ++i)
	{
		csChoiceList += aShape[i].strName;
		csChoiceList += _T("\n");
	}
	
	SetStyleRange(CGXRange().SetCols(3), CGXStyle()
		.SetFont(CGXFont_GC().SetSize(9))
		.SetControl(GX_IDS_CTRL_TABBED_COMBOBOX)
		.SetChoiceList(csChoiceList)
		.SetUserAttribute(GX_IDS_UA_TABLIST_KEYCOL, _T("0"))
		.SetUserAttribute(GX_IDS_UA_TABLIST_TEXTCOL, _T("0"))
		.SetUserAttribute(GX_IDS_UA_TABLIST_SHOWALLCOLS, _T("1"))
		.SetValueType(GX_VT_STRING)
		.SetHorizontalAlignment(DT_LEFT)
		.SetValue(_T(""))
		.SetEnabled(TRUE)
		);
}

BOOL CCMSectStiffenerGrid::GetStyleRowCol(ROWCOL nRow, ROWCOL nCol, CGXStyle& style, GXModifyType mt, int nType)
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
			.SetInterior(COLORREF(RGB(192, 220, 192)))
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
			.SetInterior(COLORREF(RGB(192, 192, 192)))
			.SetTextColor(COLORREF(RGB(0, 0, 0)));
		else style                      // Disabled Cell
			.SetInterior(COLORREF(RGB(192, 192, 192)))
			.SetTextColor(COLORREF(RGB(0, 0, 0)));
		
		return TRUE;
	}

	if (bCurrentCell) style
		.SetInterior(COLORREF(RGB(255, 251, 240)))
		.SetTextColor(COLORREF(RGB(0, 0, 0)));
	else if (bCurrentRow) style
		.SetInterior(COLORREF(RGB(192, 220, 192)))
		.SetTextColor(COLORREF(RGB(0, 0, 0)));
	else if (bHeaderCol || bHeaderRow) style    // Enabled Header Col(Only Frozen)
		.SetInterior(COLORREF(RGB(192, 220, 192)))
		.SetTextColor(COLORREF(RGB(0, 0, 0)));

	return TRUE;
}

BOOL CCMSectStiffenerGrid::OnStartEditing(ROWCOL nRow, ROWCOL nCol)
{
	if(nCol == 1)
	{
		m_bSetCalc = TRUE;
		m_nRow = nRow;

		m_pParent->UpdateFromLeftToRight();
		m_pParent->UpdateSectView(m_nStiffPos);

		CGXGridWnd::OnStartEditing(nRow, nCol);
	}

	return TRUE;
}

void CCMSectStiffenerGrid::OnModifyCell(ROWCOL nRow, ROWCOL nCol)
{
	m_pParent->UpdateFromLeftToRight();
	m_pParent->UpdateSectView(m_nStiffPos);
	
	CGXGridWnd::OnModifyCell(nRow, nCol);
}

BOOL CCMSectStiffenerGrid::OnEndEditing(ROWCOL nRow, ROWCOL nCol)
{
	m_pParent->UpdateFromLeftToRight();
	m_pParent->UpdateSectView(m_nStiffPos);
	
	return CGXGridWnd::OnEndEditing(nRow, nCol);
}

// BOOL CCMSectStiffenerGrid::OnLButtonClickedRowCol(ROWCOL nRow, ROWCOL nCol, UINT nFlags, CPoint pt)
// {
//   BOOL bRet = CGXGridWnd::OnLButtonClickedRowCol(nRow, nCol, nFlags, pt);
// 	
// 	m_pParent->UpdateFromLeftToRight();
//   m_pParent->UpdateSectView(m_nStiffPos);
// 	
// 	return bRet;
// }
// 
// BOOL CCMSectStiffenerGrid::OnLButtonDblClkRowCol(ROWCOL nRow, ROWCOL nCol, UINT nFlags, CPoint pt)
// {
//   BOOL bRet = CGXGridWnd::OnLButtonDblClkRowCol(nRow, nCol, nFlags, pt);
// 	
// 	m_pParent->UpdateFromLeftToRight();
//   m_pParent->UpdateSectView(m_nStiffPos);
// 	
// 	return bRet;
// }

BOOL CCMSectStiffenerGrid::ProcessKeys(CWnd *pSender, UINT nMessage, UINT nChar, UINT nRepCnt, UINT flags)
{
	CRowColArray awRows;
	ROWCOL nRow=1, nCol=1;
	
	if(!GetCurrentCell(nRow, nCol)) // if no current cell, default action
		return CGXGridWnd::ProcessKeys(pSender, nMessage, nChar, nRepCnt, flags);
	
	if(nMessage == WM_KEYDOWN)
	{
		switch(nChar)
		{
		case VK_TAB: 
			{
				BOOL bShift = GetKeyState(VK_SHIFT) & 0x8000;
				if( bShift )
				{
					if( SetLeftValidCell(nRow,  nCol)) { goto GT_PROC_RETURN; }
				}

				if(!bShift )
				{
					if( SetRightValidCell(nRow, nCol)) { goto GT_PROC_RETURN; }
				}
			}
			break;
		default: 
			break;
		}
	}

	return CGXGridWnd::ProcessKeys(pSender, nMessage, nChar, nRepCnt, flags); 
	
GT_PROC_RETURN:
	if(nMessage==WM_KEYDOWN && (nChar==VK_TAB || nChar==VK_RETURN || nChar==VK_RIGHT || nChar==VK_LEFT))
	{
		Redraw();    
	}
	
	return TRUE;
}

BOOL CCMSectStiffenerGrid::SetRightValidCell(ROWCOL nRow, ROWCOL nCol)
{

	long i=nRow, j=nCol+1; // 현재 행 다음 열
	int nColCount = GetColCount();
	if(j>nColCount) { ++i; j=1; }
		
	BOOL bCovered;/* = FALSE;*/
	while(i<=GetRowCount())
	{
		bCovered = FALSE;
		
		while(j<=nColCount)
		{
			// 열이 선택될 수 있는지 조사
			CGXStyle style; style.Free();
			ComposeStyleRowCol(i, j, &style);
			
			// Is it a covered cell?
			CGXRange cvr;
			if(!bCovered && GetCoveredCellsRowCol(i, j, cvr))
			{
				if((cvr.top<=i && i<=cvr.bottom) && (cvr.left<=j && j<=cvr.right))
				{
					if (nRow == cvr.top) i = cvr.bottom;
					j = cvr.right+1;
					bCovered = TRUE;
					continue;
				}
			}
			
			if(IsRowHidden(i)) break;
			if(IsColHidden(j)) { ++j; continue; }
			
			BOOL bEnable = (!style.GetIncludeEnabled() || style.GetEnabled());
			BOOL bReadOnly = style.GetReadOnly();
			
			if(bEnable && !bReadOnly)
			{
				if(SetCurrentCell(i, j)) return TRUE;
				return FALSE;
			}
			//bCovered = TRUE;
			bCovered = FALSE;
			
			++j;  // 다음 열을 조사
		}
		if(j>nColCount) { ++i; j=1; }
	}
	return FALSE;
}


BOOL CCMSectStiffenerGrid::SetLeftValidCell(ROWCOL nRow, ROWCOL nCol)
{
	long i=nRow, j=nCol-1; // 현재 행 이전 열
	int nColCount = GetColCount();
	if(nCol==0 || j<1) { --i; j=nColCount; } 

	BOOL bCovered = FALSE;
	while(i>=1)
	{
		bCovered = FALSE;
		while(j>=1)
		{
			// 열이 선택될 수 있는지 조사
			CGXStyle style; style.Free();
			ComposeStyleRowCol(i, j, &style);
			
			// Is it a covered cell?
			CGXRange cvr;
			if(!bCovered && GetCoveredCellsRowCol(i, j, cvr))
			{
				if((cvr.top<=i && i<=cvr.bottom) && (cvr.left<=j && j<=cvr.right))
				{
					i = cvr.bottom;
					j = cvr.left-1;
					bCovered = TRUE;
					continue;
				}
			}
			
			if(IsRowHidden(i)) break;
			if(IsColHidden(j)) { --j; continue; }
			
			BOOL bEnable = !style.GetIncludeEnabled() || style.GetEnabled();
			BOOL bReadOnly = style.GetReadOnly();
			
			if(bEnable && !bReadOnly)
			{
				if(GetCoveredCellsRowCol(i, j, cvr))
				{
					if((cvr.top<=i && i<=cvr.bottom) && (cvr.left<=j && j<=cvr.right))
					{
						j = cvr.left;
					}
				}
				if(SetCurrentCell(i, j)) return TRUE;
				return FALSE;
			}
			bCovered = TRUE;
			--j;  // 이전 열을 조사
		}
		if(nCol==0 || j<1) { --i; j=nColCount; } 
	}
	return FALSE;
}
