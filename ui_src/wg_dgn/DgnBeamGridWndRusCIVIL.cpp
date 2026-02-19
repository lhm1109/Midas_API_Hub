// DgnBeamGridWndRusCIVIL.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnBeamGridWndRusCIVIL.h"
#include "DgnConBeamBarDlgCIVIL.h"

#include "..\wg_common\wg_common_TBCommon.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_DBLib.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_ViewBuff.h"
#include "..\wg_db\wg_db_UnitCtrl.h"

#include "..\wg_base\wg_base_CompFunc.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDgnBeamGridWndRusCIVIL

CDgnBeamGridWndRusCIVIL::CDgnBeamGridWndRusCIVIL(T_RCHK_BEAM* pBeamData, CWnd* pParent)
{
	m_pBeamData = NULL;
	ASSERT(pBeamData);
	m_pBeamData = pBeamData;

	m_pParent = NULL;
	//ASSERT(pParent);
	//m_pParent = pParent;

	m_nOldCol = -1;

	m_nColCount  = 7;
	m_nColHeader = 8;   // extra column header(+default header 1)
	m_nTopBtm = 0;
}

CDgnBeamGridWndRusCIVIL::~CDgnBeamGridWndRusCIVIL()
{
}


#define CWnd CGXGridWnd
BEGIN_MESSAGE_MAP(CDgnBeamGridWndRusCIVIL, CWnd)
#undef CWnd
	//{{AFX_MSG_MAP(CDgnBeamGridWndRusCIVIL)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CDgnBeamGridWndRusCIVIL message handlers

//---------------------------------------------------------------------------
// InitSizeComBoBox
//---------------------------------------------------------------------------
void CDgnBeamGridWndRusCIVIL::InitSizeComBoBox()
{
	CStringArray strRebarName;
	BOOL bGetOK = CDBLib::GetRebarNameListByCode(strRebarName);
	ASSERT(bGetOK);
	int nCount = strRebarName.GetSize();

	CString szNameList1 = _T("");
	CString szNameList2 = _T("");     szNameList2+=_T("") + CString(_T("\n"));
	for(int j=0; j<nCount; j++)
	{
		szNameList1 += strRebarName.GetAt(j) + CString(_T("\n"));
		szNameList2 += strRebarName.GetAt(j) + CString(_T("\n"));
	}
	for(int i=1; i<T_RCHK_BEAM_LAYER; i++) 
	{
		SetStyleRange(CGXRange(i+1,2), CGXStyle().SetChoiceList(szNameList1));		
		SetStyleRange(CGXRange(i+1,3), CGXStyle().SetChoiceList(szNameList2));		
	}
}

void CDgnBeamGridWndRusCIVIL::SetLayerNum(UINT nLayer)
{
	CString strTemp;
	for(int nRow = 2; nRow <= T_RCHK_BEAM_LAYER + 1; nRow++)
	{
		strTemp.Format(_T("%d"), nRow - 1);
		SetExpressionRowCol(nRow, 0, strTemp);
		SetEnableByMod(nRow, 5);
	}

	HideRows(1, nLayer + 1, FALSE);

	if(nLayer<0 || nLayer>T_RCHK_BEAM_LAYER-1) return;

	HideRows(nLayer+2, T_RCHK_BEAM_LAYER+1, TRUE);

	for(int i=nLayer+2; i<=T_RCHK_BEAM_LAYER+1; i++)
	{
		SetValueRange(CGXRange(i, 1), double(0));
		SetValueRange(CGXRange(i, 2), _T(""));
		SetValueRange(CGXRange(i, 3), _T(""));
		SetValueRange(CGXRange(i, 4), double(0));
	}
}

void CDgnBeamGridWndRusCIVIL::OnModifyCell(ROWCOL nRow, ROWCOL nCol)
{/*
	m_nOldCol = nCol;

	if(nCol == 1) m_pParent->UpdateViewWnd();
*/
	CGXGridWnd::OnModifyCell(nRow, nCol);
}

void CDgnBeamGridWndRusCIVIL::OnInitCurrentCell(ROWCOL nRow, ROWCOL nCol)
{
	CGXGridWnd::OnInitCurrentCell(nRow, nCol);

//  if(m_nOldCol ==  3) m_pParent->UpdateViewWnd();
}

