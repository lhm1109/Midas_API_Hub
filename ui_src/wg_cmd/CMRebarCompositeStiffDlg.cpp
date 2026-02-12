// CMStageCompSectStiffDlg.cpp : implementation file
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMRebarCompositeStiffDlg.h"

#include "..\wg_db\wg_db_UnitCtrl.h"
//#include "..\wg_db\wg_db_SectUtil.h"
#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_base\wg_base_I_GridColorMgr.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define D_ROW_NUMBER     24

/////////////////////////////////////////////////////////////////////////////
// CCMRebarCompositeStiffGrid member functions

BOOL CCMRebarCompositeStiffGrid::ProcessKeys(CWnd *pSender, UINT nMessage, UINT nChar, UINT nRepCnt, UINT flags)
{
	CRowColArray awRows;
	ROWCOL nRow=1, nCol=1;
	CGXControl* pControl;

	if (!GetCurrentCell(nRow, nCol)) // if no current cell, default action
		return CGXGridWnd::ProcessKeys(pSender, nMessage, nChar, nRepCnt, flags);

	pControl = GetControl(nRow, nCol);
	if (nMessage == WM_KEYDOWN)
	{
		switch(nChar)
		{
		case VK_TAB: 
			if (TabKeyPressed(nRow, nCol, pControl)) return TRUE;
			break;
		case VK_RETURN: case VK_RIGHT:  // 다음 셀로 이동
			if (RightKeyPressed(nRow, nCol, pControl)) return TRUE;
			break;
		case VK_LEFT: // 이전 셀로 이동
			if (LeftKeyPressed(nRow, nCol, pControl)) return TRUE;
			break;
		}
	}
	else if (nMessage == WM_CHAR)
	{
		switch(nChar)
		{
		// 셀이 Inactive 상태에서 Edit 셀에 스페이스를 눌렀을 때 편집을 활성화 시킨다.
		case VK_SPACE:
			if (SpaceKeyPressed(nRow, nCol, pControl)) return TRUE;
			break;
		}
	}
	return CGXGridWnd::ProcessKeys(pSender, nMessage, nChar, nRepCnt, flags);
}


BOOL CCMRebarCompositeStiffGrid::TabKeyPressed(ROWCOL nRow, ROWCOL nCol, CGXControl* pControl)
{
	GetParent()->GetDlgItem(IDC_CMD_SP_SD_BTN_OK)->SetFocus();
	return TRUE;
}

BOOL CCMRebarCompositeStiffGrid::LeftKeyPressed(ROWCOL nRow, ROWCOL nCol, CGXControl* pControl)
{
	if (SetLeftValidCell(nRow, nCol)) return TRUE;
	return TRUE;
}

BOOL CCMRebarCompositeStiffGrid::RightKeyPressed(ROWCOL nRow, ROWCOL nCol, CGXControl* pControl)
{
	if (SetRightValidCell(nRow, nCol)) return TRUE;
	return TRUE;
}

BOOL CCMRebarCompositeStiffGrid::SpaceKeyPressed(ROWCOL nRow, ROWCOL nCol, CGXControl* pControl)
{
	if (pControl && !pControl->IsActive() 
			&& pControl->CGXControl::IsKindOf(CONTROL_CLASS(CGXEditControl)))
	{
		if (!pControl->IsInit()) pControl->Init(nRow, nCol);
		CString value = GetExpressionRowCol(nRow, nCol);
		pControl->SetActive(TRUE);
		pControl->Refresh(); // 반드시 호출

		if (!pControl->IsKindOf(CONTROL_CLASS(CGXComboBox)))  // combo box가 아니면
			((CGXEditControl*)pControl)->SetSel(0, value.GetLength());
		return TRUE;
	}
	return FALSE;
}

BOOL CCMRebarCompositeStiffGrid::SetLeftValidCell(ROWCOL nRow, ROWCOL nCol)
{
	ROWCOL i, j;
	i = nRow; j = nCol-1; // 현재 행 이전 열
	if (j < 1) { i--; j = GetColCount(); }  

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
				SetCurrentCell(i, j);
				return TRUE;
			} 
			j--;  // 이전 열을 조사
		} 
		i--;  // 이전 줄로 이동
		j = GetColCount();
	}
	return FALSE;
}

