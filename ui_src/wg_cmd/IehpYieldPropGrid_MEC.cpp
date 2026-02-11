// IehpYieldPropGrid_MEC.cpp : implementation file
// 
// 2003. 04. 04   by TAE
/////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "wg_cmd.h"
#include "IehpYieldPropGrid_MEC.h"
#include "IehpYieldPropDlgNew_MEC.h"

#include "..\wg_common\wg_common_TBGrid.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define D_COL_NUM         13

/////////////////////////////////////////////////////////////////////////////
// CIehpYieldPropGrid_MEC

CIehpYieldPropGrid_MEC::CIehpYieldPropGrid_MEC(CIehpYieldPropDlgNew_MEC* pParent)
{
	m_pParent = NULL;
	ASSERT(pParent);
	m_pParent = pParent;
}

CIehpYieldPropGrid_MEC::~CIehpYieldPropGrid_MEC()
{
}

///////////////////////////////////////////////////////////////////////////////
//
// User Defined Finction

void CIehpYieldPropGrid_MEC::SetFormatMode(int nMode)
{
	HideCols(6, 9, nMode == 0 || nMode == 2 || nMode == 3);
	HideCols(2, 5, nMode == 1 || nMode == 2 || nMode == 3);
	HideCols(10, 13, nMode != 3);
}

void CIehpYieldPropGrid_MEC::SetDataToGrid(double dP1st[][11], double dM1st[][11], double dP2nd[][11], double dM2nd[][11], double dP3rd[][11], double dM3rd[][11],BOOL bAssignProperties, BOOL bAuto)
{
	CString aTitle[] = {_T("E(c)"), _T("D(c)"), _T("C(c)"), _T("B(c)"), _T("A(c)"), _T("O"), _T("A(t)"), _T("B(t)"), _T("C(t)"), _T("D(t)"), _T("E(t)")};

	CStringArray aValue;
	aValue.SetSize(D_COL_NUM);

	int nHeader = GetColHeaderNum();
	for (int i=0; i<11; i++)
	{
		aValue[0] = aTitle[i];
		if(m_bConc)
		{
			if(i==5 || i==10) 
			{
				aValue[1].Format(_T("%g"), dP1st[0][i]);
				aValue[2].Format(_T("%g"), dM1st[0][i]);
				aValue[3].Format(_T("%g"), dP1st[1][i]);
				aValue[4].Format(_T("%g"), dM1st[1][i]);
			}            
			else
			{
				aValue[1].Format(_T("-"));
				aValue[2].Format(_T("-"));
				aValue[3].Format(_T("-"));
				aValue[4].Format(_T("-"));
			}
		}
		else
		{
			aValue[1].Format(_T("%g"), dP1st[0][i]);
			aValue[2].Format(_T("%g"), dM1st[0][i]);
			aValue[3].Format(_T("%g"), dP1st[1][i]);
			aValue[4].Format(_T("%g"), dM1st[1][i]);
		}

		aValue[5].Format(_T("%g"), dP2nd[0][i]);
		aValue[6].Format(_T("%g"), dM2nd[0][i]);
		aValue[7].Format(_T("%g"), dP2nd[1][i]);
		aValue[8].Format(_T("%g"), dM2nd[1][i]);

		aValue[9].Format(_T("%g"), dP3rd[0][i]);
		aValue[10].Format(_T("%g"), dM3rd[0][i]);
		aValue[11].Format(_T("%g"), dP3rd[1][i]);
		aValue[12].Format(_T("%g"), dM3rd[1][i]);

		SetDataToRow(i+nHeader, aValue);
	}

	SetEnabled(bAuto, m_bConc, bAssignProperties);
}

BOOL CIehpYieldPropGrid_MEC::GetGridToData(double dP1st[][11], double dM1st[][11], double dP2nd[][11], double dM2nd[][11], double dP3rd[][11], double dM3rd[][11])
{
	CStringArray aValue;
	aValue.SetSize(D_COL_NUM);

	int nHeader = GetColHeaderNum();
	for (int i=0; i<11; i++)
	{
		if (!GetRowToData(i+nHeader, aValue)) return FALSE;

		if(!m_bConc)
		{
			dP1st[0][i] = _tstof(aValue[1]);
			dM1st[0][i] = _tstof(aValue[2]);
			dP1st[1][i] = _tstof(aValue[3]);
			dM1st[1][i] = _tstof(aValue[4]); 
		}
		dP2nd[0][i] = _tstof(aValue[5]);
		dM2nd[0][i] = _tstof(aValue[6]);
		dP2nd[1][i] = _tstof(aValue[7]);
		dM2nd[1][i] = _tstof(aValue[8]);

		dP3rd[0][i] = _tstof(aValue[9]);
		dM3rd[0][i] = _tstof(aValue[10]);
		dP3rd[1][i] = _tstof(aValue[11]);
		dM3rd[1][i] = _tstof(aValue[12]);
	}
	return TRUE;
}

int  CIehpYieldPropGrid_MEC::GetColHeaderNum()
{
	return 4;
}

void CIehpYieldPropGrid_MEC::SetEnabled(BOOL bAuto, BOOL bConc, BOOL bAssignProperties)
{
	if(bAssignProperties)
	{
		SetStyleRange(CGXRange( 5, 2,  8, 5), CGXStyle().SetEnabled(FALSE));
		SetStyleRange(CGXRange(10, 2, 13, 5), CGXStyle().SetEnabled(FALSE));
		SetStyleRange(CGXRange( 5, 6,  8, 9), CGXStyle().SetEnabled(FALSE));
		SetStyleRange(CGXRange(10, 6, 13, 9), CGXStyle().SetEnabled(FALSE));
		SetStyleRange(CGXRange( 5,10,  8,13), CGXStyle().SetEnabled(FALSE));
		SetStyleRange(CGXRange(10,10, 13,13), CGXStyle().SetEnabled(FALSE));
	}
	else
	{
		SetStyleRange(CGXRange( 5, 2,  8, 5), CGXStyle().SetEnabled(!bAuto && !bConc));
		SetStyleRange(CGXRange(10, 2, 13, 5), CGXStyle().SetEnabled(!bAuto && !bConc));
		SetStyleRange(CGXRange( 5, 6,  8, 9), CGXStyle().SetEnabled(!bAuto));
		SetStyleRange(CGXRange(10, 6, 13, 9), CGXStyle().SetEnabled(!bAuto));
		SetStyleRange(CGXRange( 5,10,  8,13), CGXStyle().SetEnabled(!bAuto));
		SetStyleRange(CGXRange(10,10, 13,13), CGXStyle().SetEnabled(!bAuto));
	}
}

void CIehpYieldPropGrid_MEC::SetDataToRow(ROWCOL nRow, CStringArray& raValue)
{
	int nColCount = GetColCount();
	ASSERT(nColCount == raValue.GetSize());

	for (int i=1; i<=nColCount; i++)
		SetValueRange(CGXRange(nRow, i), raValue[i-1]);
}

