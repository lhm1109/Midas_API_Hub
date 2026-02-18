#include "stdafx.h"
#include "SeismicDesignSimd.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl2.h"
#include "..\wg_db\BrdgSeisEvalResult_JP.h"

#include "DgnProgressDlg.h"

P_SIMD::P_SIMD(const T_SIMD_K& simdKey)
{
	bValidValue = FALSE;
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	if (pDoc == nullptr) { ASSERT(pDoc); return; }

	if (pDoc->m_pAttrCtrl2->GetQSimd()->Get(simdKey, tSimd) == TRUE) { kSimd = simdKey; }
	else { return; };
	if (pDoc->m_pAttrCtrl2->GetQSipa()->Get(tSimd.kSipa, tSipa) == TRUE) { kSipa = tSimd.kSipa; }
	else { return; };
	if (pDoc->m_pAttrCtrl2->GetQSigr()->Get(tSimd.kSigr, tSigr) == TRUE) { kSigr = tSimd.kSigr; }
	else { return; };

	bValidValue = TRUE;
}

P_SIMD_Lower::P_SIMD_Lower(IN P_SIMD* pPSimd, const T_THIS_K& kTHIS, IN T_SIMD_SPLD_D* pSPLD)
{
	bValidValue = FALSE;
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	if (pDoc == nullptr) { ASSERT(pDoc); return; }
	if (pPSimd == nullptr) { return; }
	if (kTHIS == 0) { return; }
	if (pSPLD == nullptr) { return; }

	kThis = kTHIS;
	pSpld = pSPLD;

	if (pDoc->m_pAttrCtrl2->GetQSigrLower()->Get(pPSimd->tSimd.kSigr, tSigrLower) == TRUE) { kSigrLower = pPSimd->tSimd.kSigr; }
	else { return; };

	kPierTopNode = (pPSimd->tSipa.nSeismicForceDirection == T_SIPA_D::kAxis_Dir) ? tSigrLower.kNodeAxis : tSigrLower.kNodePerp;

	if (pPSimd->tSimd.nAlwCalcType != seis_jp::kUserInput) {
		if (pDoc->m_pAttrCtrl2->GetQSigrLower()->FindColumn(kSigrLower, pSpld->strNameCOLM, tCOLM) == FALSE) return;
		if (tCOLM.aInvestigatedElem.GetCount() < 1) return;

		if (pDoc->m_pAttrCtrl2->GetQSigrLower()->FindColumnLowNode(kSigrLower, pSpld->strNameCOLM, tLowColmData) == FALSE) return;
		if (tLowColmData.Column.aInvestigatedElem.GetCount() < 1) return;
	}

	bValidValue = TRUE;
}

P_SIMD_Upper::P_SIMD_Upper(IN P_SIMD* pPSimd, const T_THIS_K& kTHIS, IN T_SIMD_SPLD_D* pSPLD)
{
	bValidValue = FALSE;
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	if (pDoc == nullptr) { ASSERT(pDoc); return; }
	if (pPSimd == nullptr) { ASSERT(pDoc); return; }
	if (kTHIS == 0) { ASSERT(pDoc); return; }
	if (pSPLD == nullptr) { ASSERT(pDoc); return; }

	kThis = kTHIS;
	pSpld = pSPLD;

	if (pDoc->m_pAttrCtrl2->GetQSigrUpper()->Get(pPSimd->tSimd.kSigr, tSigrUpper) == TRUE) { kSigrUpper = pPSimd->tSimd.kSigr; }
	else { return; };

	if (pDoc->m_pAttrCtrl2->GetQSigrLower()->Get(pSPLD->kSigrLower, tSigrLower) == TRUE) { kSigrLower = pSPLD->kSigrLower; }
	else { return; };

	kPierTopNode = (pPSimd->tSipa.nSeismicForceDirection == T_SIPA_D::kAxis_Dir) ? tSigrLower.kNodeAxis : tSigrLower.kNodePerp;

	// H14년 전체교량 조사인 경우 교각의 기둥 정보는 계산에서 사용하지 않는다.
	if (pPSimd->tSimd.nAlwCalcType != seis_jp::kUserInput && pPSimd->tSipa.nDesignCode != BrdgSE_CodeJP::JROAD5_H14) {
		if (pDoc->m_pAttrCtrl2->GetQSigrLower()->FindColumn(kSigrLower, pSpld->strNameCOLM, tCOLM) == FALSE) return;
		if (tCOLM.aInvestigatedElem.GetCount() < 1) return;

		if (pDoc->m_pAttrCtrl2->GetQSigrLower()->FindColumnLowNode(kSigrLower, pSpld->strNameCOLM, tLowColmData) == FALSE) return;
		if (tLowColmData.Column.aInvestigatedElem.GetCount() < 1) return;
	}

	bValidValue = TRUE;
}

