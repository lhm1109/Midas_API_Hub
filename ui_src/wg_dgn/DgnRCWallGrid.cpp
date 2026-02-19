// DgnRCWallGrid.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnRCWallGrid.h"
#include "RCWallParameter.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_DBLib.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_PostCtrl.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define NAME_COL_ID         2
#define ELEM_KEY_FMT        _T("%d")

/////////////////////////////////////////////////////////////////////////////
// CDgnRCWallGrid
CString CDgnRCWallGrid::m_aCurDefVal[] = {_T(""), _T(""), _T(""), _T("")};


CDgnRCWallGrid::CDgnRCWallGrid()
{
	m_pDoc = CDBDoc::GetDocPoint();

	m_RcwaData.InitHashTable(101);

	m_pParent = 0;
	m_nCurrentRow = 0;

	m_nColCount = 3;
	m_nColHeader = 0;

	T_STLD_UNIT * pUnit = &(CUnitCtrl::m_STLD_UNIT);

	AddColInfo(_LSX(key),				t_integer,  0);
	AddColInfo(_LS(IDS_DGN_MEMB),					t_integer,  0);
	AddColInfo(_LS(IDS_DGN_RC_PLATE_CHECK_POSITION),			t_string,   0);
	AddColInfo(_LS(IDS_DGN_REMARK),					t_string,   0);
	
	m_aPrimaryKey.Add(0);
}

CDgnRCWallGrid::~CDgnRCWallGrid()
{
}

#define CWnd CTBCommon
BEGIN_MESSAGE_MAP(CDgnRCWallGrid, CWnd)
#undef CWnd
	//{{AFX_MSG_MAP(CDgnRCWallGrid)
	ON_WM_RBUTTONDBLCLK()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CDgnRCWallGrid message handlers
void CDgnRCWallGrid::Initialize(CRCWallParameter* pParent)
{
	CTBBrowserWndEx::Initialize();

	GetParam()->EnableUndo(FALSE);
	EnableHints();
	GetParam()->EnableTrackRowHeight(FALSE); 

	m_pParent = pParent;

	T_UNIT_SYSTEM UnitSystem;
	
	CTBCommon::m_pDoc = m_pDoc;

	LockUpdate(TRUE);

	// 컬럼 갯수 설정
	SetColCount(GetInitialColCount());
	SetHeaderTitle(UnitSystem, TRUE);

	// *^^* Grid에서 입력안하기 때문에 Static control로 변경
	// 표준 스타일 변경
	ChangeStandardStyle(CGXStyle()
			.SetFont(CGXFont_GC().SetSize(9))
			.SetVerticalAlignment(DT_BOTTOM)
			.SetAllowEnter(FALSE)
			.SetControl(GX_IDS_CTRL_STATIC));

	// 컬럼 스타일 변경
	SetStyleRange(CGXRange().SetCols(1, 4), CGXStyle()
			.SetHorizontalAlignment(DT_CENTER)
			.SetValueType(GX_VT_NUMERIC));
	SetRowHeightByDPI(0, 0, 18);

	// Key column의 스타일 변경
	/*
	SetStyleRange(CGXRange().SetCols(1, 2), CGXStyle()
			.SetHorizontalAlignment(DT_RIGHT)
			.SetValueType(GX_VT_NUMERIC)
			.SetFormat(GX_FMT_FIXED)
			.SetPlaces(0)
			.SetEnabled(FALSE));*/

	// Element.
	SetStyleRange(CGXRange().SetCols(1), CGXStyle()
			//.SetFormat(GX_FMT_FIXED)
			.SetPlaces(0));

	// 검토위치
	SetStyleRange(CGXRange().SetCols(2), CGXStyle()      
			.SetPlaces(0));

	// 비고.
	SetStyleRange(CGXRange().SetCols(3), CGXStyle()      
			.SetPlaces(0));

	// Key, Serial No Column은 숨긴다.
	// 주1 참조
	SetFrozenCols(1, 1);
	HideCols(1, 1);
	HideColsByIntend(1, 1, TRUE);

	// 현재 셀의 border 설정 변경
	CGXProperties* pProp = GetParam()->GetProperties();
			pProp->SetUserProperty(GX_IDS_OUTLINECURRENTCELL,
		      (CGXStyle) pProp->sInvertNoBorder);

	// 현재 셀 설정
	SetRightValidCell(1, 1);
	GetParam()->SetHideCurrentCell(GX_HIDE_NEVER);

	// Sorting 정보 초기화
	m_aSortInfo.SetSize(2);
	m_aSortInfo[0].nRC = 1; // serial no
	m_aSortInfo[0].sortType = CGXSortInfo::numeric;
	m_aSortInfo[0].sortOrder = CGXSortInfo::ascending;
	m_aSortInfo[1].nRC = 0; // key
	m_aSortInfo[1].sortType = CGXSortInfo::numeric;
	m_aSortInfo[1].sortOrder = CGXSortInfo::ascending;

	// always show vertical scrollbar
	SetScrollBarMode(SB_VERT, gxnEnabled | gxnEnhanced);  

	// Column 크기 설정
	SetColWidthByDPI(1, 1, 0);
	SetColWidthByDPI(2, 2, 60);
	SetColWidthByDPI(3, 3, 140);
	SetColWidthByDPI(4, 5, 145);

	MakeItemEx();
	LockUpdate(FALSE);

	SetModeDisplayOnly(TRUE);
	HideRows(GetRowCount(), GetRowCount());
}

