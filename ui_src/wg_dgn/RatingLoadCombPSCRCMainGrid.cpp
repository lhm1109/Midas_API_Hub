// RatingLoadCombPSCRCMainGrid.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "RatingLoadCombPSCRCMainGrid.h"

#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "RatingLoadCombCommonMainGrid.h"
#include "..\wg_db\AttrCtrl2.h"

#include "RatingLoadCombPSCRCSubGrid.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRatingLoadCombPSCRCMainGrid
CRatingLoadCombPSCRCMainGrid::CRatingLoadCombPSCRCMainGrid()
: CRatingLoadCombCommonMainGrid()
{
	
}

CRatingLoadCombPSCRCMainGrid::~CRatingLoadCombPSCRCMainGrid()
{
}


BEGIN_MESSAGE_MAP(CRatingLoadCombPSCRCMainGrid, CRatingLoadCombCommonMainGrid)
	//{{AFX_MSG_MAP(CRatingLoadCombPSCRCMainGrid)

	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CRatingLoadCombPSCRCMainGrid::InsertRecordByKey(const T_RKLC_K &key, const T_RKLC_D &data)
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

BOOL CRatingLoadCombPSCRCMainGrid::DeleteRecordByKey(const T_RKLC_K &key, const T_RKLC_D &data)
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
BOOL CRatingLoadCombPSCRCMainGrid::ValidateAndMakeRecord(CStringArray &value, CRowColArray& aCols, T_RKLC_K &key, T_RKLC_D &data)
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

BOOL CRatingLoadCombPSCRCMainGrid::AddToDB(ROWCOL nRow)
{
	// Record를 만들어서 DB에 입력한다.
	T_RKLC_K key;
	T_RKLC_D data;
	data.Initialize();
 
	GetValue(nRow, key, data);
	
	return m_pDoc->m_pDataCtrl->AddRklc(key, data);
}

BOOL CRatingLoadCombPSCRCMainGrid::AddToDB(CArray<void*, void*>& raKey, CArray<void*, void*>& raData)
{
	ASSERT(0);

	ASSERT(raKey.GetSize() == raData.GetSize());
	T_RKLC_K tKey;
	CArray<T_RKLC_K, T_RKLC_K> aKey;
	CArray<T_RKLC_D, T_RKLC_D&> aData;
	for (int i = 0; i < raKey.GetSize(); i++)
	{
		tKey = *((T_RKLC_K*)raKey.GetAt(i));
		aKey.Add(tKey);
		aData.Add(*((T_RKLC_D*)raData.GetAt(i)));
	}
	return m_pDoc->m_pDataCtrl->AddRklc(aKey, aData);
}


BOOL CRatingLoadCombPSCRCMainGrid::DeleteFromDB(ROWCOL nRow)
{
	T_RKLC_K key;
	T_RKLC_D data;
	data.Initialize();
 
	GetValue(nRow, key, data);
	return m_pDoc->m_pDataCtrl->DelRklc(key);
}

BOOL CRatingLoadCombPSCRCMainGrid::DeleteFromDB(CRowColArray& awRows)
{
	if (awRows.GetSize() <= 0) return TRUE;

	ROWCOL nCol;
	nCol = 1;

	CString value;
	CArray<T_RKLC_K, T_RKLC_K> aKey;

	for (int i = 0; i < awRows.GetSize(); i++)
	{
		value = GetValueRowColBeforeEdit(awRows.GetAt(i), nCol);
		UINT nKey = (UINT)_ttoi(value);
		aKey.Add(nKey);
	}

	return m_pDoc->m_pDataCtrl->DelRklc(aKey);
}

BOOL CRatingLoadCombPSCRCMainGrid::ModifyToDB(ROWCOL nRow)
{
	ROWCOL nCol;
	nCol = 1;

	T_RKLC_K key;
	T_RKLC_D data;
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
	return m_pDoc->m_pDataCtrl->ModifyRklc(key, key, data); 
}

BOOL CRatingLoadCombPSCRCMainGrid::IsDataChange(T_RKLC_K key, T_RKLC_D& dataNew)
{
	T_RKLC_D dataOld;
	m_pDoc->m_pAttrCtrl->GetRklc(key, dataOld);
	if(dataOld == dataNew)
	{
		return FALSE;
	}
	else
	{
		return TRUE;
	}
}