#pragma region // CSeismicDesignSimd

CSeismicDesignSimd::CSeismicDesignSimd() : CSeismicDesignDisp()
{
	m_param = nullptr;
}

void CSeismicDesignSimd::AddStep(CDgnProgressDlg* pProgress)
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	ASSERT(pDoc);
	if (pDoc == nullptr) return;

	T_SIDC_D dSidc; dSidc.Initialize();
	if (pDoc->m_pAttrCtrl2->GetQSidc()->Get(dSidc) == FALSE) return;

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
			//좌단 검토
			AddStep_Calculate(pProgress, kSimd);
			//우단 검토
			AddStep_Calculate(pProgress, kSimd);
		}
		else if (dSigr.nSubType == T_SIGR_D::kLower) {
			AddStep_Calculate(pProgress, kSimd);
		}
	}
}

void CSeismicDesignSimd::AddStep_Calculate(CDgnProgressDlg* pProgress, const T_SIMD_K& kSimd)
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();

	T_SIMD_D dSimd; T_SIPA_D dSipa;
	if (pDoc->m_pAttrCtrl2->GetQSimd()->Get(kSimd, dSimd) == FALSE) return;
	if (pDoc->m_pAttrCtrl2->GetQSipa()->Get(dSimd.kSipa, dSipa) == FALSE) return;

	int nCount = dSipa.aSeisThisK.GetCount();
	for (int i = 0; i < nCount; i++)
	{
		T_THIS_K kThis = dSipa.aSeisThisK[i];

		// 최대허용변위 δmax
		AddStep_RunEvaluation(pProgress, kSimd, kThis);
	}
}

void CSeismicDesignSimd::AddStep_RunEvaluation(CDgnProgressDlg* pProgress, const T_SIMD_K& kSimd, const T_THIS_K& kThis)
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();

	T_SIMD_D dSimd;
	if (pDoc->m_pAttrCtrl2->GetQSimd()->Get(kSimd, dSimd) == FALSE) return;

	CArray<double, double> aTimeStep;
	int nSize = pDoc->m_pAttrCtrl->GetTimeStep(kThis, aTimeStep);

	double dTimeStt = -DBL_MAX, dTiemEnd = DBL_MAX;
	if (dSimd.bTimeStepRange == TRUE) {
		m_pDoc->m_pAttrCtrl2->GetQSimd()->GetTimeRange(kSimd, kThis, dTimeStt, dTiemEnd);
	}

	for (int nTimeIdx = 0; nTimeIdx < nSize; nTimeIdx++)
	{
		const double& dTimeStep = aTimeStep[nTimeIdx];
		if (dTimeStep < dTimeStt) continue;
		if (dTiemEnd < dTimeStep) {
			break;
		}

		pProgress->m_nTotalSteps++;
	}
}

