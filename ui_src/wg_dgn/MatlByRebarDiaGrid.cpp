// MatlByRebarDiaGrid.cpp: implementation of the CDgnConBeamRebarGrid class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "wg_dgn.h"
#include "MatlByRebarDiaGrid.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_DBLib.h"
#include "..\wg_db\wg_db_MatlDB.h"
#include "..\wg_db\wg_db_UnitCtrl.h"

#include "..\wg_common\wg_common_TBGrid.h"
#include "..\wg_base\SpecialChar.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

enum RowIndex
{ ROWINDEX_HEADER        = 0, ROWINDEX_TITLE = 0, ROWINDEX_BODY = 1,
};

enum ColumnIndex
{ COLINDEX_HEADER    =  0, COLINDEX_NUMBER  =  0, COLINDEX_REBAR  =  1, COLINDEX_GRADE = 2, COLINDEX_FY = 3
};

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMatlByRebarDiaGrid::CMatlByRebarDiaGrid()
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	m_pUnitCtrl = pDoc->m_pUnitCtrl;
	m_pMatlDB = pDoc->m_pMatlDB;

	m_pData = nullptr;
	m_pMarb = nullptr;
}

// CDgnConBeamRebarGrid::~CDgnConBeamRebarGrid()
// {
// 
// }

//////////////////////////////////////////////////////////////////////
// Interface
void CMatlByRebarDiaGrid::EnableGrid(int nRow, int nCol, BOOL bEnable)
{
	SetStyleRange(CGXRange().SetCells(nRow, nCol), CGXStyle().SetEnabled(bEnable));
}

void CMatlByRebarDiaGrid::Initialize(CWnd* pParent, T_MSRB_D* const pMsrb)
{
	m_pMarb = pMsrb;
	m_eMode = GRID_MODE::MSRB_REBAR;

	Initialize_Grid();
}

void CMatlByRebarDiaGrid::Initialize(CWnd* pParent, T_DCRB_D* const pDcrb, const GRID_MODE eMode)
{
	//m_pParent = (CDgnConBeamNewDlg*)pParent;
	m_pData = pDcrb;
	m_eMode = eMode;

	Initialize_Grid();

}
void CMatlByRebarDiaGrid::Initialize_Grid()
{
	CGXGridWnd::Initialize();

	LockUpdate(TRUE);

	// cell 바깥영역 배경색 지정 
	GetParam()->GetProperties()->SetColor(GX_COLOR_BACKGROUND, CTBGrid::m_Color.OuterBackground);

	// Header 배경색 지정 
	ChangeColHeaderStyle(CGXStyle().SetInterior(CTBGrid::m_Color.RowColHeaderBg));
	ChangeRowHeaderStyle(CGXStyle().SetInterior(CTBGrid::m_Color.RowColHeaderBg));

	// 기타 Cells 배경색 지정
	ChangeStandardStyle(CGXStyle().SetInterior(CTBGrid::m_Color.NormalModeBg));

	// 표준 스타일 변경
	ChangeStandardStyle(CGXStyle()
		.SetFont(CGXFont_GC().SetSize(9))
		.SetHorizontalAlignment(DT_CENTER)
		.SetVerticalAlignment(DT_VCENTER)
		.SetAllowEnter(FALSE));

	// Column header의 스타일 변경
	ChangeColHeaderStyle(CGXStyle().SetFont(CGXFont_GC().SetBold(FALSE)));
	SetRowHeight(0, 0, globalUtils.ScaleByDPI(20));

	GetParam()->EnableUndo(FALSE);

	// Row와  Column의 이동을 금지시킨다.
	GetParam()->EnableTrackRowHeight(FALSE);
	GetParam()->EnableTrackColWidth(FALSE);
	GetParam()->EnableMoveRows(FALSE);
	GetParam()->EnableMoveCols(FALSE);

	// MemoryDC를 사용한다.
//  SetDrawingTechnique(gxDrawUsingMemDC);

	// 헤더의 Current cell을 막는다.
	ColHeaderStyle().SetEnabled(FALSE);
	RowHeaderStyle().SetEnabled(FALSE);

	CGXProperties* pProp = GetParam()->GetProperties();
	pProp->SetMarkColHeader(FALSE); // Turn off pressed button effect for column headers
	pProp->SetMarkRowHeader(FALSE);
	pProp->SetUserProperty(GX_IDS_OUTLINECURRENTCELL, // 현재 셀의 border 설정 변경
		(CGXStyle)pProp->sInvertNoBorder);

	// Grid Number
	SetColCount(3); // Number of column excluding header
	SetRowCount(1); // Number of row excluding header

	// Row_Grid Size
	SetSizeColumn();

	// Set Title
	SetHeaderTitle();
	SetDataColumn();

	SetScrollBarMode(SB_VERT, gxnEnabled);

	LockUpdate(FALSE);
}

