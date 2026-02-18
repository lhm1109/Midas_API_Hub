// DgnColumnGridWndCIVIL.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnColumnGridWndCIVIL.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_DBLib.h"
#include "..\wg_db\wg_db_AttrCtrl.h"

#include "..\wg_common\wg_common_TBGrid.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDgnColumnGridWndCIVIL


CDgnColumnGridWndCIVIL::CDgnColumnGridWndCIVIL()
{

}

CDgnColumnGridWndCIVIL::~CDgnColumnGridWndCIVIL()
{
}

#define CWnd  CGXGridWnd
BEGIN_MESSAGE_MAP(CDgnColumnGridWndCIVIL, CWnd)
#undef CWnd

	//{{AFX_MSG_MAP(CDgnColumnGridWndCIVIL)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CDgnColumnGridWndCIVIL message handlers

void CDgnColumnGridWndCIVIL::SetLayerNum(UINT RchkKey, UINT nLayer)
{
	HideRows(0, nLayer+1, FALSE);

	//(2010.02.09) Modify by Unsang :: Layer가 5개인 경우, Grid를 신규 생성 시 Position별 Colm을 제어하지 못함.
	//if(nLayer<0 || nLayer>4) return;
	if(nLayer<1 || nLayer>5) return;

	HideRows(nLayer+2, 7, TRUE);
	
	for(int i=nLayer+2; i<=7; i++)
	{
		SetValueRange(CGXRange(i, 1), double(0));
		SetValueRange(CGXRange(i, 2), _T(""));
		SetValueRange(CGXRange(i, 3), _T(""));
		SetValueRange(CGXRange(i, 4), double(0));
		SetValueRange(CGXRange(i, 5), _T(""));
		SetValueRange(CGXRange(i, 6), _T(""));
		SetValueRange(CGXRange(i, 7), double(0));
		SetValueRange(CGXRange(i, 8), _T(""));
		SetValueRange(CGXRange(i, 9), _T(""));
		SetValueRange(CGXRange(i, 10), double(0));
	}

	T_SECT_D SectData;  SectData.Initialize();
	CDBDoc* m_pDoc = CDBDoc::GetDocPoint();
	if(!m_pDoc->m_pAttrCtrl->GetSectDesign(RchkKey, SectData))	return;

	if(SectData.SectBefore.Shape == D_SECT_SHAPE_REG_SB  ||
		 SectData.SectBefore.Shape == D_SECT_SHAPE_REG_B   ||
		 SectData.SectBefore.Shape == D_SECT_SHAPE_REG_TRK ||
		 SectData.SectBefore.Shape == D_SECT_SHAPE_REG_STRK||
		 SectData.SectBefore.Shape == D_SECT_SHAPE_REG_H     )
	{
		// Change by ZINU.('02.10.31).
		HideCols(7,9);
		/*
		SetStyleRange(CGXRange().SetCols(7,9), CGXStyle()
	    .SetControl(GX_IDS_CTRL_STATIC)    
			.SetInterior(RGB(192,192,192)));
		*/
	}

	else if(SectData.SectBefore.Shape == D_SECT_SHAPE_REG_P ||
					SectData.SectBefore.Shape == D_SECT_SHAPE_REG_SR)
	{
		// Change by ZINU.('02.10.31).
		HideCols(4,9);
		/*
		SetStyleRange(CGXRange().SetCols(4,9), CGXStyle()
	    .SetControl(GX_IDS_CTRL_STATIC)    
			.SetInterior(RGB(192,192,192)));
		*/
	}
}

