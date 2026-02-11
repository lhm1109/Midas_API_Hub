// CMPermitLoadGrid.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMPermitLoadGrid.h"

#include "..\wg_base\CompFunc.h"
#include "..\wg_db\UnitCtrl.h"

//#include "SpfcItemDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define D_MAX_WHEEL_LINE  10
#define DATA_FMT _T("%.15g")

/////////////////////////////////////////////////////////////////////////////
// CCMPermitLoadGrid
CString CCMPermitLoadGrid::m_aCurDefVal[] = 
{
	_T(""), _T(""), _T("0.0"),
	_T(""), _T(""), _T(""), _T(""), _T(""), _T(""), _T(""), _T(""), _T(""), _T(""), // P & D
	_T(""), _T(""), _T(""), _T(""), _T(""), _T(""), _T(""), _T(""), _T(""), _T(""), // P & D
};

CCMPermitLoadGrid::CCMPermitLoadGrid()
{
	m_pData = 0;
	m_bCheckVS = FALSE;

	AddColInfo(_LSX(No),                    t_integer, 0);
	AddColInfo(_LS(IDS_CMD_MVHL_TYPE_OF_AXLE),        t_string, 0);
	AddColInfo(_T("VS"),                                 t_integer, 0);
	AddColInfo(_LS(IDS_CMD_MVHL_SPACING),             t_real,    D_UNITSYS_BASE_LENGTH);
	CString csName;
	for (int i = 0; i < D_MAX_WHEEL_LINE; i++)
	{
		csName.Format(_T("P%d"), i+1);
		AddColInfo(csName, t_real, D_UNITSYS_BASE_FORCE);
		csName.Format(_T("D%d"), i+1);
		AddColInfo(csName, t_real, D_UNITSYS_BASE_LENGTH);
	}

	m_aPrimaryKey.Add(0);
}

CCMPermitLoadGrid::~CCMPermitLoadGrid()
{
}


BEGIN_MESSAGE_MAP(CCMPermitLoadGrid, CTBCommon)
	//{{AFX_MSG_MAP(CCMPermitLoadGrid)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CCMPermitLoadGrid message handlers

void CCMPermitLoadGrid::Initialize(T_MVHL_D *pData)
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
			.SetPlaces(2));
	SetRowHeightByDPI(0, 0, 36);

	SetStyleRange(CGXRange().SetCols(1, 1), CGXStyle().SetPlaces(0));
	HideColsByIntend(1,1, TRUE);
	SetTypeColumn(2);

	// VS CheckBox
	SetStyleRange(CGXRange().SetCols(3), CGXStyle()
		.SetControl(GX_IDS_CTRL_CHECKBOX3D)
		.SetValueType(GX_VT_NUMERIC)
		.SetHorizontalAlignment(DT_CENTER)
		.SetVerticalAlignment(DT_VCENTER)
		.SetValue(_T("0")));

	// Spacing 비활성화
	// Insert될 때 첫번째가 아니면 활성화한다. - 이곳과 SetValue 부분 - 20080520 - jhyun 
	SetStyleRange(CGXRange().SetCols(3, 3), CGXStyle()
		.SetEnabled(FALSE));
 
	SetStyleRange(CGXRange().SetCols(5, m_nColCount), CGXStyle()
		.SetEnabled(FALSE));

	// 현재 셀 설정
	SetRightValidCell(1, 0);
	GetParam()->SetHideCurrentCell(GX_HIDE_NEVER);

	// Sorting 정보 초기화
	m_aSortInfo.SetSize(1);
	m_aSortInfo[0].nRC = 0;
	m_aSortInfo[0].sortType = CGXSortInfo::numeric;
	m_aSortInfo[0].sortOrder = CGXSortInfo::ascending;

	// Table Name 및 그리드 간격 등 설정
	//m_csTableName = D_TB_TABLE_NODE;
	//ApplyPreference();

	LockUpdate(FALSE);
}

