// CMSecInputGrid.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMSecInputGrid.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_SectUtil.h"

#include "..\wg_common\wg_common_TBUtilFunc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCMSecInputGrid

CCMSecInputGrid::CCMSecInputGrid()
{
	m_pParam = NULL;
	m_pDoc = 0;
}

CCMSecInputGrid::~CCMSecInputGrid()
{
	if (m_pParam != NULL) delete m_pParam;
}

/////////////////////////////////////////////////////////////////////////////
// CCMSecInputGrid interfaces

void CCMSecInputGrid::InitGrid(CDBDoc* pDoc)
{
	// set DB
	m_pDoc = pDoc;

	// section specific items initialization
	Initialize();
	InitSectionByData(0, FALSE);
}

void CCMSecInputGrid::InitSectionByData(T_SECT_D* pDataSrc, BOOL bAfter)
{
	if (pDataSrc != 0)
	{
		switch(pDataSrc->nStype)
		{
		case D_SECT_TYPE_REGULAR:
			InitGridForRegular(pDataSrc); // 테이블의 형태를 정한다.(Row갯수, Title, Unit, Value)
			break;
		case D_SECT_TYPE_SRC:
			InitGridForSrc(pDataSrc); break;
		case D_SECT_TYPE_USER:
			InitGridForUser(pDataSrc); break;
		case D_SECT_TYPE_COMBINED:
			InitGridForCombined(pDataSrc); break;
		case D_SECT_TYPE_TAPERED:
			InitGridForTapered(pDataSrc); break;
		case D_SECT_TYPE_CONSTRUCTION:
			{
				T_SECT_D Sect;
				CSectUtil::GetSectFromConPart(*pDataSrc, Sect, bAfter);
				//if (Sect.nStype == D_SECT_TYPE_REGULAR)
					Sect.SName = Sect.SectBefore.SectI.SName;
				//else Sect.SName = _T("");
				InitSectionByData(&Sect, FALSE);
			}
			break;
		default: 
			HideRows(1, GetRowCount());
			break;
		}
		SetRowHeight(0, GetRowCount(), globalUtils.ScaleByDPI(18));
	}
	else
	{
		HideRows(1, GetRowCount());
	}
	LockUpdate(TRUE);
	InitGridParam();
	CTBUtilFunc::SetTitleByLanguage(this);
//NX 버전에서 폰트는 일괄 관리한다.
//#if !defined(_ORG)  // 국내 버전 아니면 해당 나라별 Font로 변경
//	GetParam()->SetLockReadOnly(FALSE);
//	SetStyleRange(CGXRange(1, 1, GetRowCount(), GetColCount()), 
//								CGXStyle().SetFont(CGXFont_GC().SetFaceName(_LS(IDS_CMD_FONT_Ms_Sans_Serif))));
//	GetParam()->SetLockReadOnly(TRUE);
//#endif
	LockUpdate(FALSE);
	Redraw();
}

/////////////////////////////////////////////////////////////////////////////
// CCMSecInputGrid implementations
BOOL CCMSecInputGrid::ProcessKeys(CWnd *pSender, UINT nMessage, UINT nChar, UINT nRepCnt, UINT flags)
{
	CRowColArray awRows;
	ROWCOL nRow=1, nCol=1;
	CGXControl* pControl;

	if (!GetCurrentCell(nRow, nCol)) // if no current cell, default action
		return CGXGridWnd::ProcessKeys(pSender, nMessage, nChar, nRepCnt, flags);

	pControl = GetControl(nRow, nCol);
	if (nMessage == WM_KEYDOWN)
	{
		switch(nChar)
		{
		case VK_TAB: 
			break;
		case VK_RETURN: case VK_RIGHT:  // 다음 셀로 이동
			if (nRow < GetRowCount())
				SetCurrentCell(nRow+1, nCol);
			break;
		case VK_LEFT: // 이전 셀로 이동
			if (nRow > 1)
				SetCurrentCell(nRow-1, nCol);
			break;
		}
	}
	else if (nMessage == WM_CHAR)
	{
		switch(nChar)
		{
		// 셀이 Inactive 상태에서 Edit 셀에 스페이스를 눌렀을 때 편집을 활성화 시킨다.
		case VK_SPACE:
			if (pControl && !pControl->IsActive() 
				&& pControl->CGXControl::IsKindOf(CONTROL_CLASS(CGXEditControl)))
			{
				if (!pControl->IsInit()) pControl->Init(nRow, nCol);
				CString value = GetExpressionRowCol(nRow, nCol);
				pControl->SetActive(TRUE);
				pControl->Refresh(); // 반드시 호출

				if (!pControl->IsKindOf(CONTROL_CLASS(CGXComboBox)))  // combo box가 아니면

					((CGXEditControl*)pControl)->SetSel(0, value.GetLength());
				return TRUE;
			}
			break;
		}
	}
	return CGXGridWnd::ProcessKeys(pSender, nMessage, nChar, nRepCnt, flags);
}

