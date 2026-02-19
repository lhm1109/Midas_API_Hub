// RegressionResultGrid.cpp: implementation of the CDgnMPhiCurveGrid class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnMPhiCurveGrid.h"
#include "DgnMPhiCurveDlg.h"

#include "..\wg_base\wg_base_CompFunc.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl2.h"
#include "..\wg_db\wg_db_UnitCtrl.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDgnMPhiCurveGrid::CDgnMPhiCurveGrid(CDgnMPhiCurveDlg *pParent)
{
	m_pParent = NULL;
	ASSERT(pParent);
	m_pParent = pParent;

	m_bSelCalc = FALSE;

	m_aSelectedMpccKey.RemoveAll();

	m_pDoc = CDBDoc::GetDocPoint();
}

CDgnMPhiCurveGrid::~CDgnMPhiCurveGrid()
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDgnMPhiCurveGrid::Initialize()
{
	CTBBrowserWnd::Initialize();// CTBBrowserWndUnit 초기화시에는 뒷배경이 있으며 첫번째 Column은 상태표시가 들어감

	LockUpdate(TRUE);

	// undo를 금지시킨다.
	GetParam()->EnableUndo(FALSE);  

	// cell의 width와 height 크기 변경을 금지한다.
	GetParam()->EnableTrackRowHeight(FALSE);
	GetParam()->EnableTrackColWidth(FALSE);

	// Row와  Column의 이동을 금지시킨다.
	GetParam()->EnableMoveRows(FALSE);
	GetParam()->EnableMoveCols(FALSE);

	// double click시 셀 편집이 되도록 한다.
	GetParam()->SetActivateCellFlags(GX_CAFOCUS_DBLCLICKONCELL);

	// MemoryDC를 사용한다.
	SetDrawingTechnique(gxDrawUsingMemDC);
		
	// 헤더의 Current cell을 막는다.
	ColHeaderStyle().SetEnabled(FALSE);
	RowHeaderStyle().SetEnabled(FALSE);


	CGXProperties* pProp = GetParam()->GetProperties();
	pProp->SetMarkColHeader(FALSE); // Turn off pressed button effect for column headers
	pProp->SetMarkRowHeader(FALSE);
	pProp->SetUserProperty(GX_IDS_OUTLINECURRENTCELL, // 현재 셀의 border 설정 변경
												(CGXStyle) pProp->sInvertNoBorder);

	// 표준 스타일 변경
	ChangeColHeaderStyle(CGXStyle().SetFont(CGXFont_GC().SetBold(FALSE)));

	SetRowCount(1); // Header를 제외한 갯수
	SetColCount(4); // Header를 제외한 갯수
	// Row_Grid Size
	SetRowHeightByDPI(0, 0, 18);

	// Hide Mpcc Key Column
	HideCols(0, 0, TRUE);

	// Col_Grid Size
	SetColWidthByDPI(0, 0, 8);
	SetColWidthByDPI(1, 1, 100); 
	SetColWidthByDPI(2, 2, 18);
	SetColWidthByDPI(3, 3, 161);
	SetColWidthByDPI(4, 4, 50);

	SetHeaderTitle();

	HideCols(1, 1);

	//
	SetStyleRange(CGXRange().SetCols(0), CGXStyle()
		.SetHorizontalAlignment(DT_CENTER)
		.SetValueType(GX_VT_STRING)
		);

	// Key
	SetStyleRange(CGXRange().SetCols(1), CGXStyle()
		.SetHorizontalAlignment(DT_CENTER)
		.SetValueType(GX_VT_STRING)
		);
	
	// Check Box
	SetStyleRange(CGXRange().SetCols(2), CGXStyle()
		.SetControl(GX_IDS_CTRL_CHECKBOX3D)
		.SetValueType(GX_VT_STRING)
		.SetEnabled(TRUE)
		.SetHorizontalAlignment(DT_CENTER)
		.SetVerticalAlignment(DT_VCENTER)
		);

	// Name
	SetStyleRange(CGXRange().SetCols(3), CGXStyle()
		.SetEnabled(TRUE)
		.SetControl(GX_IDS_CTRL_EDIT)
		.SetValueType(GX_VT_STRING)
		.SetHorizontalAlignment(DT_LEFT)
		.SetFormat(GX_FMT_FIXED)
		);

	// Result
	SetStyleRange(CGXRange().SetCols(4), CGXStyle()
		.SetEnabled(TRUE)
		.SetControl(GX_IDS_CTRL_EDIT)
		.SetValueType(GX_VT_STRING)
		.SetHorizontalAlignment(DT_CENTER)
		);
	
	SetStyleRange(CGXRange().SetRows(0), CGXStyle().SetWrapText(TRUE)); // Header만 2줄쓰기 가능
	
	SetScrollBarMode(SB_VERT, gxnEnabled | gxnEnhanced);

	LockUpdate(FALSE);

	// Default Cell Position
	SetCurrentCell(1,1);

	// Redraw();
	MakeItemEx();
}

