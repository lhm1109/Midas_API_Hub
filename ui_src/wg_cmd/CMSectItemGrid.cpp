// CMSectItemGrid.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMSectItemGrid.h"

#include "CMSecViewWnd.h"

#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_SectUtil.h"
#include "..\wg_db\wg_db_SectDB.h"
#include "CMSectItemImportReg.h"
#include "..\wg_db\wg_db_AttrCtrl.h"

#define D_SIZE_MAX_NUM 8
#define D_STIFF_MAX_NUM (24+2)  // Zyy, Zzz 추가 v692부터
#define D_STIFF_WARPING_MAX_NUM 16
//#define D_STIFF_PSC_DESIGN 7

#define D_TOTAL_ROW_NUM (1+D_SIZE_MAX_NUM+1+1+1+D_STIFF_MAX_NUM+D_STIFF_WARPING_MAX_NUM/*+D_STIFF_PSC_DESIGN*/)
#define D_SIZE_START_ROW 2
#define D_STIFF_START_ROW 13//bbong 12->13

#include "..\wg_db\DB_ST_DT.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

UINT CCMSectItemGrid::WM_GRID_DATA_CHANGED = RegisterWindowMessage(_T("WM_GRID_DATA_CHANGED"));

/////////////////////////////////////////////////////////////////////////////
// CCMSectItemGrid
CCMSectItemGrid::CCMSectItemGrid()
{
	m_pPreview = 0;
	m_pSectBase = 0;
	m_pPSCDesign = 0;
	m_nSectType = -1;
	m_nShapeIndex = -1;
	m_pCellNumber = 0;
	m_bInitDone = FALSE;
	m_tapered=FALSE;
	m_iOrj=0;
	m_SectKOrg =0;
	m_pStiff = 0;
	m_pPeriOut = 0;
	m_pPeriIn = 0;
	m_pYBar = 0;
	m_pZBar = 0;
	m_bConsiderWarpingEffect=FALSE;
	m_STIFF_MAX_NUM =0;
	m_TOTAL_ROW_NUM =0;

	//m_bPSCDesign=FALSE;
}

CCMSectItemGrid::~CCMSectItemGrid()
{
}

//---------------------------------------------------------------------------
// Public Interface Functions
void CCMSectItemGrid::InitGrid(CCMSecViewWnd* pPreview)
{
	ASSERT(pPreview);
	m_pPreview = pPreview;

	if (!m_bInitDone) Initialize(); // 그리드를 초기화한다.
	else { ResetGrid(); Initialize(); }
	m_bInitDone = TRUE;

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

	LockUpdate(FALSE);
	Redraw();
}
/*
#define D_SIZE_MAX_NUM 6
#define D_STIFF_MAX_NUM 16

#define D_TOTAL_ROW_NUM (1+D_SIZE_MAX_NUM+1+1+D_STIFF_MAX_NUM)
#define D_SIZE_START_ROW 2
#define D_STIFF_START_ROW 10
*/
void CCMSectItemGrid::SetStyleOnSizeCell()
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	int i;

	// Size : 소수 넷째자리까지 표현
	for (i = 0; i < D_SIZE_MAX_NUM; i++)
	{
		SetStyleRange(CGXRange(D_SIZE_START_ROW+i, 2), CGXStyle()
			.SetFormat(GX_FMT_FIXED).SetPlaces(4));
	}

	// Stiffness 전체 : 소수 넷째자리까지 표현
	for (i = 0; i < m_STIFF_MAX_NUM; i++)
	{
		SetStyleRange(CGXRange(D_STIFF_START_ROW+i, 2), CGXStyle()
			.SetFormat(GX_FMT_FIXED).SetPlaces(4));
	}

	for (i = 0; i < D_STIFF_WARPING_MAX_NUM; i++)
	{
		SetStyleRange(CGXRange(D_STIFF_START_ROW+m_STIFF_MAX_NUM+i, 2), CGXStyle()
			.SetFormat(GX_FMT_FIXED).SetPlaces(4));
	}

// 	for (i = 0; i < D_STIFF_PSC_DESIGN; i++)
//   {
//     SetStyleRange(CGXRange(D_STIFF_START_ROW+m_STIFF_MAX_NUM+D_STIFF_WARPING_MAX_NUM+i, 2), CGXStyle()
//       .SetFormat(GX_FMT_FIXED).SetPlaces(4));
//   }

	// Area, Asy, Asz, Ixx, Iyy, Izz : 소수 다섯째 자리 지수 표현
	int nNum =(m_nSectType == D_SECT_TYPE_COMPO_G)? 7 : 6;
	for (i = 0; i < nNum; i++)
	{
		SetStyleRange(CGXRange(D_STIFF_START_ROW+i, 2), CGXStyle()
			.SetFormat(GX_FMT_FLOAT).SetPlaces(5));
	}

	// Peri:O, Peri:I : 소수 다섯째 자리 지수 표현
	int nNum2 =(m_nSectType == D_SECT_TYPE_COMPO_G)? 13 : 12;
	for (i = 0; i < 2; i++)
	{
		SetStyleRange(CGXRange(D_STIFF_START_ROW+nNum2+i, 2), CGXStyle()
			.SetFormat(GX_FMT_FLOAT).SetPlaces(5));
	}

	/*
	T_UNIT_INDEX UnitIndex;
	pDoc->m_pUnitCtrl->GetUnitIndexCurrent(UnitIndex);
	int nPlace;
	switch(UnitIndex.nBase_Length)
	{
	case D_UNITSYS_LENGTH_INDEX_MM : nPlace = 2; break;
	case D_UNITSYS_LENGTH_INDEX_M  : nPlace = 4; break;
	default                        : nPlace = 3; break;
	}

	// 일단 해당 컬럼이 몇 번째 로우인지 알아야 한다.
	for (int i = 0; i < D_SIZE_MAX_NUM; i++)
	{
		if(m_pSectBase->Size[i] >= 0.0001 || m_pSectBase->Size[i] == 0.0)
		{
			SetStyleRange(CGXRange(2+i, 2), CGXStyle()
				.SetFormat(GX_FMT_FIXED).SetPlaces(nPlace));
		}
		else
		{
			SetStyleRange(CGXRange(2+i, 2), CGXStyle()
				.SetFormat(GX_FMT_FLOAT).SetPlaces(2));
		}
	}

	// Ixx, Iyy, Izz는 소수 넷째자리 지수로 표현 
	for(i = D_STIFF_START_ROW + 3; i <= D_STIFF_START_ROW + 5; i++)
	{
		SetStyleRange(CGXRange(i, 2), CGXStyle()
			.SetFormat(GX_FMT_FLOAT).SetPlaces(4));
	}
	*/
}

