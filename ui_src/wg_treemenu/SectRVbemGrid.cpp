// MeshRatioList.cpp : implementation file
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SectRVbemGrid.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl2.h"

#include "..\wg_base\CompFunc.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

UINT CSectRVbemGrid::WM_SECT_RFORC_GRID_VBEM_CLICKED = RegisterWindowMessage(_T("WM_SECT_RFORC_GRID_VBEM_CLICKED"));

/////////////////////////////////////////////////////////////////////////////
// CSectRVbemGrid
CString CSectRVbemGrid::m_aCurDefVal[] = { _T("-1") ,_T("0"), _T("0") };
CSectRVbemGrid::CSectRVbemGrid()
{
	Init_Construction();
}

CSectRVbemGrid::CSectRVbemGrid(CWnd* pParent)
{
	Init_Construction();
}

CSectRVbemGrid::~CSectRVbemGrid()
{
}

void CSectRVbemGrid::Init_Construction()
{
	T_VBEM_UNIT * pUnit = &(CUnitCtrl::m_VBEM_UNIT);

	AddColInfo(_T("ID"),      t_integer, 0); // Key.
	AddColInfo(_LS(IDS_TM_SECT_RVBEM_I_end   ),      t_integer,  D_UNITSYS_NONE); // Y
	AddColInfo(_LS(IDS_TM_SECT_RVBEM_J_end   ),      t_integer,  D_UNITSYS_NONE); // Z
	AddColInfo(_LS(IDS_TM_SECT_RVBEM_VIRTUAL_SECTION), t_integer, D_UNITSYS_NONE); // Z

	m_aPrimaryKey.Add(0);
}

BEGIN_MESSAGE_MAP(CSectRVbemGrid, CTBCommon)
	//{{AFX_MSG_MAP(CSectRVbemGrid)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CSectRVbemGrid message handlers

void CSectRVbemGrid::Initialize()
{
	T_UNIT_SYSTEM us;
	CDBDoc *pDoc = CDBDoc::GetDocPoint();
	CTBCommon::Initialize(pDoc, us);

	LockUpdate(TRUE);

	SetHeaderTitle(us, TRUE);
	SetRowHeight(0, 0, globalUtils.ScaleByDPI(20));

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
	//GetParam()->SetNumberedRowHeaders(TRUE);

	// 컬럼 스타일 변경
	SetStyleRange(CGXRange().SetCols(1, GetInitialColCount()), CGXStyle()     // 전체 컬럼의 스타일 변경 
			.SetHorizontalAlignment(DT_CENTER)
			.SetValueType(GX_VT_NUMERIC)
			.SetFormat(GX_FMT_FIXED));

	SetStyleRange(CGXRange().SetCols(1), CGXStyle()         // 1번째 컬럼 Key 
			.SetPlaces(0)
			.SetEnabled(FALSE));
	//HideColsByIntend(1, 1, TRUE);

	SetStyleRange(CGXRange().SetCols(2,3), CGXStyle()         // 2,3번째 컬럼 I-end, J-end
			.SetPlaces(0)
			.SetEnabled(TRUE));

	SetStyleRange(CGXRange().SetCols(4), CGXStyle()         // 4번째 컬럼 Virtual Section
		.SetPlaces(0)
		.SetEnabled(TRUE));
	
	SetColumnAttr();

//   // 현재 셀 설정
//   GetParam()->SetHideCurrentCell(GX_HIDE_NEVER);
	GetParam()->SetEnterKeyAction(GX_DOWN);
	EnableInsertRowByPressingKey(FALSE);  // Disable inserting row by key

	// Row 헤더와 Column 헤더를 숨긴다.
	//HideRows(0, 0);
	//HideCols(0, 0);


//   // Sorting 정보 초기화
//   m_aSortInfo.SetSize(2);
//   m_aSortInfo[0].nRC = 1;
//   m_aSortInfo[0].sortType = CGXSortInfo::numeric;
//   m_aSortInfo[0].sortOrder = CGXSortInfo::ascending;
//   m_aSortInfo[1].nRC = 2;
//   m_aSortInfo[1].sortType = CGXSortInfo::alphanumeric;
//   m_aSortInfo[1].sortOrder = CGXSortInfo::ascending;

	MakeItemEx();

	//SetScrollBarMode(SB_VERT, gxnEnabled | gxnEnhanced);
	SetScrollBarMode(SB_BOTH, gxnAutomatic | gxnEnhanced);
	this->EnableInsertRowByPressingKey(FALSE);  // Insert Key에 의한 Record 삽입 금지

	LockUpdate(FALSE);
}