void CDgnRCWallGrid::MakeItemEx()
{
	CWaitCursor wait;

	HideRows(GetRowCount(), GetRowCount());
	SetModeDisplayOnly(TRUE);

	BOOL bOldLock = LockUpdate(TRUE);

	CArray<T_RCWA_K, T_RCWA_K> KeyList;
	m_pDoc->m_pAttrCtrl->GetRcwaKeyList(KeyList);

	m_RcwaData.RemoveAll();
	T_RCWA_D Data;
	
	int KeyListSize = KeyList.GetSize();
	for(int i=0 ; i<KeyListSize ; i++)
	{
		if(m_pDoc->m_pAttrCtrl->GetRcwa(KeyList[i], Data))
			InsertRecordByKey(KeyList[i], Data);
	}

	LockUpdate(bOldLock);
	if (!bOldLock) Redraw();  
}

BOOL CDgnRCWallGrid::InsertRecordByKey(T_ELEM_K &key, T_RCWA_D &data)
{
	T_RCWA_D Data;
	if(m_RcwaData.Lookup(key, Data))//해당 키의 데이터가 있으면 Add하지 않는다
		return FALSE;
	m_RcwaData.SetAt(key, data);

	// 삽입 위치를 찾는다.
	ROWCOL nRow;

	// aKey를 만드는 코드 삽입
	CStringArray aKey;
	MakeSearchKey(key, data, aKey);
	FindRecordByKey(aKey, nRow);

	VERIFY(InsertRecord(nRow));
	return SetValue(nRow, key, data);
}

BOOL CDgnRCWallGrid::DeleteRecordByKey(T_ELEM_K &key, T_RCWA_D &data)
{
	T_RCWA_D Data;
	if(!m_RcwaData.Lookup(key, Data))//해당 키의 데이터가 없으면 Delete 하지 않는다.
		return FALSE;
	m_RcwaData.RemoveKey(key);

	// 레코드를 찾는다.
	ROWCOL nRow;

	// aKey를 만드는 코드 삽입
	CStringArray aKey;
	MakeSearchKey(key, data, aKey);
	if (!FindRecordByKey(aKey, nRow)) return FALSE;

	// 삭제한다.
	return DeleteRecord(nRow);
}

void CDgnRCWallGrid::UnitChanged(CDBDoc* pDoc)
{
	// unit을 사용하는 것이 없기 때문에 그냥 return 한다.
	return;
}


/////////////////////////////////////////////////////////////////////////////
// CDgnRCWallGrid Overridable Functions
void CDgnRCWallGrid::OnChangedSelectionPublic(long *keys, int nSize)
{
	if (nSize <= 0)
	{
		m_aSelectedKeys.RemoveAll();
	}
	else
	{
		m_aSelectedKeys.RemoveAll();
		for (int i = 0; i < nSize; i++) m_aSelectedKeys.Add((T_ELEM_K)keys[i]);
	}

	m_pParent->Data2Dlg();
}

