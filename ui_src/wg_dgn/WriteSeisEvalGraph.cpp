#include "stdafx.h"
#include "wg_dgn.h"
#include "WriteSeisEvalGraph.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\DB_ST_DN_ENUM.h"
#include "..\wg_db\wg_db_PostCtrl.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl2.h"
#include "..\wg_db\DB_QUERY_SEIS_CVL_JP.h"
#include "..\wg_db\BrdgSeisEvalResult_JP.h"

//#include "DgnExcelPrintDlg.h" // UI
#include "DgnProgressDlg.h"
#include "..\mit_frx\MSExcel.h"

using namespace dgn::lib;

// ===== Cell size =====
#define EXCEL_ONE_CELL_WIDTH          7.0     // 문자 수 ≒ 37 point
#define EXCEL_ONE_CELL_HEIGHT         15    // point

// ===== A3 (Landscape) =====
#define EXCEL_A3_MAX_ROW              58 
#define EXCEL_A3_MAX_COL              24

// ===== Table layout =====
#define  EXCEL_TALBE_START_ROW			3
#define  EXCEL_TALBE_START_COL			30

// ===== Graph layout =====
#define EXCEL_GRAPH_START_ROW         3
#define EXCEL_GRAPH_START_COL         0

#define EXCEL_GRAPH_TITLE_ROWS        4

#define GRAPH_ROWS                    21
#define GRAPH_COLS                    4

#define GRAPH_TOTAL_ROWS       (EXCEL_GRAPH_TITLE_ROWS + GRAPH_ROWS)

// ===== Graph size (point) =====
#define GRAPH_WIDTH                   (EXCEL_ONE_CELL_WIDTH * GRAPH_COLS * 7)
#define GRAPH_HEIGHT                  (EXCEL_ONE_CELL_HEIGHT * GRAPH_ROWS)

// ===== Legend (optional) =====
#define GRAPH_LEGEND_WIDTH            GRAPH_WIDTH
#define GRAPH_LEGEND_HEIGHT           GRAPH_HEIGHT/6

#define MAX_NUM									1.0E+21
#define MIN_NUM									-1.0E+21	

#pragma region CSeisEvalGraphBase

CSeisEvalGraphBase::CSeisEvalGraphBase(mit::frx::CMSExcel* pXL, CDgnProgressDlg* pProgress, const CString& SheetName, const UINT& nDesignCode)
	/*: iWriteSeisEvalReportBase()*/
{
	m_pXL = pXL;
	m_pProgress = pProgress;
	m_nDesignCode = nDesignCode;
	m_GraphPos = { EXCEL_GRAPH_START_ROW, EXCEL_GRAPH_START_COL }; // row, col
	m_TablePos = { EXCEL_TALBE_START_ROW, EXCEL_TALBE_START_COL }; // row, col
	m_TableSize = { 0,0 }; // width, height

	m_pDoc = CDBDoc::GetDocPoint();
	if (!m_pDoc) return;
	m_pResult = m_pDoc->m_pPostCtrl->GetBrdgSeisEvalResult_JP();

	m_BaseSheetName = SheetName;
	m_CurrentSheetCnt = 1;

	m_ChartTitle = _T("");
	m_bGraph = FALSE;
}

void CSeisEvalGraphBase::WriteGraphTitle(int& stRow)
{
	m_pXL->SetValue(stRow -1, m_GraphPos.y, m_ChartTitle);
}

void CSeisEvalGraphBase::SetLegendName(const int& nType, const int& nIndex, CString& strLegend)
{
	if (nType == eType::kFlexure_Phi)
	{
		switch (nIndex)
		{
			case 1:
			case 2:
			{
				CString strMinMax = (nIndex == 1) ? _LS(IDS_BRDG_SEIS_EVAL_REPORT_JP_GRAPH_LEGEND_MIN) : _LS(IDS_BRDG_SEIS_EVAL_REPORT_JP_GRAPH_LEGEND_MAX);
				strLegend.Format(_LS(IDS_BRDG_SEIS_EVAL_REPORT_JP_GRAPH_LEGEND_dAverge), strMinMax);
			}
				break;
			case 3:
			case 4:
			{
				CString strMinMax = (nIndex == 3) ? _LS(IDS_BRDG_SEIS_EVAL_REPORT_JP_GRAPH_LEGEND_MIN) : _LS(IDS_BRDG_SEIS_EVAL_REPORT_JP_GRAPH_LEGEND_MAX);
				strLegend.Format(_LS(IDS_BRDG_SEIS_EVAL_REPORT_JP_GRAPH_CURVATURE_LEGEND_dPhi_y), strMinMax);
			}
			break;
			case 5:
			case 6:
			{
				CString strMinMax = (nIndex == 5) ? _LS(IDS_BRDG_SEIS_EVAL_REPORT_JP_GRAPH_LEGEND_MIN) : _LS(IDS_BRDG_SEIS_EVAL_REPORT_JP_GRAPH_LEGEND_MAX);
				strLegend.Format(_LS(IDS_BRDG_SEIS_EVAL_REPORT_JP_GRAPH_CURVATURE_LEGEND_dPhi_u), strMinMax);
			}
			break;
		default:
			break;
		}
	}
	else if (nType == eType::kShear)
	{
		switch (nIndex)
		{
		case 1:
		case 2:
		{
			CString strMinMax = (nIndex == 1) ? _LS(IDS_BRDG_SEIS_EVAL_REPORT_JP_GRAPH_LEGEND_MIN) : _LS(IDS_BRDG_SEIS_EVAL_REPORT_JP_GRAPH_LEGEND_MAX);
			strLegend.Format(_LS(IDS_BRDG_SEIS_EVAL_REPORT_JP_GRAPH_LEGEND_dAverge), strMinMax);
		}
		break;
		case 3:
		case 4:
		{
			CString strMinMax = (nIndex == 1) ? _LS(IDS_BRDG_SEIS_EVAL_REPORT_JP_GRAPH_LEGEND_MIN) : _LS(IDS_BRDG_SEIS_EVAL_REPORT_JP_GRAPH_LEGEND_MAX);
			strLegend.Format(_LS(IDS_BRDG_SEIS_EVAL_REPORT_JP_GRAPH_SHEAR_LEGEND_dPs), strMinMax);
		}
		default:
			break;
		}
	}
}

