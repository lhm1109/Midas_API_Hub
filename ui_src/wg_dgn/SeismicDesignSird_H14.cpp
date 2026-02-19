#include "stdafx.h"
#include "SeismicDesignSird_H14.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_PostCtrl.h"
#include "..\wg_db\wg_db_ThisInfo.h"
#include "..\wg_db\wg_db_AttrCtrl2.h"
#include "..\wg_db\DB_QUERY_SEIS_CVL_JP.h"
#include "..\wg_db\BrdgSeisEvalResult_JP.h"

#include "BrdgSeisEval_JP.h"

#pragma region // CSeismicDesignSird_H14

void CSeismicDesignSird_H14::AddProgressStep(CDgnProgressDlg* pProgress)
{
	CSeismicDesignSird::AddStep(pProgress);
}

BOOL CSeismicDesignSird_H14::Calculate(CDgnProgressDlg* pProgress)
{
	m_pDoc = CDBDoc::GetDocPoint();
	ASSERT(m_pDoc);
	if (m_pDoc == nullptr) return FALSE;

	T_SIDC_D dSidc; dSidc.Initialize();
	if (m_pDoc->m_pAttrCtrl2->GetQSidc()->Get(dSidc) == FALSE) return FALSE;

	// Code 단위계 변환
	CUnitConversion unit(D_UNITSYS_FORCE_INDEX_N, D_UNITSYS_LENGTH_INDEX_M);

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
			CalculateUpper(pProgress, kSird);
		}
		else if (dSigr.nSubType == T_SIGR_D::kLower) {
			CalculateLower(pProgress, kSird);
		}
	}

	return FALSE;
}

void CSeismicDesignSird_H14::CalculateUpper(CDgnProgressDlg* pProgress, const T_SIRD_K& kSird)
{
	T_SIRD_D dSird; T_SIPA_D dSipa; T_SIGR_UPPER_D dSigrUpper;
	if (m_pDoc->m_pAttrCtrl2->GetQSird()->Get(kSird, dSird) == FALSE) return;
	if (m_pDoc->m_pAttrCtrl2->GetQSipa()->Get(dSird.kSipa, dSipa) == FALSE) return;
	if (m_pDoc->m_pAttrCtrl2->GetQSigrUpper()->Get(dSird.kSigr, dSigrUpper) == FALSE) return;

	//좌단 검토
	T_SIRD_EVAL evalLeft;
	evalLeft.kSird = kSird;
	evalLeft.kSipa = dSird.kSipa;
	evalLeft.kSigr = dSird.kSigr;
	evalLeft.kGirderNode = dSigrUpper.kNodeL;
	evalLeft.nPosType = T_SIRD_EVAL::kLeft;
	evalLeft.nSeismicForceType = dSipa.nSeismicForceType;

	CSeismicDesignSird_H14_Upper SirdLeft(pProgress);
	SirdLeft.Calculate(kSird, &evalLeft);

	//우단 검토
	T_SIRD_EVAL evalRight;
	evalRight.kSird = kSird;
	evalRight.kSipa = dSird.kSipa;
	evalRight.kSigr = dSird.kSigr;
	evalRight.kGirderNode = dSigrUpper.kNodeR;
	evalRight.nPosType = T_SIRD_EVAL::kRight;
	evalRight.nSeismicForceType = dSipa.nSeismicForceType;

	CSeismicDesignSird_H14_Upper SirdRight(pProgress);
	SirdRight.Calculate(kSird, &evalRight);

	// save result
	CBrdgSeisEvalResult_JP* pResult = m_pDoc->m_pPostCtrl->GetBrdgSeisEvalResult_JP(); if (!pResult) { ASSERT(0); }
	pResult->SetResultSird(dSird.kSipa, dSird.kSigr, evalLeft.nPosType, evalLeft);
	pResult->SetResultSird(dSird.kSipa, dSird.kSigr, evalRight.nPosType, evalRight);
}

