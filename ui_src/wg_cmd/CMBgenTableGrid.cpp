// CMBgenTableGrid.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMBgenTableGrid.h"

#include "..\wg_base\wg_base_CompFunc.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_UnitCtrl.h"

#include "..\wg_common\wg_common_TBBrowserWndEx.h"  // for ID_USER_ATTR_COL_ID

#include "CMBgenTableDlg.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCMBgenTableGrid

CCMBgenTableGrid::CCMBgenTableGrid()
{
	m_pDoc = 0;

	m_aTitle.Add(_LS(IDS_WG_CMD__ADDD__Distance)); m_aUnit.Add(D_UNITSYS_BASE_LENGTH); m_aDefVal.Add(_T("0.0"));
	m_aTitle.Add(_LS(IDS_WG_CMD__ADDD__Material)); m_aUnit.Add(0);                     m_aDefVal.Add(_T("0"));
	m_aTitle.Add(_LS(IDS_WG_CMD__ADDD__Column));   m_aUnit.Add(0);                     m_aDefVal.Add(_T("0"));
	m_aTitle.Add(_LS(IDS_WG_CMD__ADDD__Beam));     m_aUnit.Add(0);                     m_aDefVal.Add(_T("0"));
	m_aTitle.Add(_LS(IDS_WG_CMD__ADDD__Brace));    m_aUnit.Add(0);                     m_aDefVal.Add(_T("0"));
	m_aTitle.Add(_LS(IDS_WG_CMD__ADDD__Wall));     m_aUnit.Add(0);                     m_aDefVal.Add(_T("0"));
}

CCMBgenTableGrid::~CCMBgenTableGrid()
{
}

#define CWnd CTBBrowserWnd
BEGIN_MESSAGE_MAP(CCMBgenTableGrid, CWnd)
#undef CWnd
	//{{AFX_MSG_MAP(CCMBgenTableGrid)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CCMBgenTableGrid message handlers
void CCMBgenTableGrid::Initialize(CDBDoc* pDoc)
{
	CTBBrowserWnd::Initialize();
	m_pDoc = pDoc;

	LockUpdate(TRUE);

	// 컬럼 갯수 설정
	SetColCount(m_aTitle.GetSize());
	SetHeaderTitle();

	// 표준 스타일 변경
	ChangeStandardStyle(CGXStyle()
			.SetFont(CGXFont_GC().SetSize(10))
			.SetVerticalAlignment(DT_BOTTOM)
			.SetAllowEnter(FALSE));

	ChangeRowHeaderStyle(CGXStyle()
					.SetFont(CGXFont_GC().SetBold(FALSE))
					.SetHorizontalAlignment(DT_RIGHT)
					.SetControl(GX_IDS_CTRL_HEADER));
	GetParam()->SetNumberedRowHeaders(TRUE);

	// 컬럼 스타일 변경

	SetStyleRange(CGXRange().SetCols(1), CGXStyle()
			.SetHorizontalAlignment(DT_RIGHT)
			.SetValueType(GX_VT_NUMERIC)
			.SetFormat(GX_FMT_FIXED)
			.SetPlaces(4));
	SetStyleRange(CGXRange().SetCols(2, 6), CGXStyle()
			.SetHorizontalAlignment(DT_RIGHT)
			.SetValueType(GX_VT_NUMERIC)
			.SetFormat(GX_FMT_FIXED)
			.SetPlaces(0));

	// 현재 셀의 border 설정 변경
	CGXProperties* pProp = GetParam()->GetProperties();
			pProp->SetUserProperty(GX_IDS_OUTLINECURRENTCELL,
		      (CGXStyle) pProp->sInvertNoBorder);

	// 현재 셀 설정
	SetRightValidCell(1, 0);
	GetParam()->SetHideCurrentCell(GX_HIDE_NEVER);

	// always show vertical scrollbar
	SetScrollBarMode(SB_VERT, gxnEnabled | gxnEnhanced);  

	SetRowHeightByDPI(0, 0, 36);

	// column size 설정
	SetColWidthByDPI(0, 0, 35);
	SetColWidthByDPI(1, 1, 100);
	SetColWidthByDPI(2, 6, 70);

	// column의 이동을 금지시킨다.
	GetParam()->EnableMoveCols(FALSE);

	// 데이타 설정
	//MakeItemEx();
	LockUpdate(FALSE);
}

