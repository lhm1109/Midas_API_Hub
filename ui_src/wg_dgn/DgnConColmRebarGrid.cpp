// DgnConColmRebarGrid.cpp: implementation of the CDgnConColmRebarGrid class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnConColmRebarGrid.h"
#include "DgnConColmNewDlg.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_DBLib.h"
#include "..\wg_db\wg_db_UnitCtrl.h"

#include "..\wg_common\wg_common_TBGrid.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define D_MAX_ROW 8
#define D_MAX_COL 7

enum RowIndex
{ ROWINDEX_HEADER   = 0,
	ROWINDEX_HEADER2  = 1,
	ROWINDEX_NUMBER   = 2, 
	ROWINDEX_ROWS     = 3, 
	ROWINDEX_CORNER   = 4, // Add/Modify by Seungjun PMS:4496  '20120613 RC 보/기둥 철근종류 여러 개 지정 (코너철근 등, 설계없이 검토만) 사각형 단면일 경우만.
	ROWINDEX_END_Y    = 5, 
	ROWINDEX_END_Z    = 6,
	ROWINDEX_CENTER_Y = 7, 
	ROWINDEX_CENTER_Z = 8,
};

enum ColumnIndex
{ COLINDEX_HEADER  = 0,
	COLINDEX_HD_MAIN = 1, 
	COLINDEX_HD_END  = 2, 
	COLINDEX_HD_YZ   = 3,
	COLINDEX_NUMBER  = 4, 
	COLINDEX_NAME    = 5, 
	COLINDEX_SYMBOL  = 6, 
	COLINDEX_SPACE   = 7,
};

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDgnConColmRebarGrid::CDgnConColmRebarGrid()
{
	m_bSameRebarEndNMid = TRUE;
	m_nWndWidth = 0;
	m_nWndHeight = 0;

	memset(m_dSpace, 0, sizeof(m_dSpace));
	m_pUnitCtrl = CDBDoc::GetDocPoint()->m_pUnitCtrl;
}

// CDgnConColmRebarGrid::~CDgnConColmRebarGrid()
// {
// 
// }

//////////////////////////////////////////////////////////////////////
// Interface
void CDgnConColmRebarGrid::EnableGrid(int nRow, int nCol, BOOL bEnable)
{
	SetStyleRange(CGXRange().SetCells(nRow, nCol), CGXStyle().SetEnabled(bEnable));
}

void CDgnConColmRebarGrid::Initialize(CWnd* pParent)
{
	m_pParent = (CDgnConColmNewDlg*)pParent;

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
	SetRowHeight(0, D_MAX_ROW, m_nWndHeight/(D_MAX_ROW/*+1*/));
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

	T_REBC_D RebcD;     RebcD.Initialize();
	Data2Grid(&RebcD);

	LockUpdate(FALSE);
}

void CDgnConColmRebarGrid::SetSizeColumn()
{
	double dRatio = (double)m_nWndWidth/369.0;      // 369 = 국문 OS 에서의 넓이

	int nSizeMain   = 84 * dRatio;
	int nSizeEnd   = 70 * dRatio;
	int nSizeYZ     =  35 * dRatio;
	int nSizeNo = 45 * dRatio;
	int nSizeName = 60 * dRatio;
	int nSizeSymbol =  15 * dRatio;
	int nSizeSpace    =  55 * dRatio;
	
	// Col_Grid Size
	SetColWidth(COLINDEX_HEADER   , COLINDEX_HEADER   , 0           );
	SetColWidth(COLINDEX_HD_MAIN  , COLINDEX_HD_MAIN  , nSizeMain   );
	SetColWidth(COLINDEX_HD_END   , COLINDEX_HD_END   , nSizeEnd    );
	SetColWidth(COLINDEX_HD_YZ    , COLINDEX_HD_YZ    , nSizeYZ     );
	SetColWidth(COLINDEX_NUMBER   , COLINDEX_NUMBER   , nSizeNo     );
	SetColWidth(COLINDEX_NAME     , COLINDEX_NAME     , nSizeName   );
	SetColWidth(COLINDEX_SYMBOL   , COLINDEX_SYMBOL   , nSizeSymbol );
	SetColWidth(COLINDEX_SPACE    , COLINDEX_SPACE    , nSizeSpace  );
}