BOOL CCMPermitLoadGrid::InsertRecordByKey(const int &key, const T_MVHL_PERMIT_ITEM &data)
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

BOOL CCMPermitLoadGrid::DeleteRecordByKey(const int &key, const T_MVHL_PERMIT_ITEM &data)
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
// CCMPermitLoadGrid Overridable Functions
void CCMPermitLoadGrid::OnChangedSelectionPublic(long *keys, int nSize)
{
	return;
}

long CCMPermitLoadGrid::GetSelectedKeys(CArray<long, long> &caKey)
{
	return 0;
}

BOOL CCMPermitLoadGrid::ValidateField(CString value, int nColID)
{
	if (nColID >= GetColCount())
	{
		ASSERT(0);
		return FALSE;
	}

	int nErrCode;
	if (nColID == 2)
	{
		if (!IsValidDoubleNumber(value, nErrCode))
		{
			switch(nErrCode)
			{
			case 1: SetWarningText(_LS(IDS_WG_CMD__ADDD__Invalid_formula_)); break;
			case 2: SetWarningText(_LS(IDS_WG_CMD__ADDD__Charater_exist_in_double_value)); break;
			case 3: SetWarningText(_LS(IDS_WG_CMD__ADDD__Value_is_too_big_)); break;
			}
			return FALSE;
		}
		return TRUE;
	}
	else if (nColID >= 3) return TRUE;
	return TRUE;
}

