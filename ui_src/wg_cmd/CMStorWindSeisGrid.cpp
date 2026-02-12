// CMStorWindSeisGrid.cpp : implementation file
//
// 2005.5.12 층 순서를 고층부터 Display 하도록 변경(jbseon)
#include "stdafx.h"
#include "wg_cmd.h"
#include "CMStorWindSeisGrid.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "..\wg_base\wg_base_CompFunc.h"
#include "..\wg_db\wg_db_ViewBuff.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_ViewCtrl.h"

#define STOR_KEY_FMT    _T("%d")
#define STOR_DATA_FMT   _T("%.15g")

/////////////////////////////////////////////////////////////////////////////
// CCMStorWindSeisGrid
CString CCMStorWindSeisGrid::m_aCurDefVal[] = {_T(""), _T("0"),  
	_T("0"), _T("0"), _T("0"), _T("0"), _T("0"), _T("0"), _T("0"), _T("0"), _T("0.0"), _T("0.0"), _T("1.0"), _T("1.0"), _T("0")};

CCMStorWindSeisGrid::CCMStorWindSeisGrid()
{
	m_bBeforeSaved = FALSE;

	T_STOR_UNIT * pUnit = &(CUnitCtrl::m_STOR_UNIT);

	AddColInfo(_T("ID"),                     t_integer, 0);
	AddColInfo(_LS(IDS_WG_CMD__ADDD__Name),                   t_string,  pUnit->StoryName);
	AddColInfo(_LS(IDS_WG_CMD__ADDD__Level),                  t_real,    pUnit->dStoryLevel);
	AddColInfo(_LS(IDS_WG_CMD__ADDD__Floor_Width_nX_Dir),     t_real,    pUnit->WindWidthX);
	AddColInfo(_LS(IDS_WG_CMD__ADDD__Floor_Width_nY_Dir),     t_real,    pUnit->WindWidthY);
	AddColInfo(_LS(IDS_WG_CMD__ADDD__Floor_Center_nXc),       t_real,    pUnit->WindCenterX);
	AddColInfo(_LS(IDS_WG_CMD__ADDD__Floor_Center_nYc),       t_real,    pUnit->WindCenterY);
	AddColInfo(_LS(IDS_WG_CMD__ADDD__Eccentricity_nX_Dir),    t_real,    pUnit->WindEccentX);
	AddColInfo(_LS(IDS_WG_CMD__ADDD__Eccentricity_nY_Dir),    t_real,    pUnit->WindEccentY);
	AddColInfo(_LS(IDS_WG_CMD__STORY_ACC_ECC_X),    t_real,    pUnit->SeisEccentX);
	AddColInfo(_LS(IDS_WG_CMD__STORY_ACC_ECC_Y),    t_real,    pUnit->SeisEccentY);
	AddColInfo(_LS(IDS_WG_CMD__STORY_INH_ECC_X),    t_real,    pUnit->dInherentEccenX);
	AddColInfo(_LS(IDS_WG_CMD__STORY_INH_ECC_Y),    t_real,    pUnit->dInherentEccenY);
	AddColInfo(_LS(IDS_WG_CMD__STORY_TOR_AMP_X),    t_real,    pUnit->dTorsionalAmpFactorX);
	AddColInfo(_LS(IDS_WG_CMD__STORY_TOR_AMP_Y),    t_real,    pUnit->dTorsionalAmpFactorX);
	AddColInfo(_T("Module index"),                                t_integer,  D_UNITSYS_NONE);

	m_aPrimaryKey.Add(0);
}

CCMStorWindSeisGrid::~CCMStorWindSeisGrid()
{
}


BEGIN_MESSAGE_MAP(CCMStorWindSeisGrid, CTBCommon)
	//{{AFX_MSG_MAP(CCMStorWindSeisGrid)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CCMStorWindSeisGrid message handlers

