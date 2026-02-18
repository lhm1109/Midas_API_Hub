#include "stdafx.h"
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

using namespace dgn::lib;

#define BY_UNKNOWN -1
#define BY_WHOLE_MODEL 0
#define BY_PIER_MODEL 1


#pragma region // CDispWholeModel
CDispWholeModel::CDispWholeModel(IExcel* pXL, CDgnProgressDlg* pProgress, const CString& strBasePath, const CString& strBaseBook, const SDgnExcelPrintOpt* pPrintOpt, const eDispType& DispType)
	: CWriteSeisEvalReportBase(pXL, pProgress, strBasePath, strBaseBook, pPrintOpt)
{
	if (DispType == eDispType::kMaxDisp)
		m_strBaseSheet = _LSX(01.MaxDisp);
	else if (DispType == eDispType::kResidualDisp)
		m_strBaseSheet = _LSX(01.ResDisp);
}

CDispWholeModel::~CDispWholeModel()
{
}

T_SIPA_K CDispWholeModel::GetSimd2SipaKey(const T_SIGR_K& SigrK, const T_SIPA_D::eSeismicForceDirection& enDir, const T_SIPA_D::eSeismicForceType& enType)
{
	CArray<T_SIMD_K, T_SIMD_K> aSimdK;
	m_pDoc->m_pAttrCtrl2->GetQSimd()->GetKeyList(aSimdK);
	const auto nSimd = aSimdK.GetSize();
	if (nSimd == 0) return T_SIPA_K(0);

	for (int i = 0; i < nSimd; ++i)
	{
		const auto& SimdK = aSimdK[i];
		T_SIMD_D SimdD;
		if (!m_pDoc->m_pAttrCtrl2->GetQSimd()->Get(SimdK, SimdD))	continue;
		if (SimdD.kSigr != SigrK)									continue;

		const auto& SipaK = SimdD.kSipa;

		T_SIPA_D SipaD;
		if (!m_pDoc->m_pAttrCtrl2->GetQSipa()->Get(SipaK, SipaD))	continue;
		if (SipaD.nDesignCode != m_nDesignCode)						continue;
		if (SipaD.nSeismicForceDirection != enDir)					continue;
		if (SipaD.nSeismicForceType != enType)						continue;
		if (SipaD.nDesignCode != m_nDesignCode)						continue;

		return SipaK;
	}
	return T_SIPA_K(0);
}

T_SIPA_K CDispWholeModel::GetSird2SipaKey(const T_SIGR_K& SigrK, const T_SIPA_D::eSeismicForceDirection& enDir, const T_SIPA_D::eSeismicForceType& enType)
{
	CArray<T_SIRD_K, T_SIRD_K> aSirdK;
	m_pDoc->m_pAttrCtrl2->GetQSird()->GetKeyList(aSirdK);
	const auto nSird = aSirdK.GetSize();
	if (nSird == 0) return T_SIPA_K(0);

	for (int i = 0; i < nSird; ++i)
	{
		const auto SirdK = aSirdK[i];
		T_SIRD_D SirdD;
		if (!m_pDoc->m_pAttrCtrl2->GetQSird()->Get(SirdK, SirdD))	continue;
		if (SirdD.kSigr != SigrK)									continue;

		const auto SipaK = SirdD.kSipa;

		T_SIPA_D SipaD;
		if (!m_pDoc->m_pAttrCtrl2->GetQSipa()->Get(SipaK, SipaD))	continue;
		if (SipaD.nDesignCode != m_nDesignCode)					continue;
		if (SipaD.nSeismicForceDirection != enDir)					continue;
		if (SipaD.nSeismicForceType != enType)						continue;

		return SipaK;
	}
	return T_SIPA_K(0);
}

double CDispWholeModel::GetDeltaMax(const int& nRspCalcType, const doubleArray& aDeltaMax)
{
	double dValue = 0;
	switch (nRspCalcType)
	{
	case seis_jp::kNEXCO:
		ASSERT(aDeltaMax.GetCount() == seis_jp::kSizeNexco);
		if (aDeltaMax.GetCount() > seis_jp::kDmaxNexco) {
			dValue = aDeltaMax[seis_jp::kDmaxNexco];
		}
		break;

	case seis_jp::kJBEC:
		ASSERT(aDeltaMax.GetCount() == seis_jp::kSizeJbec);
		if (aDeltaMax.GetCount() > seis_jp::kDmaxJbec) {
			dValue = aDeltaMax[seis_jp::kDmaxJbec];
		}
		break;
	}
	return dValue;
};

double CDispWholeModel::GetDeltaA(const int& nDesignCode, const int& nAlwCalcType, const doubleArray& aDeltaA)
{
	auto L_GetDeltaA_H14 = [&](OUT double& rValue) {
		switch (nAlwCalcType)
		{
		case seis_jp::kAnalysisResult:
			ASSERT(aDeltaA.GetCount() == seis_jp::kSizeDa_AnalH14);
			if (aDeltaA.GetCount() == seis_jp::kSizeDa_AnalH14) {
				rValue = aDeltaA[seis_jp::kDa_AnalH14];
			}
			break;
		case seis_jp::kMphi_Dy:
		case seis_jp::kMphi_Dpy:
			ASSERT(aDeltaA.GetCount() == seis_jp::kSizeDa_MPhiH14);
			if (aDeltaA.GetCount() == seis_jp::kSizeDa_MPhiH14) {
				rValue = aDeltaA[seis_jp::kDa_MPhiH14];
			}
			break;
		}
		};

	auto L_GetDeltaA_H24 = [&](OUT double& rValue) {
		switch (nAlwCalcType)
		{
		case seis_jp::kAnalysisResult:
			ASSERT(aDeltaA.GetCount() == seis_jp::kSizeDa_AnalH24);
			if (aDeltaA.GetCount() == seis_jp::kSizeDa_AnalH24) {
				rValue = aDeltaA[seis_jp::kDa_AnalH24];
			}
			break;
		case seis_jp::kMphi_Dy:
		case seis_jp::kMphi_Dpy:
			ASSERT(aDeltaA.GetCount() == seis_jp::kSizeDa_MPhiH24);
			if (aDeltaA.GetCount() == seis_jp::kSizeDa_MPhiH24) {
				rValue = aDeltaA[seis_jp::kDa_MPhiH24];
			}
			break;
		}
		};

	auto L_GetDeltaA_H29 = [&](OUT double& rValue) {
		switch (nAlwCalcType)
		{
		case seis_jp::kAnalysisResult:
			ASSERT(aDeltaA.GetCount() == seis_jp::kSizeDa_AnalH29);
			if (aDeltaA.GetCount() == seis_jp::kSizeDa_AnalH29) {
				rValue = aDeltaA[seis_jp::kDlsd_AnalH29];
			}
			break;
		case seis_jp::kMphi_Dy:
		case seis_jp::kMphi_Dpy:
			ASSERT(aDeltaA.GetCount() == seis_jp::kSizeDa_MPhiH29);
			if (aDeltaA.GetCount() == seis_jp::kSizeDa_MPhiH29) {
				rValue = aDeltaA[seis_jp::kDlsd_MPhiH29];
			}
			break;
		}
		};

	double dValue = 0;
	switch (nDesignCode)
	{
	case BrdgSE_CodeJP::JROAD5_H14:
		L_GetDeltaA_H14(dValue);
		break;
	case BrdgSE_CodeJP::JROAD5_H24:
		L_GetDeltaA_H24(dValue);
		break;
	case BrdgSE_CodeJP::JROAD5_H29:
		L_GetDeltaA_H29(dValue);
		break;
	}
	return dValue;
}

#pragma endregion

#pragma region // CDispPierModel
CDispPierModel::CDispPierModel(IExcel* pXL, CDgnProgressDlg* pProgress, const CString& strBasePath, const CString& strBaseBook, const SDgnExcelPrintOpt* pPrintOpt, const eDispType& DispType)
	: CWriteSeisEvalReportBase(pXL, pProgress, strBasePath, strBaseBook, pPrintOpt)
{
	if (DispType == eDispType::kMaxDisp)
	{
		m_strBaseSheet = _LSX(01.MaxDisp);
		// Nexco
		m_mMaxDispNEXCOTheory.insert({
				{ std::make_pair(T_SIGR_LOWER_D::kRC,    seis_jp::kAnalysisResult), _T("_01.MaxDisp_RC_Pier_H24") },
				{ std::make_pair(T_SIGR_LOWER_D::kRC,    seis_jp::kMphi_Dy),        _T("_01.MaxDisp_RC_Pier_H24") }, // 임시.,.?
				{ std::make_pair(T_SIGR_LOWER_D::kSteel, seis_jp::kAnalysisResult), _T("_01.MaxDisp_S_Pier_NEXCO") },
				{ std::make_pair(T_SIGR_LOWER_D::kSteel, seis_jp::kMphi_Dy),        _T("_01.MaxDisp_S_Pier_MFAI") }
			});
		m_mMaxDispNEXCO.insert({
				{ std::make_pair(T_SIGR_LOWER_D::kRC,    seis_jp::kAnalysisResult), _T("_01.MaxDisp_Body1_pier") },
				{ std::make_pair(T_SIGR_LOWER_D::kRC,    seis_jp::kMphi_Dy),        _T("_01.MaxDisp_Body1_pier") }, // 임시.,.?
				{ std::make_pair(T_SIGR_LOWER_D::kSteel, seis_jp::kAnalysisResult), _T("_01.MaxDisp_Body1_pier") }, // 임시.,.?
				{ std::make_pair(T_SIGR_LOWER_D::kSteel, seis_jp::kMphi_Dy),        _T("_01.MaxDisp_Body1_pier") } // 임시.,.?
			});
		// Jbec
		m_mMaxDispJEBCTheory.insert({
				{ std::make_pair(T_SIGR_LOWER_D::kRC,    seis_jp::kAnalysisResult), _T("_01.MaxDisp_RC_Pier_H24") },
				{ std::make_pair(T_SIGR_LOWER_D::kRC,    seis_jp::kMphi_Dy),        _T("_01.MaxDisp_RC_Pier_H24") }, // 임시.,.?
				{ std::make_pair(T_SIGR_LOWER_D::kSteel, seis_jp::kAnalysisResult), _T("_01.MaxDisp_S_Pier_JBEC") },
				{ std::make_pair(T_SIGR_LOWER_D::kSteel, seis_jp::kMphi_Dy),        _T("_01.MaxDisp_S_Pier_MFAI") }
			});
		m_mMaxDispJEBC.insert({
				{ std::make_pair(T_SIGR_LOWER_D::kRC,    seis_jp::kAnalysisResult), _T("_01.MaxDisp_Body1_pier") },
				{ std::make_pair(T_SIGR_LOWER_D::kRC,    seis_jp::kMphi_Dy),        _T("_01.MaxDisp_Body1_pier") }, // 임시.,.?
				{ std::make_pair(T_SIGR_LOWER_D::kSteel, seis_jp::kAnalysisResult), _T("_01.MaxDisp_Body1_pier") }, // 임시.,.?
				{ std::make_pair(T_SIGR_LOWER_D::kSteel, seis_jp::kMphi_Dy),        _T("_01.MaxDisp_Body1_pier") } // 임시.,.?
			});
	}
	else if (DispType == eDispType::kResidualDisp)
	{
		m_strBaseSheet = _LSX(01.ResDisp);
		// Nexco
		m_mResidualDispNEXCOTheory.insert({
				{ std::make_pair(T_SIGR_LOWER_D::kRC,    seis_jp::kAnalysisResult), _T("_01.ResDisp_Pier_NEXCO") },
				{ std::make_pair(T_SIGR_LOWER_D::kRC,    seis_jp::kMphi_Dy),        _T("_01.ResDisp_Pier_NEXCO") }, // 임시.,.?
				{ std::make_pair(T_SIGR_LOWER_D::kSteel, seis_jp::kAnalysisResult), _T("_01.ResDisp_Pier_NEXCO") }, // 임시.,.?
				{ std::make_pair(T_SIGR_LOWER_D::kSteel, seis_jp::kMphi_Dy),        _T("_01.ResDisp_Pier_NEXCO") } // 임시.,.?
			});
		m_mResidualDispNEXCO.insert({
				{ std::make_pair(T_SIGR_LOWER_D::kRC,    seis_jp::kAnalysisResult), _T("_01.ResDisp_Body2") },
				{ std::make_pair(T_SIGR_LOWER_D::kRC,    seis_jp::kMphi_Dy),        _T("_01.ResDisp_Body2") }, // 임시.,.?
				{ std::make_pair(T_SIGR_LOWER_D::kSteel, seis_jp::kAnalysisResult), _T("_01.ResDisp_Body2") }, // 임시.,.?
				{ std::make_pair(T_SIGR_LOWER_D::kSteel, seis_jp::kMphi_Dy),        _T("_01.ResDisp_Body2") }, // 임시.,.?
			});
		// Jbec
		m_mResidualDispJEBCTheory.insert({
				{ std::make_pair(T_SIGR_LOWER_D::kRC,    seis_jp::kAnalysisResult), _T("_01.ResDisp_Pier_JBEC") },
				{ std::make_pair(T_SIGR_LOWER_D::kRC,    seis_jp::kMphi_Dy),        _T("_01.ResDisp_Pier_JBEC") }, // 임시.,.?
				{ std::make_pair(T_SIGR_LOWER_D::kSteel, seis_jp::kAnalysisResult), _T("_01.ResDisp_Pier_JBEC") }, // 임시.,.?
				{ std::make_pair(T_SIGR_LOWER_D::kSteel, seis_jp::kMphi_Dy),        _T("_01.ResDisp_Pier_JBEC") } // 임시.,.?
			});
		m_mResidualDispJEBC.insert({
				{ std::make_pair(T_SIGR_LOWER_D::kRC,    seis_jp::kAnalysisResult), _T("_01.ResDisp_Body2") },
				{ std::make_pair(T_SIGR_LOWER_D::kRC,    seis_jp::kMphi_Dy),        _T("_01.ResDisp_Body2")}, // 임시.,.?
				{ std::make_pair(T_SIGR_LOWER_D::kSteel, seis_jp::kAnalysisResult), _T("_01.ResDisp_Body2")},// 임시.,.?
				{ std::make_pair(T_SIGR_LOWER_D::kSteel, seis_jp::kMphi_Dy),        _T("_01.ResDisp_Body2")}// 임시.,.?
			});
	}
	else if (DispType == eDispType::kDispPlasticRat)
	{
		m_strBaseSheet = _LSX(03.DispPlasticRat);
		// Nexco
		m_mDispPlasticRatNEXCOTheory.insert({
				{ std::make_pair(T_SIGR_LOWER_D::kRC,    seis_jp::kAnalysisResult), _T("_03.DispPlasticRat_H24_NEXCO") },
				{ std::make_pair(T_SIGR_LOWER_D::kRC,    seis_jp::kMphi_Dy),        _T("_03.DispPlasticRat_H24_NEXCO") }, // 임시.,.?
				{ std::make_pair(T_SIGR_LOWER_D::kSteel, seis_jp::kAnalysisResult), _T("_03.DispPlasticRat_H24_NEXCO") }, // 임시.,.?
				{ std::make_pair(T_SIGR_LOWER_D::kSteel, seis_jp::kMphi_Dy),        _T("_03.DispPlasticRat_H24_NEXCO") } // 임시.,.?
			});
		m_mDispPlasticRatNEXCO.insert({
				{ std::make_pair(T_SIGR_LOWER_D::kRC,    seis_jp::kAnalysisResult), _T("_03.DispPlasticRat_H24_Body") },
				{ std::make_pair(T_SIGR_LOWER_D::kRC,    seis_jp::kMphi_Dy),        _T("_03.DispPlasticRat_H24_Body") }, // 임시.,.?
				{ std::make_pair(T_SIGR_LOWER_D::kSteel, seis_jp::kAnalysisResult), _T("_03.DispPlasticRat_H24_Body") }, // 임시.,.?
				{ std::make_pair(T_SIGR_LOWER_D::kSteel, seis_jp::kMphi_Dy),        _T("_03.DispPlasticRat_H24_Body") } // 임시.,.?
			});
		// Jbec
		m_mDispPlasticRatJEBCTheory.insert({
				{ std::make_pair(T_SIGR_LOWER_D::kRC,    seis_jp::kAnalysisResult), _T("_03.DispPlasticRat_H24_JBEC") },
				{ std::make_pair(T_SIGR_LOWER_D::kRC,    seis_jp::kMphi_Dy),        _T("_03.DispPlasticRat_H24_JBEC") }, // 임시.,.?
				{ std::make_pair(T_SIGR_LOWER_D::kSteel, seis_jp::kAnalysisResult), _T("_03.DispPlasticRat_H24_JBEC") }, // 임시.,.?
				{ std::make_pair(T_SIGR_LOWER_D::kSteel, seis_jp::kMphi_Dy),        _T("_03.DispPlasticRat_H24_JBEC") } // 임시.,.?
			});
		m_mDispPlasticRatJEBC.insert({
				{ std::make_pair(T_SIGR_LOWER_D::kRC,    seis_jp::kAnalysisResult), _T("_03.DispPlasticRat_H24_Body") },
				{ std::make_pair(T_SIGR_LOWER_D::kRC,    seis_jp::kMphi_Dy),        _T("_03.DispPlasticRat_H24_Body") }, // 임시.,.?
				{ std::make_pair(T_SIGR_LOWER_D::kSteel, seis_jp::kAnalysisResult), _T("_03.DispPlasticRat_H24_Body") }, // 임시.,.?
				{ std::make_pair(T_SIGR_LOWER_D::kSteel, seis_jp::kMphi_Dy),        _T("_03.DispPlasticRat_H24_Body") } // 임시.,.?
			});

	}
}

