// CMLoadCombGrid.cpp : implementation file
//
// 2004. 02. 17    by TAE
/////////////////////////////////////////////////////////////////////////////
//   1       2       3       4      5      6       7      8     9     10      11     12          13
// --------------------------------------------------------------------------------------------------------+
// LOAD | CENTER |  GIRDER/LINE |    CROSS    | MEDIAL STRIP |     LOAD    | LOAD | LOAD | MAIN LOAD RANGE |
//      |        | -------------|-------------|--------------|-------------|      | UNIT |                 | 
// TYPE |   LINE | START |  END | START | END |  START | END | START | END |      |      |                 |
// --------------------------------------------------------------------------------------------------------+

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMGildBaseGrid.h"

#include "..\wg_base\wg_base_CompFunc.h"

#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_ViewBuff.h"
#include "..\wg_db\wg_db_UnitCtrl.h"

#include "CMLoadCombMainGrid.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define D_GILD_LOAD_TYPE_PONT   _LS(IDS_CMD_LOAD_TYPE_POINT)
#define D_GILD_LOAD_TYPE_LINE   _LS(IDS_CMD_LOAD_TYPE_LINE)
#define D_GILD_LOAD_TYPE_AREA   _LS(IDS_CMD_LOAD_TYPE_AREA)
#define D_GILD_NONE             _LS(IDS_WG_CMD__ADDD__None)

/////////////////////////////////////////////////////////////////////////////
// CCMGildBaseGrid

CCMGildBaseGrid::CCMGildBaseGrid()
{
	m_pDoc = 0;
	m_pData = 0;

	m_nColCount = 13;
	m_nColHeader = 1;   // extra column header(+default header 1)
	m_aColInfo.RemoveAll();
}

CCMGildBaseGrid::~CCMGildBaseGrid()
{
}

//--------------------------------------------------------------------------
// Interface
void CCMGildBaseGrid::Initialize(CDBDoc* pDoc, T_GILD_D *pData)
{
	m_pDoc = pDoc;
	m_pData = pData;
	CTBBrowserWnd::Initialize();
	SetColInfo();

	GetParam()->EnableMoveCols(FALSE);  // Disable moving column
	GetParam()->EnableMoveRows(FALSE);   // Disable moving row
	GetParam()->EnableTrackRowHeight(FALSE);  // Disable resizing row height
	//GetParam()->EnableSelection(FALSE);

	SetScrollBarMode(SB_VERT, gxnEnabled | gxnEnhanced);

	EnableInsertRowByPressingKey(FALSE);  // Disable inserting row by key
	MakeItemEx();
}
void CCMGildBaseGrid::Setdata(int nComboBoxIndex)
{
//   CString aType[] = { 
//     _LS(IDS_CMD_GILD_L_LOAD_TYPE_A), 
//     _LS(IDS_CMD_GILD_L_LOAD_TYPE_B), 
//     _LS(IDS_CMD_GILD_T_LOAD_A),   
//     _LS(IDS_CMD_GILD_T_LOAD_B),   
//   };
//   UINT aData[] = { 0, 1, 2, 4};

	LockUpdate(TRUE);
	BOOL bHIde = TRUE;
	if     (nComboBoxIndex==0 || nComboBoxIndex==1) bHIde = FALSE;
	else if(nComboBoxIndex==2 || nComboBoxIndex==4) bHIde = TRUE;
	else ASSERT(0);
	HideColsByIntend(13, 13, bHIde);
	LockUpdate(FALSE);
	Redraw();

	ROWCOL ncRow = GetRowCount();
	ChangeColumnStatus(ncRow,13);
}

void CCMGildBaseGrid::MakeItemEx()
{
	CWaitCursor wait;

	BOOL bOldLock = LockUpdate(TRUE);

	int nRecCount = m_pData->aData.GetSize();
	SetRowCount(m_nColHeader);
	SetRecordCount(m_nColHeader+nRecCount);

	T_GILD_BASE data;
	for (int i=0; i<nRecCount; i++)
	{
		data = m_pData->aData.GetAt(i);
		SetValue((ROWCOL)(i+1+m_nColHeader), i, data);
	}

	SetRightValidCell(1+m_nColHeader, 1);

	LockUpdate(bOldLock);
	if (!bOldLock) Redraw();
}

void CCMGildBaseGrid::UpdateBuffer(int nCmd, CArray<int, int> &aOldIndex, CArray<int, int> &aIndex, CArray<T_GILD_BASE, T_GILD_BASE&> &aData)
{
	// 현재 Row, Col을 저장 : 나중에 복구하기 위함
	MakeItemEx();
	/*
	ROWCOL ncRow, ncCol;
	GetCurrentCell(ncRow, ncCol);

	BOOL bOldLock = LockUpdate(TRUE);

	int nSize = aIndex.GetSize();
	for (int i = 0; i < nSize; i++)
	{
		switch(nCmd)
		{
		case(0):
				// 삽입될 위치(nRow)를 찾아 넣는다.
				InsertRecordByKey(aIndex[i], aData[i]);
				break;
		case(1):
				// 삭제될 위치(nRow)를 찾아서 지운다.
				DeleteRecordByKey(aIndex[i], aData[i]);
				break;
		case(2):
				DeleteRecordByKey(aOldIndex[i], aData[i]);
				InsertRecordByKey(aIndex[i], aData[i]);
				break;
		default:
			break;
		}
	} // end of while

	// 원래 row, col을 복구하려고 시도
	unsigned int nOldMode = m_nEditMode;
	if (nOldMode != CTBBrowserWnd::addnew)
	{ // addnew 상태에서 복구하면 추가되는 Row가 삭제(CancelEdit에서)된다.
		m_nEditMode = CTBBrowserWnd::noMode;
		if (GetRowCount() < ncRow) ncRow = GetRowCount();
		BOOL bSet = SetCurrentCell(ncRow, ncCol);
		if (!bSet) bSet = SetRightValidCell(ncRow, ncCol);
		if (!bSet) bSet = SetLeftValidCell(ncRow, ncCol);
		m_nEditMode = nOldMode;
	}

	LockUpdate(bOldLock);
	if (!bOldLock) Redraw();
	*/
}

BOOL CCMGildBaseGrid::IsUsedColumn(ROWCOL nCol)
{
	int i;
	BOOL bCol[13];
	for (i=0; i<13; i++) bCol[i] = FALSE;

	for (i=0; i<m_aColInfo.GetSize(); i++)
	{
		if (m_aColInfo[i] == D_GILD_LOAD_TYPE_COL) bCol[0] = TRUE;
		else if (m_aColInfo[i] == D_GILD_CENT_LINE_COL) bCol[1] = TRUE;
		else if (m_aColInfo[i] == D_GILD_GIRD_LINE_COL) bCol[2] = bCol[3] = TRUE;
		else if (m_aColInfo[i] == D_GILD_CROS_LINE_COL) bCol[4] = bCol[5] = TRUE;
		else if (m_aColInfo[i] == D_GILD_MEDN_STRP_COL) bCol[6] = bCol[7] = TRUE;
		else if (m_aColInfo[i] == D_GILD_LOAD_STED_COL) bCol[8] = bCol[9] = TRUE;
		else if (m_aColInfo[i] == D_GILD_LOAD_VALU_COL) bCol[10] = TRUE;
		else if (m_aColInfo[i] == D_GILD_LOAD_UNIT_COL) bCol[11] = TRUE;
		else if (m_aColInfo[i] == D_GILD_MAIN_LOAD_COL) bCol[12] = TRUE;
		else ASSERT(0);
	}

	return bCol[nCol-1];
}

void CCMGildBaseGrid::SetLoadTypeColumn(ROWCOL nCol)
{  
	CString csChoiceList;
	csChoiceList.Format(_T("%s\n%s\n%s\n"), 
		D_GILD_LOAD_TYPE_PONT, D_GILD_LOAD_TYPE_LINE, D_GILD_LOAD_TYPE_AREA);
	SetStyleRange(CGXRange().SetCols(nCol), CGXStyle()
			.SetControl(GX_IDS_CTRL_TABBED_COMBOBOX)
			.SetChoiceList(csChoiceList)
			.SetUserAttribute(GX_IDS_UA_TABLIST_KEYCOL, _T("0"))
			.SetUserAttribute(GX_IDS_UA_TABLIST_TEXTCOL, _T("0"))
			.SetUserAttribute(GX_IDS_UA_TABLIST_SHOWALLCOLS, _T("1"))
			.SetValueType(GX_VT_STRING)
			.SetHorizontalAlignment(DT_LEFT)
			.SetValue(_T(""))
			);
}

void CCMGildBaseGrid::SetLoadLineColumn(ROWCOL nCol)
{
	CString csChoiceList(_T(""));

	CArray<T_LDLN_K, T_LDLN_K> aKey;
	m_pDoc->m_pAttrCtrl->GetLdlnKeyList(aKey);
	int nSize = aKey.GetSize();

	T_LDLN_D data;
	for (int i=0; i<nSize; i++)
	{
		m_pDoc->m_pAttrCtrl->GetLdln(aKey[i], data);
		csChoiceList += data.Name;
		csChoiceList += _T("\n");    
	}
	SetStyleRange(CGXRange().SetCols(nCol), CGXStyle()
			.SetControl(GX_IDS_CTRL_TABBED_COMBOBOX)
			.SetChoiceList(csChoiceList)
			.SetUserAttribute(GX_IDS_UA_TABLIST_KEYCOL, _T("0"))
			.SetUserAttribute(GX_IDS_UA_TABLIST_TEXTCOL, _T("0"))
			.SetUserAttribute(GX_IDS_UA_TABLIST_SHOWALLCOLS, _T("1"))
			.SetValueType(GX_VT_STRING)
			.SetHorizontalAlignment(DT_LEFT)
			.SetValue(_T(""))
			);
}

