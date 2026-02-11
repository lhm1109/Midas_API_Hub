// CMStorGrid.cpp : implementation file
//
// 2005.5.12 층 순서를 고층부터 Display 하도록 변경(jbseon)
#include "stdafx.h"
#include "wg_cmd.h"
#include "CMStorGrid.h"

#include "CMStorDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "..\wg_base\wg_base_CompFunc.h"
#include "..\wg_base\wg_base_TreeMenuBarBase.h"
#include "..\wg_base\wg_base_I_GENModelBase.h"

#include "..\wg_db\wg_db_ViewBuff.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl2.h"
#include "..\wg_db\wg_db_ViewCtrl.h"
#include "..\wg_db\TempDrawObjectCtrl.h"   // for CTempDrawObjectCtrl
#include "..\wg_db\StorSubData.h"

#include "..\wg_main\wg_mainRes2.h"  // for ID_VIEW_REDRAW, ID_VIEW_SNAP_OFF

#define STOR_KEY_FMT    _T("%d")
#define STOR_DATA_FMT   _T("%.15g")

#define D_STOR_DIAPH_YES  _LS(IDS_WG_CMD__ADDD__Consider)
#define D_STOR_DIAPH_NO   _LS(IDS_WG_CMD__ADDD__Do_not_consider)

/////////////////////////////////////////////////////////////////////////////
// CCMStorGrid
CString CCMStorGrid::m_aCurDefVal[] = {_T(""), _T(""), _T("0"), _T("0"), _T(""), _T(""), _T("0"), _T("0")};

CCMStorGrid::CCMStorGrid()
{
	m_bBeforeSaved = FALSE;

	T_STOR_UNIT * pUnit = &(CUnitCtrl::m_STOR_UNIT);

	AddColInfo(_T("ID"),                     t_integer, 0);
	AddColInfo(_LS(IDS_WG_CMD__ADDD__Module_Name),                   t_string,  D_UNITSYS_NONE);
	AddColInfo(_LS(IDS_WG_CMD__ADDD__Story_Name),                   t_string,  pUnit->StoryName);
	AddColInfo(_LS(IDS_WG_CMD__ADDD__Level),                  t_real,    pUnit->dStoryLevel);
	AddColInfo(_LS(IDS_WG_CMD__ADDD__Height),                 t_real,    D_UNITSYS_BASE_LENGTH);
	AddColInfo(_LS(IDS_WG_CMD__ADDD__Floor_nDiaphragm),       t_string,  pUnit->bFloorDiaphragm);
	AddColInfo(_LS(IDS_WG_CMD__ADDD__Tracing_Drawing),		  t_string, D_UNITSYS_NONE);
	AddColInfo(_LS(IDS_WG_CMD__ADDD__Line_Grid), t_integer, D_UNITSYS_NONE);
	AddColInfo(_T("Module index"),                                t_integer,  D_UNITSYS_NONE);

	m_aPrimaryKey.Add(0);

	CString aCurDefVal[] = {_T(""), _T(""), _T("0"), _T("0"), D_STOR_DIAPH_YES,_T(""), _T("0"), _T("0")};
	for(int i=0; i<sizeof(aCurDefVal)/sizeof(CString); i++)
		m_aCurDefVal[i] = aCurDefVal[i];

	//SetSelectRegionMode(FALSE);
	m_nRow_prev = -1;
}

CCMStorGrid::~CCMStorGrid()
{
}

BEGIN_MESSAGE_MAP(CCMStorGrid, CTBCommon)
	//{{AFX_MSG_MAP(CCMStorGrid)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CCMStorGrid message handlers

