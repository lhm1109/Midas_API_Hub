// CMSectItemPSCNormalGrid.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMSectItemPSCNormalGrid.h"


#include "CMSectItemPSCNormal.h"
#include "CMSectItmePSCView.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_SectUtil.h"
#include "..\wg_db\wg_db_SectDB.h"
#include "..\wg_db\wg_db_NewSect.h"


#include "CMSectItemPSCNormalGridDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#define D_JOINT_START_ROW 2
#define D_JOINT_END_ROW 5
#define D_JOINT_ROW_NUM     D_JOINT_END_ROW-D_JOINT_START_ROW+1
#define D_OUTER_START_ROW 7
#define D_OUTER_END_ROW 12
#define D_OUTER_ROW_NUM     D_OUTER_END_ROW-D_OUTER_START_ROW+1
#define D_INNER_START_ROW 14
#define D_INNER_END_ROW 23
#define D_INNER_ROW_NUM     D_INNER_END_ROW-D_INNER_START_ROW+1
#define D_TOTAL_ROW_NUM 23


/////////////////////////////////////////////////////////////////////////////
// CCMSectItemPSCNormalGrid

CCMSectItemPSCNormalGrid::CCMSectItemPSCNormalGrid()
{
	m_pSect = 0;
	m_bInitDone = FALSE;
	m_pParent=0;
	m_bOnStartEditing=FALSE;
}

CCMSectItemPSCNormalGrid::~CCMSectItemPSCNormalGrid()
{
}

void CCMSectItemPSCNormalGrid::InitGrid()
{
	if (!m_bInitDone) Initialize(); // 그리드를 초기화한다.
	else { ResetGrid(); Initialize(); }
	m_bInitDone = TRUE;

	GetParam()->EnableUndo(FALSE);  // undo를 금지시킨다.

	// cell의 width와 height 크기 변경을 금지한다.
	GetParam()->EnableTrackRowHeight(FALSE);
	GetParam()->EnableTrackColWidth(FALSE);

	// column과 row의 위치 이동을 금지시킨다.
	GetParam()->EnableMoveCols(FALSE);
	GetParam()->EnableMoveRows(FALSE);

	// double click시 셀 편집이 되도록 한다.
	GetParam()->SetActivateCellFlags(GX_CAFOCUS_DBLCLICKONCELL);

	// Text만 카피 되게 한다.
	//EnableOleDataSource(GX_DNDTEXT);
	m_nClipboardFlags &= ~GX_DNDSTYLES;

	// Row 헤더와 Column 헤더를 숨긴다.
	HideRows(0, 0);
	HideCols(0, 0);

	// always show vertical scrollbar
	SetScrollBarMode(SB_VERT, gxnEnabled | gxnEnhanced);

	// 표준 스타일 변경
	ChangeStandardStyle(CGXStyle()
			.SetFont(CGXFont_GC().SetSize(10))
			.SetVerticalAlignment(DT_BOTTOM)
			.SetAllowEnter(FALSE));

	LockUpdate(FALSE);
	Redraw();
}

void CCMSectItemPSCNormalGrid::SetStyleOnSizeCell()
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	int i;

	for (i = D_OUTER_START_ROW; i <= D_OUTER_END_ROW; i++)
	{
		SetStyleRange(CGXRange(i, 2), CGXStyle()
			.SetFormat(GX_FMT_FIXED).SetPlaces(4));
		SetStyleRange(CGXRange(i, 4), CGXStyle()
			.SetFormat(GX_FMT_FIXED).SetPlaces(4));
	}

	
	for (i = D_INNER_START_ROW; i <= D_INNER_END_ROW; i++)
	{
		SetStyleRange(CGXRange(i, 2), CGXStyle()
			.SetFormat(GX_FMT_FIXED).SetPlaces(4));
		SetStyleRange(CGXRange(i, 4), CGXStyle()
			.SetFormat(GX_FMT_FIXED).SetPlaces(4));
	}

	 SetStyleRange(CGXRange(D_JOINT_START_ROW, 2), CGXStyle()
			.SetControl(GX_IDS_CTRL_CHECKBOX3D).SetHorizontalAlignment(DT_CENTER));
	 SetStyleRange(CGXRange(D_JOINT_START_ROW, 4), CGXStyle()
			.SetControl(GX_IDS_CTRL_CHECKBOX3D).SetHorizontalAlignment(DT_CENTER));
	 SetStyleRange(CGXRange(D_JOINT_START_ROW+1, 2), CGXStyle()
			.SetControl(GX_IDS_CTRL_CHECKBOX3D).SetHorizontalAlignment(DT_CENTER));
	 SetStyleRange(CGXRange(D_JOINT_START_ROW+1, 4), CGXStyle()
			.SetControl(GX_IDS_CTRL_CHECKBOX3D).SetHorizontalAlignment(DT_CENTER));
	 SetStyleRange(CGXRange(D_JOINT_START_ROW+2, 2), CGXStyle()
			.SetControl(GX_IDS_CTRL_CHECKBOX3D).SetHorizontalAlignment(DT_CENTER));
	 SetStyleRange(CGXRange(D_JOINT_START_ROW+2, 4), CGXStyle()
			.SetControl(GX_IDS_CTRL_CHECKBOX3D).SetHorizontalAlignment(DT_CENTER));
	 SetStyleRange(CGXRange(D_JOINT_START_ROW+3, 2), CGXStyle()
			.SetControl(GX_IDS_CTRL_CHECKBOX3D).SetHorizontalAlignment(DT_CENTER));
	 SetStyleRange(CGXRange(D_JOINT_START_ROW+3, 4), CGXStyle()
			.SetControl(GX_IDS_CTRL_CHECKBOX3D).SetHorizontalAlignment(DT_CENTER));
	 SetStyleRange(CGXRange(D_JOINT_START_ROW+4, 2), CGXStyle()
			.SetControl(GX_IDS_CTRL_CHECKBOX3D).SetHorizontalAlignment(DT_CENTER));
	 SetStyleRange(CGXRange(D_JOINT_START_ROW+4, 4), CGXStyle()
			.SetControl(GX_IDS_CTRL_CHECKBOX3D).SetHorizontalAlignment(DT_CENTER));
	 SetStyleRange(CGXRange(D_INNER_START_ROW+8,4), CGXStyle()
		.SetEnabled(FALSE).SetInterior(::GetSysColor(COLOR_BTNFACE)));
	 SetStyleRange(CGXRange(D_INNER_START_ROW+9,4), CGXStyle()
		.SetEnabled(FALSE).SetInterior(::GetSysColor(COLOR_BTNFACE)));


}

