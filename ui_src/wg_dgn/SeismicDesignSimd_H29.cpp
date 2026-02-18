#include "stdafx.h"
#include "SeismicDesignSimd_H29.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_PostCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl2.h"
#include "..\wg_db\DB_QUERY_SEIS_CVL_JP.h"
#include "..\wg_db\BrdgSeisEvalResult_JP.h"

#include "BrdgSeisEval_JP.h"

#pragma region // CSeismicDesignSimd_H29

CSeismicDesignSimd_H29::CSeismicDesignSimd_H29()
{
	m_kPierTopNode = 0;
	m_pSigrLower = nullptr;
	m_pColumn = nullptr;
}

void CSeismicDesignSimd_H29::AddProgressStep(CDgnProgressDlg* pProgress)
{
	CSeismicDesignSimd::AddStep(pProgress);
}

BOOL CSeismicDesignSimd_H29::Calculate(CDgnProgressDlg* pProgress)
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	ASSERT(pDoc);
	if (pDoc == nullptr) return FALSE;

	T_SIDC_D dSidc; dSidc.Initialize();
	if (pDoc->m_pAttrCtrl2->GetQSidc()->Get(dSidc) == FALSE) return FALSE;

	// Code 단위계 변환
	CUnitConversion unit(D_UNITSYS_FORCE_INDEX_N, D_UNITSYS_LENGTH_INDEX_M);

	T_KEY_LIST simdKeys;
	pDoc->m_pAttrCtrl2->GetQSimd()->GetKeyList(simdKeys);

	for (int idx = 0; idx < simdKeys.GetCount(); idx++)
	{
		T_SIMD_K kSimd = simdKeys[idx];
		T_SIMD_D dSimd; T_SIPA_D dSipa; T_SIGR_D dSigr;
		if (pDoc->m_pAttrCtrl2->GetQSimd()->Get(kSimd, dSimd) == FALSE) continue;
		if (pDoc->m_pAttrCtrl2->GetQSipa()->Get(dSimd.kSipa, dSipa) == FALSE) continue;
		if (pDoc->m_pAttrCtrl2->GetQSigr()->Get(dSimd.kSigr, dSigr) == FALSE) continue;
		if (dSipa.nDesignCode != dSidc.nDesignCode) continue;

		if (dSigr.nSubType == T_SIGR_D::kUpper) {
			CalculateUpper(pProgress, kSimd);
		}
		else if (dSigr.nSubType == T_SIGR_D::kLower) {
			CalculateLower(pProgress, kSimd);
		}
	}

	return FALSE;
}

void CSeismicDesignSimd_H29::CalculateUpper(CDgnProgressDlg* pProgress, const T_SIMD_K& kSimd)
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();

	T_SIMD_D dSimd; T_SIPA_D dSipa; T_SIGR_UPPER_D dSigrUpper;
	if (pDoc->m_pAttrCtrl2->GetQSimd()->Get(kSimd, dSimd) == FALSE) return;
	if (pDoc->m_pAttrCtrl2->GetQSipa()->Get(dSimd.kSipa, dSipa) == FALSE) return;
	if (pDoc->m_pAttrCtrl2->GetQSigrUpper()->Get(dSimd.kSigr, dSigrUpper) == FALSE) return;

	//좌단 검토
	T_SIMD_EVAL evalLeft;
	evalLeft.kSimd = kSimd;
	evalLeft.kSipa = dSimd.kSipa;
	evalLeft.kSigr = dSimd.kSigr;
	evalLeft.nDesignCode = dSipa.nDesignCode;
	evalLeft.nAlwCalcType = dSimd.nAlwCalcType;
	evalLeft.nRspCalcType = dSimd.nRspCalcType;
	evalLeft.nSeismicForceType = dSipa.nSeismicForceType;
	evalLeft.kGirderNode = dSigrUpper.kNodeL;
	evalLeft.nPosType = T_SIMD_EVAL::kLeft;

	CSeismicDesignSimd_H29_Upper SimdLeft(pProgress);
	if (SimdLeft.Calculate(kSimd, &evalLeft) == FALSE) return;

	//우단 검토
	T_SIMD_EVAL evalRight;
	evalRight.kSimd = kSimd;
	evalRight.kSipa = dSimd.kSipa;
	evalRight.kSigr = dSimd.kSigr;
	evalRight.nDesignCode = dSipa.nDesignCode;
	evalRight.nAlwCalcType = dSimd.nAlwCalcType;
	evalRight.nRspCalcType = dSimd.nRspCalcType;
	evalRight.nSeismicForceType = dSipa.nSeismicForceType;
	evalRight.kGirderNode = dSigrUpper.kNodeR;
	evalRight.nPosType = T_SIMD_EVAL::kRight;

	CSeismicDesignSimd_H29_Upper SimdRight(pProgress);
	SimdRight.Calculate(kSimd, &evalRight);

	// save result
	CBrdgSeisEvalResult_JP* pResult = pDoc->m_pPostCtrl->GetBrdgSeisEvalResult_JP(); if (!pResult) { ASSERT(0); }
	pResult->SetResultSimd(dSimd.kSipa, evalLeft.kSigr, evalLeft.nPosType, evalLeft);
	pResult->SetResultSimd(dSimd.kSipa, evalRight.kSigr, evalRight.nPosType, evalRight);
}

