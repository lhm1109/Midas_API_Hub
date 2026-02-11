// PsltGrid.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "PsltGrid.h"
#include "CMPsltDlg.h"

#include "..\wg_base\wg_base_CompFunc.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_AttrCtrl2.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_UnitCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define PSLT_KEY_FMT _T("%d")
/////////////////////////////////////////////////////////////////////////////
// CPsltGrid
CString CPsltGrid::m_aCurDefVal[] = {_T(""), _T(""), _T(""), _T("")};

CPsltGrid::CPsltGrid()
{
	m_pParent = 0;
	m_nCurrentRow = 0;

	AddColInfo(_LS(IDS_WG_CMD__ADDD__No),             t_integer, 0);
	AddColInfo(_LS(IDS_WG_CMD__ADDD__Sno),            t_integer, 0);
	AddColInfo(_LS(IDS_WG_CMD__ADDD__Name),           t_string,  0);
	AddColInfo(_LS(IDS_WG_CMD_Element_Type),          t_string,  0);
	AddColInfo(_LS(IDS_WG_CMD__ADDD__Description),    t_string,  0);
#if defined (_MGEN_CH)
	AddColInfo(_LS(IDS_WG_CMD__ADDD__Color),			t_integer,  0);
#endif
	m_aPrimaryKey.Add(0);
	m_aPrimaryKey.Add(1);
	m_mapRow2PsltK.RemoveAll();
}

CPsltGrid::~CPsltGrid()
{
}

#define CWnd CTBCommon
BEGIN_MESSAGE_MAP(CPsltGrid, CWnd)
#undef CWnd
	//{{AFX_MSG_MAP(CPsltGrid)
	ON_WM_RBUTTONDBLCLK()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CPsltGrid message handlers
void CPsltGrid::Initialize(CDBDoc* pDoc, CCMPsltDlg* pParent)
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
		.SetControl(GX_IDS_CTRL_STATIC));
	SetStyleRange(CGXRange().SetCols(1, 2), CGXStyle()
		.SetHorizontalAlignment(DT_LEFT)
		.SetValueType(GX_VT_NUMERIC)
		.SetEnabled(FALSE)
		.SetPlaces(0));
	SetStyleRange(CGXRange().SetCols(3, 3), CGXStyle()
		.SetHorizontalAlignment(DT_LEFT)
		.SetValueType(GX_VT_STRING));
	SetStyleRange(CGXRange().SetCols(4, 4), CGXStyle()
		.SetHorizontalAlignment(DT_LEFT)
		.SetValueType(GX_VT_STRING));
	SetStyleRange(CGXRange().SetCols(5, 5), CGXStyle()
		.SetHorizontalAlignment(DT_LEFT)
		.SetValueType(GX_VT_STRING));

	// Key Field도 숨긴다.
	HideColsByIntend(1, 1, TRUE);
	// PsltId는 숨긴다.(사용자가 알 필요 없는 칼럼이다.)
	HideColsByIntend(2, 2, TRUE); 
	SetRightValidCell(1, 2);

	// Sorting 정보 초기화
	m_aSortInfo.SetSize(2);
	m_aSortInfo[0].nRC = 1; // Sno
	m_aSortInfo[0].sortType = CGXSortInfo::numeric;
	m_aSortInfo[0].sortOrder = CGXSortInfo::ascending;
	m_aSortInfo[1].nRC = 0; // Key
	m_aSortInfo[1].sortType = CGXSortInfo::numeric;
	m_aSortInfo[1].sortOrder = CGXSortInfo::ascending;

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

BOOL CPsltGrid::InsertRecordByKey(const T_PSLT_K &key, const T_PSLT_D &data)
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

BOOL CPsltGrid::DeleteRecordByKey(const T_PSLT_K &key, const T_PSLT_D &data)
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
// CPsltGrid Overridable Functions
void CPsltGrid::OnChangedSelectionPublic(long *keys, int nSize)
{
	return;
}

long CPsltGrid::GetSelectedKeys(CArray<long, long> &caKey)
{
	return 0;
}

BOOL CPsltGrid::ValidateField(CString value, int nColID)
{
	return TRUE;
}

