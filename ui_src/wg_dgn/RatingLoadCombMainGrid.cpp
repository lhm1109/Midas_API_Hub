// RatingLoadCombMainGrid.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "RatingLoadCombMainGrid.h"

#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_LoadDB.h"
#include "..\wg_db\wg_db_LoadDgnCodeCheck.h"

#include "RatingLoadCombSubGrid.h"
#include "..\wg_base\wg_base_ServiceProvider.h"
#include "..\wg_tb\Tb_ServiceDef.h"
#include "..\wg_db\AttrCtrl2.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define LCOM_KEY_FMT        _T("%d")
#define D_TYPE_DEF  D_LCOM_CT_ADD

/////////////////////////////////////////////////////////////////////////////
// CRatingLoadCombMainGrid
CString CRatingLoadCombMainGrid::m_aCurDefVal[] = {_T(""), _T(""), _T(""), _T(""), _T(""), _T("0")};
BOOL CRatingLoadCombMainGrid::m_bElastStag = FALSE;
BOOL CRatingLoadCombMainGrid::m_bTB10002_1_05 = FALSE;
CRatingLoadCombMainGrid::CRatingLoadCombMainGrid()
{
	CString DefVal[] = {_T(""), _T(""), D_TYPE_DEF, _T(""), _T(""), _T("0")};
	for(int i=0; i<sizeof(DefVal)/sizeof(CString); i++)	m_aCurDefVal[i] = DefVal[i];

	//m_nLcomType = D_LCOMTYPE_GENERAL;
	m_pSubGrid = 0;
	m_pSubGridMV = 0;
	
	m_bIgnoreSetEditMode = FALSE;

	m_aCurDefVal[4] = _LS(IDS_CMD_LCOM_ACTIVE);

	AddColInfo(_ULS(Key), t_integer, 0);
	AddColInfo(_LS(IDS_WG_CMD__ADDD__No),             t_integer, 0);
	AddColInfo(_LS(IDS_WG_CMD__ADDD__Name),           t_string,  0);
	AddColInfo(_LS(IDS_WG_CMD__ADDD__Type),           t_string,  0);
	AddColInfo(_LS(IDS_WG_CMD__ADDD__Description),    t_string,  0);
	AddColInfo(_LS(IDS_WG_CMD__ADDD__Active),         t_integer, 0);
	AddColInfo(_LSX(E),                               t_integer, 0);  // PMS:XXXX-JWKWON-20091014 : 문자열 번역 안하는 걸로

	m_aPrimaryKey.Add(0);

	// MNET:1209, KJH20050203
#if defined(_CIVIL)
	CString csErrMsg(_T(""));
	m_bElastStag = CLoadDgnCodeChecker::CheckCodeAndErrorMsgST(CLoadDgnCodeChecker::eLCBConcrete,
																														 _LSX(JTG D60-04), csErrMsg);
	m_bTB10002_1_05 = CLoadDgnCodeChecker::CheckCodeAndErrorMsgST(CLoadDgnCodeChecker::eLCBConcrete,
																														 _LSX(TB 10002.1-05), csErrMsg);
#else
	m_bElastStag = FALSE;
	m_bTB10002_1_05 = FALSE;
#endif
	// End KJH20050203
}

CRatingLoadCombMainGrid::~CRatingLoadCombMainGrid()
{
}

#define CWnd CTBCommon
BEGIN_MESSAGE_MAP(CRatingLoadCombMainGrid, CWnd)
#undef CWnd
	//{{AFX_MSG_MAP(CRatingLoadCombMainGrid)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CRatingLoadCombMainGrid message handlers
