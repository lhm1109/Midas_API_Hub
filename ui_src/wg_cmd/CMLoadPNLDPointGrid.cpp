// CMLoadPNLDPointGrid.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMLoadPNLDPointGrid.h"

#include "..\wg_base\wg_base_CompFunc.h"
#include "..\wg_db\wg_db_UnitCtrl.h"

#include "CMTendonProfileDlg.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define EPSILON 1.e-12
#define DATA_FMT _T("%.15g")

/////////////////////////////////////////////////////////////////////////////
// CCMLoadPNLDPointGrid
CString CCMLoadPNLDPointGrid::m_aCurDefVal[] = {_T("0.0"), _T("0.0"), _T("0.0"), _T("0.0")};

CCMLoadPNLDPointGrid::CCMLoadPNLDPointGrid()
{
	m_pData = 0;

	// 수정
	AddColInfo(_T("no"), t_real, 0);
	AddColInfo(_T("x"), t_real, CUnitCtrl::m_PNLD_UNIT.x);
	AddColInfo(_T("y"), t_real, CUnitCtrl::m_PNLD_UNIT.y);
	AddColInfo(_LS(IDS_CMD0417__Force), t_real, CUnitCtrl::m_PNLD_UNIT.dPointForce);
//  AddColInfo(_LS(IDS_CMD0417__Moment), t_real, CUnitCtrl::m_PNLD_UNIT.dPointMoment); // 필요없는 기능이라 판단되어 막음.

	m_aPrimaryKey.Add(0);
}

CCMLoadPNLDPointGrid::~CCMLoadPNLDPointGrid()
{
}


BEGIN_MESSAGE_MAP(CCMLoadPNLDPointGrid, CTBCommon)
	//{{AFX_MSG_MAP(CCMLoadPNLDPointGrid)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CCMLoadPNLDPointGrid message handlers
/////////////////////////////////////////////////////////////////////////////
// CCMLoadPNLDPointGrid message handlers

