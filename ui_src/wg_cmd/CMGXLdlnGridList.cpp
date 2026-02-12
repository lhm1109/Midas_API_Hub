// CMGXLdlnGridList.cpp : implementation file
//
// 2004. 02. 13   by TAE
// CListCtrl의 기능을 할 수 있는 Grid 
// 두줄의 헤더를 갖는다.
//
// NAME |  FIRST POINT(m) |
//      |  X  |  Y  |  Z  |
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMGXLdlnGridList.h"

#include "..\wg_base\wg_base_CompFunc.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_ViewBuff.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_QSort.h"

#include "CMLoadCombMainGrid.h"
#include "CMLoadLineListDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCMGXLdlnGridList

CCMGXLdlnGridList::CCMGXLdlnGridList()
{
	m_pDoc = 0;

	m_nColCount  = 6;   // key, serial, name, x, y, z
	m_nColHeader = 1;   // extra column header(+default header 1)

	/*
	m_aColInfo.RemoveAll();
	m_aColInfo.Add(SListColItem(_T("no"),     0, GX_VT_NUMERIC, 0)); // key
	m_aColInfo.Add(SListColItem(_T("Serial"), 0, GX_VT_NUMERIC, 0)); // serial 
	*/
}

CCMGXLdlnGridList::~CCMGXLdlnGridList()
{
}

/*
void CCMGXLdlnGridList::AddColInfo(LPCTSTR name, int width, UINT type, UINT place)
{
	m_aColInfo.Add(SListColItem(name, width, type, place)); 
	m_nUserDefCount++;
}
*/

int  CCMGXLdlnGridList::GetSelectedKeyList(CArray<UINT, UINT> &aKey)
{
	aKey.RemoveAll();
	
	CRowColArray awRows;
	GetSelectedRows(awRows, TRUE, FALSE);
	int nSize = awRows.GetSize();

	if (nSize > 0)
	{
		// Append Row는 뺀다.
		if (IsAppendRow(awRows.GetAt(nSize-1))) { nSize--; awRows.RemoveAt(nSize); }
		
		// Title Row도 뺀다.
		for (int i=m_nColHeader; i>=0; i--)
			if (nSize > 0 && awRows.GetAt(i) <= m_nColHeader) { nSize--; awRows.RemoveAt(i); }
		if (nSize == 0) return TRUE;

		aKey.SetSize(nSize);
		for (int i = 0; i < nSize; i++)
			aKey[i] = _ttol(GetValueRowCol(awRows[i], 1));
	}
	else  // 이 경우는 현재 레코드
	{
		ROWCOL nRow, nCol;
		GetCurrentCell(nRow, nCol);
		if (IsAppendRow(nRow)) return 0;
		aKey.SetSize(1);
		aKey[0] = _ttol(GetValueRowCol(nRow, 1));
	}

	return aKey.GetSize();
}

/*
int CCMGXLdlnGridList::GetSelectedNameList(CArray<CString, CString&> &raName)
{
	raName.RemoveAll();
	
	CRowColArray awRows;
	GetSelectedRows(awRows, TRUE, FALSE);
	int nSize = awRows.GetSize();

	if (nSize > 0)
	{
		// Append Row는 뺀다.
		if (IsAppendRow(awRows.GetAt(nSize-1))) { nSize--; awRows.RemoveAt(nSize); }
		// Title Row도 뺀다.
		if (nSize > 0 && awRows.GetAt(0) == 0) { nSize--; awRows.RemoveAt(0); }
		if (nSize == 0) return TRUE;

		raName.SetSize(nSize);
		for (int i = 0; i < nSize; i++)
			raName[i] = GetValueRowCol(awRows[i], 3);
	}
	else  // 이 경우는 현재 레코드
	{
		ROWCOL nRow, nCol;
		GetCurrentCell(nRow, nCol);
		if (IsAppendRow(nRow)) return 0;
		raName.SetSize(1);
		raName[0] = GetValueRowCol(nRow, 3);
	}

	return raName.GetSize();
}
*/