void CRatingLoadCombMainGrid::Initialize(CDBDoc* pDoc, T_UNIT_SYSTEM &UnitSystem)
{
	CTBCommon::Initialize(pDoc, UnitSystem);

	LockUpdate(TRUE);

	// 컬럼 갯수 설정
	SetColCount(GetInitialColCount());
	SetHeaderTitle(UnitSystem, TRUE);

	// 표준 스타일 변경
	ChangeStandardStyle(CGXStyle()
			.SetFont(CGXFont_GC().SetSize(10))
			.SetVerticalAlignment(DT_BOTTOM)
			.SetAllowEnter(FALSE));

	// 컬럼 스타일 변경
	SetStyleRange(CGXRange().SetCols(3, 5), CGXStyle()
			.SetHorizontalAlignment(DT_LEFT)
			.SetValueType(GX_VT_STRING));
	SetFrozenCols(2, 2);
	//HideCols(1, 1);
	HideColsByIntend(1, 1, TRUE);

	// Serial No column의 스타일 변경
	SetStyleRange(CGXRange().SetCols(1, 2), CGXStyle()
			.SetHorizontalAlignment(DT_RIGHT)
			.SetValueType(GX_VT_NUMERIC)
			.SetFormat(GX_FMT_FIXED)
			.SetPlaces(0)
			.SetEnabled(FALSE));

//   CString TypeList = GetLcomTypeList(m_nLcomType);
// 
//   SetStyleRange(CGXRange().SetCols(4), CGXStyle()
//       .SetControl(GX_IDS_CTRL_TABBED_COMBOBOX)
//       .SetChoiceList(TypeList)
//       .SetUserAttribute(GX_IDS_UA_TABLIST_KEYCOL, _T("0"))
//       .SetUserAttribute(GX_IDS_UA_TABLIST_TEXTCOL, _T("0"))
//       .SetUserAttribute(GX_IDS_UA_TABLIST_SHOWALLCOLS, _T("1"))
//       .SetValueType(GX_VT_STRING)
//       .SetHorizontalAlignment(DT_LEFT)
//       .SetValue(_T(""))
//       );

	//SetStyleRange(CGXRange().SetCols(6), CGXStyle()
	//    .SetControl(GX_IDS_CTRL_CHECKBOX3D)
	//    .SetHorizontalAlignment(DT_CENTER)
	//    );
//   CString csChoiceList = GetActiveChoiceList(m_nLcomType);
//   SetStyleRange(CGXRange().SetCols(6), CGXStyle()
//       .SetControl(GX_IDS_CTRL_TABBED_COMBOBOX)
//       .SetChoiceList(csChoiceList)
//       .SetUserAttribute(GX_IDS_UA_TABLIST_KEYCOL, _T("0"))
//       .SetUserAttribute(GX_IDS_UA_TABLIST_TEXTCOL, _T("0"))
//       .SetUserAttribute(GX_IDS_UA_TABLIST_SHOWALLCOLS, _T("1"))
//       .SetValueType(GX_VT_STRING)
//       .SetHorizontalAlignment(DT_LEFT)
//       .SetValue(_T(""))
//       );

	// MNET1209, KJH20050204, JTG D60-04 only
	SetStyleRange(CGXRange().SetCols(7), CGXStyle()
			.SetControl(GX_IDS_CTRL_CHECKBOX3D)
			.SetValueType(GX_VT_NUMERIC)
			.SetHorizontalAlignment(DT_CENTER)
			.SetVerticalAlignment(DT_VCENTER)
			.SetValue(_T("0"))
			);

	// 현재 셀의 border 설정 변경
	CGXProperties* pProp = GetParam()->GetProperties();
			pProp->SetUserProperty(GX_IDS_OUTLINECURRENTCELL,
		      (CGXStyle) pProp->sInvertNoBorder);

	// 현재 셀 설정
	SetRightValidCell(1, 0);
	GetParam()->SetHideCurrentCell(GX_HIDE_NEVER);
	
	// Sorting 정보 초기화
	m_aSortInfo.SetSize(2);
	m_aSortInfo[0].nRC = 1; // Serial No
	m_aSortInfo[0].sortType = CGXSortInfo::numeric;
	m_aSortInfo[0].sortOrder = CGXSortInfo::ascending;
	m_aSortInfo[1].nRC = 0; // key
	m_aSortInfo[1].sortType = CGXSortInfo::numeric;
	m_aSortInfo[1].sortOrder = CGXSortInfo::ascending;

	// always show vertical scrollbar
	SetScrollBarMode(SB_VERT, gxnEnabled | gxnEnhanced);  

	// Row 이동을 가능하게 한다.
	GetParam()->EnableMoveRows(TRUE);

	// Insert key에 의한 삽입 금지
	EnableInsertRowByPressingKey(FALSE);

	// Column 크기 설정
	SetColWidthByDPI(1, 1, 0);
	SetColWidthByDPI(2, 2, 50);
	SetColWidthByDPI(3, 3, 58);
	SetColWidthByDPI(4, 4, 75);
	SetColWidthByDPI(5, 5, 190);
	SetColWidthByDPI(6, 6, 55);
	SetColWidthByDPI(7, 7, 25); // MNET:1209, KJH20050203

	MoveCols(6, 6, 4);

	// MNET:1209, KJH20050203
	MoveCols(7, 7, 6);
	//BOOL bElastHide = !m_bElastStag || m_nLcomType!=D_LCOMTYPE_CONCRETE;
	//HideColsByIntend(6, 6, bElastHide);
	// End KJH20050203

	MakeItemEx();


	//  Rating 관련 Col 빼고 숨김..	
	HideColsByIntend(2, 2, TRUE);
	HideColsByIntend(4, 6, TRUE);
	
	LockUpdate(FALSE);
}

BOOL CRatingLoadCombMainGrid::GetRowNoByKey(T_RLCS_K key, ROWCOL& nRow)
{
	ROWCOL nColKey;
	GetColumnNoFor(m_aTitle[0], nColKey);
	for(ROWCOL nRowLoop = 1; nRowLoop <= GetRowCount(); nRowLoop++)
	{
		const CString& szKey = GetValueRowCol(nRowLoop, nColKey);

		if(_ttoi(szKey) == key)
		{
			nRow = nRowLoop;
			return TRUE;
		}
	}
	return FALSE;
}

BOOL CRatingLoadCombMainGrid::GetInsertRowByKey(T_RLCS_K key, ROWCOL& nRow)
{
	LockUpdate(FALSE);
	Redraw();

	if(GetRowCount() == 1)
	{
		nRow = 1;
		return TRUE;
	}

	ROWCOL nColKey;
	T_RLCS_K EachKey;
	GetColumnNoFor(m_aTitle[0], nColKey);
    ROWCOL nRowLoop = 0;
	for(nRowLoop = 1; nRowLoop < GetRowCount(); nRowLoop++)
	{
		const CString& szKey = GetValueRowCol(nRowLoop, nColKey);
		
		EachKey = _ttoi(szKey);

		if(EachKey == key)
		{
			nRow = nRowLoop;
			return TRUE;  // Insert를 할려는데 이미 같은 Key가 있는 경우... Modify를 할 때 변경된놈을 Insert하고 기존껄 삭제하는 더러운 코드가 있어서 reutrn TRUE해줘야 함.
		}
		else if(EachKey > key)
		{
			nRow = nRowLoop;
			return TRUE;
		}
	}
	nRow = nRowLoop - 1;
	return TRUE;
}

BOOL CRatingLoadCombMainGrid::InsertRecordByKey(const T_RLCS_K &key, const T_RLCS_D &data)
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

BOOL CRatingLoadCombMainGrid::DeleteRecordByKey(const T_RLCS_K &key, const T_RLCS_D &data)
{
	ROWCOL nRow;

	if(!GetRowNoByKey(key, nRow))
	{
		ASSERT(0);
		return FALSE;
	}

	return DeleteRecord(nRow);
}

