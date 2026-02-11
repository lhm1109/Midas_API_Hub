#include "stdafx.h"
#include "wg_cmd.h"
#include "NXNlctLoadStepGrid.h"

#include "CMNXNlctLoadStepDlg.h"
#include "..\wg_base\wg_base_CompFunc.h"
#include "..\wg_db\wg_db_UnitCtrl.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#define KEY_FMT  _T("%d")
#define DATA_FMT _T("%.15g")

CString CNXNlctLoadStepGrid::m_aCurDefVal[] = { _T(""),_T(""), _T("0") };
CNXNlctLoadStepGrid::CNXNlctLoadStepGrid()
{
	m_nRowCount = 0;
	m_nOldRowCount = 0;

	AddColInfo(_LS(IDS_CMD2_STEP), t_integer, 0);
	AddColInfo(_LS(IDS_CMD2_LOAD_INCRE_FACTOR), t_real, 0);
	AddColInfo(_LS(IDS_CMD2_SAVE_STEP), t_integer, 0);
}

CNXNlctLoadStepGrid::~CNXNlctLoadStepGrid()
{

}

BEGIN_MESSAGE_MAP(CNXNlctLoadStepGrid, CTBCommon)

END_MESSAGE_MAP()

void CNXNlctLoadStepGrid::SetRowCountUser(int nRowCount)
{
	ASSERT(nRowCount >= 0);

	m_nRowCount = nRowCount;
	SetRowCount(m_nRowCount);

	if (m_nRowCount > m_nOldRowCount) // 새로 추가된 Row에 대해서 Default 값 Setting
	{ 
		T_NLCT_DATA* NlctData = m_pData;

		m_pData->aLoadSteps.SetSize(m_nRowCount);

		for (int i = 0; i < m_nOldRowCount; i++)
		{
			m_pData->aLoadSteps[i] = NlctData->aLoadSteps[i];
		}

		SetDefaultValue(m_nOldRowCount + 1, m_nRowCount);	
	}	

	m_nOldRowCount = m_nRowCount;
}

void CNXNlctLoadStepGrid::SetDefaultValue(ROWCOL nFromRow, ROWCOL nToRow)
{
	double dValue = 1.0 / nToRow;

	T_NLCT_LOADSTEP LoadStep;

	for (ROWCOL nRow = nFromRow; nRow <= nToRow; nRow++)
	{
		SetValueRange(CGXRange(nRow, 1), LONG(nRow));
		SetValueRange(CGXRange(nRow, 2), double(dValue));

		LoadStep.Initialize();
		LoadStep.nStep = nRow;
		LoadStep.dLoadIncFactor = dValue;
		LoadStep.bSave = FALSE;

		m_pData->aLoadSteps[nRow - 1] = LoadStep;
	}
}

void CNXNlctLoadStepGrid::GenerateValue(double dValue)
{
	T_NLCT_LOADSTEP LoadStep;

	ROWCOL nCol = 2;
	int nRowCount = GetRowCount();

	if (m_nOldRowCount != nRowCount)
		ASSERT(0);

	for (ROWCOL nRow = 1; nRow <= m_nOldRowCount; nRow++)
	{
		SetValueRange(CGXRange(nRow, nCol), double(dValue));

		LoadStep.Initialize();
		LoadStep.nStep = nRow;
		LoadStep.dLoadIncFactor = dValue;
		LoadStep.bSave = FALSE;

		m_pData->aLoadSteps[nRow - 1] = LoadStep;
	}		
}

BOOL CNXNlctLoadStepGrid::DeleteRecordByKey(const int &key, const T_NLCT_LOADSTEP &data)
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

BOOL CNXNlctLoadStepGrid::InsertRecordByKey(const int &key, const T_NLCT_LOADSTEP &data)
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

void CNXNlctLoadStepGrid::OnChangedSelectionPublic(long *keys, int nSize)
{
	return;
}

long CNXNlctLoadStepGrid::GetSelectedKeys(CArray<long, long> &caKey)
{
	return 0;
}

BOOL CNXNlctLoadStepGrid::AddToDB(ROWCOL nRow)
{
	int key;
	T_NLCT_LOADSTEP data;

	GetValue(nRow, key, data);
	return AddData(key, data);

	return TRUE;
}

