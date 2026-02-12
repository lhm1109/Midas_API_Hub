// ThfcItemGrid.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "ThfcItemGrid.h"

#include "..\wg_base\wg_base_CompFunc.h"
#include "..\wg_db\wg_db_UnitCtrl.h"

#include "ThfcItemDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define EPSILON 1.e-12
#define DATA_FMT _T("%.15g")

/////////////////////////////////////////////////////////////////////////////
// CThfcItemGrid
CString CThfcItemGrid::m_aCurDefVal[] = {_T("0.0"), _T("0.0")};

CThfcItemGrid::CThfcItemGrid()
{
	m_pData = 0;

	AddColInfo(_LS(IDS_CMD_THFC_time),      t_real, 0);
	AddColInfo(_LS(IDS_CMD_THFC_Function),  t_real, 0);

//18japan	 AddColInfo(_T("Time"),      t_real, 0);
//18japan  AddColInfo(_T("Function"),  t_real, 0);

	m_aPrimaryKey.Add(0);
}

CThfcItemGrid::~CThfcItemGrid()
{
}


BEGIN_MESSAGE_MAP(CThfcItemGrid, CTBCommon)
	//{{AFX_MSG_MAP(CThfcItemGrid)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CThfcItemGrid message handlers

void CThfcItemGrid::Initialize(T_THFC_D *pData, const int nValFmtPlace/*=4*/)
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

    SetStyleRange(CGXRange().SetCols(2), CGXStyle()
        .SetPlaces(nValFmtPlace));
    
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

BOOL CThfcItemGrid::InsertRecordByKey(const double &key, const double &data)
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

BOOL CThfcItemGrid::DeleteRecordByKey(const double &key, const double &data)
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
// CThfcItemGrid Overridable Functions
void CThfcItemGrid::OnChangedSelectionPublic(long *keys, int nSize)
{
	return;
}

long CThfcItemGrid::GetSelectedKeys(CArray<long, long> &caKey)
{
	return 0;
}

BOOL CThfcItemGrid::ValidateField(CString value, int nColID)
{
	if (nColID >= GetColCount())
	{
		SetWarningText(_LS(IDS_WG_CMD__ADDD___CThfcItemGrid__ValidateField__Er));
		return FALSE;
	}

	int nErrCode;
	if (nColID == 0 || nColID == 1)
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
		if (nColID == 0 && _tstof(value) < 0.0)
		{
			SetWarningText(_LS(IDS_WG_CMD__ADDD__Negative_value_is_not_allowed_));
			return FALSE;
		}
		return TRUE;
	}
	else return FALSE;

	return TRUE;
}

