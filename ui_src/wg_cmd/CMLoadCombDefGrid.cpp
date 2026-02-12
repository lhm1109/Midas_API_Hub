// SpfcItemGrid.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMLoadCombDefGrid.h"

#include "..\wg_base\wg_base_CompFunc.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "CMLoadCombDefPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define D_MAX_COL 17
CCMLoadCombDefGrid::CCMLoadCombDefGrid(BOOL bReset)
{
	m_pDoc = CDBDoc::GetDocPoint();

	AddColInfo(_LS(IDS_WG_CMD__ADDD__No),       t_real, 0);

	m_aPrimaryKey.Add(0);
	m_bInit = bReset;
}

CCMLoadCombDefGrid::~CCMLoadCombDefGrid()
{
}

/////////////////////////////////////////////////////////////////////////////
// CCMLoadCombDefGrid message handlers

void CCMLoadCombDefGrid::Initialize(int nIdx, CLoadCombDefineData *pdata, BOOL bInit)
{
	CDBDoc *pDoc = CDBDoc::GetDocPoint();	
	CTBBrowserWndEx::Initialize();

	GetParam()->EnableUndo(FALSE);
	GetParam()->EnableTrackRowHeight(FALSE);  
	GetParam()->EnableMoveCols(FALSE);        // columns can not be dragged

	CGXProperties* pProp = GetParam()->GetProperties();
	pProp->SetMarkColHeader(FALSE); // Turn off pressed button effect for column headers
	pProp->SetMarkRowHeader(FALSE);
	pProp->SetUserProperty(GX_IDS_OUTLINECURRENTCELL, // 현재 셀의 border 설정 변경
												(CGXStyle) pProp->sInvertNoBorder);

	LockUpdate(TRUE);
	T_UNIT_SYSTEM UnitSystem;
	// 컬럼 갯수 설정	
	m_pdata = pdata;
	if (bInit)
	{
		ILoadCombDefineMgr::Instance()->getCodeData(nIdx, m_pdata->m_item, m_nRow, m_nCol);
		m_aFactor = m_pdata->m_item.m_aFactor;
	}
	else
	{
		m_aFactor = m_pdata->m_item.m_aFactor;
		m_nCol = m_pdata->m_item.m_aType.GetCount();
		m_nRow = m_pdata->m_item.m_aFactor.GetCount();
	}
	SetColCount(m_nCol+1);
	SetRowCount(m_nRow);
	SetTitle();
	SetHeaderTitle(UnitSystem, TRUE);
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

	SetStyleRange(CGXRange().SetCols(1, GetColCount()), CGXStyle()
		.SetReadOnly(FALSE));

	LockUpdate(FALSE);
	Redraw();
}

void CCMLoadCombDefGrid::SetHeaderTitle(T_UNIT_SYSTEM &UnitSystem, BOOL bInit)
{
	if(bInit == FALSE)    return;

	int nSize = 45;

	CString csTitle;
	int nColCount = GetColCount();
	for (int i = 0; i < nColCount; i++)
	{
		csTitle = m_aTitle[i];
		SetStyleRange(CGXRange(0, i+1), CGXStyle()
			.SetValue(csTitle)
			.SetUserAttribute(ID_USER_ATTR_COL_ID, m_aTitle[i]));
		if (bInit) SetColWidthByDPI(i+1, i+1, nSize);
	}
	SetColWidthByDPI(0,0,0);
}

void CCMLoadCombDefGrid::SetTitle()
{
	// Set Title
	m_aTitle.RemoveAll();

	CString aTitle[] = {_T("D"), _T("L"), _T("LR"), _T("W"), _T("E"), _T("EVT"), _T("S"), _T("T"), _T("PS"), _T("EH"), _T("EV"), _T("WP"), _T("FP"), _T("SF"), _T("B")};

	m_aTitle.Add(_LS(IDS_WG_CMD__ADDD__Key)); 
	for (int i = 0; i < m_nCol; i++)
	{
		m_aTitle.Add(aTitle[i]);
	}
}

void CCMLoadCombDefGrid::Data2Grid()
{
	CString strType;
	
	for (int k=0; k<m_nRow; k++)
	{
		strType.Format(_T("%d"),k+1);
		SetValueRange(CGXRange(k+1, 1), strType);
		for (int i=0; i<m_nCol; i++)
		{
			strType.Format(_T("%.2f"),m_aFactor[k][i]);
			SetValueRange(CGXRange(k+1, i+2),  strType);
		}
	}
	AddNew(m_nRow);
}

void CCMLoadCombDefGrid::Grid2Data()
{
	m_pdata->m_item.m_aFactor.RemoveAll();

	for(int k = 0; k < m_nRow; k++)
	{
		CArrayEx<double, double> arFactor;
		CString strVal;
		double dVal;
		for (int i = 0; i< m_nCol; i++)
		{
			strVal = GetValueRowCol(k+1, i+1);
			dVal = strVal==_T("") ? 0.0 : _tstof(strVal);
			arFactor.Add(dVal);
		}
		m_pdata->m_item.m_aFactor.Add(arFactor);
	}
}

BOOL CCMLoadCombDefGrid::OnDeleteRecords(CRowColArray &awRows)
{
if (!DeleteRecord(awRows))
	{
		SetWarningText(_T("Error : Can't delete data."));
		return FALSE;
	}
	CString strType;
	m_nRow = GetRowCount();
	for (int k = 0; k < m_nRow-1; k++)
	{
		strType.Format(_T("%d"), k + 1);
		SetValueRange(CGXRange(k + 1, 1), strType);
	}
	return TRUE;
}

BOOL CCMLoadCombDefGrid::OnPasteFromClipboard(const CGXRange &range)
{
	return TRUE;
}

void CCMLoadCombDefGrid::OnModifyCell(ROWCOL nRow, ROWCOL nCol)
{
	if (IsAppendRow(nRow)) AddNew(nRow);
	else Edit(nRow, TRUE);
	CGXGridWnd::OnModifyCell(nRow, nCol);
	SetNewLineNo(nRow, nCol);
}

void CCMLoadCombDefGrid :: SetNewLineNo(ROWCOL nRow, ROWCOL nCol)
{
	CString strLineNo;
	strLineNo.Format(_T("%d"), nRow);
	SetValueRange(CGXRange(nRow, 1), strLineNo);
}
