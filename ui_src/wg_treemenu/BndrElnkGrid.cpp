// BndrElnkGrid.cpp: implementation of the CBndrElnkGrid class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BndrElnkGrid.h"

#include "..\wg_db\DBDoc.h"
#include "..\wg_db\UnitCtrl.h"

#include "..\wg_common\TBGrid.h"

#include "BndrElnkDlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBndrElnkGrid::CBndrElnkGrid(CBndrElnkDlg* pParent)
{
	m_pParent = NULL;
	ASSERT(pParent);
	m_pParent = pParent;

	m_bOnEditing = FALSE;
	m_nDirection = 0;
}

CBndrElnkGrid::~CBndrElnkGrid()
{
}

#define D_COL_NUM 2

///////////////////////////////////////////////////////////////////////////////
//
// User Defined Function

void CBndrElnkGrid::SetDataToRow(ROWCOL nRow, CStringArray& raValue)
{
	int nColCount = GetColCount();
	ASSERT(nColCount == raValue.GetSize());

	for (int i=1; i<=nColCount; i++)
		SetValueRange(CGXRange(nRow, i), raValue[i-1]);
}

BOOL CBndrElnkGrid::GetRowToData(ROWCOL nRow, CStringArray& raValue)
{
	int nColCount = GetColCount();
	ASSERT(nColCount == raValue.GetSize());

	for (int i=1; i<=nColCount; i++)
		raValue[i-1] = GetEditingValue(nRow, i);

	return TRUE;
}

CString CBndrElnkGrid::GetEditingValue(ROWCOL nRow, ROWCOL nCol)
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

void CBndrElnkGrid::SetHeaderTitle()
{ 
	int aWidth[] = {27, 65, 65};
	for (int i=0; i<3; i++) SetColWidth(i, i, globalUtils.ScaleByDPI(aWidth[i]));

	CString strUnit;
	CString str;

	if(m_nDirection == 0 || m_nDirection == 1 || m_nDirection == 2)
	{
		CUnitCtrl::GetUnitSystem(CUnitCtrl::m_ELNK_UNIT.dDistance, strUnit);
		str.Format(_T("%s\n(%s)"), _T("d(x)"), strUnit);
		SetValueRange(CGXRange(0, 1), str);

		CUnitCtrl::GetUnitSystem(CUnitCtrl::m_ELNK_UNIT.dForce, strUnit);  
		str.Format(_T("%s\n(%s)"), _T("F(y)"), strUnit);
		SetValueRange(CGXRange(0, 2), str);
	}
	else
	{
		CUnitCtrl::GetUnitSystem(CUnitCtrl::m_ELNK_UNIT.dRadian, strUnit);
		str.Format(_T("%s\n(%s)"), _T("r(rad.)"), strUnit);
		SetValueRange(CGXRange(0, 1), str);

		CUnitCtrl::GetUnitSystem(CUnitCtrl::m_ELNK_UNIT.dMoment, strUnit);  
		str.Format(_T("%s\n(%s)"), _T("M(y)"), strUnit);
		SetValueRange(CGXRange(0, 2), str);
	}
}

#define CWnd CGXGridWnd
BEGIN_MESSAGE_MAP(CBndrElnkGrid, CWnd)
#undef CWnd
	//{{AFX_MSG_MAP(CBndrElnkGrid)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBndrElnkGrid message handlers

// 그리드 초기화 
void CBndrElnkGrid::Initialize()
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
	SetFrozenCols(0, 0);

	// Index
	SetStyleRange(CGXRange().SetCols(0), CGXStyle()
		.SetFont(CGXFont_GC().SetSize(9))
		.SetHorizontalAlignment(DT_CENTER)
		.SetVerticalAlignment(DT_VCENTER)
		);

	// d(x)
	SetStyleRange(CGXRange().SetCols(1), CGXStyle()
		.SetFont(CGXFont_GC().SetSize(9))
		.SetValueType(GX_VT_NUMERIC)
		.SetHorizontalAlignment(DT_RIGHT)
		.SetVerticalAlignment(DT_VCENTER)
		.SetEnabled(TRUE)
		.SetPlaces(4)
		);

	// F(y)
	SetStyleRange(CGXRange().SetCols(2), CGXStyle()
		.SetFont(CGXFont_GC().SetSize(9))
		.SetValueType(GX_VT_NUMERIC)
		.SetHorizontalAlignment(DT_RIGHT)
		.SetVerticalAlignment(DT_VCENTER)
		.SetEnabled(TRUE)
		.SetPlaces(4)
		);

	SetRowHeight(0, 0, globalUtils.ScaleByDPI(36));

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

	SetScrollBarMode(SB_VERT, gxnEnabled | gxnEnhanced);

	// 표준 스타일 변경
	ChangeColHeaderStyle(CGXStyle().SetFont(CGXFont_GC().SetBold(FALSE)));

	SetHeaderTitle();

	LockUpdate(FALSE);
}

BOOL CBndrElnkGrid::GetStyleRowCol(ROWCOL nRow, ROWCOL nCol, CGXStyle& style, GXModifyType mt, int nType)
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

void CBndrElnkGrid::OnModifyCell(ROWCOL nRow, ROWCOL nCol)
{
	CGXGridWnd::OnModifyCell(nRow, nCol);

}

BOOL CBndrElnkGrid::OnEndEditing(ROWCOL nRow, ROWCOL nCol)
{
	return CGXGridWnd::OnEndEditing(nRow, nCol);
}

