// DgnSeisAnchorTensionGrid.cpp : implementation file


#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnSeisAnchorTensionGrid.h"

#include "..\wg_base\wg_base_StrParser.h"
#include "..\wg_common\wg_common_TBCommon.h"
#include "..\wg_common\wg_common_TBGrid.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_AttrCtrl2.h"

CDgnSeisAnchorTensionGrid::CDgnSeisAnchorTensionGrid()
{
	m_pDoc = CDBDoc::GetDocPoint();
}

CDgnSeisAnchorTensionGrid::~CDgnSeisAnchorTensionGrid()
{
}

/////////////////////////////////////////////////////////////////////////////
// User Defined Finction
void CDgnSeisAnchorTensionGrid::SetData2Grid(T_ANEV_D& Data)
{
	BOOL bOldLock = LockUpdate(TRUE);

	int nRow = Data.TensChk.aElnkNodeKs.GetSize();
	int nCol = 0;

	if(nRow > 0) 
	{
		nCol = Data.TensChk.aElnkNodeKs[0].aNodeK1.GetSize();
	}

	CString strPropertyList = _T("");
	CString item;
	CSize nTempLen;
	int nNameLen = 0;
	CDC* pDC = GetDC();
	ASSERT(pDC);

	T_NODE_K NodeK1;
	T_NODE_K NodeK2;
	for(int i=0; i< nRow; i++)
	{
		for (int j = 0; j < nCol; j++)
		{
			NodeK1 = Data.TensChk.aElnkNodeKs[i].aNodeK1[j];
			NodeK2 = Data.TensChk.aElnkNodeKs[i].aNodeK2[j];
			item.Format(_T("Node(%d,%d)\n"), NodeK1, NodeK2);

			strPropertyList += item;
			nTempLen = pDC->GetTextExtent(item);
			if (nTempLen.cx > nNameLen) nNameLen = nTempLen.cx;
		}
	}

	SetColCount(nCol);
	SetRowCount(nRow);

	SetColWidth(0, 0, globalUtils.ScaleByDPI(50));
	SetColWidth(1, nCol, globalUtils.ScaleByDPI(90));

	SetRowHeight(0, 0, globalUtils.ScaleByDPI(20));
	SetRowHeight(1, nRow, globalUtils.ScaleByDPI(20));
	
	if (nRow > 0)
	{
		CString csSize;
		csSize.Format(_T("0,%d"), nNameLen);
		SetStyleRange(CGXRange().SetCols(1, nCol), CGXStyle()
			.SetControl(GX_IDS_CTRL_CBS_DROPDOWNLIST)  // GX_IDS_CTRL_TABBED_COMBOBOX
			.SetChoiceList(strPropertyList)
			.SetUserAttribute(GX_IDS_UA_TABLIST_KEYCOL, _T("0"))
			.SetUserAttribute(GX_IDS_UA_TABLIST_TEXTCOL, _T("0"))
			.SetUserAttribute(GX_IDS_UA_TABLIST_SHOWALLCOLS, _T("1"))
			.SetUserAttribute(GX_IDS_UA_TABLIST_COLWIDTHS, csSize)
			.SetValueType(GX_VT_STRING)
			.SetVerticalAlignment(DT_VCENTER)
			.SetHorizontalAlignment(DT_CENTER)
			.SetValue(_T("")));
	}

	CString strColName;
	for(int i=0; i<nCol; i++)
	{
		strColName.Format(_T("%d"), i+1);
		SetValueRange(CGXRange(0, i+1), strColName), CGXStyle()
			.SetVerticalAlignment(DT_VCENTER)
			.SetHorizontalAlignment(DT_CENTER);
	}

	CString strRowName;
	for(int i=0; i<nRow; i++)
	{
		strRowName.Format(_T("%s%d"), _LS(IDS_DGN_SEIS_ANCHOR_SUPPPOT_LAYER), i+1);
		SetValueRange(CGXRange(i+1, 0), strRowName), CGXStyle()
			.SetVerticalAlignment(DT_VCENTER)
			.SetHorizontalAlignment(DT_CENTER);
	}

	for(int i=0; i<nRow; i++)
	{
		CStringArray aValue;
		aValue.SetSize(nCol);

		for(int j=0; j<nCol; j++)
		{

			T_NODE_K NodeK1;
			T_NODE_K NodeK2;
			NodeK1 = Data.TensChk.aElnkNodeKs[i].aNodeK1[j];
			NodeK2 = Data.TensChk.aElnkNodeKs[i].aNodeK2[j];
			item.Format(_T("Node(%d,%d)"), NodeK1, NodeK2);

			aValue[j] = item;
		}
		
		SetDataToRow(i+1, aValue);
	}

	LockUpdate(bOldLock);
	if(!bOldLock) Redraw();
}