void CCMStorWindSeisGrid::Initialize(CDBDoc* pDoc)
{
	T_UNIT_SYSTEM UnitSystem;
	CTBCommon::Initialize(pDoc, UnitSystem);

	LockUpdate(TRUE);

	// 표준 스타일 변경
	ChangeStandardStyle(CGXStyle()
			.SetFont(CGXFont_GC().SetSize(10))
			.SetVerticalAlignment(DT_BOTTOM)
			.SetAllowEnter(FALSE));

	// Column header의 스타일 변경
	ChangeColHeaderStyle(CGXStyle().SetAllowEnter(TRUE));
	SetRowHeightByDPI(0, 0, 36);

	// 컬럼 스타일 변경
	SetStyleRange(CGXRange().SetCols(1, GetInitialColCount()), CGXStyle()
			.SetHorizontalAlignment(DT_RIGHT)
			.SetValueType(GX_VT_NUMERIC));

	// Key column의 스타일 변경
	SetStyleRange(CGXRange().SetCols(1), CGXStyle()
			.SetPlaces(0)
			.SetEnabled(FALSE));
	SetFrozenCols(2, 0);
	HideColsByIntend(1, 1, TRUE);

	// Name Column의 스타일 변경
	SetStyleRange(CGXRange().SetCols(2), CGXStyle()
			.SetValueType(GX_VT_STRING)
			.SetHorizontalAlignment(DT_LEFT));

	// real type column의 스타일 변경
	int anCol[] = {4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
	for (int i = 0; i < sizeof(anCol)/sizeof(int); i++)
	{
		SetStyleRange(CGXRange().SetCols(anCol[i]), CGXStyle()
			.SetValueType(GX_VT_NUMERIC)
			.SetFormat(GX_FMT_FIXED)
			.SetPlaces(2));
	}

	// MQC 12651 : Module index 열 폭이 0이긴 한데 유저가 따블클릭해서 이거 뭐냐고 물어봄.. 숨김
	HideColsByIntend(16, 16, TRUE);
	 
	SetColumnAttr();

#ifdef _MVIL
	SetModeDisplayOnly(TRUE);
#endif

	SetRightValidCell(1, 1);

	// Story가 아니면 Append, Insert를 금지시킨다.(수정/Delete만 가능)
	EnableInsertRowByPressingKey(FALSE);
	EnableDeleteRowByPressingKey(FALSE);
	EnableAppendRow(FALSE);

	// Sorting : Level로 Sorting한다. : 이 순서는 안 바뀌는 것으로 가정한다.
	// 왜냐하면 Height는 계산되어 설정되는 값이고 성능상의 이유로 Level별로
	// Sorting되어 있다고 가정하고 프로그래밍 되어 있다.

	CGXSortInfo SortInfo;
	m_aSortInfo.RemoveAll();
	// by Module index
	SortInfo.nRC       = 15;
	SortInfo.sortType  = CGXSortInfo::numeric;
	SortInfo.sortOrder = CGXSortInfo::descending;
	m_aSortInfo.Add(SortInfo);
	// by Level
	SortInfo.nRC       = 2;
	SortInfo.sortType  = CGXSortInfo::numeric;
	SortInfo.sortOrder = CGXSortInfo::descending;
	m_aSortInfo.Add(SortInfo);
	// by key
	SortInfo.nRC       = 0;
	SortInfo.sortType  = CGXSortInfo::numeric;
	SortInfo.sortOrder = CGXSortInfo::ascending;
	m_aSortInfo.Add(SortInfo);

	MakeItemEx();

	this->EnableInsertRowByPressingKey(FALSE);  // Insert Key에 의한 Record 삽입 금지
	LockUpdate(FALSE);
}

// 수정 없음
BOOL CCMStorWindSeisGrid::InsertRecordByKey(const T_STOR_K &key, const T_STOR_D &data)
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

// 수정 없음
BOOL CCMStorWindSeisGrid::DeleteRecordByKey(const T_STOR_K &key, const T_STOR_D &data)
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

// 수정 없음
void CCMStorWindSeisGrid::UnitChanged(CDBDoc* pDoc)
{
	BOOL bOldLock = LockUpdate(TRUE);

	T_UNIT_SYSTEM UnitSystem;
 	//pDoc->m_pUnitCtrl->GetUnitSystemStor(UnitSystem);

	// 타이틀을 수정
	SetHeaderTitle(UnitSystem);
	// 각 항목의 값을 수정
	MakeItemEx(0, -2, TRUE);
	LockUpdate(bOldLock);
	if (!bOldLock) Redraw();
}

// IsExist???? 수정
void CCMStorWindSeisGrid::UpdateBuffer(CDBDoc* pDoc)
{
	int nModified = 0;

	CViewBuff* pViewBuff = pDoc->m_pViewBuff;
	int nCount = pViewBuff->GetCount();
	if (nCount == 0) return; 
	ASSERT(nCount == 1);

	T_UDRD_BUFFER buffer_ur;
	POSITION pos = pViewBuff->GetStartBuffer();

	BOOL bOldLock = LockUpdate(TRUE);

	T_STOR_K key, keyBak; 
	T_STOR_D data, dataBak;
	BOOL bMFD = FALSE;
	BOOL bResetGrid = FALSE;
	while(pos != NULL)
	{
		buffer_ur = pViewBuff->GetNextBuffer(pos);

		int nCmd = buffer_ur.nCmd;
		int nKey = buffer_ur.nKey;
		
		switch(nCmd)
		{
		case(UR_STOR_ADD):
//         // 삽입될 위치(nRow)를 찾아 넣는다.
//         pViewBuff->GetStor(nKey, key, data);
//         InsertRecordByKey(key, data);
				bResetGrid = TRUE;
				nModified |= 0x01;
				break;
		case(UR_STOR_DEL):
//         // 삭제될 위치(nRow)를 찾아서 지운다.
//         pViewBuff->GetStor(nKey, key, data);
//         DeleteRecordByKey(key, data);
				bResetGrid = TRUE;
				nModified |= 0x02;
				break;
		case(UR_STOR_MFD):
				// 여기서는 저장만 하고 UR_STOR_MFS에서 처리한다.
				pViewBuff->GetStor(nKey, keyBak, dataBak);
				nModified |= 0x01;
				bMFD = TRUE;
				break;
		case(UR_STOR_MFS):
				// MFD가 먼저오고 MFS가 온다. 항상 이 순서대로 한 짝이 되어 온다.
				ASSERT(bMFD);
				pViewBuff->GetStor(nKey, key, data);
				dataBak.StoryName = m_pDoc->m_pAttrCtrl->GetStorNameWithModuleName(key);
				nModified |= 0x02;
				DeleteRecordByKey(key, data);
				InsertRecordByKey(keyBak, dataBak);
				// 플래그 수정
				bMFD = FALSE;
				break;
		default:
			break;
		}
	} // end of while

	if (bResetGrid) MakeItemEx();    // 층이 추가/삭제되면 테이블을 리셋한다.

	if (nModified) ShowSelected();
	AdjustAppendRowNo();
	LockUpdate(bOldLock);
	if (!bOldLock) Redraw();
}

/////////////////////////////////////////////////////////////////////////////
// CCMStorWindSeisGrid Overridable Functions
// 수정 없음
void CCMStorWindSeisGrid::OnChangedSelectionPublic(long *keys, int nSize)
{
	if (nSize <= 0)
	{
		m_aSelectedKeys.RemoveAll();
	}
	else
	{
		m_aSelectedKeys.RemoveAll();
		for (int i = 0; i < nSize; i++) m_aSelectedKeys.Add((T_STOR_K)keys[i]);
	}
}

// 수정 없음
long CCMStorWindSeisGrid::GetSelectedKeys(CArray<long, long> &caKey)
{
	// override this function to return selected record count
	long nKeyCount;
	CArray<T_STOR_K, T_STOR_K> aKey;

	GetAllSelectedStor(aKey);
	nKeyCount = aKey.GetSize();
	caKey.SetSize(nKeyCount);
	for (long i = 0; i < nKeyCount; i++)
		caKey[i] = (long)aKey[i];
	
	return nKeyCount;
}

// 데이타 타입에 맞게 수정
BOOL CCMStorWindSeisGrid::ValidateField(CString value, int nColID)
{
	if (nColID >= GetColCount())
	{
		SetWarningText(_LS(IDS_WG_CMD__ADDD___CCMStorWindSeisGrid__ValidateField__Erro));
		return FALSE;
	}

	int nErrCode;

	if (nColID == 0)  // ID : error check 없음
	{
		return TRUE;  
	}
	else if (nColID == 1)  // Name
	{
		nErrCode; // Do Nothing...
	}
	else if (nColID == 15)  // Module Index
	{
		nErrCode; // Do Nothing...
	}
	else
	{
		if (!IsValidDoubleNumber(value, nErrCode))
		{
			switch(nErrCode)
			{
			case 1: SetWarningText(_LS(IDS_WG_CMD__ADDD__Invalid_formula_)); break;
			case 2: SetWarningText(_LS(IDS_WG_CMD__ADDD__Character_exists_in_double_value)); break;
			case 3: SetWarningText(_LS(IDS_WG_CMD__ADDD__Value_is_too_big_)); break;
			}
			return FALSE;
		}
	}
	return TRUE;
}

// 수정 없음
BOOL CCMStorWindSeisGrid::ValidateRecord(CStringArray &value, CRowColArray &aCols)
{
	CString msg = _T("");
	int nPos;

	// ID 필드는 제외
	CArray<int, int> aCheck;
	switch(m_nViewMode)
	{
	case D_STOR_VIEWMODE_WIND:
				aCheck.Add(1); 
				aCheck.Add(3); aCheck.Add(4); aCheck.Add(5); aCheck.Add(6); aCheck.Add(7); aCheck.Add(8); 
				break;
	case D_STOR_VIEWMODE_SEISMIC:
				aCheck.Add(1);
				aCheck.Add( 9); aCheck.Add(10);
				aCheck.Add(11); aCheck.Add(12);
				aCheck.Add(13); aCheck.Add(14);
				break;
	}

	int nSize = aCheck.GetSize();
	int i;
	for (int c = 0; c < nSize; c++) 
	{
		i = aCheck[c];
		nPos = aCols[i]-1;
		
		if (value[nPos] == _T(""))
			msg += ((msg != _T("")) ? _T("\n") : _T("")) + m_aTitle[i] + _LS(IDS_WG_CMD__ADDD___has_not_been_entered_);
		else if (!ValidateField(value[nPos], i)) return FALSE;
	}
	if (msg == _T("")) return TRUE;
	else SetWarningText(msg);
	return FALSE;
}

// 2Part Key 아니면 수정 없음
BOOL CCMStorWindSeisGrid::ValidateAndMakeRecord(CStringArray &value, CRowColArray& aCols, T_STOR_K &key, T_STOR_D &data)
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
		if(i==0) 
		{
			if (value[nPos] == _T("")) key = 0;
			else key = _tstol(value[nPos]);

			if (!m_pDoc->m_pAttrCtrl->GetStor(key, data))  data.Initialize();		// 2014-05-27. by Hsshim. Story Grid를 둘로 나눴기 때문에 여기서 초기화 시킴.
		}
		else ConvStrToData(i, value[nPos], data);
	}
	return TRUE;
}

