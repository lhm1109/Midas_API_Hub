// AssessmentLoadCombMainGridSTLNR.cpp : implementation file
//

#include "stdafx.h"

#include "wg_dgn.h"
#include "AssessmentLoadCombMainGridSTLNR.h"

#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_LoadDB.h"
#include "..\wg_db\wg_db_LoadDgnCodeCheck.h"

#include "AssessmentLoadCombSubGridSTLNR.h"
#include "AssessmentLoadCombSTLNRDlg.h"
#include "..\wg_base\wg_base_ServiceProvider.h"
#include "..\wg_tb\Tb_ServiceDef.h"
#include "..\wg_db\AttrCtrl2.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define D_ALCOM_COL_KEY		    1
#define D_ALCOM_COL_NAME		2
#define D_ALCOM_COL_LIM_STATE	3
#define D_ALCOM_COL_COMB_TYPE	4
#define D_ALCOM_COL_DESC		5

/////////////////////////////////////////////////////////////////////////////
// CAssessmentLoadCombMainGridSTLNR
CAssessmentLoadCombMainGridSTLNR::CAssessmentLoadCombMainGridSTLNR(CAssessmentLoadCombSTLNRDlg* pParent)
	:m_bElastStag(FALSE), m_bTB10002_1_05(FALSE)
{
	m_pParent = pParent;

	m_aCurDefVal[D_ALCOM_COL_KEY - 1] = _T("");
	m_aCurDefVal[D_ALCOM_COL_NAME - 1] = _T("");
	m_aCurDefVal[D_ALCOM_COL_LIM_STATE - 1] = _LS(IDS_DGN_ULS);
	m_aCurDefVal[D_ALCOM_COL_COMB_TYPE - 1] = _LS(IDS_DGN_COMB_SMPL) + _T("1");
	m_aCurDefVal[D_ALCOM_COL_DESC - 1] = _T("");

	//m_nLcomType = D_LCOMTYPE_GENERAL;
	m_pSubGrid = 0;

	m_bIgnoreSetEditMode = FALSE;

	AddColInfo(_ULS(Key), t_integer, 0);
	AddColInfo(_LS(IDS_CMD_STLD_name), t_string, 0);
	AddColInfo(_LS(IDS_DGN_LIM_STATE), t_string, 0);
	AddColInfo(_LS(IDS_DGN_COMB_TYPE), t_string, 0);
	AddColInfo(_LS(IDS_WG_CMD__ADDD__Description), t_string, 0);

	m_aPrimaryKey.Add(0);

	m_nUpdateState = 1;
	m_LastDelRow = 1;

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

CAssessmentLoadCombMainGridSTLNR::~CAssessmentLoadCombMainGridSTLNR()
{
}

#define CWnd CTBCommon
BEGIN_MESSAGE_MAP(CAssessmentLoadCombMainGridSTLNR, CWnd)
#undef CWnd
	//{{AFX_MSG_MAP(CAssessmentLoadCombMainGridSTLNR)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CAssessmentLoadCombMainGridSTLNR message handlers
void CAssessmentLoadCombMainGridSTLNR::Initialize(CDBDoc* pDoc, T_UNIT_SYSTEM &UnitSystem)
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
	SetStyleRange(CGXRange().SetCols(D_ALCOM_COL_KEY), CGXStyle()
		.SetHorizontalAlignment(DT_RIGHT)
		.SetValueType(GX_VT_NUMERIC)
		.SetFormat(GX_FMT_FIXED)
		.SetPlaces(0));

	SetStyleRange(CGXRange().SetCols(D_ALCOM_COL_NAME, D_ALCOM_COL_COMB_TYPE), CGXStyle()
		.SetHorizontalAlignment(DT_LEFT)
		.SetValueType(GX_VT_STRING));

	SetStyleRange(CGXRange().SetCols(D_ALCOM_COL_DESC), CGXStyle()
		.SetHorizontalAlignment(DT_LEFT)
		.SetValueType(GX_VT_STRING));

	SetFrozenCols(0, 0);
	HideColsByIntend(D_ALCOM_COL_KEY, D_ALCOM_COL_KEY, TRUE);

	CString csChoiceList;
	csChoiceList.Format(_T("%s\n"), GetLimStateStr(0));
	SetStyleRange(CGXRange().SetCols(D_ALCOM_COL_LIM_STATE), CGXStyle()
		.SetControl(GX_IDS_CTRL_TABBED_COMBOBOX)
		.SetChoiceList(csChoiceList)
		.SetUserAttribute(GX_IDS_UA_TABLIST_KEYCOL, _T("0"))
		.SetUserAttribute(GX_IDS_UA_TABLIST_TEXTCOL, _T("0"))
		.SetUserAttribute(GX_IDS_UA_TABLIST_SHOWALLCOLS, _T("1"))
		.SetValueType(GX_VT_STRING)
		.SetHorizontalAlignment(DT_LEFT)
		.SetValue(_T(""))
	);

	csChoiceList.Format(_T("%s\n%s\n%s\n"),
			GetCombTypeStr(0), GetCombTypeStr(1), GetCombTypeStr(2));

	SetStyleRange(CGXRange().SetCols(D_ALCOM_COL_COMB_TYPE), CGXStyle()
		.SetControl(GX_IDS_CTRL_TABBED_COMBOBOX)
		.SetChoiceList(csChoiceList)
		.SetUserAttribute(GX_IDS_UA_TABLIST_KEYCOL, _T("0"))
		.SetUserAttribute(GX_IDS_UA_TABLIST_TEXTCOL, _T("0"))
		.SetUserAttribute(GX_IDS_UA_TABLIST_SHOWALLCOLS, _T("1"))
		.SetValueType(GX_VT_STRING)
		.SetHorizontalAlignment(DT_LEFT)
		.SetValue(_T(""))
	);

	// 현재 셀의 border 설정 변경
	CGXProperties* pProp = GetParam()->GetProperties();
	pProp->SetUserProperty(GX_IDS_OUTLINECURRENTCELL,
		(CGXStyle)pProp->sInvertNoBorder);

	// 현재 셀 설정
	SetRightValidCell(1, 0);
	GetParam()->SetHideCurrentCell(GX_HIDE_NEVER);

	// Sorting 정보 초기화
	m_aSortInfo.SetSize(1);
	m_aSortInfo[0].nRC = 0; // key
	m_aSortInfo[0].sortType = CGXSortInfo::numeric;
	m_aSortInfo[0].sortOrder = CGXSortInfo::ascending;

	// always show vertical scrollbar
	SetScrollBarMode(SB_VERT, gxnEnabled | gxnEnhanced);

	GetParam()->EnableMoveRows(FALSE);

	// Insert key에 의한 삽입 금지
	EnableInsertRowByPressingKey(FALSE);

	// Column 크기 설정
	auto L_SetColWidth = [this] (ROWCOL nCol, int nWidth)
	{
		SetColWidth(nCol, nCol, globalUtils.ScaleByDPI(nWidth));
	};

	L_SetColWidth(D_ALCOM_COL_KEY, 0);
	L_SetColWidth(D_ALCOM_COL_NAME, 120);
	L_SetColWidth(D_ALCOM_COL_LIM_STATE, 100);
	L_SetColWidth(D_ALCOM_COL_COMB_TYPE, 110);
	L_SetColWidth(D_ALCOM_COL_DESC, 110);

	MakeItemEx();

	LockUpdate(FALSE);
}