// 20080104 mylee - Tapered 창에서 Composite General 과 일반 General 구분 위해
void CCMSectItemGrid::SetDefineValue(int nSectType)
{
	if(nSectType == D_SECT_TYPE_COMPO_G)
	{ // WArea 정보 추가
		m_STIFF_MAX_NUM = D_STIFF_MAX_NUM+1;
		m_TOTAL_ROW_NUM = D_TOTAL_ROW_NUM+1;
	}
	else
	{
		m_STIFF_MAX_NUM = D_STIFF_MAX_NUM;
		m_TOTAL_ROW_NUM = D_TOTAL_ROW_NUM;
	}
}

void CCMSectItemGrid::ChangeSectType(int nSectType)
{
	m_nSectType = nSectType;
	SetDefineValue(nSectType);
}

// 20080226 mylee - Composite General 형태로 초기화
void CCMSectItemGrid::SetDataSource2(T_SECT_STIFFNESS* pStiff, double* pPeriOut, double* pPeriIn, double* pYBar, double* pZBar, 
																		 T_SECT_SECTBASE_D* pSectBase, int nSectType, int *pCellNum)
{
	SetDataSource(pSectBase, 0, nSectType, pCellNum);

	m_pStiff = pStiff;
	m_pPeriOut = pPeriOut;
	m_pPeriIn = pPeriIn;
	m_pYBar = pYBar;
	m_pZBar = pZBar;
}

