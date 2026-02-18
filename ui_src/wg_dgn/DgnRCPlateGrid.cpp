// DgnRCPlateGrid.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnRCPlateGrid.h"
#include "RCPlateParameter.h"

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
// CDgnRCPlateGrid
CString CDgnRCPlateGrid::m_aCurDefVal[] = {_T(""), _T(""), _T("")};


CDgnRCPlateGrid::CDgnRCPlateGrid()
{
	m_pDoc = CDBDoc::GetDocPoint();

	m_RcplData.InitHashTable(101);  

	m_pParent = 0;
	m_nCurrentRow = 0;

	m_nColCount = 2;
	m_nColHeader = 0;

	AddColInfo(_LSX(key),							t_integer,  0);
	AddColInfo(_LS(IDS_DGN_NAME),								t_string,		0);
	AddColInfo(_LS(IDS_DGN_RC_PLATE_CHECK_MEMB),						t_string,   0);  
	
	m_aPrimaryKey.Add(0);
}

CDgnRCPlateGrid::~CDgnRCPlateGrid()
{
}

#define CWnd CTBCommon
BEGIN_MESSAGE_MAP(CDgnRCPlateGrid, CWnd)
#undef CWnd
	//{{AFX_MSG_MAP(CDgnRCPlateGrid)
	ON_WM_RBUTTONDBLCLK()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CDgnRCPlateGrid message handlers
