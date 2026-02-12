// CMSectItemGrid4Stlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMSectItemGrid4Stlg.h"

#include "CMSecViewWnd.h"
#include "CMSectItemTap.h"          
#include "CMSectItemImportStlg.h"
#include "CMSectStiffenerDlg.h"
#include "CMSectSODDeckStiffenerDlg.h"
#include "CMSectRivetAngleDlg.h"

#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\SectUtil.h"
#include "..\wg_db\wg_db_SectDB.h"

#include "..\wg_base\wg_base_MsgDll.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCMSectItemGrid4Stlg
CCMSectItemGrid4Stlg::CCMSectItemGrid4Stlg()
{
	m_bInit = FALSE;
	m_pParent = 0;
	m_pPreview = 0;
	m_pSectD = 0;
	m_bOnStartEditing = FALSE;
	
	m_nSymmetryStlGSectI  = 0;
	m_nSymmetryStlGSectJ  = 0;
	m_bSymmetryStlGSectI = FALSE;
	m_bSymmetryStlGSectJ = FALSE;
}

CCMSectItemGrid4Stlg::~CCMSectItemGrid4Stlg()
{
}

//---------------------------------------------------------------------------
// Public Interface Functions
void CCMSectItemGrid4Stlg::InitGrid(CCMSecViewWnd* pPreview)
{
	ASSERT(pPreview);
	m_pPreview = pPreview;

	Initialize(); // 그리드를 초기화한다.
	GetParam()->EnableUndo(FALSE);  // undo를 금지시킨다.

	// cell의 width와 height 크기 변경을 금지한다.
	GetParam()->EnableTrackRowHeight(FALSE);
	GetParam()->EnableTrackColWidth(FALSE);

	// column과 row의 위치 이동을 금지시킨다.
	GetParam()->EnableMoveCols(FALSE);
	GetParam()->EnableMoveRows(FALSE);

	// double click시 셀 편집이 되도록 한다.
	GetParam()->SetActivateCellFlags(GX_CAFOCUS_DBLCLICKONCELL);

	// Text만 카피 되게 한다.
	//EnableOleDataSource(GX_DNDTEXT);
	m_nClipboardFlags &= ~GX_DNDSTYLES;

	// Row 헤더와 Column 헤더를 숨긴다.
	HideRows(0, 0, TRUE);
	HideCols(0, 0, TRUE);

	// always show vertical scrollbar
	SetScrollBarMode(SB_VERT, gxnEnabled | gxnEnhanced);

	// 표준 스타일 변경
	ChangeStandardStyle(CGXStyle()
			.SetFont(CGXFont_GC().SetSize(10))
			.SetVerticalAlignment(DT_BOTTOM)
			.SetAllowEnter(FALSE));

	// Wheel Mouse 지원
	if (!m_bInit) EnableMouseWheel();

	LockUpdate(FALSE);
	Redraw();

	m_bInit = TRUE;
}

void CCMSectItemGrid4Stlg::SetDataSource(T_SECT_D* pSectD, CCMSectItemTap* pParent)
{
	m_pSectD = pSectD;
	m_pParent = pParent;
	
	ShowData();
	SetRightValidCell(1, 1);
}

void CCMSectItemGrid4Stlg::ChangeShape(int nNewShapeIndex)
{
	CancelEditing();

	m_nShapeIndex = nNewShapeIndex;
	
	SetRightValidCell(1, 1);

	// show/hide row according to shape
	ShowData();
}

void CCMSectItemGrid4Stlg::ShowData() // MNET:XXXX 20070221 changed by mylee
{
	BOOL bOldLock = LockUpdate(TRUE);
	BOOL bOldReadOnlyLock = GetParam()->IsLockReadOnly();
	GetParam()->SetLockReadOnly(FALSE);

	SetColCount(3);
	// set column width
	SetColWidth(1, 1, globalUtils.ScaleByDPI(75));
	SetColWidth(2, 2, globalUtils.ScaleByDPI(100));
	SetColWidth(3, 3, globalUtils.ScaleByDPI(45));

	int nRowCount = GetRowCount();
	if (nRowCount > 0) RemoveRows(1, nRowCount);

	SetViewData(); 

	m_nSizeIRow       = 0;
	m_nSizeJRow       = 0;
	m_nStiffenerRow2  = 0; // new
	m_nRivetRow       = 0;
	m_nSymmetryStlGSectI  = 0;
	m_nSymmetryStlGSectJ  = 0;
	m_bSymmetryStlGSectI = FALSE;
	m_bSymmetryStlGSectJ = FALSE;

	// Unit 및 자리수 설정
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	T_UNIT_INDEX UnitIndex;
	pDoc->m_pUnitCtrl->GetUnitIndexCurrent(UnitIndex);
	int nPlace;
	switch(UnitIndex.nBase_Length)
	{
	case D_UNITSYS_LENGTH_INDEX_MM : nPlace = 2; break;
	case D_UNITSYS_LENGTH_INDEX_M  : nPlace = 4; break;
	default                        : nPlace = 3; break;
	}
	CString csUnit;
	CUnitCtrl::GetUnitSystem(D_UNITSYS_BASE_LENGTH, csUnit);
	
	// Row 추가 START
	int nRow = 0;
	
	//Size
	if (m_nNumSize > 0)
	{
		CDBDoc* pDoc = CDBDoc::GetDocPoint();
		T_UNIT_INDEX UnitIndex;
		pDoc->m_pUnitCtrl->GetUnitIndexCurrent(UnitIndex);
		int nPlace;
		switch(UnitIndex.nBase_Length)
		{
		case D_UNITSYS_LENGTH_INDEX_MM : nPlace = 2; break;
		case D_UNITSYS_LENGTH_INDEX_M  : nPlace = 4; break;
		default                        : nPlace = 3; break;
		}
		CString csUnit;
		CUnitCtrl::GetUnitSystem(D_UNITSYS_BASE_LENGTH, csUnit);

		CStringArray aSizeList;
		GetSizeList(aSizeList);
		CStringArray aUnitList;
		GetUnitList(aUnitList);

		CString aTitle[] = {_LS(IDS_CMD_PSC_GRID_Size_I), _LS(IDS_CMD_PSC_GRID_Size_J)};
		for (int n = 0; n < 2; n++)
		{
			nRow++;
			InsertRows(nRow, 1);  // Title
			SetCoveredCellsRowCol(nRow, 2, nRow, 3);
			SetStyleRange(CGXRange(nRow, 1), CGXStyle()
				.SetInterior(COLORREF(RGB(0, 0, 255)))
				.SetTextColor(COLORREF(RGB(255, 255, 0)))
				.SetValue(aTitle[n]));
			SetStyleRange(CGXRange(nRow, 2), CGXStyle()
				.SetControl(GX_IDS_CTRL_PUSHBTN)
				.SetInterior(GXSYSCOLOR(COLOR_BTNFACE))
				.SetChoiceList(_LS(IDS_CMD_PSC_GRID_Import___))
				.SetTextColor(COLORREF(RGB(0, 0, 0)))
				.SetEnabled(TRUE));

			// size
			double *pSize;
			if (n == 0) 
			{
				pSize = m_pSectD->SectBefore.SectI.Size;
			}
			else 
			{
				pSize = m_pSectD->SectBefore.SectJ.Size;
			}

			// size
			nRow++;
			if (n == 0) m_nSizeIRow = nRow;
			else m_nSizeJRow = nRow;
			InsertRows(nRow, m_nNumSize);

			if(m_nShapeIndex == 0 || m_nShapeIndex == 1)
			{
				// Stl Girder I,Box,Tub는 Size그룹이.. Symetric(CheckBox), Top, Bot, Size로 구성되기 때문에 따로 처리 함
				ShowDataStlGirderBoxI(n, nRow, aSizeList, nPlace, csUnit, pSize);
			}
			else
			{
				for (int i = 0; i < m_nNumSize; i++)
				{
					SetValueRange(CGXRange(nRow+i, 1), aSizeList[i]);
					SetStyleRange(CGXRange(nRow+i, 2), CGXStyle()
						.SetHorizontalAlignment(DT_RIGHT)
						.SetFormat(GX_FMT_FIXED)
						.SetPlaces(nPlace)
						.SetValue(pSize[i])
						.SetEnabled(FALSE));
					SetStyleRange(CGXRange(nRow+i, 3), CGXStyle().SetValue(csUnit));
			}      
			}      
			nRow += m_nNumSize-1;
		}
	}

	// Stiffener
	if(m_nNumStiffener2 > 0)
	{
// 		nRow++;
// 		if(n==0) m_nStiffenerRow_I = nRow;
// 		else     m_nStiffenerRow_J = nRow;
		nRow++;
		InsertRows(nRow, 1);
		SetCoveredCellsRowCol(nRow, 1, nRow, 3);
		SetStyleRange(CGXRange(nRow, 1), CGXStyle()
			.SetInterior(COLORREF(RGB(0, 0, 255)))
			.SetTextColor(COLORREF(RGB(255, 255, 0)))
			.SetValue(_LS(IDS_CMD_COMPO_GRID_Stiffener)));
		
		nRow++;
		m_nStiffenerRow2 = nRow;

		InsertRows(nRow, 1);  // Title
		SetStyleRange(CGXRange(nRow, 1), CGXStyle()
			.SetValue(_LS(IDS_CMD_SECT_COMPO_GRID_STIFFENER)));
		SetCoveredCellsRowCol(nRow, 2, nRow, 3);
		SetStyleRange(CGXRange(nRow, 2), CGXStyle()
			.SetControl(GX_IDS_CTRL_PUSHBTN)
			.SetInterior(GXSYSCOLOR(COLOR_BTNFACE))
			.SetChoiceList(_LS(IDS_CMD_SECT_COMPO_GRID_STIFFENER))
			.SetTextColor(COLORREF(RGB(0, 0, 0)))
			.SetEnabled(TRUE));
	}

	// Rivet
	if (m_nNumRivet > 0)
	{
		nRow++;
		InsertRows(nRow, 1);
		SetCoveredCellsRowCol(nRow, 1, nRow, 3);
		SetStyleRange(CGXRange(nRow, 1), CGXStyle()
			.SetInterior(COLORREF(RGB(0, 0, 255)))
			.SetTextColor(COLORREF(RGB(255, 255, 0)))
			.SetValue(_LS(IDS_CMD_COMPO_GRID_Rivet)));

		nRow++;
		m_nRivetRow = nRow;

		InsertRows(nRow, 1);  // Title
		SetStyleRange(CGXRange(nRow, 1), CGXStyle()
			.SetValue(_LS(IDS_CMD_SECT_COMPO_GRID_RIVET)));
		SetCoveredCellsRowCol(nRow, 2, nRow, 3);
		SetStyleRange(CGXRange(nRow, 2), CGXStyle()
			.SetControl(GX_IDS_CTRL_PUSHBTN)
			.SetInterior(GXSYSCOLOR(COLOR_BTNFACE))
			.SetChoiceList(_LS(IDS_CMD_SECT_COMPO_GRID_RIVET))
			.SetTextColor(COLORREF(RGB(0, 0, 0)))
			.SetEnabled(TRUE));

	}

	// Row Insert END

	SetStyleRange(CGXRange().SetTable(), CGXStyle().SetVerticalAlignment(DT_VCENTER));
	SetStyleRange(CGXRange().SetCols(1), CGXStyle().SetEnabled(FALSE));
	SetStyleRange(CGXRange().SetCols(3), CGXStyle().SetEnabled(FALSE)); 

	SetColumnStatus();

	GetParam()->SetLockReadOnly(bOldReadOnlyLock);
	LockUpdate(bOldLock);
	if (!bOldLock) Redraw();
	if (m_pPreview) 
	{
		m_pPreview->SetCentroidFlag(FALSE);
		m_pPreview->Invalidate();
	}
	if(m_nShapeIndex == 2)
	{
		m_pParent->DrawStlgViewDlgSection();
	}  
}