void CMatlByRebarDiaGrid::SetSizeColumn()
{ 
	// Col_Grid Size
	SetColWidth(COLINDEX_NUMBER, COLINDEX_NUMBER, globalUtils.ScaleByDPI(40));
	SetColWidth(COLINDEX_REBAR, COLINDEX_REBAR, globalUtils.ScaleByDPI(75));
	SetColWidth(COLINDEX_GRADE, COLINDEX_GRADE, globalUtils.ScaleByDPI(88));
	SetColWidth(COLINDEX_FY, COLINDEX_FY, globalUtils.ScaleByDPI(100));
}

void CMatlByRebarDiaGrid::SetDataColumn()
{
	//////////////////////////////////////////////////////////////////////////
	//한번만 호출하면 되는 코드
	CString strListRebar = Get_ListRebar();
	SetStyleRange(CGXRange().SetCols(COLINDEX_REBAR), CGXStyle()
		.SetControl(GX_IDS_CTRL_CBS_DROPDOWNLIST)
		.SetValueType(GX_VT_STRING)
		.SetEnabled(TRUE)
		.SetHorizontalAlignment(DT_LEFT)
		.SetVerticalAlignment(DT_VCENTER)
		.SetChoiceList(strListRebar)
		);

	SetStyleRange(CGXRange().SetCols(COLINDEX_FY), CGXStyle()
		.SetValueType(GX_VT_NUMERIC)
		.SetHorizontalAlignment(DT_RIGHT)
		.SetVerticalAlignment(DT_VCENTER)
		);
	//////////////////////////////////////////////////////////////////////////
	CString strMatlCode = m_eMode == MSRB_REBAR ? m_pMarb->strMatlCode : m_pData->strMatlCode;
	if(strMatlCode != T_DCRB_D::GetNoneCode())
	{
		CString strListGrade = Get_ListMatl();
		SetStyleRange(CGXRange().SetCols(COLINDEX_GRADE), CGXStyle()
			.SetControl(GX_IDS_CTRL_CBS_DROPDOWNLIST)
			.SetValueType(GX_VT_STRING)
			.SetEnabled(TRUE)
			.SetHorizontalAlignment(DT_LEFT)
			.SetVerticalAlignment(DT_VCENTER)
			.SetChoiceList(strListGrade)
		);
		SetStyleRange(CGXRange().SetCols(COLINDEX_FY), CGXStyle().SetEnabled(FALSE));
	}
	else
	{
		CString strListGrade = Get_ListMatl();
		SetStyleRange(CGXRange().SetCols(COLINDEX_GRADE), CGXStyle()
			.SetControl(GX_IDS_CTRL_EDIT)
			.SetValueType(GX_VT_STRING)
			.SetEnabled(TRUE)
			.SetHorizontalAlignment(DT_LEFT)
			.SetVerticalAlignment(DT_VCENTER)
		);
		SetStyleRange(CGXRange().SetCols(COLINDEX_FY), CGXStyle().SetEnabled(TRUE));
	}
}


