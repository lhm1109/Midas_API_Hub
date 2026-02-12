// WindAmpFactorDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "WindAmpFactorDlg.h"
#include "..\wg_common\wg_common_TBGrid.h"



// CWindAmpFactorDlg 대화 상자입니다.

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
//////////////////////////



#define D_MAX_ROW 3
#define D_MAX_COL 4

enum RowIndex
{
	ROWINDEX_HEADER = 1,
	ROWINDEX_XDIR,
	ROWINDEX_YDIR,
};

enum ColumnIndex
{
	COLINDEX_HEADER = 1,
	COLINDEX_ALONG,
	COLINDEX_ACROSS,
	COLINDEX_TORSION,
};

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CWindAmpFactorGrid::CWindAmpFactorGrid()
{
	m_nWndWidth = 0;
	m_nWndHeight = 0;
}

//////////////////////////////////////////////////////////////////////
// Interface

void CWindAmpFactorGrid::Initialize(CWnd* pParent)
{
// 	m_pParent = (CMasonryMaterialDlg*)pParent;
	CGXGridWnd::Initialize();

	// cell 바깥영역 배경색 지정 
	GetParam()->GetProperties()->SetColor(GX_COLOR_BACKGROUND, CTBGrid::m_Color.OuterBackground);

	// Header 배경색 지정 
	ChangeColHeaderStyle(CGXStyle().SetInterior(CTBGrid::m_Color.RowColHeaderBg));
	ChangeRowHeaderStyle(CGXStyle().SetInterior(CTBGrid::m_Color.RowColHeaderBg));

	// 기타 Cells 배경색 지정
	ChangeStandardStyle(CGXStyle().SetInterior(CTBGrid::m_Color.NormalModeBg));

	LockUpdate(TRUE);

	// 표준 스타일 변경
	ChangeStandardStyle(CGXStyle()
		.SetFont(CGXFont_GC().SetSize(9))
		.SetHorizontalAlignment(DT_CENTER)
		.SetVerticalAlignment(DT_VCENTER)
		.SetAllowEnter(FALSE));

	// Column header의 스타일 변경
	ChangeColHeaderStyle(CGXStyle().SetFont(CGXFont_GC().SetBold(FALSE)));
	SetRowHeightByDPI(0, 0, 20);

	GetParam()->EnableUndo(FALSE);

	// Row와  Column의 이동을 금지시킨다.
	GetParam()->EnableTrackRowHeight(FALSE);
	GetParam()->EnableTrackColWidth(FALSE);
	GetParam()->EnableMoveRows(FALSE);
	GetParam()->EnableMoveCols(FALSE);

	// double click시 셀 편집이 되도록 한다.
	GetParam()->SetActivateCellFlags(GX_CAFOCUS_DBLCLICKONCELL);

	// MemoryDC를 사용한다.
	//  SetDrawingTechnique(gxDrawUsingMemDC);

	// 헤더의 Current cell을 막는다.
	ColHeaderStyle().SetEnabled(FALSE);
	RowHeaderStyle().SetEnabled(FALSE);

	CGXProperties* pProp = GetParam()->GetProperties();
	pProp->SetMarkColHeader(FALSE); // Turn off pressed button effect for column headers
	pProp->SetMarkRowHeader(FALSE);
	pProp->SetUserProperty(GX_IDS_OUTLINECURRENTCELL, // 현재 셀의 border 설정 변경
		(CGXStyle)pProp->sInvertNoBorder);

	// Grid Number
	SetColCount(D_MAX_COL); // Number of column excluding header
	SetRowCount(D_MAX_ROW); // Number of row excluding header

	// Row_Grid Size
	int nRowHeight = (m_nWndHeight - 3) / (D_MAX_ROW+1);
	SetRowHeightByDPI(0, ROWINDEX_HEADER, int(nRowHeight*1.5));
	SetRowHeightByDPI(ROWINDEX_XDIR, ROWINDEX_YDIR, int(nRowHeight*1.25));
	SetSizeColumn();

	// Set Title
	SetHeaderTitle();
	SetDataColumn();

	HideCols(0, 0, TRUE);
	HideRows(0, 0, TRUE);
	SetScrollBarMode(SB_VERT, gxnDisabled);

	LockUpdate(FALSE);

	Redraw();



// 	CString strValue = _T("-");
// 	CGXStyle style;
// 	style.SetValue(strValue).SetHorizontalAlignment(DT_CENTER).SetEnabled(FALSE).SetReadOnly(TRUE);

// 	SetStyleRange(CGXRange().SetCells(ROWINDEX_LNEV, COLINDEX_COMP_E), style);
// 	SetStyleRange(CGXRange().SetCells(ROWINDEX_LNEV, COLINDEX_TENS_E), style);
// 	SetStyleRange(CGXRange().SetCells(ROWINDEX_LNEV, COLINDEX_SHEAR_E), style);

// 	Data2Grid();

	LockUpdate(FALSE);
}

