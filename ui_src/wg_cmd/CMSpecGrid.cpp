// CMSpecGrid.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMSpecGrid.h"

#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_common\wg_common_TBGrid.h"

/*
#include "..\ADS_db\ADS_db_DBDoc.h"
#include "..\ADS_db\ADS_db_MDBCtrl.h"
#include "..\ADS_db\ADS_db_UnitCtrl.h"
#include "..\ADS_db\ADS_db_ClassStor.h"
#include "..\ADS_db\ADS_db_ClassMdul.h"
*/

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CCMSpecGrid

CCMSpecGrid::CCMSpecGrid()
{
	m_pDoc = CDBDoc::GetDocPoint(); 
	ASSERT(m_pDoc);
	//m_pMDBCtrl = m_pDoc->GetMDBCtrl(); ASSERT(m_pMDBCtrl);
	//m_bOnStartEditing = FALSE;
}

CCMSpecGrid::~CCMSpecGrid()
{
}


/////////////////////////////////////////////////////////////////////////////
// User Defined Function

void CCMSpecGrid::SetHeaderTitle()
{  
	int nColNum = GetColNumber();

	int aWidth[] = {0, 30, 80, 80};
	for(int i = 0; i <= nColNum; i++)
		SetColWidthByDPI(i, i, aWidth[i]);

	SetValueRange(CGXRange(0, 1), _LS(IDS_WG_CMD__ADDD__Story));    
	
	CString csTitle, csUnit;
	m_pDoc->m_pUnitCtrl->GetUnitSystem(D_UNITSYS_BASE_LENGTH, csUnit);
	csTitle = _LS(IDS_CMD_ECCENTRICITY_CROSS);
	csTitle = csTitle + _T(" (") + csUnit + _T(")");
	SetValueRange(CGXRange(0, 2), csTitle); 

	csTitle = _LS(IDS_CMD_ECCENTRICITY_ALONG);
	csTitle = csTitle + _T(" (") + csUnit + _T(")");
	SetValueRange(CGXRange(0, 3), csTitle); 
}

int  CCMSpecGrid::GetColNumber()
{
	return 3;
}

BEGIN_MESSAGE_MAP(CCMSpecGrid, CCMGXGridWnd)
	//{{AFX_MSG_MAP(CCMSpecGrid)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CCMSpecGrid message handlers
void CCMSpecGrid::Initialize()
{
	CCMGXGridWnd::Initialize();
	
	LockUpdate(TRUE);
		
	int nColNum = GetColNumber();
	SetColCount(nColNum);

	// 표준 스타일 변경
	ChangeStandardStyle(CGXStyle()
			.SetFont(CGXFont_GC().SetSize(9))
			.SetVerticalAlignment(DT_BOTTOM)
			.SetAllowEnter(FALSE));

	// Column header의 스타일 변경
	ChangeColHeaderStyle(CGXStyle().SetFont(CGXFont_GC().SetBold(FALSE)));
	SetRowHeightByDPI(0, 0, 20);

	// 컬럼 스타일 변경
	SetStyleRange(CGXRange().SetRows(0), CGXStyle()
			.SetControl(GX_IDS_CTRL_HEADER));

	SetStyleRange(CGXRange().SetCols(0), CGXStyle()
			.SetControl(GX_IDS_CTRL_HEADER));

	SetStyleRange(CGXRange().SetCols(1), CGXStyle()
			.SetHorizontalAlignment(DT_CENTER)
			.SetValueType(GX_VT_STRING)
			.SetEnabled(FALSE)
			);
	
	// Eccentricity의 스타일 변경
	SetStyleRange(CGXRange().SetCols(2, 3), CGXStyle()
		.SetControl(GX_IDS_CTRL_EDIT)
		.SetValueType(GX_VT_NUMERIC)
		.SetHorizontalAlignment(DT_CENTER)
		.SetVerticalAlignment(DT_VCENTER)
		.SetValue(_T(""))
		.SetFormat(GX_FMT_FIXED)
		.SetPlaces(4)
		);

	GetParam()->EnableUndo(FALSE);
	GetParam()->EnableTrackRowHeight(FALSE);  

	// Row와  Column의 이동을 금지시킨다.
	GetParam()->EnableMoveRows(FALSE);
	GetParam()->EnableMoveCols(FALSE);

	// MemoryDC를 사용한다.
	SetDrawingTechnique(gxDrawUsingMemDC);
		
	// 헤더의 Current cell을 막는다.
	ColHeaderStyle().SetEnabled(FALSE);
	RowHeaderStyle().SetEnabled(FALSE);

	CGXProperties* pProp = GetParam()->GetProperties();
	pProp->SetMarkColHeader(FALSE); // Turn off pressed button effect for column headers
	pProp->SetMarkRowHeader(FALSE);
	pProp->SetUserProperty(GX_IDS_OUTLINECURRENTCELL, // 현재 셀의 border 설정 변경
												(CGXStyle) pProp->sInvertNoBorder);
	 
	// always show vertical scrollbar
	SetScrollBarMode(SB_VERT, gxnEnabled | gxnEnhanced);

	SetHeaderTitle();

	LockUpdate(FALSE);
}