CDispPierModel::~CDispPierModel()
{
}

T_SIPA_K CDispPierModel::GetSimd2SipaKey(const T_SIGR_K& SigrK, const T_SIPA_D::eSeismicForceDirection& enDir, const T_SIPA_D::eSeismicForceType& enType)
{
	CArray<T_SIMD_K, T_SIMD_K> aSimdK;
	m_pDoc->m_pAttrCtrl2->GetQSimd()->GetKeyList(aSimdK);
	const auto nSimd = aSimdK.GetSize();
	if (nSimd == 0) return T_SIPA_K(0);

	for (int i = 0; i < nSimd; ++i)
	{
		const auto& SimdK = aSimdK[i];
		T_SIMD_D SimdD;
		if (!m_pDoc->m_pAttrCtrl2->GetQSimd()->Get(SimdK, SimdD))	continue;
		if (SimdD.kSigr != SigrK)									continue;

		const auto& SipaK = SimdD.kSipa;

		T_SIPA_D SipaD;
		if (!m_pDoc->m_pAttrCtrl2->GetQSipa()->Get(SipaK, SipaD))	continue;
		if (SipaD.nDesignCode != m_nDesignCode)						continue;
		if (SipaD.nSeismicForceDirection != enDir)					continue;
		if (SipaD.nSeismicForceType != enType)						continue;
		if (SipaD.nDesignCode != m_nDesignCode)						continue;

		return SipaK;
	}
	return T_SIPA_K(0);
}

T_SIPA_K CDispPierModel::GetSird2SipaKey(const T_SIGR_K& SigrK, const T_SIPA_D::eSeismicForceDirection& enDir, const T_SIPA_D::eSeismicForceType& enType)
{
	CArray<T_SIRD_K, T_SIRD_K> aSirdK;
	m_pDoc->m_pAttrCtrl2->GetQSird()->GetKeyList(aSirdK);
	const auto nSird = aSirdK.GetSize();
	if (nSird == 0) return T_SIPA_K(0);

	for (int i = 0; i < nSird; ++i)
	{
		const auto SirdK = aSirdK[i];
		T_SIRD_D SirdD;
		if (!m_pDoc->m_pAttrCtrl2->GetQSird()->Get(SirdK, SirdD))	continue;
		if (SirdD.kSigr != SigrK)									continue;

		const auto SipaK = SirdD.kSipa;

		T_SIPA_D SipaD;
		if (!m_pDoc->m_pAttrCtrl2->GetQSipa()->Get(SipaK, SipaD))	continue;
		if (SipaD.nDesignCode != m_nDesignCode)					continue;
		if (SipaD.nSeismicForceDirection != enDir)					continue;
		if (SipaD.nSeismicForceType != enType)						continue;

		return SipaK;
	}
	return T_SIPA_K(0);
}

T_SIPA_K CDispPierModel::GetSidp2SipaKey(const T_SIGR_K& SigrK, const T_SIPA_D::eSeismicForceDirection& enDir, const T_SIPA_D::eSeismicForceType& enType)
{
	CArray<T_SIDP_K, T_SIDP_K> aSidpK;
	m_pDoc->m_pAttrCtrl2->GetQSidp()->GetKeyList(aSidpK);
	const auto nSidp = aSidpK.GetSize();
	if (nSidp == 0) return T_SIPA_K(0);

	for (int i = 0; i < nSidp; ++i)
	{
		const auto& SidpK = aSidpK[i];
		T_SIDP_D SidpD;
		if (!m_pDoc->m_pAttrCtrl2->GetQSidp()->Get(SidpK, SidpD))	continue;
		if (SidpD.kSigrLower != SigrK)								continue;

		const auto& SipaK = SidpD.kSipa;
		T_SIPA_D SipaD;
		if (!m_pDoc->m_pAttrCtrl2->GetQSipa()->Get(SipaK, SipaD))	continue;
		if (SipaD.nSeismicForceDirection != enDir)					continue;
		if (SipaD.nSeismicForceType != enType)						continue;
		if (SipaD.nDesignCode != m_nDesignCode)						continue;
		return SipaK;
	}
	return T_SIPA_K(0);
}

double CDispPierModel::GetDeltaMax(const int& nRspCalcType, const doubleArray& aDeltaMax)
{
	double dValue = 0;
	switch (nRspCalcType)
	{
	case seis_jp::kNEXCO:
		ASSERT(aDeltaMax.GetCount() == seis_jp::kSizeNexco);
		if (aDeltaMax.GetCount() > seis_jp::kDmaxNexco) {
			dValue = aDeltaMax[seis_jp::kDmaxNexco];
		}
		break;

	case seis_jp::kJBEC:
		ASSERT(aDeltaMax.GetCount() == seis_jp::kSizeJbec);
		if (aDeltaMax.GetCount() > seis_jp::kDmaxJbec) {
			dValue = aDeltaMax[seis_jp::kDmaxJbec];
		}
		break;
	}
	return dValue;
};

double CDispPierModel::GetDeltaA(const int& nDesignCode, const int& nAlwCalcType, const doubleArray& aDeltaA)
{
	auto L_GetDeltaA_H14 = [&](OUT double& rValue) {
		switch (nAlwCalcType)
		{
		case seis_jp::kAnalysisResult:
			ASSERT(aDeltaA.GetCount() == seis_jp::kSizeDa_AnalH14);
			if (aDeltaA.GetCount() == seis_jp::kSizeDa_AnalH14) {
				rValue = aDeltaA[seis_jp::kDa_AnalH14];
			}
			break;
		case seis_jp::kMphi_Dy:
		case seis_jp::kMphi_Dpy:
			ASSERT(aDeltaA.GetCount() == seis_jp::kSizeDa_MPhiH14);
			if (aDeltaA.GetCount() == seis_jp::kSizeDa_MPhiH14) {
				rValue = aDeltaA[seis_jp::kDa_MPhiH14];
			}
			break;
		}
		};

	auto L_GetDeltaA_H24 = [&](OUT double& rValue) {
		switch (nAlwCalcType)
		{
		case seis_jp::kAnalysisResult:
			ASSERT(aDeltaA.GetCount() == seis_jp::kSizeDa_AnalH24);
			if (aDeltaA.GetCount() == seis_jp::kSizeDa_AnalH24) {
				rValue = aDeltaA[seis_jp::kDa_AnalH24];
			}
			break;
		case seis_jp::kMphi_Dy:
		case seis_jp::kMphi_Dpy:
			ASSERT(aDeltaA.GetCount() == seis_jp::kSizeDa_MPhiH24);
			if (aDeltaA.GetCount() == seis_jp::kSizeDa_MPhiH24) {
				rValue = aDeltaA[seis_jp::kDa_MPhiH24];
			}
			break;
		}
		};

	auto L_GetDeltaA_H29 = [&](OUT double& rValue) {
		switch (nAlwCalcType)
		{
		case seis_jp::kAnalysisResult:
			ASSERT(aDeltaA.GetCount() == seis_jp::kSizeDa_AnalH29);
			if (aDeltaA.GetCount() == seis_jp::kSizeDa_AnalH29) {
				rValue = aDeltaA[seis_jp::kDlsd_AnalH29];
			}
			break;
		case seis_jp::kMphi_Dy:
		case seis_jp::kMphi_Dpy:
			ASSERT(aDeltaA.GetCount() == seis_jp::kSizeDa_MPhiH29);
			if (aDeltaA.GetCount() == seis_jp::kSizeDa_MPhiH29) {
				rValue = aDeltaA[seis_jp::kDlsd_MPhiH29];
			}
			break;
		}
		};

	double dValue = 0;
	switch (nDesignCode)
	{
	case BrdgSE_CodeJP::JROAD5_H14:
		L_GetDeltaA_H14(dValue);
		break;
	case BrdgSE_CodeJP::JROAD5_H24:
		L_GetDeltaA_H24(dValue);
		break;
	case BrdgSE_CodeJP::JROAD5_H29:
		L_GetDeltaA_H29(dValue);
		break;
	}
	return dValue;
}

#pragma endregion

#pragma region H24

#pragma region // CMaxDispWhole
CMaxDispWhole::CMaxDispWhole(IExcel* pXL, CDgnProgressDlg* pProgress, const CString& strBasePath, const CString& strBaseBook, const SDgnExcelPrintOpt* pPrintOpt)
	: CDispWholeModel(pXL, pProgress, strBasePath, strBaseBook, pPrintOpt, eDispType::kMaxDisp)
{
}

CMaxDispWhole::~CMaxDispWhole()
{
}

void CMaxDispWhole::Write(const std::vector<T_SIMD_K> mExistData, const int& nSeisDir, int& iParagraph3, int& stRow)
{
	CString RptBlock;
	int rowCnt;
	for (const auto& key : mExistData)
	{
		T_SIMD_K SimdK = key;
		T_SIMD_D SimdD;
		if (!m_pDoc->m_pAttrCtrl2->GetQSimd()->Get(SimdK, SimdD)) continue;
		T_SIPA_D SipaD;
		if (!m_pDoc->m_pAttrCtrl2->GetQSipa()->Get(SimdD.kSipa, SipaD)) continue;
		T_SIGR_D SigrD;
		if (!m_pDoc->m_pAttrCtrl2->GetQSigr()->Get(SimdD.kSigr, SigrD)) continue;

		const T_SIGR_K SigrK = SimdD.kSigr;
		const T_SIPA_K SipaK_I = GetSimd2SipaKey(SigrK, (T_SIPA_D::eSeismicForceDirection)nSeisDir, T_SIPA_D::kType1);
		const T_SIPA_K SipaK_II = GetSimd2SipaKey(SigrK, (T_SIPA_D::eSeismicForceDirection)nSeisDir, T_SIPA_D::kType2);
		if (SipaK_I == 0 && SipaK_II == 0) continue;

		RptBlock = _T("_01.MaxDisp_Body1");
		rowCnt = CopyRange(m_pXL, RptBlock, stRow);
		m_pXL->SetValue(stRow, 0, SigrD.strName);

		// 전체계 -> 좌우 체크
		for (int nPos = T_SIMD_EVAL::kLeft; nPos < T_SIMD_EVAL::kLower; ++nPos) // 좌/우
		{
			T_SIMD_EVAL EvalD_I, EvalD_II;
			bool bExist_I = m_pResult->GetResultSimd(SipaK_I, SigrK, nPos, EvalD_I);
			bool bExist_II = m_pResult->GetResultSimd(SipaK_II, SigrK, nPos, EvalD_II);

			if (bExist_I || bExist_II)
			{
				const auto NodeK = EvalD_I.bCheckResult ? EvalD_I.kGirderNode : EvalD_II.kGirderNode;
				m_pXL->SetValue(stRow, 1, GetText(_T("%s\n(%d)"),
					(nPos == T_SIMD_EVAL::kLeft ? _LS(IDS_BRDG_SEIS_EVAL_REPORT_JP_LEFT) : _LS(IDS_BRDG_SEIS_EVAL_REPORT_JP_RIGHT)), NodeK));// 左端, 右端
			}
			else
			{
				m_pXL->SetValue(stRow, 1, GetText(_T("%s\n(--)"),
					(nPos == T_SIMD_EVAL::kLeft ? _LS(IDS_BRDG_SEIS_EVAL_REPORT_JP_LEFT) : _LS(IDS_BRDG_SEIS_EVAL_REPORT_JP_RIGHT))));// 左端, 右端
			}

			// タイプⅠ
			if (bExist_I)
			{
				T_SIGR_D SigrLowerD;
				int col = 3;
				if (!m_pDoc->m_pAttrCtrl2->GetQSigr()->Get(EvalD_I.kSigrLower, SigrLowerD)) SigrLowerD.Initialize();;
				m_pXL->SetValue(stRow, col++, SigrLowerD.strName);

				WriteData(EvalD_I, col, stRow);
			}

			// タイプⅠⅠ
			if (bExist_II)
			{
				T_SIGR_D SigrLowerD;
				int col = 9;
				if (!m_pDoc->m_pAttrCtrl2->GetQSigr()->Get(EvalD_II.kSigrLower, SigrLowerD)) SigrLowerD.Initialize();
				m_pXL->SetValue(stRow, col++, SigrLowerD.strName);

				WriteData(EvalD_II, col, stRow);
			}
			stRow += (rowCnt / 2);
			if (m_pProgress->NextStep() == FALSE) throw FALSE;
		}
	}
	stRow++;
}

