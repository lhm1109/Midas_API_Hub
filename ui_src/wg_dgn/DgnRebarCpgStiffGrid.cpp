// DgnRebarCpgStiffGrid.cpp : implementation file
//
// Add by sshan. ('20061227)
/////////////////////////////////////////////////////////////////////////////
//   1        2         3      4      5       6     
// -------------------------------------------------
//  Width |   T    | Number |Ref.Z |  Z  |  Spacing  

#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnRebarCpgStiffGrid.h"
#include "DgnRebarCpgStiff.h"

#include "..\wg_common\wg_common_TBCommon.h"

#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_ViewBuff.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_DBLib.h"

#include "..\wg_base\wg_base_CompFunc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define DATA_FMT _T("%.15g")
//#define D_RPSC_ARRANGE_ONE     _LS(IDS_DGN_RPSC_ARRANGE_ONE)
//#define D_RPSC_ARRANGE_BOTH    _LS(IDS_DGN_RPSC_ARRANGE_BOTH)
#define D_RPSC_REF_CENTROID    _LS(IDS_DGN_RPSC_REF_CENTROID)
#define D_RPSC_REF_LEFT        _LS(IDS_DGN_RPSC_REF_LEFT)
#define D_RPSC_REF_TOP         _LS(IDS_DGN_RPSC_REF_TOP)
#define D_RPSC_REF_BOP         _LS(IDS_DGN_RPSC_REF_BOTTOM)

/////////////////////////////////////////////////////////////////////////////
// CDgnRebarCpgStiffGrid

CDgnRebarCpgStiffGrid::CDgnRebarCpgStiffGrid(CDgnRebarCpgStiff* pParent)
{
	ASSERT(pParent);
	m_pParent = pParent;

	m_pData = 0;
	m_bSymmetry = FALSE;

	m_nColCount = 6;
	m_nColHeader = 0;   // extra column header(+default header 1)
}

CDgnRebarCpgStiffGrid::~CDgnRebarCpgStiffGrid()
{
}

//--------------------------------------------------------------------------
// Interface
void CDgnRebarCpgStiffGrid::Initialize(CArray<T_CLRS_STIF, T_CLRS_STIF&> *pData, BOOL bSym)
{
	ASSERT(pData);
	m_pData = pData;
	m_bSymmetry = bSym;
	
	CTBBrowserWnd::Initialize();
	SetColInfo();

	GetParam()->EnableMoveCols(FALSE);  // Disable moving column
	GetParam()->EnableMoveRows(FALSE);   // Disable moving row
	GetParam()->EnableTrackRowHeight(FALSE);  // Disable resizing row height

	SetScrollBarMode(SB_VERT, gxnEnabled | gxnEnhanced);

	EnableInsertRowByPressingKey(FALSE);  // Disable inserting row by key
	MakeItemEx();
}

void CDgnRebarCpgStiffGrid::MakeItemEx()
{
	CWaitCursor wait;

	SetModeDisplayOnly(FALSE);

	BOOL bOldLock = LockUpdate(TRUE);

	int nRecCount = m_pData->GetSize();
	SetRowCount(m_nColHeader);
	SetRecordCount(m_nColHeader+nRecCount);

	T_CLRS_STIF data;
	for (int i=0; i<nRecCount; i++)
	{
		data = m_pData->GetAt(i);
		SetValue((ROWCOL)(i+1+m_nColHeader), i, data);
	}

	SetRightValidCell(1+m_nColHeader, 1);

	LockUpdate(bOldLock);
	if (!bOldLock) Redraw();

	ChangeSymmetry(m_bSymmetry);
}

void CDgnRebarCpgStiffGrid::ChangeSymmetry(BOOL bSym)
{
	m_bSymmetry = bSym;
	SetModeDisplayOnly(m_bSymmetry);

	/*
	BOOL bOldLock = LockUpdate(TRUE);

	int nRowCount = GetRowCount();
	SetStyleRange(CGXRange(m_nColHeader+1, 1, nRowCount, m_nColCount), CGXStyle().
		SetEnabled(!m_bSymmetry));

	LockUpdate(bOldLock);
	if (!bOldLock) Redraw();
	*/
}

void CDgnRebarCpgStiffGrid::UpdateBuffer(int nCmd, CArray<int, int> &aOldIndex, CArray<int, int> &aIndex, CArray<T_CLRS_STIF, T_CLRS_STIF&> &aData)
{
	// 현재 Row, Col을 저장 : 나중에 복구하기 위함
	ROWCOL ncRow, ncCol;
	GetCurrentCell(ncRow, ncCol);

	BOOL bOldLock = LockUpdate(TRUE);

	int nSize = aIndex.GetSize();
	for (int i = 0; i < nSize; i++)
	{
		switch(nCmd)
		{
		case(0):
				// 삽입될 위치(nRow)를 찾아 넣는다.
				InsertRecordByKey(aIndex[i], aData[i]);
				break;
		case(1):
				// 삭제될 위치(nRow)를 찾아서 지운다.
				DeleteRecordByKey(aIndex[i], aData[i]);
				break;
		case(2):
				DeleteRecordByKey(aOldIndex[i], aData[i]);
				InsertRecordByKey(aIndex[i], aData[i]);
				break;
		default:
			break;
		}
	} // end of while

	ASSERT(m_pParent);
	m_pParent->RedrawView();

	// 원래 row, col을 복구하려고 시도
	unsigned int nOldMode = m_nEditMode;
	if (nOldMode != CTBBrowserWnd::addnew)
	{ // addnew 상태에서 복구하면 추가되는 Row가 삭제(CancelEdit에서)된다.
		m_nEditMode = CTBBrowserWnd::noMode;
		if (GetRowCount() < ncRow) ncRow = GetRowCount();
		BOOL bSet = SetCurrentCell(ncRow, ncCol);
		if (!bSet) bSet = SetRightValidCell(ncRow, ncCol);
		if (!bSet) bSet = SetLeftValidCell(ncRow, ncCol);
		m_nEditMode = nOldMode;
	}

	LockUpdate(bOldLock);
	if (!bOldLock) Redraw();
}

CString CDgnRebarCpgStiffGrid::GetRefPosStr(int data)
{
	if (data == 0) return D_RPSC_REF_TOP;
	if (data == 1) return D_RPSC_REF_BOP;
	return _T("");
}

int CDgnRebarCpgStiffGrid::GetRefPosData(CString &value)
{
	if (value == D_RPSC_REF_TOP) return 0;
	if (value == D_RPSC_REF_BOP) return 1;
	return -1;
}