BOOL CIehpYieldPropGrid_MEC::GetRowToData(ROWCOL nRow, CStringArray& raValue)
{
	int nColCount = GetColCount();
	ASSERT(nColCount == raValue.GetSize());

	for (int i=1; i<=nColCount; i++)
		raValue[i-1] = GetEditingValue(nRow, i);

	return TRUE;
}

CString CIehpYieldPropGrid_MEC::GetEditingValue(ROWCOL nRow, ROWCOL nCol)
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
					if (csNum == _T("0")) csNum = _T("1");
					else csNum = _T("0");
				}
			}
		}
	}
	if (!bGet) csNum = GetValueRowCol(nRow, nCol);
	return csNum;
}

void CIehpYieldPropGrid_MEC::SetHeaderTitle()
{ 
	/*
	if (m_bConc)
	{
		int aWidth[] = {20, 65, 85, 85, 85, 85, 80, 80, 80, 80};
		for (int i=0; i<=D_COL_NUM; i++)
			SetColWidth(i, i, aWidth[i]);
	}
	else 
	{
		int aWidth[] = {20, 65, 85, 85, 85, 85, 80, 80, 80, 80};
		for (int i=0; i<=D_COL_NUM; i++)
			SetColWidth(i, i, aWidth[i]);
	}
	*/
	int aWidth[] = {21, 65, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85};
	for (int i=0; i<=D_COL_NUM; i++) SetColWidthByDPI(i, i, aWidth[i]);

	SetCoveredCellsRowCol(0, 0, 3, 0);
	SetCoveredCellsRowCol(0, 1, 3, 1);
	SetValueRange(CGXRange(0, 1), _T(""));
	SetCoveredCellsRowCol(0, 2, 0, 5);
	SetValueRange(CGXRange(0, 2), _LS(IDS_CMD_IEHP_1st_PM_Interaction_Curve));
	SetCoveredCellsRowCol(1, 2, 1, 3);
	SetValueRange(CGXRange(1, 2), _LS(IDS_CMD_IEHP_about_y_axis));
	SetCoveredCellsRowCol(1, 4, 1, 5);
	SetValueRange(CGXRange(1, 4), _LS(IDS_CMD_IEHP_about_z_axis));
	SetCoveredCellsRowCol(2, 2, 3, 2);
	if (m_bConc) SetValueRange(CGXRange(2, 2), _LS(IDS_CMD_IEHP_P_PC));
	else SetValueRange(CGXRange(2, 2), m_bValueMode ? _ULS(P) : _LS(IDS_CMD_IEHP_PPCBy_PCPCBy));
	SetCoveredCellsRowCol(2, 3, 3, 3);
	if (m_bConc) SetValueRange(CGXRange(2, 3), _LS(IDS_CMD_IEHP_M_MCy));
	else SetValueRange(CGXRange(2, 3), m_bValueMode ? _ULS(M) : _LS(IDS_CMD_IEHP_M_MCy_max));
	SetCoveredCellsRowCol(2, 4, 3, 4);
	if (m_bConc) SetValueRange(CGXRange(2, 4), _LS(IDS_CMD_IEHP_P_PC));
	else SetValueRange(CGXRange(2, 4), m_bValueMode ? _ULS(P) : _LS(IDS_CMD_IEHP_PPCBy_PCPCBz));
	SetCoveredCellsRowCol(2, 5, 3, 5);
	if (m_bConc) SetValueRange(CGXRange(2, 5), _LS(IDS_CMD_IEHP_M_MCz));
	else SetValueRange(CGXRange(2, 5), m_bValueMode ? _ULS(M) : _LS(IDS_CMD_IEHP_M_MCz_max));

	SetCoveredCellsRowCol(0, 6, 0, 9);
	SetValueRange(CGXRange(0, 6), _LS(IDS_CMD_IEHP_2st_PM_Interaction_Curve));
	SetCoveredCellsRowCol(1, 6, 1, 7);
	SetValueRange(CGXRange(1, 6), _LS(IDS_CMD_IEHP_about_y_axis));
	SetCoveredCellsRowCol(1, 8, 1, 9);
	SetValueRange(CGXRange(1, 8), _LS(IDS_CMD_IEHP_about_z_axis));
	SetCoveredCellsRowCol(2, 6, 3, 6);
	SetValueRange(CGXRange(2, 6), m_bValueMode ? _ULS(P) : _LS(IDS_CMD_IEHP_PPYBy_PYPYBy));
	SetCoveredCellsRowCol(2, 7, 3, 7);
	SetValueRange(CGXRange(2, 7), m_bValueMode ? _ULS(M) : _LS(IDS_CMD_IEHP_M_MYy_max));
	SetCoveredCellsRowCol(2, 8, 3, 8);
	SetValueRange(CGXRange(2, 8), m_bValueMode ? _ULS(P) : _LS(IDS_CMD_IEHP_PPYBy_PYPYBz));
	SetCoveredCellsRowCol(2, 9, 3, 9);
	SetValueRange(CGXRange(2, 9), m_bValueMode ? _ULS(M) : _LS(IDS_CMD_IEHP_M_MYz_max));

	SetCoveredCellsRowCol(0, 10, 0, 13);
	SetValueRange(CGXRange(0, 10), _LS(IDS_CMD_IEHP_3rd_PM_Interaction_Curve));
	SetCoveredCellsRowCol(1, 10, 1, 11);
	SetValueRange(CGXRange(1, 10), _LS(IDS_CMD_IEHP_about_y_axis));
	SetCoveredCellsRowCol(1, 12, 1, 13);
	SetValueRange(CGXRange(1, 12), _LS(IDS_CMD_IEHP_about_z_axis));
	SetCoveredCellsRowCol(2, 10, 3, 10);
	SetValueRange(CGXRange(2, 10), m_bValueMode ? _ULS(P) : _LS(IDS_CMD_IEHP_PPUBy_PUPUBy));
	SetCoveredCellsRowCol(2, 11, 3, 11);
	SetValueRange(CGXRange(2, 11), m_bValueMode ? _ULS(M) : _LS(IDS_CMD_IEHP_M_MUy_max));
	SetCoveredCellsRowCol(2, 12, 3, 12);
	SetValueRange(CGXRange(2, 12), m_bValueMode ? _ULS(P) : _LS(IDS_CMD_IEHP_PPUBy_PUPUBz));
	SetCoveredCellsRowCol(2, 13, 3, 13);
	SetValueRange(CGXRange(2, 13), m_bValueMode ? _ULS(M) : _LS(IDS_CMD_IEHP_M_MUz_max));
}

#define CWnd CCMGXGridWnd
BEGIN_MESSAGE_MAP(CIehpYieldPropGrid_MEC, CWnd)
#undef CWnd
	//{{AFX_MSG_MAP(CIehpYieldPropGrid_MEC)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CIehpYieldPropGrid_MEC message handlers