BOOL CMatlByRebarDiaGrid::Data2Grid(CArray<T_MATLRBDIA_D, T_MATLRBDIA_D&>* paMatl)
{
	const int nDataSize = paMatl->GetSize();

	SetRowCount(nDataSize);
	for (int nData_i = 0; nData_i < nDataSize; ++nData_i)
	{
		SetStyleRange(CGXRange(ROWINDEX_BODY + nData_i, COLINDEX_REBAR), CGXStyle().SetValue(paMatl->GetAt(nData_i).strDia));
		BOOL bEnableFy = paMatl->GetAt(nData_i).strMatl == T_MATLRBDIA_D::GetNoneMatl();
		SetStyleRange(CGXRange(ROWINDEX_BODY + nData_i, COLINDEX_GRADE), CGXStyle().SetValue(paMatl->GetAt(nData_i).strMatl)
		);
		SetStyleRange(CGXRange(ROWINDEX_BODY + nData_i, COLINDEX_FY), CGXStyle()
			.SetValue(paMatl->GetAt(nData_i).dFy)
			.SetEnabled(bEnableFy)
		);
	}

	if (nDataSize == 0)
	{
		AddRow(FALSE);
	}
	return TRUE;
}

BOOL CMatlByRebarDiaGrid::Data2Grid(T_DCRB_D* pData)
{
	SetDataColumn();

	CArray<T_MATLRBDIA_D, T_MATLRBDIA_D&> *paMatl = NULL;
	if(m_eMode == VERT_RABAR)
	{
		paMatl = &(pData->aVertMatlByDia);
	}
	else if( m_eMode == HORZ_REBAR)
	{
		paMatl = &(pData->aHorzMatlByDia);
	}
	else
	{
		ASSERT(FALSE); return FALSE;
	}

	return Data2Grid(paMatl);
}

BOOL CMatlByRebarDiaGrid::Data2Grid(T_MSRB_D* pData)
{
	SetDataColumn();
	return Data2Grid(&pData->aMatlByDia);
}

BOOL CMatlByRebarDiaGrid::Grid2Data(T_MSRB_D* pData)
{
	return Grid2Data(&pData->aMatlByDia);
}

BOOL CMatlByRebarDiaGrid::Grid2Data(T_DCRB_D* pData)
{
	CArray<T_MATLRBDIA_D, T_MATLRBDIA_D&>* paMatl = NULL;
	if (m_eMode == VERT_RABAR)
	{
		paMatl = &(pData->aVertMatlByDia);
	}
	else if (m_eMode == HORZ_REBAR)
	{
		paMatl = &(pData->aHorzMatlByDia);
	}
	else
	{
		ASSERT(FALSE); return FALSE;
	}

	return Grid2Data(paMatl);
}

BOOL CMatlByRebarDiaGrid::Grid2Data(CArray<T_MATLRBDIA_D, T_MATLRBDIA_D&>* paMatl)
{
	paMatl->RemoveAll();

	const int nRows = GetRowCount();
	CString strSelect = _LS(IDS_DGN_CONC_REBAR_MATL_SELECT);
	for (int nRow_i = 0; nRow_i < nRows; ++nRow_i)
	{
		T_MATLRBDIA_D curData;
		curData.strDia = GetEditingValue(ROWINDEX_BODY + nRow_i, COLINDEX_REBAR);
		if (curData.strDia == strSelect) continue;
		curData.strMatl = GetEditingValue(ROWINDEX_BODY + nRow_i, COLINDEX_GRADE);
		if (curData.strMatl.IsEmpty())
		{
			AfxMessageBox(_LS(IDS_DGN_CONC_REBAR_MATL_INPUT_GRADE));
			return FALSE;
		}
		CString strFy = GetEditingValue(ROWINDEX_BODY + nRow_i, COLINDEX_FY);
		if (strFy.IsEmpty())
		{
			AfxMessageBox(_LS(IDS_DGN_CONC_REBAR_MATL_INPUT_FY));
			return FALSE;
		}
		curData.dFy = _tstof(strFy);
		if (curData.dFy <= 0)
		{
			AfxMessageBox(_LS(IDS_DGN_CONC_REBAR_MATL_WRONG_FY));
			return FALSE;
		}

		paMatl->Add(curData);
	}
	return TRUE;
}

