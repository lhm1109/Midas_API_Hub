// CMLoadCombMainGrid.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMLoadCombMainGrid.h"

#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_LoadDB.h"
#include "..\wg_db\wg_db_LoadDgnCodeCheck.h"

#include "CMLoadCombSubGrid.h"
#include "..\wg_base\wg_base_ServiceProvider.h"
#include "..\wg_base\XMultiLang.h"
#include "..\wg_tb\Tb_ServiceDef.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define NAME_COL_ID         2
#define LCOM_KEY_FMT        _T("%d")
#define D_TYPE_DEF  D_LCOM_CT_ADD

/////////////////////////////////////////////////////////////////////////////
// CCMLoadCombMainGrid
CString CCMLoadCombMainGrid::m_aCurDefVal[] = {_T(""), _T(""), _T(""), _T(""), _T(""), _T("0")};
BOOL CCMLoadCombMainGrid::m_bElastStag = FALSE;
BOOL CCMLoadCombMainGrid::m_bTB10002_1_05 = FALSE;
CCMLoadCombMainGrid::CCMLoadCombMainGrid()
{
	CString DefVal[] = {_T(""), _T(""), D_TYPE_DEF, _T(""), _T(""), _T("0")};
	for(int i=0; i<sizeof(DefVal)/sizeof(CString); i++)	m_aCurDefVal[i] = DefVal[i];

	m_nLcomType = D_LCOMTYPE_GENERAL;
	m_pSubGrid = 0;

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

CCMLoadCombMainGrid::~CCMLoadCombMainGrid()
{
}

#define CWnd CTBCommon
BEGIN_MESSAGE_MAP(CCMLoadCombMainGrid, CWnd)
#undef CWnd
	//{{AFX_MSG_MAP(CCMLoadCombMainGrid)
	ON_WM_RBUTTONDBLCLK()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_RBUTTONDOWN()
	ON_COMMAND(ID_CMD_SMART_REPORT_TABLE, OnDynamicReportTable)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CCMLoadCombMainGrid message handlers
void CCMLoadCombMainGrid::Initialize(CDBDoc* pDoc, T_UNIT_SYSTEM &UnitSystem)
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

	CString TypeList = GetLcomTypeList(m_nLcomType);

	SetStyleRange(CGXRange().SetCols(4), CGXStyle()
			.SetControl(GX_IDS_CTRL_TABBED_COMBOBOX)
			.SetChoiceList(TypeList)
			.SetUserAttribute(GX_IDS_UA_TABLIST_KEYCOL, _T("0"))
			.SetUserAttribute(GX_IDS_UA_TABLIST_TEXTCOL, _T("0"))
			.SetUserAttribute(GX_IDS_UA_TABLIST_SHOWALLCOLS, _T("1"))
			.SetValueType(GX_VT_STRING)
			.SetHorizontalAlignment(DT_LEFT)
			.SetValue(_T(""))
			);

	//SetStyleRange(CGXRange().SetCols(6), CGXStyle()
	//    .SetControl(GX_IDS_CTRL_CHECKBOX3D)
	//    .SetHorizontalAlignment(DT_CENTER)
	//    );
	CString csChoiceList = GetActiveChoiceList(m_nLcomType);
	SetStyleRange(CGXRange().SetCols(6), CGXStyle()
			.SetControl(GX_IDS_CTRL_TABBED_COMBOBOX)
			.SetChoiceList(csChoiceList)
			.SetUserAttribute(GX_IDS_UA_TABLIST_KEYCOL, _T("0"))
			.SetUserAttribute(GX_IDS_UA_TABLIST_TEXTCOL, _T("0"))
			.SetUserAttribute(GX_IDS_UA_TABLIST_SHOWALLCOLS, _T("1"))
			.SetValueType(GX_VT_STRING)
			.SetHorizontalAlignment(DT_LEFT)
			.SetValue(_T(""))
			);

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
	int nColWidth[] = { 0, 35, 135, 75, 35, 60, 25 };
	for(int i=1; i<=7; i++)
		SetColWidthByDPI(i, i, nColWidth[i-1]);

	MoveCols(6, 6, 4);

	// MNET:1209, KJH20050203
	MoveCols(7, 7, 6);
	BOOL bElastHide = !m_bElastStag || m_nLcomType!=D_LCOMTYPE_CONCRETE;
	HideColsByIntend(6, 6, bElastHide);
	resizeElastColumn(bElastHide);
	// End KJH20050203

	MakeItemEx();
	LockUpdate(FALSE);
}

BOOL CCMLoadCombMainGrid::InsertRecordByKey(const T_LCOM_K &key, const T_LCOM_D &data)
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

BOOL CCMLoadCombMainGrid::DeleteRecordByKey(const T_LCOM_K &key, const T_LCOM_D &data)
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

void CCMLoadCombMainGrid::ChangeLcomType(int nLcomType)
{
	m_nLcomType = nLcomType;

	ROWCOL nCol;
	GetColumnNoFor(3, nCol);

	CString TypeList = GetLcomTypeList(m_nLcomType);

	SetStyleRange(CGXRange().SetCols(nCol), CGXStyle()
			.SetControl(GX_IDS_CTRL_TABBED_COMBOBOX)
			.SetChoiceList(TypeList)
			.SetUserAttribute(GX_IDS_UA_TABLIST_KEYCOL, _T("0"))
			.SetUserAttribute(GX_IDS_UA_TABLIST_TEXTCOL, _T("0"))
			.SetUserAttribute(GX_IDS_UA_TABLIST_SHOWALLCOLS, _T("1"))
			.SetValueType(GX_VT_STRING)
			.SetHorizontalAlignment(DT_LEFT)
			.SetValue(_T(""))
			);  

	GetColumnNoFor(5, nCol);
	CString csChoiceList = GetActiveChoiceList(m_nLcomType);
	SetStyleRange(CGXRange().SetCols(nCol), CGXStyle()
			.SetControl(GX_IDS_CTRL_TABBED_COMBOBOX)
			.SetChoiceList(csChoiceList)
			.SetUserAttribute(GX_IDS_UA_TABLIST_KEYCOL, _T("0"))
			.SetUserAttribute(GX_IDS_UA_TABLIST_TEXTCOL, _T("0"))
			.SetUserAttribute(GX_IDS_UA_TABLIST_SHOWALLCOLS, _T("1"))
			.SetValueType(GX_VT_STRING)
			.SetHorizontalAlignment(DT_LEFT)
			.SetValue(_T(""))
			);

	m_aCurDefVal[4] = GetLcomActiveDef(m_nLcomType);

	// MNET:1209, KJH20050203
	BOOL bElastHide = !m_bElastStag || m_nLcomType!=D_LCOMTYPE_CONCRETE;
	GetColumnNoFor(6, nCol);
	HideColsByIntend(nCol, nCol, bElastHide);
	resizeElastColumn(bElastHide);
	// End KJH20050203
}

