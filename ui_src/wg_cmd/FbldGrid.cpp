// FbldGrid.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "FbldGrid.h"
#include "CMFbldDlg.h"

#include "..\wg_base\wg_base_CompFunc.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_UnitCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define FBLD_KEY_FMT _T("%d")
/////////////////////////////////////////////////////////////////////////////
// CFbldGrid
CString CFbldGrid::m_aCurDefVal[] = {_T(""), _T(""), _T("")};

CFbldGrid::CFbldGrid()
{
	m_pParent = 0;
	m_nCurrentRow = 0;

	AddColInfo(_LS(IDS_WG_CMD__ADDD__No),             t_integer, 0);
	AddColInfo(_LS(IDS_WG_CMD__ADDD__Sno),            t_integer, 0);
	AddColInfo(_LS(IDS_WG_CMD__ADDD__Name),           t_string,  0);
	AddColInfo(_LS(IDS_WG_CMD__ADDD__Description),    t_string,  0);

	m_aPrimaryKey.Add(0);
	m_aPrimaryKey.Add(1);
}

CFbldGrid::~CFbldGrid()
{
}

#define CWnd CTBCommon
BEGIN_MESSAGE_MAP(CFbldGrid, CWnd)
#undef CWnd
	//{{AFX_MSG_MAP(CFbldGrid)
	ON_WM_RBUTTONDBLCLK()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CFbldGrid message handlers
void CFbldGrid::Initialize(CDBDoc* pDoc, CCMFbldDlg* pParent)
{
	m_pParent = pParent;

	T_UNIT_SYSTEM UnitSystem; // not used, but compatibility
	CTBCommon::Initialize(pDoc, UnitSystem);

	LockUpdate(TRUE);

	// 표준 스타일 변경
	ChangeStandardStyle(CGXStyle()
			.SetFont(CGXFont_GC().SetSize(9))
			.SetVerticalAlignment(DT_BOTTOM)
			.SetAllowEnter(FALSE));

	// 컬럼 헤더 스타일 변경
	ChangeColHeaderStyle(CGXStyle()
			.SetFont(CGXFont_GC().SetBold(FALSE)));

	// 컬럼 스타일 변경
	SetStyleRange(CGXRange().SetCols(1, 4), CGXStyle()
			.SetControl(GX_IDS_CTRL_STATIC));
	SetStyleRange(CGXRange().SetCols(1, 2), CGXStyle()
			.SetHorizontalAlignment(DT_LEFT)
			.SetValueType(GX_VT_NUMERIC)
			.SetEnabled(FALSE)
			.SetPlaces(0));
	SetStyleRange(CGXRange().SetCols(3, 4), CGXStyle()
			.SetHorizontalAlignment(DT_LEFT)
			.SetValueType(GX_VT_STRING));

	// Key Field도 숨긴다.
	HideColsByIntend(1, 1, TRUE);
	// FloorLoadTypeID는 숨긴다.(사용자가 알 필요 없는 칼럼이다.)
	HideColsByIntend(2, 2, TRUE); 
	SetRightValidCell(1, 2);

	// Sorting 정보 초기화
	m_aSortInfo.SetSize(2);
	m_aSortInfo[0].nRC = 1; // Sno
	m_aSortInfo[0].sortType = CGXSortInfo::numeric;
	m_aSortInfo[0].sortOrder = CGXSortInfo::ascending;
	m_aSortInfo[1].nRC = 0; // Key
	m_aSortInfo[1].sortType = CGXSortInfo::numeric;
	m_aSortInfo[1].sortOrder = CGXSortInfo::ascending;

	// always show vertical scrollbar
	SetScrollBarMode(SB_VERT, gxnEnabled | gxnEnhanced);  

	// Row 이동을 가능하게 한다.
	GetParam()->EnableMoveRows(TRUE);

	// Record를 읽어서 채운다.
	MakeItemEx();

	// Display Only Mode로 설정한다. Insert, Delete Key Disable의 효과
	SetModeDisplayOnly(TRUE);

	LockUpdate(FALSE);
	Redraw();
}

