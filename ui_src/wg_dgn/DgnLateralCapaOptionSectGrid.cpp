// CDgnLateralCapaOptionSectGrid.cpp : implementation file
//

#include "stdafx.h" 

#include "wg_dgn.h"
#include "DgnLateralCapaOptionSectGrid.h"

#include "..\wg_common\wg_common_TBCommon.h"
#include "..\wg_common\wg_common_TBGrid.h"
#include "..\wg_db\SectUtil.h"
#include "..\wg_db\DBDoc.h"
#include "..\wg_db\EditData.h"
#include "..\wg_base\SpecialChar.h"


#define D_COL_ID 	    1
#define D_COL_NAME 	  2
#define D_COL_SHAPE 	3
#define D_COL_REBAR 	4
#define D_COL_DIR 	  5
#define D_COL_D 	    6
#define D_COL_ALPHA 	7
#define D_COL_BETA 	  8
#define D_COL_015H 	  9
#define D_COL_NS 	    10
#define D_COL_D2 	    11
#define D_COL_ANG 	  12
#define D_COL_NUM 	  12

CDgnLateralCapaOptionSectGrid::CDgnLateralCapaOptionSectGrid()
{
}

CDgnLateralCapaOptionSectGrid::~CDgnLateralCapaOptionSectGrid()
{
}

///////////////////////////////////////////////////////////////////////////////
// User Defined Function
void CDgnLateralCapaOptionSectGrid::SetData(CArray<T_DGN_SECT_DATA, T_DGN_SECT_DATA&>& aData)
{
	BOOL bOldLock = LockUpdate(TRUE);

	m_setRedFont.clear();

	int nSize = aData.GetSize();

	if (nSize < 0) return;
	SetRowCount(0); // Cell Merge 정보를 삭제하기 위해서 0으로 먼저 세팅 함
	nSize *= 2;
	SetRowCount(nSize + 1);
	HideRows(nSize + 1, nSize + 1);

	// Merge Cell  
	for (int i = 0; i < nSize; i += 2)
	{
		ROWCOL nRow = i + 1;
		SetCoveredCellsRowCol(nRow, D_COL_ID, nRow + 1, D_COL_ID);
		SetCoveredCellsRowCol(nRow, D_COL_NAME, nRow + 1, D_COL_NAME);
		SetCoveredCellsRowCol(nRow, D_COL_SHAPE, nRow + 1, D_COL_SHAPE);
		SetCoveredCellsRowCol(nRow, D_COL_REBAR, nRow + 1, D_COL_REBAR);
	}

	// Set Data 
	for (int i = 0; i < nSize; ++i)
	{
		CStringArray aValue;
		aValue.SetSize(D_COL_NUM);

		int nDataIdx = i / 2;

		if (i % 2 == 0)
		{
			aValue[D_COL_ID - 1].Format(_T("%d"), aData[nDataIdx].SectK);
			aValue[D_COL_NAME - 1].Format(_T("%s"), aData[nDataIdx].strName);
			//aValue[D_COL_SHAPE  - 1].Format(_T("%d"), aData[nDataIdx].nID);
			aValue[D_COL_REBAR - 1] = aData[nDataIdx].bRebar ? _T("O") : _T("X");
			aValue[D_COL_DIR - 1] = _T("y");
			aValue[D_COL_D - 1].Format(_T("%g"), aData[nDataIdx].MpstD.dDim);
			aValue[D_COL_ALPHA - 1].Format(_T("%g"), aData[nDataIdx].MpstD.dAlpha);
			aValue[D_COL_BETA - 1].Format(_T("%g"), aData[nDataIdx].MpstD.dBeta);
			aValue[D_COL_015H - 1].Format(_T("%g"), aData[nDataIdx].MpstD.dHingeLimY);
			aValue[D_COL_NS - 1].Format(_T("%d"), aData[nDataIdx].MpstD.nnsY);
			aValue[D_COL_D2 - 1].Format(_T("%g"), aData[nDataIdx].MpstD.dd2Y);
			aValue[D_COL_ANG - 1].Format(_T("%g"), aData[nDataIdx].MpstD.dAngY);
		}
		else
		{
			// Merge 된 것도 같은값으로 setting해도 문제 없음..
			aValue[D_COL_ID - 1].Format(_T("%d"), aData[nDataIdx].SectK);
			aValue[D_COL_NAME - 1].Format(_T("%s"), aData[nDataIdx].strName);
			//aValue[D_COL_SHAPE  - 1].Format(_T("%d"), aData[nDataIdx].nID);
			aValue[D_COL_REBAR - 1] = aData[nDataIdx].bRebar ? _T("O") : _T("X");
			aValue[D_COL_DIR - 1] = _T("z");
			aValue[D_COL_D - 1].Format(_T("%g"), aData[nDataIdx].MpstD.dDimz);
			aValue[D_COL_ALPHA - 1].Format(_T("%g"), aData[nDataIdx].MpstD.dAlphaZ);
			aValue[D_COL_BETA - 1].Format(_T("%g"), aData[nDataIdx].MpstD.dBetaZ);
			aValue[D_COL_015H - 1].Format(_T("%g"), aData[nDataIdx].MpstD.dHingeLimZ);
			aValue[D_COL_NS - 1].Format(_T("%d"), aData[nDataIdx].MpstD.nnsZ);
			aValue[D_COL_D2 - 1].Format(_T("%g"), aData[nDataIdx].MpstD.dd2Z);
			aValue[D_COL_ANG - 1].Format(_T("%g"), aData[nDataIdx].MpstD.dAngZ);
		}

		SetDataToRow(i + 1, aValue);

		if (i % 2 == 0)
		{
			SetShapeBitmap(i + 1, aData[nDataIdx]);
		}

		SetSectionDataEdit(i + 1, aData[nDataIdx]);
	}

	SetRowHeightByDPI(1, nSize, 18);

	LockUpdate(bOldLock);
	if (!bOldLock) Redraw();
}