void CCMSectItemGrid::SetDataSource(T_SECT_SECTBASE_D* pSectBase, T_SECP_D *pPSCDesign, int nSectType, int *pCellNum)
{
	m_pSectBase = pSectBase;
	m_pPSCDesign = pPSCDesign;
	m_nSectType = nSectType;
	SetDefineValue( nSectType);
	m_pCellNumber = pCellNum;

	// Compo Gen 타입 위해 밑에 변수 추가하고 여기서 참조해서 꺼내씀.
	// Compo Gen 타입일때는 SetDataSource2 에서 밑에 변수 따로 초기화
	m_pStiff = &m_pSectBase->Stiffness;
	m_pPeriOut = &m_pSectBase->PeriOut;
	m_pPeriIn = &m_pSectBase->PeriIn;
	m_pYBar = &m_pSectBase->Design.YBar;
	m_pZBar = &m_pSectBase->Design.ZBar;

	BOOL bOldLock = LockUpdate(TRUE);
	BOOL bOldReadOnlyLock = GetParam()->IsLockReadOnly();
	GetParam()->SetLockReadOnly(FALSE);
	if (m_nSectType == D_SECT_TYPE_USER || m_nSectType == D_SECT_TYPE_COMPO_G)
	{
		SetRowCount(m_TOTAL_ROW_NUM);
		SetColCount(3);

		// set column width
		SetColWidth(1, 1, globalUtils.ScaleByDPI(60));
		SetColWidth(2, 2, globalUtils.ScaleByDPI(100));
		SetColWidth(3, 3, globalUtils.ScaleByDPI(60));

		SetCoveredCellsRowCol(D_SIZE_START_ROW-1, 1, D_SIZE_START_ROW-1, 3);
		SetCoveredCellsRowCol(D_STIFF_START_ROW-3, 1, D_STIFF_START_ROW-3, 3);
		SetCoveredCellsRowCol(D_STIFF_START_ROW-2, 1, D_STIFF_START_ROW-2, 3);
		SetCoveredCellsRowCol(D_STIFF_START_ROW-1, 1, D_STIFF_START_ROW-1, 3);
		SetStyleRange(CGXRange(D_SIZE_START_ROW-1, 1), CGXStyle()
			.SetInterior(COLORREF(RGB(0, 0, 255)))
			.SetTextColor(COLORREF(RGB(255, 255, 0)))
			.SetValue(_LS(IDS_WG_CMD__ADDD__Size)));
		SetStyleRange(CGXRange(D_STIFF_START_ROW-3, 1), CGXStyle()
			.SetInterior(COLORREF(RGB(0, 0, 255)))
			.SetTextColor(COLORREF(RGB(255, 255, 0)))
			.SetValue(_LS(IDS_WG_CMD__ADDD__Stiffness)));
		SetStyleRange(CGXRange(D_STIFF_START_ROW-1, 1), CGXStyle()
			.SetControl(GX_IDS_CTRL_PUSHBTN)
			.SetInterior(GXSYSCOLOR(COLOR_BTNFACE))
			.SetChoiceList(_LS(IDS_WG_CMD__ADDD__Tapered_Import))
			.SetTextColor(COLORREF(RGB(0, 0, 0)))
			.SetEnabled(TRUE));
		 SetStyleRange(CGXRange(D_STIFF_START_ROW-2, 1), CGXStyle()
			.SetControl(GX_IDS_CTRL_PUSHBTN)
			.SetInterior(GXSYSCOLOR(COLOR_BTNFACE))
			.SetChoiceList(_LS(IDS_WG_CMD__ADDD__Calculate_Stiffness))
			.SetTextColor(COLORREF(RGB(0, 0, 0)))
			.SetEnabled(TRUE));
		SetRowHeight(D_STIFF_START_ROW-1, D_STIFF_START_ROW-1, globalUtils.ScaleByDPI(22));
		SetRowHeight(D_STIFF_START_ROW-1, D_STIFF_START_ROW-1, globalUtils.ScaleByDPI(22));

		SetStyleRange(CGXRange().SetCols(1), CGXStyle()
			.SetReadOnly(TRUE)
			.SetEnabled(FALSE)
			.SetInterior(::GetSysColor(COLOR_BTNFACE)));
		SetStyleRange(CGXRange().SetCols(2), CGXStyle()
			.SetHorizontalAlignment(DT_RIGHT)
			.SetValueType(GX_VT_NUMERIC));
		SetStyleRange(CGXRange().SetCols(3), CGXStyle()
			.SetReadOnly(TRUE)
			.SetEnabled(FALSE)
			.SetInterior(::GetSysColor(COLOR_BTNFACE)));

		SetStyleOnSizeCell();
 
		CString csUnit;
		CUnitCtrl::GetUnitSystem(D_UNITSYS_BASE_LENGTH, csUnit);
		for (int i = 0; i < D_SIZE_MAX_NUM; i++)
		{
			SetStyleRange(CGXRange(2+i, 3), CGXStyle().SetValue(csUnit));
		}

		/*
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
		for (int i = 0; i < D_SIZE_MAX_NUM; i++)
		{
			if(m_pSectBase->Size[i] >= 0.0001 || m_pSectBase->Size[i] == 0.0)
			{
				SetStyleRange(CGXRange(2+i, 2), CGXStyle()
					.SetFormat(GX_FMT_FIXED)
					.SetPlaces(nPlace));
			}
			else
			{
				SetStyleRange(CGXRange(2+i, 2), CGXStyle()
					.SetFormat(GX_FMT_FLOAT).SetPlaces(4));
			}
			SetStyleRange(CGXRange(2+i, 3), CGXStyle().SetValue(csUnit));
		}
		*/
		//CString aStiffTitle[27]; //27 = m_STIFF_MAX_NUM
		CString aStiffTitle1[] = { 
			_LS(IDS_WG_CMD__ADDD__Area), _T("WArea"), _LS(IDS_WG_CMD__ADDD__Asy), _LS(IDS_WG_CMD__ADDD__Asz), _LS(IDS_WG_CMD__ADDD__Ixx), _LS(IDS_WG_CMD__ADDD__Iyy), _LS(IDS_WG_CMD__ADDD__Izz),  
			_LS(IDS_WG_CMD__ADDD__Cyp), _LS(IDS_WG_CMD__ADDD__Cym), _LS(IDS_WG_CMD__ADDD__Czp), _LS(IDS_WG_CMD__ADDD__Czm), _LS(IDS_WG_CMD__ADDD__Qyb), _LS(IDS_WG_CMD__ADDD__Qzb), 
			_LS(IDS_WG_CMD__ADDD__PeriOut), _LS(IDS_WG_CMD__ADDD__PeriIn), 
			_LS(IDS_WG_CMD__ADDD__Cent_Y), _LS(IDS_WG_CMD__ADDD__Cent_Z),
			_T("y1"), _T("z1"), _T("y2"), _T("z2"), _T("y3"), _T("z3"), _T("y4"), _T("z4"), _T("Zyy"), _T("Zzz"),
		};
		CString aStiffTitle2[] ={ 
			_LS(IDS_WG_CMD__ADDD__Area), _LS(IDS_WG_CMD__ADDD__Asy), _LS(IDS_WG_CMD__ADDD__Asz), _LS(IDS_WG_CMD__ADDD__Ixx), _LS(IDS_WG_CMD__ADDD__Iyy), _LS(IDS_WG_CMD__ADDD__Izz),  
			_LS(IDS_WG_CMD__ADDD__Cyp), _LS(IDS_WG_CMD__ADDD__Cym), _LS(IDS_WG_CMD__ADDD__Czp), _LS(IDS_WG_CMD__ADDD__Czm), _LS(IDS_WG_CMD__ADDD__Qyb), _LS(IDS_WG_CMD__ADDD__Qzb), 
			_LS(IDS_WG_CMD__ADDD__PeriOut), _LS(IDS_WG_CMD__ADDD__PeriIn), 
			_LS(IDS_WG_CMD__ADDD__Cent_Y), _LS(IDS_WG_CMD__ADDD__Cent_Z),
			_T("y1"), _T("z1"), _T("y2"), _T("z2"), _T("y3"), _T("z3"), _T("y4"), _T("z4"), _T("Zyy"), _T("Zzz"),
		};


		T_SECT_STIFFNESS_UNIT* pUnit = &CUnitCtrl::m_SECT_UNIT.SectBefore.SectI.Stiffness;
		T_SECT_DESIGN_UNIT* pUnit2 = &CUnitCtrl::m_SECT_UNIT.SectBefore.SectI.Design;
		//int aStiffUnit[27]; //= m_STIFF_MAX_NUM
		int aStiffUnit1[] = { 
			pUnit->Area,  pUnit->WArea, pUnit->Asy, pUnit->Asz, pUnit->Rxx, pUnit->Ryy, pUnit->Rzz,
			pUnit->Cyp, pUnit->Cym, pUnit->Czp, pUnit->Czm, pUnit->Qyb, pUnit->Qzb,
			D_UNITSYS_BASE_LENGTH, D_UNITSYS_BASE_LENGTH,
			D_UNITSYS_BASE_LENGTH, D_UNITSYS_BASE_LENGTH,
			D_UNITSYS_BASE_LENGTH, D_UNITSYS_BASE_LENGTH, D_UNITSYS_BASE_LENGTH, D_UNITSYS_BASE_LENGTH, 
			D_UNITSYS_BASE_LENGTH, D_UNITSYS_BASE_LENGTH, D_UNITSYS_BASE_LENGTH, D_UNITSYS_BASE_LENGTH,
			pUnit2->Zyy, pUnit2->Zzz,
		};
		int aStiffUnit2[] = { 
			pUnit->Area, pUnit->Asy, pUnit->Asz, pUnit->Rxx, pUnit->Ryy, pUnit->Rzz,
			pUnit->Cyp, pUnit->Cym, pUnit->Czp, pUnit->Czm, pUnit->Qyb, pUnit->Qzb,
			D_UNITSYS_BASE_LENGTH, D_UNITSYS_BASE_LENGTH,
			D_UNITSYS_BASE_LENGTH, D_UNITSYS_BASE_LENGTH,
			D_UNITSYS_BASE_LENGTH, D_UNITSYS_BASE_LENGTH, D_UNITSYS_BASE_LENGTH, D_UNITSYS_BASE_LENGTH, 
			D_UNITSYS_BASE_LENGTH, D_UNITSYS_BASE_LENGTH, D_UNITSYS_BASE_LENGTH, D_UNITSYS_BASE_LENGTH,
			pUnit2->Zyy, pUnit2->Zzz,
		};

		CArray<CString, CString> aStiffTitle;          aStiffTitle.SetSize(m_STIFF_MAX_NUM);
		CArray<unsigned int, unsigned int> aStiffUnit; aStiffUnit.SetSize(m_STIFF_MAX_NUM);
		for(int i=0; i<m_STIFF_MAX_NUM; i++)
		{
			aStiffTitle[i] = (m_nSectType == D_SECT_TYPE_COMPO_G)? aStiffTitle1[i] : aStiffTitle2[i];
			aStiffUnit[i]  = (m_nSectType == D_SECT_TYPE_COMPO_G)? aStiffUnit1[i]  : aStiffUnit2[i];
		}
		

		for (int i = 0; i < m_STIFF_MAX_NUM; i++)
		{
			CUnitCtrl::GetUnitSystem(aStiffUnit[i], csUnit);
			SetValueRange(CGXRange(D_STIFF_START_ROW+i, 1), aStiffTitle[i]);
			SetValueRange(CGXRange(D_STIFF_START_ROW+i, 3), csUnit);
		}

		CString aStiffWarpingTitle[] = {_T("Iw"), _T("w1"), _T("w2"), _T("w3"), _T("w4"), _T("Cxy1"), _T("Cxy2"), _T("Cxy3"), _T("Cxy4"),_T("Cxz1"), _T("Cxz2"), _T("Cxz3"), _T("Cxz4"), _T("ys-yc"), _T("zs-zc"), _T("Ip")};
		int aStiffWarpingUnit[] = { 
			pUnit->dIw, pUnit->dw1, pUnit->dw2, pUnit->dw3, pUnit->dw4,
			pUnit->dxy1, pUnit->dxy2, pUnit->dxy3, pUnit->dxy4, pUnit->dxz1, pUnit->dxz2, pUnit->dxz3, pUnit->dxz4, pUnit->dYsy, pUnit->dYsz, pUnit->dIp};

		for (int i = 0; i < D_STIFF_WARPING_MAX_NUM; i++)
		{
			CUnitCtrl::GetUnitSystem(aStiffWarpingUnit[i], csUnit);
			SetValueRange(CGXRange(D_STIFF_START_ROW+m_STIFF_MAX_NUM+i, 1), aStiffWarpingTitle[i]);
			SetValueRange(CGXRange(D_STIFF_START_ROW+m_STIFF_MAX_NUM+i, 3), csUnit);
		}

// 		T_SECP_UNIT* pUnitPSCDesign = &CUnitCtrl::m_SECP_UNIT;
// 
// 		CString aStiffPSCDesign[] = {_T("Z1"), _T("Z2"), _T("Z3"), _T("t1"), _T("t2"), _T("t3"), _T("Torsion t")};
// 		int aStiffPSCDesignUnit[] = { 
// 			pUnitPSCDesign->dZ1, pUnitPSCDesign->dZ2, pUnitPSCDesign->dZ3, pUnitPSCDesign->dt1, pUnitPSCDesign->dt2, pUnitPSCDesign->dt3, pUnitPSCDesign->dTort};
// 
// 		for (i = 0; i < D_STIFF_PSC_DESIGN; i++)
// 		{
// 			CUnitCtrl::GetUnitSystem(aStiffPSCDesignUnit[i], csUnit);
// 			SetValueRange(CGXRange(D_STIFF_START_ROW+m_STIFF_MAX_NUM+D_STIFF_WARPING_MAX_NUM+i, 1), aStiffPSCDesign[i]);
// 			SetValueRange(CGXRange(D_STIFF_START_ROW+m_STIFF_MAX_NUM+D_STIFF_WARPING_MAX_NUM+i, 3), csUnit);
//     }

		HideRows(D_STIFF_START_ROW+m_STIFF_MAX_NUM, D_STIFF_START_ROW+m_STIFF_MAX_NUM+D_STIFF_WARPING_MAX_NUM, m_bConsiderWarpingEffect==FALSE);
		//HideRows(D_STIFF_START_ROW+m_STIFF_MAX_NUM+D_STIFF_WARPING_MAX_NUM, D_STIFF_START_ROW+m_STIFF_MAX_NUM+D_STIFF_WARPING_MAX_NUM+D_STIFF_PSC_DESIGN, m_bPSCDesign==FALSE);
	}
	GetParam()->SetLockReadOnly(bOldReadOnlyLock);

	LockUpdate(bOldLock);
	if (!bOldLock) Redraw();

	SetRightValidCell(1, 1);
}