void CSeismicDesignSimd_H29::CalculateLower(CDgnProgressDlg* pProgress, const T_SIMD_K& kSimd)
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();

	T_SIMD_D dSimd; T_SIPA_D dSipa; T_SIGR_LOWER_D dSigrLower;
	if (pDoc->m_pAttrCtrl2->GetQSimd()->Get(kSimd, dSimd) == FALSE) return;
	if (pDoc->m_pAttrCtrl2->GetQSipa()->Get(dSimd.kSipa, dSipa) == FALSE) return;
	if (pDoc->m_pAttrCtrl2->GetQSigrLower()->Get(dSimd.kSigr, dSigrLower) == FALSE) return;

	//교각별 최대응답변위 조사
	T_SIMD_EVAL eval;
	eval.kSimd = kSimd;
	eval.kSipa = dSimd.kSipa;
	eval.kSigr = dSimd.kSigr;
	eval.nDesignCode = dSipa.nDesignCode;
	eval.nAlwCalcType = dSimd.nAlwCalcType;
	eval.nRspCalcType = dSimd.nRspCalcType;
	eval.nSeismicForceType = dSipa.nSeismicForceType;
	eval.nPosType = T_SIMD_EVAL::kLower;

	CSeismicDesignSimd_H29_Lower Simd(pProgress);
	Simd.Calculate(kSimd, &eval);

	// save result
	CBrdgSeisEvalResult_JP* pResult = pDoc->m_pPostCtrl->GetBrdgSeisEvalResult_JP(); if (!pResult) { ASSERT(0); }
	pResult->SetResultSimd(dSimd.kSipa, eval.kSigr, eval.nPosType, eval);
}

BOOL CSeismicDesignSimd_H29::GetDelta_yEd_fromAnalysis(IN T_THIS_K& kThis, IN T_SIMD_SPLD_D* pSpld, OUT double& rDelta_yEd)
{
	// δy: 교각 관성력 작용위치에서의 푸쉬오버해석 지정스텝의 변위
	double aDelta_y[eVector2d::kMax2d] = { 0.0, 0.0 };
	if (GetDispFromTimeHistory(kThis, m_kPierTopNode, pSpld->nStepDeltaY, aDelta_y[eVector2d::kDx], aDelta_y[eVector2d::kDy]) == FALSE) return FALSE;

	QSifaH29* pSifa = m_pDoc->m_pAttrCtrl2->GetQSifaH29();

	double dXi1 = pSifa->GetXi1(m_param->tSipa.nLimitType, m_pSigrLower->nMaterialType);
	double dPhi_RY = pSifa->GetPhiS(m_param->tSipa.nLimitType, m_pSigrLower->nMaterialType);

	// δyEd = ξ1・φRY・δy
	double dDelta_yEd_Ly = dXi1 * dPhi_RY * aDelta_y[eLocal2d::kLy];
	double dDelta_yEd_Lz = dXi1 * dPhi_RY * aDelta_y[eLocal2d::kLz];

	ParamAngleOption param;
	param.kThis = kThis;
	param.kElem = m_pColumn->aInvestigatedElem[0];
	param.nDirDisp = m_param->tSimd.nDirDisp;
	param.nShearDir = m_pColumn->nDirShear;
	param.dUserAngle = m_param->tSimd.dAngle;
	param.nSeismicForceDirection = m_param->tSipa.nSeismicForceDirection;

	GetDispOfAngle(param, dDelta_yEd_Ly, dDelta_yEd_Lz, rDelta_yEd);

	return TRUE;
}
#pragma endregion

#pragma region // CSeismicDesignSimd_H29_Upper

CSeismicDesignSimd_H29_Upper::CSeismicDesignSimd_H29_Upper(CDgnProgressDlg* pProgress) : CSeismicDesignSimd_H29()
{
	m_pProgress = pProgress;
}

