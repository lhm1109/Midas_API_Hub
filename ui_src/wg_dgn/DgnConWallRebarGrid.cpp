// DgnConWallRebarGrid.cpp: implementation of the CDgnConWallRebarGrid class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnConWallRebarGrid.h"
#include "DgnConWallNewDlg.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_DBLib.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"

#include "..\wg_common\wg_common_TBGrid.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define D_MAX_ROW 5
#define D_MAX_COL 6

enum RowIndex
{ 
	ROWINDEX_HEADER       = 0,
	ROWINDEX_HEADER2      = 1,
	ROWINDEX_VERTICAL     = 2,
	ROWINDEX_HORIZONTAL   = 3,
	ROWINDEX_END          = 4, 
	ROWINDEX_BEHORIZONTAL = 5,
};

enum ColumnIndex
{
	COLINDEX_HEADER     = 0,
	COLINDEX_HD_CHECK   = 1,
	COLINDEX_HD_END     = 2,
	COLINDEX_HD_END_NO  = 3,
	COLINDEX_NAME       = 4, 
	COLINDEX_SYMBOL     = 5, 
	COLINDEX_SPACE      = 6,
};

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDgnConWallRebarGrid::CDgnConWallRebarGrid()
{
	m_nWndWidth = 0;
	m_nWndHeight = 0;

	memset(m_dSpace, 0, sizeof(m_dSpace));
	m_pUnitCtrl = CDBDoc::GetDocPoint()->m_pUnitCtrl;
}

// CDgnConWallRebarGrid::~CDgnConWallRebarGrid()
// {
// 
// }

//////////////////////////////////////////////////////////////////////
// Interface
void CDgnConWallRebarGrid::EnableGrid(int nRow, int nCol, BOOL bEnable)
{
	SetStyleRange(CGXRange().SetCells(nRow, nCol), CGXStyle().SetEnabled(bEnable));
}

void CDgnConWallRebarGrid::Initialize(CWnd* pParent)
{
	m_pParent = (CDgnConWallNewDlg*)pParent;

	CGXGridWnd::Initialize();
	
	// cell 바깥영역 배경색 지정 
	GetParam()->GetProperties()->SetColor(GX_COLOR_BACKGROUND, CTBGrid::m_Color.OuterBackground);
	
	// Header 배경색 지정 
	ChangeColHeaderStyle(CGXStyle().SetInterior(CTBGrid::m_Color.RowColHeaderBg)); 
	ChangeRowHeaderStyle(CGXStyle().SetInterior(CTBGrid::m_Color.RowColHeaderBg)); 
	
	// 기타 Cells 배경색 지정
	ChangeStandardStyle(CGXStyle( ).SetInterior(CTBGrid::m_Color.NormalModeBg));
	
	LockUpdate(TRUE);

	// 표준 스타일 변경
	ChangeStandardStyle(CGXStyle()
			.SetFont(CGXFont_GC().SetSize(9))
			.SetHorizontalAlignment(DT_CENTER)
			.SetVerticalAlignment(DT_VCENTER)
			.SetAllowEnter(FALSE));

	// Column header의 스타일 변경
	ChangeColHeaderStyle(CGXStyle().SetFont(CGXFont_GC().SetBold(FALSE)));
	SetRowHeight(0, 0, globalUtils.ScaleByDPI(20));

	GetParam()->EnableUndo(FALSE);

	// Row와  Column의 이동을 금지시킨다.
	GetParam()->EnableTrackRowHeight(FALSE);
	GetParam()->EnableTrackColWidth(FALSE);
	GetParam()->EnableMoveRows(FALSE);
	GetParam()->EnableMoveCols(FALSE);

	// MemoryDC를 사용한다.
//  SetDrawingTechnique(gxDrawUsingMemDC);
		
	// 헤더의 Current cell을 막는다.
	ColHeaderStyle().SetEnabled(FALSE);
	RowHeaderStyle().SetEnabled(FALSE);

	CGXProperties* pProp = GetParam()->GetProperties();
	pProp->SetMarkColHeader(FALSE); // Turn off pressed button effect for column headers
	pProp->SetMarkRowHeader(FALSE);
	pProp->SetUserProperty(GX_IDS_OUTLINECURRENTCELL, // 현재 셀의 border 설정 변경
												(CGXStyle) pProp->sInvertNoBorder);

	// Grid Number
	SetColCount(D_MAX_COL); // Number of column excluding header
	SetRowCount(D_MAX_ROW); // Number of row excluding header
	
	// Row_Grid Size
	SetRowHeight(0, D_MAX_ROW, (m_nWndHeight-3)/(D_MAX_ROW/*+1*/));
	SetSizeColumn();
	
	// Set Title
	SetHeaderTitle();
	SetDataColumn();
	
	HideCols(0, 0, TRUE);
	HideRows(0, 0, TRUE);
	SetScrollBarMode(SB_VERT, gxnDisabled);

	LockUpdate(FALSE);
	
	Redraw();
//	HideRows(GetRowCount(), GetRowCount(), TRUE);
	
	T_REBW_D RebwD;     RebwD.Initialize();
	Data2Grid(&RebwD, FALSE);

	LockUpdate(FALSE);
}