void CMaxDispWhole::WriteData(const T_SIMD_EVAL& EvalD, int& col, int& stRow)
{
	auto l_WriteData = [this](int nRow, int nCol, const T_SIMD_EVAL& simdEval, const T_SIMD_EVAL_DISP& evalDisp)
		{
			m_pXL->SetValue(nRow + 0, nCol, GetDeltaMax(simdEval.nRspCalcType, evalDisp.aDeltaMax) * m_dLenToCM);
			m_pXL->SetValue(nRow + 1, nCol, GetDeltaA(simdEval.nDesignCode, simdEval.nAlwCalcType, evalDisp.aDeltaA) * m_dLenToCM);
			m_pXL->SetValue(nRow + 2, nCol, evalDisp.dRatio);

			if (evalDisp.dRatio > 1.0)
			{
				m_pXL->SetTextColor(nRow + 2, nCol, nRow + 2, nCol, RED);
			}
		};

	const auto nThis = EvalD.aDisplacement.GetSize();
	for (int nThisIdx = 0; nThisIdx < nThis; ++nThisIdx)
	{
		if (nThisIdx == 3) { ASSERT(0);  break; }
		l_WriteData(stRow, col + nThisIdx, EvalD, EvalD.aDisplacement[nThisIdx]);
	}
	col += 3;
	// 평균
	m_pXL->SetValue(stRow, col, GetDeltaMax(EvalD.nRspCalcType, EvalD.Averge.aDeltaMax) * m_dLenToCM);
	m_pXL->SetValue(stRow + 1, col, GetDeltaA(EvalD.nDesignCode, EvalD.nAlwCalcType, EvalD.Averge.aDeltaA) * m_dLenToCM);
	m_pXL->SetValue(stRow + 2, col, EvalD.Averge.dRatio);
	m_pXL->SetValue(stRow + 2, col + 1, EvalD.Averge.dRatio > 1.0 ? _T("NG") : _T("OK"));
	if (EvalD.Averge.dRatio > 1.0) {
		SetNG_Red(m_pXL, stRow + 2, col + 1, stRow + 2, col + 1);
	}
}

void CMaxDispWhole::WirteTheory(const int& nMaterial, const int iParagraph1, int& iParagraph2, OUT int& stRow)
{
	CBrdgSeisEvalResult_JP* pResult = m_pDoc->m_pPostCtrl->GetBrdgSeisEvalResult_JP(); if (!pResult) { ASSERT(0); return; }

	int eCode = 0;
	BOOL bConsiderAlpha = FALSE;
	double dAlpha = 0;

	const std::map<SIMD_EVAL_KEY, T_SIMD_EVAL>& mSimdResult = pResult->GetResultSimdMap();
	std::map<SIMD_EVAL_KEY, T_SIMD_EVAL>::const_iterator iter;
	for (iter = mSimdResult.begin(); iter != mSimdResult.end(); ++iter)
	{
		const SIMD_EVAL_KEY& key = iter->first;
		const T_SIMD_EVAL& data = iter->second;

		if (data.nPosType == T_SIMD_EVAL::kUnKnown) continue;
		if (data.nPosType == T_SIMD_EVAL::kLower) continue;

		T_SIPA_D SipaD;
		if (!m_pDoc->m_pAttrCtrl2->GetQSipa()->Get(data.kSipa, SipaD)) continue;
		if (SipaD.nDesignCode != m_nDesignCode) continue;

		T_SIMD_D simd;
		if (!m_pDoc->m_pAttrCtrl2->GetQSimd()->Get(data.kSimd, simd)) continue;

		eCode = SipaD.nDesignCode;
		bConsiderAlpha = simd.bConsiderAlpha;
		dAlpha = simd.dAlpha;
		break;
	}

	if (eCode == 0) return;

	int nSubRow = 0;
	CString RptBlock;

	GetTheoryRangeName(nMaterial, bConsiderAlpha, RptBlock, nSubRow);

	int rowCnt = CopyRange(m_pXL, RptBlock, stRow);

	// N-1. 最大応答変位の照査 - 全体系
	m_pXL->SetValue(stRow, 0, GetText(_T("%d-%d. %s"), iParagraph1, ++iParagraph2, _LS(IDS_BRDG_SEIS_EVAL_REPORT_JP_CHK_MAX_DISP_ALL)));// 最大応答変位の照査 - 全体系
	if (nMaterial == T_SIGR_LOWER_D::kRC) m_pXL->SetValue(stRow + nSubRow, 4, GetText(_T("%g"), dAlpha));

	stRow += rowCnt;
	stRow++;
}

void CMaxDispWhole::GetTheoryRangeName(const int& nMaterial, const BOOL& bConsiderAlpha, OUT CString& RptBlock, OUT int& nSubRow)
{
	if (nMaterial != T_SIGR_LOWER_D::kRC)
	{
		RptBlock = _T("_01.MaxDisp_S_Global");
	}
	else
	{
		if (m_nDesignCode == BrdgSE_CodeJP::JROAD5_H14) {

			RptBlock = _T("_01.MaxDisp_RC_Global_H14");
			nSubRow = 8;
		}
		else {
			if (bConsiderAlpha) {
				RptBlock = _T("_01.MaxDisp_RC_Global_H24_Eq.24_2");
				nSubRow = 7;
			}
			else {
				RptBlock = _T("_01.MaxDisp_RC_Global_H24_Eq.24_1");
				nSubRow = 9;
			}
		}
	}
}

#pragma endregion

#pragma region // CMaxDispNEXCO_H24
CMaxDispNEXCO_H24::CMaxDispNEXCO_H24(IExcel* pXL, CDgnProgressDlg* pProgress, const CString& strBasePath, const CString& strBaseBook, const SDgnExcelPrintOpt* pPrintOpt)
	: CDispPierModel(pXL, pProgress, strBasePath, strBaseBook, pPrintOpt, eDispType::kMaxDisp)
{
}

CMaxDispNEXCO_H24::~CMaxDispNEXCO_H24()
{
}

void CMaxDispNEXCO_H24::Write(const std::vector<T_SIMD_K> mExistData, const int& nSeisDir, int& iParagraph3, int& stRow)
{
	if (m_strRptBlockBody == _T("")) return;

	CString RptBlock;
	int rowCnt;
	for (const auto& key : mExistData)
	{
		T_SIMD_K SimdK = key;
		T_SIMD_D SimdD;
		if (!m_pDoc->m_pAttrCtrl2->GetQSimd()->Get(SimdK, SimdD)) continue;
		T_SIPA_D SipaD;
		if (!m_pDoc->m_pAttrCtrl2->GetQSipa()->Get(SimdD.kSipa, SipaD)) continue;
		T_SIGR_D SigrD;
		if (!m_pDoc->m_pAttrCtrl2->GetQSigr()->Get(SimdD.kSigr, SigrD)) continue;

		RptBlock = m_strRptBlockBody;
		rowCnt = CopyRange(m_pXL, RptBlock, stRow);
		//대상 그룹
		m_pXL->SetValue(stRow, 0, SigrD.strName);
		//위치
		SetValue_Hyphen(stRow, 1);

		const T_SIGR_K SigrK = SimdD.kSigr;
		const T_SIPA_K SipaK_I = GetSimd2SipaKey(SigrK, (T_SIPA_D::eSeismicForceDirection)nSeisDir, T_SIPA_D::kType1);
		const T_SIPA_K SipaK_II = GetSimd2SipaKey(SigrK, (T_SIPA_D::eSeismicForceDirection)nSeisDir, T_SIPA_D::kType2);
		if (SipaK_I == 0 && SipaK_II == 0) continue;

		T_SIMD_EVAL EvalD_I, EvalD_II;
		bool bExist_I = m_pResult->GetResultSimd(SipaK_I, SigrK, T_SIRD_EVAL::kLower, EvalD_I);
		bool bExist_II = m_pResult->GetResultSimd(SipaK_II, SigrK, T_SIRD_EVAL::kLower, EvalD_II);

		// タイプⅠ
		if (bExist_I)
		{
			int col = 3;
			m_pXL->SetValue(stRow, col++, EvalD_I.strColumnName);
			WriteData(EvalD_I, col, stRow); // RC
			//WriteData(EvalD_I, col, stRow); // S
		}

		// タイプⅠⅠ
		if (bExist_II)
		{
			int col = 9;
			m_pXL->SetValue(stRow, col++, EvalD_II.strColumnName);
			WriteData(EvalD_II, col, stRow); // RC
			//WriteData(EvalD_II, col, stRow); // S
		}

		stRow += rowCnt;
		if (m_pProgress->NextStep() == FALSE) throw FALSE;
	}
	stRow++;
}

void CMaxDispNEXCO_H24::WriteData(const T_SIMD_EVAL& EvalD, int& col, int& stRow)
{
	auto l_WriteData = [this](int nRow, int nCol, const T_SIMD_EVAL& simdEval, const T_SIMD_EVAL_DISP& evalDisp)
		{
			m_pXL->SetValue(nRow + 0, nCol, GetDeltaMax(simdEval.nRspCalcType, evalDisp.aDeltaMax) * m_dLenToCM);
			m_pXL->SetValue(nRow + 1, nCol, GetDeltaA(simdEval.nDesignCode, simdEval.nAlwCalcType, evalDisp.aDeltaA) * m_dLenToCM);
			m_pXL->SetValue(nRow + 2, nCol, evalDisp.dRatio);

			if (evalDisp.dRatio > 1.0)
			{
				m_pXL->SetTextColor(nRow + 2, nCol, nRow + 2, nCol, RED);
			}
		};

	const auto nThis = EvalD.aDisplacement.GetSize();
	for (int nThisIdx = 0; nThisIdx < nThis; ++nThisIdx)
	{
		if (nThisIdx == 3) { ASSERT(0);  break; }
		l_WriteData(stRow, col + nThisIdx, EvalD, EvalD.aDisplacement[nThisIdx]);
	}
	col += 3;
	m_pXL->SetValue(stRow, col, GetDeltaMax(EvalD.nRspCalcType, EvalD.Averge.aDeltaMax) * m_dLenToCM);
	m_pXL->SetValue(stRow + 1, col, GetDeltaA(EvalD.nDesignCode, EvalD.nAlwCalcType, EvalD.Averge.aDeltaA) * m_dLenToCM);
	m_pXL->SetValue(stRow + 2, col, EvalD.Averge.dRatio);
	m_pXL->SetValue(stRow + 2, col + 1, EvalD.Averge.dRatio > 1.0 ? _T("NG") : _T("OK"));
	if (EvalD.Averge.dRatio > 1.0) {
		SetNG_Red(m_pXL, stRow + 2, col + 1, stRow + 2, col + 1);
	}
}

void CMaxDispNEXCO_H24::WirteTheory(const int& nMaterial, const int& nAlwCalcType, const int iParagraph1, int& iParagraph2, OUT int& stRow)
{
	CBrdgSeisEvalResult_JP* pResult = m_pDoc->m_pPostCtrl->GetBrdgSeisEvalResult_JP(); if (!pResult) { ASSERT(0); return; }

	int eCode = 0;
	BOOL bConsiderAlpha = FALSE;
	double dAlpha = 0;

	const std::map<SIMD_EVAL_KEY, T_SIMD_EVAL>& mSimdResult = pResult->GetResultSimdMap();
	std::map<SIMD_EVAL_KEY, T_SIMD_EVAL>::const_iterator iter;
	for (iter = mSimdResult.begin(); iter != mSimdResult.end(); ++iter)
	{
		const SIMD_EVAL_KEY& key = iter->first;
		const T_SIMD_EVAL& data = iter->second;

		if (data.nPosType == T_SIMD_EVAL::kUnKnown) continue;
		if ((data.nPosType == T_SIMD_EVAL::kLeft || data.nPosType == T_SIMD_EVAL::kRight)) continue;

		T_SIPA_D SipaD;
		if (!m_pDoc->m_pAttrCtrl2->GetQSipa()->Get(data.kSipa, SipaD)) continue;
		if (SipaD.nDesignCode != m_nDesignCode) continue;

		T_SIMD_D simd;
		if (!m_pDoc->m_pAttrCtrl2->GetQSimd()->Get(data.kSimd, simd)) continue;

		eCode = SipaD.nDesignCode;
		bConsiderAlpha = simd.bConsiderAlpha;
		dAlpha = simd.dAlpha;
		break;
	}

	if (eCode == 0) return;

	int nSubRow = 0;
	CString RptBlock;

	GetTheoryRangeName(nMaterial, nAlwCalcType, bConsiderAlpha, RptBlock, nSubRow);
	if (RptBlock == _T("")) return;

	int rowCnt = CopyRange(m_pXL, RptBlock, stRow);

	// N-1. 最大応答変位の照査 - 全体系
	m_pXL->SetValue(stRow, 0, GetText(_T("%d-%d. %s"), iParagraph1, ++iParagraph2, _LS(IDS_BRDG_SEIS_EVAL_REPORT_JP_CHK_MAX_DISP_PIER)));// 最大応答変位の照査 - 橋脚別
	if (nMaterial == T_SIGR_LOWER_D::kRC) m_pXL->SetValue(stRow + nSubRow, 4, GetText(_T("%g"), dAlpha));

	stRow += rowCnt;
	stRow++;
}

void CMaxDispNEXCO_H24::GetTheoryRangeName(const int& nMaterial, const int& nAlwCalcType, const BOOL& bConsiderAlpha, OUT CString& RptBlock, OUT int& nSubRow)
{
	// Set Theory
	const auto iter = m_mMaxDispNEXCOTheory.find(std::make_pair((T_SIGR_LOWER_D::eMaterialType)nMaterial, (seis_jp::eAlwCalcType)nAlwCalcType));
	if (iter == m_mMaxDispNEXCOTheory.end()) {
		RptBlock = _T("");
		nSubRow = 0;
		return;
	}
	RptBlock = m_mMaxDispNEXCOTheory[std::make_pair((T_SIGR_LOWER_D::eMaterialType)nMaterial, (seis_jp::eAlwCalcType)nAlwCalcType)];
	if (m_nDesignCode == BrdgSE_CodeJP::JROAD5_H14)  RptBlock = _T("_01.MaxDisp_RC_Pier_H14"); // 임시

	// Set Body
	SetRptBlockBody(nMaterial, nAlwCalcType);

	// Set Sub Row
	if (m_nDesignCode == BrdgSE_CodeJP::JROAD5_H14) {
		nSubRow = 8;
	}
	else
	{
		nSubRow = 7;
	}
}