BOOL CDgnBeamGridWndRusCIVIL::OnPasteFromClipboard(const CGXRange &range)
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

	if (nPastedCols > 7)  
	{
		csErrMsg = _LS(IDS_TM_STDG_GRID_ERR_MSG5);
		goto PASTE_ERROR;
	}

	nTotalRow = GetRowCount();
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
	
	SetEnableAll();

	return TRUE;   

PASTE_ERROR:
	SetWarningText(csErrMsg);
	DisplayWarningText();
	x.DestroyWindow();
	return FALSE;
}

void CDgnBeamGridWndRusCIVIL::SetEnableAll()
{
	int nRowCnt = GetRowCount();
	for(int i = 2; i <= nRowCnt; i++)
	{
		SetEnableByMod(i, 5);		
	}
}

// Delete 를 막아줌 
BOOL CDgnBeamGridWndRusCIVIL::ProcessKeys(CWnd *pSender, UINT nMessage, UINT nChar, UINT nRepCnt, UINT flags)
{
	if((nMessage == WM_KEYDOWN) && (nChar == VK_DELETE)) return TRUE;
	 
	return CGXGridWnd::ProcessKeys(pSender, nMessage, nChar, nRepCnt, flags); 
}


// Add
//--------------------------------------------------------------------------
// Interface
void CDgnBeamGridWndRusCIVIL::Initialize()
{
	 
	CTBBrowserWnd::Initialize();
	SetColInfo();

	GetParam()->EnableMoveCols(FALSE);  // Disable moving column
	GetParam()->EnableMoveRows(FALSE);   // Disable moving row
	GetParam()->EnableTrackRowHeight(FALSE);  // Disable resizing row height
 	//GetParam()->EnableSelection(GX_SELROW);                       // Row Selection
	GetParam()->EnableTrackRowHeight(GX_TRACK_NOTHEADER);         // Row Size 변경금지
	GetParam()->EnableTrackColWidth(GX_TRACK_NOTHEADER);          // Col Size 변경금지
	GetParam()->SetActivateCellFlags(GX_CAFOCUS_DBLCLICKONCELL);
	SetDrawingTechnique(gxDrawUsingMemDC);                        // MemDC 사용

	ColHeaderStyle().SetEnabled(FALSE);
	RowHeaderStyle().SetEnabled(FALSE);

	//SetScrollBarMode(SB_VERT, gxnEnabled | gxnEnhanced);

	EnableInsertRowByPressingKey(FALSE);  // Disable inserting row by key
	MakeItemEx();
}

void CDgnBeamGridWndRusCIVIL::MakeItemEx()
{

}