void CCMSecInputGrid::InitGridParam()
{
	// cell의 width와 height 크기 변경을 금지한다.
	GetParam()->EnableTrackRowHeight(FALSE);
	GetParam()->EnableTrackColWidth(FALSE);

	// column과 row의 위치 이동을 금지시킨다.
	GetParam()->EnableMoveCols(FALSE);
	GetParam()->EnableMoveRows(FALSE);

	// double click시 셀 편집이 되도록 한다.
	GetParam()->SetActivateCellFlags(GX_CAFOCUS_DBLCLICKONCELL);

	// Row 헤더와 Column 헤더를 숨긴다.
	HideRows(0, 0);
	HideCols(0, 0);

	// always show vertical scrollbar
	SetScrollBarMode(SB_VERT, gxnEnabled | gxnEnhanced);  

	// set column width
	SetColWidth(1, 1, globalUtils.ScaleByDPI(60));
	SetColWidth(2, 2, globalUtils.ScaleByDPI(100));
	SetColWidth(3, 3, globalUtils.ScaleByDPI(60));

	SetReadOnly(TRUE);
}

void CCMSecInputGrid::InitGridForRegular(T_SECT_D* pDataSrc)
{
	CSectUtil SecUtil;
	CStringArray aSizeTitle;

	if (m_pParam != NULL) delete m_pParam;  // 기존 param 변수를 해제한다.
	m_pParam = CGXGridParam::CreateFromResource(_T("CCM_GRID_REG"));
	SetParam(m_pParam, FALSE);
	Initialize();
	LockUpdate(TRUE);
	GetParam()->EnableUndo(FALSE);  // undo를 끈다.

	int nShapeIndex = SecUtil.GetShapeIndexFromNameReg(pDataSrc->SectBefore.Shape);
	SecUtil.GetSizeTitleFromShapeIndexReg(nShapeIndex, aSizeTitle);

	if(nShapeIndex==1) // Channel, 임시 KJH20040125 => Channel의 경우 r1, r2는 DB에서만 고려
	{
		aSizeTitle.RemoveAt(6,2);
	}
	int nSizeCount = aSizeTitle.GetSize();

	int nSizeStartRow = 8;
	int nSizeEndRow = 15;
	// Size에서 필요 없는 부분을 숨긴다.
	if (nSizeCount < 8) HideRows(nSizeStartRow+nSizeCount, nSizeEndRow);

	// Size의 치수 이름을 설정한다.
	for (int i = 0; i < nSizeCount; i++)
		SetValueRange(CGXRange(nSizeStartRow+i, 1), aSizeTitle.GetAt(i));

	// 단위계 이름을 설정한다.
	CString aUnit[8];
	T_SECT_SECTBASE_UNIT *pUnit = &(CUnitCtrl::m_SECT_UNIT.SectBefore.SectI);
	for (int i = 0; i < 8; i++)
		m_pDoc->m_pUnitCtrl->GetUnitSystem(pUnit->Size, aUnit[i]);
	for (int i = 0; i < nSizeCount; i++)
		SetValueRange(CGXRange(nSizeStartRow+i, 3), aUnit[i]);

	// *^^*  연희가 선과장님 없을 때 고친 부분


	// 값을 설정한다. ------------------------------------------------------
	T_SECT_SECTION_D* pSectData;
	T_SECT_SECTBASE_D* pBaseData;
	pSectData = &pDataSrc->SectBefore;
	pBaseData = &pSectData->SectI;

	SetValueRange(CGXRange(2, 1), pBaseData->DBName); // DB
	SetValueRange(CGXRange(4, 1), pDataSrc->SName); // Name
	SetValueRange(CGXRange(6, 1), pSectData->Shape); // Shape

	CString strBuiltUp;
	int nBuiltUp = pSectData->SectI.BuiltUpFlag;
	if (nBuiltUp == 0 || nBuiltUp == 1) strBuiltUp = _LS(IDS_WG_CMD__ADDD__Built_Up);
	else strBuiltUp = _LS(IDS_WG_CMD__ADDD__Rolled);
	SetValueRange(CGXRange(7, 2), strBuiltUp); // Built-Up

	for (int i = 0; i < nSizeCount; i++)
		SetValueRange(CGXRange(nSizeStartRow+i, 2), pBaseData->Size[i]);

	LockUpdate(FALSE);
}