void CSeismicDesignSird_H14::CalculateLower(CDgnProgressDlg* pProgress, const T_SIRD_K& kSird)
{
	T_SIRD_D dSird; T_SIPA_D dSipa; T_SIGR_LOWER_D dSigrLower;
	if (m_pDoc->m_pAttrCtrl2->GetQSird()->Get(kSird, dSird) == FALSE) return;
	if (m_pDoc->m_pAttrCtrl2->GetQSipa()->Get(dSird.kSipa, dSipa) == FALSE) return;
	if (m_pDoc->m_pAttrCtrl2->GetQSigrLower()->Get(dSird.kSigr, dSigrLower) == FALSE) return;

	//교각별 잔류변위 조사
	T_SIRD_EVAL eval;
	eval.kSird = kSird;
	eval.kSipa = dSird.kSipa;
	eval.kSigr = dSird.kSigr;
	eval.nPosType = T_SIRD_EVAL::kLower;
	eval.nSeismicForceType = dSipa.nSeismicForceType;
	if (dSipa.nSeismicForceDirection == T_SIPA_D::kAxis_Dir)
		eval.kPierTopNode = dSigrLower.kNodeAxis;
	else if (dSipa.nSeismicForceDirection == T_SIPA_D::kPerp_Dir)
		eval.kPierTopNode = dSigrLower.kNodePerp;
	else { ASSERT(FALSE); return; }

	CSeismicDesignSird_H14_Lower Sird(pProgress);
	Sird.Calculate(kSird, &eval);

	// save result
	CBrdgSeisEvalResult_JP* pResult = m_pDoc->m_pPostCtrl->GetBrdgSeisEvalResult_JP(); if (!pResult) { ASSERT(0); }
	pResult->SetResultSird(dSird.kSipa, dSird.kSigr, eval.nPosType, eval);
}

BOOL CSeismicDesignSird_H14::Get_Cr(const T_SIGR_LOWER_K& kSigrLower, OUT double& d_Cr)
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	ASSERT(pDoc);
	if (pDoc == nullptr) return FALSE;

	T_SIGR_LOWER_D dSigrLower;
	if (pDoc->m_pAttrCtrl2->GetQSigrLower()->Get(kSigrLower, dSigrLower) == FALSE) return FALSE;

	switch (dSigrLower.nMaterialType)
	{
	case T_SIGR_LOWER_D::eMaterialType::kRC:
		d_Cr = 0.6;
		break;
	case T_SIGR_LOWER_D::eMaterialType::kSteel:
		d_Cr = 0.45;
		break;
	case T_SIGR_LOWER_D::eMaterialType::kSRC:
		d_Cr = 0.35;
		break;
	default:
		return FALSE;
		break;
	}

	return TRUE;
}

BOOL CSeismicDesignSird_H14::Get_r(const T_SIGR_LOWER_K& kSigrLower, OUT double& d_r)
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	ASSERT(pDoc);
	if (pDoc == nullptr) return FALSE;

	T_SIGR_LOWER_D dSigrLower;
	if (pDoc->m_pAttrCtrl2->GetQSigrLower()->Get(kSigrLower, dSigrLower) == FALSE) return FALSE;

	switch (dSigrLower.nMaterialType)
	{
	case T_SIGR_LOWER_D::eMaterialType::kRC:
		d_r = 0.0;
		break;
	case T_SIGR_LOWER_D::eMaterialType::kSteel:
		d_r = 0.2;
		break;
	case T_SIGR_LOWER_D::eMaterialType::kSRC:
		d_r = 0.05;
		break;
	default:
		return FALSE;
		break;
	}

	return TRUE;
}
#pragma endregion

#pragma region // CSeismicDesignSird_H14_Upper

CSeismicDesignSird_H14_Upper::CSeismicDesignSird_H14_Upper(CDgnProgressDlg* pProgress) : CSeismicDesignSird_H14()
{
	m_pProgress = pProgress;
}

BOOL CSeismicDesignSird_H14_Upper::Calculate(const T_SIRD_K& kSird, T_SIRD_EVAL* pEval)
{
	return CSeismicDesignSird::Calculate(kSird, pEval);
}

BOOL CSeismicDesignSird_H14_Upper::CalcDeltaMax(IN T_SIRD_EVAL* pEval, const T_THIS_K& kThis, const int nTimeIdx, OUT double& rDeltaMax)
{
	T_SIGR_UPPER_D tSigrUpper;
	if (m_pDoc->m_pAttrCtrl2->GetQSigrUpper()->Get(m_param->kSigr, tSigrUpper) == FALSE) return FALSE;

	CSeismicDesignDisp_Upper DispU; DispU.SetProgressDlg(m_pProgress);
	double aDisp[eVector2d::kMax2d] = { 0.0, 0.0 };
	if (DispU.CalcDeltaMax(pEval->kGirderNode, kThis, nTimeIdx, aDisp) == FALSE) return FALSE;

	ParamAngleOption param;
	param.kThis = kThis;
	param.kElem = tSigrUpper.aElements[0];
	param.nDirDisp = m_param->tSird.nDirDisp;
	param.nShearDir = tSigrUpper.nDirShear;
	param.dUserAngle = m_param->tSird.dAngle;
	param.nSeismicForceDirection = m_param->tSipa.nSeismicForceDirection;

	double dCompVal = 0;//합성벡터
	GetDispOfAngle(param, aDisp[eVector2d::kDx], aDisp[eVector2d::kDy], dCompVal);

	rDeltaMax = dCompVal;
	return TRUE;
}

