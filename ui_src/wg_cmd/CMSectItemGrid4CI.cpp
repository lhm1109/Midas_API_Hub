// CMSectItemGrid4PscNew.cpp : implementation file
//
// 2002. 10. 28   by TAE
///////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "wg_cmd.h"
#include "CMSectItemGrid4CI.h"
#include "CMSecViewWnd.h"
#include "CMSectItemImportPSC.h"

#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCMSectItemGrid4CI
CCMSectItemGrid4CI::CCMSectItemGrid4CI()
{
	m_bInit = FALSE;
	m_pPreview = 0;
	m_pSectD = 0;
	m_nShapeIndex = -1;
	m_bSymmetry = FALSE;
	m_bOnStartEditing = FALSE;
}

CCMSectItemGrid4CI::~CCMSectItemGrid4CI()
{
}

//---------------------------------------------------------------------------
// Public Interface Functions
void CCMSectItemGrid4CI::InitGrid(CCMSecViewWnd* pPreview)
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
	HideRows(0, 0);
	HideCols(0, 0);

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

void CCMSectItemGrid4CI::SetDataSource(T_SECT_D* pSectD)
{
	m_bSymmetry = 0;
	m_pSectD = pSectD;

	if (m_pSectD->nStype == D_SECT_TYPE_COMPO_CI)
		m_nShapeIndex = 3;
	else if (m_pSectD->nStype == D_SECT_TYPE_COMPO_CT)
		m_nShapeIndex = 4;
	else ASSERT(0);
	SetViewData();
	ShowData();
	SetRightValidCell(1, 1);
}

void CCMSectItemGrid4CI::ChangeShape(int nNewShapeIndex)
{
	CancelEditing();

	m_bSymmetry = 0;
	m_nShapeIndex = nNewShapeIndex;
	SetRightValidCell(1, 1);

	/* *^^*
	if(nNewShapeIndex == 3) // mid
	{
		m_pSectD->SectBefore.nCellType = 2;
		m_pSectD->SectBefore.nCellShape = 2;
	}
	*/
	ShowData();
}

