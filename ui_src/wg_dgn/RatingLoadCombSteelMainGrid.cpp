// RatingLoadCombSteelMainGrid.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "RatingLoadCombSteelMainGrid.h"

#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "RatingLoadCombCommonMainGrid.h"
#include "..\wg_db\AttrCtrl2.h"

#include "RatingLoadCombSteelSubGrid.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRatingLoadCombSteelMainGrid
CRatingLoadCombSteelMainGrid::CRatingLoadCombSteelMainGrid()
: CRatingLoadCombCommonMainGrid()
{
	
}

CRatingLoadCombSteelMainGrid::~CRatingLoadCombSteelMainGrid()
{
}


BEGIN_MESSAGE_MAP(CRatingLoadCombSteelMainGrid, CRatingLoadCombCommonMainGrid)
	//{{AFX_MSG_MAP(CRatingLoadCombSteelMainGrid)

	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CRatingLoadCombSteelMainGrid::InsertRecordByKey(const T_RLCS_K &key, const T_RLCS_D &data)
{
	ROWCOL nRow;

	if(!GetInsertRowByKey(key, nRow))
	{
		ASSERT(0);
		return FALSE;
	}

	VERIFY(InsertRecord(nRow));
	return SetValue(nRow, key, data);
}

BOOL CRatingLoadCombSteelMainGrid::DeleteRecordByKey(const T_RLCS_K &key, const T_RLCS_D &data)
{
	ROWCOL nRow;

	if(!GetRowNoByKey(key, nRow))
	{
		ASSERT(0);
		return FALSE;
	}

	return DeleteRecord(nRow);
}

// 이 함수는 Paste할 때 호출되므로 False를 리턴할 때는 ValidateField와 
// Validate Record에서 설정된 Warning Text를 없애주고 리턴한다.
BOOL CRatingLoadCombSteelMainGrid::ValidateAndMakeRecord(CStringArray &value, CRowColArray& aCols, T_RLCS_K &key, T_RLCS_D &data)
{
	// 각 필드에 대해 Validation을 검사한다.
	for (int i = 0; i < value.GetSize(); i++)
	{
		if (!ValidateField(value[aCols[i]-1], i)) 
		{
			SetWarningText(_T(""));
			return FALSE;
		}
	}
	// 레코드에 대해 Validation을 검사한다.
	if (!ValidateRecord(value, aCols))
	{
		SetWarningText(_T(""));
		return FALSE;
	}

	// 레코드를 만들어서 리턴한다.
	for (int i = 0; i < GetColCount(); i++)
	{
		int nPos = aCols[i]-1; // col id i에 해당하는 value가 저장된 위치
		if(i==0) 
		{
			if (value[nPos] == _T("")) key = 0;
			else key = _ttol(value[nPos]);
		}
		else ConvStrToData(i, value[nPos], data);
	}
	return TRUE;
}

BOOL CRatingLoadCombSteelMainGrid::AddToDB(ROWCOL nRow)
{
	// Record를 만들어서 DB에 입력한다.
	T_RLCS_K key;
	T_RLCS_D data;
	data.Initialize();
 
	GetValue(nRow, key, data);

	return m_pDoc->m_pDataCtrl->AddRlcs(key, data);
}

BOOL CRatingLoadCombSteelMainGrid::AddToDB(CArray<void*, void*>& raKey, CArray<void*, void*>& raData)
{
	ASSERT(0);

	ASSERT(raKey.GetSize() == raData.GetSize());
	T_RLCS_K tKey;
	CArray<T_RLCS_K, T_RLCS_K> aKey;
	CArray<T_RLCS_D, T_RLCS_D&> aData;
	for (int i = 0; i < raKey.GetSize(); i++)
	{
		tKey = *((T_RLCS_K*)raKey.GetAt(i));
		aKey.Add(tKey);
		aData.Add(*((T_RLCS_D*)raData.GetAt(i)));
	}
	return m_pDoc->m_pDataCtrl->AddRlcs(aKey, aData);
}


BOOL CRatingLoadCombSteelMainGrid::DeleteFromDB(ROWCOL nRow)
{
	T_RLCS_K key;
	T_RLCS_D data;
	data.Initialize();
 
	GetValue(nRow, key, data);
	return m_pDoc->m_pDataCtrl->DelRlcs(key);
}

