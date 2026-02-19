#include "stdafx.h"
#include "SeismicDesignSird.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl2.h"
#include "..\wg_db\DB_QUERY_SEIS_CVL_JP.h"
#include "..\wg_db\BrdgSeisEvalResult_JP.h"

#include "DgnProgressDlg.h"
#include "SeismicDesignSird_H14.h"
#include "SeismicDesignSird_H24.h"

P_SIRD::P_SIRD(const T_SIRD_K& sirdKey)
{
	bValidValue = FALSE;
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	if (pDoc == nullptr) { ASSERT(pDoc); return; }

	if (pDoc->m_pAttrCtrl2->GetQSird()->Get(sirdKey, tSird) == TRUE) { kSird = sirdKey; }
	else { return; };
	if (pDoc->m_pAttrCtrl2->GetQSipa()->Get(tSird.kSipa, tSipa) == TRUE) { kSipa = tSird.kSipa; }
	else { return; };
	if (pDoc->m_pAttrCtrl2->GetQSigr()->Get(tSird.kSigr, tSigr) == TRUE) { kSigr = tSird.kSigr; }
	else { return; };

	bValidValue = TRUE;
}

P_SIRD_Lower::P_SIRD_Lower(IN P_SIRD* pPSird, IN T_SIRD_RSDP_D* pRSDP)
{
	bValidValue = FALSE;
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	if (pDoc == nullptr) { ASSERT(pDoc); return; }
	if (pPSird == nullptr) { return; }
	if (pRSDP == nullptr) { return; }

	pRsdp = pRSDP;

	if (pDoc->m_pAttrCtrl2->GetQSigrLower()->Get(pPSird->tSird.kSigr, tSigrLower) == TRUE) { kSigrLower = pPSird->tSird.kSigr; }
	else { return; };

	kPierTopNode = (pPSird->tSipa.nSeismicForceDirection == T_SIPA_D::kAxis_Dir) ? tSigrLower.kNodeAxis : tSigrLower.kNodePerp;

	if (pPSird->tSird.nAlwCalcType != seis_jp::kUserInput) {
		if (pDoc->m_pAttrCtrl2->GetQSigrLower()->FindColumn(kSigrLower, pRsdp->strNameCOLM, tCOLM) == FALSE) return;
		if (tCOLM.aInvestigatedElem.GetCount() < 1) return;

		if (pDoc->m_pAttrCtrl2->GetQSigrLower()->FindColumnLowNode(kSigrLower, pRsdp->strNameCOLM, tLowColmData) == FALSE) return;
		if (tLowColmData.Column.aInvestigatedElem.GetCount() < 1) return;
	}

	bValidValue = TRUE;
}

#pragma region // CSeismicDesignSird

CSeismicDesignSird::CSeismicDesignSird() : CSeismicDesignDisp()
{
	m_param = nullptr;
}

void CSeismicDesignSird::AddStep(CDgnProgressDlg* pProgress)
{
	T_SIDC_D dSidc; dSidc.Initialize();
	if (m_pDoc->m_pAttrCtrl2->GetQSidc()->Get(dSidc) == FALSE) return;

	T_KEY_LIST sirdKeys;
	m_pDoc->m_pAttrCtrl2->GetQSird()->GetKeyList(sirdKeys);

	for (int idx = 0; idx < sirdKeys.GetCount(); idx++)
	{
		T_SIRD_K kSird = sirdKeys[idx];
		T_SIRD_D dSird; T_SIPA_D dSipa; T_SIGR_D dSigr;
		if (m_pDoc->m_pAttrCtrl2->GetQSird()->Get(kSird, dSird) == FALSE) continue;
		if (m_pDoc->m_pAttrCtrl2->GetQSipa()->Get(dSird.kSipa, dSipa) == FALSE) continue;
		if (m_pDoc->m_pAttrCtrl2->GetQSigr()->Get(dSird.kSigr, dSigr) == FALSE) continue;
		if (dSipa.nDesignCode != dSidc.nDesignCode) continue;

		if (dSigr.nSubType == T_SIGR_D::kUpper) {
			//좌단 검토
			AddStep_Calculate(pProgress, kSird);
			//우단 검토
			AddStep_Calculate(pProgress, kSird);
		}
		else if (dSigr.nSubType == T_SIGR_D::kLower) {
			AddStep_Calculate(pProgress, kSird);
		}
	}
}

