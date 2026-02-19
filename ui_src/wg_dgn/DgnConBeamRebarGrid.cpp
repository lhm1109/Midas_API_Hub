// DgnConBeamRebarGrid.cpp: implementation of the CDgnConBeamRebarGrid class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnConBeamRebarGrid.h"
#include "DgnConBeamNewDlg.h"
#include "DgnDataCtrl.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_DBLib.h"
#include "..\wg_db\wg_db_UnitCtrl.h"

#include "..\wg_common\wg_common_TBGrid.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define D_MAX_ROW 7
#define D_MAX_COL 12

enum RowIndex
{ ROWINDEX_HEADER        = 0,     ROWINDEX_HEADER2  = 1,
ROWINDEX_TOP1    = 2, ROWINDEX_TOP2    = 3, ROWINDEX_BOT2     = 4, 
ROWINDEX_BOT1     = 5, ROWINDEX_STIRRUP     = 6, ROWINDEX_SKIN = 7,
};

enum ColumnIndex
{ COLINDEX_HEADER    =  0,
COLINDEX_HD_MAIN  =  1, COLINDEX_HD_TOP   =  2, COLINDEX_HD_LAYER = 3,

COLINDEX_END_I_NO  =  4, COLINDEX_END_I_SYMBOL  =  5, COLINDEX_END_I_SPACE = 6,
COLINDEX_MIDDLE_NO =  7, COLINDEX_MIDDLE_SYMBOL =  8, COLINDEX_MIDDLE_SPACE = 9,
COLINDEX_END_J_NO  = 10, COLINDEX_END_J_SYMBOL  = 11, COLINDEX_END_J_SPACE = 12
};

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDgnConBeamRebarGrid::CDgnConBeamRebarGrid()
{
	m_bSameTopBot = TRUE;
	m_bSameIMJ = TRUE;
	m_bSameLayer = TRUE;
	m_nWndWidth = 0;
	m_nWndHeight = 0;

	memset(m_dSpace, 0, sizeof(m_dSpace));
	m_pUnitCtrl = CDBDoc::GetDocPoint()->m_pUnitCtrl;
}

// CDgnConBeamRebarGrid::~CDgnConBeamRebarGrid()
// {
// 
// }

//////////////////////////////////////////////////////////////////////
// Interface
void CDgnConBeamRebarGrid::EnableGrid(int nRow, int nCol, BOOL bEnable)
{
	SetStyleRange(CGXRange().SetCells(nRow, nCol), CGXStyle().SetEnabled(bEnable));
}

void CDgnConBeamRebarGrid::Initialize(CWnd* pParent, int nArrangeType)
{
	m_pParent = (CDgnConBeamNewDlg*)pParent;

	CGXGridWnd::Initialize();
	
	LockUpdate(TRUE);
	
	// cell 바깥영역 배경색 지정 
	GetParam()->GetProperties()->SetColor(GX_COLOR_BACKGROUND, CTBGrid::m_Color.OuterBackground);
	
	// Header 배경색 지정 
	ChangeColHeaderStyle(CGXStyle().SetInterior(CTBGrid::m_Color.RowColHeaderBg)); 
	ChangeRowHeaderStyle(CGXStyle().SetInterior(CTBGrid::m_Color.RowColHeaderBg)); 
	
	// 기타 Cells 배경색 지정
	ChangeStandardStyle(CGXStyle( ).SetInterior(CTBGrid::m_Color.NormalModeBg));

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
	int nColNum=0;
	m_nArrangeType = nArrangeType;
	switch(nArrangeType)
	{
		case 0: nColNum =  6; break;
		case 1: nColNum =  9; break;
		case 2: nColNum = D_MAX_COL; break;
		default: ASSERT(0); break;
	}
	SetColCount(nColNum); // Number of column excluding header
	SetRowCount(D_MAX_ROW); // Number of row excluding header
	
	// Row_Grid Size
	SetRowHeight(0, D_MAX_ROW, m_nWndHeight/(D_MAX_ROW/*+1*/));
	SetSizeColumn(nArrangeType);
	
	// Set Title
	SetHeaderTitle(nArrangeType);
	SetDataColumn(nArrangeType);
	
	HideCols(0, 0, TRUE);
	HideRows(0, 0, TRUE);
	SetScrollBarMode(SB_VERT, gxnDisabled);
	
	const bool bUseSkin = UseSkin();
	SetStyleRange(CGXRange().SetRows(ROWINDEX_SKIN), CGXStyle().SetEnabled(bUseSkin));
	
	LockUpdate(FALSE);
	Redraw();

	T_REBB_D RebbD;     RebbD.Initialize();
	Data2Grid(&RebbD);

	LockUpdate(FALSE);
}

BOOL CDgnConBeamRebarGrid::UseSkin()
{
	CDgnDataCtrl DataCtrl;
	T_DCON_D DconD;
	if(!DataCtrl.Get_DgnConDcon(DconD)) ASSERT(0);

	CString strDgnCode = DconD.DesignCode;
	return CDBLib::IsConCodeForTorsionCheck(strDgnCode);
}