BOOL CDgnSeisAnchorTensionGrid::GetGrid2Data(T_ANEV_D& Data)
{
	int nRow = GetRowCount();
	int nCol = GetColCount();
	if(nRow == 0 || nCol == 0) return TRUE;

	CStringArray aValue;
	aValue.SetSize(nCol);

	T_ANEV_ELNK_NODE_D ElnkNodeKs; ElnkNodeKs.Initialize();
	ElnkNodeKs.aNodeK1.SetSize(nCol);
	ElnkNodeKs.aNodeK2.SetSize(nCol);

	CArray<int, int> aNode;
	for(int i=0; i<nRow; i++)
	{
		if(!GetRowToData(i+1, aValue)) return FALSE;

 		for(int j=0; j<nCol; j++)
 		{
			CString strVal = aValue[j];
			int iNo1 = strVal.Replace(_T("Node("), _T(""));
			int iNo2 = strVal.Replace(_T(","), _T(" "));
			int iNo3 = strVal.Replace(_T(")"), _T(""));

			CStrParser::GetNodeList(strVal, aNode);
			ElnkNodeKs.aNodeK1[j] = aNode[0];
			ElnkNodeKs.aNodeK2[j] = aNode[1];
 		}
 		
		Data.TensChk.aElnkNodeKs.Add(ElnkNodeKs);
	}

	return TRUE;
}

void CDgnSeisAnchorTensionGrid::SetDataToRow(ROWCOL nRow, CStringArray& raValue)
{
	int nColCount = GetColCount();
	ASSERT(nColCount == raValue.GetSize());

	for(int i=1; i<=nColCount; ++i)
		SetValueRange(CGXRange(nRow, i), raValue[i-1]);
}

BOOL CDgnSeisAnchorTensionGrid::GetRowToData(ROWCOL nRow, CStringArray& raValue)
{
	int nColCount = GetColCount();
	ASSERT(nColCount == raValue.GetSize());

	for(int i=1; i<=nColCount; ++i)
		raValue[i-1] = GetEditingValue(nRow, i);

	return TRUE;
}

CString CDgnSeisAnchorTensionGrid::GetEditingValue(ROWCOL nRow, ROWCOL nCol)
{
	CString strNum(_T(""));
	BOOL bGet = FALSE;
	ROWCOL ncRow, ncCol;
	if(GetCurrentCell(ncRow, ncCol))
	{
		if(ncRow==nRow && ncCol==nCol)
		{
			CGXControl* pControl = GetControl(nRow, nCol);
			if(pControl && pControl->IsActive())
				bGet = pControl->GetValue(strNum);
		}
	}
	if(!bGet) strNum = GetValueRowCol(nRow, nCol);

	return strNum;
}

#define CWnd CGXGridWnd
BEGIN_MESSAGE_MAP(CDgnSeisAnchorTensionGrid, CWnd)
#undef CWnd
	//{{AFX_MSG_MAP(CDgnSeisAnchorTensionGrid)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

///////////////////////////////////////////////////////////////////////////
// CDgnSeisAnchorTensionGrid message handlers

// 그리드 초기화 
void CDgnSeisAnchorTensionGrid::Initialize()
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

	SetScrollBarMode(SB_VERT, gxnEnabled | gxnEnhanced);
	SetScrollBarMode(SB_HORZ, gxnEnabled | gxnEnhanced);
	
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
	
	LockUpdate(FALSE);
}

BOOL CDgnSeisAnchorTensionGrid::GetStyleRowCol(ROWCOL nRow, ROWCOL nCol, CGXStyle& style, GXModifyType mt, int nType)
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
			.SetInterior(COLORREF(RGB(187, 227, 255)))
			.SetTextColor(COLORREF(RGB(0, 0, 0)));

		return TRUE;
	}

	if (bCurrentCell) style
		.SetInterior(COLORREF(RGB( 42,  64, 128)))
		.SetTextColor(COLORREF(RGB(255, 255, 255)));
	else if (bCurrentRow) style
		.SetInterior(COLORREF(RGB(187, 227, 255)))
		.SetTextColor(COLORREF(RGB(0, 0, 0)));
	else if (bHeaderCol || bHeaderRow) style    // Enabled Header Col(Only Frozen)
		.SetInterior(COLORREF(RGB(187, 227, 255)))
		.SetTextColor(COLORREF(RGB(0, 0, 0)));

	return TRUE;
}

void CDgnSeisAnchorTensionGrid::OnModifyCell(ROWCOL nRow, ROWCOL nCol)
{
	CGXGridWnd::OnModifyCell(nRow, nCol);
}