void CDgnColumnGridWndCIVIL::SetHeaderTitle()
{  
	CString title = _T("");

	// Column Title
	title = _LS(IDS_DGN_GRID_pos1);
	SetCoveredCellsRowCol(0,1,0,3);
	SetStyleRange(CGXRange(0,1), CGXStyle()
		.SetValue(title));

	title = _LS(IDS_DGN_GRID_pos2);
	SetCoveredCellsRowCol(0,4,0,6);
	SetStyleRange(CGXRange(0,4), CGXStyle()
		.SetValue(title));

	title = _LS(IDS_DGN_GRID_pos3);
	SetCoveredCellsRowCol(0,7,0,9);
	SetStyleRange(CGXRange(0,7), CGXStyle()
		.SetValue(title));

	title = _LS(IDS_DGN_CON_BEAM_Layer);
	SetCoveredCellsRowCol(0,0,1,0);
	SetStyleRange(CGXRange(0,0), CGXStyle()
		.SetValue(title));

	title = _LS(IDS_DGN_CON_BEAM_Dc);
	SetCoveredCellsRowCol(0,10,1,10);
	SetStyleRange(CGXRange(0,10), CGXStyle()
		.SetValue(title));
	CString strColTitle[] = {_LS(IDS_DGN_CON_COLM_Num), _LS(IDS_DGN_CON_BEAM_Size1), _LS(IDS_DGN_CON_BEAM_Size2), _LS(IDS_DGN_CON_COLM_Num), _LS(IDS_DGN_CON_BEAM_Size1), _LS(IDS_DGN_CON_BEAM_Size2) , _LS(IDS_DGN_CON_COLM_Num), _LS(IDS_DGN_CON_BEAM_Size1),  _LS(IDS_DGN_CON_BEAM_Size2)};
	
	for(int i=0; i<9; i++) SetStyleRange(CGXRange(1, i+1), CGXStyle().SetValue(strColTitle[i]));

	// Row Title
	for(int i=0; i<5; i++)
	{
		title.Format(_T("%d"), i+1);
		SetStyleRange(CGXRange(i+2, 0), CGXStyle().SetValue(title));
	} 
}

void CDgnColumnGridWndCIVIL::InitSizeComBoBox()
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
	for(int i=0; i<5; i++)
	{
	  SetStyleRange(CGXRange(i+2,2), CGXStyle().SetChoiceList(szNameList1));		
	  SetStyleRange(CGXRange(i+2,3), CGXStyle().SetChoiceList(szNameList2));		
	  SetStyleRange(CGXRange(i+2,5), CGXStyle().SetChoiceList(szNameList1));		
	  SetStyleRange(CGXRange(i+2,6), CGXStyle().SetChoiceList(szNameList2));		
	  SetStyleRange(CGXRange(i+2,8), CGXStyle().SetChoiceList(szNameList1));		
	  SetStyleRange(CGXRange(i+2,9), CGXStyle().SetChoiceList(szNameList2));		
	}
}