int CDgnLateralCapaOptionSectGrid::GetPosVal(CString strPos)
{
	if(strPos==_LS(IDS_CMD_MPHI_NONE))
		return 0;
	else if(strPos==_LS(IDS_CMD_MPHI_SELECT_I))
		return 1;
	else if(strPos==_LS(IDS_CMD_MPHI_SELECT_M))
		return 2;
	else if(strPos==_LS(IDS_CMD_MPHI_SELECT_J))
		return 3;
	else
	{
		ASSERT(0);
		return 0;
	}
}

CString CDgnLateralCapaOptionSectGrid::GetPosString(const T_DGN_SECT_DATA& data)
{
	CString strReturn;
	if(!data.bCandidate)
	{
		strReturn=_LS(IDS_CMD_MPHI_NONE);	
		return strReturn;
	}
	switch(data.MpstD.nPosition)
	{
	case 0: strReturn=_LS(IDS_CMD_MPHI_NONE);	break;
	case 1: strReturn=_LS(IDS_CMD_MPHI_SELECT_I);	break;
	case 2: strReturn=_LS(IDS_CMD_MPHI_SELECT_M);	break;
	case 3: strReturn=_LS(IDS_CMD_MPHI_SELECT_J);	break;
	}
	return strReturn;
}

BOOL CDgnLateralCapaOptionSectGrid::GetData(CArray<T_DGN_SECT_DATA, T_DGN_SECT_DATA&>& aData)
{
	//aData.RemoveAll();

	int nCount = GetRowCount();
	if (nCount == 0) return TRUE;

	//aData.SetSize(nCount);

	CStringArray aValue;
	aValue.SetSize(D_COL_NUM);

	ASSERT(aData.GetSize() * 2 + 1 == nCount);// 뭔가 꼬인듯 가변Size 인데 이렇게 되면 안됨..

	for (int i = 0; i < nCount - 1; ++i)
	{
		if (!GetRowToData(i + 1, aValue)) return FALSE;

		int nDataIdx = i / 2;

		if (i % 2 == 0)
		{
			aData[nDataIdx].SectK = _ttoi(aValue[D_COL_ID - 1]);
			//aData[nDataIdx].               = _ttoi(aValue[D_COL_NAME   - 1]); // 안고침
			//aData[nDataIdx].               = _ttoi(aValue[D_COL_SHAPE  - 1]); // 안고침
			//aData[nDataIdx].               = _ttoi(aValue[D_COL_REBAR  - 1]); // 안고침      
			//aData[nDataIdx].               = _ttoi(aValue[D_COL_DIR    - 1]); // 안고침
			aData[nDataIdx].MpstD.dDim = _tstof(aValue[D_COL_D - 1]);
			aData[nDataIdx].MpstD.dAlpha = _tstof(aValue[D_COL_ALPHA - 1]);
			aData[nDataIdx].MpstD.dBeta = _tstof(aValue[D_COL_BETA - 1]);
			aData[nDataIdx].MpstD.dHingeLimY = _tstof(aValue[D_COL_015H - 1]);
			aData[nDataIdx].MpstD.nnsY = _ttoi(aValue[D_COL_NS - 1]);
			aData[nDataIdx].MpstD.dd2Y = _tstof(aValue[D_COL_D2 - 1]);
			aData[nDataIdx].MpstD.dAngY = _tstof(aValue[D_COL_ANG - 1]);
		}
		else
		{
			//aData[nDataIdx].SectK            = _ttoi(aValue[D_COL_ID     - 1]);
			//aData[nDataIdx].               = _ttoi(aValue[D_COL_NAME   - 1]); // 안고침
			//aData[nDataIdx].               = _ttoi(aValue[D_COL_SHAPE  - 1]); // 안고침
			//aData[nDataIdx].               = _ttoi(aValue[D_COL_REBAR  - 1]); // 안고침      
			//aData[nDataIdx].               = _ttoi(aValue[D_COL_DIR    - 1]); // 안고침
			aData[nDataIdx].MpstD.dDimz = _tstof(aValue[D_COL_D - 1]);
			aData[nDataIdx].MpstD.dAlphaZ = _tstof(aValue[D_COL_ALPHA - 1]);
			aData[nDataIdx].MpstD.dBetaZ = _tstof(aValue[D_COL_BETA - 1]);
			aData[nDataIdx].MpstD.dHingeLimZ = _tstof(aValue[D_COL_015H - 1]);
			aData[nDataIdx].MpstD.nnsZ = _ttoi(aValue[D_COL_NS - 1]);
			aData[nDataIdx].MpstD.dd2Z = _tstof(aValue[D_COL_D2 - 1]);
			aData[nDataIdx].MpstD.dAngZ = _tstof(aValue[D_COL_ANG - 1]);
		}
	}
	return TRUE;
}

void CDgnLateralCapaOptionSectGrid::SetDataToRow(ROWCOL nRow, CStringArray& raValue)
{
	int nColCount = GetColCount();
	ASSERT(nColCount == raValue.GetSize());

	for(int i=0; i<nColCount; ++i)
	{
		SetValueRange(CGXRange(nRow, i+1), raValue[i]);
	}
}