void CCMStorGrid::Initialize(CDBDoc* pDoc)
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

	// Module Name Column의 스타일 변경
	SetStyleRange(CGXRange().SetCols(2), CGXStyle()
		.SetValueType(GX_VT_STRING)
		.SetReadOnly(TRUE)
		.SetHorizontalAlignment(DT_LEFT));

	// Story Name Column의 스타일 변경
	//SetStyleRange(CGXRange().SetCols(2), CGXStyle()
	SetStyleRange(CGXRange().SetCols(3), CGXStyle()
			.SetValueType(GX_VT_STRING)
			.SetHorizontalAlignment(DT_LEFT));

	// Floor Diaphragm의 스타일 변경
	CString szChoiceList;
	szChoiceList.Format(_T("%s\n%s\n"), D_STOR_DIAPH_YES, D_STOR_DIAPH_NO);
	//SetStyleRange(CGXRange().SetCols(5), CGXStyle()
	SetStyleRange(CGXRange().SetCols(6), CGXStyle()
			.SetControl(GX_IDS_CTRL_TABBED_COMBOBOX)
			.SetChoiceList(szChoiceList)
			.SetUserAttribute(GX_IDS_UA_TABLIST_KEYCOL, _T("0"))
			.SetUserAttribute(GX_IDS_UA_TABLIST_TEXTCOL, _T("0"))
			.SetUserAttribute(GX_IDS_UA_TABLIST_SHOWALLCOLS, _T("1"))
			.SetValueType(GX_VT_STRING)
			.SetHorizontalAlignment(DT_LEFT)
			.SetValue(_T("")));

	// Tracing Drawing
	InitTracingLine();

	SetStyleRange(CGXRange().SetCols(8), CGXStyle()
		.SetControl(GX_IDS_CTRL_CHECKBOX3D)
		.SetValueType(GX_VT_STRING)
		.SetHorizontalAlignment(DT_CENTER)
		.SetVerticalAlignment(DT_VCENTER)
		.SetValue(_T("0"))
	);

	// MQC 12651 : Module index 열 폭이 0이긴 한데 유저가 따블클릭해서 이거 뭐냐고 물어봄.. 숨김
	HideColsByIntend(9, 9, TRUE);

	// real type column의 스타일 변경
	//int anCol[] = {3, 4};
	int anCol[] = {4, 5};
	for (int i = 0; i < sizeof(anCol)/sizeof(int); i++)
	{
		SetStyleRange(CGXRange().SetCols(anCol[i]), CGXStyle()
			.SetValueType(GX_VT_NUMERIC)
			.SetFormat(GX_FMT_FIXED)
			.SetPlaces(2));
	}
	 
	SetColumnAttr();

#ifdef _MVIL
	SetModeDisplayOnly(TRUE);
#endif

	SetRightValidCell(1, 1);

	// Sorting : Level로 Sorting한다. : 이 순서는 안 바뀌는 것으로 가정한다.
	// 왜냐하면 Height는 계산되어 설정되는 값이고 성능상의 이유로 Level별로
	// Sorting되어 있다고 가정하고 프로그래밍 되어 있다.

	CGXSortInfo SortInfo;
	m_aSortInfo.RemoveAll();
	// by Module index
	SortInfo.nRC       = 8;
	SortInfo.sortType  = CGXSortInfo::numeric;
	SortInfo.sortOrder = CGXSortInfo::descending;
	m_aSortInfo.Add(SortInfo);
	// by Level
	SortInfo.nRC       = 3;
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
BOOL CCMStorGrid::InsertRecordByKey(const T_STOR_K &key, const T_STOR_D &data)
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

void CCMStorGrid::InitTracingLine()
{
	// Tracing Drawing
	CString szChoiceListTracing;
	GetTracingChoiceList(szChoiceListTracing);
	//szChoiceList.Format(_T("%s\n%s\n"), D_STOR_DIAPH_YES, D_STOR_DIAPH_NO);	
	SetStyleRange(CGXRange().SetCols(7), CGXStyle()
		.SetControl(GX_IDS_CTRL_TABBED_COMBOBOX)
		.SetChoiceList(szChoiceListTracing)
		.SetUserAttribute(GX_IDS_UA_TABLIST_KEYCOL, _T("0"))
		.SetUserAttribute(GX_IDS_UA_TABLIST_TEXTCOL, _T("0"))
		.SetUserAttribute(GX_IDS_UA_TABLIST_SHOWALLCOLS, _T("1"))
		.SetValueType(GX_VT_STRING)
		.SetHorizontalAlignment(DT_LEFT)
		.SetValue(_T("")));
}