void CCMGildBaseGrid::SetGirdLineColumn(ROWCOL nCol)
{
	CString csChoiceList(_T(""));
	CString csItem;

	// main girder +  Load Line
	T_MGDR_D MgdrD;
	CArray<T_MGDR_K, T_MGDR_K> aMgdrK;
	m_pDoc->m_pAttrCtrl->GetMgdrKeyList(aMgdrK);
	int nMgdr = aMgdrK.GetSize();
	for (int i=0; i<nMgdr; i++)
	{
		m_pDoc->m_pAttrCtrl->GetMgdr(aMgdrK[i], MgdrD);
		csItem.Format(_T("G:%s\n"), MgdrD.Name);
		csChoiceList += csItem;
	}

	T_LDLN_D LnldD;
	CArray<T_LDLN_K, T_LDLN_K> aLnldK;
	m_pDoc->m_pAttrCtrl->GetLdlnKeyList(aLnldK);
	int nLnld = aLnldK.GetSize();
	for (int i=0; i<nLnld; i++)
	{
		m_pDoc->m_pAttrCtrl->GetLdln(aLnldK[i], LnldD);
		// pig csItem.Format(_T("L:%s\n"), MgdrD.Name);
		csItem.Format(_T("L:%s\n"), LnldD.Name);
		csChoiceList += csItem;
	}
	SetStyleRange(CGXRange().SetCols(nCol), CGXStyle()
			.SetControl(GX_IDS_CTRL_TABBED_COMBOBOX)
			.SetChoiceList(csChoiceList)
			.SetUserAttribute(GX_IDS_UA_TABLIST_KEYCOL, _T("0"))
			.SetUserAttribute(GX_IDS_UA_TABLIST_TEXTCOL, _T("0"))
			.SetUserAttribute(GX_IDS_UA_TABLIST_SHOWALLCOLS, _T("1"))
			.SetValueType(GX_VT_STRING)
			.SetHorizontalAlignment(DT_LEFT)
			.SetValue(_T(""))
			);
}

void CCMGildBaseGrid::SetCrosLineColumn(ROWCOL nCol)
{
	CString csChoiceList(_T(""));

	T_CBEM_D data;
	CArray<T_CBEM_K, T_CBEM_K> aKey;
	m_pDoc->m_pAttrCtrl->GetCbemKeyList(aKey);
	int nSize = aKey.GetSize();
	for (int i=0; i<nSize; i++)
	{
		m_pDoc->m_pAttrCtrl->GetCbem(aKey[i], data);
		csChoiceList += data.Name;
		csChoiceList += _T("\n");    
	}

	SetStyleRange(CGXRange().SetCols(nCol), CGXStyle()
			.SetControl(GX_IDS_CTRL_TABBED_COMBOBOX)
			.SetChoiceList(csChoiceList)
			.SetUserAttribute(GX_IDS_UA_TABLIST_KEYCOL, _T("0"))
			.SetUserAttribute(GX_IDS_UA_TABLIST_TEXTCOL, _T("0"))
			.SetUserAttribute(GX_IDS_UA_TABLIST_SHOWALLCOLS, _T("1"))
			.SetValueType(GX_VT_STRING)
			.SetHorizontalAlignment(DT_LEFT)
			.SetValue(_T(""))
			);
}

void CCMGildBaseGrid::SetMedlStrpColumn(ROWCOL nCol, BOOL bNone)
{
	CString csChoiceList(_T(""));
	if (bNone)
	{
		csChoiceList += D_GILD_NONE;
		csChoiceList += _T("\n");
	}

	CArray<T_LDLN_K, T_LDLN_K> aKey;
	m_pDoc->m_pAttrCtrl->GetLdlnKeyList(aKey);
	int nSize = aKey.GetSize();

	T_LDLN_D data;
	for (int i=0; i<nSize; i++)
	{
		m_pDoc->m_pAttrCtrl->GetLdln(aKey[i], data);
		csChoiceList += data.Name;
		csChoiceList += _T("\n");    
	}
	SetStyleRange(CGXRange().SetCols(nCol), CGXStyle()
			.SetControl(GX_IDS_CTRL_TABBED_COMBOBOX)
			.SetChoiceList(csChoiceList)
			.SetUserAttribute(GX_IDS_UA_TABLIST_KEYCOL, _T("0"))
			.SetUserAttribute(GX_IDS_UA_TABLIST_TEXTCOL, _T("0"))
			.SetUserAttribute(GX_IDS_UA_TABLIST_SHOWALLCOLS, _T("1"))
			.SetValueType(GX_VT_STRING)
			.SetHorizontalAlignment(DT_LEFT)
			.SetValue(_T(""))
			);
}

int  CCMGildBaseGrid::GetLoadTypeData(CString value)
{
	if (value == D_GILD_LOAD_TYPE_PONT) return 0;
	if (value == D_GILD_LOAD_TYPE_LINE) return 1;
	if (value == D_GILD_LOAD_TYPE_AREA) return 2;
	return -1;
}

CString CCMGildBaseGrid::GetLoadTypeStr(int data)
{
	if (data == 0) return D_GILD_LOAD_TYPE_PONT;
	if (data == 1) return D_GILD_LOAD_TYPE_LINE;
	if (data == 2) return D_GILD_LOAD_TYPE_AREA;
	return _T("");
}

UINT CCMGildBaseGrid::GetLoadLineData(CString value)
{
	return m_pDoc->m_pAttrCtrl->GetLdlnKey(value);
}

CString CCMGildBaseGrid::GetLoadLineStr(int data)
{
	T_LDLN_D LdlnD;
	LdlnD.Initialize();
	if (!m_pDoc->m_pAttrCtrl->GetLdln(data, LdlnD)) return _T("");
	return LdlnD.Name;
}

UINT CCMGildBaseGrid::GetGirdLineData(CString value, BOOL &bGirder)
{
	// main girder + load line
	// 구분이 필요하다. -> G:Name, L:Name
	if (value.IsEmpty()) return -1;

	TCHAR ch = value.GetAt(0);
	value = value.Right(value.GetLength()-2);
	if (ch == 'G')
	{
		bGirder = TRUE;
		return m_pDoc->m_pAttrCtrl->GetMgdrKey(value);
	}
	if (ch == 'L')
	{
		bGirder = FALSE;
		return m_pDoc->m_pAttrCtrl->GetLdlnKey(value);
	}
	return -1;
}

CString CCMGildBaseGrid::GetGirdLineStr(int data, BOOL bGirder)
{
	// main girder + load line
	// 구분이 필요하다. -> G:Name, L:Name
	CString csName(_T(""));
	if (bGirder)
	{
		T_MGDR_D MgrdD;
		m_pDoc->m_pAttrCtrl->GetMgdr(data, MgrdD);
		csName.Format(_T("G:%s"), MgrdD.Name);
	}
	else 
	{
		T_LDLN_D LdlnD;
		m_pDoc->m_pAttrCtrl->GetLdln(data, LdlnD);
		csName.Format(_T("L:%s"), LdlnD.Name);
	}
	return csName;
}

UINT CCMGildBaseGrid::GetCrosLineData(CString value)
{
	return m_pDoc->m_pAttrCtrl->GetCbemKey(value);
}

CString CCMGildBaseGrid::GetCrosLineStr(int data)
{
	T_CBEM_D CbemD;
	if (!m_pDoc->m_pAttrCtrl->GetCbem(data, CbemD)) return _T("");
	return CbemD.Name;
}

UINT CCMGildBaseGrid::GetMedlStrpData(CString value)
{
	if (value == D_GILD_NONE) return 0;

	return m_pDoc->m_pAttrCtrl->GetLdlnKey(value);
}

CString CCMGildBaseGrid::GetMedlStrpStr(int data)
{
	if (data == 0) return D_GILD_NONE;

	T_LDLN_D LdlnD;
	LdlnD.Initialize();
	if (!m_pDoc->m_pAttrCtrl->GetLdln(data, LdlnD)) return D_GILD_NONE;
	return LdlnD.Name;
}
//--------------------------------------------------------------------------
// Implementation
void CCMGildBaseGrid::SetColInfo()
{
	int nColNo = m_aColInfo.GetSize();

	BOOL bOldLock = LockUpdate(TRUE);

	// Set Title Header
	SetHeaderTitle();

	// 표준 스타일 변경
	ChangeStandardStyle(CGXStyle()
			.SetFont(CGXFont_GC().SetSize(10))
			.SetVerticalAlignment(DT_BOTTOM)
			.SetAllowEnter(FALSE));

	// 컬럼 스타일 변경
	SetLoadTypeColumn(1);   // load type

	SetLoadLineColumn(2);   // center line

	SetGirdLineColumn(3);   // main girder
	SetGirdLineColumn(4);   // main girder

	SetCrosLineColumn(5);   // cross
	SetCrosLineColumn(6);   // cross

	SetMedlStrpColumn(7, TRUE);   // medial strip
	SetMedlStrpColumn(8, FALSE);  // medial strip
	SetStyleRange(CGXRange().SetCols(8), CGXStyle().SetEnabled(FALSE));

	//SetLoadLineColumn(7);   // medial strip
	//SetLoadLineColumn(8);   // medial strip

	// load
	SetStyleRange(CGXRange().SetCols(9, 11), CGXStyle()
			.SetHorizontalAlignment(DT_RIGHT)
			.SetValueType(GX_VT_NUMERIC)
			.SetFormat(GX_FMT_FIXED)
			.SetPlaces(4));

	// load unit
	SetStyleRange(CGXRange().SetCols(12), CGXStyle()
			.SetValueType(GX_VT_STRING)
			.SetHorizontalAlignment(DT_LEFT)
			.SetEnabled(FALSE));

	// main load range
	SetStyleRange(CGXRange().SetCols(13), CGXStyle()
			.SetHorizontalAlignment(DT_RIGHT)
			.SetValueType(GX_VT_NUMERIC)
			.SetFormat(GX_FMT_FIXED)
			.SetEnabled(FALSE)
			.SetPlaces(4));

	// show / hide
	for (ROWCOL nCol = 1; nCol<=m_nColCount; nCol++)
	{
		if (!IsUsedColumn(nCol))
		{
			SetStyleRange(CGXRange().SetCols(nCol), CGXStyle().SetEnabled(FALSE));
			HideColsByIntend(nCol, nCol, TRUE);
			//HideCols(nCol, nCol, TRUE);
		}
		if (nCol == 13) SetColWidthByDPI(nCol, nCol, 100);
		else if (nCol == 11) SetColWidthByDPI(nCol, nCol, 100);
		else SetColWidthByDPI(nCol, nCol, 70);
	}

	/*
	if (m_nColHeader > 0)
	{
		SetStyleRange(CGXRange().SetRows(1, m_nColHeader), CGXStyle()
				.SetControl(GX_IDS_CTRL_HEADER));
		SetFrozenRows(1, m_nColHeader, TRUE);
	}
	*/

	// 현재 셀의 border 설정 변경
	CGXProperties* pProp = GetParam()->GetProperties();
			pProp->SetUserProperty(GX_IDS_OUTLINECURRENTCELL,
		      (CGXStyle) pProp->sInvertNoBorder);

	// 현재 셀 설정
	SetRightValidCell(1+m_nColHeader, 1);
	GetParam()->SetHideCurrentCell(GX_HIDE_NEVER);

	LockUpdate(bOldLock);
	if (!bOldLock) Redraw();
}

