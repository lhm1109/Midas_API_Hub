// CMLoadCaseGrid.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMLoadCaseGrid.h"
#include "CMLoadCaseDlg.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_LoadDB.h"

#include "..\wg_base\wg_base_ServiceProvider.h"
#include "..\wg_base\XMultiLang.h"
#include "..\wg_base\CompFunc.h"
#include "..\wg_tb\Tb_ServiceDef.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// 주1:
// Column 1(Key)는 사용자와는 무관하며 프로그램상 필요하므로 Hide시킨다. 
// 따라서 어떤식으로든 사용자에게 나타나서는 안된다.

#define NAME_COL_ID         2
#define STLD_KEY_FMT        _T("%d")

/////////////////////////////////////////////////////////////////////////////
// CCMLoadCaseGrid
CString CCMLoadCaseGrid::m_aCurDefVal[] = {_T(""), _T(""), _T(""), _T("")};
CCMLoadCaseGrid::CCMLoadCaseGrid()
{
	m_pParent = 0;
	m_nCurrentRow = 0;

	T_STLD_UNIT * pUnit = &(CUnitCtrl::m_STLD_UNIT);

	AddColInfo(_LSX(key),                        t_integer,   0);
	AddColInfo(_LS(IDS_CMD_STLD_no),             t_integer,   pUnit->LoadCaseId);
	AddColInfo(_LS(IDS_CMD_STLD_name),           t_string,    pUnit->LoadCaseName);
	AddColInfo(_LS(IDS_CMD_STLD_type2),          t_string,    pUnit->LoadCaseType);
	AddColInfo(_LS(IDS_CMD_STLD_description),    t_string,    pUnit->Description);

	m_aPrimaryKey.Add(0);
}

CCMLoadCaseGrid::~CCMLoadCaseGrid()
{
}

#define CWnd CTBCommon
BEGIN_MESSAGE_MAP(CCMLoadCaseGrid, CWnd)
#undef CWnd
	//{{AFX_MSG_MAP(CCMLoadCaseGrid)
	ON_WM_RBUTTONDBLCLK()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_RBUTTONDOWN()
	ON_COMMAND(ID_CMD_SMART_REPORT_TABLE, OnDynamicReportTable)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CCMLoadCaseGrid message handlers