// Add???? 수정
BOOL CCMStorWindSeisGrid::AddToDB(ROWCOL nRow)
{
	if (!CanSaveData()) return FALSE;

	// Record를 만들어서 DB에 입력한다.
	T_STOR_K key;
	T_STOR_D data;

	GetValue(nRow, key, data);
	return m_pDoc->m_pDataCtrl->AddStor(data);  
}

// Multiple data paste 지원시 Add???? 수정
BOOL CCMStorWindSeisGrid::AddToDB(CArray<void*, void*>& raKey, CArray<void*, void*>& raData)
{
	ASSERT(raKey.GetSize() == raData.GetSize());
	T_STOR_K tKey;
	CArray<T_STOR_K, T_STOR_K> aKey;
	CArray<T_STOR_D, T_STOR_D&> aData;
	for (int i = 0; i < raKey.GetSize(); i++)
	{
		tKey = *((T_STOR_K*)raKey.GetAt(i));
		aKey.Add(tKey);
		aData.Add(*((T_STOR_D*)raData.GetAt(i)));
	}
	return m_pDoc->m_pDataCtrl->AddStor(aData);
}

// 수정 없음
BOOL CCMStorWindSeisGrid::DeleteFromDB(ROWCOL nRow)
{
	T_STOR_K key;
	T_STOR_D data;
 
	GetValue(nRow, key, data);
	return m_pDoc->m_pDataCtrl->DelStor(data.StoryName);
}