// *^^* 여기 추가 
void CCMSectItemGrid4Stlg::SetViewData() // MNET:XXXX 20070221 changed by mylee
{
	m_nNumStiffener2 = 0;
	m_nNumSize = 0;
	m_nNumRivet = 0;

	switch(m_nShapeIndex)
	{  
	case 0: // Stl Girder Box
		m_nNumSize = 14;
		m_nNumStiffener2 = 2;
		break;
	case 1: // Stl Girder I
		m_nNumSize = 11;
		m_nNumStiffener2 = 2;
		m_nNumRivet = 2;
		break;
	case 2: // Stl Girder MCell
		if(m_pSectD->SectBefore.nFlangeShape == 0)
		{
			m_nNumSize = 16;
			m_nNumStiffener2 = 2;
		}
		else if(m_pSectD->SectBefore.nFlangeShape == 1)
		{
			m_nNumSize = 22;
			m_nNumStiffener2 = 2;
		}
		else if(m_pSectD->SectBefore.nFlangeShape == 2)
		{
			m_nNumSize = 28;
			m_nNumStiffener2 = 2;
		}
		else if(m_pSectD->SectBefore.nFlangeShape == 3)
		{
			m_nNumSize = 20;
			m_nNumStiffener2 = 2;
		}
		break;
	default:
		//ASSERT(0);
		break;
	}
}

BOOL CCMSectItemGrid4Stlg::SaveData()
{
	BOOL bSuccess = SaveFlag();
	bSuccess = bSuccess && SaveSize();
	if(bSuccess)
	{
		SetSizeAuto();
	}
	return bSuccess;
}

void CCMSectItemGrid4Stlg::SetSizeAuto()
{
	if(m_nShapeIndex == 2)
	{
		int nFShape = m_pSectD->SectBefore.nFlangeShape;    
		int nCellNum = m_pSectD->SectBefore.nCellNum;    
		for (int i = 0; i < 2; i++)
		{
			T_SECT_SECTBASE_D* pSectBase = (i == 0) ? &m_pSectD->SectBefore.SectI : &m_pSectD->SectBefore.SectJ;

			if((nFShape == 0 || nFShape == 1 || nFShape == 2) && nCellNum != 0)
			{
				double dB2 = pSectBase->Size[1];
				double dB3 = pSectBase->Size[2];
				pSectBase->Size[4] = max(dB2 - dB3, 0.0);   //B5

				double dBr2 = pSectBase->Size[7];
				double dBr3 = pSectBase->Size[8];
				pSectBase->Size[10] = max(dBr2 - dBr3, 0.0); //Br5
			}
			else if(nFShape == 3)
			{
				double dB1 = pSectBase->Size[0];
				double dB2 = pSectBase->Size[1];
				pSectBase->Size[2] = max(dB1 - dB2, 0.0); //B3

				double dBr1 = pSectBase->Size[4];
				double dBr2 = pSectBase->Size[5];
				pSectBase->Size[6] = max(dBr1 - dBr2, 0.0); //Br3
			}
		}
	}
}

BOOL CCMSectItemGrid4Stlg::SaveFlag()
{
	CString csNum;
	if(m_nShapeIndex == 0 || m_nShapeIndex == 1) // MCell은 대화상자에서 아래 값을 세팅하기 때문에 초기화 안함
	{
		// symmetry
		m_pSectD->SectBefore.bSymmetric = 0;

		// cell type
		m_pSectD->SectBefore.nCellType = 0;

		// cell shape
		m_pSectD->SectBefore.nCellShape = 0;
	}  
		
	return TRUE;
}

BOOL CCMSectItemGrid4Stlg::SaveSize() // MNET:XXXX 20070221 changed by mylee
{
	CString csNum;
	int nNum = 0;
	double dNum = 0.0;
	double* pSize;
	CArray<double, double> aStiff;
	int nSizeRow;

	for (int n = 0; n < 2; n++)
	{    
		// size
		if (n == 0) 
		{
			nSizeRow   = m_nSizeIRow;
			pSize = m_pSectD->SectBefore.SectI.Size;
		}
		else
		{
			nSizeRow   = m_nSizeJRow;
			pSize = m_pSectD->SectBefore.SectJ.Size;
		}
		
		if(m_nShapeIndex == 0 || m_nShapeIndex == 1)
		{
			// Stl Girder I,Box,Tub는 Size그룹이.. Symetric(CheckBox), Top, Bot, Size로 구성되기 때문에 따로 처리 함
			// 이 함수 안에서 SymmetricAutoCalc연산도 수행 함.
			SaveSizeStlGirderBoxI(nSizeRow, n, pSize);
		}
		else if(m_nShapeIndex == 2)
		{
			SaveSizeStlGirderMCell(nSizeRow, n, pSize);
		}
		else
		{
			ASSERT(0);
			for (int i = 1; i < m_nNumSize; i++)
			{
				csNum = GetEditingValue(nSizeRow+i, 2);
				if (GetDoubleValue(csNum, dNum))  
					pSize[i-1] = dNum;
				else
					pSize[i-1] = 0.0;
			}
		}  
	}

	return TRUE;
}

//---------------------------------------------------------------------------
// Implementation Function
BOOL CCMSectItemGrid4Stlg::SetLeftValidCell(ROWCOL nRow, ROWCOL nCol)
{
	ROWCOL i, j;
	i = nRow; j = nCol-1; // 현재 행 이전 열
	if (nCol == 0 || j < 1) { i--; j = GetColCount(); }  

	while (i >= 1)
	{
		while (j >= 1)
		{
			// 열이 선택될 수 있는지 조사
			//if (SetCurrentCell(i, j)) return TRUE;
			CGXStyle style;
			ComposeStyleRowCol(i, j, &style);
			if (!style.GetIncludeEnabled() || style.GetEnabled())
			{
				m_bValidateFail = FALSE;
				if (SetCurrentCell(i, j)) return TRUE;
				if (m_bValidateFail) return FALSE;
			}
			j--;  // 이전 열을 조사
		}
		i--;  // 이전 줄로 이동
		j = GetColCount();
	}

	return FALSE;
}

BOOL CCMSectItemGrid4Stlg::SetRightValidCell(ROWCOL nRow, ROWCOL nCol)
{
	ROWCOL i, j;
	i = nRow; j = nCol+1; // 현재 행 다음 열
	if (j > GetColCount()) { i++; j = 1; }

	while (i <= GetRowCount())
	{
		while (j <= GetColCount())
		{
			// 열이 선택될 수 있는지 조사
			//if (SetCurrentCell(i, j)) return TRUE;
			CGXStyle style;
			ComposeStyleRowCol(i, j, &style);
			if (!style.GetIncludeEnabled() || style.GetEnabled())
			{
				m_bValidateFail = FALSE;
				if (SetCurrentCell(i, j)) return TRUE;
				if (m_bValidateFail) return FALSE;
			}
			j++;  // 다음 열을 조사
		}
		i++;  // 다음 줄로 이동
		j = 1;
	}

	return FALSE;
}

CString CCMSectItemGrid4Stlg::GetEditingValue(ROWCOL nRow, ROWCOL nCol)
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

// Joint, Type, Shape에 따라 해당 셀을 보여주거나 숨긴다.
void CCMSectItemGrid4Stlg::SetColumnStatus()
{
	HWND hWnd = GetSafeHwnd();
	if(!hWnd) return ;
	if((!::IsWindow(hWnd))) return ;
	
	CArray<BOOL, BOOL> aHideSizeI;   // TRUE->Hide
	CArray<BOOL, BOOL> aHideSizeJ;   // TRUE->Hide
	SetColumnStatusGetStatus(aHideSizeI, aHideSizeJ);

	BOOL bOldLock = LockUpdate(TRUE);
	
	for (int i = 0; i < aHideSizeI.GetSize(); i++)
	{
		SetColumnStatusShowHide(m_nSizeIRow+i, aHideSizeI[i]);    
	}
	for (int i = 0; i < aHideSizeJ.GetSize(); i++)
	{
		SetColumnStatusShowHide(m_nSizeJRow+i, aHideSizeJ[i]);    
	}

	LockUpdate(bOldLock);
	if (!bOldLock) Redraw();
}