// 그리드 초기화 
void CIehpYieldPropGrid_MEC::Initialize()
{
	CCMGXGridWnd::Initialize();

	m_bValueMode = FALSE;

	LockUpdate(TRUE);

	SetColCount(D_COL_NUM);
	SetRowCount(14);
	
	SetRowHeightByDPI(1, 14, 17);
	SetRowHeightByDPI(3, 3, 30);

	// 표준 스타일 변경
	ChangeStandardStyle(CGXStyle()
			.SetFont(CGXFont_GC().SetSize(9))
			.SetVerticalAlignment(DT_BOTTOM)
			.SetAllowEnter(FALSE));
	ChangeColHeaderStyle(CGXStyle().SetFont(CGXFont_GC().SetBold(FALSE)));
	ChangeRowHeaderStyle(CGXStyle()
					.SetFont(CGXFont_GC().SetBold(FALSE))
					.SetHorizontalAlignment(DT_RIGHT)
					.SetControl(GX_IDS_CTRL_HEADER));
	GetParam()->SetNumberedRowHeaders(FALSE);

	// Header
	SetStyleRange(CGXRange().SetCols(0), CGXStyle()
			.SetControl(GX_IDS_CTRL_HEADER));

	SetStyleRange(CGXRange().SetRows(0, 3), CGXStyle()
			.SetControl(GX_IDS_CTRL_HEADER));
	SetFrozenRows(1, 1);
	SetFrozenRows(2, 2);
	SetFrozenRows(3, 3);
	SetStyleRange(CGXRange().SetRows(1, 3), CGXStyle().SetEnabled(FALSE));

	SetStyleRange(CGXRange().SetCols(1), CGXStyle()
			.SetHorizontalAlignment(DT_CENTER)
			.SetValueType(GX_VT_STRING)
			.SetEnabled(FALSE)
			);

	SetStyleRange(CGXRange().SetCols(2, 13), CGXStyle()
			.SetControl(GX_IDS_CTRL_EDIT)
			.SetHorizontalAlignment(DT_CENTER)
			.SetValueType(GX_VT_NUMERIC)
			.SetPlaces(2)
			);

	int nHeader = GetColHeaderNum();
	SetStyleRange(CGXRange().SetRows(nHeader+ 0), CGXStyle().SetEnabled(FALSE));
	SetStyleRange(CGXRange().SetRows(nHeader+ 5), CGXStyle().SetEnabled(FALSE));
	SetStyleRange(CGXRange().SetRows(nHeader+10), CGXStyle().SetEnabled(FALSE));

	GetParam()->EnableUndo(FALSE);
	GetParam()->EnableTrackRowHeight(FALSE);  

	// Row와  Column의 이동을 금지시킨다.
	GetParam()->EnableMoveRows(FALSE);
	GetParam()->EnableMoveCols(FALSE);

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

	SetScrollBarMode(SB_BOTH, gxnDisabled);

	SetHeaderTitle();

	LockUpdate(FALSE);
}

BOOL CIehpYieldPropGrid_MEC::GetStyleRowCol(ROWCOL nRow, ROWCOL nCol, CGXStyle& style, GXModifyType mt, int nType)
{
	BOOL bCurrentRow;
	BOOL bCurrentCell;
	BOOL bEnabled;
	BOOL bHeaderCol;
	BOOL bHeaderRow;

	ROWCOL ncRow, ncCol;    // 현재 Row, Col
	CCMGXGridWnd::GetStyleRowCol(nRow, nCol, style, mt, nType);

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
			.SetInterior(CTBGrid::m_Color.NormalModeBg)
			.SetTextColor(CTBGrid::m_Color.NormalModeFg);

		return TRUE;
	}

	// Disable되었을 때
	if (!bEnabled)  
	{
		if (bHeaderCol || bHeaderRow) style           // Row or Column Header
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
	else if (bHeaderCol || bHeaderRow) style    // Enabled Header Col(Only Frozen)
		.SetInterior(CTBGrid::m_Color.NormalModeBg)
		.SetTextColor(CTBGrid::m_Color.NormalModeFg);

	return TRUE;
}

void CIehpYieldPropGrid_MEC::OnModifyCell(ROWCOL nRow, ROWCOL nCol)
{
	CCMGXGridWnd::OnModifyCell(nRow, nCol);
	//ChangeColumnStatus(nRow, nCol);
	UpdateParentChart(nRow, nCol);
}

/*
void CIehpYieldPropGrid_MEC::ChangeColumnStatus(ROWCOL nRow, ROWCOL nCol)
{
	if (nCol < 2) return;
	
	CString csValue = GetEditingValue(nRow, nCol);
	SetValueRange(CGXRange(nRow, nCol+1), csValue);
}
*/

void CIehpYieldPropGrid_MEC::UpdateParentChart(ROWCOL nRow, ROWCOL nCol)
{
	ASSERT(m_pParent);

	double dP1st[2][11], dM1st[2][11], dP2nd[2][11], dM2nd[2][11], dP3rd[2][11], dM3rd[2][11];
	GetGridToData(dP1st, dM1st, dP2nd, dM2nd, dP3rd, dM3rd);

	for(int i=0; i<2; i++)
	{
		for(int j=1; j<10; j++)  
		{
			if(j<5)
			{
				if(!m_bConc) if(dP1st[i][j] <= dP1st[i][j + 1]) return;
				if(dP2nd[i][j] <= dP2nd[i][j + 1]) return;
				if(dP3rd[i][j] <= dP3rd[i][j + 1]) return;
			}
			else if(j>5) // except 5
			{
				if(!m_bConc) if(dP1st[i][j] >= dP1st[i][j + 1]) return;
				if(dP2nd[i][j] >= dP2nd[i][j + 1]) return;
				if(dP3rd[i][j] >= dP3rd[i][j + 1]) return;
			}
			if(j!=5) // except 5
			{
				if(!m_bConc) if(dM1st[i][j] <= 0. || dM1st[i][j] >= 1.) return;
				if(dM2nd[i][j] <= 0. || dM2nd[i][j] >= 1.) return;
				if(dM3rd[i][j] <= 0. || dM3rd[i][j] >= 1.) return;
			}
		}

		int nTabID = m_pParent->m_Tab.GetCurSel();

		if(nTabID == 0)	
		{
			for(int j=0; j<11; j++)
			{


				if(!m_bConc)
				{
					m_pParent->m_MainData.dP1st[i][j] = dP1st[i][j];
					m_pParent->m_MainData.dM1st[i][j] = dM1st[i][j];
				}
				m_pParent->m_MainData.dP2nd[i][j] = dP2nd[i][j];
				m_pParent->m_MainData.dM2nd[i][j] = dM2nd[i][j];

				m_pParent->m_MainData.dP3rd[i][j] = dP3rd[i][j];
				m_pParent->m_MainData.dM3rd[i][j] = dM3rd[i][j];
			}
		}
		else if(nTabID == 1)
		{
			for(int j=0; j<11; j++)
			{


				if(!m_bConc)
				{
					m_pParent->m_SubData.dP1st[i][j] = dP1st[i][j];
					m_pParent->m_SubData.dM1st[i][j] = dM1st[i][j];
				}
				m_pParent->m_SubData.dP2nd[i][j] = dP2nd[i][j];
				m_pParent->m_SubData.dM2nd[i][j] = dM2nd[i][j];

				m_pParent->m_SubData.dP3rd[i][j] = dP3rd[i][j];
				m_pParent->m_SubData.dM3rd[i][j] = dM3rd[i][j];
			}
		}
		else
		{
			ASSERT(FALSE);
			return;
		}
	}

	m_pParent->UpdateGraph(m_pParent->m_cmbPlot.GetCurSel());
}