void CCMLoadPNLDPointGrid::Initialize(T_PNLD_D *pData)
{
	ASSERT(pData);
	m_pData = pData;

	T_UNIT_SYSTEM us;
	CDBDoc *pDoc = CDBDoc::GetDocPoint();
	//CTBCommon::Initialize(pDoc, us);
	//m_US = us;
	CTBCommon::Initialize(pDoc, m_US);

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
	//SetHeaderTitle(us, TRUE);
	SetHeaderTitle(m_US, TRUE);

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
	SetRowHeightByDPI(0, 0, 36);
	SetStyleRange(CGXRange().SetCols(1), CGXStyle().SetEnabled(FALSE));
	//SetStyleRange(CGXRange().SetCols(5), CGXStyle().SetEnabled(FALSE));

// 수정
	HideColsByIntend(1, 1, TRUE); // no column 숨긴다.

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

BOOL CCMLoadPNLDPointGrid::InsertRecordByKey(const double &key, const T_PNLD_PT_BASE &data)
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

BOOL CCMLoadPNLDPointGrid::DeleteRecordByKey(const double &key, const T_PNLD_PT_BASE &data)
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
// CCMLoadPNLDPointGrid Overridable Functions
void CCMLoadPNLDPointGrid::OnChangedSelectionPublic(long *keys, int nSize)
{
	return;
}

long CCMLoadPNLDPointGrid::GetSelectedKeys(CArray<long, long> &caKey)
{
	return 0;
}

BOOL CCMLoadPNLDPointGrid::ValidateField(CString value, int nColID)
{
	int nErrCode;
// 수정
	if (nColID == 0 || nColID == 1 || nColID == 2 || nColID == 3 || nColID == 4)
	{
		if (!IsValidDoubleNumber(value, nErrCode))
			return InvalidDoubleNumber(nErrCode);
	}

	return TRUE;
}

// 삽입이나 수정전에 Record의 Validation을 검사한다.
BOOL CCMLoadPNLDPointGrid::ValidateRecord(CStringArray &value, CRowColArray &aCols)
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
BOOL CCMLoadPNLDPointGrid::ValidateAndMakeRecord(CStringArray &value, CRowColArray& aCols, double &key, T_PNLD_PT_BASE &data)
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

BOOL CCMLoadPNLDPointGrid::AddToDB(ROWCOL nRow)
{
	// Record를 만들어서 DB에 입력한다.
	double key;
	T_PNLD_PT_BASE data;
 
	GetValue(nRow, key, data);

	return AddData(key, data);
}

BOOL CCMLoadPNLDPointGrid::AddToDB(CArray<void*, void*>& raKey, CArray<void*, void*>& raData)
{
	ASSERT(raKey.GetSize() == raData.GetSize());
	double tKey;
	CArray<double, double> aKey;
	CArray<T_PNLD_PT_BASE, T_PNLD_PT_BASE&> aData;
	for (int i = 0; i < raKey.GetSize(); i++)
	{
		tKey = *((double*)raKey.GetAt(i));
		aKey.Add(tKey);
		aData.Add(*((T_PNLD_PT_BASE*)raData.GetAt(i)));
	}
	return AddData(aKey, aData);
}


BOOL CCMLoadPNLDPointGrid::DeleteFromDB(ROWCOL nRow)
{
	double key;
	T_PNLD_PT_BASE data;
 
	GetValue(nRow, key, data);
	return DelData(key);
}

BOOL CCMLoadPNLDPointGrid::DeleteFromDB(CRowColArray& awRows)
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

BOOL CCMLoadPNLDPointGrid::ModifyToDB(ROWCOL nRow)
{
	ROWCOL nCol;
	VERIFY(GetColumnNoFor(0, nCol));

	double key, oldKey;
	T_PNLD_PT_BASE data;
	
	oldKey = _tstof(GetValueRowColBeforeEdit(nRow, nCol));
	GetValue(nRow, key, data);

	// 수정한다. 실패하면 에러 리턴
	return ModifyData(oldKey, key, data); 
}

BOOL CCMLoadPNLDPointGrid::ModifyToDB(CStringArray& raOldKey, 
												 CArray<void*, void*>& raKey, 
												 CArray<void*, void*>& raData)
{
	ASSERT(raKey.GetSize() == raData.GetSize());
	ASSERT(raKey.GetSize() == raOldKey.GetSize());
	CArray<double, double> aOldKey, aKey;
	CArray<T_PNLD_PT_BASE, T_PNLD_PT_BASE&> aData;
	double Key;
	int nSize = raKey.GetSize();
	for (int i = 0; i < nSize; i++)
	{
		Key = _tstof(raOldKey.GetAt(i));
		aOldKey.Add(Key);
		aKey.Add(*((double*)raKey.GetAt(i)));
		aData.Add(*((T_PNLD_PT_BASE*)raData.GetAt(i)));
	}
	return ModifyData(aOldKey, aKey, aData);
}


CString CCMLoadPNLDPointGrid::GetNewKey()
{
	ROWCOL ncRow, ncCol;
	GetCurrentCell(ncRow, ncCol);
	CString value;
	value.Format(DATA_FMT, ncRow-0.5);
	return value;
}

CString CCMLoadPNLDPointGrid::GetDefValue(int nIndex)
{
	return m_aCurDefVal[nIndex];
}

CString* CCMLoadPNLDPointGrid::GetDefValueArray()
{
	return m_aCurDefVal;
}

BOOL CCMLoadPNLDPointGrid::MakeKeyAndDataArray(CStringArray& value, CRowColArray& awCols, 
																	CArray<void*, void*>& aKey, CArray<void*, void*>& aData)
{
	double* pKey;
	T_PNLD_PT_BASE* pData;
	pKey = new double;
	pData = new T_PNLD_PT_BASE;
	aKey.Add((void*)pKey);
	aData.Add((void*)pData);
	if (!ValidateAndMakeRecord(value, awCols, *pKey, *pData)) return FALSE;
	return TRUE;
}

void CCMLoadPNLDPointGrid::DeleteKeyAndDataArray(CArray<void*, void*>& aKey, CArray<void*, void*>& aData)
{
	double* pKey;
	T_PNLD_PT_BASE* pData;
	ASSERT(aKey.GetSize() == aData.GetSize());
	for (int i = 0; i < aKey.GetSize(); i++)
	{
		pKey = (double*)aKey.GetAt(i);
		pData = (T_PNLD_PT_BASE*)aData.GetAt(i);
		delete pKey;
		delete pData;
	}
}

// 수정
long CCMLoadPNLDPointGrid::GetCountData()
{
	int i = m_pData->aPointLoad.GetSize();
	return i;
}

void CCMLoadPNLDPointGrid::MakeItemFiltered(long nFilteredNum, long* aFilteredKeys, CRowColArray& awCols)
{
	ASSERT(0); // no filtering
}

void CCMLoadPNLDPointGrid::MakeItemNormal(long nNumData, CRowColArray& awCols)
{
	double Key;
	T_PNLD_PT_BASE Data;

	long nCount;
	for(nCount=0 ; nCount < nNumData; nCount++)
	{
		Key = (double)nCount+1;
// 수정
		/*
		Data.dLoad = m_pData->dPointLoad[nCount];
		Data.dDistance = m_pData->dPointDistance[nCount];
		*/
		Data.x = m_pData->aPointLoad[nCount].x;
		Data.y = m_pData->aPointLoad[nCount].y;
		Data.frc = m_pData->aPointLoad[nCount].dForce;
		Data.mmt = m_pData->aPointLoad[nCount].dMoment;	

		SetValue(GetParam()->GetData(), nCount+1, Key, Data, awCols);
	} // end of loop nCount
}

void CCMLoadPNLDPointGrid::MakeItemEditingRecord(ROWCOL ncRow, CStringArray& EditingRecord)
{
	CDBDoc* pDoc = m_pDoc;
	double Key;
	T_PNLD_PT_BASE Data;
	for (int i = 0; i < GetColCount(); i++)
	{
		if (i == 0) Key = _tstof(EditingRecord[i]);
		else ConvStrToData(i, EditingRecord[i], Data);
	}
	Data.x = pDoc->m_pUnitCtrl->ConvertUnitDataPrevious(CUnitCtrl::m_PNLD_UNIT.x, Data.x);
	Data.y = pDoc->m_pUnitCtrl->ConvertUnitDataPrevious(CUnitCtrl::m_PNLD_UNIT.y, Data.y);
	Data.frc = pDoc->m_pUnitCtrl->ConvertUnitDataPrevious(CUnitCtrl::m_PNLD_UNIT.dPointForce, Data.frc);
	Data.mmt = pDoc->m_pUnitCtrl->ConvertUnitDataPrevious(CUnitCtrl::m_PNLD_UNIT.dPointMoment, Data.mmt);
	// 현재 값을 설정한다.
	SetValue(ncRow, Key, Data);
}

/////////////////////////////////////////////////////////////////////////////
// CCMLoadPNLDPointGrid Implementation
BOOL CCMLoadPNLDPointGrid::GetValue(ROWCOL nRow, double &key, T_PNLD_PT_BASE &data)
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

BOOL CCMLoadPNLDPointGrid::SetValue(ROWCOL nRow, const double &key, const T_PNLD_PT_BASE &data)
{
	//
	CRowColArray awCols;
	awCols.SetSize(GetColCount());
	for (int i = 0;i < GetColCount(); i++)
		VERIFY(GetColumnNoFor(m_aTitle[i], awCols[i]));
	return SetValue(GetParam()->GetData(), nRow, key, data, awCols);
}

BOOL CCMLoadPNLDPointGrid::SetValue(CGXData* pData, ROWCOL nRow, const double &key, const T_PNLD_PT_BASE &data, CRowColArray &awCols)
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

// 수정
void CCMLoadPNLDPointGrid::ConvStrToData(int i, CString& value, T_PNLD_PT_BASE& data)
{
	/*
	if (i==1) data.dLoad = _tstof(value);
	else if (i==2) data.dDistance = _tstof(value);
	*/
	if			(i==1) data.x		= _tstof(value);
	else if (i==2) data.y		= _tstof(value);
	else if (i==3) data.frc = _tstof(value);
	else if (i==4) data.mmt = _tstof(value);
}

// 수정
void CCMLoadPNLDPointGrid::ConvDataToStr(int i, const T_PNLD_PT_BASE& data, CString& value)
{
	/*
	if (i==1) value.Format(DATA_FMT, data.dLoad);
	else if (i==2) value.Format(DATA_FMT, data.dDistance);
	*/
	if			(i==1) value.Format(DATA_FMT, data.x);
	else if (i==2) value.Format(DATA_FMT, data.y);
	else if (i==3) value.Format(DATA_FMT, data.frc);
	else if (i==4) value.Format(DATA_FMT, data.mmt);
}

void CCMLoadPNLDPointGrid::MakeSearchKey(const double &key, const T_PNLD_PT_BASE &data, CStringArray &aKey)
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

void CCMLoadPNLDPointGrid::SetHeaderTitle(T_UNIT_SYSTEM &us, BOOL bInit)
{
	// Set Title
// 수정
	//int aSize[] = {30, 80, 80, 83, 83 };
	int aSize[] = {30, 80, 80, 166};
	int nColCount = GetColCount();

	CString csUnit, csTitle;
	for (int i = 0; i < nColCount; i++)
	{
		CUnitCtrl::GetUnitSystem(m_aUnit[i], csUnit);
		if (csUnit.IsEmpty()) csTitle = m_aTitle[i];
		csTitle = m_aTitle[i] + _T("\n(")+csUnit+_T(")");
		SetStyleRange(CGXRange(0, i+1), CGXStyle()
			.SetValue(csTitle)
			.SetUserAttribute(ID_USER_ATTR_COL_ID, m_aTitle[i]));
		if (bInit) SetColWidthByDPI(i+1, i+1, aSize[i]);
	}
	if (bInit) SetColWidthByDPI(0, 0, 30);
}

BOOL CCMLoadPNLDPointGrid::FindData(double key, int& nPos, int nStart)
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
		csVal.Format(DATA_FMT, TempKey);
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

BOOL CCMLoadPNLDPointGrid::ExistAllKey(CArray<double, double>& aKey, double& errorKey)
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

BOOL CCMLoadPNLDPointGrid::AddData(double key, T_PNLD_PT_BASE data)
{
	CArray<double, double> aOldKey, aKey;
	CArray<T_PNLD_PT_BASE, T_PNLD_PT_BASE&> aData;

	if (key == 0.0) key = GetCountData()+0.5;

	aKey.Add(key);
	aData.Add(data);
	T_PNLD_PT_BASE item;
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

BOOL CCMLoadPNLDPointGrid::AddData(CArray<double, double> &aKey, CArray<T_PNLD_PT_BASE, T_PNLD_PT_BASE&> &aData, BOOL bNotify)
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

		T_PNLD_PT_BASE item;
		item = aData[i];
		if (!InsertDataAt(nPos, item)) return FALSE;
	}
	UpdateBuffer(0, aOldKey, aKey, aData);
	if (bNotify) DBChanged();
	return TRUE;
}