void CCMGildBaseGrid::SetValue(ROWCOL nRow, int index, T_GILD_BASE &data)
{
	CStringArray aValue;
	ConvDataToStrArray(data, aValue);
	int nSize = aValue.GetSize();
	for (int i = 0; i < nSize; i++)
		SetExpressionRowCol(nRow, i+1, aValue[i]);
	
	// Key 설정
	SetStyleRange(CGXRange(nRow, 0), 
								CGXStyle().SetUserAttribute(ID_USER_ATTR_KEY, (long)index));

	ChangeEndColEnable(nRow, data.nLoadType);
	ChangeEndColEnable2(nRow, data.StartMedialK);
	if (data.nLoadType == 1) 
		ChangeEndColEnable3(nRow, data.StartCrossK == data.EndCrossK);

	ChangeEndColEnableLoad(nRow);
}

void CCMGildBaseGrid::SetHeaderTitle()
{
	BOOL bOldLock = LockUpdate(TRUE);
	
	SetColCount(m_nColCount);
	SetRowCount(m_nColHeader);
	// use 1 extra columns as headers ( + standard header at row 0)
	SetFrozenRows(1, 1);
	// Do not draw column headers pressed when moving the current cell
	GetParam( )->GetProperties( )->SetMarkColHeader(FALSE);

	CString aTitle[2][13] = {
		{ _LS(IDS_CMD_GILD_LOAD_TYPE), _LS(IDS_CMD_GILD_CENT_LINE), _LS(IDS_CMD_GILD_GIRD_LINE), _T(""), 
			_LS(IDS_CMD_GILD_CROS_LINE), _T(""), _LS(IDS_CMD_GILD_MEDI_STRP), _T(""), _LS(IDS_CMD_GILD_LOAD), _T(""), 
			_LS(IDS_CMD_GILD_LOAD), _LS(IDS_CMD_GILD_LOAD_UNIT), _LS(IDS_CMD_GILD_MAIN_LOAD)},
		{ _T(""), _T(""), _LS(IDS_CMD_GILD_START), _LS(IDS_CMD_GILD_END), _LS(IDS_CMD_GILD_START), _LS(IDS_CMD_GILD_END), 
			_LS(IDS_CMD_GILD_START), _LS(IDS_CMD_GILD_END), _LS(IDS_CMD_GILD_START), _LS(IDS_CMD_GILD_END), _T(""), _T("") }
	};

	SetCoveredCellsRowCol(0, 0, m_nColHeader, 0);  // header
	SetCoveredCellsRowCol(0, 1, m_nColHeader, 1);  // load type
	SetCoveredCellsRowCol(0, 2, m_nColHeader, 2);  // center line
	SetCoveredCellsRowCol(0, 3, 0, 4);             // girder/line
	SetCoveredCellsRowCol(0, 5, 0, 6);             // cross
	SetCoveredCellsRowCol(0, 7, 0, 8);             // medial strip 
	SetCoveredCellsRowCol(0, 9, 0,10);             // load 1
	SetCoveredCellsRowCol(0,11, m_nColHeader,11);  // load 2
	SetCoveredCellsRowCol(0,12, m_nColHeader,12);  // load unit
	SetCoveredCellsRowCol(0,13, m_nColHeader,13);  // main load range  

	// 단위 : load, main load
	CString csTitle, sRgUnit, sLdUnit;
	CUnitCtrl::GetUnitSystem(CUnitCtrl::m_GILD_UNIT.dMainLoadingRange,sRgUnit);
	CUnitCtrl::GetUnitSystem(CUnitCtrl::m_GILD_UNIT.dAreaLoad,sLdUnit);

	for (int nRow = 0; nRow <= m_nColHeader; nRow++)
	{
		for (int nCol = 1; nCol <= m_nColCount; nCol++)
		{
			csTitle = aTitle[nRow][nCol-1];
			if (nCol == 11) csTitle = csTitle + _T("\n(") + sLdUnit + _T(")");     
			else if (nCol == 13) csTitle = csTitle + _T("(") + sRgUnit + _T(")");     
			SetStyleRange(CGXRange(nRow, (ROWCOL)nCol), CGXStyle()
				.SetValue(csTitle));
		}
	}
	LockUpdate(bOldLock);
	if (!bOldLock) Redraw();
}

void CCMGildBaseGrid::ConvDataToStrArray(T_GILD_BASE &data, CStringArray &aValue)
{
//   1       2       3       4      5      6       7      8     9     10      11     12          13
// --------------------------------------------------------------------------------------------------------+
// LOAD | CENTER |  GIRDER/LINE |    CROSS    | MEDIAL STRIP |     LOAD    | LOAD | LOAD | MAIN LOAD RANGE |
//      |        | -------------|-------------|--------------|-------------|      | UNIT |                 | 
// TYPE |   LINE | START |  END | START | END |  START | END | START | END |      |      |                 |
// --------------------------------------------------------------------------------------------------------+

	aValue.SetSize(m_nColCount);
	for (int i=0; i<m_nColCount; i++) aValue[i].Empty();

	if (IsUsedColumn( 1)) aValue[ 0] = GetLoadTypeStr(data.nLoadType);
	if (IsUsedColumn( 2)) aValue[ 1] = GetLoadLineStr(data.CenterLineK);
	if (IsUsedColumn( 3)) aValue[ 2] = GetGirdLineStr(data.StartGirderLineK, data.bStartGirder);
	if (IsUsedColumn( 4)) aValue[ 3] = GetGirdLineStr(data.EndGirderLineK, data.bEndGirder);
	if (IsUsedColumn( 5)) aValue[ 4] = GetCrosLineStr(data.StartCrossK);
	if (IsUsedColumn( 6)) aValue[ 5] = GetCrosLineStr(data.EndCrossK);
	if (IsUsedColumn( 7)) aValue[ 6] = GetMedlStrpStr(data.StartMedialK);
	if (IsUsedColumn( 8)) aValue[ 7] = GetMedlStrpStr(data.EndMedialK);
	if (IsUsedColumn( 9)) aValue[ 8].Format(_T("%.15g"), data.dStartLoad);
	if (IsUsedColumn(10)) aValue[ 9].Format(_T("%.15g"), data.dEndLoad);
	if (IsUsedColumn(11)) aValue[10].Format(_T("%.15g"), data.dLoad);
	if (IsUsedColumn(12))
	{
		CString sLdUnit;
		int nUnitType;
		if (data.nLoadType == 0) nUnitType = CUnitCtrl::m_GILD_UNIT.dPointLoad;
		else if (data.nLoadType == 1) nUnitType = CUnitCtrl::m_GILD_UNIT.dLineLoad;
		else if (data.nLoadType == 2) nUnitType = CUnitCtrl::m_GILD_UNIT.dAreaLoad;
		else ASSERT(0);
	  CUnitCtrl::GetUnitSystem(nUnitType,sLdUnit);
		aValue[11] = sLdUnit;
	}
	if (IsUsedColumn(13)) aValue[12].Format(_T("%.15g"), data.dMainLoadingRange);
}

BOOL CCMGildBaseGrid::InsertRecordByKey(int index, T_GILD_BASE &data)
{
	ROWCOL nRowCount = GetRowCount() - 1;  // without append row
	ROWCOL nRow = m_nColHeader + index + 1;
	if (nRow > nRowCount) nRow = nRowCount+1;

	InsertRecord(nRow);
	SetValue(nRow, index, data);
	return TRUE;
}

BOOL CCMGildBaseGrid::DeleteRecordByKey(int index, T_GILD_BASE &data)
{
	ROWCOL ncRow, ncCol;
	BOOL bCurCell = GetCurrentCell(ncRow, ncCol);
	ROWCOL nRowCount = GetRowCount() - 1;  // without append row
	ROWCOL nRow = m_nColHeader + index + 1;
	ASSERT(nRow <= nRowCount);

	if (bCurCell && nRow == ncRow && m_nEditMode == edit) CancelEdit();
	DeleteRecord(nRow); 
	return TRUE;
}

// 인자로 넘어온 배열에 있는 모든 row를 Select 상태로 변경한다.
// 기존의 Select된 row는 없어진다.
void CCMGildBaseGrid::SelectGridRow(long* aRow, long nSize)
{
	// Row 배열을 오름차순으로 sorting한다.
	qsort((void*)aRow, nSize, sizeof(long), CCompFunc::longAsc);

	CGXRangeList* pSelList = GetParam()->GetRangeList();
	pSelList->DeleteAll();  // 기존 선택을 해제한다.
	long i = 0, j=0;
	for (i = 0; i < nSize; )
	{
		CGXRange* pRange = new CGXRange();
		// 연속된 Row를 찾아서 1개의 Range로 만든다.
		for (j = i+1; j < nSize; j++)
		{
			if (aRow[j] != aRow[j-1] + 1) break;
		}
		pRange->SetRows(aRow[i], aRow[j-1]);
		pSelList->AddTail(pRange);
		i = j;
	}
}

BOOL CCMGildBaseGrid::GetValue(ROWCOL nRow, T_GILD_BASE &data)
{
	data.Initialize();
	
	CStringArray aValue;
	for (int i = 0; i < m_nColCount; i++)
		aValue.Add(GetValueRowCol(nRow, i+1));
	return ConvStrArrayToData(aValue, data);
}


//--------------------------------------------------------------------------
// Overriadable
// Return은 항상 SetFieldValidation으로 한다.
// CTBBrowserWnd::ProcessKeys 참조
BOOL CCMGildBaseGrid::OnValidateField(ROWCOL nRow, ROWCOL nCol)
{
	CString value;
	int nColID;
	VERIFY(GetColumnIDFor(nCol, nColID));
	value = GetValueRowCol(nRow, nCol);

	if (!ValidateField(value, nColID)) return SetFieldValidation(FALSE);
	return SetFieldValidation(TRUE);
}