void CDgnConWallRebarGrid::SetSizeColumn()
{
	double dRatio = (double)m_nWndWidth/182.0;      // 182 = 국문 OS 에서의 넓이

	int nSizeEnd = 33 * dRatio;
	int nSizeCheck = 16 * dRatio;
	int nSizeEndNo = 33 * dRatio;
	int nSizeName = 43 * dRatio;
	int nSizeSymbol = 14 * dRatio;
	int nSizeSpace = 38 * dRatio;
	
	// Col_Grid Size
	SetColWidth(COLINDEX_HEADER   , COLINDEX_HEADER   , 0           );
	SetColWidth(COLINDEX_HD_CHECK , COLINDEX_HD_CHECK , nSizeCheck  );
	SetColWidth(COLINDEX_HD_END   , COLINDEX_HD_END   , nSizeEnd    );
	SetColWidth(COLINDEX_HD_END_NO, COLINDEX_HD_END_NO, nSizeEndNo  );
	SetColWidth(COLINDEX_NAME     , COLINDEX_NAME     , nSizeName   );
	SetColWidth(COLINDEX_SYMBOL   , COLINDEX_SYMBOL   , nSizeSymbol );
	SetColWidth(COLINDEX_SPACE    , COLINDEX_SPACE    , nSizeSpace  );
}

void CDgnConWallRebarGrid::SetDataColumn()
{
	CString strListRebar = Get_ListRebar();
	CString strListSpace = Get_ListSpaceSubBar();

	SetStyleRange(CGXRange().SetCells(ROWINDEX_VERTICAL, COLINDEX_NAME), CGXStyle()
		.SetControl(GX_IDS_CTRL_CBS_DROPDOWNLIST)
		.SetValueType(GX_VT_STRING)
		.SetEnabled(TRUE)
		.SetHorizontalAlignment(DT_CENTER)
		.SetChoiceList(strListRebar));
	SetStyleRange(CGXRange().SetCells(ROWINDEX_VERTICAL, COLINDEX_SYMBOL), CGXStyle()
		.SetControl(GX_IDS_CTRL_EDIT)
		.SetValueType(GX_VT_STRING)
		.SetValue(_T("@")).SetEnabled(FALSE)
		.SetReadOnly(TRUE)
		.SetHorizontalAlignment(DT_RIGHT));
	SetStyleRange(CGXRange().SetCells(ROWINDEX_VERTICAL, COLINDEX_SPACE), CGXStyle()
		.SetControl(GX_IDS_CTRL_CBS_DROPDOWN)
		.SetValueType(GX_VT_STRING)
		.SetHorizontalAlignment(DT_CENTER)
		.SetChoiceList(strListSpace)
		.SetFormat(GX_FMT_FIXED).SetPlaces(2));

	SetStyleRange(CGXRange().SetCells(ROWINDEX_HORIZONTAL, COLINDEX_NAME), CGXStyle()
		.SetControl(GX_IDS_CTRL_CBS_DROPDOWNLIST)
		.SetValueType(GX_VT_STRING)
		.SetEnabled(TRUE)
		.SetHorizontalAlignment(DT_CENTER)
		.SetChoiceList(strListRebar));
	SetStyleRange(CGXRange().SetCells(ROWINDEX_HORIZONTAL, COLINDEX_SYMBOL), CGXStyle()
		.SetControl(GX_IDS_CTRL_EDIT)
		.SetValueType(GX_VT_STRING)
		.SetValue(_T("@")).SetEnabled(FALSE)
		.SetReadOnly(TRUE)
		.SetHorizontalAlignment(DT_RIGHT));
	SetStyleRange(CGXRange().SetCells(ROWINDEX_HORIZONTAL, COLINDEX_SPACE), CGXStyle()
		.SetControl(GX_IDS_CTRL_CBS_DROPDOWN)
		.SetValueType(GX_VT_STRING)
		.SetHorizontalAlignment(DT_CENTER)
		.SetChoiceList(strListSpace)
		.SetFormat(GX_FMT_FIXED).SetPlaces(2));
	
	SetStyleRange(CGXRange().SetCells(ROWINDEX_END, COLINDEX_NAME), CGXStyle()
		.SetControl(GX_IDS_CTRL_CBS_DROPDOWNLIST)
		.SetValueType(GX_VT_STRING)
		.SetEnabled(TRUE)
		.SetHorizontalAlignment(DT_CENTER)
		.SetChoiceList(strListRebar));
	SetStyleRange(CGXRange().SetCells(ROWINDEX_END, COLINDEX_SYMBOL), CGXStyle()
		.SetControl(GX_IDS_CTRL_EDIT)
		.SetValueType(GX_VT_STRING)
		.SetValue(_T("@")).SetEnabled(FALSE)
		.SetReadOnly(TRUE)
		.SetHorizontalAlignment(DT_RIGHT));
	SetStyleRange(CGXRange().SetCells(ROWINDEX_END, COLINDEX_SPACE), CGXStyle()
		.SetControl(GX_IDS_CTRL_CBS_DROPDOWN)
		.SetValueType(GX_VT_STRING)
		.SetHorizontalAlignment(DT_CENTER)
		.SetChoiceList(strListSpace)
		.SetFormat(GX_FMT_FIXED).SetPlaces(2));

	SetStyleRange(CGXRange().SetCells(ROWINDEX_BEHORIZONTAL, COLINDEX_NAME), CGXStyle()
		.SetControl(GX_IDS_CTRL_CBS_DROPDOWNLIST)
		.SetValueType(GX_VT_STRING)
		.SetEnabled(TRUE)
		.SetHorizontalAlignment(DT_CENTER)
		.SetChoiceList(strListRebar));
	SetStyleRange(CGXRange().SetCells(ROWINDEX_BEHORIZONTAL, COLINDEX_SYMBOL), CGXStyle()
		.SetControl(GX_IDS_CTRL_EDIT)
		.SetValueType(GX_VT_STRING)
		.SetValue(_T("@")).SetEnabled(FALSE)
		.SetReadOnly(TRUE)
		.SetHorizontalAlignment(DT_RIGHT));
	SetStyleRange(CGXRange().SetCells(ROWINDEX_BEHORIZONTAL, COLINDEX_SPACE), CGXStyle()
		.SetControl(GX_IDS_CTRL_CBS_DROPDOWN)
		.SetValueType(GX_VT_STRING)
		.SetHorizontalAlignment(DT_CENTER)
		.SetChoiceList(strListSpace)
		.SetFormat(GX_FMT_FIXED).SetPlaces(2));
}