/*
void CCMSpecGrid::Data2Grid(CArray<T_SEIS_ECCEN, T_SEIS_ECCEN&>* paSeisEccen)
{   
	T_STOR_D StorData;
	CClassStor StorSet;
	StorSet.ConnectToDB(m_pMDBCtrl->GetCurrentDB());
	
	int nCount = paSeisEccen->GetSize();
	SetRowCount(nCount);

	CStringArray aValue;
	aValue.SetSize(D_COL_NUM);
	CString strValue;

	if(nCount > 0) 
	{
		if(!StorSet.Get(paSeisEccen->GetAt(0).StorK, StorData)) StorData.Initialize();
		m_nCurMdulKey = StorData.MdulK;  
		for(int i = 0; i < nCount; i++)
		{
			if(!StorSet.Get(paSeisEccen->GetAt(i).StorK, StorData)) StorData.Initialize();
			aValue[0] = StorData.strName;
			strValue.Format(_T("%.4g"), paSeisEccen->GetAt(i).dEccentricity);
			aValue[1] = strValue;
			SetDataToRow(i+1, aValue);
		}
	}
}

BOOL CCMSpecGrid::Grid2Data(CArray<T_SEIS_ECCEN, T_SEIS_ECCEN&>* paSeisEccen)
{
	paSeisEccen->RemoveAll();
	int nCount = GetRowCount();
		
	T_STOR_D StorData;
	CClassStor StorSet;
	StorSet.ConnectToDB(m_pMDBCtrl->GetCurrentDB());
		
	CStringArray aValue;
	aValue.SetSize(D_COL_NUM);
	T_SEIS_ECCEN SeisEccen;
	
	for(int i = 0; i < nCount; i++)
	{
		SeisEccen.Initialize();
		if(!GetRowToData(i+1, aValue)) return FALSE;
		StorData.strName = aValue[0];    
		SeisEccen.StorK = StorSet.GetKey(StorData.strName, m_nCurMdulKey);
		SeisEccen.dEccentricity = _tstof(aValue[1]);    
		paSeisEccen->Add(SeisEccen);
	}  
	return TRUE;
}
*/



/*
void CCMSpecGrid::SetDataToRow(ROWCOL nRow, CStringArray& raValue)
{
	int nColCount = GetColCount();
	ASSERT(nColCount == raValue.GetSize());
			
	for(int i = 1; i <= nColCount; i++)
	{
		SetValueRange(CGXRange(nRow, i), raValue[i-1]);        
	}
}

BOOL CCMSpecGrid::GetRowToData(ROWCOL nRow, CStringArray& raValue)
{
	int nColCount = GetColCount();
	ASSERT(nColCount == raValue.GetSize());

	for (int i=1; i <= nColCount; i++)
		raValue[i-1] = GetEditingValue(nRow, i);

	return TRUE;
}
*/

