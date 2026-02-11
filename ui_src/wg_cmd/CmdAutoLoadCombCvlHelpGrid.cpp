// CmdAutoLoadCombCvlHelpGrid.cpp: implementation of the CCmdAutoLoadCombCvlHelpGrid class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "wg_cmd.h"
#include "CmdAutoLoadCombCvlHelpGrid.h"


#include "..\wg_common\TBGrid.h"

#include "CmdAutoLoadCombCvlHelpDlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCmdAutoLoadCombCvlHelpGrid::CCmdAutoLoadCombCvlHelpGrid(CCmdAutoLoadCombCvlHelpDlg* pParent)
{
	m_pParent = NULL;
	ASSERT(pParent);
	m_pParent = pParent;

	m_bOnEditing = FALSE;
}

CCmdAutoLoadCombCvlHelpGrid::~CCmdAutoLoadCombCvlHelpGrid()
{
}

#define D_COL_NUM 4

///////////////////////////////////////////////////////////////////////////////
//
// User Defined Function

void CCmdAutoLoadCombCvlHelpGrid::SetDataToRow(ROWCOL nRow, CStringArray& raValue)
{
	int nColCount = GetColCount();
	ASSERT(nColCount == raValue.GetSize());

	for (int i=1; i<=nColCount; i++)
		SetValueRange(CGXRange(nRow, i), raValue[i-1]);
}

BOOL CCmdAutoLoadCombCvlHelpGrid::GetRowToData(ROWCOL nRow, CStringArray& raValue)
{
	int nColCount = GetColCount();
	ASSERT(nColCount == raValue.GetSize());

	for (int i=1; i<=nColCount; i++)
		raValue[i-1] = GetEditingValue(nRow, i);

	return TRUE;
}

CString CCmdAutoLoadCombCvlHelpGrid::GetEditingValue(ROWCOL nRow, ROWCOL nCol)
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
				else if (style.GetIncludeControl())
				{
					bGet = pControl->GetValue(csNum);
					if(m_bOnEditing)
					{
						if (csNum == _T("0")) csNum = _T("1");
						else csNum = _T("0");
	}}}}}

	if (!bGet) csNum = GetValueRowCol(nRow, nCol);
	return csNum;
}

void CCmdAutoLoadCombCvlHelpGrid::SetHeaderTitle()
{ 
	int aWidth[] = {0, 50, 546, 50, 50};
	for (int i=0; i<D_COL_NUM; i++) SetColWidth(i, i, globalUtils.ScaleByDPI(aWidth[i]));

	SetValueRange(CGXRange(0, 0), _T(""));
	SetValueRange(CGXRange(0, 1), _LS(IDS_CMD_LOADCOMB_CVL_HELP_GRID_INDEX));
	SetValueRange(CGXRange(0, 2), _LS(IDS_CMD_LOADCOMB_CVL_HELP_GRID_BRIDGE_COMPONENT));
	SetValueRange(CGXRange(0, 3), _LS(IDS_CMD_LOADCOMB_CVL_HELP_GRID_PS));
	SetValueRange(CGXRange(0, 4), _LS(IDS_CMD_LOADCOMB_CVL_HELP_GRID_CRSH));
}

#define CWnd CGXGridWnd
BEGIN_MESSAGE_MAP(CCmdAutoLoadCombCvlHelpGrid, CWnd)
#undef CWnd
	//{{AFX_MSG_MAP(CCmdAutoLoadCombCvlHelpGrid)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCmdAutoLoadCombCvlHelpGrid message handlers