void CRatingLoadCombMainGrid::ChangeLcomType(int nLcomType)
{
	//m_nLcomType = nLcomType;

	ROWCOL nCol;
	GetColumnNoFor(3, nCol);

//   CString TypeList = GetLcomTypeList(m_nLcomType);
// 
//   SetStyleRange(CGXRange().SetCols(nCol), CGXStyle()
//       .SetControl(GX_IDS_CTRL_TABBED_COMBOBOX)
//       .SetChoiceList(TypeList)
//       .SetUserAttribute(GX_IDS_UA_TABLIST_KEYCOL, _T("0"))
//       .SetUserAttribute(GX_IDS_UA_TABLIST_TEXTCOL, _T("0"))
//       .SetUserAttribute(GX_IDS_UA_TABLIST_SHOWALLCOLS, _T("1"))
//       .SetValueType(GX_VT_STRING)
//       .SetHorizontalAlignment(DT_LEFT)
//       .SetValue(_T(""))
//       );  
// 
//   GetColumnNoFor(5, nCol);
//   CString csChoiceList = GetActiveChoiceList(m_nLcomType);
//   SetStyleRange(CGXRange().SetCols(nCol), CGXStyle()
//       .SetControl(GX_IDS_CTRL_TABBED_COMBOBOX)
//       .SetChoiceList(csChoiceList)
//       .SetUserAttribute(GX_IDS_UA_TABLIST_KEYCOL, _T("0"))
//       .SetUserAttribute(GX_IDS_UA_TABLIST_TEXTCOL, _T("0"))
//       .SetUserAttribute(GX_IDS_UA_TABLIST_SHOWALLCOLS, _T("1"))
//       .SetValueType(GX_VT_STRING)
//       .SetHorizontalAlignment(DT_LEFT)
//       .SetValue(_T(""))
//       );

 // m_aCurDefVal[4] = GetLcomActiveDef(m_nLcomType);

	// MNET:1209, KJH20050203
	//BOOL bElastHide = !m_bElastStag || m_nLcomType!=D_LCOMTYPE_CONCRETE;
	//GetColumnNoFor(6, nCol);
 // HideColsByIntend(nCol, nCol, bElastHide);
	// End KJH20050203
}

/////////////////////////////////////////////////////////////////////////////
// CRatingLoadCombMainGrid Overridable Functions
void CRatingLoadCombMainGrid::OnChangedSelectionPublic(long *keys, int nSize)
{
	if (nSize <= 0)
	{
		m_aSelectedKeys.RemoveAll();
	}
	else
	{
		m_aSelectedKeys.RemoveAll();
		for (int i = 0; i < nSize; i++) m_aSelectedKeys.Add((T_LCOM_K)keys[i]);
	}
}

long CRatingLoadCombMainGrid::GetSelectedKeys(CArray<long, long> &caKey)
{
	// override this function to return selected record count
	long nKeyCount;
	CArray<T_LCOM_K, T_LCOM_K> aSelKey;

	GetAllSelectedLcom(aSelKey);
	nKeyCount = aSelKey.GetSize();
	caKey.SetSize(nKeyCount);
	for (long i = 0; i < nKeyCount; i++)
		caKey[i] = (long)aSelKey[i];
	
	return nKeyCount;
}

BOOL CRatingLoadCombMainGrid::ValidateField(CString value, int nColID)
{
	if (nColID >= GetColCount())
	{
		//SetWarningText(_LS(IDS_WG_CMD__ADDD___CRatingLoadCombMainGrid__ValidateFie));
		SetWarningText(_T("Invalid date!"));
		return FALSE;
	}

	return TRUE;
}

// 삽입이나 수정전에 Record의 Validation을 검사한다.
BOOL CRatingLoadCombMainGrid::ValidateRecord(CStringArray &value, CRowColArray &aCols)
{
	CString msg = _T("");
	int nPos;

	// serial no, Description, Active, Elastic 검사 안한다.
	for (int i = 2; i < GetColCount()-3; i++)
	{
		if(i != 2 && i != 4) continue;

		nPos = aCols[i]-1;
		if (value[nPos] == _T(""))
			msg += ((msg != _T("")) ? _T("\n") : _T("")) + m_aTitle[i] + _LS(IDS_WG_CMD__ADDD___has_not_been_entered_);
	}
	for (int i = 0; i < m_pSubGrid->GetRowCount()-1; i++)
	{
		if (m_pSubGrid->GetValueRowCol(i+1, 1) == _T(""))
		{
			msg += ((msg != _T("")) ? _T("\n") : _T("")) + CString(_LS(IDS_WG_CMD__ADDD__Load_Case)) + _LS(IDS_WG_CMD__ADDD___has_not_been_entered_);
			break;
		}
		if (m_pSubGrid->GetValueRowCol(i+1, 2) == _T(""))
		{
			msg += ((msg != _T("")) ? _T("\n") : _T("")) + CString(_LS(IDS_WG_CMD__ADDD__Factor)) + _LS(IDS_WG_CMD__ADDD___has_not_been_entered_);
			break;
		}
	}

	for (int i = 0; i < m_pSubGridMV->GetRowCount()-1; i++)
	{
		if (m_pSubGridMV->GetValueRowCol(i+1, 1) == _T(""))
		{
			msg += ((msg != _T("")) ? _T("\n") : _T("")) + CString(_LS(IDS_WG_CMD__ADDD__Load_Case)) + _LS(IDS_WG_CMD__ADDD___has_not_been_entered_);
			break;
		}
		if (m_pSubGridMV->GetValueRowCol(i+1, 2) == _T(""))
		{
			msg += ((msg != _T("")) ? _T("\n") : _T("")) + CString(_LS(IDS_WG_CMD__ADDD__Factor)) + _LS(IDS_WG_CMD__ADDD___has_not_been_entered_);
			break;
		}
	}

	if (msg == _T("")) return TRUE;
	else SetWarningText(msg);
	return FALSE;
}

