#include "stdafx.h"
#include "SeismicDesignSidp_H14.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_EditData.h"
#include "..\wg_db\wg_db_PostCtrl.h"
#include "..\wg_db\wg_db_ThisInfo.h"
#include "..\wg_db\wg_db_UndoCtrl.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl2.h"
#include "..\wg_db\DB_ST_DT_SIDP_EVAL.h"
#include "..\wg_db\DB_QUERY_SEIS_CVL_JP.h"
#include "..\wg_db\BrdgSeisEvalResult_JP.h"

#include "DgnProgressDlg.h"
#include "BrdgSeisEval_JP.h"
#include "SeismicDesignSird_H14.h"
#include "DgnToolMPhi_SeismicDesign_JP.h"

CSeismicDesignSidp_H14::CSeismicDesignSidp_H14() : CSeismicDesignDisp()
{
}

CSeismicDesignSidp_H14::~CSeismicDesignSidp_H14()
{
}

void CSeismicDesignSidp_H14::AddProgressStep(CDgnProgressDlg* pProgress)
{
	if (m_pDoc == NULL) return;

	CBrdgSeisEvalResult_JP* pResult = m_pDoc->m_pPostCtrl->GetBrdgSeisEvalResult_JP(); if (!pResult) { ASSERT(0); return; }
	if (pResult == NULL) { ASSERT(FALSE); return; }

	T_KEY_LIST sidpKeys;
	m_pDoc->m_pAttrCtrl2->GetQSidp()->GetKeyList(sidpKeys);

	for (int idx = 0; idx < sidpKeys.GetCount(); idx++)
	{
		T_SIDP_K kSidp = sidpKeys[idx];
		T_SIDP_D dSidp; T_SIPA_D dSipa; T_SIGR_LOWER_D dSigrLower;
		if (m_pDoc->m_pAttrCtrl2->GetQSidp()->Get(kSidp, dSidp) == FALSE) continue;
		if (m_pDoc->m_pAttrCtrl2->GetQSipa()->Get(dSidp.kSipa, dSipa) == FALSE) continue;
		if (m_pDoc->m_pAttrCtrl2->GetQSigrLower()->Get(dSidp.kSigrLower, dSigrLower) == FALSE) continue;
		if (dSipa.nDesignCode != GetCurrentDgnCode()) continue;

		// 2025.11.10 chlim 아래는 의미가 없다.( ProgressStep 이 호출될 때는 최대응답변위 계산이 실행되지 않았기 때문)
// 		T_SIMD_EVAL evalSimd; //변위 소성율조사는 최대응답변위조사 결과가 있을 경우 수행한다.
// 		if (pResult->GetResultSimd(dSidp.kSipa, dSidp.kSigrLower, T_SIRD_EVAL::kLower, evalSimd) == FALSE) {
// 			continue;
// 		}

		pProgress->m_nTotalSteps++;
	}
}