/////////////////////////////////////////////////////////////////////////////
// CCMLoadCombMainGrid Overridable Functions
void CCMLoadCombMainGrid::OnChangedSelectionPublic(long *keys, int nSize)
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

long CCMLoadCombMainGrid::GetSelectedKeys(CArray<long, long> &caKey)
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

BOOL CCMLoadCombMainGrid::ValidateField(CString value, int nColID)
{
	if (nColID >= GetColCount())
	{
		SetWarningText(_LS(IDS_WG_CMD__ADDD___CCMLoadCombMainGrid__ValidateFie));
		return FALSE;
	}

	return TRUE;
}

// 삽입이나 수정전에 Record의 Validation을 검사한다.
BOOL CCMLoadCombMainGrid::ValidateRecord(CStringArray &value, CRowColArray &aCols)
{
	CString msg = _T("");
	int nPos;

	// serial no, Description, Active, Elastic 검사 안한다.
	for (int i = 2; i < GetColCount()-3; i++)
	{
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
	if (msg == _T("")) return TRUE;
	else SetWarningText(msg);
	return FALSE;
}

// 이 함수는 Paste할 때 호출되므로 False를 리턴할 때는 ValidateField와 
// Validate Record에서 설정된 Warning Text를 없애주고 리턴한다.
BOOL CCMLoadCombMainGrid::ValidateAndMakeRecord(CStringArray &value, CRowColArray& aCols, T_LCOM_K &key, T_LCOM_D &data)
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
	int nPos;
	for (int i = 0; i < GetColCount(); i++)
	{
		nPos = aCols[i]-1; // col id i에 해당하는 value가 저장된 위치
		if(i==0) 
		{
			if (value[nPos] == _T("")) key = 0;
			else key = _tstol(value[nPos]);
		}
		else ConvStrToData(i, value[nPos], data);
	}
	return TRUE;
}

BOOL CCMLoadCombMainGrid::AddToDB(ROWCOL nRow)
{
	// Record를 만들어서 DB에 입력한다.
	T_LCOM_K key;
	T_LCOM_D data;
 
	GetValue(nRow, key, data);

	return m_pDoc->m_pDataCtrl->AddLcom(m_nLcomType, data);
}

BOOL CCMLoadCombMainGrid::AddToDB(CArray<void*, void*>& raKey, CArray<void*, void*>& raData)
{
	ASSERT(raKey.GetSize() == raData.GetSize());
	T_LCOM_K tKey;
	CArray<T_LCOM_K, T_LCOM_K> aKey;
	CArray<T_LCOM_D, T_LCOM_D&> aData;
	for (int i = 0; i < raKey.GetSize(); i++)
	{
		tKey = *((T_LCOM_K*)raKey.GetAt(i));
		aKey.Add(tKey);
		aData.Add(*((T_LCOM_D*)raData.GetAt(i)));
	}
	return m_pDoc->m_pDataCtrl->AddLcom(m_nLcomType, aData);
}


BOOL CCMLoadCombMainGrid::DeleteFromDB(ROWCOL nRow)
{
	T_LCOM_K key;
	T_LCOM_D data;
 
	GetValue(nRow, key, data);
	return m_pDoc->m_pDataCtrl->DelLcom(m_nLcomType, data.LoadCombName);
}

BOOL CCMLoadCombMainGrid::DeleteFromDB(CRowColArray& awRows)
{
	if (awRows.GetSize() <= 0) return TRUE;

	// _LS(IDS_WG_CMD__ADDD__Name) 칼럼을 얻는다.
	ROWCOL nCol;
	VERIFY(GetColumnNoFor(NAME_COL_ID, nCol));

	CString value;
	CArray<CString, CString&> awKeys;
 
	for (int i = 0; i < awRows.GetSize(); i++)
	{
		value = GetValueRowColBeforeEdit(awRows.GetAt(i), nCol);
		awKeys.Add(value);
	}

	return m_pDoc->m_pDataCtrl->DelLcom(m_nLcomType, awKeys);
}

BOOL CCMLoadCombMainGrid::ModifyToDB(ROWCOL nRow)
{
	ROWCOL nCol;
	VERIFY(GetColumnNoFor(NAME_COL_ID, nCol));  // load case name

	T_LCOM_K key;
	T_LCOM_D data;
	CString pOldLoadCombName;

	pOldLoadCombName = GetValueRowColBeforeEdit(nRow, nCol);
	GetValue(nRow, key, data);

	// 수정한다. 실패하면 에러 리턴
	return m_pDoc->m_pDataCtrl->ModifyLcom(m_nLcomType, pOldLoadCombName, data); 
}

BOOL CCMLoadCombMainGrid::ModifyToDB(CStringArray& raOldKey, 
												 CArray<void*, void*>& raKey, 
												 CArray<void*, void*>& raData)
{
	ASSERT(raKey.GetSize() == raData.GetSize());
	ASSERT(raKey.GetSize() == raOldKey.GetSize());
	CArray<CString, CString&> aOldKey;
	CArray<T_LCOM_D, T_LCOM_D&> aData;
	T_LCOM_K Key;
	T_LCOM_D Data;
	for (int i = 0; i < raKey.GetSize(); i++)
	{
		Key = _tstol(raOldKey.GetAt(i));
		m_pDoc->m_pAttrCtrl->GetLcom(m_nLcomType, Key, Data);
		aOldKey.Add(Data.LoadCombName);
		aData.Add(*((T_LCOM_D*)raData.GetAt(i)));
	}
	return m_pDoc->m_pDataCtrl->ModifyLcom(m_nLcomType, aOldKey, aData);
}


CString CCMLoadCombMainGrid::GetNewKey()
{
	CString value;
	T_LCOM_K key = m_pDoc->m_pAttrCtrl->GetStartNumLcom(m_nLcomType);
	value.Format(LCOM_KEY_FMT, key);
	return value;
}

CString CCMLoadCombMainGrid::GetDefValue(int nIndex)
{
	return m_aCurDefVal[nIndex];
}

CString* CCMLoadCombMainGrid::GetDefValueArray()
{
	return m_aCurDefVal;
}

BOOL CCMLoadCombMainGrid::MakeKeyAndDataArray(CStringArray& value, CRowColArray& awCols, 
																	CArray<void*, void*>& aKey, CArray<void*, void*>& aData)
{
	T_LCOM_K* pKey;
	T_LCOM_D* pData;
	pKey = new T_LCOM_K;
	pData = new T_LCOM_D;
	aKey.Add((void*)pKey);
	aData.Add((void*)pData);
	if (!ValidateAndMakeRecord(value, awCols, *pKey, *pData)) return FALSE;
	return TRUE;
}