BOOL CPsltGrid::ValidateRecord(CStringArray &value, CRowColArray &aCols)
{
	return TRUE;
}

BOOL CPsltGrid::ValidateAndMakeRecord(CStringArray &value, CRowColArray& aCols, T_PSLT_K &key, T_PSLT_D &data)
{
	return TRUE;
}

BOOL CPsltGrid::AddToDB(ROWCOL nRow)
{
	return TRUE;
}

BOOL CPsltGrid::AddToDB(CArray<void*, void*>& raKey, CArray<void*, void*>& raData)
{
	return TRUE;
}


BOOL CPsltGrid::DeleteFromDB(ROWCOL nRow)
{
	return TRUE;
}

BOOL CPsltGrid::DeleteFromDB(CRowColArray& awRows)
{
	return TRUE;
}

BOOL CPsltGrid::ModifyToDB(ROWCOL nRow)
{
	return TRUE;
}

BOOL CPsltGrid::ModifyToDB(CStringArray& raOldKey, 
													 CArray<void*, void*>& raKey, 
													 CArray<void*, void*>& raData)
{
	return TRUE;
}


CString CPsltGrid::GetNewKey()
{
	return CString(_T(""));
}

CString CPsltGrid::GetDefValue(int nIndex)
{
	return m_aCurDefVal[nIndex];
}

CString* CPsltGrid::GetDefValueArray()
{
	return m_aCurDefVal;
}

BOOL CPsltGrid::MakeKeyAndDataArray(CStringArray& value, CRowColArray& awCols, 
																		CArray<void*, void*>& aKey, CArray<void*, void*>& aData)
{
	ASSERT(0);
	return TRUE;
}

void CPsltGrid::DeleteKeyAndDataArray(CArray<void*, void*>& aKey, CArray<void*, void*>& aData)
{
	ASSERT(0);
}

long CPsltGrid::GetCountData()
{
	CDBDoc* pDoc = m_pDoc;
	return pDoc->m_pAttrCtrl2->GetCountPslt();
}

void CPsltGrid::MakeItemFiltered(long nFilteredNum, long* aFilteredKeys, CRowColArray& awCols)
{
	ASSERT(0);
}

void CPsltGrid::MakeItemNormal(long nNumData, CRowColArray& awCols)
{
	T_PSLT_K Key;
	T_PSLT_D Data;
	CDBDoc* pDoc = m_pDoc;

	long* KeyBuf = new long[nNumData];
	long nCount = 0;
	POSITION pos = pDoc->m_pAttrCtrl2->GetStartPslt();
	while(pos != NULL)
	{
		pDoc->m_pAttrCtrl2->GetNextPslt(pos,Key,Data);
		KeyBuf[nCount++] = Key;
	}
	qsort(KeyBuf, nNumData, sizeof(T_PSLT_K), CCompFunc::UINTAsc);

	for(nCount=0 ; nCount < nNumData; nCount++)
	{
		pDoc->m_pAttrCtrl2->GetPslt(KeyBuf[nCount], Data);
		SetValue(GetParam()->GetData(), nCount+1, KeyBuf[nCount], Data, awCols);
	} // end of loop nCount

	delete []KeyBuf;
}

void CPsltGrid::MakeItemEditingRecord(ROWCOL ncRow, CStringArray& EditingRecord)
{
	ASSERT(0);
}

/////////////////////////////////////////////////////////////////////////////
// CPsltGrid Implementation
BOOL CPsltGrid::GetValue(ROWCOL nRow, T_PSLT_K &key, T_PSLT_D &data)
{
#if defined (_MGEN_CH)
	if(m_mapRow2PsltK.PLookup(nRow))
	{
		key = m_mapRow2PsltK[nRow];
		m_pDoc->m_pAttrCtrl2->GetPslt(key,data);
		return TRUE;
	}
	return TRUE;
#endif // MGEN_CH
	ASSERT(0);
	return TRUE;
}

BOOL CPsltGrid::SetValue(ROWCOL nRow, const T_PSLT_K &key, const T_PSLT_D &data)
{
	CRowColArray awCols;
	awCols.SetSize(GetColCount());
	for (int i = 0;i < GetColCount(); i++)
		VERIFY(GetColumnNoFor(m_aTitle[i], awCols[i]));
	return SetValue(GetParam()->GetData(), nRow, key, data, awCols);
}

