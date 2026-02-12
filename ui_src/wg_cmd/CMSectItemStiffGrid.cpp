// CMSectItemStiffGrid.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMSectItemStiffGrid.h"

#include "CMSectItemPSCValue.h"
#include "CMSectItmePSCView.h"

#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_SectUtil.h"
#include "..\wg_db\wg_db_SectDB.h"


#define D_STIFF_MAX_NUM 24
#define D_STIFF_WARPING_MAX_NUM 16
#define D_TOTAL_ROW_NUM (2+D_STIFF_MAX_NUM+D_STIFF_WARPING_MAX_NUM)

#define D_STIFF_START_ROW 3

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCMSectItemStiffGrid
CCMSectItemStiffGrid::CCMSectItemStiffGrid()
{  
	m_pSectBase = 0;
	m_bInitDone = FALSE;
	m_bConsiderWarpingEffect = FALSE;
}

CCMSectItemStiffGrid::~CCMSectItemStiffGrid()
{
}

//---------------------------------------------------------------------------
// Public Interface Functions
void CCMSectItemStiffGrid::InitGrid()
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

void CCMSectItemStiffGrid::SetStyleOnSizeCell()
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	int i;

	// Stiffness 전체 : 소수 넷째자리까지 표현
	for (i = 0; i < D_STIFF_MAX_NUM; i++)
	{
		SetStyleRange(CGXRange(D_STIFF_START_ROW+i, 2), CGXStyle()
			.SetFormat(GX_FMT_FIXED).SetPlaces(4));
	}

	// Area, Asy, Asz, Ixx, Iyy, Izz : 소수 다섯째 자리 지수 표현
	for (i = 0; i < 6; i++)
	{
		SetStyleRange(CGXRange(D_STIFF_START_ROW+i, 2), CGXStyle()
			.SetFormat(GX_FMT_FLOAT).SetPlaces(5));
	}

	// Peri:O, Peri:I : 소수 다섯째 자리 지수 표현
	for (i = 0; i < 2; i++)
	{
		SetStyleRange(CGXRange(D_STIFF_START_ROW+12+i, 2), CGXStyle()
			.SetFormat(GX_FMT_FLOAT).SetPlaces(5));
	}

	for (i = 0; i < D_STIFF_WARPING_MAX_NUM; i++)
	{
		SetStyleRange(CGXRange(D_STIFF_START_ROW+D_STIFF_MAX_NUM+i, 2), CGXStyle()
			.SetFormat(GX_FMT_FIXED).SetPlaces(4));
	}
}

