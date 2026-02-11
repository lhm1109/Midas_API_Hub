// CMStageCompSectStiffDlg.cpp : implementation file
//
// 2002. 11. 15   by TAE
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMStageCompSectStiffDlg.h"

#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_SectUtil.h"
#include "..\wg_db\wg_db_SectDB.h"
#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_base\wg_base_I_GridColorMgr.h"
#include "..\wg_common\wg_common_TBGrid.h"

#include "CMStageCompSectStiffListDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//#define D_ROW_NUMBER     21
#define D_ROW_NUMBER     22   // MNET:XXXX-HSSHIM-20130902. Iw 추가 

#define STIFSCALE 1
#define STIFFNESS 2
#define STIFTAPER 3
//#define STIFTMULT 4
//#define SCALEAUTO 5
//#define SCALEAUTO_J 6

/////////////////////////////////////////////////////////////////////////////
// CCMStageSectStiffGrid member functions

BOOL CCMStageSectStiffGrid::ProcessKeys(CWnd *pSender, UINT nMessage, UINT nChar, UINT nRepCnt, UINT flags)
{
	CRowColArray awRows;
	ROWCOL nRow = 1, nCol = 1;
	CGXControl* pControl;

	if (!GetCurrentCell(nRow, nCol)) // if no current cell, default action
		return CGXGridWnd::ProcessKeys(pSender, nMessage, nChar, nRepCnt, flags);

	pControl = GetControl(nRow, nCol);
	if (nMessage == WM_KEYDOWN)
	{
		switch (nChar)
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
		switch (nChar)
		{
			// 셀이 Inactive 상태에서 Edit 셀에 스페이스를 눌렀을 때 편집을 활성화 시킨다.
		case VK_SPACE:
			if (SpaceKeyPressed(nRow, nCol, pControl)) return TRUE;
			break;
		}
	}
	return CGXGridWnd::ProcessKeys(pSender, nMessage, nChar, nRepCnt, flags);
}


BOOL CCMStageSectStiffGrid::TabKeyPressed(ROWCOL nRow, ROWCOL nCol, CGXControl* pControl)
{
	GetParent()->GetDlgItem(IDC_CMD_SP_SD_BTN_OK)->SetFocus();
	return TRUE;
}

BOOL CCMStageSectStiffGrid::LeftKeyPressed(ROWCOL nRow, ROWCOL nCol, CGXControl* pControl)
{
	if (SetLeftValidCell(nRow, nCol)) return TRUE;
	return TRUE;
}

BOOL CCMStageSectStiffGrid::RightKeyPressed(ROWCOL nRow, ROWCOL nCol, CGXControl* pControl)
{
	if (SetRightValidCell(nRow, nCol)) return TRUE;
	return TRUE;
}

BOOL CCMStageSectStiffGrid::SpaceKeyPressed(ROWCOL nRow, ROWCOL nCol, CGXControl* pControl)
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