void CWindAmpFactorGrid::SetSizeColumn()
{
	int nSizeHeader = m_nWndWidth * 0.35;
	int nSizeStrength = m_nWndWidth * 0.65 / 3 -1;

	// Col_Grid Size
	SetColWidth(COLINDEX_HEADER, COLINDEX_HEADER, nSizeHeader);
	SetColWidth(COLINDEX_ALONG, COLINDEX_ALONG, nSizeStrength);
	SetColWidth(COLINDEX_ACROSS, COLINDEX_ACROSS, nSizeStrength);
	SetColWidth(COLINDEX_TORSION, COLINDEX_TORSION, nSizeStrength);
}

void CWindAmpFactorGrid::SetDataColumn()
{
	SetStyleRange(CGXRange().SetCells(ROWINDEX_XDIR, COLINDEX_ALONG, ROWINDEX_YDIR, COLINDEX_TORSION), CGXStyle()
		.SetControl(GX_IDS_CTRL_EDIT)
		.SetValueType(GX_VT_NUMERIC)
		.SetEnabled(TRUE)
		.SetHorizontalAlignment(DT_CENTER)
		.SetVerticalAlignment(DT_VCENTER));
}


void CWindAmpFactorGrid::Data2Grid(const T_WIND_KDS2021* pKDS2021)
{
	for (int i = 0; i < 2; i++)
	{
		SetStyleRange(CGXRange(ROWINDEX_XDIR + i, COLINDEX_ALONG), CGXStyle().SetValue((float)pKDS2021->dAmFactorAlong[i]));
		SetStyleRange(CGXRange(ROWINDEX_XDIR + i, COLINDEX_ACROSS), CGXStyle().SetValue((float)pKDS2021->dAmFactorAcross[i]));
		SetStyleRange(CGXRange(ROWINDEX_XDIR + i, COLINDEX_TORSION), CGXStyle().SetValue((float)pKDS2021->dAmFactorTorsional[i]));
	}
}

BOOL CWindAmpFactorGrid::Grid2Data(T_WIND_KDS2021* pData)
{
	for (int i = 0; i < 2; i++)
	{
		pData->dAmFactorAlong[i]     = GetGridValue(ROWINDEX_XDIR + i, COLINDEX_ALONG);
		pData->dAmFactorAcross[i]    = GetGridValue(ROWINDEX_XDIR + i, COLINDEX_ACROSS);
		pData->dAmFactorTorsional[i] = GetGridValue(ROWINDEX_XDIR + i, COLINDEX_TORSION );
	}
	return TRUE;
}


//////////////////////////////////////////////////////////////////////
// Operation