void CCMSectItemPSCNormalGrid::SetDataSource(T_SECT_D* pSect,CWnd* pParent)
{
	ASSERT(pSect);
	m_pSect = pSect;
	m_pParent=pParent;
	
	//m_Cell1.Set(*m_pSect,true,true);
	//m_Cell2.Set(*m_pSect,true,true);
	BOOL bOldLock = LockUpdate(TRUE);
	BOOL bOldReadOnlyLock = GetParam()->IsLockReadOnly();
	GetParam()->SetLockReadOnly(FALSE);
		
	SetRowCount(D_TOTAL_ROW_NUM);
	SetColCount(4);

	// set column width
	SetColWidthByDPI(1, 1, 70);
	SetColWidthByDPI(2, 2, 70);
	SetColWidthByDPI(3, 3, 70);
	SetColWidthByDPI(4, 4, 70);
	SetRowHeight(0, GetRowCount(), globalUtils.ScaleByDPI(20));

	SetStyleRange(CGXRange().SetCols(1, GetColCount()), CGXStyle()
		.SetVerticalAlignment(DT_VCENTER));

	SetCoveredCellsRowCol(D_JOINT_START_ROW-1, 1, D_JOINT_START_ROW-1, 4);
	SetStyleRange(CGXRange(D_JOINT_START_ROW-1, 1), CGXStyle()
		.SetInterior(COLORREF(RGB(0, 0, 255)))
		.SetTextColor(COLORREF(RGB(255, 255, 0)))
		.SetValue(_LS(IDS_CMD_SECT_PSC_Joint_OnOff)));

	SetCoveredCellsRowCol(D_OUTER_START_ROW-1, 1, D_OUTER_START_ROW-1, 4);
 

	SetCoveredCellsRowCol(D_INNER_START_ROW-1, 1, D_INNER_START_ROW-1, 4);
	
		
	

	SetStyleRange(CGXRange().SetCols(1), CGXStyle()
		.SetReadOnly(TRUE)
		.SetEnabled(FALSE)
		.SetInterior(::GetSysColor(COLOR_BTNFACE)));

	SetStyleRange(CGXRange().SetCols(2), CGXStyle()
		.SetHorizontalAlignment(DT_RIGHT)
		.SetValueType(GX_VT_NUMERIC));

	SetStyleRange(CGXRange().SetCols(3), CGXStyle()
		.SetReadOnly(TRUE)
		.SetEnabled(FALSE)
		.SetInterior(::GetSysColor(COLOR_BTNFACE)));

	SetStyleRange(CGXRange().SetCols(4), CGXStyle()
		.SetHorizontalAlignment(DT_RIGHT)
		.SetValueType(GX_VT_NUMERIC));

	SetStyleOnSizeCell();
 
	CString aSizeTitleLeft[] = {
		_T("JO1"),_T("JO3"),_T("JI2"),_T("JI4"),
			
			_T("HO1"),_T("HO2"),_T("HO2-1"),_T("HO2-2"),_T("HO3"),_T("HO3-1"),
			
			_T("HI1"),_T("HI2"),_T("HI2-1"),_T("HI2-2"),_T("HI3"),_T("HI3-1"),_T("HI4"),_T("HI4-1"),_T("HI4-2"),_T("HI5"),
			
	};
	CString aSizeTitleRight[]={
		_T("JO2"),_T("JI1"),_T("JI3"),_T("JI5"),

			_T("BO1"),_T("BO1-1"),_T("BO1-2"),_T("BO2"),_T("BO2-1"),_T("BO3"),

			_T("BI1"),_T("BI1-1"),_T("BI1-2"),_T("BI2-1"),_T("BI3"),_T("BI3-1"),_T("BI3-2"),_T("BI4")
	};


	int aSizeUnit=CUnitCtrl::m_SECT_UNIT.SectBefore.SectI.Size;
	
	CString csUnit;
	CUnitCtrl::GetUnitSystem(aSizeUnit, csUnit);
	
	SetStyleRange(CGXRange(D_OUTER_START_ROW-1, 1), CGXStyle()
		.SetInterior(COLORREF(RGB(0, 0, 255)))
		.SetTextColor(COLORREF(RGB(255, 255, 0)))
		.SetValue(_LS(IDS_CMD0417__Outer)+CString(_T(" ("))+csUnit+CString(_T(")"))));
	SetStyleRange(CGXRange(D_INNER_START_ROW-1, 1), CGXStyle()
		.SetInterior(COLORREF(RGB(0, 0, 255)))
		.SetTextColor(COLORREF(RGB(255, 255, 0)))
		.SetValue(_LS(IDS_CMD0417__Inner)+CString(_T(" ("))+csUnit+CString(_T(")"))));
	int i;
	for(i=D_JOINT_START_ROW;i<=D_JOINT_END_ROW;i++)
	{
		SetValueRange(CGXRange(i,1),aSizeTitleLeft[i-2]);
		SetValueRange(CGXRange(i,3),aSizeTitleRight[i-2]);
	}
	for(i=D_OUTER_START_ROW;i<=D_OUTER_END_ROW;i++)
	{
		SetValueRange(CGXRange(i,1),aSizeTitleLeft[i-3]);
		SetValueRange(CGXRange(i,3),aSizeTitleRight[i-3]);
	}
	for(i=D_INNER_START_ROW;i<=D_INNER_END_ROW;i++)
	{
		SetValueRange(CGXRange(i,1),aSizeTitleLeft[i-4]);
		
	}
	for(i=D_INNER_START_ROW;i<=D_INNER_END_ROW-3;i++)
	{
		SetValueRange(CGXRange(i,3),aSizeTitleRight[i-4]);
		
	}
	if(IsHalfType())
	{
		SetStyleRange(CGXRange(D_INNER_END_ROW-2,4), CGXStyle()
		.SetEnabled(FALSE).SetInterior(::GetSysColor(COLOR_BTNFACE)));
	}
	else
	{
		SetValueRange(CGXRange(D_INNER_END_ROW-2,3),aSizeTitleRight[D_INNER_END_ROW-6]);
			
	}

		



 
	
	GetParam()->SetLockReadOnly(bOldReadOnlyLock);

	LockUpdate(bOldLock);
	if (!bOldLock) Redraw();

	SetRightValidCell(1, 1);
}