BOOL CSeismicDesignSidp_H14::Calculate(CDgnProgressDlg* pProgress)
{
	if (m_pDoc == NULL) return FALSE;

	CBrdgSeisEvalResult_JP* pResult = m_pDoc->m_pPostCtrl->GetBrdgSeisEvalResult_JP(); if (!pResult) { ASSERT(0); return FALSE; }
	if (pResult == NULL) { ASSERT(FALSE); return FALSE; }

	// Code 단위계 변환
	CUnitConversion unit(D_UNITSYS_FORCE_INDEX_N, D_UNITSYS_LENGTH_INDEX_M);

	T_KEY_LIST sidpKeys;
	m_pDoc->m_pAttrCtrl2->GetQSidp()->GetKeyList(sidpKeys);

	for (int idx = 0; idx < sidpKeys.GetCount(); idx++)
	{
		T_SIDP_K kSidp = sidpKeys[idx];
		T_SIDP_D dSidp; T_SIPA_D dSipa; T_SIGR_LOWER_D dSigrLower;
		if (m_pDoc->m_pAttrCtrl2->GetQSidp()->Get(kSidp, dSidp) == FALSE) continue;
		if (m_pDoc->m_pAttrCtrl2->GetQSipa()->Get(dSidp.kSipa, dSipa) == FALSE) continue;
		if (m_pDoc->m_pAttrCtrl2->GetQSigrLower()->Get(dSidp.kSigrLower, dSigrLower) == FALSE) continue;
		if (dSipa.nDesignCode != GetCurrentDgnCode()) continue;

		T_SIMD_EVAL evalSimd; //변위 소성율조사는 최대응답변위조사 결과가 있을 경우 수행한다.
		if (pResult->GetResultSimd(dSidp.kSipa, dSidp.kSigrLower, T_SIRD_EVAL::kLower, evalSimd) == FALSE) {
			T_SIPA_D dSipa; T_SIGR_D dSigr;
			m_pDoc->m_pAttrCtrl2->GetQSipa()->Get(dSidp.kSipa, dSipa);
			m_pDoc->m_pAttrCtrl2->GetQSigr()->Get(dSidp.kSigrLower, dSigr);

			CString str; str.Format(_LS(IDS_DGN_SEIS_EVAL_MSG_NOT_EXIST_SIMD_EVAL_DATA), dSipa.strCaseName, dSigr.strName);
			m_pDoc->DisplayHistoryMessage(str);
			continue;
		}

		T_SIDP_EVAL eval;
		eval.kSidp = kSidp;
		eval.kSipa = dSidp.kSipa;
		eval.kSigrLower = dSidp.kSigrLower;
		eval.nSeismicForceType = dSipa.nSeismicForceType;
		eval.nSeismicForceDirection = dSipa.nSeismicForceDirection;
		if (dSipa.nSeismicForceDirection == T_SIPA_D::kAxis_Dir)
			eval.kNode = dSigrLower.kNodeAxis;
		else if (dSipa.nSeismicForceDirection == T_SIPA_D::kPerp_Dir)
			eval.kNode = dSigrLower.kNodePerp;
		else { ASSERT(FALSE); continue; }

		CSeismicDesignSidp_H14_Evaluation::Execute(pProgress, kSidp, eval.kNode, &eval);
		if (pProgress->NextStep() == FALSE) throw FALSE;

		// save result
		CBrdgSeisEvalResult_JP* pResult = m_pDoc->m_pPostCtrl->GetBrdgSeisEvalResult_JP(); if (!pResult) { ASSERT(0); }
		pResult->SetResultSidp(dSidp.kSipa, eval.kSigrLower, eval);
	}

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////   CSeismicDesignSidp_H14_Evaluation   //////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

CSeismicDesignSidp_H14_Evaluation::CSeismicDesignSidp_H14_Evaluation(CDgnProgressDlg* pProgress)
	: CSeismicDesignSidp_H14()
{
	m_pProgress = pProgress;
	m_kSidp = 0;
	m_kNode = 0;
	m_pSidp = NULL;
	m_pSipa = NULL;
	m_pEvalSimd = NULL;
	m_pLowColmData = NULL;
}

CSeismicDesignSidp_H14_Evaluation::~CSeismicDesignSidp_H14_Evaluation()
{
}

void CSeismicDesignSidp_H14_Evaluation::Execute(IN CDgnProgressDlg* pProgress, const T_SIDP_K& kSidp, const T_NODE_K& kNode, OUT T_SIDP_EVAL* pEval)
{
	CSeismicDesignSidp_H14_Evaluation eval(pProgress);
	eval.Invoke(pEval);
}

void CSeismicDesignSidp_H14_Evaluation::Invoke(OUT T_SIDP_EVAL* pEval)
{
	CBrdgSeisEvalResult_JP* pResult = m_pDoc->m_pPostCtrl->GetBrdgSeisEvalResult_JP(); if (!pResult) { ASSERT(0); return; }

	T_SIDP_D dSidp; T_SIPA_D dSipa; T_SIMD_EVAL evalSimd; T_SIRD_EVAL evalSird; ColumnData colm;
	if (m_pDoc->m_pAttrCtrl2->GetQSidp()->Get(pEval->kSidp, dSidp) == FALSE) return;
	if (m_pDoc->m_pAttrCtrl2->GetQSipa()->Get(dSidp.kSipa, dSipa) == FALSE) return;
	
	auto L_OutputMsg_NotExistSimdEvalData = [&]() {
		T_SIGR_D dSigr;
		m_pDoc->m_pAttrCtrl2->GetQSigr()->Get(dSidp.kSigrLower, dSigr);
		CString str; str.Format(_LS(IDS_DGN_SEIS_EVAL_MSG_NOT_EXIST_SIMD_EVAL_DATA), dSipa.strCaseName, dSigr.strName);
		m_pDoc->DisplayHistoryMessage(str);
	};
	
	if (pResult->GetResultSimd(dSidp.kSipa, dSidp.kSigrLower, T_SIMD_EVAL::kLower, evalSimd) == FALSE) { L_OutputMsg_NotExistSimdEvalData(); return; }
	if (evalSimd.aDisplacement.GetCount() < 1) { L_OutputMsg_NotExistSimdEvalData(); return; }

	if (m_pDoc->m_pAttrCtrl2->GetQSigrLower()->FindColumnLowNode(dSidp.kSigrLower, dSidp.strNameCOLM, colm) == FALSE) return;
	if (colm.Column.aInvestigatedElem.GetCount() < 1) return;

	m_kSidp = pEval->kSidp;
	m_kNode = pEval->kNode;
	m_pSidp = &dSidp;
	m_pSipa = &dSipa;
	m_pEvalSimd = &evalSimd;
	m_pLowColmData = &colm;

	T_SIDP_EVAL_BY_CALC avergeCalc;
	T_SIDP_EVAL_BY_ANAL avergeAnal;

	int nCount = evalSimd.aDisplacement.GetCount();
	for (int i = 0; i < nCount; i++)
	{
		T_SIMD_EVAL_DISP& disp = evalSimd.aDisplacement[i];

		//μa 허용소성율 계산
		T_SIDP_EVAL_BY_CALC calc;
		if (GetCalcDisplacement(&disp, &calc) == FALSE) continue;

		T_SIDP_EVAL_BY_ANAL anal;
		//해석에 의한 최대응답변위
		anal.dDeltaMax = evalSimd.GetDeltaMax(disp.aDeltaMax);
		//2024.11.07 chlim δy의 값을 최대응답변위 조사설정으로 부터 가져오듯 δmax 도 최대응답변위로 부터 가져오기로 함(경환이사보님 확인완료)
		//CalcDeltaMax(kThis, pEval, dDeltaMax); -> 이 함수 관련 소스는 전부 삭제. 참고하고 싶으면 v955 소스 확인 바람

		anal.dMuR = dgn::SafeDiv(anal.dDeltaMax, calc.dDelta_y);
		anal.dRatio = dgn::SafeDiv(anal.dMuR, calc.dMu_a);

		pEval->aValByCalc.Add(calc);
		pEval->aValByAnal.Add(anal);

		avergeCalc += calc;
		avergeAnal += anal;
	}

	avergeCalc /= nCount;
	avergeAnal /= nCount;

	pEval->AvergeCalc = avergeCalc;
	pEval->AvergeAnal = avergeAnal;
	// 끝까지 오면 정상적으로 계산된거임.
	pEval->bCheckResult = TRUE;
}

BOOL CSeismicDesignSidp_H14_Evaluation::GetCalcDisplacement(IN T_SIMD_EVAL_DISP* pEvalDisp, OUT T_SIDP_EVAL_BY_CALC* pEval)
{
	T_SIDP_D dSidp;
	if (m_pDoc->m_pAttrCtrl2->GetQSidp()->Get(m_kSidp, dSidp) == FALSE) return FALSE;

	if (GetDeltaY(pEvalDisp, pEval->dDelta_y) == FALSE) return FALSE;
	if (GetDeltaU(pEvalDisp, pEval->dDelta_u_ls) == FALSE) return FALSE;

	//μa=1+(δu-δy)/(αδy)
	auto L_FormulaH14 = [](const double& dDeltaY, const double& dDeltaU, const double& dAlpha)
	{
		double dVal1 = abs(dDeltaU) - abs(dDeltaY);
		double dVal2 = dAlpha * abs(dDeltaY);
		double dMu_a = 1 + dgn::SafeDiv(dVal1, dVal2);
		return dMu_a;
	};

	//μa=δls/(αδy)
	auto L_FormulaH24 = [](const double& dDeltaY, const double& dDeltaLs, const double& dAlpha)
	{
		double dVal = dAlpha * abs(dDeltaY);
		double dMu_a = abs(dgn::SafeDiv(abs(dDeltaLs), dVal));
		return dMu_a;
	};

	BOOL bResult = FALSE;
	switch (dSidp.ADPR_D.nAllowDispPlasFormula)
	{
		case T_SIDP_ADPR_D::kMuA_1_Plus_Ellipsis:
			pEval->dMu_a = L_FormulaH14(pEval->dDelta_y, pEval->dDelta_u_ls, m_pSidp->ADPR_D.dAlpha); bResult = TRUE;
			break;
		case T_SIDP_ADPR_D::kMuA_DeltaLs_Ellipsis:
			pEval->dMu_a = L_FormulaH24(pEval->dDelta_y, pEval->dDelta_u_ls, m_pSidp->ADPR_D.dAlpha); bResult = TRUE;
			break;
		default:
			ASSERT(FALSE);
			break;
	}
	ASSERT_ONCE(bResult);
	return bResult;
}

BOOL CSeismicDesignSidp_H14_Evaluation::GetDeltaY(IN T_SIMD_EVAL_DISP* pDispSimd, OUT double& dDeltaY)
{
	CBrdgSeisEvalResult_JP* pResult = m_pDoc->m_pPostCtrl->GetBrdgSeisEvalResult_JP(); if (!pResult) { ASSERT(0); return FALSE; }

	auto L_GetDeltaY_H14 = [&](OUT double& rValue) {
		switch (m_pEvalSimd->nAlwCalcType)
		{
		case seis_jp::kAnalysisResult:
			ASSERT(pDispSimd->aDeltaA.GetCount() == seis_jp::kSizeDa_AnalH14);
			if (pDispSimd->aDeltaA.GetCount() > seis_jp::kDy_AnalH14) {
				rValue = pDispSimd->aDeltaA[seis_jp::kDy_AnalH14];
				return TRUE;
			}
			break;
		case seis_jp::kMphi_Dy:
		case seis_jp::kMphi_Dpy:
			ASSERT(pDispSimd->aDeltaA.GetCount() == seis_jp::kSizeDa_MPhiH14);
			if (pDispSimd->aDeltaA.GetCount() > seis_jp::kDy_MPhiH14) {
				rValue = pDispSimd->aDeltaA[seis_jp::kDy_MPhiH14];
				return TRUE;
			}
			break;
		}
		return FALSE;
	};

	auto L_OutputMsg_NotExistSirdEvalData = [&]() {
		T_SIGR_D dSigr;
		m_pDoc->m_pAttrCtrl2->GetQSigr()->Get(m_pSidp->kSigrLower, dSigr);
		CString str; str.Format(_LS(IDS_DGN_SEIS_EVAL_MSG_NOT_EXIST_SIRD_EVAL_DATA), m_pSipa->strCaseName, dSigr.strName);
		m_pDoc->DisplayHistoryMessage(str);
	};

	auto L_GetDeltaY_H24 = [&](OUT double& rValue) {
		T_SIRD_EVAL evalSird; T_SIRD_D tSird;
		if (pResult->GetResultSird(m_pSidp->kSipa, m_pSidp->kSigrLower, T_SIRD_EVAL::kLower, evalSird) == FALSE) {
			L_OutputMsg_NotExistSirdEvalData();
			return FALSE;
		}
		if (evalSird.aDisplacement.GetCount() < 1) {
			L_OutputMsg_NotExistSirdEvalData();
			return FALSE;
		}
		if (m_pDoc->m_pAttrCtrl2->GetQSird()->Get(evalSird.kSird, tSird) == FALSE) {
			L_OutputMsg_NotExistSirdEvalData();
			return FALSE;
		}

		P_SIRD input(evalSird.kSird);
		CSeismicDesignSird_H14_Lower sirdLower(m_pProgress);
		sirdLower.SetParamSird(&input);

		// #3285 変位塑性率を検討する際には δmax/δa に着目すべきであり、また、同時性でも考慮する必要があり、
		// δmax/δa が最大となる時点に対応する δy の値を用いることとする。
		double dDeltaMax = m_pEvalSimd->GetDeltaMax(pDispSimd->aDeltaMax);
		T_SIRD_RSDP_D* pRsdp = dDeltaMax < 0 ? &tSird.MINUS_D : &tSird.PLUS_D;

		return sirdLower.CalcDelta_y(pDispSimd->kThis, pRsdp, rValue);
	};

	BOOL bResult = FALSE;
	switch (m_pEvalSimd->nDesignCode)
	{
	case BrdgSE_CodeJP::JROAD5_H14:
		bResult = L_GetDeltaY_H14(dDeltaY);
		break;
	case BrdgSE_CodeJP::JROAD5_H24:
		bResult = L_GetDeltaY_H24(dDeltaY);
		break;
	}
	return bResult;
}

BOOL CSeismicDesignSidp_H14_Evaluation::GetDeltaU(IN T_SIMD_EVAL_DISP* pDisp, OUT double& dDeltaU)
{
	auto L_GetDeltaU_H14 = [&](OUT double& rValue) {
		switch (m_pEvalSimd->nAlwCalcType)
		{
		case seis_jp::kAnalysisResult:
			ASSERT(pDisp->aDeltaA.GetCount() == seis_jp::kSizeDa_AnalH14);
			if (pDisp->aDeltaA.GetCount() > seis_jp::kDu_AnalH14) {
				rValue = pDisp->aDeltaA[seis_jp::kDu_AnalH14];
				return TRUE;
			}
			break;
		case seis_jp::kMphi_Dy:
		case seis_jp::kMphi_Dpy:
			ASSERT(pDisp->aDeltaA.GetCount() == seis_jp::kSizeDa_MPhiH14);
			if (pDisp->aDeltaA.GetCount() > seis_jp::kDu_MPhiH14) {
				rValue = pDisp->aDeltaA[seis_jp::kDu_MPhiH14];
				return TRUE;
			}
			break;
		}
		return FALSE;
	};

	auto L_GetDeltaU_H24 = [&](OUT double& rValue) {
		switch (m_pEvalSimd->nAlwCalcType)
		{
		case seis_jp::kAnalysisResult:
			ASSERT(pDisp->aDeltaA.GetCount() == seis_jp::kSizeDa_AnalH24);
			if (pDisp->aDeltaA.GetCount() > seis_jp::kD_f_ls_AnalH24) {
				rValue = pDisp->aDeltaA[seis_jp::kD_f_ls_AnalH24];
				return TRUE;
			}
			break;
		case seis_jp::kMphi_Dy:
		case seis_jp::kMphi_Dpy:
			ASSERT(pDisp->aDeltaA.GetCount() == seis_jp::kSizeDa_MPhiH24);
			if (pDisp->aDeltaA.GetCount() > seis_jp::kDls_MPhiH24) {
				rValue = pDisp->aDeltaA[seis_jp::kDls_MPhiH24];
				return TRUE;
			}
			break;
		}
		return FALSE;
	};

	BOOL bResult = FALSE;
	switch (m_pEvalSimd->nDesignCode)
	{
	case BrdgSE_CodeJP::JROAD5_H14:
		bResult = L_GetDeltaU_H14(dDeltaU);
		break;
	case BrdgSE_CodeJP::JROAD5_H24:
		bResult = L_GetDeltaU_H24(dDeltaU);
		break;
	}
	return bResult;
}