//--------------------------------------------------------------------------
// Implementation
void CDgnRebarCpgStiffGrid::SetColInfo()
{
	//int nColNo = m_aColInfo.GetSize();

	BOOL bOldLock = LockUpdate(TRUE);

	// Set Title Header
	SetHeaderTitle();

	// 표준 스타일 변경
	ChangeStandardStyle(CGXStyle()
			.SetFont(CGXFont_GC().SetSize(10))
			.SetVerticalAlignment(DT_BOTTOM)
			.SetAllowEnter(FALSE));

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
	GetParam()->SetNumberedRowHeaders(TRUE);

	// 컬럼 스타일 변경
	SetStyleRange(CGXRange().SetCols(1, m_nColCount), CGXStyle()
			.SetHorizontalAlignment(DT_RIGHT)
			.SetValueType(GX_VT_NUMERIC));
	SetRowHeightByDPI(0, 0, 36);

	// Width, T
	SetStyleRange(CGXRange().SetCols(1, 2), CGXStyle()
			.SetFormat(GX_FMT_FIXED)
			.SetPlaces(2));

	// number
	SetStyleRange(CGXRange().SetCols(3), CGXStyle()
			.SetPlaces(0));

	// ref.Z
	CString csChoiceList = _T("");
	csChoiceList.Format(_T("%s\n%s\n"), D_RPSC_REF_TOP, D_RPSC_REF_BOP);
	SetStyleRange(CGXRange().SetCols(4), CGXStyle()
			.SetControl(GX_IDS_CTRL_TABBED_COMBOBOX)
			.SetChoiceList(csChoiceList)
			.SetUserAttribute(GX_IDS_UA_TABLIST_KEYCOL, _T("0"))
			.SetUserAttribute(GX_IDS_UA_TABLIST_TEXTCOL, _T("0"))
			.SetUserAttribute(GX_IDS_UA_TABLIST_SHOWALLCOLS, _T("1"))
			.SetValueType(GX_VT_STRING)
			.SetHorizontalAlignment(DT_LEFT)
			.SetValue(_T(""))
			);

	// Z, Spacing
	SetStyleRange(CGXRange().SetCols(5, 6), CGXStyle()
			.SetFormat(GX_FMT_FIXED)
			.SetPlaces(2));

	// 현재 셀의 border 설정 변경
	CGXProperties* pProp = GetParam()->GetProperties();
			pProp->SetUserProperty(GX_IDS_OUTLINECURRENTCELL,
		      (CGXStyle) pProp->sInvertNoBorder);

	// 현재 셀 설정
	SetRightValidCell(1+m_nColHeader, 1);
	GetParam()->SetHideCurrentCell(GX_HIDE_NEVER);

	LockUpdate(bOldLock);
	if (!bOldLock) Redraw();
}

void CDgnRebarCpgStiffGrid::SetValue(ROWCOL nRow, int index, T_CLRS_STIF &data)
{
	CStringArray aValue;
	ConvDataToStrArray(data, aValue);
	int nSize = aValue.GetSize();
	for (int i = 0; i < nSize; i++)
		SetExpressionRowCol(nRow, i+1, aValue[i]);
	
	// Key 설정
	SetStyleRange(CGXRange(nRow, 0), 
								CGXStyle().SetUserAttribute(ID_USER_ATTR_KEY, (long)index));

	//ChangeEndColEnable(nRow, data.nLoadType);
	//ChangeEndColEnable2(nRow, data.StartMedialK);
}

void CDgnRebarCpgStiffGrid::SetHeaderTitle()
{
	BOOL bOldLock = LockUpdate(TRUE);
	
	SetColCount(m_nColCount);
	SetRowCount(m_nColHeader);
	if (m_nColHeader > 0)
	{
		// use 1 extra columns as headers ( + standard header at row 0)
		SetFrozenRows(1, 1);
		// Do not draw column headers pressed when moving the current cell
		GetParam( )->GetProperties( )->SetMarkColHeader(FALSE);
	}

	// 단위 : 
	CString csLUnit, csTitle;
	CUnitCtrl::GetUnitSystem(D_UNITSYS_BASE_LENGTH, csLUnit);
	CString aTitle[] = {_LS(IDS_DGN_CPG_STIFF_Width), _LS(IDS_DGN_CPG_STIFF_T), _LS(IDS_DGN_CPG_STIFF_Number), _LS(IDS_DGN_CPG_STIFF_Ref_Z), 
		_LS(IDS_DGN_CPG_STIFF_Z), _LS(IDS_DGN_CPG_STIFF_Spacing)};
	int aWidth[] = {80, 80, 80, 80, 80, 80};

	SetColWidthByDPI(0, 0, 30);
	for (int nRow = 0; nRow <= m_nColHeader; nRow++)
	{
		for (int nCol = 1; nCol <= m_nColCount; nCol++)
		{
			csTitle = aTitle[nCol-1];
			if (nCol == 1 || nCol == 2 || nCol == 5 || nCol == 6) 
				csTitle = csTitle + _T("\n(") + csLUnit + _T(")");      
			SetStyleRange(CGXRange(nRow, (ROWCOL)nCol), CGXStyle()
				.SetValue(csTitle));
			SetColWidthByDPI((ROWCOL)nCol, (ROWCOL)nCol, aWidth[nCol-1]);
		}
	}
	LockUpdate(bOldLock);
	if (!bOldLock) Redraw();
}

void CDgnRebarCpgStiffGrid::ConvDataToStrArray(T_CLRS_STIF &data, CStringArray &aValue)
{
//   1        2         3      4      5       6     
// -------------------------------------------------
//  Width |   T    | Number |Ref.Z |  Z  |  Spacing 

	aValue.SetSize(m_nColCount);
	for (int i=0; i<m_nColCount; i++) aValue[i].Empty();

	aValue[0].Format(DATA_FMT, data.dWidth);
	aValue[1].Format(DATA_FMT, data.dt);
	aValue[2].Format(_T("%d"), data.iBarNum);
	aValue[3] = GetRefPosStr(data.iStaZRef);
	aValue[4].Format(DATA_FMT, data.dStaZ);
	aValue[5].Format(DATA_FMT, data.dPitch);
}

BOOL CDgnRebarCpgStiffGrid::InsertRecordByKey(int index, T_CLRS_STIF &data)
{
	ROWCOL nRowCount = GetRowCount() - 1;  // without append row
	ROWCOL nRow = m_nColHeader + index + 1;
	if (nRow > nRowCount) nRow = nRowCount+1;

	InsertRecord(nRow);
	SetValue(nRow, index, data);
	return TRUE;
}

BOOL CDgnRebarCpgStiffGrid::DeleteRecordByKey(int index, T_CLRS_STIF &data)
{
	ROWCOL ncRow, ncCol;
	BOOL bCurCell = GetCurrentCell(ncRow, ncCol);
	ROWCOL nRowCount = GetRowCount() - 1;  // without append row
	ROWCOL nRow = m_nColHeader + index + 1;
	ASSERT(nRow <= nRowCount);

	if (bCurCell && nRow == ncRow && m_nEditMode == edit) CancelEdit();
	DeleteRecord(nRow); 
	return TRUE;
}

// 인자로 넘어온 배열에 있는 모든 row를 Select 상태로 변경한다.
// 기존의 Select된 row는 없어진다.
void CDgnRebarCpgStiffGrid::SelectGridRow(long* aRow, long nSize)
{
	// Row 배열을 오름차순으로 sorting한다.
	qsort((void*)aRow, nSize, sizeof(long), CCompFunc::longAsc);

	CGXRangeList* pSelList = GetParam()->GetRangeList();
	pSelList->DeleteAll();  // 기존 선택을 해제한다.

	for (long i = 0; i < nSize; )
	{
		CGXRange* pRange = new CGXRange();
		// 연속된 Row를 찾아서 1개의 Range로 만든다.
        long j = 0;
		for (j = i+1; j < nSize; j++)
		{
			if (aRow[j] != aRow[j-1] + 1) break;
		}
		pRange->SetRows(aRow[i], aRow[j-1]);
		pSelList->AddTail(pRange);
		i = j;
	}
}