BOOL CSeismicDesignSird_H14_Upper::CalcDelta_y(const T_THIS_K& kThis, T_SIRD_RSDP_D* pRsdp, OUT double& rDeltaY)
{
	if (m_param->tSird.nAlwCalcType == seis_jp::kUserInput) {
		rDeltaY = pRsdp->dLengthDeltaVar;
		if (pRsdp->nDirAnal == seis_jp::kMinus)
			rDeltaY *= -1;
		return TRUE;
	}

	T_NODE_K kNode;
	seis_jp::eDirAnal eDir = (seis_jp::eDirAnal)pRsdp->nDirAnal;
	if (m_pDoc->m_pAttrCtrl2->GetQSird()->GetInertialForceNode(m_param->kSird, eDir, kNode) == FALSE) return FALSE;

	return GetDispFromAnalsys(kThis, pRsdp, kNode, rDeltaY);
}

BOOL CSeismicDesignSird_H14_Upper::GetDispFromAnalsys(const T_THIS_K& kThis, const T_SIRD_RSDP_D* pRsdp, const T_NODE_K& kNode, OUT double& dDisp)
{
	m_pDoc->m_pPostCtrl->SelectThis(pRsdp->kThis);

	int nTimeIndex = 0;
	if (GetTimeIndex(pRsdp->kThis, pRsdp->nStepDeltaY, nTimeIndex) == FALSE) return FALSE;

	T_DISP_D disp;
	if (m_pDoc->m_pPostCtrl->GetDispHistory(nTimeIndex, kNode, disp) == FALSE) return FALSE;

	double v[3] = { 0., };
	v[0] = disp.dblDisp[DISP_COMP_DX];
	v[1] = disp.dblDisp[DISP_COMP_DY];
	v[2] = disp.dblDisp[DISP_COMP_DZ];

	int nShearDir = 0;
	T_ELEM_K_LIST elemKeys;
	T_SIRD_D sird; T_SIGR_D sigr;
	if (m_pDoc->m_pAttrCtrl2->GetQSird()->Get(m_param->kSird, sird) == FALSE) return FALSE;
	if (m_pDoc->m_pAttrCtrl2->GetQSigr()->Get(sird.kSigr, sigr) == FALSE) return FALSE;
	switch (sigr.nSubType)
	{
	case T_SIGR_D::kUpper:
	{
		T_SIGR_UPPER_D dSigrUpper;
		if (m_pDoc->m_pAttrCtrl2->GetQSigrUpper()->Get(sird.kSigr, dSigrUpper) == FALSE) return FALSE;
		nShearDir = dSigrUpper.nDirShear;
		elemKeys.Copy(dSigrUpper.aElements);
	}
	break;
	case T_SIGR_D::kLower:
	{
		T_SIGR_LOWER_D dSigrLower;
		if (m_pDoc->m_pAttrCtrl2->GetQSigrLower()->Get(sird.kSigr, dSigrLower) == FALSE) return FALSE;
		if (dSigrLower.COLM_D.GetCount() < 1) return FALSE;
		m_pDoc->m_pAttrCtrl2->GetQSigrLower()->GetNodeKeysOfColumn(sird.kSigr, elemKeys);
		nShearDir = dSigrLower.COLM_D[0].nDirShear;
	}
	break;
	default:
		break;
	}

	if (elemKeys.GetCount() < 1) return FALSE;

	ParamAngleOption param;
	param.kThis = kThis;
	param.kElem = elemKeys[0];
	param.nDirDisp = sird.nDirDisp;
	param.nShearDir = nShearDir;
	param.dUserAngle = sird.dAngle;
	param.nSeismicForceDirection = m_param->tSipa.nSeismicForceDirection;
	double dGlobal_Dx = v[0];
	double dGlobal_Dy = v[1];

	GetDispOfAngle(param, dGlobal_Dx, dGlobal_Dy, dDisp);

	return TRUE;
}
#pragma endregion

#pragma region // CSeismicDesignSird_H14_Lower

CSeismicDesignSird_H14_Lower::CSeismicDesignSird_H14_Lower(CDgnProgressDlg* pProgress) : CSeismicDesignSird_H14()
{
	m_PLower = nullptr;
	m_pProgress = pProgress;
	m_DispL.SetProgressDlg(pProgress);
}

