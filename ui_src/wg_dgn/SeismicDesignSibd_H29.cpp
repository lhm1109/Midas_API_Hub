#include "stdafx.h"
#include "SeismicDesignSibd_H29.h"

#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_PostCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl2.h"
#include "..\wg_db\DB_QUERY_SEIS_CVL_JP.h"
#include "..\wg_db\BrdgSeisEvalResult_JP.h"

#include "..\wg_db\wg_db_DBDoc.h"

#include "DgnProgressDlg.h"
#include "BrdgSeisEval_JP.h"

#pragma region >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> CSeismicDesignSibd_H14 
CSeismicDesignSibd_H29::CSeismicDesignSibd_H29() : CSeismicDesignDisp()
{
	m_mMemberMap.clear();
}

CSeismicDesignSibd_H29::~CSeismicDesignSibd_H29()
{
}

void CSeismicDesignSibd_H29::AddProgressStep(CDgnProgressDlg* pProgress)
{
	T_KEY_LIST sibdKeys;
	m_pDoc->m_pAttrCtrl2->GetQSibd()->GetKeyList(sibdKeys);

	for (int idx = 0; idx < sibdKeys.GetCount(); idx++)
	{
		T_SIBD_K kSibd = sibdKeys[idx];
		T_SIBD_D dSibd; T_SIPA_D dSipa; T_SIGR_D dSigr; T_SIGR_LOWER_D dLower;
		if (m_pDoc->m_pAttrCtrl2->GetQSibd()->Get(kSibd, dSibd) == FALSE) continue;
		if (m_pDoc->m_pAttrCtrl2->GetQSipa()->Get(dSibd.kSipa, dSipa) == FALSE) continue;
		if (m_pDoc->m_pAttrCtrl2->GetQSigr()->Get(dSibd.kSigr, dSigr) == FALSE) continue;
		if (m_pDoc->m_pAttrCtrl2->GetQSigrLower()->Get(dSibd.kSigr, dLower) == FALSE) return;
		if (dSipa.nDesignCode != GetCurrentDgnCode()) continue;

		switch (dSibd.nSibdType)
		{
		case T_SIBD_D::kStrengthSurvey:
			CSeismicDesignSibd_H29_Strength(pProgress, kSibd).AddProgressStep();
			break;
		case T_SIBD_D::kCurvatureSurvey:
			if (dSibd.CVSV_D.nCvsvType == T_SIBD_CVSV_D::kSurrenderSurvey)
				CSeismicDesignSibd_H14_Surrender(pProgress, kSibd).AddProgressStep();
			else if (dSibd.CVSV_D.nCvsvType == T_SIBD_CVSV_D::kLimitStateSurvey)
				CSeismicDesignSibd_H29_LimitState(pProgress, kSibd).AddProgressStep();
			break;
		default:
			break;
		}

		if (dSigr.nSubType == T_SIGR_D::kLower)
			CSeismicDesignSibd_H29_RebarReducedPart(pProgress, kSibd).AddProgressStep();
	}
}

BOOL CSeismicDesignSibd_H29::Calculate(CDgnProgressDlg* pProgress)
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
			CSeismicDesignSibd_H29_Strength::Calculate(pProgress, kSibd);
			break;
		case T_SIBD_D::kCurvatureSurvey:
			if (dSibd.CVSV_D.nCvsvType == T_SIBD_CVSV_D::kSurrenderSurvey)
				CSeismicDesignSibd_H14_Surrender::Calculate(pProgress, kSibd);
			else if (dSibd.CVSV_D.nCvsvType == T_SIBD_CVSV_D::kLimitStateSurvey)
				CSeismicDesignSibd_H29_LimitState::Calculate(pProgress, kSibd);
			break;
		default:
			break;
		}

		if (dSigr.nSubType == T_SIGR_D::kLower)
			CSeismicDesignSibd_H29_RebarReducedPart::Calculate(pProgress, kSibd);
	}

	return TRUE;
}

