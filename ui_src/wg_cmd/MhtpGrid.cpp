// EtfcGrid.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "MhtpGrid.h"

#include "..\wg_base\wg_base_CompFunc.h"
#include "..\wg_db\wg_db_UnitCtrl.h"

#include "TemperatureRisePropItemDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define EPSILON 1.e-12
#define DATA_FMT _T("%g")

/////////////////////////////////////////////////////////////////////////////
// CMhtpGrid
CString CMhtpGrid::m_aCurDefVal[] = {_T("0"), _T("0.0"), _T("0.0")};
CMhtpGrid::CMhtpGrid()
{
	m_pData = 0;

	AddColInfo(_LS(IDS_CMD_MHTP_temperature),       t_real, 0);
	AddColInfo(_LS(IDS_CMD_MHTP_strength),       t_real, 0);
	AddColInfo(_LS(IDS_CMD_MHTP_Elasticity),       t_real, 0);

	m_aPrimaryKey.Add(0);
}

CMhtpGrid::~CMhtpGrid()
{
}


BEGIN_MESSAGE_MAP(CMhtpGrid, CTBCommon)
	//{{AFX_MSG_MAP(CMhtpGrid)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CMhtpGrid message handlers

void CMhtpGrid::Initialize(T_MHTP_D *pData)
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
			.SetFormat(GX_FMT_FIXED)
			.SetPlaces(3));

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

/////////////////////////////////////////////////////////////////////////////
// CMhtpGrid Overridable Functions

