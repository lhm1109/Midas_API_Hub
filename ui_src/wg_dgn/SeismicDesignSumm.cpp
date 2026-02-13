#include "stdafx.h"
#include "SeismicDesignSumm.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_PostCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl2.h"
#include "..\wg_db\DB_QUERY_SEIS_CVL_JP.h"
#include "..\wg_db\BrdgSeisEvalResult_JP.h"

#include "DgnProgressDlg.h"

CSeismicDesignSumm::CSeismicDesignSumm(void)
{
	m_pDoc = CDBDoc::GetDocPoint();
	ASSERT(m_pDoc);
}

CSeismicDesignSumm::~CSeismicDesignSumm(void)
{
}

void CSeismicDesignSumm::AddProgressStep(CDgnProgressDlg* pProgress)
{
	//전체계 변위조사
	pProgress->m_nTotalSteps++;

	//교각별 변위조사
	pProgress->m_nTotalSteps++;

	//상부구조 조사
	pProgress->m_nTotalSteps++;

	//하부구조 조사
	pProgress->m_nTotalSteps++;
}

BOOL CSeismicDesignSumm::Calculate(CDgnProgressDlg* pProgress)
{
	//전체계 변위조사
	SummaryOverrallDisp();
	if (pProgress->NextStep() == FALSE) throw FALSE;

	//교각별 변위조사
	SummaryPierDisp();
	if (pProgress->NextStep() == FALSE) throw FALSE;

	//상부구조 조사
	SummaryUpperStructure();
	if (pProgress->NextStep() == FALSE) throw FALSE;

	//하부구조 조사
	SummaryLowerStructure();
	if (pProgress->NextStep() == FALSE) throw FALSE;

	return TRUE;
}

void CSeismicDesignSumm::SummaryOverrallDisp()
{
	CBrdgSeisEvalResult_JP* pResult = m_pDoc->m_pPostCtrl->GetBrdgSeisEvalResult_JP(); if (!pResult) { ASSERT(0); return; }

	std::vector<T_SIGR_K> sigrKeys;
	std::map<T_SIGR_K, std::vector<T_SIMD_K>> mSimdByGroup;
	std::map<T_SIGR_K, std::vector<T_SIRD_K>> mSirdByGroup;
	GetSimdKeyMap(sigrKeys, mSimdByGroup);
	GetSirdKeyMap(sigrKeys, mSirdByGroup);

	for (auto kSigr : sigrKeys) {
		T_EVAL_SUMM_OVERALL summary;
		summary.kSigr = kSigr;
		
		if (mSimdByGroup.find(kSigr) != mSimdByGroup.end())
			GetSummarySimd(mSimdByGroup[kSigr], summary);
		if (mSirdByGroup.find(kSigr) != mSirdByGroup.end())
			GetSummarySird(mSirdByGroup[kSigr], summary);

		if (summary.SimdSum.Left.GetCount() < 1 && summary.SimdSum.Right.GetCount() < 1 &&
			summary.SirdSum.Left.GetCount() < 1 && summary.SirdSum.Right.GetCount() < 1) {
			continue;
		}

		pResult->SetSummaryOverrall(kSigr, summary);
	}
}

void CSeismicDesignSumm::GetSimdKeyMap(OUT std::vector<T_SIGR_K>& sigrKeys, OUT std::map<T_SIGR_K, std::vector<T_SIMD_K>>& mSimdByGroup)
{
	T_KEY_LIST simdKeys;
	m_pDoc->m_pAttrCtrl2->GetQSimd()->GetKeyList(simdKeys);

	for (int idx = 0; idx < simdKeys.GetCount(); idx++) {
		T_SIMD_K kSimd = simdKeys[idx];
		T_SIMD_D dSimd; T_SIGR_D dSigr;
		if (m_pDoc->m_pAttrCtrl2->GetQSimd()->Get(kSimd, dSimd) == FALSE) continue;
		if (m_pDoc->m_pAttrCtrl2->GetQSigr()->Get(dSimd.kSigr, dSigr) == FALSE) continue;
		if (dSigr.nSubType == T_SIGR_D::kLower) continue;

		auto it = std::find(sigrKeys.begin(), sigrKeys.end(), dSimd.kSigr);
		if (it == sigrKeys.end())
			sigrKeys.push_back(dSimd.kSigr);

		if (mSimdByGroup.find(dSimd.kSigr) == mSimdByGroup.end()) {
			std::vector<T_SIMD_K> keys;
			keys.push_back(kSimd);
			mSimdByGroup.insert(std::make_pair(dSimd.kSigr, keys));
		}
		else {
			mSimdByGroup[dSimd.kSigr].push_back(kSimd);
		}
	}
}

void CSeismicDesignSumm::GetSirdKeyMap(OUT std::vector<T_SIGR_K>& sigrKeys, OUT std::map<T_SIGR_K, std::vector<T_SIRD_K>>& mSirdByGroup)
{
	T_KEY_LIST sirdKeys;
	m_pDoc->m_pAttrCtrl2->GetQSird()->GetKeyList(sirdKeys);

	for (int idx = 0; idx < sirdKeys.GetCount(); idx++)
	{
		T_SIRD_K kSird = sirdKeys[idx];
		T_SIRD_D dSird; T_SIGR_D dSigr;
		if (m_pDoc->m_pAttrCtrl2->GetQSird()->Get(kSird, dSird) == FALSE) continue;
		if (m_pDoc->m_pAttrCtrl2->GetQSigr()->Get(dSird.kSigr, dSigr) == FALSE) continue;
		if (dSigr.nSubType == T_SIGR_D::kLower) continue;

		auto it = std::find(sigrKeys.begin(), sigrKeys.end(), dSird.kSigr);
		if (it == sigrKeys.end())
			sigrKeys.push_back(dSird.kSigr);

		if (mSirdByGroup.find(dSird.kSigr) == mSirdByGroup.end()) {
			std::vector<T_SIRD_K> keys;
			keys.push_back(kSird);
			mSirdByGroup.insert(std::make_pair(dSird.kSigr, keys));
		}
		else {
			mSirdByGroup[dSird.kSigr].push_back(kSird);
		}
	}
}

void CSeismicDesignSumm::GetSummarySimd(const std::vector<T_SIMD_K>& simdKeys, OUT T_EVAL_SUMM_OVERALL& rSummary)
{
	CBrdgSeisEvalResult_JP* pResult = m_pDoc->m_pPostCtrl->GetBrdgSeisEvalResult_JP(); if (!pResult) { ASSERT(0); return; }

	for (auto kSimd : simdKeys)
	{
		T_SIMD_D dSimd;
		if (m_pDoc->m_pAttrCtrl2->GetQSimd()->Get(kSimd, dSimd) == FALSE) continue;

		T_SIMD_EVAL eval;
		
		if (pResult->GetResultSimd(dSimd.kSipa, dSimd.kSigr, T_SIMD_EVAL::kLeft, eval) == TRUE) {
			GetSimdAvergeDisp(&eval, rSummary.SimdSum.Left);
		}

		if (pResult->GetResultSimd(dSimd.kSipa, dSimd.kSigr, T_SIMD_EVAL::kRight, eval) == TRUE) {
			GetSimdAvergeDisp(&eval, rSummary.SimdSum.Right);
		}
	}
}

void CSeismicDesignSumm::GetSimdAvergeDisp(IN T_SIMD_EVAL* pEval, OUT CArray<T_SIMD_SUMM_ITEM, T_SIMD_SUMM_ITEM>& items)
{
	int nCount = pEval->aDisplacement.GetCount();
	if (nCount < 1) return;

	T_SIPA_D dSipa;
	if (m_pDoc->m_pAttrCtrl2->GetQSipa()->Get(pEval->kSipa, dSipa) == FALSE) { ASSERT(FALSE); return; }

	T_SIGR_D dSigr;
	if (m_pDoc->m_pAttrCtrl2->GetQSigr()->Get(pEval->kSigrLower, dSigr) == FALSE) { ASSERT(FALSE); return; }

	T_SIMD_SUMM_ITEM item;
	item.strPierName = dSigr.strName;
	item.strColumnName = pEval->strColumnName;
	item.nSeismicForceType = dSipa.nSeismicForceType;
	item.nSeismicForceDirection = dSipa.nSeismicForceDirection;
	item.dDeltaMax = pEval->GetDeltaMax(pEval->Averge.aDeltaMax);
	item.dDeltaA = pEval->GetDeltaA(pEval->Averge.aDeltaA);
	
	items.Add(item);
}

void CSeismicDesignSumm::GetSummarySird(const std::vector<T_SIRD_K>& sirdKeys, OUT T_EVAL_SUMM_OVERALL& rSummary)
{
	CBrdgSeisEvalResult_JP* pResult = m_pDoc->m_pPostCtrl->GetBrdgSeisEvalResult_JP(); if (!pResult) { ASSERT(0); return; }

	for (auto kSird : sirdKeys)
	{
		T_SIRD_D dSird;
		if (m_pDoc->m_pAttrCtrl2->GetQSird()->Get(kSird, dSird) == FALSE) return;

		T_SIRD_EVAL eval;

		if (pResult->GetResultSird(dSird.kSipa, dSird.kSigr, T_SIRD_EVAL::kLeft, eval) == TRUE) {
			GetSirdAvergeDisp(&eval, rSummary.SirdSum.Left);
		}

		if (pResult->GetResultSird(dSird.kSipa, dSird.kSigr, T_SIRD_EVAL::kRight, eval) == TRUE) {
			GetSirdAvergeDisp(&eval, rSummary.SirdSum.Right);
		}
	}
}

void CSeismicDesignSumm::GetSirdAvergeDisp(IN T_SIRD_EVAL* pEval, OUT CArray<T_SIRD_SUMM_ITEM, T_SIRD_SUMM_ITEM>& items)
{
	int nCount = pEval->aDisplacement.GetCount();
	if (nCount < 1) return;

	T_SIPA_D dSipa;
	if (m_pDoc->m_pAttrCtrl2->GetQSipa()->Get(pEval->kSipa, dSipa) == FALSE) return;

	T_SIGR_D dSigr;
	if (m_pDoc->m_pAttrCtrl2->GetQSigr()->Get(pEval->kSigrLower, dSigr) == FALSE) { ASSERT(FALSE); return; }

	T_SIRD_SUMM_ITEM item;
	item.strPierName = dSigr.strName;
	item.strColumnName = pEval->strName;
	item.nSeismicForceType = dSipa.nSeismicForceType;
	item.nSeismicForceDirection = dSipa.nSeismicForceDirection;
	item.dMuR = pEval->Averge.dMu_r;
	item.dDeltaR = pEval->Averge.dDelta_r;
	item.dDrataRa = pEval->Averge.dDelta_ra;

	items.Add(item);
}