void CDgnConWallRebarGrid::Data2Grid(T_REBW_D* pData, BOOL bSelectedWall/*=TRUE*/)
{
	if(bSelectedWall == FALSE)
	{
		SetStyleRange(CGXRange(ROWINDEX_VERTICAL    ,COLINDEX_NAME), CGXStyle().SetValue(_T("-")).SetEnabled(FALSE));
		SetStyleRange(CGXRange(ROWINDEX_HORIZONTAL  ,COLINDEX_NAME), CGXStyle().SetValue(_T("-")).SetEnabled(FALSE));
		SetStyleRange(CGXRange(ROWINDEX_END         ,COLINDEX_NAME), CGXStyle().SetValue(_T("-")).SetEnabled(FALSE));
		SetStyleRange(CGXRange(ROWINDEX_BEHORIZONTAL,COLINDEX_NAME), CGXStyle().SetValue(_T("-")).SetEnabled(FALSE));

		SetStyleRange(CGXRange(ROWINDEX_END         ,COLINDEX_HD_CHECK),  CGXStyle().SetValue(_LSX(0)).SetEnabled(FALSE));
		SetStyleRange(CGXRange(ROWINDEX_END         ,COLINDEX_HD_END_NO), CGXStyle().SetValue(_T("-")).SetEnabled(FALSE));

		SetStyleRange(CGXRange(ROWINDEX_VERTICAL    ,COLINDEX_SPACE), CGXStyle().SetValue(_T("-")).SetEnabled(FALSE));
		SetStyleRange(CGXRange(ROWINDEX_HORIZONTAL  ,COLINDEX_SPACE), CGXStyle().SetValue(_T("-")).SetEnabled(FALSE));
		SetStyleRange(CGXRange(ROWINDEX_END         ,COLINDEX_SPACE), CGXStyle().SetValue(_T("-")).SetEnabled(FALSE));
		SetStyleRange(CGXRange(ROWINDEX_BEHORIZONTAL,COLINDEX_SPACE), CGXStyle().SetValue(_T("-")).SetEnabled(FALSE));

		return;
	}

	SetStyleRange(CGXRange(ROWINDEX_VERTICAL    ,COLINDEX_NAME), CGXStyle().SetValue(pData->VerticalRebarName   ).SetEnabled(TRUE));
	SetStyleRange(CGXRange(ROWINDEX_HORIZONTAL  ,COLINDEX_NAME), CGXStyle().SetValue(pData->HorizonRebarName    ).SetEnabled(TRUE));
	SetStyleRange(CGXRange(ROWINDEX_END         ,COLINDEX_NAME), CGXStyle().SetValue(pData->EndRebarName        ).SetEnabled(TRUE));
	SetStyleRange(CGXRange(ROWINDEX_BEHORIZONTAL,COLINDEX_NAME), CGXStyle().SetValue(pData->BEHorizonRebarName  ).SetEnabled(TRUE));
	int nEndRebar = pData->nNumEndRebar;
	BOOL bCheck = TRUE;
	if(nEndRebar == 0)
	{
		nEndRebar = 2;
		SetStyleRange(CGXRange(ROWINDEX_END       ,COLINDEX_HD_CHECK), CGXStyle().SetValue(_LSX(0)).SetEnabled(TRUE));
		bCheck = FALSE;
	}
	else
		SetStyleRange(CGXRange(ROWINDEX_END       ,COLINDEX_HD_CHECK), CGXStyle().SetValue(_LSX(1)).SetEnabled(TRUE));

	SetStyleRange(CGXRange().SetCells(ROWINDEX_END, COLINDEX_HD_END_NO), CGXStyle().SetEnabled(bCheck));
	SetStyleRange(CGXRange().SetCells(ROWINDEX_END, COLINDEX_NAME     ), CGXStyle().SetEnabled(bCheck));
	SetStyleRange(CGXRange().SetCells(ROWINDEX_END, COLINDEX_SPACE    ), CGXStyle().SetEnabled(bCheck));
	m_pParent->ShowBitmap(bCheck);

	SetStyleRange(CGXRange(ROWINDEX_END       ,COLINDEX_HD_END_NO), CGXStyle().SetValue((UINT)nEndRebar));

	CString strVerSpace, strHorSpace, strEndSpace, strBEHorSpace;
	T_UNIT_INDEX UnitIndex;
	m_pUnitCtrl->GetUnitIndexCurrent(UnitIndex);
	if(UnitIndex.nBase_Length == D_UNITSYS_LENGTH_INDEX_MM || UnitIndex.nBase_Length == D_UNITSYS_LENGTH_INDEX_CM ||
		UnitIndex.nBase_Length == D_UNITSYS_LENGTH_INDEX_M)
	{
		double dSpace = m_pUnitCtrl->ConvertCurUnitData2TgtUnit(0, D_UNITSYS_LENGTH_INDEX_MM, D_UNITSYS_BASE_LENGTH, pData->dVerticalRebarSpace);
		strVerSpace.Format(_T("%g"), dSpace);
		dSpace = m_pUnitCtrl->ConvertCurUnitData2TgtUnit(0, D_UNITSYS_LENGTH_INDEX_MM, D_UNITSYS_BASE_LENGTH, pData->dHorizonRebarSpace);
		strHorSpace.Format(_T("%g"), dSpace); 
		dSpace = m_pUnitCtrl->ConvertCurUnitData2TgtUnit(0, D_UNITSYS_LENGTH_INDEX_MM, D_UNITSYS_BASE_LENGTH, pData->dEndRebarSpace);
		strEndSpace.Format(_T("%g"), dSpace); 
		dSpace = m_pUnitCtrl->ConvertCurUnitData2TgtUnit(0, D_UNITSYS_LENGTH_INDEX_MM, D_UNITSYS_BASE_LENGTH, pData->dBEHorizonRebarSpace);
		strBEHorSpace.Format(_T("%g"), dSpace); 
	}
	else if(UnitIndex.nBase_Length == D_UNITSYS_LENGTH_INDEX_IN || UnitIndex.nBase_Length == D_UNITSYS_LENGTH_INDEX_FT)
	{
		double dSpace = m_pUnitCtrl->ConvertCurUnitData2TgtUnit(0, D_UNITSYS_LENGTH_INDEX_IN, D_UNITSYS_BASE_LENGTH, pData->dVerticalRebarSpace);
		strVerSpace.Format(_T("%g"), dSpace);
		dSpace = m_pUnitCtrl->ConvertCurUnitData2TgtUnit(0, D_UNITSYS_LENGTH_INDEX_IN, D_UNITSYS_BASE_LENGTH, pData->dHorizonRebarSpace);
		strHorSpace.Format(_T("%g"), dSpace); 
		dSpace = m_pUnitCtrl->ConvertCurUnitData2TgtUnit(0, D_UNITSYS_LENGTH_INDEX_IN, D_UNITSYS_BASE_LENGTH, pData->dEndRebarSpace);
		strEndSpace.Format(_T("%g"), dSpace); 
		dSpace = m_pUnitCtrl->ConvertCurUnitData2TgtUnit(0, D_UNITSYS_LENGTH_INDEX_IN, D_UNITSYS_BASE_LENGTH, pData->dBEHorizonRebarSpace);
		strBEHorSpace.Format(_T("%g"), dSpace); 
	}
	
	SetStyleRange(CGXRange(ROWINDEX_VERTICAL      ,COLINDEX_SPACE), CGXStyle().SetValue(strVerSpace).SetEnabled(TRUE));
	SetStyleRange(CGXRange(ROWINDEX_HORIZONTAL    ,COLINDEX_SPACE), CGXStyle().SetValue(strHorSpace).SetEnabled(TRUE));
	SetStyleRange(CGXRange(ROWINDEX_END           ,COLINDEX_SPACE), CGXStyle().SetValue(strEndSpace).SetEnabled(bCheck));
	SetStyleRange(CGXRange(ROWINDEX_BEHORIZONTAL  ,COLINDEX_SPACE), CGXStyle().SetValue(strBEHorSpace).SetEnabled(TRUE));

	m_dSpace[0] = pData->dVerticalRebarSpace;
	m_dSpace[1] = pData->dHorizonRebarSpace;
	m_dSpace[2] = pData->dEndRebarSpace;
	m_dSpace[3] = pData->dBEHorizonRebarSpace;

	SetEnableByRcStructWallOpt();
}

