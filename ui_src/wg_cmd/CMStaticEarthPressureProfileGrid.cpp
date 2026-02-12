// ThfcItemGrid.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMStaticEarthPressureProfileGrid.h"
#include "CMStaticEarthPressureProfileDlg.h"

#include "..\wg_base\wg_base_CompFunc.h"
#include "..\wg_db\wg_db_UnitCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define EPSILON  1.e-12
#define DATA_FMT _T("%.15g")
#define KEY_FMT    _T("%d")

enum EN_EPST_PROFILE_GRIDCOL
{
	EN_EPST_PROFILE_GRIDCOL_INDEX,
	EN_EPST_PROFILE_GRIDCOL_LEVEL,
	EN_EPST_PROFILE_GRIDCOL_PRESSURE,
	EN_EPST_PROFILE_GRIDCOL_ADDTIONAL,
};

/////////////////////////////////////////////////////////////////////////////
// CCMStaticEarthPressureProfileGrid

CCMStaticEarthPressureProfileGrid::CCMStaticEarthPressureProfileGrid()
{
	m_pData = NULL;

	AddColInfo(_LS(IDS_WG_CMD__ADDD__KEY), t_integer, D_UNITSYS_NONE);
	AddColInfo(_LS(IDS_WG_CMD__ADDD__Level), t_real, D_UNITSYS_BASE_LENGTH);
	AddColInfo(_LS(IDS_CMD_EARTHP_PROFILE_SOIL_PRESSURE), t_real, D_UNITSYS_BASE_PRESSURE);
	AddColInfo(_LS(IDS_CMD_EARTHP_PROFILE_ADDITIONAL_PRESSURE), t_real, D_UNITSYS_BASE_PRESSURE);

	m_aCurDefVal[0] = _T("0");
	m_aCurDefVal[1] = _T("0.0");
	m_aCurDefVal[2] = _T("0.0");
	m_aCurDefVal[3] = _T("0.0");

	m_aPrimaryKey.Add(0);
	m_pParent = NULL;
}

CCMStaticEarthPressureProfileGrid::~CCMStaticEarthPressureProfileGrid()
{
}


BEGIN_MESSAGE_MAP(CCMStaticEarthPressureProfileGrid, CTBCommon)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CCMStaticEarthPressureProfileGrid message handlers

void CCMStaticEarthPressureProfileGrid::Initialize(T_EPST_D *pData, CCMStaticEarthPressureProfileDlg* pParent)
{
	m_nDir = 0;
	m_pParent = pParent;
	ASSERT(pData);
	m_pData = pData;

	T_UNIT_SYSTEM us;
	CDBDoc *pDoc = CDBDoc::GetDocPoint();
	CTBCommon::Initialize(pDoc, us);

	GetParam()->EnableUndo(FALSE);
	GetParam()->EnableTrackRowHeight(FALSE);
	GetParam()->EnableMoveCols(FALSE);        // columns can not be dragged

	CGXProperties* pProp = GetParam()->GetProperties();
	pProp->SetMarkColHeader(FALSE); // Turn off pressed button effect for column headers
	pProp->SetMarkRowHeader(FALSE);
	pProp->SetUserProperty(GX_IDS_OUTLINECURRENTCELL, // 현재 셀의 border 설정 변경
		(CGXStyle)pProp->sInvertNoBorder);

	EnableAppendRow(FALSE);
	EnableInsertRowByPressingKey(FALSE);

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
	SetStyleRange(CGXRange().SetCols(EN_EPST_PROFILE_GRIDCOL_LEVEL+1, m_nColCount), CGXStyle()
		.SetHorizontalAlignment(DT_RIGHT)
		.SetValueType(GX_VT_NUMERIC)
		.SetFormat(GX_FMT_FIXED)
		.SetPlaces(4));

	//Pressure는 수정불가로...
	SetStyleRange(CGXRange().SetCols(EN_EPST_PROFILE_GRIDCOL_LEVEL + 1, EN_EPST_PROFILE_GRIDCOL_PRESSURE + 1), CGXStyle()
		.SetControl(GX_IDS_CTRL_STATIC)
		.SetEnabled(FALSE)
		.SetPlaces(4));

	SetRowHeightByDPI(0, 0, 52);

	// 현재 셀 설정
	SetRightValidCell(1, 0);
	GetParam()->SetHideCurrentCell(GX_HIDE_NEVER);

	// Sorting 정보 초기화
	m_aSortInfo.SetSize(1);
	m_aSortInfo[0].nRC = 0;
	m_aSortInfo[0].sortType = CGXSortInfo::numeric;
	m_aSortInfo[0].sortOrder = CGXSortInfo::ascending;

	LockUpdate(FALSE);
}