// *^^* 여기 추가 
void CCMSectItemGrid4Stlg::SetColumnStatusGetStatus(CArray<BOOL, BOOL>& raHideSizeI, CArray<BOOL, BOOL>& raHideSizeJ)
{
	raHideSizeI.RemoveAll();
	raHideSizeJ.RemoveAll();

	CArray<int, int> aType4SizeI, aType4SizeJ; // 1:1개(Begin만), 2:2개(Begin, End), 3:n개(Begin부터End까지)
	CArray<int, int> aBegin4SizeI,aBegin4SizeJ;
	CArray<int, int> aEnd4SizeI, aEnd4SizeJ;

	BOOL bIJSame = FALSE;  // 이 값이 TRUE면 I J가 같다고 보고.. I값을 J값으로 복사 함..

	switch(m_nShapeIndex)
	{
	case 0: // Stl Girder Box
		{
			bIJSame = FALSE;

			if(m_bSymmetryStlGSectI)
			{ 
				aBegin4SizeI.Add(1);  aEnd4SizeI.Add(1);  aType4SizeI.Add(1); // Top
				aBegin4SizeI.Add(2);  aEnd4SizeI.Add(2);  aType4SizeI.Add(1); // Bot
				aBegin4SizeI.Add(5);  aEnd4SizeI.Add(5);  aType4SizeI.Add(1); // B3
				aBegin4SizeI.Add(8);  aEnd4SizeI.Add(8);  aType4SizeI.Add(1); // B6
				aBegin4SizeI.Add(13); aEnd4SizeI.Add(13); aType4SizeI.Add(1); // tw2
			}
			
			if(m_bSymmetryStlGSectJ)
			{				
				aBegin4SizeJ.Add(1);  aEnd4SizeJ.Add(1);  aType4SizeJ.Add(1); // Top
				aBegin4SizeJ.Add(2);  aEnd4SizeJ.Add(2);  aType4SizeJ.Add(1); // Bot
				aBegin4SizeJ.Add(5);  aEnd4SizeJ.Add(5);  aType4SizeJ.Add(1); // B3
				aBegin4SizeJ.Add(8);  aEnd4SizeJ.Add(8);  aType4SizeJ.Add(1); // B6
				aBegin4SizeJ.Add(13); aEnd4SizeJ.Add(13); aType4SizeJ.Add(1); // tw2
			}
		}
		break;
	case 1: // Stl Girder I
		{
			bIJSame = FALSE;
			
			if(m_bSymmetryStlGSectI)
			{        				
				aBegin4SizeI.Add(1);  aEnd4SizeI.Add(1);  aType4SizeI.Add(1); // Top
				aBegin4SizeI.Add(2);  aEnd4SizeI.Add(2);  aType4SizeI.Add(1); // Bot
				aBegin4SizeI.Add(4);  aEnd4SizeI.Add(4);  aType4SizeI.Add(1); // B2
				aBegin4SizeI.Add(6);  aEnd4SizeI.Add(6);  aType4SizeI.Add(1); // B4
			}
			
			if(m_bSymmetryStlGSectJ)
			{				
				aBegin4SizeJ.Add(1);  aEnd4SizeJ.Add(1);  aType4SizeJ.Add(1); // Top
				aBegin4SizeJ.Add(2);  aEnd4SizeJ.Add(2);  aType4SizeJ.Add(1); // Bot
				aBegin4SizeJ.Add(4);  aEnd4SizeJ.Add(4);  aType4SizeJ.Add(1); // B2
				aBegin4SizeJ.Add(6);  aEnd4SizeJ.Add(6);  aType4SizeJ.Add(1); // B4
			}
		}
		break;
	case 2:
		{
			bIJSame = FALSE;

			int nNoOfCell = m_pSectD->SectBefore.nCellNum;          
			int nFShape = m_pSectD->SectBefore.nFlangeShape;    
			int nLorR = m_pSectD->SectBefore.nCellType;    

			m_bSymmetryStlGSectI = m_bSymmetryStlGSectJ = m_pSectD->SectBefore.bSymmetric;

			BOOL bJ1, bJ2, bJr1, bJr2;      
			bJ1 = bJ2 = bJr1 = bJr2 = FALSE;
			if(m_pSectD->SectBefore.nJoint & 0x00000001) bJ1 = TRUE;
			if(m_pSectD->SectBefore.nJoint & 0x00000002) bJ2 = TRUE;
			if(m_pSectD->SectBefore.nJoint & 0x00000004) bJr1 = TRUE;
			if(m_pSectD->SectBefore.nJoint & 0x00000008) bJr2 = TRUE;

			CArray<int, int> aHIdeIdxI, aHIdeIdxJ;
			aHIdeIdxI.RemoveAll();
			aHIdeIdxJ.RemoveAll();

			if(m_pSectD->SectBefore.nFlangeShape == 0)
			{
				if(!((nNoOfCell == 0 && nLorR == 0) || nNoOfCell > 0)                           ) aHIdeIdxI.Add(0);    // B1  
				if(!((nNoOfCell == 0 && nLorR == 0) || nNoOfCell > 0)                           ) aHIdeIdxI.Add(1);    // B2 
				if(!((nNoOfCell == 0 && nLorR == 0) || nNoOfCell > 0)                           ) aHIdeIdxI.Add(2);    // B3 
				if(!((nNoOfCell == 0 && nLorR == 0) || nNoOfCell > 0)                           ) aHIdeIdxI.Add(3);    // B4 
				if(!(nNoOfCell == 0 && nLorR == 0)                                              ) aHIdeIdxI.Add(4);    // B5
				if(!(nNoOfCell > 2)                                                             ) aHIdeIdxI.Add(5);    // B6 
				if(!((nNoOfCell == 0 && nLorR == 1) || (nNoOfCell > 0 && !m_bSymmetryStlGSectI))) aHIdeIdxI.Add(6);    // Br1
				if(!((nNoOfCell == 0 && nLorR == 1) || (nNoOfCell > 0 && !m_bSymmetryStlGSectI))) aHIdeIdxI.Add(7);    // Br2
				if(!((nNoOfCell == 0 && nLorR == 1) || (nNoOfCell > 0 && !m_bSymmetryStlGSectI))) aHIdeIdxI.Add(8);    // Br3
				if(!((nNoOfCell == 0 && nLorR == 1) || (nNoOfCell > 0 && !m_bSymmetryStlGSectI))) aHIdeIdxI.Add(9);    // Br4
				if(!(nNoOfCell == 0 && nLorR == 1)                                              ) aHIdeIdxI.Add(10);   // Br5
				if(!(nNoOfCell > 1)                                                             ) aHIdeIdxI.Add(15);   // Tw2

				if(!((nNoOfCell == 0 && nLorR == 0) || nNoOfCell > 0)                           ) aHIdeIdxJ.Add(0);    // B1 
				if(!((nNoOfCell == 0 && nLorR == 0) || nNoOfCell > 0)                           ) aHIdeIdxJ.Add(1);    // B2 
				if(!((nNoOfCell == 0 && nLorR == 0) || nNoOfCell > 0)                           ) aHIdeIdxJ.Add(2);    // B3 
				if(!((nNoOfCell == 0 && nLorR == 0) || nNoOfCell > 0)                           ) aHIdeIdxJ.Add(3);    // B4 
				if(!(nNoOfCell == 0 && nLorR == 0)                                              ) aHIdeIdxJ.Add(4);    // B5 
				if(!(nNoOfCell > 2)                                                             ) aHIdeIdxJ.Add(5);    // B6 
				if(!((nNoOfCell == 0 && nLorR == 1) || (nNoOfCell > 0 && !m_bSymmetryStlGSectJ))) aHIdeIdxJ.Add(6);    // Br1
				if(!((nNoOfCell == 0 && nLorR == 1) || (nNoOfCell > 0 && !m_bSymmetryStlGSectJ))) aHIdeIdxJ.Add(7);    // Br2
				if(!((nNoOfCell == 0 && nLorR == 1) || (nNoOfCell > 0 && !m_bSymmetryStlGSectJ))) aHIdeIdxJ.Add(8);    // Br3
				if(!((nNoOfCell == 0 && nLorR == 1) || (nNoOfCell > 0 && !m_bSymmetryStlGSectJ))) aHIdeIdxJ.Add(9);    // Br4
				if(!(nNoOfCell == 0 && nLorR == 1)                                              ) aHIdeIdxJ.Add(10);   // Br5
				if(!(nNoOfCell > 1)                                                             ) aHIdeIdxJ.Add(15);   // Tw2
			}
			else if(m_pSectD->SectBefore.nFlangeShape == 1)
			{
				if(!((nNoOfCell == 0 && nLorR == 0) || nNoOfCell > 0)                           ) aHIdeIdxI.Add(0);    // B1 
				if(!((nNoOfCell == 0 && nLorR == 0) || nNoOfCell > 0)                           ) aHIdeIdxI.Add(1);    // B2 
				if(!((nNoOfCell == 0 && nLorR == 0) || nNoOfCell > 0)                           ) aHIdeIdxI.Add(2);    // B3 
				if(!((nNoOfCell == 0 && nLorR == 0) || nNoOfCell > 0)                           ) aHIdeIdxI.Add(3);    // B4 
				if(!(nNoOfCell == 0 && nLorR == 0)                                              ) aHIdeIdxI.Add(4);    // B5
				if(!(nNoOfCell > 2)                                                             ) aHIdeIdxI.Add(5);    // B6 
				if(!((nNoOfCell == 0 && nLorR == 1) || (nNoOfCell > 0 && !m_bSymmetryStlGSectI))) aHIdeIdxI.Add(6);    // Br1
				if(!((nNoOfCell == 0 && nLorR == 1) || (nNoOfCell > 0 && !m_bSymmetryStlGSectI))) aHIdeIdxI.Add(7);    // Br2
				if(!((nNoOfCell == 0 && nLorR == 1) || (nNoOfCell > 0 && !m_bSymmetryStlGSectI))) aHIdeIdxI.Add(8);    // Br3
				if(!((nNoOfCell == 0 && nLorR == 1) || (nNoOfCell > 0 && !m_bSymmetryStlGSectI))) aHIdeIdxI.Add(9);    // Br4
				if(!(nNoOfCell == 0 && nLorR == 1)                                              ) aHIdeIdxI.Add(10);   // Br5
				if(!(nNoOfCell > 1)                                                             ) aHIdeIdxI.Add(15);   // Tw2
				if(!((nNoOfCell == 0 && nLorR == 0) || nNoOfCell > 0)                           ) aHIdeIdxI.Add(16);   // rh 
				if(!((nNoOfCell == 0 && nLorR == 0) || nNoOfCell > 0)                           ) aHIdeIdxI.Add(17);   // rt 
				if(!((nNoOfCell == 0 && nLorR == 0) || nNoOfCell > 0)                           ) aHIdeIdxI.Add(18);   // rb      
				if(!((nNoOfCell == 0 && nLorR == 1) || (nNoOfCell > 0 && !m_bSymmetryStlGSectI))) aHIdeIdxI.Add(19);   // rhr   
				if(!((nNoOfCell == 0 && nLorR == 1) || (nNoOfCell > 0 && !m_bSymmetryStlGSectI))) aHIdeIdxI.Add(20);   // rtr   
				if(!((nNoOfCell == 0 && nLorR == 1) || (nNoOfCell > 0 && !m_bSymmetryStlGSectI))) aHIdeIdxI.Add(21);   // rbr

				if(!((nNoOfCell == 0 && nLorR == 0) || nNoOfCell > 0)                           ) aHIdeIdxJ.Add(0);    // B1 
				if(!((nNoOfCell == 0 && nLorR == 0) || nNoOfCell > 0)                           ) aHIdeIdxJ.Add(1);    // B2 
				if(!((nNoOfCell == 0 && nLorR == 0) || nNoOfCell > 0)                           ) aHIdeIdxJ.Add(2);    // B3 
				if(!((nNoOfCell == 0 && nLorR == 0) || nNoOfCell > 0)                           ) aHIdeIdxJ.Add(3);    // B4 
				if(!(nNoOfCell == 0 && nLorR == 0)                                              ) aHIdeIdxJ.Add(4);    // B5 
				if(!(nNoOfCell > 2)                                                             ) aHIdeIdxJ.Add(5);    // B6 
				if(!((nNoOfCell == 0 && nLorR == 1) || (nNoOfCell > 0 && !m_bSymmetryStlGSectJ))) aHIdeIdxJ.Add(6);    // Br1
				if(!((nNoOfCell == 0 && nLorR == 1) || (nNoOfCell > 0 && !m_bSymmetryStlGSectJ))) aHIdeIdxJ.Add(7);    // Br2
				if(!((nNoOfCell == 0 && nLorR == 1) || (nNoOfCell > 0 && !m_bSymmetryStlGSectJ))) aHIdeIdxJ.Add(8);    // Br3
				if(!((nNoOfCell == 0 && nLorR == 1) || (nNoOfCell > 0 && !m_bSymmetryStlGSectJ))) aHIdeIdxJ.Add(9);    // Br4
				if(!(nNoOfCell == 0 && nLorR == 1)                                              ) aHIdeIdxJ.Add(10);    // Br5
				if(!(nNoOfCell > 1)                                                             ) aHIdeIdxJ.Add(15);   // Tw2
				if(!((nNoOfCell == 0 && nLorR == 0) || nNoOfCell > 0)                           ) aHIdeIdxJ.Add(16);   // rh 
				if(!((nNoOfCell == 0 && nLorR == 0) || nNoOfCell > 0)                           ) aHIdeIdxJ.Add(17);   // rt 
				if(!((nNoOfCell == 0 && nLorR == 0) || nNoOfCell > 0)                           ) aHIdeIdxJ.Add(18);   // rb       
				if(!((nNoOfCell == 0 && nLorR == 1) || (nNoOfCell > 0 && !m_bSymmetryStlGSectJ))) aHIdeIdxJ.Add(19);   // rhr    
				if(!((nNoOfCell == 0 && nLorR == 1) || (nNoOfCell > 0 && !m_bSymmetryStlGSectJ))) aHIdeIdxJ.Add(20);   // rtr    
				if(!((nNoOfCell == 0 && nLorR == 1) || (nNoOfCell > 0 && !m_bSymmetryStlGSectJ))) aHIdeIdxJ.Add(21);   // rbr
			}
			else if(m_pSectD->SectBefore.nFlangeShape == 2)
			{
				if(!((nNoOfCell == 0 && nLorR == 0) || nNoOfCell > 0)                           ) aHIdeIdxI.Add(0);    // B1   
				if(!((nNoOfCell == 0 && nLorR == 0) || nNoOfCell > 0)                           ) aHIdeIdxI.Add(1);    // B2 
				if(!((nNoOfCell == 0 && nLorR == 0) || nNoOfCell > 0)                           ) aHIdeIdxI.Add(2);    // B3 
				if(!((nNoOfCell == 0 && nLorR == 0) || nNoOfCell > 0)                           ) aHIdeIdxI.Add(3);    // B4 
				if(!(nNoOfCell == 0 && nLorR == 0)                                              ) aHIdeIdxI.Add(4);    // B5
				if(!(nNoOfCell > 2)                                                             ) aHIdeIdxI.Add(5);    // B6 
				if(!((nNoOfCell == 0 && nLorR == 1) || (nNoOfCell > 0 && !m_bSymmetryStlGSectI))) aHIdeIdxI.Add(6);    // Br1
				if(!((nNoOfCell == 0 && nLorR == 1) || (nNoOfCell > 0 && !m_bSymmetryStlGSectI))) aHIdeIdxI.Add(7);    // Br2
				if(!((nNoOfCell == 0 && nLorR == 1) || (nNoOfCell > 0 && !m_bSymmetryStlGSectI))) aHIdeIdxI.Add(8);    // Br3
				if(!((nNoOfCell == 0 && nLorR == 1) || (nNoOfCell > 0 && !m_bSymmetryStlGSectI))) aHIdeIdxI.Add(9);    // Br4
				if(!(nNoOfCell == 0 && nLorR == 1)                                              ) aHIdeIdxI.Add(10);   // Br5
				if(!(nNoOfCell > 1)                                                             ) aHIdeIdxI.Add(15);   // Tw2
				if(!((nNoOfCell == 0 && nLorR == 0) || nNoOfCell > 0)                           ) aHIdeIdxI.Add(16);   // rh1 
				if(!((nNoOfCell == 0 && nLorR == 0) || nNoOfCell > 0)                           ) aHIdeIdxI.Add(17);   // rh2 
				if(!((nNoOfCell == 0 && nLorR == 0) || nNoOfCell > 0)                           ) aHIdeIdxI.Add(18);   // rh3 
				if(!((nNoOfCell == 0 && nLorR == 0) || nNoOfCell > 0)                           ) aHIdeIdxI.Add(19);   // rb  
				if(!((nNoOfCell == 0 && nLorR == 0) || nNoOfCell > 0)                           ) aHIdeIdxI.Add(20);   // rt1 
				if(!((nNoOfCell == 0 && nLorR == 0) || nNoOfCell > 0)                           ) aHIdeIdxI.Add(21);   // rt2         
				if(!((nNoOfCell == 0 && nLorR == 1) || (nNoOfCell > 0 && !m_bSymmetryStlGSectI))) aHIdeIdxI.Add(22);   // rhr1  
				if(!((nNoOfCell == 0 && nLorR == 1) || (nNoOfCell > 0 && !m_bSymmetryStlGSectI))) aHIdeIdxI.Add(23);   // rhr2  
				if(!((nNoOfCell == 0 && nLorR == 1) || (nNoOfCell > 0 && !m_bSymmetryStlGSectI))) aHIdeIdxI.Add(24);   // rhr3  
				if(!((nNoOfCell == 0 && nLorR == 1) || (nNoOfCell > 0 && !m_bSymmetryStlGSectI))) aHIdeIdxI.Add(25);   // rbr   
				if(!((nNoOfCell == 0 && nLorR == 1) || (nNoOfCell > 0 && !m_bSymmetryStlGSectI))) aHIdeIdxI.Add(26);   // rtr1  
				if(!((nNoOfCell == 0 && nLorR == 1) || (nNoOfCell > 0 && !m_bSymmetryStlGSectI))) aHIdeIdxI.Add(27);   // rtr2

				if(!((nNoOfCell == 0 && nLorR == 0) || nNoOfCell > 0)                           ) aHIdeIdxJ.Add(0);    // B1   
				if(!((nNoOfCell == 0 && nLorR == 0) || nNoOfCell > 0)                           ) aHIdeIdxJ.Add(1);    // B2 
				if(!((nNoOfCell == 0 && nLorR == 0) || nNoOfCell > 0)                           ) aHIdeIdxJ.Add(2);    // B3 
				if(!((nNoOfCell == 0 && nLorR == 0) || nNoOfCell > 0)                           ) aHIdeIdxJ.Add(3);    // B4 
				if(!(nNoOfCell == 0 && nLorR == 0)                                              ) aHIdeIdxJ.Add(4);    // B5 
				if(!(nNoOfCell > 2)                                                             ) aHIdeIdxJ.Add(5);    // B6 
				if(!((nNoOfCell == 0 && nLorR == 1) || (nNoOfCell > 0 && !m_bSymmetryStlGSectJ))) aHIdeIdxJ.Add(6);    // Br1
				if(!((nNoOfCell == 0 && nLorR == 1) || (nNoOfCell > 0 && !m_bSymmetryStlGSectJ))) aHIdeIdxJ.Add(7);    // Br2
				if(!((nNoOfCell == 0 && nLorR == 1) || (nNoOfCell > 0 && !m_bSymmetryStlGSectJ))) aHIdeIdxJ.Add(8);    // Br3
				if(!((nNoOfCell == 0 && nLorR == 1) || (nNoOfCell > 0 && !m_bSymmetryStlGSectJ))) aHIdeIdxJ.Add(9);    // Br4
				if(!(nNoOfCell == 0 && nLorR == 1)                                              ) aHIdeIdxJ.Add(10);   // Br5
				if(!(nNoOfCell > 1)                                                             ) aHIdeIdxJ.Add(15);   // Tw2
				if(!((nNoOfCell == 0 && nLorR == 0) || nNoOfCell > 0)                           ) aHIdeIdxJ.Add(16);   // rh1 
				if(!((nNoOfCell == 0 && nLorR == 0) || nNoOfCell > 0)                           ) aHIdeIdxJ.Add(17);   // rh2 
				if(!((nNoOfCell == 0 && nLorR == 0) || nNoOfCell > 0)                           ) aHIdeIdxJ.Add(18);   // rh3 
				if(!((nNoOfCell == 0 && nLorR == 0) || nNoOfCell > 0)                           ) aHIdeIdxJ.Add(19);   // rb  
				if(!((nNoOfCell == 0 && nLorR == 0) || nNoOfCell > 0)                           ) aHIdeIdxJ.Add(20);   // rt1  
				if(!((nNoOfCell == 0 && nLorR == 0) || nNoOfCell > 0)                           ) aHIdeIdxJ.Add(21);   // rt2           
				if(!((nNoOfCell == 0 && nLorR == 1) || (nNoOfCell > 0 && !m_bSymmetryStlGSectJ))) aHIdeIdxJ.Add(22);   // rhr1    
				if(!((nNoOfCell == 0 && nLorR == 1) || (nNoOfCell > 0 && !m_bSymmetryStlGSectJ))) aHIdeIdxJ.Add(23);   // rhr2    
				if(!((nNoOfCell == 0 && nLorR == 1) || (nNoOfCell > 0 && !m_bSymmetryStlGSectJ))) aHIdeIdxJ.Add(24);   // rhr3    
				if(!((nNoOfCell == 0 && nLorR == 1) || (nNoOfCell > 0 && !m_bSymmetryStlGSectJ))) aHIdeIdxJ.Add(25);   // rbr     
				if(!((nNoOfCell == 0 && nLorR == 1) || (nNoOfCell > 0 && !m_bSymmetryStlGSectJ))) aHIdeIdxJ.Add(26);   // rtr1    
				if(!((nNoOfCell == 0 && nLorR == 1) || (nNoOfCell > 0 && !m_bSymmetryStlGSectJ))) aHIdeIdxJ.Add(27);   // rtr2
			}
			else if(m_pSectD->SectBefore.nFlangeShape == 3)
			{
				if(!((nNoOfCell == 0 && nLorR == 0) || nNoOfCell > 0)                                      ) aHIdeIdxI.Add(0);    // B1  
				if(!((nNoOfCell == 0 && nLorR == 0) || nNoOfCell > 0)                                      ) aHIdeIdxI.Add(1);    // B2  
				aHIdeIdxI.Add(2);                                                                                                 // B3  
				if(!(nNoOfCell > 2)                                                                        ) aHIdeIdxI.Add(3);    // B4  
				if(!((nNoOfCell == 0 && nLorR == 1) || (nNoOfCell > 0 && !m_bSymmetryStlGSectI))           ) aHIdeIdxI.Add(4);    // Br1 
				if(!((nNoOfCell == 0 && nLorR == 1) || (nNoOfCell > 0 && !m_bSymmetryStlGSectI))           ) aHIdeIdxI.Add(5);    // Br2 
				aHIdeIdxI.Add(6);                                                                                                 // Br3 
				if(!(nNoOfCell > 1)                                                                        ) aHIdeIdxI.Add(11);   // Tw2
				if(!(((nNoOfCell == 0 && nLorR == 0) || nNoOfCell > 0) && bJ1)                             ) aHIdeIdxI.Add(12);   // jh1 
				if(!(((nNoOfCell == 0 && nLorR == 0) || nNoOfCell > 0) && bJ1)                             ) aHIdeIdxI.Add(13);   // jb1 
				if(!(((nNoOfCell == 0 && nLorR == 0) || nNoOfCell > 0) && bJ2)                             ) aHIdeIdxI.Add(14);   // jh2          
				if(!(((nNoOfCell == 0 && nLorR == 0) || nNoOfCell > 0) && bJ2)                             ) aHIdeIdxI.Add(15);   // jb2   
				if(!(((nNoOfCell == 0 && nLorR == 1) || (nNoOfCell > 0 && !m_bSymmetryStlGSectI)) && bJr1) ) aHIdeIdxI.Add(16);   // jhr1   
				if(!(((nNoOfCell == 0 && nLorR == 1) || (nNoOfCell > 0 && !m_bSymmetryStlGSectI)) && bJr1) ) aHIdeIdxI.Add(17);   // jbr1   
				if(!(((nNoOfCell == 0 && nLorR == 1) || (nNoOfCell > 0 && !m_bSymmetryStlGSectI)) && bJr2) ) aHIdeIdxI.Add(18);   // jhr2   
				if(!(((nNoOfCell == 0 && nLorR == 1) || (nNoOfCell > 0 && !m_bSymmetryStlGSectI)) && bJr2) ) aHIdeIdxI.Add(19);   // jbr2   

				if(!((nNoOfCell == 0 && nLorR == 0) || nNoOfCell > 0)                                      ) aHIdeIdxJ.Add(0);    // B1  
				if(!((nNoOfCell == 0 && nLorR == 0) || nNoOfCell > 0)                                      ) aHIdeIdxJ.Add(1);    // B2  
				aHIdeIdxJ.Add(2);                                                                                                 // B3  
				if(!(nNoOfCell > 2)                                                                        ) aHIdeIdxJ.Add(3);    // B4  
				if(!((nNoOfCell == 0 && nLorR == 1) || (nNoOfCell > 0 && !m_bSymmetryStlGSectJ))           ) aHIdeIdxJ.Add(4);    // Br1 
				if(!((nNoOfCell == 0 && nLorR == 1) || (nNoOfCell > 0 && !m_bSymmetryStlGSectJ))           ) aHIdeIdxJ.Add(5);    // Br2 
				aHIdeIdxJ.Add(6);                                                                                                 // Br3 
				if(!(nNoOfCell > 1)                                                                        ) aHIdeIdxJ.Add(11);   // Tw2
				if(!(((nNoOfCell == 0 && nLorR == 0) || nNoOfCell > 0) && bJ1)                             ) aHIdeIdxJ.Add(12);   // jh1 
				if(!(((nNoOfCell == 0 && nLorR == 0) || nNoOfCell > 0) && bJ1)                             ) aHIdeIdxJ.Add(13);   // jb1 
				if(!(((nNoOfCell == 0 && nLorR == 0) || nNoOfCell > 0) && bJ2)                             ) aHIdeIdxJ.Add(14);   // jh2          
				if(!(((nNoOfCell == 0 && nLorR == 0) || nNoOfCell > 0) && bJ2)                             ) aHIdeIdxJ.Add(15);   // jb2   
				if(!(((nNoOfCell == 0 && nLorR == 1) || (nNoOfCell > 0 && !m_bSymmetryStlGSectJ)) && bJr1) ) aHIdeIdxJ.Add(16);   // jhr1   
				if(!(((nNoOfCell == 0 && nLorR == 1) || (nNoOfCell > 0 && !m_bSymmetryStlGSectJ)) && bJr1) ) aHIdeIdxJ.Add(17);   // jbr1   
				if(!(((nNoOfCell == 0 && nLorR == 1) || (nNoOfCell > 0 && !m_bSymmetryStlGSectJ)) && bJr2) ) aHIdeIdxJ.Add(18);   // jhr2   
				if(!(((nNoOfCell == 0 && nLorR == 1) || (nNoOfCell > 0 && !m_bSymmetryStlGSectJ)) && bJr2) ) aHIdeIdxJ.Add(19);   // jbr2     
			}

			for (int i = 0; i < aHIdeIdxI.GetSize(); i++)
			{
				aBegin4SizeI.Add(aHIdeIdxI[i]);
				aEnd4SizeI.Add(aHIdeIdxI[i]);  
				aType4SizeI.Add(1);
			}

			for (int i = 0; i < aHIdeIdxJ.GetSize(); i++)
			{
				aBegin4SizeJ.Add(aHIdeIdxJ[i]);
				aEnd4SizeJ.Add(aHIdeIdxJ[i]);  
				aType4SizeJ.Add(1);
			}
		}
		break;
	}
	
	if(bIJSame)
	{
		aType4SizeJ.RemoveAll();
		aType4SizeJ.Copy(aType4SizeI);

		aBegin4SizeJ.RemoveAll();
		aBegin4SizeJ.Copy(aBegin4SizeI);

		aEnd4SizeJ.RemoveAll();
		aEnd4SizeJ.Copy(aBegin4SizeI);
	}

	SetColumnStatusSetHideFlag(raHideSizeI, m_nNumSize, aType4SizeI, aBegin4SizeI, aEnd4SizeI);
	SetColumnStatusSetHideFlag(raHideSizeJ, m_nNumSize, aType4SizeJ, aBegin4SizeJ, aEnd4SizeJ);

	// 특정 행을 무조건 hide 하고싶으면 아래 코드처럼 하면 됨
	//if(m_nShapeIndex != 0 && m_nShapeIndex != 1) // Stlg Box, Stlg I 가 아닌 경우 0번째가 Symmetric 이므로 hide 하지 않음
	//{
	//  if(CSectDB::IsEnable7thDOF())
	//  { //Tapered에서 Slab 정보 행 숨김.
	//    if(raHideSizeI.GetSize()>0) raHideSizeI[0]=TRUE;
	//    if(raHideSizeJ.GetSize()>0) raHideSizeJ[0]=TRUE;
	//  }
	//}
}