void CCMSectItemStiffGrid::SetDataSource(CWnd* pParent,T_SECT_SECTBASE_D* pSectBase)
{

	m_pParent = NULL;
	ASSERT(pParent);
	m_pParent = (CCMSectItemPSCValue*)pParent;

	
	ASSERT(pSectBase);
	m_pSectBase = pSectBase;


	BOOL bOldLock = LockUpdate(TRUE);
	BOOL bOldReadOnlyLock = GetParam()->IsLockReadOnly();
	GetParam()->SetLockReadOnly(FALSE);
		
	SetRowCount(D_TOTAL_ROW_NUM);
	SetColCount(3);

	// set column width
	SetColWidthByDPI(1, 1, 60);
	SetColWidthByDPI(2, 2, 100);
	SetColWidthByDPI(3, 3, 60);

	SetCoveredCellsRowCol(D_STIFF_START_ROW-2, 1, D_STIFF_START_ROW-2, 3);
	SetStyleRange(CGXRange(D_STIFF_START_ROW-2, 1), CGXStyle()
		.SetInterior(COLORREF(RGB(0, 0, 255)))
		.SetTextColor(COLORREF(RGB(255, 255, 0)))
		.SetValue(_LS(IDS_WG_CMD__ADDD__Stiffness)));
	SetCoveredCellsRowCol(D_STIFF_START_ROW-1, 1, D_STIFF_START_ROW-1, 3);
	SetStyleRange(CGXRange(D_STIFF_START_ROW-1, 1), CGXStyle()
		.SetControl(GX_IDS_CTRL_PUSHBTN)
			.SetInterior(GXSYSCOLOR(COLOR_BTNFACE))
			.SetChoiceList(_LS(IDS_WG_CMD__ADDD__Calculate_Stiffness))
			.SetTextColor(COLORREF(RGB(0, 0, 0)))
			.SetEnabled(TRUE));


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

	SetStyleOnSizeCell();
 
	CString aStiffTitle[] = {
		_LS(IDS_WG_CMD__ADDD__Area), _LS(IDS_WG_CMD__ADDD__Asy), _LS(IDS_WG_CMD__ADDD__Asz), _LS(IDS_WG_CMD__ADDD__Ixx), _LS(IDS_WG_CMD__ADDD__Iyy), _LS(IDS_WG_CMD__ADDD__Izz),  
		_LS(IDS_WG_CMD__ADDD__Cyp), _LS(IDS_WG_CMD__ADDD__Cym), _LS(IDS_WG_CMD__ADDD__Czp), _LS(IDS_WG_CMD__ADDD__Czm), _LS(IDS_WG_CMD__ADDD__Qyb), _LS(IDS_WG_CMD__ADDD__Qzb), 
		_LS(IDS_WG_CMD__ADDD__PeriOut), _LS(IDS_WG_CMD__ADDD__PeriIn), 
		_LS(IDS_WG_CMD__ADDD__Cent_Y), _LS(IDS_WG_CMD__ADDD__Cent_Z),
		_T("y1"), _T("z1"), _T("y2"), _T("z2"), _T("y3"), _T("z3"), _T("y4"), _T("z4"),
	};
	T_SECT_STIFFNESS_UNIT* pUnit = &CUnitCtrl::m_SECT_UNIT.SectBefore.SectI.Stiffness;
	int aStiffUnit[] = { 
		pUnit->Area, pUnit->Asy, pUnit->Asz, pUnit->Rxx, pUnit->Ryy, pUnit->Rzz,
		pUnit->Cyp, pUnit->Cym, pUnit->Czp, pUnit->Czm, pUnit->Qyb, pUnit->Qzb,
		D_UNITSYS_BASE_LENGTH, D_UNITSYS_BASE_LENGTH,
		D_UNITSYS_BASE_LENGTH, D_UNITSYS_BASE_LENGTH,
		D_UNITSYS_BASE_LENGTH, D_UNITSYS_BASE_LENGTH, D_UNITSYS_BASE_LENGTH, D_UNITSYS_BASE_LENGTH, 
		D_UNITSYS_BASE_LENGTH, D_UNITSYS_BASE_LENGTH, D_UNITSYS_BASE_LENGTH, D_UNITSYS_BASE_LENGTH,
	};
	CString csUnit;
	for (int i = 0; i < D_STIFF_MAX_NUM; i++)
	{
		CUnitCtrl::GetUnitSystem(aStiffUnit[i], csUnit);
		SetValueRange(CGXRange(D_STIFF_START_ROW+i, 1), aStiffTitle[i]);
		SetValueRange(CGXRange(D_STIFF_START_ROW+i, 3), csUnit);
	}

	CString aStiffWarpingTitle[] = {_T("Iw"), _T("w1"), _T("w2"), _T("w3"), _T("w4"), _T("Cxy1"), _T("Cxy2"), _T("Cxy3"), _T("Cxy4"),_T("Cxz1"), _T("Cxz2"), _T("Cxz3"), _T("Cxz4"), _T("ys-yc"), _T("zs-zc"), _T("Ip")};
	int aStiffWarpingUnit[] = { pUnit->dIw, pUnit->dw1, pUnit->dw2, pUnit->dw3, pUnit->dw4,
			pUnit->dxy1, pUnit->dxy2, pUnit->dxy3, pUnit->dxy4, pUnit->dxz1, pUnit->dxz2, pUnit->dxz3, pUnit->dxz4, pUnit->dYsy, pUnit->dYsz, pUnit->dIp};
		
	for (int i = 0; i < D_STIFF_WARPING_MAX_NUM; i++)
	{
		CUnitCtrl::GetUnitSystem(aStiffWarpingUnit[i], csUnit);
		SetValueRange(CGXRange(D_STIFF_START_ROW+D_STIFF_MAX_NUM+i, 1), aStiffWarpingTitle[i]);
		SetValueRange(CGXRange(D_STIFF_START_ROW+D_STIFF_MAX_NUM+i, 3), csUnit);
	}
		
	HideRows(D_STIFF_START_ROW+D_STIFF_MAX_NUM, D_STIFF_START_ROW+D_STIFF_MAX_NUM+D_STIFF_WARPING_MAX_NUM, m_bConsiderWarpingEffect==FALSE);

	GetParam()->SetLockReadOnly(bOldReadOnlyLock);

	LockUpdate(bOldLock);
	if (!bOldLock) Redraw();

	SetRightValidCell(1, 1);
}

