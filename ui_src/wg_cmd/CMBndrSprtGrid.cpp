#include "stdafx.h"
#include "wg_cmd.h"
#include "CMBndrSprtGrid.h"
#include "CMBndrSprtDlg.h"

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

#define GSTP_KEY_FMT _T("%d")
/////////////////////////////////////////////////////////////////////////////
// CCMBndrSprtGrid

CString CCMBndrSprtGrid::m_aCurDefVal[] = {_T(""), _T(""), _T("")};
CCMBndrSprtGrid::CCMBndrSprtGrid()
{
	m_pParent = 0;
	m_nCurrentRow = 0;

/*  AddColInfo(_LS(IDS_WG_CMD__ADDD__No),             t_integer, 0);
	AddColInfo(_LS(IDS_WG_CMD__ADDD__Sno),            t_integer, 0);
	AddColInfo(_LS(IDS_WG_CMD__ADDD__Name),           t_string,  0);
	AddColInfo(_T("Description"),    t_string,  0);   */

	AddColInfo(_LS(IDS_WG_CMD__ADDD__No),             t_integer, 0);
	AddColInfo(_LS(IDS_WG_CMD__ADDD__Sno),            t_integer, 0);
	AddColInfo(_LS(IDS_WG_CMD__ADDD__Name),           t_string,  0);
	AddColInfo(_LS(IDS_WG_CMD__ADDD__SDx),            t_integer,  0);   
	AddColInfo(_LS(IDS_WG_CMD__ADDD__SDy),            t_integer,  0);
	AddColInfo(_LS(IDS_WG_CMD__ADDD__SDz),            t_integer,  0);   
	AddColInfo(_LS(IDS_WG_CMD__ADDD__SRx),            t_integer,  0);
	AddColInfo(_LS(IDS_WG_CMD__ADDD__SRy),            t_integer,  0);   
	AddColInfo(_LS(IDS_WG_CMD__ADDD__SRz),            t_integer,  0);
	AddColInfo(_LS(IDS_CMD_MASS),                     t_string,   0);
	AddColInfo(_LS(IDS_CMD_DAMPING),                  t_string,   0);
//  AddColInfo(_LSX(MDx),            t_integer,  0);   
//  AddColInfo(_LSX(MDy),            t_integer,  0);
//  AddColInfo(_LSX(MDz),            t_integer,  0);   
//  AddColInfo(_LSX(MRx),            t_integer,  0);
//  AddColInfo(_LSX(MRy),            t_integer,  0);   
//  AddColInfo(_LSX(MRz),            t_integer,  0);
//  AddColInfo(_LSX(DDx),            t_integer,  0);   
//  AddColInfo(_LSX(DDy),            t_integer,  0);
//  AddColInfo(_LSX(DDz),            t_integer,  0);   
//  AddColInfo(_LSX(DRx),            t_integer,  0);
//  AddColInfo(_LSX(DRy),            t_integer,  0);   
//  AddColInfo(_LSX(DRz),            t_integer,  0);

	m_aPrimaryKey.Add(0);
	m_aPrimaryKey.Add(1);
}

CCMBndrSprtGrid::~CCMBndrSprtGrid()
{
}

#define CWnd CTBCommon
BEGIN_MESSAGE_MAP(CCMBndrSprtGrid, CWnd)
#undef CWnd
	//{{AFX_MSG_MAP(CGstpGrid)
	ON_WM_RBUTTONDBLCLK()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CCMBndrSprtGrid::Initialize(CDBDoc* pDoc, CCMBndrSprtDlg* pParent)
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
	SetStyleRange(CGXRange().SetCols(1, 11), CGXStyle()
			.SetControl(GX_IDS_CTRL_STATIC));
	SetStyleRange(CGXRange().SetCols(1, 2), CGXStyle()
			.SetHorizontalAlignment(DT_LEFT)
			.SetValueType(GX_VT_NUMERIC)
			.SetEnabled(FALSE)
			.SetPlaces(0));
	SetStyleRange(CGXRange().SetCols(3, 3), CGXStyle()
			.SetHorizontalAlignment(DT_LEFT)
			.SetValueType(GX_VT_STRING));
	SetStyleRange(CGXRange().SetCols(4, 9), CGXStyle()
			.SetHorizontalAlignment(DT_LEFT)
			.SetValueType(GX_VT_NUMERIC)
			.SetEnabled(FALSE)
			.SetPlaces(0));
	SetStyleRange(CGXRange().SetCols(10, 11), CGXStyle()
			.SetHorizontalAlignment(DT_LEFT)
			.SetValueType(GX_VT_STRING)
			.SetEnabled(FALSE));

	// Key Field도 숨긴다.
	HideColsByIntend(1, 1, TRUE);
	// FloorLoadTypeID는 숨긴다.(사용자가 알 필요 없는 칼럼이다.)
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