void CDgnConBeamRebarGrid::SetSizeColumn(int nArrangeType)
{
	double dRatio = (double)m_nWndWidth/369.0;      // 369 = 국문 OS 에서의 넓이
	int nSizeMain   = 28 * dRatio;
	int nSizeTop   = 35 * dRatio;
	int nSizeNo     =  35 * dRatio;
	int nSizeLayer  =  42 * dRatio;
	int nSizeSpace  =  36 * dRatio;
	int nSizeSymbol =  15 * dRatio;
	int nSizeBar    =  nSizeSpace + nSizeSymbol;

	// PARKHJ-20111025 : Rebar Grid에서 철근 개수가 두자리수 일때 한자리수만 보이는 문제.
	//                   해결을 위해 Grid 너비 조정.
	BOOL bTaiwan = FALSE;
	if((CProduct::IsMovingType(D_PRODUCT_MOVING_US) || CProduct::IsMovingType(D_PRODUCT_MOVING_RUS)) && CProduct::IsTaiwan()) bTaiwan = TRUE;
	if (bTaiwan)
	{
		nSizeMain   = 21 * dRatio;
		nSizeTop    = 27 * dRatio;
		nSizeNo     = 40 * dRatio;
		nSizeLayer  = 42 * dRatio;
		nSizeSpace  = 36 * dRatio;
		nSizeSymbol = 15 * dRatio;
		nSizeBar    = nSizeSpace + nSizeSymbol;
	}
	
	// Col_Grid Size
	SetColWidth(COLINDEX_HEADER      , COLINDEX_HEADER      , 0           );
	SetColWidth(COLINDEX_HD_MAIN     , COLINDEX_HD_MAIN     , nSizeMain   );
	SetColWidth(COLINDEX_HD_TOP      , COLINDEX_HD_TOP      , nSizeTop    );
	SetColWidth(COLINDEX_HD_LAYER    , COLINDEX_HD_LAYER    , nSizeLayer  );
	SetColWidth(COLINDEX_END_I_NO    , COLINDEX_END_I_NO    , nSizeNo     );
	SetColWidth(COLINDEX_END_I_SYMBOL, COLINDEX_END_I_SYMBOL, nSizeSymbol );
	SetColWidth(COLINDEX_END_I_SPACE , COLINDEX_END_I_SPACE , nSizeSpace  );
	
	if ((nArrangeType == 1) || (nArrangeType == 2))
	{
		SetColWidth(COLINDEX_MIDDLE_NO    , COLINDEX_MIDDLE_NO    , nSizeNo    );
		SetColWidth(COLINDEX_MIDDLE_SYMBOL, COLINDEX_MIDDLE_SYMBOL, nSizeSymbol);
		SetColWidth(COLINDEX_MIDDLE_SPACE , COLINDEX_MIDDLE_SPACE , nSizeSpace );
	}
	
	if (nArrangeType == 2)
	{
		SetColWidth(COLINDEX_END_J_NO    , COLINDEX_END_J_NO    , nSizeNo    );
		SetColWidth(COLINDEX_END_J_SYMBOL, COLINDEX_END_J_SYMBOL, nSizeSymbol);
		SetColWidth(COLINDEX_END_J_SPACE , COLINDEX_END_J_SPACE , nSizeSpace );
	}
}

void CDgnConBeamRebarGrid::SetDataColumn(int nArrangeType)
{
	CString strListRebar = Get_ListRebar();
	SetStyleRange(CGXRange().SetCells(ROWINDEX_STIRRUP, COLINDEX_HD_LAYER), CGXStyle()
		.SetControl(GX_IDS_CTRL_CBS_DROPDOWNLIST)
		.SetValueType(GX_VT_STRING)
		.SetEnabled(TRUE)
		.SetHorizontalAlignment(DT_CENTER)
		.SetChoiceList(strListRebar)
		);
	SetStyleRange(CGXRange().SetCells(ROWINDEX_SKIN, COLINDEX_HD_LAYER), CGXStyle()
		.SetControl(GX_IDS_CTRL_CBS_DROPDOWNLIST)
		.SetValueType(GX_VT_STRING)
//    .SetEnabled(TRUE)
		.SetHorizontalAlignment(DT_CENTER)
		.SetChoiceList(strListRebar)
		);

	SetDataColumnSection(COLINDEX_END_I_NO-1);
	
	if ((nArrangeType == 1) || (nArrangeType == 2))
	{
		SetDataColumnSection(COLINDEX_MIDDLE_NO-1);
	}
	
	if (nArrangeType == 2)
	{
		SetDataColumnSection(COLINDEX_END_J_NO-1);
	}
}