void CSeismicDesignSird::AddStep_Calculate(CDgnProgressDlg* pProgress, const T_SIRD_K& kSird)
{
	T_SIRD_D dSird; T_SIPA_D dSipa;
	if (m_pDoc->m_pAttrCtrl2->GetQSird()->Get(kSird, dSird) == FALSE) return;
	if (m_pDoc->m_pAttrCtrl2->GetQSipa()->Get(dSird.kSipa, dSipa) == FALSE) return;

	int nCount = dSipa.aSeisThisK.GetCount();
	for (int i = 0; i < nCount; i++)
	{
		T_THIS_K kThis = dSipa.aSeisThisK[i];

		// 최대허용변위 δmax
		AddStep_RunEvaluation(pProgress, kSird, kThis);
	}
}

void CSeismicDesignSird::AddStep_RunEvaluation(CDgnProgressDlg* pProgress, const T_SIRD_K& kSird, const T_THIS_K& kThis)
{
	T_SIRD_D dSird;
	if (m_pDoc->m_pAttrCtrl2->GetQSird()->Get(kSird, dSird) == FALSE) return;

	CArray<double, double> aTimeStep;
	int nSize = m_pDoc->m_pAttrCtrl->GetTimeStep(kThis, aTimeStep);

	double dTimeStt = -DBL_MAX, dTiemEnd = DBL_MAX;
	if (dSird.bTimeStepRange == TRUE) {
		m_pDoc->m_pAttrCtrl2->GetQSird()->GetTimeRange(kSird, kThis, dTimeStt, dTiemEnd);
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

BOOL CSeismicDesignSird::Calculate(const T_SIRD_K& kSird, T_SIRD_EVAL* pEval)
{
	P_SIRD input(kSird);
	if (input.IsValidValue() == FALSE) return FALSE;
	m_param = &input;

	int nCount = input.tSipa.aSeisThisK.GetCount();
	for (int i = 0; i < nCount; i++)
	{
		T_SIRD_EVAL_DISP disp; disp.Initialize();

		disp.kThis = input.tSipa.aSeisThisK[i];

		RunEvaluation(pEval , &disp);

		pEval->Averge += disp;
		pEval->aDisplacement.Add(disp);
	}

	pEval->Averge /= nCount;

	SetDataEval(pEval);

	// 끝까지 오면 정상적으로 계산된거임.
	pEval->bCheckResult = TRUE;
	return TRUE;
}

void CSeismicDesignSird::RunEvaluation(IN T_SIRD_EVAL* pEval, OUT T_SIRD_EVAL_DISP* pDisp)
{
	//속도개선을 위해 스텝별로 돌기전에 아래 항목들은 +, - 방향 각각 계산 해 둔다.
	T_SIRD_EVAL_DISP aParam[T_SIRD_EVAL::kDirMax];
	// (+)δra, (-)δra 허용잔류변위(m) 계산
	if (CalcDelta_ra(&m_param->tSird.PLUS_D, aParam[T_SIRD_EVAL::kPlus].dDelta_ra) == FALSE) { ASSERT_ONCE(FALSE); return; }
	if (CalcDelta_ra(&m_param->tSird.MINUS_D, aParam[T_SIRD_EVAL::kMinus].dDelta_ra) == FALSE) { ASSERT_ONCE(FALSE); return; }
	// δy：교량 전체계의 항복변위(m)
	if (CalcDelta_y(pDisp->kThis, &m_param->tSird.PLUS_D, aParam[T_SIRD_EVAL::kPlus].dDelta_y) == FALSE) { ASSERT_ONCE(FALSE); return; }
	if (CalcDelta_y(pDisp->kThis, &m_param->tSird.MINUS_D, aParam[T_SIRD_EVAL::kMinus].dDelta_y) == FALSE) { ASSERT_ONCE(FALSE); return; }
	//Cr：잔류변위 보정계수
	if (Get_Cr((m_param->tSigr.nSubType == T_SIGR_D::kLower ? m_param->tSird.kSigr : m_param->tSird.PLUS_D.kSigrLower), aParam[T_SIRD_EVAL::kPlus].d_Cr) == FALSE) { ASSERT_ONCE(FALSE); return; }
	if (Get_Cr((m_param->tSigr.nSubType == T_SIGR_D::kLower ? m_param->tSird.kSigr : m_param->tSird.MINUS_D.kSigrLower), aParam[T_SIRD_EVAL::kMinus].d_Cr) == FALSE) { ASSERT_ONCE(FALSE); return; }
	//r：교각의 항복강성에 대한 항복후의 2차 강성비율
	if (Get_r((m_param->tSigr.nSubType == T_SIGR_D::kLower ? m_param->tSird.kSigr : m_param->tSird.PLUS_D.kSigrLower), aParam[T_SIRD_EVAL::kPlus].d_r) == FALSE) { ASSERT_ONCE(FALSE); return; }
	if (Get_r((m_param->tSigr.nSubType == T_SIGR_D::kLower ? m_param->tSird.kSigr : m_param->tSird.MINUS_D.kSigrLower), aParam[T_SIRD_EVAL::kMinus].d_r) == FALSE) { ASSERT_ONCE(FALSE); return; }

	CArray<double, double> aTimeStep;
	int nSize = m_pDoc->m_pAttrCtrl->GetTimeStep(pDisp->kThis, aTimeStep);

	double dTimeStt = -DBL_MAX, dTiemEnd = DBL_MAX;
	if (m_param->tSird.bTimeStepRange == TRUE) {
		m_pDoc->m_pAttrCtrl2->GetQSird()->GetTimeRange(m_param->kSird, pDisp->kThis, dTimeStt, dTiemEnd);
	}

	BOOL bMinus = FALSE;
	int nMaxTimeIdx = 0;
	double dMaxMu_r = -DBL_MAX;
	for (int nTimeIdx = 0; nTimeIdx < nSize; nTimeIdx++)
	{
		const double& dTimeStep = aTimeStep[nTimeIdx];
		if (dTimeStep < dTimeStt) continue;
		if (dTiemEnd < dTimeStep) {
			break;
		}

		T_SIRD_EVAL_DISP disp;
		disp.kThis = pDisp->kThis;

		//δr 교량 전체계의 잔류변위(m)
		if (CalcDelta_r(pEval, nTimeIdx, aParam, &disp) == FALSE)
			continue;

		double dRatio = (disp.dDelta_max < 0) ? 
			dgn::SafeDiv(abs(disp.dDelta_r), abs(aParam[T_SIRD_EVAL::kMinus].dDelta_ra)) :
			dgn::SafeDiv(abs(disp.dDelta_r), abs(aParam[T_SIRD_EVAL::kPlus].dDelta_ra));
	
		if (abs(disp.dMu_r) > dMaxMu_r) {
			dMaxMu_r = abs(disp.dMu_r);
			disp.dRatio = dRatio;
			nMaxTimeIdx = nTimeIdx;
			bMinus = disp.dDelta_max < 0;
			bMinus ? disp.dDelta_ra = aParam[T_SIRD_EVAL::kMinus].dDelta_ra : disp.dDelta_ra = aParam[T_SIRD_EVAL::kPlus].dDelta_ra;
			pDisp->Copy(disp);
		}

		if (m_pProgress->NextStep() == FALSE) throw FALSE;
	}

	pDisp->nDirAnal = bMinus ? seis_jp::kMinus : seis_jp::kPlus;
}

BOOL CSeismicDesignSird::CalcDelta_ra(const T_SIRD_RSDP_D* pRsdp, OUT double& dDelta_r)
{
	T_SIGR_K kSigr = 0;
	if (m_param->tSigr.nSubType == T_SIGR_D::kLower)
		kSigr = m_param->tSird.kSigr;
	else
		kSigr = pRsdp->kSigrLower;

	//"허용잔류변위 산출 교각"에서 지정한 하부구조그룹의 교각기부로부터 상부구조 관성력 작용위치까지 높이 h 의 1/100
	double d_h = 0;
	if (Get_h(kSigr, pRsdp->strNameCOLM, d_h) == FALSE) return FALSE;

	dDelta_r = dgn::SafeDiv(d_h, 100.0);
	return TRUE;
}

BOOL CSeismicDesignSird::Get_h(const T_SIGR_LOWER_K& kSigrLower, const CString& strNameCOLM, OUT double& d_h)
{
	T_SIGR_LOWER_D tSigrLower;
	if (m_pDoc->m_pAttrCtrl2->GetQSigrLower()->Get(kSigrLower, tSigrLower) == FALSE) return FALSE;

	T_NODE_K kPierTopNode = (m_param->tSipa.nSeismicForceDirection == T_SIPA_D::kAxis_Dir) ? 
		tSigrLower.kNodeAxis : tSigrLower.kNodePerp;

	ColumnData colm;
	if (m_pDoc->m_pAttrCtrl2->GetQSigrLower()->FindColumnLowNode(kSigrLower, strNameCOLM, colm) == FALSE) return FALSE;

	T_NODE_D dNodeLow;
	if (m_pDoc->m_pAttrCtrl->GetNode(colm.kNodeLow, dNodeLow) == FALSE) return FALSE;

	T_NODE_D dNodeTop;
	if (m_pDoc->m_pAttrCtrl->GetNode(kPierTopNode, dNodeTop) == FALSE) return FALSE;

	//연직거리를 구해야 함.
	d_h = dNodeTop.z - dNodeLow.z;
	return TRUE;
}

BOOL CSeismicDesignSird::CalcDelta_r(IN T_SIRD_EVAL* pEval, const int nTimeIdx, const T_SIRD_EVAL_DISP* aParam, OUT T_SIRD_EVAL_DISP* pDisp)
{
	// δmax：(전체계)거더 단부의 최대 응답변위(m)
	if (CalcDeltaMax(pEval, pDisp->kThis, nTimeIdx, pDisp->dDelta_max) == FALSE) return FALSE;

	const T_SIRD_EVAL_DISP& param = (pDisp->dDelta_max < 0) ? aParam[T_SIRD_EVAL::kMinus] : aParam[T_SIRD_EVAL::kPlus];

	pDisp->dDelta_y = param.dDelta_y;
	pDisp->d_Cr = param.d_Cr;
	pDisp->d_r = param.d_r;

	//μr = δmax/δy (최대 응답 소성율)
	pDisp->dMu_r = dgn::SafeDiv(abs(pDisp->dDelta_max), abs(pDisp->dDelta_y));

	if (pDisp->dMu_r < 1) return TRUE;

	//δr=Cr(μr-1)(1-r)δy
	pDisp->dDelta_r = pDisp->d_Cr * (pDisp->dMu_r - 1) * (1 - pDisp->d_r) * pDisp->dDelta_y;

	return TRUE;
}

void CSeismicDesignSird::SetDataEval(OUT T_SIRD_EVAL* pEval)
{
	// 교각별 조사설정 UI의 경우 +방향, -방향의 교각과 기둥은 항상 같은 값을 가진다.
	ASSERT(m_param->tSird.PLUS_D.kSigrLower == m_param->tSird.MINUS_D.kSigrLower);
	ASSERT(m_param->tSird.PLUS_D.strNameCOLM == m_param->tSird.MINUS_D.strNameCOLM);

	T_SIGR_LOWER_D dSigrLower;
	m_pDoc->m_pAttrCtrl2->GetQSigrLower()->Get(m_param->tSird.PLUS_D.kSigrLower, dSigrLower);

	pEval->nDirection = T_SIRD_EVAL::kDirMax;
	pEval->strName = m_param->tSird.PLUS_D.strNameCOLM;
	pEval->kSigrLower = m_param->tSird.PLUS_D.kSigrLower;
	pEval->kPierTopNode = (m_param->tSipa.nSeismicForceDirection == T_SIPA_D::kAxis_Dir) ? dSigrLower.kNodeAxis : dSigrLower.kNodePerp;
}

BOOL CSeismicDesignSird::Get_Cr(const T_SIGR_LOWER_K& kSigrLower, OUT double& d_Cr)
{
	switch (m_param->tSipa.nDesignCode)
	{
	case BrdgSE_CodeJP::JROAD5_H14:
		return CSeismicDesignSird_H14::Get_Cr(kSigrLower, d_Cr);
		break;
	case BrdgSE_CodeJP::JROAD5_H24:
	case BrdgSE_CodeJP::JROAD5_H29:
	case BrdgSE_CodeJP::JROAD5_R07:
		return CSeismicDesignSird_H24::Get_Cr(kSigrLower, d_Cr);
		break;
	}
	return FALSE;
}

BOOL CSeismicDesignSird::Get_r(const T_SIGR_LOWER_K& kSigrLower, OUT double& d_r)
{
	switch (m_param->tSipa.nDesignCode)
	{
	case BrdgSE_CodeJP::JROAD5_H14:
		return CSeismicDesignSird_H14::Get_r(kSigrLower, d_r);
		break;
	case BrdgSE_CodeJP::JROAD5_H24:
	case BrdgSE_CodeJP::JROAD5_H29:
	case BrdgSE_CodeJP::JROAD5_R07:
		return CSeismicDesignSird_H24::Get_r(kSigrLower, d_r);
		break;
	}
	return FALSE;
}
#pragma endregion