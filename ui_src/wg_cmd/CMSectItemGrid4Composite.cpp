// CMSectItemGrid4Composite.cpp : implementation file
//
// Tapered-Composite 단면에서 사용되는 Grid Class
// SetViewData() 함수와 GetSizeList()에서 지정된 Row 정보에 따라
// ShowData() 함수에서 로우를 생성해주며 SetColumnStatusGetStatus()에서 
// Row Show/Hide, Enable/Disable 상태를 조정해준다.

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMSectItemGrid4Composite.h"

#include "CMSecViewWnd.h"
#include "CMSectItemTap.h"
#include "CMSectItemImportComposite.h"
#include "CMSectStiffenerDlg.h"
#include "CMSectSODDeckStiffenerDlg.h"

#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\SectUtil.h"
#include "..\wg_db\wg_db_SectDB.h"

#include "..\wg_base\wg_base_MsgDll.h"
#include "..\wg_base\Registry.h"

#include "..\wg_base\wg_base_TestEnvMgr.h"





#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCMSectItemGrid4Composite
CCMSectItemGrid4Composite::CCMSectItemGrid4Composite()
{
	m_bInit = FALSE;
	m_pParent = 0;
	m_pPreview = 0;
	m_pSectD = 0;
	m_nShapeIndex = -1;
	m_bSymmetry = FALSE;
	m_bHunch = FALSE;
	m_bOnStartEditing = FALSE;

	m_nSymmetryStlGSectI  = 0;
	m_nSymmetryStlGSectJ  = 0;
	m_bSymmetryStlGSectI = FALSE;
	m_bSymmetryStlGSectJ = FALSE;
	m_SectBefore.Initialize();
}

CCMSectItemGrid4Composite::~CCMSectItemGrid4Composite()
{
}

//---------------------------------------------------------------------------
// Public Interface Functions
void CCMSectItemGrid4Composite::InitGrid(CCMSecViewWnd* pPreview)
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

void CCMSectItemGrid4Composite::SetDataSource(T_SECT_D* pSectD, CCMSectItemTap* pParent)
{
	m_pSectD = pSectD;
	m_pParent = pParent;
	m_bSymmetry = m_pSectD->SectBefore.bSymmetric;
	m_bHunch = m_pSectD->SectBefore.bSmallHole;

	/*
	if (m_pSectD->nStype == D_SECT_TYPE_COMPO_B)
		m_nShapeIndex = 0;
	else if (m_pSectD->nStype == D_SECT_TYPE_COMPO_I)
		m_nShapeIndex = 1;
	else if (m_pSectD->nStype == D_SECT_TYPE_COMPO_CI)
		m_nShapeIndex = 2;
	else if (m_pSectD->nStype == D_SECT_TYPE_COMPO_CT)
		m_nShapeIndex = 3;
	else ASSERT(0);
	*/

	ShowData();
	SetRightValidCell(1, 1);
}

void CCMSectItemGrid4Composite::ChangeShape(int nNewShapeIndex, BOOL bSlab)
{
	CancelEditing();

	m_nShapeIndex = nNewShapeIndex;
	
	m_bSymmetry = m_pSectD->SectBefore.bSymmetric;
	m_bHunch = m_pSectD->SectBefore.bSmallHole;
	SetRightValidCell(1, 1);

	// show/hide row according to shape
	ShowData();
}