void CDgnConBeamRebarGrid::SetDataColumnSection(ROWCOL nCol)
{
	CString strListRebar = Get_ListRebar();
	CString strListSpace = Get_ListSpaceSubBar();
	
	SetCoveredCellsRowCol(ROWINDEX_TOP1,  nCol+2, ROWINDEX_TOP1,  nCol+3);
	SetCoveredCellsRowCol(ROWINDEX_TOP2,  nCol+2, ROWINDEX_TOP2,  nCol+3);
	SetCoveredCellsRowCol(ROWINDEX_BOT2,  nCol+2, ROWINDEX_BOT2,  nCol+3);
	SetCoveredCellsRowCol(ROWINDEX_BOT1,  nCol+2, ROWINDEX_BOT1,  nCol+3);
	SetCoveredCellsRowCol(ROWINDEX_SKIN,  nCol+1, ROWINDEX_SKIN,  nCol+3);

	SetStyleRange(CGXRange().SetCells(ROWINDEX_TOP1, nCol+1), CGXStyle()
		.SetControl(GX_IDS_CTRL_SPINEDIT)
		.SetValueType(GX_VT_NUMERIC)
		.SetEnabled(TRUE)
		.SetHorizontalAlignment(DT_RIGHT));
	SetStyleRange(CGXRange().SetCells(ROWINDEX_TOP2, nCol+1), CGXStyle()
		.SetControl(GX_IDS_CTRL_SPINEDIT)
		.SetValueType(GX_VT_NUMERIC)
		.SetEnabled(TRUE)
		.SetHorizontalAlignment(DT_RIGHT));
	SetStyleRange(CGXRange().SetCells(ROWINDEX_TOP1, nCol+2), CGXStyle()
		.SetControl(GX_IDS_CTRL_CBS_DROPDOWNLIST)
		.SetValueType(GX_VT_STRING)
		.SetEnabled(TRUE)
		.SetHorizontalAlignment(DT_CENTER)
		.SetChoiceList(strListRebar));
	SetStyleRange(CGXRange().SetCells(ROWINDEX_TOP2, nCol+2), CGXStyle()
		.SetControl(GX_IDS_CTRL_CBS_DROPDOWNLIST)
		.SetValueType(GX_VT_STRING)
		.SetEnabled(TRUE)
		.SetHorizontalAlignment(DT_CENTER)
		.SetChoiceList(strListRebar));

	SetStyleRange(CGXRange().SetCells(ROWINDEX_BOT2, nCol+1), CGXStyle()
		.SetControl(GX_IDS_CTRL_SPINEDIT)
		.SetValueType(GX_VT_NUMERIC)
		.SetEnabled(TRUE)
		.SetHorizontalAlignment(DT_RIGHT));
	SetStyleRange(CGXRange().SetCells(ROWINDEX_BOT1, nCol+1), CGXStyle()
		.SetControl(GX_IDS_CTRL_SPINEDIT)
		.SetValueType(GX_VT_NUMERIC)
		.SetEnabled(TRUE)
		.SetHorizontalAlignment(DT_RIGHT));
	SetStyleRange(CGXRange().SetCells(ROWINDEX_BOT2, nCol+2), CGXStyle()
		.SetControl(GX_IDS_CTRL_CBS_DROPDOWNLIST)
		.SetValueType(GX_VT_STRING)
		.SetEnabled(TRUE)
		.SetHorizontalAlignment(DT_CENTER)
		.SetChoiceList(strListRebar));
	SetStyleRange(CGXRange().SetCells(ROWINDEX_BOT1, nCol+2), CGXStyle()
		.SetControl(GX_IDS_CTRL_CBS_DROPDOWNLIST)
		.SetValueType(GX_VT_STRING)
		.SetEnabled(TRUE)
		.SetHorizontalAlignment(DT_CENTER)
		.SetChoiceList(strListRebar));

	SetStyleRange(CGXRange().SetCells(ROWINDEX_STIRRUP, nCol+1), CGXStyle()
		.SetControl(GX_IDS_CTRL_SPINEDIT)
		.SetValueType(GX_VT_NUMERIC)
		.SetEnabled(TRUE)
		.SetHorizontalAlignment(DT_RIGHT));
	SetStyleRange(CGXRange().SetCells(ROWINDEX_STIRRUP, nCol+2), CGXStyle()
		.SetControl(GX_IDS_CTRL_EDIT)
		.SetValueType(GX_VT_STRING)
		.SetValue(_T("@"))
		.SetReadOnly(TRUE)
		.SetHorizontalAlignment(DT_RIGHT));
	SetStyleRange(CGXRange().SetCells(ROWINDEX_STIRRUP, nCol+3), CGXStyle()
		.SetControl(GX_IDS_CTRL_CBS_DROPDOWN)
		.SetValueType(GX_VT_STRING)
		.SetHorizontalAlignment(DT_CENTER)
		.SetChoiceList(strListSpace)
		.SetFormat(GX_FMT_FIXED).SetPlaces(2));
	SetStyleRange(CGXRange().SetCells(ROWINDEX_SKIN, nCol+1), CGXStyle()
		.SetControl(GX_IDS_CTRL_SPINEDIT)
		.SetValueType(GX_VT_NUMERIC)
//    .SetEnabled(TRUE)
		.SetHorizontalAlignment(DT_RIGHT));
}

void CDgnConBeamRebarGrid::Data2Grid(T_REBB_D* pData)
{
	SetStyleRange(CGXRange(ROWINDEX_STIRRUP,COLINDEX_HD_LAYER), CGXStyle().SetValue(pData->SubRebarName    ));
	SetStyleRange(CGXRange(ROWINDEX_SKIN   ,COLINDEX_HD_LAYER), CGXStyle().SetValue(pData->strSkinName       ));

	Data2GridSection(COLINDEX_END_I_NO -1, &(pData->Top_I), &(pData->Bot_I));
	if (m_nArrangeType==1 || m_nArrangeType==2) Data2GridSection(COLINDEX_MIDDLE_NO-1, &(pData->Top_C), &(pData->Bot_C));
	if (m_nArrangeType==2)                    Data2GridSection(COLINDEX_END_J_NO -1, &(pData->Top_J), &(pData->Bot_J));

	m_dSpace[0] = pData->Top_I.dSubRebarSpace;
	m_dSpace[1] = pData->Top_C.dSubRebarSpace;
	m_dSpace[2] = pData->Top_J.dSubRebarSpace;
}