BOOL CDgnRebarCpgStiffGrid::GetValue(ROWCOL nRow, T_CLRS_STIF &data)
{
	data.Initialize();
	
	CStringArray aValue;
	for (int i = 0; i < m_nColCount; i++)
		aValue.Add(GetValueRowCol(nRow, i+1));
	return ConvStrArrayToData(aValue, data);
}


//--------------------------------------------------------------------------
// Overriadable
// Return은 항상 SetFieldValidation으로 한다.
// CTBBrowserWnd::ProcessKeys 참조
BOOL CDgnRebarCpgStiffGrid::OnValidateField(ROWCOL nRow, ROWCOL nCol)
{
	CString value;
	int nColID;
	VERIFY(GetColumnIDFor(nCol, nColID));
	value = GetValueRowCol(nRow, nCol);

	if (!ValidateField(value, nColID)) return SetFieldValidation(FALSE);
	return SetFieldValidation(TRUE);
}

// Return은 항상 SetRecordValidation으로 한다.
// CTBBrowserWnd::ProcessKeys 참조
BOOL CDgnRebarCpgStiffGrid::OnValidateRecord(ROWCOL nRow)
{
	// CStringArray 레코드를 만들어 validation을 검사한다.
	CRowColArray aCols;
	CStringArray value;
	int nColCount = GetColCount();
	aCols.SetSize(nColCount);
	value.SetSize(nColCount);
	for (int i = 0; i < nColCount; i++)
	{
		aCols[i] = i;
		//VERIFY(GetColumnNoFor(i, aCols[i]));
		value[i] = GetValueRowCol(nRow, i+1);
	}
	if (!ValidateRecord(value, aCols)) return SetRecordValidation(FALSE);
	return SetRecordValidation(TRUE);
}

BOOL CDgnRebarCpgStiffGrid::ValidateField(CString& value, int nColID)
{
//   1        2         3      4      5       6     
// -------------------------------------------------
//  Width |   T    | Number |Ref.Z |  Z  |  Spacing 

	if (nColID >= GetColCount())
		return FALSE;

	int nErrCode = 0;

	if (nColID == 3) return TRUE;
	else if (nColID == 2)
	{
		if (!IsValidLongNumber(value, nErrCode))
		{
			switch(nErrCode)
			{
			case 1: SetWarningText(_LS(IDS_DGN__ADDD__Invalid_formula_)); break;
			case 2: case 4: SetWarningText(_LS(IDS_DGN__ADDD__Input_integer_value_)); break;
			case 3: SetWarningText(_LS(IDS_DGN__ADDD__Value_is_too_big_)); break;
			}
			return FALSE;
		}
	}
	else if (nColID == 0 || nColID == 1 || nColID == 4 || nColID == 5)
	{
		if (!IsValidDoubleNumber(value, nErrCode))
		{
			switch(nErrCode)
			{
			case 1: SetWarningText(_LS(IDS_DGN__ADDD__Invalid_formula_)); break;
			case 2: SetWarningText(_LS(IDS_DGN__ADDD__Character_exists_in_double_value)); break;
			case 3: SetWarningText(_LS(IDS_DGN__ADDD__Value_is_too_big_)); break;
			}
			return FALSE;
		}
	}
	else return FALSE;

	return TRUE;
}

BOOL CDgnRebarCpgStiffGrid::ValidateRecord(CStringArray &aValue, CRowColArray &aCols)
{
	CString aTitle[] = {_LS(IDS_DGN_CPG_STIFF_Width), _LS(IDS_DGN_CPG_STIFF_T), _LS(IDS_DGN_CPG_STIFF_Number), _LS(IDS_DGN_CPG_STIFF_Ref_Z),
		_LS(IDS_DGN_CPG_STIFF_Z), _LS(IDS_DGN_CPG_STIFF_Spacing)};

	CString msg;
	int ColCount = aCols.GetSize();
	for (int i=0; i<ColCount; i++)
	{
		//nPos = aCols[i]-1;
		if (aValue[i] == _T(""))
		{
			msg.Format(_T("%s has not been entered."), aTitle[i]);
			AfxMessageBox(msg);
			return SetRecordValidation(FALSE);
		}
		else if (!ValidateField(aValue[i], i)) return FALSE;
	}
	return TRUE;
}

BOOL CDgnRebarCpgStiffGrid::OnAddNewRecord(ROWCOL nRow)
{
	BOOL bOldLock = LockUpdate(TRUE);
	if (!AddToDB(nRow))
	{
		LockUpdate(bOldLock);
		return SetRecordValidation(FALSE);
	}
	CancelEdit();
	//SetEnableDisableLcomCol();  // Add의 경우 여기서 한 번 더 호출해야 한다.
	LockUpdate(bOldLock);
	if (!bOldLock) Redraw();
	GetBrowseParam()->m_nRecordCount -= 1; 
	return SetRecordValidation(TRUE);
}

BOOL CDgnRebarCpgStiffGrid::AddToDB(ROWCOL nRow)
{
	int nIndex;
	T_CLRS_STIF data;
	
	nIndex = m_pData->GetSize();
	GetValue(nRow, data);

	return AddData(nIndex, data);
}

BOOL CDgnRebarCpgStiffGrid::AddData(int nIndex, T_CLRS_STIF &data)
{
	CArray<int, int> aOldIndex, aIndex;
	CArray<T_CLRS_STIF, T_CLRS_STIF&> aData;
	aOldIndex.Add(nIndex);
	aIndex.Add(nIndex);
	aData.Add(data);

	m_pData->InsertAt(nIndex, data);
	UpdateBuffer(0, aOldIndex, aIndex, aData);
	
	return TRUE;
}

BOOL CDgnRebarCpgStiffGrid::OnModifyRecord(ROWCOL nRow)
{
	// 수정한다. 실패하면 에러 리턴
	BOOL bOldLock = LockUpdate(TRUE);
	if (!ModifyToDB(nRow))
	{
		LockUpdate(bOldLock);
		return SetRecordValidation(FALSE);
	}
	LockUpdate(bOldLock);
	if (!bOldLock) Redraw();
	return SetRecordValidation(TRUE);
}

BOOL CDgnRebarCpgStiffGrid::ModifyToDB(ROWCOL nRow)
{
	int nIndex;
	T_CLRS_STIF data;

	nIndex = nRow - m_nColHeader - 1;
	ASSERT(nIndex >= 0 && nIndex < m_pData->GetSize());
	GetValue(nRow, data);

	return ModifyData(nIndex, nIndex, data);
}

BOOL CDgnRebarCpgStiffGrid::ModifyData(int nOldIndex, int nIndex, T_CLRS_STIF &data)
{
	CArray<int, int> aOldIndex, aIndex;
	CArray<T_CLRS_STIF, T_CLRS_STIF&> aData;
	aOldIndex.Add(nOldIndex);
	aIndex.Add(nIndex);
	aData.Add(data);

	m_pData->SetAt(nIndex, data);
	UpdateBuffer(2, aOldIndex, aIndex, aData);
	
	return TRUE;
}