BOOL CCMStaticEarthPressureProfileGrid::InsertRecordByKey(const int &key, const T_EPST_PRESSURE_PROFILE& data)
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

BOOL CCMStaticEarthPressureProfileGrid::ModifyRecordByKey(const int &key, const T_EPST_PRESSURE_PROFILE& data)
{
	// 삽입 위치를 찾는다.
	ROWCOL nRow;

	// aKey를 만드는 코드 삽입
	CStringArray aKey;
	MakeSearchKey(key, data, aKey);
	if (!FindRecordByKey(aKey, nRow)) return FALSE;

	return SetValue(nRow, key, data);
}

BOOL CCMStaticEarthPressureProfileGrid::DeleteRecordByKey(const int &key, const T_EPST_PRESSURE_PROFILE& data)
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
// CCMStaticEarthPressureProfileGrid Overridable Functions
void CCMStaticEarthPressureProfileGrid::OnChangedSelectionPublic(long *keys, int nSize)
{
	return;
}

long CCMStaticEarthPressureProfileGrid::GetSelectedKeys(CArray<long, long> &caKey)
{
	return 0;
}

BOOL CCMStaticEarthPressureProfileGrid::ValidateField(CString value, int nColID)
{
	if (nColID >= GetColCount())
	{
		SetWarningText(_LS(IDS_WG_CMD__ADDD___CCMStorGrid__ValidateField__Erro));
		return FALSE;
	}

	int nErrCode;

	if (!IsValidDoubleNumber(value, nErrCode))
	{
		switch (nErrCode)
		{
		case 1: SetWarningText(_LS(IDS_WG_CMD__ADDD__Invalid_formula_)); break;
		case 2: SetWarningText(_LS(IDS_WG_CMD__ADDD__Charater_exist_in_double_value)); break;
		case 3: SetWarningText(_LS(IDS_WG_CMD__ADDD__Value_is_too_big_)); break;
		}
		return FALSE;
	}


	return TRUE;
}