void CCMLoadCombMainGrid::DeleteKeyAndDataArray(CArray<void*, void*>& aKey, CArray<void*, void*>& aData)
{
	T_LCOM_K* pKey;
	T_LCOM_D* pData;
	ASSERT(aKey.GetSize() == aData.GetSize());
	for (int i = 0; i < aKey.GetSize(); i++)
	{
		pKey = (T_LCOM_K*)aKey.GetAt(i);
		pData = (T_LCOM_D*)aData.GetAt(i);
		delete pKey;
		delete pData;
	}
}

long CCMLoadCombMainGrid::GetCountData()
{
	CDBDoc* pDoc = m_pDoc;
	return pDoc->m_pAttrCtrl->GetCountLcom(m_nLcomType);
}

void CCMLoadCombMainGrid::MakeItemFiltered(long nFilteredNum, long* aFilteredKeys, CRowColArray& awCols)
{
	CDBDoc* pDoc = m_pDoc;
	T_LCOM_D Data;

	for(long nCount=0 ; nCount < nFilteredNum; nCount++)
 	{
		pDoc->m_pAttrCtrl->GetLcom(m_nLcomType, aFilteredKeys[nCount], Data);
		SetValue(GetParam()->GetData(), nCount+1, aFilteredKeys[nCount], Data, awCols);
	}
}

void CCMLoadCombMainGrid::MakeItemNormal(long nNumData, CRowColArray& awCols)
{
	T_LCOM_K Key;
	T_LCOM_D Data;
	CDBDoc* pDoc = m_pDoc;

	long* KeyBuf = new long[nNumData];
	long nCount = 0;
	POSITION pos = pDoc->m_pAttrCtrl->GetStartLcom(m_nLcomType);
	while(pos != NULL)
	{
	  pDoc->m_pAttrCtrl->GetNextLcom(m_nLcomType, pos,Key,Data);
	  KeyBuf[nCount++] = Key;
	}
	qsort(KeyBuf, nNumData, sizeof(T_LCOM_K), CTBBrowserWnd::CompareKey);

	for(nCount=0 ; nCount < nNumData; nCount++)
	{
		pDoc->m_pAttrCtrl->GetLcom(m_nLcomType, KeyBuf[nCount], Data);
		SetValue(GetParam()->GetData(), nCount+1, KeyBuf[nCount], Data, awCols);
	} // end of loop nCount

	delete []KeyBuf;
}

void CCMLoadCombMainGrid::MakeItemEditingRecord(ROWCOL ncRow, CStringArray& EditingRecord)
{
	CDBDoc* pDoc = m_pDoc;
	T_LCOM_K Key;
	T_LCOM_D Data;
	for (int i = 0; i < GetColCount(); i++)
	{
		if (i == 0) Key = _tstol(EditingRecord[i]);
		else ConvStrToData(i, EditingRecord[i], Data);
	}
	//pDoc->m_pUnitCtrl->ConvertUnitLcomPrevious(Data);	// load combination은 Unit과 관계없다.
	// 현재 값을 설정한다.
	SetValue(ncRow, Key, Data);
}

/////////////////////////////////////////////////////////////////////////////
// CCMLoadCombMainGrid Implementation
BOOL CCMLoadCombMainGrid::GetValue(ROWCOL nRow, T_LCOM_K &key, T_LCOM_D &data)
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
				key = m_pDoc->m_pAttrCtrl->GetStartNumLcom(m_nLcomType);
			else
				key = _tstol(str);
		}
		else ConvStrToData(i, str, data);
	}

	// MQC 8952 : nServLcomType 데이터를 추가시켜 준다.(PSC 경우)
	if(m_nLcomType==D_LCOMTYPE_CONCRETE)
	{
		T_LCOM_D LcomDTemp;
		if(!m_pDoc->m_pAttrCtrl->GetLcom(D_LCOMTYPE_CONCRETE, key, LcomDTemp)) LcomDTemp.Initialize();
		data.nServLcomType = LcomDTemp.nServLcomType;
	}
	//

	// Sub grid에 있는 값을 가져와서 설정한다.
	m_pSubGrid->GetCombItem(data);

	return TRUE;
}

BOOL CCMLoadCombMainGrid::SetValue(ROWCOL nRow, const T_LCOM_K &key, const T_LCOM_D &data)
{
	//
	CRowColArray awCols;
	awCols.SetSize(GetColCount());
	for (int i = 0;i < GetColCount(); i++)
		VERIFY(GetColumnNoFor(m_aTitle[i], awCols[i]));
	return SetValue(GetParam()->GetData(), nRow, key, data, awCols);
}

BOOL CCMLoadCombMainGrid::SetValue(CGXData* pData, ROWCOL nRow, const T_LCOM_K &key, const T_LCOM_D &data, CRowColArray &awCols)
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

		// MNET:1209, KJH20050203
		if(i==5 && m_bElastStag && m_nLcomType==D_LCOMTYPE_CONCRETE)
		{
			BOOL bEnable = str.Find(_LS(IDS_CMD_LCOM_SERVICE))>=0;
			ROWCOL nCol; GetColumnNoFor(6, nCol);
			SetStyleRange(CGXRange(nRow, nCol, nRow, nCol), CGXStyle().SetEnabled(bEnable));
		}
		// END KJH20050203
	}

	return TRUE;
}

void CCMLoadCombMainGrid::ConvStrToData(int i, CString& value, T_LCOM_D& data)
{
	if (i==1) data.LoadCombId = _tstol(value);  // Serial No
	if (i==2) data.LoadCombName = value; // name
	else if (i==3) data.LoadCombType = GetCombType(value); // type
	else if (i==4) data.Description = value; // description
	else if (i==5) data.nActive = GetActive(m_nLcomType, value);
	else if (i==6) data.bElasticStage = (value==_T("1")); // MNET:1209, KJH20050203
}

void CCMLoadCombMainGrid::ConvDataToStr(int i, const T_LCOM_D& data, CString& value)
{
	if (i==1) value.Format(_T("%d"), data.LoadCombId);
	if (i==2) value = data.LoadCombName;
	else if (i==3) value = GetCombTypeStr(data.LoadCombType);
	else if (i==4) value = data.Description;
	else if (i==5) value = GetActiveName(m_nLcomType, data.nActive);
	else if (i==6) value = data.bElasticStage ? _T("1") : _T("0"); // MNET:1209, KJH20050203
}

void CCMLoadCombMainGrid::MakeSearchKey(const T_LCOM_K &key, const T_LCOM_D &data, CStringArray &aKey)
{
	int nColID;
	int nSortKeyNum = m_aSortInfo.GetSize();

	aKey.SetSize(nSortKeyNum);
	for (int i = 0; i < nSortKeyNum; i++)
	{
		nColID = m_aSortInfo[i].nRC;
		if (nColID == 0) aKey[i].Format(LCOM_KEY_FMT, key);
		else ConvDataToStr(nColID, data, aKey[i]);
	}
}