// insert는 serial 번호로 
BOOL CCMGXLdlnGridList::InsertRecordByKey(UINT key, UINT serial, CStringArray &aValue)
{
	CWaitCursor wait;
	SetModeDisplayOnly(FALSE);
	BOOL bOldLock = LockUpdate(TRUE);

	ROWCOL nRowCount = GetRowCount() - 1;  // without append row
	ROWCOL i = 0;
	for (i = 0; i < nRowCount; i++)
		if (_ttol(GetValueRowCol(i+1, 2)) > serial) break;

	InsertRecord(i+1);
	SetValue(i+1, key, serial, aValue);

	LockUpdate(bOldLock);
	if (!bOldLock) Redraw();
	SetModeDisplayOnly(TRUE);

	return TRUE;
}

// delete는 key로 
BOOL CCMGXLdlnGridList::DeleteRecordByKey(UINT key)
{
	CWaitCursor wait;
	SetModeDisplayOnly(FALSE);
	BOOL bOldLock = LockUpdate(TRUE);

	ROWCOL ncRow, ncCol;
	BOOL bCurCell = GetCurrentCell(ncRow, ncCol);
	ROWCOL nRowCount = GetRowCount() - 1;  // without append row
	for (ROWCOL i = 0; i < nRowCount; i++)
	{
		if (_ttol(GetValueRowCol(i+1, 1)) == key)
		{
			if (bCurCell && i+1 == ncRow && m_nEditMode == edit) CancelEdit();
			DeleteRecord(i+1); 
			break;
		}
	}

	LockUpdate(bOldLock);
	if (!bOldLock) Redraw();
	SetModeDisplayOnly(TRUE);

	return TRUE;
}

//--------------------------------------------------------------------------
// Interface
void CCMGXLdlnGridList::Initialize(CDBDoc* pDoc)
{
	m_pDoc = pDoc;
	CTBBrowserWnd::Initialize();
	SetColInfo();

	GetParam()->EnableMoveCols(FALSE);  // Disable moving column
	GetParam()->EnableMoveRows(TRUE);   // Disable moving row
	GetParam()->EnableTrackRowHeight(FALSE);  // Disable resizing row height
	//GetParam()->EnableSelection(FALSE);

	SetScrollBarMode(SB_VERT, gxnEnabled | gxnEnhanced);

	EnableInsertRowByPressingKey(FALSE);  // Disable inserting row by key  
	MakeItemEx();
}

void CCMGXLdlnGridList::MakeItemEx()
{
	SetModeDisplayOnly(FALSE);

	CWaitCursor wait;

	BOOL bOldLock = LockUpdate(TRUE);

	CArray<int, int>aSeq;
	CArray<T_LDLN_K, T_LDLN_K> aKey;
	m_pDoc->m_pAttrCtrl->GetLdlnKeyList(aKey);
	int nRecCount = aKey.GetSize();
	SetRowCount(m_nColHeader);
	SetRecordCount(m_nColHeader+nRecCount);

	T_LDLN_K key;
	T_LDLN_D data;

	//Sorting
	for (int i=0; i<nRecCount; i++)
	{
		key = aKey[i];
		m_pDoc->m_pAttrCtrl->GetLdln(key, data);
		aSeq.Add(data.nSeq);
	}
	if (aKey.GetSize() > 0)
		CQSort::QSortIntWithIndex((int*)aKey.GetData(), aSeq.GetData(), aKey.GetSize());

	CStringArray aValue;
	for (int i=0; i<nRecCount; i++)
	{
		key = aKey[i];
		m_pDoc->m_pAttrCtrl->GetLdln(key, data);
		ConvertDataToStr(data, aValue);
		SetValue((ROWCOL)(i+1+m_nColHeader), aKey[i], data.nSeq, aValue);
	}

	SetRightValidCell(1+m_nColHeader, 1);

	LockUpdate(bOldLock);
	if (!bOldLock) Redraw();
	SetModeDisplayOnly(TRUE);
}

void CCMGXLdlnGridList::ConvertDataToStr(T_LDLN_D &data, CStringArray &aValue)
{
	ASSERT(data.aData.GetSize() > 0);

	aValue.SetSize(4);
	aValue[0] = data.Name;
	aValue[1].Format(_T("%.15g"), data.aData[0].x);
	aValue[2].Format(_T("%.15g"), data.aData[0].y);
	aValue[3].Format(_T("%.15g"), data.aData[0].z);
}