// 수정 없음
BOOL CCMStorGrid::DeleteRecordByKey(const T_STOR_K &key, const T_STOR_D &data)
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
void CCMStorGrid::UnitChanged(CDBDoc* pDoc)
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
void CCMStorGrid::UpdateBuffer(CDBDoc* pDoc)
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
		{
			//// 삭제될 위치(nRow)를 찾아서 지운다.
			//pViewBuff->GetStor(nKey, key, data);
			//DeleteRecordByKey(key, data);
			bResetGrid = TRUE;
			nModified |= 0x02;

			auto& storSubData = CStorSubData::GetInstance();
			pViewBuff->GetStor(nKey, key, data);
			if (storSubData.HasStorKeyForLineGrid(key) == true)
			{
				storSubData.DeleteStorKeyForLineGrid(key);
			}
		}
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
				nModified |= 0x02;
				DeleteRecordByKey(key, data);
				InsertRecordByKey(keyBak, dataBak);
				// 플래그 수정
				bMFD = FALSE;
				break;

		case UR_TDAS_ADD:
		case UR_TDAS_DEL:
			// Tracing Line list update
			InitTracingLine();
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
// CCMStorGrid Overridable Functions
// 수정 없음
void CCMStorGrid::OnChangedSelectionPublic(long *keys, int nSize)
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
long CCMStorGrid::GetSelectedKeys(CArray<long, long> &caKey)
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
BOOL CCMStorGrid::ValidateField(CString value, int nColID)
{
	if (nColID >= GetColCount())
	{
		SetWarningText(_LS(IDS_WG_CMD__ADDD___CCMStorGrid__ValidateField__Erro));
		return FALSE;
	}

	int nErrCode;

	if (nColID == 0)  // ID : error check 없음
	{
		return TRUE;  
	}
	else if (nColID == 1)  // Module Name
	{
		nErrCode; // Do Nothing...
	}
	else if (nColID == 2 || nColID == 5 || nColID == 6)  // Story Name, Diaphragm, Tracing Drawing
	{
		nErrCode; // Do Nothing...
	}
	else if (nColID == 7)  // Line Grid
	{
		nErrCode; // Do Nothing...
	}
	else if (nColID == 8)  // Module Index
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
		//if (nColID == 3 && _tstof(value) < 0.0)  // Height의 경우 음수는 안됨
		if (nColID == 4 && _tstof(value) < 0.0)  // Height의 경우 음수는 안됨
		{
			SetWarningText(_LS(IDS_WG_CMD__ADDD__Height_can_t_be_negative_));
			return FALSE;
		}
	}
	return TRUE;
}

// 수정 없음
BOOL CCMStorGrid::ValidateRecord(CStringArray &value, CRowColArray &aCols)
{
	CString msg = _T("");
	int nPos;

	// ID 필드는 제외
	CArray<int, int> aCheck;
	//aCheck.Add(1);    // Module Name 제외
	aCheck.Add(2); aCheck.Add(3); aCheck.Add(4); aCheck.Add(5);

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
BOOL CCMStorGrid::ValidateAndMakeRecord(CStringArray &value, CRowColArray& aCols, T_STOR_K &key, T_STOR_D &data)
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

			if (!m_pDoc->m_pAttrCtrl->GetStor(key, data, FALSE))  data.Initialize();		// 2014-05-27. by Hsshim. Story Grid를 둘로 나눴기 때문에 여기서 초기화 시킴.
		}
		else ConvStrToData(i, value[nPos], key, data);
	}
	return TRUE;
}

// Add???? 수정
BOOL CCMStorGrid::AddToDB(ROWCOL nRow)
{
	if (!CanSaveData()) return FALSE;

	// Record를 만들어서 DB에 입력한다.
	T_STOR_K key;
	T_STOR_D data;

	GetValue(nRow, key, data);
	return m_pDoc->m_pDataCtrl->AddStor(data);  
}

// Multiple data paste 지원시 Add???? 수정
BOOL CCMStorGrid::AddToDB(CArray<void*, void*>& raKey, CArray<void*, void*>& raData)
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
BOOL CCMStorGrid::DeleteFromDB(ROWCOL nRow)
{
	T_STOR_K key;
	T_STOR_D data;
 
	GetValue(nRow, key, data);
	//return m_pDoc->m_pDataCtrl->DelStor(data.StoryName);
	return m_pDoc->m_pDataCtrl->DelStor(key, FALSE);
}

// 2Part Key 아니면 수정 없음
BOOL CCMStorGrid::DeleteFromDB(CRowColArray& awRows)
{
	if (awRows.GetSize() <= 0) return TRUE;

	// Story Name 칼럼을 얻는다.
	//ROWCOL nCol;
	//VERIFY(GetColumnNoFor(1, nCol));
	//VERIFY(GetColumnNoFor(2, nCol));

	//CString value;
	//CArray<CString, CString&> awKeys;
 
	T_STOR_K key;
	T_STOR_D data;
	CArray<T_STOR_K, T_STOR_K> awKeys;  awKeys.RemoveAll();

	for (int i = 0; i < awRows.GetSize(); i++)
	{
		//value = GetValueRowColBeforeEdit(awRows.GetAt(i), nCol);
		GetValue(awRows.GetAt(i), key, data);
		awKeys.Add(key);
	}

	return m_pDoc->m_pDataCtrl->DelStor(awKeys, FALSE);
}

// 2 Part Key 아니면 수정 없음
BOOL CCMStorGrid::ModifyToDB(ROWCOL nRow)
{
	if (!CanSaveData()) return FALSE;

	// Story Name 칼럼을 얻는다.
	ROWCOL nCol;
	VERIFY(GetColumnNoFor(1, nCol));

	CString csOldName;
	T_STOR_K key;
	T_STOR_D data;

	csOldName = GetValueRowColBeforeEdit(nRow, nCol);

	GetValue(nRow, key, data);

	// 수정한다. 실패하면 에러 리턴
	//return m_pDoc->m_pDataCtrl->ModifyStor(csOldName, data);
	return m_pDoc->m_pDataCtrl->ModifyStor(key, data);
}

