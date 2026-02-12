// CMGridLdldnGrid.cpp : implementation file
//
// 2004. 04. 01   by TAE
/////////////////////////////////////////////////////////////////////////////
//  NO  |  (KEY) | X  |  Y  |  Z

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMGirdLdlnGrid.h"

#include "..\wg_base\wg_base_CompFunc.h"
#include "..\wg_db\wg_db_UnitCtrl.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define EPSILON 1.e-12
#define DATA_FMT _T("%.15g")

/////////////////////////////////////////////////////////////////////////////
// CCMGridLdlnGrid
CString CCMGridLdlnGrid::m_aCurDefVal[] = {_T("0.0"), _T("0.0"), _T("0.0")};
CCMGridLdlnGrid::CCMGridLdlnGrid()
{
	m_pData = 0;

	AddColInfo(_T("Key"),   t_integer, 0);
	AddColInfo(_T("X"),     t_real,    CUnitCtrl::m_LDLN_UNIT.xyz);
	AddColInfo(_T("Y"),     t_real,    CUnitCtrl::m_LDLN_UNIT.xyz);
	AddColInfo(_T("Z"),     t_real,    CUnitCtrl::m_LDLN_UNIT.xyz);

	m_aPrimaryKey.Add(0);
}

CCMGridLdlnGrid::~CCMGridLdlnGrid()
{
}


BEGIN_MESSAGE_MAP(CCMGridLdlnGrid, CTBCommon)
	//{{AFX_MSG_MAP(CCMGridLdlnGrid)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CCMGridLdlnGrid message handlers

void CCMGridLdlnGrid::Initialize(T_LDLN_DT* pData)
{
	ASSERT(pData);
	m_pData = pData;

	T_UNIT_SYSTEM us;
	CDBDoc *pDoc = CDBDoc::GetDocPoint();
	CTBCommon::Initialize(pDoc, us);

	GetParam()->EnableUndo(FALSE);
	//EnableScrollTips();
	//EnableHints();
	GetParam()->EnableTrackRowHeight(FALSE);  
	GetParam()->EnableMoveCols(FALSE);        // columns can not be dragged

	CGXProperties* pProp = GetParam()->GetProperties();
	pProp->SetMarkColHeader(FALSE); // Turn off pressed button effect for column headers
	pProp->SetMarkRowHeader(FALSE);
	pProp->SetUserProperty(GX_IDS_OUTLINECURRENTCELL, // 현재 셀의 border 설정 변경
												(CGXStyle) pProp->sInvertNoBorder);

	LockUpdate(TRUE);

	// 컬럼 갯수 설정
	int m_nColCount = GetInitialColCount();
	SetColCount(m_nColCount);
	SetHeaderTitle(us, TRUE);

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
			.SetValueType(GX_VT_NUMERIC)
			.SetFormat(GX_FMT_FIXED)
			.SetPlaces(4));
	SetRowHeightByDPI(0, 0, 36);

	SetStyleRange(CGXRange().SetCols(1), CGXStyle()
			.SetPlaces(0)
			.SetEnabled(FALSE));
	HideColsByIntend(1, 1, TRUE);

	SetScrollBarMode(SB_VERT, gxnEnabled | gxnEnhanced);

	// 현재 셀 설정
	SetRightValidCell(1, 0);
	GetParam()->SetHideCurrentCell(GX_HIDE_NEVER);
	EnableInsertRowByPressingKey(FALSE);  // Disable inserting row by key

	// Sorting 정보 초기화
	m_aSortInfo.SetSize(1);
	m_aSortInfo[0].nRC = 0;
	m_aSortInfo[0].sortType = CGXSortInfo::numeric;
	m_aSortInfo[0].sortOrder = CGXSortInfo::ascending;

	LockUpdate(FALSE);
}

BOOL CCMGridLdlnGrid::InsertRecordByKey(const int &key, const T_LDLN_BASE &data)
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

BOOL CCMGridLdlnGrid::DeleteRecordByKey(const int &key, const T_LDLN_BASE &data)
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
// CCMGridLdlnGrid Overridable Functions
void CCMGridLdlnGrid::OnChangedSelectionPublic(long *keys, int nSize)
{
	return;
}