void CMaxDispNEXCO_H24::SetRptBlockBody(const int& nMaterial, const int& nAlwCalcType)
{
	const auto iter = m_mMaxDispNEXCO.find(std::make_pair((T_SIGR_LOWER_D::eMaterialType)nMaterial, (seis_jp::eAlwCalcType)nAlwCalcType));
	if (iter == m_mMaxDispNEXCO.end()) {
		m_strRptBlockBody = _T("");
		return;
	}

	m_strRptBlockBody = m_mMaxDispNEXCO[std::make_pair((T_SIGR_LOWER_D::eMaterialType)nMaterial, (seis_jp::eAlwCalcType)nAlwCalcType)];
}
#pragma endregion

#pragma region // CMaxDispJBEC_H24
CMaxDispJBEC_H24::CMaxDispJBEC_H24(IExcel* pXL, CDgnProgressDlg* pProgress, const CString& strBasePath, const CString& strBaseBook, const SDgnExcelPrintOpt* pPrintOpt)
	: CDispPierModel(pXL, pProgress, strBasePath, strBaseBook, pPrintOpt, eDispType::kMaxDisp)
{
}

CMaxDispJBEC_H24::~CMaxDispJBEC_H24()
{
}

void CMaxDispJBEC_H24::Write(const std::vector<T_SIMD_K> mExistData, const int& nSeisDir, int& iParagraph3, int& stRow)
{
	if (m_strRptBlockBody == _T("")) return;

	CString RptBlock;
	int rowCnt;
	for (const auto& key : mExistData)
	{
		T_SIMD_K SimdK = key;
		T_SIMD_D SimdD;
		if (!m_pDoc->m_pAttrCtrl2->GetQSimd()->Get(SimdK, SimdD)) continue;
		T_SIPA_D SipaD;
		if (!m_pDoc->m_pAttrCtrl2->GetQSipa()->Get(SimdD.kSipa, SipaD)) continue;
		T_SIGR_D SigrD;
		if (!m_pDoc->m_pAttrCtrl2->GetQSigr()->Get(SimdD.kSigr, SigrD)) continue;

		RptBlock = m_strRptBlockBody;
		rowCnt = CopyRange(m_pXL, RptBlock, stRow);
		//대상 그룹
		m_pXL->SetValue(stRow, 0, SigrD.strName);
		//위치
		SetValue_Hyphen(stRow, 1);

		const T_SIGR_K SigrK = SimdD.kSigr;
		const T_SIPA_K SipaK_I = GetSimd2SipaKey(SigrK, (T_SIPA_D::eSeismicForceDirection)nSeisDir, T_SIPA_D::kType1);
		const T_SIPA_K SipaK_II = GetSimd2SipaKey(SigrK, (T_SIPA_D::eSeismicForceDirection)nSeisDir, T_SIPA_D::kType2);
		if (SipaK_I == 0 && SipaK_II == 0) continue;

		T_SIMD_EVAL EvalD_I, EvalD_II;
		bool bExist_I = m_pResult->GetResultSimd(SipaK_I, SigrK, T_SIRD_EVAL::kLower, EvalD_I);
		bool bExist_II = m_pResult->GetResultSimd(SipaK_II, SigrK, T_SIRD_EVAL::kLower, EvalD_II);

		// タイプⅠ
		if (bExist_I)
		{
			int col = 3;
			m_pXL->SetValue(stRow, col++, EvalD_I.strColumnName);
			WriteData(EvalD_I, col, stRow); // RC
			//WriteData(EvalD_I, col, stRow); // S
		}

		// タイプⅠⅠ
		if (bExist_II)
		{
			int col = 9;
			m_pXL->SetValue(stRow, col++, EvalD_II.strColumnName);
			WriteData(EvalD_II, col, stRow); // RC
			//WriteData(EvalD_II, col, stRow); // S
		}

		stRow += rowCnt;
		if (m_pProgress->NextStep() == FALSE) throw FALSE;
	}
	stRow++;
}

void CMaxDispJBEC_H24::WriteData(const T_SIMD_EVAL& EvalD, int& col, int& stRow)
{
	auto l_WriteData = [this](int nRow, int nCol, const T_SIMD_EVAL& simdEval, const T_SIMD_EVAL_DISP& evalDisp)
		{
			m_pXL->SetValue(nRow + 0, nCol, GetDeltaMax(simdEval.nRspCalcType, evalDisp.aDeltaMax) * m_dLenToCM);
			m_pXL->SetValue(nRow + 1, nCol, GetDeltaA(simdEval.nDesignCode, simdEval.nAlwCalcType, evalDisp.aDeltaA) * m_dLenToCM);
			m_pXL->SetValue(nRow + 2, nCol, evalDisp.dRatio);

			if (evalDisp.dRatio > 1.0)
			{
				m_pXL->SetTextColor(nRow + 2, nCol, nRow + 2, nCol, RED);
			}
		};

	const auto nThis = EvalD.aDisplacement.GetSize();
	for (int nThisIdx = 0; nThisIdx < nThis; ++nThisIdx)
	{
		if (nThisIdx == 3) { ASSERT(0);  break; }
		l_WriteData(stRow, col + nThisIdx, EvalD, EvalD.aDisplacement[nThisIdx]);
	}
	col += 3;
	m_pXL->SetValue(stRow, col, GetDeltaMax(EvalD.nRspCalcType, EvalD.Averge.aDeltaMax) * m_dLenToCM);
	m_pXL->SetValue(stRow + 1, col, GetDeltaA(EvalD.nDesignCode, EvalD.nAlwCalcType, EvalD.Averge.aDeltaA) * m_dLenToCM);
	m_pXL->SetValue(stRow + 2, col, EvalD.Averge.dRatio);
	m_pXL->SetValue(stRow + 2, col + 1, EvalD.Averge.dRatio > 1.0 ? _T("NG") : _T("OK"));
	if (EvalD.Averge.dRatio > 1.0) {
		SetNG_Red(m_pXL, stRow + 2, col + 1, stRow + 2, col);
	}
}

void CMaxDispJBEC_H24::WirteTheory(const int& nMaterial, const int& nAlwCalcType, const int iParagraph1, int& iParagraph2, OUT int& stRow)
{
	CBrdgSeisEvalResult_JP* pResult = m_pDoc->m_pPostCtrl->GetBrdgSeisEvalResult_JP(); if (!pResult) { ASSERT(0); return; }

	int eCode = 0;
	BOOL bConsiderAlpha = FALSE;
	double dAlpha = 0;

	const std::map<SIMD_EVAL_KEY, T_SIMD_EVAL>& mSimdResult = pResult->GetResultSimdMap();
	std::map<SIMD_EVAL_KEY, T_SIMD_EVAL>::const_iterator iter;
	for (iter = mSimdResult.begin(); iter != mSimdResult.end(); ++iter)
	{
		const SIMD_EVAL_KEY& key = iter->first;
		const T_SIMD_EVAL& data = iter->second;

		if (data.nPosType == T_SIMD_EVAL::kUnKnown) continue;
		if ((data.nPosType == T_SIMD_EVAL::kLeft || data.nPosType == T_SIMD_EVAL::kRight)) continue;

		T_SIPA_D SipaD;
		if (!m_pDoc->m_pAttrCtrl2->GetQSipa()->Get(data.kSipa, SipaD)) continue;
		if (SipaD.nDesignCode != m_nDesignCode) continue;

		T_SIMD_D simd;
		if (!m_pDoc->m_pAttrCtrl2->GetQSimd()->Get(data.kSimd, simd)) continue;

		eCode = SipaD.nDesignCode;
		bConsiderAlpha = simd.bConsiderAlpha;
		dAlpha = simd.dAlpha;
		break;
	}

	if (eCode == 0) return;

	int nSubRow = 0;
	CString RptBlock;
	GetTheoryRangeName(nMaterial, nAlwCalcType, bConsiderAlpha, RptBlock, nSubRow);
	if (RptBlock == _T("")) return;

	int rowCnt = CopyRange(m_pXL, RptBlock, stRow);

	// N-1. 最大応答変位の照査 - 橋脚別
	m_pXL->SetValue(stRow, 0, GetText(_T("%d-%d. %s"), iParagraph1, ++iParagraph2, _LS(IDS_BRDG_SEIS_EVAL_REPORT_JP_CHK_MAX_DISP_PIER)));
	if (nMaterial == T_SIGR_LOWER_D::kRC) m_pXL->SetValue(stRow + nSubRow, 4, GetText(_T("%g"), dAlpha));

	stRow += rowCnt;
	stRow++;
}

void CMaxDispJBEC_H24::GetTheoryRangeName(const int& nMaterial, const int& nAlwCalcType, const BOOL& bConsiderAlpha, OUT CString& RptBlock, OUT int& nSubRow)
{
	// Set Theory
	const auto iter = m_mMaxDispJEBCTheory.find(std::make_pair((T_SIGR_LOWER_D::eMaterialType)nMaterial, (seis_jp::eAlwCalcType)nAlwCalcType));
	if (iter == m_mMaxDispJEBCTheory.end()) {
		RptBlock = _T("");
		nSubRow = 0;
		return;
	}
	RptBlock = m_mMaxDispJEBCTheory[std::make_pair((T_SIGR_LOWER_D::eMaterialType)nMaterial, (seis_jp::eAlwCalcType)nAlwCalcType)];
	if (m_nDesignCode == BrdgSE_CodeJP::JROAD5_H14)  RptBlock = _T("_01.MaxDisp_RC_Pier_H14"); // 임시

	// Set Body
	SetRptBlockBody(nMaterial, nAlwCalcType);

	// Set Sub Row
	if (m_nDesignCode == BrdgSE_CodeJP::JROAD5_H14) {
		nSubRow = 8;
	}
	else
	{
		nSubRow = 7;
	}
}

void CMaxDispJBEC_H24::SetRptBlockBody(const int& nMaterial, const int& nAlwCalcType)
{
	const auto iter = m_mMaxDispJEBC.find(std::make_pair((T_SIGR_LOWER_D::eMaterialType)nMaterial, (seis_jp::eAlwCalcType)nAlwCalcType));
	if (iter == m_mMaxDispJEBC.end()) {
		m_strRptBlockBody = _T("");
		return;
	}
	m_strRptBlockBody = m_mMaxDispJEBC[std::make_pair((T_SIGR_LOWER_D::eMaterialType)nMaterial, (seis_jp::eAlwCalcType)nAlwCalcType)];
}
#pragma endregion

#pragma region // CResidualDispWhole
CResidualDispWhole::CResidualDispWhole(IExcel* pXL, CDgnProgressDlg* pProgress, const CString& strBasePath, const CString& strBaseBook, const SDgnExcelPrintOpt* pPrintOpt)
	: CDispWholeModel(pXL, pProgress, strBasePath, strBaseBook, pPrintOpt, eDispType::kResidualDisp)
{
}

CResidualDispWhole::~CResidualDispWhole()
{
}

void CResidualDispWhole::Write(const std::vector<T_SIMD_K> mExistData, const int& nSeisDir, int& iParagraph3, int& stRow)
{
	CString RptBlock;
	int rowCnt;
	for (const auto& key : mExistData)
	{
		T_SIRD_K SirdK = key;
		T_SIRD_D SirdD;
		if (!m_pDoc->m_pAttrCtrl2->GetQSird()->Get(SirdK, SirdD)) continue;
		T_SIPA_D SipaD;
		if (!m_pDoc->m_pAttrCtrl2->GetQSipa()->Get(SirdD.kSipa, SipaD)) continue;
		T_SIGR_D SigrD;
		if (!m_pDoc->m_pAttrCtrl2->GetQSigr()->Get(SirdD.kSigr, SigrD)) continue;

		const T_SIGR_K SigrK = SirdD.kSigr;
		const T_SIPA_K SipaK_I = GetSird2SipaKey(SigrK, (T_SIPA_D::eSeismicForceDirection)nSeisDir, T_SIPA_D::kType1);
		const T_SIPA_K SipaK_II = GetSird2SipaKey(SigrK, (T_SIPA_D::eSeismicForceDirection)nSeisDir, T_SIPA_D::kType2);
		if (SipaK_I == 0 && SipaK_II == 0) continue;

		RptBlock = _T("_01.ResDisp_Body1");
		rowCnt = CopyRange(m_pXL, RptBlock, stRow);
		m_pXL->SetValue(stRow, 0, SigrD.strName);

		// 전체계 -> 좌우 체크
		for (int nPos = T_SIRD_EVAL::kLeft; nPos < T_SIRD_EVAL::kLower; ++nPos) // 좌/우
		{
			T_SIRD_EVAL EvalD_I, EvalD_II;
			bool bExist_I = m_pResult->GetResultSird(SipaK_I, SigrK, nPos, EvalD_I);
			bool bExist_II = m_pResult->GetResultSird(SipaK_II, SigrK, nPos, EvalD_II);

			if (bExist_I || bExist_II)
			{
				const auto NodeK = EvalD_I.bCheckResult ? EvalD_I.kGirderNode : EvalD_II.kGirderNode;
				m_pXL->SetValue(stRow, 1, GetText(_T("%s\n(%d)"),
					(nPos == T_SIRD_EVAL::kLeft ? _LS(IDS_BRDG_SEIS_EVAL_REPORT_JP_LEFT) : _LS(IDS_BRDG_SEIS_EVAL_REPORT_JP_RIGHT)), NodeK));// 左端, 右端
			}
			else
			{
				m_pXL->SetValue(stRow, 1, GetText(_T("%s\n(--)"),
					(nPos == T_SIRD_EVAL::kLeft ? _LS(IDS_BRDG_SEIS_EVAL_REPORT_JP_LEFT) : _LS(IDS_BRDG_SEIS_EVAL_REPORT_JP_RIGHT))));// 左端, 右端
			}

			// タイプⅠ
			if (bExist_I)
			{
				T_SIGR_D SigrLowerD;
				int col = 3;
				if (!m_pDoc->m_pAttrCtrl2->GetQSigr()->Get(EvalD_I.kSigrLower, SigrLowerD)) SigrLowerD.Initialize();;
				m_pXL->SetValue(stRow, col++, SigrLowerD.strName);

				WriteData(EvalD_I, col, stRow);
			}

			// タイプⅠⅠ
			if (bExist_II)
			{
				T_SIGR_D SigrLowerD;
				int col = 9;
				if (!m_pDoc->m_pAttrCtrl2->GetQSigr()->Get(EvalD_II.kSigrLower, SigrLowerD)) SigrLowerD.Initialize();
				m_pXL->SetValue(stRow, col++, SigrLowerD.strName);

				WriteData(EvalD_II, col, stRow);
			}
			stRow += (rowCnt / 2);
			if (m_pProgress->NextStep() == FALSE) throw FALSE;
		}
	}
	RptBlock = _T("_01.ResDisp_Anno1");
	rowCnt = CopyRange(m_pXL, RptBlock, stRow);
	stRow += rowCnt;
	stRow++;
}