BOOL CCMStageSectStiffGrid::SetLeftValidCell(ROWCOL nRow, ROWCOL nCol)
{
	ROWCOL i, j;
	i = nRow; j = nCol - 1; // 현재 행 이전 열
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

BOOL CCMStageSectStiffGrid::SetRightValidCell(ROWCOL nRow, ROWCOL nCol)
{
	ROWCOL i, j;
	i = nRow; j = nCol + 1; // 현재 행 다음 열
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
CString CCMStageSectStiffGrid::GetEditingValue(ROWCOL nRow, ROWCOL nCol)
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
BOOL CCMStageSectStiffGrid::Copy()
{
	// 실제로 Selected Range는 Row, Col의 조합으로 저장된다는 사실에 착안 
	// ( 따라서 Multi Selection의 경우는 별도의 처리를 해주어야 함. )
	CRowColArray aSelectedRow, aSelectedCol;

	int nSelectedCol = GetSelectedCols(aSelectedCol, FALSE, FALSE);
	for (int i = 0; i < nSelectedCol; i++)
	{
		ROWCOL nCol = aSelectedCol.GetAt(i);
		if (IsColHidden(nCol))
			SelectRange(CGXRange().SetCols(nCol), FALSE);
	}

	int nSelectedRow = GetSelectedRows(aSelectedRow, FALSE, FALSE);
	for (int i = 0; i < nSelectedRow; i++)
	{
		ROWCOL nRow = aSelectedRow.GetAt(i);
		if (IsRowHidden(nRow))
			SelectRange(CGXRange().SetRows(nRow), FALSE);
	}
	return CGXGridWnd::Copy();
}

BOOL CCMStageSectStiffGrid::OnPasteFromClipboard(const CGXRange &range)
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

//--------------------------------------------------------------------------
// GetStyleRowCol  셀 스타일지정(여기선 Color) 
//--------------------------------------------------------------------------
// BOOL CCMStageSectStiffGrid::GetStyleRowCol(ROWCOL nRow, ROWCOL nCol, CGXStyle& style, GXModifyType mt, int nType)
// {
// 	BOOL bCurrentRow;
// 	BOOL bCurrentCell;
// 	BOOL bEnabled;
// 	BOOL bHeaderCol;
// 
// 	ROWCOL ncRow, ncCol;    // 현재 Row, Col
// 	CGXGridWnd::GetStyleRowCol(nRow, nCol, style, mt, nType);
// 
// 	// 현재 셀의 그리드 좌표(Row, Col 번호)를 얻어온다.
// 	if (!GetCurrentCell(ncRow, ncCol))
// 		ncRow = GetRowCount() + 1; // 현재 셀이 없는 경우 : ncRow를 범위 밖으로 설정
// 
// 	// 현재 셀이 이동되면 새로 그려준다.
// 	m_bRefreshOnSetCurrentCell = TRUE;
// 
// 	// Row나 Col이 1보다 작으면 리턴한다.(Standard Row or Column Header)
// 	if (nRow < 1 || nCol < 1) return TRUE;
// 
// 	// Get Current Table and Cell State
// 	bCurrentRow = (nRow == ncRow);
// 	bCurrentCell = (nRow == ncRow && nCol == ncCol);
// 
// 	// Enable, ReadOnly와 같은 Cell의 상태를 조사한다.
// 	// ComposeStyleRowCol은 호출해서는 안된다.
// 	// 그래서 여기서는 직접 조사한다.
// 	// 단 Cell Style, Column Style만 조사한다.
// 	if (style.GetIncludeEnabled())  // 셀에 Enable정보 포함 여부 알아냄
// 	{
// 		bEnabled = style.GetEnabled();
// 	}
// 	else // Column style에서 enable 정보 알아냄
// 	{
// 		CGXStyle styleEx;
// 		GetColStyle(nCol, styleEx);
// 		bEnabled = (!styleEx.GetIncludeEnabled() || styleEx.GetEnabled());
// 	}
// 
// 	bHeaderCol = IsFrozenCol(nCol);
// 
// 	// Printing 모드이면 헤더 Column이 프린트 되기 위해 다음과 같이 리턴한다.
// 	if (IsPrinting())
// 	{
// 		if (bHeaderCol) style.SetControl(GX_IDS_CTRL_HEADER);
// 		return TRUE;
// 	}
// 
// 	// 일반 셀이 가장 많으므로 빨리 검사하고 바로 리턴할 수 있게 코드를 앞으로 뺀다.
// 	if (bEnabled && !bCurrentCell && !bCurrentRow && !bHeaderCol)
// 	{
// 		style
// 			.SetInterior(CTBGrid::m_Color.NormalModeBg)
// 			.SetTextColor(CTBGrid::m_Color.NormalModeFg);
// 
// 		return TRUE;
// 	}
// 
// 	// Disable되었을 때
// 	if (!bEnabled)
// 	{
// 		if (bHeaderCol) style           // Row or Column Header
// 			.SetControl(GX_IDS_CTRL_HEADER)
// 			.SetInterior(CTBGrid::m_Color.RowColHeaderBg)
// 			.SetTextColor(CTBGrid::m_Color.NormalModeFg);
// 		else if (bCurrentRow) style     // Disabled Current Row
// 			.SetInterior(CTBGrid::m_Color.DisabledBg)
// 			.SetTextColor(CTBGrid::m_Color.ReadOnlyCurrentRowFg);
// 		else style                      // Disabled Cell
// 			.SetInterior(CTBGrid::m_Color.DisabledBg)
// 			.SetTextColor(CTBGrid::m_Color.DisabledFg);
// 
// 		return TRUE;
// 	}
// 
// 	if (bCurrentCell) style
// 		.SetInterior(CTBGrid::m_Color.NormalCurrentCellBg)
// 		.SetTextColor(CTBGrid::m_Color.NormalCurrentCellFg);
// 	else if (bCurrentRow) style
// 		.SetInterior(CTBGrid::m_Color.NormalModeBg)
// 		.SetTextColor(CTBGrid::m_Color.NormalModeFg);
// 	else if (bHeaderCol) style    // Enabled Header Col(Only Frozen)
// 		.SetInterior(CTBGrid::m_Color.NormalModeBg)
// 		.SetTextColor(CTBGrid::m_Color.NormalModeFg);
// 
// 	return TRUE;
// }

/////////////////////////////////////////////////////////////////////////////
// CCMStageCompSectStiffDlg dialog
// 이 다이얼로그는 Stiffnes를 보여주기 위한 다이얼로그이다.
CCMStageCompSectStiffDlg::CCMStageCompSectStiffDlg(CDBDoc* pDoc, int nType, T_SECT_STIFFNESS* pData1, T_SECT_STIFFNESS* pData2, BOOL bReadOnly, CWnd* pParent)
	: CDialogMove(CCMStageCompSectStiffDlg::IDD, pParent)
{
	// 초기화 
	m_pDoc = NULL;
	m_pData1 = NULL;
	m_pData2 = NULL;

	ASSERT(pDoc);
	ASSERT(pData1);
	ASSERT(nType == 1 || nType == 2 || nType == 3);

	m_pDoc = pDoc;
	m_nType = nType;
	m_pData1 = pData1;
	m_pData2 = pData2;

	m_pDataAuto = pData1;
	m_pDataAutoJ = pData2;

	m_bReadOnly = bReadOnly;
	m_bHideUnderQzb = FALSE;
	m_bDumbAndStub = FALSE;
	m_bCompNormalPar2 = FALSE;

	m_strTitle.Empty();

	//{{AFX_DATA_INIT(CCMStageCompSectStiffDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

void CCMStageCompSectStiffDlg::SetCompNormalPar2(BOOL bCompNormalPar2)
{
	m_bCompNormalPar2 = bCompNormalPar2;
}

void CCMStageCompSectStiffDlg::SetTitle(CString& str)
{
	m_strTitle = str;
}

void CCMStageCompSectStiffDlg::SetMultiData(int nType, T_SECT_STIFFNESS* pData3, T_SECT_STIFFNESS* pData4)
{
	// 초기화 2
	m_pData3 = pData3;
	m_pData4 = pData4;
	ASSERT(nType == 4); // 생성자와 통일성 유지를 위해 nType 인자를 받기는 하나 4만 허용
	m_nType = 4;
}

void CCMStageCompSectStiffDlg::SetScaleAutoData(T_SECT_STIFFNESS* pDataAuto, T_SECT_STIFFNESS* pDataAutoJ)
{
	m_pDataAuto = pDataAuto;
	m_pDataAutoJ = pDataAutoJ;
}

// Composite ScaleFactor 일때 y1 부터 아래 Row 를 숨겨준다. Parent 에서 호출.
void CCMStageCompSectStiffDlg::HideUnderQzb()
{
	m_bHideUnderQzb = TRUE;
}

void CCMStageCompSectStiffDlg::GridForDumbAndStub()
{
	m_bDumbAndStub = TRUE;
}

void CCMStageCompSectStiffDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CMD_SP_SD_IXX_CHK, m_wndIxxCheck);
	DDX_Control(pDX, IDC_CMD_SP_SD_GRID, m_wndGrid);
	//{{AFX_DATA_MAP(CCMStageCompSectStiffDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

/////////////////////////////////////////////////////////////////////////////
// CCMStageCompSectStiffDlg implementation

void CCMStageCompSectStiffDlg::SetTitleInGrid()
{
	CString aTitle[] = {
		_LS(IDS_WG_CMD__ADDD__Area), _LS(IDS_WG_CMD__ADDD__Asy), _LS(IDS_WG_CMD__ADDD__Asz),
		_LS(IDS_WG_CMD__ADDD__Ixx) , _LS(IDS_WG_CMD__ADDD__Iyy), _LS(IDS_WG_CMD__ADDD__Izz),
		_LS(IDS_WG_CMD__ADDD__Cyp) , _LS(IDS_WG_CMD__ADDD__Cym), _LS(IDS_WG_CMD__ADDD__Czp),
		_LS(IDS_WG_CMD__ADDD__Czm) , _LS(IDS_WG_CMD__ADDD__Qyb), _LS(IDS_WG_CMD__ADDD__Qzb),
		_LS(IDS_WG_CMD__ADD__Weight),
		_T("y1"), _T("z1"), _T("y2"), _T("z2"), _T("y3"), _T("z3"), _T("y4"), _T("z4"), _T("Iw"),
	};

	CStringArray aColHeader;

	int nNum = sizeof(aTitle) / sizeof(CString);
	for (int i = 0; i < nNum; i++)
		m_wndGrid.SetValueRange(CGXRange(i + 1, 0), aTitle[i]);

	GetColumnTitle(aColHeader);
	for (int i = 0; i < aColHeader.GetSize(); i++)
		m_wndGrid.SetValueRange(CGXRange(0, i + 1), aColHeader[i]);
}

void CCMStageCompSectStiffDlg::SetUnitInGrid()
{
	if (m_nType == STIFSCALE)
	{
		m_wndGrid.HideCols(3, 3, TRUE);
		return;
	}

	CString aUnit[D_ROW_NUMBER];
	T_SECT_STIFFNESS_UNIT *pUnit = &(CUnitCtrl::m_SECT_UNIT.SectBefore.SectI.Stiffness);
	m_pDoc->m_pUnitCtrl->GetUnitSystem(pUnit->Area, aUnit[0]);
	m_pDoc->m_pUnitCtrl->GetUnitSystem(pUnit->Asy, aUnit[1]);
	m_pDoc->m_pUnitCtrl->GetUnitSystem(pUnit->Asz, aUnit[2]);
	m_pDoc->m_pUnitCtrl->GetUnitSystem(pUnit->Rxx, aUnit[3]);
	m_pDoc->m_pUnitCtrl->GetUnitSystem(pUnit->Ryy, aUnit[4]);
	m_pDoc->m_pUnitCtrl->GetUnitSystem(pUnit->Rzz, aUnit[5]);

	// 추가분 
	m_pDoc->m_pUnitCtrl->GetUnitSystem(pUnit->Cyp, aUnit[6]);
	m_pDoc->m_pUnitCtrl->GetUnitSystem(pUnit->Cym, aUnit[7]);
	m_pDoc->m_pUnitCtrl->GetUnitSystem(pUnit->Czp, aUnit[8]);
	m_pDoc->m_pUnitCtrl->GetUnitSystem(pUnit->Czm, aUnit[9]);
	m_pDoc->m_pUnitCtrl->GetUnitSystem(pUnit->Qyb, aUnit[10]);
	m_pDoc->m_pUnitCtrl->GetUnitSystem(pUnit->Qzb, aUnit[11]);
	m_pDoc->m_pUnitCtrl->GetUnitSystem(pUnit->WArea, aUnit[12]);

	m_pDoc->m_pUnitCtrl->GetUnitSystem(pUnit->dx1, aUnit[13]);
	m_pDoc->m_pUnitCtrl->GetUnitSystem(pUnit->dy1, aUnit[14]);
	m_pDoc->m_pUnitCtrl->GetUnitSystem(pUnit->dx2, aUnit[15]);
	m_pDoc->m_pUnitCtrl->GetUnitSystem(pUnit->dy2, aUnit[16]);
	m_pDoc->m_pUnitCtrl->GetUnitSystem(pUnit->dx3, aUnit[17]);
	m_pDoc->m_pUnitCtrl->GetUnitSystem(pUnit->dy3, aUnit[18]);
	m_pDoc->m_pUnitCtrl->GetUnitSystem(pUnit->dx4, aUnit[19]);
	m_pDoc->m_pUnitCtrl->GetUnitSystem(pUnit->dy4, aUnit[20]);

	m_pDoc->m_pUnitCtrl->GetUnitSystem(pUnit->dIw, aUnit[21]);    //  Iw

	if (m_nType != 4)
	{
		for (int i = 0; i < D_ROW_NUMBER; i++)
			m_wndGrid.SetValueRange(CGXRange(i + 1, 3), aUnit[i]);
	}
	else
	{
		for (int i = 0; i < D_ROW_NUMBER; i++)
			m_wndGrid.SetValueRange(CGXRange(i + 1, 4), aUnit[i]);
	}
}

void CCMStageCompSectStiffDlg::SetStiffnessData(ROWCOL nCol, T_SECT_STIFFNESS* pStiffness)
{
	m_wndGrid.SetValueRange(CGXRange(1, nCol), pStiffness->Area);
	m_wndGrid.SetValueRange(CGXRange(2, nCol), pStiffness->Asy);
	m_wndGrid.SetValueRange(CGXRange(3, nCol), pStiffness->Asz);
	m_wndGrid.SetValueRange(CGXRange(4, nCol), pStiffness->Rxx);
	m_wndGrid.SetValueRange(CGXRange(5, nCol), pStiffness->Ryy);
	m_wndGrid.SetValueRange(CGXRange(6, nCol), pStiffness->Rzz);

	// 추가분 
	m_wndGrid.SetValueRange(CGXRange(7, nCol), pStiffness->Cyp);
	m_wndGrid.SetValueRange(CGXRange(8, nCol), pStiffness->Cym);
	m_wndGrid.SetValueRange(CGXRange(9, nCol), pStiffness->Czp);
	m_wndGrid.SetValueRange(CGXRange(10, nCol), pStiffness->Czm);
	m_wndGrid.SetValueRange(CGXRange(11, nCol), pStiffness->Qyb);
	m_wndGrid.SetValueRange(CGXRange(12, nCol), pStiffness->Qzb);
	m_wndGrid.SetValueRange(CGXRange(13, nCol), pStiffness->WArea);

	m_wndGrid.SetValueRange(CGXRange(14, nCol), pStiffness->dx1);
	m_wndGrid.SetValueRange(CGXRange(15, nCol), pStiffness->dy1);
	m_wndGrid.SetValueRange(CGXRange(16, nCol), pStiffness->dx2);
	m_wndGrid.SetValueRange(CGXRange(17, nCol), pStiffness->dy2);
	m_wndGrid.SetValueRange(CGXRange(18, nCol), pStiffness->dx3);
	m_wndGrid.SetValueRange(CGXRange(19, nCol), pStiffness->dy3);
	m_wndGrid.SetValueRange(CGXRange(20, nCol), pStiffness->dx4);
	m_wndGrid.SetValueRange(CGXRange(21, nCol), pStiffness->dy4);

	m_wndGrid.SetValueRange(CGXRange(22, nCol), pStiffness->dIw);
}

int CCMStageCompSectStiffDlg::GetSectCount()
{
	if (m_nType == STIFTAPER) return 2;
	else if (m_nType == 4)         return 3;
	else return 1;
}

void CCMStageCompSectStiffDlg::GetColumnTitle(CStringArray &aTitle)
{
	aTitle.RemoveAll();

	if (m_nType == STIFSCALE) // Scale
	{
		aTitle.Add(_LS(IDS_CMD_CSCS___Stiff_Scale));
		aTitle.Add(_T(""));
		aTitle.Add(_T(""));
	}
	else if (m_nType == STIFFNESS) // Stiffness
	{
		aTitle.Add(_LS(IDS_WG_CMD__ADDD__ValueONLY));
		aTitle.Add(_T(""));
		aTitle.Add(_LS(IDS_WG_CMD__ADDD__Unit));
	}
	else if (m_nType == STIFTAPER) // Tapered Stiffness
	{
		aTitle.Add(_LS(IDS_WG_CMD__ADDD__Value_I_));
		aTitle.Add(_LS(IDS_WG_CMD__ADDD__Value_J_));
		aTitle.Add(_LS(IDS_WG_CMD__ADDD__Unit));
	}
	else if (m_nType == 4) // Stiffness 이면서 MultipleElastic 존재
	{
		aTitle.Add(_LS(IDS_WG_CMD__ADDD__ValueONLY));
		aTitle.Add(_LS(IDS_CMD_SECF_LONGTERM));
		aTitle.Add(_LS(IDS_CMD_SECF_SHRINKAGE));
		aTitle.Add(_LS(IDS_WG_CMD__ADDD__Unit));
	}
	else ASSERT(0);
}

ROWCOL CCMStageCompSectStiffDlg::GetUnitCol()
{
	if (m_nType != 4)
		return 3;
	else
		return 4;
}

void CCMStageCompSectStiffDlg::GetStiffAndPeri(int nIndex, T_SECT_STIFFNESS* &ppStiff)
{
	ASSERT(nIndex == 0 || nIndex == 1 || (m_nType == 4 && nIndex == 2));

	if (m_nType == STIFSCALE) ppStiff = m_pData1;
	else if (m_nType == STIFFNESS) ppStiff = m_pData1;
	else if (m_nType == STIFTAPER)
	{
		if (nIndex == 0) ppStiff = m_pData1;
		else if (nIndex == 1) ppStiff = m_pData2;
	}
	else if (m_nType == 4)
	{
		if (nIndex == 0) ppStiff = m_pData1;
		else if (nIndex == 1) ppStiff = m_pData3; // longterm
		else if (nIndex == 2) ppStiff = m_pData4; // shrinkage
	}

	else ASSERT(0);
}

void CCMStageCompSectStiffDlg::SetStiffnessData()
{
	T_SECT_STIFFNESS* pStiff;
	BOOL bOldLock = m_wndGrid.LockUpdate(TRUE);
	for (int i = 0; i < GetSectCount(); i++)
	{
		GetStiffAndPeri(i, pStiff);
		SetStiffnessData(i + 1, pStiff);
	}
	m_wndGrid.LockUpdate(bOldLock);
	if (!bOldLock) m_wndGrid.Redraw();
}

BOOL CCMStageCompSectStiffDlg::GetStiffnessData()
{
	ROWCOL nCol;
	T_SECT_STIFFNESS* pStiff;
	for (int i = 0; i < GetSectCount(); i++)
	{
		GetStiffAndPeri(i, pStiff);

		nCol = i + 1;
		pStiff->Area = _ttof(m_wndGrid.GetEditingValue(1, nCol));
		pStiff->Asy = _ttof(m_wndGrid.GetEditingValue(2, nCol));
		pStiff->Asz = _ttof(m_wndGrid.GetEditingValue(3, nCol));
		pStiff->Rxx = _ttof(m_wndGrid.GetEditingValue(4, nCol));
		pStiff->Ryy = _ttof(m_wndGrid.GetEditingValue(5, nCol));
		pStiff->Rzz = _ttof(m_wndGrid.GetEditingValue(6, nCol));

		// 추가분 
		pStiff->Cyp = _ttof(m_wndGrid.GetEditingValue(7, nCol));
		pStiff->Cym = _ttof(m_wndGrid.GetEditingValue(8, nCol));
		pStiff->Czp = _ttof(m_wndGrid.GetEditingValue(9, nCol));
		pStiff->Czm = _ttof(m_wndGrid.GetEditingValue(10, nCol));
		pStiff->Qyb = _ttof(m_wndGrid.GetEditingValue(11, nCol));
		pStiff->Qzb = _ttof(m_wndGrid.GetEditingValue(12, nCol));
		if (m_nType == STIFSCALE)
			pStiff->WArea = _ttof(m_wndGrid.GetEditingValue(13, nCol));
		else
			pStiff->WArea = pStiff->Area;

		pStiff->dx1 = _ttof(m_wndGrid.GetEditingValue(14, nCol));
		pStiff->dy1 = _ttof(m_wndGrid.GetEditingValue(15, nCol));
		pStiff->dx2 = _ttof(m_wndGrid.GetEditingValue(16, nCol));
		pStiff->dy2 = _ttof(m_wndGrid.GetEditingValue(17, nCol));
		pStiff->dx3 = _ttof(m_wndGrid.GetEditingValue(18, nCol));
		pStiff->dy3 = _ttof(m_wndGrid.GetEditingValue(19, nCol));
		pStiff->dx4 = _ttof(m_wndGrid.GetEditingValue(20, nCol));
		pStiff->dy4 = _ttof(m_wndGrid.GetEditingValue(21, nCol));

		pStiff->dIw = _ttof(m_wndGrid.GetEditingValue(22, nCol));
	}
	return TRUE;
}

void CCMStageCompSectStiffDlg::SetColumnStyle()
{
	ROWCOL nStartCol = GetSectCount();
	ROWCOL nEndCol = GetUnitCol() - 1;
	if (nStartCol == nEndCol) return;

	// 2003. 2. 13  수정
	m_wndGrid.HideCols(nStartCol + 1, nEndCol, TRUE);
	/*
	m_wndGrid.SetColWidth(nStartCol+1, nEndCol, 0);
	for (int i = 0; i < 16; i++)
	{
		m_wndGrid.SetCoveredCellsRowCol(i, nStartCol, i, nEndCol);
	}
	*/
}

void CCMStageCompSectStiffDlg::SetDialogSize()
{
	//대화상자를 리사이징하지 않는 조건
	if (!m_bHideUnderQzb && m_nType != STIFSCALE && !CSectDB::IsEnable7thDOF()) return;

	//Grid 컨트롤에 입력된 Data에 의한 폭과 높이를 계산
	int nWidth = 0;
	int nHeight = 0;
	CRect rect, rect2;
	GetWindowRect(rect);
	m_wndGrid.GetWindowRect(rect2);

	if (m_nType == STIFSCALE) // Scale
	{
		const int colWidth = rect2.Width() - 54 - 5; // Total Width - 54(Name Column) - 5(Margin)
		m_wndGrid.SetColWidth(1, 1, globalUtils.ScaleByDPI(colWidth));
	}
	else if (m_nType == STIFFNESS) // Stiffness
	{
		const int colWidth = rect2.Width() - 54 - 54 - 5; // Total Width - 54(Name Column) - 54(Unit Column) - 5(Margin)

		m_wndGrid.SetColWidth(1, 1, globalUtils.ScaleByDPI(colWidth));
		m_wndGrid.SetColWidth(2, 2, globalUtils.ScaleByDPI(0));
		m_wndGrid.SetColWidth(3, 3, globalUtils.ScaleByDPI(54));
	}
	else if (m_nType == STIFTAPER) // Tapered Stiffness
	{
		int colWidth = rect2.Width() - 54 - 54 - 5; // Total Width - 54(Name Column) - 54(Unit Column) - 5(Margin)
		colWidth = colWidth / 2;

		m_wndGrid.SetColWidth(1, 1, globalUtils.ScaleByDPI(colWidth));
		m_wndGrid.SetColWidth(2, 2, globalUtils.ScaleByDPI(colWidth));
		m_wndGrid.SetColWidth(3, 3, globalUtils.ScaleByDPI(54));
	}
	else if (m_nType == 4) // Stiffness 이면서 MultipleElastic 존재
	{
		int colWidth = rect2.Width() - 54 - 54 - 5; // Total Width - 54(Name Column) - 54(Unit Column) - 5(Margin)
		colWidth = colWidth / 3;

		m_wndGrid.SetColWidth(1, 1, globalUtils.ScaleByDPI(colWidth));
		m_wndGrid.SetColWidth(2, 2, globalUtils.ScaleByDPI(colWidth));
		m_wndGrid.SetColWidth(3, 3, globalUtils.ScaleByDPI(colWidth));
		m_wndGrid.SetColWidth(4, 4, globalUtils.ScaleByDPI(54));
	}
	else ASSERT(false);

	for (int i = 0; i <= m_wndGrid.GetRowCount(); i++)
		nHeight += m_wndGrid.GetRowHeight(i);
	nHeight += globalUtils.ScaleByDPI(5);
	
	//Gird 컨트롤을 Data에 의해 계산된 크기로 리사이징 (스크롤바를 제거하여 보기 좋게 하기 위해서)
	rect2.bottom = rect2.top + nHeight;
	ScreenToClient(rect2);
	m_wndGrid.MoveWindow(rect2);

	// Button Position 계산
	CRect rRef, rRefY;
	CRect rToMove;
	GetDlgItem(IDC_CMD_SP_SD_GRID)->GetWindowRect(rRef);

	CArray<UINT, UINT> aIxxCrtl;
	aIxxCrtl.Add(IDC_CMD_SP_SD_IXX_CHK);
	GetDlgItem(IDC_CMD_SP_SD_IXX_CHK)->GetWindowRect(rToMove);
	int nDistY = rRef.bottom - rToMove.top + globalUtils.ScaleByDPI(6);
	CDlgUtil::CtrlMoveDistY(this, aIxxCrtl, nDistY);

	if (m_bCompNormalPar2)
	{
		GetDlgItem(IDC_CMD_SP_SD_IXX_CHK)->GetWindowRect(rRefY);
	}
	else
	{
		rRefY = rRef;
	}

	GetDlgItem(IDC_CMD_SP_SD_BTN_CANCEL)->GetWindowRect(rToMove);
	nDistY = rRefY.bottom - rToMove.top + globalUtils.ScaleByDPI(16);

	CArray<UINT, UINT> aBtnGroup;
	aBtnGroup.Add(IDC_CMD_SP_SD_BTN_IMPORT);
	aBtnGroup.Add(IDC_CMD_SP_SD_BTN_OK);
	aBtnGroup.Add(IDC_CMD_SP_SD_BTN_CANCEL);
	CDlgUtil::CtrlMoveDistY(this, aBtnGroup, nDistY);

	GetDlgItem(IDC_CMD_SP_SD_BTN_CANCEL)->GetWindowRect(rRefY);
	rect.bottom = rRefY.bottom + globalUtils.ScaleByDPI(8);
	MoveWindow(rect);
}

void CCMStageCompSectStiffDlg::SetGridReadOnly(ROWCOL nFromCol, ROWCOL nToCol)
{
	m_wndGrid.SetStyleRange(CGXRange().SetCols(nFromCol, nToCol),
		CGXStyle().SetReadOnly(TRUE));
}

BEGIN_MESSAGE_MAP(CCMStageCompSectStiffDlg, CDialogMove)
	//{{AFX_MSG_MAP(CCMStageCompSectStiffDlg)
	ON_BN_CLICKED(IDC_CMD_SP_SD_BTN_OK, OnCmdSpSdBtnOk)
	ON_BN_CLICKED(IDC_CMD_SP_SD_BTN_CANCEL, OnCmdSpSdBtnCancel)
	ON_BN_CLICKED(IDC_CMD_SP_SD_BTN_IMPORT, OnCmdSpSdBtnImport)
	ON_BN_CLICKED(IDC_CMD_SP_SD_IXX_CHK, OnCmdIxxCheck)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMStageCompSectStiffDlg message handlers

BOOL CCMStageCompSectStiffDlg::OnInitDialog()
{
	CDialogMove::OnInitDialog();

	// TODO: Add extra initialization here
	ROWCOL nUnitCol = GetUnitCol();

	m_wndGrid.Initialize();
	I_GridColorMgr::ApplyColorStyle(&m_wndGrid);
	m_wndGrid.LockUpdate(TRUE); // 설정중 화면 update를 막는다.
	m_wndGrid.GetParam()->EnableUndo(FALSE);  // undo를 금지시킨다.
	m_wndGrid.SetRowCount(D_ROW_NUMBER);
	m_wndGrid.SetColCount(nUnitCol);   // Column 카운터 3개

	SetTitleInGrid(); // 타이틀 이름을 설정한다.
	SetUnitInGrid();  // 단위계 이름을 설정한다.

	// Grid의 style을 변경한다.
	m_wndGrid.ChangeStandardStyle(CGXStyle()
		.SetFont(CGXFont_GC().SetFaceName(_LS(IDS_CMD_FONT_Ms_Sans_Serif)).SetSize(9))
		.SetVerticalAlignment(DT_VCENTER)
		.SetAllowEnter(FALSE));

	// column 크기를 맞춘다.
	/*
	m_wndGrid.SetColWidth(0, 0, 54);
	m_wndGrid.SetColWidth(1, nUnitCol-1, 114);
	m_wndGrid.SetColWidth(nUnitCol, nUnitCol, 40);
	*/
	m_wndGrid.SetColWidth(0, 0, globalUtils.ScaleByDPI(58));
	m_wndGrid.SetColWidth(1, nUnitCol - 1, globalUtils.ScaleByDPI(110));
	m_wndGrid.SetColWidth(nUnitCol, nUnitCol, globalUtils.ScaleByDPI(54));

	m_wndGrid.SetRowHeight(0, 0, globalUtils.ScaleByDPI(19));
	m_wndGrid.SetRowHeight(1, m_wndGrid.GetRowCount(), globalUtils.ScaleByDPI(16));

	if (!m_bCompNormalPar2)
	{
		m_wndGrid.ColHeaderStyle()
			.SetReadOnly(TRUE).SetEnabled(FALSE);
		m_wndGrid.RowHeaderStyle()
			.SetReadOnly(TRUE).SetEnabled(FALSE);
	}

	if (m_bDumbAndStub)
	{
		m_wndGrid.SetStyleRange(CGXRange().SetCols(1, nUnitCol - 1), CGXStyle().
			SetHorizontalAlignment(DT_RIGHT).
			SetFormat(GX_FMT_FLOAT).SetPlaces(6));
		this->SetWindowText(_LS(IDS_CMD_CSCS__User_Stiffness));

		m_wndGrid.HideRows(14, 22, TRUE);
	}
	else
	{
		if (m_nType == STIFSCALE)  // Scale Factor 입력일때 
		{
			m_wndGrid.SetStyleRange(CGXRange().SetCols(1, nUnitCol - 1), CGXStyle().
				SetHorizontalAlignment(DT_RIGHT).
				SetFormat(GX_FMT_FIXED).SetPlaces(4));
			this->SetWindowText(_LS(IDS_CMD_CSCS__Stiffness_Scale_Factor));
			m_wndGrid.HideRows(7, 12, TRUE);
			m_wndGrid.HideRows(14, 21, TRUE);
		}
		else
		{
			m_wndGrid.SetStyleRange(CGXRange().SetCols(1, nUnitCol - 1), CGXStyle().
				SetHorizontalAlignment(DT_RIGHT).
				SetFormat(GX_FMT_FLOAT).SetPlaces(6));
			this->SetWindowText(_LS(IDS_CMD_CSCS__User_Stiffness));
			m_wndGrid.HideRows(13, 13, TRUE);
		}

		// 20080707 mylee 다른 제목이 지정된 경우에 대화창의 이름을 바꾼다
		if (m_strTitle != _T("")) this->SetWindowText(m_strTitle);

		if (m_bHideUnderQzb) // Composite Scalefactor 일때 부모에서 호출해줌
		{
			m_wndGrid.HideRows(14, 21, TRUE);
		}

		// MNET:XXXX-HSSHIM-20130902. 7자유도인가?
		if (!CSectDB::IsEnable7thDOF()) m_wndGrid.HideRows(22, 22, TRUE);
	}

	m_wndGrid.SetStyleRange(CGXRange().SetCols(nUnitCol),
		CGXStyle().SetReadOnly(TRUE).SetEnabled(FALSE));

	// cell의 width와 height 크기 변경을 금지한다.
	m_wndGrid.GetParam()->EnableTrackRowHeight(FALSE);
	m_wndGrid.GetParam()->EnableTrackColWidth(FALSE);

	// column과 row의 위치 이동을 금지시킨다.
	m_wndGrid.GetParam()->EnableMoveCols(FALSE);
	m_wndGrid.GetParam()->EnableMoveRows(FALSE);

	// double click시 셀 편집이 되도록 한다.
	m_wndGrid.GetParam()->SetActivateCellFlags(GX_CAFOCUS_DBLCLICKONCELL);

	// 현재 셀을 설정한다.
	m_wndGrid.SetCurrentCell(1, 1);

	// Stiffness 값을 설정한다.
	SetStiffnessData();
	SetColumnStyle();  // column size를 설정한다.
	SetDialogSize();  // Dialog의 size를 설정한다.

	// 수정  
	if (m_bReadOnly) SetGridReadOnly(1, nUnitCol - 1);
	else SetGridReadOnly(3, 3);

	m_wndGrid.LockUpdate(FALSE);
	m_wndGrid.Redraw();
	m_wndGrid.SetFocus();

	CWnd* pWnd = GetDlgItem(IDC_CMD_SP_SD_BTN_IMPORT);
	//pWnd->EnableWindow(!m_bReadOnly);
	if (m_nType == STIFSCALE)	pWnd->ShowWindow(FALSE);
	else						pWnd->ShowWindow(!m_bReadOnly);

	GetDlgItem(IDC_CMD_SP_SD_IXX_CHK)->ShowWindow(m_bCompNormalPar2);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}

void CCMStageCompSectStiffDlg::OnCmdSpSdBtnOk()
{
	if (!GetStiffnessData()) return;

	CDialogMove::OnOK();
}

void CCMStageCompSectStiffDlg::OnCmdSpSdBtnCancel()
{
	// TODO: Add your control notification handler code here
	CDialogMove::OnCancel();
}

void CCMStageCompSectStiffDlg::OnCmdSpSdBtnImport()
{
	CCMStageCompSectStiffListDlg dlg(m_nType);

	int nDlg = -1;
	nDlg = dlg.DoModal();

	if (nDlg == IDOK)
	{
		if (m_pData1 != NULL)
			(*m_pData1) = dlg.m_StiffDataI;
		if (m_pData2 != NULL)
			(*m_pData2) = dlg.m_StiffDataJ;

		SetStiffnessData();
	}
}

void CCMStageCompSectStiffDlg::OnCmdIxxCheck()
{
	int nCheck = m_wndIxxCheck.GetCheck();
	ChangeIxxType(nCheck == 1);
}

void CCMStageCompSectStiffDlg::ChangeIxxType(BOOL bIxxChk)
{
	ROWCOL nFromCol, nToCol;
	nFromCol = 1;
	nToCol = GetSectCount();

	BOOL bOldLock = m_wndGrid.LockUpdate(TRUE);

	m_wndGrid.SetStyleRange(CGXRange().SetCols(1, 2),
		CGXStyle().SetReadOnly(FALSE));

	if (bIxxChk)
	{
		m_wndGrid.SetValueRange(CGXRange(4, 0), _T("Ixx'"));
		m_wndGrid.SetValueRange(CGXRange(4, 1), m_pDataAuto->Rxx);
		if (m_nType == STIFTAPER)
		{
			m_wndGrid.SetValueRange(CGXRange(4, 2), m_pDataAutoJ->Rxx);
		}
	}
	else
	{
		m_wndGrid.SetValueRange(CGXRange(4, 0), _T("Ixx"));
		m_wndGrid.SetValueRange(CGXRange(4, 1), m_pData1->Rxx);
		if (m_nType == STIFTAPER)
		{
			m_wndGrid.SetValueRange(CGXRange(4, 2), m_pData2->Rxx);
		}
	}

	m_wndGrid.SetStyleRange(CGXRange().SetCols(1, 2),
		CGXStyle().SetReadOnly(TRUE));

	m_wndGrid.LockUpdate(FALSE);
	m_wndGrid.Redraw();

}
