#include "stdafx.h"
#include "wg_dgnRes.h"
#include "DgnMergeExcelDlg.h"

#pragma region Merge Dialog

#define COLORGRAY COLORREF(RGB(233, 235, 239))
#define COLORWHITE COLORREF(RGB(255, 255, 255))
#define DEFAULT_PATH _T("C:\\")

CDgnMergeExcelDlg::CDgnMergeExcelDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CDgnMergeExcelDlg::IDD, pParent)
{
	m_pGridCtrl = NULL;
}

CDgnMergeExcelDlg::~CDgnMergeExcelDlg()
{
}

void CDgnMergeExcelDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CDgnMergeExcelDlg, CDialogMove)
	//{{AFX_MSG_MAP(CDgnSeisBrgInfoDlg)
	ON_BN_CLICKED(IDOK, OnOk)
	ON_BN_CLICKED(IDCANCEL, OnCancel)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CDgnMergeExcelDlg::OnInitDialog()
{
	CDialogMove::OnInitDialog();

	if (m_pGridCtrl != NULL)
	{
		delete m_pGridCtrl;
		m_pGridCtrl = NULL;
	}
	m_pGridCtrl = new CMergeGrid();

	// 동적 생성
	CRect rectDummy;
	CWnd* pStatic = GetDlgItem(IDC_MERGE_EXCEL_CALC_GRID);
	// 영역 가져오기
	pStatic->GetClientRect(&rectDummy);
	pStatic->MapWindowPoints(this, rectDummy);
	pStatic->ShowWindow(FALSE); // 기존 리소스 숨기기
	// 생성 -> 마지막 인수는 단순 식별 번호로 사용
	if (!m_pGridCtrl->Create(WS_CHILD | WS_VISIBLE, rectDummy, this, IDC_MERGE_EXCEL_CALC_GRID))
	{
		//ASSERT(FALSE);
		return FALSE;
	}

	return TRUE;
}

void CDgnMergeExcelDlg::OnOk()
{
	CDialogMove::OnOK();
}

void CDgnMergeExcelDlg::OnCancel()
{
	CDialogMove::OnCancel();
}
#pragma endregion


#pragma region Merge Grid
CMergeGrid::CMergeGrid()
{
}
CMergeGrid::~CMergeGrid()
{
}
BEGIN_MESSAGE_MAP(CMergeGrid, CBCGPGridCtrl)
	ON_WM_CREATE()
 	ON_WM_LBUTTONDOWN()
 	ON_WM_MOUSEMOVE()
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()