#pragma endregion <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

#pragma region >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> CSeismicDesignSibd_H29_Strength
CSeismicDesignSibd_H29_Strength::CSeismicDesignSibd_H29_Strength(CDgnProgressDlg* pProgress, const T_SIBD_K& kSibd)
	: CSeismicDesignSibd_H14_Strength(pProgress, kSibd)
{
}

CSeismicDesignSibd_H29_Strength::~CSeismicDesignSibd_H29_Strength()
{
}

void CSeismicDesignSibd_H29_Strength::AddProgressStep()
{
	CSeismicDesignSibd_H14_Strength::AddProgressStep();
}

void CSeismicDesignSibd_H29_Strength::Calculate(CDgnProgressDlg* pProgress, const T_SIBD_K& kSibd)
{
	CSeismicDesignSibd_H29_Strength(pProgress, kSibd).Invoke();
}

void CSeismicDesignSibd_H29_Strength::Invoke()
{
	CSeismicDesignSibd_H14_Strength::Invoke();
}

void CSeismicDesignSibd_H29_Strength::GetMomentFromMPhi(T_SIBD_EVAL_MOMENT* pEval)
{
	T_SIBD_D dSibd; T_SIPA_D dSipa; T_SIFA_H29_D dSifaH29;
	if (m_pDoc->m_pAttrCtrl2->GetQSibd()->Get(m_kSibd, dSibd) == FALSE) return;
	if (m_pDoc->m_pAttrCtrl2->GetQSipa()->Get(dSibd.kSipa, dSipa) == FALSE) return;
	if (m_pDoc->m_pAttrCtrl2->GetQSifaH29()->Get(dSifaH29) == FALSE) return;

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

	// 상부구조 항복 모멘트 제한치	 Myd = ξ1*φy*Myc
	auto L_Myd = [&](const double& dStrengthY)
		{
			double dXi1 = 0.0, dDeltaY = 0.0;
			if (dSipa.nEarthquakeType == T_SIPA_D::kFluctuating_No10) {
				dXi1 = dSifaH29.RcMomentMyd.No10[T_SIFA_RC_Myd::kXi1];
				dDeltaY = dSifaH29.RcMomentMyd.No10[T_SIFA_RC_Myd::kPhiY];
			}
			else if (dSipa.nEarthquakeType == T_SIPA_D::kAccidental_No11) {
				dXi1 = dSifaH29.RcMomentMyd.No11[T_SIFA_RC_Myd::kXi1];
				dDeltaY = dSifaH29.RcMomentMyd.No11[T_SIFA_RC_Myd::kPhiY];
			}

			return dXi1 * dDeltaY * dStrengthY;
		};

	// 상부구조 파괴 모멘트의 제한치 Mud = ξ1*ξ2*φu*Muc
	auto L_Mud = [&](const double& dStrengthU)
		{
			double dXi1 = 0.0, dXi2 = 0.0, dDeltaU = 0.0;
			if (dSipa.nEarthquakeType == T_SIPA_D::kFluctuating_No10) {
				dXi1 = dSifaH29.RcMomentMud.No10[T_SIFA_RC_Mud::kXi1];
				dXi2 = dSifaH29.RcMomentMud.No10[T_SIFA_RC_Mud::kXi2];
				dDeltaU = dSifaH29.RcMomentMud.No10[T_SIFA_RC_Mud::kPhiU];
			}
			else if (dSipa.nEarthquakeType == T_SIPA_D::kAccidental_No11) {
				dXi1 = dSifaH29.RcMomentMud.No11[T_SIFA_RC_Mud::kXi1];
				dXi2 = dSifaH29.RcMomentMud.No11[T_SIFA_RC_Mud::kXi2];
				dDeltaU = dSifaH29.RcMomentMud.No11[T_SIFA_RC_Mud::kPhiU];
			}

			return dXi1 * dXi2 * dDeltaU * dStrengthU;
		};

	pEval->calcMomeMax.dC = prop_i.dStrength_C[MPhiProp::kPlus];
	pEval->calcMomeMax.dY = L_Myd(prop_i.dStrength_Y[MPhiProp::kPlus]);
	pEval->calcMomeMax.dU = L_Mud(prop_i.dStrength_U[MPhiProp::kPlus]);

	pEval->calcMomeMin.dC = -1 * prop_j.dStrength_C[MPhiProp::kMinus];
	pEval->calcMomeMin.dY = -1 * L_Myd(prop_j.dStrength_Y[MPhiProp::kMinus]);
	pEval->calcMomeMin.dU = -1 * L_Mud(prop_j.dStrength_U[MPhiProp::kMinus]);
}
#pragma endregion <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