void CCMLoadCombMainGrid::GetAllSelectedLcom(CArray<T_LCOM_K, T_LCOM_K>& aSelKey)
{
	for (int i = 0; i < m_aSelectedKeys.GetSize(); i++)
		aSelKey.Add(m_aSelectedKeys[i]);
}

// ProcessKeys, OnRButtonDblClk, OnLButtonDown은 Current Record 변경시 Grid간의
// 동기화를 맞추기 위해 Override했다.
// Current Record 변경시 OnChangeCurrentRecord 함수가 불리도록 설계했다.
BOOL CCMLoadCombMainGrid::ProcessKeys(CWnd *pSender, UINT nMessage, UINT nChar, UINT nRepCnt, UINT flags)
{
	CTBCommon::ProcessKeys(pSender, nMessage, nChar, nRepCnt, flags);

	CheckCurrentRecordChanged();
	return TRUE;
}

void CCMLoadCombMainGrid::OnRButtonDblClk(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	ROWCOL ncRowBefore, ncRowAfter, ncCol;
	if (!GetCurrentCell(ncRowBefore, ncCol)) return;

	CTBCommon::OnRButtonDblClk(nFlags, point);

	if (!GetCurrentCell(ncRowAfter, ncCol)) return;
	if (ncRowBefore != ncRowAfter) OnChangeCurrentRecord(ncRowAfter);
	return;
}

void CCMLoadCombMainGrid::OnLButtonDblClk(UINT nFlags, CPoint point) 
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
void CCMLoadCombMainGrid::OnLButtonDown(UINT nFlags, CPoint point) 
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

void CCMLoadCombMainGrid::OnChangeCurrentRecord(ROWCOL nRow)
{
	ROWCOL nkCol;
	GetColumnNoFor(GetKeyColID(), nkCol);
	CString sKey = GetKeyForRow(nRow, nkCol);
	m_pSubGrid->SetData(sKey);
}

BOOL CCMLoadCombMainGrid::OnCanceledModify()
{
	ROWCOL ncRow, ncCol;
	GetCurrentCell(ncRow, ncCol);
	OnChangeCurrentRecord(ncRow);
	return TRUE;
}

CString CCMLoadCombMainGrid::GetKeyForRow(ROWCOL ncRow, ROWCOL nkCol)
{
	CString sKey;

	if (IsAppendRow(ncRow)) sKey = _LS(IDS_WG_CMD__ADDD__Append_Row);
	else if (m_nEditMode == addnew) sKey = _LS(IDS_WG_CMD__ADDD__New_Row);
	else if (m_nEditMode == edit) sKey = GetValueRowColBeforeEdit(ncRow, nkCol);
	else sKey = GetValueRowCol(ncRow, nkCol);

	return sKey;
}

// Current Record가 변경되었는지 검사
BOOL CCMLoadCombMainGrid::CheckCurrentRecordChanged()
{
	ROWCOL ncRow, nkCol;
	CString sKey;
	GetCurrentCell(ncRow, nkCol);
	GetColumnNoFor(GetKeyColID(), nkCol);
	sKey = GetKeyForRow(ncRow, nkCol);
	if (m_pSubGrid->GetKey() != sKey) 
	{
		OnChangeCurrentRecord(ncRow);
		return TRUE;
	}

	// MNET:1209, KJH20050203
	if(IsActiveCol(nkCol) && m_bElastStag && m_nLcomType==D_LCOMTYPE_CONCRETE)
	{
		CString str = GetValueRowCol(ncRow, nkCol);
		BOOL bEnable = str.Find(_LS(IDS_CMD_LCOM_SERVICE))>=0;
		VERIFY(GetColumnNoFor(6, nkCol));
		SetStyleRange(CGXRange(ncRow, nkCol, ncRow, nkCol), CGXStyle().SetEnabled(bEnable));
	}
	// END KJH20050203
	return FALSE;
}
BOOL CCMLoadCombMainGrid::IsActiveCol(ROWCOL& nCol)
{
	ROWCOL nRow, nACol;

	GetCurrentCell(nRow, nCol);
	VERIFY(GetColumnNoFor(5, nACol));

	return nCol==nACol;
}

void CCMLoadCombMainGrid::resizeElastColumn(BOOL bElastHide)
{
	if (bElastHide)
	{
		SetColWidthByDPI(7, 7, 135);
	}
	else
	{
		SetColWidthByDPI(7, 7, 110);
	}
}
// TBBrowserWndEx::OnSelDragColsDrop 참조
// 부모를 먼저 호출하면 안 된다.
// 1, 2 모두 Frozen Cols로 설정되어 있으므로 이쪽으로 셀이 이동되거나 이 셀들이
// 이동되는 것을 금지한다.
// 주1 참조
BOOL CCMLoadCombMainGrid::OnSelDragColsDrop(ROWCOL nFirstCol, ROWCOL nLastCol, ROWCOL nDestCol)
{
	// Frozen 되어 있는 column 위치에 이동되면 이동을 금지시킨다.
	//if (IsFrozenCol(nDestCol)) return FALSE;
	if (nDestCol == 1 || nFirstCol == 1) return FALSE;
	if (nDestCol == 2 || nFirstCol == 2) return FALSE;
	return CTBCommon::OnSelDragColsDrop(nFirstCol, nLastCol, nDestCol);
}

// LoadComb의 위치를 이동(Serial No가 변경)한다.
BOOL CCMLoadCombMainGrid::OnSelDragRowsDrop(ROWCOL nFirstRow, ROWCOL nLastRow, ROWCOL nDestRow)
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
	CArray<T_LCOM_D, T_LCOM_D&> aData;
	CString LoadCombName;
	T_LCOM_K key;
	T_LCOM_D data;
	for (int i = nFirstRow; i <= nLastRow; i++)
	{
		GetValue(i, key, data);
		// Get Value만 하면 Combination이 현재 Record의 Combination으로 치환된다.
		// 여기서는 위치만 이동되므로 LoadCombId만 변경된다. 그래서 원래의 Combination
		// 을 유지하기 위해 GetLcom을 다시 호출한다.
		m_pDoc->m_pAttrCtrl->GetLcom(m_nLcomType, key, data);
		data.LoadCombId = nStart;
		nStart += nInc;

		aLoadCombName.Add(data.LoadCombName);
		aData.Add(data);
	}
	m_pDoc->m_pDataCtrl->ModifyLcom(m_nLcomType, aLoadCombName, aData);
	return FALSE;
}

void CCMLoadCombMainGrid::SetHeaderTitle(T_UNIT_SYSTEM &UnitSystem, BOOL bInit)
{
	SetHeaderTitle2((int*)m_aUnit.GetData(), bInit, TRUE);
}