void CDgnConWallRebarGrid::Grid2Data(T_REBW_D* pData)
{
	pData->VerticalRebarName  = GetEditingValue(ROWINDEX_VERTICAL     , COLINDEX_NAME);
	pData->HorizonRebarName   = GetEditingValue(ROWINDEX_HORIZONTAL   , COLINDEX_NAME);
	pData->EndRebarName       = GetEditingValue(ROWINDEX_END          , COLINDEX_NAME);
	pData->BEHorizonRebarName = GetEditingValue(ROWINDEX_BEHORIZONTAL , COLINDEX_NAME);
	pData->nNumEndRebar       = _ttoi(GetEditingValue(ROWINDEX_END     , COLINDEX_HD_END_NO));

	BOOL bEndRebar = _ttoi(GetValueRowCol(ROWINDEX_END, COLINDEX_HD_CHECK));
	if(bEndRebar == 0)
	{
		pData->nNumEndRebar = 0;
		pData->EndRebarName = _T("");
		pData->dEndRebarSpace = 0.0;
	}

	pData->dVerticalRebarSpace  = m_dSpace[0];
	pData->dHorizonRebarSpace   = m_dSpace[1];
	pData->dEndRebarSpace       = m_dSpace[2];
	pData->dBEHorizonRebarSpace = m_dSpace[3];
}

