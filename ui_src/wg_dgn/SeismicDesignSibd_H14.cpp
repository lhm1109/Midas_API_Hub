#include "stdafx.h"
#include "SeismicDesignSibd_H14.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_PostCtrl.h"
#include "..\wg_db\wg_db_ThisInfo.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl2.h"
#include "..\wg_db\DB_ST_DT_SIBD_EVAL.h"
#include "..\wg_db\DB_QUERY_SEIS_CVL_JP.h"
#include "..\wg_db\BrdgSeisEvalResult_JP.h"

#include "DgnProgressDlg.h"
#include "BrdgSeisEval_JP.h"
#include "DgnToolMPhi_SeismicDesign_JP.h"

#pragma region >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> CSeismicDesignSibd_H14 

CSeismicDesignSibd_H14::CSeismicDesignSibd_H14() : CSeismicDesignDisp()
{
	m_kSibd = 0;
	m_kSipa = 0;
	m_kSigr = 0;
	m_nMaterialType = T_SIGR_LOWER_D::kUnKnown;
	m_mMemberMap.clear();
}

CSeismicDesignSibd_H14::~CSeismicDesignSibd_H14()
{
}

void CSeismicDesignSibd_H14::AddProgressStep(CDgnProgressDlg* pProgress)
{
	if (m_pDoc == NULL) return;

	T_KEY_LIST sibdKeys;
	m_pDoc->m_pAttrCtrl2->GetQSibd()->GetKeyList(sibdKeys);

	for (int idx = 0; idx < sibdKeys.GetCount(); idx++)
	{
		T_SIBD_K kSibd = sibdKeys[idx];
		T_SIBD_D dSibd; T_SIPA_D dSipa; T_SIGR_D dSigr;
		if (m_pDoc->m_pAttrCtrl2->GetQSibd()->Get(kSibd, dSibd) == FALSE) continue;
		if (m_pDoc->m_pAttrCtrl2->GetQSipa()->Get(dSibd.kSipa, dSipa) == FALSE) continue;
		if (m_pDoc->m_pAttrCtrl2->GetQSigr()->Get(dSibd.kSigr, dSigr) == FALSE) continue;
		if (dSipa.nDesignCode != GetCurrentDgnCode()) continue;

		switch (dSibd.nSibdType)
		{
		case T_SIBD_D::kStrengthSurvey:
			CSeismicDesignSibd_H14_Strength(pProgress, kSibd).AddProgressStep();
			break;
		case T_SIBD_D::kCurvatureSurvey:
			if (dSibd.CVSV_D.nCvsvType == T_SIBD_CVSV_D::kSurrenderSurvey)
				CSeismicDesignSibd_H14_Surrender(pProgress, kSibd).AddProgressStep();
			else if (dSibd.CVSV_D.nCvsvType == T_SIBD_CVSV_D::kLimitStateSurvey)
				CSeismicDesignSibd_H14_LimitState(pProgress, kSibd).AddProgressStep();
			break;
		default:
			break;
		}

		if (dSigr.nSubType == T_SIGR_D::kLower)
			CSeismicDesignSibd_H14_RebarReducedPart(pProgress, kSibd).AddProgressStep();
	}
}

BOOL CSeismicDesignSibd_H14::Calculate(CDgnProgressDlg* pProgress)
{
	if (m_pDoc == NULL) return FALSE;

	T_KEY_LIST sibdKeys;
	m_pDoc->m_pAttrCtrl2->GetQSibd()->GetKeyList(sibdKeys);

	for (int idx = 0; idx < sibdKeys.GetCount(); idx++)
	{
		T_SIBD_K kSibd = sibdKeys[idx];
		T_SIBD_D dSibd; T_SIPA_D dSipa; T_SIGR_D dSigr;
		if (m_pDoc->m_pAttrCtrl2->GetQSibd()->Get(kSibd, dSibd) == FALSE) continue;
		if (m_pDoc->m_pAttrCtrl2->GetQSipa()->Get(dSibd.kSipa, dSipa) == FALSE) continue;
		if (m_pDoc->m_pAttrCtrl2->GetQSigr()->Get(dSibd.kSigr, dSigr) == FALSE) continue;
		if (dSipa.nDesignCode != GetCurrentDgnCode()) continue;

		switch (dSibd.nSibdType)
		{
		case T_SIBD_D::kStrengthSurvey:
			CSeismicDesignSibd_H14_Strength::Calculate(pProgress, kSibd);
			break;
		case T_SIBD_D::kCurvatureSurvey:
			if (dSibd.CVSV_D.nCvsvType == T_SIBD_CVSV_D::kSurrenderSurvey)
				CSeismicDesignSibd_H14_Surrender::Calculate(pProgress, kSibd);
			else if (dSibd.CVSV_D.nCvsvType == T_SIBD_CVSV_D::kLimitStateSurvey)
				CSeismicDesignSibd_H14_LimitState::Calculate(pProgress, kSibd);
			break;
		default:
			break;
		}

		if (dSigr.nSubType == T_SIGR_D::kLower)
			CSeismicDesignSibd_H14_RebarReducedPart::Calculate(pProgress, kSibd);
	}

	return TRUE;
}

void CSeismicDesignSibd_H14::SetMemberVariable(const T_SIBD_K& kSibd)
{
	if (!m_pDoc) { ASSERT(m_pDoc); return; }

	T_SIBD_D dSibd; T_SIGR_D dSigr;
	if (m_pDoc->m_pAttrCtrl2->GetQSibd()->Get(kSibd, dSibd) == FALSE) return;
	if (m_pDoc->m_pAttrCtrl2->GetQSigr()->Get(dSibd.kSigr, dSigr) == FALSE) return;

	m_kSibd = kSibd;
	m_kSipa = dSibd.kSipa;
	m_kSigr = dSibd.kSigr;

	int nMaterialType = T_SIGR_LOWER_D::kUnKnown;
	if (dSigr.nSubType == T_SIGR_D::eSubType::kLower) {
		T_SIGR_LOWER_D dLower;
		if (m_pDoc->m_pAttrCtrl2->GetQSigrLower()->Get(dSibd.kSigr, dLower) == TRUE)
			m_nMaterialType = dLower.nMaterialType;
	}
}

double CSeismicDesignSibd_H14::MeterToMicron(const double& val)
{
	return val * 1000000; 
}

int CSeismicDesignSibd_H14::GetComponent(const int& nDir)
{
	int nComponent = QIehp::kUnknown;
	switch (nDir)
	{
	case seis_jp::kY_Dir:
		nComponent = QIehp::kMz;
		break;
	case seis_jp::kZ_Dir:
		nComponent = QIehp::kMy;
		break;
	}
	return nComponent;
}

BOOL CSeismicDesignSibd_H14::GetInelastHngeResultDist(const int& nExamShearDir, const int& nMinMax, IN T_ELEM_K_LIST& elemKeys, OUT double& dR_yz)
{
	CArray<T_4TABLE_ELEM_D, T_4TABLE_ELEM_D&> aData;
	if (m_pDoc->m_pPostCtrl->GetThisInfo()->GetInelastHngeResultDist(nMinMax, elemKeys, aData) == FALSE) return FALSE;

	int nSize = aData.GetCount() / 2;
	int nRyIdx = 0;
	int nRzIdx = nRyIdx + nSize;

	const int nIdxRot = [&]()
	{
		if (nExamShearDir == seis_jp::kY_Dir) { return nRzIdx; }
		else if (nExamShearDir == seis_jp::kZ_Dir) { return nRyIdx; }
		else { ASSERT(FALSE); }

		return 0;
	}();

	for (int i = 0; i < nSize; i++) {
		T_4TABLE_ELEM_D data = aData[nIdxRot + i];

		int nDof = data.nDof;
		double dR = data.Data[nDof].dDisplacement;
		if (m_nMaterialType == T_SIGR_LOWER_D::kSteel)
			dR = abs(dR);

		if (nMinMax == -1) {
			if (dR_yz > dR)
				dR_yz = dR;
		}
		else if (nMinMax == 1) {
			if (dR_yz < dR)
				dR_yz = dR;
		}
	}

	return TRUE;
}