BOOL CDgnLateralCapaOptionSectGrid::GetRowToData(ROWCOL nRow, CStringArray& raValue)
{
	int nColCount = GetColCount();
	ASSERT(nColCount == raValue.GetSize());

	for(int i=0; i<nColCount; ++i)
		raValue[i] = GetEditingValue(nRow, i+1);

	return TRUE;
}

CString CDgnLateralCapaOptionSectGrid::GetEditingValue(ROWCOL nRow, ROWCOL nCol)
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

BEGIN_MESSAGE_MAP(CDgnLateralCapaOptionSectGrid, CTBBrowserWnd)
	//{{AFX_MSG_MAP(CDgnLateralCapaOptionSectGrid)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDgnLateralCapaOptionSectGrid message handlers

void CDgnLateralCapaOptionSectGrid::Initialize(int nCode)
{
	CTBBrowserWnd::Initialize();
	m_nCode = nCode;
	// cell 바깥영역 배경색 지정 
	GetParam()->GetProperties()->SetColor(GX_COLOR_BACKGROUND, CTBGrid::m_Color.OuterBackground);

	// Header 배경색 지정 
	ChangeColHeaderStyle(CGXStyle().SetInterior(CTBGrid::m_Color.RowColHeaderBg)); 
	ChangeRowHeaderStyle(CGXStyle().SetInterior(CTBGrid::m_Color.RowColHeaderBg)); 

	// 기타 Cells 배경색 지정
	ChangeStandardStyle(CGXStyle( ).SetInterior(CTBGrid::m_Color.NormalModeBg));

	LockUpdate(TRUE);

	SetColCount(D_COL_NUM);
	
	// Header
	SetFrozenCols(0, 0);

	// 표준 스타일 변경
	ChangeStandardStyle(CGXStyle()
		.SetFont(CGXFont_GC().SetSize(9))
		.SetVerticalAlignment(DT_VCENTER)
		.SetAllowEnter(FALSE));

	// 컬럼 헤더 스타일 변경
	ChangeColHeaderStyle(CGXStyle()
		.SetFont(CGXFont_GC().SetBold(FALSE)));

		// 컬럼 스타일 변경
	SetStyleRange(CGXRange().SetCols(D_COL_ID, D_COL_REBAR), CGXStyle()
			.SetControl(GX_IDS_CTRL_STATIC)
			.SetEnabled(FALSE));

	SetStyleRange(CGXRange().SetCols(D_COL_SHAPE, D_COL_REBAR), CGXStyle()
			.SetHorizontalAlignment(DT_CENTER));
	
	SetStyleRange(CGXRange().SetCols(D_COL_DIR), CGXStyle()
		.SetControl(GX_IDS_CTRL_STATIC)
		.SetEnabled(FALSE));

	SetStyleRange(CGXRange().SetCols(D_COL_D, D_COL_ANG), CGXStyle()
			.SetHorizontalAlignment(DT_RIGHT)
			.SetValueType(GX_VT_NUMERIC)
			.SetEnabled(TRUE));
		
	EnableDeleteRowByPressingKey(FALSE);
	EnableInsertRowByPressingKey(FALSE);
	EnableAppendRow(FALSE);

	SetRowHeightByDPI(0, 0, 20);

	// undo를 금지시킨다.
	GetParam()->EnableUndo(FALSE);  

	// cell의 width와 height 크기 변경을 금지한다.
	GetParam()->EnableTrackRowHeight(FALSE);
	//GetParam()->EnableTrackColWidth(FALSE);

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

	// always show vertical scrollbar
	SetScrollBarMode(SB_VERT, gxnEnabled | gxnEnhanced);

	CGXProperties* pProp = GetParam()->GetProperties();
	pProp->SetMarkColHeader(FALSE); // Turn off pressed button effect for column headers
	pProp->SetMarkRowHeader(FALSE);
	pProp->SetUserProperty(GX_IDS_OUTLINECURRENTCELL, // 현재 셀의 border 설정 변경
		(CGXStyle) pProp->sInvertNoBorder);
	
	CArray<int,int> aWidth;
	aWidth.SetSize(D_COL_NUM);	
	aWidth[D_COL_ID 	 - 1] = 22;
	aWidth[D_COL_NAME  - 1] = 102;
	aWidth[D_COL_SHAPE - 1] = 46;
	aWidth[D_COL_REBAR - 1] = 45;  
	aWidth[D_COL_DIR 	 - 1] = 40;
	aWidth[D_COL_D     - 1] = 40;
	aWidth[D_COL_ALPHA - 1] = 40;
	aWidth[D_COL_BETA  - 1] = 40;
	aWidth[D_COL_015H  - 1] = 40;
	aWidth[D_COL_NS 	 - 1] = 40;
	aWidth[D_COL_D2 	 - 1] = 40;
	aWidth[D_COL_ANG   - 1] = 40;
	
	SetColWidthByDPI(0, 0, 20);
	for (int i = 0; i < D_COL_NUM; i++)
	{
		SetColWidthByDPI(i + 1, i + 1, aWidth[i]);
	}
	
	SetValueRange(CGXRange(0, D_COL_ID), _LS(IDS_CMD_MPHI_ID));
	SetValueRange(CGXRange(0, D_COL_NAME), _LS(IDS_CMD_MPHI_NAME));
	SetValueRange(CGXRange(0, D_COL_SHAPE), _LS(IDS_CMD_MPHI_SHAPE));
	SetValueRange(CGXRange(0, D_COL_REBAR), _LS(IDS_CMD_MPHI_REBAR));  
	SetValueRange(CGXRange(0, D_COL_DIR), _LS(IDS_CMD_MPHI_DIR));
	SetValueRange(CGXRange(0, D_COL_D), _LS(IDS_CMD_MPHI_D));
	SetValueRange(CGXRange(0, D_COL_ALPHA), D_CH_ALPHA);
	SetValueRange(CGXRange(0, D_COL_BETA), D_CH_BETA);
	SetValueRange(CGXRange(0, D_COL_015H), _LS(IDS_CMD_MPHI_015H));
	SetValueRange(CGXRange(0, D_COL_NS), _LS(IDS_CMD_MPHI_NS));
	SetValueRange(CGXRange(0, D_COL_D2), _LS(IDS_CMD_MPHI_D2));
	SetValueRange(CGXRange(0, D_COL_ANG), _LS(IDS_CMD_MPHI_ANG));  

	LockUpdate(FALSE);
}