long CDgnRCWallGrid::GetSelectedKeys(CArray<long, long> &caKey)
{
	// override this function to return selected record count

	/* 현재 Selected키 관리가 안되고 있다.
		 (UpdateBuffer에서 Add, Del, Modify 처리시 Selected key 관리를 
			안하고 있기 때문)
	*/
	long nKeyCount;
	CArray<T_ELEM_K, T_ELEM_K> aSelKey;

	GetAllSelectedStld(aSelKey);
	nKeyCount = aSelKey.GetSize();
	caKey.SetSize(nKeyCount);
	for (long i = 0; i < nKeyCount; i++)
		caKey[i] = (long)aSelKey[i];
	
	return nKeyCount;
}

BOOL CDgnRCWallGrid::ValidateField(CString value, int nColID)
{
	if (nColID >= GetColCount())
	{
		SetWarningText(_LS(IDS_WG_CMD__ADDD___CCMLoadCaseGrid__ValidateField__));
		return FALSE;
	}

	return TRUE;
}

// 삽입이나 수정전에 Record의 Validation을 검사한다.
BOOL CDgnRCWallGrid::ValidateRecord(CStringArray &value, CRowColArray &aCols)
{
	CString msg = _T("");
	int nPos;

	for (int i = 2; i < GetColCount()-1; i++)
	{
		nPos = aCols[i]-1;
		if (value[nPos] == _T(""))
			msg += ((msg != _T("")) ? _T("\n") : _T("")) + m_aTitle[i] + _LS(IDS_WG_CMD__ADDD___has_not_been_entered_);
	}
	if (msg == _T("")) return TRUE;
	else SetWarningText(msg);
	return FALSE;
}

// 이 함수는 Paste할 때 호출되므로 False를 리턴할 때는 ValidateField와 
// Validate Record에서 설정된 Warning Text를 없애주고 리턴한다.
BOOL CDgnRCWallGrid::ValidateAndMakeRecord(CStringArray &value, CRowColArray& aCols, T_ELEM_K &key, T_RCWA_D &data)
{
	// 각 필드에 대해 Validation을 검사한다.
	for (int i = 0; i < value.GetSize(); i++)
	{
		if (!ValidateField(value[aCols[i]-1], i)) 
		{
			SetWarningText(_T(""));
			return FALSE;
		}
	}
	// 레코드에 대해 Validation을 검사한다.
	if (!ValidateRecord(value, aCols))
	{
		SetWarningText(_T(""));
		return FALSE;
	}

	// 레코드를 만들어서 리턴한다.
	for (int i = 0; i < GetColCount(); i++)
	{
		int nPos = aCols[i]-1; // col id i에 해당하는 value가 저장된 위치
		if(i==0) 
		{
			if (value[nPos] == _T("")) key = 0;
			else key = _ttol(value[nPos]);
		}
		else ConvStrToData(i, value[nPos], data);
	}
	return TRUE;
}

BOOL CDgnRCWallGrid::AddToDB(ROWCOL nRow)
{
	// Record를 만들어서 DB에 입력한다.
	T_ELEM_K key;
	T_RCWA_D data;
 
	GetValue(nRow, key, data);
 
	return true;
}

BOOL CDgnRCWallGrid::AddToDB(CArray<void*, void*>& raKey, CArray<void*, void*>& raData)
{
	ASSERT(raKey.GetSize() == raData.GetSize());
	T_ELEM_K tKey;
	CArray<T_ELEM_K, T_ELEM_K> aKey;
	CArray<T_RCWA_D, T_RCWA_D&> aData;
	for (int i = 0; i < raKey.GetSize(); i++)
	{
		tKey = *((T_ELEM_K*)raKey.GetAt(i));
		aKey.Add(tKey);
		aData.Add(*((T_RCWA_D*)raData.GetAt(i)));
	}

	return true;
}


BOOL CDgnRCWallGrid::DeleteFromDB(ROWCOL nRow)
{
	T_ELEM_K key;
	T_RCWA_D data;
 
	GetValue(nRow, key, data);

	return true;
}

BOOL CDgnRCWallGrid::DeleteFromDB(CRowColArray& awRows)
{
	if (awRows.GetSize() <= 0) return TRUE;

	// _LS(IDS_WG_CMD__ADDD__Name) 칼럼을 얻는다.
	ROWCOL nCol;
	VERIFY(GetColumnNoFor(NAME_COL_ID, nCol));

	CString value;
	CArray<CString, CString&> awKeys;
 
	for (int i = 0; i < awRows.GetSize(); i++)
	{
		value = GetValueRowColBeforeEdit(awRows.GetAt(i), nCol);
		awKeys.Add(value);
	}

	return TRUE;  
}