CString CDgnConWallRebarGrid::Get_ListRebar()
{
	CStringArray arRebar;	arRebar.RemoveAll();
	CDBLib::GetRebarNameListByCode(arRebar);
	
	CString strString = _T("");
	
	if (arRebar.GetSize() != 0)
	{
		for (int i=0; i<arRebar.GetSize(); i++)
		{
			strString += arRebar[i] + _T("\n");
		}
	}
	
	return strString;
}

CString CDgnConWallRebarGrid::Get_ListSpaceSubBar()
{
	CStringArray arItem; arItem.RemoveAll();
	
	T_UNIT_INDEX UnitIndex;
	m_pUnitCtrl->GetUnitIndexCurrent(UnitIndex);
	if(UnitIndex.nBase_Length == D_UNITSYS_LENGTH_INDEX_MM || UnitIndex.nBase_Length == D_UNITSYS_LENGTH_INDEX_CM ||
		 UnitIndex.nBase_Length == D_UNITSYS_LENGTH_INDEX_M)
	{
		arItem.Add(_LSX(100));
		arItem.Add(_LSX(125));
		arItem.Add(_LSX(150));
		arItem.Add(_LSX(200));
		arItem.Add(_LSX(250));
		arItem.Add(_LSX(300));
		arItem.Add(_LSX(350));
		arItem.Add(_LSX(400));
		arItem.Add(_LSX(450));
	}
	else if(UnitIndex.nBase_Length == D_UNITSYS_LENGTH_INDEX_IN || UnitIndex.nBase_Length == D_UNITSYS_LENGTH_INDEX_FT)
	{
		arItem.Add(_ULS('4'));
		arItem.Add(_ULS('5'));
		arItem.Add(_ULS('6'));
		arItem.Add(_ULS('8'));
		arItem.Add(_ULS('10'));
		arItem.Add(_ULS('12'));
		arItem.Add(_ULS('14'));
		arItem.Add(_ULS('16'));
		arItem.Add(_ULS('18'));
	}
	else   {  ASSERT(0);		return _T("");	}
	
	CString strList = _T("");
	
	for(int i=0; i<arItem.GetSize(); i++) 
	{
		strList += arItem[i] + _T("\n");
	}
	
	return strList;
}

//////////////////////////////////////////////////////////////////////
// Operation

