#include "stdafx.h"
#include "WriteSeisEvalReport_JP.h"
#include "WriteSeisEvalDisp_JP.h"

#include "..\wg_base\GColor.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\DB_ST_DN_ENUM.h"
#include "..\wg_db\wg_db_PostCtrl.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl2.h"
#include "..\wg_db\DB_QUERY_SEIS_CVL_JP.h"
#include "..\wg_db\BrdgSeisEvalResult_JP.h"

#include "DgnExcelPrintDlg.h"
#include "SeismicDesignSumm.h"

#define SURRENDER_COLOR		RGB(0,112,192)
#define BY_UNKNOWN -1
#define BY_WHOLE_MODEL 0
#define BY_PIER_MODEL 1

#define VERSION_965_DEFAULT 1

using namespace dgn::lib;

#pragma region // CWriteSeisEvalReportBase
CWriteSeisEvalReportBase::CWriteSeisEvalReportBase(IExcel* pXL, CDgnProgressDlg* pProgress, const CString& strBasePath, const CString& strBaseBook, const SDgnExcelPrintOpt* pPrintOpt) : CDgn_XLOut()
{
	m_pXL = pXL;
	m_pProgress = pProgress;
	m_strBasePath = strBasePath;
	m_strBaseBook = strBaseBook;
	m_pPrintOpt = pPrintOpt;

	CDBDoc* m_pDoc = CDBDoc::GetDocPoint();
	if (!m_pDoc) return;
	m_pResult = m_pDoc->m_pPostCtrl->GetBrdgSeisEvalResult_JP();

	m_nCode_Forc = 0;
	m_nCode_Leng = 0;
	m_dLenToM = m_pDoc->m_pUnitCtrl->ConvertCurUnitData2TgtUnit(D_UNITSYS_FORCE_INDEX_N, D_UNITSYS_LENGTH_INDEX_M, D_UNITSYS_BASE_LENGTH, 1.0);
	m_dLenToMM = m_pDoc->m_pUnitCtrl->ConvertCurUnitData2TgtUnit(D_UNITSYS_FORCE_INDEX_N, D_UNITSYS_LENGTH_INDEX_MM, D_UNITSYS_BASE_LENGTH, 1.0);
	m_dLenToCM = m_pDoc->m_pUnitCtrl->ConvertCurUnitData2TgtUnit(D_UNITSYS_FORCE_INDEX_N, D_UNITSYS_LENGTH_INDEX_CM, D_UNITSYS_BASE_LENGTH, 1.0);
	m_dStrToMPa = m_pDoc->m_pUnitCtrl->ConvertCurUnitData2TgtUnit(D_UNITSYS_FORCE_INDEX_N, D_UNITSYS_LENGTH_INDEX_MM, D_UNITSYS_BASE_STRESS, 1.0);

	T_SIDC_D dSidc;
	dSidc.Initialize();
	m_pDoc->m_pAttrCtrl2->GetQSidc()->Get(dSidc);
	m_nDesignCode = dSidc.nDesignCode;

	SetOutputUnitSystem();
	SetUnitConvertFactor();
}

CWriteSeisEvalReportBase::~CWriteSeisEvalReportBase()
{
}

void CWriteSeisEvalReportBase::SetNG_White(IExcel* pXL, ULONG iRow1, ULONG iCol1, ULONG iRow2, ULONG iCol2)
{
	// for Summary table
	pXL->SetTextColor(iRow1, iCol1, iRow2, iCol2, WHITE);
	pXL->SetPattern(iRow1, iCol1, iRow2, iCol2, libxl::FILLPATTERN_SOLID, false, true, HOT_BOLOGNESE, HOT_BOLOGNESE);
}

void CWriteSeisEvalReportBase::SetNG_Red(IExcel* pXL, ULONG iRow1, ULONG iCol1, ULONG iRow2, ULONG iCol2)
{
	// for table
	pXL->SetTextColor(iRow1, iCol1, iRow2, iCol2, CACODENON_RED);
	pXL->SetPattern(iRow1, iCol1, iRow2, iCol2, libxl::FILLPATTERN_SOLID, false, true, TINT_OF_ROSE, TINT_OF_ROSE);
}

void CWriteSeisEvalReportBase::GetTargetElements(const T_SISR_K& kSigr, const T_SIGR_D::eSubType& nGroupType, 
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

void CWriteSeisEvalReportBase::GetTargetElements(const T_SISR_K& kSigr, const T_SIGR_D::eSubType& nGroupType, 
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

				for (int j = 0; j < beam.aInvestigatedElem.GetCount(); j++)	{
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

CString CWriteSeisEvalReportBase::GetTextGroup(const T_SIGR_D::eSubType& nGroupType)
{
	CString str;
	switch (nGroupType)
	{
	case T_SIGR_D::eSubType::kUpper:
		str = _LS(IDS_BRDG_SEIS_EVAL_REPORT_JP_UPPER);// 上部
		break;
	case T_SIGR_D::eSubType::kLower:
		str = _LS(IDS_BRDG_SEIS_EVAL_REPORT_JP_LOWER);// 下部
		break;
	case T_SIGR_D::eSubType::kUser:
		str = _LS(IDS_BRDG_SEIS_EVAL_REPORT_JP_USER);// 使用者グループ
		break;
	default:
		break;
	}
	return str;
}

CString CWriteSeisEvalReportBase::GetTextTitle(const T_SIGR_D::eSubType& nGroupType)
{
	CString str;
	switch (nGroupType)
	{
	case T_SIGR_D::kUpper:
		str = _LS(IDS_BRDG_SEIS_EVAL_REPORT_JP_UPPER_STRUCT);// 上部構造の照査
		break;
	case T_SIGR_D::kLower:
		str = _LS(IDS_BRDG_SEIS_EVAL_REPORT_JP_LOWER_STRUCT);// 下部構造の降伏照査
		break;
	case T_SIGR_D::kUser:
		str = _LS(IDS_BRDG_SEIS_EVAL_REPORT_JP_USER_STRUCT);// 使用者設定グループの降伏照査
		break;
	default:
		break;
	}
	return str;
}

CString CWriteSeisEvalReportBase::GetTextExamShearDir(const int& nExamShearDir)
{
	CString str;
	switch (nExamShearDir)
	{
		case seis_jp::kY_Dir:
			str = _T("Mz, Fy"); // Y方向
			break;
		case seis_jp::kZ_Dir:
			str = _T("My, Fz"); // Z方向
			break;
	}
	return str;
}

void CWriteSeisEvalReportBase::SetValue_Hyphen(const int& nRow, const int& nCol)
{
	m_pXL->SetValue(nRow, nCol, _T("--"));
	auto* pFormat = m_pXL->GetSheet()->cellFormat(nRow, nCol);
	auto* pNewFormat = m_pXL->GetBook()->addFormat(pFormat);
	pNewFormat->setAlignH(libxl::ALIGNH_CENTER);
	m_pXL->GetSheet()->setCellFormat(nRow, nCol, pNewFormat);
}

void CWriteSeisEvalReportBase::WriteSeisDir(CString& RptBlock, const int& iSeisDir, int& iParagraph3, int& stRow)
{
	int rowCnt = CopyRange(m_pXL, RptBlock, stRow);
	m_pXL->SetValue(stRow, 0, GetText(_T("%d-%d-%d. %s"), *this->m_pParagraph1, *this->m_pParagraph2, ++iParagraph3,
		iSeisDir == T_SIPA_D::kAxis_Dir ? _LS(IDS_BRDG_SEIS_EVAL_REPORT_JP_LONGI) : _LS(IDS_BRDG_SEIS_EVAL_REPORT_JP_TRANS)));
	stRow += rowCnt;
	m_nSeisDir = iSeisDir;
}

void CWriteSeisEvalReportBase::WriteSeisType(CString& RptBlock, const int& iType, int& iParagraph4, int& stRow)
{
	int rowCnt = CopyRange(m_pXL, RptBlock, stRow);
	m_pXL->SetValue(stRow, 0, GetText(_T("%d) %s"), ++iParagraph4,
		iType == T_SIPA_D::kType1 ? _LS(IDS_BRDG_SEIS_EVAL_REPORT_JP_TYPE_I) : _LS(IDS_BRDG_SEIS_EVAL_REPORT_JP_TYPE_II)));//タイプI / タイプII
	stRow += rowCnt;
}

void CWriteSeisEvalReportBase::WriteSeisPos(CString& RptBlock, const int& iPos, int& iParagraph5, int& stRow)
{
	int rowCnt = CopyRange(m_pXL, RptBlock, stRow);
	m_pXL->SetValue(stRow, 0, GetText(_T("%d) %s"), ++iParagraph5,
		iPos == D_MBTP_BEAM ? _LS(IDS_BRDG_SEIS_EVAL_REPORT_JP_RES_BEAM) : _LS(IDS_BRDG_SEIS_EVAL_REPORT_JP_RES_COLUMN)));
	stRow += rowCnt;
}

void CWriteSeisEvalReportBase::WriteReportDir(const int& nSeisDir, const int& nExamDir, const int& nType, CString& strDir)
{
	CString strTypeY;
	CString strTypeZ;

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
}

void CWriteSeisEvalReportBase::WriteNanHyphenValue(const int& nRow, const int& nCol, const double& value)
{
	if (isnan(value))
		SetValue_Hyphen(nRow, nCol);
	else 
		m_pXL->SetValue(nRow, nCol, value);
}
#pragma endregion

#pragma region // CSeisEvalReportConditions
CSeisEvalReportConditions::CSeisEvalReportConditions(IExcel* pXL, CDgnProgressDlg* pProgress, const CString& strBasePath, const CString& strBaseBook, const SDgnExcelPrintOpt* pPrintOpt)
	: CWriteSeisEvalReportBase(pXL, pProgress, strBasePath, strBaseBook, pPrintOpt)
{
}

CSeisEvalReportConditions::~CSeisEvalReportConditions()
{
}

void CSeisEvalReportConditions::AddStep()
{
	if(m_pPrintOpt == NULL) return;
	const auto nSipa = m_pPrintOpt->aPrintSipaK.GetSize();
	for (int i = 0; i < nSipa; ++i)
	{
		const T_SIPA_K SipaK = m_pPrintOpt->aPrintSipaK[i];

		T_SIPA_D SipaD;
		if (!m_pDoc->m_pAttrCtrl2->GetQSipa()->Get(SipaK, SipaD))	continue;
		if (SipaD.nDesignCode != m_nDesignCode)	continue;

		m_pProgress->m_nTotalSteps++;
	}
}

void CSeisEvalReportConditions::Write()
{
	if (m_pPrintOpt == NULL) return;
	const auto nSipa = m_pPrintOpt->aPrintSipaK.GetSize();
	if (nSipa == 0) return;
	
	T_SIPA_D SipaD;
	if (!m_pDoc->m_pAttrCtrl2->GetQSipa()->Get(m_pPrintOpt->aPrintSipaK[0], SipaD)) { ASSERT(0);  return; }
	if (SipaD.nDesignCode != m_nDesignCode) { ASSERT(0);  return; }

	AddSheet();

	CString RptBlock;
	int rowCnt = 0;
	int stRow = 0;
	short Row_Ref = 0, Col_Ref = 0;

	RptBlock = _T("_00_Case");
	rowCnt = CopyRange(m_pXL, RptBlock, stRow);

	// 橋梁タイプ
	m_pXL->SetValue(10, 1, _T("一般橋")); // or 免震橋
	// 橋梁の耐震性能
	switch (SipaD.nSesmicPerformanceType)
	{
	case T_SIPA_D::kLevel1: ASSERT(0); break;
	case T_SIPA_D::kLevel2: m_pXL->SetValue(13, 1, _LS(IDS_IDD_DGN_SEIS_SET_PARAM_DLG_IDC_RADIO_SEIS_PERFORM2)); break;//耐震性能2
	case T_SIPA_D::kLevel3: m_pXL->SetValue(13, 1, _LS(IDS_IDD_DGN_SEIS_SET_PARAM_DLG_IDC_RADIO_SEIS_PERFORM3)); break;//耐震性能3
	default:
		break;
	}

	//照査ケース
	int iRow = 17;
	for (int i = 0; i < nSipa; ++i)
	{
		const T_SIPA_K& SipaK = m_pPrintOpt->aPrintSipaK[i];

		T_SIPA_D SipaD;
		if (!m_pDoc->m_pAttrCtrl2->GetQSipa()->Get(SipaK, SipaD)) continue;

		m_pXL->SetValue(iRow, 1, SipaD.strCaseName);
		switch (SipaD.nSeismicForceDirection)
		{
		case T_SIPA_D::kAxis_Dir:	m_pXL->SetValue(iRow, 2, _LS(IDS_BRDG_SEIS_EVAL_REPORT_JP_LONGI)); break;//橋軸方向
		case T_SIPA_D::kPerp_Dir:	m_pXL->SetValue(iRow, 2, _LS(IDS_BRDG_SEIS_EVAL_REPORT_JP_TRANS)); break;//橋軸直角方向
		default: ASSERT(0);	break;
		}
		switch (SipaD.nSeismicForceType)
		{
		case T_SIPA_D::kType1:	m_pXL->SetValue(iRow, 3, _LS(IDS_BRDG_SEIS_EVAL_REPORT_JP_TYPE_I)); break;
		case T_SIPA_D::kType2:	m_pXL->SetValue(iRow, 3, _LS(IDS_BRDG_SEIS_EVAL_REPORT_JP_TYPE_II)); break;
		default: ASSERT(0);	break;
		}
		const auto nThis = min(SipaD.aSeisThisK.GetSize(), 3);
		for (int j = 0; j < nThis; ++j)
		{
			T_THIS_D ThisD;
			if (!m_pDoc->m_pAttrCtrl->GetThis(SipaD.aSeisThisK[j], ThisD)) continue;
			m_pXL->SetValue(iRow, 4 + j, ThisD.LoadCaseName);
		}
		iRow++;
		if (m_pProgress->NextStep() == FALSE) throw FALSE;
	}

	stRow += rowCnt;
}

void CSeisEvalReportConditions::AddSheet()
{
	m_strBaseSheet = _LSX(00.Case);
	const CString strNewSheet = _LS(IDS_BRDG_SEIS_EVAL_REPORT_JP_CASE);// _T("照査条件");
	if (m_pXL->IsExistSheetName(strNewSheet))
	{
		ASSERT(0);
		m_pXL->SetActiveSheet(strNewSheet);
	}
	else
	{
		m_pXL->AddSheet2(strNewSheet);
		m_pXL->SetActiveSheet(strNewSheet);
	}

	m_pXL->SetStandardFont(_T("Meiryo UI"));
	m_pXL->SetMargin(13, 12, 20, 17);
	m_pXL->SetColumnWidth(0, 0, 0, 60, 12.9 * _ToPoint);
	m_pXL->SetRowHeight(16.5);
	m_pXL->SetZoom(75);
}
#pragma endregion

#pragma region // CSeisEvalReportSummary_A3
CSeisEvalReportSummary_A3::CSeisEvalReportSummary_A3(IExcel* pXL, CDgnProgressDlg* pProgress, const CString& strBasePath, const CString& strBaseBook, const SDgnExcelPrintOpt* pPrintOpt)
	: CWriteSeisEvalReportBase(pXL, pProgress, strBasePath, strBaseBook, pPrintOpt)
{
	m_b00_Summ_1_All = FALSE;
	m_b00_Summ_1_Pier = FALSE;
	m_00_Summ_3_Lower = FALSE;
	m_00_Summ_2_Upper_Title = FALSE;
	m_00_Summ_3_LowerSteel = FALSE;
}

CSeisEvalReportSummary_A3::~CSeisEvalReportSummary_A3()
{
}

void CSeisEvalReportSummary_A3::AddStep()
{
	CArray<T_SIGR_K, T_SIGR_K> aSigrK;
	m_pDoc->m_pAttrCtrl2->GetQSigr()->GetKeyList(aSigrK);
	if (aSigrK.GetSize() < 1) return;

	
	//1.全体系の変位照査
	AddStep_WriteSummaryOverrallDisp(aSigrK);
	//2.橋脚別の変位照査
	AddStep_WriteSummaryPierDisp(aSigrK);
	//3.上部構造の照査
	AddStep_WriteSummaryUpper(aSigrK);
	//4.下部構造の照査
	AddStep_WriteSummaryLower(aSigrK);
	
}

void CSeisEvalReportSummary_A3::AddStep_WriteSummaryOverrallDisp(const CArray<T_SIGR_K, T_SIGR_K>& aSigrK)
{
	const auto nSigr = aSigrK.GetSize();
	for (int i = 0; i < nSigr; ++i)
	{
		const T_SIGR_K SigrK = aSigrK[i];

		T_SIGR_D SigrD;
		if (!m_pDoc->m_pAttrCtrl2->GetQSigr()->Get(SigrK, SigrD)) continue;

		T_EVAL_SUMM_OVERALL eval; //요약결과 (전체계 변위조사)
		if (!m_pResult->GetSummaryOverrall(SigrK, eval)) continue;
		if (eval.IsExistData() == FALSE) continue;

		m_pProgress->m_nTotalSteps++;
	}
}

void CSeisEvalReportSummary_A3::AddStep_WriteSummaryPierDisp(const CArray<T_SIGR_K, T_SIGR_K>& aSigrK)
{
	const auto nSigr = aSigrK.GetSize();
	for (int i = 0; i < nSigr; ++i)
	{
		const T_SIGR_K SigrK = aSigrK[i];

		T_SIGR_D SigrD;
		if (!m_pDoc->m_pAttrCtrl2->GetQSigr()->Get(SigrK, SigrD)) continue;

		T_EVAL_SUMM_PIER eval;
		if (!m_pResult->GetSummaryPier(SigrK, eval)) continue;
		if (eval.IsExistData() == FALSE) continue;

		m_pProgress->m_nTotalSteps++;
	}
}

void CSeisEvalReportSummary_A3::AddStep_WriteSummaryUpper(const CArray<T_SIGR_K, T_SIGR_K>& aSigrK)
{
	const auto nSigr = aSigrK.GetSize();
	for (int i = 0; i < nSigr; ++i)
	{
		const T_SIGR_K SigrK = aSigrK[i];

		T_SIGR_D SigrD;
		if (!m_pDoc->m_pAttrCtrl2->GetQSigr()->Get(SigrK, SigrD)) continue;

		T_EVAL_SUMM_UPPER eval;
		if (!m_pResult->GetSummaryUpper(SigrK, eval)) continue;
		if (eval.IsExistData() == FALSE) continue;

		m_pProgress->m_nTotalSteps++;
	}
}

void CSeisEvalReportSummary_A3::AddStep_WriteSummaryLower(const CArray<T_SIGR_K, T_SIGR_K>& aSigrK)
{
	if (AddStep_WriteSummaryLowerTitle(aSigrK) == FALSE) return;
	AddStep_WriteSummaryLowerColumn(aSigrK);
	AddStep_WriteSummaryLowerBeam(aSigrK);
	AddStep_WriteSummaryLowerSteel(aSigrK);
}

BOOL CSeisEvalReportSummary_A3::AddStep_WriteSummaryLowerTitle(const CArray<T_SIGR_K, T_SIGR_K>& aSigrK)
{
	const auto nSigr = aSigrK.GetSize();
	for (int i = 0; i < nSigr; ++i)
	{
		const T_SIGR_K SigrK = aSigrK[i];

		T_SIGR_D SigrD;
		if (!m_pDoc->m_pAttrCtrl2->GetQSigr()->Get(SigrK, SigrD)) continue;

		T_EVAL_SUMM_PIER eval;
		if (!m_pResult->GetSummaryPier(SigrK, eval)) continue;
		if (eval.IsExistData() == FALSE) continue;

		m_pProgress->m_nTotalSteps++;
		return TRUE;
	}
	return FALSE;
}

void CSeisEvalReportSummary_A3::AddStep_WriteSummaryLowerColumn(const CArray<T_SIGR_K, T_SIGR_K>& aSigrK)
{
	const auto nSigr = aSigrK.GetSize();
	for (int i = 0; i < nSigr; ++i)
	{
		const T_SIGR_K SigrK = aSigrK[i];

		T_SIGR_D SigrD;
		if (!m_pDoc->m_pAttrCtrl2->GetQSigr()->Get(SigrK, SigrD)) continue;

		T_EVAL_SUMM_LOWER eval;
		if (!m_pResult->GetSummaryLower(SigrK, eval)) continue;

		if(!CheckLowerColumn(&eval, T_SIPA_D::kAxis_Dir)) continue;
		m_pProgress->m_nTotalSteps++;

		if (!CheckLowerColumn(&eval, T_SIPA_D::kPerp_Dir)) continue;
		m_pProgress->m_nTotalSteps++;
	}
}

void CSeisEvalReportSummary_A3::AddStep_WriteSummaryLowerBeam(const CArray<T_SIGR_K, T_SIGR_K>& aSigrK)
{
	const auto nSigr = aSigrK.GetSize();
	for (int i = 0; i < nSigr; ++i)
	{
		const T_SIGR_K SigrK = aSigrK[i];

		T_SIGR_D SigrD;
		if (!m_pDoc->m_pAttrCtrl2->GetQSigr()->Get(SigrK, SigrD)) continue;

		T_EVAL_SUMM_LOWER eval;
		if (!m_pResult->GetSummaryLower(SigrK, eval)) continue;

		if (!CheckLowerBeam(&eval, T_SIPA_D::kAxis_Dir)) continue;
		m_pProgress->m_nTotalSteps++;

		if (!CheckLowerBeam(&eval, T_SIPA_D::kAxis_Dir)) continue;
		m_pProgress->m_nTotalSteps++;
	}
}

void CSeisEvalReportSummary_A3::AddStep_WriteSummaryLowerSteel(const CArray<T_SIGR_K, T_SIGR_K>& aSigrK)
{
	const auto nSigr = aSigrK.GetSize();
	for (int i = 0; i < nSigr; ++i)
	{
		const T_SIGR_K SigrK = aSigrK[i];

		T_SIGR_D SigrD;
		if (!m_pDoc->m_pAttrCtrl2->GetQSigr()->Get(SigrK, SigrD)) continue;
		
		T_SIGR_LOWER_D SigrLowerD;
		if (!m_pDoc->m_pAttrCtrl2->GetQSigrLower()->Get(SigrK, SigrLowerD)) continue;
		if (SigrLowerD.nMaterialType != T_SIGR_LOWER_D::kSteel) continue;

		T_EVAL_SUMM_PIER evalSird; // (교각 별 변위조사)
		if (!m_pResult->GetSummaryPier(SigrK, evalSird)) continue;
		if (evalSird.IsExistData() == FALSE) continue;
		if (evalSird.SirdSum.GetSize() == 0) continue;

		T_EVAL_SUMM_LOWER eval;
		if (!m_pResult->GetSummaryLower(SigrK, eval)) continue;
		if (eval.IsExistData() == FALSE) continue;
		if (eval.StelSum.GetSize() == 0) continue;

		if (!CheckLowerSteel(evalSird.SirdSum, eval.StelSum, T_SIPA_D::kAxis_Dir)) continue;
		m_pProgress->m_nTotalSteps++;
	}
}

void CSeisEvalReportSummary_A3::Write()
{
	CArray<T_SIGR_K, T_SIGR_K> aSigrK;
	m_pDoc->m_pAttrCtrl2->GetQSigr()->GetKeyList(aSigrK);
	if (aSigrK.GetSize() < 1) return;

	AddSheet();

	int rowCnt = 0, stRow = 0;
	short Row_Ref = 0, Col_Ref = 0;

	CString RptBlock = _T("_00.Summ_Title");
	rowCnt = CopyRange(m_pXL, RptBlock, stRow);
	stRow += rowCnt;

	int iParagrah = 0;

	//全体系の照査
	WriteSummaryOverrallDisp(T_SIPA_D::kAxis_Dir, aSigrK, stRow, iParagrah);
	WriteSummaryOverrallDisp(T_SIPA_D::kPerp_Dir, aSigrK, stRow, iParagrah);
	//橋脚別の変位照査
	WriteSummaryPierDisp(T_SIPA_D::kAxis_Dir, aSigrK, stRow, iParagrah);
	WriteSummaryPierDisp(T_SIPA_D::kPerp_Dir, aSigrK, stRow, iParagrah);
	//上部構造の照査
	WriteSummaryUpper(T_SIPA_D::kAxis_Dir, aSigrK, stRow, iParagrah);
	WriteSummaryUpper(T_SIPA_D::kPerp_Dir, aSigrK, stRow, iParagrah);
	//下部構造の照査
	WriteSummaryLower(T_SIPA_D::kAxis_Dir, aSigrK, stRow, iParagrah);
	WriteSummaryLower(T_SIPA_D::kPerp_Dir, aSigrK, stRow, iParagrah);
	WriteSummaryLowerSteel(T_SIPA_D::kAxis_Dir, aSigrK, stRow, iParagrah);
	WriteSummaryLowerSteel(T_SIPA_D::kPerp_Dir, aSigrK, stRow, iParagrah);

	m_pXL->SetColumnWidth(0, 0, stRow, 0, 7.56);
}

void CSeisEvalReportSummary_A3::AddSheet()
{
	m_strBaseSheet = _LSX(00.Summ_A3);
	const CString strNewSheet = _LS(IDS_BRDG_SEIS_EVAL_REPORT_JP_SUMMARY);// _T("要約結果");
	if (m_pXL->IsExistSheetName(strNewSheet))
	{
		ASSERT(0);
		m_pXL->SetActiveSheet(strNewSheet);
	}
	else
	{
		m_pXL->AddSheet2(strNewSheet);
		m_pXL->SetActiveSheet(strNewSheet);
		m_pXL->SetA3Paper(TRUE);
	}

	m_pXL->SetStandardFont(_T("Meiryo UI"));
	m_pXL->SetMargin(13, 12, 20, 17);
	m_pXL->SetColumnWidth(0, 1, 0, 60, 7.000);
	m_pXL->SetColumnWidth(0, 0, 0, 0, 2.546);
	m_pXL->SetRowHeight(16.5);
	m_pXL->SetZoom(75);
}

void CSeisEvalReportSummary_A3::WriteSummaryOverrallDisp(const int& nSeisDir, const CArray<T_SIGR_K, T_SIGR_K>& aSigrK, OUT int& stRow, OUT int& iParagraph)
{
	auto I_WriteTitle = [&, this](OUT int& stRow, OUT int& iParagraph)
	{
		int rowCnt = 0;
		CString RptBlock;
		if (m_b00_Summ_1_All == FALSE) {
			m_b00_Summ_1_All = TRUE;
			RptBlock = _T("_00.Summ_1.All");
			rowCnt = CopyRange(m_pXL, RptBlock, stRow);
			//m_pXL->SetValue(stRow, 0, GetText(_T("%d."), ++iParagraph));
			stRow += rowCnt;
		}

		RptBlock = _T("_00.Summ_1.All_Title");
		rowCnt = CopyRange(m_pXL, RptBlock, stRow);

		if(nSeisDir == T_SIPA_D::kAxis_Dir)	m_pXL->SetValue(stRow, 0, GetText(_T("●%s"), _LS(IDS_BRDG_SEIS_EVAL_REPORT_JP_LONGI)));
		else								m_pXL->SetValue(stRow, 0, GetText(_T("●%s"), _LS(IDS_BRDG_SEIS_EVAL_REPORT_JP_TRANS)));
		
		stRow += rowCnt;
	};

	auto l_WriteSimd = [&, this](int nRow, int nCol, const T_SIMD_SUMM_ITEM& summD)
	{
		const double dRatio = SafeDiv(summD.dDeltaMax, summD.dDeltaA);
		m_pXL->SetValue(nRow, nCol+0, summD.strPierName);
		WriteNanHyphenValue(nRow, nCol + 1, summD.dDeltaMax * m_dLenToCM);
		WriteNanHyphenValue(nRow, nCol + 2, summD.dDeltaA * m_dLenToCM);
		WriteNanHyphenValue(nRow, nCol+3, dRatio);
		m_pXL->SetValue(nRow, nCol+4, dRatio > 1.0 ? _T("NG") : _T("OK"));
		if (dRatio > 1.0) {
			m_pXL->SetTextColor(nRow, nCol+3, nRow, nCol+3, CACODENON_RED);
			SetNG_Red(m_pXL, nRow, nCol+4, nRow, nCol+4);
		}
	};

	auto l_WriteSird = [&, this](int nRow, int nCol, const T_SIRD_SUMM_ITEM& summD)
	{
		const double dRatio = abs(SafeDiv(summD.dDeltaR, summD.dDrataRa));
		
		m_pXL->SetValue(nRow, nCol+0, summD.strPierName);
		if (summD.dDeltaR == 0) SetValue_Hyphen(nRow, nCol+1);
		else WriteNanHyphenValue(nRow, nCol + 1, summD.dDeltaR * m_dLenToCM);
		WriteNanHyphenValue(nRow, nCol+2, summD.dDrataRa * m_dLenToCM);
		if (dRatio == 0) {
			SetValue_Hyphen(nRow, nCol+3);
			SetValue_Hyphen(nRow, nCol+4);
		}
		else {
			WriteNanHyphenValue(nRow, nCol+3, dRatio);
			m_pXL->SetValue(nRow, nCol+4, dRatio > 1.0 ? _T("NG") : _T("OK"));
		}
		if (dRatio > 1.0) {
			m_pXL->SetTextColor(nRow, nCol+3, nRow, nCol+3, CACODENON_RED);
			SetNG_Red(m_pXL, nRow, nCol+4, nRow, nCol+4);
		}
	};

	BOOL bWrite = FALSE;
	const auto nSigr = aSigrK.GetSize();
	for (int i = 0; i < nSigr; ++i)	{
		const T_SIGR_K SigrK = aSigrK[i];

		T_SIGR_D SigrD;
		if (!m_pDoc->m_pAttrCtrl2->GetQSigr()->Get(SigrK, SigrD)) continue;

		T_EVAL_SUMM_OVERALL eval; //요약결과 (전체계 변위조사)
		if (!m_pResult->GetSummaryOverrall(SigrK, eval)) continue;
		if (eval.IsExistData(nSeisDir) == FALSE) continue;

		if (bWrite == FALSE) {
			bWrite = TRUE; I_WriteTitle(stRow, iParagraph);
		}

		CString RptBlock = _T("_00.Summ_1.All_Table");
		int rowCnt = CopyRange(m_pXL, RptBlock, stRow);

		m_pXL->SetValue(stRow, 0, SigrD.strName);

		// Simd summary
		auto nLeft = eval.SimdSum.Left.GetSize();
		for (int j = 0; j < nLeft; ++j)	{
			const auto& SimdD = eval.SimdSum.Left[j];
			if(SimdD.nSeismicForceDirection != nSeisDir) continue;
			int nRow = stRow; if (SimdD.nSeismicForceType == T_SIPA_D::kType2) nRow += 2;
			l_WriteSimd(nRow, 3, SimdD);
		}
		auto nRight = eval.SimdSum.Right.GetSize();
		for (int j = 0; j < nRight; ++j) {
			const auto& SimdD = eval.SimdSum.Right[j];
			if (SimdD.nSeismicForceDirection != nSeisDir) continue;
			int nRow = stRow+1; if (SimdD.nSeismicForceType == T_SIPA_D::kType2) nRow += 2;
			l_WriteSimd(nRow, 3, SimdD);
		}

		// Sird summary
		nLeft = eval.SirdSum.Left.GetSize();
		for (int j = 0; j < nLeft; ++j)	{
			const auto& SirdD = eval.SirdSum.Left[j];
			if (SirdD.nSeismicForceDirection != nSeisDir) continue;
			int nRow = stRow; if (SirdD.nSeismicForceType == T_SIPA_D::kType2) nRow += 2;
			l_WriteSird(nRow, 8, SirdD);
		}
		nRight = eval.SirdSum.Right.GetSize();
		for (int j = 0; j < nRight; ++j)
		{
			const auto& SirdD = eval.SirdSum.Right[j];
			if (SirdD.nSeismicForceDirection != nSeisDir) continue;
			int nRow = stRow + 1; if (SirdD.nSeismicForceType == T_SIPA_D::kType2) nRow += 2;
			l_WriteSird(nRow, 8, SirdD);
		}
		stRow += rowCnt;
		if (m_pProgress->NextStep() == FALSE) throw FALSE;
	}
	if (bWrite == TRUE)
		stRow++;
}

void CSeisEvalReportSummary_A3::WriteSummaryPierDisp(const int& nSeisDir, const CArray<T_SIGR_K, T_SIGR_K>& aSigrK, OUT int& stRow, OUT int& iParagraph)
{
	auto I_WriteTitle = [&, this](OUT int& stRow, OUT int& iParagraph)
	{
		int rowCnt = 0;
		CString RptBlock;
		if (m_b00_Summ_1_Pier == FALSE) {
			m_b00_Summ_1_Pier = TRUE;
			RptBlock = _T("_00.Summ_2.Pier");
			rowCnt = CopyRange(m_pXL, RptBlock, stRow);
			//m_pXL->SetValue(stRow, 0, GetText(_T("%d."), ++iParagraph));
			stRow += rowCnt;
		}

		RptBlock = _T("_00.Summ_2.Pier_Title");
		rowCnt = CopyRange(m_pXL, RptBlock, stRow);

		if (nSeisDir == T_SIPA_D::kAxis_Dir) m_pXL->SetValue(stRow, 0, GetText(_T("●%s"), _LS(IDS_BRDG_SEIS_EVAL_REPORT_JP_LONGI)));
		else								 m_pXL->SetValue(stRow, 0, GetText(_T("●%s"), _LS(IDS_BRDG_SEIS_EVAL_REPORT_JP_TRANS)));

		stRow += rowCnt;
	};

	auto l_WriteSimdLower = [&, this](int nRow, int nCol, const T_SIMD_SUMM_ITEM& summD)
	{
		const double dRatio = SafeDiv(summD.dDeltaMax, summD.dDeltaA);
		m_pXL->SetValue(nRow, nCol + 0, summD.strColumnName);
		WriteNanHyphenValue(nRow, nCol + 1, summD.dDeltaMax * m_dLenToCM);
		WriteNanHyphenValue(nRow, nCol + 2, summD.dDeltaA * m_dLenToCM);
		WriteNanHyphenValue(nRow, nCol + 3, dRatio);
		m_pXL->SetValue(nRow, nCol + 4, dRatio > 1.0 ? _T("NG") : _T("OK"));

		if (dRatio > 1.0) {
			m_pXL->SetTextColor(nRow, nCol + 3, nRow, nCol + 3, CACODENON_RED);
			SetNG_Red(m_pXL, nRow, nCol + 4, nRow, nCol + 4);
		}
	};

	auto l_WriteSirdLower = [&, this](int nRow, int nCol, const T_SIRD_SUMM_ITEM& summD)
	{
		const double dRatio = abs(SafeDiv(summD.dDeltaR, summD.dDrataRa));
		
		m_pXL->SetValue(nRow, nCol + 0, summD.strColumnName);
		WriteNanHyphenValue(nRow, nCol + 1, summD.dMuR);
		if (summD.dDeltaR == 0)	SetValue_Hyphen(nRow, nCol + 2);
		else		WriteNanHyphenValue(nRow, nCol + 2, summD.dDeltaR * m_dLenToCM);
		WriteNanHyphenValue(nRow, nCol + 3, summD.dDrataRa * m_dLenToCM);
		if (dRatio == 0) {
			SetValue_Hyphen(nRow, nCol + 4);
			SetValue_Hyphen(nRow, nCol + 5);
		}
		else {
			WriteNanHyphenValue(nRow, nCol + 4, dRatio);
			m_pXL->SetValue(nRow, nCol + 5, dRatio > 1.0 ? _T("NG") : _T("OK"));
		}

		if (dRatio > 1.0) {
			m_pXL->SetTextColor(nRow, nCol + 4, nRow, nCol + 4, CACODENON_RED);
			SetNG_Red(m_pXL, nRow, nCol + 5, nRow, nCol + 5);
		}
	};

	BOOL bWrite = FALSE;
	const auto nSigr = aSigrK.GetSize();
	for (int i = 0; i < nSigr; ++i) {
		const T_SIGR_K SigrK = aSigrK[i];

		T_SIGR_D SigrD;
		if (!m_pDoc->m_pAttrCtrl2->GetQSigr()->Get(SigrK, SigrD)) continue;

		T_EVAL_SUMM_PIER eval;
		if (!m_pResult->GetSummaryPier(SigrK, eval)) continue;
		if (eval.IsExistData() == FALSE) continue;

		if (bWrite == FALSE) {
			bWrite = TRUE; I_WriteTitle(stRow, iParagraph);
		}

		CString RptBlock = _T("_00.Summ_2.Pier_Table");
		int rowCnt = CopyRange(m_pXL, RptBlock, stRow);

		m_pXL->SetValue(stRow, 0, SigrD.strName);

		int nCount = eval.SimdSum.GetCount();
		for (int j = 0; j < nCount; ++j)
		{
			const auto& SimdD = eval.SimdSum[j];
			if (SimdD.nSeismicForceDirection != nSeisDir) continue;
			int nRow = stRow; if (SimdD.nSeismicForceType == T_SIPA_D::kType2) nRow += 1;
			l_WriteSimdLower(nRow, 2, SimdD);
		}

		nCount = eval.SirdSum.GetCount();
		for (int j = 0; j < nCount; ++j)
		{
			const auto& SirdD = eval.SirdSum[j];
			if (SirdD.nSeismicForceDirection != nSeisDir) continue;
			int nRow = stRow; if (SirdD.nSeismicForceType == T_SIPA_D::kType2) nRow += 1;
			l_WriteSirdLower(nRow, 7, SirdD);
		}
		stRow += rowCnt;
		if (m_pProgress->NextStep() == FALSE) throw FALSE;
	}
	if (bWrite == TRUE)
		stRow++;
}

void CSeisEvalReportSummary_A3::WriteSummaryUpper(const int& nSeisDir, const CArray<T_SIGR_K, T_SIGR_K>& aSigrK, OUT int& stRow, OUT int& iParagraph)
{
	auto I_WriteTitle = [&, this](OUT int& stRow, OUT int& iParagraph)
	{
		int rowCnt;
		CString RptBlock;
		if (m_00_Summ_2_Upper_Title == FALSE) {
			m_00_Summ_2_Upper_Title = TRUE;
			RptBlock = _T("_00.Summ_2.Upper");
			rowCnt = CopyRange(m_pXL, RptBlock, stRow);
			//m_pXL->SetValue(stRow, 0, GetText(_T("%d."), ++iParagraph));
			stRow += rowCnt;
		}
		
		RptBlock = _T("_00.Summ_2.Upper_Title");
		rowCnt = CopyRange(m_pXL, RptBlock, stRow);

		if (nSeisDir == T_SIPA_D::kAxis_Dir) m_pXL->SetValue(stRow, 0, GetText(_T("●%s"), _LS(IDS_BRDG_SEIS_EVAL_REPORT_JP_LONGI)));
		else								 m_pXL->SetValue(stRow, 0, GetText(_T("●%s"), _LS(IDS_BRDG_SEIS_EVAL_REPORT_JP_TRANS)));

		stRow += rowCnt;
	};

	BOOL bWrite = FALSE;
	const auto nSigr = aSigrK.GetSize();
	for (int i = 0; i < nSigr; ++i)
	{
		const T_SIGR_K SigrK = aSigrK[i];

		T_SIGR_D SigrD;
		if (!m_pDoc->m_pAttrCtrl2->GetQSigr()->Get(SigrK, SigrD)) continue;
		if (SigrD.nSubType != T_SIGR_D::kUpper) continue;

		T_SIGR_UPPER_D SigrUpperD;
		if (!m_pDoc->m_pAttrCtrl2->GetQSigrUpper()->Get(SigrK, SigrUpperD)) continue;

		T_EVAL_SUMM_UPPER eval;
		if (!m_pResult->GetSummaryUpper(SigrK, eval)) continue;
		if (eval.IsExistData() == FALSE) continue;

		if(!CheckUpperColumn(&eval, nSeisDir)) continue;

		if (bWrite == FALSE) {
			I_WriteTitle(stRow, iParagraph);
			bWrite = TRUE;
		}

		BOOL bWriteTable = FALSE;
		WriteUpperData(nSeisDir, &SigrD, &SigrUpperD, &eval, stRow, bWriteTable, iParagraph);
	}
	if (bWrite == TRUE)
		stRow++;
}

int CSeisEvalReportSummary_A3::WriteUpperTable(const CString& strGroupName, const BOOL& bYZ_Dir, OUT int& stRow)
{
	int rowCnt = 0;
	CString RptBlock;
	if (bYZ_Dir == TRUE) {
		RptBlock = _T("_00.Summ_2.Upper2_Table");
		rowCnt = CopyRange(m_pXL, RptBlock, stRow);
	}
	else {
		RptBlock = _T("_00.Summ_2.Upper1_Table");
		rowCnt = CopyRange(m_pXL, RptBlock, stRow);
	}

	m_pXL->SetValue(stRow, 0, strGroupName);

	return rowCnt;
}

void CSeisEvalReportSummary_A3::WriteUpperData(const int& nSeisDir, const T_SIGR_D* pSigr, const T_SIGR_UPPER_D* pSigrUpper, const T_EVAL_SUMM_UPPER* pEval,
	OUT int& stRow, OUT BOOL& bWriteTable, OUT int& iParagraph)
{
	std::vector<T_SIBD_CVSV_ITEM> CvsvMin, CvsvMax;
	std::vector<T_SIBD_SRSV_ITEM> SrsvMin, SrsvMax;
	std::vector<T_SISR_SUMM_ITEM> SisrMin, SisrMax;
	if (!CheckUpperColumn(pEval, CvsvMin, CvsvMax, SrsvMin, SrsvMax, SisrMin, SisrMax, nSeisDir))
		return;

	int rowCnt = 0;
	BOOL bWriteDir = FALSE;
	BOOL bYZ_Dir = (pSigrUpper->nDirShear == seis_jp::kYZ_Dir);

	if (bWriteTable == FALSE) {
		bWriteTable = TRUE;
		rowCnt = WriteUpperTable(pSigr->strName, bYZ_Dir, stRow);
	}
	
	int stCol = 0;

	// Sibd Cvsv summary; curvature
	stCol = 4;
	WriteCvsvItems(bYZ_Dir, CvsvMin, CvsvMax, stCol, stRow, bWriteDir);

	// Sibd Srsv summary; moment
	stCol = 10;
	WriteSrsvItems(bYZ_Dir, SrsvMin, SrsvMax, stCol, stRow, bWriteDir);

	// Sisr summary; shear
	stCol = 16;
	WriteSisrItems(bYZ_Dir, SisrMin, SisrMax, stCol, stRow, bWriteDir);

	if (rowCnt != 0)
		stRow += rowCnt;
}

BOOL CSeisEvalReportSummary_A3::CheckUpperColumn(const T_EVAL_SUMM_UPPER* pEval, const int& nSeisDir)
{
	std::vector<T_SIBD_CVSV_ITEM> CvsvMin, CvsvMax;
	std::vector<T_SIBD_SRSV_ITEM> SrsvMin, SrsvMax;
	std::vector<T_SISR_SUMM_ITEM> SisrMin, SisrMax;
	if (!CheckUpperColumn(pEval, CvsvMin, CvsvMax, SrsvMin, SrsvMax, SisrMin, SisrMax, nSeisDir))
		return FALSE;
	else
		return TRUE;
}

BOOL CSeisEvalReportSummary_A3::CheckUpperColumn(const T_EVAL_SUMM_UPPER* pEval,
	std::vector<T_SIBD_CVSV_ITEM>& CvsvMin, std::vector<T_SIBD_CVSV_ITEM>& CvsvMax,
	std::vector<T_SIBD_SRSV_ITEM>& SrsvMin, std::vector<T_SIBD_SRSV_ITEM>& SrsvMax,
	std::vector<T_SISR_SUMM_ITEM>& SisrMin, std::vector<T_SISR_SUMM_ITEM>& SisrMax, const int& nSeisDir
)
{
	GetCvsvItem(pEval->CvsvSum.CvsvMin, DB_MEMB_TYPE::D_MBTP_ETC, T_SIGR_LOWER_D::kUnKnown, nSeisDir, CvsvMin);
	GetCvsvItem(pEval->CvsvSum.CvsvMax, DB_MEMB_TYPE::D_MBTP_ETC, T_SIGR_LOWER_D::kUnKnown, nSeisDir, CvsvMax);
	GetSrsvItem(pEval->SrsvSum.SrsvMin, DB_MEMB_TYPE::D_MBTP_ETC, T_SIGR_LOWER_D::kUnKnown, nSeisDir, SrsvMin);
	GetSrsvItem(pEval->SrsvSum.SrsvMax, DB_MEMB_TYPE::D_MBTP_ETC, T_SIGR_LOWER_D::kUnKnown, nSeisDir, SrsvMax);
	GetSisrItem(pEval->SisrSum.SisrMin, DB_MEMB_TYPE::D_MBTP_ETC, T_SIGR_LOWER_D::kUnKnown, nSeisDir, SisrMin);
	GetSisrItem(pEval->SisrSum.SisrMax, DB_MEMB_TYPE::D_MBTP_ETC, T_SIGR_LOWER_D::kUnKnown, nSeisDir, SisrMax);

	if (CvsvMin.size() < 1 && CvsvMax.size() < 1 && SrsvMin.size() < 1 && SrsvMax.size() < 1 && SisrMin.size() < 1 && SisrMax.size() < 1) {
		return FALSE;
	}
	return TRUE;
}

void CSeisEvalReportSummary_A3::WriteSummaryLower(const int& nSeisDir, const CArray<T_SIGR_K, T_SIGR_K>& aSigrK, OUT int& stRow, OUT int& iParagraph)
{
	auto I_WriteTitle = [&, this](OUT int& stRow, OUT int& iParagraph)
	{
		int rowCnt;
		CString RptBlock;
		if (m_00_Summ_3_Lower == FALSE) {
			m_00_Summ_3_Lower = TRUE;
			RptBlock = _T("_00.Summ_3.Lower");
			rowCnt = CopyRange(m_pXL, RptBlock, stRow);
			//m_pXL->SetValue(stRow, 0, GetText(_T("%d."), ++iParagraph));
			stRow += rowCnt;
		}

		RptBlock = _T("_00.Summ_3.Lower_Title");
		rowCnt = CopyRange(m_pXL, RptBlock, stRow);

		if (nSeisDir == T_SIPA_D::kAxis_Dir) m_pXL->SetValue(stRow, 0, GetText(_T("●%s"), _LS(IDS_BRDG_SEIS_EVAL_REPORT_JP_LONGI)));
		else								 m_pXL->SetValue(stRow, 0, GetText(_T("●%s"), _LS(IDS_BRDG_SEIS_EVAL_REPORT_JP_TRANS)));

		stRow += rowCnt;
	};

	BOOL bWriteTitle = FALSE;
	const auto nSigr = aSigrK.GetSize();
	for (int i = 0; i < nSigr; ++i)
	{
		const T_SIGR_K SigrK = aSigrK[i];

		T_SIGR_D SigrD;
		if (!m_pDoc->m_pAttrCtrl2->GetQSigr()->Get(SigrK, SigrD)) continue;
		if (SigrD.nSubType != T_SIGR_D::eSubType::kLower) continue;

		T_SIGR_LOWER_D SigrLowerD;
		if (!m_pDoc->m_pAttrCtrl2->GetQSigrLower()->Get(SigrK, SigrLowerD)) continue;

		T_EVAL_SUMM_LOWER eval;
		if (!m_pResult->GetSummaryLower(SigrK, eval)) continue;
		if (eval.IsExistData() == FALSE) continue;

		if(!CheckLowerColumn(&eval, nSeisDir) && !CheckLowerBeam(&eval, nSeisDir)) continue;

		if (SigrLowerD.COLM_D.GetSize() > 0 || SigrLowerD.BEAM_D.GetSize() > 0)
		{
			if (bWriteTitle == FALSE) {
				bWriteTitle = TRUE; I_WriteTitle(stRow, iParagraph);
			}

			WriteSummaryLowerColumn(nSeisDir, &SigrD, &SigrLowerD, &eval, stRow, iParagraph);

			WriteSummaryLowerBeam(nSeisDir, &SigrD, &SigrLowerD, &eval, stRow, iParagraph);
		}
	}

	if (bWriteTitle == TRUE)
		stRow++;
}

int CSeisEvalReportSummary_A3::WriteLowerTable(const int& nMembType, const CString& strGroupName, const BOOL& bYZ_Dir, OUT int& stRow)
{
	int rowCnt = 0;
	if (bYZ_Dir == TRUE) {
		CString RptBlock = _T("_00.Summ_3.Lower2_Table");
		rowCnt = CopyRange(m_pXL, RptBlock, stRow);
	}
	else {
		CString RptBlock = _T("_00.Summ_3.Lower1_Table");
		rowCnt = CopyRange(m_pXL, RptBlock, stRow);
	}

	CString strMembType = _T("");
	switch (nMembType)
	{
		case DB_MEMB_TYPE::D_MBTP_COLUMN:
			strMembType = _LS(IDS_BRDG_SEIS_EVAL_REPORT_JP_COLUMN);
			break;
		case DB_MEMB_TYPE::D_MBTP_BEAM:
			strMembType = _LS(IDS_BRDG_SEIS_EVAL_REPORT_JP_BEAM);
			break;
	}

	if (strMembType != _T("")) {
		m_pXL->SetValue(stRow, 0, GetText(_T("%s(%s)"),
			strGroupName, strMembType));
	}
	else {
		m_pXL->SetValue(stRow, 0, strGroupName);
	}
	
	return rowCnt;
}

int CSeisEvalReportSummary_A3::WriteLowerSteelTable(const int& nMembType, const CString& strGroupName, const BOOL& bYZ_Dir, OUT int& stRow)
{
	int rowCnt = 0;
	if (bYZ_Dir == TRUE) {
		CString RptBlock = _T("_00.Summ_3.LowerSteel2_Table");
		rowCnt = CopyRange(m_pXL, RptBlock, stRow);
	}
	else {
		CString RptBlock = _T("_00.Summ_3.LowerSteel1_Table");
		rowCnt = CopyRange(m_pXL, RptBlock, stRow);
	}

	CString strMembType = _T("");
	switch (nMembType)
	{
	case DB_MEMB_TYPE::D_MBTP_COLUMN:
		strMembType = _LS(IDS_BRDG_SEIS_EVAL_REPORT_JP_COLUMN);
		break;
	case DB_MEMB_TYPE::D_MBTP_BEAM:
		strMembType = _LS(IDS_BRDG_SEIS_EVAL_REPORT_JP_BEAM);
		break;
	}

	if (strMembType != _T("")) {
		m_pXL->SetValue(stRow, 0, GetText(_T("%s(%s)"),
			strGroupName, strMembType));
	}
	else {
		m_pXL->SetValue(stRow, 0, strGroupName);
	}

	return rowCnt;
}

void CSeisEvalReportSummary_A3::WriteSummaryLowerColumn(const int& nSeisDir, const T_SIGR_D* pSigr, const T_SIGR_LOWER_D* pSigrLower, const T_EVAL_SUMM_LOWER* pEval, OUT int& stRow, OUT int& iParagraph)
{
	if (pSigrLower->COLM_D.GetCount() < 1) return;

	// 곡률
	std::vector<T_SIBD_CVSV_ITEM> CvsvMin, CvsvMax;
	// 휨
	std::vector<T_SIBD_SRSV_ITEM> SrsvMin, SrsvMax;
	// 전단
	std::vector<T_SISR_SUMM_ITEM> SisrMin, SisrMax, SisrRebarCutMin, SisrRebarCutMax;
	// get Cvsv, Srsv, Sisr items ...
	if (!CheckLowerColumn(pEval, CvsvMin, CvsvMax, SrsvMin, SrsvMax, SisrMin, SisrMax, SisrRebarCutMin, SisrRebarCutMax, nSeisDir))
		return;

	BOOL bWriteDir = FALSE;
	BOOL bYZ_Dir = (pSigrLower->COLM_D[0].nDirShear == seis_jp::kYZ_Dir);

	int rowCnt = WriteLowerTable(DB_MEMB_TYPE::D_MBTP_COLUMN, pSigr->strName, bYZ_Dir, stRow);
	int stCol = 0;

	// Sibd Cvsv summary; curvature
	stCol = 4;
	WriteCvsvItems(bYZ_Dir, CvsvMin, CvsvMax, stCol, stRow, bWriteDir);

	// Sisr summary; shear
	stCol = 10;
	WriteSisrItems(bYZ_Dir, SisrMin, SisrMax, stCol, stRow, bWriteDir);

	// 철근 단락부(휨)
	stCol = 15;
	WriteSrsvItems(bYZ_Dir, SrsvMin, SrsvMax, stCol, stRow, bWriteDir);

	// 철근 단락부(전단)
	stCol = 20;
	WriteSisrItems(bYZ_Dir, SisrRebarCutMin, SisrRebarCutMax, stCol, stRow, bWriteDir);

	stRow += rowCnt;
	if (m_pProgress->NextStep() == FALSE) throw FALSE;
}

void CSeisEvalReportSummary_A3::WriteSummaryLowerBeam(const int& nSeisDir, const T_SIGR_D* pSigr, const T_SIGR_LOWER_D* pSigrLower, const T_EVAL_SUMM_LOWER* pEval, OUT int& stRow, OUT int& iParagraph)
{
	if (pSigrLower->BEAM_D.GetCount() < 1) return;

	// 곡률
	std::vector<T_SIBD_CVSV_ITEM> CvsvMin, CvsvMax;
	// 전단
	std::vector<T_SISR_SUMM_ITEM> SisrMin, SisrMax;
	// get Cvsv, Sisr items ...
	if (!CheckLowerBeam(pEval, CvsvMin, CvsvMax, SisrMin, SisrMax, nSeisDir))
		return;

	BOOL bWriteDir = FALSE;
	BOOL bYZ_Dir = (pSigrLower->BEAM_D[0].nDirShear == seis_jp::kYZ_Dir);

	int rowCnt = WriteLowerTable(DB_MEMB_TYPE::D_MBTP_BEAM, pSigr->strName, bYZ_Dir, stRow);
	int stCol = 0;

	// Sibd Cvsv summary; curvature
	stCol = 4;
	WriteCvsvItems(bYZ_Dir, CvsvMin, CvsvMax, stCol, stRow, bWriteDir);

	// Sisr summary; shear
	stCol = 10;
	WriteSisrItems(bYZ_Dir, SisrMin, SisrMax, stCol, stRow, bWriteDir);

	stRow += rowCnt;
	if (m_pProgress->NextStep() == FALSE) throw FALSE;
}

BOOL CSeisEvalReportSummary_A3::CheckLowerColumn(const T_EVAL_SUMM_LOWER* pEval, const int& nSeisDir)
{
	// 곡률
	std::vector<T_SIBD_CVSV_ITEM> CvsvMin, CvsvMax;
	// 휨
	std::vector<T_SIBD_SRSV_ITEM> SrsvMin, SrsvMax;
	// 전단
	std::vector<T_SISR_SUMM_ITEM> SisrMin, SisrMax, SisrRebarCutMin, SisrRebarCutMax;
	if (!CheckLowerColumn(pEval, CvsvMin, CvsvMax, SrsvMin, SrsvMax, SisrMin, SisrMax, SisrRebarCutMin, SisrRebarCutMax, nSeisDir))
		return FALSE;
	else
		return TRUE;
}

BOOL CSeisEvalReportSummary_A3::CheckLowerColumn(const T_EVAL_SUMM_LOWER* pEval,
	std::vector<T_SIBD_CVSV_ITEM>& CvsvMin, std::vector<T_SIBD_CVSV_ITEM>& CvsvMax,
	std::vector<T_SIBD_SRSV_ITEM>& SrsvMin, std::vector<T_SIBD_SRSV_ITEM>& SrsvMax,
	std::vector<T_SISR_SUMM_ITEM>& SisrMin, std::vector<T_SISR_SUMM_ITEM>& SisrMax,
	std::vector<T_SISR_SUMM_ITEM>& SisrRebarCutMin, std::vector<T_SISR_SUMM_ITEM>& SisrRebarCutMax, const int& nSeisDir
	)
{
	// 곡률
	GetCvsvItem(pEval->CvsvSum.CvsvMin, DB_MEMB_TYPE::D_MBTP_COLUMN, T_SIGR_LOWER_D::kRC, nSeisDir, CvsvMin);
	GetCvsvItem(pEval->CvsvSum.CvsvMax, DB_MEMB_TYPE::D_MBTP_COLUMN, T_SIGR_LOWER_D::kRC, nSeisDir, CvsvMax);

	// 휨
	GetSrsvItem(pEval->RebarCutSrsvSum.SrsvMin, DB_MEMB_TYPE::D_MBTP_COLUMN, T_SIGR_LOWER_D::kRC, nSeisDir, SrsvMin);
	GetSrsvItem(pEval->RebarCutSrsvSum.SrsvMax, DB_MEMB_TYPE::D_MBTP_COLUMN, T_SIGR_LOWER_D::kRC, nSeisDir, SrsvMax);

	// 전단
	GetSisrItem(pEval->SisrSum.SisrMin, DB_MEMB_TYPE::D_MBTP_COLUMN, T_SIGR_LOWER_D::kRC, nSeisDir, SisrMin);
	GetSisrItem(pEval->SisrSum.SisrMax, DB_MEMB_TYPE::D_MBTP_COLUMN, T_SIGR_LOWER_D::kRC, nSeisDir, SisrMax);
	GetSisrItem(pEval->RebarCutSisrSum.SisrMin, DB_MEMB_TYPE::D_MBTP_COLUMN, T_SIGR_LOWER_D::kRC, nSeisDir, SisrRebarCutMin);
	GetSisrItem(pEval->RebarCutSisrSum.SisrMax, DB_MEMB_TYPE::D_MBTP_COLUMN, T_SIGR_LOWER_D::kRC, nSeisDir, SisrRebarCutMax);

	if (CvsvMin.size() < 1 && CvsvMax.size() < 1 && SisrMin.size() < 1 && SisrMax.size() < 1 &&
		SrsvMin.size() < 1 && SrsvMin.size() < 1 && SisrRebarCutMin.size() < 1 && SisrRebarCutMax.size() < 1) {
		return FALSE;
	}
	return TRUE;
}

BOOL CSeisEvalReportSummary_A3::CheckLowerBeam(const T_EVAL_SUMM_LOWER* pEval, const int& nSeisDir)
{
	// 곡률
	std::vector<T_SIBD_CVSV_ITEM> CvsvMin, CvsvMax;
	// 전단
	std::vector<T_SISR_SUMM_ITEM> SisrMin, SisrMax;
	if (!CheckLowerBeam(pEval, CvsvMin, CvsvMax,  SisrMin, SisrMax, nSeisDir))
		return FALSE;
	else
		return TRUE;
}

BOOL CSeisEvalReportSummary_A3::CheckLowerBeam(const T_EVAL_SUMM_LOWER* pEval,
	std::vector<T_SIBD_CVSV_ITEM>& CvsvMin, std::vector<T_SIBD_CVSV_ITEM>& CvsvMax,
	std::vector<T_SISR_SUMM_ITEM>& SisrMin, std::vector<T_SISR_SUMM_ITEM>& SisrMax, const int& nSeisDir)
{
	// 곡률
	GetCvsvItem(pEval->CvsvSum.CvsvMin, DB_MEMB_TYPE::D_MBTP_BEAM, T_SIGR_LOWER_D::kRC, nSeisDir, CvsvMin);
	GetCvsvItem(pEval->CvsvSum.CvsvMax, DB_MEMB_TYPE::D_MBTP_BEAM, T_SIGR_LOWER_D::kRC, nSeisDir, CvsvMax);
	// 전단
	GetSisrItem(pEval->SisrSum.SisrMin, DB_MEMB_TYPE::D_MBTP_BEAM, T_SIGR_LOWER_D::kRC, nSeisDir, SisrMin);
	GetSisrItem(pEval->SisrSum.SisrMax, DB_MEMB_TYPE::D_MBTP_BEAM, T_SIGR_LOWER_D::kRC, nSeisDir, SisrMax);

	if (CvsvMin.size() < 1 && CvsvMax.size() < 1 && SisrMin.size() < 1 && SisrMax.size() < 1) {
		return FALSE;
	}
	return TRUE;
}

void CSeisEvalReportSummary_A3::WriteSummaryLowerSteel(const int& nSeisDir, const CArray<T_SIGR_K, T_SIGR_K>& aSigrK, OUT int& stRow, OUT int& iParagraph)
{
	auto I_WriteTitle = [&, this](OUT int& stRow, OUT int& iParagraph)
		{
			int rowCnt;
			CString RptBlock;
			if (m_00_Summ_3_LowerSteel == FALSE) {
				m_00_Summ_3_LowerSteel = TRUE;
				RptBlock = _T("_00.Summ_3.LowerSteel");
				rowCnt = CopyRange(m_pXL, RptBlock, stRow);
				//m_pXL->SetValue(stRow, 0, GetText(_T("%d."), ++iParagraph));
				stRow += rowCnt;
			}

			RptBlock = _T("_00.Summ_3.LowerSteel_Title");
			rowCnt = CopyRange(m_pXL, RptBlock, stRow);

			if (nSeisDir == T_SIPA_D::kAxis_Dir) m_pXL->SetValue(stRow, 0, GetText(_T("●%s"), _LS(IDS_BRDG_SEIS_EVAL_REPORT_JP_LONGI)));
			else								 m_pXL->SetValue(stRow, 0, GetText(_T("●%s"), _LS(IDS_BRDG_SEIS_EVAL_REPORT_JP_TRANS)));

			stRow += rowCnt;
		};

	BOOL bWriteTitle = FALSE;
	const auto nSigr = aSigrK.GetSize();
	for (int i = 0; i < nSigr; ++i)
	{
		const T_SIGR_K SigrK = aSigrK[i];

		T_SIGR_D SigrD;
		if (!m_pDoc->m_pAttrCtrl2->GetQSigr()->Get(SigrK, SigrD)) continue;
		if (SigrD.nSubType != T_SIGR_D::eSubType::kLower) continue;

		T_SIGR_LOWER_D SigrLowerD;
		if (!m_pDoc->m_pAttrCtrl2->GetQSigrLower()->Get(SigrK, SigrLowerD)) continue;
		if(SigrLowerD.nMaterialType != T_SIGR_LOWER_D::kSteel) continue;

		T_EVAL_SUMM_PIER evalSird; // (교각 별 변위조사)
		if (!m_pResult->GetSummaryPier(SigrK, evalSird)) continue;
		if (evalSird.IsExistData() == FALSE) continue;
		if(evalSird.SirdSum.GetSize() ==0 ) continue;

		T_EVAL_SUMM_LOWER eval;
		if (!m_pResult->GetSummaryLower(SigrK, eval)) continue;
		if (eval.IsExistData() == FALSE) continue;
		if(eval.StelSum.GetSize() == 0) continue;

		if(!CheckLowerSteel(evalSird.SirdSum, eval.StelSum, nSeisDir)) continue;

		if (SigrLowerD.COLM_D.GetSize() > 0 || SigrLowerD.BEAM_D.GetSize() > 0)
		{
			if (bWriteTitle == FALSE) {
				bWriteTitle = TRUE; I_WriteTitle(stRow, iParagraph);
			}

			int nDispRow = stRow;
			WriteSummaryLowerSteelColBeam(nSeisDir, &SigrD, &SigrLowerD, eval.StelSum, DB_MEMB_TYPE::D_MBTP_COLUMN, stRow, iParagraph);
			if(nDispRow != stRow) WriteSummaryLowerSteelDisp(nSeisDir, &SigrLowerD, DB_MEMB_TYPE::D_MBTP_COLUMN, evalSird.SirdSum, nDispRow);
			nDispRow = stRow;
			WriteSummaryLowerSteelColBeam(nSeisDir, &SigrD, &SigrLowerD, eval.StelSum, DB_MEMB_TYPE::D_MBTP_BEAM, stRow, iParagraph);
			if (nDispRow != stRow) WriteSummaryLowerSteelDisp(nSeisDir, &SigrLowerD, DB_MEMB_TYPE::D_MBTP_BEAM, evalSird.SirdSum, nDispRow);

			if (m_pProgress->NextStep() == FALSE) throw FALSE;
		}
	}

	if (bWriteTitle == TRUE)
		stRow++;
}

void CSeisEvalReportSummary_A3::WriteSummaryLowerSteelColBeam(const int& nSeisDir, const T_SIGR_D* pSigr, const T_SIGR_LOWER_D* pSigrLower, const CArray<T_SIBD_STEL_ITEM, T_SIBD_STEL_ITEM>& StelSum, const DB_MEMB_TYPE& nMembType, OUT int& stRow, OUT int& iParagraph)
{
	auto l_WriteSteelColumn = [this](const int& nRow, const int& nCol, const T_SIBD_STEL_ITEM& data) {
		//isnan(summD.dDeltaYc)
		m_pXL->SetValue(nRow, 3, data.kElem);
		WriteNanHyphenValue(nRow, nCol+0, data.dDeltaMax);
		WriteNanHyphenValue(nRow, nCol+1, data.dDeltaYc);
		WriteNanHyphenValue(nRow, nCol+2, data.dDeltaYt);
		WriteNanHyphenValue(nRow, nCol+3, data.dDeltaA);
		WriteNanHyphenValue(nRow, nCol+4, data.dRatioYc);
		WriteNanHyphenValue(nRow, nCol+5, data.dRatioYt);
		m_pXL->SetValue(nRow, nCol+6, data.dRatioYt > 1.0 ? _T("NG") : _T("OK"));
		if (data.dRatioYt > 1.0)
		{
			m_pXL->SetTextColor(nRow, nCol + 6, nRow, nCol + 6, CACODENON_RED);
			SetNG_Red(m_pXL, nRow, nCol + 6, nRow, nCol + 6);
		}
	};

	BOOL bWriteDir = FALSE;
	BOOL bYZ_Dir = FALSE;
	int nDir = 0;
	if (nMembType == DB_MEMB_TYPE::D_MBTP_COLUMN)
	{
		if (pSigrLower->COLM_D.GetCount() < 1) return;
		bYZ_Dir = (pSigrLower->COLM_D[0].nDirShear == seis_jp::kYZ_Dir);
		nDir = pSigrLower->COLM_D[0].nDirShear;
	}
	else
	{
		if (pSigrLower->BEAM_D.GetCount() < 1) return;
		bYZ_Dir = (pSigrLower->BEAM_D[0].nDirShear == seis_jp::kYZ_Dir);
		nDir = pSigrLower->BEAM_D[0].nDirShear;
	}

	int rowCnt = WriteLowerSteelTable(nMembType, pSigr->strName, bYZ_Dir, stRow);

	CString strCheckDir;
	WriteReportDir(nSeisDir, nDir, eType::kALL, strCheckDir);
	//if (!bYZ_Dir && nDir != 0) m_pXL->SetValue(stRow, 2, nDir == seis_jp::kY_Dir ? _T("My, Fz") : _T("Mz, Fy"));
	if (!bYZ_Dir && nDir != 0) m_pXL->SetValue(stRow, 2, strCheckDir);

	int nType2 = bYZ_Dir ? 2 : 1;
	int nRow = stRow;
	for (int i = 0; i < StelSum.GetSize(); ++i)
	{
		T_SIBD_STEL_ITEM StelD = StelSum[i];
		if (StelD.nSeismicForceDirection != nSeisDir) continue;
		if (StelD.nMaterialType != T_SIGR_LOWER_D::kSteel) continue;
		if (StelD.nMemberType != nMembType) continue;

		if (bYZ_Dir && StelD.nExamShearDir == seis_jp::kZ_Dir)
			nRow += 1;

		if (StelD.nSeismicForceType == T_SIPA_D::kType1)
		{
			l_WriteSteelColumn(nRow, 8, StelD);
		}
		else if (StelD.nSeismicForceType == T_SIPA_D::kType2)
		{
			l_WriteSteelColumn(nRow + nType2, 8, StelD);
		}
	}

	stRow += rowCnt;
}

void CSeisEvalReportSummary_A3::WriteSummaryLowerSteelDisp(const int& nSeisDir, const T_SIGR_LOWER_D* pSigrLower, const DB_MEMB_TYPE& nMembType, const CArray<T_SIRD_SUMM_ITEM, T_SIRD_SUMM_ITEM>& SirdSum, const int& stRow)
{
	auto l_WriteSird = [&, this](int nRow, int nCol, const T_SIRD_SUMM_ITEM& summD)
		{
			const double dRatio = abs(SafeDiv(summD.dDeltaR, summD.dDrataRa));

			if (summD.dDeltaR == 0) SetValue_Hyphen(nRow, nCol);
			else WriteNanHyphenValue(nRow, nCol, summD.dDeltaR * m_dLenToCM);
			WriteNanHyphenValue(nRow, nCol + 1, summD.dDrataRa * m_dLenToCM);
			if (dRatio == 0) {
				SetValue_Hyphen(nRow, nCol + 2);
				SetValue_Hyphen(nRow, nCol + 3);
			}
			else {
				WriteNanHyphenValue(nRow, nCol + 2, dRatio);
				m_pXL->SetValue(nRow, nCol + 3, dRatio > 1.0 ? _T("NG") : _T("OK"));
			}
			if (dRatio > 1.0) {
				m_pXL->SetTextColor(nRow, nCol + 2, nRow, nCol + 2, CACODENON_RED);
				SetNG_Red(m_pXL, nRow, nCol + 3, nRow, nCol + 3);
			}
		};

	BOOL bYZ_Dir = FALSE;
	if (nMembType == DB_MEMB_TYPE::D_MBTP_COLUMN)
	{
		if (pSigrLower->COLM_D.GetCount() < 1) return;
		bYZ_Dir = (pSigrLower->COLM_D[0].nDirShear == seis_jp::kYZ_Dir);
	}
	else
	{
		if (pSigrLower->BEAM_D.GetCount() < 1) return;
		bYZ_Dir = (pSigrLower->BEAM_D[0].nDirShear == seis_jp::kYZ_Dir);
	}

	int nType2 = bYZ_Dir ? 2 : 1;
	int nRow = stRow;
	for (int i = 0; i < SirdSum.GetSize(); i++)
	{
		T_SIRD_SUMM_ITEM StelD = SirdSum[i];
		if (StelD.nSeismicForceDirection != nSeisDir) continue;

		if (StelD.nSeismicForceType == T_SIPA_D::kType1)
		{
			l_WriteSird(nRow, 4, StelD);
			if (bYZ_Dir) l_WriteSird(nRow +1, 4, StelD);
		}
		else if(StelD.nSeismicForceType == T_SIPA_D::kType2)
		{
			l_WriteSird(nRow + nType2, 4, StelD);
			if (bYZ_Dir) l_WriteSird(nRow +1 + nType2, 4, StelD);
		}
	}
}

BOOL CSeisEvalReportSummary_A3::CheckLowerSteel(const CArray<T_SIRD_SUMM_ITEM, T_SIRD_SUMM_ITEM>& SirdSum, const CArray<T_SIBD_STEL_ITEM, T_SIBD_STEL_ITEM>& StelSum, const int& nSeisDir)
{
	vector< T_SIRD_SUMM_ITEM> aData;
	for (int i = 0; i < SirdSum.GetSize(); i++)
	{
		T_SIRD_SUMM_ITEM StelD = SirdSum[i];
		if (StelD.nSeismicForceDirection != nSeisDir) continue;

		aData.push_back(StelD);
	}

	vector< T_SIBD_STEL_ITEM> aData2;
	for (int i = 0; i < StelSum.GetSize(); ++i)
	{
		T_SIBD_STEL_ITEM StelD = StelSum[i];
		if (StelD.nSeismicForceDirection != nSeisDir) continue;
		if (StelD.nMaterialType != T_SIGR_LOWER_D::kSteel) continue;

		aData2.push_back(StelD);
	}

	if (aData.size() == 0 || aData2.size() == 0) return FALSE;
	else return TRUE;
}

void CSeisEvalReportSummary_A3::GetCvsvItem(const CArray<T_SIBD_CVSV_ITEM, T_SIBD_CVSV_ITEM>& items, const int nMembType, T_SIGR_LOWER_D::eMaterialType eMatType, const int& nSeisDir, OUT std::vector<T_SIBD_CVSV_ITEM>& rData)
{
	int nCount = items.GetCount();
	for (int i = 0; i < nCount; i++)
	{
		const T_SIBD_CVSV_ITEM& item = items[i];

		if (item.nMemberType != nMembType) continue;
		if (item.nMaterialType != eMatType) continue;
		if (item.nSeismicForceDirection != nSeisDir) continue;

		rData.push_back(item);
	}
}

void CSeisEvalReportSummary_A3::GetSrsvItem(const CArray<T_SIBD_SRSV_ITEM, T_SIBD_SRSV_ITEM>& items, const int nMembType, T_SIGR_LOWER_D::eMaterialType eMatType, const int& nSeisDir, OUT std::vector<T_SIBD_SRSV_ITEM>& rData)
{
	int nCount = items.GetCount();
	for (int i = 0; i < nCount; i++)
	{
		const T_SIBD_SRSV_ITEM& item = items[i];

		if (item.nMemberType != nMembType) continue;
		if (item.nMaterialType != eMatType) continue;
		if (item.nSeismicForceDirection != nSeisDir) continue;

		rData.push_back(item);
	}
}

void CSeisEvalReportSummary_A3::GetSisrItem(const CArray<T_SISR_SUMM_ITEM, T_SISR_SUMM_ITEM>& items, const int nMembType, T_SIGR_LOWER_D::eMaterialType eMatType, const int& nSeisDir, OUT std::vector<T_SISR_SUMM_ITEM>& rData)
{
	int nCount = items.GetCount();
	for (int i = 0; i < nCount; i++)
	{
		const T_SISR_SUMM_ITEM& item = items[i];

		if (item.nMemberType != nMembType) continue;
		if (item.nMaterialType != eMatType) continue;
		if (item.nSeismicForceDirection != nSeisDir) continue;

		rData.push_back(item);
	}
}

void CSeisEvalReportSummary_A3::WriteCvsvItems(const BOOL& bYZ_Dir, const std::vector<T_SIBD_CVSV_ITEM>& CvsvMin, 
	const std::vector<T_SIBD_CVSV_ITEM>& CvsvMax, const int& stCol, OUT int& stRow, OUT BOOL& bWriteDir)
{
	BOOL bRebarCut = (stCol == 15); // 철근 단락부 여부
	int nRow = 0, nCol = stCol;

	auto nMax = CvsvMax.size();
	for (int j = 0; j < nMax; ++j)
	{
		nRow = stRow;
		const T_SIBD_CVSV_ITEM& SibdD = CvsvMax[j];
		const int& nShearForceDir = (SibdD.nSeismicForceDirection == T_SIPA_D::kAxis_Dir) ? SibdD.nExamShearDir : (SibdD.nExamShearDir == seis_jp::kY_Dir ? seis_jp::kZ_Dir : seis_jp::kY_Dir);

		if (bYZ_Dir == FALSE && bWriteDir == FALSE) {
			bWriteDir = TRUE; m_pXL->SetValue(stRow, 2, GetTextExamShearDir(nShearForceDir));
		}

		if (SibdD.nSeismicForceType == T_SIPA_D::kType2)
			nRow = bYZ_Dir ? nRow + 4 : nRow + 2;
		if (bYZ_Dir == TRUE && nShearForceDir == seis_jp::kY_Dir)
			nRow += 2;

		if (bRebarCut == FALSE) WriteCvsvData(nRow, nCol, SibdD);
		else					WriteCvsvRebarCutData(nRow, nCol, SibdD);
	}

	auto nMin = CvsvMin.size();
	for (int j = 0; j < nMin; ++j)
	{
		nRow = stRow;
		const T_SIBD_CVSV_ITEM& SibdD = CvsvMin[j];
		const int& nShearForceDir = (SibdD.nSeismicForceDirection == T_SIPA_D::kAxis_Dir) ? SibdD.nExamShearDir : (SibdD.nExamShearDir == seis_jp::kY_Dir ? seis_jp::kZ_Dir : seis_jp::kY_Dir);

		if (bYZ_Dir == FALSE && bWriteDir == FALSE) {
			bWriteDir = TRUE; m_pXL->SetValue(stRow, 2, GetTextExamShearDir(nShearForceDir));
		}

		if (SibdD.nSeismicForceType == T_SIPA_D::kType2)
			nRow = bYZ_Dir ? nRow + 4 : nRow + 2;
		if (bYZ_Dir == TRUE && nShearForceDir == seis_jp::kY_Dir)
			nRow += 2;

		if (bRebarCut == FALSE) WriteCvsvData(nRow+1, nCol, SibdD);
		else					WriteCvsvRebarCutData(nRow + 1, nCol, SibdD);
	}
}

void CSeisEvalReportSummary_A3::WriteSrsvItems(const BOOL& bYZ_Dir, const std::vector<T_SIBD_SRSV_ITEM>& SrsvMin,
	const std::vector<T_SIBD_SRSV_ITEM>& SrsvMax, const int& stCol, OUT int& stRow, OUT BOOL& bWriteDir)
{
	auto l_WriteData = [&](int nRow, int nCol, const T_SIBD_SRSV_ITEM& SrsvD)
	{
		const double dRatio = SafeDiv(SrsvD.dMmax, SrsvD.dMy);
		m_pXL->SetValue(nRow, nCol + 0, SrsvD.kElem);
		WriteNanHyphenValue(nRow, nCol + 1, SrsvD.dMmax);
		WriteNanHyphenValue(nRow, nCol + 2, SrsvD.dMy);
		WriteNanHyphenValue(nRow, nCol + 3, dRatio);
		m_pXL->SetValue(nRow, nCol + 4, dRatio > 1.0 ? _T("NG") : _T("OK"));

		if (dRatio > 1.0) {
			m_pXL->SetTextColor(nRow, nCol + 3, nRow, nCol + 3, CACODENON_RED);
			SetNG_Red(m_pXL, nRow, nCol + 4, nRow, nCol + 4);
		}
	};

	int nRow = 0, nCol = stCol;

	auto nMax = SrsvMax.size();
	for (int j = 0; j < nMax; ++j)
	{
		nRow = stRow;
		const T_SIBD_SRSV_ITEM& SrsvD = SrsvMax[j];
		const int& nShearForceDir = (SrsvD.nSeismicForceDirection == T_SIPA_D::kAxis_Dir) ? SrsvD.nExamShearDir : (SrsvD.nExamShearDir == seis_jp::kY_Dir ? seis_jp::kZ_Dir : seis_jp::kY_Dir);

		if (bYZ_Dir == FALSE && bWriteDir == FALSE) {
			bWriteDir = TRUE; m_pXL->SetValue(stRow, 2, GetTextExamShearDir(nShearForceDir));
		}

		if (SrsvD.nSeismicForceType == T_SIPA_D::kType2)
			nRow = bYZ_Dir ? nRow + 4 : nRow + 2;
		if (bYZ_Dir == TRUE && nShearForceDir == seis_jp::kY_Dir)
			nRow += 2;

		l_WriteData(nRow, nCol, SrsvD);
	}

	auto nMin = SrsvMin.size();
	for (int j = 0; j < nMin; ++j)
	{
		nRow = stRow;
		const T_SIBD_SRSV_ITEM& SrsvD = SrsvMin[j];
		const int& nShearForceDir = (SrsvD.nSeismicForceDirection == T_SIPA_D::kAxis_Dir) ? SrsvD.nExamShearDir : (SrsvD.nExamShearDir == seis_jp::kY_Dir ? seis_jp::kZ_Dir : seis_jp::kY_Dir);

		if (bYZ_Dir == FALSE && bWriteDir == FALSE) {
			bWriteDir = TRUE; m_pXL->SetValue(stRow, 2, GetTextExamShearDir(nShearForceDir));
		}

		if (SrsvD.nSeismicForceType == T_SIPA_D::kType2)
			nRow = bYZ_Dir ? nRow + 4 : nRow + 2;
		if (bYZ_Dir == TRUE && nShearForceDir == seis_jp::kY_Dir)
			nRow += 2;

		l_WriteData(nRow + 1, nCol, SrsvD);
	}
}

void CSeisEvalReportSummary_A3::WriteSisrItems(const BOOL& bYZ_Dir, const std::vector<T_SISR_SUMM_ITEM>& SisrMin, 
	const std::vector<T_SISR_SUMM_ITEM>& SisrMax, const int& stCol, OUT int& stRow, OUT BOOL& bWriteDir)
{
	auto l_WriteData = [&](int nRow, int nCol, const T_SISR_SUMM_ITEM& SisrD)
	{
		if (SisrD.nDesignCriteria == T_SISR_AUTO_D::kRoad3) {
			const double dRatioSus = SafeDiv(SisrD.dSmax, SisrD.dSus);
			const double dRatioSuc = SafeDiv(SisrD.dSmax, SisrD.dSuc);
			m_pXL->SetValue(nRow, nCol + 0, SisrD.kElem);
			WriteNanHyphenValue(nRow, nCol + 1, SisrD.dSmax);
			WriteNanHyphenValue(nRow, nCol + 2, SisrD.dSus);
			WriteNanHyphenValue(nRow, nCol + 3, SisrD.dSuc);
			WriteNanHyphenValue(nRow, nCol + 4, dRatioSus);
			WriteNanHyphenValue(nRow, nCol + 5, dRatioSuc);
			m_pXL->SetValue(nRow, nCol + 6, dRatioSus > 1.0 ? _T("NG") : _T("OK"));
			m_pXL->SetValue(nRow, nCol + 7, dRatioSuc > 1.0 ? _T("NG") : _T("OK"));

			if (dRatioSus > 1.0) {
				m_pXL->SetTextColor(nRow, nCol + 4, nRow, nCol + 4, CACODENON_RED);
				SetNG_Red(m_pXL, nRow, nCol + 6, nRow, nCol + 6);
			}
			if (dRatioSuc > 1.0) {
				m_pXL->SetTextColor(nRow, nCol + 5, nRow, nCol + 5, CACODENON_RED);
				SetNG_Red(m_pXL, nRow, nCol + 7, nRow, nCol + 7);
			}
		}
		else {
			const double dRatio = SafeDiv(SisrD.dSmax, SisrD.dPs);
			m_pXL->SetValue(nRow, nCol + 0, SisrD.kElem);
			WriteNanHyphenValue(nRow, nCol + 1, SisrD.dSmax);
			WriteNanHyphenValue(nRow, nCol + 2, SisrD.dPs);
			WriteNanHyphenValue(nRow, nCol + 3, dRatio);
			m_pXL->SetValue(nRow, nCol + 4, dRatio > 1.0 ? _T("NG") : _T("OK"));

			if (dRatio > 1.0) {
				m_pXL->SetTextColor(nRow, nCol + 3, nRow, nCol + 3, CACODENON_RED);
				SetNG_Red(m_pXL, nRow, nCol + 4, nRow, nCol + 4);
			}
		}
	};

	int nRow = 0, nCol = stCol;

	auto nMax = SisrMax.size();
	for (int j = 0; j < nMax; ++j)
	{
		nRow = stRow;
		const T_SISR_SUMM_ITEM& SisrD = SisrMax[j];
		const int& nShearForceDir = (SisrD.nSeismicForceDirection == T_SIPA_D::kAxis_Dir) ? SisrD.nExamShearDir : (SisrD.nExamShearDir == seis_jp::kY_Dir ? seis_jp::kZ_Dir : seis_jp::kY_Dir);

		if (bYZ_Dir == FALSE && bWriteDir == FALSE) {
			bWriteDir = TRUE; m_pXL->SetValue(stRow, 2, GetTextExamShearDir(nShearForceDir));
		}

		if (SisrD.nSeismicForceType == T_SIPA_D::kType2)
			nRow = bYZ_Dir ? nRow + 4 : nRow + 2;
		if (bYZ_Dir == TRUE && nShearForceDir == seis_jp::kY_Dir)
			nRow += 2;

		l_WriteData(nRow, nCol, SisrD);
	}

	auto nMin = SisrMin.size();
	for (int j = 0; j < nMin; ++j)
	{
		nRow = stRow;
		const T_SISR_SUMM_ITEM& SisrD = SisrMin[j];
		const int& nShearForceDir = (SisrD.nSeismicForceDirection == T_SIPA_D::kAxis_Dir) ? SisrD.nExamShearDir : (SisrD.nExamShearDir == seis_jp::kY_Dir ? seis_jp::kZ_Dir : seis_jp::kY_Dir);

		if (bYZ_Dir == FALSE && bWriteDir == FALSE) {
			bWriteDir = TRUE; m_pXL->SetValue(stRow, 2, GetTextExamShearDir(nShearForceDir));
		}

		if (SisrD.nSeismicForceType == T_SIPA_D::kType2)
			nRow = bYZ_Dir ? nRow + 4 : nRow + 2;
		if (bYZ_Dir == TRUE && nShearForceDir == seis_jp::kY_Dir)
			nRow += 2;

		l_WriteData(nRow+1, nCol, SisrD);
	}
}

void CSeisEvalReportSummary_A3::WriteCvsvData(int nRow, int nCol, const T_SIBD_CVSV_ITEM& SibdD)
{
	const double dRatio1 = SafeDiv(SibdD.dDeltaMax, SibdD.dDeltaY);
	const double dRatio2 = SafeDiv(SibdD.dDeltaMax, SibdD.dDeltaA);
	m_pXL->SetValue(nRow, nCol + 0, SibdD.kElem);
	WriteNanHyphenValue(nRow, nCol + 1, SibdD.dDeltaMax);
	//m_pXL->SetValue(nRow, nCol + 2, SibdD.dDeltaY);
	if (SibdD.dDeltaA == 0)	SetValue_Hyphen(nRow, nCol + 2);
	else					WriteNanHyphenValue(nRow, nCol + 2, SibdD.dDeltaA);
	WriteNanHyphenValue(nRow, nCol + 3, dRatio1);
	if (dRatio2 == 0)	SetValue_Hyphen(nRow, nCol + 4);
	else				WriteNanHyphenValue(nRow, nCol + 4, dRatio2);
	//m_pXL->SetValue(nRow, nCol + 6, dRatio1 > 1.0 ? _LS(IDS_BRDG_SEIS_EVAL_REPORT_JP_YIELD)/*_T("降伏")*/ : _T("OK"));
	m_pXL->SetValue(nRow, nCol + 5, dRatio2 > 1.0 ? _T("NG") : _T("OK"));

	// 		if (dRatio1 > 1.0) {
	// 			m_pXL->SetTextColor(nRow, nCol + 6, nRow, nCol + 6, SURRENDER_COLOR);
	// 		}
	if (dRatio2 > 1.0) {
		m_pXL->SetTextColor(nRow, nCol + 4, nRow, nCol + 4, CACODENON_RED);
		SetNG_Red(m_pXL, nRow, nCol + 5, nRow, nCol + 5);
	}
}

void CSeisEvalReportSummary_A3::WriteCvsvRebarCutData(int nRow, int nCol, const T_SIBD_CVSV_ITEM& SibdD)
{
	const double dRatio = SafeDiv(SibdD.dDeltaMax, SibdD.dDeltaY);
	m_pXL->SetValue(nRow, nCol + 0, SibdD.kElem);
	WriteNanHyphenValue(nRow, nCol + 1, SibdD.dDeltaMax);
	WriteNanHyphenValue(nRow, nCol + 2, SibdD.dDeltaY);
	WriteNanHyphenValue(nRow, nCol + 3, dRatio);
	m_pXL->SetValue(nRow, nCol + 4, dRatio > 1.0 ? _T("NG") : _T("OK"));
	if (dRatio > 1.0) {
		m_pXL->SetTextColor(nRow, nCol + 3, nRow, nCol + 3, CACODENON_RED);
		SetNG_Red(m_pXL, nRow, nCol + 4, nRow, nCol + 4);
	}
}

#pragma endregion

#pragma region // CSeisEvalReportSummary_A4
CSeisEvalReportSummary_A4::CSeisEvalReportSummary_A4(IExcel* pXL, CDgnProgressDlg* pProgress, const CString& strBasePath, const CString& strBaseBook, const SDgnExcelPrintOpt* pPrintOpt)
	: CWriteSeisEvalReportBase(pXL, pProgress, strBasePath, strBaseBook, pPrintOpt)
{
}

CSeisEvalReportSummary_A4::~CSeisEvalReportSummary_A4()
{
}

void CSeisEvalReportSummary_A4::GetCvsvItem(const CArray<T_SIBD_CVSV_ITEM, T_SIBD_CVSV_ITEM>& items, const int nMembType, T_SIGR_LOWER_D::eMaterialType eMatType, OUT std::vector<T_SIBD_CVSV_ITEM>& rData)
{
	int nCount = items.GetCount();
	for (int i = 0; i < nCount; i++)
	{
		const T_SIBD_CVSV_ITEM& item = items[i];

		if (item.nMemberType != nMembType) continue;
		if (item.nMaterialType != eMatType) continue;

		rData.push_back(item);
	}
}

void CSeisEvalReportSummary_A4::GetSisrItem(const CArray<T_SISR_SUMM_ITEM, T_SISR_SUMM_ITEM>& items, const int nMembType, T_SIGR_LOWER_D::eMaterialType eMatType, OUT std::vector<T_SISR_SUMM_ITEM>& rData)
{
	int nCount = items.GetCount();
	for (int i = 0; i < nCount; i++)
	{
		const T_SISR_SUMM_ITEM& item = items[i];

		if (item.nMemberType != nMembType) continue;
		if (item.nMaterialType != eMatType) continue;

		rData.push_back(item);
	}
}

void CSeisEvalReportSummary_A4::GetStelItem(const CArray<T_SIBD_STEL_ITEM, T_SIBD_STEL_ITEM>& items, const int nMembType, T_SIGR_LOWER_D::eMaterialType eMatType, OUT std::vector<T_SIBD_STEL_ITEM>& rData)
{
	int nCount = items.GetCount();
	for (int i = 0; i < nCount; i++)
	{
		const T_SIBD_STEL_ITEM& item = items[i];

		if (item.nMemberType != nMembType) continue;
		if (item.nMaterialType != eMatType) continue;

		rData.push_back(item);
	}
}

void CSeisEvalReportSummary_A4::AddStep()
{
	CArray<T_SIGR_K, T_SIGR_K> aSigrK;
	m_pDoc->m_pAttrCtrl2->GetQSigr()->GetKeyList(aSigrK);
	if (aSigrK.GetSize() < 1) return;
	
	//1.全体系の変位照査
	AddStep_WriteSummaryOverrallDisp(aSigrK);
	//2.橋脚別の変位照査
	AddStep_WriteSummaryPierDisp(aSigrK);
	//3.上部構造の照査
	AddStep_WriteSummaryUpper(aSigrK);
	//4.下部構造の照査
	AddStep_WriteSummaryLower(aSigrK);
}

void CSeisEvalReportSummary_A4::AddStep_WriteSummaryOverrallDisp(const CArray<T_SIGR_K, T_SIGR_K>& aSigrK)
{
	const auto nSigr = aSigrK.GetSize();
	for (int i = 0; i < nSigr; ++i)
	{
		const T_SIGR_K SigrK = aSigrK[i];

		T_SIGR_D SigrD;
		if (!m_pDoc->m_pAttrCtrl2->GetQSigr()->Get(SigrK, SigrD)) continue;

		T_EVAL_SUMM_OVERALL eval; //요약결과 (전체계 변위조사)
		if (!m_pResult->GetSummaryOverrall(SigrK, eval)) continue;
		if (eval.IsExistData() == FALSE) continue;

		m_pProgress->m_nTotalSteps++;
	}
}

void CSeisEvalReportSummary_A4::AddStep_WriteSummaryPierDisp(const CArray<T_SIGR_K, T_SIGR_K>& aSigrK)
{
	const auto nSigr = aSigrK.GetSize();
	for (int i = 0; i < nSigr; ++i)
	{
		const T_SIGR_K SigrK = aSigrK[i];

		T_SIGR_D SigrD;
		if (!m_pDoc->m_pAttrCtrl2->GetQSigr()->Get(SigrK, SigrD)) continue;

		T_EVAL_SUMM_PIER eval;
		if (!m_pResult->GetSummaryPier(SigrK, eval)) continue;
		if (eval.IsExistData() == FALSE) continue;

		m_pProgress->m_nTotalSteps++;
	}
}

void CSeisEvalReportSummary_A4::AddStep_WriteSummaryUpper(const CArray<T_SIGR_K, T_SIGR_K>& aSigrK)
{
	const auto nSigr = aSigrK.GetSize();
	for (int i = 0; i < nSigr; ++i)
	{
		const T_SIGR_K SigrK = aSigrK[i];

		T_SIGR_D SigrD;
		if (!m_pDoc->m_pAttrCtrl2->GetQSigr()->Get(SigrK, SigrD)) continue;

		T_EVAL_SUMM_UPPER eval;
		if (!m_pResult->GetSummaryUpper(SigrK, eval)) continue;
		if (eval.IsExistData() == FALSE) continue;

		m_pProgress->m_nTotalSteps++;
	}
}

void CSeisEvalReportSummary_A4::AddStep_WriteSummaryLower(const CArray<T_SIGR_K, T_SIGR_K>& aSigrK)
{
	if (AddStep_WriteSummaryLowerTitle(aSigrK) == FALSE) return;
	AddStep_WriteSummaryLowerColumn(aSigrK);
	AddStep_WriteSummaryLowerBeam(aSigrK);
	AddStep_WriteSummaryLowerSteel(aSigrK);
}

BOOL CSeisEvalReportSummary_A4::AddStep_WriteSummaryLowerTitle(const CArray<T_SIGR_K, T_SIGR_K>& aSigrK)
{
	const auto nSigr = aSigrK.GetSize();
	for (int i = 0; i < nSigr; ++i)
	{
		const T_SIGR_K SigrK = aSigrK[i];

		T_SIGR_D SigrD;
		if (!m_pDoc->m_pAttrCtrl2->GetQSigr()->Get(SigrK, SigrD)) continue;

		T_EVAL_SUMM_PIER eval;
		if (!m_pResult->GetSummaryPier(SigrK, eval)) continue;
		if (eval.IsExistData() == FALSE) continue;

		m_pProgress->m_nTotalSteps++;
		return TRUE;
	}
	return FALSE;
}

void CSeisEvalReportSummary_A4::AddStep_WriteSummaryLowerColumn(const CArray<T_SIGR_K, T_SIGR_K>& aSigrK)
{
	const auto nSigr = aSigrK.GetSize();
	for (int i = 0; i < nSigr; ++i)
	{
		const T_SIGR_K SigrK = aSigrK[i];

		T_SIGR_D SigrD;
		if (!m_pDoc->m_pAttrCtrl2->GetQSigr()->Get(SigrK, SigrD)) continue;

		T_EVAL_SUMM_LOWER eval;
		if (!m_pResult->GetSummaryLower(SigrK, eval)) continue;

		// 곡률
		std::vector<T_SIBD_CVSV_ITEM> CvsvMin, CvsvMax;
		GetCvsvItem(eval.CvsvSum.CvsvMin, DB_MEMB_TYPE::D_MBTP_COLUMN, T_SIGR_LOWER_D::eMaterialType::kRC, CvsvMin);
		GetCvsvItem(eval.CvsvSum.CvsvMax, DB_MEMB_TYPE::D_MBTP_COLUMN, T_SIGR_LOWER_D::eMaterialType::kRC, CvsvMax);

		// 전단
		std::vector<T_SISR_SUMM_ITEM> SisrMin, SisrMax;
		GetSisrItem(eval.SisrSum.SisrMin, DB_MEMB_TYPE::D_MBTP_COLUMN, T_SIGR_LOWER_D::eMaterialType::kRC, SisrMin);
		GetSisrItem(eval.SisrSum.SisrMax, DB_MEMB_TYPE::D_MBTP_COLUMN, T_SIGR_LOWER_D::eMaterialType::kRC, SisrMax);

		if (CvsvMin.size() < 1 && CvsvMax.size() < 1 && SisrMin.size() < 1 && SisrMax.size() < 1)
			continue;

		m_pProgress->m_nTotalSteps++;
	}
}

void CSeisEvalReportSummary_A4::AddStep_WriteSummaryLowerBeam(const CArray<T_SIGR_K, T_SIGR_K>& aSigrK)
{
	const auto nSigr = aSigrK.GetSize();
	for (int i = 0; i < nSigr; ++i)
	{
		const T_SIGR_K SigrK = aSigrK[i];

		T_SIGR_D SigrD;
		if (!m_pDoc->m_pAttrCtrl2->GetQSigr()->Get(SigrK, SigrD)) continue;

		T_EVAL_SUMM_LOWER eval;
		if (!m_pResult->GetSummaryLower(SigrK, eval)) continue;

		// 곡률
		std::vector<T_SIBD_CVSV_ITEM> CvsvMin, CvsvMax;
		GetCvsvItem(eval.CvsvSum.CvsvMin, DB_MEMB_TYPE::D_MBTP_BEAM, T_SIGR_LOWER_D::eMaterialType::kRC, CvsvMin);
		GetCvsvItem(eval.CvsvSum.CvsvMax, DB_MEMB_TYPE::D_MBTP_BEAM, T_SIGR_LOWER_D::eMaterialType::kRC, CvsvMax);

		// 전단
		std::vector<T_SISR_SUMM_ITEM> SisrMin, SisrMax;
		GetSisrItem(eval.SisrSum.SisrMin, DB_MEMB_TYPE::D_MBTP_BEAM, T_SIGR_LOWER_D::eMaterialType::kRC, SisrMin);
		GetSisrItem(eval.SisrSum.SisrMax, DB_MEMB_TYPE::D_MBTP_BEAM, T_SIGR_LOWER_D::eMaterialType::kRC, SisrMax);

		if (CvsvMin.size() < 1 && CvsvMax.size() < 1 && SisrMin.size() < 1 && SisrMax.size() < 1)
			continue;

		m_pProgress->m_nTotalSteps++;
	}
}

void CSeisEvalReportSummary_A4::AddStep_WriteSummaryLowerSteel(const CArray<T_SIGR_K, T_SIGR_K>& aSigrK)
{
	const auto nSigr = aSigrK.GetSize();
	for (int i = 0; i < nSigr; ++i)
	{
		const T_SIGR_K SigrK = aSigrK[i];

		T_SIGR_D SigrD;
		if (!m_pDoc->m_pAttrCtrl2->GetQSigr()->Get(SigrK, SigrD)) continue;

		T_EVAL_SUMM_LOWER eval;
		if (!m_pResult->GetSummaryLower(SigrK, eval)) continue;

		// 곡률
		std::vector<T_SIBD_STEL_ITEM> CvsvColumn, CvsvBeam;
		GetStelItem(eval.StelSum, DB_MEMB_TYPE::D_MBTP_COLUMN, T_SIGR_LOWER_D::eMaterialType::kSteel, CvsvColumn);
		GetStelItem(eval.StelSum, DB_MEMB_TYPE::D_MBTP_BEAM, T_SIGR_LOWER_D::eMaterialType::kSteel, CvsvBeam);

		if (CvsvColumn.size() < 1 && CvsvBeam.size() < 1)
			continue;

		m_pProgress->m_nTotalSteps++;
	}
}

void CSeisEvalReportSummary_A4::Write()
{
	CArray<T_SIGR_K, T_SIGR_K> aSigrK;
	m_pDoc->m_pAttrCtrl2->GetQSigr()->GetKeyList(aSigrK);
	if (aSigrK.GetSize() < 1) return;

	AddSheet();

	CString RptBlock;
	int rowCnt = 0;
	int stRow = 0;
	short Row_Ref = 0, Col_Ref = 0;

	RptBlock = _T("_00.Summ_Title");
	rowCnt = CopyRange(m_pXL, RptBlock, stRow);
	stRow += rowCnt;

	int iParagrah = 0;

	//全体系の変位照査
	WriteSummaryOverrallDisp(m_pXL, aSigrK, stRow, iParagrah);

	//橋脚別の変位照査
	WriteSummaryPierDisp(m_pXL, aSigrK, stRow, iParagrah);

	//上部構造の照査
	WriteSummaryUpper(m_pXL, aSigrK, stRow, iParagrah);

	//下部構造の照査
	WriteSummaryLower(m_pXL, aSigrK, stRow, iParagrah);
}

void CSeisEvalReportSummary_A4::AddSheet()
{
	m_strBaseSheet = _LSX(00.Summ_A4);
	const CString strNewSheet = _LS(IDS_BRDG_SEIS_EVAL_REPORT_JP_SUMMARY);// _T("要約結果");
	if (m_pXL->IsExistSheetName(strNewSheet))
	{
		ASSERT(0);
		m_pXL->SetActiveSheet(strNewSheet);
	}
	else
	{
		m_pXL->AddSheet2(strNewSheet);
		m_pXL->SetActiveSheet(strNewSheet);
	}

	m_pXL->SetStandardFont(_T("Meiryo UI"));
	m_pXL->SetMargin(13, 12, 20, 17);
	m_pXL->SetColumnWidth(0, 1, 0, 60, 7.000);
	m_pXL->SetColumnWidth(0, 0, 0, 0, 2.546);
	m_pXL->SetRowHeight(16.5);
	m_pXL->SetZoom(75);
}

void CSeisEvalReportSummary_A4::WriteSummaryOverrallDisp(IExcel* pXL, const CArray<T_SIGR_K, T_SIGR_K>& aSigrK, OUT int& stRow, OUT int& iParagraph)
{
	const auto nSigr = aSigrK.GetSize();

	auto I_WriteTitle = [&pXL, this](OUT int& stRow, OUT int& iParagraph)
		{
			CString RptBlock = _T("_00.Summ_1.All");
			int rowCnt = CopyRange(pXL, RptBlock, stRow);
			pXL->SetValue(stRow, 0, GetText(_T("%d."), ++iParagraph));
			stRow += rowCnt;
		};

	auto l_WriteSimd = [&pXL, this](int nRow, int nCol, const T_SIMD_SUMM_ITEM& summD)
		{
			if (summD.nSeismicForceDirection == T_SIPA_D::kAxis_Dir)
			{
				if (summD.nSeismicForceType == T_SIPA_D::kType2) nCol += 1;
			}
			else
			{
				if (summD.nSeismicForceType == T_SIPA_D::kType1) nCol += 2;
				else                                             nCol += 3;
			}
			const double dRatio = SafeDiv(summD.dDeltaMax, summD.dDeltaA);
			pXL->SetValue(nRow + 0, nCol, summD.dDeltaMax * m_dLenToCM);
			pXL->SetValue(nRow + 1, nCol, summD.dDeltaA * m_dLenToCM);
			pXL->SetValue(nRow + 2, nCol, dRatio);
			pXL->SetValue(nRow + 3, nCol, dRatio > 1.0 ? _T("NG") : _T("OK"));

			if (dRatio > 1.0) {
				pXL->SetTextColor(nRow + 2, nCol, nRow + 2, nCol, CACODENON_RED);
				SetNG_Red(pXL, nRow + 3, nCol, nRow + 3, nCol);
			}
		};

	auto l_WriteSird = [&pXL, this](int nRow, int nCol, const T_SIRD_SUMM_ITEM& summD)
		{
			if (summD.nSeismicForceDirection == T_SIPA_D::kAxis_Dir)
			{
				if (summD.nSeismicForceType == T_SIPA_D::kType2) nCol += 1;
			}
			else
			{
				if (summD.nSeismicForceType == T_SIPA_D::kType1) nCol += 2;
				else                                             nCol += 3;
			}
			const double dRatio = abs(SafeDiv(summD.dDeltaR, summD.dDrataRa));
			pXL->SetValue(nRow + 0, nCol, summD.strPierName);
			pXL->SetValue(nRow + 1, nCol, summD.dMuR);
			if(summD.dDeltaR == 0) SetValue_Hyphen(nRow + 2, nCol);
			else			  pXL->SetValue(nRow + 2, nCol, summD.dDeltaR * m_dLenToCM);
			pXL->SetValue(nRow + 3, nCol, summD.dDrataRa * m_dLenToCM);
			if (dRatio == 0) {
				SetValue_Hyphen(nRow + 4, nCol);
				SetValue_Hyphen(nRow + 5, nCol);
			}
			else {
				pXL->SetValue(nRow + 4, nCol, dRatio);
				pXL->SetValue(nRow + 5, nCol, dRatio > 1.0 ? _T("NG") : _T("OK"));
			}

			if (dRatio > 1.0) {
				pXL->SetTextColor(nRow + 4, nCol, nRow + 4, nCol, CACODENON_RED);
				SetNG_Red(pXL, nRow + 5, nCol, nRow + 5, nCol);
			}
		};

	BOOL bWrite = FALSE;
	for (int i = 0; i < nSigr; ++i)
	{
		const T_SIGR_K SigrK = aSigrK[i];

		T_SIGR_D SigrD;
		if (!m_pDoc->m_pAttrCtrl2->GetQSigr()->Get(SigrK, SigrD)) continue;

		T_EVAL_SUMM_OVERALL eval; //요약결과 (전체계 변위조사)
		if (!m_pResult->GetSummaryOverrall(SigrK, eval)) continue;
		if (eval.IsExistData() == FALSE) continue;

		if (bWrite == FALSE) {
			I_WriteTitle(stRow, iParagraph);
			bWrite = TRUE;
		}

		CString RptBlock = _T("_00.Summ_1.All_Table");
		int rowCnt = CopyRange(pXL, RptBlock, stRow);

		pXL->SetValue(stRow, 4, SigrD.strName);

		// Simd summary
		auto nLeft = eval.SimdSum.Left.GetSize();
		for (int j = 0; j < nLeft; ++j)
		{
			const auto& SimdD = eval.SimdSum.Left[j];
			l_WriteSimd(stRow + 5, 4, SimdD);
		}
		auto nRight = eval.SimdSum.Right.GetSize();
		for (int j = 0; j < nRight; ++j)
		{
			const auto& SimdD = eval.SimdSum.Right[j];
			l_WriteSimd(stRow + 5, 8, SimdD);
		}

		// Sird summary
		nLeft = eval.SirdSum.Left.GetSize();
		for (int j = 0; j < nLeft; ++j)
		{
			const auto& SirdD = eval.SirdSum.Left[j];
			l_WriteSird(stRow + 10, 4, SirdD);
		}
		nRight = eval.SirdSum.Right.GetSize();
		for (int j = 0; j < nRight; ++j)
		{
			const auto& SirdD = eval.SirdSum.Right[j];
			l_WriteSird(stRow + 10, 8, SirdD);
		}
		stRow += rowCnt;
		stRow++;
		if (m_pProgress->NextStep() == FALSE) throw FALSE;
	}
	if (bWrite == TRUE)
		stRow++;
}

void CSeisEvalReportSummary_A4::WriteSummaryPierDisp(IExcel* pXL, const CArray<T_SIGR_K, T_SIGR_K>& aSigrK, OUT int& stRow, OUT int& iParagraph)
{
	const auto nSigr = aSigrK.GetSize();

	auto I_WriteTitle = [&pXL, this](OUT int& stRow, OUT int& iParagraph)
		{
			CString RptBlock = _T("_00.Summ_2.SirdLower");
			int rowCnt = CopyRange(pXL, RptBlock, stRow);
			pXL->SetValue(stRow, 0, GetText(_T("%d."), ++iParagraph));
			stRow += rowCnt;
		};

	auto l_WriteSimdLower = [&pXL, this](int nRow, int nCol, const T_SIMD_SUMM_ITEM& summD)
		{
			if (summD.nSeismicForceDirection == T_SIPA_D::kAxis_Dir)
			{
				if (summD.nSeismicForceType == T_SIPA_D::kType2) nCol += 1;
			}
			else
			{
				if (summD.nSeismicForceType == T_SIPA_D::kType1) nCol += 2;
				else                                             nCol += 3;
			}
			const double dRatio = SafeDiv(summD.dDeltaMax, summD.dDeltaA);
			pXL->SetValue(nRow + 0, nCol, summD.dDeltaMax * m_dLenToCM);
			pXL->SetValue(nRow + 1, nCol, summD.dDeltaA * m_dLenToCM);
			pXL->SetValue(nRow + 2, nCol, dRatio);
			pXL->SetValue(nRow + 3, nCol, dRatio > 1.0 ? _T("NG") : _T("OK"));

			if (dRatio > 1.0) {
				pXL->SetTextColor(nRow + 2, nCol, nRow + 2, nCol, CACODENON_RED);
				SetNG_Red(pXL, nRow + 3, nCol, nRow + 3, nCol);
			}
		};

	auto l_WriteSirdLower = [&pXL, this](int nRow, int nCol, const T_SIRD_SUMM_ITEM& summD)
		{
			if (summD.nSeismicForceDirection == T_SIPA_D::kAxis_Dir)
			{
				if (summD.nSeismicForceType == T_SIPA_D::kType2) nCol += 1;
			}
			else
			{
				if (summD.nSeismicForceType == T_SIPA_D::kType1) nCol += 2;
				else                                             nCol += 3;
			}
			const double dRatio = abs(SafeDiv(summD.dDeltaR, summD.dDrataRa));
									pXL->SetValue(nRow + 0, nCol, summD.strColumnName);
									pXL->SetValue(nRow + 1, nCol, summD.dMuR);
			if(summD.dDeltaR == 0)	SetValue_Hyphen(nRow+2, nCol);
			else					pXL->SetValue(nRow + 2, nCol, summD.dDeltaR * m_dLenToCM);
									pXL->SetValue(nRow + 3, nCol, summD.dDrataRa * m_dLenToCM);
			if (dRatio == 0) {
									SetValue_Hyphen(nRow + 4, nCol);
									SetValue_Hyphen(nRow + 5, nCol);
			}
			else {
									pXL->SetValue(nRow + 4, nCol, dRatio);
									pXL->SetValue(nRow + 5, nCol, dRatio > 1.0 ? _T("NG") : _T("OK"));
			}

			if (dRatio > 1.0) {
									pXL->SetTextColor(nRow + 4, nCol, nRow + 4, nCol, CACODENON_RED);
									SetNG_Red(pXL, nRow + 5, nCol, nRow + 5, nCol);
			}
		};

	int nCol = 4;
	int nTableRow = 0;
	BOOL bWrite = FALSE;
	for (int i = 0; i < nSigr; ++i)
	{
		const T_SIGR_K SigrK = aSigrK[i];

		T_SIGR_D SigrD;
		if (!m_pDoc->m_pAttrCtrl2->GetQSigr()->Get(SigrK, SigrD)) continue;

		T_EVAL_SUMM_PIER eval;
		if (!m_pResult->GetSummaryPier(SigrK, eval)) continue;
		if (eval.IsExistData() == FALSE) continue;

		if (bWrite == FALSE) {
			I_WriteTitle(stRow, iParagraph);
			bWrite = TRUE;
		}

		if (nCol == 4) {
			CString RptBlock = _T("_00.Summ_2.SirdLower_Table");
			nTableRow = CopyRange(pXL, RptBlock, stRow);
		}

		pXL->SetValue(stRow, nCol, SigrD.strName);

		int nCount = eval.SimdSum.GetCount();
		for (int j = 0; j < nCount; ++j)
		{
			const auto& SimdD = eval.SimdSum[j];
			l_WriteSimdLower(stRow + 4, nCol, SimdD);
		}

		nCount = eval.SirdSum.GetCount();
		for (int j = 0; j < nCount; ++j)
		{
			const auto& SirdD = eval.SirdSum[j];
			l_WriteSirdLower(stRow + 9, nCol, SirdD);
		}

		if (nCol < 8)
			nCol += 4;
		else {
			nCol = 4;
			stRow += nTableRow;
			nTableRow = 0;
			stRow++;
		}

		if (m_pProgress->NextStep() == FALSE) throw FALSE;
	}
	if (bWrite == TRUE) {
		if (nTableRow != 0) {
			stRow += nTableRow; 
			nTableRow = 0;
		}
		stRow++;
}
}

void CSeisEvalReportSummary_A4::WriteSummaryUpper(IExcel* pXL, const CArray<T_SIGR_K, T_SIGR_K>& aSigrK, OUT int& stRow, OUT int& iParagraph)
{
	const auto nSigr = aSigrK.GetSize();

	auto I_WriteTitle = [&pXL, this](OUT int& stRow, OUT int& iParagraph)
		{
			CString RptBlock = _T("_00.Summ_2.Upper");
			int rowCnt = CopyRange(pXL, RptBlock, stRow);
			pXL->SetValue(stRow, 0, GetText(_T("%d."), ++iParagraph));
			stRow += rowCnt;
		};

	BOOL bWrite = FALSE;
	for (int i = 0; i < nSigr; ++i)
	{
		const T_SIGR_K SigrK = aSigrK[i];

		T_SIGR_D SigrD;
		if (!m_pDoc->m_pAttrCtrl2->GetQSigr()->Get(SigrK, SigrD)) continue;

		T_EVAL_SUMM_UPPER eval;
		if (!m_pResult->GetSummaryUpper(SigrK, eval)) continue;
		if (eval.IsExistData() == FALSE) continue;

		if (bWrite == FALSE) {
			I_WriteTitle(stRow, iParagraph);
			bWrite = TRUE;
		}

		CString RptBlock = _T("_00.Summ_2.Upper_Table");
		int rowCnt = CopyRange(pXL, RptBlock, stRow);

		pXL->SetValue(stRow, 4, SigrD.strName);

		// Sibd Cvsv summary; curvature
		auto nMin = eval.CvsvSum.CvsvMin.GetSize();
		for (int j = 0; j < nMin; ++j)
		{
			const auto& SibdD = eval.CvsvSum.CvsvMin[j];
			WriteDataCurvature(pXL, stRow + 5, 4, SibdD.nSeismicForceDirection, SibdD.nSeismicForceType, SibdD.kElem, SibdD);
		}
		auto nMax = eval.CvsvSum.CvsvMax.GetSize();
		for (int j = 0; j < nMax; ++j)
		{
			const auto& SibdD = eval.CvsvSum.CvsvMax[j];
			WriteDataCurvature(pXL, stRow + 5, 8, SibdD.nSeismicForceDirection, SibdD.nSeismicForceType, SibdD.kElem, SibdD);
		}

		// Sibd Srsv summary; moment
		nMin = eval.SrsvSum.SrsvMin.GetSize();
		for (int j = 0; j < nMin; ++j)
		{
			const auto& SibdD = eval.SrsvSum.SrsvMin[j];
			WriteDataMoment(pXL, stRow + 14, 4, SibdD.nSeismicForceDirection, SibdD.nSeismicForceType, SibdD.kElem, SibdD);
		}
		nMax = eval.SrsvSum.SrsvMax.GetSize();
		for (int j = 0; j < nMax; ++j)
		{
			const auto& SibdD = eval.SrsvSum.SrsvMax[j];
			WriteDataMoment(pXL, stRow + 14, 8, SibdD.nSeismicForceDirection, SibdD.nSeismicForceType, SibdD.kElem, SibdD);
		}

		// Sisr summary; shear
		nMin = eval.SisrSum.SisrMin.GetSize();
		for (int j = 0; j < nMin; ++j)
		{
			const auto& SisrD = eval.SisrSum.SisrMin[j];
			WriteDataShear(pXL, stRow + 23, 4, SisrD.nSeismicForceDirection, SisrD.nSeismicForceType, SisrD.kElem, SisrD);
		}
		nMax = eval.SisrSum.SisrMax.GetSize();
		for (int j = 0; j < nMax; ++j)
		{
			const auto& SisrD = eval.SisrSum.SisrMax[j];
			WriteDataShear(pXL, stRow + 23, 8, SisrD.nSeismicForceDirection, SisrD.nSeismicForceType, SisrD.kElem, SisrD);
		}

		stRow += rowCnt;
		stRow++;
		if (m_pProgress->NextStep() == FALSE) throw FALSE;
	}
	if (bWrite == TRUE)
		stRow++;
}

void CSeisEvalReportSummary_A4::WriteDataCurvature(IExcel* pXL, int nRow, int nCol, const int& nDir, const int& nType, const int& kElem, const T_SIBD_CVSV_ITEM& item)
{
	if (nDir == T_SIPA_D::kAxis_Dir)
	{
		if (nType == T_SIPA_D::kType2) nCol += 1;
	}
	else
	{
		if (nType == T_SIPA_D::kType1) nCol += 2;
		else                           nCol += 3;
	}

	const double dRatio1 = SafeDiv(item.dDeltaMax, item.dDeltaY);
	const double dRatio2 = SafeDiv(item.dDeltaMax, item.dDeltaA);
	pXL->SetValue(nRow + 0, nCol, kElem);
	pXL->SetValue(nRow + 1, nCol, item.dDeltaMax);
	pXL->SetValue(nRow + 2, nCol, item.dDeltaY);
	if(item.dDeltaA == 0)	SetValue_Hyphen(nRow+3, nCol);
	else					pXL->SetValue(nRow + 3, nCol, item.dDeltaA);
	pXL->SetValue(nRow + 4, nCol, dRatio1);
	if(dRatio2 == 0)	SetValue_Hyphen(nRow + 5, nCol);
	else				pXL->SetValue(nRow + 5, nCol, dRatio2);
	pXL->SetValue(nRow + 6, nCol, dRatio1 > 1.0 ? _LS(IDS_BRDG_SEIS_EVAL_REPORT_JP_YIELD)/*_T("降伏")*/ : _T("OK"));
	pXL->SetValue(nRow + 7, nCol, dRatio2 > 1.0 ? _T("NG") : _T("OK"));

	if (dRatio1 > 1.0) {
		//pXL->SetPattern(nRow + 6, nCol, nRow + 6, nCol, libxl::FILLPATTERN_SOLID, false, true, PINK_DIAMOND, PINK_DIAMOND);
		pXL->SetTextColor(nRow + 6, nCol, nRow + 6, nCol, SURRENDER_COLOR);
	}
	if (dRatio2 > 1.0) {
		pXL->SetTextColor(nRow + 5, nCol, nRow + 5, nCol, CACODENON_RED);
		SetNG_Red(pXL, nRow + 7, nCol, nRow + 7, nCol);
	}
}

void CSeisEvalReportSummary_A4::WriteDataMoment(IExcel* pXL, int nRow, int nCol, const int& nDir, const int& nType, const int& kElem, const T_SIBD_SRSV_ITEM& item)
{
	if (nDir == T_SIPA_D::kAxis_Dir)
	{
		if (nType == T_SIPA_D::kType2) nCol += 1;
	}
	else
	{
		if (nType == T_SIPA_D::kType1) nCol += 2;
		else                          nCol += 3;
	}

	const double dRatio1 = SafeDiv(item.dMmax, item.dMy);
	const double dRatio2 = SafeDiv(item.dMmax, item.dMu);
	pXL->SetValue(nRow + 0, nCol, kElem);
	pXL->SetValue(nRow + 1, nCol, item.dMmax);
	pXL->SetValue(nRow + 2, nCol, item.dMy);
	pXL->SetValue(nRow + 3, nCol, item.dMu);
	pXL->SetValue(nRow + 4, nCol, dRatio1);
	pXL->SetValue(nRow + 5, nCol, dRatio2);
	pXL->SetValue(nRow + 6, nCol, dRatio1 > 1.0 ? _LS(IDS_BRDG_SEIS_EVAL_REPORT_JP_YIELD)/*_T("降伏")*/ : _T("OK"));
	pXL->SetValue(nRow + 7, nCol, dRatio2 > 1.0 ? _T("NG") : _T("OK"));

	if (dRatio1 > 1.0) {
		//pXL->SetPattern(nRow + 6, nCol, nRow + 6, nCol, libxl::FILLPATTERN_SOLID, false, true, PINK_DIAMOND, PINK_DIAMOND);
		pXL->SetTextColor(nRow + 6, nCol, nRow + 6, nCol, SURRENDER_COLOR);
	}
	if (dRatio2 > 1.0) {
		pXL->SetTextColor(nRow + 5, nCol, nRow + 5, nCol, CACODENON_RED);
		SetNG_Red(pXL, nRow + 7, nCol, nRow + 7, nCol);
	}
}

void CSeisEvalReportSummary_A4::WriteDataShear(IExcel* pXL, int nRow, int nCol, const int& nDir, const int& nType, const int& kElem, const T_SISR_SUMM_ITEM& item)
{
	if (nDir == T_SIPA_D::kAxis_Dir)
	{
		if (nType == T_SIPA_D::kType2) nCol += 1;
	}
	else
	{
		if (nType == T_SIPA_D::kType1) nCol += 2;
		else                          nCol += 3;
	}

	const double dRatio1 = SafeDiv(item.dSmax, item.dSus);
	const double dRatio2 = SafeDiv(item.dSmax, item.dSuc);
	pXL->SetValue(nRow + 0, nCol, kElem);
	pXL->SetValue(nRow + 1, nCol, item.dSmax);
	pXL->SetValue(nRow + 2, nCol, item.dSus);
	pXL->SetValue(nRow + 3, nCol, item.dSuc);
	pXL->SetValue(nRow + 4, nCol, dRatio1);
	pXL->SetValue(nRow + 5, nCol, dRatio2);
	pXL->SetValue(nRow + 6, nCol, dRatio1 > 1.0 ? _LS(IDS_BRDG_SEIS_EVAL_REPORT_JP_YIELD)/*_T("降伏")*/ : _T("OK"));
	pXL->SetValue(nRow + 7, nCol, dRatio2 > 1.0 ? _T("NG") : _T("OK"));

	if (dRatio1 > 1.0) {
		pXL->SetTextColor(nRow + 6, nCol, nRow + 6, nCol, SURRENDER_COLOR);
	}
	if (dRatio2 > 1.0)
	{
		pXL->SetTextColor(nRow + 5, nCol, nRow + 5, nCol, CACODENON_RED);
		SetNG_Red(pXL, nRow + 7, nCol, nRow + 7, nCol);
	}
}

void CSeisEvalReportSummary_A4::WriteSummaryLower(IExcel* pXL, const CArray<T_SIGR_K, T_SIGR_K>& aSigrK, OUT int& stRow, OUT int& iParagraph)
{
	if (WriteSummaryLowerTitle(pXL, aSigrK, stRow, iParagraph) == FALSE) return;

	WriteSummaryLowerColumn(pXL, aSigrK, stRow, iParagraph);

	WriteSummaryLowerBeam(pXL, aSigrK, stRow, iParagraph);

	WriteSummaryLowerSteel(pXL, aSigrK, stRow, iParagraph);
}

BOOL CSeisEvalReportSummary_A4::WriteSummaryLowerTitle(IExcel* pXL, const CArray<T_SIGR_K, T_SIGR_K>& aSigrK, OUT int& stRow, OUT int& iParagraph)
{
	const auto nSigr = aSigrK.GetSize();

	auto I_WriteTitle = [&pXL, this](OUT int& stRow, OUT int& iParagraph)
		{
			CString RptBlock = _T("_00.Summ_3.Lower");
			int rowCnt = CopyRange(pXL, RptBlock, stRow);
			pXL->SetValue(stRow, 0, GetText(_T("%d."), ++iParagraph));
			stRow += rowCnt;
		};

	for (int i = 0; i < nSigr; ++i)
	{
		const T_SIGR_K SigrK = aSigrK[i];

		T_SIGR_D SigrD;
		if (!m_pDoc->m_pAttrCtrl2->GetQSigr()->Get(SigrK, SigrD)) continue;

		T_EVAL_SUMM_LOWER eval;
		if (!m_pResult->GetSummaryLower(SigrK, eval)) continue;
		if (eval.IsExistData() == FALSE) continue;

		I_WriteTitle(stRow, iParagraph);
		return TRUE;
	}
	return FALSE;
}

void CSeisEvalReportSummary_A4::WriteCvsvItems(IExcel* pXL, const std::vector<T_SIBD_CVSV_ITEM>& CvsvMin, const std::vector<T_SIBD_CVSV_ITEM>& CvsvMax, OUT int& stRow)
{
	auto l_WriteData = [&pXL](int nRow, int nCol, const int& nDir, const int& nType,
		const int& iMemb, const double& dDemand, const double& dCapacity1, const double& dCapacity2)
		{
			if (nDir == T_SIPA_D::kAxis_Dir)
			{
				if (nType == T_SIPA_D::kType2) nCol += 1;
			}
			else
			{
				if (nType == T_SIPA_D::kType1) nCol += 2;
				else                          nCol += 3;
			}

			const double dRatio1 = SafeDiv(dDemand, dCapacity1);
			const double dRatio2 = SafeDiv(dDemand, dCapacity2);
			pXL->SetValue(nRow + 0, nCol, iMemb);
			pXL->SetValue(nRow + 1, nCol, dDemand);
			pXL->SetValue(nRow + 2, nCol, dCapacity1);
			pXL->SetValue(nRow + 3, nCol, dCapacity2);
			pXL->SetValue(nRow + 4, nCol, dRatio1);
			pXL->SetValue(nRow + 5, nCol, dRatio2);
			pXL->SetValue(nRow + 6, nCol, dRatio1 > 1.0 ? _LS(IDS_BRDG_SEIS_EVAL_REPORT_JP_YIELD)/*_T("降伏")*/ : _T("OK"));
			pXL->SetValue(nRow + 7, nCol, dRatio2 > 1.0 ? _T("NG") : _T("OK"));

			if (dRatio1 > 1.0) {
				//pXL->SetPattern(nRow + 6, nCol, nRow + 6, nCol, libxl::FILLPATTERN_SOLID, false, true, PINK_DIAMOND, PINK_DIAMOND);
				pXL->SetTextColor(nRow + 6, nCol, nRow + 6, nCol, SURRENDER_COLOR);
			}
			if (dRatio2 > 1.0) {
				pXL->SetTextColor(nRow + 5, nCol, nRow + 5, nCol, CACODENON_RED);
				SetNG_Red(pXL, nRow + 7, nCol, nRow + 7, nCol);
			}
		};

	auto nMin = CvsvMin.size();
	for (int j = 0; j < nMin; ++j)
	{
		const auto& SibdD = CvsvMin[j];
		l_WriteData(stRow + 5, 4, SibdD.nSeismicForceDirection, SibdD.nSeismicForceType, SibdD.kElem, SibdD.dDeltaMax, SibdD.dDeltaY, SibdD.dDeltaA);
	}
	auto nMax = CvsvMax.size();
	for (int j = 0; j < nMax; ++j)
	{
		const auto& SibdD = CvsvMax[j];
		l_WriteData(stRow + 5, 8, SibdD.nSeismicForceDirection, SibdD.nSeismicForceType, SibdD.kElem, SibdD.dDeltaMax, SibdD.dDeltaY, SibdD.dDeltaA);
	}
}

void CSeisEvalReportSummary_A4::WriteSisrItems(IExcel* pXL, const std::vector<T_SISR_SUMM_ITEM>& SisrMin, const std::vector<T_SISR_SUMM_ITEM>& SisrMax, OUT int& stRow)
{
	auto l_WriteData = [&pXL](int nRow, int nCol, const int& nDir, const int& nType,
		const int& iMemb, const double& dDemand, const double& dCapacity1)
		{
			if (nDir == T_SIPA_D::kAxis_Dir)
			{
				if (nType == T_SIPA_D::kType2) nCol += 1;
			}
			else
			{
				if (nType == T_SIPA_D::kType1) nCol += 2;
				else                          nCol += 3;
			}

			const double dRatio = SafeDiv(dDemand, dCapacity1);
			pXL->SetValue(nRow + 0, nCol, iMemb);
			pXL->SetValue(nRow + 1, nCol, dDemand);
			pXL->SetValue(nRow + 2, nCol, dCapacity1);
			pXL->SetValue(nRow + 3, nCol, dRatio);
			pXL->SetValue(nRow + 4, nCol, dRatio > 1.0 ? _T("NG") : _T("OK"));

			if (dRatio > 1.0) {
				pXL->SetTextColor(nRow + 3, nCol, nRow + 3, nCol, CACODENON_RED);
				SetNG_Red(pXL, nRow + 4, nCol, nRow + 4, nCol);
			}
		};

	auto nMin = SisrMin.size();
	for (int j = 0; j < nMin; ++j)
	{
		const auto& SisrD = SisrMin[j];
		l_WriteData(stRow + 14, 4, SisrD.nSeismicForceDirection, SisrD.nSeismicForceType, SisrD.kElem, SisrD.dSmax, SisrD.dPs);
	}
	auto nMax = SisrMax.size();
	for (int j = 0; j < nMax; ++j)
	{
		const auto& SisrD = SisrMax[j];
		l_WriteData(stRow + 14, 8, SisrD.nSeismicForceDirection, SisrD.nSeismicForceType, SisrD.kElem, SisrD.dSmax, SisrD.dPs);
	}
}

void CSeisEvalReportSummary_A4::WriteSummaryLowerColumn(IExcel* pXL, const CArray<T_SIGR_K, T_SIGR_K>& aSigrK, OUT int& stRow, OUT int& iParagraph)
{
	BOOL bWrite = FALSE;
	const auto nSigr = aSigrK.GetSize();
	for (int i = 0; i < nSigr; ++i)
	{
		const T_SIGR_K SigrK = aSigrK[i];

		T_SIGR_D SigrD;
		if (!m_pDoc->m_pAttrCtrl2->GetQSigr()->Get(SigrK, SigrD)) continue;

		T_EVAL_SUMM_LOWER eval;
		if (!m_pResult->GetSummaryLower(SigrK, eval)) continue;

		// 곡률
		std::vector<T_SIBD_CVSV_ITEM> CvsvMin, CvsvMax;
		GetCvsvItem(eval.CvsvSum.CvsvMin, DB_MEMB_TYPE::D_MBTP_COLUMN, T_SIGR_LOWER_D::eMaterialType::kRC, CvsvMin);
		GetCvsvItem(eval.CvsvSum.CvsvMax, DB_MEMB_TYPE::D_MBTP_COLUMN, T_SIGR_LOWER_D::eMaterialType::kRC, CvsvMax);

		// 전단
		std::vector<T_SISR_SUMM_ITEM> SisrMin, SisrMax;
		GetSisrItem(eval.SisrSum.SisrMin, DB_MEMB_TYPE::D_MBTP_COLUMN, T_SIGR_LOWER_D::eMaterialType::kRC, SisrMin);
		GetSisrItem(eval.SisrSum.SisrMax, DB_MEMB_TYPE::D_MBTP_COLUMN, T_SIGR_LOWER_D::eMaterialType::kRC, SisrMax);

		if (CvsvMin.size() < 1 && CvsvMax.size() < 1 && SisrMin.size() < 1 && SisrMax.size() < 1)
			continue;

		CString RptBlock = _T("_00.Summ_3.Lower_Table");
		int rowCnt = CopyRange(pXL, RptBlock, stRow);

		pXL->SetValue(stRow, 4, GetText(_T("%s(%s)"),
			SigrD.strName, _LS(IDS_BRDG_SEIS_EVAL_REPORT_JP_COLUMN)));

		// Sibd Cvsv summary; curvature
		WriteCvsvItems(pXL, CvsvMin, CvsvMax, stRow);

		// Sisr summary; shear
		WriteSisrItems(pXL, SisrMin, SisrMax, stRow);

		stRow += rowCnt;
		stRow++;
		bWrite = TRUE;
		if (m_pProgress->NextStep() == FALSE) throw FALSE;
	}
	if(bWrite == TRUE)
		stRow++;
}

void CSeisEvalReportSummary_A4::WriteSummaryLowerBeam(IExcel* pXL, const CArray<T_SIGR_K, T_SIGR_K>& aSigrK, OUT int& stRow, OUT int& iParagraph)
{
	BOOL bWrite = FALSE;
	const auto nSigr = aSigrK.GetSize();
	for (int i = 0; i < nSigr; ++i)
	{
		const T_SIGR_K SigrK = aSigrK[i];

		T_SIGR_D SigrD;
		if (!m_pDoc->m_pAttrCtrl2->GetQSigr()->Get(SigrK, SigrD)) continue;

		T_EVAL_SUMM_LOWER eval;
		if (!m_pResult->GetSummaryLower(SigrK, eval)) continue;

		// 곡률
		std::vector<T_SIBD_CVSV_ITEM> CvsvMin, CvsvMax;
		GetCvsvItem(eval.CvsvSum.CvsvMin, DB_MEMB_TYPE::D_MBTP_BEAM, T_SIGR_LOWER_D::eMaterialType::kRC, CvsvMin);
		GetCvsvItem(eval.CvsvSum.CvsvMax, DB_MEMB_TYPE::D_MBTP_BEAM, T_SIGR_LOWER_D::eMaterialType::kRC, CvsvMax);

		// 전단
		std::vector<T_SISR_SUMM_ITEM> SisrMin, SisrMax;
		GetSisrItem(eval.SisrSum.SisrMin, DB_MEMB_TYPE::D_MBTP_BEAM, T_SIGR_LOWER_D::eMaterialType::kRC, SisrMin);
		GetSisrItem(eval.SisrSum.SisrMax, DB_MEMB_TYPE::D_MBTP_BEAM, T_SIGR_LOWER_D::eMaterialType::kRC, SisrMax);

		if (CvsvMin.size() < 1 && CvsvMax.size() < 1 && SisrMin.size() < 1 && SisrMax.size() < 1)
			continue;

		CString RptBlock = _T("_00.Summ_3.Lower_Table");
		int rowCnt = CopyRange(pXL, RptBlock, stRow);

		pXL->SetValue(stRow, 4, GetText(_T("%s(%s)"),
			SigrD.strName, _LS(IDS_BRDG_SEIS_EVAL_REPORT_JP_BEAM)));

		// Sibd Cvsv summary; curvature
		WriteCvsvItems(pXL, CvsvMin, CvsvMax, stRow);

		// Sisr summary; shear
		WriteSisrItems(pXL, SisrMin, SisrMax, stRow);

		stRow += rowCnt;
		stRow++;
		bWrite = TRUE;
		if (m_pProgress->NextStep() == FALSE) throw FALSE;
	}
	if(bWrite == TRUE)
		stRow++;
}

void CSeisEvalReportSummary_A4::WriteSummaryLowerSteel(IExcel* pXL, const CArray<T_SIGR_K, T_SIGR_K>& aSigrK, OUT int& stRow, OUT int& iParagraph)
{
	auto I_WriteTitle = [&pXL, this](OUT int& stRow)
		{
			CString RptBlock = _T("_00.Summ_3.LowerSteel");
			int rowCnt = CopyRange(pXL, RptBlock, stRow);
			stRow += rowCnt;
		};

	auto l_WriteSibdSteel = [&pXL, this](int nRow, int nCol, const T_SIBD_STEL_ITEM& summD)
		{
			if (summD.nSeismicForceDirection == T_SIPA_D::kAxis_Dir)
			{
				if (summD.nSeismicForceType == T_SIPA_D::kType2) nCol += 1;
			}
			else
			{
				if (summD.nSeismicForceType == T_SIPA_D::kType1) nCol += 2;
				else                                             nCol += 3;
			}

			pXL->SetValue(nRow + 0, nCol, summD.kElem);
			isnan(summD.dDeltaMax) != 0 ? SetValue_Hyphen(nRow+1, nCol) : pXL->SetValue(nRow+1, nCol, summD.dDeltaMax * m_dLenToM);
			isnan(summD.dDeltaYc) != 0	? SetValue_Hyphen(nRow+2, nCol) : pXL->SetValue(nRow+2, nCol, summD.dDeltaYc * m_dLenToM);
			isnan(summD.dDeltaYt) != 0	? SetValue_Hyphen(nRow+3, nCol) : pXL->SetValue(nRow+3, nCol, summD.dDeltaYt * m_dLenToM);
			isnan(summD.dDeltaA) != 0	? SetValue_Hyphen(nRow+4, nCol) : pXL->SetValue(nRow+4, nCol, summD.dDeltaA * m_dLenToM);
			isnan(summD.dRatioYc) != 0	? SetValue_Hyphen(nRow+5, nCol) : pXL->SetValue(nRow+5, nCol, summD.dRatioYc);
			isnan(summD.dRatioYt) != 0	? SetValue_Hyphen(nRow+6, nCol) : pXL->SetValue(nRow+6, nCol, summD.dRatioYt);
			isnan(summD.dRatioYc) != 0  ? SetValue_Hyphen(nRow+7, nCol) : pXL->SetValue(nRow+7, nCol, summD.dRatioYc > 1.0 ? _LS(IDS_BRDG_SEIS_EVAL_REPORT_JP_YIELD)/*_T("降伏")*/ : _T("OK"));
			isnan(summD.dRatioYt) != 0  ? SetValue_Hyphen(nRow+8, nCol) : pXL->SetValue(nRow+8, nCol, summD.dRatioYt > 1.0 ? _T("NG") : _T("OK"));

			if (isnan(summD.dRatioYc) == 0 && summD.dRatioYc > 1.0)	{
				//pXL->SetPattern(nRow + 7, nCol, nRow + 7, nCol, libxl::FILLPATTERN_SOLID, false, true, PINK_DIAMOND, PINK_DIAMOND);
				pXL->SetTextColor(nRow + 7, nCol, nRow + 7, nCol, SURRENDER_COLOR);
			}
			if (isnan(summD.dRatioYt) == 0 && summD.dRatioYt > 1.0)
			{
				pXL->SetTextColor(nRow + 6, nCol, nRow + 6, nCol, CACODENON_RED);
				SetNG_Red(pXL, nRow + 8, nCol, nRow + 8, nCol);
			}
		};

	BOOL bWrite = FALSE;
	const auto nSigr = aSigrK.GetSize();
	for (int i = 0; i < nSigr; ++i)
	{
		const T_SIGR_K SigrK = aSigrK[i];

		T_SIGR_D SigrD;
		if (!m_pDoc->m_pAttrCtrl2->GetQSigr()->Get(SigrK, SigrD)) continue;

		T_EVAL_SUMM_LOWER eval;
		if (!m_pResult->GetSummaryLower(SigrK, eval)) continue;

		// 곡률
		std::vector<T_SIBD_STEL_ITEM> CvsvColumn, CvsvBeam;
		GetStelItem(eval.StelSum, DB_MEMB_TYPE::D_MBTP_COLUMN, T_SIGR_LOWER_D::eMaterialType::kSteel, CvsvColumn);
		GetStelItem(eval.StelSum, DB_MEMB_TYPE::D_MBTP_BEAM, T_SIGR_LOWER_D::eMaterialType::kSteel, CvsvBeam);

		if (CvsvColumn.size() < 1 && CvsvBeam.size() < 1)
			continue;

		if (bWrite == FALSE) {
			I_WriteTitle(stRow);
			bWrite = TRUE;
		}

		T_SIBD_STEL_ITEM item;
		if		(CvsvColumn.size()) item = CvsvColumn[0];
		else if (CvsvBeam.size())	item = CvsvBeam[0];

		CString RptBlock = _T("_00.Summ_3.LowerSteel_Table");
		if(item.nSurveyType == T_SIBD_CVSV_D::kSurrenderSurvey)
			RptBlock = _T("_00.Summ_3.LowerSteel_Table2");

		int rowCnt = CopyRange(pXL, RptBlock, stRow);

		// 기둥
		auto nCount = CvsvColumn.size();
		if (nCount > 0) {
			pXL->SetValue(stRow, 4, GetText(_T("%s(%s)"),
				SigrD.strName, _LS(IDS_BRDG_SEIS_EVAL_REPORT_JP_COLUMN)));
		}
		for (int j = 0; j < nCount; ++j)
		{
			const auto& SidbD = CvsvColumn[j];
			l_WriteSibdSteel(stRow + 4, 4, SidbD);
		}

		// 보
		nCount = CvsvBeam.size();
		if (nCount > 0) {
			pXL->SetValue(stRow, 8, GetText(_T("%s(%s)"),
				SigrD.strName, _LS(IDS_BRDG_SEIS_EVAL_REPORT_JP_BEAM)));
		}
		for (int j = 0; j < nCount; ++j)
		{
			const auto& SidbD = CvsvBeam[j];
			l_WriteSibdSteel(stRow + 4, 8, SidbD);
		}

		stRow += rowCnt;
		stRow++;
		if (m_pProgress->NextStep() == FALSE) throw FALSE;
	}
	stRow++;
}
#pragma endregion

#pragma region // CSeisEvalReportMaxDisp
CSeisEvalReportMaxDisp::CSeisEvalReportMaxDisp(IExcel* pXL, CDgnProgressDlg* pProgress, const CString& strBasePath, const CString& strBaseBook, const SDgnExcelPrintOpt* pPrintOpt)
	: CWriteSeisEvalReportBase(pXL, pProgress, strBasePath, strBaseBook, pPrintOpt)
{
	m_pDispWhole = nullptr;
	m_pDispPierNEXCO = nullptr;
	m_pDispPierJBEC = nullptr;

	switch (m_nDesignCode)
	{
		case BrdgSE_CodeJP::JROAD5_H14:
			m_pDispWhole = new CMaxDispWhole(pXL, pProgress, strBasePath, strBaseBook, pPrintOpt);
			m_pDispPierNEXCO = new CMaxDispNEXCO_H14(pXL, pProgress, strBasePath, strBaseBook, pPrintOpt);
			m_pDispPierJBEC = new CMaxDispJBEC_H14(pXL, pProgress, strBasePath, strBaseBook, pPrintOpt);
			break;
		case BrdgSE_CodeJP::JROAD5_H24:
			m_pDispWhole = new CMaxDispWhole(pXL, pProgress, strBasePath, strBaseBook, pPrintOpt);
			m_pDispPierNEXCO = new CMaxDispNEXCO_H24(pXL, pProgress, strBasePath, strBaseBook, pPrintOpt);
			m_pDispPierJBEC = new CMaxDispJBEC_H24(pXL, pProgress, strBasePath, strBaseBook, pPrintOpt);
			break;
		case BrdgSE_CodeJP::JROAD5_H29:
			m_pDispWhole = new CMaxDispWhole_H29(pXL, pProgress, strBasePath, strBaseBook, pPrintOpt);
			m_pDispPierNEXCO = new CMaxDispNEXCO_H29(pXL, pProgress, strBasePath, strBaseBook, pPrintOpt);
			m_pDispPierJBEC = new CMaxDispJBEC_H29(pXL, pProgress, strBasePath, strBaseBook, pPrintOpt);
			break;
	default:
		break;
	}
	// default
	if (m_pDispWhole == nullptr || m_pDispPierNEXCO == nullptr || m_pDispPierJBEC == nullptr)
	{
		AfxMessageBox(_T("Design Code Error - Default H24"));
		m_pDispWhole = new CMaxDispWhole(pXL, pProgress, strBasePath, strBaseBook, pPrintOpt);
		m_pDispPierNEXCO = new CMaxDispNEXCO_H24(pXL, pProgress, strBasePath, strBaseBook, pPrintOpt);
		m_pDispPierJBEC = new CMaxDispJBEC_H24(pXL, pProgress, strBasePath, strBaseBook, pPrintOpt);
	}
}

CSeisEvalReportMaxDisp::~CSeisEvalReportMaxDisp()
{
	if (m_pDispWhole)		delete m_pDispWhole;
	if (m_pDispPierNEXCO)	delete m_pDispPierNEXCO;
	if (m_pDispPierJBEC)	delete m_pDispPierJBEC;
}

void CSeisEvalReportMaxDisp::AddStep()
{
	CArray<T_SIGR_K, T_SIGR_K> aSigrK;
	m_pDoc->m_pAttrCtrl2->GetQSigr()->GetKeyList(aSigrK);
	const auto nSigr = aSigrK.GetSize();
	if (nSigr == 0) return;

	for (int z = 0; z < 2; ++z) // “基礎変位影響分に対する安全系の考慮”=check on/off
	{
		// 1-1. 最大応答変位の照査 - 全体系
		for (int i = 0; i < 2; ++i) // 橋軸方向, 橋軸直角方向
		{
			for (int j = 0; j < nSigr; ++j)
			{
				for (int k = 0; k < 2; ++k) // 좌/우
				{
					m_pProgress->m_nTotalSteps++;
				}
			}
		}
	}
}

void CSeisEvalReportMaxDisp::Write()
{
	//	1. 変位の照査 변위조사
	//		1-1. 最大応答変位の照査 - 全体系 최대응답변위검토 - 전체시스템
	//		1-1-1. 橋軸方向 교축방향
	//		1-1-2. 橋軸直角方向 교축직각방향

	// Check exist result
	std::map<std::tuple<int, int, int, int, int>, std::vector<T_SIMD_K>> mExistSimd;
	if (!IsExistData(mExistSimd)) return;

	AddSheet();

	int stRow = 0;
	*this->m_pParagraph1 = *this->m_pParagraph1 + 1;

	// N. 変位の照査
	CString RptBlock = _T("_01.MaxDisp_Title");
	int rowCnt = CopyRange(m_pXL, RptBlock, stRow);
	m_pXL->SetValue(stRow, 0, GetText(_T("%d. %s"), *this->m_pParagraph1, _LS(IDS_BRDG_SEIS_EVAL_REPORT_JP_CHK_DISP))); // 変位の照査
	stRow += rowCnt;

	int iParagraph3 = 0;

	int nModel = BY_UNKNOWN;
	int nRspCalcType = seis_jp::kRspType_0;
	int nMaterial = T_SIGR_LOWER_D::kUnKnown;
	int nAlwCalcType = seis_jp::kAlwCalcType_0;
	int nSeisDir = T_SIPA_D::kUnKnown; // 1: 교축방향, 2: 교축직각방향
	std::map<std::tuple<int, int, int, int>, std::vector<T_SIMD_K>> mNexco, mJbec;
	for (const auto& it : mExistSimd)
	{
		const auto& key = it.first;
		const auto& vecSimd = it.second;
		if (vecSimd.size() < 1) continue;

		const int iModel = std::get<0>(key);
		const int iRspCalcType = std::get<1>(key);
		const int iMaterial = std::get<2>(key);
		const int iAlwCalcType = std::get<3>(key);
		const int iSeisDir = std::get<4>(key);

		if (iRspCalcType != nRspCalcType || iMaterial != nMaterial || iAlwCalcType != nAlwCalcType || iModel != nModel)
		{
			if (iModel == BY_WHOLE_MODEL) m_pDispWhole->WirteTheory(iMaterial, *this->m_pParagraph1, *this->m_pParagraph2, stRow);
			else
			{
				if (iRspCalcType == seis_jp::kNEXCO)
					m_pDispPierNEXCO->WirteTheory(iMaterial, iAlwCalcType, *this->m_pParagraph1, *this->m_pParagraph2, stRow);
				else if (iRspCalcType == seis_jp::kJBEC)
					m_pDispPierJBEC->WirteTheory(iMaterial, iAlwCalcType, *this->m_pParagraph1, *this->m_pParagraph2, stRow);
			}
			nModel = iModel;
			nRspCalcType = iRspCalcType;
			nMaterial = iMaterial;
			nAlwCalcType = iAlwCalcType;
			nSeisDir = T_SIPA_D::kUnKnown; // 초기화
		}

		if (iSeisDir != nSeisDir)
		{
			CString RptBlock = iModel == BY_WHOLE_MODEL ? _T("_01.MaxDisp_Head1") : _T("_01.MaxDisp_Head1_pier");
			WriteSeisDir(RptBlock, iSeisDir, iParagraph3, stRow);
			nSeisDir = iSeisDir;
		}

		// Set Data
		if (iModel == BY_WHOLE_MODEL)
			m_pDispWhole->Write(vecSimd, nSeisDir, iParagraph3, stRow);
		else
		{
			if (iRspCalcType == seis_jp::kNEXCO)
			{
				m_pDispPierNEXCO->Write(vecSimd, nSeisDir, iParagraph3, stRow);
			}
			else if (iRspCalcType == seis_jp::kJBEC)
			{
				m_pDispPierJBEC->Write(vecSimd, nSeisDir, iParagraph3, stRow);
			}
		}
	}
}

void CSeisEvalReportMaxDisp::AddSheet()
{
	m_strBaseSheet = _LSX(01.MaxDisp);
	const CString strNewSheet = _LS(IDS_BRDG_SEIS_EVAL_REPORT_JP_MAX_DISP);// _T("最大変位");
	if (m_pXL->IsExistSheetName(strNewSheet))
	{
		ASSERT(0);
		m_pXL->SetActiveSheet(strNewSheet);
	}
	else
	{
		m_pXL->AddSheet2(strNewSheet);
		m_pXL->SetActiveSheet(strNewSheet);
	}

	m_pXL->SetStandardFont(_T("Meiryo UI"));
	m_pXL->SetMargin(13, 12, 20, 17);
	m_pXL->SetColumnWidth(0, 0, 0, 60, 5.75); // 6.9 * 0.833333333
	m_pXL->SetRowHeight(16.5);
	m_pXL->SetZoom(75);
}

BOOL CSeisEvalReportMaxDisp::GetData_MaxDisp(IN CDBDoc* pDoc, OUT CArray<T_SIGR_K, T_SIGR_K>& aSigrK, OUT CArray<T_SIMD_K, T_SIMD_K>& aSimdK)
{
	pDoc->m_pAttrCtrl2->GetQSigr()->GetKeyList(aSigrK);
	if (aSigrK.GetSize() == 0) return FALSE;

	pDoc->m_pAttrCtrl2->GetQSimd()->GetKeyList(aSimdK);
	if (aSimdK.GetSize() == 0) return FALSE;

	return TRUE;
}

BOOL CSeisEvalReportMaxDisp::IsExistData(std::map<std::tuple<int, int, int, int, int>, std::vector<T_SIMD_K>>& mExistSimd)
{
	CArray<T_SIGR_K, T_SIGR_K> aSigrK; CArray<T_SIMD_K, T_SIMD_K> aSimdK;
	if (GetData_MaxDisp(m_pDoc, aSigrK, aSimdK) == FALSE) return FALSE;
	if (aSigrK.GetSize() < 1 || aSimdK.GetSize() < 1) return FALSE;

	for (int i = 0; i < aSimdK.GetSize(); i++)
	{
		const T_SIMD_K SimdK = aSimdK[i];

		T_SIMD_D SimdD;
		if (!m_pDoc->m_pAttrCtrl2->GetQSimd()->Get(SimdK, SimdD)) continue;

		T_SIGR_D SigrD;
		if (!m_pDoc->m_pAttrCtrl2->GetQSigr()->Get(SimdD.kSigr, SigrD)) continue;
		//if (SigrD.nSubType != this->m_subType)								continue;

		T_SIGR_LOWER_D SigrLowerD;
		if (SigrD.nSubType == T_SIGR_D::kLower)
		{
			if (!m_pDoc->m_pAttrCtrl2->GetQSigrLower()->Get(SimdD.kSigr, SigrLowerD)) continue;;
		}
		else
		{
			if (!m_pDoc->m_pAttrCtrl2->GetQSigrLower()->Get(SimdD.PLUS_D.kSigrLower, SigrLowerD) && 
				!m_pDoc->m_pAttrCtrl2->GetQSigrLower()->Get(SimdD.MINUS_D.kSigrLower, SigrLowerD)) continue;;
		}

		T_SIPA_D SipaD;
		if (!m_pDoc->m_pAttrCtrl2->GetQSipa()->Get(SimdD.kSipa, SipaD)) continue;
		if (SipaD.nDesignCode != m_nDesignCode)							continue;

		const auto nPos = (int)GetExistPos(SimdD.kSigr, SimdD.kSipa); // 1: 왼쪽(전체계) 2: 오른쪽(전체계) 3: 아래(교각별)
		if (nPos == T_SIMD_EVAL::kUnKnown) continue;

		const auto nModel = nPos == T_SIMD_EVAL::kLower ? BY_PIER_MODEL : BY_WHOLE_MODEL;
		const auto nRspCalcType = (int)SimdD.nRspCalcType; // 계산방법
		auto nMaterial = (int)SigrLowerD.nMaterialType; // 재료 타입
		nMaterial = nMaterial == T_SIGR_LOWER_D::kSRC ? T_SIGR_LOWER_D::kSteel : nMaterial; // SRC -> Steel로 간주
		const auto nAlwCalcType = (int)SimdD.nAlwCalcType; // 허용치계산방법
		const auto nSeisDir = (int)SipaD.nSeismicForceDirection; // 방향

		mExistSimd[std::make_tuple(nModel, nRspCalcType, nMaterial, nAlwCalcType, nSeisDir)].push_back(SimdK); // 우선순위...
	}

	if (mExistSimd.size() < 1) return FALSE;
	return TRUE;
}

int CSeisEvalReportMaxDisp::GetExistPos(const T_SIGR_K& SigrK, const T_SIPA_K& SipaK)
{
	for (int k = T_SIMD_EVAL::kLeft; k <= T_SIMD_EVAL::kLower; ++k) // 좌/우
	{
		const auto nPos = k;
		T_SIMD_EVAL EvalD;
		const bool bExist = m_pResult->GetResultSimd(SipaK, SigrK, nPos, EvalD);
		if (bExist)
		{
			return nPos;
		}
	}
	return T_SIMD_EVAL::kUnKnown; // 없음
}

#pragma endregion

#pragma region // CSeisEvalReportResidualDisp
CSeisEvalReportResidualDisp::CSeisEvalReportResidualDisp(IExcel* pXL, CDgnProgressDlg* pProgress, const CString& strBasePath, const CString& strBaseBook, const SDgnExcelPrintOpt* pPrintOpt)
	: CWriteSeisEvalReportBase(pXL, pProgress, strBasePath, strBaseBook, pPrintOpt)
{
	m_pDispWhole = nullptr;
	m_pDispPierNEXCO = nullptr;
	m_pDispPierJBEC = nullptr;

	switch (m_nDesignCode)
	{
		case BrdgSE_CodeJP::JROAD5_H14:
			m_pDispWhole = new CResidualDispWhole(pXL, pProgress, strBasePath, strBaseBook, pPrintOpt);
			m_pDispPierNEXCO = new CResidualDispNEXCO_H14(pXL, pProgress, strBasePath, strBaseBook, pPrintOpt);
			m_pDispPierJBEC = new CResidualDispJBEC_H14(pXL, pProgress, strBasePath, strBaseBook, pPrintOpt);
			break;
		case BrdgSE_CodeJP::JROAD5_H24:
			m_pDispWhole = new CResidualDispWhole(pXL, pProgress, strBasePath, strBaseBook, pPrintOpt);
			m_pDispPierNEXCO = new CResidualDispNEXCO_H24(pXL, pProgress, strBasePath, strBaseBook, pPrintOpt);
			m_pDispPierJBEC = new CResidualDispJBEC_H24(pXL, pProgress, strBasePath, strBaseBook, pPrintOpt);
			break;
		case BrdgSE_CodeJP::JROAD5_H29:
			m_pDispWhole = new CResidualDispWhole_H29(pXL, pProgress, strBasePath, strBaseBook, pPrintOpt);
			m_pDispPierNEXCO = new CResidualDispNEXCO_H29(pXL, pProgress, strBasePath, strBaseBook, pPrintOpt);
			m_pDispPierJBEC = new CResidualDispJBEC_H29(pXL, pProgress, strBasePath, strBaseBook, pPrintOpt);
			break;
		default:
			break;
	}
	// default
	if (m_pDispWhole == nullptr || m_pDispPierNEXCO == nullptr || m_pDispPierJBEC == nullptr)
	{
		AfxMessageBox(_T("Design Code Error - Default H24"));
		m_pDispWhole = new CResidualDispWhole(pXL, pProgress, strBasePath, strBaseBook, pPrintOpt);
		m_pDispPierNEXCO = new CResidualDispNEXCO_H24(pXL, pProgress, strBasePath, strBaseBook, pPrintOpt);
		m_pDispPierJBEC = new CResidualDispJBEC_H24(pXL, pProgress, strBasePath, strBaseBook, pPrintOpt);
	}
}

CSeisEvalReportResidualDisp::~CSeisEvalReportResidualDisp()
{
	if (m_pDispWhole)		delete m_pDispWhole;
	if (m_pDispPierNEXCO)	delete m_pDispPierNEXCO;
	if (m_pDispPierJBEC)	delete m_pDispPierJBEC;
}

void CSeisEvalReportResidualDisp::AddStep()
{
	CArray<T_SIGR_K, T_SIGR_K> aSigrK;
	m_pDoc->m_pAttrCtrl2->GetQSigr()->GetKeyList(aSigrK);
	const auto nSigr = aSigrK.GetSize();
	if (nSigr == 0) return;

	for (int z = 0; z < 2; ++z) // 전체계 / 교각별
	{
		for (int i = 0; i < 2; ++i) // 橋軸方向, 橋軸直角方向
		{
			for (int j = 0; j < nSigr; ++j)
			{
				const int iEd = z == 0 ? 2 : 1; // 상부그룹은 좌/우
				for (int k = 0; k < iEd; ++k)
				{
					m_pProgress->m_nTotalSteps++;
				}
			}
		}
	}
}

void CSeisEvalReportResidualDisp::Write()
{
	std::map<std::tuple<int, int, int, int, int>, std::vector<T_SIRD_K>> mExistSird;
	if (!IsExistData(mExistSird)) return;

	AddSheet();

	int stRow = 0;
	*this->m_pParagraph1 = *this->m_pParagraph1 + 1;
// 1. 変位の照査
	CString RptBlock = _T("_01.ResDisp_Title");
	int rowCnt = CopyRange(m_pXL, RptBlock, stRow);
	m_pXL->SetValue(stRow, 0, GetText(_T("%d. %s"), *this->m_pParagraph1, _LS(IDS_BRDG_SEIS_EVAL_REPORT_JP_CHK_DISP)));
	stRow += rowCnt;

	int iParagraph3 = 0;
	int nModel = BY_UNKNOWN;
	int nRspCalcType = seis_jp::kRspType_0;
	int nMaterial = T_SIGR_LOWER_D::kUnKnown;
	int nAlwCalcType = seis_jp::kAlwCalcType_0;
	int nSeisDir = T_SIPA_D::kUnKnown; // 1: 교축방향, 2: 교축직각방향
	for (const auto& it : mExistSird)
	{
		const auto& key = it.first;
		const auto& vecSird = it.second;
		if (vecSird.size() < 1) continue;

		const int iModel = std::get<0>(key);
		const int iRspCalcType = std::get<1>(key);
		const int iMaterial = std::get<2>(key);
		const int iAlwCalcType = std::get<3>(key);
		const int iSeisDir = std::get<4>(key);

		if (iRspCalcType != nRspCalcType || iMaterial != nMaterial ||iAlwCalcType != nAlwCalcType || iModel != nModel)
		{
			if (iModel == BY_WHOLE_MODEL) // 전체계
			{
				m_pDispWhole->WirteTheory(iMaterial, *this->m_pParagraph1, *this->m_pParagraph2, stRow);
			}
			else
			{
				if (iRspCalcType == seis_jp::kNEXCO)
					m_pDispPierNEXCO->WirteTheory(iMaterial, iAlwCalcType, *this->m_pParagraph1, *this->m_pParagraph2, stRow);
				else if (iRspCalcType == seis_jp::kJBEC)
					m_pDispPierJBEC->WirteTheory(iMaterial, iAlwCalcType, *this->m_pParagraph1, *this->m_pParagraph2, stRow);
			}

			nModel = iModel;
			nRspCalcType = iRspCalcType;
			nMaterial = iMaterial;
			nAlwCalcType = iAlwCalcType;
			nSeisDir = T_SIPA_D::kUnKnown; // 초기화
		}

		if (iSeisDir != nSeisDir)
		{
			CString RptBlock = iModel == BY_WHOLE_MODEL ? _T("_01.ResDisp_Head1") : _T("_01.ResDisp_Head2");
			WriteSeisDir(RptBlock, iSeisDir, iParagraph3, stRow);
			nSeisDir = iSeisDir;
		}

		// Set Data
		if (iModel == BY_WHOLE_MODEL) // 전체계
		{
			m_pDispWhole->Write(vecSird, nSeisDir, iParagraph3, stRow);
		}
		else // 교각별
		{
			if (iRspCalcType == seis_jp::kNEXCO)
			{
				m_pDispPierNEXCO->Write(vecSird, nSeisDir, iParagraph3, stRow);
			}
			else if (iRspCalcType == seis_jp::kJBEC)
			{
				m_pDispPierJBEC->Write(vecSird, nSeisDir, iParagraph3, stRow);
			}
		}
	}
}

void CSeisEvalReportResidualDisp::AddSheet()
{
	m_strBaseSheet = _LSX(01.ResDisp);
	const CString strNewSheet = _LS(IDS_BRDG_SEIS_EVAL_REPORT_JP_RES_DISP);;//_T("残留変位");
	if (m_pXL->IsExistSheetName(strNewSheet))
	{
		ASSERT(0);
		m_pXL->SetActiveSheet(strNewSheet);
	}
	else
	{
		m_pXL->AddSheet2(strNewSheet);
		m_pXL->SetActiveSheet(strNewSheet);
	}

	m_pXL->SetStandardFont(_T("Meiryo UI"));
	m_pXL->SetMargin(13, 12, 20, 17);
	m_pXL->SetColumnWidth(0, 0, 0, 60, 5.75); // 6.9 * 0.833333333
	m_pXL->SetRowHeight(16.5);
	m_pXL->SetZoom(70);
}

BOOL CSeisEvalReportResidualDisp::IsExistData(std::map<std::tuple<int, int, int, int, int>, std::vector<T_SIRD_K>>& mExistSird)
{
	CArray<T_SIGR_K, T_SIGR_K> aSigrK;
	m_pDoc->m_pAttrCtrl2->GetQSigr()->GetKeyList(aSigrK);
	if (aSigrK.GetSize() < 1) return FALSE;

	CArray<T_SIRD_K, T_SIRD_K> aSirdK;
	m_pDoc->m_pAttrCtrl2->GetQSird()->GetKeyList(aSirdK);
	if (aSirdK.GetSize() < 1) return FALSE;

	for (int i = 0; i < aSirdK.GetSize(); i++)
	{
		const T_SIRD_K SirdK = aSirdK[i];

		T_SIRD_D SirdD;
		if (!m_pDoc->m_pAttrCtrl2->GetQSird()->Get(SirdK, SirdD)) continue;

		T_SIGR_D SigrD;
		if (!m_pDoc->m_pAttrCtrl2->GetQSigr()->Get(SirdD.kSigr, SigrD)) continue;

		T_SIGR_LOWER_D SigrLowerD;
		if (SigrD.nSubType == T_SIGR_D::kLower)
		{
			if (!m_pDoc->m_pAttrCtrl2->GetQSigrLower()->Get(SirdD.kSigr, SigrLowerD)) continue;;
		}
		else
		{
			if (!m_pDoc->m_pAttrCtrl2->GetQSigrLower()->Get(SirdD.PLUS_D.kSigrLower, SigrLowerD) &&
				!m_pDoc->m_pAttrCtrl2->GetQSigrLower()->Get(SirdD.MINUS_D.kSigrLower, SigrLowerD)) continue;;
		}

		T_SIPA_D SipaD;
		if (!m_pDoc->m_pAttrCtrl2->GetQSipa()->Get(SirdD.kSipa, SipaD)) continue;
		if (SipaD.nDesignCode != m_nDesignCode)							continue;

		const auto nPos = (int)GetExistPos(SirdD.kSigr, SirdD.kSipa); // 1: 왼쪽(전체계) 2: 오른쪽(전체계) 3: 아래(교각별)
		if (nPos == T_SIMD_EVAL::kUnKnown) continue;

		const auto nModel = nPos == T_SIMD_EVAL::kLower ? BY_PIER_MODEL : BY_WHOLE_MODEL;
		const auto nRspCalcType = (int)SirdD.nRspCalcType; // 계산방법
		auto nMaterial = (int)SigrLowerD.nMaterialType; // 재료 타입
		nMaterial = nMaterial == T_SIGR_LOWER_D::kSRC ? T_SIGR_LOWER_D::kSteel : nMaterial; // SRC -> Steel로 간주
		nMaterial = VERSION_965_DEFAULT; //v970 삭제
		const auto nAlwCalcType = VERSION_965_DEFAULT; //  v970 -> (int)SirdD.nAlwCalcType; // 허용치계산방법
		const auto nSeisDir = (int)SipaD.nSeismicForceDirection; // 방향

		mExistSird[std::make_tuple(nModel, nRspCalcType, nMaterial, nAlwCalcType, nSeisDir)].push_back(SirdK); // 우선순위...
	}

	if (mExistSird.size() < 1) return FALSE;
	return TRUE;
}

int CSeisEvalReportResidualDisp::GetExistPos(const T_SIGR_K& SigrK, const T_SIPA_K& SipaK)
{
	for (int k = T_SIRD_EVAL::kLeft; k <= T_SIRD_EVAL::kLower; ++k) // 좌/우
	{
		const auto nPos = k;
		T_SIRD_EVAL EvalD;
		const bool bExist = m_pResult->GetResultSird(SipaK, SigrK, nPos, EvalD);
		if (bExist)
		{
			return nPos;
		}
	}
	return T_SIRD_EVAL::kUnKnown; // 없음
}

#pragma endregion

#pragma region // CSeisEvalReportFlexure
CSeisEvalReportFlexure::CSeisEvalReportFlexure(IExcel* pXL, CDgnProgressDlg* pProgress, const CString& strBasePath, const CString& strBaseBook, const SDgnExcelPrintOpt* pPrintOpt)
	: CWriteSeisEvalReportBase(pXL, pProgress, strBasePath, strBaseBook, pPrintOpt)
{
	m_nCode_Forc = D_UNITSYS_FORCE_INDEX_KN;
	m_nCode_Leng = D_UNITSYS_LENGTH_INDEX_M;
}

CSeisEvalReportFlexure::~CSeisEvalReportFlexure()
{
}

void CSeisEvalReportFlexure::AddStep()
{
	T_KEY_LIST aSibdK;
	m_pDoc->m_pAttrCtrl2->GetQSibd()->GetKeyList(aSibdK);
	if (aSibdK.GetSize() < 1) return;

	std::vector<T_SIBD_K> vExistSibd[2][2][2];
	for (int i = 0; i < aSibdK.GetSize(); ++i)
	{
		const T_SIBD_K SibdK = aSibdK[i];
		T_SIBD_D SibdD;
		if (!m_pDoc->m_pAttrCtrl2->GetQSibd()->Get(SibdK, SibdD))		continue;
		if (SibdD.nSibdType != T_SIBD_D::kStrengthSurvey) continue;
		if (SibdD.SRSV_D.nMomentType != T_SIBD_SRSV_D::kMy && SibdD.SRSV_D.nMomentType != T_SIBD_SRSV_D::kMu) continue;
		T_SIGR_D SigrD;
		if (!m_pDoc->m_pAttrCtrl2->GetQSigr()->Get(SibdD.kSigr, SigrD)) continue;
		if (SigrD.nSubType != this->m_subType)								continue;
		T_SIPA_D SipaD;
		if (!m_pDoc->m_pAttrCtrl2->GetQSipa()->Get(SibdD.kSipa, SipaD)) continue;
		if (SipaD.nDesignCode != m_nDesignCode)							continue;

		const auto idx0 = (int)SibdD.SRSV_D.nMomentType - 1; // 0: My, 1: Mu
		const auto iIdx1 = (int)SipaD.nSeismicForceDirection - 1;
		const auto iIdx2 = (int)SipaD.nSeismicForceType - 1;
		vExistSibd[idx0][iIdx1][iIdx2].push_back(SibdK);
	}

	if (vExistSibd[0][0][0].size() == 0 && vExistSibd[0][0][1].size() == 0 &&
		vExistSibd[0][1][0].size() == 0 && vExistSibd[0][1][1].size() == 0 &&
		vExistSibd[1][0][0].size() == 0 && vExistSibd[1][0][1].size() == 0 &&
		vExistSibd[1][1][0].size() == 0 && vExistSibd[1][1][1].size() == 0) return;

	BOOL bWrited = FALSE;

	for (int iMyMz = 0; iMyMz < 2; ++iMyMz)
	{
		if (vExistSibd[iMyMz][0][0].size() == 0 && vExistSibd[iMyMz][0][1].size() == 0 &&
			vExistSibd[iMyMz][1][0].size() == 0 && vExistSibd[iMyMz][1][1].size() == 0) continue;

		for (int iSeisDir = 0; iSeisDir < 2; ++iSeisDir) // 橋軸方向, 橋軸直角方向
		{
			if (vExistSibd[iMyMz][iSeisDir][0].size() == 0 && vExistSibd[iMyMz][iSeisDir][1].size() == 0) continue;
			for (int iType = 0; iType < 2; ++iType) // タイプ type
			{
				if (vExistSibd[iMyMz][iSeisDir][iType].size() == 0) continue;
				for (const auto& SibdK : vExistSibd[iMyMz][iSeisDir][iType])
				{
					T_SIBD_D SibdD;
					if (!m_pDoc->m_pAttrCtrl2->GetQSibd()->Get(SibdK, SibdD))	continue;

					T_SIGR_D SigrD;
					if (!m_pDoc->m_pAttrCtrl2->GetQSigr()->Get(SibdD.kSigr, SigrD)) continue;
					if (SigrD.nSubType != this->m_subType) continue;

					T_ELEM_K_LIST aElemKeys; std::map <T_ELEM_K, std::tuple<CString, int>> mElemInfo;
					GetTargetElements(SibdD.kSigr, this->m_subType, aElemKeys, mElemInfo);
					if (aElemKeys.GetCount() < 1) continue;

					const int nMemb = aElemKeys.GetSize();
					for (int k = 0; k < nMemb; ++k)
					{
						const T_ELEM_K& ElemK = aElemKeys[k];
						if (mElemInfo.find(ElemK) == mElemInfo.end()) continue;

						//const CString& sGroup = std::get<0>(mElemInfo[ElemK]);
						//const int nShearDir = std::get<1>(mElemInfo[ElemK]);

						for(int d = 0; d < 2; ++d) // y, z
						{
							int nShearDir = d == 0 ? seis_jp::kY_Dir : seis_jp::kZ_Dir;
							T_SIBD_EVAL_MOMENT eval;
							eval.kSipa = SibdD.kSipa;
							eval.kSigr = SibdD.kSigr;
							eval.kElem = ElemK;
							eval.nExamShearDir = nShearDir;
							if (!m_pResult->GetResultSibdMoment(eval)) continue;
							m_pProgress->m_nTotalSteps++;
						}
					}
				}
			}
		}
	}
}

void CSeisEvalReportFlexure::AddSheet(const T_SIGR_D::eSubType& nGroupType, OUT CString& rSheetName)
{
	m_strBaseSheet = _LSX(02.Flexure);
	rSheetName = _LS(IDS_BRDG_SEIS_EVAL_REPORT_JP_FLEXURE) + _T("_") + GetTextGroup(nGroupType);// _T("曲げ耐力");
	if (m_pXL->IsExistSheetName(rSheetName))
	{
		ASSERT(0);
		m_pXL->SetActiveSheet(rSheetName);
	}
	else
	{
		m_pXL->AddSheet2(rSheetName);
		m_pXL->SetActiveSheet(rSheetName);
	}

	m_pXL->SetStandardFont(_T("Meiryo UI"));
	m_pXL->SetMargin(13, 12, 20, 17);
	m_pXL->SetColumnWidth(0, 0, 0, 0, 5.9 * _ToPoint); // 6.9 * 0.833333333
	m_pXL->SetColumnWidth(0, 1, 0, 2, 4.9 * _ToPoint); // 4.9 * 0.833333333
	m_pXL->SetColumnWidth(0, 3, 0, 60, 7.5 * _ToPoint); // 7.5 * 0.833333333
	m_pXL->SetRowHeight(16.5);
	m_pXL->SetZoom(70);
}

void CSeisEvalReportFlexure::WirteTheory(const int& nMomentType, const int& nEarthquakeType, const T_SIGR_D::eSubType& nGroupType, const int iParagraph1, int& iParagraph2, OUT int& stRow)
{
	CString RptBlock = _T("_02.Flexure_Title");
	int rowCnt = CopyRange(m_pXL, RptBlock, stRow);
	m_pXL->SetValue(stRow, 0, GetText(_T("%d. %s"), iParagraph1, GetTextTitle(nGroupType)));
	stRow += rowCnt;

	switch (nMomentType)
	{
	case T_SIBD_SRSV_D::kMy:
		RptBlock = _T("_02.Flexure_Theory");
		break;
	case T_SIBD_SRSV_D::kMu:
		RptBlock = _T("_02.Flexure_Theory_Ult");
		break;
	default:
		ASSERT(FALSE);
		break;
	}

	rowCnt = CopyRange(m_pXL, RptBlock, stRow);
	m_pXL->SetValue(stRow, 0, GetText(_T("%d-%d.%s"), iParagraph1, ++iParagraph2, _LS(IDS_BRDG_SEIS_EVAL_REPORT_JP_CHK_FLEX)));// 曲げの照査
	stRow += rowCnt;
	stRow++;
}

void CSeisEvalReportFlexure::Write()
{
	// Code 단위계 변환 //////////////////////////////////////////////////////			
	CCurUnitSaver UnitSaver(TRUE);
	T_UNIT_INDEX CurrUnit;
	m_pDoc->m_pUnitCtrl->GetUnitIndexCurrent(CurrUnit);
	T_UNIT_INDEX CodeUnit = CurrUnit;
	CodeUnit.nBase_Force = m_nCode_Forc;
	CodeUnit.nBase_Length = m_nCode_Leng;
	m_pDoc->m_pUnitCtrl->SetUnitIndexCurrent(CodeUnit);

	//	2. 上部構造の照査 상부구조조사
	//		2-1. 曲げの照査 휨조사
	//		2-1-1. 橋軸方向 교축방향
	//			1) タイプⅠ
	//			2) タイプⅡ
	//		2-1-2. 橋軸直角方向 교축직각방향
	//			1) タイプⅠ
	//			2) タイプⅡ
	T_KEY_LIST aSibdK;
	m_pDoc->m_pAttrCtrl2->GetQSibd()->GetKeyList(aSibdK);
	if (aSibdK.GetSize() < 1) return;

	int nEarthquakeType = 0;
	std::vector<T_SIBD_K> vExistSibd[2][2][2];
	for (int i = 0; i < aSibdK.GetSize(); ++i)
	{
		const T_SIBD_K SibdK = aSibdK[i];
		T_SIBD_D SibdD;
		if (!m_pDoc->m_pAttrCtrl2->GetQSibd()->Get(SibdK, SibdD))		continue;
		if (SibdD.nSibdType != T_SIBD_D::kStrengthSurvey) continue;
		if (SibdD.SRSV_D.nMomentType != T_SIBD_SRSV_D::kMy && SibdD.SRSV_D.nMomentType != T_SIBD_SRSV_D::kMu) continue;
		T_SIGR_D SigrD;
		if (!m_pDoc->m_pAttrCtrl2->GetQSigr()->Get(SibdD.kSigr, SigrD)) continue;
		if (SigrD.nSubType != this->m_subType)								continue;
		T_SIPA_D SipaD;
		if (!m_pDoc->m_pAttrCtrl2->GetQSipa()->Get(SibdD.kSipa, SipaD)) continue;
		if (SipaD.nDesignCode != m_nDesignCode)							continue;

		const auto idx0 = (int)SibdD.SRSV_D.nMomentType - 1; // 0: My, 1: Mu
		const auto iIdx1 = (int)SipaD.nSeismicForceDirection - 1;
		const auto iIdx2 = (int)SipaD.nSeismicForceType - 1;
		vExistSibd[idx0][iIdx1][iIdx2].push_back(SibdK);
		if (nEarthquakeType == 0) nEarthquakeType = SipaD.nEarthquakeType;
	}

	if (vExistSibd[0][0][0].size() == 0 && vExistSibd[0][0][1].size() == 0 &&
		vExistSibd[0][1][0].size() == 0 && vExistSibd[0][1][1].size() == 0 &&
		vExistSibd[1][0][0].size() == 0 && vExistSibd[1][0][1].size() == 0 &&
		vExistSibd[1][1][0].size() == 0 && vExistSibd[1][1][1].size() == 0) return;


	CString strSheetName;
	AddSheet(this->m_subType, strSheetName);
	*this->m_pParagraph1 = *this->m_pParagraph1 + 1;

	WriteCommon(vExistSibd, nEarthquakeType, strSheetName);
	// 단위계 복구 ///////////////////////////////////////////////////////////	
	m_pDoc->m_pUnitCtrl->SetUnitIndexCurrent(CurrUnit);
	////////////////////////////////////////////////////////////////////////
}

void CSeisEvalReportFlexure::WriteCommon(const std::vector<T_SISR_K>(&vExistSibd)[2][2][2], const int& nEarthquakeType, const CString& strSheetName)
{
	CString RptBlock;
	int rowCnt = 0;
	int stRow = 0;

	BOOL bWrited = FALSE;
	int iParagraph3 = 0;
	for (int iMyMz = 0; iMyMz < 2; ++iMyMz)
	{
		if (vExistSibd[iMyMz][0][0].size() == 0 && vExistSibd[iMyMz][0][1].size() == 0 &&
			vExistSibd[iMyMz][1][0].size() == 0 && vExistSibd[iMyMz][1][1].size() == 0) continue;

		const int nMomentType = iMyMz == 0 ? T_SIBD_SRSV_D::kMy : T_SIBD_SRSV_D::kMu;
		WirteTheory(nMomentType, nEarthquakeType, this->m_subType, *this->m_pParagraph1, *this->m_pParagraph2, stRow); // My / Mu

		for (int iSeisDir = 0; iSeisDir < 2; ++iSeisDir) // 橋軸方向, 橋軸直角方向
		{
			if (vExistSibd[iMyMz][iSeisDir][0].size() == 0 && vExistSibd[iMyMz][iSeisDir][1].size() == 0) continue;

			RptBlock = _T("_02.Flexure_Dir");
			WriteSeisDir(RptBlock, iSeisDir+1, iParagraph3, stRow);

			BOOL bWriteSeisDir = FALSE;
			int iParagraph4 = 0;
			for (int iType = 0; iType < 2; ++iType) // タイプ type
			{
				if (vExistSibd[iMyMz][iSeisDir][iType].size() == 0) continue;

				RptBlock = nMomentType == T_SIBD_SRSV_D::kMy ? _T("_02.Flexure_Head") : _T("_02.Flexure_Head2");
				WriteSeisType(RptBlock, iType + 1, iParagraph4, stRow);

				for (const auto& SibdK : vExistSibd[iMyMz][iSeisDir][iType])
				{
					T_SIBD_D SibdD;
					if (!m_pDoc->m_pAttrCtrl2->GetQSibd()->Get(SibdK, SibdD))		continue;

					T_SIGR_D SigrD;
					if (!m_pDoc->m_pAttrCtrl2->GetQSigr()->Get(SibdD.kSigr, SigrD)) continue;
					if (SigrD.nSubType != this->m_subType) continue;

					T_ELEM_K_LIST aElemKeys; std::map <T_ELEM_K, std::tuple<CString, int>> mElemInfo;
					GetTargetElements(SibdD.kSigr, this->m_subType, aElemKeys, mElemInfo);
					if (aElemKeys.GetCount() < 1) continue;

					const int stRowGrp = stRow;
					const int nMemb = aElemKeys.GetSize();
					for (int k = 0; k < nMemb; ++k)
					{
						const T_ELEM_K& ElemK = aElemKeys[k];
						if (mElemInfo.find(ElemK) == mElemInfo.end()) continue;

						//const CString& sGroup = std::get<0>(mElemInfo[ElemK]);
						const int nShearDir = std::get<1>(mElemInfo[ElemK]);

						std::tuple<T_SIPA_K, T_SIGR_K, T_ELEM_K, CString> tParam; // 내진 방향 포함
						tParam = std::make_tuple(SibdD.kSipa, SibdD.kSigr, ElemK, SigrD.strName);

						if (nShearDir == seis_jp::kYZ_Dir) {
							WriteAxisTwo(tParam, stRow, bWrited);
						}
						else {
							WriteAxisOne(tParam, stRow, bWrited);
						}
					}
					auto* pFormat = m_pXL->GetSheet()->cellFormat(stRow - 1, 0);
					m_pXL->GetSheet()->setCellFormat(stRowGrp, 0, pFormat);
					m_pXL->SetMergeCell(stRowGrp, 0, stRow + -1, 0);
					m_pXL->SetValue(stRowGrp, 0, SigrD.strName);
				}
				stRow++;
			}
		}
	}

	if (bWrited == FALSE && m_pXL->IsExistSheetName(strSheetName))
		m_pXL->DeleteSheet(strSheetName);
}

void CSeisEvalReportFlexure::WriteAxisOne(IN std::tuple<T_SIPA_K, T_SIGR_K, T_ELEM_K, CString>& tParam,
	OUT int& stRow, OUT BOOL& bWrited)
{
	int rowCnt = 0;
	CString RptBlock;
	for (int iExamShearDir = 0; iExamShearDir < 2; iExamShearDir++) {
		const int sDir = iExamShearDir == 0 ? seis_jp::kY_Dir : seis_jp::kZ_Dir;

		T_SIBD_EVAL_MOMENT eval;
		eval.kSipa = std::get<0>(tParam);
		eval.kSigr = std::get<1>(tParam);
		eval.kElem = std::get<2>(tParam);
		eval.nExamShearDir = sDir;
		if (!m_pResult->GetResultSibdMoment(eval)) continue;

		RptBlock = _T("_02.Flexure_Body");
		rowCnt = CopyRange(m_pXL, RptBlock, stRow);
		m_pXL->SetValue(stRow, 0, std::get<3>(tParam));
		m_pXL->SetValue(stRow, 1, eval.kElem);

		CString strCheckDir;
		WriteReportDir(m_nSeisDir, iExamShearDir + 1, eType::kFlexure, strCheckDir);
		m_pXL->SetValue(stRow, 3, strCheckDir);

		// I단
		WriteData(&eval.analMoment_i, &eval.result_i, &eval.calcMomeMax, &eval.calcMomeMin, stRow, bWrited);
		// J단
		WriteData(&eval.analMoment_j, &eval.result_j, &eval.calcMomeMax, &eval.calcMomeMin, stRow +3, bWrited);

		stRow += rowCnt;
		if (m_pProgress->NextStep() == FALSE) throw FALSE;
	}
}

void CSeisEvalReportFlexure::WriteAxisTwo(IN std::tuple<T_SIPA_K, T_SIGR_K, T_ELEM_K, CString>& tParam,
	OUT int& stRow, OUT BOOL& bWrited)
{
	int rowCnt = 0;
	CString RptBlock;
	BOOL bWriteTable = FALSE;
	for (int iExamShearDir = 0; iExamShearDir < 2; iExamShearDir++) {
		const int sDir = iExamShearDir == 0 ? seis_jp::kY_Dir : seis_jp::kZ_Dir;

		T_SIBD_EVAL_MOMENT eval;
		eval.kSipa = std::get<0>(tParam);
		eval.kSigr = std::get<1>(tParam);
		eval.kElem = std::get<2>(tParam);
		eval.nExamShearDir = sDir;
		if (!m_pResult->GetResultSibdMoment(eval)) continue;

		if (bWriteTable == FALSE) {
			bWriteTable = TRUE;
			RptBlock = _T("_02.Flexure_Body2");
			rowCnt = CopyRange(m_pXL, RptBlock, stRow);

			m_pXL->SetValue(stRow, 0, std::get<3>(tParam));
			m_pXL->SetValue(stRow, 1, eval.kElem);
		}

		int nRow = stRow;
		// I단
		if (eval.nExamShearDir == seis_jp::kY_Dir)
			nRow += 3;

		WriteData(&eval.analMoment_i, &eval.result_i, &eval.calcMomeMax, &eval.calcMomeMin, nRow, bWrited);

		// J단
		nRow = stRow + 6;
		if (eval.nExamShearDir == seis_jp::kY_Dir)
			nRow += 3;

		WriteData(&eval.analMoment_j, &eval.result_j, &eval.calcMomeMax, &eval.calcMomeMin, nRow, bWrited);
		if (m_pProgress->NextStep() == FALSE) throw FALSE;
	}

	if (bWrited)
		stRow += rowCnt;
}

void CSeisEvalReportFlexure::WriteData(const T_SIBD_EVAL_ANAL_VALUE* analMoment, const T_SIBD_EVAL_RESULT_RC* result, const T_SIBD_EVAL_CALC_MPhi* calcMax, const T_SIBD_EVAL_CALC_MPhi* calcMin, const int& nRow, OUT BOOL& bWrited)
{
	auto l_WriteData = [this](const int nRow,const  int nCol,
		const vector<double> dResult)
		{
			WriteNanHyphenValue(nRow, nCol, dResult[0]);
			WriteNanHyphenValue(nRow + 1, nCol, dResult[1]);
			WriteNanHyphenValue(nRow + 2, nCol, dResult[2]);
			for (int i = 3; i < dResult.size(); i++)
			{
				int nColstart = i - 2; // nCol + 1 시작
				if (i == dResult.size() - 1)
				{
					CString strValue = dResult[i] > 1.0 ? _T("NG") : _T("OK");
					m_pXL->SetValue(nRow, nCol + nColstart, strValue);
					if (strValue == _T("NG")) SetNG_Red(m_pXL, nRow, nCol + nColstart, nRow, nCol + nColstart);
				}
				else
					WriteNanHyphenValue(nRow, nCol + nColstart, dResult[i]);
			}
		};

	m_pXL->SetValue(nRow, 2, result->kNode);

	vector<double> dMinResult, dMaxResult;
	dMinResult.resize(3, NAN); dMaxResult.resize(3, NAN);
	// Min
	const int& nMinThis = min(analMoment->aSmin.GetSize(), 3);
	for (int i = 0; i < nMinThis; i++) dMinResult[i] = (analMoment->aSmin.GetAt(i));
	dMinResult.push_back(analMoment->dAvergeMin);
	dMinResult.push_back(calcMin->dC);
	dMinResult.push_back(calcMin->dY);
	dMinResult.push_back(calcMin->dU);
	dMinResult.push_back(result->dRatioMin);
	l_WriteData(nRow, 4, dMinResult);

	// Max
	const int& nMaxThis = min(analMoment->aSmax.GetSize(), 3);
	for (int i = 0; i < nMaxThis; i++) dMaxResult[i] = (analMoment->aSmax.GetAt(i));
	dMaxResult.push_back(analMoment->dAvergeMax);
	dMaxResult.push_back(calcMax->dC);
	dMaxResult.push_back(calcMax->dY);
	dMaxResult.push_back(calcMax->dU);
	dMaxResult.push_back(result->dRatioMax);
	l_WriteData(nRow, 10, dMaxResult);

	bWrited = TRUE;
}

#pragma endregion

#pragma region CSeisEvalReportShear 전단

CSeisEvalReportShearBase::CSeisEvalReportShearBase(IExcel* pXL, CDgnProgressDlg* pProgress, const CString& strBasePath, const CString& strBaseBook, const SDgnExcelPrintOpt* pPrintOpt)
	: CWriteSeisEvalReportBase(pXL, pProgress, strBasePath, strBaseBook, pPrintOpt)
{
}

CSeisEvalReportShearBase::~CSeisEvalReportShearBase()
{
}

BOOL CSeisEvalReportShearBase::IsExistData(std::map<std::tuple<int, int, int, int>, std::vector<T_SISR_K>>& mExistSisr)
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
		if (SigrD.nSubType != this->m_subType)								continue;
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
			if (this->m_subType == T_SIGR_D::kUpper || this->m_subType == T_SIGR_D::kUser)
				mExistSisr[std::make_tuple(nEarthquakeType, nSeisDir, nSeisForce, D_MBTP_BEAM)].push_back(SisrK);
		}
	}

	if (mExistSisr.size() < 1) return FALSE;
	else return TRUE;
}

BOOL CSeisEvalReportShearBase::IsExistData(std::map<std::tuple<int, int, int>, std::vector<T_SISR_K>>& mExistSisr)
{
	// Result for Shear
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
		if (SigrD.nSubType != this->m_subType)								continue;
		T_SIPA_D SipaD;
		if (!m_pDoc->m_pAttrCtrl2->GetQSipa()->Get(SisrD.kSipa, SipaD)) continue;
		if (SipaD.nDesignCode != m_nDesignCode)							continue;

		//const auto nEarthquakeType = (int)SipaD.nEarthquakeType;
		const auto nSeisDir = (int)SipaD.nSeismicForceDirection;
		const auto nSeisForce = (int)SipaD.nSeismicForceType;
		T_SIGR_LOWER_D SigrLowerD;
		if (m_pDoc->m_pAttrCtrl2->GetQSigrLower()->Get(SisrD.kSigr, SigrLowerD))
		{
			if (SigrLowerD.nMaterialType == T_SIGR_LOWER_D::kRC)
			{
				if (SigrLowerD.BEAM_D.GetSize() > 0)
					mExistSisr[std::make_tuple(nSeisDir, nSeisForce, D_MBTP_BEAM)].push_back(SisrK);
				if (SigrLowerD.COLM_D.GetSize() > 0)
					mExistSisr[std::make_tuple(nSeisDir, nSeisForce, D_MBTP_COLUMN)].push_back(SisrK);
			}
		}
		else
		{
			if (this->m_subType == T_SIGR_D::kUpper || this->m_subType == T_SIGR_D::kUser)
				mExistSisr[std::make_tuple(nSeisDir, nSeisForce, D_MBTP_BEAM)].push_back(SisrK);
		}
	}

	if (mExistSisr.size() < 1) return FALSE;
	else return TRUE;
}

void CSeisEvalReportShearBase::WriteCommon(const std::vector<T_SISR_K>& vec, const BOOL& bMax, const int& iMember, int& stRow, BOOL& bWrited)
{
	// Cal for Shear
	for (const auto& SisrK : vec)
	{
		T_SISR_D SisrD;
		if (!m_pDoc->m_pAttrCtrl2->GetQSisr()->Get(SisrK, SisrD)) continue;

		T_SIGR_D SigrD;
		if (!m_pDoc->m_pAttrCtrl2->GetQSigr()->Get(SisrD.kSigr, SigrD)) continue;

		T_ELEM_K_LIST aElemKeys; std::map <T_ELEM_K, std::tuple<CString, int>> mElemInfo;
		GetTargetElements(SisrD.kSigr, this->m_subType, T_SIGR_LOWER_D::kRC, iMember, aElemKeys, mElemInfo);
		if (aElemKeys.GetCount() < 1) continue;

		const int stRowGrp = stRow;
		const int nMemb = aElemKeys.GetSize();
		for (int k = 0; k < nMemb; ++k)
		{
			const auto ElemK = aElemKeys[k];
			if (mElemInfo.find(ElemK) == mElemInfo.end()) continue;
			const int nShearDir = std::get<1>(mElemInfo[ElemK]);

			std::tuple<T_SIPA_K, T_SIGR_K, T_ELEM_K> tParam;
			tParam = std::make_tuple(SisrD.kSipa, SisrD.kSigr, ElemK);

			if (nShearDir == seis_jp::kYZ_Dir) {
				WriteAxisTwo(bMax, tParam, stRow, bWrited);
			}
			else {
				WriteAxisOne(bMax, tParam, stRow, bWrited);
			}
		}
		auto* pFormat = m_pXL->GetSheet()->cellFormat(stRow - 1, 0);
		m_pXL->GetSheet()->setCellFormat(stRowGrp, 0, pFormat);
		m_pXL->SetMergeCell(stRowGrp, 0, stRow + -1, 0);
		m_pXL->SetValue(stRowGrp, 0, SigrD.strName);
	}
}

void CSeisEvalReportShearBase::WriteCommon(const std::vector<T_SISR_K>& vec, const int& iMember, int& stRow, BOOL& bWrited)
{
	// Result for Shear
	for (const auto& SisrK : vec)
	{
		T_SISR_D SisrD;
		if (!m_pDoc->m_pAttrCtrl2->GetQSisr()->Get(SisrK, SisrD)) continue;

		T_SIGR_D SigrD;
		if (!m_pDoc->m_pAttrCtrl2->GetQSigr()->Get(SisrD.kSigr, SigrD)) continue;

		T_ELEM_K_LIST aElemKeys; std::map <T_ELEM_K, std::tuple<CString, int>> mElemInfo;
		GetTargetElements(SisrD.kSigr, this->m_subType, T_SIGR_LOWER_D::kRC, iMember, aElemKeys, mElemInfo);
		if (aElemKeys.GetCount() < 1) continue;

		const int stRowGrp = stRow;
		const int nMemb = aElemKeys.GetSize();
		for (int k = 0; k < nMemb; ++k)
		{
			const T_ELEM_K& ElemK = aElemKeys[k];
			if (mElemInfo.find(ElemK) == mElemInfo.end()) continue;
			const int nShearDir = std::get<1>(mElemInfo[ElemK]);

			std::tuple<T_SIPA_K, T_SIGR_K, T_ELEM_K, CString> tParam;
			tParam = std::make_tuple(SisrD.kSipa, SisrD.kSigr, ElemK, SigrD.strName);

			if (nShearDir == seis_jp::kYZ_Dir) {
				WriteAxisTwo(tParam, stRow, bWrited);
			}
			else {
				WriteAxisOne(tParam, stRow, bWrited);
			}
		}
		auto* pFormat = m_pXL->GetSheet()->cellFormat(stRow - 1, 0);
		m_pXL->GetSheet()->setCellFormat(stRowGrp, 0, pFormat);
		m_pXL->SetMergeCell(stRowGrp, 0, stRow + -1, 0);
		m_pXL->SetValue(stRowGrp, 0, SigrD.strName);
	}
}

#pragma region // CSeisEvalReportShearSus
CSeisEvalReportShearSus::CSeisEvalReportShearSus(IExcel* pXL, CDgnProgressDlg* pProgress, const CString& strBasePath, const CString& strBaseBook, const SDgnExcelPrintOpt* pPrintOpt)
	: CSeisEvalReportShearBase(pXL, pProgress, strBasePath, strBaseBook, pPrintOpt)
{
}

CSeisEvalReportShearSus::~CSeisEvalReportShearSus()
{
}

void CSeisEvalReportShearSus::AddStep()
{
	CArray<T_SISR_K, T_SISR_K> aSisrK;
	m_pDoc->m_pAttrCtrl2->GetQSisr()->GetKeyList(aSisrK);
	const int nSisr = aSisrK.GetSize();
	if (nSisr == 0) return;

	std::vector<T_SISR_K> vExistSisr[2][2];
	for (int i = 0; i < nSisr; ++i)
	{
		const T_SISR_K SisrK = aSisrK[i];
		T_SISR_D SisrD;
		if (!m_pDoc->m_pAttrCtrl2->GetQSisr()->Get(SisrK, SisrD))		continue;
		T_SIGR_D SigrD;
		if (!m_pDoc->m_pAttrCtrl2->GetQSigr()->Get(SisrD.kSigr, SigrD)) continue;
		if (SigrD.nSubType != this->m_subType)							continue;
		T_SIPA_D SipaD;
		if (!m_pDoc->m_pAttrCtrl2->GetQSipa()->Get(SisrD.kSipa, SipaD)) continue;
		if (SipaD.nDesignCode != m_nDesignCode)							continue;

		const auto iIdx1 = (int)SipaD.nSeismicForceDirection - 1;
		const auto iIdx2 = (int)SipaD.nSeismicForceType - 1;
		vExistSisr[iIdx1][iIdx2].push_back(SisrK);
	}

	if (vExistSisr[0][0].size() == 0 && vExistSisr[0][1].size() == 0 &&
		vExistSisr[1][0].size() == 0 && vExistSisr[1][1].size() == 0) return;

	// せん断耐力 Sus
	for (int iSeisDir = 0; iSeisDir < 2; ++iSeisDir) // 橋軸方向, 橋軸直角方向
	{
		if (vExistSisr[iSeisDir][0].size() == 0 && vExistSisr[iSeisDir][1].size() == 0) continue;

		for (int iType = 0; iType < 2; ++iType) // タイプ type
		{
			if (vExistSisr[iSeisDir][iType].size() == 0) continue;

			for (int mm = 0; mm < 2; ++mm) // max/min
			{
				for (const auto& SisrK : vExistSisr[iSeisDir][iType])
				{
					T_SISR_D SisrD;
					if (!m_pDoc->m_pAttrCtrl2->GetQSisr()->Get(SisrK, SisrD)) continue;

					T_SIGR_D SigrD;
					if (!m_pDoc->m_pAttrCtrl2->GetQSigr()->Get(SisrD.kSigr, SigrD)) continue;

					T_ELEM_K_LIST aElemKeys; std::map <T_ELEM_K, std::tuple<CString, int>> mElemInfo;
					GetTargetElements(SisrD.kSigr, this->m_subType, aElemKeys, mElemInfo);
					if (aElemKeys.GetCount() < 1) continue;

					const int nMemb = aElemKeys.GetSize();
					for (int k = 0; k < nMemb; ++k)
					{
						const auto ElemK = aElemKeys[k];
						if (mElemInfo.find(ElemK) == mElemInfo.end()) continue;

						//const CString& sGroup = std::get<0>(mElemInfo[ElemK]);
						//const int nShearDir = std::get<1>(mElemInfo[ElemK]);

						for(int d= 0; d < 2; ++d) // y, z
						{
							int nShearDir = d == 0 ? seis_jp::kY_Dir : seis_jp::kZ_Dir;
							T_SISR_EVAL_ROAD3 eval;
							eval.kSipa = SisrD.kSipa;
							eval.kSigr = SisrD.kSigr;
							eval.kElem = ElemK;
							eval.nExamShearDir = nShearDir;
							if (!m_pResult->GetResultSisrRoad3(eval)) continue;
							m_pProgress->m_nTotalSteps++;
						}
					}
				}
			}
		}
	}
}

void CSeisEvalReportShearSus::Write()
{
//	2. 上部構造の照査 상부구조조사
//		2-2. せん断耐力 Sus
//			2-2-1. 橋軸方向 교축방향
//				1) タイプⅠ
//				2) タイプⅡ
//			2-2-2. 橋軸直角方向 교축직각방향
//				1) タイプⅠ
//				2) タイプⅡ

	std::map<std::tuple<int, int, int, int>, std::vector<T_SISR_K>> mExistSisr;
	if (!IsExistData(mExistSisr)) return;

	CString RptBlock;
	int rowCnt = 0;
	int stRow = 0;

	CString strSheetName;
	AddSheet(this->m_subType, strSheetName);
	*this->m_pParagraph1 = *this->m_pParagraph1 + 1;

	BOOL bWrited = FALSE;
	int iParagraph3 = 0;
	int iParagraph4 = 0;
	int iParagraph5 = 0;

	int nEarthquakeType = -1;
	int nSeisDir = T_SIPA_D::kUnKnown; // 1: 교축방향, 2: 교축직각방향
	int nType = T_SIPA_D::kUnKnown; // 1: 타입Ⅰ, 2: 타입Ⅱ
	int nMember = D_MBTP_ETC; // 1: 梁部, 2: 柱部
	for (const auto& pair : mExistSisr)
	{
		const auto& key = pair.first;
		const auto& vec = pair.second;
		if (vec.size() == 0) continue;

		const int iEathquake = std::get<0>(key);
		const int iSeisDir = std::get<1>(key);
		const int iType = std::get<2>(key);
		const int iMember = std::get<3>(key);
		if (nEarthquakeType != iEathquake)
		{
			WirteTheory(this->m_subType, nEarthquakeType, *this->m_pParagraph1, *this->m_pParagraph2, stRow);
			nEarthquakeType = iEathquake;
		}

		if (nSeisDir != iSeisDir)
		{
			RptBlock = _T("_02.Shear_Sus_Dir");
			WriteSeisDir(RptBlock, iSeisDir, iParagraph3, stRow);
			nSeisDir = iSeisDir;
			nType = T_SIPA_D::kUnKnown; // 초기화
		}

		if (nType != iType)
		{
			RptBlock = _T("_02.Shear_Sus_Type");
			WriteSeisType(RptBlock, iType, iParagraph4, stRow);
			nType = iType;
			nMember = D_MBTP_ETC; // 초기화
		}

		for(int mm= 0; mm < 2; ++mm) // max/min
		{
			BOOL bMax = mm == 0 ? TRUE : FALSE;
			int nDirRow = stRow;
			RptBlock = _T("_02.Shear_Sus_Head1");
			WriteSeisPos(RptBlock, iMember, iParagraph5, stRow);
			m_pXL->SetValue(nDirRow, 0, bMax ? _LS(IDS_BRDG_SEIS_EVAL_REPORT_JP_DIRECTION_PLUS) : _LS(IDS_BRDG_SEIS_EVAL_REPORT_JP_DIRECTION_MINUS));// ◎方向：(+)方向
			nMember = iMember;

			WriteCommon(vec, bMax, iMember, stRow, bWrited);
			stRow++;
		}
	}

	if (bWrited == FALSE && m_pXL->IsExistSheetName(strSheetName))
		m_pXL->DeleteSheet(strSheetName);
}

void CSeisEvalReportShearSus::WriteAxisOne(const BOOL& bMax, IN std::tuple<T_SIPA_K, T_SIGR_K, T_ELEM_K>& tParam, 
	OUT int& stRow, OUT BOOL& bWrited)
{
	auto l_WriteData = [this](int nRow, int nCol, bool bAuto, const T_SISR_EVAL_ROAD3_CAPA_DIAG& resD)
	{
		if (bAuto)
		{
			WriteNanHyphenValue(nRow, nCol + 0, resD.dB * m_dLenToMM);
			WriteNanHyphenValue(nRow, nCol + 1, resD.dD * m_dLenToMM);
			WriteNanHyphenValue(nRow, nCol + 2, resD.dM0);
			WriteNanHyphenValue(nRow, nCol + 3, resD.dMd);
			WriteNanHyphenValue(nRow, nCol + 4, resD.dK);
			WriteNanHyphenValue(nRow, nCol + 5, resD.dSc);
			WriteNanHyphenValue(nRow, nCol + 6, resD.dSs);
			WriteNanHyphenValue(nRow, nCol + 7, resD.dSp);
		}
		WriteNanHyphenValue(nRow, nCol + 8, resD.dSus);
	};

	int rowCnt = 0;
	CString RptBlock;
	for (int iExamShearDir = 0; iExamShearDir < 2; iExamShearDir++) {
		const int sDir = iExamShearDir == 0 ? seis_jp::kY_Dir : seis_jp::kZ_Dir;

		T_SISR_EVAL_ROAD3 eval;
		eval.kSipa = std::get<0>(tParam);
		eval.kSigr = std::get<1>(tParam);
		eval.kElem = std::get<2>(tParam);
		eval.nExamShearDir = sDir;
		if (!m_pResult->GetResultSisrRoad3(eval)) continue;

		RptBlock = _T("_02.Shear_Sus_Body");
		rowCnt = CopyRange(m_pXL, RptBlock, stRow);

		m_pXL->SetValue(stRow, 1, eval.kElem);
		CString strCheckDir;
		WriteReportDir(m_nSeisDir, iExamShearDir+1, eType::kShear, strCheckDir);

		m_pXL->SetValue(stRow, 3, strCheckDir);
		m_pXL->SetMergeCell(stRow, 3, stRow + 1, 3);

		int nRow = stRow;
		for (int l = 0; l < 2; ++l) // I, J
		{
			const auto& PosD = l == 0 ? eval.PosI : eval.PosJ;

			m_pXL->SetValue(nRow, 2, PosD.NodeK);

			if (bMax == TRUE)
			{
				// 最大
				l_WriteData(nRow, 4, PosD.RsltMax.bAutoCapa, PosD.RsltMax.CapaDiag);
			}
			else
			{
				// 最小
				l_WriteData(nRow, 4, PosD.RsltMin.bAutoCapa, PosD.RsltMin.CapaDiag);
			}
			nRow += 1;
			bWrited = TRUE;
			if (m_pProgress->NextStep() == FALSE) throw FALSE;
		}
		stRow += rowCnt;
	}
}

void CSeisEvalReportShearSus::WriteAxisTwo(const BOOL& bMax, IN std::tuple<T_SIPA_K, T_SIGR_K, T_ELEM_K>& tParam, OUT int& stRow, OUT BOOL& bWrited)
{
	auto L_GetEvalData = [&](seis_jp::eDirShear sDir, IN std::tuple<T_SIPA_K, T_SIGR_K, T_ELEM_K>& tParam, OUT T_SISR_EVAL_ROAD3& rEval) {
		rEval.kSipa = std::get<0>(tParam);
		rEval.kSigr = std::get<1>(tParam);
		rEval.kElem = std::get<2>(tParam);
		rEval.nExamShearDir = sDir;
		return m_pResult->GetResultSisrRoad3(rEval);
	};

	auto L_WriteData = [&](const int& nExamShearDir, IN T_SISR_EVAL_ROAD3_POS& PosD, int nRow) {
		m_pXL->SetValue(nRow, 2, PosD.NodeK);

		const BOOL& bAutoCapa = bMax ? PosD.RsltMax.bAutoCapa : PosD.RsltMin.bAutoCapa;
		const T_SISR_EVAL_ROAD3_CAPA_DIAG& resD = bMax ? PosD.RsltMax.CapaDiag : PosD.RsltMin.CapaDiag;

		if ( nExamShearDir == seis_jp::kY_Dir )
			nRow += 1;

		if (bAutoCapa) {
			WriteNanHyphenValue(nRow, 4, resD.dB * m_dLenToMM);
			WriteNanHyphenValue(nRow, 5, resD.dD * m_dLenToMM);
			WriteNanHyphenValue(nRow, 6, resD.dM0);
			WriteNanHyphenValue(nRow, 7, resD.dMd);
			WriteNanHyphenValue(nRow, 8, resD.dK);
			WriteNanHyphenValue(nRow, 9, resD.dSc);
			WriteNanHyphenValue(nRow, 10, resD.dSs);
			WriteNanHyphenValue(nRow, 11, resD.dSp);
		}
		WriteNanHyphenValue(nRow, 12, resD.dSus);
	};

	T_ELEM_K kElem = std::get<2>(tParam);
	T_SISR_EVAL_ROAD3 yEval, zEval;
	BOOL bResY = L_GetEvalData(seis_jp::kY_Dir, tParam, yEval);
	BOOL bResZ = L_GetEvalData(seis_jp::kZ_Dir, tParam, zEval);
	if(bResY == FALSE && bResZ == FALSE) return;

	CString RptBlock = _T("_02.Shear_Sus_Body1");
	int rowCnt = CopyRange(m_pXL, RptBlock, stRow);

	m_pXL->SetValue(stRow, 1, kElem);

	if (bResZ == TRUE) {
		L_WriteData(zEval.nExamShearDir, zEval.PosI, stRow);
		L_WriteData(zEval.nExamShearDir, zEval.PosJ, stRow+2);
	}
	if(bResY == TRUE) {
		L_WriteData(yEval.nExamShearDir, yEval.PosI, stRow);
		L_WriteData(yEval.nExamShearDir, yEval.PosJ, stRow+2);
	}

	bWrited = TRUE;
	stRow += rowCnt;
	if (m_pProgress->NextStep() == FALSE) throw FALSE;
}

void CSeisEvalReportShearSus::AddSheet(const T_SIGR_D::eSubType& nGroupType, OUT CString& rSheetName)
{
	m_strBaseSheet = _LSX(02.Shear_Sus);
	rSheetName = _LS(IDS_BRDG_SEIS_EVAL_REPORT_JP_SHEAR_Sus) + _T("_") + GetTextGroup(nGroupType);// _T("せん断(斜引張破壊)");
	if (m_pXL->IsExistSheetName(rSheetName))
	{
		ASSERT(0);
		m_pXL->SetActiveSheet(rSheetName);
	}
	else
	{
		m_pXL->AddSheet2(rSheetName);
		m_pXL->SetActiveSheet(rSheetName);
	}

	m_pXL->SetStandardFont(_T("Meiryo UI"));
	m_pXL->SetMargin(13, 12, 20, 17);
	m_pXL->SetColumnWidth(0, 0, 0, 2, 7.0 * _ToPoint); // 7.0 * 0.833333333
	m_pXL->SetColumnWidth(0, 3, 0, 60, 8.08 * _ToPoint); // 8.08 * 0.833333333
	m_pXL->SetRowHeight(16.5);
	m_pXL->SetZoom(70);
}

void CSeisEvalReportShearSus::WirteTheory(const T_SIGR_D::eSubType& nGroupType, const int& nEarthquakeType, const int iParagraph1, int& iParagraph2, OUT int& stRow)
{
	CString RptBlock = _T("_02.Shear_Sus_Title");
	int rowCnt = CopyRange(m_pXL, RptBlock, stRow);
	m_pXL->SetValue(stRow, 0, GetText(_T("%d. %s"), iParagraph1, GetTextTitle(nGroupType)));//上部構造の照査
	stRow += rowCnt;

	RptBlock = _T("_02.Shear_Sus_Theory1");
	rowCnt = CopyRange(m_pXL, RptBlock, stRow);
	m_pXL->SetValue(stRow, 0, GetText(_T("%d-%d. %s Sus"), iParagraph1, ++iParagraph2, _LS(IDS_BRDG_SEIS_EVAL_REPORT_JP_SHEAR_STR)));// せん断耐力
	stRow += rowCnt;
	stRow++;
}
#pragma endregion

#pragma region // CSeisEvalReportShearSusResult
CSeisEvalReportShearSusResult::CSeisEvalReportShearSusResult(IExcel* pXL, CDgnProgressDlg* pProgress, const CString& strBasePath, const CString& strBaseBook, const SDgnExcelPrintOpt* pPrintOpt)
	: CSeisEvalReportShearBase(pXL, pProgress, strBasePath, strBaseBook, pPrintOpt)
{
}

CSeisEvalReportShearSusResult::~CSeisEvalReportShearSusResult()
{
}

void CSeisEvalReportShearSusResult::AddStep()
{
	CArray<T_SISR_K, T_SISR_K> aSisrK;
	m_pDoc->m_pAttrCtrl2->GetQSisr()->GetKeyList(aSisrK);
	const int nSisr = aSisrK.GetSize();
	if (nSisr == 0) return;

	std::vector<T_SISR_K> vExistSisr[2][2];
	for (int i = 0; i < nSisr; ++i)
	{
		const T_SISR_K SisrK = aSisrK[i];
		T_SISR_D SisrD;
		if (!m_pDoc->m_pAttrCtrl2->GetQSisr()->Get(SisrK, SisrD))		continue;
		T_SIGR_D SigrD;
		if (!m_pDoc->m_pAttrCtrl2->GetQSigr()->Get(SisrD.kSigr, SigrD)) continue;
		if (SigrD.nSubType != this->m_subType)							continue;
		T_SIPA_D SipaD;
		if (!m_pDoc->m_pAttrCtrl2->GetQSipa()->Get(SisrD.kSipa, SipaD)) continue;
		if (SipaD.nDesignCode != m_nDesignCode)							continue;

		const auto iIdx1 = (int)SipaD.nSeismicForceDirection - 1;
		const auto iIdx2 = (int)SipaD.nSeismicForceType - 1;
		vExistSisr[iIdx1][iIdx2].push_back(SisrK);
	}

	if (vExistSisr[0][0].size() == 0 && vExistSisr[0][1].size() == 0 &&
		vExistSisr[1][0].size() == 0 && vExistSisr[1][1].size() == 0) return;

	// せん断の照査
	for (int iSeisDir = 0; iSeisDir < 2; ++iSeisDir) // 橋軸方向, 橋軸直角方向
	{
		if (vExistSisr[iSeisDir][0].size() == 0 && vExistSisr[iSeisDir][1].size() == 0) continue;

		for (int iType = 0; iType < 2; ++iType) // タイプ type
		{
			if (vExistSisr[iSeisDir][iType].size() == 0) continue;

			for (const auto& SisrK : vExistSisr[iSeisDir][iType])
			{
				T_SISR_D SisrD;
				if (!m_pDoc->m_pAttrCtrl2->GetQSisr()->Get(SisrK, SisrD)) continue;

				T_SIGR_D SigrD;
				if (!m_pDoc->m_pAttrCtrl2->GetQSigr()->Get(SisrD.kSigr, SigrD)) continue;

				T_ELEM_K_LIST aElemKeys; std::map <T_ELEM_K, std::tuple<CString, int>> mElemInfo;
				GetTargetElements(SisrD.kSigr, this->m_subType, aElemKeys, mElemInfo);
				if (aElemKeys.GetCount() < 1) continue;

				const int nMemb = aElemKeys.GetSize();
				for (int k = 0; k < nMemb; ++k)
				{
					const auto ElemK = aElemKeys[k];
					if(mElemInfo.find(ElemK) == mElemInfo.end() ) continue;
					
					//const CString& sGroup = std::get<0>(mElemInfo[ElemK]);
					//const int nShearDir = std::get<1>(mElemInfo[ElemK]);

					for (int d = 0; d < 2; ++d) // y, z
					{
						int nShearDir = d == 0 ? seis_jp::kY_Dir : seis_jp::kZ_Dir;
						T_SISR_EVAL_ROAD3 eval;
						eval.kSipa = SisrD.kSipa;
						eval.kSigr = SisrD.kSigr;
						eval.kElem = ElemK;
						eval.nExamShearDir = nShearDir;
						if (!m_pResult->GetResultSisrRoad3(eval)) continue;
						m_pProgress->m_nTotalSteps++;
					}
				}
			}
		}
	}
}

void CSeisEvalReportShearSusResult::Write()
{
	//	2. 上部構造の照査 상부구조조사
	//		2-3. せん断の照査
	//			2-3-1. 橋軸方向 교축방향
	//				1) タイプⅠ
	//				2) タイプⅡ
	//			2-3-2. 橋軸直角方向 교축직각방향
	//				1) タイプⅠ
	//				2) タイプⅡ

	std::map<std::tuple<int, int, int>, std::vector<T_SISR_K>> mExistSisr;
	if (!IsExistData(mExistSisr)) return;

	CString RptBlock;
	int rowCnt = 0;
	int stRow = 0;

	CString strSheetName;
	AddSheet(this->m_subType, strSheetName);
	WirteTheory(this->m_subType, *this->m_pParagraph1, *this->m_pParagraph2, stRow);
	*this->m_pParagraph1 = *this->m_pParagraph1 + 1;

	BOOL bWrited = FALSE;
	int iParagraph3 = 0;
	int iParagraph4 = 0;
	int iParagraph5 = 0;

	//int nEarthquakeType = -1;;
	int nSeisDir = T_SIPA_D::kUnKnown; // 1: 교축방향, 2: 교축직각방향
	int nType = T_SIPA_D::kUnKnown; // 1: 타입Ⅰ, 2: 타입Ⅱ
	int nMember = D_MBTP_ETC; // 1: 梁部, 2: 柱部
	for (const auto& pair : mExistSisr)
	{
		const auto& key = pair.first;
		const auto& vec = pair.second;
		if (vec.size() == 0) continue;

		//const int iEathquake = std::get<0>(key);
		const int iSeisDir = std::get<0>(key);
		const int iType = std::get<1>(key);
		const int iMember = std::get<2>(key);

		if (nSeisDir != iSeisDir)
		{
			RptBlock = _T("_02.Shear_Sus_Dir");
			WriteSeisDir(RptBlock, iSeisDir, iParagraph3, stRow);
			nSeisDir = iSeisDir;
			nType = T_SIPA_D::kUnKnown; // 초기화
		}

		if (nType != iType)
		{
			RptBlock = _T("_02.Shear_Sus_Type");
			WriteSeisType(RptBlock, iType, iParagraph4, stRow);
			nType = iType;
			nMember = D_MBTP_ETC; // 초기화
		}

		if(nMember != iMember)
		{
			RptBlock = _T("_02.Shear_Sus_Head2");
			WriteSeisPos(RptBlock, iMember, iParagraph5, stRow);
			nMember = iMember;
		}

		WriteCommon(vec, iMember, stRow, bWrited);
		stRow++;
	}

	if (bWrited == FALSE && m_pXL->IsExistSheetName(strSheetName))
		m_pXL->DeleteSheet(strSheetName);
}

void CSeisEvalReportShearSusResult::WriteData(const T_SISR_EVAL_ROAD3_POS* pPosD, const int& nRow, OUT BOOL& bWrited)
{
	m_pXL->SetValue(nRow, 2, pPosD->NodeK);

	const int& nThis = min(pPosD->RsltMin.Forc.adShear.GetSize(), 3);

	const auto& MinForc = pPosD->RsltMin.Forc;
	for (int m = 0; m < nThis; ++m)
	{
		WriteNanHyphenValue(nRow + m, 4, MinForc.adShear[m]);
		WriteNanHyphenValue(nRow + m, 5, MinForc.adShearh[m]);
	}
	WriteNanHyphenValue(nRow, 6, MinForc.dShear);
	WriteNanHyphenValue(nRow, 7, pPosD->RsltMin.CapaDiag.dSus);
	if (pPosD->RsltMin.CapaDiag.dRatio > 1.0)
	{
		m_pXL->SetValue(nRow, 8, _T("NG"));
		SetNG_Red(m_pXL, nRow, 8, nRow, 8);
	}
	else
	{
		m_pXL->SetValue(nRow, 8, _T("OK"));
	}

	const auto& MaxForc = pPosD->RsltMax.Forc;
	for (int m = 0; m < nThis; ++m)
	{
		WriteNanHyphenValue(nRow + m, 9, MaxForc.adShear[m]);
		WriteNanHyphenValue(nRow + m, 10, MaxForc.adShearh[m]);
	}
	WriteNanHyphenValue(nRow, 11, MaxForc.dShear);
	WriteNanHyphenValue(nRow, 12, pPosD->RsltMax.CapaDiag.dSus);
	if (pPosD->RsltMax.CapaDiag.dRatio > 1.0)
	{
		m_pXL->SetValue(nRow, 13, _T("NG"));
		SetNG_Red(m_pXL, nRow, 13, nRow, 13);
	}
	else
	{
		m_pXL->SetValue(nRow, 13, _T("OK"));
	}

	bWrited = TRUE;
}

void CSeisEvalReportShearSusResult::WriteAxisOne(IN std::tuple<T_SIPA_K, T_SIGR_K, T_ELEM_K, CString>& tParam,
	OUT int& stRow, OUT BOOL& bWrited)
{
	int rowCnt = 0;
	CString RptBlock;
	for (int iExamShearDir = 0; iExamShearDir < 2; iExamShearDir++) {
		const int sDir = iExamShearDir == 0 ? seis_jp::kY_Dir : seis_jp::kZ_Dir;

		T_SISR_EVAL_ROAD3 eval;
		eval.kSipa = std::get<0>(tParam);
		eval.kSigr = std::get<1>(tParam);
		eval.kElem = std::get<2>(tParam);
		eval.nExamShearDir = sDir;
		if (!m_pResult->GetResultSisrRoad3(eval)) continue;

		RptBlock = _T("_02.Shear_Sus_Body2");
		rowCnt = CopyRange(m_pXL, RptBlock, stRow);

		m_pXL->SetValue(stRow, 0, std::get<3>(tParam));
		m_pXL->SetValue(stRow, 1, eval.kElem);

		//CString strCheckDir = sDir == seis_jp::kZ_Dir ? _T("Fz") : _T("Fy");
		CString strCheckDir;
		WriteReportDir(m_nSeisDir,iExamShearDir+1, eType::kShear, strCheckDir);

		m_pXL->SetValue(stRow, 3, strCheckDir);

		WriteData(&eval.PosI, stRow, bWrited);
		WriteData(&eval.PosJ, stRow + 3, bWrited);

		stRow += rowCnt;
		if (m_pProgress->NextStep() == FALSE) throw FALSE;
	}
}

void CSeisEvalReportShearSusResult::WriteAxisTwo(IN std::tuple<T_SIPA_K, T_SIGR_K, T_ELEM_K, CString>& tParam,
	OUT int& stRow, OUT BOOL& bWrited)
{
	int rowCnt = 0;
	CString RptBlock;
	BOOL bWriteTable = FALSE;
	for (int iExamShearDir = 0; iExamShearDir < 2; iExamShearDir++) {
		const int sDir = iExamShearDir == 0 ? seis_jp::kY_Dir : seis_jp::kZ_Dir;

		T_SISR_EVAL_ROAD3 eval;
		eval.kSipa = std::get<0>(tParam);
		eval.kSigr = std::get<1>(tParam);
		eval.kElem = std::get<2>(tParam);
		eval.nExamShearDir = sDir;
		if (!m_pResult->GetResultSisrRoad3(eval)) continue;

		if (bWriteTable == FALSE) {
			bWriteTable = TRUE;
			RptBlock = _T("_02.Shear_Sus_Body3");
			rowCnt = CopyRange(m_pXL, RptBlock, stRow);

			m_pXL->SetValue(stRow, 0, std::get<3>(tParam));
			m_pXL->SetValue(stRow, 1, eval.kElem);
		}

		int nRow = stRow;
		// I단
		if (eval.nExamShearDir == seis_jp::kY_Dir)
			nRow += 3;

		WriteData(&eval.PosI, nRow, bWrited);

		// J단
		nRow = stRow + 6;
		if ( eval.nExamShearDir == seis_jp::kY_Dir)
			nRow += 3;

		WriteData(&eval.PosJ, nRow, bWrited);
		if (m_pProgress->NextStep() == FALSE) throw FALSE;
	}

	if (bWrited) 
		stRow += rowCnt;
}

void CSeisEvalReportShearSusResult::AddSheet(const T_SIGR_D::eSubType& nGroupType, OUT CString& rSheetName)
{
	m_strBaseSheet = _LSX(02.Shear_Sus);
	rSheetName = _LS(IDS_BRDG_SEIS_EVAL_REPORT_JP_SHEAR_Sus_Result) + _T("_") + GetTextGroup(nGroupType);// _T("せん断結果(斜引張破壊)");
	if (m_pXL->IsExistSheetName(rSheetName))
	{
		ASSERT(0);
		m_pXL->SetActiveSheet(rSheetName);
	}
	else
	{
		m_pXL->AddSheet2(rSheetName);
		m_pXL->SetActiveSheet(rSheetName);
	}

	m_pXL->SetStandardFont(_T("Meiryo UI"));
	m_pXL->SetMargin(13, 12, 20, 17);
	m_pXL->SetColumnWidth(0, 0, 0, 2, 7.0 * _ToPoint); // 7.0 * 0.833333333
	m_pXL->SetColumnWidth(0, 3, 0, 60, 8.08 * _ToPoint); // 8.08 * 0.833333333
	m_pXL->SetRowHeight(16.5);
	m_pXL->SetZoom(70);
}

void CSeisEvalReportShearSusResult::WirteTheory(const T_SIGR_D::eSubType& nGroupType, const int iParagraph1, int& iParagraph2, OUT int& stRow)
{
	CString RptBlock = _T("_02.Shear_Sus_Title");
	int rowCnt = CopyRange(m_pXL, RptBlock, stRow);
	m_pXL->SetValue(stRow, 0, GetText(_T("%d. %s"), iParagraph1, GetTextTitle(nGroupType)));
	stRow += rowCnt;

	RptBlock = _T("_02.Shear_Sus_Theory2");
	rowCnt = CopyRange(m_pXL, RptBlock, stRow);
	m_pXL->SetValue(stRow, 0, GetText(_T("%d-%d. %s"), iParagraph1, ++iParagraph2, _LS(IDS_BRDG_SEIS_EVAL_REPORT_JP_CHK_SHEAR)));// せん断耐力
	stRow += rowCnt;
	stRow++;
}
#pragma endregion

#pragma region // CSeisEvalReportShearSuc
CSeisEvalReportShearSuc::CSeisEvalReportShearSuc(IExcel* pXL, CDgnProgressDlg* pProgress, const CString& strBasePath, const CString& strBaseBook, const SDgnExcelPrintOpt* pPrintOpt)
	: CSeisEvalReportShearBase(pXL, pProgress, strBasePath, strBaseBook, pPrintOpt)
{
}

CSeisEvalReportShearSuc::~CSeisEvalReportShearSuc()
{
}

void CSeisEvalReportShearSuc::AddStep()
{
	CArray<T_SISR_K, T_SISR_K> aSisrK;
	m_pDoc->m_pAttrCtrl2->GetQSisr()->GetKeyList(aSisrK);
	const int nSisr = aSisrK.GetSize();
	if (nSisr == 0) return;

	std::vector<T_SISR_K> vExistSisr[2][2];
	for (int i = 0; i < nSisr; ++i)
	{
		const T_SISR_K SisrK = aSisrK[i];
		T_SISR_D SisrD;
		if (!m_pDoc->m_pAttrCtrl2->GetQSisr()->Get(SisrK, SisrD))		continue;
		T_SIGR_D SigrD;
		if (!m_pDoc->m_pAttrCtrl2->GetQSigr()->Get(SisrD.kSigr, SigrD)) continue;
		if (SigrD.nSubType != this->m_subType)							continue;
		T_SIPA_D SipaD;
		if (!m_pDoc->m_pAttrCtrl2->GetQSipa()->Get(SisrD.kSipa, SipaD)) continue;
		if (SipaD.nDesignCode != m_nDesignCode)							continue;

		const auto iIdx1 = (int)SipaD.nSeismicForceDirection - 1;
		const auto iIdx2 = (int)SipaD.nSeismicForceType - 1;
		vExistSisr[iIdx1][iIdx2].push_back(SisrK);
	}

	std::vector<BOOL>bCheck = { FALSE, FALSE };
	for (int i = 0; i < 2; i++)
	{
		if (vExistSisr[i][0].size() == 0 && vExistSisr[i][1].size() == 0)
			bCheck[i] = TRUE;
	}
	if (bCheck[0] && bCheck[1]) return;

	// せん断耐力 Suc
	for (int iSeisDir = 0; iSeisDir < 2; ++iSeisDir) // 橋軸方向, 橋軸直角方向
	{
		if (vExistSisr[iSeisDir][0].size() == 0 && vExistSisr[iSeisDir][1].size() == 0) continue;

		for (int iType = 0; iType < 2; ++iType) // タイプ type
		{
			if (vExistSisr[iSeisDir][iType].size() == 0) continue;

			for (const auto& SisrK : vExistSisr[iSeisDir][iType])
			{
				T_SISR_D SisrD;
				if (!m_pDoc->m_pAttrCtrl2->GetQSisr()->Get(SisrK, SisrD)) continue;

				T_SIGR_D SigrD;
				if (!m_pDoc->m_pAttrCtrl2->GetQSigr()->Get(SisrD.kSigr, SigrD)) continue;
				if (SigrD.nSubType != this->m_subType) continue;

				T_ELEM_K_LIST aElemKeys; std::map <T_ELEM_K, std::tuple<CString, int>> mElemInfo;
				GetTargetElements(SisrD.kSigr, this->m_subType, aElemKeys, mElemInfo);
				if (aElemKeys.GetCount() < 1) continue;

				const int nMemb = aElemKeys.GetSize();
				for (int k = 0; k < nMemb; ++k)
				{
					const auto ElemK = aElemKeys[k];
					if (mElemInfo.find(ElemK) == mElemInfo.end()) continue;

					//const CString& sGroup = std::get<0>(mElemInfo[ElemK]);
					//const int nShearDir = std::get<1>(mElemInfo[ElemK]);

					for (int d = 0; d < 2; d++)
					{
						int nShearDir = d == 0 ? seis_jp::kY_Dir : seis_jp::kZ_Dir;
						T_SISR_EVAL_ROAD3 eval;
						eval.kSipa = SisrD.kSipa;
						eval.kSigr = SisrD.kSigr;
						eval.kElem = ElemK;
						eval.nExamShearDir = nShearDir;
						if (!m_pResult->GetResultSisrRoad3(eval)) continue;
						m_pProgress->m_nTotalSteps++;
					}
				}
			}

		}
	}
}

void CSeisEvalReportShearSuc::Write()
{
//	2. 上部構造の照査 상부구조조사
//		2-4. せん断耐力 Suc
//			2-4-1. 橋軸方向 교축방향
//				1) タイプⅠ
//				2) タイプⅡ
//			2-4-2. 橋軸直角方向 교축직각방향
//				1) タイプⅠ
//				2) タイプⅡ
//		2-5. せん断の照査
//			2-5-1. 橋軸方向 교축방향
//				1) タイプⅠ
//				2) タイプⅡ
//			2-5-2. 橋軸直角方向 교축직각방향
//				1) タイプⅠ
//				2) タイプⅡ
	std::map<std::tuple<int, int, int, int>, std::vector<T_SISR_K>> mExistSisr;
	if (!IsExistData(mExistSisr)) return;

	CString RptBlock;
	int rowCnt = 0;
	int stRow = 0;

	CString strSheetName;
	AddSheet(this->m_subType, strSheetName);
	*this->m_pParagraph1 = *this->m_pParagraph1 + 1;

	BOOL bWrited = FALSE;
	int iParagraph3 = 0;
	int iParagraph4 = 0;
	int iParagraph5 = 0;

	int nEarthquakeType = -1;
	int nSeisDir = T_SIPA_D::kUnKnown; // 1: 교축방향, 2: 교축직각방향
	int nType = T_SIPA_D::kUnKnown; // 1: 타입Ⅰ, 2: 타입Ⅱ
	int nMember = D_MBTP_ETC; // 1: 梁部, 2: 柱部
	for (const auto& pair : mExistSisr)
	{
		const auto& key = pair.first;
		const auto& vec = pair.second;
		if (vec.size() == 0) continue;

		const int iEathquake = std::get<0>(key);
		const int iSeisDir = std::get<1>(key);
		const int iType = std::get<2>(key);
		const int iMember = std::get<3>(key);
		if (nEarthquakeType != iEathquake)
		{
			WirteTheory(this->m_subType, nEarthquakeType, *this->m_pParagraph1, *this->m_pParagraph2, stRow);
			nEarthquakeType = iEathquake;
		}

		if (nSeisDir != iSeisDir)
		{
			RptBlock = _T("_02.Shear_Suc_Dir");
			WriteSeisDir(RptBlock, iSeisDir, iParagraph3, stRow);
			nSeisDir = iSeisDir;
			nType = T_SIPA_D::kUnKnown; // 초기화
		}

		if (nType != iType)
		{
			RptBlock = _T("_02.Shear_Suc_Type");
			WriteSeisType(RptBlock, iType, iParagraph4, stRow);
			nType = iType;
			nMember = D_MBTP_ETC; // 초기화
		}

		for (int mm = 0; mm < 2; ++mm) // max/min
		{
			BOOL bMax = mm == 0 ? TRUE : FALSE;
			int nDirRow = stRow;
			RptBlock = _T("_02.Shear_Suc_Head1");
			WriteSeisPos(RptBlock, iMember, iParagraph5, stRow);
			m_pXL->SetValue(nDirRow, 0, bMax ? _LS(IDS_BRDG_SEIS_EVAL_REPORT_JP_DIRECTION_PLUS) : _LS(IDS_BRDG_SEIS_EVAL_REPORT_JP_DIRECTION_MINUS));// ◎方向：(+)方向
			nMember = iMember;

			WriteCommon(vec, bMax, iMember, stRow, bWrited);
			stRow++;
		}

	}

	if (bWrited == FALSE && m_pXL->IsExistSheetName(strSheetName))
		m_pXL->DeleteSheet(strSheetName);
}

void CSeisEvalReportShearSuc::WriteAxisOne(const BOOL& bMax, IN std::tuple<T_SIPA_K, T_SIGR_K, T_ELEM_K>& tParam,
	OUT int& stRow, OUT BOOL& bWrited)
{
	auto l_WriteData = [this](int nRow, int nCol, bool bAuto, const T_SISR_EVAL_ROAD3_CAPA_COMP& resD)
	{
		if (bAuto)
		{
			WriteNanHyphenValue(nRow, 4, resD.dB * m_dLenToMM);
			WriteNanHyphenValue(nRow, 5, resD.dD * m_dLenToMM);
			WriteNanHyphenValue(nRow, 6, resD.dTaumax * m_dStrToMPa);
			WriteNanHyphenValue(nRow, 7, resD.dSucw);
			WriteNanHyphenValue(nRow, 8, resD.dSp);
		}
		WriteNanHyphenValue(nRow, 9, resD.dSuc);
	};

	int rowCnt = 0;
	CString RptBlock;
	for (int iExamShearDir = 0; iExamShearDir < 2; iExamShearDir++) {
		const int sDir = iExamShearDir == 0 ? seis_jp::kY_Dir : seis_jp::kZ_Dir;
		
		T_SISR_EVAL_ROAD3 eval;
		eval.kSipa = std::get<0>(tParam);
		eval.kSigr = std::get<1>(tParam);
		eval.kElem = std::get<2>(tParam);
		eval.nExamShearDir = sDir;
		if (!m_pResult->GetResultSisrRoad3(eval)) continue;

		RptBlock = _T("_02.Shear_Suc_Body");
		rowCnt = CopyRange(m_pXL, RptBlock, stRow);

		m_pXL->SetValue(stRow, 1, eval.kElem);
		//CString strCheckDir = sDir == seis_jp::kZ_Dir ? _T("Fz") : _T("Fy");
		CString strCheckDir;
		WriteReportDir(m_nSeisDir, iExamShearDir + 1, eType::kShear, strCheckDir);

		m_pXL->SetValue(stRow, 3, strCheckDir);
		m_pXL->SetMergeCell(stRow, 3, stRow + 1, 3);

		int nRow = stRow;
		for (int l = 0; l < 2; ++l) // I, J
		{
			const auto& PosD = l == 0 ? eval.PosI : eval.PosJ;

			m_pXL->SetValue(nRow, 2, PosD.NodeK);

			if (bMax == TRUE)
			{
				// 最大
				l_WriteData(nRow, 4, PosD.RsltMax.bAutoCapa, PosD.RsltMax.CapaComp);
			}
			else
			{
				// 最小
				l_WriteData(nRow, 4, PosD.RsltMin.bAutoCapa, PosD.RsltMin.CapaComp);
			}
			nRow += 1;
			bWrited = TRUE;
			if (m_pProgress->NextStep() == FALSE) throw FALSE;
		}
		stRow += rowCnt;
	}
}

void CSeisEvalReportShearSuc::WriteAxisTwo(const BOOL& bMax, IN std::tuple<T_SIPA_K, T_SIGR_K, T_ELEM_K>& tParam,
	OUT int& stRow, OUT BOOL& bWrited)
{
	auto L_GetEvalData = [&](seis_jp::eDirShear sDir, IN std::tuple<T_SIPA_K, T_SIGR_K, T_ELEM_K>& tParam, OUT T_SISR_EVAL_ROAD3& rEval) {
		rEval.kSipa = std::get<0>(tParam);
		rEval.kSigr = std::get<1>(tParam);
		rEval.kElem = std::get<2>(tParam);
		rEval.nExamShearDir = sDir;
		return m_pResult->GetResultSisrRoad3(rEval);
	};

	auto L_WriteData = [&](const int& nExamShearDir, IN T_SISR_EVAL_ROAD3_POS& PosD, int nRow) {
		m_pXL->SetValue(nRow, 2, PosD.NodeK);

		const BOOL& bAutoCapa = bMax ? PosD.RsltMax.bAutoCapa : PosD.RsltMin.bAutoCapa;
		const T_SISR_EVAL_ROAD3_CAPA_COMP& resD = bMax ? PosD.RsltMax.CapaComp : PosD.RsltMin.CapaComp;

		if (nExamShearDir == seis_jp::kY_Dir)
			nRow += 1;

		if (bAutoCapa)
		{
			WriteNanHyphenValue(nRow, 4, resD.dB * m_dLenToMM);
			WriteNanHyphenValue(nRow, 5, resD.dD * m_dLenToMM);
			WriteNanHyphenValue(nRow, 6, resD.dTaumax * m_dStrToMPa);
			WriteNanHyphenValue(nRow, 7, resD.dSucw);
			WriteNanHyphenValue(nRow, 8, resD.dSp);
		}
		WriteNanHyphenValue(nRow, 9, resD.dSuc);
	};

	T_ELEM_K kElem = std::get<2>(tParam);
	T_SISR_EVAL_ROAD3 yEval, zEval;
	BOOL bResY = L_GetEvalData(seis_jp::kY_Dir, tParam, yEval);
	BOOL bResZ = L_GetEvalData(seis_jp::kZ_Dir, tParam, zEval);
	if (bResY == FALSE && bResZ == FALSE) return;

	CString RptBlock = _T("_02.Shear_Suc_Body1");
	int rowCnt = CopyRange(m_pXL, RptBlock, stRow);

	m_pXL->SetValue(stRow, 1, kElem);

	if (bResZ == TRUE) {
		L_WriteData(zEval.nExamShearDir, zEval.PosI, stRow);
		L_WriteData(zEval.nExamShearDir, zEval.PosJ, stRow + 2);
	}
	if (bResY == TRUE) {
		L_WriteData(yEval.nExamShearDir, yEval.PosI, stRow);
		L_WriteData(yEval.nExamShearDir, yEval.PosJ, stRow + 2);
	}

	bWrited = TRUE;
	stRow += rowCnt;
	if (m_pProgress->NextStep() == FALSE) throw FALSE;
}

void CSeisEvalReportShearSuc::AddSheet(const T_SIGR_D::eSubType& nGroupType, OUT CString& rSheetName)
{
	m_strBaseSheet = _LSX(02.Shear_Suc);
	rSheetName = _LS(IDS_BRDG_SEIS_EVAL_REPORT_JP_SHEAR_Suc) + _T("_") + GetTextGroup(nGroupType);// _T("せん断(斜引張破壊)");
	if (m_pXL->IsExistSheetName(rSheetName))
	{
		ASSERT(0);
		m_pXL->SetActiveSheet(rSheetName);
	}
	else
	{
		m_pXL->AddSheet2(rSheetName);
		m_pXL->SetActiveSheet(rSheetName);
	}

	m_pXL->SetStandardFont(_T("Meiryo UI"));
	m_pXL->SetMargin(13, 12, 20, 17);
	m_pXL->SetColumnWidth(0, 0, 0, 2, 7.0 * _ToPoint); // 7.0 * 0.833333333
	m_pXL->SetColumnWidth(0, 3, 0, 60, 8.08 * _ToPoint); // 8.08 * 0.833333333
	m_pXL->SetRowHeight(16.5);
	m_pXL->SetZoom(70);
}

void CSeisEvalReportShearSuc::WirteTheory(const T_SIGR_D::eSubType& nGroupType, const int& nEarthquakeType, const int iParagraph1, int& iParagraph2, OUT int& stRow)
{
	CString RptBlock = _T("_02.Shear_Suc_Title");
	int rowCnt = CopyRange(m_pXL, RptBlock, stRow);
	m_pXL->SetValue(stRow, 0, GetText(_T("%d. %s"), iParagraph1, GetTextTitle(nGroupType)));
	stRow += rowCnt;

	RptBlock = _T("_02.Shear_Suc_Theory1");
	rowCnt = CopyRange(m_pXL, RptBlock, stRow);
	m_pXL->SetValue(stRow, 0, GetText(_T("%d-%d. %s Suc"), iParagraph1, ++iParagraph2, _LS(IDS_BRDG_SEIS_EVAL_REPORT_JP_SHEAR_STR)));// せん断耐力
	stRow += rowCnt;
	stRow++;
}
#pragma endregion

#pragma region // CSeisEvalReportShearSucResult
CSeisEvalReportShearSucResult::CSeisEvalReportShearSucResult(IExcel* pXL, CDgnProgressDlg* pProgress, const CString& strBasePath, const CString& strBaseBook, const SDgnExcelPrintOpt* pPrintOpt)
	: CSeisEvalReportShearBase(pXL, pProgress, strBasePath, strBaseBook, pPrintOpt)
{
}

CSeisEvalReportShearSucResult::~CSeisEvalReportShearSucResult()
{
}

void CSeisEvalReportShearSucResult::AddStep()
{
	CArray<T_SISR_K, T_SISR_K> aSisrK;
	m_pDoc->m_pAttrCtrl2->GetQSisr()->GetKeyList(aSisrK);
	const int nSisr = aSisrK.GetSize();
	if (nSisr == 0) return;

	std::vector<T_SISR_K> vExistSisr[2][2];
	for (int i = 0; i < nSisr; ++i)
	{
		const T_SISR_K SisrK = aSisrK[i];
		T_SISR_D SisrD;
		if (!m_pDoc->m_pAttrCtrl2->GetQSisr()->Get(SisrK, SisrD))		continue;
		T_SIGR_D SigrD;
		if (!m_pDoc->m_pAttrCtrl2->GetQSigr()->Get(SisrD.kSigr, SigrD)) continue;
		if (SigrD.nSubType != this->m_subType)							continue;
		T_SIPA_D SipaD;
		if (!m_pDoc->m_pAttrCtrl2->GetQSipa()->Get(SisrD.kSipa, SipaD)) continue;
		if (SipaD.nDesignCode != m_nDesignCode)							continue;

		const auto iIdx1 = (int)SipaD.nSeismicForceDirection - 1;
		const auto iIdx2 = (int)SipaD.nSeismicForceType - 1;
		vExistSisr[iIdx1][iIdx2].push_back(SisrK);
	}

	std::vector<BOOL>bCheck = { FALSE, FALSE };
	for (int i = 0; i < 2; i++)
	{
		if (vExistSisr[i][0].size() == 0 && vExistSisr[i][1].size() == 0)
			bCheck[i] = TRUE;
	}
	if (bCheck[0] && bCheck[1]) return;

	// せん断の照査
	for (int iSeisDir = 0; iSeisDir < 2; ++iSeisDir) // 橋軸方向, 橋軸直角方向
	{
		if (vExistSisr[iSeisDir][0].size() == 0 && vExistSisr[iSeisDir][1].size() == 0) continue;

		for (int iType = 0; iType < 2; ++iType) // タイプ type
		{
			if (vExistSisr[iSeisDir][iType].size() == 0) continue;

			for (const auto& SisrK : vExistSisr[iSeisDir][iType])
			{
				T_SISR_D SisrD;
				if (!m_pDoc->m_pAttrCtrl2->GetQSisr()->Get(SisrK, SisrD)) continue;

				T_SIGR_D SigrD;
				if (!m_pDoc->m_pAttrCtrl2->GetQSigr()->Get(SisrD.kSigr, SigrD)) continue;

				T_ELEM_K_LIST aElemKeys; std::map <T_ELEM_K, std::tuple<CString, int>> mElemInfo;
				GetTargetElements(SisrD.kSigr, this->m_subType, aElemKeys, mElemInfo);
				if (aElemKeys.GetCount() < 1) continue;

				const int nMemb = aElemKeys.GetSize();
				for (int k = 0; k < nMemb; ++k)
				{
					const auto ElemK = aElemKeys[k];
					if(mElemInfo.find(ElemK) == mElemInfo.end() ) continue;

					//const CString& sGroup = std::get<0>(mElemInfo[ElemK]);
					//const int nShearDir = std::get<1>(mElemInfo[ElemK]);

					for (int d = 0; d < 2; d++)
					{
						int nShearDir = d == 0 ? seis_jp::kY_Dir : seis_jp::kZ_Dir;
						T_SISR_EVAL_ROAD3 eval;
						eval.kSipa = SisrD.kSipa;
						eval.kSigr = SisrD.kSigr;
						eval.kElem = ElemK;
						eval.nExamShearDir = nShearDir;
						if (!m_pResult->GetResultSisrRoad3(eval)) continue;
						m_pProgress->m_nTotalSteps++;
					}
				}
			}
		}
	}
}

void CSeisEvalReportShearSucResult::Write()
{
//	2. 上部構造の照査 상부구조조사
//		2-5. せん断の照査
//			2-5-1. 橋軸方向 교축방향
//				1) タイプⅠ
//				2) タイプⅡ
//			2-5-2. 橋軸直角方向 교축직각방향
//				1) タイプⅠ
//				2) タイプⅡ
	std::map<std::tuple<int, int, int>, std::vector<T_SISR_K>> mExistSisr;
	if (!IsExistData(mExistSisr)) return;

	CString RptBlock;
	int rowCnt = 0;
	int stRow = 0;

	CString strSheetName;
	AddSheet(this->m_subType, strSheetName);
	WirteTheory(this->m_subType, *this->m_pParagraph1, *this->m_pParagraph2, stRow);
	*this->m_pParagraph1 = *this->m_pParagraph1 + 1;

	BOOL bWrited = FALSE;
	int iParagraph3 = 0;
	int iParagraph4 = 0;
	int iParagraph5 = 0;

	//int nEarthquakeType = -1;;
	int nSeisDir = T_SIPA_D::kUnKnown; // 1: 교축방향, 2: 교축직각방향
	int nType = T_SIPA_D::kUnKnown; // 1: 타입Ⅰ, 2: 타입Ⅱ
	int nMember = D_MBTP_ETC; // 1: 梁部, 2: 柱部
	for (const auto& pair : mExistSisr)
	{
		const auto& key = pair.first;
		const auto& vec = pair.second;
		if (vec.size() == 0) continue;

		//const int iEathquake = std::get<0>(key);
		const int iSeisDir = std::get<0>(key);
		const int iType = std::get<1>(key);
		const int iMember = std::get<2>(key);

		if (nSeisDir != iSeisDir)
		{
			RptBlock = _T("_02.Shear_Suc_Dir");
			WriteSeisDir(RptBlock, iSeisDir, iParagraph3, stRow);
			nSeisDir = iSeisDir;
			nType = T_SIPA_D::kUnKnown; // 초기화
		}

		if (nType != iType)
		{
			RptBlock = _T("_02.Shear_Suc_Type");
			WriteSeisType(RptBlock, iType, iParagraph4, stRow);
			nType = iType;
			nMember = D_MBTP_ETC; // 초기화
		}

		if (nMember != iMember)
		{
			RptBlock = _T("_02.Shear_Suc_Head2");
			WriteSeisPos(RptBlock, iMember, iParagraph5, stRow);
			nMember = iMember;
		}

		WriteCommon(vec, iMember, stRow, bWrited);
		stRow++;
	}

	if (bWrited == FALSE && m_pXL->IsExistSheetName(strSheetName))
		m_pXL->DeleteSheet(strSheetName);
}

void CSeisEvalReportShearSucResult::WriteData(const T_SISR_EVAL_ROAD3_POS* pPosD, const int& nRow, OUT BOOL& bWrited)
{
	m_pXL->SetValue(nRow, 2, pPosD->NodeK);

	const int& nThis = min(pPosD->RsltMin.Forc.adShear.GetSize(), 3);

	const auto& MinForc = pPosD->RsltMin.Forc;
	for (int m = 0; m < nThis; ++m)
	{
		WriteNanHyphenValue(nRow + m, 4, MinForc.adShear[m]);
		WriteNanHyphenValue(nRow + m, 5, MinForc.adShearh[m]);
	}
	WriteNanHyphenValue(nRow, 6, MinForc.dShear);
	WriteNanHyphenValue(nRow, 7, pPosD->RsltMin.CapaComp.dSuc);
	if (pPosD->RsltMin.CapaComp.dRatio > 1.0)
	{
		m_pXL->SetValue(nRow, 8, _T("NG"));
		SetNG_Red(m_pXL, nRow, 8, nRow, 7);
	}
	else
	{
		m_pXL->SetValue(nRow, 8, _T("OK"));
	}

	const auto& MaxForc = pPosD->RsltMax.Forc;
	for (int m = 0; m < nThis; ++m)
	{
		WriteNanHyphenValue(nRow + m, 9, MaxForc.adShear[m]);
		WriteNanHyphenValue(nRow + m, 10, MaxForc.adShearh[m]);
	}
	WriteNanHyphenValue(nRow, 11, MaxForc.dShear);
	WriteNanHyphenValue(nRow, 12, pPosD->RsltMax.CapaComp.dSuc);
	if (pPosD->RsltMax.CapaComp.dRatio > 1.0)
	{
		m_pXL->SetValue(nRow, 13, _T("NG"));
		SetNG_Red(m_pXL, nRow, 13, nRow, 12);
	}
	else
	{
		m_pXL->SetValue(nRow, 13, _T("OK"));
	}

	bWrited = TRUE;
}

void CSeisEvalReportShearSucResult::WriteAxisOne(IN std::tuple<T_SIPA_K, T_SIGR_K, T_ELEM_K, CString>& tParam,
	OUT int& stRow, OUT BOOL& bWrited)
{
	int rowCnt = 0;
	CString RptBlock;
	for (int iExamShearDir = 0; iExamShearDir < 2; iExamShearDir++) {
		const int sDir = iExamShearDir == 0 ? seis_jp::kY_Dir : seis_jp::kZ_Dir;
		
		T_SISR_EVAL_ROAD3 eval;
		eval.kSipa = std::get<0>(tParam);
		eval.kSigr = std::get<1>(tParam);
		eval.kElem = std::get<2>(tParam);
		eval.nExamShearDir = sDir;
		if (!m_pResult->GetResultSisrRoad3(eval)) continue;

		RptBlock = _T("_02.Shear_Suc_Body2");
		rowCnt = CopyRange(m_pXL, RptBlock, stRow);

		m_pXL->SetValue(stRow, 0, std::get<3>(tParam));
		m_pXL->SetValue(stRow, 1, eval.kElem);

		//CString strCheckDir = sDir == seis_jp::kZ_Dir ? _T("Fz") : _T("Fy");
		CString strCheckDir;
		WriteReportDir(m_nSeisDir, iExamShearDir + 1, eType::kShear, strCheckDir);
		m_pXL->SetValue(stRow, 3, strCheckDir);

		WriteData(&eval.PosI, stRow, bWrited);
		WriteData(&eval.PosJ, stRow + 3, bWrited);

		stRow += rowCnt;
		if (m_pProgress->NextStep() == FALSE) throw FALSE;
	}
}

void CSeisEvalReportShearSucResult::WriteAxisTwo(IN std::tuple<T_SIPA_K, T_SIGR_K, T_ELEM_K, CString>& tParam,
	OUT int& stRow, OUT BOOL& bWrited)
{
	int rowCnt = 0;
	CString RptBlock;
	BOOL bWriteTable = FALSE;
	for (int iExamShearDir = 0; iExamShearDir < 2; iExamShearDir++) {
		const int sDir = iExamShearDir == 0 ? seis_jp::kY_Dir : seis_jp::kZ_Dir;

		T_SISR_EVAL_ROAD3 eval;
		eval.kSipa = std::get<0>(tParam);
		eval.kSigr = std::get<1>(tParam);
		eval.kElem = std::get<2>(tParam);
		eval.nExamShearDir = sDir;
		if (!m_pResult->GetResultSisrRoad3(eval)) continue;

		if (bWriteTable == FALSE) {
			bWriteTable = TRUE;
			RptBlock = _T("_02.Shear_Suc_Body3");
			rowCnt = CopyRange(m_pXL, RptBlock, stRow);

			m_pXL->SetValue(stRow, 0, std::get<3>(tParam));
			m_pXL->SetValue(stRow, 1, eval.kElem);
		}

		int nRow = stRow;
		// I단
		if (eval.nExamShearDir == seis_jp::kY_Dir)
			nRow += 3;

		WriteData(&eval.PosI, nRow, bWrited);

		// J단
		nRow = stRow + 6;
		if (eval.nExamShearDir == seis_jp::kY_Dir)
			nRow += 3;

		WriteData(&eval.PosJ, nRow, bWrited);
		if (m_pProgress->NextStep() == FALSE) throw FALSE;
	}

	if (bWrited)
		stRow += rowCnt;
}

void CSeisEvalReportShearSucResult::AddSheet(const T_SIGR_D::eSubType& nGroupType, OUT CString& rSheetName)
{
	m_strBaseSheet = _LSX(02.Shear_Suc);
	rSheetName = _LS(IDS_BRDG_SEIS_EVAL_REPORT_JP_SHEAR_Suc_Result) + _T("_") + GetTextGroup(nGroupType);// _T("せん断結果(斜引張破壊)");
	if (m_pXL->IsExistSheetName(rSheetName))
	{
		ASSERT(0);
		m_pXL->SetActiveSheet(rSheetName);
	}
	else
	{
		m_pXL->AddSheet2(rSheetName);
		m_pXL->SetActiveSheet(rSheetName);
	}

	m_pXL->SetStandardFont(_T("Meiryo UI"));
	m_pXL->SetMargin(13, 12, 20, 17);
	m_pXL->SetColumnWidth(0, 0, 0, 2, 7.0 * _ToPoint); // 7.0 * 0.833333333
	m_pXL->SetColumnWidth(0, 3, 0, 60, 8.08 * _ToPoint); // 8.08 * 0.833333333
	m_pXL->SetRowHeight(16.5);
	m_pXL->SetZoom(70);
}

void CSeisEvalReportShearSucResult::WirteTheory(const T_SIGR_D::eSubType& nGroupType, const int iParagraph1, int& iParagraph2, OUT int& stRow)
{
	CString RptBlock = _T("_02.Shear_Suc_Title");
	int rowCnt = CopyRange(m_pXL, RptBlock, stRow);
	m_pXL->SetValue(stRow, 0, GetText(_T("%d. %s"), iParagraph1, GetTextTitle(nGroupType)));
	stRow += rowCnt;

	RptBlock = _T("_02.Shear_Suc_Theory2");
	rowCnt = CopyRange(m_pXL, RptBlock, stRow);
	m_pXL->SetValue(stRow, 0, GetText(_T("%d-%d. %s"), iParagraph1, ++iParagraph2, _LS(IDS_BRDG_SEIS_EVAL_REPORT_JP_CHK_SHEAR)));// せん断耐力
	stRow += rowCnt;
	stRow++;
}
#pragma endregion

#pragma region // CSeisEvalReportShearPs
CSeisEvalReportShearPs::CSeisEvalReportShearPs(IExcel* pXL, CDgnProgressDlg* pProgress, const CString& strBasePath, const CString& strBaseBook, const SDgnExcelPrintOpt* pPrintOpt)
	: CSeisEvalReportShearBase(pXL, pProgress, strBasePath, strBaseBook, pPrintOpt)
{
}

CSeisEvalReportShearPs::~CSeisEvalReportShearPs()
{
}

void CSeisEvalReportShearPs::AddStep()
{
	CArray<T_SISR_K, T_SISR_K> aSisrK;
	m_pDoc->m_pAttrCtrl2->GetQSisr()->GetKeyList(aSisrK);
	const int nSisr = aSisrK.GetSize();
	if (nSisr == 0) return;

	std::vector<T_SISR_K> vExistSisr[2][2];
	for (int i = 0; i < nSisr; ++i)
	{
		const T_SISR_K SisrK = aSisrK[i];
		T_SISR_D SisrD;
		if (!m_pDoc->m_pAttrCtrl2->GetQSisr()->Get(SisrK, SisrD))		continue;
		T_SIGR_D SigrD;
		if (!m_pDoc->m_pAttrCtrl2->GetQSigr()->Get(SisrD.kSigr, SigrD)) continue;
		if (SigrD.nSubType != this->m_subType)								continue;
		T_SIPA_D SipaD;
		if (!m_pDoc->m_pAttrCtrl2->GetQSipa()->Get(SisrD.kSipa, SipaD)) continue;
		if (SipaD.nDesignCode != m_nDesignCode)							continue;

		const auto iIdx1 = (int)SipaD.nSeismicForceDirection - 1;
		const auto iIdx2 = (int)SipaD.nSeismicForceType - 1;
		vExistSisr[iIdx1][iIdx2].push_back(SisrK);
	}

	std::vector<BOOL>bCheck = { FALSE, FALSE };
	for (int i = 0; i < 2; i++)
	{
		if (vExistSisr[i][0].size() == 0 && vExistSisr[i][1].size() == 0)
			bCheck[i] = TRUE;
	}
	if (bCheck[0] && bCheck[1]) return;

	for (int iSeisDir = 0; iSeisDir < 2; ++iSeisDir) // 橋軸方向, 橋軸直角方向
	{
		if (vExistSisr[iSeisDir][0].size() == 0 && vExistSisr[iSeisDir][1].size() == 0) continue;

		for (int iType = 0; iType < 2; ++iType) // タイプ
		{
			if (vExistSisr[iSeisDir][iType].size() == 0) continue;

			for (int iMembType = 1; iMembType < 3; ++iMembType) // 柱(D_MBTP_COLUMN)部の結果, 梁(D_MBTP_BEAM)部の結果
			{
				for (const auto& SisrK : vExistSisr[iSeisDir][iType])
				{
					T_SISR_D SisrD;
					if (!m_pDoc->m_pAttrCtrl2->GetQSisr()->Get(SisrK, SisrD)) continue;

					T_SIGR_D SigrD;
					if (!m_pDoc->m_pAttrCtrl2->GetQSigr()->Get(SisrD.kSigr, SigrD)) continue;
					if (SigrD.nSubType != this->m_subType) continue;

					T_ELEM_K_LIST aElemKeys;
					std::map <T_ELEM_K, std::tuple<CString, int>> mElemInfo;
					GetTargetElements(SisrD.kSigr, this->m_subType, T_SIGR_LOWER_D::kRC, iMembType, aElemKeys, mElemInfo);

					const auto nMemb = aElemKeys.GetSize();
					for (int n = 0; n < nMemb; ++n)
					{
						const auto MembK = aElemKeys[n];

						for (int iExamShearDir = 0; iExamShearDir < 2; iExamShearDir++)
						{
							const int sDir = iExamShearDir == 0 ? seis_jp::kY_Dir : seis_jp::kZ_Dir;

							T_SISR_EVAL_LOWER EvalD;
							EvalD.kSipa = SisrD.kSipa;
							EvalD.kSigr = SisrD.kSigr;
							EvalD.kElem = MembK;
							EvalD.nExamShearDir = sDir;
							if (!m_pResult->GetResultSisrLower(EvalD)) continue;

							m_pProgress->m_nTotalSteps++;
						}
					}
				}
			}
		}
	}
}

void CSeisEvalReportShearPs::Write()
{
	//	3. 下部構造の3降伏照査 하부구조항복조사
	//		3-3. せん断耐力 Ps
	//			3-3-1. 橋軸方向
	//				1）タイプI
	//					(1) 柱部の結果
	//					(2) 梁部の結果
	//				2）タイプⅡ
	//					(1) 柱部の結果
	//					(2) 梁部の結果
	//			3-3-2. 橋軸直角方向
	//				1）タイプI
	//					(1) 柱部の結果
	//					(2) 梁部の結果
	//				2）タイプⅡ
	//					(1) 柱部の結果
	//					(2) 梁部の結果

	std::map<std::tuple<int, int, int, int>, std::vector<T_SISR_K>> mExistSisr;
	if (!IsExistData(mExistSisr)) return;

	CString RptBlock;
	int rowCnt = 0;
	int stRow = 0;

	CString strSheetName;
	AddSheet(this->m_subType, strSheetName);
	*this->m_pParagraph1 = *this->m_pParagraph1 + 1;

	BOOL bWrited = FALSE;
	int iParagraph3 = 0;
	int iParagraph4 = 0;
	int iParagraph5 = 0;

	int nEarthquakeType = -1;; // 1: 항복조사, 2: 한계상태조사
	int nSeisDir = T_SIPA_D::kUnKnown; // 1: 교축방향, 2: 교축직각방향
	int nType = T_SIPA_D::kUnKnown; // 1: 타입Ⅰ, 2: 타입Ⅱ
	int nMember = D_MBTP_ETC; // 1: 梁部, 2: 柱部
	for (const auto& pair : mExistSisr)
	{
		const auto& key = pair.first;
		const auto& vec = pair.second;
		if (vec.size() == 0) continue;

		const int iEathquake = std::get<0>(key);
		const int iSeisDir = std::get<1>(key);
		const int iType = std::get<2>(key);
		const int iMember = std::get<3>(key);
		if (nEarthquakeType != iEathquake)
		{
			WirteTheory(this->m_subType, nEarthquakeType, *this->m_pParagraph1, *this->m_pParagraph2, stRow);
			nEarthquakeType = iEathquake;
		}

		if (nSeisDir != iSeisDir)
		{
			RptBlock = _T("_03.Shear_Ps_Dir");
			WriteSeisDir(RptBlock, iSeisDir, iParagraph3, stRow);
			nSeisDir = iSeisDir;
			nType = T_SIPA_D::kUnKnown; // 초기화
		}

		if (nType != iType)
		{
			RptBlock = _T("_03.Shear_Ps_Type");
			WriteSeisType(RptBlock, iType, iParagraph4, stRow);
			nType = iType;
			nMember = D_MBTP_ETC; // 초기화
		}

		for (int mm = 0; mm < 2; ++mm) // max/min
		{
			BOOL bMax = mm == 0 ? TRUE : FALSE;
			int nDirRow = stRow + 1;
			RptBlock = _T("_03.Shear_Ps_Head1");
			WriteSeisPos(RptBlock, iMember, iParagraph5, stRow);
			m_pXL->SetValue(nDirRow, 0, bMax ? _LS(IDS_BRDG_SEIS_EVAL_REPORT_JP_DIRECTION_PLUS) : _LS(IDS_BRDG_SEIS_EVAL_REPORT_JP_DIRECTION_MINUS));// ◎方向：(+)方向
			nMember = iMember;

			WriteCommon(vec, bMax, iMember, stRow, bWrited);
			stRow++;
		}
	}

	if (bWrited == FALSE && m_pXL->IsExistSheetName(strSheetName))
		m_pXL->DeleteSheet(strSheetName);
}

void CSeisEvalReportShearPs::WriteCommon(const std::vector<T_SISR_K>& vec, const BOOL& bMax, const int& iMember, int& stRow, BOOL& bWrited)
{
	for (const auto& SisrK : vec)
	{
		T_SISR_D SisrD;
		if (!m_pDoc->m_pAttrCtrl2->GetQSisr()->Get(SisrK, SisrD)) continue;

		T_SIGR_D SigrD;
		if (!m_pDoc->m_pAttrCtrl2->GetQSigr()->Get(SisrD.kSigr, SigrD)) continue;

		T_ELEM_K_LIST aElemKeys; std::map <T_ELEM_K, std::tuple<CString, int>> mElemInfo;
		GetTargetElements(SisrD.kSigr, this->m_subType, T_SIGR_LOWER_D::kRC, iMember, aElemKeys, mElemInfo);
		if (aElemKeys.GetCount() < 1) continue;

		const int stRowGrp = stRow;
		const int nMemb = aElemKeys.GetSize();
		for (int k = 0; k < nMemb; ++k)
		{
			const auto ElemK = aElemKeys[k];
			if (mElemInfo.find(ElemK) == mElemInfo.end()) continue;
			const CString& sGroup = std::get<0>(mElemInfo[ElemK]);
			const int nShearDir = std::get<1>(mElemInfo[ElemK]);

			std::tuple<T_SIPA_K, T_SIGR_K, T_ELEM_K, CString> tParam;
			tParam = std::make_tuple(SisrD.kSipa, SisrD.kSigr, ElemK, sGroup);

			if (nShearDir == seis_jp::kYZ_Dir) {
				WriteAxisTwo(bMax, tParam, stRow, bWrited);
			}
			else {
				WriteAxisOne(bMax, tParam, stRow, bWrited);
			}
		}
		auto* pFormat = m_pXL->GetSheet()->cellFormat(stRow - 1, 0);
		m_pXL->GetSheet()->setCellFormat(stRowGrp, 0, pFormat);
		m_pXL->SetMergeCell(stRowGrp, 0, stRow + -1, 0);
		m_pXL->SetValue(stRowGrp, 0, SigrD.strName);
	}
}

void CSeisEvalReportShearPs::WriteAxisOne(const BOOL& bMax, IN std::tuple<T_SIPA_K, T_SIGR_K, T_ELEM_K, CString>& tParam, OUT int& stRow, OUT BOOL& bWrited)
{
	auto l_WriteData = [this](int nRow, int nCol, const T_SISR_EVAL_LOWER_BASE& SisrBase)
		{
			if (SisrBase.bAutoCapa)
			{
				WriteNanHyphenValue(nRow, nCol++, SisrBase.dB * m_dLenToMM);
				WriteNanHyphenValue(nRow, nCol++, SisrBase.dD * m_dLenToMM);
				WriteNanHyphenValue(nRow, nCol++, SisrBase.dPt);
				WriteNanHyphenValue(nRow, nCol++, SisrBase.dCe);
				WriteNanHyphenValue(nRow, nCol++, SisrBase.dCpt);
				WriteNanHyphenValue(nRow, nCol++, SisrBase.dCdc);
				WriteNanHyphenValue(nRow, nCol++, SisrBase.dCds);
				WriteNanHyphenValue(nRow, nCol++, SisrBase.dSc);
				WriteNanHyphenValue(nRow, nCol++, SisrBase.dSs);
				WriteNanHyphenValue(nRow, nCol++, SisrBase.dPs);
			}
			else
			{
				WriteNanHyphenValue(nRow, nCol + 9, SisrBase.dPs);
			}
		};

	int rowCnt = 0;
	CString RptBlock;
	for (int iExamShearDir = 0; iExamShearDir < 2; iExamShearDir++)
	{
		const int sDir = iExamShearDir == 0 ? seis_jp::kY_Dir : seis_jp::kZ_Dir;

		T_SISR_EVAL_LOWER EvalD;
		EvalD.kSipa = std::get<0>(tParam);
		EvalD.kSigr = std::get<1>(tParam);
		EvalD.kElem = std::get<2>(tParam);
		EvalD.nExamShearDir = sDir;
		if (!m_pResult->GetResultSisrLower(EvalD)) continue;

		RptBlock = _T("_03.Shear_Ps_Body1");
		rowCnt = CopyRange(m_pXL, RptBlock, stRow);

		(std::get<3>(tParam) == _T("--")) ? SetValue_Hyphen(stRow, 1) : m_pXL->SetValue(stRow, 1, std::get<3>(tParam));

		m_pXL->SetValue(stRow, 2, EvalD.kElem); // Element
		m_pXL->SetValue(stRow, 3, EvalD.PosI.kNode); // node
		m_pXL->SetValue(stRow + 1, 3, EvalD.PosJ.kNode);
		CString strCheckDir;
		WriteReportDir(m_nSeisDir, iExamShearDir + 1, eType::kShear, strCheckDir);

		m_pXL->SetValue(stRow, 4, strCheckDir);
		m_pXL->SetMergeCell(stRow, 4, stRow + 1, 4);

		T_SISR_EVAL_LOWER_BASE Rslt_I, Rslt_J;
		if (bMax) {
			Rslt_I = EvalD.PosI.RsltMax;
			Rslt_J = EvalD.PosJ.RsltMax;
		}
		else {
			Rslt_I = EvalD.PosI.RsltMin;
			Rslt_J = EvalD.PosJ.RsltMin;
		}

		l_WriteData(stRow, 5, Rslt_I);
		l_WriteData(stRow + 1, 5, Rslt_J);

		stRow += rowCnt;
		bWrited = TRUE;
		if (m_pProgress->NextStep() == FALSE) throw FALSE;
	}
}

void CSeisEvalReportShearPs::WriteAxisTwo(const BOOL& bMax, IN std::tuple<T_SIPA_K, T_SIGR_K, T_ELEM_K, CString>& tParam, OUT int& stRow, OUT BOOL& bWrited)
{
	auto l_WriteData = [this](int nRow, int nCol, const T_SISR_EVAL_LOWER_BASE& SisrBase)
		{
			if (SisrBase.bAutoCapa)
			{
				WriteNanHyphenValue(nRow, nCol++, SisrBase.dB * m_dLenToMM);
				WriteNanHyphenValue(nRow, nCol++, SisrBase.dD * m_dLenToMM);
				WriteNanHyphenValue(nRow, nCol++, SisrBase.dPt);
				WriteNanHyphenValue(nRow, nCol++, SisrBase.dCe);
				WriteNanHyphenValue(nRow, nCol++, SisrBase.dCpt);
				WriteNanHyphenValue(nRow, nCol++, SisrBase.dCdc);
				WriteNanHyphenValue(nRow, nCol++, SisrBase.dCds);
				WriteNanHyphenValue(nRow, nCol++, SisrBase.dSc);
				WriteNanHyphenValue(nRow, nCol++, SisrBase.dSs);
				WriteNanHyphenValue(nRow, nCol++, SisrBase.dPs);
			}
			else
			{
				WriteNanHyphenValue(nRow, nCol + 9, SisrBase.dPs);
			}
		};

	int rowCnt = 0;
	CString RptBlock;
	BOOL bWriteTable = FALSE;
	for (int iExamShearDir = 0; iExamShearDir < 2; iExamShearDir++) {
		const int sDir = iExamShearDir == 0 ? seis_jp::kY_Dir : seis_jp::kZ_Dir;

		T_SISR_EVAL_LOWER EvalD;
		EvalD.kSipa = std::get<0>(tParam);
		EvalD.kSigr = std::get<1>(tParam);
		EvalD.kElem = std::get<2>(tParam);
		EvalD.nExamShearDir = sDir;
		if (!m_pResult->GetResultSisrLower(EvalD)) continue;

		if (bWriteTable == FALSE) {
			RptBlock = _T("_03.Shear_Ps_Body2");
			rowCnt = CopyRange(m_pXL, RptBlock, stRow);
			bWriteTable = TRUE;
		}

		(std::get<3>(tParam) == _T("--")) ? SetValue_Hyphen(stRow, 1) : m_pXL->SetValue(stRow, 1, std::get<3>(tParam));
		m_pXL->SetValue(stRow, 2, EvalD.kElem); // Element
		m_pXL->SetValue(stRow, 3, EvalD.PosI.kNode); // node
		m_pXL->SetValue(stRow + 2, 3, EvalD.PosJ.kNode);

		T_SISR_EVAL_LOWER_BASE Rslt_I, Rslt_J;
		if (bMax) {
			Rslt_I = EvalD.PosI.RsltMax;
			Rslt_J = EvalD.PosJ.RsltMax;
		}
		else {
			Rslt_I = EvalD.PosI.RsltMin;
			Rslt_J = EvalD.PosJ.RsltMin;
		}

		int nRow = stRow;
		// I단
		if (EvalD.nExamShearDir == seis_jp::kY_Dir)
			nRow += 1;

		l_WriteData(nRow, 5, Rslt_I);

		// J단
		nRow = stRow + 2;
		if (EvalD.nExamShearDir == seis_jp::kY_Dir)
			nRow += 1;

		l_WriteData(nRow, 5, Rslt_J);

		bWrited = TRUE;
		if (m_pProgress->NextStep() == FALSE) throw FALSE;
	}

	if (bWrited) stRow += rowCnt;
}

void CSeisEvalReportShearPs::AddSheet(const T_SIGR_D::eSubType& nGroupType, OUT CString& rSheetName)
{
	m_strBaseSheet = _LSX(03.Shear_Ps);
	rSheetName = _LS(IDS_BRDG_SEIS_EVAL_REPORT_JP_SHEAR_CAPA) + _T("_") + GetTextGroup(nGroupType);// _T("せん断(橋脚)");
	if (m_pXL->IsExistSheetName(rSheetName))
	{
		ASSERT(0);
		m_pXL->SetActiveSheet(rSheetName);
	}
	else
	{
		m_pXL->AddSheet2(rSheetName);
		m_pXL->SetActiveSheet(rSheetName);
	}

	m_pXL->SetStandardFont(_T("Meiryo UI"));
	m_pXL->SetMargin(13, 12, 20, 17);
	m_pXL->SetColumnWidth(0, 0, 0, 3, 6.9 * _ToPoint); //  6.9 * 0.833333333
	m_pXL->SetColumnWidth(0, 4, 0, 60, 7.4 * _ToPoint); //  7.4 * 0.833333333
	m_pXL->SetRowHeight(16.5);
	m_pXL->SetZoom(65);
}

void CSeisEvalReportShearPs::WirteTheory(const T_SIGR_D::eSubType& nGroupType, const int& nEarthquakeType, const int iParagraph1, int& iParagraph2, OUT int& stRow)
{
	CString RptBlock = _T("_03.Shear_Ps_Title");
	int rowCnt = CopyRange(m_pXL, RptBlock, stRow);
	m_pXL->SetValue(stRow, 0, GetText(_T("%d. %s"), iParagraph1, GetTextTitle(nGroupType)));
	stRow += rowCnt;

	RptBlock = _T("_03.Shear_Ps_Theory1");
	rowCnt = CopyRange(m_pXL, RptBlock, stRow);
	m_pXL->SetValue(stRow, 0, GetText(_T("%d-%d. %s Ps"), iParagraph1, ++iParagraph2, _LS(IDS_BRDG_SEIS_EVAL_REPORT_JP_SHEAR_STR))); //せん断耐力
	stRow += rowCnt;
	stRow++;
}
#pragma endregion

#pragma region // CSeisEvalReportShearPsResult
CSeisEvalReportShearPsResult::CSeisEvalReportShearPsResult(IExcel* pXL, CDgnProgressDlg* pProgress, const CString& strBasePath, const CString& strBaseBook, const SDgnExcelPrintOpt* pPrintOpt)
	: CSeisEvalReportShearBase(pXL, pProgress, strBasePath, strBaseBook, pPrintOpt)
{
}

CSeisEvalReportShearPsResult::~CSeisEvalReportShearPsResult()
{
}

void CSeisEvalReportShearPsResult::AddStep()
{
	CArray<T_SISR_K, T_SISR_K> aSisrK;
	m_pDoc->m_pAttrCtrl2->GetQSisr()->GetKeyList(aSisrK);
	const int nSisr = aSisrK.GetSize();
	if (nSisr == 0) return;

	std::vector<T_SISR_K> vExistSisr[2][2];
	for (int i = 0; i < nSisr; ++i)
	{
		const T_SISR_K SisrK = aSisrK[i];
		T_SISR_D SisrD;
		if (!m_pDoc->m_pAttrCtrl2->GetQSisr()->Get(SisrK, SisrD))		continue;
		T_SIGR_D SigrD;
		if (!m_pDoc->m_pAttrCtrl2->GetQSigr()->Get(SisrD.kSigr, SigrD)) continue;
		if (SigrD.nSubType != this->m_subType)								continue;
		T_SIPA_D SipaD;
		if (!m_pDoc->m_pAttrCtrl2->GetQSipa()->Get(SisrD.kSipa, SipaD)) continue;
		if (SipaD.nDesignCode != m_nDesignCode)							continue;

		const auto iIdx1 = (int)SipaD.nSeismicForceDirection - 1;
		const auto iIdx2 = (int)SipaD.nSeismicForceType - 1;
		vExistSisr[iIdx1][iIdx2].push_back(SisrK);
	}

	std::vector<BOOL>bCheck = { FALSE, FALSE };
	for (int i = 0; i < 2; i++)
	{
		if (vExistSisr[i][0].size() == 0 && vExistSisr[i][1].size() == 0)
			bCheck[i] = TRUE;
	}
	if (bCheck[0] && bCheck[1]) return;

	for (int iSeisDir = 0; iSeisDir < 2; ++iSeisDir) // 橋軸方向, 橋軸直角方向
	{
		if (vExistSisr[iSeisDir][0].size() == 0 && vExistSisr[iSeisDir][1].size() == 0) continue;

		for (int iType = 0; iType < 2; ++iType) // タイプ
		{
			if (vExistSisr[iSeisDir][iType].size() == 0) continue;

			for (int iMembType = 1; iMembType < 3; ++iMembType) // 柱(D_MBTP_COLUMN)部の結果, 梁(D_MBTP_BEAM)部の結果
			{
				for (const auto& SisrK : vExistSisr[iSeisDir][iType])
				{
					T_SISR_D SisrD;
					if (!m_pDoc->m_pAttrCtrl2->GetQSisr()->Get(SisrK, SisrD)) continue;

					T_SIGR_D SigrD;
					if (!m_pDoc->m_pAttrCtrl2->GetQSigr()->Get(SisrD.kSigr, SigrD)) continue;
					if (SigrD.nSubType != this->m_subType) continue;

					T_ELEM_K_LIST aElemKeys;
					std::map <T_ELEM_K, std::tuple<CString, int>> mElemInfo;
					GetTargetElements(SisrD.kSigr, this->m_subType, T_SIGR_LOWER_D::kRC, iMembType, aElemKeys, mElemInfo);

					const auto nMemb = aElemKeys.GetSize();
					for (int n = 0; n < nMemb; ++n)
					{
						const auto MembK = aElemKeys[n];

						for (int iExamShearDir = 0; iExamShearDir < 2; iExamShearDir++)
						{
							const int sDir = iExamShearDir == 0 ? seis_jp::kY_Dir : seis_jp::kZ_Dir;

							T_SISR_EVAL_LOWER EvalD;
							EvalD.kSipa = SisrD.kSipa;
							EvalD.kSigr = SisrD.kSigr;
							EvalD.kElem = MembK;
							EvalD.nExamShearDir = sDir;
							if (!m_pResult->GetResultSisrLower(EvalD)) continue;

							m_pProgress->m_nTotalSteps++;
						}
					}
				}
			}
		}
	}
}

void CSeisEvalReportShearPsResult::Write()
{
	//	3. 下部構造の3降伏照査 하부구조항복조사
	//		3-4. せん断の照査 
	//			3-4-1. 橋軸方向
	//				1）タイプI
	//					(1) 柱部の結果
	//					(2) 梁部の結果
	//				2）タイプⅡ
	//					(1) 柱部の結果
	//					(2) 梁部の結果
	//			3-4-2. 橋軸直角方向
	//				1）タイプI
	//					(1) 柱部の結果
	//					(2) 梁部の結果
	//				2）タイプⅡ
	//					(1) 柱部の結果
	//					(2) 梁部の結果

	std::map<std::tuple<int, int, int>, std::vector<T_SISR_K>> mExistSisr;
	if (!IsExistData(mExistSisr)) return;

	CString RptBlock;
	int rowCnt = 0;
	int stRow = 0;

	CString strSheetName;
	AddSheet(this->m_subType, strSheetName);
	WirteTheory(this->m_subType, *this->m_pParagraph1, *this->m_pParagraph2, stRow);
	*this->m_pParagraph1 = *this->m_pParagraph1 + 1;

	BOOL bWrited = FALSE;
	int iParagraph3 = 0;
	int iParagraph4 = 0;
	int iParagraph5 = 0;

	//int nEarthquakeType = -1;;
	int nSeisDir = T_SIPA_D::kUnKnown; // 1: 교축방향, 2: 교축직각방향
	int nType = T_SIPA_D::kUnKnown; // 1: 타입Ⅰ, 2: 타입Ⅱ
	int nMember = D_MBTP_ETC; // 1: 梁部, 2: 柱部
	for (const auto& pair : mExistSisr)
	{
		const auto& key = pair.first;
		const auto& vec = pair.second;
		if (vec.size() == 0) continue;

		//const int iEathquake = std::get<0>(key);
		const int iSeisDir = std::get<0>(key);
		const int iType = std::get<1>(key);
		const int iMember = std::get<2>(key);

		if (nSeisDir != iSeisDir)
		{
			RptBlock = _T("_03.Shear_Ps_Dir");
			WriteSeisDir(RptBlock, iSeisDir, iParagraph3, stRow);
			nSeisDir = iSeisDir;
			nType = T_SIPA_D::kUnKnown; // 초기화
		}

		if (nType != iType)
		{
			RptBlock = _T("_03.Shear_Ps_Type");
			WriteSeisType(RptBlock, iType, iParagraph4, stRow);
			nType = iType;
			nMember = D_MBTP_ETC; // 초기화
		}

		if (nMember != iMember)
		{
			RptBlock = _T("_03.Shear_Ps_Head2");
			WriteSeisPos(RptBlock, iMember, iParagraph5, stRow);
			nMember = iMember;
		}

		WriteCommon(vec, iMember, stRow, bWrited);
		stRow++;
	}

	if (bWrited == FALSE && m_pXL->IsExistSheetName(strSheetName))
		m_pXL->DeleteSheet(strSheetName);
}

void CSeisEvalReportShearPsResult::WriteCommon(const std::vector<T_SISR_K>& vec, const int& iMember, int& stRow, BOOL& bWrited)
{
	for (const auto& SisrK : vec)
	{
		T_SISR_D SisrD;
		if (!m_pDoc->m_pAttrCtrl2->GetQSisr()->Get(SisrK, SisrD)) continue;

		T_SIGR_D SigrD;
		if (!m_pDoc->m_pAttrCtrl2->GetQSigr()->Get(SisrD.kSigr, SigrD)) continue;

		T_ELEM_K_LIST aElemKeys; std::map <T_ELEM_K, std::tuple<CString, int>> mElemInfo;
		GetTargetElements(SisrD.kSigr, this->m_subType, T_SIGR_LOWER_D::kRC, iMember, aElemKeys, mElemInfo);
		if (aElemKeys.GetCount() < 1) continue;

		const int stRowGrp = stRow;
		const int nMemb = aElemKeys.GetSize();
		for (int k = 0; k < nMemb; ++k)
		{
			const auto ElemK = aElemKeys[k];
			if (mElemInfo.find(ElemK) == mElemInfo.end()) continue;
			const CString& sGroup = std::get<0>(mElemInfo[ElemK]);
			const int nShearDir = std::get<1>(mElemInfo[ElemK]);

			std::tuple<T_SIPA_K, T_SIGR_K, T_ELEM_K, CString> tParam;
			tParam = std::make_tuple(SisrD.kSipa, SisrD.kSigr, ElemK, sGroup);

			if (nShearDir == seis_jp::kYZ_Dir) {
				WriteAxisTwo(tParam, stRow, bWrited);
			}
			else {
				WriteAxisOne(tParam, stRow, bWrited);
			}
		}
		auto* pFormat = m_pXL->GetSheet()->cellFormat(stRow - 1, 0);
		m_pXL->GetSheet()->setCellFormat(stRowGrp, 0, pFormat);
		m_pXL->SetMergeCell(stRowGrp, 0, stRow + -1, 0);
		m_pXL->SetValue(stRowGrp, 0, SigrD.strName);
	}
}

void CSeisEvalReportShearPsResult::WriteAxisOne(IN std::tuple<T_SIPA_K, T_SIGR_K, T_ELEM_K, CString>& tParam, OUT int& stRow, OUT BOOL& bWrited)
{
	auto l_WriteData = [this](T_SISR_EVAL_LOWER_BASE::eDirection eDir, int nRow, int nCol, const T_SISR_EVAL_LOWER_BASE& RsltD)
		{
			const auto nThis = min(RsltD.adShear.GetSize(), 3);
			for (int i = 0; i < nThis; ++i)	WriteNanHyphenValue(nRow + i, nCol, RsltD.adShear[i]);
			WriteNanHyphenValue(nRow, ++nCol, RsltD.dShear);
			WriteNanHyphenValue(nRow, ++nCol, RsltD.dPs);
			WriteNanHyphenValue(nRow, ++nCol, RsltD.dRatio);

			if ((eDir == T_SISR_EVAL_LOWER_BASE::kMax && RsltD.dShear < 0) || (eDir == T_SISR_EVAL_LOWER_BASE::kMin && RsltD.dShear > 0)) {
				SetValue_Hyphen(nRow, ++nCol);
			}
			else {
				if (RsltD.dRatio > 1.0) {
					m_pXL->SetValue(nRow, ++nCol, _T("NG"));
					SetNG_Red(m_pXL, nRow, nCol, nRow, nCol);
				}
				else {
					m_pXL->SetValue(nRow, ++nCol, _T("OK"));
				}
			}
		};

	int rowCnt = 0;
	CString RptBlock;
	for (int iExamShearDir = 0; iExamShearDir < 2; iExamShearDir++) {
		const int sDir = iExamShearDir == 0 ? seis_jp::kY_Dir : seis_jp::kZ_Dir;

		T_SISR_EVAL_LOWER EvalD;
		EvalD.kSipa = std::get<0>(tParam);
		EvalD.kSigr = std::get<1>(tParam);
		EvalD.kElem = std::get<2>(tParam);
		EvalD.nExamShearDir = sDir;
		if (!m_pResult->GetResultSisrLower(EvalD)) continue;

		CString RptBlock = _T("_03.Shear_Ps_Body3");
		int rowCnt = CopyRange(m_pXL, RptBlock, stRow);

		(std::get<3>(tParam) == _T("--")) ? SetValue_Hyphen(stRow, 1) : m_pXL->SetValue(stRow, 1, std::get<3>(tParam));
		m_pXL->SetValue(stRow, 2, EvalD.kElem); // Element
		m_pXL->SetValue(stRow, 3, EvalD.PosI.kNode); // node
		m_pXL->SetValue(stRow + 3, 3, EvalD.PosJ.kNode);
		CString strCheckDir;
		WriteReportDir(m_nSeisDir, iExamShearDir + 1, eType::kShear, strCheckDir);

		m_pXL->SetValue(stRow, 4, strCheckDir);
		m_pXL->SetMergeCell(stRow, 4, stRow + 5, 4);

		l_WriteData(T_SISR_EVAL_LOWER_BASE::kMin, stRow, 5, EvalD.PosI.RsltMin);
		l_WriteData(T_SISR_EVAL_LOWER_BASE::kMax, stRow, 10, EvalD.PosI.RsltMax);
		l_WriteData(T_SISR_EVAL_LOWER_BASE::kMin, stRow + 3, 5, EvalD.PosJ.RsltMin);
		l_WriteData(T_SISR_EVAL_LOWER_BASE::kMax, stRow + 3, 10, EvalD.PosJ.RsltMax);

		bWrited = TRUE;
		stRow += rowCnt;
		if (m_pProgress->NextStep() == FALSE) throw FALSE;
	}
}

void CSeisEvalReportShearPsResult::WriteAxisTwo(IN std::tuple<T_SIPA_K, T_SIGR_K, T_ELEM_K, CString>& tParam, OUT int& stRow, OUT BOOL& bWrited)
{
	auto l_WriteData = [this](T_SISR_EVAL_LOWER_BASE::eDirection eDir, int nRow, int nCol, const T_SISR_EVAL_LOWER_BASE& RsltD)
		{
			const auto nThis = min(RsltD.adShear.GetSize(), 3);
			for (int i = 0; i < nThis; ++i)	WriteNanHyphenValue(nRow + i, nCol, RsltD.adShear[i]);
			WriteNanHyphenValue(nRow, ++nCol, RsltD.dShear);
			WriteNanHyphenValue(nRow, ++nCol, RsltD.dPs);
			WriteNanHyphenValue(nRow, ++nCol, RsltD.dRatio);

			if ((eDir == T_SISR_EVAL_LOWER_BASE::kMax && RsltD.dShear < 0) || (eDir == T_SISR_EVAL_LOWER_BASE::kMin && RsltD.dShear > 0)) {
				SetValue_Hyphen(nRow, ++nCol);
			}
			else {
				if (RsltD.dRatio > 1.0) {
					m_pXL->SetValue(nRow, ++nCol, _T("NG"));
					SetNG_Red(m_pXL, nRow, nCol, nRow, nCol);
				}
				else {
					m_pXL->SetValue(nRow, ++nCol, _T("OK"));
				}
			}
		};

	int rowCnt = 0;
	CString RptBlock;
	BOOL bWriteTable = FALSE;
	for (int iExamShearDir = 0; iExamShearDir < 2; iExamShearDir++) {
		const int sDir = iExamShearDir == 0 ? seis_jp::kY_Dir : seis_jp::kZ_Dir;

		T_SISR_EVAL_LOWER EvalD;
		EvalD.kSipa = std::get<0>(tParam);
		EvalD.kSigr = std::get<1>(tParam);
		EvalD.kElem = std::get<2>(tParam);
		EvalD.nExamShearDir = sDir;
		if (!m_pResult->GetResultSisrLower(EvalD)) continue;

		if (bWriteTable == FALSE) {
			CString RptBlock = _T("_03.Shear_Ps_Body4");
			rowCnt = CopyRange(m_pXL, RptBlock, stRow);
			bWriteTable = TRUE;
		}

		(std::get<3>(tParam) == _T("--")) ? SetValue_Hyphen(stRow, 1) : m_pXL->SetValue(stRow, 1, std::get<3>(tParam));
		m_pXL->SetValue(stRow, 2, EvalD.kElem);
		m_pXL->SetValue(stRow, 3, EvalD.PosI.kNode);
		m_pXL->SetValue(stRow + 6, 3, EvalD.PosJ.kNode);

		int nRow = stRow;
		// I단
		if (EvalD.nExamShearDir == seis_jp::kY_Dir)
			nRow += 3;

		l_WriteData(T_SISR_EVAL_LOWER_BASE::kMin, nRow, 5, EvalD.PosI.RsltMin);
		l_WriteData(T_SISR_EVAL_LOWER_BASE::kMax, nRow, 10, EvalD.PosI.RsltMax);

		// J단
		nRow = stRow + 6;
		if (EvalD.nExamShearDir == seis_jp::kY_Dir)
			nRow += 3;

		l_WriteData(T_SISR_EVAL_LOWER_BASE::kMin, nRow, 5, EvalD.PosJ.RsltMin);
		l_WriteData(T_SISR_EVAL_LOWER_BASE::kMax, nRow, 10, EvalD.PosJ.RsltMax);

		bWrited = TRUE;
		if (m_pProgress->NextStep() == FALSE) throw FALSE;
	}

	if (bWrited) stRow += rowCnt;
}

void CSeisEvalReportShearPsResult::AddSheet(const T_SIGR_D::eSubType& nGroupType, OUT CString& rSheetName)
{
	m_strBaseSheet = _LSX(03.Shear_Ps);
	rSheetName = _LS(IDS_BRDG_SEIS_EVAL_REPORT_JP_SHEAR) + _T("_") + GetTextGroup(nGroupType);// _T("せん断結果(橋脚)");
	if (m_pXL->IsExistSheetName(rSheetName))
	{
		ASSERT(0);
		m_pXL->SetActiveSheet(rSheetName);
	}
	else
	{
		m_pXL->AddSheet2(rSheetName);
		m_pXL->SetActiveSheet(rSheetName);
	}

	m_pXL->SetStandardFont(_T("Meiryo UI"));
	m_pXL->SetMargin(13, 12, 20, 17);
	m_pXL->SetColumnWidth(0, 0, 0, 3, 6.9 * _ToPoint); //  6.9 * 0.833333333
	m_pXL->SetColumnWidth(0, 4, 0, 60, 7.4 * _ToPoint); //  7.4 * 0.833333333
	m_pXL->SetRowHeight(16.5);
	m_pXL->SetZoom(70);
}

void CSeisEvalReportShearPsResult::WirteTheory(const T_SIGR_D::eSubType& nGroupType, const int iParagraph1, int& iParagraph2, OUT int& stRow)
{
	CString RptBlock = _T("_03.Shear_Ps_Title");
	int rowCnt = CopyRange(m_pXL, RptBlock, stRow);
	m_pXL->SetValue(stRow, 0, GetText(_T("%d. %s"), iParagraph1, GetTextTitle(nGroupType)));
	stRow += rowCnt;

	RptBlock = _T("_03.Shear_Ps_Theory2");
	rowCnt = CopyRange(m_pXL, RptBlock, stRow);
	m_pXL->SetValue(stRow, 0, GetText(_T("%d-%d. %s"), iParagraph1, ++iParagraph2, _LS(IDS_BRDG_SEIS_EVAL_REPORT_JP_CHK_SHEAR))); // せん断の照査
	stRow += rowCnt;
	stRow++;
}
#pragma endregion

#pragma endregion CSeisEvalReportShear 전단

#pragma region // CSeisEvalReportDispPlastRatio
CSeisEvalReportDispPlastRatio::CSeisEvalReportDispPlastRatio(IExcel* pXL, CDgnProgressDlg* pProgress, const CString& strBasePath, const CString& strBaseBook, const SDgnExcelPrintOpt* pPrintOpt)
	: CWriteSeisEvalReportBase(pXL, pProgress, strBasePath, strBaseBook, pPrintOpt)
{
	m_pDispPierNEXCO = m_nDesignCode == BrdgSE_CodeJP::JROAD5_H14 ? new CDispPlasticRatNEXCO_H14(pXL, pProgress, strBasePath, strBaseBook, pPrintOpt) : new CDispPlasticRatNEXCO_H24(pXL, pProgress, strBasePath, strBaseBook, pPrintOpt);
	m_pDispPierJBEC = m_nDesignCode == BrdgSE_CodeJP::JROAD5_H14 ? new CDispPlasticRatJBEC_H14(pXL, pProgress, strBasePath, strBaseBook, pPrintOpt) :  new CDispPlasticRatJBEC_H24(pXL, pProgress, strBasePath, strBaseBook, pPrintOpt);
}

CSeisEvalReportDispPlastRatio::~CSeisEvalReportDispPlastRatio()
{
	if (m_pDispPierNEXCO) delete m_pDispPierNEXCO;
	if (m_pDispPierJBEC) delete m_pDispPierJBEC;
}

void CSeisEvalReportDispPlastRatio::AddStep()
{
	T_KEY_LIST aSidpK;
	m_pDoc->m_pAttrCtrl2->GetQSidp()->GetKeyList(aSidpK);
	const auto nSidp = aSidpK.GetSize();
	if (nSidp == 0) return;

	T_KEY_LIST aSigrK;
	m_pDoc->m_pAttrCtrl2->GetQSigr()->GetKeyList(aSigrK);
	const auto nSigr = aSigrK.GetSize();
	if (nSigr == 0) return;

	auto l_GetSipaKey = [&](const T_SIGR_K& SigrK, const T_SIPA_D::eSeismicForceDirection& enDir, const T_SIPA_D::eSeismicForceType& enType)
		{
			for (int i = 0; i < nSidp; ++i)
			{
				const auto& SidpK = aSidpK[i];
				T_SIDP_D SidpD;
				if (!m_pDoc->m_pAttrCtrl2->GetQSidp()->Get(SidpK, SidpD))	continue;
				if (SidpD.kSigrLower != SigrK)								continue;

				const auto& SipaK = SidpD.kSipa;
				T_SIPA_D SipaD;
				if (!m_pDoc->m_pAttrCtrl2->GetQSipa()->Get(SipaK, SipaD))	continue;
				if (SipaD.nDesignCode != m_nDesignCode)						continue;
				if (SipaD.nSeismicForceDirection != enDir)					continue;
				if (SipaD.nSeismicForceType != enType)						continue;

				return SipaK;
			}

			return T_SIPA_K(0);
		};

	for (int i = 0; i < 2; ++i) // 橋軸方向, 橋軸直角方向
	{
		const auto enDir = i == 0 ? T_SIPA_D::kAxis_Dir : T_SIPA_D::kPerp_Dir;
		for (int j = 0; j < nSigr; ++j)
		{
			const auto SigrK = aSigrK[j];
			const auto SipaK_I = l_GetSipaKey(SigrK, enDir, T_SIPA_D::kType1);
			const auto SipaK_II = l_GetSipaKey(SigrK, enDir, T_SIPA_D::kType2);

			if (SipaK_I != 0 || SipaK_II != 0)
			{
				m_pProgress->m_nTotalSteps++;
			}
		}
	}
}

void CSeisEvalReportDispPlastRatio::Write()
{
//	3. 下部構造の3降伏照査 하부구조항복조사
//		3-2. 塑性率の照査 소성율조사	
//			3-2-1. 橋軸方向 교축방향
//			3-2-2. 橋軸直角方向 교축직각방향

	std::map<std::tuple<int, int, int, int>, std::vector<T_SIDP_K>> mExistSidp;
	if (!IsExistData(mExistSidp)) return;

	AddSheet();

	int stRow = 0;
	*this->m_pParagraph1 = *this->m_pParagraph1 + 1;
	// 1. 変位の照査
	CString RptBlock = _T("_03.DispPlasticRat_Title");
	int rowCnt = CopyRange(m_pXL, RptBlock, stRow);
	m_pXL->SetValue(stRow, 0, GetText(_T("%d. %s"), *this->m_pParagraph1, _LS(IDS_BRDG_SEIS_EVAL_REPORT_JP_DISP_PLASTIC_RATIO_TITLE)));
	stRow += rowCnt;

	int iParagraph3 = 0;
	int nRspCalcType = seis_jp::kRspType_0;
	int nMaterial = T_SIGR_LOWER_D::kUnKnown;
	int nAlwCalcType = seis_jp::kAlwCalcType_0;
	int nSeisDir = T_SIPA_D::kUnKnown; // 1: 교축방향, 2: 교축직각방향
	for (const auto& it : mExistSidp)
	{
		const auto& key = it.first;
		const auto& vecSird = it.second;
		if (vecSird.size() < 1) continue;

		const int iRspCalcType = std::get<0>(key);
		const int iMaterial = std::get<1>(key);
		const int iAlwCalcType = std::get<2>(key);
		const int iSeisDir = std::get<3>(key);

		if (iRspCalcType != nRspCalcType ||  iAlwCalcType != nAlwCalcType)
		{
			if (iRspCalcType == seis_jp::kNEXCO)
				m_pDispPierNEXCO->WirteTheory(iMaterial, iAlwCalcType, *this->m_pParagraph1, *this->m_pParagraph2, stRow);
			else if (iRspCalcType == seis_jp::kJBEC)
				m_pDispPierJBEC->WirteTheory(iMaterial, iAlwCalcType, *this->m_pParagraph1, *this->m_pParagraph2, stRow);

			nRspCalcType = iRspCalcType;
			nMaterial = iMaterial;
			nAlwCalcType = iAlwCalcType;
			nSeisDir = T_SIPA_D::kUnKnown; // 초기화
		}

		if (iSeisDir != nSeisDir)
		{
			CString RptBlock = _T("_03.DispPlasticRat_Head");
			WriteSeisDir(RptBlock, iSeisDir, iParagraph3, stRow);
			nSeisDir = iSeisDir;
		}

		// Set Data
		if (iRspCalcType == seis_jp::kNEXCO)
		{
			m_pDispPierNEXCO->Write(vecSird, nSeisDir, iParagraph3, stRow);
		}
		else if (iRspCalcType == seis_jp::kJBEC)
		{
			m_pDispPierJBEC->Write(vecSird, nSeisDir, iParagraph3, stRow);
		}
	}
}

void CSeisEvalReportDispPlastRatio::AddSheet()
{
	m_strBaseSheet = _LSX(03.DispPlasticRat);
	const CString strNewSheet = _LS(IDS_BRDG_SEIS_EVAL_REPORT_JP_DISP_PLASTIC_RAT) + _T("_") + GetTextGroup(T_SIGR_D::kLower); // _T("変位塑性率");
	if (m_pXL->IsExistSheetName(strNewSheet))
	{
		ASSERT(0);
		m_pXL->SetActiveSheet(strNewSheet);
	}
	else
	{
		m_pXL->AddSheet2(strNewSheet);
		m_pXL->SetActiveSheet(strNewSheet);
	}

	m_pXL->SetStandardFont(_T("Meiryo UI"));
	m_pXL->SetMargin(13, 12, 20, 17);
	m_pXL->SetColumnWidth(0, 0, 0, 1, 6.9 * _ToPoint); //  6.9 * 0.833333333
	m_pXL->SetColumnWidth(0, 2, 0, 2, 16.9 * _ToPoint); // 16.9 * 0.833333333
	m_pXL->SetColumnWidth(0, 3, 0, 60, 7.4 * _ToPoint); //  7.4 * 0.833333333
	m_pXL->SetRowHeight(16.5);
	m_pXL->SetZoom(70);
}

BOOL CSeisEvalReportDispPlastRatio::GetData(OUT CArray<T_SIDP_K, T_SIDP_K>& aSidpK, OUT CArray<T_SIGR_K, T_SIGR_K>& aSigrK)
{
	m_pDoc->m_pAttrCtrl2->GetQSidp()->GetKeyList(aSidpK);
	if (aSidpK.GetSize() == 0) return FALSE;

	m_pDoc->m_pAttrCtrl2->GetQSigr()->GetKeyList(aSigrK);
	if (aSigrK.GetSize() == 0) return FALSE;

	return TRUE;
}

void CSeisEvalReportDispPlastRatio::WirteTheory(const int iParagraph1, int& iParagraph2, OUT int& stRow)
{
	CBrdgSeisEvalResult_JP* pResult = m_pDoc->m_pPostCtrl->GetBrdgSeisEvalResult_JP(); if (!pResult) { ASSERT(0); return; }

	int eCode = 0;
	int nAllowDispPlasFormula = 0;
	double dAlpha = 0;

	const std::map<SIDP_EVAL_KEY, T_SIDP_EVAL>& mSidpResult = pResult->GetResultSidpMap();
	std::map<SIDP_EVAL_KEY, T_SIDP_EVAL>::const_iterator iter;
	for (iter = mSidpResult.begin(); iter != mSidpResult.end(); ++iter)
	{
		const SIDP_EVAL_KEY& key = iter->first;
		const T_SIDP_EVAL& data = iter->second;

		T_SIPA_D SipaD;
		if (!m_pDoc->m_pAttrCtrl2->GetQSipa()->Get(data.kSipa, SipaD)) continue;
		if (SipaD.nDesignCode != m_nDesignCode) continue;

		T_SIDP_D sidp;
		if (!m_pDoc->m_pAttrCtrl2->GetQSidp()->Get(data.kSidp, sidp)) continue;

		eCode = SipaD.nDesignCode;
		nAllowDispPlasFormula = sidp.ADPR_D.nAllowDispPlasFormula;
		dAlpha = sidp.ADPR_D.dAlpha;
		break;
	}

	if (eCode == 0) return;

	CString RptBlock = _T("_03.DispPlasticRat_Theory");
	if (eCode == BrdgSE_CodeJP::JROAD5_H14)
		RptBlock = _T("_03.DispPlasticRat_Theory_H14");

	int rowCnt = CopyRange(m_pXL, RptBlock, stRow);

	//N-N. 塑性率の照査
	m_pXL->SetValue(stRow, 0, GetText(_T("%d-%d. %s"), iParagraph1, ++iParagraph2, _LS(IDS_BRDG_SEIS_EVAL_REPORT_JP_CHK_PLAS)));

	//허용소성율 μa
	CString str;
	if (eCode == BrdgSE_CodeJP::JROAD5_H24) {
		if (nAllowDispPlasFormula == T_SIDP_ADPR_D::kMuA_1_Plus_Ellipsis)
			str = _LS(IDS_IDD_TM_SIDP_IDC_TM_SIDP_ALLO_DISP_PLAS_RATE1_RDO);//_T("μa=1+(δls-δy)/(αδy)");
		else if (nAllowDispPlasFormula == T_SIDP_ADPR_D::kMuA_DeltaLs_Ellipsis)
			str = _LS(IDS_IDD_TM_SIDP_IDC_TM_SIDP_ALLO_DISP_PLAS_RATE2_RDO);//_T("μa=δls/(αδy)");

		m_pXL->SetValue(stRow + 7, 3, GetText(_T("%s"), str));
	}

	//안전계수 α
	m_pXL->SetValue(stRow + 8, 3, GetText(_T("%g"), dAlpha));

	stRow += rowCnt;
	stRow++;
}

BOOL CSeisEvalReportDispPlastRatio::IsExistData(std::map<std::tuple<int, int, int, int>, std::vector<T_SIDP_K>>& mExistSidp)
{
	CArray<T_SIDP_K, T_SIDP_K> aSidpK;
	CArray<T_SIGR_K, T_SIGR_K> aSigrK;
	if (GetData(aSidpK, aSigrK) == FALSE) return FALSE;

	for (int i = 0; i < aSidpK.GetSize(); i++)
	{
		const T_SIDP_K SidpK = aSidpK[i];

		T_SIDP_D SidpD;
		if (!m_pDoc->m_pAttrCtrl2->GetQSidp()->Get(SidpK, SidpD)) continue;

		//T_SIGR_D SigrD;
		//if (!m_pDoc->m_pAttrCtrl2->GetQSigr()->Get(SidpD.kSigr, SigrD)) continue;

		T_SIGR_LOWER_D SigrLowerD;
		if(!m_pDoc->m_pAttrCtrl2->GetQSigrLower()->Get(SidpD.kSigrLower, SigrLowerD)) continue;

		T_SIPA_D SipaD;
		if (!m_pDoc->m_pAttrCtrl2->GetQSipa()->Get(SidpD.kSipa, SipaD)) continue;
		if (SipaD.nDesignCode != m_nDesignCode)							continue;

		T_SIMD_D SimdD;
		if (!m_pDoc->m_pAttrCtrl2->GetQSimd()->Get(SigrLowerD.kSigr, SidpD.kSipa, SimdD)) continue;

		const auto nPos = (int)GetExistPos(SigrLowerD.kSigr, SidpD.kSipa); // 1: 왼쪽(전체계) 2: 오른쪽(전체계) 3: 아래(교각별)
		if (nPos == T_SIMD_EVAL::kUnKnown) continue;

		const auto nRspCalcType = (int)SimdD.nRspCalcType; // 계산방법
		auto nMaterial = (int)SigrLowerD.nMaterialType; // 재료 타입 -> 변위소성률은 재료에 구분은 없지만 일단 넣어둠
		nMaterial = nMaterial == T_SIGR_LOWER_D::kSRC ? T_SIGR_LOWER_D::kSteel : nMaterial; // SRC -> Steel로 간주
		nMaterial = VERSION_965_DEFAULT; // v970 삭제
		const auto nAlwCalcType = VERSION_965_DEFAULT; // v970 ->  (int)SimdD.nAlwCalcType; // 허용치계산방법
		const auto nSeisDir = (int)SipaD.nSeismicForceDirection; // 방향

		mExistSidp[std::make_tuple(nRspCalcType, nMaterial, nAlwCalcType, nSeisDir)].push_back(SidpK); // 우선순위...
	}

	if (mExistSidp.size() < 1) return FALSE;
	return TRUE;
}

int CSeisEvalReportDispPlastRatio::GetExistPos(const T_SIGR_K& SigrK, const T_SIPA_K& SipaK)
{
	for (int k = T_SIRD_EVAL::kLeft; k <= T_SIRD_EVAL::kLower; ++k) // 좌/우
	{
		const auto nPos = k;
		T_SIRD_EVAL EvalD;
		const bool bExist = m_pResult->GetResultSird(SipaK, SigrK, nPos, EvalD);
		if (bExist)
		{
			return nPos;
		}
	}
	return T_SIRD_EVAL::kUnKnown; // 없음
}

#pragma endregion

#pragma region // CSeisEvalReportCurvatureRC
CSeisEvalReportCurvatureRC::CSeisEvalReportCurvatureRC(IExcel* pXL, CDgnProgressDlg* pProgress, const CString& strBasePath, const CString& strBaseBook, const SDgnExcelPrintOpt* pPrintOpt)
	: CWriteSeisEvalReportBase(pXL, pProgress, strBasePath, strBaseBook, pPrintOpt)
{
}

CSeisEvalReportCurvatureRC::~CSeisEvalReportCurvatureRC()
{
}

void CSeisEvalReportCurvatureRC::AddStep()
{
	T_KEY_LIST aSibdK;
	m_pDoc->m_pAttrCtrl2->GetQSibd()->GetKeyList(aSibdK);
	if (aSibdK.GetSize() < 1) return;

	std::vector<T_SIBD_K> vExistSibd[2][2][2][2];
	for (int i = 0; i < aSibdK.GetSize(); ++i)
	{
		const T_SIBD_K SibdK = aSibdK[i];
		T_SIBD_D SibdD;
		if (!m_pDoc->m_pAttrCtrl2->GetQSibd()->Get(SibdK, SibdD))		continue;
		if (SibdD.nSibdType != T_SIBD_D::kCurvatureSurvey) continue;

		T_SIGR_D SigrD;
		if (!m_pDoc->m_pAttrCtrl2->GetQSigr()->Get(SibdD.kSigr, SigrD)) continue;
		if (SigrD.nSubType != this->m_subType)								continue;

		T_SIPA_D SipaD;
		if (!m_pDoc->m_pAttrCtrl2->GetQSipa()->Get(SibdD.kSipa, SipaD)) continue;
		if (SipaD.nDesignCode != m_nDesignCode)							continue;

		const auto idx0 = (int)SibdD.CVSV_D.nCvsvType - 1; // 0: 항복조사, 1: 한계상태조사
		const auto iIdx1 = (int)SipaD.nSeismicForceDirection - 1;
		const auto iIdx2 = (int)SipaD.nSeismicForceType - 1;
		if (this->m_subType == T_SIGR_D::kUpper || this->m_subType == T_SIGR_D::kUser)
			vExistSibd[idx0][iIdx1][iIdx2][0].push_back(SibdK);
		else
		{
			T_SIGR_LOWER_D SigrLowerD;
			if (!m_pDoc->m_pAttrCtrl2->GetQSigrLower()->Get(SibdD.kSigr, SigrLowerD)) continue;
			if (SigrLowerD.nMaterialType != T_SIGR_LOWER_D::kRC) continue;

			if (SigrLowerD.BEAM_D.GetSize() > 0)
				vExistSibd[idx0][iIdx1][iIdx2][0].push_back(SibdK);
			if (SigrLowerD.COLM_D.GetSize() > 0)
				vExistSibd[idx0][iIdx1][iIdx2][1].push_back(SibdK);
		}
	}

	for (int iCond = 0; iCond < 2; ++iCond)
	{
		if (vExistSibd[iCond][0][0][0].size() == 0 && vExistSibd[iCond][0][1][0].size() == 0 &&
			vExistSibd[iCond][1][0][0].size() == 0 && vExistSibd[iCond][1][1][0].size() == 0 &&
			vExistSibd[iCond][0][0][1].size() == 0 && vExistSibd[iCond][0][1][1].size() == 0 &&
			vExistSibd[iCond][1][0][1].size() == 0 && vExistSibd[iCond][1][1][1].size() == 0) continue;

		const int nSurveyCondition = iCond == 0 ? T_SIBD_CVSV_D::kSurrenderSurvey : T_SIBD_CVSV_D::kLimitStateSurvey;

		for (int iSeisDir = 0; iSeisDir < 2; ++iSeisDir) // 橋軸方向, 橋軸直角方向
		{
			if (vExistSibd[iCond][iSeisDir][0][0].size() == 0 && vExistSibd[iCond][iSeisDir][1][0].size() == 0 &&
				vExistSibd[iCond][iSeisDir][0][1].size() == 0 && vExistSibd[iCond][iSeisDir][1][1].size() == 0) continue;

			for (int iType = 0; iType < 2; ++iType) // タイプ type
			{
				if (vExistSibd[iCond][iSeisDir][iType][0].size() == 0 && vExistSibd[iCond][iSeisDir][iType][1].size() == 0) continue;

				for (int iPos = 0; iPos < 2; ++iPos)
				{
					const auto nMembType = iPos == 0 ? D_MBTP_BEAM : D_MBTP_COLUMN;
					if (vExistSibd[iCond][iSeisDir][iType][iPos].size() == 0) continue;

					for (const auto& SibdK : vExistSibd[iCond][iSeisDir][iType][iPos])
					{
						T_SIBD_D SibdD;
						if (!m_pDoc->m_pAttrCtrl2->GetQSibd()->Get(SibdK, SibdD))		continue;

						T_SIGR_D SigrD;
						if (!m_pDoc->m_pAttrCtrl2->GetQSigr()->Get(SibdD.kSigr, SigrD)) continue;
						if (SigrD.nSubType != this->m_subType) continue;

						T_ELEM_K_LIST aElemKeys; std::map <T_ELEM_K, std::tuple<CString, int>> mElemInfo;
						GetTargetElements(SibdD.kSigr, this->m_subType, T_SIGR_LOWER_D::kRC, nMembType, aElemKeys, mElemInfo);
						if (aElemKeys.GetCount() < 1) continue;

						const int nMemb = aElemKeys.GetSize();
						for (int k = 0; k < nMemb; ++k)
						{
							const T_ELEM_K& ElemK = aElemKeys[k];
							if (mElemInfo.find(ElemK) == mElemInfo.end()) continue;

							//const CString& sGroup = std::get<0>(mElemInfo[ElemK]);
							//const int nShearDir = std::get<1>(mElemInfo[ElemK]);

							for (int d = 0; d < 2; d++)
							{
								int nShearDir = d == 0 ? seis_jp::kY_Dir : seis_jp::kZ_Dir;
								if (nSurveyCondition == T_SIBD_CVSV_D::kSurrenderSurvey)
								{
									T_SIBD_EVAL_PHI_Y eval;
									eval.kSipa = SibdD.kSipa;
									eval.kSigr = SibdD.kSigr;
									eval.kElem = ElemK;
									eval.nExamShearDir = nShearDir;
									if (!m_pResult->GetResultSibdPhiY(eval))  continue;
									m_pProgress->m_nTotalSteps++;
								}
								else
								{
									T_SIBD_EVAL_PHI_A eval;
									eval.kSipa = SibdD.kSipa;
									eval.kSigr = SibdD.kSigr;
									eval.kElem = ElemK;
									eval.nExamShearDir = nShearDir;
									if (!m_pResult->GetResultSibdPhiA(eval))  continue;
									m_pProgress->m_nTotalSteps++;
								}
							}
						}
					}
				}
			}
		}
	}
}

void CSeisEvalReportCurvatureRC::AddSheet(const T_SIGR_D::eSubType& nGroupType, CString& strSheetName)
{
	m_strBaseSheet = _LSX(03.Curvature_RC);
	strSheetName = _LS(IDS_BRDG_SEIS_EVAL_REPORT_JP_CURVATURE_RC) + _T("_") + GetTextGroup(nGroupType);// _T("曲率");
	if (m_pXL->IsExistSheetName(strSheetName))
	{
		ASSERT(0);
		m_pXL->SetActiveSheet(strSheetName);
	}
	else
	{
		m_pXL->AddSheet2(strSheetName);
		m_pXL->SetActiveSheet(strSheetName);
	}

	m_pXL->SetStandardFont(_T("Meiryo UI"));
	m_pXL->SetMargin(13, 12, 20, 17);
	m_pXL->SetColumnWidth(0, 0, 0, 60, 6.9 * _ToPoint); // 6.9 * 0.833333333
	m_pXL->SetRowHeight(16.5);
	m_pXL->SetZoom(70);
}

void CSeisEvalReportCurvatureRC::WirteTheory(const T_SIGR_D::eSubType& nGroupType, const int& nSurveyType,
	const int iParagraph1, int& iParagraph2, OUT int& stRow)
{
	CBrdgSeisEvalResult_JP* pResult = m_pDoc->m_pPostCtrl->GetBrdgSeisEvalResult_JP(); if (!pResult) { ASSERT(0); return; }

	// RC部材に対する降伏照査 
	CString RptBlock = (nSurveyType == T_SIBD_CVSV_D::kSurrenderSurvey) ? _T("_03.Curvature_RC_Theory1") : _T("_03.Curvature_RC_Theory2");
	int rowCnt = CopyRange(m_pXL, RptBlock, stRow);
	m_pXL->SetValue(stRow, 0, GetText(_T("%d. %s"), iParagraph1, GetTextTitle(nGroupType)));
	m_pXL->SetValue(stRow + 1, 0, GetText(_T("%d-%d. %s"), iParagraph1, ++iParagraph2, _LS(IDS_BRDG_SEIS_EVAL_REPORT_JP_CHK_FLEX)));// 曲げの照査

	if (nSurveyType == T_SIBD_CVSV_D::kLimitStateSurvey)
	{
		int nAllowCurvFormula = 0;
		double dAlpha = 0;
		GetInputValue(nAllowCurvFormula, dAlpha);

		//耐震性能照査に用いる許容曲率 φa
		CString str = _LS(IDS_BRDG_SEIS_EVAL_REPORT_JP_ALLOW_CURV_FORMULA);
		if (m_nDesignCode == BrdgSE_CodeJP::JROAD5_H14) {
			ASSERT(nAllowCurvFormula == T_SIBD_CVSV_D::kPhiA_Phiy0_Ellipsis);
			str += _T("φa=φy0 + (φu-φy0)/α");
		}
		else {
			if (nAllowCurvFormula == T_SIBD_CVSV_D::kPhiA_Phiy0_Ellipsis)
				str += _T("φa=φy0 + (φls-φy0)/α");
			else if (nAllowCurvFormula == T_SIBD_CVSV_D::kPhiA_Phiyls_Ellipsis)
				str += _T("φa=φls/α");
		}
		m_pXL->SetValue(stRow + 5, 2, GetText(_T("%s"), str));
		//안전계수 α
		m_pXL->SetValue(stRow + 7, 5, GetText(_T("%g"), dAlpha));
	}

	stRow += rowCnt;
	stRow++;
}

BOOL CSeisEvalReportCurvatureRC::IsExistData(std::map<std::tuple<int, int, int, int>, std::vector<T_SIBD_K>>& mExistSibd)
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
		if (SigrD.nSubType != this->m_subType)								continue;

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
			if (this->m_subType == T_SIGR_D::kUpper || this->m_subType == T_SIGR_D::kUser)
				mExistSibd[std::make_tuple(nSurveyCondition, nSeisDir, nSeisForce, D_MBTP_BEAM)].push_back(SibdK);
		}
	}

	if (mExistSibd.size() < 1) return FALSE;
	else return TRUE;
}

void CSeisEvalReportCurvatureRC::Write()
{
	//	3. 下部構造の3降伏照査 하부구조항복조사
	//		3-1. 曲げの照査 곡률조사
	//			3-1-1. 橋軸方向 교축방향
	//				1) タイプⅠ
	//					(1) 柱部の結果
	//					(2) 梁部の結果
	//				2) タイプⅡ
	//					(1) 柱部の結果
	//					(2) 梁部の結果
	//			3-1-2. 橋軸直角方向 교축직각방향
	//				1) タイプⅠ
	//					(1) 柱部の結果
	//					(2) 梁部の結果
	//				2) タイプⅡ
	//					(1) 柱部の結果
	//					(2) 梁部の結果
	std::map<std::tuple<int, int, int, int>, std::vector<T_SIBD_K>> mExistSibd;
	if (!IsExistData(mExistSibd)) return;

	CString RptBlock;
	int rowCnt = 0;
	int stRow = 0;

	CString strSheetName;
	AddSheet(this->m_subType, strSheetName);
	*this->m_pParagraph1 = *this->m_pParagraph1 + 1;

	BOOL bWrited = FALSE;
	int iParagraph3 = 0;
	int iParagraph4 = 0;
	int iParagraph5 = 0;

	int nSurveyCondition = T_SIBD_CVSV_D::kUnKnown; // 1: 항복조사, 2: 한계상태조사
	int nSeisDir = T_SIPA_D::kUnKnown; // 1: 교축방향, 2: 교축직각방향
	int nType = T_SIPA_D::kUnKnown; // 1: 타입Ⅰ, 2: 타입Ⅱ
	int nMember = D_MBTP_ETC; // 1: 梁部, 2: 柱部
	for (const auto& pair : mExistSibd)
	{
		const auto& key = pair.first;
		const auto& vec = pair.second;
		if (vec.size() == 0) continue;

		const int iCond = std::get<0>(key);
		const int iSeisDir = std::get<1>(key);
		const int iType = std::get<2>(key);
		const int iMember = std::get<3>(key);
		if (nSurveyCondition != iCond)
		{
			WirteTheory(this->m_subType, iCond, *this->m_pParagraph1, *this->m_pParagraph2, stRow); // 이론
			nSurveyCondition = iCond;
		}

		if (nSeisDir != iSeisDir)
		{
			RptBlock = _T("_03.Curvature_RC_Dir");
			WriteSeisDir(RptBlock, iSeisDir, iParagraph3, stRow);
			nSeisDir = iSeisDir;
			nType = T_SIPA_D::kUnKnown; // 초기화
		}

		if (nType != iType)
		{
			RptBlock = _T("_03.Curvature_RC_Type");
			WriteSeisType(RptBlock, iType, iParagraph4, stRow);
			nType = iType;
			nMember = D_MBTP_ETC; // 초기화
		}

		if (nMember != iMember)
		{
			RptBlock = (iCond == T_SIBD_CVSV_D::kSurrenderSurvey) ? _T("_03.Curvature_RC_Head1") : _T("_03.Curvature_RC_Head2");
			WriteSeisPos(RptBlock, iMember, iParagraph5, stRow);
			nMember = iMember;
		}

		WriteCommon(vec, iMember, iCond, stRow, bWrited);
		stRow++;
	}

	if (bWrited == FALSE && m_pXL->IsExistSheetName(strSheetName))
		m_pXL->DeleteSheet(strSheetName);
}

void CSeisEvalReportCurvatureRC::WriteCommon(const std::vector<T_SIBD_K>& vec, const int& iMember, const int& iCond, int& stRow, BOOL& bWrited)
{
	for (const auto& SibdK : vec)
	{
		T_SIBD_D SibdD;
		if (!m_pDoc->m_pAttrCtrl2->GetQSibd()->Get(SibdK, SibdD))		continue;

		T_SIGR_D SigrD;
		if (!m_pDoc->m_pAttrCtrl2->GetQSigr()->Get(SibdD.kSigr, SigrD)) continue;
		if (SigrD.nSubType != this->m_subType) continue;

		T_ELEM_K_LIST aElemKeys; std::map <T_ELEM_K, std::tuple<CString, int>> mElemInfo;
		GetTargetElements(SibdD.kSigr, this->m_subType, T_SIGR_LOWER_D::kRC, iMember, aElemKeys, mElemInfo);
		if (aElemKeys.GetCount() < 1) continue;

		const int stRowGrp = stRow;
		const int nMemb = aElemKeys.GetSize();
		for (int k = 0; k < nMemb; ++k)
		{
			const T_ELEM_K& ElemK = aElemKeys[k];
			if (mElemInfo.find(ElemK) == mElemInfo.end()) continue;

			//const CString& sGroup = std::get<0>(mElemInfo[ElemK]);
			const int nShearDir = std::get<1>(mElemInfo[ElemK]);
			const CString strGroupName = (mElemInfo.find(ElemK) == mElemInfo.end()) ? _T("--") : std::get<0>(mElemInfo[ElemK]);

			std::tuple<T_SIPA_K, T_SIGR_K, T_ELEM_K, CString, CString> tParam; // 내진 방향 포함
			tParam = std::make_tuple(SibdD.kSipa, SibdD.kSigr, ElemK, SigrD.strName, strGroupName);

			if (nShearDir == seis_jp::kYZ_Dir) {
				WriteAxisTwo(iCond, tParam, stRow, bWrited);
			}
			else {
				WriteAxisOne(iCond, tParam, stRow, bWrited);
			}
		}
		auto* pFormat = m_pXL->GetSheet()->cellFormat(stRow - 1, 0);
		m_pXL->GetSheet()->setCellFormat(stRowGrp, 0, pFormat);
		m_pXL->SetMergeCell(stRowGrp, 0, stRow + -1, 0);
		m_pXL->SetValue(stRowGrp, 0, SigrD.strName);
	}
}

void CSeisEvalReportCurvatureRC::WriteAxisOne(IN const int& nSurveyCondition, IN std::tuple<T_SIPA_K, T_SIGR_K, T_ELEM_K, CString, CString>& tParam, OUT int& stRow, OUT BOOL& bWrited)
{
	int rowCnt = 0;
	CString RptBlock;
	for (int iExamShearDir = 0; iExamShearDir < 2; iExamShearDir++) {
		const int sDir = iExamShearDir == 0 ? seis_jp::kY_Dir : seis_jp::kZ_Dir;

		if (nSurveyCondition == T_SIBD_CVSV_D::kSurrenderSurvey)
		{
			T_SIBD_EVAL_PHI_Y eval;
			eval.kSipa = std::get<0>(tParam);
			eval.kSigr = std::get<1>(tParam);
			eval.kElem = std::get<2>(tParam);
			eval.nExamShearDir = sDir;
			if (!m_pResult->GetResultSibdPhiY(eval))  continue;

			RptBlock = _T("_03.Curvature_RC_Body1");
			rowCnt = CopyRange(m_pXL, RptBlock, stRow);
			m_pXL->SetValue(stRow, 0, std::get<3>(tParam));
			m_pXL->SetValue(stRow, 1, std::get<4>(tParam));
			m_pXL->SetValue(stRow, 2, eval.kElem);

			CString strCheckDir;
			WriteReportDir(m_nSeisDir, iExamShearDir + 1, eType::kFlexure_Phi, strCheckDir);
			m_pXL->SetValue(stRow, 3, strCheckDir);

			WriteData(&eval.analDeform, &eval.resultRc, &eval.calcDeformMax, &eval.calcDeformMin, stRow, bWrited);
		}
		else
		{
			T_SIBD_EVAL_PHI_A eval;
			eval.kSipa = std::get<0>(tParam);
			eval.kSigr = std::get<1>(tParam);
			eval.kElem = std::get<2>(tParam);
			eval.nExamShearDir = sDir;
			if (!m_pResult->GetResultSibdPhiA(eval))  continue;

			RptBlock = _T("_03.Curvature_RC_Body1");
			rowCnt = CopyRange(m_pXL, RptBlock, stRow);
			m_pXL->SetValue(stRow, 0, std::get<3>(tParam));
			m_pXL->SetValue(stRow, 1, std::get<4>(tParam));
			m_pXL->SetValue(stRow, 2, eval.kElem);

			CString strCheckDir;
			WriteReportDir(m_nSeisDir, iExamShearDir + 1, eType::kFlexure_Phi, strCheckDir);
			m_pXL->SetValue(stRow, 3, strCheckDir);

			WriteData(&eval.analDeform, &eval.resultRc, &eval.calcDeformMax, &eval.calcDeformMin, stRow, bWrited);
		}

		stRow += rowCnt;
		if (m_pProgress->NextStep() == FALSE) throw FALSE;
	}
}

void CSeisEvalReportCurvatureRC::WriteAxisTwo(IN const int& nSurveyCondition, IN std::tuple<T_SIPA_K, T_SIGR_K, T_ELEM_K, CString, CString>& tParam, OUT int& stRow, OUT BOOL& bWrited)
{
	int rowCnt = 0;
	CString RptBlock;
	BOOL bWriteTable = FALSE;
	for (int iExamShearDir = 0; iExamShearDir < 2; iExamShearDir++) {
		const int sDir = iExamShearDir == 0 ? seis_jp::kY_Dir : seis_jp::kZ_Dir;

		if (nSurveyCondition == T_SIBD_CVSV_D::kSurrenderSurvey)
		{
			T_SIBD_EVAL_PHI_Y eval;
			eval.kSipa = std::get<0>(tParam);
			eval.kSigr = std::get<1>(tParam);
			eval.kElem = std::get<2>(tParam);
			eval.nExamShearDir = sDir;
			if (!m_pResult->GetResultSibdPhiY(eval))  continue;

			if (bWriteTable == FALSE) {
				bWriteTable = TRUE;
				RptBlock = _T("_03.Curvature_RC_Body3");
				rowCnt = CopyRange(m_pXL, RptBlock, stRow);

				m_pXL->SetValue(stRow, 0, std::get<3>(tParam));
				m_pXL->SetValue(stRow, 1, std::get<4>(tParam));
				m_pXL->SetValue(stRow, 2, eval.kElem);
			}

			int nRow = stRow;
			if (eval.nExamShearDir == seis_jp::kY_Dir)
				nRow += 3;
			WriteData(&eval.analDeform, &eval.resultRc, &eval.calcDeformMax, &eval.calcDeformMin, nRow, bWrited);
		}
		else
		{
			T_SIBD_EVAL_PHI_A eval;
			eval.kSipa = std::get<0>(tParam);
			eval.kSigr = std::get<1>(tParam);
			eval.kElem = std::get<2>(tParam);
			eval.nExamShearDir = sDir;
			if (!m_pResult->GetResultSibdPhiA(eval))  continue;

			if (bWriteTable == FALSE) {
				bWriteTable = TRUE;
				RptBlock = _T("_03.Curvature_RC_Body3");
				rowCnt = CopyRange(m_pXL, RptBlock, stRow);

				m_pXL->SetValue(stRow, 0, std::get<3>(tParam));
				m_pXL->SetValue(stRow, 1, std::get<4>(tParam));
				m_pXL->SetValue(stRow, 2, eval.kElem);
			}

			int nRow = stRow;
			if (eval.nExamShearDir == seis_jp::kY_Dir)
				nRow += 3;
			WriteData(&eval.analDeform, &eval.resultRc, &eval.calcDeformMax, &eval.calcDeformMin, nRow, bWrited);
		}
		if (m_pProgress->NextStep() == FALSE) throw FALSE;
	}

	if (bWrited)
		stRow += rowCnt;
}

void CSeisEvalReportCurvatureRC::WriteData(const T_SIBD_EVAL_ANAL_VALUE* analDeform, const T_SIBD_EVAL_RESULT_RC* result, const T_SIBD_EVAL_CALC_PHI_Y* calcMax, const T_SIBD_EVAL_CALC_PHI_Y* calcMin, const int& nRow, OUT BOOL& bWrited)
{
	auto l_WriteData = [this](const int nRow, const  int nCol,
		const vector<double> dResult)
		{
			WriteNanHyphenValue(nRow, nCol, dResult[0]);
			WriteNanHyphenValue(nRow + 1, nCol, dResult[1]);
			WriteNanHyphenValue(nRow + 2, nCol, dResult[2]);
			for (int i = 3; i < dResult.size(); i++)
			{
				int nColstart = i - 2; // nCol + 1 시작
				if (i == dResult.size() - 1)
				{
					CString strValue = dResult[i] > 1.0 ? _T("NG") : _T("OK");
					m_pXL->SetValue(nRow, nCol + nColstart, strValue);
					if (strValue == _T("NG")) SetNG_Red(m_pXL, nRow, nCol + nColstart, nRow, nCol + nColstart);
				}
				else
					WriteNanHyphenValue(nRow, nCol + nColstart, dResult[i]);
			}
		};

	vector<double> dMinResult, dMaxResult;
	dMinResult.resize(3, NAN); dMaxResult.resize(3, NAN);
	// Min
	const int& nMinThis = min(analDeform->aSmin.GetSize(), 3);
	for (int i = 0; i < nMinThis; i++) dMinResult[i] = (analDeform->aSmin.GetAt(i));
	dMinResult.push_back(analDeform->dAvergeMin);
	dMinResult.push_back(calcMin->dPhi_c);
	dMinResult.push_back(calcMin->dPhi_y);
	dMinResult.push_back(calcMin->dPhi_u);
	dMinResult.push_back(result->dRatioMin);
	l_WriteData(nRow, 4, dMinResult);

	// Max
	const int& nMaxThis = min(analDeform->aSmax.GetSize(), 3);
	for (int i = 0; i < nMaxThis; i++) dMaxResult[i] = (analDeform->aSmax.GetAt(i));
	dMaxResult.push_back(analDeform->dAvergeMax);
	dMaxResult.push_back(calcMax->dPhi_c);
	dMaxResult.push_back(calcMax->dPhi_y);
	dMaxResult.push_back(calcMax->dPhi_u);
	dMaxResult.push_back(result->dRatioMax);
	l_WriteData(nRow, 10, dMaxResult);

	bWrited = TRUE;
}

void CSeisEvalReportCurvatureRC::WriteData(const T_SIBD_EVAL_ANAL_VALUE* analDeform, const T_SIBD_EVAL_RESULT_RC* result, const T_SIBD_EVAL_CALC_PHI_A* calcMax, const T_SIBD_EVAL_CALC_PHI_A* calcMin, const int& nRow, OUT BOOL& bWrited)
{
	auto l_WriteData = [this](const int nRow, const  int nCol,
		const vector<double> dResult)
		{
			WriteNanHyphenValue(nRow, nCol, dResult[0]);
			WriteNanHyphenValue(nRow + 1, nCol, dResult[1]);
			WriteNanHyphenValue(nRow + 2, nCol, dResult[2]);
			for (int i = 3; i < dResult.size(); i++)
			{
				int nColstart = i - 2; // nCol + 1 시작
				if (i == dResult.size() - 1)
				{
					CString strValue = dResult[i] > 1.0 ? _T("NG") : _T("OK");
					m_pXL->SetValue(nRow, nCol + nColstart, strValue);
					if (strValue == _T("NG")) SetNG_Red(m_pXL, nRow, nCol + nColstart, nRow, nCol + nColstart);
				}
				else
				WriteNanHyphenValue(nRow, nCol + nColstart, dResult[i]);
			}
		};

	vector<double> dMinResult, dMaxResult;
	dMinResult.resize(3, NAN); dMaxResult.resize(3, NAN);
	// Min
	const int& nMinThis = min(analDeform->aSmin.GetSize(), 3);
	for (int i = 0; i < nMinThis; i++) dMinResult[i] = (analDeform->aSmin.GetAt(i));
	dMinResult.push_back(analDeform->dAvergeMin);
	dMinResult.push_back(calcMin->dPhi_y);
	dMinResult.push_back(calcMin->dPhi_u);
	dMinResult.push_back(calcMin->dPhi_a);
	dMinResult.push_back(result->dRatioMin);
	l_WriteData(nRow, 4, dMinResult);

	// Max
	const int& nMaxThis = min(analDeform->aSmax.GetSize(), 3);
	for (int i = 0; i < nMaxThis; i++) dMaxResult[i] = (analDeform->aSmax.GetAt(i));
	dMaxResult.push_back(analDeform->dAvergeMax);
	dMaxResult.push_back(calcMax->dPhi_y);
	dMaxResult.push_back(calcMax->dPhi_u);
	dMaxResult.push_back(calcMax->dPhi_a);
	dMaxResult.push_back(result->dRatioMax);
	l_WriteData(nRow, 10, dMaxResult);

	bWrited = TRUE;
}

void CSeisEvalReportCurvatureRC::GetInputValue(OUT int& nFormula, OUT double& dAlpha)
{
	CBrdgSeisEvalResult_JP* pResult = m_pDoc->m_pPostCtrl->GetBrdgSeisEvalResult_JP(); if (!pResult) { ASSERT(0); return; }

	const std::map<SIBD_EVAL_KEY, T_SIBD_EVAL_PHI_A>& mSidpResultA = pResult->GetResultSibdPhiAMap();
	std::map<SIBD_EVAL_KEY, T_SIBD_EVAL_PHI_A>::const_iterator iter;
	for (iter = mSidpResultA.begin(); iter != mSidpResultA.end(); ++iter)
	{
		const SIBD_EVAL_KEY& key = iter->first;
		const T_SIBD_EVAL_PHI_A& data = iter->second;

		T_SIPA_D SipaD;
		if (!m_pDoc->m_pAttrCtrl2->GetQSipa()->Get(data.kSipa, SipaD)) continue;
		if (SipaD.nDesignCode != m_nDesignCode) continue;

		T_SIBD_D sibd;
		if (!m_pDoc->m_pAttrCtrl2->GetQSibd()->Get(data.kSibd, sibd)) continue;
		if (sibd.nSibdType == T_SIBD_D::kStrengthSurvey) continue;

		nFormula = sibd.CVSV_D.nAllowCurvFormula;
		dAlpha = sibd.CVSV_D.dLimitStateAlpha;
		return;
	}

	const std::map<SIBD_EVAL_KEY, T_SIBD_EVAL_PHI_Y>& mSidpResultY = pResult->GetResultSibdPhiYMap();
	std::map<SIBD_EVAL_KEY, T_SIBD_EVAL_PHI_Y>::const_iterator iter2;
	for (iter2 = mSidpResultY.begin(); iter2 != mSidpResultY.end(); ++iter2)
	{
		const SIBD_EVAL_KEY& key = iter2->first;
		const T_SIBD_EVAL_PHI_Y& data = iter2->second;

		T_SIPA_D SipaD;
		if (!m_pDoc->m_pAttrCtrl2->GetQSipa()->Get(data.kSipa, SipaD)) continue;
		if (SipaD.nDesignCode != m_nDesignCode) continue;

		T_SIBD_D sibd;
		if (!m_pDoc->m_pAttrCtrl2->GetQSibd()->Get(data.kSibd, sibd)) continue;
		if (sibd.nSibdType == T_SIBD_D::kStrengthSurvey) continue;

		nFormula = sibd.CVSV_D.nAllowCurvFormula;
		dAlpha = sibd.CVSV_D.dLimitStateAlpha;
		return;
	}
}
#pragma endregion

#pragma region // CSeisEvalReportCurvatureST
CSeisEvalReportCurvatureST::CSeisEvalReportCurvatureST(IExcel* pXL, CDgnProgressDlg* pProgress, const CString& strBasePath, const CString& strBaseBook, const SDgnExcelPrintOpt* pPrintOpt)
	: CWriteSeisEvalReportBase(pXL, pProgress, strBasePath, strBaseBook, pPrintOpt)
{
}

CSeisEvalReportCurvatureST::~CSeisEvalReportCurvatureST()
{
}

void CSeisEvalReportCurvatureST::AddStep()
{
	T_KEY_LIST aSibdK;
	m_pDoc->m_pAttrCtrl2->GetQSibd()->GetKeyList(aSibdK);
	if (aSibdK.GetSize() < 1) return;

	std::vector<T_SIBD_K> vExistSibd[2][2][2][2];
	for (int i = 0; i < aSibdK.GetSize(); ++i)
	{
		const T_SIBD_K SibdK = aSibdK[i];
		T_SIBD_D SibdD;
		if (!m_pDoc->m_pAttrCtrl2->GetQSibd()->Get(SibdK, SibdD))		continue;
		if (SibdD.nSibdType != T_SIBD_D::kCurvatureSurvey) continue;

		T_SIGR_D SigrD;
		if (!m_pDoc->m_pAttrCtrl2->GetQSigr()->Get(SibdD.kSigr, SigrD)) continue;
		if (SigrD.nSubType != this->m_subType)								continue;

		T_SIPA_D SipaD;
		if (!m_pDoc->m_pAttrCtrl2->GetQSipa()->Get(SibdD.kSipa, SipaD)) continue;
		if (SipaD.nDesignCode != m_nDesignCode)							continue;

		const auto idx0 = (int)SibdD.CVSV_D.nCvsvType - 1; // 0: 항복조사, 1: 한계상태조사
		const auto iIdx1 = (int)SipaD.nSeismicForceDirection - 1;
		const auto iIdx2 = (int)SipaD.nSeismicForceType - 1;
		if (this->m_subType == T_SIGR_D::kUpper || this->m_subType == T_SIGR_D::kUser)
			vExistSibd[idx0][iIdx1][iIdx2][0].push_back(SibdK);
		else
		{
			T_SIGR_LOWER_D SigrLowerD;
			if (!m_pDoc->m_pAttrCtrl2->GetQSigrLower()->Get(SibdD.kSigr, SigrLowerD)) continue;
			if (SigrLowerD.nMaterialType != T_SIGR_LOWER_D::kSteel) continue;

			if (SigrLowerD.BEAM_D.GetSize() > 0)
				vExistSibd[idx0][iIdx1][iIdx2][0].push_back(SibdK);
			if (SigrLowerD.COLM_D.GetSize() > 0)
				vExistSibd[idx0][iIdx1][iIdx2][1].push_back(SibdK);
		}
	}

	for (int iCond = 0; iCond < 2; ++iCond)
	{
		if (vExistSibd[iCond][0][0][0].size() == 0 && vExistSibd[iCond][0][1][0].size() == 0 &&
			vExistSibd[iCond][1][0][0].size() == 0 && vExistSibd[iCond][1][1][0].size() == 0 &&
			vExistSibd[iCond][0][0][1].size() == 0 && vExistSibd[iCond][0][1][1].size() == 0 &&
			vExistSibd[iCond][1][0][1].size() == 0 && vExistSibd[iCond][1][1][1].size() == 0) continue;

		const int nSurveyCondition = iCond == 0 ? T_SIBD_CVSV_D::kSurrenderSurvey : T_SIBD_CVSV_D::kLimitStateSurvey;

		for (int iSeisDir = 0; iSeisDir < 2; ++iSeisDir) // 橋軸方向, 橋軸直角方向
		{
			if (vExistSibd[iCond][iSeisDir][0][0].size() == 0 && vExistSibd[iCond][iSeisDir][1][0].size() == 0 &&
				vExistSibd[iCond][iSeisDir][0][1].size() == 0 && vExistSibd[iCond][iSeisDir][1][1].size() == 0) continue;

			for (int iType = 0; iType < 2; ++iType) // タイプ type
			{
				if (vExistSibd[iCond][iSeisDir][iType][0].size() == 0 && vExistSibd[iCond][iSeisDir][iType][1].size() == 0) continue;

				for (int iPos = 0; iPos < 2; ++iPos)
				{
					const auto nMembType = iPos == 0 ? D_MBTP_BEAM : D_MBTP_COLUMN;
					if (vExistSibd[iCond][iSeisDir][iType][iPos].size() == 0) continue;

					for (const auto& SibdK : vExistSibd[iCond][iSeisDir][iType][iPos])
					{
						T_SIBD_D SibdD;
						if (!m_pDoc->m_pAttrCtrl2->GetQSibd()->Get(SibdK, SibdD))		continue;

						T_SIGR_D SigrD;
						if (!m_pDoc->m_pAttrCtrl2->GetQSigr()->Get(SibdD.kSigr, SigrD)) continue;
						if (SigrD.nSubType != this->m_subType) continue;

						T_ELEM_K_LIST aElemKeys; std::map <T_ELEM_K, std::tuple<CString, int>> mElemInfo;
						GetTargetElements(SibdD.kSigr, this->m_subType, T_SIGR_LOWER_D::kSteel, nMembType, aElemKeys, mElemInfo);
						if (aElemKeys.GetCount() < 1) continue;

						const int nMemb = aElemKeys.GetSize();
						for (int k = 0; k < nMemb; ++k)
						{
							const T_ELEM_K& ElemK = aElemKeys[k];
							if (mElemInfo.find(ElemK) == mElemInfo.end()) continue;

							//const CString& sGroup = std::get<0>(mElemInfo[ElemK]);
							//const int nShearDir = std::get<1>(mElemInfo[ElemK]);

							for(int d=0; d<2; ++d) // 교축방향, 교축직각방향
							{
								int nShearDir = (d == 0) ? seis_jp::kY_Dir : seis_jp::kZ_Dir;
								if (nSurveyCondition == T_SIBD_CVSV_D::kSurrenderSurvey)
								{
									T_SIBD_EVAL_PHI_Y eval;
									eval.kSipa = SibdD.kSipa;
									eval.kSigr = SibdD.kSigr;
									eval.kElem = ElemK;
									eval.nExamShearDir = nShearDir;
									if (!m_pResult->GetResultSibdPhiY(eval))  continue;
									m_pProgress->m_nTotalSteps++;
								}
								else
								{
									T_SIBD_EVAL_PHI_A eval;
									eval.kSipa = SibdD.kSipa;
									eval.kSigr = SibdD.kSigr;
									eval.kElem = ElemK;
									eval.nExamShearDir = nShearDir;
									if (!m_pResult->GetResultSibdPhiA(eval))  continue;
									m_pProgress->m_nTotalSteps++;
								}
							}
						}
					}
				}
			}
		}
	}
}

void CSeisEvalReportCurvatureST::AddSheet(const T_SIGR_D::eSubType& nGroupType, CString& strSheetName)
{
	m_strBaseSheet = _LSX(03.Curvature_Stl);
	strSheetName = _LS(IDS_BRDG_SEIS_EVAL_REPORT_JP_CURVATURE_STL) + _T("_") + GetTextGroup(nGroupType);// _T("曲率(鋼製橋脚)");
	if (m_pXL->IsExistSheetName(strSheetName))
	{
		ASSERT(0);
		m_pXL->SetActiveSheet(strSheetName);
	}
	else
	{
		m_pXL->AddSheet2(strSheetName);
		m_pXL->SetActiveSheet(strSheetName);
	}

	m_pXL->SetStandardFont(_T("Meiryo UI"));
	m_pXL->SetMargin(13, 12, 20, 17);
	m_pXL->SetColumnWidth(0, 0, 0, 60, 5.75); // 6.9 * 0.833333333
	m_pXL->SetRowHeight(16.5);
	m_pXL->SetZoom(70);
}

void CSeisEvalReportCurvatureST::WirteTheory(const T_SIGR_D::eSubType& nGroupType, const int& nSurveyType, const int iParagraph1, int& iParagraph2, OUT int& stRow)
{
	CBrdgSeisEvalResult_JP* pResult = m_pDoc->m_pPostCtrl->GetBrdgSeisEvalResult_JP(); if (!pResult) { ASSERT(0); return; }

	// Stl部材に対する降伏照査 
	CString RptBlock = (nSurveyType == T_SIBD_CVSV_D::kSurrenderSurvey) ? _T("_03.Curvature_Stl_Thery1") : _T("_03.Curvature_Stl_Thery2");
	int rowCnt = CopyRange(m_pXL, RptBlock, stRow);
	m_pXL->SetValue(stRow, 0, GetText(_T("%d. %s"), iParagraph1, GetTextTitle(nGroupType)));
	m_pXL->SetValue(stRow + 1, 0, GetText(_T("%d-%d. %s"), iParagraph1, ++iParagraph2, _LS(IDS_BRDG_SEIS_EVAL_REPORT_JP_CHK_FLEX)));// 曲げの照査

	stRow += rowCnt;
	stRow++;
}

BOOL CSeisEvalReportCurvatureST::IsExistData(std::map<std::tuple<int, int, int, int>, std::vector<T_SIBD_K>>& mExistSibd)
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
		if (SigrD.nSubType != this->m_subType)								continue;

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
			if (this->m_subType == T_SIGR_D::kUpper || this->m_subType == T_SIGR_D::kUser)
				mExistSibd[std::make_tuple(nSurveyCondition, nSeisDir, nSeisForce, D_MBTP_BEAM)].push_back(SibdK);
		}
	}

	if (mExistSibd.size() < 1) return FALSE;
	else return TRUE;
}

void CSeisEvalReportCurvatureST::Write()
{
//	3. 下部構造の3降伏照査 하부구조항복조사
//		3-1. 曲げの照査 곡률조사
//			3-1-1. 橋軸方向 교축방향
//				1) タイプⅠ
//					(1) 柱部の結果
//					(2) 梁部の結果
//				2) タイプⅡ
//					(1) 柱部の結果
//					(2) 梁部の結果
//			3-1-2. 橋軸直角方向 교축직각방향
//				1) タイプⅠ
//					(1) 柱部の結果
//					(2) 梁部の結果
//				2) タイプⅡ
//					(1) 柱部の結果
//					(2) 梁部の結果
	std::map<std::tuple<int, int, int, int>, std::vector<T_SIBD_K>> mExistSibd;
	if (!IsExistData(mExistSibd)) return;

	CString RptBlock;
	int rowCnt = 0;
	int stRow = 0;

	CString strSheetName;
	AddSheet(this->m_subType, strSheetName);
	*this->m_pParagraph1 = *this->m_pParagraph1 + 1;

	BOOL bWrited = FALSE;
	int iParagraph3 = 0;
	int iParagraph4 = 0;
	int iParagraph5 = 0;

	int nSurveyCondition = T_SIBD_CVSV_D::kUnKnown; // 1: 항복조사, 2: 한계상태조사
	int nSeisDir = T_SIPA_D::kUnKnown; // 1: 교축방향, 2: 교축직각방향
	int nType = T_SIPA_D::kUnKnown; // 1: 타입Ⅰ, 2: 타입Ⅱ
	int nMember = D_MBTP_ETC; // 1: 梁部, 2: 柱部
	for (const auto& pair : mExistSibd)
	{
		const auto& key = pair.first;
		const auto& vec = pair.second;
		if (vec.size() == 0) continue;

		const int iCond = std::get<0>(key);
		const int iSeisDir = std::get<1>(key);
		const int iType = std::get<2>(key);
		const int iMember = std::get<3>(key);
		if (nSurveyCondition != iCond)
		{
			WirteTheory(this->m_subType, iCond, *this->m_pParagraph1, *this->m_pParagraph2, stRow); // 이론
			nSurveyCondition = iCond;
		}

		if (nSeisDir != iSeisDir)
		{
			RptBlock = _T("_03.Curvature_Stl_Dir");
			WriteSeisDir(RptBlock, iSeisDir, iParagraph3, stRow);
			nSeisDir = iSeisDir;
			nType = T_SIPA_D::kUnKnown; // 초기화
		}

		if (nType != iType)
		{
			RptBlock = _T("_03.Curvature_Stl_Type");
			WriteSeisType(RptBlock, iType, iParagraph4, stRow);
			nType = iType;
			nMember = D_MBTP_ETC; // 초기화
		}

		if (nMember != iMember)
		{
			RptBlock = (iCond == T_SIBD_CVSV_D::kSurrenderSurvey) ? _T("_03.Curvature_Stl_Head1") : _T("_03.Curvature_Stl_Head2");
			WriteSeisPos(RptBlock, iMember, iParagraph5, stRow);
			nMember = iMember;
		}

		WriteCommon(vec, iMember, iCond, stRow, bWrited);
		stRow++;
	}

	if (bWrited == FALSE && m_pXL->IsExistSheetName(strSheetName))
		m_pXL->DeleteSheet(strSheetName);
}

void CSeisEvalReportCurvatureST::WriteCommon(const std::vector<T_SIBD_K>& vec, const int& iMember, const int& iCond, int& stRow, BOOL& bWrited)
{
	for (const auto& SibdK : vec)
	{
		T_SIBD_D SibdD;
		if (!m_pDoc->m_pAttrCtrl2->GetQSibd()->Get(SibdK, SibdD))		continue;

		T_SIGR_D SigrD;
		if (!m_pDoc->m_pAttrCtrl2->GetQSigr()->Get(SibdD.kSigr, SigrD)) continue;
		if (SigrD.nSubType != this->m_subType) continue;

		T_ELEM_K_LIST aElemKeys; std::map <T_ELEM_K, std::tuple<CString, int>> mElemInfo;
		GetTargetElements(SibdD.kSigr, this->m_subType, T_SIGR_LOWER_D::kSteel, iMember, aElemKeys, mElemInfo);
		if (aElemKeys.GetCount() < 1) continue;

		const int stRowGrp = stRow;
		const int nMemb = aElemKeys.GetSize();
		for (int k = 0; k < nMemb; ++k)
		{
			const T_ELEM_K& ElemK = aElemKeys[k];
			if (mElemInfo.find(ElemK) == mElemInfo.end()) continue;

			//const CString& sGroup = std::get<0>(mElemInfo[ElemK]);
			const int nShearDir = std::get<1>(mElemInfo[ElemK]);
			const CString strGroupName = (mElemInfo.find(ElemK) == mElemInfo.end()) ? _T("--") : std::get<0>(mElemInfo[ElemK]);

			std::tuple<T_SIPA_K, T_SIGR_K, T_ELEM_K, CString, CString> tParam; // 내진 방향 포함
			tParam = std::make_tuple(SibdD.kSipa, SibdD.kSigr, ElemK, SigrD.strName, strGroupName);

			if (nShearDir == seis_jp::kYZ_Dir) {
				WriteAxisTwo(iCond, tParam, stRow, bWrited);
			}
			else {
				WriteAxisOne(iCond, tParam, stRow, bWrited);
			}
		}
		auto* pFormat = m_pXL->GetSheet()->cellFormat(stRow - 1, 0);
		m_pXL->GetSheet()->setCellFormat(stRowGrp, 0, pFormat);
		m_pXL->SetMergeCell(stRowGrp, 0, stRow + -1, 0);
		m_pXL->SetValue(stRowGrp, 0, SigrD.strName);
	}
}

void CSeisEvalReportCurvatureST::WriteAxisOne(IN const int& nSurveyCondition, IN std::tuple<T_SIPA_K, T_SIGR_K, T_ELEM_K, CString, CString>& tParam, OUT int& stRow, OUT BOOL& bWrited)
{
	int rowCnt = 0;
	CString RptBlock;
	for (int iExamShearDir = 0; iExamShearDir < 2; iExamShearDir++) {
		const int sDir = iExamShearDir == 0 ? seis_jp::kY_Dir : seis_jp::kZ_Dir;

		if (nSurveyCondition == T_SIBD_CVSV_D::kSurrenderSurvey)
		{
			T_SIBD_EVAL_PHI_Y_ST eval;
			eval.kSipa = std::get<0>(tParam);
			eval.kSigr = std::get<1>(tParam);
			eval.kElem = std::get<2>(tParam);
			eval.nExamShearDir = sDir;
			if (!m_pResult->GetResultSibdPhiY_ST(eval))  continue;

			RptBlock = _T("_03.Curvature_Stl_Body");
			rowCnt = CopyRange(m_pXL, RptBlock, stRow);
			m_pXL->SetValue(stRow, 0, std::get<3>(tParam));
			m_pXL->SetValue(stRow, 1, std::get<4>(tParam));
			m_pXL->SetValue(stRow, 2, eval.kElem);

			CString strCheckDir;
			WriteReportDir(m_nSeisDir, iExamShearDir + 1, eType::kFlexure_Phi, strCheckDir);
			m_pXL->SetValue(stRow, 3, strCheckDir);

			WriteData(&eval, stRow, bWrited);
		}
		else
		{
			T_SIBD_EVAL_PHI_A_ST eval;
			eval.kSipa = std::get<0>(tParam);
			eval.kSigr = std::get<1>(tParam);
			eval.kElem = std::get<2>(tParam);
			eval.nExamShearDir = sDir;
			if (!m_pResult->GetResultSibdPhiA_ST(eval))  continue;

			RptBlock = _T("_03.Curvature_Stl_Body");
			rowCnt = CopyRange(m_pXL, RptBlock, stRow);
			m_pXL->SetValue(stRow, 0, std::get<3>(tParam));
			m_pXL->SetValue(stRow, 1, std::get<4>(tParam));
			m_pXL->SetValue(stRow, 2, eval.kElem);

			CString strCheckDir;
			WriteReportDir(m_nSeisDir, iExamShearDir + 1, eType::kFlexure_Phi, strCheckDir);
			m_pXL->SetValue(stRow, 3, strCheckDir);

			WriteData(&eval, stRow, bWrited);
		}

		stRow += rowCnt;
		if (m_pProgress->NextStep() == FALSE) throw FALSE;
	}
}

void CSeisEvalReportCurvatureST::WriteAxisTwo(IN const int& nSurveyCondition, IN std::tuple<T_SIPA_K, T_SIGR_K, T_ELEM_K, CString, CString>& tParam, OUT int& stRow, OUT BOOL& bWrited)
{
	int rowCnt = 0;
	CString RptBlock;
	BOOL bWriteTable = FALSE;
	for (int iExamShearDir = 0; iExamShearDir < 2; iExamShearDir++) {
		const int sDir = iExamShearDir == 0 ? seis_jp::kY_Dir : seis_jp::kZ_Dir;

		if (nSurveyCondition == T_SIBD_CVSV_D::kSurrenderSurvey)
		{
			T_SIBD_EVAL_PHI_Y_ST eval;
			eval.kSipa = std::get<0>(tParam);
			eval.kSigr = std::get<1>(tParam);
			eval.kElem = std::get<2>(tParam);
			eval.nExamShearDir = sDir;
			if (!m_pResult->GetResultSibdPhiY_ST(eval))  continue;

			if (bWriteTable == FALSE) {
				bWriteTable = TRUE;
				RptBlock = _T("_03.Curvature_Stl_Body2");
				rowCnt = CopyRange(m_pXL, RptBlock, stRow);

				m_pXL->SetValue(stRow, 0, std::get<3>(tParam));
				m_pXL->SetValue(stRow, 1, std::get<4>(tParam));
				m_pXL->SetValue(stRow, 2, eval.kElem);
			}

			int nRow = stRow;
			if (eval.nExamShearDir == seis_jp::kY_Dir)
				nRow += 1;
			WriteData(&eval, nRow, bWrited);
		}
		else
		{
			T_SIBD_EVAL_PHI_A_ST eval;
			eval.kSipa = std::get<0>(tParam);
			eval.kSigr = std::get<1>(tParam);
			eval.kElem = std::get<2>(tParam);
			eval.nExamShearDir = sDir;
			if (!m_pResult->GetResultSibdPhiA_ST(eval))  continue;

			if (bWriteTable == FALSE) {
				bWriteTable = TRUE;
				RptBlock = _T("_03.Curvature_Stl_Body2");
				rowCnt = CopyRange(m_pXL, RptBlock, stRow);

				m_pXL->SetValue(stRow, 0, std::get<3>(tParam));
				m_pXL->SetValue(stRow, 1, std::get<4>(tParam));
				m_pXL->SetValue(stRow, 2, eval.kElem);
			}

			int nRow = stRow;
			if (eval.nExamShearDir == seis_jp::kY_Dir)
				nRow += 1;
			WriteData(&eval, nRow, bWrited);
		}
		if (m_pProgress->NextStep() == FALSE) throw FALSE;
	}

	if (bWrited)
		stRow += rowCnt;
}

void CSeisEvalReportCurvatureST::WriteData(const T_SIBD_EVAL_PHI_Y_ST* eval, const int& nRow, OUT BOOL& bWrited)
{
	auto l_WriteData = [this](const int nRow, const  int nCol,
		const vector<double> dResult)
		{
			for (int i = 0; i < dResult.size(); i++)
			{
				if(i == 7 || i == 8) // dRatioYc, dRatioYt
					WriteNanHyphenValue(nRow, nCol + i + 1, dResult[i]);
				else
					WriteNanHyphenValue(nRow, nCol + i, dResult[i]);
			}
		};

	vector<double> dResult;
	dResult.resize(3, NAN);
	const int& nThis = min(eval->aAnalDeform.GetSize(), 3);
	for (int i = 0; i < nThis; i++) dResult[i] = eval->aAnalDeform.GetAt(i);
	const double dAverage = eval->dAverge;
	const double dPhi_u = eval->calcDeform.dPhi_u;
	const double dRatioYc = eval->resultSt.dRatioYc;
	const double dRatioYt = eval->resultSt.dRatioYt;
	dResult.push_back(dAverage);
	dResult.push_back(eval->calcDeform.dPhi_c);
	dResult.push_back(eval->calcDeform.dPhi_y);
	dResult.push_back(dPhi_u);
	dResult.push_back(dRatioYc);
	dResult.push_back(dRatioYt);
	l_WriteData(nRow, 4, dResult);

	if (isnan(dRatioYc) == 0 && isnan(dRatioYt) == 0)
		max(dRatioYc, dRatioYt) > 1.0 ? m_pXL->SetValue(nRow, 11, _LS(IDS_BRDG_SEIS_EVAL_REPORT_JP_YIELD)/*_T("降伏")*/) : SetValue_Hyphen(nRow, 11);
	else
		SetValue_Hyphen(nRow, 11);

	if (isnan(dAverage) != 0 || isnan(dPhi_u) != 0) {
		SetValue_Hyphen(nRow, 14);
	}
	else
	{
		const double dRatio = SafeDiv(dAverage, dPhi_u);
		if (dRatio > 1.0) {
			m_pXL->SetValue(nRow, 14, _T("NG"));
			SetNG_Red(m_pXL, nRow, 14, nRow, 14);
		}
		else {
			m_pXL->SetValue(nRow, 14, _T("OK"));
		}
	}

	bWrited = TRUE;
}

void CSeisEvalReportCurvatureST::WriteData(const T_SIBD_EVAL_PHI_A_ST* eval, const int& nRow, OUT BOOL& bWrited)
{
	auto l_WriteData = [this](const int nRow, const  int nCol,
		const vector<double> dResult)
		{
			for (int i = 0; i < dResult.size(); i++)
			{
				if (i == 7 || i == 8) // dRatioYc, dRatioYt
					WriteNanHyphenValue(nRow, nCol + i + 1, dResult[i]);
				else
					WriteNanHyphenValue(nRow, nCol + i, dResult[i]);
			}
		};

	vector<double> dResult;
	dResult.resize(3, NAN);
	const int& nThis = min(eval->aAnalDeform.GetSize(), 3);
	for (int i = 0; i < nThis; i++) dResult[i] = eval->aAnalDeform.GetAt(i);
	const double dAverage = eval->dAverge;
	const double dPhi_u = eval->calcDeform.dPhi_u;
	const double dRatioYc = eval->resultSt.dRatioYc;
	const double dRatioYt = eval->resultSt.dRatioYt;
	dResult.push_back(dAverage);
	dResult.push_back(eval->calcDeform.dPhi_c);
	dResult.push_back(eval->calcDeform.dPhi_y);
	dResult.push_back(dPhi_u);
	dResult.push_back(dRatioYc);
	dResult.push_back(dRatioYt);
	l_WriteData(nRow, 4, dResult);

	if (isnan(dRatioYc) == 0 && isnan(dRatioYt) == 0)
		max(dRatioYc, dRatioYt) > 1.0 ? m_pXL->SetValue(nRow, 11, _LS(IDS_BRDG_SEIS_EVAL_REPORT_JP_YIELD)/*_T("降伏")*/) : SetValue_Hyphen(nRow, 11);
	else
		SetValue_Hyphen(nRow, 11);

	if (isnan(dAverage) != 0 || isnan(dPhi_u) != 0) {
		SetValue_Hyphen(nRow, 14);
	}
	else
	{
		const double dRatio = SafeDiv(dAverage, dPhi_u);
		if (dRatio > 1.0) {
			m_pXL->SetValue(nRow, 14, _T("NG"));
			SetNG_Red(m_pXL, nRow, 14, nRow, 14);
		}
		else {
			m_pXL->SetValue(nRow, 14, _T("OK"));
		}
	}

	bWrited = TRUE;
}

#pragma endregion

#pragma region // CSeisEvalReportRebarReduction
CSeisEvalReportRebarReduction::CSeisEvalReportRebarReduction(IExcel* pXL, CDgnProgressDlg* pProgress, const CString& strBasePath, const CString& strBaseBook, const SDgnExcelPrintOpt* pPrintOpt)
	: CWriteSeisEvalReportBase(pXL, pProgress, strBasePath, strBaseBook, pPrintOpt)
{
	m_bWriteHeadSibd = FALSE;
	m_bWriteHeadSisr = FALSE;
}

CSeisEvalReportRebarReduction::~CSeisEvalReportRebarReduction()
{
}

void CSeisEvalReportRebarReduction::AddStep()
{
	CArray<T_SIGR_LOWER_K, T_SIGR_LOWER_K> aSigrLowerK;
	m_pDoc->m_pAttrCtrl2->GetQSigrLower()->GetKeyList(aSigrLowerK);
	const int nSisr = aSigrLowerK.GetSize();
	if (nSisr == 0) return;

	int iParagraph3 = 0;
	for (int i = 0; i < aSigrLowerK.GetCount(); i++)
	{
		const T_SIGR_LOWER_K& kSigrLower = aSigrLowerK[i];

		// N-N-1. 曲げ耐力の照査
		AddStepWriteRebarCutSibd(kSigrLower);

		// N-N-2. せん断耐力の照査
		AddStepWriteRebarCutSisr(kSigrLower);
	}
}

void CSeisEvalReportRebarReduction::AddStepWriteRebarCutSibd(const T_SIGR_LOWER_K& kSigrLower)
{
	T_SIGR_LOWER_D dLower; T_KEY_LIST aSipaKeys;
	if (!m_pDoc->m_pAttrCtrl2->GetQSigrLower()->Get(kSigrLower, dLower)) return;
	m_pDoc->m_pAttrCtrl2->GetQSipa()->GetKeyList(aSipaKeys);

	for (int iColm = 0; iColm < dLower.COLM_D.GetCount(); iColm++) {
		const T_SIGR_LOWER_COLM_D& colm = dLower.COLM_D[iColm];
		if (colm.bRebarCut == FALSE) continue;

		for (int i = 0; i < aSipaKeys.GetCount(); i++) {
			const T_SIPA_K& kSipa = aSipaKeys[i];

			T_SIPA_D SipaD;
			if (!m_pDoc->m_pAttrCtrl2->GetQSipa()->Get(kSipa, SipaD)) continue;
			if (SipaD.nDesignCode != m_nDesignCode) continue;

			// Write Sibd ...
		}
		m_pProgress->m_nTotalSteps++;
	}
}

void CSeisEvalReportRebarReduction::AddStepWriteRebarCutSisr(const T_SIGR_LOWER_K& kSigrLower)
{
	T_SIGR_LOWER_D dLower; T_KEY_LIST aSipaKeys;
	if (!m_pDoc->m_pAttrCtrl2->GetQSigrLower()->Get(kSigrLower, dLower)) return;
	m_pDoc->m_pAttrCtrl2->GetQSipa()->GetKeyList(aSipaKeys);

	for (int iColm = 0; iColm < dLower.COLM_D.GetCount(); iColm++) {
		const T_SIGR_LOWER_COLM_D& colm = dLower.COLM_D[iColm];
		if (colm.bRebarCut == FALSE) continue;

		for (int i = 0; i < aSipaKeys.GetCount(); i++) {
			const T_SIPA_K& kSipa = aSipaKeys[i];

			T_SIPA_D SipaD;
			if (!m_pDoc->m_pAttrCtrl2->GetQSipa()->Get(kSipa, SipaD)) continue;
			if (SipaD.nDesignCode != m_nDesignCode) continue;

			// Write Sisr ...
		}
		m_pProgress->m_nTotalSteps++;
	}
}

void CSeisEvalReportRebarReduction::Write()
{
//	3. 下部構造の3降伏照査 하부구조항복조사
//		3-5. 段落し部の検討(3波平均) 단락부의 검토
//			3-5-1.曲げ耐力の照査 휨내력조사
//			3-5-2.せん断耐力の照査 전단내력조사
	int stRow = 0;
	CString strSheetName;
	
	AddSheet(strSheetName);
	WirteTheory(*this->m_pParagraph1, *this->m_pParagraph2, stRow);

	BOOL bWritedSibd = FALSE, bWritedSisr = FALSE;

	CArray<T_SIGR_LOWER_K, T_SIGR_LOWER_K> aSigrLowerK;
	m_pDoc->m_pAttrCtrl2->GetQSigrLower()->GetKeyList(aSigrLowerK);
	const int nSisr = aSigrLowerK.GetSize();
	if (nSisr == 0) return;

	int iParagraph3 = 0;

	// 휨 -> 전단
	for (int iPos = 0; iPos < 2; ++iPos)
	{
		for (int i = 0; i < aSigrLowerK.GetCount(); i++)
		{
			const T_SIGR_LOWER_K& kSigrLower = aSigrLowerK[i];

			// N-N-1. 曲げ耐力の照査
			if (iPos == 0)
			{
				if (WriteRebarCutSibd(kSigrLower, *this->m_pParagraph1, *this->m_pParagraph2, stRow) == TRUE)
					bWritedSibd = TRUE;
			}
			// N-N-2. せん断耐力の照査
			else if (iPos == 1)
			{
				if (WriteRebarCutSisr(kSigrLower, *this->m_pParagraph1, *this->m_pParagraph2, stRow) == TRUE)
					bWritedSisr = TRUE;
			}
		}

		if ((iPos == 0 &&  bWritedSibd) || (iPos == 1 && bWritedSisr))
		{
			CString RptBlock = _T("_03.RebarReduction_Anno");
			int rowCnt = CopyRange(m_pXL, RptBlock, stRow);
			stRow += rowCnt;
			stRow++;
		}
	}

	if (bWritedSibd == FALSE && bWritedSisr == FALSE && m_pXL->IsExistSheetName(strSheetName))
		m_pXL->DeleteSheet(strSheetName);
}

void CSeisEvalReportRebarReduction::AddSheet(OUT CString& rSheetName)
{
	m_strBaseSheet = _LSX(03.RebarReduction);
	rSheetName = _LS(IDS_BRDG_SEIS_EVAL_REPORT_JP_REBAR_REDUCTION) + _T("_") + GetTextGroup(T_SIGR_D::kLower);// _T("段落し部");
	if (m_pXL->IsExistSheetName(rSheetName))
	{
		ASSERT(0);
		m_pXL->SetActiveSheet(rSheetName);
	}
	else
	{
		m_pXL->AddSheet2(rSheetName);
		m_pXL->SetActiveSheet(rSheetName);
	}

	m_pXL->SetStandardFont(_T("Meiryo UI"));
	m_pXL->SetMargin(13, 12, 20, 17);
	m_pXL->SetColumnWidth(0, 0, 0, 2, 6.9 * _ToPoint); //  6.9 * 0.833333333
	m_pXL->SetColumnWidth(0, 3, 0, 3, 9.9 * _ToPoint);
	m_pXL->SetColumnWidth(0, 4, 0, 60, 7.9 * _ToPoint);
	m_pXL->SetRowHeight(16.5);
	m_pXL->SetZoom(75);
}

void CSeisEvalReportRebarReduction::WirteTheory(const int iParagraph1, int& iParagraph2, OUT int& stRow)
{
	CString RptBlock = _T("_03.RebarReduction_Title");
	int rowCnt = CopyRange(m_pXL, RptBlock, stRow);
	m_pXL->SetValue(stRow, 0, GetText(_T("%d. %s"), iParagraph1, _LS(IDS_BRDG_SEIS_EVAL_REPORT_JP_LOWER_STRUCT))); // 下部構造の降伏照査
	m_pXL->SetValue(stRow + 1, 0, GetText(_T("%d-%d. %s"), iParagraph1, ++iParagraph2, _LS(IDS_BRDG_SEIS_EVAL_REPORT_JP_CHK_REBAR_REDUCTION)));//段落し部の検討(3波平均)
	stRow += rowCnt;
	stRow++;
}

BOOL CSeisEvalReportRebarReduction::WriteRebarCutSibd(const T_SIGR_LOWER_K& kSigrLower, const int& iParagraph1, const int& iParagraph2, 
	OUT int& stRow)
{
	CBrdgSeisEvalResult_JP* pResult = m_pDoc->m_pPostCtrl->GetBrdgSeisEvalResult_JP(); if (!pResult) { ASSERT(0); return FALSE; }

	T_SIGR_LOWER_D dLower; T_KEY_LIST aSipaKeys;
	if (!m_pDoc->m_pAttrCtrl2->GetQSigrLower()->Get(kSigrLower, dLower)) return FALSE;
	m_pDoc->m_pAttrCtrl2->GetQSipa()->GetKeyList(aSipaKeys);

	int nStartRow = stRow;
	int iParagraph3 = 0;
	
	for (int iColm = 0; iColm < dLower.COLM_D.GetCount(); iColm++) {
		const T_SIGR_LOWER_COLM_D& colm = dLower.COLM_D[iColm];
		if (colm.bRebarCut == FALSE) continue;

		int rowCnt = 0;
		BOOL bWriteTable = FALSE;
		for (int i = 0; i < aSipaKeys.GetCount(); i++) {
			const T_SIPA_K& kSipa = aSipaKeys[i];

			T_SIPA_D SipaD;
			if (!m_pDoc->m_pAttrCtrl2->GetQSipa()->Get(kSipa, SipaD)) continue;
			if (SipaD.nDesignCode != m_nDesignCode) continue;

			std::tuple<int, int, T_SIPA_K, T_SIGR_K, int> tParam;
			tParam = std::make_tuple(iParagraph1, iParagraph2, kSipa, kSigrLower, SipaD.nSeismicForceDirection);

			if (colm.nDirShear == seis_jp::kYZ_Dir) {
				rowCnt += WriteRebarCutSibdTwo(tParam, &colm, stRow, bWriteTable, iParagraph3);
			}
			else {
				rowCnt += WriteRebarCutSibdOne(tParam, &colm, stRow, bWriteTable, iParagraph3);
			}
		}
		stRow += rowCnt;
		if (m_pProgress->NextStep() == FALSE) throw FALSE;
	}

	return (nStartRow != stRow);
}

int CSeisEvalReportRebarReduction::WriteRebarCutSibdOne(const std::tuple<T_SIPA_K, T_SIGR_K, int, int, int>& tParam,
	const T_SIGR_LOWER_COLM_D* pColumn, OUT int& stRow, OUT BOOL& bWriteTable, OUT int& iParagraph3)
{
	CBrdgSeisEvalResult_JP* pResult = m_pDoc->m_pPostCtrl->GetBrdgSeisEvalResult_JP(); if (!pResult) { ASSERT(0); return 0; }

	auto l_WriteData = [&](const T_SIBD_EVAL_REBAR_CUT& eval, int stRow)
	{
		int nRow = stRow;
		m_pXL->SetValue(nRow, 0, eval.strSigrName);
		m_pXL->SetValue(nRow, 1, eval.strColmName);
		m_pXL->SetValue(nRow, 2, eval.kElem);

		if (eval.nSeismicForceDirection == T_SIPA_D::kPerp_Dir) nRow += 2;

		CString strCheckDir = eval.nExamShearDir == seis_jp::kZ_Dir ? _T("My") : _T("Mz");
		m_pXL->SetValue(nRow, 4, strCheckDir);
		m_pXL->SetMergeCell(nRow, 4, stRow+1, 4);

		//Max
		int nCol = 6;
		if (eval.nSeismicForceType == T_SIPA_D::kType2) nCol += 1;

		WriteNanHyphenValue(nRow, nCol, eval.analStrength.dAvergeMax);
		nCol += 2;
		WriteNanHyphenValue(nRow, nCol, eval.calcStrengthMax.dY);
		nCol += 2;
		WriteNanHyphenValue(nRow, nCol, eval.resultRc.dRatioMax);
		nCol += 2;
		if (eval.resultRc.dRatioMax > 1.0) {
			m_pXL->SetValue(nRow, nCol, _T("NG"));
			SetNG_Red(m_pXL, nRow, nCol, nRow, nCol);
		}
		else {
			m_pXL->SetValue(nRow, nCol, _T("OK"));
		}

		//Min
		nRow += 1;
		nCol = 6;
		if (eval.nSeismicForceType == T_SIPA_D::kType2) nCol += 1;

		WriteNanHyphenValue(nRow, nCol, eval.analStrength.dAvergeMin);
		nCol += 2;
		WriteNanHyphenValue(nRow, nCol, eval.calcStrengthMin.dY);
		nCol += 2;
		WriteNanHyphenValue(nRow, nCol, eval.resultRc.dRatioMin);
		nCol += 2;
		if (eval.resultRc.dRatioMin > 1.0) {
			m_pXL->SetValue(nRow, nCol, _T("NG"));
			SetNG_Red(m_pXL, nRow, nCol, nRow, nCol);
		}
		else {
			m_pXL->SetValue(nRow, nCol, _T("OK"));
		}
	};

	const int& iParagraph1 = std::get<0>(tParam);
	const int& iParagraph2 = std::get<1>(tParam);
	const int& nSeisDir = std::get<4>(tParam);
	const auto ElemK = (nSeisDir == T_SIPA_D::kAxis_Dir) ? pColumn->kAxialElem : pColumn->kAxisPerpElem;

	T_SIBD_EVAL_REBAR_CUT eval;
	eval.kSipa = std::get<2>(tParam);
	eval.kSigr = std::get<3>(tParam);
	eval.kElem = ElemK;
	eval.nExamShearDir = pColumn->nDirShear;
	if (!pResult->GetResultSibdRebarCut(eval)) return 0;

	if (m_bWriteHeadSibd == FALSE) {
		m_bWriteHeadSibd = TRUE; WriteRebarCutSibdHead(iParagraph1, iParagraph2, iParagraph3, stRow);
	}

	int rowCnt = 0;
	if (bWriteTable == FALSE) {
		bWriteTable = TRUE;
		CString RptBlock = _T("_03.RebarReduction_Sibd1");
		rowCnt = CopyRange(m_pXL, RptBlock, stRow);
	}

	l_WriteData(eval, stRow);

	return rowCnt;
}

int CSeisEvalReportRebarReduction::WriteRebarCutSibdTwo(const std::tuple<T_SIPA_K, T_SIGR_K, int, int, int>& tParam,
	const T_SIGR_LOWER_COLM_D* pColumn, OUT int& stRow, OUT BOOL& bWriteTable, OUT int& iParagraph3)
{
	CBrdgSeisEvalResult_JP* pResult = m_pDoc->m_pPostCtrl->GetBrdgSeisEvalResult_JP(); if (!pResult) { ASSERT(0); return 0; }

	auto l_WriteData = [&](const T_SIBD_EVAL_REBAR_CUT& eval, int stRow)
	{
		int nRow = stRow;
		m_pXL->SetValue(nRow, 0, eval.strSigrName);
		m_pXL->SetValue(nRow, 1, eval.strColmName);
		m_pXL->SetValue(nRow, 2, eval.kElem);

		if (eval.nSeismicForceDirection == T_SIPA_D::kPerp_Dir) nRow += 4;
		if (eval.nExamShearDir == seis_jp::kY_Dir) nRow += 2;

		//Max
		int nCol = 6;
		if (eval.nSeismicForceType == T_SIPA_D::kType2) nCol += 1;

		WriteNanHyphenValue(nRow, nCol, eval.analStrength.dAvergeMax);
		nCol += 2;
		WriteNanHyphenValue(nRow, nCol, eval.calcStrengthMax.dY);
		nCol += 2;
		WriteNanHyphenValue(nRow, nCol, eval.resultRc.dRatioMax);
		nCol += 2;
		if (eval.resultRc.dRatioMax > 1.0) {
			m_pXL->SetValue(nRow, nCol, _T("NG"));
			SetNG_Red(m_pXL, nRow, nCol, nRow, nCol);
		}
		else {
			m_pXL->SetValue(nRow, nCol, _T("OK"));
		}

		//Min
		nRow += 1;
		nCol = 6;
		if (eval.nSeismicForceType == T_SIPA_D::kType2) nCol += 1;

		WriteNanHyphenValue(nRow, nCol, eval.analStrength.dAvergeMin);
		nCol += 2;
		WriteNanHyphenValue(nRow, nCol, eval.calcStrengthMin.dY);
		nCol += 2;
		WriteNanHyphenValue(nRow, nCol, eval.resultRc.dRatioMin);
		nCol += 2;
		if (eval.resultRc.dRatioMin > 1.0) {
			m_pXL->SetValue(nRow, nCol, _T("NG"));
			SetNG_Red(m_pXL, nRow, nCol, nRow, nCol);
		}
		else {
			m_pXL->SetValue(nRow, nCol, _T("OK"));
		}
	};

	int rowCnt = 0;
	for (int iExamShearDir = 0; iExamShearDir < 2; iExamShearDir++) {
		const int sDir = iExamShearDir == 0 ? seis_jp::kY_Dir : seis_jp::kZ_Dir;

		const int& iParagraph1 = std::get<0>(tParam);
		const int& iParagraph2 = std::get<1>(tParam);
		const int& nSeisDir = std::get<4>(tParam);
		const auto ElemK = (nSeisDir == T_SIPA_D::kAxis_Dir) ? pColumn->kAxialElem : pColumn->kAxisPerpElem;

		T_SIBD_EVAL_REBAR_CUT eval;
		eval.kSipa = std::get<2>(tParam);
		eval.kSigr = std::get<3>(tParam);
		eval.kElem = ElemK;
		eval.nExamShearDir = sDir;
		if (!pResult->GetResultSibdRebarCut(eval)) continue;

		if (m_bWriteHeadSibd == FALSE) {
			m_bWriteHeadSibd = TRUE; WriteRebarCutSibdHead(iParagraph1, iParagraph2, iParagraph3, stRow);
		}

		if (bWriteTable == FALSE) {
			bWriteTable = TRUE;
			CString RptBlock = _T("_03.RebarReduction_Sibd2");
			rowCnt = CopyRange(m_pXL, RptBlock, stRow);
		}

		l_WriteData(eval, stRow);
	}

	return rowCnt;
}

void CSeisEvalReportRebarReduction::WriteRebarCutSibdHead(const int& iParagraph1, const int& iParagraph2, OUT int& iParagraph3, OUT int& stRow)
{
	CString RptBlock = _T("_03.RebarReduction_Theory1");
	int rowCnt = CopyRange(m_pXL, RptBlock, stRow);
	m_pXL->SetValue(stRow, 0, GetText(_T("%d-%d-%d. %s"), iParagraph1, iParagraph2, ++iParagraph3, _LS(IDS_BRDG_SEIS_EVAL_REPORT_JP_CHK_FLEX_STR))); // 曲げ耐力の照査
	stRow += rowCnt;
	stRow++;

	RptBlock = _T("_03.RebarReduction_Head1");
	rowCnt = CopyRange(m_pXL, RptBlock, stRow);
	stRow += rowCnt;
}

BOOL CSeisEvalReportRebarReduction::WriteRebarCutSisr(const T_SIGR_LOWER_K& kSigrLower, const int& iParagraph1, const int& iParagraph2, 
	OUT int& stRow)
{
	CBrdgSeisEvalResult_JP* pResult = m_pDoc->m_pPostCtrl->GetBrdgSeisEvalResult_JP(); if (!pResult) { ASSERT(0); return FALSE; }

	T_SIGR_LOWER_D dLower; T_KEY_LIST aSipaKeys;
	if (!m_pDoc->m_pAttrCtrl2->GetQSigrLower()->Get(kSigrLower, dLower)) return FALSE;
	m_pDoc->m_pAttrCtrl2->GetQSipa()->GetKeyList(aSipaKeys);

	int nStartRow = stRow;
	int iParagraph3 = 0;
	
	for (int iColm = 0; iColm < dLower.COLM_D.GetCount(); iColm++) {
		const T_SIGR_LOWER_COLM_D& colm = dLower.COLM_D[iColm];
		if (colm.bRebarCut == FALSE) continue;

		int rowCnt = 0;
		BOOL bWriteTable = FALSE;
		for (int i = 0; i < aSipaKeys.GetCount(); i++) {
			const T_SIPA_K& kSipa = aSipaKeys[i];

			T_SIPA_D SipaD;
			if (!m_pDoc->m_pAttrCtrl2->GetQSipa()->Get(kSipa, SipaD)) continue;
			if (SipaD.nDesignCode != m_nDesignCode) continue;

			std::tuple<int, int, T_SIPA_K, T_SIGR_K, int> tParam;
			tParam = std::make_tuple(iParagraph1, iParagraph2, kSipa, kSigrLower, SipaD.nSeismicForceDirection);

			if (colm.nDirShear == seis_jp::kYZ_Dir) {
				rowCnt += WriteRebarCutSisrTwo(tParam, &colm, stRow, bWriteTable, iParagraph3);
			}
			else {
				rowCnt += WriteRebarCutSisrOne(tParam, &colm, stRow, bWriteTable, iParagraph3);
			}
		}
		stRow += rowCnt;
		if (m_pProgress->NextStep() == FALSE) throw FALSE;
	}

	return (nStartRow != stRow);
}

int CSeisEvalReportRebarReduction::WriteRebarCutSisrOne(const std::tuple<T_SIPA_K, T_SIGR_K, int, int, int>& tParam,
	const T_SIGR_LOWER_COLM_D* pColumn, OUT int& stRow, OUT BOOL& bWriteTable, OUT int& iParagraph3)
{
	CBrdgSeisEvalResult_JP* pResult = m_pDoc->m_pPostCtrl->GetBrdgSeisEvalResult_JP(); if (!pResult) { ASSERT(0); return 0; }

	auto l_WriteData = [&](const T_SISR_EVAL_REBAR_CUT& eval, int stRow)
	{
		int nRow = stRow;
		m_pXL->SetValue(nRow, 0, eval.strSigrName);
		m_pXL->SetValue(nRow, 1, eval.strColmName);
		m_pXL->SetValue(nRow, 2, eval.kElem);

		if (eval.nSeismicForceDirection == T_SIPA_D::kPerp_Dir) nRow += 2;

		CString strCheckDir = eval.nExamShearDir == seis_jp::kZ_Dir ? _T("Fz") : _T("Fy");
		m_pXL->SetValue(nRow, 4, strCheckDir);
		//m_pXL->SetMergeCell(nRow, 4, stRow + 1, 4);

		//Max
		int nCol = 6;
		if (eval.nSeismicForceType == T_SIPA_D::kType2) nCol += 1;

		WriteNanHyphenValue(nRow, nCol, eval.RsltMax.dSmax);
		nCol += 2;
		WriteNanHyphenValue(nRow, nCol, eval.RsltMax.dPs);
		nCol += 2;
		WriteNanHyphenValue(nRow, nCol, eval.RsltMax.dRatio);
		nCol += 2;
		if (eval.RsltMax.dRatio > 1.0) {
			m_pXL->SetValue(nRow, nCol, _T("NG"));
			SetNG_Red(m_pXL, nRow, nCol, nRow, nCol);
		}
		else {
			m_pXL->SetValue(nRow, nCol, _T("OK"));
		}

		//Min
		nRow += 1;
		nCol = 6;
		if (eval.nSeismicForceType == T_SIPA_D::kType2) nCol += 1;

		WriteNanHyphenValue(nRow, nCol, eval.RsltMin.dSmax);
		nCol += 2;
		WriteNanHyphenValue(nRow, nCol, eval.RsltMin.dPs);
		nCol += 2;
		WriteNanHyphenValue(nRow, nCol, eval.RsltMin.dRatio);
		nCol += 2;
		if (eval.RsltMin.dRatio > 1.0) {
			m_pXL->SetValue(nRow, nCol, _T("NG"));
			SetNG_Red(m_pXL, nRow, nCol, nRow, nCol);
		}
		else {
			m_pXL->SetValue(nRow, nCol, _T("OK"));
		}
	};

	const int& iParagraph1 = std::get<0>(tParam);
	const int& iParagraph2 = std::get<1>(tParam);
	const int& nSeisDir = std::get<4>(tParam);
	const auto ElemK = (nSeisDir == T_SIPA_D::kAxis_Dir) ? pColumn->kAxialElem : pColumn->kAxisPerpElem;

	T_SISR_EVAL_REBAR_CUT eval;
	eval.kSipa = std::get<2>(tParam);
	eval.kSigr = std::get<3>(tParam);
	eval.kElem = ElemK;
	eval.nExamShearDir = pColumn->nDirShear;
	if (!pResult->GetResultSisrRebarCut(eval)) return 0;

	if (m_bWriteHeadSisr == FALSE) {
		m_bWriteHeadSisr = TRUE; WriteRebarCutSisrHead(iParagraph1, iParagraph2, iParagraph3, stRow);
	}

	int rowCnt = 0;
	if (bWriteTable == FALSE) {
		bWriteTable = TRUE;
		CString RptBlock = _T("_03.RebarReduction_Sisr1");
		rowCnt = CopyRange(m_pXL, RptBlock, stRow);
	}

	l_WriteData(eval, stRow);
	
	return rowCnt;
}

int CSeisEvalReportRebarReduction::WriteRebarCutSisrTwo(const std::tuple<T_SIPA_K, T_SIGR_K, int, int, int>& tParam,
	const T_SIGR_LOWER_COLM_D* pColumn, OUT int& stRow, OUT BOOL& bWriteTable, OUT int& iParagraph3)
{
	CBrdgSeisEvalResult_JP* pResult = m_pDoc->m_pPostCtrl->GetBrdgSeisEvalResult_JP(); if (!pResult) { ASSERT(0); return 0; }

	auto l_WriteData = [&](const T_SISR_EVAL_REBAR_CUT& eval, int stRow)
	{
		int nRow = stRow;

		//Max
		int nCol = 6;
		if (eval.nSeismicForceType == T_SIPA_D::kType2) nCol += 1;

		WriteNanHyphenValue(nRow, nCol, eval.RsltMax.dSmax);
		nCol += 2;
		WriteNanHyphenValue(nRow, nCol, eval.RsltMax.dPs);
		nCol += 2;
		WriteNanHyphenValue(nRow, nCol, eval.RsltMax.dRatio);
		nCol += 2;
		if (eval.RsltMax.dRatio > 1.0) {
			m_pXL->SetValue(nRow, nCol, _T("NG"));
			SetNG_Red(m_pXL, nRow, nCol, nRow, nCol);
		}
		else {
			m_pXL->SetValue(nRow, nCol, _T("OK"));
		}

		//Min
		nRow += 1;
		nCol = 6;
		if (eval.nSeismicForceType == T_SIPA_D::kType2) nCol += 1;

		WriteNanHyphenValue(nRow, nCol, eval.RsltMin.dSmax);
		nCol += 2;
		WriteNanHyphenValue(nRow, nCol, eval.RsltMin.dPs);
		nCol += 2;
		WriteNanHyphenValue(nRow, nCol, eval.RsltMin.dRatio);
		nCol += 2;
		if (eval.RsltMin.dRatio > 1.0) {
			m_pXL->SetValue(nRow, nCol, _T("NG"));
			SetNG_Red(m_pXL, nRow, nCol, nRow, nCol);
		}
		else {
			m_pXL->SetValue(nRow, nCol, _T("OK"));
		}
	};

	int rowCnt = 0;
	for (int iExamShearDir = 0; iExamShearDir < 2; iExamShearDir++) {
		const int sDir = iExamShearDir == 0 ? seis_jp::kY_Dir : seis_jp::kZ_Dir;

		const int& iParagraph1 = std::get<0>(tParam);
		const int& iParagraph2 = std::get<1>(tParam);
		const int& nSeisDir = std::get<4>(tParam);
		const auto ElemK = (nSeisDir == T_SIPA_D::kAxis_Dir) ? pColumn->kAxialElem : pColumn->kAxisPerpElem;

		T_SISR_EVAL_REBAR_CUT eval;
		eval.kSipa = std::get<2>(tParam);
		eval.kSigr = std::get<3>(tParam);
		eval.kElem = ElemK;
		eval.nExamShearDir = sDir;
		if (!pResult->GetResultSisrRebarCut(eval)) continue;

		if (m_bWriteHeadSisr == FALSE) {
			m_bWriteHeadSisr = TRUE; WriteRebarCutSisrHead(iParagraph1, iParagraph2, iParagraph3, stRow);
		}

		if (bWriteTable == FALSE) {
			bWriteTable = TRUE;
			CString RptBlock = _T("_03.RebarReduction_Sisr2");
			rowCnt = CopyRange(m_pXL, RptBlock, stRow);
		}
		
		m_pXL->SetValue(stRow, 0, eval.strSigrName);
		m_pXL->SetValue(stRow, 1, eval.strColmName);
		m_pXL->SetValue(stRow, 2, eval.kElem);

		int nRow = stRow;
		// 교축직각 방향일 경우 4줄넘김
		if (nSeisDir == T_SIPA_D::kPerp_Dir)
			nRow += 4;
		if (eval.nExamShearDir == seis_jp::kY_Dir)
			nRow += 2;

		l_WriteData(eval, nRow);
	}

	return rowCnt;
}

void CSeisEvalReportRebarReduction::WriteRebarCutSisrHead(const int& iParagraph1, const int& iParagraph2, OUT int& iParagraph3, OUT int& stRow)
{
	CString RptBlock = _T("_03.RebarReduction_Theory2");
	int rowCnt = CopyRange(m_pXL, RptBlock, stRow);
	m_pXL->SetValue(stRow, 0, GetText(_T("%d-%d-%d. %s"), iParagraph1, iParagraph2, ++iParagraph3, _LS(IDS_BRDG_SEIS_EVAL_REPORT_JP_CHK_SHEAR_STR)));// せん断耐力の照査
	stRow += rowCnt;
	stRow++;

	RptBlock = _T("_03.RebarReduction_Head2");
	rowCnt = CopyRange(m_pXL, RptBlock, stRow);
	stRow += rowCnt;
}

#pragma endregion

#pragma region // CSeisEvalReportPierBottom
CSeisEvalReportPierBottom::CSeisEvalReportPierBottom(IExcel* pXL, CDgnProgressDlg* pProgress, const CString& strBasePath, const CString& strBaseBook, const SDgnExcelPrintOpt* pPrintOpt)
	: CWriteSeisEvalReportBase(pXL, pProgress, strBasePath, strBaseBook, pPrintOpt)
{
}

CSeisEvalReportPierBottom::~CSeisEvalReportPierBottom()
{
}

void CSeisEvalReportPierBottom::AddStep()
{
	AddStepMaxMin();
	AddStepConcurEach();
	AddStepConcurByFooting();
}

void CSeisEvalReportPierBottom::AddStepMaxMin()
{
	CArray<T_SIET_K, T_SIET_K> aSietK;
	m_pDoc->m_pAttrCtrl2->GetQSiet()->GetKeyList(aSietK);
	const auto nSiet = aSietK.GetSize();
	if (nSiet == 0) return;

	std::vector<T_SIET_K> vExistSiet[2][2];
	for (int iSiet = 0; iSiet < nSiet; ++iSiet)
	{
		const T_SIET_K SietK = aSietK[iSiet];
		T_SIET_D SietD;
		if (!m_pDoc->m_pAttrCtrl2->GetQSiet()->Get(SietK, SietD))		continue;
		if (SietD.nOutTypeMember != T_SIET_D::kMinMax_Mb)				continue;
		T_SIPA_D SipaD;
		if (!m_pDoc->m_pAttrCtrl2->GetQSipa()->Get(SietD.kSipa, SipaD)) continue;
		if (SipaD.nDesignCode != m_nDesignCode)							continue;

		const auto iIdx1 = (int)SipaD.nSeismicForceDirection - 1;
		const auto iIdx2 = (int)SipaD.nSeismicForceType - 1;
		vExistSiet[iIdx1][iIdx2].push_back(SietK);
	}

	if (vExistSiet[0][0].size() > 0 || vExistSiet[0][1].size() > 0 ||
		vExistSiet[1][0].size() > 0 || vExistSiet[1][1].size() > 0)
	{
		for (int iSeisDir = 0; iSeisDir < 2; ++iSeisDir) // 橋軸方向, 橋軸直角方向
		{
			if (vExistSiet[iSeisDir][0].size() == 0 && vExistSiet[iSeisDir][1].size() == 0) continue;

			for (int iType = 0; iType < 2; ++iType) // タイプ
			{
				if (vExistSiet[iSeisDir][iType].size() == 0) continue;

				for (const auto& SietK : vExistSiet[iSeisDir][iType])
				{
					T_SIET_D SietD;
					if (!m_pDoc->m_pAttrCtrl2->GetQSiet()->Get(SietK, SietD)) continue;
					T_SIGR_D SigrD;
					if (!m_pDoc->m_pAttrCtrl2->GetQSigr()->Get(SietD.kSigrLower, SigrD)) continue;

					T_SIET_EVAL_MEMB_MINMAX eval;
					if (!m_pResult->GetResultSietMembMinMax(SietD.kSipa, SietD.kSigrLower, eval)) eval.Initialize();

					const auto nGroup = eval.aGroup.GetSize();
					for (int l = 0; l < nGroup; ++l)
					{
						m_pProgress->m_nTotalSteps++;
					}
				}
			}
		}
	}
}

void CSeisEvalReportPierBottom::AddStepConcurEach()
{
	CArray<T_SIET_K, T_SIET_K> aSietK;
	m_pDoc->m_pAttrCtrl2->GetQSiet()->GetKeyList(aSietK);
	const auto nSiet = aSietK.GetSize();
	if (nSiet == 0) return;

	std::vector<T_SIET_K> vExistSiet[2][2];
	for (int iSiet = 0; iSiet < nSiet; ++iSiet)
	{
		const T_SIET_K SietK = aSietK[iSiet];
		T_SIET_D SietD;
		if (!m_pDoc->m_pAttrCtrl2->GetQSiet()->Get(SietK, SietD))		continue;
		if (SietD.nOutTypeMember != T_SIET_D::kConcurvity_Each_Mb)		continue;
		T_SIPA_D SipaD;
		if (!m_pDoc->m_pAttrCtrl2->GetQSipa()->Get(SietD.kSipa, SipaD)) continue;
		if (SipaD.nDesignCode != m_nDesignCode)							continue;

		const auto iIdx1 = (int)SipaD.nSeismicForceDirection - 1;
		const auto iIdx2 = (int)SipaD.nSeismicForceType - 1;
		vExistSiet[iIdx1][iIdx2].push_back(SietK);
	}

	if (vExistSiet[0][0].size() > 0 || vExistSiet[0][1].size() > 0 ||
		vExistSiet[1][0].size() > 0 || vExistSiet[1][1].size() > 0)
	{
		for (int iSeisDir = 0; iSeisDir < 2; ++iSeisDir) // 橋軸方向, 橋軸直角方向
		{
			if (vExistSiet[iSeisDir][0].size() == 0 && vExistSiet[iSeisDir][1].size() == 0) continue;

			for (int iType = 0; iType < 2; ++iType) // タイプ
			{
				if (vExistSiet[iSeisDir][iType].size() == 0) continue;

				for (const auto& SietK : vExistSiet[iSeisDir][iType])
				{
					T_SIET_D SietD;
					if (!m_pDoc->m_pAttrCtrl2->GetQSiet()->Get(SietK, SietD)) continue;
					T_SIGR_D SigrD;
					if (!m_pDoc->m_pAttrCtrl2->GetQSigr()->Get(SietD.kSigrLower, SigrD)) continue;

					T_SIET_EVAL_MEMB_EACH eval;
					if (!m_pResult->GetResultSietMembConcEach(SietD.kSipa, SietD.kSigrLower, eval)) eval.Initialize();

					const auto nGroup = eval.aGroup.GetSize();
					for (int l = 0; l < nGroup; ++l)
					{
						m_pProgress->m_nTotalSteps++;
					}
				}
			}
		}
	}
}

void CSeisEvalReportPierBottom::AddStepConcurByFooting()
{
	CArray<T_SIET_K, T_SIET_K> aSietK;
	m_pDoc->m_pAttrCtrl2->GetQSiet()->GetKeyList(aSietK);
	const auto nSiet = aSietK.GetSize();
	if (nSiet == 0) return;

	std::vector<T_SIET_K> vExistSiet[2][2];
	for (int iSiet = 0; iSiet < nSiet; ++iSiet)
	{
		const T_SIET_K SietK = aSietK[iSiet];
		T_SIET_D SietD;
		if (!m_pDoc->m_pAttrCtrl2->GetQSiet()->Get(SietK, SietD))		continue;
		if (SietD.nOutTypeMember != T_SIET_D::kConcurvity_FootingLow)	continue;
		T_SIPA_D SipaD;
		if (!m_pDoc->m_pAttrCtrl2->GetQSipa()->Get(SietD.kSipa, SipaD)) continue;
		if (SipaD.nDesignCode != m_nDesignCode)							continue;

		const auto iIdx1 = (int)SipaD.nSeismicForceDirection - 1;
		const auto iIdx2 = (int)SipaD.nSeismicForceType - 1;
		vExistSiet[iIdx1][iIdx2].push_back(SietK);
	}

	if (vExistSiet[0][0].size() < 1 && vExistSiet[0][1].size() < 1 &&
		vExistSiet[1][0].size() < 1 && vExistSiet[1][1].size() < 1) return;
	
	for (int iSeisDir = 0; iSeisDir < 2; ++iSeisDir) // 橋軸方向, 橋軸直角方向
	{
		if (vExistSiet[iSeisDir][0].size() == 0 && vExistSiet[iSeisDir][1].size() == 0) continue;

		for (int iType = 0; iType < 2; ++iType) // タイプ
		{
			if (vExistSiet[iSeisDir][iType].size() == 0) continue;

			for (const auto& SietK : vExistSiet[iSeisDir][iType]) // SIET
			{
				T_SIET_D SietD;
				if (!m_pDoc->m_pAttrCtrl2->GetQSiet()->Get(SietK, SietD)) continue;
				T_SIGR_D SigrD;
				if (!m_pDoc->m_pAttrCtrl2->GetQSigr()->Get(SietD.kSigrLower, SigrD)) continue;
				T_SIGR_LOWER_D SigrLowerD;
				if (!m_pDoc->m_pAttrCtrl2->GetQSigrLower()->Get(SietD.kSigrLower, SigrLowerD)) continue;

				const auto nFoot = SigrLowerD.FOOT_D.GetSize();
				for (int k = 0; k < nFoot; ++k)	// Number of Foot
				{
					const T_SIGR_LOWER_FOOT_D& foot = SigrLowerD.FOOT_D[k];

					const auto nElem = foot.aInvestigatedElem.GetSize();
					for (int l = 0; l < nElem; ++l)	// // Number of element of Foot
					{
						const auto& ElemK = foot.aInvestigatedElem[l];
						T_SIET_EVAL_FOOT_MASTER eval;
						if (!m_pResult->GetResultSietConcFootingLow(SietD.kSipa, SietD.kSigrLower, ElemK, eval)) continue;

						for (int dof = 0; dof < 6; ++dof) // degree of freedoms
						{
							const auto& ComponentD = eval.FootConcu[dof];

							for (int mm = 0; mm < 2; ++mm) // max/min type
							{
								const auto& aConcuD = mm == 0 ? ComponentD.aMax : ComponentD.aMin;

								const int nCnt = aConcuD.GetSize();
								for (int gp = 0; gp < nCnt; ++gp) // Group
								{
									m_pProgress->m_nTotalSteps++;
								}
							}
						}
					}
				}
			}
		}
	}
}

void CSeisEvalReportPierBottom::Write()
{
//	4.橋脚下端の断面力(3波平均) 교각하단의 단면력(3파평균)
//		4-1.断面力成分別の最大・最小 단면력 성분별 최대/최소
//			4-1-1. 橋軸方向 교축방향
//				1) タイプⅠ
//				2) タイプⅡ
//			4-1-2. 橋軸直角方向 교축직각방향
//				1) タイプⅠ
//				2) タイプⅡ
//		4-2.断面力成分別の最大・最小時における同時性結果 단면력 성분별 최대·최소시에 있어서의 동시발생 결과
//			4-2-1. 橋軸方向 교축방향
//				1) タイプⅠ
//				2) タイプⅡ
//			4-2-2. 橋軸直角方向 교축직각방향
//				1) タイプⅠ
//				2) タイプⅡ
	CBrdgSeisEvalResult_JP* pResult = m_pDoc->m_pPostCtrl->GetBrdgSeisEvalResult_JP(); if (!pResult) { ASSERT(0); return; }

	CArray<T_SIET_K, T_SIET_K> aSietK;
	m_pDoc->m_pAttrCtrl2->GetQSiet()->GetKeyList(aSietK);
	const auto nSiet = aSietK.GetSize();
	if (nSiet == 0) return;

	std::vector<BOOL>bCheck = { FALSE, FALSE, FALSE };
	for (int i = 0; i < nSiet; ++i)
	{
		const T_SIET_K SietK = aSietK[i];
		T_SIET_D SietD;
		if (!m_pDoc->m_pAttrCtrl2->GetQSiet()->Get(SietK, SietD))		continue;
		if (SietD.bPierBase_Sf == FALSE && SietD.bFootingLower_Sf == FALSE) continue;
		T_SIPA_D SipaD;
		if (!m_pDoc->m_pAttrCtrl2->GetQSipa()->Get(SietD.kSipa, SipaD)) continue;
		if (SipaD.nDesignCode != m_nDesignCode)							continue;

		if (SietD.nOutTypeMember == T_SIET_D::kMinMax_Mb) bCheck[0] = TRUE;
		if (SietD.nOutTypeMember == T_SIET_D::kConcurvity_Each_Mb) bCheck[1] = TRUE;
		if (SietD.nOutTypeMember == T_SIET_D::kConcurvity_FootingLow) bCheck[2] = TRUE;
	}
	if (!bCheck[0] && !bCheck[1] && !bCheck[2]) return;

	size_t nMinMax_Mb = pResult->GetResultSietMembMinMaxMap().size();
	size_t nEach_Mb = pResult->GetResultSietMembConcEachMap().size();
	size_t nFootingLow = pResult->GetResultSietConcFootingLowMap().size();

	size_t nCount = 0;
	if (bCheck[0]) nCount += nMinMax_Mb;
	if (bCheck[1]) nCount += nEach_Mb;
	if (bCheck[2]) nCount += nFootingLow;

	if (nCount < 1)
		return;

	AddSheet();

	CString RptBlock;
	int rowCnt = 0;
	int stRow = 0;
	short Row_Ref = 0, Col_Ref = 0;

	*this->m_pParagraph1 = *this->m_pParagraph1 + 1;
	WirteTheory(*this->m_pParagraph1, stRow);

	int iParagraph2 = 0;
	WriteMaxMin(m_pXL, *this->m_pParagraph1, iParagraph2, stRow);
	WriteConcurEach(m_pXL, *this->m_pParagraph1, iParagraph2, stRow);
	WriteConcurByFooting(m_pXL, *this->m_pParagraph1, iParagraph2, stRow);
}

void CSeisEvalReportPierBottom::AddSheet()
{
	m_strBaseSheet = _LSX(04.Forces);
	const CString strNewSheet = _LS(IDS_BRDG_SEIS_EVAL_REPORT_JP_FORCES);// _T("下端断面力");
	if (m_pXL->IsExistSheetName(strNewSheet))
	{
		ASSERT(0);
		m_pXL->SetActiveSheet(strNewSheet);
	}
	else
	{
		m_pXL->AddSheet2(strNewSheet);
		m_pXL->SetActiveSheet(strNewSheet);
	}

	m_pXL->SetStandardFont(_T("Meiryo UI"));
	m_pXL->SetMargin(13, 12, 20, 17);
	m_pXL->SetColumnWidth(0, 0, 0, 4, 7.4 * _ToPoint); //  7.4 * 0.833333333
	m_pXL->SetColumnWidth(0, 5, 0, 5, 8.3 * _ToPoint);
	m_pXL->SetColumnWidth(0, 6, 0, 60, 9.4 * _ToPoint);
	m_pXL->SetRowHeight(16.5);
	m_pXL->SetZoom(75);
}

void CSeisEvalReportPierBottom::WirteTheory(const int iParagraph1, OUT int& stRow)
{
	CString RptBlock = _T("_04.Forces_Title");
	int rowCnt = CopyRange(m_pXL, RptBlock, stRow);
	m_pXL->SetValue(stRow, 0, GetText(_T("%d. %s"), iParagraph1, _LS(IDS_BRDG_SEIS_EVAL_REPORT_JP_PIER_FORCES))); // 橋脚下端の断面力(3波平均)
	stRow += rowCnt;
}

BOOL CSeisEvalReportPierBottom::WriteMaxMin(IExcel* pXL, const int iParagraph1, int& iParagraph2, int& stRow)
{
//	4.橋脚下端の断面力(3波平均) 교각하단의 단면력(3파평균)
//		4-1.断面力成分別の最大・最小 단면력 성분별 최대/최소
//			4-1-1. 橋軸方向 교축방향
//				1) タイプⅠ
//				2) タイプⅡ
//			4-1-2. 橋軸直角方向 교축직각방향
//				1) タイプⅠ
//				2) タイプⅡ
	auto l_WriteData = [&pXL, this](int nRow, int nCol, const T_SIET_EVAL_BEAM_FORCE& maxD, const T_SIET_EVAL_BEAM_FORCE& minD)
		{
			pXL->SetValue(nRow, nCol, maxD.dFx);
			pXL->SetValue(nRow, nCol + 1, maxD.dFy);
			pXL->SetValue(nRow, nCol + 2, maxD.dFz);
			pXL->SetValue(nRow, nCol + 3, maxD.dMx);
			pXL->SetValue(nRow, nCol + 4, maxD.dMy);
			pXL->SetValue(nRow, nCol + 5, maxD.dMz);

			pXL->SetValue(nRow + 1, nCol, minD.dFx);
			pXL->SetValue(nRow + 1, nCol + 1, minD.dFy);
			pXL->SetValue(nRow + 1, nCol + 2, minD.dFz);
			pXL->SetValue(nRow + 1, nCol + 3, minD.dMx);
			pXL->SetValue(nRow + 1, nCol + 4, minD.dMy);
			pXL->SetValue(nRow + 1, nCol + 5, minD.dMz);
		};

	BOOL bStop = FALSE;

	CArray<T_SIET_K, T_SIET_K> aSietK;
	m_pDoc->m_pAttrCtrl2->GetQSiet()->GetKeyList(aSietK);
	const auto nSiet = aSietK.GetSize();
	if (nSiet == 0) return bStop;

	CString RptBlock;
	int rowCnt = 0;
	short Row_Ref = 0, Col_Ref = 0;

#pragma region // N-1. 断面力成分別の最大・最小
	std::vector<T_SIET_K> vExistSiet[2][2];
	for (int iSiet = 0; iSiet < nSiet; ++iSiet)
	{
		const T_SIET_K SietK = aSietK[iSiet];
		T_SIET_D SietD;
		if (!m_pDoc->m_pAttrCtrl2->GetQSiet()->Get(SietK, SietD))		continue;
		if (SietD.nOutTypeMember != T_SIET_D::kMinMax_Mb)				continue;
		T_SIPA_D SipaD;
		if (!m_pDoc->m_pAttrCtrl2->GetQSipa()->Get(SietD.kSipa, SipaD)) continue;
		if (SipaD.nDesignCode != m_nDesignCode)							continue;

		const auto iIdx1 = (int)SipaD.nSeismicForceDirection - 1;
		const auto iIdx2 = (int)SipaD.nSeismicForceType - 1;
		vExistSiet[iIdx1][iIdx2].push_back(SietK);
	}

	if (vExistSiet[0][0].size() < 1 && vExistSiet[0][1].size() < 1 &&
		vExistSiet[1][0].size() < 1 && vExistSiet[1][1].size() < 1) {
		return FALSE;
	}

	RptBlock = _T("_04.Forces_MinMax");
	rowCnt = CopyRange(pXL, RptBlock, stRow);
	pXL->SetValue(stRow, 0, GetText(_T("%d-%d. %s"), iParagraph1, ++iParagraph2, _LS(IDS_BRDG_SEIS_EVAL_REPORT_JP_PIER_FORCES_MAXMIN)));//断面力成分別の最大・最小
	stRow += rowCnt;
	stRow++;

	int iParagraph3 = 0;
	for (int iSeisDir = 0; iSeisDir < 2; ++iSeisDir) // 橋軸方向, 橋軸直角方向
	{
		if (vExistSiet[iSeisDir][0].size() == 0 && vExistSiet[iSeisDir][1].size() == 0) continue;

		RptBlock = _T("_04.Forces_Dir");
		WriteSeisDir(RptBlock, iSeisDir + 1, iParagraph3, stRow);

		int iParagraph4 = 0;
		for (int iType = 0; iType < 2; ++iType) // タイプ
		{
			if (vExistSiet[iSeisDir][iType].size() == 0) continue;

			RptBlock = _T("_04.Forces_Head1");
			WriteSeisType(RptBlock, iType + 1, iParagraph4, stRow);

			for (const auto& SietK : vExistSiet[iSeisDir][iType])
			{
				T_SIET_D SietD;
				if (!m_pDoc->m_pAttrCtrl2->GetQSiet()->Get(SietK, SietD)) continue;
				T_SIGR_D SigrD;
				if (!m_pDoc->m_pAttrCtrl2->GetQSigr()->Get(SietD.kSigrLower, SigrD)) continue;

				T_SIET_EVAL_MEMB_MINMAX eval;
				if (!m_pResult->GetResultSietMembMinMax(SietD.kSipa, SietD.kSigrLower, eval)) eval.Initialize();

				const auto nGroup = eval.aGroup.GetSize();
				for (int l = 0; l < nGroup; ++l)
				{
					const auto& GroupD = eval.aGroup[l];
					RptBlock = _T("_04.Forces_Body1");
					rowCnt = CopyRange(pXL, RptBlock, stRow);
					pXL->SetValue(stRow, 0, SigrD.strName);
					pXL->SetValue(stRow, 1, GroupD.strName);
					pXL->SetValue(stRow, 2, GroupD.kElem);
					l_WriteData(stRow, 4, GroupD.max, GroupD.min);

					stRow += rowCnt;

					if (m_pProgress->NextStep() == FALSE) throw FALSE;
				}
			}
			stRow++;
		}
	}

#pragma endregion

	return bStop;
}

BOOL CSeisEvalReportPierBottom::WriteConcurEach(IExcel* pXL, const int iParagraph1, int& iParagraph2, int& stRow)
{
//	4.橋脚下端の断面力(3波平均) 교각하단의 단면력(3파평균)
//		4-1.断面力成分別の最大・最小 단면력 성분별 최대/최소
//			4-1-1. 橋軸方向 교축방향
//				1) タイプⅠ
//				2) タイプⅡ
//			4-1-2. 橋軸直角方向 교축직각방향
//				1) タイプⅠ
//				2) タイプⅡ
	auto l_WriteData = [&pXL, this](int nRow, int nCol, const T_SIET_EVAL_BEAM_FORCE& maxD, const T_SIET_EVAL_BEAM_FORCE& minD)
		{
			pXL->SetValue(nRow, nCol, maxD.dFx);
			pXL->SetValue(nRow, nCol + 1, maxD.dFy);
			pXL->SetValue(nRow, nCol + 2, maxD.dFz);
			pXL->SetValue(nRow, nCol + 3, maxD.dMx);
			pXL->SetValue(nRow, nCol + 4, maxD.dMy);
			pXL->SetValue(nRow, nCol + 5, maxD.dMz);

			pXL->SetValue(nRow + 1, nCol, minD.dFx);
			pXL->SetValue(nRow + 1, nCol + 1, minD.dFy);
			pXL->SetValue(nRow + 1, nCol + 2, minD.dFz);
			pXL->SetValue(nRow + 1, nCol + 3, minD.dMx);
			pXL->SetValue(nRow + 1, nCol + 4, minD.dMy);
			pXL->SetValue(nRow + 1, nCol + 5, minD.dMz);
		};

	BOOL bStop = FALSE;

	CArray<T_SIET_K, T_SIET_K> aSietK;
	m_pDoc->m_pAttrCtrl2->GetQSiet()->GetKeyList(aSietK);
	const auto nSiet = aSietK.GetSize();
	if (nSiet == 0) return bStop;

	CString RptBlock;
	int rowCnt = 0;
	short Row_Ref = 0, Col_Ref = 0;

#pragma region // N-2. 断面力成分別の最大・最小時における同時性結果; 교각기부(동시성)(각각), 기초하단(동시성)(각각)
	std::vector<T_SIET_K> vExistSiet[2][2];
	for (int iSiet = 0; iSiet < nSiet; ++iSiet)
	{
		const T_SIET_K SietK = aSietK[iSiet];
		T_SIET_D SietD;
		if (!m_pDoc->m_pAttrCtrl2->GetQSiet()->Get(SietK, SietD))		continue;
		if (SietD.nOutTypeMember != T_SIET_D::kConcurvity_Each_Mb)		continue;
		T_SIPA_D SipaD;
		if (!m_pDoc->m_pAttrCtrl2->GetQSipa()->Get(SietD.kSipa, SipaD)) continue;
		if (SipaD.nDesignCode != m_nDesignCode)							continue;

		const auto iIdx1 = (int)SipaD.nSeismicForceDirection - 1;
		const auto iIdx2 = (int)SipaD.nSeismicForceType - 1;
		vExistSiet[iIdx1][iIdx2].push_back(SietK);
	}

	if (vExistSiet[0][0].size() < 1 && vExistSiet[0][1].size() < 1 &&
		vExistSiet[1][0].size() < 1 && vExistSiet[1][1].size() < 1) return FALSE;
	
	RptBlock = _T("_04.Forces_Concurrent");
	rowCnt = CopyRange(pXL, RptBlock, stRow);
	pXL->SetValue(stRow, 0, GetText(_T("%d-%d. %s"), iParagraph1, ++iParagraph2, _LS(IDS_BRDG_SEIS_EVAL_REPORT_JP_PIER_FORCES_CONCURRENT_EACH)));//断面力成分別の最大・最小時における同時性結果
	stRow += rowCnt;
	stRow++;

	int iParagraph3 = 0;
	for (int iSeisDir = 0; iSeisDir < 2; ++iSeisDir) // 橋軸方向, 橋軸直角方向
	{
		if (vExistSiet[iSeisDir][0].size() == 0 && vExistSiet[iSeisDir][1].size() == 0) continue;
		const auto nDir = iSeisDir == 0 ? T_SIPA_D::kAxis_Dir : T_SIPA_D::kPerp_Dir;

		RptBlock = _T("_04.Forces_Dir");
		WriteSeisDir(RptBlock, iSeisDir+1, iParagraph3, stRow);

		int iParagraph4 = 0;
		for (int iType = 0; iType < 2; ++iType) // タイプ
		{
			if (vExistSiet[iSeisDir][iType].size() == 0) continue;
			const auto nType = iType == 0 ? T_SIPA_D::kType1 : T_SIPA_D::kType2;

			RptBlock = _T("_04.Forces_Head2");
			WriteSeisType(RptBlock, iType + 1, iParagraph4, stRow);

			for (const auto& SietK : vExistSiet[iSeisDir][iType])
			{
				T_SIET_D SietD;
				if (!m_pDoc->m_pAttrCtrl2->GetQSiet()->Get(SietK, SietD)) continue;
				T_SIGR_D SigrD;
				if (!m_pDoc->m_pAttrCtrl2->GetQSigr()->Get(SietD.kSigrLower, SigrD)) continue;

				T_SIET_EVAL_MEMB_EACH eval;
				if (!m_pResult->GetResultSietMembConcEach(SietD.kSipa, SietD.kSigrLower, eval)) eval.Initialize();

				const auto nGroup = eval.aGroup.GetSize();
				for (int l = 0; l < nGroup; ++l)
				{
					const auto& GroupD = eval.aGroup[l];
					RptBlock = _T("_04.Forces_Body2");
					rowCnt = CopyRange(pXL, RptBlock, stRow);
					pXL->SetValue(stRow, 0, SigrD.strName);
					pXL->SetValue(stRow, 1, GroupD.strName);
					for (int m = 0; m < T_SIET_EVAL_MEMB_CONCU::kForceNum; ++m)
					{
						pXL->SetValue(stRow + 2 * m, 2, GroupD.kElem);
						l_WriteData(stRow + 2 * m, 5, GroupD.max[m], GroupD.min[m]);
					}

					stRow += rowCnt;

					if (m_pProgress->NextStep() == FALSE) throw FALSE;
				}
			}
			stRow++;
		}
	}
	
#pragma endregion

	return bStop;
}

BOOL CSeisEvalReportPierBottom::WriteConcurByFooting(IExcel* pXL, const int iParagraph1, int& iParagraph2, int& stRow)
{
//	4.橋脚下端の断面力(3波平均) 교각하단의 단면력(3파평균)
//		4-2.断面力成分別の最大・最小時における同時性結果 단면력 성분별 최대·최소시에 있어서의 동시발생 결과
//			4-2-1. 橋軸方向 교축방향
//				1) タイプⅠ
//				2) タイプⅡ
//			4-2-2. 橋軸直角方向 교축직각방향
//				1) タイプⅠ
//				2) タイプⅡ
	auto l_WriteData = [&pXL, this](int nRow, int nCol, const T_SIET_EVAL_BEAM_FORCE& maxD, const T_SIET_EVAL_BEAM_FORCE& minD)
		{
			pXL->SetValue(nRow, nCol, maxD.dFx);
			pXL->SetValue(nRow, nCol + 1, maxD.dFy);
			pXL->SetValue(nRow, nCol + 2, maxD.dFz);
			pXL->SetValue(nRow, nCol + 3, maxD.dMx);
			pXL->SetValue(nRow, nCol + 4, maxD.dMy);
			pXL->SetValue(nRow, nCol + 5, maxD.dMz);

			pXL->SetValue(nRow + 1, nCol, maxD.dFx);
			pXL->SetValue(nRow + 1, nCol + 1, maxD.dFy);
			pXL->SetValue(nRow + 1, nCol + 2, maxD.dFz);
			pXL->SetValue(nRow + 1, nCol + 3, maxD.dMx);
			pXL->SetValue(nRow + 1, nCol + 4, maxD.dMy);
			pXL->SetValue(nRow + 1, nCol + 5, maxD.dMz);
		};

	BOOL bStop = FALSE;

	CArray<T_SIET_K, T_SIET_K> aSietK;
	m_pDoc->m_pAttrCtrl2->GetQSiet()->GetKeyList(aSietK);
	const auto nSiet = aSietK.GetSize();
	if (nSiet == 0) return bStop;

	CString RptBlock;
	int rowCnt = 0;
	short Row_Ref = 0, Col_Ref = 0;

	const CString strDofs[6] = { _T("軸力"), _T("せん断-y"), _T("せん断-z"), _T("ねじりﾓｰﾒﾝﾄ"), _T("ﾓｰﾒﾝﾄ-y"), _T("ﾓｰﾒﾝﾄ-z") };

#pragma region // N-3. 교각기부기준(동시성)
	std::vector<T_SIET_K> vExistSiet[2][2];
	for (int iSiet = 0; iSiet < nSiet; ++iSiet)
	{
		const T_SIET_K SietK = aSietK[iSiet];
		T_SIET_D SietD;
		if (!m_pDoc->m_pAttrCtrl2->GetQSiet()->Get(SietK, SietD))		continue;
		if (SietD.nOutTypeMember != T_SIET_D::kConcurvity_FootingLow)	continue;
		T_SIPA_D SipaD;
		if (!m_pDoc->m_pAttrCtrl2->GetQSipa()->Get(SietD.kSipa, SipaD)) continue;
		if (SipaD.nDesignCode != m_nDesignCode)							continue;

		const auto iIdx1 = (int)SipaD.nSeismicForceDirection - 1;
		const auto iIdx2 = (int)SipaD.nSeismicForceType - 1;
		vExistSiet[iIdx1][iIdx2].push_back(SietK);
	}

	if (vExistSiet[0][0].size() < 1 && vExistSiet[0][1].size() < 1 &&
		vExistSiet[1][0].size() < 1 && vExistSiet[1][1].size() < 1) {
		return FALSE;
	}

	RptBlock = _T("_04.Forces_Concurrent");
	rowCnt = CopyRange(pXL, RptBlock, stRow);
	pXL->SetValue(stRow, 0, GetText(_T("%d-%d. %s"), iParagraph1, ++iParagraph2, _LS(IDS_BRDG_SEIS_EVAL_REPORT_JP_PIER_FORCES_CONCURRENT_FND)));//断面力成分別の最大・最小時における同時性結果
	stRow += rowCnt;
	stRow++;

	int iParagraph3 = 0;
	for (int iSeisDir = 0; iSeisDir < 2; ++iSeisDir) // 橋軸方向, 橋軸直角方向
	{
		if (vExistSiet[iSeisDir][0].size() == 0 && vExistSiet[iSeisDir][1].size() == 0) continue;
		const auto nDir = iSeisDir == 0 ? T_SIPA_D::kAxis_Dir : T_SIPA_D::kPerp_Dir;

		RptBlock = _T("_04.Forces_Dir");
		WriteSeisDir(RptBlock, iSeisDir + 1, iParagraph3, stRow);

		int iParagraph4 = 0;
		for (int iType = 0; iType < 2; ++iType) // タイプ
		{
			if (vExistSiet[iSeisDir][iType].size() == 0) continue;
			const auto nType = iType == 0 ? T_SIPA_D::kType1 : T_SIPA_D::kType2;

			RptBlock = _T("_04.Forces_Head3");
			WriteSeisType(RptBlock, iType + 1, iParagraph4, stRow);

			for (const auto& SietK : vExistSiet[iSeisDir][iType]) // SIET
			{
				T_SIET_D SietD;
				if (!m_pDoc->m_pAttrCtrl2->GetQSiet()->Get(SietK, SietD)) continue;
				T_SIGR_D SigrD;
				if (!m_pDoc->m_pAttrCtrl2->GetQSigr()->Get(SietD.kSigrLower, SigrD)) continue;
				T_SIGR_LOWER_D SigrLowerD;
				if (!m_pDoc->m_pAttrCtrl2->GetQSigrLower()->Get(SietD.kSigrLower, SigrLowerD)) continue;

				const auto nFoot = SigrLowerD.FOOT_D.GetSize();
				for (int k = 0; k < nFoot; ++k)	// Number of Foot
				{
					const T_SIGR_LOWER_FOOT_D& foot = SigrLowerD.FOOT_D[k];
					const auto nElem = foot.aInvestigatedElem.GetSize();
					for (int l = 0; l < nElem; ++l)	// // Number of element of Foot
					{
						const auto& ElemK = foot.aInvestigatedElem[l];
						T_SIET_EVAL_FOOT_MASTER eval;
						if (!m_pResult->GetResultSietConcFootingLow(SietD.kSipa, SietD.kSigrLower, ElemK, eval)) continue;

						int GrpRow = stRow;
						for (int dof = 0; dof < 6; ++dof) // degree of freedoms
						{
							const auto& ComponentD = eval.FootConcu[dof];

							int DofsRow = stRow;
							for (int mm = 0; mm < 2; ++mm) // max/min type
							{
								const auto& aConcuD = mm == 0 ? ComponentD.aMax : ComponentD.aMin;
								const CString strMaxMin = mm == 0 ? _T("Max") : _T("Min");

								const int nCnt = aConcuD.GetSize();
								for (int gp = 0; gp < nCnt; ++gp) // Group
								{
									const auto& ElemD = aConcuD.GetAt(gp);

									if (mm == 0 && gp == 0)				RptBlock = _T("_04.Forces_Body3_St");
									else if (mm == 1 && gp == nCnt - 1)	RptBlock = _T("_04.Forces_Body3_Ed");
									else    						RptBlock = _T("_04.Forces_Body3_Mid");
									rowCnt = CopyRange(pXL, RptBlock, stRow);

									pXL->SetValue(stRow, 1, ElemD.strName);
									pXL->SetValue(stRow, 2, ElemD.kElem);
									if (gp == 0) {
										pXL->SetValue(stRow, 3, strMaxMin);
										if (mm == 0) {
											pXL->SetValue(stRow, 4, strDofs[dof]);
											if (dof == 0) { pXL->SetValue(stRow, 0, SigrD.strName); }
										}
									}

									pXL->SetValue(stRow, 5, ElemD.Force.dFx);
									pXL->SetValue(stRow, 6, ElemD.Force.dFy);
									pXL->SetValue(stRow, 7, ElemD.Force.dFz);
									pXL->SetValue(stRow, 8, ElemD.Force.dMx);
									pXL->SetValue(stRow, 9, ElemD.Force.dMy);
									pXL->SetValue(stRow, 10, ElemD.Force.dMz);

									if (ElemK == ElemD.kElem)
									{
										pXL->SetTextColor(stRow, 5 + dof, stRow, 5 + dof, (mm == 0 ? RED : BLUE));
									}
									stRow += rowCnt;

									if (gp == 4)
									{
										// merge max/min
										if (mm == 1)
										{
											auto* pFormat = pXL->GetSheet()->cellFormat(stRow - 1, 3);
											pXL->GetSheet()->setCellFormat(stRow - nCnt, 3, pFormat);
											pXL->SetMergeCell(stRow - nCnt, 3, stRow - 1, 3);
										}
										else
										{
											pXL->SetMergeCell(stRow - nCnt, 3, stRow - 1, 3);
										}

										if (mm == 1)
										{
											// merge Dofs
											auto* pFormat = pXL->GetSheet()->cellFormat(stRow - 1, 4);
											pXL->GetSheet()->setCellFormat(stRow - nCnt * 2, 4, pFormat);
											pXL->SetMergeCell(stRow - nCnt * 2, 4, stRow - 1, 4);

											if (dof == 5)
											{
												// merge Group
												auto* pFormat = pXL->GetSheet()->cellFormat(stRow - 1, 0);
												pXL->GetSheet()->setCellFormat(stRow - nCnt * 2 * 6, 0, pFormat);
												pXL->SetMergeCell(stRow - nCnt * 2 * 6, 0, stRow - 1, 0);
											}
										}
									}

									if (m_pProgress->NextStep() == FALSE) throw FALSE;
								} // Group
							} // Max/Min
						}  // Dofs
					} // Element
				} // Foot
			} // SIET
			stRow++;
		} // タイプ
	} // 方向

#pragma endregion
	return bStop;
}

#pragma endregion

#pragma region // CSeisEvalReportBearings
CSeisEvalReportBearings::CSeisEvalReportBearings(IExcel* pXL, CDgnProgressDlg* pProgress, const CString& strBasePath, const CString& strBaseBook, const SDgnExcelPrintOpt* pPrintOpt)
	: CWriteSeisEvalReportBase(pXL, pProgress, strBasePath, strBaseBook, pPrintOpt)
{
}

CSeisEvalReportBearings::~CSeisEvalReportBearings()
{
}

void CSeisEvalReportBearings::AddStep()
{
	AddStepMaxMin();
	AddStepConcurEach();
	AddStepConcurByLine();
}

void CSeisEvalReportBearings::AddStepMaxMin()
{
	CArray<T_SIET_K, T_SIET_K> aSietK;
	m_pDoc->m_pAttrCtrl2->GetQSiet()->GetKeyList(aSietK);
	const auto nSiet = aSietK.GetSize();
	if (nSiet == 0) return;

	std::vector<T_SIET_K> vExistSiet[2][2];
	for (int i = 0; i < nSiet; ++i)
	{
		const T_SIET_K SietK = aSietK[i];
		T_SIET_D SietD;
		if (!m_pDoc->m_pAttrCtrl2->GetQSiet()->Get(SietK, SietD))		continue;
		if (SietD.nOutTypeBearing != T_SIET_D::kMinMax_Br)				continue;
		T_SIPA_D SipaD;
		if (!m_pDoc->m_pAttrCtrl2->GetQSipa()->Get(SietD.kSipa, SipaD)) continue;
		if (SipaD.nDesignCode != m_nDesignCode)							continue;

		const auto iIdx1 = (int)SipaD.nSeismicForceDirection - 1;
		const auto iIdx2 = (int)SipaD.nSeismicForceType - 1;
		vExistSiet[iIdx1][iIdx2].push_back(SietK);
	}

	if (vExistSiet[0][0].size() < 1 && vExistSiet[0][1].size() < 1 &&
		vExistSiet[1][0].size() < 1 && vExistSiet[1][1].size() < 1) return;
	
	for (int i = 0; i < 2; ++i) // 橋軸方向, 橋軸直角方向
	{
		if (vExistSiet[i][0].size() == 0 && vExistSiet[i][1].size() == 0) continue;

		for (int j = 0; j < 2; ++j) // タイプ
		{
			if (vExistSiet[i][j].size() == 0) continue;

			const int nGroup = vExistSiet[i][j].size();
			for (int k = 0; k < nGroup; ++k)
			{
				const T_SIET_K SietK = vExistSiet[i][j][k];
				T_SIET_D SietD;
				if (!m_pDoc->m_pAttrCtrl2->GetQSiet()->Get(SietK, SietD)) continue;
				T_SIGR_D SigrD;
				if (!m_pDoc->m_pAttrCtrl2->GetQSigr()->Get(SietD.kSigrLower, SigrD)) continue;

				T_SIET_EVAL_BEAR_MINMAX eval;
				if (!m_pResult->GetResultSietBearMinMax(SietD.kSipa, SietD.kSigrLower, eval)) eval.Initialize();

				const int nBearingLine = 2;
				for (int l = 0; l < nBearingLine; ++l)
				{
					const auto& BearingD = l == 0 ? eval.Line1 : eval.Line2;

					m_pProgress->m_nTotalSteps++;
				}
			}
		}
	}
}

void CSeisEvalReportBearings::AddStepConcurEach()
{
	CArray<T_SIET_K, T_SIET_K> aSietK;
	m_pDoc->m_pAttrCtrl2->GetQSiet()->GetKeyList(aSietK);
	const auto nSiet = aSietK.GetSize();
	if (nSiet == 0) return;

	std::vector<T_SIET_K> vExistSiet[2][2];
	for (int i = 0; i < nSiet; ++i)
	{
		const T_SIET_K SietK = aSietK[i];
		T_SIET_D SietD;
		if (!m_pDoc->m_pAttrCtrl2->GetQSiet()->Get(SietK, SietD))		continue;
		if (SietD.nOutTypeBearing != T_SIET_D::kConcurvity_Each_Br)		continue;
		T_SIPA_D SipaD;
		if (!m_pDoc->m_pAttrCtrl2->GetQSipa()->Get(SietD.kSipa, SipaD)) continue;
		if (SipaD.nDesignCode != m_nDesignCode)							continue;

		const auto iIdx1 = (int)SipaD.nSeismicForceDirection - 1;
		const auto iIdx2 = (int)SipaD.nSeismicForceType - 1;
		vExistSiet[iIdx1][iIdx2].push_back(SietK);
	}

	if (vExistSiet[0][0].size() < 1 || vExistSiet[0][1].size() < 1 ||
		vExistSiet[1][0].size() < 1 || vExistSiet[1][1].size() < 1) return;
	
	for (int i = 0; i < 2; ++i) // 橋軸方向, 橋軸直角方向
	{
		if (vExistSiet[i][0].size() == 0 && vExistSiet[i][1].size() == 0) continue;

		for (int j = 0; j < 2; ++j) // タイプ
		{
			if (vExistSiet[i][j].size() == 0) continue;

			const int nGroup = vExistSiet[i][j].size();
			for (int k = 0; k < nGroup; ++k)
			{
				const T_SIET_K SietK = vExistSiet[i][j][k];
				T_SIET_D SietD;
				if (!m_pDoc->m_pAttrCtrl2->GetQSiet()->Get(SietK, SietD)) continue;
				T_SIGR_D SigrD;
				if (!m_pDoc->m_pAttrCtrl2->GetQSigr()->Get(SietD.kSigrLower, SigrD)) continue;

				T_SIET_EVAL_BEAR_EACH eval;
				if (!m_pResult->GetResultSietBearConcEach(SietD.kSipa, SietD.kSigrLower, eval)) eval.Initialize();

				int nBearingLine = 2; // line은 2개 고정
				for (int l = 0; l < nBearingLine; ++l)
				{
					m_pProgress->m_nTotalSteps++;
				}
			}
		}
	}
}

void CSeisEvalReportBearings::AddStepConcurByLine()
{
	CArray<T_SIET_K, T_SIET_K> aSietK;
	m_pDoc->m_pAttrCtrl2->GetQSiet()->GetKeyList(aSietK);
	const auto nSiet = aSietK.GetSize();
	if (nSiet == 0) return;

	std::vector<T_SIET_K> vExistSiet[2][2];
	for (int i = 0; i < nSiet; ++i)
	{
		const T_SIET_K SietK = aSietK[i];
		T_SIET_D SietD;
		if (!m_pDoc->m_pAttrCtrl2->GetQSiet()->Get(SietK, SietD))		continue;
		if (SietD.nOutTypeBearing != T_SIET_D::kConcurvity_Line_Br)		continue;
		T_SIPA_D SipaD;
		if (!m_pDoc->m_pAttrCtrl2->GetQSipa()->Get(SietD.kSipa, SipaD)) continue;
		if (SipaD.nDesignCode != m_nDesignCode)							continue;

		const auto iIdx1 = (int)SipaD.nSeismicForceDirection - 1;
		const auto iIdx2 = (int)SipaD.nSeismicForceType - 1;
		vExistSiet[iIdx1][iIdx2].push_back(SietK);
	}

	if (vExistSiet[0][0].size() < 1 || vExistSiet[0][1].size() < 1 ||
		vExistSiet[1][0].size() < 1 || vExistSiet[1][1].size() < 1) return;

	for (int i = 0; i < 2; ++i) // 橋軸方向, 橋軸直角方向
	{
		if (vExistSiet[i][0].size() == 0 && vExistSiet[i][1].size() == 0) continue;

		for (int j = 0; j < 2; ++j) // タイプ
		{
			if (vExistSiet[i][j].size() == 0) continue;

			const int nGroup = vExistSiet[i][j].size();
			for (int k = 0; k < nGroup; ++k)
			{
				const T_SIET_K SietK = vExistSiet[i][j][k];
				T_SIET_D SietD;
				if (!m_pDoc->m_pAttrCtrl2->GetQSiet()->Get(SietK, SietD)) continue;
				T_SIGR_D SigrD;
				if (!m_pDoc->m_pAttrCtrl2->GetQSigr()->Get(SietD.kSigrLower, SigrD)) continue;

				T_SIET_EVAL_LINE_CONC eval;
				if (!m_pResult->GetResultSietBearLineConc(SietD.kSipa, SietD.kSigrLower, eval)) continue;

				int nBearingLine = 2; // line은 2개 고정
				for (int l = 0; l < nBearingLine; ++l)
				{
					m_pProgress->m_nTotalSteps++;
				}
			}
		}
	}
}

void CSeisEvalReportBearings::Write()
{
//	5.支承の断面力と変位(3波平均) 받침 단면력과 변위(3파평균)
//		5-1.断面力の算出：支承別の最大	단면력산출 : 받침별 최대
//			5-1-1. 橋軸方向 교축방향
//				1) タイプⅠ
//				2) タイプⅡ
//			5-1-2. 橋軸直角方向 교축직각방향
//				1) タイプⅠ
//				2) タイプⅡ
//		5-2.断面力の算出：支承グループ別の同時性 단면력산출 : 받침그룹별 동시발생
//			5-2-1. 橋軸方向 교축방향
//				1) タイプⅠ
//				2) タイプⅡ
//			5-2-2. 橋軸直角方向 교축직각방향
//				1) タイプⅠ
//				2) タイプⅡ
	CBrdgSeisEvalResult_JP* pResult = m_pDoc->m_pPostCtrl->GetBrdgSeisEvalResult_JP(); if (!pResult) { ASSERT(0); return; }

	CArray<T_SIET_K, T_SIET_K> aSietK;
	m_pDoc->m_pAttrCtrl2->GetQSiet()->GetKeyList(aSietK);
	const auto nSiet = aSietK.GetSize();
	if (nSiet == 0) return;

	std::vector<BOOL>bCheck = { FALSE, FALSE, FALSE };
	for (int i = 0; i < nSiet; ++i)
	{
		const T_SIET_K SietK = aSietK[i];
		T_SIET_D SietD;
		if (!m_pDoc->m_pAttrCtrl2->GetQSiet()->Get(SietK, SietD))		continue;
		T_SIPA_D SipaD;
		if (!m_pDoc->m_pAttrCtrl2->GetQSipa()->Get(SietD.kSipa, SipaD)) continue;
		if (SipaD.nDesignCode != m_nDesignCode)							continue;

		if (SietD.nOutTypeBearing == T_SIET_D::kMinMax_Br) bCheck[0] = TRUE;
		if (SietD.nOutTypeBearing == T_SIET_D::kConcurvity_Each_Br) bCheck[1] = TRUE;
		if (SietD.nOutTypeBearing == T_SIET_D::kConcurvity_Line_Br) bCheck[2] = TRUE;
	}

	if (!bCheck[0] && !bCheck[1] && !bCheck[2]) return;

	size_t nMinMax_Br = pResult->GetResultSietBearMinMaxMap().size();
	size_t nEach_Br = pResult->GetResultSietBearConcEachMap().size();
	size_t nLine_Br = pResult->GetResultSietBearLineConcMap().size();

	size_t nCount = 0;
	if (bCheck[0]) nCount += nMinMax_Br;
	if (bCheck[1]) nCount += nEach_Br;
	if (bCheck[2]) nCount += nLine_Br;

	if (nCount < 1)
		return;

	CString strSheetName;
	AddSheet(strSheetName);

	CString RptBlock;
	int rowCnt = 0;
	int stRow = 0;
	short Row_Ref = 0, Col_Ref = 0;

	*this->m_pParagraph1 = *this->m_pParagraph1 + 1;
	WirteTheory(*this->m_pParagraph1, stRow);

	BOOL bWrite[3] = { FALSE, FALSE, FALSE };
	int iParagraph2 = 0;
	WriteMaxMin(m_pXL, *this->m_pParagraph1, iParagraph2, stRow, bWrite[0]);
	WriteConcurEach(m_pXL, *this->m_pParagraph1, iParagraph2, stRow, bWrite[1]);
	WriteConcurByLine(m_pXL, *this->m_pParagraph1, iParagraph2, stRow, bWrite[2]);

	if (m_pXL->IsExistSheetName(strSheetName) && !bWrite[0] && !bWrite[1] && !bWrite[2])
		m_pXL->DeleteSheet(strSheetName);
}

void CSeisEvalReportBearings::AddSheet(CString& strSheetName)
{
	m_strBaseSheet = _LSX(05.Bearing);
	strSheetName = _LS(IDS_BRDG_SEIS_EVAL_REPORT_JP_BEARING);// _T("支承");
	if (m_pXL->IsExistSheetName(strSheetName))
	{
		ASSERT(0);
		m_pXL->SetActiveSheet(strSheetName);
	}
	else
	{
		m_pXL->AddSheet2(strSheetName);
		m_pXL->SetActiveSheet(strSheetName);
	}

	m_pXL->SetStandardFont(_T("Meiryo UI"));
	m_pXL->SetMargin(13, 12, 20, 17);
	m_pXL->SetColumnWidth(0, 0, 0, 0, 6.9 * _ToPoint); //  6.9 * 0.833333333
	m_pXL->SetColumnWidth(0, 1, 0, 1, 7.7 * _ToPoint);
	m_pXL->SetColumnWidth(0, 2, 0, 5, 7.4 * _ToPoint);
	m_pXL->SetColumnWidth(0, 6, 0, 6, 9.9 * _ToPoint);
	m_pXL->SetColumnWidth(0, 7, 0, 60, 7.9 * _ToPoint);
	m_pXL->SetRowHeight(16.5);
	m_pXL->SetZoom(70);
}

void CSeisEvalReportBearings::WirteTheory(const int iParagraph1, OUT int& stRow)
{
	CString RptBlock = _T("_05.Bearing_Title");
	int rowCnt = CopyRange(m_pXL, RptBlock, stRow);
	m_pXL->SetValue(stRow, 0, GetText(_T("%d. %s"), iParagraph1, _LS(IDS_BRDG_SEIS_EVAL_REPORT_JP_BEARING_RES))); // 支承の断面力と変位(3波平均)
	stRow += rowCnt;
}

BOOL CSeisEvalReportBearings::WriteMaxMin(IExcel* pXL, const int iParagraph1, int& iParagraph2, int& stRow, BOOL& bWrite)
{
	CBrdgSeisEvalResult_JP* pResult = m_pDoc->m_pPostCtrl->GetBrdgSeisEvalResult_JP(); if (!pResult) { ASSERT(0); return FALSE; }
	if (pResult->GetResultSietBearMinMaxMap().size() < 1) return FALSE;

//	5.支承の断面力と変位(3波平均) 받침 단면력과 변위(3파평균)
//		5-1.断面力の算出：支承別の最大	단면력산출 : 받침별 최대
//			5-1-1. 橋軸方向 교축방향
//				1) タイプⅠ
//				2) タイプⅡ
//			5-1-2. 橋軸直角方向 교축직각방향
//				1) タイプⅠ
//				2) タイプⅡ
	auto l_WriteData = [&pXL, this](int nRow, int nCol, double dMax[5], double dMin[5])
		{
			for (int i = 0; i < 5; ++i)
			{
				pXL->SetValue(nRow, nCol + i, dMax[i]);
				pXL->SetValue(nRow + 1, nCol + i, dMin[i]);
			}
		};

	CArray<T_SIET_K, T_SIET_K> aSietK;
	m_pDoc->m_pAttrCtrl2->GetQSiet()->GetKeyList(aSietK);
	const auto nSiet = aSietK.GetSize();
	if (nSiet == 0) return FALSE;

	CString RptBlock;
	int rowCnt = 0;
	short Row_Ref = 0, Col_Ref = 0;

#pragma region // N-1. 断面力の算出：支承別の最大
	std::vector<T_SIET_K> vExistSiet[2][2];
	for (int iSiet = 0; iSiet < nSiet; ++iSiet)
	{
		const T_SIET_K SietK = aSietK[iSiet];
		T_SIET_D SietD;
		if (!m_pDoc->m_pAttrCtrl2->GetQSiet()->Get(SietK, SietD))		continue;
		if (SietD.nOutTypeBearing != T_SIET_D::kMinMax_Br)				continue;
		T_SIPA_D SipaD;
		if (!m_pDoc->m_pAttrCtrl2->GetQSipa()->Get(SietD.kSipa, SipaD)) continue;
		if (SipaD.nDesignCode != m_nDesignCode)							continue;

		const auto iIdx1 = (int)SipaD.nSeismicForceDirection - 1;
		const auto iIdx2 = (int)SipaD.nSeismicForceType - 1;
		vExistSiet[iIdx1][iIdx2].push_back(SietK);
	}

	if (vExistSiet[0][0].size() < 1 && vExistSiet[0][1].size() < 1 &&
		vExistSiet[1][0].size() < 1 && vExistSiet[1][1].size() < 1) {
		return FALSE;
	}

	RptBlock = _T("_05.Bearing_Max");
	rowCnt = CopyRange(pXL, RptBlock, stRow);
	pXL->SetValue(stRow, 0, GetText(_T("%d-%d. %s"), iParagraph1, ++iParagraph2, _LS(IDS_BRDG_SEIS_EVAL_REPORT_JP_BEARING_MAX))); // 断面力の算出：支承別の最大
	stRow += rowCnt;
	stRow++;

	int iParagraph3 = 0;
	for (int iSeisDir = 0; iSeisDir < 2; ++iSeisDir) // 橋軸方向, 橋軸直角方向
	{
		if (vExistSiet[iSeisDir][0].size() == 0 && vExistSiet[iSeisDir][1].size() == 0) continue;

		RptBlock = _T("_05.Bearing_Dir");
		WriteSeisDir(RptBlock, iSeisDir + 1, iParagraph3, stRow);

		int iParagraph4 = 0;
		for (int iType = 0; iType < 2; ++iType) // タイプ
		{
			if (vExistSiet[iSeisDir][iType].size() == 0) continue;

			RptBlock = _T("_05.Bearing_Head");
			WriteSeisType(RptBlock, iType + 1, iParagraph4, stRow);

			const size_t nGroup = vExistSiet[iSeisDir][iType].size();
			for (int k = 0; k < nGroup; ++k)
			{
				const T_SIET_K SietK = vExistSiet[iSeisDir][iType][k];
				T_SIET_D SietD;
				if (!m_pDoc->m_pAttrCtrl2->GetQSiet()->Get(SietK, SietD)) continue;
				T_SIGR_D SigrD;
				if (!m_pDoc->m_pAttrCtrl2->GetQSigr()->Get(SietD.kSigrLower, SigrD)) continue;

				T_SIET_EVAL_BEAR_MINMAX eval;
				if (!m_pResult->GetResultSietBearMinMax(SietD.kSipa, SietD.kSigrLower, eval)) eval.Initialize();

				int stRowBefore = stRow;
				const int nBearingLine = 2;
				for (int l = 0; l < nBearingLine; ++l)
				{
					const auto& BearingD = l == 0 ? eval.Line1 : eval.Line2;
					if (BearingD.nLineNum == 0) continue;

					int nBearing = BearingD.aGroup.GetSize();
					for (int m = 0; m < nBearing; ++m)
					{
						const auto& GroupD = BearingD.aGroup[m];

						if (k == 0 && l == 0 && m == 0)
							RptBlock = _T("_05.Bearing_Body_st");
						else if (k == nGroup - 1 && l == nBearingLine - 1 && m == nBearing - 1)
							RptBlock = _T("_05.Bearing_Body_ed");
						else
							RptBlock = _T("_05.Bearing_Body_mid");
						rowCnt = CopyRange(pXL, RptBlock, stRow + m);

						//===============================================================
						// Set data
						pXL->SetValue(stRow + m, 3, GroupD.strName);
						pXL->SetValue(stRow + m, 4, GetText(_T("%d, %d"), GroupD.kNode_i, GroupD.kNode_j));
						pXL->SetValue(stRow + m, 5, GetTextBearingType(SietD.kSipa, GroupD.nType));
						pXL->SetValue(stRow + m, 6, GroupD.strComp);
						pXL->SetValue(stRow + m, 7, GroupD.Force.dHmax);
						pXL->SetValue(stRow + m, 9, GroupD.Force.dVmax);
						pXL->SetValue(stRow + m, 11, GroupD.Force.dUmax * m_dLenToMM);
						//===============================================================
						bWrite = TRUE;
					}
					int nMergeCol[4] = { 2, 8, 10, 12 };
					for (int m = 0; m < 4; ++m)
					{
						if (k == nGroup - 1 && l == nBearingLine - 1)
						{
							auto* pFormat = pXL->GetSheet()->cellFormat(stRow + nBearing - 1, nMergeCol[m]);
							pXL->GetSheet()->setCellFormat(stRow, nMergeCol[m], pFormat);
						}
						pXL->SetMergeCell(stRow, nMergeCol[m], stRow + nBearing - 1, nMergeCol[m]);
					}

					//===============================================================
					// Set data
					pXL->SetValue(stRow, nMergeCol[0], BearingD.nLineNum);
					pXL->SetValue(stRow, nMergeCol[1], BearingD.SumForce.dHmax);
					pXL->SetValue(stRow, nMergeCol[2], BearingD.SumForce.dVmax);
					pXL->SetValue(stRow, nMergeCol[3], BearingD.SumForce.dUmax * m_dLenToMM);
					//===============================================================

					stRow += nBearing;
					if (m_pProgress->NextStep() == FALSE) throw FALSE;
				}

				for (int m = 0; m < 2; ++m)
				{
					if (k == nGroup - 1)
					{
						auto* pFormat = pXL->GetSheet()->cellFormat(stRow - 1, m);
						pXL->GetSheet()->setCellFormat(stRowBefore, m, pFormat);
					}
					pXL->SetMergeCell(stRowBefore, m, stRow - 1, m);
				}
				pXL->SetValue(stRowBefore, 0, SigrD.strName);
				pXL->SetValue(stRowBefore, 1, _T("--"));
			}
			RptBlock = _T("_05.Bearing_Anno");
			rowCnt = CopyRange(pXL, RptBlock, stRow);
			stRow += rowCnt;
			stRow++;
		}
	}

#pragma endregion

	return TRUE;
}

BOOL CSeisEvalReportBearings::WriteConcurEach(IExcel* pXL, const int iParagraph1, int& iParagraph2, int& stRow, BOOL& bWrite)
{
	CBrdgSeisEvalResult_JP* pResult = m_pDoc->m_pPostCtrl->GetBrdgSeisEvalResult_JP(); if (!pResult) { ASSERT(0); return FALSE; }
	if (pResult->GetResultSietBearConcEachMap().size() < 1) return FALSE;

//	5.支承の断面力と変位(3波平均) 받침 단면력과 변위(3파평균)
//		5-1.断面力の算出：支承別の最大	단면력산출 : 받침별 최대
//			5-1-1. 橋軸方向 교축방향
//				1) タイプⅠ
//				2) タイプⅡ
//			5-1-2. 橋軸直角方向 교축직각방향
//				1) タイプⅠ
//				2) タイプⅡ
	auto l_WriteData = [&pXL, this](int nRow, int nCol, double dMax[5], double dMin[5])
		{
			for (int i = 0; i < 5; ++i)
			{
				pXL->SetValue(nRow, nCol + i, dMax[i]);
				pXL->SetValue(nRow + 1, nCol + i, dMin[i]);
			}
		};

	CArray<T_SIET_K, T_SIET_K> aSietK;
	m_pDoc->m_pAttrCtrl2->GetQSiet()->GetKeyList(aSietK);
	const auto nSiet = aSietK.GetSize();
	if (nSiet == 0) return FALSE;

	CString RptBlock;
	int rowCnt = 0;
	short Row_Ref = 0, Col_Ref = 0;

	enum enCuncurrent
	{
		enCuncurrent_Hmax = 0,
		enCuncurrent_Hmin,
		enCuncurrent_Num
	};

	CString strCuncurrent[enCuncurrent_Num] = { _T("Hmax"), _T("Hmin") };

#pragma region // N-2. 断面力の算出：支承グループ別の同時性; 각각
	std::vector<T_SIET_K> vExistSiet[2][2];
	for (int iSiet = 0; iSiet < nSiet; ++iSiet)
	{
		const T_SIET_K SietK = aSietK[iSiet];
		T_SIET_D SietD;
		if (!m_pDoc->m_pAttrCtrl2->GetQSiet()->Get(SietK, SietD))		continue;
		if (SietD.nOutTypeBearing != T_SIET_D::kConcurvity_Each_Br)		continue;
		T_SIPA_D SipaD;
		if (!m_pDoc->m_pAttrCtrl2->GetQSipa()->Get(SietD.kSipa, SipaD)) continue;
		if (SipaD.nDesignCode != m_nDesignCode)							continue;

		const auto iIdx1 = (int)SipaD.nSeismicForceDirection - 1;
		const auto iIdx2 = (int)SipaD.nSeismicForceType - 1;
		vExistSiet[iIdx1][iIdx2].push_back(SietK);
	}

	if (vExistSiet[0][0].size() < 1 && vExistSiet[0][1].size() < 1 &&
		vExistSiet[1][0].size() < 1 && vExistSiet[1][1].size() < 1) {
		return FALSE;
	}
	
	RptBlock = _T("_05.Bearing_Cuncurrent");
	rowCnt = CopyRange(pXL, RptBlock, stRow);
	pXL->SetValue(stRow, 0, GetText(_T("%d-%d. %s"), iParagraph1, ++iParagraph2, _LS(IDS_BRDG_SEIS_EVAL_REPORT_JP_BEARING_CONCURRENT_EACH)));//断面力の算出：支承グループ別の同時性
	stRow += rowCnt;
	stRow++;

	int iParagraph3 = 0;
	for (int iSeisDir = 0; iSeisDir < 2; ++iSeisDir) // 橋軸方向, 橋軸直角方向
	{
		if (vExistSiet[iSeisDir][0].size() == 0 && vExistSiet[iSeisDir][1].size() == 0) continue;

		RptBlock = _T("_05.Bearing_Dir");
		WriteSeisDir(RptBlock, iSeisDir + 1, iParagraph3, stRow);

		int iParagraph4 = 0;
		for (int iType = 0; iType < 2; ++iType) // タイプ
		{
			if (vExistSiet[iSeisDir][iType].size() == 0) continue;

			RptBlock = _T("_05.Bearing_Head");
			WriteSeisType(RptBlock, iType + 1, iParagraph4, stRow);

			const size_t nGroup = vExistSiet[iSeisDir][iType].size();
			for (int k = 0; k < nGroup; ++k)
			{
				const T_SIET_K SietK = vExistSiet[iSeisDir][iType][k];
				T_SIET_D SietD;
				if (!m_pDoc->m_pAttrCtrl2->GetQSiet()->Get(SietK, SietD)) continue;
				T_SIGR_D SigrD;
				if (!m_pDoc->m_pAttrCtrl2->GetQSigr()->Get(SietD.kSigrLower, SigrD)) continue;

				T_SIET_EVAL_BEAR_EACH eval;
				if (!m_pResult->GetResultSietBearConcEach(SietD.kSipa, SietD.kSigrLower, eval)) eval.Initialize();

				int stRowBefore = stRow;
				int nBearingLine = 2; // line은 2개 고정
				for (int l = 0; l < nBearingLine; ++l)
				{
					const auto& BearingLineD = l == 0 ? eval.Line1 : eval.Line2;

					double dSumUmax = 0.0;
					int nBearing = BearingLineD.aGroup.GetSize();
					if (nBearing < 1) continue;

					for (int m = 0; m < nBearing; ++m)
					{
						const auto& GroupD = BearingLineD.aGroup[m];
						for (int c = 0; c < enCuncurrent_Num; ++c)
						{
							const auto& ForceD = c == 0 ? GroupD.max : GroupD.min;

							if (k == 0 && l == 0 && m == 0 && c == 0)
								RptBlock = _T("_05.Bearing_Body_st");
							else if (k == nGroup - 1 && l == nBearingLine - 1 && m == nBearing - 1 && c == enCuncurrent_Hmin)
								RptBlock = _T("_05.Bearing_Body_ed");
							else
								RptBlock = _T("_05.Bearing_Body_mid");
							rowCnt = CopyRange(pXL, RptBlock, stRow);

							//===============================================================
							// Set data
							double dUmax = ForceD.dUmax * m_dLenToMM;
							dSumUmax = max(dSumUmax, dUmax);

							pXL->SetValue(stRow, 6, strCuncurrent[c]);
							pXL->SetValue(stRow, 7, ForceD.dHmax);
							pXL->SetValue(stRow, 9, ForceD.dVmax);
							pXL->SetValue(stRow, 11, dUmax);
							//===============================================================

							if (c == enCuncurrent_Hmin)
							{
								for (int nC = 3; nC <= 5; ++nC)
								{
									if (k == (nGroup - 1) && l == (nBearingLine - 1) && m == (nBearing - 1))
									{
										auto* pFormat = pXL->GetSheet()->cellFormat(stRow, nC);
										pXL->GetSheet()->setCellFormat(stRow - 1, nC, pFormat);
									}
									pXL->SetMergeCell(stRow - 1, nC, stRow, nC);
								}
								//===============================================================
								// Set data
								pXL->SetValue(stRow - 1, 3, GroupD.strName); // 支承
								pXL->SetValue(stRow - 1, 4, GetText(_T("%d, %d"), GroupD.kNode_i, GroupD.kNode_j)); // 番号
								pXL->SetValue(stRow - 1, 5, GetTextBearingType(SietD.kSipa, GroupD.nType)); // 支承条件
								//===============================================================
							}
							stRow += rowCnt;
						}
						bWrite = TRUE;
						if (m_pProgress->NextStep() == FALSE) throw FALSE;
					}
					const int nMergeCol[4] = { 2, 8, 10, 12 };
					for (int m = 0; m < 4; ++m)
					{
						if (k == nGroup - 1 && l == nBearingLine - 1)
						{
							auto* pFormat = pXL->GetSheet()->cellFormat(stRow - 1, nMergeCol[m]);
							pXL->GetSheet()->setCellFormat(stRow - nBearing * 2, nMergeCol[m], pFormat);
						}
						pXL->SetMergeCell(stRow - nBearing * 2, nMergeCol[m], stRow - 1, nMergeCol[m]);
					}
					//===============================================================
					// Set data
					pXL->SetValue(stRow - nBearing * 2, nMergeCol[0], BearingLineD.nLineNum); // 支承線
					SetValue_Hyphen(stRow - nBearing * 2, nMergeCol[1]); // Hmax 計
					SetValue_Hyphen(stRow - nBearing * 2, nMergeCol[2]); // Vmax 計
					pXL->SetValue(stRow - nBearing * 2, nMergeCol[3], dSumUmax); // Umax 計
					//===============================================================
				}

				for (int m = 0; m < 2; ++m)
				{
					if (k == nGroup - 1)
					{
						auto* pFormat = pXL->GetSheet()->cellFormat(stRow - 1, m);
						pXL->GetSheet()->setCellFormat(stRowBefore, m, pFormat);
					}
					pXL->SetMergeCell(stRowBefore, m, stRow - 1, m);
				}

				pXL->SetValue(stRowBefore, 0, SigrD.strName);
				SetValue_Hyphen(stRowBefore, 1);
			}
			RptBlock = _T("_05.Bearing_Anno");
			rowCnt = CopyRange(pXL, RptBlock, stRow);
			stRow += rowCnt;
			stRow++;
		}
	}
	
#pragma endregion

	return TRUE;
}

BOOL CSeisEvalReportBearings::WriteConcurByLine(IExcel* pXL, const int iParagraph1, int& iParagraph2, int& stRow, BOOL& bWrite)
{
	CBrdgSeisEvalResult_JP* pResult = m_pDoc->m_pPostCtrl->GetBrdgSeisEvalResult_JP(); if (!pResult) { ASSERT(0); return FALSE; }
	if (pResult->GetResultSietBearLineConcMap().size() < 1) return FALSE;

//	5.支承の断面力と変位(3波平均) 받침 단면력과 변위(3파평균)
//		5-2.断面力の算出：支承グループ別の同時性 단면력산출 : 받침그룹별 동시발생
//			5-2-1. 橋軸方向 교축방향
//				1) タイプⅠ
//				2) タイプⅡ
//			5-2-2. 橋軸直角方向 교축직각방향
//				1) タイプⅠ
//				2) タイプⅡ
	auto l_WriteData = [&pXL, this](int nRow, int nCol, double dMax[5], double dMin[5])
	{
		for (int i = 0; i < 5; ++i)
		{
			pXL->SetValue(nRow, nCol + i, dMax[i]);
			pXL->SetValue(nRow + 1, nCol + i, dMin[i]);
		}
	};

	CArray<T_SIET_K, T_SIET_K> aSietK;
	m_pDoc->m_pAttrCtrl2->GetQSiet()->GetKeyList(aSietK);
	const auto nSiet = aSietK.GetSize();
	if (nSiet == 0) return FALSE;

	CString RptBlock;
	int rowCnt = 0;
	short Row_Ref = 0, Col_Ref = 0;

	enum enCuncurrent
	{
		enCuncurrent_Hmax = 0,
		enCuncurrent_Hmin,
		enCuncurrent_Num
	};

	CString strCuncurrent[enCuncurrent_Num] = { _T("Hmax"), _T("Hmin") };

#pragma region // N-2. 断面力の算出：支承グループ別の同時性; 각각
	std::vector<T_SIET_K> vExistSiet[2][2];
	for (int iSiet = 0; iSiet < nSiet; ++iSiet)
	{
		const T_SIET_K SietK = aSietK[iSiet];
		T_SIET_D SietD;
		if (!m_pDoc->m_pAttrCtrl2->GetQSiet()->Get(SietK, SietD))		continue;
		if (SietD.nOutTypeBearing != T_SIET_D::kConcurvity_Line_Br)		continue;
		T_SIPA_D SipaD;
		if (!m_pDoc->m_pAttrCtrl2->GetQSipa()->Get(SietD.kSipa, SipaD)) continue;
		if (SipaD.nDesignCode != m_nDesignCode)							continue;

		const auto iIdx1 = (int)SipaD.nSeismicForceDirection - 1;
		const auto iIdx2 = (int)SipaD.nSeismicForceType - 1;
		vExistSiet[iIdx1][iIdx2].push_back(SietK);
	}

	if (vExistSiet[0][0].size() < 1 && vExistSiet[0][1].size() < 1 &&
		vExistSiet[1][0].size() < 1 && vExistSiet[1][1].size() < 1) return FALSE;

	RptBlock = _T("_05.Bearing_Cuncurrent");
	rowCnt = CopyRange(pXL, RptBlock, stRow);
	pXL->SetValue(stRow, 0, GetText(_T("%d-%d. %s"), iParagraph1, ++iParagraph2, _LS(IDS_BRDG_SEIS_EVAL_REPORT_JP_BEARING_CONCURRENT_LINE)));//断面力の算出：支承グループ別の同時性
	stRow += rowCnt;
	stRow++;

	int iParagraph3 = 0;
	for (int iSeisDir = 0; iSeisDir < 2; ++iSeisDir) // 橋軸方向, 橋軸直角方向
	{
		if (vExistSiet[iSeisDir][0].size() == 0 && vExistSiet[iSeisDir][1].size() == 0) continue;

		RptBlock = _T("_05.Bearing_Dir");
		WriteSeisDir(RptBlock, iSeisDir + 1, iParagraph3, stRow);

		int iParagraph4 = 0;
		for (int iType = 0; iType < 2; ++iType) // タイプ
		{
			if (vExistSiet[iSeisDir][iType].size() == 0) continue;

			RptBlock = _T("_05.Bearing_Head");
			WriteSeisType(RptBlock, iType + 1, iParagraph4, stRow);

			const size_t nGroup = vExistSiet[iSeisDir][iType].size();
			for (int k = 0; k < nGroup; ++k)
			{
				const T_SIET_K SietK = vExistSiet[iSeisDir][iType][k];
				T_SIET_D SietD;
				if (!m_pDoc->m_pAttrCtrl2->GetQSiet()->Get(SietK, SietD)) continue;
				T_SIGR_D SigrD;
				if (!m_pDoc->m_pAttrCtrl2->GetQSigr()->Get(SietD.kSigrLower, SigrD)) continue;

				T_SIET_EVAL_LINE_CONC eval;
				if (!m_pResult->GetResultSietBearLineConc(SietD.kSipa, SietD.kSigrLower, eval)) continue;

				int stRowBefore = stRow;
				int nBearingLine = 2; // line은 2개 고정
				for (int l = 0; l < nBearingLine; ++l)
				{
					const auto& LineD = l == 0 ? eval.Line1 : eval.Line2;
					if (LineD.nLineNum == 0) continue;

					int stRowBearingLine = stRow;
					for (int m = 0; m < 2; ++m) // Max / Min
					{
						const auto& aGroupD = m == 0 ? LineD.aHmax : LineD.aHmin;
						const auto& SumD = m == 0 ? LineD.SumMax : LineD.SumMin;

						const auto nBearing = aGroupD.GetSize();
						for (int n = 0; n < nBearing; ++n)
						{
							const auto& BearingD = aGroupD[n];

							if (k == 0 && l == 0 && m == 0 && n == 0)
								RptBlock = _T("_05.Bearing_Body_st");
							else if (k == nGroup - 1 && l == nBearingLine - 1 && m == 1 && n == nBearing - 1)
								RptBlock = _T("_05.Bearing_Body_ed");
							else
								RptBlock = _T("_05.Bearing_Body_mid");
							rowCnt = CopyRange(pXL, RptBlock, stRow);

							//===============================================================
							// Set data
							pXL->SetValue(stRow, 3, BearingD.strName); // 支承
							pXL->SetValue(stRow, 4, GetText(_T("%d, %d"), BearingD.kNode_i, BearingD.kNode_j)); // 番号
							pXL->SetValue(stRow, 5, GetTextBearingType(SietD.kSipa, BearingD.nType)); // 支承条件
							pXL->SetValue(stRow, 7, BearingD.Force.dHmax);
							pXL->SetValue(stRow, 9, BearingD.Force.dVmax);
							pXL->SetValue(stRow, 11, BearingD.Force.dUmax * m_dLenToMM);
							//===============================================================

							stRow += rowCnt;
							bWrite = TRUE;
						}
						for (int p = 6; p <= 12; p += 2)
						{
							if (k == nGroup - 1 && l == nBearingLine - 1 && m == 1)
							{
								auto* pFormat = pXL->GetSheet()->cellFormat(stRow - 1, p);
								pXL->GetSheet()->setCellFormat(stRow - nBearing, p, pFormat);
							}
							pXL->SetMergeCell(stRow - nBearing, p, stRow - 1, p);
						}

						//===============================================================
						// Set data
						pXL->SetValue(stRow - nBearing, 6, strCuncurrent[m]); // 同時性
						pXL->SetValue(stRow - nBearing, 8, SumD.dHmax); // Hmax 計
						pXL->SetValue(stRow - nBearing, 10, SumD.dVmax); // Vmax 計
						pXL->SetValue(stRow - nBearing, 12, SumD.dUmax * m_dLenToMM); // Umax 計
						//===============================================================
					}

					if (k == nGroup - 1 && l == nBearingLine - 1)
					{
						auto* pFormat = pXL->GetSheet()->cellFormat(stRow - 1, 2);
						pXL->GetSheet()->setCellFormat(stRowBearingLine, 2, pFormat);
					}
					pXL->SetMergeCell(stRowBearingLine, 2, stRow - 1, 2);

					//===============================================================
					// Set data
					pXL->SetValue(stRowBearingLine, 2, LineD.nLineNum); // 支承線
					//===============================================================

					if (m_pProgress->NextStep() == FALSE) throw FALSE;
				}

				for (int m = 0; m < 2; ++m)
				{
					if (k == nGroup - 1)
					{
						auto* pFormat = pXL->GetSheet()->cellFormat(stRow - 1, m);
						pXL->GetSheet()->setCellFormat(stRowBefore, m, pFormat);
					}
					pXL->SetMergeCell(stRowBefore, m, stRow - 1, m);
				}
				pXL->SetValue(stRowBefore, 0, SigrD.strName);
				SetValue_Hyphen(stRowBefore, 1);
			}
			RptBlock = _T("_05.Bearing_Anno");
			rowCnt = CopyRange(pXL, RptBlock, stRow);
			stRow += rowCnt;
			stRow++;
		}
	}
	
#pragma endregion

	return TRUE;
}

CString CSeisEvalReportBearings::GetTextBearingType(const T_SIPA_K& kSipa, const int& nType)
{
	CString str = _T("--");

	T_SIPA_D sipa;
	if (!m_pDoc->m_pAttrCtrl2->GetQSipa()->Get(kSipa, sipa)) return str;

	switch (nType)
	{
	case T_SIGR_LOWER_BEAR_D::kRigidLink:
	case T_SIGR_LOWER_BEAR_D::kFixed:
		str = _T("F");
		break;
	case T_SIGR_LOWER_BEAR_D::kMovable:
		str = (sipa.nSeismicForceDirection == T_SIPA_D::kAxis_Dir) ? _T("M") : _T("F");
		break;
	case T_SIGR_LOWER_BEAR_D::kElastic:
	case T_SIGR_LOWER_BEAR_D::kSeismicIsolation:
		str = _T("E");
		break;
	default:
		break;
	}
	return str;
}

#pragma endregion