// 이 함수는 Paste할 때 호출되므로 False를 리턴할 때는 ValidateField와 
// Validate Record에서 설정된 Warning Text를 없애주고 리턴한다.
BOOL CRatingLoadCombMainGrid::ValidateAndMakeRecord(CStringArray &value, CRowColArray& aCols, T_RLCS_K &key, T_RLCS_D &data)
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

BOOL CRatingLoadCombMainGrid::AddToDB(ROWCOL nRow)
{
	// Record를 만들어서 DB에 입력한다.
	T_RLCS_K key;
	T_RLCS_D data;
	data.Initialize();
 
	GetValue(nRow, key, data);

	return m_pDoc->m_pDataCtrl->AddRlcs(key, data);
}

BOOL CRatingLoadCombMainGrid::AddToDB(CArray<void*, void*>& raKey, CArray<void*, void*>& raData)
{
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


BOOL CRatingLoadCombMainGrid::DeleteFromDB(ROWCOL nRow)
{
	T_RLCS_K key;
	T_RLCS_D data;
	data.Initialize();
 
	GetValue(nRow, key, data);
	return m_pDoc->m_pDataCtrl->DelRlcs(key);
}

BOOL CRatingLoadCombMainGrid::DeleteFromDB(CRowColArray& awRows)
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

BOOL CRatingLoadCombMainGrid::ModifyToDB(ROWCOL nRow)
{
	ROWCOL nCol;
	nCol = 1;

	T_RLCS_K key;
	T_RLCS_D data;
	data.Initialize();
	CString pOldLoadCombName;

	pOldLoadCombName = GetValueRowColBeforeEdit(nRow, nCol);
	GetValue(nRow, key, data);

	// 수정한다. 실패하면 에러 리턴
	return m_pDoc->m_pDataCtrl->ModifyRlcs(key, key, data); 
}

BOOL CRatingLoadCombMainGrid::ModifyToDB(CStringArray& raOldKey, 
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


CString CRatingLoadCombMainGrid::GetNewKey()
{  
	CString szTemp;
	szTemp.Format(_T("%d"), GetCountData() + 1);
	return szTemp;
}

CString CRatingLoadCombMainGrid::GetDefValue(int nIndex)
{
	return _T("");
	//return m_aCurDefVal[nIndex];
}

CString* CRatingLoadCombMainGrid::GetDefValueArray()
{
	return m_aCurDefVal;
}

BOOL CRatingLoadCombMainGrid::MakeKeyAndDataArray(CStringArray& value, CRowColArray& awCols, 
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

void CRatingLoadCombMainGrid::DeleteKeyAndDataArray(CArray<void*, void*>& aKey, CArray<void*, void*>& aData)
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

long CRatingLoadCombMainGrid::GetCountData()
{
	CDBDoc* pDoc = m_pDoc;
	T_RLCS_D tData;
	tData.Initialize();
	return pDoc->m_pAttrCtrl2->GetCountRlcs();
}

void CRatingLoadCombMainGrid::MakeItemFiltered(long nFilteredNum, long* aFilteredKeys, CRowColArray& awCols)
{
	ASSERT(0);
	/*
	CDBDoc* pDoc = m_pDoc;
	T_RLCS_D Data;

	for(long nCount=0 ; nCount < nFilteredNum; nCount++)
 	{
		pDoc->m_pAttrCtrl->GetLcom(m_nLcomType, aFilteredKeys[nCount], Data);
		SetValue(GetParam()->GetData(), nCount+1, aFilteredKeys[nCount], Data, awCols);
	}*/
}

void CRatingLoadCombMainGrid::MakeItemNormal(long nNumData, CRowColArray& awCols)
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

void CRatingLoadCombMainGrid::MakeItemEditingRecord(ROWCOL ncRow, CStringArray& EditingRecord)
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
// CRatingLoadCombMainGrid Implementation
BOOL CRatingLoadCombMainGrid::GetValue(ROWCOL nRow, T_RLCS_K &key, T_RLCS_D &data)
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
	m_pSubGrid->GetCombItem(data);
	m_pSubGridMV->GetCombItem(data);
	
	return TRUE;
}

/*
BOOL CRatingLoadCombMainGrid::OnAddNewRecord(ROWCOL nRow)
{
	if (m_pDoc == 0) return FALSE;
	
	BOOL bOldLock = LockUpdate(FALSE);
	// Record를 만들어서 DB에 입력한다.
	if (!AddToDB(nRow))
	{
		LockUpdate(bOldLock);
		return SetRecordValidation(FALSE);
	}
	
	// CancelEdit();   hslee : 이것때문에 추가가 안되서 주석처리.. 
	LockUpdate(bOldLock);
	if (!bOldLock) Redraw();
	GetBrowseParam()->m_nRecordCount -= 1;
	
	return SetRecordValidation(TRUE);
}
*/
BOOL CRatingLoadCombMainGrid::SetValue(ROWCOL nRow, const T_RLCS_K &key, const T_RLCS_D &data)
{
	//
	CRowColArray awCols;
	awCols.SetSize(GetColCount());
	for (int i = 0;i < GetColCount(); i++)
		VERIFY(GetColumnNoFor(m_aTitle[i], awCols[i]));
	return SetValue(GetParam()->GetData(), nRow, key, data, awCols);
}

BOOL CRatingLoadCombMainGrid::SetValue(CGXData* pData, ROWCOL nRow, const T_RLCS_K &key, const T_RLCS_D &data, CRowColArray &awCols)
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
/*
		// MNET:1209, KJH20050203
		if(i==5 && m_bElastStag && m_nLcomType==D_LCOMTYPE_CONCRETE)
		{
			BOOL bEnable = str.Find(_LS(IDS_CMD_LCOM_SERVICE))>=0;
			ROWCOL nCol; GetColumnNoFor(6, nCol);
			SetStyleRange(CGXRange(nRow, nCol, nRow, nCol), CGXStyle().SetEnabled(bEnable));
		}/*/
		// END KJH20050203
	}

	return TRUE;
}