/////////////////////////////////////////////////////////////////////////
void CDgnMPhiCurveGrid::SetHeaderTitle() // Setting Header Title
{
	// Set RbarTable Header!!
	// Set RowHeader
	SetStyleRange(CGXRange().SetRows(0), CGXStyle()
		.SetHorizontalAlignment(DT_CENTER)
	  .SetVerticalAlignment(DT_VCENTER)
		.SetFont(CGXFont_GC().SetBold(FALSE).SetSize(9))
		);

	// Column의 이름
	SetStyleRange(CGXRange(0, 1), CGXStyle().SetValue(_LS(IDS_DGN_MPHI_CURVE_GRID_KEY)));
	SetStyleRange(CGXRange(0, 2), CGXStyle().SetValue(_T("")));
	SetStyleRange(CGXRange(0, 3), CGXStyle().SetValue(_LS(IDS_DGN_MPHI_CURVE_GRID_NAME)));
	SetStyleRange(CGXRange(0, 4), CGXStyle().SetValue(_LS(IDS_DGN_MPHI_CURVE_GRID_RESULT)));
}
/////////////////////////////////////////////////////////////////////////

void CDgnMPhiCurveGrid::MakeItemEx()
{
	DeleteAllRecords();

	CArray<T_MPCC_K, T_MPCC_K> aMpccKey;
	m_pDoc->m_pAttrCtrl2->GetMpccKeyList(aMpccKey);
	
	int nSize = aMpccKey.GetSize();
	BOOL bCheck = FALSE;
	CString strName;
	CString strKey;
	T_MPCC_D MpccData;

	if(nSize > 0)
	{
		SetStyleRange(CGXRange().SetCols(3), CGXStyle().SetReadOnly(FALSE));
		SetStyleRange(CGXRange().SetCols(4), CGXStyle().SetReadOnly(FALSE));

		SetRowCount(1 + nSize);
		for(int i=0; i<nSize; i++)
		{
			bCheck = FALSE;
			m_pDoc->m_pAttrCtrl2->GetMpcc(aMpccKey[i], MpccData);
			strName = MpccData.strName;

			strKey.Format(_T("%d"), aMpccKey[i]);
		
			SetValueRange(CGXRange(i+1, 1), strKey);

			for(int j=0; j<m_aSelectedMpccKey.GetSize(); j++)
			{
				if(m_aSelectedMpccKey[j] == aMpccKey[i]) bCheck = TRUE;
			}

			if(bCheck) SetValueRange(CGXRange(i+1, 2), _T("1"));
			else       SetValueRange(CGXRange(i+1, 2), _T("0"));

			SetValueRange(CGXRange(i+1, 3), strName);

			if(!m_bSelCalc) 
				SetValueRange(CGXRange(i+1, 4), _T("X"));
			else            
				SetValueRange(CGXRange(i+1, 4), (m_aResultIndex.GetSize() > i) ? (m_aResultIndex[i]==0 ? _T("X") : _T("O")) : _T("X"));
		}
		SetStyleRange(CGXRange().SetCols(3), CGXStyle().SetReadOnly(TRUE));
		SetStyleRange(CGXRange().SetCols(4), CGXStyle().SetReadOnly(TRUE));
	}
	
	HideRows(GetRowCount(), GetRowCount(), TRUE); // to Delete making Row When Checking Latest Row
}