BOOL CSeismicDesignSird_H14_Lower::Calculate(const T_SIRD_K& kSird, T_SIRD_EVAL* pEval)
{
	return CSeismicDesignSird::Calculate(kSird, pEval);
}

BOOL CSeismicDesignSird_H14_Lower::CalcDeltaMax(IN T_SIRD_EVAL* pEval, const T_THIS_K& kThis, const int nTimeIdx, OUT double& rDeltaMax)
{
	ASSERT(m_param->tSird.PLUS_D.strNameCOLM == m_param->tSird.MINUS_D.strNameCOLM);
	// 교각별 조사의 경우 +방향, -방향의 기둥은 항상 같은 값을 가진다.
	P_SIRD_Lower PLower(m_param, &m_param->tSird.PLUS_D);
	if (PLower.IsValidValue() == FALSE) return FALSE;

	ParamAngleOption param;
	param.kThis = kThis;
	param.kElem = PLower.tLowColmData.Column.aInvestigatedElem[0];
	param.nDirDisp = m_param->tSird.nDirDisp;
	param.nShearDir = PLower.tLowColmData.Column.nDirShear;
	param.dUserAngle = m_param->tSird.dAngle;
	param.nSeismicForceDirection = m_param->tSipa.nSeismicForceDirection;

	PDeltaVal tParam = std::make_tuple(	m_param->tSird.kSigr, 
										kThis, 
										PLower.kPierTopNode, 
										(seis_jp::eDirAnal)PLower.pRsdp->nDirAnal,
										m_param->tSird.nRspCalcType, 
										&PLower.tLowColmData);

	doubleArray aDeltaMax; aDeltaMax.SetSize(1);
	if (m_DispL.CalcDeltaMax(tParam, param, nTimeIdx, aDeltaMax) == FALSE) return FALSE;

	BOOL bResult = FALSE;
	switch (m_param->tSird.nRspCalcType)
	{
	case seis_jp::kJBEC:
		if (aDeltaMax.GetCount() > seis_jp::kDmaxJbec) {
			rDeltaMax = aDeltaMax[seis_jp::kDmaxJbec];
			bResult = TRUE;
		}
		break;
	case seis_jp::kNEXCO:
		if (aDeltaMax.GetCount() > seis_jp::kDmaxNexco) {
			rDeltaMax = aDeltaMax[seis_jp::kDmaxNexco];
			bResult = TRUE;
		}
		break;
	}
	return bResult;
}

BOOL CSeismicDesignSird_H14_Lower::CalcDelta_y(const T_THIS_K& kThis_Seismic, T_SIRD_RSDP_D* pRsdp, OUT double& rDeltaY)
{
	P_SIRD_Lower PLower(m_param, pRsdp);
	if (PLower.IsValidValue() == FALSE) return FALSE;
	m_PLower = &PLower;

	BOOL bResult = FALSE;

	switch (m_param->tSird.nAlwCalcType)
	{
	case seis_jp::kUserInput:
		rDeltaY = pRsdp->dLengthDeltaVar;
		if (pRsdp->nDirAnal == seis_jp::kMinus)
			rDeltaY *= -1;
		return TRUE;

	case seis_jp::kAnalysisResult:
		bResult = CalcDeltaY_fromAnalysis(kThis_Seismic, pRsdp, rDeltaY);
		break;

	case seis_jp::kMphi_Dy:
		bResult = CalcDeltaY_fromkMphi_Dy(pRsdp, rDeltaY);
		break;

	case seis_jp::kMphi_Dpy:
		bResult = CalcDeltaY_fromkMphi_Dpy(pRsdp, rDeltaY);
		break;

	default:
		ASSERT_ONCE(FALSE);
		break;
	}

	return bResult;
}