int CCMLoadCombMainGrid::GetCombType(CString &value)
{
	if (value == D_LCOM_CT_ADD) return 0;
	else if (value == D_LCOM_CT_ENVELOPE) return 1;
	else if (value == D_LCOM_CT_ABS) return 2;
	else if (value == D_LCOM_CT_SRSS) return 3;
/*
	if (value == D_LCOM_CT_LINEAR) return 0;
	else if (value == D_LCOM_CT_PSRSS) return 1;
	else if (value == D_LCOM_CT_MSRSS) return 2;
*/
	return 0;
}

CString CCMLoadCombMainGrid::GetCombTypeStr(int nCombType)
{
	CString aCombTypeStr[] = { D_LCOM_CT_ADD, D_LCOM_CT_ENVELOPE, D_LCOM_CT_ABS, D_LCOM_CT_SRSS };
	if (nCombType >= 0 && nCombType <= 3) return aCombTypeStr[nCombType];
	else return _LS(IDS_WG_CMD__ADDD__Error);
/*
	CString aCombTypeStr[] = { D_LCOM_CT_LINEAR, D_LCOM_CT_PSRSS, D_LCOM_CT_MSRSS };
	if (nCombType >= 0 && nCombType <= 2) return aCombTypeStr[nCombType];
	else return _LS(IDS_WG_CMD__ADDD__Error);
*/
}

CString CCMLoadCombMainGrid::GetActiveChoiceList(int nLcomType)
{
	CString csChoiceList;
#if defined(_CIVIL)
	if(nLcomType == D_LCOMTYPE_GENERAL || nLcomType == D_LCOMTYPE_SEISMIC)
	{
		csChoiceList.Format(_T("%s\n%s\n"),
			_LS(IDS_CMD_LCOM_INACTIVE), _LS(IDS_CMD_LCOM_ACTIVE));
	}
	else
	{
		#if defined(_CH)
		if(m_bTB10002_1_05)
		{
			if(nLcomType == D_LCOMTYPE_STEEL)
			{
				csChoiceList.Format(_T("%s\n%s\n%s\n"),
					_LS(IDS_CMD_LCOM_INACTIVE), _LS(IDS_CMD_LCOM_STRENGTH), _LS(IDS_CMD_LCOM_SERVICE));
			}
			else if(nLcomType == D_LCOMTYPE_CONCRETE)
			{
				csChoiceList.Format(_T("%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n"),
					_LS(IDS_CMD_LCOM_INACTIVE), _LS(IDS_CMD_LCOM_STRENGTH), _LS(IDS_CMD_LCOM_SERVICE),
					//          _LS(_T("Main")), _LS(_T("Main & Addtional")), _LS(_T("Main & Special"))); //by maxiao 
					_LS(IDS_CMD_LCOM_TB10002_1_MAIN),_LS(IDS_CMD_LCOM_TB10002_1_MAINADDITION),_LS(IDS_CMD_LCOM_TB10002_1_MAINSPECIAL),
					_LS(IDS_CMD_LCOM_JTGD6015_ACCIDENT),_LS(IDS_CMD_LCOM_JTGD6015_FREQUENCY),_LS(IDS_CMD_LCOM_JTGD6015_QPERMNENT));//add by gongxing 20151023
			}
			else if (nLcomType == D_LCOMTYPE_SRC)
			{
				csChoiceList.Format(_T("%s\n%s\n%s\n%s\n%s\n%s\n"),
					_LS(IDS_CMD_LCOM_INACTIVE), 
					_LS(IDS_CMD_LCOM_STRENGTH), 
					_LS(IDS_CMD_LCOM_SERVICE),
					_LS(IDS_CMD_LCOM_JTGD6015_ACCIDENT),      //탉횔莉북
					_LS(IDS_CMD_LCOM_JTGD6015_FREQUENCY),     //틉島莉북
					_LS(IDS_CMD_LCOM_JTGD6015_QPERMNENT));    //硫湛씹莉북
			}
			else if (nLcomType == D_LCOMTYPE_STLCOMP)
			{
				csChoiceList.Format(_T("%s\n%s\n%s\n%s\n%s\n"),
					_LS(IDS_CMD_LCOM_INACTIVE),               //뛸뺏
					_LS(IDS_CMD_LCOM_STRENGTH),               //샘굶莉북
					_LS(IDS_CMD_LCOM_JTGD6015_ACCIDENT),      //탉횔莉북
					_LS(IDS_CMD_LCOM_JTGD6015_FREQUENCY),     //틉島莉북
					_LS(IDS_CMD_LCOM_JTGD6015_QPERMNENT));    //硫湛씹莉북
			}
		}
		else
		{
			csChoiceList.Format(_T("%s\n%s\n%s\n"),
				_LS(IDS_CMD_LCOM_INACTIVE), _LS(IDS_CMD_LCOM_STRENGTH), _LS(IDS_CMD_LCOM_SERVICE));
		}
	#else
		csChoiceList.Format(_T("%s\n%s\n%s\n"),
			_LS(IDS_CMD_LCOM_INACTIVE), _LS(IDS_CMD_LCOM_STRENGTH), _LS(IDS_CMD_LCOM_SERVICE));
	#endif    

	}
#elif defined(_MGEN)
	if(nLcomType == D_LCOMTYPE_GENERAL || nLcomType == D_LCOMTYPE_FDN)
	{
		csChoiceList.Format(_T("%s\n%s\n"),
			_LS(IDS_CMD_LCOM_INACTIVE), _LS(IDS_CMD_LCOM_ACTIVE));
	}
	else if(nLcomType == D_LCOMTYPE_STEEL)
	{
		csChoiceList.Format(_T("%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n"),
			_LS(IDS_CMD_LCOM_INACTIVE), 
			_LS(IDS_CMD_LCOM_STRENGTH), 
			_LS(IDS_CMD_LCOM_SERVICE),
			_LS(IDS_CMD_LCOM_SPECIAL), 
			_LS(IDS_CMD_LCOM_VERTICAL),
			_LS(IDS_CMD_LCOM_STRENGTH_ELASTIC),
			_LS(IDS_CMD_LCOM_UG_STRENGTH),
			_LS(IDS_CMD_LCOM_UG_SERVICE),
			_LS(IDS_CMD_LCOM_UG_SPECIAL));
	}
	else if (nLcomType == D_LCOMTYPE_ALUMINUM)
	{
		csChoiceList.Format(_T("%s\n%s\n%s\n%s\n%s\n%s\n"),
			_LS(IDS_CMD_LCOM_INACTIVE),
			_LS(IDS_CMD_LCOM_STRENGTH),
			_LS(IDS_CMD_LCOM_SERVICE),
			_LS(IDS_CMD_LCOM_SPECIAL),
			_LS(IDS_CMD_LCOM_VERTICAL),
			_LS(IDS_CMD_LCOM_STRENGTH_ELASTIC));
	}
	else if(nLcomType == D_LCOMTYPE_CONCRETE)
	{
		csChoiceList.Format(_T("%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n"),
			_LS(IDS_CMD_LCOM_INACTIVE), 
			_LS(IDS_CMD_LCOM_STRENGTH), 
			_LS(IDS_CMD_LCOM_SERVICE),
			_LS(IDS_CMD_LCOM_SPECIAL), 
			_LS(IDS_CMD_LCOM_VERTICAL),
			_LS(IDS_CMD_LCOM_STRENGTH_ELASTIC),
			_LS(IDS_CMD_LCOM_UG_STRENGTH),
			_LS(IDS_CMD_LCOM_UG_SERVICE),
			_LS(IDS_CMD_LCOM_UG_SPECIAL));
	}    
	else if( nLcomType == D_LCOMTYPE_SRC)
	{
		csChoiceList.Format(_T("%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n"),
			_LS(IDS_CMD_LCOM_INACTIVE), 
			_LS(IDS_CMD_LCOM_STRENGTH), 
			_LS(IDS_CMD_LCOM_SERVICE), // by GAY. ('13.09.25). MQC:6480. Combo에 _T("Serviceability") 추가.
			_LS(IDS_CMD_LCOM_SPECIAL), 
			_LS(IDS_CMD_LCOM_VERTICAL),
			_LS(IDS_CMD_LCOM_STRENGTH_ELASTIC),
			_LS(IDS_CMD_LCOM_UG_STRENGTH),
			_LS(IDS_CMD_LCOM_UG_SERVICE),
			_LS(IDS_CMD_LCOM_UG_SPECIAL));
	}
	else if(nLcomType == D_LCOMTYPE_CFSTEEL)
	{
		csChoiceList.Format(_T("%s\n%s\n%s\n"),
			_LS(IDS_CMD_LCOM_INACTIVE), 
			_LS(IDS_CMD_LCOM_STRENGTH), 
			_LS(IDS_CMD_LCOM_SERVICE));
	}
	else if(nLcomType == D_LCOMTYPE_SEISMIC)
	{
		csChoiceList.Format(_T("%s\n%s\n%s\n"),
			_LS(IDS_CMD_LCOM_INACTIVE), 
			_LS(IDS_CMD_LCOM_GENERAL), 
			_LS(IDS_CMD_LCOM_VERTICAL));
	}
	else if(nLcomType == D_LCOMTYPE_LINEAR_EVAL)
	{
		csChoiceList.Format(_T("%s\n%s\n%s\n%s\n%s\n"),
			_LS(IDS_CMD_LCOM_INACTIVE),
			_LS(IDS_CMD_LCOM_GRAVITY),
			_LS(IDS_CMD_LCOM_SEISMIC),
			_LS(IDS_CMD_LCOM_SPECIAL),
			_LS(IDS_CMD_LCOM_VERTICAL));
	}
#else
		csChoiceList.Format(_T("%s\n%s\n"),
			_LS(IDS_CMD_LCOM_INACTIVE), _LS(IDS_CMD_LCOM_ACTIVE));
#endif  
	return csChoiceList;
}

