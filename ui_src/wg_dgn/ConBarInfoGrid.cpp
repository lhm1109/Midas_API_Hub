#include "stdafx.h"
#include "ConBarInfoGrid.h"

#include "wg_dgn.h"
#include "DgnConBarInfoDlg.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_DBLib.h"
#include "..\wg_db\wg_db_AttrCtrl2.h"

#include "..\wg_common\wg_common_TBGrid.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

#define CONTROL_WIDTH 257.0

#define MAX_COL 6
#define ROW_HEADER_COUNT 1

#define NUMFORMAT _T("%.4f")

#define  MAX_SEL_COUNT 5

enum ColumnIndex
{
	COLINDEX_NO			= 0,
	COLINDEX_CHECK		= 1,
	COLINDEX_NAME		= 2,
	COLINDEX_DIA		= 3,
	COLINDEX_AREA		= 4,
	COLINDEX_DIA_OUT	= 5,
	COLINDEX_WEIGHT		= 6,
};

int arColumnWidth[] = { 10, 30, 30, 40, 40, 40, 50 };

CConBarInfoGrid::CConBarInfoGrid(CMatlDB* pMatlDB)
{
	m_nWndWidth = 0;
	m_nWndHeight = 0;

	m_pMatlDB = pMatlDB;
}

CConBarInfoGrid::~CConBarInfoGrid()
{
}

void CConBarInfoGrid::Initialize(CWnd* pParent)
{
	m_pParent = (CDgnConBarInfoDlg*)pParent;

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
	SetRowHeightByDPI(0, 0, 40);

	GetParam()->EnableUndo(FALSE);

	// Row와  Column의 이동을 금지시킨다.
	GetParam()->EnableTrackRowHeight(FALSE);
	GetParam()->EnableTrackColWidth(FALSE);
	GetParam()->EnableMoveRows(FALSE);
	GetParam()->EnableMoveCols(FALSE);

	// double click시 셀 편집이 되도록 한다.
	//GetParam()->SetActivateCellFlags(GX_CAFOCUS_DBLCLICKONCELL);

	// MemoryDC를 사용한다.
	//SetDrawingTechnique(gxDrawUsingMemDC);

	// 헤더의 Current cell을 막는다.
	ColHeaderStyle().SetEnabled(FALSE);
	RowHeaderStyle().SetEnabled(FALSE);

	CGXProperties* pProp = GetParam()->GetProperties();

	// Turn off pressed button effect for column headers
	pProp->SetMarkColHeader(FALSE);
	pProp->SetMarkRowHeader(FALSE);

	// 현재 셀의 border 설정 변경
	pProp->SetUserProperty(GX_IDS_OUTLINECURRENTCELL, (CGXStyle)pProp->sInvertNoBorder);

	// Grid Number
	SetColCount(MAX_COL); // Number of column excluding header

	SetStyleRange(CGXRange().SetCols(COLINDEX_CHECK), CGXStyle()
		.SetControl(GX_IDS_CTRL_CHECKBOX3D)
		.SetValueType(GX_VT_NUMERIC)
		.SetValue(_T("0")));

	// chk열만 수정할 수 있도록
	SetReadOnly(TRUE);

	// Column Width
	SetColumnWidth();

	// Set Title
	SetHeaderTitle();

#ifdef _CIVIL	
	HideCols(COLINDEX_DIA_OUT, COLINDEX_DIA_OUT);
#endif

	SetScrollBarMode(SB_VERT, gxnEnabled | gxnEnhanced);
	if (m_pImousePlugin == NULL) EnableMouseWheel();

	LockUpdate(FALSE);

	Redraw();

	LockUpdate(FALSE);
}

void CConBarInfoGrid::SetHeaderTitle()
{
	CDBDoc *pDoc = CDBDoc::GetDocPoint();
	T_SEME_D SemeD;
	pDoc->m_pAttrCtrl2->GetSeme(SemeD);

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

	CStringArray aTitle;
	auto lambda_addTitleUnit = [&aTitle](CString strTitle, int nUnit)
	{
		if(nUnit == D_UNITSYS_NONE)
		{
			aTitle.Add(strTitle);
			return;
		}

		CString strUnit;
		CUnitCtrl::GetUnitSystem(nUnit, strUnit);
		strTitle = strTitle + _T("\n(") + strUnit + _T(")");
		aTitle.Add(strTitle);
	};

	lambda_addTitleUnit(_LS(IDS_DGN_CON_BAR_INFO_GRID_CHECK)	, D_UNITSYS_NONE);
	lambda_addTitleUnit(_LS(IDS_DGN_CON_BAR_INFO_GRID_NAME)		, D_UNITSYS_NONE);
	lambda_addTitleUnit(_LS(IDS_DGN_CON_BAR_INFO_GRID_DIA)		, D_UNITSYS_BASE_LENGTH);
	lambda_addTitleUnit(_LS(IDS_DGN_CON_BAR_INFO_GRID_AREA)		, D_UNITSYS_BASE_AREA);
	lambda_addTitleUnit(_LS(IDS_DGN_CON_BAR_INFO_GRID_DIA_OUT)	, D_UNITSYS_BASE_LENGTH);
	lambda_addTitleUnit(_LS(IDS_DGN_CON_BAR_INFO_GRID_WEIGHT)	, D_UNITSYS_BASE_UNITFORCE);

	for (int i = COLINDEX_CHECK; i <= COLINDEX_WEIGHT; i++)
	{
		int nIndex = i - 1;
		style.SetValue(aTitle[nIndex]);
		SetStyleRange(CGXRange(0, i), style);
	}
}