BOOL CCMBndrSprtGrid::InsertRecordByKey(const T_GSTP_K &key, const T_GSTP_D &data)
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

BOOL CCMBndrSprtGrid::DeleteRecordByKey(const T_GSTP_K &key, const T_GSTP_D &data)
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
// CCMBndrSprtGrid Overridable Functions
void CCMBndrSprtGrid::OnChangedSelectionPublic(long *keys, int nSize)
{
	return;
}

long CCMBndrSprtGrid::GetSelectedKeys(CArray<long, long> &caKey)
{
	return 0;
}

BOOL CCMBndrSprtGrid::ValidateField(CString value, int nColID)
{
	return TRUE;
}

BOOL CCMBndrSprtGrid::ValidateRecord(CStringArray &value, CRowColArray &aCols)
{
	return TRUE;
}

BOOL CCMBndrSprtGrid::ValidateAndMakeRecord(CStringArray &value, CRowColArray& aCols, T_GSTP_K &key, T_GSTP_D &data)
{
	return TRUE;
}

BOOL CCMBndrSprtGrid::AddToDB(ROWCOL nRow)
{
	return TRUE;
}

BOOL CCMBndrSprtGrid::AddToDB(CArray<void*, void*>& raKey, CArray<void*, void*>& raData)
{
	return TRUE;
}


BOOL CCMBndrSprtGrid::DeleteFromDB(ROWCOL nRow)
{
	return TRUE;
}

BOOL CCMBndrSprtGrid::DeleteFromDB(CRowColArray& awRows)
{
	return TRUE;
}

BOOL CCMBndrSprtGrid::ModifyToDB(ROWCOL nRow)
{
	return TRUE;
}

BOOL CCMBndrSprtGrid::ModifyToDB(CStringArray& raOldKey, 
												 CArray<void*, void*>& raKey, 
												 CArray<void*, void*>& raData)
{
	return TRUE;
}


CString CCMBndrSprtGrid::GetNewKey()
{
	return CString(_T(""));
}

CString CCMBndrSprtGrid::GetDefValue(int nIndex)
{
	return m_aCurDefVal[nIndex];
}

CString* CCMBndrSprtGrid::GetDefValueArray()
{
	return m_aCurDefVal;
}

BOOL CCMBndrSprtGrid::MakeKeyAndDataArray(CStringArray& value, CRowColArray& awCols, 
																	CArray<void*, void*>& aKey, CArray<void*, void*>& aData)
{
	ASSERT(0);
	return TRUE;
}

void CCMBndrSprtGrid::DeleteKeyAndDataArray(CArray<void*, void*>& aKey, CArray<void*, void*>& aData)
{
	ASSERT(0);
}

long CCMBndrSprtGrid::GetCountData()
{
	CDBDoc* pDoc = m_pDoc;
//  return pDoc->m_pAttrCtrl->GetCountFbld();
	return pDoc->m_pAttrCtrl->GetCountGstp();
}

void CCMBndrSprtGrid::MakeItemFiltered(long nFilteredNum, long* aFilteredKeys, CRowColArray& awCols)
{
	ASSERT(0);
}

void CCMBndrSprtGrid::MakeItemNormal(long nNumData, CRowColArray& awCols)
{
	T_GSTP_K Key;
	T_GSTP_D Data;
	CDBDoc* pDoc = m_pDoc;

	long* KeyBuf = new long[nNumData];
	long nCount = 0;
//  POSITION pos = pDoc->m_pAttrCtrl->GetStartFbld();
	POSITION pos = pDoc->m_pAttrCtrl->GetStartGstp();
	while(pos != NULL)
	{
//	  pDoc->m_pAttrCtrl->GetNextFbld(pos,Key,Data);
	  pDoc->m_pAttrCtrl->GetNextGstp(pos,Key,Data);
	  KeyBuf[nCount++] = Key;
	}
	qsort(KeyBuf, nNumData, sizeof(T_GSTP_K), CCompFunc::UINTAsc);

	for(nCount=0 ; nCount < nNumData; nCount++)
	{
//    pDoc->m_pAttrCtrl->GetFbld(KeyBuf[nCount], Data);
		pDoc->m_pAttrCtrl->GetGstp(KeyBuf[nCount], Data);
		SetValue(GetParam()->GetData(), nCount+1, KeyBuf[nCount], Data, awCols);
	} // end of loop nCount

	delete []KeyBuf;
}

