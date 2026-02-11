// CMNlacItemGrid.cpp: implementation of the CCMNlacItemGrid class.
//
// 2003. 03. 26   by TAE
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMNlacItemGrid.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_UnitCtrl.h"

#include "..\wg_common\wg_common_TBGrid.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define COLUMN_COUNT   2

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCMNlacItemGrid::CCMNlacItemGrid()
{
	m_nOption = 0;
	m_dMaxDisp = 1;
	m_nRowCount = 0;
	m_nOldRowCount = 0;
}

CCMNlacItemGrid::~CCMNlacItemGrid()
{

}

#define CWnd CGXGridWnd
BEGIN_MESSAGE_MAP(CCMNlacItemGrid, CWnd)
#undef CWnd
	//{{AFX_MSG_MAP(CCMNlacItemGrid)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////

// 그리드 초기화
void CCMNlacItemGrid::Initialize()
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
	
	// 표준 스타일 변경
	ChangeStandardStyle(CGXStyle()
		.SetFont(CGXFont_GC().SetSize(9))
		.SetVerticalAlignment(DT_BOTTOM)
		.SetAllowEnter(FALSE)
		);
	ChangeColHeaderStyle(CGXStyle()
		.SetFont(CGXFont_GC().SetBold(FALSE))
		);
	ChangeRowHeaderStyle(CGXStyle()
		.SetFont(CGXFont_GC().SetBold(FALSE))
		.SetHorizontalAlignment(DT_RIGHT)
		.SetControl(GX_IDS_CTRL_HEADER)
		);
	GetParam()->SetNumberedRowHeaders(FALSE);

	// Header
	SetStyleRange(CGXRange().SetCols(0), CGXStyle()
			.SetControl(GX_IDS_CTRL_HEADER)
			);
	SetStyleRange(CGXRange().SetRows(0), CGXStyle()
			.SetControl(GX_IDS_CTRL_HEADER)
			);

	SetColCount(COLUMN_COUNT);

	// Step
	SetStyleRange(CGXRange().SetCols(1), CGXStyle()
			.SetControl(GX_IDS_CTRL_STATIC)
			.SetHorizontalAlignment(DT_CENTER)
			.SetValueType(GX_VT_NUMERIC)
			.SetFormat(GX_FMT_FIXED)
			.SetPlaces(0)
			.SetEnabled(FALSE)
			);

	// Load Factor or Master Node Displacement
	SetStyleRange(CGXRange().SetCols(2), CGXStyle()
			.SetControl(GX_IDS_CTRL_EDIT)
			.SetFormat(GX_FMT_FIXED)
			.SetPlaces(4)
			.SetHorizontalAlignment(DT_RIGHT)
			.SetVerticalAlignment(DT_VCENTER)
			.SetValueType(GX_VT_NUMERIC)
			);

	// MemoryDC를 사용한다.
	SetDrawingTechnique(gxDrawUsingMemDC);

	CGXProperties* pProp = GetParam()->GetProperties();
	pProp->SetMarkColHeader(FALSE); // Turn off pressed button effect for column headers
	pProp->SetMarkRowHeader(FALSE);
	pProp->SetUserProperty(GX_IDS_OUTLINECURRENTCELL, (CGXStyle) pProp->sInvertNoBorder);

	// Row와  Column의 이동을 금지시킨다.
	GetParam()->EnableMoveRows(FALSE);
	GetParam()->EnableMoveCols(FALSE);

	GetParam()->EnableTrackRowHeight(FALSE);  
	GetParam()->EnableUndo(FALSE);
		
	// 헤더의 Current cell을 막는다.
	ColHeaderStyle().SetEnabled(FALSE);
	RowHeaderStyle().SetEnabled(FALSE);

	// scroll bar
	SetScrollBarMode(SB_VERT, gxnEnabled | gxnEnhanced);  

	SetHeaderTitle();

	LockUpdate(FALSE);
}