void CDgnConWallRebarGrid::SetHeaderTitle()
{
	int iFontSizeHeader  = 8;
	int iFontSizeNormal  = 8;
	BOOL bFontBoldHeader = FALSE;
	BOOL bFontBoldNormal = FALSE;
	
	CGXStyle style;
	style.SetControl(GX_IDS_CTRL_STATIC);
	style.SetDraw3dFrame(gxFrameRaised);
	style.SetFont(CGXFont_GC().SetBold(FALSE).SetSize(9));
	style.SetInterior(ColHeaderStyle().GetInterior()); 

	SetCoveredCellsRowCol(ROWINDEX_HEADER2, COLINDEX_HD_CHECK  ,  ROWINDEX_HEADER2, COLINDEX_HD_END_NO);
	style.SetValue(_LS(IDS_DGN_REBAR));
	SetStyleRange(CGXRange(ROWINDEX_HEADER2, COLINDEX_HD_CHECK), style);

	SetCoveredCellsRowCol(ROWINDEX_HEADER2, COLINDEX_NAME ,  ROWINDEX_HEADER2, COLINDEX_SPACE );
	style.SetValue(_LS(IDS_DGN_DATA));
	SetStyleRange(CGXRange(ROWINDEX_HEADER2, COLINDEX_NAME), style);
	
	SetCoveredCellsRowCol(ROWINDEX_VERTICAL, COLINDEX_HD_CHECK ,  ROWINDEX_VERTICAL, COLINDEX_HD_END_NO);
	style.SetValue(_LS(IDS_DGN_VERTICAL));
	SetStyleRange(CGXRange(ROWINDEX_VERTICAL, COLINDEX_HD_CHECK), style);

	SetCoveredCellsRowCol(ROWINDEX_HORIZONTAL, COLINDEX_HD_CHECK  ,  ROWINDEX_HORIZONTAL, COLINDEX_HD_END_NO );
	style.SetValue(_LS(IDS_DGN_HORIAONTAL));
	SetStyleRange(CGXRange(ROWINDEX_HORIZONTAL, COLINDEX_HD_CHECK), style);

	style.SetValue(_LS(IDS_DGN_END));
	SetStyleRange(CGXRange(ROWINDEX_END, COLINDEX_HD_END), style);

	SetCoveredCellsRowCol(ROWINDEX_BEHORIZONTAL, COLINDEX_HD_CHECK  ,  ROWINDEX_BEHORIZONTAL, COLINDEX_HD_END_NO );
	style.SetValue(_LS(IDS_DGN_BEHORIAONTAL));
	SetStyleRange(CGXRange(ROWINDEX_BEHORIZONTAL, COLINDEX_HD_CHECK), style);

	CGXStyle style2;
	style2.SetControl(GX_IDS_CTRL_CHECKBOX3D);
	style2.SetValue(_LSX(1));
	SetStyleRange(CGXRange(ROWINDEX_END, COLINDEX_HD_CHECK), style2);

	style2.SetControl(GX_IDS_CTRL_SPINEDIT).SetValueType(GX_VT_NUMERIC);
	SetStyleRange(CGXRange(ROWINDEX_END, COLINDEX_HD_END_NO), style2);
}

CString CDgnConWallRebarGrid::GetEditingValue(ROWCOL nRow, ROWCOL nCol)
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

//////////////////////////////////////////////////////////////////////
// Override

void CDgnConWallRebarGrid::OnModifyCell(ROWCOL nRow, ROWCOL nCol)
{
	CGXGridWnd::OnModifyCell(nRow, nCol);

	if(nRow == ROWINDEX_END && nCol == COLINDEX_HD_END_NO)
	{
		int nValue = _ttoi(GetEditingValue(nRow, nCol));
		if(nValue < 0)   SetStyleRange(CGXRange(nRow, nCol), CGXStyle().SetValue((UINT)0));
	}
}

BOOL CDgnConWallRebarGrid::OnEndEditing(ROWCOL nRow, ROWCOL nCol)
{  
	BOOL bRet = CGXGridWnd::OnEndEditing(nRow, nCol);
	
	if(nRow != ROWINDEX_VERTICAL && nRow != ROWINDEX_HORIZONTAL && nRow != ROWINDEX_END && nRow != ROWINDEX_BEHORIZONTAL)   return bRet;
	if(nCol != COLINDEX_SPACE)  return bRet;
	
	int nIndex = 0;
	if(nRow == ROWINDEX_HORIZONTAL)         nIndex = 1;
	else if(nRow == ROWINDEX_END)           nIndex = 2;
	else if(nRow == ROWINDEX_BEHORIZONTAL)  nIndex = 3;
	
	CString strSpace   = GetEditingValue(nRow, nCol);
	
	T_UNIT_INDEX UnitIndex;
	m_pUnitCtrl->GetUnitIndexCurrent(UnitIndex);
	int nTargetUnitL = D_UNITSYS_LENGTH_INDEX_MM;
	if(UnitIndex.nBase_Length == D_UNITSYS_LENGTH_INDEX_MM || UnitIndex.nBase_Length == D_UNITSYS_LENGTH_INDEX_CM ||
		UnitIndex.nBase_Length == D_UNITSYS_LENGTH_INDEX_M)
	{
		double dSpace = _tstof(strSpace);
		m_dSpace[nIndex] = M_InitValueCurUnit(dSpace, N, MM, D_UNITSYS_BASE_LENGTH);
	}
	else if(UnitIndex.nBase_Length == D_UNITSYS_LENGTH_INDEX_IN || UnitIndex.nBase_Length == D_UNITSYS_LENGTH_INDEX_FT)
	{
		strSpace.Remove('\\');
		double dSpace = _tstof(strSpace);
		strSpace += _T("\\");
		SetValueRange(CGXRange(nRow, nCol), strSpace);
		m_dSpace[nIndex] = M_InitValueCurUnit(dSpace, N, IN, D_UNITSYS_BASE_LENGTH);
	}
	else   {  ASSERT(0);		return FALSE;	}
	
	return bRet;
}