BOOL CDgnRebarCpgStiffGrid::OnDeleteRecord(ROWCOL nRow)
{
	// save current row
	ROWCOL ncRow, ncCol;
	GetCurrentCell(ncRow, ncCol);

	if (m_nEditMode == addnew) return CancelEdit(); // addnew 상태이면 그냥 취소
	CRowColArray awRow;
	awRow.Add(nRow);
	if (!DeleteFromDB(awRow)) return FALSE;

	// restore current row
	if (GetRowCount() > ncRow) SetCurrentCell(ncRow, ncCol);
	return TRUE;
}

BOOL CDgnRebarCpgStiffGrid::OnDeleteRecords(CRowColArray &awRows)
{
	// save current row
	ROWCOL ncRow, ncCol;
	GetCurrentCell(ncRow, ncCol);

	BOOL bNewRecordIncluded = FALSE;
	if (m_nEditMode == addnew)
	{
		// addnew 상태에서 현재 행이 포함되어 있으면 뺀다.
		for (int i = 0; i < awRows.GetSize(); i++)
		{
			if (GetBrowseParam()->m_nCurrentRow == awRows.GetAt(i))
			{ awRows.RemoveAt(i); bNewRecordIncluded = TRUE; break; }
		}
	}
 
	if (!DeleteFromDB(awRows)) return FALSE;
	if (bNewRecordIncluded) VERIFY(CancelEdit());

	// restore current row
	if (GetRowCount() > ncRow) SetCurrentCell(ncRow, ncCol);
	return TRUE;
}

BOOL CDgnRebarCpgStiffGrid::DeleteFromDB(CRowColArray &awRows)
{
	int nIndex;
	CArray<int, int> aIndex;
	int nSize = awRows.GetSize();
	if (nSize == 0) return FALSE;
	//aIndex.SetSize(nSize);
	for (int i = nSize-1; i >= 0; i--)
	{
		if (awRows[i] <= m_nColHeader) continue;
		nIndex = awRows[i] - m_nColHeader - 1;
		ASSERT(nIndex >= 0 && nIndex < m_pData->GetSize());
		m_pData->RemoveAt(nIndex);
		aIndex.Add(nIndex);
	}

	CArray<int, int> aOldIndex;
	CArray<T_CLRS_STIF, T_CLRS_STIF&> aData;
	aOldIndex.Copy(aIndex); 
	T_CLRS_STIF data; 
	data.Initialize();
	for (int i=0; i<aIndex.GetSize(); i++) aData.Add(data);

	UpdateBuffer(1, aOldIndex, aIndex, aData);

	return TRUE;
}

//--------------------------------------------------------------------------
// Grid's Standard Overridable Functions
// Record선택시 title row와 append row는 제거한다.
void CDgnRebarCpgStiffGrid::OnChangedSelection(const CGXRange *changedRect, BOOL bIsDraggin, BOOL bKey)
{
	long nSize, nTmpSize;
	if (changedRect)
	{
		if (!bIsDraggin)
		{
			CRowColArray awRows;
			//if ((nSize = GetSelectedRows(awRows, TRUE, FALSE)) == 0)
			nSize = GetSelectedRows(awRows, TRUE, FALSE);
			// 전체 테이블 선택시 헤더와 append row는 제거한다.
			for (long i = nSize-1; i >= 0; i--)
				if (awRows[i] == GetRowCount() || awRows[i] <= m_nColHeader)
					awRows.RemoveAt(i);
			nTmpSize = awRows.GetSize();
			if (nSize != nTmpSize)
			{
				SelectGridRow((long*)awRows.GetData(), nTmpSize);
				if (nTmpSize + 2 != nSize)  // append row가 선택된 경우는 다시 그려준다.
				{
					BOOL bOldLock = LockUpdate(FALSE);
					if (!bOldLock) Redraw();
					LockUpdate(bOldLock);
				}
			}
		}
	}
}

/*
// LoadComb의 위치를 이동(Serial No가 변경)한다.
BOOL CDgnRebarCpgStiffGrid::OnSelDragRowsDrop(ROWCOL nFirstRow, ROWCOL nLastRow, ROWCOL nDestRow)
{
	// 수정 모드이면 에러 리턴
	if (IsEdit()) 
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Can_t_Move_row_while_editing_));
		return FALSE;
	}

	// Append Row의 뒤면 맨 마지막으로 이동하는 것으로 한다.
	if (nDestRow == GetRowCount()+1) nDestRow--;  

	// 이동의 효과가 없는 곳으로 이동(선택범위내로 다시 이동)하면 그냥 리턴
	if (nDestRow >= nFirstRow && nDestRow <= nLastRow+1) return FALSE;

	// 연속 선택이 아니면 이동을 금지시킨다.
	CRowColArray awRows;
	GetSelectedRows(awRows, TRUE, FALSE);
	if (awRows.GetSize() != nLastRow-nFirstRow+1) return FALSE;


	int nStart, nInc;
	nStart = (int)nDestRow;
	if (nDestRow < nFirstRow)  // 선택 영역 앞으로 이동
	{
		nInc = 1;
	}
	else  // 선택 영역 뒤로 이동
	{
		nStart--;
		nInc = 0;
	}

	CArray<CString, CString&> aLoadCombName;
	CArray<T_LCOM_D, T_LCOM_D&> aData;
	CString LoadCombName;
	T_LCOM_D data;
	for (int i = nFirstRow; i <= nLastRow; i++)
	{
		LoadCombName = GetValueRowColBeforeEdit(i, 2);
		m_pDoc->m_pAttrCtrl->GetLcom(m_nLcomType, LoadCombName, data);
		data.LoadCombId = nStart;
		nStart += nInc;

		aLoadCombName.Add(data.LoadCombName);
		aData.Add(data);
	}
	m_pDoc->m_pDataCtrl->ModifyLcom(m_nLcomType, aLoadCombName, aData);
	return FALSE;
}
*/