#pragma endregion <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

#pragma region >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> CSeismicDesignSibd_H14_Strength
CSeismicDesignSibd_H14_Strength::CSeismicDesignSibd_H14_Strength(CDgnProgressDlg* pProgress, const T_SIBD_K& kSibd)
	: CSeismicDesignSibd_H14()
{
	m_kSibd = kSibd;
	m_pProgress = pProgress;
	SetMemberVariable(kSibd);
}

CSeismicDesignSibd_H14_Strength::~CSeismicDesignSibd_H14_Strength()
{
}

void CSeismicDesignSibd_H14_Strength::AddProgressStep()
{
	T_SIGR_D dSigr;
	if (m_pDoc->m_pAttrCtrl2->GetQSigr()->Get(m_kSigr, dSigr) == FALSE) return;
	if (dSigr.nSubType != T_SIGR_D::kUpper && dSigr.nSubType != T_SIGR_D::kUser) return;

	T_ELEM_K_LIST kElemList;
	MakeMemberMap(m_kSigr, kElemList);

	for (int i = 0; i < kElemList.GetCount(); i++)
	{
		const T_ELEM_K& kElem = kElemList[i];
		if (m_mMemberMap.find(kElem) == m_mMemberMap.end()) { ASSERT(FALSE); continue; }

		m_pProgress->m_nTotalSteps++;
	}
}

void CSeismicDesignSibd_H14_Strength::Calculate(CDgnProgressDlg* pProgress, const T_SIBD_K& kSibd)
{
	CSeismicDesignSibd_H14_Strength(pProgress, kSibd).Invoke();
}

void CSeismicDesignSibd_H14_Strength::Invoke()
{
	T_SIGR_D dSigr;
	if (m_pDoc->m_pAttrCtrl2->GetQSigr()->Get(m_kSigr, dSigr) == FALSE) return;
	//상부구조 그룹, 사용자 그룹인 경우
	if (dSigr.nSubType != T_SIGR_D::kUpper && dSigr.nSubType != T_SIGR_D::kUser) return;

	// Code 단위계 변환
	CUnitConversion unit(D_UNITSYS_FORCE_INDEX_KN, D_UNITSYS_LENGTH_INDEX_M);

	T_ELEM_K_LIST kElemList;
	MakeMemberMap(m_kSigr, kElemList);

	for (int i = 0; i < kElemList.GetCount(); i++)
	{
		const T_ELEM_K& kElem = kElemList[i];
		if (m_mMemberMap.find(kElem) == m_mMemberMap.end()) { ASSERT(FALSE); continue; }

		if (m_mMemberMap[kElem].nDirShear == seis_jp::kYZ_Dir) {
			RunEvaluation(kElem, seis_jp::kY_Dir);
			RunEvaluation(kElem, seis_jp::kZ_Dir);
		}
		else {
			T_SIPA_D tSipa;
			if (m_pDoc->m_pAttrCtrl2->GetQSipa()->Get(m_kSipa, tSipa) == FALSE) { ASSERT(FALSE); continue; }

			seis_jp::eDirShear nDirShear = m_mMemberMap[kElem].nDirShear;
			if (tSipa.nSeismicForceDirection == T_SIPA_D::kPerp_Dir) {
				nDirShear = (nDirShear == seis_jp::kY_Dir) ? seis_jp::kZ_Dir : seis_jp::kY_Dir;
			}

			RunEvaluation(kElem, nDirShear);
		}

		if (m_pProgress->NextStep() == FALSE) {
			throw FALSE;
		}
	}
}

void CSeismicDesignSibd_H14_Strength::RunEvaluation(const T_ELEM_K& kElem, const int& nExamShearDir)
{
	CBrdgSeisEvalResult_JP* pResult = m_pDoc->m_pPostCtrl->GetBrdgSeisEvalResult_JP(); if (!pResult) { ASSERT(0); return; }

	T_SIBD_D dSibd;
	if (m_pDoc->m_pAttrCtrl2->GetQSibd()->Get(m_kSibd, dSibd) == FALSE) return;

	T_ELEM_D elem;
	if (m_pDoc->m_pAttrCtrl->GetElem(kElem, elem) == FALSE) return;

	T_SIBD_EVAL_MOMENT eval;
	eval.kSibd = m_kSibd;
	eval.kSipa = m_kSipa;
	eval.kSigr = m_kSigr;
	eval.kElem = kElem;
	eval.nExamShearDir = nExamShearDir;
	if (m_mMemberMap.find(eval.kElem) != m_mMemberMap.end())
		eval.nMemberType = m_mMemberMap[eval.kElem].nType;

	eval.result_i.kNode = elem.elnod[0];
	eval.result_j.kNode = elem.elnod[1];

	//허용모멘트 계산
	GetMomentFromMPhi(&eval);

	//해석에 의한 응답 모멘트
	GetMomentFromAnalysis(&eval);

	double dMomentMax = 0, dMomentMin = 0;
	if (dSibd.SRSV_D.nMomentType == T_SIBD_SRSV_D::kMy) {
		dMomentMax = eval.calcMomeMax.dY;
		dMomentMin = eval.calcMomeMin.dY;
	}
	else if (dSibd.SRSV_D.nMomentType == T_SIBD_SRSV_D::kMu) {
		dMomentMax = eval.calcMomeMax.dU;
		dMomentMin = eval.calcMomeMin.dU;
	}

	eval.result_i.dRatioMax = dgn::SafeDiv(eval.analMoment_i.dAvergeMax, dMomentMax);
	eval.result_i.bCheckResultMax = dgn::ME(eval.result_i.dRatioMax, 1.0) ? TRUE : FALSE;

	eval.result_i.dRatioMin = dgn::SafeDiv(eval.analMoment_i.dAvergeMin, dMomentMin);
	eval.result_i.bCheckResultMin = dgn::ME(eval.result_i.dRatioMin, 1.0) ? TRUE : FALSE;

	eval.result_j.dRatioMax = dgn::SafeDiv(eval.analMoment_j.dAvergeMax, dMomentMax);
	eval.result_j.bCheckResultMax = dgn::ME(eval.result_j.dRatioMax, 1.0) ? TRUE : FALSE;

	eval.result_j.dRatioMin = dgn::SafeDiv(eval.analMoment_j.dAvergeMin, dMomentMin);
	eval.result_j.bCheckResultMin = dgn::ME(eval.result_j.dRatioMin, 1.0) ? TRUE : FALSE;

	// save result
	if (pResult) pResult->SetResultSibdMoment(eval);
}

