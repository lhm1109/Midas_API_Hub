// CMPhiSectDetailGrid_JP.cpp : implementation file
//

#include "stdafx.h" 

#include "wg_cmd.h"
#include "MPhiSectDetailGrid_JP.h"

#include "..\wg_common\wg_common_TBCommon.h"
#include "..\wg_common\wg_common_TBGrid.h"
#include "..\wg_db\SectUtil.h"
#include "..\wg_db\DBDoc.h"
#include "..\wg_db\AttrCtrl.h"
#include "..\wg_db\AttrCtrl2.h"
#include "..\wg_db\DB_QUERY_SEIS_CVL_JP.h"
#include "..\wg_db\EditData.h"
#include "..\wg_base\SpecialChar.h"
#include "..\wg_db\wg_db_UnitCtrl.h"

# define D_COL_INDENT_1 1
# define D_COL_INDENT_2 2
# define D_COL_CATEGORY 3
# define D_COL_CATEGORY_HALF 4
# define D_COL_UNIT 5
# define D_COL_Y_PLUS 6
# define D_COL_Z_PLUS 7
# define D_COL_NUM 7

#define D_ROW_RC_SECT			2
#define D_ROW_CONC				3
#define D_ROW_REIN_A			4
#define D_ROW_REIN_B			5
#define D_ROW_STEEL_SECT		6
#define D_ROW_STEEL_USER_A		7
#define D_ROW_STEEL_BUCK_L		8
#define D_ROW_STEEL_REINI_A		9
#define D_ROW_STEEL_PARAM_RF	10
#define D_ROW_STEEL_PARAM_RR	11
#define  D_ROW_STEEL_PARAM_C   12
#define D_ROW_NUM	12

CMPhiSectDetailGrid_JP::CMPhiSectDetailGrid_JP()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_SectData.RemoveAll();
	m_SectCurrentRow = 0;
	m_bChange = FALSE;
}

CMPhiSectDetailGrid_JP::~CMPhiSectDetailGrid_JP()
{
}

///////////////////////////////////////////////////////////////////////////////
// User Defined Function

BEGIN_MESSAGE_MAP(CMPhiSectDetailGrid_JP, CTBBrowserWnd)
	//{{AFX_MSG_MAP(CMPhiSectDetailGrid_JP)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMPhiSectDetailGrid_JP message handlers

void CMPhiSectDetailGrid_JP::Initialize(const CArray<T_MPHG_K, T_MPHG_K>& aMphgK, const CArray<T_MPHG_D, T_MPHG_D&>& aMphgD)
{
	m_aMphgK.Copy(aMphgK);
	m_aMphgD.Copy(aMphgD);
	m_MatlCode = CMPhiSectDetailGrid_JP::kMPHI_V24_DEFAULT;

	CTBBrowserWnd::Initialize();

	// cell 바깥영역 배경색 지정 
	GetParam()->GetProperties()->SetColor(GX_COLOR_BACKGROUND, CTBGrid::m_Color.OuterBackground);

	// Header 배경색 지정 
	ChangeColHeaderStyle(CGXStyle().SetInterior(CTBGrid::m_Color.RowColHeaderBg)); 
	ChangeRowHeaderStyle(CGXStyle().SetInterior(CTBGrid::m_Color.RowColHeaderBg)); 

	// 기타 Cells 배경색 지정
	ChangeStandardStyle(CGXStyle( ).SetInterior(CTBGrid::m_Color.NormalModeBg));

	LockUpdate(TRUE);

	SetColCount(D_COL_NUM);
	
	// Header
	SetFrozenCols(0, 0);

	// 표준 스타일 변경
	ChangeStandardStyle(CGXStyle()
		.SetFont(CGXFont_GC().SetSize(9))
		.SetVerticalAlignment(DT_VCENTER)
		.SetAllowEnter(FALSE));

	// 컬럼 헤더 스타일 변경
	ChangeColHeaderStyle(CGXStyle()
		.SetFont(CGXFont_GC().SetBold(FALSE)));

	SetCoveredCellsRowCol(0, 0, 1, 0);
	SetCoveredCellsRowCol(0, D_COL_INDENT_1, 1, D_COL_CATEGORY_HALF);
	SetCoveredCellsRowCol(0, D_COL_UNIT, 1, D_COL_UNIT);
	SetCoveredCellsRowCol(0, D_COL_Y_PLUS, 1, D_COL_Y_PLUS);
	SetCoveredCellsRowCol(0, D_COL_Z_PLUS, 1, D_COL_Z_PLUS);

		// 컬럼 스타일 변경
	SetStyleRange(CGXRange().SetCols(D_COL_INDENT_1, D_COL_UNIT), CGXStyle()
		.SetControl(GX_IDS_CTRL_STATIC)
		.SetEnabled(FALSE));

	SetStyleRange(CGXRange().SetCols(D_COL_Y_PLUS, D_COL_Z_PLUS), CGXStyle()
		.SetValueType(GX_VT_NUMERIC)
		.SetHorizontalAlignment(DT_CENTER));

	EnableDeleteRowByPressingKey(FALSE);
	EnableInsertRowByPressingKey(FALSE);
	EnableAppendRow(FALSE);

	SetRowHeightByDPI(0, 0, 30);

	// undo를 금지시킨다.
	GetParam()->EnableUndo(FALSE);  

	// cell의 width와 height 크기 변경을 금지한다.
	GetParam()->EnableTrackRowHeight(FALSE);
	//GetParam()->EnableTrackColWidth(FALSE);

	// Row와  Column의 이동을 금지시킨다.
	GetParam()->EnableMoveRows(FALSE);
	GetParam()->EnableMoveCols(FALSE);

	// double click시 셀 편집이 되도록 한다.
	GetParam()->SetActivateCellFlags(GX_CAFOCUS_DBLCLICKONCELL);

	// MemoryDC를 사용한다.
	SetDrawingTechnique(gxDrawUsingMemDC);

	// 헤더의 Current cell을 막는다.
	ColHeaderStyle().SetEnabled(FALSE);
	RowHeaderStyle().SetEnabled(FALSE);

	// always show vertical scrollbar
	//SetScrollBarMode(SB_VERT, gxnEnabled | gxnEnhanced);

	CGXProperties* pProp = GetParam()->GetProperties();
	pProp->SetMarkColHeader(FALSE); // Turn off pressed button effect for column headers
	pProp->SetMarkRowHeader(FALSE);
	pProp->SetUserProperty(GX_IDS_OUTLINECURRENTCELL, // 현재 셀의 border 설정 변경
		(CGXStyle) pProp->sInvertNoBorder);
	
	CArray<int, int> aWidth;
	aWidth.SetSize(D_COL_NUM); 
	aWidth[D_COL_INDENT_1 - 1] = 18;
	aWidth[D_COL_INDENT_2 - 1] = 18;
	aWidth[D_COL_CATEGORY - 1] = 110;
	aWidth[D_COL_CATEGORY_HALF - 1] = 18;
	aWidth[D_COL_UNIT - 1] = 33;
	aWidth[D_COL_Y_PLUS - 1] = 100;
	aWidth[D_COL_Z_PLUS - 1] = 100;

	SetColWidthByDPI(0, 0, 18);
	for (int i = 0; i < D_COL_NUM; i++)
	{
		SetColWidthByDPI(i + 1, i + 1, aWidth[i]);
	}
	SetValueRange(CGXRange(0, D_COL_INDENT_1), _LS(IDS_CMD_MPHI_SECT_JP_CATEGORY));
	SetValueRange(CGXRange(0, D_COL_UNIT), _LS(IDS_CMD_MPHI_SECT_JP_UNIT));
	SetValueRange(CGXRange(0, D_COL_Y_PLUS), _LS(IDS_CMD_MPHI_SECT_JP_Y));
	SetValueRange(CGXRange(1, D_COL_Y_PLUS), _LS(IDS_CMD_MPHI_SECT_JP_PlUS));
	SetValueRange(CGXRange(0, D_COL_Z_PLUS), _LS(IDS_CMD_MPHI_SECT_JP_Z));
	SetValueRange(CGXRange(1, D_COL_Z_PLUS), _LS(IDS_CMD_MPHI_SECT_JP_PlUS));
	
	setInitRow();

	LockUpdate(FALSE);
}