// 2Part Key 아니면 수정 없음
BOOL CCMStorWindSeisGrid::DeleteFromDB(CRowColArray& awRows)
{
	if (awRows.GetSize() <= 0) return TRUE;

	// Name 칼럼을 얻는다.
	ROWCOL nCol;
	VERIFY(GetColumnNoFor(1, nCol));

	CString value;
	CArray<CString, CString&> awKeys;
 
	for (int i = 0; i < awRows.GetSize(); i++)
	{
		value = GetValueRowColBeforeEdit(awRows.GetAt(i), nCol);
		awKeys.Add(value);
	}

	return m_pDoc->m_pDataCtrl->DelStor(awKeys);
}

// 2 Part Key 아니면 수정 없음
BOOL CCMStorWindSeisGrid::ModifyToDB(ROWCOL nRow)
{
	if (!CanSaveData()) return FALSE;

	// Name 칼럼을 얻는다.
	ROWCOL nCol;
	VERIFY(GetColumnNoFor(1, nCol));

	CString csOldName;
	T_STOR_K key;
	T_STOR_D data;

	csOldName = GetValueRowColBeforeEdit(nRow, nCol);

	GetValue(nRow, key, data);

	// 수정한다. 실패하면 에러 리턴
	return m_pDoc->m_pDataCtrl->ModifyStor(csOldName, data);
}