//--------------------------------------------------------------------------
// Implementation
void CDgnBeamGridWndRusCIVIL::SetColInfo()
{
	//int nColNo = m_aColInfo.GetSize();

	BOOL bOldLock = LockUpdate(TRUE);

	// Set Title Header
	SetHeaderTitle();

	// 표준 스타일 변경
	ChangeStandardStyle(CGXStyle()
			.SetFont(CGXFont_GC().SetSize(10))
			.SetVerticalAlignment(DT_BOTTOM)
			.SetAllowEnter(FALSE));

	// 표준 스타일 변경
	ChangeStandardStyle(CGXStyle()
			.SetFont(CGXFont_GC().SetSize(9))
			.SetVerticalAlignment(DT_BOTTOM)
			.SetAllowEnter(FALSE));
	ChangeColHeaderStyle(CGXStyle().SetFont(CGXFont_GC().SetBold(TRUE)));
	ChangeRowHeaderStyle(CGXStyle()
					.SetFont(CGXFont_GC().SetBold(FALSE))
					.SetHorizontalAlignment(DT_RIGHT)
					.SetControl(GX_IDS_CTRL_HEADER));
	GetParam()->SetNumberedRowHeaders(TRUE);

	// 컬럼 스타일 변경
	SetStyleRange(CGXRange().SetCols(1, m_nColCount), CGXStyle()
			.SetHorizontalAlignment(DT_RIGHT)
			.SetValueType(GX_VT_NUMERIC));
	SetRowHeightByDPI(0, 0, 20);
	SetRowHeightByDPI(1, 7, 18);
	
	CStringArray aRBName;
	CDBLib::GetRebarNameListByCode(aRBName, TRUE);

	CString csChoiceList = _T("");
	int nSize = aRBName.GetSize();
	// dia.
	for (int i = 0; i < nSize; i++)
	{
		csChoiceList += aRBName[i]+_T("\n");
	}

	SetStyleRange(CGXRange().SetCols(0), CGXStyle()
			.SetHorizontalAlignment(DT_CENTER)
			.SetValueType(GX_VT_STRING)
			);

	// number
	SetStyleRange(CGXRange().SetCols(1), CGXStyle()
			.SetPlaces(0)); 
	
	// Size1, Size2.
	//.SetChoiceList(csChoiceList)
	SetStyleRange(CGXRange().SetCols(2,3), CGXStyle()
			.SetControl(GX_IDS_CTRL_TABBED_COMBOBOX)      
			.SetUserAttribute(GX_IDS_UA_TABLIST_KEYCOL, _T("0"))
			.SetUserAttribute(GX_IDS_UA_TABLIST_TEXTCOL, _T("0"))
			.SetUserAttribute(GX_IDS_UA_TABLIST_SHOWALLCOLS, _T("1"))
			.SetValueType(GX_VT_STRING)
			.SetHorizontalAlignment(DT_LEFT)
			.SetValue(_T(""))
			);
	
	//Dc
	SetStyleRange(CGXRange().SetCols(4), CGXStyle()
	    .SetControl(GX_IDS_CTRL_EDIT)    
			.SetValueType(GX_VT_NUMERIC)
			.SetHorizontalAlignment(DT_RIGHT)
			);
	
	// App
	SetStyleRange(CGXRange().SetCols(5), CGXStyle()
		.SetControl(GX_IDS_CTRL_CHECKBOX3D)
		.SetValueType(GX_VT_NUMERIC)
		.SetHorizontalAlignment(DT_CENTER)
		.SetVerticalAlignment(DT_VCENTER)
		.SetValue(_T("0")));
	
	// Num, Gap
	SetStyleRange(CGXRange().SetCols(6, 7), CGXStyle().SetHorizontalAlignment(DT_RIGHT).SetValueType(GX_VT_NUMERIC));


	/*
	// arrange
	csChoiceList.Format(_T("%s\n%s\n"), D_RPSC_ARRANGE_BOTH, D_RPSC_ARRANGE_ONE);
	SetStyleRange(CGXRange().SetCols(4), CGXStyle()
			.SetControl(GX_IDS_CTRL_TABBED_COMBOBOX)
			.SetChoiceList(csChoiceList)
			.SetUserAttribute(GX_IDS_UA_TABLIST_KEYCOL, _T("0"))
			.SetUserAttribute(GX_IDS_UA_TABLIST_TEXTCOL, _T("0"))
			.SetUserAttribute(GX_IDS_UA_TABLIST_SHOWALLCOLS, _T("1"))
			.SetValueType(GX_VT_STRING)
			.SetHorizontalAlignment(DT_LEFT)
			.SetValue(_T(""))
			);
			*/

	for(int i=1; i<T_RCHK_BEAM_LAYER+1; i++) 
	{
		SetStyleRange(CGXRange(i+1,2), CGXStyle().SetChoiceList(csChoiceList));		
		SetStyleRange(CGXRange(i+1,3), CGXStyle().SetChoiceList(csChoiceList));		
	}

	// 현재 셀의 border 설정 변경
	CGXProperties* pProp = GetParam()->GetProperties();
			pProp->SetUserProperty(GX_IDS_OUTLINECURRENTCELL,
		      (CGXStyle) pProp->sInvertNoBorder);

	SetStyleRange(CGXRange().SetRows(1), CGXStyle().SetIncludeDraw3dFrame(TRUE).SetHorizontalAlignment(DT_CENTER).SetValueType(GX_VT_STRING)
		.SetFormat(GX_FMT_FIXED).SetPlaces(0).SetEnabled(FALSE).SetDraw3dFrame(gxFrameRaised).SetIncludeInterior(TRUE).SetInterior(RGB(255, 0, 0)));
			
	SetFrozenRows(1, 1);

	// 현재 셀 설정
	SetRightValidCell(1, 1);
	GetParam()->SetHideCurrentCell(GX_HIDE_NEVER);

	

	LockUpdate(bOldLock);
	Redraw();
}