void CCMSectItemGrid::ChangeShape(int nNewShapeIndex)
{
	m_nShapeIndex = nNewShapeIndex;

	if(m_nSectType == D_SECT_TYPE_COMPO_G) m_nShapeIndex = 21;

	// show/hide row according to shape
	BOOL bOldLock = LockUpdate(TRUE);
	if (m_nSectType == D_SECT_TYPE_USER || m_nSectType == D_SECT_TYPE_COMPO_G)
	{
		CString csShape;
		csShape = CSectUtil::GetShapeNameFromIndexReg(m_nShapeIndex);
		int nSizeNum = CSectUtil::GetSizeCountByRegShape(csShape);
		if(m_nShapeIndex==1) // Channel, 임시 KJH20040120 => Channel의 경우 r1, r2는 DB에서만 고려
		{
			nSizeNum -= 2;
		}
		//HideRows(2, 2+nSizeNum-1, FALSE); // show
		HideRows(1, 2+nSizeNum-1, csShape==D_SECT_SHAPE_REG_GEN); // show
		if (nSizeNum < D_SIZE_MAX_NUM) HideRows(2+nSizeNum, 2+D_SIZE_MAX_NUM-1, TRUE); // hide
		HideRows(D_STIFF_START_ROW-2,D_STIFF_START_ROW-2, csShape==D_SECT_SHAPE_REG_GEN);
		HideRows(D_STIFF_START_ROW-1,D_STIFF_START_ROW-1, !(IsTapered()&&(csShape==D_SECT_SHAPE_REG_GEN)));


		// change size title
		CStringArray aSizeTitle;
		CSectUtil::GetSizeTitleFromShapeIndexReg(m_nShapeIndex, aSizeTitle);
		if(m_nShapeIndex==1) // Channel, 임시 KJH20040120 => Channel의 경우 r1, r2는 DB에서만 고려
		{
			aSizeTitle.RemoveAt(6,2);
		}
		ASSERT(nSizeNum == aSizeTitle.GetSize());
		nSizeNum = aSizeTitle.GetSize();
		BOOL bOldReadOnlyLock = GetParam()->IsLockReadOnly();
		GetParam()->SetLockReadOnly(FALSE);
		for (int i = 0; i < nSizeNum; i++)
			SetValueRange(CGXRange(2+i, 1), aSizeTitle[i]);
		GetParam()->SetLockReadOnly(bOldReadOnlyLock);
	}
	LockUpdate(bOldLock);
	if (!bOldLock) Redraw();
}