void CDgnConColmRebarGrid::SetDataColumn()
{
	CString strListRebar = Get_ListRebar();
	CString strListSpace = Get_ListSpaceSubBar();
	
	// Add/Modify by Seungjun PMS:4496  '20120613 RC 보/기둥 철근종류 여러 개 지정 (코너철근 등, 설계없이 검토만) 사각형 단면일 경우만.
	SetCoveredCellsRowCol(ROWINDEX_NUMBER, COLINDEX_SYMBOL, ROWINDEX_CORNER, COLINDEX_SPACE); // main 입력 없는 부분.
	SetCoveredCellsRowCol(ROWINDEX_NUMBER, COLINDEX_NAME, ROWINDEX_ROWS, COLINDEX_NAME);      // main rebar 철근 종류 선택
	//SetCoveredCellsRowCol(ROWINDEX_CORNER, COLINDEX_NAME, ROWINDEX_CORNER, COLINDEX_NAME);  // Corner rebar 철근 종류 선택
	SetCoveredCellsRowCol(ROWINDEX_END_Y, COLINDEX_NAME, ROWINDEX_END_Z, COLINDEX_NAME);      // I,J sub-rebar 철근 종류 선택
	SetCoveredCellsRowCol(ROWINDEX_CENTER_Y, COLINDEX_NAME, ROWINDEX_CENTER_Z, COLINDEX_NAME);// Center sub-rebar 철근 종류 선택
	SetCoveredCellsRowCol(ROWINDEX_END_Y, COLINDEX_SYMBOL, ROWINDEX_END_Z, COLINDEX_SYMBOL);  // I,J @
	SetCoveredCellsRowCol(ROWINDEX_CENTER_Y, COLINDEX_SYMBOL, ROWINDEX_CENTER_Z, COLINDEX_SYMBOL);// Center @
	SetCoveredCellsRowCol(ROWINDEX_END_Y, COLINDEX_SPACE, ROWINDEX_END_Z, COLINDEX_SPACE);        // I,J Sapce
	SetCoveredCellsRowCol(ROWINDEX_CENTER_Y, COLINDEX_SPACE, ROWINDEX_CENTER_Z, COLINDEX_SPACE);  // Center Sapce

	SetStyleRange(CGXRange().SetCells(ROWINDEX_NUMBER, COLINDEX_NUMBER), CGXStyle()
		.SetControl(GX_IDS_CTRL_SPINEDIT)
		.SetValueType(GX_VT_NUMERIC)
		.SetEnabled(TRUE)
		.SetHorizontalAlignment(DT_RIGHT));
	SetStyleRange(CGXRange().SetCells(ROWINDEX_ROWS, COLINDEX_NUMBER), CGXStyle()
		.SetControl(GX_IDS_CTRL_SPINEDIT)
		.SetValueType(GX_VT_NUMERIC)
		.SetEnabled(TRUE)
		.SetHorizontalAlignment(DT_RIGHT));
	SetStyleRange(CGXRange().SetCells(ROWINDEX_NUMBER, COLINDEX_NAME), CGXStyle()
		.SetControl(GX_IDS_CTRL_CBS_DROPDOWNLIST)
		.SetValueType(GX_VT_STRING)
		.SetEnabled(TRUE)
		.SetHorizontalAlignment(DT_CENTER)
		.SetChoiceList(strListRebar));
	
	// Add/Modify by Seungjun PMS:4496  '20120613 RC 보/기둥 철근종류 여러 개 지정 (코너철근 등, 설계없이 검토만) 사각형 단면일 경우만.
	SetStyleRange(CGXRange().SetCells(ROWINDEX_CORNER, COLINDEX_NUMBER), CGXStyle()
		.SetControl(GX_IDS_CTRL_CHECKBOX3D)
		.SetValueType(GX_VT_NUMERIC)
		.SetHorizontalAlignment(DT_CENTER)
		.SetVerticalAlignment(DT_VCENTER));
	SetStyleRange(CGXRange().SetCells(ROWINDEX_CORNER, COLINDEX_NAME), CGXStyle()
		.SetControl(GX_IDS_CTRL_CBS_DROPDOWNLIST)
		.SetValueType(GX_VT_STRING)
		.SetEnabled(TRUE)
		.SetHorizontalAlignment(DT_CENTER)
		.SetChoiceList(strListRebar));

	SetStyleRange(CGXRange().SetCells(ROWINDEX_NUMBER, COLINDEX_SYMBOL), CGXStyle()
		.SetControl(GX_IDS_CTRL_STATIC)
		.SetEnabled(FALSE));

	SetStyleRange(CGXRange().SetCells(ROWINDEX_END_Y, COLINDEX_NUMBER), CGXStyle()
		.SetControl(GX_IDS_CTRL_SPINEDIT)
		.SetValueType(GX_VT_NUMERIC)
		.SetEnabled(TRUE)
		.SetHorizontalAlignment(DT_RIGHT));
	SetStyleRange(CGXRange().SetCells(ROWINDEX_END_Z, COLINDEX_NUMBER), CGXStyle()
		.SetControl(GX_IDS_CTRL_SPINEDIT)
		.SetValueType(GX_VT_NUMERIC)
		.SetEnabled(TRUE)
		.SetHorizontalAlignment(DT_RIGHT));
	SetStyleRange(CGXRange().SetCells(ROWINDEX_END_Y, COLINDEX_NAME), CGXStyle()
		.SetControl(GX_IDS_CTRL_CBS_DROPDOWNLIST)
		.SetValueType(GX_VT_STRING)
		.SetEnabled(TRUE)
		.SetHorizontalAlignment(DT_CENTER)
		.SetChoiceList(strListRebar));
	SetStyleRange(CGXRange().SetCells(ROWINDEX_END_Y, COLINDEX_SYMBOL), CGXStyle()
		.SetControl(GX_IDS_CTRL_EDIT)
		.SetValueType(GX_VT_STRING)
		.SetValue(_T("@"))
		.SetReadOnly(TRUE)
		.SetHorizontalAlignment(DT_RIGHT));
	SetStyleRange(CGXRange().SetCells(ROWINDEX_END_Y, COLINDEX_SPACE), CGXStyle()
		.SetControl(GX_IDS_CTRL_CBS_DROPDOWN)
		.SetValueType(GX_VT_STRING)
		.SetHorizontalAlignment(DT_CENTER)
		.SetChoiceList(strListSpace)
		.SetFormat(GX_FMT_FIXED).SetPlaces(2));

	SetStyleRange(CGXRange().SetCells(ROWINDEX_CENTER_Y, COLINDEX_NUMBER), CGXStyle()
		.SetControl(GX_IDS_CTRL_SPINEDIT)
		.SetValueType(GX_VT_NUMERIC)
		.SetEnabled(TRUE)
		.SetHorizontalAlignment(DT_RIGHT));
	SetStyleRange(CGXRange().SetCells(ROWINDEX_CENTER_Z, COLINDEX_NUMBER), CGXStyle()
		.SetControl(GX_IDS_CTRL_SPINEDIT)
		.SetValueType(GX_VT_NUMERIC)
		.SetEnabled(TRUE)
		.SetHorizontalAlignment(DT_RIGHT));
	SetStyleRange(CGXRange().SetCells(ROWINDEX_CENTER_Y, COLINDEX_NAME), CGXStyle()
		.SetControl(GX_IDS_CTRL_CBS_DROPDOWNLIST)
		.SetValueType(GX_VT_STRING)
		.SetEnabled(TRUE)
		.SetHorizontalAlignment(DT_CENTER)
		.SetChoiceList(strListRebar));
	SetStyleRange(CGXRange().SetCells(ROWINDEX_CENTER_Y, COLINDEX_SYMBOL), CGXStyle()
		.SetControl(GX_IDS_CTRL_EDIT)
		.SetValueType(GX_VT_STRING)
		.SetValue(_T("@"))
		.SetReadOnly(TRUE)
		.SetHorizontalAlignment(DT_RIGHT));
	SetStyleRange(CGXRange().SetCells(ROWINDEX_CENTER_Y, COLINDEX_SPACE), CGXStyle()
		.SetEnabled(FALSE)
		.SetControl(GX_IDS_CTRL_CBS_DROPDOWN)
		.SetValueType(GX_VT_STRING)
		.SetHorizontalAlignment(DT_CENTER)
		.SetChoiceList(strListSpace)
		.SetFormat(GX_FMT_FIXED).SetPlaces(2));
}

