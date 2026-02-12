// MvldjpItemGrid.cpp : implementation file
// 2002. 7. 19  by TAE
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMMvldItemGrid.h"

#include "..\wg_base\wg_base_CompFunc.h"
#include "..\wg_db\wg_db_UnitCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define EPSILON 1.e-12
#define DATA_FMT _T("%g")

/////////////////////////////////////////////////////////////////////////////
// CCMvldItemJPGrid
// 수정
CString CCMvldItemJPGrid::m_aCurDefVal[] = {_T("0.0"), _T("0.0")};

CCMvldItemJPGrid::CCMvldItemJPGrid()
{
	m_pData = 0;

	AddColInfo(_T("no"), t_real, 0);
	AddColInfo(_LS(IDS_CMD0417__Load_Position), t_real, CUnitCtrl::m_MVLDjp_UNIT.dLocation);
	AddColInfo(_LS(IDS_CMD0417__Load_Value), t_real, CUnitCtrl::m_MVLDjp_UNIT.dLoadValue);

	m_aPrimaryKey.Add(0);
}

CCMvldItemJPGrid::~CCMvldItemJPGrid()
{
}


BEGIN_MESSAGE_MAP(CCMvldItemJPGrid, CTBCommon)
	//{{AFX_MSG_MAP(CCMvldItemJPGrid)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CCMvldItemJPGrid message handlers

void CCMvldItemJPGrid::Initialize(T_MVLDjp_D *pData)
{
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
			.SetVerticalAlignment(DT_VCENTER)
			.SetHorizontalAlignment(DT_RIGHT)
			.SetValueType(GX_VT_NUMERIC)
			.SetFormat(GX_FMT_FIXED)
			.SetPlaces(4));
	SetRowHeightByDPI(0, 0, 25);
	SetStyleRange(CGXRange().SetCols(1), CGXStyle().SetEnabled(FALSE));

	// 수정
	HideColsByIntend(1, 1, TRUE); // no column 숨긴다.

	// 현재 셀 설정
	SetRightValidCell(1, 0);
	GetParam()->SetHideCurrentCell(GX_HIDE_NEVER);

	// Sorting 정보 초기화
	m_aSortInfo.SetSize(1);
	m_aSortInfo[0].nRC = 1;
	m_aSortInfo[0].sortType = CGXSortInfo::numeric;
	m_aSortInfo[0].sortOrder = CGXSortInfo::ascending;

	// Table Name 및 그리드 간격 등 설정
	//m_csTableName = D_TB_TABLE_NODE;
	//ApplyPreference();

	LockUpdate(FALSE);
}

BOOL CCMvldItemJPGrid::InsertRecordByKey(const double &key, const T_MVLDjp_MLOAD_BASE &data)
{
	// 삽입 위치를 찾는다.
	ROWCOL nRow;

	// aKey를 만드는 코드 삽입
	CStringArray aKey;
	MakeSearchKey(key, data, aKey);   // Data 만들기 
	FindRecordByKey(aKey, nRow);      // Data가 삽입될 Row의 위치찾기

	VERIFY(InsertRecord(nRow));       // Insert Record
	return SetValue(nRow, key, data); // Setting Data
}


BOOL CCMvldItemJPGrid::DeleteRecordByKey(const double &key, const T_MVLDjp_MLOAD_BASE &data)
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
// CCMvldItemJPGrid Overridable Functions
void CCMvldItemJPGrid::OnChangedSelectionPublic(long *keys, int nSize)
{
	return;
}

long CCMvldItemJPGrid::GetSelectedKeys(CArray<long, long> &caKey)
{
	return 0;
}

BOOL CCMvldItemJPGrid::ValidateField(CString value, int nColID)
{
	int nErrCode;

	if (nColID == 0 || nColID == 1 || nColID == 2)
	{
		if (!IsValidDoubleNumber(value, nErrCode))
			return InvalidDoubleNumber(nErrCode);
		if (nColID == 2)
		{
			VERIFY(ConvertFormulaToValue(value)); 
			if (_tstof(value) == 0.0) 
			{
				SetWarningText(_LS(IDS_CMD_MVHL_ERROR_ZERO_LOAD));
				return FALSE;
			}
		}
	}

	return TRUE;
}