void CSeisEvalGraphBase::GetTargetElements(const T_SISR_K& kSigr, const T_SIGR_D::eSubType& nGroupType,
	OUT T_ELEM_K_LIST& aElemKeys, OUT std::map <T_ELEM_K, std::tuple<CString, int>>& mElemInfo,
	const T_SIGR_LOWER_D::eMaterialType& nMatType/*=T_SIGR_LOWER_D::kUnKnown*/, const int& nMembType/*=DB_MEMB_TYPE::D_MBTP_ETC*/)
{
	if (m_pDoc == nullptr) return;

	auto IsEqualMaterial = [&](const T_ELEM_K& kElem)
		{
			T_ELEM_D elemD; T_MATL_D matdD;
			if (m_pDoc->m_pAttrCtrl->GetElem(kElem, elemD) == FALSE) return false;
			if (m_pDoc->m_pPostCtrl->GetMatlPost(elemD.elmat, matdD) == FALSE) return false;

			switch (nMatType)
			{
			case T_SIGR_LOWER_D::eMaterialType::kRC:
				if (matdD.Type == _T("C")) return true;
				break;
			case T_SIGR_LOWER_D::eMaterialType::kSteel:
				if (matdD.Type == _T("S")) return true;
				break;
			case T_SIGR_LOWER_D::eMaterialType::kSRC:
				if (matdD.Type == _T("SRC")) return true;
				break;
			default:
				ASSERT(FALSE);
				break;
			}
			return false;
		};

	auto IsEqualMemberType = [&](const T_ELEM_K& kElem)
		{
			if (m_pDoc->m_pAttrCtrl->GetMemberType(kElem) == nMembType) return true;
			else return false;
		};

	T_SIGR_D dSigr;
	if (m_pDoc->m_pAttrCtrl2->GetQSigr()->Get(kSigr, dSigr) == FALSE) return;
	if (dSigr.nSubType != nGroupType) return;

	int nDir = 0;
	T_ELEM_K_LIST elemKeys;
	switch (dSigr.nSubType)
	{
	case T_SIGR_D::kUpper:
	{
		T_SIGR_UPPER_D SigrUpperD;
		if (!m_pDoc->m_pAttrCtrl2->GetQSigrUpper()->Get(kSigr, SigrUpperD)) return;
		nDir = SigrUpperD.nDirShear;
		elemKeys.Append(SigrUpperD.aElements);
	}
	break;
	case T_SIGR_D::kUser:
	{
		T_SIGR_USER_D SigrUserD;
		if (!m_pDoc->m_pAttrCtrl2->GetQSigrUser()->Get(kSigr, SigrUserD)) return;
		nDir = SigrUserD.nDirShear;
		elemKeys.Append(SigrUserD.aInvestigatedElem);
	}
	break;
	default:
		ASSERT(FALSE);
		break;
	}

	for (int i = 0; i < elemKeys.GetCount(); i++)
	{
		const T_ELEM_K& kElem = elemKeys[i];
		if (nMatType != T_SIGR_LOWER_D::kUnKnown) {
			if (IsEqualMaterial(kElem) == false) continue;
		}
		if (nMembType != DB_MEMB_TYPE::D_MBTP_ETC) {
			if (IsEqualMemberType(kElem) == false) continue;
		}

		aElemKeys.Add(kElem);
		std::tuple<CString, int> tInfo = std::make_tuple(_T("--"), nDir);
		mElemInfo.insert(std::make_pair(kElem, tInfo));
	}
}

void CSeisEvalGraphBase::GetTargetElements(const T_SISR_K& kSigr, const T_SIGR_D::eSubType& nGroupType,
	const T_SIGR_LOWER_D::eMaterialType& nMatType, const int& nMembType,
	OUT T_ELEM_K_LIST& aElemKeys,
	OUT std::map <T_ELEM_K, std::tuple<CString, int>>& mElemInfo)
{
	if (m_pDoc == nullptr) return;

	T_SIGR_D dSigr;
	if (m_pDoc->m_pAttrCtrl2->GetQSigr()->Get(kSigr, dSigr) == FALSE) return;
	if (dSigr.nSubType != nGroupType) return;

	auto L_SetData = [&](const T_ELEM_K& kElem, const CString& strName, const int& nDir)
		{
			aElemKeys.Add(kElem);
			std::tuple<CString, int> tInfo = std::make_tuple(strName, nDir);
			mElemInfo.insert(std::make_pair(kElem, tInfo));
		};

	switch (dSigr.nSubType)
	{
	case T_SIGR_D::eSubType::kLower:
	{
		T_SIGR_LOWER_D SigrLowerD;
		if (!m_pDoc->m_pAttrCtrl2->GetQSigrLower()->Get(kSigr, SigrLowerD)) return;
		if (SigrLowerD.nMaterialType != nMatType) return;

		if (nMembType == D_MBTP_COLUMN) {
			const auto nColmD = SigrLowerD.COLM_D.GetSize();
			for (int i = 0; i < nColmD; ++i) {
				const T_SIGR_LOWER_COLM_D& colm = SigrLowerD.COLM_D[i];

				for (int j = 0; j < colm.aInvestigatedElem.GetCount(); j++) {
					const T_ELEM_K& kElem = colm.aInvestigatedElem[j];
					L_SetData(kElem, colm.strName, colm.nDirShear);
				}
			}
		}
		else if (nMembType == D_MBTP_BEAM) {
			const auto nBeamD = SigrLowerD.BEAM_D.GetSize();
			for (int i = 0; i < nBeamD; ++i) {
				const T_SIGR_LOWER_BEAM_D& beam = SigrLowerD.BEAM_D[i];

				for (int j = 0; j < beam.aInvestigatedElem.GetCount(); j++) {
					const T_ELEM_K& kElem = beam.aInvestigatedElem[j];
					L_SetData(kElem, beam.strName, beam.nDirShear);
				}
			}
		}
	}
	break;
	case T_SIGR_D::kUpper:
	case T_SIGR_D::kUser:
	{
		return GetTargetElements(kSigr, nGroupType, aElemKeys, mElemInfo, nMatType, nMembType);
	}
	break;
	default:
		ASSERT(FALSE);
		break;
	}
}

void CSeisEvalGraphBase::SetCurrentSheetNum(const int& sheetNum)
{
	m_CurrentSheetCnt = sheetNum;
}

int CSeisEvalGraphBase::GetCurrentSheetNum()
{
	return m_CurrentSheetCnt;
}

void CSeisEvalGraphBase::SetPosNextGraph(const POINT& graphPos, const POINT& tablePos)
{
	m_GraphPos = graphPos;
	m_TablePos = tablePos;
	m_GraphPos.y = EXCEL_GRAPH_START_COL;
	m_GraphPos.x += (GRAPH_TOTAL_ROWS + 2);
	m_TablePos.y = EXCEL_TALBE_START_COL;
	m_TablePos.x += 2;
}

void CSeisEvalGraphBase::GetPosPreGraph(POINT& graphPos, POINT& tablePos)
{
	graphPos = m_GraphPos;
	tablePos = m_TablePos;
}

void CSeisEvalGraphBase::CheckSheetSize()
{
	if (m_GraphPos.y + GRAPH_COLS > EXCEL_A3_MAX_COL)
	{
		m_GraphPos.y = EXCEL_GRAPH_START_COL;
		m_GraphPos.x += (GRAPH_TOTAL_ROWS+2);
		m_TablePos.y = EXCEL_TALBE_START_COL;
		m_TablePos.x += m_TableSize.x + 2;
	}

	if (m_GraphPos.x + GRAPH_TOTAL_ROWS > EXCEL_A3_MAX_ROW)
	{
		// Reset Position
		m_GraphPos = { EXCEL_GRAPH_START_ROW, EXCEL_GRAPH_START_COL };
		m_TablePos = { EXCEL_TALBE_START_ROW, EXCEL_TALBE_START_COL };

		// New Sheet
		m_CurrentSheetCnt++;
		AddSheet();
	}
}

void CSeisEvalGraphBase::AddSheet()
{
	m_NewSheetName.Format(_T("%s_%d"), m_BaseSheetName, m_CurrentSheetCnt);
	if (m_pXL->IsExistSheetName(m_NewSheetName))
	{
		m_pXL->SetActiveSheet(m_NewSheetName);
	}
	else
	{
		m_pXL->AddSheet2(m_NewSheetName);
		m_pXL->SetActiveSheet(m_NewSheetName);
		//m_pXL->SetStandardFont(_T("Meiryo UI"));
		m_pXL->SetPageSetup(XlPageOrientation::xlLandscape, XlPaperSize::xlPaperA3);
		m_pXL->SetZoom(85);
		//m_pXL->SetMargin(0, 0, 0, 0);
		m_pXL->SetMargin(20, 10, 20, 20);
		m_pXL->SetColumnWidth(0, 0, 60, 60, EXCEL_ONE_CELL_WIDTH);
		m_pXL->SetRowHeight(0, 0, 60, 60, EXCEL_ONE_CELL_HEIGHT);
		m_pXL->SetPrintArea(EXCEL_A3_MAX_ROW-2, EXCEL_A3_MAX_COL-1);
	}
}