void CCMLoadCaseGrid::Initialize(CDBDoc* pDoc, CCMLoadCaseDlg* pParent)
{
	m_pParent = pParent;

	T_UNIT_SYSTEM UnitSystem;
	CTBCommon::Initialize(pDoc, UnitSystem);

	LockUpdate(TRUE);

	// 컬럼 갯수 설정
	SetColCount(GetInitialColCount());
	SetHeaderTitle(UnitSystem, TRUE);

	// *^^* Grid에서 입력안하기 때문에 Static control로 변경
	// 표준 스타일 변경
	ChangeStandardStyle(CGXStyle()
			.SetFont(CGXFont_GC().SetSize(10))
			.SetVerticalAlignment(DT_BOTTOM)
			.SetAllowEnter(FALSE)
			.SetControl(GX_IDS_CTRL_STATIC));

	// 컬럼 스타일 변경
	SetStyleRange(CGXRange().SetCols(3, 4), CGXStyle()
			.SetHorizontalAlignment(DT_LEFT)
			.SetValueType(GX_VT_STRING));

	// Key column의 스타일 변경
	SetStyleRange(CGXRange().SetCols(1, 2), CGXStyle()
			.SetHorizontalAlignment(DT_RIGHT)
			.SetValueType(GX_VT_NUMERIC)
			.SetFormat(GX_FMT_FIXED)
			.SetPlaces(0)
			.SetEnabled(FALSE));

	// Key, Serial No Column은 숨긴다.
	// 주1 참조
	//SetFrozenCols(2, 2);
	//HideCols(1, 1);
	HideColsByIntend(1, 1, TRUE);

	// *^^* Grid가 Read Only로 변했기 때문에 Load Type은 콤보로 안 함.
	/*
	CArray<CString, CString&> LoadType;
	m_pDoc->m_pLoadDB->GetLoadTypeList(&LoadType);
	VERIFY(LoadType.GetSize() > 0);

	CString LoadTypeList = _T("");
	for (int i = 0; i < LoadType.GetSize(); i++)
	{
		LoadTypeList += LoadType.GetAt(i)+CString(_T("\t"))
										+m_pDoc->m_pLoadDB->GetLoadTypeName(LoadType.GetAt(i))
										+CString(_T("\n"));
		//GSaveHistoryFormatNF(_T("%s\t%s"), LoadType.GetAt(i),
		//  m_pDoc->m_pLoadDB->GetLoadTypeName(LoadType.GetAt(i)));
	}
	
	SetStyleRange(CGXRange().SetCols(4), CGXStyle()
			.SetControl(GX_IDS_CTRL_TABBED_COMBOBOX)
			.SetChoiceList(LoadTypeList)
			.SetUserAttribute(GX_IDS_UA_TABLIST_KEYCOL, _T("0"))
			.SetUserAttribute(GX_IDS_UA_TABLIST_TEXTCOL, _T("1"))
			.SetUserAttribute(GX_IDS_UA_TABLIST_SHOWALLCOLS, _T("1"))
			.SetValueType(GX_VT_STRING)
			.SetHorizontalAlignment(DT_LEFT)
			.SetValue(_T(""))
			);
	*/

	// 현재 셀의 border 설정 변경
	CGXProperties* pProp = GetParam()->GetProperties();
			pProp->SetUserProperty(GX_IDS_OUTLINECURRENTCELL,
		      (CGXStyle) pProp->sInvertNoBorder);

	// 현재 셀 설정
	SetRightValidCell(1, 0);
	GetParam()->SetHideCurrentCell(GX_HIDE_NEVER);

	// Sorting 정보 초기화
	m_aSortInfo.SetSize(2);
	m_aSortInfo[0].nRC = 1; // serial no
	m_aSortInfo[0].sortType = CGXSortInfo::numeric;
	m_aSortInfo[0].sortOrder = CGXSortInfo::ascending;
	m_aSortInfo[1].nRC = 0; // key
	m_aSortInfo[1].sortType = CGXSortInfo::numeric;
	m_aSortInfo[1].sortOrder = CGXSortInfo::ascending;

	// always show vertical scrollbar
	SetScrollBarMode(SB_VERT, gxnEnabled | gxnEnhanced);  

	// Row 이동을 가능하게 한다.
	GetParam()->EnableMoveRows(TRUE);

	// Column 크기 설정
	SetColWidthByDPI(1, 1, 0);
	SetColWidthByDPI(2, 2, 40);
	SetColWidthByDPI(3, 3, 80);
	SetColWidthByDPI(4, 4, 170);
	SetColWidthByDPI(5, 5, 203);

	MakeItemEx();
	LockUpdate(FALSE);

	SetModeDisplayOnly(TRUE);
}

BOOL CCMLoadCaseGrid::InsertRecordByKey(T_STLD_K &key, T_STLD_D &data)
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

BOOL CCMLoadCaseGrid::DeleteRecordByKey(T_STLD_K &key, T_STLD_D &data)
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

void CCMLoadCaseGrid::UnitChanged(CDBDoc* pDoc)
{
	// unit을 사용하는 것이 없기 때문에 그냥 return 한다.
	return;
}


/////////////////////////////////////////////////////////////////////////////
// CCMLoadCaseGrid Overridable Functions
void CCMLoadCaseGrid::OnChangedSelectionPublic(long *keys, int nSize)
{
	if (nSize <= 0)
	{
		m_aSelectedKeys.RemoveAll();
	}
	else
	{
		m_aSelectedKeys.RemoveAll();
		for (int i = 0; i < nSize; i++) m_aSelectedKeys.Add((T_STLD_K)keys[i]);
	}
}