void CCMSectItemPSCNormalGrid::ShowData()
{

	CCMSectItemPSCNormal* pWnd=(CCMSectItemPSCNormal*)m_pParent;
	ChangeByType();
	const T_SECT_SECTION_D* pSectionD;
	pSectionD = &m_pSect->SectBefore;
	const T_SECT_SECTBASE_D* pSectBaseD;
	pSectBaseD = &pSectionD->SectI;
	int nJoint;
	if(IsHalfType())
		nJoint=pSectionD->nJoint;
	else
		nJoint=pSectionD->SectI.BuiltUpFlag;
	
	int i,j=0;
	
	SetValueRange(CGXRange(D_JOINT_START_ROW,2), short((nJoint & 0x0001) != 0));
	SetValueRange(CGXRange(D_JOINT_START_ROW,4), short((nJoint & 0x0002) != 0));
	SetValueRange(CGXRange(D_JOINT_START_ROW+1,2), short((nJoint & 0x0004) != 0));
	SetValueRange(CGXRange(D_JOINT_START_ROW+1,4), short((nJoint & 0x0008) != 0));
	SetValueRange(CGXRange(D_JOINT_START_ROW+2,2), short((nJoint & 0x0010) != 0));
	SetValueRange(CGXRange(D_JOINT_START_ROW+2,4), short((nJoint & 0x0020) != 0));
	SetValueRange(CGXRange(D_JOINT_START_ROW+3,2), short((nJoint & 0x0040) != 0));
	SetValueRange(CGXRange(D_JOINT_START_ROW+3,4), short((nJoint & 0x0080) != 0));
 
	for(i=D_OUTER_START_ROW;i<=D_OUTER_END_ROW;i++)
	{
		SetValueRange(CGXRange(i,2),pSectBaseD->Size[j++]);
	}
	for(i=D_OUTER_START_ROW;i<=D_OUTER_END_ROW;i++)
	{
		SetValueRange(CGXRange(i,4),pSectBaseD->Size[j++]);
	}
	for(i=D_INNER_START_ROW;i<=D_INNER_END_ROW;i++)
	{
		SetValueRange(CGXRange(i,2),pSectBaseD->Size[j++]);
	}
	for(i=D_INNER_START_ROW;i<=D_INNER_END_ROW-3;i++)
	{
		SetValueRange(CGXRange(i,4),pSectBaseD->Size[j++]);
	}
	if(!IsHalfType())
	{
		SetValueRange(CGXRange(D_INNER_END_ROW-2,4),pSectBaseD->Size[j++]);
	}
}


BOOL CCMSectItemPSCNormalGrid::SaveData()
{
	T_SECT_SECTION_D* pSectionD;
	pSectionD = &m_pSect->SectBefore;
	T_SECT_SECTBASE_D* pSectBaseD;
	pSectBaseD = &pSectionD->SectI;
	 int i,j=0;
	CString csNum;
	double dNum;
	BOOL bNum;
	int Set=0x01;
	int nJoint=0;
	

	csNum = GetEditingValue(D_JOINT_START_ROW, 2);
	GetDoubleValue(csNum, dNum);
	bNum=(BOOL)dNum;
	if (bNum) nJoint=nJoint|Set; 
	Set <<= 1;

	csNum = GetEditingValue(D_JOINT_START_ROW, 4);
	GetDoubleValue(csNum, dNum);
	bNum=(BOOL)dNum;
	if (bNum) nJoint |= Set; Set <<= 1;

	csNum = GetEditingValue(D_JOINT_START_ROW+1, 2);
	GetDoubleValue(csNum, dNum);
	bNum=(BOOL)dNum;
	if (bNum) nJoint |= Set; Set <<= 1;

	csNum = GetEditingValue(D_JOINT_START_ROW+1, 4);
	GetDoubleValue(csNum, dNum);
	bNum=(BOOL)dNum;
	if (bNum) nJoint |= Set; Set <<= 1;

	csNum = GetEditingValue(D_JOINT_START_ROW+2, 2);
	GetDoubleValue(csNum, dNum);
	bNum=(BOOL)dNum;
	if (bNum) nJoint |= Set; Set <<= 1;

	csNum = GetEditingValue(D_JOINT_START_ROW+2, 4);
	GetDoubleValue(csNum, dNum);
	bNum=(BOOL)dNum;
	if (bNum) nJoint |= Set; Set <<= 1;

	csNum = GetEditingValue(D_JOINT_START_ROW+3, 2);
	GetDoubleValue(csNum, dNum);
	bNum=(BOOL)dNum;
	if (bNum) nJoint |= Set; Set <<= 1;

	csNum = GetEditingValue(D_JOINT_START_ROW+3, 4);
	GetDoubleValue(csNum, dNum);
	bNum=(BOOL)dNum;
	if (bNum) nJoint |= Set; Set <<= 1;
	if(IsHalfType())
		pSectionD->nJoint=nJoint;
	else
		pSectionD->SectI.BuiltUpFlag=nJoint;




	for(i=D_OUTER_START_ROW;i<=D_OUTER_END_ROW;i++)
	{
		csNum=GetEditingValue(i,2);
		GetDoubleValue(csNum,dNum);
		pSectBaseD->Size[j++]=dNum;
	}
	for(i=D_OUTER_START_ROW;i<=D_OUTER_END_ROW;i++)
	{
		csNum=GetEditingValue(i,4);
		GetDoubleValue(csNum,dNum);
		pSectBaseD->Size[j++]=dNum;
	}
	for(i=D_INNER_START_ROW;i<=D_INNER_END_ROW;i++)
	{
		csNum=GetEditingValue(i,2);
		GetDoubleValue(csNum,dNum);
		pSectBaseD->Size[j++]=dNum;
	}
	for(i=D_INNER_START_ROW;i<=D_INNER_END_ROW-3;i++)
	{
		csNum=GetEditingValue(i,4);
		GetDoubleValue(csNum,dNum);
		pSectBaseD->Size[j++]=dNum;
	} 
	if(!IsHalfType())
	{
		csNum=GetEditingValue(D_INNER_END_ROW-2,4);
		GetDoubleValue(csNum,dNum);
		pSectBaseD->Size[j++]=dNum;
	}

	
 
	
	return TRUE;
}

void CCMSectItemPSCNormalGrid::OnSizeChange()
{
	// 임시(다시 그려주기)
	CCMSectItemPSCNormal *pParent = (CCMSectItemPSCNormal*)((CCMSectItemPSCNormalGridDlg*)m_pParent)->m_pWnd;
	if (pParent != 0)
	{
		pParent->UpdateCurDatas();
		
		pParent->RedrawSection();
		ChangeByType();

	} 
}