BOOL CSeismicDesignSimd::GetDeltaA(const doubleArray& aDeltaA, OUT double& rDeltaA)
{
	auto L_GetDeltaA_H14 = [&](OUT double& rValue) {
		switch (m_param->tSimd.nAlwCalcType)
		{
		case seis_jp::kAnalysisResult:
			ASSERT(aDeltaA.GetCount() == seis_jp::kSizeDa_AnalH14);
			if (aDeltaA.GetCount() > seis_jp::kDa_AnalH14) {
				rValue = aDeltaA[seis_jp::kDa_AnalH14];
				return TRUE;
			}
			break;
		case seis_jp::kMphi_Dy:
		case seis_jp::kMphi_Dpy:
			ASSERT(aDeltaA.GetCount() == seis_jp::kSizeDa_MPhiH14);
			if (aDeltaA.GetCount() > seis_jp::kDa_MPhiH14) {
				rValue = aDeltaA[seis_jp::kDa_MPhiH14];
				return TRUE;
			}
			break;
		}
		return FALSE;
		};

	auto L_GetDeltaA_H24 = [&](OUT double& rValue) {
		switch (m_param->tSimd.nAlwCalcType)
		{
		case seis_jp::kAnalysisResult:
			ASSERT(aDeltaA.GetCount() == seis_jp::kSizeDa_AnalH24);
			if (aDeltaA.GetCount() > seis_jp::kDa_AnalH24) {
				rValue = aDeltaA[seis_jp::kDa_AnalH24];
				return TRUE;
			}
			break;
		case seis_jp::kMphi_Dy:
		case seis_jp::kMphi_Dpy:
			ASSERT(aDeltaA.GetCount() == seis_jp::kSizeDa_MPhiH24);
			if (aDeltaA.GetCount() > seis_jp::kDa_MPhiH24) {
				rValue = aDeltaA[seis_jp::kDa_MPhiH24];
				return TRUE;
			}
			break;
		}
		return FALSE;
		};

	auto L_GetDeltaA_H29 = [&](OUT double& rValue) {
		switch (m_param->tSimd.nAlwCalcType)
		{
		case seis_jp::kAnalysisResult:
			ASSERT(aDeltaA.GetCount() == seis_jp::kSizeDa_AnalH29);
			if (aDeltaA.GetCount() > seis_jp::kDlsd_AnalH29) {
				rValue = aDeltaA[seis_jp::kDlsd_AnalH29];
				return TRUE;
			}
			break;
		case seis_jp::kMphi_Dy:
		case seis_jp::kMphi_Dpy:
			ASSERT(aDeltaA.GetCount() == seis_jp::kSizeDa_MPhiH29);
			if (aDeltaA.GetCount() > seis_jp::kDlsd_MPhiH29) {
				rValue = aDeltaA[seis_jp::kDlsd_MPhiH29];
				return TRUE;
			}
			break;
		}
		return FALSE;
		};

	BOOL bResult = FALSE;
	switch (m_param->tSipa.nDesignCode)
	{
	case BrdgSE_CodeJP::JROAD5_H14:
		bResult = L_GetDeltaA_H14(rDeltaA);
		break;
	case BrdgSE_CodeJP::JROAD5_H24:
		bResult = L_GetDeltaA_H24(rDeltaA);
		break;
	case BrdgSE_CodeJP::JROAD5_H29:
		bResult = L_GetDeltaA_H29(rDeltaA);
		break;
	}
	return bResult;
}

BOOL CSeismicDesignSimd::GetDeltaMax(const doubleArray& aDeltaMax, OUT double& rDeltaMax)
{
	BOOL bResult = FALSE;
	switch (m_param->tSimd.nRspCalcType)
	{
	case seis_jp::kNEXCO:
		ASSERT(aDeltaMax.GetCount() == seis_jp::kSizeNexco);
		if (aDeltaMax.GetCount() > seis_jp::kDmaxNexco) {
			bResult = TRUE;	rDeltaMax = aDeltaMax[seis_jp::kDmaxNexco];
		}
		break;

	case seis_jp::kJBEC:
		ASSERT(aDeltaMax.GetCount() == seis_jp::kSizeJbec);
		if (aDeltaMax.GetCount() > seis_jp::kDmaxJbec) {
			bResult = TRUE;	rDeltaMax = aDeltaMax[seis_jp::kDmaxJbec];
		}
		break;
	}
	return bResult;
}

void CSeismicDesignSimd::SetSizeArray(OUT T_SIMD_EVAL_DISP* pDisp)
{
	SetSizeDeltaA(pDisp->aDeltaA);
	SetSizeDeltaMax(pDisp->aDeltaMax);
}