void CMatlByRebarDiaGrid::AddRow(BOOL bCurrent)
{
	CRowColArray awRows; awRows.RemoveAll();
	GetSelectedRows(awRows, TRUE, FALSE);

	int nAddRow = 1;
	const int nRowsSize = awRows.GetSize();
	if(bCurrent && (nRowsSize != 0))
	{
		int nSelRow = awRows.GetAt(0);
		if(nSelRow != 0) nAddRow = nSelRow;
	}
	else
	{
		nAddRow = GetRowCount() + 1;
	}
	InsertRows(nAddRow, 1);
	CString strSelect = _LS(IDS_DGN_CONC_REBAR_MATL_SELECT);
	SetStyleRange(CGXRange(nAddRow,COLINDEX_REBAR), CGXStyle().SetValue(strSelect));
}

void CMatlByRebarDiaGrid::DelRow(BOOL bCurrent)
{
	CRowColArray awRows; awRows.RemoveAll();
	GetSelectedRows(awRows, TRUE, FALSE);

	const int nRowsSize = awRows.GetSize();
	if(bCurrent && (nRowsSize != 0))
	{
		for(int nRow_i = nRowsSize - 1; nRow_i >= 0; --nRow_i)
		{
			UINT nRowSel= awRows.GetAt(nRow_i);
			if(nRowSel == 0) continue;
			RemoveRows(nRowSel, nRowSel);
		}
	}
	else
	{
		int nLastRow = GetRowCount();
		if(nLastRow != 0) RemoveRows(nLastRow, nLastRow);
	}
}

CString CMatlByRebarDiaGrid::Get_ListRebar()
{
	CStringArray arRebar;	arRebar.RemoveAll();
	CDBLib::GetRebarNameListByCode(arRebar);
	
	CString strString = _T("");
	
	if (arRebar.GetSize() != 0)
	{
		for (int i=0; i<arRebar.GetSize(); i++)
		{
			strString += arRebar[i] + _T("\n");
		}
	}
	
	return strString;
}

CString CMatlByRebarDiaGrid::Get_ListMatl()
{
	CArray<CString, CString&> arRebar; arRebar.RemoveAll();
	CString strMatlCode = m_eMode == MSRB_REBAR ? m_pMarb->strMatlCode : m_pData->strMatlCode;

	m_pMatlDB->GetRebarNameList(strMatlCode, arRebar);
	
	arRebar.InsertAt(0, T_MATLRBDIA_D::GetNoneMatl());

	CString strString = _T("");
	if (arRebar.GetSize() != 0)
	{
		for (int i=0; i<arRebar.GetSize(); i++)
		{
			strString += arRebar[i] + _T("\n");
		}
	}
	
	return strString;
}

//////////////////////////////////////////////////////////////////////
// Operation