BOOL CCMLoadPNLDPointGrid::DelData(double key)
{
	CArray<double, double> aOldKey, aKey;
	CArray<T_PNLD_PT_BASE, T_PNLD_PT_BASE&> aData;
	T_PNLD_PT_BASE data;
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

BOOL CCMLoadPNLDPointGrid::DelData(CArray<double, double> &aKey, BOOL bNotify)
{
	CArray<double, double> aOldKey;
	CArray<T_PNLD_PT_BASE, T_PNLD_PT_BASE&> aData;
	T_PNLD_PT_BASE data;
	int nSize = aKey.GetSize();
	int i = 0;
	for (i = 0; i < nSize; i++) aData.Add(data);

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
	for (i = nSize-1; i >= 0; i--)
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

BOOL CCMLoadPNLDPointGrid::ModifyData(double oldKey, double key, T_PNLD_PT_BASE data)
{
	CArray<double, double> aOldKey, aKey;
	CArray<T_PNLD_PT_BASE, T_PNLD_PT_BASE&> aData;

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
		T_PNLD_PT_BASE item;
		item = data;
		if (!RemoveDataAt(nPos)) return FALSE;
		if (nPos2 > nPos) nPos2--;
		if (!InsertDataAt(nPos2, item)) return FALSE;
	}

	UpdateBuffer(2, aOldKey, aKey, aData);

	DBChanged();
	return TRUE;
}

BOOL CCMLoadPNLDPointGrid::ModifyData(CArray<double, double> &aOldKey, CArray<double, double> &aKey, CArray<T_PNLD_PT_BASE, T_PNLD_PT_BASE&> &aData)
{
	CArray<T_PNLD_PT_BASE, T_PNLD_PT_BASE&> aOrgData;
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

void CCMLoadPNLDPointGrid::UpdateBuffer(int nCmd, CArray<double, double> &aOldKey, CArray<double, double> &aKey, CArray<T_PNLD_PT_BASE, T_PNLD_PT_BASE&> &aData)
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
void CCMLoadPNLDPointGrid::DBChanged()
{
	// Nothing to do
}

// 수정
BOOL CCMLoadPNLDPointGrid::InsertDataAt(int nzbPos, T_PNLD_PT_BASE& data)
{
	//int nDataCount = GetCountData();
	int nDataCount = m_pData->aPointLoad.GetSize();
	if(nzbPos > nDataCount) return FALSE;
	//m_pData->aPointLoad.SetSize(nDataCount+1);
	
	T_PNLD_POINT Data;
	Data.x			= data.x;
	Data.y			= data.y;
	Data.dForce = data.frc;
	Data.dMoment= data.mmt;
	m_pData->aPointLoad.InsertAt(nzbPos,Data);
	//m_pData->dPointLoad[nzbPos] = data.dLoad;
	//m_pData->dPointDistance[nzbPos] = data.dDistance;
	 return TRUE;
}

// 수정
BOOL CCMLoadPNLDPointGrid::RemoveDataAt(int nzbPos)
{
	int nDataCount = m_pData->aPointLoad.GetSize();
	if(nzbPos > nDataCount-1) return FALSE;
	/*
	if (nzbPos < 0 || nzbPos >= nDataCount)
	{
		ASSERT(0);
		AfxMessageBox(_T("Error : Can't find item to delete"));
		return FALSE;
	}
	*/
	m_pData->aPointLoad.RemoveAt(nzbPos);

	return TRUE;
}

// 수정
BOOL CCMLoadPNLDPointGrid::ChangeDataAt(int nzbPos, T_PNLD_PT_BASE& data)
{
	int nDataCount = GetCountData();
	/*
	if (nzbPos < 0 || nzbPos >= nDataCount)
	{
		ASSERT(0);
		AfxMessageBox(_T("Error : Can't find item to change"));
		return FALSE;
	}
	
	m_pData->dPointLoad[nzbPos] = data.dLoad;
	m_pData->dPointDistance[nzbPos] = data.dDistance;
	*/

	m_pData->aPointLoad[nzbPos].x			= data.x;
	m_pData->aPointLoad[nzbPos].y			= data.y;
	m_pData->aPointLoad[nzbPos].dForce = data.frc;
	m_pData->aPointLoad[nzbPos].dMoment= data.mmt;

	return TRUE;
}

// 수정
BOOL CCMLoadPNLDPointGrid::SaveData(CArray<T_PNLD_PT_BASE, T_PNLD_PT_BASE&> &aData)
{
	int num = m_pData->aPointLoad.GetSize();
	aData.SetSize(num);

	for (int i = 0; i < num; i++)
	{
		aData[i].x = m_pData->aPointLoad[i].x;
		aData[i].y = m_pData->aPointLoad[i].x;
		aData[i].frc = m_pData->aPointLoad[i].dForce;
		aData[i].mmt = m_pData->aPointLoad[i].dMoment;
	}
	return TRUE;
}

// 수정
BOOL CCMLoadPNLDPointGrid::RestoreData(CArray<T_PNLD_PT_BASE, T_PNLD_PT_BASE&> &aData)
{
	int num = aData.GetSize();
	
	m_pData->aPointLoad.RemoveAll();
	m_pData->aPointLoad.SetSize(num);
	for (int i = 0; i < num; i++)
	{
		m_pData->aPointLoad[i].x = aData[i].x;
		m_pData->aPointLoad[i].y = aData[i].x;
		m_pData->aPointLoad[i].dForce		= aData[i].frc;
		m_pData->aPointLoad[i].dMoment	= aData[i].mmt;
	}
	return TRUE;
}

void CCMLoadPNLDPointGrid::AfterUpdateBuffer()
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


// 저장을 위해서.. 임의로 만들었다. 
void CCMLoadPNLDPointGrid::My_SaveData(void)
{
	
}

void CCMLoadPNLDPointGrid::UpdateUnit()
{
	SetHeaderTitle(m_US, TRUE);
}

void CCMLoadPNLDPointGrid::UnitChanged(CDBDoc* pDoc)
{
	BOOL bOldLock = LockUpdate(TRUE);

	T_UNIT_SYSTEM UnitSystem;
 	//pDoc->m_pUnitCtrl->GetUnitSystemBmld(UnitSystem);
	pDoc->m_pUnitCtrl->ConvertUnitPnldPrevious(*m_pData);

	// 타이틀을 수정
	SetHeaderTitle(m_US, TRUE);

	// 각 항목의 값을 수정
	MakeItemEx(0, -2, TRUE);
	LockUpdate(bOldLock);
	if (!bOldLock) Redraw();
}