BOOL CRatingLoadCombPSCRCMainGrid::ModifyToDB(CStringArray& raOldKey, 
												 CArray<void*, void*>& raKey, 
												 CArray<void*, void*>& raData)
{
	ASSERT(raKey.GetSize() == raData.GetSize());
	ASSERT(raKey.GetSize() == raOldKey.GetSize());  
	CArray<T_RKLC_D, T_RKLC_D&> aData;
	CArray<T_RKLC_K, T_RKLC_K> aKey;
	T_RKLC_K Key;
	T_RKLC_D Data;
	Data.Initialize();
	for (int i = 0; i < raKey.GetSize(); i++)
	{
		Key = _ttoi(raOldKey.GetAt(i));
		aKey.Add(Key);
		m_pDoc->m_pAttrCtrl->GetRklc(Key, Data);
		aData.Add(*((T_RKLC_D*)raData.GetAt(i)));
	}

	return m_pDoc->m_pDataCtrl->ModifyRklc(aKey, aKey, aData);
}

BOOL CRatingLoadCombPSCRCMainGrid::MakeKeyAndDataArray(CStringArray& value, CRowColArray& awCols, 
																	CArray<void*, void*>& aKey, CArray<void*, void*>& aData)
{
	T_RKLC_K* pKey;
	T_RKLC_D* pData;
	pKey = new T_RKLC_K;
	pData = new T_RKLC_D;
	pData->Initialize();
	aKey.Add((void*)pKey);
	aData.Add((void*)pData);
	if (!ValidateAndMakeRecord(value, awCols, *pKey, *pData)) return FALSE;
	return TRUE;
}

void CRatingLoadCombPSCRCMainGrid::DeleteKeyAndDataArray(CArray<void*, void*>& aKey, CArray<void*, void*>& aData)
{
	T_RKLC_K* pKey;
	T_RKLC_D* pData;
	ASSERT(aKey.GetSize() == aData.GetSize());
	for (int i = 0; i < aKey.GetSize(); i++)
	{
		pKey = (T_RKLC_K*)aKey.GetAt(i);
		pData = (T_RKLC_D*)aData.GetAt(i);
		delete pKey;
		delete pData;
	}
}

long CRatingLoadCombPSCRCMainGrid::GetCountData()
{
	CDBDoc* pDoc = m_pDoc;
	T_RKLC_D tData;
	tData.Initialize();
	return pDoc->m_pAttrCtrl->GetCountRklc();
}

void CRatingLoadCombPSCRCMainGrid::MakeItemNormal(long nNumData, CRowColArray& awCols)
{
	T_RKLC_K Key;
	T_RKLC_D Data;
	Data.Initialize();
	CDBDoc* pDoc = m_pDoc;

	long* KeyBuf = new long[nNumData];
	long nCount = 0;
	POSITION pos = pDoc->m_pAttrCtrl->GetStartRklc();
	while(pos != NULL)
	{
	  pDoc->m_pAttrCtrl->GetNextRklc(pos,Key,Data);
	  KeyBuf[nCount++] = Key;
	}
	qsort(KeyBuf, nNumData, sizeof(T_LCOM_K), CTBBrowserWnd::CompareKey);

	for(nCount=0 ; nCount < nNumData; nCount++)
	{
		pDoc->m_pAttrCtrl->GetRklc(KeyBuf[nCount], Data);
		SetValue(GetParam()->GetData(), nCount+1, KeyBuf[nCount], Data, awCols);
	} // end of loop nCount

	delete []KeyBuf;
}

void CRatingLoadCombPSCRCMainGrid::MakeItemEditingRecord(ROWCOL ncRow, CStringArray& EditingRecord)
{
	CDBDoc* pDoc = m_pDoc;
	T_RKLC_K Key;
	T_RKLC_D Data;
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
// CRatingLoadCombPSCRCMainGrid Implementation
BOOL CRatingLoadCombPSCRCMainGrid::GetValue(ROWCOL nRow, T_RKLC_K &key, T_RKLC_D &data)
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
	((CRatingLoadCombPSCRCSubGrid*)m_pSubGrid)->GetData(data);
	((CRatingLoadCombPSCRCSubGrid*)m_pSubGridMV)->GetData(data);
	
	return TRUE;
}