void CWindAmpFactorGrid::SetHeaderTitle()
{
	int iFontSizeHeader = 8;
	int iFontSizeNormal = 8;
	BOOL bFontBoldHeader = FALSE;
	BOOL bFontBoldNormal = FALSE;

	CGXStyle style;
	style.SetControl(GX_IDS_CTRL_STATIC);
	style.SetDraw3dFrame(gxFrameRaised);
	style.SetFont(CGXFont_GC().SetBold(FALSE).SetSize(9));
	style.SetInterior(ColHeaderStyle().GetInterior());
	style.SetReadOnly(TRUE);

	SetCoveredCellsRowCol(ROWINDEX_HEADER, COLINDEX_HEADER, ROWINDEX_HEADER, COLINDEX_HEADER);
	style.SetValue(_LS(IDS_CMD_ALONG_WIND_DIRECTION));
	SetStyleRange(CGXRange(ROWINDEX_HEADER, COLINDEX_HEADER), style);

	SetCoveredCellsRowCol(ROWINDEX_HEADER, COLINDEX_ALONG, ROWINDEX_HEADER, COLINDEX_ALONG);
	style.SetValue(_LS(IDS_CMD_WIND_DIRECTION_ALONG));
	SetStyleRange(CGXRange(ROWINDEX_HEADER, COLINDEX_ALONG), style);

	SetCoveredCellsRowCol(ROWINDEX_HEADER, COLINDEX_ACROSS, ROWINDEX_HEADER, COLINDEX_ACROSS);
	style.SetValue(_LS(IDS_CMD_WIND_DIRECTION_ACROSS));
	SetStyleRange(CGXRange(ROWINDEX_HEADER, COLINDEX_ACROSS), style);

	SetCoveredCellsRowCol(ROWINDEX_HEADER, COLINDEX_TORSION, ROWINDEX_HEADER, COLINDEX_TORSION);
	style.SetValue(_LS(IDS_WG_CMD_NLLINK_TORSIONAL));
	SetStyleRange(CGXRange(ROWINDEX_HEADER, COLINDEX_TORSION), style);


	style.SetValue(_LS(IDS_WG_CMD__X_Dir));
	SetStyleRange(CGXRange(ROWINDEX_XDIR, COLINDEX_HEADER), style);

	style.SetValue(_LS(IDS_WG_CMD__Y_Dir));
	SetStyleRange(CGXRange(ROWINDEX_YDIR, COLINDEX_HEADER), style);


}



//////////////////////////////////////////////////////////////////////
// Override

BOOL CWindAmpFactorGrid::OnValidateCell(ROWCOL nRow, ROWCOL nCol)
{
	if (CGXGridWnd::OnValidateCell(nRow, nCol))
		return OnValidateField(nRow, nCol);
	else return FALSE;
}

BOOL CWindAmpFactorGrid::OnValidateField(ROWCOL nRow, ROWCOL nCol)
{
	CString value = _T("");

	CGXControl* pControl = GetControl(nRow, nCol);
	if (pControl && pControl->IsActive()) pControl->GetValue(value);

	return ValidateField(value, nCol);
}

BOOL CWindAmpFactorGrid::OnPasteFromClipboard(const CGXRange &range)
{
// 	if (m_nDefineType == D_SEMM_STRMODE_AUTO) return TRUE;

	// 임시로 CGXGridWnd를 하나 만들어 Paste를 한 후 작업을 한다.
	CGXGridWnd x;
	int nPasteMode;
	int nPastedCols;

	ROWCOL nRow, nCol;
	VERIFY(GetCurrentCell(nRow, nCol));

	// 임시 GridWnd에 Paste 작업을 한다.
	int nColCount = GetColCount();

	if (!PasteFromClipboard(range, 0, x, nPasteMode, nPastedCols))
	{
		x.DestroyWindow();
		DisplayWarningText();
		return FALSE;
	}

	CString strValue = _T("");
	int nxRowCount = x.GetRowCount();
	for (int i = 1; i <= nxRowCount; i++)
	{
		// Table에서 레코드 값을 가져온다.
		int nxColCount = x.GetColCount();
		for (int j = 1; j <= nxColCount; j++)
		{
			strValue = x.GetValueRowCol(i, j);
			CGXStyle gxStyle = LookupStyleRowCol(nRow + i - 1, nCol + j - 1);
			if (gxStyle.GetReadOnly()) continue;
			if (!ValidateField(strValue, nCol + i - 1))
			{
				x.DestroyWindow();
				DisplayWarningText();
				return FALSE;
			}
		}
	}

	x.DestroyWindow();

	BOOL bSuccess = CCMGXGridWnd2::OnPasteFromClipboard(range);
	//ChangeEnable();

	return bSuccess;
}