void CMPhiSectDetailGrid_JP::SetAllData(CArray<T_SECT_DATA_JP, T_SECT_DATA_JP&>& aData)
{
	m_SectData.RemoveAll();
	m_SectData.Copy(aData);
}

void CMPhiSectDetailGrid_JP::setInitRow()
{
	LockUpdate(TRUE);

	m_SectCurrentRow = 0;
	int nSize = D_ROW_NUM;
	SetRowCount(nSize+1);
	HideRows(nSize + 1, nSize + 1);

	// set category
	for (int i = D_ROW_RC_SECT; i < nSize+1; i++)
	{
		// set check box
		if (i == D_ROW_STEEL_USER_A)
		{
			SetStyleRange(CGXRange().SetCells(i, D_COL_INDENT_1), CGXStyle()
				.SetValue(_T("0"))
				.SetControl(GX_IDS_CTRL_CHECKBOX3D)
				.SetEnabled(TRUE)
				.SetValueType(GX_VT_NUMERIC)
				.SetHorizontalAlignment(DT_CENTER));
		}
		else {
			SetValueRange(CGXRange(i, D_COL_INDENT_1), _T("-"));
			SetValueRange(CGXRange(i, D_COL_INDENT_2), _T("-"));
		}
		// merge cell
		switch (i)
		{
		case D_ROW_RC_SECT:
		case D_ROW_CONC:
		case D_ROW_STEEL_SECT:
			SetCoveredCellsRowCol(i, D_COL_INDENT_1, i, D_COL_CATEGORY_HALF);
			break;
		case D_ROW_REIN_A:
			SetCoveredCellsRowCol(4, D_COL_INDENT_2, 5, D_COL_CATEGORY);
			break;
		case D_ROW_STEEL_USER_A:
		case D_ROW_STEEL_BUCK_L:
		case D_ROW_STEEL_REINI_A:
		case D_ROW_STEEL_PARAM_RF:
		case D_ROW_STEEL_PARAM_RR:
		case D_ROW_STEEL_PARAM_C:
			SetCoveredCellsRowCol(i, D_COL_INDENT_2, i, D_COL_CATEGORY_HALF);
			break;
		default:
			break;
		}
	}
	SetValueRange(CGXRange(D_ROW_RC_SECT, D_COL_INDENT_1), _LS(IDS_CMD_MPHI_SECT_JP_RC_SECT));
	SetValueRange(CGXRange(D_ROW_CONC, D_COL_INDENT_1), _LS(IDS_CMD_MPHI_SECT_JP_CONC));
	SetValueRange(CGXRange(D_ROW_REIN_A, D_COL_INDENT_2), _LS(IDS_CMD_MPHI_SECT_JP_SECT_REIN));
	SetValueRange(CGXRange(D_ROW_REIN_A, D_COL_CATEGORY_HALF), _LS(IDS_CMD_MPHI_SECT_JP_REIN_A));
	SetValueRange(CGXRange(D_ROW_REIN_B, D_COL_CATEGORY_HALF), _LS(IDS_CMD_MPHI_SECT_JP_REIN_B));

	SetValueRange(CGXRange(D_ROW_STEEL_SECT, D_COL_INDENT_1), _LS(IDS_CMD_MPHI_SECT_JP_STEEL_SECT));
	SetValueRange(CGXRange(D_ROW_STEEL_USER_A, D_COL_INDENT_2), _LS(IDS_CMD_MPHI_SECT_JP_STEEL_USER_A));
	SetValueRange(CGXRange(D_ROW_STEEL_BUCK_L, D_COL_INDENT_2), _LS(IDS_CMD_MPHI_SECT_JP_STEEL_BUCK_L));
	SetValueRange(CGXRange(D_ROW_STEEL_REINI_A, D_COL_INDENT_2), _LS(IDS_CMD_MPHI_SECT_JP_STEEL_REINI_A));
	SetValueRange(CGXRange(D_ROW_STEEL_PARAM_RF, D_COL_INDENT_2), _LS(IDS_CMD_MPHI_SECT_JP_STEEL_PARAM_RF));
	SetValueRange(CGXRange(D_ROW_STEEL_PARAM_RR, D_COL_INDENT_2), _LS(IDS_CMD_MPHI_SECT_JP_STEEL_PARAM_RR));
	SetValueRange(CGXRange(D_ROW_STEEL_PARAM_C, D_COL_INDENT_2), _LS(IDS_CMD_MPHI_SECT_JP_STEEL_PARAM_C));

	// set others
	int nColCount = GetColCount();
	for (int row = D_ROW_RC_SECT; row < nSize+1; row++)
	{
		setInitUnit(row);
		if (row == D_ROW_RC_SECT || row == D_ROW_CONC || row == D_ROW_STEEL_SECT || row == D_ROW_STEEL_PARAM_C)
			SetCoveredCellsRowCol(row, D_COL_Y_PLUS, row, D_COL_Z_PLUS);

		// style others
		for (int col = D_COL_Y_PLUS ; col <= nColCount; col++)
		{
			SetStyleRange(CGXRange().SetCells(row, col), CGXStyle()
				.SetValue(_T(""))
				.SetValueType(GX_VT_NUMERIC)
				.SetEnabled(FALSE)
				.SetHorizontalAlignment(DT_CENTER));
		}
	}

	SetRowHeightByDPI(D_ROW_RC_SECT, D_ROW_NUM, 28);
	LockUpdate(FALSE);
}