void CCMSectItemGrid4CI::ShowData()
{
	BOOL bOldLock = LockUpdate(TRUE);
	BOOL bOldReadOnlyLock = GetParam()->IsLockReadOnly();
	GetParam()->SetLockReadOnly(FALSE);

	SetColCount(3);
	// set column width
	SetColWidth(1, 1, globalUtils.ScaleByDPI(60));
	SetColWidth(2, 2, globalUtils.ScaleByDPI(100));
	SetColWidth(3, 3, globalUtils.ScaleByDPI(60));

	int nRowCount = GetRowCount();
	if (nRowCount > 0) RemoveRows(1, nRowCount);
	SetViewData();

	m_nSymmetryRow = 0;
	m_nCellTypeRow = 0;
	m_nCellShapeRow = 0;
	m_nJointRow = 0;
	m_nSizeIRow = 0;

	int nRow = 0;
	if (m_nNumSymmetry > 0)
	{
		nRow++;
		m_nSymmetryRow = nRow;
		InsertRows(nRow, m_nNumSymmetry);
		SetCoveredCellsRowCol(nRow, 1, nRow, 2);
		SetValueRange(CGXRange(nRow, 1), _LS(IDS_CMD_PSC_GRID_Symmetry));
		SetStyleRange(CGXRange(nRow, 3), CGXStyle()
			.SetControl(GX_IDS_CTRL_CHECKBOX3D).SetHorizontalAlignment(DT_CENTER));
		SetValueRange(CGXRange(nRow, 3), (m_bSymmetry?_T("1"):_T("0")));
		//HideRows(nRow, nRow);
	}
	if (m_nNumCellType > 0)
	{
		nRow++;
		m_nCellTypeRow = nRow;
		InsertRows(nRow, m_nNumCellType);
		SetValueRange(CGXRange(nRow, 1), m_csNameCellType);
		SetCoveredCellsRowCol(nRow, 2, nRow, 3);
		SetStyleRange(CGXRange(nRow, 2), CGXStyle()
			.SetValue(m_csValueCellType)
			.SetReadOnly(TRUE));
	}
	if (m_nNumCellShape > 0)
	{
		nRow++;
		m_nCellShapeRow = nRow;
		InsertRows(nRow, m_nNumCellShape);
		SetValueRange(CGXRange(nRow, 1), m_csNameCellShape);
		SetCoveredCellsRowCol(nRow, 2, nRow, 3);
		SetStyleRange(CGXRange(m_nCellShapeRow, 2), CGXStyle()
			.SetValue(m_csValueCellShape)
			.SetReadOnly(TRUE));
	}
	if (m_nNumJoint > 0)
	{
		nRow++;
		InsertRows(nRow, 1);
		SetCoveredCellsRowCol(nRow, 1, nRow, 3);
		SetStyleRange(CGXRange(nRow, 1), CGXStyle()
			.SetInterior(COLORREF(RGB(0, 0, 255)))
			.SetTextColor(COLORREF(RGB(255, 255, 0)))
			.SetValue(_LS(IDS_CMD_PSC_GRID_Joint)));
		nRow++;
		m_nJointRow = nRow;
		InsertRows(nRow, m_nNumJoint);
		CStringArray aJointList;
		GetJointList(aJointList);
		int nPos = 0x01;
		for (int i = 0; i < m_nNumJoint; i++)
		{
			SetValueRange(CGXRange(nRow+i, 1), aJointList[i]);
			SetStyleRange(CGXRange(nRow+i, 2), CGXStyle()
				.SetControl(GX_IDS_CTRL_CHECKBOX3D)
				.SetHorizontalAlignment(DT_CENTER)
				.SetValue(m_nJoint&nPos?_T("1"):_T("0")));
			nPos <<= 1;
		}
		nRow += m_nNumJoint-1;
	}
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

		CString aTitle[] = {_LS(IDS_CMD_PSC_GRID_Size_I), _LS(IDS_CMD_PSC_GRID_Size_J)};
		for (int n = 0; n < 1; n++) // 여기서는 I단만 
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

			nRow++;
			if (n == 0) m_nSizeIRow = nRow;
			InsertRows(nRow, m_nNumSize);

			double *pSize;
			if (n == 0) pSize = m_pSectD->SectBefore.SectI.Size;
			else pSize = m_pSectD->SectBefore.SectJ.Size;
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
			nRow += m_nNumSize-1;
		}
	}
	SetStyleRange(CGXRange().SetTable(), CGXStyle().SetVerticalAlignment(DT_VCENTER));
	SetStyleRange(CGXRange().SetCols(1), CGXStyle().SetEnabled(FALSE));
	SetStyleRange(CGXRange().SetCols(3), CGXStyle().SetEnabled(FALSE));
	if(m_nSymmetryRow > 0) SetStyleRange(CGXRange(m_nSymmetryRow, 3), CGXStyle().SetEnabled(TRUE));

	SetColumnStatus();

	GetParam()->SetLockReadOnly(bOldReadOnlyLock);
	LockUpdate(bOldLock);
	if (!bOldLock) Redraw();

	if (m_pPreview) 
	{
		m_pPreview->SetCentroidFlag(FALSE);
		m_pPreview->Invalidate();
	}
}

void CCMSectItemGrid4CI::SetViewData()
{
	m_nNumSymmetry = 0;
	m_nNumCellType = 0; m_csNameCellType = _T(""); //m_nKindCellType = 0;
	m_nNumCellShape = 0; m_csNameCellShape = _T(""); //m_nKindCellShape = 0;
	m_nNumJoint = 0;
	m_nNumSize = 0;
	m_nJointType = 0; // In/Out, Left/Right
	m_nJointNum0 = 0; // Single(J1)
	m_nJointNum1 = 0; // Out or Left
	m_nJointNum2 = 0; // In or Right
	m_nSizeNum0 = 0;
	m_nSizeNum1 = 0;
	m_nSizeNum2 = 0;

	switch(m_nShapeIndex)
	{
	case 3: // CI
		m_nNumSymmetry = 1;
		//m_nNumCellType = 1; m_csNameCellType = _LS(IDS_CMD_PSC_GRID_Left); m_csValueCellType = _LS(IDS_CMD_PSC_GRID_Polygon);
		//m_nNumCellShape = 1; m_csNameCellShape = _LS(IDS_CMD_PSC_GRID_Right); m_csValueCellShape = _LS(IDS_CMD_PSC_GRID_Polygon);
		m_nNumJoint = 9;
		m_nNumSize = 33;
		m_nSizeNum0 = 1;
		m_nSizeNum1 = 16;
		m_nSizeNum2 = 16;
		m_nJointType = 2; // Left/Right
		m_nJointNum0 = 1;
		m_nJointNum1 = 4;
		m_nJointNum2 = 4;
		m_nJoint = m_pSectD->SectBefore.nJoint;
		break;
	case 4: // CT
		m_nNumSymmetry = 1;
		m_nNumJoint = 9;
		m_nNumSize = 31;
		m_nSizeNum0 = 1;
		m_nSizeNum1 = 15;
		m_nSizeNum2 = 15;
		m_nJointType = 2; // Left/Right
		m_nJointNum0 = 1;
		m_nJointNum1 = 4;
		m_nJointNum2 = 4;
		m_nJoint = m_pSectD->SectBefore.nJoint;
		break;
	}
}

