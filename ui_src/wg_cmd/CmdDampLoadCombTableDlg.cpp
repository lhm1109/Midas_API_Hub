#include "stdafx.h"
#include "wg_cmd.h"
#include "CmdDampLoadCombTableDlg.h"
//#include "CmdIsolatorGBStressLmtTableDlg.h"
#include "..\wg_base\wg_base_CompFunc.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_ViewBuff.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_DBLib.h"
#include "..\wg_db\DampDgnMgr_CH.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/************************************************************************/
/*						CCmdDampLoadCombTableDlg						*/
/************************************************************************/

CCmdDampLoadCombTableDlg::CCmdDampLoadCombTableDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CCmdDampLoadCombTableDlg::IDD, pParent), m_wndGrid()
{
	m_DpciD.Initialize();
}

CCmdDampLoadCombTableDlg:: ~CCmdDampLoadCombTableDlg()
{

}

void CCmdDampLoadCombTableDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCmdDampLoadCombTableDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CCmdDampLoadCombTableDlg, CDialogMove)
	//{{AFX_MSG_MAP(CCmdDampLoadCombTableDlg)
	ON_BN_CLICKED(IDC_CMD_BTN_OK, OnCmdBtnOK)
	ON_BN_CLICKED(IDC_CMD_BTN_CANCEL, OnCmdBtnCancel)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////

BOOL CCmdDampLoadCombTableDlg::OnInitDialog()
{
	CDialogMove::OnInitDialog();

	// TODO: Add extra initialization here
	m_pDoc = CDBDoc::GetDocPoint();

	m_wndGrid.SubclassDlgItem(IDC_CMD_DAMP_LOADCOMB_TABLE_GRID, this);
	m_wndGrid.m_pDpciD = &m_DpciD;
	m_wndGrid.Initialize();
	//m_pData = m_wndGrid.m_pDpciD;
	// always show vertical scrollbar
	m_wndGrid.SetScrollBarMode(SB_VERT, gxnEnabled | gxnEnhanced);
	m_wndGrid.MakeItemEx();

	return TRUE;
}

void CCmdDampLoadCombTableDlg::OnCmdBtnOK()
{
	//m_wndGrid.Grid2Data();
	//m_pData = m_wndGrid.m_pDpciD;
	CDialogMove::OnOK();
}

void CCmdDampLoadCombTableDlg::OnCmdBtnCancel()
{
	//T_ISGB_D data;	data.Initialize();
	//m_wndGrid.Grid2Data(data);
	CDialogMove::OnCancel();
}

BOOL CCmdDampLoadCombTableDlg::SetParamData(const T_DPCI_D& ParamData)
{
	m_DpciD = ParamData;
	return TRUE;
}

BOOL CCmdDampLoadCombTableDlg::GetParamData(T_DPCI_D& ParamData)
{
	ParamData = m_DpciD;
	return TRUE;
}




CCmdDampLoadCombGridWnd::CCmdDampLoadCombGridWnd()
{
	m_aPrimaryKey.Add(0);
}

CCmdDampLoadCombGridWnd::~CCmdDampLoadCombGridWnd()
{
}


BEGIN_MESSAGE_MAP(CCmdDampLoadCombGridWnd, CGXGridWnd)
	//{{AFX_MSG_MAP(CCmdDampLoadCombGridWnd)
	ON_WM_RBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/************************************************************************/
/*						CCmdDampLoadCombGridWnd						*/
/************************************************************************/