void CSeismicDesignSumm::SummaryPierDisp()
{
	CBrdgSeisEvalResult_JP* pResult = m_pDoc->m_pPostCtrl->GetBrdgSeisEvalResult_JP(); if (!pResult) { ASSERT(0); return; }

	std::vector<T_SIGR_K> sigrKeys;
	std::map<T_SIGR_K, std::vector<T_SIMD_K>> mSimdByGroup;
	std::map<T_SIGR_K, std::vector<T_SIRD_K>> mSirdByGroup;
	GetSimdLowerKeyMap(sigrKeys, mSimdByGroup);
	GetSirdLowerKeyMap(sigrKeys, mSirdByGroup);

	for (auto kSigr : sigrKeys) {
		T_EVAL_SUMM_PIER summary;
		summary.kSigr = kSigr;

		if (mSimdByGroup.find(kSigr) != mSimdByGroup.end())
			GetSummarySimdLower(mSimdByGroup[kSigr], summary);
		if (mSirdByGroup.find(kSigr) != mSirdByGroup.end())
			GetSummarySirdLower(mSirdByGroup[kSigr], summary);

		if (summary.SirdSum.GetCount() < 1 && summary.SimdSum.GetCount() < 1) {
			continue;
		}

		pResult->SetSummaryPier(kSigr, summary);
	}
}

void CSeismicDesignSumm::GetSimdLowerKeyMap(OUT std::vector<T_SIGR_K>& sigrKeys, OUT std::map<T_SIGR_K, std::vector<T_SIMD_K>>& mSimdByGroup)
{
	T_KEY_LIST simdKeys;
	m_pDoc->m_pAttrCtrl2->GetQSimd()->GetKeyList(simdKeys);

	for (int idx = 0; idx < simdKeys.GetCount(); idx++) {
		T_SIMD_K kSimd = simdKeys[idx];
		T_SIMD_D dSimd; T_SIGR_D dSigr;
		if (m_pDoc->m_pAttrCtrl2->GetQSimd()->Get(kSimd, dSimd) == FALSE) continue;
		if (m_pDoc->m_pAttrCtrl2->GetQSigr()->Get(dSimd.kSigr, dSigr) == FALSE) continue;
		if (dSigr.nSubType != T_SIGR_D::kLower) continue;

		auto it = std::find(sigrKeys.begin(), sigrKeys.end(), dSimd.kSigr);
		if (it == sigrKeys.end())
			sigrKeys.push_back(dSimd.kSigr);

		if (mSimdByGroup.find(dSimd.kSigr) == mSimdByGroup.end()) {
			std::vector<T_SIMD_K> keys;
			keys.push_back(kSimd);
			mSimdByGroup.insert(std::make_pair(dSimd.kSigr, keys));
		}
		else {
			mSimdByGroup[dSimd.kSigr].push_back(kSimd);
		}
	}
}

void CSeismicDesignSumm::GetSirdLowerKeyMap(OUT std::vector<T_SIGR_K>& sigrKeys, OUT std::map<T_SIGR_K, std::vector<T_SIRD_K>>& mSirdByGroup)
{
	T_KEY_LIST sirdKeys;
	m_pDoc->m_pAttrCtrl2->GetQSird()->GetKeyList(sirdKeys);

	for (int idx = 0; idx < sirdKeys.GetCount(); idx++)
	{
		T_SIRD_K kSird = sirdKeys[idx];
		T_SIRD_D dSird; T_SIPA_D dSipa; T_SIGR_D dSigr;
		if (m_pDoc->m_pAttrCtrl2->GetQSird()->Get(kSird, dSird) == FALSE) continue;
		if (m_pDoc->m_pAttrCtrl2->GetQSigr()->Get(dSird.kSigr, dSigr) == FALSE) continue;
		if (dSigr.nSubType != T_SIGR_D::kLower) continue;

		auto it = std::find(sigrKeys.begin(), sigrKeys.end(), dSird.kSigr);
		if (it == sigrKeys.end())
			sigrKeys.push_back(dSird.kSigr);

		if (mSirdByGroup.find(dSird.kSigr) == mSirdByGroup.end()) {
			std::vector<T_SIRD_K> keys;
			keys.push_back(kSird);
			mSirdByGroup.insert(std::make_pair(dSird.kSigr, keys));
		}
		else {
			mSirdByGroup[dSird.kSigr].push_back(kSird);
		}
	}
}

void CSeismicDesignSumm::GetSummarySimdLower(const std::vector<T_SIMD_K>& simdKeys, OUT T_EVAL_SUMM_PIER& rSummary)
{
	CBrdgSeisEvalResult_JP* pResult = m_pDoc->m_pPostCtrl->GetBrdgSeisEvalResult_JP(); if (!pResult) { ASSERT(0); return; }

	for (auto kSimd : simdKeys)
	{
		T_SIMD_D dSimd;
		if (m_pDoc->m_pAttrCtrl2->GetQSimd()->Get(kSimd, dSimd) == FALSE) return;

		T_SIMD_EVAL eval;
		if (pResult->GetResultSimd(dSimd.kSipa, dSimd.kSigr, T_SIRD_EVAL::kLower, eval) == TRUE) {
			GetSimdAvergeDisp(&eval, rSummary.SimdSum);
		}
	}
}

void CSeismicDesignSumm::GetSummarySirdLower(const std::vector<T_SIRD_K>& sirdKeys, OUT T_EVAL_SUMM_PIER& rSummary)
{
	CBrdgSeisEvalResult_JP* pResult = m_pDoc->m_pPostCtrl->GetBrdgSeisEvalResult_JP(); if (!pResult) { ASSERT(0); return; }

	for (auto kSird : sirdKeys)
	{
		T_SIRD_D dSird;
		if (m_pDoc->m_pAttrCtrl2->GetQSird()->Get(kSird, dSird) == FALSE) return;

		T_SIRD_EVAL eval;
		if (pResult->GetResultSird(dSird.kSipa, dSird.kSigr, T_SIRD_EVAL::kLower, eval) == TRUE) {
			GetSirdAvergeDisp(&eval, rSummary.SirdSum);
		}
	}
}

void CSeismicDesignSumm::SummaryUpperStructure()
{
	CBrdgSeisEvalResult_JP* pResult = m_pDoc->m_pPostCtrl->GetBrdgSeisEvalResult_JP(); if (!pResult) { ASSERT(0); return; }

	CArray<T_SIGR_K, T_SIGR_K> sigrKeys;
	std::map<T_SIGR_K, std::vector<T_SIBD_K>> mSibd_CVSV;
	std::map<T_SIGR_K, std::vector<T_SIBD_K>> mSibd_SRSV;
	std::map<T_SIGR_K, std::vector<T_SISR_K>> mSisr_UPPER;
	m_pDoc->m_pAttrCtrl2->GetQSigr()->GetKeyList(sigrKeys);
	GetSibdkeysBySigr(T_SIBD_D::kCurvatureSurvey, mSibd_CVSV);
	GetSibdkeysBySigr(T_SIBD_D::kStrengthSurvey, mSibd_SRSV);
	GetSisrUpperKeyMap(mSisr_UPPER);

	for (int i = 0; i < sigrKeys.GetCount(); i++) {
		const T_SIGR_K& kSigr = sigrKeys[i];
		T_EVAL_SUMM_UPPER summary;
		summary.kSigr = kSigr;

		T_SIGR_D dSigr;
		if (m_pDoc->m_pAttrCtrl2->GetQSigr()->Get(kSigr, dSigr) == FALSE) continue;
		if (dSigr.nSubType != T_SIGR_D::kUpper) continue;

		if (mSibd_CVSV.find(kSigr) != mSibd_CVSV.end())
			GetSummarySibdCvsv(mSibd_CVSV[kSigr], summary.CvsvSum);
		if (mSibd_SRSV.find(kSigr) != mSibd_SRSV.end())
			GetSummarySibdSrsv(mSibd_SRSV[kSigr], summary.SrsvSum);
		if (mSisr_UPPER.find(kSigr) != mSisr_UPPER.end())
			GetSummarySisrUpper(mSisr_UPPER[kSigr], summary.SisrSum);

		if (summary.CvsvSum.CvsvMin.GetCount() < 1 && summary.CvsvSum.CvsvMax.GetCount() < 1 &&
			summary.SrsvSum.SrsvMin.GetCount() < 1 && summary.SrsvSum.SrsvMax.GetCount() < 1 &&
			summary.SisrSum.SisrMin.GetCount() < 1 && summary.SisrSum.SisrMax.GetCount() < 1) {
			continue;
		}

		pResult->SetSummaryUpper(kSigr, summary);
	}
}

void CSeismicDesignSumm::GetSibdkeysBySigr(/*T_SIBD_D::eSibdType*/const int& nSibdType, OUT std::map<T_SIGR_K, std::vector<T_SIBD_K>>& mSibdKeys)
{
	T_KEY_LIST sibdKeys;
	m_pDoc->m_pAttrCtrl2->GetQSibd()->GetKeyList(sibdKeys);

	for (int idx = 0; idx < sibdKeys.GetCount(); idx++) {
		T_SIBD_K kSibd = sibdKeys[idx];
		T_SIBD_D dSibd;
		if (m_pDoc->m_pAttrCtrl2->GetQSibd()->Get(kSibd, dSibd) == FALSE) continue;
		if (nSibdType != T_SIBD_D::kUnKnown) {
			if (dSibd.nSibdType != nSibdType) continue;
		}

		if (mSibdKeys.find(dSibd.kSigr) == mSibdKeys.end()) {
			std::vector<T_SIMD_K> keys;
			keys.push_back(kSibd);
			mSibdKeys.insert(std::make_pair(dSibd.kSigr, keys));
		}
		else {
			mSibdKeys[dSibd.kSigr].push_back(kSibd);
		}
	}
}