// 수정 없음
BOOL CCMStorWindSeisGrid::ModifyToDB(CStringArray& raOldKey, 
												 CArray<void*, void*>& raKey, 
												 CArray<void*, void*>& raData)
{
	ASSERT(raKey.GetSize() == raData.GetSize());
	ASSERT(raKey.GetSize() == raOldKey.GetSize());
	CArray<CString, CString&> aOldName;
	T_STOR_K key;
	T_STOR_D data;
	CArray<T_STOR_D, T_STOR_D&> aData;
	for (int i = 0; i < raKey.GetSize(); i++)
	{
		key = _tstol(raOldKey.GetAt(i));
		m_pDoc->m_pAttrCtrl->GetStor(key, data);
		aOldName.Add(data.StoryName);
		aData.Add(*((T_STOR_D*)raData.GetAt(i)));
	}
	return m_pDoc->m_pDataCtrl->ModifyStor(aOldName, aData);
}

// 수정 없음
CString CCMStorWindSeisGrid::GetNewKey()
{
	CString value;
	//T_STOR_K key = m_pDoc->m_pAttrCtrl->GetStartNumStor();
	//value.Format(STOR_KEY_FMT, key);
	value = _T("");
	return value;
}

// 수정 없음
CString CCMStorWindSeisGrid::GetDefValue(int nIndex)
{
	return m_aCurDefVal[nIndex];
}

// 수정 없음
CString* CCMStorWindSeisGrid::GetDefValueArray()
{
	return m_aCurDefVal;
}

// 수정 없음
BOOL CCMStorWindSeisGrid::MakeKeyAndDataArray(CStringArray& value, CRowColArray& awCols, 
																	CArray<void*, void*>& aKey, CArray<void*, void*>& aData)
{
	T_STOR_K* pKey;
	T_STOR_D* pData;
	pKey = new T_STOR_K;
	pData = new T_STOR_D;
	aKey.Add((void*)pKey);
	aData.Add((void*)pData);
	if (!ValidateAndMakeRecord(value, awCols, *pKey, *pData)) return FALSE;
	return TRUE;
}

// 수정 없음
void CCMStorWindSeisGrid::DeleteKeyAndDataArray(CArray<void*, void*>& aKey, CArray<void*, void*>& aData)
{
	T_STOR_K* pKey;
	T_STOR_D* pData;
	ASSERT(aKey.GetSize() == aData.GetSize());
	for (int i = 0; i < aKey.GetSize(); i++)
	{
		pKey = (T_STOR_K*)aKey.GetAt(i);
		pData = (T_STOR_D*)aData.GetAt(i);
		delete pKey;
		delete pData;
	}
}

// 수정 없음
long CCMStorWindSeisGrid::GetCountData()
{
	CDBDoc* pDoc = m_pDoc;
	return pDoc->m_pAttrCtrl->GetCountStor();
}

// 수정 없음
void CCMStorWindSeisGrid::MakeItemFiltered(long nFilteredNum, long* aFilteredKeys, CRowColArray& awCols)
{
	CDBDoc* pDoc = m_pDoc;
	T_STOR_D Data;

	for(long nCount=0 ; nCount < nFilteredNum; nCount++)
 	{
		pDoc->m_pAttrCtrl->GetStor(aFilteredKeys[nCount], Data);
		SetValue(GetParam()->GetData(), nCount+1, aFilteredKeys[nCount], Data, awCols);
	}
}