void CSeismicDesignSibd_H14_Strength::GetMomentFromMPhi(T_SIBD_EVAL_MOMENT* pEval)
{
	T_SIBD_D dSibd; T_SIPA_D dSipa;
	if (m_pDoc->m_pAttrCtrl2->GetQSibd()->Get(m_kSibd, dSibd) == FALSE) return;
	if (m_pDoc->m_pAttrCtrl2->GetQSipa()->Get(dSibd.kSipa, dSipa) == FALSE) return;

	int nComponent = GetComponent(pEval->nExamShearDir);
	if (nComponent == QIehp::kUnknown) return;

	MPhiProp prop_i; MPhiProp prop_j;
	if (m_pDoc->m_pAttrCtrl2->GetQIehp()->GetHingeProp(pEval->kElem, nComponent, QIehp::ePosI, prop_i) == FALSE ||
		m_pDoc->m_pAttrCtrl2->GetQIehp()->GetHingeProp(pEval->kElem, nComponent, QIehp::ePosJ, prop_j) == FALSE) {
		CString str;
		str.Format(_LS(IDS_DGN_SEIS_EVAL_MSG_NOT_EXIST_HINGE_DATA), pEval->kElem);
		m_pDoc->DisplayHistoryMessage(str);
		return;
	}

	pEval->calcMomeMax.dC = prop_i.dStrength_C[MPhiProp::kPlus];
	pEval->calcMomeMax.dY = prop_i.dStrength_Y[MPhiProp::kPlus];
	pEval->calcMomeMax.dU = prop_i.dStrength_U[MPhiProp::kPlus];

	pEval->calcMomeMin.dC = -1 * prop_j.dStrength_C[MPhiProp::kMinus];
	pEval->calcMomeMin.dY = -1 * prop_j.dStrength_Y[MPhiProp::kMinus];
	pEval->calcMomeMin.dU = -1 * prop_j.dStrength_U[MPhiProp::kMinus];
}

void CSeismicDesignSibd_H14_Strength::GetMomentFromAnalysis(T_SIBD_EVAL_MOMENT* pEval)
{
	T_SIBD_D dSibd; T_SIPA_D dSipa;
	if (m_pDoc->m_pAttrCtrl2->GetQSibd()->Get(m_kSibd, dSibd) == FALSE) return;
	if (m_pDoc->m_pAttrCtrl2->GetQSipa()->Get(dSibd.kSipa, dSipa) == FALSE) return;

	auto nCompIdx = [&](const int& nPos)
	{
		if (pEval->nExamShearDir == seis_jp::kY_Dir)
			return (nPos == STRB_POSI_I) ? STRB_MOMENT_Z_I : STRB_MOMENT_Z_J;
		else if (pEval->nExamShearDir == seis_jp::kZ_Dir)
			return (nPos == STRB_POSI_I) ? STRB_MOMENT_Y_I : STRB_MOMENT_Y_J;

		return 0;
	};

	double dMomentMaxSum_i = 0, dMomentMinSum_i = 0, dMomentMaxSum_j = 0, dMomentMinSum_j = 0;

	for (int i = 0; i < dSipa.aSeisThisK.GetCount(); i++)
	{
		T_THIS_K kThis = dSipa.aSeisThisK[i];

		T_LCOM_K LcomKey = kThis;
		T_LCOM_D LcomData;
		m_pDoc->m_pPostCtrl->MakeLoadComb(D_LOADCASE_HISTORY, LcomKey, LcomData);
		m_pDoc->m_pPostCtrl->SelectLoadCombNew(LcomData);

		T_STRB_D max, min;
		m_pDoc->m_pPostCtrl->GetStrbNew(pEval->kElem, &max, &min, 0, FALSE);

		double dMomentMax_I = max.dblForce[STRB_POSI_I][nCompIdx(STRB_POSI_I)];
		dMomentMaxSum_i += dMomentMax_I;
		pEval->analMoment_i.aSmax.Add(dMomentMax_I);

		double dMomentMin_I = min.dblForce[STRB_POSI_I][nCompIdx(STRB_POSI_I)];
		dMomentMinSum_i += dMomentMin_I;
		pEval->analMoment_i.aSmin.Add(dMomentMin_I);

		double dMomentMax_J = max.dblForce[STRB_POSI_J][nCompIdx(STRB_POSI_J)];
		dMomentMaxSum_j += dMomentMax_J;
		pEval->analMoment_j.aSmax.Add(dMomentMax_J);

		double dMomentMin_J = min.dblForce[STRB_POSI_J][nCompIdx(STRB_POSI_J)];
		dMomentMinSum_j += dMomentMin_J;
		pEval->analMoment_j.aSmin.Add(dMomentMin_J);
	}

	double dCount = dSipa.aSeisThisK.GetCount();

	pEval->analMoment_i.dAvergeMax = dgn::SafeDiv(dMomentMaxSum_i, dCount);
	pEval->analMoment_i.dAvergeMin = dgn::SafeDiv(dMomentMinSum_i, dCount);
	pEval->analMoment_j.dAvergeMax = dgn::SafeDiv(dMomentMaxSum_j, dCount);
	pEval->analMoment_j.dAvergeMin = dgn::SafeDiv(dMomentMinSum_j, dCount);
}
#pragma endregion <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

#pragma region >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> CSeismicDesignSibd_H14_Surrender
CSeismicDesignSibd_H14_Surrender::CSeismicDesignSibd_H14_Surrender(CDgnProgressDlg* pProgress, const T_SIBD_K& kSibd)
	: CSeismicDesignSibd_H14()
{
	m_pProgress = pProgress;
	SetMemberVariable(kSibd);
}

CSeismicDesignSibd_H14_Surrender::~CSeismicDesignSibd_H14_Surrender()
{
}

void CSeismicDesignSibd_H14_Surrender::AddProgressStep()
{
	T_SIBD_D dSibd; T_SIGR_D dSigr;
	if (m_pDoc->m_pAttrCtrl2->GetQSibd()->Get(m_kSibd, dSibd) == FALSE) return;
	if (m_pDoc->m_pAttrCtrl2->GetQSigr()->Get(dSibd.kSigr, dSigr) == FALSE) return;

	T_ELEM_K_LIST kElemList;
	MakeMemberMap(dSibd.kSigr, kElemList);

	for (int i = 0; i < kElemList.GetCount(); i++)
	{
		const T_ELEM_K& kElem = kElemList[i];
		if (m_mMemberMap.find(kElem) == m_mMemberMap.end()) { ASSERT(FALSE); continue; }

		m_pProgress->m_nTotalSteps++;
	}
}

void CSeismicDesignSibd_H14_Surrender::Calculate(CDgnProgressDlg* pProgress, const T_SIBD_K& kSibd)
{
	CSeismicDesignSibd_H14_Surrender(pProgress, kSibd).Invoke();
}