BOOL CIehpYieldPropGrid_MEC::SetValueMode(BOOL bValueMode)
{
	m_bValueMode = bValueMode;
	SetHeaderTitle();
	return TRUE;
}



///////////////////////////////////////////////////////////////////////////////////////////
// Graph

BEGIN_MESSAGE_MAP(CIehpYieldPropGraph_MEC, CWnd)
	//{{AFX_MSG_MAP(CIehpYieldPropGraph_MEC)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

CIehpYieldPropGraph_MEC::CIehpYieldPropGraph_MEC() : m_bDraw1stCurve(TRUE), m_bDraw3rdCurve(FALSE), m_bDrawCrack(false)
{
	memset(m_dV_Pcrst, 0, sizeof(m_dV_Pcrst));
	memset(m_dV_Mcrst, 0, sizeof(m_dV_Mcrst));
}
CIehpYieldPropGraph_MEC::~CIehpYieldPropGraph_MEC()
{
}

BOOL CIehpYieldPropGraph_MEC::Create(CWnd *pParentWnd, const RECT& rect, UINT nID)
{
	CREATESTRUCT cs;
	LPCTSTR lpClass = AfxRegisterWndClass(CS_OWNDC);
	cs.lpszClass = lpClass;
	cs.style = WS_CHILD | WS_CLIPCHILDREN | WS_VISIBLE;
	CWnd::PreCreateWindow(cs);
	return CWnd::Create(lpClass, NULL, WS_CHILD | WS_CLIPCHILDREN | WS_VISIBLE, rect, pParentWnd, nID, NULL);
}

void CIehpYieldPropGraph_MEC::FitToParent(CRect rect)
{
	SetWindowPos(NULL, 0, 0, rect.Width(), rect.Height(), SWP_NOZORDER | SWP_SHOWWINDOW | SWP_NOMOVE);
}

void CIehpYieldPropGraph_MEC::ClearStrengthBkgrnd(CDC* pDC)
{
	CBrush BackBrush(D_COLOR_WHITE);
	CBrush* pOldBrush = pDC->SelectObject(&BackBrush);
	CRect rect;
	GetClientRect(&rect);
	pDC->PatBlt(rect.left, rect.top, rect.Width(), rect.Height(), PATCOPY);
	pDC->SelectObject(pOldBrush);
	BackBrush.DeleteObject();

	// Draw Axis

	rect.DeflateRect(0,0,0,0);

	int nWidth = rect.Width();
	int nHeight = rect.Height();
	int nMargin = 5;
	int nGrid = 4;

	// X axis(P)
	CPen Pen,*pOldPen;
	Pen.CreatePen(PS_SOLID, 1, D_COLOR_BLACK);
	pOldPen = (CPen*)pDC->SelectObject(&Pen);
	pDC->MoveTo(nMargin, int(nHeight/2));
	pDC->LineTo(nWidth-nMargin, int(nHeight/2));

	pDC->MoveTo(nWidth-nMargin, int(nHeight/2));
	pDC->LineTo(nWidth-nMargin-10, int(nHeight/2)+5);
	pDC->MoveTo(nWidth-nMargin, int(nHeight/2));
	pDC->LineTo(nWidth-nMargin-10, int(nHeight/2)-5);

	pDC->SelectObject(pOldPen);
	Pen.DeleteObject();

	// Y axis(M)
	//CPen Pen,*pOldPen;
	Pen.CreatePen(PS_SOLID, 1, D_COLOR_BLACK);
	pOldPen = (CPen*)pDC->SelectObject(&Pen);
	pDC->MoveTo(int(nWidth/2), nMargin);
	pDC->LineTo(int(nWidth/2), nHeight-nMargin);

	pDC->MoveTo(int(nWidth/2), nMargin);
	pDC->LineTo(int(nWidth/2)-5, nMargin+10);
	pDC->MoveTo(int(nWidth/2), nMargin);
	pDC->LineTo(int(nWidth/2)+5, nMargin+10);

	pDC->SelectObject(pOldPen);
	Pen.DeleteObject();

	//Grid
	Pen.CreatePen(PS_SOLID, 1, D_COLOR_GRAY);
	pOldPen = (CPen*)pDC->SelectObject(&Pen);
	//X Grid
	int nRealHeight = nHeight - 2*nMargin;
	int nRealWidth = nWidth - 2*nMargin;

	//최외각 그리드
	pDC->MoveTo(nMargin, nMargin);
	pDC->LineTo(nWidth - nMargin, nMargin);
	pDC->MoveTo(nWidth - nMargin, nMargin);
	pDC->LineTo(nWidth - nMargin, nHeight - nMargin);
	pDC->MoveTo(nWidth - nMargin, nHeight - nMargin);
	pDC->LineTo(nMargin, nHeight - nMargin);
	pDC->MoveTo(nMargin, nHeight - nMargin);
	pDC->LineTo(nMargin, nMargin);

	for (int i=0; i<nGrid-1; i++)
	{
		pDC->MoveTo(nMargin,        int(nHeight/2 + nRealHeight/(2*nGrid)*(i+1)));
	  pDC->LineTo(nWidth-nMargin, int(nHeight/2 + nRealHeight/(2*nGrid)*(i+1)));
		pDC->MoveTo(nMargin,        int(nHeight/2 - nRealHeight/(2*nGrid)*(i+1)));
	  pDC->LineTo(nWidth-nMargin, int(nHeight/2 - nRealHeight/(2*nGrid)*(i+1)));
	}

	for (int i=0; i<nGrid-1; i++)
	{
		pDC->MoveTo(int(nWidth/2 + nRealWidth/(2*nGrid)*(i+1)), nMargin);
	  pDC->LineTo(int(nWidth/2 + nRealWidth/(2*nGrid)*(i+1)), nHeight-nMargin);
		pDC->MoveTo(int(nWidth/2 - nRealWidth/(2*nGrid)*(i+1)), nMargin);
	  pDC->LineTo(int(nWidth/2 - nRealWidth/(2*nGrid)*(i+1)), nHeight-nMargin);
	}
	pDC->SelectObject(pOldPen);
	Pen.DeleteObject();
}