//--------------------------------------------------------------------------
// Implementation
void CCMGXLdlnGridList::SetColInfo()
{
	BOOL bOldLock = LockUpdate(TRUE);

	// Set Title Header
	SetHeaderTitle();

	// 표준 스타일 변경
	ChangeStandardStyle(CGXStyle()
			.SetFont(CGXFont_GC().SetSize(10))
			//.SetControl(GX_IDS_CTRL_STATIC)
			.SetVerticalAlignment(DT_BOTTOM)
			.SetAllowEnter(FALSE));

	// 컬럼 스타일 변경

	// key, serial
	SetStyleRange(CGXRange().SetCols(1, 2), CGXStyle()
		.SetHorizontalAlignment(DT_RIGHT)
		.SetValueType(GX_VT_NUMERIC)
		.SetFormat(GX_FMT_FIXED)
		.SetPlaces(0)
		.SetEnabled(FALSE));
	//HideCols(0, 0, TRUE);
	HideColsByIntend(1, 1, TRUE); //HideCols(1, 1, TRUE);
	HideColsByIntend(2, 2, TRUE); //HideCols(2, 2, TRUE);

	// name
	SetStyleRange(CGXRange().SetCols(1, 2), CGXStyle()
		.SetHorizontalAlignment(DT_LEFT)
		.SetValueType(GX_VT_STRING)
		.SetValue(_T("")));

	// x, y, z
	SetStyleRange(CGXRange().SetCols(1, 2), CGXStyle()
		.SetHorizontalAlignment(DT_RIGHT)
		.SetValueType(GX_VT_NUMERIC)
		.SetFormat(GX_FMT_FIXED)
		.SetPlaces(2));

	// 현재 셀의 border 설정 변경
	CGXProperties* pProp = GetParam()->GetProperties();
			pProp->SetUserProperty(GX_IDS_OUTLINECURRENTCELL,
		      (CGXStyle) pProp->sInvertNoBorder);

	// 현재 셀 설정
	SetRightValidCell(1, 1);
	GetParam()->SetHideCurrentCell(GX_HIDE_NEVER);

	SetColWidthByDPI(0, 0, 20);
	SetColWidthByDPI(1, 1, 0);
	SetColWidthByDPI(2, 2, 0);
	SetColWidthByDPI(3, 3, 55);
	SetColWidthByDPI(4, 4, 42);
	SetColWidthByDPI(5, 5, 42);
	SetColWidthByDPI(6, 6, 42);

	LockUpdate(bOldLock);
	if (!bOldLock) Redraw();
}

void CCMGXLdlnGridList::SetValue(ROWCOL nRow, UINT key, UINT serial, CStringArray &aValue)
{
	CString value;
	value.Format(_T("%ld"), (long)serial);
	aValue.InsertAt(0, value);    
	value.Format(_T("%ld"), (long)key);
	aValue.InsertAt(0, value);
	int nSize = aValue.GetSize();
	for (int i = 0; i < nSize; i++)
	{
		SetExpressionRowCol(nRow, i+1, aValue[i]);
	}
		
	// key, serial 설정
	SetStyleRange(CGXRange(nRow, 0), 
								CGXStyle().SetUserAttribute(ID_USER_ATTR_KEY, (long)key));
}

void CCMGXLdlnGridList::UpdateUnit()
{
	double dCoord;
	int nUnitTypeOffset = D_UNITSYS_BASE_LENGTH;

	//SetHeaderTitle();
	SetModeDisplayOnly(FALSE);
	CString csTitle, sRgUnit;
	CUnitCtrl::GetUnitSystem(CUnitCtrl::m_LDLN_UNIT.xyz,sRgUnit);
	csTitle.Format(_LS(IDS_CMD_LDLN_FIRST), sRgUnit);
	SetStyleRange(CGXRange(0, (ROWCOL)4), CGXStyle()
				.SetValue(csTitle));
	
	BOOL bOldLock = LockUpdate(TRUE);
	int nRow = GetRowCount();
	for (int i=2; i<nRow; i++)
	{
		for (int j=0; j<3; j++)
		{
			dCoord = _tstof(GetValueRowCol(i, j+4));
			dCoord = m_pDoc->m_pUnitCtrl->ConvertUnitDataPrevious(nUnitTypeOffset, dCoord);
			SetValueRange(CGXRange(i, j+4), dCoord);
		}
	}
	LockUpdate(bOldLock);
	if (!bOldLock) Redraw();
	SetModeDisplayOnly(TRUE);
}