void CSeismicDesignSibd_H14_Surrender::Invoke()
{
	T_SIBD_D dSibd; T_SIGR_D dSigr;
	if (m_pDoc->m_pAttrCtrl2->GetQSibd()->Get(m_kSibd, dSibd) == FALSE) return;
	if (m_pDoc->m_pAttrCtrl2->GetQSigr()->Get(dSibd.kSigr, dSigr) == FALSE) return;

	// Code 단위계 변환
	CUnitConversion unit(D_UNITSYS_FORCE_INDEX_N, D_UNITSYS_LENGTH_INDEX_M);

	T_ELEM_K_LIST kElemList;
	MakeMemberMap(dSibd.kSigr, kElemList);

	for (int i = 0; i < kElemList.GetCount(); i++)
	{
		const T_ELEM_K& kElem = kElemList[i];
		if (m_mMemberMap.find(kElem) == m_mMemberMap.end()) { ASSERT(FALSE); continue; }

		if (m_mMemberMap[kElem].nDirShear == seis_jp::kYZ_Dir) {
			RunEvaluation(kElem, seis_jp::kY_Dir);
			RunEvaluation(kElem, seis_jp::kZ_Dir);
		}
		else {
			T_SIPA_D tSipa;
			if (m_pDoc->m_pAttrCtrl2->GetQSipa()->Get(m_kSipa, tSipa) == FALSE) { ASSERT(FALSE); continue; }

			seis_jp::eDirShear nDirShear = m_mMemberMap[kElem].nDirShear;
			if (tSipa.nSeismicForceDirection == T_SIPA_D::kPerp_Dir) {
				nDirShear = (nDirShear == seis_jp::kY_Dir) ? seis_jp::kZ_Dir : seis_jp::kY_Dir;
			}

			RunEvaluation(kElem, nDirShear);
		}

		if (m_pProgress->NextStep() == FALSE) {
			throw FALSE;
		}
	}
}

void CSeismicDesignSibd_H14_Surrender::RunEvaluation(const T_ELEM_K& kElem, const int& nExamShearDir)
{
	CBrdgSeisEvalResult_JP* pResult = m_pDoc->m_pPostCtrl->GetBrdgSeisEvalResult_JP(); if (!pResult) { ASSERT(0); return; }

	T_SIBD_EVAL_PHI_Y eval;
	eval.kSibd = m_kSibd;
	eval.kSipa = m_kSipa;
	eval.kSigr = m_kSigr;
	eval.kElem = kElem;
	eval.nMaterialType = m_nMaterialType;
	eval.nExamShearDir = nExamShearDir;
	if (m_mMemberMap.find(eval.kElem) != m_mMemberMap.end())
		eval.nMemberType = m_mMemberMap[eval.kElem].nType;

	//허용곡률 계산
	if (GetDeformFromMPhi(&eval) == FALSE) return;

	//해석에 의한 응답 변형률
	GetDeformFromAnalysis(&eval);

	if (isnan(eval.analDeform.dAvergeMax) == 0 && isnan(eval.calcDeformMax.dPhi_y) == 0)
		eval.resultRc.dRatioMax = dgn::SafeDiv(abs(eval.analDeform.dAvergeMax), abs(eval.calcDeformMax.dPhi_y));
	if (isnan(eval.analDeform.dAvergeMin) == 0 && isnan(eval.calcDeformMin.dPhi_y) == 0)
		eval.resultRc.dRatioMin = dgn::SafeDiv(abs(eval.analDeform.dAvergeMin), abs(eval.calcDeformMin.dPhi_y));
	// 끝까지 오면 정상적으로 계산된거임.
	eval.resultRc.bCheckResultMax = TRUE;
	eval.resultRc.bCheckResultMin = TRUE;

	if (eval.nMaterialType == T_SIGR_LOWER_D::kSteel)
	{
		T_SIBD_EVAL_PHI_Y_ST evalST;
		GetEvalPhiY_ST(&eval, &evalST);


		if (pResult) pResult->SetResultSibdPhiY_ST(evalST);
	}
	else {
		// save result
		CBrdgSeisEvalResult_JP* pResult = m_pDoc->m_pPostCtrl->GetBrdgSeisEvalResult_JP(); if (!pResult) { ASSERT(0); }
		if (pResult) pResult->SetResultSibdPhiY(eval);
	}
}

BOOL CSeismicDesignSibd_H14_Surrender::GetDeformFromMPhi(T_SIBD_EVAL_PHI_Y* pEval)
{
	T_SIBD_D dSibd; T_SIPA_D dSipa;
	if (m_pDoc->m_pAttrCtrl2->GetQSibd()->Get(m_kSibd, dSibd) == FALSE) return FALSE;
	if (m_pDoc->m_pAttrCtrl2->GetQSipa()->Get(dSibd.kSipa, dSipa) == FALSE) return FALSE;

	int nComponent = GetComponent(pEval->nExamShearDir);
	if (nComponent == QIehp::kUnknown) return FALSE;

	MPhiProp prop_i; MPhiProp prop_j;
	if (m_pDoc->m_pAttrCtrl2->GetQIehp()->GetHingeProp(pEval->kElem, nComponent, QIehp::ePosI, prop_i) == FALSE ||
		m_pDoc->m_pAttrCtrl2->GetQIehp()->GetHingeProp(pEval->kElem, nComponent, QIehp::ePosJ, prop_j) == FALSE) {
		CString str;
		str.Format(_LS(IDS_DGN_SEIS_EVAL_MSG_NOT_EXIST_HINGE_DATA), pEval->kElem);
		m_pDoc->DisplayHistoryMessage(str);
		return FALSE;
	}

	pEval->calcDeformMax.dPhi_c = MeterToMicron(prop_i.dDisplace_C[MPhiProp::kPlus]);
	pEval->calcDeformMax.dPhi_y = MeterToMicron(prop_i.dDisplace_Y[MPhiProp::kPlus]);
	pEval->calcDeformMax.dPhi_u = MeterToMicron(prop_i.dDisplace_U[MPhiProp::kPlus]);

	pEval->calcDeformMin.dPhi_c = -1 * MeterToMicron(prop_i.dDisplace_C[MPhiProp::kMinus]);
	pEval->calcDeformMin.dPhi_y = -1 * MeterToMicron(prop_j.dDisplace_Y[MPhiProp::kMinus]);
	pEval->calcDeformMin.dPhi_u = -1 * MeterToMicron(prop_j.dDisplace_U[MPhiProp::kMinus]);

	return TRUE;
}

void CSeismicDesignSibd_H14_Surrender::GetDeformFromAnalysis(OUT T_SIBD_EVAL_PHI_Y* pEval)
{
	T_SIBD_D dSibd; T_SIPA_D dSipa;
	if (m_pDoc->m_pAttrCtrl2->GetQSibd()->Get(m_kSibd, dSibd) == FALSE) return;
	if (m_pDoc->m_pAttrCtrl2->GetQSipa()->Get(dSibd.kSipa, dSipa) == FALSE) return;

	double dDeformMaxSum = 0, dDeformMinSum = 0;

	for (int i = 0; i < dSipa.aSeisThisK.GetCount(); i++)
	{
		T_THIS_K kThis = dSipa.aSeisThisK[i];
		m_pDoc->m_pPostCtrl->SelectThis(kThis);

		T_ELEM_K_LIST elemKeys;
		elemKeys.Add(pEval->kElem);

		double dR_min = DBL_MAX, dR_max = -DBL_MAX;
		if (GetInelastHngeResultDist(pEval->nExamShearDir, -1, elemKeys, dR_min) == FALSE) continue;
		if (GetInelastHngeResultDist(pEval->nExamShearDir, 1, elemKeys, dR_max) == FALSE) continue;

		dR_min = MeterToMicron(dR_min);
		dR_max = MeterToMicron(dR_max);

		dDeformMaxSum += dR_max;
		pEval->analDeform.aSmax.Add(dR_max);

		dDeformMinSum += dR_min;
		pEval->analDeform.aSmin.Add(dR_min);
	}

	double dCount = dSipa.aSeisThisK.GetCount();

	pEval->analDeform.dAvergeMax = dgn::SafeDiv(dDeformMaxSum, dCount);
	pEval->analDeform.dAvergeMin = dgn::SafeDiv(dDeformMinSum, dCount);
}