void CCMSectItemGrid::SetWarpingEffect(BOOL bConsiderWarpingEffect)
{
	m_bConsiderWarpingEffect = bConsiderWarpingEffect;
	HideRows(D_STIFF_START_ROW+m_STIFF_MAX_NUM, D_STIFF_START_ROW+m_STIFF_MAX_NUM+D_STIFF_WARPING_MAX_NUM, m_bConsiderWarpingEffect==FALSE);
	ShowData();
}

// void CCMSectItemGrid::SetPSCDesign(BOOL bPSCDesign)
// {
// 	m_bPSCDesign = bPSCDesign;
// 	HideRows(D_STIFF_START_ROW+m_STIFF_MAX_NUM+D_STIFF_WARPING_MAX_NUM, D_STIFF_START_ROW+m_STIFF_MAX_NUM+D_STIFF_WARPING_MAX_NUM+D_STIFF_PSC_DESIGN, m_bPSCDesign==FALSE);
// 	ShowData();
// }

void CCMSectItemGrid::ShowData()
{
	if (m_nSectType == D_SECT_TYPE_USER || m_nSectType == D_SECT_TYPE_COMPO_G)
	{
		CString csShape;
		csShape = CSectUtil::GetShapeNameFromIndexReg(m_nShapeIndex);
		int nSizeNum = CSectUtil::GetSizeCountByRegShape(csShape);
		for (int i = 0; i < nSizeNum; i++)
			SetValueRange(CGXRange(D_SIZE_START_ROW+i, 2), m_pSectBase->Size[i]);

		BOOL bWarea= (m_nSectType == D_SECT_TYPE_COMPO_G)? TRUE : FALSE;
		int nNum=(bWarea)? 17 : 16;

		double *pValue1[17], *pValue2[8+2];

		CSectUtil::MakeMapStiffVar(m_pStiff, pValue1, bWarea);
		if(bWarea)
		{
			pValue1[13] = m_pPeriOut;
			pValue1[14] = m_pPeriIn;
			pValue1[15] = m_pYBar;
			pValue1[16] = m_pZBar;
		}
		else
		{
			pValue1[12] = m_pPeriOut;
			pValue1[13] = m_pPeriIn;
			pValue1[14] = m_pYBar;
			pValue1[15] = m_pZBar;    
		}

		CSectUtil::MakeMapStiffVar2(m_pStiff, pValue2);
		pValue2[8] = &m_pSectBase->Design.Zyy;
		pValue2[9] = &m_pSectBase->Design.Zzz;

		for (int i = 0; i < nNum; i++)
			SetValueRange(CGXRange(D_STIFF_START_ROW+i, 2), *pValue1[i]);
		for (int i = nNum; i < m_STIFF_MAX_NUM; i++)
			SetValueRange(CGXRange(D_STIFF_START_ROW+i, 2), *pValue2[i-nNum]);

		if(m_bConsiderWarpingEffect)
		{
			double *pValue3[16];
			CSectUtil::MakeMapStiffVar3(m_pStiff, pValue3);
			for(int i = m_STIFF_MAX_NUM; i<m_STIFF_MAX_NUM+D_STIFF_WARPING_MAX_NUM; i++)
				SetValueRange(CGXRange(D_STIFF_START_ROW+i, 2), *pValue3[i-m_STIFF_MAX_NUM]);
		}

// 		if(m_bConsiderWarpingEffect)
// 		{
// 			if(m_bPSCDesign)
// 			{
// 				double *pValuePSCDesign[7];
// 				CSectUtil::MakeMapStiffPSCDesign(m_pPSCDesign, pValuePSCDesign, m_tapered, m_iOrj);
// 				for(i = m_STIFF_MAX_NUM+D_STIFF_WARPING_MAX_NUM; i<m_STIFF_MAX_NUM+D_STIFF_WARPING_MAX_NUM+D_STIFF_PSC_DESIGN; i++)
// 					SetValueRange(CGXRange(D_STIFF_START_ROW+i, 2), *pValuePSCDesign[i-m_STIFF_MAX_NUM+D_STIFF_WARPING_MAX_NUM]);
// 			}
// 		}
// 		else
// 		{
// 			if(m_bPSCDesign)
// 			{
// 				double *pValuePSCDesign[7];
// 				CSectUtil::MakeMapStiffPSCDesign(m_pPSCDesign, pValuePSCDesign, m_tapered, m_iOrj);
// 				for(i = m_STIFF_MAX_NUM; i<m_STIFF_MAX_NUM+D_STIFF_PSC_DESIGN; i++)
// 					SetValueRange(CGXRange(D_STIFF_START_ROW+i, 2), *pValuePSCDesign[i-m_STIFF_MAX_NUM]);
// 			}
// 		}

		m_pPreview->Invalidate();
	}
}

BOOL CCMSectItemGrid::SaveData()
{
	BOOL bSuccess = SaveSize();
	bSuccess = bSuccess && SaveStiffness();
	return bSuccess;
}

BOOL CCMSectItemGrid::SaveSize()
{
	CString csNum;
	double dNum;

	CString csShape;
	csShape = CSectUtil::GetShapeNameFromIndexReg(m_nShapeIndex);
	int nSizeNum = CSectUtil::GetSizeCountByRegShape(csShape);
	for (int i = 0; i < nSizeNum; i++)
	{
		csNum = GetEditingValue(D_SIZE_START_ROW+i, 2);
		if (GetDoubleValue(csNum, dNum)) 
			m_pSectBase->Size[i] = dNum;
		else m_pSectBase->Size[i] = 0.0;
	}

	return TRUE;
}