BOOL CDgnSeisAnchorTensionGrid::OnPasteFromClipboard(const CGXRange &range)
{
	// 	if (IsLockEdit()) return FALSE; // display only mode이면 바로 리턴
	// 	return CTBCommon::OnPasteFromClipboard(range);
	//	return CGXGridWnd::OnPasteFromClipboard(range);

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

	// 범위에 고의로 숨겨진 Column(HideByIntend)이 포함되어 있으면 이 칼럼을
	// 삽입하고 Default Value로 채운다.
	CArray<int, int> aHideCol, aHideRow;
	int nColCount = GetColCount();
	int nRowCount = GetRowCount();
	int nPasteColCount = r.GetWidth();
	int nPasteRowCount = r.GetHeight();
	for (int k = 1; k < nPasteColCount; k++)
	{
		if (range.left + k < nColCount && IsColHidden(range.left + k))
		{
			x.InsertCols(k + 1, 1);
			aHideCol.Add(range.left + k);
			r.right++;
			nPasteColCount++;
		}
	}
	for (int k = 1; k < nPasteRowCount; k++)
	{
		if (range.top + k < nRowCount && IsRowHidden(range.top + k))
		{
			x.InsertRows(k + 1, 1);
			x.SetExpressionRowCol(k + 1, 1, _T("100"));
			aHideRow.Add(range.top + k);
			r.bottom++;
			nPasteRowCount++;
		}
	}

	CString sErrMsg = _T("");
	ROWCOL nRow, nCol;
	VERIFY(GetCurrentCell(nRow, nCol));
	// 대상 범위가 지정되지 않은 경우
	// 현재 셀이 있는 위치를 기준으로 한다.
	if (range.GetHeight() == 1 && range.GetWidth() == 1)
	{
		// 대상 범위가 Column 한계를 넘어서면 에러
		if (nCol + r.GetWidth() - 1 > GetColCount())
		{
			//sErrMsg = _T("붙여 넣을 범위가 칼럼 범위를 초과합니다. 위치를 다시 지정하세요.");
			sErrMsg = _LS(IDS_CMD_PASTE_EXCEED_COLUMN);
			goto END_OF_PASTE;
		}
		// 대상 범위가 Row 한계를 넘어서면 에러
		if (nRow + r.GetHeight() - 1 > GetRowCount())
		{
			//sErrMsg = _T("붙여 넣을 범위가 로우 범위를 초과합니다. 위치를 다시 지정하세요.");
			sErrMsg = _LS(IDS_CMD_PASTE_EXCEED_ROW);
			goto END_OF_PASTE;
		}

	}
	// 대상 범위가 지정된 경우 : 원본과 같은 범위, Fill Mode(원본 1줄, 동일 칸수)
	else
	{
		// Fill 조건을 검사한다.
		// 원본이 1줄 이상이면 에러, 칸수가 달라도 에러가 된다.
		BOOL bFillMode = FALSE;
		if (range.GetHeight() != r.GetHeight() || range.GetWidth() != r.GetWidth())
		{
			// Fill Mode가 안 되는 조건
			if (r.GetHeight() > 1 || range.GetWidth() != r.GetWidth())
			{
				//sErrMsg = _T("대상 범위를 지정한 경우는 원본 범위의 크기와 같아야 합니다.");
				sErrMsg = _LS(IDS_CMD_PASTE_RANGE_MISMATCH);
				goto END_OF_PASTE;
			}
			// 대상 범위가 1줄 이상이면 Fill Mode, 아니면 Paste와 동일하다.
			if (range.GetHeight() > 1) bFillMode = TRUE;
		}

		// Fill Mode인지 검사한다.
		if (bFillMode)
		{
			// 대상 범위의 줄수 만큼 Row를 만들어 채운다.
			x.SetRowCount(range.GetHeight());
			CString value;
			for (int i = 0; i < r.GetWidth(); i++)  // Column Wise Copy...
			{
				value = x.GetValueRowCol(1, i + 1);
				for (int j = 1; j < range.GetHeight(); j++)
					x.SetExpressionRowCol(j + 1, i + 1, value);
			}
			r.bottom = x.GetRowCount();
		}
	}

END_OF_PASTE:
	if (!sErrMsg.IsEmpty())
	{
		AfxMessageBox(sErrMsg);
		x.DestroyWindow();
		return FALSE;
	}

	x.DestroyWindow();
	return CGXGridWnd::OnPasteFromClipboard(range);
}