BOOL CCMRebarCompositeStiffGrid::SetRightValidCell(ROWCOL nRow, ROWCOL nCol)
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
				SetCurrentCell(i, j);
				return TRUE;
			}
			j++;  // 다음 열을 조사
		}
		i++;  // 다음 줄로 이동
		j = 1;
	}
	return FALSE;
}

// 주어진 셀의 값을 가져오는 함수
CString CCMRebarCompositeStiffGrid::GetEditingValue(ROWCOL nRow, ROWCOL nCol)
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
					if (csNum == _T("0")) csNum = _T("1");
					else csNum = _T("0");
				}
			}
		}
	}
	if (!bGet) csNum = GetValueRowCol(nRow, nCol);
	return csNum;
}

// 2003. 2. 13    by TAE
// Hidden Rows/Cols의 복사를 막기 위한 구현 
// ( HideCols(), HideRows() 함수에 의한 Hide일때만 ) 
BOOL CCMRebarCompositeStiffGrid::Copy( )
{
	// 실제로 Selected Range는 Row, Col의 조합으로 저장된다는 사실에 착안 
	// ( 따라서 Multi Selection의 경우는 별도의 처리를 해주어야 함. )
	CRowColArray aSelectedRow, aSelectedCol;
	
	int nSelectedCol = GetSelectedCols(aSelectedCol, FALSE, FALSE);
	for(int i=0; i<nSelectedCol; i++)
	{
		ROWCOL nCol = aSelectedCol.GetAt(i);
		if(IsColHidden(nCol))
			SelectRange(CGXRange().SetCols(nCol), FALSE);
	}  

	int nSelectedRow = GetSelectedRows(aSelectedRow, FALSE, FALSE);
	for(int i=0; i<nSelectedRow; i++)
	{
		ROWCOL nRow = aSelectedRow.GetAt(i);
		if(IsRowHidden(nRow))
			SelectRange(CGXRange().SetRows(nRow), FALSE);
	}
	return CGXGridWnd::Copy();
}

