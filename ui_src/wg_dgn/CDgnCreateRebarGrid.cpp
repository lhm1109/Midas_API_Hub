// CDgnCreateRebarGrid.cpp: implementation of the CDgnCreateRebarGrid class.
// (060123) sshan Create
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "wg_dgn.h"
#include "CDgnCreateRebarGrid.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_DBLib.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_PostCtrl.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_CobxTdnt.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_db\wg_db_SelectCtrl.h"
#include "..\wg_db\wg_db_CobxTdgr.h"
#include "..\wg_db\wg_db_ViewBuff.h"

#include "..\wg_base\wg_base_MsgDll.h"  
#include "CRebarGridUpdator.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define NAME_COL_ID         2
#define ELEM_KEY_FMT        _T("%d")

//#define _LS(A) ""

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// CDgnCreateRebarGrid
CString CDgnCreateRebarGrid::m_aCurDefVal[] = {_T(""), _T(""), _T(""), _T("")};


CDgnCreateRebarGrid::CDgnCreateRebarGrid()
{
	m_pDoc = CDBDoc::GetDocPoint();

 // m_pParent = 0;
	m_nCurrentRow = 0;

	m_nColHeader = 0;

	T_STLD_UNIT * pUnit = &(CUnitCtrl::m_STLD_UNIT);

	AddColInfo(_LS(IDS_DGN_CREATE_REBAR_KEY),   t_integer, 0);    
	AddColInfo(_LS(IDS_DGN_CREATE_REBAR_NUM),   t_string,  0);
	AddColInfo(_LS(IDS_DGN_CREATE_REBAR_REBAR), t_string,  0);  
	AddColInfo(_LS(IDS_DGN_CREATE_REBAR_Y),     t_real,    D_UNITSYS_BASE_LENGTH);
	AddColInfo(_LS(IDS_DGN_CREATE_REBAR_Z),     t_real,    D_UNITSYS_BASE_LENGTH);
	AddColInfo(_LS(IDS_DGN_CREATE_REBAR_BETA),  t_real,    D_UNITSYS_NONE);

	m_aPrimaryKey.Add(0);

	m_MabrDataList.InitHashTable(101);
}

CDgnCreateRebarGrid::~CDgnCreateRebarGrid()
{
}

#define CWnd CTBCommon
BEGIN_MESSAGE_MAP(CDgnCreateRebarGrid, CWnd)
#undef CWnd
	//{{AFX_MSG_MAP(CDgnCreateRebarGrid)
	ON_WM_RBUTTONDBLCLK()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CDgnCreateRebarGrid message handlers