BOOL CSeisEvalGraphBase::WriteGraph(mit::frx::ChartData& data)
{
	try
	{
		SetDefaultMutiParam(data);
		m_pXL->AddChartScatterMultiSeriesYRev(data, m_GraphPos.x+ EXCEL_GRAPH_TITLE_ROWS, m_GraphPos.y, GRAPH_WIDTH, GRAPH_HEIGHT, m_NewSheetName);
		m_pXL->SetYAxisPosition();
		m_pXL->SetLegendPosWithHeight(XlLegendPosition::xlLegendPositionTop, GRAPH_LEGEND_WIDTH, GRAPH_LEGEND_HEIGHT);
		m_pXL->SetPanelPosMargin(XlDirection::xlToLeft, 0);

		m_GraphPos.y += GRAPH_COLS+1;
		m_TablePos.y += m_TableSize.y+1;

		return TRUE;
	}
	catch (const std::exception&)
	{
		ASSERT(0);
		return FALSE;
	}
}

CString CSeisEvalGraphBase::GetCellOrgExp(long Row, long Col)
{
	CString sCol, sRow;

	sRow.Format(_T("%d"), Row + 1);

	if (Col < 26)
	{
		sCol.Format(_T("%c"), (TCHAR)('A' + Col));
	}
	else if (Col < 255)
	{
		long first, second;
		first = Col / 26 - 1;
		second = Col % 26;
		sCol.Format(_T("%c%c"), (TCHAR)('A' + first), (TCHAR)('A' + second));
	}

	CString sCell = _T("$") + sCol + _T("$") + sRow;

	return sCell;
}

void CSeisEvalGraphBase::WriteSeisDir(const int& iSeisDir, int& stRow)
{
	m_pXL->SetValue(stRow, m_GraphPos.y, iSeisDir == T_SIPA_D::kAxis_Dir ? _LS(IDS_BRDG_SEIS_EVAL_REPORT_JP_LONGI) : _LS(IDS_BRDG_SEIS_EVAL_REPORT_JP_TRANS));
	stRow++;
}

void CSeisEvalGraphBase::WriteSeisType(const int& iType, int& stRow)
{
	m_pXL->SetValue(stRow, m_GraphPos.y, iType == T_SIPA_D::kType1 ? _LS(IDS_BRDG_SEIS_EVAL_REPORT_JP_TYPE_I) : _LS(IDS_BRDG_SEIS_EVAL_REPORT_JP_TYPE_II));//タイプI / タイプII
	stRow++;
}

void CSeisEvalGraphBase::WriteSeisPos(const int& iPos, int& stRow)
{
	m_pXL->SetValue(stRow, m_GraphPos.y, iPos == D_MBTP_BEAM ? _LS(IDS_BRDG_SEIS_EVAL_REPORT_JP_RES_BEAM) : _LS(IDS_BRDG_SEIS_EVAL_REPORT_JP_RES_COLUMN));
	stRow++;
}

void CSeisEvalGraphBase::WriteReportDir(const int& nExamDir, const int& nType, int& stRow)
{
	CString strTypeY;
	CString strTypeZ;
	CString strDir;

	switch (nType)
	{
	case eType::kFlexure:
		strTypeY = _T("Mz");
		strTypeZ = _T("My");
		break;
	case eType::kFlexure_Phi:
		strTypeY = _T("φz");
		strTypeZ = _T("φy");
		break;
	case eType::kShear:
		strTypeY = _T("Fy");
		strTypeZ = _T("Fz");
		break;
		break;
	case eType::kALL:
		strTypeY = _T("Mz, Fy");
		strTypeZ = _T("My, Fz");
	default:
		break;
	}

	switch (nExamDir)
	{
	case seis_jp::kY_Dir:
		strDir = strTypeY;
		break;
	case seis_jp::kZ_Dir:
		strDir = strTypeZ;
		break;
	default:
		ASSERT(FALSE);
		break;
	}

	m_pXL->SetValue(stRow, m_GraphPos.y, strDir);
	stRow++;
}

#pragma endregion CSeisEvalGraphBase

#pragma region Graph Control

CGraphControl::CGraphControl(CString strPath)
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();

	T_SIDC_D dSidc;
	dSidc.Initialize();
	pDoc->m_pAttrCtrl2->GetQSidc()->Get(dSidc);
	m_nDesignCode = dSidc.nDesignCode;

	m_pXL= new mit::frx::CMSExcel;
	m_strPath = strPath;
}

CGraphControl::~CGraphControl()
{
	for (size_t i = 0; i < m_aGraph.size(); i++)
	{
		if (m_aGraph[i] != NULL)
		{
			delete m_aGraph[i];
			m_aGraph[i] = NULL;
		}
	}
	m_aGraph.clear();

	if (m_pXL != nullptr)
	{
		delete m_pXL;
		m_pXL = nullptr;
	}
}

void CGraphControl::CreateGraphControl(CDgnProgressDlg* pProgress)
{
	CString SheetBaseName = _T("Graph_Sheet");
	m_pProgress = pProgress;
	switch (m_nDesignCode)
	{
	case BrdgSE_CodeJP::JROAD5_H14:
	{
		// Curvature RC
		CSeisEvalGraphBase* pGraphCurvatureRC = new CGraphCurvatureRC(m_pXL, pProgress, SheetBaseName, m_nDesignCode);
		CGraphShearPS* pGraphShearPS = new CGraphShearPS(m_pXL, pProgress, SheetBaseName, m_nDesignCode);
		m_aGraph.push_back(pGraphCurvatureRC);
		m_aGraph.push_back(pGraphShearPS);
	}
		break;
	case BrdgSE_CodeJP::JROAD5_H24:
	{
		// Curvature RC
		CSeisEvalGraphBase* pGraphCurvatureRC = new CGraphCurvatureRC(m_pXL, pProgress, SheetBaseName, m_nDesignCode);
		CGraphShearPS* pGraphShearPS = new CGraphShearPS(m_pXL, pProgress, SheetBaseName, m_nDesignCode);
		m_aGraph.push_back(pGraphCurvatureRC);
		m_aGraph.push_back(pGraphShearPS);
	}
		break;
	case BrdgSE_CodeJP::JROAD5_H29:
	{
		// Curvature RC
		CSeisEvalGraphBase* pGraphCurvatureRC = new CGraphCurvatureRC(m_pXL, pProgress, SheetBaseName, m_nDesignCode);
		CGraphShearPS* pGraphShearPS = new CGraphShearPS(m_pXL, pProgress, SheetBaseName, m_nDesignCode);
		m_aGraph.push_back(pGraphCurvatureRC);
		m_aGraph.push_back(pGraphShearPS);
	}
		break;
	default:
		break;
	}
}

void CGraphControl::WriteGraph()
{
	auto l_delete_XL = [&]()
		{
			if (m_pXL != nullptr)
			{
				delete m_pXL;
				m_pXL = nullptr;
			}
		};

	try
	{
		m_pXL->Open(m_strPath);

		int prevSheetNum = 0;
		POINT graphPos, tablePos;
		for (size_t i = 0; i < m_aGraph.size(); i++)
		{
			if (m_aGraph[i] != NULL)
			{
				// Set Previous Position
				if (prevSheetNum != 0)
				{
					m_aGraph[i]->SetCurrentSheetNum(prevSheetNum);
					m_aGraph[i]->SetPosNextGraph(graphPos, tablePos);
				}

				// Write Graph
				m_aGraph[i]->Write();

				// Save Position
				if (m_aGraph[i]->IsHaveGraph())
				{
					prevSheetNum = m_aGraph[i]->GetCurrentSheetNum();
					m_aGraph[i]->GetPosPreGraph(graphPos, tablePos);
				}
			}
		}

		m_pXL->SaveAs(m_pXL->GetBookName(), m_strPath);
		Sleep(1000);

		m_pXL->Quit();
	}
	catch (BOOL bStop)
	{
		throw bStop;
		ASSERT(bStop == FALSE);
		l_delete_XL();
		return;
	}
}

void CGraphControl::AddStepGraph()
{
	//m_pProgress->m_ProgressBar.SetState(PBST_PAUSED);
	for (size_t i = 0; i < m_aGraph.size(); i++)
	{
		if (m_aGraph[i] != NULL)
		{
			m_aGraph[i]->AddStep();
		}
	}
}