void CMatlByRebarDiaGrid::SetHeaderTitle()
{
	int iFontSizeHeader  = 8;
	int iFontSizeNormal  = 8;
	BOOL bFontBoldHeader = FALSE;
	BOOL bFontBoldNormal = FALSE;

	CGXStyle style;
	style.SetControl(GX_IDS_CTRL_STATIC);
	style.SetDraw3dFrame(gxFrameRaised);
	style.SetFont(CGXFont_GC().SetBold(FALSE).SetSize(9));
	style.SetInterior(ColHeaderStyle().GetInterior()); 

	CString strNumber = _LS(IDS_DGN_CONC_REBAR_MATL_BY_DIA_SN);
	style.SetValue(strNumber);
	SetStyleRange(CGXRange(ROWINDEX_TITLE, COLINDEX_NUMBER), style);
	CString strRebar = _LS(IDS_DGN_CONC_REBAR_MATL_BY_DIA_REBAR);
	style.SetValue(strRebar);
	SetStyleRange(CGXRange(ROWINDEX_TITLE, COLINDEX_REBAR), style);
	CString strGrade = _LS(IDS_DGN_CONC_REBAR_MATL_BY_DIA_GRADE);
	style.SetValue(strGrade);
	SetStyleRange(CGXRange(ROWINDEX_TITLE, COLINDEX_GRADE), style);
	CString strFy;
	strFy.Format(_LS(IDS_DGN_CONC_REBAR_MATL_BY_DIA_FY), Get_StressForm());
	style.SetValue(strFy);
	SetStyleRange(CGXRange(ROWINDEX_TITLE, COLINDEX_FY), style);
}

CString CMatlByRebarDiaGrid::GetEditingValue(ROWCOL nRow, ROWCOL nCol)
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


//////////////////////////////////////////////////////////////////////
// Override

void CMatlByRebarDiaGrid::OnModifyCell(ROWCOL nRow, ROWCOL nCol)
{
	CGXGridWnd::OnModifyCell(nRow, nCol);

	CString strSelect = _LS(IDS_DGN_CONC_REBAR_MATL_SELECT);
	if(nCol == COLINDEX_REBAR)
	{
		CString srDiaPrev = GetValueRowCol(nRow, nCol);
		CString strDia = GetEditingValue(nRow, nCol);
		if(strDia != strSelect)
		{
			BOOL bExist = FALSE;
			const int nRowCount = GetRowCount();
			for(int nRow_i = ROWINDEX_BODY; nRow_i < nRowCount && bExist == FALSE; ++nRow_i)
			{
				if(nRow == nRow_i) continue;
				CString strExistDia = GetEditingValue(nRow_i, nCol);
				if(strDia == strExistDia) bExist = TRUE;        
			}
			if(bExist)
			{
				CString strErrMsg = _LS(IDS_DGN_CONC_REBAR_MATL_EXIST_REBAR);
				AfxMessageBox(strErrMsg);

				SetValueRange(CGXRange(nRow, nCol), srDiaPrev);
			}
		}
	}

	CString strMatlCode = m_eMode == MSRB_REBAR ? m_pMarb->strMatlCode : m_pData->strMatlCode;
	if(nCol == COLINDEX_GRADE && strMatlCode != T_DCRB_D::GetNoneCode())
	{
		CString strCurGrade = GetEditingValue(nRow, nCol);
		BOOL bFyEnable = strCurGrade == T_MATLRBDIA_D::GetNoneMatl();
		SetStyleRange(CGXRange(nRow, COLINDEX_FY), CGXStyle().SetEnabled(bFyEnable));

		if(!bFyEnable)
		{
			T_MATL_REBAR MatlRebar; MatlRebar.Initialize();
			m_pMatlDB->GetRebarData(strMatlCode, strCurGrade, MatlRebar);
			SetStyleRange(CGXRange(nRow, COLINDEX_FY), CGXStyle().SetValue(MatlRebar.B_fy));
		}
	}
}

void CMatlByRebarDiaGrid::OnCanceledEditing(ROWCOL nRow, ROWCOL nCol)
{
	CGXGridWnd::OnCanceledEditing(nRow, nCol);
}