BOOL CCMSectItemGrid::SaveStiffness()
{
	CString csNum;
	double dNum;

	BOOL bWarea= (m_nSectType == D_SECT_TYPE_COMPO_G)? TRUE : FALSE;
	int nNum=(bWarea)? 17 : 16;
 
	double *pValue1[17], *pValue2[8+2];

	CSectUtil::MakeMapStiffVar(m_pStiff, pValue1, bWarea);
	if(bWarea)
	{
		pValue1[13] = m_pPeriOut;
		pValue1[14] = m_pPeriIn;
		pValue1[15] = m_pYBar;
		pValue1[16] = m_pZBar;
	}
	else
	{
		pValue1[12] = m_pPeriOut;
		pValue1[13] = m_pPeriIn;
		pValue1[14] = m_pYBar;
		pValue1[15] = m_pZBar;    
	}

	CSectUtil::MakeMapStiffVar2(m_pStiff, pValue2);
	pValue2[8] = &m_pSectBase->Design.Zyy;
	pValue2[9] = &m_pSectBase->Design.Zzz;

	for (int i = 0; i < nNum; i++)
	{
		csNum = GetEditingValue(D_STIFF_START_ROW+i, 2);
		if (GetDoubleValue(csNum, dNum))  *pValue1[i] = dNum;
		else                              *pValue1[i] = 0.0;
	}
	for (int i = nNum; i < m_STIFF_MAX_NUM; i++)
	{
		csNum = GetEditingValue(D_STIFF_START_ROW+i, 2);
		if (GetDoubleValue(csNum, dNum))  *pValue2[i-nNum] = dNum;
		else                              *pValue2[i-nNum] = 0.0;
	}

	if(m_bConsiderWarpingEffect)
	{
		double *pValue3[16];
		CSectUtil::MakeMapStiffVar3(m_pStiff, pValue3);

		for(int i = m_STIFF_MAX_NUM; i<m_STIFF_MAX_NUM+D_STIFF_WARPING_MAX_NUM; i++)
		{
			csNum = GetEditingValue(D_STIFF_START_ROW+i, 2);
			if (GetDoubleValue(csNum, dNum)) *pValue3[i-m_STIFF_MAX_NUM] = dNum;
			else                             *pValue3[i-m_STIFF_MAX_NUM] = 0.0;
		}
	}

// 	if(m_bConsiderWarpingEffect)
// 	{
// 		if(m_bPSCDesign)
// 		{
// 			double *pValuePSCDesign[7];
// 			CSectUtil::MakeMapStiffPSCDesign(m_pPSCDesign, pValuePSCDesign, m_tapered, m_iOrj);
// 			
// 			for(i = m_STIFF_MAX_NUM+D_STIFF_WARPING_MAX_NUM; i<m_STIFF_MAX_NUM+D_STIFF_WARPING_MAX_NUM+D_STIFF_PSC_DESIGN; i++)
// 			{
// 				csNum = GetEditingValue(D_STIFF_START_ROW+i, 2);
// 				if (GetDoubleValue(csNum, dNum)) *pValuePSCDesign[i-m_STIFF_MAX_NUM+D_STIFF_WARPING_MAX_NUM] = dNum;
// 				else                             *pValuePSCDesign[i-m_STIFF_MAX_NUM+D_STIFF_WARPING_MAX_NUM] = 0.0;
// 			}
// 		}
// 	}
// 	else
// 	{
// 		if(m_bPSCDesign)
// 		{
// 			double *pValuePSCDesign[7];
// 			CSectUtil::MakeMapStiffPSCDesign(m_pPSCDesign, pValuePSCDesign, m_tapered, m_iOrj);
// 			
// 			for(i = m_STIFF_MAX_NUM; i<m_STIFF_MAX_NUM+D_STIFF_PSC_DESIGN; i++)
// 			{
// 				csNum = GetEditingValue(D_STIFF_START_ROW+i, 2);
// 				if (GetDoubleValue(csNum, dNum)) *pValuePSCDesign[i-m_STIFF_MAX_NUM] = dNum;
// 				else                             *pValuePSCDesign[i-m_STIFF_MAX_NUM] = 0.0;
// 			}
// 		}
// 	}	

	return TRUE;
}

//---------------------------------------------------------------------------
// Implementation Function
BOOL CCMSectItemGrid::SetLeftValidCell(ROWCOL nRow, ROWCOL nCol)
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
				if (SetCurrentCell(i, j)) return TRUE;
			}
			j--;  // 이전 열을 조사
		}
		i--;  // 이전 줄로 이동
		j = GetColCount();
	}

	return FALSE;
}

BOOL CCMSectItemGrid::SetRightValidCell(ROWCOL nRow, ROWCOL nCol)
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
				if (SetCurrentCell(i, j)) return TRUE;
			}
			j++;  // 다음 열을 조사
		}
		i++;  // 다음 줄로 이동
		j = 1;
	}

	return FALSE;
}

void CCMSectItemGrid::OnSizeChange()
{
	if (m_pPreview != 0) 
	{
		m_pPreview->SetCentroidFlag(FALSE);
		m_pPreview->Invalidate();
	}
}

CString CCMSectItemGrid::GetEditingValue(ROWCOL nRow, ROWCOL nCol)
{
	CString csNum;
	BOOL bGet = FALSE;
	ROWCOL ncRow, ncCol;
	if (GetCurrentCell(ncRow, ncCol))
	{
		if (ncRow == nRow && ncCol == nCol)
		{
			CGXControl* pControl = GetControl(nRow, nCol);
			if (pControl && pControl->IsActive())
				bGet = pControl->GetValue(csNum);
		}
	}
	if (!bGet) csNum = GetValueRowCol(nRow, nCol);
	return csNum;
}

BEGIN_MESSAGE_MAP(CCMSectItemGrid, CGXGridWnd)
	//{{AFX_MSG_MAP(CCMSectItemGrid)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CCMSectItemGrid message handlers
/////////////////////////////////////////////////////////////////////////////
BOOL CCMSectItemGrid::ProcessKeys(CWnd *pSender, UINT nMessage, UINT nChar, UINT nRepCnt, UINT flags)
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
				SetRightValidCell(nRow, nCol+1);
			if (nChar == VK_DOWN) return TRUE;
			break;
		case VK_LEFT: case VK_UP: // 이전 셀로 이동
			if (nRow > 1)
				SetLeftValidCell(nRow, nCol-1);
			if (nChar == VK_UP) return TRUE;
			break;
		}
	}
	return CGXGridWnd::ProcessKeys(pSender, nMessage, nChar, nRepCnt, flags);
}

BOOL CCMSectItemGrid::OnValidateCell(ROWCOL nRow, ROWCOL nCol)
{
	CString csNum = GetEditingValue(nRow, nCol);
	double dNum;
	if (GetDoubleValue(csNum, dNum)) return TRUE;
	return FALSE;
}

BOOL CCMSectItemGrid::OnActivateGrid(BOOL bActivate)
{
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
	return CGXGridWnd::OnActivateGrid(bActivate);
}