void CDgnConBeamRebarGrid::Data2GridSection(ROWCOL nCol, T_REBB_BASE* pTop, T_REBB_BASE* pBot)
{
	SetStyleRange(CGXRange(ROWINDEX_TOP1   ,nCol+1), CGXStyle().SetValue((UINT)pTop->nRebar1     ));
	SetStyleRange(CGXRange(ROWINDEX_TOP2   ,nCol+1), CGXStyle().SetValue((UINT)pTop->nRebar2     ));
	SetStyleRange(CGXRange(ROWINDEX_TOP1   ,nCol+2), CGXStyle().SetValue(pTop->RebarName         ));
	SetStyleRange(CGXRange(ROWINDEX_TOP2   ,nCol+2), CGXStyle().SetValue(pTop->RebarName2nd      ));
	SetStyleRange(CGXRange(ROWINDEX_BOT2   ,nCol+1), CGXStyle().SetValue((UINT)pBot->nRebar2     ));
	SetStyleRange(CGXRange(ROWINDEX_BOT1   ,nCol+1), CGXStyle().SetValue((UINT)pBot->nRebar1     ));
	SetStyleRange(CGXRange(ROWINDEX_BOT2   ,nCol+2), CGXStyle().SetValue(pBot->RebarName2nd      ));
	SetStyleRange(CGXRange(ROWINDEX_BOT1   ,nCol+2), CGXStyle().SetValue(pBot->RebarName         ));
	SetStyleRange(CGXRange(ROWINDEX_STIRRUP,nCol+1), CGXStyle().SetValue((UINT)pTop->iSubRebarNum));
	CString strSpace;
	T_UNIT_INDEX UnitIndex;
	m_pUnitCtrl->GetUnitIndexCurrent(UnitIndex);
	if(UnitIndex.nBase_Length == D_UNITSYS_LENGTH_INDEX_MM || UnitIndex.nBase_Length == D_UNITSYS_LENGTH_INDEX_CM ||
		 UnitIndex.nBase_Length == D_UNITSYS_LENGTH_INDEX_M)
	{
		double dSpace = m_pUnitCtrl->ConvertCurUnitData2TgtUnit(0, D_UNITSYS_LENGTH_INDEX_MM, D_UNITSYS_BASE_LENGTH, pTop->dSubRebarSpace);
		strSpace.Format(_T("%g"), dSpace); 
	}
	else if(UnitIndex.nBase_Length == D_UNITSYS_LENGTH_INDEX_IN || UnitIndex.nBase_Length == D_UNITSYS_LENGTH_INDEX_FT)
	{
		double dSpace = m_pUnitCtrl->ConvertCurUnitData2TgtUnit(0, D_UNITSYS_LENGTH_INDEX_IN, D_UNITSYS_BASE_LENGTH, pTop->dSubRebarSpace);
		strSpace.Format(_T("%g"), dSpace); 
	}
	else   {  ASSERT(0);		return ;	}
	SetStyleRange(CGXRange(ROWINDEX_STIRRUP,nCol+3), CGXStyle().SetValue(strSpace));

	SetStyleRange(CGXRange(ROWINDEX_SKIN,nCol+1), CGXStyle().SetValue((UINT)pTop->nSkin));
}

void CDgnConBeamRebarGrid::Grid2Data(T_REBB_D* pData)
{
	pData->SubRebarName = GetEditingValue(ROWINDEX_STIRRUP, COLINDEX_HD_LAYER);
	pData->strSkinName    = GetEditingValue(ROWINDEX_SKIN   , COLINDEX_HD_LAYER);

	Grid2Data_Section(COLINDEX_END_I_NO -1, &(pData->Top_I), &(pData->Bot_I));
	if(m_nArrangeType==1 || m_nArrangeType==2) Grid2Data_Section(COLINDEX_MIDDLE_NO-1, &(pData->Top_C), &(pData->Bot_C));
	if(m_nArrangeType==2)                    Grid2Data_Section(COLINDEX_END_J_NO -1, &(pData->Top_J), &(pData->Bot_J));

	if(m_nArrangeType==0 || m_nArrangeType==1) {  pData->Top_J = pData->Top_I;    pData->Bot_J = pData->Bot_I;    }
	if(m_nArrangeType==0)                      {  pData->Top_C = pData->Top_I;    pData->Bot_C = pData->Bot_I;    }

	pData->Top_I.dSubRebarSpace = m_dSpace[0];
	pData->Top_C.dSubRebarSpace = m_dSpace[1];
	pData->Top_J.dSubRebarSpace = m_dSpace[2];
}


void CDgnConBeamRebarGrid::Grid2Data_Section(ROWCOL nCol, T_REBB_BASE* pTop, T_REBB_BASE* pBot)
{	
	pTop->nRebar1        = _ttoi(GetEditingValue(ROWINDEX_TOP1   , nCol+1));
	pTop->nRebar2        = _ttoi(GetEditingValue(ROWINDEX_TOP2   , nCol+1));
	pTop->RebarName      = GetEditingValue(ROWINDEX_TOP1   , nCol+2);
	pTop->RebarName2nd   = GetEditingValue(ROWINDEX_TOP2   , nCol+2);
	pBot->nRebar2        = _ttoi(GetEditingValue(ROWINDEX_BOT2   , nCol+1));
	pBot->nRebar1        = _ttoi(GetEditingValue(ROWINDEX_BOT1   , nCol+1));
	pBot->RebarName      = GetEditingValue(ROWINDEX_BOT1   , nCol+2);
	pBot->RebarName2nd   = GetEditingValue(ROWINDEX_BOT2   , nCol+2);
	pTop->iSubRebarNum   = _ttoi(GetEditingValue(ROWINDEX_STIRRUP, nCol+1));
	pTop->nSkin   = _ttoi(GetEditingValue(ROWINDEX_SKIN, nCol+1));
}