BOOL CGraphControl::IsGraph()
{
	for (size_t i = 0; i < m_aGraph.size(); i++)
	{
		if (m_aGraph[i] != NULL)
		{
			if (m_aGraph[i]->IsHaveGraph())
			{
				return TRUE;
			}
		}
	}
	return FALSE;
}

BOOL CGraphControl::IsExcel()
{
	return m_pXL->IsExcel();
}

#pragma endregion Graph Control

#pragma region CGraphCurvature 곡률

#pragma region CGraphCurvatureRC
CGraphCurvatureRC::CGraphCurvatureRC(mit::frx::CMSExcel* pXL, CDgnProgressDlg* pProgress, const CString& SheetName, const UINT& nDesignCode)
	: CSeisEvalGraphBase(pXL, pProgress, SheetName, nDesignCode)
{
	//m_subType = T_SIGR_D::kLower;
	m_ChartTitle = _LS(IDS_BRDG_SEIS_EVAL_REPORT_JP_GRAPH_CURVATURE_CHART_TITLE);
	m_Xtitle = _LS(IDS_BRDG_SEIS_EVAL_REPORT_JP_GRAPH_CURVATURE_X_TITLE);
	m_Ytitle = _LS(IDS_BRDG_SEIS_EVAL_REPORT_JP_GRAPH_Y_TITLE);
}

CGraphCurvatureRC::~CGraphCurvatureRC()
{
}

void CGraphCurvatureRC::Write()
{
	if (!IsHaveGraph()) return;

	AddSheet();

	// Graph Data
	mit::frx::ChartData chart;
	chart.ChartType = xlXYScatterLines;
	chart.PlotBy = xlColumns;
	chart.color = xlNone;
	chart.xTitle = m_Xtitle;
	chart.yTitle = m_Ytitle;
	chart.bLegend = TRUE;
	chart.LegendPosition = XlLegendPosition::xlLegendPositionTop; // 범례 위치
	chart.bXMajorGridLine = TRUE;
	chart.bXMinorGridLine = TRUE;
	chart.bYMajorGridLine = TRUE;
	chart.bYMinorGridLine = TRUE;
	chart.bMutiParam = TRUE;

	int MaxTableRow = 0;
	BOOL bGraphTitle = FALSE;
	// Eval Phi Y
	for (auto map : m_GraphEval_Phi_Y)
	{
		// Table Data
		CheckSheetSize();
		if (m_TablePos.x == EXCEL_TALBE_START_ROW) MaxTableRow = 0;
		int tableStartRow = m_TablePos.x;
		int tableStartCOl = m_TablePos.y;
		int tableEndRow = tableStartRow;
		int tableEndCOl = tableStartCOl;
		const int nSeisDir = std::get<0>(map.first);
		const int nSubType = std::get<1>(map.first);
		const int nSheerDir = std::get<2>(map.first);
		const CString strGroupName = std::get<3>(map.first);

		// TItle
		int nGraphRow = m_GraphPos.x;
		if (!bGraphTitle) WriteGraphTitle(nGraphRow); bGraphTitle = TRUE;
		WriteSeisDir(nSeisDir, nGraphRow);
		WriteSeisType(nSubType, nGraphRow);
		WriteReportDir(nSheerDir, eType::kFlexure_Phi, nGraphRow);
		m_pXL->SetValue(nGraphRow++, m_GraphPos.y, strGroupName);

		double nXMin = MAX_NUM, nXMax = MIN_NUM, nYMin = MAX_NUM, nYMax = MIN_NUM;
		for (auto eval : map.second)
		{
			T_SIBD_EVAL_PHI_Y evalY = eval;
			tableEndCOl = tableStartCOl;
			m_pXL->SetValue(tableEndRow, tableEndCOl++, evalY.kElem);
			m_pXL->SetValue(tableEndRow, tableEndCOl++, evalY.analDeform.dAvergeMin);
			m_pXL->SetValue(tableEndRow, tableEndCOl++, evalY.analDeform.dAvergeMax);
			m_pXL->SetValue(tableEndRow, tableEndCOl++, evalY.calcDeformMin.dPhi_y);
			m_pXL->SetValue(tableEndRow, tableEndCOl++, evalY.calcDeformMax.dPhi_y);
			m_pXL->SetValue(tableEndRow, tableEndCOl++, evalY.calcDeformMin.dPhi_u);
			m_pXL->SetValue(tableEndRow, tableEndCOl, evalY.calcDeformMax.dPhi_u);
			nXMin = min(nXMin, evalY.calcDeformMin.dPhi_u);
			nXMax = max(nXMax, evalY.calcDeformMax.dPhi_u);
			nYMin = min(nYMin, evalY.kElem);
			nYMax = max(nYMax, evalY.kElem);
			tableEndRow++;
			if (m_pProgress->NextStep() == FALSE) throw FALSE;
		}
		tableEndRow--;

		//double margin = (nXMax - nXMin) * 0.1;
		//chart.dXmin = std::floor(nXMin - margin);
		//chart.dXmax = std::ceil(nXMax + margin);
		chart.dYmin = nYMin;
		chart.dYmax = nYMax;
		chart.SourceData = m_pXL->GetCellOrgExp(tableStartRow, tableStartCOl) + _T(":") + m_pXL->GetCellOrgExp(tableEndRow, tableEndCOl);

		// Y Value (공통)
		CString strYValue;
		strYValue.Format(_T("\'%s\'!%s:%s"), m_NewSheetName, GetCellOrgExp(tableStartRow, tableStartCOl), GetCellOrgExp(tableEndRow, tableStartCOl ));

		chart.aName.RemoveAll();
		chart.aXValue.RemoveAll();
		chart.aYValue.RemoveAll();
		for (int i = tableStartCOl+1; i <= tableEndCOl; i++)
		{
			CString strLegend;
			SetLegendName(eType::kFlexure_Phi, i - tableStartCOl, strLegend);
			chart.aName.Add(strLegend);
		
			CString strXValue = _T("");
			strXValue.Format(_T("\'%s\'!%s:%s"), m_NewSheetName, GetCellOrgExp(tableStartRow, i), GetCellOrgExp(tableEndRow, i));
			chart.aXValue.Add(strXValue);
			chart.aYValue.Add(strYValue);   //동일 Y
		}

		m_TableSize.x = tableEndRow - tableStartRow + 1;
		m_TableSize.y = tableEndCOl - tableStartCOl + 1;
		MaxTableRow = max(MaxTableRow, tableStartRow + m_TableSize.x);
		if(!WriteGraph(chart)) break;
	}

	// Eval Phi A
	for (auto map : m_GraphEval_Phi_A)
	{
		// Table Data
		CheckSheetSize();
		if (m_TablePos.x == EXCEL_TALBE_START_ROW) MaxTableRow = 0;
		int tableStartRow = m_TablePos.x;
		int tableStartCOl = m_TablePos.y;
		int tableEndRow = tableStartRow;
		int tableEndCOl = tableStartCOl;
		const int nSeisDir = std::get<0>(map.first);
		const int nSubType = std::get<1>(map.first);
		const int nSheerDir = std::get<2>(map.first);
		const CString strGroupName = std::get<3>(map.first);

		// TItle
		int nGraphRow = m_GraphPos.x;
		if (!bGraphTitle) WriteGraphTitle(nGraphRow); bGraphTitle = TRUE;
		WriteSeisDir(nSeisDir, nGraphRow);
		WriteSeisType(nSubType, nGraphRow);
		WriteReportDir(nSheerDir, eType::kFlexure_Phi, nGraphRow);
		m_pXL->SetValue(nGraphRow++, m_GraphPos.y, strGroupName);

		double nXMin = MAX_NUM, nXMax = MIN_NUM, nYMin = MAX_NUM, nYMax = MIN_NUM;
		for (auto eval : map.second)
		{
			T_SIBD_EVAL_PHI_A evalA = eval;
			tableEndCOl = tableStartCOl;
			m_pXL->SetValue(tableEndRow, tableEndCOl++, evalA.kElem);
			m_pXL->SetValue(tableEndRow, tableEndCOl++, evalA.analDeform.dAvergeMin);
			m_pXL->SetValue(tableEndRow, tableEndCOl++, evalA.analDeform.dAvergeMax);
			m_pXL->SetValue(tableEndRow, tableEndCOl++, evalA.calcDeformMin.dPhi_u);
			m_pXL->SetValue(tableEndRow, tableEndCOl++, evalA.calcDeformMax.dPhi_u);
			m_pXL->SetValue(tableEndRow, tableEndCOl++, evalA.calcDeformMin.dPhi_a);
			m_pXL->SetValue(tableEndRow, tableEndCOl, evalA.calcDeformMax.dPhi_a);
			nXMin = min(nXMin, evalA.calcDeformMin.dPhi_a);
			nXMax = max(nXMax, evalA.calcDeformMax.dPhi_a);
			nYMin = min(nYMin, evalA.kElem);
			nYMax = max(nYMax, evalA.kElem);
			tableEndRow++;
			if (m_pProgress->NextStep() == FALSE) throw FALSE;
		}
		tableEndRow--;

		//chart.ChartTitle = strGroupName;
		//double margin = (nXMax - nXMin) * 0.1;
		//chart.dXmin = std::floor(nXMin - margin);
		//chart.dXmax = std::ceil(nXMax + margin);
		chart.dYmin = nYMin;
		chart.dYmax = nYMax;
		chart.SourceData = m_pXL->GetCellOrgExp(tableStartRow, tableStartCOl) + _T(":") + m_pXL->GetCellOrgExp(tableEndRow, tableEndCOl);

		// Y Value (공통)
		CString strYValue;
		strYValue.Format(_T("\'%s\'!%s:%s"), m_NewSheetName, GetCellOrgExp(tableStartRow, tableStartCOl), GetCellOrgExp(tableEndRow, tableStartCOl));

		chart.aName.RemoveAll();
		chart.aXValue.RemoveAll();
		chart.aYValue.RemoveAll();
		for (int i = tableStartCOl + 1; i <= tableEndCOl; i++)
		{
			CString strLegend;
			SetLegendName(eType::kFlexure_Phi, i - tableStartCOl, strLegend);
			chart.aName.Add(strLegend);

			CString strXValue = _T("");
			strXValue.Format(_T("\'%s\'!%s:%s"), m_NewSheetName, GetCellOrgExp(tableStartRow, i), GetCellOrgExp(tableEndRow, i));
			chart.aXValue.Add(strXValue);
			chart.aYValue.Add(strYValue);   // 동일 Y
		}

		m_TableSize.x = tableEndRow - tableStartRow + 1;
		m_TableSize.y = tableEndCOl - tableStartCOl + 1;
		MaxTableRow = max(MaxTableRow, tableStartRow + m_TableSize.x);
		if (!WriteGraph(chart)) break;
	}

	// next pos
	m_TablePos.x = MaxTableRow;
}