BOOL CMatlByRebarDiaGrid::OnEndEditing(ROWCOL nRow, ROWCOL nCol)
{  
	BOOL bRet = CGXGridWnd::OnEndEditing(nRow, nCol);

//   if(nRow != ROWINDEX_STIRRUP)    return bRet;
//   if(nCol != COLINDEX_END_I_SPACE && nCol != COLINDEX_MIDDLE_SPACE && nCol != COLINDEX_END_J_SPACE) return bRet;
//   
//   int nIndex = 0;
//   if(nCol == COLINDEX_MIDDLE_SPACE)   nIndex = 1;
//   else if(nCol == COLINDEX_END_J_SPACE)   nIndex = 2;

	CString strSpace= GetEditingValue(nRow, nCol);
	T_UNIT_INDEX UnitIndex;
	m_pUnitCtrl->GetUnitIndexCurrent(UnitIndex);
	if(UnitIndex.nBase_Length == D_UNITSYS_LENGTH_INDEX_MM || UnitIndex.nBase_Length == D_UNITSYS_LENGTH_INDEX_CM ||
		UnitIndex.nBase_Length == D_UNITSYS_LENGTH_INDEX_M)
	{
		double dSpace = _tstof(strSpace);
		//m_dSpace[nIndex] = M_InitValueCurUnit(dSpace, N, MM, D_UNITSYS_BASE_LENGTH);
	}
	else if(UnitIndex.nBase_Length == D_UNITSYS_LENGTH_INDEX_IN || UnitIndex.nBase_Length == D_UNITSYS_LENGTH_INDEX_FT)
	{
		strSpace.Remove('\\');
		double dSpace = _tstof(strSpace);
		strSpace += _T("\\");
		SetValueRange(CGXRange(nRow, nCol), strSpace);
		//m_dSpace[nIndex] = M_InitValueCurUnit(dSpace, N, IN, D_UNITSYS_BASE_LENGTH);
	}
	else   {  ASSERT(0);		return FALSE;	}

	return bRet;
}

