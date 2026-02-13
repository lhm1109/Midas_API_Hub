// RatingLoadCombCommonMainGrid.cpp : implementation file
//

#include "stdafx.h"

#include "wg_dgn.h"
#include "RatingLoadCombCommonMainGrid.h"

#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_LoadDB.h"
#include "..\wg_db\wg_db_LoadDgnCodeCheck.h"

#include "RatingLoadCombCommonSubGrid.h"
#include "..\wg_base\wg_base_ServiceProvider.h"
#include "..\wg_tb\Tb_ServiceDef.h"
#include "..\wg_db\AttrCtrl2.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CRatingLoadCombCommonMainGrid
CRatingLoadCombCommonMainGrid::CRatingLoadCombCommonMainGrid()
:m_bElastStag(FALSE), m_bTB10002_1_05(FALSE)
{
	m_aCurDefVal[0] = _T("");
	m_aCurDefVal[1] = _T("");
	m_aCurDefVal[2] = D_TYPE_DEF;
	m_aCurDefVal[3] = _T("");
	m_aCurDefVal[4] = _T("");
	m_aCurDefVal[5] = _T("0");

	//m_nLcomType = D_LCOMTYPE_GENERAL;
	m_pSubGrid = 0;
	m_pSubGridMV = 0;
	
	m_bIgnoreSetEditMode = FALSE;

	m_aCurDefVal[4] = _LS(IDS_CMD_LCOM_ACTIVE);

	AddColInfo(_ULS(Key), t_integer, 0);
	AddColInfo(_LS(IDS_WG_CMD__ADDD__No),             t_integer, 0);
	AddColInfo(_LS(IDS_CMD_NAME_OF_RATING_CASE),      t_string,  0);
	AddColInfo(_LS(IDS_WG_CMD__ADDD__Type),           t_string,  0);
	AddColInfo(_LS(IDS_WG_CMD__ADDD__Description),    t_string,  0);
	AddColInfo(_LS(IDS_WG_CMD__ADDD__Active),         t_integer, 0);
	AddColInfo(_LSX(E),                               t_integer, 0);  // PMS:XXXX-JWKWON-20091014 : 문자열 번역 안하는 걸로

	m_aPrimaryKey.Add(0);

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

CRatingLoadCombCommonMainGrid::~CRatingLoadCombCommonMainGrid()
{
}

#define CWnd CTBCommon
BEGIN_MESSAGE_MAP(CRatingLoadCombCommonMainGrid, CWnd)
#undef CWnd
	//{{AFX_MSG_MAP(CRatingLoadCombCommonMainGrid)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CRatingLoadCombCommonMainGrid message handlers
void CRatingLoadCombCommonMainGrid::Initialize(CDBDoc* pDoc, T_UNIT_SYSTEM &UnitSystem)
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
		(CGXStyle)pProp->sInvertNoBorder);

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

	auto L_SetColWidth =[this] (ROWCOL nColSt, ROWCOL nColEd, int nWidth)
	{
		SetColWidth(nColSt, nColEd, globalUtils.ScaleByDPI(nWidth));
	};
	// Column 크기 설정
	L_SetColWidth(1, 1, 0);
	L_SetColWidth(2, 2, 10);
	L_SetColWidth(3, 3, 158);
	L_SetColWidth(4, 4, 75);
	L_SetColWidth(5, 5, 230);
	L_SetColWidth(6, 6, 55);
	L_SetColWidth(7, 7, 25); // MNET:1209, KJH20050203

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

BOOL CRatingLoadCombCommonMainGrid::GetRowNoByKey(UINT key, ROWCOL& nRow)
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

BOOL CRatingLoadCombCommonMainGrid::GetInsertRowByKey(UINT key, ROWCOL& nRow)
{
	if(GetRowCount() == 1)
	{
		nRow = 1;
		return TRUE;
	}

	ROWCOL nColKey;
	UINT EachKey;
	GetColumnNoFor(m_aTitle[0], nColKey);
	for(ROWCOL nRowLoop = 1; nRowLoop < GetRowCount(); nRowLoop++)
	{
		const CString& szKey = GetValueRowCol(nRowLoop, nColKey);
		
		EachKey = _ttoi(szKey);
	
		if(EachKey == key)
		{
			nRow = nRowLoop;
			return TRUE;
		}
	}
	nRow = m_LastDelRow;	// m_LastDelRow를 Return 해주는 이유 :
												// Modify할때 대화상자클래스 UpdateBuffer()함수에서 DeleteRecordByKey()이후 InsertRecordByKey()를 하는데
												// InsertRecordByKey()를 할 때 이미 지워버린 Row를 이 함수를 통해 찾으려고 시도 하기 때문에 찾지를 못함
	                      // 그래서 Row를 못찾았을 때는 직전에 지웠던 Row를 찾는 시도라고 봄

	//nRow = nRowLoop;
	return TRUE;
}