BOOL CNXNlctLoadStepGrid::AddToDB(CArray<void*, void*>& raKey, CArray<void*, void*>& raData)
{
	ASSERT(raKey.GetSize() == raData.GetSize());
	int tKey;
	CArray<int, int> aKey;
	CArray<T_NLCT_LOADSTEP, T_NLCT_LOADSTEP&> aData;
	for (int i = 0; i < raKey.GetCount(); i++)
	{
		tKey = *((int*)raKey.GetAt(i));
		aKey.Add(tKey);
		aData.Add(*((T_NLCT_LOADSTEP*)raData.GetAt(i)));
	}
	return AddData(aKey, aData);
}

BOOL CNXNlctLoadStepGrid::DeleteFromDB(ROWCOL nRow)
{
	int key;
	T_NLCT_LOADSTEP data;
	GetValue(nRow, key, data);
	return DelData(key);
}

BOOL CNXNlctLoadStepGrid::DeleteFromDB(CRowColArray& awRows)
{
	if (awRows.GetSize() <= 0)return FALSE;
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

BOOL CNXNlctLoadStepGrid::ModifyToDB(ROWCOL nRow)
{
	// Append Row (빈 행)는 처리하지 않음
	if (nRow > m_pData->aLoadSteps.GetCount())
		return TRUE;

	ROWCOL nCol;
	VERIFY(GetColumnNoFor(0, nCol));
	int key, oldKey;
	T_NLCT_LOADSTEP data;

	oldKey = _ttoi(GetValueRowColBeforeEdit(nRow, nCol));
	
	// 기존 데이터 복사 (빈 값 읽는 문제 방지)
	int nPos = nRow - 1;  // 0-based index
	if (nPos >= 0 && nPos < m_pData->aLoadSteps.GetCount())
	{
		data = m_pData->aLoadSteps[nPos];  // 기존 데이터 보존
	}
	else
	{
		data.Initialize();
	}
	
	// Grid에서 값 읽기 (각 컬럼별로 검증)
	CString strValue;
	
	// Step 번호 (Column 1)
	strValue = GetValueRowCol(nRow, 1);
	if (!strValue.IsEmpty())
		key = _ttoi(strValue);
	else
		key = oldKey;
	
	// Incre Factor (Column 2)
	strValue = GetValueRowCol(nRow, 2);
	if (!strValue.IsEmpty())
	{
		data.dLoadIncFactor = _tstof(strValue);
	}
	// 빈 값이면 기존 값 유지
	
	// Save Step (Column 3)
	strValue = GetValueRowCol(nRow, 3);
	if (!strValue.IsEmpty())
		data.bSave = (_ttoi(strValue) != 0);
	// 빈 값이면 기존 값 유지
	
	data.nStep = key;
	
	return ModifyData(oldKey, key, data);
}

BOOL CNXNlctLoadStepGrid::ModifyToDB(CStringArray& raOldKey, CArray<void*, void*>& raKey, CArray<void*, void*>& raData)
{
	ASSERT(raKey.GetSize() == raData.GetSize());
	ASSERT(raKey.GetSize() == raOldKey.GetSize());
	CArray<int, int> aOldKey, aKey;
	CArray<T_NLCT_LOADSTEP, T_NLCT_LOADSTEP&> aData;
	int key;
	int nSize = raKey.GetSize();
	for (int i = 0; i < nSize; i++)
	{
		key = _ttoi(raOldKey.GetAt(i));
		aOldKey.Add(key);
		aKey.Add(*((double*)raKey.GetAt(i)));
		aData.Add(*((T_NLCT_LOADSTEP*)raData.GetAt(i)));
	}
	return ModifyData(aOldKey, aKey, aData);
}

CString CNXNlctLoadStepGrid::GetNewKey()
{
	CString value(_T(""));
	return value;
}

CString CNXNlctLoadStepGrid::GetDefValue(int nIndex)
{
	return m_aCurDefVal[nIndex];
}

CString* CNXNlctLoadStepGrid::GetDefValueArray()
{
	return m_aCurDefVal;
}

BOOL CNXNlctLoadStepGrid::MakeKeyAndDataArray(CStringArray& value, CRowColArray& awCols, CArray<void*, void*>& aKey, CArray<void*, void*>& aData)
{
	int* pKey;
	T_NLCT_LOADSTEP* pData;
	pKey = new int;
	pData = new T_NLCT_LOADSTEP;
	aKey.Add((void*)pKey);
	aData.Add((void*)pData);
	if (!ValidateAndMakeRecord(value, awCols, *pKey, *pData))return FALSE;

	return TRUE;
}

void CNXNlctLoadStepGrid::DeleteKeyAndDataArray(CArray<void*, void*>& aKey, CArray<void*, void*>& aData)
{
	int* pKey;
	T_NLCT_LOADSTEP* pData;
	ASSERT(aKey.GetSize() == aData.GetSize());
	for (int i = 0; i < aKey.GetSize(); i++)
	{
		pKey = (int*)aKey.GetAt(i);
		pData = (T_NLCT_LOADSTEP*)aData.GetAt(i);
		delete pKey;
		delete pData;
	}
}

long CNXNlctLoadStepGrid::GetCountData()
{
	return m_pData->aLoadSteps.GetCount();
}

void CNXNlctLoadStepGrid::MakeItemFiltered(long nFilteredNum, long* aFilteredKeys, CRowColArray& awCols)
{
	ASSERT(0); //no Filtering
}

void CNXNlctLoadStepGrid::MakeItemNormal(long nNumData, CRowColArray& awCols)
{
	int Key;
	T_NLCT_LOADSTEP Data;

	long nCount;
	for (nCount = 0; nCount < nNumData; nCount++)
	{
		Key = m_pData->aLoadSteps[nCount].nStep;
		Data.dLoadIncFactor = m_pData->aLoadSteps[nCount].dLoadIncFactor;
		Data.bSave = m_pData->aLoadSteps[nCount].bSave;
		SetValue(GetParam()->GetData(), nCount + 1, Key, Data, awCols);
	}
}

void CNXNlctLoadStepGrid::MakeItemEditingRecord(ROWCOL ncRow, CStringArray& EditingRecord)
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	int Key;
	T_NLCT_LOADSTEP Data;
	for (int i = 0; i < GetColCount(); i++)
	{
		if (i == 0)Key = _ttoi(EditingRecord[i]);
		else ConvStrToData(i, EditingRecord[i], Data);
	}
	SetValue(ncRow, Key, Data);
}