void CRatingLoadCombMainGrid::ConvStrToData(int i, CString& value, T_RLCS_D& data)
{
	//if (i==1) data.LoadCombId = _ttol(value);  // Serial No
	if (i==2) data.strCaseName = value; // name
	//else if (i==3) data.LoadCombType = GetCombType(value); // type
	else if (i==4) data.strDescription = value; // description
//  else if (i==5) data.nActive = GetActive(m_nLcomType, value);
//  else if (i==6) data.bElasticStage = (value==_T("1")); // MNET:1209, KJH20050203
}

void CRatingLoadCombMainGrid::ConvDataToStr(int i, const T_RLCS_D& data, CString& value)
{
	//if (i==1) value.Format(_T("%d"), data.LoadCombId);
	if (i==2) value = data.strCaseName;
//  else if (i==3) value = GetCombTypeStr(data.LoadCombType);
	else if (i==4) value = data.strDescription;
//  else if (i==5) value = GetActiveName(m_nLcomType, data.nActive);
//  else if (i==6) value = data.bElasticStage ? _T("1") : _T("0"); // MNET:1209, KJH20050203
}

void CRatingLoadCombMainGrid::GetAllSelectedLcom(CArray<T_RLCS_K, T_RLCS_K>& aSelKey)
{
	for (int i = 0; i < m_aSelectedKeys.GetSize(); i++)
	{
		aSelKey.Add(m_aSelectedKeys[i]);
	}
}

// ProcessKeys, OnRButtonDblClk, OnLButtonDown은 Current Record 변경시 Grid간의
// 동기화를 맞추기 위해 Override했다.
// Current Record 변경시 OnChangeCurrentRecord 함수가 불리도록 설계했다.
BOOL CRatingLoadCombMainGrid::ProcessKeys(CWnd *pSender, UINT nMessage, UINT nChar, UINT nRepCnt, UINT flags)
{
	CTBCommon::ProcessKeys(pSender, nMessage, nChar, nRepCnt, flags);

	CheckCurrentRecordChanged();
	return TRUE;
}

void CRatingLoadCombMainGrid::OnChangeCurrentRecord(ROWCOL nRow)
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

BOOL CRatingLoadCombMainGrid::OnCanceledModify()
{
	ROWCOL ncRow, ncCol;
	GetCurrentCell(ncRow, ncCol);
	OnChangeCurrentRecord(ncRow);
	return TRUE;
}

CString CRatingLoadCombMainGrid::GetKeyForRow(ROWCOL ncRow, ROWCOL nkCol)
{
	CString sKey;

	if (IsAppendRow(ncRow)) sKey = _LS(IDS_WG_CMD__ADDD__Append_Row);
	else if (m_nEditMode == addnew) sKey = _LS(IDS_WG_CMD__ADDD__New_Row);
	else if (m_nEditMode == edit) sKey = GetValueRowColBeforeEdit(ncRow, nkCol);
	else sKey = GetValueRowCol(ncRow, nkCol);

	return sKey;
}

// Current Record가 변경되었는지 검사
BOOL CRatingLoadCombMainGrid::CheckCurrentRecordChanged()
{
	ROWCOL ncRow, nkCol;
	CString sKey;
	GetCurrentCell(ncRow, nkCol);
	GetColumnNoFor(GetKeyColID(), nkCol);
	sKey = GetKeyForRow(ncRow, nkCol);
	if (m_pSubGrid->GetKey() != sKey || m_pSubGridMV->GetKey() != sKey) 
	{
		OnChangeCurrentRecord(ncRow);
		return TRUE;
	}
/*
	// MNET:1209, KJH20050203
	if(IsActiveCol(nkCol) && m_bElastStag && m_nLcomType==D_LCOMTYPE_CONCRETE)
	{
		CString str = GetValueRowCol(ncRow, nkCol);
		BOOL bEnable = str.Find(_LS(IDS_CMD_LCOM_SERVICE))>=0;
		VERIFY(GetColumnNoFor(6, nkCol));
		SetStyleRange(CGXRange(ncRow, nkCol, ncRow, nkCol), CGXStyle().SetEnabled(bEnable));
	}*/
	// END KJH20050203
	return FALSE;
}
BOOL CRatingLoadCombMainGrid::IsActiveCol(ROWCOL& nCol)
{
	ROWCOL nRow, nACol;

	GetCurrentCell(nRow, nCol);
	VERIFY(GetColumnNoFor(5, nACol));

	return nCol==nACol;
}
// TBBrowserWndEx::OnSelDragColsDrop 참조
// 부모를 먼저 호출하면 안 된다.
// 1, 2 모두 Frozen Cols로 설정되어 있으므로 이쪽으로 셀이 이동되거나 이 셀들이
// 이동되는 것을 금지한다.
// 주1 참조
BOOL CRatingLoadCombMainGrid::OnSelDragColsDrop(ROWCOL nFirstCol, ROWCOL nLastCol, ROWCOL nDestCol)
{
	// Frozen 되어 있는 column 위치에 이동되면 이동을 금지시킨다.
	//if (IsFrozenCol(nDestCol)) return FALSE;
	if (nDestCol == 1 || nFirstCol == 1) return FALSE;
	if (nDestCol == 2 || nFirstCol == 2) return FALSE;
	return CTBCommon::OnSelDragColsDrop(nFirstCol, nLastCol, nDestCol);
}