void CRatingLoadCombCommonMainGrid::ChangeLcomType(int nLcomType)
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
// CRatingLoadCombCommonMainGrid Overridable Functions
void CRatingLoadCombCommonMainGrid::OnChangedSelectionPublic(long *keys, int nSize)
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

long CRatingLoadCombCommonMainGrid::GetSelectedKeys(CArray<long, long> &caKey)
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

BOOL CRatingLoadCombCommonMainGrid::ValidateField(CString value, int nColID)
{
	if (nColID >= GetColCount())
	{
		//SetWarningText(_LS(IDS_WG_CMD__ADDD___CRatingLoadCombCommonMainGrid__ValidateFie));
		SetWarningText(_T("Invalid date!"));
		return FALSE;
	}

	return TRUE;
}

// 삽입이나 수정전에 Record의 Validation을 검사한다.
BOOL CRatingLoadCombCommonMainGrid::ValidateRecord(CStringArray &value, CRowColArray &aCols)
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
    int i = 0;
	for (i = 0; i < m_pSubGrid->GetRowCount()-1; i++)
	{
		CString szDebug = m_pSubGrid->GetValueRowCol(i+1, 1);
		if (szDebug == _T(""))
		{
			msg += ((msg != _T("")) ? _T("\n") : _T("")) + CString(_LS(IDS_WG_CMD__ADDD__Load_Case)) + _LS(IDS_WG_CMD__ADDD___has_not_been_entered_);
			break;
		}
		szDebug = m_pSubGrid->GetValueRowCol(i+1, 2);
		if (szDebug == _T(""))
		{
			msg += ((msg != _T("")) ? _T("\n") : _T("")) + CString(_LS(IDS_WG_CMD__ADDD__Factor)) + _LS(IDS_WG_CMD__ADDD___has_not_been_entered_);
			break;
		}
	}

	if(i == 0)
	{
		msg = CString(_LS(IDS_CMD_PJST_Static_Load_Case)) + _LS(IDS_WG_CMD__ADDD___has_not_been_entered_);
		SetWarningText(msg);
		return FALSE;
	}

	for (i = 0; i < m_pSubGridMV->GetRowCount() - 1; i++)
	{
		CString szDebug = m_pSubGridMV->GetValueRowCol(i+1, 1);
		if (szDebug == _T(""))
		{
			msg += ((msg != _T("")) ? _T("\n") : _T("")) + CString(_LS(IDS_WG_CMD__ADDD__Load_Case)) + _LS(IDS_WG_CMD__ADDD___has_not_been_entered_);
			break;
		}
		szDebug = m_pSubGridMV->GetValueRowCol(i+1, 2);
		if (szDebug == _T(""))
		{
			msg += ((msg != _T("")) ? _T("\n") : _T("")) + CString(_LS(IDS_WG_CMD__ADDD__Factor)) + _LS(IDS_WG_CMD__ADDD___has_not_been_entered_);
			break;
		}
	}

	if(i == 0)
	{
		msg = CString(_LS(IDS_CMD_LIVE_LOAD_CASE)) + _LS(IDS_WG_CMD__ADDD___has_not_been_entered_);
		SetWarningText(msg);
		return FALSE;
	}

	if (msg == _T("")) return TRUE;
	else SetWarningText(msg);
	return FALSE;
}

CString CRatingLoadCombCommonMainGrid::GetDefValue(int nIndex)
{
	return _T("");
	//return m_aCurDefVal[nIndex];
}

CString* CRatingLoadCombCommonMainGrid::GetDefValueArray()
{
	return m_aCurDefVal;
}

void CRatingLoadCombCommonMainGrid::MakeItemFiltered(long nFilteredNum, long* aFilteredKeys, CRowColArray& awCols)
{
	ASSERT(0);

}

void CRatingLoadCombCommonMainGrid::GetAllSelectedLcom(CArray<UINT, UINT>& aSelKey)
{
	for (int i = 0; i < m_aSelectedKeys.GetSize(); i++)
	{
		aSelKey.Add(m_aSelectedKeys[i]);
	}
}

// ProcessKeys, OnRButtonDblClk, OnLButtonDown은 Current Record 변경시 Grid간의
// 동기화를 맞추기 위해 Override했다.
// Current Record 변경시 OnChangeCurrentRecord 함수가 불리도록 설계했다.
BOOL CRatingLoadCombCommonMainGrid::ProcessKeys(CWnd *pSender, UINT nMessage, UINT nChar, UINT nRepCnt, UINT flags)
{
	CTBCommon::ProcessKeys(pSender, nMessage, nChar, nRepCnt, flags);

	CheckCurrentRecordChanged();
	return TRUE;
}