BOOL CRatingLoadCombSteelMainGrid::DeleteFromDB(CRowColArray& awRows)
{
	if (awRows.GetSize() <= 0) return TRUE;

	ROWCOL nCol;
	nCol = 1;

	CString value;
	CArray<T_RLCS_K, T_RLCS_K> aKey;

	for (int i = 0; i < awRows.GetSize(); i++)
	{
		value = GetValueRowColBeforeEdit(awRows.GetAt(i), nCol);
		UINT nKey = (UINT)_ttoi(value);
		aKey.Add(nKey);
	}

	return m_pDoc->m_pDataCtrl->DelRlcs(aKey);
}

BOOL CRatingLoadCombSteelMainGrid::ModifyToDB(ROWCOL nRow)
{
	ROWCOL nCol;
	nCol = 1;

	T_RLCS_K key;
	T_RLCS_D data;
	data.Initialize();
	CString pOldLoadCombName;

	pOldLoadCombName = GetValueRowColBeforeEdit(nRow, nCol);
	GetValue(nRow, key, data);

	// 값이 변경되지 않았다면 DB에 셋팅 하지 않는다.
	if(!IsDataChange(key, data))
	{
		return TRUE;
	}

	// 수정한다. 실패하면 에러 리턴
	return m_pDoc->m_pDataCtrl->ModifyRlcs(key, key, data); 
}

BOOL CRatingLoadCombSteelMainGrid::IsDataChange(T_RLCS_K key, T_RLCS_D& dataNew)
{
	T_RLCS_D dataOld;
	m_pDoc->m_pAttrCtrl2->GetRlcs(key, dataOld);
	if(dataOld == dataNew)
	{
		return FALSE;
	}
	else
	{
		return TRUE;
	}
}

BOOL CRatingLoadCombSteelMainGrid::ModifyToDB(CStringArray& raOldKey, 
												 CArray<void*, void*>& raKey, 
												 CArray<void*, void*>& raData)
{
	ASSERT(raKey.GetSize() == raData.GetSize());
	ASSERT(raKey.GetSize() == raOldKey.GetSize());  
	CArray<T_RLCS_D, T_RLCS_D&> aData;
	CArray<T_RLCS_K, T_RLCS_K> aKey;
	T_RLCS_K Key;
	T_RLCS_D Data;
	Data.Initialize();
	for (int i = 0; i < raKey.GetSize(); i++)
	{
		Key = _ttoi(raOldKey.GetAt(i));
		aKey.Add(Key);
		m_pDoc->m_pAttrCtrl2->GetRlcs(Key, Data);
		aData.Add(*((T_RLCS_D*)raData.GetAt(i)));
	}

	return m_pDoc->m_pDataCtrl->ModifyRlcs(aKey, aKey, aData);
}

BOOL CRatingLoadCombSteelMainGrid::MakeKeyAndDataArray(CStringArray& value, CRowColArray& awCols, 
																	CArray<void*, void*>& aKey, CArray<void*, void*>& aData)
{
	T_RLCS_K* pKey;
	T_RLCS_D* pData;
	pKey = new T_RLCS_K;
	pData = new T_RLCS_D;
	pData->Initialize();
	aKey.Add((void*)pKey);
	aData.Add((void*)pData);
	if (!ValidateAndMakeRecord(value, awCols, *pKey, *pData)) return FALSE;
	return TRUE;
}

void CRatingLoadCombSteelMainGrid::DeleteKeyAndDataArray(CArray<void*, void*>& aKey, CArray<void*, void*>& aData)
{
	T_RLCS_K* pKey;
	T_RLCS_D* pData;
	ASSERT(aKey.GetSize() == aData.GetSize());
	for (int i = 0; i < aKey.GetSize(); i++)
	{
		pKey = (T_RLCS_K*)aKey.GetAt(i);
		pData = (T_RLCS_D*)aData.GetAt(i);
		delete pKey;
		delete pData;
	}
}

long CRatingLoadCombSteelMainGrid::GetCountData()
{
	CDBDoc* pDoc = m_pDoc;
	T_RLCS_D tData;
	tData.Initialize();
	return pDoc->m_pAttrCtrl2->GetCountRlcs();
}