// Return은 항상 SetRecordValidation으로 한다.
// CTBBrowserWnd::ProcessKeys 참조
BOOL CCMGildBaseGrid::OnValidateRecord(ROWCOL nRow)
{
	// CStringArray 레코드를 만들어 validation을 검사한다.
	CRowColArray aCols;
	CStringArray value;
	int nColCount = GetColCount();
	aCols.SetSize(nColCount);
	value.SetSize(nColCount);
	for (int i = 0; i < nColCount; i++)
	{
		aCols[i] = i;
		//VERIFY(GetColumnNoFor(i, aCols[i]));
		value[i] = GetValueRowCol(nRow, i+1);
	}
	if (!ValidateRecord(value, aCols)) return SetRecordValidation(FALSE);
	return SetRecordValidation(TRUE);
}

BOOL CCMGildBaseGrid::ValidateField(CString& value, int nColID)
{
//   1       2       3       4      5      6       7      8     9     10      11     12          13
// --------------------------------------------------------------------------------------------------------+
// LOAD | CENTER |  GIRDER/LINE |    CROSS    | MEDIAL STRIP |     LOAD    | LOAD | LOAD | MAIN LOAD RANGE |
//      |        | -------------|-------------|--------------|-------------|      | UNIT |                 | 
// TYPE |   LINE | START |  END | START | END |  START | END | START | END |      |      |                 |
// --------------------------------------------------------------------------------------------------------+

	if (nColID >= GetColCount())
		return FALSE;

	if (nColID >=0 && nColID <= 7) return TRUE;
	else if (nColID == 8 || nColID == 9)
	{
		int nErrCode;
		/*
		if (_tstof(value) <= 0.0)
		{
			AfxMessageBox(_LS(IDS_CMD_ERROR_LOAD_VALUE));
			return FALSE;
		}
		*/
		if (!IsValidDoubleNumber(value, nErrCode))
		{
			switch(nErrCode)
			{
			case 1: SetWarningText(_LS(IDS_WG_CMD__ADDD__Invalid_formula_)); break;
			case 2: SetWarningText(_LS(IDS_WG_CMD__ADDD__Character_exists_in_double_value)); break;
			case 3: SetWarningText(_LS(IDS_WG_CMD__ADDD__Value_is_too_big_)); break;
			}
			return FALSE;
		}
	}
	else if (nColID == 10)
	{
		int nErrCode;
		if (_tstof(value) <= 0.0)
		{
			AfxMessageBox(_LS(IDS_CMD_ERROR_LOAD_VALUE));
			return FALSE;
		}
		if (!IsValidDoubleNumber(value, nErrCode))
		{
			switch(nErrCode)
			{
			case 1: SetWarningText(_LS(IDS_WG_CMD__ADDD__Invalid_formula_)); break;
			case 2: SetWarningText(_LS(IDS_WG_CMD__ADDD__Character_exists_in_double_value)); break;
			case 3: SetWarningText(_LS(IDS_WG_CMD__ADDD__Value_is_too_big_)); break;
			}
			return FALSE;
		}
	}
	else if (nColID == 12)  
	{
		int nErrCode;
		if (!IsValidDoubleNumber(value, nErrCode))
		{
			switch(nErrCode)
			{
			case 1: SetWarningText(_LS(IDS_WG_CMD__ADDD__Invalid_formula_)); break;
			case 2: SetWarningText(_LS(IDS_WG_CMD__ADDD__Character_exists_in_double_value)); break;
			case 3: SetWarningText(_LS(IDS_WG_CMD__ADDD__Value_is_too_big_)); break;
			}
			return FALSE;
		}    
	}
	else if (nColID == 11) return TRUE;
	else return FALSE;
	return TRUE;
}

BOOL CCMGildBaseGrid::ValidateRecord(CStringArray &aValue, CRowColArray &aCols)
{
	CString aTitle[] = {_LS(IDS_CMD_GILD_LOAD_TYPE), _LS(IDS_CMD_GILD_CENT_LINE), _LS(IDS_CMD_GILD_GIRD_LINE), _LS(IDS_CMD_GILD_GIRD_LINE), 
		_LS(IDS_CMD_GILD_CROS_LINE), _LS(IDS_CMD_GILD_CROS_LINE), _LS(IDS_CMD_GILD_MEDI_STRP), _LS(IDS_CMD_GILD_MEDI_STRP), 
		_LS(IDS_CMD_GILD_LOAD), _LS(IDS_CMD_GILD_LOAD), _LS(IDS_CMD_GILD_LOAD), _LS(IDS_CMD_GILD_LOAD_UNIT), _LS(IDS_CMD_GILD_MAIN_LOAD)};
	int nLoadType = GetLoadTypeData(aValue[0]);
	int nMedlStrp = GetMedlStrpData(aValue[6]);

	CString msg;
	int ColCount = aCols.GetSize();
	for (int i=0; i<ColCount; i++)
	{
		if (!IsUsedColumn(i+1)) continue;
		if (i == 3 || i == 5)
		{
			if (nLoadType == 0 && IsUsedColumn(1)) continue;
		}
		else if (i == 7)
		{
			if (IsUsedColumn(7) && nMedlStrp == 0) continue;
		}
		else if (i == 8)
		{
			if (_tstof(aValue[9]) == 0. && _tstof(aValue[8]) <= 0.)
			{
				AfxMessageBox(_LS(IDS_CMD_ERROR_LOAD_VALUE));
				return FALSE;
			}
		}
		else if (i == 9)
		{
			if (!IsUsedColumn(1)) continue;
			if (nLoadType == 0) continue;
			else if (nLoadType == 1 && aValue[2].Compare(aValue[3])==0) continue;
			if (_tstof(aValue[8]) == 0. && _tstof(aValue[9]) <= 0.)
			{
				AfxMessageBox(_LS(IDS_CMD_ERROR_LOAD_VALUE));
				return FALSE;
			}
		}

		//nPos = aCols[i]-1;
		if (aValue[i] == _T(""))
		{
			msg.Format(_T("%s has not been entered."), aTitle[i]);
			AfxMessageBox(msg);
			return SetRecordValidation(FALSE);
		}
		else if (!ValidateField(aValue[i], i)) return FALSE;
		
		// additional check
		if (IsUsedColumn(1))  // DEAD LOAD
		{
			if (nLoadType == 1)   // line
			{
				if ((aValue[2].Compare(aValue[3]) == 0 && aValue[4].Compare(aValue[5]) == 0) ||
						(aValue[2].Compare(aValue[3]) != 0 && aValue[4].Compare(aValue[5]) != 0))
				{
					AfxMessageBox(_LS(IDS_CMD_ERROR_GIRDER_CROSS_LINE));
					return FALSE;
				}
			}
			else if (nLoadType == 2)  // area
			{
				if (aValue[2].Compare(aValue[3]) == 0 || aValue[4].Compare(aValue[5]) == 0)
				{
					AfxMessageBox(_LS(IDS_CMD_ERROR_GIRDER_CROSS_AREA));
					return FALSE;
				}
			}
		}
		else    // LIVE LOAD
		{
			if (aValue[2].Compare(aValue[3]) == 0 || aValue[4].Compare(aValue[5]) == 0)
			{
				AfxMessageBox(_LS(IDS_CMD_ERROR_GIRDER_CROSS_AREA));
				return FALSE;
			}
		}
	}
	return TRUE;
}

BOOL CCMGildBaseGrid::OnAddNewRecord(ROWCOL nRow)
{
	BOOL bOldLock = LockUpdate(TRUE);
	if (!AddToDB(nRow))
	{
		LockUpdate(bOldLock);
		return SetRecordValidation(FALSE);
	}
	CancelEdit();
	//SetEnableDisableLcomCol();  // Add의 경우 여기서 한 번 더 호출해야 한다.
	LockUpdate(bOldLock);
	if (!bOldLock) Redraw();
	GetBrowseParam()->m_nRecordCount -= 1; 
	return SetRecordValidation(TRUE);
}

BOOL CCMGildBaseGrid::AddToDB(ROWCOL nRow)
{
	int nIndex;
	T_GILD_BASE data;
	
	nIndex = m_pData->aData.GetSize();
	GetValue(nRow, data);

	return AddData(nIndex, data);
}

BOOL CCMGildBaseGrid::AddData(int nIndex, T_GILD_BASE &data)
{
	CArray<int, int> aOldIndex, aIndex;
	CArray<T_GILD_BASE, T_GILD_BASE&> aData;
	aOldIndex.Add(nIndex);
	aIndex.Add(nIndex);
	aData.Add(data);

	m_pData->aData.InsertAt(nIndex, data);
	UpdateBuffer(0, aOldIndex, aIndex, aData);
	
	return TRUE;
}

BOOL CCMGildBaseGrid::OnModifyRecord(ROWCOL nRow)
{
	// 수정한다. 실패하면 에러 리턴
	BOOL bOldLock = LockUpdate(TRUE);
	if (!ModifyToDB(nRow))
	{
		LockUpdate(bOldLock);
		return SetRecordValidation(FALSE);
	}
	LockUpdate(bOldLock);
	if (!bOldLock) Redraw();
	return SetRecordValidation(TRUE);
}

BOOL CCMGildBaseGrid::ModifyToDB(ROWCOL nRow)
{
	int nIndex;
	T_GILD_BASE data;

	nIndex = nRow - m_nColHeader - 1;
	ASSERT(nIndex >= 0 && nIndex < m_pData->aData.GetSize());
	GetValue(nRow, data);

	return ModifyData(nIndex, nIndex, data);
}

BOOL CCMGildBaseGrid::ModifyData(int nOldIndex, int nIndex, T_GILD_BASE &data)
{
	CArray<int, int> aOldIndex, aIndex;
	CArray<T_GILD_BASE, T_GILD_BASE&> aData;
	aOldIndex.Add(nOldIndex);
	aIndex.Add(nIndex);
	aData.Add(data);

	m_pData->aData.SetAt(nIndex, data);
	UpdateBuffer(2, aOldIndex, aIndex, aData);
	
	return TRUE;
}

BOOL CCMGildBaseGrid::OnDeleteRecord(ROWCOL nRow)
{
	// save current row
	ROWCOL ncRow, ncCol;
	GetCurrentCell(ncRow, ncCol);

	if (m_nEditMode == addnew) return CancelEdit(); // addnew 상태이면 그냥 취소
	CRowColArray awRow;
	awRow.Add(nRow);
	if (!DeleteFromDB(awRow)) return FALSE;

	// restore current row
	if (GetRowCount() > ncRow) SetCurrentCell(ncRow, ncCol);
	return TRUE;
}