void CSeismicDesignSimd::SetSizeDeltaA(OUT doubleArray& aValues)
{
	auto L_SetSizeDeltaA_H14 = [&]() {
		switch (m_param->tSimd.nAlwCalcType)
		{
		case seis_jp::kAnalysisResult:
			aValues.RemoveAll(); aValues.SetSize(seis_jp::kSizeDa_AnalH14);
			break;
		case seis_jp::kMphi_Dy:
		case seis_jp::kMphi_Dpy:
			aValues.RemoveAll(); aValues.SetSize(seis_jp::kSizeDa_MPhiH14);
			break;
		case seis_jp::kUserInput:
			aValues.RemoveAll(); aValues.SetSize(seis_jp::kSizeDa_UserH14);
			break;
		}
		};

	auto L_SetSizeDeltaA_H24 = [&]() {
		switch (m_param->tSimd.nAlwCalcType)
		{
		case seis_jp::kAnalysisResult:
			aValues.RemoveAll(); aValues.SetSize(seis_jp::kSizeDa_AnalH24);
			break;
		case seis_jp::kMphi_Dy:
		case seis_jp::kMphi_Dpy:
			aValues.RemoveAll(); aValues.SetSize(seis_jp::kSizeDa_MPhiH24);
			break;
		case seis_jp::kUserInput:
			aValues.RemoveAll(); aValues.SetSize(seis_jp::kSizeDa_UserH24);
			break;
		}
		};

	auto L_SetSizeDeltaA_H29 = [&]() {
		switch (m_param->tSimd.nAlwCalcType)
		{
		case seis_jp::kAnalysisResult:
			aValues.RemoveAll(); aValues.SetSize(seis_jp::kSizeDa_AnalH29);
			break;
		case seis_jp::kMphi_Dy:
		case seis_jp::kMphi_Dpy:
			aValues.RemoveAll(); aValues.SetSize(seis_jp::kSizeDa_MPhiH29);
			break;
		case seis_jp::kUserInput:
			aValues.RemoveAll(); aValues.SetSize(seis_jp::kSizeDa_UserH29);
			break;
		}
		};

	switch (m_param->tSipa.nDesignCode)
	{
	case BrdgSE_CodeJP::JROAD5_H14:
		L_SetSizeDeltaA_H14();
		break;
	case BrdgSE_CodeJP::JROAD5_H24:
		L_SetSizeDeltaA_H24();
		break;
	case BrdgSE_CodeJP::JROAD5_H29:
		L_SetSizeDeltaA_H29();
		break;
	}
}

void CSeismicDesignSimd::SetSizeDeltaMax(OUT doubleArray& aValues)
{
	switch (m_param->tSimd.nRspCalcType)
	{
	case seis_jp::kNEXCO:
		aValues.RemoveAll(); aValues.SetSize(seis_jp::kSizeNexco);
		break;

	case seis_jp::kJBEC:
		aValues.RemoveAll(); aValues.SetSize(seis_jp::kSizeJbec);
		break;
	}
}

BOOL CSeismicDesignSimd::Calculate(const T_SIMD_K& kSimd, T_SIMD_EVAL* pEval)
{
	P_SIMD input(kSimd);
	if (input.IsValidValue() == FALSE) return FALSE;
	m_param = &input;

	SetSizeArray(&pEval->Averge);

	int nCount = input.tSipa.aSeisThisK.GetCount();
	for (int i = 0; i < nCount; i++)
	{
		T_SIMD_EVAL_DISP disp;
		SetSizeArray(&disp);

		disp.kThis = input.tSipa.aSeisThisK[i];

		RunEvaluation(pEval, &disp);

		pEval->Averge += disp;
		pEval->aDisplacement.Add(disp);
	}

	pEval->Averge /= nCount;

	SetDataEval(pEval);

	// 끝까지 오면 정상적으로 계산된거임.
	pEval->bCheckResult = TRUE;
	return TRUE;
};