void CCMSectItemGrid4Stlg::SetColumnStatusSetHideFlag(CArray<BOOL, BOOL>& raHideFlag, 
																int nCount, 
																CArray<int, int>& raType, 
																CArray<int, int>& raBegin, 
																CArray<int, int>& raEnd)
{
	raHideFlag.SetSize(nCount);
	for (int i = 0; i < nCount; i++)
		raHideFlag[i] = FALSE;
	for (int i = 0; i < raType.GetSize(); i++)
	{
		int begin = raBegin[i];
		int end = raEnd[i];
		if (raType[i] == 1) raHideFlag[begin] = TRUE;
		else if (raType[i] == 2) raHideFlag[begin] = raHideFlag[end] = TRUE;
		else
		{
			for (int j = begin; j <= end; j++)
				raHideFlag[j] = TRUE;
		}
	}
}

void CCMSectItemGrid4Stlg::SetColumnStatusShowHide(ROWCOL nRow, BOOL bHide)
{
	SetStyleRange(CGXRange(nRow, 2), CGXStyle().SetEnabled(!bHide));
	HideRows(nRow, nRow, bHide);
}

BOOL CCMSectItemGrid4Stlg::IsEditing()
{
	return GetEditingControl() != 0;
}

void CCMSectItemGrid4Stlg::CancelEditing()
{
	CGXControl* pControl = GetEditingControl();
	if (pControl)
	{
		ROWCOL ncRow, ncCol;
		GetCurrentCell(ncRow, ncCol);
		pControl->OnCancelEditing();
		pControl->Init(ncRow, ncCol);
		pControl->Refresh();
		pControl->OnCanceledEditing();
	}
}