void CCMSectItemGrid::OnModifyCell(ROWCOL nRow, ROWCOL nCol)
{
	CGXGridWnd::OnModifyCell(nRow, nCol);
	if (nRow >=D_SIZE_START_ROW && nRow <= D_SIZE_START_ROW+D_SIZE_MAX_NUM-1) 
	{
		CString csNum = GetEditingValue(nRow, nCol);
		double dNum;
		if (GetDoubleValue(csNum, dNum)) 
			m_pSectBase->Size[nRow-2] = dNum;
		else m_pSectBase->Size[nRow-2] = 0.0;

		SetStyleOnSizeCell();

		OnSizeChange();
	}
	// stress position
	else if (nRow >= D_STIFF_START_ROW+m_STIFF_MAX_NUM-8-2 && nRow <= D_STIFF_START_ROW+m_STIFF_MAX_NUM-1-2)
	{
		CString csNum = GetEditingValue(nRow, nCol);
		double dNum;
		if (!GetDoubleValue(csNum, dNum)) dNum = 0.0;

		// Zyy, Zzz 추가로 2뺀다.
				 if (nRow == D_STIFF_START_ROW+m_STIFF_MAX_NUM-8-2) m_pStiff->dx1 = dNum;
		else if (nRow == D_STIFF_START_ROW+m_STIFF_MAX_NUM-7-2) m_pStiff->dy1 = dNum;
		else if (nRow == D_STIFF_START_ROW+m_STIFF_MAX_NUM-6-2) m_pStiff->dx2 = dNum;
		else if (nRow == D_STIFF_START_ROW+m_STIFF_MAX_NUM-5-2) m_pStiff->dy2 = dNum;
		else if (nRow == D_STIFF_START_ROW+m_STIFF_MAX_NUM-4-2) m_pStiff->dx3 = dNum;
		else if (nRow == D_STIFF_START_ROW+m_STIFF_MAX_NUM-3-2) m_pStiff->dy3 = dNum;
		else if (nRow == D_STIFF_START_ROW+m_STIFF_MAX_NUM-2-2) m_pStiff->dx4 = dNum;
		else if (nRow == D_STIFF_START_ROW+m_STIFF_MAX_NUM-1-2) m_pStiff->dy4 = dNum;

		OnSizeChange();
	}
	else if (nRow >= D_STIFF_START_ROW+m_STIFF_MAX_NUM-1)
	{
		CString csNum = GetEditingValue(nRow, nCol);
		double dNum;
		if (!GetDoubleValue(csNum, dNum)) dNum = 0.0;

				 if (nRow == D_STIFF_START_ROW+m_STIFF_MAX_NUM+0) m_pStiff->dIw = dNum;
		else if (nRow == D_STIFF_START_ROW+m_STIFF_MAX_NUM+1) m_pStiff->dw1 = dNum;
		else if (nRow == D_STIFF_START_ROW+m_STIFF_MAX_NUM+2) m_pStiff->dw2 = dNum;
		else if (nRow == D_STIFF_START_ROW+m_STIFF_MAX_NUM+3) m_pStiff->dw3 = dNum;
		else if (nRow == D_STIFF_START_ROW+m_STIFF_MAX_NUM+4) m_pStiff->dw4 = dNum;
		else if (nRow == D_STIFF_START_ROW+m_STIFF_MAX_NUM+5) m_pStiff->dxy1 = dNum;
		else if (nRow == D_STIFF_START_ROW+m_STIFF_MAX_NUM+6) m_pStiff->dxy2 = dNum;
		else if (nRow == D_STIFF_START_ROW+m_STIFF_MAX_NUM+7) m_pStiff->dxy3 = dNum;
		else if (nRow == D_STIFF_START_ROW+m_STIFF_MAX_NUM+8) m_pStiff->dxy4 = dNum;
		else if (nRow == D_STIFF_START_ROW+m_STIFF_MAX_NUM+9) m_pStiff->dxz1 = dNum;
		else if (nRow == D_STIFF_START_ROW+m_STIFF_MAX_NUM+10) m_pStiff->dxz2 = dNum;
		else if (nRow == D_STIFF_START_ROW+m_STIFF_MAX_NUM+11) m_pStiff->dxz3 = dNum;
		else if (nRow == D_STIFF_START_ROW+m_STIFF_MAX_NUM+12) m_pStiff->dxz4 = dNum;
		else if (nRow == D_STIFF_START_ROW+m_STIFF_MAX_NUM+13) m_pStiff->dYcy = dNum;
		else if (nRow == D_STIFF_START_ROW+m_STIFF_MAX_NUM+14) m_pStiff->dYcz = dNum;
		else if (nRow == D_STIFF_START_ROW+m_STIFF_MAX_NUM+15) m_pStiff->dIp = dNum;

		OnSizeChange();
	}
}

BOOL CCMSectItemGrid::GetPeriAndStiff()
{
	// Regular Section 단면 계산을 사용한다.
	T_SECT_D data;
	data.Initialize();
	data.nStype = D_SECT_TYPE_REGULAR;
	data.SectBefore.bConsiderWarpingEffect = m_bConsiderWarpingEffect;
	data.SectBefore.Shape = CSectUtil::GetShapeNameFromIndexReg(m_nShapeIndex);
	data.SectBefore.nCellShape = *m_pCellNumber;
	data.SectBefore.SectI = *m_pSectBase;

	// Compo Gen 을 위해 아래 세개 복사. 
	// Compo Gen 이 아닌 경우는 결국 m_pSectBase 안의 값을 복사
	data.SectBefore.SectI.Stiffness = *m_pStiff;
	data.SectBefore.SectI.PeriIn = *m_pPeriIn;
	data.SectBefore.SectI.PeriOut = *m_pPeriOut;
	data.SectBefore.SectI.Design.YBar = *m_pYBar;
	data.SectBefore.SectI.Design.ZBar = *m_pZBar;

	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	BOOL bOK = pDoc->m_pSectDB->CalcSectData(data);

	// peri and stiffness
	*m_pPeriIn = data.SectBefore.SectI.PeriIn;
	*m_pPeriOut = data.SectBefore.SectI.PeriOut;
	*m_pYBar = data.SectBefore.SectI.Design.YBar;
	*m_pZBar = data.SectBefore.SectI.Design.ZBar;
	m_pSectBase->Design.Zyy = data.SectBefore.SectI.Design.Zyy;
	m_pSectBase->Design.Zzz = data.SectBefore.SectI.Design.Zzz;
	*m_pStiff = data.SectBefore.SectI.Stiffness;
	
	//m_pSectBase->Design =data.SectBefore.SectI.Design; // user section은 설계 안됨

	return bOK;
}