int CCMLoadCombMainGrid::GetActive(int nLcomType, CString& value)
{
#if defined(_CIVIL)
	if(nLcomType == D_LCOMTYPE_GENERAL || nLcomType == D_LCOMTYPE_SEISMIC)
	{
		if(value == _LS(IDS_CMD_LCOM_INACTIVE)) return 0;
		else if(value == _LS(IDS_CMD_LCOM_ACTIVE)) return 1;  
	}
	else
	{
		if     (value == _LS(IDS_CMD_LCOM_INACTIVE)) return 0;
		else if(value == _LS(IDS_CMD_LCOM_STRENGTH)) return 1;//샘굶莉북
		else if(value == _LS(IDS_CMD_LCOM_SERVICE))  return 2;//깃硫莉북
	#if defined(_CH)
		if(m_bTB10002_1_05 && nLcomType == D_LCOMTYPE_CONCRETE)
		{
			if     (value == _LS(IDS_CMD_LCOM_TB10002_1_MAIN))  return 3;
			else if(value == _LS(IDS_CMD_LCOM_TB10002_1_MAINADDITION)) return 4;
			else if(value == _LS(IDS_CMD_LCOM_TB10002_1_MAINSPECIAL)) return 5;
			else if(value == _LS(IDS_CMD_LCOM_JTGD6015_ACCIDENT))   return 6;//탉횔莉북
			else if(value == _LS(IDS_CMD_LCOM_JTGD6015_FREQUENCY))  return 7;//틉島莉북
			else if(value == _LS(IDS_CMD_LCOM_JTGD6015_QPERMNENT))  return 8;//硫湛씹莉북
		}
		else if(nLcomType == D_LCOMTYPE_STLCOMP || nLcomType == D_LCOMTYPE_SRC)
		{
			if(value == _LS(IDS_CMD_LCOM_JTGD6015_ACCIDENT))        return 6;//탉횔莉북
			else if(value == _LS(IDS_CMD_LCOM_JTGD6015_FREQUENCY))  return 7;//틉島莉북
			else if(value == _LS(IDS_CMD_LCOM_JTGD6015_QPERMNENT))  return 8;//硫湛씹莉북
		}
	#endif
	}
#elif defined(_MGEN)
	if(nLcomType == D_LCOMTYPE_GENERAL || nLcomType == D_LCOMTYPE_FDN)
	{
		if(value == _LS(IDS_CMD_LCOM_INACTIVE)) return 0;
		else if(value == _LS(IDS_CMD_LCOM_ACTIVE)) return 1;  
	}
	else if(nLcomType == D_LCOMTYPE_STEEL)
	{
		if(value == _LS(IDS_CMD_LCOM_INACTIVE)) return 0;
		else if(value == _LS(IDS_CMD_LCOM_STRENGTH)) return 1;
		else if(value == _LS(IDS_CMD_LCOM_SERVICE)) return 2;
		else if(value == _LS(IDS_CMD_LCOM_SPECIAL)) return 3;
		else if(value == _LS(IDS_CMD_LCOM_VERTICAL)) return 4;
		else if(value == _LS(IDS_CMD_LCOM_STRENGTH_ELASTIC)) return 5;
		else if (value == _LS(IDS_CMD_LCOM_UG_STRENGTH)) return EN_DGN_UG_STRN;
		else if (value == _LS(IDS_CMD_LCOM_UG_SERVICE)) return EN_DGN_UG_SERV;
		else if (value == _LS(IDS_CMD_LCOM_UG_SPECIAL)) return EN_DGN_UG_SPEC;
	}
	else if (nLcomType == D_LCOMTYPE_ALUMINUM)
	{
		if (value == _LS(IDS_CMD_LCOM_INACTIVE)) return 0;
		else if(value == _LS(IDS_CMD_LCOM_STRENGTH)) return 1;
		else if(value == _LS(IDS_CMD_LCOM_SERVICE)) return 2;
		else if(value == _LS(IDS_CMD_LCOM_SPECIAL)) return 3;
		else if(value == _LS(IDS_CMD_LCOM_VERTICAL)) return 4;
		else if(value == _LS(IDS_CMD_LCOM_STRENGTH_ELASTIC)) return 5;
	}

	else if(nLcomType == D_LCOMTYPE_CONCRETE)
	{
		if(value == _LS(IDS_CMD_LCOM_INACTIVE)) return 0;
		else if(value == _LS(IDS_CMD_LCOM_STRENGTH)) return 1;
		else if(value == _LS(IDS_CMD_LCOM_SERVICE)) return 2;
		else if(value == _LS(IDS_CMD_LCOM_SPECIAL)) return 3;
		else if(value == _LS(IDS_CMD_LCOM_VERTICAL)) return 4;
		else if(value == _LS(IDS_CMD_LCOM_STRENGTH_ELASTIC)) return 5;
		else if (value == _LS(IDS_CMD_LCOM_UG_STRENGTH)) return EN_DGN_UG_STRN;
		else if (value == _LS(IDS_CMD_LCOM_UG_SERVICE)) return EN_DGN_UG_SERV;
		else if (value == _LS(IDS_CMD_LCOM_UG_SPECIAL)) return EN_DGN_UG_SPEC;
	}
	else if(nLcomType == D_LCOMTYPE_SRC)
	{
		if(value == _LS(IDS_CMD_LCOM_INACTIVE)) return 0;
		else if(value == _LS(IDS_CMD_LCOM_STRENGTH)) return 1;
		else if(value == _LS(IDS_CMD_LCOM_SERVICE)) return 2;  // by GAY. ('13.09.25). MQC:6480. Combo에 _T("Serviceability") 추가.
		else if(value == _LS(IDS_CMD_LCOM_SPECIAL)) return 3;
		else if(value == _LS(IDS_CMD_LCOM_VERTICAL)) return 4;
		else if(value == _LS(IDS_CMD_LCOM_STRENGTH_ELASTIC)) return 5;
		else if(value == _LS(IDS_CMD_LCOM_UG_STRENGTH)) return EN_DGN_UG_STRN;
		else if(value == _LS(IDS_CMD_LCOM_UG_SERVICE)) return EN_DGN_UG_SERV;
		else if(value == _LS(IDS_CMD_LCOM_UG_SPECIAL)) return EN_DGN_UG_SPEC;
	}
	else if(nLcomType == D_LCOMTYPE_CFSTEEL)
	{
		if(value == _LS(IDS_CMD_LCOM_INACTIVE)) return 0;
		else if(value == _LS(IDS_CMD_LCOM_STRENGTH)) return 1;
		else if(value == _LS(IDS_CMD_LCOM_SERVICE)) return 2;
	}
	else if(nLcomType == D_LCOMTYPE_SEISMIC)
	{
		if     (value == _LS(IDS_CMD_LCOM_INACTIVE)) return 0;
		else if(value == _LS(IDS_CMD_LCOM_GENERAL))  return 1;
		else if(value == _LS(IDS_CMD_LCOM_VERTICAL)) return 2;
	}
	else if(nLcomType == D_LCOMTYPE_LINEAR_EVAL)
	{
				 if( value == _LS(IDS_CMD_LCOM_INACTIVE))  return 0;
	  else if( value == _LS(IDS_CMD_LCOM_GRAVITY) )  return 1;
	  else if( value == _LS(IDS_CMD_LCOM_SEISMIC) )  return 2;
		else if( value == _LS(IDS_CMD_LCOM_SPECIAL) )  return 3;
		else if( value == _LS(IDS_CMD_LCOM_VERTICAL))  return 4;
	}
#else
	if(value == _LS(IDS_CMD_LCOM_INACTIVE)) return 0;
	else if(value == _LS(IDS_CMD_LCOM_ACTIVE)) return 1;  
#endif
	return 0;
}