void CRatingLoadCombSteelMainGrid::MakeItemNormal(long nNumData, CRowColArray& awCols)
{
	T_RLCS_K Key;
	T_RLCS_D Data;
	Data.Initialize();
	CDBDoc* pDoc = m_pDoc;

	long* KeyBuf = new long[nNumData];
	long nCount = 0;
	POSITION pos = pDoc->m_pAttrCtrl2->GetStartRlcs();
	while(pos != NULL)
	{
	  pDoc->m_pAttrCtrl2->GetNextRlcs(pos,Key,Data);
	  KeyBuf[nCount++] = Key;
	}
	qsort(KeyBuf, nNumData, sizeof(T_LCOM_K), CTBBrowserWnd::CompareKey);

	for(nCount=0 ; nCount < nNumData; nCount++)
	{
		pDoc->m_pAttrCtrl2->GetRlcs(KeyBuf[nCount], Data);
		SetValue(GetParam()->GetData(), nCount+1, KeyBuf[nCount], Data, awCols);
	} // end of loop nCount

	delete []KeyBuf;
}

void CRatingLoadCombSteelMainGrid::MakeItemEditingRecord(ROWCOL ncRow, CStringArray& EditingRecord)
{
	CDBDoc* pDoc = m_pDoc;
	T_RLCS_K Key;
	T_RLCS_D Data;
	Data.Initialize();
	for (int i = 0; i < GetColCount(); i++)
	{
		if (i == 0)		
		{
			Key = _ttol(EditingRecord[i]);
		}
		else
		{
			ConvStrToData(i, EditingRecord[i], Data);
		}
	}
	//pDoc->m_pUnitCtrl->ConvertUnitLcomPrevious(Data);	// load combination은 Unit과 관계없다.
	// 현재 값을 설정한다.
	SetValue(ncRow, Key, Data);
}

/////////////////////////////////////////////////////////////////////////////
// CRatingLoadCombSteelMainGrid Implementation
BOOL CRatingLoadCombSteelMainGrid::GetValue(ROWCOL nRow, T_RLCS_K &key, T_RLCS_D &data)
{
	ROWCOL nCol;
	CString str;
	data.Initialize();
	for (int i = 0; i < GetColCount(); i++)
	{
		VERIFY(GetColumnNoFor(m_aTitle[i], nCol));
		str = GetValueRowCol(nRow, nCol);
		// 수식인 경우 값으로 변환한다.
		// Validation이 OK인 Field이므로 항상 TRUE 리턴
		VERIFY(ConvertFormulaToValue(str)); 

		if(i==0) 
		{
			if (str == _T(""))
			{
				key = 1;
				ASSERT(0);
			}
			else
				key = _ttol(str);
		}
		else ConvStrToData(i, str, data);
	}
	// Sub grid에 있는 값을 가져와서 설정한다.
	((CRatingLoadCombSteelSubGrid*)m_pSubGrid)->GetData(data);
	((CRatingLoadCombSteelSubGrid*)m_pSubGridMV)->GetData(data);
	
	return TRUE;
}
BOOL CRatingLoadCombSteelMainGrid::SetValue(ROWCOL nRow, const T_RLCS_K &key, const T_RLCS_D &data)
{
	//
	CRowColArray awCols;
	awCols.SetSize(GetColCount());
	for (int i = 0;i < GetColCount(); i++)
		VERIFY(GetColumnNoFor(m_aTitle[i], awCols[i]));
	return SetValue(GetParam()->GetData(), nRow, key, data, awCols);
}

BOOL CRatingLoadCombSteelMainGrid::SetValue(CGXData* pData, ROWCOL nRow, const T_RLCS_K &key, const T_RLCS_D &data, CRowColArray &awCols)
{
	//
	CString str;
	for (int i = 0; i < GetColCount(); i++)
	{
		if(i==0) 
		{
			if (key == 0) str.Empty();
			else str.Format(LCOM_KEY_FMT,key);
		}
		else ConvDataToStr(i, data, str);
		pData->StoreValueRowCol(nRow, awCols[i], str, gxOverride);

		// END KJH20050203
	}

	return TRUE;
}

void CRatingLoadCombSteelMainGrid::ConvStrToData(int i, CString& value, T_RLCS_D& data)
{
	//if (i==1) data.LoadCombId = _ttol(value);  // Serial No
	if (i==2) data.strCaseName = value; // name
	//else if (i==3) data.LoadCombType = GetCombType(value); // type
	else if (i==4) data.strDescription = value; // description
//  else if (i==5) data.nActive = GetActive(m_nLcomType, value);
//  else if (i==6) data.bElasticStage = (value==_T("1")); // MNET:1209, KJH20050203
}