// 삽입이나 수정전에 Record의 Validation을 검사한다.
BOOL CCMvldItemJPGrid::ValidateRecord(CStringArray &value, CRowColArray &aCols)
{
	CString msg = _T("");
	int nPos;

	for (int i = 0; i < GetColCount(); i++)
	{
		if (i == 0) continue; // no column은 check 안 한다.
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
BOOL CCMvldItemJPGrid::ValidateAndMakeRecord(CStringArray &value, CRowColArray& aCols, double &key, T_MVLDjp_MLOAD_BASE &data)
{
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

BOOL CCMvldItemJPGrid::AddToDB(ROWCOL nRow)
{
	// Record를 만들어서 DB에 입력한다.
	double key;
	T_MVLDjp_MLOAD_BASE data;
 
	GetValue(nRow, key, data);

	return AddData(key, data);
}

BOOL CCMvldItemJPGrid::AddToDB(CArray<void*, void*>& raKey, CArray<void*, void*>& raData)
{
	ASSERT(raKey.GetSize() == raData.GetSize());
	double tKey;
	CArray<double, double> aKey;
	CArray<T_MVLDjp_MLOAD_BASE, T_MVLDjp_MLOAD_BASE&> aData;
	for (int i = 0; i < raKey.GetSize(); i++)
	{
		tKey = *((double*)raKey.GetAt(i));
		aKey.Add(tKey);
		aData.Add(*((T_MVLDjp_MLOAD_BASE*)raData.GetAt(i)));
	}
	return AddData(aKey, aData);
}


BOOL CCMvldItemJPGrid::DeleteFromDB(ROWCOL nRow)
{
	double key;
	T_MVLDjp_MLOAD_BASE data;
 
	GetValue(nRow, key, data);
	return DelData(key);
}

BOOL CCMvldItemJPGrid::DeleteFromDB(CRowColArray& awRows)
{
	if (awRows.GetSize() <= 0) return TRUE;

	ROWCOL nCol;
	VERIFY(GetColumnNoFor(0, nCol));

	CString value;
	CArray<double, double> awKeys;
 
	for (int i = 0; i < awRows.GetSize(); i++)
	{
		value = GetValueRowColBeforeEdit(awRows.GetAt(i), nCol);
		awKeys.Add(_tstof(value));
	}

	return DelData(awKeys);
}

BOOL CCMvldItemJPGrid::ModifyToDB(ROWCOL nRow)
{
	ROWCOL nCol;
	VERIFY(GetColumnNoFor(0, nCol));

	double key, oldKey;
	T_MVLDjp_MLOAD_BASE data;
	
	oldKey = _tstof(GetValueRowColBeforeEdit(nRow, nCol));
	GetValue(nRow, key, data);

	// 수정한다. 실패하면 에러 리턴
	return ModifyData(oldKey, key, data); 
}

BOOL CCMvldItemJPGrid::ModifyToDB(CStringArray& raOldKey, 
												 CArray<void*, void*>& raKey, 
												 CArray<void*, void*>& raData)
{
	ASSERT(raKey.GetSize() == raData.GetSize());
	ASSERT(raKey.GetSize() == raOldKey.GetSize());
	CArray<double, double> aOldKey, aKey;
	CArray<T_MVLDjp_MLOAD_BASE, T_MVLDjp_MLOAD_BASE&> aData;
	double Key;
	int nSize = raKey.GetSize();
	for (int i = 0; i < nSize; i++)
	{
		Key = _tstof(raOldKey.GetAt(i));
		aOldKey.Add(Key);
		aKey.Add(*((double*)raKey.GetAt(i)));
		aData.Add(*((T_MVLDjp_MLOAD_BASE*)raData.GetAt(i)));
	}
	return ModifyData(aOldKey, aKey, aData);
}


CString CCMvldItemJPGrid::GetNewKey()
{
	ROWCOL ncRow, ncCol;
	GetCurrentCell(ncRow, ncCol);
	CString value;
	value.Format(_T("%g"), ncRow-0.5);
	return value;
}

CString CCMvldItemJPGrid::GetDefValue(int nIndex)
{
	return m_aCurDefVal[nIndex];
}

CString* CCMvldItemJPGrid::GetDefValueArray()
{
	return m_aCurDefVal;
}

BOOL CCMvldItemJPGrid::MakeKeyAndDataArray(CStringArray& value, CRowColArray& awCols, 
																	CArray<void*, void*>& aKey, CArray<void*, void*>& aData)
{
	double* pKey;
	T_MVLDjp_MLOAD_BASE* pData;
	pKey = new double;
	pData = new T_MVLDjp_MLOAD_BASE;
	aKey.Add((void*)pKey);
	aData.Add((void*)pData);
	if (!ValidateAndMakeRecord(value, awCols, *pKey, *pData)) return FALSE;
	return TRUE;
}

void CCMvldItemJPGrid::DeleteKeyAndDataArray(CArray<void*, void*>& aKey, CArray<void*, void*>& aData)
{
	double* pKey;
	T_MVLDjp_MLOAD_BASE* pData;
	ASSERT(aKey.GetSize() == aData.GetSize());
	for (int i = 0; i < aKey.GetSize(); i++)
	{
		pKey = (double*)aKey.GetAt(i);
		pData = (T_MVLDjp_MLOAD_BASE*)aData.GetAt(i);
		delete pKey;
		delete pData;
	}
}

// 수정
long CCMvldItemJPGrid::GetCountData()
{
	int i = 0;
	for (i = 0; i < T_MVLDjp_NUM_MVLDjp_MLOAD; i++)
		if (m_pData->MLoad.Load[i].dLoadValue == 0.0) break;
	return i;
}

void CCMvldItemJPGrid::MakeItemFiltered(long nFilteredNum, long* aFilteredKeys, CRowColArray& awCols)
{
	ASSERT(0); // no filtering
}

void CCMvldItemJPGrid::MakeItemNormal(long nNumData, CRowColArray& awCols)
{
	double Key;
	T_MVLDjp_MLOAD_BASE Data;

	long nCount;
	for(nCount=0 ; nCount < nNumData; nCount++)
	{
		Key = (double)nCount+1;
	// 수정
		Data.dLocation = m_pData->MLoad.Load[nCount].dLocation;
		Data.dLoadValue = m_pData->MLoad.Load[nCount].dLoadValue;

		SetValue(GetParam()->GetData(), nCount+1, Key, Data, awCols);
	} // end of loop nCount
}

void CCMvldItemJPGrid::MakeItemEditingRecord(ROWCOL ncRow, CStringArray& EditingRecord)
{
	CDBDoc* pDoc = m_pDoc;
	double Key;
	T_MVLDjp_MLOAD_BASE Data;
	for (int i = 0; i < GetColCount(); i++)
	{
		if (i == 0) Key = _tstof(EditingRecord[i]);
		else ConvStrToData(i, EditingRecord[i], Data);
	}
	// 현재 값을 설정한다.
	SetValue(ncRow, Key, Data);
}

/////////////////////////////////////////////////////////////////////////////
// CCMvldItemJPGrid Implementation
BOOL CCMvldItemJPGrid::GetValue(ROWCOL nRow, double &key, T_MVLDjp_MLOAD_BASE &data)
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
			key = _tstof(str);
			if (key == 0.0) // 입력 안된 경우
				key = (double)nRow - 0.5;
		}
		else ConvStrToData(i, str, data);
	}
	return TRUE;
}