CGXControl* CCMSectItemGrid4Stlg::GetEditingControl()
{
	ROWCOL ncRow, ncCol;
	if (GetCurrentCell(ncRow, ncCol))
	{
		CGXControl* pControl = GetControl(ncRow, ncCol);
		if (pControl && pControl->IsActive()) return pControl;
	}
	return 0;
}

BEGIN_MESSAGE_MAP(CCMSectItemGrid4Stlg, CGXGridWnd)
	//{{AFX_MSG_MAP(CCMSectItemGrid4Stlg)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CCMSectItemGrid4Stlg message handlers
/////////////////////////////////////////////////////////////////////////////
BOOL CCMSectItemGrid4Stlg::ProcessKeys(CWnd *pSender, UINT nMessage, UINT nChar, UINT nRepCnt, UINT flags)
{
	CRowColArray awRows;
	ROWCOL nRow=1, nCol=1;
	//CGXControl* pControl;

	if (!GetCurrentCell(nRow, nCol)) // if no current cell, default action
		return CGXGridWnd::ProcessKeys(pSender, nMessage, nChar, nRepCnt, flags);

	//pControl = GetControl(nRow, nCol);
	if (nMessage == WM_KEYDOWN)
	{
		switch(nChar)
		{
		case VK_TAB: 
			{
				BOOL bShift = GetKeyState(VK_SHIFT) & 0x8000;
				if (bShift && SetLeftValidCell(nRow, nCol-1)) break;
				if (!bShift && SetRightValidCell(nRow, nCol+1)) break;
				if (IsEditing()) break;

				CWnd* pWnd = GetParent();
				if (pWnd && pWnd->GetSafeHwnd())
				{
					//pWnd->PostMessage(nMessage, nChar, nRepCnt);
					CWnd* pNextWnd = pWnd->GetNextDlgTabItem(this, bShift);
					if (pNextWnd && pNextWnd->GetSafeHwnd()) pNextWnd->SetFocus();
				}
			}
			break;
		case VK_RETURN: case VK_RIGHT: case VK_DOWN: // 다음 셀로 이동
			if (nRow < GetRowCount())
			{ if (!SetRightValidCell(nRow, nCol+1)) return FALSE; }
			if (nChar == VK_DOWN) return TRUE;
			break;
		case VK_LEFT: case VK_UP: // 이전 셀로 이동
			if (nRow > 1)
			{ if (!SetLeftValidCell(nRow, nCol-1)) return FALSE; }
			if (nChar == VK_UP) return TRUE;
			break;
		}
	}
	return CGXGridWnd::ProcessKeys(pSender, nMessage, nChar, nRepCnt, flags);
}