BOOL CPsltGrid::SetValue(CGXData* pData, ROWCOL nRow, const T_PSLT_K &key, const T_PSLT_D &data, CRowColArray &awCols)
{
	CString str;
	for (int i = 0; i < GetColCount(); i++)
	{
		if(i==0) 
		{
			if (key == 0) str.Empty();
			else str.Format(PSLT_KEY_FMT, key);
		}
		else ConvDataToStr(i, data, str);
		pData->StoreValueRowCol(nRow, awCols[i], str, gxOverride);
	}
#if defined (_MGEN_CH)
	m_mapRow2PsltK[nRow] = key;
#endif
	return TRUE;
}

void CPsltGrid::ConvStrToData(int i, CString& value, T_PSLT_D& data)
{
	ASSERT(0);
}

void CPsltGrid::ConvDataToStr(int i, const T_PSLT_D& data, CString& value)
{
	if (i==1) value.Format(_T("%d"), data.PsltId);
	else if (i==2) value = data.strName;
	else if (i==3) value = m_pDoc->m_pAttrCtrl2->GetPsltElemTypeStr(data.nElemType);
	else if (i==4) value = data.strDesc;
#if defined (_MGEN_CH)
	else if(i==5) value = _T("");
#endif
}

void CPsltGrid::MakeSearchKey(const T_PSLT_K &key, const T_PSLT_D &data, CStringArray &aKey)
{
	int nColID;
	int nSortKeyNum = m_aSortInfo.GetSize();

	aKey.SetSize(nSortKeyNum);
	for (int i = 0; i < nSortKeyNum; i++)
	{
		nColID = m_aSortInfo[i].nRC;
		if (nColID == 0) aKey[i].Format(PSLT_KEY_FMT, key);
		else ConvDataToStr(nColID, data, aKey[i]);
	}
}

void CPsltGrid::GetAllSelectedPslt(CArray<T_PSLT_K, T_PSLT_K>& aSelKey)
{
	return;
}

void CPsltGrid::SetHeaderTitle(T_UNIT_SYSTEM &UnitSystem, BOOL bInit)
{
	// Set Title
	CString csTitle, csUnit;
#if defined (_MGEN_CH)
	int aSize[] = {30, 30, 100, 130, 140, 59};
	int aUnit[] = {0, 0, 0, 0, 0, 0};
#else
	int aSize[] = {30, 30, 100, 155, 174};
	int aUnit[] = {0, 0, 0, 0, 0};
#endif
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

BOOL CPsltGrid::GetStyleRowCol(ROWCOL nRow, ROWCOL nCol, CGXStyle& style, GXModifyType mt /* = gxCopy */, int nType /* = 0 */)
{
	BOOL bRlt = CTBCommon::GetStyleRowCol(nRow,nCol,style,mt,nType);
#if defined (_MGEN_CH)
	if(nRow > 0 && nRow < GetRowCount() && nCol == GetColCount())
	{
		T_PSLT_K key;
		T_PSLT_D data;
		if(!GetValue(nRow,key,data))
			return FALSE;
		if(data.nElemType == 0)
			style.SetInterior(data.color);
	}
#endif // MGEN_CH
	return bRlt;
}
//------------------------------------------------------------------------------>
// Floor Load의 위치를 이동한다.
BOOL CPsltGrid::OnSelDragRowsDrop(ROWCOL nFirstRow, ROWCOL nLastRow, ROWCOL nDestRow)
{
	ASSERT(!IsEdit());

	// Append Row의 뒤면 맨 마지막으로 이동하는 것으로 한다.
	if (nDestRow == GetRowCount()+1) nDestRow--;  

	// 이동의 효과가 없는 곳으로 이동(선택범위내로 다시 이동)하면 그냥 리턴
	if (nDestRow >= nFirstRow && nDestRow <= nLastRow+1) return FALSE;

	// 연속 선택이 아니면 이동을 금지시킨다.
	CRowColArray awRows;
	GetSelectedRows(awRows, TRUE, FALSE);
	if (awRows.GetSize() != nLastRow-nFirstRow+1) 
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Can_t_not_move_uncontinuous_recor));
		return FALSE;
	}

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

	// 순번 변경하는 코드 추가.
	CArray<CString, CString&> aLoadTypeName;
	CArray<T_PSLT_D, T_PSLT_D&> aData;
	T_PSLT_K key;
	T_PSLT_D data;
	ROWCOL nkCol;
	GetColumnNoFor(GetKeyColID(), nkCol);
	for (int i = nFirstRow; i <= nLastRow; i++)
	{
		key = _tstol(GetValueRowCol(i, nkCol));
		m_pDoc->m_pAttrCtrl2->GetPslt(key, data);
		data.PsltId = nStart;
		nStart += nInc;

		aLoadTypeName.Add(data.strName);
		aData.Add(data);
	}
	m_pDoc->m_pDataCtrl->ModifyPslt(aLoadTypeName, aData);

	return FALSE;
}
//<-----------------------------------------------------------------------------