// 수정 없음
void CCMStorWindSeisGrid::MakeItemNormal(long nNumData, CRowColArray& awCols)
{
	T_STOR_K Key;
	T_STOR_D Data;
	CDBDoc* pDoc = m_pDoc;

	long* KeyBuf = new long[nNumData];
	long nCount = 0;
	POSITION pos = pDoc->m_pAttrCtrl->GetStartStor();
	while(pos != NULL)
	{
	  pDoc->m_pAttrCtrl->GetNextStor(pos,Key,Data);
	  KeyBuf[nCount++] = Key;
	}
	qsort(KeyBuf, nNumData, sizeof(T_STOR_K), CCompFunc::UINTAsc);

	for(nCount=0 ; nCount < nNumData; nCount++)
	{
		pDoc->m_pAttrCtrl->GetStor(KeyBuf[nCount], Data);
		SetValue(GetParam()->GetData(), nCount+1, KeyBuf[nCount], Data, awCols);
	} // end of loop nCount

	delete []KeyBuf;
}

// 2 Part Key 외 수정 없음
void CCMStorWindSeisGrid::MakeItemEditingRecord(ROWCOL ncRow, CStringArray& EditingRecord)
{
	CDBDoc* pDoc = m_pDoc;
	T_STOR_K Key;
	T_STOR_D Data;
	for (int i = 0; i < GetColCount(); i++)
	{
		if (i == 0) Key = _tstol(EditingRecord[i]);
		else ConvStrToData(i, EditingRecord[i], Data);
	}
	pDoc->m_pUnitCtrl->ConvertUnitStorPrevious(Data);
	// 현재 값을 설정한다.
	SetValue(ncRow, Key, Data);
}

/////////////////////////////////////////////////////////////////////////////
// CCMStorWindSeisGrid Implementation
// 2 Part Key 외 수정 없음
BOOL CCMStorWindSeisGrid::GetValue(ROWCOL nRow, T_STOR_K &key, T_STOR_D &data)
{
	if (nRow < 1) return FALSE;
	if (IsAppendRow(nRow) || nRow > GetRowCount()) return FALSE;
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
			key = _tstol(str);

			if(!m_pDoc->m_pAttrCtrl->GetStor(key, data)) data.Initialize();
		}
		else ConvStrToData(i, str, data);
	}
	return TRUE;
}

// 수정 없음
BOOL CCMStorWindSeisGrid::SetValue(ROWCOL nRow, const T_STOR_K &key, const T_STOR_D &data)
{
	//
	CRowColArray awCols;
	awCols.SetSize(GetColCount());
	for (int i = 0; i < GetColCount(); i++)
		VERIFY(GetColumnNoFor(m_aTitle[i], awCols[i]));
	return SetValue(GetParam()->GetData(), nRow, key, data, awCols);
}

// 2Part Key, Column Enable/Disable이 있으면 수정
BOOL CCMStorWindSeisGrid::SetValue(CGXData* pData, ROWCOL nRow, const T_STOR_K &key, const T_STOR_D &data, CRowColArray &awCols)
{
	//
	CString str;
	int nValueType;
	for (int i = 0; i < GetColCount(); i++)
	{
		if(i==0) 
		{
			if (key == 0) str.Empty();
			else str.Format(STOR_KEY_FMT,key);
		}
		else ConvDataToStr(i, data, str, key);
		if (m_aType[i] == t_string) nValueType = GX_VT_STRING;
		else nValueType = GX_VT_NUMERIC;
		pData->StoreValueRowCol(nRow, awCols[i], str, gxOverride, nValueType);
	}

	return TRUE;
}

// 데이타 특성에 따라 수정
void CCMStorWindSeisGrid::ConvStrToData(int i, CString& value, T_STOR_D& data)
{
	if (i == 1) data.StoryName = value;
	else if (i ==  2) data.dStoryLevel = _tstof(value);
	else if (i ==  3)  data.WindWidthX = _tstof(value);
	else if (i ==  4)  data.WindWidthY = _tstof(value);
	else if (i ==  5)  data.WindCenterX = _tstof(value);
	else if (i ==  6)  data.WindCenterY = _tstof(value);
	else if (i ==  7)  data.WindEccentX = _tstof(value); 
	else if (i ==  8) data.WindEccentY = _tstof(value); 
	else if (i ==  9) data.SeisEccentX = _tstof(value); 
	else if (i == 10) data.SeisEccentY = _tstof(value);   
	else if (i == 11) data.dInherentEccenX = _tstof(value);
	else if (i == 12) data.dInherentEccenY = _tstof(value);
	else if (i == 13) data.dTorsionalAmpFactorX = _tstof(value);
	else if (i == 14) data.dTorsionalAmpFactorY = _tstof(value);
	else if (i == 15) i;    // do nothing...
}