void CBndrElnkGrid::SetData2Grid(int *nSymNum)
{
	//BOOL bOldLock = LockUpdate(TRUE);

	int nCount = *nSymNum;
	int nRowCount = 1;
	double dZero = 0.;
	
	SetRowCount(nCount);

	for(int i=0; i<nCount; i++)
	{
		CStringArray aValue;
		aValue.SetSize(D_COL_NUM);

		aValue[0].Format(_T("%g"), dZero);
		aValue[1].Format(_T("%g"), dZero);

		SetDataToRow(i+1, aValue);
	}
	SetRowHeight(1, nCount, globalUtils.ScaleByDPI(18));

	//LockUpdate(bOldLock);
	//if(!bOldLock) Redraw();
}

BOOL CBndrElnkGrid::GetGrid2Data(double dDistance[10], double dRadian[10], double dForce[10], double dMoment[10])
{
	int nCount = GetRowCount();
	
	CStringArray aValue;
	aValue.SetSize(D_COL_NUM);

	for(int i=0; i<nCount; i++)
	{
		if(!GetRowToData(i+1, aValue)) return FALSE;

		if(m_nDirection == 0 || m_nDirection == 1 || m_nDirection == 2)
		{
			dDistance[i] = _tstof(aValue[0]);
			dForce[i] = _tstof(aValue[1]);
		}
		else
		{
			dRadian[i] = _tstof(aValue[0]);
			dMoment[i] = _tstof(aValue[1]);
		}
	}

	return TRUE;
}

void CBndrElnkGrid::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
	HWND hWnd;
	if ((hWnd = GetSafeHwnd()) == 0 || !IsWindow(hWnd)) return;

	switch(lHint)
	{
	case D_UPDATE_UNIT:
		SetHeaderTitle();
		ConvertCoeffUnit();
		break;
	default:
		break;
	}
}
 
void CBndrElnkGrid::ConvertCoeffUnit()
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();     ASSERT(pDoc);
	CUnitCtrl* pUnitCtrl = pDoc->m_pUnitCtrl; ASSERT(pUnitCtrl);
	
	double dDistance[10];
	double dRadian[10];
	double dForce[10];
	double dMoment[10];

	int nCount = GetRowCount();
	
	CStringArray aValue;
	aValue.SetSize(D_COL_NUM);

	for(int i=0; i<nCount; i++)
	{
		if(!GetRowToData(i+1, aValue)) return;

		if(m_nDirection == 0 || m_nDirection == 1 || m_nDirection == 2)
		{
			dDistance[i] = _tstof(aValue[0]);
			dForce[i] = _tstof(aValue[1]);
		}
		else
		{
			dRadian[i] = _tstof(aValue[0]);
			dMoment[i] = _tstof(aValue[1]);
		}
		
	}

	double dConvDistanceFactPrev = pUnitCtrl->GetConvertFactorPrevious(CUnitCtrl::m_ELNK_UNIT.dDistance);
	double dConvDistanceFactCur  = pUnitCtrl->GetConvertFactorCurrent(CUnitCtrl::m_ELNK_UNIT.dDistance);
	double dConvRadianFactPrev = pUnitCtrl->GetConvertFactorPrevious(CUnitCtrl::m_ELNK_UNIT.dRadian);
	double dConvRadianFactCur  = pUnitCtrl->GetConvertFactorCurrent(CUnitCtrl::m_ELNK_UNIT.dRadian);
	double dConvForceFactPrev = pUnitCtrl->GetConvertFactorPrevious(CUnitCtrl::m_ELNK_UNIT.dForce);
	double dConvForceFactCur  = pUnitCtrl->GetConvertFactorCurrent(CUnitCtrl::m_ELNK_UNIT.dForce);
	double dConvMomentFactPrev = pUnitCtrl->GetConvertFactorPrevious(CUnitCtrl::m_ELNK_UNIT.dMoment);
	double dConvMomentFactCur  = pUnitCtrl->GetConvertFactorCurrent(CUnitCtrl::m_ELNK_UNIT.dMoment);

	for(int i=0; i<nCount; i++)
	{
		dDistance[i] *= dConvDistanceFactPrev;
		dDistance[i] /= dConvDistanceFactCur;
		dRadian[i] *= dConvRadianFactPrev;
		dRadian[i] /= dConvRadianFactCur;
		dForce[i] *= dConvForceFactPrev;
		dForce[i] /= dConvForceFactCur;
		dMoment[i] *= dConvMomentFactPrev;
		dMoment[i] /= dConvMomentFactCur;
	}
	
	int nRowCount = 1;
	
	SetRowCount(nCount);

	for(int i=0; i<nCount; i++)
	{
		CStringArray aValue;
		aValue.SetSize(D_COL_NUM);

		if(m_nDirection == 0 || m_nDirection == 1 || m_nDirection == 2)
		{
			aValue[0].Format(_T("%g"), dDistance[i]);
			aValue[1].Format(_T("%g"), dForce[i]);
		}
		else
		{
			aValue[0].Format(_T("%g"), dRadian[i]);
			aValue[1].Format(_T("%g"), dMoment[i]);
		}

		SetDataToRow(i+1, aValue);
	}
	SetRowHeight(1, nCount, globalUtils.ScaleByDPI(18));
}

void CBndrElnkGrid::OnChangeSymmetric(int nIndex)
{
	if(nIndex == 0) // Symmetric
	{
		SetStyleRange(CGXRange().SetRows(1), CGXStyle().SetEnabled(FALSE)); 
	}
	else // Unsymmetric
	{
		SetStyleRange(CGXRange().SetRows(1), CGXStyle().SetEnabled(TRUE)); 
	}
	
	CStringArray aValue;
	aValue.SetSize(D_COL_NUM);

	aValue[0].Format(_T("%g"), 0.);
	aValue[1].Format(_T("%g"), 0.);

	SetDataToRow(1, aValue);
}

void CBndrElnkGrid::OnChangeDirecion(int nIndex)
{
	m_nDirection = nIndex;

	SetHeaderTitle();
	//ConvertCoeffUnit();
}