void CDgnRCPlateGrid::Initialize(CRCPlateParameter* pParent)
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
	SetStyleRange(CGXRange().SetCols(1, 3), CGXStyle()
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

	// Name.
	SetStyleRange(CGXRange().SetCols(1), CGXStyle()
			//.SetFormat(GX_FMT_FIXED)
			.SetPlaces(0));
	
	// 검토위치
	SetStyleRange(CGXRange().SetCols(2), CGXStyle()      
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
	SetColWidthByDPI(2, 2, 120);
	SetColWidthByDPI(3, 3, 240);
		
	MakeItemEx();
	LockUpdate(FALSE);

	SetModeDisplayOnly(TRUE);
	HideRows(GetRowCount(), GetRowCount());
}

void CDgnRCPlateGrid::MakeItemEx()
{
	CWaitCursor wait;

	HideRows(GetRowCount(), GetRowCount());
	SetModeDisplayOnly(TRUE);

	BOOL bOldLock = LockUpdate(TRUE);

	CArray<T_RCPL_K, T_RCPL_K> KeyList;
	m_pDoc->m_pAttrCtrl->GetRcplKeyList(KeyList);

	m_RcplData.RemoveAll();
	T_RCPL_D Data;
	
	int KeyListSize = KeyList.GetSize();
	for(int i=0 ; i<KeyListSize ; i++)
	{
		if(m_pDoc->m_pAttrCtrl->GetRcpl(KeyList[i], Data))
			InsertRecordByKey(Data.strName);
	}
	//SetRightValidCell(1+m_nColHeader, 1);

	LockUpdate(bOldLock);
	if (!bOldLock) Redraw();  
}

BOOL CDgnRCPlateGrid::InsertRecordByKey(CString srtName)
{
	T_RCPL_K key;
	T_RCPL_D Data;
	Data.Initialize();
	key = m_pDoc->m_pAttrCtrl->GetRcplKey(srtName);
	m_pDoc->m_pAttrCtrl->GetRcpl(srtName, Data);
	
	if(m_RcplData.Lookup(key, Data))//해당 키의 데이터가 있으면 Add하지 않는다
		return FALSE;
	m_RcplData.SetAt(key, Data);
	
	// 삽입 위치를 찾는다.
	ROWCOL nRow;

	// aKey를 만드는 코드 삽입
	CStringArray aKey;
	MakeSearchKey(key, Data, aKey);
	FindRecordByKey(aKey, nRow);

	VERIFY(InsertRecord(nRow));
	return SetValue(nRow, key, Data);
}


BOOL CDgnRCPlateGrid::DeleteRecordByKey(T_RCPL_K &key, T_RCPL_D &data)
{
	T_RCPL_D Data;
	if(!m_RcplData.Lookup(key, Data))//해당 키의 데이터가 없으면 Delete 하지 않는다.
		return FALSE;
	m_RcplData.RemoveKey(key);
	
	// 레코드를 찾는다.
	ROWCOL nRow;

	// aKey를 만드는 코드 삽입
	CStringArray aKey;
	MakeSearchKey(key, data, aKey);
	if (!FindRecordByKey(aKey, nRow)) return FALSE;

	// 삭제한다.
	return DeleteRecord(nRow);
}
/*
BOOL CDgnRCPlateGrid::DeleteRecordByKey(CString srtName)
{
	T_RCPL_K key;
	T_RCPL_D Data;
	Data.Initialize();
	key = m_pDoc->m_pAttrCtrl->GetRcplKey(srtName);
	m_pDoc->m_pAttrCtrl->GetRcpl(srtName, Data);	
	
	// 레코드를 찾는다.
	ROWCOL nRow;

	// aKey를 만드는 코드 삽입
	CStringArray aKey;
	MakeSearchKey(key, Data, aKey);
	if (!FindRecordByKey(aKey, nRow)) return FALSE;

	// 삭제한다.
	return DeleteRecord(nRow);
}*/

void CDgnRCPlateGrid::UnitChanged(CDBDoc* pDoc)
{
	// unit을 사용하는 것이 없기 때문에 그냥 return 한다.
	return;
}


/////////////////////////////////////////////////////////////////////////////
// CDgnRCPlateGrid Overridable Functions
void CDgnRCPlateGrid::OnChangedSelectionPublic(long *keys, int nSize)
{
	if (nSize <= 0)
	{
		m_aSelectedKeys.RemoveAll();
	}
	else
	{
		m_aSelectedKeys.RemoveAll();
		for (int i = 0; i < nSize; i++) m_aSelectedKeys.Add((T_RCPL_K)keys[i]);
	}

	//m_pParent->Data2Dlg();
}

long CDgnRCPlateGrid::GetSelectedKeys(CArray<long, long> &caKey)
{
	// override this function to return selected record count

	/* 현재 Selected키 관리가 안되고 있다.
		 (UpdateBuffer에서 Add, Del, Modify 처리시 Selected key 관리를 
			안하고 있기 때문)
	*/
	long nKeyCount;
	CArray<T_RCPL_K, T_RCPL_K> aSelKey;

	GetAllSelectedStld(aSelKey);
	nKeyCount = aSelKey.GetSize();
	caKey.SetSize(nKeyCount);
	for (long i = 0; i < nKeyCount; i++)
		caKey[i] = (long)aSelKey[i];
	
	return nKeyCount;
}

BOOL CDgnRCPlateGrid::ValidateField(CString value, int nColID)
{
	if (nColID >= GetColCount())
	{
		SetWarningText(_LS(IDS_WG_CMD__ADDD___CCMLoadCaseGrid__ValidateField__));
		return FALSE;
	}

	return TRUE;
}

// 삽입이나 수정전에 Record의 Validation을 검사한다.
BOOL CDgnRCPlateGrid::ValidateRecord(CStringArray &value, CRowColArray &aCols)
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
BOOL CDgnRCPlateGrid::ValidateAndMakeRecord(CStringArray &value, CRowColArray& aCols, T_RCPL_K &key, T_RCPL_D &data)
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

BOOL CDgnRCPlateGrid::AddToDB(ROWCOL nRow)
{
	//
	return true;
}

BOOL CDgnRCPlateGrid::AddToDB(CArray<void*, void*>& raKey, CArray<void*, void*>& raData)
{
	//
	return true;
}


BOOL CDgnRCPlateGrid::DeleteFromDB(ROWCOL nRow)
{
	//
	return true;
}

BOOL CDgnRCPlateGrid::DeleteFromDB(CRowColArray& awRows)
{
	//
	return true;  
}

BOOL CDgnRCPlateGrid::ModifyToDB(ROWCOL nRow)
{
	//
	return true;
}

BOOL CDgnRCPlateGrid::ModifyToDB(CStringArray& raOldKey, 
												 CArray<void*, void*>& raKey, 
												 CArray<void*, void*>& raData)
{
	//
	return true;
}


CString CDgnRCPlateGrid::GetNewKey()
{
	CString value = _T("");
	//T_RCPL_K key = m_pDoc->m_pAttrCtrl->GetStartNumElem();  
	//value.Format(ELEM_KEY_FMT, key);
	return value;
}

CString CDgnRCPlateGrid::GetDefValue(int nIndex)
{
	return m_aCurDefVal[nIndex];
}

CString* CDgnRCPlateGrid::GetDefValueArray()
{
	return m_aCurDefVal;
}

BOOL CDgnRCPlateGrid::MakeKeyAndDataArray(CStringArray& value, CRowColArray& awCols, 
																	CArray<void*, void*>& aKey, CArray<void*, void*>& aData)
{
	T_RCPL_K* pKey;
	T_RCPL_D* pData;
	pKey = new T_RCPL_K;
	pData = new T_RCPL_D;
	aKey.Add((void*)pKey);
	aData.Add((void*)pData);
	if (!ValidateAndMakeRecord(value, awCols, *pKey, *pData)) return FALSE;
	return TRUE;
}

void CDgnRCPlateGrid::DeleteKeyAndDataArray(CArray<void*, void*>& aKey, CArray<void*, void*>& aData)
{
	T_RCPL_K* pKey;
	T_RCPL_D* pData;
	ASSERT(aKey.GetSize() == aData.GetSize());
	for (int i = 0; i < aKey.GetSize(); i++)
	{
		pKey = (T_RCPL_K*)aKey.GetAt(i);
		pData = (T_RCPL_D*)aData.GetAt(i);
		delete pKey;
		delete pData;
	}
}

long CDgnRCPlateGrid::GetCountData()
{
	CDBDoc* pDoc = m_pDoc;
	CArray<T_RCPL_K, T_RCPL_K> aKeyList;
	pDoc->m_pAttrCtrl->GetRcplKeyList(aKeyList);

	return (long)aKeyList.GetSize();	
}

void CDgnRCPlateGrid::MakeItemFiltered(long nFilteredNum, long* aFilteredKeys, CRowColArray& awCols)
{
	CDBDoc* pDoc = m_pDoc;
	T_RCPL_D Data;

	for(long nCount=0 ; nCount < nFilteredNum; nCount++)
 	{
		//pDoc->m_pAttrCtrl->GetStld(aFilteredKeys[nCount], Data);
		SetValue(GetParam()->GetData(), nCount+1, aFilteredKeys[nCount], Data, awCols);
	}
}

void CDgnRCPlateGrid::MakeItemNormal(long nNumData, CRowColArray& awCols)
{
	//T_RCPL_K Key;
	T_RCPL_D Data;
	CDBDoc* pDoc = m_pDoc;

	long* KeyBuf = new long[nNumData];
	long nCount = 0;

	qsort(KeyBuf, nNumData, sizeof(T_RCPL_K), CTBBrowserWnd::CompareKey);

	for(nCount=0 ; nCount < nNumData; nCount++)
	{
		//pDoc->m_pAttrCtrl->GetStld(KeyBuf[nCount], Data);
		SetValue(GetParam()->GetData(), nCount+1, KeyBuf[nCount], Data, awCols);
	} // end of loop nCount

	delete []KeyBuf;
}

void CDgnRCPlateGrid::MakeItemEditingRecord(ROWCOL ncRow, CStringArray& EditingRecord)
{
	CDBDoc* pDoc = m_pDoc;
	T_RCPL_K Key;
	T_RCPL_D Data;
	for (int i = 0; i < GetColCount(); i++)
	{
		if (i == 0) Key = _ttol(EditingRecord[i]);
		else ConvStrToData(i, EditingRecord[i], Data);
	}
	
	// 현재 값을 설정한다.
	SetValue(ncRow, Key, Data);
}

/////////////////////////////////////////////////////////////////////////////
// CDgnRCPlateGrid Implementation
BOOL CDgnRCPlateGrid::GetValue(ROWCOL nRow, T_RCPL_K &key, T_RCPL_D &data)
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
				//key = m_pDoc->m_pAttrCtrl->GetStartNumStld();
			else
				key = _ttol(str);
		}
		else ConvStrToData(i, str, data);
	}
	return TRUE;
}