// 수정 없음
BOOL CCMStorGrid::ModifyToDB(CStringArray& raOldKey, 
												 CArray<void*, void*>& raKey, 
												 CArray<void*, void*>& raData)
{
	ASSERT(raKey.GetSize() == raData.GetSize());
	ASSERT(raKey.GetSize() == raOldKey.GetSize());
	//CArray<CString, CString&> aOldName;
	CArray<T_STOR_K,T_STOR_K> aStorK_old;   aStorK_old.RemoveAll();
	T_STOR_K key;
	T_STOR_D data;
	CArray<T_STOR_D, T_STOR_D&> aData;
	for (int i = 0; i < raKey.GetSize(); i++)
	{
		key = _tstol(raOldKey.GetAt(i));
		m_pDoc->m_pAttrCtrl->GetStor(key, data, FALSE);
		aStorK_old.Add(key);
		//aOldName.Add(data.StoryName);
		T_STOR_D TempStor2 = *((T_STOR_D*)raData.GetAt(i));
		aData.Add(*((T_STOR_D*)raData.GetAt(i)));
	}
	//return m_pDoc->m_pDataCtrl->ModifyStor(aOldName, aData);
	return m_pDoc->m_pDataCtrl->ModifyStor(aStorK_old, aData);
}

// 수정 없음
CString CCMStorGrid::GetNewKey()
{
	//ASSERT(0);  // 지금 key 없는데.... 쯥....
	CString value;
	T_STOR_K key = m_pDoc->m_pAttrCtrl->GetStartNumStor();
	value.Format(STOR_KEY_FMT, key);
	//value = _T("");
	return value;
}

// 수정 없음
CString CCMStorGrid::GetDefValue(int nIndex)
{
	return m_aCurDefVal[nIndex];
}

// 수정 없음
CString* CCMStorGrid::GetDefValueArray()
{
	return m_aCurDefVal;
}

// 수정 없음
BOOL CCMStorGrid::MakeKeyAndDataArray(CStringArray& value, CRowColArray& awCols, 
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
void CCMStorGrid::DeleteKeyAndDataArray(CArray<void*, void*>& aKey, CArray<void*, void*>& aData)
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
long CCMStorGrid::GetCountData()
{
	CDBDoc* pDoc = m_pDoc;
	return pDoc->m_pAttrCtrl->GetCountStor();
}

// 수정 없음
void CCMStorGrid::MakeItemFiltered(long nFilteredNum, long* aFilteredKeys, CRowColArray& awCols)
{
	CDBDoc* pDoc = m_pDoc;
	T_STOR_D Data;

	for(long nCount=0 ; nCount < nFilteredNum; nCount++)
 	{
		pDoc->m_pAttrCtrl->GetStor(aFilteredKeys[nCount], Data, FALSE);
		SetValue(GetParam()->GetData(), nCount+1, aFilteredKeys[nCount], Data, awCols);
	}
}

// 수정 없음
void CCMStorGrid::MakeItemNormal(long nNumData, CRowColArray& awCols)
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
		pDoc->m_pAttrCtrl->GetStor(KeyBuf[nCount], Data, FALSE);
		SetValue(GetParam()->GetData(), nCount+1, KeyBuf[nCount], Data, awCols);
	} // end of loop nCount

	delete []KeyBuf;
}

// 2 Part Key 외 수정 없음
void CCMStorGrid::MakeItemEditingRecord(ROWCOL ncRow, CStringArray& EditingRecord)
{
	CDBDoc* pDoc = m_pDoc;
	T_STOR_K Key;
	T_STOR_D Data;
	for (int i = 0; i < GetColCount(); i++)
	{
		if (i == 0) Key = _tstol(EditingRecord[i]);
		else ConvStrToData(i, EditingRecord[i], Key, Data);
	}
	pDoc->m_pUnitCtrl->ConvertUnitStorPrevious(Data);
	// 현재 값을 설정한다.
	SetValue(ncRow, Key, Data);
}

/////////////////////////////////////////////////////////////////////////////
// CCMStorGrid Implementation
// 2 Part Key 외 수정 없음
BOOL CCMStorGrid::GetValue(ROWCOL nRow, T_STOR_K &key, T_STOR_D &data)
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
			if (!m_pDoc->m_pAttrCtrl->GetStor(key, data, FALSE))   data.Initialize();
		}
		else ConvStrToData(i, str, key, data);
	}
	return TRUE;
}