BOOL CSectRVbemGrid::InsertRecordByKey(const int &key, const T_VBEM_D &data)
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

BOOL CSectRVbemGrid::DeleteRecordByKey(const int &key, const T_VBEM_D &data)
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
// CSectRVbemGrid Overridable Functions
void CSectRVbemGrid::OnChangedSelectionPublic(long *keys, int nSize)
{
	return;
}

long CSectRVbemGrid::GetSelectedKeys(CArray<long, long> &caKey)
{
	return 0;
}

int CSectRVbemGrid::GetSelectedVbemKList(CArray<UINT, UINT>& aVbemK)
{
	aVbemK.RemoveAll();

	// get key
	ROWCOL nkCol;
	GetColumnNoFor(GetKeyColID(), nkCol);

	CRowColArray awRows;
	ROWCOL nSel = GetSelectedRows(awRows);
	for (int i=0; i<nSel; ++i)
	{
		T_VBEM_K VbemK = _ttol(GetValueRowCol(awRows[i], nkCol));
		if (VbemK == 0)  continue;

		aVbemK.Add(VbemK);
	}

	return aVbemK.GetSize();
}

BOOL CSectRVbemGrid::OnLButtonClickedRowCol(ROWCOL nRow, ROWCOL nCol, UINT nFlags, CPoint pt)
{
	BOOL bRet = CTBCommon::OnLButtonClickedRowCol(nRow, nCol, nFlags, pt);

	// get key
	ROWCOL nkCol;
	GetColumnNoFor(GetKeyColID(), nkCol);
	T_VBEM_K VbemK = _ttol(GetValueRowCol(nRow, nkCol));

	// TODO: Add your control notification handler code here
	CWnd* pParentWnd = GetParent();
	if(pParentWnd && pParentWnd->GetSafeHwnd())   
		pParentWnd->PostMessage(CSectRVbemGrid::WM_SECT_RFORC_GRID_VBEM_CLICKED, (WPARAM)VbemK, (LPARAM)0);	

	return bRet;
}

BOOL CSectRVbemGrid::ValidateField(CString value, int nColID)
{
	if (nColID >= GetColCount())
	{
		SetWarningText(_LS(IDS_LBC_ERR_OUT_OF_RANGE));
		return FALSE;
	}

	int nErrCode;

	if (nColID == 0) return TRUE;   // ID : error check 없음
	else if( nColID == 1 || nColID == 2)           // I,J-end
	{
		if (!IsValidLongNumber(value, nErrCode)) return InvalidLongNumber(nErrCode);
	}

	return TRUE;
}

// 삽입이나 수정전에 Record의 Validation을 검사한다.
BOOL CSectRVbemGrid::ValidateRecord(CStringArray &value, CRowColArray &aCols)
{//?
	CString msg = _T("");
	int nPos;

	for (int i = 0; i < GetColCount(); i++)
	{
		if (i == 0) continue;
		nPos = aCols[i]-1;
 
		if (value[nPos] == _T("")) value[nPos] = _T("0."); // customized! (Y.Jee)
			
		else if (!ValidateField(value[nPos], i)) return FALSE;
	}
	if (msg == _T("")) return TRUE;
	else SetWarningText(msg);
	return FALSE;
 
	return TRUE;
}

// 이 함수는 Paste할 때 호출되므로 False를 리턴할 때는 ValidateField와 
// Validate Record에서 설정된 Warning Text를 없애주고 리턴한다.
BOOL CSectRVbemGrid::ValidateAndMakeRecord(CStringArray &value, CRowColArray& aCols, int &key, T_VBEM_D &data)
{
	// 레코드에 대해 Validation을 검사한다.
	if (!ValidateRecord(value, aCols))
		return FALSE;

	// 레코드를 만들어서 리턴한다.
	int nPos;
	for (int i = 0; i < GetColCount(); i++)
	{
		nPos = aCols[i]-1; // col id i에 해당하는 value가 저장된 위치
		if(i==0) key = _ttoi(value[nPos]);
		else ConvStrToData(i, value[nPos], data);
	}
	return TRUE;
}