BOOL CDgnConWallRebarGrid::OnLButtonClickedRowCol(ROWCOL nRow, ROWCOL nCol, UINT nFlags, CPoint pt)
{
	BOOL bRet = CGXGridWnd::OnLButtonClickedRowCol(nRow, nCol, nFlags, pt);

	if(nRow == ROWINDEX_END && nCol == COLINDEX_HD_CHECK)
	{
		BOOL bCheck = _ttoi(GetEditingValue(nRow, nCol));
		
		SetStyleRange(CGXRange().SetCells(ROWINDEX_END, COLINDEX_HD_END_NO), CGXStyle().SetEnabled(bCheck));
		SetStyleRange(CGXRange().SetCells(ROWINDEX_END, COLINDEX_NAME     ), CGXStyle().SetEnabled(bCheck));
		SetStyleRange(CGXRange().SetCells(ROWINDEX_END, COLINDEX_SPACE    ), CGXStyle().SetEnabled(bCheck));
		
		m_pParent->ShowBitmap(bCheck);
	}

	return bRet;
}

void CDgnConWallRebarGrid::OnCanceledEditing(ROWCOL nRow, ROWCOL nCol)
{
	CGXGridWnd::OnCanceledEditing(nRow, nCol);
}

BOOL CDgnConWallRebarGrid::Copy( )
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

BOOL CDgnConWallRebarGrid::Paste()
{
	BOOL bReturn = CGXGridWnd::Paste();

	return bReturn;
}

BOOL CDgnConWallRebarGrid::OnPasteFromClipboard(const CGXRange &range)
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
			//sErrMsg = ")붙여 넣을 범위가 칼럼 범위를 초과합니다. 위치를 다시 지정하세요._T(";
			sErrMsg = _LS(IDS_CMD_PASTE_EXCEED_COLUMN);
			goto END_OF_PASTE;
		}
		// 대상 범위가 Row 한계를 넘어서면 에러
		if (nRow + r.GetHeight() - 1 > GetRowCount())
		{
			//sErrMsg = ")붙여 넣을 범위가 로우 범위를 초과합니다. 위치를 다시 지정하세요._T(";
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
				//sErrMsg = ")대상 범위를 지정한 경우는 원본 범위의 크기와 같아야 합니다._T(";
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

BOOL CDgnConWallRebarGrid::ProcessKeys(CWnd *pSender, UINT nMessage, UINT nChar, UINT nRepCnt, UINT flags)
{
	BOOL bCtl = GetKeyState(VK_CONTROL) & 0x8000;
	BOOL bShift = GetKeyState(VK_SHIFT) & 0x8000;

	if (bCtl || bShift) 
		return CGXGridWnd::ProcessKeys(pSender, nMessage, nChar, nRepCnt, flags);

	if (nMessage == WM_KEYDOWN)
	{
		switch(nChar)
		{
		case VK_INSERT: 
		case VK_DELETE: 
			return TRUE;
		}
	}
	return CGXGridWnd::ProcessKeys(pSender, nMessage, nChar, nRepCnt, flags);
}

BOOL CDgnConWallRebarGrid::GetStyleRowCol(ROWCOL nRow, ROWCOL nCol, CGXStyle& style, GXModifyType mt, int nType)
{
	BOOL bCurrentRow;
	BOOL bCurrentCell;
	BOOL bEnabled;
	BOOL bHeaderCol;
	BOOL bHeaderRow;

	ROWCOL ncRow, ncCol;    // 현재 Row, Col
	CGXGridWnd::GetStyleRowCol(nRow, nCol, style, mt, nType);

	// 현재 셀의 그리드 좌표(Row, Col 번호)를 얻어온다.
	if (!GetCurrentCell(ncRow, ncCol)) 
		ncRow = GetRowCount()+1; // 현재 셀이 없는 경우 : ncRow를 범위 밖으로 설정

	// 현재 셀이 이동되면 새로 그려준다.
	m_bRefreshOnSetCurrentCell = TRUE;

	// Row나 Col이 1보다 작으면 리턴한다.(Standard Row or Column Header)
	if (nRow < 1 || nCol < 1) return TRUE;
	// 사용자 정의 된 Header도 리턴
	if (nRow == ROWINDEX_HEADER2) return TRUE;
	else if((nRow == ROWINDEX_VERTICAL || nRow == ROWINDEX_HORIZONTAL || nRow == ROWINDEX_BEHORIZONTAL) && nCol == COLINDEX_HD_CHECK)  return TRUE;
	else if(nRow = ROWINDEX_END && nCol == COLINDEX_HD_END)   return TRUE;

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
//     style
//       .SetInterior(CTBGrid::m_Color.NormalModeBg)
//       .SetTextColor(CTBGrid::m_Color.NormalModeFg);

		return TRUE;
	}

	// Disable되었을 때
	if (!bEnabled)  
	{
		if (bHeaderCol || bHeaderRow) style           // Row or Column Header
			.SetControl(GX_IDS_CTRL_HEADER)
			.SetInterior(CTBGrid::m_Color.RowColHeaderBg)
			.SetTextColor(CTBGrid::m_Color.NormalModeFg);
//     else if (bCurrentRow) style     // Disabled Current Row
//       .SetInterior(CTBGrid::m_Color.DisabledBg)
//       .SetTextColor(CTBGrid::m_Color.ReadOnlyCurrentRowFg);
		else style                      // Disabled Cell
			.SetInterior(CTBGrid::m_Color.DisabledBg)
			.SetTextColor(CTBGrid::m_Color.DisabledFg);

		return TRUE;
	}

	if (bCurrentCell) style
		.SetInterior(CTBGrid::m_Color.NormalCurrentCellBg)
		.SetTextColor(CTBGrid::m_Color.NormalCurrentCellFg);
//   else if (bCurrentRow) style
//     .SetInterior(CTBGrid::m_Color.NormalModeBg)
//     .SetTextColor(CTBGrid::m_Color.NormalModeFg);
	else if (bHeaderCol || bHeaderRow) style    // Enabled Header Col(Only Frozen)
		.SetInterior(CTBGrid::m_Color.NormalModeBg)
		.SetTextColor(CTBGrid::m_Color.NormalModeFg);

	return TRUE;
}