void CDgnCreateRebarGrid::Initialize(CArray<T_REBT_MABR, T_REBT_MABR&>* pData, CConcSectionCreateRebar* pParent)
{
 CTBBrowserWndEx::Initialize();

	GetParam()->EnableUndo(FALSE);
	EnableHints();
	GetParam()->EnableTrackRowHeight(FALSE);

	m_pParent = pParent;

	m_pData = pData;

	T_UNIT_SYSTEM UnitSystem;
	
	CTBCommon::m_pDoc = m_pDoc;

	// Get Rebar Information
	CStringArray aRBName;
	CDBLib::GetRebarNameListByCode(aRBName, TRUE);

	CString csChoiceList = _T("");
	int nSize = aRBName.GetSize();
	// dia.
	for (int i = 0; i < nSize; i++)
	{
		csChoiceList += aRBName[i]+_T("\n");
	}

	LockUpdate(TRUE);

	// 컬럼 갯수 설정
	SetColCount(GetInitialColCount());
	SetHeaderTitle(UnitSystem, TRUE);

	// Sorting 정보 초기화
	m_aSortInfo.SetSize(1);
	m_aSortInfo[0].nRC = GetKeyColID();
	m_aSortInfo[0].sortType = CGXSortInfo::numeric;
	m_aSortInfo[0].sortOrder = CGXSortInfo::ascending;

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

	
	// SetValue....  (Header를 제외하고 0-base);
	// 초기화  ....  (Header를 포함하고 0-base);


	SetRowHeightByDPI(0, 0, 24);

	// Key Column 
	SetStyleRange(CGXRange().SetCols(1), CGXStyle()
			.SetVerticalAlignment(DT_VCENTER)
			.SetHorizontalAlignment(DT_RIGHT)
			.SetValueType(GX_VT_NUMERIC)
			.SetPlaces(0));
	
	// Key Column 감추기 
	HideColsByIntend(1, 1, TRUE);

	SetStyleRange(CGXRange().SetCols(2), CGXStyle()
			.SetVerticalAlignment(DT_VCENTER)
			.SetHorizontalAlignment(DT_RIGHT)
			.SetValueType(GX_VT_STRING)
			//.SetValue(_T("0"))      
			.SetReadOnly(FALSE));

	SetStyleRange(CGXRange().SetCols(3), CGXStyle()
			.SetVerticalAlignment(DT_VCENTER)
			.SetHorizontalAlignment(DT_RIGHT)
			.SetValueType(GX_VT_STRING)
			.SetPlaces(0)
			.SetControl(GX_IDS_CTRL_TABBED_COMBOBOX)
			.SetChoiceList(csChoiceList)
			//.SetValue(_T("Dia0"))
			.SetReadOnly(FALSE));

	// 컬럼 스타일 변경
	SetStyleRange(CGXRange().SetCols(4), CGXStyle()
			.SetVerticalAlignment(DT_VCENTER)
			.SetHorizontalAlignment(DT_RIGHT)
			.SetValueType(GX_VT_NUMERIC)
			.SetFormat(GX_FMT_FIXED)
			.SetPlaces(4)
			//.SetValue(_T("0"))
			.SetReadOnly(FALSE));

	SetStyleRange(CGXRange().SetCols(5), CGXStyle()
			.SetVerticalAlignment(DT_VCENTER)
			.SetHorizontalAlignment(DT_RIGHT)
			.SetValueType(GX_VT_NUMERIC)
			.SetFormat(GX_FMT_FIXED)
			.SetPlaces(4)
			//.SetValue(_T("0"))
			.SetReadOnly(FALSE));

	SetStyleRange(CGXRange().SetCols(6), CGXStyle()
		.SetVerticalAlignment(DT_VCENTER)
		.SetHorizontalAlignment(DT_RIGHT)
		.SetValueType(GX_VT_NUMERIC)
		.SetFormat(GX_FMT_FIXED)
		.SetPlaces(2)
		//.SetValue(_T("0"))
		.SetReadOnly(FALSE));

	// Key, Serial No Column은 숨긴다.
	// 주1 참조
	SetFrozenCols(1, 1);
	HideCols(1, 1);
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
	/*
	m_aSortInfo.SetSize(2);
	m_aSortInfo[0].nRC = 1; // serial no
	m_aSortInfo[0].sortType = CGXSortInfo::numeric;
	m_aSortInfo[0].sortOrder = CGXSortInfo::ascending;
	m_aSortInfo[1].nRC = 0; // key
	m_aSortInfo[1].sortType = CGXSortInfo::numeric;
	m_aSortInfo[1].sortOrder = CGXSortInfo::ascending;
	*/

	// always show vertical scrollbar
	SetScrollBarMode(SB_VERT, gxnEnabled | gxnEnhanced);  

	// Row 이동을 가능하게 한다.
	GetParam()->EnableMoveRows(TRUE);

	CString strCodeName = CDBLib::GetConCodeName();

	// Column 크기 설정
	if(strCodeName != CONCODE_SNIP_2_05_03_84 && strCodeName != CONCODE_SP_35_13330_2011 && strCodeName != CONCODE_SNIP_2_05_03_84_MKS && strCodeName != CONCODE_SP_35_13330_2011_MKS)
	{
		SetColWidthByDPI(1, 1, 0);
		SetColWidthByDPI(2, 2, 50);
		SetColWidthByDPI(3, 3, 70);
		SetColWidthByDPI(4, 5, 70);
		SetColWidthByDPI(6, 6, 70); // Hide
	}
	else
	{
		SetColWidthByDPI(1, 1, 0);
		SetColWidthByDPI(2, 2, 40);
		SetColWidthByDPI(3, 3, 50);
		SetColWidthByDPI(4, 5, 60);
		SetColWidthByDPI(6, 6, 50);
	}	
	
	MakeItemEx();

	if(strCodeName != CONCODE_SNIP_2_05_03_84 && strCodeName != CONCODE_SP_35_13330_2011 && strCodeName != CONCODE_SNIP_2_05_03_84_MKS && strCodeName != CONCODE_SP_35_13330_2011_MKS)
	{
		HideCols(6, 6);
	}
	
	LockUpdate(FALSE);

	SetModeDisplayOnly(FALSE);
}

void CDgnCreateRebarGrid::MakeItemEx()
{
	CWaitCursor wait;

	SetModeDisplayOnly(FALSE);

	BOOL bOldLock = LockUpdate(TRUE);

	int nRecCount = m_pData->GetSize();
	SetRowCount(m_nColHeader);
	SetRecordCount(m_nColHeader+nRecCount);
/*
	T_REBT_MABR data;
	for (int i=0; i<nRecCount; i++)
	{
		data = m_pData->GetAt(i);
		SetValue((ROWCOL)(i+1+m_nColHeader), i, data);
	}
*/
	//SetRightValidCell(1+m_nColHeader, 1);

	LockUpdate(bOldLock);
	if (!bOldLock) Redraw();  
}

BOOL CDgnCreateRebarGrid::InsertRecordByKey(REBAR_KEY &key, T_REBT_MABR &data)
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