void CSeismicDesignSibd_H14_Surrender::GetEvalPhiY_ST(IN T_SIBD_EVAL_PHI_Y* pEval, OUT T_SIBD_EVAL_PHI_Y_ST* pEvalST)
{
	pEvalST->kSibd = pEval->kSibd;
	pEvalST->kSipa = pEval->kSipa;
	pEvalST->kSigr = pEval->kSigr;
	pEvalST->kElem = pEval->kElem;
	pEvalST->nMemberType = pEval->nMemberType;
	pEvalST->nMaterialType = pEval->nMaterialType;
	pEvalST->nExamShearDir = pEval->nExamShearDir;

	double dSum = 0;
	ASSERT(pEval->analDeform.aSmax.GetSize() == pEval->analDeform.aSmin.GetSize());
	int nThis = min(pEval->analDeform.aSmax.GetSize(), 3);
	for (int i = 0; i < nThis; ++i) {
		const double& dMax = pEval->analDeform.aSmax[i];
		const double& dMin = pEval->analDeform.aSmin[i];

		double dVal = 0;
		if (abs(dMax) > abs(dMin))	dVal = dMax;
		else						dVal = dMin;

		pEvalST->aAnalDeform.Add(dVal);
		dSum += dVal;
	}

	pEvalST->dAverge = dgn::SafeDiv(dSum, (double)nThis);

	auto L_GetMPhiValue = [&](const double& dMax, const double& dMin, OUT double& rVal)
		{
			if (isnan(dMax) == 0 && isnan(dMin) == 0) {
				rVal = (abs(dMax) < abs(dMin)) ? abs(dMax) : abs(dMin);
			}
			else if (isnan(dMax) != 0 && isnan(dMin) == 0) {
				rVal = abs(dMin);
			}
			else if (isnan(dMax) == 0 && isnan(dMin) != 0) {
				rVal = abs(dMax);
			}
			else {
				rVal = std::nan("--");
			}
		};

	L_GetMPhiValue(pEval->calcDeformMax.dPhi_c, pEval->calcDeformMin.dPhi_c, pEvalST->calcDeform.dPhi_c);
	L_GetMPhiValue(pEval->calcDeformMax.dPhi_y, pEval->calcDeformMin.dPhi_y, pEvalST->calcDeform.dPhi_y);
	L_GetMPhiValue(pEval->calcDeformMax.dPhi_u, pEval->calcDeformMin.dPhi_u, pEvalST->calcDeform.dPhi_u);

	if (isnan(pEvalST->dAverge) == 0 && isnan(pEvalST->calcDeform.dPhi_c) == 0)
		pEvalST->resultSt.dRatioYc = dgn::SafeDivAbs(pEvalST->dAverge, pEvalST->calcDeform.dPhi_c);
	else
		pEvalST->resultSt.dRatioYc = std::nan("--");

	if (isnan(pEvalST->dAverge) == 0 && isnan(pEvalST->calcDeform.dPhi_y) == 0)
		pEvalST->resultSt.dRatioYt = dgn::SafeDivAbs(pEvalST->dAverge, pEvalST->calcDeform.dPhi_y);
	else
		pEvalST->resultSt.dRatioYt = std::nan("--");

	pEvalST->resultSt.bCheckResultYc = TRUE;
	pEvalST->resultSt.bCheckResultYt = TRUE;
}
#pragma endregion <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

#pragma region >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> CSeismicDesignSibd_H14_LimitState
CSeismicDesignSibd_H14_LimitState::CSeismicDesignSibd_H14_LimitState(CDgnProgressDlg* pProgress, const T_SIBD_K& kSibd)
	: CSeismicDesignSibd_H14()
{
	m_pProgress = pProgress;
	SetMemberVariable(kSibd);
}

CSeismicDesignSibd_H14_LimitState::~CSeismicDesignSibd_H14_LimitState()
{
}

void CSeismicDesignSibd_H14_LimitState::AddProgressStep()
{
	T_ELEM_K_LIST kElemList;
	MakeMemberMap(m_kSigr, kElemList);

	for (int i = 0; i < kElemList.GetCount(); i++)
	{
		const T_ELEM_K& kElem = kElemList[i];
		if (m_mMemberMap.find(kElem) == m_mMemberMap.end()) { ASSERT(FALSE); continue; }

		m_pProgress->m_nTotalSteps++;
	}
}

void CSeismicDesignSibd_H14_LimitState::Calculate(CDgnProgressDlg* pProgress, const T_SIBD_K& kSibd)
{
	CSeismicDesignSibd_H14_LimitState(pProgress, kSibd).Invoke();
}

void CSeismicDesignSibd_H14_LimitState::Invoke()
{
	// Code 단위계 변환
	CUnitConversion unit(D_UNITSYS_FORCE_INDEX_N, D_UNITSYS_LENGTH_INDEX_M);

	T_ELEM_K_LIST kElemList;
	MakeMemberMap(m_kSigr, kElemList);

	for (int i = 0; i < kElemList.GetCount(); i++)
	{
		const T_ELEM_K& kElem = kElemList[i];
		if (m_mMemberMap.find(kElem) == m_mMemberMap.end()) { ASSERT(FALSE); continue; }

		if (m_mMemberMap[kElem].nDirShear == seis_jp::kYZ_Dir) {
			RunEvaluation(kElem, seis_jp::kY_Dir);
			RunEvaluation(kElem, seis_jp::kZ_Dir);
		}
		else {
			T_SIPA_D tSipa;
			if (m_pDoc->m_pAttrCtrl2->GetQSipa()->Get(m_kSipa, tSipa) == FALSE) { ASSERT(FALSE); continue; }

			seis_jp::eDirShear nDirShear = m_mMemberMap[kElem].nDirShear;
			if (tSipa.nSeismicForceDirection == T_SIPA_D::kPerp_Dir) {
				nDirShear = (nDirShear == seis_jp::kY_Dir) ? seis_jp::kZ_Dir : seis_jp::kY_Dir;
			}

			RunEvaluation(kElem, nDirShear);
		}

		if (m_pProgress->NextStep() == FALSE) {
			throw FALSE;
		}
	}
}

void CSeismicDesignSibd_H14_LimitState::RunEvaluation(const T_ELEM_K& kElem, const int& nExamShearDir)
{
	CBrdgSeisEvalResult_JP* pResult = m_pDoc->m_pPostCtrl->GetBrdgSeisEvalResult_JP(); if (!pResult) { ASSERT(0); return; }

	T_SIBD_EVAL_PHI_A eval;
	eval.kSibd = m_kSibd;
	eval.kSipa = m_kSipa;
	eval.kSigr = m_kSigr;
	eval.kElem = kElem;
	eval.nMaterialType = m_nMaterialType;
	eval.nExamShearDir = nExamShearDir;
	if (m_mMemberMap.find(eval.kElem) != m_mMemberMap.end())
		eval.nMemberType = m_mMemberMap[eval.kElem].nType;

	//허용곡률 계산
	if (GetDeformFromMPhi(&eval) == FALSE) return;

	//해석에 의한 응답 변형률
	GetDeformFromAnalysis(&eval);

	if (isnan(eval.analDeform.dAvergeMax) == 0 && isnan(eval.calcDeformMax.dPhi_a) == 0)
		eval.resultRc.dRatioMax = dgn::SafeDiv(abs(eval.analDeform.dAvergeMax), abs(eval.calcDeformMax.dPhi_a));
	if (isnan(eval.analDeform.dAvergeMin) == 0 && isnan(eval.calcDeformMin.dPhi_a) == 0)
		eval.resultRc.dRatioMin = dgn::SafeDiv(abs(eval.analDeform.dAvergeMin), abs(eval.calcDeformMin.dPhi_a));
	// 끝까지 오면 정상적으로 계산된거임.
	eval.resultRc.bCheckResultMax = TRUE;
	eval.resultRc.bCheckResultMin = TRUE;

	if (eval.nMaterialType == T_SIGR_LOWER_D::kSteel)
	{
		T_SIBD_EVAL_PHI_A_ST evalST;
		GetEvalPhiA_ST(&eval, &evalST);

		if (pResult) pResult->SetResultSibdPhiA_ST(evalST);
	}
	else {
		// save result
		if (pResult) pResult->SetResultSibdPhiA(eval);
	}
}