#pragma region >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> CSeismicDesignSibd_H29_LimitState
CSeismicDesignSibd_H29_LimitState::CSeismicDesignSibd_H29_LimitState(CDgnProgressDlg* pProgress, const T_SIBD_K& kSibd)
	: CSeismicDesignSibd_H14_LimitState(pProgress, kSibd)
{
}

CSeismicDesignSibd_H29_LimitState::~CSeismicDesignSibd_H29_LimitState()
{
}

void CSeismicDesignSibd_H29_LimitState::AddProgressStep()
{
	CSeismicDesignSibd_H14_LimitState::AddProgressStep();
}

void CSeismicDesignSibd_H29_LimitState::Calculate(CDgnProgressDlg* pProgress, const T_SIBD_K& kSibd)
{
	CSeismicDesignSibd_H29_LimitState(pProgress, kSibd).Invoke();
}

void CSeismicDesignSibd_H29_LimitState::Invoke()
{
	CSeismicDesignSibd_H14_LimitState::Invoke();
}

BOOL CSeismicDesignSibd_H29_LimitState::GetPhi_a(T_SIBD_D* pSibd, OUT T_SIBD_EVAL_CALC_PHI_A* pDeform)
{
	T_SIBD_D dSibd; T_SIPA_D dSipa; T_SIFA_H29_D dSifaH29;
	if (m_pDoc->m_pAttrCtrl2->GetQSibd()->Get(m_kSibd, dSibd) == FALSE) return FALSE;
	if (m_pDoc->m_pAttrCtrl2->GetQSipa()->Get(dSibd.kSipa, dSipa) == FALSE) return FALSE;
	if (m_pDoc->m_pAttrCtrl2->GetQSifaH29()->Get(dSifaH29) == FALSE) return FALSE;

	auto L_GetFactor = [&](const T_SIFA_RC_Curv::eRcCurv& facor)
		{
			double dFactor = 0.0;
			switch (dSipa.nLimitType)
			{
			case T_SIPA_D::kLimitState2:
				dFactor = dSifaH29.RcCurvature.Phi_ls2d[facor];
				break;
			case T_SIPA_D::kLimitState3:
				dFactor = dSifaH29.RcCurvature.Phi_ls3d[facor];
				break;
			}
			return dFactor;
		};

	double dXi1 = L_GetFactor(T_SIFA_RC_Curv::kXi1);
	double dXi2 = (dSipa.nLimitType != T_SIPA_D::kLimitState3) ? 1.0 : L_GetFactor(T_SIFA_RC_Curv::kXi2);
	double dPhiS = L_GetFactor(T_SIFA_RC_Curv::kPhiS);
	double dK = L_GetFactor(T_SIFA_RC_Curv::kK);

	//φls2d = ξ1*φs*k*φls2
	//φls3d = ξ1*ξ2*φs*k*φls3

	pDeform->dPhi_a = dXi1 * dXi2* dPhiS * dK * pDeform->dPhi_u;

	T_SIGR_LOWER_BEAR_D::eBearingType bearType = m_pDoc->m_pAttrCtrl2->GetQSigrLower()->GetBearingType(m_kSigr);
	if (bearType == T_SIGR_LOWER_BEAR_D::kSeismicIsolation) {
		//φls2d/αm
		pDeform->dPhi_a = dgn::SafeDiv(pDeform->dPhi_a, pSibd->CVSV_D.dLimitStateAlpha);
	}
	return TRUE;
}