/*
CString CCMSpecGrid::GetEditingValue(ROWCOL nRow, ROWCOL nCol)
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
					if (m_bOnStartEditing)
					{
						if (csNum == _T("0")) csNum = _T("1");
						else csNum = _T("0");
					}
				}
			}
		}
	}
	if (!bGet) csNum = GetValueRowCol(nRow, nCol);
	return csNum;
}
*/
BOOL CCMSpecGrid::GetStyleRowCol(ROWCOL nRow, ROWCOL nCol, CGXStyle& style, GXModifyType mt, int nType)
{
	BOOL bCurrentRow;
	BOOL bCurrentCell;
	BOOL bEnabled;
	BOOL bHeaderCol;
	BOOL bHeaderRow;

	ROWCOL ncRow, ncCol;    // 현재 Row, Col
	CCMGXGridWnd::GetStyleRowCol(nRow, nCol, style, mt, nType);

	// 현재 셀의 그리드 좌표(Row, Col 번호)를 얻어온다.
	if (!GetCurrentCell(ncRow, ncCol)) 
		ncRow = GetRowCount()+1; // 현재 셀이 없는 경우 : ncRow를 범위 밖으로 설정

	// 현재 셀이 이동되면 새로 그려준다.
	m_bRefreshOnSetCurrentCell = TRUE;

	// Row나 Col이 1보다 작으면 리턴한다.(Standard Row or Column Header)
	if (nRow < 1 || nCol < 1) return TRUE;

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

/*
BOOL CCMSpecGrid::OnStartEditing(ROWCOL nRow, ROWCOL nCol)
{
	// 부모의 OnModifyCell을 항상 먼저 호출해 주어야 한다.
	BOOL bResult = CCMGXGridWnd::OnStartEditing(nRow, nCol);
	m_bOnStartEditing = TRUE; 
	m_bOnStartEditing = FALSE;
	return bResult;
}

void CCMSpecGrid::OnCanceledEditing(ROWCOL nRow, ROWCOL nCol)
{
	// 부모의 OnCanceledEditing을 항상 먼저 호출해 주어야 한다.
	CCMGXGridWnd::OnCanceledEditing(nRow, nCol);
	m_bOnStartEditing = FALSE;    
}
*/


void CCMSpecGrid::Data2Grid(CArray<T_SPLC_ECCEN, T_SPLC_ECCEN&>& rSplcEccen, CArray<T_SPLC_ECCEN, T_SPLC_ECCEN&>& rSplcEccen_AL)
{
	int nNum = rSplcEccen.GetSize();
	if (nNum <= 0) return;

	T_STOR_D StorD;
	T_SPLC_ECCEN item;
	T_SPLC_ECCEN item_AL;
	
	SetRowCount(nNum);
	for (int i=0; i<nNum; i++)
	{
		item = rSplcEccen.GetAt(i);
		item_AL = rSplcEccen_AL.GetAt(i);
		if (!m_pDoc->m_pAttrCtrl->GetStor(item.KeyStor, StorD)) continue;
		
		SetValueRange(CGXRange(i+1, 1), StorD.StoryName);
		SetValueRange(CGXRange(i+1, 2), (double)item.dEccentricity);
		SetValueRange(CGXRange(i+1, 3), (double)item_AL.dEccentricity);
	}
}

void CCMSpecGrid::EnableGrid(BOOL bEnable, BOOL bUseGL)
{
	int nRow = GetRowCount();
	if (nRow < 1) return;
	SetStyleRange(CGXRange(1, 2, nRow, 3), CGXStyle().SetEnabled(bEnable));
	
	if(!bUseGL)
	{
		if (bEnable)    // 지하층 Disable
		{
			T_STOR_K StorK;
			T_STOR_D StorD;
			CString csStorName;
			double dGL = m_pDoc->m_pAttrCtrl->GetGroundLevel();
			for (int i=0; i<nRow; i++)
			{
				csStorName = GetValueRowCol(i+1, 1);
				StorK = m_pDoc->m_pAttrCtrl->GetStorKey(csStorName);
				if (!m_pDoc->m_pAttrCtrl->GetStor(StorK, StorD)) continue;
				if (StorD.dStoryLevel <= dGL)
					SetStyleRange(CGXRange(i+1, 2, i+1, 3), CGXStyle().SetEnabled(FALSE));
			}
		}
	}
	m_bEnable = bEnable;
}

BOOL CCMSpecGrid::Grid2Data(CArray<T_SPLC_ECCEN, T_SPLC_ECCEN&>& rSplcEccen, CArray<T_SPLC_ECCEN, T_SPLC_ECCEN&>& rSplcEccen_AL)
{
	CString value;
	int nNum = rSplcEccen.GetSize();
	for (int i=0; i<nNum; i++)
	{
		value = GetValueRowCol(i+1, 2);
		rSplcEccen[i].dEccentricity = _tstof(value);

		value = GetValueRowCol(i+1, 3);
		rSplcEccen_AL[i].dEccentricity = _tstof(value);
	}
	return TRUE;
}