BOOL CNXNlctLoadStepGrid::OnPasteFromClipboard(const CGXRange &range)
{
	if (IsLockEdit()) return FALSE; // display only mode이면 바로 리턴
	return CTBCommon::OnPasteFromClipboard(range);
}

BOOL CNXNlctLoadStepGrid::Copy()
{
	return CTBCommon::Copy();
}

BOOL CNXNlctLoadStepGrid::OnInsertEmptyRecord(ROWCOL nRow)
{
	BOOL bActive;
	ROWCOL nCol, ncRow, ncCol;
	CString value;

	VERIFY(GetCurrentCell(ncRow, ncCol));
	CGXControl* pControl = (CGXControl*)GetControl(ncRow, ncCol);
	if (pControl&&pControl->IsActive())bActive = TRUE;
	else bActive = FALSE;

	VERIFY(GetColumnNoFor(m_aTitle[0], nCol));
	CString strStepNo;
	if (nRow > 1)
	{
		strStepNo = GetValueRowCol(nRow - 1, 1);
		int nStep = _ttoi(strStepNo) + 1;
		strStepNo.Format(_T("%d"), nStep);
	}
	else
		strStepNo = _T("1");
	if (nCol != ncCol || !bActive)SetExpressionRowCol(nRow, nCol, strStepNo);
	if (ncCol == 3)
	{
		VERIFY(GetColumnNoFor(m_aTitle[1], nCol));
		T_NLCT_LOADSTEP temp; temp.Initialize();

		double lfac = 1.0;
		int nrow = int(nRow);
		if (m_nOldRowCount <= nrow)
		{
			lfac = m_pData->aLoadSteps[nrow - 1].dLoadIncFactor;
			temp.dLoadIncFactor = lfac;
		}

		CString strIncreFact;
		strIncreFact.Format(DATA_FMT, temp.dLoadIncFactor);
		if (nCol != ncCol || !bActive)SetExpressionRowCol(nRow, nCol, strIncreFact);
	}



	return TRUE;
}