#pragma endregion <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

#pragma region >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> CSeismicDesignSibd_H29_RebarReducedPart
CSeismicDesignSibd_H29_RebarReducedPart::CSeismicDesignSibd_H29_RebarReducedPart(CDgnProgressDlg* pProgress, const T_SIBD_K& kSibd)
	: CSeismicDesignSibd_H14()
{
	m_pProgress = pProgress;
	SetMemberVariable(kSibd);
}

CSeismicDesignSibd_H29_RebarReducedPart::~CSeismicDesignSibd_H29_RebarReducedPart()
{
}

void CSeismicDesignSibd_H29_RebarReducedPart::AddProgressStep()
{
	T_SIPA_D dSipa; T_SIGR_D dSigr; T_SIGR_LOWER_D dLower;
	if (m_pDoc->m_pAttrCtrl2->GetQSipa()->Get(m_kSipa, dSipa) == FALSE) return;
	if (m_pDoc->m_pAttrCtrl2->GetQSigr()->Get(m_kSigr, dSigr) == FALSE) return;
	if (dSigr.nSubType != T_SIGR_D::kLower) return;
	if (m_pDoc->m_pAttrCtrl2->GetQSigrLower()->Get(m_kSigr, dLower) == FALSE) return;

	T_ELEM_K_LIST kElemList;
	MakeMemberMap(m_kSigr, kElemList); //m_mMemberMap 채워야 함.

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

void CSeismicDesignSibd_H29_RebarReducedPart::Calculate(CDgnProgressDlg* pProgress, const T_SIBD_K& kSibd)
{
	CSeismicDesignSibd_H29_RebarReducedPart(pProgress, kSibd).Invoke();
}

void CSeismicDesignSibd_H29_RebarReducedPart::Invoke()
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
			RunEvaluation(colm, kElem, nPos, m_mMemberMap[kElem].nDirShear);
		}

		if (m_pProgress->NextStep() == FALSE) {
			throw FALSE;
		}
	}
}

void CSeismicDesignSibd_H29_RebarReducedPart::RunEvaluation(const T_SIGR_LOWER_COLM_D& colm, const T_ELEM_K& kElem, const int& nPos, const int& nExamShearDir)
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

	// 손상판정
	ReviewDamaged(colm, &eval);

	// save result
	CBrdgSeisEvalResult_JP* pResult = m_pDoc->m_pPostCtrl->GetBrdgSeisEvalResult_JP(); if (!pResult) { ASSERT(0); }
	if (pResult) pResult->SetResultSibdRebarCut(eval);
}