void CCMSecInputGrid::InitGridForSrc(T_SECT_D* pDataSrc)
{
	CSectUtil SecUtil;
	CStringArray aSizeTitleConcr;
	CStringArray aSizeTitleSteel;

	if (m_pParam != NULL) delete m_pParam;  // 기존 param 변수를 해제한다.
	m_pParam = CGXGridParam::CreateFromResource(_T("CCM_GRID_SRC"));
	SetParam(m_pParam, FALSE);
	Initialize();
	LockUpdate(TRUE);
	GetParam()->EnableUndo(FALSE);  // undo를 끈다.

	int nShapeIndex = SecUtil.GetShapeIndexFromNameSrc(pDataSrc->SectBefore.Shape);
	SecUtil.GetSizeTitleFromShapeIndexSrcConcr(nShapeIndex, aSizeTitleConcr);
	SecUtil.GetSizeTitleFromShapeIndexSrcSteel(nShapeIndex, aSizeTitleSteel);

	int nSizeCountConcr = aSizeTitleConcr.GetSize();
	int nSizeCountSteel = aSizeTitleSteel.GetSize();

	int nHideFromRow, nHideToRow;
	// Concrete Data에서 필요없는 부분을 숨긴다.
	if (nSizeCountConcr == 0) nHideFromRow = 5;
	else nHideFromRow = 6+nSizeCountConcr;
	nHideToRow = 7;
	if (nSizeCountConcr < 2) HideRows(nHideFromRow, nHideToRow);
	// Steel Data에서 필요없는 부분을 숨긴다.
	nHideFromRow = 12+nSizeCountSteel;
	nHideToRow = 19;
	if (nSizeCountSteel < 8) HideRows(nHideFromRow, nHideToRow);

	// Concrete의 치수 이름을 설정한다.
	for (int i = 0; i < nSizeCountConcr; i++)
		SetValueRange(CGXRange(6+i, 1), aSizeTitleConcr.GetAt(i));
	// Steel의 치수 이름을 설정한다.
	for (int i = 0; i < nSizeCountSteel; i++)
		SetValueRange(CGXRange(12+i, 1), aSizeTitleSteel.GetAt(i));

	// Concrete의 단위계 이름을 설정한다.
	CString aUnit[8];
	T_SECT_SECTBASE_UNIT *pUnit = &(CUnitCtrl::m_SECT_UNIT.SectBefore.SectJ);
	for (int i = 0; i < 2; i++)
		m_pDoc->m_pUnitCtrl->GetUnitSystem(pUnit->Size, aUnit[i]);
	for (int i = 0; i < nSizeCountConcr; i++)
		SetValueRange(CGXRange(6+i, 3), aUnit[i]);

	pUnit = &(CUnitCtrl::m_SECT_UNIT.SectBefore.SectI);
	for (int i = 0; i < 8; i++)
		m_pDoc->m_pUnitCtrl->GetUnitSystem(pUnit->Size, aUnit[i]);
	for (int i = 0; i < nSizeCountSteel; i++)
		SetValueRange(CGXRange(12+i, 3), aUnit[i]);

	// 값을 설정한다. ------------------------------------------------------
	T_SECT_SECTION_D* pSectData;
	T_SECT_SECTBASE_D* pBaseDataConcr;
	T_SECT_SECTBASE_D* pBaseDataSteel;
	pSectData = &pDataSrc->SectBefore;
	pBaseDataConcr = &pSectData->SectJ;
	pBaseDataSteel = &pSectData->SectI;

	SetValueRange(CGXRange(2, 1), pDataSrc->SName); // Section Name
	SetValueRange(CGXRange(4, 1), pSectData->Shape); // Shape

	// concrete data
	for (int i = 0;i < nSizeCountConcr; i++)
		SetValueRange(CGXRange(6+i, 2), pBaseDataConcr->Size[i]);

	// steel data
	SetValueRange(CGXRange(9, 2), pBaseDataSteel->DBName);
	SetValueRange(CGXRange(10, 2), pBaseDataSteel->SName);
	CString strBuiltUp;
	int nBuiltUp = pSectData->SectI.BuiltUpFlag;
	if (nBuiltUp == 0 || nBuiltUp == 1) strBuiltUp = _LS(IDS_WG_CMD__ADDD__Built_Up);
	else strBuiltUp = _LS(IDS_WG_CMD__ADDD__Rolled);
	SetValueRange(CGXRange(11, 2), strBuiltUp); // Built-Up
	for (int i = 0; i < nSizeCountSteel; i++)
		SetValueRange(CGXRange(12+i, 2), pBaseDataSteel->Size[i]);

	// material data
	SetValueRange(CGXRange(21, 2), pSectData->Matl_Elast);
	SetValueRange(CGXRange(22, 2), pSectData->Matl_Density);
	SetValueRange(CGXRange(23, 2), pSectData->Matl_Poisson_S);
	SetValueRange(CGXRange(24, 2), pSectData->Matl_Poisson_C);
	CString strReplace;
	int nReplace = pSectData->nSRCtype;
	if (nReplace == 0 || nReplace == 1) strReplace = _LS(IDS_WG_CMD__ADDD__Steel);
	else strReplace = _LS(IDS_WG_CMD__ADDD__Concrete);
	SetValueRange(CGXRange(25, 2), strReplace);

	LockUpdate(FALSE);
}