BOOL CWindAmpFactorGrid::PasteFromClipboard(const CGXRange &range, int nKeyCol, CGXGridWnd &x, int &nPasteMode, int &nPastedCols)
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
		SetWarningText(sErrMsg);
		return FALSE;
	}
	return TRUE;
}

BOOL CWindAmpFactorGrid::ValidateField(CString& value, int nColID)
{
	ROWCOL nColCount = GetColCount();
	if (nColID > 1)
	{
		int nErrCode;
		if (!IsValidDoubleNumber(value, nErrCode))
		{
			switch (nErrCode)
			{
			case 1: SetWarningText(_LS(IDS_WG_CMD__ADDD__Invalid_formula_)); break;
			case 2: SetWarningText(_LS(IDS_WG_CMD__ADDD__Character_exists_in_double_value)); break;
			case 3: SetWarningText(_LS(IDS_WG_CMD__ADDD__Value_is_too_big_)); break;
			}
			return FALSE;
		}
	}

	return TRUE;
}

BOOL CWindAmpFactorGrid::GetStyleRowCol(ROWCOL nRow, ROWCOL nCol, CGXStyle& style, GXModifyType mt, int nType)
{
	if (nRow < ROWINDEX_XDIR || nCol < COLINDEX_ALONG) return CCMGXGridWnd::GetStyleRowCol(nRow, nCol, style, mt, nType);
	return CCMGXGridWnd2::GetStyleRowCol(nRow, nCol, style, mt, nType);
}

void CWindAmpFactorGrid::UpdateUnit()
{
	BOOL bOldLock = LockUpdate(TRUE);

	LockUpdate(bOldLock);
	if (!bOldLock) Redraw();
}