void CDgnConColmRebarGrid::SetSameRebarEndNCenter(BOOL bSameRebarEndNMid)
{
	m_bSameRebarEndNMid = bSameRebarEndNMid;
	EnableGrid(ROWINDEX_CENTER_Y, COLINDEX_SPACE, !m_bSameRebarEndNMid);
	OnEndEditing(ROWINDEX_END_Y, COLINDEX_SPACE);
}

void CDgnConColmRebarGrid::Data2Grid(T_REBC_D* pData, CString* pStrShape)
{
	if(pStrShape)   m_strShape = *pStrShape;

	SetStyleRange(CGXRange(ROWINDEX_NUMBER  ,COLINDEX_NUMBER), CGXStyle().SetValue((UINT)pData->nQrb     ));
	SetStyleRange(CGXRange(ROWINDEX_ROWS    ,COLINDEX_NUMBER), CGXStyle().SetValue((UINT)pData->nRow     ));
	SetStyleRange(CGXRange(ROWINDEX_NUMBER  ,COLINDEX_NAME  ), CGXStyle().SetValue(pData->MainRebarName         ));
	SetStyleRange(CGXRange(ROWINDEX_CORNER,COLINDEX_NUMBER), CGXStyle().SetValue((UINT)pData->bUseCornerRebar));
	if(m_strShape==_LSX(SB))
	{
		EnableGrid(ROWINDEX_CORNER,COLINDEX_NUMBER);
		EnableGrid(ROWINDEX_CORNER,COLINDEX_NAME, pData->bUseCornerRebar);
		SetStyleRange(CGXRange(ROWINDEX_CORNER,COLINDEX_NAME), CGXStyle().SetValue(pData->bUseCornerRebar ? pData->CornerRebarName : pData->MainRebarName));
	}
	else
	{
		SetStyleRange(CGXRange(ROWINDEX_CORNER,COLINDEX_NAME), CGXStyle().SetValue(_T("")));    
		EnableGrid(ROWINDEX_CORNER,COLINDEX_NUMBER, FALSE);
		EnableGrid(ROWINDEX_CORNER,COLINDEX_NAME, FALSE);
	}
	SetStyleRange(CGXRange(ROWINDEX_END_Y   ,COLINDEX_NUMBER), CGXStyle().SetValue((UINT)pData->iSubRebarNum[0] ));
	SetStyleRange(CGXRange(ROWINDEX_END_Z   ,COLINDEX_NUMBER), CGXStyle().SetValue((UINT)pData->iSubRebarNum[1] ));
	SetStyleRange(CGXRange(ROWINDEX_END_Y   ,COLINDEX_NAME  ), CGXStyle().SetValue(pData->SubRebarName          ));
	SetStyleRange(CGXRange(ROWINDEX_CENTER_Y,COLINDEX_NUMBER), CGXStyle().SetValue((UINT)pData->iSubRebarNum2[0]));
	SetStyleRange(CGXRange(ROWINDEX_CENTER_Z,COLINDEX_NUMBER), CGXStyle().SetValue((UINT)pData->iSubRebarNum2[1]));
	SetStyleRange(CGXRange(ROWINDEX_CENTER_Y,COLINDEX_NAME  ), CGXStyle().SetValue(pData->SubRebarName2         ));

	CString strEndSpace, strCenSpace;
	T_UNIT_INDEX UnitIndex;
	m_pUnitCtrl->GetUnitIndexCurrent(UnitIndex);
	if(UnitIndex.nBase_Length == D_UNITSYS_LENGTH_INDEX_MM || UnitIndex.nBase_Length == D_UNITSYS_LENGTH_INDEX_CM ||
		UnitIndex.nBase_Length == D_UNITSYS_LENGTH_INDEX_M)
	{
		double dSpace = m_pUnitCtrl->ConvertCurUnitData2TgtUnit(0, D_UNITSYS_LENGTH_INDEX_MM, D_UNITSYS_BASE_LENGTH, pData->dSubRebarSpace);
		strEndSpace.Format(_T("%g"), dSpace); 
		dSpace = m_pUnitCtrl->ConvertCurUnitData2TgtUnit(0, D_UNITSYS_LENGTH_INDEX_MM, D_UNITSYS_BASE_LENGTH, pData->dSubRebarSpace2);
		strCenSpace.Format(_T("%g"), dSpace); 
	}
	else if(UnitIndex.nBase_Length == D_UNITSYS_LENGTH_INDEX_IN || UnitIndex.nBase_Length == D_UNITSYS_LENGTH_INDEX_FT)
	{
		double dSpace = m_pUnitCtrl->ConvertCurUnitData2TgtUnit(0, D_UNITSYS_LENGTH_INDEX_IN, D_UNITSYS_BASE_LENGTH, pData->dSubRebarSpace);
		strEndSpace.Format(_T("%g"), dSpace);
		dSpace = m_pUnitCtrl->ConvertCurUnitData2TgtUnit(0, D_UNITSYS_LENGTH_INDEX_IN, D_UNITSYS_BASE_LENGTH, pData->dSubRebarSpace2);
		strCenSpace.Format(_T("%g"), dSpace); 
	}
	else   {  ASSERT(0);		return ;	}
	SetStyleRange(CGXRange(ROWINDEX_END_Y,COLINDEX_SPACE), CGXStyle().SetValue(strEndSpace));
	SetStyleRange(CGXRange(ROWINDEX_CENTER_Y,COLINDEX_SPACE), CGXStyle().SetValue(strCenSpace));

	m_dSpace[0] = pData->dSubRebarSpace;
	m_dSpace[1] = pData->dSubRebarSpace2;

	ROWCOL ncRow, ncCol;
	if(m_strShape!=_LSX(SB) && GetCurrentCell(ncRow, ncCol) && ncRow==ROWINDEX_CORNER && ncCol==COLINDEX_NUMBER)
	{
		// 원형기둥의 경우
		// 사각형 기둥의 배근 정보 편집하다가 focus가 코너철근 사용 유무 check box에 있는 상태에서
		// 원형기둥으로 단면을 변경한 경우
		// Grid는 disable 되는데 check box는 계속 동작해서
		// 다음과 같이 강제로 grid의 focus를 사용하지 않는 cell로 이동시켜줌.
		SetCurrentCell(ROWINDEX_HEADER, COLINDEX_HEADER);
	}
}