BOOL CCMSectItemGrid4Stlg::OnValidateCell(ROWCOL nRow, ROWCOL nCol)
{
	if ( (nRow >= m_nSizeIRow     && nRow < m_nSizeIRow+m_nNumSize)     ||
			 (nRow >= m_nSizeJRow     && nRow < m_nSizeJRow+m_nNumSize))
	{
		CString csNum = GetEditingValue(nRow, nCol);
		double dNum;
		if (!GetDoubleValue(csNum, dNum)) { m_bValidateFail = TRUE; return FALSE;}
	}

	//// 정수만 입력받도록
	//if ( nRow == m_nGirderRow || 
	//     (m_nNumStiffener>0 && (nRow == m_nStiffenerRow || nRow == m_nStiffenerRow+3)) )
	//{
	//  CString csNum = GetEditingValue(nRow, nCol);
	//  double dNum;
	//  if (!GetDoubleValue(csNum, dNum)) { m_bValidateFail = TRUE; return FALSE; }
	//  if (dNum != (int)dNum) 
	//  {
	//    SetWarningText(_LS(IDS_CMD0417__Error___Invalid_formula_));
	//    m_bValidateFail = TRUE; return FALSE;
	//  }
	//}

	return TRUE;
}

BOOL CCMSectItemGrid4Stlg::OnActivateGrid(BOOL bActivate)
{
	/*
	if (bActivate) return CGXGridWnd::OnActivateGrid(bActivate);
	ROWCOL nRow, nCol;
	if (GetCurrentCell(nRow, nCol))
	{
		if (!OnValidateCell(nRow, nCol)) 
		{
			CGXControl* pControl = GetControl(nRow, nCol);
			if (pControl && pControl->IsActive())
			{
				  pControl->OnCancelEditing();
					pControl->Init(nRow, nCol);
					pControl->Refresh();
					pControl->OnCanceledEditing();
			}
		}
	}
	*/
	return CGXGridWnd::OnActivateGrid(bActivate);
}

void CCMSectItemGrid4Stlg::OnModifyCell(ROWCOL nRow, ROWCOL nCol)
{
	CGXGridWnd::OnModifyCell(nRow, nCol);
	BOOL bShowData = FALSE;
	
	SaveData();  
	
	if (m_pPreview != 0) 
	{
		m_pPreview->SetCentroidFlag(FALSE);
		m_pPreview->Invalidate();
	}
	if(m_nShapeIndex == 2)
	{
		m_pParent->DrawStlgViewDlgSection();
	}
}

BOOL CCMSectItemGrid4Stlg::OnStartEditing(ROWCOL nRow, ROWCOL nCol)
{
	if (!CGXGridWnd::OnStartEditing(nRow, nCol)) return FALSE;

	//BOOL bChanged = FALSE;
	//
	//// auto shear check positon
	//if (nRow == m_nAutoShearCheckRow[0]   || 
	//    nRow == m_nAutoShearCheckRow[0]+2 || 
	//    nRow == m_nAutoShearCheckRow[0]+4)
	//{
	//  if (nCol == 2) bChanged = TRUE;
	//}
	//if (nRow == m_nAutoShearCheckRow[1]   || 
	//    nRow == m_nAutoShearCheckRow[1]+2 || 
	//    nRow == m_nAutoShearCheckRow[1]+4)
	//{
	//  if (nCol == 2) bChanged = TRUE;
	//}
	//
	//if (bChanged)
	//{
	//  // SaveData에서 Current Cell이 CheckBox이면 값을 Reverse해서 가져오기
	//  m_bOnStartEditing = TRUE; 
	//  SaveData();
	//  m_bOnStartEditing = FALSE;
	//  SetViewData();
	//  SetColumnStatus();
	//  if (m_pPreview != 0)
	//  {
	//    m_pPreview->SetCentroidFlag(FALSE);
	//    m_pPreview->Invalidate();
	//  }
	//}

	return TRUE;
}

BOOL CCMSectItemGrid4Stlg::OnPasteFromClipboard(const CGXRange &range)
{
	/*
	if (!CGXGridWnd::OnPasteFromClipboard(range)) return FALSE;
	SaveData();
	OnSizeChange();
	//Apply시 에러체킹한다.
	//m_pOtherEnd->m_pSectBase->BuiltUpFlag = m_pSectBase->BuiltUpFlag;
	//m_pOtherEnd->ShowData();
	*/
	return TRUE;
}