BOOL CDgnCreateRebarGrid::DeleteRecordByKey(REBAR_KEY &key, T_REBT_MABR &data)
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

void CDgnCreateRebarGrid::UnitChanged(CDBDoc* pDoc)
{
	// unit을 사용하는 것이 없기 때문에 그냥 return 한다.
	return;
}


/////////////////////////////////////////////////////////////////////////////
// CDgnCreateRebarGrid Overridable Functions
void CDgnCreateRebarGrid::OnChangedSelectionPublic(long *keys, int nSize)
{
	if (nSize <= 0)
	{
		m_aSelectedKeys.RemoveAll();
	}
	else
	{
		m_aSelectedKeys.RemoveAll();
		for (int i = 0; i < nSize; i++) m_aSelectedKeys.Add((REBAR_KEY)keys[i]);
	}
}

long CDgnCreateRebarGrid::GetSelectedKeys(CArray<long, long> &caKey)
{
	// override this function to return selected record count

	/* 현재 Selected키 관리가 안되고 있다.
		 (UpdateBuffer에서 Add, Del, Modify 처리시 Selected key 관리를 
			안하고 있기 때문)
	*/
	long nKeyCount;
	CArray<REBAR_KEY, REBAR_KEY> aSelKey;

	GetAllSelectedStld(aSelKey);
	nKeyCount = aSelKey.GetSize();
	caKey.SetSize(nKeyCount);
	for (long i = 0; i < nKeyCount; i++)
		caKey[i] = (long)aSelKey[i];
	
	return nKeyCount;
}

BOOL CDgnCreateRebarGrid::ValidateField(CString value, int nColID)
{
	if (nColID >= GetColCount())
	{
		SetWarningText(_LS(IDS_WG_CMD__ADDD___CCMLoadCaseGrid__ValidateField__));
		return FALSE;
	}

	return TRUE;
}

// 삽입이나 수정전에 Record의 Validation을 검사한다.
BOOL CDgnCreateRebarGrid::ValidateRecord(CStringArray &value, CRowColArray &aCols)
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
BOOL CDgnCreateRebarGrid::ValidateAndMakeRecord(CStringArray &value, CRowColArray& aCols, REBAR_KEY &key, T_REBT_MABR &data)
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

BOOL CDgnCreateRebarGrid::AddToDB(ROWCOL nRow)
{
	// Record를 만들어서 DB에 입력한다.
	REBAR_KEY key;
	T_REBT_MABR data;
 
	GetValue(nRow, key, data);

	//return m_pDoc->m_pDataCtrl->AddStld(data);
	return true;
}

BOOL CDgnCreateRebarGrid::AddToDB(CArray<void*, void*>& raKey, CArray<void*, void*>& raData)
{
	ASSERT(raKey.GetSize() == raData.GetSize());
	REBAR_KEY tKey;
	CArray<REBAR_KEY, REBAR_KEY> aKey;
	CArray<T_REBT_MABR, T_REBT_MABR&> aData;
	for (int i = 0; i < raKey.GetSize(); i++)
	{
		tKey = *((REBAR_KEY*)raKey.GetAt(i));
		aKey.Add(tKey);
		aData.Add(*((T_REBT_MABR*)raData.GetAt(i)));
	}
//  return m_pDoc->m_pDataCtrl->AddStld(aData);
	return true;
}


BOOL CDgnCreateRebarGrid::DeleteFromDB(ROWCOL nRow)
{
	REBAR_KEY key;
	T_REBT_MABR data;
 
	GetValue(nRow, key, data);
//  return m_pDoc->m_pDataCtrl->DelStld(data.LoadCaseName);
	return true;
}

BOOL CDgnCreateRebarGrid::DeleteFromDB(CRowColArray& awRows)
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

	return TRUE;
	//return m_pDoc->m_pDataCtrl->DelStld(awKeys);
}

BOOL CDgnCreateRebarGrid::ModifyToDB(ROWCOL nRow)
{
	ROWCOL nCol;
	VERIFY(GetColumnNoFor(2, nCol));  // load case name

	REBAR_KEY key;
	T_REBT_MABR data;
	CString pOldLoadCaseName;

	pOldLoadCaseName = GetValueRowColBeforeEdit(nRow, nCol);
	GetValue(nRow, key, data);

	// 수정한다. 실패하면 에러 리턴
	//return m_pDoc->m_pDataCtrl->ModifyStld(pOldLoadCaseName, data); 
	return true;
}