BOOL CDgnRCWallGrid::ModifyToDB(ROWCOL nRow)
{
	ROWCOL nCol;
	VERIFY(GetColumnNoFor(2, nCol));  // load case name

	T_ELEM_K key;
	T_RCWA_D data;
	CString pOldLoadCaseName;

	pOldLoadCaseName = GetValueRowColBeforeEdit(nRow, nCol);
	GetValue(nRow, key, data);

	return true;
}

BOOL CDgnRCWallGrid::ModifyToDB(CStringArray& raOldKey, 
												 CArray<void*, void*>& raKey, 
												 CArray<void*, void*>& raData)
{
	ASSERT(raKey.GetSize() == raData.GetSize());
	ASSERT(raKey.GetSize() == raOldKey.GetSize());
	CArray<CString, CString&> aOldKey;
	CArray<T_RCWA_D, T_RCWA_D&> aData;
	T_ELEM_K Key;
	T_RCWA_D Data;
	for (int i = 0; i < raKey.GetSize(); i++)
	{
		Key = _ttol(raOldKey.GetAt(i));
		
		aData.Add(*((T_RCWA_D*)raData.GetAt(i)));
	}
	
	return true;
}


CString CDgnRCWallGrid::GetNewKey()
{
	CString value;
	T_ELEM_K key = m_pDoc->m_pAttrCtrl->GetStartNumElem();
	
	value.Format(ELEM_KEY_FMT, key);
	return value;
}

CString CDgnRCWallGrid::GetDefValue(int nIndex)
{
	return m_aCurDefVal[nIndex];
}

CString* CDgnRCWallGrid::GetDefValueArray()
{
	return m_aCurDefVal;
}

BOOL CDgnRCWallGrid::MakeKeyAndDataArray(CStringArray& value, CRowColArray& awCols, 
																	CArray<void*, void*>& aKey, CArray<void*, void*>& aData)
{
	T_ELEM_K* pKey;
	T_RCWA_D* pData;
	pKey = new T_ELEM_K;
	pData = new T_RCWA_D;
	aKey.Add((void*)pKey);
	aData.Add((void*)pData);
	if (!ValidateAndMakeRecord(value, awCols, *pKey, *pData)) return FALSE;
	return TRUE;
}

void CDgnRCWallGrid::DeleteKeyAndDataArray(CArray<void*, void*>& aKey, CArray<void*, void*>& aData)
{
	T_ELEM_K* pKey;
	T_RCWA_D* pData;
	ASSERT(aKey.GetSize() == aData.GetSize());
	for (int i = 0; i < aKey.GetSize(); i++)
	{
		pKey = (T_ELEM_K*)aKey.GetAt(i);
		pData = (T_RCWA_D*)aData.GetAt(i);
		delete pKey;
		delete pData;
	}
}

long CDgnRCWallGrid::GetCountData()
{
	CDBDoc* pDoc = m_pDoc;
	return pDoc->m_pAttrCtrl->GetCountElem();  
}

void CDgnRCWallGrid::MakeItemFiltered(long nFilteredNum, long* aFilteredKeys, CRowColArray& awCols)
{
	T_RCWA_D Data;

	for(long nCount=0 ; nCount < nFilteredNum; nCount++)
 	{
		SetValue(GetParam()->GetData(), nCount+1, aFilteredKeys[nCount], Data, awCols);
	}
}

void CDgnRCWallGrid::MakeItemNormal(long nNumData, CRowColArray& awCols)
{
	//T_ELEM_K Key;
	T_RCWA_D Data;
	CDBDoc* pDoc = m_pDoc;

	long* KeyBuf = new long[nNumData];
	long nCount = 0;
	
	POSITION pos = pDoc->m_pAttrCtrl->GetStartElem();
	while(pos != NULL)
	{
	
	}
	qsort(KeyBuf, nNumData, sizeof(T_ELEM_K), CTBBrowserWnd::CompareKey);

	for(nCount=0 ; nCount < nNumData; nCount++)
	{
		 SetValue(GetParam()->GetData(), nCount+1, KeyBuf[nCount], Data, awCols);
	} // end of loop nCount

	delete []KeyBuf;
}