// 삽입이나 수정전에 Record의 Validation을 검사한다.
BOOL CCMPermitLoadGrid::ValidateRecord(CStringArray &value, CRowColArray &aCols)
{
	CString msg = _T("");
	int nPos;

	for (int i = 0; i < GetColCount(); i++)
	{
		nPos = aCols[i]-1;
		if (nPos >= 3) continue;
		if (nPos == 0 && value[nPos] == _T("")) value[nPos] = GetNewKey();

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
BOOL CCMPermitLoadGrid::ValidateAndMakeRecord(CStringArray &value, CRowColArray& aCols, int &key, T_MVHL_PERMIT_ITEM &data)
{
	/*
	// 각 필드에 대해 Validation을 검사한다.
	for (int i = 0; i < value.GetSize(); i++)
	{
		if (!ValidateField(value[aCols[i]-1], i)) 
		{
			//SetWarningText(_T(""));
			return FALSE;
		}
	}
	*/
	// 레코드에 대해 Validation을 검사한다.
	if (!ValidateRecord(value, aCols))
	{
		//SetWarningText(_T(""));
		return FALSE;
	}

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

BOOL CCMPermitLoadGrid::AddToDB(ROWCOL nRow)
{
	// Record를 만들어서 DB에 입력한다.
	int key;
	T_MVHL_PERMIT_ITEM data;
 
	GetValue(nRow, key, data);

	return AddData(key, data);
}

BOOL CCMPermitLoadGrid::AddToDB(CArray<void*, void*>& raKey, CArray<void*, void*>& raData)
{
	ASSERT(raKey.GetSize() == raData.GetSize());
	int tKey;
	CArray<int, int> aKey;
	CArray<T_MVHL_PERMIT_ITEM, T_MVHL_PERMIT_ITEM&> aData;
	for (int i = 0; i < raKey.GetSize(); i++)
	{
		tKey = *((int*)raKey.GetAt(i));
		aKey.Add(tKey);
		aData.Add(*((T_MVHL_PERMIT_ITEM*)raData.GetAt(i)));
	}
	return AddData(aKey, aData);
}


BOOL CCMPermitLoadGrid::DeleteFromDB(ROWCOL nRow)
{
	int key;
	T_MVHL_PERMIT_ITEM data;
 
	GetValue(nRow, key, data);
	if(m_pData->PermitLoad.nVS_Idx > 0) // 20080517 mylee - Check 지워질때 처리
	{
		if(nRow==m_pData->PermitLoad.nVS_Idx) m_pData->PermitLoad.nVS_Idx=0;
		if(nRow< m_pData->PermitLoad.nVS_Idx) m_pData->PermitLoad.nVS_Idx--;
	}
	return DelData(key);
}

BOOL CCMPermitLoadGrid::DeleteFromDB(CRowColArray& awRows)
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
		if(m_pData->PermitLoad.nVS_Idx > 0) // 20080517 mylee - Check 지워질때 처리
		{
			if(awRows.GetAt(i)==m_pData->PermitLoad.nVS_Idx) m_pData->PermitLoad.nVS_Idx=0;
			if(awRows.GetAt(i)< m_pData->PermitLoad.nVS_Idx) m_pData->PermitLoad.nVS_Idx--;
		}
	}

	return DelData(awKeys);
}

BOOL CCMPermitLoadGrid::ModifyToDB(ROWCOL nRow)
{
	ROWCOL nCol;
	VERIFY(GetColumnNoFor(0, nCol));

	int key, oldKey;
	T_MVHL_PERMIT_ITEM data;
	
	oldKey = _tstof(GetValueRowColBeforeEdit(nRow, nCol));
	GetValue(nRow, key, data);

	// 수정한다. 실패하면 에러 리턴
	return ModifyData(oldKey, key, data); 
}

BOOL CCMPermitLoadGrid::ModifyToDB(CStringArray& raOldKey, 
												 CArray<void*, void*>& raKey, 
												 CArray<void*, void*>& raData)
{
	ASSERT(raKey.GetSize() == raData.GetSize());
	ASSERT(raKey.GetSize() == raOldKey.GetSize());
	CArray<int, int> aOldKey, aKey;
	CArray<T_MVHL_PERMIT_ITEM, T_MVHL_PERMIT_ITEM&> aData;
	int Key;
	int nSize = raKey.GetSize();
	for (int i = 0; i < nSize; i++)
	{
		Key = _tstof(raOldKey.GetAt(i));
		aOldKey.Add(Key);
		aKey.Add(*((int*)raKey.GetAt(i)));
		aData.Add(*((T_MVHL_PERMIT_ITEM*)raData.GetAt(i)));
	}
	return ModifyData(aOldKey, aKey, aData);
}


CString CCMPermitLoadGrid::GetNewKey()
{
	CString value(_T("-1"));
	return value;
}

CString CCMPermitLoadGrid::GetDefValue(int nIndex)
{
	return m_aCurDefVal[nIndex];
}

CString* CCMPermitLoadGrid::GetDefValueArray()
{
	return m_aCurDefVal;
}

BOOL CCMPermitLoadGrid::MakeKeyAndDataArray(CStringArray& value, CRowColArray& awCols, 
																	CArray<void*, void*>& aKey, CArray<void*, void*>& aData)
{
	int* pKey;
	T_MVHL_PERMIT_ITEM* pData;
	pKey = new int;
	pData = new T_MVHL_PERMIT_ITEM;
	aKey.Add((void*)pKey);
	aData.Add((void*)pData);
	if (!ValidateAndMakeRecord(value, awCols, *pKey, *pData)) return FALSE;
	return TRUE;
}

void CCMPermitLoadGrid::DeleteKeyAndDataArray(CArray<void*, void*>& aKey, CArray<void*, void*>& aData)
{
	int* pKey;
	T_MVHL_PERMIT_ITEM* pData;
	ASSERT(aKey.GetSize() == aData.GetSize());
	for (int i = 0; i < aKey.GetSize(); i++)
	{
		pKey = (int*)aKey.GetAt(i);
		pData = (T_MVHL_PERMIT_ITEM*)aData.GetAt(i);
		delete pKey;
		delete pData;
	}
}

long CCMPermitLoadGrid::GetCountData()
{
	return m_pData->PermitLoad.aPermitLoad.GetSize();
}

void CCMPermitLoadGrid::MakeItemFiltered(long nFilteredNum, long* aFilteredKeys, CRowColArray& awCols)
{
	ASSERT(0); // no filtering
}

void CCMPermitLoadGrid::MakeItemNormal(long nNumData, CRowColArray& awCols)
{
	int Key;
	T_MVHL_PERMIT_ITEM Data;

	long nCount;
	for(nCount=0 ; nCount < nNumData; nCount++)
	{
		Key = nCount;
		Data = m_pData->PermitLoad.aPermitLoad[nCount];

		SetValue(GetParam()->GetData(), nCount+1, Key, Data, awCols);
	} // end of loop nCount
}

void CCMPermitLoadGrid::MakeItemEditingRecord(ROWCOL ncRow, CStringArray& EditingRecord)
{
	CDBDoc* pDoc = m_pDoc;
	int Key;
	T_MVHL_PERMIT_ITEM Data;
	for (int i = 0; i < GetColCount(); i++)
	{
		if (i == 0) Key = _ttoi(EditingRecord[i]);
		else ConvStrToData(i, EditingRecord[i], Data);
	}
	// 현재 값을 설정한다.
	SetValue(ncRow, Key, Data);
}

/////////////////////////////////////////////////////////////////////////////
// CCMPermitLoadGrid Implementation
BOOL CCMPermitLoadGrid::GetValue(ROWCOL nRow, int &key, T_MVHL_PERMIT_ITEM &data)
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

		if(i==0) key = _ttoi(str);
		else ConvStrToData(i, str, data);
		if(i==2)
		{
			if(str == _T("1")) m_pData->PermitLoad.nVS_Idx = nRow;
			else
			{
				if(m_pData->PermitLoad.nVS_Idx == nRow) m_pData->PermitLoad.nVS_Idx = 0;
			}
		}
	}
	return TRUE;
}