// Delete 를 막아줌 
BOOL CDgnMPhiCurveGrid::ProcessKeys(CWnd *pSender, UINT nMessage, UINT nChar, UINT nRepCnt, UINT flags)
{
	if((nMessage == WM_KEYDOWN) && (nChar == VK_DELETE)) return TRUE;
	 
	return CGXGridWnd::ProcessKeys(pSender, nMessage, nChar, nRepCnt, flags); 
}

BOOL CDgnMPhiCurveGrid::OnLButtonClickedRowCol(ROWCOL nRow, ROWCOL nCol, UINT nFlags, CPoint pt)
{
	BOOL bRet = CGXGridWnd::OnLButtonClickedRowCol(nRow, nCol, nFlags, pt);

	if(nRow==0)	return FALSE;
	
	T_MPCC_K MpccKey;

	CString value;
	value = GetValueRowCol(nRow, 1);

	MpccKey = _ttoi(value);

	m_pParent->GetGridData(MpccKey);

	return bRet;
}

BOOL CDgnMPhiCurveGrid::GetStyleRowCol(ROWCOL nRow, ROWCOL nCol, CGXStyle& style, GXModifyType mt, int nType)
{
	BOOL bCurrentRow;
	BOOL bCurrentCell;
	BOOL bEnabled;
	BOOL bHeaderCol;
	BOOL bHeaderRow;

	ROWCOL ncRow, ncCol;    // 현재 Row, Col
	CGXGridWnd::GetStyleRowCol(nRow, nCol, style, mt, nType);

	// 현재 셀의 그리드 좌표(Row, Col 번호)를 얻어온다.
	if (!GetCurrentCell(ncRow, ncCol)) 
		ncRow = GetRowCount()+1; // 현재 셀이 없는 경우 : ncRow를 범위 밖으로 설정

	// 현재 셀이 이동되면 새로 그려준다.
	m_bRefreshOnSetCurrentCell = TRUE;

	// Row나 Col이 1보다 작으면 리턴한다.(Standard Row or Column Header)
	if (nRow < 1 || nCol < 1) return TRUE;

	// Get Current Table and Cell State
	bCurrentRow = (nRow == ncRow);
	bCurrentCell = (nRow == ncRow && nCol == ncCol);

	// Enable, ReadOnly와 같은 Cell의 상태를 조사한다.
	// ComposeStyleRowCol은 호출해서는 안된다.
	// 그래서 여기서는 직접 조사한다.
	// 단 Cell Style, Column Style만 조사한다.
	if (style.GetIncludeEnabled())  // 셀에 Enable정보 포함 여부 알아냄
	{
		bEnabled = style.GetEnabled();
	}
	else // Column & Row style에서 enable 정보 알아냄
	{
		CGXStyle styleEx;
		GetColStyle(nCol, styleEx);
		bEnabled = (!styleEx.GetIncludeEnabled() || styleEx.GetEnabled());
		GetRowStyle(nRow, styleEx);
		bEnabled = (!styleEx.GetIncludeEnabled() || styleEx.GetEnabled());
	}

	bHeaderCol = IsFrozenCol(nCol);
	bHeaderRow = IsFrozenRow(nRow);

	// Printing 모드이면 헤더 Column이 프린트 되기 위해 다음과 같이 리턴한다.
	if (IsPrinting())
	{
		if (bHeaderCol) style.SetControl(GX_IDS_CTRL_HEADER);
		return TRUE;
	}

	// 일반 셀이 가장 많으므로 빨리 검사하고 바로 리턴할 수 있게 코드를 앞으로 뺀다.
	if (bEnabled && !bCurrentCell && !bCurrentRow && !bHeaderCol)
	{
		style
			.SetInterior(COLORREF(RGB(192, 220, 192)))
			.SetTextColor(COLORREF(RGB(0, 0, 0)));

		return TRUE;
	}

	if (bCurrentCell) style
		.SetInterior(COLORREF(RGB(255, 251, 240)))
		.SetTextColor(COLORREF(RGB(0, 0, 0)));
	else if (bCurrentRow) style
		.SetInterior(COLORREF(RGB(166, 202, 240)))
		.SetTextColor(COLORREF(RGB(0, 0, 0)));
	else if (bHeaderCol || bHeaderRow) style    // Enabled Header Col(Only Frozen)
		.SetInterior(COLORREF(RGB(166, 202, 240)))
		.SetTextColor(COLORREF(RGB(0, 0, 0)));

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CDgnMPhiCurveGrid::GetSelectedMpccKeyList(CArray<T_MPCC_K, T_MPCC_K> &aMpccKey)
{
	aMpccKey.RemoveAll();

	int nRow = GetRowCount() - 1;
	int nCheck = 0;
	CString value;
	CString Check;
	T_MPCC_K MpccKey;

	if(nRow == 0) return FALSE;

	for(int i=0; i<nRow; i++)
	{
		// Check
		Check = GetValueRowCol(i+1 , 2);
		nCheck = _ttoi(Check);

		if(nCheck)
		{
			// Key
			value = GetValueRowCol(i+1 , 1);
			MpccKey = _ttoi(value);
			aMpccKey.Add(MpccKey);
		}
	}

	return TRUE;
}

BOOL CDgnMPhiCurveGrid::SetSelectedMpccKey(T_MPCC_K MpccKey)
{
	CString value;
	int nRow = GetRowCount() - 1;
	for(int i=0; i<nRow; i++)
	{
		// Key
		value = GetValueRowCol(i+1 , 1);
		if(MpccKey == _ttoi(value))
		{
			SetCurrentCell(i+1 , 1);
			return TRUE;
		}
	}
	return FALSE;
}

BOOL CDgnMPhiCurveGrid::GetSelectedMpccKey(T_MPCC_K &MpccKey)
{
	ROWCOL nRow;
	ROWCOL nCol;

	GetCurrentCell(nRow, nCol);

	if(nRow < 1 || nCol < 1) return FALSE;

	CString value;
	value = GetValueRowCol(nRow, 1);
	MpccKey = _ttoi(value);

	return TRUE;
}

BOOL CDgnMPhiCurveGrid::SelectAllorUnSelectAll(BOOL bSelect)
{
	int nRow = GetRowCount() - 1;
	for(int i=0; i<nRow; i++)
	{
		if(bSelect) SetStyleRange(CGXRange(i+1, 2), _T("1"));
		else        SetStyleRange(CGXRange(i+1, 2), _T("0"));
	}
	return TRUE;
}

BOOL CDgnMPhiCurveGrid::GetMpccData(CArray<T_MPCC_K, T_MPCC_K> &aMpccKey, CArray<CString, CString> &aNameList)
{
	aMpccKey.RemoveAll();
	aNameList.RemoveAll();

	int nRow = GetRowCount() - 1;
	CString value;
	CString Name;
	T_MPCC_K MpccKey;

	if(nRow == 0) return FALSE;

	for(int i=0; i<nRow; i++)
	{
		// Key
		value = GetValueRowCol(i+1 , 1);
		MpccKey = _ttoi(value);
		aMpccKey.Add(MpccKey);
		
		// Name
		Name = GetValueRowCol(i+1, 3);
		aNameList.Add(Name);
	}

	return TRUE;
}

BOOL CDgnMPhiCurveGrid::SetResultIndex(CArray<T_MPCC_K, T_MPCC_K> &aResultMpccKey)
{
	m_aResultIndex.RemoveAll();
	
	int nRow = GetRowCount() - 1;
	CString value;
	T_MPCC_K MpccKey;
	BOOL bClac = FALSE;

	m_bSelCalc = TRUE;

	for(int i=0; i<nRow; i++)
	{
		bClac = FALSE;

		// Key
		value = GetValueRowCol(i+1 , 1);
		MpccKey = _ttoi(value);

		for(int j=0; j<aResultMpccKey.GetSize(); j++)
		{
			if(MpccKey == aResultMpccKey[j])
			{
				bClac = TRUE;
				break;      
			}
		}
		m_aResultIndex.Add(bClac);
	}

	return TRUE;
}

BOOL CDgnMPhiCurveGrid::SetSelectedKey(CArray<T_MPCC_K, T_MPCC_K> &aSelectedMpccKey)
{
	m_aSelectedMpccKey.RemoveAll();

	for(int i=0; i<aSelectedMpccKey.GetSize(); i++)
	{
		m_aSelectedMpccKey.Add(aSelectedMpccKey[i]);
	}

	return TRUE;
}