BOOL CFbldGrid::InsertRecordByKey(const T_FBLD_K &key, const T_FBLD_D &data)
{
	// 삽입 위치를 찾는다.
	ROWCOL nRow;

	// aKey를 만드는 코드 삽입
	CStringArray aKey;
	MakeSearchKey(key, data, aKey);
	FindRecordByKey(aKey, nRow);

	VERIFY(InsertRecord(nRow));
	return SetValue(nRow, key, data);
}

BOOL CFbldGrid::DeleteRecordByKey(const T_FBLD_K &key, const T_FBLD_D &data)
{
	// 레코드를 찾는다.
	ROWCOL nRow;

	// aKey를 만드는 코드 삽입
	CStringArray aKey;
	MakeSearchKey(key, data, aKey);
	if (!FindRecordByKey(aKey, nRow)) return FALSE;

	// 삭제한다.
	return DeleteRecord(nRow);
}

/////////////////////////////////////////////////////////////////////////////
// CFbldGrid Overridable Functions
void CFbldGrid::OnChangedSelectionPublic(long *keys, int nSize)
{
	return;
}

long CFbldGrid::GetSelectedKeys(CArray<long, long> &caKey)
{
	return 0;
}

BOOL CFbldGrid::ValidateField(CString value, int nColID)
{
	return TRUE;
}

BOOL CFbldGrid::ValidateRecord(CStringArray &value, CRowColArray &aCols)
{
	return TRUE;
}

BOOL CFbldGrid::ValidateAndMakeRecord(CStringArray &value, CRowColArray& aCols, T_FBLD_K &key, T_FBLD_D &data)
{
	return TRUE;
}

BOOL CFbldGrid::AddToDB(ROWCOL nRow)
{
	return TRUE;
}

BOOL CFbldGrid::AddToDB(CArray<void*, void*>& raKey, CArray<void*, void*>& raData)
{
	return TRUE;
}


BOOL CFbldGrid::DeleteFromDB(ROWCOL nRow)
{
	return TRUE;
}

BOOL CFbldGrid::DeleteFromDB(CRowColArray& awRows)
{
	return TRUE;
}

BOOL CFbldGrid::ModifyToDB(ROWCOL nRow)
{
	return TRUE;
}

BOOL CFbldGrid::ModifyToDB(CStringArray& raOldKey, 
												 CArray<void*, void*>& raKey, 
												 CArray<void*, void*>& raData)
{
	return TRUE;
}


CString CFbldGrid::GetNewKey()
{
	return CString(_T(""));
}

CString CFbldGrid::GetDefValue(int nIndex)
{
	return m_aCurDefVal[nIndex];
}

CString* CFbldGrid::GetDefValueArray()
{
	return m_aCurDefVal;
}

BOOL CFbldGrid::MakeKeyAndDataArray(CStringArray& value, CRowColArray& awCols, 
																	CArray<void*, void*>& aKey, CArray<void*, void*>& aData)
{
	ASSERT(0);
	return TRUE;
}

void CFbldGrid::DeleteKeyAndDataArray(CArray<void*, void*>& aKey, CArray<void*, void*>& aData)
{
	ASSERT(0);
}

long CFbldGrid::GetCountData()
{
	CDBDoc* pDoc = m_pDoc;
	return pDoc->m_pAttrCtrl->GetCountFbld();
}

void CFbldGrid::MakeItemFiltered(long nFilteredNum, long* aFilteredKeys, CRowColArray& awCols)
{
	ASSERT(0);
}

void CFbldGrid::MakeItemNormal(long nNumData, CRowColArray& awCols)
{
	T_FBLD_K Key;
	T_FBLD_D Data;
	CDBDoc* pDoc = m_pDoc;

	long* KeyBuf = new long[nNumData];
	long nCount = 0;
	POSITION pos = pDoc->m_pAttrCtrl->GetStartFbld();
	while(pos != NULL)
	{
	  pDoc->m_pAttrCtrl->GetNextFbld(pos,Key,Data);
	  KeyBuf[nCount++] = Key;
	}
	qsort(KeyBuf, nNumData, sizeof(T_FBLD_K), CCompFunc::UINTAsc);

	for(nCount=0 ; nCount < nNumData; nCount++)
	{
		pDoc->m_pAttrCtrl->GetFbld(KeyBuf[nCount], Data);
		SetValue(GetParam()->GetData(), nCount+1, KeyBuf[nCount], Data, awCols);
	} // end of loop nCount

	delete []KeyBuf;
}