BOOL CCMvldItemJPGrid::SetValue(ROWCOL nRow, const double &key, const T_MVLDjp_MLOAD_BASE &data)
{
	CRowColArray awCols;
	awCols.SetSize(GetColCount());
	for (int i = 0;i < GetColCount(); i++)
		VERIFY(GetColumnNoFor(m_aTitle[i], awCols[i]));
	return SetValue(GetParam()->GetData(), nRow, key, data, awCols);
}

BOOL CCMvldItemJPGrid::SetValue(CGXData* pData, ROWCOL nRow, const double &key, const T_MVLDjp_MLOAD_BASE &data, CRowColArray &awCols)
{
	CString str;
	for (int i = 0; i < GetColCount(); i++)
	{
		if(i==0) str.Format(DATA_FMT, key);
		else ConvDataToStr(i, data, str);
		pData->StoreValueRowCol(nRow, awCols[i], str, gxOverride, GX_VT_NUMERIC);
	}
	return TRUE;
}

// String을 Data로 변환
void CCMvldItemJPGrid::ConvStrToData(int i, CString& value, T_MVLDjp_MLOAD_BASE& data)
{
	if (i==1) data.dLocation = _tstof(value);
	else if (i==2) data.dLoadValue = _tstof(value);
}

// Data를 String으로 변환
void CCMvldItemJPGrid::ConvDataToStr(int i, const T_MVLDjp_MLOAD_BASE& data, CString& value)
{
	if (i==1) value.Format(DATA_FMT, data.dLocation);
	else if (i==2) value.Format(DATA_FMT, data.dLoadValue);
}