CString CDgnConBeamRebarGrid::Get_ListRebar()
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

CString CDgnConBeamRebarGrid::Get_ListSpaceSubBar()
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

int CDgnConBeamRebarGrid::GetColumnNum()
{
	return 12;
}

void CDgnConBeamRebarGrid::SetHeaderTitle(int nArrangeType)
{
	int iFontSizeHeader  = 8;
	int iFontSizeNormal  = 8;
	BOOL bFontBoldHeader = FALSE;
	BOOL bFontBoldNormal = FALSE;
	
	SetCoveredCellsRowCol(ROWINDEX_HEADER2, COLINDEX_HD_MAIN  ,  ROWINDEX_HEADER2, COLINDEX_HD_LAYER    );
	SetCoveredCellsRowCol(ROWINDEX_HEADER2, COLINDEX_END_I_NO ,  ROWINDEX_HEADER2, COLINDEX_END_I_SPACE );
	SetCoveredCellsRowCol(ROWINDEX_HEADER2, COLINDEX_MIDDLE_NO,  ROWINDEX_HEADER2, COLINDEX_MIDDLE_SPACE);
	SetCoveredCellsRowCol(ROWINDEX_HEADER2, COLINDEX_END_J_NO ,  ROWINDEX_HEADER2, COLINDEX_END_J_SPACE );

	CGXStyle style;
	style.SetControl(GX_IDS_CTRL_STATIC);
	style.SetDraw3dFrame(gxFrameRaised);
	style.SetFont(CGXFont_GC().SetBold(FALSE).SetSize(9));
	style.SetInterior(ColHeaderStyle().GetInterior()); 

	style.SetValue(_LS(IDS_DGN_REBAR));
	SetStyleRange(CGXRange(ROWINDEX_HEADER2, COLINDEX_HD_MAIN), style);
	
	CString strEndName = _T("");
	switch(nArrangeType)
	{
		case 0: strEndName = _LS(IDS_DGN_ALL_SECTION); break;
		case 1: strEndName = _LS(IDS_DGN_BOTH_END);    break;
		case 2: strEndName = _LS(IDS_DGN_END_I);      break;
		default: ASSERT(0); break;
	}
	style.SetValue(strEndName);
	SetStyleRange(CGXRange(ROWINDEX_HEADER2, COLINDEX_END_I_NO), style);
	
	if ((nArrangeType==1)||(nArrangeType==2))
	{
		style.SetValue(_LS(IDS_DGN_CENTER));
		SetStyleRange(CGXRange(ROWINDEX_HEADER2, COLINDEX_MIDDLE_NO), style);
	}
	
	if (nArrangeType==2)
	{
		style.SetValue(_LS(IDS_DGN_END_J));
		SetStyleRange(CGXRange(ROWINDEX_HEADER2, COLINDEX_END_J_NO), style);
	}
	
	SetCoveredCellsRowCol(ROWINDEX_TOP1, COLINDEX_HD_MAIN ,  ROWINDEX_BOT1, COLINDEX_HD_MAIN);
	SetCoveredCellsRowCol(ROWINDEX_TOP1, COLINDEX_HD_TOP  ,  ROWINDEX_TOP2, COLINDEX_HD_TOP );
	SetCoveredCellsRowCol(ROWINDEX_BOT2, COLINDEX_HD_TOP  ,  ROWINDEX_BOT1, COLINDEX_HD_TOP );
	SetCoveredCellsRowCol(ROWINDEX_STIRRUP  , COLINDEX_HD_MAIN ,  ROWINDEX_STIRRUP  , COLINDEX_HD_TOP );
	SetCoveredCellsRowCol(ROWINDEX_SKIN     , COLINDEX_HD_MAIN ,  ROWINDEX_SKIN     , COLINDEX_HD_TOP );

#if defined(_ORG) || defined(_US) || defined(_RUS)
	style.SetValue(_LSX(m\na\ni\nn));
#else
	style.SetValue(_LS(IDS_DGN_MAIN));
#endif
	SetStyleRange(CGXRange(ROWINDEX_TOP1, COLINDEX_HD_MAIN), style);
	style.SetValue(_LS(IDS_DGN_TOP));
	SetStyleRange(CGXRange(ROWINDEX_TOP1, COLINDEX_HD_TOP), style);
	style.SetValue(_LSX(1));
	SetStyleRange(CGXRange(ROWINDEX_TOP1, COLINDEX_HD_LAYER), style);
	style.SetValue(_LSX(2));
	SetStyleRange(CGXRange(ROWINDEX_TOP2, COLINDEX_HD_LAYER), style);
	style.SetValue(_LS(IDS_DGN_BOT));
	SetStyleRange(CGXRange(ROWINDEX_BOT2, COLINDEX_HD_TOP), style);
	style.SetValue(_LSX(2));
	SetStyleRange(CGXRange(ROWINDEX_BOT2, COLINDEX_HD_LAYER), style);
	style.SetValue(_LSX(1));
	SetStyleRange(CGXRange(ROWINDEX_BOT1, COLINDEX_HD_LAYER), style);
	style.SetValue(_LS(IDS_DGN_STIRRUP));
	SetStyleRange(CGXRange(ROWINDEX_STIRRUP, COLINDEX_HD_MAIN), style);
	style.SetValue(_LS(IDS_DGN_SKIN));
	SetStyleRange(CGXRange(ROWINDEX_SKIN, COLINDEX_HD_MAIN), style);
}