void CDgnBeamGridWndRusCIVIL::SetHeaderTitle()
{
	BOOL bOldLock = LockUpdate(TRUE);
	
	SetColCount(m_nColCount);
	SetRowCount(T_RCHK_BEAM_LAYER + 1);	

	// 단위 : 
	CString csLUnit, csTitle, strDtDb;
	strDtDb = (m_nTopBtm == 0) ? _LS(IDS_DGN_CON_BEAM_Dt) : _LS(IDS_DGN_CON_BEAM_Db);
	CString aTitle[] = { _LS(IDS_DGN_CON_BEAM_Layer), _LS(IDS_DGN_CON_BEAM_Num), _LS(IDS_DGN_CON_BEAM_Size1), _LS(IDS_DGN_CON_BEAM_Size2), strDtDb };
	int aWidth[] = {47, 47, 47, 47, 47, 46, 46, 46};
	
	for (int nCol = 0; nCol <= 4; nCol++)
	{
		SetCoveredCellsRowCol(0, nCol, 1, nCol);
		csTitle = aTitle[nCol];
		SetStyleRange(CGXRange(0, (ROWCOL)nCol, 1, (ROWCOL)nCol), CGXStyle()
				.SetValue(csTitle));
		SetColWidthByDPI((ROWCOL)nCol, (ROWCOL)nCol, aWidth[nCol]);
	}

 	SetCoveredCellsRowCol(0, 5, 0, 7);
	SetStyleRange(CGXRange(0, 5		), CGXStyle().SetValue(_LS(IDS_DGN_RATING_GROUP)));

	SetStyleRange(CGXRange(1, 5		), CGXStyle().SetValue(_LS(IDS_DGN_APP_DOT)));
	SetStyleRange(CGXRange(1, 6		), CGXStyle().SetValue(_LS(IDS_DGN_NUM_DOT)));
	SetStyleRange(CGXRange(1, 7		), CGXStyle().SetValue(_LS(IDS_DGN_GAP)));

	SetColWidthByDPI(5, 5, aWidth[5]);
	SetColWidthByDPI(5, 6, aWidth[6]);
	SetColWidthByDPI(5, 7, aWidth[7]);


	// Row Title
	CString strRowTitle;
	for(int i=1; i<T_RCHK_BEAM_LAYER + 1; i++)
	{
		strRowTitle.Format(_T("%d"), i+1);
		SetStyleRange(CGXRange(i+1, 0), CGXStyle()
			.SetValue(strRowTitle));
	}
	LockUpdate(bOldLock);
	//if (!bOldLock) Redraw();
}

BOOL CDgnBeamGridWndRusCIVIL::GetStyleRowCol(ROWCOL nRow, ROWCOL nCol, CGXStyle& style, GXModifyType mt, int nType)
{
	CTBBrowserWnd::GetStyleRowCol(nRow, nCol, style, mt, nType);
	
	if(nRow == 1)
	{
		style.SetInterior(CTBGrid::m_Color.RowColHeaderBg );
	}
	
	return TRUE;
}

// 인자로 넘어온 배열에 있는 모든 row를 Select 상태로 변경한다.
// 기존의 Select된 row는 없어진다.
void CDgnBeamGridWndRusCIVIL::SelectGridRow(long* aRow, long nSize)
{
	// Row 배열을 오름차순으로 sorting한다.
	qsort((void*)aRow, nSize, sizeof(long), CCompFunc::longAsc);

	CGXRangeList* pSelList = GetParam()->GetRangeList();
	pSelList->DeleteAll();  // 기존 선택을 해제한다.

	for (long i = 0; i < nSize; )
	{
		CGXRange* pRange = new CGXRange();
		// 연속된 Row를 찾아서 1개의 Range로 만든다.
        long j = 0;
		for ( j = i+1; j < nSize; j++)
		{
			if (aRow[j] != aRow[j-1] + 1) break;
		}
		pRange->SetRows(aRow[i], aRow[j-1]);
		pSelList->AddTail(pRange);
		i = j;
	}
}