void CMPhiSectDetailGrid_JP::setInitUnit(const int& row)
{
	CString strUnit;
	m_pDoc->m_pUnitCtrl->GetUnitSystemLength(strUnit);
	if ( row == D_ROW_STEEL_BUCK_L || row == D_ROW_STEEL_REINI_A || row == D_ROW_STEEL_PARAM_C)
		SetStyleRange(CGXRange().SetCells(row, D_COL_UNIT ), CGXStyle()
			.SetValue(strUnit)
			.SetEnabled(FALSE)
			.SetHorizontalAlignment(DT_CENTER));
	else 
		SetStyleRange(CGXRange().SetCells(row, D_COL_UNIT ), CGXStyle()
			.SetValue(_T("-"))
			.SetEnabled(FALSE)
			.SetHorizontalAlignment(DT_CENTER));
}

void CMPhiSectDetailGrid_JP::InitCursor(ROWCOL nRow)
{
	LockUpdate(TRUE);

	if (m_SectData.GetSize() == 0) return;
	if (nRow <= 0) return;
	m_SectCurrentRow = nRow;
	T_SECT_DATA_JP SectData;
	for (int i = 0; i < m_SectData.GetSize(); i++)
	{
		if (i == nRow -1)
		{
			SectData = m_SectData[i];
			SetData(SectData);
			break;
		}
	}
	LockUpdate(FALSE);
}

void CMPhiSectDetailGrid_JP::SetData(const T_SECT_DATA_JP& rData)
{
	if (rData.bCandidate)
	{
		SetMatlYear(rData);

		if (rData.bRC)
			SetRC(rData);
		else
		{
			for (int row = D_ROW_REIN_A; row <= D_ROW_REIN_B; row++)
			{
				for (int col = D_COL_Y_PLUS; col <= D_COL_Z_PLUS; col++)
					SetStyleRange(CGXRange().SetCells(row, col), CGXStyle().SetEnabled(FALSE));
			}
		}

		if (rData.bStl || rData.bSRC)
			SetSteel(rData);
		else
		{
			for (int row = D_ROW_STEEL_USER_A; row <= D_ROW_NUM; row++)
			{
				for (int col = D_COL_INDENT_1; col <= D_COL_Z_PLUS; col++)
					SetStyleRange(CGXRange().SetCells(row, col), CGXStyle()
						.SetEnabled(FALSE));
			}
		}

		if (m_MatlCode != CMPhiSectDetailGrid_JP::kMPHI_V24_DEFAULT)
			EnableMatlYear(rData.bStl);
	}
}

void CMPhiSectDetailGrid_JP::SetRC(const T_SECT_DATA_JP& rData)
{
	CString strData;
	for (int row = D_ROW_REIN_A; row <= D_ROW_REIN_B; row++)
	{
		for(int col = D_COL_Y_PLUS; col <= D_COL_Z_PLUS; col ++)
			SetStyleRange(CGXRange().SetCells(row, col), CGXStyle().SetEnabled(TRUE));
	}
	BOOL bCircle = GetCheckCircleSolidPipe(rData);
	BOOL bTrack = GetCheckTrackSolidTrack(rData);
	SetValueRange(CGXRange(D_ROW_REIN_A, D_COL_Y_PLUS), rData.MpstD.dAlpha);
	double dAlphaZ = rData.MpstD.dAlphaZ == 0 ? 0.2 : rData.MpstD.dAlphaZ;
	if (bCircle || bTrack) dAlphaZ = rData.MpstD.dAlphaZ == 0 ? 1 : rData.MpstD.dAlphaZ;
	SetValueRange(CGXRange(D_ROW_REIN_A, D_COL_Z_PLUS), dAlphaZ);

	SetValueRange(CGXRange(D_ROW_REIN_B, D_COL_Y_PLUS), rData.MpstD.dBeta);
	double dBetaZ = rData.MpstD.dBetaZ == 0 ? 0.4 : rData.MpstD.dBetaZ;
	if (bCircle || bTrack) dBetaZ = rData.MpstD.dBetaZ == 0 ? 1 : rData.MpstD.dBetaZ;
	SetValueRange(CGXRange(D_ROW_REIN_B, D_COL_Z_PLUS), dBetaZ);

}

