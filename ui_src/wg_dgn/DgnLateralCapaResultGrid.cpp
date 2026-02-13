// DgnLateralCapaResultGrid.cpp: implementation of the CDgnLateralCapaResultGrid class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DgnLateralCapaResultGrid.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

const COLORREF cRLCC_SelZinuColor1   = RGB(166,202,240);
const COLORREF cRLCC_SelZinuColor2   = RGB(136,172,210);
const COLORREF cRLCC_UnSelZinuColor1 = RGB(255,255,255);
const COLORREF cRLCC_UnSelZinuColor2 = RGB(225,225,225);

const COLORREF cRLCC_SelZinuColor3   = RGB(240,188,188);
const COLORREF cRLCC_SelZinuColor4   = RGB(210,158,158);
const COLORREF cRLCC_UnSelZinuColor3 = RGB(255,231,231);
const COLORREF cRLCC_UnSelZinuColor4 = RGB(225,201,201);


CDgnLateralCapaResultGrid::CDgnLateralCapaResultGrid()
{
}

CDgnLateralCapaResultGrid::~CDgnLateralCapaResultGrid()
{
}

BEGIN_MESSAGE_MAP(CDgnLateralCapaResultGrid, CGXGridWnd)
	//{{AFX_MSG_MAP(CDgnLateralCapaResultGrid)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CDgnLateralCapaResultGrid message handlers

BOOL CDgnLateralCapaResultGrid::OnLButtonClickedRowCol(ROWCOL nRow, ROWCOL nCol, UINT nFlags, CPoint pt)
{
	BOOL bCngGrid=FALSE;
	
	COLORREF SelCol1 = cRLCC_SelZinuColor1;
	COLORREF SelCol2 = cRLCC_SelZinuColor2;
	COLORREF SelCol3 = cRLCC_SelZinuColor1;
	COLORREF SelCol4 = cRLCC_SelZinuColor2;

	COLORREF UnSelCol1 = cRLCC_UnSelZinuColor1;
	COLORREF UnSelCol2 = cRLCC_UnSelZinuColor2;
	COLORREF UnSelCol3 = cRLCC_UnSelZinuColor1;
	COLORREF UnSelCol4 = cRLCC_UnSelZinuColor2;

	if(nCol==1 && nRow!=0)
	{
		GetParam()->SetLockReadOnly(FALSE);

		int iChk = _ttoi(GetValueRowCol(nRow,nCol));
		

		if(iChk==1)	SetValueRange(CGXRange(nRow,nCol),_T("0"),gxOverride,0,GX_UPDATENOW);
		else		    SetValueRange(CGXRange(nRow,nCol),_T("1"),gxOverride,0,GX_UPDATENOW);

		
		bCngGrid = TRUE;
		GetParam()->SetLockReadOnly(TRUE);
	}
	return bCngGrid;
}

// Add by ZINU.('03.01.22). Use Spacebar with Toggle Button.
BOOL CDgnLateralCapaResultGrid::ProcessKeys(CWnd *pSender, UINT nMessage, UINT nChar, UINT nRepCnt, UINT flags)
{
	if(nMessage==WM_KEYDOWN && nChar==VK_SPACE)
	{
		ROWCOL nRow, nCol;
		if(GetCurrentCell(nRow,nCol))
		{
			if(!OnLButtonClickedRowCol(nRow,nCol,0,NULL))	return FALSE;
			if(_ttoi(GetValueRowCol(nRow,nCol))==1)	SetValueRange(CGXRange(nRow,nCol),_T("0"),gxOverride,0,GX_UPDATENOW);
			else																		SetValueRange(CGXRange(nRow,nCol),_T("1"),gxOverride,0,GX_UPDATENOW);
			return TRUE;
		}
	}
	return CGXGridWnd::ProcessKeys(pSender, nMessage, nChar, nRepCnt, flags); 
}

//060406 sshan Add
BOOL CDgnLateralCapaResultGrid::IsHiddenByIntend(int nCol)
{
	CGXStyle style;
	CString csHide;
	GetStyleRowCol(0, nCol, style);
	style.GetUserAttribute(4, csHide);
	if (csHide == _T("Yes")) return TRUE;
	else return FALSE;
}

//060406 sshan Add
// 전체 테이블 Copy
BOOL CDgnLateralCapaResultGrid::CopyAll()
{
	/*
	// 원래 Range를 저장하고 의도적으로 숨겨진 Column은 뺀다.
	CGXRangeList OrgRange;
	CGXRangeList* pRangeList = GetParam()->GetRangeList();
	OrgRange = *pRangeList;
	
	SelectRange(CGXRange().SetTable(), FALSE, FALSE);
	for (int i = 0; i <= GetColCount(); i++)
	{
		if (IsHiddenByIntend(i)) 
			SelectRange(CGXRange().SetCols(i), FALSE, FALSE);
		else SelectRange(CGXRange().SetCols(i), TRUE, FALSE);
	}
	m_nClipboardFlags |= GX_DNDCOLHEADER; // 잠시 Column Header Copy 되도록 함
	// 복사하고
	CGXGridWnd::Copy();
	m_nClipboardFlags &= ~GX_DNDCOLHEADER; // Column Header Copy 안 되도록 함
	
	// 원래 RangeList를 복구한다.
	*pRangeList = OrgRange;
	*/
	return TRUE;
}