BOOL CSeismicDesignSibd_H14_LimitState::GetDeformFromMPhi(OUT T_SIBD_EVAL_PHI_A* pEval)
{
	T_SIBD_D dSibd; T_SIPA_D dSipa;
	if (m_pDoc->m_pAttrCtrl2->GetQSibd()->Get(m_kSibd, dSibd) == FALSE) return FALSE;
	if (m_pDoc->m_pAttrCtrl2->GetQSipa()->Get(dSibd.kSipa, dSipa) == FALSE) return FALSE;

	int nComponent = GetComponent(pEval->nExamShearDir);
	if (nComponent == QIehp::kUnknown) return FALSE;

	MPhiProp prop_i; MPhiProp prop_j;
	if (m_pDoc->m_pAttrCtrl2->GetQIehp()->GetHingeProp(pEval->kElem, nComponent, QIehp::ePosI, prop_i) == FALSE ||
		m_pDoc->m_pAttrCtrl2->GetQIehp()->GetHingeProp(pEval->kElem, nComponent, QIehp::ePosJ, prop_j) == FALSE) {
		CString str;
		str.Format(_LS(IDS_DGN_SEIS_EVAL_MSG_NOT_EXIST_HINGE_DATA), pEval->kElem);
		m_pDoc->DisplayHistoryMessage(str);
		return FALSE;
	}

	pEval->calcDeformMax.dPhi_c = MeterToMicron(prop_i.dDisplace_C[MPhiProp::kPlus]);
	pEval->calcDeformMax.dPhi_y = MeterToMicron(prop_i.dDisplace_Y[MPhiProp::kPlus]);
	pEval->calcDeformMax.dPhi_u = MeterToMicron(prop_i.dDisplace_U[MPhiProp::kPlus]);
	if (m_nMaterialType == T_SIGR_LOWER_D::kRC) {
		if (GetPhi_a(&dSibd, &pEval->calcDeformMax) == FALSE) return FALSE;
	}
	else if (m_nMaterialType == T_SIGR_LOWER_D::kSteel) {
		pEval->calcDeformMax.dPhi_a = pEval->calcDeformMax.dPhi_u;
	}

	pEval->calcDeformMin.dPhi_c = -1 * MeterToMicron(prop_j.dDisplace_C[MPhiProp::kMinus]);
	pEval->calcDeformMin.dPhi_y = -1 * MeterToMicron(prop_j.dDisplace_Y[MPhiProp::kMinus]);
	pEval->calcDeformMin.dPhi_u = -1 * MeterToMicron(prop_j.dDisplace_U[MPhiProp::kMinus]);
	if (m_nMaterialType == T_SIGR_LOWER_D::kRC) {
		if (GetPhi_a(&dSibd, &pEval->calcDeformMin) == FALSE) return FALSE;
	}
	else if (m_nMaterialType == T_SIGR_LOWER_D::kSteel) {
		pEval->calcDeformMin.dPhi_a = pEval->calcDeformMin.dPhi_u;
	}

	return TRUE;
}

BOOL CSeismicDesignSibd_H14_LimitState::GetPhi_a(T_SIBD_D* pSibd, OUT T_SIBD_EVAL_CALC_PHI_A* pDeform)
{
	//φa=φls/α
	if (pSibd->CVSV_D.nAllowCurvFormula == T_SIBD_CVSV_D::kPhiA_Phiyls_Ellipsis) {
		pDeform->dPhi_a = dgn::SafeDiv(pDeform->dPhi_u, pSibd->CVSV_D.dLimitStateAlpha);
	}
	//φa=φy0+(φls-φy0)/α
	else if (pSibd->CVSV_D.nAllowCurvFormula == T_SIBD_CVSV_D::kPhiA_Phiy0_Ellipsis) {
		double dVal = pDeform->dPhi_u - pDeform->dPhi_y;
		dVal = dgn::SafeDiv(dVal, pSibd->CVSV_D.dLimitStateAlpha);
		pDeform->dPhi_a = pDeform->dPhi_y + dVal;
	}
	else {
		return FALSE;
	}
	return TRUE;
}

void CSeismicDesignSibd_H14_LimitState::GetDeformFromAnalysis(OUT T_SIBD_EVAL_PHI_A* pEval)
{
	T_SIBD_D dSibd; T_SIPA_D dSipa;
	if (m_pDoc->m_pAttrCtrl2->GetQSibd()->Get(m_kSibd, dSibd) == FALSE) return;
	if (m_pDoc->m_pAttrCtrl2->GetQSipa()->Get(dSibd.kSipa, dSipa) == FALSE) return;

	double dDeformMaxSum = 0, dDeformMinSum = 0;

	int nCount = dSipa.aSeisThisK.GetCount();
	for (int i = 0; i < nCount; i++)
	{
		T_THIS_K kThis = dSipa.aSeisThisK[i];
		m_pDoc->m_pPostCtrl->SelectThis(kThis);

		T_ELEM_K_LIST elemKeys;
		elemKeys.Add(pEval->kElem);

		double dR_min = DBL_MAX, dR_max = -DBL_MAX;
		if (GetInelastHngeResultDist(pEval->nExamShearDir, -1, elemKeys, dR_min) == FALSE) continue;
		if (GetInelastHngeResultDist(pEval->nExamShearDir, 1, elemKeys, dR_max) == FALSE) continue;

		dR_min = MeterToMicron(dR_min);
		dR_max = MeterToMicron(dR_max);

		dDeformMaxSum += dR_max;
		pEval->analDeform.aSmax.Add(dR_max);

		dDeformMinSum += dR_min;
		pEval->analDeform.aSmin.Add(dR_min);
	}

	pEval->analDeform.dAvergeMax = dgn::SafeDiv(dDeformMaxSum, (double)nCount);
	pEval->analDeform.dAvergeMin = dgn::SafeDiv(dDeformMinSum, (double)nCount);
}

