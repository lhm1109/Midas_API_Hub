// CMStageGrid.cpp : implementation file
//

#include "stdafx.h"
/* 사용안함
#include "wg_cmd.h"
#include "CMStageGrid.h"
#include "CMStageDefMainDlg.h"

#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_ViewBuff.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// 주1:
// Column 1(Key), 2(Id)는 사용자와는 무관하며 프로그램상 필요하므로 Hide시킨다. 
// 따라서 어떤식으로든 사용자에게 나타나서는 안된다.

#define NAME_COL_ID         3
#define STAG_KEY_FMT        _T("%d")
#define STAG_DATA_FMT       _T("%.18g")


/////////////////////////////////////////////////////////////////////////////
// CCMStageGrid
CString CCMStageGrid::m_aCurDefVal[] = {_T("0"), _T("0"), _T(""), _T("0"), _T("0"), _T("0"), _T("0.0"), _T("0")};
CCMStageGrid::CCMStageGrid()
{
	m_pParent = 0;
	m_nCurrentRow = 0;

	m_bBeforeSaved = FALSE;

	T_STAG_UNIT * pUnit = &(CUnitCtrl::m_STAG_UNIT);

	AddColInfo(_LS(IDS_CMD_STAG_Key),       t_integer,  0);
	AddColInfo(_LS(IDS_CMD_STAG_Id),        t_integer,  0);
	AddColInfo(_LS(IDS_CMD_STAG_Final),     t_integer,  0);
	AddColInfo(_LS(IDS_CMD_STAG_Name),      t_string,   0);
	AddColInfo(_LS(IDS_CMD_STAG_Duration),  t_integer,  pUnit->nDuration);
	AddColInfo(_LS(IDS_CMD_STAG_Date),      t_integer,  pUnit->nDuration);
	AddColInfo(_LS(IDS_CMD_STAG_T_Step),    t_integer,  0);
	AddColInfo(_LS(IDS_CMD_STAG_Distance),  t_real,     pUnit->dLaunchDist);
	AddColInfo(_LS(IDS_CMD_STAG_Step),      t_string,   0);

	m_aPrimaryKey.Add(1);
	m_aPrimaryKey.Add(0);
}

CCMStageGrid::~CCMStageGrid()
{
}

#define CWnd CTBCommon
BEGIN_MESSAGE_MAP(CCMStageGrid, CWnd)
#undef CWnd
	//{{AFX_MSG_MAP(CCMStageGrid)
	ON_WM_RBUTTONDBLCLK()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CCMStageGrid message handlers
void CCMStageGrid::Initialize(CCMStageDefMainDlg* pParent)
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	m_pParent = pParent;

	T_UNIT_SYSTEM UnitSystem;
	CTBCommon::Initialize(pDoc, UnitSystem);

	LockUpdate(TRUE);

	// 컬럼 갯수 설정
	SetColCount(GetInitialColCount());
	SetHeaderTitle(UnitSystem, TRUE);

	// 표준 스타일 변경
	ChangeStandardStyle(CGXStyle()
			.SetFont(CGXFont_GC().SetSize(10))
			.SetVerticalAlignment(DT_BOTTOM)
			.SetAllowEnter(FALSE));
	SetRowHeight(0, 0, 36);

	// 컬럼 스타일 변경
	SetStyleRange(CGXRange().SetCols(4), CGXStyle()
			.SetHorizontalAlignment(DT_LEFT)
			.SetValueType(GX_VT_STRING));

	// Key, Id column의 스타일 변경
	SetStyleRange(CGXRange().SetCols(1, 2), CGXStyle()
			.SetHorizontalAlignment(DT_RIGHT)
			.SetValueType(GX_VT_NUMERIC)
			.SetFormat(GX_FMT_FIXED)
			.SetPlaces(0)
			.SetEnabled(FALSE));
	SetStyleRange(CGXRange().SetCols(3), CGXStyle()
			.SetValueType(GX_VT_NUMERIC)
			.SetFormat(GX_FMT_FIXED)
			.SetPlaces(0)
			.SetHorizontalAlignment(DT_CENTER)
			.SetVerticalAlignment(DT_VCENTER)
			.SetValue(_T(""))
			.SetControl(GX_IDS_CTRL_CHECKBOX3D));
	SetStyleRange(CGXRange().SetCols(5, 9), CGXStyle()
			.SetHorizontalAlignment(DT_RIGHT)
			.SetValueType(GX_VT_NUMERIC)
			.SetFormat(GX_FMT_FIXED)
			.SetPlaces(0));
	SetStyleRange(CGXRange().SetCols(8), CGXStyle()
			.SetHorizontalAlignment(DT_RIGHT)
			.SetValueType(GX_VT_NUMERIC)
			.SetFormat(GX_FMT_FIXED)
			.SetPlaces(4));
	SetStyleRange(CGXRange().SetCols(6), CGXStyle()
			.SetEnabled(FALSE));

	// Key, Id Column은 숨긴다.
	// 주1 참조
	SetFrozenCols(2, 2);
	HideColsByIntend(1, 2, TRUE);

	// 현재 셀의 border 설정 변경
	CGXProperties* pProp = GetParam()->GetProperties();
			pProp->SetUserProperty(GX_IDS_OUTLINECURRENTCELL,
		      (CGXStyle) pProp->sInvertNoBorder);

	// 현재 셀 설정
	SetRightValidCell(1, 0);
	GetParam()->SetHideCurrentCell(GX_HIDE_NEVER);

	// Sorting 정보 초기화
	m_aSortInfo.SetSize(2);
	m_aSortInfo[0].nRC = 1; // id
	m_aSortInfo[0].sortType = CGXSortInfo::numeric;
	m_aSortInfo[0].sortOrder = CGXSortInfo::ascending;
	m_aSortInfo[1].nRC = 0; // key
	m_aSortInfo[1].sortType = CGXSortInfo::numeric;
	m_aSortInfo[1].sortOrder = CGXSortInfo::ascending;

	// always show vertical scrollbar
	SetScrollBarMode(SB_VERT, gxnEnabled | gxnEnhanced);  

	// Row 이동을 가능하게 한다.
	GetParam()->EnableMoveRows(TRUE);

	// Column 크기 설정
	SetColWidth(1, 1, 0);
	SetColWidth(2, 2, 0);
	SetColWidth(3, 3, 45);
	SetColWidth(4, 4, 110);
	SetColWidth(5, 5, 70);
	SetColWidth(6, 6, 70);
	SetColWidth(7, 7, 60);
	SetColWidth(8, 8, 90);
	SetColWidth(9, 9, 60);

	MakeItemEx();
	LockUpdate(FALSE);

	EnableAppendRow(FALSE); // append 금지
	EnableInsertRowByPressingKey(FALSE);  // Insert Key 금지
}

BOOL CCMStageGrid::InsertRecordByKey(T_STAG_K &key, T_STAG_D &data)
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

BOOL CCMStageGrid::DeleteRecordByKey(T_STAG_K &key, T_STAG_D &data)
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

void CCMStageGrid::UpdateBuffer()
{
	CDBDoc* pDoc = m_pDoc;
	int nModified = 0;

	CViewBuff* pViewBuff = pDoc->m_pViewBuff;
	int nCount = pViewBuff->GetCount();
	if (nCount == 0) return; 
	ASSERT(nCount == 1);

	T_UDRD_BUFFER buffer_ur;
	POSITION pos = pViewBuff->GetStartBuffer();

	BOOL bOldLock = LockUpdate(TRUE);

	T_STAG_K key, keyBak; 
	T_STAG_D data, dataBak;
	BOOL bMFD = FALSE;
	while(pos != NULL)
	{
		buffer_ur = pViewBuff->GetNextBuffer(pos);

		int nCmd = buffer_ur.nCmd;
		int nKey = buffer_ur.nKey;

		switch(nCmd)
		{
		case(UR_STAG_ADD):
				// 삽입될 위치(nRow)를 찾아 넣는다.
				pViewBuff->GetStag(nKey, key, data);
				InsertRecordByKey(key, data);
				nModified |= 0x01;
				break;
		case(UR_STAG_DEL):
				// 삭제될 위치(nRow)를 찾아서 지운다.
				pViewBuff->GetStag(nKey, key, data);
				DeleteRecordByKey(key, data);
				nModified |= 0x02;
				break;
		case(UR_STAG_MFD):
				// 여기서는 저장만 하고 UR_ELEM_MFS에서 처리한다.
				pViewBuff->GetStag(nKey, key, data);
				nModified |= 0x01;
				keyBak = key;
				dataBak = data;
				bMFD = TRUE;
				break;
		case(UR_STAG_MFS):
				// MFD가 먼저오고 MFS가 온다. 항상 이 순서대로 한 짝이 되어 온다.
				ASSERT(bMFD);
				pViewBuff->GetStag(nKey, key, data);
				nModified |= 0x02;
				DeleteRecordByKey(key, data);
				InsertRecordByKey(keyBak, dataBak);
				// 플래그 수정
				bMFD = FALSE;
				break;
		case(UR_STAG_ITEM_ADD):
		case(UR_STAG_ITEM_DEL):
			break;
		default:
			break;
		}
	} // end of while
	if (nModified) ShowSelected();
	AdjustAppendRowNo();

	LockUpdate(bOldLock);
	if (!bOldLock) Redraw();
}

void CCMStageGrid::UnitChanged()
{
	BOOL bOldLock = LockUpdate(TRUE);

	// 타이틀을 수정
	T_UNIT_SYSTEM UnitSystem;
	SetHeaderTitle(UnitSystem, FALSE);
	// 각 항목의 값을 수정
	MakeItemEx(0, -2, TRUE);
	LockUpdate(bOldLock);
	if (!bOldLock) Redraw();
}


/////////////////////////////////////////////////////////////////////////////
// CCMStageGrid Overridable Functions
void CCMStageGrid::OnChangedSelectionPublic(long *keys, int nSize)
{
	if (nSize <= 0)
	{
		m_aSelectedKeys.RemoveAll();
	}
	else
	{
		m_aSelectedKeys.RemoveAll();
		for (int i = 0; i < nSize; i++) m_aSelectedKeys.Add((T_STAG_K)keys[i]);
	}
}

long CCMStageGrid::GetSelectedKeys(CArray<long, long> &caKey)
{
	// override this function to return selected record count

	long nKeyCount;
	CArray<T_STAG_K, T_STAG_K> aSelKey;

	GetAllSelectedStag(aSelKey);
	nKeyCount = aSelKey.GetSize();
	caKey.SetSize(nKeyCount);
	for (long i = 0; i < nKeyCount; i++)
		caKey[i] = (long)aSelKey[i];
	
	return nKeyCount;
}

BOOL CCMStageGrid::ValidateField(CString value, int nColID)
{
	if (nColID >= GetColCount())
	{
		ASSERT(0);
		return FALSE;
	}
	int nErrCode;
	switch(nColID)
	{
	case 0: case 1: case 2: case 4: case 5: case 6: case 8:
		if (!IsValidLongNumber(value, nErrCode)) return InvalidLongNumber(nErrCode);
		break;
	case 7:
		if (!IsValidDoubleNumber(value, nErrCode)) return InvalidDoubleNumber(nErrCode);
		break;
	case 3:
		break;
	}

	return TRUE;
}

// 삽입이나 수정전에 Record의 Validation을 검사한다.
BOOL CCMStageGrid::ValidateRecord(CStringArray &value, CRowColArray &aCols)
{
	CString msg = _T("");
	int nPos;

	for (int i = 0; i < GetColCount()-1; i++)
	{
		nPos = aCols[i]-1;
		if (value[nPos] == _T(""))
			msg += ((msg != _T("")) ? _T("\n") : _T("")) + m_aTitle[i] + _LS(IDS_WG_CMD__ADDD___has_not_been_entered_);
		else if (!ValidateField(value[nPos], i)) return FALSE;
	}
	if (msg == _T("")) return TRUE;
	else SetWarningText(msg);
	return FALSE;
}

// 이 함수는 Paste할 때 호출되므로 False를 리턴할 때는 ValidateField와 
// Validate Record에서 설정된 Warning Text를 없애주고 리턴한다.
BOOL CCMStageGrid::ValidateAndMakeRecord(CStringArray &value, CRowColArray& aCols, T_STAG_K &key, T_STAG_D &data)
{
	// 레코드에 대해 Validation을 검사한다.
	if (!ValidateRecord(value, aCols))
	{
		SetWarningText(_T(""));
		return FALSE;
	}

	// 레코드를 만들어서 리턴한다.
	int nPos;
	for (int i = 0; i < GetColCount(); i++)
	{
		nPos = aCols[i]-1; // col id i에 해당하는 value가 저장된 위치
		ConvStrToData(i, value[nPos], key, data);
	}
	return TRUE;
}

BOOL CCMStageGrid::AddToDB(ROWCOL nRow)
{
	ASSERT(0);  // Add는 없다.
	return FALSE;
}

BOOL CCMStageGrid::AddToDB(CArray<void*, void*>& raKey, CArray<void*, void*>& raData)
{
	ASSERT(0);  // Add는 없다.
	return FALSE;
}


BOOL CCMStageGrid::DeleteFromDB(ROWCOL nRow)
{
	T_STAG_K key;
	T_STAG_D data;
 
	GetValue(nRow, key, data);
	return m_pDoc->m_pDataCtrl->DelStag(key);
}

BOOL CCMStageGrid::DeleteFromDB(CRowColArray& awRows)
{
	if (awRows.GetSize() <= 0) return TRUE;

	ROWCOL nCol;
	VERIFY(GetColumnNoFor(0, nCol));

	CString value;
	CArray<T_STAG_K, T_STAG_K> awKeys;
 
	for (int i = 0; i < awRows.GetSize(); i++)
	{
		value = GetValueRowColBeforeEdit(awRows.GetAt(i), nCol);
		awKeys.Add(_tstol(value));
	}

	return m_pDoc->m_pDataCtrl->DelStag(awKeys);
}

BOOL CCMStageGrid::ModifyToDB(ROWCOL nRow)
{
	ROWCOL nCol;
	VERIFY(GetColumnNoFor(0, nCol));  // load case name

	T_STAG_K key;
	T_STAG_D data;
	T_STAG_K OldKey;;

	OldKey = _tstol(GetValueRowColBeforeEdit(nRow, nCol));
	GetValue(nRow, key, data);

	// 수정한다. 실패하면 에러 리턴
	return m_pDoc->m_pDataCtrl->ModifyStag(OldKey, data); 
}

BOOL CCMStageGrid::ModifyToDB(CStringArray& raOldKey, 
												 CArray<void*, void*>& raKey, 
												 CArray<void*, void*>& raData)
{
	ASSERT(raKey.GetSize() == raData.GetSize());
	ASSERT(raKey.GetSize() == raOldKey.GetSize());
	CArray<T_STAG_K, T_STAG_K> aOldKey;
	CArray<T_STAG_D, T_STAG_D&> aData;
	T_STAG_K Key;
	T_STAG_D Data;
	for (int i = 0; i < raKey.GetSize(); i++)
	{
		Key = _tstol(raOldKey.GetAt(i));
		aOldKey.Add(Key);
		aData.Add(*((T_STAG_D*)raData.GetAt(i)));
	}
	return m_pDoc->m_pDataCtrl->ModifyStag(aOldKey, aData);
}


CString CCMStageGrid::GetNewKey()
{
	CString value;
	T_STAG_K key = m_pDoc->m_pAttrCtrl->GetStartNumStag();
	value.Format(STAG_KEY_FMT, key);
	return value;
}

CString CCMStageGrid::GetDefValue(int nIndex)
{
	return m_aCurDefVal[nIndex];
}

CString* CCMStageGrid::GetDefValueArray()
{
	return m_aCurDefVal;
}

BOOL CCMStageGrid::MakeKeyAndDataArray(CStringArray& value, CRowColArray& awCols, 
																	CArray<void*, void*>& aKey, CArray<void*, void*>& aData)
{
	T_STAG_K* pKey;
	T_STAG_D* pData;
	pKey = new T_STAG_K;
	pData = new T_STAG_D;
	aKey.Add((void*)pKey);
	aData.Add((void*)pData);
	if (!ValidateAndMakeRecord(value, awCols, *pKey, *pData)) return FALSE;
	return TRUE;
}

void CCMStageGrid::DeleteKeyAndDataArray(CArray<void*, void*>& aKey, CArray<void*, void*>& aData)
{
	T_STAG_K* pKey;
	T_STAG_D* pData;
	ASSERT(aKey.GetSize() == aData.GetSize());
	for (int i = 0; i < aKey.GetSize(); i++)
	{
		pKey = (T_STAG_K*)aKey.GetAt(i);
		pData = (T_STAG_D*)aData.GetAt(i);
		delete pKey;
		delete pData;
	}
}

long CCMStageGrid::GetCountData()
{
	CDBDoc* pDoc = m_pDoc;
	return pDoc->m_pAttrCtrl->GetCountStag();
}

void CCMStageGrid::MakeItemFiltered(long nFilteredNum, long* aFilteredKeys, CRowColArray& awCols)
{
	CDBDoc* pDoc = m_pDoc;
	T_STAG_D Data;

	for(long nCount=0 ; nCount < nFilteredNum; nCount++)
 	{
		pDoc->m_pAttrCtrl->GetStag(aFilteredKeys[nCount], Data);
		SetValue(GetParam()->GetData(), nCount+1, aFilteredKeys[nCount], Data, awCols);
	}
}

void CCMStageGrid::MakeItemNormal(long nNumData, CRowColArray& awCols)
{
	T_STAG_K Key;
	T_STAG_D Data;
	CDBDoc* pDoc = m_pDoc;

	long* KeyBuf = new long[nNumData];
	long nCount = 0;
	POSITION pos = pDoc->m_pAttrCtrl->GetStartStag();
	while(pos != NULL)
	{
	  pDoc->m_pAttrCtrl->GetNextStag(pos,Key,Data);
	  KeyBuf[nCount++] = Key;
	}
	qsort(KeyBuf, nNumData, sizeof(T_STAG_K), CTBBrowserWnd::CompareKey);

	for(nCount=0 ; nCount < nNumData; nCount++)
	{
		pDoc->m_pAttrCtrl->GetStag(KeyBuf[nCount], Data);
		SetValue(GetParam()->GetData(), nCount+1, KeyBuf[nCount], Data, awCols);
	} // end of loop nCount

	delete []KeyBuf;
}

void CCMStageGrid::MakeItemEditingRecord(ROWCOL ncRow, CStringArray& EditingRecord)
{
	CDBDoc* pDoc = m_pDoc;
	T_STAG_K Key;
	T_STAG_D Data;
	for (int i = 0; i < GetColCount(); i++)
	{
		ConvStrToData(i, EditingRecord[i], Key, Data);
	}
	pDoc->m_pUnitCtrl->ConvertUnitStagPrevious(Data);
	// 현재 값을 설정한다.
	SetValue(ncRow, Key, Data);
}

/////////////////////////////////////////////////////////////////////////////
// CCMStageGrid Implementation
BOOL CCMStageGrid::GetValue(ROWCOL nRow, T_STAG_K &key, T_STAG_D &data)
{
	ROWCOL nCol;
	CString str;

	GetColumnNoFor(GetKeyColID(), nCol);
	T_STAG_K OrgKey = _ttoi(GetValueRowColBeforeEdit(nRow, nCol));
	m_pDoc->m_pAttrCtrl->GetStag(OrgKey, data);
	for (int i = 0; i < GetColCount(); i++)
	{
		VERIFY(GetColumnNoFor(m_aTitle[i], nCol));
		str = GetValueRowCol(nRow, nCol);
		// 수식인 경우 값으로 변환한다.
		// Validation이 OK인 Field이므로 항상 TRUE 리턴
		VERIFY(ConvertFormulaToValue(str)); 

		ConvStrToData(i, str, key, data);
	}
	return TRUE;
}

BOOL CCMStageGrid::SetValue(ROWCOL nRow, T_STAG_K key, T_STAG_D &data)
{
	CRowColArray awCols;
	awCols.SetSize(GetColCount());
	for (int i = 0;i < GetColCount(); i++)
		VERIFY(GetColumnNoFor(m_aTitle[i], awCols[i]));
	return SetValue(GetParam()->GetData(), nRow, key, data, awCols);
}

BOOL CCMStageGrid::SetValue(CGXData* pData, ROWCOL nRow, T_STAG_K key, T_STAG_D &data, CRowColArray &awCols)
{
	int nValueType;
	CString str;
	for (int i = 0; i < GetColCount(); i++)
	{
		ConvDataToStr(i, key, data, str);
		if (m_aType[i] == t_string) nValueType = GX_VT_STRING;
		else nValueType = GX_VT_NUMERIC;
		pData->StoreValueRowCol(nRow, awCols[i], str, gxOverride, nValueType);
	}

	return TRUE;
}

void CCMStageGrid::ConvStrToData(int i, CString& value, T_STAG_K& rKey, T_STAG_D& rData)
{
	if (i==0) 
	{
		if (value == _T("")) rKey = m_pDoc->m_pAttrCtrl->GetStartNumStag();
		else rKey = _tstol(value);
	}
	else if (i==1) rData.nStageId = _tstol(value);
	else if (i==2) i=2; // Final (데이타 의미 없음)
	else if (i==3) rData.StageName = value;
	else if (i==4) rData.nDuration = _tstol(value);
	else if (i==5) i=5; // Date (데이타 의미 없음)
	else if (i==6) rData.nTimeStepNum = _tstol(value);
	else if (i==7) rData.dLaunchDist = _tstof(value);
	else if (i==8) rData.nLaunchStep = _tstol(value);
}

void CCMStageGrid::ConvDataToStr(int i, T_STAG_K& rKey, T_STAG_D& rData, CString& value)
{
	if(i==0)
	{
		if (rKey == 0) value = _T("");
		else value.Format(_T("%d"), rKey);
	}
	else if(i==1) value.Format(_T("%d"), rData.nStageId);
	else if(i==2) value = _T("0");  // Final
	else if(i==3) value = rData.StageName;
	else if(i==4) value.Format(_T("%d"), rData.nDuration);
	else if(i==5) value = _T("0");  // Date
	else if(i==6) value.Format(_T("%d"), rData.nTimeStepNum);
	else if(i==7) value.Format(STAG_DATA_FMT, rData.dLaunchDist);
	else if(i==8) value.Format(_T("%d"), rData.nLaunchStep);
}

void CCMStageGrid::MakeSearchKey(T_STAG_K &key, T_STAG_D &data, CStringArray &aKey)
{
	int nColID;
	int nSortKeyNum = m_aSortInfo.GetSize();

	aKey.SetSize(nSortKeyNum);
	for (int i = 0; i < nSortKeyNum; i++)
	{
		nColID = m_aSortInfo[i].nRC;
		ConvDataToStr(nColID, key, data, aKey[i]);
	}
}

void CCMStageGrid::GetAllSelectedStag(CArray<T_STAG_K, T_STAG_K>& aSelKey)
{
	for (int i = 0; i < m_aSelectedKeys.GetSize(); i++)
		aSelKey.Add(m_aSelectedKeys[i]);
}

void CCMStageGrid::SetHeaderTitle(T_UNIT_SYSTEM& UnitSystem, BOOL bInit)
{
	SetHeaderTitle2((int*)m_aUnit.GetData(), bInit, FALSE);
}

// Hide 된 Row들이 resize되지 않도록 한다.
// 주1 참조
BOOL CCMStageGrid::OnTrackColWidth(ROWCOL nCol)
{
	if (nCol == 1) return FALSE;
	return CTBCommon::OnTrackColWidth(nCol);
}

// TBBrowserWndEx::OnSelDragColsDrop 참조
// 부모를 먼저 호출하면 안 된다.
// 1, 2 모두 Frozen Cols로 설정되어 있으므로 이쪽으로 셀이 이동되거나 이 셀들이
// 이동되는 것을 금지한다.
// 주1 참조
BOOL CCMStageGrid::OnSelDragColsDrop(ROWCOL nFirstCol, ROWCOL nLastCol, ROWCOL nDestCol)
{
	// Frozen 되어 있는 column 위치에 이동되면 이동을 금지시킨다.
	//if (IsFrozenCol(nDestCol)) return FALSE;
	if (nDestCol == 1 || nFirstCol == 1) return FALSE;
	if (nDestCol == 2 || nFirstCol == 2) return FALSE;
	return CTBCommon::OnSelDragColsDrop(nFirstCol, nLastCol, nDestCol);
}

// Stage의 위치를 이동(StageId가 변경)한다.
BOOL CCMStageGrid::OnSelDragRowsDrop(ROWCOL nFirstRow, ROWCOL nLastRow, ROWCOL nDestRow)
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

	CArray<T_STAG_K, T_STAG_K> aKey;
	CArray<T_STAG_D, T_STAG_D&> aData;
	T_STAG_K key;
	T_STAG_D data;
	for (int i = nFirstRow; i <= nLastRow; i++)
	{
		GetValue(i, key, data);
		data.nStageId = nStart;
		nStart += nInc;

		aKey.Add(key);
		aData.Add(data);
	}
	m_pDoc->m_pDataCtrl->ModifyStag(aKey, aData);
	return FALSE;
}

void CCMStageGrid::OnLButtonDown(UINT nFlags, CPoint point)
{
	CTBCommon::OnLButtonDown(nFlags, point);

	CheckCurrentRecordChanged();
}

//----------------------------------------------------------------------------->
// 여기서 부터 Current Record의 위치 변경을 Detect하기 위해 삽입된 코드이다.
// Current Record가 변경되었는지 검사
BOOL CCMStageGrid::CheckCurrentRecordChanged()
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

BOOL CCMStageGrid::ProcessKeys(CWnd *pSender, UINT nMessage, UINT nChar, UINT nRepCnt, UINT flags)
{
	BOOL bResult = CTBCommon::ProcessKeys(pSender, nMessage, nChar, nRepCnt, flags);
	CheckCurrentRecordChanged();
	return bResult;
}

void CCMStageGrid::OnRButtonDblClk(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	ROWCOL ncRowBefore, ncRowAfter, ncCol;
	if (!GetCurrentCell(ncRowBefore, ncCol)) return;

	CTBCommon::OnRButtonDblClk(nFlags, point);

	if (!GetCurrentCell(ncRowAfter, ncCol)) return;
	if (ncRowBefore != ncRowAfter) OnChangeCurrentRecord(ncRowAfter);
}

void CCMStageGrid::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CTBCommon::OnLButtonDblClk(nFlags, point);
	CheckCurrentRecordChanged();
}

void CCMStageGrid::OnChangeCurrentRecord(ROWCOL nRow)
{
}
//<-----------------------------------------------------------------------------

BOOL CCMStageGrid::GetCurrentStag(T_STAG_K& nKey)
{
	ROWCOL ncRow, ncCol;
	if (!GetCurrentCell(ncRow, ncCol)) return FALSE;
	if (ncRow < 1) return FALSE;
	if (IsAppendRow(ncRow)) return FALSE;

	if (!GetColumnNoFor(GetKeyColID(), ncCol)) return FALSE;
	nKey = _ttoi(GetValueRowCol(ncRow, ncCol));
	return TRUE;
}

BOOL CCMStageGrid::SetCurrentStag(T_STAG_K key, T_STAG_D& data)
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

void CCMStageGrid::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
	switch(lHint)
	{
	case D_UPDATE_DBALL:
	case D_UPDATE_DEFAULT:
		UpdateBuffer();
		break;
	case D_UPDATE_BUFFER_BEFORE:
		break;
	case D_UPDATE_BUFFER_AFTER:
		UpdateBuffer();
		break;
	case D_UPDATE_UNIT:
		UnitChanged();
		break;
	}
}

void CCMStageGrid::AdjustAppendRowNo(ROWCOL nCol)
{
	CTBCommon::AdjustAppendRowNo(nCol);

	// Date를 설정해 준다.
	ROWCOL nDateCol;
	GetColumnNoFor(5, nDateCol);
	SetStageDate(nDateCol);

	// Add,Del,Modified시 ID 변경이 있으므로 Update
	ROWCOL nIdCol;
	GetColumnNoFor(1, nIdCol);
	SetId(nIdCol);
}

// 테이블 전체의 Date를 계산해서 설정
void CCMStageGrid::SetStageDate(ROWCOL nDateCol)
{
	T_STAG_K key;
	T_STAG_D data;
	double dblDate, dblPrevDate;
	int i = 1;
	ROWCOL nRowCount = GetRowCount();
	if (nRowCount <= 1) return; // if no story exist then return

	//ROWCOL nDurationCol;
	//GetColumnNoFor(4, nDurationCol);
	ROWCOL ncRow, ncCol;
	BOOL bGet = GetCurrentCell(ncRow, ncCol);

	GetValue(i, key, data);
	dblPrevDate = data.nDuration;
	if (nRowCount > 1)
		SetValueRange(CGXRange(i, nDateCol), long(data.nDuration)); 
	for (i = 2; i < nRowCount; i++) // Append Row 제외
	{
		if (m_nEditMode == addnew && bGet && ncRow == i) continue;  // 편집 상태인 row는 제외
		GetValue(i, key, data);
		dblDate = dblPrevDate + data.nDuration;
		SetValueRange(CGXRange(i, nDateCol), dblDate);
		dblPrevDate = dblDate;
	}
}

void CCMStageGrid::SetId(ROWCOL nIdCol)
{
	ROWCOL nKeyCol;
	GetColumnNoFor(GetKeyColID(), nKeyCol);
	T_STAG_K key;
	T_STAG_D data;
	ROWCOL nRowCount = GetRowCount();
	for (int i = 1; i < nRowCount; i++)
	{
		key = _ttoi(GetValueRowCol(i, nKeyCol));
		m_pDoc->m_pAttrCtrl->GetStag(key, data);
		SetValueRange(CGXRange(i, nIdCol), (long)data.nStageId);
	}
}

////////////////////////////////////////////////////////////////////////////
// 여기부터 Column 값 변경이 다른 column의 상태나 값을 변경시키는 경우를
// 처리해 주기 위해서 Override 및 새로 정의되는 함수이다.
// Level 변경시 Height 변경, Height 변경시 Level 변경
void CCMStageGrid::OnModifyCell(ROWCOL nRow, ROWCOL nCol)
{
	// 부모의 OnModifyCell을 항상 먼저 호출해 주어야 한다.
	CTBCommon::OnModifyCell(nRow, nCol);
	SaveCurrentRecordValue(nRow, nCol);
	ChangeColumnStatus(nRow, nCol);
}

void CCMStageGrid::OnCanceledEditing(ROWCOL nRow, ROWCOL nCol)
{
	// 부모의 OnCanceledEditing을 항상 먼저 호출해 주어야 한다.
	CTBCommon::OnCanceledEditing(nRow, nCol);
	if (!IsEdit()) return;
	RestorePrevRecordValue(nRow, nCol);
}

BOOL CCMStageGrid::OnEndEditing(ROWCOL nRow, ROWCOL nCol)
{
	// 부모의 OnEndEditing을 호출한다.
	if (!CTBCommon::OnEndEditing(nRow, nCol)) return FALSE;
	if (!m_bBeforeSaved) return TRUE;

	// 값이 저장되었다는 플래그를 reset한다.
	m_bBeforeSaved = FALSE;
	return TRUE;
}

BOOL CCMStageGrid::OnCanceledModify()
{
	CTBCommon::OnCanceledModify();
	// Date를 재설정한다.
	ROWCOL nRow, nCol;
	GetCurrentCell(nRow, nCol);
 
	ROWCOL nDurationCol, nDateCol;
	GetColumnNoFor(4, nDurationCol);
	if (nCol != nDurationCol) return TRUE;

	// recalculate stage date and set again
	GetColumnNoFor(5, nDateCol);
	SetStageDate(nDateCol);

	return TRUE;
}

void CCMStageGrid::ChangeColumnStatus(ROWCOL nRow, ROWCOL nCol)
{
	ROWCOL nDurationCol, nDateCol;
	GetColumnNoFor(4, nDurationCol);
	GetColumnNoFor(5, nDateCol);

	if (nCol != nDurationCol) return;

	SetStageDate(nDateCol);
}

// 변경된 후의 값이 저장된다.
void CCMStageGrid::SaveCurrentRecordValue(ROWCOL nRow, ROWCOL nCol)
{
	if (m_bBeforeSaved) return; // 이전에 저장되었으면 리턴한다.

	ROWCOL nDurationCol;
	GetColumnNoFor(4, nDurationCol);
	if (nCol != nDurationCol) return;

	m_bBeforeSaved = TRUE;
}

// 변경된 후의 값이 저장되므로 변경 이전의 값을 구하려면
// GetValueRowCol을 이용해야 한다. 단 이함수가 OnCanceledEditing()에서
// 호출되는 경우에 한해서이다.
void CCMStageGrid::RestorePrevRecordValue(ROWCOL nRow, ROWCOL nCol)
{
	if (!m_bBeforeSaved) return;

	// Column ID를 구한다.
	int nColID;
	ROWCOL nDateCol;
	GetColumnIDFor(nCol, nColID);
	GetColumnNoFor(5, nDateCol);

	if (nColID == 4) // nDurationCol
		SetStageDate(nDateCol);

	// 플래그를 Reset시킨다.
	m_bBeforeSaved = FALSE;
}

*/