BOOL CDgnCreateRebarGrid::ModifyToDB(CStringArray& raOldKey, 
												 CArray<void*, void*>& raKey, 
												 CArray<void*, void*>& raData)
{
	ASSERT(raKey.GetSize() == raData.GetSize());
	ASSERT(raKey.GetSize() == raOldKey.GetSize());
	CArray<CString, CString&> aOldKey;
	CArray<T_REBT_MABR, T_REBT_MABR&> aData;
	REBAR_KEY Key;
	T_REBT_MABR Data;
	for (int i = 0; i < raKey.GetSize(); i++)
	{
		Key = _ttol(raOldKey.GetAt(i));
		//m_pDoc->m_pAttrCtrl->GetStld(Key, Data);
		//aOldKey.Add(Data.LoadCaseName);
		aData.Add(*((T_REBT_MABR*)raData.GetAt(i)));
	}
	//return m_pDoc->m_pDataCtrl->ModifyStld(aOldKey, aData);
	return true;
}


CString CDgnCreateRebarGrid::GetNewKey()
{
	CString value;
	/*
	REBAR_KEY key = m_pDoc->m_pAttrCtrl->GetStartNumElem();
	//REBAR_KEY key = m_pDoc->m_pAttrCtrl->GetStartNumStld();
	value.Format(ELEM_KEY_FMT, key);*/
	return value;
}

CString CDgnCreateRebarGrid::GetDefValue(int nIndex)
{
	return m_aCurDefVal[nIndex];
}

CString* CDgnCreateRebarGrid::GetDefValueArray()
{
	return m_aCurDefVal;
}

BOOL CDgnCreateRebarGrid::MakeKeyAndDataArray(CStringArray& value, CRowColArray& awCols, 
																	CArray<void*, void*>& aKey, CArray<void*, void*>& aData)
{
	REBAR_KEY* pKey;
	T_REBT_MABR* pData;
	pKey = new REBAR_KEY;
	pData = new T_REBT_MABR;
	aKey.Add((void*)pKey);
	aData.Add((void*)pData);
	if (!ValidateAndMakeRecord(value, awCols, *pKey, *pData)) return FALSE;
	return TRUE;
}

void CDgnCreateRebarGrid::DeleteKeyAndDataArray(CArray<void*, void*>& aKey, CArray<void*, void*>& aData)
{
	REBAR_KEY* pKey;
	T_REBT_MABR* pData;
	ASSERT(aKey.GetSize() == aData.GetSize());
	for (int i = 0; i < aKey.GetSize(); i++)
	{
		pKey = (REBAR_KEY*)aKey.GetAt(i);
		pData = (T_REBT_MABR*)aData.GetAt(i);
		delete pKey;
		delete pData;
	}
}


long CDgnCreateRebarGrid::GetCountData()
{
	CDBDoc* pDoc = m_pDoc;
	return pDoc->m_pAttrCtrl->GetCountElem();
	//return pDoc->m_pAttrCtrl->GetCountStld();
}


void CDgnCreateRebarGrid::MakeItemFiltered(long nFilteredNum, long* aFilteredKeys, CRowColArray& awCols)
{
	CDBDoc* pDoc = m_pDoc;
	T_REBT_MABR Data;

	for(long nCount=0 ; nCount < nFilteredNum; nCount++)
 	{
		//pDoc->m_pAttrCtrl->GetStld(aFilteredKeys[nCount], Data);
		SetValue(GetParam()->GetData(), nCount+1, aFilteredKeys[nCount], Data, awCols);
	}
}

void CDgnCreateRebarGrid::MakeItemNormal(long nNumData, CRowColArray& awCols)
{
	//REBAR_KEY Key;
	T_REBT_MABR Data;
	CDBDoc* pDoc = m_pDoc;

	long* KeyBuf = new long[nNumData];
	long nCount = 0;
	//POSITION pos = pDoc->m_pAttrCtrl->GetStartStld();
	POSITION pos = pDoc->m_pAttrCtrl->GetStartElem();
	while(pos != NULL)
	{
	  //pDoc->m_pAttrCtrl->GetNextStld(pos,Key,Data);
	  //KeyBuf[nCount++] = Key;
	}
	qsort(KeyBuf, nNumData, sizeof(REBAR_KEY), CTBBrowserWnd::CompareKey);

	for(nCount=0 ; nCount < nNumData; nCount++)
	{
		//pDoc->m_pAttrCtrl->GetStld(KeyBuf[nCount], Data);
		SetValue(GetParam()->GetData(), nCount+1, KeyBuf[nCount], Data, awCols);
	} // end of loop nCount

	delete []KeyBuf;
}

void CDgnCreateRebarGrid::MakeItemEditingRecord(ROWCOL ncRow, CStringArray& EditingRecord)
{
	CDBDoc* pDoc = m_pDoc;
	REBAR_KEY Key;
	T_REBT_MABR Data;
	for (int i = 0; i < GetColCount(); i++)
	{
		if (i == 0) Key = _ttol(EditingRecord[i]);
		else ConvStrToData(i, EditingRecord[i], Data);
	}
	//pDoc->m_pUnitCtrl->ConvertUnitStldPrevious(Data);	// load case는 Unit과 관계없다.
	// 현재 값을 설정한다.
	SetValue(ncRow, Key, Data);
}