// 
// BOOL CMatlByRebarDiaGrid::Copy( )
// {
//   // 실제로 Selected Range는 Row, Col의 조합으로 저장된다는 사실에 착안 
//   // ( 따라서 Multi Selection의 경우는 별도의 처리를 해주어야 함. )
//   CRowColArray aSelectedRow, aSelectedCol;
//   
//   int nSelectedCol = GetSelectedCols(aSelectedCol, FALSE, FALSE);
//   for(int i=0; i<nSelectedCol; i++)
//   {
//     ROWCOL nCol = aSelectedCol.GetAt(i);
//     if(IsColHidden(nCol))
//       SelectRange(CGXRange().SetCols(nCol), FALSE);
//   }  
// 
//   int nSelectedRow = GetSelectedRows(aSelectedRow, FALSE, FALSE);
//   for(i=0; i<nSelectedRow; i++)
//   {
//     ROWCOL nRow = aSelectedRow.GetAt(i);
//     if(IsRowHidden(nRow))
//       SelectRange(CGXRange().SetRows(nRow), FALSE);
//   }
//   return CGXGridWnd::Copy();
// }
// 
// BOOL CMatlByRebarDiaGrid::Paste()
// {
//   BOOL bReturn = CGXGridWnd::Paste();
// 
//   return bReturn;
// }
// 
// BOOL CMatlByRebarDiaGrid::OnPasteFromClipboard(const CGXRange &range)
// {
//   CGXRange r;
//   r.top = r.bottom = 1;
//   r.left = r.right = 1;
//   CGXGridWnd x;
//   x.Create(WS_CHILD, CRect(0, 0, 1, 1), this, 0);
//   x.Initialize();
//   x.SetRowCount(0); x.SetColCount(1);
//   x.OnPasteFromClipboard(r);
// 
//   r.top = 1; r.left = 1;
//   r.bottom = x.GetRowCount(); r.right = x.GetColCount();
// 
//   CString sErrMsg = _T("");
//   ROWCOL nRow, nCol;
//   VERIFY(GetCurrentCell(nRow, nCol));
//   // 대상 범위가 지정되지 않은 경우
//   // 현재 셀이 있는 위치를 기준으로 한다.
//   if (range.GetHeight() == 1 && range.GetWidth() == 1)
//   {
//     // 대상 범위가 Column 한계를 넘어서면 에러
//     if (nCol + r.GetWidth() - 1 > GetColCount())
//     {
//       //sErrMsg = _T("붙여 넣을 범위가 칼럼 범위를 초과합니다. 위치를 다시 지정하세요.");
//       sErrMsg = _LS(IDS_CMD_PASTE_EXCEED_COLUMN);
//       goto END_OF_PASTE;
//     }
//     // 대상 범위가 Row 한계를 넘어서면 에러
//     if (nRow + r.GetHeight() - 1 > GetRowCount())
//     {
//       //sErrMsg = _T("붙여 넣을 범위가 로우 범위를 초과합니다. 위치를 다시 지정하세요.");
//       sErrMsg = _LS(IDS_CMD_PASTE_EXCEED_ROW);
//       goto END_OF_PASTE;
//     }
// 
//   }
//   // 대상 범위가 지정된 경우 : 원본과 같은 범위, Fill Mode(원본 1줄, 동일 칸수)
//   else
//   {
//     // Fill 조건을 검사한다.
//     // 원본이 1줄 이상이면 에러, 칸수가 달라도 에러가 된다.
//     BOOL bFillMode = FALSE;
//     if (range.GetHeight() != r.GetHeight() || range.GetWidth() != r.GetWidth())
//     {
//       // Fill Mode가 안 되는 조건
//       if (r.GetHeight() > 1 || range.GetWidth() != r.GetWidth())
//       {
//         //sErrMsg = _T("대상 범위를 지정한 경우는 원본 범위의 크기와 같아야 합니다.");
//         sErrMsg = _LS(IDS_CMD_PASTE_RANGE_MISMATCH);
//         goto END_OF_PASTE;
//       }
//       // 대상 범위가 1줄 이상이면 Fill Mode, 아니면 Paste와 동일하다.
//       if (range.GetHeight() > 1) bFillMode = TRUE;
//     }
// 
//     // Fill Mode인지 검사한다.
//     if (bFillMode)
//     {
//       // 대상 범위의 줄수 만큼 Row를 만들어 채운다.
//       x.SetRowCount(range.GetHeight());
//       CString value;
//       for (int i = 0; i < r.GetWidth(); i++)  // Column Wise Copy...
//       {
//         value = x.GetValueRowCol(1, i+1);
//         for (int j = 1; j < range.GetHeight(); j++)
//           x.SetExpressionRowCol(j+1, i+1, value);
//       }
//       r.bottom = x.GetRowCount();
//     }
//   }
// 
// END_OF_PASTE:
//   if (!sErrMsg.IsEmpty())
//   {
//     AfxMessageBox(sErrMsg);
//     x.DestroyWindow();
//     return FALSE;
//   }
// 
//   x.DestroyWindow();
//   return CGXGridWnd::OnPasteFromClipboard(range);
// }

BOOL CMatlByRebarDiaGrid::ProcessKeys(CWnd *pSender, UINT nMessage, UINT nChar, UINT nRepCnt, UINT flags)
{
	BOOL bCtl = GetKeyState(VK_CONTROL) & 0x8000;
	BOOL bShift = GetKeyState(VK_SHIFT) & 0x8000;

	if (bCtl || bShift) 
		return CGXGridWnd::ProcessKeys(pSender, nMessage, nChar, nRepCnt, flags);

	if (nMessage == WM_KEYDOWN)
	{
		switch(nChar)
		{
		case VK_INSERT: 
		case VK_DELETE: 
			return TRUE;
		}
	}
	return CGXGridWnd::ProcessKeys(pSender, nMessage, nChar, nRepCnt, flags);
}

BOOL CMatlByRebarDiaGrid::GetStyleRowCol(ROWCOL nRow, ROWCOL nCol, CGXStyle& style, GXModifyType mt, int nType)
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
	// 사용자 정의 된 Header도 리턴
