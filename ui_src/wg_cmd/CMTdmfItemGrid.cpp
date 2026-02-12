#include "stdafx.h"
#include "wg_cmd.h"
#include "CMTdmfItemGrid.h"
#include "CMTdmfItemDlg.h"

#include "..\wg_base\wg_base_UtilFunc.h"
#include "..\wg_base\wg_base_CompFunc.h"

#include "..\wg_db\wg_db_UnitCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define DATA_FMT _T("%.15g")

/////////////////////////////////////////////////////////////////////////////
// CCMTdmfItemGrid
CString CCMTdmfItemGrid::m_aCurDefVal[] = {_T("0.0"), _T("0.0")};
CCMTdmfItemGrid::CCMTdmfItemGrid()
{
	m_pData = 0;
	m_bReadOnly = FALSE;

	AddColInfo(_LS(IDS_CMD_TDMF_GRID_Time),  t_real,    0);
	AddColInfo(_LS(IDS_CMD_TDMF_GRID_Value), t_real,    0);

	m_aPrimaryKey.Add(0);
}

CCMTdmfItemGrid::~CCMTdmfItemGrid()
{
}

BEGIN_MESSAGE_MAP(CCMTdmfItemGrid, CTBCommon)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMTdmfItemGrid message handlers
void CCMTdmfItemGrid::SetReadOnlyMode(BOOL bReadOnly/*= TRUE*/)
{
	m_bReadOnly = bReadOnly;
}

void CCMTdmfItemGrid::Initialize(T_TDMF_D *pData)
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
			.SetValueType(GX_VT_NUMERIC));
	SetRowHeightByDPI(0, 0, 36);

	SetStyleRange(CGXRange().SetCols(1), CGXStyle()
			.SetFormat(GX_FMT_FIXED)
			.SetPlaces(2));

	SetValueStyle();


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
	
	// 2002. 7. 15. by TAE 
	// Display Only Mode로 설정한다. Insert, Delete Key Disable의 효과
	if(m_bReadOnly) SetModeDisplayOnly(TRUE);

	LockUpdate(FALSE);
}

BOOL CCMTdmfItemGrid::InsertRecordByKey(const double &key, const double &data)
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

BOOL CCMTdmfItemGrid::DeleteRecordByKey(const double &key, const double &data)
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
// CCMTdmfItemGrid Overridable Functions

BOOL CCMTdmfItemGrid::ValidateField(CString value, int nColID)
{
	if (nColID >= GetColCount())
	{
		ASSERT(0);
		return FALSE;
	}

	int nErrCode;
	if (nColID == 0 || nColID == 1)
	{
		if (!IsValidDoubleNumber(value, nErrCode))
			 return InvalidDoubleNumber(nErrCode);
	}
	else return FALSE;

	return TRUE;
}

// 삽입이나 수정전에 Record의 Validation을 검사한다.
BOOL CCMTdmfItemGrid::ValidateRecord(CStringArray &value, CRowColArray &aCols)
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
BOOL CCMTdmfItemGrid::ValidateAndMakeRecord(CStringArray &value, CRowColArray& aCols, double &key, double &data)
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

BOOL CCMTdmfItemGrid::AddToDB(ROWCOL nRow)
{
	// Record를 만들어서 DB에 입력한다.
	double key;
	double data;
 
	GetValue(nRow, key, data);

	return AddData(key, data);
}

BOOL CCMTdmfItemGrid::AddToDB(CArray<void*, void*>& raKey, CArray<void*, void*>& raData)
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


BOOL CCMTdmfItemGrid::DeleteFromDB(ROWCOL nRow)
{
	double key;
	double data;
 
	GetValue(nRow, key, data);
	return DelData(key);
}

BOOL CCMTdmfItemGrid::DeleteFromDB(CRowColArray& awRows)
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

BOOL CCMTdmfItemGrid::ModifyToDB(ROWCOL nRow)
{
	ROWCOL nCol;
	VERIFY(GetColumnNoFor(0, nCol));

	double key, oldkey;
	double data;
	
	oldkey = _tstof(GetValueRowColBeforeEdit(nRow, nCol));
	GetValue(nRow, key, data);

	// 수정한다. 실패하면 에러 리턴
	return ModifyData(oldkey, key, data); 
}