void CIehpYieldPropGraph_MEC::ClearSurfaceBkgrnd(CDC* pDC)
{
	CBrush BackBrush(D_COLOR_WHITE);
	CBrush* pOldBrush = pDC->SelectObject(&BackBrush);
	CRect rect;
	GetClientRect(&rect);
	pDC->PatBlt(rect.left, rect.top, rect.Width(), rect.Height(), PATCOPY);
	pDC->SelectObject(pOldBrush);
	BackBrush.DeleteObject();

	// Draw Axis

	rect.DeflateRect(0,0,0,0);

	int nWidth = rect.Width();
	int nHeight = rect.Height();

	// X axis(P)
	CPen Pen,*pOldPen;
	Pen.CreatePen(PS_SOLID, 1, D_COLOR_GRAY);
	pOldPen = (CPen*)pDC->SelectObject(&Pen);
	pDC->MoveTo(8, int(nHeight/2));
	pDC->LineTo(nWidth-5, int(nHeight/2));
	pDC->SelectObject(pOldPen);
	Pen.DeleteObject();

	// Y axis(M)
	//CPen Pen,*pOldPen;
	Pen.CreatePen(PS_SOLID, 1, D_COLOR_GRAY);
	pOldPen = (CPen*)pDC->SelectObject(&Pen);
	pDC->MoveTo(8, nHeight-5);
	pDC->LineTo(8, 5);
	pDC->SelectObject(pOldPen);
	Pen.DeleteObject();

	// x Axis Label
	LOGFONT lf;
	CFont newFont, *pOldFont;
	
	pOldFont = pDC->GetCurrentFont();
	pOldFont->GetLogFont(&lf);
	lf.lfHeight = 12;
	lf.lfWidth = 6; 
	lf.lfWeight = FW_THIN;
	_tcscpy(lf.lfFaceName, _T("Arial"));
	
	/**
	lf.lfHeight = 0;
	lf.lfWidth = 0; 
	lf.lfEscapement = 0; 
	lf.lfOrientation = 0; 
	lf.lfWeight = FW_THIN; 
	lf.lfItalic = FALSE;
	lf.lfUnderline = FALSE; 
	lf.lfStrikeOut = FALSE;
	lf.lfCharSet = DEFAULT_CHARSET;
	lf.lfOutPrecision = OUT_CHARACTER_PRECIS;
	lf.lfClipPrecision = CLIP_CHARACTER_PRECIS;
	lf.lfQuality = DEFAULT_QUALITY;
	lf.lfPitchAndFamily = DEFAULT_PITCH | FF_DONTCARE;
	_tcscpy(lf.lfFaceName, _T("Arial"));
	**/

	newFont.CreateFontIndirect(&lf);
	pOldFont = (CFont*)pDC->SelectObject(&newFont);

	pDC->SetTextAlign(TA_LEFT | TA_TOP);

	if (m_nMethod == 2 || m_nMethod == 3)
	{
		pDC->TextOut(15, 10, _LS(IDS_CMD_IEHP_YIELD_COMPRESSION));
		pDC->TextOut(15, nHeight-15, _LS(IDS_CMD_IEHP_YIELD_TENSION));
		
		if (m_nMethod == 2)
			pDC->TextOut(nWidth-30, int(nHeight/2)-10, _LSX(My));
		else
			pDC->TextOut(nWidth-30, int(nHeight/2)-10, _LSX(Mz));
	}
	else if (m_nMethod == 4)
	{
		pDC->TextOut(15, 10, _LSX(My));
		pDC->TextOut(nWidth-30, int(nHeight/2)-10, _LSX(Mz));
	}

	pDC->SelectObject(pOldFont);
	newFont.DeleteObject();
}

double CIehpYieldPropGraph_MEC::CalcScale(CRect rect, double dX, double dY, double dZoomRat/*0.8*/)
{
	double dScale=0.0;
	double dScale1=0.0, dScale2=0.0;
	if(fabs(dX) > 0. && fabs(dY) > 0.)
	{
		dScale1 = (rect.Width() /dX) * dZoomRat;
		dScale2 = (rect.Height()/dY) * dZoomRat;
	}
	else if(fabs(dX) < 0. && fabs(dY) > 0.)
	{
		dScale2 = (rect.Height()/dY) * dZoomRat;
		dScale1 = dScale2;
	}
	else if(fabs(dX) > 0. && fabs(dY) < 0.)
	{
		dScale1 = (rect.Width() /dX) * dZoomRat;
		dScale2 = dScale1;
	}
	else
	{
		dScale1 = 0.0;
		dScale2 = 0.0;
	}
	dScale = min(dScale1, dScale2);

	return dScale;
}

CPoint CIehpYieldPropGraph_MEC::CalcOriginPoint(CRect rect, double dScale, double dX, double dY)
{
	CPoint orPoint;

	orPoint.x = long(rect.Width()/2.);
	orPoint.y = long(rect.Height()/2.);
	
	return orPoint;
}