BOOL CSeismicDesignSimd_H29_Upper::Calculate(const T_SIMD_K& kSimd, T_SIMD_EVAL* pEval)
{
	return CSeismicDesignSimd::Calculate(kSimd, pEval);
}

BOOL CSeismicDesignSimd_H29_Upper::CalcDeltaA(const T_THIS_K& kThis, IN T_SIMD_SPLD_D* pSpld, OUT doubleArray& aDeltaA)
{
	P_SIMD_Upper input(m_param, kThis, pSpld);
	if (input.IsValidValue() == FALSE) return FALSE;
	m_PUpper = &input;
	m_kPierTopNode = m_PUpper->kPierTopNode;
	m_pSigrLower = &m_PUpper->tSigrLower;
	m_pColumn = &m_PUpper->tCOLM;

	if (m_param->tSipa.nLimitType != T_SIPA_D::kLimitState2 && m_param->tSipa.nLimitType != T_SIPA_D::kLimitState3)
		return FALSE;

	//직접입력의 경우
	if (m_param->tSimd.nAlwCalcType == seis_jp::kUserInput) {
		double dDelta_a = (pSpld->nDirAnal == seis_jp::kMinus) ? -1 * pSpld->dLengthDeltaVar : pSpld->dLengthDeltaVar;
		aDeltaA[seis_jp::kDa_UserH14] = dDelta_a;
		return TRUE;
	}

	BOOL bResult = FALSE;

	//기초변위 영향분에 대해 부분계수 고려
	if (m_param->tSimd.bConsiderAlpha == TRUE) {
		bResult = CalcDeltaLsdByTotalDistance(kThis, pSpld, aDeltaA);
	}
	else {
		bResult = CalcDeltaLsdByPureDistance(kThis, pSpld, aDeltaA);
	}

	if (bResult == FALSE) return FALSE;

	T_SIGR_LOWER_BEAR_D::eBearingType bearType = m_pDoc->m_pAttrCtrl2->GetQSigrLower()->GetBearingType(pSpld->kSigrLower);
	//※ 면진교를 적용한 경우, 수평변위 한계치는 한계상태 2, 3과 관계없이 δls2d/αm 을 한계치로 사용합니다. 여기서, αm=2.0
	if (bearType == T_SIGR_LOWER_BEAR_D::kSeismicIsolation) {
		const double dDelta_m = 2.0;
		double dVal = dgn::SafeDiv(aDeltaA[seis_jp::kDlsd_AnalH29], dDelta_m);
		double dDelta_yEd = 0;
		// 단, δls2d/αm 가 δyEd 이하가 되는 경우는 δlsd=δyEd 로 한다.
		if (GetDelta_yEd_fromAnalysis(m_PUpper->kThis, m_PUpper->pSpld, dDelta_yEd) == TRUE && dVal < dDelta_yEd) {
			aDeltaA[seis_jp::kDlsd_AnalH29] = dDelta_yEd;
		}
		else {
			aDeltaA[seis_jp::kDlsd_AnalH29] = dVal;
		}
	}

	return bResult;
}

BOOL CSeismicDesignSimd_H29_Upper::CalcDeltaLsdByTotalDistance(const T_THIS_K& kThis, IN T_SIMD_SPLD_D* pSpld, OUT doubleArray& aDeltaA)
{
	// δrls: 교각 관성력 작용위치에서의 푸쉬오버해석 지정스텝의 변위
	double aDelta_rls[eVector2d::kMax2d] = { 0.0, 0.0 };
	if (GetDispFromTimeHistory(kThis, m_PUpper->kPierTopNode, pSpld->nStepDeltaVar, aDelta_rls[eVector2d::kDx], aDelta_rls[eVector2d::kDy]) == FALSE) return FALSE;

	QSifaH29* pSifa = m_pDoc->m_pAttrCtrl2->GetQSifaH29();

	const double dXi1 = pSifa->GetXi1(m_param->tSipa.nLimitType, m_PUpper->tSigrLower.nMaterialType);
	const double dPhi_RY = pSifa->GetPhiS(m_param->tSipa.nLimitType, m_PUpper->tSigrLower.nMaterialType);

	// δlsd ＝ ξ1・Φs・δrls
	double dDelta_yEd_Ly = dXi1 * dPhi_RY * aDelta_rls[eVector2d::kDx];
	double dDelta_yEd_Lz = dXi1 * dPhi_RY * aDelta_rls[eVector2d::kDy];

	ParamAngleOption param;
	param.kThis = kThis;
	param.kElem = m_PUpper->tCOLM.aInvestigatedElem[0];
	param.nDirDisp = m_param->tSimd.nDirDisp;
	param.nShearDir = m_PUpper->tCOLM.nDirShear;
	param.dUserAngle = m_param->tSimd.dAngle;
	param.nSeismicForceDirection = m_param->tSipa.nSeismicForceDirection;

	double dDelta_lsd = 0;//합성벡터
	GetDispOfAngle(param, dDelta_yEd_Ly, dDelta_yEd_Lz, dDelta_lsd);

	aDeltaA[seis_jp::kDlsd_AnalH29] = dDelta_lsd;
	return TRUE;
}