void CResidualDispWhole::WriteData(const T_SIRD_EVAL& EvalD, int& col, int& stRow)
{
	auto l_WriteData = [this](int nRow, int nCol, const T_SIRD_EVAL_DISP& evalDisp)
		{
			m_pXL->SetValue(nRow + 0, nCol, evalDisp.dDelta_max * m_dLenToCM);
			m_pXL->SetValue(nRow + 1, nCol, evalDisp.dDelta_y * m_dLenToCM);
			m_pXL->SetValue(nRow + 2, nCol, evalDisp.dMu_r);
			if (evalDisp.dDelta_r == 0)
				SetValue_Hyphen(nRow + 3, nCol);
			else
				m_pXL->SetValue(nRow + 3, nCol, evalDisp.dDelta_r * m_dLenToCM);
			m_pXL->SetValue(nRow + 4, nCol, evalDisp.dDelta_ra * m_dLenToCM);
			if (evalDisp.dRatio == 0)
				SetValue_Hyphen(nRow + 5, nCol);
			else
				m_pXL->SetValue(nRow + 5, nCol, evalDisp.dRatio);
		};

	const auto nThis = EvalD.aDisplacement.GetSize();
	for (int nThisIdx = 0; nThisIdx < nThis; ++nThisIdx)
	{
		if (nThisIdx == 3) { ASSERT(0);  break; }
		l_WriteData(stRow, col + nThisIdx, EvalD.aDisplacement[nThisIdx]);
	}
	col += 3;

	// 평균
	m_pXL->SetValue(stRow + 0, col, EvalD.Averge.dDelta_max * m_dLenToCM);
	m_pXL->SetValue(stRow + 1, col, EvalD.Averge.dDelta_y * m_dLenToCM);
	m_pXL->SetValue(stRow + 2, col, EvalD.Averge.dMu_r);
	if (EvalD.Averge.dDelta_r == 0) SetValue_Hyphen(stRow + 3, col);
	else							  m_pXL->SetValue(stRow + 3, col, EvalD.Averge.dDelta_r * m_dLenToCM);
	m_pXL->SetValue(stRow + 4, col, EvalD.Averge.dDelta_ra * m_dLenToCM);
	if (EvalD.Averge.dRatio == 0.0) SetValue_Hyphen(stRow + 5, col);
	else							 m_pXL->SetValue(stRow + 5, col, EvalD.Averge.dRatio);

	if (EvalD.Averge.dRatio == 0.0) {
		SetValue_Hyphen(stRow + 5, col + 1);
	}
	else {
		m_pXL->SetValue(stRow + 5, col + 1, EvalD.Averge.dRatio > 1.0 ? _T("NG") : _T("OK"));
		if (EvalD.Averge.dRatio > 1.0) SetNG_Red(m_pXL, stRow + 5, col + 1, stRow + 5, col + 1);
	}
}

void CResidualDispWhole::WirteTheory(const int& nMaterial, const int iParagraph1, int& iParagraph2, OUT int& stRow)
{
	CBrdgSeisEvalResult_JP* pResult = m_pDoc->m_pPostCtrl->GetBrdgSeisEvalResult_JP(); if (!pResult) { ASSERT(0); return; }

	double d_Cr = 0, d_r = 0;
	const std::map<SIRD_EVAL_KEY, T_SIRD_EVAL>& mResult = pResult->GetResultSirdMap();
	std::map<SIRD_EVAL_KEY, T_SIRD_EVAL>::const_iterator iter;
	for (iter = mResult.begin(); iter != mResult.end(); ++iter)
	{
		const SIRD_EVAL_KEY& key = iter->first;
		const T_SIRD_EVAL& data = iter->second;
		if (data.aDisplacement.GetCount() < 1) continue;

		if (data.nPosType == T_SIRD_EVAL::kUnKnown) continue;
		if (data.nPosType == T_SIRD_EVAL::kLower) continue;

		T_SIPA_D SipaD;
		if (!m_pDoc->m_pAttrCtrl2->GetQSipa()->Get(data.kSipa, SipaD)) continue;
		if (SipaD.nDesignCode != m_nDesignCode) continue;

		T_SIRD_D sird;
		if (!m_pDoc->m_pAttrCtrl2->GetQSird()->Get(data.kSird, sird)) continue;

		T_SIGR_D sigr;
		if (!m_pDoc->m_pAttrCtrl2->GetQSigr()->Get(data.kSigr, sigr)) continue;

		d_Cr = data.aDisplacement[0].d_Cr;
		d_r = data.aDisplacement[0].d_r;
		break;
	}

	CString RptBlock = _T("_01.ResDisp_Global");
	int rowCnt = CopyRange(m_pXL, RptBlock, stRow);
	m_pXL->SetValue(stRow, 0, GetText(_T("%d-%d. %s"), iParagraph1, ++iParagraph2, _LS(IDS_BRDG_SEIS_EVAL_REPORT_JP_CHK_RES_DISP_ALL)));

	m_pXL->SetValue(stRow + 4, 5, GetText(_T("%g"), d_Cr));
	m_pXL->SetValue(stRow + 5, 7, GetText(_T("%g"), d_r));

	stRow += rowCnt;
	stRow++;
}

#pragma endregion

#pragma region // CResidualDispNEXCO_H24
CResidualDispNEXCO_H24::CResidualDispNEXCO_H24(IExcel* pXL, CDgnProgressDlg* pProgress, const CString& strBasePath, const CString& strBaseBook, const SDgnExcelPrintOpt* pPrintOpt)
	: CDispPierModel(pXL, pProgress, strBasePath, strBaseBook, pPrintOpt, eDispType::kResidualDisp)
{
}

CResidualDispNEXCO_H24::~CResidualDispNEXCO_H24()
{
}

void CResidualDispNEXCO_H24::Write(const std::vector<T_SIMD_K> mExistData, const int& nSeisDir, int& iParagraph3, int& stRow)
{
	if (m_strRptBlockBody == _T("")) return;

	CString RptBlock;
	int rowCnt;
	for (const auto& key : mExistData)
	{
		T_SIRD_K SirdK = key;
		T_SIRD_D SirdD;
		if (!m_pDoc->m_pAttrCtrl2->GetQSird()->Get(SirdK, SirdD)) continue;
		T_SIPA_D SipaD;
		if (!m_pDoc->m_pAttrCtrl2->GetQSipa()->Get(SirdD.kSipa, SipaD)) continue;
		T_SIGR_D SigrD;
		if (!m_pDoc->m_pAttrCtrl2->GetQSigr()->Get(SirdD.kSigr, SigrD)) continue;

		const T_SIGR_K SigrK = SirdD.kSigr;
		const T_SIPA_K SipaK_I = GetSird2SipaKey(SigrK, (T_SIPA_D::eSeismicForceDirection)nSeisDir, T_SIPA_D::kType1);
		const T_SIPA_K SipaK_II = GetSird2SipaKey(SigrK, (T_SIPA_D::eSeismicForceDirection)nSeisDir, T_SIPA_D::kType2);
		if (SipaK_I == 0 && SipaK_II == 0) continue;

		RptBlock = m_strRptBlockBody;
		rowCnt = CopyRange(m_pXL, RptBlock, stRow);
		m_pXL->SetValue(stRow, 0, SigrD.strName);

		T_SIRD_EVAL EvalD_I, EvalD_II;
		bool bExist_I = m_pResult->GetResultSird(SipaK_I, SigrK, T_SIRD_EVAL::kLower, EvalD_I);
		bool bExist_II = m_pResult->GetResultSird(SipaK_II, SigrK, T_SIRD_EVAL::kLower, EvalD_II);

		// タイプⅠ
		if (bExist_I)
		{
			T_SIGR_D SigrLowerD;
			int col = 3;
			m_pXL->SetValue(stRow, col++, EvalD_I.strName);

			WriteData(EvalD_I, col, stRow); // RC
			//WriteData(EvalD_I, col, stRow); // S
		}

		// タイプⅠⅠ
		if (bExist_II)
		{
			T_SIGR_D SigrLowerD;
			int col = 9;
			m_pXL->SetValue(stRow, col++, EvalD_II.strName);

			WriteData(EvalD_II, col, stRow); // RC
			//WriteData(EvalD_II, col, stRow); // S
		}
		stRow += rowCnt;
		if (m_pProgress->NextStep() == FALSE) throw FALSE;
	}
	RptBlock = _T("_01.ResDisp_Anno1");
	rowCnt = CopyRange(m_pXL, RptBlock, stRow);
	stRow += rowCnt;
	stRow++;
}

void CResidualDispNEXCO_H24::WriteData(const T_SIRD_EVAL& EvalD, int& col, int& stRow)
{
	auto l_WriteData = [this](int nRow, int nCol, const T_SIRD_EVAL_DISP& evalDisp)
		{
			m_pXL->SetValue(nRow + 0, nCol, evalDisp.dDelta_max * m_dLenToCM);
			m_pXL->SetValue(nRow + 1, nCol, evalDisp.dDelta_y * m_dLenToCM);
			m_pXL->SetValue(nRow + 2, nCol, evalDisp.dMu_r);
			if (evalDisp.dDelta_r == 0)
				SetValue_Hyphen(nRow + 3, nCol);
			else
				m_pXL->SetValue(nRow + 3, nCol, evalDisp.dDelta_r * m_dLenToCM);
			m_pXL->SetValue(nRow + 4, nCol, evalDisp.dDelta_ra * m_dLenToCM);
			if (evalDisp.dRatio == 0)
				SetValue_Hyphen(nRow + 5, nCol);
			else
				m_pXL->SetValue(nRow + 5, nCol, evalDisp.dRatio);
		};

	const auto nThis = EvalD.aDisplacement.GetSize();
	for (int nThisIdx = 0; nThisIdx < nThis; ++nThisIdx)
	{
		if (nThisIdx == 3) { ASSERT(0);  break; }
		l_WriteData(stRow, col + nThisIdx, EvalD.aDisplacement[nThisIdx]);
	}
	col += 3;

	// 평균
	m_pXL->SetValue(stRow + 0, col, EvalD.Averge.dDelta_max * m_dLenToCM);
	m_pXL->SetValue(stRow + 1, col, EvalD.Averge.dDelta_y * m_dLenToCM);
	m_pXL->SetValue(stRow + 2, col, EvalD.Averge.dMu_r);
	if (EvalD.Averge.dDelta_r == 0) SetValue_Hyphen(stRow + 3, col);
	else							  m_pXL->SetValue(stRow + 3, col, EvalD.Averge.dDelta_r * m_dLenToCM);
	m_pXL->SetValue(stRow + 4, col, EvalD.Averge.dDelta_ra * m_dLenToCM);
	if (EvalD.Averge.dRatio == 0.0) SetValue_Hyphen(stRow + 5, col);
	else							 m_pXL->SetValue(stRow + 5, col, EvalD.Averge.dRatio);

	if (EvalD.Averge.dRatio == 0.0) {
		SetValue_Hyphen(stRow + 5, col + 1);
	}
	else {
		m_pXL->SetValue(stRow + 5, col + 1, EvalD.Averge.dRatio > 1.0 ? _T("NG") : _T("OK"));
		if (EvalD.Averge.dRatio > 1.0) SetNG_Red(m_pXL, stRow + 5, col + 1, stRow + 5, col + 1);
	}
}

void CResidualDispNEXCO_H24::WirteTheory(const int& nMaterial, const int& nAlwCalcType, const int iParagraph1, int& iParagraph2, OUT int& stRow)
{
	CBrdgSeisEvalResult_JP* pResult = m_pDoc->m_pPostCtrl->GetBrdgSeisEvalResult_JP(); if (!pResult) { ASSERT(0); return; }

	double d_Cr = 0, d_r = 0;
	const std::map<SIRD_EVAL_KEY, T_SIRD_EVAL>& mResult = pResult->GetResultSirdMap();
	std::map<SIRD_EVAL_KEY, T_SIRD_EVAL>::const_iterator iter;
	for (iter = mResult.begin(); iter != mResult.end(); ++iter)
	{
		const SIRD_EVAL_KEY& key = iter->first;
		const T_SIRD_EVAL& data = iter->second;
		if (data.aDisplacement.GetCount() < 1) continue;

		if (data.nPosType == T_SIRD_EVAL::kUnKnown) continue;
		if ((data.nPosType == T_SIRD_EVAL::kLeft || data.nPosType == T_SIRD_EVAL::kRight)) continue;

		T_SIPA_D SipaD;
		if (!m_pDoc->m_pAttrCtrl2->GetQSipa()->Get(data.kSipa, SipaD)) continue;
		if (SipaD.nDesignCode != m_nDesignCode) continue;

		T_SIRD_D sird;
		if (!m_pDoc->m_pAttrCtrl2->GetQSird()->Get(data.kSird, sird)) continue;

		T_SIGR_D sigr;
		if (!m_pDoc->m_pAttrCtrl2->GetQSigr()->Get(data.kSigr, sigr)) continue;

		d_Cr = data.aDisplacement[0].d_Cr;
		d_r = data.aDisplacement[0].d_r;
		break;
	}

	// N-1. 最大応答変位の照査 - 全体系
	CString RptBlock;
	GetTheoryRangeName(nMaterial, nAlwCalcType, RptBlock);
	if (RptBlock == _T("")) return;

	int rowCnt = CopyRange(m_pXL, RptBlock, stRow);
	m_pXL->SetValue(stRow, 0, GetText(_T("%d-%d. %s"), iParagraph1, ++iParagraph2, _LS(IDS_BRDG_SEIS_EVAL_REPORT_JP_CHK_RES_DISP_PIER)));
	m_pXL->SetValue(stRow + 4, 5, GetText(_T("%g"), d_Cr));
	m_pXL->SetValue(stRow + 5, 7, GetText(_T("%g"), d_r));

	stRow += rowCnt;
	stRow++;
}