BOOL CCMSectItemPSCNormalGrid::SetLeftValidCell(ROWCOL nRow, ROWCOL nCol)
{
	ROWCOL i, j;
	i = nRow; j = nCol-1; // 현재 행 이전 열
	if (nCol == 0 || j < 1) { i--; j = GetColCount(); }  

	while (i >= 1)
	{
		while (j >= 1)
		{
			// 열이 선택될 수 있는지 조사
			//if (SetCurrentCell(i, j)) return TRUE;
			CGXStyle style;
			ComposeStyleRowCol(i, j, &style);
			if (!style.GetIncludeEnabled() || style.GetEnabled())
			{
				if (SetCurrentCell(i, j)) return TRUE;
			}
			j--;  // 이전 열을 조사
		}
		i--;  // 이전 줄로 이동
		j = GetColCount();
	}

	return FALSE;
}
BOOL CCMSectItemPSCNormalGrid::SetRightValidCell(ROWCOL nRow, ROWCOL nCol)
{
	ROWCOL i, j;
	i = nRow; j = nCol+1; // 현재 행 다음 열
	if (j > GetColCount()) { i++; j = 1; }

	while (i <= GetRowCount())
	{
		while (j <= GetColCount())
		{
			// 열이 선택될 수 있는지 조사
			//if (SetCurrentCell(i, j)) return TRUE;
			CGXStyle style;
			ComposeStyleRowCol(i, j, &style);
			if (!style.GetIncludeEnabled() || style.GetEnabled())
			{
				if (SetCurrentCell(i, j)) return TRUE;
			}
			j++;  // 다음 열을 조사
		}
		i++;  // 다음 줄로 이동
		j = 1;
	}

	return FALSE;
}

BOOL CCMSectItemPSCNormalGrid::SetDownValidCell(ROWCOL nRow, ROWCOL nCol)
{
	ROWCOL i,j;
	i=nRow+1;
	j=nCol;
	if (nRow > GetRowCount()) { i=1; }  

	while (i <=GetRowCount())
	{
		CGXStyle style;
		ComposeStyleRowCol(i, j, &style);
		if (!style.GetIncludeEnabled() || style.GetEnabled())
		{
			if (SetCurrentCell(i, j)) return TRUE;
		}
		i++;  // 이전 열을 조사
		
	}

	return FALSE;
}

BOOL CCMSectItemPSCNormalGrid::SetUpValidCell(ROWCOL nRow, ROWCOL nCol)
{
	ROWCOL i,j;
	i = nRow-1;
	j = nCol;

	if (i < 1) { i= GetRowCount(); }

	while (i >=1)
	{
		CGXStyle style;
		ComposeStyleRowCol(i, j, &style);
		if (!style.GetIncludeEnabled() || style.GetEnabled())
		{
			if (SetCurrentCell(i, j)) return TRUE;
		}
		
		i--;  // 다음 줄로 이동
		
	}

	return FALSE;
}

CString CCMSectItemPSCNormalGrid::GetEditingValue(ROWCOL nRow, ROWCOL nCol)
{
	CString csNum;
	BOOL bGet = FALSE;
	ROWCOL ncRow, ncCol;
	if (GetCurrentCell(ncRow, ncCol))
	{
		if (ncRow == nRow && ncCol == nCol)
		{
			CGXControl* pControl = GetControl(nRow, nCol);
			const CGXStyle style = LookupStyleRowCol(nRow, nCol);
			if (pControl)
			{
				if (pControl->IsActive())
					bGet = pControl->GetValue(csNum);
				else if (style.GetIncludeControl() && style.GetControl() == GX_IDS_CTRL_CHECKBOX3D)
				{
					bGet = pControl->GetValue(csNum);
					if (m_bOnStartEditing)
					{
						if (csNum == _T("0")) csNum = _T("1");
						else csNum = _T("0");
					}
				}
			}
		}
	}
	if (!bGet) csNum = GetValueRowCol(nRow, nCol);
	return csNum;
}

BEGIN_MESSAGE_MAP(CCMSectItemPSCNormalGrid, CGXGridWnd)
	//{{AFX_MSG_MAP(CCMSectItemPSCNormalGrid)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CCMSectItemPSCNormalGrid message handlers

BOOL CCMSectItemPSCNormalGrid::ProcessKeys(CWnd *pSender, UINT nMessage, UINT nChar, UINT nRepCnt, UINT flags)
{
	CRowColArray awRows;
	ROWCOL nRow=1, nCol=1;
	//CGXControl* pControl;

	if (!GetCurrentCell(nRow, nCol)) // if no current cell, default action
		return CGXGridWnd::ProcessKeys(pSender, nMessage, nChar, nRepCnt, flags);

	//pControl = GetControl(nRow, nCol);
	if (nMessage == WM_KEYDOWN)
	{
		switch(nChar)
		{
		case VK_TAB: 
			{
				BOOL bShift = GetKeyState(VK_SHIFT) & 0x8000;
				if (bShift && SetLeftValidCell(nRow, nCol-1)) break;
				if (!bShift && SetRightValidCell(nRow, nCol+1)) break;

				CWnd* pWnd = GetParent();
				if (pWnd && pWnd->GetSafeHwnd())
				{
					//pWnd->PostMessage(nMessage, nChar, nRepCnt);
					CWnd* pNextWnd = pWnd->GetNextDlgTabItem(this, bShift);
					if (pNextWnd && pNextWnd->GetSafeHwnd()) pNextWnd->SetFocus();
				}
			}
			break;
		case VK_RETURN: case VK_RIGHT:  // 다음 셀로 이동
			if (nRow < GetRowCount())
				return SetRightValidCell(nRow, nCol+1);
			break;
		case VK_DOWN:
			if (nRow < GetRowCount())
				return SetDownValidCell(nRow, nCol);
			break;
		case VK_LEFT:  // 이전 셀로 이동
			if (nRow > 1)
			{
				return SetLeftValidCell(nRow, nCol-1);
			}
			break;
		case VK_UP:
			if(nRow>1)
			{
				return SetUpValidCell(nRow,nCol);
			}
			break;

		}
	}
	return CGXGridWnd::ProcessKeys(pSender, nMessage, nChar, nRepCnt, flags);
}

BOOL CCMSectItemPSCNormalGrid::OnValidateCell(ROWCOL nRow, ROWCOL nCol)
{
	CString csNum = GetEditingValue(nRow, nCol);
	double dNum;
	if (GetDoubleValue(csNum, dNum)) return TRUE;
	return FALSE;
}

BOOL CCMSectItemPSCNormalGrid::OnActivateGrid(BOOL bActivate)
{
	if (bActivate) return CGXGridWnd::OnActivateGrid(bActivate);
	ROWCOL nRow, nCol;
	if (GetCurrentCell(nRow, nCol))
	{
		if (!OnValidateCell(nRow, nCol)) 
		{
			CGXControl* pControl = GetControl(nRow, nCol);
			if (pControl && pControl->IsActive())
			{
				  pControl->OnCancelEditing();
					pControl->Init(nRow, nCol);
					pControl->Refresh();
					pControl->OnCanceledEditing();
			}
		}
	}
	return CGXGridWnd::OnActivateGrid(bActivate);
}