// 항상 FALSE를 리턴해야 한다.
// 왜냐 하면 Grid가 Paste 작업을 해서는 안 되기 때문
BOOL CDgnRebarCpgStiffGrid::OnPasteFromClipboard(const CGXRange &range)
{
	if (IsLockEdit()) return FALSE; // display only mode이면 바로 리턴

	// 임시로 CGXGridWnd를 하나 만들어 Paste를 한 후 작업을 한다.
	CGXGridWnd x;
	int nPasteMode;
	int nPastedCols;

	// 임시 GridWnd에 Paste 작업을 한다.
	// 먼저 Default Value Array를 만든다.
	CString* aDefVal = new CString[m_nColCount];
	for (int ix = 0; ix < m_nColCount; ix++)
		aDefVal[ix] = GetDefValue(ix);

	if (!PasteFromClipboard(range, aDefVal, 0, x, nPasteMode, nPastedCols))
	{
		delete []aDefVal;
		x.DestroyWindow();
		DisplayWarningText();
		return FALSE;
	}
	delete []aDefVal;

	CStringArray value;
	value.SetSize(m_nColCount);
	CArray<T_CLRS_STIF, T_CLRS_STIF&> aData;
	CRowColArray aCols;
	aCols.SetSize(m_nColCount);
	for (int i = 0; i < m_nColCount; i++)
		aCols[i] = i;
		//VERIFY(GetColumnNoFor(i, aCols[i]));

	// DB 수정이 되지 않는 경우이므로 Copy되는 Field Validation만 수행한다.
	if (nPasteMode == TB_PASTE_MODE_ONMODIFY)
	{
		int nColID;
		for (int j = range.left; j < range.left+nPastedCols; j++)
		{
			VERIFY(GetColumnIDFor(j, nColID));
			value[j-1] = x.GetValueRowCol(1, j);
			if (!ValidateField(value[j-1], nColID))
			{
				x.DestroyWindow();
				DisplayWarningText();
				return FALSE;
			}
		}
	}
	else // DB에 추가 또는 수정이 일어나는 경우이므로 Record Validation을 한다.
	{
		int nxRowCount = x.GetRowCount();
		for (int i = 1; i <= nxRowCount; i++)
		{
			// Table에서 레코드 값을 가져온다.
			int nxColCount = x.GetColCount();
			for (int j = 1; j <= nxColCount; j++)
				value[j-1] = x.GetValueRowCol(i, j);
			// String Array에 대해 Validation 검사. (필드, 레코드 전체)
			if (!MakeDataArray(value, aCols, aData))
			{
				x.DestroyWindow();
				if (m_sWarningText.IsEmpty())
					SetWarningText(_LS(IDS_DGN__ADDD__Invalid_data__Operation_has_been_));
				DisplayWarningText();
				return FALSE;
			}
		}
	}
	
	// 모드에 따라 Append 또는 Modify를 한다.
	if (nPasteMode == TB_PASTE_MODE_APPEND) // DB에 추가된다.
	{
		CArray<int, int> aIndex;
		for (int i=0; i<aData.GetSize(); i++)
		{
			m_pData->Add(aData[i]);
			aIndex.Add(m_pData->GetSize());
		}
		UpdateBuffer(0, aIndex, aIndex, aData);
	}
	else if (nPasteMode == TB_PASTE_MODE_ONMODIFY)  // DB 수정 없다.
	{
		// 직접 그리드를 수정한다.
		CArray<int, int> aColID;
		aColID.SetSize(nPastedCols);
		for (int j = 0; j < nPastedCols; j++)
			VERIFY(GetColumnIDFor(range.left+j, aColID[j]));
		qsort((void*)aColID.GetData(), aColID.GetSize(), sizeof(UINT), CCompFunc::intAsc);
		ROWCOL nColNo;
		for (int j = 0; j < nPastedCols; j++)
		{
			VERIFY(GetColumnNoFor(aColID[j], nColNo));
			SetExpressionRowCol(range.top, nColNo, value[nColNo-1]);
			OnModifyCell(range.top, nColNo);
		}
	}   
	else if (nPasteMode == TB_PASTE_MODE_MODIFY) // DB가 수정된다.
	{
		CArray<int, int> aIndex;
		int nxRowCount = x.GetRowCount();
		aIndex.SetSize(nxRowCount);
		for (int i = 0; i < nxRowCount; i++)
		{
			aIndex[i] = range.top+i-(m_nColHeader+1);
			ASSERT(aIndex[i] >= 0 && aIndex[i] < m_pData->GetSize());
			m_pData->SetAt(aIndex[i], aData[i]);
		}
		UpdateBuffer(2, aIndex, aIndex, aData);
	}

	x.DestroyWindow();
	return FALSE;   // 실제 Paste는 하지 않는다.
}