void CCMGXLdlnGridList::SetHeaderTitle()
{
	BOOL bOldLock = LockUpdate(TRUE);
	
	SetColCount(m_nColCount);
	SetRowCount(m_nColHeader);
	if (m_nColHeader > 0)
	{
		// use 1 extra columns as headers ( + standard header at row 0)
		SetFrozenRows(1, m_nColHeader);
		// Do not draw column headers pressed when moving the current cell
		GetParam( )->GetProperties( )->SetMarkColHeader(FALSE);
	}

	CString aTitle[2][6] = {
		{ _T("Key"), _T("Serial"), _LS(IDS_CMD_LDLN_NAME), _LS(IDS_CMD_LDLN_FIRST_POINT), _T(""), _T("") },
		{ _T(""), _T(""), _T(""), _T("X") ,_T("Y"), _T("Z")}
	};

	SetCoveredCellsRowCol(0, 0, m_nColHeader, 0);  // header
	SetCoveredCellsRowCol(0, 1, m_nColHeader, 1);  // key
	SetCoveredCellsRowCol(0, 2, m_nColHeader, 2);  // serial
	SetCoveredCellsRowCol(0, 3, m_nColHeader, 3);  // name
	SetCoveredCellsRowCol(0, 4, 0, 6);             // first point

	// 단위 : load, main load
	CString csTitle, sRgUnit;
	CUnitCtrl::GetUnitSystem(CUnitCtrl::m_LDLN_UNIT.xyz,sRgUnit);

	for (int nRow = 0; nRow <= m_nColHeader; nRow++)
	{
		for (int nCol = 1; nCol <= m_nColCount; nCol++)
		{
			csTitle = aTitle[nRow][nCol-1];
			if (nRow == 0 && nCol == 4) csTitle = csTitle + _T("(") + sRgUnit + _T(")");      
			SetStyleRange(CGXRange(nRow, (ROWCOL)nCol), CGXStyle()
				.SetValue(csTitle));
		}
	}
	LockUpdate(bOldLock);
	if (!bOldLock) Redraw();
}

//--------------------------------------------------------------------------
// Overriadable
// Return은 항상 SetFieldValidation으로 한다.
// CTBBrowserWnd::ProcessKeys 참조
BOOL CCMGXLdlnGridList::OnValidateField(ROWCOL nRow, ROWCOL nCol)
{
	return TRUE;
}

// Return은 항상 SetRecordValidation으로 한다.
// CTBBrowserWnd::ProcessKeys 참조
BOOL CCMGXLdlnGridList::OnValidateRecord(ROWCOL nRow)
{
	return TRUE;
}

BOOL CCMGXLdlnGridList::ValidateField(CString& value, int nColID)
{
	return TRUE;
}

BOOL CCMGXLdlnGridList::ValidateRecord(CStringArray &aValue, CRowColArray &aCols)
{
	return TRUE;
}

BOOL CCMGXLdlnGridList::OnAddNewRecord(ROWCOL nRow)
{
	ASSERT(0);
	return TRUE;
}

BOOL CCMGXLdlnGridList::OnModifyRecord(ROWCOL nRow)
{
	ASSERT(0);
	return TRUE;
}

BOOL CCMGXLdlnGridList::OnDeleteRecord(ROWCOL nRow)
{
	ASSERT(0);
	return TRUE;
}


BOOL CCMGXLdlnGridList::OnDeleteRecords(CRowColArray &awRows)
{
	ASSERT(0);
	return TRUE;
}

BOOL CCMGXLdlnGridList::OnInsertEmptyRecord(ROWCOL nRow)
{
	ASSERT(0);
	return TRUE;
}

//--------------------------------------------------------------------------
// Grid's Standard Overridable Functions
// Record선택시 title row와 append row는 제거한다.
/*
void CCMGXLdlnGridList::OnChangedSelection(const CGXRange *changedRect, BOOL bIsDraggin, BOOL bKey)
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
				if (awRows[i] == GetRowCount() || awRows[i] == 0)
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
*/