void CDgnConColmRebarGrid::Grid2Data(T_REBC_D* pData)
{
	pData->nQrb = _ttoi(GetEditingValue(ROWINDEX_NUMBER, COLINDEX_NUMBER));
	pData->nRow    = _ttoi(GetEditingValue(ROWINDEX_ROWS   , COLINDEX_NUMBER));
	pData->MainRebarName        = GetEditingValue(ROWINDEX_NUMBER   , COLINDEX_NAME);
	if(m_strShape==_LSX(SB))
	{
		ROWCOL ncRow, ncCol;
		if(GetCurrentCell(ncRow, ncCol))
		{
			if(ncRow == ROWINDEX_CORNER && ncCol == COLINDEX_NUMBER)
				pData->bUseCornerRebar = !_ttoi(GetEditingValue(ROWINDEX_CORNER, COLINDEX_NUMBER));
			else
				pData->bUseCornerRebar = _ttoi(GetEditingValue(ROWINDEX_CORNER, COLINDEX_NUMBER));      
		}
		else
		{
			pData->bUseCornerRebar = !_ttoi(GetEditingValue(ROWINDEX_CORNER, COLINDEX_NUMBER));
		}
		pData->CornerRebarName = GetEditingValue(ROWINDEX_CORNER, COLINDEX_NAME);
	}
	else  
	{
		pData->bUseCornerRebar = FALSE;
		pData->CornerRebarName = pData->MainRebarName;
	}

	pData->iSubRebarNum[0]        = _ttoi(GetEditingValue(ROWINDEX_END_Y   , COLINDEX_NUMBER));
	pData->iSubRebarNum[1]      = _ttoi(GetEditingValue(ROWINDEX_END_Z   , COLINDEX_NUMBER));
	pData->SubRebarName        = GetEditingValue(ROWINDEX_END_Y   , COLINDEX_NAME);

	pData->iSubRebarNum2[0]      = _ttoi(GetEditingValue(ROWINDEX_CENTER_Y   , COLINDEX_NUMBER));
	pData->iSubRebarNum2[1]   = _ttoi(GetEditingValue(ROWINDEX_CENTER_Z, COLINDEX_NUMBER));
	pData->SubRebarName2   = GetEditingValue(ROWINDEX_CENTER_Y, COLINDEX_NAME);

	pData->dSubRebarSpace = m_dSpace[0];
	pData->dSubRebarSpace2 = m_dSpace[1];
	if(m_bSameRebarEndNMid) pData->dSubRebarSpace2 = m_dSpace[0];
}