BOOL CRatingLoadCombPSCRCMainGrid::SetValue(ROWCOL nRow, const T_RKLC_K &key, const T_RKLC_D &data)
{
	//
	CRowColArray awCols;
	awCols.SetSize(GetColCount());
	for (int i = 0;i < GetColCount(); i++)
		VERIFY(GetColumnNoFor(m_aTitle[i], awCols[i]));
	return SetValue(GetParam()->GetData(), nRow, key, data, awCols);
}

BOOL CRatingLoadCombPSCRCMainGrid::SetValue(CGXData* pData, ROWCOL nRow, const T_RKLC_K &key, const T_RKLC_D &data, CRowColArray &awCols)
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

void CRatingLoadCombPSCRCMainGrid::ConvStrToData(int i, CString& value, T_RKLC_D& data)
{
	//if (i==1) data.LoadCombId = _ttol(value);  // Serial No
	if (i==2) data.strCaseName = value; // name
	//else if (i==3) data.LoadCombType = GetCombType(value); // type
	else if (i==4) data.strDescription = value; // description
//  else if (i==5) data.nActive = GetActive(m_nLcomType, value);
//  else if (i==6) data.bElasticStage = (value==_T("1")); // MNET:1209, KJH20050203
}

void CRatingLoadCombPSCRCMainGrid::ConvDataToStr(int i, const T_RKLC_D& data, CString& value)
{
	//if (i==1) value.Format(_T("%d"), data.LoadCombId);
	if (i==2) value = data.strCaseName;
//  else if (i==3) value = GetCombTypeStr(data.LoadCombType);
	else if (i==4) value = data.strDescription;
//  else if (i==5) value = GetActiveName(m_nLcomType, data.nActive);
//  else if (i==6) value = data.bElasticStage ? _T("1") : _T("0"); // MNET:1209, KJH20050203
}

// Main Grid, Sub Grid간의 동기화  
void CRatingLoadCombPSCRCMainGrid::OnChangeCurrentRecord(ROWCOL nRow)
{
	ROWCOL nkCol;
	GetColumnNoFor(GetKeyColID(), nkCol);
	CString sKey = GetKeyForRow(nRow, nkCol);
	m_pSubGrid->SetData(sKey);
	m_pSubGridMV->SetData(sKey);

	T_RKLC_K key;
	T_RKLC_D data;	
	data.Initialize();
	key = _ttol(sKey);  
	m_pDoc->m_pAttrCtrl->GetRklc(key, data);
	CString szTemp;
	szTemp.Format(_T("%g"), data.dImpFactor);

	m_bIgnoreSetEditMode = TRUE;
	//m_pEdtImpactFactor->SetWindowText(szTemp);
	m_bIgnoreSetEditMode = FALSE;
}


// LoadComb의 위치를 이동(Serial No가 변경)한다.
BOOL CRatingLoadCombPSCRCMainGrid::OnSelDragRowsDrop(ROWCOL nFirstRow, ROWCOL nLastRow, ROWCOL nDestRow)
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
	CArray<T_RKLC_D, T_RKLC_D&> aData;
	CArray<T_RKLC_K, T_RKLC_K> aKey;
	CString LoadCombName;
	T_RKLC_K key;
	T_RKLC_D data;
	data.Initialize();
	for (int i = nFirstRow; i <= nLastRow; i++)
	{
		GetValue(i, key, data);
		// Get Value만 하면 Combination이 현재 Record의 Combination으로 치환된다.
		// 여기서는 위치만 이동되므로 LoadCombId만 변경된다. 그래서 원래의 Combination
		// 을 유지하기 위해 GetLcom을 다시 호출한다.
		m_pDoc->m_pAttrCtrl->GetRklc(key, data);
		//data.LoadCombId = nStart;
		nStart += nInc;
		
		aKey.Add(key);
		aData.Add(data);
	}

	m_pDoc->m_pDataCtrl->ModifyRklc(aKey, aKey, aData);
	return FALSE;
}

CString CRatingLoadCombPSCRCMainGrid::GetNewKey()
{
	int i;

	CArray<T_RKLC_K, T_RKLC_K> aKeyList;  
	m_pDoc->m_pAttrCtrl->GetRklcKeyList(aKeyList);
	
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
	
	T_RKLC_K KeyTemp = 1;
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