void CCMSectItemPSCNormalGrid::OnModifyCell(ROWCOL nRow, ROWCOL nCol)
{
	CGXGridWnd::OnModifyCell(nRow, nCol);
	SaveData();
	OnSizeChange();

}

BOOL CCMSectItemPSCNormalGrid::GetDoubleValue(CString& csNum, double& dNum)
{
	int ecode;
	if (!IsValidDoubleNumber(csNum, ecode)) return InvalidDoubleNumber(ecode);
	ConvertFormulaToValue(csNum);
	dNum = _tstof(csNum);
	return TRUE;
}

// nErrCode : 1(수식 에러), 2(숫자 아님), 3(범위 초과)
BOOL CCMSectItemPSCNormalGrid::IsValidDoubleNumber(CString value, int &nErrCode)
{
	value.TrimLeft();
	value.TrimRight();
	if (value == _T("")) return TRUE; // 공백은 0으로 생각하여 TRUE 리턴
	// 수식이면 변환, 수식이 아니면 TRUE 반환(수식은 첫자가 '=' 임)
	if (!ConvertFormulaToValue(value)) { nErrCode = 1; return FALSE; }

	int ecode;
	// 숫자인지 문자열인지 검사
	if (!IsNumeric(value, ecode))
	{
		if (ecode == 1) nErrCode = 3;
		else nErrCode = 2;
		return FALSE;
	}

	TCHAR *num = new TCHAR[value.GetLength()+1];
	TCHAR *endp;
	double n;

	_stprintf(num, value);
	n = _tcstod(num, &endp);
	delete[] num;

	// 실수 범위 검사
	if (n == HUGE_VAL || n == -HUGE_VAL) { nErrCode = 3; return FALSE; }

	return TRUE;
}

BOOL CCMSectItemPSCNormalGrid::ConvertFormulaToValue(CString& value)
{
	if (GetSheetContext() == NULL && value.GetLength() > 0 && value.GetAt(0) == _T('='))
	{
		// 대문자 E로 표시된 지수를 소문자 e로 변경
		value.Replace(_T("E"), _T("e"));

		// Formula engine이 꺼져 있는데 수식이 입력되어 있으면
		CGXFormulaSheet fs;
		CGXFormula fm;
		if (!fs.ParseExpression(value, fm)) 
		{
			SetWarningText(_LS(IDS_CMD0417__Error___Invalid_formula_));
			return FALSE;
		}
		if (!fs.EvaluateExpression(value, fm))
		{
			SetWarningText(_LS(IDS_CMD0417__Error___Can_t_evaluate_formula_));
			return FALSE;
		}
	}
	return TRUE;
}

BOOL CCMSectItemPSCNormalGrid::IsNumeric(CString str, int& ecode)
{
	if (str.GetLength() < 1) return FALSE;

	TCHAR *num = new TCHAR[str.GetLength()+1];
	TCHAR *endp;
	double n;

	_stprintf(num, str);
	n = _tcstod(num, &endp);
	BOOL bAllScaned = endp == &num[str.GetLength()];
	delete[] num;

	if (n == HUGE_VAL || n == -HUGE_VAL) 
		ecode = 1;
	else if (!bAllScaned) 
		ecode = 2;
	else ecode = 0;

	if (ecode) return FALSE;
	else return TRUE;
}

BOOL CCMSectItemPSCNormalGrid::InvalidDoubleNumber(int nErrCode)
{
	switch(nErrCode)
	{
	case 1: return ValidateError(_LS(IDS_CMD0417__Formula_is_invalid_)); 
	case 2: return ValidateError(_LS(IDS_CMD0417__Character_exist_in_the_number_));
	case 3: return ValidateError(_LS(IDS_CMD0417__Out_of_value_range_));
	}
	return FALSE;
}

BOOL CCMSectItemPSCNormalGrid::ValidateError(LPCTSTR lpszError)
{
	SetWarningText(lpszError);
	return FALSE;
}

BOOL CCMSectItemPSCNormalGrid::OnEndEditing(ROWCOL nRow, ROWCOL nCol)
{
	BOOL bModified = GetControl(nRow, nCol)->GetModify();
	BOOL bResult = CGXGridWnd::OnEndEditing(nRow, nCol);
	if (bModified)
	{
		CString value = GetValueRowCol(nRow, nCol);
		if (ConvertFormulaToValue(value)) SetExpressionRowCol(nRow, nCol, value);
	}
	return bResult;
}

BOOL CCMSectItemPSCNormalGrid::OnPasteFromClipboard(const CGXRange &range)
{
	CGXGridWnd x;
	CGXRange r;
	CGXRange toRange;
	toRange=range;
	r.top = r.bottom = 1;
	r.left = r.right = 1;
	x.Create(WS_CHILD, CRect(0, 0, 1, 1), GetParent(), 0);
	x.Initialize();
	x.GetParam()->EnableUndo(FALSE);
	x.LockUpdate(TRUE);
	x.SetRowCount(0); x.SetColCount(1);
	x.OnPasteFromClipboard(r);

	r.top = 1; r.left = 1;
	r.bottom = x.GetRowCount(); r.right = x.GetColCount();

	if(!VerifyPasteRange(r,toRange))
	 return FALSE;

	int fromRows,fromCols,toCols,toRows;
	fromRows=r.bottom-r.top+1;
	fromCols=r.right-r.left+1;
	toRows=GetSelectedRowsCount(toRange.top,toRange.bottom);
	toCols=GetSelectedColsCount(toRange.left,toRange.right);
	int fromi,fromj,toi,toj;
	fromi=1;
	for(toi=toRange.top;toi<=toRange.bottom;toi++)
	{
		fromj=1;
		if(toi==D_OUTER_START_ROW-1||toi==D_INNER_START_ROW-1)
			continue;
		for(toj=toRange.left;toj<=toRange.right;toj++)
		{
			if(toj==1||toj==3)
				continue;
			BOOL bCondition=0;
			bCondition=(toi==D_INNER_END_ROW||toi==D_INNER_END_ROW-1||(IsHalfType()&&toi==D_INNER_END_ROW-2))&&toj==4;
			if(!bCondition)
			{
				SetValueRange(CGXRange(toi,toj),x.GetValueRowCol(fromi,fromj));
				if(!OnValidateCell(toi,toj))
				{
					DisplayWarningText();
					ShowData();
					return FALSE;
				}
			}

			if(fromCols!=1) fromj++;
		}
		if(fromRows!=1)  fromi++;
		
	}
	
	SaveData();
	OnSizeChange();
	ShowData();
	return TRUE;
}