void CDgnColumnGridWndCIVIL::Initialize()
{
	CGXGridWnd::Initialize();
	
	// cell 바깥영역 배경색 지정 
	GetParam()->GetProperties()->SetColor(GX_COLOR_BACKGROUND, CTBGrid::m_Color.OuterBackground);
	
	// Header 배경색 지정 
	ChangeColHeaderStyle(CGXStyle().SetInterior(CTBGrid::m_Color.RowColHeaderBg)); 
	ChangeRowHeaderStyle(CGXStyle().SetInterior(CTBGrid::m_Color.RowColHeaderBg)); 
	
	// 기타 Cells 배경색 지정
	ChangeStandardStyle(CGXStyle( ).SetInterior(CTBGrid::m_Color.NormalModeBg));

	LockUpdate(TRUE);

	GetParam()->EnableUndo(FALSE);                                // Undo / Redo 금지
	GetParam()->EnableMoveRows(FALSE);                            // Row 이동 금지
	GetParam()->EnableMoveCols(FALSE);                            // Col 이동 금지
	//GetParam()->EnableSelection(GX_SELROW);                       // Row Selection
	GetParam()->EnableTrackRowHeight(GX_TRACK_NOTHEADER);         // Row Size 변경금지
	GetParam()->EnableTrackColWidth(GX_TRACK_NOTHEADER);          // Col Size 변경금지
	GetParam()->SetActivateCellFlags(GX_CAFOCUS_DBLCLICKONCELL);
	SetDrawingTechnique(gxDrawUsingMemDC);                        // MemDC 사용

	ColHeaderStyle().SetEnabled(FALSE);
	RowHeaderStyle().SetEnabled(FALSE);

	CGXProperties* pProp = GetParam()->GetProperties();
	pProp->SetUserProperty(GX_IDS_OUTLINECURRENTCELL, (CGXStyle)pProp->sInvertNoBorder);

	ChangeStandardStyle(CGXStyle()
		.SetFont(CGXFont_GC().SetSize(9))
		.SetVerticalAlignment(DT_BOTTOM)
		.SetAllowEnter(FALSE));

	// Grid Num
	SetColCount(10);        // Header를 제외한 갯수
	SetRowCount(6);         // Header 때문에 (Header가 두줄)

	// Grid Size
	SetRowHeightByDPI(0, 1, 20);
	SetRowHeightByDPI(2, 6, 18);
	SetColWidthByDPI(0, 0, 45);
	SetColWidthByDPI(1, 1, 45);
	SetColWidthByDPI(2, 2, 45);
	SetColWidthByDPI(3, 3, 45);
	SetColWidthByDPI(4, 4, 45);
	SetColWidthByDPI(5, 5, 45);
	SetColWidthByDPI(6, 6, 45);
	SetColWidthByDPI(7, 7, 45);
	SetColWidthByDPI(8, 8, 45);
	SetColWidthByDPI(9, 9, 45);
	SetColWidthByDPI(10, 10, 45);

	// Set Title
	SetHeaderTitle();

	SetStyleRange(CGXRange().SetCols(0), CGXStyle()
		.SetHorizontalAlignment(DT_CENTER)
		.SetValueType(GX_VT_STRING)
		);

	for(int i=1; i<=9; i=i+3) 
		SetStyleRange(CGXRange().SetCols(i), CGXStyle()
		.SetHorizontalAlignment(DT_RIGHT)
		//.SetControl(GX_IDS_CTRL_SPINEDIT)    
		.SetValueType(GX_VT_NUMERIC)
		//.SetInterior(RGB(217,248,216))
		);
	
	for(int i=2; i<=9; i=i+3)
	{
		SetStyleRange(CGXRange().SetCols(i), CGXStyle()
	    .SetControl(GX_IDS_CTRL_TABBED_COMBOBOX)    
			.SetValueType(GX_VT_STRING)
			.SetHorizontalAlignment(DT_LEFT)
	    .SetVertScrollBar(FALSE)
			.SetUserAttribute(GX_IDS_UA_TABLIST_KEYCOL     , _T("0"))
			.SetUserAttribute(GX_IDS_UA_TABLIST_TEXTCOL    , _T("0"))
			.SetUserAttribute(GX_IDS_UA_TABLIST_SHOWALLCOLS, _T("1"))
			//.SetInterior(RGB(217,248,216))
			);

		SetStyleRange(CGXRange().SetCols(i+1), CGXStyle()
	    .SetControl(GX_IDS_CTRL_TABBED_COMBOBOX)    
			.SetValueType(GX_VT_STRING)
			.SetHorizontalAlignment(DT_LEFT)
	    .SetVertScrollBar(FALSE)
			.SetUserAttribute(GX_IDS_UA_TABLIST_KEYCOL     , _T("0"))
			.SetUserAttribute(GX_IDS_UA_TABLIST_TEXTCOL    , _T("0"))
			.SetUserAttribute(GX_IDS_UA_TABLIST_SHOWALLCOLS, _T("1"))
			//.SetInterior(RGB(217,248,216))
			);
	}

	SetStyleRange(CGXRange().SetCols(10), CGXStyle()
	  .SetControl(GX_IDS_CTRL_EDIT)    
		.SetValueType(GX_VT_NUMERIC)
		.SetHorizontalAlignment(DT_RIGHT)
		//.SetPlaces(2)
		//.SetInterior(RGB(217,248,216))
		);

	SetStyleRange(CGXRange().SetRows(1), CGXStyle()
		.SetHorizontalAlignment(DT_CENTER)
		.SetValueType(GX_VT_STRING)
		.SetDraw3dFrame(gxFrameRaised)
		.SetEnabled(FALSE)
		.SetIncludeEnabled(FALSE)
		);
	SetFrozenRows(1, 1);

	InitSizeComBoBox();
	 
	LockUpdate(FALSE);
	Redraw();

	SetCurrentCell(2,1);
}


