#include "stdafx.h"
#include "SeismicDesignSimd_H24.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_PostCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl2.h"
#include "..\wg_db\DB_QUERY_SEIS_CVL_JP.h"
#include "..\wg_db\BrdgSeisEvalResult_JP.h"

#include "BrdgSeisEval_JP.h"

#pragma region // CSeismicDesignSimd_H24

void CSeismicDesignSimd_H24::AddProgressStep(CDgnProgressDlg* pProgress)
{
	CSeismicDesignSimd::AddStep(pProgress);
}

BOOL CSeismicDesignSimd_H24::Calculate(CDgnProgressDlg* pProgress)
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

void CSeismicDesignSimd_H24::CalculateUpper(CDgnProgressDlg* pProgress, const T_SIMD_K& kSimd)
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

	CSeismicDesignSimd_H24_Upper SimdLeft(pProgress);
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

	CSeismicDesignSimd_H24_Upper SimdRight(pProgress);
	SimdRight.Calculate(kSimd, &evalRight);

	// save result
	CBrdgSeisEvalResult_JP* pResult = pDoc->m_pPostCtrl->GetBrdgSeisEvalResult_JP(); if (!pResult) { ASSERT(0); }
	pResult->SetResultSimd(dSimd.kSipa, evalLeft.kSigr, evalLeft.nPosType, evalLeft);
	pResult->SetResultSimd(dSimd.kSipa, evalRight.kSigr, evalRight.nPosType, evalRight);
}

void CSeismicDesignSimd_H24::CalculateLower(CDgnProgressDlg* pProgress, const T_SIMD_K& kSimd)
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

	CSeismicDesignSimd_H24_Lower Simd(pProgress);
	Simd.Calculate(kSimd, &eval);

	// save result
	CBrdgSeisEvalResult_JP* pResult = pDoc->m_pPostCtrl->GetBrdgSeisEvalResult_JP(); if (!pResult) { ASSERT(0); }
	pResult->SetResultSimd(dSimd.kSipa, eval.kSigr, eval.nPosType, eval);
}
#pragma endregion

#pragma region // CSeismicDesignSimd_H24_Upper

CSeismicDesignSimd_H24_Upper::CSeismicDesignSimd_H24_Upper(CDgnProgressDlg* pProgress) : CSeismicDesignSimd()
{
	m_pProgress = pProgress;
}

BOOL CSeismicDesignSimd_H24_Upper::Calculate(const T_SIMD_K& kSimd, T_SIMD_EVAL* pEval)
{
	return CSeismicDesignSimd::Calculate(kSimd, pEval);
}

BOOL CSeismicDesignSimd_H24_Upper::CalcDeltaA(const T_THIS_K& kThis, IN T_SIMD_SPLD_D* pSpld, OUT doubleArray& aDeltaA)
{
	P_SIMD_Upper input(m_param, kThis, pSpld);
	if (input.IsValidValue() == FALSE) return FALSE;
	m_PUpper = &input;

	//직접입력의 경우
	if (m_param->tSimd.nAlwCalcType == seis_jp::kUserInput) {
		double dDelta_a = (pSpld->nDirAnal == seis_jp::kMinus) ? -1 * pSpld->dLengthDeltaVar : pSpld->dLengthDeltaVar;
		aDeltaA[seis_jp::kDa_UserH24] = dDelta_a;
		return TRUE;
	}

	//기초변위 영향분에 대해 안전계수 고려 할 경우
	if (m_param->tSimd.bConsiderAlpha == TRUE)
		return CalcDeltaA_ConsiderAlpha(kThis, pSpld, aDeltaA);

	BOOL bResult = FALSE;

	if (m_PUpper->tSigrLower.nMaterialType == T_SIGR_LOWER_D::kRC)
		bResult = CalcDeltaA_forRc(kThis, pSpld, aDeltaA);
	else if (m_PUpper->tSigrLower.nMaterialType == T_SIGR_LOWER_D::kSteel)
		bResult = CalcDeltaA_forSteel(kThis, pSpld, aDeltaA);

	return bResult;
}