BOOL CSectRVbemGrid::AddToDB(ROWCOL nRow)
{
	if (!CanSaveData()) return FALSE;

	// Record를 만들어서 DB에 입력한다.
	T_VBEM_K key;
	T_VBEM_D data;

	GetValue(nRow, key, data);
	return m_pDoc->m_pDataCtrl->AddVbem(key, data);  
}

BOOL CSectRVbemGrid::AddToDB(CArray<void*, void*>& raKey, CArray<void*, void*>& raData)
{
	ASSERT(raKey.GetSize() == raData.GetSize());
	T_VBEM_K tKey;
	CArray<T_VBEM_K, T_VBEM_K> aKey;
	CArray<T_VBEM_D, T_VBEM_D&> aData;
	T_VBEM_K VbemK_new = m_pDoc->m_pAttrCtrl2->GetLastNumVbem()+1;
	for (int i = 0; i < raKey.GetSize(); i++)
	{
		tKey = *((T_VBEM_K*)raKey.GetAt(i));

		// 여러 데이터를 한꺼번에 추가
		if (!m_pDoc->m_pAttrCtrl2->ExistVbem(tKey)) { tKey = VbemK_new;  VbemK_new++; }

		aKey.Add(tKey);
		aData.Add(*((T_VBEM_D*)raData.GetAt(i)));
	}
	return m_pDoc->m_pDataCtrl->AddVbem(aKey, aData);
}

BOOL CSectRVbemGrid::DeleteFromDB(ROWCOL nRow)
{
	T_VBEM_K key;
	T_VBEM_D data;

	GetValue(nRow, key, data);
	return m_pDoc->m_pDataCtrl->DelVbem(key);
}

BOOL CSectRVbemGrid::DeleteFromDB(CRowColArray& awRows)
{
	if (awRows.GetSize() <= 0) return TRUE;

	T_VBEM_K key;
	T_VBEM_D data;
	CArray<T_VBEM_K, T_VBEM_K> awKeys;  awKeys.RemoveAll();

	for (int i = 0; i < awRows.GetSize(); i++)
	{
		GetValue(awRows.GetAt(i), key, data);
		awKeys.Add(key);
	}

	return m_pDoc->m_pDataCtrl->DelVbem(awKeys);
}

BOOL CSectRVbemGrid::ModifyToDB(ROWCOL nRow)
{
	if (!CanSaveData()) return FALSE;

	T_VBEM_K key;
	T_VBEM_D data;
	GetValue(nRow, key, data);

	return m_pDoc->m_pDataCtrl->ModifyVbem(key, key, data);
}

BOOL CSectRVbemGrid::ModifyToDB(CStringArray& raOldKey, 
												 CArray<void*, void*>& raKey, 
												 CArray<void*, void*>& raData)
{
	ASSERT(raKey.GetSize() == raData.GetSize());
	ASSERT(raKey.GetSize() == raOldKey.GetSize());

	CArray<T_VBEM_K,T_VBEM_K> aVbemK_old;   aVbemK_old.RemoveAll();
	T_VBEM_K key;
	T_VBEM_D data;
	CArray<T_VBEM_D, T_VBEM_D&> aData;
	for (int i = 0; i < raKey.GetSize(); i++)
	{
		key = _ttol(raOldKey.GetAt(i));
		m_pDoc->m_pAttrCtrl2->GetVbem(key, data);
		aVbemK_old.Add(key);

		T_VBEM_D TempVbem2 = *((T_VBEM_D*)raData.GetAt(i));
		aData.Add(*((T_VBEM_D*)raData.GetAt(i)));
	}

	return m_pDoc->m_pDataCtrl->ModifyVbem(aVbemK_old, aVbemK_old, aData);
}

CString CSectRVbemGrid::GetNewKey()
{
	CString value = _T("");
	T_VBEM_K key = m_pDoc->m_pAttrCtrl2->GetStartNumVbem();
	value.Format(_T("%d"), key);
	return value;
}