BOOL CCMStorGrid::GetValue_StoryLevel(ROWCOL nRow, T_STOR_K &key, double &rdVal)
{
	rdVal = 0.0;

	if (nRow < 1) return FALSE;
	if (IsAppendRow(nRow) || nRow > GetRowCount()) return FALSE;

	ROWCOL nCol;
	//VERIFY(GetColumnNoFor(m_aTitle[2], nCol));
	VERIFY(GetColumnNoFor(m_aTitle[3], nCol));
	CString str = GetValueRowCol(nRow, nCol);

	// 수식인 경우 값으로 변환한다.
	// Validation이 OK인 Field이므로 항상 TRUE 리턴
	VERIFY(ConvertFormulaToValue(str)); 

	rdVal = _tstof(str);

	key = GetStorKey(nRow);

	return TRUE;
}

// 수정 없음
BOOL CCMStorGrid::SetValue(ROWCOL nRow, const T_STOR_K &key, const T_STOR_D &data)
{
	//
	CRowColArray awCols;
	awCols.SetSize(GetColCount());
	for (int i = 0; i < GetColCount(); i++)
		VERIFY(GetColumnNoFor(m_aTitle[i], awCols[i]));
	return SetValue(GetParam()->GetData(), nRow, key, data, awCols);
}

// 2Part Key, Column Enable/Disable이 있으면 수정
BOOL CCMStorGrid::SetValue(CGXData* pData, ROWCOL nRow, const T_STOR_K &key, const T_STOR_D &data, CRowColArray &awCols)
{
	//
	//CString strStorName = _T("");
	ROWCOL nCol_StorName = 0;
	//if (!GetColumnNoFor(m_aTitle[1], nCol_StorName))  { ASSERT(0);  return FALSE; }
	if (!GetColumnNoFor(m_aTitle[2], nCol_StorName))  { ASSERT(0);  return FALSE; }

	CString str;
	int nValueType;
	for (int i = 0; i < GetColCount(); i++)
	{
		if(i==0) 
		{
			if (key == 0) str.Empty();
			else str.Format(STOR_KEY_FMT,key);

			//if (key != 0)  strStorName = m_pDoc->m_pAttrCtrl->GetStorNameWithModuleName(key);
		}
		else ConvDataToStr(i, data, str, key);

		//if (awCols[i] == nCol_StorName)  str = strStorName;

		if (m_aType[i] == t_string) nValueType = GX_VT_STRING;
		else nValueType = GX_VT_NUMERIC;
		pData->StoreValueRowCol(nRow, awCols[i], str, gxOverride, nValueType);
	}

//   BOOL bEnableDefineAreaBtn = FALSE;
//   if (data.nAreaPosNum!=0)  bEnableDefineAreaBtn = TRUE;
//   EnableDisableColumn_DefineAreaBtn(nRow, bEnableDefineAreaBtn);

	return TRUE;
}

// void CCMStorGrid::EnableDisableColumn_DefineAreaBtn(ROWCOL nRow, BOOL bEnableDefineAreaBtn)
// {
//   // Define Area Button
//   ROWCOL nCol_DefineAreaBtn = 7;
// 
//   if (bEnableDefineAreaBtn)
//   {
//     SetStyleRange(CGXRange().SetCells(nRow, nCol_DefineAreaBtn), CGXStyle().SetEnabled(TRUE).SetControl(GX_IDS_CTRL_PUSHBTN));
//   }
//   else
//   {
//     SetStyleRange(CGXRange().SetCells(nRow, nCol_DefineAreaBtn), CGXStyle().SetEnabled(FALSE).SetControl(GX_IDS_CTRL_STATIC).SetValue(_T("")));
//   }
// }

// 데이타 특성에 따라 수정
void CCMStorGrid::ConvStrToData(int i, const CString& value, const T_STOR_K& key, T_STOR_D& data)
{
	if (i == 1) i;    // module name // do nothing
	else if (i == 2) data.StoryName = value;
	else if (i == 3) data.dStoryLevel = _tstof(value);
	else if (i == 4) i;  // do nothing
	else if (i == 5)
	{
		if (value == D_STOR_DIAPH_YES) data.bFloorDiaphragm = TRUE;
		else data.bFloorDiaphragm = FALSE;
	}
	else if (i == 6)
	{
		data.TdasK = m_pDoc->m_pAttrCtrl2->GetTdasKey(const_cast<CString&>(value));
	}
	else if (i == 7)
	{
		auto& storSubData = CStorSubData::GetInstance();
		if (value == L"0")
		{
			storSubData.DeleteStorKeyForLineGrid(key);
		}
		else
		{
			storSubData.AddStorKeyForLineGrid(key);
		}
	}
	else if (i == 8) i;  // do nothing
}