void CResidualDispNEXCO_H24::GetTheoryRangeName(const int& nMaterial, const int& nAlwCalcType, OUT CString& RptBlock)
{
	// Set Theory
	const auto iter = m_mResidualDispNEXCOTheory.find(std::make_pair((T_SIGR_LOWER_D::eMaterialType)nMaterial, (seis_jp::eAlwCalcType)nAlwCalcType));
	if (iter == m_mResidualDispNEXCOTheory.end()) {
		RptBlock = _T("");
		return;
	}
	RptBlock = m_mResidualDispNEXCOTheory[std::make_pair((T_SIGR_LOWER_D::eMaterialType)nMaterial, (seis_jp::eAlwCalcType)nAlwCalcType)];

	// Set Body
	SetRptBlockBody(nMaterial, nAlwCalcType);
}

void CResidualDispNEXCO_H24::SetRptBlockBody(const int& nMaterial, const int& nAlwCalcType)
{
	const auto iter = m_mResidualDispNEXCO.find(std::make_pair((T_SIGR_LOWER_D::eMaterialType)nMaterial, (seis_jp::eAlwCalcType)nAlwCalcType));
	if (iter == m_mResidualDispNEXCO.end()) {
		m_strRptBlockBody = _T("");
		return;
	}
	m_strRptBlockBody = m_mResidualDispNEXCO[std::make_pair((T_SIGR_LOWER_D::eMaterialType)nMaterial, (seis_jp::eAlwCalcType)nAlwCalcType)];
}
#pragma endregion

#pragma region // CResidualDispJBEC_H24
CResidualDispJBEC_H24::CResidualDispJBEC_H24(IExcel* pXL, CDgnProgressDlg* pProgress, const CString& strBasePath, const CString& strBaseBook, const SDgnExcelPrintOpt* pPrintOpt)
	: CDispPierModel(pXL, pProgress, strBasePath, strBaseBook, pPrintOpt, eDispType::kResidualDisp)
{
}

CResidualDispJBEC_H24::~CResidualDispJBEC_H24()
{
}

void CResidualDispJBEC_H24::Write(const std::vector<T_SIMD_K> mExistData, const int& nSeisDir, int& iParagraph3, int& stRow)
{
	if (m_strRptBlockBody == _T("")) return;

	CString RptBlock;
	int rowCnt;
	for (const auto& key : mExistData)
	{
		T_SIRD_K SirdK = key;
		T_SIRD_D SirdD;
		if (!m_pDoc->m_pAttrCtrl2->GetQSird()->Get(SirdK, SirdD)) continue;
		T_SIPA_D SipaD;
		if (!m_pDoc->m_pAttrCtrl2->GetQSipa()->Get(SirdD.kSipa, SipaD)) continue;
		T_SIGR_D SigrD;
		if (!m_pDoc->m_pAttrCtrl2->GetQSigr()->Get(SirdD.kSigr, SigrD)) continue;

		const T_SIGR_K SigrK = SirdD.kSigr;
		const T_SIPA_K SipaK_I = GetSird2SipaKey(SigrK, (T_SIPA_D::eSeismicForceDirection)nSeisDir, T_SIPA_D::kType1);
		const T_SIPA_K SipaK_II = GetSird2SipaKey(SigrK, (T_SIPA_D::eSeismicForceDirection)nSeisDir, T_SIPA_D::kType2);
		if (SipaK_I == 0 && SipaK_II == 0) continue;

		RptBlock = m_strRptBlockBody;
		rowCnt = CopyRange(m_pXL, RptBlock, stRow);
		m_pXL->SetValue(stRow, 0, SigrD.strName);

		T_SIRD_EVAL EvalD_I, EvalD_II;
		bool bExist_I = m_pResult->GetResultSird(SipaK_I, SigrK, T_SIRD_EVAL::kLower, EvalD_I);
		bool bExist_II = m_pResult->GetResultSird(SipaK_II, SigrK, T_SIRD_EVAL::kLower, EvalD_II);

		// タイプⅠ
		if (bExist_I)
		{
			T_SIGR_D SigrLowerD;
			int col = 3;
			m_pXL->SetValue(stRow, col++, EvalD_I.strName);

			WriteData(EvalD_I, col, stRow); // RC
			//WriteData(EvalD_I, col, stRow); // S
		}

		// タイプⅠⅠ
		if (bExist_II)
		{
			T_SIGR_D SigrLowerD;
			int col = 9;
			m_pXL->SetValue(stRow, col++, EvalD_II.strName);

			WriteData(EvalD_II, col, stRow); // RC
			//WriteData(EvalD_II, col, stRow); // S
		}
		stRow += rowCnt;
		if (m_pProgress->NextStep() == FALSE) throw FALSE;
	}
	RptBlock = _T("_01.ResDisp_Anno1");
	rowCnt = CopyRange(m_pXL, RptBlock, stRow);
	stRow += rowCnt;
	stRow++;
}

void CResidualDispJBEC_H24::WriteData(const T_SIRD_EVAL& EvalD, int& col, int& stRow)
{
	auto l_WriteData = [this](int nRow, int nCol, const T_SIRD_EVAL_DISP& evalDisp)
		{
			m_pXL->SetValue(nRow + 0, nCol, evalDisp.dDelta_max * m_dLenToCM);
			m_pXL->SetValue(nRow + 1, nCol, evalDisp.dDelta_y * m_dLenToCM);
			m_pXL->SetValue(nRow + 2, nCol, evalDisp.dMu_r);
			if (evalDisp.dDelta_r == 0)
				SetValue_Hyphen(nRow + 3, nCol);
			else
				m_pXL->SetValue(nRow + 3, nCol, evalDisp.dDelta_r * m_dLenToCM);
			m_pXL->SetValue(nRow + 4, nCol, evalDisp.dDelta_ra * m_dLenToCM);
			if (evalDisp.dRatio == 0)
				SetValue_Hyphen(nRow + 5, nCol);
			else
				m_pXL->SetValue(nRow + 5, nCol, evalDisp.dRatio);
		};

	const auto nThis = EvalD.aDisplacement.GetSize();
	for (int nThisIdx = 0; nThisIdx < nThis; ++nThisIdx)
	{
		if (nThisIdx == 3) { ASSERT(0);  break; }
		l_WriteData(stRow, col + nThisIdx, EvalD.aDisplacement[nThisIdx]);
	}
	col += 3;

	// 평균
	m_pXL->SetValue(stRow + 0, col, EvalD.Averge.dDelta_max * m_dLenToCM);
	m_pXL->SetValue(stRow + 1, col, EvalD.Averge.dDelta_y * m_dLenToCM);
	m_pXL->SetValue(stRow + 2, col, EvalD.Averge.dMu_r);
	if (EvalD.Averge.dDelta_r == 0) SetValue_Hyphen(stRow + 3, col);
	else							  m_pXL->SetValue(stRow + 3, col, EvalD.Averge.dDelta_r * m_dLenToCM);
	m_pXL->SetValue(stRow + 4, col, EvalD.Averge.dDelta_ra * m_dLenToCM);
	if (EvalD.Averge.dRatio == 0.0) SetValue_Hyphen(stRow + 5, col);
	else							 m_pXL->SetValue(stRow + 5, col, EvalD.Averge.dRatio);

	if (EvalD.Averge.dRatio == 0.0) {
		SetValue_Hyphen(stRow + 5, col + 1);
	}
	else {
		m_pXL->SetValue(stRow + 5, col + 1, EvalD.Averge.dRatio > 1.0 ? _T("NG") : _T("OK"));
		if (EvalD.Averge.dRatio > 1.0) SetNG_Red(m_pXL, stRow + 5, col + 1, stRow + 5, col + 1);
	}
}

void CResidualDispJBEC_H24::WirteTheory(const int& nMaterial, const int& nAlwCalcType, const int iParagraph1, int& iParagraph2, OUT int& stRow)
{
	CBrdgSeisEvalResult_JP* pResult = m_pDoc->m_pPostCtrl->GetBrdgSeisEvalResult_JP(); if (!pResult) { ASSERT(0); return; }

	double d_Cr = 0, d_r = 0;
	const std::map<SIRD_EVAL_KEY, T_SIRD_EVAL>& mResult = pResult->GetResultSirdMap();
	std::map<SIRD_EVAL_KEY, T_SIRD_EVAL>::const_iterator iter;
	for (iter = mResult.begin(); iter != mResult.end(); ++iter)
	{
		const SIRD_EVAL_KEY& key = iter->first;
		const T_SIRD_EVAL& data = iter->second;
		if (data.aDisplacement.GetCount() < 1) continue;

		if (data.nPosType == T_SIRD_EVAL::kUnKnown) continue;
		if ((data.nPosType == T_SIRD_EVAL::kLeft || data.nPosType == T_SIRD_EVAL::kRight)) continue;

		T_SIPA_D SipaD;
		if (!m_pDoc->m_pAttrCtrl2->GetQSipa()->Get(data.kSipa, SipaD)) continue;
		if (SipaD.nDesignCode != m_nDesignCode) continue;

		T_SIRD_D sird;
		if (!m_pDoc->m_pAttrCtrl2->GetQSird()->Get(data.kSird, sird)) continue;

		T_SIGR_D sigr;
		if (!m_pDoc->m_pAttrCtrl2->GetQSigr()->Get(data.kSigr, sigr)) continue;

		d_Cr = data.aDisplacement[0].d_Cr;
		d_r = data.aDisplacement[0].d_r;
		break;
	}

	CString RptBlock;
	GetTheoryRangeName(nMaterial, nAlwCalcType, RptBlock);
	if (RptBlock == _T("")) return;

	int rowCnt = CopyRange(m_pXL, RptBlock, stRow);
	m_pXL->SetValue(stRow, 0, GetText(_T("%d-%d. %s"), iParagraph1, ++iParagraph2, _LS(IDS_BRDG_SEIS_EVAL_REPORT_JP_CHK_RES_DISP_PIER)));

	m_pXL->SetValue(stRow + 4, 5, GetText(_T("%g"), d_Cr));
	m_pXL->SetValue(stRow + 5, 7, GetText(_T("%g"), d_r));

	stRow += rowCnt;
	stRow++;
}

void CResidualDispJBEC_H24::GetTheoryRangeName(const int& nMaterial, const int& nAlwCalcType, OUT CString& RptBlock)
{
	// Set Theory
	const auto iter = m_mResidualDispJEBCTheory.find(std::make_pair((T_SIGR_LOWER_D::eMaterialType)nMaterial, (seis_jp::eAlwCalcType)nAlwCalcType));
	if (iter == m_mResidualDispJEBCTheory.end()) {
		RptBlock = _T("");
		return;
	}
	RptBlock = m_mResidualDispJEBCTheory[std::make_pair((T_SIGR_LOWER_D::eMaterialType)nMaterial, (seis_jp::eAlwCalcType)nAlwCalcType)];

	// Set Body
	SetRptBlockBody(nMaterial, nAlwCalcType);
}

void CResidualDispJBEC_H24::SetRptBlockBody(const int& nMaterial, const int& nAlwCalcType)
{
	const auto iter = m_mResidualDispJEBC.find(std::make_pair((T_SIGR_LOWER_D::eMaterialType)nMaterial, (seis_jp::eAlwCalcType)nAlwCalcType));
	if (iter == m_mResidualDispJEBC.end()) {
		m_strRptBlockBody = _T("");
		return;
	}
	m_strRptBlockBody = m_mResidualDispJEBC[std::make_pair((T_SIGR_LOWER_D::eMaterialType)nMaterial, (seis_jp::eAlwCalcType)nAlwCalcType)];
}
#pragma endregion

#pragma region CDispPlasticRatNEXCO_H24
CDispPlasticRatNEXCO_H24::CDispPlasticRatNEXCO_H24(IExcel* pXL, CDgnProgressDlg* pProgress, const CString& strBasePath, const CString& strBaseBook, const SDgnExcelPrintOpt* pPrintOpt)
	: CDispPierModel(pXL, pProgress, strBasePath, strBaseBook, pPrintOpt, eDispType::kDispPlasticRat)
{
}

CDispPlasticRatNEXCO_H24::~CDispPlasticRatNEXCO_H24()
{
}

void CDispPlasticRatNEXCO_H24::Write(const std::vector<T_SIMD_K> mExistData, const int& nSeisDir, int& iParagraph3, int& stRow)
{
	if (m_strRptBlockBody == _T("")) return;

	CString RptBlock;
	int rowCnt;
	for (const auto& key : mExistData)
	{
		T_SIDP_K SidpK = key;
		T_SIDP_D SidpD;
		if (!m_pDoc->m_pAttrCtrl2->GetQSidp()->Get(SidpK, SidpD)) continue;
		T_SIPA_D SipaD;
		if (!m_pDoc->m_pAttrCtrl2->GetQSipa()->Get(SidpD.kSipa, SipaD)) continue;
		T_SIGR_LOWER_D SigrLowerD;
		if (!m_pDoc->m_pAttrCtrl2->GetQSigrLower()->Get(SidpD.kSigrLower, SigrLowerD)) continue;

		const T_SIGR_K SigrK = SigrLowerD.kSigr;
		const T_SIPA_K SipaK_I = GetSidp2SipaKey(SigrK, (T_SIPA_D::eSeismicForceDirection)nSeisDir, T_SIPA_D::kType1);
		const T_SIPA_K SipaK_II = GetSidp2SipaKey(SigrK, (T_SIPA_D::eSeismicForceDirection)nSeisDir, T_SIPA_D::kType2);
		if (SipaK_I == 0 && SipaK_II == 0) continue;

		T_SIGR_D SigrD;
		if (!m_pDoc->m_pAttrCtrl2->GetQSigr()->Get(SigrK, SigrD)) continue;

		RptBlock = m_strRptBlockBody;
		rowCnt = CopyRange(m_pXL, RptBlock, stRow);
		m_pXL->SetValue(stRow, 0, SigrD.strName);
		m_pXL->SetValue(stRow, 1, SidpD.strNameCOLM);

		T_SIDP_EVAL EvalD_I, EvalD_II;
		bool bExist_I = m_pResult->GetResultSidp(SipaK_I, SigrK, EvalD_I);
		bool bExist_II = m_pResult->GetResultSidp(SipaK_II, SigrK, EvalD_II);

		// タイプⅠ
		if (bExist_I)
		{
			int col = 3;
			WriteData(EvalD_I, col, stRow);
		}

		// タイプⅠⅠ
		if (bExist_II)
		{
			int col = 8;
			WriteData(EvalD_II, col, stRow);
		}
		stRow += rowCnt;
		if (m_pProgress->NextStep() == FALSE) throw FALSE;
	}
	stRow++;
}