CString CSectRVbemGrid::GetDefValue(int nIndex)
{
	return m_aCurDefVal[nIndex];
}

CString* CSectRVbemGrid::GetDefValueArray()
{
	return m_aCurDefVal;
}

BOOL CSectRVbemGrid::MakeKeyAndDataArray(CStringArray& value, CRowColArray& awCols, 
																	CArray<void*, void*>& aKey, CArray<void*, void*>& aData)
{
	int* pKey;
	T_VBEM_D* pData;
	pKey = new int;
	pData = new T_VBEM_D;
	aKey.Add((int*)pKey);
	aData.Add((T_VBEM_D*)pData);
	if (!ValidateAndMakeRecord(value, awCols, *pKey, *pData)) return FALSE;
	return TRUE;
}

void CSectRVbemGrid::DeleteKeyAndDataArray(CArray<void*, void*>& aKey, CArray<void*, void*>& aData)
{
	int* pKey;
	T_VBEM_D* pData;
	ASSERT(aKey.GetSize() == aData.GetSize());
	for (int i = 0; i < aKey.GetSize(); i++)
	{
		pKey = (int*)aKey.GetAt(i);
		pData = (T_VBEM_D*)aData.GetAt(i);
		delete pKey;
		delete pData;
	}
}

long CSectRVbemGrid::GetCountData()
{
	CDBDoc* pDoc = m_pDoc;
	return pDoc->m_pAttrCtrl2->GetCountVbem();
}

void CSectRVbemGrid::MakeItemFiltered(long nFilteredNum, long* aFilteredKeys, CRowColArray& awCols)
{
	CDBDoc* pDoc = m_pDoc;
	T_VBEM_D Data;

	for(long nCount=0 ; nCount < nFilteredNum; nCount++)
	{
		pDoc->m_pAttrCtrl2->GetVbem(aFilteredKeys[nCount], Data);
		SetValue(GetParam()->GetData(), nCount+1, aFilteredKeys[nCount], Data, awCols);
	}
}

void CSectRVbemGrid::MakeItemNormal(long nNumData, CRowColArray& awCols)
{
	T_VBEM_K Key;
	T_VBEM_D Data;
	CDBDoc* pDoc = m_pDoc;

	long* KeyBuf = new long[nNumData];
	long nCount = 0;
	POSITION pos = pDoc->m_pAttrCtrl2->GetStartVbem();
	while(pos != NULL)
	{
		pDoc->m_pAttrCtrl2->GetNextVbem(pos,Key,Data);
		KeyBuf[nCount++] = Key;
	}
	qsort(KeyBuf, nNumData, sizeof(T_VBEM_K), CCompFunc::UINTAsc);

	for(nCount=0 ; nCount < nNumData; nCount++)
	{
		pDoc->m_pAttrCtrl2->GetVbem(KeyBuf[nCount], Data);
		SetValue(GetParam()->GetData(), nCount+1, KeyBuf[nCount], Data, awCols);
	} // end of loop nCount

	delete []KeyBuf;
}

void CSectRVbemGrid::MakeItemEditingRecord(ROWCOL ncRow, CStringArray& EditingRecord)
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	int Key;
	T_VBEM_D Data;
	for (int i = 0; i < GetColCount(); i++)
	{
		if (i == 0) Key = _ttoi(EditingRecord[i]);
		else ConvStrToData(i, EditingRecord[i], Data);
	}
	pDoc->m_pUnitCtrl->ConvertUnitVbemPrevious(Data);
	// 현재 값을 설정한다.
	SetValue(ncRow, Key, Data);
}

/////////////////////////////////////////////////////////////////////////////
// CSectRVbemGrid Implementation
BOOL CSectRVbemGrid::GetValue(ROWCOL nRow, UINT &key, T_VBEM_D &data)
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
			key = _ttol(str);
			if (!m_pDoc->m_pAttrCtrl2->GetVbem(key, data))   
			{
				key = m_pDoc->m_pAttrCtrl2->GetLastNumVbem()+1;
				data.Initialize();
			}
		}
		else ConvStrToData(i, str, data);
	}
	return TRUE;
}

