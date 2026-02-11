// MeshRatioList.cpp : implementation file
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "wg_treemenu.h"
#include "SeisCvlJpSetInvestMethodTimeGrid.h"

#include "..\wg_db\DBDoc.h"
#include "..\wg_db\UnitCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define  D_COL_CASE 1
#define  D_COL_START 2
#define  D_COL_END 3
#define  D_COL_COUNT 3

/////////////////////////////////////////////////////////////////////////////
// CSeisCvlJpSetInvestMethodTimeGrid
CSeisCvlJpSetInvestMethodTimeGrid::CSeisCvlJpSetInvestMethodTimeGrid()
{
}

CSeisCvlJpSetInvestMethodTimeGrid::~CSeisCvlJpSetInvestMethodTimeGrid()
{
}


BEGIN_MESSAGE_MAP(CSeisCvlJpSetInvestMethodTimeGrid, CTBBrowserWnd)
	//{{AFX_MSG_MAP(CSeisCvlJpSetInvestMethodTimeGrid)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSeisCvlJpSetInvestMethodTimeGrid message handlers

void CSeisCvlJpSetInvestMethodTimeGrid::Initialize()
{
	CTBBrowserWnd::Initialize();

	// cell 바깥영역 배경색 지정 
	GetParam()->GetProperties()->SetColor(GX_COLOR_BACKGROUND, CTBGrid::m_Color.OuterBackground);

	// Header 배경색 지정 
	ChangeColHeaderStyle(CGXStyle().SetInterior(CTBGrid::m_Color.RowColHeaderBg));
	ChangeRowHeaderStyle(CGXStyle().SetInterior(CTBGrid::m_Color.RowColHeaderBg));

	// 기타 Cells 배경색 지정
	ChangeStandardStyle(CGXStyle().SetInterior(CTBGrid::m_Color.NormalModeBg));

	LockUpdate(TRUE);

	SetColCount(D_COL_COUNT);

	// Header
	SetFrozenCols(0, 0);

	// 표준 스타일 변경
	ChangeStandardStyle(CGXStyle()
		.SetFont(CGXFont_GC().SetSize(9))
		.SetVerticalAlignment(DT_VCENTER)
		.SetAllowEnter(FALSE));

	// 컬럼 헤더 스타일 변경
	ChangeColHeaderStyle(CGXStyle()
		.SetFont(CGXFont_GC().SetBold(FALSE)));

	// 컬럼 스타일 변경
	SetStyleRange(CGXRange().SetCols(D_COL_CASE), CGXStyle()
		.SetControl(GX_IDS_CTRL_STATIC)
		.SetEnabled(FALSE));

	SetStyleRange(CGXRange().SetCols(D_COL_START, D_COL_END), CGXStyle()
		.SetValueType(GX_VT_NUMERIC)
		.SetHorizontalAlignment(DT_CENTER));

	EnableDeleteRowByPressingKey(FALSE);
	EnableInsertRowByPressingKey(FALSE);
	EnableAppendRow(FALSE);

	SetRowHeightByDPI(0, 0, 25);

	// undo를 금지시킨다.
	GetParam()->EnableUndo(FALSE);

	// cell의 width와 height 크기 변경을 금지한다.
	GetParam()->EnableTrackRowHeight(FALSE);
	//GetParam()->EnableTrackColWidth(FALSE);

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
	SetScrollBarMode(SB_HORZ, gxnEnabled | gxnEnhanced);

	CGXProperties* pProp = GetParam()->GetProperties();
	pProp->SetMarkColHeader(FALSE); // Turn off pressed button effect for column headers
	pProp->SetMarkRowHeader(FALSE);
	pProp->SetUserProperty(GX_IDS_OUTLINECURRENTCELL, // 현재 셀의 border 설정 변경
		(CGXStyle)pProp->sInvertNoBorder);

	CArray<int, int> aWidth;
	aWidth.SetSize(D_COL_COUNT);
	aWidth[D_COL_CASE - 1] = 100;
	aWidth[D_COL_START - 1] = 40;
	aWidth[D_COL_END - 1] = 40;

	SetColWidthByDPI(0, 0, 0);
	for (int i = 0; i < D_COL_COUNT; i++)
	{
		SetColWidthByDPI(i + 1, i + 1, aWidth[i]);
	}
	SetValueRange(CGXRange(0, D_COL_CASE), _LS(IDS_SEIS_METHOD_TIME_COL_CASE));
	SetValueRange(CGXRange(0, D_COL_START), _LS(IDS_SEIS_METHOD_TIME_COL_START));
	SetValueRange(CGXRange(0, D_COL_END), _LS(IDS_SEIS_METHOD_TIME_COL_END));

	SetRowCount(4);
	SetRowHeightByDPI(1, 3, 25);
	HideRows(4, 4);

	LockUpdate(FALSE);
}

void CSeisCvlJpSetInvestMethodTimeGrid::SetRow(const CArray< CString, CString>& aCase)
{
	for (int i = 0; i < aCase.GetSize(); i++)
	{
		int nRow = i + 1;
		SetValueRange(CGXRange(nRow, D_COL_CASE), aCase[i]);
	}
}

void CSeisCvlJpSetInvestMethodTimeGrid::GetRow(CArray< CString, CString>& aCase, CArray<double, double>& aStart, CArray<double, double>& aEnd)
{
	for (int nRow = 1; nRow < 4; nRow++)
	{
		aCase.Add(GetValueRowCol(nRow, D_COL_CASE));
		int nStart = _ttof(GetValueRowCol(nRow, D_COL_START));
		aStart.Add(nStart);
		int nEnd = _ttof(GetValueRowCol(nRow, D_COL_END));
		aEnd.Add(nEnd);
	}
}

void CSeisCvlJpSetInvestMethodTimeGrid::EnableControl(const BOOL& bEnable)
{
	for(int nRow=1; nRow<=3; nRow++)
	{
		for(int nCol=D_COL_START; nCol<=D_COL_COUNT; nCol++)
		{
			SetStyleRange(CGXRange().SetCells(nRow, nCol), CGXStyle()
				.SetValue(0.0)
				.SetEnabled(bEnable));
		}
	}
}