BOOL CCMSectItemGrid4CI::SaveData()
{
	BOOL bSuccess = SaveFlag();
	bSuccess = bSuccess && SaveSize();
	return bSuccess;
}

BOOL CCMSectItemGrid4CI::SaveFlag(BOOL bExcludeJoint/*=FALSE*/)
{
	CString csNum;
	int nNum;

	// *^^*
	/*
	int nCellTypeShape = (m_nShapeIndex == 3) ? 2 : 0 ;
	m_pSectD->SectBefore.nCellType = nCellTypeShape;
	m_pSectD->SectBefore.nCellShape = nCellTypeShape;
	*/

	if(bExcludeJoint) return TRUE;

	int nJoint = 0;
	if (m_nJointRow > 0)
	{
		int nJointFlag = 0x01;
		if(m_nSymmetryRow && m_bSymmetry)  // 대칭 
		{
			int nRow;
			for (int i = 0; i < m_nNumJoint; i++)
			{
				if(i < m_nJointNum0) nRow = m_nJointRow+i;
				else nRow = m_nJointRow+m_nJointNum0+((i-m_nJointNum0)%m_nJointNum1);
				csNum = GetEditingValue(nRow, 2);
				nNum = _ttoi(csNum);
				if (nNum != 0) nJoint |= nJointFlag;
				nJointFlag <<= 1;
			}
		}
		else
		{
			for (int i = 0; i < m_nNumJoint; i++)
			{
				csNum = GetEditingValue(m_nJointRow+i, 2);
				nNum = _ttoi(csNum);
				if (nNum != 0) nJoint |= nJointFlag;
				nJointFlag <<= 1;
			}
		}
	}
	m_pSectD->SectBefore.nJoint = nJoint;

	return TRUE;
}

BOOL CCMSectItemGrid4CI::SaveSize()
{
	CString csNum;
	double dNum;

	int nSizeRow;
	double* pSize;
	for (int n = 0; n < 1; n++)
	{
		if (n == 0) 
		{
			nSizeRow = m_nSizeIRow;
			pSize = m_pSectD->SectBefore.SectI.Size;
		}
		if(m_nSymmetryRow && m_bSymmetry)  // 대칭 
		{
			int nRow;
			for (int i = 0; i < m_nNumSize; i++)
			{
				if(i < m_nSizeNum0) nRow = nSizeRow+i;
				else nRow = nSizeRow+m_nSizeNum0+((i-m_nSizeNum0)%m_nSizeNum1);
				csNum = GetEditingValue(nRow, 2);
				if (GetDoubleValue(csNum, dNum)) 
					pSize[i] = dNum;
				else pSize[i] = 0.0;
			}
		}
		else
		{
			for (int i = 0; i < m_nNumSize; i++)
			{
				csNum = GetEditingValue(nSizeRow+i, 2);
				if (GetDoubleValue(csNum, dNum)) 
					pSize[i] = dNum;
				else pSize[i] = 0.0;
			}
		}
	}
	return TRUE;
}

//---------------------------------------------------------------------------
// Implementation Function
BOOL CCMSectItemGrid4CI::SetLeftValidCell(ROWCOL nRow, ROWCOL nCol)
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

BOOL CCMSectItemGrid4CI::SetRightValidCell(ROWCOL nRow, ROWCOL nCol)
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

