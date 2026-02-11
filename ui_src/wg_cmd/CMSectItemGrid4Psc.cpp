// CMSectItemGrid4Psc.cpp : implementation file
//
// 사용 안됨 : CCMSectItemGrid4PscNew 사용

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMSectItemGrid4Psc.h"

#include "CMSecViewWnd.h"

#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_SectUtil.h"
#include "..\wg_db\wg_db_SectDB.h"

#define D_FLAG_MAX_NUM 8
#define D_SIZE_MAX_NUM 30

#define D_TOTAL_ROW_NUM (1+D_FLAG_MAX_NUM+1+D_SIZE_MAX_NUM)
#define D_FLAG_START_ROW 2
#define D_SIZE_START_ROW (D_FLAG_START_ROW+D_FLAG_MAX_NUM+1)




#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCMSectItemGrid4Psc
CCMSectItemGrid4Psc::CCMSectItemGrid4Psc()
{
	m_pPreview = 0;
	m_pSectBase = 0;
	m_nSectType = -1;
	m_nShapeIndex = -1;
}

CCMSectItemGrid4Psc::~CCMSectItemGrid4Psc()
{
}

//---------------------------------------------------------------------------
// Public Interface Functions
void CCMSectItemGrid4Psc::InitGrid(CCMSecViewWnd* pPreview, CCMSectItemGrid4Psc* pOtherEnd)
{
	ASSERT(pPreview);
	m_pPreview = pPreview;
	m_pOtherEnd = pOtherEnd;

	Initialize(); // 그리드를 초기화한다.
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

void CCMSectItemGrid4Psc::SetDataSource(T_SECT_SECTBASE_D* pSectBase, int nSectType)
{
	ASSERT(nSectType == D_SECT_TYPE_PSC);

	m_pSectBase = pSectBase;
	m_nSectType = nSectType;

	BOOL bOldLock = LockUpdate(TRUE);
	BOOL bOldReadOnlyLock = GetParam()->IsLockReadOnly();
	GetParam()->SetLockReadOnly(FALSE);
	if (m_nSectType == D_SECT_TYPE_PSC)
	{
		SetRowCount(D_TOTAL_ROW_NUM);
		SetColCount(3);

		// set column width
		SetColWidth(1, 1, globalUtils.ScaleByDPI(45));
		SetColWidth(2, 2, globalUtils.ScaleByDPI(85));
		SetColWidth(3, 3, globalUtils.ScaleByDPI(35));

		SetCoveredCellsRowCol(D_FLAG_START_ROW-1, 1, D_FLAG_START_ROW-1, 3);
		SetCoveredCellsRowCol(D_SIZE_START_ROW-1, 1, D_SIZE_START_ROW-1, 3);
		SetStyleRange(CGXRange(D_FLAG_START_ROW-1, 1), CGXStyle()
			.SetInterior(COLORREF(RGB(0, 0, 255)))
			.SetTextColor(COLORREF(RGB(255, 255, 0)))
			.SetValue(_LS(IDS_WG_CMD__ADDD__Joint)));
		SetStyleRange(CGXRange(D_SIZE_START_ROW-1, 1), CGXStyle()
			.SetInterior(COLORREF(RGB(0, 0, 255)))
			.SetTextColor(COLORREF(RGB(255, 255, 0)))
			.SetValue(_LS(IDS_WG_CMD__ADDD__Size)));

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

		CDBDoc* pDoc = CDBDoc::GetDocPoint();
		T_UNIT_INDEX UnitIndex;
	  pDoc->m_pUnitCtrl->GetUnitIndexCurrent(UnitIndex);
		int nPlace;
		switch(UnitIndex.nBase_Length)
		{
		case D_UNITSYS_LENGTH_INDEX_MM : nPlace = 2; break;
		case D_UNITSYS_LENGTH_INDEX_M  : nPlace = 4; break;
		default                        : nPlace = 3; break;
		}

		CString csUnit;
		CUnitCtrl::GetUnitSystem(D_UNITSYS_BASE_LENGTH, csUnit);
		for (int i = 0; i < D_SIZE_MAX_NUM; i++)
		{
			SetStyleRange(CGXRange(D_SIZE_START_ROW+i, 2), CGXStyle()
				.SetFormat(GX_FMT_FIXED)
				.SetPlaces(nPlace));
			SetStyleRange(CGXRange(D_SIZE_START_ROW+i, 3), CGXStyle().SetValue(csUnit));
		}

		CString aFlagTitle[] = {
			_T("JO1"), _T("JO2"), _T("JO3"), _T("JI1"), _T("JI2"), _T("JI3"), _T("JI4"), _T("JI5"),
		};

		for (int i = 0; i < D_FLAG_MAX_NUM; i++)
		{
			SetValueRange(CGXRange(D_FLAG_START_ROW+i, 1), aFlagTitle[i]);
		}
		SetStyleRange(CGXRange(D_FLAG_START_ROW, 2, D_FLAG_START_ROW+D_FLAG_MAX_NUM-1, 2), CGXStyle()
			.SetControl(GX_IDS_CTRL_CHECKBOX3D)
			.SetValueType(GX_VT_NUMERIC)
			.SetHorizontalAlignment(DT_CENTER)
			.SetVerticalAlignment(DT_VCENTER)
			.SetValue(_T("0"))
			);
	}
	GetParam()->SetLockReadOnly(bOldReadOnlyLock);

	LockUpdate(bOldLock);
	if (!bOldLock) Redraw();

	SetRightValidCell(1, 1);
}

void CCMSectItemGrid4Psc::ChangeShape(int nNewShapeIndex)
{
	m_nShapeIndex = nNewShapeIndex;

	// show/hide row according to shape
	BOOL bOldLock = LockUpdate(TRUE);
	if (m_nSectType == D_SECT_TYPE_PSC)
	{
		CString csShape;
		csShape = CSectUtil::GetShapeNameFromIndexPsc(m_nShapeIndex);
		int nSizeNum = CSectUtil::GetSizeCountByPscShape(csShape);
		HideRows(D_SIZE_START_ROW, D_SIZE_START_ROW+nSizeNum-1, FALSE); // show
		if (nSizeNum < D_SIZE_MAX_NUM) HideRows(D_SIZE_START_ROW+nSizeNum, D_SIZE_START_ROW+D_SIZE_MAX_NUM-1, TRUE); // hide

		// change size title
		CStringArray aSizeTitle;
		CSectUtil::GetSizeTitleFromShapeIndexPsc(m_nShapeIndex, aSizeTitle);
		ASSERT(nSizeNum == aSizeTitle.GetSize());
		nSizeNum = aSizeTitle.GetSize();
		BOOL bOldReadOnlyLock = GetParam()->IsLockReadOnly();
		GetParam()->SetLockReadOnly(FALSE);
		for (int i = 0; i < nSizeNum; i++)
			SetValueRange(CGXRange(D_SIZE_START_ROW+i, 1), aSizeTitle[i]);
		GetParam()->SetLockReadOnly(bOldReadOnlyLock);
	}
	LockUpdate(bOldLock);
	if (!bOldLock) Redraw();
}

void CCMSectItemGrid4Psc::ShowData()
{
	if (m_nSectType == D_SECT_TYPE_PSC)
	{
		CString csShape;
		csShape = CSectUtil::GetShapeNameFromIndexPsc(m_nShapeIndex);
		int nSizeNum = CSectUtil::GetSizeCountByPscShape(csShape);
		for (int i = 0; i < nSizeNum; i++)
			SetValueRange(CGXRange(D_SIZE_START_ROW+i, 2), m_pSectBase->Size[i]);

		int nJointPos = 0x01;
		long bFlag;
		for (int i = 0; i < D_FLAG_MAX_NUM; i++)
		{
			bFlag = (m_pSectBase->BuiltUpFlag & nJointPos) != 0 ? 1: 0;
			SetValueRange(CGXRange(D_FLAG_START_ROW+i, 2), bFlag);
			nJointPos <<= 1;
		}
		SetColumnStatus();
	}
}

BOOL CCMSectItemGrid4Psc::SaveData()
{
	BOOL bSuccess = SaveSize();
	bSuccess = bSuccess && SaveFlag();
	return bSuccess;
}

BOOL CCMSectItemGrid4Psc::SaveSize()
{
	CString csNum;
	double dNum;

	CString csShape;
	csShape = CSectUtil::GetShapeNameFromIndexPsc(m_nShapeIndex);
	int nSizeNum = CSectUtil::GetSizeCountByPscShape(csShape);
	for (int i = 0; i < nSizeNum; i++)
	{
		csNum = GetEditingValue(D_SIZE_START_ROW+i, 2);
		if (GetDoubleValue(csNum, dNum)) 
			m_pSectBase->Size[i] = dNum;
		else m_pSectBase->Size[i] = 0.0;
	}

	return TRUE;
}

BOOL CCMSectItemGrid4Psc::SaveFlag()
{
	CString csNum;
	int nNum;

	int nJointFlag = 0x01;
	m_pSectBase->BuiltUpFlag = 0;
	for (int i = 0; i < D_FLAG_MAX_NUM; i++)
	{
		csNum = GetEditingValue(D_FLAG_START_ROW+i, 2);
		nNum = _ttoi(csNum);
		if (nNum != 0) m_pSectBase->BuiltUpFlag |= nJointFlag;
		nJointFlag <<= 1;
	}

	return TRUE;
}

//---------------------------------------------------------------------------
// Implementation Function
BOOL CCMSectItemGrid4Psc::SetLeftValidCell(ROWCOL nRow, ROWCOL nCol)
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

BOOL CCMSectItemGrid4Psc::SetRightValidCell(ROWCOL nRow, ROWCOL nCol)
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

void CCMSectItemGrid4Psc::OnFlagChange()
{
	if (m_pPreview != 0) 
	{
		m_pPreview->SetCentroidFlag(FALSE);
		m_pPreview->Invalidate();
	}
}

void CCMSectItemGrid4Psc::OnSizeChange()
{
	if (m_pPreview != 0) 
	{
		m_pPreview->SetCentroidFlag(FALSE);
		m_pPreview->Invalidate();
	}
}

CString CCMSectItemGrid4Psc::GetEditingValue(ROWCOL nRow, ROWCOL nCol)
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

void CCMSectItemGrid4Psc::SetColumnStatus()
{
	int aRow[][2] = {
		{2,7}, {3,8}, {5,10}, {14,23}, {15,24}, {17,25}, {20,28}, {19,27},
	};
	
	int nFlag;
	int nFlagPos = 0x01;
	for (int n = 0; n < 8; n++)
	{
		CGXStyle style;
		nFlag = (m_pSectBase->BuiltUpFlag & nFlagPos);
		if (nFlag == 0) 
			style.SetEnabled(FALSE).SetValue((long)0);
		else style.SetEnabled(TRUE);
		for (int i = 0; i < 2; i++)
		{
			SetStyleRange(CGXRange(D_SIZE_START_ROW+aRow[n][i], 2), style);
			HideRows(D_SIZE_START_ROW+aRow[n][i], D_SIZE_START_ROW+aRow[n][i], nFlag == 0);
		}
		nFlagPos <<= 1;
	}
}

BEGIN_MESSAGE_MAP(CCMSectItemGrid4Psc, CGXGridWnd)
	//{{AFX_MSG_MAP(CCMSectItemGrid4Psc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CCMSectItemGrid4Psc message handlers
/////////////////////////////////////////////////////////////////////////////
BOOL CCMSectItemGrid4Psc::ProcessKeys(CWnd *pSender, UINT nMessage, UINT nChar, UINT nRepCnt, UINT flags)
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

BOOL CCMSectItemGrid4Psc::OnValidateCell(ROWCOL nRow, ROWCOL nCol)
{
	CString csNum = GetEditingValue(nRow, nCol);
	double dNum;
	if (nRow >= D_SIZE_START_ROW)
	{
		if (GetDoubleValue(csNum, dNum)) return TRUE;
		return FALSE;
	}
	return TRUE;
}

BOOL CCMSectItemGrid4Psc::OnActivateGrid(BOOL bActivate)
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

void CCMSectItemGrid4Psc::OnModifyCell(ROWCOL nRow, ROWCOL nCol)
{
	CGXGridWnd::OnModifyCell(nRow, nCol);
	if (nRow >=D_SIZE_START_ROW && nRow <= D_SIZE_START_ROW+D_SIZE_MAX_NUM-1) 
	{
		CString csNum = GetEditingValue(nRow, nCol);
		double dNum;
		if (GetDoubleValue(csNum, dNum)) 
			m_pSectBase->Size[nRow-D_SIZE_START_ROW] = dNum;
		else m_pSectBase->Size[nRow-D_SIZE_START_ROW] = 0.0;

		OnSizeChange();
	}
}

BOOL CCMSectItemGrid4Psc::OnStartEditing(ROWCOL nRow, ROWCOL nCol)
{
	// Flag가 변경됨
	// 부모의 OnStartEditing을 항상 먼저 호출해 주어야 한다.
	BOOL bResult = CGXGridWnd::OnStartEditing(nRow, nCol);
	if (nRow < D_FLAG_START_ROW) return bResult;
	if (nRow >= D_FLAG_START_ROW+D_FLAG_MAX_NUM) return bResult;
	CString csVal = GetValueRowCol(nRow, nCol);
	int nFlag = _ttoi(csVal);
	// OnStartEditing시 GetValueRowCol을 하면 변경되기 전의 값을 가져온다.
	// 따라서 반대로 해준다.
	if (nFlag == 0) nFlag = 1;  
	else nFlag = 0;

	int nFlagPos = 0x01;
	nFlagPos <<= nRow - D_FLAG_START_ROW;
	if (nFlag == 1) m_pSectBase->BuiltUpFlag |= nFlagPos;
	else m_pSectBase->BuiltUpFlag &= ~nFlagPos;

	SetColumnStatus();

	//Apply시 에러체킹한다.
	//m_pOtherEnd->m_pSectBase->BuiltUpFlag = m_pSectBase->BuiltUpFlag;
	//m_pOtherEnd->ShowData();

	return bResult;
}

BOOL CCMSectItemGrid4Psc::OnPasteFromClipboard(const CGXRange &range)
{
	if (!CGXGridWnd::OnPasteFromClipboard(range)) return FALSE;
	SaveData();
	OnSizeChange();
	//Apply시 에러체킹한다.
	//m_pOtherEnd->m_pSectBase->BuiltUpFlag = m_pSectBase->BuiltUpFlag;
	//m_pOtherEnd->ShowData();
	return TRUE;
}

BOOL CCMSectItemGrid4Psc::GetDoubleValue(CString& csNum, double& dNum)
{
	CString csExpr;
	csExpr = _T("=") + csNum;
	if (!ConvertFormulaToValue(csExpr)) return FALSE;
	dNum = _tstof(csExpr);
	return TRUE;
}

BOOL CCMSectItemGrid4Psc::ConvertFormulaToValue(CString& value)
{
	if (GetSheetContext() == NULL && value.GetLength() > 0 && value.GetAt(0) == _T('='))
	{
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