BOOL CCMSectItemPSCNormalGrid::CopyRange(const CGXRangeList& selList)
{
	CGXRangeList mSelList;
	CGXRange* mRange;
	POSITION position=selList.GetHeadPosition();
	while(TRUE)
	{
		if(!position||position<0)
			break;
		mRange=new CGXRange();
		if(mRange->IntersectRange(CGXRange(D_JOINT_START_ROW,2,D_JOINT_END_ROW,2),selList.GetAt(position)))
		{
			mSelList.AddTail(mRange);
			mRange=new CGXRange();
		}
		if(mRange->IntersectRange(CGXRange(D_JOINT_START_ROW,4,D_JOINT_END_ROW-2,4),selList.GetAt(position)))
		{
			mSelList.AddTail(mRange);
			mRange=new CGXRange();
		}
		if(mRange->IntersectRange(CGXRange(D_OUTER_START_ROW,2,D_OUTER_END_ROW,2),selList.GetAt(position)))
		{
			mSelList.AddTail(mRange);
			mRange=new CGXRange();
		}
		if(mRange->IntersectRange(CGXRange(D_OUTER_START_ROW,4,D_OUTER_END_ROW,4),selList.GetAt(position)))
		{
			mSelList.AddTail(mRange);
			mRange=new CGXRange();
		}
		if(mRange->IntersectRange(CGXRange(D_INNER_START_ROW,2,D_INNER_END_ROW,2),selList.GetAt(position)))
		{
			mSelList.AddTail(mRange);
			mRange=new CGXRange();
		}
		if(mRange->IntersectRange(CGXRange(D_INNER_START_ROW,4,D_INNER_END_ROW,4),selList.GetAt(position)))
		{
			mSelList.AddTail(mRange);
	 
		}
		
		else
			delete mRange;
		selList.GetNext(position);
	}
 
	return CGXGridWnd::CopyRange(mSelList);
}
BOOL CCMSectItemPSCNormalGrid::VerifyPasteRange(CGXRange& from,CGXRange& to)
{
	
	int fromCols,fromRows,toCols,toRows;
	fromCols=from.right-from.left+1;
	fromRows=from.bottom-from.top+1;
	toRows=GetSelectedRowsCount(to.top,to.bottom);
	toCols=GetSelectedColsCount(to.left,to.right);
	
	if(toRows==1&&toCols==1)
	{
		int i;
		for(i=to.left;i<=4;i++)
		{
			if(GetSelectedColsCount(to.left,i)==fromCols)
			{
				to.right=i;
				break;
			}
		}
		if(fromCols!=1&&to.right==to.left)
		{
			MessageBox(_LS(IDS_CMD_PASTE_EXCEED_COLUMN));
			return FALSE;
		}
		for(i=to.top;i<=D_TOTAL_ROW_NUM;i++)
		{
			if(GetSelectedRowsCount(to.top,i)==fromRows)
			{
				to.bottom=i;
				break;
			}
		}
		if(fromRows!=1&&to.bottom==to.top)
		{
			MessageBox(_LS(IDS_CMD_PASTE_EXCEED_ROW));
			return FALSE;
		}
		toRows=GetSelectedRowsCount(to.top,to.bottom);
		toCols=GetSelectedColsCount(to.left,to.right);
	}

		
		
	
	if(fromCols==1)
	{
		if(fromRows==1||fromRows==toRows) return TRUE;
	}
	else if(fromRows==1)
	{
		if(fromCols==1||fromCols==toCols)  return TRUE;
	}
	else if(fromRows==toRows&&fromCols==toCols)
		return TRUE;
	MessageBox(_LS(IDS_CMD_PASTE_RANGE_MISMATCH));
	return FALSE;
}
int CCMSectItemPSCNormalGrid::GetSelectedColsCount(int left,int right)
{
	int toCols;
	toCols=0;
	if(left<=2&&right>=2)
		toCols++;
	if(left<=4&&right>=4)
		toCols++;
	return toCols;
}
int CCMSectItemPSCNormalGrid::GetSelectedRowsCount(int top,int bottom)
{
	int toRows;
	toRows=bottom-top+1;
	if(top==D_JOINT_START_ROW-1)
		toRows--;

	if(top<=D_OUTER_START_ROW-1&&bottom>=D_OUTER_START_ROW-1)
		toRows--;
	if(top<=D_INNER_START_ROW-1&&bottom>=D_INNER_START_ROW-1)
		toRows--;
	return toRows;
}
void CCMSectItemPSCNormalGrid::ChangeByJoint()
{
	BOOL JO1, JO2, JO3, JI1, JI2, JI3, JI4, JI5;
	if(IsHalfType())
	{
		T_SECT_D_PSC_HALF Cell;
		Cell.Set(*m_pSect,TRUE,TRUE);
		JO1=Cell.JO1;
		JO2=Cell.JO2;
		JO3=Cell.JO3;
		JI1=Cell.JI1;
		JI2=Cell.JI2;
		JI3=Cell.JI3;
		JI4=Cell.JI4;
		JI5=Cell.JI5;
	}
	else
	{
		T_SECT_D_PSC_2CELL Cell;
		Cell.Set(*m_pSect,TRUE,TRUE);
		JO1=Cell.JO1;
		JO2=Cell.JO2;
		JO3=Cell.JO3;
		JI1=Cell.JI1;
		JI2=Cell.JI2;
		JI3=Cell.JI3;
		JI4=Cell.JI4;
		JI5=Cell.JI5;
	}
	SetStyleRange(CGXRange(D_OUTER_START_ROW+2,2), CGXStyle()
		.SetEnabled(JO1).SetInterior(::GetSysColor((JO1)?COLOR_WINDOW:COLOR_BTNFACE)));
	SetStyleRange(CGXRange(D_OUTER_START_ROW+1,4), CGXStyle()
		.SetEnabled(JO1).SetInterior(::GetSysColor((JO1)?COLOR_WINDOW:COLOR_BTNFACE)));

	SetStyleRange(CGXRange(D_OUTER_START_ROW+3,2), CGXStyle()
		.SetEnabled(JO2).SetInterior(::GetSysColor((JO2)?COLOR_WINDOW:COLOR_BTNFACE)));
	SetStyleRange(CGXRange(D_OUTER_START_ROW+2,4), CGXStyle()
		.SetEnabled(JO2).SetInterior(::GetSysColor((JO2)?COLOR_WINDOW:COLOR_BTNFACE)));

	SetStyleRange(CGXRange(D_OUTER_START_ROW+5,2), CGXStyle()
		.SetEnabled(JO3).SetInterior(::GetSysColor((JO3)?COLOR_WINDOW:COLOR_BTNFACE)));
	SetStyleRange(CGXRange(D_OUTER_START_ROW+4,4), CGXStyle()
		.SetEnabled(JO3).SetInterior(::GetSysColor((JO3)?COLOR_WINDOW:COLOR_BTNFACE)));

	SetStyleRange(CGXRange(D_INNER_START_ROW+2,2), CGXStyle()
		.SetEnabled(JI1).SetInterior(::GetSysColor((JI1)?COLOR_WINDOW:COLOR_BTNFACE)));
	SetStyleRange(CGXRange(D_INNER_START_ROW+1,4), CGXStyle()
		.SetEnabled(JI1).SetInterior(::GetSysColor((JI1)?COLOR_WINDOW:COLOR_BTNFACE)));

	SetStyleRange(CGXRange(D_INNER_START_ROW+3,2), CGXStyle()
		.SetEnabled(JI2).SetInterior(::GetSysColor((JI2)?COLOR_WINDOW:COLOR_BTNFACE)));
	SetStyleRange(CGXRange(D_INNER_START_ROW+2,4), CGXStyle()
		.SetEnabled(JI2).SetInterior(::GetSysColor((JI2)?COLOR_WINDOW:COLOR_BTNFACE)));

	SetStyleRange(CGXRange(D_INNER_START_ROW+5,2), CGXStyle()
		.SetEnabled(JI3).SetInterior(::GetSysColor((JI3)?COLOR_WINDOW:COLOR_BTNFACE)));
	SetStyleRange(CGXRange(D_INNER_START_ROW+3,4), CGXStyle()
		.SetEnabled(JI3).SetInterior(::GetSysColor((JI3)?COLOR_WINDOW:COLOR_BTNFACE)));

	SetStyleRange(CGXRange(D_INNER_START_ROW+8,2), CGXStyle()
		.SetEnabled(JI4).SetInterior(::GetSysColor((JI4)?COLOR_WINDOW:COLOR_BTNFACE)));
	SetStyleRange(CGXRange(D_INNER_START_ROW+6,4), CGXStyle()
		.SetEnabled(JI4).SetInterior(::GetSysColor((JI4)?COLOR_WINDOW:COLOR_BTNFACE)));

	SetStyleRange(CGXRange(D_INNER_START_ROW+7,2), CGXStyle()
		.SetEnabled(JI5).SetInterior(::GetSysColor((JI5)?COLOR_WINDOW:COLOR_BTNFACE)));
	SetStyleRange(CGXRange(D_INNER_START_ROW+5,4), CGXStyle()
		.SetEnabled(JI5).SetInterior(::GetSysColor((JI5)?COLOR_WINDOW:COLOR_BTNFACE)));

	//CDlgUtil::CtrlEnableDisable(this, arJO1Group, m_bJO1);
	//CDlgUtil::CtrlEnableDisable(this, arJO2Group, m_bJO2);
	//CDlgUtil::CtrlEnableDisable(this, arJO3Group, m_bJO3);
	//CDlgUtil::CtrlEnableDisable(this, arJI1Group, m_bJI1);
	//CDlgUtil::CtrlEnableDisable(this, arJI2Group, m_bJI2);
	//CDlgUtil::CtrlEnableDisable(this, arJI3Group, m_bJI3);
	//CDlgUtil::CtrlEnableDisable(this, arJI4Group, m_bJI4);
	//CDlgUtil::CtrlEnableDisable(this, arJI5Group, m_bJI5);
}
void CCMSectItemPSCNormalGrid::ChangeByCell()
{


	//CString strTemp;
	//int nIndex = m_cSecTypeCB.GetCurSel();
	//m_cSecTypeCB.GetLBText(nIndex, strTemp);
	
	SetStyleRange(CGXRange(D_JOINT_START_ROW+1,4), CGXStyle()
		.SetEnabled(TRUE).SetInterior(::GetSysColor(COLOR_WINDOW)));
	SetStyleRange(CGXRange(D_JOINT_START_ROW+2,2), CGXStyle()
		.SetEnabled(TRUE).SetInterior(::GetSysColor(COLOR_WINDOW)));
	SetStyleRange(CGXRange(D_JOINT_START_ROW+2,4), CGXStyle()
		.SetEnabled(TRUE).SetInterior(::GetSysColor(COLOR_WINDOW)));
	SetStyleRange(CGXRange(D_JOINT_START_ROW+3,2), CGXStyle()
		.SetEnabled(TRUE).SetInterior(::GetSysColor(COLOR_WINDOW)));
	SetStyleRange(CGXRange(D_JOINT_START_ROW+3,4), CGXStyle()
		.SetEnabled(TRUE).SetInterior(::GetSysColor(COLOR_WINDOW)));

	//CDlgUtil::CtrlEnableDisable(this, arComnGroup, TRUE);

	SetStyleRange(CGXRange(D_INNER_START_ROW+1,2), CGXStyle()
		.SetEnabled(TRUE).SetInterior(::GetSysColor(COLOR_WINDOW)));
	SetStyleRange(CGXRange(D_INNER_START_ROW+4,2), CGXStyle()
		.SetEnabled(TRUE).SetInterior(::GetSysColor(COLOR_WINDOW)));
	SetStyleRange(CGXRange(D_INNER_START_ROW+6,2), CGXStyle()
		.SetEnabled(TRUE).SetInterior(::GetSysColor(COLOR_WINDOW)));
	SetStyleRange(CGXRange(D_INNER_START_ROW+9,2), CGXStyle()
		.SetEnabled(TRUE).SetInterior(::GetSysColor(COLOR_WINDOW)));
	SetStyleRange(CGXRange(D_INNER_START_ROW,4), CGXStyle()
		.SetEnabled(TRUE).SetInterior(::GetSysColor(COLOR_WINDOW)));
	SetStyleRange(CGXRange(D_INNER_START_ROW+4,4), CGXStyle()
		.SetEnabled(TRUE).SetInterior(::GetSysColor(COLOR_WINDOW)));

	//CDlgUtil::CtrlEnableDisable(this, arNoneGroup, TRUE);

	SetStyleRange(CGXRange(D_INNER_START_ROW+1,2), CGXStyle()
		.SetEnabled(TRUE).SetInterior(::GetSysColor(COLOR_WINDOW)));
	SetStyleRange(CGXRange(D_INNER_START_ROW+6,2), CGXStyle()
		.SetEnabled(TRUE).SetInterior(::GetSysColor(COLOR_WINDOW)));
	SetStyleRange(CGXRange(D_INNER_START_ROW,4), CGXStyle()
		.SetEnabled(TRUE).SetInterior(::GetSysColor(COLOR_WINDOW)));
	SetStyleRange(CGXRange(D_INNER_START_ROW+4,4), CGXStyle()
		.SetEnabled(TRUE).SetInterior(::GetSysColor(COLOR_WINDOW)));

	//CDlgUtil::CtrlEnableDisable(this, arCircGroup, TRUE);

	

	if(GetHalfType() == 0)  // None
	{
	 
		SetStyleRange(CGXRange(D_JOINT_START_ROW+1,4), CGXStyle()
		.SetEnabled(FALSE).SetInterior(::GetSysColor(COLOR_BTNFACE)));
		SetStyleRange(CGXRange(D_JOINT_START_ROW+2,2), CGXStyle()
			.SetEnabled(FALSE).SetInterior(::GetSysColor(COLOR_BTNFACE)));
		SetStyleRange(CGXRange(D_JOINT_START_ROW+2,4), CGXStyle()
			.SetEnabled(FALSE).SetInterior(::GetSysColor(COLOR_BTNFACE)));
		SetStyleRange(CGXRange(D_JOINT_START_ROW+3,2), CGXStyle()
			.SetEnabled(FALSE).SetInterior(::GetSysColor(COLOR_BTNFACE)));
		SetStyleRange(CGXRange(D_JOINT_START_ROW+3,4), CGXStyle()
			.SetEnabled(FALSE).SetInterior(::GetSysColor(COLOR_BTNFACE)));
		//CDlgUtil::CtrlEnableDisable(this, arComnGroup, FALSE);
		SetStyleRange(CGXRange(D_INNER_START_ROW+1,2), CGXStyle()
		.SetEnabled(FALSE).SetInterior(::GetSysColor(COLOR_BTNFACE)));
		SetStyleRange(CGXRange(D_INNER_START_ROW+4,2), CGXStyle()
			.SetEnabled(FALSE).SetInterior(::GetSysColor(COLOR_BTNFACE)));
		SetStyleRange(CGXRange(D_INNER_START_ROW+6,2), CGXStyle()
			.SetEnabled(FALSE).SetInterior(::GetSysColor(COLOR_BTNFACE)));
		SetStyleRange(CGXRange(D_INNER_START_ROW+9,2), CGXStyle()
			.SetEnabled(FALSE).SetInterior(::GetSysColor(COLOR_BTNFACE)));
		SetStyleRange(CGXRange(D_INNER_START_ROW,4), CGXStyle()
			.SetEnabled(FALSE).SetInterior(::GetSysColor(COLOR_BTNFACE)));
		SetStyleRange(CGXRange(D_INNER_START_ROW+4,4), CGXStyle()
			.SetEnabled(FALSE).SetInterior(::GetSysColor(COLOR_BTNFACE)));
		//CDlgUtil::CtrlEnableDisable(this, arNoneGroup, FALSE);

	 
	}
	else if(GetHalfType() == 1) // Circ
	{
		
		SetStyleRange(CGXRange(D_JOINT_START_ROW+1,4), CGXStyle()
		.SetEnabled(FALSE).SetInterior(::GetSysColor(COLOR_BTNFACE)));
		SetStyleRange(CGXRange(D_JOINT_START_ROW+2,2), CGXStyle()
			.SetEnabled(FALSE).SetInterior(::GetSysColor(COLOR_BTNFACE)));
		SetStyleRange(CGXRange(D_JOINT_START_ROW+2,4), CGXStyle()
			.SetEnabled(FALSE).SetInterior(::GetSysColor(COLOR_BTNFACE)));
		SetStyleRange(CGXRange(D_JOINT_START_ROW+3,2), CGXStyle()
			.SetEnabled(FALSE).SetInterior(::GetSysColor(COLOR_BTNFACE)));
		SetStyleRange(CGXRange(D_JOINT_START_ROW+3,4), CGXStyle()
			.SetEnabled(FALSE).SetInterior(::GetSysColor(COLOR_BTNFACE)));
		//CDlgUtil::CtrlEnableDisable(this, arComnGroup, FALSE);
		SetStyleRange(CGXRange(D_INNER_START_ROW+1,2), CGXStyle()
		.SetEnabled(FALSE).SetInterior(::GetSysColor(COLOR_BTNFACE)));
		SetStyleRange(CGXRange(D_INNER_START_ROW+6,2), CGXStyle()
			.SetEnabled(FALSE).SetInterior(::GetSysColor(COLOR_BTNFACE)));
		SetStyleRange(CGXRange(D_INNER_START_ROW,4), CGXStyle()
			.SetEnabled(FALSE).SetInterior(::GetSysColor(COLOR_BTNFACE)));
		SetStyleRange(CGXRange(D_INNER_START_ROW+4,4), CGXStyle()
			.SetEnabled(FALSE).SetInterior(::GetSysColor(COLOR_BTNFACE)));
		//CDlgUtil::CtrlEnableDisable(this, arCircGroup, FALSE);

	 
	}
	else if(GetHalfType() == 2) // Poly
	{
		
	}
	
	else ASSERT(0);

}
void CCMSectItemPSCNormalGrid::ChangeBySect()
{
	// Normal 일때만

	if(IsHalfType()) return;
	SetStyleRange(CGXRange(D_INNER_START_ROW+7,4), CGXStyle()
			.SetEnabled(GetSellType()).SetInterior(::GetSysColor((GetSellType())?COLOR_WINDOW:COLOR_BTNFACE))); 
}