//   if (nRow == ROWINDEX_HEADER2 || nCol < COLINDEX_HD_LAYER) return TRUE;
//   if (nRow < ROWINDEX_STIRRUP && nCol == COLINDEX_HD_LAYER) return TRUE;

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
//     style
//       .SetInterior(CTBGrid::m_Color.NormalModeBg)
//       .SetTextColor(CTBGrid::m_Color.NormalModeFg);

		return TRUE;
	}

	// Disable되었을 때
	if (!bEnabled)  
	{
		if (bHeaderCol || bHeaderRow) style           // Row or Column Header
			.SetControl(GX_IDS_CTRL_HEADER)
			.SetInterior(CTBGrid::m_Color.RowColHeaderBg)
			.SetTextColor(CTBGrid::m_Color.NormalModeFg);
//     else if (bCurrentRow) style     // Disabled Current Row
//       .SetInterior(CTBGrid::m_Color.DisabledBg)
//       .SetTextColor(CTBGrid::m_Color.ReadOnlyCurrentRowFg);
		else style                      // Disabled Cell
			.SetInterior(CTBGrid::m_Color.DisabledBg)
			.SetTextColor(CTBGrid::m_Color.DisabledFg);

		return TRUE;
	}

	if (bCurrentCell) style
		.SetInterior(CTBGrid::m_Color.NormalCurrentCellBg)
		.SetTextColor(CTBGrid::m_Color.NormalCurrentCellFg);
//   else if (bCurrentRow) style
//     .SetInterior(CTBGrid::m_Color.NormalModeBg)
//     .SetTextColor(CTBGrid::m_Color.NormalModeFg);
	else if (bHeaderCol || bHeaderRow) style    // Enabled Header Col(Only Frozen)
		.SetInterior(CTBGrid::m_Color.NormalModeBg)
		.SetTextColor(CTBGrid::m_Color.NormalModeFg);

	return TRUE;
}

CString CMatlByRebarDiaGrid::Get_StressForm()
{
	CString strUserForUnit, strUserLenUnit;
	m_pUnitCtrl->GetUnitSystemLength(strUserLenUnit);
	m_pUnitCtrl->GetUnitSystemForce(strUserForUnit);

	// N/m^2., kN/m^2., lb/in^2., kip/ft^2., kgf/m^2., tonf/m^2., ...
	CString strStressForm=_T("");
	if(     strUserForUnit==D_UNITSYS_FORCE_NAME_LBF && strUserLenUnit==D_UNITSYS_LENGTH_NAME_FT)	strStressForm = _T("psf");
	else if(strUserForUnit==D_UNITSYS_FORCE_NAME_LBF && strUserLenUnit==D_UNITSYS_LENGTH_NAME_IN)	strStressForm = _T("psi");
	else if(strUserForUnit==D_UNITSYS_FORCE_NAME_KIP && strUserLenUnit==D_UNITSYS_LENGTH_NAME_FT)	strStressForm = _T("ksf");
	else if(strUserForUnit==D_UNITSYS_FORCE_NAME_KIP && strUserLenUnit==D_UNITSYS_LENGTH_NAME_IN)	strStressForm = _T("ksi");
	// Change by ZINU.('04.09.17). If JP, Pa,MPa,KPa -> N/m2,N/mm2,KN/m2.
#if defined(_ORG) || defined(_US) || defined(_RUS) || defined(_CH)
	else if(strUserForUnit==D_UNITSYS_FORCE_NAME_N   && strUserLenUnit==D_UNITSYS_LENGTH_NAME_M )	strStressForm = _T("Pa");
	else if(strUserForUnit==D_UNITSYS_FORCE_NAME_N   && strUserLenUnit==D_UNITSYS_LENGTH_NAME_MM)	strStressForm = _T("MPa");
	else if(strUserForUnit==D_UNITSYS_FORCE_NAME_KN  && strUserLenUnit==D_UNITSYS_LENGTH_NAME_M )	strStressForm = _T("KPa");
#endif
	else	strStressForm = strUserForUnit+_T("/")+strUserLenUnit+CSpecialChar::m_cs2Square;
	return strStressForm;
}