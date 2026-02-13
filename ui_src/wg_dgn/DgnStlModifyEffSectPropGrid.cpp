// StrutTieCornerGrid.cpp : implementation file
//
#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnStlModifyEffSectPropGrid.h"
#include "DgnStlModifyEffSectPropDlg.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_DBLib.h"
#include "..\wg_db\wg_db_SectUtil.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


//////////////////////////////////////////////////////////////////////////
CDgnStlModifyEffSectPropULSGrid::CDgnStlModifyEffSectPropULSGrid(CDgnStlModifyEffSectPropDlg* pParent)
{
	m_pParent = pParent;
	VERIFY(m_pParent);
}

CDgnStlModifyEffSectPropULSGrid::~CDgnStlModifyEffSectPropULSGrid()
{
}


void CDgnStlModifyEffSectPropULSGrid::InitGrid()
{
	// 1. GXGridWnd 초기화
	CGXGridWnd::Initialize();

	// 2. Lock Update 시작
	StartGridUpdate();

	// 3. EditGrid 초기화
	//EnableMouseWheel(FALSE); // InitEditGrid 전에 수행해야 함.
	InitEditGrid(EN_EDT_GRID);

	// 4. row & column count
	SetRowCount(10);
	SetColCount(3);

//   CString strUnit;
//   if(nUnit!=D_UNITSYS_NONE)
//   {
//     CUnitCtrl::GetUnitSystem(nUnit, strUnit);
//   }

	// 5. row info 추가
	AddRowInfo_s_gen(1, _T("A_eff"),  EN_CEL_NONE, D_UNITSYS_BASE_AREA, 6);
	AddRowInfo_s_gen(2, _T("A_net"),  EN_CEL_NONE, D_UNITSYS_BASE_AREA, 6);
	AddRowInfo_s_gen(3, _T("Iy_eff"), EN_CEL_NONE, D_UNITSYS_BASE_STIF, 6);
	AddRowInfo_s_gen(4, _T("Iz_eff"), EN_CEL_NONE, D_UNITSYS_BASE_STIF, 6);
	AddRowInfo_s_gen(5, _T("Wy_eff"), EN_CEL_NONE, D_UNITSYS_BASE_VOLUME, 6);
	AddRowInfo_s_gen(6, _T("Wz_eff"), EN_CEL_NONE, D_UNITSYS_BASE_VOLUME, 6);
	AddRowInfo_s_gen(7, _T("It_eff"), EN_CEL_NONE, D_UNITSYS_BASE_STIF, 6);
	AddRowInfo_s_gen(8, _T("Iw_eff"), EN_CEL_NONE, D_UNITSYS_BASE_L6, 6);
	AddRowInfo_s_gen(9, _T("Cy_eff"), EN_CEL_NONE, D_UNITSYS_BASE_LENGTH, 6);
	AddRowInfo_s_gen(10, _T("Cz_eff"),  EN_CEL_NONE, D_UNITSYS_BASE_LENGTH, 6);
//  SetReadOnly(9, 2, 9, 2);

	// 6. column width
	int nWidth[] = { 60, 95, 50};
	SetColumnWidth(nWidth, 3);

	// 7. row height
	SetRowHeight(1, 10, 20);

	// 8. scroll bar 사용금지
	SetScrollBarMode(SB_BOTH, gxnDisabled);

	SetRightValidCell(1,1); 

	// 9. lock update 끝
	EndGridUpdate();
}


void CDgnStlModifyEffSectPropULSGrid::SetData(T_SECT_STIFFNESS_CF_BASE *pData)
{
	if (pData == NULL) { ASSERT(0); return; }

	StartGridUpdate();
	
	SetVal(1, 2, pData->dA_eff);
	SetVal(2, 2, pData->dA_net);
	SetVal(3, 2, pData->dIy_eff);
	SetVal(4, 2, pData->dIz_eff);
	SetVal(5, 2, pData->dWy_eff);
	SetVal(6, 2, pData->dWz_eff);
	SetVal(7, 2, pData->dIt_eff);
	SetVal(8, 2, pData->dIw_eff);
	SetVal(9, 2, pData->dRy_eff);
	SetVal(10, 2, pData->dRz_eff);

	EndGridUpdate();
}

BOOL CDgnStlModifyEffSectPropULSGrid::GetData(T_SECT_STIFFNESS_CF_BASE *pData)
{
	if (pData == NULL) { ASSERT(0); return FALSE; }

	//StartGridUpdate();

	GetVal(1, 2, pData->dA_eff);
	GetVal(2, 2, pData->dA_net);
	GetVal(3, 2, pData->dIy_eff);
	GetVal(4, 2, pData->dIz_eff);
	GetVal(5, 2, pData->dWy_eff);
	GetVal(6, 2, pData->dWz_eff);
	GetVal(7, 2, pData->dIt_eff);
	GetVal(8, 2, pData->dIw_eff);
	GetVal(9, 2, pData->dRy_eff);
	GetVal(10, 2, pData->dRz_eff);

	return TRUE;
}