BOOL CDgnSeisAnchorTensionGrid::ProcessKeys(CWnd *pSender, UINT nMessage, UINT nChar, UINT nRepCnt, UINT flags)
{
	CRowColArray awRows;
	ROWCOL nRow=1, nCol=1;

	if(!GetCurrentCell(nRow, nCol)) // if no current cell, default action
		return CGXGridWnd::ProcessKeys(pSender, nMessage, nChar, nRepCnt, flags);

	if(nMessage == WM_KEYDOWN)
	{
		switch(nChar)
		{
		case VK_TAB: 
			{
				BOOL bShift = GetKeyState(VK_SHIFT) & 0x8000;
				if( bShift )
				{
					if( SetLeftValidCell(nRow,  nCol)) { goto GT_PROC_RETURN; }
				}

				if(!bShift )
				{
					if( SetRightValidCell(nRow, nCol)) { goto GT_PROC_RETURN; }
				}

			}
			break;
		case VK_DELETE:
			{
				goto GT_PROC_RETURN;
			}
			break;
		default: 
			break;
		}
	}

	return CGXGridWnd::ProcessKeys(pSender, nMessage, nChar, nRepCnt, flags); 

GT_PROC_RETURN:
	if(nMessage==WM_KEYDOWN && (nChar==VK_TAB || nChar==VK_RETURN || nChar==VK_RIGHT || nChar==VK_LEFT))
	{
		Redraw();
	}

	return TRUE;
}

BOOL CDgnSeisAnchorTensionGrid::SetRightValidCell(ROWCOL nRow, ROWCOL nCol)
{

	long i=nRow, j=nCol+1; // 현재 행 다음 열
	int nColCount = GetColCount();
	if(j>nColCount) { ++i; j=1; }

	BOOL bCovered;/* = FALSE;*/
	while(i<=GetRowCount())
	{
		bCovered = FALSE;

		while(j<=nColCount)
		{
			// 열이 선택될 수 있는지 조사
			CGXStyle style; style.Free();
			ComposeStyleRowCol(i, j, &style);

			// Is it a covered cell?
			CGXRange cvr;
			if(!bCovered && GetCoveredCellsRowCol(i, j, cvr))
			{
				if((cvr.top<=i && i<=cvr.bottom) && (cvr.left<=j && j<=cvr.right))
				{
					if (nRow == cvr.top) i = cvr.bottom;
					j = cvr.right+1;
					bCovered = TRUE;
					continue;
				}
			}

			if(IsRowHidden(i)) break;
			if(IsColHidden(j)) { ++j; continue; }

			BOOL bEnable = (!style.GetIncludeEnabled() || style.GetEnabled());
			BOOL bReadOnly = style.GetReadOnly();

			if(bEnable && !bReadOnly)
			{
				if(SetCurrentCell(i, j)) return TRUE;
				return FALSE;
			}
			//bCovered = TRUE;
			bCovered = FALSE;

			++j;  // 다음 열을 조사
		}
		if(j>nColCount) { ++i; j=1; }
	}
	return FALSE;
}


BOOL CDgnSeisAnchorTensionGrid::SetLeftValidCell(ROWCOL nRow, ROWCOL nCol)
{
	long i=nRow, j=nCol-1; // 현재 행 이전 열
	int nColCount = GetColCount();
	if(nCol==0 || j<1) { --i; j=nColCount; } 

	BOOL bCovered = FALSE;
	while(i>=1)
	{
		bCovered = FALSE;
		while(j>=1)
		{
			// 열이 선택될 수 있는지 조사
			CGXStyle style; style.Free();
			ComposeStyleRowCol(i, j, &style);

			// Is it a covered cell?
			CGXRange cvr;
			if(!bCovered && GetCoveredCellsRowCol(i, j, cvr))
			{
				if((cvr.top<=i && i<=cvr.bottom) && (cvr.left<=j && j<=cvr.right))
				{
					i = cvr.bottom;
					j = cvr.left-1;
					bCovered = TRUE;
					continue;
				}
			}

			if(IsRowHidden(i)) break;
			if(IsColHidden(j)) { --j; continue; }

			BOOL bEnable = !style.GetIncludeEnabled() || style.GetEnabled();
			BOOL bReadOnly = style.GetReadOnly();

			if(bEnable && !bReadOnly)
			{
				if(GetCoveredCellsRowCol(i, j, cvr))
				{
					if((cvr.top<=i && i<=cvr.bottom) && (cvr.left<=j && j<=cvr.right))
					{
						j = cvr.left;
					}
				}
				if(SetCurrentCell(i, j)) return TRUE;
				return FALSE;
			}
			bCovered = TRUE;
			--j;  // 이전 열을 조사
		}
		if(nCol==0 || j<1) { --i; j=nColCount; } 
	}
	return FALSE;
}