void CSeismicDesignSimd::RunEvaluation(IN T_SIMD_EVAL* pEval, OUT T_SIMD_EVAL_DISP* pDisp)
{
	// (+)δa, (-)δa 계산
	doubleArray aPlus; SetSizeDeltaA(aPlus);
	doubleArray aMinus; SetSizeDeltaA(aMinus);
	if (CalcDeltaA(pDisp->kThis, &m_param->tSimd.PLUS_D, aPlus) == FALSE) { ASSERT(FALSE); return; }
	if (CalcDeltaA(pDisp->kThis, &m_param->tSimd.MINUS_D, aMinus) == FALSE) { ASSERT(FALSE); return; }

	double dDa_plus = 0, dDa_minus = 0;
	if (GetDeltaA(aPlus, dDa_plus) == FALSE) { ASSERT(FALSE); return; }
	if (GetDeltaA(aMinus, dDa_minus) == FALSE) { ASSERT(FALSE); return; }

	CArray<double, double> aTimeStep;
	int nSize = m_pDoc->m_pAttrCtrl->GetTimeStep(pDisp->kThis, aTimeStep);

	double dTimeStt = -DBL_MAX, dTiemEnd = DBL_MAX;
	if (m_param->tSimd.bTimeStepRange == TRUE) {
		m_pDoc->m_pAttrCtrl2->GetQSimd()->GetTimeRange(m_param->kSimd, pDisp->kThis, dTimeStt, dTiemEnd);
	}

	BOOL bMinus = FALSE;
	int nMaxTimeIdx = 0;
	double dMaxRatio = -DBL_MAX;
	doubleArray aMaxDa, aMaxDmax;
	for (int nTimeIdx = 0; nTimeIdx < nSize; nTimeIdx++)
	{
		const double& dTimeStep = aTimeStep[nTimeIdx];
		if(dTimeStep < dTimeStt) continue;
		if (dTiemEnd < dTimeStep) {
			break;
		}

		doubleArray aDeltaMax; SetSizeDeltaMax(aDeltaMax);
		BOOL bResult = CalcDeltaMax(pEval, pDisp->kThis, nTimeIdx, aDeltaMax);

		if (m_pProgress->NextStep() == FALSE) throw FALSE;

		if (bResult == FALSE)
			continue;

		double dDmax_step = 0;
		if (GetDeltaMax(aDeltaMax, dDmax_step) == FALSE) { ASSERT(FALSE); continue; }

		double dRatio = (dDmax_step < 0) ? dgn::SafeDiv(abs(dDmax_step), abs(dDa_minus)) : dgn::SafeDiv(abs(dDmax_step), abs(dDa_plus));

		if (dRatio > dMaxRatio) {
			dMaxRatio = dRatio;
			nMaxTimeIdx = nTimeIdx;
			bMinus = dDmax_step < 0;
			aMaxDa.RemoveAll();
			aMaxDmax.RemoveAll();
			aMaxDmax.Copy(aDeltaMax);
			(dDmax_step < 0) ? aMaxDa.Copy(aMinus) : aMaxDa.Copy(aPlus);
		}
	}

	if (dMaxRatio == -DBL_MAX)
		return;

	pDisp->dRatio = dMaxRatio;
	pDisp->aDeltaA.RemoveAll();
	pDisp->aDeltaA.Copy(aMaxDa);
	pDisp->aDeltaMax.RemoveAll();
	pDisp->aDeltaMax.Copy(aMaxDmax);
	pDisp->nDirAnal = bMinus ? seis_jp::kMinus : seis_jp::kPlus;
}

BOOL CSeismicDesignSimd::CalcDeltaMax(IN T_SIMD_EVAL* pEval, const T_THIS_K& kThis, const int& nTimeIdx, OUT doubleArray& aDeltaMax)
{
	BOOL bResult = FALSE;

	switch (pEval->nPosType)
	{
	case T_SIMD_EVAL::kLower:
		bResult = CalcDeltaMax_Lower(pEval, kThis, nTimeIdx, aDeltaMax);
		break;

	case T_SIMD_EVAL::kLeft:
	case T_SIMD_EVAL::kRight:
		bResult = CalcDeltaMax_Upper(pEval, kThis, nTimeIdx, aDeltaMax);
		break;
	}

	return bResult;
}