// 삽입이나 수정전에 Record의 Validation을 검사한다.
BOOL CCMStaticEarthPressureProfileGrid::ValidateRecord(CStringArray &value, CRowColArray &aCols)
{
	CString msg = _T("");
	int nPos;
	int nColCnt = GetColCount();

	for (int i = 0; i < nColCnt; i++)
	{
		nPos = aCols[i] - 1;
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
BOOL CCMStaticEarthPressureProfileGrid::ValidateAndMakeRecord(CStringArray &value, CRowColArray& aCols, int &key, T_EPST_PRESSURE_PROFILE& data)
{
	// 레코드에 대해 Validation을 검사한다.
	if (!ValidateRecord(value, aCols))
	{
		return FALSE;
	}

	// 레코드를 만들어서 리턴한다.
	int nPos;
	for (int i = 0; i < GetColCount(); i++)
	{
		nPos = aCols[i] - 1; // col id i에 해당하는 value가 저장된 위치
		if (i == 0) key = nPos+1;
		else ConvStrToData(i, value[nPos], data);
	}
	return TRUE;
}

BOOL CCMStaticEarthPressureProfileGrid::AddToDB(ROWCOL nRow)
{
	// Record를 만들어서 DB에 입력한다.
	T_EPST_PRESSURE_PROFILE data;

	GetValue(nRow, data);

	return AddData(nRow, data);
}

BOOL CCMStaticEarthPressureProfileGrid::AddToDB(CArray<void*, void*>& raKey, CArray<void*, void*>& raData)
{
	ASSERT(raKey.GetSize() == raData.GetSize());
	int tKey;
	CArray<int, int> aKey;
	CArray<T_EPST_PRESSURE_PROFILE, T_EPST_PRESSURE_PROFILE&> aData;
	for (int i = 0; i < raKey.GetSize(); i++)
	{
		tKey = *((int*)raKey.GetAt(i));
		aKey.Add(tKey);
		aData.Add(*((T_EPST_PRESSURE_PROFILE*)raData.GetAt(i)));
	}
	return AddData(aKey, aData);
}


BOOL CCMStaticEarthPressureProfileGrid::DeleteFromDB(ROWCOL nRow)
{
	T_EPST_PRESSURE_PROFILE data;

	GetValue(nRow, data);
	return DelData(data.dLevel);
}

BOOL CCMStaticEarthPressureProfileGrid::DeleteFromDB(CRowColArray& awRows)
{
	if (awRows.GetSize() <= 0) return TRUE;

	ROWCOL nCol;
	VERIFY(GetColumnNoFor(0, nCol));

	CString value;
	CArray<int, int> awKeys;

	for (int i = 0; i < awRows.GetSize(); i++)
	{
		value = GetValueRowColBeforeEdit(awRows.GetAt(i), nCol);
		awKeys.Add(_ttoi(value));
	}

	return DelData(awKeys);
}

BOOL CCMStaticEarthPressureProfileGrid::ModifyToDB(ROWCOL nRow)
{
	ROWCOL nCol;
	VERIFY(GetColumnNoFor(0, nCol));

	double oldKey;
	T_EPST_PRESSURE_PROFILE data;

	oldKey = _ttoi(GetValueRowColBeforeEdit(nRow, nCol));
	GetValue(nRow, data);

	// 수정한다. 실패하면 에러 리턴
	return ModifyData(oldKey, data.dLevel, data);
}

BOOL CCMStaticEarthPressureProfileGrid::ModifyToDB(CStringArray& raOldKey,
	CArray<void*, void*>& raKey,
	CArray<void*, void*>& raData)
{
	ASSERT(raKey.GetSize() == raData.GetSize());
	ASSERT(raKey.GetSize() == raOldKey.GetSize());
	CArray<int, int> aOldKey, aKey;
	CArray<T_EPST_PRESSURE_PROFILE, T_EPST_PRESSURE_PROFILE&> aData;
	int Key;
	int nSize = raKey.GetSize();
	for (int i = 0; i < nSize; i++)
	{
		Key = _ttoi(raOldKey.GetAt(i));
		aOldKey.Add(Key);
		aKey.Add(Key);
		//((T_EPST_PRESSURE_PROFILE*)raData.GetAt(i))->dLevel = Key;
		aData.Add(*((T_EPST_PRESSURE_PROFILE*)raData.GetAt(i)));
	}
	return ModifyData(aOldKey, aKey, aData);
}


CString CCMStaticEarthPressureProfileGrid::GetNewKey()
{
	CString value(_T("0"));
	return value;
}

CString CCMStaticEarthPressureProfileGrid::GetDefValue(int nIndex)
{
	return m_aCurDefVal[nIndex];
}

CString* CCMStaticEarthPressureProfileGrid::GetDefValueArray()
{
	return &m_aCurDefVal[0];
}

BOOL CCMStaticEarthPressureProfileGrid::MakeKeyAndDataArray(CStringArray& value, CRowColArray& awCols,
	CArray<void*, void*>& aKey, CArray<void*, void*>& aData)
{
	int* pKey;
	T_EPST_PRESSURE_PROFILE* pData;
	pKey = new int;
	pData = new T_EPST_PRESSURE_PROFILE;
	aKey.Add((void*)pKey);
	aData.Add((void*)pData);
	if (!ValidateAndMakeRecord(value, awCols, *pKey, *pData)) return FALSE;
	return TRUE;
}

void CCMStaticEarthPressureProfileGrid::DeleteKeyAndDataArray(CArray<void*, void*>& aKey, CArray<void*, void*>& aData)
{
	double* pKey;
	T_EPST_PRESSURE_PROFILE* pData;
	ASSERT(aKey.GetSize() == aData.GetSize());
	for (int i = 0; i < aKey.GetSize(); i++)
	{
		pKey = (double*)aKey.GetAt(i);
		pData = (T_EPST_PRESSURE_PROFILE*)aData.GetAt(i);
		delete pKey;
		delete pData;
	}
}

long CCMStaticEarthPressureProfileGrid::GetCountData()
{
	return m_pData->arPresProfile.GetSize();
}

void CCMStaticEarthPressureProfileGrid::MakeItemFiltered(long nFilteredNum, long* aFilteredKeys, CRowColArray& awCols)
{
	ASSERT(0); // no filtering
}

void CCMStaticEarthPressureProfileGrid::MakeItemNormal(long nNumData, CRowColArray& awCols)
{
	T_EPST_PRESSURE_PROFILE data;

	long i;
	long nCount = 1;
	//테이블 상단이 마지막이 되도록 수정하였음.
	//for(nCount=0 ; nCount < nNumData; nCount++)
	long lSizeProfile = m_pData->arPresProfile.GetSize();
	ASSERT(lSizeProfile == nNumData);
	for (i = 0; i < nNumData; i++)
	{
		data = m_pData->arPresProfile[i];

		SetValue(GetParam()->GetData(), nCount++, i+1, data, awCols);
	}
}

void CCMStaticEarthPressureProfileGrid::MakeItemEditingRecord(ROWCOL ncRow, CStringArray& EditingRecord)
{
	CDBDoc* pDoc = m_pDoc;
	double Key;
	T_EPST_PRESSURE_PROFILE Data;
	for (int i = 0; i < GetColCount(); i++)
	{
		if (i == 0) Key = _ttoi(EditingRecord[i]);
		else ConvStrToData(i, EditingRecord[i], Data);
	}
	// 현재 값을 설정한다.
	SetValue(ncRow, Key, Data);
}

/////////////////////////////////////////////////////////////////////////////
// CCMStaticEarthPressureProfileGrid Implementation
BOOL CCMStaticEarthPressureProfileGrid::GetValue(ROWCOL nRow, T_EPST_PRESSURE_PROFILE& profileItem)
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

		int nKey=0;
		if (i == 0) nKey = _tstof(str);
		else ConvStrToData(i, str, profileItem);
	}
	return TRUE;
}