BOOL CSectRVbemGrid::SetValue(ROWCOL nRow, const UINT &key, const T_VBEM_D &data)
{
	//
	CRowColArray awCols;
	awCols.SetSize(GetColCount());
	for (int i = 0;i < GetColCount(); i++)
		VERIFY(GetColumnNoFor(m_aTitle[i], awCols[i]));
	return SetValue(GetParam()->GetData(), nRow, key, data, awCols);
}

BOOL CSectRVbemGrid::SetValue(CGXData* pData, ROWCOL nRow, const UINT &key, const T_VBEM_D &data, CRowColArray &awCols)
{
	//
	CString str;
	int nValueType;
	for (int i = 0; i < GetColCount(); i++)
	{
		if(i==0) 
		{
			if (key == 0) str.Empty();
			else str.Format(_T("%d"), key);
		}
		else ConvDataToStr(i, data, str, key);

		if (m_aType[i] == t_string) nValueType = GX_VT_STRING;
		else nValueType = GX_VT_NUMERIC;
		pData->StoreValueRowCol(nRow, awCols[i], str, gxOverride, nValueType);
	}

	return TRUE;
}

void CSectRVbemGrid::ConvStrToData(int i, CString& value, T_VBEM_D& data)
{
	if (i == 1) data.Vsec1 = _ttoi(value);    // I-end
	else if (i == 2) data.Vsec2 = _ttoi(value);  // J-end
	else ASSERT(0);
}

void CSectRVbemGrid::ConvDataToStr(int i, const T_VBEM_D& data, CString& value, const T_VBEM_K VbemK)
{
	if (i == 1) value.Format(_T("%d"), data.Vsec1);    // I-end
	else if (i == 2) value.Format(_T("%d"), data.Vsec2);    // J-end
	else if (i == 3) {
		T_SECV_D VecData;
		if(m_pDoc->m_pAttrCtrl2->GetSecv(VbemK, VecData))
		{
			value = VecData.SName;
		}
	    else
	    {
		value = _T(" ");
	    }
	}
	else ASSERT(0);
}

void CSectRVbemGrid::MakeSearchKey(const int &key, const T_VBEM_D &data, CStringArray &aKey)
{
	int nColID;
	int nSortKeyNum = m_aSortInfo.GetSize();

	aKey.SetSize(nSortKeyNum);
	for (int i = 0; i < nSortKeyNum; i++)
	{
		nColID = m_aSortInfo[i].nRC;
		if (nColID == 0)
		{
			if (key == 0) aKey[i] = GetNewKey();
			aKey[i].Format(_T("%d"), key);
		}
		else ConvDataToStr(nColID, data, aKey[i], key);
	}
}

void CSectRVbemGrid::SetHeaderTitle(T_UNIT_SYSTEM &us, BOOL bInit)
{
	SetHeaderTitle2((int*)m_aUnit.GetData(), bInit, TRUE);
}

// Column 폭을 지정한다.
// Hide 시켜야 할 칼럼을 지정한다.
void CSectRVbemGrid::SetColumnAttr()
{
	int anColWidth[] = {30, 65, 65};

	for (int i = 0; i < sizeof(anColWidth)/sizeof(int); i++)    SetColWidthByDPI(i+1, i+1, anColWidth[i]);
}

void CSectRVbemGrid::UpdateBuffer()
{
	MakeItemEx();
}

void CSectRVbemGrid::ConvertUnit()
{
	MakeItemEx();
}

// MNET:XXXX 20080117 mylee - Unit 변경시 회전 자유도를 길이처럼 변경시켜 주기 위해
void CSectRVbemGrid::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
	HWND hWnd;
	if ((hWnd = GetSafeHwnd()) == 0 || !IsWindow(hWnd)) return;

	switch(lHint)
	{
	case D_UPDATE_DBALL:
	case D_UPDATE_DEFAULT:
		UpdateBuffer();
		break;
	case D_UPDATE_BUFFER_BEFORE:
		break;
	case D_UPDATE_BUFFER_AFTER:
		UpdateBuffer();
		break;
	case D_UPDATE_UNIT:
		ConvertUnit();
		break;
	case D_UPDATE_SEL_ADD: case D_UPDATE_SEL_DEL:
		break;
	case D_UPDATE_EXE_START: case D_UPDATE_EXE_END:
		break;
	default:
		//ASSERT(FALSE);
		break;
	}
}
