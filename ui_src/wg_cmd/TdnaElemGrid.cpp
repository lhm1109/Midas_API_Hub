// TdnaElemGrid.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "TdnaElemGrid.h"
#include "TdnaElemGrid.h"

#include "..\wg_base\wg_base_CompFunc.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "CMTendonProfileCopyDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define TDNA_KEY_FMT _T("%d")
/////////////////////////////////////////////////////////////////////////////
// CTdnaElemGrid
CString CTdnaElemGrid::m_aCurDefVal[] = {_T(""), _T(""), _T("")};

CTdnaElemGrid::CTdnaElemGrid()
{
	m_pParent = 0;
	m_nCurrentRow = 0;
	AddColInfo(_LS(IDS_WG_CMD__ADDD__No),  t_integer, 0);
	AddColInfo(_LS(IDS_CMD__ADDD__Assigned_Elements),  t_string,  0);
	AddColInfo(_LS(IDS_CMD__ADDD__Insertion), t_string,  0);

	m_aPrimaryKey.Add(0);
}

CTdnaElemGrid::~CTdnaElemGrid()
{
}

#define CWnd CTBCommon
BEGIN_MESSAGE_MAP(CTdnaElemGrid, CWnd)
#undef CWnd
	//{{AFX_MSG_MAP(CTdnaElemGrid)
	ON_WM_RBUTTONDBLCLK()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CTdnaElemGrid message handlers
void CTdnaElemGrid::Initialize(CDBDoc* pDoc, CDialogMove* pParent)
{
	m_pParent = pParent;

	T_UNIT_SYSTEM UnitSystem; // not used, but compatibility
	CTBCommon::Initialize(pDoc, UnitSystem);

	LockUpdate(TRUE);

	// 표준 스타일 변경
	ChangeStandardStyle(CGXStyle()
			.SetFont(CGXFont_GC().SetSize(9))
			.SetVerticalAlignment(DT_BOTTOM)
			.SetAllowEnter(FALSE));

	// 컬럼 헤더 스타일 변경
	ChangeColHeaderStyle(CGXStyle()
			.SetFont(CGXFont_GC().SetBold(FALSE)));

	// 컬럼 스타일 변경
	SetStyleRange(CGXRange().SetCols(1, 3), CGXStyle()
			.SetControl(GX_IDS_CTRL_STATIC));
	SetStyleRange(CGXRange().SetCols(1, 1), CGXStyle()
			.SetHorizontalAlignment(DT_LEFT)
			.SetValueType(GX_VT_NUMERIC)
			.SetEnabled(FALSE)
			.SetPlaces(0));
	SetStyleRange(CGXRange().SetCols(2, 3), CGXStyle()
			.SetHorizontalAlignment(DT_LEFT)
			.SetValueType(GX_VT_STRING));

	// Key Field도 숨긴다.
	//HideColsByIntend(1, 1, TRUE);
	//bbong
	SetRightValidCell(1, 1);

	// Sorting 정보 초기화
	m_aSortInfo.SetSize(1);
	m_aSortInfo[0].nRC = 0;   m_aSortInfo[0].sortType = CGXSortInfo::numeric;
	m_aSortInfo[0].sortOrder = CGXSortInfo::ascending;


	// always show vertical scrollbar
	SetScrollBarMode(SB_VERT, gxnEnabled | gxnEnhanced);  

	// Row 이동을 가능하게 한다.
	GetParam()->EnableMoveRows(TRUE);

	// Record를 읽어서 채운다.
	MakeItemEx();

	// Display Only Mode로 설정한다. Insert, Delete Key Disable의 효과
	SetModeDisplayOnly(TRUE);

	LockUpdate(FALSE);
	Redraw();
}

BOOL CTdnaElemGrid::InsertRecordByKey(const int &key, const T_TDNA_ELEM_D &data)
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

BOOL CTdnaElemGrid::DeleteRecordByKey(const int &key, const T_TDNA_ELEM_D &data)
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
BOOL CTdnaElemGrid::ModifyRecordByKey(const int &key,const T_TDNA_ELEM_D &data)
{
	return SetValue(key,key,data);     
}
/////////////////////////////////////////////////////////////////////////////
// CTdnaElemGrid Overridable Functions
void CTdnaElemGrid::OnChangedSelectionPublic(long *keys, int nSize)
{
	return;
}

long CTdnaElemGrid::GetSelectedKeys(CArray<long, long> &caKey)
{
	return 0;
}

BOOL CTdnaElemGrid::ValidateField(CString value, int nColID)
{
	return TRUE;
}

BOOL CTdnaElemGrid::ValidateRecord(CStringArray &value, CRowColArray &aCols)
{
	return TRUE;
}