BOOL CAssessmentLoadCombMainGridSTLNR::GetRowNoByKey(UINT key, ROWCOL& nRow)
{
	ROWCOL nColKey;
	GetColumnNoFor(m_aTitle[0], nColKey);
	for (ROWCOL nRowLoop = 1; nRowLoop <= GetRowCount(); nRowLoop++)
	{
		const CString& szKey = GetValueRowCol(nRowLoop, nColKey);

		if (_ttoi(szKey) == key)
		{
			nRow = nRowLoop;
			return TRUE;
		}
	}
	return FALSE;
}

BOOL CAssessmentLoadCombMainGridSTLNR::GetInsertRowByKey(UINT key, ROWCOL& nRow)
{
	if (GetRowCount() == 1)
	{
		nRow = 1;
		return TRUE;
	}

	ROWCOL nColKey;
	UINT EachKey;
	GetColumnNoFor(m_aTitle[0], nColKey);
    ROWCOL nRowLoop;
	for ( nRowLoop = 1; nRowLoop < GetRowCount(); nRowLoop++)
	{
		const CString& szKey = GetValueRowCol(nRowLoop, nColKey);

		EachKey = _ttoi(szKey);

		if (EachKey == key)
		{
			nRow = nRowLoop;
			return TRUE;
		}
	}
	if (m_nUpdateState == 0)
	{
		nRow = m_LastDelRow;	// m_LastDelRow를 Return 해주는 이유 :
		// Modify할때 대화상자클래스 UpdateBuffer()함수에서 DeleteRecordByKey()이후 InsertRecordByKey()를 하는데
		// InsertRecordByKey()를 할 때 이미 지워버린 Row를 이 함수를 통해 찾으려고 시도 하기 때문에 찾지를 못함
		// 그래서 Row를 못찾았을 때는 직전에 지웠던 Row를 찾는 시도라고 봄
	}
	else if (m_nUpdateState == 1)
	{
		nRow = nRowLoop; // 원래 m_nUpdateState == 0 인 상황 밖에 없었는데, Copy Button 이 들어가면서 Modify 일 경우엔 지웠던것 바로 아래에 데이터가 추가되고, Add 일 경우에 제일 아래에 추가해야 하는 상황이 생김.
						// 그러나 DB 에서 Add 와 Del Flag 밖에 없어서 이 둘을 구분을 못함..
						// 그래서.. 이딴식의 이상한 처리를 하게 되었음....
	}
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CAssessmentLoadCombMainGridSTLNR Overridable Functions
void CAssessmentLoadCombMainGridSTLNR::OnChangedSelectionPublic(long *keys, int nSize)
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

long CAssessmentLoadCombMainGridSTLNR::GetSelectedKeys(CArray<long, long> &caKey)
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

BOOL CAssessmentLoadCombMainGridSTLNR::ValidateField(CString value, int nColID)
{
	if (nColID >= GetColCount())
	{
		//SetWarningText(_LS(IDS_WG_CMD__ADDD___CAssessmentLoadCombMainGridSTLNR__ValidateFie));
		SetWarningText(_T("Invalid date!"));
		return FALSE;
	}

	return TRUE;
}

// 삽입이나 수정전에 Record의 Validation을 검사한다.
BOOL CAssessmentLoadCombMainGridSTLNR::ValidateRecord(CStringArray &value, CRowColArray &aCols)
{
	CString msg = _T("");
	int nPos;

	for (int i = D_ALCOM_COL_NAME - 1; i < D_ALCOM_COL_COMB_TYPE; i++)
	{
		nPos = aCols[i] - 1;
		if (value[nPos] == _T(""))
			msg += ((msg != _T("")) ? _T("\n") : _T("")) + m_aTitle[i] + _LS(IDS_WG_CMD__ADDD___has_not_been_entered_);
	}

    int i = 0;
	for (i = 0; i < m_pSubGrid->GetRowCount() - 1; i++)
	{
		CString szDebug = m_pSubGrid->GetValueRowCol(i + 1, D_COL_RAC_LCASE);
		if (szDebug == _T(""))
		{
			msg += ((msg != _T("")) ? _T("\n") : _T("")) + CString(_LS(IDS_WG_CMD__ADDD__Load_Case)) + _LS(IDS_WG_CMD__ADDD___has_not_been_entered_);
			break;
		}
		szDebug = m_pSubGrid->GetValueRowCol(i + 1, D_COL_RAC_FACTOR);
		if (szDebug == _T(""))
		{
			msg += ((msg != _T("")) ? _T("\n") : _T("")) + CString(_LS(IDS_WG_CMD__ADDD__Factor)) + _LS(IDS_WG_CMD__ADDD___has_not_been_entered_);
			break;
		}
	}

	if (i == 0)
	{
		msg = CString(_LS(IDS_CMD_PJST_Static_Load_Case)) + _LS(IDS_WG_CMD__ADDD___has_not_been_entered_);
		SetWarningText(msg);
		return FALSE;
	}

	if (msg == _T("")) return TRUE;
	else SetWarningText(msg);
	return FALSE;
}

CString CAssessmentLoadCombMainGridSTLNR::GetDefValue(int nIndex)
{
	if (nIndex == 0) return _T("");
	return m_aCurDefVal[nIndex + 1];
}

CString* CAssessmentLoadCombMainGridSTLNR::GetDefValueArray()
{
	return m_aCurDefVal;
}

void CAssessmentLoadCombMainGridSTLNR::MakeItemFiltered(long nFilteredNum, long* aFilteredKeys, CRowColArray& awCols)
{
	ASSERT(0);

}

void CAssessmentLoadCombMainGridSTLNR::GetAllSelectedLcom(CArray<UINT, UINT>& aSelKey)
{
	for (int i = 0; i < m_aSelectedKeys.GetSize(); i++)
	{
		aSelKey.Add(m_aSelectedKeys[i]);
	}
}

// ProcessKeys, OnRButtonDblClk, OnLButtonDown은 Current Record 변경시 Grid간의
// 동기화를 맞추기 위해 Override했다.
// Current Record 변경시 OnChangeCurrentRecord 함수가 불리도록 설계했다.
BOOL CAssessmentLoadCombMainGridSTLNR::ProcessKeys(CWnd *pSender, UINT nMessage, UINT nChar, UINT nRepCnt, UINT flags)
{
	CTBCommon::ProcessKeys(pSender, nMessage, nChar, nRepCnt, flags);

	CheckCurrentRecordChanged();
	return TRUE;
}


BOOL CAssessmentLoadCombMainGridSTLNR::OnCanceledModify()
{
	ROWCOL ncRow, ncCol;
	GetCurrentCell(ncRow, ncCol);
	OnChangeCurrentRecord(ncRow);
	return TRUE;
}

CString CAssessmentLoadCombMainGridSTLNR::GetKeyForRow(ROWCOL ncRow, ROWCOL nkCol)
{
	CString sKey;

	if (IsAppendRow(ncRow)) sKey = _LS(IDS_WG_CMD__ADDD__Append_Row);
	else if (m_nEditMode == addnew) sKey = _LS(IDS_WG_CMD__ADDD__New_Row);
	else if (m_nEditMode == edit) sKey = GetValueRowColBeforeEdit(ncRow, nkCol);
	else sKey = GetValueRowCol(ncRow, nkCol);

	return sKey;
}

// Current Record가 변경되었는지 검사
BOOL CAssessmentLoadCombMainGridSTLNR::CheckCurrentRecordChanged()
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
	// END KJH20050203
	return FALSE;
}
BOOL CAssessmentLoadCombMainGridSTLNR::IsActiveCol(ROWCOL& nCol)
{
	ROWCOL nRow, nACol;

	GetCurrentCell(nRow, nCol);
	VERIFY(GetColumnNoFor(5, nACol));

	return nCol == nACol;
}
// TBBrowserWndEx::OnSelDragColsDrop 참조
// 부모를 먼저 호출하면 안 된다.
// 1, 2 모두 Frozen Cols로 설정되어 있으므로 이쪽으로 셀이 이동되거나 이 셀들이
// 이동되는 것을 금지한다.
// 주1 참조
BOOL CAssessmentLoadCombMainGridSTLNR::OnSelDragColsDrop(ROWCOL nFirstCol, ROWCOL nLastCol, ROWCOL nDestCol)
{
	// Frozen 되어 있는 column 위치에 이동되면 이동을 금지시킨다.
	//if (IsFrozenCol(nDestCol)) return FALSE;
	if (nDestCol == 1 || nFirstCol == 1) return FALSE;
	return CTBCommon::OnSelDragColsDrop(nFirstCol, nLastCol, nDestCol);
}