// 한줄씩(each row) Data를 만드는 함수(String으로)
void CCMvldItemJPGrid::MakeSearchKey(const double &key, const T_MVLDjp_MLOAD_BASE &data, CStringArray &aKey)
{
	int nColID;
	int nSortKeyNum = m_aSortInfo.GetSize();

	aKey.SetSize(nSortKeyNum);
	for (int i = 0; i < nSortKeyNum; i++)
	{
		nColID = m_aSortInfo[i].nRC;
		if (nColID == 0) aKey[i].Format(DATA_FMT, key);
		else ConvDataToStr(nColID, data, aKey[i]);
	}
}

void CCMvldItemJPGrid::SetHeaderTitle(T_UNIT_SYSTEM &us, BOOL bInit)
{
	// Set Title
	// 수정
	int aSize[] = {30, 90, 90};
	int nColCount = GetColCount();

	CString csUnit, csTitle;
	for (int i = 0; i < nColCount; i++)
	{
		CUnitCtrl::GetUnitSystem(m_aUnit[i], csUnit);
		if (csUnit.IsEmpty()) csTitle = m_aTitle[i];
		csTitle = m_aTitle[i] + _T("(")+csUnit+_T(")");
		SetStyleRange(CGXRange(0, i+1), CGXStyle()
			.SetValue(csTitle)
			.SetUserAttribute(ID_USER_ATTR_COL_ID, m_aTitle[i]));
		if (bInit) SetColWidthByDPI(i+1, i+1, aSize[i]);
	}
	if (bInit) SetColWidthByDPI(0, 0, 30);
}

BOOL CCMvldItemJPGrid::FindData(double key, int& nPos, int nStart)
{
	int i;
	int nSize = GetCountData();
	
	CString csVal;
	double dblVal;
	double TempKey;
	int nCompResult;
	for (i = nStart; i < nSize; i++)
	{
		TempKey = i+1;
		csVal.Format(_T("%g"), TempKey);
		dblVal = _tstof(csVal);
		nCompResult = CCompFunc::FastCompDouble(key, dblVal);
		if (nCompResult > 0) continue;    // key > value
		else if (nCompResult < 0) break;  // key < value
		else                              // key == value
		{
			nPos = i;
			return TRUE;;
		}
	}
	nPos = i;
	return FALSE;
}

BOOL CCMvldItemJPGrid::ExistAllKey(CArray<double, double>& aKey, double& errorKey)
{
	CArray<double, double> aTempKey;

	int nSize = aKey.GetSize();
	aTempKey.Copy(aKey);
	BOOL bSame;
	int nPos=0, nStart;
	for (int i = 0; i < nSize; i++)
	{
		nStart = nPos;
		bSame = FindData(aTempKey[i], nPos, nStart);
		if (!bSame)
		{
			errorKey = aTempKey[i];
			return FALSE;
		}
		nPos = nStart;
	}
	return TRUE;
}