BOOL CSeismicDesignSimd_H24_Upper::CalcDeltaA_forRc(const T_THIS_K& kThis, IN T_SIMD_SPLD_D* pSpld, OUT doubleArray& aDeltaA)
{
	T_NODE_K kNode;
	seis_jp::eDirAnal eDir = (seis_jp::eDirAnal)pSpld->nDirAnal;
	if (m_pDoc->m_pAttrCtrl2->GetQSimd()->GetInertialForceNode(m_param->kSimd, eDir, kNode) == FALSE) return FALSE;

	//기둥 그룹에서 기둥의 최하단 절점을 찾는다.
	ColumnData colm;
	if (m_pDoc->m_pAttrCtrl2->GetQSigrLower()->FindColumnLowNode(pSpld->kSigrLower, pSpld->strNameCOLM, colm) == FALSE) return FALSE;

	double aDelta_rls[eVector2d::kMax2d] = { 0.0, 0.0 };
	if (GetDispFromTimeHistory(pSpld->kThis, kNode, pSpld->nStepDeltaVar, aDelta_rls[eVector2d::kDx], aDelta_rls[eVector2d::kDy]) == FALSE) return FALSE;

	PDeltaVal deltaParam = std::make_tuple(	pSpld->kSigrLower, 
											pSpld->kThis, 
											m_PUpper->kPierTopNode, 
											(seis_jp::eDirAnal)pSpld->nDirAnal,
											m_param->tSimd.nRspCalcType, 
											&m_PUpper->tLowColmData);

	double dDelta_fls_Dx = 0, dDelta_fls_Dy = 0;
	if (GetF_ls(deltaParam, pSpld->nStepDeltaVar, dDelta_fls_Dx, dDelta_fls_Dy) == FALSE) return FALSE;

	//δa=(δrls-δfls)/α＋δfls
	double aDelta_a[eVector2d::kMax2d] = { 0.0, 0.0 };
	aDelta_a[eVector2d::kDx] = dgn::SafeDiv((aDelta_rls[eVector2d::kDx] - dDelta_fls_Dx), m_param->tSimd.dAlpha) + dDelta_fls_Dx;
	aDelta_a[eVector2d::kDy] = dgn::SafeDiv((aDelta_rls[eVector2d::kDy] - dDelta_fls_Dy), m_param->tSimd.dAlpha) + dDelta_fls_Dy;

	ParamAngleOption param;
	param.kThis = kThis;
	param.kElem = m_PUpper->tSigrUpper.aElements[0];
	param.nDirDisp = m_param->tSimd.nDirDisp;
	param.nShearDir = m_PUpper->tSigrUpper.nDirShear;
	param.dUserAngle = m_param->tSimd.dAngle;
	param.nSeismicForceDirection = m_param->tSipa.nSeismicForceDirection;

	double dDelta_rls = 0, dDelta_a = 0;
	GetDispOfAngle(param, aDelta_rls[eVector2d::kDx], aDelta_rls[eVector2d::kDy], dDelta_rls);
	GetDispOfAngle(param, aDelta_a[eVector2d::kDx], aDelta_a[eVector2d::kDy], dDelta_a);

	aDeltaA[seis_jp::kDrls_AnalH24] = dDelta_rls;
	aDeltaA[seis_jp::kDa_AnalH24] = dDelta_a;

	return TRUE;
}

BOOL CSeismicDesignSimd_H24_Upper::CalcDeltaA_forSteel(const T_THIS_K& kThis, IN T_SIMD_SPLD_D* pSpld, OUT doubleArray& aDeltaA)
{
	T_NODE_K kNode;
	seis_jp::eDirAnal eDir = (seis_jp::eDirAnal)pSpld->nDirAnal;
	if (m_pDoc->m_pAttrCtrl2->GetQSimd()->GetInertialForceNode(m_param->kSimd, eDir, kNode) == FALSE) return FALSE;

	double aDelta_ra[eVector2d::kMax2d] = { 0.0, 0.0 };
	if (GetDispFromTimeHistory(kThis, kNode, pSpld->nStepDeltaVar, aDelta_ra[eVector2d::kDx], aDelta_ra[eVector2d::kDy]) == FALSE) return FALSE;

	// δa＝δra
	ParamAngleOption param;
	param.kThis = kThis;
	param.kElem = m_PUpper->tSigrUpper.aElements[0];
	param.nDirDisp = m_param->tSimd.nDirDisp;
	param.nShearDir = m_PUpper->tSigrUpper.nDirShear;
	param.dUserAngle = m_param->tSimd.dAngle;
	param.nSeismicForceDirection = m_param->tSipa.nSeismicForceDirection;

	double dDelta_ra = 0;
	GetDispOfAngle(param, aDelta_ra[eVector2d::kDx], aDelta_ra[eVector2d::kDy], dDelta_ra);

	aDeltaA[seis_jp::kDrls_AnalH24] = dDelta_ra;
	aDeltaA[seis_jp::kDa_AnalH24] = dDelta_ra;

	return TRUE;
}