void CGraphCurvatureRC::SetDefaultMutiParam(mit::frx::ChartData& chart)
{
	chart.aMutiLine.RemoveAll();
	chart.aMutiMarker.RemoveAll();

	for(int i=1; i<=6; i++)
	{
		mit::frx::ChartData::sXlMarker marker;
		mit::frx::ChartData::sXlLine line;

		switch (i)
		{
		case 1:
		case 2:
		{
			// line
			auto pColor = i == 1 ? (long)RGB(0, 200, 200) : (long)RGB(64, 128, 128);
			line.LineStyle.lVal = XlLineStyle::xlLineStyleNone;
			line.Weight.lVal = XlBorderWeight::xlThin;
			line.color.lVal = pColor;

			// marker
			marker.MakerStyle = XlMarkerStyle::xlMarkerStyleCircle;
			marker.nSize = 2;
			marker.color = pColor;
		}
			break;
		case 3:
		case 4:
		case 5:
		case 6:
		{
			// line
			auto pColor = (i == 5 || i == 6) ? (long)RGB(255, 150, 0) : (long)RGB(0, 150, 0); 
			line.LineStyle.lVal = XlLineStyle::xlDash;
			line.Weight.lVal = XlBorderWeight::xlThin;
			line.color.lVal = pColor;

			// marker
			marker.MakerStyle = XlMarkerStyle::xlMarkerStyleNone;
			marker.nSize = 0;
			marker.color = pColor;
		}
			break;
		default:
			break;
		}

		chart.aMutiLine.Add(line);
		chart.aMutiMarker.Add(marker);
	}
}

void CGraphCurvatureRC::AddStep()
{
	std::map<std::tuple<int, int, int, int>, std::vector<T_SIBD_K>> mExistSibd;
	if (!IsExistData(mExistSibd)) return;;

	for (const auto& pair : mExistSibd)
	{
		const auto& key = pair.first;
		const auto& vec = pair.second;
		if (vec.size() == 0) continue;

		const int iCond = std::get<0>(key);
		const int iSeisDir = std::get<1>(key);
		const int iType = std::get<2>(key);
		const int iMember = std::get<3>(key);

		for (const auto& SibdK : vec)
		{
			T_SIBD_D SibdD;
			if (!m_pDoc->m_pAttrCtrl2->GetQSibd()->Get(SibdK, SibdD))		continue;

			T_SIGR_D SigrD;
			if (!m_pDoc->m_pAttrCtrl2->GetQSigr()->Get(SibdD.kSigr, SigrD)) continue;
			//if (SigrD.nSubType != this->m_subType) continue;

			T_ELEM_K_LIST aElemKeys; std::map <T_ELEM_K, std::tuple<CString, int>> mElemInfo;
			GetTargetElements(SibdD.kSigr, (T_SIGR_D::eSubType)SigrD.nSubType, T_SIGR_LOWER_D::kRC, iMember, aElemKeys, mElemInfo);
			if (aElemKeys.GetCount() < 1) continue;

			const int nMemb = aElemKeys.GetSize();
			for (int k = 0; k < nMemb; ++k)
			{
				const T_ELEM_K& ElemK = aElemKeys[k];
				if (mElemInfo.find(ElemK) == mElemInfo.end()) continue;
				//const int nShearDir = std::get<1>(mElemInfo[ElemK]);
				const CString strGroupName = (mElemInfo.find(ElemK) == mElemInfo.end()) ? _T("--") : std::get<0>(mElemInfo[ElemK]);

				for (int d = 0; d < 2; d++)
				{
					int nDir = d == 0 ? seis_jp::kY_Dir : seis_jp::kZ_Dir;
					if (iCond == T_SIBD_CVSV_D::kSurrenderSurvey)
					{
						T_SIBD_EVAL_PHI_Y eval;
						eval.kSipa = SibdD.kSipa;
						eval.kSigr = SibdD.kSigr;
						eval.kElem = ElemK;
						eval.nExamShearDir = nDir;
						if (!m_pResult->GetResultSibdPhiY(eval))  continue;
						m_pProgress->m_nTotalSteps++;
						m_GraphEval_Phi_Y[std::make_tuple(iSeisDir, SigrD.nSubType, nDir, strGroupName)].push_back(eval);
						m_bGraph = TRUE;
					}
					else if (iCond == T_SIBD_CVSV_D::kLimitStateSurvey)
					{
						T_SIBD_EVAL_PHI_A eval;
						eval.kSipa = SibdD.kSipa;
						eval.kSigr = SibdD.kSigr;
						eval.kElem = ElemK;
						eval.nExamShearDir = nDir;
						if (!m_pResult->GetResultSibdPhiA(eval))  continue;
						m_pProgress->m_nTotalSteps++;
						m_GraphEval_Phi_A[std::make_tuple(iSeisDir, SigrD.nSubType, nDir, strGroupName)].push_back(eval);
						m_bGraph = TRUE;
					}
				}
			}
		}
	}

}