BOOL CSeismicDesignSibd_H29_RebarReducedPart::ReviewDamaged(const T_SIGR_LOWER_COLM_D& colm, OUT T_SIBD_EVAL_REBAR_CUT* pEval)
{
	T_SIGR_LOWER_D dLower;
	if (m_pDoc->m_pAttrCtrl2->GetQSigrLower()->Get(m_kSigr, dLower) == FALSE) return FALSE;

	ColumnData colData;
	if (m_pDoc->m_pAttrCtrl2->GetQSigrLower()->FindColumnLowNode(m_kSigr, colm.strName, colData) == FALSE) return FALSE;

	T_ELEM_D tElem;
	if (m_pDoc->m_pAttrCtrl->GetElem(pEval->kElem, tElem) == FALSE) return FALSE;

	//철근 단락부 절점
	T_NODE_K kNodeRebarCut = tElem.elnod[pEval->nRebarCutPos];
	//교각기부 절점
	T_NODE_K kNodePierBot = colData.kNodeLow;

	T_NODE_D tNodeRebarCut;
	if (m_pDoc->m_pAttrCtrl->GetNode(kNodeRebarCut, tNodeRebarCut) == FALSE) return FALSE;

	T_NODE_D tNodePierBot;
	if (m_pDoc->m_pAttrCtrl->GetNode(kNodePierBot, tNodePierBot) == FALSE) return FALSE;

	//교각기부에서 철근 단락부까지의 높이
	pEval->DamageH29Max.dHi = tNodeRebarCut.z - tNodePierBot.z;
	ASSERT(pEval->DamageH29Max.dHi > 0.0);
	pEval->DamageH29Min.dHi = pEval->DamageH29Max.dHi;

	//상부구조 관성력 작용위치
	T_NODE_D tNodePierTop;
	if (pEval->nSeismicForceDirection == T_SIPA_D::kAxis_Dir)
		m_pDoc->m_pAttrCtrl->GetNode(dLower.kNodeAxis, tNodePierTop);
	else
		m_pDoc->m_pAttrCtrl->GetNode(dLower.kNodePerp, tNodePierTop);

	//교각기부에서 상부구조 관성력 작용위치까지의 높이
	pEval->DamageH29Max.dH = tNodePierTop.z - tNodePierBot.z;
	ASSERT(pEval->DamageH29Max.dH > 0.0);
	pEval->DamageH29Min.dH = pEval->DamageH29Max.dH;

	int nComponent = GetComponent(pEval->nExamShearDir);
	if (nComponent == QIehp::kUnknown) return FALSE;

	//철근 단락부 단면의 항복 굽힘 모멘트
	MPhiProp MPhiRebarCut;
	if (m_pDoc->m_pAttrCtrl2->GetQIehp()->GetHingeProp(pEval->kElem, nComponent, pEval->nRebarCutPos, MPhiRebarCut) == FALSE) return FALSE;
	pEval->DamageH29Max.dMyi = MPhiRebarCut.dStrength_Y[MPhiProp::kPlus];
	pEval->DamageH29Min.dMyi = MPhiRebarCut.dStrength_Y[MPhiProp::kMinus];

	//교각기부 단면의 항복 굽힘 모멘트
	MPhiProp MPhiPierBot;
	if (m_pDoc->m_pAttrCtrl2->GetQIehp()->GetHingeProp(colData.kElem, nComponent, colData.ePosNodeLow, MPhiPierBot) == FALSE) return FALSE;
	pEval->DamageH29Max.dMyB = MPhiPierBot.dStrength_Y[MPhiProp::kPlus];
	pEval->DamageH29Min.dMyB = MPhiPierBot.dStrength_Y[MPhiProp::kMinus];

	//철근 단락부 위치의 교각 치수
	double hi = 0, bi = 0, hj = 0, bj = 0;
	if (m_pDoc->m_pAttrCtrl->GetSectDimen(tElem.elpro, hi, bi, hj, bj, NULL, FALSE) == FALSE) return FALSE;
	if (pEval->nRebarCutPos == QIehp::ePosI) {
		pEval->DamageH29Max.dD = (hi < bi) ? hi : bi;
		pEval->DamageH29Min.dD = pEval->DamageH29Max.dD;
	}
	else if (pEval->nRebarCutPos == QIehp::ePosJ) {
		pEval->DamageH29Max.dD = (hj < bj) ? hj : bj;
		pEval->DamageH29Min.dD = pEval->DamageH29Max.dD;
	}
	else {
		return FALSE;
	}

	//h(1-Myi/(2*MyB))+D
	pEval->DamageH29Max.dVal = pEval->DamageH29Max.dH * (1.0 - pEval->DamageH29Max.dMyi / (2.0 * pEval->DamageH29Max.dMyB)) + pEval->DamageH29Max.dD;
	pEval->DamageH29Min.dVal = pEval->DamageH29Min.dH * (1.0 - pEval->DamageH29Min.dMyi / (2.0 * pEval->DamageH29Min.dMyB)) + pEval->DamageH29Min.dD;
	return TRUE;
}
#pragma endregion <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<