// 삽입이나 수정전에 Record의 Validation을 검사한다.
BOOL CThfcItemGrid::ValidateRecord(CStringArray &value, CRowColArray &aCols)
{
	CString msg = _T("");
	int nPos;

	for (int i = 0; i < GetColCount(); i++)
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
BOOL CThfcItemGrid::ValidateAndMakeRecord(CStringArray &value, CRowColArray& aCols, double &key, double &data)
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

BOOL CThfcItemGrid::AddToDB(ROWCOL nRow)
{
	// Record를 만들어서 DB에 입력한다.
	double key;
	double data;
 
	GetValue(nRow, key, data);

	return AddData(key, data);
}

BOOL CThfcItemGrid::AddToDB(CArray<void*, void*>& raKey, CArray<void*, void*>& raData)
{
	ASSERT(raKey.GetSize() == raData.GetSize());
	double tKey;
	CArray<double, double> aKey;
	CArray<double, double> aData;
	for (int i = 0; i < raKey.GetSize(); i++)
	{
		tKey = *((double*)raKey.GetAt(i));
		aKey.Add(tKey);
		aData.Add(*((double*)raData.GetAt(i)));
	}
	return AddData(aKey, aData);
}


BOOL CThfcItemGrid::DeleteFromDB(ROWCOL nRow)
{
	double key;
	double data;
 
	GetValue(nRow, key, data);
	return DelData(key);
}

BOOL CThfcItemGrid::DeleteFromDB(CRowColArray& awRows)
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

BOOL CThfcItemGrid::ModifyToDB(ROWCOL nRow)
{
	ROWCOL nCol;
	VERIFY(GetColumnNoFor(0, nCol));

	double key, oldKey;
	double data;
	
	oldKey = _tstof(GetValueRowColBeforeEdit(nRow, nCol));
	GetValue(nRow, key, data);

	// 수정한다. 실패하면 에러 리턴
	return ModifyData(oldKey, key, data); 
}

BOOL CThfcItemGrid::ModifyToDB(CStringArray& raOldKey, 
												 CArray<void*, void*>& raKey, 
												 CArray<void*, void*>& raData)
{
	ASSERT(raKey.GetSize() == raData.GetSize());
	ASSERT(raKey.GetSize() == raOldKey.GetSize());
	CArray<double, double> aOldKey, aKey, aData;
	double Key;
	int nSize = raKey.GetSize();
	for (int i = 0; i < nSize; i++)
	{
		Key = _tstof(raOldKey.GetAt(i));
		aOldKey.Add(Key);
		aKey.Add(*((double*)raKey.GetAt(i)));
		aData.Add(*((double*)raData.GetAt(i)));
	}
	return ModifyData(aOldKey, aKey, aData);
}


CString CThfcItemGrid::GetNewKey()
{
	CString value(_T("0"));
	return value;
}

CString CThfcItemGrid::GetDefValue(int nIndex)
{
	return m_aCurDefVal[nIndex];
}

CString* CThfcItemGrid::GetDefValueArray()
{
	return m_aCurDefVal;
}

BOOL CThfcItemGrid::MakeKeyAndDataArray(CStringArray& value, CRowColArray& awCols, 
																	CArray<void*, void*>& aKey, CArray<void*, void*>& aData)
{
	double* pKey;
	double* pData;
	pKey = new double;
	pData = new double;
	aKey.Add((void*)pKey);
	aData.Add((void*)pData);
	if (!ValidateAndMakeRecord(value, awCols, *pKey, *pData)) return FALSE;
	return TRUE;
}

void CThfcItemGrid::DeleteKeyAndDataArray(CArray<void*, void*>& aKey, CArray<void*, void*>& aData)
{
	double* pKey;
	double* pData;
	ASSERT(aKey.GetSize() == aData.GetSize());
	for (int i = 0; i < aKey.GetSize(); i++)
	{
		pKey = (double*)aKey.GetAt(i);
		pData = (double*)aData.GetAt(i);
		delete pKey;
		delete pData;
	}
}

long CThfcItemGrid::GetCountData()
{
	return m_pData->arThisFuncData.GetSize();
}

void CThfcItemGrid::MakeItemFiltered(long nFilteredNum, long* aFilteredKeys, CRowColArray& awCols)
{
	ASSERT(0); // no filtering
}

void CThfcItemGrid::MakeItemNormal(long nNumData, CRowColArray& awCols)
{
	double Key;
	double Data;

	long nCount;
	for(nCount=0 ; nCount < nNumData; nCount++)
	{
		Key = m_pData->arThisFuncData[nCount].dblTime;
		Data = m_pData->arThisFuncData[nCount].dblValue;

		SetValue(GetParam()->GetData(), nCount+1, Key, Data, awCols);
	} // end of loop nCount
}

void CThfcItemGrid::MakeItemEditingRecord(ROWCOL ncRow, CStringArray& EditingRecord)
{
	CDBDoc* pDoc = m_pDoc;
	double Key;
	double Data;
	for (int i = 0; i < GetColCount(); i++)
	{
		if (i == 0) Key = _tstof(EditingRecord[i]);
		else ConvStrToData(i, EditingRecord[i], Data);
	}
	// 현재 값을 설정한다.
	SetValue(ncRow, Key, Data);
}

/////////////////////////////////////////////////////////////////////////////
// CThfcItemGrid Implementation
BOOL CThfcItemGrid::GetValue(ROWCOL nRow, double &key, double &data)
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

BOOL CThfcItemGrid::SetValue(ROWCOL nRow, const double &key, const double &data)
{
	//
	CRowColArray awCols;
	awCols.SetSize(GetColCount());
	for (int i = 0;i < GetColCount(); i++)
		VERIFY(GetColumnNoFor(m_aTitle[i], awCols[i]));
	return SetValue(GetParam()->GetData(), nRow, key, data, awCols);
}

BOOL CThfcItemGrid::SetValue(CGXData* pData, ROWCOL nRow, const double &key, const double &data, CRowColArray &awCols)
{
	//
	CString str;
	for (int i = 0; i < GetColCount(); i++)
	{
		if(i==0) str.Format(DATA_FMT,key);
		else ConvDataToStr(i, data, str);
		pData->StoreValueRowCol(nRow, awCols[i], str, gxOverride, GX_VT_NUMERIC);
	}

	return TRUE;
}

void CThfcItemGrid::ConvStrToData(int i, CString& value, double& data)
{
	if (i==1) data = _tstof(value);
}

void CThfcItemGrid::ConvDataToStr(int i, const double& data, CString& value)
{
	if (i==1) value.Format(DATA_FMT, data);
}

void CThfcItemGrid::MakeSearchKey(const double &key, const double &data, CStringArray &aKey)
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

void CThfcItemGrid::SetHeaderTitle(T_UNIT_SYSTEM &us, BOOL bInit)
{
	// Set Title
	int nDataType = m_pData->nThisDataType;
	if (nDataType > 0) nDataType--;

	ASSERT(nDataType >= 0 && nDataType <= 4);

	CString csTitle, csUnit;
	int aUnit[][2] = {
		{CUnitCtrl::m_THFC_UNIT.dblTime, CUnitCtrl::m_THFC_UNIT.dblValue[0]}, 
		{CUnitCtrl::m_THFC_UNIT.dblTime, CUnitCtrl::m_THFC_UNIT.dblValue[1]}, 
		{CUnitCtrl::m_THFC_UNIT.dblTime, CUnitCtrl::m_THFC_UNIT.dblValue[2]}, 
		{CUnitCtrl::m_THFC_UNIT.dblTime, CUnitCtrl::m_THFC_UNIT.dblValue[3]}, 
		{CUnitCtrl::m_THFC_UNIT.dblTime, CUnitCtrl::m_THFC_UNIT.dblValue[4]},  // added by KYM(2002.5.4)
	};

	int aSize[] = {60, 85};
	int nColCount = GetColCount();

	for (int i = 0; i < nColCount; i++)
	{
		m_aUnit.SetAt(i, aUnit[nDataType][i]);
		CUnitCtrl::GetUnitSystem(aUnit[nDataType][i], csUnit);
		if (csUnit.IsEmpty()) 
		{
			if (nDataType == 0) csTitle = m_aTitle[i] + _T("\n(g)");
			else csTitle = m_aTitle[i];
		}
		else csTitle = m_aTitle[i] + _T("\n(")+csUnit+_T(")");
		SetStyleRange(CGXRange(0, i+1), CGXStyle()
			.SetValue(csTitle)
			.SetUserAttribute(ID_USER_ATTR_COL_ID, m_aTitle[i]));
		if (bInit) SetColWidthByDPI(i+1, i+1, aSize[i]);
	}
	if (bInit) SetColWidthByDPI(0, 0, 35);
}

BOOL CThfcItemGrid::FindData(double key, int& nPos, int nStart)
{
	int i;
	int nSize = m_pData->arThisFuncData.GetSize();
	
	CString csVal;
	double dblVal;
	int nCompResult;
	for (i = nStart; i < nSize; i++)
	{
		csVal.Format(DATA_FMT, m_pData->arThisFuncData[i].dblTime);
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

BOOL CThfcItemGrid::ExistAllKey(CArray<double, double>& aKey, double& errorKey)
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

BOOL CThfcItemGrid::AddData(double key, double data)
{
	CArray<double, double> aOldKey, aKey, aData;
	aKey.Add(key);
	aData.Add(data);
	T_THFC_BASE item;
	item.dblTime = key;
	item.dblValue = data;

	// 동일한 key값 중복 검토
	int nPos;
	if (FindData(key, nPos, 0))
	{
		CString msg;
		msg.Format(_LS(IDS_WG_CMD__ADDD__Time__g_already_exist_), key);
		AfxMessageBox(msg);
		return FALSE;
	}
	m_pData->arThisFuncData.InsertAt(nPos, item);
	UpdateBuffer(0, aOldKey, aKey, aData);
	DBChanged();
	return TRUE;
}

BOOL CThfcItemGrid::AddData(CArray<double, double> &aKey, CArray<double, double> &aData, BOOL bNotify)
{
	CArray<double, double> aOldKey;

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
			msg.Format(_LS(IDS_WG_CMD__ADDD__Time__g_already_exist_), aOldKey[i]);
			AfxMessageBox(msg);
			return FALSE;
		}
		nPos = nStart;
	}
	for (int i = 0; i < nSize; i++)
	{
		bSame = FindData(aKey[i], nPos, 0);
		VERIFY(!bSame);

		T_THFC_BASE item;
		item.dblTime = aKey[i];
		item.dblValue = aData[i];
		m_pData->arThisFuncData.InsertAt(nPos, item);
	}
	UpdateBuffer(0, aOldKey, aKey, aData);

	if (bNotify) DBChanged();
	return TRUE;
}

BOOL CThfcItemGrid::DelData(double key)
{
	CArray<double, double> aOldKey, aKey, aData;
	aKey.Add(key);
	aData.Add(0.0);

	int nPos;
	if (!FindData(key, nPos))
	{
		CString msg;
		msg.Format(_LS(IDS_WG_CMD__ADDD__Time__g_does_not_exist_), key);
		AfxMessageBox(msg);
		return FALSE;
	}
	m_pData->arThisFuncData.RemoveAt(nPos);
	UpdateBuffer(1, aOldKey, aKey, aData);
	DBChanged();
	return TRUE;
}

BOOL CThfcItemGrid::DelData(CArray<double, double> &aKey, BOOL bNotify)
{
	CArray<double, double> aOldKey, aData;
	int nSize = aKey.GetSize();
	for (int i = 0; i < nSize; i++) aData.Add(0.0);

	aOldKey.Copy(aKey);
	qsort((void*)aOldKey.GetData(), nSize, sizeof(double), CCompFunc::doubleAsc);
	// 동일한 key값 중복 검토
	double errKey;
	if (!ExistAllKey(aOldKey, errKey))
	{
		CString msg;
		msg.Format(_LS(IDS_WG_CMD__ADDD__Time__g_does_not_exist_), errKey);
		AfxMessageBox(msg);
		return FALSE;
	}
	int nSize2 = m_pData->arThisFuncData.GetSize();
	int j = nSize2-1;
	double dblDiff;
	for (int i = nSize-1; i >= 0; i--)
	{
		for (; j >= 0; j--)
		{
			dblDiff = aOldKey[i] - m_pData->arThisFuncData[j].dblTime;
			if (dblDiff >= -EPSILON && dblDiff <= EPSILON) 
			{
				m_pData->arThisFuncData.RemoveAt(j);
				j--;
				break;
			}
		}
	}

	UpdateBuffer(1, aOldKey, aKey, aData);

	if (bNotify) DBChanged();
	return TRUE;
}

BOOL CThfcItemGrid::ModifyData(double oldKey, double key, double data)
{
	CArray<double, double> aOldKey, aKey, aData;
	aOldKey.Add(oldKey);
	aKey.Add(key);
	aData.Add(data);

	int nPos;
	if (!FindData(oldKey, nPos))
	{
		CString msg;
		msg.Format(_LS(IDS_WG_CMD__ADDD__Old_Time__g_does_not_exist_), oldKey);
		AfxMessageBox(msg);
		return FALSE;
	}
	if (oldKey == key)
	{
		m_pData->arThisFuncData[nPos].dblValue = data;
	}
	else
	{
		int nPos2;
		if (FindData(key, nPos2))
		{
			CString msg;
			msg.Format(_LS(IDS_WG_CMD__ADDD__New_Time__g_already_exists_), key);
			AfxMessageBox(msg);
			return FALSE;
		}
		T_THFC_BASE item;
		item.dblTime = key;
		item.dblValue = data;
		m_pData->arThisFuncData.RemoveAt(nPos);
		if (nPos2 > nPos) nPos2--;
		m_pData->arThisFuncData.InsertAt(nPos2, item);
	}

	UpdateBuffer(2, aOldKey, aKey, aData);

	DBChanged();
	return TRUE;
}

BOOL CThfcItemGrid::ModifyData(CArray<double, double> &aOldKey, CArray<double, double> &aKey, CArray<double, double> &aData)
{
	CArray<T_THFC_BASE, T_THFC_BASE&> aOrgData;
	aOrgData.Copy(m_pData->arThisFuncData);
	
	if (!DelData(aOldKey, FALSE))
		return FALSE;
	if (!AddData(aKey, aData, FALSE))
	{
		m_pData->arThisFuncData.Copy(aOrgData);
		return FALSE;
	}
	DBChanged();
	return TRUE;
}

void CThfcItemGrid::UpdateBuffer(int nCmd, CArray<double, double> &aOldKey, CArray<double, double> &aKey, CArray<double, double> &aData)
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

	LockUpdate(bOldLock);
	if (!bOldLock) Redraw();
}

void CThfcItemGrid::DBChanged()
{
	CThfcItemDlg* pDlg = (CThfcItemDlg*)GetParent();
	if (!pDlg) return;
	if (pDlg->GetSafeHwnd() == 0) return;
	if (!IsWindow(pDlg->GetSafeHwnd())) return;

	pDlg->OnDataChanged();
}