BOOL CSeismicDesignSimd_H24_Upper::CalcDeltaA_ConsiderAlpha(const T_THIS_K& kThis, IN T_SIMD_SPLD_D* pSpld, OUT doubleArray& aDeltaA)
{
	T_NODE_K kNode;
	seis_jp::eDirAnal eDir = (seis_jp::eDirAnal)pSpld->nDirAnal;
	if (m_pDoc->m_pAttrCtrl2->GetQSimd()->GetInertialForceNode(m_param->kSimd, eDir, kNode) == FALSE) return FALSE;

	//δls：소성 힌지 중, 처음으로 한계상태에 다달았을 시의 변위
	double dDeltaLs_Dx = 0, dDeltaLs_Dy = 0;
	if (GetDispFromTimeHistory(pSpld->kThis, kNode, pSpld->nStepDeltaVar, dDeltaLs_Dx, dDeltaLs_Dy) == FALSE) return FALSE;

	//δa=δls/α
	double dDeltaA_Dx = dgn::SafeDiv(dDeltaLs_Dx, m_param->tSimd.dAlpha);
	double dDeltaA_Dy = dgn::SafeDiv(dDeltaLs_Dy, m_param->tSimd.dAlpha);

	ParamAngleOption param;
	param.kThis = kThis;
	param.kElem = m_PUpper->tSigrUpper.aElements[0];
	param.nDirDisp = m_param->tSimd.nDirDisp;
	param.nShearDir = m_PUpper->tSigrUpper.nDirShear;
	param.dUserAngle = m_param->tSimd.dAngle;
	param.nSeismicForceDirection = m_param->tSipa.nSeismicForceDirection;

	double dDelta_ls = 0, dDelta_a = 0;
	GetDispOfAngle(param, dDeltaLs_Dx, dDeltaLs_Dy, dDelta_ls);
	GetDispOfAngle(param, dDeltaA_Dx, dDeltaA_Dy, dDelta_a);

	aDeltaA[seis_jp::kD_f_ls_AnalH24] = dDelta_ls;
	aDeltaA[seis_jp::kDa_AnalH24] = dDelta_a;

	return TRUE;
}

#pragma endregion

#pragma region // CSeismicDesignSimd_H24_Lower

CSeismicDesignSimd_H24_Lower::CSeismicDesignSimd_H24_Lower(CDgnProgressDlg* pProgress) : CSeismicDesignSimd()
{
	m_pProgress = pProgress;
	m_DispL.SetProgressDlg(pProgress);
}

BOOL CSeismicDesignSimd_H24_Lower::Calculate(const T_SIMD_K& kSimd, T_SIMD_EVAL* pEval)
{
	return CSeismicDesignSimd::Calculate(kSimd, pEval);
}