void CCMSectItemPSCNormalGrid::ChangeByType()
{

	 if(IsHalfType()) 
		 ChangeByCell();
	 else            
			ChangeBySect();
	
	ChangeByJoint();
}
int CCMSectItemPSCNormalGrid::GetSellType()
{
	CCMSectItemPSCNormal* m_pWndGrand=((CCMSectItemPSCNormal*)((CCMSectItemPSCNormalGridDlg*) m_pParent)->m_pWnd);
	return m_pWndGrand->GetSellType();
}
int CCMSectItemPSCNormalGrid::GetHalfType()
{
	CCMSectItemPSCNormal* m_pWndGrand=((CCMSectItemPSCNormal*)((CCMSectItemPSCNormalGridDlg*) m_pParent)->m_pWnd);
	return m_pWndGrand->GetHalfType();
}

int CCMSectItemPSCNormalGrid::IsHalfType()
{
	CCMSectItemPSCNormal* m_pWndGrand=((CCMSectItemPSCNormal*)((CCMSectItemPSCNormalGridDlg*) m_pParent)->m_pWnd);
	return m_pWndGrand->IsHalfType();
}

BOOL CCMSectItemPSCNormalGrid::OnStartEditing(ROWCOL nRow, ROWCOL nCol)
{
	if (!CGXGridWnd::OnStartEditing(nRow, nCol)) return FALSE;
	
	if(nRow>=D_JOINT_START_ROW && nRow<=D_JOINT_END_ROW)
	{
		m_bOnStartEditing = TRUE; 
		SaveData();
		OnSizeChange();
		m_bOnStartEditing = FALSE;
	}

	

	return TRUE;
}