// 데이타 특성에 따라 수정
void CCMStorWindSeisGrid::ConvDataToStr(int i, const T_STOR_D& data, CString& value, const T_STOR_K StorK)
{
	if (i == 1) value = data.StoryName;
	else if (i ==  2) value.Format(STOR_DATA_FMT, data.dStoryLevel);
	else if (i ==  3)  value.Format(STOR_DATA_FMT, data.WindWidthX);
	else if (i ==  4)  value.Format(STOR_DATA_FMT, data.WindWidthY);
	else if (i ==  5)  value.Format(STOR_DATA_FMT, data.WindCenterX);
	else if (i ==  6)  value.Format(STOR_DATA_FMT, data.WindCenterY);
	else if (i ==  7)  value.Format(STOR_DATA_FMT, data.WindEccentX); 
	else if (i ==  8) value.Format(STOR_DATA_FMT, data.WindEccentY);
	else if (i ==  9) value.Format(STOR_DATA_FMT, data.SeisEccentX);
	else if (i == 10) value.Format(STOR_DATA_FMT, data.SeisEccentY);
	else if (i == 11) value.Format(STOR_DATA_FMT, data.dInherentEccenX);
	else if (i == 12) value.Format(STOR_DATA_FMT, data.dInherentEccenY);
	else if (i == 13) value.Format(STOR_DATA_FMT, data.dTorsionalAmpFactorX);
	else if (i == 14) value.Format(STOR_DATA_FMT, data.dTorsionalAmpFactorY);
	else if (i == 15) 
	{
		CString strModuleName;
		int iModl = m_pDoc->m_pAttrCtrl->GetModuleIndexOfStor(StorK, strModuleName);
		value.Format(_T("%d"), iModl);
	}
	else value = _LS(IDS_WG_CMD__ADDD__Error);
}

// 2Part Key 외 수정 없음
void CCMStorWindSeisGrid::MakeSearchKey(const T_STOR_K &key, const T_STOR_D &data, CStringArray &aKey)
{
	int nColID;
	int nSortKeyNum = m_aSortInfo.GetSize();

	aKey.SetSize(nSortKeyNum);
	for (int i = 0; i < nSortKeyNum; i++)
	{
		nColID = m_aSortInfo[i].nRC;
		if (nColID == 0) aKey[i].Format(STOR_KEY_FMT, key);
		else ConvDataToStr(nColID, data, aKey[i], key);
	}
}

// 수정 없음
void CCMStorWindSeisGrid::GetAllSelectedStor(CArray<T_STOR_K, T_STOR_K>& aKey)
{
	for (int i = 0; i < m_aSelectedKeys.GetSize(); i++)
		aKey.Add(m_aSelectedKeys[i]);
}

// 데이타 특성에 따라 수정
void CCMStorWindSeisGrid::SetHeaderTitle(T_UNIT_SYSTEM &UnitSystem, BOOL bInit)
{
	SetHeaderTitle2((int*)m_aUnit.GetData(), bInit, TRUE);
}

// Column 폭을 지정한다.
// Hide 시켜야 할 칼럼을 지정한다.
void CCMStorWindSeisGrid::SetColumnAttr()
{
	int anColWidth[] = { 
		0, 125, 150,              // Story
		100, 100, 100, 100, 100, 100,   // Wind
		100, 100, 100, 100, 100, 100,   // Seismic
		0};                       // Module Index
	anColWidth[1] = 100;

	for (int i = 0; i < sizeof(anColWidth)/sizeof(int); i++)  SetColWidthByDPI(i+1, i+1, anColWidth[i]);

	switch(m_nViewMode)
	{
	case D_STOR_VIEWMODE_WIND:
		HideColsByIntend(3, 3, TRUE);
		SetStyleRange(CGXRange().SetCols(3, 3), CGXStyle().SetEnabled(FALSE));        
		if(CProduct::GetMovingType() == D_PRODUCT_MOVING_CH)
		{
			HideColsByIntend(8, 15, TRUE);
			SetStyleRange(CGXRange().SetCols(8, 15), CGXStyle().SetEnabled(FALSE));
		}
		else
		{
			HideColsByIntend(10, 15, TRUE);
			SetStyleRange(CGXRange().SetCols(10, 15), CGXStyle().SetEnabled(FALSE));
		}    
		break;
	case D_STOR_VIEWMODE_SEISMIC:
		HideColsByIntend(3, 9, TRUE);
		SetStyleRange(CGXRange().SetCols(3, 9), CGXStyle().SetEnabled(FALSE));
		SetRowHeightByDPI(0, 0, 64);
		break;
	}
}