BOOL CSeismicDesignSimd_H24_Lower::CalcDeltaA(const T_THIS_K& kThis, IN T_SIMD_SPLD_D* pSpld, OUT doubleArray& aDeltaA)
{
	P_SIMD_Lower input(m_param, kThis, pSpld);
	if (input.IsValidValue() == FALSE) return FALSE;
	m_PLower = &input;

	BOOL bResult = FALSE;

	switch (m_param->tSimd.nAlwCalcType)
	{
	case seis_jp::kUserInput: {
		double dDelta_a = (pSpld->nDirAnal == seis_jp::kMinus) ? -1 * pSpld->dLengthDeltaVar : pSpld->dLengthDeltaVar;
		aDeltaA[seis_jp::kDa_UserH24] = dDelta_a;
		bResult = TRUE;	}
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

BOOL CSeismicDesignSimd_H24_Lower::CalcDeltaA_fromAnalysis(OUT doubleArray& aDeltaA)
{
	BOOL bResult = FALSE;

	if (m_PLower->tSigrLower.nMaterialType == T_SIGR_LOWER_D::kRC)
		bResult = CalcDeltaA_fromAnalysis_forRc(aDeltaA);
	else if (m_PLower->tSigrLower.nMaterialType == T_SIGR_LOWER_D::kSteel)
		bResult = CalcDeltaA_fromAnalysis_forSteel(aDeltaA);

	return bResult;
}

BOOL CSeismicDesignSimd_H24_Lower::CalcDeltaA_fromAnalysis_forRc(OUT doubleArray& aDeltaA)
{
	PDeltaVal deltaParam = std::make_tuple(	m_param->tSimd.kSigr, 
											m_PLower->pSpld->kThis,
											m_PLower->kPierTopNode, 
											(seis_jp::eDirAnal)m_PLower->pSpld->nDirAnal,
											m_param->tSimd.nRspCalcType,
											&m_PLower->tLowColmData);

	// δls=δrls-δfd-Θfp×h-δs
	double dDelta_ls[eVector2d::kMax2d] = { 0.0, 0.0 };
	if (m_DispL.CalcDeltaValAtStep(deltaParam, m_PLower->pSpld->nStepDeltaVar, dDelta_ls[eVector2d::kDx], dDelta_ls[eVector2d::kDy]) == FALSE) return FALSE;

	// δa＝δls/α
	double dDeltaA_dx = dgn::SafeDiv(dDelta_ls[eVector2d::kDx], m_param->tSimd.dAlpha);
	double dDeltaA_dy = dgn::SafeDiv(dDelta_ls[eVector2d::kDy], m_param->tSimd.dAlpha);

	ParamAngleOption angleP;
	angleP.kThis = m_PLower->kThis;
	angleP.kElem = m_PLower->tCOLM.aInvestigatedElem[0];
	angleP.nDirDisp = m_param->tSimd.nDirDisp;
	angleP.nShearDir = m_PLower->tCOLM.nDirShear;
	angleP.dUserAngle = m_param->tSimd.dAngle;
	angleP.nSeismicForceDirection = m_param->tSipa.nSeismicForceDirection;

	double dComp_Dls = 0, dComp_Da = 0;//합성벡터 크기
	GetDispOfAngle(angleP, dDelta_ls[eVector2d::kDx], dDelta_ls[eVector2d::kDy], dComp_Dls);
	GetDispOfAngle(angleP, dDeltaA_dx, dDeltaA_dy, dComp_Da);

	aDeltaA[seis_jp::kD_f_ls_AnalH24] = dComp_Dls;
	aDeltaA[seis_jp::kDa_AnalH24] = dComp_Da;
	return TRUE;
}

BOOL CSeismicDesignSimd_H24_Lower::CalcDeltaA_fromAnalysis_forSteel(OUT doubleArray& aDeltaA)
{
	PDeltaVal deltaParam = std::make_tuple(	m_param->tSimd.kSigr, 
											m_PLower->pSpld->kThis,
											m_PLower->kPierTopNode, 
											(seis_jp::eDirAnal)m_PLower->pSpld->nDirAnal,
											m_param->tSimd.nRspCalcType, 
											&m_PLower->tLowColmData);

	// δa=δra-δfd-Θfp×h-δs
	double dDelta_a[eVector2d::kMax2d] = { 0.0, 0.0 };
	if (m_DispL.CalcDeltaValAtStep(deltaParam, m_PLower->pSpld->nStepDeltaVar, dDelta_a[eVector2d::kDx], dDelta_a[eVector2d::kDy]) == FALSE) return FALSE;

	ParamAngleOption angleP;
	angleP.kThis = m_PLower->kThis;
	angleP.kElem = m_PLower->tCOLM.aInvestigatedElem[0];
	angleP.nDirDisp = m_param->tSimd.nDirDisp;
	angleP.nShearDir = m_PLower->tCOLM.nDirShear;
	angleP.dUserAngle = m_param->tSimd.dAngle;
	angleP.nSeismicForceDirection = m_param->tSipa.nSeismicForceDirection;

	double dComp_Da = 0;//합성벡터 크기
	GetDispOfAngle(angleP, dDelta_a[eVector2d::kDx], dDelta_a[eVector2d::kDy], dComp_Da);

	aDeltaA[seis_jp::kD_f_ls_AnalH24] = dComp_Da;
	aDeltaA[seis_jp::kDa_AnalH24] = dComp_Da;
	return TRUE;
}

BOOL CSeismicDesignSimd_H24_Lower::CalcDeltaA_fromkMphi_Dy(OUT doubleArray& aDeltaA)
{
	BOOL bResult = FALSE;

	if (m_PLower->tSigrLower.nMaterialType == T_SIGR_LOWER_D::kRC)
		bResult = CalcDeltaA_fromkMphi_Dy_forRc(aDeltaA);
	else if (m_PLower->tSigrLower.nMaterialType == T_SIGR_LOWER_D::kSteel)
		bResult = CalcDeltaA_fromkMphi_Dy_forSteel(aDeltaA);

	return bResult;
}

BOOL CSeismicDesignSimd_H24_Lower::CalcDeltaA_fromkMphi_Dy_forRc(OUT doubleArray& aDeltaA)
{
	//기초변위 영향분에 대해 안전계수 고려 할 경우
	if (m_param->tSimd.bConsiderAlpha == TRUE)
		return CalcDeltaA_fromkMphi_Dy_forSteel(aDeltaA);

	const int nDirShear = m_PLower->tLowColmData.Column.nDirShear;

	if (nDirShear != seis_jp::eDirShear::kYZ_Dir) {
		return CalcDeltaA_fromkMphi_Dy_forRc_ByShearDir(nDirShear, aDeltaA);
	}
	
	//2축 고려시
	doubleArray aDeltaA_Y; aDeltaA_Y.SetSize(aDeltaA.GetSize());
	doubleArray aDeltaA_Z; aDeltaA_Z.SetSize(aDeltaA.GetSize());

	if (CalcDeltaA_fromkMphi_Dy_forRc_ByShearDir(seis_jp::kY_Dir, aDeltaA_Y) == FALSE) return FALSE;
	if (CalcDeltaA_fromkMphi_Dy_forRc_ByShearDir(seis_jp::kZ_Dir, aDeltaA_Z) == FALSE) return FALSE;

	//합성벡터 크기
	ParamAngleOption angleP;
	angleP.kThis = m_PLower->kThis;
	angleP.kElem = m_PLower->tCOLM.aInvestigatedElem[0];
	angleP.nDirDisp = m_param->tSimd.nDirDisp;
	angleP.nShearDir = m_PLower->tCOLM.nDirShear;
	angleP.dUserAngle = m_param->tSimd.dAngle;
	angleP.nSeismicForceDirection = m_param->tSipa.nSeismicForceDirection;

	GetDispOfAngle(angleP, aDeltaA_Y[seis_jp::kDy_MPhiH24], aDeltaA_Z[seis_jp::kDy_MPhiH24], aDeltaA[seis_jp::kDy_MPhiH24]);
	GetDispOfAngle(angleP, aDeltaA_Y[seis_jp::kDls_MPhiH24], aDeltaA_Z[seis_jp::kDls_MPhiH24], aDeltaA[seis_jp::kDls_MPhiH24]);
	GetDispOfAngle(angleP, aDeltaA_Y[seis_jp::kDa_MPhiH24], aDeltaA_Z[seis_jp::kDa_MPhiH24], aDeltaA[seis_jp::kDa_MPhiH24]);

	return TRUE;
}

BOOL CSeismicDesignSimd_H24_Lower::CalcDeltaA_fromkMphi_Dy_forRc_ByShearDir(const int& nDirShear, OUT doubleArray& aDeltaA)
{
	const int nSeisForceDir = m_param->tSipa.nSeismicForceDirection;

	//교각 기부의 비선형특성(M-φ관계)로부터 상태 단계별 수평변위를 가정하여, 요소별 적분의 누적에 의해 상부공 관성력 작용위치의 변위 산출
	PDeltaYandU PDy = std::make_tuple(m_param->tSimd.kSipa,
		m_param->tSimd.kSigr,
		m_PLower->kPierTopNode,
		m_PLower->pSpld->strNameCOLM,
		(seis_jp::eDirAnal)m_PLower->pSpld->nDirAnal,
		m_param->tSipa.nSeismicForceDirection,
		&m_PLower->tLowColmData);

	// δy
	double dDeltaY = 0.0, dDelta_U0 = 0.0;
	if (m_DispL.GetDeltaYandU(PDy, dDeltaY, dDelta_U0) == FALSE) return FALSE;

	// h : 검토 대상을 찾아 관성력 작용위치까지의 높이와 제일 아래에 있는 요소를 찾아온다.
	double d_h = 0;
	if (GetH(m_PLower->kPierTopNode, m_PLower->tLowColmData.kNodeLow, d_h) == FALSE) return FALSE;

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

	// Lp=9.5σsy1/6βn-1/3Φ’
	PLp lpP = std::make_tuple(T_SIDP_LSLP_D::kLp_9point5_Ellipsis,
							  m_param->tSipa.nExamTarget,
							  nLocalIdx,
							  d_h,
							  &m_PLower->tLowColmData);

	double dLp = 0;
	if (GetLp(lpP, dLp) == FALSE) return FALSE;

	// φls, φy：M-φ관계에 의한 계산
	double dPhiY = 0.0, dPhiLs = 0.0;
	CalcPhiY_U(m_PLower->tLowColmData.kElem, nCompIdx, (seis_jp::eDirAnal)m_PLower->pSpld->nDirAnal, m_PLower->tLowColmData.ePosNodeLow, dPhiY, dPhiLs);

	// δls : 교각의 한계 상태에 상당하는 수평 변위
	// δls=δy+(Φls-φy)Lp(h-Lp/2)
	double dDeltaLs = dDeltaY + (dPhiLs - dPhiY) * dLp * (d_h - dLp / 2);

	// δa＝δls/α
	double dDeltaA = dgn::SafeDiv(dDeltaLs, m_param->tSimd.dAlpha);

	aDeltaA[seis_jp::kDy_MPhiH24] = dDeltaY;
	aDeltaA[seis_jp::kDls_MPhiH24] = dDeltaLs;
	aDeltaA[seis_jp::kDa_MPhiH24] = dDeltaA;
	return TRUE;
}

BOOL CSeismicDesignSimd_H24_Lower::CalcDeltaA_fromkMphi_Dy_forSteel(OUT doubleArray& aDeltaA)
{
	double d_h = 0;
	//검토 대상을 찾아 관성력 작용위치까지의 높이와 제일 아래에 있는 요소를 찾아온다.
	if (GetH(m_PLower->kPierTopNode, m_PLower->tLowColmData.kNodeLow, d_h) == FALSE) return FALSE;

	//교각 기부의 비선형특성(M-φ관계)로부터 상태 단계별 수평변위를 가정하여, 요소별 적분의 누적에 의해 상부공 관성력 작용위치의 변위 산출
	PDeltaYandU PDy = std::make_tuple(	m_param->tSimd.kSipa, 
										m_param->tSimd.kSigr, 
										m_PLower->kPierTopNode, 
										m_PLower->pSpld->strNameCOLM,
										(seis_jp::eDirAnal)m_PLower->pSpld->nDirAnal,
										m_param->tSipa.nSeismicForceDirection, 
										&m_PLower->tLowColmData);

	double dDeltaY = 0.0, dDeltaU = 0.0;
	if (m_DispL.GetDeltaYandU(PDy, dDeltaY, dDeltaU) == FALSE) return FALSE;

	// (강제교각) δa：柱基部가'φa'에 도달할 때의 곡률분포를 적분해서 구한 값
	aDeltaA[seis_jp::kDa_AnalH24] = dDeltaU;
	return TRUE;
}

BOOL CSeismicDesignSimd_H24_Lower::CalcDeltaA_fromkMphi_Dpy(OUT doubleArray& aDeltaA)
{
	return FALSE;
}
#pragma endregion