void CCmdDampLoadCombGridWnd::Initialize()
{
	m_pDoc = CDBDoc::GetDocPoint();
	CTBBrowserWndEx::Initialize();

	CDampDesignMgr_CH::Instance()->MakeDampLoadComb(*m_pDpciD,&m_LcomD);

	GetParam()->EnableUndo(FALSE);
	GetParam()->EnableTrackRowHeight(FALSE);
	GetParam()->EnableMoveCols(FALSE);        // columns can not be dragged

	CGXProperties* pProp = GetParam()->GetProperties();
	pProp->SetMarkColHeader(FALSE); // Turn off pressed button effect for column headers
	pProp->SetMarkRowHeader(FALSE);
	pProp->SetUserProperty(GX_IDS_OUTLINECURRENTCELL, // 현재 셀의 border 설정 변경
		(CGXStyle)pProp->sInvertNoBorder);

	LockUpdate(TRUE);

	m_nCol = 8;
	m_nRow = m_LcomD.GetSize();
	SetColCount(m_nCol);
	SetRowCount(m_nRow);
	SetTitle();
	SetHeaderTitle();
	Data2Grid();
	Redraw();

	ChangeStandardStyle(CGXStyle()
		.SetFont(CGXFont_GC().SetSize(9))
		.SetVerticalAlignment(DT_BOTTOM)
		.SetAllowEnter(FALSE));
	ChangeColHeaderStyle(CGXStyle().SetFont(CGXFont_GC().SetBold(FALSE)));
	ChangeRowHeaderStyle(CGXStyle()
		.SetFont(CGXFont_GC().SetBold(FALSE))
		.SetHorizontalAlignment(DT_RIGHT)
		.SetControl(GX_IDS_CTRL_HEADER));
	GetParam()->SetNumberedRowHeaders(TRUE);

	SetStyleRange(CGXRange().SetCols(2, m_nCol), CGXStyle()
		.SetHorizontalAlignment(DT_CENTER)
		.SetValueType(GX_VT_NUMERIC)
		.SetFormat(GX_FMT_FIXED)
		.SetPlaces(2));

	SetRowHeightByDPI(0, 0, 36);
	SetStyleRange(CGXRange().SetCols(1), CGXStyle()
		.SetHorizontalAlignment(DT_CENTER)
		.SetValueType(GX_VT_NUMERIC)
		.SetFormat(GX_FMT_FIXED)
		.SetEnabled(FALSE));

	SetRightValidCell(1, 0);
	GetParam()->SetHideCurrentCell(GX_HIDE_NEVER);

	SetScrollBarMode(SB_HORZ, gxnEnabled);
	SetScrollBarMode(SB_VERT, gxnEnabled);

	GetParam()->EnableMoveRows(TRUE);

	m_aSortInfo.SetSize(1);
	m_aSortInfo[0].nRC = 0;
	m_aSortInfo[0].sortType = CGXSortInfo::numeric;
	m_aSortInfo[0].sortOrder = CGXSortInfo::ascending;

	SetStyleRange(CGXRange().SetCols(1, 1), CGXStyle()
		.SetReadOnly(TRUE));
	SetStyleRange(CGXRange().SetCols(2, GetColCount()), CGXStyle()
		.SetReadOnly(FALSE));

	LockUpdate(FALSE);
	Redraw();
}

void CCmdDampLoadCombGridWnd::SetHeaderTitle()
{
	int nColWidth[10] = { 80, 150, 80, 80,80,80,80,80 };
	CString csTitle;

	CGXStyle style;
	style.SetControl(GX_IDS_CTRL_STATIC);
	style.SetHorizontalAlignment(DT_CENTER);
	style.SetVerticalAlignment(DT_VCENTER);
	style.SetDraw3dFrame(gxFrameRaised);
	style.SetFont(CGXFont_GC().SetBold(FALSE).SetSize(9));

	for (int i = 0; i < m_nCol; i++)
	{
		csTitle = m_aTitle[i];
		SetStyleRange(CGXRange(0, i + 1), CGXStyle()
			.SetValue(csTitle)
			.SetUserAttribute(ID_USER_ATTR_COL_ID, m_aTitle[i]));
		SetColWidthByDPI(i + 1, i + 1, nColWidth[i]);
	}
	SetColWidthByDPI(0, 0, 0);

}