void CSeismicDesignSumm::GetSisrUpperKeyMap(std::map<T_SIGR_K, std::vector<T_SISR_K>>& mSisr_UPPER)
{
	T_KEY_LIST sisrKeys;
	m_pDoc->m_pAttrCtrl2->GetQSisr()->GetKeyList(sisrKeys);

	for (int idx = 0; idx < sisrKeys.GetCount(); idx++) {
		T_SISR_K kSisr = sisrKeys[idx];
		T_SISR_D dSisr;
		if (m_pDoc->m_pAttrCtrl2->GetQSisr()->Get(kSisr, dSisr) == FALSE) continue;
		if (dSisr.AUTO_D.nDesignCriteria != T_SISR_AUTO_D::kRoad3) continue;

		if (mSisr_UPPER.find(dSisr.kSigr) == mSisr_UPPER.end()) {
			std::vector<T_SISR_K> keys;
			keys.push_back(kSisr);
			mSisr_UPPER.insert(std::make_pair(dSisr.kSigr, keys));
		}
		else {
			mSisr_UPPER[dSisr.kSigr].push_back(kSisr);
		}
	}
}

void CSeismicDesignSumm::GetSisrLowerKeyMap(std::map<T_SIGR_K, std::vector<T_SISR_K>>& mSisr_LOWER)
{
	T_KEY_LIST sisrKeys;
	m_pDoc->m_pAttrCtrl2->GetQSisr()->GetKeyList(sisrKeys);

	for (int idx = 0; idx < sisrKeys.GetCount(); idx++) {
		T_SISR_K kSisr = sisrKeys[idx];
		T_SISR_D dSisr;
		if (m_pDoc->m_pAttrCtrl2->GetQSisr()->Get(kSisr, dSisr) == FALSE) continue;
		if (dSisr.AUTO_D.nDesignCriteria == T_SISR_AUTO_D::kRoad3) continue;

		if (mSisr_LOWER.find(dSisr.kSigr) == mSisr_LOWER.end()) {
			std::vector<T_SISR_K> keys;
			keys.push_back(kSisr);
			mSisr_LOWER.insert(std::make_pair(dSisr.kSigr, keys));
		}
		else {
			mSisr_LOWER[dSisr.kSigr].push_back(kSisr);
		}
	}
}

void CSeismicDesignSumm::GetSummarySibdCvsv(const std::vector<T_SIBD_K>& sibdKeys, OUT T_SIBD_CVSV_DATA& rSummary)
{
	//최대/최소, 교축방향/교축직각방향, 조사(Y/Z)방향 8가지 경우 각각에 해당하는 가장 불리한 값을 찾아낸다.
	std::map<SUMM_EVAL_KEY, T_SIBD_CVSV_ITEM> mCvsvItem;

	for (auto kSibd : sibdKeys)
	{
		T_SIBD_D dSibd;
		if (m_pDoc->m_pAttrCtrl2->GetQSibd()->Get(kSibd, dSibd) == FALSE) continue;

		switch (dSibd.CVSV_D.nCvsvType)
		{
		case T_SIBD_CVSV_D::kSurrenderSurvey:
			GetCvsvItemFromSurrender(kSibd, mCvsvItem);
			break;
		case T_SIBD_CVSV_D::kLimitStateSurvey:
			GetCvsvItemFromLimitState(kSibd, mCvsvItem);
			break;
		default:
			break;
		}
	}

	for (const auto& pair : mCvsvItem)
	{
		SUMM_EVAL_K evalKey;
		evalKey.keyMap = pair.first;

		switch (evalKey.Key.nMinMax)
		{
		case SUMM_EVAL_K::kMin:
			rSummary.CvsvMin.Add(pair.second);
			break;
		case SUMM_EVAL_K::kMax:
			rSummary.CvsvMax.Add(pair.second);
			break;
		default:
			break;
		}
	}
}

void SetCvsvItemMapMinPhiY(const T_SIBD_K& kSibd, const T_ELEM_K& kElem, const T_SIPA_D& dSipa, const T_SIBD_EVAL_PHI_Y& eval, const UINT& keyMap, OUT std::map<SUMM_EVAL_KEY, T_SIBD_CVSV_ITEM>& mCvsvItem)
{
	T_SIBD_CVSV_ITEM item;
	BOOL bNew = mCvsvItem.find(keyMap) == mCvsvItem.end();
	if (bNew == FALSE) {
		item = mCvsvItem[keyMap];
		if (eval.resultRc.dRatioMin <= item.dRatio) 
			return;
	}

	item.kSibd = kSibd;
	item.kElem = kElem;
	item.nMemberType = eval.nMemberType;
	item.nMaterialType = eval.nMaterialType;
	item.nSeismicForceType = dSipa.nSeismicForceType;
	item.nSeismicForceDirection = dSipa.nSeismicForceDirection;
	item.nExamShearDir = eval.nExamShearDir;
	item.dDeltaMax = eval.analDeform.dAvergeMin;
	item.dDeltaY = eval.calcDeformMin.dPhi_y;
	item.dRatio = eval.resultRc.dRatioMin;

	if (bNew) mCvsvItem.insert(std::make_pair(keyMap, item));
	else mCvsvItem[keyMap] = item;
}

void SetCvsvItemMapMaxPhiY(const T_SIBD_K& kSibd, const T_ELEM_K& kElem, const T_SIPA_D& dSipa, const T_SIBD_EVAL_PHI_Y& eval, const UINT& keyMap, OUT std::map<SUMM_EVAL_KEY, T_SIBD_CVSV_ITEM>& mCvsvItem)
{
	T_SIBD_CVSV_ITEM item;
	BOOL bNew = mCvsvItem.find(keyMap) == mCvsvItem.end();
	if (bNew == FALSE) {
		item = mCvsvItem[keyMap];
		if (eval.resultRc.dRatioMax <= item.dRatio)
			return;
	}

	item.kSibd = kSibd;
	item.kElem = kElem;
	item.nMemberType = eval.nMemberType;
	item.nMaterialType = eval.nMaterialType;
	item.nSeismicForceType = dSipa.nSeismicForceType;
	item.nSeismicForceDirection = dSipa.nSeismicForceDirection;
	item.nExamShearDir = eval.nExamShearDir;
	item.dDeltaMax = eval.analDeform.dAvergeMax;
	item.dDeltaY = eval.calcDeformMax.dPhi_y;
	item.dRatio = eval.resultRc.dRatioMax;

	if (bNew) mCvsvItem.insert(std::make_pair(keyMap, item));
	else mCvsvItem[keyMap] = item;
}

void CSeismicDesignSumm::GetCvsvItemFromSurrender(const T_SIBD_K& kSibd, OUT std::map<SUMM_EVAL_KEY, T_SIBD_CVSV_ITEM>& mCvsvItem)
{
	CBrdgSeisEvalResult_JP* pResult = m_pDoc->m_pPostCtrl->GetBrdgSeisEvalResult_JP(); if (!pResult) { ASSERT(0); return; }

	T_SIBD_D dSibd; T_SIPA_D dSipa; 
	if (m_pDoc->m_pAttrCtrl2->GetQSibd()->Get(kSibd, dSibd) == FALSE) return;
	if (m_pDoc->m_pAttrCtrl2->GetQSipa()->Get(dSibd.kSipa, dSipa) == FALSE) return;

	T_ELEM_K_LIST kElemList;
	m_pDoc->m_pAttrCtrl2->GetQSibd()->GetElementKeys(kSibd, kElemList);

	for (int i = 0; i < kElemList.GetCount(); i++)
	{
		const T_ELEM_K& kElem = kElemList[i];

		for (int nShearDir = seis_jp::kY_Dir; nShearDir < 3; nShearDir++)
		{
			T_SIBD_EVAL_PHI_Y eval;
			eval.kSipa = dSibd.kSipa;
			eval.kSigr = dSibd.kSigr;
			eval.kElem = kElem;
			eval.nExamShearDir = nShearDir;
			if (pResult->GetResultSibdPhiY(eval) == FALSE) continue;

			SUMM_EVAL_K minKey(SUMM_EVAL_K::kMin, nShearDir, dSipa.nSeismicForceDirection, eval.nMemberType);
			SetCvsvItemMapMinPhiY(kSibd, kElem, dSipa, eval, minKey.keyMap, mCvsvItem);

			SUMM_EVAL_K maxKey(SUMM_EVAL_K::kMax, nShearDir, dSipa.nSeismicForceDirection, eval.nMemberType);
			SetCvsvItemMapMaxPhiY(kSibd, kElem, dSipa, eval, maxKey.keyMap, mCvsvItem);
		}
	}
}

void SetCvsvItemMapMinPhiA(const T_SIBD_K& kSibd, const T_ELEM_K& kElem, const T_SIPA_D& dSipa, const T_SIBD_EVAL_PHI_A& eval, const UINT& keyMap, OUT std::map<SUMM_EVAL_KEY, T_SIBD_CVSV_ITEM>& mCvsvItem)
{
	T_SIBD_CVSV_ITEM item;
	BOOL bNew = mCvsvItem.find(keyMap) == mCvsvItem.end();
	if (bNew == FALSE) {
		item = mCvsvItem[keyMap];
		if (eval.resultRc.dRatioMin <= item.dRatio)
			return;
	}

	item.kSibd = kSibd;
	item.kElem = kElem;
	item.nMemberType = eval.nMemberType;
	item.nMaterialType = eval.nMaterialType;
	item.nExamShearDir = eval.nExamShearDir;
	item.nSeismicForceType = dSipa.nSeismicForceType;
	item.nSeismicForceDirection = dSipa.nSeismicForceDirection;
	item.dDeltaMax = eval.analDeform.dAvergeMin;
	item.dDeltaY = eval.calcDeformMin.dPhi_y;
	item.dDeltaA = eval.calcDeformMin.dPhi_a;
	item.dRatio = eval.resultRc.dRatioMin;

	if (bNew) mCvsvItem.insert(std::make_pair(keyMap, item));
	else mCvsvItem[keyMap] = item;
}

void SetCvsvItemMapMaxPhiA(const T_SIBD_K& kSibd, const T_ELEM_K& kElem, const T_SIPA_D& dSipa, const T_SIBD_EVAL_PHI_A& eval, const UINT& keyMap, OUT std::map<SUMM_EVAL_KEY, T_SIBD_CVSV_ITEM>& mCvsvItem)
{
	T_SIBD_CVSV_ITEM item;
	BOOL bNew = mCvsvItem.find(keyMap) == mCvsvItem.end();
	if (bNew == FALSE) {
		item = mCvsvItem[keyMap];
		if (eval.resultRc.dRatioMax <= item.dRatio)
			return;
	}

	item.kSibd = kSibd;
	item.kElem = kElem;
	item.nMemberType = eval.nMemberType;
	item.nMaterialType = eval.nMaterialType;
	item.nExamShearDir = eval.nExamShearDir;
	item.nSeismicForceType = dSipa.nSeismicForceType;
	item.nSeismicForceDirection = dSipa.nSeismicForceDirection;
	item.dDeltaMax = eval.analDeform.dAvergeMax;
	item.dDeltaY = eval.calcDeformMax.dPhi_y;
	item.dDeltaA = eval.calcDeformMax.dPhi_a;
	item.dRatio = eval.resultRc.dRatioMax;

	if (bNew) mCvsvItem.insert(std::make_pair(keyMap, item));
	else mCvsvItem[keyMap] = item;
}