void CCMSecInputGrid::InitGridForUser(T_SECT_D* pDataSrc)
{
	CSectUtil SecUtil;
	CStringArray aSizeTitle;

	if (m_pParam != NULL) delete m_pParam;  // 기존 param 변수를 해제한다.
	m_pParam = CGXGridParam::CreateFromResource(_T("CCM_GRID_USR"));
	SetParam(m_pParam, FALSE);
	Initialize();
	LockUpdate(TRUE);
	GetParam()->EnableUndo(FALSE);  // undo를 끈다.

	int nShapeIndex = SecUtil.GetShapeIndexFromNameReg(pDataSrc->SectBefore.Shape);
	SecUtil.GetSizeTitleFromShapeIndexReg(nShapeIndex, aSizeTitle);

	if(nShapeIndex==1) // Channel, 임시 KJH20040125 => Channel의 경우 r1, r2는 DB에서만 고려
	{
		aSizeTitle.RemoveAt(6,2);
	}
	int nSizeCount = aSizeTitle.GetSize();

	int nSizeStartRow = 6;
	int nSizeEndRow = 13;
	// Size에서 필요 없는 부분을 숨긴다.
	if (nSizeCount < 8) HideRows(nSizeStartRow+nSizeCount, nSizeEndRow);

	// Size의 치수 이름을 설정한다.
	for (int i = 0; i < nSizeCount; i++)
		SetValueRange(CGXRange(nSizeStartRow+i, 1), aSizeTitle.GetAt(i));

	// 단위계 이름을 설정한다.
	CString aUnit[8];
	CString aUnitStiff[14];
	T_SECT_SECTBASE_UNIT *pUnit = &(CUnitCtrl::m_SECT_UNIT.SectBefore.SectI);
	for (int i = 0; i < 8; i++)
		m_pDoc->m_pUnitCtrl->GetUnitSystem(pUnit->Size, aUnit[i]);
	T_SECT_STIFFNESS_UNIT *pUnitStiff = &(CUnitCtrl::m_SECT_UNIT.SectBefore.SectI.Stiffness);
	m_pDoc->m_pUnitCtrl->GetUnitSystem(pUnitStiff->Area, aUnitStiff[0]);
	m_pDoc->m_pUnitCtrl->GetUnitSystem(pUnitStiff->Asy, aUnitStiff[1]);
	m_pDoc->m_pUnitCtrl->GetUnitSystem(pUnitStiff->Asz, aUnitStiff[2]);
	m_pDoc->m_pUnitCtrl->GetUnitSystem(pUnitStiff->Rxx, aUnitStiff[3]);
	m_pDoc->m_pUnitCtrl->GetUnitSystem(pUnitStiff->Ryy, aUnitStiff[4]);
	m_pDoc->m_pUnitCtrl->GetUnitSystem(pUnitStiff->Rzz, aUnitStiff[5]);
	m_pDoc->m_pUnitCtrl->GetUnitSystem(pUnitStiff->Cyp, aUnitStiff[6]);
	m_pDoc->m_pUnitCtrl->GetUnitSystem(pUnitStiff->Cym, aUnitStiff[7]);
	m_pDoc->m_pUnitCtrl->GetUnitSystem(pUnitStiff->Czp, aUnitStiff[8]);
	m_pDoc->m_pUnitCtrl->GetUnitSystem(pUnitStiff->Czm, aUnitStiff[9]);
	m_pDoc->m_pUnitCtrl->GetUnitSystem(pUnitStiff->Qyb, aUnitStiff[10]);
	m_pDoc->m_pUnitCtrl->GetUnitSystem(pUnitStiff->Qzb, aUnitStiff[11]);
	m_pDoc->m_pUnitCtrl->GetUnitSystem(pUnit->PeriOut, aUnitStiff[12]);
	m_pDoc->m_pUnitCtrl->GetUnitSystem(pUnit->PeriIn, aUnitStiff[13]);

	for (int i = 0; i < nSizeCount; i++)
		SetValueRange(CGXRange(nSizeStartRow+i, 3), aUnit[i]);
	for (int i = 0; i < 14; i++)
		SetValueRange(CGXRange(15+i, 3), aUnitStiff[i]);

	// 값을 설정한다. ------------------------------------------------------
	T_SECT_SECTION_D* pSectData;
	T_SECT_SECTBASE_D* pBaseData;
	pSectData = &pDataSrc->SectBefore;
	pBaseData = &pSectData->SectI;

	SetValueRange(CGXRange(2, 1), pDataSrc->SName); // Name
	SetValueRange(CGXRange(4, 1), pSectData->Shape); // Shape

	CString strBuiltUp;
	int nBuiltUp = pSectData->SectI.BuiltUpFlag;
	if (nBuiltUp == 0 || nBuiltUp == 1) strBuiltUp = _LS(IDS_WG_CMD__ADDD__Built_Up);
	else strBuiltUp = _LS(IDS_WG_CMD__ADDD__Rolled);
	SetValueRange(CGXRange(5, 2), strBuiltUp); // Built-Up

	for (int i = 0; i < nSizeCount; i++)
		SetValueRange(CGXRange(nSizeStartRow+i, 2), pBaseData->Size[i]);

	// stiffness data
	T_SECT_STIFFNESS * pStiff = &pSectData->SectI.Stiffness;
	SetValueRange(CGXRange(15, 2), pStiff->Area);
	SetValueRange(CGXRange(16, 2), pStiff->Asy);
	SetValueRange(CGXRange(17, 2), pStiff->Asz);
	SetValueRange(CGXRange(18, 2), pStiff->Rxx);
	SetValueRange(CGXRange(19, 2), pStiff->Ryy);
	SetValueRange(CGXRange(20, 2), pStiff->Rzz);
	SetValueRange(CGXRange(21, 2), pStiff->Cyp);
	SetValueRange(CGXRange(22, 2), pStiff->Cym);
	SetValueRange(CGXRange(23, 2), pStiff->Czp);
	SetValueRange(CGXRange(24, 2), pStiff->Czm);
	SetValueRange(CGXRange(25, 2), pStiff->Qyb);
	SetValueRange(CGXRange(26, 2), pStiff->Qzb);
	SetValueRange(CGXRange(27, 2), pSectData->SectI.PeriOut);
	SetValueRange(CGXRange(28, 2), pSectData->SectI.PeriIn);

	LockUpdate(FALSE);
}