void CFbldGrid::MakeItemEditingRecord(ROWCOL ncRow, CStringArray& EditingRecord)
{
	ASSERT(0);
}

/////////////////////////////////////////////////////////////////////////////
// CFbldGrid Implementation
BOOL CFbldGrid::GetValue(ROWCOL nRow, T_FBLD_K &key, T_FBLD_D &data)
{
	ASSERT(0);
	return TRUE;
}

BOOL CFbldGrid::SetValue(ROWCOL nRow, const T_FBLD_K &key, const T_FBLD_D &data)
{
	CRowColArray awCols;
	awCols.SetSize(GetColCount());
	for (int i = 0;i < GetColCount(); i++)
		VERIFY(GetColumnNoFor(m_aTitle[i], awCols[i]));
	return SetValue(GetParam()->GetData(), nRow, key, data, awCols);
}

BOOL CFbldGrid::SetValue(CGXData* pData, ROWCOL nRow, const T_FBLD_K &key, const T_FBLD_D &data, CRowColArray &awCols)
{
	CString str;
	for (int i = 0; i < GetColCount(); i++)
	{
		if(i==0) 
		{
			if (key == 0) str.Empty();
			else str.Format(FBLD_KEY_FMT, key);
		}
		else ConvDataToStr(i, data, str);
		pData->StoreValueRowCol(nRow, awCols[i], str, gxOverride);
	}

	return TRUE;
}

void CFbldGrid::ConvStrToData(int i, CString& value, T_FBLD_D& data)
{
	ASSERT(0);
}

void CFbldGrid::ConvDataToStr(int i, const T_FBLD_D& data, CString& value)
{
	if (i==1) value.Format(_T("%d"), data.FloorLoadTypeId);
	else if (i==2) value = data.FloorLoadTypeName;
	else if (i==3) value = data.Description;
}

void CFbldGrid::MakeSearchKey(const T_FBLD_K &key, const T_FBLD_D &data, CStringArray &aKey)
{
	int nColID;
	int nSortKeyNum = m_aSortInfo.GetSize();

	aKey.SetSize(nSortKeyNum);
	for (int i = 0; i < nSortKeyNum; i++)
	{
		nColID = m_aSortInfo[i].nRC;
		if (nColID == 0) aKey[i].Format(FBLD_KEY_FMT, key);
		else ConvDataToStr(nColID, data, aKey[i]);
	}
}

void CFbldGrid::GetAllSelectedFbld(CArray<T_FBLD_K, T_FBLD_K>& aSelKey)
{
	return;
}

void CFbldGrid::SetHeaderTitle(T_UNIT_SYSTEM &UnitSystem, BOOL bInit)
{
	// Set Title
	CString csTitle, csUnit;
	int aUnit[] = {0, 0, 0, 0};

	int aSize[] = {30, 30, 100, 185};
	int nColCount = GetColCount();

	for (int i = 0; i < nColCount; i++)
	{
		CUnitCtrl::GetUnitSystem(aUnit[i], csUnit);
		if (csUnit.IsEmpty()) csTitle = m_aTitle[i];
		else csTitle = m_aTitle[i] + _T("\n(")+csUnit+_T(")");
		SetStyleRange(CGXRange(0, i+1), CGXStyle()
			.SetValue(csTitle)
			.SetUserAttribute(ID_USER_ATTR_COL_ID, m_aTitle[i]));
		if (bInit) SetColWidthByDPI(i+1, i+1, aSize[i]);
	}
}