void CSeismicDesignSumm::GetCvsvItemFromLimitState(const T_SIBD_K& kSibd, OUT std::map<SUMM_EVAL_KEY, T_SIBD_CVSV_ITEM>& mCvsvItem)
{
	CBrdgSeisEvalResult_JP* pResult = m_pDoc->m_pPostCtrl->GetBrdgSeisEvalResult_JP(); if (!pResult) { ASSERT(0); return; }

	T_SIBD_D dSibd; T_SIPA_D dSipa;
	if (m_pDoc->m_pAttrCtrl2->GetQSibd()->Get(kSibd, dSibd) == FALSE) return;
	if (m_pDoc->m_pAttrCtrl2->GetQSipa()->Get(dSibd.kSipa, dSipa) == FALSE) return;

	T_ELEM_K_LIST kElemList;
	m_pDoc->m_pAttrCtrl2->GetQSibd()->GetElementKeys(kSibd, kElemList);

	for (int i = 0; i < kElemList.GetCount(); i++)
	{
		const T_ELEM_K& kElem = kElemList[i];

		for (int nShearDir = seis_jp::kY_Dir; nShearDir < 3; nShearDir++)
		{
			T_SIBD_EVAL_PHI_A eval;
			eval.kSipa = dSibd.kSipa;
			eval.kSigr = dSibd.kSigr;
			eval.kElem = kElem;
			eval.nExamShearDir = nShearDir;
			if (pResult->GetResultSibdPhiA(eval) == FALSE) continue;

			SUMM_EVAL_K minKey(SUMM_EVAL_K::kMin, nShearDir, dSipa.nSeismicForceDirection, eval.nMemberType);
			SetCvsvItemMapMinPhiA(kSibd, kElem, dSipa, eval, minKey.keyMap, mCvsvItem);

			SUMM_EVAL_K maxKey(SUMM_EVAL_K::kMax, nShearDir, dSipa.nSeismicForceDirection, eval.nMemberType);
			SetCvsvItemMapMaxPhiA(kSibd, kElem, dSipa, eval, maxKey.keyMap, mCvsvItem);
		}
	}
}

void CSeismicDesignSumm::GetSummarySibdSrsv(const std::vector<T_SIBD_K>& sibdKeys, OUT T_SIBD_SRSV_DATA& rSummary)
{
	//최대/최소, 교축방향/교축직각방향 4가지 경우 각각에 해당하는 가장 불리한 값을 찾아낸다.
	std::map<SUMM_EVAL_KEY, T_SIBD_SRSV_ITEM> mCvsvItem;

	for (auto kSibd : sibdKeys)
	{
		T_SIBD_D dSibd;
		if (m_pDoc->m_pAttrCtrl2->GetQSibd()->Get(kSibd, dSibd) == FALSE) continue;

		GetCvsvItemFromStrength(kSibd, mCvsvItem);
	}

	for (const auto& pair : mCvsvItem)
	{
		SUMM_EVAL_K evalKey;
		evalKey.keyMap = pair.first;

		switch (evalKey.Key.nMinMax)
		{
		case SUMM_EVAL_K::kMin:
			rSummary.SrsvMin.Add(const_cast<T_SIBD_SRSV_ITEM&>(pair.second));
			break;
		case SUMM_EVAL_K::kMax:
			rSummary.SrsvMax.Add(const_cast<T_SIBD_SRSV_ITEM&>(pair.second));
			break;
		default:
			break;
		}
	}
}

void SetCvsvItemMapMinMoment(const T_ELEM_K& kElem, const T_SIPA_D& dSipa, const T_SIBD_EVAL_MOMENT& eval, const UINT& keyMap, OUT std::map<SUMM_EVAL_KEY, T_SIBD_SRSV_ITEM>& mSrsvItem)
{
	T_SIBD_SRSV_ITEM item;
	BOOL bNew = mSrsvItem.find(keyMap) == mSrsvItem.end();
	if (bNew == FALSE) {
		item = mSrsvItem[keyMap];
		if (eval.analMoment_j.dAvergeMin <= item.dRatio)
			return;
	}

	item.kElem = kElem;
	item.nMemberType = eval.nMemberType;
	item.nSeismicForceType = dSipa.nSeismicForceType;
	item.nSeismicForceDirection = dSipa.nSeismicForceDirection;
	item.dMmax = eval.analMoment_j.dAvergeMin;
	item.dMy = eval.calcMomeMin.dY;
	item.dMu = eval.calcMomeMin.dU;
	item.dRatio = eval.analMoment_j.dAvergeMin;

	if (bNew) mSrsvItem.insert(std::make_pair(keyMap, item));
	else mSrsvItem[keyMap] = item;
}

void SetCvsvItemMapMaxMoment(const T_ELEM_K& kElem, const T_SIPA_D& dSipa, const T_SIBD_EVAL_MOMENT& eval, const UINT& keyMap, OUT std::map<SUMM_EVAL_KEY, T_SIBD_SRSV_ITEM>& mSrsvItem)
{
	T_SIBD_SRSV_ITEM item;
	BOOL bNew = mSrsvItem.find(keyMap) == mSrsvItem.end();
	if (bNew == FALSE) {
		item = mSrsvItem[keyMap];
		if (eval.analMoment_i.dAvergeMax <= item.dRatio)
			return;
	}

	item.kElem = kElem;
	item.nMemberType = eval.nMemberType;
	item.nSeismicForceType = dSipa.nSeismicForceType;
	item.nSeismicForceDirection = dSipa.nSeismicForceDirection;
	item.dMmax = eval.analMoment_i.dAvergeMax;
	item.dMy = eval.calcMomeMax.dY;
	item.dMu = eval.calcMomeMax.dU;
	item.dRatio = eval.analMoment_i.dAvergeMax;

	if (bNew) mSrsvItem.insert(std::make_pair(keyMap, item));
	else mSrsvItem[keyMap] = item;
}

void CSeismicDesignSumm::GetCvsvItemFromStrength(const T_SIBD_K& kSibd, OUT std::map<SUMM_EVAL_KEY, T_SIBD_SRSV_ITEM>& mCvsvItem)
{
	CBrdgSeisEvalResult_JP* pResult = m_pDoc->m_pPostCtrl->GetBrdgSeisEvalResult_JP(); if (!pResult) { ASSERT(0); return; }

	T_SIBD_D dSibd; T_SIPA_D dSipa;
	if (m_pDoc->m_pAttrCtrl2->GetQSibd()->Get(kSibd, dSibd) == FALSE) return;
	if (m_pDoc->m_pAttrCtrl2->GetQSipa()->Get(dSibd.kSipa, dSipa) == FALSE) return;

	T_ELEM_K_LIST kElemList;
	m_pDoc->m_pAttrCtrl2->GetQSibd()->GetElementKeys(kSibd, kElemList);

	for (int i = 0; i < kElemList.GetCount(); i++)
	{
		const T_ELEM_K& kElem = kElemList[i];

		for (int nShearDir = seis_jp::kY_Dir; nShearDir < 3; nShearDir++)
		{
			T_SIBD_EVAL_MOMENT eval;
			eval.kSipa = dSibd.kSipa;
			eval.kSigr = dSibd.kSigr;
			eval.kElem = kElem;
			eval.nExamShearDir = nShearDir;
			if (pResult->GetResultSibdMoment(eval) == FALSE) continue;

			SUMM_EVAL_K minKey(SUMM_EVAL_K::kMin, nShearDir, dSipa.nSeismicForceDirection, eval.nMemberType);
			SetCvsvItemMapMinMoment(kElem, dSipa, eval, minKey.keyMap, mCvsvItem);

			SUMM_EVAL_K maxKey(SUMM_EVAL_K::kMax, nShearDir, dSipa.nSeismicForceDirection, eval.nMemberType);
			SetCvsvItemMapMaxMoment(kElem, dSipa, eval, maxKey.keyMap, mCvsvItem);
		}
	}
}

void CSeismicDesignSumm::GetSummarySisrUpper(const std::vector<T_SISR_K>& sisrKeys, OUT T_SISR_SUMM_DATA& rSummary)
{
	//최대/최소, 교축방향/교축직각방향 4가지 경우 각각에 해당하는 가장 불리한 값을 찾아낸다.
	std::map<SUMM_EVAL_KEY, T_SISR_SUMM_ITEM> mSisrItem;

	for (auto kSisr : sisrKeys)
	{
		T_SISR_D dSisr;
		if (m_pDoc->m_pAttrCtrl2->GetQSisr()->Get(kSisr, dSisr) == FALSE) continue;
		if (dSisr.AUTO_D.nDesignCriteria != T_SISR_AUTO_D::kRoad3) continue;

		GetSisrItemFromUpper(kSisr, mSisrItem);
	}

	for (const auto& pair : mSisrItem)
	{
		SUMM_EVAL_K evalKey;
		evalKey.keyMap = pair.first;

		switch (evalKey.Key.nMinMax)
		{
		case SUMM_EVAL_K::kMin:
			rSummary.SisrMin.Add(const_cast<T_SISR_SUMM_ITEM&>(pair.second));
			break;
		case SUMM_EVAL_K::kMax:
			rSummary.SisrMax.Add(const_cast<T_SISR_SUMM_ITEM&>(pair.second));
			break;
		default:
			break;
		}
	}
}

T_SISR_USER_D::ePosType GetUnfavorablePosOfUpper(const T_SISR_EVAL_ROAD3& eval) {

	auto maxRatio = [](double& max, double ratio, T_SISR_USER_D::ePosType& pos, T_SISR_USER_D::ePosType myPos) {
		if (ratio > max) { max = ratio; pos = myPos; }
		};

	T_SISR_USER_D::ePosType nPos = T_SISR_USER_D::kUnKnown;
	double dMax = -DBL_MAX;
	maxRatio(dMax, eval.PosI.RsltMin.CapaComp.dRatio, nPos, T_SISR_USER_D::kI);
	maxRatio(dMax, eval.PosJ.RsltMin.CapaComp.dRatio, nPos, T_SISR_USER_D::kJ);
	maxRatio(dMax, eval.PosI.RsltMin.CapaDiag.dRatio, nPos, T_SISR_USER_D::kI);
	maxRatio(dMax, eval.PosJ.RsltMin.CapaDiag.dRatio, nPos, T_SISR_USER_D::kJ);

	return nPos;
}