long CCMGridLdlnGrid::GetSelectedKeys(CArray<long, long> &caKey)
{
	return 0;
}

BOOL CCMGridLdlnGrid::ValidateField(CString value, int nColID)
{
	if (nColID >= GetColCount())
	{
		SetWarningText(_T("<CCMGridLdlnGrid::ValidateField> Error: Out of range column id!"));
		return FALSE;
	}

	int nErrCode;
	if (nColID == 0) 
	{
		return TRUE;
		//if (!IsValidLongNumber(value, nErrCode)) return InvalidLongNumber(nErrCode);
	}
	else if (nColID == 1 || nColID == 2 || nColID == 3)
	{
		if (!IsValidDoubleNumber(value, nErrCode)) return InvalidDoubleNumber(nErrCode);
	}
	else return FALSE;

	return TRUE;
}

// 삽입이나 수정전에 Record의 Validation을 검사한다.
BOOL CCMGridLdlnGrid::ValidateRecord(CStringArray &value, CRowColArray &aCols)
{
	CString msg = _T("");
	int nPos;

	for (int i = 0; i < GetColCount(); i++)
	{
		if (i == 0) continue;
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
BOOL CCMGridLdlnGrid::ValidateAndMakeRecord(CStringArray &value, CRowColArray& aCols, int &key, T_LDLN_BASE &data)
{
	// 레코드에 대해 Validation을 검사한다.
	if (!ValidateRecord(value, aCols))
		return FALSE;

	// 레코드를 만들어서 리턴한다.
	int nPos;
	for (int i = 0; i < GetColCount(); i++)
	{
		nPos = aCols[i]-1; // col id i에 해당하는 value가 저장된 위치
		if(i==0) key = _tstof(value[nPos]);
		else ConvStrToData(i, value[nPos], data);
	}
	return TRUE;
}

BOOL CCMGridLdlnGrid::AddToDB(ROWCOL nRow)
{
	// Record를 만들어서 DB에 입력한다.
	int key;
	T_LDLN_BASE data;
 
	GetValue(nRow, key, data);

	return AddData(key, data);
}

BOOL CCMGridLdlnGrid::AddToDB(CArray<void*, void*>& raKey, CArray<void*, void*>& raData)
{
	ASSERT(raKey.GetSize() == raData.GetSize());
	int tKey;
	CArray<int, int> aKey;
	CArray<T_LDLN_BASE, T_LDLN_BASE&> aData;
	for (int i = 0; i < raKey.GetSize(); i++)
	{
		tKey = *((int*)raKey.GetAt(i));
		aKey.Add(tKey);
		aData.Add(*((T_LDLN_BASE*)raData.GetAt(i)));
	}
	return AddData(aKey, aData);
}


BOOL CCMGridLdlnGrid::DeleteFromDB(ROWCOL nRow)
{
	int key;
	T_LDLN_BASE data;
 
	GetValue(nRow, key, data);
	return DelData(key);
}

BOOL CCMGridLdlnGrid::DeleteFromDB(CRowColArray& awRows)
{
	if (awRows.GetSize() <= 0) return TRUE;

	ROWCOL nCol;
	VERIFY(GetColumnNoFor(0, nCol));

	CString value;
	CArray<int, int> awKeys;
 
	for (int i = 0; i < awRows.GetSize(); i++)
	{
		value = GetValueRowColBeforeEdit(awRows.GetAt(i), nCol);
		awKeys.Add(_tstof(value));
	}

	return DelData(awKeys);
}

BOOL CCMGridLdlnGrid::ModifyToDB(ROWCOL nRow)
{
	ROWCOL nCol;
	VERIFY(GetColumnNoFor(0, nCol));

	int key, oldKey;
	T_LDLN_BASE data;
	
	oldKey = _tstof(GetValueRowColBeforeEdit(nRow, nCol));
	GetValue(nRow, key, data);

	// 수정한다. 실패하면 에러 리턴
	return ModifyData(oldKey, key, data); 
}

BOOL CCMGridLdlnGrid::ModifyToDB(CStringArray& raOldKey, 
												 CArray<void*, void*>& raKey, 
												 CArray<void*, void*>& raData)
{
	ASSERT(raKey.GetSize() == raData.GetSize());
	ASSERT(raKey.GetSize() == raOldKey.GetSize());
	CArray<int, int> aOldKey, aKey;
	CArray<T_LDLN_BASE, T_LDLN_BASE&> aData;
	int Key;
	int nSize = raKey.GetSize();
	for (int i = 0; i < nSize; i++)
	{
		Key = _tstof(raOldKey.GetAt(i));
		aOldKey.Add(Key);
		aKey.Add(*((int*)raKey.GetAt(i)));
		aData.Add(*((T_LDLN_BASE*)raData.GetAt(i)));
	}
	return ModifyData(aOldKey, aKey, aData);
}


CString CCMGridLdlnGrid::GetNewKey()
{
	CString value(_T(""));
	value.Format(_T("%d"), m_pData->GetSize()+1);
	return value;
}

CString CCMGridLdlnGrid::GetDefValue(int nIndex)
{
	return m_aCurDefVal[nIndex];
}

CString* CCMGridLdlnGrid::GetDefValueArray()
{
	return m_aCurDefVal;
}

BOOL CCMGridLdlnGrid::MakeKeyAndDataArray(CStringArray& value, CRowColArray& awCols, 
																	CArray<void*, void*>& aKey, CArray<void*, void*>& aData)
{
	int* pKey;
	T_LDLN_BASE* pData;
	pKey = new int;
	pData = new T_LDLN_BASE;
	aKey.Add((int*)pKey);
	aData.Add((T_LDLN_BASE*)pData);
	if (!ValidateAndMakeRecord(value, awCols, *pKey, *pData)) return FALSE;
	return TRUE;
}

void CCMGridLdlnGrid::DeleteKeyAndDataArray(CArray<void*, void*>& aKey, CArray<void*, void*>& aData)
{
	int* pKey;
	T_LDLN_BASE* pData;
	ASSERT(aKey.GetSize() == aData.GetSize());
	for (int i = 0; i < aKey.GetSize(); i++)
	{
		pKey = (int*)aKey.GetAt(i);
		pData = (T_LDLN_BASE*)aData.GetAt(i);
		delete pKey;
		delete pData;
	}
}

long CCMGridLdlnGrid::GetCountData()
{
	return m_pData->GetSize();
}

void CCMGridLdlnGrid::MakeItemFiltered(long nFilteredNum, long* aFilteredKeys, CRowColArray& awCols)
{
	ASSERT(0); // no filtering
}

void CCMGridLdlnGrid::MakeItemNormal(long nNumData, CRowColArray& awCols)
{
	int Key;
	T_LDLN_BASE Data;

	long nCount;
	for(nCount=0 ; nCount < nNumData; nCount++)
	{
		Key = nCount+1;
		Data = m_pData->GetAt(nCount);

		SetValue(GetParam()->GetData(), nCount+1, Key, Data, awCols);
	} // end of loop nCount
}

void CCMGridLdlnGrid::MakeItemEditingRecord(ROWCOL ncRow, CStringArray& EditingRecord)
{
	CDBDoc* pDoc = m_pDoc;
	int Key;
	T_LDLN_BASE Data;
	for (int i = 0; i < GetColCount(); i++)
	{
		if (i == 0) Key = _tstof(EditingRecord[i]);
		else ConvStrToData(i, EditingRecord[i], Data);
	}
	// 현재 값을 설정한다.
	SetValue(ncRow, Key, Data);
}

/////////////////////////////////////////////////////////////////////////////
// CCMGridLdlnGrid Implementation
BOOL CCMGridLdlnGrid::GetValue(ROWCOL nRow, int &key, T_LDLN_BASE &data)
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

		if(i==0) key = _tstof(str);
		else ConvStrToData(i, str, data);
	}
	return TRUE;
}