void CDgnLateralCapaOptionSectGrid::SetSectionDataEdit(ROWCOL nRow, T_DGN_SECT_DATA& data)
{
	// bRCSect와 bStlSrcSect는 단면과 Material만 봤을때 절대 지원할 일이 없는 것만 Disable되도록 처리함... 즉 beam과 column중 한 곳에서라도 쓰이면 enable임..
	BOOL bRCSect, bStlSect, bSrcSect;
	CDBDoc* pDoc;
	pDoc = CDBDoc::GetDocPoint();

	BOOL bCol, bBeam, bBrace;
	bCol = bBeam = bBrace = FALSE;
	pDoc->m_pEditData->GetMemberType4Mpst(data.SectK, bCol, bBeam, bBrace);

	pDoc->m_pEditData->IsValidSectType4MpstEachMatl(data.strShape, data.nStype, bCol, bBeam, bRCSect, bStlSect, bSrcSect);
	
	if(nRow % 2 == 1) // y축
	{
		// Tappered 단면은 Beam일 때만 배근을 할 수 있는데... Node Elem Relation처리는 안하므로.. Column, Beam 중 한 군대 에서라도 가능한 단면은 enable 처리 함..
		// 참고로 Beam일 경우엔 PSC단면이 Enable임..
		 
		// RC
		SetStyleRange(CGXRange().SetCells(nRow, D_COL_D), CGXStyle().SetEnabled(data.bRC && bRCSect && data.bCandidate && bCol));
		SetStyleRange(CGXRange().SetCells(nRow, D_COL_ALPHA), CGXStyle().SetEnabled(data.bRC && bRCSect && data.bCandidate && bCol));
		SetStyleRange(CGXRange().SetCells(nRow, D_COL_BETA), CGXStyle().SetEnabled(data.bRC && bRCSect && data.bCandidate && bCol));
		SetStyleRange(CGXRange().SetCells(nRow, D_COL_015H), CGXStyle().SetEnabled(data.bRC && bRCSect && data.bCandidate && bCol && m_nCode == 4));
		SetStyleRange(CGXRange().SetCells(nRow, D_COL_NS), CGXStyle().SetEnabled(data.bRC && bRCSect && data.bCandidate && bCol && m_nCode == 4));
		SetStyleRange(CGXRange().SetCells(nRow, D_COL_D2), CGXStyle().SetEnabled(data.bRC && bRCSect && data.bCandidate && bCol && m_nCode == 4));
		SetStyleRange(CGXRange().SetCells(nRow, D_COL_ANG), CGXStyle().SetEnabled(data.bRC && bRCSect && data.bCandidate && bCol));        
	}
	else // z축
	{
		// RC
			SetStyleRange(CGXRange().SetCells(nRow, D_COL_D), CGXStyle().SetEnabled(data.bRC && bRCSect && data.bCandidate && bCol && !CDbToolMphi::IsSameDZDir(data.strShape)));
			SetStyleRange(CGXRange().SetCells(nRow, D_COL_ALPHA), CGXStyle().SetEnabled(data.bRC && bRCSect && data.bCandidate && bCol && !CDbToolMphi::IsSameAlphaZDir(data.strShape, m_nCode)));
			SetStyleRange(CGXRange().SetCells(nRow, D_COL_BETA), CGXStyle().SetEnabled(data.bRC && bRCSect && data.bCandidate && bCol && !CDbToolMphi::IsSameBetaZDir(data.strShape, m_nCode)));
			SetStyleRange(CGXRange().SetCells(nRow, D_COL_015H), CGXStyle().SetEnabled(data.bRC && bRCSect && data.bCandidate && bCol && m_nCode == 4));
			SetStyleRange(CGXRange().SetCells(nRow, D_COL_NS), CGXStyle().SetEnabled(data.bRC && bRCSect && data.bCandidate && bCol  && m_nCode == 4 && !CDbToolMphi::IsSameNsZDir(data.strShape)));
			SetStyleRange(CGXRange().SetCells(nRow, D_COL_D2), CGXStyle().SetEnabled(data.bRC && bRCSect && data.bCandidate && bCol  && m_nCode == 4 && !CDbToolMphi::IsSameD2ZDir(data.strShape)));
			SetStyleRange(CGXRange().SetCells(nRow, D_COL_ANG), CGXStyle().SetEnabled(data.bRC && bRCSect && data.bCandidate && bCol));
	}
	
	CGXStyle style;	
	for (int nCol = D_COL_D; nCol <= D_COL_ANG; nCol++)
	{ 
		GetStyleRowCol(nRow, nCol, style);
		if(style.GetIncludeEnabled() && !style.GetEnabled())
		{
			SetValueRange(CGXRange(nRow, nCol), _T(""));  // Disable은 공백으로 표시
		}
	}

	/*
	if(m_setRedFont.size() > 0)
	{
		for(auto pairRowCol : m_setRedFont)
		{
			CGXStyle style;	
			GetStyleRowCol(pairRowCol.first, pairRowCol.second, style);
			SetStyleRange(CGXRange().SetCells(pairRowCol.first, pairRowCol.second), style);
		}
	}
	*/
	//BOOL bStiffened = (data.strShape == D_SECT_SHAPE_REG_BSTF || data.strShape == D_SECT_SHAPE_REG_PSTF
	//  || data.strShape == D_SECT_SHAPE_SRC_BSTF || data.strShape == D_SECT_SHAPE_SRC_PSTF);    
}