void CIehpYieldPropGraph_MEC::DrawSurfaceGraph()
{
	CClientDC dc(this);
	ClearSurfaceBkgrnd(&dc);
	dc.SetBkMode(TRANSPARENT);
	CRect rect;
	GetClientRect(rect);

	double dMaxX=0., dMaxY=0.0, dMinX=0.0, dMinY=0.0;
	double dScaleX=0., dScaleY=0.;
	
	rect.DeflateRect(0,0,0,0);
	// Calculate Scale
	int nHeight = int(rect.Height()/2.);
	int nx0=0, ny0=0, nx1=0, ny1=0, nx2=0, ny2=0, nx3=0, ny3=0;
	if(m_nMethod == 2 || m_nMethod == 3) // P-My
	{
		double X[6];
		X[0] = m_dV_M1st[m_nMethod-2][5];
		X[1] = m_dV_M2nd[m_nMethod-2][5];
		X[2] = m_dV_M3rd[m_nMethod-2][5];
		X[3] = m_dV_M1st_aprx[m_nMethod-2][50];
		X[4] = m_dV_M2nd_aprx[m_nMethod-2][50];
		X[5] = m_dV_M3rd_aprx[m_nMethod-2][50];

		for(int i=0; i<6; i++)
		{
			if(i==0) dMaxX = fabs(X[i]);
			else
			{
				if(fabs(X[i]) - dMaxX > 0.) dMaxX = fabs(X[i]);
			}
		}
/*
		dMaxX = m_dV_M2nd[m_nMethod-2][5];
		if(m_dV_M1st[m_nMethod-2][5] <= dMaxX)
		{
			if(m_dV_M2nd_aprx[m_nMethod-2][50] > dMaxX) dMaxX = m_dV_M2nd_aprx[m_nMethod-2][50];
		}
		else
		{
			dMaxX = m_dV_M1st[m_nMethod-2][5];
			if(m_dV_M1st_aprx[m_nMethod-2][50] > dMaxX) dMaxX = m_dV_M1st_aprx[m_nMethod-2][50];
		}
*/
/*
		double X[2];
		for(int i=0; i<2; i++)
		{
			X[i] = m_dV_M2nd[i][5];
			if(m_dV_M1st[i][5] <= X[i])
			{
				if(m_dV_M2nd_aprx[i][50] > X[i]) X[i] = m_dV_M2nd_aprx[i][50];
			}
			else
			{
				X[i] = m_dV_M1st[i][5];
				if(m_dV_M1st_aprx[i][50] > X[i]) X[i] = m_dV_M1st_aprx[i][50];
			}
		}
		dMaxX = max(X[0], X[1]);
*/
		dScaleX = CalcScale(rect, dMaxX, dMaxX, 0.95);

		double Y[14];
		Y[0] = m_dV_P1st[m_nMethod-2][0];
		Y[1] = m_dV_P1st[m_nMethod-2][10];
		Y[2] = m_dV_P2nd[m_nMethod-2][0];
		Y[3] = m_dV_P2nd[m_nMethod-2][10];
		Y[4] = m_dV_P3rd[m_nMethod-2][0];
		Y[5] = m_dV_P3rd[m_nMethod-2][10];
		Y[6] = m_dV_P1st_aprx[m_nMethod-2][0];
		Y[7] = m_dV_P1st_aprx[m_nMethod-2][0];
		Y[8] = m_dV_P2nd_aprx[m_nMethod-2][100];
		Y[9] = m_dV_P2nd_aprx[m_nMethod-2][100];
		Y[10] = m_dV_P3rd_aprx[m_nMethod-2][100];
		Y[11] = m_dV_P3rd_aprx[m_nMethod-2][100];
		Y[12] = m_dV_Pcrst[m_nMethod-2][0];
		Y[13] = m_dV_Pcrst[m_nMethod-2][10];
		for(int i=0; i<14; i++)
		{
			if(i==0) dMaxY = fabs(Y[i]);
			else
			{
				if(fabs(Y[i]) - dMaxY > 0.) dMaxY = fabs(Y[i]);
			}
		} 
 
		dScaleY = CalcScale(rect, dMaxY, dMaxY, 0.95) /2.;
		//dScaleX = CalcScale(rect, dMaxX, dMaxY, 0.9);
		//dScaleY = dScaleX/2.;

		// Interaction Curve
		for(int i=0; i<11; i++)
		{
			CPen Pen,*pOldPen;

			// Crack
			if(m_bDrawCrack)
			{
				Pen.CreatePen(PS_SOLID, 1, D_COLOR_MAGNT);
				pOldPen = (CPen*)dc.SelectObject(&Pen);

				if(i > 0) dc.MoveTo(8 + nx0, nHeight - ny0);
				nx0 = int(m_dV_Mcrst[m_nMethod - 2][i] * dScaleX);
				ny0 = int(m_dV_Pcrst[m_nMethod - 2][i] * dScaleY);
				if(i == 0) dc.MoveTo(8 + nx0, nHeight - ny0);
				dc.LineTo(8 + nx0, nHeight - ny0);

				dc.SelectObject(pOldPen);
				Pen.DeleteObject();
			}

			// 1st
			if(m_bDraw1stCurve)
			{
				Pen.CreatePen(PS_SOLID, 1, D_COLOR_BLUE);
				pOldPen = (CPen*)dc.SelectObject(&Pen);

				if(i>0) dc.MoveTo(8+nx1, nHeight - ny1);
				nx1 = int(m_dV_M1st[m_nMethod-2][i]*dScaleX);
				ny1 = int(m_dV_P1st[m_nMethod-2][i]*dScaleY);
				if(i==0) dc.MoveTo(8+nx1, nHeight - ny1);
				dc.LineTo(8+nx1, nHeight - ny1);

				dc.SelectObject(pOldPen);
				Pen.DeleteObject();
			}

			// 2nd
			Pen.CreatePen(PS_SOLID, 1, D_COLOR_GREEN);
			pOldPen = (CPen*)dc.SelectObject(&Pen);

			if(i>0) dc.MoveTo(8+nx2, nHeight - ny2);
			nx2 = int(m_dV_M2nd[m_nMethod-2][i]*dScaleX);
			ny2 = int(m_dV_P2nd[m_nMethod-2][i]*dScaleY);
			if(i==0) dc.MoveTo(8+nx2, nHeight - ny2);
			dc.LineTo(8+nx2, nHeight - ny2);

			dc.SelectObject(pOldPen);
			Pen.DeleteObject();

			// 3rd
			if (m_bDraw3rdCurve)
			{
				Pen.CreatePen(PS_SOLID, 1, D_COLOR_RED);
				pOldPen = (CPen*)dc.SelectObject(&Pen);

				if(i>0) dc.MoveTo(8+nx3, nHeight - ny3);
				nx3 = int(m_dV_M3rd[m_nMethod-2][i]*dScaleX);
				ny3 = int(m_dV_P3rd[m_nMethod-2][i]*dScaleY);
				if(i==0) dc.MoveTo(8+nx3, nHeight - ny3);
				dc.LineTo(8+nx3, nHeight - ny3);

				dc.SelectObject(pOldPen);
				Pen.DeleteObject();
			}
		}

		// Approximate Interaction Curve
		for(int i=0; i<101; i++)
		{
			CPen Pen,*pOldPen;
			// 1st
			if(m_bDraw1stCurve)
			{
				Pen.CreatePen(PS_SOLID, 2, D_COLOR_BLUE);
				pOldPen = (CPen*)dc.SelectObject(&Pen);
				if(i>0) dc.MoveTo(8+nx1, nHeight - ny1);
				nx1 = int(m_dV_M1st_aprx[m_nMethod-2][i]*dScaleX);
				ny1 = int(m_dV_P1st_aprx[m_nMethod-2][i]*dScaleY);
				if(i==0) dc.MoveTo(8+nx1, nHeight - ny1);
				dc.LineTo(8+nx1, nHeight - ny1);

				dc.SelectObject(pOldPen);
				Pen.DeleteObject();
			}

			// 2nd
			Pen.CreatePen(PS_SOLID, 2, D_COLOR_GREEN);
			pOldPen = (CPen*)dc.SelectObject(&Pen);

			if(i>0) dc.MoveTo(8+nx2, nHeight - ny2);
			nx2 = int(m_dV_M2nd_aprx[m_nMethod-2][i]*dScaleX);
			ny2 = int(m_dV_P2nd_aprx[m_nMethod-2][i]*dScaleY);
			if(i==0) dc.MoveTo(8+nx2, nHeight - ny2);
			dc.LineTo(8+nx2, nHeight - ny2);

			dc.SelectObject(pOldPen);
			Pen.DeleteObject();

			// 3rd
			if(m_bDraw3rdCurve)
			{
				Pen.CreatePen(PS_SOLID, 2, D_COLOR_RED);
				pOldPen = (CPen*)dc.SelectObject(&Pen);
				if(i>0) dc.MoveTo(8+nx3, nHeight - ny3);
				nx3 = int(m_dV_M3rd_aprx[m_nMethod-2][i]*dScaleX);
				ny3 = int(m_dV_P3rd_aprx[m_nMethod-2][i]*dScaleY);
				if(i==0) dc.MoveTo(8+nx3, nHeight - ny3);
				dc.LineTo(8+nx3, nHeight - ny3);

				dc.SelectObject(pOldPen);
				Pen.DeleteObject();
			}
		}
	}
	else if(m_nMethod == 4) // My-Mz
	{
		dMaxX = m_dV_MM2nd_aprx[0][50];
		dMaxY = m_dV_MM2nd_aprx[1][0];

		dScaleX = CalcScale(rect, dMaxX, dMaxX, 0.95);
		dScaleY = CalcScale(rect, dMaxY, dMaxY, 0.95)/2.;

		for(int i=0; i<51; i++)
		{
			CPen Pen,*pOldPen;
			// 1st
			Pen.CreatePen(PS_SOLID, 2, D_COLOR_BLUE);
			pOldPen = (CPen*)dc.SelectObject(&Pen);
			if(i>0) dc.MoveTo(8+nx1, nHeight - ny1);
			nx1 = int(m_dV_MM1st_aprx[0][i]*dScaleX);
			ny1 = int(m_dV_MM1st_aprx[1][i]*dScaleY);
			if(i==0) dc.MoveTo(8+nx1, nHeight - ny1);
			dc.LineTo(8+nx1, nHeight - ny1);

			dc.SelectObject(pOldPen);
			Pen.DeleteObject();

			// 2nd
			Pen.CreatePen(PS_SOLID, 2, D_COLOR_GREEN);
			pOldPen = (CPen*)dc.SelectObject(&Pen);

			if(i>0) dc.MoveTo(8+nx2, nHeight - ny2);
			nx2 = int(m_dV_MM2nd_aprx[0][i]*dScaleX);
			ny2 = int(m_dV_MM2nd_aprx[1][i]*dScaleY);
			if(i==0) dc.MoveTo(8+nx2, nHeight - ny2);
			dc.LineTo(8+nx2, nHeight - ny2);

			dc.SelectObject(pOldPen);
			Pen.DeleteObject();

			// 3rd
			if (m_bDraw3rdCurve)
			{
				Pen.CreatePen(PS_SOLID, 2, D_COLOR_RED);
				pOldPen = (CPen*)dc.SelectObject(&Pen);

				if(i>0) dc.MoveTo(8+nx3, nHeight - ny3);
				nx3 = int(m_dV_MM2nd_aprx[0][i]*dScaleX);
				ny3 = int(m_dV_MM2nd_aprx[1][i]*dScaleY);
				if(i==0) dc.MoveTo(8+nx3, nHeight - ny3);
				dc.LineTo(8+nx3, nHeight - ny3);

				dc.SelectObject(pOldPen);
				Pen.DeleteObject();
			}

		}
	}
	else ASSERT(0);
}