// LoadComb의 위치를 이동(Serial No가 변경)한다.
BOOL CRatingLoadCombMainGrid::OnSelDragRowsDrop(ROWCOL nFirstRow, ROWCOL nLastRow, ROWCOL nDestRow)
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

void CRatingLoadCombMainGrid::SetHeaderTitle(T_UNIT_SYSTEM &UnitSystem, BOOL bInit)
{
	SetHeaderTitle2((int*)m_aUnit.GetData(), bInit, TRUE);
}


int CRatingLoadCombMainGrid::GetCombType(CString &value)
{
	if (value == D_LCOM_CT_ADD) return 0;
	else if (value == D_LCOM_CT_ENVELOPE) return 1;
	else if (value == D_LCOM_CT_ABS) return 2;
	else if (value == D_LCOM_CT_SRSS) return 3;

	return 0;
}

CString CRatingLoadCombMainGrid::GetCombTypeStr(int nCombType)
{
	CString aCombTypeStr[] = { D_LCOM_CT_ADD, D_LCOM_CT_ENVELOPE, D_LCOM_CT_ABS, D_LCOM_CT_SRSS };
	if (nCombType >= 0 && nCombType <= 3) return aCombTypeStr[nCombType];
	else return _LS(IDS_WG_CMD__ADDD__Error);

}

CString CRatingLoadCombMainGrid::GetActiveChoiceList(int nLcomType)
{
	CString csChoiceList;
#if defined(_CIVIL)
	if(nLcomType == D_LCOMTYPE_GENERAL)
	{
		csChoiceList.Format(_T("%s\n%s\n"),
			_LS(IDS_CMD_LCOM_INACTIVE), _LS(IDS_CMD_LCOM_ACTIVE));
	}
	else
	{
		if(CProduct::IsMovingType(D_PRODUCT_MOVING_CH) && m_bTB10002_1_05)
		{
			if(nLcomType == D_LCOMTYPE_STEEL || nLcomType == D_LCOMTYPE_SRC || nLcomType == D_LCOMTYPE_STLCOMP)
			{
				csChoiceList.Format(_T("%s\n%s\n%s\n"),
					_LS(IDS_CMD_LCOM_INACTIVE), _LS(IDS_CMD_LCOM_STRENGTH), _LS(IDS_CMD_LCOM_SERVICE));
			}
			else if(nLcomType == D_LCOMTYPE_CONCRETE)
			{
				csChoiceList.Format(_T("%s\n%s\n%s\n%s\n%s\n%s\n"),
					_LS(IDS_CMD_LCOM_INACTIVE), _LS(IDS_CMD_LCOM_STRENGTH), _LS(IDS_CMD_LCOM_SERVICE),
//          _LS(_T("Main")), _LS(_T("Main & Addtional")), _LS(_T("Main & Special"))); //by maxiao 
					_LS(IDS_CMD_LCOM_TB10002_1_MAIN),_LS(IDS_CMD_LCOM_TB10002_1_MAINADDITION),_LS(IDS_CMD_LCOM_TB10002_1_MAINSPECIAL));

			}
		}
		else
		{
			csChoiceList.Format(_T("%s\n%s\n%s\n"),
				_LS(IDS_CMD_LCOM_INACTIVE), _LS(IDS_CMD_LCOM_STRENGTH), _LS(IDS_CMD_LCOM_SERVICE));
		}
	}
#elif defined(_MGEN)
	if(nLcomType == D_LCOMTYPE_GENERAL || nLcomType == D_LCOMTYPE_FDN)
	{
		csChoiceList.Format(_T("%s\n%s\n"),
			_LS(IDS_CMD_LCOM_INACTIVE), _LS(IDS_CMD_LCOM_ACTIVE));
	}
	else if(nLcomType == D_LCOMTYPE_STEEL || nLcomType == D_LCOMTYPE_ALUMINUM)
	{
		csChoiceList.Format(_T("%s\n%s\n%s\n%s\n%s\n"),
			_LS(IDS_CMD_LCOM_INACTIVE), 
			_LS(IDS_CMD_LCOM_STRENGTH), 
			_LS(IDS_CMD_LCOM_SERVICE),
			_LS(IDS_CMD_LCOM_SPECIAL), 
			_LS(IDS_CMD_LCOM_VERTICAL));
	}
	else if(nLcomType == D_LCOMTYPE_CONCRETE)
	{
		csChoiceList.Format(_T("%s\n%s\n%s\n%s\n%s\n"),
			_LS(IDS_CMD_LCOM_INACTIVE), 
			_LS(IDS_CMD_LCOM_STRENGTH), 
			_LS(IDS_CMD_LCOM_SERVICE),
			_LS(IDS_CMD_LCOM_SPECIAL), 
			_LS(IDS_CMD_LCOM_VERTICAL));
	}    
	else if( nLcomType == D_LCOMTYPE_SRC)
	{
		csChoiceList.Format(_T("%s\n%s\n%s\n%s\n%s\n"),
			_LS(IDS_CMD_LCOM_INACTIVE), 
			_LS(IDS_CMD_LCOM_STRENGTH), 
			_LS(IDS_CMD_LCOM_SERVICE), // by GAY. ('13.09.25). MQC:6480. Combo에 _T("Serviceability") 추가.
			_LS(IDS_CMD_LCOM_SPECIAL), 
			_LS(IDS_CMD_LCOM_VERTICAL));  
	}
#else
		csChoiceList.Format(_T("%s\n%s\n"),
			_LS(IDS_CMD_LCOM_INACTIVE), _LS(IDS_CMD_LCOM_ACTIVE));
#endif  
	return csChoiceList;
}