void CMPhiSectDetailGrid_JP::SetSteel(const T_SECT_DATA_JP& rData)
{
	BOOL bUser = rData.MpstD.Steel.bEpsilon_a;
	for (int row = D_ROW_STEEL_USER_A; row <=  D_ROW_NUM; row++)
	{
		if (row == D_ROW_STEEL_PARAM_RF || row == D_ROW_STEEL_PARAM_RR) continue;
		if (row == D_ROW_STEEL_USER_A)
		{
			for (int col = D_COL_Y_PLUS; col <= D_COL_Z_PLUS; col++)
				SetStyleRange(CGXRange().SetCells(row, col), CGXStyle()
					.SetEnabled(bUser));
		}
		else
		{
			for (int col = D_COL_Y_PLUS; col <= D_COL_Z_PLUS; col++)
				SetStyleRange(CGXRange().SetCells(row, col), CGXStyle()
					.SetEnabled(!bUser));
		}
	}

	T_MPST_STEEL_ITEM localY, localZ;
	localY= rData.MpstD.Steel.LocalY;
	localZ = rData.MpstD.Steel.LocalZ;

	BOOL bCircle = GetCheckCircleSolidPipe(rData);
	if (bUser)
	{
		SetValueRange(CGXRange(D_ROW_STEEL_USER_A, D_COL_INDENT_1), "1");
		SetValueRange(CGXRange(D_ROW_STEEL_USER_A, D_COL_Y_PLUS), localY.dEpsilon_a);
		SetValueRange(CGXRange(D_ROW_STEEL_USER_A, D_COL_Z_PLUS), localZ.dEpsilon_a);
	}
	else
	{
		SetValueRange(CGXRange(D_ROW_STEEL_USER_A, D_COL_INDENT_1), "0");
		SetValueRange(CGXRange(D_ROW_STEEL_BUCK_L, D_COL_Y_PLUS), localY.dL);
		SetValueRange(CGXRange(D_ROW_STEEL_BUCK_L, D_COL_Z_PLUS), localZ.dL);
		SetValueRange(CGXRange(D_ROW_STEEL_REINI_A, D_COL_Y_PLUS), localY.da);
		SetValueRange(CGXRange(D_ROW_STEEL_REINI_A, D_COL_Z_PLUS), localZ.da);
	}

	BOOL bBoxStiff = rData.strShape == D_SECT_SHAPE_REG_BSTF ? TRUE : FALSE;
	if (m_MatlCode == CMPhiSectDetailGrid_JP::kMPHI_V || m_MatlCode == CMPhiSectDetailGrid_JP::kMPHI_V24_DEFAULT)
	{
		double dRFY = std::floor(rData.dRFY * 1000) / 1000;
		double dRFZ = std::floor(rData.dRFZ * 1000) / 1000;
		SetValueRange(CGXRange(D_ROW_STEEL_PARAM_RF, D_COL_Y_PLUS), dRFY);
		SetValueRange(CGXRange(D_ROW_STEEL_PARAM_RF, D_COL_Z_PLUS), bCircle ? dRFY : dRFZ); // 원형일 때 Y 적용
		double dRRY = std::floor(rData.dRRY * 1000) / 1000;
		double dRRZ = std::floor(rData.dRRZ * 1000) / 1000;
		SetValueRange(CGXRange(D_ROW_STEEL_PARAM_RR, D_COL_Y_PLUS), dRRY);
		SetValueRange(CGXRange(D_ROW_STEEL_PARAM_RR, D_COL_Z_PLUS), bCircle ? dRRY : dRRZ); // 원형일 때 Y 적용
	}

	// 부식대
	if (bCircle && m_MatlCode == CMPhiSectDetailGrid_JP::kMPHI_IV_14_24_PIPE)
	{
		for (int col = D_COL_Y_PLUS; col <= D_COL_Z_PLUS; col++)
			SetStyleRange(CGXRange().SetCells(D_ROW_STEEL_PARAM_C, col), CGXStyle()
				.SetEnabled(TRUE));
		SetValueRange(CGXRange(D_ROW_STEEL_PARAM_C, D_COL_Y_PLUS), rData.MpstD.dc);
	}
	else
	{
		for (int col = D_COL_Y_PLUS; col <= D_COL_Z_PLUS; col++)
			SetStyleRange(CGXRange().SetCells(D_ROW_STEEL_PARAM_C, col), CGXStyle()
				.SetEnabled(FALSE));
	}
}

void CMPhiSectDetailGrid_JP::EnableMatlYear(const  BOOL& bStl)
{
	// RC
	if (m_MatlCode != CMPhiSectDetailGrid_JP::kMPHI_V)
	{
		for (int row = D_ROW_REIN_A; row <= D_ROW_REIN_B; row++)
		{
			for (int col = D_COL_Y_PLUS; col <= D_COL_Z_PLUS; col++)
				SetStyleRange(CGXRange().SetCells(row, col), CGXStyle().SetEnabled(FALSE));
		}
	}

	// Steel, SRC
	if (m_MatlCode == CMPhiSectDetailGrid_JP::kMPHI_V  ||
		m_MatlCode == CMPhiSectDetailGrid_JP::kMPHI_CONC_CODE_BOOK ||
		m_MatlCode == CMPhiSectDetailGrid_JP::kMPHI_III_SIG_CK ||
		m_MatlCode == CMPhiSectDetailGrid_JP::kMPHI_III_85_SIG_CK ||
		m_MatlCode == CMPhiSectDetailGrid_JP::kMPHI_III_SIG_CK_H29)
	{
		if (bStl && m_MatlCode == CMPhiSectDetailGrid_JP::kMPHI_V)
		{
			// 부식대만 비활성
			for (int col = D_COL_Y_PLUS; col <= D_COL_Z_PLUS; col++)
				SetStyleRange(CGXRange().SetCells(D_ROW_STEEL_PARAM_C, col), CGXStyle()
					.SetEnabled(FALSE));
		}
		else
		{
			// 다 비활성
			for (int row = D_ROW_STEEL_USER_A; row <= D_ROW_NUM; row++)
			{
				for (int col = D_COL_INDENT_1; col <= D_COL_Z_PLUS; col++)
					SetStyleRange(CGXRange().SetCells(row, col), CGXStyle()
						.SetEnabled(FALSE));
			}
		}
	}
	else if (m_MatlCode == CMPhiSectDetailGrid_JP::kMPHI_IV_14_24_PIPE)
	{
		// 부식대 이외 비활성
		for (int row = D_ROW_STEEL_USER_A; row <= D_ROW_STEEL_PARAM_RR; row++)
		{
			for (int col = D_COL_INDENT_1; col <= D_COL_Z_PLUS; col++)
				SetStyleRange(CGXRange().SetCells(row, col), CGXStyle()
					.SetEnabled(FALSE));
		}
	}
}