void CDgnRCWallGrid::MakeItemEditingRecord(ROWCOL ncRow, CStringArray& EditingRecord)
{
	T_ELEM_K Key;
	T_RCWA_D Data;
	for (int i = 0; i < GetColCount(); i++)
	{
		if (i == 0) Key = _ttol(EditingRecord[i]);
		else ConvStrToData(i, EditingRecord[i], Data);
	}  
	// 현재 값을 설정한다.
	SetValue(ncRow, Key, Data);
}

/////////////////////////////////////////////////////////////////////////////
// CDgnRCWallGrid Implementation
BOOL CDgnRCWallGrid::GetValue(ROWCOL nRow, T_ELEM_K &key, T_RCWA_D &data)
{
	ROWCOL nCol;
	CString str;
	for (int i = 0; i < GetColCount(); i++)
	{
		VERIFY(GetColumnNoFor(m_aTitle[i], nCol));
		str = GetValueRowCol(nRow, nCol);
		// 수식인 경우 값으로 변환한다.
		// Validation이 OK인 Field이므로 항상 TRUE 리턴
		VERIFY(ConvertFormulaToValue(str)); 

		if(i==0) 
		{
			if (str == _T(""))
				key = m_pDoc->m_pAttrCtrl->GetStartNumElem();        
			else
				key = _ttol(str);
		}
		else ConvStrToData(i, str, data);
	}
	return TRUE;
}

BOOL CDgnRCWallGrid::SetValue(ROWCOL nRow, T_ELEM_K key, T_RCWA_D &data)
{
	CRowColArray awCols;
	awCols.SetSize(GetColCount());
	for (int i = 0;i < GetColCount(); i++)
		VERIFY(GetColumnNoFor(m_aTitle[i], awCols[i]));
	return SetValue(GetParam()->GetData(), nRow, key, data, awCols);
}

BOOL CDgnRCWallGrid::SetValue(CGXData* pData, ROWCOL nRow, T_ELEM_K key, T_RCWA_D &data, CRowColArray &awCols)
{
	CString str;
	for (int i = 0; i < GetColCount(); i++)
	{
		if(i==0) 
		{
			if (key == 0) str.Empty();
			else str.Format(ELEM_KEY_FMT,key);
		}
		else ConvDataToStr(i, key, data, str);
		pData->StoreValueRowCol(nRow, awCols[i], str, gxOverride, GX_VT_NUMERIC);
	}

	return TRUE;
}

void CDgnRCWallGrid::ConvStrToData(int i, CString& value, T_RCWA_D& data)
{

}




void CDgnRCWallGrid::ConvDataToStr(int i, T_ELEM_K key, T_RCWA_D& data, CString& value)
{
	switch(i)
	{
		case 1:
			value.Format(_T("%d"), key);
			break;
		case 2:
			value = _T("");
			if(data.bPosi[0])
				value = _T("I");
			if(data.bPosi[1])
			{
				if(value == _T(""))
					value = _T("MID");
				else
					value += _T(", MID");
			}
			if(data.bPosi[2])
			{
				if(value == _T(""))
					value = _T("J");
				else
					value += _T(", J");
			}
			break;
		case 3:
			value = data.strRemark;
			break;		
	}	
}



void CDgnRCWallGrid::MakeSearchKey(T_ELEM_K &key, T_RCWA_D &data, CStringArray &aKey)
{
	int nColID;
	int nSortKeyNum = m_aSortInfo.GetSize();

	aKey.SetSize(nSortKeyNum);
	for (int i = 0; i < nSortKeyNum; i++)
	{
		nColID = m_aSortInfo[i].nRC;
		if (nColID == 0) aKey[i].Format(ELEM_KEY_FMT, key);
		else ConvDataToStr(nColID, key, data, aKey[i]);
	}
}

void CDgnRCWallGrid::GetAllSelectedStld(CArray<T_ELEM_K, T_ELEM_K>& aSelKey)
{
	for (int i = 0; i < m_aSelectedKeys.GetSize(); i++)
		aSelKey.Add(m_aSelectedKeys[i]);
}

void CDgnRCWallGrid::SetHeaderTitle(T_UNIT_SYSTEM &UnitSystem, BOOL bInit)
{
	SetHeaderTitle2((int*)m_aUnit.GetData(), bInit, TRUE);
}


// Hide 된 Row들이 resize되지 않도록 한다.
// 주1 참조
BOOL CDgnRCWallGrid::OnTrackColWidth(ROWCOL nCol)
{
	if (nCol == 1) return FALSE;
	return CTBCommon::OnTrackColWidth(nCol);
}