void CDispPlasticRatNEXCO_H24::WriteData(const T_SIDP_EVAL& EvalD, int& col, int& stRow)
{
	const auto nAnal = min(EvalD.aValByAnal.GetSize(), 3);
	for (int i = 0; i < nAnal; ++i)
	{
		const auto& AnalD = EvalD.aValByAnal[i];
		m_pXL->SetValue(stRow + 0, col + i, AnalD.dDeltaMax * m_dLenToCM);
		m_pXL->SetValue(stRow + 2, col + i, AnalD.dMuR);
		m_pXL->SetValue(stRow + 5, col + i, AnalD.dRatio);
	}
	const auto nCalc = min(EvalD.aValByCalc.GetSize(), 3);
	for (int i = 0; i < nCalc; ++i)
	{
		const auto& CalcID = EvalD.aValByCalc[i];
		m_pXL->SetValue(stRow + 1, col + i, CalcID.dDelta_y * m_dLenToCM);
		m_pXL->SetValue(stRow + 3, col + i, CalcID.dDelta_u_ls * m_dLenToCM);
		m_pXL->SetValue(stRow + 4, col + i, CalcID.dMu_a);
	}

	// 평균
	m_pXL->SetValue(stRow + 0, col + 3, EvalD.AvergeAnal.dDeltaMax * m_dLenToCM);
	m_pXL->SetValue(stRow + 1, col + 3, EvalD.AvergeCalc.dDelta_y * m_dLenToCM);
	m_pXL->SetValue(stRow + 2, col + 3, EvalD.AvergeAnal.dMuR);
	m_pXL->SetValue(stRow + 3, col + 3, EvalD.AvergeCalc.dDelta_u_ls * m_dLenToCM);
	m_pXL->SetValue(stRow + 4, col + 3, EvalD.AvergeCalc.dMu_a);
	m_pXL->SetValue(stRow + 5, col + 3, EvalD.AvergeAnal.dRatio);
	m_pXL->SetValue(stRow + 5, col + 4, EvalD.AvergeAnal.dRatio > 1.0 ? _T("NG") : _T("OK"));
	if (EvalD.AvergeAnal.dRatio > 1.0)
	{
		SetNG_Red(m_pXL, stRow + 5, col + 4, stRow + 5, col + 4);
	}
}

void CDispPlasticRatNEXCO_H24::WirteTheory(const int& nMaterial, const int& nAlwCalcType, const int iParagraph1, int& iParagraph2, OUT int& stRow)
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

	// N-1. 最大応答変位の照査 - 全体系
	CString RptBlock;
	GetTheoryRangeName(nMaterial, nAlwCalcType, RptBlock);
	if (RptBlock == _T("")) return;

	int rowCnt = CopyRange(m_pXL, RptBlock, stRow);
	//N-N. 塑性率の照査
	m_pXL->SetValue(stRow, 0, GetText(_T("%d-%d. %s"), iParagraph1, ++iParagraph2, _LS(IDS_BRDG_SEIS_EVAL_REPORT_JP_CHK_PLAS)));

	//허용소성율 μa
	//CString str;
	//if (eCode == BrdgSE_CodeJP::JROAD5_H24)
	//	str = _LS(IDS_IDD_TM_SIDP_IDC_TM_SIDP_ALLO_DISP_PLAS_RATE2_RDO);//_T("μa=δls/(αδy)");
	//else if (eCode == BrdgSE_CodeJP::JROAD5_H14)
	//	str = _LS(IDS_IDD_TM_SIDP_IDC_TM_SIDP_ALLO_DISP_PLAS_RATE1_RDO);//_T("μa=1+(δls-δy)/(αδy)"); // 임시
	//m_pXL->SetValue(stRow + 7, 3, GetText(_T("%s"), str));

	//안전계수 α
	m_pXL->SetValue(stRow + 8, 3, GetText(_T("%g"), dAlpha));

	stRow += rowCnt;
	stRow++;
}

void CDispPlasticRatNEXCO_H24::GetTheoryRangeName(const int& nMaterial, const int& nAlwCalcType, OUT CString& RptBlock)
{
	// Set Theory
	const auto iter = m_mDispPlasticRatNEXCOTheory.find(std::make_pair((T_SIGR_LOWER_D::eMaterialType)nMaterial, (seis_jp::eAlwCalcType)nAlwCalcType));
	if (iter == m_mDispPlasticRatNEXCOTheory.end()) {
		RptBlock = _T("");
		return;
	}
	RptBlock = m_mDispPlasticRatNEXCOTheory[std::make_pair((T_SIGR_LOWER_D::eMaterialType)nMaterial, (seis_jp::eAlwCalcType)nAlwCalcType)];
	if (m_nDesignCode == BrdgSE_CodeJP::JROAD5_H14) RptBlock = _T("_03.DispPlasticRat_H14_NEXCO"); // 임시

	// Set Body
	SetRptBlockBody(nMaterial, nAlwCalcType);
}

void CDispPlasticRatNEXCO_H24::SetRptBlockBody(const int& nMaterial, const int& nAlwCalcType)
{
	const auto iter = m_mDispPlasticRatNEXCO.find(std::make_pair((T_SIGR_LOWER_D::eMaterialType)nMaterial, (seis_jp::eAlwCalcType)nAlwCalcType));
	if (iter == m_mDispPlasticRatNEXCO.end()) {
		m_strRptBlockBody = _T("");
		return;
	}
	m_strRptBlockBody = m_mDispPlasticRatNEXCO[std::make_pair((T_SIGR_LOWER_D::eMaterialType)nMaterial, (seis_jp::eAlwCalcType)nAlwCalcType)];
}

#pragma endregion

#pragma region CDispPlasticRatJBEC_H24
CDispPlasticRatJBEC_H24::CDispPlasticRatJBEC_H24(IExcel* pXL, CDgnProgressDlg* pProgress, const CString& strBasePath, const CString& strBaseBook, const SDgnExcelPrintOpt* pPrintOpt)
	: CDispPierModel(pXL, pProgress, strBasePath, strBaseBook, pPrintOpt, eDispType::kDispPlasticRat)
{
}

CDispPlasticRatJBEC_H24::~CDispPlasticRatJBEC_H24()
{
}

void CDispPlasticRatJBEC_H24::Write(const std::vector<T_SIMD_K> mExistData, const int& nSeisDir, int& iParagraph3, int& stRow)
{
	if (m_strRptBlockBody == _T("")) return;

	CString RptBlock;
	int rowCnt;
	for (const auto& key : mExistData)
	{
		T_SIDP_K SidpK = key;
		T_SIDP_D SidpD;
		if (!m_pDoc->m_pAttrCtrl2->GetQSidp()->Get(SidpK, SidpD)) continue;
		T_SIPA_D SipaD;
		if (!m_pDoc->m_pAttrCtrl2->GetQSipa()->Get(SidpD.kSipa, SipaD)) continue;
		T_SIGR_LOWER_D SigrLowerD;
		if (!m_pDoc->m_pAttrCtrl2->GetQSigrLower()->Get(SidpD.kSigrLower, SigrLowerD)) continue;

		const T_SIGR_K SigrK = SigrLowerD.kSigr;
		const T_SIPA_K SipaK_I = GetSidp2SipaKey(SigrK, (T_SIPA_D::eSeismicForceDirection)nSeisDir, T_SIPA_D::kType1);
		const T_SIPA_K SipaK_II = GetSidp2SipaKey(SigrK, (T_SIPA_D::eSeismicForceDirection)nSeisDir, T_SIPA_D::kType2);
		if (SipaK_I == 0 && SipaK_II == 0) continue;

		T_SIGR_D SigrD;
		if (!m_pDoc->m_pAttrCtrl2->GetQSigr()->Get(SigrK, SigrD)) continue;

		RptBlock = m_strRptBlockBody;
		rowCnt = CopyRange(m_pXL, RptBlock, stRow);
		m_pXL->SetValue(stRow, 0, SigrD.strName);
		m_pXL->SetValue(stRow, 1, SidpD.strNameCOLM);

		T_SIDP_EVAL EvalD_I, EvalD_II;
		bool bExist_I = m_pResult->GetResultSidp(SipaK_I, SigrK, EvalD_I);
		bool bExist_II = m_pResult->GetResultSidp(SipaK_II, SigrK, EvalD_II);

		// タイプⅠ
		if (bExist_I)
		{
			int col = 3;
			WriteData(EvalD_I, col, stRow);
		}

		// タイプⅠⅠ
		if (bExist_II)
		{
			int col = 8;
			WriteData(EvalD_II, col, stRow);
		}
		stRow += rowCnt;
		if (m_pProgress->NextStep() == FALSE) throw FALSE;
	}
	stRow++;
}

void CDispPlasticRatJBEC_H24::WriteData(const T_SIDP_EVAL& EvalD, int& col, int& stRow)
{
	const auto nAnal = min(EvalD.aValByAnal.GetSize(), 3);
	for (int i = 0; i < nAnal; ++i)
	{
		const auto& AnalD = EvalD.aValByAnal[i];
		m_pXL->SetValue(stRow + 0, col + i, AnalD.dDeltaMax * m_dLenToCM);
		m_pXL->SetValue(stRow + 2, col + i, AnalD.dMuR);
		m_pXL->SetValue(stRow + 5, col + i, AnalD.dRatio);
	}
	const auto nCalc = min(EvalD.aValByCalc.GetSize(), 3);
	for (int i = 0; i < nCalc; ++i)
	{
		const auto& CalcID = EvalD.aValByCalc[i];
		m_pXL->SetValue(stRow + 1, col + i, CalcID.dDelta_y * m_dLenToCM);
		m_pXL->SetValue(stRow + 3, col + i, CalcID.dDelta_u_ls * m_dLenToCM);
		m_pXL->SetValue(stRow + 4, col + i, CalcID.dMu_a);
	}

	// 평균
	m_pXL->SetValue(stRow + 0, col + 3, EvalD.AvergeAnal.dDeltaMax * m_dLenToCM);
	m_pXL->SetValue(stRow + 1, col + 3, EvalD.AvergeCalc.dDelta_y * m_dLenToCM);
	m_pXL->SetValue(stRow + 2, col + 3, EvalD.AvergeAnal.dMuR);
	m_pXL->SetValue(stRow + 3, col + 3, EvalD.AvergeCalc.dDelta_u_ls * m_dLenToCM);
	m_pXL->SetValue(stRow + 4, col + 3, EvalD.AvergeCalc.dMu_a);
	m_pXL->SetValue(stRow + 5, col + 3, EvalD.AvergeAnal.dRatio);
	m_pXL->SetValue(stRow + 5, col + 4, EvalD.AvergeAnal.dRatio > 1.0 ? _T("NG") : _T("OK"));
	if (EvalD.AvergeAnal.dRatio > 1.0)
	{
		SetNG_Red(m_pXL, stRow + 5, col + 4, stRow + 5, col + 4);
	}
}

void CDispPlasticRatJBEC_H24::WirteTheory(const int& nMaterial, const int& nAlwCalcType, const int iParagraph1, int& iParagraph2, OUT int& stRow)
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

	// N-1. 最大応答変位の照査 - 全体系
	CString RptBlock;
	GetTheoryRangeName(nMaterial, nAlwCalcType, RptBlock);
	if (RptBlock == _T("")) return;

	int rowCnt = CopyRange(m_pXL, RptBlock, stRow);
	//N-N. 塑性率の照査
	m_pXL->SetValue(stRow, 0, GetText(_T("%d-%d. %s"), iParagraph1, ++iParagraph2, _LS(IDS_BRDG_SEIS_EVAL_REPORT_JP_CHK_PLAS)));

	//허용소성율 μa
	//CString str;
	//if (eCode == BrdgSE_CodeJP::JROAD5_H24)
	//	str = _LS(IDS_IDD_TM_SIDP_IDC_TM_SIDP_ALLO_DISP_PLAS_RATE2_RDO);//_T("μa=δls/(αδy)");
	//else if (eCode == BrdgSE_CodeJP::JROAD5_H14)
	//	str = _LS(IDS_IDD_TM_SIDP_IDC_TM_SIDP_ALLO_DISP_PLAS_RATE1_RDO);//_T("μa=1+(δls-δy)/(αδy)"); // 임시
	//m_pXL->SetValue(stRow + 7, 3, GetText(_T("%s"), str));

	//안전계수 α
	m_pXL->SetValue(stRow + 8, 3, GetText(_T("%g"), dAlpha));

	stRow += rowCnt;
	stRow++;
}

void CDispPlasticRatJBEC_H24::GetTheoryRangeName(const int& nMaterial, const int& nAlwCalcType, OUT CString& RptBlock)
{
	// Set Theory
	const auto iter = m_mDispPlasticRatJEBCTheory.find(std::make_pair((T_SIGR_LOWER_D::eMaterialType)nMaterial, (seis_jp::eAlwCalcType)nAlwCalcType));
	if (iter == m_mDispPlasticRatJEBCTheory.end()) {
		RptBlock = _T("");
		return;
	}
	RptBlock = m_mDispPlasticRatJEBCTheory[std::make_pair((T_SIGR_LOWER_D::eMaterialType)nMaterial, (seis_jp::eAlwCalcType)nAlwCalcType)];
	if (m_nDesignCode == BrdgSE_CodeJP::JROAD5_H14) RptBlock = _T("_03.DispPlasticRat_H14_JBEC"); // 임시

	// Set Body
	SetRptBlockBody(nMaterial, nAlwCalcType);
}

void CDispPlasticRatJBEC_H24::SetRptBlockBody(const int& nMaterial, const int& nAlwCalcType)
{
	const auto iter = m_mDispPlasticRatJEBC.find(std::make_pair((T_SIGR_LOWER_D::eMaterialType)nMaterial, (seis_jp::eAlwCalcType)nAlwCalcType));
	if (iter == m_mDispPlasticRatJEBC.end()) {
		m_strRptBlockBody = _T("");
		return;
	}
	m_strRptBlockBody = m_mDispPlasticRatJEBC[std::make_pair((T_SIGR_LOWER_D::eMaterialType)nMaterial, (seis_jp::eAlwCalcType)nAlwCalcType)];
}

#pragma endregion

#pragma endregion H24

#pragma region H14

#pragma region // CMaxDispNEXCO_H14
CMaxDispNEXCO_H14::CMaxDispNEXCO_H14(IExcel* pXL, CDgnProgressDlg* pProgress, const CString& strBasePath, const CString& strBaseBook, const SDgnExcelPrintOpt* pPrintOpt)
	: CMaxDispNEXCO_H24(pXL, pProgress, strBasePath, strBaseBook, pPrintOpt)
{
}

CMaxDispNEXCO_H14::~CMaxDispNEXCO_H14()
{
}
#pragma endregion

#pragma region // CMaxDispJBEC_H14
CMaxDispJBEC_H14::CMaxDispJBEC_H14(IExcel* pXL, CDgnProgressDlg* pProgress, const CString& strBasePath, const CString& strBaseBook, const SDgnExcelPrintOpt* pPrintOpt)
	: CMaxDispJBEC_H24(pXL, pProgress, strBasePath, strBaseBook, pPrintOpt)
{
}

CMaxDispJBEC_H14::~CMaxDispJBEC_H14()
{
}
#pragma endregion