void CCMSectItemGrid::OnClickedButtonRowCol(ROWCOL nRow, ROWCOL nCol)
{


	T_SECT_SECTBASE_D *pSect;
	if (nRow == D_STIFF_START_ROW-2)
	{
		GetPeriAndStiff();
		ShowData();
		OnSizeChange();
		return;
	}
	else if(nRow==D_STIFF_START_ROW-1)
	{
		pSect =m_pSectBase;
	}
	else
	{
		CGXGridWnd::OnClickedButtonRowCol(nRow, nCol);
		return;
	}
	CCMSectItemImportReg dlg;
	if(m_nSectType == D_SECT_TYPE_COMPO_G)
		dlg.SetTargetShape(CString(D_SECT_SHAPE_COMPO_G));
	else
		dlg.SetTargetShape(CString(D_SECT_SHAPE_REG_GEN));

	
	if (dlg.DoModal() == IDOK)
	{
		T_SECT_K key = dlg.m_SectKey;
		m_SectKOrg = key;
		T_SECT_D SectData;
		CDBDoc* pDoc = CDBDoc::GetDocPoint();
		pDoc->m_pAttrCtrl->GetSect(key, SectData);
		
		pSect->Stiffness = SectData.SectBefore.SectI.Stiffness;
		pSect->PeriIn = SectData.SectBefore.SectI.PeriIn;
		pSect->PeriOut = SectData.SectBefore.SectI.PeriOut;
		pSect->Design.YBar = SectData.SectBefore.SectI.Design.YBar;
		pSect->Design.ZBar = SectData.SectBefore.SectI.Design.ZBar;
		pSect->Design.Zyy = SectData.SectBefore.SectI.Design.Zyy;
		pSect->Design.Zzz = SectData.SectBefore.SectI.Design.Zzz;
		pSect->aOuterPolygon.Copy(SectData.SectBefore.SectI.aOuterPolygon);
		pSect->aInnerPolygon.Copy(SectData.SectBefore.SectI.aInnerPolygon);
		pSect->aLine.Copy(SectData.SectBefore.SectI.aLine);
		pSect->aLineLoop.Copy(SectData.SectBefore.SectI.aLineLoop);
		pSect->aShape.Copy(SectData.SectBefore.SectI.aShape);
	 
		ShowData();
		GetParent()->PostMessage(CCMSectItemGrid::WM_GRID_DATA_CHANGED, 0, m_iOrj);
	}  
}

BOOL CCMSectItemGrid::GetDoubleValue(CString& csNum, double& dNum)
{
	int ecode;
	if (!IsValidDoubleNumber(csNum, ecode)) return InvalidDoubleNumber(ecode);
	ConvertFormulaToValue(csNum);
	dNum = _tstof(csNum);
	return TRUE;
}

// nErrCode : 1(수식 에러), 2(숫자 아님), 3(범위 초과)
BOOL CCMSectItemGrid::IsValidDoubleNumber(CString value, int &nErrCode)
{
	value.TrimLeft();
	value.TrimRight();
	if (value == _T("")) return TRUE; // 공백은 0으로 생각하여 TRUE 리턴
	// 수식이면 변환, 수식이 아니면 TRUE 반환(수식은 첫자가 '=' 임)
	if (!ConvertFormulaToValue(value)) { nErrCode = 1; return FALSE; }

	int ecode;
	// 숫자인지 문자열인지 검사
	if (!IsNumeric(value, ecode))
	{
		if (ecode == 1) nErrCode = 3;
		else nErrCode = 2;
		return FALSE;
	}

	TCHAR *num = new TCHAR[value.GetLength()+1];
	TCHAR *endp;
	double n;

	_stprintf(num, value);
	n = _tcstod(num, &endp);
	delete[] num;

	// 실수 범위 검사
	if (n == HUGE_VAL || n == -HUGE_VAL) { nErrCode = 3; return FALSE; }

	return TRUE;
}

BOOL CCMSectItemGrid::ConvertFormulaToValue(CString& value)
{
	if (GetSheetContext() == NULL && value.GetLength() > 0 && value.GetAt(0) == _T('='))
	{
		// 대문자 E로 표시된 지수를 소문자 e로 변경
		value.Replace(_T("E"), _T("e"));

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

BOOL CCMSectItemGrid::IsNumeric(CString str, int& ecode)
{
	if (str.GetLength() < 1) return FALSE;

	TCHAR *num = new TCHAR[str.GetLength()+1];
	TCHAR *endp;
	double n;

	_stprintf(num, str);
	n = _tcstod(num, &endp);
	BOOL bAllScaned = endp == &num[str.GetLength()];
	delete[] num;

	if (n == HUGE_VAL || n == -HUGE_VAL) 
		ecode = 1;
	else if (!bAllScaned) 
		ecode = 2;
	else ecode = 0;

	if (ecode) return FALSE;
	else return TRUE;
}

BOOL CCMSectItemGrid::InvalidDoubleNumber(int nErrCode)
{
	switch(nErrCode)
	{
	case 1: return ValidateError(_LS(IDS_CMD0417__Formula_is_invalid_)); 
	case 2: return ValidateError(_LS(IDS_CMD0417__Character_exist_in_the_number_));
	case 3: return ValidateError(_LS(IDS_CMD0417__Out_of_value_range_));
	}
	return FALSE;
}

BOOL CCMSectItemGrid::ValidateError(LPCTSTR lpszError)
{
	SetWarningText(lpszError);
	return FALSE;
}

BOOL CCMSectItemGrid::OnEndEditing(ROWCOL nRow, ROWCOL nCol)
{
	BOOL bModified = GetControl(nRow, nCol)->GetModify();
	BOOL bResult = CGXGridWnd::OnEndEditing(nRow, nCol);
	if (bModified)
	{
		CString value = GetValueRowCol(nRow, nCol);
		if (ConvertFormulaToValue(value)) SetExpressionRowCol(nRow, nCol, value);
	}
	return bResult;
}

BOOL CCMSectItemGrid::OnPasteFromClipboard(const CGXRange &range)
{
	if (!CGXGridWnd::OnPasteFromClipboard(range)) return FALSE;
	SaveData();
	OnSizeChange();
	//Apply시 에러체킹한다.
	//m_pOtherEnd->m_pSectBase->BuiltUpFlag = m_pSectBase->BuiltUpFlag;
	//m_pOtherEnd->ShowData();
	return TRUE;
}
void CCMSectItemGrid::SetTapered(BOOL tapered, int iOrj)  // i=0, j=1
{
	m_tapered=tapered;
	m_iOrj = iOrj;
}
BOOL CCMSectItemGrid::IsTapered()
{
	return m_tapered;
}