BOOL CMPhiSectDetailGrid_JP::GetData(CArray<T_SECT_DATA_JP, T_SECT_DATA_JP&>& aData)
{
	if (aData.GetSize() == 0) return TRUE;

	for (int i = 0; i < aData.GetSize(); i++)
	{
		if (aData[i].SectK != m_SectData[i].SectK) continue;

		// Candidate
		if (! aData[i].bCandidate) continue;

		SetMatlYear(aData[i]);

		// rc
		if (aData[i].bRC)
			GetRc(m_SectData[i], aData[i]);

		// steel
		if (aData[i].bStl || aData[i].bSRC)
			GetSteel(m_SectData[i], aData[i]);
	}
	return TRUE;
}

void CMPhiSectDetailGrid_JP::GetRc(const T_SECT_DATA_JP& newData, T_SECT_DATA_JP& rData)
{
	rData.MpstD.dAlpha = newData.MpstD.dAlpha;
	rData.MpstD.dBeta = newData.MpstD.dBeta;
	rData.MpstD.dDim = newData.MpstD.dDim;
	rData.MpstD.dDimz = newData.MpstD.dDimz;

	BOOL bCircle = GetCheckCircleSolidPipe(rData);
	BOOL bTrack = GetCheckTrackSolidTrack(rData);
	double dAlphaZ = newData.MpstD.dAlphaZ == 0 ? 0.2 : newData.MpstD.dAlphaZ;
	if (bCircle || bTrack) dAlphaZ = newData.MpstD.dAlphaZ == 0 ? 1 : newData.MpstD.dAlphaZ;
	rData.MpstD.dAlphaZ = dAlphaZ;

	double dBetaZ = newData.MpstD.dBetaZ == 0 ? 0.4 : newData.MpstD.dBetaZ;
	if (bCircle || bTrack) dBetaZ = newData.MpstD.dBetaZ == 0 ? 1 : newData.MpstD.dBetaZ;
	rData.MpstD.dBetaZ = dBetaZ;
}

void CMPhiSectDetailGrid_JP::GetSteel(const T_SECT_DATA_JP& newData, T_SECT_DATA_JP& rData)
{
	rData.MpstD.Steel.Initialize();
	rData.MpstD.Steel.bEpsilon_a = newData.MpstD.Steel.bEpsilon_a;
	if (rData.MpstD.Steel.bEpsilon_a)
	{
		rData.MpstD.Steel.LocalY.dEpsilon_a = newData.MpstD.Steel.LocalY.dEpsilon_a;
		rData.MpstD.Steel.LocalZ.dEpsilon_a = newData.MpstD.Steel.LocalZ.dEpsilon_a;
	}
	else
	{
		rData.MpstD.Steel.LocalY.dL = newData.MpstD.Steel.LocalY.dL;
		rData.MpstD.Steel.LocalZ.dL = newData.MpstD.Steel.LocalZ.dL;
		rData.MpstD.Steel.LocalY.da = newData.MpstD.Steel.LocalY.da;
		rData.MpstD.Steel.LocalZ.da = newData.MpstD.Steel.LocalZ.da;
		rData.MpstD.Steel.LocalY.dRF = _ttof(GetValueRowCol(D_ROW_STEEL_PARAM_RF, D_COL_Y_PLUS));
		rData.MpstD.Steel.LocalZ.dRF = _ttof(GetValueRowCol(D_ROW_STEEL_PARAM_RF, D_COL_Z_PLUS));
		rData.MpstD.Steel.LocalY.dRR = _ttof(GetValueRowCol(D_ROW_STEEL_PARAM_RR, D_COL_Y_PLUS));
		rData.MpstD.Steel.LocalZ.dRR = _ttof(GetValueRowCol(D_ROW_STEEL_PARAM_RR, D_COL_Z_PLUS));
	}
	// 부식대
	rData.MpstD.dc = newData.MpstD.dc;
}

BOOL CMPhiSectDetailGrid_JP::GetCheckCircleSolidPipe(const T_SECT_DATA_JP& rData)
{
	CString strShape = rData.strShape;

	if (strShape == D_SECT_SHAPE_REG_P || strShape == D_SECT_SHAPE_REG_SR ||
		strShape == D_SECT_SHAPE_REG_PSTF || strShape == D_SECT_SHAPE_SRC_PSTF || 
		strShape == D_SECT_SHAPE_SRC_EPC)
		return TRUE;
	else
		return FALSE;
}

BOOL CMPhiSectDetailGrid_JP::GetCheckTrackSolidTrack(const T_SECT_DATA_JP& rData)
{
	CString strShape = rData.strShape;

	if (strShape == D_SECT_SHAPE_REG_TRK || strShape == D_SECT_SHAPE_REG_STRK)
		return TRUE;
	else
		return FALSE;
}