BOOL CDgnRCPlateGrid::SetValue(ROWCOL nRow, T_RCPL_K key, T_RCPL_D &data)
{
	//
	CRowColArray awCols;
	awCols.SetSize(GetColCount());
	for (int i = 0;i < GetColCount(); i++)
		VERIFY(GetColumnNoFor(m_aTitle[i], awCols[i]));
	return SetValue(GetParam()->GetData(), nRow, key, data, awCols);
}

BOOL CDgnRCPlateGrid::SetValue(CGXData* pData, ROWCOL nRow, T_RCPL_K key, T_RCPL_D &data, CRowColArray &awCols)
{
	//
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

void CDgnRCPlateGrid::ConvStrToData(int i, CString& value, T_RCPL_D& data)
{/*
	if (i==1) data.LoadCaseId = _ttol(value); // serial no
	else if (i==2) data.LoadCaseName = value; // name
	else if (i==3) data.LoadCaseType = GetLoadTypeCode(value); // type
	else if (i==4) data.Description = value; // description */
}

void CDgnRCPlateGrid::ConvDataToStr(int i, T_RCPL_K key, T_RCPL_D& data, CString& value)
{
	switch(i)
	{
		case 1:
			value = data.strName;
			break;
		case 2:
			value = _T("");
			CString srtTemp = _T("");
			int size = data.arElemK.GetSize();
			for(int c=0 ; c<size ; c++)
			{
				srtTemp.Format(_T("%d"), data.arElemK[c]);
				if(value == _T(""))
					value = srtTemp;
				else
					value += _T(", ") + srtTemp;
			}
			break;
	}
}



void CDgnRCPlateGrid::MakeSearchKey(T_RCPL_K &key, T_RCPL_D &data, CStringArray &aKey)
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

void CDgnRCPlateGrid::GetAllSelectedStld(CArray<T_RCPL_K, T_RCPL_K>& aSelKey)
{
	for (int i = 0; i < m_aSelectedKeys.GetSize(); i++)
		aSelKey.Add(m_aSelectedKeys[i]);
}

void CDgnRCPlateGrid::SetHeaderTitle(T_UNIT_SYSTEM &UnitSystem, BOOL bInit)
{
	SetHeaderTitle2((int*)m_aUnit.GetData(), bInit, TRUE);
}


// Hide 된 Row들이 resize되지 않도록 한다.
// 주1 참조
BOOL CDgnRCPlateGrid::OnTrackColWidth(ROWCOL nCol)
{
	if (nCol == 1) return FALSE;
	return CTBCommon::OnTrackColWidth(nCol);
}

// TBBrowserWndEx::OnSelDragColsDrop 참조
// 부모를 먼저 호출하면 안 된다.
// 1, 2 모두 Frozen Cols로 설정되어 있으므로 이쪽으로 셀이 이동되거나 이 셀들이
// 이동되는 것을 금지한다.
// 주1 참조
BOOL CDgnRCPlateGrid::OnSelDragColsDrop(ROWCOL nFirstCol, ROWCOL nLastCol, ROWCOL nDestCol)
{
	// Frozen 되어 있는 column 위치에 이동되면 이동을 금지시킨다.
	//if (IsFrozenCol(nDestCol)) return FALSE;
	if (nDestCol == 1 || nFirstCol == 1) return FALSE;
	if (nDestCol == 2 || nFirstCol == 2) return FALSE;
	return CTBCommon::OnSelDragColsDrop(nFirstCol, nLastCol, nDestCol);
}


// 이 함수는 No Column을 클릭했을 때 Row Header를 클릭한 것과 같은 효과를
// 내기 위해서이다. CTBBrowserWndEx::OnLButtonDown 참조
void CDgnRCPlateGrid::OnLButtonDown(UINT nFlags, CPoint point)
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
BOOL CDgnRCPlateGrid::CheckCurrentRecordChanged()
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

BOOL CDgnRCPlateGrid::ProcessKeys(CWnd *pSender, UINT nMessage, UINT nChar, UINT nRepCnt, UINT flags)
{
	BOOL bResult = CTBCommon::ProcessKeys(pSender, nMessage, nChar, nRepCnt, flags);
	CheckCurrentRecordChanged();
	return bResult;
}

void CDgnRCPlateGrid::OnRButtonDblClk(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	ROWCOL ncRowBefore, ncRowAfter, ncCol;
	if (!GetCurrentCell(ncRowBefore, ncCol)) return;

	CTBCommon::OnRButtonDblClk(nFlags, point);

	if (!GetCurrentCell(ncRowAfter, ncCol)) return;
	if (ncRowBefore != ncRowAfter) OnChangeCurrentRecord(ncRowAfter);
}

void CDgnRCPlateGrid::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CTBCommon::OnLButtonDblClk(nFlags, point);
	CheckCurrentRecordChanged();
}