long CCMLoadCaseGrid::GetSelectedKeys(CArray<long, long> &caKey)
{
	// override this function to return selected record count

	/* 현재 Selected키 관리가 안되고 있다.
		 (UpdateBuffer에서 Add, Del, Modify 처리시 Selected key 관리를 
			안하고 있기 때문)
	*/
	long nKeyCount;
	CArray<T_STLD_K, T_STLD_K> aSelKey;

	GetAllSelectedStld(aSelKey);
	nKeyCount = aSelKey.GetSize();
	caKey.SetSize(nKeyCount);
	for (long i = 0; i < nKeyCount; i++)
		caKey[i] = (long)aSelKey[i];
	
	return nKeyCount;
}

BOOL CCMLoadCaseGrid::ValidateField(CString value, int nColID)
{
	if (nColID >= GetColCount())
	{
		SetWarningText(_LS(IDS_WG_CMD__ADDD___CCMLoadCaseGrid__ValidateField__));
		return FALSE;
	}

	return TRUE;
}

// 삽입이나 수정전에 Record의 Validation을 검사한다.
BOOL CCMLoadCaseGrid::ValidateRecord(CStringArray &value, CRowColArray &aCols)
{
	CString msg = _T("");
	int nPos;

	for (int i = 2; i < GetColCount()-1; i++)
	{
		nPos = aCols[i]-1;
		if (value[nPos] == _T(""))
			msg += ((msg != _T("")) ? _T("\n") : _T("")) + m_aTitle[i] + _LS(IDS_WG_CMD__ADDD___has_not_been_entered_);
	}
	if (msg == _T("")) return TRUE;
	else SetWarningText(msg);
	return FALSE;
}

// 이 함수는 Paste할 때 호출되므로 False를 리턴할 때는 ValidateField와 
// Validate Record에서 설정된 Warning Text를 없애주고 리턴한다.
BOOL CCMLoadCaseGrid::ValidateAndMakeRecord(CStringArray &value, CRowColArray& aCols, T_STLD_K &key, T_STLD_D &data)
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

BOOL CCMLoadCaseGrid::AddToDB(ROWCOL nRow)
{
	// Record를 만들어서 DB에 입력한다.
	T_STLD_K key;
	T_STLD_D data;
 
	GetValue(nRow, key, data);

	return m_pDoc->m_pDataCtrl->AddStld(data);
}

BOOL CCMLoadCaseGrid::AddToDB(CArray<void*, void*>& raKey, CArray<void*, void*>& raData)
{
	ASSERT(raKey.GetSize() == raData.GetSize());
	T_STLD_K tKey;
	CArray<T_STLD_K, T_STLD_K> aKey;
	CArray<T_STLD_D, T_STLD_D&> aData;
	for (int i = 0; i < raKey.GetSize(); i++)
	{
		tKey = *((T_STLD_K*)raKey.GetAt(i));
		aKey.Add(tKey);
		aData.Add(*((T_STLD_D*)raData.GetAt(i)));
	}
	return m_pDoc->m_pDataCtrl->AddStld(aData);
}


BOOL CCMLoadCaseGrid::DeleteFromDB(ROWCOL nRow)
{
	T_STLD_K key;
	T_STLD_D data;
 
	GetValue(nRow, key, data);
	return m_pDoc->m_pDataCtrl->DelStld(data.LoadCaseName);
}

BOOL CCMLoadCaseGrid::DeleteFromDB(CRowColArray& awRows)
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

	return m_pDoc->m_pDataCtrl->DelStld(awKeys);
}

BOOL CCMLoadCaseGrid::ModifyToDB(ROWCOL nRow)
{
	ROWCOL nCol;
	VERIFY(GetColumnNoFor(2, nCol));  // load case name

	T_STLD_K key;
	T_STLD_D data;
	CString pOldLoadCaseName;

	pOldLoadCaseName = GetValueRowColBeforeEdit(nRow, nCol);
	GetValue(nRow, key, data);

	// 수정한다. 실패하면 에러 리턴
	return m_pDoc->m_pDataCtrl->ModifyStld(pOldLoadCaseName, data); 
}