BOOL CCMStaticEarthPressureProfileGrid::SetValue(ROWCOL nRow, const int key, const T_EPST_PRESSURE_PROFILE &data)
{
	//
	CRowColArray awCols;
	awCols.SetSize(GetColCount());
	for (int i = 0;i < GetColCount(); i++)
		VERIFY(GetColumnNoFor(m_aTitle[i], awCols[i]));
	return SetValue(GetParam()->GetData(), nRow, key, data, awCols);
}

BOOL CCMStaticEarthPressureProfileGrid::SetValue(CGXData* pData, ROWCOL nRow, const int key, const T_EPST_PRESSURE_PROFILE &data, CRowColArray &awCols)
{
	//
	CString str;
	for (int i = 0; i < GetColCount(); i++)
	{
		if (i == 0) str.Format(KEY_FMT, key);
		else ConvDataToStr(i, data, str);
		pData->StoreValueRowCol(nRow, awCols[i], str, gxOverride, GX_VT_NUMERIC);
	}

	return TRUE;
}

void CCMStaticEarthPressureProfileGrid::ConvStrToData(int i, CString& value, T_EPST_PRESSURE_PROFILE& data)
{
	if      (i == EN_EPST_PROFILE_GRIDCOL_LEVEL)		data.dLevel = _tstof(value);
	else if (i == EN_EPST_PROFILE_GRIDCOL_PRESSURE)     data.dSoilPressure = _tstof(value);
	else if (i == EN_EPST_PROFILE_GRIDCOL_ADDTIONAL)    data.dAdditionalPressure = _tstof(value);
	else ASSERT(0);
}

void CCMStaticEarthPressureProfileGrid::ConvDataToStr(int i, const T_EPST_PRESSURE_PROFILE& data, CString& value)
{
	if		(i == EN_EPST_PROFILE_GRIDCOL_LEVEL)		value.Format(DATA_FMT, data.dLevel);
	else if (i == EN_EPST_PROFILE_GRIDCOL_PRESSURE)     value.Format(DATA_FMT, data.dSoilPressure);
	else if (i == EN_EPST_PROFILE_GRIDCOL_ADDTIONAL)    value.Format(DATA_FMT, data.dAdditionalPressure);
	else ASSERT(0);
}