BOOL CGraphCurvatureRC::IsExistData(std::map<std::tuple<int, int, int, int>, std::vector<T_SIBD_K>>& mExistSibd)
{
	T_KEY_LIST aSibdK;
	m_pDoc->m_pAttrCtrl2->GetQSibd()->GetKeyList(aSibdK);
	if (aSibdK.GetSize() < 1) return FALSE;

	for (int i = 0; i < aSibdK.GetSize(); ++i)
	{
		const T_SIBD_K SibdK = aSibdK[i];
		T_SIBD_D SibdD;
		if (!m_pDoc->m_pAttrCtrl2->GetQSibd()->Get(SibdK, SibdD))		continue;
		if (SibdD.nSibdType != T_SIBD_D::kCurvatureSurvey) continue;

		T_SIGR_D SigrD;
		if (!m_pDoc->m_pAttrCtrl2->GetQSigr()->Get(SibdD.kSigr, SigrD)) continue;
		//if (SigrD.nSubType != this->m_subType)								continue;

		T_SIPA_D SipaD;
		if (!m_pDoc->m_pAttrCtrl2->GetQSipa()->Get(SibdD.kSipa, SipaD)) continue;
		if (SipaD.nDesignCode != m_nDesignCode)							continue;

		const auto nSurveyCondition = (int)SibdD.CVSV_D.nCvsvType; // 0: 항복조사, 1: 한계상태조사
		const auto nSeisDir = (int)SipaD.nSeismicForceDirection;
		const auto nSeisForce = (int)SipaD.nSeismicForceType;
		T_SIGR_LOWER_D SigrLowerD;
		if (m_pDoc->m_pAttrCtrl2->GetQSigrLower()->Get(SibdD.kSigr, SigrLowerD))
		{
			if (SigrLowerD.nMaterialType == T_SIGR_LOWER_D::kRC)
			{
				if (SigrLowerD.BEAM_D.GetSize() > 0)
					mExistSibd[std::make_tuple(nSurveyCondition, nSeisDir, nSeisForce, D_MBTP_BEAM)].push_back(SibdK);
				if (SigrLowerD.COLM_D.GetSize() > 0)
					mExistSibd[std::make_tuple(nSurveyCondition, nSeisDir, nSeisForce, D_MBTP_COLUMN)].push_back(SibdK);
			}
		}
		else
		{
			if (SigrD.nSubType == T_SIGR_D::kUpper || SigrD.nSubType == T_SIGR_D::kUser)
				mExistSibd[std::make_tuple(nSurveyCondition, nSeisDir, nSeisForce, D_MBTP_BEAM)].push_back(SibdK);
		}
	}

	if (mExistSibd.size() < 1) return FALSE;
	else return TRUE;
}
#pragma endregion CGraphCurvatureRC

#pragma region CGraphCurvatureST
CGraphCurvatureST::CGraphCurvatureST(mit::frx::CMSExcel* pXL, CDgnProgressDlg* pProgress, const CString& SheetName, const UINT& nDesignCode)
	: CSeisEvalGraphBase(pXL, pProgress, SheetName, nDesignCode)
{
	//m_subType = T_SIGR_D::kLower;
	m_ChartTitle = _LS(IDS_BRDG_SEIS_EVAL_REPORT_JP_GRAPH_CURVATURE_CHART_TITLE);
	m_Xtitle = _LS(IDS_BRDG_SEIS_EVAL_REPORT_JP_GRAPH_CURVATURE_X_TITLE);
	m_Ytitle = _LS(IDS_BRDG_SEIS_EVAL_REPORT_JP_GRAPH_Y_TITLE);
}

CGraphCurvatureST::~CGraphCurvatureST()
{
}

void CGraphCurvatureST::Write()
{
	if (!IsHaveGraph()) return;
}

void CGraphCurvatureST::SetDefaultMutiParam(mit::frx::ChartData& chart)
{
	chart.aMutiLine.RemoveAll();
	chart.aMutiMarker.RemoveAll();
}

void CGraphCurvatureST::AddStep()
{
	std::map<std::tuple<int, int, int, int>, std::vector<T_SIBD_K>> mExistSibd;
	if (!IsExistData(mExistSibd)) return;

	for (const auto& pair : mExistSibd)
	{
		const auto& key = pair.first;
		const auto& vec = pair.second;
		if (vec.size() == 0) continue;

		const int iCond = std::get<0>(key);
		const int iSeisDir = std::get<1>(key);
		const int iType = std::get<2>(key);
		const int iMember = std::get<3>(key);

		for (const auto& SibdK : vec)
		{
			T_SIBD_D SibdD;
			if (!m_pDoc->m_pAttrCtrl2->GetQSibd()->Get(SibdK, SibdD))		continue;

			T_SIGR_D SigrD;
			if (!m_pDoc->m_pAttrCtrl2->GetQSigr()->Get(SibdD.kSigr, SigrD)) continue;
			//if (SigrD.nSubType != this->m_subType) continue;

			T_ELEM_K_LIST aElemKeys; std::map <T_ELEM_K, std::tuple<CString, int>> mElemInfo;
			GetTargetElements(SibdD.kSigr, (T_SIGR_D::eSubType)SigrD.nSubType, T_SIGR_LOWER_D::kSteel, iMember, aElemKeys, mElemInfo);
			if (aElemKeys.GetCount() < 1) continue;

			const int nMemb = aElemKeys.GetSize();
			for (int k = 0; k < nMemb; ++k)
			{
				const T_ELEM_K& ElemK = aElemKeys[k];
				if (mElemInfo.find(ElemK) == mElemInfo.end()) continue;
				//const int nShearDir = std::get<1>(mElemInfo[ElemK]);
				const CString strGroupName = (mElemInfo.find(ElemK) == mElemInfo.end()) ? _T("--") : std::get<0>(mElemInfo[ElemK]);

				for (int d = 0; d < 2; d++)
				{
					int nDir = d == 0 ? seis_jp::kY_Dir : seis_jp::kZ_Dir;
					if (iCond == T_SIBD_CVSV_D::kSurrenderSurvey)
					{
						T_SIBD_EVAL_PHI_Y_ST eval;
						eval.kSipa = SibdD.kSipa;
						eval.kSigr = SibdD.kSigr;
						eval.kElem = ElemK;
						eval.nExamShearDir = nDir;
						if (!m_pResult->GetResultSibdPhiY_ST(eval))  continue;
						m_pProgress->m_nTotalSteps++;
						m_GraphEval_Phi_Y[std::make_tuple(iSeisDir, SigrD.nSubType, nDir, strGroupName)].push_back(eval);
						m_bGraph = TRUE;
					}
					else if (iCond == T_SIBD_CVSV_D::kLimitStateSurvey)
					{
						T_SIBD_EVAL_PHI_A_ST eval;
						eval.kSipa = SibdD.kSipa;
						eval.kSigr = SibdD.kSigr;
						eval.kElem = ElemK;
						eval.nExamShearDir = nDir;
						if (!m_pResult->GetResultSibdPhiA_ST(eval))  continue;
						m_pProgress->m_nTotalSteps++;
						m_GraphEval_Phi_A[std::make_tuple(iSeisDir, SigrD.nSubType, nDir, strGroupName)].push_back(eval);
						m_bGraph = TRUE;
					}
				}
			}
		}
	}
}