CString CDgnConBeamRebarGrid::GetEditingValue(ROWCOL nRow, ROWCOL nCol)
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

void CDgnConBeamRebarGrid::SetSameSizeSetting(BOOL bSameTopBot, BOOL bSameIMJ, BOOL bSameLayer)
{
	m_bSameTopBot = bSameTopBot;
	m_bSameIMJ = bSameIMJ;
	m_bSameLayer = bSameLayer;

	if(bSameTopBot && bSameIMJ)
	{
		OnModifyCell(ROWINDEX_TOP1, COLINDEX_END_I_SYMBOL);
		OnModifyCell(ROWINDEX_TOP2, COLINDEX_END_I_SYMBOL);
	}
	else if(bSameTopBot)      
	{
		OnModifyCell(ROWINDEX_TOP1, COLINDEX_END_I_SYMBOL);
		OnModifyCell(ROWINDEX_TOP2, COLINDEX_END_I_SYMBOL);
	}
	else if(bSameIMJ)
	{
		OnModifyCell(ROWINDEX_TOP1, COLINDEX_END_I_SYMBOL);
		OnModifyCell(ROWINDEX_BOT1, COLINDEX_END_I_SYMBOL);
		OnModifyCell(ROWINDEX_TOP2, COLINDEX_END_I_SYMBOL);
		OnModifyCell(ROWINDEX_BOT2, COLINDEX_END_I_SYMBOL);
	}
	
	if(bSameLayer)
	{
		OnModifyCell(ROWINDEX_TOP1, COLINDEX_END_I_SYMBOL);
		OnModifyCell(ROWINDEX_BOT1, COLINDEX_END_I_SYMBOL);
		if (m_nArrangeType==1 || m_nArrangeType==2)
		{
			OnModifyCell(ROWINDEX_TOP1, COLINDEX_MIDDLE_SYMBOL);
			OnModifyCell(ROWINDEX_BOT1, COLINDEX_MIDDLE_SYMBOL);
		}
		if (m_nArrangeType==2)
		{
			OnModifyCell(ROWINDEX_TOP1, COLINDEX_END_J_SYMBOL);
			OnModifyCell(ROWINDEX_BOT1, COLINDEX_END_J_SYMBOL);
		}
	}


	EnableGrid(ROWINDEX_TOP2, COLINDEX_END_I_SYMBOL, !bSameLayer);
	EnableGrid(ROWINDEX_BOT1, COLINDEX_END_I_SYMBOL, !bSameTopBot);
	EnableGrid(ROWINDEX_BOT2, COLINDEX_END_I_SYMBOL ,!(bSameLayer||bSameTopBot));
	if (m_nArrangeType==1 || m_nArrangeType==2)
	{
		EnableGrid(ROWINDEX_BOT1, COLINDEX_MIDDLE_SYMBOL, !(bSameTopBot||bSameIMJ));
		EnableGrid(ROWINDEX_TOP1, COLINDEX_MIDDLE_SYMBOL, !bSameIMJ);

		EnableGrid(ROWINDEX_BOT2, COLINDEX_MIDDLE_SYMBOL, !(bSameTopBot||bSameIMJ||bSameLayer));
		EnableGrid(ROWINDEX_TOP2, COLINDEX_MIDDLE_SYMBOL, !(bSameIMJ||bSameLayer));

		if(bSameTopBot)      
		{
			OnModifyCell(ROWINDEX_TOP1, COLINDEX_MIDDLE_SYMBOL);
			OnModifyCell(ROWINDEX_TOP2, COLINDEX_MIDDLE_SYMBOL);
		}
//    else if(bSameIMJ)      OnModifyCell(ROWINDEX_BOT1, COLINDEX_MIDDLE_SYMBOL);
	}
	if (m_nArrangeType==2)
	{
		EnableGrid(ROWINDEX_BOT1, COLINDEX_END_J_SYMBOL, !(bSameTopBot||bSameIMJ));
		EnableGrid(ROWINDEX_TOP1, COLINDEX_END_J_SYMBOL, !bSameIMJ);

		EnableGrid(ROWINDEX_BOT2, COLINDEX_END_J_SYMBOL, !(bSameTopBot||bSameIMJ||bSameLayer));
		EnableGrid(ROWINDEX_TOP2, COLINDEX_END_J_SYMBOL, !(bSameIMJ||bSameLayer));

		if(bSameTopBot)
		{
			OnModifyCell(ROWINDEX_TOP1, COLINDEX_END_J_SYMBOL);
			OnModifyCell(ROWINDEX_TOP2, COLINDEX_END_J_SYMBOL);
		}
//s    else if(bSameIMJ)      OnModifyCell(ROWINDEX_BOT1, COLINDEX_END_J_SYMBOL);
	}
}

//////////////////////////////////////////////////////////////////////
// Override

void CDgnConBeamRebarGrid::OnModifyCell(ROWCOL nRow, ROWCOL nCol)
{
	CGXGridWnd::OnModifyCell(nRow, nCol);
	if (m_bSameTopBot || m_bSameIMJ || m_bSameLayer) ChangeStatusColumn(nRow, nCol);

	if(nCol == COLINDEX_END_I_NO || nCol == COLINDEX_MIDDLE_NO || nCol == COLINDEX_END_J_NO)
	{
		int nValue = _ttoi(GetEditingValue(nRow, nCol));
		if(nValue < 0)   SetStyleRange(CGXRange(nRow, nCol), CGXStyle().SetValue((UINT)0));
	}

	T_REBB_D RebbD;
	Grid2Data(&RebbD);
	m_pParent->DrawSectionWnd(&RebbD);
}

