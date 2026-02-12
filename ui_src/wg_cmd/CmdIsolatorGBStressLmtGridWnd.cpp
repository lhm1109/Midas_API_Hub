// ACS_ResultGridWnd.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CmdIsolatorGBStressLmtGridWnd.h"
#include "CmdIsolatorGBStressLmtTableDlg.h"
#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_base\wg_base_ControlEx.h"
#include "..\wg_base\wg_base_I_GridColorMgr.h"
#include "..\wg_base\wg_base_CompFunc.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_DBLib.h"
#include "..\wg_db\wg_db_FileCtrl.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCmdIsolatorGBStressLmtGridWnd

CCmdIsolatorGBStressLmtGridWnd::CCmdIsolatorGBStressLmtGridWnd()
{
	m_aPrimaryKey.Add(0);
}

CCmdIsolatorGBStressLmtGridWnd::~CCmdIsolatorGBStressLmtGridWnd()
{
}


BEGIN_MESSAGE_MAP(CCmdIsolatorGBStressLmtGridWnd, CGXGridWnd)
	//{{AFX_MSG_MAP(CCmdIsolatorGBStressLmtGridWnd)
	ON_WM_RBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CCmdIsolatorGBStressLmtGridWnd message handlers
void CCmdIsolatorGBStressLmtGridWnd::Initialize()
{
	m_pDoc = CDBDoc::GetDocPoint();
	CTBBrowserWndEx::Initialize();

	GetParam()->EnableUndo(FALSE);
	GetParam()->EnableTrackRowHeight(FALSE);
	GetParam()->EnableMoveCols(FALSE);        // columns can not be dragged

	CGXProperties* pProp = GetParam()->GetProperties();
	pProp->SetMarkColHeader(FALSE); // Turn off pressed button effect for column headers
	pProp->SetMarkRowHeader(FALSE);
	pProp->SetUserProperty(GX_IDS_OUTLINECURRENTCELL, // 현재 셀의 border 설정 변경
		(CGXStyle)pProp->sInvertNoBorder);

	LockUpdate(TRUE);
	// 컬럼 갯수 설정	
	
	m_nCol = 4;
	m_nRow = 3;
	SetColCount(m_nCol);
	SetRowCount(m_nRow);
	SetTitle();
	SetHeaderTitle();
	Data2Grid();
	Redraw();

	// 표준 스타일 변경
	ChangeStandardStyle(CGXStyle()
		.SetFont(CGXFont_GC().SetSize(9))
		.SetVerticalAlignment(DT_BOTTOM)
		.SetAllowEnter(FALSE));
	ChangeColHeaderStyle(CGXStyle().SetFont(CGXFont_GC().SetBold(FALSE)));
	ChangeRowHeaderStyle(CGXStyle()
		.SetFont(CGXFont_GC().SetBold(FALSE))
		.SetHorizontalAlignment(DT_RIGHT)
		.SetControl(GX_IDS_CTRL_HEADER));
	GetParam()->SetNumberedRowHeaders(TRUE);

	// 컬럼 스타일 변경
	SetStyleRange(CGXRange().SetCols(2, m_nCol), CGXStyle()
		.SetHorizontalAlignment(DT_CENTER)
		.SetValueType(GX_VT_NUMERIC)
		.SetFormat(GX_FMT_FIXED)
		.SetPlaces(2));

	SetRowHeightByDPI(0, 0, 36);
	SetStyleRange(CGXRange().SetCols(1), CGXStyle()
		.SetHorizontalAlignment(DT_CENTER)
		.SetValueType(GX_VT_NUMERIC)
		.SetFormat(GX_FMT_FIXED)
		.SetEnabled(FALSE));

	// 현재 셀 설정
	SetRightValidCell(1, 0);
	GetParam()->SetHideCurrentCell(GX_HIDE_NEVER);

	SetScrollBarMode(SB_HORZ, gxnEnabled);
	SetScrollBarMode(SB_VERT, gxnEnabled);

	GetParam()->EnableMoveRows(TRUE);

	// Sorting 정보 초기화
	m_aSortInfo.SetSize(1);
	m_aSortInfo[0].nRC = 0;
	m_aSortInfo[0].sortType = CGXSortInfo::numeric;
	m_aSortInfo[0].sortOrder = CGXSortInfo::ascending;

	SetStyleRange(CGXRange().SetCols(1, 1), CGXStyle()
		.SetReadOnly(TRUE));
	SetStyleRange(CGXRange().SetCols(2, GetColCount()), CGXStyle()
		.SetReadOnly(FALSE));

	LockUpdate(FALSE);
	Redraw();
}