BOOL CGraphCurvatureST::IsExistData(std::map<std::tuple<int, int, int, int>, std::vector<T_SIBD_K>>& mExistSibd)
{
	T_KEY_LIST aSibdK;
	m_pDoc->m_pAttrCtrl2->GetQSibd()->GetKeyList(aSibdK);
	if (aSibdK.GetSize() < 1) return FALSE;

	for (int i = 0; i < aSibdK.GetSize(); ++i)
	{
		const T_SIBD_K SibdK = aSibdK[i];
		T_SIBD_D SibdD;
		if (!m_pDoc->m_pAttrCtrl2->GetQSibd()->Get(SibdK, SibdD))		continue;
		if (SibdD.nSibdType != T_SIBD_D::kCurvatureSurvey) continue;

		T_SIGR_D SigrD;
		if (!m_pDoc->m_pAttrCtrl2->GetQSigr()->Get(SibdD.kSigr, SigrD)) continue;
		//if (SigrD.nSubType != this->m_subType)								continue;

		T_SIPA_D SipaD;
		if (!m_pDoc->m_pAttrCtrl2->GetQSipa()->Get(SibdD.kSipa, SipaD)) continue;
		if (SipaD.nDesignCode != m_nDesignCode)							continue;

		const auto nSurveyCondition = (int)SibdD.CVSV_D.nCvsvType; // 0: 항복조사, 1: 한계상태조사
		const auto nSeisDir = (int)SipaD.nSeismicForceDirection;
		const auto nSeisForce = (int)SipaD.nSeismicForceType;
		T_SIGR_LOWER_D SigrLowerD;
		if (m_pDoc->m_pAttrCtrl2->GetQSigrLower()->Get(SibdD.kSigr, SigrLowerD))
		{
			if (SigrLowerD.nMaterialType == T_SIGR_LOWER_D::kSteel)
			{
				if (SigrLowerD.BEAM_D.GetSize() > 0)
					mExistSibd[std::make_tuple(nSurveyCondition, nSeisDir, nSeisForce, D_MBTP_BEAM)].push_back(SibdK);
				if (SigrLowerD.COLM_D.GetSize() > 0)
					mExistSibd[std::make_tuple(nSurveyCondition, nSeisDir, nSeisForce, D_MBTP_COLUMN)].push_back(SibdK);
			}
		}
		else
		{
			if (SigrD.nSubType == T_SIGR_D::kUpper || SigrD.nSubType == T_SIGR_D::kUser)
				mExistSibd[std::make_tuple(nSurveyCondition, nSeisDir, nSeisForce, D_MBTP_BEAM)].push_back(SibdK);
		}
	}

	if (mExistSibd.size() < 1) return FALSE;
	else return TRUE;
}
#pragma endregion CGraphCurvatureST

#pragma endregion CGraphCurvature 곡률

#pragma region CGraphShear 전단

CGraphShearBase::CGraphShearBase(mit::frx::CMSExcel* pXL, CDgnProgressDlg* pProgress, const CString& SheetName, const UINT& nDesignCode)
	: CSeisEvalGraphBase(pXL, pProgress, SheetName, nDesignCode)
{
	//m_subType = T_SIGR_D::kLower;
	m_ChartTitle = _LS(IDS_BRDG_SEIS_EVAL_REPORT_JP_GRAPH_SHEAR_CHART_TITLE);
	m_Xtitle = _LS(IDS_BRDG_SEIS_EVAL_REPORT_JP_GRAPH_SHEAR_X_TITLE);
	m_Ytitle = _LS(IDS_BRDG_SEIS_EVAL_REPORT_JP_GRAPH_Y_TITLE);
}

CGraphShearBase::~CGraphShearBase()
{
}

BOOL CGraphShearBase::IsExistData(std::map<std::tuple<int, int, int, int>, std::vector<T_SISR_K>>& mExistSisr)
{
	// Cal for Shear
	CArray<T_SISR_K, T_SISR_K> aSisrK;
	m_pDoc->m_pAttrCtrl2->GetQSisr()->GetKeyList(aSisrK);
	const int nSisr = aSisrK.GetSize();
	if (nSisr == 0) return FALSE;

	for (int i = 0; i < nSisr; ++i)
	{
		const T_SISR_K SisrK = aSisrK[i];
		T_SISR_D SisrD;
		if (!m_pDoc->m_pAttrCtrl2->GetQSisr()->Get(SisrK, SisrD))		continue;
		T_SIGR_D SigrD;
		if (!m_pDoc->m_pAttrCtrl2->GetQSigr()->Get(SisrD.kSigr, SigrD)) continue;
		//if (SigrD.nSubType != this->m_subType)								continue;
		T_SIPA_D SipaD;
		if (!m_pDoc->m_pAttrCtrl2->GetQSipa()->Get(SisrD.kSipa, SipaD)) continue;
		if (SipaD.nDesignCode != m_nDesignCode)							continue;

		const auto nEarthquakeType = (int)SipaD.nEarthquakeType;
		const auto nSeisDir = (int)SipaD.nSeismicForceDirection;
		const auto nSeisForce = (int)SipaD.nSeismicForceType;
		T_SIGR_LOWER_D SigrLowerD;
		if (m_pDoc->m_pAttrCtrl2->GetQSigrLower()->Get(SisrD.kSigr, SigrLowerD))
		{
			if (SigrLowerD.nMaterialType == T_SIGR_LOWER_D::kRC)
			{
				if (SigrLowerD.BEAM_D.GetSize() > 0)
					mExistSisr[std::make_tuple(nEarthquakeType, nSeisDir, nSeisForce, D_MBTP_BEAM)].push_back(SisrK);
				if (SigrLowerD.COLM_D.GetSize() > 0)
					mExistSisr[std::make_tuple(nEarthquakeType, nSeisDir, nSeisForce, D_MBTP_COLUMN)].push_back(SisrK);
			}
		}
		else
		{
			if (SigrD.nSubType == T_SIGR_D::kUpper || SigrD.nSubType == T_SIGR_D::kUser)
				mExistSisr[std::make_tuple(nEarthquakeType, nSeisDir, nSeisForce, D_MBTP_BEAM)].push_back(SisrK);
		}
	}

	if (mExistSisr.size() < 1) return FALSE;
	else return TRUE;
}

#pragma region CGraphShearPS
CGraphShearPS::CGraphShearPS(mit::frx::CMSExcel* pXL, CDgnProgressDlg* pProgress, const CString& SheetName, const UINT& nDesignCode)
	: CGraphShearBase(pXL, pProgress, SheetName, nDesignCode)
{
}

CGraphShearPS::~CGraphShearPS()
{
}