BOOL CDgnLateralCapaOptionSectGrid::OnStartEditing(ROWCOL nRow, ROWCOL nCol)
{
	return CTBBrowserWnd::OnStartEditing(nRow, nCol);
}

void CDgnLateralCapaOptionSectGrid::OnModifyCell(ROWCOL nRow, ROWCOL nCol)
{
	CTBBrowserWnd::OnModifyCell(nRow, nCol);
}

BOOL CDgnLateralCapaOptionSectGrid::OnEndEditing(ROWCOL nRow, ROWCOL nCol)
{
	return CTBBrowserWnd::OnEndEditing(nRow, nCol);
}

BOOL CDgnLateralCapaOptionSectGrid::ProcessKeys(CWnd *pSender, UINT nMessage, UINT nChar, UINT nRepCnt, UINT flags)
{
	CRowColArray awRows;
	ROWCOL nRow=1, nCol=1;

	if(!GetCurrentCell(nRow, nCol)) // if no current cell, default action
		return CTBBrowserWnd::ProcessKeys(pSender, nMessage, nChar, nRepCnt, flags);

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
		default: 
			break;
		}
	}

	return CTBBrowserWnd::ProcessKeys(pSender, nMessage, nChar, nRepCnt, flags); 

GT_PROC_RETURN:
	if(nMessage==WM_KEYDOWN && (nChar==VK_TAB || nChar==VK_RETURN || nChar==VK_RIGHT || nChar==VK_LEFT))
	{
		Redraw();    
	}

	return TRUE;
}

BOOL CDgnLateralCapaOptionSectGrid::SetRightValidCell(ROWCOL nRow, ROWCOL nCol)
{
	long i=nRow, j=nCol+1; // 현재 행 다음 열
	int nColCount = GetColCount();
	if(j>nColCount) { ++i; j=1; }

	BOOL bCovered;
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
		++i; j=1;
	}
	return FALSE;
}

BOOL CDgnLateralCapaOptionSectGrid::SetLeftValidCell(ROWCOL nRow, ROWCOL nCol)
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

BOOL CDgnLateralCapaOptionSectGrid::CopyRange(const CGXRangeList& selList)
{
	CGXRangeList mSelList;
	CGXRange* mRange;
	POSITION position=selList.GetHeadPosition();

	while(TRUE)
	{
		if(!position||position<0)
			break;

		mRange=new CGXRange();
		if(mRange->IntersectRange(CGXRange(1,1,GetRowCount(),GetColCount()),selList.GetAt(position)))
		{
			mSelList.AddTail(mRange);
		}
		else
			delete mRange;

		selList.GetNext(position);
	}

	return CTBBrowserWnd::CopyRange(mSelList);
}

BOOL CDgnLateralCapaOptionSectGrid::OnPasteFromClipboard(const CGXRange &range)
{
	OpenClipboard(); // open clipboard
	HGLOBAL handle = GetClipboardData(CF_UNICODETEXT);
	LPTSTR psz = (LPTSTR)GlobalLock(handle);
	TCHAR* pTChar = (TCHAR*)psz;
	DWORD size = (DWORD)GlobalSize(handle);
	GlobalUnlock(handle);
	CloseClipboard(); // close the clipboard

	CHAR	strMultibyte[1024] = {0,};
	WideCharToMultiByte(CP_ACP, 0, (LPCWSTR)psz, -1, strMultibyte, 1024 ,NULL ,NULL);

	CString  sub = _T(""), szData = strMultibyte;
	LPTSTR   buf = szData.GetBuffer(1);
	int      pos = 0, endpos, len = szData.GetLength();
	TCHAR    endTCHAR = L' ';
	int		 col = range.left;
	int  	 row = range.top;	

	BOOL bPasteToAllRange = IsPasteToAllRange(range, szData);

	BOOL bOldLock = LockUpdate(TRUE);

	BOOL bFirstDone = FALSE;
	CString szSubFirst = _T("");
	BOOL bEnable;
	CGXStyle style;	
	while(pos < len)
	{
		if(buf[pos] == _T('\t') || buf[pos] == _T('\r') || buf[pos] == _T('\n'))
		{
			endTCHAR = buf[pos];
			endpos = pos;
		}		
		else // find the end of the item then copy the item to the cell
		{
			endpos = pos + 1;
			while(endpos < len)
			{
				endTCHAR = buf[endpos];
				if(endTCHAR == _T('\n') || endTCHAR == _T('\r') || endTCHAR == _T('\t')) break;
				endpos++;
			}

			if(col <= (int)GetColCount() && row <= (int)GetRowCount())
			{	
				sub = szData.Mid(pos, endpos - pos);
				if(bFirstDone == FALSE)
				{
					szSubFirst = sub;
				}
				
				GetStyleRowCol(row, col, style);
				bEnable = !style.GetIncludeEnabled() || style.GetEnabled();
				if (style.GetIncludeReadOnly() && style.GetReadOnly()) // 셀에 readonly정보 포함 여부 알아냄
				{
					bEnable = FALSE;
				}
				if(bEnable)
				{
					SetValueRange(CGXRange(row, col), sub);
				}
			}
		}

		if(endTCHAR == _T('\t')) col++;
		if(endTCHAR == _T('\r') || endTCHAR == _T('\n'))
		{
			col = range.left;
			row++;
			if(buf[endpos] == _T('\r') && buf[endpos + 1] == _T('\n')) endpos++;
		}
		pos = endpos + 1;
	}

	if(bPasteToAllRange && !szSubFirst.IsEmpty())
	{
		while(row <= (int)range.bottom)
		{
			GetStyleRowCol(row, col, style);
			bEnable = !style.GetIncludeEnabled() || style.GetEnabled();
			if (style.GetIncludeReadOnly() && style.GetReadOnly()) // 셀에 readonly정보 포함 여부 알아냄
			{
				bEnable = FALSE;
			}
			if(bEnable)
			{
				SetValueRange(CGXRange(row, col), szSubFirst);
			}

			row++;
		}
	}

	szData.ReleaseBuffer();

	LockUpdate(bOldLock);
	if (!bOldLock) Redraw();

	return TRUE;	
}