// Column별 헤더 이름을 설정한다.
void CCMBgenTableGrid::SetHeaderTitle()
{
	CString title, csUnit;
	int i;
	ROWCOL nCol;

	for(i = 0; i < GetColCount(); i++)
	{
		nCol = i + 1;

		CUnitCtrl::GetUnitSystem(m_aUnit[i], csUnit);
		if (!csUnit.IsEmpty()) title = m_aTitle[i] + _T("\n(")+csUnit+_T(")");
		else title = m_aTitle[i];

		// 컬럼 헤더 이름 및 ID를 설정
		SetStyleRange(CGXRange(0, nCol), CGXStyle()
			.SetValue(title)
			.SetUserAttribute(ID_USER_ATTR_COL_ID, m_aTitle[i]));
	}
}

BOOL CCMBgenTableGrid::OnValidateField(ROWCOL nRow, ROWCOL nCol)
{
	// Factor가 실수(정수포함)로 입력되어 있는지 검사한다.
	CString value;
	int nErrCode;
	value = GetValueRowCol(nRow, nCol);
	value.TrimLeft();
	value.TrimRight();
	if (nCol == 1)
	{
		if (!IsValidDoubleNumber(value, nErrCode))
		{
			switch(nErrCode)
			{
			case 1: SetWarningText(_LS(IDS_WG_CMD__ADDD__Invalid_Formula_)); break;
			case 2: SetWarningText(_LS(IDS_WG_CMD__ADDD__Input_Number_)); break;
			case 3: SetWarningText(_LS(IDS_WG_CMD__ADDD__Out_of_range_)); break;
			}
			return FALSE;
		}
	}
	else if (nCol >= 2 && nCol <= 6)
	{
		if (!IsValidLongNumber(value, nErrCode))
		{
			switch(nErrCode)
			{
			case 1: SetWarningText(_LS(IDS_WG_CMD__ADDD__Invalid_formula_)); break;
			case 2: case 4: SetWarningText(_LS(IDS_WG_CMD__ADDD__Input_integer_value_)); break;
			case 3: SetWarningText(_LS(IDS_WG_CMD__ADDD__Value_is_too_big_)); break;
			}
			return FALSE;
		}
	}
	return TRUE;
}

BOOL CCMBgenTableGrid::OnInsertEmptyRecord(ROWCOL nRow)
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

	BOOL bOldLock = LockUpdate(TRUE);
	int nColCount = GetColCount();
	for (int i = 0; i < nColCount; i++)
	{
		nCol = i+1;
		value = m_aDefVal[i];
		if (!value.IsEmpty() && (nCol != ncCol || !bActive)) 
			SetExpressionRowCol(nRow, nCol, value);
	}
	LockUpdate(bOldLock);
	if (!bOldLock) Redraw();

	return TRUE;
}

void CCMBgenTableGrid::MakeItemEx(CArray<T_MAKEBUILDING, T_MAKEBUILDING&> &arMakeBuilding)
{
	T_MAKEBUILDING tempBuilding;

	BOOL bOldLock = LockUpdate(TRUE);

//	SetRecordCount(arMakeBuilding.GetSize());
	SetRowCount(arMakeBuilding.GetSize()+1,GX_UPDATENOW); 

	for(int i=0; i<arMakeBuilding.GetSize(); i++)
	{	
		tempBuilding = arMakeBuilding.GetAt(i);
		SetValueRange(CGXRange(i+1,1), tempBuilding.dDistance);
		SetValueRange(CGXRange(i+1,2), (long)tempBuilding.nIncMatl);
		SetValueRange(CGXRange(i+1,3), (long)tempBuilding.nIncColumn);
		SetValueRange(CGXRange(i+1,4), (long)tempBuilding.nIncBeam);
		SetValueRange(CGXRange(i+1,5), (long)tempBuilding.nIncBrace);
		SetValueRange(CGXRange(i+1,6), (long)tempBuilding.nIncWall);   
	}
	LockUpdate(bOldLock);
	if(!bOldLock) Redraw(); 
}

int CCMBgenTableGrid::GetCount()
{
	return GetRowCount();
}