BOOL CSeismicDesignSimd_H29_Upper::CalcDeltaLsdByPureDistance(const T_THIS_K& kThis, IN T_SIMD_SPLD_D* pSpld, OUT doubleArray& aDeltaA)
{
	T_SIGR_LOWER_D tSigrLower;
	if (m_pDoc->m_pAttrCtrl2->GetQSigrLower()->Get(pSpld->kSigrLower, tSigrLower) == FALSE) return FALSE;

	ColumnData colm;
	if (m_pDoc->m_pAttrCtrl2->GetQSigrLower()->FindColumnLowNode(pSpld->kSigrLower, pSpld->strNameCOLM, colm) == FALSE) return FALSE;

	// δls: 교각 관성력 작용위치에서의 푸쉬오버해석 지정스텝의 변위
	double aDelta_ls[eVector2d::kMax2d] = { 0.0, 0.0 };
	if (GetDispFromTimeHistory(kThis, m_PUpper->kPierTopNode, pSpld->nStepDeltaVar, aDelta_ls[eVector2d::kDx], aDelta_ls[eVector2d::kDy]) == FALSE) return FALSE;

	PDeltaVal deltaParam = std::make_tuple(	pSpld->kSigrLower, 
											pSpld->kThis, 
											m_kPierTopNode, 
											(seis_jp::eDirAnal)pSpld->nDirAnal,
											m_param->tSimd.nRspCalcType, 
											&m_PUpper->tLowColmData);

	//δfls=δfd+Θfp*h+δs
	double aDelta_fls[eVector2d::kMax2d] = { 0.0, 0.0 };
	if (GetF_ls(deltaParam, pSpld->nStepDeltaVar, aDelta_fls[eVector2d::kDx], aDelta_fls[eVector2d::kDy]) == FALSE) return FALSE;

	QSifaH29* pSifa = m_pDoc->m_pAttrCtrl2->GetQSifaH29();

	const double dXi1 = pSifa->GetXi1(m_param->tSipa.nLimitType, tSigrLower.nMaterialType);
	const double dXi2 = (m_param->tSipa.nLimitType != T_SIPA_D::eLimitStateType::kLimitState3) ? 1.0 : pSifa->GetXi2(m_param->tSipa.nLimitType, tSigrLower.nMaterialType);
	const double dPhiS = pSifa->GetPhiS(m_param->tSipa.nLimitType, tSigrLower.nMaterialType);
	const double dK = pSifa->GetFactorK(tSigrLower.nMaterialType);

	double dDeltaLs_dx = 0, dDeltaLs_dy = 0;
	switch (m_param->tSipa.nLimitType)
	{
	case T_SIPA_D::kLimitState2:
		// δls2d = ξ1*φs(δls2 - k*δfls2)+δfls2
		dDeltaLs_dx = dXi1 * dPhiS * (aDelta_ls[eVector2d::kDx] - dK * aDelta_fls[eVector2d::kDx]) + aDelta_fls[eVector2d::kDx];
		dDeltaLs_dy = dXi1 * dPhiS * (aDelta_ls[eVector2d::kDy] - dK * aDelta_fls[eVector2d::kDy]) + aDelta_fls[eVector2d::kDy];
		break;
	case T_SIPA_D::kLimitState3:
		// δls3d = ξ1*ξ2*φs(δls3 - k*δfls3)+δfls3
		dDeltaLs_dx = dXi1 * dXi2 * dPhiS * (aDelta_ls[eVector2d::kDx] - dK * aDelta_fls[eVector2d::kDx]) + aDelta_fls[eVector2d::kDx];
		dDeltaLs_dy = dXi1 * dXi2 * dPhiS * (aDelta_ls[eVector2d::kDy] - dK * aDelta_fls[eVector2d::kDy]) + aDelta_fls[eVector2d::kDy];
		break;
	default:
		ASSERT(FALSE);
		return FALSE;
	}

	ParamAngleOption param;
	param.kThis = kThis;
	param.kElem = m_PUpper->tCOLM.aInvestigatedElem[0];
	param.nDirDisp = m_param->tSimd.nDirDisp;
	param.nShearDir = m_PUpper->tCOLM.nDirShear;
	param.dUserAngle = m_param->tSimd.dAngle;
	param.nSeismicForceDirection = m_param->tSipa.nSeismicForceDirection;

	double dDelta_lsd = 0;//합성벡터
	GetDispOfAngle(param, dDeltaLs_dx, dDeltaLs_dy, dDelta_lsd);

	aDeltaA[seis_jp::kDlsd_AnalH29] = dDelta_lsd;
	return TRUE;
}