BOOL CDgnLateralCapaOptionSectGrid::IsPasteToAllRange(const CGXRange &range, CString szData) // copy한 Cell이 1개 이고, 동일한 행의 cell 여러개로 붙여넣은 상황일 때 TRUE.. 이 경우엔 붙여넣을 Range의 모든 Cell에 복사한 Cell의 값을 붙여 넣어 줘야 함
{
	// 붙여넣을 Cell이 모두 동일한 행에 있는지 확인
	if(szData.Find(_T('\t')) != -1 || szData.Find(_T('\r')) != -1 || szData.Find(_T('\n')) != -1 )
	{
		return FALSE;
	}

	// 복사한 cell이 하나인지 확인
	if(range.left != range.right)
	{
		return FALSE;
	}

	return TRUE;
}

void CDgnLateralCapaOptionSectGrid::SetShapeBitmap(ROWCOL nRow, T_DGN_SECT_DATA data)
{	  
	UINT aRegBitmapID[] = {    
		IDB_DGN_SEC_REG_01, IDB_DGN_SEC_REG_02, IDB_DGN_SEC_REG_03,
		IDB_DGN_SEC_REG_04, IDB_DGN_SEC_REG_05, IDB_DGN_SEC_REG_06,
		IDB_DGN_SEC_REG_07, IDB_DGN_SEC_REG_08, IDB_DGN_SEC_REG_09,
		IDB_DGN_SEC_REG_10, IDB_DGN_SEC_REG_11, IDB_DGN_SEC_REG_12,
		IDB_DGN_SEC_REG_13, IDB_DGN_SEC_REG_14, IDB_DGN_SEC_REG_15,
		IDB_DGN_SEC_REG_16, IDB_DGN_SEC_REG_17, IDB_DGN_SEC_REG_18,
		/*IDB_DGN_SEC_REG_19, IDB_DGN_SEC_REG_20,*/ IDB_DGN_SEC_REG_21,
		IDB_DGN_SEC_REG_22, IDB_DGN_SEC_REG_23, IDB_DGN_SEC_REG_24
	};

	UINT aPscBitmapID[] = {  
		IDB_DGN_SEC_PSC_1C,	 IDB_DGN_SEC_PSC_2C,	IDB_DGN_SEC_PSC_3C,
		IDB_DGN_SEC_PSC_4C,	 IDB_DGN_SEC_PSC_5C,	IDB_DGN_SEC_PSC_6C,
		IDB_DGN_SEC_PSC_7C,	 IDB_DGN_SEC_PSC_8C,	IDB_DGN_SEC_PSC_9C,
		IDB_DGN_SEC_PSC_10C, IDB_DGN_SEC_PSC_11C,	IDB_DGN_SEC_PSC_12C
	};

	//UINT aSRCBitmapID[] = { 
	//  IDB_CMD_SEC_SRC_01, IDB_CMD_SEC_SRC_02, IDB_CMD_SEC_SRC_03,
	//  IDB_CMD_SEC_SRC_04, IDB_CMD_SEC_SRC_05, IDB_CMD_SEC_SRC_06,
	//  IDB_CMD_SEC_SRC_07, IDB_CMD_SEC_SRC_08, IDB_CMD_SEC_SRC_09,
	//  IDB_CMD_SEC_SRC_10, IDB_CMD_SEC_SRC_11, IDB_CMD_SEC_SRC_12,
	//  IDB_CMD_SEC_SRC_13, IDB_CMD_SEC_SRC_15, IDB_CMD_SEC_SRC_16,
	//  IDB_CMD_SEC_SRC_17
	//};

	int nIndex;
	UINT nBitmapIndex;
	if(data.nStype==D_SECT_TYPE_REGULAR || data.nStype==D_SECT_TYPE_USER)
	{
		nIndex = CSectUtil::GetShapeIndexFromNameReg(data.strShape);
		nBitmapIndex = aRegBitmapID[nIndex];
	}
	else if(data.nStype==D_SECT_TYPE_PSC)
	{
		nIndex = CSectUtil::GetShapeIndexFromNamePsc(data.strShape);	
		nBitmapIndex = aPscBitmapID[nIndex];
	}
	//else if(data.nStype==D_SECT_TYPE_SRC)
	//{
	//	nIndex = CSectUtil::GetShapeIndexFromNameSrc(data.strShape);	
	//	nBitmapIndex = aSRCBitmapID[nIndex];
	//}
	else if(data.nStype == D_SECT_TYPE_TAPERED)
	{
		if(data.strShape == D_SECT_SHAPE_REG_B ||
			data.strShape == D_SECT_SHAPE_REG_P ||
			data.strShape == D_SECT_SHAPE_REG_SB ||
			data.strShape == D_SECT_SHAPE_REG_SR ||
			data.strShape == D_SECT_SHAPE_REG_OCT ||
			data.strShape == D_SECT_SHAPE_REG_SOCT ||
			data.strShape == D_SECT_SHAPE_REG_TRK ||
			data.strShape == D_SECT_SHAPE_REG_STRK ||
			data.strShape == D_SECT_SHAPE_REG_BSTF ||
			data.strShape == D_SECT_SHAPE_REG_PSTF ||
			data.strShape == D_SECT_SHAPE_REG_GEN)
		{
			nIndex = CSectUtil::GetShapeIndexFromNameReg(data.strShape);
			nBitmapIndex = aRegBitmapID[nIndex];
		}
		else if(
			 data.strShape == D_SECT_SHAPE_PSC_1CELL ||
			 data.strShape == D_SECT_SHAPE_PSC_2CELL ||
			 data.strShape == D_SECT_SHAPE_PSC_3CELL ||
			 data.strShape == D_SECT_SHAPE_PSC_4CELL ||
			 data.strShape == D_SECT_SHAPE_PSC_MID   ||
			 data.strShape == D_SECT_SHAPE_PSC_HALF  ||
			 data.strShape == D_SECT_SHAPE_PSC_T     ||
			 data.strShape == D_SECT_SHAPE_PSC_BOX   ||
			 data.strShape == D_SECT_SHAPE_PSC_I     ||
			 data.strShape == D_SECT_SHAPE_PSC_NCEL2 ||
			 data.strShape == D_SECT_SHAPE_PSC_VALUE ||
			 data.strShape == D_SECT_SHAPE_PSC_CMPWEB)
		{
			nIndex = CSectUtil::GetShapeIndexFromNamePsc(data.strShape);	
			nBitmapIndex = aPscBitmapID[nIndex];
		}

	}

	CString csBmp;
	csBmp.Format(_LSX(#BMP(%d)), nBitmapIndex);
	SetStyleRange(CGXRange(nRow, D_COL_SHAPE), CGXStyle()
		.SetControl(GX_IDS_CTRL_STATIC).SetValue(csBmp)								
		.SetVerticalAlignment(DT_VCENTER)
		.SetHorizontalAlignment(DT_CENTER)
		);


		
}

BOOL CDgnLateralCapaOptionSectGrid::GetStyleRowCol(ROWCOL nRow, ROWCOL nCol, CGXStyle& style, GXModifyType mt, int nType)
{
	// 이 Table은 Readonly의 색을 일반Cell과 동일하게 하자...
	BOOL bReadOnlyMode;
	BOOL bCurrentRow;
	BOOL bCurrentCell;
	BOOL bEnabled;
	BOOL bReadOnly;
	BOOL bAppendRow;
	BOOL bHeaderCol;

	ROWCOL ncRow, ncCol;    // 현재 Row, Col
	CGXGridWnd::GetStyleRowCol(nRow, nCol, style, mt, nType);

	// 현재 셀의 그리드 좌표(Row, Col 번호)를 얻어온다.
	if (GetCurrentCell(ncRow, ncCol)) GetBrowseParam()->m_nCurrentRow = ncRow;
	else ncRow = GetRowCount()+1; // 현재 셀이 없는 경우 : ncRow를 범위 밖으로 설정

	// 현재 셀이 이동되면 새로 그려준다.
	m_bRefreshOnSetCurrentCell = TRUE;

	// Row나 Col이 1보다 작으면 리턴한다.(Standard Row or Column Header)
	if (nRow < 1 || nCol < 1) return TRUE;

	// Get Current Table and Cell State
	bReadOnlyMode = IsLockEdit();
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
	else // Column style에서 enable 정보 알아냄
	{
		CGXStyle styleEx;
		GetColStyle(nCol, styleEx);
		bEnabled = (!styleEx.GetIncludeEnabled() || styleEx.GetEnabled());
	}

	if (style.GetIncludeReadOnly()) // 셀에 readonly정보 포함 여부 알아냄
	{
		bReadOnly = style.GetReadOnly();
	}
	else // Column style에서 readonly 정보 알아냄
	{
		CGXStyle styleEx;
		GetColStyle(nCol, styleEx);
		bReadOnly = (styleEx.GetIncludeReadOnly() && styleEx.GetReadOnly());
	}

	bAppendRow = IsAppendRow(nRow);
	bHeaderCol = IsFrozenCol(nCol);

	// Printing 모드이면 헤더 Column이 프린트 되기 위해 다음과 같이 리턴한다.
	if (IsPrinting())
	{
		if (bHeaderCol) style.SetControl(GX_IDS_CTRL_HEADER);
		return TRUE;
	}

	// 일반 셀이 가장 많으므로 빨리 검사하고 바로 리턴할 수 있게 코드를 앞으로 뺀다.
	if (bEnabled && !bReadOnly && !bCurrentCell && !bCurrentRow && !bAppendRow && !bHeaderCol)
	{/*
		if (bReadOnlyMode) style
			.SetInterior(m_Color.ReadOnlyModeBg)
			.SetTextColor(m_Color.ReadOnlyModeFg);
		else */style
			.SetInterior(m_Color.NormalModeBg)
			.SetTextColor(m_Color.NormalModeFg);

		if(m_setRedFont.find(std::pair<ROWCOL, ROWCOL>(nRow, nCol)) != m_setRedFont.end())
		{
			style.SetTextColor(COLORREF(RGB(255,0,0)));	
		}
		
		return TRUE;
	}

	// Disable되었을 때
	if (!bEnabled)  
	{
		if (bHeaderCol) style           // Row or Column Header
			.SetControl(GX_IDS_CTRL_HEADER)
			.SetInterior(m_Color.RowColHeaderBg)
			.SetTextColor(m_Color.NormalModeFg);
		else if (bCurrentRow) style     // Disabled Current Row
			.SetInterior(m_Color.DisabledBg);//.SetTextColor(m_Color.ReadOnlyCurrentRowFg);
		/////////////////////////////////////////////////
		// modified by hp
		else style                      // Disabled Cell
			.SetInterior(m_Color.DisabledBg);//.SetTextColor(m_Color.DisabledFg);
		
		if (!bHeaderCol && bCurrentRow)
		{
			if(m_setRedFont.find(std::pair<ROWCOL, ROWCOL>(nRow, nCol)) != m_setRedFont.end())
			{
				style.SetTextColor(COLORREF(RGB(255,0,0)));	
			}
			SetCurrentRowStyle(style);		
		}

		return TRUE;
	}

	// Disable안 되고 ReadOnly만 설정되었을 때
	/*
	if (bReadOnly)
	{
		if (bCurrentCell) style         // Current Cell
			.SetInterior(m_Color.ReadOnlyCurrentCellBg)
			.SetTextColor(m_Color.ReadOnlyCurrentCellFg);
		else if (bCurrentRow) style     // Current Row
			.SetInterior(m_Color.ReadOnlyCurrentRowBg)
			.SetTextColor(m_Color.ReadOnlyCurrentRowFg);
		else style                      // Readonly Cell
			.SetInterior(m_Color.ReadOnlyBg)
			.SetTextColor(m_Color.ReadOnlyFg);

		if (bCurrentRow) SetCurrentRowStyle(style);

		return TRUE;
	}
	*/
	// Append Row가 Disable되었을 때(EnableAppendRow(FALSE)로 금지된 경우)
	if (bAppendRow && !m_bEnabledAppend)  // Disabled Apppend Row
	{
		if (bCurrentCell) style
			.SetInterior(m_Color.ReadOnlyCurrentCellBg)
			.SetTextColor(m_Color.ReadOnlyCurrentCellFg);
		else if (bCurrentRow) style
			.SetInterior(m_Color.DisabledAppendRowBg)
			.SetTextColor(m_Color.ReadOnlyCurrentRowFg);
		else style
			.SetInterior(m_Color.DisabledAppendRowBg)
			.SetTextColor(m_Color.ReadOnlyModeFg);

		if(m_setRedFont.find(std::pair<ROWCOL, ROWCOL>(nRow, nCol)) != m_setRedFont.end())
		{
			style.SetTextColor(COLORREF(RGB(255,0,0)));	
		}

		style.SetControl(GX_IDS_CTRL_STATIC);  // 수정이 안되도록 control을 static으로 바꾼다.

		if (bCurrentRow) SetCurrentRowStyle(style);

		return TRUE;
	}

	// Disable 안되고, ReadOnly 아니고, Append가 Disable 되지 않은 경우
	// Current Cell or Current Row or Enabled Append Row인 경우이다.
	/*
	if (bReadOnlyMode)
	{
		if (bCurrentCell) style
			.SetInterior(m_Color.ReadOnlyCurrentCellBg)
			.SetTextColor(m_Color.ReadOnlyCurrentCellFg);
		else if (bCurrentRow) style
			.SetInterior(m_Color.ReadOnlyCurrentRowBg)
			.SetTextColor(m_Color.ReadOnlyCurrentRowFg);
		else if (bAppendRow) style
			.SetInterior(m_Color.ReadOnlyModeBg)
			.SetTextColor(m_Color.ReadOnlyModeFg);
		else if (bHeaderCol) style    // Enabled Header Col(Only Frozen)
			.SetInterior(m_Color.ReadOnlyModeBg)
			.SetTextColor(m_Color.ReadOnlyModeFg);

		if (bCurrentRow || bHeaderCol) SetCurrentRowStyle(style);

		return TRUE;
	}
	else
	{*/
		if (bCurrentCell) style
			.SetInterior(m_Color.NormalCurrentCellBg)
			.SetTextColor(m_Color.NormalCurrentCellFg);
		else if (bCurrentRow) style
			.SetInterior(m_Color.NormalCurrentRowBg)
			.SetTextColor(m_Color.NormalCurrentRowFg);
		else if (bAppendRow) style
			.SetInterior(m_Color.NormalModeBg)
			.SetTextColor(m_Color.NormalModeFg);
		else if (bHeaderCol) style    // Enabled Header Col(Only Frozen)
			.SetInterior(m_Color.NormalModeBg)
			.SetTextColor(m_Color.NormalModeFg);

		if(m_setRedFont.find(std::pair<ROWCOL, ROWCOL>(nRow, nCol)) != m_setRedFont.end())
		{
			style.SetTextColor(COLORREF(RGB(255,0,0)));	
		}

		if (bCurrentRow || bHeaderCol) SetCurrentRowStyle(style);

		return TRUE;
 // }

	return TRUE;
}