#pragma region // CResidualDispNEXCO_H14
CResidualDispNEXCO_H14::CResidualDispNEXCO_H14(IExcel* pXL, CDgnProgressDlg* pProgress, const CString& strBasePath, const CString& strBaseBook, const SDgnExcelPrintOpt* pPrintOpt)
	: CResidualDispNEXCO_H24(pXL, pProgress, strBasePath, strBaseBook, pPrintOpt)
{
}

CResidualDispNEXCO_H14::~CResidualDispNEXCO_H14()
{
}
#pragma endregion

#pragma region // CResidualDispJBEC_H14
CResidualDispJBEC_H14::CResidualDispJBEC_H14(IExcel* pXL, CDgnProgressDlg* pProgress, const CString& strBasePath, const CString& strBaseBook, const SDgnExcelPrintOpt* pPrintOpt)
	: CResidualDispJBEC_H24(pXL, pProgress, strBasePath, strBaseBook, pPrintOpt)
{
}

CResidualDispJBEC_H14::~CResidualDispJBEC_H14()
{
}

#pragma endregion

#pragma region CDispPlasticRatNEXCO_H14
CDispPlasticRatNEXCO_H14::CDispPlasticRatNEXCO_H14(IExcel* pXL, CDgnProgressDlg* pProgress, const CString& strBasePath, const CString& strBaseBook, const SDgnExcelPrintOpt* pPrintOpt)
	: CDispPlasticRatNEXCO_H24(pXL, pProgress, strBasePath, strBaseBook, pPrintOpt)
{
}

CDispPlasticRatNEXCO_H14::~CDispPlasticRatNEXCO_H14()
{
}

#pragma endregion

#pragma region CDispPlasticRatJBEC_H14
CDispPlasticRatJBEC_H14::CDispPlasticRatJBEC_H14(IExcel* pXL, CDgnProgressDlg* pProgress, const CString& strBasePath, const CString& strBaseBook, const SDgnExcelPrintOpt* pPrintOpt)
	: CDispPlasticRatJBEC_H24(pXL, pProgress, strBasePath, strBaseBook, pPrintOpt)
{
}

CDispPlasticRatJBEC_H14::~CDispPlasticRatJBEC_H14()
{
}

#pragma endregion

#pragma endregion H14

#pragma region H29

#pragma region // CMaxDisp H29
CMaxDispWhole_H29::CMaxDispWhole_H29(IExcel* pXL, CDgnProgressDlg* pProgress, const CString& strBasePath, const CString& strBaseBook, const SDgnExcelPrintOpt* pPrintOpt)
	: CMaxDispWhole(pXL, pProgress, strBasePath, strBaseBook, pPrintOpt)
{
}

CMaxDispWhole_H29::~CMaxDispWhole_H29()
{
}

void CMaxDispWhole_H29::WirteTheory(const int& nMaterial, const int iParagraph1, int& iParagraph2, OUT int& stRow)
{
	CBrdgSeisEvalResult_JP* pResult = m_pDoc->m_pPostCtrl->GetBrdgSeisEvalResult_JP(); if (!pResult) { ASSERT(0); return; }

	int eCode = 0;
	BOOL bConsiderAlpha = FALSE;
	double dXi1 = m_tSifaH29.RcPierVal.Delta_yEd[T_SIFA_RC_Disp::kXi1];
	double dPhiS = m_tSifaH29.RcPierVal.Delta_yEd[T_SIFA_RC_Disp::kPhiS];
	double dK = m_tSifaH29.RcPierVal.dFactorK;
	const std::map<SIMD_EVAL_KEY, T_SIMD_EVAL>& mSimdResult = pResult->GetResultSimdMap();
	std::map<SIMD_EVAL_KEY, T_SIMD_EVAL>::const_iterator iter;
	for (iter = mSimdResult.begin(); iter != mSimdResult.end(); ++iter)
	{
		const SIMD_EVAL_KEY& key = iter->first;
		const T_SIMD_EVAL& data = iter->second;

		if (data.nPosType == T_SIMD_EVAL::kUnKnown) continue;

		T_SIPA_D SipaD;
		if (!m_pDoc->m_pAttrCtrl2->GetQSipa()->Get(data.kSipa, SipaD)) continue;
		if (SipaD.nDesignCode != m_nDesignCode) continue;

		T_SIMD_D simd;
		if (!m_pDoc->m_pAttrCtrl2->GetQSimd()->Get(data.kSimd, simd)) continue;

		eCode = SipaD.nDesignCode;
		bConsiderAlpha = simd.bConsiderAlpha;

		break;
	}

	if (eCode == 0) return;

	int nSubRow = 0;
	CString RptBlock;
	GetTheoryRangeName(nMaterial, bConsiderAlpha, RptBlock, nSubRow);

	int rowCnt = CopyRange(m_pXL, RptBlock, stRow);

	m_pXL->SetValue(stRow, 0, GetText(_T("%d-%d. %s"), iParagraph1, ++iParagraph2, _LS(IDS_BRDG_SEIS_EVAL_REPORT_JP_CHK_MAX_DISP_ALL)));// 最大応答変位の照査 - 全体系
	m_pXL->SetValue(stRow + nSubRow + 0, 4, GetText(_T("%g"), dXi1));
	m_pXL->SetValue(stRow + nSubRow + 1, 4, GetText(_T("%g"), dPhiS));
	m_pXL->SetValue(stRow + nSubRow + 2, 4, GetText(_T("%g"), dK));

	stRow += rowCnt;
	stRow++;
}

void CMaxDispWhole_H29::GetTheoryRangeName(const int& nMaterial, const BOOL& bConsiderAlpha, OUT CString& RptBlock, OUT int& nSubRow)
{
	if (bConsiderAlpha) {
		RptBlock = _T("_01.MaxDisp_Theory2");
		nSubRow = 6;
	}
	else {
		RptBlock = _T("_01.MaxDisp_Theory1");
		nSubRow = 6;
	}
}

CMaxDispNEXCO_H29::CMaxDispNEXCO_H29(IExcel* pXL, CDgnProgressDlg* pProgress, const CString& strBasePath, const CString& strBaseBook, const SDgnExcelPrintOpt* pPrintOpt)
	: CMaxDispNEXCO_H24(pXL, pProgress, strBasePath, strBaseBook, pPrintOpt)
{
}

CMaxDispNEXCO_H29::~CMaxDispNEXCO_H29()
{
}

void CMaxDispNEXCO_H29::WirteTheory(const int& nMaterial, const int& nAlwCalcType, const int iParagraph1, int& iParagraph2, OUT int& stRow)
{
	CBrdgSeisEvalResult_JP* pResult = m_pDoc->m_pPostCtrl->GetBrdgSeisEvalResult_JP(); if (!pResult) { ASSERT(0); return; }

	int eCode = 0;
	BOOL bConsiderAlpha = FALSE;
	double dXi1 = m_tSifaH29.RcPierVal.Delta_yEd[T_SIFA_RC_Disp::kXi1];
	double dPhiS = m_tSifaH29.RcPierVal.Delta_yEd[T_SIFA_RC_Disp::kPhiS];
	double dK = m_tSifaH29.RcPierVal.dFactorK;
	const std::map<SIMD_EVAL_KEY, T_SIMD_EVAL>& mSimdResult = pResult->GetResultSimdMap();
	std::map<SIMD_EVAL_KEY, T_SIMD_EVAL>::const_iterator iter;
	for (iter = mSimdResult.begin(); iter != mSimdResult.end(); ++iter)
	{
		const SIMD_EVAL_KEY& key = iter->first;
		const T_SIMD_EVAL& data = iter->second;

		if (data.nPosType == T_SIMD_EVAL::kUnKnown) continue;

		T_SIPA_D SipaD;
		if (!m_pDoc->m_pAttrCtrl2->GetQSipa()->Get(data.kSipa, SipaD)) continue;
		if (SipaD.nDesignCode != m_nDesignCode) continue;

		T_SIMD_D simd;
		if (!m_pDoc->m_pAttrCtrl2->GetQSimd()->Get(data.kSimd, simd)) continue;

		eCode = SipaD.nDesignCode;
		bConsiderAlpha = simd.bConsiderAlpha;

		break;
	}

	if (eCode == 0) return;

	int nSubRow = 0;
	CString RptBlock;
	GetTheoryRangeName(nMaterial, nAlwCalcType, bConsiderAlpha, RptBlock, nSubRow);
	m_strRptBlockBody = _T("_01.MaxDisp_Body1_pier"); // 임시

	int rowCnt = CopyRange(m_pXL, RptBlock, stRow);

	// N-1. 最大応答変位の照査 - 全体系
	m_pXL->SetValue(stRow, 0, GetText(_T("%d-%d. %s"), iParagraph1, ++iParagraph2, _LS(IDS_BRDG_SEIS_EVAL_REPORT_JP_CHK_MAX_DISP_PIER)));// 最大応答変位の照査 - 橋脚別
	m_pXL->SetValue(stRow + nSubRow + 0, 5, GetText(_T("%g"), dXi1));
	m_pXL->SetValue(stRow + nSubRow + 1, 5, GetText(_T("%g"), dPhiS));
	m_pXL->SetValue(stRow + nSubRow + 5, 4, GetText(_T("%g"), dK));

	stRow += rowCnt;
	stRow++;
}

void CMaxDispNEXCO_H29::GetTheoryRangeName(const int& nMaterial, const int& nAlwCalcType, const BOOL& bConsiderAlpha, OUT CString& RptBlock, OUT int& nSubRow)
{
	RptBlock = _T("_01.MaxDisp_Theory1_pier");
	nSubRow = 7;
}

CMaxDispJBEC_H29::CMaxDispJBEC_H29(IExcel* pXL, CDgnProgressDlg* pProgress, const CString& strBasePath, const CString& strBaseBook, const SDgnExcelPrintOpt* pPrintOpt)
	: CMaxDispJBEC_H24(pXL, pProgress, strBasePath, strBaseBook, pPrintOpt)
{
}

CMaxDispJBEC_H29::~CMaxDispJBEC_H29()
{
}

void CMaxDispJBEC_H29::WirteTheory(const int& nMaterial, const int& nAlwCalcType, const int iParagraph1, int& iParagraph2, OUT int& stRow)
{
	CBrdgSeisEvalResult_JP* pResult = m_pDoc->m_pPostCtrl->GetBrdgSeisEvalResult_JP(); if (!pResult) { ASSERT(0); return; }

	int eCode = 0;
	BOOL bConsiderAlpha = FALSE;
	double dXi1 = m_tSifaH29.RcPierVal.Delta_yEd[T_SIFA_RC_Disp::kXi1];
	double dPhiS = m_tSifaH29.RcPierVal.Delta_yEd[T_SIFA_RC_Disp::kPhiS];
	double dK = m_tSifaH29.RcPierVal.dFactorK;
	const std::map<SIMD_EVAL_KEY, T_SIMD_EVAL>& mSimdResult = pResult->GetResultSimdMap();
	std::map<SIMD_EVAL_KEY, T_SIMD_EVAL>::const_iterator iter;
	for (iter = mSimdResult.begin(); iter != mSimdResult.end(); ++iter)
	{
		const SIMD_EVAL_KEY& key = iter->first;
		const T_SIMD_EVAL& data = iter->second;

		if (data.nPosType == T_SIMD_EVAL::kUnKnown) continue;

		T_SIPA_D SipaD;
		if (!m_pDoc->m_pAttrCtrl2->GetQSipa()->Get(data.kSipa, SipaD)) continue;
		if (SipaD.nDesignCode != m_nDesignCode) continue;

		T_SIMD_D simd;
		if (!m_pDoc->m_pAttrCtrl2->GetQSimd()->Get(data.kSimd, simd)) continue;

		eCode = SipaD.nDesignCode;
		bConsiderAlpha = simd.bConsiderAlpha;

		break;
	}

	if (eCode == 0) return;

	int nSubRow = 0;
	CString RptBlock;
	GetTheoryRangeName(nMaterial, nAlwCalcType, bConsiderAlpha, RptBlock, nSubRow);
	m_strRptBlockBody = _T("_01.MaxDisp_Body1_pier"); // 임시

	int rowCnt = CopyRange(m_pXL, RptBlock, stRow);

	// N-1. 最大応答変位の照査 - 全体系
	m_pXL->SetValue(stRow, 0, GetText(_T("%d-%d. %s"), iParagraph1, ++iParagraph2, _LS(IDS_BRDG_SEIS_EVAL_REPORT_JP_CHK_MAX_DISP_PIER)));// 最大応答変位の照査 - 橋脚別
	m_pXL->SetValue(stRow + nSubRow + 0, 5, GetText(_T("%g"), dXi1));
	m_pXL->SetValue(stRow + nSubRow + 1, 5, GetText(_T("%g"), dPhiS));
	m_pXL->SetValue(stRow + nSubRow + 5, 4, GetText(_T("%g"), dK));

	stRow += rowCnt;
	stRow++;
}

void CMaxDispJBEC_H29::GetTheoryRangeName(const int& nMaterial, const int& nAlwCalcType, const BOOL& bConsiderAlpha, OUT CString& RptBlock, OUT int& nSubRow)
{
	RptBlock = _T("_01.MaxDisp_Theory1_pier");
	nSubRow = 7;
}

#pragma endregion CMaxDisp H29

#pragma region CResidualDisp H29
CResidualDispWhole_H29::CResidualDispWhole_H29(IExcel* pXL, CDgnProgressDlg* pProgress, const CString& strBasePath, const CString& strBaseBook, const SDgnExcelPrintOpt* pPrintOpt)
	: CResidualDispWhole(pXL, pProgress, strBasePath, strBaseBook, pPrintOpt)
{
}

CResidualDispWhole_H29::~CResidualDispWhole_H29()
{
}

CResidualDispNEXCO_H29::CResidualDispNEXCO_H29(IExcel* pXL, CDgnProgressDlg* pProgress, const CString& strBasePath, const CString& strBaseBook, const SDgnExcelPrintOpt* pPrintOpt)
	: CResidualDispNEXCO_H24(pXL, pProgress, strBasePath, strBaseBook, pPrintOpt)
{
}

CResidualDispNEXCO_H29::~CResidualDispNEXCO_H29()
{
}

CResidualDispJBEC_H29::CResidualDispJBEC_H29(IExcel* pXL, CDgnProgressDlg* pProgress, const CString& strBasePath, const CString& strBaseBook, const SDgnExcelPrintOpt* pPrintOpt)
	: CResidualDispJBEC_H24(pXL, pProgress, strBasePath, strBaseBook, pPrintOpt)
{
}

CResidualDispJBEC_H29::~CResidualDispJBEC_H29()
{
}

#pragma endregion CResidualDisp H29

#pragma endregion H29