void CCMStaticEarthPressureProfileGrid::MakeSearchKey(const int& key, const T_EPST_PRESSURE_PROFILE& data, CStringArray& aKey)
{
	int nColID;
	int nSortKeyNum = m_aSortInfo.GetSize();

	aKey.SetSize(nSortKeyNum);
	for (int i = 0; i < nSortKeyNum; i++)
	{
		nColID = m_aSortInfo[i].nRC;
		if (nColID == 0) aKey[i].Format(KEY_FMT, key);
		else ConvDataToStr(nColID, data, aKey[i]);
	}
}

void CCMStaticEarthPressureProfileGrid::SetHeaderTitle(T_UNIT_SYSTEM &UnitSystem, BOOL bInit)
{
	int nIndexWidth = 35;
	int i;
	int nColCount = GetColCount();
	CRect rectGrid;
	GetWindowRect(rectGrid);
	int nWndWidth = rectGrid.Width() - nIndexWidth - 22; //윈도우 너비에서 index와 scrollbar 사이즈 뺀다.
	int aSize[4];
	double dWidthRatio[] = { 0.0, 0.33, 0.335, 0.335};
	for (i = 0; i < nColCount; i++)
	{
		aSize[i] = (int)((double)nWndWidth * dWidthRatio[i]);
	}

	CString csUnit;
	CString csTitle;

	for (i = 0; i < nColCount; i++)
	{
		//m_aUnit.SetAt(i, aUnit[nDataType][i]);
		CUnitCtrl::GetUnitSystem(m_aUnit[i], csUnit);

		if (csUnit.IsEmpty()) csTitle = m_aTitle[i];
		else                  csTitle = m_aTitle[i] + _T("\n(") + csUnit + _T(")");

		SetStyleRange(CGXRange(0, i + 1), CGXStyle()
			.SetValue(csTitle)
			.SetUserAttribute(ID_USER_ATTR_COL_ID, m_aTitle[i]));

		if (bInit) SetColWidth(i + 1, i + 1, aSize[i]);
	}
	if (bInit) SetColWidthByDPI(0, 0, nIndexWidth);
}

BOOL CCMStaticEarthPressureProfileGrid::FindData(int key, int& nPos, int nStart)
{
	int nSize = m_pData->arPresProfile.GetSize();
	if (key > nSize) return FALSE;

	nPos = key-1;
	return TRUE;
}

BOOL CCMStaticEarthPressureProfileGrid::AddData(int key, T_EPST_PRESSURE_PROFILE& data)
{
	CArray<int, int> aOldKey, aKey;
	CArray<T_EPST_PRESSURE_PROFILE, T_EPST_PRESSURE_PROFILE&> aData;
	aKey.Add(key);
	aData.Add(data);
	T_EPST_PRESSURE_PROFILE item = data;
	item.dLevel = key;

	// 동일한 key값 중복 검토
	int nPos;
	if (FindData(key, nPos, 0))
	{
		ASSERT(0);
		return FALSE;
	}
	m_pData->arPresProfile.InsertAt(nPos, item);
	UpdateBuffer(0, aOldKey, aKey, aData);
	return TRUE;
}

BOOL CCMStaticEarthPressureProfileGrid::AddData(CArray<int, int> &aKey,
	CArray<T_EPST_PRESSURE_PROFILE, T_EPST_PRESSURE_PROFILE&> &aData)
{
	CArray<int, int> aOldKey;

	// 동일한 key값 중복 검토
	int nSize = aKey.GetSize();
	aOldKey.Copy(aKey);
	qsort((void*)aOldKey.GetData(), nSize, sizeof(int), CCompFunc::intAsc);

	for (int i = 0; i < nSize; i++)
	{
		int nPos = 0;
		if (!FindData(aKey[i], nPos, 0)) { ASSERT(0); continue; }
		VERIFY(nPos < nSize);

		T_EPST_PRESSURE_PROFILE item = aData[i];
		m_pData->arPresProfile.InsertAt(nPos, item);
	}
	UpdateBuffer(0, aOldKey, aKey, aData);

	return TRUE;
}