BOOL CCMPermitLoadGrid::SetValue(ROWCOL nRow, const int &key, const T_MVHL_PERMIT_ITEM &data)
{
	//
	CRowColArray awCols;
	awCols.SetSize(GetColCount());
	for (int i = 0;i < GetColCount(); i++)
		VERIFY(GetColumnNoFor(m_aTitle[i], awCols[i]));
	return SetValue(GetParam()->GetData(), nRow, key, data, awCols);
}

BOOL CCMPermitLoadGrid::SetValue(CGXData* pData, ROWCOL nRow, const int &key, const T_MVHL_PERMIT_ITEM &data, CRowColArray &awCols)
{
	//
	CString str;
	for (int i = 0; i < GetColCount(); i++)
	{
		if(i==0) str.Format(_T("%d"),key);
		else ConvDataToStr(key, i, data, str);
		if(i==2)
		{
			if(m_pData->PermitLoad.nVS_Idx == nRow) str = _T("1");
			else str = _T("0");
		}
		pData->StoreValueRowCol(nRow, awCols[i], str, gxOverride, GX_VT_NUMERIC);
	}

	if(nRow == GetRowCount()-1)
		MakeLastSpaceEnd(m_pData->PermitLoad.nVS_Idx == 0);

	if (nRow > 1)
	{
		ROWCOL nSpaceCol;
		GetColumnNoFor(2, nSpaceCol);
		SetStyleRange(CGXRange(nRow, nSpaceCol), CGXStyle()
			.SetEnabled(TRUE));
	}

	return TRUE;
}

void CCMPermitLoadGrid::ConvStrToData(int i, CString& value, T_MVHL_PERMIT_ITEM& data)
{
	if (i == 1) data.nAxleType = GetAxleTypeIx(value);
	else if (i == 2) ; // GetValue 에서 처리
	else if (i == 3) data.dSpacing = _tstof(value);
}