//----------------------------------------------------------------------------->
// Current Record가 변경되었는지 검사
BOOL CPsltGrid::CheckCurrentRecordChanged()
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

BOOL CPsltGrid::ProcessKeys(CWnd *pSender, UINT nMessage, UINT nChar, UINT nRepCnt, UINT flags)
{
	BOOL bResult = CTBCommon::ProcessKeys(pSender, nMessage, nChar, nRepCnt, flags);
	CheckCurrentRecordChanged();
	return bResult;
}

void CPsltGrid::OnRButtonDblClk(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	ROWCOL ncRowBefore, ncRowAfter, ncCol;
	if (!GetCurrentCell(ncRowBefore, ncCol)) return;

	CTBCommon::OnRButtonDblClk(nFlags, point);

	if (!GetCurrentCell(ncRowAfter, ncCol)) return;
	if (ncRowBefore != ncRowAfter) OnChangeCurrentRecord(ncRowAfter);
}

void CPsltGrid::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CTBCommon::OnLButtonDblClk(nFlags, point);
	CheckCurrentRecordChanged();
}

void CPsltGrid::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CTBCommon::OnLButtonDown(nFlags, point);
	CheckCurrentRecordChanged();
}

void CPsltGrid::OnChangeCurrentRecord(ROWCOL nRow)
{
	m_nCurrentRow = nRow;

	// Append Row이면 그냥 리턴한다.
	if (nRow < 1) return;
	if (IsAppendRow(nRow)) 
	{
		m_pParent->OnChangeCurrentPslt(0);
		return;
	}

	// Key값을 알아와서 Parent Dialog에 Row의 변동을 알린다.
	ROWCOL nkCol;
	GetColumnNoFor(GetKeyColID(), nkCol);
	T_PSLT_K nKey = _ttoi(GetValueRowCol(nRow, nkCol));
	m_pParent->OnChangeCurrentPslt(nKey);
}
//<-----------------------------------------------------------------------------

BOOL CPsltGrid::GetCurrentPslt(T_PSLT_K& nKey)
{
	ROWCOL ncRow, ncCol;
	if (!GetCurrentCell(ncRow, ncCol)) return FALSE;
	if (ncRow < 1) return FALSE;
	if (IsAppendRow(ncRow)) return FALSE;

	if (!GetColumnNoFor(GetKeyColID(), ncCol)) return FALSE;
	nKey = _ttoi(GetValueRowCol(ncRow, ncCol));
	return TRUE;
}

BOOL CPsltGrid::SetCurrentPslt(T_PSLT_K key, T_PSLT_D& data)
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

void CPsltGrid::SelectPsltAndUpdateDlg(T_PSLT_K PsltK)
{
	T_PSLT_K PsltK_row=0;
	ROWCOL nkCol;
	GetColumnNoFor(GetKeyColID(), nkCol);
	for (int i=1; i<GetRowCount(); i++)
	{
		if(PsltK == _tstol(GetValueRowCol(i, nkCol)))
		{
			SetCurrentCell(i,3);
			m_nCurrentRow = i;
			m_pParent->OnChangeCurrentPslt(PsltK);
			break;
		}
	}
}