BOOL CDgnBeamGridWndRusCIVIL::PasteFromClipboard(const CGXRange &range, CString* aDefVal, int nKeyCol, 
																				 CGXGridWnd &x, int &nPasteMode, int &nPastedCols)
{
	CGXRange r;
	r.top = r.bottom = 1;
	r.left = r.right = 1;
	x.Create(WS_CHILD, CRect(0, 0, 1, 1), GetParent(), 0);
	x.Initialize();
	x.GetParam()->EnableUndo(FALSE);
	x.LockUpdate(TRUE);
	x.SetRowCount(0); x.SetColCount(1);
	x.OnPasteFromClipboard(r);

	// Paste된 범위를 구한다.
	r.top = 1; r.left = 1;
	r.bottom = x.GetRowCount(); r.right = x.GetColCount();

	// 규칙
	// 범위가 다르면 경고 메세지를 보내고 리턴
	// Paste된 내용이 레코드의 모든 필드를 만족하지 못하면 Default 값으로 채운다.
	// column 구분이 없기 때문에 칼럼 by 칼럼으로 Paste한다.
	// 즉 column의 위치가 변경되었으면 변경된 대로 Paste 한다.
	// 고의적으로 Hide된 칼럼은 Default 값으로 채운다.

	// 용어
	// 원본 범위 : Copy해서 Clipboard로 들어간 row, col 갯수
	// 대상 범위 : User가 마우스로 선택해 놓은 범위

	// 범위에 고의로 숨겨진 Column(HideByIntend)이 포함되어 있으면 이 칼럼을
	// 삽입하고 Default Value로 채운다.
	CArray<int, int> aHideCol;
	int nColCount = GetColCount();
	int nPasteColCount = r.GetWidth();
	for (int k = 1; k < nPasteColCount; k++)
	{
		if (range.left+k < nColCount && IsHiddenByIntend(range.left+k))
		{
			x.InsertCols(k+1, 1);
			aHideCol.Add(range.left+k);
			r.right++;
			nPasteColCount++;
		}
	}

	CString sErrMsg = _T("");
	ROWCOL nRow, nCol;
	int nLeftInsertWidth, nRightInsertWidth;
	VERIFY(GetCurrentCell(nRow, nCol));
	// 대상 범위가 지정되지 않은 경우
	// 현재 셀이 있는 위치를 기준으로 한다.
	if (range.GetHeight() == 1 && range.GetWidth() == 1)
	{
		// 대상 범위가 Append Row를 넘어서면 수정과 추가가 중복되므로 에러
		if (nRow != GetRowCount() && (nRow + r.GetHeight()) > GetRowCount())
		{
			//sErrMsg = _T("수정과 추가가 혼합되게 됩니다. 위치를 다시 지정하세요.");
			sErrMsg = _LS(IDS_TB_PASTE_ADD_MODIFY_MIX);
			goto END_OF_PASTE;
		}

		// 대상 범위가 Column 한계를 넘어서면 에러
		if (nCol + r.GetWidth() - 1 > GetColCount())
		{
			//sErrMsg = _T("붙여 넣을 범위가 칼럼 범위를 초과합니다. 위치를 다시 지정하세요.");
			sErrMsg = _LS(IDS_TB_PASTE_EXCEED_COLUMN);
			goto END_OF_PASTE;
		}

		// 편집 모드이고 원본 범위가 2줄 이상이면 에러
		if (IsEdit() && r.GetHeight() > 1)
		{
			//sErrMsg = _T("현재 편집중인 레코드가 복사 범위에 포함되어 있습니다.");
			sErrMsg = _LS(IDS_TB_PASTE_EDIT_DATA_INCLUDED);
			goto END_OF_PASTE;
		}

		// 현재 셀이 Append Row에 있으면 모두 추가(DB에 추가된다.)
		if (nRow == GetRowCount())
			nPasteMode = TB_PASTE_MODE_APPEND;
		// 레코드 수정상태를 유지(DB를 수정하지 않는다.)
		else if (IsEdit()) nPasteMode = TB_PASTE_MODE_ONMODIFY;
		// 모두 수정(DB에서 수정이 된다.)
		else nPasteMode = TB_PASTE_MODE_MODIFY;
	}
	// 대상 범위가 지정된 경우 : 원본과 같은 범위, Fill Mode(원본 1줄, 동일 칸수)
	else
	{
		// 편집 모드에서 편집 레코드가 대상 범위에 포함되어 있고 2줄 이상이면 에러 표시
		if (IsEdit() && nRow >= range.top && nRow <= range.bottom && range.GetHeight() > 1)
		{
			//sErrMsg = _T("현재 편집중인 레코드가 복사 범위에 포함되어 있습니다.");
			sErrMsg = _LS(IDS_TB_PASTE_EDIT_DATA_INCLUDED);
			goto END_OF_PASTE;
		}
		
		// Append Row가 범위에 포함된 경우 다른 Row도 범위에 포함되어 있으면 에러
		if (range.bottom == GetRowCount() && range.GetHeight() > 1)
		{
			//sErrMsg = _T("수정과 추가가 혼합되게 됩니다. 위치를 다시 지정하세요.");
			sErrMsg = _LS(IDS_TB_PASTE_ADD_MODIFY_MIX);
			goto END_OF_PASTE;
		}

		// Fill 조건을 검사한다.
		// 원본이 1줄 이상이면 에러, 칸수가 달라도 에러가 된다.
		BOOL bFillMode = FALSE;
		if (range.GetHeight() != r.GetHeight() || range.GetWidth() != r.GetWidth())
		{
			// Fill Mode가 안 되는 조건
			if (r.GetHeight() > 1 || range.GetWidth() != r.GetWidth())
			{
				//sErrMsg = _T("대상 범위를 지정한 경우는 원본 범위의 크기와 같아야 합니다.");
				sErrMsg = _LS(IDS_TB_PASTE_RANGE_MISMATCH);
				goto END_OF_PASTE;
			}
			// 대상 범위가 1줄 이상이면 Fill Mode, 아니면 Paste와 동일하다.
			if (range.GetHeight() > 1) bFillMode = TRUE;
		}

		// Append 단독 Row면 레코드 추가
		if (range.bottom == GetRowCount()) nPasteMode = TB_PASTE_MODE_APPEND; 
		// 레코드 수정 상태를 유지하며 DB에서 수정은 일어나지 않는다.
		else if (IsEdit() && range.top == nRow) nPasteMode = TB_PASTE_MODE_ONMODIFY;
		// 아니면 레코드 수정이 된다.
		else nPasteMode = TB_PASTE_MODE_MODIFY;

		// Fill Mode인지 검사한다.
		if (bFillMode)
		{
			// 대상 범위의 줄수 만큼 Row를 만들어 채운다.
			x.SetRowCount(range.GetHeight());
			CString value;
			for (int i = 0; i < r.GetWidth(); i++)  // Column Wise Copy...
			{
				value = x.GetValueRowCol(1, i+1);
				for (int j = 1; j < range.GetHeight(); j++)
					x.SetExpressionRowCol(j+1, i+1, value);
			}
			r.bottom = x.GetRowCount();
		}
	}

	// 레코드 또는 레코드 셋을 만든다.
	// 왼쪽과 오른쪽에 모자라는 칼럼수를 삽입한다.
	nPastedCols = r.GetWidth();
	nLeftInsertWidth = range.left-1;
	if (nLeftInsertWidth > 0) x.InsertCols(1, nLeftInsertWidth);
	nRightInsertWidth = GetColCount()-x.GetColCount();
	if (nRightInsertWidth > 0) x.InsertCols(x.GetColCount()+1, nRightInsertWidth);
	// 모자라는 값을 카피한다. (Append는 Default값, Modify는 기존 값)
	if (nPasteMode == TB_PASTE_MODE_APPEND)
	{
		int nID;
		CString value;
		// 속도 향상을 위해 column별로 복사
		// 왼쪽 칼럼 카피 
		for (int j = 1; j <= nLeftInsertWidth; j++)
		{
			nID = j-1;
			if (nID == nKeyCol) value = _T("");
			else value = aDefVal[nID-1];
			for (int i = 1; i <= r.GetHeight(); i++)
				x.SetExpressionRowCol(i, j, value);
		}
		// Copy된 영역중 고의로 숨겨진 칼럼에 Default값 채우기
		for (int j = 0; j < aHideCol.GetSize(); j++)
		{
			nID = aHideCol[j]-1;
			for (int i = 1; i <= r.GetHeight(); i++)
				x.SetExpressionRowCol(i, aHideCol[j], aDefVal[nID-1]);
		}
		// 오른쪽 칼럼 카피
		for (int j = nLeftInsertWidth+nPastedCols+1; j <= GetColCount(); j++)
		{
			nID = j-1;
			if (nID == nKeyCol) value = _T("");
			else value = aDefVal[nID-1];
			for (int i = 1; i <= r.GetHeight(); i++)
				x.SetExpressionRowCol(i, j, value);
		}
	}
	else if (nPasteMode == TB_PASTE_MODE_ONMODIFY || nPasteMode == TB_PASTE_MODE_MODIFY)
	{
		CString value;
		// 왼쪽 칼럼 카피 
		for (int j = 1; j <= nLeftInsertWidth; j++)
		{
			for (int i = 1; i <= r.GetHeight(); i++)
			{
				value = GetValueRowCol(range.top-1+i, j);
				x.SetExpressionRowCol(i, j, value);
			}
		}
		// Copy된 영역중 고의로 숨겨진 칼럼에 기존의 값 채우기
		for (int j = 0; j < aHideCol.GetSize(); j++)
		{
			for (int i = 1; i <= r.GetHeight(); i++)
			{
				value = GetValueRowCol(range.top-1+i, aHideCol[j]);
				x.SetExpressionRowCol(i, aHideCol[j], value);
			}
		}
		// 오른쪽 칼럼 카피
		for (int j = nLeftInsertWidth+nPastedCols+1; j <= GetColCount(); j++)
		{
			for (int i = 1; i <= r.GetHeight(); i++)
			{
				value = GetValueRowCol(range.top-1+i, j);
				x.SetExpressionRowCol(i, j, value);
			}
		}
	}

END_OF_PASTE:
	if (!sErrMsg.IsEmpty())
	{
		SetWarningText(sErrMsg);
		return FALSE;
	}
	return TRUE;
}