BOOL CMhtpGrid::ValidateField(CString value, int nColID)
{
	if (nColID >= GetColCount())
	{
		ASSERT(0);
		return FALSE;
	}

	int nErrCode;
	if (nColID == 0 || nColID == 1 || nColID == 2)
	{
		if (!IsValidDoubleNumber(value, nErrCode))
		{
			switch(nErrCode)
			{
			case 1: SetWarningText(_LS(IDS_WG_CMD__ADDD__Invalid_formula_)); break;
			case 2: SetWarningText(_LS(IDS_WG_CMD__ADDD__Charater_exist_in_double_value)); break;
			}
			return FALSE;
		}
		if (_tstof(value) < 0.0)
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
BOOL CMhtpGrid::ValidateRecord(CStringArray &value, CRowColArray &aCols)
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

BOOL CMhtpGrid::ValidateAndMakeRecord(CStringArray &value, CRowColArray &aCols, double &key, T_MHTP_BASE &data)
{
	if (!ValidateRecord(value, aCols))
	{
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

BOOL CMhtpGrid::InsertRecordByKey(const double &key, const T_MHTP_BASE &data)
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

BOOL CMhtpGrid::DeleteRecordByKey(const double &key, const T_MHTP_BASE &data)
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

BOOL CMhtpGrid::AddToDB(ROWCOL nRow)
{
	// Record를 만들어서 DB에 입력한다.
	double key;
	T_MHTP_BASE data;
 
	GetValue(nRow, key, data);

	return AddData(key, data);
}

BOOL CMhtpGrid::AddToDB(CArray<void*, void*>& raKey, CArray<void*, void*>& raData)
{
	ASSERT(raKey.GetSize() == raData.GetSize());
	double tKey;
	CArray<double, double> aKey;
	T_MHTP_DT aData;
	for (int i = 0; i < raKey.GetSize(); i++)
	{
		tKey = *((double*)raKey.GetAt(i));
		aKey.Add(tKey);
		aData.Add(*((T_MHTP_BASE*)raData.GetAt(i)));
	}
	return AddData(aKey, aData);
}

BOOL CMhtpGrid::DeleteFromDB(ROWCOL nRow)
{
	double key;
	T_MHTP_BASE data;
 
	GetValue(nRow, key, data);
	return DelData(key);
}

BOOL CMhtpGrid::DeleteFromDB(CRowColArray& awRows)
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

BOOL CMhtpGrid::ModifyToDB(ROWCOL nRow)
{
	ROWCOL nCol;
	VERIFY(GetColumnNoFor(0, nCol));

	double key, oldKey;
	T_MHTP_BASE data;
	
	oldKey = _tstof(GetValueRowColBeforeEdit(nRow, nCol));
	GetValue(nRow, key, data);

	// 수정한다. 실패하면 에러 리턴
	return ModifyData(oldKey, key, data); 
}

BOOL CMhtpGrid::ModifyToDB(CStringArray& raOldKey, CArray<void*, void*>& raKey, CArray<void*, void*>& raData)
{
	ASSERT(raKey.GetSize() == raData.GetSize());
	ASSERT(raKey.GetSize() == raOldKey.GetSize());
	CArray<double, double> aOldKey, aKey;
	T_MHTP_DT aData;
	double Key;
	int nSize = raKey.GetSize();
	for (int i = 0; i < nSize; i++)
	{
		Key = _tstof(raOldKey.GetAt(i));
		aOldKey.Add(Key);
		aKey.Add(*((double*)raKey.GetAt(i)));
		aData.Add(*((T_MHTP_BASE*)raData.GetAt(i)));
	}
	return ModifyData(aOldKey, aKey, aData);
}

CString CMhtpGrid::GetNewKey()
{
	CString value(_T("0"));
	return value;
}

CString CMhtpGrid::GetDefValue(int nIndex)
{
	return m_aCurDefVal[nIndex];
}

CString* CMhtpGrid::GetDefValueArray()
{
	return m_aCurDefVal;
}

BOOL CMhtpGrid::MakeKeyAndDataArray(CStringArray& value, CRowColArray& awCols, 
																	CArray<void*, void*>& aKey, CArray<void*, void*>& aData)
{
	double* pKey;
	T_MHTP_BASE* pData;
	pKey = new double;
	pData = new T_MHTP_BASE;
	aKey.Add((void*)pKey);
	aData.Add((void*)pData);
	if (!ValidateAndMakeRecord(value, awCols, *pKey, *pData)) return FALSE;
	return TRUE;
}

void CMhtpGrid::DeleteKeyAndDataArray(CArray<void*, void*>& aKey, CArray<void*, void*>& aData)
{
	double* pKey;
	T_MHTP_BASE* pData;
	ASSERT(aKey.GetSize() == aData.GetSize());
	for (int i = 0; i < aKey.GetSize(); i++)
	{
		pKey = (double*)aKey.GetAt(i);
		pData = (T_MHTP_BASE*)aData.GetAt(i);
		delete pKey;
		delete pData;
	}
}

long CMhtpGrid::GetCountData()
{
	return m_pData->aFuncData.GetSize();
}

void CMhtpGrid::MakeItemFiltered(long nFilteredNum, long* aFilteredKeys, CRowColArray& awCols)
{
	ASSERT(0); // no filtering
}

void CMhtpGrid::MakeItemNormal(long nNumData, CRowColArray& awCols)
{
	double Key;
	T_MHTP_BASE Data;

	long nCount;
	for(nCount=0 ; nCount < nNumData; nCount++)
	{
		Key = m_pData->aFuncData[nCount].nC0;
		Data.dComp = m_pData->aFuncData[nCount].dComp;
		Data.dElast = m_pData->aFuncData[nCount].dElast;

		SetValue(GetParam()->GetData(), nCount+1, Key, Data, awCols);
	} // end of loop nCount
}

void CMhtpGrid::MakeItemEditingRecord(ROWCOL ncRow, CStringArray& EditingRecord)
{
	CDBDoc* pDoc = m_pDoc;
	double Key;
	T_MHTP_BASE Data;
	for (int i = 0; i < GetColCount(); i++)
	{
		if (i == 0) Key = _tstof(EditingRecord[i]);
		else ConvStrToData(i, EditingRecord[i], Data);
	}
	// 현재 값을 설정한다.
	SetValue(ncRow, Key, Data);
}

/////////////////////////////////////////////////////////////////////////////
// CMhtpGrid Implementation
BOOL CMhtpGrid::GetValue(ROWCOL nRow, double &key, T_MHTP_BASE &data)
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

BOOL CMhtpGrid::SetValue(ROWCOL nRow, const double &key, const T_MHTP_BASE &data)
{
	//
	CRowColArray awCols;
	awCols.SetSize(GetColCount());
	for (int i = 0;i < GetColCount(); i++)
		VERIFY(GetColumnNoFor(m_aTitle[i], awCols[i]));
	return SetValue(GetParam()->GetData(), nRow, key, data, awCols);
}

BOOL CMhtpGrid::SetValue(CGXData* pData, ROWCOL nRow, const double &key, const T_MHTP_BASE &data, CRowColArray &awCols)
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

void CMhtpGrid::ConvStrToData(int i, CString& value, T_MHTP_BASE& data)
{
	if (i==1) data.dComp = _tstof(value);
	else if (i==2) data.dElast = _tstof(value);
}

void CMhtpGrid::ConvDataToStr(int i, const T_MHTP_BASE& data, CString& value)
{
	if (i==1) value.Format(DATA_FMT, data.dComp);
	else if (i==2) value.Format(DATA_FMT, data.dElast);
}

void CMhtpGrid::MakeSearchKey(const double &key, const T_MHTP_BASE &data, CStringArray &aKey)
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

void CMhtpGrid::SetHeaderTitle(T_UNIT_SYSTEM &us, BOOL bInit)
{
	// Set Title
	CString csTitle, csUnit;
	int iUnit = CUnitCtrl::m_MHTP_UNIT.dC0;

	int aSize[] = {60, 90, 100};
	int nColCount = GetColCount();

	for (int i = 0; i < nColCount; i++)
	{
		m_aUnit.SetAt(0, iUnit);
		CUnitCtrl::GetUnitSystem(iUnit, csUnit);
		if (csUnit.IsEmpty()) csTitle = m_aTitle[i] + _T("\n");
		else csTitle = m_aTitle[i] + _T("\n(")+csUnit+_T(")");
		SetStyleRange(CGXRange(0, i+1), CGXStyle()
			.SetValue(csTitle)
			.SetUserAttribute(ID_USER_ATTR_COL_ID, m_aTitle[i]));
		if (bInit) SetColWidthByDPI(i+1, i+1, aSize[i]);
	}
	if (bInit) SetColWidthByDPI(0, 0, 35);
}

BOOL CMhtpGrid::FindData(double key, int& nPos, int nStart)
{
	int i;
	int nSize = m_pData->aFuncData.GetSize();
	
	CString csVal;
	double dblVal;
	int nCompResult;
	for (i = nStart; i < nSize; i++)
	{
		csVal.Format(_T("%g"), m_pData->aFuncData[i].nC0);
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

BOOL CMhtpGrid::ExistAllKey(CArray<double, double>& aKey, double& errorKey)
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

BOOL CMhtpGrid::AddData(double key, T_MHTP_BASE& data)
{
	CArray<double, double> aOldKey, aKey;
	T_MHTP_DT aData;
	aKey.Add(key);
	aData.Add(data);
	
	T_MHTP_BASE item;
	item.nC0  = key;
	item.dComp = data.dComp;
	item.dElast = data.dElast;

	// 동일한 key값 중복 검토
	int nPos;
	if (FindData(key, nPos, 0))
	{
		CString msg;
		msg.Format(_LS(IDS_WG_CMD__ADDD__Temperature__g_already_exist_), key);
		AfxMessageBox(msg);
		return FALSE;
	}
	m_pData->aFuncData.InsertAt(nPos, item);
	UpdateBuffer(0, aOldKey, aKey, aData);
	DBChanged();
	return TRUE;
}

BOOL CMhtpGrid::AddData(CArray<double, double> &aKey, T_MHTP_DT &aData, BOOL bNotify)
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
			msg.Format(_LS(IDS_WG_CMD__ADDD__Temperature__g_already_exist_), aOldKey[i]);
			AfxMessageBox(msg);
			return FALSE;
		}
		nPos = nStart;
	}
	for (int i = 0; i < nSize; i++)
	{
		bSame = FindData(aKey[i], nPos, 0);
		VERIFY(!bSame);

		T_MHTP_BASE item;
		item.nC0  = aKey[i];
		item.dComp = aData[i].dComp;
		item.dElast = aData[i].dElast;
		m_pData->aFuncData.InsertAt(nPos, item);
	}
	UpdateBuffer(0, aOldKey, aKey, aData);

	if (bNotify) DBChanged();
	return TRUE;
}

BOOL CMhtpGrid::DelData(double key)
{
	CArray<double, double> aOldKey, aKey;
	T_MHTP_DT aData;
	T_MHTP_BASE tmp;
	tmp.Initialize();
	aKey.Add(key);
	aData.Add(tmp);

	int nPos;
	if (!FindData(key, nPos))
	{
		CString msg;
		msg.Format(_LS(IDS_WG_CMD__ADDD__Temperature__g_does_not_exist_), key);
		AfxMessageBox(msg);
		return FALSE;
	}
	m_pData->aFuncData.RemoveAt(nPos);
	UpdateBuffer(1, aOldKey, aKey, aData);
	DBChanged();
	return TRUE;
}

BOOL CMhtpGrid::DelData(CArray<double, double> &aKey, BOOL bNotify)
{
	CArray<double, double> aOldKey;
	T_MHTP_DT aData;
	T_MHTP_BASE tmp;
	tmp.Initialize();
	int nSize = aKey.GetSize();
	for (int i = 0; i < nSize; i++) aData.Add(tmp);

	aOldKey.Copy(aKey);
	qsort((void*)aOldKey.GetData(), nSize, sizeof(double), CCompFunc::doubleAsc);
	// 동일한 key값 중복 검토
	double errKey;
	if (!ExistAllKey(aOldKey, errKey))
	{
		CString msg;
		msg.Format(_LS(IDS_WG_CMD__ADDD__Temperature__g_does_not_exist_), errKey);
		AfxMessageBox(msg);
		return FALSE;
	}
	int nSize2 = m_pData->aFuncData.GetSize();
	int j = nSize2-1;
	double dblDiff;
	for (int i = nSize-1; i >= 0; i--)
	{
		for (; j >= 0; j--)
		{
			dblDiff = aOldKey[i] - m_pData->aFuncData[j].nC0;
			if (dblDiff >= -EPSILON && dblDiff <= EPSILON) 
			{
				m_pData->aFuncData.RemoveAt(j);
				j--;
				break;
			}
		}
	}

	UpdateBuffer(1, aOldKey, aKey, aData);

	if (bNotify) DBChanged();
	return TRUE;
}

BOOL CMhtpGrid::ModifyData(double oldKey, double key, T_MHTP_BASE data)
{
	CArray<double, double> aOldKey, aKey;
	T_MHTP_DT aData;
	aOldKey.Add(oldKey);
	aKey.Add(key);
	aData.Add(data);

	int nPos;
	if (!FindData(oldKey, nPos))
	{
		CString msg;
		msg.Format(_LS(IDS_WG_CMD__ADDD__Old_temperature__g_does_not_exist_), oldKey);
		AfxMessageBox(msg);
		return FALSE;
	}
	if (oldKey == key)
	{
		m_pData->aFuncData[nPos].dComp = data.dComp;
		m_pData->aFuncData[nPos].dElast = data.dElast;
	}
	else
	{
		int nPos2;
		if (FindData(key, nPos2))
		{
			CString msg;
			msg.Format(_LS(IDS_WG_CMD__ADDD__New_temperature__g_already_exists_), key);
			AfxMessageBox(msg);
			return FALSE;
		}
		T_MHTP_BASE item;
		item.nC0  = key;
		item.dComp = data.dComp;
		item.dElast = data.dElast;
		m_pData->aFuncData.RemoveAt(nPos);
		if (nPos2 > nPos) nPos2--;
		m_pData->aFuncData.InsertAt(nPos2, item);
	}

	UpdateBuffer(2, aOldKey, aKey, aData);

	DBChanged();
	return TRUE;
}

BOOL CMhtpGrid::ModifyData(CArray<double, double> &aOldKey, CArray<double, double> &aKey, T_MHTP_DT  &aData)
{
	CArray<T_MHTP_BASE, T_MHTP_BASE&> aOrgData;
	aOrgData.Copy(m_pData->aFuncData);
	
	if (!DelData(aOldKey, FALSE))
		return FALSE;
	if (!AddData(aKey, aData, FALSE))
	{
		m_pData->aFuncData.Copy(aOrgData);
		return FALSE;
	}
	DBChanged();
	return TRUE;
}

void CMhtpGrid::UpdateBuffer(int nCmd, CArray<double, double> &aOldKey, CArray<double, double> &aKey, T_MHTP_DT &aData)
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

void CMhtpGrid::DBChanged()
{
	CTemperatureRisePropItemDlg* pDlg = (CTemperatureRisePropItemDlg*)GetParent();
	if (!pDlg) return;
	if (pDlg->GetSafeHwnd() == 0) return;
	if (!IsWindow(pDlg->GetSafeHwnd())) return;

	pDlg->OnDataChanged();
}