// 그리드 초기화 
void CCmdAutoLoadCombCvlHelpGrid::Initialize()
{
	CGXGridWnd::Initialize();
	
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

	// Index
	SetStyleRange(CGXRange().SetCols(1), CGXStyle()
		.SetFont(CGXFont_GC().SetSize(9))
		.SetHorizontalAlignment(DT_CENTER)
		.SetVerticalAlignment(DT_VCENTER)
		.SetEnabled(FALSE)
		);

	// Bridge Component
	SetStyleRange(CGXRange().SetCols(2), CGXStyle()
		.SetFont(CGXFont_GC().SetSize(9))
		.SetHorizontalAlignment(DT_LEFT)
		.SetVerticalAlignment(DT_VCENTER)
		.SetEnabled(FALSE)
		);

	// PS
	SetStyleRange(CGXRange().SetCols(3), CGXStyle()
		.SetFont(CGXFont_GC().SetSize(9))
		.SetValueType(GX_VT_NUMERIC)
		.SetHorizontalAlignment(DT_CENTER)
		.SetVerticalAlignment(DT_VCENTER)
		.SetEnabled(FALSE)
		);

	// CR, SH
	SetStyleRange(CGXRange().SetCols(4), CGXStyle()
		.SetFont(CGXFont_GC().SetSize(9))
		.SetValueType(GX_VT_NUMERIC)
		.SetHorizontalAlignment(DT_CENTER)
		.SetVerticalAlignment(DT_VCENTER)
		.SetEnabled(FALSE)
		);

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

	CGXProperties* pProp = GetParam()->GetProperties();
	pProp->SetMarkColHeader(FALSE); // Turn off pressed button effect for column headers
	pProp->SetMarkRowHeader(FALSE);
	pProp->SetUserProperty(GX_IDS_OUTLINECURRENTCELL, // 현재 셀의 border 설정 변경
												(CGXStyle) pProp->sInvertNoBorder);

	// 표준 스타일 변경
	ChangeColHeaderStyle(CGXStyle().SetFont(CGXFont_GC().SetBold(TRUE)));

	SetHeaderTitle();

	LockUpdate(FALSE);
}

BOOL CCmdAutoLoadCombCvlHelpGrid::GetStyleRowCol(ROWCOL nRow, ROWCOL nCol, CGXStyle& style, GXModifyType mt, int nType)
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

void CCmdAutoLoadCombCvlHelpGrid::OnModifyCell(ROWCOL nRow, ROWCOL nCol)
{
	CGXGridWnd::OnModifyCell(nRow, nCol);
}

BOOL CCmdAutoLoadCombCvlHelpGrid::OnEndEditing(ROWCOL nRow, ROWCOL nCol)
{
	return CGXGridWnd::OnEndEditing(nRow, nCol);
}

void CCmdAutoLoadCombCvlHelpGrid::SetText()
{
	SetRowCount(5);

	CString aIndex[] = {_T("A"), _T("B"), _T("C"), _T("D"), _T("E")};
	CString aBridgeComponent[] = {_LS(IDS_CMD_LOADCOMB_CVL_HELP_GRID_BRIDGE_COMPONENT_A),
																_LS(IDS_CMD_LOADCOMB_CVL_HELP_GRID_BRIDGE_COMPONENT_B),
																_LS(IDS_CMD_LOADCOMB_CVL_HELP_GRID_BRIDGE_COMPONENT_C),
																_LS(IDS_CMD_LOADCOMB_CVL_HELP_GRID_BRIDGE_COMPONENT_D),
																_LS(IDS_CMD_LOADCOMB_CVL_HELP_GRID_BRIDGE_COMPONENT_E)};

	CString aPSValue[] = {_T("1.0"), _T("1.0"), _T("0.5"), _T("1.0"), _T("1.0")};
	CString aCRSHValue[] = {_LS(IDS_CMD_LOADCOMB_CVL_HELP_GRID_SEE_DC_VALUE), _T("1.0"), _T("0.5"), _T("1.0"), _T("1.0")};

	int nRowCount=1;
	for(int i=0; i<5; i++)
	{
		CStringArray aValue;
		aValue.SetSize(D_COL_NUM);

		aValue[0].Format(_T("%s"), aIndex[i]);
		aValue[1].Format(_T("%s"), aBridgeComponent[i]);
		aValue[2].Format(_T("%s"), aPSValue[i]);
		aValue[3].Format(_T("%s"), aCRSHValue[i]);

		SetDataToRow(nRowCount, aValue);
		nRowCount++;
	}
	SetRowHeight(1, 5, globalUtils.ScaleByDPI(18));
}