void CNXNlctLoadStepGrid::Initialize(T_NLCT_DATA* pData)
{
	ASSERT(pData);
	m_pData = pData;
	m_nOldRowCount = m_pData->aLoadSteps.GetCount();

	T_UNIT_SYSTEM unitSystem;
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	CTBCommon::Initialize(pDoc, unitSystem);

	GetParam()->EnableUndo(FALSE);
	GetParam()->EnableTrackRowHeight(FALSE);
	GetParam()->EnableMoveCols(FALSE);

	CGXProperties* pProp = GetParam()->GetProperties();
	pProp->SetMarkColHeader(FALSE);
	pProp->SetMarkRowHeader(FALSE);
	pProp->SetUserProperty(GX_IDS_OUTLINECURRENTCELL, (CGXStyle)pProp->sInvertNoBorder);

	LockUpdate(TRUE);

	int nColCount = GetInitialColCount();
	SetColCount(nColCount);
	SetHeaderTitle(unitSystem, TRUE);

	//표준 스타일 변경
	ChangeStandardStyle(CGXStyle()
		.SetFont(CGXFont_GC().SetSize(9))
		.SetVerticalAlignment(DT_BOTTOM)
		.SetAllowEnter(FALSE));

	//Column의 Header스타일 변경
	ChangeColHeaderStyle(CGXStyle().SetFont(CGXFont_GC().SetBold(FALSE)));

	//Row의 Header스타일 변경
	ChangeRowHeaderStyle(CGXStyle()
		.SetFont(CGXFont_GC().SetBold(FALSE))
		.SetHorizontalAlignment(DT_RIGHT)
		.SetControl(GX_IDS_CTRL_HEADER));

	SetRowHeight(0, 0, 36);

	//컬럼 스타일 변경
	SetStyleRange(CGXRange().SetCols(1, nColCount), CGXStyle()
		.SetHorizontalAlignment(DT_RIGHT)
		.SetVerticalAlignment(DT_VCENTER)
		.SetValueType(GX_VT_NUMERIC)
		.SetFormat(GX_FMT_FIXED)
		.SetPlaces(6));

	//Step 컬럼 스타일 변경
	SetStyleRange(CGXRange().SetCols(1), CGXStyle()
		.SetPlaces(0)
		.SetEnabled(FALSE));

	//Save Step 컬럼 스타일 변경
	SetStyleRange(CGXRange().SetCols(3), CGXStyle()
		.SetControl(GX_IDS_CTRL_CHECKBOX3D)
		.SetHorizontalAlignment(DT_CENTER)
		.SetPlaces(0)
		.SetValue(_T("0")));

	SetScrollBarMode(SB_VERT, gxnEnabled | gxnEnhanced);
	SetRightValidCell(1, 1);
	GetParam()->SetHideCurrentCell(GX_HIDE_NEVER);

	m_aSortInfo.SetSize(1);
	m_aSortInfo[0].nRC = 0;
	m_aSortInfo[0].sortType = CGXSortInfo::numeric;
	m_aSortInfo[0].sortOrder = CGXSortInfo::ascending;
	
	LockUpdate(FALSE);
}

BOOL CNXNlctLoadStepGrid::GetValue(ROWCOL nRow, int &key, T_NLCT_LOADSTEP &data)
{
	data.Initialize();
	ROWCOL nCol;
	CString str;
	for (int i = 0; i < GetColCount(); i++)
	{
		VERIFY(GetColumnNoFor(m_aTitle[i], nCol));
		str = GetValueRowCol(nRow, nCol);
		VERIFY(ConvertFormulaToValue(str));

		if (i == 0)
		{
			key = _ttoi(str);
			data.nStep = key;
		}
		else ConvStrToData(i, str, data);
	}
	return TRUE;
}