/////////////////////////////////////////////////////////////////////////////
// CDgnCreateRebarGrid Implementation
BOOL CDgnCreateRebarGrid::GetValue(ROWCOL nRow, REBAR_KEY &key, T_REBT_MABR &data)
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
				key = m_pDoc->m_pAttrCtrl->GetStartNumElem();
				//key = m_pDoc->m_pAttrCtrl->GetStartNumStld();
			else
				key = _ttol(str);
		}
		else ConvStrToData(i, str, data);
	}
	return TRUE;
}

BOOL CDgnCreateRebarGrid::SetValue(ROWCOL nRow, REBAR_KEY key, T_REBT_MABR &data)
{
	//
	CRowColArray awCols;
	awCols.SetSize(GetColCount());
	for (int i = 0;i < GetColCount(); i++)
		VERIFY(GetColumnNoFor(m_aTitle[i], awCols[i]));
	return SetValue(GetParam()->GetData(), nRow, key, data, awCols);
}

BOOL CDgnCreateRebarGrid::SetValue(CGXData* pData, ROWCOL nRow, REBAR_KEY key, T_REBT_MABR &data, CRowColArray &awCols)
{
	//
	CString str;
	for (int i = 0; i < GetColCount(); i++)
	{
		ConvDataToStr(i, key, data, str);
		pData->StoreValueRowCol(nRow, awCols[i], str, gxOverride, GX_VT_NUMERIC);
	}

	/*
	for (int i = 0; i < GetColCount(); i++)
	{
		if(i==0) 
		{
			if (key == 0) str.Empty();
			else str.Format(ELEM_KEY_FMT,key);
		}
		else ConvDataToStr(i, key, data, str);
		pData->StoreValueRowCol(nRow, awCols[i], str, gxOverride, GX_VT_NUMERIC);
	}*/

	return TRUE;
}

void CDgnCreateRebarGrid::ConvStrToData(int i, CString& value, T_REBT_MABR& data)
{
	switch(i)
	{
		case 0:
			break;
		case 1:
			data.iLabel = _ttoi(value);
			break;
		case 2:
			data.strRebarName = value;
			break;
		case 3:
			data.dCenterY = _tstof(value);
			break;
		case 4:
			data.dCenterZ = _tstof(value);
			break;
		case 5:
			data.dBeta = _tstof(value);
			break;		
		default: ASSERT(0); return ;
	}
}




void CDgnCreateRebarGrid::ConvDataToStr(int i, REBAR_KEY key, T_REBT_MABR& data, CString& value)
{
	switch(i)
	{
		case 0:
			value.Format(_T("%d"), key);	
			break;
		case 1:
			value.Format(_T("%d"), data.iLabel);	
			break;
		case 2:
			value = data.strRebarName;
			break;
		case 3:
			value.Format(_T("%.9g"), data.dCenterY);
			break;
		case 4:
			value.Format(_T("%.9g"), data.dCenterZ);
			break;
		case 5:
			value.Format(_T("%.9g"), data.dBeta);
			break;
		default: ASSERT(0); return ;
	}
}



void CDgnCreateRebarGrid::MakeSearchKey(REBAR_KEY &key, T_REBT_MABR &data, CStringArray &aKey)
{
	int nColID;
	int nSortKeyNum = m_aSortInfo.GetSize();

	aKey.SetSize(nSortKeyNum);
	for (int i = 0; i < nSortKeyNum; i++)
	{
		nColID = m_aSortInfo[i].nRC;
		if (nColID == 0) aKey[i].Format(ELEM_KEY_FMT, key);
		else ConvDataToStr(nColID, key, data, aKey[i]);
	}
}

void CDgnCreateRebarGrid::GetAllSelectedStld(CArray<REBAR_KEY, REBAR_KEY>& aSelKey)
{
	for (int i = 0; i < m_aSelectedKeys.GetSize(); i++)
		aSelKey.Add(m_aSelectedKeys[i]);
}