// Row의 위치를 이동(Serial No가 변경)한다.
BOOL CCMGXLdlnGridList::OnSelDragRowsDrop(ROWCOL nFirstRow, ROWCOL nLastRow, ROWCOL nDestRow)
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
	nStart = (int)nDestRow-m_nColHeader;
	if (nDestRow < nFirstRow)  // 선택 영역 앞으로 이동
	{
		nInc = 1;
	}
	else  // 선택 영역 뒤로 이동
	{
		nStart--;
		nInc = 0;
	}

	CString csName;
	CArray<CString, CString&> aName;
	CArray<UINT, UINT> aSerial;
	for (int i = nFirstRow; i <= nLastRow; i++)
	{
		csName = GetValueRowColBeforeEdit(i, 3);
		aName.Add(csName);
		aSerial.Add(nStart);
		nStart += nInc;
	}

	ModifySerialID(aName, aSerial);
	
	return FALSE;
}

void CCMGXLdlnGridList::ModifySerialID(CArray<CString, CString&> &aName, CArray<UINT, UINT> &aSerial)
{
	T_LDLN_D data;
	CArray<T_LDLN_D, T_LDLN_D&> aData;
	for (int i=0; i<aName.GetSize(); i++)
	{
		if (!m_pDoc->m_pAttrCtrl->GetLdln(aName[i], data)) ASSERT(0);
		data.nSeq = aSerial[i];
		aData.Add(data);
	}
	m_pDoc->m_pDataCtrl->ModifyLdln(aName, aData);
}


BEGIN_MESSAGE_MAP(CCMGXLdlnGridList, CTBBrowserWnd)
	//{{AFX_MSG_MAP(CCMGXLdlnGridList)
	ON_WM_LBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CCMGXLdlnGridList message handlers

BOOL CCMGXLdlnGridList::OnLButtonDblClkRowCol(ROWCOL nRow, ROWCOL nCol, UINT nFlags, CPoint pt)
{
	BOOL bReturn;
	bReturn = CTBBrowserWnd::OnLButtonDblClkRowCol(nRow, nCol, nFlags, pt);

	if (nRow<2)
		return bReturn;

	int nRowCount = GetRowCount();
	if (nRowCount == nRow)
		return bReturn;

	UINT Key = _ttol(GetValueRowCol(nRow, 1));

	if (Key < 0)
		return bReturn;

	((CCMLoadLineListDlg*)(GetParent()))->OnLButtonDblClkOnGrid(Key);
	return bReturn;
}

// 마우스 클릭시 번호 컬럼이 헤더 컬럼처럼 동작하게 한다.
void CCMGXLdlnGridList::OnLButtonDown(UINT nFlags, CPoint point)
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

BOOL CCMGXLdlnGridList::GetStyleRowCol(ROWCOL nRow, ROWCOL nCol, CGXStyle& style, GXModifyType mt, int nType)
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

////////////////////////////////////////////////////

BOOL CCMGXLdlnGridList::HideColsByIntend(int nFromCol, int nToCol, BOOL bHide)
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

BOOL CCMGXLdlnGridList::IsHiddenByIntend(int nCol)
{
	CGXStyle style;
	CString csHide;
	GetStyleRowCol(0, nCol, style);
	style.GetUserAttribute(ID_USER_ATTR_HIDDEN, csHide);
	if (csHide == _T("Yes")) return TRUE;
	else return FALSE;
}

// HideColsByIntend()함수를 도와주는 함수 
int CCMGXLdlnGridList::HitTest(CPoint& pt, ROWCOL* pnRow, ROWCOL* pnCol, CRect* rectHit)
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
BOOL CCMGXLdlnGridList::OnTrackColWidth(ROWCOL nCol)
{
	if (nCol == 0) return FALSE;
	if (IsHiddenByIntend(nCol)) return FALSE;
	return CGXGridWnd::OnTrackColWidth(nCol);
}

// HideColsByIntend()함수를 도와주는 함수
BOOL CCMGXLdlnGridList::HideCols(ROWCOL nFromCol, ROWCOL nToCol, BOOL bHide, BOOL *abHideArray, UINT flags, GXCmdType ctCmd)
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
BOOL CCMGXLdlnGridList::Copy()
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