void CRatingLoadCombSteelMainGrid::ConvDataToStr(int i, const T_RLCS_D& data, CString& value)
{
	//if (i==1) value.Format(_T("%d"), data.LoadCombId);
	if (i==2) value = data.strCaseName;
//  else if (i==3) value = GetCombTypeStr(data.LoadCombType);
	else if (i==4) value = data.strDescription;
//  else if (i==5) value = GetActiveName(m_nLcomType, data.nActive);
//  else if (i==6) value = data.bElasticStage ? _T("1") : _T("0"); // MNET:1209, KJH20050203
}

// Main Grid, Sub Grid간의 동기화  
void CRatingLoadCombSteelMainGrid::OnChangeCurrentRecord(ROWCOL nRow)
{
	ROWCOL nkCol;
	GetColumnNoFor(GetKeyColID(), nkCol);
	CString sKey = GetKeyForRow(nRow, nkCol);
	m_pSubGrid->SetData(sKey);
	m_pSubGridMV->SetData(sKey);

	T_RLCS_K key;
	T_RLCS_D data;	
	data.Initialize();
	key = _ttol(sKey);  
	m_pDoc->m_pAttrCtrl2->GetRlcs(key, data);
	CString szTemp;
	szTemp.Format(_T("%g"), data.dImpFactor);

	m_bIgnoreSetEditMode = TRUE;
	//m_pEdtImpactFactor->SetWindowText(szTemp);
	m_bIgnoreSetEditMode = FALSE;
}

CString CRatingLoadCombSteelMainGrid::GetNewKey()
{
	CArray<T_RLCS_K, T_RLCS_K> aKeyList;  
	m_pDoc->m_pAttrCtrl2->GetRlcsKeyList(aKeyList);
	
	int i;
	// aKeyList가 Sorting이 되어 있다고 가정..
#ifdef _DEBUG
	for(i = 0; i < aKeyList.GetSize(); i++)
	{
		for(int j = i + 1; j < aKeyList.GetSize(); j++)
		{
			ASSERT(aKeyList[i] < aKeyList[j]);  // 여기에 ASSERT가 걸린다면 aKeyList를 Sorting 해야 함..
		}
	}
#endif

	T_RLCS_K KeyTemp = 1;
	for(i =0; i < aKeyList.GetSize(); i++)
	{
		if(aKeyList[i] != KeyTemp)
		{
			break;
		}		
		KeyTemp++;
	}

	if(aKeyList.GetSize())
	{
		if(aKeyList[0] < 1)  
		{
			ASSERT(0);//         hslee : Key가 0부터 시작하는 경우... 여기 걸린다면 원인을 찾아서 Key가 1부터 시작하도록 고치자..
			ASSERT(aKeyList[0] == 0); // hslee : Key가 음수인 경우... 여기 걸린다면 원인을 찾아서 Key가 1부터 시작하도록 고치자..
			KeyTemp = aKeyList[0];
		}
	}
	
	CString szTemp;
	szTemp.Format(LCOM_KEY_FMT, KeyTemp);
	return szTemp;
}

// LoadComb의 위치를 이동(Serial No가 변경)한다.
BOOL CRatingLoadCombSteelMainGrid::OnSelDragRowsDrop(ROWCOL nFirstRow, ROWCOL nLastRow, ROWCOL nDestRow)
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
	
	CArray<CString, CString&> aLoadCombName;
	CArray<T_RLCS_D, T_RLCS_D&> aData;
	CArray<T_RLCS_K, T_RLCS_K> aKey;
	CString LoadCombName;
	T_RLCS_K key;
	T_RLCS_D data;
	data.Initialize();
	for (int i = nFirstRow; i <= nLastRow; i++)
	{
		GetValue(i, key, data);
		// Get Value만 하면 Combination이 현재 Record의 Combination으로 치환된다.
		// 여기서는 위치만 이동되므로 LoadCombId만 변경된다. 그래서 원래의 Combination
		// 을 유지하기 위해 GetLcom을 다시 호출한다.
		m_pDoc->m_pAttrCtrl2->GetRlcs(key, data);
		//data.LoadCombId = nStart;
		nStart += nInc;
		
		aKey.Add(key);
		aData.Add(data);
	}

	m_pDoc->m_pDataCtrl->ModifyRlcs(aKey, aKey, aData);
	return FALSE;
}