void CConBarInfoGrid::SetColumnWidth()
{
	double dRatio = (double)m_nWndWidth / CONTROL_WIDTH;

	for (int i = COLINDEX_NO; i <= COLINDEX_WEIGHT; i++)
	{
		SetColWidth(i, i, arColumnWidth[i] * dRatio);
	}
}

void CConBarInfoGrid::SetReadOnly(BOOL bSetReadOnly)
{
	SetStyleRange(CGXRange().SetCols(COLINDEX_NAME, COLINDEX_WEIGHT), CGXStyle().SetReadOnly(bSetReadOnly));
}

void CConBarInfoGrid::SetValue(ROWCOL row, const CString& strBarName, const CArray<CString, CString>& arBarSize)
{
	for (int i = COLINDEX_NO; i <= COLINDEX_WEIGHT; i++)
	{
		CString strVal;
		ConvDataToStr(i, strBarName, arBarSize, strVal);
		SetValueRange(CGXRange(row, i), strVal);
	}
}

void CConBarInfoGrid::ConvDataToStr(int i, const CString& strBarName, const CArray<CString, CString>& arBarSize, CString& value)
{
	_DGN_REBAR_DATA data = m_pMatlDB->Get_RebarData(strBarName);

	if (i == COLINDEX_NO) value = _T("");
	else if (i == COLINDEX_CHECK) value = SelectionCheck(strBarName, arBarSize) ? _T("1") : _T("0");
	else if (i == COLINDEX_NAME) value = strBarName;
	else if (i == COLINDEX_DIA) value.Format(NUMFORMAT, data.dDia);
	else if (i == COLINDEX_AREA) value.Format(NUMFORMAT, data.dArea);
	else if (i == COLINDEX_DIA_OUT) value.Format(NUMFORMAT, data.dOutDia);
	else if (i == COLINDEX_WEIGHT) value.Format(NUMFORMAT, data.dWeight);
}

BOOL CConBarInfoGrid::SelectionCheck(const CString& strBarName, const CArray<CString, CString>& arBarSize)
{
	BOOL bCheck = FALSE;
	int nSize = arBarSize.GetSize();

	for (int i = 0; i < nSize; i++)
	{
		if (arBarSize.GetAt(i) == strBarName)
		{
			bCheck = TRUE;
			break;
		}
	}

	return bCheck;
}

BOOL CConBarInfoGrid::Cutting_RebarSizeString(CString str, CArray<CString, CString>& arBarSize)
{
	BOOL bCheck = TRUE;

	TCHAR ch;
	CString temp = _T("");
	int iLength = str.GetLength();
	for (int i = 0; i < iLength; i++)
	{
		ch = str.GetAt(i);
		if (ch == ',' || i + 1 == iLength)
		{
			if (ch == ',') arBarSize.Add(temp);
			else			  arBarSize.Add(temp + ch);
			temp = _T("");
		}
		else	temp = temp + ch;
	}
	return bCheck;
}

void CConBarInfoGrid::Data2Grid(const CStringArray& rebarNameList, const CString& strTotalRebarSize)
{
	int nRebarSize = rebarNameList.GetSize();
	SetRowCount(nRebarSize);

	SetReadOnly(FALSE);

	CArray<CString, CString> ReBarSize;
	BOOL bCheck = Cutting_RebarSizeString(strTotalRebarSize, ReBarSize);
	ASSERT(bCheck);

	for (int i = 0; i < nRebarSize; i++)
	{
		int nIndex = i + ROW_HEADER_COUNT;
		SetValue(nIndex, rebarNameList[i], ReBarSize);
	}
	SetReadOnly(TRUE);
}


void CConBarInfoGrid::Grid2Data(CString& strTotalRebarSize)
{
	CString strSize = _T("");
	int nRowCount = GetRowCount();
	int nSelectCount = 0;
	for (int i = 0; i < nRowCount; i++)
	{
		CString strChk = GetValueRowCol(i + 1, COLINDEX_CHECK);
		
		if (strChk == _T("1"))
		{
			CString strName = GetValueRowCol(i + 1, COLINDEX_NAME);
			strSize = strSize + strName + _T(",");

			nSelectCount++;
			if(nSelectCount > MAX_SEL_COUNT)
			{
				strTotalRebarSize = _T("Over");
				return;
			}
		}
	}

	int iLen = strSize.GetLength();
	strTotalRebarSize = _T("");
	if (iLen > 0)	strTotalRebarSize = strSize.Left(iLen - 1);
	else			strTotalRebarSize = _T("");
}

void CConBarInfoGrid::UpdateUnit()
{
	BOOL bOldLock = LockUpdate(TRUE);

	LockUpdate(bOldLock);
	if (!bOldLock) Redraw();
}