CString CDgnConColmRebarGrid::Get_ListRebar()
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

CString CDgnConColmRebarGrid::Get_ListSpaceSubBar()
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

void CDgnConColmRebarGrid::SetHeaderTitle()
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

	SetCoveredCellsRowCol(ROWINDEX_HEADER2, COLINDEX_HD_MAIN  ,  ROWINDEX_HEADER2, COLINDEX_HD_YZ);
	style.SetValue(_LS(IDS_DGN_REBAR));
	SetStyleRange(CGXRange(ROWINDEX_HEADER2, COLINDEX_HD_MAIN), style);

	SetCoveredCellsRowCol(ROWINDEX_HEADER2, COLINDEX_NUMBER ,  ROWINDEX_HEADER2, COLINDEX_SPACE );
	style.SetValue(_LS(IDS_DGN_DATA));
	SetStyleRange(CGXRange(ROWINDEX_HEADER2, COLINDEX_NUMBER), style);
	
	SetCoveredCellsRowCol(ROWINDEX_NUMBER, COLINDEX_HD_MAIN ,  ROWINDEX_CORNER, COLINDEX_HD_MAIN);
	style.SetValue(_LS(IDS_DGN_MAIN));
	SetStyleRange(CGXRange(ROWINDEX_NUMBER, COLINDEX_HD_MAIN), style);

	SetCoveredCellsRowCol(ROWINDEX_NUMBER, COLINDEX_HD_END  ,  ROWINDEX_NUMBER, COLINDEX_HD_YZ );
	style.SetValue(_LS(IDS_DGN_NUMBERS));
	SetStyleRange(CGXRange(ROWINDEX_NUMBER, COLINDEX_HD_END), style);

	SetCoveredCellsRowCol(ROWINDEX_ROWS, COLINDEX_HD_END  ,  ROWINDEX_ROWS, COLINDEX_HD_YZ );
	style.SetValue(_LS(IDS_DGN_ROWS));
	SetStyleRange(CGXRange(ROWINDEX_ROWS, COLINDEX_HD_END), style);

	// Add/Modify by Seungjun PMS:4496  '20120613 RC 보/기둥 철근종류 여러 개 지정 (코너철근 등, 설계없이 검토만) 사각형 단면일 경우만.
	SetCoveredCellsRowCol(ROWINDEX_CORNER, COLINDEX_HD_END  ,  ROWINDEX_CORNER, COLINDEX_HD_YZ);
	style.SetValue(_LS(IDS_DGN_CORNER));
	SetStyleRange(CGXRange(ROWINDEX_CORNER, COLINDEX_HD_END), style);

	SetCoveredCellsRowCol(ROWINDEX_END_Y, COLINDEX_HD_MAIN  ,  ROWINDEX_CENTER_Z, COLINDEX_HD_MAIN );
	style.SetValue(_LS(IDS_DGN_TIES_SPIRALS));
	SetStyleRange(CGXRange(ROWINDEX_END_Y, COLINDEX_HD_MAIN), style);

	SetCoveredCellsRowCol(ROWINDEX_END_Y, COLINDEX_HD_END  ,  ROWINDEX_END_Z, COLINDEX_HD_END );
	style.SetValue(_LS(IDS_DGN_END_I_AND_J));
	SetStyleRange(CGXRange(ROWINDEX_END_Y, COLINDEX_HD_END), style);

	SetCoveredCellsRowCol(ROWINDEX_CENTER_Y, COLINDEX_HD_END  ,  ROWINDEX_CENTER_Z, COLINDEX_HD_END );
	style.SetValue(_LS(IDS_DGN_CENTER_M));
	SetStyleRange(CGXRange(ROWINDEX_CENTER_Y, COLINDEX_HD_END), style);

	style.SetValue(_LSX(y));
	SetStyleRange(CGXRange(ROWINDEX_END_Y, COLINDEX_HD_YZ), style);

	style.SetValue(_LSX(z));
	SetStyleRange(CGXRange(ROWINDEX_END_Z, COLINDEX_HD_YZ), style);

	style.SetValue(_LSX(y));
	SetStyleRange(CGXRange(ROWINDEX_CENTER_Y, COLINDEX_HD_YZ), style);

	style.SetValue(_LSX(z));
	SetStyleRange(CGXRange(ROWINDEX_CENTER_Z, COLINDEX_HD_YZ), style);
}