// 데이타 특성에 따라 수정
void CCMStorGrid::ConvDataToStr(int i, const T_STOR_D& data, CString& value, const T_STOR_K StorK)
{
	if (i == 1) { value = m_pDoc->m_pAttrCtrl->GetModuleNameOfStor(StorK);}
	else if (i == 2) value = data.StoryName;
	else if (i == 3) value.Format(STOR_DATA_FMT, data.dStoryLevel);
	else if (i == 4) value = _T("0");
	else if (i == 5) 
	{
		if (data.bFloorDiaphragm) value = D_STOR_DIAPH_YES;
		else value = D_STOR_DIAPH_NO;
	}
	else if(i==6)
	{
		value = m_pDoc->m_pAttrCtrl2->GetTdasName(data.TdasK);
		if (value.IsEmpty())
		{
			value = _LS(IDS_WG_CMD__ADDD__None);
		}
	}
	else if (i == 7)
	{
		auto& storSubData = CStorSubData::GetInstance();
		value = (storSubData.HasStorKeyForLineGrid(StorK) == true) ? L"1" : L"0";
	}
	else if (i == 8) 
	{
		CString strModuleName;
		int iModl = m_pDoc->m_pAttrCtrl->GetModuleIndexOfStor(StorK, strModuleName);
		value.Format(_T("%d"), iModl);
	}
	else value = _LS(IDS_WG_CMD__ADDD__Error);
}

// 2Part Key 외 수정 없음
void CCMStorGrid::MakeSearchKey(const T_STOR_K &key, const T_STOR_D &data, CStringArray &aKey)
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
void CCMStorGrid::GetAllSelectedStor(CArray<T_STOR_K, T_STOR_K>& aKey)
{
	for (int i = 0; i < m_aSelectedKeys.GetSize(); i++)
		aKey.Add(m_aSelectedKeys[i]);
}

// 데이타 특성에 따라 수정
void CCMStorGrid::SetHeaderTitle(T_UNIT_SYSTEM &UnitSystem, BOOL bInit)
{
	SetHeaderTitle2((int*)m_aUnit.GetData(), bInit, TRUE);
}

// Column 폭을 지정한다.
// Hide 시켜야 할 칼럼을 지정한다.
void CCMStorGrid::SetColumnAttr()
{
	int anColWidth[] = { 0, 120, 110, 90, 90, 110, 120, 60, 0};

	double dFactor = globalUtils.GetDPIScale();

	for (int i = 0; i < sizeof(anColWidth) / sizeof(int); i++)
	{
		int nWidthDPI = anColWidth[i];
		if (1.0 < dFactor && dFactor <= 1.25)
		{
			nWidthDPI = int(anColWidth[i] * dFactor);
		}
		else if (1.25 < dFactor && dFactor <= 1.5)
		{
			nWidthDPI = int(anColWidth[i] * dFactor * 0.95);
		}
		else
		{
			nWidthDPI = int(anColWidth[i] * dFactor);
		}
		SetColWidth(i + 1, i + 1, nWidthDPI);
	}
}

void CCMStorGrid::SetStoryHeight(ROWCOL nHeightCol)
{
	T_STOR_K key;
	T_STOR_K key_1 = 0;
	double dStoryLevel = 0.0;
	double dblHeight, dblPrevLevel;
	ROWCOL nRowCount = GetRowCount();
	if (nRowCount <= 1) return; // if no story exist then return

	ROWCOL ncRow, ncCol;
	BOOL bGet = GetCurrentCell(ncRow, ncCol);

	CArray<T_STOR_K,T_STOR_K> aStorK;
	m_pDoc->m_pAttrCtrl->GetStorKeyList(aStorK);

	CMap<T_STOR_K,T_STOR_K, double,double> mStor2Height;
	m_pDoc->m_pAttrCtrl->GetStorHeight(aStorK, mStor2Height, TRUE);
		
	GetValue_StoryLevel(nRowCount - 1, key, dStoryLevel); // 첫번째 층(맨 아래층)의 데이터를 얻는다.
	dblPrevLevel = dStoryLevel;
	for (int i = nRowCount - 2; i >= 1; i--) // Append Row 제외
	{
		if(m_nEditMode == addnew && bGet && ncRow == i) continue;  // 편집 상태인 row는 제외
		GetValue_StoryLevel(i, key, dStoryLevel);
		key_1 = GetStorKey(i+1);
		if (!mStor2Height.Lookup(key_1, dblHeight))  { ASSERT(0);  dblHeight = dStoryLevel - dblPrevLevel; }
		if (dblHeight <= 0.0)
		{
			dblHeight = dStoryLevel - dblPrevLevel;	// RPMNX-1137 : 절점 없이 Story를 만들 수 있기 때문에 height가 없으면 하부층과 차이로 만든다.
			//dblHeight = 0.0; // Add될 때 Update된 것과 입력시 남아 있는 것 처리
		}
		SetValueRange(CGXRange(i+1, nHeightCol), dblHeight);
		BOOL bEnable = FALSE;
		if (IsEdit() && i+1 == ncRow) bEnable = TRUE;
		SetStyleRange(CGXRange(i+1, nHeightCol), CGXStyle().SetEnabled(bEnable));  //2
		dblPrevLevel = dStoryLevel;
	}

	if (nRowCount > 1)
	{
		SetValueRange(CGXRange(1, nHeightCol), double(0.0));   // 최상층 층고 0으로 강제 설정 
		SetStyleRange(CGXRange(1, nHeightCol), CGXStyle().SetEnabled(FALSE));  // 최상층 층고는 편집불가
	}
}