void CCmdIsolatorGBStressLmtGridWnd::SetData(T_ISGB_D *pdata)
{
	pIsgbD = pdata;
}

void CCmdIsolatorGBStressLmtGridWnd::SetHeaderTitle()
{
	int nColWidth[10] = { 120, 130, 130, 130};
	CString csTitle;

	CGXStyle style;
	style.SetControl(GX_IDS_CTRL_STATIC);
	style.SetHorizontalAlignment(DT_CENTER);
	style.SetVerticalAlignment(DT_VCENTER);
	style.SetDraw3dFrame(gxFrameRaised);
	style.SetFont(CGXFont_GC().SetBold(FALSE).SetSize(9));
	
	for (int i = 0; i < m_nCol; i++)
	{
		csTitle = m_aTitle[i];
		SetStyleRange(CGXRange(0, i + 1), CGXStyle()
			.SetValue(csTitle)
			.SetUserAttribute(ID_USER_ATTR_COL_ID, m_aTitle[i]));
		SetColWidthByDPI(i + 1, i + 1, nColWidth[i]);
	}
	SetColWidthByDPI(0, 0, 0);
	
}

void CCmdIsolatorGBStressLmtGridWnd::GetTitleUnitName(CString &strUnit_L, CString &strUnit_F)
{
	T_UNIT_INDEX CurIndex;
	m_pDoc->m_pUnitCtrl->GetUnitIndexCurrent(CurIndex);
	int iLenID = CurIndex.nBase_Length;
	int iForID = CurIndex.nBase_Force;

	switch (iLenID)
	{
	case (D_UNITSYS_LENGTH_INDEX_MM): strUnit_L = D_UNITSYS_LENGTH_NAME_MM; break;
	case (D_UNITSYS_LENGTH_INDEX_CM): strUnit_L = D_UNITSYS_LENGTH_NAME_CM; break;
	case (D_UNITSYS_LENGTH_INDEX_M):  strUnit_L = D_UNITSYS_LENGTH_NAME_M;  break;
	case (D_UNITSYS_LENGTH_INDEX_IN): strUnit_L = D_UNITSYS_LENGTH_NAME_IN; break;
	case (D_UNITSYS_LENGTH_INDEX_FT): strUnit_L = D_UNITSYS_LENGTH_NAME_FT; break;
	default:									  
		break;
	}

	switch (iForID)
	{
	case (D_UNITSYS_FORCE_INDEX_KG): strUnit_F = D_UNITSYS_FORCE_NAME_KG; break;
	case (D_UNITSYS_FORCE_INDEX_TON):strUnit_F = D_UNITSYS_FORCE_NAME_TON; break;
	case (D_UNITSYS_FORCE_INDEX_N):  strUnit_F = D_UNITSYS_FORCE_NAME_N;  break;
	case (D_UNITSYS_FORCE_INDEX_KN): strUnit_F = D_UNITSYS_FORCE_NAME_KN; break;
	case (D_UNITSYS_FORCE_INDEX_LBF): strUnit_F = D_UNITSYS_FORCE_NAME_LBF; break;
	case (D_UNITSYS_FORCE_INDEX_KIP): strUnit_F = D_UNITSYS_FORCE_NAME_KIP; break;
	default:
		break;
	}
}

void CCmdIsolatorGBStressLmtGridWnd::SetTitle()
{
	m_aTitle.RemoveAll();
	CString strUnit_L, strUnit_F;
	GetTitleUnitName(strUnit_L, strUnit_F);
	CString strCompressG, strCompressRare, strTensileRare;
	strCompressG.Format(_LS(IDS_CMD_LOAD_ISGB_COMPRESS_LMT_G), strUnit_F, strUnit_L);
	strCompressRare.Format(_LS(IDS_CMD_LOAD_ISGB_COMPRESS_LMT_RARE), strUnit_F, strUnit_L);
	strTensileRare.Format(_LS(IDS_CMD_LOAD_ISGB_TENSILE_LMT_RARE), strUnit_F, strUnit_L);

	CString aTitle1[6] = { _LS(IDS_CMD_LOAD_ISGB_TYPE),  strCompressG, strCompressRare, strTensileRare };
	
	for (int i = 0; i < 4; i++)
	{
		m_aTitle.Add(aTitle1[i]);
	}
}