void CGraphShearPS::Write()
{
	if (!IsHaveGraph()) return;

	AddSheet();

	// Graph Data
	mit::frx::ChartData chart;
	chart.ChartType = xlXYScatterLines;
	chart.PlotBy = xlColumns;
	chart.color = xlNone;
	chart.xTitle = m_Xtitle;
	chart.yTitle = m_Ytitle;
	chart.bLegend = TRUE;
	chart.LegendPosition = XlLegendPosition::xlLegendPositionTop; // 범례 위치
	chart.bXMajorGridLine = TRUE;
	chart.bXMinorGridLine = TRUE;
	chart.bYMajorGridLine = TRUE;
	chart.bYMinorGridLine = TRUE;
	chart.bMutiParam = TRUE;

	int MaxTableRow = 0;
	BOOL bGraphTitle = FALSE;
	// Eval Phi Y
	for (auto map : m_GraphEvalLower)
	{
		// Table Data
		CheckSheetSize();
		if(m_TablePos.x == EXCEL_TALBE_START_ROW) MaxTableRow = 0;
		int tableStartRow = m_TablePos.x;
		int tableStartCOl = m_TablePos.y;
		int tableEndRow = tableStartRow;
		int tableEndCOl = tableStartCOl;
		const int nSeisDir = std::get<0>(map.first);
		const int nSubType = std::get<1>(map.first);
		const int nSheerDir = std::get<2>(map.first);
		const CString strGroupName = std::get<3>(map.first);

		// TItle
		int nGraphRow = m_GraphPos.x;
		if (!bGraphTitle) WriteGraphTitle(nGraphRow); bGraphTitle = TRUE;
		WriteSeisDir(nSeisDir, nGraphRow);
		WriteSeisType(nSubType, nGraphRow);
		WriteReportDir(nSheerDir, eType::kShear, nGraphRow);
		m_pXL->SetValue(nGraphRow++, m_GraphPos.y, strGroupName);

		double nXMin = MAX_NUM, nXMax = MIN_NUM, nYMin = MAX_NUM, nYMax = MIN_NUM;
		for (auto eval : map.second)
		{
			T_SISR_EVAL_LOWER evalD = eval;
			tableEndCOl = tableStartCOl;
			// I Node
			m_pXL->SetValue(tableEndRow, tableEndCOl, evalD.kElem);
			m_pXL->SetValue(tableEndRow, tableEndCOl + 1, evalD.PosI.RsltMin.dShear);
			m_pXL->SetValue(tableEndRow, tableEndCOl + 2, evalD.PosI.RsltMax.dShear);
			m_pXL->SetValue(tableEndRow, tableEndCOl + 3, evalD.PosI.RsltMin.dPs);
			m_pXL->SetValue(tableEndRow, tableEndCOl + 4, evalD.PosI.RsltMax.dPs);

			// J Node
			m_pXL->SetValue(tableEndRow+1, tableEndCOl, evalD.kElem);
			m_pXL->SetValue(tableEndRow + 1, tableEndCOl + 1, evalD.PosJ.RsltMin.dShear);
			m_pXL->SetValue(tableEndRow + 1, tableEndCOl + 2, evalD.PosJ.RsltMax.dShear);
			m_pXL->SetValue(tableEndRow + 1, tableEndCOl + 3, evalD.PosJ.RsltMin.dPs);
			m_pXL->SetValue(tableEndRow + 1, tableEndCOl + 4, evalD.PosJ.RsltMax.dPs);

			nXMin = min(nXMin, min(evalD.PosI.RsltMin.dPs, evalD.PosJ.RsltMin.dPs));
			nXMax = max(nXMax, (evalD.PosI.RsltMax.dPs, evalD.PosJ.RsltMax.dPs));
			nYMin = min(nYMin, evalD.kElem);
			nYMax = max(nYMax, evalD.kElem);
			tableEndRow += 2;
			tableEndCOl += 4;
			if (m_pProgress->NextStep() == FALSE) throw FALSE;
		}
		tableEndRow -= 1;

		//double margin = (nXMax - nXMin) * 0.1;
		//chart.dXmin = std::floor(nXMin - margin);
		//chart.dXmax = std::ceil(nXMax + margin);
		chart.dYmin = nYMin;
		chart.dYmax = nYMax;
		chart.SourceData = m_pXL->GetCellOrgExp(tableStartRow, tableStartCOl) + _T(":") + m_pXL->GetCellOrgExp(tableEndRow, tableEndCOl);

		// Y Value (공통)
		CString strYValue;
		strYValue.Format(_T("\'%s\'!%s:%s"), m_NewSheetName, GetCellOrgExp(tableStartRow, tableStartCOl), GetCellOrgExp(tableEndRow, tableStartCOl));

		chart.aName.RemoveAll();
		chart.aXValue.RemoveAll();
		chart.aYValue.RemoveAll();
		for (int i = tableStartCOl + 1; i <= tableEndCOl; i++)
		{
			CString strLegend;
			SetLegendName(eType::kShear, i - tableStartCOl, strLegend);
			chart.aName.Add(strLegend);

			CString strXValue = _T("");
			strXValue.Format(_T("\'%s\'!%s:%s"), m_NewSheetName, GetCellOrgExp(tableStartRow, i), GetCellOrgExp(tableEndRow, i));
			chart.aXValue.Add(strXValue);
			chart.aYValue.Add(strYValue);   //동일 Y
		}

		m_TableSize.x = tableEndRow - tableStartRow + 1;
		m_TableSize.y = tableEndCOl - tableStartCOl + 1;
		MaxTableRow = max(MaxTableRow, tableStartRow + m_TableSize.x);
		if (!WriteGraph(chart)) break;
	}

	// next pos
	m_TablePos.x = MaxTableRow;
}

void CGraphShearPS::SetDefaultMutiParam(mit::frx::ChartData& chart)
{
	chart.aMutiLine.RemoveAll();
	chart.aMutiMarker.RemoveAll();

	for (int i = 1; i <= 4; i++)
	{
		mit::frx::ChartData::sXlMarker marker;
		mit::frx::ChartData::sXlLine line;

		switch (i)
		{
		case 1:
		case 2:
		{
			// line
			auto pColor = i == 1 ? (long)RGB(0, 200, 200) : (long)RGB(64, 128, 128);
			line.LineStyle.lVal = XlLineStyle::xlLineStyleNone;
			line.Weight.lVal = XlBorderWeight::xlThin;
			line.color.lVal = pColor;

			// marker
			marker.MakerStyle = XlMarkerStyle::xlMarkerStyleCircle;
			marker.nSize = 2;
			marker.color = pColor;
		}
		break;
		case 3:
		case 4:
		{
			// line
			auto pColor = (long)RGB(0, 150, 0);
			line.LineStyle.lVal = XlLineStyle::xlDash;
			line.Weight.lVal = XlBorderWeight::xlThin;
			line.color.lVal = pColor;

			// marker
			marker.MakerStyle = XlMarkerStyle::xlMarkerStyleNone;
			marker.nSize = 0;
			marker.color = pColor;
		}
		break;
		default:
			break;
		}

		chart.aMutiLine.Add(line);
		chart.aMutiMarker.Add(marker);
	}
}

void CGraphShearPS::AddStep()
{
	std::map<std::tuple<int, int, int, int>, std::vector<T_SISR_K>> mExistSisr;
	if (!IsExistData(mExistSisr)) return;


	for (const auto& pair : mExistSisr)
	{
		const auto& key = pair.first;
		const auto& vec = pair.second;
		if (vec.size() == 0) continue;

		const int iEathquake = std::get<0>(key);
		const int iSeisDir = std::get<1>(key);
		const int iType = std::get<2>(key);
		const int iMember = std::get<3>(key);

		for (const auto& SisrK : vec)
		{
			T_SISR_D SisrD;
			if (!m_pDoc->m_pAttrCtrl2->GetQSisr()->Get(SisrK, SisrD)) continue;

			T_SIGR_D SigrD;
			if (!m_pDoc->m_pAttrCtrl2->GetQSigr()->Get(SisrD.kSigr, SigrD)) continue;

			T_ELEM_K_LIST aElemKeys; std::map <T_ELEM_K, std::tuple<CString, int>> mElemInfo;
			GetTargetElements(SisrD.kSigr, (T_SIGR_D::eSubType)SigrD.nSubType, T_SIGR_LOWER_D::kRC, iMember, aElemKeys, mElemInfo);
			if (aElemKeys.GetCount() < 1) continue;

			const int nMemb = aElemKeys.GetSize();
			for (int k = 0; k < nMemb; ++k)
			{
				const T_ELEM_K ElemK = aElemKeys[k];
				if (mElemInfo.find(ElemK) == mElemInfo.end()) continue;
				//const int nShearDir = std::get<1>(mElemInfo[ElemK]);
				const CString strGroupName = (mElemInfo.find(ElemK) == mElemInfo.end()) ? _T("--") : std::get<0>(mElemInfo[ElemK]);

				for (int d = 0; d < 2; d++)
				{
					int nDir = d == 0 ? seis_jp::kY_Dir : seis_jp::kZ_Dir;
					T_SISR_EVAL_LOWER EvalD;
					EvalD.kSipa = SisrD.kSipa;
					EvalD.kSigr = SisrD.kSigr;
					EvalD.kElem = ElemK;
					EvalD.nExamShearDir = nDir;
					if (!m_pResult->GetResultSisrLower(EvalD)) continue;
					m_pProgress->m_nTotalSteps++;
					m_GraphEvalLower[std::make_tuple(iSeisDir, SigrD.nSubType, nDir, strGroupName)].push_back(EvalD);
					m_bGraph = TRUE;
				}
			}
		}
	}
}

#pragma endregion CGraphShearPS

#pragma endregion CGraphShear 전단