void CCMStorGrid::AdjustAppendRowNo(ROWCOL nCol)
{
	CTBCommon::AdjustAppendRowNo(nCol);

	ROWCOL nHeightCol;
	//GetColumnNoFor(3, nHeightCol);
	GetColumnNoFor(4, nHeightCol);

	// Story를 설정해 준다.
	SetStoryHeight(nHeightCol);

	BOOL bEnable = FALSE;
	ROWCOL nRowCount = GetRowCount();
	if (nRowCount > 1) bEnable = TRUE;
	// 첫번째 Row의 Story는 Disable한다.  => 층 개념 변경으로 첫번째 Row의 Disable을 취소한다.
//  SetStyleRange(CGXRange(1, nHeightCol), CGXStyle().SetEnabled(FALSE));  
	// Append Row의 Story는 Record가 있으면 Enable한다.
	SetStyleRange(CGXRange(nRowCount, nHeightCol), CGXStyle().SetEnabled(bEnable));
	// 현재 셀이 Disable된 Cell에 있으면 첫번째 Valid한 Cell로 이동
	if (!bEnable)
	{
		ROWCOL ncCol, ncRow;
		GetCurrentCell(ncRow, ncCol);
		if (ncCol == nHeightCol) SetRightValidCell(ncRow, 0);
	}
}

T_STOR_K CCMStorGrid::GetStorKey(ROWCOL nRow)
{
	if (nRow == 0)  return 0;

	ROWCOL nCol_key = 1;
	VERIFY(GetColumnNoFor(m_aTitle[0], nCol_key));
	T_STOR_K StorK = _ttoi(GetValueRowCol(nRow,nCol_key)); // Stor Key.

	return StorK;
}

void CCMStorGrid::ChangeColumnStatus_height(ROWCOL nRow, ROWCOL nCol)
{
	ROWCOL nLevelCol, nHeightCol;
	GetColumnNoFor(3, nLevelCol);   //GetColumnNoFor(2, nLevelCol);
	GetColumnNoFor(4, nHeightCol);  //GetColumnNoFor(3, nHeightCol);

	if (nCol != nHeightCol) return;  // height가 변경될 때만 쓰임
	CString csLevel, csHeight;
	double dblLevel, dblHeight;

	CGXControl *pControl;
	pControl = GetControl(nRow, nCol);  // 현재층의 control을 얻는다.  
	if (pControl->IsActive() && nCol == nHeightCol) pControl->GetValue(csHeight);
	else csHeight = GetValueRowCol(nRow, nHeightCol);
	csLevel = GetValueRowCol(nRow, nLevelCol);

	T_STOR_K key;
	T_STOR_D data;
	CArray<T_STOR_K, T_STOR_K> aStorKey;
	m_pDoc->m_pAttrCtrl->GetStorKeyList(aStorKey);
	int nNumStor = aStorKey.GetSize();

	data.Initialize();
	dblHeight = _tstof(csHeight);
	dblLevel  = _tstof(csLevel);
	GetValue(nRow, key, data);  // 현재층의 데이터  

	if (!pControl->IsActive() && dblLevel != 0.0) return;
	// Height칼럼이 active ---> Height를 직접 수정한 경우
	// Height칼럼이 inactive ---> Paste됨
	// 또는 Level이 0.0인 경우만 Level수정한다.
	// Paste시 Level이 0.0인 경우 Level 재 수정
	// 층 순서 변경 05.05.12
	//dblLevel = data.dStoryLevel + dblHeight;  // 윗층 Level = 현재층 Level + 현재층 층고
	//if(nRow < nNumStor) SetValueRange(CGXRange(nRow + 1, nLevelCol), dblLevel);  
	if (nRow > 1)
	{
		GetValue(nRow-1, key, data);  // 위층의 데이터
		dblLevel = data.dStoryLevel - dblHeight;
		SetValueRange(CGXRange(nRow, nLevelCol), dblLevel);
	}
}