CString CDgnConColmRebarGrid::GetEditingValue(ROWCOL nRow, ROWCOL nCol)
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

void CDgnConColmRebarGrid::OnModifyCell(ROWCOL nRow, ROWCOL nCol)
{
	CGXGridWnd::OnModifyCell(nRow, nCol);

	/// [GEN-11023] 원형기둥의 주철근 배근 최소값 수정 : 6 -> 4.
	const int nMinQrb = 4;
	const int nMinRow = 2;
	
	if(nRow == ROWINDEX_NUMBER && nCol == COLINDEX_NUMBER)
	{
		int nValue = _ttoi(GetEditingValue(nRow, nCol));
		if(nValue < nMinQrb)   SetStyleRange(CGXRange(nRow, nCol), CGXStyle().SetValue((UINT)nMinQrb));
	} 
	else if(nRow == ROWINDEX_ROWS && nCol == COLINDEX_NUMBER)
	{
		int nNumber = _ttoi(GetEditingValue(ROWINDEX_NUMBER, COLINDEX_NUMBER));
		int nMaxRow = nNumber/2;
		
		int nValue = _ttoi(GetEditingValue(nRow, nCol));
		if(nValue < nMinRow)   SetStyleRange(CGXRange(nRow, nCol), CGXStyle().SetValue((UINT)nMinRow));
		else if(nValue > nMaxRow)   SetStyleRange(CGXRange(nRow, nCol), CGXStyle().SetValue((UINT)nMaxRow));
	}
	// Add/Modify by Seungjun PMS:4496  '20120613 RC 보/기둥 철근종류 여러 개 지정 (코너철근 등, 설계없이 검토만) 사각형 단면일 경우만.
	else if(nRow == ROWINDEX_NUMBER && nCol == COLINDEX_NAME)
	{
		if(m_strShape==_LSX(SB) && !_ttoi(GetEditingValue(ROWINDEX_CORNER, COLINDEX_NUMBER)))
		{
			CString sMainRebarName = GetEditingValue(ROWINDEX_NUMBER, COLINDEX_NAME);
			SetStyleRange(CGXRange(ROWINDEX_CORNER,COLINDEX_NAME), CGXStyle().SetValue(sMainRebarName));
			EnableGrid(ROWINDEX_CORNER,COLINDEX_NAME, FALSE);
		}
	}
	else if(nCol == COLINDEX_NUMBER)
	{
		int nValue = _ttoi(GetEditingValue(nRow, nCol));
		if(nValue < 0)   SetStyleRange(CGXRange(nRow, nCol), CGXStyle().SetValue((UINT)0));
	}

	T_REBC_D RebcD;
	Grid2Data(&RebcD);
	m_pParent->DrawSectionWnd(&RebcD);
}

