#include "stdafx.h"
#include "wg_cmd.h"
#include "CMLoadDirectionGroupGrid.h"
#include "CMLoadDirectionGroupDlg.h"

#include "..\wg_base\wg_base_CompFunc.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_UnitCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define EGLD_KEY_FMT _T("%d")
/////////////////////////////////////////////////////////////////////////////
// CCMLoadDirectionGroupGrid

CString CCMLoadDirectionGroupGrid::m_aCurDefVal[] = {_T(""), _T(""), _T(""), _T(""),_T("")};
CCMLoadDirectionGroupGrid::CCMLoadDirectionGroupGrid()
{
	m_pParent = 0;
	m_nCurrentRow = 0;

	AddColInfo(_LS(IDS_WG_CMD__ADDD__No),               t_integer, 0);
	AddColInfo(_LS(IDS_WG_CMD__ADDD__Load_Case),        t_string,  0);
	AddColInfo(_LS(IDS_WG_CMD__ADDD__Angle),            t_integer, 0);
	AddColInfo(_LS(IDS_WG_CMD__ADDD__SYSTEM),           t_string,  0);
	AddColInfo(_LS(IDS_WG_CMD__ADDD__ELEM_LIST),        t_string,  0);   

	m_aPrimaryKey.Add(0);
	//m_aPrimaryKey.Add(1);
}

CCMLoadDirectionGroupGrid::~CCMLoadDirectionGroupGrid()
{
}

#define CWnd CTBCommon
BEGIN_MESSAGE_MAP(CCMLoadDirectionGroupGrid, CWnd)
#undef CWnd
	//{{AFX_MSG_MAP(CEgldGrid)
	ON_WM_RBUTTONDBLCLK()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CCMLoadDirectionGroupGrid::Initialize(CDBDoc* pDoc, CCMLoadDirectionGroupDlg* pParent)
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

	SetStyleRange(CGXRange().SetCols(1, 5), CGXStyle()
			.SetControl(GX_IDS_CTRL_STATIC)
			.SetHorizontalAlignment(DT_CENTER)
			.SetValueType(GX_VT_STRING)
			/*.SetEnabled(FALSE)*/
			.SetPlaces(0));
	SetStyleRange(CGXRange().SetCols(3, 3), CGXStyle()
			.SetValueType(GX_VT_NUMERIC));
	

	// Sorting 정보 초기화
	m_aSortInfo.SetSize(3);
	m_aSortInfo[0].nRC = 1; // LoadCase
	m_aSortInfo[0].sortType  = CGXSortInfo::alphanumeric;
	m_aSortInfo[0].sortOrder = CGXSortInfo::ascending;
	m_aSortInfo[1].nRC = 2; // Angle
	m_aSortInfo[1].sortType  = CGXSortInfo::numeric;
	m_aSortInfo[1].sortOrder = CGXSortInfo::ascending;
	m_aSortInfo[2].nRC = 0; // No
	m_aSortInfo[2].sortType  = CGXSortInfo::numeric;
	m_aSortInfo[2].sortOrder = CGXSortInfo::ascending;

	// always show vertical scrollbar
	SetScrollBarMode(SB_VERT, gxnEnabled | gxnEnhanced);  

	HideColsByIntend(1,1, TRUE);
	// Row 이동을 가능하게 한다.
//   GetParam()->EnableMoveRows(TRUE);

	// Record를 읽어서 채운다.
	MakeItemEx();

	// Display Only Mode로 설정한다. Insert, Delete Key Disable의 효과
	SetModeDisplayOnly(TRUE);

	LockUpdate(FALSE);
	Redraw();
}

BOOL CCMLoadDirectionGroupGrid::InsertRecordByKey(const T_EGLD_K &key, const T_EGLD_D &data)
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

BOOL CCMLoadDirectionGroupGrid::DeleteRecordByKey(const T_EGLD_K &key, const T_EGLD_D &data)
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
// CCMLoadDirectionGroupGrid Overridable Functions
void CCMLoadDirectionGroupGrid::OnChangedSelectionPublic(long *keys, int nSize)
{
	return;
}

long CCMLoadDirectionGroupGrid::GetSelectedKeys(CArray<long, long> &caKey)
{
	return 0;
}