#pragma endregion

#pragma region // CSeismicDesignSimd_H29_Lower

CSeismicDesignSimd_H29_Lower::CSeismicDesignSimd_H29_Lower(CDgnProgressDlg* pProgress) : CSeismicDesignSimd_H29()
{
	m_pProgress = pProgress;
	m_DispL.SetProgressDlg(pProgress);
}

BOOL CSeismicDesignSimd_H29_Lower::Calculate(const T_SIMD_K& kSimd, T_SIMD_EVAL* pEval)
{
	return CSeismicDesignSimd::Calculate(kSimd, pEval);
}

BOOL CSeismicDesignSimd_H29_Lower::CalcDeltaA(const T_THIS_K& kThis, IN T_SIMD_SPLD_D* pSpld, OUT doubleArray& aDeltaA)
{
	P_SIMD_Lower input(m_param, kThis, pSpld);
	if (input.IsValidValue() == FALSE) return FALSE;
	m_PLower = &input;

	BOOL bResult = FALSE;

	switch (m_param->tSimd.nAlwCalcType)
	{
	case seis_jp::kUserInput: {
		double dDelta_a = (pSpld->nDirAnal == seis_jp::kMinus) ? -1 * pSpld->dLengthDeltaVar : pSpld->dLengthDeltaVar;
		aDeltaA[seis_jp::kDa_UserH29] = dDelta_a;
		bResult = TRUE;
	}
		break;

	case seis_jp::kAnalysisResult:
		bResult = CalcDeltaA_fromAnalysis(aDeltaA);
		break;

	case seis_jp::kMphi_Dy:
		bResult = CalcDeltaA_fromkMphi_Dy(aDeltaA);
		break;

	case  seis_jp::kMphi_Dpy:
		bResult = CalcDeltaA_fromkMphi_Dpy(aDeltaA);
		break;

	default:
		ASSERT_ONCE(FALSE);
		break;
	}

	return bResult;
}
BOOL CSeismicDesignSimd_H29_Lower::CalcDeltaA_fromAnalysis(OUT doubleArray& aDeltaA)
{
	BOOL bResult = FALSE;

	if (m_PLower->tSigrLower.nMaterialType == T_SIGR_LOWER_D::kRC)
		bResult = CalcDeltaA_fromAnalysis_forRc(aDeltaA);
	else if (m_PLower->tSigrLower.nMaterialType == T_SIGR_LOWER_D::kSteel)
		bResult = CalcDeltaA_fromAnalysis_forSteel(aDeltaA);

	if (bResult == FALSE) return FALSE;

	T_SIGR_LOWER_BEAR_D::eBearingType bearType = m_pDoc->m_pAttrCtrl2->GetQSigrLower()->GetBearingType(m_PLower->pSpld->kSigrLower);
	//※ 면진교를 적용한 경우, 수평변위 한계치는 한계상태 2, 3과 관계없이 δls2d/αm 을 한계치로 사용합니다. 여기서, αm=2.0
	if (bearType == T_SIGR_LOWER_BEAR_D::kSeismicIsolation) {
		const double dDelta_m = 2.0;
		double dVal = dgn::SafeDiv(aDeltaA[seis_jp::kDlsd_AnalH29], dDelta_m);
		double dDelta_yEd = 0;
		// 단, δls2d/αm 가 δyEd 이하가 되는 경우는 δlsd=δyEd 로 한다.
		if (GetDelta_yEd_fromAnalysis(m_PLower->kThis, m_PLower->pSpld, dDelta_yEd) == TRUE && dVal < dDelta_yEd) {
			aDeltaA[seis_jp::kDlsd_AnalH29] = dDelta_yEd;
		}
		else {
			aDeltaA[seis_jp::kDlsd_AnalH29] = dVal;
		}
	}

	return bResult;
}