int CRatingLoadCombMainGrid::GetActive(int nLcomType, CString& value)
{
#if defined(_CIVIL)
	if(nLcomType == D_LCOMTYPE_GENERAL)
	{
		if(value == _LS(IDS_CMD_LCOM_INACTIVE)) return 0;
		else if(value == _LS(IDS_CMD_LCOM_ACTIVE)) return 1;  
	}
	else
	{
		if     (value == _LS(IDS_CMD_LCOM_INACTIVE)) return 0;
		else if(value == _LS(IDS_CMD_LCOM_STRENGTH)) return 1;
		else if(value == _LS(IDS_CMD_LCOM_SERVICE))  return 2;
		if(CProduct::IsMovingType(D_PRODUCT_MOVING_CH) && m_bTB10002_1_05 && nLcomType == D_LCOMTYPE_CONCRETE)
		{
			if (value == _LS(IDS_CMD_LCOM_TB10002_1_MAIN))  return 3;
			else if (value == _LS(IDS_CMD_LCOM_TB10002_1_MAINADDITION)) return 4;
			else if (value == _LS(IDS_CMD_LCOM_TB10002_1_MAINSPECIAL)) return 5;
		}
	}
#elif defined(_MGEN)
	if(nLcomType == D_LCOMTYPE_GENERAL || nLcomType == D_LCOMTYPE_FDN)
	{
		if(value == _LS(IDS_CMD_LCOM_INACTIVE)) return 0;
		else if(value == _LS(IDS_CMD_LCOM_ACTIVE)) return 1;  
	}
	else if(nLcomType == D_LCOMTYPE_STEEL || nLcomType == D_LCOMTYPE_ALUMINUM)
	{
		if(value == _LS(IDS_CMD_LCOM_INACTIVE)) return 0;
		else if(value == _LS(IDS_CMD_LCOM_STRENGTH)) return 1;
		else if(value == _LS(IDS_CMD_LCOM_SERVICE)) return 2;
		else if(value == _LS(IDS_CMD_LCOM_SPECIAL)) return 3;
		else if(value == _LS(IDS_CMD_LCOM_VERTICAL)) return 4;
	}

	else if(nLcomType == D_LCOMTYPE_CONCRETE)
	{
		if(value == _LS(IDS_CMD_LCOM_INACTIVE)) return 0;
		else if(value == _LS(IDS_CMD_LCOM_STRENGTH)) return 1;
		else if(value == _LS(IDS_CMD_LCOM_SERVICE)) return 2;
		else if(value == _LS(IDS_CMD_LCOM_SPECIAL)) return 3;
		else if(value == _LS(IDS_CMD_LCOM_VERTICAL)) return 4;
	}
	else if(nLcomType == D_LCOMTYPE_SRC)
	{
		if(value == _LS(IDS_CMD_LCOM_INACTIVE)) return 0;
		else if(value == _LS(IDS_CMD_LCOM_STRENGTH)) return 1;
		else if(value == _LS(IDS_CMD_LCOM_SERVICE)) return 2;  // by GAY. ('13.09.25). MQC:6480. Combo에 _T("Serviceability") 추가.
		else if(value == _LS(IDS_CMD_LCOM_SPECIAL)) return 3;
		else if(value == _LS(IDS_CMD_LCOM_VERTICAL)) return 4;
	}
#else
	if(value == _LS(IDS_CMD_LCOM_INACTIVE)) return 0;
	else if(value == _LS(IDS_CMD_LCOM_ACTIVE)) return 1;  
#endif
	return 0;
}

CString CRatingLoadCombMainGrid::GetActiveName(int nLcomType, int value)
{
#if defined(_CIVIL)
	if(nLcomType == D_LCOMTYPE_GENERAL)
	{
		if(value == 0) return _LS(IDS_CMD_LCOM_INACTIVE);
		else if(value == 1) return _LS(IDS_CMD_LCOM_ACTIVE);    
	}
	else //if(nLcomType == D_LCOMTYPE_CONCRETE || nLcomType == D_LCOMTYPE_STEEL || nLcomType == D_LCOMTYPE_SRC)
	{
		if     (value == 0) return _LS(IDS_CMD_LCOM_INACTIVE);
		else if(value == 1) return _LS(IDS_CMD_LCOM_STRENGTH);
		else if(value == 2) return _LS(IDS_CMD_LCOM_SERVICE);
		if(CProduct::IsMovingType(D_PRODUCT_MOVING_CH) && m_bTB10002_1_05 && nLcomType == D_LCOMTYPE_CONCRETE)
		{
			if (value == 3)  return _LS(IDS_CMD_LCOM_TB10002_1_MAIN);
			else if (value == 4) return _LS(IDS_CMD_LCOM_TB10002_1_MAINADDITION);
			else if (value == 5) return _LS(IDS_CMD_LCOM_TB10002_1_MAINSPECIAL);
		}
	}
#elif defined(_MGEN)
	if(nLcomType == D_LCOMTYPE_GENERAL  || nLcomType == D_LCOMTYPE_FDN)
	{
		if(value == 0) return _LS(IDS_CMD_LCOM_INACTIVE);
		else if(value == 1) return _LS(IDS_CMD_LCOM_ACTIVE);    
	}
	else if(nLcomType == D_LCOMTYPE_CONCRETE||nLcomType == D_LCOMTYPE_STEEL || nLcomType == D_LCOMTYPE_ALUMINUM)
	{
		if (value == 0) return _LS(IDS_CMD_LCOM_INACTIVE);
		else if(value == 1) return _LS(IDS_CMD_LCOM_STRENGTH);
		else if(value == 2) return _LS(IDS_CMD_LCOM_SERVICE);
		else if(value == 3) return _LS(IDS_CMD_LCOM_SPECIAL);
		else if(value == 4) return _LS(IDS_CMD_LCOM_VERTICAL);
	}
	else if(nLcomType == D_LCOMTYPE_SRC)
	{
		if (value == 0) return _LS(IDS_CMD_LCOM_INACTIVE);
		else if(value == 1) return _LS(IDS_CMD_LCOM_STRENGTH);
		else if(value == 2) return _LS(IDS_CMD_LCOM_SERVICE);  // by GAY. ('13.09.25). MQC:6480. Combo에 _T("Serviceability") 추가.
		else if(value == 3) return _LS(IDS_CMD_LCOM_SPECIAL);
		else if(value == 4) return _LS(IDS_CMD_LCOM_VERTICAL);
	}
#else
	if(value == 0) return _LS(IDS_CMD_LCOM_INACTIVE);
	else if(value == 1) return _LS(IDS_CMD_LCOM_ACTIVE);  
#endif 
	return _T("Error");
}