void CSeismicDesignSibd_H14_LimitState::GetEvalPhiA_ST(IN T_SIBD_EVAL_PHI_A* pEval, OUT T_SIBD_EVAL_PHI_A_ST* pEvalST)
{
	pEvalST->kSibd = pEval->kSibd;
	pEvalST->kSipa = pEval->kSipa;
	pEvalST->kSigr = pEval->kSigr;
	pEvalST->kElem = pEval->kElem;
	pEvalST->nMemberType = pEval->nMemberType;
	pEvalST->nMaterialType = pEval->nMaterialType;
	pEvalST->nExamShearDir = pEval->nExamShearDir;

	double dSum = 0;
	ASSERT(pEval->analDeform.aSmax.GetSize() == pEval->analDeform.aSmin.GetSize());
	int nThis = min(pEval->analDeform.aSmax.GetSize(), 3);
	for (int i = 0; i < nThis; ++i) {
		const double& dMax = pEval->analDeform.aSmax[i];
		const double& dMin = pEval->analDeform.aSmin[i];

		double dVal = 0;
		if (abs(dMax) > abs(dMin))	dVal = dMax;
		else						dVal = dMin;

		pEvalST->aAnalDeform.Add(dVal);
		dSum += dVal;
	}

	pEvalST->dAverge = dgn::SafeDiv(dSum, (double)nThis);

	auto L_GetMPhiValue = [&](const double& dMax, const double& dMin, OUT double& rVal)
		{
			if (isnan(dMax) == 0 && isnan(dMin) == 0) {
				rVal = (abs(dMax) < abs(dMin)) ? abs(dMax) : abs(dMin);
			}
			else if (isnan(dMax) != 0 && isnan(dMin) == 0) {
				rVal = abs(dMin);
			}
			else if (isnan(dMax) == 0 && isnan(dMin) != 0) {
				rVal = abs(dMax);
			}
			else {
				rVal = std::nan("--");
			}
		};

	L_GetMPhiValue(pEval->calcDeformMax.dPhi_c, pEval->calcDeformMin.dPhi_c, pEvalST->calcDeform.dPhi_c);
	L_GetMPhiValue(pEval->calcDeformMax.dPhi_y, pEval->calcDeformMin.dPhi_y, pEvalST->calcDeform.dPhi_y);
	L_GetMPhiValue(pEval->calcDeformMax.dPhi_u, pEval->calcDeformMin.dPhi_u, pEvalST->calcDeform.dPhi_u);
	L_GetMPhiValue(pEval->calcDeformMax.dPhi_a, pEval->calcDeformMin.dPhi_a, pEvalST->calcDeform.dPhi_a);

	if (isnan(pEvalST->dAverge) == 0 && isnan(pEvalST->calcDeform.dPhi_c) == 0)
		pEvalST->resultSt.dRatioYc = dgn::SafeDivAbs(pEvalST->dAverge, pEvalST->calcDeform.dPhi_c);
	else
		pEvalST->resultSt.dRatioYc = std::nan("--");

	if (isnan(pEvalST->dAverge) == 0 && isnan(pEvalST->calcDeform.dPhi_a) == 0)
		pEvalST->resultSt.dRatioYt = dgn::SafeDivAbs(pEvalST->dAverge, pEvalST->calcDeform.dPhi_a);
	else
		pEvalST->resultSt.dRatioYt = std::nan("--");

	pEvalST->resultSt.bCheckResultYc = TRUE;
	pEvalST->resultSt.bCheckResultYt = TRUE;
}
#pragma endregion <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

#pragma region >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> CSeismicDesignSibd_H14_RebarReducedPart 

CSeismicDesignSibd_H14_RebarReducedPart::CSeismicDesignSibd_H14_RebarReducedPart(CDgnProgressDlg* pProgress, const T_SIBD_K& kSibd)
	: CSeismicDesignSibd_H14()
{
	m_pProgress = pProgress;
	SetMemberVariable(kSibd);
}

CSeismicDesignSibd_H14_RebarReducedPart::~CSeismicDesignSibd_H14_RebarReducedPart()
{
}

void CSeismicDesignSibd_H14_RebarReducedPart::AddProgressStep()
{
	T_SIPA_D dSipa; T_SIGR_D dSigr; T_SIGR_LOWER_D dLower;
	if (m_pDoc->m_pAttrCtrl2->GetQSipa()->Get(m_kSipa, dSipa) == FALSE) return;
	if (m_pDoc->m_pAttrCtrl2->GetQSigr()->Get(m_kSigr, dSigr) == FALSE) return;
	if (dSigr.nSubType != T_SIGR_D::kLower) return;
	if (m_pDoc->m_pAttrCtrl2->GetQSigrLower()->Get(m_kSigr, dLower) == FALSE) return;

	T_ELEM_K_LIST kElemList;
	MakeMemberMap(m_kSigr, kElemList);

	for (int i = 0; i < dLower.COLM_D.GetCount(); i++)
	{
		const T_SIGR_LOWER_COLM_D& colm = dLower.COLM_D[i];
		if (colm.bRebarCut == FALSE) continue;

		T_ELEM_K kElem = 0;
		if (dSipa.nSeismicForceDirection == T_SIPA_D::kAxis_Dir) {
			kElem = colm.kAxialElem;
		}
		else if (dSipa.nSeismicForceDirection == T_SIPA_D::kPerp_Dir) {
			kElem = colm.kAxisPerpElem;
		}
		else {
			ASSERT(0);
			continue;
		}

		if (m_mMemberMap.find(kElem) == m_mMemberMap.end()) { ASSERT(FALSE); continue; }

		m_pProgress->m_nTotalSteps++;
	}
}

void CSeismicDesignSibd_H14_RebarReducedPart::Calculate(CDgnProgressDlg* pProgress, const T_SIBD_K& kSibd)
{
	CSeismicDesignSibd_H14_RebarReducedPart(pProgress, kSibd).Invoke();
}

void CSeismicDesignSibd_H14_RebarReducedPart::Invoke()
{
	T_SIPA_D dSipa; T_SIGR_D dSigr; T_SIGR_LOWER_D dLower;
	if (m_pDoc->m_pAttrCtrl2->GetQSipa()->Get(m_kSipa, dSipa) == FALSE) return;
	if (m_pDoc->m_pAttrCtrl2->GetQSigr()->Get(m_kSigr, dSigr) == FALSE) return;
	if (dSigr.nSubType != T_SIGR_D::kLower) return;
	if (m_pDoc->m_pAttrCtrl2->GetQSigrLower()->Get(m_kSigr, dLower) == FALSE) return;

	// Code 단위계 변환
	CUnitConversion unit(D_UNITSYS_FORCE_INDEX_N, D_UNITSYS_LENGTH_INDEX_M);

	T_ELEM_K_LIST kElemList;
	MakeMemberMap(m_kSigr, kElemList); //m_mMemberMap 채워야 함.

	for (int i = 0; i < dLower.COLM_D.GetCount(); i++)
	{
		const T_SIGR_LOWER_COLM_D& colm = dLower.COLM_D[i];
		if (colm.bRebarCut == FALSE) continue;
		
		int nPos = 0;
		T_ELEM_K kElem = 0;
		if (dSipa.nSeismicForceDirection == T_SIPA_D::kAxis_Dir) {
			nPos = colm.nAxialLoc;
			kElem = colm.kAxialElem;
		}
		else if (dSipa.nSeismicForceDirection == T_SIPA_D::kPerp_Dir) {
			nPos = colm.nAxisPerpLoc;
			kElem = colm.kAxisPerpElem;
		}
		else {
			ASSERT(0);
			continue;
		}

		if (m_mMemberMap.find(kElem) == m_mMemberMap.end()) { ASSERT(FALSE); continue; }

		if (m_mMemberMap[kElem].nDirShear == seis_jp::kYZ_Dir) {
			RunEvaluation(colm, kElem, nPos, seis_jp::kY_Dir);
			RunEvaluation(colm, kElem, nPos, seis_jp::kZ_Dir);
		}
		else {
			T_SIPA_D tSipa;
			if (m_pDoc->m_pAttrCtrl2->GetQSipa()->Get(m_kSipa, tSipa) == FALSE) { ASSERT(FALSE); continue; }

			seis_jp::eDirShear nDirShear = m_mMemberMap[kElem].nDirShear;
			if (tSipa.nSeismicForceDirection == T_SIPA_D::kPerp_Dir) {
				nDirShear = (nDirShear == seis_jp::kY_Dir) ? seis_jp::kZ_Dir : seis_jp::kY_Dir;
			}

			RunEvaluation(colm, kElem, nPos, nDirShear);
		}

		if (m_pProgress->NextStep() == FALSE) {
			throw FALSE;
		}
	}
}