CString CCMLoadCombMainGrid::GetActiveName(int nLcomType, int value)
{
#if defined(_CIVIL)
	if(nLcomType == D_LCOMTYPE_GENERAL || nLcomType == D_LCOMTYPE_SEISMIC)
	{
		if(value == 0) return _LS(IDS_CMD_LCOM_INACTIVE);
		else if(value == 1) return _LS(IDS_CMD_LCOM_ACTIVE);    
	}
	else //if(nLcomType == D_LCOMTYPE_CONCRETE || nLcomType == D_LCOMTYPE_STEEL || nLcomType == D_LCOMTYPE_SRC)
	{
		if     (value == 0) return _LS(IDS_CMD_LCOM_INACTIVE);//뛸뺏
		else if(value == 1) return _LS(IDS_CMD_LCOM_STRENGTH);//샘굶莉북
		else if(value == 2) return _LS(IDS_CMD_LCOM_SERVICE);//깃硫莉북 
	#if defined(_CH)
		if(m_bTB10002_1_05 && nLcomType == D_LCOMTYPE_CONCRETE)
		{
			if (value == 3)  return _LS(IDS_CMD_LCOM_TB10002_1_MAIN);
			else if (value == 4) return _LS(IDS_CMD_LCOM_TB10002_1_MAINADDITION);
			else if (value == 5) return _LS(IDS_CMD_LCOM_TB10002_1_MAINSPECIAL);
			else if(value == 6) return _LS(IDS_CMD_LCOM_JTGD6015_ACCIDENT);//탉횔莉북
			else if(value == 7) return _LS(IDS_CMD_LCOM_JTGD6015_FREQUENCY);//틉島莉북
			else if(value == 8) return _LS(IDS_CMD_LCOM_JTGD6015_QPERMNENT);//硫湛씹莉북
		}
		else if (nLcomType == D_LCOMTYPE_STLCOMP || nLcomType == D_LCOMTYPE_SRC)
		{
			if(value == 6)      return _LS(IDS_CMD_LCOM_JTGD6015_ACCIDENT);//탉횔莉북
			else if(value == 7) return _LS(IDS_CMD_LCOM_JTGD6015_FREQUENCY);//틉島莉북
			else if(value == 8) return _LS(IDS_CMD_LCOM_JTGD6015_QPERMNENT);//硫湛씹莉북
		}
	#else 
		if(m_bTB10002_1_05 && nLcomType == D_LCOMTYPE_CONCRETE)
		{
			if(value == 3 || value == 4 || value == 5 || value == 6)
			{
				return _LS(IDS_CMD_LCOM_STRENGTH);
			}
			else
			{
				return _LS(IDS_CMD_LCOM_SERVICE);
			}
		}
	#endif    
	}
#elif defined(_MGEN)
	if(nLcomType == D_LCOMTYPE_GENERAL  || nLcomType == D_LCOMTYPE_FDN)
	{
		if(value == 0) return _LS(IDS_CMD_LCOM_INACTIVE);
		else if(value == 1) return _LS(IDS_CMD_LCOM_ACTIVE);    
	}
	else if(nLcomType == D_LCOMTYPE_CONCRETE||nLcomType == D_LCOMTYPE_STEEL)
	{
		if (value == 0) return _LS(IDS_CMD_LCOM_INACTIVE);
		else if(value == 1) return _LS(IDS_CMD_LCOM_STRENGTH);
		else if(value == 2) return _LS(IDS_CMD_LCOM_SERVICE);
		else if(value == 3) return _LS(IDS_CMD_LCOM_SPECIAL);
		else if(value == 4) return _LS(IDS_CMD_LCOM_VERTICAL);
		else if(value == 5) return _LS(IDS_CMD_LCOM_STRENGTH_ELASTIC);
		else if(value == EN_DGN_UG_STRN) return _LS(IDS_CMD_LCOM_UG_STRENGTH);
		else if(value == EN_DGN_UG_SERV) return _LS(IDS_CMD_LCOM_UG_SERVICE);
		else if(value == EN_DGN_UG_SPEC) return _LS(IDS_CMD_LCOM_UG_SPECIAL);
	}
	else if (nLcomType == D_LCOMTYPE_ALUMINUM)
	{
		if (value == 0) return _LS(IDS_CMD_LCOM_INACTIVE);
		else if (value == 1) return _LS(IDS_CMD_LCOM_STRENGTH);
		else if (value == 2) return _LS(IDS_CMD_LCOM_SERVICE);
		else if (value == 3) return _LS(IDS_CMD_LCOM_SPECIAL);
		else if (value == 4) return _LS(IDS_CMD_LCOM_VERTICAL);
		else if (value == 5) return _LS(IDS_CMD_LCOM_STRENGTH_ELASTIC);
	}
	else if(nLcomType == D_LCOMTYPE_SRC)
	{
		if (value == 0) return _LS(IDS_CMD_LCOM_INACTIVE);
		else if(value == 1) return _LS(IDS_CMD_LCOM_STRENGTH);
		else if(value == 2) return _LS(IDS_CMD_LCOM_SERVICE);  // by GAY. ('13.09.25). MQC:6480. Combo에 _T("Serviceability") 추가.
		else if(value == 3) return _LS(IDS_CMD_LCOM_SPECIAL);
		else if(value == 4) return _LS(IDS_CMD_LCOM_VERTICAL);
		else if(value == 5) return _LS(IDS_CMD_LCOM_STRENGTH_ELASTIC);
		else if(value == EN_DGN_UG_STRN) return _LS(IDS_CMD_LCOM_UG_STRENGTH);
		else if(value == EN_DGN_UG_SERV) return _LS(IDS_CMD_LCOM_UG_SERVICE);
		else if(value == EN_DGN_UG_SPEC) return _LS(IDS_CMD_LCOM_UG_SPECIAL);
	}
	else if(nLcomType == D_LCOMTYPE_CFSTEEL)
	{
		if (value == 0) return _LS(IDS_CMD_LCOM_INACTIVE);
		else if(value == 1) return _LS(IDS_CMD_LCOM_STRENGTH);
		else if(value == 2) return _LS(IDS_CMD_LCOM_SERVICE);
	}
	else if(nLcomType == D_LCOMTYPE_SEISMIC)
	{
		if     (value == 0) return _LS(IDS_CMD_LCOM_INACTIVE);
		else if(value == 1) return _LS(IDS_CMD_LCOM_GENERAL);
		else if(value == 2) return _LS(IDS_CMD_LCOM_VERTICAL);
	}
	else if(nLcomType == D_LCOMTYPE_LINEAR_EVAL)
	{
				 if( value == 0 )  return _LS(IDS_CMD_LCOM_INACTIVE) ;
		else if( value == 1 )  return _LS(IDS_CMD_LCOM_GRAVITY)  ;
		else if( value == 2 )  return _LS(IDS_CMD_LCOM_SEISMIC)  ;
		else if( value == 3 )  return _LS(IDS_CMD_LCOM_SPECIAL)  ;
		else if( value == 4 )  return _LS(IDS_CMD_LCOM_VERTICAL) ;
	}
#else
	if(value == 0) return _LS(IDS_CMD_LCOM_INACTIVE);
	else if(value == 1) return _LS(IDS_CMD_LCOM_ACTIVE);  
#endif 
	return _T("Error");
}