BOOL CSeismicDesignSird_H14_Lower::CalcDeltaY_fromAnalysis(const T_THIS_K& kThis_Seismic, const T_SIRD_RSDP_D* pRsdp, OUT double& rDeltaY)
{
	PDeltaVal deltaParam = std::make_tuple(	m_param->kSigr, 
											pRsdp->kThis, 
											m_PLower->kPierTopNode, 
											(seis_jp::eDirAnal)pRsdp->nDirAnal,
											m_param->tSird.nRspCalcType, 
											&m_PLower->tLowColmData);

	// δy = δry-δfd-Θfp×h-δs
	double dDelta_y[eVector2d::kMax2d] = { 0.0, 0.0 };
	if (m_DispL.CalcDeltaValAtStep(deltaParam, pRsdp->nStepDeltaY, dDelta_y[eVector2d::kDx], dDelta_y[eVector2d::kDy]) == FALSE) return FALSE;

	ParamAngleOption param;
	param.kThis = kThis_Seismic;//pRsdp->kThis;
	param.kElem = m_PLower->tCOLM.aInvestigatedElem[0];
	param.nDirDisp = m_param->tSird.nDirDisp;
	param.nShearDir = m_PLower->tCOLM.nDirShear;
	param.dUserAngle = m_param->tSird.dAngle;
	param.nSeismicForceDirection = m_param->tSipa.nSeismicForceDirection;

	double dCompVal = 0;//합성벡터
	GetDispOfAngle(param, dDelta_y[eVector2d::kDx], dDelta_y[eVector2d::kDy], dCompVal);

	rDeltaY = dCompVal;
	return TRUE;
}

BOOL CSeismicDesignSird_H14_Lower::CalcDeltaY_fromkMphi_Dy(const T_SIRD_RSDP_D* pRsdp, OUT double& rDeltaY)
{
	BOOL bResult = FALSE;

	if (m_PLower->tSigrLower.nMaterialType == T_SIGR_LOWER_D::kRC)
		bResult = CalcDeltaA_fromkMphi_Dy_forRc(pRsdp, rDeltaY);
	else if (m_PLower->tSigrLower.nMaterialType == T_SIGR_LOWER_D::kSteel)
		bResult = CalcDeltaA_fromkMphi_Dy_forSteel(pRsdp, rDeltaY);

	return bResult;
}

BOOL CSeismicDesignSird_H14_Lower::CalcDeltaA_fromkMphi_Dy_forRc(const T_SIRD_RSDP_D* pRsdp, OUT double& rDeltaY)
{
	// engineers studioではφy0はmφから算出ではなく、鉄筋降伏条件から直接算出するが、Civil本体開発がいるため今は対応しない。(h24,29も共通)
	// 최대응답변위와 같이 M-φ관계로부터 산출
	PDeltaYandU PDy = std::make_tuple(	m_param->kSipa, 
										m_param->kSigr, 
										m_PLower->kPierTopNode, 
										pRsdp->strNameCOLM, 
										(seis_jp::eDirAnal)pRsdp->nDirAnal,
										m_param->tSipa.nSeismicForceDirection, 
										&m_PLower->tLowColmData);

	// δy
	double dDeltaY = 0.0, dDeltaU = 0.0;
	if (m_DispL.GetDeltaYandU(PDy, dDeltaY, dDeltaU) == FALSE) return FALSE;

	rDeltaY = dDeltaY;
	return TRUE;
}

BOOL CSeismicDesignSird_H14_Lower::CalcDeltaA_fromkMphi_Dy_forSteel(const T_SIRD_RSDP_D* pRsdp, OUT double& rDeltaY)
{
	double d_h = 0;
	//검토 대상을 찾아 관성력 작용위치까지의 높이와 제일 아래에 있는 요소를 찾아온다.
	if (GetH(m_PLower->kPierTopNode, m_PLower->tLowColmData.kNodeLow, d_h) == FALSE) return FALSE;

	//교각 기부의 비선형특성(M-φ관계)로부터 상태 단계별 수평변위를 가정하여, 요소별 적분의 누적에 의해 상부공 관성력 작용위치의 변위 산출
	PDeltaYandU PDy = std::make_tuple(	m_param->kSipa, 
										m_param->kSigr, 
										m_PLower->kPierTopNode, 
										pRsdp->strNameCOLM, 
										(seis_jp::eDirAnal)pRsdp->nDirAnal,
										m_param->tSipa.nSeismicForceDirection, 
										&m_PLower->tLowColmData);

	// δy
	double dDeltaY = 0.0, dDeltaU = 0.0;
	if (m_DispL.GetDeltaYandU(PDy, dDeltaY, dDeltaU) == FALSE) return FALSE;

	// (강제교각) δy：柱基部가'φy'에 도달할 때의 곡률분포를 적분해서 구한 값
	rDeltaY = dDeltaY;
	return TRUE;
}

BOOL CSeismicDesignSird_H14_Lower::CalcDeltaY_fromkMphi_Dpy(const T_SIRD_RSDP_D* pRsdp, OUT double& rDeltaY)
{
	// 최대응답변위 CSeismicDesignSimd_H14_Lower::CalcDeltaA_fromkMphi_Dpy() 와 같으나 구현되어 있지 않음
	return FALSE;
}
#pragma endregion