void CAssessmentLoadCombMainGridSTLNR::SetHeaderTitle(T_UNIT_SYSTEM &UnitSystem, BOOL bInit)
{
	SetHeaderTitle2((int*)m_aUnit.GetData(), bInit, TRUE);
}

BOOL CAssessmentLoadCombMainGridSTLNR::OnPasteFromClipboard(const CGXRange &range)
{
	//AfxMessageBox(_LS(IDS_CMD_DONT_SUPPORT_PASTE));
	AfxMessageBox(_T("Not Supported!"));
	return FALSE;
}

void CAssessmentLoadCombMainGridSTLNR::OnLButtonDblClk(UINT nFlags, CPoint point)
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
void CAssessmentLoadCombMainGridSTLNR::OnLButtonDown(UINT nFlags, CPoint point)
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
			point.x = (r.left + r.right) / 2;
		}
	}

	ROWCOL ncRowBefore, ncRowAfter, ncCol;
	if (!GetCurrentCell(ncRowBefore, ncCol)) return;

	CTBCommon::OnLButtonDown(nFlags, point);

	if (!GetCurrentCell(ncRowAfter, ncCol)) return;
	if (ncRowBefore != ncRowAfter) OnChangeCurrentRecord(ncRowAfter);
	return;
}

void CAssessmentLoadCombMainGridSTLNR::SetModeEdit()
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