BOOL CTdnaElemGrid::ValidateAndMakeRecord(CStringArray &value, CRowColArray& aCols, int &key, T_TDNA_ELEM_D &data)
{
	return TRUE;
}

BOOL CTdnaElemGrid::AddToDB(ROWCOL nRow)
{
	return TRUE;
}

BOOL CTdnaElemGrid::AddToDB(CArray<void*, void*>& raKey, CArray<void*, void*>& raData)
{
	return TRUE;
}


BOOL CTdnaElemGrid::DeleteFromDB(ROWCOL nRow)
{
	return TRUE;
}

BOOL CTdnaElemGrid::DeleteFromDB(CRowColArray& awRows)
{
	return TRUE;
}

BOOL CTdnaElemGrid::ModifyToDB(ROWCOL nRow)
{
	return TRUE;
}

BOOL CTdnaElemGrid::ModifyToDB(CStringArray& raOldKey, 
												 CArray<void*, void*>& raKey, 
												 CArray<void*, void*>& raData)
{
	return TRUE;
}


CString CTdnaElemGrid::GetNewKey()
{
	return CString(_T(""));
}

CString CTdnaElemGrid::GetDefValue(int nIndex)
{
	return m_aCurDefVal[nIndex];
}

CString* CTdnaElemGrid::GetDefValueArray()
{
	return m_aCurDefVal;
}

BOOL CTdnaElemGrid::MakeKeyAndDataArray(CStringArray& value, CRowColArray& awCols, 
																	CArray<void*, void*>& aKey, CArray<void*, void*>& aData)
{
	ASSERT(0);
	return TRUE;
}

void CTdnaElemGrid::DeleteKeyAndDataArray(CArray<void*, void*>& aKey, CArray<void*, void*>& aData)
{
	ASSERT(0);
}

long CTdnaElemGrid::GetCountData()
{
	CDBDoc* pDoc = m_pDoc;
	return ((CCMTendonProfileCopyDlg*)m_pParent)->m_aElems.GetSize();
}

void CTdnaElemGrid::MakeItemFiltered(long nFilteredNum, long* aFilteredKeys, CRowColArray& awCols)
{
	ASSERT(0);
}

void CTdnaElemGrid::MakeItemNormal(long nNumData, CRowColArray& awCols)
{
	T_TDNA_ELEM_D Data;
	CDBDoc* pDoc = m_pDoc;

	//long* KeyBuf = new long[nNumData];
	long nCount = 0;
	for(int i=0;i<((CCMTendonProfileCopyDlg*)m_pParent)->m_aElems.GetSize();i++)
	{
		Data=((CCMTendonProfileCopyDlg*)m_pParent)->m_aElems[i];
		SetValue(GetParam()->GetData(), i+1, i+1, Data, awCols);
	} 
}

void CTdnaElemGrid::MakeItemEditingRecord(ROWCOL ncRow, CStringArray& EditingRecord)
{
	ASSERT(0);
}

/////////////////////////////////////////////////////////////////////////////
// CTdnaElemGrid Implementation
BOOL CTdnaElemGrid::GetValue(ROWCOL nRow, int &key, T_TDNA_ELEM_D &data)
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

		if(i==0) key  = _ttoi(str);
		else ConvStrToData(i, str, data);
	}
	return TRUE;
}

BOOL CTdnaElemGrid::SetValue(ROWCOL nRow, const int &key, const T_TDNA_ELEM_D &data)
{
	CRowColArray awCols;
	awCols.SetSize(GetColCount());
	for (int i = 0;i < GetColCount(); i++)
		VERIFY(GetColumnNoFor(m_aTitle[i], awCols[i]));
	return SetValue(GetParam()->GetData(), nRow, key, data, awCols);
}

BOOL CTdnaElemGrid::SetValue(CGXData* pData, ROWCOL nRow, const int &key, const T_TDNA_ELEM_D &data, CRowColArray &awCols)
{
	CString str;
	for (int i = 0; i < GetColCount(); i++)
	{
		if(i==0) 
		{
			if (key == 0) str.Empty();
			else str.Format(_T("%d"), key);
		}
		else ConvDataToStr(i, data, str);
		pData->StoreValueRowCol(nRow, awCols[i], str, gxOverride);
	}

	return TRUE;
}

void CTdnaElemGrid::ConvStrToData(int i, CString& value, T_TDNA_ELEM_D& data)
{
	if(i==1) data.strAssigned=value;
	else if(i==2) data.nInsertion=_ttoi(value);
}

void CTdnaElemGrid::ConvDataToStr(int i, const T_TDNA_ELEM_D& data, CString& value)
{
	if (i==1) value=data.strAssigned;
	else if (i==2) value.Format(_T("%d"),data.nInsertion);
}