BOOL CDgnRebarCpgStiffGrid::PasteFromClipboard(const CGXRange &range, CString* aDefVal, int nKeyCol, 
																				 CGXGridWnd &x, int &nPasteMode, int &nPastedCols)
{
	CGXRange r;
	r.top = r.bottom = 1;
	r.left = r.right = 1;
	x.Create(WS_CHILD, CRect(0, 0, 1, 1), GetParent(), 0);
	x.Initialize();
	x.GetParam()->EnableUndo(FALSE);
	x.LockUpdate(TRUE);
	x.SetRowCount(0); x.SetColCount(1);
	x.OnPasteFromClipboard(r);

	// Paste된 범위를 구한다.
	r.top = 1; r.left = 1;
	r.bottom = x.GetRowCount(); r.right = x.GetColCount();

	// 규칙
	// 범위가 다르면 경고 메세지를 보내고 리턴
	// Paste된 내용이 레코드의 모든 필드를 만족하지 못하면 Default 값으로 채운다.
	// column 구분이 없기 때문에 칼럼 by 칼럼으로 Paste한다.
	// 즉 column의 위치가 변경되었으면 변경된 대로 Paste 한다.
	// 고의적으로 Hide된 칼럼은 Default 값으로 채운다.

	// 용어
	// 원본 범위 : Copy해서 Clipboard로 들어간 row, col 갯수
	// 대상 범위 : User가 마우스로 선택해 놓은 범위

	// 범위에 고의로 숨겨진 Column(HideByIntend)이 포함되어 있으면 이 칼럼을
	// 삽입하고 Default Value로 채운다.
	CArray<int, int> aHideCol;
	int nColCount = GetColCount();
	int nPasteColCount = r.GetWidth();
	for (int k = 1; k < nPasteColCount; k++)
	{
		if (range.left+k < nColCount && IsHiddenByIntend(range.left+k))
		{
			x.InsertCols(k+1, 1);
			aHideCol.Add(range.left+k);
			r.right++;
			nPasteColCount++;
		}
	}

	CString sErrMsg = _T("");
	ROWCOL nRow, nCol;
	int nLeftInsertWidth, nRightInsertWidth;
	VERIFY(GetCurrentCell(nRow, nCol));
	// 대상 범위가 지정되지 않은 경우
	// 현재 셀이 있는 위치를 기준으로 한다.
	if (range.GetHeight() == 1 && range.GetWidth() == 1)
	{
		// 대상 범위가 Append Row를 넘어서면 수정과 추가가 중복되므로 에러
		if (nRow != GetRowCount() && (nRow + r.GetHeight()) > GetRowCount())
		{
			//sErrMsg = _T("수정과 추가가 혼합되게 됩니다. 위치를 다시 지정하세요.");
			sErrMsg = _LS(IDS_TB_PASTE_ADD_MODIFY_MIX);
			goto END_OF_PASTE;
		}

		// 대상 범위가 Column 한계를 넘어서면 에러
		if (nCol + r.GetWidth() - 1 > GetColCount())
		{
			//sErrMsg = _T("붙여 넣을 범위가 칼럼 범위를 초과합니다. 위치를 다시 지정하세요.");
			sErrMsg = _LS(IDS_TB_PASTE_EXCEED_COLUMN);
			goto END_OF_PASTE;
		}

		// 편집 모드이고 원본 범위가 2줄 이상이면 에러
		if (IsEdit() && r.GetHeight() > 1)
		{
			//sErrMsg = _T("현재 편집중인 레코드가 복사 범위에 포함되어 있습니다.");
			sErrMsg = _LS(IDS_TB_PASTE_EDIT_DATA_INCLUDED);
			goto END_OF_PASTE;
		}

		// 현재 셀이 Append Row에 있으면 모두 추가(DB에 추가된다.)
		if (nRow == GetRowCount())
			nPasteMode = TB_PASTE_MODE_APPEND;
		// 레코드 수정상태를 유지(DB를 수정하지 않는다.)
		else if (IsEdit()) nPasteMode = TB_PASTE_MODE_ONMODIFY;
		// 모두 수정(DB에서 수정이 된다.)
		else nPasteMode = TB_PASTE_MODE_MODIFY;
	}
	// 대상 범위가 지정된 경우 : 원본과 같은 범위, Fill Mode(원본 1줄, 동일 칸수)
	else
	{
		// 편집 모드에서 편집 레코드가 대상 범위에 포함되어 있고 2줄 이상이면 에러 표시
		if (IsEdit() && nRow >= range.top && nRow <= range.bottom && range.GetHeight() > 1)
		{
			//sErrMsg = _T("현재 편집중인 레코드가 복사 범위에 포함되어 있습니다.");
			sErrMsg = _LS(IDS_TB_PASTE_EDIT_DATA_INCLUDED);
			goto END_OF_PASTE;
		}
		
		// Append Row가 범위에 포함된 경우 다른 Row도 범위에 포함되어 있으면 에러
		if (range.bottom == GetRowCount() && range.GetHeight() > 1)
		{
			//sErrMsg = _T("수정과 추가가 혼합되게 됩니다. 위치를 다시 지정하세요.");
			sErrMsg = _LS(IDS_TB_PASTE_ADD_MODIFY_MIX);
			goto END_OF_PASTE;
		}

		// Fill 조건을 검사한다.
		// 원본이 1줄 이상이면 에러, 칸수가 달라도 에러가 된다.
		BOOL bFillMode = FALSE;
		if (range.GetHeight() != r.GetHeight() || range.GetWidth() != r.GetWidth())
		{
			// Fill Mode가 안 되는 조건
			if (r.GetHeight() > 1 || range.GetWidth() != r.GetWidth())
			{
				//sErrMsg = _T("대상 범위를 지정한 경우는 원본 범위의 크기와 같아야 합니다.");
				sErrMsg = _LS(IDS_TB_PASTE_RANGE_MISMATCH);
				goto END_OF_PASTE;
			}
			// 대상 범위가 1줄 이상이면 Fill Mode, 아니면 Paste와 동일하다.
			if (range.GetHeight() > 1) bFillMode = TRUE;
		}

		// Append 단독 Row면 레코드 추가
		if (range.bottom == GetRowCount()) nPasteMode = TB_PASTE_MODE_APPEND; 
		// 레코드 수정 상태를 유지하며 DB에서 수정은 일어나지 않는다.
		else if (IsEdit() && range.top == nRow) nPasteMode = TB_PASTE_MODE_ONMODIFY;
		// 아니면 레코드 수정이 된다.
		else nPasteMode = TB_PASTE_MODE_MODIFY;

		// Fill Mode인지 검사한다.
		if (bFillMode)
		{
			// 대상 범위의 줄수 만큼 Row를 만들어 채운다.
			x.SetRowCount(range.GetHeight());
			CString value;
			for (int i = 0; i < r.GetWidth(); i++)  // Column Wise Copy...
			{
				value = x.GetValueRowCol(1, i+1);
				for (int j = 1; j < range.GetHeight(); j++)
					x.SetExpressionRowCol(j+1, i+1, value);
			}
			r.bottom = x.GetRowCount();
		}
	}

	// 레코드 또는 레코드 셋을 만든다.
	// 왼쪽과 오른쪽에 모자라는 칼럼수를 삽입한다.
	nPastedCols = r.GetWidth();
	nLeftInsertWidth = range.left-1;
	if (nLeftInsertWidth > 0) x.InsertCols(1, nLeftInsertWidth);
	nRightInsertWidth = GetColCount()-x.GetColCount();
	if (nRightInsertWidth > 0) x.InsertCols(x.GetColCount()+1, nRightInsertWidth);
	// 모자라는 값을 카피한다. (Append는 Default값, Modify는 기존 값)
	if (nPasteMode == TB_PASTE_MODE_APPEND)
	{
		int nID;
		CString value;
		// 속도 향상을 위해 column별로 복사
		// 왼쪽 칼럼 카피 
		for (int j = 1; j <= nLeftInsertWidth; j++)
		{
			nID = j-1;
			if (nID == nKeyCol) value = _T("");
			else value = aDefVal[nID-1];
			for (int i = 1; i <= r.GetHeight(); i++)
				x.SetExpressionRowCol(i, j, value);
		}
		// Copy된 영역중 고의로 숨겨진 칼럼에 Default값 채우기
		for (int j = 0; j < aHideCol.GetSize(); j++)
		{
			nID = aHideCol[j]-1;
			for (int i = 1; i <= r.GetHeight(); i++)
				x.SetExpressionRowCol(i, aHideCol[j], aDefVal[nID-1]);
		}
		// 오른쪽 칼럼 카피
		for (int j = nLeftInsertWidth+nPastedCols+1; j <= GetColCount(); j++)
		{
			nID = j-1;
			if (nID == nKeyCol) value = _T("");
			else value = aDefVal[nID-1];
			for (int i = 1; i <= r.GetHeight(); i++)
				x.SetExpressionRowCol(i, j, value);
		}
	}
	else if (nPasteMode == TB_PASTE_MODE_ONMODIFY || nPasteMode == TB_PASTE_MODE_MODIFY)
	{
		CString value;
		// 왼쪽 칼럼 카피 
		for (int j = 1; j <= nLeftInsertWidth; j++)
		{
			for (int i = 1; i <= r.GetHeight(); i++)
			{
				value = GetValueRowCol(range.top-1+i, j);
				x.SetExpressionRowCol(i, j, value);
			}
		}
		// Copy된 영역중 고의로 숨겨진 칼럼에 기존의 값 채우기
		for (int j = 0; j < aHideCol.GetSize(); j++)
		{
			for (int i = 1; i <= r.GetHeight(); i++)
			{
				value = GetValueRowCol(range.top-1+i, aHideCol[j]);
				x.SetExpressionRowCol(i, aHideCol[j], value);
			}
		}
		// 오른쪽 칼럼 카피
		for (int j = nLeftInsertWidth+nPastedCols+1; j <= GetColCount(); j++)
		{
			for (int i = 1; i <= r.GetHeight(); i++)
			{
				value = GetValueRowCol(range.top-1+i, j);
				x.SetExpressionRowCol(i, j, value);
			}
		}
	}

END_OF_PASTE:
	if (!sErrMsg.IsEmpty())
	{
		SetWarningText(sErrMsg);
		return FALSE;
	}
	return TRUE;
}