BOOL CMPhiSectDetailGrid_JP::GetStyleRowCol(ROWCOL nRow, ROWCOL nCol, CGXStyle& style, GXModifyType mt, int nType)
{
	BOOL bReadOnlyMode;
	BOOL bCurrentRow;
	BOOL bCurrentCell;
	BOOL bEnabled;
	BOOL bReadOnly;
	BOOL bAppendRow;
	BOOL bHeaderCol;

	ROWCOL ncRow, ncCol;    // 현재 Row, Col
	CGXGridWnd::GetStyleRowCol(nRow, nCol, style, mt, nType);

	// 현재 셀의 그리드 좌표(Row, Col 번호)를 얻어온다.
	if (GetCurrentCell(ncRow, ncCol)) GetBrowseParam()->m_nCurrentRow = ncRow;
	else ncRow = GetRowCount() + 1; // 현재 셀이 없는 경우 : ncRow를 범위 밖으로 설정

	// 현재 셀이 이동되면 새로 그려준다.
	m_bRefreshOnSetCurrentCell = TRUE;

	// Row나 Col이 1보다 작으면 리턴한다.(Standard Row or Column Header)
	if (nRow < 1 || nCol < 1) return TRUE;

	// Get Current Table and Cell State
	bReadOnlyMode = IsLockEdit();
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
	else // Column style에서 enable 정보 알아냄
	{
		CGXStyle styleEx;
		GetColStyle(nCol, styleEx);
		bEnabled = (!styleEx.GetIncludeEnabled() || styleEx.GetEnabled());
	}

	if (style.GetIncludeReadOnly()) // 셀에 readonly정보 포함 여부 알아냄
	{
		bReadOnly = style.GetReadOnly();
	}
	else // Column style에서 readonly 정보 알아냄
	{
		CGXStyle styleEx;
		GetColStyle(nCol, styleEx);
		bReadOnly = (styleEx.GetIncludeReadOnly() && styleEx.GetReadOnly());
	}

	bAppendRow = IsAppendRow(nRow);
	bHeaderCol = IsFrozenCol(nCol);

	// Printing 모드이면 헤더 Column이 프린트 되기 위해 다음과 같이 리턴한다.
	if (IsPrinting())
	{
		if (bHeaderCol) style.SetControl(GX_IDS_CTRL_HEADER);
		return TRUE;
	}

	// 일반 셀이 가장 많으므로 빨리 검사하고 바로 리턴할 수 있게 코드를 앞으로 뺀다.
	if (bEnabled && !bReadOnly && !bCurrentCell && !bCurrentRow && !bAppendRow && !bHeaderCol)
	{
		if (bReadOnlyMode) style
			.SetInterior(m_Color.ReadOnlyModeBg)
			.SetTextColor(m_Color.ReadOnlyModeFg);
		else style
			.SetInterior(m_Color.NormalModeBg)
			.SetTextColor(m_Color.NormalModeFg);

		return TRUE;
	}

	// Disable되었을 때
	if (!bEnabled)
	{
		if (bHeaderCol) style           // Row or Column Header
			.SetControl(GX_IDS_CTRL_HEADER)
			.SetInterior(m_Color.RowColHeaderBg)
			.SetTextColor(m_Color.NormalModeFg);
		else if (bCurrentRow) style     // Disabled Current Row
			.SetInterior(m_Color.DisabledBg)
			.SetTextColor(m_Color.ReadOnlyCurrentRowFg);
		else style                      // Disabled Cell
			.SetInterior(m_Color.DisabledBg)
			.SetTextColor(m_Color.ReadOnlyModeFg);

		if (!bHeaderCol && bCurrentRow) SetCurrentRowStyle(style);

		return TRUE;
	}

	//// Disable안 되고 ReadOnly만 설정되었을 때
	//if (bReadOnly)
	//{
	//	if (bCurrentCell) style         // Current Cell
	//		.SetInterior(m_Color.ReadOnlyCurrentCellBg)
	//		.SetTextColor(m_Color.ReadOnlyCurrentCellFg);
	//	else if (bCurrentRow) style     // Current Row
	//		.SetInterior(m_Color.ReadOnlyCurrentRowBg)
	//		.SetTextColor(m_Color.ReadOnlyCurrentRowFg);
	//	else style                      // Readonly Cell
	//		.SetInterior(m_Color.ReadOnlyBg)
	//		.SetTextColor(m_Color.ReadOnlyFg);

	//	if (bCurrentRow) SetCurrentRowStyle(style);

	//	return TRUE;
	//}

	// Append Row가 Disable되었을 때(EnableAppendRow(FALSE)로 금지된 경우)
	if (bAppendRow && !m_bEnabledAppend)  // Disabled Apppend Row
	{
		if (bCurrentCell) style
			.SetInterior(m_Color.ReadOnlyCurrentCellBg)
			.SetTextColor(m_Color.ReadOnlyCurrentCellFg);
		else if (bCurrentRow) style
			.SetInterior(m_Color.DisabledAppendRowBg)
			.SetTextColor(m_Color.ReadOnlyCurrentRowFg);
		else style
			.SetInterior(m_Color.DisabledAppendRowBg)
			.SetTextColor(m_Color.ReadOnlyModeFg);

		//if (m_setRedFont.find(std::pair<ROWCOL, ROWCOL>(nRow, nCol)) != m_setRedFont.end())
		//{
		//	style.SetTextColor(COLORREF(RGB(255, 0, 0)));
		//}

		style.SetControl(GX_IDS_CTRL_STATIC);  // 수정이 안되도록 control을 static으로 바꾼다.

		if (bCurrentRow) SetCurrentRowStyle(style);

		return TRUE;
	}

	// Disable 안되고, ReadOnly 아니고, Append가 Disable 되지 않은 경우
	// Current Cell or Current Row or Enabled Append Row인 경우이다.
	//if (bReadOnlyMode)
	//{
	//	if (bCurrentCell) style
	//		.SetInterior(m_Color.ReadOnlyCurrentCellBg)
	//		.SetTextColor(m_Color.ReadOnlyCurrentCellFg);
	//	else if (bCurrentRow) style
	//		.SetInterior(m_Color.ReadOnlyCurrentRowBg)
	//		.SetTextColor(m_Color.ReadOnlyCurrentRowFg);
	//	else if (bAppendRow) style
	//		.SetInterior(m_Color.ReadOnlyModeBg)
	//		.SetTextColor(m_Color.ReadOnlyModeFg);
	//	else if (bHeaderCol) style    // Enabled Header Col(Only Frozen)
	//		.SetInterior(m_Color.ReadOnlyModeBg)
	//		.SetTextColor(m_Color.ReadOnlyModeFg);

	//	if (bCurrentRow || bHeaderCol) SetCurrentRowStyle(style);

	//	return TRUE;
	//}
	//else
	//{
		if (bCurrentCell) style
			.SetInterior(m_Color.NormalCurrentCellBg)
			.SetTextColor(m_Color.NormalCurrentCellFg);
		else if (bCurrentRow) style
			.SetInterior(m_Color.NormalCurrentRowBg)
			.SetTextColor(m_Color.NormalCurrentRowFg);
		else if (bAppendRow) style
			.SetInterior(m_Color.NormalModeBg)
			.SetTextColor(m_Color.NormalModeFg);
		else if (bHeaderCol) style    // Enabled Header Col(Only Frozen)
			.SetInterior(m_Color.NormalModeBg)
			.SetTextColor(m_Color.NormalModeFg);

		if (bCurrentRow || bHeaderCol) SetCurrentRowStyle(style);

		return TRUE;
	//}

	return TRUE;
}