void CCMPermitLoadGrid::ConvDataToStr(int key, int i, const T_MVHL_PERMIT_ITEM& data, CString& value)
{
	if (i == 1) value = GetAxleTypeName(data.nAxleType);
	else if (i == 2) ; // SetValue 에서 처리
	else if (i == 3) value.Format(DATA_FMT, data.dSpacing);
	else if (i >= 4)
	{
		int ix = m_pData->PermitLoad.aPermitLoad[key].nAxleType;
		if ( (i-4)/2 >= m_pData->PermitLoad.aAxleType[ix].aLoad.GetSize())
			value = _T("");
		else
		{
			if (i % 2 == 0) // P
				//value.Format(DATA_FMT, m_pData->PermitLoad.aAxleType[ix].aDist[(i-4)/2]);
				value.Format(DATA_FMT, m_pData->PermitLoad.aAxleType[ix].aLoad[(i-4)/2]);
			else  // D
				//value.Format(DATA_FMT, m_pData->PermitLoad.aAxleType[ix].aLoad[(i-4)/2]);
				value.Format(DATA_FMT, m_pData->PermitLoad.aAxleType[ix].aDist[(i-4)/2]);
		}
	}
}

void CCMPermitLoadGrid::MakeSearchKey(const int &key, const T_MVHL_PERMIT_ITEM &data, CStringArray &aKey)
{
	int nColID;
	int nSortKeyNum = m_aSortInfo.GetSize();

	aKey.SetSize(nSortKeyNum);
	for (int i = 0; i < nSortKeyNum; i++)
	{
		nColID = m_aSortInfo[i].nRC;
		if (nColID == 0) aKey[i].Format(_T("%d"), key);
		else ConvDataToStr(key, nColID, data, aKey[i]);
	}
}

void CCMPermitLoadGrid::SetHeaderTitle(T_UNIT_SYSTEM &us, BOOL bInit)
{
	// Set Title
	CString csTitle, csUnit;
	int aUnit[] = {
		D_UNITSYS_NONE, D_UNITSYS_NONE, D_UNITSYS_NONE, D_UNITSYS_BASE_LENGTH, 
	};
	int aUnit2[] = { D_UNITSYS_BASE_FORCE, D_UNITSYS_BASE_LENGTH, };

	int aSize[] = {50, 80, 30, 70, 50};
	int nColCount = GetColCount();

	int nUnit;
	for (int i = 0; i < nColCount; i++)
	{
		if (i < 4) nUnit = aUnit[i];
		else nUnit = aUnit2[(i-4)%2];
		m_aUnit.SetAt(i, nUnit);
		CUnitCtrl::GetUnitSystem(nUnit, csUnit);
		if (csUnit.IsEmpty()) csTitle = m_aTitle[i];
		else csTitle = m_aTitle[i] + _T("\n(")+csUnit+_T(")");
		SetStyleRange(CGXRange(0, i+1), CGXStyle()
			.SetValue(csTitle)
			.SetUserAttribute(ID_USER_ATTR_COL_ID, m_aTitle[i]));
		if (bInit) 
		{
			if (i < 4) SetColWidthByDPI(i+1, i+1, aSize[i]);
			else SetColWidthByDPI(i+1, i+1, aSize[4]);
		}
	}
	if (bInit) SetColWidthByDPI(0, 0, 35);
}

BOOL CCMPermitLoadGrid::AddData(int key, T_MVHL_PERMIT_ITEM& data)
{
	if (key < 0) key = m_pData->PermitLoad.aPermitLoad.GetSize();
	CArray<int, int> aOldKey, aKey;
	CArray<T_MVHL_PERMIT_ITEM, T_MVHL_PERMIT_ITEM&> aData;
	aKey.Add(key);
	aData.Add(data);
	T_MVHL_PERMIT_ITEM item;
	item = data;

	m_pData->PermitLoad.aPermitLoad.InsertAt(key, item);
	UpdateBuffer(0, aOldKey, aKey, aData);
	DBChanged();
	return TRUE;
}