BOOL CCMRebarCompositeStiffGrid::OnPasteFromClipboard(const CGXRange &range)
{
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
		if (range.left+k < nColCount && IsColHidden(range.left+k))
		{
			x.InsertCols(k+1, 1);
			aHideCol.Add(range.left+k);
			r.right++;
			nPasteColCount++;
		}
	}
	for (int k = 1; k < nPasteRowCount; k++)
	{
		if (range.top+k < nRowCount && IsRowHidden(range.top+k))
		{
			x.InsertRows(k+1, 1);
			x.SetExpressionRowCol(k+1, 1, _T("100"));
			aHideRow.Add(range.top+k);
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
				value = x.GetValueRowCol(1, i+1);
				for (int j = 1; j < range.GetHeight(); j++)
					x.SetExpressionRowCol(j+1, i+1, value);
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

/////////////////////////////////////////////////////////////////////////////
// CCMRebarCompositeStiffDlg dialog
// 이 다이얼로그는 Stiffnes를 보여주기 위한 다이얼로그이다.
CCMRebarCompositeStiffDlg::CCMRebarCompositeStiffDlg(CDBDoc* pDoc, 
		CArray<T_SECT_SECTBASE_D, T_SECT_SECTBASE_D&>* pData, CStringArray* pTitle, BOOL bReadOnly, CWnd* pParent)
	: CDialogMove(CCMRebarCompositeStiffDlg::IDD, pParent)
{
	// 초기화 
	m_pDoc = NULL; 
	m_pData = NULL;
	m_pTitle = NULL;

	ASSERT(pDoc);
	ASSERT(pData);
	ASSERT(pTitle);

	m_pDoc = pDoc;
	m_pData = pData;
	m_pTitle = pTitle;
	m_bReadOnly = bReadOnly;
	ASSERT((*pData).GetSize()==(*pTitle).GetSize());
	m_nColSize = (*pData).GetSize();

	//{{AFX_DATA_INIT(CCMRebarCompositeStiffDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

void CCMRebarCompositeStiffDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMRebarCompositeStiffDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	DDX_Control(pDX, IDC_CMD_SP_SD_GRID, m_wndGrid);
	//}}AFX_DATA_MAP
}

/////////////////////////////////////////////////////////////////////////////
// CCMRebarCompositeStiffDlg implementation

void CCMRebarCompositeStiffDlg::SetTitleInGrid()
{
	CString aTitle[] = {
		_LS(IDS_WG_CMD__ADDD__Area),  _LS(IDS_WG_CMD__ADDD__Asy), _LS(IDS_WG_CMD__ADDD__Asz), 
		_LS(IDS_WG_CMD__ADDD__Ixx),   _LS(IDS_WG_CMD__ADDD__Iyy), _LS(IDS_WG_CMD__ADDD__Izz),
		_LS(IDS_WG_CMD__ADDD__Cyp),   _LS(IDS_WG_CMD__ADDD__Cym), _LS(IDS_WG_CMD__ADDD__Czp), 
		_LS(IDS_WG_CMD__ADDD__Czm),   _LS(IDS_WG_CMD__ADDD__Qyb), _LS(IDS_WG_CMD__ADDD__Qzb), 
		_LS(IDS_WG_CMD__ADDD__PeriOut),   _LS(IDS_WG_CMD__ADDD__PeriIn), 
		_LS(IDS_WG_CMD__ADDD__Center_Y),  _LS(IDS_WG_CMD__ADDD__Center_Z),
		_T("y1"), _T("z1"), _T("y2"), _T("z2"), _T("y3"), _T("z3"), _T("y4"), _T("z4")
	};

	int nNum = sizeof(aTitle)/sizeof(CString);
	for (int i = 0; i < nNum; i++) 
		m_wndGrid.SetValueRange(CGXRange(i+1, 0), aTitle[i]);

	for (int i = 0; i < m_nColSize; i++)
		m_wndGrid.SetValueRange(CGXRange(0, i+1), (*m_pTitle)[i]);
	int nUnitCol = m_nColSize+1;
	m_wndGrid.SetValueRange(CGXRange(0, nUnitCol), _LS(IDS_WG_CMD__ADDD__Unit));
}

void CCMRebarCompositeStiffDlg::SetUnitInGrid()
{
	CString aUnit[D_ROW_NUMBER];
	T_SECT_STIFFNESS_UNIT *pUnit = &(CUnitCtrl::m_SECT_UNIT.SectBefore.SectI.Stiffness);
	m_pDoc->m_pUnitCtrl->GetUnitSystem(pUnit->Area, aUnit[0]);
	m_pDoc->m_pUnitCtrl->GetUnitSystem(pUnit->Asy, aUnit[1]);
	m_pDoc->m_pUnitCtrl->GetUnitSystem(pUnit->Asz, aUnit[2]);
	m_pDoc->m_pUnitCtrl->GetUnitSystem(pUnit->Rxx, aUnit[3]);
	m_pDoc->m_pUnitCtrl->GetUnitSystem(pUnit->Ryy, aUnit[4]);
	m_pDoc->m_pUnitCtrl->GetUnitSystem(pUnit->Rzz, aUnit[5]);

	m_pDoc->m_pUnitCtrl->GetUnitSystem(pUnit->Cyp, aUnit[6]);
	m_pDoc->m_pUnitCtrl->GetUnitSystem(pUnit->Cym, aUnit[7]);
	m_pDoc->m_pUnitCtrl->GetUnitSystem(pUnit->Czp, aUnit[8]);
	m_pDoc->m_pUnitCtrl->GetUnitSystem(pUnit->Czm, aUnit[9]);
	m_pDoc->m_pUnitCtrl->GetUnitSystem(pUnit->Qyb, aUnit[10]);
	m_pDoc->m_pUnitCtrl->GetUnitSystem(pUnit->Qzb, aUnit[11]);

	T_SECT_SECTBASE_UNIT *pUnit2 = &(CUnitCtrl::m_SECT_UNIT.SectBefore.SectI);
	m_pDoc->m_pUnitCtrl->GetUnitSystem(pUnit2->PeriOut,     aUnit[12]);
	m_pDoc->m_pUnitCtrl->GetUnitSystem(pUnit2->PeriIn,      aUnit[13]);
	m_pDoc->m_pUnitCtrl->GetUnitSystem(pUnit2->Design.YBar, aUnit[14]);
	m_pDoc->m_pUnitCtrl->GetUnitSystem(pUnit2->Design.ZBar, aUnit[15]);

	m_pDoc->m_pUnitCtrl->GetUnitSystem(pUnit->dx1, aUnit[16]);
	m_pDoc->m_pUnitCtrl->GetUnitSystem(pUnit->dy1, aUnit[17]);
	m_pDoc->m_pUnitCtrl->GetUnitSystem(pUnit->dx2, aUnit[18]);
	m_pDoc->m_pUnitCtrl->GetUnitSystem(pUnit->dy2, aUnit[19]);
	m_pDoc->m_pUnitCtrl->GetUnitSystem(pUnit->dx3, aUnit[20]);
	m_pDoc->m_pUnitCtrl->GetUnitSystem(pUnit->dy3, aUnit[21]);
	m_pDoc->m_pUnitCtrl->GetUnitSystem(pUnit->dx4, aUnit[22]);
	m_pDoc->m_pUnitCtrl->GetUnitSystem(pUnit->dy4, aUnit[23]);

	for (int i = 0; i < D_ROW_NUMBER; i++)
		m_wndGrid.SetValueRange(CGXRange(i+1, m_nColSize+1), aUnit[i]);
}

void CCMRebarCompositeStiffDlg::SetStiffnessData(ROWCOL nCol, T_SECT_SECTBASE_D* pBase)
{
	m_wndGrid.SetValueRange(CGXRange( 1, nCol), pBase->Stiffness.Area);
	m_wndGrid.SetValueRange(CGXRange( 2, nCol), pBase->Stiffness.Asy);
	m_wndGrid.SetValueRange(CGXRange( 3, nCol), pBase->Stiffness.Asz);
	m_wndGrid.SetValueRange(CGXRange( 4, nCol), pBase->Stiffness.Rxx);
	m_wndGrid.SetValueRange(CGXRange( 5, nCol), pBase->Stiffness.Ryy);
	m_wndGrid.SetValueRange(CGXRange( 6, nCol), pBase->Stiffness.Rzz);

	m_wndGrid.SetValueRange(CGXRange(7, nCol),  pBase->Stiffness.Cyp);
	m_wndGrid.SetValueRange(CGXRange(8, nCol),  pBase->Stiffness.Cym);
	m_wndGrid.SetValueRange(CGXRange(9, nCol),  pBase->Stiffness.Czp);
	m_wndGrid.SetValueRange(CGXRange(10, nCol), pBase->Stiffness.Czm);
	m_wndGrid.SetValueRange(CGXRange(11, nCol), pBase->Stiffness.Qyb);
	m_wndGrid.SetValueRange(CGXRange(12, nCol), pBase->Stiffness.Qzb);

	m_wndGrid.SetValueRange(CGXRange(13, nCol), pBase->PeriOut);
	m_wndGrid.SetValueRange(CGXRange(14, nCol), pBase->PeriIn);
	m_wndGrid.SetValueRange(CGXRange(15, nCol), pBase->Design.YBar);
	m_wndGrid.SetValueRange(CGXRange(16, nCol), pBase->Design.ZBar);

	m_wndGrid.SetValueRange(CGXRange(17, nCol), pBase->Stiffness.dx1);
	m_wndGrid.SetValueRange(CGXRange(18, nCol), pBase->Stiffness.dy1);
	m_wndGrid.SetValueRange(CGXRange(19, nCol), pBase->Stiffness.dx2);
	m_wndGrid.SetValueRange(CGXRange(20, nCol), pBase->Stiffness.dy2);
	m_wndGrid.SetValueRange(CGXRange(21, nCol), pBase->Stiffness.dx3);
	m_wndGrid.SetValueRange(CGXRange(22, nCol), pBase->Stiffness.dy3);
	m_wndGrid.SetValueRange(CGXRange(23, nCol), pBase->Stiffness.dx4);
	m_wndGrid.SetValueRange(CGXRange(24, nCol), pBase->Stiffness.dy4);
}

void CCMRebarCompositeStiffDlg::SetStiffnessData()
{
	BOOL bOldLock = m_wndGrid.LockUpdate(TRUE);
	for (int i = 0; i < m_nColSize; i++)
	{
		SetStiffnessData(i+1, &(*m_pData)[i]);
	}
	m_wndGrid.LockUpdate(bOldLock);
	if (!bOldLock) m_wndGrid.Redraw();
}

void CCMRebarCompositeStiffDlg::SetDialogSize()
{
	int nUnitCol = m_nColSize + 1;
	int nWidth = 0;
	int nHeight = 0;
	CRect rect, rect2;
	GetWindowRect(rect);
	m_wndGrid.GetWindowRect(rect2);
	for (int i = 0; i <= (nUnitCol); i++)
		nWidth += m_wndGrid.GetColWidth(i);
	nWidth += 5;
	for (int i = 0; i <= m_wndGrid.GetRowCount(); i++)
	{
		if (!m_wndGrid.IsRowHidden(i)) nHeight += m_wndGrid.GetRowHeight(i);
	}
	nHeight += 5;
	
	int nMarginW = (rect.Width() - rect2.Width());
	int nMarginH = (rect.Height() - rect2.Height());
	rect.right = rect.left + nWidth + nMarginW;
	rect.bottom = rect.top + nHeight + nMarginH;
	MoveWindow(rect);
	
	rect2.right = rect2.left + nWidth;
	rect2.bottom = rect2.top + nHeight;
	ScreenToClient(rect2);
	m_wndGrid.MoveWindow(rect2);
		
	// Button Position 
	CArray<UINT, UINT> aBtnGroup;
	aBtnGroup.Add(IDC_CMD_SP_SD_BTN_OK);
	
	CRect DlgRect, BtnRect;
	m_wndGrid.GetWindowRect(DlgRect);
	GetDlgItem(IDC_CMD_SP_SD_BTN_OK)->GetWindowRect(BtnRect);
	int nDistX = (DlgRect.right - BtnRect.right);
	CDlgUtil::CtrlMoveDistX(this, aBtnGroup, nDistX);

	nHeight = 0;
	for (int i = 0; i <= m_wndGrid.GetRowCount(); i++)
		nHeight += m_wndGrid.GetRowHeight(i);
	nHeight += globalUtils.ScaleByDPI(5);

	nMarginH = (rect.Height() - rect2.Height());
	rect.bottom = rect.top + nHeight + nMarginH;
	MoveWindow(rect);

	rect2.bottom = rect2.top + nHeight;
	m_wndGrid.MoveWindow(rect2);

	ClientToScreen(rect2);
	int nDistY = (rect2.bottom - BtnRect.top) + globalUtils.ScaleByDPI(7);
	CDlgUtil::CtrlMoveDistY(this, aBtnGroup, nDistY);

}

void CCMRebarCompositeStiffDlg::SetGridReadOnly(ROWCOL nFromCol, ROWCOL nToCol)
{
	m_wndGrid.SetStyleRange(CGXRange().SetCols(nFromCol, nToCol), 
													CGXStyle().SetReadOnly(TRUE));
}

BEGIN_MESSAGE_MAP(CCMRebarCompositeStiffDlg, CDialogMove)
	//{{AFX_MSG_MAP(CCMRebarCompositeStiffDlg)
	ON_BN_CLICKED(IDC_CMD_SP_SD_BTN_OK, OnCmdSpSdBtnOk)
	ON_BN_CLICKED(IDC_CMD_SP_SD_BTN_CANCEL, OnCmdSpSdBtnCancel)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMRebarCompositeStiffDlg message handlers

BOOL CCMRebarCompositeStiffDlg::OnInitDialog() 
{
	CDialogMove::OnInitDialog();
	
	// TODO: Add extra initialization here
	ROWCOL nUnitCol = m_nColSize+1;

	m_wndGrid.Initialize();
	I_GridColorMgr::ApplyColorStyle(&m_wndGrid);
	m_wndGrid.LockUpdate(TRUE); 
	m_wndGrid.GetParam()->EnableUndo(FALSE);  
	m_wndGrid.SetRowCount(D_ROW_NUMBER); 
	m_wndGrid.SetColCount(nUnitCol); // 전체 컬럼 크기 

	SetTitleInGrid(); 
	SetUnitInGrid();  

	// Grid의 style을 변경한다.
	m_wndGrid.ChangeStandardStyle(CGXStyle()
			.SetFont(CGXFont_GC().SetFaceName(_LS(IDS_CMD_FONT_Ms_Sans_Serif)).SetSize(9))
			.SetVerticalAlignment(DT_VCENTER)
			.SetAllowEnter(FALSE));

	// column 크기를 맞춘다.
	m_wndGrid.SetColWidth(0, 0, globalUtils.ScaleByDPI(65));
	m_wndGrid.SetColWidth(1, nUnitCol-1, globalUtils.ScaleByDPI(116));
	m_wndGrid.SetColWidth(nUnitCol, nUnitCol, globalUtils.ScaleByDPI(60));
	m_wndGrid.SetRowHeight(0, 0, globalUtils.ScaleByDPI(19));
	m_wndGrid.SetRowHeight(1, m_wndGrid.GetRowCount(), globalUtils.ScaleByDPI(16));

	m_wndGrid.ColHeaderStyle()
			.SetReadOnly(TRUE).SetEnabled(FALSE);
	m_wndGrid.RowHeaderStyle()
			.SetReadOnly(TRUE).SetEnabled(FALSE);
	m_wndGrid.SetStyleRange(CGXRange().SetCols(1, nUnitCol-1), CGXStyle()
		.SetControl(GX_IDS_CTRL_EDIT)    
		.SetHorizontalAlignment(DT_RIGHT)
		.SetValueType(GX_VT_NUMERIC)
		.SetFormat(GX_FMT_FLOAT)
		.SetPlaces(6)
		.SetEnabled(FALSE)
		);
	m_wndGrid.SetStyleRange(CGXRange().SetCols(nUnitCol), 
			CGXStyle().SetReadOnly(TRUE).SetEnabled(FALSE));

	// cell의 width와 height 크기 변경을 금지한다.
	m_wndGrid.GetParam()->EnableTrackRowHeight(FALSE);
	m_wndGrid.GetParam()->EnableTrackColWidth(FALSE);

	// column과 row의 위치 이동을 금지시킨다.
	m_wndGrid.GetParam()->EnableMoveCols(FALSE);
	m_wndGrid.GetParam()->EnableMoveRows(FALSE);

	// double click시 셀 편집이 되도록 한다.
	// m_wndGrid.GetParam()->SetActivateCellFlags(GX_CAFOCUS_DBLCLICKONCELL);

	// 현재 셀을 설정한다.
	m_wndGrid.SetCurrentCell(1, 1);

	// Stiffness 값을 설정한다.
	SetStiffnessData();

	SetDialogSize();  // Dialog의 size를 설정한다.

	// 수정  
	if(m_bReadOnly) SetGridReadOnly(1, nUnitCol);
	else SetGridReadOnly(nUnitCol, nUnitCol);

	m_wndGrid.LockUpdate(FALSE);
	m_wndGrid.Redraw();
	m_wndGrid.SetFocus();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCMRebarCompositeStiffDlg::OnCmdSpSdBtnOk() 
{
	CDialogMove::OnOK();
}

void CCMRebarCompositeStiffDlg::OnCmdSpSdBtnCancel() 
{
	// TODO: Add your control notification handler code here
	CDialogMove::OnCancel();
}