BOOL CCMLoadCaseGrid::ModifyToDB(CStringArray& raOldKey, 
												 CArray<void*, void*>& raKey, 
												 CArray<void*, void*>& raData)
{
	ASSERT(raKey.GetSize() == raData.GetSize());
	ASSERT(raKey.GetSize() == raOldKey.GetSize());
	CArray<CString, CString&> aOldKey;
	CArray<T_STLD_D, T_STLD_D&> aData;
	T_STLD_K Key;
	T_STLD_D Data;
	for (int i = 0; i < raKey.GetSize(); i++)
	{
		Key = _tstol(raOldKey.GetAt(i));
		m_pDoc->m_pAttrCtrl->GetStld(Key, Data);
		aOldKey.Add(Data.LoadCaseName);
		aData.Add(*((T_STLD_D*)raData.GetAt(i)));
	}
	return m_pDoc->m_pDataCtrl->ModifyStld(aOldKey, aData);
}


CString CCMLoadCaseGrid::GetNewKey()
{
	CString value;
	T_STLD_K key = m_pDoc->m_pAttrCtrl->GetStartNumStld();
	value.Format(STLD_KEY_FMT, key);
	return value;
}

CString CCMLoadCaseGrid::GetDefValue(int nIndex)
{
	return m_aCurDefVal[nIndex];
}

CString* CCMLoadCaseGrid::GetDefValueArray()
{
	return m_aCurDefVal;
}

BOOL CCMLoadCaseGrid::MakeKeyAndDataArray(CStringArray& value, CRowColArray& awCols, 
																	CArray<void*, void*>& aKey, CArray<void*, void*>& aData)
{
	T_STLD_K* pKey;
	T_STLD_D* pData;
	pKey = new T_STLD_K;
	pData = new T_STLD_D;
	aKey.Add((void*)pKey);
	aData.Add((void*)pData);
	if (!ValidateAndMakeRecord(value, awCols, *pKey, *pData)) return FALSE;
	return TRUE;
}

void CCMLoadCaseGrid::DeleteKeyAndDataArray(CArray<void*, void*>& aKey, CArray<void*, void*>& aData)
{
	T_STLD_K* pKey;
	T_STLD_D* pData;
	ASSERT(aKey.GetSize() == aData.GetSize());
	for (int i = 0; i < aKey.GetSize(); i++)
	{
		pKey = (T_STLD_K*)aKey.GetAt(i);
		pData = (T_STLD_D*)aData.GetAt(i);
		delete pKey;
		delete pData;
	}
}

long CCMLoadCaseGrid::GetCountData()
{
	CDBDoc* pDoc = m_pDoc;
	return pDoc->m_pAttrCtrl->GetCountStld();
}

void CCMLoadCaseGrid::MakeItemFiltered(long nFilteredNum, long* aFilteredKeys, CRowColArray& awCols)
{
	CDBDoc* pDoc = m_pDoc;
	T_STLD_D Data;

	for(long nCount=0 ; nCount < nFilteredNum; nCount++)
 	{
		pDoc->m_pAttrCtrl->GetStld(aFilteredKeys[nCount], Data);
		SetValue(GetParam()->GetData(), nCount+1, aFilteredKeys[nCount], Data, awCols);
	}
}

void CCMLoadCaseGrid::MakeItemNormal(long nNumData, CRowColArray& awCols)
{
	T_STLD_K Key;
	T_STLD_D Data;
	CDBDoc* pDoc = m_pDoc;

	long* KeyBuf = new long[nNumData];
	long nCount = 0;
	POSITION pos = pDoc->m_pAttrCtrl->GetStartStld();
	while(pos != NULL)
	{
	  pDoc->m_pAttrCtrl->GetNextStld(pos,Key,Data);
	  KeyBuf[nCount++] = Key;
	}
	qsort(KeyBuf, nNumData, sizeof(T_STLD_K), CTBBrowserWnd::CompareKey);

	for(nCount=0 ; nCount < nNumData; nCount++)
	{
		pDoc->m_pAttrCtrl->GetStld(KeyBuf[nCount], Data);
		SetValue(GetParam()->GetData(), nCount+1, KeyBuf[nCount], Data, awCols);
	} // end of loop nCount

	delete []KeyBuf;
}