void CCMSectItemGrid4Stlg::OnClickedButtonRowCol(ROWCOL nRow, ROWCOL nCol)
{
	if ((nRow == m_nSymmetryStlGSectI || nRow == m_nSymmetryStlGSectJ) && m_nShapeIndex != 2)
	{
		CString csSymmetry = GetEditingValue(nRow, 3);
		BOOL bSymmetry = (csSymmetry == _T("0")) ? FALSE : TRUE;
		if(nRow == m_nSymmetryStlGSectI)
		{
			m_pSectD->SectBefore.SectI.bSymAutoCalc = bSymmetry;
		}
		else
		{
			m_pSectD->SectBefore.SectJ.bSymAutoCalc = bSymmetry;
		}

		SetViewData();
		SetColumnStatus();

		if (SaveData())
		{
//       UINT nIndex = m_pParent->GetBitmapIndexFromInfoComposite();
//       if(nIndex>=0) m_pParent->ChangeBitmapForViewer(nIndex);
			if(m_pPreview != 0)
			{
				m_pPreview->SetCentroidFlag(FALSE);
				m_pPreview->Invalidate();
			}
			if(m_nShapeIndex == 2)
			{
				m_pParent->DrawStlgViewDlgSection();
			}

			if((m_nShapeIndex == 0 || m_nShapeIndex == 1 || m_nShapeIndex == 2) &&
				nCol == 3)
			{
				// Stl Girder일 때 Symmetriy 체크로 값 바뀐거 갱신
				ShowData();
			}
		}
		return;
	}
	else if(nRow == m_nStiffenerRow2)
	{
		if(m_nShapeIndex == 0 || m_nShapeIndex == 1 || m_nShapeIndex == 2)
		{
			CCMSectSODDeckStiffenerDlg dlg;

			dlg.m_bTapJ = FALSE;

			dlg.m_Data = *m_pSectD;
			
			if(dlg.DoModal() == IDOK)
			{
				*m_pSectD = dlg.m_Data;

				if(m_pPreview != 0)
				{
					m_pPreview->SetCentroidFlag(FALSE);
					m_pPreview->Invalidate();
				}
				if(m_nShapeIndex == 2)
				{
					m_pParent->DrawStlgViewDlgSection();
				}
			}
		}
		else
		{
			CCMSectStiffenerDlg dlg;

			// 		if(nRow == m_nStiffenerRow_I) dlg.m_bTapJ = FALSE;
			// 		else                          dlg.m_bTapJ = TRUE;
			dlg.m_bTapJ = FALSE;
			dlg.m_Data = *m_pSectD;

			if(dlg.DoModal() == IDOK)
			{
				//m_pSectD = &dlg.m_Data;
				m_pSectD->SectBefore.SectI.Stiffener = dlg.m_Data.SectBefore.SectI.Stiffener;
			}

			if(m_pPreview != 0)
			{
				m_pPreview->SetCentroidFlag(FALSE);
				m_pPreview->Invalidate();
			}
			if(m_nShapeIndex == 2)
			{
				m_pParent->DrawStlgViewDlgSection();
			}
		}
		return;
	}
	else if (nRow == m_nRivetRow)
	{ 
		CCMSectRivetAngleDlg dlg;

		dlg.m_Data = *m_pSectD;

		if (dlg.DoModal() == IDOK)
		{
			*m_pSectD = dlg.m_Data;

			if (m_pPreview != 0)
			{
				m_pPreview->SetCentroidFlag(FALSE);
				m_pPreview->Invalidate();
			}
			if (m_nShapeIndex == 2)
			{
				m_pParent->DrawStlgViewDlgSection();
			}
		}

	}

	BOOL bImportBtn = TRUE;
	if (nCol != 2) bImportBtn = FALSE;

	int    nIorJ;   // (0) I  (1) J 
	BOOL   bIncludeOption[3] = {1, 1, 1};
	double *pSize;
	T_SECT_SECTBASE_D *pSect;
	CString csTitle;
	if (bImportBtn) csTitle = GetValueRowCol(nRow, 1);  
	if (csTitle == _LS(IDS_CMD_PSC_GRID_Size_I))
	{
		nIorJ = 0;
		bIncludeOption[0] = TRUE;
		bIncludeOption[1] = TRUE; // not used
		pSize = m_pSectD->SectBefore.SectI.Size;
		pSect = &m_pSectD->SectBefore.SectI;
	}
	else if (csTitle == _LS(IDS_CMD_PSC_GRID_Size_J))
	{
		nIorJ = 1;
		bIncludeOption[0] = FALSE;
		bIncludeOption[1] = TRUE; // not used
		pSize = m_pSectD->SectBefore.SectJ.Size;
		pSect = &m_pSectD->SectBefore.SectJ;
	}
	else bImportBtn = FALSE;

	if (!bImportBtn)
	{
		CGXGridWnd::OnClickedButtonRowCol(nRow, nCol);
		return;
	}
	// 임시
	SaveFlag();

	CCMSectItemImportStlg dlg;
	dlg.SetTargetShape(m_pSectD->SectBefore.Shape);
	dlg.m_bIncludeOption[0] = bIncludeOption[0];
	//dlg.m_bIncludeOption[1] = bIncludeOption[1];
	dlg.m_nFlangeShape = m_pSectD->SectBefore.nFlangeShape;
	dlg.m_nCellNum = m_pSectD->SectBefore.nCellNum;
	dlg.m_nCellType = m_pSectD->SectBefore.nCellType;

	if (dlg.DoModal() == IDOK)
	{
		T_SECT_K key = dlg.m_SectKey;
		T_SECT_D SectData;
		CDBDoc* pDoc = CDBDoc::GetDocPoint();
		pDoc->m_pAttrCtrl->GetSect(key, SectData);

		// copy size
		m_pSectD->SectBefore.bSymmetric = SectData.SectBefore.bSymmetric;
		
		memcpy(pSize, SectData.SectBefore.SectI.Size, sizeof(SectData.SectBefore.SectI.Size));
		
		if (dlg.m_bIncludeOption[0])   // Option1 : Joint, Type, Shape
		{
			//m_pSectD->SectBefore.SectI.BuiltUpFlag = SectData.SectBefore.SectI.BuiltUpFlag;
			//m_pSectD->SectBefore.nCellType = SectData.SectBefore.nCellType;
			//m_pSectD->SectBefore.nCellShape = SectData.SectBefore.nCellShape;
			m_pSectD->SectBefore.nJoint = SectData.SectBefore.nJoint;
		}
		
		if(m_nShapeIndex == 0 || m_nShapeIndex == 1)
		{
			// Symmetric, Top, Bot Gap값 불러오기
			if(nIorJ == 0)
			{
				m_pSectD->SectBefore.SectI.bSymAutoCalc = SectData.SectBefore.SectI.bSymAutoCalc;
				m_pSectD->SectAfter.SectI.Size[0]       = SectData.SectAfter.SectI.Size[0];
				m_pSectD->SectAfter.SectI.Size[1]       = SectData.SectAfter.SectI.Size[1];
			}
			else
			{
				m_pSectD->SectBefore.SectJ.bSymAutoCalc = SectData.SectBefore.SectI.bSymAutoCalc;
				m_pSectD->SectAfter.SectJ.Size[0]       = SectData.SectAfter.SectI.Size[0];
				m_pSectD->SectAfter.SectJ.Size[1]       = SectData.SectAfter.SectI.Size[1];
			}
		}
		else if(m_nShapeIndex == 2)
		{
			m_pSectD->SectBefore.bSymmetric      = SectData.SectBefore.bSymmetric;  
			m_pSectD->SectBefore.nCellNum        = SectData.SectBefore.nCellNum;    
			m_pSectD->SectBefore.nCellType       = SectData.SectBefore.nCellType;   
			m_pSectD->SectBefore.nFlangeShape    = SectData.SectBefore.nFlangeShape;
			m_pSectD->SectBefore.nJoint          = SectData.SectBefore.nJoint;      
		}

		ShowData();
	} 
}

BOOL CCMSectItemGrid4Stlg::GetIntValue(CString& csNum, int& nNum)
{
	CString csExpr;
	if (csNum != _T("") && csNum.GetAt(0) == '=') csExpr = csNum;
	else csExpr = _T("=") + csNum;
	if (!ConvertFormulaToValue(csExpr)) return FALSE;
	nNum = _ttoi(csExpr);
	return TRUE;
}

BOOL CCMSectItemGrid4Stlg::GetDoubleValue(CString& csNum, double& dNum)
{
	CString csExpr;
	if (csNum != _T("") && csNum.GetAt(0) == '=') csExpr = csNum;
	else csExpr = _T("=") + csNum;
	if (!ConvertFormulaToValue(csExpr)) return FALSE;
	dNum = _tstof(csExpr);
	return TRUE;
}

BOOL CCMSectItemGrid4Stlg::ConvertFormulaToValue(CString& value)
{
	if (GetSheetContext() == NULL && value.GetLength() > 0 && value.GetAt(0) == _T('='))
	{
		// Formula engine이 꺼져 있는데 수식이 입력되어 있으면
		CGXFormulaSheet fs;
		CGXFormula fm;
		if (!fs.ParseExpression(value, fm)) 
		{
			SetWarningText(_LS(IDS_CMD0417__Error___Invalid_formula_));
			return FALSE;
		}
		if (!fs.EvaluateExpression(value, fm))
		{
			SetWarningText(_LS(IDS_CMD0417__Error___Can_t_evaluate_formula_));
			return FALSE;
		}
	}
	return TRUE;
}

void CCMSectItemGrid4Stlg::SetCobxColumn(CGXRange& rRange, LPCTSTR lpszChoiceList)
{
	SetStyleRange(rRange, CGXStyle()
			.SetControl(GX_IDS_CTRL_TABBED_COMBOBOX)
			.SetChoiceList(lpszChoiceList)
			.SetUserAttribute(GX_IDS_UA_TABLIST_KEYCOL, _T("0"))
			.SetUserAttribute(GX_IDS_UA_TABLIST_TEXTCOL, _T("0"))
			.SetUserAttribute(GX_IDS_UA_TABLIST_SHOWALLCOLS, _T("1"))
			.SetValueType(GX_VT_STRING)
			.SetHorizontalAlignment(DT_LEFT)
			.SetValue(_T(""))
			);
}

// *^^* 여기 추가 
int CCMSectItemGrid4Stlg::GetSizeList(CStringArray& raSizeList)
{
#define _S(x) raSizeList.Add(#x)
	raSizeList.RemoveAll();
	
	if (m_nShapeIndex == 0) // Stl Girder B
	{
		_S(Symmetry); raSizeList.Add(_LS(IDS_CMD_SECT_SOD_TOP)); raSizeList.Add(_LS(IDS_CMD_SECT_SOD_BOTTOM)); _S(B1); _S(B2); _S(B3); _S(B4); _S(B5); _S(B6); _S(H); _S(t1); _S(t2); _S(tw1); _S(tw2);
	}
	else if (m_nShapeIndex == 1) // Stl Girder I
	{
		_S(Symmetry); raSizeList.Add(_LS(IDS_CMD_SECT_SOD_TOP)); raSizeList.Add(_LS(IDS_CMD_SECT_SOD_BOTTOM)); _S(B1); _S(B2); _S(B3); _S(B4); _S(H); _S(t1); _S(t2); _S(tw);
	}
	else if (m_nShapeIndex == 2) // Stl Girder MCell
	{
		if(m_pSectD->SectBefore.nFlangeShape == 0)
		{
			_S(B1);  _S(B2);  _S(B3);  _S(B4);  _S(B5);  _S(B6);  _S(Br1);  _S(Br2);  _S(Br3);  _S(Br4);  _S(Br5);  _S(H);  _S(tf1);  _S(tw1);  _S(tf2);  _S(tw2);  
		}
		else if(m_pSectD->SectBefore.nFlangeShape == 1)
		{
			_S(B1);  _S(B2);  _S(B3);  _S(B4);  _S(B5);  _S(B6);  _S(Br1);  _S(Br2);  _S(Br3);  _S(Br4);  _S(Br5);  _S(H);  _S(tf1);  _S(tw1);  _S(tf2);  _S(tw2);
			_S(rh);  _S(rt);  _S(rb);  _S(rhr); _S(rtr); _S(rbr);  
		}
		else if(m_pSectD->SectBefore.nFlangeShape == 2)
		{
			_S(B1);   _S(B2);  _S(B3);  _S(B4);  _S(B5);  _S(B6);  _S(Br1);  _S(Br2);   _S(Br3);  _S(Br4);  _S(Br5);  _S(H);    _S(tf1);  _S(tw1);  _S(tf2);  _S(tw2);
			_S(rh1);  _S(rh2); _S(rh3); _S(rb);  _S(rt1); _S(rt2); _S(rhr1); _S(rhr2);  _S(rhr3); _S(rbr);  _S(rtr1); _S(rtr2);   
		}
		else if(m_pSectD->SectBefore.nFlangeShape == 3)
		{
			_S(B1);  _S(B2);  _S(B3);  _S(B4);  _S(Br1);  _S(Br2);  _S(Br3);  _S(H);    _S(tf1);  _S(tw1);  _S(tf2);  _S(tw2);
			_S(jh1); _S(jb1); _S(jh2); _S(jb2); _S(jhr1); _S(jbr1); _S(jhr2); _S(jbr2);  
		}
		else ASSERT(0);    
	}

	return raSizeList.GetSize();
#undef _S
}

