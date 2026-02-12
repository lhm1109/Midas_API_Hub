// CMGXGridList.cpp : implementation file
//
// 2004. 02. 13   by TAE
// CListCtrl의 기능을 할 수 있는 Grid 
// User는 Initialize() 함수를 호출하기 전 AddColInfo()함수로 필요한 컬럼 정보를 세팅하고,
// GetSelectedNameList()를 사용하여 현재 선택된 Name을 가져올 수 있다.
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMGXGridList.h"

#include "..\wg_base\wg_base_CompFunc.h"

#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_ViewBuff.h"

#include "CMLoadCombMainGrid.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCMGXGridList

CCMGXGridList::CCMGXGridList()
{
	m_pDoc = 0;

	m_nDefaultCount = 2;   // key, serial
	m_nUserDefCount = 0;

	m_aColInfo.RemoveAll();
	m_aColInfo.Add(SListColItem(_T("no"),     0, GX_VT_NUMERIC, 0)); // key
	m_aColInfo.Add(SListColItem(_T("Serial"), 0, GX_VT_NUMERIC, 0)); // serial 

}

CCMGXGridList::~CCMGXGridList()
{
}

void CCMGXGridList::AddColInfo(LPCTSTR name, int width, UINT type, UINT place)
{
	m_aColInfo.Add(SListColItem(name, width, type, place)); 
	m_nUserDefCount++;
}

int  CCMGXGridList::GetSelectedKeyList(CArray<UINT, UINT> &aKey)
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
		if (nSize > 0 && awRows.GetAt(0) == 0) { nSize--; awRows.RemoveAt(0); }
		if (nSize == 0) return TRUE;

		aKey.SetSize(nSize);
		for (int i = 0; i < nSize; i++)
			aKey[i] = _tstol(GetValueRowCol(awRows[i], 1));
	}
	else  // 이 경우는 현재 레코드
	{
		ROWCOL nRow, nCol;
		GetCurrentCell(nRow, nCol);
		if (IsAppendRow(nRow)) return 0;
		aKey.SetSize(1);
		aKey[0] = _tstol(GetValueRowCol(nRow, 1));
	}

	return aKey.GetSize();
}

/*
int CCMGXGridList::GetSelectedNameList(CArray<CString, CString&> &raName)
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
BOOL CCMGXGridList::InsertRecordByKey(UINT key, UINT serial, CStringArray &aValue)
{
	CWaitCursor wait;
	SetModeDisplayOnly(FALSE);
	BOOL bOldLock = LockUpdate(TRUE);

	ROWCOL nRowCount = GetRowCount() - 1;  // without append row
	ROWCOL i = 0;
	for (i = 0; i < nRowCount; i++)
		if (_tstol(GetValueRowCol(i+1, 2)) > serial) break;

	InsertRecord(i+1);
	SetValue(i+1, key, serial, aValue);

	LockUpdate(bOldLock);
	if (!bOldLock) Redraw();
	SetModeDisplayOnly(TRUE);

	return TRUE;
}

// delete는 key로 
BOOL CCMGXGridList::DeleteRecordByKey(UINT key)
{
	CWaitCursor wait;
	SetModeDisplayOnly(FALSE);
	BOOL bOldLock = LockUpdate(TRUE);

	ROWCOL ncRow, ncCol;
	BOOL bCurCell = GetCurrentCell(ncRow, ncCol);
	ROWCOL nRowCount = GetRowCount() - 1;  // without append row
	for (ROWCOL i = 0; i < nRowCount; i++)
	{
		if (_tstol(GetValueRowCol(i+1, 1)) == key)
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
void CCMGXGridList::Initialize(CDBDoc* pDoc)
{
	m_pDoc = pDoc;
	CTBBrowserWnd::Initialize();
	SetColInfo();

	GetParam()->EnableMoveCols(FALSE);  // Disable moving column
	GetParam()->EnableMoveRows(TRUE);   // Enable moving row
	GetParam()->EnableTrackRowHeight(FALSE);  // Disable resizing row height

	SetScrollBarMode(SB_VERT, gxnEnabled | gxnEnhanced);

	EnableInsertRowByPressingKey(FALSE);  // Disable inserting row by key  
	MakeItemEx();
}

void CCMGXGridList::MakeItemEx()
{
	CWaitCursor wait;
	BOOL bOldLock = LockUpdate(TRUE);

	SetModeDisplayOnly(FALSE);

	MakeItemList();

	SetRightValidCell(1, 1);
	SetModeDisplayOnly(TRUE);

	LockUpdate(bOldLock);
	if (!bOldLock) Redraw();
}

//--------------------------------------------------------------------------
// Implementation
void CCMGXGridList::SetColInfo()
{
	int nColNo = m_aColInfo.GetSize();

	BOOL bOldLock = LockUpdate(TRUE);

	// Set Title Header
	SetHeaderTitle();

	// 표준 스타일 변경
	ChangeStandardStyle(CGXStyle()
			.SetFont(CGXFont_GC().SetSize(10))
			//.SetControl(GX_IDS_CTRL_STATIC)
			.SetVerticalAlignment(DT_BOTTOM)
			.SetAllowEnter(FALSE));

	UINT  type;
	DWORD align;
	for (int nCol=1; nCol<=nColNo; nCol++)
	{
		type = m_aColInfo[nCol-1].nType;
		align = (type==GX_VT_STRING) ? DT_LEFT : DT_RIGHT;
		SetStyleRange(CGXRange().SetCols(nCol), CGXStyle()
			.SetHorizontalAlignment(align)
			.SetValueType(type));

		if (type == GX_VT_NUMERIC)
		{
			SetStyleRange(CGXRange().SetCols(nCol), CGXStyle()
				.SetFormat(GX_FMT_FIXED)
				.SetPlaces(m_aColInfo[nCol-1].nPlace));
		}
		if (nCol <= m_nDefaultCount) 
		{
			SetStyleRange(CGXRange().SetCols(nCol), CGXStyle().SetEnabled(FALSE));
			HideCols(nCol, nCol, TRUE);
		}
	}

	// 현재 셀의 border 설정 변경
	CGXProperties* pProp = GetParam()->GetProperties();
			pProp->SetUserProperty(GX_IDS_OUTLINECURRENTCELL,
		      (CGXStyle) pProp->sInvertNoBorder);

	// 현재 셀 설정
	SetRightValidCell(1, 1);
	GetParam()->SetHideCurrentCell(GX_HIDE_NEVER);

	for (int nCol=1; nCol<=m_nDefaultCount; nCol++)
		SetColWidthByDPI(nCol, nCol, 0);
	for (int nCol=m_nDefaultCount+1; nCol<=m_nDefaultCount+m_nUserDefCount; nCol++)
		SetColWidthByDPI(nCol, nCol, m_aColInfo[nCol-1].nWidth); 

	LockUpdate(bOldLock);
	if (!bOldLock) Redraw();
}

void CCMGXGridList::SetValue(ROWCOL nRow, UINT key, UINT serial, CStringArray &aValue)
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

/*
CString CCMGXGridList::GetKeyString(ROWCOL nRow)
{
	CString csKey;
	CGXStyle style;
	GetStyleRowCol(nRow, 0, style);
	style.GetUserAttribute(ID_USER_ATTR_KEY, csKey);
	return csKey;
}
*/