void SetSisrItemMapMinUpper(const T_ELEM_K& kElem, const T_SIPA_D& dSipa, const T_SISR_EVAL_ROAD3& eval, const UINT& keyMap, std::map<SUMM_EVAL_KEY, T_SISR_SUMM_ITEM>& mSisrItem) 
{
	T_SISR_USER_D::ePosType ePos = GetUnfavorablePosOfUpper(eval);

	auto updateItem = [&](T_SISR_SUMM_ITEM& item, const auto& rsltMin) {
		item.dSuc = rsltMin.CapaComp.dSuc;
		item.dSus = rsltMin.CapaDiag.dSus;
		item.dSmax = rsltMin.Forc.dShear;
		item.dRatio = dgn::ME(rsltMin.CapaComp.dRatio, rsltMin.CapaDiag.dRatio) ? rsltMin.CapaComp.dRatio : rsltMin.CapaDiag.dRatio;
		};

	auto emplaceOrUpdate = [&](T_SISR_SUMM_ITEM& item, const auto& rsltMin) {
		updateItem(item, rsltMin);
		item.kElem = kElem;
		item.nExamShearDir = eval.nExamShearDir;
		item.nDesignCriteria = T_SISR_AUTO_D::kRoad3;
		item.nSeismicForceType = dSipa.nSeismicForceType;
		item.nSeismicForceDirection = dSipa.nSeismicForceDirection;
		};

	auto it = mSisrItem.find(keyMap);
	if (it == mSisrItem.end()) {
		T_SISR_SUMM_ITEM item;
		(ePos == T_SISR_USER_D::kI) ? emplaceOrUpdate(item, eval.PosI.RsltMin) : emplaceOrUpdate(item, eval.PosJ.RsltMin);
		mSisrItem[keyMap] = item;
	}
	else {
		T_SISR_SUMM_ITEM& item = it->second;
		double newRatio = 0;
		if (ePos == T_SISR_USER_D::kI)
			newRatio = (eval.PosI.RsltMin.CapaComp.dRatio > eval.PosI.RsltMin.CapaDiag.dRatio) ? eval.PosI.RsltMin.CapaComp.dRatio : eval.PosI.RsltMin.CapaDiag.dRatio;
		else
			newRatio = (eval.PosJ.RsltMin.CapaComp.dRatio > eval.PosJ.RsltMin.CapaDiag.dRatio) ? eval.PosJ.RsltMin.CapaComp.dRatio : eval.PosJ.RsltMin.CapaDiag.dRatio;
		if (newRatio > item.dRatio) {
			emplaceOrUpdate(item, (ePos == T_SISR_USER_D::kI) ? eval.PosI.RsltMin : eval.PosJ.RsltMin);
		}
	}
}

void SetSisrItemMapMaxUpper(const T_ELEM_K& kElem, const T_SIPA_D& dSipa, const T_SISR_EVAL_ROAD3& eval, const UINT& keyMap, OUT std::map<SUMM_EVAL_KEY, T_SISR_SUMM_ITEM>& mSisrItem)
{
	T_SISR_USER_D::ePosType ePos = GetUnfavorablePosOfUpper(eval);

	auto updateItem = [&](T_SISR_SUMM_ITEM& item, const auto& rsltMax) {
		item.dSuc = rsltMax.CapaComp.dSuc;
		item.dSus = rsltMax.CapaDiag.dSus;
		item.dSmax = rsltMax.Forc.dShear;
		item.dRatio = dgn::ME(rsltMax.CapaComp.dRatio, rsltMax.CapaDiag.dRatio) ? rsltMax.CapaComp.dRatio : rsltMax.CapaDiag.dRatio;
		};

	auto emplaceOrUpdate = [&](T_SISR_SUMM_ITEM& item, const auto& rsltMax) {
		updateItem(item, rsltMax);
		item.kElem = kElem;
		item.nExamShearDir = eval.nExamShearDir;
		item.nDesignCriteria = T_SISR_AUTO_D::kRoad3;
		item.nSeismicForceType = dSipa.nSeismicForceType;
		item.nSeismicForceDirection = dSipa.nSeismicForceDirection;
		};

	auto it = mSisrItem.find(keyMap);
	if (it == mSisrItem.end()) {
		T_SISR_SUMM_ITEM item;
		(ePos == T_SISR_USER_D::kI) ? emplaceOrUpdate(item, eval.PosI.RsltMax) : emplaceOrUpdate(item, eval.PosJ.RsltMax);
		mSisrItem[keyMap] = item;
	}
	else {
		T_SISR_SUMM_ITEM& item = it->second;
		double newRatio = 0;
		if (ePos == T_SISR_USER_D::kI)
			newRatio = (eval.PosI.RsltMax.CapaComp.dRatio > eval.PosI.RsltMax.CapaDiag.dRatio) ? eval.PosI.RsltMax.CapaComp.dRatio : eval.PosI.RsltMax.CapaDiag.dRatio;
		else
			newRatio = (eval.PosJ.RsltMax.CapaComp.dRatio > eval.PosJ.RsltMax.CapaDiag.dRatio) ? eval.PosJ.RsltMax.CapaComp.dRatio : eval.PosJ.RsltMax.CapaDiag.dRatio;
		if (newRatio > item.dRatio) {
			emplaceOrUpdate(item, (ePos == T_SISR_USER_D::kI) ? eval.PosI.RsltMax : eval.PosJ.RsltMax);
		}
	}
}

void CSeismicDesignSumm::GetSisrItemFromUpper(const T_SISR_K& kSisr, OUT std::map<SUMM_EVAL_KEY, T_SISR_SUMM_ITEM>& mSisrItem)
{
	CBrdgSeisEvalResult_JP* pResult = m_pDoc->m_pPostCtrl->GetBrdgSeisEvalResult_JP(); if (!pResult) { ASSERT(0); return; }

	T_SISR_D dSisr; T_SIPA_D dSipa;
	if (m_pDoc->m_pAttrCtrl2->GetQSisr()->Get(kSisr, dSisr) == FALSE) return;
	if (m_pDoc->m_pAttrCtrl2->GetQSipa()->Get(dSisr.kSipa, dSipa) == FALSE) return;

	T_ELEM_K_LIST kElemList;
	m_pDoc->m_pAttrCtrl2->GetQSisr()->GetElementKeys(kSisr, kElemList);

	for (int i = 0; i < kElemList.GetCount(); i++)
	{
		const T_ELEM_K& kElem = kElemList[i];

		for (int nShearDir = seis_jp::kY_Dir; nShearDir < 3; nShearDir++)
		{
			T_SISR_EVAL_ROAD3 eval;
			eval.kSipa = dSisr.kSipa;
			eval.kSigr = dSisr.kSigr;
			eval.kElem = kElem;
			eval.nExamShearDir = nShearDir;
			if (pResult->GetResultSisrRoad3(eval) == FALSE) continue;

			SUMM_EVAL_K minKey(SUMM_EVAL_K::kMin, nShearDir, dSipa.nSeismicForceDirection, DB_MEMB_TYPE::D_MBTP_ETC);
			SetSisrItemMapMinUpper(kElem, dSipa, eval, minKey.keyMap, mSisrItem);

			SUMM_EVAL_K maxKey(SUMM_EVAL_K::kMax, nShearDir, dSipa.nSeismicForceDirection, DB_MEMB_TYPE::D_MBTP_ETC);
			SetSisrItemMapMaxUpper(kElem, dSipa, eval, maxKey.keyMap, mSisrItem);
		}
	}
}

void CSeismicDesignSumm::SummaryLowerStructure()
{
	CBrdgSeisEvalResult_JP* pResult = m_pDoc->m_pPostCtrl->GetBrdgSeisEvalResult_JP(); if (!pResult) { ASSERT(0); return; }

	CArray<T_SIGR_K, T_SIGR_K> sigrKeys;
	std::map<T_SIGR_K, std::vector<T_SIBD_K>> mSibd_ALL;
	std::map<T_SIGR_K, std::vector<T_SIBD_K>> mSibd_CVSV;
	std::map<T_SIGR_K, std::vector<T_SISR_K>> mSisr_LOWER;
	m_pDoc->m_pAttrCtrl2->GetQSigr()->GetKeyList(sigrKeys);
	GetSibdkeysBySigr(T_SIBD_D::kUnKnown, mSibd_ALL);
	GetSibdkeysBySigr(T_SIBD_D::kCurvatureSurvey, mSibd_CVSV);
	GetSisrLowerKeyMap(mSisr_LOWER);

	for (int i = 0; i < sigrKeys.GetCount(); i++) {
		const T_SIGR_K& kSigr = sigrKeys[i];
		T_EVAL_SUMM_LOWER summary;
		summary.kSigr = kSigr;

		T_SIGR_D dSigr;
		if (m_pDoc->m_pAttrCtrl2->GetQSigr()->Get(kSigr, dSigr) == FALSE) continue;
		if (dSigr.nSubType != T_SIGR_D::kLower) continue;

		if (mSibd_CVSV.find(kSigr) != mSibd_CVSV.end()) {
			GetSummarySibdCvsv(mSibd_CVSV[kSigr], summary.CvsvSum);
			GetSummarySibdSteel(mSibd_CVSV[kSigr], summary.StelSum);
			GetSummarySibdRebarCutSrsv(mSibd_ALL[kSigr], summary.RebarCutSrsvSum);
		}
		if (mSisr_LOWER.find(kSigr) != mSisr_LOWER.end()) {
			GetSummarySisrLower(mSisr_LOWER[kSigr], summary.SisrSum);
			GetSummarySisrRebarCut(mSisr_LOWER[kSigr], summary.RebarCutSisrSum);
		}

		if (summary.CvsvSum.CvsvMin.GetCount() < 1 && summary.CvsvSum.CvsvMax.GetCount() < 1 &&
			summary.SisrSum.SisrMin.GetCount() < 1 && summary.SisrSum.SisrMax.GetCount() < 1 &&
			summary.RebarCutSrsvSum.SrsvMin.GetCount() < 1 && summary.RebarCutSrsvSum.SrsvMax.GetCount() < 1 &&
			summary.RebarCutSisrSum.SisrMin.GetCount() < 1 && summary.RebarCutSisrSum.SisrMax.GetCount() < 1 &&
			summary.StelSum.GetCount() < 1) {
			continue;
		}

		pResult->SetSummaryLower(kSigr, summary);
	}
}