void CIehpYieldPropGraph_MEC::DrawStrengthGraph()
{
	CClientDC dc(this);
	ClearStrengthBkgrnd(&dc);
	dc.SetBkMode(TRANSPARENT);
	CRect rect;
	GetClientRect(rect);

	double dMaxX=0.0, dMaxY=0.0, dMinX=0.0, dMinY=0.0;
	int ar=3;
	if     (m_nMethod == -1) ar = 4;  // [Tetralinear]
	else if(m_nMethod ==  0) ar = 3;  // [Trilinear]  
	else if(m_nMethod ==  1) ar = 2;  // [Bilinear]
	else if(m_nMethod == 11) ar = 3;  // SLIP Bilinear
	else if(m_nMethod == 12) ar = 4;  // SLIP Trilinear
		
	double dDistX = m_dx[ar] - m_dx[ar*2];
	double dDistY = m_dy[ar] - m_dy[ar*2];
	
	// Calculate Scale
	double dScale = CalcScale(rect, 4., 4., 0.95);

	// Calculate Starting Point.
	CPoint orPoint = CalcOriginPoint(rect, dScale, dDistX, dDistY);

	//rect.DeflateRect(0,0,0,0);

	int nx=0., ny=0.;
	for(int i=1; i<9; i++)
	{
		CPen Pen,*pOldPen;
	  Pen.CreatePen(PS_SOLID, 2, D_COLOR_BLUE);
	  pOldPen = (CPen*)dc.SelectObject(&Pen);

		if(m_nMethod==-1 || m_nMethod==12)  // [Tetralinear]
		{
			if(i==1 || i==5) dc.MoveTo(orPoint.x,      orPoint.y);
			else             dc.MoveTo(orPoint.x + nx, orPoint.y - ny);
		}
		else if(m_nMethod==0 || m_nMethod==11)  // [Trilinear]
		{
			if     (i==1 || i==4) dc.MoveTo(orPoint.x,      orPoint.y);
			else if(i==2 || i==5) dc.MoveTo(orPoint.x + nx, orPoint.y - ny);
			else if(i==3 || i==6) dc.MoveTo(orPoint.x + nx, orPoint.y - ny);
			else break;
		}
		else if(m_nMethod==1)  // [Bilinear]
		{
			if     (i==1 || i==3) dc.MoveTo(orPoint.x,      orPoint.y);
			else if(i==2 || i==4) dc.MoveTo(orPoint.x + nx, orPoint.y - ny);
			else break;
		}

		nx = int(m_dx[i]*dScale);
		ny = int(m_dy[i]*dScale);
		dc.LineTo(orPoint.x + nx, orPoint.y - ny);

		dc.SelectObject(pOldPen);
	  Pen.DeleteObject();
	}

	if(m_nMethod==11 || m_nMethod==12)
	{
		CPen Pen,*pOldPen;
	  Pen.CreatePen(PS_SOLID, 2, D_COLOR_BLUE);
	  pOldPen = (CPen*)dc.SelectObject(&Pen);

		if(m_nMethod==11) { nx = int(m_dx[3]*dScale), ny = int(m_dy[3]*dScale); }
		else              { nx = int(m_dx[4]*dScale), ny = int(m_dy[4]*dScale); }
		dc.MoveTo(orPoint.x+nx, orPoint.y-ny);
		nx = int(m_dOp*dScale);
		dc.LineTo(orPoint.x+nx, orPoint.y);
		nx = int(m_dOm*dScale);
		dc.LineTo(orPoint.x+nx, orPoint.y);
		if(m_nMethod==11) { nx = int(m_dx[6]*dScale), ny = int(m_dy[6]*dScale); }
		else              { nx = int(m_dx[8]*dScale), ny = int(m_dy[8]*dScale); }
		dc.LineTo(orPoint.x+nx, orPoint.y-ny);

		dc.SelectObject(pOldPen);
	  Pen.DeleteObject();
	}
	
}