BOOL CCMGridLdlnGrid::SetValue(ROWCOL nRow, const int &key, const T_LDLN_BASE &data)
{
	//
	CRowColArray awCols;
	awCols.SetSize(GetColCount());
	for (int i = 0;i < GetColCount(); i++)
		VERIFY(GetColumnNoFor(m_aTitle[i], awCols[i]));
	return SetValue(GetParam()->GetData(), nRow, key, data, awCols);
}

BOOL CCMGridLdlnGrid::SetValue(CGXData* pData, ROWCOL nRow, const int &key, const T_LDLN_BASE &data, CRowColArray &awCols)
{
	//
	CString str;
	for (int i = 0; i < GetColCount(); i++)
	{
		if(i==0) 
		{
			if (key == 0) str = GetNewKey();
			else str.Format(_T("%d"), key);
		}
		else ConvDataToStr(i, data, str);
		pData->StoreValueRowCol(nRow, awCols[i], str, gxOverride, GX_VT_NUMERIC);
	}

	return TRUE;
}

void CCMGridLdlnGrid::ConvStrToData(int i, CString& value, T_LDLN_BASE& data)
{
	if (i == 1) data.x = _tstof(value);
	else if (i == 2) data.y = _tstof(value);
	else if (i == 3) data.z = _tstof(value);
	else ASSERT(0);
}