BOOL CCMvldItemJPGrid::AddData(double key, T_MVLDjp_MLOAD_BASE data)
{
	CArray<double, double> aOldKey, aKey;
	CArray<T_MVLDjp_MLOAD_BASE, T_MVLDjp_MLOAD_BASE&> aData;

	if (key == 0.0) key = GetCountData()+0.5;

	aKey.Add(key);
	aData.Add(data);
	T_MVLDjp_MLOAD_BASE item;
	item = data;

	// 동일한 key값 중복 검토
	int nPos;
	if (FindData(key, nPos, 0))
	{
		CString msg;
		msg.Format(_LS(IDS_CMD0417__x__g_already_exists), key);
		AfxMessageBox(msg);
		return FALSE;
	}
	if (!InsertDataAt(nPos, item)) return FALSE;
	UpdateBuffer(0, aOldKey, aKey, aData);
	DBChanged();

	return TRUE;
}

BOOL CCMvldItemJPGrid::AddData(CArray<double, double> &aKey, CArray<T_MVLDjp_MLOAD_BASE, T_MVLDjp_MLOAD_BASE&> &aData, BOOL bNotify)
{
	CArray<double, double> aOldKey;

	int nCountData = GetCountData();
	for (int n = 0; n < aKey.GetSize(); n++)
		if (aKey[n] == 0.0) aKey[n] = nCountData+n+0.5;

	// 동일한 key값 중복 검토
	int nSize = aKey.GetSize();
	aOldKey.Copy(aKey);
	qsort((void*)aOldKey.GetData(), nSize, sizeof(double), CCompFunc::doubleAsc);

	BOOL bSame;
	int nPos=0, nStart;
	for (int i = 0; i < nSize; i++)
	{
		nStart = nPos;
		bSame = FindData(aOldKey[i], nPos, nStart);
		if (!bSame && i > 0)
			bSame = (aOldKey[i]-aOldKey[i-1] <= EPSILON);
		if (bSame)
		{
			CString msg;
			msg.Format(_LS(IDS_CMD0417__x__g_already_exists), aOldKey[i]);
			AfxMessageBox(msg);
			return FALSE;
		}
		nPos = nStart;
	}
	for (int i = 0; i < nSize; i++)
	{
		bSame = FindData(aKey[i], nPos, 0);
		VERIFY(!bSame);

		T_MVLDjp_MLOAD_BASE item;
		item = aData[i];
		if (!InsertDataAt(nPos, item)) return FALSE;
	}
	UpdateBuffer(0, aOldKey, aKey, aData);
	if (bNotify) DBChanged();

	return TRUE;
}

BOOL CCMvldItemJPGrid::DelData(double key)
{
	CArray<double, double> aOldKey, aKey;
	CArray<T_MVLDjp_MLOAD_BASE, T_MVLDjp_MLOAD_BASE&> aData;
	T_MVLDjp_MLOAD_BASE data;
	aKey.Add(key);
	aData.Add(data);

	int nPos;
	if (!FindData(key, nPos))
	{
		CString msg;
		msg.Format(_LS(IDS_CMD0417__x__g_does_not_exist), key);
		AfxMessageBox(msg);
		return FALSE;
	}
	if (!RemoveDataAt(nPos)) return FALSE;
	UpdateBuffer(1, aOldKey, aKey, aData);
	DBChanged();

	return TRUE;
}

BOOL CCMvldItemJPGrid::DelData(CArray<double, double> &aKey, BOOL bNotify)
{
	CArray<double, double> aOldKey;
	CArray<T_MVLDjp_MLOAD_BASE, T_MVLDjp_MLOAD_BASE&> aData;
	T_MVLDjp_MLOAD_BASE data;
	int nSize = aKey.GetSize();
	for (int i = 0; i < nSize; i++) aData.Add(data);

	aOldKey.Copy(aKey);
	qsort((void*)aOldKey.GetData(), nSize, sizeof(double), CCompFunc::doubleAsc);
	// 동일한 key값 중복 검토
	double errKey;
	if (!ExistAllKey(aOldKey, errKey))
	{
		CString msg;
		msg.Format(_LS(IDS_CMD0417__x__g_does_not_exist), errKey);
		AfxMessageBox(msg);
		return FALSE;
	}
	int nSize2 = GetCountData();
	int j = nSize2-1;
	double TempKey;
	double dblDiff;
	for (int i = nSize-1; i >= 0; i--)
	{
		for (; j >= 0; j--)
		{
			TempKey = j+1;
			dblDiff = aOldKey[i] - TempKey;
			if (dblDiff >= -EPSILON && dblDiff <= EPSILON) 
			{
				if (!RemoveDataAt(j)) return FALSE;
				j--;
				break;
			}
		}
	}

	UpdateBuffer(1, aOldKey, aKey, aData);

	if (bNotify) DBChanged();

	return TRUE;
}