void CSeismicDesignSumm::GetSummarySibdRebarCutSrsv(const std::vector<T_SIBD_K>& sibdKeys, OUT T_SIBD_SRSV_DATA& rSummary)
{
	//최대/최소, 교축방향/교축직각방향, 조사(Y/Z)방향 8가지 경우 각각에 해당하는 가장 불리한 값을 찾아낸다.
	std::map<SUMM_EVAL_KEY, T_SIBD_SRSV_ITEM> mCvsvItem;

	for (auto kSibd : sibdKeys)	{
		GetSrsvItemFromRebarCut(kSibd, mCvsvItem);
	}

	for (const auto& pair : mCvsvItem)
	{
		SUMM_EVAL_K evalKey;
		evalKey.keyMap = pair.first;

		switch (evalKey.Key.nMinMax)
		{
		case SUMM_EVAL_K::kMin:
			rSummary.SrsvMin.Add(pair.second);
			break;
		case SUMM_EVAL_K::kMax:
			rSummary.SrsvMax.Add(pair.second);
			break;
		default:
			break;
		}
	}
}

void SetSrsvItemMapMinRebarCut(const T_SIBD_K& kSibd, const T_ELEM_K& kElem, const T_SIPA_D& dSipa, const T_SIBD_EVAL_REBAR_CUT& eval, const UINT& keyMap, OUT std::map<SUMM_EVAL_KEY, T_SIBD_SRSV_ITEM>& mSrsvItem)
{
	T_SIBD_SRSV_ITEM item;
	BOOL bNew = mSrsvItem.find(keyMap) == mSrsvItem.end();
	if (bNew == FALSE) {
		item = mSrsvItem[keyMap];
		if (eval.resultRc.dRatioMin <= item.dRatio)
			return;
	}

	item.kSibd = kSibd;
	item.kElem = kElem;
	item.nMemberType = DB_MEMB_TYPE::D_MBTP_COLUMN;
	item.nMaterialType = T_SIGR_LOWER_D::eMaterialType::kRC;
	item.nSeismicForceType = dSipa.nSeismicForceType;
	item.nSeismicForceDirection = dSipa.nSeismicForceDirection;
	item.nExamShearDir = eval.nExamShearDir;
	item.dMmax = eval.analStrength.dAvergeMin;
	item.dMy = eval.calcStrengthMin.dY;
	item.dRatio = eval.resultRc.dRatioMin;

	if (bNew) mSrsvItem.insert(std::make_pair(keyMap, item));
	else mSrsvItem[keyMap] = item;
}

void SetSrsvItemMapMaxRebarCut(const T_SIBD_K& kSibd, const T_ELEM_K& kElem, const T_SIPA_D& dSipa, const T_SIBD_EVAL_REBAR_CUT& eval, const UINT& keyMap, OUT std::map<SUMM_EVAL_KEY, T_SIBD_SRSV_ITEM>& mSrsvItem)
{
	T_SIBD_SRSV_ITEM item;
	BOOL bNew = mSrsvItem.find(keyMap) == mSrsvItem.end();
	if (bNew == FALSE) {
		item = mSrsvItem[keyMap];
		if (eval.resultRc.dRatioMax <= item.dRatio)
			return;
	}

	item.kSibd = kSibd;
	item.kElem = kElem;
	item.nMemberType = DB_MEMB_TYPE::D_MBTP_COLUMN;
	item.nMaterialType = T_SIGR_LOWER_D::eMaterialType::kRC;
	item.nSeismicForceType = dSipa.nSeismicForceType;
	item.nSeismicForceDirection = dSipa.nSeismicForceDirection;
	item.nExamShearDir = eval.nExamShearDir;
	item.dMmax = eval.analStrength.dAvergeMax;
	item.dMy = eval.calcStrengthMax.dY;
	item.dRatio = eval.resultRc.dRatioMax;

	if (bNew) mSrsvItem.insert(std::make_pair(keyMap, item));
	else mSrsvItem[keyMap] = item;
}

void CSeismicDesignSumm::GetSrsvItemFromRebarCut(T_SIBD_K kSibd, OUT std::map<SUMM_EVAL_KEY, T_SIBD_SRSV_ITEM>& mSrsvItem)
{
	CBrdgSeisEvalResult_JP* pResult = m_pDoc->m_pPostCtrl->GetBrdgSeisEvalResult_JP(); if (!pResult) { ASSERT(0); return; }

	T_SIBD_D SibdD; T_SIPA_D SipaD; T_SIGR_D SigrD; T_SIGR_LOWER_D dLower;
	if (m_pDoc->m_pAttrCtrl2->GetQSibd()->Get(kSibd, SibdD) == FALSE) return;
	if (!m_pDoc->m_pAttrCtrl2->GetQSipa()->Get(SibdD.kSipa, SipaD)) return;
	if (!m_pDoc->m_pAttrCtrl2->GetQSigr()->Get(SibdD.kSigr, SigrD)) return;
	if (!m_pDoc->m_pAttrCtrl2->GetQSigrLower()->Get(SibdD.kSigr, dLower)) return;
	if (SigrD.nSubType != T_SIGR_D::kLower) return;

	for (int iColumn = 0; iColumn < dLower.COLM_D.GetCount(); iColumn++)
	{
		const T_SIGR_LOWER_COLM_D& colm = dLower.COLM_D[iColumn];
		if (colm.bRebarCut == FALSE) continue;

		const auto ElemK = (SipaD.nSeismicForceDirection == T_SIPA_D::kAxis_Dir) ? colm.kAxialElem : colm.kAxisPerpElem;

		for (int nShearDir = seis_jp::kY_Dir; nShearDir < 3; nShearDir++)	{

			T_SIBD_EVAL_REBAR_CUT eval;
			eval.kSipa = SibdD.kSipa;
			eval.kSigr = SibdD.kSigr;
			eval.kElem = ElemK;
			eval.nExamShearDir = nShearDir;
			if (!pResult->GetResultSibdRebarCut(eval)) continue;

			SUMM_EVAL_K minKey(SUMM_EVAL_K::kMin, nShearDir, SipaD.nSeismicForceDirection, DB_MEMB_TYPE::D_MBTP_COLUMN);
			SetSrsvItemMapMinRebarCut(kSibd, ElemK, SipaD, eval, minKey.keyMap, mSrsvItem);

			SUMM_EVAL_K maxKey(SUMM_EVAL_K::kMax, nShearDir, SipaD.nSeismicForceDirection, DB_MEMB_TYPE::D_MBTP_COLUMN);
			SetSrsvItemMapMaxRebarCut(kSibd, ElemK, SipaD, eval, maxKey.keyMap, mSrsvItem);
		}
	}
}

void CSeismicDesignSumm::GetSummarySisrRebarCut(const std::vector<T_SISR_K>& sisrKeys, OUT T_SISR_SUMM_DATA& rSummary)
{
	//최대/최소, 교축방향/교축직각방향 4가지 경우 각각에 해당하는 가장 불리한 값을 찾아낸다.
	std::map<SUMM_EVAL_KEY, T_SISR_SUMM_ITEM> mSisrItem;

	for (auto kSisr : sisrKeys)
	{
		T_SISR_D dSisr;
		if (m_pDoc->m_pAttrCtrl2->GetQSisr()->Get(kSisr, dSisr) == FALSE) continue;
		if (dSisr.AUTO_D.nDesignCriteria == T_SISR_AUTO_D::kRoad3) continue;

		GetSisrItemFromRebarCut(kSisr, mSisrItem);
	}

	for (const auto& pair : mSisrItem)
	{
		SUMM_EVAL_K evalKey;
		evalKey.keyMap = pair.first;

		switch (evalKey.Key.nMinMax)
		{
		case SUMM_EVAL_K::kMin:
			rSummary.SisrMin.Add(const_cast<T_SISR_SUMM_ITEM&>(pair.second));
			break;
		case SUMM_EVAL_K::kMax:
			rSummary.SisrMax.Add(const_cast<T_SISR_SUMM_ITEM&>(pair.second));
			break;
		default:
			break;
		}
	}
}

void SetSisrItemMapMinRebarCut(const T_ELEM_K& kElem, const T_SIPA_D& dSipa, const T_SISR_EVAL_REBAR_CUT& eval, const UINT& keyMap, std::map<SUMM_EVAL_KEY, T_SISR_SUMM_ITEM>& mSisrItem)
{
	auto updateItem = [&](T_SISR_SUMM_ITEM& item, const T_SISR_EVAL_REBAR_CUT_VALUE& rsltMin) {
		item.dPs = rsltMin.dPs;
		item.dSmax = rsltMin.dSmax;
		item.dRatio = rsltMin.dRatio;
	};

	auto emplaceOrUpdate = [&](T_SISR_SUMM_ITEM& item, const T_SISR_EVAL_REBAR_CUT_VALUE& rsltMin) {
		updateItem(item, rsltMin);
		item.kElem = kElem;
		item.nExamShearDir = eval.nExamShearDir;
		item.nMaterialType = T_SIGR_LOWER_D::kRC;
		item.nDesignCriteria = T_SISR_AUTO_D::kRoad0;
		item.nMemberType = DB_MEMB_TYPE::D_MBTP_COLUMN;
		item.nSeismicForceType = dSipa.nSeismicForceType;
		item.nSeismicForceDirection = dSipa.nSeismicForceDirection;
	};

	auto it = mSisrItem.find(keyMap);
	if (it == mSisrItem.end()) {
		T_SISR_SUMM_ITEM item;
		emplaceOrUpdate(item, eval.RsltMin);
		mSisrItem[keyMap] = item;
	}
	else {
		T_SISR_SUMM_ITEM& item = it->second;
		double newRatio = 0;
		newRatio = eval.RsltMin.dRatio;
		if (newRatio > item.dRatio) {
			emplaceOrUpdate(item, eval.RsltMin);
		}
	}
}