void CCMSecInputGrid::InitGridForCombined(T_SECT_D* pDataSrc)
{
	CSectUtil SecUtil;
	// determine if it is A type or B type
	// call corresponding initialization function
	if (SecUtil.IsCombinedAType(*pDataSrc)) InitGridForCombinedA(pDataSrc);
	else InitGridForCombinedB(pDataSrc);
}

void CCMSecInputGrid::InitGridForCombinedA(T_SECT_D* pDataSrc)
{
	CSectUtil SecUtil;
	CStringArray aSizeTitleF;
	CStringArray aSizeTitleS;

	if (m_pParam != NULL) delete m_pParam;  // 기존 param 변수를 해제한다.
	m_pParam = CGXGridParam::CreateFromResource(_T("CCM_GRID_COMA"));
	SetParam(m_pParam, FALSE);
	Initialize();
	LockUpdate(TRUE);
	GetParam()->EnableUndo(FALSE);  // undo를 끈다.
	
	int nShapeIndex = SecUtil.GetShapeIndexFromNameComA(pDataSrc->SectBefore.Shape);
	SecUtil.GetSizeTitleFromShapeIndexComAFirst(nShapeIndex, aSizeTitleF);
	SecUtil.GetSizeTitleFromShapeIndexComASecond(nShapeIndex, aSizeTitleS);

	int nSizeCountF = aSizeTitleF.GetSize();
	int nSizeCountS = aSizeTitleS.GetSize();

	int nHideFromRow, nHideToRow;
	// First Section에서 필요없는 부분을 숨긴다.
	nHideFromRow = 9+nSizeCountF;
	nHideToRow = 13;
	if (nSizeCountF < 5) HideRows(nHideFromRow, nHideToRow);
	// Second Section에서 필요없는 부분을 숨긴다.
	nHideFromRow = 16+nSizeCountS;
	nHideToRow = 19;
	if (nSizeCountS < 4) HideRows(nHideFromRow, nHideToRow);

	// First and Second Section의 치수 이름을 설정한다.
	for (int i = 0; i < nSizeCountF; i++)
		SetValueRange(CGXRange(9+i, 1), aSizeTitleF.GetAt(i));
	for (int i = 0; i < nSizeCountS; i++)
		SetValueRange(CGXRange(16+i, 1), aSizeTitleS.GetAt(i));

	// 단위계 이름을 설정한다.
	// First section
	CString aUnit[5];
	T_SECT_SECTBASE_UNIT *pUnit = &(CUnitCtrl::m_SECT_UNIT.SectBefore.SectI);
	for (int i = 0; i < 5; i++)
		m_pDoc->m_pUnitCtrl->GetUnitSystem(pUnit->Size, aUnit[i]);
	for (int i = 0; i < nSizeCountF; i++)
		SetValueRange(CGXRange(9+i, 3), aUnit[i]);
	// Second section
	pUnit = &(CUnitCtrl::m_SECT_UNIT.SectBefore.SectJ);
	for (int i = 0; i < 4; i++)
		m_pDoc->m_pUnitCtrl->GetUnitSystem(pUnit->Size, aUnit[i]);
	for (int i = 0; i < nSizeCountS; i++)
		SetValueRange(CGXRange(16+i, 3), aUnit[i]);

	// 값을 설정한다. ------------------------------------------------------
	T_SECT_SECTION_D* pSectData;
	T_SECT_SECTBASE_D* pBaseDataF;
	T_SECT_SECTBASE_D* pBaseDataS;
	pSectData = &pDataSrc->SectBefore;
	pBaseDataF = &pSectData->SectI;
	pBaseDataS = &pSectData->SectJ;

	SetValueRange(CGXRange(2, 1), pDataSrc->SName); // Section Name
	SetValueRange(CGXRange(4, 1), pSectData->Shape); // Shape
	SetValueRange(CGXRange(6, 1), pBaseDataF->DBName); // DB

	// First Section
	SetValueRange(CGXRange(8, 2), pBaseDataF->SName);
	for (int i = 0; i < nSizeCountF; i++)
		SetValueRange(CGXRange(9+i, 2), pBaseDataF->Size[i]);

	// Second Section
	SetValueRange(CGXRange(15, 2), pBaseDataS->SName);
	for (int i = 0; i < nSizeCountS; i++)
		SetValueRange(CGXRange(16+i, 2), pBaseDataS->Size[i]);

	LockUpdate(FALSE);
}