CString CCMSectItemGrid4CI::GetEditingValue(ROWCOL nRow, ROWCOL nCol)
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
void CCMSectItemGrid4CI::SetColumnStatus()
{
	CArray<BOOL, BOOL> aHideJoint;  // TRUE->Hide
	CArray<BOOL, BOOL> aHideSize;   // TRUE->Hide
	SetColumnStatusGetStatus(aHideJoint, aHideSize);

	BOOL bOldLock = LockUpdate(TRUE);
	if(m_nCellShapeRow > 0)
		HideRows(m_nCellShapeRow, m_nCellShapeRow, (m_nSymmetryRow && m_bSymmetry && m_nShapeIndex==3));
	for (int i = 0; i < aHideJoint.GetSize(); i++)
		SetColumnStatusShowHide(m_nJointRow+i, aHideJoint[i]);
	for (int i = 0; i < aHideSize.GetSize(); i++)
	{
		SetColumnStatusShowHide(m_nSizeIRow+i, aHideSize[i]);
		//SetColumnStatusShowHide(m_nSizeJRow+i, aHideSize[i]);
	}
	LockUpdate(bOldLock);
	if (!bOldLock) Redraw();
}

void CCMSectItemGrid4CI::SetColumnStatusGetStatus(CArray<BOOL, BOOL>& raHideJoint, CArray<BOOL, BOOL>& raHideSize)
{
	raHideJoint.RemoveAll();
	raHideSize.RemoveAll();

	CArray<int, int> aType4Joint, aType4Size; // 1:1개(Begin만), 2:2개(Begin, End), 3:n개(Begin부터End까지)
	CArray<int, int> aBegin4Joint, aBegin4Size;
	CArray<int, int> aEnd4Joint, aEnd4Size;

	CArray<BOOL, BOOL> aJF; // Joint Flag
	
	int nPos = 0x01;
	aJF.SetSize(m_nNumJoint);
	for (int i = 0; i < m_nNumJoint; i++)
	{  
		aJF[i] = (m_nJoint & nPos) != 0;
		nPos <<= 1;
	}

	switch(m_nShapeIndex)
	{
	case 3: // I  
		if(m_nSymmetryRow && m_bSymmetry)  // 대칭이면 
		{
			aBegin4Joint.Add(5); aEnd4Joint.Add(8); aType4Joint.Add(3);
			aBegin4Size.Add(17); aEnd4Size.Add(32); aType4Size.Add(3);
		}
		if (!aJF[0]) { aBegin4Size.Add(0); aEnd4Size.Add(0); aType4Size.Add(1); }
		// *^^*
		//if(m_pSectD->SectBefore.nCellType == 2)
		//{
			if (!aJF[1]) { aBegin4Size.Add(3); aEnd4Size.Add(12); aType4Size.Add(2); }
			if (!aJF[2]) { aBegin4Size.Add(4); aEnd4Size.Add(13); aType4Size.Add(2); }
			if (!aJF[3]) { aBegin4Size.Add(8); aEnd4Size.Add(16); aType4Size.Add(2); }
			if (!aJF[4]) { aBegin4Size.Add(7); aEnd4Size.Add(15); aType4Size.Add(2); }
		//}
		//else ASSERT(0);
		//if(m_pSectD->SectBefore.nCellShape == 2)
		//{
			if (!aJF[5]) { aBegin4Size.Add(19); aEnd4Size.Add(28); aType4Size.Add(2); }
			if (!aJF[6]) { aBegin4Size.Add(20); aEnd4Size.Add(29); aType4Size.Add(2); }
			if (!aJF[7]) { aBegin4Size.Add(24); aEnd4Size.Add(32); aType4Size.Add(2); }
			if (!aJF[8]) { aBegin4Size.Add(23); aEnd4Size.Add(31); aType4Size.Add(2); }
		//}
		//else ASSERT(0);
		break;
	case 4: // tee
		if(m_nSymmetryRow && m_bSymmetry)  // 대칭이면 
		{
			aBegin4Joint.Add(5); aEnd4Joint.Add(8); aType4Joint.Add(3);
			aBegin4Size.Add(16); aEnd4Size.Add(30); aType4Size.Add(3);
		}
		if (!aJF[0]) { aBegin4Size.Add(0); aEnd4Size.Add(0); aType4Size.Add(1); }
		if (!aJF[1]) { aBegin4Size.Add(8); aEnd4Size.Add(14); aType4Size.Add(2); }
		if (!aJF[2]) { aBegin4Size.Add(9); aEnd4Size.Add(15); aType4Size.Add(2); }
		if (!aJF[3]) { aBegin4Size.Add(10); aEnd4Size.Add(12); aType4Size.Add(2); }
		if (!aJF[4]) { aBegin4Size.Add(11); aEnd4Size.Add(13); aType4Size.Add(2); }
		if (!aJF[5]) { aBegin4Size.Add(23); aEnd4Size.Add(29); aType4Size.Add(2); }
		if (!aJF[6]) { aBegin4Size.Add(24); aEnd4Size.Add(30); aType4Size.Add(2); }
		if (!aJF[7]) { aBegin4Size.Add(25); aEnd4Size.Add(27); aType4Size.Add(2); }
		if (!aJF[8]) { aBegin4Size.Add(26); aEnd4Size.Add(28); aType4Size.Add(2); }
		break;
	}

	SetColumnStatusSetHideFlag(raHideJoint, m_nNumJoint, aType4Joint, aBegin4Joint, aEnd4Joint);
	SetColumnStatusSetHideFlag(raHideSize, m_nNumSize, aType4Size, aBegin4Size, aEnd4Size);
}