BOOL CSeismicDesignSimd_H29_Lower::CalcDeltaA_fromAnalysis_forRc(OUT doubleArray& aDeltaA)
{
	PDeltaVal deltaParam = std::make_tuple(	m_param->tSimd.kSigr, 
											m_PLower->pSpld->kThis, 
											m_PLower->kPierTopNode, 
											(seis_jp::eDirAnal)m_PLower->pSpld->nDirAnal,
											m_param->tSimd.nRspCalcType, 
											&m_PLower->tLowColmData);

	// δls = k*(δrls-δfd-Θfp×h-δs)
	double dDelta_ls[eVector2d::kMax2d] = { 0.0, 0.0 };
	if (m_DispL.CalcDeltaValAtStep(deltaParam, m_PLower->pSpld->nStepDeltaVar, dDelta_ls[eVector2d::kDx], dDelta_ls[eVector2d::kDy]) == FALSE) return FALSE;

	QSifaH29* pSifa = m_pDoc->m_pAttrCtrl2->GetQSifaH29();

	double dK = pSifa->GetFactorK(m_PLower->tSigrLower.nMaterialType);
	double dXi1 = pSifa->GetXi1(m_param->tSipa.nLimitType, m_PLower->tSigrLower.nMaterialType);
	double dPhiS = pSifa->GetPhiS(m_param->tSipa.nLimitType, m_PLower->tSigrLower.nMaterialType);

	// δlsd＝ξ1・Φs・δls
	double dDeltaLsd_dx = dXi1 * dPhiS * (dK * dDelta_ls[eVector2d::kDx]);
	double dDeltaLsd_dy = dXi1 * dPhiS * (dK * dDelta_ls[eVector2d::kDy]);

	ParamAngleOption angleP;
	angleP.kThis = m_PLower->kThis;
	angleP.kElem = m_PLower->tCOLM.aInvestigatedElem[0];
	angleP.nDirDisp = m_param->tSimd.nDirDisp;
	angleP.nShearDir = m_PLower->tCOLM.nDirShear;
	angleP.dUserAngle = m_param->tSimd.dAngle;
	angleP.nSeismicForceDirection = m_param->tSipa.nSeismicForceDirection;

	double dCompVal = 0;//합성벡터 크기
	GetDispOfAngle(angleP, dDeltaLsd_dx, dDeltaLsd_dy, dCompVal);

	aDeltaA[seis_jp::kDlsd_AnalH29] = dCompVal;
	return TRUE;
}

BOOL CSeismicDesignSimd_H29_Lower::CalcDeltaA_fromAnalysis_forSteel(OUT doubleArray& aDeltaA)
{
	// δls=δra-δfd-Θfp×h-δs
	// δlsd＝ξ1・Φs・δls
	return CalcDeltaA_fromAnalysis_forRc(aDeltaA);
}

BOOL CSeismicDesignSimd_H29_Lower::CalcDeltaA_fromkMphi_Dy(OUT doubleArray& aDeltaA)
{
	BOOL bResult = FALSE;

	if (m_PLower->tSigrLower.nMaterialType == T_SIGR_LOWER_D::kRC)
		bResult = CalcDeltaA_fromkMphi_Dy_forRc(aDeltaA);
	else if (m_PLower->tSigrLower.nMaterialType == T_SIGR_LOWER_D::kSteel)
		bResult = CalcDeltaA_fromkMphi_Dy_forSteel(aDeltaA);

	if (bResult == FALSE) return FALSE;

	// 면진교를 적용한 경우, 수평변위 한계치는 한계상태 2, 3과 관계없이 δls2d/αm 을 한계치로 사용합니다. αm=2.0
	T_SIGR_LOWER_BEAR_D::eBearingType bearType = m_pDoc->m_pAttrCtrl2->GetQSigrLower()->GetBearingType(m_param->tSimd.kSigr);
	if (bearType == T_SIGR_LOWER_BEAR_D::kSeismicIsolation) {
		const double dDelta_m = 2.0;
		double dVal = dgn::SafeDiv(aDeltaA[seis_jp::kDlsd_MPhiH29], dDelta_m);

		doubleArray aDelta_yEd;
		// 단, δls2d/αm 가 δyEd 이하가 되는 경우는 δlsd=δyEd 로 한다.
		if (GetDelta_yEd_fromMphi(aDelta_yEd) == TRUE && dVal < aDelta_yEd[seis_jp::kDlsd_MPhiH29]) {
			aDeltaA[seis_jp::kDlsd_MPhiH29] = aDelta_yEd[seis_jp::kDlsd_MPhiH29];
		}
		else {
			aDeltaA[seis_jp::kDlsd_MPhiH29] = dVal;
		}
	}

	return bResult;
}