void CCMSectItemGrid4Composite::ShowData() // MNET:XXXX 20070221 changed by mylee
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

	m_nSymmetryRow    = 0;
	m_nCellTypeRow    = 0;
	m_nCellShapeRow   = 0;
	m_nJointRow       = 0;
	m_nSizeIRow       = 0;
	m_nSizeJRow       = 0;
	m_nSideHoleRow    = 0;
	m_nShearOptionRow = 0;
	m_nStiffIRow      = 0;
	m_nStiffJRow      = 0;
	m_nHunchRow       = 0;
	m_nCMPWebRowI     = 0;
	m_nCMPWebRowJ     = 0;
	m_nGirderRow      = 0; // new
	m_nSlabRow        = 0; // new
	m_nStiffenerRow   = 0; // new
	m_nStiffenerRow_I = 0; // new
	m_nStiffenerRow_J = 0; // new
	m_nStiffenerRow2  = 0; // new
	m_nSymmetryStlGSectI  = 0;
	m_nSymmetryStlGSectJ  = 0;
	m_bSymmetryStlGSectI = FALSE;
	m_bSymmetryStlGSectJ = FALSE;

	for (int i=0; i<2; i++)
	{
		m_nAutoShearCheckRow[i] = 0;
		m_nShearCheckRow[i] = 0;
		m_nAutoMinWebThikRow[i] = 0;
		m_nMinWebThikRow[i] = 0;
		m_nAutoShearQyRow[i] = 0;
		m_nShearQyRow[i] = 0;
	}

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
	if (CTestEnvMgr::GetTestEnvST(_LSX(Test_PSCComp)) != _LSX(yes)) {
		if (m_nShapeIndex == 2 || m_nShapeIndex == 3) {
			m_nNumSlab = 3;
		}
	}
	

	// Row 추가 START
	int nRow = 0;
	if (m_nNumGirder > 0)
	{
		nRow++;
		InsertRows(nRow, 1);
		SetCoveredCellsRowCol(nRow, 1, nRow, 3);
		SetStyleRange(CGXRange(nRow, 1), CGXStyle()
			.SetInterior(COLORREF(RGB(0, 0, 255)))
			.SetTextColor(COLORREF(RGB(255, 255, 0)))
			.SetValue(_LS(IDS_CMD_COMPO_GRID_Girder)));
		
		CStringArray aGirderList, aGirderListUnit;
		CArray<double, double> aGirderListValue;
		GetGirderList(aGirderList, aGirderListUnit, aGirderListValue);

		nRow++;
		m_nGirderRow = nRow;
		InsertRows(nRow, m_nNumGirder);
		for (int i = 0; i < m_nNumGirder; i++)
		{
			SetValueRange(CGXRange(nRow+i, 1), aGirderList[i]);
			SetStyleRange(CGXRange(nRow+i, 2), CGXStyle()
				.SetHorizontalAlignment(DT_RIGHT)
				.SetFormat(GX_FMT_FIXED)
				.SetPlaces(i==0?0:nPlace)
				.SetValue(aGirderListValue[i]) 
				.SetEnabled(TRUE));
			SetStyleRange(CGXRange(nRow+i, 3), CGXStyle().SetValue(aGirderListUnit[i]));
		}
		nRow += m_nNumGirder-1;
	}

	if (m_nNumSlab > 0)
	{
		nRow++;
		InsertRows(nRow, 1);
		SetCoveredCellsRowCol(nRow, 1, nRow, 3);
		SetStyleRange(CGXRange(nRow, 1), CGXStyle()
			.SetInterior(COLORREF(RGB(0, 0, 255)))
			.SetTextColor(COLORREF(RGB(255, 255, 0)))
			.SetValue(_LS(IDS_CMD_COMPO_GRID_Slab)));

		CStringArray aSlabList, aSlabListUnit;
		CArray<double, double> aSlabListValue;
		GetSlabList(aSlabList, aSlabListUnit, aSlabListValue);

		if(aSlabList.GetSize()>0)
		{
			nRow++;
			m_nSlabRow = nRow;
			InsertRows(nRow, m_nNumSlab);
			for (int i = 0; i < m_nNumSlab; i++)
			{
				SetValueRange(CGXRange(nRow+i, 1), aSlabList[i]);
				SetStyleRange(CGXRange(nRow+i, 2), CGXStyle()
					.SetHorizontalAlignment(DT_RIGHT)
					.SetFormat(GX_FMT_FIXED)
					.SetPlaces(nPlace)
					.SetValue(aSlabListValue[i]) 
					.SetEnabled(TRUE));
				SetStyleRange(CGXRange(nRow+i, 3), CGXStyle().SetValue(aSlabListUnit[i]));
			}
			nRow += m_nNumSlab-1;
		}
	}

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
	if (m_nNumHunch > 0)
	{
		nRow++;
		m_nHunchRow = nRow;
		InsertRows(nRow, m_nNumHunch);
		SetCoveredCellsRowCol(nRow, 1, nRow, 2);
		SetValueRange(CGXRange(nRow, 1), _LS(IDS_CMD_TAP_PSC_Hunch));
		SetStyleRange(CGXRange(nRow, 3), CGXStyle()
			.SetControl(GX_IDS_CTRL_CHECKBOX3D).SetHorizontalAlignment(DT_CENTER));
		SetValueRange(CGXRange(nRow, 3), (m_bHunch?_T("1"):_T("0")));
	}
	if (m_nNumCellType > 0)
	{
		nRow++;
		m_nCellTypeRow = nRow;
		InsertRows(nRow, m_nNumCellType);
		SetValueRange(CGXRange(nRow, 1), m_csNameCellType);
		SetCoveredCellsRowCol(nRow, 2, nRow, 3);
		SetCobxColumn(CGXRange(nRow, 2), GetCListCellType(m_nKindCellType));
		SetValueRange(CGXRange(nRow, 2), m_csValueCellType);
	}
	if (m_nNumSideHole > 0)
	{
		nRow++;
		m_nSideHoleRow = nRow;
		InsertRows(nRow, m_nNumSideHole);
		SetValueRange(CGXRange(nRow, 1), _LS(IDS_CMD_TAP_PSC_Side_Hole));
		SetCoveredCellsRowCol(nRow, 2, nRow, 3);
		SetStyleRange(CGXRange(nRow, 2), CGXStyle()
			.SetControl(GX_IDS_CTRL_CHECKBOX3D).SetHorizontalAlignment(DT_CENTER));
		SetValueRange(CGXRange(nRow, 2), (m_pSectD->SectBefore.bSmallHole?_T("1"):_T("0")));
	}
	if (m_nNumCellShape > 0)
	{
		nRow++;
		m_nCellShapeRow = nRow;
		InsertRows(nRow, m_nNumCellShape);
		SetValueRange(CGXRange(nRow, 1), m_csNameCellShape);
		SetCoveredCellsRowCol(nRow, 2, nRow, 3);
		if (m_nKindCellShape == 4 || m_nKindCellShape == 5) // girder num, cell num 
		{
			SetStyleRange(CGXRange(m_nCellShapeRow, 2), CGXStyle()
				.SetHorizontalAlignment(DT_RIGHT)
				.SetFormat(GX_FMT_FIXED)
				.SetPlaces(0)
				.SetValue(m_csValueCellShape)
				.SetEnabled(TRUE));
		}
		else
		{
			SetCobxColumn(CGXRange(nRow, 2), GetCListCellShape(m_nKindCellShape));
			SetStyleRange(CGXRange(m_nCellShapeRow, 2), CGXStyle()
				.SetEnabled(m_csValueCellShape != _T(""))
				.SetValue(m_csValueCellShape));
		}
	}
	if (m_nNumShearCheck > 0 && m_bUseShearCheck)
	{
		nRow++;
		m_nShearOptionRow = nRow;
		InsertRows(nRow, 1);
		SetCoveredCellsRowCol(nRow, 1, nRow, 2);
		SetValueRange(CGXRange(nRow, 1), _LS(IDS_CMD_TAP_PSC_Shear_Check));
		SetStyleRange(CGXRange(nRow, 3), CGXStyle()
			.SetControl(GX_IDS_CTRL_CHECKBOX3D).SetHorizontalAlignment(DT_CENTER));
		SetValueRange(CGXRange(nRow, 3), (m_pSectD->SectBefore.bShearCheck?_T("1"):_T("0")));
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
		GetJointList(aJointList); // Get Data
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
	if (m_nNumSize+m_nNumSitff > 0)
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

			if (m_nNumShearCheck > 0)
			{
				if (m_bUseAutoShearCheck)   // 옵션이 있는 경우 
				{
					ASSERT(m_nShearOptionRow > 0);
					nRow++;
					m_nAutoShearCheckRow[n] = nRow;
					m_nShearCheckRow[n] = nRow + 1;
					InsertRows(nRow, 3*2);

					CString csTitle;
					for (int i = 0; i < 3; i++)
					{
						// auto clac. shear check pos
						csTitle.Format(_LS(IDS_CMD_PSC_GRID_Z_Auto), i+1);
						SetValueRange(CGXRange(nRow+i*2, 1), csTitle);
						SetStyleRange(CGXRange(nRow+i*2, 2), CGXStyle()
							.SetControl(GX_IDS_CTRL_CHECKBOX3D)
							.SetHorizontalAlignment(DT_CENTER)
							.SetValue(m_pSectD->SectBefore.bAutoShearCheckPos[n][i] ? _T("1"):_T("0"))
							.SetEnabled(TRUE));
						// shear check pos
						csTitle.Format(_T("Z%d"), i+1);
						SetValueRange(CGXRange(nRow+i*2+1, 1), csTitle);
						SetStyleRange(CGXRange(nRow+i*2+1, 2), CGXStyle()
							.SetHorizontalAlignment(DT_RIGHT)
							.SetFormat(GX_FMT_FIXED)
							.SetPlaces(nPlace)
							.SetValue(m_pSectD->SectBefore.dShearCheckPos[n][i])
							.SetEnabled(TRUE));
						SetStyleRange(CGXRange(nRow+i+1, 3), CGXStyle().SetValue(csUnit));
					}
					nRow += 3*2-1;
				}
				else
				{
					//ASSERT(m_nShearOptionRow == 0);
					nRow++;
					m_nShearCheckRow[n] = nRow;
					InsertRows(nRow, 3);

					CString csTitle;
					for (int i = 0; i < 3; i++)
					{
						// auto clac. shear check pos : 표시 안함

						// shear check pos
						csTitle.Format(_T("Z%d"), i+1);
						SetValueRange(CGXRange(nRow+i, 1), csTitle);
						SetStyleRange(CGXRange(nRow+i, 2), CGXStyle()
							.SetHorizontalAlignment(DT_RIGHT)
							.SetFormat(GX_FMT_FIXED)
							.SetPlaces(nPlace)
							.SetValue(m_pSectD->SectBefore.dShearCheckPos[n][i])
							.SetEnabled(TRUE));
						SetStyleRange(CGXRange(nRow+i, 3), CGXStyle().SetValue(csUnit));
					}
					nRow += 3-1;
				}
			}
			if (m_nNumShearQy > 0)    // Shear Qy
			{
				nRow++;
				m_nAutoShearQyRow[n] = nRow;
				m_nShearQyRow[n] = nRow + 1;
				InsertRows(nRow, 2*3);

				CString str;
				CUnitCtrl::GetUnitSystem(D_UNITSYS_BASE_VOLUME, csUnit);
				for (int i = 0; i < 3; i++)
				{
					// auto clac. Qy
					str.Format(_LS(IDS_CMD_PSC_GRID_Q_Auto), i+1);
					SetValueRange(CGXRange(nRow+2*i+0, 1), str);
					SetStyleRange(CGXRange(nRow+2*i+0, 2), CGXStyle()
						.SetControl(GX_IDS_CTRL_CHECKBOX3D)
						.SetHorizontalAlignment(DT_CENTER)
						.SetValue(m_pSectD->SectBefore.bAutoQy[n][i] ? _T("1"):_T("0"))
						.SetEnabled(TRUE));
					// Qy
					str.Format(_LS(IDS_CMD_PSC_GRID_Qy_Q), i+1);
					SetValueRange(CGXRange(nRow+2*i+1, 1), str);
					SetStyleRange(CGXRange(nRow+2*i+1, 2), CGXStyle()
						.SetHorizontalAlignment(DT_RIGHT)
						.SetFormat(GX_FMT_FIXED)
						.SetPlaces(nPlace)
						.SetValue(m_pSectD->SectBefore.dQy[n][i])
						.SetEnabled(TRUE));
					SetStyleRange(CGXRange(nRow+2*i+1, 3), CGXStyle().SetValue(csUnit));
				}
				nRow += 2*3-1;
				CUnitCtrl::GetUnitSystem(D_UNITSYS_BASE_LENGTH, csUnit);
			}
			if (m_nNumMinWebThik > 0)  // minimum web thickness
			{
				nRow++;
				m_nAutoMinWebThikRow[n] = nRow;
				m_nMinWebThikRow[n] = nRow + 1;
				InsertRows(nRow, 2*4);

				CString str;
				int i = 0;
				for (i = 0; i < 3; i++)
				{
					// auto clac. Min. Web-Thick (for shear)
					str.Format(_LS(IDS_CMD_PSC_GRID_S_Auto), i+1);
					SetValueRange(CGXRange(nRow+2*i+0, 1), str);
					SetStyleRange(CGXRange(nRow+2*i+0, 2), CGXStyle()
						.SetControl(GX_IDS_CTRL_CHECKBOX3D)
						.SetHorizontalAlignment(DT_CENTER)
						.SetValue(m_pSectD->SectBefore.bAutoWebThkShear[n][i] ? _T("1"):_T("0"))
						.SetEnabled(TRUE));
					// Min. Web-Thick (for shear)
					str.Format(_LS(IDS_CMD_PSC_GRID_Web_S), i+1);
					SetValueRange(CGXRange(nRow+2*i+1, 1), str);
					SetStyleRange(CGXRange(nRow+2*i+1, 2), CGXStyle()
						.SetHorizontalAlignment(DT_RIGHT)
						.SetFormat(GX_FMT_FIXED)
						.SetPlaces(nPlace)
						.SetValue(m_pSectD->SectBefore.dWebThkShear[n][i])
						.SetEnabled(TRUE));
					SetStyleRange(CGXRange(nRow+2*i+1, 3), CGXStyle().SetValue(csUnit));
				}
				
				// auto clac. Min. Web-Thick (for torsion)
				SetValueRange(CGXRange(nRow+2*i+0, 1), _LS(IDS_CMD_PSC_GRID_T_Auto));
				SetStyleRange(CGXRange(nRow+2*i+0, 2), CGXStyle()
					.SetControl(GX_IDS_CTRL_CHECKBOX3D)
					.SetHorizontalAlignment(DT_CENTER)
					.SetValue(m_pSectD->SectBefore.bAutoWebThk[n] ? _T("1"):_T("0"))
					.SetEnabled(TRUE));
				// Min. Web-Thick (for torsion)
				SetValueRange(CGXRange(nRow+2*i+1, 1), _LS(IDS_CMD_PSC_GRID_Web_T));
				SetStyleRange(CGXRange(nRow+2*i+1, 2), CGXStyle()
					.SetHorizontalAlignment(DT_RIGHT)
					.SetFormat(GX_FMT_FIXED)
					.SetPlaces(nPlace)
					.SetValue(m_pSectD->SectBefore.dWebThk[n])
					.SetEnabled(TRUE));
				SetStyleRange(CGXRange(nRow+3, 3), CGXStyle().SetValue(csUnit));
				nRow += 2*4-1;
			}

			// size
			double *pSize;
			double  dValue[2];
			double  dSlabWidth = 0.0;
			if (n == 0) 
			{
				pSize = m_pSectD->SectBefore.SectI.Size;
				dValue[0] = m_pSectD->SectBefore.SectI.Design.TanA;
				dValue[1] = m_pSectD->SectBefore.SectI.Design.Beta;
				dSlabWidth = m_pSectD->SectAfter.SectI.Size[0];
			}
			else 
			{
				pSize = m_pSectD->CmpTapJ.Size;
				dValue[0] = m_pSectD->SectBefore.SectJ.Design.TanA;
				dValue[1] = m_pSectD->SectBefore.SectJ.Design.Beta;
				dSlabWidth = m_pSectD->CmpTapJ.Size[59];
			}

			if (m_nNumCMPWEB > 0)  // CMP Web size
			{
				ASSERT(m_nNumCMPWEB == 4 || m_nNumCMPWEB == 6);
				
				nRow++;
				if (n == 0) m_nCMPWebRowI = nRow;
				else m_nCMPWebRowJ = nRow;
				InsertRows(nRow, m_nNumCMPWEB);

				CString aTitleCMP[] = { _T("a"), _T("b"), _T("h"), _T("t"), _T("EFD"), _T("LRF") };
				int i = 0;
				for (i = 0; i < 4; i++)
				{
					SetValueRange(CGXRange(nRow+i, 1), aTitleCMP[i]);
					SetStyleRange(CGXRange(nRow+i, 2), CGXStyle()
						.SetHorizontalAlignment(DT_RIGHT)
						.SetFormat(GX_FMT_FIXED)
						.SetPlaces(nPlace)
						.SetValue(pSize[56+i])
						.SetEnabled(TRUE));
					SetStyleRange(CGXRange(nRow+i, 3), CGXStyle().SetValue(csUnit));
				}
				for ( ; i < m_nNumCMPWEB; i++)
				{
					SetValueRange(CGXRange(nRow+i, 1), aTitleCMP[i]);
					SetStyleRange(CGXRange(nRow+i, 2), CGXStyle()
						.SetHorizontalAlignment(DT_RIGHT)
						.SetFormat(GX_FMT_FIXED)
						.SetPlaces(nPlace)
						.SetValue(dValue[i-4])
						.SetEnabled(TRUE));
					SetStyleRange(CGXRange(nRow+i, 3), CGXStyle().SetValue(_T("")));
				}
				nRow += m_nNumCMPWEB-1;
			}

			// size
			nRow++;
			if (n == 0) m_nSizeIRow = nRow;
			else m_nSizeJRow = nRow;
			InsertRows(nRow, m_nNumSize);

			if(m_nShapeIndex == 7 || m_nShapeIndex == 8 || m_nShapeIndex == 9)
			{
				// Stl Girder I,Box,Tub는 Size그룹이.. SlabWidth, Symetric(CheckBox), Top, Bot, Size로 구성되기 때문에 따로 처리 함
				ShowDataStlGirder(n, nRow, aSizeList, nPlace, csUnit, dSlabWidth, pSize);
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
						.SetValue((i==0 ? dSlabWidth : pSize[i-1]))
						.SetEnabled(FALSE));
					SetStyleRange(CGXRange(nRow+i, 3), CGXStyle().SetValue(csUnit));
				}
			}      
			nRow += m_nNumSize-1;

			// Stiff
			if (m_nNumSitff > 0)
			{
				nRow++;
				if (n == 0) m_nStiffIRow = nRow;
				else m_nStiffJRow = nRow;
				InsertRows(nRow, m_nNumSitff);

				CArray<double, double> aStiff;
				int nStiff = GetStiffList(n, aStiff);
				ASSERT(nStiff == m_nNumSitff);
				for (int i = 0; i < m_nNumSitff; i++)
				{
					SetValueRange(CGXRange(nRow+i, 1), aSizeList[m_nNumSize+i]);
					SetStyleRange(CGXRange(nRow+i, 2), CGXStyle()
						.SetHorizontalAlignment(DT_RIGHT)
						.SetFormat(GX_FMT_FIXED)
						.SetPlaces(nPlace)
						.SetValue(aStiff[i])
						.SetEnabled(TRUE));
					SetStyleRange(CGXRange(nRow+i, 3), CGXStyle().SetValue(aUnitList[m_nNumSize+i]));
				}
				nRow += m_nNumSitff-1;
			}
		}
	}
	if (m_nNumStiffener > 0)
	{
		nRow++;
		InsertRows(nRow, 1);
		SetCoveredCellsRowCol(nRow, 1, nRow, 3);
		SetStyleRange(CGXRange(nRow, 1), CGXStyle()
			.SetInterior(COLORREF(RGB(0, 0, 255)))
			.SetTextColor(COLORREF(RGB(255, 255, 0)))
			.SetValue(_LS(IDS_CMD_COMPO_GRID_Stiffener)));

		CStringArray aStiffenerList, aStiffenerListUnit;
		CArray<double, double> aStiffenerListValue;
		GetStiffenerList(aStiffenerList, aStiffenerListUnit, aStiffenerListValue);

		nRow++;
		m_nStiffenerRow = nRow;
		InsertRows(nRow, m_nNumStiffener);
		for (int i = 0; i < m_nNumStiffener; i++)
		{
			SetValueRange(CGXRange(nRow+i, 1), aStiffenerList[i]);
			SetStyleRange(CGXRange(nRow+i, 2), CGXStyle()
				.SetHorizontalAlignment(DT_RIGHT)
				.SetFormat(GX_FMT_FIXED)
				.SetPlaces((i==0||i==3)?0:nPlace)
				.SetValue(aStiffenerListValue[i]) 
				.SetEnabled(TRUE));
			SetStyleRange(CGXRange(nRow+i, 3), CGXStyle().SetValue(aStiffenerListUnit[i]));
		}
		nRow += m_nNumStiffener-1;
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

	// Row Insert END

	SetStyleRange(CGXRange().SetTable(), CGXStyle().SetVerticalAlignment(DT_VCENTER));
	SetStyleRange(CGXRange().SetCols(1), CGXStyle().SetEnabled(FALSE));
	SetStyleRange(CGXRange().SetCols(3), CGXStyle().SetEnabled(FALSE)); 
	if(m_nSymmetryRow > 0)    SetStyleRange(CGXRange(m_nSymmetryRow   , 3), CGXStyle().SetEnabled(TRUE));
	if(m_nHunchRow > 0)       SetStyleRange(CGXRange(m_nHunchRow      , 3), CGXStyle().SetEnabled(TRUE));
	if(m_nNumShearCheck > 0)  SetStyleRange(CGXRange(m_nShearOptionRow, 3), CGXStyle().SetEnabled(TRUE));

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

// *^^* 여기 추가 
void CCMSectItemGrid4Composite::SetViewData() // MNET:XXXX 20070221 changed by mylee
{
	m_nNumGirder = 0;
	m_nNumSlab = 0;
	m_nNumStiffener = 0;
	m_nNumStiffener2 = 0;

	m_nNumSymmetry = 0;
	m_nNumCellType = 0; m_csNameCellType = _T(""); m_nKindCellType = 0;
	m_nNumCellShape = 0; m_csNameCellShape = _T(""); m_nKindCellShape = 0;
	m_nNumSideHole = 0;
	m_nNumShearCheck = 0;
	m_nNumMinWebThik = 0;
	m_nNumShearQy = 0;
	m_nNumJoint = 0;
	m_nNumSize = 0;
	m_nJointType = 0; // In/Out, Left/Right
	m_nJointNum0 = 0; // Single(J1)
	m_nJointNum1 = 0; // Out or Left
	m_nJointNum2 = 0; // In or Right
	m_nSizeNum0 = 0;
	m_nSizeNum1 = 0;
	m_nSizeNum2 = 0;
	m_nNumSitff = 0;
	m_bUseShearCheck = TRUE;
	m_bUseAutoShearCheck = TRUE;      // Shear-Pos 용
	m_bUseAutoTorsionCheck = TRUE;    // Torsion-Thk. 용 
	m_nNumHunch = 0;
	m_nNumCMPWEB = 0;

	CString aNoneCirclePolygon[] = {_LS(IDS_CMD_PSC_GRID_None), _LS(IDS_CMD_PSC_GRID_Circle), _LS(IDS_CMD_PSC_GRID_Polygon)};
	CString aLeftRight[] = {_LS(IDS_CMD_PSC_GRID_Left), _LS(IDS_CMD_PSC_GRID_Right)};
	CString aHalf1Cell2Cell[] = {_LS(IDS_CMD_PSC_GRID_Half), _LS(IDS_CMD_PSC_GRID_1Cell), _LS(IDS_CMD_PSC_GRID_2Cell)};
	CString aVertSlopeChamfr[] = {_LS(IDS_CMD_PSC_GRID_Vertical), _LS(IDS_CMD_PSC_GRID_Slope), _LS(IDS_CMD_PSC_GRID_Chamfer)};
	CString aPolygonCircle[] = {_LS(IDS_CMD_PSC_GRID_Polygon), _LS(IDS_CMD_PSC_GRID_Circle)};

	BOOL b7Dof  = (CSectDB::IsEnable7thDOF());
	switch(m_nShapeIndex)
	{
	case 0: // B
		m_nNumGirder = (b7Dof)? 0 : 2;
		m_nNumSlab = 3;
		m_nNumSize = 9;
		//m_nNumStiffener = 6;
		m_nNumStiffener2 = 2;
		break;
	case 1: // I
		m_nNumGirder = (b7Dof)? 0 : 2;
		m_nNumSlab = 3;
		m_nNumSize = 7;
		m_nNumStiffener2 = 2;
		break;
	case 2: // CI
		m_nNumGirder = (b7Dof)? 0 : 2;
		m_nNumSlab = 4;
		m_nNumSymmetry = 1;
		m_nNumJoint = 9;
		m_nNumSize = 34; 
		m_nSizeNum0 = 2; 
		m_nSizeNum1 = 16; 
		m_nSizeNum2 = 16;
		m_nJointType = 2; // Left/Right
		m_nJointNum0 = 1;
		m_nJointNum1 = 4;
		m_nJointNum2 = 4;
		m_nJoint = m_pSectD->SectBefore.nJoint;
		break;
	case 3: // CT
		m_nNumGirder = (b7Dof)? 0 : 2;
		m_nNumSlab = 4;
		m_nNumSymmetry = 1;
		m_nNumJoint = 9;
		m_nNumSize = 32;
		m_nSizeNum0 = 2; 
		m_nSizeNum1 = 15; 
		m_nSizeNum2 = 15;
		m_nJointType = 2; // Left/Right
		m_nJointNum0 = 1;
		m_nJointNum1 = 4;
		m_nJointNum2 = 4;
		m_nJoint = m_pSectD->SectBefore.nJoint;
		break;
	case 6: // Tub
		m_nNumGirder = (b7Dof)? 0 : 2;
		m_nNumSlab = 3;
		m_nNumSize = 11;
		m_nNumStiffener2 = 2;
		break;
	case 7: // Stl Girder Box
		m_nNumGirder = (b7Dof)? 0 : 2;
		m_nNumSlab = 3;
		m_nNumSize = 15;
		m_nNumStiffener2 = 2;
		break;
	case 8: // Stl Girder I
		m_nNumGirder = (b7Dof)? 0 : 2;
		m_nNumSlab = 3;
		m_nNumSize = 12;
		m_nNumStiffener2 = 2;
		break;
	case 9: // Stl Girder Tub
		m_nNumGirder = (b7Dof)? 0 : 2;
		m_nNumSlab = 3;
		m_nNumSize = 18;
		m_nNumStiffener2 = 2;
		break;
	/*
	case 0: // D_SECT_SHAPE_PSC_1CELL
		//m_nNumJoint = 8;
		m_nNumSize = 9;
		//m_nJointType = 1; // In/Out
		//m_nJointNum1 = 3;
		//m_nJointNum2 = 5;
		//m_nJoint = m_pSectD->SectBefore.SectI.BuiltUpFlag;
		//m_nNumShearCheck = 1;
		//m_nNumMinWebThik = 1;
		break;
	case 1: // D_SECT_SHAPE_PSC_2CELL -> I
		m_nNumJoint = 8;
		m_nNumSize = 30;
		m_nJointType = 1; // In/Out
		m_nJointNum1 = 3;
		m_nJointNum2 = 5;
		m_nJoint = m_pSectD->SectBefore.SectI.BuiltUpFlag;
		m_nNumShearCheck = 1;
		m_nNumMinWebThik = 1;
		break;
	case 2: // D_SECT_SHAPE_PSC_3CELL -> CI
		m_nNumJoint = 13;
		m_nNumSize = 48;
		m_nJointType = 1; // In/Out
		m_nJointNum1 = 2;
		m_nJointNum2 = 11;
		m_nJoint = m_pSectD->SectBefore.nJoint;
		m_nNumShearCheck = 1;
		m_nNumMinWebThik = 1;
		break;
	case 3: // D_SECT_SHAPE_PSC_MID -> CT
		m_nNumSymmetry = 1;
		m_nNumCellType = 1; m_csNameCellType = _LS(IDS_CMD_PSC_GRID_Left); m_nKindCellType = 1; // none/circle/polygon
		m_csValueCellType = aNoneCirclePolygon[m_pSectD->SectBefore.nCellType];
		m_nNumCellShape = 1; m_csNameCellShape = _LS(IDS_CMD_PSC_GRID_Right); m_nKindCellShape = 1; // none/circle/polygon
		m_csValueCellShape = aNoneCirclePolygon[m_pSectD->SectBefore.nCellShape];
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
		m_nNumShearCheck = 1;
		m_nNumMinWebThik = 1;
		break;
	case 4: // D_SECT_SHAPE_PSC_HALF
		m_nNumCellType = 1; m_csNameCellType = _LS(IDS_CMD_PSC_GRID_Type); m_nKindCellType = 2;    // left/right
		m_csValueCellType = aLeftRight[m_pSectD->SectBefore.nCellType];
		m_nNumCellShape = 1; m_csNameCellShape = _LS(IDS_CMD_PSC_GRID_Shape); m_nKindCellShape = 1; // none/circle/polygon
		m_csValueCellShape = aNoneCirclePolygon[m_pSectD->SectBefore.nCellShape];
		m_nNumJoint = 8;
		m_nNumSize = 29;
		m_nJointType = 1; // In/Out
		m_nJointNum1 = 3;
		m_nJointNum2 = 5;
		m_nJoint = m_pSectD->SectBefore.nJoint;
		m_nNumShearCheck = 1;
		m_nNumMinWebThik = 1;
		break;
	case 5: // D_SECT_SHAPE_PSC_T
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
		m_nNumShearCheck = 1;
		m_nNumMinWebThik = 1;
		break;
	case 6: // D_SECT_SHAPE_PSC_BOX
		m_nNumJoint = 2;
		m_nNumCellType = 1; m_csNameCellType = _LS(IDS_CMD_PSC_GRID_Type); m_nKindCellType = 3;  // half/1cell/2cell
		m_csValueCellType = aHalf1Cell2Cell[m_pSectD->SectBefore.nCellType];
		if (m_pSectD->SectBefore.nCellType == 0) 
		{
			m_nNumCellShape = 1; m_csNameCellShape = _LS(IDS_CMD_PSC_GRID_Shape); m_nKindCellShape = 2;  // left/right
			m_csValueCellShape = aLeftRight[m_pSectD->SectBefore.nCellShape];
		}
		else if (m_pSectD->SectBefore.nCellType == 1) 
		{
			m_nNumSymmetry = 1;
			m_nNumCellShape = 1; m_csNameCellShape = _LS(IDS_CMD_PSC_GRID_Shape); m_nKindCellShape = 3;  // circle/polygon
			m_csValueCellShape = aNoneCirclePolygon[m_pSectD->SectBefore.nCellShape];
		}
		else if (m_pSectD->SectBefore.nCellType == 2) 
		{
			m_nNumCellShape = 1; m_csNameCellShape = _LS(IDS_CMD_PSC_GRID_Shape); m_nKindCellShape = 2; // left/right
			m_csValueCellShape = _T("");
			m_nNumSymmetry = 1;
		}
		m_nNumSize = 29;
		m_nSizeNum0 = 2; 
		m_nSizeNum1 = 14; 
		m_nSizeNum2 = 13;
		m_nJointType = 2; // Left/Right
		m_nJointNum1 = 1;
		m_nJointNum2 = 1;
		m_nJoint = m_pSectD->SectBefore.nJoint;
		m_nNumShearCheck = 1;
		m_nNumMinWebThik = 1;
		break;
	case 7: // D_SECT_SHAPE_PSC_I
		m_nNumSymmetry = 1;
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
		m_nNumShearCheck = 1;
		m_nNumMinWebThik = 1;
		break;
	case 8: // D_SECT_SHAPE_PSC_4CELL
		m_nNumCellType = 1; m_csNameCellType = _LS(IDS_CMD_TAP_PSC_Type); m_nKindCellType = 4; // vertical/slope/chamfer
		m_csValueCellType = aVertSlopeChamfr[m_pSectD->SectBefore.nCellType];
		m_nNumCellShape = 1; m_csNameCellShape = _LS(IDS_CMD_PSC_GRID_Girder_Number); m_nKindCellShape = 4;  // Girder Number
		m_csValueCellShape.Format(_T("%d"), m_pSectD->SectBefore.nCellShape+1);
		m_nNumSize = 13;
		m_nNumShearCheck = 1;
		m_nNumMinWebThik = 1;
		break;
		// *^^*
	case 9: // D_SECT_SHAPE_PSC_NCEL2
		m_nNumSymmetry = 1;
		m_nNumCellType = 1; m_csNameCellType = _LS(IDS_CMD_TAP_PSC_Type); m_nKindCellType = 5;  // polygon/circle
		m_csValueCellType = aPolygonCircle[m_pSectD->SectBefore.nCellType];
		m_nNumCellShape = 1; m_csNameCellShape = _LS(IDS_CMD_TAP_PSC_Cell_Num); m_nKindCellShape = 5;  // cell number = girder number + 1
		if (m_pSectD->SectBefore.nCellShape < 1) m_pSectD->SectBefore.nCellShape = 1;
		m_csValueCellShape.Format(_T("%d"), m_pSectD->SectBefore.nCellShape);
		m_nNumSideHole = 1;
		m_nNumJoint = 2;
		m_nNumSize = 59; 
		m_nSizeNum0 = 1; 
		m_nSizeNum1 = 29; 
		m_nSizeNum2 = 29;
		m_nJointType = 3; // In/Out
		m_nJointNum1 = 1;
		m_nJointNum2 = 1;
		m_nJoint = m_pSectD->SectBefore.nJoint;
		m_nNumShearCheck = 1;
		m_nNumMinWebThik = 1;
		break; 
	case 10: // D_SECT_SHAPE_PSC_VALUE
		m_nNumShearCheck = 1;
		m_nNumShearQy = 1;
		m_nNumMinWebThik = 1;
		m_nNumSize = 4;
		m_nSizeNum0 = 4;
		m_nSizeNum1 = 0;
		m_nSizeNum2 = 0;
		m_nNumSitff = 24;
		m_bUseShearCheck = FALSE;
		m_bUseAutoShearCheck = FALSE;
		m_bUseAutoTorsionCheck = FALSE;
		break;
	case 11:  // D_SECT_SHAPE_PSC_CMPWEB
		m_nNumSymmetry = 1;
		m_nNumShearCheck = 1;
		m_nNumSize = 35;
		m_nSizeNum0 = 35;
		m_nSizeNum1 = 0;
		m_nSizeNum2 = 0;
		m_nNumHunch = 1;
		if (CProduct::GetMovingType() == D_PRODUCT_MOVING_JP)
			m_nNumCMPWEB = 6;
		else 
			m_nNumCMPWEB = 4;
		m_bUseShearCheck = TRUE;
		m_bUseAutoShearCheck = FALSE;
		break;
		*/
	default:
		//ASSERT(0);
		break;
	}
}

BOOL CCMSectItemGrid4Composite::SaveData()
{
	BOOL bSuccess = SaveFlag();
	bSuccess = bSuccess && SaveSize();
	return bSuccess;
}

BOOL CCMSectItemGrid4Composite::SaveFlag(BOOL bExcludeJoint/*=FALSE*/)
{
	CString csNum;
	int nNum;

	// girder
	if (m_nGirderRow > 0)
	{
		csNum = GetEditingValue(m_nGirderRow, 2);
		m_pSectD->SectAfter.SectI.BuiltUpFlag = _ttoi(csNum);
		csNum = GetEditingValue(m_nGirderRow+1, 2);
		if(!GetDoubleValue(csNum, m_pSectD->SectAfter.SectI.Size[1])) { return FALSE; }
	}

	// slab
	if (m_nSlabRow > 0)
	{
		csNum = GetEditingValue(m_nSlabRow, 2);
		if(!GetDoubleValue(csNum, m_pSectD->SectAfter.SectJ.Size[0])) { return FALSE; }
		csNum = GetEditingValue(m_nSlabRow+1, 2);
		if(!GetDoubleValue(csNum, m_pSectD->SectAfter.SectJ.Size[1])) { return FALSE; }
		csNum = GetEditingValue(m_nSlabRow+2, 2);
		if(!GetDoubleValue(csNum, m_pSectD->SectAfter.SectJ.Size[2])) { return FALSE; }

		if (CTestEnvMgr::GetTestEnvST(_LSX(Test_PSCComp)) == _LSX(yes))
		{
			if (m_pSectD->SectBefore.nStype == 12 || m_pSectD->SectBefore.nStype == 13) //  PSC Composite 이면
			{
				csNum = GetEditingValue(m_nSlabRow + 3, 2);
				if (!GetDoubleValue(csNum, m_pSectD->SectAfter.SectJ.Size[3])) { return FALSE; }
			}
		}
	}

	// stiffener
	if (m_nStiffenerRow > 0)
	{
		m_pSectD->SectBefore.nStiffNum[0] = _ttoi(GetEditingValue(m_nStiffenerRow, 2));
		csNum = GetEditingValue(m_nStiffenerRow+1, 2);
		if(!GetDoubleValue(csNum, m_pSectD->SectBefore.SectJ.Size[2])) { return FALSE; }
		csNum = GetEditingValue(m_nStiffenerRow+2, 2);
		if(!GetDoubleValue(csNum, m_pSectD->SectBefore.SectJ.Size[4])) { return FALSE; }

		m_pSectD->SectBefore.nStiffNum[1] = _ttoi(GetEditingValue(m_nStiffenerRow+3, 2));
		csNum = GetEditingValue(m_nStiffenerRow+4, 2);
		if(!GetDoubleValue(csNum, m_pSectD->SectBefore.SectJ.Size[3])) { return FALSE; }
		csNum = GetEditingValue(m_nStiffenerRow+5, 2);
		if(!GetDoubleValue(csNum, m_pSectD->SectBefore.SectJ.Size[5])) { return FALSE; }
	}

	// symmetry
	m_pSectD->SectBefore.bSymmetric = 0;
	if (m_nSymmetryRow > 0)
	{
		m_pSectD->SectBefore.bSymmetric = m_bSymmetry;
		//CString csSymmetric = GetEditingValue(m_nSymmetryRow, 3);
		//m_pSectD->SectBefore.bSymmetric = (csSymmetric == _T("1")) ? TRUE : FALSE;
	}

	// cell type
	m_pSectD->SectBefore.nCellType = 0;
	if (m_nCellTypeRow > 0)
	{
		CString csType = GetEditingValue(m_nCellTypeRow, 2);
		if (m_nKindCellType == 5)   // polygon / circle
		{
			if (csType == _LS(IDS_CMD_PSC_GRID_Polygon)) m_pSectD->SectBefore.nCellType = 0;
			else if (csType == _LS(IDS_CMD_PSC_GRID_Circle)) m_pSectD->SectBefore.nCellType = 1;
			else ASSERT(0);
		}
		else
		{
			if (csType == _LS(IDS_CMD_PSC_GRID_None) || csType == _LS(IDS_CMD_PSC_GRID_Left) || 
					csType == _LS(IDS_CMD_PSC_GRID_Half) || csType == _LS(IDS_CMD_PSC_GRID_Vertical)) m_pSectD->SectBefore.nCellType = 0;
			else if (csType == _LS(IDS_CMD_PSC_GRID_Circle) || csType == _LS(IDS_CMD_PSC_GRID_Right) || 
							 csType == _LS(IDS_CMD_PSC_GRID_1Cell) || csType == _LS(IDS_CMD_PSC_GRID_Slope)) m_pSectD->SectBefore.nCellType = 1;
			else if (csType == _LS(IDS_CMD_PSC_GRID_Polygon) || csType == _LS(IDS_CMD_PSC_GRID_2Cell) || 
							 csType == _LS(IDS_CMD_PSC_GRID_Chamfer)) m_pSectD->SectBefore.nCellType = 2;
		}
	}

	// cell shape
	m_pSectD->SectBefore.nCellShape = 0;
	if (m_nCellShapeRow > 0)
	{
		if (m_nSymmetryRow && m_bSymmetry && m_nShapeIndex == 3) // Mid형 대칭 
			m_pSectD->SectBefore.nCellShape = m_pSectD->SectBefore.nCellType;
		else if (m_nKindCellShape == 4)   // PSC Multi-Cell : girder number
		{
			CString csGirderNum = GetEditingValue(m_nCellShapeRow, 2);
			m_pSectD->SectBefore.nCellShape = _ttoi(csGirderNum) - 1;
		}
		else if (m_nKindCellShape == 5)   // PSC Multi-Cell : cell number
		{
			CString csCellNum = GetEditingValue(m_nCellShapeRow, 2);
			m_pSectD->SectBefore.nCellShape = _ttoi(csCellNum);
		}
		else
		{
			CString csShape = GetEditingValue(m_nCellShapeRow, 2);
			if (csShape == _LS(IDS_CMD_PSC_GRID_None) || csShape == _LS(IDS_CMD_PSC_GRID_Left)) m_pSectD->SectBefore.nCellShape = 0;
			else if (csShape == _LS(IDS_CMD_PSC_GRID_Circle) || csShape == _LS(IDS_CMD_PSC_GRID_Right)) m_pSectD->SectBefore.nCellShape = 1;
			else if (csShape == _LS(IDS_CMD_PSC_GRID_Polygon)) m_pSectD->SectBefore.nCellShape = 2;
		}
	}

	// side hole or hunch
	m_pSectD->SectBefore.bSmallHole = 0;
	if (m_nHunchRow > 0)
	{
		CString csHunch = GetEditingValue(m_nHunchRow, 2);
		m_pSectD->SectBefore.bSmallHole = (csHunch == _T("1")) ? TRUE : FALSE;
	}
	if (m_nSideHoleRow > 0)
	{
		CString csSideHole = GetEditingValue(m_nSideHoleRow, 2);
		m_pSectD->SectBefore.bSmallHole = (csSideHole == _T("1")) ? TRUE : FALSE;
	}

	// shear check option
	if (!m_bUseShearCheck) // 옵션을 사용하지 않으면 무조건 입력받음.
		m_pSectD->SectBefore.bShearCheck = TRUE;
	else
	{
		m_pSectD->SectBefore.bShearCheck = 0;
		if (m_nShearOptionRow > 0)
		{
			CString csShearCheck = GetEditingValue(m_nShearOptionRow, 3);
			m_pSectD->SectBefore.bShearCheck = (csShearCheck == _T("1")) ? TRUE : FALSE;
		}
	}

	if(bExcludeJoint) return TRUE;

	int nJoint = 0;
	if (m_nJointRow > 0)
	{
		// MNET:XXXX-JBSEON-20061030 ncel2 는 예외 처리(I, MID 등과 다르다)
		// I, MID는 Joint가 Symmetry 따라가나 Ncel2는 그렇지 않다.
		int nJointFlag = 0x01;
		if(m_nSymmetryRow && m_bSymmetry && m_pSectD->SectBefore.Shape != D_SECT_SHAPE_PSC_NCEL2)  // 대칭 
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
	if (m_nShapeIndex == 0 || m_nShapeIndex == 1 || m_nShapeIndex == 6 
		|| m_nShapeIndex == 7 || m_nShapeIndex == 8 || m_nShapeIndex == 9 )
		m_pSectD->SectBefore.SectI.BuiltUpFlag = nJoint;
	else m_pSectD->SectBefore.nJoint = nJoint;

	return TRUE;
}

BOOL CCMSectItemGrid4Composite::SaveSize() // MNET:XXXX 20070221 changed by mylee
{
	CString csNum;
	double dNum;
	int nNum = 0;
	double* pSize;
	CArray<double, double> aStiff;
	int nSizeRow, nStiffRow, nCMPWebRow;

	for (int n = 0; n < 2; n++)
	{
		// shear check position
		if (m_nNumShearCheck > 0)
		{
			if (m_bUseAutoShearCheck)
			{
				//nIndex = (m_nSymmetryRow && m_bSymmetry) ? 0 : n;
				for (int i=0; i<3; i++)
				{
					// auto
					csNum = GetEditingValue(m_nAutoShearCheckRow[n]+i*2, 2);
					m_pSectD->SectBefore.bAutoShearCheckPos[n][i] = (csNum == _T("1")) ? TRUE : FALSE;
					// position
					if (m_pSectD->SectBefore.bAutoShearCheckPos[n][i])
						m_pSectD->SectBefore.dShearCheckPos[n][i] = 0.0;
					else
					{
						csNum = GetEditingValue(m_nShearCheckRow[n]+i*2, 2);
						if (GetDoubleValue(csNum, dNum)) 
							m_pSectD->SectBefore.dShearCheckPos[n][i] = dNum;
						else 
							m_pSectD->SectBefore.dShearCheckPos[n][i] = 0.0;
					}
				}
			}
			else
			{
				for (int i=0; i<3; i++)
				{
					// auto
					m_pSectD->SectBefore.bAutoShearCheckPos[n][i] = FALSE;
					// position
					if (m_pSectD->SectBefore.bAutoShearCheckPos[n][i])
						m_pSectD->SectBefore.dShearCheckPos[n][i] = 0.0;
					else
					{
						csNum = GetEditingValue(m_nShearCheckRow[n]+i, 2);
						if (GetDoubleValue(csNum, dNum)) 
							m_pSectD->SectBefore.dShearCheckPos[n][i] = dNum;
						else 
							m_pSectD->SectBefore.dShearCheckPos[n][i] = 0.0;
					}
				}
			}
		}
		// Shear Qy 
		if (m_nNumShearQy)
		{
			for (int i = 0; i < 3; i++)
			{
				csNum = GetEditingValue(m_nAutoShearQyRow[n]+2*i, 2);
				m_pSectD->SectBefore.bAutoQy[n][i] = (csNum == _T("1")) ? TRUE : FALSE;
				csNum = GetEditingValue(m_nShearQyRow[n]+2*i, 2);
				if (GetDoubleValue(csNum, dNum)) 
					m_pSectD->SectBefore.dQy[n][i] = dNum;
				else 
					m_pSectD->SectBefore.dQy[n][i] = 0.0;
			}
		}
		// minimum web thickness
		if (m_nNumMinWebThik > 0)
		{
			//nIndex = (m_nSymmetryRow && m_bSymmetry) ? 0 : n;
			// for shear
			int i = 0;
			for (i = 0; i < 3; i++)
			{
				csNum = GetEditingValue(m_nAutoMinWebThikRow[n]+2*i, 2);
				m_pSectD->SectBefore.bAutoWebThkShear[n][i] = (csNum == _T("1")) ? TRUE : FALSE;
				csNum = GetEditingValue(m_nMinWebThikRow[n]+2*i, 2);
				if (GetDoubleValue(csNum, dNum)) 
					m_pSectD->SectBefore.dWebThkShear[n][i] = dNum;
				else 
					m_pSectD->SectBefore.dWebThkShear[n][i] = 0.0;
			}
			// for torsion
			if (m_bUseAutoTorsionCheck)
			{
				csNum = GetEditingValue(m_nAutoMinWebThikRow[n]+2*i, 2);
				m_pSectD->SectBefore.bAutoWebThk[n] = (csNum == _T("1")) ? TRUE : FALSE;
			}
			else m_pSectD->SectBefore.bAutoWebThk[n] = FALSE;
			csNum = GetEditingValue(m_nMinWebThikRow[n]+2*i, 2);
			if (GetDoubleValue(csNum, dNum)) 
				m_pSectD->SectBefore.dWebThk[n] = dNum;
			else 
				m_pSectD->SectBefore.dWebThk[n] = 0.0;
		}

		// size
		if (n == 0) 
		{
			nSizeRow   = m_nSizeIRow;
			nCMPWebRow = m_nCMPWebRowI;
			pSize = m_pSectD->SectBefore.SectI.Size;
		}
		else
		{
			nSizeRow   = m_nSizeJRow;
			nCMPWebRow = m_nCMPWebRowJ;
			pSize = m_pSectD->CmpTapJ.Size;
		}

		if (m_nNumCMPWEB > 0)
		{
			for (int i = 0; i < 4; i++)
			{
				csNum = GetEditingValue(nCMPWebRow+i, 2);
				if (GetDoubleValue(csNum, dNum)) 
					pSize[56+i] = dNum;
				else pSize[56+i] = 0.0;
			}
			if (m_nNumCMPWEB == 6)
			{
				T_SECT_DESIGN* pDesign;
				if (n == 0) pDesign = &m_pSectD->SectBefore.SectI.Design;
				else        pDesign = &m_pSectD->SectBefore.SectJ.Design;

				csNum = GetEditingValue(nCMPWebRow+4, 2);
				if (GetDoubleValue(csNum, dNum)) 
					pDesign->TanA = dNum;
				else pDesign->TanA = 0.0;

				csNum = GetEditingValue(nCMPWebRow+5, 2);
				if (GetDoubleValue(csNum, dNum)) 
					pDesign->Beta = dNum;
				else pDesign->Beta = 0.0;
			}
			else
			{
				m_pSectD->SectBefore.SectI.Design.TanA = 1.0;
				m_pSectD->SectBefore.SectI.Design.Beta = 1.0;
			}
		}

		// Slab Width
		double* pSlabWidth = 0;
		if (n==0) pSlabWidth = &m_pSectD->SectAfter.SectI.Size[0];
		else      pSlabWidth = &m_pSectD->CmpTapJ.Size[59];
		csNum = GetEditingValue(nSizeRow, 2);
		if (GetDoubleValue(csNum, dNum)) *pSlabWidth = dNum;
		else *pSlabWidth = 0.0;

		if (m_nSymmetryRow && m_bSymmetry)  // 대칭 
		{
			int nRow;
			for (int i = 1; i < m_nNumSize; i++)
			{
				if(i < m_nSizeNum0) nRow = nSizeRow+i;
				else nRow = nSizeRow+m_nSizeNum0+((i-m_nSizeNum0)%m_nSizeNum1);
				csNum = GetEditingValue(nRow, 2);
				if (GetDoubleValue(csNum, dNum)) 
					pSize[i-1] = dNum;
				else 
					pSize[i-1] = 0.0;
			}
		}
		else // 비대칭
		{
			if(m_nShapeIndex == 7 || m_nShapeIndex == 8 || m_nShapeIndex == 9)
			{
				// Stl Girder I,Box,Tub는 Size그룹이.. SlabWidth, Symetric(CheckBox), Top, Bot, Size로 구성되기 때문에 따로 처리 함
				// 이 함수 안에서 SymmetricAutoCalc연산도 수행 함.
				SaveSizeStlGirder(nSizeRow, nNum, n, dNum, pSize);
			}
			else
			{
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

		// stiffness
		if (m_nNumSitff > 0)
		{
			if (n == 0) nStiffRow = m_nStiffIRow;
			else  nStiffRow = m_nStiffJRow;
			aStiff.RemoveAll();

			for (int i = 0; i < m_nNumSitff; i++)
			{
				csNum = GetEditingValue(nStiffRow+i, 2);
				if (GetDoubleValue(csNum, dNum)) aStiff.Add(dNum);
				else aStiff.Add(0.0);
			}
			SetStiffList(n, aStiff);
		}
	}

	// PSC_nCell에서 CellNum가 1개 혹은 2개인 경우 Slab Width 저장
	if (m_pSectD->SectBefore.Shape == D_SECT_SHAPE_PSC_NCEL2)
	{
		BOOL bSaveSlabWidth;
		if (m_pSectD->SectBefore.nCellType == 0)
			bSaveSlabWidth = (m_pSectD->SectBefore.nCellShape <= 2);
		else
			bSaveSlabWidth = (m_pSectD->SectBefore.nCellShape <= 1);

		if (bSaveSlabWidth)
		{
			m_pSectD->SectBefore.SectI.Size[0] = m_pSectD->SectBefore.SectI.Size[6] +
																					 m_pSectD->SectBefore.SectI.Size[8] +
																					 m_pSectD->SectBefore.SectI.Size[9] +
																					 m_pSectD->SectBefore.SectI.Size[10] +
																					 m_pSectD->SectBefore.SectI.Size[11];
		}
	}
	// 대칭인 경우, 짝이 맞지 않으므로 특별 처리
	else if (m_pSectD->SectBefore.Shape == D_SECT_SHAPE_PSC_CMPWEB)
	{
		if (m_nSymmetryRow && m_bSymmetry)  // 대칭
		{
			m_pSectD->SectBefore.SectI.Size[32] = m_pSectD->SectBefore.SectI.Size[7];
			m_pSectD->SectBefore.SectI.Size[33] = m_pSectD->SectBefore.SectI.Size[8];
			m_pSectD->SectBefore.SectI.Size[34] = m_pSectD->SectBefore.SectI.Size[17];

			m_pSectD->SectBefore.SectJ.Size[32] = m_pSectD->SectBefore.SectJ.Size[7];
			m_pSectD->SectBefore.SectJ.Size[33] = m_pSectD->SectBefore.SectJ.Size[8];
			m_pSectD->SectBefore.SectJ.Size[34] = m_pSectD->SectBefore.SectJ.Size[17];
		}
	}

	return TRUE;
}

//---------------------------------------------------------------------------
// Implementation Function
BOOL CCMSectItemGrid4Composite::SetLeftValidCell(ROWCOL nRow, ROWCOL nCol)
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

BOOL CCMSectItemGrid4Composite::SetRightValidCell(ROWCOL nRow, ROWCOL nCol)
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

CString CCMSectItemGrid4Composite::GetEditingValue(ROWCOL nRow, ROWCOL nCol)
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
void CCMSectItemGrid4Composite::SetColumnStatus()
{
	CArray<BOOL, BOOL> aHideJoint;  // TRUE->Hide
	CArray<BOOL, BOOL> aHideSizeI;   // TRUE->Hide
	CArray<BOOL, BOOL> aHideSizeJ;   // TRUE->Hide
	SetColumnStatusGetStatus(aHideJoint, aHideSizeI, aHideSizeJ);

	BOOL bOldLock = LockUpdate(TRUE);
	
	if (m_nCellShapeRow > 0)
		HideRows(m_nCellShapeRow, m_nCellShapeRow, (m_nSymmetryRow && m_bSymmetry && m_nShapeIndex==3));

	if (m_nNumSideHole > 0)
	{
		HideRows(m_nSideHoleRow, m_nSideHoleRow, m_pSectD->SectBefore.nCellType==0);
	}

	if (m_nNumShearCheck > 0)
	{
		HideRows(m_nShearOptionRow, m_nShearOptionRow, TRUE); // Hide Always-사용안함(2005.3.25)

		if (m_bUseAutoShearCheck)
		{
			BOOL bHide;
			for (int n=0; n<2; n++)
			{
				bHide = !m_pSectD->SectBefore.bShearCheck;
				HideRows(m_nAutoShearCheckRow[n], m_nShearCheckRow[n], bHide);        
				bHide = (!m_pSectD->SectBefore.bShearCheck || m_pSectD->SectBefore.bAutoShearCheckPos[n][0]);
				HideRows(m_nShearCheckRow[n],     m_nShearCheckRow[n], bHide);

				HideRows(m_nAutoShearCheckRow[n]+2, m_nAutoShearCheckRow[n]+2, TRUE); // Hide Always
				HideRows(m_nShearCheckRow[n]+2,     m_nShearCheckRow[n]+2, TRUE); // Hide Always

				bHide = !m_pSectD->SectBefore.bShearCheck;
				HideRows(m_nAutoShearCheckRow[n]+4, m_nShearCheckRow[n]+4, bHide);
				bHide = (!m_pSectD->SectBefore.bShearCheck || m_pSectD->SectBefore.bAutoShearCheckPos[n][2]);
				HideRows(m_nShearCheckRow[n]+4,     m_nShearCheckRow[n]+4, bHide);
			}
		}
		else  // 옵션이 없는 경우 무조건 입력 받음 
		{
			for (int n=0; n<2; n++)
			{
				HideRows(m_nShearCheckRow[n]+1,  m_nShearCheckRow[n]+1, TRUE);  // Hide Always
				if (m_pSectD->SectBefore.Shape == D_SECT_SHAPE_PSC_CMPWEB)      // 파형강판인 경우
				{
					HideRows(m_nShearCheckRow[n]+0,  m_nShearCheckRow[n]+2, TRUE); // Hide Always
					HideRows(m_nShearCheckRow[n]+0,  m_nShearCheckRow[n]+2, TRUE); // Hide Always
				}
			}
		}
	}

	if (m_nNumShearQy > 0)
	{
		for (int n=0; n<2; n++)
		{
			HideRows(m_nShearQyRow[n],   m_nShearQyRow[n],   m_pSectD->SectBefore.bAutoQy[n][0]);
			HideRows(m_nShearQyRow[n]+2, m_nShearQyRow[n]+2, m_pSectD->SectBefore.bAutoQy[n][1]);
			HideRows(m_nShearQyRow[n]+4, m_nShearQyRow[n]+4, m_pSectD->SectBefore.bAutoQy[n][2]);
		}
	}

	if (m_nNumMinWebThik > 0)
	{
		for (int n=0; n<2; n++)
		{
			HideRows(m_nMinWebThikRow[n],   m_nMinWebThikRow[n],   m_pSectD->SectBefore.bAutoWebThkShear[n][0]);
			HideRows(m_nMinWebThikRow[n]+2, m_nMinWebThikRow[n]+2, m_pSectD->SectBefore.bAutoWebThkShear[n][1]);
			HideRows(m_nMinWebThikRow[n]+4, m_nMinWebThikRow[n]+4, m_pSectD->SectBefore.bAutoWebThkShear[n][2]);
			HideRows(m_nAutoMinWebThikRow[n]+6, m_nAutoMinWebThikRow[n]+6, !m_bUseAutoTorsionCheck);
			HideRows(m_nMinWebThikRow[n]+6, m_nMinWebThikRow[n]+6, m_bUseAutoTorsionCheck && m_pSectD->SectBefore.bAutoWebThk[n]);
		}
	}

	for (int i = 0; i < aHideJoint.GetSize(); i++)
	{
		SetColumnStatusShowHide(m_nJointRow+i, aHideJoint[i]);
	}
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
void CCMSectItemGrid4Composite::SetColumnStatusGetStatus(CArray<BOOL, BOOL>& raHideJoint, CArray<BOOL, BOOL>& raHideSizeI, CArray<BOOL, BOOL>& raHideSizeJ)
{
	raHideJoint.RemoveAll();
	raHideSizeI.RemoveAll();
	raHideSizeJ.RemoveAll();

	CArray<int, int> aType4Joint, aType4SizeI, aType4SizeJ; // 1:1개(Begin만), 2:2개(Begin, End), 3:n개(Begin부터End까지)
	CArray<int, int> aBegin4Joint, aBegin4SizeI,aBegin4SizeJ;
	CArray<int, int> aEnd4Joint, aEnd4SizeI, aEnd4SizeJ;

	CArray<BOOL, BOOL> aJF; // Joint Flag
	int nPos = 0x01;
	aJF.SetSize(m_nNumJoint);
	for (int i = 0; i < m_nNumJoint; i++)
	{  
		aJF[i] = (m_nJoint & nPos) != 0;
		nPos <<= 1;
	}

	BOOL bIJSame = TRUE;  // 이 값이 TRUE면 I J가 같다고 보고.. I값을 J값으로 복사 함..

	switch(m_nShapeIndex)
	{
	case 0: // B
	case 1: // I
	case 6: // Tub
		break;
	case 7: // Stl Girder Box
		{
			bIJSame = FALSE;

			if(m_bSymmetryStlGSectI)
			{ 
				aBegin4SizeI.Add(1);  aEnd4SizeI.Add(1);  aType4SizeI.Add(1); // Sg
				aBegin4SizeI.Add(2);  aEnd4SizeI.Add(2);  aType4SizeI.Add(1); // Top
				aBegin4SizeI.Add(3);  aEnd4SizeI.Add(3);  aType4SizeI.Add(1); // Bot
				aBegin4SizeI.Add(6);  aEnd4SizeI.Add(6);  aType4SizeI.Add(1); // B3
				aBegin4SizeI.Add(9);  aEnd4SizeI.Add(9);  aType4SizeI.Add(1); // B6
				aBegin4SizeI.Add(14); aEnd4SizeI.Add(14); aType4SizeI.Add(1); // tw2
			}
			
			if(m_bSymmetryStlGSectJ)
			{	
				aBegin4SizeJ.Add(1);  aEnd4SizeJ.Add(1);  aType4SizeJ.Add(1); // Sg
				aBegin4SizeJ.Add(2);  aEnd4SizeJ.Add(2);  aType4SizeJ.Add(1); // Top
				aBegin4SizeJ.Add(3);  aEnd4SizeJ.Add(3);  aType4SizeJ.Add(1); // Bot
				aBegin4SizeJ.Add(6);  aEnd4SizeJ.Add(6);  aType4SizeJ.Add(1); // B3
				aBegin4SizeJ.Add(9);  aEnd4SizeJ.Add(9);  aType4SizeJ.Add(1); // B6
				aBegin4SizeJ.Add(14); aEnd4SizeJ.Add(14); aType4SizeJ.Add(1); // tw2
			}
		}
		break;
	case 8: // Stl Girder I
		{
			bIJSame = FALSE;
			
			if(m_bSymmetryStlGSectI)
			{ 
				aBegin4SizeI.Add(1);  aEnd4SizeI.Add(1);  aType4SizeI.Add(1); // Sg
				aBegin4SizeI.Add(2);  aEnd4SizeI.Add(2);  aType4SizeI.Add(1); // Top
				aBegin4SizeI.Add(3);  aEnd4SizeI.Add(3);  aType4SizeI.Add(1); // Bot
				aBegin4SizeI.Add(5);  aEnd4SizeI.Add(5);  aType4SizeI.Add(1); // B2
				aBegin4SizeI.Add(7);  aEnd4SizeI.Add(7);  aType4SizeI.Add(1); // B4
			}
			
			if(m_bSymmetryStlGSectJ)
			{	
				aBegin4SizeJ.Add(1);  aEnd4SizeJ.Add(1);  aType4SizeJ.Add(1); // Sg
				aBegin4SizeJ.Add(2);  aEnd4SizeJ.Add(2);  aType4SizeJ.Add(1); // Top
				aBegin4SizeJ.Add(3);  aEnd4SizeJ.Add(3);  aType4SizeJ.Add(1); // Bot
				aBegin4SizeJ.Add(5);  aEnd4SizeJ.Add(5);  aType4SizeJ.Add(1); // B2
				aBegin4SizeJ.Add(7);  aEnd4SizeJ.Add(7);  aType4SizeJ.Add(1); // B4
			}
		}
		break;
	case 9: // Stl Girder Tub
		{
			bIJSame = FALSE;

			if(m_bSymmetryStlGSectI)
			{        
				aBegin4SizeI.Add(1);  aEnd4SizeI.Add(1);  aType4SizeI.Add(1); // Sg
				aBegin4SizeI.Add(2);  aEnd4SizeI.Add(2);  aType4SizeI.Add(1); // Top
				aBegin4SizeI.Add(3);  aEnd4SizeI.Add(3);  aType4SizeI.Add(1); // Bot
				aBegin4SizeI.Add(6);  aEnd4SizeI.Add(6);  aType4SizeI.Add(1); // B3
				aBegin4SizeI.Add(9);  aEnd4SizeI.Add(9);  aType4SizeI.Add(1); // B6
				aBegin4SizeI.Add(14); aEnd4SizeI.Add(14); aType4SizeI.Add(1); // tw2
				aBegin4SizeI.Add(16); aEnd4SizeI.Add(16); aType4SizeI.Add(1); // bf2
			}
			if(m_bSymmetryStlGSectJ)
			{
				aBegin4SizeJ.Add(1);  aEnd4SizeJ.Add(1);  aType4SizeJ.Add(1); // Sg
				aBegin4SizeJ.Add(2);  aEnd4SizeJ.Add(2);  aType4SizeJ.Add(1); // Top
				aBegin4SizeJ.Add(3);  aEnd4SizeJ.Add(3);  aType4SizeJ.Add(1); // Bot
				aBegin4SizeJ.Add(6);  aEnd4SizeJ.Add(6);  aType4SizeJ.Add(1); // B3
				aBegin4SizeJ.Add(9);  aEnd4SizeJ.Add(9);  aType4SizeJ.Add(1); // B6
				aBegin4SizeJ.Add(14); aEnd4SizeJ.Add(14); aType4SizeJ.Add(1); // tw2
				aBegin4SizeJ.Add(16); aEnd4SizeJ.Add(16); aType4SizeJ.Add(1); // bf2
			}
		}
		break;
	case 2: // CI
		if(m_nSymmetryRow && m_bSymmetry)  // 대칭이면 
		{
			aBegin4Joint.Add(5); aEnd4Joint.Add(8); aType4Joint.Add(3);
			aBegin4SizeI.Add(1+17); aEnd4SizeI.Add(1+32); aType4SizeI.Add(3);
		}
		if (!aJF[0]) { aBegin4SizeI.Add(1+0);  aEnd4SizeI.Add(1+0);  aType4SizeI.Add(1); }
		if (!aJF[1]) { aBegin4SizeI.Add(1+3);  aEnd4SizeI.Add(1+12); aType4SizeI.Add(2); }
		if (!aJF[2]) { aBegin4SizeI.Add(1+4);  aEnd4SizeI.Add(1+13); aType4SizeI.Add(2); }
		if (!aJF[3]) { aBegin4SizeI.Add(1+8);  aEnd4SizeI.Add(1+16); aType4SizeI.Add(2); }
		if (!aJF[4]) { aBegin4SizeI.Add(1+7);  aEnd4SizeI.Add(1+15); aType4SizeI.Add(2); }
		if (!aJF[5]) { aBegin4SizeI.Add(1+19); aEnd4SizeI.Add(1+28); aType4SizeI.Add(2); }
		if (!aJF[6]) { aBegin4SizeI.Add(1+20); aEnd4SizeI.Add(1+29); aType4SizeI.Add(2); }
		if (!aJF[7]) { aBegin4SizeI.Add(1+24); aEnd4SizeI.Add(1+32); aType4SizeI.Add(2); }
		if (!aJF[8]) { aBegin4SizeI.Add(1+23); aEnd4SizeI.Add(1+31); aType4SizeI.Add(2); }
		break;
	case 3: // CT
		if(m_nSymmetryRow && m_bSymmetry)  // 대칭이면 
		{
			aBegin4Joint.Add(5); aEnd4Joint.Add(8); aType4Joint.Add(3);
			aBegin4SizeI.Add(1+16); aEnd4SizeI.Add(1+30); aType4SizeI.Add(3);
		}
		if (!aJF[0]) { aBegin4SizeI.Add(1+0);  aEnd4SizeI.Add(1+0);  aType4SizeI.Add(1); }
		if (!aJF[1]) { aBegin4SizeI.Add(1+8);  aEnd4SizeI.Add(1+14); aType4SizeI.Add(2); }
		if (!aJF[2]) { aBegin4SizeI.Add(1+9);  aEnd4SizeI.Add(1+15); aType4SizeI.Add(2); }
		if (!aJF[3]) { aBegin4SizeI.Add(1+10); aEnd4SizeI.Add(1+12); aType4SizeI.Add(2); }
		if (!aJF[4]) { aBegin4SizeI.Add(1+11); aEnd4SizeI.Add(1+13); aType4SizeI.Add(2); }
		if (!aJF[5]) { aBegin4SizeI.Add(1+23); aEnd4SizeI.Add(1+29); aType4SizeI.Add(2); }
		if (!aJF[6]) { aBegin4SizeI.Add(1+24); aEnd4SizeI.Add(1+30); aType4SizeI.Add(2); }
		if (!aJF[7]) { aBegin4SizeI.Add(1+25); aEnd4SizeI.Add(1+27); aType4SizeI.Add(2); }
		if (!aJF[8]) { aBegin4SizeI.Add(1+26); aEnd4SizeI.Add(1+28); aType4SizeI.Add(2); }
		break;

	// Old PSC Data - 임시로 100을 더해 놓았음
	case 100: // 1cell
	case 101: // 2cell
		if (!aJF[0]) { aBegin4SizeI.Add(2); aEnd4SizeI.Add(7); aType4SizeI.Add(2); }
		if (!aJF[1]) { aBegin4SizeI.Add(3); aEnd4SizeI.Add(8); aType4SizeI.Add(2); }
		if (!aJF[2]) { aBegin4SizeI.Add(5); aEnd4SizeI.Add(10); aType4SizeI.Add(2); }
		if (!aJF[3]) { aBegin4SizeI.Add(14); aEnd4SizeI.Add(23); aType4SizeI.Add(2); }
		if (!aJF[4]) { aBegin4SizeI.Add(15); aEnd4SizeI.Add(24); aType4SizeI.Add(2); }
		if (!aJF[5]) { aBegin4SizeI.Add(17); aEnd4SizeI.Add(25); aType4SizeI.Add(2); }
		if (!aJF[6]) { aBegin4SizeI.Add(20); aEnd4SizeI.Add(28); aType4SizeI.Add(2); }
		if (!aJF[7]) { aBegin4SizeI.Add(19); aEnd4SizeI.Add(27); aType4SizeI.Add(2); }
		break;
	case 102: // 3cell
		if (!aJF[0]) { aBegin4SizeI.Add(22); aEnd4SizeI.Add(35); aType4SizeI.Add(2); }
		if (!aJF[1]) { aBegin4SizeI.Add(23); aEnd4SizeI.Add(36); aType4SizeI.Add(2); }
		if (!aJF[2]) { aBegin4SizeI.Add(24); aEnd4SizeI.Add(37); aType4SizeI.Add(2); }
		if (!aJF[3]) { aBegin4SizeI.Add(25); aEnd4SizeI.Add(38); aType4SizeI.Add(2); }
		if (!aJF[4]) { aBegin4SizeI.Add(26); aEnd4SizeI.Add(39); aType4SizeI.Add(2); }
		if (!aJF[5]) { aBegin4SizeI.Add(27); aEnd4SizeI.Add(40); aType4SizeI.Add(2); }
		if (!aJF[6]) { aBegin4SizeI.Add(28); aEnd4SizeI.Add(41); aType4SizeI.Add(2); }
		if (!aJF[7]) { aBegin4SizeI.Add(29); aEnd4SizeI.Add(42); aType4SizeI.Add(2); }
		if (!aJF[8]) { aBegin4SizeI.Add(30); aEnd4SizeI.Add(43); aType4SizeI.Add(2); }
		if (!aJF[9]) { aBegin4SizeI.Add(31); aEnd4SizeI.Add(44); aType4SizeI.Add(2); }
		if (!aJF[10]) { aBegin4SizeI.Add(32); aEnd4SizeI.Add(45); aType4SizeI.Add(2); }
		if (!aJF[11]) { aBegin4SizeI.Add(33); aEnd4SizeI.Add(46); aType4SizeI.Add(2); }
		if (!aJF[12]) { aBegin4SizeI.Add(34); aEnd4SizeI.Add(47); aType4SizeI.Add(2); }
		break;
	case 108: // 4cell
		if (m_pSectD->SectBefore.nCellType == 0)
		{ aBegin4SizeI.Add(11); aEnd4SizeI.Add(12); aType4SizeI.Add(3); }
		else if (m_pSectD->SectBefore.nCellType == 1)
		{ aBegin4SizeI.Add(12); aEnd4SizeI.Add(12); aType4SizeI.Add(1); }
		break;
		// *^^*
	case 109: // multi-cell
		if(m_nSymmetryRow && m_bSymmetry)  // 대칭이면 
		{
			aBegin4SizeI.Add(30); aEnd4SizeI.Add(58); aType4SizeI.Add(3);
		}
		if (!aJF[0]) 
		{ 
			aBegin4SizeI.Add(3); aEnd4SizeI.Add(7); aType4SizeI.Add(2); 
			aBegin4SizeI.Add(32); aEnd4SizeI.Add(36); aType4SizeI.Add(2);
		}
		switch(m_pSectD->SectBefore.nCellType)
		{
		case 0:   // polygon
			if (!aJF[1]) 
			{ 
				aBegin4SizeI.Add(14); aEnd4SizeI.Add(21); aType4SizeI.Add(2); 
				aBegin4SizeI.Add(43); aEnd4SizeI.Add(50); aType4SizeI.Add(2);
			}
			if (m_pSectD->SectBefore.nCellShape == 1)
			{
				aBegin4SizeI.Add(0); aEnd4SizeI.Add(0); aType4SizeI.Add(1); 
				aBegin4SizeI.Add(15); aEnd4SizeI.Add(16); aType4SizeI.Add(2); 
				aBegin4SizeI.Add(22); aEnd4SizeI.Add(23); aType4SizeI.Add(2); 
				aBegin4SizeI.Add(25); aEnd4SizeI.Add(27); aType4SizeI.Add(2); 
				aBegin4SizeI.Add(34); aEnd4SizeI.Add(41); aType4SizeI.Add(2); 
				aBegin4SizeI.Add(44); aEnd4SizeI.Add(45); aType4SizeI.Add(2); 
				aBegin4SizeI.Add(47); aEnd4SizeI.Add(47); aType4SizeI.Add(1); 
				aBegin4SizeI.Add(51); aEnd4SizeI.Add(52); aType4SizeI.Add(2); 
				aBegin4SizeI.Add(54); aEnd4SizeI.Add(56); aType4SizeI.Add(3); 
			}
			else if (m_pSectD->SectBefore.nCellShape == 2)
			{
				aBegin4SizeI.Add(0); aEnd4SizeI.Add(0); aType4SizeI.Add(1); 
				aBegin4SizeI.Add(26); aEnd4SizeI.Add(27); aType4SizeI.Add(2); 
				aBegin4SizeI.Add(34); aEnd4SizeI.Add(41); aType4SizeI.Add(2); 
				aBegin4SizeI.Add(44); aEnd4SizeI.Add(45); aType4SizeI.Add(2); 
				aBegin4SizeI.Add(47); aEnd4SizeI.Add(47); aType4SizeI.Add(1); 
				aBegin4SizeI.Add(54); aEnd4SizeI.Add(56); aType4SizeI.Add(3); 
			}
			else
			{
				aBegin4SizeI.Add(34); aEnd4SizeI.Add(41); aType4SizeI.Add(2); 
				aBegin4SizeI.Add(44); aEnd4SizeI.Add(45); aType4SizeI.Add(2); 
				aBegin4SizeI.Add(47); aEnd4SizeI.Add(47); aType4SizeI.Add(1); 
				aBegin4SizeI.Add(54); aEnd4SizeI.Add(56); aType4SizeI.Add(3); 
			}
			break;
		case 1:   // circle
			aBegin4Joint.Add(1); aEnd4Joint.Add(1); aType4Joint.Add(1);
			if (!m_pSectD->SectBefore.bSmallHole)
			{
				aBegin4SizeI.Add(15); aEnd4SizeI.Add(19); aType4SizeI.Add(2); 
				aBegin4SizeI.Add(20); aEnd4SizeI.Add(44); aType4SizeI.Add(2); 
				aBegin4SizeI.Add(48); aEnd4SizeI.Add(49); aType4SizeI.Add(2); 
			}
			aBegin4SizeI.Add(14); aEnd4SizeI.Add(14); aType4SizeI.Add(1); 
			aBegin4SizeI.Add(16); aEnd4SizeI.Add(18); aType4SizeI.Add(3); 
			aBegin4SizeI.Add(21); aEnd4SizeI.Add(27); aType4SizeI.Add(3); 
			aBegin4SizeI.Add(34); aEnd4SizeI.Add(34); aType4SizeI.Add(1); 
			aBegin4SizeI.Add(41); aEnd4SizeI.Add(43); aType4SizeI.Add(3); 
			aBegin4SizeI.Add(45); aEnd4SizeI.Add(47); aType4SizeI.Add(3); 
			aBegin4SizeI.Add(50); aEnd4SizeI.Add(56); aType4SizeI.Add(3); 
			if (m_pSectD->SectBefore.nCellShape <= 1)
			{
				aBegin4SizeI.Add(0); aEnd4SizeI.Add(0); aType4SizeI.Add(1); 
			}
			break;
		}
		break;
	case 103: // mid
		if(m_nSymmetryRow && m_bSymmetry)  // 대칭이면 
		{
			aBegin4Joint.Add(5); aEnd4Joint.Add(8); aType4Joint.Add(3);
			aBegin4SizeI.Add(16); aEnd4SizeI.Add(30); aType4SizeI.Add(3);
		}
		if (!aJF[0]) { aBegin4SizeI.Add(0); aEnd4SizeI.Add(0); aType4SizeI.Add(1); }
		switch(m_pSectD->SectBefore.nCellType)
		{
		case 0: // none
			aBegin4Joint.Add(1); aEnd4Joint.Add(4); aType4Joint.Add(3);
			aBegin4SizeI.Add(2); aEnd4SizeI.Add(10); aType4SizeI.Add(3);
			aBegin4SizeI.Add(12); aEnd4SizeI.Add(15); aType4SizeI.Add(3);
			break;
		case 1: // circle
			aBegin4Joint.Add(1); aEnd4Joint.Add(4); aType4Joint.Add(3);
			aBegin4SizeI.Add(2); aEnd4SizeI.Add(4); aType4SizeI.Add(3);
			aBegin4SizeI.Add(6); aEnd4SizeI.Add(8); aType4SizeI.Add(3);
			aBegin4SizeI.Add(10); aEnd4SizeI.Add(10); aType4SizeI.Add(1);
			aBegin4SizeI.Add(12); aEnd4SizeI.Add(15); aType4SizeI.Add(3);
			break;
		case 2: // polygon
			if (!aJF[1]) { aBegin4SizeI.Add(3); aEnd4SizeI.Add(12); aType4SizeI.Add(2); }
			if (!aJF[2]) { aBegin4SizeI.Add(4); aEnd4SizeI.Add(13); aType4SizeI.Add(2); }
			if (!aJF[3]) { aBegin4SizeI.Add(8); aEnd4SizeI.Add(15); aType4SizeI.Add(2); }
			if (!aJF[4]) { aBegin4SizeI.Add(7); aEnd4SizeI.Add(14); aType4SizeI.Add(2); }
			break;
		}
		switch(m_pSectD->SectBefore.nCellShape)
		{
		case 0: // none
			aBegin4Joint.Add(5); aEnd4Joint.Add(8); aType4Joint.Add(3);
			aBegin4SizeI.Add(17); aEnd4SizeI.Add(25); aType4SizeI.Add(3);
			aBegin4SizeI.Add(27); aEnd4SizeI.Add(30); aType4SizeI.Add(3);
			break;
		case 1: // circle
			aBegin4Joint.Add(5); aEnd4Joint.Add(8); aType4Joint.Add(3);
			aBegin4SizeI.Add(17); aEnd4SizeI.Add(19); aType4SizeI.Add(3);
			aBegin4SizeI.Add(21); aEnd4SizeI.Add(23); aType4SizeI.Add(3);
			aBegin4SizeI.Add(25); aEnd4SizeI.Add(25); aType4SizeI.Add(1);
			aBegin4SizeI.Add(27); aEnd4SizeI.Add(30); aType4SizeI.Add(3);
			break;
		case 2: // polygon
			if (!aJF[5]) { aBegin4SizeI.Add(18); aEnd4SizeI.Add(27); aType4SizeI.Add(2); }
			if (!aJF[6]) { aBegin4SizeI.Add(19); aEnd4SizeI.Add(28); aType4SizeI.Add(2); }
			if (!aJF[7]) { aBegin4SizeI.Add(23); aEnd4SizeI.Add(30); aType4SizeI.Add(2); }
			if (!aJF[8]) { aBegin4SizeI.Add(22); aEnd4SizeI.Add(29); aType4SizeI.Add(2); }
			break;
		}
		break;
	case 104: // half
		if (!aJF[0]) { aBegin4SizeI.Add(2); aEnd4SizeI.Add(7); aType4SizeI.Add(2); }
		if (!aJF[1]) { aBegin4SizeI.Add(3); aEnd4SizeI.Add(8); aType4SizeI.Add(2); }
		if (!aJF[2]) { aBegin4SizeI.Add(5); aEnd4SizeI.Add(10); aType4SizeI.Add(2); }
		switch(m_pSectD->SectBefore.nCellShape)
		{
		case 0: // none
			aBegin4Joint.Add(3); aEnd4Joint.Add(7); aType4Joint.Add(3);
			aBegin4SizeI.Add(13); aEnd4SizeI.Add(28); aType4SizeI.Add(3);
			break;
		case 1: // circle
			aBegin4Joint.Add(3); aEnd4Joint.Add(7); aType4Joint.Add(3);
			aBegin4SizeI.Add(13); aEnd4SizeI.Add(15); aType4SizeI.Add(3);
			aBegin4SizeI.Add(17); aEnd4SizeI.Add(20); aType4SizeI.Add(3);
			aBegin4SizeI.Add(22); aEnd4SizeI.Add(28); aType4SizeI.Add(3);
			break;
		case 2: // polygon
			if (!aJF[3]) { aBegin4SizeI.Add(14); aEnd4SizeI.Add(23); aType4SizeI.Add(2); }
			if (!aJF[4]) { aBegin4SizeI.Add(15); aEnd4SizeI.Add(24); aType4SizeI.Add(2); }
			if (!aJF[5]) { aBegin4SizeI.Add(17); aEnd4SizeI.Add(25); aType4SizeI.Add(2); }
			if (!aJF[6]) { aBegin4SizeI.Add(20); aEnd4SizeI.Add(28); aType4SizeI.Add(2); }
			if (!aJF[7]) { aBegin4SizeI.Add(19); aEnd4SizeI.Add(27); aType4SizeI.Add(2); }
			break;
		}
		break;
	case 105: // tee
		if(m_nSymmetryRow && m_bSymmetry)  // 대칭이면 
		{
			aBegin4Joint.Add(5); aEnd4Joint.Add(8); aType4Joint.Add(3);
			aBegin4SizeI.Add(16); aEnd4SizeI.Add(30); aType4SizeI.Add(3);
		}
		if (!aJF[0]) { aBegin4SizeI.Add(0); aEnd4SizeI.Add(0); aType4SizeI.Add(1); }
		if (!aJF[1]) { aBegin4SizeI.Add(8); aEnd4SizeI.Add(14); aType4SizeI.Add(2); }
		if (!aJF[2]) { aBegin4SizeI.Add(9); aEnd4SizeI.Add(15); aType4SizeI.Add(2); }
		if (!aJF[3]) { aBegin4SizeI.Add(10); aEnd4SizeI.Add(12); aType4SizeI.Add(2); }
		if (!aJF[4]) { aBegin4SizeI.Add(11); aEnd4SizeI.Add(13); aType4SizeI.Add(2); }
		if (!aJF[5]) { aBegin4SizeI.Add(23); aEnd4SizeI.Add(29); aType4SizeI.Add(2); }
		if (!aJF[6]) { aBegin4SizeI.Add(24); aEnd4SizeI.Add(30); aType4SizeI.Add(2); }
		if (!aJF[7]) { aBegin4SizeI.Add(25); aEnd4SizeI.Add(27); aType4SizeI.Add(2); }
		if (!aJF[8]) { aBegin4SizeI.Add(26); aEnd4SizeI.Add(28); aType4SizeI.Add(2); }
		break;
	case 106: // plat
		if(m_nSymmetryRow && m_bSymmetry)  // 대칭이면 
		{
			aBegin4Joint.Add(1); aEnd4Joint.Add(1); aType4Joint.Add(1);
			aBegin4SizeI.Add(16); aEnd4SizeI.Add(28); aType4SizeI.Add(3);
		}
		switch(m_pSectD->SectBefore.nCellType)
		{
		case 0: // half
			if (m_pSectD->SectBefore.nCellShape == 0) // left
			{ 
				aBegin4Joint.Add(1); aEnd4Joint.Add(1); aType4Joint.Add(1); 
				aBegin4SizeI.Add(10); aEnd4SizeI.Add(28); aType4SizeI.Add(3);
				if (!aJF[0]) { aBegin4SizeI.Add(8); aEnd4SizeI.Add(9); aType4SizeI.Add(2); }
			}
			else  // right
			{ 
				aBegin4Joint.Add(0); aEnd4Joint.Add(0); aType4Joint.Add(0); 
				aBegin4SizeI.Add(2); aEnd4SizeI.Add(15); aType4SizeI.Add(3);
				aBegin4SizeI.Add(24); aEnd4SizeI.Add(28); aType4SizeI.Add(3);
				if (!aJF[1]) { aBegin4SizeI.Add(22); aEnd4SizeI.Add(23); aType4SizeI.Add(2); }
			}
			break;
		case 1: // 1cell
			if (!aJF[0]) { aBegin4SizeI.Add(8); aEnd4SizeI.Add(9); aType4SizeI.Add(2); }
			if (!aJF[1]) { aBegin4SizeI.Add(22); aEnd4SizeI.Add(23); aType4SizeI.Add(2); }
			if (m_pSectD->SectBefore.nCellShape == 1) // circle
			{
				aBegin4SizeI.Add(10); aEnd4SizeI.Add(15); aType4SizeI.Add(3);
				aBegin4SizeI.Add(24); aEnd4SizeI.Add(28); aType4SizeI.Add(3);
			}
			else  // polygon
			{
				aBegin4SizeI.Add(15); aEnd4SizeI.Add(15); aType4SizeI.Add(1);
			}
			break;
		case 2: // 2cell
			if (!aJF[0]) { aBegin4SizeI.Add(8); aEnd4SizeI.Add(9); aType4SizeI.Add(2); }
			if (!aJF[1]) { aBegin4SizeI.Add(22); aEnd4SizeI.Add(23); aType4SizeI.Add(2); }
			aBegin4SizeI.Add(10); aEnd4SizeI.Add(14); aType4SizeI.Add(3);
			aBegin4SizeI.Add(24); aEnd4SizeI.Add(28); aType4SizeI.Add(3);
			break;
		}
		break;
	case 107: // I
		if(m_nSymmetryRow && m_bSymmetry)  // 대칭이면 
		{
			aBegin4Joint.Add(5); aEnd4Joint.Add(8); aType4Joint.Add(3);
			aBegin4SizeI.Add(17); aEnd4SizeI.Add(32); aType4SizeI.Add(3);
		}
		if (!aJF[0]) { aBegin4SizeI.Add(0); aEnd4SizeI.Add(0); aType4SizeI.Add(1); }
		if (!aJF[1]) { aBegin4SizeI.Add(3); aEnd4SizeI.Add(12); aType4SizeI.Add(2); }
		if (!aJF[2]) { aBegin4SizeI.Add(4); aEnd4SizeI.Add(13); aType4SizeI.Add(2); }
		if (!aJF[3]) { aBegin4SizeI.Add(8); aEnd4SizeI.Add(16); aType4SizeI.Add(2); }
		if (!aJF[4]) { aBegin4SizeI.Add(7); aEnd4SizeI.Add(15); aType4SizeI.Add(2); }
		if (!aJF[5]) { aBegin4SizeI.Add(19); aEnd4SizeI.Add(28); aType4SizeI.Add(2); }
		if (!aJF[6]) { aBegin4SizeI.Add(20); aEnd4SizeI.Add(29); aType4SizeI.Add(2); }
		if (!aJF[7]) { aBegin4SizeI.Add(24); aEnd4SizeI.Add(32); aType4SizeI.Add(2); }
		if (!aJF[8]) { aBegin4SizeI.Add(23); aEnd4SizeI.Add(31); aType4SizeI.Add(2); }
		break;
	case 110:  // value
		// Nothing
		break;
	case 111:   // CMPWEB
		if (m_nSymmetryRow && m_bSymmetry)  // 대칭
		{
			aBegin4SizeI.Add(32); aEnd4SizeI.Add(34); aType4SizeI.Add(3);
		}
		if (m_nHunchRow && !m_bHunch)
		{
			aBegin4SizeI.Add(3);  aEnd4SizeI.Add(6);  aType4SizeI.Add(2);
			aBegin4SizeI.Add(10); aEnd4SizeI.Add(11); aType4SizeI.Add(3);
			aBegin4SizeI.Add(15); aEnd4SizeI.Add(16); aType4SizeI.Add(2);
			aBegin4SizeI.Add(21); aEnd4SizeI.Add(22); aType4SizeI.Add(2);
		}
		aBegin4SizeI.Add(31); aEnd4SizeI.Add(31); aType4SizeI.Add(1);
		break;
	}
	
	if(bIJSame)
	{
		aType4SizeJ.RemoveAll();
		aType4SizeJ.Copy(aType4SizeI);

		aBegin4SizeJ.RemoveAll();
		aBegin4SizeJ.Copy(aBegin4SizeI);

		aEnd4SizeJ.RemoveAll();
		aEnd4SizeJ.Copy(aEnd4SizeI);
	}

	SetColumnStatusSetHideFlag(raHideJoint, m_nNumJoint, aType4Joint, aBegin4Joint, aEnd4Joint);
	SetColumnStatusSetHideFlag(raHideSizeI, m_nNumSize, aType4SizeI, aBegin4SizeI, aEnd4SizeI);
	SetColumnStatusSetHideFlag(raHideSizeJ, m_nNumSize, aType4SizeJ, aBegin4SizeJ, aEnd4SizeJ);

	if(m_nShapeIndex != 7 && m_nShapeIndex != 8 && m_nShapeIndex != 9) // Comp Stlg Box, Comp Stlg I, Comp Stlg Tub, Stlg Box, Stlg I 가 아닌 경우 (7,8,9 번은 0번째가 Symmetric 이므로 hide 하지 않음)
	{
		if(CSectDB::IsEnable7thDOF())
		{ //Tapered에서 Slab 정보 행 숨김.
			if(raHideSizeI.GetSize()>0) raHideSizeI[0]=TRUE;
			if(raHideSizeJ.GetSize()>0) raHideSizeJ[0]=TRUE;
		}
	}  
}

void CCMSectItemGrid4Composite::SetColumnStatusSetHideFlag(CArray<BOOL, BOOL>& raHideFlag, 
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

void CCMSectItemGrid4Composite::SetColumnStatusShowHide(ROWCOL nRow, BOOL bHide)
{
	SetStyleRange(CGXRange(nRow, 2), CGXStyle().SetEnabled(!bHide));
	HideRows(nRow, nRow, bHide);
}

BOOL CCMSectItemGrid4Composite::IsEditing()
{
	return GetEditingControl() != 0;
}

void CCMSectItemGrid4Composite::CancelEditing()
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

CGXControl* CCMSectItemGrid4Composite::GetEditingControl()
{
	ROWCOL ncRow, ncCol;
	if (GetCurrentCell(ncRow, ncCol))
	{
		CGXControl* pControl = GetControl(ncRow, ncCol);
		if (pControl && pControl->IsActive()) return pControl;
	}
	return 0;
}

BEGIN_MESSAGE_MAP(CCMSectItemGrid4Composite, CGXGridWnd)
	//{{AFX_MSG_MAP(CCMSectItemGrid4Composite)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CCMSectItemGrid4Composite message handlers
/////////////////////////////////////////////////////////////////////////////
BOOL CCMSectItemGrid4Composite::ProcessKeys(CWnd *pSender, UINT nMessage, UINT nChar, UINT nRepCnt, UINT flags)
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

BOOL CCMSectItemGrid4Composite::OnValidateCell(ROWCOL nRow, ROWCOL nCol)
{
	if ( (nRow >= m_nSizeIRow     && nRow < m_nSizeIRow+m_nNumSize)     ||
			 (nRow >= m_nSizeJRow     && nRow < m_nSizeJRow+m_nNumSize)     ||
			 (nRow >= m_nGirderRow    && nRow < m_nGirderRow+m_nNumGirder)  ||
			 (nRow >= m_nSlabRow      && nRow < m_nSlabRow+m_nNumSlab)      ||
			 (nRow >= m_nStiffenerRow && nRow < m_nStiffenerRow+m_nNumStiffener)
		 )
	{
		CString csNum = GetEditingValue(nRow, nCol);
		double dNum;
		if (!GetDoubleValue(csNum, dNum)) { m_bValidateFail = TRUE; return FALSE;}
	}

	// 정수만 입력받도록
	if ( nRow == m_nGirderRow || 
			 (m_nNumStiffener>0 && (nRow == m_nStiffenerRow || nRow == m_nStiffenerRow+3)) )
	{
		CString csNum = GetEditingValue(nRow, nCol);
		double dNum;
		if (!GetDoubleValue(csNum, dNum)) { m_bValidateFail = TRUE; return FALSE; }
		if (dNum != (int)dNum) 
		{
			SetWarningText(_LS(IDS_CMD0417__Error___Invalid_formula_));
			m_bValidateFail = TRUE; return FALSE;
		}
	}

	return TRUE;
}

BOOL CCMSectItemGrid4Composite::OnActivateGrid(BOOL bActivate)
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

void CCMSectItemGrid4Composite::OnModifyCell(ROWCOL nRow, ROWCOL nCol)
{
	CGXGridWnd::OnModifyCell(nRow, nCol);
	BOOL bShowData = FALSE;
	if (nRow == m_nCellTypeRow && nCol == 2)
	{
		if (m_pSectD->SectBefore.Shape == D_SECT_SHAPE_PSC_BOX)
		{
			CString csType = GetEditingValue(nRow, nCol);
			if (csType == _LS(IDS_CMD_PSC_GRID_Half)) { m_csValueCellShape = _LS(IDS_CMD_PSC_GRID_Left); m_nKindCellShape = 2; } // left/right
			else if (csType == _LS(IDS_CMD_PSC_GRID_1Cell)) { m_csValueCellShape = _LS(IDS_CMD_PSC_GRID_Circle); m_nKindCellShape = 3; } // Circle/Polygon
			else { m_csValueCellShape = _T(""); m_nKindCellShape = 2; } // left/right
			SetCobxColumn(CGXRange(m_nCellShapeRow, 2), GetCListCellShape(m_nKindCellShape));
			SetStyleRange(CGXRange(m_nCellShapeRow, 2), CGXStyle()
				.SetEnabled(m_csValueCellShape != _T(""))
				.SetValue(m_csValueCellShape));
			bShowData = TRUE;
		}
	}
	SaveData();  
	if (nRow == m_nCellTypeRow || nRow == m_nCellShapeRow)
	{
		if(bShowData) ShowData();
		else
		{
			SetViewData();
			SetColumnStatus();
		}
		if (SaveFlag(TRUE))
		{
			UINT nIndex = m_pParent->GetBitmapIndexFromInfoPSC();
			if(nIndex>=0) m_pParent->ChangeBitmapForViewer(nIndex);
		}
	}
	else if (nRow == m_nShearCheckRow[0] || nRow == m_nShearCheckRow[1])
	{
		SetViewData();
		SetColumnStatus();
	}

	if(m_nShapeIndex == 7 || m_nShapeIndex == 8 || m_nShapeIndex == 9)
	{
		// Stl Girder Composit일 때는 Symmetric일 때 AutoCalc를 수행을 위해 SaveData호출
		SaveData();
	}

	if (m_pPreview != 0) 
	{
		m_pPreview->SetCentroidFlag(FALSE);
		m_pPreview->Invalidate();
	}
}

BOOL CCMSectItemGrid4Composite::OnStartEditing(ROWCOL nRow, ROWCOL nCol)
{
	if (!CGXGridWnd::OnStartEditing(nRow, nCol)) return FALSE;

	BOOL bChanged = FALSE;
	// Joint Flag 변경되면 Data 저장하고 Show/Hide 다시 하고 그림 다시 그린다.
	if (nCol == 2 && nRow >= m_nJointRow && nRow < m_nJointRow + m_nNumJoint)
		bChanged = TRUE;
	// shear check position
	if (nCol == 3 && nRow == m_nShearOptionRow) 
		bChanged = TRUE;
	// side hole
	if (nCol == 2 && nRow == m_nSideHoleRow) 
		bChanged = TRUE;

	// auto shear check positon
	if (nRow == m_nAutoShearCheckRow[0]   || 
			nRow == m_nAutoShearCheckRow[0]+2 || 
			nRow == m_nAutoShearCheckRow[0]+4)
	{
		if (nCol == 2) bChanged = TRUE;
	}
	if (nRow == m_nAutoShearCheckRow[1]   || 
			nRow == m_nAutoShearCheckRow[1]+2 || 
			nRow == m_nAutoShearCheckRow[1]+4)
	{
		if (nCol == 2) bChanged = TRUE;
	}
	// auto Qy
	if (nRow == m_nAutoShearQyRow[0]   || 
			nRow == m_nAutoShearQyRow[0]+2 || 
			nRow == m_nAutoShearQyRow[0]+4)
	{
		if (nCol == 2) bChanged = TRUE;
	}
	if (nRow == m_nAutoShearQyRow[1]   || 
			nRow == m_nAutoShearQyRow[1]+2 || 
			nRow == m_nAutoShearQyRow[1]+4)
	{
		if (nCol == 2) bChanged = TRUE;
	}
	// auto min. web thick
	if (nRow == m_nAutoMinWebThikRow[0]+0 || 
			nRow == m_nAutoMinWebThikRow[0]+2 || 
			nRow == m_nAutoMinWebThikRow[0]+4 || 
			nRow == m_nAutoMinWebThikRow[0]+6)
	{
		if (nCol == 2) bChanged = TRUE;
	}
	if (nRow == m_nAutoMinWebThikRow[1]+0 || 
			nRow == m_nAutoMinWebThikRow[1]+2 ||
			nRow == m_nAutoMinWebThikRow[1]+4 ||
			nRow == m_nAutoMinWebThikRow[1]+6)
	{
		if (nCol == 2) bChanged = TRUE;
	}

	if (bChanged)
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

BOOL CCMSectItemGrid4Composite::OnPasteFromClipboard(const CGXRange &range)
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

void CCMSectItemGrid4Composite::OnClickedButtonRowCol(ROWCOL nRow, ROWCOL nCol)
{
	if (nRow == m_nSymmetryRow || nRow == m_nSymmetryStlGSectI || nRow == m_nSymmetryStlGSectJ)
	{
		CString csSymmetry = GetEditingValue(m_nSymmetryRow, 3);
		m_bSymmetry = (csSymmetry == _T("0")) ? FALSE : TRUE;
		m_pSectD->SectBefore.bSymmetric = m_bSymmetry;
		
		SetViewData();
		SetColumnStatus();

		if (SaveData())
		{
			UINT nIndex = m_pParent->GetBitmapIndexFromInfoComposite();
			if(nIndex>=0) m_pParent->ChangeBitmapForViewer(nIndex);
			if(m_pPreview != 0)
			{
				m_pPreview->SetCentroidFlag(FALSE);
				m_pPreview->Invalidate();
			}

			if((m_nShapeIndex == 7 || m_nShapeIndex == 8 || m_nShapeIndex == 9) &&
				nCol == 3)
			{
				// Composite Stl Girder일 때 Symmetriy 체크로 값 바뀐거 갱신
				ShowData();
			}
		}
		return;
	}
	else if (nRow == m_nHunchRow)
	{
		CString csHunch = GetEditingValue(m_nHunchRow, 3);
		m_bHunch = (csHunch == _T("0")) ? FALSE : TRUE;
		m_pSectD->SectBefore.bSmallHole = m_bHunch;
		
		SetViewData();
		SetColumnStatus();

		if (SaveData())
		{
			UINT nIndex = m_pParent->GetBitmapIndexFromInfoPSC();
			if(nIndex>=0) m_pParent->ChangeBitmapForViewer(nIndex);
			if(m_pPreview != 0)
			{
				m_pPreview->SetCentroidFlag(FALSE);
				m_pPreview->Invalidate();
			}
		}
		return;
	}  
	//else if(nRow == m_nStiffenerRow_I || nRow == m_nStiffenerRow_J)
	else if(nRow == m_nStiffenerRow2)
	{
		if(m_nShapeIndex == 7 || m_nShapeIndex == 8 || m_nShapeIndex == 9)
		{
			CCMSectSODDeckStiffenerDlg dlg;

			dlg.m_bTapJ = FALSE;

			dlg.m_Data = *m_pSectD;
			
			if(dlg.DoModal() == IDOK)
			{
				*m_pSectD = dlg.m_Data;
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
		}
		return;
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
		pSize = m_pSectD->CmpTapJ.Size;
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

	CCMSectItemImportComposite dlg;
	dlg.SetTargetShape(m_pSectD->SectBefore.Shape);
	dlg.m_bIncludeOption[0] = bIncludeOption[0];
	//dlg.m_bIncludeOption[1] = bIncludeOption[1];

	if (dlg.DoModal() == IDOK)
	{
		T_SECT_K key = dlg.m_SectKey;
		T_SECT_D SectData;
		CDBDoc* pDoc = CDBDoc::GetDocPoint();
		pDoc->m_pAttrCtrl->GetSect(key, SectData);

		// copy size
		m_pSectD->SectBefore.bSymmetric = SectData.SectBefore.bSymmetric;
		double nOldSlabWidth = pSize[59];
		memcpy(pSize, SectData.SectBefore.SectI.Size, sizeof(SectData.SectBefore.SectI.Size));

		// copy slab width
		if (csTitle == _LS(IDS_CMD_PSC_GRID_Size_I)) 
			m_pSectD->SectAfter.SectI.Size[0] = SectData.SectAfter.SectI.Size[0];
		else
			pSize[59] = SectData.SectAfter.SectI.Size[0]; 


		/*if (m_nNumCMPWEB > 0)
		{
			pSect->Design.TanA = SectData.SectBefore.SectI.Design.TanA;
			pSect->Design.Beta = SectData.SectBefore.SectI.Design.Beta;
		}*/
		// Stiffness도 복사(2004.12.11 PSC-Value 추가하면서) - Composite 에서는 쓰이지않으나 일단 남겨둠
		if (m_nNumSitff > 0)  
		{
			pSect->Stiffness = SectData.SectBefore.SectI.Stiffness;
			pSect->PeriIn = SectData.SectBefore.SectI.PeriIn;
			pSect->PeriOut = SectData.SectBefore.SectI.PeriOut;
			pSect->Design.YBar = SectData.SectBefore.SectI.Design.YBar;
			pSect->Design.ZBar = SectData.SectBefore.SectI.Design.ZBar;
			pSect->aOuterPolygon.Copy(SectData.SectBefore.SectI.aOuterPolygon);
			pSect->aInnerPolygon.Copy(SectData.SectBefore.SectI.aInnerPolygon);
		}
		if (dlg.m_bIncludeOption[0])   // Option1 : Joint, Type, Shape
		{
			//m_pSectD->SectBefore.SectI.BuiltUpFlag = SectData.SectBefore.SectI.BuiltUpFlag;
			//m_pSectD->SectBefore.nCellType = SectData.SectBefore.nCellType;
			//m_pSectD->SectBefore.nCellShape = SectData.SectBefore.nCellShape;
			m_SectBefore.nJoint = SectData.SectBefore.nJoint;
		}
		if (TRUE/*dlg.m_bIncludeOption[1]*/)    // Option2 : Shear Check, Min. Web Thickness
		{
			m_SectBefore.bShearCheck = SectData.SectBefore.bShearCheck;

			m_SectBefore.bAutoWebThk[nIorJ] = SectData.SectBefore.bAutoWebThk[0];
			m_SectBefore.dWebThk[nIorJ]     = SectData.SectBefore.dWebThk[0];

			for (int i = 0; i < 3; i++)
			{
				m_SectBefore.bAutoShearCheckPos[nIorJ][i] = SectData.SectBefore.bAutoShearCheckPos[0][i];
				m_SectBefore.dShearCheckPos[nIorJ][i]     = SectData.SectBefore.dShearCheckPos[0][i];

				m_SectBefore.bAutoQy[nIorJ][i]            = SectData.SectBefore.bAutoQy[0][i];
				m_SectBefore.dQy[nIorJ][i]                = SectData.SectBefore.dQy[0][i];

				m_SectBefore.bAutoWebThkShear[nIorJ][i]   = SectData.SectBefore.bAutoWebThkShear[0][i];
				m_SectBefore.dWebThkShear[nIorJ][i]       = SectData.SectBefore.dWebThkShear[0][i];
			}
		}

		if(m_nShapeIndex == 7 || m_nShapeIndex == 8 || m_nShapeIndex == 9)
		{
			// Symmetric, Slab Gap, Top, Bot값 불러오기
			if(nIorJ == 0)
			{
				m_pSectD->SectBefore.SectI.bSymAutoCalc = SectData.SectBefore.SectI.bSymAutoCalc;
				m_pSectD->SectAfter.SectI.Size[3]       = SectData.SectAfter.SectI.Size[3];
				m_pSectD->SectAfter.SectI.Size[4]       = SectData.SectAfter.SectI.Size[4];
				m_pSectD->SectAfter.SectI.Size[5]       = SectData.SectAfter.SectI.Size[5];
			}
			else
			{
				m_pSectD->CmpTapJ.bSymAutoCalc          = SectData.SectBefore.SectI.bSymAutoCalc;
				m_pSectD->SectAfter.SectJ.Size[3]       = SectData.SectAfter.SectI.Size[3];
				m_pSectD->SectAfter.SectJ.Size[4]       = SectData.SectAfter.SectI.Size[4];
				m_pSectD->SectAfter.SectJ.Size[5]       = SectData.SectAfter.SectI.Size[5];
			}
		}

		ShowData();

		if (m_pSectD->nStype == D_SECT_TYPE_COMPO_CI || m_pSectD->nStype == D_SECT_TYPE_COMPO_CT ||
				m_pSectD->nStype == D_SECT_TYPE_COMPO_PC)
		{
			UINT nIndex = m_pParent->GetBitmapIndexFromInfoComposite();
			if(nIndex>=0) m_pParent->ChangeBitmapForViewer(nIndex);
		}
	} 
}

BOOL CCMSectItemGrid4Composite::GetIntValue(CString& csNum, int& nNum)
{
	CString csExpr;
	if (csNum != _T("") && csNum.GetAt(0) == '=') csExpr = csNum;
	else csExpr = _T("=") + csNum;
	if (!ConvertFormulaToValue(csExpr)) return FALSE;
	nNum = _ttoi(csExpr);
	return TRUE;
}

BOOL CCMSectItemGrid4Composite::GetDoubleValue(CString& csNum, double& dNum)
{
	CString csExpr;
	if (csNum != _T("") && csNum.GetAt(0) == '=') csExpr = csNum;
	else csExpr = _T("=") + csNum;
	if (!ConvertFormulaToValue(csExpr)) return FALSE;
	dNum = _tstof(csExpr);
	return TRUE;
}

BOOL CCMSectItemGrid4Composite::ConvertFormulaToValue(CString& value)
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

void CCMSectItemGrid4Composite::SetCobxColumn(CGXRange& rRange, LPCTSTR lpszChoiceList)
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

CString CCMSectItemGrid4Composite::GetCListCellType(int nKindCellType)
{
	CString csCListCellType;
	if (nKindCellType == 1) 
		csCListCellType.Format(_T("%s\n%s\n%s\n"), _LS(IDS_CMD_PSC_GRID_None), _LS(IDS_CMD_PSC_GRID_Circle), _LS(IDS_CMD_PSC_GRID_Polygon));
	else if (nKindCellType == 2) 
		csCListCellType.Format(_T("%s\n%s\n"), _LS(IDS_CMD_PSC_GRID_Left), _LS(IDS_CMD_PSC_GRID_Right));
	else if (nKindCellType == 3) 
		csCListCellType.Format(_T("%s\n%s\n%s\n"), _LS(IDS_CMD_PSC_GRID_Half), _LS(IDS_CMD_PSC_GRID_1Cell), _LS(IDS_CMD_PSC_GRID_2Cell));
	else if (nKindCellType == 4) 
		csCListCellType.Format(_T("%s\n%s\n%s\n"), _LS(IDS_CMD_PSC_GRID_Vertical), _LS(IDS_CMD_PSC_GRID_Slope), _LS(IDS_CMD_PSC_GRID_Chamfer));
	else if (nKindCellType == 5)
		csCListCellType.Format(_T("%s\n%s\n"), _LS(IDS_CMD_PSC_GRID_Polygon), _LS(IDS_CMD_PSC_GRID_Circle));
	else ASSERT(0);
	return csCListCellType;
}

CString CCMSectItemGrid4Composite::GetCListCellShape(int nKindCellShape)
{
	CString csCListCellShape;
	if (nKindCellShape == 1) 
		csCListCellShape.Format(_T("%s\n%s\n%s\n"), _LS(IDS_CMD_PSC_GRID_None), _LS(IDS_CMD_PSC_GRID_Circle), _LS(IDS_CMD_PSC_GRID_Polygon));
	else if (nKindCellShape == 2) 
		csCListCellShape.Format(_T("%s\n%s\n"), _LS(IDS_CMD_PSC_GRID_Left), _LS(IDS_CMD_PSC_GRID_Right));
	else if (nKindCellShape == 3) 
		csCListCellShape.Format(_T("%s\n%s\n"), _LS(IDS_CMD_PSC_GRID_Circle), _LS(IDS_CMD_PSC_GRID_Polygon));
	else ASSERT(0);
	return csCListCellShape;
}

int CCMSectItemGrid4Composite::GetGirderList(CStringArray& raGirderList, CStringArray& raGirderListU, CArray<double, double>& raGirderListV)
{
	CString csUnit;
	CUnitCtrl::GetUnitSystem(D_UNITSYS_BASE_LENGTH, csUnit); 

	raGirderList.RemoveAll();
	raGirderListU.RemoveAll();
	raGirderListV.RemoveAll();
	
	if (m_nShapeIndex == 0 || m_nShapeIndex == 1 || m_nShapeIndex == 2 || m_nShapeIndex ==3 || m_nShapeIndex == 6 ||
			m_nShapeIndex == 7 || m_nShapeIndex == 8 || m_nShapeIndex == 9)
	{
		raGirderList.Add(_T("Num"));
		raGirderList.Add(_T("CTC"));
		
		raGirderListU.Add(_T(""));
		raGirderListU.Add(csUnit);

		raGirderListV.Add(m_pSectD->SectAfter.SectI.BuiltUpFlag);
		raGirderListV.Add(m_pSectD->SectAfter.SectI.Size[1]);
	}

	return raGirderList.GetSize();
}

int CCMSectItemGrid4Composite::GetSlabList(CStringArray& raSlabList, CStringArray& raSlabListU, CArray<double, double>& raSlabListV)
{
	CString csUnit;
	CUnitCtrl::GetUnitSystem(D_UNITSYS_BASE_LENGTH, csUnit); 

	raSlabList.RemoveAll();
	raSlabListU.RemoveAll();
	raSlabListV.RemoveAll();

	if (m_nShapeIndex == 0 || m_nShapeIndex == 1 || m_nShapeIndex == 2 || m_nShapeIndex ==3 || m_nShapeIndex == 6 ||
			m_nShapeIndex == 7 || m_nShapeIndex == 8 || m_nShapeIndex == 9)
	{
		raSlabList.Add(_T("Bc"));
		raSlabList.Add(_T("tc"));
		raSlabList.Add(_T("Hh"));
		raSlabList.Add(_T("Sg"));
		
		raSlabListU.Add(csUnit);
		raSlabListU.Add(csUnit);
		raSlabListU.Add(csUnit);
		raSlabListU.Add(csUnit);

		raSlabListV.Add(m_pSectD->SectAfter.SectJ.Size[0]);
		raSlabListV.Add(m_pSectD->SectAfter.SectJ.Size[1]);
		raSlabListV.Add(m_pSectD->SectAfter.SectJ.Size[2]);
		raSlabListV.Add(m_pSectD->SectAfter.SectJ.Size[3]);
	}

	return raSlabList.GetSize();
}

int CCMSectItemGrid4Composite::GetStiffenerList(CStringArray& raStiffenerList, CStringArray& raStiffenerListU, CArray<double, double>& raStiffenerListV)
{
	CString csUnit;
	CUnitCtrl::GetUnitSystem(D_UNITSYS_BASE_LENGTH, csUnit); 

	raStiffenerList.RemoveAll();
	raStiffenerListU.RemoveAll();
	raStiffenerListV.RemoveAll();

	if (m_nShapeIndex == 0)
	{
		raStiffenerList.Add(_T("N1"));
		raStiffenerList.Add(_T("Hr1"));
		raStiffenerList.Add(_T("tr1"));
		raStiffenerList.Add(_T("N2"));
		raStiffenerList.Add(_T("Hr2"));
		raStiffenerList.Add(_T("tr2"));
		
		raStiffenerListU.Add(_T(""));
		raStiffenerListU.Add(csUnit);
		raStiffenerListU.Add(csUnit);
		raStiffenerListU.Add(_T(""));
		raStiffenerListU.Add(csUnit);
		raStiffenerListU.Add(csUnit);

		raStiffenerListV.Add(m_pSectD->SectBefore.nStiffNum[0]);
		raStiffenerListV.Add(m_pSectD->SectBefore.SectJ.Size[2]);
		raStiffenerListV.Add(m_pSectD->SectBefore.SectJ.Size[4]);
		raStiffenerListV.Add(m_pSectD->SectBefore.nStiffNum[1]);
		raStiffenerListV.Add(m_pSectD->SectBefore.SectJ.Size[3]);
		raStiffenerListV.Add(m_pSectD->SectBefore.SectJ.Size[5]);
	}
	else if(m_nShapeIndex == 7) // Stl Girder Box
	{
		raStiffenerList.Add(_T("N1"));
		raStiffenerList.Add(_T("Hr1"));
		raStiffenerList.Add(_T("tr1"));
		raStiffenerList.Add(_T("N2"));
		raStiffenerList.Add(_T("Hr2"));
		raStiffenerList.Add(_T("tr2"));

		raStiffenerListU.Add(_T(""));
		raStiffenerListU.Add(csUnit);
		raStiffenerListU.Add(csUnit);
		raStiffenerListU.Add(_T(""));
		raStiffenerListU.Add(csUnit);
		raStiffenerListU.Add(csUnit);

		raStiffenerListV.Add(m_pSectD->SectBefore.nStiffNum[0]);
		raStiffenerListV.Add(m_pSectD->SectBefore.SectJ.Size[2]);
		raStiffenerListV.Add(m_pSectD->SectBefore.SectJ.Size[4]);
		raStiffenerListV.Add(m_pSectD->SectBefore.nStiffNum[1]);
		raStiffenerListV.Add(m_pSectD->SectBefore.SectJ.Size[3]);
		raStiffenerListV.Add(m_pSectD->SectBefore.SectJ.Size[5]);
	}

	return raStiffenerList.GetSize();
}

int CCMSectItemGrid4Composite::GetJointList(CStringArray& raJointList)
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
	else if(m_nJointType ==3)
	{
		for (int i = 0; i < m_nJointNum1; i++)
		{
			csJoint.Format(_T("JO"));
			raJointList.Add(csJoint);
		}
		for (int j = 0; j < m_nJointNum2; j++)
		{
			csJoint.Format(_T("JI"));
			raJointList.Add(csJoint);
		}
	}
	return raJointList.GetSize();
}

// *^^* 여기 추가 
int CCMSectItemGrid4Composite::GetSizeList(CStringArray& raSizeList)
{
#define _S(x) raSizeList.Add(#x)
	raSizeList.RemoveAll();
	if (m_nShapeIndex == 0) // B
	{
		raSizeList.Add(_T("Slab Width")); 
		_S(Hw); _S(tw); _S(B1); _S(Bf1); _S(tf1); _S(B2); _S(Bf2); _S(tf2);
	}
	else if (m_nShapeIndex == 1) // I
	{
		raSizeList.Add(_T("Slab Width")); 
		_S(Hw); _S(tw); _S(B1); _S(tf1); _S(B2); _S(tf2);
	}
	else if (m_nShapeIndex == 2) // CI
	{
		raSizeList.Add(_T("Slab Width")); 
		_S(H1);
		_S(HL1); _S(HL2); _S(HL2-1); _S(HL2-2); _S(HL3); _S(HL4); _S(HL4-1); _S(HL4-2); _S(HL5); 
		_S(BL1); _S(BL2); _S(BL2-1); _S(BL2-2); _S(BL4); _S(BL4-1); _S(BL4-2); 
		_S(HR1); _S(HR2); _S(HR2-1); _S(HR2-2); _S(HR3); _S(HR4); _S(HR4-1); _S(HR4-2); _S(HR5); 
		_S(BR1); _S(BR2); _S(BR2-1); _S(BR2-2); _S(BR4); _S(BR4-1); _S(BR4-2); 
	}
	else if (m_nShapeIndex == 3) // CT
	{
		raSizeList.Add(_T("Slab Width")); 
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
	else if (m_nShapeIndex == 6) // Tub
	{
		raSizeList.Add(_T("Slab Width")); 
		_S(Hw); _S(tw); _S(B1); _S(Bf1); _S(tf1); _S(B2); _S(Bf2); _S(tf2); _S(Bf3); _S(tfp);
	}
	else if (m_nShapeIndex == 7) // Stl Girder B
	{
		_S(Symmetry); _S(Slab Gap); raSizeList.Add(_LS(IDS_CMD_SECT_SOD_TOP)); raSizeList.Add(_LS(IDS_CMD_SECT_SOD_BOTTOM)); _S(B1); _S(B2); _S(B3); _S(B4); _S(B5); _S(B6); _S(H); _S(t1); _S(t2); _S(tw1); _S(tw2);
	}
	else if (m_nShapeIndex == 8) // Stl Girder I
	{
		_S(Symmetry); _S(Slab Gap); raSizeList.Add(_LS(IDS_CMD_SECT_SOD_TOP)); raSizeList.Add(_LS(IDS_CMD_SECT_SOD_BOTTOM)); _S(B1); _S(B2); _S(B3); _S(B4); _S(H); _S(t1); _S(t2); _S(tw);
	}
	else if (m_nShapeIndex == 9) // Stl Girder Tub
	{
		_S(Symmetry); _S(Slab Gap); raSizeList.Add(_LS(IDS_CMD_SECT_SOD_TOP)); raSizeList.Add(_LS(IDS_CMD_SECT_SOD_BOTTOM)); _S(B1); _S(B2); _S(B3); _S(B4); _S(B5); _S(B6); _S(H); _S(t1); _S(t2); _S(tw1); _S(tw2); _S(Bf1); _S(Bf2); _S(tfp);
	}

	/*
	if (m_nShapeIndex == 0 || m_nShapeIndex == 1 || m_nShapeIndex == 4) // 1Cell, 2Cell, Half
	{
		_S(HO1); _S(HO2); _S(HO2-1); _S(HO2-2); _S(HO3); _S(HO3-1);
		_S(BO1); _S(BO1-1); _S(BO1-2); _S(BO2); _S(BO2-1); _S(BO3);
		_S(HI1); _S(HI2); _S(HI2-1); _S(HI2-2); _S(HI3); _S(HI3-1); _S(HI4); _S(HI4-1); _S(HI4-2); _S(HI5);
		_S(BI1); _S(BI1-1); _S(BI1-2); _S(BI2-1); _S(BI3); _S(BI3-1); _S(BI3-2); 
		if (m_nShapeIndex == 1) _S(BI4);  // 2Cell
	}
	else if (m_nShapeIndex == 2)  // 3Cell
	{
		_S(HI1); _S(HI2); _S(HI3); _S(HI4); _S(HI5);
		_S(HI6); _S(HI7); _S(HI8); _S(HI9); _S(HI10); 
		_S(BI1); _S(BI2); _S(BI3); _S(BI4); _S(BI5); _S(BI6); _S(BI7); 
		_S(HO1); _S(HO2); 
		_S(BO1); _S(BO2); _S(BO3); 
		_S(HO2-1); _S(HO2-2);
		_S(HI2-1); _S(HI2-2); _S(HI4-1); _S(HI4-2); _S(HI6-1); _S(HI6-2); _S(HI6-3); _S(HI9-1); _S(HI9-2); _S(HI8-1); _S(HI8-2);
		_S(BO2-1); _S(BO2-2); 
		_S(BI1-1); _S(BI1-2); _S(BI4-1); _S(BI4-2); _S(BI3-1); _S(BI3-2); _S(BI3-3); _S(BI7-1); _S(BI7-2); _S(BI6-1); _S(BI6-2); 
	}
	else if (m_nShapeIndex == 8)  // 4Cell
	{
		_S(H1); _S(H2); _S(H3); _S(H4); _S(H5);
		_S(B1); _S(B2); _S(B3); _S(B4); _S(B5); _S(B6); _S(B7); _S(B8); 
	}
	else if (m_nShapeIndex == 9)  // nCell
	{
		_S(S-Width);

		_S(L:HO1); _S(L:HO2); _S(L:HO2-1); _S(L:HO3); _S(L:HO4);
		_S(L:BO1); _S(L:BO1-1); _S(L:BO2); _S(L:BO3); _S(L:BO4); _S(L:BO5);
		_S(L:HI1); _S(L:HI2); _S(L:HI2-1); _S(L:HI3); _S(L:HI4); _S(L:HI5); _S(L:HI6);
		_S(L:BI1); _S(L:BI2); _S(L:BI2-1); _S(L:BI3); _S(L:BI4); _S(L:BI5); _S(L:BI6); _S(L:BI7); _S(L:BI8);
		_S(L:R1); _S(L:R2);

		_S(R:HO1); _S(R:HO2); _S(R:HO2-1); _S(R:HO3); _S(R:HO4);
		_S(R:BO1); _S(R:BO1-1); _S(R:BO2); _S(R:BO3); _S(R:BO4); _S(R:BO5);
		_S(R:HI1); _S(R:HI2); _S(R:HI2-1); _S(R:HI3); _S(R:HI4); _S(R:HI5); _S(R:HI6);
		_S(R:BI1); _S(R:BI2); _S(R:BI2-1); _S(R:BI3); _S(R:BI4); _S(R:BI5); _S(R:BI6); _S(R:BI7); _S(R:BI8);
		_S(R:R1); _S(R:R2);

	}
	else if (m_nShapeIndex == 3)  // Mid
	{
		_S(H1);
		_S(HL1); _S(HL2); _S(HL2-1); _S(HL2-2); _S(HL3); _S(HL4); _S(HL4-1); _S(HL4-2); _S(HL5); 
		_S(BL1); _S(BL2); _S(BL2-1); _S(BL2-2); _S(BL4-1); _S(BL4-2); 
		_S(HR1); _S(HR2); _S(HR2-1); _S(HR2-2); _S(HR3); _S(HR4); _S(HR4-1); _S(HR4-2); _S(HR5); 
		_S(BR1); _S(BR2); _S(BR2-1); _S(BR2-2); _S(BR4-1); _S(BR4-2); 
	}
	else if (m_nShapeIndex == 5)  // Tee
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
	else if (m_nShapeIndex == 6)  // PLAT
	{
		_S(H1); _S(H2);
		_S(HOL1); _S(HOL2); _S(HOL3); 
		_S(BOL1); _S(BOL2); _S(BOL3); 
		_S(HOL1-1); _S(BOL1-1);
		_S(HIL1); _S(HIL2); 
		_S(BIL1); _S(BIL2); _S(BIL3); _S(BIL4); 
		_S(HOR1); _S(HOR2); _S(HOR3); 
		_S(BOR1); _S(BOR2); _S(BOR3); 
		_S(HOR1-1); _S(BOR1-1);
		_S(HIR1); _S(HIR2); 
		_S(BIR1); _S(BIR2); _S(BIR3);
	}
	else if (m_nShapeIndex == 7) // I
	{
		_S(H1);
		_S(HL1); _S(HL2); _S(HL2-1); _S(HL2-2); _S(HL3); _S(HL4); _S(HL4-1); _S(HL4-2); _S(HL5); 
		_S(BL1); _S(BL2); _S(BL2-1); _S(BL2-2); _S(BL4); _S(BL4-1); _S(BL4-2); 
		_S(HR1); _S(HR2); _S(HR2-1); _S(HR2-2); _S(HR3); _S(HR4); _S(HR4-1); _S(HR4-2); _S(HR5); 
		_S(BR1); _S(BR2); _S(BR2-1); _S(BR2-2); _S(BR4); _S(BR4-1); _S(BR4-2); 
	}
	else if (m_nShapeIndex == 10) // value
	{
		_S(HT); _S(BT); _S(T1); _S(T2); 
		_S(Area); _S(Asy); _S(Asz); _S(Ixx); _S(Iyy); _S(Izz); _S(Cyp); _S(Cym); _S(Czp); _S(Czm); 
		_S(Qyb); _S(Qzb); _S(Peri:O); _S(Peri:I); _S(Cent:y); _S(Cent:z);
		_S(y1); _S(z1); _S(y2); _S(z2); _S(y3); _S(z3); _S(y4); _S(z4);
	}
	else if (m_nShapeIndex == 11)   // CMPWEB
	{
		_S(HOL1); _S(HOL2); _S(HOL3); _S(HOL4); _S(HOL5); _S(HOL6); _S(HOL6-1);
		_S(BOL1); _S(BOL2); _S(BOL3); _S(BOL4); _S(BOL5); _S(BOL6); _S(BOL7); _S(BOL8); _S(BOL9); _S(BOL10); _S(BOL11); 
		_S(HIL1); _S(HIL2); _S(HIL3); _S(HIL4); _S(HIL5); _S(HIL6); _S(HIL7); _S(HIL8);
		_S(BIL1); _S(BIL2); _S(BIL3); _S(BIL4); _S(BIL5); _S(BIL6);
		_S(BOR1); _S(BOR2); _S(BOR10);
	}
	*/
	return raSizeList.GetSize();
#undef _S
}

int CCMSectItemGrid4Composite::GetUnitList(CStringArray& raUnitList)
{
	raUnitList.RemoveAll();
	
	CString csUnit;

	if (m_nShapeIndex == 10) // value
	{
		CUnitCtrl::GetUnitSystem(D_UNITSYS_BASE_LENGTH, csUnit); raUnitList.Add(csUnit);
		CUnitCtrl::GetUnitSystem(D_UNITSYS_BASE_LENGTH, csUnit); raUnitList.Add(csUnit);
		CUnitCtrl::GetUnitSystem(D_UNITSYS_BASE_LENGTH, csUnit); raUnitList.Add(csUnit);
		CUnitCtrl::GetUnitSystem(D_UNITSYS_BASE_LENGTH, csUnit); raUnitList.Add(csUnit);
		CUnitCtrl::GetUnitSystem(D_UNITSYS_BASE_AREA  , csUnit); raUnitList.Add(csUnit);
		CUnitCtrl::GetUnitSystem(D_UNITSYS_BASE_AREA  , csUnit); raUnitList.Add(csUnit);
		CUnitCtrl::GetUnitSystem(D_UNITSYS_BASE_AREA  , csUnit); raUnitList.Add(csUnit);
		CUnitCtrl::GetUnitSystem(D_UNITSYS_BASE_STIF  , csUnit); raUnitList.Add(csUnit);
		CUnitCtrl::GetUnitSystem(D_UNITSYS_BASE_STIF  , csUnit); raUnitList.Add(csUnit);
		CUnitCtrl::GetUnitSystem(D_UNITSYS_BASE_STIF  , csUnit); raUnitList.Add(csUnit);
		CUnitCtrl::GetUnitSystem(D_UNITSYS_BASE_LENGTH, csUnit); raUnitList.Add(csUnit);
		CUnitCtrl::GetUnitSystem(D_UNITSYS_BASE_LENGTH, csUnit); raUnitList.Add(csUnit);
		CUnitCtrl::GetUnitSystem(D_UNITSYS_BASE_LENGTH, csUnit); raUnitList.Add(csUnit);
		CUnitCtrl::GetUnitSystem(D_UNITSYS_BASE_LENGTH, csUnit); raUnitList.Add(csUnit);
		CUnitCtrl::GetUnitSystem(D_UNITSYS_BASE_AREA  , csUnit); raUnitList.Add(csUnit);
		CUnitCtrl::GetUnitSystem(D_UNITSYS_BASE_AREA  , csUnit); raUnitList.Add(csUnit);
		CUnitCtrl::GetUnitSystem(D_UNITSYS_BASE_LENGTH, csUnit); raUnitList.Add(csUnit);
		CUnitCtrl::GetUnitSystem(D_UNITSYS_BASE_LENGTH, csUnit); raUnitList.Add(csUnit);
		CUnitCtrl::GetUnitSystem(D_UNITSYS_BASE_LENGTH, csUnit); raUnitList.Add(csUnit);
		CUnitCtrl::GetUnitSystem(D_UNITSYS_BASE_LENGTH, csUnit); raUnitList.Add(csUnit);
		CUnitCtrl::GetUnitSystem(D_UNITSYS_BASE_LENGTH, csUnit); raUnitList.Add(csUnit);
		CUnitCtrl::GetUnitSystem(D_UNITSYS_BASE_LENGTH, csUnit); raUnitList.Add(csUnit);
		CUnitCtrl::GetUnitSystem(D_UNITSYS_BASE_LENGTH, csUnit); raUnitList.Add(csUnit);
		CUnitCtrl::GetUnitSystem(D_UNITSYS_BASE_LENGTH, csUnit); raUnitList.Add(csUnit);
		CUnitCtrl::GetUnitSystem(D_UNITSYS_BASE_LENGTH, csUnit); raUnitList.Add(csUnit);
		CUnitCtrl::GetUnitSystem(D_UNITSYS_BASE_LENGTH, csUnit); raUnitList.Add(csUnit);
		CUnitCtrl::GetUnitSystem(D_UNITSYS_BASE_LENGTH, csUnit); raUnitList.Add(csUnit);
		CUnitCtrl::GetUnitSystem(D_UNITSYS_BASE_LENGTH, csUnit); raUnitList.Add(csUnit);
	}
	return raUnitList.GetSize();
}

void  CCMSectItemGrid4Composite::GetSectBefore(T_SECT_SECTION_D& SectBefore)
{
	SectBefore.Initialize();
	SectBefore = m_SectBefore; // m_pSectD->SectBefore;
}

int CCMSectItemGrid4Composite::GetStiffList(int n, CArray<double, double> &raStiff)
{
	raStiff.RemoveAll();

	T_SECT_SECTBASE_D* pSectBase;
	if (n == 0) pSectBase = &m_pSectD->SectBefore.SectI;
	else pSectBase = &m_pSectD->SectBefore.SectJ;

	if (m_nShapeIndex == 10)    // PSC-Value
	{
		raStiff.SetSize(24);
		raStiff[0]  = pSectBase->Stiffness.Area;
		raStiff[1]  = pSectBase->Stiffness.Asy;
		raStiff[2]  = pSectBase->Stiffness.Asz;
		raStiff[3]  = pSectBase->Stiffness.Rxx;
		raStiff[4]  = pSectBase->Stiffness.Ryy;
		raStiff[5]  = pSectBase->Stiffness.Rzz;
		raStiff[6]  = pSectBase->Stiffness.Cyp;
		raStiff[7]  = pSectBase->Stiffness.Cym;
		raStiff[8]  = pSectBase->Stiffness.Czp;
		raStiff[9]  = pSectBase->Stiffness.Czm;
		raStiff[10] = pSectBase->Stiffness.Qyb;
		raStiff[11] = pSectBase->Stiffness.Qzb;
		raStiff[12] = pSectBase->PeriOut;
		raStiff[13] = pSectBase->PeriIn;
		raStiff[14] = pSectBase->Design.YBar;
		raStiff[15] = pSectBase->Design.ZBar;
		raStiff[16] = pSectBase->Stiffness.dx1;
		raStiff[17] = pSectBase->Stiffness.dy1;
		raStiff[18] = pSectBase->Stiffness.dx2;
		raStiff[19] = pSectBase->Stiffness.dy2;
		raStiff[20] = pSectBase->Stiffness.dx3;
		raStiff[21] = pSectBase->Stiffness.dy3;
		raStiff[22] = pSectBase->Stiffness.dx4;
		raStiff[23] = pSectBase->Stiffness.dy4;
	}
	else ASSERT(0);

	return raStiff.GetSize();
}

void CCMSectItemGrid4Composite::SetStiffList(int n, CArray<double, double> &raStiff)
{
	T_SECT_SECTBASE_D* pSectBase;
	if (n == 0) pSectBase = &m_pSectD->SectBefore.SectI;
	else pSectBase = &m_pSectD->SectBefore.SectJ;

	if (m_nShapeIndex == 10)    // PSC-Value
	{
		if (raStiff.GetSize() != 24) 
		{ ASSERT(0); raStiff.SetSize(24); }
		
		pSectBase->Stiffness.Area = raStiff[0];
		pSectBase->Stiffness.Asy  = raStiff[1];
		pSectBase->Stiffness.Asz  = raStiff[2]; 
		pSectBase->Stiffness.Rxx  = raStiff[3];
		pSectBase->Stiffness.Ryy  = raStiff[4];
		pSectBase->Stiffness.Rzz  = raStiff[5];
		pSectBase->Stiffness.Cyp  = raStiff[6];
		pSectBase->Stiffness.Cym  = raStiff[7];
		pSectBase->Stiffness.Czp  = raStiff[8];
		pSectBase->Stiffness.Czm  = raStiff[9];
		pSectBase->Stiffness.Qyb  = raStiff[10];
		pSectBase->Stiffness.Qzb  = raStiff[11];
		pSectBase->PeriOut        = raStiff[12];
		pSectBase->PeriIn         = raStiff[13];
		pSectBase->Design.YBar    = raStiff[14];
		pSectBase->Design.ZBar    = raStiff[15];
		pSectBase->Stiffness.dx1  = raStiff[16];
		pSectBase->Stiffness.dy1  = raStiff[17];
		pSectBase->Stiffness.dx2  = raStiff[18];
		pSectBase->Stiffness.dy2  = raStiff[19];
		pSectBase->Stiffness.dx3  = raStiff[20];
		pSectBase->Stiffness.dy3  = raStiff[21];
		pSectBase->Stiffness.dx4  = raStiff[22];
		pSectBase->Stiffness.dy4  = raStiff[23];
	}
	else ASSERT(0);
}

void CCMSectItemGrid4Composite::SaveSizeStlGirder(int nSizeRow, int nNum, int n, double dNum, double* pSize )
{
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
				m_pSectD->CmpTapJ.bSymAutoCalc = bSymmetricSecAutoCalc;
			}
		}
		else if(1 <= i && i <= 3) // Slab,Top,Bot Gap
		{
			double& dGapVal = n==0 ? m_pSectD->SectAfter.SectI.Size[i + 2] : m_pSectD->SectAfter.SectJ.Size[i + 2];
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
				pSize[i-4] = dNum;
			else
				pSize[i-4] = 0.0;
		}
	}
	
	// Symmetric일때 AutoCalc
	if(bSymmetricSecAutoCalc)
	{
		CompStlGSectSyAutoCalc(pSize, n == 1);
		if(m_pPreview != 0)
		{
			m_pPreview->SetCentroidFlag(FALSE);
			m_pPreview->Invalidate();
		}
	}
}

void CCMSectItemGrid4Composite::CompStlGSectSyAutoCalc(double* pSize, BOOL bJ)
{
	int nStype;  
	
	if(m_nShapeIndex == 7)
	{
		nStype = D_SECT_TYPE_COMPO_STLG_B;
	}
	else if(m_nShapeIndex == 8)
	{
		nStype = D_SECT_TYPE_COMPO_STLG_I;
	}
	else if(m_nShapeIndex == 9)
	{
		nStype = D_SECT_TYPE_COMPO_STLG_TUB;
	}
	else
	{
		ASSERT(0);
		return;
	}  
		
	double dB1, dB2, dB3, dB4, dB5, dtw1, dbf1;    
	if(m_nShapeIndex == 7)
	{
		dB1  = pSize[0];
		dB2  = pSize[1];
		dB3  = pSize[2];
		dB4  = pSize[3];
		dB5  = pSize[4];
		dtw1 = pSize[9];
		dbf1 = 0; // 안씀
	}
	else if (m_nShapeIndex == 8)
	{
		dB1  = pSize[0];
		dB2  = 0; // 안씀
		dB3  = pSize[2];
		dB4  = 0; // 안씀
		dB5  = 0; // 안씀
		dtw1 = 0; // 안씀
		dbf1 = 0; // 안씀    
	}
	else if (m_nShapeIndex == 9)
	{
		dB1  = pSize[0];
		dB2  = pSize[1];
		dB3  = pSize[2];
		dB4  = pSize[3];
		dB5  = pSize[4];
		dtw1 = pSize[9];
		dbf1 = pSize[11];
	}

	double dBc = m_pSectD->SectAfter.SectJ.Size[0];
	double dTop, dBot, dSg; // 계산되어 나오는 변수
	CSectUtil::CompStlGSymmetricSectAutoCalc(nStype, dBc, dB1, dB2, dB3, dB4, dB5, dtw1, dbf1, bJ, dSg, dTop, dBot, *m_pSectD);

	(!bJ) ? m_pSectD->SectAfter.SectI.Size[3] : m_pSectD->SectAfter.SectJ.Size[3] = dSg;
	(!bJ) ? m_pSectD->SectAfter.SectI.Size[4] : m_pSectD->SectAfter.SectJ.Size[4] = dTop;
	(!bJ) ? m_pSectD->SectAfter.SectI.Size[5] : m_pSectD->SectAfter.SectJ.Size[5] = dBot;
}

void CCMSectItemGrid4Composite::ShowDataStlGirder( int n, int nRow, CStringArray& aSizeList, int nPlace, CString csUnit, double dSlabWidth, double * pSize )
{
	BOOL bSymmetricSecAutoCalc;
	bSymmetricSecAutoCalc = n==0 ? m_pSectD->SectBefore.SectI.bSymAutoCalc : m_pSectD->CmpTapJ.bSymAutoCalc;

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

			if(1 <= i && i <= 3) // Slab,Top,Bot Gap
			{
				SetStyleRange(CGXRange(nRow+i, 2), CGXStyle().SetValue(n==0 ? m_pSectD->SectAfter.SectI.Size[i + 2] : m_pSectD->SectAfter.SectJ.Size[i + 2]));
			}
			else // Size
			{
				SetStyleRange(CGXRange(nRow+i, 2), CGXStyle().SetValue(pSize[i-4]));
			}   
		}      
	}
}