void CCMSectItemStiffGrid::SetWarpingEffect(BOOL bConsiderWarpingEffect)
{
	m_bConsiderWarpingEffect = bConsiderWarpingEffect;
	HideRows(D_STIFF_START_ROW+D_STIFF_MAX_NUM, D_STIFF_START_ROW+D_STIFF_MAX_NUM+D_STIFF_WARPING_MAX_NUM, m_bConsiderWarpingEffect==FALSE);
	ShowData();
}

void CCMSectItemStiffGrid::ShowData()
{
	double *pValue1[16], *pValue2[8];

	CSectUtil::MakeMapStiffVar(&m_pSectBase->Stiffness, pValue1);
	CSectUtil::MakeMapStiffVar2(&m_pSectBase->Stiffness, pValue2);
	pValue1[12] = &m_pSectBase->PeriOut;
	pValue1[13] = &m_pSectBase->PeriIn;
	pValue1[14] = &m_pSectBase->Design.YBar;
	pValue1[15] = &m_pSectBase->Design.ZBar;

	int i;
	for (i = 0; i < 16; i++)
		SetValueRange(CGXRange(D_STIFF_START_ROW+i, 2), *pValue1[i]);
	for (i = 16; i < D_STIFF_MAX_NUM; i++)
		SetValueRange(CGXRange(D_STIFF_START_ROW+i, 2), *pValue2[i-16]);

	if(m_bConsiderWarpingEffect)
	{
		double *pValue3[16];
		CSectUtil::MakeMapStiffVar3(&m_pSectBase->Stiffness, pValue3);
		for(i = D_STIFF_MAX_NUM; i<D_STIFF_MAX_NUM+D_STIFF_WARPING_MAX_NUM; i++)
			SetValueRange(CGXRange(D_STIFF_START_ROW+i, 2), *pValue3[i-D_STIFF_MAX_NUM]);
	}
}

BOOL CCMSectItemStiffGrid::SaveData()
{
	CString csNum;
	double dNum;

	//double *pValue[D_STIFF_MAX_NUM];
	double *pValue1[16], *pValue2[8];
	CSectUtil::MakeMapStiffVar(&m_pSectBase->Stiffness, pValue1);
	CSectUtil::MakeMapStiffVar2(&m_pSectBase->Stiffness, pValue2);
	pValue1[12] = &m_pSectBase->PeriOut;
	pValue1[13] = &m_pSectBase->PeriIn;
	pValue1[14] = &m_pSectBase->Design.YBar;
	pValue1[15] = &m_pSectBase->Design.ZBar;
	for (int i = 0; i < 16; i++)
	{
		csNum = GetEditingValue(D_STIFF_START_ROW+i, 2);
		if (GetDoubleValue(csNum, dNum))  *pValue1[i] = dNum;
		else                              *pValue1[i] = 0.0;
	}
	for (int i = 16; i < D_STIFF_MAX_NUM; i++)
	{
		csNum = GetEditingValue(D_STIFF_START_ROW+i, 2);
		if (GetDoubleValue(csNum, dNum))  *pValue2[i-16] = dNum;
		else                              *pValue2[i-16] = 0.0;
	}

	if(m_bConsiderWarpingEffect)
	{
		double *pValue3[16];
		CSectUtil::MakeMapStiffVar3(&m_pSectBase->Stiffness, pValue3);
		
		for(int i = D_STIFF_MAX_NUM; i<D_STIFF_MAX_NUM+D_STIFF_WARPING_MAX_NUM; i++)
		{
			csNum = GetEditingValue(D_STIFF_START_ROW+i, 2);
			if (GetDoubleValue(csNum, dNum)) *pValue3[i-D_STIFF_MAX_NUM] = dNum;
			else                             *pValue3[i-D_STIFF_MAX_NUM] = 0.0;
		}
	}

	return TRUE;
}

void CCMSectItemStiffGrid::OnSizeChange()
{
	// 임시(다시 그려주기)
	CCMSectItemPSCValue *pParent = (CCMSectItemPSCValue*)GetParent();
	if (pParent != 0)
	{
		CCMSectItmePSCView *pView = pParent->GetViewPointer();
		if (pView != 0) pView->RedrawSection(FALSE);
	}
}