BOOL CCMLoadDirectionGroupGrid::ValidateField(CString value, int nColID)
{
	return TRUE;
}

BOOL CCMLoadDirectionGroupGrid::ValidateRecord(CStringArray &value, CRowColArray &aCols)
{
	return TRUE;
}

BOOL CCMLoadDirectionGroupGrid::ValidateAndMakeRecord(CStringArray &value, CRowColArray& aCols, T_EGLD_K &key, T_EGLD_D &data)
{
	return TRUE;
}

BOOL CCMLoadDirectionGroupGrid::AddToDB(ROWCOL nRow)
{
	return TRUE;
}

BOOL CCMLoadDirectionGroupGrid::AddToDB(CArray<void*, void*>& raKey, CArray<void*, void*>& raData)
{
	return TRUE;
}


BOOL CCMLoadDirectionGroupGrid::DeleteFromDB(ROWCOL nRow)
{
	return TRUE;
}

BOOL CCMLoadDirectionGroupGrid::DeleteFromDB(CRowColArray& awRows)
{
	return TRUE;
}

BOOL CCMLoadDirectionGroupGrid::ModifyToDB(ROWCOL nRow)
{
	return TRUE;
}

BOOL CCMLoadDirectionGroupGrid::ModifyToDB(CStringArray& raOldKey, 
												 CArray<void*, void*>& raKey, 
												 CArray<void*, void*>& raData)
{
	return TRUE;
}


CString CCMLoadDirectionGroupGrid::GetNewKey()
{
	return CString(_T(""));
}

CString CCMLoadDirectionGroupGrid::GetDefValue(int nIndex)
{
	return m_aCurDefVal[nIndex];
}

CString* CCMLoadDirectionGroupGrid::GetDefValueArray()
{
	return m_aCurDefVal;
}

BOOL CCMLoadDirectionGroupGrid::MakeKeyAndDataArray(CStringArray& value, CRowColArray& awCols, 
																	CArray<void*, void*>& aKey, CArray<void*, void*>& aData)
{
	ASSERT(0);
	return TRUE;
}

void CCMLoadDirectionGroupGrid::DeleteKeyAndDataArray(CArray<void*, void*>& aKey, CArray<void*, void*>& aData)
{
	ASSERT(0);
}

long CCMLoadDirectionGroupGrid::GetCountData()
{
	CDBDoc* pDoc = m_pDoc;
//  return pDoc->m_pAttrCtrl->GetCountFbld();
	return pDoc->m_pAttrCtrl->GetCountEgld();
}

void CCMLoadDirectionGroupGrid::MakeItemFiltered(long nFilteredNum, long* aFilteredKeys, CRowColArray& awCols)
{
	ASSERT(0);
}

void CCMLoadDirectionGroupGrid::MakeItemNormal(long nNumData, CRowColArray& awCols)
{
	T_EGLD_K Key;
	T_EGLD_D Data;
	CDBDoc* pDoc = m_pDoc;

	long* KeyBuf = new long[nNumData];
	long nCount = 0;
	POSITION pos = pDoc->m_pAttrCtrl->GetStartEgld();
	while(pos != NULL)
	{
	  pDoc->m_pAttrCtrl->GetNextEgld(pos,Key,Data);
	  KeyBuf[nCount++] = Key;
	}
	qsort(KeyBuf, nNumData, sizeof(T_EGLD_K), CCompFunc::UINTAsc);

	for(nCount=0 ; nCount < nNumData; nCount++)
	{
		pDoc->m_pAttrCtrl->GetEgld(KeyBuf[nCount], Data);
		SetValue(GetParam()->GetData(), nCount+1, KeyBuf[nCount], Data, awCols);
	} // end of loop nCount

	delete []KeyBuf;
}

void CCMLoadDirectionGroupGrid::MakeItemEditingRecord(ROWCOL ncRow, CStringArray& EditingRecord)
{
	ASSERT(0);
}

/////////////////////////////////////////////////////////////////////////////
// CCMLoadDirectionGroupGrid Implementation
BOOL CCMLoadDirectionGroupGrid::GetValue(ROWCOL nRow, T_EGLD_K &key, T_EGLD_D &data)
{
	ASSERT(0);
	return TRUE;
}