BOOL CCMPermitLoadGrid::AddData(CArray<int, int> &aKey, CArray<T_MVHL_PERMIT_ITEM, T_MVHL_PERMIT_ITEM&> &aData, BOOL bNotify)
{
	CArray<int, int> aOldKey;

	int nSize = aKey.GetSize();
	for (int i = 0; i < nSize; i++)
	{
		if (aKey[i] < 0) aKey[i] = m_pData->PermitLoad.aPermitLoad.GetSize();

		T_MVHL_PERMIT_ITEM item = aData[i];
		m_pData->PermitLoad.aPermitLoad.InsertAt(aKey[i], item);
	}
	UpdateBuffer(0, aOldKey, aKey, aData);

	if (bNotify) DBChanged();
	return TRUE;
}

BOOL CCMPermitLoadGrid::DelData(int key)
{
	CArray<int, int> aKey;
	aKey.Add(key);
	return DelData(aKey, TRUE);
}

BOOL CCMPermitLoadGrid::DelData(CArray<int, int> &aKey, BOOL bNotify)
{
	if (aKey.GetSize() < 1) return TRUE;

	CArray<int, int> aOldKey;
	CArray<T_MVHL_PERMIT_ITEM, T_MVHL_PERMIT_ITEM&> aData;
	T_MVHL_PERMIT_ITEM temp;
	int nSize = aKey.GetSize();
	qsort(aKey.GetData(), nSize, sizeof(int), CCompFunc::intAsc);
	for (int i = 0; i < nSize; i++) aData.Add(temp);
	for (int i = nSize-1; i >= 0; i--)
	{
		if (aKey[i] >= 0 && aKey[i] < m_pData->PermitLoad.aPermitLoad.GetSize())
			m_pData->PermitLoad.aPermitLoad.RemoveAt(aKey[i]);
	}
	UpdateBuffer(1, aOldKey, aKey, aData);

	if (bNotify) DBChanged();
	return TRUE;
}

BOOL CCMPermitLoadGrid::ModifyData(int oldKey, int key, T_MVHL_PERMIT_ITEM& data)
{
	CArray<int, int> aOldKey, aKey;
	CArray<T_MVHL_PERMIT_ITEM, T_MVHL_PERMIT_ITEM&> aData;
	aOldKey.Add(oldKey);
	aKey.Add(key);
	aData.Add(data);
	return ModifyData(aOldKey, aKey, aData);
}

BOOL CCMPermitLoadGrid::ModifyData(CArray<int, int> &aOldKey, CArray<int, int> &aKey, CArray<T_MVHL_PERMIT_ITEM, T_MVHL_PERMIT_ITEM&> &aData)
{
	for (int i = 0; i < aKey.GetSize(); i++)
	{
		if (aKey[i] >= 0 && aKey[i] < m_pData->PermitLoad.aPermitLoad.GetSize())
			m_pData->PermitLoad.aPermitLoad[aKey[i]] = aData[i];
	}
	UpdateBuffer(2, aOldKey, aKey, aData);
	DBChanged();
	return TRUE;
}