void CCMLoadCaseGrid::MakeItemEditingRecord(ROWCOL ncRow, CStringArray& EditingRecord)
{
	CDBDoc* pDoc = m_pDoc;
	T_STLD_K Key;
	T_STLD_D Data;
	for (int i = 0; i < GetColCount(); i++)
	{
		if (i == 0) Key = _tstol(EditingRecord[i]);
		else ConvStrToData(i, EditingRecord[i], Data);
	}
	//pDoc->m_pUnitCtrl->ConvertUnitStldPrevious(Data);	// load case는 Unit과 관계없다.
	// 현재 값을 설정한다.
	SetValue(ncRow, Key, Data);
}

/////////////////////////////////////////////////////////////////////////////
// CCMLoadCaseGrid Implementation
BOOL CCMLoadCaseGrid::GetValue(ROWCOL nRow, T_STLD_K &key, T_STLD_D &data)
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
			if (str == _T(""))
				key = m_pDoc->m_pAttrCtrl->GetStartNumStld();
			else
				key = _tstol(str);
		}
		else ConvStrToData(i, str, data);
	}
	return TRUE;
}

BOOL CCMLoadCaseGrid::SetValue(ROWCOL nRow, T_STLD_K key, T_STLD_D &data)
{
	//
	CRowColArray awCols;
	awCols.SetSize(GetColCount());
	for (int i = 0;i < GetColCount(); i++)
		VERIFY(GetColumnNoFor(m_aTitle[i], awCols[i]));
	return SetValue(GetParam()->GetData(), nRow, key, data, awCols);
}

BOOL CCMLoadCaseGrid::SetValue(CGXData* pData, ROWCOL nRow, T_STLD_K key, T_STLD_D &data, CRowColArray &awCols)
{
	//
	CString str;
	for (int i = 0; i < GetColCount(); i++)
	{
		if(i==0) 
		{
			if (key == 0) str.Empty();
			else str.Format(STLD_KEY_FMT,key);
		}
		else ConvDataToStr(i, data, str);
		pData->StoreValueRowCol(nRow, awCols[i], str, gxOverride, GX_VT_NUMERIC);
	}

	return TRUE;
}

void CCMLoadCaseGrid::ConvStrToData(int i, CString& value, T_STLD_D& data)
{
	if (i==1) data.LoadCaseId = _tstol(value); // serial no
	else if (i==2) data.LoadCaseName = value; // name
	else if (i==3) data.LoadCaseType = GetLoadTypeCode(value); // type
	else if (i==4) data.Description = value; // description
}

void CCMLoadCaseGrid::ConvDataToStr(int i, T_STLD_D& data, CString& value)
{
	if(i==1)value.Format(_T("%d"), data.LoadCaseId);
	else if(i==2)value = data.LoadCaseName;
	else if(i==3)value = GetLoadTypeName(data.LoadCaseType);
	else if(i==4) value = data.Description;
}

void CCMLoadCaseGrid::MakeSearchKey(T_STLD_K &key, T_STLD_D &data, CStringArray &aKey)
{
	int nColID;
	int nSortKeyNum = m_aSortInfo.GetSize();

	aKey.SetSize(nSortKeyNum);
	for (int i = 0; i < nSortKeyNum; i++)
	{
		nColID = m_aSortInfo[i].nRC;
		if (nColID == 0) aKey[i].Format(STLD_KEY_FMT, key);
		else ConvDataToStr(nColID, data, aKey[i]);
	}
}

void CCMLoadCaseGrid::GetAllSelectedStld(CArray<T_STLD_K, T_STLD_K>& aSelKey)
{
	for (int i = 0; i < m_aSelectedKeys.GetSize(); i++)
		aSelKey.Add(m_aSelectedKeys[i]);
}