BOOL CCMLoadDirectionGroupGrid::SetValue(ROWCOL nRow, const T_EGLD_K &key, const T_EGLD_D &data)
{
	CRowColArray awCols;
	awCols.SetSize(GetColCount());
	for (int i = 0;i < GetColCount(); i++)
		VERIFY(GetColumnNoFor(m_aTitle[i], awCols[i]));
	return SetValue(GetParam()->GetData(), nRow, key, data, awCols);
}

BOOL CCMLoadDirectionGroupGrid::SetValue(CGXData* pData, ROWCOL nRow, const T_EGLD_K &key, const T_EGLD_D &data, CRowColArray &awCols)
{
	CString str;
	for (int i = 0; i < GetColCount(); i++)
	{
		if(i==0) 
		{
			if (key == 0) str.Empty();
			else str.Format(EGLD_KEY_FMT, key);
		}
		else ConvDataToStr(i, data, str);
		pData->StoreValueRowCol(nRow, awCols[i], str, gxOverride);
	}

	return TRUE;
}

void CCMLoadDirectionGroupGrid::ConvStrToData(int i, CString& value, T_EGLD_D& data)
{
	ASSERT(0);
}

void CCMLoadDirectionGroupGrid::ConvDataToStr(int i, const T_EGLD_D& data, CString& value)
{
	if     (i==1) value = GetLoadCaseName(data.nLoadCaseType, data.StldKey);
	else if(i==2) value.Format(_T("%g"), data.dAngle);
	else if(i==3)
	{
		if(data.nSystemGrupType == EN_USER_GRUP) value = data.strUserGroup;
		else value = m_pDoc->m_pAttrCtrl->GetEglcGroupString(data.nSystemGrupType);
	}
	else if(i==4) 
	{
		CString SelectElemString;
		CString TempString;
		for(int i=0 ; i<data.arKeyElem.GetSize() ; i++)
		{
			if(SelectElemString.GetLength()==0)
				SelectElemString.Format(_T("%d"), data.arKeyElem[i]);
			else
			{
				TempString.Format(_T(" %d"), data.arKeyElem[i]);
				SelectElemString += TempString;
			}

			value = SelectElemString;
		}
	}
	else ASSERT(0);
}

CString CCMLoadDirectionGroupGrid::GetLoadCaseName(UINT nLoadCaseType, UINT StldKey)
{
	CString strLoadCaseName; strLoadCaseName.Empty();
	if(nLoadCaseType == D_LOADCASE_STATIC)
	{
		T_STLD_D StldD;
		if (!m_pDoc->m_pAttrCtrl->GetStld(StldKey, StldD)) { ASSERT(0); return strLoadCaseName; }
		strLoadCaseName = StldD.LoadCaseName;
	}
	else if(nLoadCaseType == D_LOADCASE_SPECTRUM)
	{
		T_SPLC_D SplcD;
		if (!m_pDoc->m_pAttrCtrl->GetSplc(StldKey, SplcD)) { ASSERT(0); return strLoadCaseName; }
		strLoadCaseName = SplcD.LoadCaseName;
	}
	
	return strLoadCaseName;
}
void CCMLoadDirectionGroupGrid::MakeSearchKey(const T_EGLD_K &key, const T_EGLD_D &data, CStringArray &aKey)
{
	int nColID;
	int nSortKeyNum = m_aSortInfo.GetSize();

	aKey.SetSize(nSortKeyNum);
	for (int i = 0; i < nSortKeyNum; i++)
	{
		nColID = m_aSortInfo[i].nRC;
		if (nColID == 0) aKey[i].Format(EGLD_KEY_FMT, key);
		else ConvDataToStr(nColID, data, aKey[i]);
	}
}

void CCMLoadDirectionGroupGrid::GetAllSelectedEgld(CArray<T_EGLD_K, T_EGLD_K>& aSelKey)
{
	return;
}