void CCMNlacItemGrid::SetHeaderTitle()
{
	SetRowHeight(0, 0, globalUtils.ScaleByDPI(20));
	int nColWidth[] = { 20, 87, 220 };
	for (ROWCOL nCol=0; nCol<=COLUMN_COUNT; nCol++)
		SetColWidth(nCol, nCol, globalUtils.ScaleByDPI(nColWidth[nCol]));

	CString czTitle[2][COLUMN_COUNT] = {
		{ _LS(IDS_WG_CMD__ADDD__Step), _LS(IDS_CMD_LOAD_FACTOR) },
		{ _LS(IDS_WG_CMD__ADDD__Step), _LS(IDS_CMD_MASTER_NODE_DISPLACEMENT) },
	};

	CString title;
	for (ROWCOL nCol=1; nCol<=COLUMN_COUNT; nCol++)
	{
		title = czTitle[m_nOption][nCol-1];

		if (m_nOption==1 && nCol==2)
		{
			CString unit = _T("");
			CDBDoc* pDoc= CDBDoc::GetDocPoint(); ASSERT(pDoc);
			pDoc->m_pUnitCtrl->GetUnitSystem(D_UNITSYS_BASE_LENGTH, unit);
			title += _T(" ("); title += unit; title += _T(")");
		}

		SetValueRange(CGXRange(0, nCol), title);
	}
}

void CCMNlacItemGrid::SetRowCountUser(int nRowCount)
{
	ASSERT(nRowCount >= 0);

	m_nRowCount = nRowCount;
	SetRowCount(m_nRowCount);

	if (m_nRowCount > m_nOldRowCount) // 새로 추가된 Row에 대해서 Default 값 Setting
		SetDefaultValue(m_nOldRowCount+1, m_nRowCount);

	m_nOldRowCount = m_nRowCount;
}

void CCMNlacItemGrid::GenerateValue(double dValue)
{
	ROWCOL nCol = 2;
	int nRowCount = GetRowCount();

	for(ROWCOL nRow=1; nRow<=nRowCount; nRow++)
		SetValueRange(CGXRange(nRow, nCol), double(dValue*nRow));
}

void CCMNlacItemGrid::SetValueRow(ROWCOL nRow, double dValue)
{
	ASSERT(nRow > 0 && nRow <= GetRowCount());

	ROWCOL nCol = 2;
	SetValueRange(CGXRange(nRow, nCol), double(dValue));
}

double CCMNlacItemGrid::GetValueRow(ROWCOL nRow)
{
	ASSERT(nRow > 0 && nRow <= GetRowCount());

	ROWCOL nCol = 2;
	CString csValue = GetValueRowCol(nRow, nCol);
	return _tstof(csValue);
}

void CCMNlacItemGrid::SetDefaultValue(ROWCOL nFromRow, ROWCOL nToRow)
{
	double dValue;

	for (ROWCOL nRow=nFromRow; nRow<=nToRow; nRow++)
	{
		SetValueRange(CGXRange(nRow, 1), LONG(nRow));
		dValue = m_dMaxDisp / nRow;
		SetValueRange(CGXRange(nRow, 2), double(dValue));
	}
}


//////////////////////////////////////////////////////////////////////
BOOL CCMNlacItemGrid::GetStyleRowCol(ROWCOL nRow, ROWCOL nCol, CGXStyle& style, GXModifyType mt, int nType)
{
	BOOL bCurrentRow;
	BOOL bCurrentCell;
	BOOL bEnabled;
	BOOL bHeaderCol;

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
	else // Column style에서 enable 정보 알아냄
	{
		CGXStyle styleEx;
		GetColStyle(nCol, styleEx);
		bEnabled = (!styleEx.GetIncludeEnabled() || styleEx.GetEnabled());
	}

	bHeaderCol = IsFrozenCol(nCol);

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
		if (bHeaderCol) style           // Row or Column Header
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
	else if (bHeaderCol) style    // Enabled Header Col(Only Frozen)
		.SetInterior(CTBGrid::m_Color.NormalModeBg)
		.SetTextColor(CTBGrid::m_Color.NormalModeFg);

	return TRUE;
}