////////////////////////////////////////////////////////////////////////////
// 여기부터 Column 값 변경이 다른 column의 상태나 값을 변경시키는 경우를
// 처리해 주기 위해서 Override 및 새로 정의되는 함수이다.
// Level 변경시 Height 변경, Height 변경시 Level 변경
void CCMStorWindSeisGrid::OnModifyCell(ROWCOL nRow, ROWCOL nCol)
{
	// 부모의 OnModifyCell을 항상 먼저 호출해 주어야 한다.
	CTBCommon::OnModifyCell(nRow, nCol);
	SaveCurrentRecordValue(nRow, nCol);
//  SaveCurrentRecordValue(nRow + 1, nCol);
	ChangeColumnStatus(nRow, nCol);
}

void CCMStorWindSeisGrid::OnCanceledEditing(ROWCOL nRow, ROWCOL nCol)
{
	// 부모의 OnCanceledEditing을 항상 먼저 호출해 주어야 한다.
	CTBCommon::OnCanceledEditing(nRow, nCol);
	if (!IsEdit()) return;
	RestorePrevRecordValue(nRow, nCol);
}

BOOL CCMStorWindSeisGrid::OnEndEditing(ROWCOL nRow, ROWCOL nCol)
{
	// 부모의 OnEndEditing을 호출한다.
	if (!CTBCommon::OnEndEditing(nRow, nCol)) return FALSE;
	if (!m_bBeforeSaved) return TRUE;

	// 값이 저장되었다는 플래그를 reset한다.
	m_bBeforeSaved = FALSE;
	return TRUE;
}

BOOL CCMStorWindSeisGrid::OnCanceledModify()  // Unused function
{
	CTBCommon::OnCanceledModify();

	return TRUE;
}

void CCMStorWindSeisGrid::ChangeColumnStatus(ROWCOL nRow, ROWCOL nCol)
{

}

// 변경된 후의 값이 저장된다.
void CCMStorWindSeisGrid::SaveCurrentRecordValue(ROWCOL nRow, ROWCOL nCol)
{
	if (m_bBeforeSaved) return; // 이전에 저장되었으면 리턴한다.
}

// 변경된 후의 값이 저장되므로 변경 이전의 값을 구하려면
// GetValueRowCol을 이용해야 한다. 단 이함수가 OnCanceledEditing()에서
// 호출되는 경우에 한해서이다.
void CCMStorWindSeisGrid::RestorePrevRecordValue(ROWCOL nRow, ROWCOL nCol)
{
	if (!m_bBeforeSaved) return;

	// Column ID를 구한다.
	int nColID;
	ROWCOL nLevelCol;
	GetColumnIDFor(nCol, nColID);
	GetColumnNoFor(2, nLevelCol);

	if (nColID == 3)
	{
		SetValueRange(CGXRange(nRow, nLevelCol), m_dblOldLevel);
	}

	// 플래그를 Reset시킨다.
	m_bBeforeSaved = FALSE;
}

// Story 이름을 바꾸어 주기 위해 Override한다.
BOOL CCMStorWindSeisGrid::OnInsertEmptyRecord(ROWCOL nRow)
{
	if (!CTBCommon::OnInsertEmptyRecord(nRow)) return FALSE;

	ROWCOL nNameCol, ncRow, ncCol;
	BOOL bCurSel = GetCurrentCell(ncRow, ncCol);
	ASSERT(bCurSel);
	if (!bCurSel) return TRUE;
	GetColumnNoFor(1, nNameCol);
	if (ncCol == nNameCol) return TRUE; // Name 필드를 수정하고 있으면 그냥 리턴

	int no=1;
	CString csStoryName;
	csStoryName.Format(_LS(IDS_WG_CMD__ADD2__New_d), no++);
	while (m_pDoc->m_pAttrCtrl->GetStorKey(csStoryName))
	{
		csStoryName.Format(_LS(IDS_WG_CMD__ADD2__New_d), no++);
	}
	SetValueRange(CGXRange(nRow, nNameCol), csStoryName);

	return TRUE;
}