void CDgnCreateRebarGrid::SetHeaderTitle(T_UNIT_SYSTEM &UnitSystem, BOOL bInit)
{
	// Set Title
	/*
	int aSize[11];
	if (m_nInputType == 1)
	{
		aSize[0]=52; aSize[1]=52; aSize[2]=52; aSize[3]=20; aSize[4]=54; aSize[5]=54; aSize[6]=120; 
		aSize[7]=0; aSize[8]=0; aSize[9]=0; aSize[10]=0;
	}
	else if (m_nInputType == 2)
	{
		aSize[0]=38; aSize[1]=38; aSize[2]= 0; aSize[3]=20; aSize[4]= 0; aSize[5]=60; aSize[6]=40; 
		aSize[7]=50; aSize[8]=46; aSize[9]=37; aSize[10]=37;
	}
	else if (m_nInputType == 3)
	{
		aSize[0]=38; aSize[1]= 0; aSize[2]=38; aSize[3]=20; aSize[4]=60; aSize[5]= 0; aSize[6]=40; 
		aSize[7]=50; aSize[8]=46; aSize[9]=37; aSize[10]=37;
	}
	else ASSERT(0);

	CString csUnit, csTitle;
	int nColCount = GetColCount();
	for (int i = 0; i < nColCount; i++)
	{
		CUnitCtrl::GetUnitSystem(m_aUnit[i], csUnit);
		if (csUnit.IsEmpty()) csTitle = m_aTitle[i];
		else if (i==4 || i==5 || i==8) csTitle = m_aTitle[i] + csUnit;
		else csTitle = m_aTitle[i] + _T("(")+csUnit+_T(")");

		SetStyleRange(CGXRange(0, i+1), CGXStyle()
			.SetValue(csTitle)
			.SetUserAttribute(ID_USER_ATTR_COL_ID, m_aTitle[i]));
		if (bInit) SetColWidth(i+1, i+1, aSize[i]);
	}
	if (bInit) SetColWidth(0, 0, 25);
	*/

	const int GWidth = 80;  
	
	int aSize[6];

	aSize[0]=10; 
	aSize[1]=GWidth; 
	aSize[2]=GWidth;
	aSize[3]=GWidth;
	aSize[4]=GWidth;
	aSize[5]=GWidth;
	

	CString csUnit, csTitle;
	int nColCount = GetColCount();
	for (int i = 0; i < nColCount; i++)
	{
		CUnitCtrl::GetUnitSystem(m_aUnit[i], csUnit);
		if (csUnit.IsEmpty()) csTitle = m_aTitle[i];
		//else if (i==4 || i==5 || i==8) csTitle = m_aTitle[i] + csUnit;
		else csTitle = m_aTitle[i] + _T("(")+csUnit+_T(")");

		SetStyleRange(CGXRange(0, i+1), CGXStyle()
			.SetValue(csTitle)
			.SetUserAttribute(ID_USER_ATTR_COL_ID, m_aTitle[i]));
		if (bInit) SetColWidthByDPI(i+1, i+1, aSize[i]);
	}
	if (bInit) SetColWidthByDPI(0, 0, 25);
}


// Hide 된 Row들이 resize되지 않도록 한다.
// 주1 참조
BOOL CDgnCreateRebarGrid::OnTrackColWidth(ROWCOL nCol)
{
	if (nCol == 1) return FALSE;
	return CTBCommon::OnTrackColWidth(nCol);
}

// TBBrowserWndEx::OnSelDragColsDrop 참조
// 부모를 먼저 호출하면 안 된다.
// 1, 2 모두 Frozen Cols로 설정되어 있으므로 이쪽으로 셀이 이동되거나 이 셀들이
// 이동되는 것을 금지한다.
// 주1 참조
BOOL CDgnCreateRebarGrid::OnSelDragColsDrop(ROWCOL nFirstCol, ROWCOL nLastCol, ROWCOL nDestCol)
{
	// Frozen 되어 있는 column 위치에 이동되면 이동을 금지시킨다.
	//if (IsFrozenCol(nDestCol)) return FALSE;
	if (nDestCol == 1 || nFirstCol == 1) return FALSE;
	if (nDestCol == 2 || nFirstCol == 2) return FALSE;
	return CTBCommon::OnSelDragColsDrop(nFirstCol, nLastCol, nDestCol);
}

// LoadCase의 위치를 이동(Serial No가 변경)한다.
/*
BOOL CDgnCreateRebarGrid::OnSelDragRowsDrop(ROWCOL nFirstRow, ROWCOL nLastRow, ROWCOL nDestRow)
{
	// 수정 모드이면 에러 리턴
/*
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
	CArray<T_REBT_MABR, T_REBT_MABR&> aData;
	CString LoadCaseName;
	REBAR_KEY key;
	T_REBT_MABR data;
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
*/

// 이 함수는 No Column을 클릭했을 때 Row Header를 클릭한 것과 같은 효과를
// 내기 위해서이다. CTBBrowserWndEx::OnLButtonDown 참조
void CDgnCreateRebarGrid::OnLButtonDown(UINT nFlags, CPoint point)
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

/*
CString CDgnCreateRebarGrid::GetLoadTypeCode(CString &csLoadTypeName)
{
	return m_pDoc->m_pLoadDB->GetLoadType(csLoadTypeName);
}

CString CDgnCreateRebarGrid::GetLoadTypeName(CString &csLoadTypeCode)
{
	return m_pDoc->m_pLoadDB->GetLoadTypeName(csLoadTypeCode);
}
*/

//----------------------------------------------------------------------------->
// 여기서 부터 Current Record의 위치 변경을 Detect하기 위해 삽입된 코드이다.
// Current Record가 변경되었는지 검사
BOOL CDgnCreateRebarGrid::CheckCurrentRecordChanged()
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