CString CCMLoadCombMainGrid::GetLcomActiveDef(int nLcomType)
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

BOOL CCMLoadCombMainGrid::OnPasteFromClipboard(const CGXRange &range)
{
	AfxMessageBox(_LS(IDS_CMD_DONT_SUPPORT_PASTE));
	return FALSE;
}

CString CCMLoadCombMainGrid::GetLcomTypeList(int nLcomType)
{
	CString strLcomTypeList = _T("");
	if(nLcomType==D_LCOMTYPE_GENERAL)
		strLcomTypeList.Format(_T("%s\n%s\n%s\n%s\n"), D_LCOM_CT_ADD, D_LCOM_CT_ENVELOPE, D_LCOM_CT_ABS, D_LCOM_CT_SRSS);
	else
	{
		if(nLcomType!=D_LCOMTYPE_FDN)
		{
			strLcomTypeList.Format(_T("%s\n%s\n%s\n"), D_LCOM_CT_ADD, D_LCOM_CT_ENVELOPE, D_LCOM_CT_SRSS);
		}
		else strLcomTypeList.Format(_T("%s\n"), D_LCOM_CT_ADD);
	}
	return strLcomTypeList;
}

void CCMLoadCombMainGrid::OnRButtonDown(UINT nFlags, CPoint point)
{
	// Grid가 없으면 바로 리턴
	if(this==NULL) return;
	
	if(!CDBDoc::EnableDynaReport()) return; 
	
	// 메뉴를 읽어온다.
	HMENU hMenu;
	CMenu menu;
	INIResourceManager::LoadMenuToINITranslate(hMenu,IDR_CMD_SMART_TABLE,_T("wg_cmd"), &menu);
	
	CMenu* pPopup = menu.GetSubMenu(0);
	VERIFY(pPopup != NULL);
	ClientToScreen(&point);
	pPopup->TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON,point.x,point.y,this);
}

void CCMLoadCombMainGrid::OnDynamicReportTable()
{
	T_AGEN_D agen_data;
	agen_data.nDlgID = m_nLcomType; // MQC:10236 index 설정오류 by KJH
	m_csTableName = _T("LoadComb");
	agen_data.pData = this;
	CServiceHeadOffice* pServiceHeadOffice = CServiceHeadOffice::Instance();
	int nResultLeng = pServiceHeadOffice->ReqService(_ULS(tb), EN_TBSA_CTBAMRUDTDLG, ( void* ) &agen_data); // MQC:10236 index 설정오류 by KJH
}