void CDgnConBeamRebarGrid::OnCanceledEditing(ROWCOL nRow, ROWCOL nCol)
{
	CGXGridWnd::OnCanceledEditing(nRow, nCol);
	if (m_bSameTopBot || m_bSameIMJ || m_bSameLayer) ChangeStatusColumn(nRow, nCol);

	T_REBB_D RebbD;
	Grid2Data(&RebbD);
	m_pParent->DrawSectionWnd(&RebbD);
}

BOOL CDgnConBeamRebarGrid::OnEndEditing(ROWCOL nRow, ROWCOL nCol)
{  
	BOOL bRet = CGXGridWnd::OnEndEditing(nRow, nCol);

	if(nRow != ROWINDEX_STIRRUP)    return bRet;
	if(nCol != COLINDEX_END_I_SPACE && nCol != COLINDEX_MIDDLE_SPACE && nCol != COLINDEX_END_J_SPACE) return bRet;
	
	int nIndex = 0;
	if(nCol == COLINDEX_MIDDLE_SPACE)   nIndex = 1;
	else if(nCol == COLINDEX_END_J_SPACE)   nIndex = 2;

	CString strSpace= GetEditingValue(nRow, nCol);
	T_UNIT_INDEX UnitIndex;
	m_pUnitCtrl->GetUnitIndexCurrent(UnitIndex);
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

	if(nCol == COLINDEX_END_I_SPACE)
	{
		if(m_nArrangeType == 0 || m_nArrangeType == 1)    m_dSpace[2] = m_dSpace[0];
		if(m_nArrangeType == 0)                           m_dSpace[1] = m_dSpace[0];
	}

	return bRet;
}

void CDgnConBeamRebarGrid::ChangeStatusColumn(ROWCOL nRow, ROWCOL nCol)
{
	if (!m_bSameTopBot && !m_bSameIMJ && !m_bSameLayer) return;
	
	CString csVal = GetEditingValue(nRow, nCol);
	if(m_bSameTopBot && m_bSameIMJ)
	{
		if(nRow == ROWINDEX_TOP1 && nCol == COLINDEX_END_I_SYMBOL)
		{
			if(m_nArrangeType > 0)
			{
				SetStyleRange(CGXRange(ROWINDEX_TOP1, nCol+3), CGXStyle().SetValue(csVal));
				SetStyleRange(CGXRange(ROWINDEX_BOT1, nCol+3), CGXStyle().SetValue(csVal));
				if(m_bSameLayer)
				{
					SetStyleRange(CGXRange(ROWINDEX_TOP2, nCol+3), CGXStyle().SetValue(csVal));
					SetStyleRange(CGXRange(ROWINDEX_BOT2, nCol+3), CGXStyle().SetValue(csVal));
				}
			}
			if(m_nArrangeType > 1)
			{
				SetStyleRange(CGXRange(ROWINDEX_TOP1, nCol+6), CGXStyle().SetValue(csVal));
				SetStyleRange(CGXRange(ROWINDEX_BOT1, nCol+6), CGXStyle().SetValue(csVal));
				if(m_bSameLayer)
				{
					SetStyleRange(CGXRange(ROWINDEX_TOP2, nCol+6), CGXStyle().SetValue(csVal));
					SetStyleRange(CGXRange(ROWINDEX_BOT2, nCol+6), CGXStyle().SetValue(csVal));
				}
			}
			SetStyleRange(CGXRange(ROWINDEX_BOT1, nCol), CGXStyle().SetValue(csVal));
			if(m_bSameLayer)
				SetStyleRange(CGXRange(ROWINDEX_BOT2, nCol), CGXStyle().SetValue(csVal));
		}
		else if(nRow == ROWINDEX_TOP2 && nCol == COLINDEX_END_I_SYMBOL)
		{
			if(m_nArrangeType > 0)
			{
				SetStyleRange(CGXRange(ROWINDEX_TOP2, nCol+3), CGXStyle().SetValue(csVal));
				SetStyleRange(CGXRange(ROWINDEX_BOT2, nCol+3), CGXStyle().SetValue(csVal));
			}
			if(m_nArrangeType > 1)
			{
				SetStyleRange(CGXRange(ROWINDEX_TOP2, nCol+6), CGXStyle().SetValue(csVal));
				SetStyleRange(CGXRange(ROWINDEX_BOT2, nCol+6), CGXStyle().SetValue(csVal));
			}
			SetStyleRange(CGXRange(ROWINDEX_BOT2, nCol), CGXStyle().SetValue(csVal));
		}
	}
	else if(m_bSameTopBot)
	{
		if(nRow == ROWINDEX_TOP1 && 
			(nCol == COLINDEX_END_I_SYMBOL || nCol == COLINDEX_MIDDLE_SYMBOL || nCol == COLINDEX_END_J_SYMBOL))
		{
			SetStyleRange(CGXRange(ROWINDEX_BOT1, nCol), CGXStyle().SetValue(csVal));
			if(m_bSameLayer)
				SetStyleRange(CGXRange(ROWINDEX_BOT2, nCol), CGXStyle().SetValue(csVal));
		} 
		else if(nRow == ROWINDEX_TOP2 && 
					 (nCol == COLINDEX_END_I_SYMBOL || nCol == COLINDEX_MIDDLE_SYMBOL || nCol == COLINDEX_END_J_SYMBOL))
		{
			SetStyleRange(CGXRange(ROWINDEX_BOT2, nCol), CGXStyle().SetValue(csVal));
		} 
	}
	else if(m_bSameIMJ)
	{
		if(nRow == ROWINDEX_TOP1 && nCol == COLINDEX_END_I_SYMBOL)
		{
			if(m_nArrangeType > 0)
			{
				SetStyleRange(CGXRange(ROWINDEX_TOP1, nCol+3), CGXStyle().SetValue(csVal));
				if(m_bSameLayer)
					SetStyleRange(CGXRange(ROWINDEX_TOP2, nCol+3), CGXStyle().SetValue(csVal));
			}
			if(m_nArrangeType > 1)
			{
				SetStyleRange(CGXRange(ROWINDEX_TOP1, nCol+6), CGXStyle().SetValue(csVal));
				if(m_bSameLayer)
					SetStyleRange(CGXRange(ROWINDEX_TOP2, nCol+6), CGXStyle().SetValue(csVal));
			}
		} 
		else if(nRow == ROWINDEX_BOT1 && nCol == COLINDEX_END_I_SYMBOL)
		{
			if(m_nArrangeType > 0)
			{
				SetStyleRange(CGXRange(ROWINDEX_BOT1, nCol+3), CGXStyle().SetValue(csVal));
				if(m_bSameLayer)
					SetStyleRange(CGXRange(ROWINDEX_BOT2, nCol+3), CGXStyle().SetValue(csVal));
			}
			if(m_nArrangeType > 1)
			{
				SetStyleRange(CGXRange(ROWINDEX_BOT1, nCol+6), CGXStyle().SetValue(csVal));
				if(m_bSameLayer)
					SetStyleRange(CGXRange(ROWINDEX_BOT2, nCol+6), CGXStyle().SetValue(csVal));
			}
		}
		else if((nRow == ROWINDEX_TOP2 || nRow == ROWINDEX_BOT2 ) && nCol == COLINDEX_END_I_SYMBOL)
		{
			if(m_nArrangeType > 0)
				SetStyleRange(CGXRange(nRow, nCol+3), CGXStyle().SetValue(csVal));
			if(m_nArrangeType > 1)
				SetStyleRange(CGXRange(nRow, nCol+6), CGXStyle().SetValue(csVal));
		} 
	}
	
	if(m_bSameLayer)
	{
		if(nRow == ROWINDEX_TOP1 && 
			(nCol == COLINDEX_END_I_SYMBOL || nCol == COLINDEX_MIDDLE_SYMBOL || nCol == COLINDEX_END_J_SYMBOL))
		{
			SetStyleRange(CGXRange(ROWINDEX_TOP2, nCol), CGXStyle().SetValue(csVal));
		} 
		else if(nRow == ROWINDEX_BOT1 && 
			(nCol == COLINDEX_END_I_SYMBOL || nCol == COLINDEX_MIDDLE_SYMBOL || nCol == COLINDEX_END_J_SYMBOL))
		{
			SetStyleRange(CGXRange(ROWINDEX_BOT2, nCol), CGXStyle().SetValue(csVal));
		} 
	}
}