void CIehpYieldPropGraph_MEC::DrawMultiLinearGraph()
{
	CClientDC dc(this);
	ClearStrengthBkgrnd(&dc);
	dc.SetBkMode(TRANSPARENT);
	CRect rect;
	GetClientRect(rect);

	int nCount = m_aMultiLinearData.GetSize();

	if(nCount < 1) return;

	double dMinX = 0.0, dMaxX = 0.0; 
	double dMinY = 0.0, dMaxY = 0.0;
	for (int i=0; i<nCount; i++)
	{
		dMinX = min(dMinX, m_aMultiLinearData[i].dblDisp);
		dMaxX = max(dMaxX, m_aMultiLinearData[i].dblDisp);

		dMinY = min(dMinY, m_aMultiLinearData[i].dblForce);
		dMaxY = max(dMaxY, m_aMultiLinearData[i].dblForce);
	}
	double dDistX = fabs(dMaxX - dMinX);
	double dDistY = fabs(dMaxY - dMinY);

	if(m_bPmmc)
	{
		if((dMaxX == 0.0 && dMaxY == 0.0) ||  (dMinX == 0.0 && dMinY == 0.0))
		{
			dDistX = fabs(dMaxX - dMinX);
			dDistY = fabs(dMaxY - dMinY);
		}
		else
		{
			dDistX = 2.0*max(fabs(dMaxX), fabs(dMinX));
			dDistY = 2.0*max(fabs(dMaxY), fabs(dMinY));
		}
	}

	if(dMinX == 0 || dMaxX == 0) dDistX = 2*dDistX;
	if(dMinY == 0 || dMaxY == 0) dDistY = 2*dDistY;

	// Calculate Scale
	double dZoomRat = 0.95;
	double dScaleX=0.0, dScaleY=0.0;
	if(fabs(dDistX) > 0. && fabs(dDistY) > 0.)
	{
		dScaleX = (rect.Width() /dDistX) * dZoomRat;
		dScaleY = (rect.Height()/dDistY) * dZoomRat;
	}
	else if(fabs(dDistX) > 0.)
	{
		dScaleX = (rect.Width() /dDistX) * dZoomRat;
		dScaleY = (rect.Height()/dDistX) * dZoomRat;
	}
	else if(fabs(dDistY) > 0.)
	{
		dScaleX = (rect.Width() /dDistY) * dZoomRat;
		dScaleY = (rect.Height()/dDistY) * dZoomRat;
	}
	else
	{
		//ASSERT(0);
		return;
	}


	
	// Calculate Starting Point.
	CPoint orPoint = CalcOriginPoint(rect, dScaleX, dDistX, dDistY);
	
	//rect.DeflateRect(0,0,0,0);
	
	int nx=0., ny=0.;
	for(int i=0; i<nCount-1; i++)
	{
		CPen Pen,*pOldPen;
		Pen.CreatePen(PS_SOLID, 2, D_COLOR_BLUE);
		pOldPen = (CPen*)dc.SelectObject(&Pen);
		
		nx = int(m_aMultiLinearData[i].dblDisp *dScaleX);
		ny = int(m_aMultiLinearData[i].dblForce*dScaleY);


		dc.MoveTo(orPoint.x + nx, orPoint.y - ny);

		
		
		nx = int(m_aMultiLinearData[i+1].dblDisp *dScaleX);
		ny = int(m_aMultiLinearData[i+1].dblForce*dScaleY);
		dc.LineTo(orPoint.x + nx, orPoint.y - ny);
		
		dc.SelectObject(pOldPen);
		Pen.DeleteObject();
	}
}

void CIehpYieldPropGraph_MEC::DrawText(CDC* pDC, CString str, double dx, double dy)
{
	
}

void CIehpYieldPropGraph_MEC::SetDefaultData(int nMethod, double dx[], double dy[], double dOp, double dOm)
{
	m_nMethod = nMethod;
	for(int i=0; i<9; i++)
	{
		m_dx[i] = dx[i];
		m_dy[i] = dy[i];
	}
	m_dOp = dOp;
	m_dOm = dOm;
}
/*
void CIehpYieldPropGraph_MEC::SetDefaultData(int nMethod, int nMat, double dPC0, double dMC0[],
																				 double dPC[], double dPCB[], double dMC[], double dPY[],
																				 double dPYB[], double dMY[], double dBetay1st[],
																				 double dBetaz1st[], double dBetay2nd[], double dBetaz2nd[],
																				 double dGamma1st, double dGamma2nd, double dAlpha,
																				 double dP1st[][11], double dM1st[][11],
																				 double dP2nd[][11], double dM2nd[][11])
{
	m_nMethod = nMethod;
	
}
*/
void CIehpYieldPropGraph_MEC::SetDefaultData(int nMethod, double dV_P1st[][11], double dV_M1st[][11],
																				 double dV_P2nd[][11], double dV_M2nd[][11], 
																				 double dV_P3rd[][11], double dV_M3rd[][11],
																				 double dV_P1st_aprx[][101], double dV_M1st_aprx[][101], 
																				 double dV_P2nd_aprx[][101], double dV_M2nd_aprx[][101], 
																				 double dV_P3rd_aprx[][101], double dV_M3rd_aprx[][101], 
																				 double dV_MM1st_aprx[][51], double dV_MM2nd_aprx[][51], double dV_MM3rd_aprx[][51])
{
	m_nMethod = nMethod;
	for(int i=0; i<2; i++)
	{
		for(int j=0; j<11; j++)
		{
			m_dV_P1st[i][j] = dV_P1st[i][j];
			m_dV_M1st[i][j] = dV_M1st[i][j];
			m_dV_P2nd[i][j] = dV_P2nd[i][j];
			m_dV_M2nd[i][j] = dV_M2nd[i][j];
			m_dV_P3rd[i][j] = dV_P3rd[i][j];
			m_dV_M3rd[i][j] = dV_M3rd[i][j];
		}
		for(int j=0; j<101; j++)
		{
			m_dV_P1st_aprx[i][j] = dV_P1st_aprx[i][j];
			m_dV_M1st_aprx[i][j] = dV_M1st_aprx[i][j];
			m_dV_P2nd_aprx[i][j] = dV_P2nd_aprx[i][j];
			m_dV_M2nd_aprx[i][j] = dV_M2nd_aprx[i][j];
			m_dV_P3rd_aprx[i][j] = dV_P3rd_aprx[i][j];
			m_dV_M3rd_aprx[i][j] = dV_M3rd_aprx[i][j];
		}
		for(int j=0; j<51; j++)
		{
			m_dV_MM1st_aprx[i][j] = dV_MM1st_aprx[i][j];
			m_dV_MM2nd_aprx[i][j] = dV_MM2nd_aprx[i][j];
			m_dV_MM3rd_aprx[i][j] = dV_MM3rd_aprx[i][j];
		}
	}
	
	
}

void CIehpYieldPropGraph_MEC::SetCrackData(double dV_Pcrst[][11], double dV_Mcrst[][11])
{
	m_bDrawCrack = true;
	memcpy(m_dV_Pcrst, dV_Pcrst, sizeof(m_dV_Pcrst));
	memcpy(m_dV_Mcrst, dV_Mcrst, sizeof(m_dV_Mcrst));
}

void CIehpYieldPropGraph_MEC::SetDefaultData(int nMethod, T_IEHP_DT& arMultiData, BOOL bPmmc)
{
	m_bPmmc = bPmmc;
	m_nMethod = nMethod;
	m_aMultiLinearData.Copy(arMultiData);
}

void CIehpYieldPropGraph_MEC::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here

	switch(m_nMethod)
	{
	case -1: case 0: case 1: case 11: case 12:
		DrawStrengthGraph();
		break;
	case 2: case 3: case 4:
		DrawSurfaceGraph();
		break;
	case 20:
		DrawMultiLinearGraph();
	default: break;
	}

	// Do not call CWnd::OnPaint() for painting messages
}