BOOL CSeismicDesignSimd_H29_Lower::CalcDeltaA_fromkMphi_Dy_forRc(OUT doubleArray& aDeltaA)
{
	const int nSeisForceDir = m_param->tSipa.nSeismicForceDirection;
	const int nDirShear = m_PLower->tLowColmData.Column.nDirShear;

	const int nLocalIdx = [nSeisForceDir, nDirShear]()
	{
		int nShearForceDir = (nSeisForceDir == T_SIPA_D::kAxis_Dir) ? nDirShear : (nDirShear == seis_jp::kY_Dir ? seis_jp::kZ_Dir : seis_jp::kY_Dir);
		switch (nShearForceDir)
		{
		case seis_jp::kY_Dir: return LOCAL_AXIS_Y;
		case seis_jp::kZ_Dir: return LOCAL_AXIS_Z;
		default: ASSERT(0); return 0;
		}
	}();

	const int nCompIdx = [nSeisForceDir, nDirShear]()
	{
		int nShearForceDir = (nSeisForceDir == T_SIPA_D::kAxis_Dir) ? nDirShear : (nDirShear == seis_jp::kY_Dir ? seis_jp::kZ_Dir : seis_jp::kY_Dir);
		switch (nShearForceDir)
		{
		case seis_jp::kY_Dir: return QIehp::kMz;
		case seis_jp::kZ_Dir: return QIehp::kMy;
		default: ASSERT(0); return QIehp::kUnknown;
		}
	}();

	auto L_CalcDelta_lsd = [&](OUT doubleArray& aDeltaA) {
		QSifaH29* pSifa = m_pDoc->m_pAttrCtrl2->GetQSifaH29();

		double dK = pSifa->GetFactorK(m_PLower->tSigrLower.nMaterialType);
		double dXi1 = pSifa->GetXi1(m_param->tSipa.nLimitType, m_PLower->tSigrLower.nMaterialType);
		double dPhiS = pSifa->GetPhiS(m_param->tSipa.nLimitType, m_PLower->tSigrLower.nMaterialType);
		double dPhi_RY = pSifa->GetPhiS(m_param->tSipa.nLimitType, m_PLower->tSigrLower.nMaterialType);

		double d_h = 0;
		//검토 대상을 찾아 관성력 작용위치까지의 높이와 제일 아래에 있는 요소를 찾아온다.
		if (GetH(m_PLower->kPierTopNode, m_PLower->tLowColmData.kNodeLow, d_h) == FALSE) return FALSE;

		PLp lpP = std::make_tuple(T_SIDP_LSLP_D::kLp_9point5_Ellipsis, m_param->tSipa.nExamTarget, nLocalIdx, d_h, &m_PLower->tLowColmData);

		double dLp = 0;
		if (GetLp(lpP, dLp) == FALSE) return FALSE;

		double dPhiY = 0.0, dPhiLs2 = 0.0;
		CalcPhiY_U(m_PLower->tLowColmData.kElem, nCompIdx, (seis_jp::eDirAnal)m_PLower->pSpld->nDirAnal, m_PLower->tLowColmData.ePosNodeLow, dPhiY, dPhiLs2);

		//δyE
		//δy0 : 교각 기부의 비선형특성(M-φ관계)로부터 상태 단계별 수평변위를 가정하여, 요소별 적분의 누적에 의해 상부공 관성력 작용위치의 변위 산출
		PDeltaYandU PDy = std::make_tuple(	m_param->tSimd.kSipa, 
											m_param->tSimd.kSigr, 
											m_PLower->kPierTopNode, 
											m_PLower->pSpld->strNameCOLM, 
											(seis_jp::eDirAnal)m_PLower->pSpld->nDirAnal,
											m_param->tSipa.nSeismicForceDirection, 
											&m_PLower->tLowColmData);

		// δyE = (Mls2 / My0) * δy0
		double dDelta_yE = 0.0, dDeltaU = 0.0;
		if (m_DispL.GetDeltaYandU(PDy, dDelta_yE, dDeltaU) == FALSE) return FALSE;

		ParamAngleOption param;
		param.kThis = m_PLower->kThis;
		param.kElem = m_PLower->tCOLM.aInvestigatedElem[0];
		param.nDirDisp = m_param->tSimd.nDirDisp;
		param.nShearDir = m_PLower->tCOLM.nDirShear;
		param.dUserAngle = m_param->tSimd.dAngle;
		param.nSeismicForceDirection = m_param->tSipa.nSeismicForceDirection;

		// δls = k*(δyE+(φls-φy)Lp(h-Lp/2))
		//먼저 로컬축 기준으로 k*(δyE+(φls-φy)Lp(h-Lp/2)) 이것 부터 계산
		double dDeltaLs = dK * (dDelta_yE + (dPhiLs2 - dPhiY) * dLp * (d_h - dLp / 2));

		// δlsd = ξ1*φs*δls
		double dDeltaLsd = dXi1 * dPhiS * dDeltaLs;

		aDeltaA[seis_jp::kDls_MPhiH29] = dDeltaLs;
		aDeltaA[seis_jp::kDlsd_MPhiH29] = dDeltaLsd;

		return TRUE;
	};


	BOOL bResult = FALSE;
	switch (m_param->tSipa.nLimitType)
	{
	case T_SIPA_D::kLimitState1:
		bResult = GetDelta_yEd_fromMphi(aDeltaA);
		break;

	case T_SIPA_D::kLimitState2:
	case T_SIPA_D::kLimitState3:
		// δlsd : 교각의 한계 상태에 상당하는 수평 변위
		bResult = L_CalcDelta_lsd(aDeltaA);
		break;
	default:
		ASSERT(FALSE);
		bResult = FALSE;
		break;
	}

	return bResult;
}