BOOL CNXNlctLoadStepGrid::SetValue(ROWCOL nRow, const int &key, const T_NLCT_LOADSTEP &data)
{
	CRowColArray awCols;
	awCols.SetSize(GetColCount());
	for (int i = 0; i < GetColCount(); i++)
		VERIFY(GetColumnNoFor(m_aTitle[i], awCols[i]));
	return SetValue(GetParam()->GetData(), nRow, key, data, awCols);
}

BOOL CNXNlctLoadStepGrid::SetValue(CGXData* pData, ROWCOL nRow, const int &key, const T_NLCT_LOADSTEP &data, CRowColArray &awCols)
{
	CString str;
	for (int i = 0; i < GetColCount(); i++)
	{
		if (i == 0)str.Format(_T("%d"), key);
		else ConvDataToStr(i, data, str);
		pData->StoreValueRowCol(nRow, awCols[i], str, gxOverride, GX_VT_NUMERIC);
	}
	return TRUE;
}

void CNXNlctLoadStepGrid::ConvStrToData(int i, CString& value, T_NLCT_LOADSTEP& data)
{
	if (i == 1)data.dLoadIncFactor = _tstof(value);
	if (i == 2)data.bSave = _ttoi(value) == 0 ? FALSE : TRUE;
}

void CNXNlctLoadStepGrid::ConvDataToStr(int i, const T_NLCT_LOADSTEP& data, CString& value)
{
	if (i == 1) value.Format(DATA_FMT, data.dLoadIncFactor);
	if (i == 2)value.Format(_T("%d"), data.bSave == TRUE ? 1 : 0);
}

BOOL CNXNlctLoadStepGrid::AddData(int key, T_NLCT_LOADSTEP data)
{
	if (key > m_nOldRowCount)
	{
		ASSERT(0);
		return FALSE;
	}

	CArray<int, int> aOldKey, aKey;
	CArray<T_NLCT_LOADSTEP, T_NLCT_LOADSTEP&> aData;
	aKey.Add(key);
	aData.Add(data);
	T_NLCT_LOADSTEP item;
	item.nStep = key;
	item.dLoadIncFactor = data.dLoadIncFactor;
	item.bSave = data.bSave;

	int nPos;
	if (FindData(key, nPos, 0))
	{
		//CString msg;
		//msg.Format(_LS(IDS_WG_CMD2__ADDD__Step__d_already_exist_), key);
		//AfxMessageBox(msg);
		//return FALSE;

		// 이미 존재하면 업데이트 (에러 대신)
		//m_pData->aLoadSteps[nPos].dLoadIncFactor = data.dLoadIncFactor;
		//m_pData->aLoadSteps[nPos].bSave = data.bSave;
		m_pData->aLoadSteps[nPos] = data;
		
		// Grid UI 업데이트
		ROWCOL nRow = nPos + 1;
		SetValueRange(CGXRange(nRow, 2), data.dLoadIncFactor);
		SetValueRange(CGXRange(nRow, 3), data.bSave ? _T("1") : _T("0"));
	}
	else
	{
		// 존재하지 않으면 추가
		m_pData->aLoadSteps.InsertAt(nPos, item);
	}

	UpdateBuffer(0, aOldKey, aKey, aData);
	DBChanged();
	return TRUE;
}

BOOL CNXNlctLoadStepGrid::AddData(CArray<int, int> &aKey, CArray<T_NLCT_LOADSTEP, T_NLCT_LOADSTEP&> &aData, BOOL bNotify /*= TRUE*/)
{
	CArray<int, int> aOldKey;
	int nSize = aKey.GetSize();
	aOldKey.Copy(aKey);
	qsort((void*)aOldKey.GetData(), nSize, sizeof(int), CCompFunc::intAsc);

	BOOL bSame;
	int nPos = 0, nStart;
	for (int i = 0; i < nSize; i++)
	{
		nStart = nPos;
		bSame = FindData(aOldKey[i], nPos, nStart);
		if (!bSame&&i > 0)
			bSame = (aOldKey[i] - aOldKey[i - 1] == 0);
		if (bSame)
		{
			CString msg;
			msg.Format(_LS(IDS_WG_CMD2__ADDD__Step__d_already_exist_), aOldKey[i]);
			AfxMessageBox(msg);
			return FALSE;
		}
		nPos = nStart;
	}
	for (int i = 0; i < nSize; i++)
	{
		bSame = FindData(aKey[i], nPos, 0);
		VERIFY(!bSame);
		T_NLCT_LOADSTEP item;
		item.nStep = aKey[i];
		item.dLoadIncFactor = aData[i].dLoadIncFactor;
		item.bSave = aData[i].bSave;
		m_pData->aLoadSteps.InsertAt(nPos, item);
	}
	UpdateBuffer(0, aOldKey, aKey, aData);
	if (bNotify)DBChanged();
	return TRUE;

}

