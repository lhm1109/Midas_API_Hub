#include "stdafx.h"
#include "WriteSeisEvalReport_JP.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_PostCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl2.h"
#include "..\wg_db\DB_QUERY_SEIS_CVL_JP.h"
#include "..\wg_db\BrdgSeisEvalResult_JP.h"

#include "DgnExcelPrintDlg.h"

// Summery
//#define SURRENDER_COLOR		RGB(0,112,192)
// MaxDisp
#define BY_WHOLE_MODEL 0
#define BY_PIER_MODEL 1

using namespace dgn::lib;

#pragma region // CSeisEvalReportConditions_H29
CSeisEvalReportConditions_H29::CSeisEvalReportConditions_H29(IExcel* pXL, CDgnProgressDlg* pProgress, const CString& strBasePath, const CString& strBaseBook, const SDgnExcelPrintOpt* pPrintOpt)
	: CSeisEvalReportConditions(pXL, pProgress, strBasePath, strBaseBook, pPrintOpt)
{
}

CSeisEvalReportConditions_H29::~CSeisEvalReportConditions_H29()
{
}

void CSeisEvalReportConditions_H29::Write()
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
	m_pXL->SetValue(7, 3, _T("一般橋")); // or 免震橋
	
	// 橋梁の限界状態
	// - 作用の種類：
	switch (SipaD.nEarthquakeType)
	{
	case T_SIPA_D::kUnKnown: ASSERT(0); break;
	case T_SIPA_D::kFluctuating_No10: m_pXL->SetValue(10, 3, _LS(IDS_IDD_DGN_SEIS_SET_PARAM_DLG_IDC_RADIO_SEIS_EARTHQUAKE2)); break;//変動作用-No.10
	case T_SIPA_D::kAccidental_No11: m_pXL->SetValue(10, 3, _LS(IDS_IDD_DGN_SEIS_SET_PARAM_DLG_IDC_RADIO_SEIS_EARTHQUAKE3)); break;//偶発作用-No.11
	default:
		break;
	}
	// 橋の限界状態：
	switch (SipaD.nLimitType)
	{
	case T_SIPA_D::kLimitState0: ASSERT(0); break;
	case T_SIPA_D::kLimitState1: m_pXL->SetValue(11, 3, _LS(IDS_IDD_DGN_SEIS_SET_PARAM_DLG_IDC_RADIO_SEIS_LIMITSTATE1)); break;//変動作用-No.10
	case T_SIPA_D::kLimitState2: m_pXL->SetValue(11, 3, _LS(IDS_IDD_DGN_SEIS_SET_PARAM_DLG_IDC_RADIO_SEIS_LIMITSTATE2)); break;//偶発作用-No.11
	case T_SIPA_D::kLimitState3: m_pXL->SetValue(11, 3, _LS(IDS_IDD_DGN_SEIS_SET_PARAM_DLG_IDC_RADIO_SEIS_LIMITSTATE3)); break;//偶発作用-No.11
	default:
		break;
	}

	//照査ケース
	int iRow = 15;
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

#pragma endregion

#pragma region // CSeisEvalReportFlexure_H29

CSeisEvalReportFlexure_H29::CSeisEvalReportFlexure_H29(IExcel* pXL, CDgnProgressDlg* pProgress, const CString& strBasePath, const CString& strBaseBook, const SDgnExcelPrintOpt* pPrintOpt)
	: CSeisEvalReportFlexure(pXL, pProgress, strBasePath, strBaseBook, pPrintOpt)
{
}

CSeisEvalReportFlexure_H29::~CSeisEvalReportFlexure_H29()
{
}