void CTdnaElemGrid::MakeSearchKey(const int &key, const T_TDNA_ELEM_D &data, CStringArray &aKey)
{
	int nColID;
	int nSortKeyNum = m_aSortInfo.GetSize();

	aKey.SetSize(nSortKeyNum);
	for (int i = 0; i < nSortKeyNum; i++)
	{
		nColID = m_aSortInfo[i].nRC;
		if (nColID == 0) aKey[i].Format(_T("%d"), key);
		else ConvDataToStr(nColID, data, aKey[i]);
	}
}

void CTdnaElemGrid::GetAllSelectedFbld(CArray<int, int>& aSelKey)
{
	return;
}

void CTdnaElemGrid::SetHeaderTitle(T_UNIT_SYSTEM &UnitSystem, BOOL bInit)
{
	// Set Title
	CString csTitle, csUnit;
	int aUnit[] = {0, 0, 0};

	int aSize[] = {40, 100, 60};
	int nColCount = GetColCount();

	for (int i = 0; i < nColCount; i++)
	{
		CUnitCtrl::GetUnitSystem(aUnit[i], csUnit);
		if (csUnit.IsEmpty()) csTitle = m_aTitle[i];
		else csTitle = m_aTitle[i] + _T("\n(")+csUnit+_T(")");
		SetStyleRange(CGXRange(0, i+1), CGXStyle()
			.SetValue(csTitle)
			.SetUserAttribute(ID_USER_ATTR_COL_ID, m_aTitle[i]));
		if (bInit) SetColWidthByDPI(i+1, i+1, aSize[i]);
	}
}


// Current Record가 변경되었는지 검사
BOOL CTdnaElemGrid::CheckCurrentRecordChanged()
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

BOOL CTdnaElemGrid::ProcessKeys(CWnd *pSender, UINT nMessage, UINT nChar, UINT nRepCnt, UINT flags)
{
	BOOL bResult = CTBCommon::ProcessKeys(pSender, nMessage, nChar, nRepCnt, flags);
	CheckCurrentRecordChanged();
	return bResult;
}

void CTdnaElemGrid::OnRButtonDblClk(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	ROWCOL ncRowBefore, ncRowAfter, ncCol;
	if (!GetCurrentCell(ncRowBefore, ncCol)) return;

	CTBCommon::OnRButtonDblClk(nFlags, point);

	if (!GetCurrentCell(ncRowAfter, ncCol)) return;
	if (ncRowBefore != ncRowAfter) OnChangeCurrentRecord(ncRowAfter);
}

void CTdnaElemGrid::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CTBCommon::OnLButtonDblClk(nFlags, point);
	CheckCurrentRecordChanged();
}

void CTdnaElemGrid::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CTBCommon::OnLButtonDown(nFlags, point);
	CheckCurrentRecordChanged();
}

void CTdnaElemGrid::OnChangeCurrentRecord(ROWCOL nRow)
{
	m_nCurrentRow = nRow;

	// Append Row이면 그냥 리턴한다.
	if (nRow < 1) return;
	if (IsAppendRow(nRow)) 
	{
		//m_pParent->OnChangeCurrentFbld(0);
		return;
	}

	// Key값을 알아와서 Parent Dialog에 Row의 변동을 알린다.
	ROWCOL nkCol;
	GetColumnNoFor(GetKeyColID(), nkCol);
	int nKey = _ttoi(GetValueRowCol(nRow, nkCol));
	((CCMTendonProfileCopyDlg*)m_pParent)->OnChangeCurrentElem(nKey);
}
//<-----------------------------------------------------------------------------

BOOL CTdnaElemGrid::GetCurrentElem(int& nKey)
{
	ROWCOL ncRow, ncCol;
	if (!GetCurrentCell(ncRow, ncCol)) return FALSE;
	if (ncRow < 1) return FALSE;
	if (IsAppendRow(ncRow)) return FALSE;

	if (!GetColumnNoFor(GetKeyColID(), ncCol)) return FALSE;
	nKey = _ttoi(GetValueRowCol(ncRow, ncCol));
	return TRUE;
}

BOOL CTdnaElemGrid::SetCurrentElem(int key, T_TDNA_ELEM_D& data)
{
	ROWCOL nRow, nCol;

	if (!GetCurrentCell(nRow, nCol)) return FALSE;
	
	CStringArray aKey;
	MakeSearchKey(key, data, aKey);
	if (!FindRecordByKey(aKey, nRow)) return FALSE;
	SetCurrentCell(nRow, nCol);
	//CheckCurrentRecordChanged();
	return TRUE;
}

void CTdnaElemGrid::ReStoreKeys()
{
	int nCount;
	nCount=GetRowCount()-1;
	T_TDNA_ELEM_D tempData;
	int nTempKey;
	for(int i=1;i<=nCount;i++)
	{
		GetValue(i,nTempKey,tempData);
		SetValue(i,i,tempData);    
	}

}