void CCMSectItemGrid4CI::SetColumnStatusSetHideFlag(CArray<BOOL, BOOL>& raHideFlag, 
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

void CCMSectItemGrid4CI::SetColumnStatusShowHide(ROWCOL nRow, BOOL bHide)
{
	SetStyleRange(CGXRange(nRow, 2), CGXStyle().SetEnabled(!bHide));
	HideRows(nRow, nRow, bHide);
}

BOOL CCMSectItemGrid4CI::IsEditing()
{
	return GetEditingControl() != 0;
}

void CCMSectItemGrid4CI::CancelEditing()
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

CGXControl* CCMSectItemGrid4CI::GetEditingControl()
{
	ROWCOL ncRow, ncCol;
	if (GetCurrentCell(ncRow, ncCol))
	{
		CGXControl* pControl = GetControl(ncRow, ncCol);
		if (pControl && pControl->IsActive()) return pControl;
	}
	return 0;
}

BEGIN_MESSAGE_MAP(CCMSectItemGrid4CI, CGXGridWnd)
	//{{AFX_MSG_MAP(CCMSectItemGrid4CI)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CCMSectItemGrid4CI message handlers
/////////////////////////////////////////////////////////////////////////////
BOOL CCMSectItemGrid4CI::ProcessKeys(CWnd *pSender, UINT nMessage, UINT nChar, UINT nRepCnt, UINT flags)
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

BOOL CCMSectItemGrid4CI::OnValidateCell(ROWCOL nRow, ROWCOL nCol)
{
	if(nRow >= m_nSizeIRow && nRow < m_nSizeIRow+m_nNumSize)
	{
		CString csNum = GetEditingValue(nRow, nCol);
		double dNum;
		if (GetDoubleValue(csNum, dNum)) return TRUE;
		m_bValidateFail = TRUE;
		return FALSE;
	}
	return TRUE;
}

BOOL CCMSectItemGrid4CI::OnActivateGrid(BOOL bActivate)
{
	return CGXGridWnd::OnActivateGrid(bActivate);
}

void CCMSectItemGrid4CI::OnModifyCell(ROWCOL nRow, ROWCOL nCol)
{
	CGXGridWnd::OnModifyCell(nRow, nCol);

	SaveData();

	if (nRow == m_nCellTypeRow || nRow == m_nCellShapeRow)
	{
		SetViewData();
		SetColumnStatus();
	}
	if (m_pPreview != 0) 
	{
		m_pPreview->SetCentroidFlag(FALSE);
		m_pPreview->Invalidate();
	}
}

BOOL CCMSectItemGrid4CI::OnStartEditing(ROWCOL nRow, ROWCOL nCol)
{
	if (!CGXGridWnd::OnStartEditing(nRow, nCol)) return FALSE;
	// Joint Flag 변경되면 Data 저장하고 Show/Hide 다시 하고 그림 다시 그린다.
	if (nCol == 2 && nRow >= m_nJointRow && nRow < m_nJointRow + m_nNumJoint)
	{
		// SaveData에서 Current Cell이 CheckBox이면 값을 Reverse해서 가져오기
		m_bOnStartEditing = TRUE; 
		SaveData();
		m_bOnStartEditing = FALSE;
		SetViewData();
		SetColumnStatus();
		if (m_pPreview != 0)
		{
			m_pPreview->SetCentroidFlag(FALSE);
			m_pPreview->Invalidate();
		}
	}
	return TRUE;
}

BOOL CCMSectItemGrid4CI::OnPasteFromClipboard(const CGXRange &range)
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

void CCMSectItemGrid4CI::OnClickedButtonRowCol(ROWCOL nRow, ROWCOL nCol)
{
	if(nRow == m_nSymmetryRow)
	{
		CString csSymmetry = GetEditingValue(m_nSymmetryRow, 3);
		m_bSymmetry = (csSymmetry == _T("0")) ? FALSE : TRUE;
		
		SetViewData();
		SetColumnStatus();

		if(SaveData())
		{
//      UINT nIndex = m_pParent->GetBitmapIndexFromInfoPSC();
//      if(nIndex>=0) m_pParent->ChangeBitmapForViewer(nIndex);
			if(m_pPreview != 0) 
			{
				m_pPreview->SetCentroidFlag(FALSE);
				m_pPreview->Invalidate();
			}
		}
		return;
	}

	BOOL bImportBtn = TRUE;
	if (nCol != 2) bImportBtn = FALSE;

	BOOL   bIncludeOption[2];
	double *pSize;
	CString csTitle;
	if (bImportBtn) csTitle = GetValueRowCol(nRow, 1);
	if (csTitle == _LS(IDS_CMD_PSC_GRID_Size_I))
	{
		bIncludeOption[0] = TRUE;
		bIncludeOption[1] = FALSE;
		pSize = m_pSectD->SectBefore.SectI.Size;
	}
	else bImportBtn = FALSE;

	if (!bImportBtn)
	{
		CGXGridWnd::OnClickedButtonRowCol(nRow, nCol);
		return;
	}

	// 임시
	SaveFlag();

	CCMSectItemImportPSC dlg;
	if(m_nShapeIndex == 3) 
	{
		// *^^*
		dlg.SetTargetShape(CString(D_SECT_SHAPE_PSC_I));
		//dlg.SetTargetCellTypeShape(2, 2);
	}
	else if(m_nShapeIndex == 4)
		dlg.SetTargetShape(CString(D_SECT_SHAPE_PSC_T));
	
	dlg.m_bIncludeOption[0] = bIncludeOption[0];
	dlg.m_bIncludeOption[1] = bIncludeOption[1];
	if(dlg.DoModal() == IDOK)
	{
		T_SECT_K key = dlg.m_SectKey;
		T_SECT_D SectData;
		CDBDoc* pDoc = CDBDoc::GetDocPoint();
		pDoc->m_pAttrCtrl->GetSect(key, SectData);
		memcpy(pSize, SectData.SectBefore.SectI.Size, sizeof(SectData.SectBefore.SectI.Size));
		if (dlg.m_bIncludeOption[0])    // option1 : 일부만 반영 
		{
			m_pSectD->SectBefore.SectI.BuiltUpFlag = SectData.SectBefore.SectI.BuiltUpFlag;
			//m_pSectD->SectBefore.nCellType = SectData.SectBefore.nCellType;
			//m_pSectD->SectBefore.nCellShape = SectData.SectBefore.nCellShape;
			m_pSectD->SectBefore.nJoint = SectData.SectBefore.nJoint;
		}
		if (dlg.m_bIncludeOption[1])    // option2 
		{
			if (SectData.SectBefore.bShearCheck == TRUE)
			{
				m_pSectD->SectBefore.bShearCheck = SectData.SectBefore.bShearCheck;
				for (int i = 0; i < 2; i++)
				{
					m_pSectD->SectBefore.bAutoWebThk[i] = SectData.SectBefore.bAutoWebThk[i];
					m_pSectD->SectBefore.dWebThk[i]     = SectData.SectBefore.dWebThk[i];

					for (int j = 0; j < 3; j++)
					{
						m_pSectD->SectBefore.bAutoShearCheckPos[i][j] = SectData.SectBefore.bAutoShearCheckPos[i][j];
						m_pSectD->SectBefore.dShearCheckPos[i][j]     = SectData.SectBefore.dShearCheckPos[i][j];

						m_pSectD->SectBefore.bAutoQy[i][j]            = SectData.SectBefore.bAutoQy[i][j];
						m_pSectD->SectBefore.dQy[i][j]                = SectData.SectBefore.dQy[i][j];

						m_pSectD->SectBefore.bAutoWebThkShear[i][j]   = SectData.SectBefore.bAutoWebThkShear[i][j];
						m_pSectD->SectBefore.dWebThkShear[i][j]       = SectData.SectBefore.dWebThkShear[i][j];						
					}
				}
			}

		}
		ShowData();
	} 
}

BOOL CCMSectItemGrid4CI::GetDoubleValue(CString& csNum, double& dNum)
{
	CString csExpr;
	if (csNum != _T("") && csNum.GetAt(0) == '=') csExpr = csNum;
	else csExpr = _T("=") + csNum;
	if (!ConvertFormulaToValue(csExpr)) return FALSE;
	dNum = _tstof(csExpr);
	return TRUE;
}

BOOL CCMSectItemGrid4CI::ConvertFormulaToValue(CString& value)
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

int CCMSectItemGrid4CI::GetJointList(CStringArray& raJointList)
{
	raJointList.RemoveAll();
	CString csJoint;
	if (m_nJointType == 1)
	{
		for (int i = 0; i < m_nJointNum1; i++)
		{
			csJoint.Format(_T("JO%d"), i+1);
			raJointList.Add(csJoint);
		}
		for (int j = 0; j < m_nJointNum2; j++)
		{
			csJoint.Format(_T("JI%d"), j+1);
			raJointList.Add(csJoint);
		}
	}
	else if (m_nJointType == 2)
	{
		for (int i = 0; i < m_nJointNum0; i++)
		{
			csJoint.Format(_T("J%d"), i+1);
			raJointList.Add(csJoint);
		}
		for (int j = 0; j < m_nJointNum1; j++)
		{
			csJoint.Format(_T("JL%d"), j+1);
			raJointList.Add(csJoint);
		}
		for (int k = 0; k < m_nJointNum2; k++)
		{
			csJoint.Format(_T("JR%d"), k+1);
			raJointList.Add(csJoint);
		}
	}
	return raJointList.GetSize();
}

int CCMSectItemGrid4CI::GetSizeList(CStringArray& raSizeList)
{
#define _S(x) raSizeList.Add(#x)
	raSizeList.RemoveAll();

	if (m_nShapeIndex == 3)  // I
	{
		_S(H1);
		_S(HL1); _S(HL2); _S(HL2-1); _S(HL2-2); _S(HL3); _S(HL4); _S(HL4-1); _S(HL4-2); _S(HL5); 
		_S(BL1); _S(BL2); _S(BL2-1); _S(BL2-2); _S(BL4); _S(BL4-1); _S(BL4-2); 
		_S(HR1); _S(HR2); _S(HR2-1); _S(HR2-2); _S(HR3); _S(HR4); _S(HR4-1); _S(HR4-2); _S(HR5); 
		_S(BR1); _S(BR2); _S(BR2-1); _S(BR2-2); _S(BR4); _S(BR4-1); _S(BR4-2); 
	}
	else if (m_nShapeIndex == 4)  // Tee
	{
		_S(H1);
		_S(HL1); _S(HL2); _S(HL3); 
		_S(BL1); _S(BL2); _S(BL3); _S(BL4); 
		_S(HL2-1); _S(HL2-2); _S(HL3-1); _S(HL3-2); 
		_S(BL2-1); _S(BL2-2); _S(BL3-1); _S(BL3-2); 
		_S(HR1); _S(HR2); _S(HR3); 
		_S(BR1); _S(BR2); _S(BR3); _S(BR4); 
		_S(HR2-1); _S(HR2-2); _S(HR3-1); _S(HR3-2); 
		_S(BR2-1); _S(BR2-2); _S(BR3-1); _S(BR3-2); 
	}

	return raSizeList.GetSize();
#undef _S
}