void CCMBndrSprtGrid::MakeItemEditingRecord(ROWCOL ncRow, CStringArray& EditingRecord)
{
	ASSERT(0);
}

/////////////////////////////////////////////////////////////////////////////
// CCMBndrSprtGrid Implementation
BOOL CCMBndrSprtGrid::GetValue(ROWCOL nRow, T_GSTP_K &key, T_GSTP_D &data)
{
	ASSERT(0);
	return TRUE;
}

BOOL CCMBndrSprtGrid::SetValue(ROWCOL nRow, const T_GSTP_K &key, const T_GSTP_D &data)
{
	CRowColArray awCols;
	awCols.SetSize(GetColCount());
	for (int i = 0;i < GetColCount(); i++)
		VERIFY(GetColumnNoFor(m_aTitle[i], awCols[i]));
	return SetValue(GetParam()->GetData(), nRow, key, data, awCols);
}

BOOL CCMBndrSprtGrid::SetValue(CGXData* pData, ROWCOL nRow, const T_GSTP_K &key, const T_GSTP_D &data, CRowColArray &awCols)
{
	CString str;
	for (int i = 0; i < GetColCount(); i++)
	{
		if(i==0) 
		{
			if (key == 0) str.Empty();
			else str.Format(GSTP_KEY_FMT, key);
		}
		else ConvDataToStr(i, data, str);
		pData->StoreValueRowCol(nRow, awCols[i], str, gxOverride);
	}

	return TRUE;
}

void CCMBndrSprtGrid::ConvStrToData(int i, CString& value, T_GSTP_D& data)
{
	ASSERT(0);
}

void CCMBndrSprtGrid::ConvDataToStr(int i, const T_GSTP_D& data, CString& value)
{
/*  if (i==1) value.Format(_T("%d"), data.FloorLoadTypeId);
	else if (i==2) value = data.FloorLoadTypeName;
	else if (i==3) value = data.Description;   */

	CString strSpring = _T("0");
	//else if (i >=9 && i<15)
	//  strSpring.Format(_T("%g"), data.Mass[i-9]);
	//else
	//  strSpring.Format(_T("%g"), data.Damping[i-15]);
	
	if     (i==1) value.Format(_T("%d"), data.nTypeId);
	else if(i==2) value = data.TypeName;
	else if(i >= 3 && i<9)
	{
		strSpring.Format(_T("%g"), data.Spring[i-3]);
		value = strSpring;  
	}
	else if(i==9) 
	{
		if(data.bMass) value = _LS(IDS_CMD_SPRT_CONSIDERED);
		else           value = _LS(IDS_CMD_SPRT_NONE);
	}
	else if(i==10)
	{
		if(data.bDamping) value = _LS(IDS_CMD_SPRT_CONSIDERED);
		else              value = _LS(IDS_CMD_SPRT_NONE);
	}
}

void CCMBndrSprtGrid::MakeSearchKey(const T_GSTP_K &key, const T_GSTP_D &data, CStringArray &aKey)
{
	int nColID;
	int nSortKeyNum = m_aSortInfo.GetSize();

	aKey.SetSize(nSortKeyNum);
	for (int i = 0; i < nSortKeyNum; i++)
	{
		nColID = m_aSortInfo[i].nRC;
		if (nColID == 0) aKey[i].Format(GSTP_KEY_FMT, key);
		else ConvDataToStr(nColID, data, aKey[i]);
	}
}

void CCMBndrSprtGrid::GetAllSelectedGstp(CArray<T_GSTP_K, T_GSTP_K>& aSelKey)
{
	return;
}

void CCMBndrSprtGrid::SetHeaderTitle(T_UNIT_SYSTEM &UnitSystem, BOOL bInit)
{
	// Set Title
	CString csTitle, csUnit;
//  int aUnit[] = {0, 0, 0, 0};
	int aUnit[] = {0, 0, 0, D_UNITSYS_BASE_SPRING  , D_UNITSYS_BASE_SPRING  , D_UNITSYS_BASE_SPRING,
													D_UNITSYS_BASE_ROSPRING, D_UNITSYS_BASE_ROSPRING, D_UNITSYS_BASE_ROSPRING,
								 0, 0,
													/*D_UNITSYS_BASE_SPRING  , D_UNITSYS_BASE_SPRING  , D_UNITSYS_BASE_SPRING,
													D_UNITSYS_BASE_ROSPRING, D_UNITSYS_BASE_ROSPRING, D_UNITSYS_BASE_ROSPRING,
													D_UNITSYS_BASE_SPRING  , D_UNITSYS_BASE_SPRING  , D_UNITSYS_BASE_SPRING,
													D_UNITSYS_BASE_ROSPRING, D_UNITSYS_BASE_ROSPRING, D_UNITSYS_BASE_ROSPRING,*/};

//  int aSize[] = {30, 30, 100, 185};
	int aSize[] = {30, 30, 60, 60, 60, 60, 85, 85, 85, 60, 60 /*60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60*/};
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
	SetRowHeightByDPI(0, 0, 36);
}