//---------------------------------------------------------------------------
// Implementation Function
BOOL CCMSectItemStiffGrid::SetLeftValidCell(ROWCOL nRow, ROWCOL nCol)
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

BOOL CCMSectItemStiffGrid::SetRightValidCell(ROWCOL nRow, ROWCOL nCol)
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

CString CCMSectItemStiffGrid::GetEditingValue(ROWCOL nRow, ROWCOL nCol)
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

BEGIN_MESSAGE_MAP(CCMSectItemStiffGrid, CGXGridWnd)
	//{{AFX_MSG_MAP(CCMSectItemStiffGrid)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CCMSectItemStiffGrid message handlers
/////////////////////////////////////////////////////////////////////////////
BOOL CCMSectItemStiffGrid::ProcessKeys(CWnd *pSender, UINT nMessage, UINT nChar, UINT nRepCnt, UINT flags)
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

BOOL CCMSectItemStiffGrid::OnValidateCell(ROWCOL nRow, ROWCOL nCol)
{
	CString csNum = GetEditingValue(nRow, nCol);
	double dNum;
	if (GetDoubleValue(csNum, dNum)) return TRUE;
	return FALSE;
}

BOOL CCMSectItemStiffGrid::OnActivateGrid(BOOL bActivate)
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

void CCMSectItemStiffGrid::OnModifyCell(ROWCOL nRow, ROWCOL nCol)
{
	CGXGridWnd::OnModifyCell(nRow, nCol);

	// stress position
	if (nRow >= D_STIFF_START_ROW+D_STIFF_MAX_NUM-8 && nRow <= D_STIFF_START_ROW+D_STIFF_MAX_NUM-1)
	{
		CString csNum = GetEditingValue(nRow, nCol);
		double dNum;
		if (!GetDoubleValue(csNum, dNum)) dNum = 0.0;

		if (nRow == D_STIFF_START_ROW+D_STIFF_MAX_NUM-8)      m_pSectBase->Stiffness.dx1 = dNum;
		else if (nRow == D_STIFF_START_ROW+D_STIFF_MAX_NUM-7) m_pSectBase->Stiffness.dy1 = dNum;
		else if (nRow == D_STIFF_START_ROW+D_STIFF_MAX_NUM-6) m_pSectBase->Stiffness.dx2 = dNum;
		else if (nRow == D_STIFF_START_ROW+D_STIFF_MAX_NUM-5) m_pSectBase->Stiffness.dy2 = dNum;
		else if (nRow == D_STIFF_START_ROW+D_STIFF_MAX_NUM-4) m_pSectBase->Stiffness.dx3 = dNum;
		else if (nRow == D_STIFF_START_ROW+D_STIFF_MAX_NUM-3) m_pSectBase->Stiffness.dy3 = dNum;
		else if (nRow == D_STIFF_START_ROW+D_STIFF_MAX_NUM-2) m_pSectBase->Stiffness.dx4 = dNum;
		else if (nRow == D_STIFF_START_ROW+D_STIFF_MAX_NUM-1) m_pSectBase->Stiffness.dy4 = dNum;

		OnSizeChange();
	}
	else if (nRow >= D_STIFF_START_ROW+D_STIFF_MAX_NUM-1)
	{
		CString csNum = GetEditingValue(nRow, nCol);
		double dNum;
		if (!GetDoubleValue(csNum, dNum)) dNum = 0.0;
		
		if (nRow == D_STIFF_START_ROW+D_STIFF_MAX_NUM+0) m_pSectBase->Stiffness.dIw = dNum;
		else if (nRow == D_STIFF_START_ROW+D_STIFF_MAX_NUM+1) m_pSectBase->Stiffness.dw1 = dNum;
		else if (nRow == D_STIFF_START_ROW+D_STIFF_MAX_NUM+2) m_pSectBase->Stiffness.dw2 = dNum;
		else if (nRow == D_STIFF_START_ROW+D_STIFF_MAX_NUM+3) m_pSectBase->Stiffness.dw3 = dNum;
		else if (nRow == D_STIFF_START_ROW+D_STIFF_MAX_NUM+4) m_pSectBase->Stiffness.dw4 = dNum;
		else if (nRow == D_STIFF_START_ROW+D_STIFF_MAX_NUM+5) m_pSectBase->Stiffness.dxy1 = dNum;
		else if (nRow == D_STIFF_START_ROW+D_STIFF_MAX_NUM+6) m_pSectBase->Stiffness.dxy2 = dNum;
		else if (nRow == D_STIFF_START_ROW+D_STIFF_MAX_NUM+7) m_pSectBase->Stiffness.dxy3 = dNum;
		else if (nRow == D_STIFF_START_ROW+D_STIFF_MAX_NUM+8) m_pSectBase->Stiffness.dxy4 = dNum;
		else if (nRow == D_STIFF_START_ROW+D_STIFF_MAX_NUM+9) m_pSectBase->Stiffness.dxz1 = dNum;
		else if (nRow == D_STIFF_START_ROW+D_STIFF_MAX_NUM+10) m_pSectBase->Stiffness.dxz2 = dNum;
		else if (nRow == D_STIFF_START_ROW+D_STIFF_MAX_NUM+11) m_pSectBase->Stiffness.dxz3 = dNum;
		else if (nRow == D_STIFF_START_ROW+D_STIFF_MAX_NUM+12) m_pSectBase->Stiffness.dxz4 = dNum;
		else if (nRow == D_STIFF_START_ROW+D_STIFF_MAX_NUM+13) m_pSectBase->Stiffness.dYsy = dNum; // Stiffness.dYsy, Stiffness.dYsz  =  도심(dYcy, dYzy)에서 전단중심까지의 편심
		else if (nRow == D_STIFF_START_ROW+D_STIFF_MAX_NUM+14) m_pSectBase->Stiffness.dYsz = dNum;
		else if (nRow == D_STIFF_START_ROW+D_STIFF_MAX_NUM+15) m_pSectBase->Stiffness.dIp = dNum;
		
		OnSizeChange();
	}
}