BOOL CSeismicDesignSimd::CalcDeltaMax_Upper(IN T_SIMD_EVAL* pEval, const T_THIS_K& kThis, const int& nTimeIdx, OUT doubleArray& aDeltaMax)
{
	T_SIGR_UPPER_D tSigrUpper;
	if (m_pDoc->m_pAttrCtrl2->GetQSigrUpper()->Get(m_param->kSigr, tSigrUpper) == FALSE) return FALSE;

	CSeismicDesignDisp_Upper DispU; DispU.SetProgressDlg(m_pProgress);
	double aDisp[eVector2d::kMax2d] = { 0.0, 0.0 };
	if (DispU.CalcDeltaMax(pEval->kGirderNode, kThis, nTimeIdx, aDisp) == FALSE) return FALSE;

	ParamAngleOption param;
	param.kThis = kThis;
	param.kElem = tSigrUpper.aElements[0];
	param.nDirDisp = m_param->tSimd.nDirDisp;
	param.nShearDir = tSigrUpper.nDirShear;
	param.dUserAngle = m_param->tSimd.dAngle;
	param.nSeismicForceDirection = m_param->tSipa.nSeismicForceDirection;

	double dCompVal = 0;//합성벡터
	GetDispOfAngle(param, aDisp[eVector2d::kDx], aDisp[eVector2d::kDy], dCompVal);

	BOOL bResult = FALSE;
	switch (m_param->tSimd.nRspCalcType)
	{
	case seis_jp::kJBEC:
		if (aDeltaMax.GetCount() > seis_jp::kDmaxJbec) {
			bResult = TRUE; aDeltaMax[seis_jp::kDmaxJbec] = dCompVal;
		}
		break;
	case seis_jp::kNEXCO:
		if (aDeltaMax.GetCount() > seis_jp::kDmaxNexco) {
			bResult = TRUE; aDeltaMax[seis_jp::kDmaxNexco] = dCompVal;
		}
		break;
	}
	return bResult;
}

BOOL CSeismicDesignSimd::CalcDeltaMax_Lower(IN T_SIMD_EVAL* pEval, const T_THIS_K& kThis, const int& nTimeIdx, OUT doubleArray& aDeltaMax)
{
	ASSERT(m_param->tSimd.PLUS_D.strNameCOLM == m_param->tSimd.MINUS_D.strNameCOLM);
	// 교각별 조사의 경우 +방향, -방향의 기둥은 항상 같은 값을 가진다.
	P_SIMD_Lower PLower(m_param, kThis, &m_param->tSimd.PLUS_D);
	if (PLower.IsValidValue() == FALSE) return FALSE;

	ParamAngleOption param;
	param.kThis = kThis;
	param.kElem = PLower.tLowColmData.Column.aInvestigatedElem[0];
	param.nDirDisp = m_param->tSimd.nDirDisp;
	param.nShearDir = PLower.tLowColmData.Column.nDirShear;
	param.dUserAngle = m_param->tSimd.dAngle;
	param.nSeismicForceDirection = m_param->tSipa.nSeismicForceDirection;

	PDeltaVal tParam = std::make_tuple(	m_param->tSimd.kSigr,
										kThis,
										PLower.kPierTopNode,
										seis_jp::kDirAnal_0,
										m_param->tSimd.nRspCalcType,
										&PLower.tLowColmData);

	CSeismicDesignDisp_Lower DispL; DispL.SetProgressDlg(m_pProgress);
	return DispL.CalcDeltaMax(tParam, param, nTimeIdx, aDeltaMax);
}

void CSeismicDesignSimd::SetDataEval(OUT T_SIMD_EVAL* pEval)
{
	// 2025.10.22 경환이사보님 확인 각 지진파 별로 +방향, -방향 따로 따로 값이 나올 수 있어
	// 전체계/교각별 조사설정 UI의 경우 +방향, -방향의 교각과 기둥은 항상 같은 값을 가진다.
	ASSERT_ONCE(m_param->tSimd.PLUS_D.kSigrLower == m_param->tSimd.MINUS_D.kSigrLower);
	ASSERT_ONCE(m_param->tSimd.PLUS_D.strNameCOLM == m_param->tSimd.MINUS_D.strNameCOLM);
	
	T_SIGR_LOWER_D dSigrLower;
	m_pDoc->m_pAttrCtrl2->GetQSigrLower()->Get(m_param->tSimd.PLUS_D.kSigrLower, dSigrLower);

	pEval->kSigrLower = m_param->tSimd.PLUS_D.kSigrLower;
	pEval->strColumnName = m_param->tSimd.PLUS_D.strNameCOLM;
	pEval->kPierTopNode = (m_param->tSipa.nSeismicForceDirection == T_SIPA_D::kAxis_Dir) ? dSigrLower.kNodeAxis : dSigrLower.kNodePerp;
}
#pragma endregion

#pragma region // CSeismicDesignSimd_Upper

#pragma endregion

#pragma region // CSeismicDesignSimd_Lower

#pragma endregion