int CMergeGrid::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CBCGPGridCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (GetColumnCount() == 0)
	{
		//--------------------------
		// Set grid general options:
		//--------------------------
		EnableMarkSortedColumn(FALSE, FALSE);
		EnableHeader(TRUE);
		SetClearInplaceEditOnEnter(FALSE);
		SetEditFirstClick(FALSE);
		EnableInvertSelOnCtrl();
		SetScalingRange(0.1, 4.0);
		//-----------------
		// MultilineHeader:
		//-----------------
		EnableRowHeader(TRUE);
		EnableGridLines(TRUE);
		//-------------
		// Add columns:
		//-------------
		CRect rect;
		GetClientRect(rect);
		int nWidth = rect.Width();
		std::vector<std::tuple<int, int, CString, int>> svPair;
		svPair.push_back(std::make_tuple(eCol::kDirection, nWidth * 0.25, _LS(IDS_IDD_SEIS_MERGE_EXCEL_DLG_JP_GRID_COL_DIR), HDF_CENTER));
		svPair.push_back(std::make_tuple(eCol::kType, nWidth * 0.20, _LS(IDS_IDD_SEIS_MERGE_EXCEL_DLG_JP_GRID_COL_TYPE), HDF_CENTER));
		svPair.push_back(std::make_tuple(eCol::kBtn, nWidth * 0.05, _T(""), HDF_CENTER));
		svPair.push_back(std::make_tuple(eCol::kFile, nWidth * 0.50, _LS(IDS_IDD_SEIS_MERGE_EXCEL_DLG_JP_GRID_COL_FILE), HDF_CENTER));
		int nColumns = static_cast<int>(svPair.size());
		for (int nColumn = 0; nColumn < nColumns; nColumn++)
		{
			int nIdx = std::get<0>(svPair[nColumn]);
			int nWidth = std::get<1>(svPair[nColumn]);
			CString strName = std::get<2>(svPair[nColumn]);
			InsertColumn(nIdx, strName, nWidth);
			GetColumnsInfo().SetHeaderMultiLine(nColumn, FALSE);
			GetColumnsInfo().SetColumnData(nColumn, nIdx);
			SetHeaderAlign(nColumn, HDF_CENTER);
			SetColumnAlign(nColumn, std::get<3>(svPair[nColumn]));
			//			GetColumnsInfo().SetColumnLocked(nColumn,TRUE);
		}
		BCGP_GRID_COLOR_DATA pColor;
		pColor = GetColorTheme();
		pColor.m_clrBackground = RGB(255, 255, 255);
		pColor.m_clrText = RGB(0, 0, 0);
		SetColorTheme(pColor);
		GetColumnsInfo().EnableAutoSize(TRUE);
		//GetColumnsInfo().EnableCheckBox(TRUE);
	}
	EnableDragHeaderItems(FALSE);
	EnableHeader(TRUE, BCGP_GRID_HEADER_MOVE_ITEMS | BCGP_GRID_HEADER_HIDE_ITEMS);
	EnableRowHeader(TRUE, BCGP_GRID_HEADER_SELECT);

	InitRow();

	AdjustLayout();
	return 0;
}

void CMergeGrid::InitRow()
{
	for(int i=0; i<4; i++)
	{
		CBCGPGridRow* pRow = new CBCGPGridRow(GetColumnCount());
		AddRow(pRow);
		// Direction
		CBCGPGridItem* pItem = new CBCGPGridItem(_T(""));
		pItem->SetReadOnly(TRUE);
		pRow->AddItem(pItem);
		// Type
		CString strType;
		strType.Format(_LS(IDS_IDD_SEIS_MERGE_EXCEL_DLG_JP_GRID_ROW_TYPE), i % 2 + 1);
		pItem = new CBCGPGridItem((_variant_t)strType);
		pItem->SetReadOnly(TRUE);
		pRow->AddItem(pItem);
		// Btn
		pItem = new CBCGPGridItem(_T("..."));
		pItem->SetBackgroundColor(COLORGRAY);
		pItem->SetReadOnly(TRUE);
		pRow->AddItem(pItem);
		// File
		pItem = new CBCGPGridItem(DEFAULT_PATH);
		pItem->SetReadOnly(TRUE);
		pRow->AddItem(pItem);
	}
	
	// Merge
	for (int i = 0; i < 4; i+=2)
	{
		CBCGPGridRow* pMergeRow = GetRow(i);
		CBCGPGridRow* pMergeRow2 = GetRow(i+1);
		CBCGPGridItemID pID = pMergeRow->GetItem(eCol::kDirection)->GetGridItemID();
		CBCGPGridItemID pID2 = pMergeRow2->GetItem(eCol::kDirection)->GetGridItemID();
		MergeRange(CBCGPGridRange(pID, pID2));
		if(i<2)pMergeRow->GetItem(eCol::kDirection)->SetValue((_variant_t)_LS(IDS_IDD_SEIS_MERGE_EXCEL_DLG_JP_GRID_ROW_AXIS));
		else pMergeRow->GetItem(eCol::kDirection)->SetValue((_variant_t)_LS(IDS_IDD_SEIS_MERGE_EXCEL_DLG_JP_GRID_ROW_PERP));
	}
}