BOOL CCMSectItemStiffGrid::GetDoubleValue(CString& csNum, double& dNum)
{
	int ecode;
	if (!IsValidDoubleNumber(csNum, ecode)) return InvalidDoubleNumber(ecode);
	ConvertFormulaToValue(csNum);
	dNum = _tstof(csNum);
	return TRUE;
}

// nErrCode : 1(수식 에러), 2(숫자 아님), 3(범위 초과)
BOOL CCMSectItemStiffGrid::IsValidDoubleNumber(CString value, int &nErrCode)
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

BOOL CCMSectItemStiffGrid::ConvertFormulaToValue(CString& value)
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

BOOL CCMSectItemStiffGrid::IsNumeric(CString str, int& ecode)
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

BOOL CCMSectItemStiffGrid::InvalidDoubleNumber(int nErrCode)
{
	switch(nErrCode)
	{
	case 1: return ValidateError(_LS(IDS_CMD0417__Formula_is_invalid_)); 
	case 2: return ValidateError(_LS(IDS_CMD0417__Character_exist_in_the_number_));
	case 3: return ValidateError(_LS(IDS_CMD0417__Out_of_value_range_));
	}
	return FALSE;
}

BOOL CCMSectItemStiffGrid::ValidateError(LPCTSTR lpszError)
{
	SetWarningText(lpszError);
	return FALSE;
}

BOOL CCMSectItemStiffGrid::OnEndEditing(ROWCOL nRow, ROWCOL nCol)
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

BOOL CCMSectItemStiffGrid::OnPasteFromClipboard(const CGXRange &range)
{
	if (!CGXGridWnd::OnPasteFromClipboard(range)) return FALSE;
	SaveData();
	//OnSizeChange();
	return TRUE;
}
void CCMSectItemStiffGrid::OnCalculateButton()
{
	ASSERT(m_pParent);
	if(m_pParent==0)return;
	m_pParent->OnCalculateButton();
}
void CCMSectItemStiffGrid::OnDisplayCentroldButton()
{
	ASSERT(m_pParent);
	if(m_pParent==0)return;
	m_pParent->OnDisplayCentroldButton();
}
void CCMSectItemStiffGrid::OnClickedButtonRowCol(ROWCOL nRow, ROWCOL nCol)
{
	if (nRow == D_STIFF_START_ROW-1)
	{
		OnCalculateButton();
		ShowData();
		OnSizeChange();
		OnDisplayCentroldButton();
		return;
	}
	else
	{
		CGXGridWnd::OnClickedButtonRowCol(nRow, nCol);
		return;
	}
	
}