void CDgnConWallRebarGrid::UpdateUnit()
{
	BOOL bOldLock = LockUpdate(TRUE);
	
	DoCancelMode();
	CString strListSpace = Get_ListSpaceSubBar();
	CString strSpace;
	
	T_UNIT_INDEX UnitIndex;
	m_pUnitCtrl->GetUnitIndexCurrent(UnitIndex);
	
	for (int k=0; k<4; k++)
	{
		m_dSpace[k] = m_pUnitCtrl->ConvertUnitDataPrevious(D_UNITSYS_BASE_LENGTH, m_dSpace[k]);

		SetStyleRange(CGXRange().SetCells(ROWINDEX_VERTICAL+k, COLINDEX_SPACE), CGXStyle().SetChoiceList(strListSpace));  
		strSpace = GetEditingValue(ROWINDEX_VERTICAL+k, COLINDEX_SPACE);
		
		if(UnitIndex.nBase_Length == D_UNITSYS_LENGTH_INDEX_MM || UnitIndex.nBase_Length == D_UNITSYS_LENGTH_INDEX_CM ||
			UnitIndex.nBase_Length == D_UNITSYS_LENGTH_INDEX_M)
		{
			if(strSpace.Find(_T("\\")) > -1)
			{
				double dSpace = m_pUnitCtrl->ConvertCurUnitData2TgtUnit(0, D_UNITSYS_LENGTH_INDEX_MM, D_UNITSYS_BASE_LENGTH, m_dSpace[k]);
				strSpace.Format(_T("%g"), dSpace); 
			}
		}
		else if(UnitIndex.nBase_Length == D_UNITSYS_LENGTH_INDEX_IN || UnitIndex.nBase_Length == D_UNITSYS_LENGTH_INDEX_FT)
		{
			if(strSpace.Find(_T("\\")) == -1)
			{
				double dSpace = m_pUnitCtrl->ConvertCurUnitData2TgtUnit(0, D_UNITSYS_LENGTH_INDEX_IN, D_UNITSYS_BASE_LENGTH, m_dSpace[k]);
				strSpace.Format(_T("%g"), dSpace); 
			}
		}
		else   {  ASSERT(0);		return ;	}
		
		SetValueRange(CGXRange(ROWINDEX_VERTICAL+k, COLINDEX_SPACE), strSpace);
	}
	
	LockUpdate(bOldLock);
	if (!bOldLock) Redraw();
}

void CDgnConWallRebarGrid::SetEnableByRcStructWallOpt()
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();  
	BOOL bEnable = pDoc->m_pAttrCtrl->IsWallEndHorBarCondition();

	SetStyleRange(CGXRange(ROWINDEX_BEHORIZONTAL, COLINDEX_NAME), CGXStyle().SetEnabled(bEnable));
	SetStyleRange(CGXRange(ROWINDEX_BEHORIZONTAL, COLINDEX_SPACE), CGXStyle().SetEnabled(bEnable));
}