//------------------------------------------------------------------------------>
// Floor Load의 위치를 이동한다.
BOOL CFbldGrid::OnSelDragRowsDrop(ROWCOL nFirstRow, ROWCOL nLastRow, ROWCOL nDestRow)
{
	ASSERT(!IsEdit());

	// Append Row의 뒤면 맨 마지막으로 이동하는 것으로 한다.
	if (nDestRow == GetRowCount()+1) nDestRow--;  

	// 이동의 효과가 없는 곳으로 이동(선택범위내로 다시 이동)하면 그냥 리턴
	if (nDestRow >= nFirstRow && nDestRow <= nLastRow+1) return FALSE;

	// 연속 선택이 아니면 이동을 금지시킨다.
	CRowColArray awRows;
	GetSelectedRows(awRows, TRUE, FALSE);
	if (awRows.GetSize() != nLastRow-nFirstRow+1) 
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Can_t_not_move_uncontinuous_recor));
		return FALSE;
	}

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
	
	// 순번 변경하는 코드 추가.
	CArray<CString, CString&> aLoadTypeName;
	CArray<T_FBLD_D, T_FBLD_D&> aData;
	T_FBLD_K key;
	T_FBLD_D data;
	ROWCOL nkCol;
	GetColumnNoFor(GetKeyColID(), nkCol);
	for (int i = nFirstRow; i <= nLastRow; i++)
	{
		key = _tstol(GetValueRowCol(i, nkCol));
		m_pDoc->m_pAttrCtrl->GetFbld(key, data);
		data.FloorLoadTypeId = nStart;
		nStart += nInc;

		aLoadTypeName.Add(data.FloorLoadTypeName);
		aData.Add(data);
	}
	m_pDoc->m_pDataCtrl->ModifyFbld(aLoadTypeName, aData);
	
	return FALSE;
}
//<-----------------------------------------------------------------------------

//----------------------------------------------------------------------------->
// Current Record가 변경되었는지 검사
BOOL CFbldGrid::CheckCurrentRecordChanged()
{
	ROWCOL ncRow, ncCol;
	GetCurrentCell(ncRow, ncCol);
	if (ncRow != m_nCurrentRow)
	{
		OnChangeCurrentRecord(ncRow);
		return TRUE;
	}
	return FALSE;
}

BOOL CFbldGrid::ProcessKeys(CWnd *pSender, UINT nMessage, UINT nChar, UINT nRepCnt, UINT flags)
{
	BOOL bResult = CTBCommon::ProcessKeys(pSender, nMessage, nChar, nRepCnt, flags);
	CheckCurrentRecordChanged();
	return bResult;
}

void CFbldGrid::OnRButtonDblClk(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	ROWCOL ncRowBefore, ncRowAfter, ncCol;
	if (!GetCurrentCell(ncRowBefore, ncCol)) return;

	CTBCommon::OnRButtonDblClk(nFlags, point);

	if (!GetCurrentCell(ncRowAfter, ncCol)) return;
	if (ncRowBefore != ncRowAfter) OnChangeCurrentRecord(ncRowAfter);
}

void CFbldGrid::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CTBCommon::OnLButtonDblClk(nFlags, point);
	CheckCurrentRecordChanged();
}

void CFbldGrid::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CTBCommon::OnLButtonDown(nFlags, point);
	CheckCurrentRecordChanged();
}

void CFbldGrid::OnChangeCurrentRecord(ROWCOL nRow)
{
	m_nCurrentRow = nRow;

	// Append Row이면 그냥 리턴한다.
	if (nRow < 1) return;
	if (IsAppendRow(nRow)) 
	{
		m_pParent->OnChangeCurrentFbld(0);
		return;
	}

	// Key값을 알아와서 Parent Dialog에 Row의 변동을 알린다.
	ROWCOL nkCol;
	GetColumnNoFor(GetKeyColID(), nkCol);
	T_FBLD_K nKey = _ttoi(GetValueRowCol(nRow, nkCol));
	m_pParent->OnChangeCurrentFbld(nKey);
}
//<-----------------------------------------------------------------------------

BOOL CFbldGrid::GetCurrentFbld(T_FBLD_K& nKey)
{
	ROWCOL ncRow, ncCol;
	if (!GetCurrentCell(ncRow, ncCol)) return FALSE;
	if (ncRow < 1) return FALSE;
	if (IsAppendRow(ncRow)) return FALSE;

	if (!GetColumnNoFor(GetKeyColID(), ncCol)) return FALSE;
	nKey = _ttoi(GetValueRowCol(ncRow, ncCol));
	return TRUE;
}

BOOL CFbldGrid::SetCurrentFbld(T_FBLD_K key, T_FBLD_D& data)
{
	ROWCOL nRow, nCol;

	if (!GetCurrentCell(nRow, nCol)) return FALSE;
	
	CStringArray aKey;
	MakeSearchKey(key, data, aKey);
	if (!FindRecordByKey(aKey, nRow)) return FALSE;
	SetCurrentCell(nRow, nCol);
	CheckCurrentRecordChanged();
	return TRUE;
}