void CDgnColumnGridWndCIVIL::OnModifyCell(ROWCOL nRow, ROWCOL nCol)
{/*
	if(nCol != 1) return;

	// 여기서 View를 Update 시켜준다.
	CString str;  
	str = GetValueRowCol(i+1, 1);
	int nCount = _ttoi(str);
*/
/*
	if(m_BeamShape ==_T("SB"))
	{
		m_pViewWnd->Beam_DrawData(eDGN_RECT,m_Hc,m_Bc,m_Hf,m_Bf,m_TOneArray,m_BOneArray,
														m_Nb,m_Nb2,m_Nt,m_Nt2,m_DtNo,m_DbNo);
	}
	else if(m_BeamShape ==_T("T"))
	{
		m_pViewWnd->Beam_DrawData(eDGN_TEE,m_Hc,m_Bc,m_Hf,m_Bf,m_TOneArray,m_BOneArray,
														m_Nb,m_Nb2,m_Nt,m_Nt2,m_DtNo,m_DbNo);
	}
	m_pViewWnd->Invalidate(TRUE);
*/

	CGXGridWnd::OnModifyCell(nRow, nCol);
}


BOOL CDgnColumnGridWndCIVIL::OnPasteFromClipboard(const CGXRange &range)
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

	if (nPastedCols > 10)  
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
	return TRUE;   

PASTE_ERROR:
	SetWarningText(csErrMsg);
	DisplayWarningText();
	x.DestroyWindow();
	return FALSE;
}

// Delete 를 막아줌 
BOOL CDgnColumnGridWndCIVIL::ProcessKeys(CWnd *pSender, UINT nMessage, UINT nChar, UINT nRepCnt, UINT flags)
{
	if((nMessage == WM_KEYDOWN) && (nChar == VK_DELETE)) return TRUE;
	 
	return CGXGridWnd::ProcessKeys(pSender, nMessage, nChar, nRepCnt, flags); 
}

BOOL CDgnColumnGridWndCIVIL::GetStyleRowCol(ROWCOL nRow, ROWCOL nCol, CGXStyle& style, GXModifyType mt, int nType)
{
	BOOL bReturn;
	bReturn = CTBBrowserWnd::GetStyleRowCol(nRow, nCol, style, mt, nType);

	if (nRow == 1) style
		.SetInterior(m_Color.RowColHeaderBg)
		.SetTextColor(m_Color.NormalModeFg);

	return bReturn;
}

BOOL CDgnColumnGridWndCIVIL::OnValidateField(ROWCOL nRow, ROWCOL nCol)
{
	CString value;
	//int nColID;
	//VERIFY(GetColumnIDFor(nCol, nColID));
	value = GetValueRowCol(nRow, nCol);

	if (!ValidateField(value, nCol)) return SetFieldValidation(FALSE);
	return SetFieldValidation(TRUE);
}

BOOL CDgnColumnGridWndCIVIL::ValidateField(CString& value, int nColID)
{
	int nErrCode=0;
	if (nColID == 1 || nColID == 4) 
		IsValidLongNumber(value, nErrCode);
	else if (nColID == 7)
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