void CCMGridLdlnGrid::ConvDataToStr(int i, const T_LDLN_BASE& data, CString& value)
{
	if (i == 1) value.Format(DATA_FMT, data.x);
	else if (i == 2) value.Format(DATA_FMT, data.y);
	else if (i == 3) value.Format(DATA_FMT, data.z);
	else ASSERT(0);
}

void CCMGridLdlnGrid::MakeSearchKey(const int &key, const T_LDLN_BASE &data, CStringArray &aKey)
{
	int nColID;
	int nSortKeyNum = m_aSortInfo.GetSize();

	aKey.SetSize(nSortKeyNum);
	for (int i = 0; i < nSortKeyNum; i++)
	{
		nColID = m_aSortInfo[i].nRC;
		if (nColID == 0)
		{
			if (key == 0) aKey[i] = GetNewKey();
			aKey[i].Format(_T("%d"), key);
		}
		else ConvDataToStr(nColID, data, aKey[i]);
	}
}

void CCMGridLdlnGrid::SetHeaderTitle(T_UNIT_SYSTEM &us, BOOL bInit)
{
	CString csUnit, csTitle;
	int aSize[] = {0, 58, 58, 58};
	int nColCount = GetColCount();

	for (int i = 0; i < nColCount; i++)
	{
		if (i==0) csUnit.Empty();
		else CUnitCtrl::GetUnitSystem(CUnitCtrl::m_LDLN_UNIT.xyz, csUnit);
		if (csUnit.IsEmpty()) csTitle = m_aTitle[i];
		else csTitle = m_aTitle[i] + _T("\n(")+csUnit+_T(")");
		SetStyleRange(CGXRange(0, i+1), CGXStyle()
			.SetValue(csTitle)
			.SetUserAttribute(ID_USER_ATTR_COL_ID, m_aTitle[i]));
		if (bInit) SetColWidthByDPI(i+1, i+1, aSize[i]);
	}
	if (bInit) SetColWidthByDPI(0, 0, 35);
}

BOOL CCMGridLdlnGrid::AddData(int key, T_LDLN_BASE& data)
{
	if (key == 0) key = m_pData->GetSize()+1;
	
	CArray<int, int> aOldKey, aKey;
	CArray<T_LDLN_BASE, T_LDLN_BASE&> aData;
	aOldKey.Add(key);
	aKey.Add(key);
	aData.Add(data);

	m_pData->InsertAt(key-1, data);
	UpdateBuffer(0, aOldKey, aKey, aData);

	return TRUE;
}