void CDgnRCPlateGrid::OnChangeCurrentRecord(ROWCOL nRow)
{
	m_nCurrentRow = nRow;

	// Append Row이면 그냥 리턴한다.~
	if (nRow < 1) return;
	if (IsAppendRow(nRow)) return;

	// Key값을 알아와서 Parent Dialog에 Row의 변동을 알린다.
	ROWCOL nkCol;
	GetColumnNoFor(GetKeyColID(), nkCol);
	T_RCPL_K nKey = _ttoi(GetValueRowCol(nRow, nkCol));
	m_pParent->OnChangeCurrentRcpl(nKey);
}
//<-----------------------------------------------------------------------------

BOOL CDgnRCPlateGrid::GetCurrentRcpl(T_RCPL_K& nKey)
{
	ROWCOL ncRow, ncCol;
	if (!GetCurrentCell(ncRow, ncCol)) return FALSE;
	if (ncRow < 1) return FALSE;
	if (IsAppendRow(ncRow)) return FALSE;

	if (!GetColumnNoFor(GetKeyColID(), ncCol)) return FALSE;
	nKey = _ttoi(GetValueRowCol(ncRow, ncCol));
	return TRUE;
}

BOOL CDgnRCPlateGrid::SetCurrentRcpl(T_RCPL_K key, T_RCPL_D& data)
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

BOOL CDgnRCPlateGrid::LockUpdateGrid(BOOL bUpdate)
{
	return LockUpdate(bUpdate);
}