void CMergeGrid::FileOpen(const int& nRow)
{
	CString strFilter = _T("mcb Files(*.mcb, *.mcbz)|*.mcb;*.mcbz|mcb Files(*.mcb)|*.mcb|mcbz Files (*.mcbz)|*.mcbz||");

	CFileDialog fd(TRUE, _T(".mcb"), NULL,
		OFN_FILEMUSTEXIST | OFN_LONGNAMES,
		strFilter, NULL);
	if (fd.DoModal() == IDOK)
	{
		// filename 
		CString strFileName = fd.GetFileName();
		CString strPath = fd.GetPathName();

		int nSeisDesc = T_SIPA_D::kUnKnown_Dir;
		int nSeisType = T_SIPA_D::kType0;
		GetMapIndexFromRow(nRow, nSeisDesc, nSeisType);

		m_mapFilePath[nSeisDesc][nSeisType] = strPath;
		GetRow(nRow)->GetItem(eCol::kFile)->SetValue((_variant_t)strFileName);
	}
}

void CMergeGrid::GetMapIndexFromRow(const int& nRow, int& nSeisDesc, int& nSeisType)
{
	switch (nRow)
	{
	case 0:
		nSeisDesc = T_SIPA_D::kAxis_Dir;
		nSeisType = T_SIPA_D::kType1;
		break;
	case 1:
		nSeisDesc = T_SIPA_D::kAxis_Dir;
		nSeisType = T_SIPA_D::kType2;
		break;
	case 2:
		nSeisDesc = T_SIPA_D::kPerp_Dir;
		nSeisType = T_SIPA_D::kType1;
		break;
	case 3:
		nSeisDesc = T_SIPA_D::kPerp_Dir;
		nSeisType = T_SIPA_D::kType2;
		break;
	default:
		break;
	}
}

void CMergeGrid::OnLButtonDown(UINT nFlags, CPoint point)
{
	CBCGPGridCtrl::OnLButtonDown(nFlags, point);

	int nRow = -1, nCol = -1;
	CBCGPGridItemID id;
	CBCGPGridItem* pItem = NULL;
	HitTest(point, id, pItem);

	nRow = id.m_nRow;
	nCol = id.m_nColumn;

	if (nRow >= 0 && nCol == eCol::kBtn)
	{
		FileOpen(nRow);
	}
}

void CMergeGrid::OnMouseMove(UINT nFlags, CPoint point)
{
	CBCGPGridCtrl::OnMouseMove(nFlags, point);

	int nRow = -1, nCol = -1;
	CBCGPGridItemID id;
	CBCGPGridItem* pItem = NULL;
	HitTest(point, id, pItem);

	nRow = id.m_nRow;
	nCol = id.m_nColumn;
	for (int i = 0; i < 4; i++)
	{
		CBCGPGridItem* pItem = GetRow(i)->GetItem(eCol::kBtn);
		pItem->SetBackgroundColor(COLORGRAY);
	}

	if (nRow >= 0 && nCol == eCol::kBtn) pItem->SetBackgroundColor(COLORWHITE);
}

void CMergeGrid::OnSetFocus(CWnd* pOldWnd)
{
	CBCGPGridCtrl::OnSetFocus(pOldWnd);
	Invalidate();
}

void CMergeGrid::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	CBCGPGridCtrl::OnKeyDown(nChar, nRepCnt, nFlags);

	// Delete Key
	if (nChar == VK_DELETE)
	{
		CBCGPGridItemID id = GetCurSelItemID();
		if (id.m_nRow >= 0 && id.m_nColumn == eCol::kFile)
		{
			int nSeisDesc = T_SIPA_D::kUnKnown_Dir;
			int nSeisType = T_SIPA_D::kType0;
			GetMapIndexFromRow(id.m_nRow, nSeisDesc, nSeisType);
			m_mapFilePath[nSeisDesc][nSeisType].Empty();
			GetRow(id.m_nRow)->GetItem(eCol::kFile)->SetValue((_variant_t)DEFAULT_PATH);
		}
	}

	Invalidate();
}

void CMergeGrid::OnDestroy()
{
	CBCGPGridCtrl::OnDestroy();
}
#pragma endregion