void CCMLoadCaseGrid::SetHeaderTitle(T_UNIT_SYSTEM &UnitSystem, BOOL bInit)
{
	SetHeaderTitle2((int*)m_aUnit.GetData(), bInit, TRUE);
}


// Hide 된 Row들이 resize되지 않도록 한다.
// 주1 참조
BOOL CCMLoadCaseGrid::OnTrackColWidth(ROWCOL nCol)
{
	if (nCol == 1) return FALSE;
	return CTBCommon::OnTrackColWidth(nCol);
}

// TBBrowserWndEx::OnSelDragColsDrop 참조
// 부모를 먼저 호출하면 안 된다.
// 1, 2 모두 Frozen Cols로 설정되어 있으므로 이쪽으로 셀이 이동되거나 이 셀들이
// 이동되는 것을 금지한다.
// 주1 참조
BOOL CCMLoadCaseGrid::OnSelDragColsDrop(ROWCOL nFirstCol, ROWCOL nLastCol, ROWCOL nDestCol)
{
	// Frozen 되어 있는 column 위치에 이동되면 이동을 금지시킨다.
	//if (IsFrozenCol(nDestCol)) return FALSE;
	if (nDestCol == 1 || nFirstCol == 1) return FALSE;
	if (nDestCol == 2 || nFirstCol == 2) return FALSE;
	return CTBCommon::OnSelDragColsDrop(nFirstCol, nLastCol, nDestCol);
}

// LoadCase의 위치를 이동(Serial No가 변경)한다.
BOOL CCMLoadCaseGrid::OnSelDragRowsDrop(ROWCOL nFirstRow, ROWCOL nLastRow, ROWCOL nDestRow)
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

	CArray<CString, CString&> aLoadCaseName;
	CArray<T_STLD_D, T_STLD_D&> aData;
	CString LoadCaseName;
	T_STLD_K key;
	T_STLD_D data;
	for (int i = nFirstRow; i <= nLastRow; i++)
	{
		GetValue(i, key, data);
		data.LoadCaseId = nStart;
		nStart += nInc;

		aLoadCaseName.Add(data.LoadCaseName);
		aData.Add(data);
	}
	m_pDoc->m_pDataCtrl->ModifyStld(aLoadCaseName, aData);
	return FALSE;
}

// 이 함수는 No Column을 클릭했을 때 Row Header를 클릭한 것과 같은 효과를
// 내기 위해서이다. CTBBrowserWndEx::OnLButtonDown 참조
void CCMLoadCaseGrid::OnLButtonDown(UINT nFlags, CPoint point)
{
	ROWCOL nhRow, nhCol, nkCol;
	int ht = HitTest(point, &nhRow, &nhCol);
	if (ht != GX_NOHIT && ht != GX_VERTLINE)
	{
		GetColumnNoFor(1, nkCol);
		if (nhCol == nkCol && IsFrozenCol(nhCol)) // key column이면
		{
			// Row header의 rectangle을 얻는다.
			CRect r = CalcRectFromRowColEx(nhRow, 0, nhRow, 0);  
			point.x = (r.left+r.right)/2;
		}
	}
	CTBCommon::OnLButtonDown(nFlags, point);

	CheckCurrentRecordChanged();
}

CString CCMLoadCaseGrid::GetLoadTypeCode(CString &csLoadTypeName)
{
	return m_pDoc->m_pLoadDB->GetLoadType(csLoadTypeName);
}

CString CCMLoadCaseGrid::GetLoadTypeName(CString &csLoadTypeCode)
{
	return m_pDoc->m_pLoadDB->GetLoadTypeName(csLoadTypeCode);
}

//----------------------------------------------------------------------------->
// 여기서 부터 Current Record의 위치 변경을 Detect하기 위해 삽입된 코드이다.
// Current Record가 변경되었는지 검사
BOOL CCMLoadCaseGrid::CheckCurrentRecordChanged()
{
	ROWCOL ncRow, ncCol;
	GetCurrentCell(ncRow, ncCol);
	if (ncRow != m_nCurrentRow)
	{
		OnChangeCurrentRecord(ncRow);
		return TRUE;
	}
	return FALSE;
}