BOOL CRatingLoadCombCommonMainGrid::OnCanceledModify()
{
	ROWCOL ncRow, ncCol;
	GetCurrentCell(ncRow, ncCol);
	OnChangeCurrentRecord(ncRow);
	return TRUE;
}

CString CRatingLoadCombCommonMainGrid::GetKeyForRow(ROWCOL ncRow, ROWCOL nkCol)
{
	CString sKey;

	if (IsAppendRow(ncRow)) sKey = _LS(IDS_WG_CMD__ADDD__Append_Row);
	else if (m_nEditMode == addnew) sKey = _LS(IDS_WG_CMD__ADDD__New_Row);
	else if (m_nEditMode == edit) sKey = GetValueRowColBeforeEdit(ncRow, nkCol);
	else sKey = GetValueRowCol(ncRow, nkCol);

	return sKey;
}

// Current Record가 변경되었는지 검사
BOOL CRatingLoadCombCommonMainGrid::CheckCurrentRecordChanged()
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
	// END KJH20050203
	return FALSE;
}
BOOL CRatingLoadCombCommonMainGrid::IsActiveCol(ROWCOL& nCol)
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
BOOL CRatingLoadCombCommonMainGrid::OnSelDragColsDrop(ROWCOL nFirstCol, ROWCOL nLastCol, ROWCOL nDestCol)
{
	// Frozen 되어 있는 column 위치에 이동되면 이동을 금지시킨다.
	//if (IsFrozenCol(nDestCol)) return FALSE;
	if (nDestCol == 1 || nFirstCol == 1) return FALSE;
	if (nDestCol == 2 || nFirstCol == 2) return FALSE;
	return CTBCommon::OnSelDragColsDrop(nFirstCol, nLastCol, nDestCol);
}

void CRatingLoadCombCommonMainGrid::SetHeaderTitle(T_UNIT_SYSTEM &UnitSystem, BOOL bInit)
{
	SetHeaderTitle2((int*)m_aUnit.GetData(), bInit, TRUE);
}


int CRatingLoadCombCommonMainGrid::GetCombType(CString &value)
{
	if (value == D_LCOM_CT_ADD) return 0;
	else if (value == D_LCOM_CT_ENVELOPE) return 1;
	else if (value == D_LCOM_CT_ABS) return 2;
	else if (value == D_LCOM_CT_SRSS) return 3;

	return 0;
}

CString CRatingLoadCombCommonMainGrid::GetCombTypeStr(int nCombType)
{
	CString aCombTypeStr[] = { D_LCOM_CT_ADD, D_LCOM_CT_ENVELOPE, D_LCOM_CT_ABS, D_LCOM_CT_SRSS };
	if (nCombType >= 0 && nCombType <= 3) return aCombTypeStr[nCombType];
	else return _LS(IDS_WG_CMD__ADDD__Error);

}

CString CRatingLoadCombCommonMainGrid::GetActiveChoiceList(int nLcomType)
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

int CRatingLoadCombCommonMainGrid::GetActive(int nLcomType, CString& value)
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

CString CRatingLoadCombCommonMainGrid::GetActiveName(int nLcomType, int value)
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

CString CRatingLoadCombCommonMainGrid::GetLcomActiveDef(int nLcomType)
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

BOOL CRatingLoadCombCommonMainGrid::OnPasteFromClipboard(const CGXRange &range)
{
	//AfxMessageBox(_LS(IDS_CMD_DONT_SUPPORT_PASTE));
	AfxMessageBox(_T("Not Supported!"));
	return FALSE;
}

CString CRatingLoadCombCommonMainGrid::GetLcomTypeList(int nLcomType)
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


void CRatingLoadCombCommonMainGrid::OnLButtonDblClk(UINT nFlags, CPoint point) 
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
void CRatingLoadCombCommonMainGrid::OnLButtonDown(UINT nFlags, CPoint point) 
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

void CRatingLoadCombCommonMainGrid::SetModeEdit()
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


BOOL CRatingLoadCombCommonMainGrid::DeleteRecord( ROWCOL nRow )
{
	BOOL bReturn = CTBCommon::DeleteRecord(nRow);
	if(bReturn)
	{
		m_LastDelRow = nRow;  // Modify명령시 이상한 예외가 있어서 이렇게 처리함.. GetInsertRowByKey()에 달아놓은 주석을 참고 하세요..
	}
	return bReturn;
}