int CAssessmentLoadCombMainGridSTLNR::GetLimState(CString &value)
{
	if (value == _LS(IDS_DGN_ULS))
	{
		return 0;
	}
	else if (value == _LS(IDS_DGN_SLS))
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

CString CAssessmentLoadCombMainGridSTLNR::GetLimStateStr(int nVal)
{
	if (nVal == 0)
	{
		return _LS(IDS_DGN_ULS);
	}
	else if (nVal == 1)
	{
		return _LS(IDS_DGN_SLS);
	}
	else
	{
		return _LS(IDS_DGN_ULS);
	}
}

int CAssessmentLoadCombMainGridSTLNR::GetCombType(CString &value)
{
	if (value == _LS(IDS_DGN_COMB_SMPL) + _T("1"))
	{
		return 0;
	}
	else if (value == _LS(IDS_DGN_COMB_SMPL) + _T("2"))
	{
		return 1;
	}
	else if (value == _LS(IDS_DGN_COMB_SMPL) + _T("3"))
	{
		return 2;
	}
	else if (value == _LS(IDS_DGN_COMB_SMPL) + _T("4"))
	{
		return 3;
	}
	else if (value == _LS(IDS_DGN_COMB_SMPL) + _T("5"))
	{
		return 4;
	}
	else
	{
		return 0;
	}
}

CString CAssessmentLoadCombMainGridSTLNR::GetCombTypeStr(int nVal)
{
	if (nVal == 0)
	{
		return _LS(IDS_DGN_COMB_SMPL) + _T("1");
	}
	else if (nVal == 1)
	{
		return _LS(IDS_DGN_COMB_SMPL) + _T("2");
	}
	else if (nVal == 2)
	{
		return _LS(IDS_DGN_COMB_SMPL) + _T("3");
	}
	else if (nVal == 3)
	{
		return _LS(IDS_DGN_COMB_SMPL) + _T("4");
	}
	else if (nVal == 4)
	{
		return _LS(IDS_DGN_COMB_SMPL) + _T("5");
	}
	else
	{
		return _LS(IDS_DGN_COMB_SMPL) + _T("1");
	}
}

void CAssessmentLoadCombMainGridSTLNR::GetCurSelKey(CArray<T_ALCS_K, T_ALCS_K>& aAlcsK)
{
	aAlcsK.RemoveAll();
	ROWCOL ncRow, ncCol;

	CRowColArray awRows;
	GetSelectedRows(awRows, TRUE, FALSE);
	if (awRows.GetSize() == 0)
	{
		if (GetCurrentCell(ncRow, ncCol))
		{
			awRows.Add(ncRow);
		}
	}

	T_ALCS_K key;
	T_ALCS_D data;
	for (int i = 0; i < awRows.GetSize(); i++)
	{
		if (GetValue(awRows[i], key, data))
		{
			aAlcsK.Add(key);
		}
	}
}

BOOL CAssessmentLoadCombMainGridSTLNR::DeleteRecord(ROWCOL nRow)
{
	BOOL bReturn = CTBCommon::DeleteRecord(nRow);
	if (bReturn)
	{
		m_LastDelRow = nRow;  // Modify명령시 이상한 예외가 있어서 이렇게 처리함.. GetInsertRowByKey()에 달아놓은 주석을 참고 하세요..
	}
	return bReturn;
}


BOOL CAssessmentLoadCombMainGridSTLNR::InsertRecordByKey(const T_ALCS_K &key, const T_ALCS_D &data)
{
	ROWCOL nRow;

	if (!GetInsertRowByKey(key, nRow))
	{
		ASSERT(0);
		return FALSE;
	}

	VERIFY(InsertRecord(nRow));
	return SetValue(nRow, key, data);
}

BOOL CAssessmentLoadCombMainGridSTLNR::DeleteRecordByKey(const T_ALCS_K &key, const T_ALCS_D &data)
{
	ROWCOL nRow;

	if (!GetRowNoByKey(key, nRow))
	{
		ASSERT(0);
		return FALSE;
	}

	return DeleteRecord(nRow);
}

// 이 함수는 Paste할 때 호출되므로 False를 리턴할 때는 ValidateField와 
// Validate Record에서 설정된 Warning Text를 없애주고 리턴한다.
BOOL CAssessmentLoadCombMainGridSTLNR::ValidateAndMakeRecord(CStringArray &value, CRowColArray& aCols, T_ALCS_K &key, T_ALCS_D &data)
{
	// 각 필드에 대해 Validation을 검사한다.
	for (int i = 0; i < value.GetSize(); i++)
	{
		if (!ValidateField(value[aCols[i] - 1], i))
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
		nPos = aCols[i] - 1; // col id i에 해당하는 value가 저장된 위치
		if (i == 0)
		{
			if (value[nPos] == _T("")) key = 0;
			else key = _ttol(value[nPos]);
		}
		else ConvStrToData(i, value[nPos], data);
	}
	return TRUE;
}

BOOL CAssessmentLoadCombMainGridSTLNR::AddToDB(ROWCOL nRow)
{
	// Record를 만들어서 DB에 입력한다.
	T_ALCS_K key;
	T_ALCS_D data;
	data.Initialize();

	GetValue(nRow, key, data);

	return m_pDoc->m_pDataCtrl->AddAlcs(key, data);
}

BOOL CAssessmentLoadCombMainGridSTLNR::AddToDB(CArray<void*, void*>& raKey, CArray<void*, void*>& raData)
{
	ASSERT(0);

	ASSERT(raKey.GetSize() == raData.GetSize());
	T_ALCS_K tKey;
	CArray<T_ALCS_K, T_ALCS_K> aKey;
	CArray<T_ALCS_D, T_ALCS_D&> aData;
	for (int i = 0; i < raKey.GetSize(); i++)
	{
		tKey = *((T_ALCS_K*)raKey.GetAt(i));
		aKey.Add(tKey);
		aData.Add(*((T_ALCS_D*)raData.GetAt(i)));
	}
	return m_pDoc->m_pDataCtrl->AddAlcs(aKey, aData);
}


BOOL CAssessmentLoadCombMainGridSTLNR::DeleteFromDB(ROWCOL nRow)
{
	T_ALCS_K key;
	T_ALCS_D data;
	data.Initialize();

	GetValue(nRow, key, data);
	return m_pDoc->m_pDataCtrl->DelAlcs(key);
}

BOOL CAssessmentLoadCombMainGridSTLNR::DeleteFromDB(CRowColArray& awRows)
{
	if (awRows.GetSize() <= 0) return TRUE;

	ROWCOL nCol;
	nCol = 1;

	CString value;
	CArray<T_ALCS_K, T_ALCS_K> aKey;

	for (int i = 0; i < awRows.GetSize(); i++)
	{
		value = GetValueRowColBeforeEdit(awRows.GetAt(i), nCol);
		UINT nKey = (UINT)_ttoi(value);
		aKey.Add(nKey);
	}

	return m_pDoc->m_pDataCtrl->DelAlcs(aKey);
}

BOOL CAssessmentLoadCombMainGridSTLNR::ModifyToDB(ROWCOL nRow)
{
	ROWCOL nCol;
	nCol = 1;

	T_ALCS_K key;
	T_ALCS_D data;
	data.Initialize();
	CString pOldLoadCombName;

	pOldLoadCombName = GetValueRowColBeforeEdit(nRow, nCol);
	GetValue(nRow, key, data);

	// 값이 변경되지 않았다면 DB에 셋팅 하지 않는다.
	if (!IsDataChange(key, data))
	{
		return TRUE;
	}

	// 수정한다. 실패하면 에러 리턴
	m_nUpdateState = 0;
	BOOL bReturn = m_pDoc->m_pDataCtrl->ModifyAlcs(key, key, data);
	m_nUpdateState = 1;
	return bReturn;
}

BOOL CAssessmentLoadCombMainGridSTLNR::IsDataChange(T_ALCS_K key, T_ALCS_D& dataNew)
{
	T_ALCS_D dataOld;
	m_pDoc->m_pAttrCtrl2->GetAlcs(key, dataOld);
	if (dataOld == dataNew)
	{
		return FALSE;
	}
	else
	{
		return TRUE;
	}
}

BOOL CAssessmentLoadCombMainGridSTLNR::ModifyToDB(CStringArray& raOldKey,
	CArray<void*, void*>& raKey,
	CArray<void*, void*>& raData)
{
	ASSERT(raKey.GetSize() == raData.GetSize());
	ASSERT(raKey.GetSize() == raOldKey.GetSize());
	CArray<T_ALCS_D, T_ALCS_D&> aData;
	CArray<T_ALCS_K, T_ALCS_K> aKey;
	T_ALCS_K Key;
	T_ALCS_D Data;
	Data.Initialize();
	for (int i = 0; i < raKey.GetSize(); i++)
	{
		Key = _ttoi(raOldKey.GetAt(i));
		aKey.Add(Key);
		m_pDoc->m_pAttrCtrl2->GetAlcs(Key, Data);
		aData.Add(*((T_ALCS_D*)raData.GetAt(i)));
	}
	m_nUpdateState = 0;
	BOOL bReturn = m_pDoc->m_pDataCtrl->ModifyAlcs(aKey, aKey, aData);
	m_nUpdateState = 1;
	return bReturn;
}

BOOL CAssessmentLoadCombMainGridSTLNR::MakeKeyAndDataArray(CStringArray& value, CRowColArray& awCols,
	CArray<void*, void*>& aKey, CArray<void*, void*>& aData)
{
	T_ALCS_K* pKey;
	T_ALCS_D* pData;
	pKey = new T_ALCS_K;
	pData = new T_ALCS_D;
	pData->Initialize();
	aKey.Add((void*)pKey);
	aData.Add((void*)pData);
	if (!ValidateAndMakeRecord(value, awCols, *pKey, *pData)) return FALSE;
	return TRUE;
}

void CAssessmentLoadCombMainGridSTLNR::DeleteKeyAndDataArray(CArray<void*, void*>& aKey, CArray<void*, void*>& aData)
{
	T_ALCS_K* pKey;
	T_ALCS_D* pData;
	ASSERT(aKey.GetSize() == aData.GetSize());
	for (int i = 0; i < aKey.GetSize(); i++)
	{
		pKey = (T_ALCS_K*)aKey.GetAt(i);
		pData = (T_ALCS_D*)aData.GetAt(i);
		delete pKey;
		delete pData;
	}
}

long CAssessmentLoadCombMainGridSTLNR::GetCountData()
{
	CDBDoc* pDoc = m_pDoc;
	POSITION pos = pDoc->m_pAttrCtrl2->GetStartAlcs();
	T_ALCS_K tKey;
	T_ALCS_D tData;
	
	int nCount = 0;
	while (pos)
	{
		pDoc->m_pAttrCtrl2->GetNextAlcs(pos, tKey, tData);
		if (tData.nRatingType == 0) // Steel
			nCount++;
	}

	if (m_nEditMode == addnew)
		nCount++;

	return nCount;
}

void CAssessmentLoadCombMainGridSTLNR::MakeItemNormal(long nNumData, CRowColArray& awCols)
{
	T_ALCS_K Key;
	T_ALCS_D Data;
	Data.Initialize();
	CDBDoc* pDoc = m_pDoc;

	long* KeyBuf = new long[nNumData];
	long nCount = 0;
	POSITION pos = pDoc->m_pAttrCtrl2->GetStartAlcs();
	while (pos != NULL)
	{
		pDoc->m_pAttrCtrl2->GetNextAlcs(pos, Key, Data);

		if(Data.nRatingType == 1) continue;

		KeyBuf[nCount++] = Key;
	}
	qsort(KeyBuf, nNumData, sizeof(T_LCOM_K), CTBBrowserWnd::CompareKey);

	for (nCount = 0; nCount < nNumData; nCount++)
	{
		pDoc->m_pAttrCtrl2->GetAlcs(KeyBuf[nCount], Data);

		if(Data.nRatingType == 1) continue;

		SetValue(GetParam()->GetData(), nCount + 1, KeyBuf[nCount], Data, awCols);
	} // end of loop nCount

	delete[]KeyBuf;
}

void CAssessmentLoadCombMainGridSTLNR::MakeItemEditingRecord(ROWCOL ncRow, CStringArray& EditingRecord)
{
	CDBDoc* pDoc = m_pDoc;
	T_ALCS_K Key;
	T_ALCS_D Data;
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
// CAssessmentLoadCombMainGridSTLNR Implementation
BOOL CAssessmentLoadCombMainGridSTLNR::GetValue(ROWCOL nRow, T_ALCS_K &key, T_ALCS_D &data)
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

		if (i == 0)
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
	((CAssessmentLoadCombSubGridSTLNR*)m_pSubGrid)->GetData(data);
	m_pParent->GetData(data);

	return TRUE;
}

BOOL CAssessmentLoadCombMainGridSTLNR::SetValue(ROWCOL nRow, const T_ALCS_K &key, const T_ALCS_D &data)
{
	//
	CRowColArray awCols;
	awCols.SetSize(GetColCount());
	for (int i = 0; i < GetColCount(); i++)
		VERIFY(GetColumnNoFor(m_aTitle[i], awCols[i]));
	return SetValue(GetParam()->GetData(), nRow, key, data, awCols);
}

BOOL CAssessmentLoadCombMainGridSTLNR::SetValue(CGXData* pData, ROWCOL nRow, const T_ALCS_K &key, const T_ALCS_D &data, CRowColArray &awCols)
{
	//
	CString str;
	for (int i = 0; i < GetColCount(); i++)
	{
		if (i == 0)
		{
			if (key == 0) str.Empty();
			else str.Format(_T("%d"), key);
		}
		else ConvDataToStr(i, data, str);
		pData->StoreValueRowCol(nRow, awCols[i], str, gxOverride);

		// END KJH20050203
	}

	return TRUE;
}

void CAssessmentLoadCombMainGridSTLNR::ConvStrToData(int i, CString& value, T_ALCS_D& data)
{
	if (i == D_ALCOM_COL_NAME - 1) data.strCaseName = value;
	else if (i == D_ALCOM_COL_LIM_STATE - 1) data.nLimitState = GetLimState(value);
	else if (i == D_ALCOM_COL_COMB_TYPE - 1) data.nCombType = GetCombType(value);
	else if (i == D_ALCOM_COL_DESC - 1) data.strDescription = value;
}

void CAssessmentLoadCombMainGridSTLNR::ConvDataToStr(int i, const T_ALCS_D& data, CString& value)
{
	if (i == D_ALCOM_COL_NAME - 1) value = data.strCaseName;
	else if (i == D_ALCOM_COL_LIM_STATE - 1) value = GetLimStateStr(data.nLimitState);
	else if (i == D_ALCOM_COL_COMB_TYPE - 1) value = GetCombTypeStr(data.nCombType);
	else if (i == D_ALCOM_COL_DESC - 1) value = data.strDescription;
}

// Main Grid, Sub Grid간의 동기화  
void CAssessmentLoadCombMainGridSTLNR::OnChangeCurrentRecord(ROWCOL nRow)
{
	ROWCOL nkCol;
	GetColumnNoFor(GetKeyColID(), nkCol);
	CString sKey = GetKeyForRow(nRow, nkCol);
	m_pSubGrid->SetData(sKey);
	m_pParent->SetData(_ttoi(sKey));

	T_ALCS_K key;
	T_ALCS_D data;
	data.Initialize();
	key = _ttol(sKey);
	m_pDoc->m_pAttrCtrl2->GetAlcs(key, data);

	m_bIgnoreSetEditMode = TRUE;
	//m_pEdtImpactFactor->SetWindowText(szTemp);
	m_bIgnoreSetEditMode = FALSE;
}


// LoadComb의 위치를 이동(Serial No가 변경)한다.
BOOL CAssessmentLoadCombMainGridSTLNR::OnSelDragRowsDrop(ROWCOL nFirstRow, ROWCOL nLastRow, ROWCOL nDestRow)
{
	// 수정 모드이면 에러 리턴
	if (IsEdit())
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Can_t_Move_row_while_editing_));
		return FALSE;
	}

	// Append Row의 뒤면 맨 마지막으로 이동하는 것으로 한다.
	if (nDestRow == GetRowCount() + 1) nDestRow--;

	// 이동의 효과가 없는 곳으로 이동(선택범위내로 다시 이동)하면 그냥 리턴
	if (nDestRow >= nFirstRow && nDestRow <= nLastRow + 1) return FALSE;

	// 연속 선택이 아니면 이동을 금지시킨다.
	CRowColArray awRows;
	GetSelectedRows(awRows, TRUE, FALSE);
	if (awRows.GetSize() != nLastRow - nFirstRow + 1) return FALSE;


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
	CArray<T_ALCS_D, T_ALCS_D&> aData;
	CArray<T_ALCS_K, T_ALCS_K> aKey;
	CString LoadCombName;
	T_ALCS_K key;
	T_ALCS_D data;
	data.Initialize();
	for (int i = nFirstRow; i <= nLastRow; i++)
	{
		GetValue(i, key, data);
		// Get Value만 하면 Combination이 현재 Record의 Combination으로 치환된다.
		// 여기서는 위치만 이동되므로 LoadCombId만 변경된다. 그래서 원래의 Combination
		// 을 유지하기 위해 GetLcom을 다시 호출한다.
		m_pDoc->m_pAttrCtrl2->GetAlcs(key, data);
		//data.LoadCombId = nStart;
		nStart += nInc;

		aKey.Add(key);
		aData.Add(data);
	}

	m_nUpdateState = 0;
	BOOL bReturn = m_pDoc->m_pDataCtrl->ModifyAlcs(aKey, aKey, aData);
	m_nUpdateState = 1;
	return bReturn;
}