BOOL CDgnBeamGridWndRusCIVIL::OnValidateField(ROWCOL nRow, ROWCOL nCol)
{
	CString value;
	//int nColID;
	//VERIFY(GetColumnIDFor(nCol, nColID));
	value = GetValueRowCol(nRow, nCol);

	if (!ValidateField(value, nCol)) return SetFieldValidation(FALSE);
	return SetFieldValidation(TRUE);
}

BOOL CDgnBeamGridWndRusCIVIL::ValidateField(CString& value, int nColID)
{
	int nErrCode=0;
	if (nColID == 1) 
		IsValidLongNumber(value, nErrCode);
	else if (nColID == 4)
		IsValidDoubleNumber(value, nErrCode);
	else
		return TRUE;

	switch(nErrCode)
	{
		case 1: SetWarningText(_LS(IDS_WG_DGN_INVALID_FORMULA)); break;
		case 2: SetWarningText(_LS(IDS_WG_DGN_CHARACTER_EXIST)); break;
		case 3: SetWarningText(_LS(IDS_WG_DGN_BIG_VALUE)); break;
		case 4: SetWarningText(_LS(IDS_WG_CMD__ADDD__Integer_Is_Required)); break;
	}
	if (nErrCode != 0)
		return FALSE;

	return TRUE;
}