void CCmdDampLoadCombGridWnd::GetTitleUnitName(CString &strUnit_L, CString &strUnit_F)
{
	T_UNIT_INDEX CurIndex;
	m_pDoc->m_pUnitCtrl->GetUnitIndexCurrent(CurIndex);
	int iLenID = CurIndex.nBase_Length;
	int iForID = CurIndex.nBase_Force;

	switch (iLenID)
	{
	case (D_UNITSYS_LENGTH_INDEX_MM): strUnit_L = D_UNITSYS_LENGTH_NAME_MM; break;
	case (D_UNITSYS_LENGTH_INDEX_CM): strUnit_L = D_UNITSYS_LENGTH_NAME_CM; break;
	case (D_UNITSYS_LENGTH_INDEX_M):  strUnit_L = D_UNITSYS_LENGTH_NAME_M;  break;
	case (D_UNITSYS_LENGTH_INDEX_IN): strUnit_L = D_UNITSYS_LENGTH_NAME_IN; break;
	case (D_UNITSYS_LENGTH_INDEX_FT): strUnit_L = D_UNITSYS_LENGTH_NAME_FT; break;
	default:
		break;
	}

	switch (iForID)
	{
	case (D_UNITSYS_FORCE_INDEX_KG): strUnit_F = D_UNITSYS_FORCE_NAME_KG; break;
	case (D_UNITSYS_FORCE_INDEX_TON):strUnit_F = D_UNITSYS_FORCE_NAME_TON; break;
	case (D_UNITSYS_FORCE_INDEX_N):  strUnit_F = D_UNITSYS_FORCE_NAME_N;  break;
	case (D_UNITSYS_FORCE_INDEX_KN): strUnit_F = D_UNITSYS_FORCE_NAME_KN; break;
	case (D_UNITSYS_FORCE_INDEX_LBF): strUnit_F = D_UNITSYS_FORCE_NAME_LBF; break;
	case (D_UNITSYS_FORCE_INDEX_KIP): strUnit_F = D_UNITSYS_FORCE_NAME_KIP; break;
	default:
		break;
	}
}

void CCmdDampLoadCombGridWnd::SetTitle()
{
	m_aTitle.RemoveAll();
	CString aTitle1[8] = { _LS(IDS_CMD_DAMP_LOADCOMBTABLEDLG_1),_LS(IDS_CMD_DAMP_LOADCOMBTABLEDLG_2),_LS(IDS_CMD_DAMP_LOADCOMBTABLEDLG_3),
		_LS(IDS_CMD_DAMP_LOADCOMBTABLEDLG_4),_LS(IDS_CMD_DAMP_LOADCOMBTABLEDLG_5),_LS(IDS_CMD_DAMP_LOADCOMBTABLEDLG_6),
		_LS(IDS_CMD_DAMP_LOADCOMBTABLEDLG_7),_LS(IDS_CMD_DAMP_LOADCOMBTABLEDLG_8) };

	for (int i = 0; i < 8; i++)
	{
		m_aTitle.Add(aTitle1[i]);
	}
}

void CCmdDampLoadCombGridWnd::Data2Grid()
{
	CString strType = _T("");
	T_LCOM_D GravityData;
	T_SPLC_D SplcData;
	T_THIS_D ThisData;
	for (int i = 0; i < m_nRow; i++)
	{
		const T_LCOM_D& LcomD = m_LcomD[i];
		if (LcomD.aCombination.GetSize() < 2)
		{
			ASSERT(FALSE);
			continue;
		}
		strType.Format(_T("%d"), LcomD.LoadCombId);
		SetValueRange(CGXRange(i + 1, 1), strType);
		SetValueRange(CGXRange(i + 1, 2), LcomD.LoadCombName);
		ASSERT(LcomD.aCombination[0].AnalType == D_LCOM_CB_GENERAL);
		if (m_pDoc->m_pAttrCtrl->GetLcom(D_LCOMTYPE_GENERAL,LcomD.aCombination[0].LoadCaseKey, GravityData))
		{
			SetValueRange(CGXRange(i + 1,3), LcomD.aCombination[0].Factor);
			SetValueRange(CGXRange(i + 1,4), GravityData.LoadCombName);
		}
		else
		{
			ASSERT(FALSE);
			SetValueRange(CGXRange(i + 1, 3), _T("-"));
			SetValueRange(CGXRange(i + 1, 4), _T("-"));
		}

		if (LcomD.aCombination[1].AnalType == D_LCOM_SPECTRUM)
		{
			if (m_pDoc->m_pAttrCtrl->GetSplc(LcomD.aCombination[1].LoadCaseKey, SplcData))
			{
				SetValueRange(CGXRange(i + 1, 5), LcomD.aCombination[1].Factor);
				SetValueRange(CGXRange(i + 1, 6), SplcData.LoadCaseName);
			}
			else
			{
				SetValueRange(CGXRange(i + 1, 5), _T("-"));
				SetValueRange(CGXRange(i + 1, 6), _T("-"));
			}

			if (LcomD.aCombination.GetSize() > 2 && m_pDoc->m_pAttrCtrl->GetSplc(LcomD.aCombination[2].LoadCaseKey, SplcData))
			{
				SetValueRange(CGXRange(i + 1, 7), LcomD.aCombination[2].Factor);
				SetValueRange(CGXRange(i + 1, 8), SplcData.LoadCaseName);
			}
			else
			{
				SetValueRange(CGXRange(i + 1, 7), _T("-"));
				SetValueRange(CGXRange(i + 1, 8), _T("-"));
			}
		}
		else if (LcomD.aCombination[1].AnalType == D_LCOM_HISTORY)
		{
			if (m_pDoc->m_pAttrCtrl->GetThis(LcomD.aCombination[1].LoadCaseKey, ThisData))
			{
				SetValueRange(CGXRange(i + 1, 5), LcomD.aCombination[1].Factor);
				SetValueRange(CGXRange(i + 1, 6), ThisData.LoadCaseName);
			}
			else
			{
				SetValueRange(CGXRange(i + 1, 5), _T("-"));
				SetValueRange(CGXRange(i + 1, 6), _T("-"));
			}

			if (LcomD.aCombination.GetSize() > 2 && m_pDoc->m_pAttrCtrl->GetThis(LcomD.aCombination[2].LoadCaseKey, ThisData))
			{
				SetValueRange(CGXRange(i + 1, 7), LcomD.aCombination[2].Factor);
				SetValueRange(CGXRange(i + 1, 8), ThisData.LoadCaseName);
			}
			else
			{
				SetValueRange(CGXRange(i + 1, 7), _T("-"));
				SetValueRange(CGXRange(i + 1, 8), _T("-"));
			}
		}
	}
}