BOOL CCMTdmfItemGrid::ModifyToDB(CStringArray& raOldKey, 
												 CArray<void*, void*>& raKey, 
												 CArray<void*, void*>& raData)
{
	ASSERT(raKey.GetSize() == raData.GetSize());
	ASSERT(raKey.GetSize() == raOldKey.GetSize());
	CArray<double, double> aOldKey, aKey;
	CArray<double, double> aData;
	double key;
	int nSize = raKey.GetSize();
	for (int i = 0; i < nSize; i++)
	{
		key = _tstof(raOldKey.GetAt(i));
		aOldKey.Add(key);
		aKey.Add(*((double*)raKey.GetAt(i)));
		aData.Add(*((double*)raData.GetAt(i)));
	}
	return ModifyData(aOldKey, aKey, aData);
}


CString CCMTdmfItemGrid::GetNewKey()
{
	CString value(_T("0"));
	return value;
}

CString CCMTdmfItemGrid::GetDefValue(int nIndex)
{
	return m_aCurDefVal[nIndex];
}

CString* CCMTdmfItemGrid::GetDefValueArray()
{
	return m_aCurDefVal;
}

BOOL CCMTdmfItemGrid::MakeKeyAndDataArray(CStringArray& value, CRowColArray& awCols, 
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

void CCMTdmfItemGrid::DeleteKeyAndDataArray(CArray<void*, void*>& aKey, CArray<void*, void*>& aData)
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

long CCMTdmfItemGrid::GetCountData()
{
	return m_pData->arMatlFuncData.GetSize();
}

void CCMTdmfItemGrid::MakeItemFiltered(long nFilteredNum, long* aFilteredKeys, CRowColArray& awCols)
{
	ASSERT(0);
}

void CCMTdmfItemGrid::MakeItemNormal(long nNumData, CRowColArray& awCols)
{
	SetValueStyle();

	double Key;
	double Data;

	long nCount;
	for(nCount=0 ; nCount < nNumData; nCount++)
	{
		Key = m_pData->arMatlFuncData[nCount].dDay;
		Data = m_pData->arMatlFuncData[nCount].dValue;

		SetValue(GetParam()->GetData(), nCount+1, Key, Data, awCols);
	} // end of loop nCount
}

void CCMTdmfItemGrid::MakeItemEditingRecord(ROWCOL ncRow, CStringArray& EditingRecord)
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
// CCMTdmfItemGrid Implementation
BOOL CCMTdmfItemGrid::GetValue(ROWCOL nRow, double &key, double &data)
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

BOOL CCMTdmfItemGrid::SetValue(ROWCOL nRow, const double &key, const double &data)
{
	//
	CRowColArray awCols;
	awCols.SetSize(GetColCount());
	for (int i = 0;i < GetColCount(); i++)
		VERIFY(GetColumnNoFor(m_aTitle[i], awCols[i]));
	return SetValue(GetParam()->GetData(), nRow, key, data, awCols);
}

BOOL CCMTdmfItemGrid::SetValue(CGXData* pData, ROWCOL nRow, const double &key, const double &data, CRowColArray &awCols)
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

void CCMTdmfItemGrid::ConvStrToData(int i, CString& value, double& data)
{
	if (i==1) data = _tstof(value);
}

void CCMTdmfItemGrid::ConvDataToStr(int i, const double& data, CString& value)
{
	if (i==1) value.Format(DATA_FMT, data);
}

void CCMTdmfItemGrid::MakeSearchKey(const double &key, const double &data, CStringArray &aKey)
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

void CCMTdmfItemGrid::SetHeaderTitle(T_UNIT_SYSTEM &us, BOOL bInit)
{
	// Set Title
	ASSERT(m_pData->nMatlFuncType >= TDMF_FUNC_CREEP && m_pData->nMatlFuncType <= TDMF_FUNC_RELAXATION);

	int nUnitIx = 0;

	// 이거 좀 이상.... 
	if (m_pData->nMatlFuncType == TDMF_FUNC_CREEP)
		nUnitIx = 1 + m_pData->nCreepType; // creep
	else if (m_pData->nMatlFuncType == TDMF_FUNC_RELAXATION)
		nUnitIx = m_pData->nRelaxTime == TDMF_RELAX_TIME_HOUR ? 5 : 6;
	else 
		nUnitIx = m_pData->nMatlFuncType-1; // 얘는 1만 쓸거 같은데... ;;;
	//

	CString csTitle, csUnit;
	int aUnit[][2] = {
		{CUnitCtrl::m_TDMF_UNIT.dDay, CUnitCtrl::m_TDMF_UNIT.dValue[0]}, 
		{CUnitCtrl::m_TDMF_UNIT.dDay, CUnitCtrl::m_TDMF_UNIT.dValue[1]}, 
		{CUnitCtrl::m_TDMF_UNIT.dDay, CUnitCtrl::m_TDMF_UNIT.dCreep[0]}, 
		{CUnitCtrl::m_TDMF_UNIT.dDay, CUnitCtrl::m_TDMF_UNIT.dCreep[1]}, 
		{CUnitCtrl::m_TDMF_UNIT.dDay, CUnitCtrl::m_TDMF_UNIT.dCreep[2]}, 
		{CUnitCtrl::m_TDMF_UNIT.dHour,CUnitCtrl::m_TDMF_UNIT.dRelax}, 
		{CUnitCtrl::m_TDMF_UNIT.dDay, CUnitCtrl::m_TDMF_UNIT.dRelax}, 
	};

	int aSize[] = {60, 86};
	int nColCount = GetColCount();

	for (int i = 0; i < nColCount; i++)
	{
		m_aUnit.SetAt(i, aUnit[nUnitIx][i]);

		CUnitCtrl::GetUnitSystem(aUnit[nUnitIx][i], csUnit);
		
		if (csUnit.IsEmpty()) csTitle = m_aTitle[i];
		else csTitle = m_aTitle[i] + _T("\n(")+csUnit+_T(")");

		SetStyleRange(CGXRange(0, i+1), CGXStyle()
			.SetValue(csTitle)
			.SetUserAttribute(ID_USER_ATTR_COL_ID, m_aTitle[i]));

		if (bInit) SetColWidthByDPI(i+1, i+1, aSize[i]);
	}
	if (bInit) SetColWidthByDPI(0, 0, 35);
}

BOOL CCMTdmfItemGrid::FindData(double key, int& nPos, int nStart)
{
	int i;
	int nSize = m_pData->arMatlFuncData.GetSize();
	
	double nVal;
	int nCompResult;
	for (i = nStart; i < nSize; i++)
	{
		nVal = m_pData->arMatlFuncData[i].dDay;
		nCompResult = SafeCompareReal(key, nVal);
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

BOOL CCMTdmfItemGrid::ExistAllKey(CArray<double, double>& aKey, int& errorKey)
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

BOOL CCMTdmfItemGrid::AddData(double key, double data)
{
	CArray<double, double> aOldKey, aKey;
	CArray<double, double> aData;
	aKey.Add(key);
	aData.Add(data);
	T_TDMF_BASE item;
	item.dDay = key;
	item.dValue = data;

	// 동일한 key값 중복 검토
	int nPos;
	if (FindData(key, nPos, 0))
	{
		CString msg;
		msg.Format(_LS(IDS_CMD_TDMF_GRID_time__d__already_exist_), key);
		AfxMessageBox(msg);
		return FALSE;
	}
	m_pData->arMatlFuncData.InsertAt(nPos, item);
	UpdateBuffer(0, aOldKey, aKey, aData);
	DBChanged();
	return TRUE;
}

BOOL CCMTdmfItemGrid::AddData(CArray<double, double> &aKey, CArray<double, double> &aData, BOOL bNotify)
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
			bSame = (SafeCompareReal(aOldKey[i],aOldKey[i-1]) == 0);
		if (bSame)
		{
			CString msg;
			msg.Format(_LS(IDS_CMD_TDMF_GRID_time__d__already_exist_), aOldKey[i]);
			AfxMessageBox(msg);
			return FALSE;
		}
		nPos = nStart;
	}
	for (int i = 0; i < nSize; i++)
	{
		bSame = FindData(aKey[i], nPos, 0);
		VERIFY(!bSame);

		T_TDMF_BASE item;
		item.dDay = aKey[i];
		item.dValue = aData[i];
		m_pData->arMatlFuncData.InsertAt(nPos, item);
	}
	UpdateBuffer(0, aOldKey, aKey, aData);

	if (bNotify) DBChanged();
	return TRUE;
}

BOOL CCMTdmfItemGrid::DelData(double key)
{
	CArray<double, double> aOldKey, aKey;
	CArray<double, double> aData;
	aKey.Add(key);
	aData.Add(0.0);

	int nPos;
	if (!FindData(key, nPos))
	{
		CString msg;
		msg.Format(_LS(IDS_CMD_TDMF_GRID_time__d__does_not_exist_), key);
		AfxMessageBox(msg);
		return FALSE;
	}
	m_pData->arMatlFuncData.RemoveAt(nPos);
	UpdateBuffer(1, aOldKey, aKey, aData);
	DBChanged();
	return TRUE;
}

BOOL CCMTdmfItemGrid::DelData(CArray<double, double> &aKey, BOOL bNotify)
{
	CArray<double, double> aOldKey;
	CArray<double, double> aData;
	int nSize = aKey.GetSize();
	for (int i = 0; i < nSize; i++) aData.Add(0.0);

	aOldKey.Copy(aKey);
	qsort((void*)aOldKey.GetData(), nSize, sizeof(double), CCompFunc::doubleAsc);
	// 동일한 key값 중복 검토
	int errKey;
	if (!ExistAllKey(aOldKey, errKey))
	{
		CString msg;
		msg.Format(_LS(IDS_CMD_TDMF_GRID_time__d__does_not_exist_), errKey);
		AfxMessageBox(msg);
		return FALSE;
	}
	int nSize2 = m_pData->arMatlFuncData.GetSize();
	int j = nSize2-1;
	int nDiff;
	for (int i = nSize-1; i >= 0; i--)
	{
		for (; j >= 0; j--)
		{
			nDiff = SafeCompareReal(aOldKey[i], m_pData->arMatlFuncData[j].dDay);
			if (nDiff == 0) 
			{
				m_pData->arMatlFuncData.RemoveAt(j);
				j--;
				break;
			}
		}
	}

	UpdateBuffer(1, aOldKey, aKey, aData);

	if (bNotify) DBChanged();
	return TRUE;
}

BOOL CCMTdmfItemGrid::ModifyData(double oldkey, double key, double data)
{
	CArray<double, double> aOldKey, aKey;
	CArray<double, double> aData;
	aOldKey.Add(oldkey);
	aKey.Add(key);
	aData.Add(data);

	int nPos;
	if (!FindData(oldkey, nPos))
	{
		CString msg;
		msg.Format(_LS(IDS_CMD_TDMF_GRID_time__d__does_not_exist_), oldkey);
		AfxMessageBox(msg);
		return FALSE;
	}
	if (SafeCompareReal(oldkey, key) == 0)
	{
		m_pData->arMatlFuncData[nPos].dValue = data;
	}
	else
	{
		int nPos2;
		if (FindData(key, nPos2))
		{
			CString msg;
			msg.Format(_LS(IDS_CMD_TDMF_GRID_new_time__d__already_exist_), key);
			AfxMessageBox(msg);
			return FALSE;
		}
		T_TDMF_BASE item;
		item.dDay = key;
		item.dValue = data;
		m_pData->arMatlFuncData.RemoveAt(nPos);
		if (nPos2 > nPos) nPos2--;
		m_pData->arMatlFuncData.InsertAt(nPos2, item);
	}

	UpdateBuffer(2, aOldKey, aKey, aData);

	DBChanged();
	return TRUE;
}

BOOL CCMTdmfItemGrid::ModifyData(CArray<double, double> &aOldKey, CArray<double, double> &aKey, CArray<double, double> &aData)
{
	CArray<T_TDMF_BASE, T_TDMF_BASE&> aOrgData;
	aOrgData.Copy(m_pData->arMatlFuncData);
	
	if (!DelData(aOldKey, FALSE))
		return FALSE;
	if (!AddData(aKey, aData, FALSE))
	{
		m_pData->arMatlFuncData.Copy(aOrgData);
		return FALSE;
	}
	DBChanged();
	return TRUE;
}

void CCMTdmfItemGrid::UpdateBuffer(int nCmd, CArray<double, double> &aOldKey, CArray<double, double> &aKey, CArray<double, double> &aData)
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

void CCMTdmfItemGrid::DBChanged()
{
	CCMTdmfItemDlg* pDlg = (CCMTdmfItemDlg*)GetParent();
	if (!pDlg) return;
	if (pDlg->GetSafeHwnd() == 0) return;
	if (!IsWindow(pDlg->GetSafeHwnd())) return;

	pDlg->MakeChartData();
}

void CCMTdmfItemGrid::SetValueStyle()
{
	int nColCount = GetColCount();
	if (m_pData->nMatlFuncType == 1)
		SetStyleRange(CGXRange().SetCols(nColCount), CGXStyle()
			.SetFormat(GX_FMT_FLOAT).SetPlaces(4));
	else
		SetStyleRange(CGXRange().SetCols(nColCount), CGXStyle()
			.SetFormat(GX_FMT_FLOAT).SetPlaces(4));
}