CString CRatingLoadCombMainGrid::GetLcomActiveDef(int nLcomType)
{
#if defined(_CIVIL) || defined(_MGEN)  
	if(nLcomType == D_LCOMTYPE_CONCRETE)  return _LS(IDS_CMD_LCOM_STRENGTH);  
	else return _LS(IDS_CMD_LCOM_ACTIVE);  
	ASSERT(0);  
#else  
	if(nLcomType == D_LCOMTYPE_CONCRETE)  return _LS(IDS_CMD_LCOM_ACTIVE);
	else return _LS(IDS_CMD_LCOM_ACTIVE);  
	ASSERT(0);  
#endif
	return _T("");
}

BOOL CRatingLoadCombMainGrid::OnPasteFromClipboard(const CGXRange &range)
{
	AfxMessageBox(_LS(IDS_CMD_DONT_SUPPORT_PASTE));
	return FALSE;
}

CString CRatingLoadCombMainGrid::GetLcomTypeList(int nLcomType)
{
	CString strLcomTypeList = _T("");
	if(nLcomType==D_LCOMTYPE_GENERAL)
		strLcomTypeList.Format(_T("%s\n%s\n%s\n%s\n"), D_LCOM_CT_ADD, D_LCOM_CT_ENVELOPE, D_LCOM_CT_ABS, D_LCOM_CT_SRSS);
	else
	{
		if(nLcomType!=D_LCOMTYPE_FDN)
		{
#if defined(_MGEN)
			strLcomTypeList.Format(_T("%s\n%s\n%s\n"), D_LCOM_CT_ADD, D_LCOM_CT_ENVELOPE, D_LCOM_CT_SRSS);
#elif defined(_CIVIL)
			if(CProduct::IsMovingType(D_PRODUCT_MOVING_CH) && nLcomType == D_LCOMTYPE_CONCRETE)
			{
				strLcomTypeList.Format(_T("%s\n%s\n%s\n"), D_LCOM_CT_ADD, D_LCOM_CT_ENVELOPE, D_LCOM_CT_SRSS);
			}
			else
			{
				strLcomTypeList.Format(_T("%s\n%s\n"), D_LCOM_CT_ADD, D_LCOM_CT_ENVELOPE);
			}
#else
#endif
		}
		else strLcomTypeList.Format(_T("%s\n"), D_LCOM_CT_ADD);
	}
	return strLcomTypeList;
}


void CRatingLoadCombMainGrid::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	ROWCOL ncRowBefore, ncRowAfter, ncCol;
	if (!GetCurrentCell(ncRowBefore, ncCol)) return;
	
	CTBCommon::OnLButtonDblClk(nFlags, point);
	
	if (!GetCurrentCell(ncRowAfter, ncCol)) return;
	if (ncRowBefore != ncRowAfter) OnChangeCurrentRecord(ncRowAfter);
}

// 이 함수는 No Column을 클릭했을 때 Row Header를 클릭한 것과 같은 효과를
// 내기 위해서이다. CTBBrowserWndEx::OnLButtonDown 참조
// 또한 sub grid와의 동기화를 수행한다.
void CRatingLoadCombMainGrid::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	// Serial No 칼럼이 선택되어졌는지 본다.
	ROWCOL nhRow, nhCol, nkCol;
	int ht = HitTest(point, &nhRow, &nhCol);
	if (ht != GX_NOHIT && ht != GX_VERTLINE)
	{
		GetColumnNoFor(1, nkCol);
		if (nhCol == nkCol && IsFrozenCol(nhCol)) // Serial No column이면
		{
			// Row header의 rectangle을 얻는다.
			CRect r = CalcRectFromRowColEx(nhRow, 0, nhRow, 0);  
			point.x = (r.left+r.right)/2;
		}
	}
	
	ROWCOL ncRowBefore, ncRowAfter, ncCol;
	if (!GetCurrentCell(ncRowBefore, ncCol)) return;
	
	CTBCommon::OnLButtonDown(nFlags, point);
	
	if (!GetCurrentCell(ncRowAfter, ncCol)) return;
	if (ncRowBefore != ncRowAfter) OnChangeCurrentRecord(ncRowAfter);
	return;
}

void CRatingLoadCombMainGrid::SetModeEdit()
{
	if (IsEdit() || m_bIgnoreSetEditMode) return;
	
	ROWCOL ncRow, ncCol;
	GetCurrentCell(ncRow, ncCol);
	if (IsAppendRow(ncRow)) 
	{
		AddNew(ncRow);
		OnChangeCurrentRecord(ncRow);
	}
	else Edit(ncRow);
}