BOOL CCMGildBaseGrid::OnDeleteRecords(CRowColArray &awRows)
{
	// save current row
	ROWCOL ncRow, ncCol;
	GetCurrentCell(ncRow, ncCol);

	BOOL bNewRecordIncluded = FALSE;
	if (m_nEditMode == addnew)
	{
		// addnew 상태에서 현재 행이 포함되어 있으면 뺀다.
		for (int i = 0; i < awRows.GetSize(); i++)
		{
			if (GetBrowseParam()->m_nCurrentRow == awRows.GetAt(i))
			{ awRows.RemoveAt(i); bNewRecordIncluded = TRUE; break; }
		}
	}
 
	if (!DeleteFromDB(awRows)) return FALSE;
	if (bNewRecordIncluded) VERIFY(CancelEdit());

	// restore current row
	if (GetRowCount() > ncRow) SetCurrentCell(ncRow, ncCol);
	return TRUE;
}

BOOL CCMGildBaseGrid::DeleteFromDB(CRowColArray &awRows)
{
	CArray<int, int> aIndex;
	int nSize = awRows.GetSize();
	if (nSize == 0) return FALSE;
	aIndex.SetSize(nSize);
	for (int i = nSize-1; i >= 0; i--)
	{
		if (awRows[i] <= m_nColHeader) continue;
		aIndex[i] = awRows[i] - m_nColHeader - 1;
		ASSERT(aIndex[i] >= 0 && aIndex[i] < m_pData->aData.GetSize());
		m_pData->aData.RemoveAt(aIndex[i]);
	}

	CArray<int, int> aOldIndex;
	CArray<T_GILD_BASE, T_GILD_BASE&> aData;
	aOldIndex.Copy(aIndex); 
	T_GILD_BASE data; 
	data.Initialize();
	for (int i=0; i<aIndex.GetSize(); i++) aData.Add(data);

	UpdateBuffer(1, aOldIndex, aIndex, aData);

	return TRUE;
}

//--------------------------------------------------------------------------
// Grid's Standard Overridable Functions
// Record선택시 title row와 append row는 제거한다.
void CCMGildBaseGrid::OnChangedSelection(const CGXRange *changedRect, BOOL bIsDraggin, BOOL bKey)
{
	long nSize, nTmpSize;
	if (changedRect)
	{
		if (!bIsDraggin)
		{
			CRowColArray awRows;
			//if ((nSize = GetSelectedRows(awRows, TRUE, FALSE)) == 0)
			nSize = GetSelectedRows(awRows, TRUE, FALSE);
			// 전체 테이블 선택시 헤더와 append row는 제거한다.
			for (long i = nSize-1; i >= 0; i--)
				if (awRows[i] == GetRowCount() || awRows[i] <= m_nColHeader)
					awRows.RemoveAt(i);
			nTmpSize = awRows.GetSize();
			if (nSize != nTmpSize)
			{
				SelectGridRow((long*)awRows.GetData(), nTmpSize);
				if (nTmpSize + 2 != nSize)  // append row가 선택된 경우는 다시 그려준다.
				{
					BOOL bOldLock = LockUpdate(FALSE);
					if (!bOldLock) Redraw();
					LockUpdate(bOldLock);
				}
			}
		}
	}
}

/*
// LoadComb의 위치를 이동(Serial No가 변경)한다.
BOOL CCMGildBaseGrid::OnSelDragRowsDrop(ROWCOL nFirstRow, ROWCOL nLastRow, ROWCOL nDestRow)
{
	// 수정 모드이면 에러 리턴
	if (IsEdit()) 
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Can_t_Move_row_while_editing_));
		return FALSE;
	}

	// Append Row의 뒤면 맨 마지막으로 이동하는 것으로 한다.
	if (nDestRow == GetRowCount()+1) nDestRow--;  

	// 이동의 효과가 없는 곳으로 이동(선택범위내로 다시 이동)하면 그냥 리턴
	if (nDestRow >= nFirstRow && nDestRow <= nLastRow+1) return FALSE;

	// 연속 선택이 아니면 이동을 금지시킨다.
	CRowColArray awRows;
	GetSelectedRows(awRows, TRUE, FALSE);
	if (awRows.GetSize() != nLastRow-nFirstRow+1) return FALSE;


	int nStart, nInc;
	nStart = (int)nDestRow;
	if (nDestRow < nFirstRow)  // 선택 영역 앞으로 이동
	{
		nInc = 1;
	}
	else  // 선택 영역 뒤로 이동
	{
		nStart--;
		nInc = 0;
	}

	CArray<CString, CString&> aLoadCombName;
	CArray<T_LCOM_D, T_LCOM_D&> aData;
	CString LoadCombName;
	T_LCOM_D data;
	for (int i = nFirstRow; i <= nLastRow; i++)
	{
		LoadCombName = GetValueRowColBeforeEdit(i, 2);
		m_pDoc->m_pAttrCtrl->GetLcom(m_nLcomType, LoadCombName, data);
		data.LoadCombId = nStart;
		nStart += nInc;

		aLoadCombName.Add(data.LoadCombName);
		aData.Add(data);
	}
	m_pDoc->m_pDataCtrl->ModifyLcom(m_nLcomType, aLoadCombName, aData);
	return FALSE;
}
*/

// 항상 FALSE를 리턴해야 한다.
// 왜냐 하면 Grid가 Paste 작업을 해서는 안 되기 때문
BOOL CCMGildBaseGrid::OnPasteFromClipboard(const CGXRange &range)
{
	if (IsLockEdit()) return FALSE; // display only mode이면 바로 리턴

	// 임시로 CGXGridWnd를 하나 만들어 Paste를 한 후 작업을 한다.
	CGXGridWnd x;
	int nPasteMode;
	int nPastedCols;

	// 임시 GridWnd에 Paste 작업을 한다.
	// 먼저 Default Value Array를 만든다.
	CString* aDefVal = new CString[m_nColCount];
	for (int ix = 0; ix < m_nColCount; ix++)
		aDefVal[ix] = GetDefValue(ix);

	if (!PasteFromClipboard(range, aDefVal, 0, x, nPasteMode, nPastedCols))
	{
		delete []aDefVal;
		x.DestroyWindow();
		DisplayWarningText();
		return FALSE;
	}
	delete []aDefVal;

	CStringArray value;
	value.SetSize(m_nColCount);
	CArray<T_GILD_BASE, T_GILD_BASE&> aData;
	CRowColArray aCols;
	aCols.SetSize(m_nColCount);
	for (int i = 0; i < m_nColCount; i++)
		aCols[i] = i;
		//VERIFY(GetColumnNoFor(i, aCols[i]));

	// DB 수정이 되지 않는 경우이므로 Copy되는 Field Validation만 수행한다.
	if (nPasteMode == TB_PASTE_MODE_ONMODIFY)
	{
		int nColID;
		for (int j = range.left; j < range.left+nPastedCols; j++)
		{
			VERIFY(GetColumnIDFor(j, nColID));
			value[j-1] = x.GetValueRowCol(1, j);
			if (!ValidateField(value[j-1], nColID))
			{
				x.DestroyWindow();
				DisplayWarningText();
				return FALSE;
			}
		}
	}
	else // DB에 추가 또는 수정이 일어나는 경우이므로 Record Validation을 한다.
	{
		int nxRowCount = x.GetRowCount();
		for (int i = 1; i <= nxRowCount; i++)
		{
			// Table에서 레코드 값을 가져온다.
			int nxColCount = x.GetColCount();
			for (int j = 1; j <= nxColCount; j++)
				value[j-1] = x.GetValueRowCol(i, j);
			// String Array에 대해 Validation 검사. (필드, 레코드 전체)
			if (!MakeDataArray(value, aCols, aData))
			{
				x.DestroyWindow();
				if (m_sWarningText.IsEmpty())
					SetWarningText(_LS(IDS_WG_CMD__ADDD__Invalid_data__Operation_has_been_));
				DisplayWarningText();
				return FALSE;
			}
		}
	}
	
	// 모드에 따라 Append 또는 Modify를 한다.
	if (nPasteMode == TB_PASTE_MODE_APPEND) // DB에 추가된다.
	{
		CArray<int, int> aIndex;
		for (int i=0; i<aData.GetSize(); i++)
		{
			m_pData->aData.Add(aData[i]);
			aIndex.Add(m_pData->aData.GetSize());
		}
		UpdateBuffer(0, aIndex, aIndex, aData);
	}
	else if (nPasteMode == TB_PASTE_MODE_ONMODIFY)  // DB 수정 없다.
	{
		// 직접 그리드를 수정한다.
		CArray<int, int> aColID;
		aColID.SetSize(nPastedCols);
		for (int j = 0; j < nPastedCols; j++)
			VERIFY(GetColumnIDFor(range.left+j, aColID[j]));
		qsort((void*)aColID.GetData(), aColID.GetSize(), sizeof(UINT), CCompFunc::intAsc);
		ROWCOL nColNo;
		for (int j = 0; j < nPastedCols; j++)
		{
			VERIFY(GetColumnNoFor(aColID[j], nColNo));
			SetExpressionRowCol(range.top, nColNo, value[nColNo-1]);
			OnModifyCell(range.top, nColNo);
		}
	}   
	else if (nPasteMode == TB_PASTE_MODE_MODIFY) // DB가 수정된다.
	{
		CArray<int, int> aIndex;
		int nxRowCount = x.GetRowCount();
		aIndex.SetSize(nxRowCount);
		for (int i = 0; i < nxRowCount; i++)
		{
			aIndex[i] = range.top+i-(1+m_nColHeader);
			ASSERT(aIndex[i] >= 0 && aIndex[i] < m_pData->aData.GetSize());
			m_pData->aData.SetAt(aIndex[i], aData[i]);
		}
		UpdateBuffer(2, aIndex, aIndex, aData);
	}

	x.DestroyWindow();
	return FALSE;   // 실제 Paste는 하지 않는다.
}