// void CWindAmpFactorGrid::ChangeEnable()
// {
// 	BOOL bOldLock = LockUpdate(TRUE);
// 	BOOL bOldReadOnlyLock = GetParam()->IsLockReadOnly();
// 	GetParam()->SetLockReadOnly(FALSE);
// 
// 	const bool bNotUseExpect = IsUseExpectStrn_ZerothStepEval() ? false : true;
// 	const bool bAutoStrn = (m_nDefineType == D_SEMM_STRMODE_AUTO) ? true : false;
// 	SetStyleRange(CGXRange(ROWINDEX_LNEV, COLINDEX_COMP), CGXStyle().SetReadOnly(bAutoStrn));
// 	SetStyleRange(CGXRange(ROWINDEX_LNEV, COLINDEX_COMP_E), CGXStyle().SetReadOnly(bAutoStrn || bNotUseExpect));
// 	SetStyleRange(CGXRange(ROWINDEX_LNEV, COLINDEX_TENS), CGXStyle().SetReadOnly(bAutoStrn));
// 	SetStyleRange(CGXRange(ROWINDEX_LNEV, COLINDEX_TENS_E), CGXStyle().SetReadOnly(bAutoStrn || bNotUseExpect));
// 	SetStyleRange(CGXRange(ROWINDEX_LNEV, COLINDEX_SHEAR), CGXStyle().SetReadOnly(bAutoStrn));
// 	SetStyleRange(CGXRange(ROWINDEX_LNEV, COLINDEX_SHEAR_E), CGXStyle().SetReadOnly(bAutoStrn || bNotUseExpect));
// 
// 	SetStyleRange(CGXRange(ROWINDEX_MFAC, COLINDEX_COMP), CGXStyle().SetReadOnly(bAutoStrn));
// 	SetStyleRange(CGXRange(ROWINDEX_MFAC, COLINDEX_COMP_E), CGXStyle().SetReadOnly(bAutoStrn));
// 	SetStyleRange(CGXRange(ROWINDEX_MFAC, COLINDEX_TENS), CGXStyle().SetReadOnly(bAutoStrn));
// 	SetStyleRange(CGXRange(ROWINDEX_MFAC, COLINDEX_TENS_E), CGXStyle().SetReadOnly(bAutoStrn));
// 	SetStyleRange(CGXRange(ROWINDEX_MFAC, COLINDEX_SHEAR), CGXStyle().SetReadOnly(bAutoStrn));
// 	SetStyleRange(CGXRange(ROWINDEX_MFAC, COLINDEX_SHEAR_E), CGXStyle().SetReadOnly(bAutoStrn));
// 
// 	SetStyleRange(CGXRange(ROWINDEX_PSOV, COLINDEX_COMP), CGXStyle().SetReadOnly(bAutoStrn));
// 	SetStyleRange(CGXRange(ROWINDEX_PSOV, COLINDEX_COMP_E), CGXStyle().SetReadOnly(bAutoStrn));
// 	SetStyleRange(CGXRange(ROWINDEX_PSOV, COLINDEX_TENS), CGXStyle().SetReadOnly(bAutoStrn));
// 	SetStyleRange(CGXRange(ROWINDEX_PSOV, COLINDEX_TENS_E), CGXStyle().SetReadOnly(bAutoStrn));
// 	SetStyleRange(CGXRange(ROWINDEX_PSOV, COLINDEX_SHEAR), CGXStyle().SetReadOnly(bAutoStrn));
// 	SetStyleRange(CGXRange(ROWINDEX_PSOV, COLINDEX_SHEAR_E), CGXStyle().SetReadOnly(bAutoStrn));
// 
// 	LockUpdate(bOldLock);
// 	if (!bOldLock) Redraw();
// 	GetParam()->SetLockReadOnly(bOldReadOnlyLock);
// }

double CWindAmpFactorGrid::GetGridValue(ROWCOL nRow, ROWCOL nCol)
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
				else if (style.GetIncludeControl() && style.GetControl() == GX_IDS_CTRL_EDIT)
				{
					bGet = pControl->GetValue(csNum);
				}
			}
		}
	}
	if (!bGet) csNum = GetValueRowCol(nRow, nCol);
	return _ttof(csNum);
}


//////////////////////////
BEGIN_MESSAGE_MAP(CWindAmpFactorDlg, CDialogMove)
END_MESSAGE_MAP()

CWindAmpFactorDlg::CWindAmpFactorDlg(T_WIND_KDS2021* pKDS2021, CWnd* pParent /*=NULL*/)
	: CDialogMove(CWindAmpFactorDlg::IDD, pParent)
{
	m_pKDS2021 = pKDS2021;
}

void CWindAmpFactorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ETC_WIND_AMPFACTOR_GRD, m_AmpFactorGrid);
}

BOOL CWindAmpFactorDlg::OnInitDialog()
{
	CDialogMove::OnInitDialog();

	InitCtrl();
	Data2Dlg();

	return FALSE;
}

void CWindAmpFactorDlg::InitCtrl()
{
	CRect rect;
	GetDlgItem(IDC_ETC_WIND_AMPFACTOR_GRD)->GetWindowRect(rect);

	m_AmpFactorGrid.SetGridWndSize(rect.Width(), rect.Height());
	m_AmpFactorGrid.Initialize(this);
}

BOOL CWindAmpFactorDlg::Data2Dlg()
{
	/////////////// List Loading ///////////////
	m_AmpFactorGrid.Data2Grid(m_pKDS2021);
	return TRUE;
}
BOOL CWindAmpFactorDlg::Dlg2Data()
{
	return m_AmpFactorGrid.Grid2Data(m_pKDS2021);
}

void CWindAmpFactorDlg::OnOK()
{
	Dlg2Data();
	CDialogMove::OnOK();
}