void CSeisEvalReportFlexure_H29::WirteTheory(const int& nMomentType, const int& nEarthquakeType, const T_SIGR_D::eSubType& nGroupType, const int iParagraph1, int& iParagraph2, OUT int& stRow)
{
	double dXi1 = 0;
	double dXi2 = 0;
	double dPhiYU = 0;

	CString RptBlock = _T("_02.Flexure_Title");
	int rowCnt = CopyRange(m_pXL, RptBlock, stRow);
	m_pXL->SetValue(stRow, 0, GetText(_T("%d. %s"), iParagraph1, GetTextTitle(nGroupType)));
	stRow += rowCnt;

	switch (nMomentType)
	{
	case T_SIBD_SRSV_D::kMy:
		if (nEarthquakeType == T_SIPA_D::kFluctuating_No10)
		{
			dXi1 = m_tSifaH29.RcMomentMyd.No10[T_SIFA_RC_Myd::kXi1];
			dPhiYU = m_tSifaH29.RcMomentMyd.No10[T_SIFA_RC_Myd::kPhiY];
		}
		else
		{
			dXi1 = m_tSifaH29.RcMomentMyd.No11[T_SIFA_RC_Myd::kXi1];
			dPhiYU = m_tSifaH29.RcMomentMyd.No11[T_SIFA_RC_Myd::kPhiY];
		}
		RptBlock = _T("_02.Flexure_Theory");
		break;
	case T_SIBD_SRSV_D::kMu:
		if (nEarthquakeType == T_SIPA_D::kFluctuating_No10)
		{
			dXi1 = m_tSifaH29.RcMomentMud.No10[T_SIFA_RC_Mud::kXi1];
			dXi2 = m_tSifaH29.RcMomentMud.No10[T_SIFA_RC_Mud::kXi2];
			dPhiYU = m_tSifaH29.RcMomentMud.No10[T_SIFA_RC_Mud::kPhiU];
		}
		else
		{
			dXi1 = m_tSifaH29.RcMomentMud.No11[T_SIFA_RC_Mud::kXi1];
			dXi2 = m_tSifaH29.RcMomentMud.No11[T_SIFA_RC_Mud::kXi2];
			dPhiYU = m_tSifaH29.RcMomentMud.No11[T_SIFA_RC_Mud::kPhiU];
		}
		RptBlock = _T("_02.Flexure_Theory_Ult");
		break;
	default:
		ASSERT(FALSE);
		break;
	}

	rowCnt = CopyRange(m_pXL, RptBlock, stRow);
	m_pXL->SetValue(stRow, 0, GetText(_T("%d-%d.%s"), iParagraph1, ++iParagraph2, _LS(IDS_BRDG_SEIS_EVAL_REPORT_JP_CHK_FLEX)));// 曲げの照査
	
	if (nMomentType == T_SIBD_SRSV_D::kMy)
	{
		m_pXL->SetValue(stRow + 6, 5, GetText(_T("%g"), dXi1)) ;// 曲げの照査
		m_pXL->SetValue(stRow + 7, 5, GetText(_T("%g"), dPhiYU));// 曲げの照査
	}
	else
	{
		m_pXL->SetValue(stRow + 6, 5, GetText(_T("%g"), dXi1));// 曲げの照査
		m_pXL->SetValue(stRow + 7, 5, GetText(_T("%g"), dXi2));// 曲げの照査
		m_pXL->SetValue(stRow + 8, 5, GetText(_T("%g"), dPhiYU));// 曲げの照査
	}

	stRow += rowCnt;
	stRow++;
}
#pragma endregion

#pragma region // CSeisEvalReportShearSus_H29

CSeisEvalReportShearSus_H29::CSeisEvalReportShearSus_H29(IExcel* pXL, CDgnProgressDlg* pProgress, const CString& strBasePath, const CString& strBaseBook, const SDgnExcelPrintOpt* pPrintOpt)
	: CSeisEvalReportShearSus(pXL, pProgress, strBasePath, strBaseBook, pPrintOpt)
{
}

CSeisEvalReportShearSus_H29::~CSeisEvalReportShearSus_H29()
{
}