// Add/Modify by Seungjun PMS:4496  '20120613 RC 보/기둥 철근종류 여러 개 지정 (코너철근 등, 설계없이 검토만) 사각형 단면일 경우만.
BOOL CDgnConColmRebarGrid::OnStartEditing(ROWCOL nRow, ROWCOL nCol)
{  
	BOOL bRet = CGXGridWnd::OnStartEditing(nRow, nCol);

	BOOL bCorner=FALSE;
	if(nRow == ROWINDEX_CORNER && nCol == COLINDEX_NUMBER) // check box.
	{
		if(m_strShape==_LSX(SB))
		{
			if(!_ttoi(GetEditingValue(ROWINDEX_CORNER, COLINDEX_NUMBER)))
			{
				CString sMainRebarName = GetEditingValue(ROWINDEX_NUMBER, COLINDEX_NAME);
				SetStyleRange(CGXRange(ROWINDEX_CORNER,COLINDEX_NAME), CGXStyle().SetValue(sMainRebarName));
				EnableGrid(ROWINDEX_CORNER,COLINDEX_NAME, FALSE);
			}
			else
			{
				bCorner = TRUE;
				EnableGrid(ROWINDEX_CORNER,COLINDEX_NAME);
			}
		}
	}

	T_REBC_D RebcD;
	Grid2Data(&RebcD);
	RebcD.bUseCornerRebar = bCorner;
	m_pParent->DrawSectionWnd(&RebcD);

	return bRet;
}