BOOL CDgnCreateRebarGrid::ProcessKeys(CWnd *pSender, UINT nMessage, UINT nChar, UINT nRepCnt, UINT flags)
{
	BOOL bResult = CTBCommon::ProcessKeys(pSender, nMessage, nChar, nRepCnt, flags);
	CheckCurrentRecordChanged();
	return bResult;
}

void CDgnCreateRebarGrid::OnRButtonDblClk(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	ROWCOL ncRowBefore, ncRowAfter, ncCol;
	if (!GetCurrentCell(ncRowBefore, ncCol)) return;

	CTBCommon::OnRButtonDblClk(nFlags, point);

	if (!GetCurrentCell(ncRowAfter, ncCol)) return;
	if (ncRowBefore != ncRowAfter) OnChangeCurrentRecord(ncRowAfter);
}

void CDgnCreateRebarGrid::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CTBCommon::OnLButtonDblClk(nFlags, point);
	CheckCurrentRecordChanged();
}

// OnLButtonDown이 기존에 정의되어 있기 때문에 기존 함수에 
// CheckCurrentRecordChanged를 호출하도록 한다.
/*
void CDgnCreateRebarGrid::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CTBCommon::OnLButtonDown(nFlags, point);
	CheckCurrentRecordChanged();
}
*/

void CDgnCreateRebarGrid::OnChangeCurrentRecord(ROWCOL nRow)
{
	m_nCurrentRow = nRow;

	// Append Row이면 그냥 리턴한다.~
	if (nRow < 1) return;
	if (IsAppendRow(nRow)) return;

	// Key값을 알아와서 Parent Dialog에 Row의 변동을 알린다.
	ROWCOL nkCol;
	GetColumnNoFor(GetKeyColID(), nkCol);
	REBAR_KEY nKey = _ttoi(GetValueRowCol(nRow, nkCol));
	//m_pParent->OnChangeCurrentRcbe(nKey);
}
//<-----------------------------------------------------------------------------

BOOL CDgnCreateRebarGrid::GetCurrentMaRb(REBAR_KEY& nKey)
{
	ROWCOL ncRow, ncCol;
	if (!GetCurrentCell(ncRow, ncCol)) return FALSE;
	if (ncRow < 1) return FALSE;
	if (IsAppendRow(ncRow)) return FALSE;

	if (!GetColumnNoFor(GetKeyColID(), ncCol)) return FALSE;
	nKey = _ttoi(GetValueRowCol(ncRow, ncCol));
	return TRUE;
}

BOOL CDgnCreateRebarGrid::SetCurrentMaRb(REBAR_KEY key, T_REBT_MABR& data)
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

//데이터의 Key값으로 해당 Row를 삭제한다
void CDgnCreateRebarGrid::DeleteRowByKey(REBAR_KEY key)
{
	REBAR_KEY keyData;
	T_REBT_MABR data;

	for(int i=0 ; i<GetRowCount() ; i++)
	{
		if(!GetValue(i, keyData, data))
			continue;

		if(key == keyData)
		{
			if(i==0)
			{
				DeleteRecord(i+1);
			}
			else
			{
				DeleteRecord(i);
			}
			return;
		}
	}
}

BOOL CDgnCreateRebarGrid::OnPasteFromClipboard(const CGXRange &range)
{
	// 임시로 CGXGridWnd를 하나 만들어 Paste를 한 후 작업을 한다.
	CString csErrMsg;
	CRowColArray awRows, awCols;
	int nTotalRow, nTotalCol;

	CGXRange r;
	r.top = r.bottom = 1;
	r.left = r.right = 1;
	CGXGridWnd x;
	x.Create(WS_CHILD, CRect(0, 0, 1, 1), this, 0);
	x.Initialize();
	x.SetRowCount(0); x.SetColCount(1);
	x.OnPasteFromClipboard(r);

	r.top = 1; r.left = 1;
	r.bottom = x.GetRowCount(); r.right = x.GetColCount();
	int nPastedRows = r.bottom - r.top + 1;   
	int nPastedCols = r.right - r.left + 1;  

	if (nPastedCols > 4)  
	{
		csErrMsg = _LS(IDS_TM_STDG_GRID_ERR_MSG5);
		goto PASTE_ERROR;
	}

	nTotalRow = GetRowCount()-1;
	nTotalCol = GetColCount();

	GetSelectedRows(awRows);
	GetSelectedCols(awCols);

	if (awRows.GetSize() > 1) // Select된 것이 2줄 이상
	{
		if (nPastedRows != 1)
		{
			if (awRows.GetSize() != nPastedRows)
			{
				csErrMsg = _LS(IDS_TM_STDG_GRID_ERR_MSG2);
				goto PASTE_ERROR;
			}
		}
	}
	else 
	{
		int nTgtRow;
		if (awRows.GetSize() == 0)  
		{
			ROWCOL ncRow, ncCol;
			if (!GetCurrentCell(ncRow, ncCol))  
			{
				csErrMsg = _LS(IDS_TM_STDG_GRID_ERR_MSG4);
				goto PASTE_ERROR;
			}
			nTgtRow = ncRow;
		}
		else nTgtRow = awRows[0];
		
		if (nTgtRow + nPastedRows - 1 > nTotalRow)
		{
			csErrMsg = _LS(IDS_TM_STDG_GRID_ERR_MSG5);
			goto PASTE_ERROR;
		}

		if(awCols[0] + nPastedCols - 1 > nTotalCol)
		{
			csErrMsg = _LS(IDS_TM_STDG_GRID_ERR_MSG5);
			goto PASTE_ERROR;
		}
	}
	
	x.DestroyWindow();

	CGXGridWnd::OnPasteFromClipboard(range);

	//SaveGridData();
	return TRUE;   

PASTE_ERROR:
	SetWarningText(csErrMsg);
	DisplayWarningText();
	x.DestroyWindow();
	return FALSE;
}