void CDgnStlModifyEffSectPropULSGrid::OnModifyCell(ROWCOL nRow, ROWCOL nCol)
{
	CGXGridWnd::OnModifyCell(nRow, nCol);

	if(nCol!=2) return;

	//m_pParent->RedrawShape();
}

BOOL CDgnStlModifyEffSectPropULSGrid::OnEndEditing(ROWCOL nRow, ROWCOL nCol)
{
	CGXGridWnd::OnEndEditing(nRow, nCol);
	if(nCol!=2) return TRUE;

	//m_pParent->RedrawShape();

	return TRUE;
}

BOOL CDgnStlModifyEffSectPropULSGrid::OnPasteFromClipboard(const CGXRange &range)
{
	if(range.left!=2 || range.right!=2) // value cell이 아니면...
	{
		AfxMessageBox(_STR(_T("대상 범위 지정에 오류가 있습니다.")));//_LS(IDS_CMD_ERROR___OBJECT_RANGE));
		return FALSE;
	}

	CClassPaste CP;
	if(!CP.MakeDataFromClipBoard()) return FALSE;

	BOOL bFill = FALSE;
	CString strVal;
	T_CMAP_K<UINT> RC;
	int nCount = CP.m_mValue.GetCount();
	if(nCount<1) return FALSE;
	else if(nCount==1)
	{
		bFill = TRUE;
		RC.i1 = RC.i2 = 0;
		if(!CP.m_mValue.Lookup(RC, strVal)) ASSERT(0);
	}

	ROWCOL nMaxRow = 0;
	if(range.GetHeight()==1 && range.GetWidth()==1) // 대상범위가 1Cell일 경우
	{
		nMaxRow = (bFill) ? range.bottom : GetRowCount();
	}
	else
	{
		nMaxRow = range.bottom;
	}

	StartGridUpdate();
	for(ROWCOL nRow=range.top; nRow<nMaxRow+1; ++nRow)
	{
		if(!bFill)
		{
			RC.i1 = nRow-range.top;
			RC.i2 = 0;
			if(!CP.m_mValue.Lookup(RC, strVal)) continue;
		}
		if(!IsRowHidden(nRow))
		{
			SetVal(nRow, 2, strVal);
		}
	}
	EndGridUpdate();

	return FALSE; // 반드시 FALSE로 return해야한다.
}
//////////////////////////////////////////////////////////////////////////


CDgnStlModifyEffSectPropSLSGrid::CDgnStlModifyEffSectPropSLSGrid(CDgnStlModifyEffSectPropDlg* pParent)
{
	m_pParent = pParent;
	VERIFY(m_pParent);
}

CDgnStlModifyEffSectPropSLSGrid::~CDgnStlModifyEffSectPropSLSGrid()
{
}


void CDgnStlModifyEffSectPropSLSGrid::InitGrid()
{
	// 1. GXGridWnd 초기화
	CGXGridWnd::Initialize();

	// 2. Lock Update 시작
	StartGridUpdate();

	// 3. EditGrid 초기화
	//EnableMouseWheel(FALSE); // InitEditGrid 전에 수행해야 함.
	InitEditGrid(EN_EDT_GRID);

	// 4. row & column count
	SetRowCount(9);
	SetColCount(3);

	// 5. row info 추가
	AddRowInfo_s_gen(1, _T("A_eff"),  EN_CEL_NONE, D_UNITSYS_BASE_AREA, 6);
	AddRowInfo_s_gen(2, _T("A_net"),  EN_CEL_NONE, D_UNITSYS_BASE_AREA, 6);
	AddRowInfo_s_gen(3, _T("Iy_eff"), EN_CEL_NONE, D_UNITSYS_BASE_STIF, 6);
	AddRowInfo_s_gen(4, _T("Iz_eff"), EN_CEL_NONE, D_UNITSYS_BASE_STIF, 6);
	AddRowInfo_s_gen(5, _T("Wy_eff"), EN_CEL_NONE, D_UNITSYS_BASE_VOLUME, 6);
	AddRowInfo_s_gen(6, _T("Wz_eff"), EN_CEL_NONE, D_UNITSYS_BASE_VOLUME, 6);
	AddRowInfo_s_gen(7, _T("It_eff"), EN_CEL_NONE, D_UNITSYS_BASE_STIF, 6);
	AddRowInfo_s_gen(8, _T("Iw_eff"), EN_CEL_NONE, D_UNITSYS_BASE_L6, 6);
	AddRowInfo_s_gen(9, _T("Cy_eff"), EN_CEL_NONE, D_UNITSYS_BASE_LENGTH, 6);
	AddRowInfo_s_gen(10, _T("Cz_eff"),  EN_CEL_NONE, D_UNITSYS_BASE_LENGTH, 6);
	//SetReadOnly(9, 2, 9, 2);

	// 6. column width
	int nWidth[] = { 60, 80, 50};
	SetColumnWidth(nWidth, 3);

	// 7. row height
	SetRowHeight(1, 9, globalUtils.ScaleByDPI(20));

	// 8. scroll bar 사용금지
	SetScrollBarMode(SB_BOTH, gxnDisabled);

	SetRightValidCell(1,1); 

	// 9. lock update 끝
	EndGridUpdate();
}