void CCMSecInputGrid::InitGridForCombinedB(T_SECT_D* pDataSrc)
{
	CSectUtil SecUtil;
	CStringArray aSizeTitleF;
	CStringArray aSizeTitleS;

	if (m_pParam != NULL) delete m_pParam;  // 기존 param 변수를 해제한다.
	m_pParam = CGXGridParam::CreateFromResource(_T("CCM_GRID_COMB"));
	SetParam(m_pParam, FALSE);
	Initialize();
	LockUpdate(TRUE);
	GetParam()->EnableUndo(FALSE);  // undo를 끈다.
	
	int nShapeIndex = SecUtil.GetShapeIndexFromNameComB(pDataSrc->SectBefore.Shape);
	SecUtil.GetSizeTitleFromShapeIndexComBFirst(nShapeIndex, aSizeTitleF);
	SecUtil.GetSizeTitleFromShapeIndexComBSecond(nShapeIndex, aSizeTitleS);

	int nSizeCountF = aSizeTitleF.GetSize();
	int nSizeCountS = aSizeTitleS.GetSize();

	int nHideFromRow, nHideToRow;
	// First Section에서 필요없는 부분을 숨긴다.
	nHideFromRow = 8+nSizeCountF;
	nHideToRow = 11;
	if (nSizeCountF < 4) HideRows(nHideFromRow, nHideToRow);
	// Second Section에서 필요없는 부분을 숨긴다.
	nHideFromRow = 13+nSizeCountS;
	nHideToRow = 14;
	if (nSizeCountS < 2) HideRows(nHideFromRow, nHideToRow);

	// First and Second Section의 치수 이름을 설정한다.
	for (int i = 0; i < nSizeCountF; i++)
		SetValueRange(CGXRange(8+i, 1), aSizeTitleF.GetAt(i));
	for (int i = 0; i < nSizeCountS; i++)
		SetValueRange(CGXRange(13+i, 1), aSizeTitleS.GetAt(i));

	// 단위계 이름을 설정한다.
	// First section
	CString aUnit[5];
	T_SECT_SECTBASE_UNIT *pUnit = &(CUnitCtrl::m_SECT_UNIT.SectBefore.SectI);
	for (int i = 0; i < 4; i++)
		m_pDoc->m_pUnitCtrl->GetUnitSystem(pUnit->Size, aUnit[i]);
	for (int i = 0; i < nSizeCountF; i++)
		SetValueRange(CGXRange(8+i, 3), aUnit[i]);
	// Second section
	pUnit = &(CUnitCtrl::m_SECT_UNIT.SectBefore.SectJ);
	for (int i = 0; i < 2; i++)
		m_pDoc->m_pUnitCtrl->GetUnitSystem(pUnit->Size, aUnit[i]);
	for (int i = 0; i < nSizeCountS; i++)
		SetValueRange(CGXRange(13+i, 3), aUnit[i]);

	// 값을 설정한다. ------------------------------------------------------
	T_SECT_SECTION_D* pSectData;
	T_SECT_SECTBASE_D* pBaseDataF;
	T_SECT_SECTBASE_D* pBaseDataS;
	pSectData = &pDataSrc->SectBefore;
	pBaseDataF = &pSectData->SectI;
	pBaseDataS = &pSectData->SectJ;

	SetValueRange(CGXRange(2, 1), pDataSrc->SName); // Section Name
	SetValueRange(CGXRange(4, 1), pSectData->Shape); // Shape

	// First Section
	SetValueRange(CGXRange(6, 2), pBaseDataF->DBName); // DB
	SetValueRange(CGXRange(7, 2), pBaseDataF->SName);
	for (int i = 0; i < nSizeCountF; i++)
		SetValueRange(CGXRange(8+i, 2), pBaseDataF->Size[i]);

	// Second Section
	for (int i = 0; i < nSizeCountS; i++)
		SetValueRange(CGXRange(13+i, 2), pBaseDataS->Size[i]);

	LockUpdate(FALSE);
}