void CCmdIsolatorGBStressLmtGridWnd::Data2Grid()
{
	CString strType = _T("");
	strType = _LS(IDS_CMD_LOAD_ISGB_TYPE_RUBBER);
	SetValueRange(CGXRange(1, 1), strType);
	strType = _LS(IDS_CMD_LOAD_ISGB_TYPE_ESB);
	SetValueRange(CGXRange(2, 1), strType);
	strType = _LS(IDS_CMD_LOAD_ISGB_TYPE_FPS);
	SetValueRange(CGXRange(3, 1), strType);
	for (int k = 1; k < m_nCol; k++)
	{
		strType.Format(_T("%.3f"), pIsgbD->dLNR[k - 1]);
		SetValueRange(CGXRange(1, k + 1), strType);
		strType.Format(_T("%.3f"), pIsgbD->dESB[k - 1]);
		SetValueRange(CGXRange(2, k + 1), strType);
		strType.Format(_T("%.3f"), pIsgbD->dFPS[k - 1]);
		SetValueRange(CGXRange(3, k + 1), strType);
	}
}

void CCmdIsolatorGBStressLmtGridWnd::Grid2Data()
{
	for (int i = 1; i < m_nCol; i++)
	{
		pIsgbD->dLNR[i - 1] = _ttof(GetValueRowCol(1, i + 1));
		pIsgbD->dESB[i - 1] = _ttof(GetValueRowCol(2, i + 1));
		pIsgbD->dFPS[i - 1] = _ttof(GetValueRowCol(3, i + 1));
	}
}

void CCmdIsolatorGBStressLmtGridWnd::Write_Cell(ROWCOL nRow, ROWCOL nCol, CGXStyle& rstyle, const CString& strValue)
{
	rstyle.SetValue(strValue);
	SetStyleRange(CGXRange(nRow, nCol), rstyle);
}

void CCmdIsolatorGBStressLmtGridWnd::Write_Cell(ROWCOL nRow, ROWCOL nCol, CGXStyle& rstyle, const TCHAR* Fmt, ...)
{
	va_list argptr;
	va_start(argptr, Fmt);

	CString strTemp;
	strTemp.FormatV(Fmt, argptr);
	va_end(argptr);

	Write_Cell(nRow, nCol, rstyle, strTemp);
}

void CCmdIsolatorGBStressLmtGridWnd::Write_CellMerge(ROWCOL nRow1, ROWCOL nCol1, ROWCOL nRow2, ROWCOL nCol2, CGXStyle& rstyle, const CString& strValue)
{
	rstyle.SetValue(strValue);
	SetCoveredCellsRowCol(nRow1, nCol1, nRow2, nCol2);
	SetStyleRange(CGXRange(nRow1, nCol1, nRow2, nCol2), rstyle);
}

void CCmdIsolatorGBStressLmtGridWnd::Write_CellMerge(ROWCOL nRow1, ROWCOL nCol1, ROWCOL nRow2, ROWCOL nCol2, CGXStyle& rstyle, const TCHAR* Fmt, ...)
{
	va_list argptr;
	va_start(argptr, Fmt);

	CString strTemp;
	strTemp.FormatV(Fmt, argptr);
	va_end(argptr);

	Write_CellMerge(nRow1, nCol1, nRow2, nCol2, rstyle, strTemp);
}

void CCmdIsolatorGBStressLmtGridWnd::OnModifyCell(ROWCOL nRow, ROWCOL nCol)
{
	if (IsAppendRow(nRow)) AddNew(nRow);
	else Edit(nRow, TRUE);
	CGXGridWnd::OnModifyCell(nRow, nCol);
	//SetNewLineNo(nRow, nCol);
}

//BOOL CCmdIsolatorGBStressLmtGridWnd::OnPasteFromClipboard(const CGXRange &range)
//{
//	if (CGXGridWnd::OnPasteFromClipboard(range))
//	{
//		return TRUE;
//	}
//	 return FALSE;
//}

CString CCmdIsolatorGBStressLmtGridWnd::GetEditingValue(ROWCOL nRow, ROWCOL nCol)
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