BOOL CCMGridLdlnGrid::AddData(CArray<int, int> &aKey, CArray<T_LDLN_BASE, T_LDLN_BASE&> &aData, BOOL bNotify)
{
	int nSize = aKey.GetSize();
	int KeyIndex = m_pData->GetSize()+1;
	for (int i=0; i<nSize; i++)
	{
		if (aKey[i] != 0) continue;
		aKey[i] = KeyIndex;
		KeyIndex++;
	}
	CArray<int, int> aOldKey;
	aOldKey.Copy(aKey);

	for (int i = 0; i < nSize; i++)
		m_pData->InsertAt(aKey[i]-1, aData[i]);
	UpdateBuffer(0, aOldKey, aKey, aData);

	return TRUE;
}

BOOL CCMGridLdlnGrid::DelData(int key)
{
	if (key <= 0 || key > m_pData->GetSize())
	{ ASSERT(0); return FALSE; }

	CArray<int, int> aOldKey, aKey;
	CArray<T_LDLN_BASE, T_LDLN_BASE&> aData;
	aKey.Add(key);
	aOldKey.Add(key);
	T_LDLN_BASE data;
	data.Initialize();
	aData.Add(data);

	m_pData->RemoveAt(key-1);
	UpdateBuffer(1, aOldKey, aKey, aData);

	return TRUE;
}

BOOL CCMGridLdlnGrid::DelData(CArray<int, int> &aKey, BOOL bNotify)
{
	CArray<int, int> aOldKey;
	CArray<T_LDLN_BASE, T_LDLN_BASE&> aData;
	aOldKey.Copy(aKey);
	T_LDLN_BASE data; 
	data.Initialize();
	int nSize = aKey.GetSize();
	for (int i = 0; i < nSize; i++)
	{
		if (aKey[i] <= 0 || aKey[i] > m_pData->GetSize())
		{ ASSERT(0); return FALSE; }
		aData.Add(data);
	}
	
	for (int i = nSize-1; i >= 0; i--)
		m_pData->RemoveAt(aKey[i]-1);
	UpdateBuffer(1, aOldKey, aKey, aData);

	return TRUE;
}

BOOL CCMGridLdlnGrid::ModifyData(int oldKey, int key, T_LDLN_BASE &data)
{
	if (key <= 0 || key > m_pData->GetSize())
	{ ASSERT(0); return FALSE; }

	CArray<int, int> aOldKey, aKey;
	CArray<T_LDLN_BASE, T_LDLN_BASE&> aData;
	aOldKey.Add(oldKey);
	aKey.Add(key);
	aData.Add(data);
 
	ASSERT(oldKey == key);
	m_pData->SetAt(key-1, data);
	UpdateBuffer(2, aOldKey, aKey, aData);

	return TRUE;
}

BOOL CCMGridLdlnGrid::ModifyData(CArray<int, int> &aOldKey, CArray<int, int> &aKey, CArray<T_LDLN_BASE, T_LDLN_BASE&> &aData)
{
	CArray<T_LDLN_BASE, T_LDLN_BASE&> aOrgData;
	aOrgData.Copy(*m_pData);

	if (!DelData(aOldKey, FALSE))
		return FALSE;
	if (!AddData(aKey, aData, FALSE))
	{
		m_pData->Copy(aOrgData);
		return FALSE;
	}
	return TRUE;
}

void CCMGridLdlnGrid::UpdateBuffer(int nCmd, CArray<int, int> &aOldKey, CArray<int, int> &aKey, CArray<T_LDLN_BASE, T_LDLN_BASE&> &aData)
{
	MakeItemEx();
	/*
	// 현재 Row, Col을 저장 : 나중에 복구하기 위함
	ROWCOL ncRow, ncCol;
	GetCurrentCell(ncRow, ncCol);

	BOOL bOldLock = LockUpdate(TRUE);

	int nSize = aKey.GetSize();
	for (int i = 0; i < nSize; i++)
	{
		case(0):
				// 삽입될 위치(nRow)를 찾아 넣는다.
				InsertRecordByKey(aKey[i], aData[i]);
				break;
		case(1):
				// 삭제될 위치(nRow)를 찾아서 지운다.
				DeleteRecordByKey(aKey[i], aData[i]);
				break;
		case(2):
				DeleteRecordByKey(aOldKey[i], aData[i]);
				InsertRecordByKey(aKey[i], aData[i]);
				break;
		default:
			break;
		}
	} // end of while

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
	*/
}