BOOL CNXNlctLoadStepGrid::DelData(int key)
{
	CArray<int, int> aOldKey, aKey;
	CArray<T_NLCT_LOADSTEP, T_NLCT_LOADSTEP&> aData;
	aKey.Add(key);
	T_NLCT_LOADSTEP temp; temp.Initialize();
	aData.Add(temp);

	int nPos;
	if (!FindData(key, nPos))
	{
		CString msg;
		msg.Format(_LS(IDS_WG_CMD2__ADDD__Step__d_does_not_exist_), key);
		AfxMessageBox(msg);
		return FALSE;
	}
	m_pData->aLoadSteps.RemoveAt(nPos);
	UpdateBuffer(1, aOldKey, aKey, aData);

	int nRowCount = GetRowCount();
	aOldKey.RemoveAll();
	aKey.RemoveAll();
	aData.RemoveAll();

	int nKey, nPrevKey = 0, nIdx = 1;
	for (int i = 0; i < nRowCount; i++)
	{
		ROWCOL nRow = (ROWCOL)i + 1;
		temp.Initialize();
		if(!GetValue(nRow, nKey, temp))continue;
		if (nKey > key)
		{
			aOldKey.Add(nKey);
			aKey.Add(nPrevKey + nIdx);
			aData.Add(temp);
			nIdx++;
		}
		else
		{
			if (nPrevKey < nKey)nPrevKey = nKey;
		}
	}
	ModifyData(aOldKey, aKey, aData);

	DBChanged();
	return TRUE;
}

BOOL CNXNlctLoadStepGrid::DelData(CArray<int, int> &aKey, BOOL bNotify /*= TRUE*/)
{
	CArray<int, int> aOldKey;
	CArray<T_NLCT_LOADSTEP, T_NLCT_LOADSTEP&> aData;
	int nSize = aKey.GetSize();
	T_NLCT_LOADSTEP temp; temp.Initialize();
	for (int i = 0; i < nSize; i++)aData.Add(temp);
	aOldKey.Copy(aKey);
	qsort((void*)aOldKey.GetData(), nSize, sizeof(int), CCompFunc::intAsc);
	int errKey;
	if (!ExistAllKey(aOldKey, errKey))
	{
		CString msg;
		msg.Format(_LS(IDS_WG_CMD2__ADDD__Step__d_does_not_exist_), errKey);
		AfxMessageBox(msg);
		return FALSE;
	}
	int nSize2 = m_pData->aLoadSteps.GetSize();
	int j = nSize2 - 1;
	for (int i = nSize - 1; i >= 0; i--)
	{
		j = m_pData->aLoadSteps.GetSize() - 1;
		for (; j >= 0; j--)
		{
			if (aOldKey[i] == m_pData->aLoadSteps[j].nStep)
			{
				m_pData->aLoadSteps.RemoveAt(j);
				break;
			}
		}
	}
	UpdateBuffer(1, aOldKey, aKey, aData);

	CArray<int, int> aNewKey;
	aOldKey.RemoveAll();
	aNewKey.RemoveAll();
	aData.RemoveAll();

	
	int nKey;
	for (int i = 0; i < GetRowCount(); i++)
	{
		ROWCOL nRow = (ROWCOL)i + 1;
		temp.Initialize();
		if (!GetValue(nRow, nKey, temp))continue;
		if (nKey != i + 1)
		{
			aOldKey.Add(nKey);
			aNewKey.Add(i + 1);
			aData.Add(temp);
		}
	}
	ModifyData(aOldKey, aNewKey, aData);

	if (bNotify)DBChanged();
	return TRUE;
}