void CSeismicDesignSibd_H14_RebarReducedPart::RunEvaluation(const T_SIGR_LOWER_COLM_D& colm, const T_ELEM_K& kElem, const int& nPos, const int& nExamShearDir)
{
	T_SIPA_D dSipa; T_SIGR_D dSigr;
	if (m_pDoc->m_pAttrCtrl2->GetQSipa()->Get(m_kSipa, dSipa) == FALSE) return;
	if (m_pDoc->m_pAttrCtrl2->GetQSigr()->Get(m_kSigr, dSigr) == FALSE) return;

	T_SIBD_EVAL_REBAR_CUT eval;
	eval.kSipa = m_kSipa;
	eval.kSigr = m_kSigr;
	eval.kElem = kElem;
	eval.nRebarCutPos = nPos;
	eval.strSigrName = dSigr.strName;
	eval.strColmName = colm.strName;
	eval.nSeismicForceDirection = dSipa.nSeismicForceDirection;
	eval.nSeismicForceType = dSipa.nSeismicForceType;
	eval.nExamShearDir = nExamShearDir;

	//허용모멘트 계산
	GetMomentFromMPhi(&eval);

	//해석에 의한 응답 모멘트
	GetMomentFromAnalysis(nExamShearDir, &eval);

	eval.resultRc.dRatioMax = dgn::SafeDiv(abs(eval.analStrength.dAvergeMax), eval.calcStrengthMax.dY);
	eval.resultRc.dRatioMin = dgn::SafeDiv(abs(eval.analStrength.dAvergeMin), eval.calcStrengthMin.dY);

	// 끝까지 오면 정상적으로 계산된거임.
	eval.resultRc.bCheckResultMax = TRUE;
	eval.resultRc.bCheckResultMin = TRUE;

	// save result
	CBrdgSeisEvalResult_JP* pResult = m_pDoc->m_pPostCtrl->GetBrdgSeisEvalResult_JP(); if (!pResult) { ASSERT(0); }
	if (pResult) pResult->SetResultSibdRebarCut(eval);
}

void CSeismicDesignSibd_H14_RebarReducedPart::GetMomentFromMPhi(OUT T_SIBD_EVAL_REBAR_CUT* pEval)
{
	T_SIBD_D dSibd; T_SIPA_D dSipa;
	if (m_pDoc->m_pAttrCtrl2->GetQSibd()->Get(m_kSibd, dSibd) == FALSE) return;
	if (m_pDoc->m_pAttrCtrl2->GetQSipa()->Get(dSibd.kSipa, dSipa) == FALSE) return;

	int nComponent = GetComponent(pEval->nExamShearDir);
	if (nComponent == QIehp::kUnknown) return;

	MPhiProp prop;
	m_pDoc->m_pAttrCtrl2->GetQIehp()->GetHingeProp(pEval->kElem, nComponent, pEval->nRebarCutPos, prop);

	pEval->calcStrengthMax.dC = prop.dStrength_C[MPhiProp::kPlus];
	pEval->calcStrengthMax.dY = prop.dStrength_Y[MPhiProp::kPlus];
	pEval->calcStrengthMax.dU = prop.dStrength_U[MPhiProp::kPlus];

	pEval->calcStrengthMin.dC = prop.dStrength_C[MPhiProp::kMinus];
	pEval->calcStrengthMin.dY = prop.dStrength_Y[MPhiProp::kMinus];
	pEval->calcStrengthMin.dU = prop.dStrength_U[MPhiProp::kMinus];
}

void CSeismicDesignSibd_H14_RebarReducedPart::GetMomentFromAnalysis(const int& nDir, OUT T_SIBD_EVAL_REBAR_CUT* pEval)
{
	T_SIBD_D dSibd; T_SIPA_D dSipa;
	if (m_pDoc->m_pAttrCtrl2->GetQSibd()->Get(m_kSibd, dSibd) == FALSE) return;
	if (m_pDoc->m_pAttrCtrl2->GetQSipa()->Get(dSibd.kSipa, dSipa) == FALSE) return;
	int nRebarCutPos = pEval->nRebarCutPos;

	const int nCompIdx = [nDir, nRebarCutPos]()
	{
		switch (nDir)
		{
		case seis_jp::kY_Dir:
			return (nRebarCutPos == T_SIGR_LOWER_COLM_D::kI) ? STRB_MOMENT_Z_I : STRB_MOMENT_Z_J;
		case seis_jp::kZ_Dir:
			return (nRebarCutPos == T_SIGR_LOWER_COLM_D::kI) ? STRB_MOMENT_Y_I : STRB_MOMENT_Y_J;
		default: ASSERT(0); return 2;
		}
	}();

	//int nAnalType = D_LOADCASE_HISTORY;
	double dStrengthSumMax = 0, dStrengthSumMin = 0;

	int nCount = dSipa.aSeisThisK.GetCount();
	for (int i = 0; i < nCount; i++)
	{
		T_THIS_K kThis = dSipa.aSeisThisK[i];

		T_LCOM_K LcomKey = kThis;
		T_LCOM_D LcomData;
		m_pDoc->m_pPostCtrl->MakeLoadComb(D_LOADCASE_HISTORY, LcomKey, LcomData);
		m_pDoc->m_pPostCtrl->SelectLoadCombNew(LcomData);

		T_STRB_D max, min;
		m_pDoc->m_pPostCtrl->GetStrbNew(pEval->kElem, &max, 0, 0, FALSE);
		m_pDoc->m_pPostCtrl->GetStrbNew(pEval->kElem, 0, &min, 0, FALSE);

		double dMax = 0, dMin = 0;
		if (nRebarCutPos == T_SIGR_LOWER_COLM_D::kI) {
			dMax = max.dblForce[STRB_POSI_I][nCompIdx];
			dMin = min.dblForce[STRB_POSI_I][nCompIdx];
		}
		else if (nRebarCutPos == T_SIGR_LOWER_COLM_D::kJ) {
			dMax = max.dblForce[STRB_POSI_J][nCompIdx];
			dMin = min.dblForce[STRB_POSI_J][nCompIdx];
		}

		dStrengthSumMax += dMax;
		dStrengthSumMin += dMin;
		pEval->analStrength.aSmax.Add(dMax);
		pEval->analStrength.aSmin.Add(dMin);
	}

	double dCount = dSipa.aSeisThisK.GetCount();

	pEval->analStrength.dAvergeMax = dgn::SafeDiv(dStrengthSumMax, (double)nCount);
	pEval->analStrength.dAvergeMin = dgn::SafeDiv(dStrengthSumMin, (double)nCount);
}
#pragma endregion <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<