BOOL CDgnCreateRebarGrid::SaveGridData()
{
	//저장하기전 올바른 데이터인지 체크한다
	if(!GridCheckData())
	{
		AfxMessageBox(_LS(IDS_DGN_CREATE_REBAR_GRID_ERROR), MB_OK);
		return FALSE;
	}

	m_MabrDataList.RemoveAll();

	int RowCount = GetRowCount()-1;
	T_REBT_MABR MData;
	MData.Initialize();
	for(int i=1 ; i<GetRowCount() ; i++)
	{
		MData.iLabel = _ttoi(GetValueRowCol(i, 2));
		MData.strRebarName = GetValueRowCol(i, 3);
		MData.dCenterY = _tstof(GetValueRowCol(i, 4));
		MData.dCenterZ = _tstof(GetValueRowCol(i, 5));
		MData.dBeta = _tstof(GetValueRowCol(i, 6));
		m_MabrDataList.SetAt(_ttoi(GetValueRowCol(i, 1)), MData);		
	}

	return TRUE;
}

BOOL CDgnCreateRebarGrid::OnInsertEmptyRecord(ROWCOL nRow)
{
	CGXData* pData = GetParam()->GetData();

	CString value = _T("");
	value.Format(_T("%d"), GetLastRebarKey()+1);
	pData->StoreValueRowCol(nRow, 1, value, gxOverride, GX_VT_NUMERIC);

	return TRUE;   
}

BOOL CDgnCreateRebarGrid::OnAddNewRecord(ROWCOL nRow)
{	
	return TRUE;   
}

REBAR_KEY CDgnCreateRebarGrid::GetLastRebarKey()
{	
	REBAR_KEY key = 0;
	int RowCount = GetRowCount()-1;
	T_REBT_MABR MData;
	MData.Initialize();
	for(int i=1 ; i<GetRowCount() ; i++)
	{
		if(key < _ttoi(GetValueRowCol(i, 1)))
			key = _ttoi(GetValueRowCol(i, 1));
	}

	return key;  
}

BOOL CDgnCreateRebarGrid::GridCheckData()
{	
	int RowCount = GetRowCount()-1;
	BOOL result = TRUE;
		
	for(int i=1 ; i<GetRowCount() ; i++)
	{
		result = bCheck_int(GetValueRowCol(i, 2));
		if(!result)
			return result;
		
		result = bCheck_double(GetValueRowCol(i, 4));
		if(!result)
			return result;

		result = bCheck_double(GetValueRowCol(i, 5));
		if(!result)
			return result;		

		result = bCheck_double(GetValueRowCol(i, 6));
		if(!result)
			return result;		
	}
	
	return TRUE;
}

BOOL CDgnCreateRebarGrid::bCheck_int(CString str)
{	
	int iLength = str.GetLength();
	for(int i=0; i<iLength; i++)
	{
		TCHAR ch = str.GetAt(i);

		if(ch=='0' || ch=='1' || ch=='2' || ch=='3' || ch=='4' || ch=='5' || 
			ch=='6' || ch=='7' || ch=='8' || ch=='9' || ch=='-')
		{
		}
		else
			return FALSE;
	}

	return TRUE;   
}

BOOL CDgnCreateRebarGrid::bCheck_double(CString str)
{	
	int iLength = str.GetLength();
	for(int i=0; i<iLength; i++)
	{
		TCHAR ch = str.GetAt(i);

		if(ch=='0' || ch=='1' || ch=='2' || ch=='3' || ch=='4' || ch=='5' || 
			ch=='6' || ch=='7' || ch=='8' || ch=='9' || ch=='-' || ch=='.' || ch=='e')
		{
		}
		else
			return FALSE;
	}

	if(str.Find(_T("ee")) >= 0)
		return FALSE;

	return TRUE; 
}