BOOL CDgnRebarCpgStiffGrid::GetStyleRowCol(ROWCOL nRow, ROWCOL nCol, CGXStyle& style, GXModifyType mt, int nType)
{
	BOOL bReadOnlyMode;
	BOOL bCurrentRow;
	BOOL bCurrentCell;
	BOOL bEnabled;
	BOOL bReadOnly;
	BOOL bAppendRow;
	BOOL bHeaderCol;

	ROWCOL ncRow, ncCol;    // 현재 Row, Col
	CGXGridWnd::GetStyleRowCol(nRow, nCol, style, mt, nType);

	// 현재 셀의 그리드 좌표(Row, Col 번호)를 얻어온다.
	if (GetCurrentCell(ncRow, ncCol)) GetBrowseParam()->m_nCurrentRow = ncRow;
	else ncRow = GetRowCount()+1; // 현재 셀이 없는 경우 : ncRow를 범위 밖으로 설정

	// 현재 셀이 이동되면 새로 그려준다.
	m_bRefreshOnSetCurrentCell = TRUE;

	// Row나 Col이 1보다 작으면 리턴한다.(Standard Row or Column Header)
	//if (nRow < 1 || nCol < 1) return TRUE;
	if (nRow <= m_nColHeader || nCol < 1) return TRUE;

	// Get Current Table and Cell State
	bReadOnlyMode = IsLockEdit();
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

	if (style.GetIncludeReadOnly()) // 셀에 readonly정보 포함 여부 알아냄
	{
		bReadOnly = style.GetReadOnly();
	}
	else // Column style에서 readonly 정보 알아냄
	{
		CGXStyle styleEx;
		GetColStyle(nCol, styleEx);
		bReadOnly = (styleEx.GetIncludeReadOnly() && styleEx.GetReadOnly());
	}

	bAppendRow = IsAppendRow(nRow);
	bHeaderCol = IsFrozenCol(nCol);

	// Printing 모드이면 헤더 Column이 프린트 되기 위해 다음과 같이 리턴한다.
	if (IsPrinting())
	{
		if (bHeaderCol) style.SetControl(GX_IDS_CTRL_HEADER);
		return TRUE;
	}

	// 일반 셀이 가장 많으므로 빨리 검사하고 바로 리턴할 수 있게 코드를 앞으로 뺀다.
	if (bEnabled && !bReadOnly && !bCurrentCell && !bCurrentRow && !bAppendRow && !bHeaderCol)
	{
		if (bReadOnlyMode) style
			.SetInterior(m_Color.ReadOnlyModeBg)
			.SetTextColor(m_Color.ReadOnlyModeFg);
		else style
			.SetInterior(m_Color.NormalModeBg)
			.SetTextColor(m_Color.NormalModeFg);

		return TRUE;
	}

	// Disable되었을 때
	if (!bEnabled)  
	{
		if (bHeaderCol) style           // Row or Column Header
			.SetControl(GX_IDS_CTRL_HEADER)
			.SetInterior(m_Color.RowColHeaderBg)
			.SetTextColor(m_Color.NormalModeFg);
		else if (bCurrentRow) style     // Disabled Current Row
			.SetInterior(m_Color.DisabledBg)
			.SetTextColor(m_Color.ReadOnlyCurrentRowFg);
		else style                      // Disabled Cell
			.SetInterior(m_Color.DisabledBg)
			.SetTextColor(m_Color.DisabledFg);

		if (!bHeaderCol && bCurrentRow) SetCurrentRowStyle(style);

		return TRUE;
	}

	// Disable안 되고 ReadOnly만 설정되었을 때
	if (bReadOnly)
	{
		if (bCurrentCell) style         // Current Cell
			.SetInterior(m_Color.ReadOnlyCurrentCellBg)
			.SetTextColor(m_Color.ReadOnlyCurrentCellFg);
		else if (bCurrentRow) style     // Current Row
			.SetInterior(m_Color.ReadOnlyCurrentRowBg)
			.SetTextColor(m_Color.ReadOnlyCurrentRowFg);
		else style                      // Readonly Cell
			.SetInterior(m_Color.ReadOnlyBg)
			.SetTextColor(m_Color.ReadOnlyFg);

		if (bCurrentRow) SetCurrentRowStyle(style);

		return TRUE;
	}

	// Append Row가 Disable되었을 때(EnableAppendRow(FALSE)로 금지된 경우)
	if (bAppendRow && !m_bEnabledAppend)  // Disabled Apppend Row
	{
		if (bCurrentCell) style
			.SetInterior(m_Color.ReadOnlyCurrentCellBg)
			.SetTextColor(m_Color.ReadOnlyCurrentCellFg);
		else if (bCurrentRow) style
			.SetInterior(m_Color.DisabledAppendRowBg)
			.SetTextColor(m_Color.ReadOnlyCurrentRowFg);
		else style
			.SetInterior(m_Color.DisabledAppendRowBg)
			.SetTextColor(m_Color.ReadOnlyModeFg);

		style.SetControl(GX_IDS_CTRL_STATIC);  // 수정이 안되도록 control을 static으로 바꾼다.

		if (bCurrentRow) SetCurrentRowStyle(style);

		return TRUE;
	}
	
	// Disable 안되고, ReadOnly 아니고, Append가 Disable 되지 않은 경우
	// Current Cell or Current Row or Enabled Append Row인 경우이다.
	if (bReadOnlyMode)
	{
		if (bCurrentCell) style
			.SetInterior(m_Color.ReadOnlyCurrentCellBg)
			.SetTextColor(m_Color.ReadOnlyCurrentCellFg);
		else if (bCurrentRow) style
			.SetInterior(m_Color.ReadOnlyCurrentRowBg)
			.SetTextColor(m_Color.ReadOnlyCurrentRowFg);
		else if (bAppendRow) style
			.SetInterior(m_Color.ReadOnlyModeBg)
			.SetTextColor(m_Color.ReadOnlyModeFg);
		else if (bHeaderCol) style    // Enabled Header Col(Only Frozen)
			.SetInterior(m_Color.ReadOnlyModeBg)
			.SetTextColor(m_Color.ReadOnlyModeFg);

		if (bCurrentRow || bHeaderCol) SetCurrentRowStyle(style);

		return TRUE;
	}
	else
	{
		if (bCurrentCell) style
			.SetInterior(m_Color.NormalCurrentCellBg)
			.SetTextColor(m_Color.NormalCurrentCellFg);
		else if (bCurrentRow) style
			.SetInterior(m_Color.NormalCurrentRowBg)
			.SetTextColor(m_Color.NormalCurrentRowFg);
		else if (bAppendRow) style
			.SetInterior(m_Color.NormalModeBg)
			.SetTextColor(m_Color.NormalModeFg);
		else if (bHeaderCol) style    // Enabled Header Col(Only Frozen)
			.SetInterior(m_Color.NormalModeBg)
			.SetTextColor(m_Color.NormalModeFg);

		if (bCurrentRow || bHeaderCol) SetCurrentRowStyle(style);

		return TRUE;
	}

	return TRUE;
}

BOOL CDgnRebarCpgStiffGrid::MakeDataArray(CStringArray& value, CRowColArray& awCols, 
																		CArray<T_CLRS_STIF, T_CLRS_STIF&>& aData)
{
	T_CLRS_STIF data;
	data.Initialize();
	if (!ValidateAndMakeRecord(value, awCols, data)) return FALSE;
	aData.Add(data);
	return TRUE;
}

BOOL CDgnRebarCpgStiffGrid::ValidateAndMakeRecord(CStringArray &value, CRowColArray& aCols, T_CLRS_STIF &data)
{
	// 레코드에 대해 Validation을 검사한다.
	if (!ValidateRecord(value, aCols))
		return FALSE;

	// 레코드를 만들어서 리턴한다.
	ConvStrArrayToData(value, data);
	return TRUE;
}

BOOL CDgnRebarCpgStiffGrid::ConvStrArrayToData(CStringArray &aValue, T_CLRS_STIF& data)
{
//   1        2         3      4      5       6     
// -------------------------------------------------
//  Width |   T    | Number |Ref.Z |  Z  |  Spacing 

	data.dWidth = _tstof(aValue[0]);
	data.dt = _tstof(aValue[1]);
	data.iBarNum = _ttoi(aValue[2]);
	data.iStaZRef = GetRefPosData(aValue[3]);
	data.dStaZ = _tstof(aValue[4]);
	data.dPitch = _tstof(aValue[5]);

	return TRUE;
}