BOOL CNXNlctLoadStepGrid::ModifyData(int oldKey, int key, T_NLCT_LOADSTEP data)
{
	CArray<int, int> aOldKey, aKey;
	CArray<T_NLCT_LOADSTEP, T_NLCT_LOADSTEP&> aData;
	aOldKey.Add(oldKey);
	aKey.Add(key);
	aData.Add(data);

	int nPos;
	if (!FindData(oldKey, nPos))
	{
		CString msg;
		msg.Format(_LS(IDS_WG_CMD2__ADDD__Step__d_does_not_exist_), oldKey);
		AfxMessageBox(msg);
		return FALSE;
	}
	
	if (oldKey == key)
	{
		m_pData->aLoadSteps[nPos] = data;

		//// Step 번호가 동일한 경우 데이터만 업데이트
		//m_pData->aLoadSteps[nPos].dLoadIncFactor = data.dLoadIncFactor;
		//m_pData->aLoadSteps[nPos].bSave = data.bSave;
		//
		//// Grid UI만 직접 업데이트 (UpdateBuffer 호출 안 함)
		//ROWCOL nRow = nPos + 1;  // 1-based indexing
		//SetValueRange(CGXRange(nRow, 2), data.dLoadIncFactor);
		//SetValueRange(CGXRange(nRow, 3), data.bSave ? _T("1") : _T("0"));
		//
		//DBChanged();
		//return TRUE;
	}
	else
	{
		int nPos2;
		if (FindData(key, nPos2))
		{
			CString msg;
			msg.Format(_LS(IDS_WG_CMD2__ADDD__Step__d_already_exist_), key);
			AfxMessageBox(msg);
			return FALSE;
		}
		T_NLCT_LOADSTEP item;
		item.nStep = key;
		item.dLoadIncFactor = data.dLoadIncFactor;
		item.bSave = data.bSave;
		m_pData->aLoadSteps.RemoveAt(nPos);
		if (nPos2 > nPos)nPos2--;
		m_pData->aLoadSteps.InsertAt(nPos2, item);
	}
	UpdateBuffer(2, aOldKey, aKey, aData);
	DBChanged();
	return TRUE;
}

BOOL CNXNlctLoadStepGrid::ModifyData(CArray<int, int> &aOldKey, CArray<int, int> &aKey, CArray<T_NLCT_LOADSTEP, T_NLCT_LOADSTEP&> &aData)
{
	CArray<T_NLCT_LOADSTEP, T_NLCT_LOADSTEP&> aOrgData;
	aOrgData.Copy(m_pData->aLoadSteps);

	if (!DelData(aOldKey, FALSE))
		return FALSE;
	if (!AddData(aKey, aData, FALSE))
 	{
		m_pData->aLoadSteps.Copy(aOrgData);
		return FALSE;
	}
	DBChanged();
	return TRUE;
}

void CNXNlctLoadStepGrid::MakeSearchKey(const int &key, const T_NLCT_LOADSTEP &data, CStringArray &aKey)
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