BOOL CCMvldItemJPGrid::ModifyData(double oldKey, double key, T_MVLDjp_MLOAD_BASE data)
{
	CArray<double, double> aOldKey, aKey;
	CArray<T_MVLDjp_MLOAD_BASE, T_MVLDjp_MLOAD_BASE&> aData;

	if (key != oldKey) key = oldKey;

	aOldKey.Add(oldKey);
	aKey.Add(key);
	aData.Add(data);

	int nPos;
	if (!FindData(oldKey, nPos))
	{
		CString msg;
		msg.Format(_LS(IDS_CMD0417__Old_x__g_does_not_exist), oldKey);
		AfxMessageBox(msg);
		return FALSE;
	}
	if (oldKey == key)
	{
		if (!ChangeDataAt(nPos, data)) return FALSE;
	}
	else
	{
		int nPos2;
		if (FindData(key, nPos2))
		{
			CString msg;
			msg.Format(_LS(IDS_CMD0417__New_x__g_already_exists), key);
			AfxMessageBox(msg);
			return FALSE;
		}
		T_MVLDjp_MLOAD_BASE item;
		item = data;
		if (!RemoveDataAt(nPos)) return FALSE;
		if (nPos2 > nPos) nPos2--;
		if (!InsertDataAt(nPos2, item)) return FALSE;
	}

	UpdateBuffer(2, aOldKey, aKey, aData);

	DBChanged();

	return TRUE;
}

BOOL CCMvldItemJPGrid::ModifyData(CArray<double, double> &aOldKey, CArray<double, double> &aKey, CArray<T_MVLDjp_MLOAD_BASE, T_MVLDjp_MLOAD_BASE&> &aData)
{
	T_MVLDjp_MLOAD_BASE aOrgData[T_MVLDjp_NUM_MVLDjp_MLOAD];
	SaveData(aOrgData);

	int nPos;
	for (int i = 0; i < aOldKey.GetSize(); i++)
	{
		aKey[i] = aOldKey[i];
		if (!FindData(aOldKey[i], nPos))
		{
			CString msg;
			msg.Format(_LS(IDS_CMD0417__Old_x__g_does_not_exist), aOldKey[i]);
			AfxMessageBox(msg);
			RestoreData(aOrgData);
			return FALSE;
		}
		if (!ChangeDataAt(nPos, aData[i])) 
		{
			RestoreData(aOrgData);
			return FALSE;
		}
	}
	
	UpdateBuffer(2, aOldKey, aKey, aData);
	DBChanged();

	return TRUE;
}

void CCMvldItemJPGrid::UpdateBuffer(int nCmd, CArray<double, double> &aOldKey, CArray<double, double> &aKey, CArray<T_MVLDjp_MLOAD_BASE, T_MVLDjp_MLOAD_BASE&> &aData)
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

	AfterUpdateBuffer();

	LockUpdate(bOldLock);
	if (!bOldLock) Redraw();
}

// 수정
void CCMvldItemJPGrid::DBChanged()
{
	// Nothing to do
}