BOOL CSeismicDesignSimd_H29_Lower::CalcDeltaA_fromkMphi_Dy_forSteel(OUT doubleArray& aDeltaA)
{
	if (m_param->tSipa.nLimitType != T_SIPA_D::kLimitState2 && m_param->tSipa.nLimitType != T_SIPA_D::kLimitState3)
		return FALSE;

	QSifaH29* pSifa = m_pDoc->m_pAttrCtrl2->GetQSifaH29();

	double dK = pSifa->GetFactorK(m_PLower->tSigrLower.nMaterialType);

	PDeltaYandU PDy = std::make_tuple(	m_param->kSipa, 
										m_param->tSimd.kSigr, 
										m_PLower->kPierTopNode, 
										m_PLower->pSpld->strNameCOLM, 
										(seis_jp::eDirAnal)m_PLower->pSpld->nDirAnal,
										m_param->tSipa.nSeismicForceDirection,
										&m_PLower->tLowColmData);

	double dDeltaY = 0.0, dDeltaU = 0.0;
	if (m_DispL.GetDeltaYandU(PDy, dDeltaY, dDeltaU) == FALSE) return FALSE;

	// δls = k*δa
	// δa : 강재 교각의 휨모멘트-곡률 관계로부터 강재교각의 수평력이 최대가 될 때의 수평변위
	double dDeltaLs = dK * dDeltaU;

	aDeltaA[seis_jp::kDls_MPhiH29] = abs(dDeltaLs);
	aDeltaA[seis_jp::kDlsd_MPhiH29] = abs(dDeltaLs);
	return TRUE;
}

BOOL CSeismicDesignSimd_H29_Lower::GetDelta_yEd_fromMphi(OUT doubleArray& aDeltaA)
{
	//δy0 : 교각 기부의 비선형특성(M-φ관계)로부터 상태 단계별 수평변위를 가정하여, 요소별 적분의 누적에 의해 상부공 관성력 작용위치의 변위 산출
	PDeltaYandU PDy = std::make_tuple(	m_param->kSipa, 
										m_param->kSigr, 
										m_PLower->kPierTopNode, 
										m_PLower->pSpld->strNameCOLM, 
										(seis_jp::eDirAnal)m_PLower->pSpld->nDirAnal,
										m_param->tSipa.nSeismicForceDirection, 
										&m_PLower->tLowColmData);

	// δyE = (Mls2 / My0) * δy0
	double dDelta_yE = 0.0, dDeltaU = 0.0;
	if (m_DispL.GetDeltaYandU(PDy, dDelta_yE, dDeltaU) == FALSE) return FALSE;

	QSifaH29* pSifa = m_pDoc->m_pAttrCtrl2->GetQSifaH29();

	double dXi1 = pSifa->GetXi1(m_param->tSipa.nLimitType, m_PLower->tSigrLower.nMaterialType);
	double dPhi_RY = pSifa->GetPhiS(m_param->tSipa.nLimitType, m_PLower->tSigrLower.nMaterialType);

	// δyEd = ξ1・φRY・δyE
	double dDelta_yEd = dXi1 * dPhi_RY * dDelta_yE;
	aDeltaA[seis_jp::kDyE_MPhiH29] = dDelta_yEd;
	return TRUE;
}

BOOL CSeismicDesignSimd_H29_Lower::CalcDeltaA_fromkMphi_Dpy(OUT doubleArray& aDeltaA)
{
	return FALSE;
}

#pragma endregion