BOOL CCMGildBaseGrid::PasteFromClipboard(const CGXRange &range, CString* aDefVal, int nKeyCol, 
																				 CGXGridWnd &x, int &nPasteMode, int &nPastedCols)
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

	// 규칙
	// 범위가 다르면 경고 메세지를 보내고 리턴
	// Paste된 내용이 레코드의 모든 필드를 만족하지 못하면 Default 값으로 채운다.
	// column 구분이 없기 때문에 칼럼 by 칼럼으로 Paste한다.
	// 즉 column의 위치가 변경되었으면 변경된 대로 Paste 한다.
	// 고의적으로 Hide된 칼럼은 Default 값으로 채운다.

	// 용어
	// 원본 범위 : Copy해서 Clipboard로 들어간 row, col 갯수
	// 대상 범위 : User가 마우스로 선택해 놓은 범위

	// 범위에 고의로 숨겨진 Column(HideByIntend)이 포함되어 있으면 이 칼럼을
	// 삽입하고 Default Value로 채운다.
	CArray<int, int> aHideCol;
	int nColCount = GetColCount();
	int nPasteColCount = r.GetWidth();
	for (int k = 1; k < nPasteColCount; k++)
	{
		if (range.left+k < nColCount && IsHiddenByIntend(range.left+k))
		{
			x.InsertCols(k+1, 1);
			aHideCol.Add(range.left+k);
			r.right++;
			nPasteColCount++;
		}
	}

	CString sErrMsg = _T("");
	ROWCOL nRow, nCol;
	int nLeftInsertWidth, nRightInsertWidth;
	VERIFY(GetCurrentCell(nRow, nCol));
	// 대상 범위가 지정되지 않은 경우
	// 현재 셀이 있는 위치를 기준으로 한다.
	if (range.GetHeight() == 1 && range.GetWidth() == 1)
	{
		// 대상 범위가 Append Row를 넘어서면 수정과 추가가 중복되므로 에러
		if (nRow != GetRowCount() && (nRow + r.GetHeight()) > GetRowCount())
		{
			//sErrMsg = _T("수정과 추가가 혼합되게 됩니다. 위치를 다시 지정하세요.");
			sErrMsg = _LS(IDS_TB_PASTE_ADD_MODIFY_MIX);
			goto END_OF_PASTE;
		}

		// 대상 범위가 Column 한계를 넘어서면 에러
		if (nCol + r.GetWidth() - 1 > GetColCount())
		{
			//sErrMsg = _T("붙여 넣을 범위가 칼럼 범위를 초과합니다. 위치를 다시 지정하세요.");
			sErrMsg = _LS(IDS_TB_PASTE_EXCEED_COLUMN);
			goto END_OF_PASTE;
		}

		// 편집 모드이고 원본 범위가 2줄 이상이면 에러
		if (IsEdit() && r.GetHeight() > 1)
		{
			//sErrMsg = _T("현재 편집중인 레코드가 복사 범위에 포함되어 있습니다.");
			sErrMsg = _LS(IDS_TB_PASTE_EDIT_DATA_INCLUDED);
			goto END_OF_PASTE;
		}

		// 현재 셀이 Append Row에 있으면 모두 추가(DB에 추가된다.)
		if (nRow == GetRowCount())
			nPasteMode = TB_PASTE_MODE_APPEND;
		// 레코드 수정상태를 유지(DB를 수정하지 않는다.)
		else if (IsEdit()) nPasteMode = TB_PASTE_MODE_ONMODIFY;
		// 모두 수정(DB에서 수정이 된다.)
		else nPasteMode = TB_PASTE_MODE_MODIFY;
	}
	// 대상 범위가 지정된 경우 : 원본과 같은 범위, Fill Mode(원본 1줄, 동일 칸수)
	else
	{
		// 편집 모드에서 편집 레코드가 대상 범위에 포함되어 있고 2줄 이상이면 에러 표시
		if (IsEdit() && nRow >= range.top && nRow <= range.bottom && range.GetHeight() > 1)
		{
			//sErrMsg = _T("현재 편집중인 레코드가 복사 범위에 포함되어 있습니다.");
			sErrMsg = _LS(IDS_TB_PASTE_EDIT_DATA_INCLUDED);
			goto END_OF_PASTE;
		}
		
		// Append Row가 범위에 포함된 경우 다른 Row도 범위에 포함되어 있으면 에러
		if (range.bottom == GetRowCount() && range.GetHeight() > 1)
		{
			//sErrMsg = _T("수정과 추가가 혼합되게 됩니다. 위치를 다시 지정하세요.");
			sErrMsg = _LS(IDS_TB_PASTE_ADD_MODIFY_MIX);
			goto END_OF_PASTE;
		}

		// Fill 조건을 검사한다.
		// 원본이 1줄 이상이면 에러, 칸수가 달라도 에러가 된다.
		BOOL bFillMode = FALSE;
		if (range.GetHeight() != r.GetHeight() || range.GetWidth() != r.GetWidth())
		{
			// Fill Mode가 안 되는 조건
			if (r.GetHeight() > 1 || range.GetWidth() != r.GetWidth())
			{
				//sErrMsg = _T("대상 범위를 지정한 경우는 원본 범위의 크기와 같아야 합니다.");
				sErrMsg = _LS(IDS_TB_PASTE_RANGE_MISMATCH);
				goto END_OF_PASTE;
			}
			// 대상 범위가 1줄 이상이면 Fill Mode, 아니면 Paste와 동일하다.
			if (range.GetHeight() > 1) bFillMode = TRUE;
		}

		// Append 단독 Row면 레코드 추가
		if (range.bottom == GetRowCount()) nPasteMode = TB_PASTE_MODE_APPEND; 
		// 레코드 수정 상태를 유지하며 DB에서 수정은 일어나지 않는다.
		else if (IsEdit() && range.top == nRow) nPasteMode = TB_PASTE_MODE_ONMODIFY;
		// 아니면 레코드 수정이 된다.
		else nPasteMode = TB_PASTE_MODE_MODIFY;

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

	// 레코드 또는 레코드 셋을 만든다.
	// 왼쪽과 오른쪽에 모자라는 칼럼수를 삽입한다.
	nPastedCols = r.GetWidth();
	nLeftInsertWidth = range.left-1;
	if (nLeftInsertWidth > 0) x.InsertCols(1, nLeftInsertWidth);
	nRightInsertWidth = GetColCount()-x.GetColCount();
	if (nRightInsertWidth > 0) x.InsertCols(x.GetColCount()+1, nRightInsertWidth);
	// 모자라는 값을 카피한다. (Append는 Default값, Modify는 기존 값)
	if (nPasteMode == TB_PASTE_MODE_APPEND)
	{
		int nID;
		CString value;
		// 속도 향상을 위해 column별로 복사
		// 왼쪽 칼럼 카피 
		for (int j = 1; j <= nLeftInsertWidth; j++)
		{
			nID = j-1;
			if (nID == nKeyCol) value = _T("");
			else value = aDefVal[nID-1];
			for (int i = 1; i <= r.GetHeight(); i++)
				x.SetExpressionRowCol(i, j, value);
		}
		// Copy된 영역중 고의로 숨겨진 칼럼에 Default값 채우기
		for (int j = 0; j < aHideCol.GetSize(); j++)
		{
			nID = aHideCol[j]-1;
			for (int i = 1; i <= r.GetHeight(); i++)
				x.SetExpressionRowCol(i, aHideCol[j], aDefVal[nID-1]);
		}
		// 오른쪽 칼럼 카피
		for (int j = nLeftInsertWidth+nPastedCols+1; j <= GetColCount(); j++)
		{
			nID = j-1;
			if (nID == nKeyCol) value = _T("");
			else value = aDefVal[nID-1];
			for (int i = 1; i <= r.GetHeight(); i++)
				x.SetExpressionRowCol(i, j, value);
		}
	}
	else if (nPasteMode == TB_PASTE_MODE_ONMODIFY || nPasteMode == TB_PASTE_MODE_MODIFY)
	{
		CString value;
		// 왼쪽 칼럼 카피 
		for (int j = 1; j <= nLeftInsertWidth; j++)
		{
			for (int i = 1; i <= r.GetHeight(); i++)
			{
				value = GetValueRowCol(range.top-1+i, j);
				x.SetExpressionRowCol(i, j, value);
			}
		}
		// Copy된 영역중 고의로 숨겨진 칼럼에 기존의 값 채우기
		for (int j = 0; j < aHideCol.GetSize(); j++)
		{
			for (int i = 1; i <= r.GetHeight(); i++)
			{
				value = GetValueRowCol(range.top-1+i, aHideCol[j]);
				x.SetExpressionRowCol(i, aHideCol[j], value);
			}
		}
		// 오른쪽 칼럼 카피
		for (int j = nLeftInsertWidth+nPastedCols+1; j <= GetColCount(); j++)
		{
			for (int i = 1; i <= r.GetHeight(); i++)
			{
				value = GetValueRowCol(range.top-1+i, j);
				x.SetExpressionRowCol(i, j, value);
			}
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

// Load Type이 변경되면 단위 변환 
void CCMGildBaseGrid::OnModifyCell(ROWCOL nRow, ROWCOL nCol)
{
	// 부모의 OnModifyCell을 항상 먼저 호출해 주어야 한다.
	CTBBrowserWnd::OnModifyCell(nRow, nCol);

	if (nCol == 1 || nCol == 5 || nCol == 6 || nCol == 7 || nCol == 13) 
		ChangeColumnStatus(nRow, nCol);
}

void CCMGildBaseGrid::OnCanceledEditing(ROWCOL nRow, ROWCOL nCol)
{
	CTBBrowserWnd::OnCanceledEditing(nRow, nCol);
	if (!IsEdit()) return;

	if (nCol == 1 || nCol == 5 || nCol == 6 || nCol == 7 || nCol == 13) 
		ChangeColumnStatus(nRow, nCol);
}

void CCMGildBaseGrid::ChangeColumnStatus(ROWCOL nRow, ROWCOL nCol)
{
	// load type column
	// 1. unit
	// 2. enable/disable girder line, cross, load
	if ((nCol != 1 || !IsUsedColumn(1)) &&
			(nCol != 5 || !IsUsedColumn(5)) &&
			(nCol != 6 || !IsUsedColumn(6)) &&
			(nCol != 7 || !IsUsedColumn(7)) &&
			(nCol != 13 || !IsUsedColumn(13))) return;


	/*
	if (nCol == 1)
	{
		int nLoadType;
		CString csLoadType;
		CGXControl *pControl;
		pControl = GetControl(nRow, nCol);
		if (pControl->IsActive() && nCol == 1) pControl->GetValue(csLoadType);
		else csLoadType = GetValueRowCol(nRow, 1);
		nLoadType = GetLoadTypeData(csLoadType);

		CString sLdUnit;
		int nUnitType;
		if (nLoadType == 0) nUnitType = CUnitCtrl::m_GILD_UNIT.dPointLoad;
		else if (nLoadType == 1) nUnitType = CUnitCtrl::m_GILD_UNIT.dLineLoad;
		else if (nLoadType == 2) nUnitType = CUnitCtrl::m_GILD_UNIT.dAreaLoad;
		else ASSERT(0);
		CUnitCtrl::GetUnitSystem(nUnitType,sLdUnit);

		SetValueRange(CGXRange(nRow, 12), sLdUnit);

		ChangeEndColEnable(nRow, nLoadType);
	}
	else 
	*/
	if (nCol == 1 || nCol == 5 || nCol == 6)
	{
		int nLoadType, nStartCross, nEndCross;
		CString csLoadType, csStartCross, csEndCross;

		CGXControl *pControl;
		pControl = GetControl(nRow, nCol);
		if (pControl->IsActive() && nCol == 1) pControl->GetValue(csLoadType);
		else csLoadType = GetValueRowCol(nRow, 1);
		nLoadType = GetLoadTypeData(csLoadType);
		if (pControl->IsActive() && nCol == 5) pControl->GetValue(csStartCross);
		else csStartCross = GetValueRowCol(nRow, 5);
		if (pControl->IsActive() && nCol == 6) pControl->GetValue(csEndCross);
		else csEndCross = GetValueRowCol(nRow, 6);
		nStartCross = GetCrosLineData(csStartCross);
		nEndCross = GetCrosLineData(csEndCross);

		if (nCol == 1)
		{
			CString sLdUnit;
			int nUnitType;
			if (nLoadType == 0) nUnitType = CUnitCtrl::m_GILD_UNIT.dPointLoad;
			else if (nLoadType == 1) nUnitType = CUnitCtrl::m_GILD_UNIT.dLineLoad;
			else if (nLoadType == 2) nUnitType = CUnitCtrl::m_GILD_UNIT.dAreaLoad;
			else ASSERT(0);
			CUnitCtrl::GetUnitSystem(nUnitType,sLdUnit);

			SetValueRange(CGXRange(nRow, 12), sLdUnit);
		}

		ChangeEndColEnable(nRow, nLoadType);
		if (nLoadType == 1) ChangeEndColEnable3(nRow, nStartCross == nEndCross);
	}
	else if (nCol == 7)
	{
		int nStrip;
		CString csStrip;
		CGXControl *pControl;
		pControl = GetControl(nRow, nCol);
		if (pControl->IsActive() && nCol == 7) pControl->GetValue(csStrip);
		else csStrip = GetValueRowCol(nRow, 7);
		nStrip = GetMedlStrpData(csStrip);

		ChangeEndColEnable2(nRow, nStrip);
	}

	//else if (nCol == 13)
	{
		BOOL bOldLock = LockUpdate(TRUE);

		int nRowCount = GetRowCount();
		if(nRow==2)
		{
			CString csVal= GetValueRowCol(2, 13);

			for(int i=2+1; i<nRowCount; i++)
			{
				ROWCOL nRowTemp = i;
				//SetExpressionRowCol(nRow, nCol, csVal);
				SetStyleRange(CGXRange(nRowTemp, 13), CGXStyle()
					.SetPlaces(4)
					.SetValue(csVal));

				//OnModifyRecord(nRowTemp);
				//ModifyToDB(nRowTemp);
				T_GILD_BASE data;
				GetValue(nRowTemp, data);
				int nIndex = nRowTemp - m_nColHeader - 1;
				m_pData->aData.SetAt(nIndex, data);
			}
		}

		for(int i=2; i<nRowCount; i++)
		{
			ChangeEndColEnableLoad(i+1);
		}

		LockUpdate(bOldLock);
		if (!bOldLock) Redraw();
		SetRecordValidation(TRUE);

	}
}

void CCMGildBaseGrid::ChangeEndColEnable(ROWCOL nRow, int nLoadType)
{
	if (!IsUsedColumn(1)) return;

	BOOL bEnable = (nLoadType == 0) ? FALSE : TRUE;
	SetStyleRange(CGXRange(nRow, 4), CGXStyle().SetEnabled(bEnable));
	SetStyleRange(CGXRange(nRow, 6), CGXStyle().SetEnabled(bEnable));
	SetStyleRange(CGXRange(nRow,10), CGXStyle().SetEnabled(bEnable));
}

void CCMGildBaseGrid::ChangeEndColEnable2(ROWCOL nRow, int nMedialStrp)
{
	if (!IsUsedColumn(7) || !IsUsedColumn(8)) return;

	BOOL bEnable = (nMedialStrp == 0) ? FALSE : TRUE;
	SetStyleRange(CGXRange(nRow, 8), CGXStyle().SetEnabled(bEnable));
	if (!bEnable) SetValueRange(CGXRange(nRow, 8), CString(_T("")));
}

void CCMGildBaseGrid::ChangeEndColEnable3(ROWCOL nRow, BOOL bEnable)
{
	if (!IsUsedColumn(5) && !IsUsedColumn(6)) return;
	if (!IsUsedColumn(10)) return;
		
	SetStyleRange(CGXRange(nRow,10), CGXStyle().SetEnabled(bEnable));
	if (!bEnable) SetValueRange(CGXRange(nRow, 10), CString(_T("0.0")));
}

void CCMGildBaseGrid::ChangeEndColEnableLoad(ROWCOL nRow)
{
	if (!IsUsedColumn(13)) return;

	SetStyleRange(CGXRange(nRow, 13), CGXStyle().SetEnabled(nRow==2));
}


/*
void CCMGildBaseGrid::ChangeEndColEnable3(ROWCOL nRow, BOOL bEnable)
{
	if (!IsUsedColumn(5) && !IsUsedColumn(6)) return;

	SetStyleRange(CGXRange(nRow, 10), CGXStyle().SetEnabled(bEnable));
	if (!bEnable) SetValueRange(CGXRange(nRow, 10), CString(_T("0.0")));
}
*/

BOOL CCMGildBaseGrid::GetStyleRowCol(ROWCOL nRow, ROWCOL nCol, CGXStyle& style, GXModifyType mt, int nType)
{
	BOOL bCurrentRow;
	BOOL bCurrentCell;
	BOOL bEnabled;
	BOOL bHeaderCol;
	BOOL bHeaderRow;

	ROWCOL ncRow, ncCol;    // 현재 Row, Col
	//CTBBrowserWnd::GetStyleRowCol(nRow, nCol, style, mt, nType);
	CGXGridWnd::GetStyleRowCol(nRow, nCol, style, mt, nType);

	// 현재 셀의 그리드 좌표(Row, Col 번호)를 얻어온다.
	if (!GetCurrentCell(ncRow, ncCol)) 
		ncRow = GetRowCount()+1; // 현재 셀이 없는 경우 : ncRow를 범위 밖으로 설정

	// 현재 셀이 이동되면 새로 그려준다.
	m_bRefreshOnSetCurrentCell = TRUE;

	// Row나 Col이 1보다 작으면 리턴한다.(Standard Row or Column Header)
	//if (nRow < 1 || nCol < 1) return TRUE;
	if (nRow <= m_nColHeader || nCol < 1) return TRUE;

	// Get Current Table and Cell State
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
	else // Column & Row style에서 enable 정보 알아냄
	{
		CGXStyle styleEx;
		GetColStyle(nCol, styleEx);
		bEnabled = (!styleEx.GetIncludeEnabled() || styleEx.GetEnabled());
		GetRowStyle(nRow, styleEx);
		bEnabled = (!styleEx.GetIncludeEnabled() || styleEx.GetEnabled());
	}

	bHeaderCol = IsFrozenCol(nCol);
	bHeaderRow = IsFrozenRow(nRow);

	// Printing 모드이면 헤더 Column이 프린트 되기 위해 다음과 같이 리턴한다.
	if (IsPrinting())
	{
		if (bHeaderCol) style.SetControl(GX_IDS_CTRL_HEADER);
		return TRUE;
	}

	// 일반 셀이 가장 많으므로 빨리 검사하고 바로 리턴할 수 있게 코드를 앞으로 뺀다.
	if (bEnabled && !bCurrentCell && !bCurrentRow && !bHeaderCol)
	{
		style
			.SetInterior(CTBGrid::m_Color.NormalModeBg)
			.SetTextColor(CTBGrid::m_Color.NormalModeFg);

		return TRUE;
	}

	// Disable되었을 때
	if (!bEnabled)  
	{
		if (bHeaderCol || bHeaderRow) style           // Row or Column Header
			.SetControl(GX_IDS_CTRL_HEADER)
			.SetInterior(CTBGrid::m_Color.RowColHeaderBg)
			.SetTextColor(CTBGrid::m_Color.NormalModeFg);
		else if (bCurrentRow) style     // Disabled Current Row
			.SetInterior(CTBGrid::m_Color.DisabledBg)
			.SetTextColor(CTBGrid::m_Color.ReadOnlyCurrentRowFg);
		else style                      // Disabled Cell
			.SetInterior(CTBGrid::m_Color.DisabledBg)
			.SetTextColor(CTBGrid::m_Color.DisabledFg);

		return TRUE;
	}

	if (bCurrentCell) style
		.SetInterior(CTBGrid::m_Color.NormalCurrentCellBg)
		.SetTextColor(CTBGrid::m_Color.NormalCurrentCellFg);
	else if (bCurrentRow) style
		.SetInterior(CTBGrid::m_Color.NormalModeBg)
		.SetTextColor(CTBGrid::m_Color.NormalModeFg);
	else if (bHeaderCol || bHeaderRow) style    // Enabled Header Col(Only Frozen)
		.SetInterior(CTBGrid::m_Color.NormalModeBg)
		.SetTextColor(CTBGrid::m_Color.NormalModeFg);

	return TRUE;
}

BOOL CCMGildBaseGrid::MakeDataArray(CStringArray& value, CRowColArray& awCols, 
																		CArray<T_GILD_BASE, T_GILD_BASE&>& aData)
{
	T_GILD_BASE data;
	data.Initialize();
	if (!ValidateAndMakeRecord(value, awCols, data)) return FALSE;
	aData.Add(data);
	return TRUE;
}

BOOL CCMGildBaseGrid::ValidateAndMakeRecord(CStringArray &value, CRowColArray& aCols, T_GILD_BASE &data)
{
	// 레코드에 대해 Validation을 검사한다.
	if (!ValidateRecord(value, aCols))
		return FALSE;

	// 레코드를 만들어서 리턴한다.
	ConvStrArrayToData(value, data);
	return TRUE;
}

BOOL CCMGildBaseGrid::ConvStrArrayToData(CStringArray &aValue, T_GILD_BASE& data)
{
//   1       2       3       4      5      6       7      8     9     10      11     12          13
// --------------------------------------------------------------------------------------------------------+
// LOAD | CENTER |  GIRDER/LINE |    CROSS    | MEDIAL STRIP |     LOAD    | LOAD | LOAD | MAIN LOAD RANGE |
//      |        | -------------|-------------|--------------|-------------|      | UNIT |                 | 
// TYPE |   LINE | START |  END | START | END |  START | END | START | END |      |      |                 |
// --------------------------------------------------------------------------------------------------------+

	if (IsUsedColumn( 1)) data.nLoadType = GetLoadTypeData(aValue[0]);
	if (IsUsedColumn( 2)) data.CenterLineK = GetLoadLineData(aValue[1]);
	if (IsUsedColumn( 3)) data.StartGirderLineK = GetGirdLineData(aValue[2], data.bStartGirder);
	if (IsUsedColumn( 4)) data.EndGirderLineK = GetGirdLineData(aValue[3], data.bEndGirder);
	if (IsUsedColumn( 5)) data.StartCrossK = GetCrosLineData(aValue[4]);
	if (IsUsedColumn( 6)) data.EndCrossK = GetCrosLineData(aValue[5]);
	if (IsUsedColumn( 7)) data.StartMedialK = GetMedlStrpData(aValue[6]);
	if (IsUsedColumn( 8)) data.EndMedialK = GetMedlStrpData(aValue[7]);
	if (IsUsedColumn( 9)) data.dStartLoad = _tstof(aValue[8]);
	if (IsUsedColumn(10)) data.dEndLoad = _tstof(aValue[9]);
	if (IsUsedColumn(11)) data.dLoad = _tstof(aValue[10]);
	if (IsUsedColumn(13)) data.dMainLoadingRange = _tstof(aValue[12]);

	if (IsUsedColumn( 1) && data.nLoadType == 0)
	{
		data.EndGirderLineK = data.StartGirderLineK;
		data.bEndGirder = data.bStartGirder;
		data.EndCrossK = data.StartCrossK;
		data.dEndLoad = data.dStartLoad;
	}

	return TRUE;
}


BEGIN_MESSAGE_MAP(CCMGildBaseGrid, CTBBrowserWnd)
	//{{AFX_MSG_MAP(CCMGildBaseGrid)
	//ON_WM_LBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CCMGildBaseGrid message handlers

/*
// 마우스 클릭시 번호 컬럼이 헤더 컬럼처럼 동작하게 한다.
void CCMGildBaseGrid::OnLButtonDown(UINT nFlags, CPoint point)
{
	ROWCOL nhRow, nhCol;
	int ht = HitTest(point, &nhRow, &nhCol);
	if (ht != GX_NOHIT && ht != GX_VERTLINE)
	{
		if (IsFrozenCol(nhCol)) // key column이면
		{
			// Row header의 rectangle을 얻는다.
			CRect r = CalcRectFromRowColEx(nhRow, 0, nhRow, 0);  
			point.x = (r.left+r.right)/2;
		}
	}

	CTBBrowserWnd::OnLButtonDown(nFlags, point);
}
*/

BOOL CCMGildBaseGrid::OnInsertEmptyRecord(ROWCOL nRow)
{
	// 여기서는 Default 값을 채워 넣는 역할을 하면 된다.
	// 새 레코드가 입력되는 순간에 번호 자동 입력(DB에서 가져온다.)
	BOOL bActive;
	ROWCOL nCol, ncRow, ncCol;
	CString value;

	// 현재 셀이 Active되어 있으면 값을 채워 넣지 않는다.
	VERIFY(GetCurrentCell(ncRow, ncCol));
	CGXControl* pControl = (CGXControl*)GetControl(ncRow, ncCol);
	if (pControl && pControl->IsActive()) bActive = TRUE;
	else bActive = FALSE;
	if (pControl->IsKindOf(CONTROL_CLASS(CGXCheckBox))) bActive = TRUE;
	else if (pControl->IsKindOf(CONTROL_CLASS(CGXCheckListComboBox))) bActive = TRUE;

	for (int i = 0; i < m_nColCount; i++)
	{
		nCol = i+1;
		//VERIFY(GetColumnNoFor(i+1, nCol));
		value = GetDefValue(i);
		if (!value.IsEmpty() && (nCol != ncCol || !bActive)) 
			SetExpressionRowCol(nRow, nCol, value);
		if(nRow > 1 && nCol==13) ChangeColumnStatus(nRow, nCol);
	}

	return TRUE;
}

CString CCMGildBaseGrid::GetDefValue(int ix)
{
	if (ix == 0) return D_GILD_LOAD_TYPE_PONT;
	else if (ix == 1) return _T("");
	else if (ix == 2) return _T("");
	else if (ix == 3) return _T("");
	else if (ix == 4) return _T("");
	else if (ix == 5) return _T("");
	else if (ix == 6) return D_GILD_NONE;
	else if (ix == 7) return _T("");
	else if (ix == 8) return _T("0.0");
	else if (ix == 9) return _T("0.0");
	else if (ix ==10) return _T("0.0");
	else if (ix ==11) return _T("");
	else if (ix ==12) 
	{
		 int nRowCount = GetRowCount();
		// 5.5m
		double dVal;
		CString str;
		if(nRowCount==3) // 처음
		{
			dVal = m_pDoc->m_pUnitCtrl->ConvertTgtUnitData2CurUnit(D_UNITSYS_FORCE_INDEX_KG, D_UNITSYS_LENGTH_INDEX_M, 
				CUnitCtrl::m_GILD_UNIT.dMainLoadingRange, 5.5);   
			str.Format(_T("%g"), dVal);
		}
		else
		{
			str = GetValueRowCol(2, 13);
		}
		return str;
	}
	else ASSERT(0);
	return _T("");
}


////////////////////////////////////////////////////

BOOL CCMGildBaseGrid::HideColsByIntend(int nFromCol, int nToCol, BOOL bHide)
{
	ASSERT(nFromCol >= 0 && nToCol <= GetColCount());

	if (!CGXGridWnd::HideCols(nFromCol, nToCol, bHide)) return FALSE;

	CString csHide;
	if (bHide) csHide = _T("Yes");
	else csHide = _T("");

	for (int i = nFromCol; i <= nToCol; i++)
	{
		SetStyleRange(CGXRange(0, i), CGXStyle()
			.SetUserAttribute(ID_USER_ATTR_HIDDEN, csHide));
		SetStyleRange(CGXRange().SetCols(i), CGXStyle().SetEnabled(!bHide));
	}

	return TRUE;
}

BOOL CCMGildBaseGrid::IsHiddenByIntend(int nCol)
{
	CGXStyle style;
	CString csHide;
	GetStyleRowCol(0, nCol, style);
	style.GetUserAttribute(ID_USER_ATTR_HIDDEN, csHide);
	if (csHide == _T("Yes")) return TRUE;
	else return FALSE;
}

// HideColsByIntend()함수를 도와주는 함수 
int CCMGildBaseGrid::HitTest(CPoint& pt, ROWCOL* pnRow, ROWCOL* pnCol, CRect* rectHit)
{
	int nHitResult = CGXGridWnd::HitTest(pt, pnRow, pnCol, rectHit);

	int nRow, nCol;
	nRow = (pnRow != NULL) ? *pnRow : -1;
	nCol = (pnCol != NULL) ? *pnCol : -1;

	if (nCol == -1) return nHitResult;

	if (nHitResult == GX_VERTLINE)
	{
		if (IsHiddenByIntend(nCol))
		{
			if (*pnCol == 0) nHitResult = GX_NOHIT;
			//else *pnCol -= 1; // 이 항목 대신 OnTrackColWidth() 함수를 Override해서 해결
		}
	}
	else if (nHitResult == GX_HDNCOL_VERTLINE)
	{
		if (IsHiddenByIntend(nCol))
		{
			*pnCol += 1;
			nHitResult = GX_HEADERHIT;
		}
	}
	return nHitResult;
}

// HideColsByIntend()함수를 도와주는 함수 
BOOL CCMGildBaseGrid::OnTrackColWidth(ROWCOL nCol)
{
	if (nCol == 0) return FALSE;
	if (IsHiddenByIntend(nCol)) return FALSE;
	return CGXGridWnd::OnTrackColWidth(nCol);
}

// HideColsByIntend()함수를 도와주는 함수
BOOL CCMGildBaseGrid::HideCols(ROWCOL nFromCol, ROWCOL nToCol, BOOL bHide, BOOL *abHideArray, UINT flags, GXCmdType ctCmd)
{
	BOOL bAllocated = FALSE;
	if (!bHide) // Show일 경우
	{
		if (abHideArray == 0) // flag 배열이 NULL이면 배열을 만든다.
		{
			bAllocated = TRUE;  // 새로 배열을 위한 메모리가 allocate되었음을 표시
			abHideArray = new BOOL[nToCol-nFromCol+1];
			for (int i = nFromCol; i <= nToCol; i++)
				abHideArray[i-nFromCol] = FALSE;
		}
		for (int i = nFromCol; i <= nToCol; i++)
			if (IsHiddenByIntend(i))
				abHideArray[i-nFromCol] = TRUE;
	}
	BOOL bResult = CGXGridWnd::HideCols(nFromCol, nToCol, bHide, abHideArray, flags, ctCmd);
	if (bAllocated) delete[] abHideArray;
	return bResult;
}

// HideColsByIntend로 인해 숨겨진 칼럼은 복사, Paste에서 빼야 한다.
BOOL CCMGildBaseGrid::Copy()
{
	// 원래 Range를 저장하고 의도적으로 숨겨진 Column은 뺀다.
	CGXRangeList OrgRange;
	CGXRangeList* pRangeList = GetParam()->GetRangeList();
	OrgRange = *pRangeList;
	for (int i = 1; i <= GetColCount(); i++)
	{
		if (IsHiddenByIntend(i)) 
			SelectRange(CGXRange().SetCols(i), FALSE, FALSE);
	}
	// 복사하고
	BOOL bResult = CTBBrowserWnd::Copy();
	// 원래 RangeList를 복구한다.
	*pRangeList = OrgRange;
	return bResult;
}