CString CAssessmentLoadCombMainGridSTLNR::GetNewKey()
{
	int i;

	CArray<T_ALCS_K, T_ALCS_K> aKeyList;
	m_pDoc->m_pAttrCtrl2->GetAlcsKeyList(aKeyList);

	// aKeyList가 Sorting이 되어 있다고 가정..
#ifdef _DEBUG
	for (i = 0; i < aKeyList.GetSize(); i++)
	{
		for (int j = i + 1; j < aKeyList.GetSize(); j++)
		{
			ASSERT(aKeyList[i] < aKeyList[j]);  // 여기에 ASSERT가 걸린다면 aKeyList를 Sorting 해야 함..
		}
	}
#endif

	T_ALCS_K KeyTemp = 1;
	for (i = 0; i < aKeyList.GetSize(); i++)
	{
		if (aKeyList[i] != KeyTemp)
		{
			break;
		}
		KeyTemp++;
	}

	if (aKeyList.GetSize())
	{
		if (aKeyList[0] < 1)
		{
			ASSERT(0);//         hslee : Key가 0부터 시작하는 경우... 여기 걸린다면 원인을 찾아서 Key가 1부터 시작하도록 고치자..
			ASSERT(aKeyList[0] == 0); // hslee : Key가 음수인 경우... 여기 걸린다면 원인을 찾아서 Key가 1부터 시작하도록 고치자..
			KeyTemp = aKeyList[0];
		}
	}

	CString szTemp;
	szTemp.Format(_T("%d"), KeyTemp);
	return szTemp;
}