BEGIN_MESSAGE_MAP(CDgnRebarCpgStiffGrid, CTBBrowserWnd)
	//{{AFX_MSG_MAP(CDgnRebarCpgStiffGrid)
	//ON_WM_LBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CDgnRebarCpgStiffGrid message handlers

/*
// 마우스 클릭시 번호 컬럼이 헤더 컬럼처럼 동작하게 한다.
void CDgnRebarCpgStiffGrid::OnLButtonDown(UINT nFlags, CPoint point)
{
	ROWCOL nhRow, nhCol;
	int ht = HitTest(point, &nhRow, &nhCol);
	if (ht != GX_NOHIT && ht != GX_VERTLINE)
	{
		if (IsFrozenCol(nhCol)) // key column이면
		{
			// Row header의 rectangle을 얻는다.
			CRect r = CalcRectFromRowColEx(nhRow, 0, nhRow, 0);  
			point.x = (r.left+r.right)/2;
		}
	}

	CTBBrowserWnd::OnLButtonDown(nFlags, point);
}
*/

BOOL CDgnRebarCpgStiffGrid::OnInsertEmptyRecord(ROWCOL nRow)
{
	// 여기서는 Default 값을 채워 넣는 역할을 하면 된다.
	// 새 레코드가 입력되는 순간에 번호 자동 입력(DB에서 가져온다.)
	BOOL bActive;
	ROWCOL nCol, ncRow, ncCol;
	CString value;

	// 현재 셀이 Active되어 있으면 값을 채워 넣지 않는다.
	VERIFY(GetCurrentCell(ncRow, ncCol));
	CGXControl* pControl = (CGXControl*)GetControl(ncRow, ncCol);
	if (pControl && pControl->IsActive()) bActive = TRUE;
	else bActive = FALSE;
	if (pControl->IsKindOf(CONTROL_CLASS(CGXCheckBox))) bActive = TRUE;
	else if (pControl->IsKindOf(CONTROL_CLASS(CGXCheckListComboBox))) bActive = TRUE;

	for (int i = 0; i < m_nColCount; i++)
	{
		nCol = i+1;
		//VERIFY(GetColumnNoFor(i+1, nCol));
		value = GetDefValue(i);
		if (!value.IsEmpty() && (nCol != ncCol || !bActive)) 
			SetExpressionRowCol(nRow, nCol, value);
	}

	return TRUE;
}

CString CDgnRebarCpgStiffGrid::GetDefValue(int ix)
{
	if (ix == 0) return _T("0");
	else if (ix == 1) return _T("0");
	else if (ix == 2) return _T("0");
	else if (ix == 3) return D_RPSC_REF_TOP;
	else if (ix == 4) return _T("0");
	else if (ix == 5) return _T("0");  
	else ASSERT(0);

	return _T("");
}


////////////////////////////////////////////////////

BOOL CDgnRebarCpgStiffGrid::HideColsByIntend(int nFromCol, int nToCol, BOOL bHide)
{
	ASSERT(nFromCol >= 0 && nToCol <= GetColCount());

	if (!CGXGridWnd::HideCols(nFromCol, nToCol, bHide)) return FALSE;

	CString csHide;
	if (bHide) csHide = _T("Yes");
	else csHide = _T("");

	for (int i = nFromCol; i <= nToCol; i++)
	{
		SetStyleRange(CGXRange(0, i), CGXStyle()
			.SetUserAttribute(ID_USER_ATTR_HIDDEN, csHide));
		SetStyleRange(CGXRange().SetCols(i), CGXStyle().SetEnabled(!bHide));
	}

	return TRUE;
}

BOOL CDgnRebarCpgStiffGrid::IsHiddenByIntend(int nCol)
{
	CGXStyle style;
	CString csHide;
	GetStyleRowCol(0, nCol, style);
	style.GetUserAttribute(ID_USER_ATTR_HIDDEN, csHide);
	if (csHide == _T("Yes")) return TRUE;
	else return FALSE;
}

// HideColsByIntend()함수를 도와주는 함수 
int CDgnRebarCpgStiffGrid::HitTest(CPoint& pt, ROWCOL* pnRow, ROWCOL* pnCol, CRect* rectHit)
{
	int nHitResult = CGXGridWnd::HitTest(pt, pnRow, pnCol, rectHit);

	int nRow, nCol;
	nRow = (pnRow != NULL) ? *pnRow : -1;
	nCol = (pnCol != NULL) ? *pnCol : -1;

	if (nCol == -1) return nHitResult;

	if (nHitResult == GX_VERTLINE)
	{
		if (IsHiddenByIntend(nCol))
		{
			if (*pnCol == 0) nHitResult = GX_NOHIT;
			//else *pnCol -= 1; // 이 항목 대신 OnTrackColWidth() 함수를 Override해서 해결
		}
	}
	else if (nHitResult == GX_HDNCOL_VERTLINE)
	{
		if (IsHiddenByIntend(nCol))
		{
			*pnCol += 1;
			nHitResult = GX_HEADERHIT;
		}
	}
	return nHitResult;
}

// HideColsByIntend()함수를 도와주는 함수 
BOOL CDgnRebarCpgStiffGrid::OnTrackColWidth(ROWCOL nCol)
{
	if (nCol == 0) return FALSE;
	if (IsHiddenByIntend(nCol)) return FALSE;
	return CGXGridWnd::OnTrackColWidth(nCol);
}

// HideColsByIntend()함수를 도와주는 함수
BOOL CDgnRebarCpgStiffGrid::HideCols(ROWCOL nFromCol, ROWCOL nToCol, BOOL bHide, BOOL *abHideArray, UINT flags, GXCmdType ctCmd)
{
	BOOL bAllocated = FALSE;
	if (!bHide) // Show일 경우
	{
		if (abHideArray == 0) // flag 배열이 NULL이면 배열을 만든다.
		{
			bAllocated = TRUE;  // 새로 배열을 위한 메모리가 allocate되었음을 표시
			abHideArray = new BOOL[nToCol-nFromCol+1];
			for (int i = nFromCol; i <= nToCol; i++)
				abHideArray[i-nFromCol] = FALSE;
		}
		for (int i = nFromCol; i <= nToCol; i++)
			if (IsHiddenByIntend(i))
				abHideArray[i-nFromCol] = TRUE;
	}
	BOOL bResult = CGXGridWnd::HideCols(nFromCol, nToCol, bHide, abHideArray, flags, ctCmd);
	if (bAllocated) delete[] abHideArray;
	return bResult;
}

// HideColsByIntend로 인해 숨겨진 칼럼은 복사, Paste에서 빼야 한다.
BOOL CDgnRebarCpgStiffGrid::Copy()
{
	// 원래 Range를 저장하고 의도적으로 숨겨진 Column은 뺀다.
	CGXRangeList OrgRange;
	CGXRangeList* pRangeList = GetParam()->GetRangeList();
	OrgRange = *pRangeList;
	for (int i = 1; i <= GetColCount(); i++)
	{
		if (IsHiddenByIntend(i)) 
			SelectRange(CGXRange().SetCols(i), FALSE, FALSE);
	}
	// 복사하고
	BOOL bResult = CTBBrowserWnd::Copy();
	// 원래 RangeList를 복구한다.
	*pRangeList = OrgRange;
	return bResult;
}