BOOL CMPhiSectDetailGrid_JP::ProcessKeys(CWnd* pSender, UINT nMessage, UINT nChar, UINT nRepCnt, UINT flags)
{
	CRowColArray awRows;
	ROWCOL nRow = 1, nCol = 1;

	if (!GetCurrentCell(nRow, nCol)) // if no current cell, default action
		return CTBBrowserWnd::ProcessKeys(pSender, nMessage, nChar, nRepCnt, flags);

	//return CTBBrowserWnd::ProcessKeys(pSender, nMessage, nChar, nRepCnt, flags);

	if (nMessage == WM_KEYDOWN)
	{
		int i = nRow, j = nCol, nextStep = 1;

		if (nRow == D_ROW_REIN_A || nRow == D_ROW_REIN_B ||
			nRow == D_ROW_STEEL_USER_A || nRow == D_ROW_STEEL_BUCK_L ||
			nRow == D_ROW_STEEL_REINI_A || nRow == D_ROW_STEEL_PARAM_RF ||
			nRow == D_ROW_STEEL_PARAM_RR
			)
		{
			nextStep = 1;
		}
		else if (nRow == D_ROW_STEEL_PARAM_C)
		{
			nextStep = 2;
		}

		if (nChar == VK_LEFT)
		{
			j -= nextStep; // 왼쪽으로 이동
			if (j < 1) { i--; j = GetColCount(); }
			while (i >= 1)
			{
				while (j >= 1)
				{
					// 열이 선택될 수 있는지 조사
					if (CanSelectCurrentCell(FALSE, i, j, nRow, nCol))
					{
						if (CanSelectCurrentCell(TRUE, i, j, nRow, nCol))
						{
							SetCurrentCell(i, j); return TRUE;
						}
					}
					j--;  // 이전 열을 조사
				}
				i--;  // 이전 줄로 이동
				j = GetColCount();
			}
		}
		else if (nChar == VK_RIGHT)
		{
			j += nextStep; // 오른쪽으로 이동
			if (j > GetColCount()) { i++; j = 1; }

			while (i <= GetRowCount())
			{
				while (j <= GetColCount())
				{
					if (CanSelectCurrentCell(FALSE, i, j, nRow, nCol))
					{
						if (CanSelectCurrentCell(TRUE, i, j, nRow, nCol))
						{
							SetCurrentCell(i, j); return TRUE;
						}
					}
					j++;  // 다음 열을 조사
				}
				i++;  // 다음 줄로 이동
				j = 1;
			}
		}
		else return CTBBrowserWnd::ProcessKeys(pSender, nMessage, nChar, nRepCnt, flags);
	}
	else return CTBBrowserWnd::ProcessKeys(pSender, nMessage, nChar, nRepCnt, flags);
	
	return TRUE;
}

BOOL CMPhiSectDetailGrid_JP::OnStartEditing(ROWCOL nRow, ROWCOL nCol)
{
	if (m_SectCurrentRow == 0) return TRUE;

	m_bChange = TRUE;
	if (nCol == D_COL_INDENT_1 || nCol == D_COL_INDENT_2)
	{
		T_SECT_DATA_JP SectData;
		SectData = m_SectData[m_SectCurrentRow - 1];
		EditCheckBox(nRow, nCol, SectData);
		m_SectData[m_SectCurrentRow - 1] = SectData;
	}
	return CTBBrowserWnd::OnStartEditing(nRow, nCol);
}

BOOL CMPhiSectDetailGrid_JP::OnEndEditing(ROWCOL nRow, ROWCOL nCol)
{
	if (!m_bChange)  return TRUE;
	if (m_SectCurrentRow == 0) return TRUE;

	CString data = GetValueRowCol(nRow, nCol);
	double dValue = _ttof(data);
	// double
	if (dValue == 0)
	{
		SetValueRange(CGXRange(nRow, nCol), _T(""));
	}

	if (nCol != D_COL_INDENT_1 && nCol != D_COL_INDENT_2)
	{
		T_SECT_DATA_JP SectData;
		SectData = m_SectData[m_SectCurrentRow - 1];
		EditData(nRow, nCol, SectData);
		m_SectData[m_SectCurrentRow - 1] = SectData;
	}

	m_bChange = FALSE;

	return CTBBrowserWnd::OnEndEditing(nRow, nCol);
}