void CDgnStlModifyEffSectPropSLSGrid::SetData(T_SECT_STIFFNESS_CF_BASE *pData)
{
	ASSERT(pData);

	StartGridUpdate();

	SetVal(1, 2, pData->dA_eff);
	SetVal(2, 2, pData->dA_net);
	SetVal(3, 2, pData->dIy_eff);
	SetVal(4, 2, pData->dIz_eff);
	SetVal(5, 2, pData->dWy_eff);
	SetVal(6, 2, pData->dWz_eff);
	SetVal(7, 2, pData->dIt_eff);
	SetVal(8, 2, pData->dIw_eff);
	SetVal(9, 2, pData->dRy_eff);
	SetVal(10, 2, pData->dRz_eff);

	EndGridUpdate();

}

BOOL CDgnStlModifyEffSectPropSLSGrid::GetData(T_SECT_STIFFNESS_CF_BASE *pData)
{
	ASSERT(pData);

	//StartGridUpdate();

	GetVal(1, 2, pData->dA_eff);
	GetVal(2, 2, pData->dA_net);
	GetVal(3, 2, pData->dIy_eff);
	GetVal(4, 2, pData->dIz_eff);
	GetVal(5, 2, pData->dWy_eff);
	GetVal(6, 2, pData->dWz_eff);
	GetVal(7, 2, pData->dIt_eff);
	GetVal(8, 2, pData->dIw_eff);
	GetVal(9, 2, pData->dRy_eff);
	GetVal(10, 2, pData->dRz_eff);
	
	return TRUE;
}

void CDgnStlModifyEffSectPropSLSGrid::OnModifyCell(ROWCOL nRow, ROWCOL nCol)
{
	CGXGridWnd::OnModifyCell(nRow, nCol);

	if(nCol!=2) return;

	//m_pParent->RedrawShape();
}

BOOL CDgnStlModifyEffSectPropSLSGrid::OnEndEditing(ROWCOL nRow, ROWCOL nCol)
{
	CGXGridWnd::OnEndEditing(nRow, nCol);
	if(nCol!=2) return TRUE;

	//m_pParent->RedrawShape();

	return TRUE;
}

BOOL CDgnStlModifyEffSectPropSLSGrid::OnPasteFromClipboard(const CGXRange &range)
{
	if(range.left!=2 || range.right!=2) // value cell이 아니면...
	{
		AfxMessageBox(_STR(_T("대상 범위 지정에 오류가 있습니다.")));//_LS(IDS_CMD_ERROR___OBJECT_RANGE));
		return FALSE;
	}

	CClassPaste CP;
	if(!CP.MakeDataFromClipBoard()) return FALSE;

	BOOL bFill = FALSE;
	CString strVal;
	T_CMAP_K<UINT> RC;
	int nCount = CP.m_mValue.GetCount();
	if(nCount<1) return FALSE;
	else if(nCount==1)
	{
		bFill = TRUE;
		RC.i1 = RC.i2 = 0;
		if(!CP.m_mValue.Lookup(RC, strVal)) ASSERT(0);
	}

	ROWCOL nMaxRow = 0;
	if(range.GetHeight()==1 && range.GetWidth()==1) // 대상범위가 1Cell일 경우
	{
		nMaxRow = (bFill) ? range.bottom : GetRowCount();
	}
	else
	{
		nMaxRow = range.bottom;
	}

	StartGridUpdate();
	for(ROWCOL nRow=range.top; nRow<nMaxRow+1; ++nRow)
	{
		if(!bFill)
		{
			RC.i1 = nRow-range.top;
			RC.i2 = 0;
			if(!CP.m_mValue.Lookup(RC, strVal)) continue;
		}
		if(!IsRowHidden(nRow))
		{
			SetVal(nRow, 2, strVal);
		}
	}
	EndGridUpdate();

	return FALSE; // 반드시 FALSE로 return해야한다.
}
//////////////////////////////////////////////////////////////////////////