void CCMPermitLoadGrid::UpdateBuffer(int nCmd, CArray<int, int> &aOldKey, CArray<int, int> &aKey, CArray<T_MVHL_PERMIT_ITEM, T_MVHL_PERMIT_ITEM&> &aData)
{
	// 현재 Row, Col을 저장 : 나중에 복구하기 위함
	ROWCOL ncRow, ncCol;
	GetCurrentCell(ncRow, ncCol);

	BOOL bOldLock = LockUpdate(TRUE);

	int nSize = aKey.GetSize();
	for (int i = 0; i < nSize; i++)
	{
		switch(nCmd)
		{
		case(0):
				// 삽입될 위치(nRow)를 찾아 넣는다.
				//InsertRecordByKey(aKey[i], aData[i]);
				MakeItemEx();
				break;
		case(1):
				// 삭제될 위치(nRow)를 찾아서 지운다.
				//DeleteRecordByKey(aKey[i], aData[i]);
				MakeItemEx();
				break;
		case(2):
				//DeleteRecordByKey(aOldKey[i], aData[i]);
				//InsertRecordByKey(aKey[i], aData[i]);
				MakeItemEx();
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

	// 특수 처리 : key 정보 다시 Update
	CString csVal;
	ROWCOL nKeyCol;
	GetColumnNoFor(0, nKeyCol);
	for (int i = 0; i < m_pData->PermitLoad.aPermitLoad.GetSize(); i++)
	{
		csVal.Format(_T("%d"), i);
		SetValueRange(CGXRange(i+1, nKeyCol), csVal);
	}

	LockUpdate(bOldLock);
	if (!bOldLock) Redraw();
}

void CCMPermitLoadGrid::DBChanged()
{
	AdjustAppendRowNo();
	/*
	CSpfcItemDlg* pDlg = (CSpfcItemDlg*)GetParent();
	if (!pDlg) return;
	if (pDlg->GetSafeHwnd() == 0) return;
	if (!IsWindow(pDlg->GetSafeHwnd())) return;

	pDlg->OnDataChanged();
	*/
}

BOOL CCMPermitLoadGrid::OnPasteFromClipboard(const CGXRange &range)
{
	if (IsLockEdit()) return FALSE; // display only mode이면 바로 리턴
	BOOL bSuccess = CTBCommon::OnPasteFromClipboard(range);
	return bSuccess;
}

BOOL CCMPermitLoadGrid::Copy()
{
	return CTBCommon::Copy();
}

void CCMPermitLoadGrid::SetTypeColumn(ROWCOL nCol)
{
	CString szChoiceList, tmp;
	szChoiceList = _T("");

	int nSize = m_pData->PermitLoad.aAxleType.GetSize();
	for (int i = 0; i < nSize; i++)
	{
		tmp.Format(_T("%s\n"), m_pData->PermitLoad.aAxleType[i].strAxleType);
		szChoiceList += tmp;
	}

	SetStyleRange(CGXRange().SetCols(nCol), CGXStyle()
			.SetControl(GX_IDS_CTRL_TABBED_COMBOBOX)
			.SetChoiceList(szChoiceList)
			.SetUserAttribute(GX_IDS_UA_TABLIST_KEYCOL, _T("0"))
			.SetUserAttribute(GX_IDS_UA_TABLIST_TEXTCOL, _T("0"))
			.SetUserAttribute(GX_IDS_UA_TABLIST_SHOWALLCOLS, _T("1"))
			.SetValueType(GX_VT_STRING)
			.SetHorizontalAlignment(DT_LEFT)
			.SetValue(_T(""))
			);
	ROWCOL ncRow, ncCol;
	GetCurrentCell(ncRow, ncCol);
	if (ncCol == nCol) CGXGridWnd::SetCurrentCell(ncRow, ncCol);
}

int CCMPermitLoadGrid::GetAxleTypeIx(LPCTSTR lpszTypeName)
{
	for (int i = 0; i < m_pData->PermitLoad.aAxleType.GetSize(); i++)
		if (m_pData->PermitLoad.aAxleType[i].strAxleType == lpszTypeName) return i;
	ASSERT(0);
	return -1;
}

CString CCMPermitLoadGrid::GetAxleTypeName(int nTypeIx)
{
	if (nTypeIx < 0 || nTypeIx >= m_pData->PermitLoad.aAxleType.GetSize()) return _T("");
	return m_pData->PermitLoad.aAxleType[nTypeIx].strAxleType;
}

void CCMPermitLoadGrid::UpdateAxleTypeColumn()
{
	ROWCOL nTypeCol;
	GetColumnNoFor(1, nTypeCol);
	BOOL bOldLock = LockUpdate(TRUE);
	SetTypeColumn(nTypeCol);
	LockUpdate(bOldLock);
	if (!bOldLock) Redraw();
}

void CCMPermitLoadGrid::AdjustAppendRowNo(ROWCOL nCol)
{
	BOOL bEnable = TRUE;
	if (m_pData->PermitLoad.aPermitLoad.GetSize() > 0) bEnable = FALSE;
	GetColumnNoFor(2, nCol);

	ROWCOL ncRow, ncCol;
	GetCurrentCell(ncRow, ncCol);
	if (!bEnable && ncRow == 1 && ncCol == nCol)
	{
		CGXControl* pControl = GetControl(ncRow, ncCol);
		if (pControl->IsActive())
			pControl->OnCancelEditing();
		pControl->Init(ncRow, ncCol);
		pControl->Refresh();
		pControl->OnCanceledEditing();
	}
	if (m_pData->PermitLoad.aPermitLoad.GetSize() > 0)
	{
		SetStyleRange(CGXRange(1, nCol), CGXStyle()
			.SetEnabled(bEnable)
			.SetValue(_T("0")));
	}
}

BOOL CCMPermitLoadGrid::OnInsertEmptyRecord(ROWCOL nRow)
{
	BOOL bResult = CTBCommon::OnInsertEmptyRecord(nRow);
	// 첫번째 Record 아니면 Spacing 활성화
	ROWCOL nSpaceCol;
	GetColumnNoFor(3, nSpaceCol);
	if (m_pData->PermitLoad.aPermitLoad.GetSize() > 0)
	{
		SetStyleRange(CGXRange(nRow, nSpaceCol), CGXStyle()
			.SetEnabled(TRUE));
	}
	return bResult;
}


BOOL CCMPermitLoadGrid::OnStartEditing(ROWCOL nRow, ROWCOL nCol)
{
	// 부모의 OnModifyCell을 항상 먼저 호출해 주어야 한다.
	BOOL bResult = CTBCommon::OnStartEditing(nRow, nCol);
	
	ROWCOL nVSCol;
	GetColumnNoFor(2, nVSCol);
	if(nCol != nVSCol) return bResult;

	CString str;
	str = GetValueRowCol(nRow, nCol);
	int nOldIdx = m_pData->PermitLoad.nVS_Idx;

	// 20080517 mylee - 두개일 경우 Check off
	if(str == _T("0"))
	{
		if(nOldIdx != 0) SetValueRange(CGXRange(nOldIdx, nVSCol), _T("0"));
	}

	MakeLastSpaceEnd(str == _T("1"));
	
	return bResult;
}

// 20080517 mylee - end 처리(두군데 쓰임 SetValue, OnStartEditing)
void CCMPermitLoadGrid::MakeLastSpaceEnd(BOOL bMakeEnd)
{
	int nRowCount = GetRowCount();
	if (nRowCount>1)
	{
		ROWCOL nSpaceCol;
		GetColumnNoFor(3, nSpaceCol);
		if(bMakeEnd)
		{
			SetStyleRange(CGXRange(nRowCount-1, nSpaceCol), CGXStyle().SetEnabled(FALSE));
			SetValueRange(CGXRange(nRowCount-1, nSpaceCol), _T("end"));
			int nSize = m_pData->PermitLoad.aPermitLoad.GetSize();
			m_pData->PermitLoad.aPermitLoad[nSize-1].dSpacing = 0.0;
		}
		else
		{
			SetStyleRange(CGXRange(nRowCount-1, nSpaceCol), CGXStyle().SetEnabled(TRUE));
			int nSize = m_pData->PermitLoad.aPermitLoad.GetSize();
			SetValueRange(CGXRange(nRowCount-1, nSpaceCol), m_pData->PermitLoad.aPermitLoad[nSize-1].dSpacing);
		}
	}
}