BOOL CNXNlctLoadStepGrid::ExistAllKey(CArray<int, int>& aKey, int& errorKey)
{
	CArray<int, int> aTempKey;

	int nSize = aKey.GetSize();
	aTempKey.Copy(aKey);
	BOOL bSame;
	int nPos = 0, nStart;
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

BOOL CNXNlctLoadStepGrid::FindData(int key, int& nPos, int nStart/*=0*/)
{
	int i;
	int nSize = m_pData->aLoadSteps.GetSize();

	CString csVal;
	int nVal;
	int nCompResult;
	for (i = nStart; i < nSize; i++)
	{
		csVal.Format(KEY_FMT, m_pData->aLoadSteps[i].nStep);
		nVal = _ttoi(csVal);
		nCompResult = key - nVal;
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

void CNXNlctLoadStepGrid::SetHeaderTitle(T_UNIT_SYSTEM &UnitSystem, BOOL bInit /*= FALSE*/)
{
	int aSize[] = { 50, 75, 65 };
	CString strTitle;
	for (int i = 0; i < GetColCount(); i++)
	{
		strTitle = m_aTitle[i];
		SetStyleRange(CGXRange(0, i + 1), CGXStyle()
			.SetValue(strTitle)
			.SetUserAttribute(ID_USER_ATTR_COL_ID, m_aTitle[i]));
		if (bInit)SetColWidth(i + 1, i + 1, aSize[i]);
	}
	if (bInit)SetColWidth(0, 0, 15);
}

BOOL CNXNlctLoadStepGrid::ValidateRecord(CStringArray &value, CRowColArray &aCols)
{
	CString msg = _T("");
	int nPos;

	for (int i = 0; i < GetColCount(); i++)
	{
		nPos = aCols[i] - 1;
		if (nPos == 0)
		{
			CString strValue;
			strValue.Format(_T("%d"), i+1);
			value[nPos] = strValue;
		}
		else if (nPos == 2 && value[nPos].IsEmpty())
		{
			CString strValue;
			strValue.Format(_T("%d"), 1);
			value[nPos] = strValue;
		}
		if (value[nPos].IsEmpty())
			msg += ((!msg.IsEmpty()) ? _T("\n") : _T("")) + m_aTitle[i] + _LS(IDS_WG_CMD__ADDD___has_not_been_entered_);
		else if (!ValidateField(value[nPos], i)) return FALSE;
	}
	if (msg.IsEmpty()) return TRUE;
	else SetWarningText(msg);
	return FALSE;
}

BOOL CNXNlctLoadStepGrid::ValidateAndMakeRecord(CStringArray &value, CRowColArray &aCols, int &key, T_NLCT_LOADSTEP &data)
{
	if (!ValidateRecord(value, aCols))
	{
		//SetWarningText("");
		return FALSE;
	}

	// 레코드를 만들어서 리턴한다.
	int nPos;
	for (int i = 0; i < GetColCount(); i++)
	{
		nPos = aCols[i] - 1; // col id i에 해당하는 value가 저장된 위치
		if (i == 0) key = _tstoi(value[nPos]);
		else ConvStrToData(i, value[nPos], data);
	}
	return TRUE;
}

BOOL CNXNlctLoadStepGrid::ValidateField(CString value, int nColID)
{
	if (nColID >= GetColCount())
	{
		SetWarningText(_LS(IDS_WG_CMD__ADDD___CNSNlctLoadStepGrid__ValidateField__Er));
		return FALSE;
	}

	int nErrorCode;
	if (nColID == 0 || nColID == 2)
	{
	}
	else if (nColID == 1)
	{
		if (!IsValidDoubleNumber(value, nErrorCode))
		{
			switch (nErrorCode)
			{
			case 1: SetWarningText(_LS(IDS_WG_CMD__ADDD__Invalid_formula_)); break;
			case 2: SetWarningText(_LS(IDS_WG_CMD__ADDD__Charater_exist_in_double_value)); break;
			case 3: SetWarningText(_LS(IDS_WG_CMD__ADDD__Value_is_too_big_)); break;
			}
			return FALSE;
		}

		//negative 가능
		//double dTemp = _tstof(value);
		//if (dTemp <= 0.)
		//{
		//	SetWarningText(_LS(IDS_WG_CMD2_ADDD_Value_is_negative_number));
		//	return FALSE;
		//}
	}

	return TRUE;	
}

void CNXNlctLoadStepGrid::UpdateBuffer(int nCmd, CArray<int, int> &aOldKey, CArray<int, int> &aKey, CArray<T_NLCT_LOADSTEP, T_NLCT_LOADSTEP&> &aData)
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

void CNXNlctLoadStepGrid::DBChanged()
{
	CMNXNlctLoadStepDlg* pDlg = (CMNXNlctLoadStepDlg*)GetParent();
	if (!pDlg)return;
	if (pDlg->GetSafeHwnd() == 0)return;
	if (!IsWindow(pDlg->GetSafeHwnd()))return;
	pDlg->OnDataChanged();
}