int CCMSectItemGrid4Stlg::GetUnitList(CStringArray& raUnitList)
{
	raUnitList.RemoveAll();
	
	CString csUnit;

	//if (m_nShapeIndex == 10) // value
	//{
	//  CUnitCtrl::GetUnitSystem(D_UNITSYS_BASE_LENGTH, csUnit); raUnitList.Add(csUnit);
	//  CUnitCtrl::GetUnitSystem(D_UNITSYS_BASE_LENGTH, csUnit); raUnitList.Add(csUnit);
	//  CUnitCtrl::GetUnitSystem(D_UNITSYS_BASE_LENGTH, csUnit); raUnitList.Add(csUnit);
	//  CUnitCtrl::GetUnitSystem(D_UNITSYS_BASE_LENGTH, csUnit); raUnitList.Add(csUnit);
	//  CUnitCtrl::GetUnitSystem(D_UNITSYS_BASE_AREA  , csUnit); raUnitList.Add(csUnit);
	//  CUnitCtrl::GetUnitSystem(D_UNITSYS_BASE_AREA  , csUnit); raUnitList.Add(csUnit);
	//  CUnitCtrl::GetUnitSystem(D_UNITSYS_BASE_AREA  , csUnit); raUnitList.Add(csUnit);
	//  CUnitCtrl::GetUnitSystem(D_UNITSYS_BASE_STIF  , csUnit); raUnitList.Add(csUnit);
	//  CUnitCtrl::GetUnitSystem(D_UNITSYS_BASE_STIF  , csUnit); raUnitList.Add(csUnit);
	//  CUnitCtrl::GetUnitSystem(D_UNITSYS_BASE_STIF  , csUnit); raUnitList.Add(csUnit);
	//  CUnitCtrl::GetUnitSystem(D_UNITSYS_BASE_LENGTH, csUnit); raUnitList.Add(csUnit);
	//  CUnitCtrl::GetUnitSystem(D_UNITSYS_BASE_LENGTH, csUnit); raUnitList.Add(csUnit);
	//  CUnitCtrl::GetUnitSystem(D_UNITSYS_BASE_LENGTH, csUnit); raUnitList.Add(csUnit);
	//  CUnitCtrl::GetUnitSystem(D_UNITSYS_BASE_LENGTH, csUnit); raUnitList.Add(csUnit);
	//  CUnitCtrl::GetUnitSystem(D_UNITSYS_BASE_AREA  , csUnit); raUnitList.Add(csUnit);
	//  CUnitCtrl::GetUnitSystem(D_UNITSYS_BASE_AREA  , csUnit); raUnitList.Add(csUnit);
	//  CUnitCtrl::GetUnitSystem(D_UNITSYS_BASE_LENGTH, csUnit); raUnitList.Add(csUnit);
	//  CUnitCtrl::GetUnitSystem(D_UNITSYS_BASE_LENGTH, csUnit); raUnitList.Add(csUnit);
	//  CUnitCtrl::GetUnitSystem(D_UNITSYS_BASE_LENGTH, csUnit); raUnitList.Add(csUnit);
	//  CUnitCtrl::GetUnitSystem(D_UNITSYS_BASE_LENGTH, csUnit); raUnitList.Add(csUnit);
	//  CUnitCtrl::GetUnitSystem(D_UNITSYS_BASE_LENGTH, csUnit); raUnitList.Add(csUnit);
	//  CUnitCtrl::GetUnitSystem(D_UNITSYS_BASE_LENGTH, csUnit); raUnitList.Add(csUnit);
	//  CUnitCtrl::GetUnitSystem(D_UNITSYS_BASE_LENGTH, csUnit); raUnitList.Add(csUnit);
	//  CUnitCtrl::GetUnitSystem(D_UNITSYS_BASE_LENGTH, csUnit); raUnitList.Add(csUnit);
	//  CUnitCtrl::GetUnitSystem(D_UNITSYS_BASE_LENGTH, csUnit); raUnitList.Add(csUnit);
	//  CUnitCtrl::GetUnitSystem(D_UNITSYS_BASE_LENGTH, csUnit); raUnitList.Add(csUnit);
	//  CUnitCtrl::GetUnitSystem(D_UNITSYS_BASE_LENGTH, csUnit); raUnitList.Add(csUnit);
	//  CUnitCtrl::GetUnitSystem(D_UNITSYS_BASE_LENGTH, csUnit); raUnitList.Add(csUnit);
	//}
	return raUnitList.GetSize();
}

void CCMSectItemGrid4Stlg::SaveSizeStlGirderBoxI(int nSizeRow, int n, double* pSize )
{
	int nNum;
	double dNum;
	BOOL bSymmetricSecAutoCalc;
	CString csNum;
	// Save Size
	for (int i = 0; i < m_nNumSize; i++)
	{
		if(i == 0) // Symmetric
		{
			csNum = GetEditingValue(nSizeRow+i, 3);      
			if (GetIntValue(csNum, nNum))  
			{
				bSymmetricSecAutoCalc = nNum == 1 ? TRUE : FALSE;
			}              
			else
			{
				bSymmetricSecAutoCalc = FALSE;
			}

			if(n==0)
			{
				m_pSectD->SectBefore.SectI.bSymAutoCalc = bSymmetricSecAutoCalc;
			}
			else
			{
				m_pSectD->SectBefore.SectJ.bSymAutoCalc = bSymmetricSecAutoCalc;
			}
		}
		else if(1 <= i && i <= 2) // Top,Bot Gap
		{
			double& dGapVal = n==0 ? m_pSectD->SectAfter.SectI.Size[i - 1] : m_pSectD->SectAfter.SectJ.Size[i - 1];
			csNum = GetEditingValue(nSizeRow+i, 2);
			if (GetDoubleValue(csNum, dNum))
			{
				dGapVal = dNum;
			}
			else
			{
				dGapVal = 0.0;
			}
		}
		else // Size
		{
			csNum = GetEditingValue(nSizeRow+i, 2);
			if (GetDoubleValue(csNum, dNum))  
				pSize[i-3] = dNum;
			else
				pSize[i-3] = 0.0;
		}
	}
	
	// Symmetric일때 AutoCalc
	if(bSymmetricSecAutoCalc)
	{
		StlGSectSyAutoCalc(pSize, n == 1);
		if(m_pPreview != 0)
		{
			m_pPreview->SetCentroidFlag(FALSE);
			m_pPreview->Invalidate();
		}
	}
}

void CCMSectItemGrid4Stlg::SaveSizeStlGirderMCell(int nSizeRow, int n, double* pSize )
{
	double dNum;
	BOOL bSymmetricSecAutoCalc;
	bSymmetricSecAutoCalc = m_pSectD->SectBefore.bSymmetric;
	CString csNum;
	// Save Size
	for (int i = 0; i < m_nNumSize; i++)
	{
		csNum = GetEditingValue(nSizeRow+i, 2);
		if (GetDoubleValue(csNum, dNum))  
			pSize[i] = dNum;
		else
			pSize[i] = 0.0;
	}

	// Symmetric일때 AutoCalc
	if(bSymmetricSecAutoCalc)
	{
		StlGSectSyAutoCalc(pSize, n == 1);
		m_pParent->DrawStlgViewDlgSection();
	}
}

void CCMSectItemGrid4Stlg::StlGSectSyAutoCalc(double* pSize, BOOL bJ)
{
	int nStype;  
	
	if(m_nShapeIndex == 0)
	{
		nStype = D_SECT_TYPE_STLG_B;
	}
	else if(m_nShapeIndex == 1)
	{
		nStype = D_SECT_TYPE_STLG_I;
	}
	else if(m_nShapeIndex == 2)
	{
		nStype = D_SECT_TYPE_STLG_MCELL;
	}
	else
	{
		ASSERT(0);
		return;
	}
		
	CSectUtil::StlGSymmetricSectAutoCalc(*m_pSectD, bJ);
}

void CCMSectItemGrid4Stlg::ShowDataStlGirderBoxI( int n, int nRow, CStringArray& aSizeList, int nPlace, CString csUnit, double * pSize )
{
	BOOL bSymmetricSecAutoCalc;
	bSymmetricSecAutoCalc = n==0 ? m_pSectD->SectBefore.SectI.bSymAutoCalc : m_pSectD->SectBefore.SectJ.bSymAutoCalc;

	for (int i = 0; i < m_nNumSize; i++)
	{
		if(i == 0) // Symmetric
		{
			SetCoveredCellsRowCol(nRow+i, 1, nRow+i, 2);
			SetValueRange(CGXRange(nRow+i, 1), _LS(IDS_CMD_PSC_GRID_Symmetry));
			SetStyleRange(CGXRange(nRow+i, 3), CGXStyle()
				.SetEnabled(TRUE)
				.SetControl(GX_IDS_CTRL_CHECKBOX3D).SetHorizontalAlignment(DT_CENTER));
			SetValueRange(CGXRange(nRow+i, 3), (bSymmetricSecAutoCalc?_T("1"):_T("0")));

			if (n == 0)
			{
				m_nSymmetryStlGSectI = nRow+i;
				m_bSymmetryStlGSectI = bSymmetricSecAutoCalc;
			}
			else
			{
				m_nSymmetryStlGSectJ = nRow+i;
				m_bSymmetryStlGSectJ = bSymmetricSecAutoCalc;
			}
		}
		else
		{
			SetValueRange(CGXRange(nRow+i, 1), aSizeList[i]);
			SetStyleRange(CGXRange(nRow+i, 2), CGXStyle()
				.SetHorizontalAlignment(DT_RIGHT)
				.SetFormat(GX_FMT_FIXED)
				.SetPlaces(nPlace)            
				.SetEnabled(FALSE));
			SetStyleRange(CGXRange(nRow+i, 3), CGXStyle().SetValue(csUnit));

			if(1 <= i && i <= 2) // Top,Bot Gap
			{
				SetStyleRange(CGXRange(nRow+i, 2), CGXStyle().SetValue(n==0 ? m_pSectD->SectAfter.SectI.Size[i - 1] : m_pSectD->SectAfter.SectJ.Size[i - 1]));
			}
			else // Size
			{
				SetStyleRange(CGXRange(nRow+i, 2), CGXStyle().SetValue(pSize[i-3]));
			}   
		}      
	}
}