//void CCmdDampLoadCombGridWnd::Grid2Data()
//{
//	for (int i = 1; i < m_nCol; i++)
//	{
//		//pIsgbD->dLNR[i - 1] = _ttof(GetValueRowCol(1, i + 1));
//		//pIsgbD->dESB[i - 1] = _ttof(GetValueRowCol(2, i + 1));
//		//pIsgbD->dFPS[i - 1] = _ttof(GetValueRowCol(3, i + 1));
//	}
//
//}

void CCmdDampLoadCombGridWnd::Write_Cell(ROWCOL nRow, ROWCOL nCol, CGXStyle& rstyle, const CString& strValue)
{
	rstyle.SetValue(strValue);
	SetStyleRange(CGXRange(nRow, nCol), rstyle);
}

void CCmdDampLoadCombGridWnd::Write_Cell(ROWCOL nRow, ROWCOL nCol, CGXStyle& rstyle, const TCHAR* Fmt, ...)
{
	va_list argptr;
	va_start(argptr, Fmt);

	CString strTemp;
	strTemp.FormatV(Fmt, argptr);
	va_end(argptr);

	Write_Cell(nRow, nCol, rstyle, strTemp);
}

void CCmdDampLoadCombGridWnd::Write_CellMerge(ROWCOL nRow1, ROWCOL nCol1, ROWCOL nRow2, ROWCOL nCol2, CGXStyle& rstyle, const CString& strValue)
{
	rstyle.SetValue(strValue);
	SetCoveredCellsRowCol(nRow1, nCol1, nRow2, nCol2);
	SetStyleRange(CGXRange(nRow1, nCol1, nRow2, nCol2), rstyle);
}

void CCmdDampLoadCombGridWnd::Write_CellMerge(ROWCOL nRow1, ROWCOL nCol1, ROWCOL nRow2, ROWCOL nCol2, CGXStyle& rstyle, const TCHAR* Fmt, ...)
{
	va_list argptr;
	va_start(argptr, Fmt);

	CString strTemp;
	strTemp.FormatV(Fmt, argptr);
	va_end(argptr);

	Write_CellMerge(nRow1, nCol1, nRow2, nCol2, rstyle, strTemp);
}

void CCmdDampLoadCombGridWnd::OnModifyCell(ROWCOL nRow, ROWCOL nCol)
{
	if (IsAppendRow(nRow)) AddNew(nRow);
	else Edit(nRow, TRUE);
	CGXGridWnd::OnModifyCell(nRow, nCol);
	//SetNewLineNo(nRow, nCol);
}

//BOOL CCmdDampLoadCombGridWnd::OnPasteFromClipboard(const CGXRange &range)
//{
//	if (CGXGridWnd::OnPasteFromClipboard(range))
//	{
//		return TRUE;
//	}
//	 return FALSE;
//}

CString CCmdDampLoadCombGridWnd::GetEditingValue(ROWCOL nRow, ROWCOL nCol)
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