// TBBrowserWndEx::OnSelDragColsDrop 참조
// 부모를 먼저 호출하면 안 된다.
// 1, 2 모두 Frozen Cols로 설정되어 있으므로 이쪽으로 셀이 이동되거나 이 셀들이
// 이동되는 것을 금지한다.
// 주1 참조
BOOL CDgnRCWallGrid::OnSelDragColsDrop(ROWCOL nFirstCol, ROWCOL nLastCol, ROWCOL nDestCol)
{
	// Frozen 되어 있는 column 위치에 이동되면 이동을 금지시킨다.
	//if (IsFrozenCol(nDestCol)) return FALSE;
	if (nDestCol == 1 || nFirstCol == 1) return FALSE;
	if (nDestCol == 2 || nFirstCol == 2) return FALSE;
	return CTBCommon::OnSelDragColsDrop(nFirstCol, nLastCol, nDestCol);
}


// 이 함수는 No Column을 클릭했을 때 Row Header를 클릭한 것과 같은 효과를
// 내기 위해서이다. CTBBrowserWndEx::OnLButtonDown 참조
void CDgnRCWallGrid::OnLButtonDown(UINT nFlags, CPoint point)
{
	ROWCOL nhRow, nhCol, nkCol;
	int ht = HitTest(point, &nhRow, &nhCol);
	if (ht != GX_NOHIT && ht != GX_VERTLINE)
	{
		GetColumnNoFor(1, nkCol);
		if (nhCol == nkCol && IsFrozenCol(nhCol)) // key column이면
		{
			// Row header의 rectangle을 얻는다.
			CRect r = CalcRectFromRowColEx(nhRow, 0, nhRow, 0);  
			point.x = (r.left+r.right)/2;
		}
	}
	CTBCommon::OnLButtonDown(nFlags, point);

	CheckCurrentRecordChanged();
}


//----------------------------------------------------------------------------->
// 여기서 부터 Current Record의 위치 변경을 Detect하기 위해 삽입된 코드이다.
// Current Record가 변경되었는지 검사
BOOL CDgnRCWallGrid::CheckCurrentRecordChanged()
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

BOOL CDgnRCWallGrid::ProcessKeys(CWnd *pSender, UINT nMessage, UINT nChar, UINT nRepCnt, UINT flags)
{
	BOOL bResult = CTBCommon::ProcessKeys(pSender, nMessage, nChar, nRepCnt, flags);
	CheckCurrentRecordChanged();
	return bResult;
}

void CDgnRCWallGrid::OnRButtonDblClk(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	ROWCOL ncRowBefore, ncRowAfter, ncCol;
	if (!GetCurrentCell(ncRowBefore, ncCol)) return;

	CTBCommon::OnRButtonDblClk(nFlags, point);

	if (!GetCurrentCell(ncRowAfter, ncCol)) return;
	if (ncRowBefore != ncRowAfter) OnChangeCurrentRecord(ncRowAfter);
}

void CDgnRCWallGrid::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CTBCommon::OnLButtonDblClk(nFlags, point);
	CheckCurrentRecordChanged();
}


void CDgnRCWallGrid::OnChangeCurrentRecord(ROWCOL nRow)
{
	m_nCurrentRow = nRow;

	// Append Row이면 그냥 리턴한다.~
	if (nRow < 1) return;
	if (IsAppendRow(nRow)) return;

	// Key값을 알아와서 Parent Dialog에 Row의 변동을 알린다.
	ROWCOL nkCol;
	GetColumnNoFor(GetKeyColID(), nkCol);
	T_ELEM_K nKey = _ttoi(GetValueRowCol(nRow, nkCol));
	m_pParent->OnChangeCurrentRcwa(nKey);
}
//<-----------------------------------------------------------------------------

BOOL CDgnRCWallGrid::GetCurrentRcwa(T_ELEM_K& nKey)
{
	ROWCOL ncRow, ncCol;
	if (!GetCurrentCell(ncRow, ncCol)) return FALSE;
	if (ncRow < 1) return FALSE;
	if (IsAppendRow(ncRow)) return FALSE;

	if (!GetColumnNoFor(GetKeyColID(), ncCol)) return FALSE;
	nKey = _ttoi(GetValueRowCol(ncRow, ncCol));
	return TRUE;
}

BOOL CDgnRCWallGrid::SetCurrentRcwa(T_ELEM_K key, T_RCWA_D& data)
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