BOOL CCMLoadCaseGrid::ProcessKeys(CWnd *pSender, UINT nMessage, UINT nChar, UINT nRepCnt, UINT flags)
{
	BOOL bResult = CTBCommon::ProcessKeys(pSender, nMessage, nChar, nRepCnt, flags);
	CheckCurrentRecordChanged();
	return bResult;
}

void CCMLoadCaseGrid::OnRButtonDblClk(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	ROWCOL ncRowBefore, ncRowAfter, ncCol;
	if (!GetCurrentCell(ncRowBefore, ncCol)) return;

	CTBCommon::OnRButtonDblClk(nFlags, point);

	if (!GetCurrentCell(ncRowAfter, ncCol)) return;
	if (ncRowBefore != ncRowAfter) OnChangeCurrentRecord(ncRowAfter);
}

void CCMLoadCaseGrid::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CTBCommon::OnLButtonDblClk(nFlags, point);
	CheckCurrentRecordChanged();
}

// OnLButtonDown이 기존에 정의되어 있기 때문에 기존 함수에 
// CheckCurrentRecordChanged를 호출하도록 한다.
/*
void CCMLoadCaseGrid::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CTBCommon::OnLButtonDown(nFlags, point);
	CheckCurrentRecordChanged();
}
*/

void CCMLoadCaseGrid::OnChangeCurrentRecord(ROWCOL nRow)
{
	m_nCurrentRow = nRow;

	// Append Row이면 그냥 리턴한다.
	if (nRow < 1) return;
	if (IsAppendRow(nRow)) return;

	// Key값을 알아와서 Parent Dialog에 Row의 변동을 알린다.
	ROWCOL nkCol;
	GetColumnNoFor(GetKeyColID(), nkCol);
	T_STLD_K nKey = _ttoi(GetValueRowCol(nRow, nkCol));
	m_pParent->OnChangeCurrentStld(nKey);
}
//<-----------------------------------------------------------------------------

BOOL CCMLoadCaseGrid::GetCurrentStld(T_STLD_K& nKey)
{
	ROWCOL ncRow, ncCol;
	if (!GetCurrentCell(ncRow, ncCol)) return FALSE;
	if (ncRow < 1) return FALSE;
	if (IsAppendRow(ncRow)) return FALSE;

	if (!GetColumnNoFor(GetKeyColID(), ncCol)) return FALSE;
	nKey = _ttoi(GetValueRowCol(ncRow, ncCol));
	return TRUE;
}

BOOL CCMLoadCaseGrid::SetCurrentStld(T_STLD_K key, T_STLD_D& data)
{
	ROWCOL nRow, nCol;

	if (!GetCurrentCell(nRow, nCol)) return FALSE;
	
	CStringArray aKey;
	MakeSearchKey(key, data, aKey);
	if (!FindRecordByKey(aKey, nRow)) return FALSE;
	SetCurrentCell(nRow, nCol);
	CheckCurrentRecordChanged();
	return TRUE;
}

void CCMLoadCaseGrid::OnRButtonDown(UINT nFlags, CPoint point)
{
	// Grid가 없으면 바로 리턴
	if(this==NULL) return;
	
	if(!CDBDoc::EnableDynaReport()) return; // PMS:XXXX-JWKWON-20100502
	
	// 메뉴를 읽어온다.
	CMenu menu;
	HMENU hMenu;
	INIResourceManager::LoadMenuToINITranslate(hMenu,IDR_CMD_SMART_TABLE, _T("wg_cmd"),&menu);
	
	CMenu* pPopup = menu.GetSubMenu(0);
	VERIFY(pPopup != NULL);
	ClientToScreen(&point);
	pPopup->TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON,point.x,point.y,this);
}

void CCMLoadCaseGrid::OnDynamicReportTable()
{
}