void SetSisrItemMapMaxRebarCut(const T_ELEM_K& kElem, const T_SIPA_D& dSipa, const T_SISR_EVAL_REBAR_CUT& eval, const UINT& keyMap, OUT std::map<SUMM_EVAL_KEY, T_SISR_SUMM_ITEM>& mSisrItem)
{
	auto updateItem = [&](T_SISR_SUMM_ITEM& item, const T_SISR_EVAL_REBAR_CUT_VALUE& rsltMax) {
		item.dPs = rsltMax.dPs;
		item.dSmax = rsltMax.dSmax;
		item.dRatio = rsltMax.dRatio;
		};

	auto emplaceOrUpdate = [&](T_SISR_SUMM_ITEM& item, const T_SISR_EVAL_REBAR_CUT_VALUE& rsltMax) {
		updateItem(item, rsltMax);
		item.kElem = kElem;
		item.nExamShearDir = eval.nExamShearDir;
		item.nMaterialType = T_SIGR_LOWER_D::kRC;
		item.nDesignCriteria = T_SISR_AUTO_D::kRoad0;
		item.nMemberType = DB_MEMB_TYPE::D_MBTP_COLUMN;
		item.nSeismicForceType = dSipa.nSeismicForceType;
		item.nSeismicForceDirection = dSipa.nSeismicForceDirection;
		};

	auto it = mSisrItem.find(keyMap);
	if (it == mSisrItem.end()) {
		T_SISR_SUMM_ITEM item;
		emplaceOrUpdate(item, eval.RsltMax);
		mSisrItem[keyMap] = item;
	}
	else {
		T_SISR_SUMM_ITEM& item = it->second;
		double newRatio = 0;
		newRatio = eval.RsltMax.dRatio;
		if (newRatio > item.dRatio) {
			emplaceOrUpdate(item, eval.RsltMax);
		}
	}
}

void CSeismicDesignSumm::GetSisrItemFromRebarCut(const T_SISR_K& kSisr, OUT std::map<SUMM_EVAL_KEY, T_SISR_SUMM_ITEM>& mSisrItem)
{
	CBrdgSeisEvalResult_JP* pResult = m_pDoc->m_pPostCtrl->GetBrdgSeisEvalResult_JP(); if (!pResult) { ASSERT(0); return; }

	T_SISR_D dSisr; T_SIPA_D dSipa; T_SIGR_D SigrD; T_SIGR_LOWER_D dLower;
	if (m_pDoc->m_pAttrCtrl2->GetQSisr()->Get(kSisr, dSisr) == FALSE) return;
	if (m_pDoc->m_pAttrCtrl2->GetQSipa()->Get(dSisr.kSipa, dSipa) == FALSE) return;
	if (!m_pDoc->m_pAttrCtrl2->GetQSigr()->Get(dSisr.kSigr, SigrD)) return;
	if (!m_pDoc->m_pAttrCtrl2->GetQSigrLower()->Get(dSisr.kSigr, dLower)) return;
	if (SigrD.nSubType != T_SIGR_D::kLower) return;

	for (int iColumn = 0; iColumn < dLower.COLM_D.GetCount(); iColumn++)
	{
		const T_SIGR_LOWER_COLM_D& colm = dLower.COLM_D[iColumn];
		if (colm.bRebarCut == FALSE) continue;

		const auto kElem = (dSipa.nSeismicForceDirection == T_SIPA_D::kAxis_Dir) ? colm.kAxialElem : colm.kAxisPerpElem;

		for (int nShearDir = seis_jp::kY_Dir; nShearDir < 3; nShearDir++) {

			T_SISR_EVAL_REBAR_CUT eval;
			eval.kSipa = dSisr.kSipa;
			eval.kSigr = dSisr.kSigr;
			eval.kElem = kElem;
			eval.nExamShearDir = nShearDir;
			if (!pResult->GetResultSisrRebarCut(eval)) continue;

			SUMM_EVAL_K minKey(SUMM_EVAL_K::kMin, nShearDir, dSipa.nSeismicForceDirection, DB_MEMB_TYPE::D_MBTP_COLUMN);
			SetSisrItemMapMinRebarCut(kElem, dSipa, eval, minKey.keyMap, mSisrItem);

			SUMM_EVAL_K maxKey(SUMM_EVAL_K::kMax, nShearDir, dSipa.nSeismicForceDirection, DB_MEMB_TYPE::D_MBTP_COLUMN);
			SetSisrItemMapMaxRebarCut(kElem, dSipa, eval, maxKey.keyMap, mSisrItem);
		}
	}
}

void CSeismicDesignSumm::GetSummarySisrLower(const std::vector<T_SISR_K>& sisrKeys, OUT T_SISR_SUMM_DATA& rSummary)
{
	//최대/최소, 교축방향/교축직각방향 4가지 경우 각각에 해당하는 가장 불리한 값을 찾아낸다.
	std::map<SUMM_EVAL_KEY, T_SISR_SUMM_ITEM> mSisrItem;

	for (auto kSisr : sisrKeys)
	{
		T_SISR_D dSisr;
		if (m_pDoc->m_pAttrCtrl2->GetQSisr()->Get(kSisr, dSisr) == FALSE) continue;
		if (dSisr.AUTO_D.nDesignCriteria == T_SISR_AUTO_D::kRoad3) continue;

		GetSisrItemFromLower(kSisr, mSisrItem);
	}

	for (const auto& pair : mSisrItem)
	{
		SUMM_EVAL_K evalKey;
		evalKey.keyMap = pair.first;

		switch (evalKey.Key.nMinMax)
		{
		case SUMM_EVAL_K::kMin:
			rSummary.SisrMin.Add(const_cast<T_SISR_SUMM_ITEM&>(pair.second));
			break;
		case SUMM_EVAL_K::kMax:
			rSummary.SisrMax.Add(const_cast<T_SISR_SUMM_ITEM&>(pair.second));
			break;
		default:
			break;
		}
	}
}

T_SISR_USER_D::ePosType GetUnfavorablePosOfLower(const T_SISR_EVAL_LOWER& eval) {

	auto maxRatio = [](double& max, double ratio, T_SISR_USER_D::ePosType& pos, T_SISR_USER_D::ePosType myPos) {
		if (ratio > max) { max = ratio; pos = myPos; }
		};

	T_SISR_USER_D::ePosType nPos = T_SISR_USER_D::kUnKnown;
	double dMax = -DBL_MAX;
	maxRatio(dMax, eval.PosI.RsltMin.dRatio, nPos, T_SISR_USER_D::kI);
	maxRatio(dMax, eval.PosJ.RsltMin.dRatio, nPos, T_SISR_USER_D::kJ);
	maxRatio(dMax, eval.PosI.RsltMin.dRatio, nPos, T_SISR_USER_D::kI);
	maxRatio(dMax, eval.PosJ.RsltMin.dRatio, nPos, T_SISR_USER_D::kJ);

	return nPos;
}

void SetSisrItemMapMinLower(const T_ELEM_K& kElem, const T_SIPA_D& dSipa, const T_SISR_EVAL_LOWER& eval, const UINT& keyMap, std::map<SUMM_EVAL_KEY, T_SISR_SUMM_ITEM>& mSisrItem) 
{
	T_SISR_USER_D::ePosType ePos = GetUnfavorablePosOfLower(eval);

	auto updateItem = [&](T_SISR_SUMM_ITEM& item, const auto& rsltMin) {
		item.dPs = rsltMin.dPs;
		item.dSmax = rsltMin.dShear;
		item.dRatio = rsltMin.dRatio;
		};

	auto emplaceOrUpdate = [&](T_SISR_SUMM_ITEM& item, const auto& rsltMin) {
		updateItem(item, rsltMin);
		item.kElem = kElem;
		item.nMemberType = eval.nMemberType;
		item.nExamShearDir = eval.nExamShearDir;
		item.nMaterialType = eval.nMaterialType;
		item.nDesignCriteria = T_SISR_AUTO_D::kRoad0;
		item.nSeismicForceType = dSipa.nSeismicForceType;
		item.nSeismicForceDirection = dSipa.nSeismicForceDirection;
		};

	auto it = mSisrItem.find(keyMap);
	if (it == mSisrItem.end()) {
		T_SISR_SUMM_ITEM item;
		(ePos == T_SISR_USER_D::kI) ? emplaceOrUpdate(item, eval.PosI.RsltMin) : emplaceOrUpdate(item, eval.PosJ.RsltMin);
		mSisrItem[keyMap] = item;
	}
	else {
		T_SISR_SUMM_ITEM& item = it->second;
		double newRatio = 0;
		if (ePos == T_SISR_USER_D::kI)
			newRatio = eval.PosI.RsltMin.dRatio;
		else
			newRatio = eval.PosJ.RsltMin.dRatio;
		if (newRatio > item.dRatio) {
			emplaceOrUpdate(item, (ePos == T_SISR_USER_D::kI) ? eval.PosI.RsltMin : eval.PosJ.RsltMin);
		}
	}
}

void SetSisrItemMapMaxLower(const T_ELEM_K& kElem, const T_SIPA_D& dSipa, const T_SISR_EVAL_LOWER& eval, const UINT& keyMap, OUT std::map<SUMM_EVAL_KEY, T_SISR_SUMM_ITEM>& mSisrItem)
{
	T_SISR_USER_D::ePosType ePos = GetUnfavorablePosOfLower(eval);

	auto updateItem = [&](T_SISR_SUMM_ITEM& item, const auto& rsltMax) {
		item.dPs = rsltMax.dPs;
		item.dSmax = rsltMax.dShear;
		item.dRatio = rsltMax.dRatio;
		};

	auto emplaceOrUpdate = [&](T_SISR_SUMM_ITEM& item, const auto& rsltMax) {
		updateItem(item, rsltMax);
		item.kElem = kElem;
		item.nMemberType = eval.nMemberType;
		item.nExamShearDir = eval.nExamShearDir;
		item.nMaterialType = eval.nMaterialType;
		item.nDesignCriteria = T_SISR_AUTO_D::kRoad0;
		item.nSeismicForceType = dSipa.nSeismicForceType;
		item.nSeismicForceDirection = dSipa.nSeismicForceDirection;
		};

	auto it = mSisrItem.find(keyMap);
	if (it == mSisrItem.end()) {
		T_SISR_SUMM_ITEM item;
		(ePos == T_SISR_USER_D::kI) ? emplaceOrUpdate(item, eval.PosI.RsltMax) : emplaceOrUpdate(item, eval.PosJ.RsltMax);
		mSisrItem[keyMap] = item;
	}
	else {
		T_SISR_SUMM_ITEM& item = it->second;
		double newRatio = 0;
		if (ePos == T_SISR_USER_D::kI)
			newRatio = eval.PosI.RsltMax.dRatio;
		else
			newRatio = eval.PosJ.RsltMax.dRatio;
		if (newRatio > item.dRatio) {
			emplaceOrUpdate(item, (ePos == T_SISR_USER_D::kI) ? eval.PosI.RsltMax : eval.PosJ.RsltMax);
		}
	}
}

