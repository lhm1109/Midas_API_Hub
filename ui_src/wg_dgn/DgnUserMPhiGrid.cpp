// DgnUserMPhiGrid.cpp: implementation of the CDgnUserMPhiGrid class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnUserMPhiGrid.h"

	

#include "DgnUserMPhiDlg.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDgnUserMPhiGrid::CDgnUserMPhiGrid(CDgnUserMPhiDlg* pParent)
{
	m_pParent = pParent;
	m_nHeadRow   = 0;
	m_nReturnDir = 0; // 아래
}

CDgnUserMPhiGrid::~CDgnUserMPhiGrid()
{

}

void CDgnUserMPhiGrid::SetHeaderTitle()
{
	CString title = _T(""), strUnit = _T("");

	// Column Title
	title = _T("Curvature");
	if(CUnitCtrl::GetUnitSystem(D_UNITSYS_BASE_PERLENGTH, strUnit))
		title += (_T("\n") + strUnit);
	SetStyleRange(CGXRange(0,1), CGXStyle()
		.SetHorizontalAlignment(DT_CENTER)
	  .SetVerticalAlignment(DT_VCENTER)
		.SetFont(CGXFont_GC().SetBold(FALSE).SetSize(8))
		.SetValue(title)
		);

	title = _T("Moment");
	if(CUnitCtrl::GetUnitSystem(D_UNITSYS_BASE_MOMENT, strUnit))
		title += (_T("\n") + strUnit);
	SetStyleRange(CGXRange(0,2), CGXStyle()
		.SetHorizontalAlignment(DT_CENTER)
	  .SetVerticalAlignment(DT_VCENTER)
		.SetFont(CGXFont_GC().SetBold(FALSE).SetSize(8))
		.SetValue(title)
		);
}

BOOL CDgnUserMPhiGrid::OnPasteFromClipboard(const CGXRange &range)
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

	if (nPastedCols > 4)  
	{
		csErrMsg = _LS(IDS_TM_STDG_GRID_ERR_MSG5);
		goto PASTE_ERROR;
	}

	nTotalRow = GetRowCount()-1;
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

	//SaveGridData();
	return TRUE;   

PASTE_ERROR:
	SetWarningText(csErrMsg);
	DisplayWarningText();
	x.DestroyWindow();
	return FALSE;
}


void CDgnUserMPhiGrid::Initialize(CArray<T_RSDL_MPHI_D,T_RSDL_MPHI_D>* parPierUserMPhi)
{	
	m_parPierUserMPhi = parPierUserMPhi;
	CTBBrowserWnd::Initialize();

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

	SetRowHeightByDPI(0, 0, 30);
	SetColCount(2);
	SetColWidthByDPI(1, 1, 91);
	SetColWidthByDPI(2, 2, 91);
	
	SetStyleRange(CGXRange().SetRows(0), CGXStyle().SetWrapText(TRUE)); // Header만 2줄쓰기 가능	
	SetScrollBarMode(SB_VERT, gxnEnabled | gxnEnhanced);

	SetHeaderTitle();

	

	LockUpdate(FALSE);
}
void CDgnUserMPhiGrid::Data2Grid()
{
	int nSize = m_parPierUserMPhi->GetSize();
	SetRowSize(nSize);

	T_RSDL_MPHI_D MPhiD; 
	for(int i=0 ; i<nSize ; i++)
	{
		MPhiD = m_parPierUserMPhi->GetAt(i);
		SetValueRange(CGXRange(i+1, 1), MPhiD.dCurvature);
		SetValueRange(CGXRange(i+1, 2), MPhiD.dMoment);
	}
}
void CDgnUserMPhiGrid::Grid2Data()
{
	int nSize = GetRowCount() - 1;
	if(nSize <= 0) m_parPierUserMPhi->RemoveAll();
	else
	{
		m_parPierUserMPhi->SetSize(nSize);
		T_RSDL_MPHI_D MPhiD; 
		for(int i=0 ; i<nSize ; i++)
		{
			MPhiD.dCurvature = _tstof(GetValueRowCol(i+1, 1));
			MPhiD.dMoment    = _tstof(GetValueRowCol(i+1, 2));
			m_parPierUserMPhi->SetAt(i, MPhiD);
		}
	}
}
void CDgnUserMPhiGrid::SetRowSize(int nSize)
{	
	HideRows(0, GetRowCount(), FALSE); // Grid Initializing!!!
	

	ROWCOL nRow, nCol;
	GetCurrentCell(nRow, nCol);

	SetRowCount(nSize+1);
	int nRowCount = GetRowCount();
	HideRows(nRowCount, nRowCount, TRUE); // To Reject Add Bottom_Row		
	SetValueRange(CGXRange(nRowCount, 1), _T(""));
	SetValueRange(CGXRange(nRowCount, 2), _T(""));
	
	nRow = max(1, nRow);
	nCol = max(1, min(nRowCount-1, nCol));

	if(nRowCount >= 1) 
		SetCurrentCell(nRow, nCol);
}