void CCMLoadDirectionGroupGrid::SetHeaderTitle(T_UNIT_SYSTEM &UnitSystem, BOOL bInit)
{
	// Set Title
	CString csTitle, csUnit;
	int aUnit[] = {0, 0, 0, 0, 0 };

	int aSize[] = {30, 80, 60, 120, 160};
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

//------------------------------------------------------------------------------>
// BOOL CCMLoadDirectionGroupGrid::OnSelDragRowsDrop(ROWCOL nFirstRow, ROWCOL nLastRow, ROWCOL nDestRow)
// {
//   ASSERT(!IsEdit());
// 
//   // Append Row의 뒤면 맨 마지막으로 이동하는 것으로 한다.
//   if (nDestRow == GetRowCount()+1) nDestRow--;  
// 
//   // 이동의 효과가 없는 곳으로 이동(선택범위내로 다시 이동)하면 그냥 리턴
//   if (nDestRow >= nFirstRow && nDestRow <= nLastRow+1) return FALSE;
// 
//   // 연속 선택이 아니면 이동을 금지시킨다.
//   CRowColArray awRows;
//   GetSelectedRows(awRows, TRUE, FALSE);
//   if (awRows.GetSize() != nLastRow-nFirstRow+1) 
//   {
//     AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Can_t_not_move_uncontinuous_recor));
//     return FALSE;
//   }
// 
//   int nStart, nInc;
//   nStart = (int)nDestRow;
//   if (nDestRow < nFirstRow)  // 선택 영역 앞으로 이동
//   {
//     nInc = 1;
//   }
//   else  // 선택 영역 뒤로 이동
//   {
//     nStart--;
//     nInc = 0;
//   }
//   
//   return FALSE;
// }
//<-----------------------------------------------------------------------------

//----------------------------------------------------------------------------->
// Current Record가 변경되었는지 검사
BOOL CCMLoadDirectionGroupGrid::CheckCurrentRecordChanged()
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

BOOL CCMLoadDirectionGroupGrid::ProcessKeys(CWnd *pSender, UINT nMessage, UINT nChar, UINT nRepCnt, UINT flags)
{
	BOOL bResult = CTBCommon::ProcessKeys(pSender, nMessage, nChar, nRepCnt, flags);
	CheckCurrentRecordChanged();
	return bResult;
}

void CCMLoadDirectionGroupGrid::OnRButtonDblClk(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	ROWCOL ncRowBefore, ncRowAfter, ncCol;
	if (!GetCurrentCell(ncRowBefore, ncCol)) return;

	CTBCommon::OnRButtonDblClk(nFlags, point);

	if (!GetCurrentCell(ncRowAfter, ncCol)) return;
	if (ncRowBefore != ncRowAfter) OnChangeCurrentRecord(ncRowAfter);
}

void CCMLoadDirectionGroupGrid::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CTBCommon::OnLButtonDblClk(nFlags, point);
	CheckCurrentRecordChanged();
}

void CCMLoadDirectionGroupGrid::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CTBCommon::OnLButtonDown(nFlags, point);
	CheckCurrentRecordChanged();
}

void CCMLoadDirectionGroupGrid::OnChangeCurrentRecord(ROWCOL nRow)
{
	m_nCurrentRow = nRow;

	// Append Row이면 그냥 리턴한다.
	if (nRow < 1) return;

	if (IsAppendRow(nRow)) 
	{
		m_pParent->OnChangeCurrentEGLD(0);
		return;
	}

	// Key값을 알아와서 Parent Dialog에 Row의 변동을 알린다.
	ROWCOL nkCol;
	GetColumnNoFor(GetKeyColID(), nkCol);
	T_EGLD_K nKey = _ttoi(GetValueRowCol(nRow, nkCol));
	m_pParent->OnChangeCurrentEGLD(nKey);
}
//<-----------------------------------------------------------------------------

BOOL CCMLoadDirectionGroupGrid::GetCurrentEGLD(T_EGLD_K& nKey)
{
	ROWCOL ncRow, ncCol;
	if (!GetCurrentCell(ncRow, ncCol)) return FALSE;
	if (ncRow < 1) return FALSE;
	if (IsAppendRow(ncRow)) return FALSE;

	if (!GetColumnNoFor(GetKeyColID(), ncCol)) return FALSE;
	nKey = _ttoi(GetValueRowCol(ncRow, ncCol));
	return TRUE;
}

BOOL CCMLoadDirectionGroupGrid::SetCurrentEGLD(T_EGLD_K key, T_EGLD_D& data)
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