void CMPhiSectDetailGrid_JP::EditCheckBox(ROWCOL nRow, ROWCOL nCol, T_SECT_DATA_JP& rData)
{
	CString strCheckBox;
	strCheckBox = GetValueRowCol(nRow, nCol);
	BOOL bCheck = strCheckBox == "1" ? FALSE : TRUE; // 이전 값이 들어옴...

	switch (nRow)
	{
	case D_ROW_STEEL_USER_A:
		if (!rData.bStl && !rData.bSRC) break;

		rData.MpstD.Steel.bEpsilon_a= bCheck;
		if (!bCheck)
		{
			rData.MpstD.Steel.LocalY.Initialize();
			rData.MpstD.Steel.LocalZ.Initialize();
			for (int col = D_COL_Y_PLUS; col <= D_COL_Z_PLUS; col++)
				SetValueRange(CGXRange(D_ROW_STEEL_USER_A, col), _T(""));
		}

		for (int col = D_COL_Y_PLUS; col <= D_COL_Z_PLUS; col++)
			SetStyleRange(CGXRange().SetCells(D_ROW_STEEL_USER_A, col), CGXStyle()
				.SetEnabled(bCheck));

		for (int row = D_ROW_STEEL_BUCK_L; row <= D_ROW_STEEL_PARAM_RR; row++)
		{
			if (row == D_ROW_STEEL_PARAM_RF || row == D_ROW_STEEL_PARAM_RR) continue;
			for (int col = D_COL_Y_PLUS; col <= D_COL_Z_PLUS; col++)
			{
				SetStyleRange(CGXRange().SetCells(row, col), CGXStyle()
					.SetEnabled(bCheck ? FALSE : TRUE));
			}
		}
		// 부식대
		if (GetCheckCircleSolidPipe(rData) && m_MatlCode == CMPhiSectDetailGrid_JP::kMPHI_IV_14_24_PIPE)
		{
			for (int col = D_COL_Y_PLUS; col <= D_COL_Z_PLUS; col++)
			{
				SetStyleRange(CGXRange().SetCells(D_ROW_STEEL_PARAM_C, col), CGXStyle()
					.SetEnabled(bCheck ? FALSE : TRUE));
				if (bCheck)
					SetValueRange(CGXRange(D_ROW_STEEL_PARAM_C, col), _T(""));
			}
		}
		else
		{
			for (int col = D_COL_Y_PLUS; col <= D_COL_Z_PLUS; col++)
			{
				SetStyleRange(CGXRange().SetCells(D_ROW_STEEL_PARAM_C, col), CGXStyle()
					.SetEnabled(FALSE));
				SetValueRange(CGXRange(D_ROW_STEEL_PARAM_C, col), _T(""));
			}
		}

		break;
	default:
		break;
	}
}

void CMPhiSectDetailGrid_JP::EditData(ROWCOL nRow, ROWCOL nCol, T_SECT_DATA_JP& rData)
{
	CString strValue;
	strValue = GetValueRowCol(nRow, nCol);
	double ws = _ttof(strValue);

	switch (nRow)
	{
	case D_ROW_REIN_A :
		if(nCol == D_COL_Y_PLUS) rData.MpstD.dAlpha = ws;
		if(nCol == D_COL_Z_PLUS) rData.MpstD.dAlphaZ = ws;
		break;
	case D_ROW_REIN_B	:
		if (nCol == D_COL_Y_PLUS) rData.MpstD.dBeta = ws;
		if (nCol == D_COL_Z_PLUS) rData.MpstD.dBetaZ = ws;
		break;
	case D_ROW_STEEL_USER_A	:
		if (nCol == D_COL_Y_PLUS) rData.MpstD.Steel.LocalY.dEpsilon_a = ws;
		if (nCol == D_COL_Z_PLUS) rData.MpstD.Steel.LocalZ.dEpsilon_a = ws;
		break;
	case D_ROW_STEEL_BUCK_L	:
		if (nCol == D_COL_Y_PLUS) rData.MpstD.Steel.LocalY.dL = ws;
		if (nCol == D_COL_Z_PLUS) rData.MpstD.Steel.LocalZ.dL = ws;
		break;
	case D_ROW_STEEL_REINI_A	:
		if (nCol == D_COL_Y_PLUS) rData.MpstD.Steel.LocalY.da = ws;
		if (nCol == D_COL_Z_PLUS) rData.MpstD.Steel.LocalZ.da = ws;
		break;
	case D_ROW_STEEL_PARAM_RF:
		if (nCol == D_COL_Y_PLUS) rData.MpstD.Steel.LocalY.dRF = ws;
		if (nCol == D_COL_Z_PLUS) rData.MpstD.Steel.LocalZ.dRF = ws;
		break;
	case D_ROW_STEEL_PARAM_RR:
		if (nCol == D_COL_Y_PLUS) rData.MpstD.Steel.LocalY.dRR = ws;
		if (nCol == D_COL_Z_PLUS) rData.MpstD.Steel.LocalZ.dRR = ws;
		break;
	case D_ROW_STEEL_PARAM_C:
		rData.MpstD.dc = ws;
	default:
		break;
	}
}

void CMPhiSectDetailGrid_JP::OnModifyCell(ROWCOL nRow, ROWCOL nCol)
{
	CTBBrowserWnd::OnModifyCell(nRow, nCol);
}

void CMPhiSectDetailGrid_JP::SetMatlYear(const T_SECT_DATA_JP& rData)
{
	CArray<T_ELEM_K, T_ELEM_K> aElemKeyList;
	m_pDoc->m_pAttrCtrl->GetElemKeyList(aElemKeyList);

	BOOL bFind = FALSE;
	for (int j = 0; j < m_aMphgD.GetSize(); j++)
	{
		T_MPHG_K MphgK = m_aMphgK.GetAt(j);
		T_MPHG_D MphgD = m_aMphgD.GetAt(j);
		for (int i = 0; i < aElemKeyList.GetSize(); i++)
		{
			T_ELEM_D ElemD; ElemD.Initialize();
			m_pDoc->m_pAttrCtrl->GetElem(aElemKeyList.GetAt(i), ElemD);
			if (MphgK != ElemD.elmat)  continue;
			if (rData.SectK != ElemD.elpro) continue;

			m_MatlCode = MphgD.nCode;
			bFind = TRUE;
			break;
		}
		if (bFind) break;
	}
}