BOOL CDgnConBeamRebarGrid::Copy( )
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

BOOL CDgnConBeamRebarGrid::Paste()
{
	BOOL bReturn = CGXGridWnd::Paste();

	return bReturn;
}

BOOL CDgnConBeamRebarGrid::OnPasteFromClipboard(const CGXRange &range)
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

BOOL CDgnConBeamRebarGrid::ProcessKeys(CWnd *pSender, UINT nMessage, UINT nChar, UINT nRepCnt, UINT flags)
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

BOOL CDgnConBeamRebarGrid::GetStyleRowCol(ROWCOL nRow, ROWCOL nCol, CGXStyle& style, GXModifyType mt, int nType)
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
	if (nRow == ROWINDEX_HEADER2 || nCol < COLINDEX_HD_LAYER) return TRUE;
	if (nRow < ROWINDEX_STIRRUP && nCol == COLINDEX_HD_LAYER) return TRUE;

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

void CDgnConBeamRebarGrid::UpdateUnit()
{
	BOOL bOldLock = LockUpdate(TRUE);
	
	CString strListSpace = Get_ListSpaceSubBar();
	CString strSpace;

	T_UNIT_INDEX UnitIndex;
	m_pUnitCtrl->GetUnitIndexCurrent(UnitIndex);
	
	for (int k=0; k<3; k++)
	{
		m_dSpace[k] = m_pUnitCtrl->ConvertUnitDataPrevious(D_UNITSYS_BASE_LENGTH, m_dSpace[k]);

		if(m_nArrangeType == 0 && k > 0)    break;
		if(m_nArrangeType == 1 && k > 1)    break;

		SetStyleRange(CGXRange().SetCells(ROWINDEX_STIRRUP, COLINDEX_END_I_SPACE+k*3), CGXStyle().SetChoiceList(strListSpace));
		
		strSpace = GetEditingValue(ROWINDEX_STIRRUP, COLINDEX_END_I_SPACE+k*3);
		
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

		SetValueRange(CGXRange(ROWINDEX_STIRRUP, COLINDEX_END_I_SPACE+k*3), strSpace);
	}
	
	LockUpdate(bOldLock);
		if (!bOldLock) Redraw();
}