#define D_STIFF_ROW_NUM 14
void CCMSecInputGrid::InitGridForTapered(T_SECT_D* pDataSrc)
{
	CSectUtil SecUtil;
	CStringArray aSizeTitleI;
	CStringArray aSizeTitleJ;

	if (m_pParam != NULL) delete m_pParam;  // 기존 param 변수를 해제한다.
	m_pParam = CGXGridParam::CreateFromResource(_T("CCM_GRID_TAP"));
	SetParam(m_pParam, FALSE);
	Initialize();
	LockUpdate(TRUE);
	GetParam()->EnableUndo(FALSE);  // undo를 끈다.

	int nShapeIndex = SecUtil.GetShapeIndexFromNameReg(pDataSrc->SectBefore.Shape);
	SecUtil.GetSizeTitleFromShapeIndexReg(nShapeIndex, aSizeTitleI);
	SecUtil.GetSizeTitleFromShapeIndexReg(nShapeIndex, aSizeTitleJ);
	if(nShapeIndex==1) // Channel, 임시 KJH20040125 => Channel의 경우 r1, r2는 DB에서만 고려
	{
		aSizeTitleI.RemoveAt(6,2);
		aSizeTitleJ.RemoveAt(6,2);
	}

	int nSizeCountI = aSizeTitleI.GetSize();
	int nSizeCountJ = aSizeTitleJ.GetSize();

	int nHideFromRow, nHideToRow;
	// Section I 에서 필요없는 부분을 숨긴다.
	nHideFromRow = 9+nSizeCountI;
	nHideToRow = 16;
	if (nSizeCountI < 8) HideRows(nHideFromRow, nHideToRow);
	// Section J 에서 필요없는 부분을 숨긴다.
	nHideFromRow = 20+D_STIFF_ROW_NUM+nSizeCountJ;
	nHideToRow = 27+D_STIFF_ROW_NUM;
	if (nSizeCountJ < 8) HideRows(nHideFromRow, nHideToRow);

	// Section I, Section J의 치수 이름을 설정한다.
	for (int i = 0; i < nSizeCountI; i++)
		SetValueRange(CGXRange(9+i, 1), aSizeTitleI.GetAt(i));
	for (int i = 0; i < nSizeCountJ; i++)
		SetValueRange(CGXRange(20+D_STIFF_ROW_NUM+i, 1), aSizeTitleJ.GetAt(i));

	T_SECT_STIFFNESS_UNIT* pUnitS = &CUnitCtrl::m_SECT_UNIT.SectBefore.SectI.Stiffness;
	int aStiffUnit[] = { 
		pUnitS->Area, pUnitS->Asy, pUnitS->Asz, pUnitS->Rxx, pUnitS->Ryy, pUnitS->Rzz,
		pUnitS->Cyp, pUnitS->Cym, pUnitS->Czp, pUnitS->Czm, pUnitS->Qyb, pUnitS->Qzb,
		D_UNITSYS_BASE_LENGTH, D_UNITSYS_BASE_LENGTH,
	};

	// 단위계 이름을 설정한다.
	// section-I
	CString csUnit;
	CString aUnit[8];
	T_SECT_SECTBASE_UNIT *pUnit = &(CUnitCtrl::m_SECT_UNIT.SectBefore.SectI);
	for (int i = 0; i < 8; i++)
		m_pDoc->m_pUnitCtrl->GetUnitSystem(pUnit->Size, aUnit[i]);
	for (int i = 0; i < nSizeCountI; i++)
		SetValueRange(CGXRange(9+i, 3), aUnit[i]);
	for (int i = 0; i < D_STIFF_ROW_NUM; i++)
	{
		m_pDoc->m_pUnitCtrl->GetUnitSystem(aStiffUnit[i], csUnit);
		SetValueRange(CGXRange(18+i, 3), csUnit);
	}

	// section-J
	pUnit = &(CUnitCtrl::m_SECT_UNIT.SectBefore.SectJ);
	for (int i = 0; i < 8; i++)
		m_pDoc->m_pUnitCtrl->GetUnitSystem(pUnit->Size, aUnit[i]);
	for (int i = 0; i < nSizeCountJ; i++)
		SetValueRange(CGXRange(20+D_STIFF_ROW_NUM+i, 3), aUnit[i]);
	for (int i = 0; i < D_STIFF_ROW_NUM; i++)
	{
		m_pDoc->m_pUnitCtrl->GetUnitSystem(aStiffUnit[i], csUnit);
		SetValueRange(CGXRange(20+D_STIFF_ROW_NUM+8+1+i, 3), csUnit);
	}

	// 값을 설정한다. ------------------------------------------------------
	T_SECT_SECTION_D* pSectData;
	T_SECT_SECTBASE_D* pBaseDataI;
	T_SECT_SECTBASE_D* pBaseDataJ;
	pSectData = &pDataSrc->SectBefore;
	pBaseDataI = &pSectData->SectI;
	pBaseDataJ = &pSectData->SectJ;

	SetValueRange(CGXRange(2, 1), pDataSrc->SName); // Section Name
	SetValueRange(CGXRange(4, 1), pSectData->Shape); // Shape
	SetValueRange(CGXRange(6, 1), pBaseDataI->DBName); // DB

	double *pValue[14];
	// Section-I
	SetValueRange(CGXRange(8, 2), pBaseDataI->SName);
	for (int i = 0;i < nSizeCountI; i++)
		SetValueRange(CGXRange(9+i, 2), pBaseDataI->Size[i]);
	CSectUtil::MakeMapStiffVar(&pBaseDataI->Stiffness, pValue);
	pValue[12] = &pBaseDataI->PeriOut;
	pValue[13] = &pBaseDataI->PeriIn;
	for (int i = 0; i < 14; i++)
		SetValueRange(CGXRange(18+i, 2), *pValue[i]);

	// Section-J
	SetValueRange(CGXRange(19+D_STIFF_ROW_NUM, 2), pBaseDataJ->SName);
	for (int i = 0;i < nSizeCountJ; i++)
		SetValueRange(CGXRange(20+D_STIFF_ROW_NUM+i, 2), pBaseDataJ->Size[i]);
	CSectUtil::MakeMapStiffVar(&pBaseDataJ->Stiffness, pValue);
	pValue[12] = &pBaseDataJ->PeriOut;
	pValue[13] = &pBaseDataJ->PeriIn;
	for (int i = 0; i < D_STIFF_ROW_NUM; i++)
		SetValueRange(CGXRange(20+D_STIFF_ROW_NUM+8+1+i, 2), *pValue[i]);

	CString aVariation[] = {_LS(IDS_WG_CMD__ADDD__Linear_ITIZI), _LS(IDS_WG_CMD__ADDD__Parabolic), _LS(IDS_WG_CMD__ADDD__Cubic)};

	if (pSectData->nLytype >= 1 && pSectData->nLytype <= 3)
		SetValueRange(CGXRange(58, 2), aVariation[pSectData->nLytype-1]);
	if (pSectData->nLztype >= 1 && pSectData->nLztype <= 3)
		SetValueRange(CGXRange(59, 2), aVariation[pSectData->nLztype-1]);

	LockUpdate(FALSE);
}

#define CWnd CGXGridWnd
BEGIN_MESSAGE_MAP(CCMSecInputGrid, CWnd)
#undef CWnd
	//{{AFX_MSG_MAP(CCMSecInputGrid)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CCMSecInputGrid message handlers