void CCMStorGrid::ChangeColumnStatus(ROWCOL nRow, ROWCOL nCol)
{
	//ROWCOL nLevelCol, nHeightCol, nUseArea;
	ROWCOL nLevelCol, nHeightCol;
	GetColumnNoFor(3, nLevelCol);  //GetColumnNoFor(2, nLevelCol);
	GetColumnNoFor(4, nHeightCol);  //GetColumnNoFor(3, nHeightCol);
	//GetColumnNoFor(5, nUseArea);

	if (nCol == nHeightCol) ChangeColumnStatus_height(nRow, nCol);
}

// 변경된 후의 값이 저장된다.
void CCMStorGrid::SaveCurrentRecordValue(ROWCOL nRow, ROWCOL nCol)
{
	if (m_bBeforeSaved) return; // 이전에 저장되었으면 리턴한다.

	ROWCOL nLevelCol, nHeightCol;
	GetColumnNoFor(3, nLevelCol);  //GetColumnNoFor(2, nLevelCol);
	GetColumnNoFor(4, nHeightCol);  //GetColumnNoFor(3, nHeightCol);
	if(nCol != nHeightCol) return;

	m_dblOldLevel = _tstof(GetValueRowCol(nRow, nLevelCol));
	m_bBeforeSaved = TRUE;
}

// 변경된 후의 값이 저장되므로 변경 이전의 값을 구하려면
// GetValueRowCol을 이용해야 한다. 단 이함수가 OnCanceledEditing()에서
// 호출되는 경우에 한해서이다.
void CCMStorGrid::RestorePrevRecordValue(ROWCOL nRow, ROWCOL nCol)
{
	if (!m_bBeforeSaved) return;

	// Column ID를 구한다.
	int nColID;
	ROWCOL nLevelCol;
	GetColumnIDFor(nCol, nColID);
	GetColumnNoFor(3, nLevelCol);  //GetColumnNoFor(2, nLevelCol);

	//if (nColID == 3) // Height
	if (nColID == 4) // Height
	{
//2    SetValueRange(CGXRange(nRow + 1, nLevelCol), m_dblOldLevel);
		SetValueRange(CGXRange(nRow, nLevelCol), m_dblOldLevel);
	}

	// 플래그를 Reset시킨다.
	m_bBeforeSaved = FALSE;
}

// Story 이름을 바꾸어 주기 위해 Override한다.
BOOL CCMStorGrid::OnInsertEmptyRecord(ROWCOL nRow)
{
	if (!CTBCommon::OnInsertEmptyRecord(nRow)) return FALSE;

	ROWCOL nNameCol, ncRow, ncCol;
	BOOL bCurSel = GetCurrentCell(ncRow, ncCol);
	ASSERT(bCurSel);
	if (!bCurSel) return TRUE;
	//GetColumnNoFor(1, nNameCol);
	GetColumnNoFor(2, nNameCol);
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

void CCMStorGrid::GetTracingChoiceList(CString& strChoiceList)
{
	CArray<CString, CString&> aTdasList;
	m_pDoc->m_pAttrCtrl2->GetTdasList(aTdasList);
	strChoiceList = _LS(IDS_WG_CMD__ADDD__None) + _T("\n");

	for (int i=0; i<aTdasList.GetSize(); i++)
	{
		strChoiceList += aTdasList.GetAt(i) +_T("\n");
	}
}

void CCMStorGrid::SetLineGridAll(bool checkAll)
{
	auto& storSubData = CStorSubData::GetInstance();
	if (checkAll)
	{
		CArray<T_STOR_K, T_STOR_K> arrKey;
		m_pDoc->m_pAttrCtrl->GetStorKeyList(arrKey, FALSE);

		CDBDoc* pDoc = (CDBDoc*)CTreeMenuBarBase::GetDocST();
		auto size = arrKey.GetSize();
		for (int i = 0; i < size; i++)
		{
			T_STOR_D StorD;
			if (!pDoc->m_pAttrCtrl->GetStor(arrKey[i], StorD)) continue;

			if (pDoc->m_pAttrCtrl2->ExistTdas(StorD.TdasK))
			{
				if (I_GENModelBase::GetCurMySelfST()->IsCurShowTracingStoryKey(arrKey[i]) == FALSE)
					I_GENModelBase::GetCurMySelfST()->AddCurShowTracingStoryKey(arrKey[i]);
			}
		}

		storSubData.AddAllStorKeyForLineGrid(false);
		I_GENModelBase::GetCurMySelfST()->ViewInvalidate4WorksTree(FALSE);
	}
	else
	{
		storSubData.DeleteAllStorKeyForLineGrid();
	}

	CString strValue = (checkAll) ? L"1" : L"0";

	for (int idxRow = 1; idxRow < GetRowCount(); idxRow++)
	{
		ROWCOL nCol;
		GetColumnNoFor(7, nCol);
		SetValueRange(CGXRange(idxRow, nCol), strValue);
	}
}