void CCMGXGridList::SetHeaderTitle()
{
	int nColCount = m_aColInfo.GetSize();

	BOOL bOldLock = LockUpdate(TRUE);
	SetColCount(nColCount);
	
	for (int i = 0; i < nColCount; i++)
	{
		SetStyleRange(CGXRange(0, (ROWCOL)(i+1)), CGXStyle()
			.SetValue(m_aColInfo[i].csName));
	}

	LockUpdate(bOldLock);
	if (!bOldLock) Redraw();
}

//--------------------------------------------------------------------------
// Overriadable
// Return은 항상 SetFieldValidation으로 한다.
// CTBBrowserWnd::ProcessKeys 참조
BOOL CCMGXGridList::OnValidateField(ROWCOL nRow, ROWCOL nCol)
{
	return TRUE;
}

// Return은 항상 SetRecordValidation으로 한다.
// CTBBrowserWnd::ProcessKeys 참조
BOOL CCMGXGridList::OnValidateRecord(ROWCOL nRow)
{
	return TRUE;
}

BOOL CCMGXGridList::ValidateField(CString& value, int nColID)
{
	return TRUE;
}

BOOL CCMGXGridList::ValidateRecord(CStringArray &aValue, CRowColArray &aCols)
{
	return TRUE;
}

BOOL CCMGXGridList::OnAddNewRecord(ROWCOL nRow)
{
	ASSERT(0);
	return TRUE;
}

BOOL CCMGXGridList::OnModifyRecord(ROWCOL nRow)
{
	ASSERT(0);
	return TRUE;
}

BOOL CCMGXGridList::OnDeleteRecord(ROWCOL nRow)
{
	ASSERT(0);
	return TRUE;
}


BOOL CCMGXGridList::OnDeleteRecords(CRowColArray &awRows)
{
	ASSERT(0);
	return TRUE;
}

BOOL CCMGXGridList::OnInsertEmptyRecord(ROWCOL nRow)
{
	ASSERT(0);
	return TRUE;
}

//--------------------------------------------------------------------------
// Grid's Standard Overridable Functions
// Record선택시 title row와 append row는 제거한다.
/*
void CCMGXGridList::OnChangedSelection(const CGXRange *changedRect, BOOL bIsDraggin, BOOL bKey)
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
BOOL CCMGXGridList::OnSelDragRowsDrop(ROWCOL nFirstRow, ROWCOL nLastRow, ROWCOL nDestRow)
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


BEGIN_MESSAGE_MAP(CCMGXGridList, CTBBrowserWnd)
	//{{AFX_MSG_MAP(CCMGXGridList)
	ON_WM_LBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CCMGXGridList message handlers

// 마우스 클릭시 번호 컬럼이 헤더 컬럼처럼 동작하게 한다.
void CCMGXGridList::OnLButtonDown(UINT nFlags, CPoint point)
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


/*
BOOL CCMGXGridList::OnLButtonDblClkRowCol(ROWCOL nRow, ROWCOL nCol, UINT nFlags, CPoint pt)
{
	CString csKey = GetValueRwoCol(nRow, 1);
	UINT key = _tstol(csKey);
	DoLButtonDblClick(nRow, nCol, key);

	return CTBBrowserWnd::OnLButtonDblClkRowCol(nRow, nCol, nFlags, pt);
}
*/