//------------------------------------------------------------------------------>
// General Spring Type의 위치를 이동한다.
BOOL CCMBndrSprtGrid::OnSelDragRowsDrop(ROWCOL nFirstRow, ROWCOL nLastRow, ROWCOL nDestRow)
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
//  CArray<CString, CString&> aLoadTypeName;
	CArray<CString, CString&> aTypeName;
	CArray<T_GSTP_D, T_GSTP_D&> aData;
	T_GSTP_K key;
	T_GSTP_D data;
	ROWCOL nkCol;
	GetColumnNoFor(GetKeyColID(), nkCol);
	for (int i = nFirstRow; i <= nLastRow; i++)
	{
		key = _tstol(GetValueRowCol(i, nkCol));
		m_pDoc->m_pAttrCtrl->GetGstp(key, data);
		data.nTypeId = nStart;
		nStart += nInc;

		aTypeName.Add(data.TypeName);
		aData.Add(data);
	}
	m_pDoc->m_pDataCtrl->ModifyGstp(aTypeName, aData);
	
	return FALSE;
}
//<-----------------------------------------------------------------------------

//----------------------------------------------------------------------------->
// Current Record가 변경되었는지 검사
BOOL CCMBndrSprtGrid::CheckCurrentRecordChanged()
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

BOOL CCMBndrSprtGrid::ProcessKeys(CWnd *pSender, UINT nMessage, UINT nChar, UINT nRepCnt, UINT flags)
{
	BOOL bResult = CTBCommon::ProcessKeys(pSender, nMessage, nChar, nRepCnt, flags);
	CheckCurrentRecordChanged();
	return bResult;
}

void CCMBndrSprtGrid::OnRButtonDblClk(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	ROWCOL ncRowBefore, ncRowAfter, ncCol;
	if (!GetCurrentCell(ncRowBefore, ncCol)) return;

	CTBCommon::OnRButtonDblClk(nFlags, point);

	if (!GetCurrentCell(ncRowAfter, ncCol)) return;
	if (ncRowBefore != ncRowAfter) OnChangeCurrentRecord(ncRowAfter);
}

void CCMBndrSprtGrid::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CTBCommon::OnLButtonDblClk(nFlags, point);
	CheckCurrentRecordChanged();
}

void CCMBndrSprtGrid::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CTBCommon::OnLButtonDown(nFlags, point);
	CheckCurrentRecordChanged();
}

void CCMBndrSprtGrid::OnChangeCurrentRecord(ROWCOL nRow)
{
	m_nCurrentRow = nRow;

	// Append Row이면 그냥 리턴한다.
	if (nRow < 1) return;
	
	int nTabSel = m_pParent->m_Tab.GetCurSel();

	if (IsAppendRow(nRow)) 
	{
		m_pParent->OnChangeCurrentGSTP(0);
		return;
	}

	// Key값을 알아와서 Parent Dialog에 Row의 변동을 알린다.
	ROWCOL nkCol;
	GetColumnNoFor(GetKeyColID(), nkCol);
	T_GSTP_K nKey = _ttoi(GetValueRowCol(nRow, nkCol));
	m_pParent->OnChangeCurrentGSTP(nKey);

	m_pParent->m_Tab.SetCurSel(nTabSel);
}
//<-----------------------------------------------------------------------------

BOOL CCMBndrSprtGrid::GetCurrentGSTP(T_GSTP_K& nKey)
{
	ROWCOL ncRow, ncCol;
	if (!GetCurrentCell(ncRow, ncCol)) return FALSE;
	if (ncRow < 1) return FALSE;
	if (IsAppendRow(ncRow)) return FALSE;

	if (!GetColumnNoFor(GetKeyColID(), ncCol)) return FALSE;
	nKey = _ttoi(GetValueRowCol(ncRow, ncCol));
	return TRUE;
}

BOOL CCMBndrSprtGrid::SetCurrentGSTP(T_GSTP_K key, T_GSTP_D& data)
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