void CSeisEvalReportShearSus_H29::Write()
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

		for (int mm = 0; mm < 2; ++mm) // max/min
		{
			BOOL bMax = mm == 0 ? TRUE : FALSE;
			int nDirRow = stRow + 1;
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

void CSeisEvalReportShearSus_H29::WirteTheory(const T_SIGR_D::eSubType& nGroupType, const int& nEarthquakeType, const int iParagraph1, int& iParagraph2, OUT int& stRow)
{
	double dXi1 = 0;
	double dXi2 = 0;
	double dPhiUc = 0;
	double dPhiUs = 0;
	double dK = 0;
	if (nEarthquakeType == T_SIPA_D::kFluctuating_No10 )
	{
		dXi1 = m_tSifaH29.RcShearSusd.No10[T_SIFA_RC_Susd::kXi1];
		dXi2 = m_tSifaH29.RcShearSusd.No10[T_SIFA_RC_Susd::kXi2];
		dPhiUc = m_tSifaH29.RcShearSusd.No10[T_SIFA_RC_Susd::kPhi_uc_us];
		dPhiUs = m_tSifaH29.RcShearSusd.No10[T_SIFA_RC_Susd::kXi2_Phi_up];
		dK = m_tSifaH29.RcShearSusd.No10[T_SIFA_RC_Susd::kK];
	}
	else
	{
		dXi1 = m_tSifaH29.RcShearSusd.No11[T_SIFA_RC_Susd::kXi1];
		dXi2 = m_tSifaH29.RcShearSusd.No11[T_SIFA_RC_Susd::kXi2];
		dPhiUc = m_tSifaH29.RcShearSusd.No11[T_SIFA_RC_Susd::kPhi_uc_us];
		dPhiUs = m_tSifaH29.RcShearSusd.No11[T_SIFA_RC_Susd::kXi2_Phi_up];
		dK = m_tSifaH29.RcShearSusd.No11[T_SIFA_RC_Susd::kK];
	}

	CString RptBlock = _T("_02.Shear_Sus_Title");
	int rowCnt = CopyRange(m_pXL, RptBlock, stRow);
	m_pXL->SetValue(stRow, 0, GetText(_T("%d. %s"), iParagraph1, GetTextTitle(nGroupType)));//上部構造の照査
	stRow += rowCnt;

	RptBlock = _T("_02.Shear_Sus_Theory1");
	rowCnt = CopyRange(m_pXL, RptBlock, stRow);
	m_pXL->SetValue(stRow, 0, GetText(_T("%d-%d. %s Susd"), iParagraph1, ++iParagraph2, _LS(IDS_BRDG_SEIS_EVAL_REPORT_JP_SHEAR_STR)));// せん断耐力
	m_pXL->SetValue(stRow + 6, 4, GetText(_T("%g"), dXi1));
	m_pXL->SetValue(stRow + 7, 4, GetText(_T("%g"), dXi2));
	m_pXL->SetValue(stRow + 8, 4, GetText(_T("%g"), dPhiUc));
	m_pXL->SetValue(stRow + 9, 4, GetText(_T("%g"), dPhiUs));
	m_pXL->SetValue(stRow + 10, 4, GetText(_T("%g"), dK));

	stRow += rowCnt;
	stRow++;
}

#pragma endregion

#pragma region // CSeisEvalReportShearSuc_H29

CSeisEvalReportShearSuc_H29::CSeisEvalReportShearSuc_H29(IExcel* pXL, CDgnProgressDlg* pProgress, const CString& strBasePath, const CString& strBaseBook, const SDgnExcelPrintOpt* pPrintOpt)
	: CSeisEvalReportShearSuc(pXL, pProgress, strBasePath, strBaseBook, pPrintOpt)
{
}

CSeisEvalReportShearSuc_H29::~CSeisEvalReportShearSuc_H29()
{
}

void CSeisEvalReportShearSuc_H29::Write()
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
			int nDirRow = stRow + 1;
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

void CSeisEvalReportShearSuc_H29::WirteTheory(const T_SIGR_D::eSubType& nGroupType, const int& nEarthquakeType, const int iParagraph1, int& iParagraph2, OUT int& stRow)
{
	double dXi1 = 0;
	double dPhiUcw = 0;
	double dPhiUp = 0;

	if (nEarthquakeType == T_SIPA_D::kFluctuating_No10)
	{
		dXi1 = m_tSifaH29.RcShearSucd.No10[T_SIFA_RC_Sucd::kXi1];
		dPhiUcw = m_tSifaH29.RcShearSucd.No10[T_SIFA_RC_Sucd::kXi2_Phi_ucw];
		dPhiUp = m_tSifaH29.RcShearSusd.No10[T_SIFA_RC_Susd::kXi2_Phi_up];
	}
	else
	{
		dXi1 = m_tSifaH29.RcShearSucd.No11[T_SIFA_RC_Sucd::kXi1];
		dPhiUcw = m_tSifaH29.RcShearSucd.No11[T_SIFA_RC_Sucd::kXi2_Phi_ucw];
		dPhiUp = m_tSifaH29.RcShearSusd.No11[T_SIFA_RC_Susd::kXi2_Phi_up];
	}

	CString RptBlock = _T("_02.Shear_Suc_Title");
	int rowCnt = CopyRange(m_pXL, RptBlock, stRow);
	m_pXL->SetValue(stRow, 0, GetText(_T("%d. %s"), iParagraph1, GetTextTitle(nGroupType)));
	stRow += rowCnt;

	RptBlock = _T("_02.Shear_Suc_Theory1");
	rowCnt = CopyRange(m_pXL, RptBlock, stRow);
	m_pXL->SetValue(stRow, 0, GetText(_T("%d-%d. %s Sucd"), iParagraph1, ++iParagraph2, _LS(IDS_BRDG_SEIS_EVAL_REPORT_JP_SHEAR_STR)));// せん断耐力
	m_pXL->SetValue(stRow + 5, 4, GetText(_T("%g"), dXi1));
	m_pXL->SetValue(stRow + 6, 11, GetText(_T("%g"), dXi1));
	m_pXL->SetValue(stRow + 7, 11, GetText(_T("%g"), dXi1));

	stRow += rowCnt;
	stRow++;
}

void CSeisEvalReportShearSuc_H29::WriteAxisOne(const BOOL& bMax, IN std::tuple<T_SIPA_K, T_SIGR_K, T_ELEM_K>& tParam,
	OUT int& stRow, OUT BOOL& bWrited)
{
	auto l_WriteData = [this](int nRow, int nCol, bool bAuto, const T_SISR_EVAL_ROAD3_CAPA_COMP& resD)
		{
			if (bAuto)
			{
				m_pXL->SetValue(nRow, 4, resD.dB * m_dLenToMM);
				m_pXL->SetValue(nRow, 5, resD.dD * m_dLenToMM);
				m_pXL->SetValue(nRow, 6, resD.dTaumax * m_dStrToMPa);
				m_pXL->SetValue(nRow, 7, resD.dAp * m_dStrToMPa);
				m_pXL->SetValue(nRow, 8, resD.dSigmape * m_dStrToMPa);
				m_pXL->SetValue(nRow, 9, resD.dSucw);
				m_pXL->SetValue(nRow, 10, resD.dSp);
			}
			m_pXL->SetValue(nRow, 11, resD.dSuc);
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
		CString strCheckDir = sDir == seis_jp::kZ_Dir ? _T("Fz") : _T("Fy");
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

void CSeisEvalReportShearSuc_H29::WriteAxisTwo(const BOOL& bMax, IN std::tuple<T_SIPA_K, T_SIGR_K, T_ELEM_K>& tParam,
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
			m_pXL->SetValue(nRow, 4, resD.dB * m_dLenToMM);
			m_pXL->SetValue(nRow, 5, resD.dD * m_dLenToMM);
			m_pXL->SetValue(nRow, 6, resD.dTaumax * m_dStrToMPa);
			m_pXL->SetValue(nRow, 7, resD.dAp * m_dStrToMPa);
			m_pXL->SetValue(nRow, 8, resD.dSigmape * m_dStrToMPa);
			m_pXL->SetValue(nRow, 9, resD.dSucw);
			m_pXL->SetValue(nRow, 10, resD.dSp);
		}
		m_pXL->SetValue(nRow, 11, resD.dSuc);
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

#pragma endregion

#pragma region // CSeisEvalReportCurvatureRC_H29

CSeisEvalReportCurvatureRC_H29::CSeisEvalReportCurvatureRC_H29(IExcel* pXL, CDgnProgressDlg* pProgress, const CString& strBasePath, const CString& strBaseBook, const SDgnExcelPrintOpt* pPrintOpt)
	: CSeisEvalReportCurvatureRC(pXL, pProgress, strBasePath, strBaseBook, pPrintOpt)
{
}

CSeisEvalReportCurvatureRC_H29::~CSeisEvalReportCurvatureRC_H29()
{
}

void CSeisEvalReportCurvatureRC_H29::WirteTheory(const T_SIGR_D::eSubType& nGroupType, const int& nSurveyType, const int iParagraph1, int& iParagraph2, OUT int& stRow)
{
	CBrdgSeisEvalResult_JP* pResult = m_pDoc->m_pPostCtrl->GetBrdgSeisEvalResult_JP(); if (!pResult) { ASSERT(0); return; }

	// RC部材に対する降伏照査 
	CString RptBlock = (nSurveyType == T_SIBD_CVSV_D::kSurrenderSurvey) ? _T("_03.Curvature_RC_Theory1") : _T("_03.Curvature_RC_Theory2");
	int rowCnt = CopyRange(m_pXL, RptBlock, stRow);
	m_pXL->SetValue(stRow, 0, GetText(_T("%d. %s"), iParagraph1, GetTextTitle(nGroupType)));
	m_pXL->SetValue(stRow + 1, 0, GetText(_T("%d-%d. %s"), iParagraph1, ++iParagraph2, _LS(IDS_BRDG_SEIS_EVAL_REPORT_JP_CHK_FLEX)));// 曲げの照査

	if (nSurveyType == T_SIBD_CVSV_D::kLimitStateSurvey)
	{
		double dXi1 = m_tSifaH29.RcCurvature.Phi_ls2d[T_SIFA_RC_Curv::kXi1];
		double dPhiS = m_tSifaH29.RcCurvature.Phi_ls2d[T_SIFA_RC_Curv::kPhiS];
		double dK = m_tSifaH29.RcCurvature.Phi_ls2d[T_SIFA_RC_Curv::kK];
		m_pXL->SetValue(stRow + 7, 4, dXi1);
		m_pXL->SetValue(stRow + 8, 4, dPhiS);
		m_pXL->SetValue(stRow + 9, 4, dK);
	}

	stRow += rowCnt;
	stRow++;
}
#pragma endregion

#pragma region // CSeisEvalReportShearPs_H29

CSeisEvalReportShearPs_H29::CSeisEvalReportShearPs_H29(IExcel* pXL, CDgnProgressDlg* pProgress, const CString& strBasePath, const CString& strBaseBook, const SDgnExcelPrintOpt* pPrintOpt)
	: CSeisEvalReportShearPs(pXL, pProgress, strBasePath, strBaseBook, pPrintOpt)
{
}

CSeisEvalReportShearPs_H29::~CSeisEvalReportShearPs_H29()
{
}

void CSeisEvalReportShearPs_H29::WirteTheory(const T_SIGR_D::eSubType& nGroupType, const int& nEarthquakeType, const int iParagraph1, int& iParagraph2, OUT int& stRow)
{
	double dXi1 = 0;
	double dXi2 = 0;
	double dPhiUc = 0;
	double dPhiUs = 0;
	double dK = 0;
	if (nEarthquakeType == T_SIPA_D::kFluctuating_No10)
	{
		dXi1 = m_tSifaH29.RcShearSusd.No10[T_SIFA_RC_Susd::kXi1];
		dXi2 = m_tSifaH29.RcShearSusd.No10[T_SIFA_RC_Susd::kXi2];
		dPhiUc = m_tSifaH29.RcShearSusd.No10[T_SIFA_RC_Susd::kPhi_uc_us];
		dPhiUs = m_tSifaH29.RcShearSusd.No10[T_SIFA_RC_Susd::kXi2_Phi_up];
		dK = m_tSifaH29.RcShearSusd.No10[T_SIFA_RC_Susd::kK];
	}
	else
	{
		dXi1 = m_tSifaH29.RcShearSusd.No11[T_SIFA_RC_Susd::kXi1];
		dXi2 = m_tSifaH29.RcShearSusd.No11[T_SIFA_RC_Susd::kXi2];
		dPhiUc = m_tSifaH29.RcShearSusd.No11[T_SIFA_RC_Susd::kPhi_uc_us];
		dPhiUs = m_tSifaH29.RcShearSusd.No11[T_SIFA_RC_Susd::kXi2_Phi_up];
		dK = m_tSifaH29.RcShearSusd.No11[T_SIFA_RC_Susd::kK];
	}

	CString RptBlock = _T("_03.Shear_Ps_Title");
	int rowCnt = CopyRange(m_pXL, RptBlock, stRow);
	m_pXL->SetValue(stRow, 0, GetText(_T("%d. %s"), iParagraph1, GetTextTitle(nGroupType)));
	stRow += rowCnt;

	RptBlock = _T("_03.Shear_Ps_Theory1");
	rowCnt = CopyRange(m_pXL, RptBlock, stRow);
	m_pXL->SetValue(stRow, 0, GetText(_T("%d-%d. %s Ps-Susd"), iParagraph1, ++iParagraph2, _LS(IDS_BRDG_SEIS_EVAL_REPORT_JP_SHEAR_STR))); //せん断耐力
	m_pXL->SetValue(stRow + 5, 4, GetText(_T("%g"), dXi1));
	m_pXL->SetValue(stRow + 6, 4, GetText(_T("%g"), dXi2));
	m_pXL->SetValue(stRow + 7, 4, GetText(_T("%g"), dPhiUc));
	m_pXL->SetValue(stRow + 8, 4, GetText(_T("%g"), dPhiUs));
	m_pXL->SetValue(stRow + 9, 4, GetText(_T("%g"), dK));

	stRow += rowCnt;
	stRow++;
}
#pragma endregion

#pragma region // CSeisEvalReportRebarReduction_H29

CSeisEvalReportRebarReduction_H29::CSeisEvalReportRebarReduction_H29(IExcel* pXL, CDgnProgressDlg* pProgress, const CString& strBasePath, const CString& strBaseBook, const SDgnExcelPrintOpt* pPrintOpt)
	: CWriteSeisEvalReportBase(pXL, pProgress, strBasePath, strBaseBook, pPrintOpt)
{
}

CSeisEvalReportRebarReduction_H29::~CSeisEvalReportRebarReduction_H29()
{
}

void CSeisEvalReportRebarReduction_H29::AddStep()
{
	CArray<T_SISR_K, T_SISR_K> aSisrK;
	m_pDoc->m_pAttrCtrl2->GetQSisr()->GetKeyList(aSisrK);
	const int nSisr = aSisrK.GetSize();
	if (nSisr == 0) return;

	std::vector<T_SISR_K> vExistSisr[2];
	for (int i = 0; i < nSisr; ++i)
	{
		const T_SISR_K SisrK = aSisrK[i];
		T_SISR_D SisrD;
		if (!m_pDoc->m_pAttrCtrl2->GetQSisr()->Get(SisrK, SisrD))		continue;
		T_SIGR_D SigrD;
		if (!m_pDoc->m_pAttrCtrl2->GetQSigr()->Get(SisrD.kSigr, SigrD)) continue;
		if (SigrD.nSubType != this->m_subType)							continue;
		T_SIGR_LOWER_D dLower;
		if (!m_pDoc->m_pAttrCtrl2->GetQSigrLower()->Get(SisrD.kSigr, dLower)) continue;;
		if (dLower.COLM_D.GetSize() == 0) continue;
		T_SIPA_D SipaD;
		if (!m_pDoc->m_pAttrCtrl2->GetQSipa()->Get(SisrD.kSipa, SipaD)) continue;
		if (SipaD.nDesignCode != m_nDesignCode)							continue;

		const auto iIdx1 = (int)SipaD.nSeismicForceDirection - 1;
		vExistSisr[iIdx1].push_back(SisrK);
	}

	for (int iSeisDir = 0; iSeisDir < 2; ++iSeisDir) // 橋軸方向, 橋軸直角方向
	{
		if (vExistSisr[iSeisDir].size() == 0) continue;
		
		const int nSeismicForceDirection = iSeisDir == 0 ? T_SIPA_D::kAxis_Dir : T_SIPA_D::kPerp_Dir;
		for (const auto& SisrK : vExistSisr[iSeisDir])
		{
			T_SISR_D SisrD;
			if (!m_pDoc->m_pAttrCtrl2->GetQSisr()->Get(SisrK, SisrD))		continue;

			T_SIGR_D SigrD;
			if (!m_pDoc->m_pAttrCtrl2->GetQSigr()->Get(SisrD.kSigr, SigrD)) continue;

			T_SIPA_D SipaD;
			if (!m_pDoc->m_pAttrCtrl2->GetQSipa()->Get(SisrD.kSipa, SipaD)) continue;

			T_SIGR_LOWER_D dLower;
			if (!m_pDoc->m_pAttrCtrl2->GetQSigrLower()->Get(SisrD.kSigr, dLower)) continue;;

			for (int i = 0; i < dLower.COLM_D.GetCount(); i++)
			{
				const T_SIGR_LOWER_COLM_D& colm = dLower.COLM_D[i];
				if (colm.bRebarCut == FALSE) continue;

				int nPos = nSeismicForceDirection == T_SIPA_D::kAxis_Dir ? colm.nAxialLoc : colm.nAxisPerpLoc;
				T_ELEM_K kElem = nSeismicForceDirection == T_SIPA_D::kAxis_Dir ? colm.kAxialElem : colm.kAxisPerpElem;

				T_SIBD_EVAL_REBAR_CUT eval;
				eval.kSipa = SisrD.kSipa;
				eval.kSigr = SisrD.kSigr;
				eval.kElem = kElem;
				eval.nRebarCutPos = nPos;
				eval.strSigrName = SigrD.strName;
				eval.strColmName = colm.strName;
				eval.nSeismicForceDirection = SipaD.nSeismicForceDirection;
				eval.nSeismicForceType = SipaD.nSeismicForceType;
				eval.nExamShearDir = colm.nDirShear;

				if(!m_pResult->GetResultSibdRebarCut(eval)) continue;

				m_pProgress->m_nTotalSteps++;
			}
		}
	}
}

void CSeisEvalReportRebarReduction_H29::AddSheet(CString& rSheetName)
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
}

void CSeisEvalReportRebarReduction_H29::WirteTheory(const int iParagraph1, int& iParagraph2, OUT int& stRow)
{
	CString RptBlock = _T("_03.RebarReduction_Title");
	int rowCnt = CopyRange(m_pXL, RptBlock, stRow);
	m_pXL->SetValue(stRow, 0, GetText(_T("%d. %s"), iParagraph1, _LS(IDS_BRDG_SEIS_EVAL_REPORT_JP_LOWER_STRUCT))); // 下部構造の降伏照査
	m_pXL->SetValue(stRow + 1, 0, GetText(_T("%d-%d. %s"), iParagraph1, ++iParagraph2, _LS(IDS_BRDG_SEIS_EVAL_REPORT_JP_CHK_REBAR_REDUCTION)));//段落し部の検討(3波平均)
	stRow += rowCnt;
	
	RptBlock = _T("_03.RebarReduction_Theory1");
	rowCnt = CopyRange(m_pXL, RptBlock, stRow);
	stRow += rowCnt;

	stRow++;
}

void CSeisEvalReportRebarReduction_H29::Write()
{
	//	3. 下部構造の3降伏照査 하부구조항복조사
	//		3-5. 段落し部の検討(3波平均) 단락부의 검토
	//		'N-N-1. 損傷判定 손상 판정
	CArray<T_SIBD_K, T_SIBD_K> aSibdK;
	m_pDoc->m_pAttrCtrl2->GetQSibd()->GetKeyList(aSibdK);
	const int nSibd = aSibdK.GetSize();
	if (nSibd == 0) return;

	std::vector<T_SIBD_K> vExistSibd[2];
	for (int i = 0; i < nSibd; ++i)
	{
		const T_SIBD_K SibdK = aSibdK[i];
		T_SIBD_D SibdD;
		if (!m_pDoc->m_pAttrCtrl2->GetQSibd()->Get(SibdK, SibdD))		continue;
		T_SIGR_D SigrD;
		if (!m_pDoc->m_pAttrCtrl2->GetQSigr()->Get(SibdD.kSigr, SigrD)) continue;
		if (SigrD.nSubType != this->m_subType)							continue;
		T_SIGR_LOWER_D dLower;
		if (!m_pDoc->m_pAttrCtrl2->GetQSigrLower()->Get(SibdD.kSigr, dLower)) continue;;
		if (dLower.COLM_D.GetSize() == 0) continue;
		T_SIPA_D SipaD;
		if (!m_pDoc->m_pAttrCtrl2->GetQSipa()->Get(SibdD.kSipa, SipaD)) continue;
		if (SipaD.nDesignCode != m_nDesignCode)							continue;

		const auto iIdx1 = (int)SipaD.nSeismicForceDirection - 1;
		vExistSibd[iIdx1].push_back(SibdK);
	}

	int stRow = 0;
	CString strSheetName;

	AddSheet(strSheetName);
	WirteTheory(*this->m_pParagraph1, *this->m_pParagraph2, stRow);

	WriteCommon(vExistSibd, stRow, strSheetName);
}

void CSeisEvalReportRebarReduction_H29::WriteCommon(const std::vector<T_SISR_K>(&vExistSibd)[2], const int& nRow, const CString& strSheetName)
{
	CString RptBlock;
	BOOL bWrited = FALSE;
	int rowCnt = 0;
	int stRow = nRow;
	int iParagraph3 = 0;
	for (int iSeisDir = 0; iSeisDir < 2; ++iSeisDir) // 橋軸方向, 橋軸直角方向
	{
		if (vExistSibd[iSeisDir].size() == 0) continue;

		const int nSeismicForceDirection = iSeisDir == 0 ? T_SIPA_D::kAxis_Dir : T_SIPA_D::kPerp_Dir;
		RptBlock = _T("_03.RebarReduction_Dir");
		rowCnt = CopyRange(m_pXL, RptBlock, stRow);
		m_pXL->SetValue(stRow, 0, GetText(_T("%d-%d-%d. %s"), *this->m_pParagraph1, *this->m_pParagraph2, ++iParagraph3,
			nSeismicForceDirection == T_SIPA_D::kAxis_Dir ? _LS(IDS_BRDG_SEIS_EVAL_REPORT_JP_LONGI) : _LS(IDS_BRDG_SEIS_EVAL_REPORT_JP_TRANS)));
		stRow += rowCnt;

		RptBlock = _T("_03.RebarReduction_Head1");
		rowCnt = CopyRange(m_pXL, RptBlock, stRow);
		stRow += rowCnt;

		for (const auto& SibdK : vExistSibd[iSeisDir])
		{
			T_SIBD_D SibdD;
			if (!m_pDoc->m_pAttrCtrl2->GetQSibd()->Get(SibdK, SibdD))		continue;

			T_SIGR_D SigrD;
			if (!m_pDoc->m_pAttrCtrl2->GetQSigr()->Get(SibdD.kSigr, SigrD)) continue;

			T_SIPA_D SipaD;
			if (!m_pDoc->m_pAttrCtrl2->GetQSipa()->Get(SibdD.kSipa, SipaD)) continue;

			T_SIGR_LOWER_D dLower;
			if (!m_pDoc->m_pAttrCtrl2->GetQSigrLower()->Get(SibdD.kSigr, dLower)) continue;;

			for (int i = 0; i < dLower.COLM_D.GetCount(); i++)
			{
				const T_SIGR_LOWER_COLM_D& colm = dLower.COLM_D[i];
				if (colm.bRebarCut == FALSE) continue;

				int nPos = nSeismicForceDirection == T_SIPA_D::kAxis_Dir ? colm.nAxialLoc : colm.nAxisPerpLoc;
				T_ELEM_K kElem = nSeismicForceDirection == T_SIPA_D::kAxis_Dir ? colm.kAxialElem : colm.kAxisPerpElem;

				std::tuple<T_SIPA_K, T_SIGR_K, T_ELEM_K, int, CString, CString, int, int> tParam; // 내진 방향 포함
				tParam = std::make_tuple(SibdD.kSipa, SibdD.kSigr, kElem, nPos, SigrD.strName, colm.strName, nSeismicForceDirection, SipaD.nSeismicForceType);

				if (colm.nDirShear == seis_jp::kYZ_Dir)
				{
					WriteAxisTwo(tParam, stRow, bWrited);
				}
				else
				{
					WriteAxisOne(tParam, stRow, bWrited);
				}
			}
		}
		stRow++;
	}

	if (bWrited == FALSE && m_pXL->IsExistSheetName(strSheetName))
		m_pXL->DeleteSheet(strSheetName);
}

void CSeisEvalReportRebarReduction_H29::WriteAxisOne(std::tuple<T_SIPA_K, T_SIGR_K, T_ELEM_K, int, CString, CString, int, int>& tParam, int& stRow, BOOL& bWrited)
{
	int rowCnt = 0;
	BOOL bWriteBody = FALSE;
	for (int iExamShearDir = 0; iExamShearDir < 2; iExamShearDir++) {
		const int sDir = iExamShearDir == 0 ? seis_jp::kY_Dir : seis_jp::kZ_Dir;
		T_SIBD_EVAL_REBAR_CUT eval;
		eval.kSipa = std::get<0>(tParam);
		eval.kSigr = std::get<1>(tParam);
		eval.kElem = std::get<2>(tParam);
		eval.nRebarCutPos = std::get<3>(tParam);
		eval.strSigrName = std::get<4>(tParam);
		eval.strColmName = std::get<5>(tParam);
		eval.nSeismicForceDirection = std::get<6>(tParam);
		eval.nSeismicForceType = std::get<7>(tParam);
		eval.nExamShearDir = sDir;

		if(!m_pResult->GetResultSibdRebarCut(eval)) continue;

		int nRow = stRow;
		if (!bWriteBody)
		{
			CString RptBlock;
			RptBlock = _T("_03.RebarReduction_Body1");
			rowCnt = CopyRange(m_pXL, RptBlock, nRow);

			m_pXL->SetValue(nRow, 0, eval.strSigrName);
			m_pXL->SetValue(nRow, 1, eval.strColmName);
			m_pXL->SetValue(nRow, 2, eval.kElem);
			CString strShearDir = eval.nExamShearDir == seis_jp::kY_Dir ? _LS(IDS_BRDG_SEIS_EVAL_REPORT_JP_SDIR_Y_1) : _LS(IDS_BRDG_SEIS_EVAL_REPORT_JP_SDIR_Z_1);
			m_pXL->SetValue(nRow, 4, strShearDir);
			bWriteBody = TRUE;
		}

		if (eval.nSeismicForceType == T_SIPA_D::kType1)
		{
			WriteData(eval.resultRc, eval.DamageH29Max, eval.DamageH29Min, nRow, bWrited);
		}
		else
		{
			WriteData(eval.resultRc, eval.DamageH29Max, eval.DamageH29Min, nRow +2, bWrited);
		}

	}

	if (bWrited)
		stRow += rowCnt;
}

void CSeisEvalReportRebarReduction_H29::WriteAxisTwo(std::tuple<T_SIPA_K, T_SIGR_K, T_ELEM_K, int, CString, CString, int, int>& tParam, int& stRow, BOOL& bWrited)
{
	int rowCnt = 0;
	BOOL bWriteBody = FALSE;
	for (int iExamShearDir = 0; iExamShearDir < 2; iExamShearDir++) {
		const int sDir = iExamShearDir == 0 ? seis_jp::kY_Dir : seis_jp::kZ_Dir;
		T_SIBD_EVAL_REBAR_CUT eval;
		eval.kSipa = std::get<0>(tParam);
		eval.kSigr = std::get<1>(tParam);
		eval.kElem = std::get<2>(tParam);
		eval.nRebarCutPos = std::get<3>(tParam);
		eval.strSigrName = std::get<4>(tParam);
		eval.strColmName = std::get<5>(tParam);
		eval.nSeismicForceDirection = std::get<6>(tParam);
		eval.nSeismicForceType = std::get<7>(tParam);
		eval.nExamShearDir = sDir;

		if (!m_pResult->GetResultSibdRebarCut(eval)) continue;

		int nRow = stRow;
		if (!bWriteBody)
		{
			CString RptBlock;
			RptBlock = _T("_03.RebarReduction_Body2");
			rowCnt = CopyRange(m_pXL, RptBlock, nRow);

			m_pXL->SetValue(nRow, 0, eval.strSigrName);
			m_pXL->SetValue(nRow, 1, eval.strColmName);
			m_pXL->SetValue(nRow, 2, eval.kElem);
			bWriteBody = TRUE;
		}

		if (eval.nExamShearDir == seis_jp::kY_Dir)
			nRow += 2;

		if (eval.nSeismicForceType == T_SIPA_D::kType1)
		{
			WriteData(eval.resultRc, eval.DamageH29Max, eval.DamageH29Min, nRow, bWrited);
		}
		else
		{
			WriteData(eval.resultRc, eval.DamageH29Max, eval.DamageH29Min, nRow + 4, bWrited);
		}

	}

	if (bWrited)
		stRow += rowCnt;
}

void CSeisEvalReportRebarReduction_H29::WriteData(const T_SIBD_EVAL_RESULT_RC& result, const T_SIBD_EVAL_CALC_DAMAGE_H29& DamageMax, const T_SIBD_EVAL_CALC_DAMAGE_H29& DamageMin, const int& nRow, BOOL& bWrited)
{
	auto l_WriteData = [&](const T_SIBD_EVAL_CALC_DAMAGE_H29& data, const double& dRatio, const int& nRow, const int& nCol) {
		m_pXL->SetValue(nRow, nCol+0, data.dH);
		m_pXL->SetValue(nRow, nCol+1, data.dHi);
		m_pXL->SetValue(nRow, nCol+2, data.dD);
		m_pXL->SetValue(nRow, nCol+3, data.dMyi);
		m_pXL->SetValue(nRow, nCol+4, data.dMyB);
		m_pXL->SetValue(nRow, nCol+5, data.dVal);
		m_pXL->SetValue(nRow, nCol+6, dRatio > 1 ? _T("NG") : _T("OK"));
		if (dRatio > 1)
			SetNG_Red(m_pXL, nRow, nCol + 6, nRow, nCol + 6);
	};
	
	// max
	l_WriteData(DamageMax, result.dRatioMax, nRow, 6);
	// min
	l_WriteData(DamageMin, result.dRatioMin, nRow+1, 6);

	bWrited = TRUE;
	if (m_pProgress->NextStep() == FALSE) throw FALSE;
}
#pragma endregion