/*
BOOL CDgnBeamGridWndRusCIVIL::ValidateRecord(CStringArray &aValue, CRowColArray &aCols)
{
}
*/


void CDgnBeamGridWndRusCIVIL::SetEnableByMod(ROWCOL nRow, ROWCOL nCol)
{
	ROWCOL nRowTemp, nColTemp;
	BOOL bCanGetCurCell;
	if(GetCurrentCell(nRowTemp, nColTemp))
	{
		bCanGetCurCell = TRUE;
	}
	else
	{
		bCanGetCurCell = FALSE;
	}
	
	if(nCol == 5)
	{
		CString strApp;
		
		strApp = GetValueRowCol(nRow, 5);
		
		SetStyleRange(CGXRange(nRow, 6), CGXStyle().SetEnabled(strApp == _T("1")));
		SetStyleRange(CGXRange(nRow, 7), CGXStyle().SetEnabled(strApp == _T("1")));
	}
}

BOOL CDgnBeamGridWndRusCIVIL::OnLButtonHitRowCol(ROWCOL nHitRow, ROWCOL nHitCol, ROWCOL nDragRow, ROWCOL nDragCol, CPoint point, UINT flags, WORD nHitState)
{
	if(!CTBBrowserWnd::OnLButtonHitRowCol(nHitRow, nHitCol, nDragRow, nDragCol, point, flags, nHitState))
		return FALSE;
	
	SetEnableByMod(nHitRow, nHitCol);
	
	return TRUE;
}