// 수정
BOOL CCMvldItemJPGrid::InsertDataAt(int nzbPos, T_MVLDjp_MLOAD_BASE& data)
{
	int nDataCount = GetCountData();
	if (nDataCount == T_MVLDjp_NUM_MVLDjp_MLOAD) 
	{
		CString csMsg;
		csMsg.Format(_LS(IDS_CMD0417__Error___Can_t_add_more_than__d_items), T_MVLDjp_NUM_MVLDjp_MLOAD);
		AfxMessageBox(csMsg);
		return FALSE;
	}
	if (nzbPos < 0 || nzbPos > nDataCount) 
	{
		ASSERT(0);
		AfxMessageBox(_LS(IDS_CMD0417__Error___Can_t_find_insertion_position));
		return FALSE;
	}
	for (int i = nDataCount; i >= nzbPos+1; i--)
	{
		m_pData->MLoad.Load[i].dLocation = m_pData->MLoad.Load[i-1].dLocation; 
		m_pData->MLoad.Load[i].dLoadValue = m_pData->MLoad.Load[i-1].dLoadValue; 
	}
	m_pData->MLoad.Load[nzbPos].dLocation = data.dLocation; 
	m_pData->MLoad.Load[nzbPos].dLoadValue = data.dLoadValue; 
	
	return TRUE;
}

// 수정
BOOL CCMvldItemJPGrid::RemoveDataAt(int nzbPos)
{
	int nDataCount = GetCountData();
	if (nzbPos < 0 || nzbPos >= nDataCount)
	{
		ASSERT(0);
		AfxMessageBox(_LS(IDS_CMD0417__Error___Can_t_find_item_to_delete));
		return FALSE;
	}
	for (int i = nzbPos; i < nDataCount-1; i++)
	{
		m_pData->MLoad.Load[i].dLocation = m_pData->MLoad.Load[i+1].dLocation; 
		m_pData->MLoad.Load[i].dLoadValue = m_pData->MLoad.Load[i+1].dLoadValue; 
	}
	m_pData->MLoad.Load[nzbPos].dLocation = 0.0; 
	m_pData->MLoad.Load[nzbPos].dLoadValue = 0.0; 
	
	return TRUE;
}

// 수정
BOOL CCMvldItemJPGrid::ChangeDataAt(int nzbPos, T_MVLDjp_MLOAD_BASE& data)
{
	int nDataCount = GetCountData();
	if (nzbPos < 0 || nzbPos >= nDataCount)
	{
		ASSERT(0);
		AfxMessageBox(_LS(IDS_CMD0417__Error___Can_t_find_item_to_change));
		return FALSE;
	}
	m_pData->MLoad.Load[nzbPos].dLocation = data.dLocation; 
	m_pData->MLoad.Load[nzbPos].dLoadValue = data.dLoadValue; 

	return TRUE;
}

// 수정
BOOL CCMvldItemJPGrid::SaveData(T_MVLDjp_MLOAD_BASE aData[])
{
	for (int i = 0; i < T_MVLDjp_NUM_MVLDjp_MLOAD; i++)
	{
		aData[i].dLocation = m_pData->MLoad.Load[i].dLocation;
		aData[i].dLoadValue = m_pData->MLoad.Load[i].dLoadValue;
	}
	return TRUE;
}

// 수정
BOOL CCMvldItemJPGrid::RestoreData(T_MVLDjp_MLOAD_BASE aData[])
{
	for (int i = 0; i < T_MVLDjp_NUM_MVLDjp_MLOAD; i++)
	{
		m_pData->MLoad.Load[i].dLocation = aData[i].dLocation;
		m_pData->MLoad.Load[i].dLoadValue = aData[i].dLoadValue;
	}
	return TRUE;
}

void CCMvldItemJPGrid::AfterUpdateBuffer()
{
	// Update 완료되면 key를 다시 renumbering해 준다.
	BOOL bOldLock = LockUpdate(TRUE);

	CString csValue;
	ROWCOL nkCol;
	GetColumnNoFor(0, nkCol);
	double dValue;
	for (int i = 0; i < GetRowCount()-1; i++)
	{
		csValue = GetValueRowCol(i+1, nkCol);
		dValue = _tstof(csValue);
		if (dValue - ceil(dValue) != 0) // 편집중인 것
			SetValueRange(CGXRange(i+1, nkCol), ceil(_tstof(csValue)));
		else if (i == 0) SetValueRange(CGXRange(i+1, nkCol), 1.0);
		else
		{
			csValue = GetValueRowCol(i, nkCol);
			SetValueRange(CGXRange(i+1, nkCol), _tstof(csValue)+1);
		}
	}

	LockUpdate(bOldLock);
	if (!bOldLock) Redraw();
}

