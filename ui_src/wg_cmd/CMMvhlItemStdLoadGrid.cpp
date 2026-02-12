// CMMvhlItemStdLoadGrid.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMMvhlItemStdLoadGrid.h"


#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_SectUtil.h"
#include "..\wg_db\wg_db_SectDB.h"
#include "..\wg_db\wg_db_NewSect.h"
#include "..\wg_db\wg_db_DBDoc.h"

#include "..\wg_base\TestEnvMgr.h"
#include "..\wg_db\VehlDef.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CCMMvhlItemStdLoadGrid

#define D_TABLE_COUNT  3
#define COLCOUNT 3


CCMMvhlItemStdLoadGrid::CCMMvhlItemStdLoadGrid()
{
	m_pMvhl = 0;
	m_bInitDone = FALSE;
	m_pParent=0;
	m_nSize=0;
	m_nIndex=0;
	m_nLoadType=0;
}

CCMMvhlItemStdLoadGrid::~CCMMvhlItemStdLoadGrid()
{
}
void CCMMvhlItemStdLoadGrid::InitGrid(int nGridWidth)
{
	if (!m_bInitDone) Initialize(); // 그리드를 초기화한다.
	else { ResetGrid(); Initialize(); }
	m_bInitDone = TRUE;

	m_nGridWidth = nGridWidth;

	GetParam()->EnableUndo(FALSE);  // undo를 금지시킨다.

	// cell의 width와 height 크기 변경을 금지한다.
	GetParam()->EnableTrackRowHeight(FALSE);
	//GetParam()->EnableTrackColWidth(FALSE);

	// column과 row의 위치 이동을 금지시킨다.
	GetParam()->EnableMoveCols(FALSE);
	GetParam()->EnableMoveRows(FALSE);

	// double click시 셀 편집이 되도록 한다.
	GetParam()->SetActivateCellFlags(GX_CAFOCUS_DBLCLICKONCELL);
	GetParam()->SetNumberedRowHeaders(TRUE);

	// Text만 카피 되게 한다.
	//EnableOleDataSource(GX_DNDTEXT);
	m_nClipboardFlags &= ~GX_DNDSTYLES;

	// Row 헤더와 Column 헤더를 숨긴다.
	//HideRows(0, 0);
	//HideCols(0, 0);

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


BOOL CCMMvhlItemStdLoadGrid::IsAutoCellSVSOV( int nRow )
{
	CString strVehicle;

	if     (m_pMvhl->nStandardCode == D_MVHL_EURO_EN1991_2_2003_ROAD_BRG) strVehicle = m_pMvhl->SelVehicle;
	else if(m_pMvhl->nStandardCode == D_MVHL_BS_BD8611)                   strVehicle = m_pMvhl->VehicleTypeName;
	
	if(strVehicle == _T("SV 80") || strVehicle == _T("SV 100"))
	{
		if(nRow==3)
		{
			return TRUE;
		}
	}
	else if(strVehicle == _T("SV 150"))
	{
		if(nRow==5)
		{
			return TRUE;
		}
	}
	else if(strVehicle == _T("SV 196"))
	{
		if(nRow==7)
		{
			return TRUE;
		}
	}
	else if(strVehicle == _T("SV-Train"))
	{
		if(nRow==8)
		{
			return TRUE;
		}
	}
	else if(strVehicle == _T("SOV 250"))
	{
		if(nRow==10)
		{
			return TRUE;
		}
	}
	else if(strVehicle == _T("SOV 350"))
	{
		if(nRow==12)
		{
			return TRUE;
		}
	}
	else if(strVehicle == _T("SOV 450"))
	{
		if(nRow==14)
		{
			return TRUE;
		}
	}
	else if(strVehicle == _T("SOV 600"))
	{
		if(nRow==18)
		{
			return TRUE;
		}
	}
	return FALSE;
}

BOOL CCMMvhlItemStdLoadGrid::IsAutoCellCS454( int nRow )
{
	CString strVehicle = m_pMvhl->SelVehicle;;

	if     (strVehicle == _T("A-4AXLE")  )   if(nRow==4) {return TRUE;}
	else if(strVehicle == _T("B-4AXLE")  )   if(nRow==4) {return TRUE;}
	else if(strVehicle == _T("C-5AXLE")  )   if(nRow==5) {return TRUE;}
	else if(strVehicle == _T("D-5AXLE_1"))   if(nRow==5) {return TRUE;}
	else if(strVehicle == _T("D-5AXLE_2"))   if(nRow==5) {return TRUE;}
	else if(strVehicle == _T("E-5AXLE_1"))   if(nRow==5) {return TRUE;}
	else if(strVehicle == _T("E-5AXLE_2"))   if(nRow==5) {return TRUE;}
	else if(strVehicle == _T("F-6AXLE_1"))   if(nRow==6) {return TRUE;}
	else if(strVehicle == _T("F-6AXLE_2"))   if(nRow==6) {return TRUE;}
	else if(strVehicle == _T("G-6AXLE_1"))   if(nRow==6) {return TRUE;}
	else if(strVehicle == _T("G-6AXLE_2"))   if(nRow==6) {return TRUE;}
	else if(strVehicle == _T("H-5AXLE_1"))   if(nRow==5) {return TRUE;}
	else if(strVehicle == _T("H-5AXLE_2"))   if(nRow==5) {return TRUE;}
	else if(strVehicle == _T("I-3AXLE")  )   if(nRow==3) {return TRUE;}
	else if(strVehicle == _T("J-3AXLE")  )   if(nRow==3) {return TRUE;}
	else if(strVehicle == _T("K-3AXLE_1"))   if(nRow==3) {return TRUE;}
	else if(strVehicle == _T("K-3AXLE_2"))   if(nRow==3) {return TRUE;}
	else if(strVehicle == _T("L-3AXLE_1"))   if(nRow==3) {return TRUE;}
	else if(strVehicle == _T("L-3AXLE_2"))   if(nRow==3) {return TRUE;}
	else if(strVehicle == _T("M-2AXLE")  )   if(nRow==2) {return TRUE;}
	else if(strVehicle == _T("N-2AXLE")  )   if(nRow==2) {return TRUE;}
	else if(strVehicle == _T("O-2AXLE")  )   if(nRow==2) {return TRUE;}

	return FALSE;
}

BOOL CCMMvhlItemStdLoadGrid::IsAutoCell(int nRow)
{
	if(m_pMvhl->nStandardCode == D_MVHL_EURO_EN1991_2_2003_ROAD_BRG)
	{
		if(m_pMvhl->VehicleTypeName == _T("Load Model 3 (UK NA)"))
		{
			return IsAutoCellSVSOV(nRow);
		}
	}
	else if(m_pMvhl->nStandardCode == D_MVHL_BS_BD8611)
	{
		return IsAutoCellSVSOV(nRow);
	}
	else if(m_pMvhl->nStandardCode == D_MVHL_BS_CS454)
	{
		return IsAutoCellCS454(nRow);
	}
	return FALSE;
}


void CCMMvhlItemStdLoadGrid::SetStyleOnSizeCell()
{
	int count;
	int i;
	count=m_nSize;
	
	for (i = 1; i <= count; i++)
	{
		SetStyleRange(CGXRange(i,1), CGXStyle()
			.SetFormat(GX_FMT_FIXED).SetPlaces(2));
		SetStyleRange(CGXRange(i,2), CGXStyle()
			.SetFormat(GX_FMT_FIXED).SetPlaces(2));

		if(IsAutoCell(i))
		{
			SetStyleRange(CGXRange(i, 2), CGXStyle().SetFormat(GX_VT_STRING));
			SetValueRange(CGXRange(i, 2), _LS(IDS_CMD_MVHL_EURO_BS_SOSOV_GRID_A));
		}		

		if(count == i)
		{
			SetStyleRange(CGXRange(i, 2), CGXStyle().SetFormat(GX_VT_STRING));
			SetValueRange(CGXRange(i,2),_LS(IDS_WG_CMD__ADDD__end));
		}
	}
	
}
void CCMMvhlItemStdLoadGrid::SetDataSource(T_MVHL_D* pMvhl,CWnd* pParent, int nIndex, int nSize, int nLoadType)
{
	ASSERT(pMvhl);
	m_pMvhl = pMvhl;
	m_pParent=pParent;
	m_nSize=nSize;
	m_nIndex=nIndex;
	m_nLoadType=nLoadType;
	CString strUnit,strTemp;
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	BOOL bOldLock = LockUpdate(TRUE);
	BOOL bOldReadOnlyLock = GetParam()->IsLockReadOnly();
	GetParam()->SetLockReadOnly(FALSE);
	
	SetRowCount(nSize);
	SetColCount(2);

	// set column width  
	int nWidth[3];
	nWidth[0] = m_nGridWidth * 0.2;
	nWidth[1] = m_nGridWidth * 0.45;
	nWidth[2] = m_nGridWidth - nWidth[0] - nWidth[1];
	for (int i = 0; i < 3; i++)
	{
		ASSERT(nWidth[i] > 0);
		SetColWidth(i, i, nWidth[i]);
	}

	int nHeight = GetRowHeight(1);
	SetRowHeight(0,0,nHeight*globalUtils.ScaleByDPI(2));
	
	SetStyleRange(CGXRange(0, 0), CGXStyle()
		.SetHorizontalAlignment(DT_LEFT)
			.SetValue(_LS(IDS_CMD_MVHL_NO)));
	pDoc->m_pUnitCtrl->GetUnitSystem(CUnitCtrl::m_MVHL_UNIT.dPointLoad, strUnit); 
	strTemp.Format(_T("%s\n(%s)"),_LS(IDS_CMD_MVHL_P),strUnit);
 
	SetStyleRange(CGXRange(0, 1), CGXStyle()
		.SetHorizontalAlignment(DT_CENTER)
			.SetValue(strTemp));
	pDoc->m_pUnitCtrl->GetUnitSystem(CUnitCtrl::m_MVHL_UNIT.dPointDistance, strUnit); 
	strTemp.Format(_T("%s\n(%s)"),_LS(IDS_CMD_MVHL_D),strUnit);
	SetStyleRange(CGXRange(0, 2), CGXStyle()
		.SetHorizontalAlignment(DT_CENTER)
			.SetValue(strTemp));
	SetStyleRange(CGXRange().SetCols(0), CGXStyle()
		.SetHorizontalAlignment(DT_RIGHT));
	
	SetStyleRange(CGXRange().SetCols(1), CGXStyle()
		.SetHorizontalAlignment(DT_RIGHT)
		.SetInterior(::GetSysColor(COLOR_BTNFACE))
		.SetEnabled(FALSE)
		.SetValueType(GX_VT_NUMERIC)
		.SetFormat(GX_FMT_FIXED)
		.SetPlaces(2));

	SetStyleRange(CGXRange().SetCols(2), CGXStyle()
		.SetHorizontalAlignment(DT_RIGHT)
		.SetInterior(::GetSysColor(COLOR_BTNFACE))
		.SetEnabled(FALSE)
		.SetValueType(GX_VT_NUMERIC)
		.SetFormat(GX_FMT_FIXED)
		.SetPlaces(2));
	SetStyleRange(CGXRange().SetCols(0), CGXStyle()
		.SetHorizontalAlignment(DT_LEFT));
	SetStyleOnSizeCell();

	GetParam()->SetLockReadOnly(bOldReadOnlyLock);

	LockUpdate(bOldLock);
	if (!bOldLock) Redraw();
	ShowData();
	SetRightValidCell(1, 1);
}

void CCMMvhlItemStdLoadGrid::ShowData()
{
	CString str;
	double* dLoad,*dSpacing;
	dLoad=m_pMvhl->dPointLoad3[m_nIndex];
	dSpacing=m_pMvhl->dPointDistance3[m_nIndex];

	for (int i = 0; i < m_nSize; i++)
	{
		str.Format(_T("%g"),dLoad[i]);
		SetValueRange(CGXRange(i+1, 1),str);

		if(IsAutoCell(i + 1))
		{
			continue;
		}

		if(i == m_nSize-1)
			continue;
		str.Format(_T("%g"),dSpacing[i]);
		SetValueRange(CGXRange(i+1, 2),str);
	}
	
		
}
BOOL CCMMvhlItemStdLoadGrid::SaveData()
{
	CString csNum;
	double dNum;
	double* dLoad,*dSpacing;
	dLoad=m_pMvhl->dPointLoad3[m_nIndex];
	dSpacing=m_pMvhl->dPointDistance3[m_nIndex];
	
	for (int i = 0; i <m_nSize; i++)
	{
		csNum = GetEditingValue(i+1, 1);
		if (GetDoubleValue(csNum, dNum)) 
		{
			dLoad[i] = dNum;
		}
		else
		{
			dLoad[i] = 0.0;
		}
		if(i == m_nSize -1)
		{
			dSpacing[i] = 0.0;
			continue;
		}
		csNum = GetEditingValue(i+1, 2);
		if (GetDoubleValue(csNum, dNum)) 
		{
			dSpacing[i] = dNum;
		}
		else
		{
			dSpacing[i] = 0.0;
		}
		
	}
	
	return TRUE;
}

BOOL CCMMvhlItemStdLoadGrid::SetDownValidCell(ROWCOL nRow, ROWCOL nCol)
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
BOOL CCMMvhlItemStdLoadGrid::SetUpValidCell(ROWCOL nRow, ROWCOL nCol)
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

CString CCMMvhlItemStdLoadGrid::GetEditingValue(ROWCOL nRow, ROWCOL nCol)
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




BEGIN_MESSAGE_MAP(CCMMvhlItemStdLoadGrid, CGXGridWnd)
	//{{AFX_MSG_MAP(CCMMvhlItemStdLoadGrid)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CCMMvhlItemStdLoadGrid message handlers
/////////////////////////////////////////////////////////////////////////////

BOOL CCMMvhlItemStdLoadGrid::ProcessKeys(CWnd *pSender, UINT nMessage, UINT nChar, UINT nRepCnt, UINT flags)
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
				if (bShift && SetDownValidCell(nRow, nCol)) break;
				if (!bShift && SetUpValidCell(nRow,nCol)) break;

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
				return SetDownValidCell(nRow, nCol);
			break;
		case VK_DOWN:
			if (nRow < GetRowCount())
				return SetDownValidCell(nRow, nCol);
			break;
		case VK_LEFT:  // 이전 셀로 이동
			if (nRow > 1)
			{
				return SetUpValidCell(nRow, nCol);
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

BOOL CCMMvhlItemStdLoadGrid::OnValidateCell(ROWCOL nRow, ROWCOL nCol)
{
	CString csNum = GetEditingValue(nRow, nCol);
	double dNum;
	if (GetDoubleValue(csNum, dNum)) return TRUE;
	return FALSE;
}

BOOL CCMMvhlItemStdLoadGrid::OnActivateGrid(BOOL bActivate)
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

void CCMMvhlItemStdLoadGrid::OnModifyCell(ROWCOL nRow, ROWCOL nCol)
{
	CGXGridWnd::OnModifyCell(nRow, nCol);
	SaveData();  

}

BOOL CCMMvhlItemStdLoadGrid::GetDoubleValue(CString& csNum, double& dNum)
{
	int ecode;
	if (!IsValidDoubleNumber(csNum, ecode)) return InvalidDoubleNumber(ecode);
	ConvertFormulaToValue(csNum);
	dNum = _tstof(csNum);
	return TRUE;
}

// nErrCode : 1(수식 에러), 2(숫자 아님), 3(범위 초과)
BOOL CCMMvhlItemStdLoadGrid::IsValidDoubleNumber(CString value, int &nErrCode)
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

BOOL CCMMvhlItemStdLoadGrid::ConvertFormulaToValue(CString& value)
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

BOOL CCMMvhlItemStdLoadGrid::IsNumeric(CString str, int& ecode)
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

BOOL CCMMvhlItemStdLoadGrid::InvalidDoubleNumber(int nErrCode)
{
	switch(nErrCode)
	{
	case 1: return ValidateError(_LS(IDS_CMD0417__Formula_is_invalid_)); 
	case 2: return ValidateError(_LS(IDS_CMD0417__Character_exist_in_the_number_));
	case 3: return ValidateError(_LS(IDS_CMD0417__Out_of_value_range_));
	}
	return FALSE;
}

BOOL CCMMvhlItemStdLoadGrid::ValidateError(LPCTSTR lpszError)
{
	SetWarningText(lpszError);
	return FALSE;
}

BOOL CCMMvhlItemStdLoadGrid::OnEndEditing(ROWCOL nRow, ROWCOL nCol)
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
BOOL CCMMvhlItemStdLoadGrid::SetRightValidCell(ROWCOL nRow, ROWCOL nCol)
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