BOOL CDgnConColmRebarGrid::OnEndEditing(ROWCOL nRow, ROWCOL nCol)
{  
	BOOL bRet = CGXGridWnd::OnEndEditing(nRow, nCol);

	if(nRow == ROWINDEX_NUMBER && nCol == COLINDEX_NUMBER)
	{
		int nValue = _ttoi(GetEditingValue(nRow, nCol));
		if(nValue%2)    SetStyleRange(CGXRange(nRow, nCol), CGXStyle().SetValue((UINT)(nValue-1)));
	} 
	
	if(nRow != ROWINDEX_END_Y && nRow != ROWINDEX_CENTER_Y)   return bRet;
	if(nCol != COLINDEX_SPACE)  return bRet;
	
	int nIndex = 0;
	if(nRow == ROWINDEX_CENTER_Y)   nIndex = 1;

	CString strSpace   = GetEditingValue(nRow, nCol);
	T_UNIT_INDEX UnitIndex;
	m_pUnitCtrl->GetUnitIndexCurrent(UnitIndex);
	if(nRow == ROWINDEX_END_Y && nCol == COLINDEX_SPACE && m_bSameRebarEndNMid)
	{
		SetStyleRange(CGXRange(ROWINDEX_CENTER_Y,COLINDEX_SPACE), CGXStyle().SetValue(strSpace));
		EnableGrid(ROWINDEX_CENTER_Y,COLINDEX_SPACE, FALSE);
	}

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
	if(m_bSameRebarEndNMid) m_dSpace[1] = m_dSpace[0];

	return bRet;
}

BOOL CDgnConColmRebarGrid::OnLButtonHitRowCol(ROWCOL nHitRow, ROWCOL nHitCol, ROWCOL nDragRow, ROWCOL nDragCol, CPoint point, UINT flags, WORD nHitState)
{
	if(nHitRow == ROWINDEX_NUMBER && nHitCol == COLINDEX_NUMBER)
		CGXGridCore::OnLButtonHitRowCol(nHitRow, nHitCol, nDragRow, nDragCol, point, flags, nHitState);   // 2씩 증가/감소 시키기 위해
	return CGXGridCore::OnLButtonHitRowCol(nHitRow, nHitCol, nDragRow, nDragCol, point, flags, nHitState);
}

void CDgnConColmRebarGrid::OnCanceledEditing(ROWCOL nRow, ROWCOL nCol)
{
	CGXGridWnd::OnCanceledEditing(nRow, nCol);

	if(nRow == ROWINDEX_ROWS && nCol == COLINDEX_NUMBER)
	{
		int nValue = _ttoi(GetEditingValue(nRow, nCol));
		if(nValue < 2)   SetStyleRange(CGXRange(nRow, nCol+3), CGXStyle().SetValue((UINT)2));
	}

	T_REBC_D RebcD;
	Grid2Data(&RebcD);
	m_pParent->DrawSectionWnd(&RebcD);
}

BOOL CDgnConColmRebarGrid::Copy( )
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

BOOL CDgnConColmRebarGrid::Paste()
{
	BOOL bReturn = CGXGridWnd::Paste();

	return bReturn;
}

BOOL CDgnConColmRebarGrid::OnPasteFromClipboard(const CGXRange &range)
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

BOOL CDgnConColmRebarGrid::ProcessKeys(CWnd *pSender, UINT nMessage, UINT nChar, UINT nRepCnt, UINT flags)
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

BOOL CDgnConColmRebarGrid::GetStyleRowCol(ROWCOL nRow, ROWCOL nCol, CGXStyle& style, GXModifyType mt, int nType)
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
	if (nRow == ROWINDEX_HEADER2 || nCol < COLINDEX_NUMBER) return TRUE;

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

void CDgnConColmRebarGrid::UpdateUnit()
{
	BOOL bOldLock = LockUpdate(TRUE);
	
	CString strListSpace = Get_ListSpaceSubBar();
	CString strSpace;
	
	T_UNIT_INDEX UnitIndex;
	m_pUnitCtrl->GetUnitIndexCurrent(UnitIndex);
	
	for (int k=0; k<2; k++)
	{
		m_dSpace[k] = m_pUnitCtrl->ConvertUnitDataPrevious(D_UNITSYS_BASE_LENGTH, m_dSpace[k]);

		SetStyleRange(CGXRange().SetCells(ROWINDEX_END_Y+k*2, COLINDEX_SPACE), CGXStyle().SetChoiceList(strListSpace));  
		strSpace = GetEditingValue(ROWINDEX_END_Y+k*2, COLINDEX_SPACE);
		
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
		
		SetValueRange(CGXRange(ROWINDEX_END_Y+k*2, COLINDEX_SPACE), strSpace);
	}
	
	LockUpdate(bOldLock);
	if (!bOldLock) Redraw();
}