void CSeismicDesignSumm::GetSisrItemFromLower(const T_SISR_K& kSisr, OUT std::map<SUMM_EVAL_KEY, T_SISR_SUMM_ITEM>& mSisrItem)
{
	CBrdgSeisEvalResult_JP* pResult = m_pDoc->m_pPostCtrl->GetBrdgSeisEvalResult_JP(); if (!pResult) { ASSERT(0); return; }

	T_SISR_D dSisr; T_SIPA_D dSipa;
	if (m_pDoc->m_pAttrCtrl2->GetQSisr()->Get(kSisr, dSisr) == FALSE) return;
	if (m_pDoc->m_pAttrCtrl2->GetQSipa()->Get(dSisr.kSipa, dSipa) == FALSE) return;

	T_ELEM_K_LIST kElemList;
	m_pDoc->m_pAttrCtrl2->GetQSisr()->GetElementKeys(kSisr, kElemList);

	for (int i = 0; i < kElemList.GetCount(); i++)
	{
		const T_ELEM_K& kElem = kElemList[i];

		for (int nShearDir = seis_jp::kY_Dir; nShearDir < 3; nShearDir++)
		{
			T_SISR_EVAL_LOWER eval;
			eval.kSipa = dSisr.kSipa;
			eval.kSigr = dSisr.kSigr;
			eval.kElem = kElem;
			eval.nExamShearDir = nShearDir;
			if (pResult->GetResultSisrLower(eval) == FALSE) continue;

			SUMM_EVAL_K minKey(SUMM_EVAL_K::kMin, nShearDir, dSipa.nSeismicForceDirection, eval.nMemberType);
			SetSisrItemMapMinLower(kElem, dSipa, eval, minKey.keyMap, mSisrItem);

			SUMM_EVAL_K maxKey(SUMM_EVAL_K::kMax, nShearDir, dSipa.nSeismicForceDirection, eval.nMemberType);
			SetSisrItemMapMaxLower(kElem, dSipa, eval, maxKey.keyMap, mSisrItem);
		}
	}
}

void CSeismicDesignSumm::GetSummarySibdSteel(const std::vector<T_SIBD_K>& sibdKeys, OUT CArray<T_SIBD_STEL_ITEM, T_SIBD_STEL_ITEM>& rSummary)
{
	//교축방향/교축직각방향 2가지 경우 각각에 해당하는 가장 불리한 값을 찾아낸다.
	std::map<int, std::map<int, T_SIBD_STEL_ITEM>> mStelItem;

	for (auto kSibd : sibdKeys)
	{
		T_SIBD_D dSibd;
		if (m_pDoc->m_pAttrCtrl2->GetQSibd()->Get(kSibd, dSibd) == FALSE) return;

		switch (dSibd.CVSV_D.nCvsvType)
		{
		case T_SIBD_CVSV_D::kSurrenderSurvey:
			GetStelItemFromSurrender(kSibd, mStelItem);
			break;
		case T_SIBD_CVSV_D::kLimitStateSurvey:
			GetStelItemFromLimitState(kSibd, mStelItem);
			break;
		default:
			break;
		}
	}

	for (const auto& pair : mStelItem) {
		for (const auto& pair2 : mStelItem[pair.first])
		{
			rSummary.Add(pair2.second);
		}
	}
}

void CSeismicDesignSumm::GetStelItemFromSurrender(const T_SIBD_K& kSibd, OUT std::map<int, std::map<int, T_SIBD_STEL_ITEM>>& mStelItem)
{
	CBrdgSeisEvalResult_JP* pResult = m_pDoc->m_pPostCtrl->GetBrdgSeisEvalResult_JP(); 
	if (!pResult) { ASSERT(0); return; }

	T_SIBD_D dSibd; T_SIPA_D dSipa;
	if (m_pDoc->m_pAttrCtrl2->GetQSibd()->Get(kSibd, dSibd) == FALSE) return;
	if (m_pDoc->m_pAttrCtrl2->GetQSipa()->Get(dSibd.kSipa, dSipa) == FALSE) return;

	auto updateItem = [&](T_SIBD_STEL_ITEM& item, const T_ELEM_K& kElem, const T_SIBD_EVAL_PHI_Y_ST& eval) {
		item.kElem = kElem;
		item.nSurveyType = T_SIBD_CVSV_D::kSurrenderSurvey;
		item.nMemberType = eval.nMemberType;
		item.nMaterialType = eval.nMaterialType;
		item.nSeismicForceType = dSipa.nSeismicForceType;
		item.nSeismicForceDirection = dSipa.nSeismicForceDirection;
		item.dDeltaMax = eval.dAverge;
		item.dDeltaYc = eval.calcDeform.dPhi_c;
		item.dDeltaYt = eval.calcDeform.dPhi_y;
		//item.dDeltaA = eval.calcDeformMax.dPhi_a;
		item.dRatioYc = eval.resultSt.dRatioYc;
		item.dRatioYt = eval.resultSt.dRatioYt;
		item.nExamShearDir = eval.nExamShearDir;
	};

	T_ELEM_K_LIST kElemList;
	m_pDoc->m_pAttrCtrl2->GetQSibd()->GetElementKeys(kSibd, kElemList);

	for (int i = 0; i < kElemList.GetCount(); i++)
	{
		const T_ELEM_K& kElem = kElemList[i];

		for (int nShearDir = seis_jp::kY_Dir; nShearDir < 3; nShearDir++)
		{
			T_SIBD_EVAL_PHI_Y_ST eval;
			eval.kSipa = dSibd.kSipa;
			eval.kSigr = dSibd.kSigr;
			eval.kElem = kElem;
			eval.nExamShearDir = nShearDir;
			if (pResult->GetResultSibdPhiY_ST(eval) == FALSE) continue;
			if (eval.nMaterialType != T_SIGR_LOWER_D::kSteel) continue;

			auto it = mStelItem.find(dSipa.nSeismicForceDirection);
			if (it == mStelItem.end()) {
				T_SIBD_STEL_ITEM item;
				updateItem(item, kElem, eval);
				mStelItem[dSipa.nSeismicForceDirection].insert(std::make_pair(eval.nExamShearDir, item));
			}
			else {
				auto it2 = it->second.find(eval.nExamShearDir);
				if (it2 == mStelItem[dSipa.nSeismicForceDirection].end())
				{
					T_SIBD_STEL_ITEM item;
					updateItem(item, kElem, eval);
					mStelItem[dSipa.nSeismicForceDirection].insert(std::make_pair(eval.nExamShearDir, item));
				}
				else
				{
					T_SIBD_STEL_ITEM& item = it2->second;
					if (isnan(eval.resultSt.dRatioYt) != 0 || isnan(item.dRatioYt) != 0)
						continue;

					if (eval.resultSt.dRatioYt > item.dRatioYt) {
						updateItem(item, kElem, eval);
					}
				}
			}
		}
	}
}

void CSeismicDesignSumm::GetStelItemFromLimitState(const T_SIBD_K& kSibd, std::map<int, std::map<int, T_SIBD_STEL_ITEM>>& mStelItem)
{
	CBrdgSeisEvalResult_JP* pResult = m_pDoc->m_pPostCtrl->GetBrdgSeisEvalResult_JP();
	if (!pResult) { ASSERT(0); return; }

	T_SIBD_D dSibd; T_SIPA_D dSipa;
	if (!m_pDoc->m_pAttrCtrl2->GetQSibd()->Get(kSibd, dSibd)) return;
	if (!m_pDoc->m_pAttrCtrl2->GetQSipa()->Get(dSibd.kSipa, dSipa)) return;

	T_ELEM_K_LIST kElemList;
	m_pDoc->m_pAttrCtrl2->GetQSibd()->GetElementKeys(kSibd, kElemList);

	auto updateItem = [&](T_SIBD_STEL_ITEM& item, const T_ELEM_K& kElem, const T_SIBD_EVAL_PHI_A_ST& eval) {
		item.kElem = kElem;
		item.nSurveyType = T_SIBD_CVSV_D::kLimitStateSurvey;
		item.nMemberType = eval.nMemberType;
		item.nMaterialType = eval.nMaterialType;
		item.nSeismicForceType = dSipa.nSeismicForceType;
		item.nSeismicForceDirection = dSipa.nSeismicForceDirection;
		item.dDeltaMax = eval.dAverge;
		item.dDeltaYc = eval.calcDeform.dPhi_c;
		item.dDeltaYt = eval.calcDeform.dPhi_y;
		item.dDeltaA = eval.calcDeform.dPhi_a;
		item.dRatioYc = eval.resultSt.dRatioYc;
		item.dRatioYt = eval.resultSt.dRatioYt;
		item.nExamShearDir = eval.nExamShearDir;
	};

	for (int i = 0; i < kElemList.GetCount(); i++)
	{
		const T_ELEM_K& kElem = kElemList[i];

		for (int nShearDir = seis_jp::kY_Dir; nShearDir < 3; nShearDir++)
		{
			T_SIBD_EVAL_PHI_A_ST eval;
			eval.kSipa = dSibd.kSipa;
			eval.kSigr = dSibd.kSigr;
			eval.kElem = kElem;
			eval.nExamShearDir = nShearDir;
			if (!pResult->GetResultSibdPhiA_ST(eval)) continue;
			if (eval.nMaterialType != T_SIGR_LOWER_D::kSteel) continue;

			auto it = mStelItem.find(dSipa.nSeismicForceDirection);
			if (it == mStelItem.end()) {
				T_SIBD_STEL_ITEM item;
				updateItem(item, kElem, eval);
				mStelItem[dSipa.nSeismicForceDirection].insert(std::make_pair(eval.nExamShearDir, item));
			}
			else {
				auto it2 = it->second.find(eval.nExamShearDir);
				if (it2 == mStelItem[dSipa.nSeismicForceDirection].end())
				{
					T_SIBD_STEL_ITEM item;
					updateItem(item, kElem, eval);
					mStelItem[dSipa.nSeismicForceDirection].insert(std::make_pair(eval.nExamShearDir, item));
				}
				else
				{
					T_SIBD_STEL_ITEM& item = it2->second;
					if (isnan(eval.resultSt.dRatioYt) != 0 || isnan(item.dRatioYt) != 0)
						continue;

					if (eval.resultSt.dRatioYt > item.dRatioYt) {
						updateItem(item, kElem, eval);
					}
				}
			}
		}
	}
}