BOOL CCMStaticEarthPressureProfileGrid::DelData(int key)
{
	CArray<int, int> aOldKey, aKey;
	CArray<T_EPST_PRESSURE_PROFILE, T_EPST_PRESSURE_PROFILE&> aData;
	aKey.Add(key);

	int nPos;
	if (!FindData(key, nPos))
	{
		CString msg;
		msg.Format(_LS(IDS_WG_CMD__ADDD__Time__g_does_not_exist_), key);
		AfxMessageBox(msg);
		return FALSE;
	}
	m_pData->arPresProfile[nPos].dAdditionalPressure = 0;
	aData.Add(m_pData->arPresProfile[nPos]);
	UpdateBuffer(1, aOldKey, aKey, aData);
	return TRUE;
}

BOOL CCMStaticEarthPressureProfileGrid::DelData(CArray<int, int> &aKey)
{
	CArray<int, int> aOldKey;
	CArray<T_EPST_PRESSURE_PROFILE, T_EPST_PRESSURE_PROFILE&> aData;
	int nSize = aKey.GetSize();

	aOldKey.Copy(aKey);
	qsort((void*)aOldKey.GetData(), nSize, sizeof(int), CCompFunc::intAsc);

	int nSize2 = m_pData->arPresProfile.GetSize();
	for (int j=0; j<nSize2; j++)
	{
		BOOL bExist = FALSE;
		for (int i=0; i<nSize; i++)
		{
			if (j != aKey[i]-1) continue;
			bExist = TRUE;
			break;
		}
		if(!bExist) continue;
		aData.Add(m_pData->arPresProfile[j]);
	}
	

	UpdateBuffer(1, aOldKey, aKey, aData);

	return TRUE;
}

BOOL CCMStaticEarthPressureProfileGrid::ModifyData(int oldKey, int key, T_EPST_PRESSURE_PROFILE& data)
{
	CArray<int, int> aOldKey, aKey;
	CArray<T_EPST_PRESSURE_PROFILE, T_EPST_PRESSURE_PROFILE&> aData;
	aOldKey.Add(oldKey);
	aKey.Add(key);
	aData.Add(data);

	int nPos;
	if (!FindData(oldKey, nPos))
	{
		ASSERT(0);
		return FALSE;
	}
	ASSERT(nPos < m_pData->arPresProfile.GetSize());
	m_pData->arPresProfile[nPos] = data;
	
	UpdateBuffer(2, aOldKey, aKey, aData);

	return TRUE;
}

BOOL CCMStaticEarthPressureProfileGrid::ModifyData(CArray<int, int> &aOldKey, CArray<int, int> &aKey,
	CArray<T_EPST_PRESSURE_PROFILE, T_EPST_PRESSURE_PROFILE&> &aData)
{
	int nPos;

	for (int i = 0; i < aKey.GetCount(); i++)
	{
		if (!FindData(aOldKey[i], nPos))
		{
			ASSERT(0);
			return FALSE;
		}
		
		ASSERT(nPos < m_pData->arPresProfile.GetSize());
		T_EPST_PRESSURE_PROFILE item = aData[i];
		m_pData->arPresProfile.SetAt(nPos, item);
	}

	UpdateBuffer(2, aOldKey, aKey, aData);

	return TRUE;
}

void CCMStaticEarthPressureProfileGrid::UpdateBuffer(int nCmd, CArray<int, int> &aOldKey, CArray<int, int> &aKey,
	CArray<T_EPST_PRESSURE_PROFILE, T_EPST_PRESSURE_PROFILE&> &aData)
{
	// 현재 Row, Col을 저장 : 나중에 복구하기 위함
	ROWCOL ncRow, ncCol;
	GetCurrentCell(ncRow, ncCol);

	BOOL bOldLock = LockUpdate(TRUE);

	int nSize = aKey.GetSize();
	for (int i = 0; i < nSize; i++)
	{
		switch (nCmd)
		{
		case(0):
			// 삽입될 위치(nRow)를 찾아 넣는다.
			InsertRecordByKey(aKey[i], aData[i]);
			break;
		case(1):
			// 삭제될 위치(nRow)를 찾아서 addPressure 값을 0으로 바꾼다
			ModifyRecordByKey(aKey[i], aData[i]);
			break;
		case(2):
			ModifyRecordByKey(aKey[i], aData[i]);
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
	if (m_pParent!= NULL)
		m_pParent->UpdateProfileData();
}

