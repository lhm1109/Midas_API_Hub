#include "stdafx.h"
#include "SeismicDesignSiet.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_PostCtrl.h"
#include "..\wg_db\wg_db_ThisInfo.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl2.h"
#include "..\wg_db\DB_QUERY_SEIS_CVL_JP.h"
#include "..\wg_db\BrdgSeisEvalResult_JP.h"

#include "DgnProgressDlg.h"
#include "BrdgSeisEval_JP.h"

#define RESULT_TYPE_SELECTION_FORCE 1;

#pragma region >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> CSeismicDesignSiet 
CSeismicDesignSiet::CSeismicDesignSiet(void) : CSeismicDesignBase()
{
}

CSeismicDesignSiet::~CSeismicDesignSiet(void)
{
}

void CSeismicDesignSiet::AddProgressStep(CDgnProgressDlg* pProgress)
{
	T_KEY_LIST sietKeys;
	m_pDoc->m_pAttrCtrl2->GetQSiet()->GetKeyList(sietKeys);

	for (int idx = 0; idx < sietKeys.GetCount(); idx++)
	{
		const T_SIET_K& kSiet = sietKeys[idx];

		T_SIET_D dSiet; T_SIPA_D dSipa;
		if (m_pDoc->m_pAttrCtrl2->GetQSiet()->Get(kSiet, dSiet) == FALSE) return;
		if (m_pDoc->m_pAttrCtrl2->GetQSipa()->Get(dSiet.kSipa, dSipa) == FALSE) return;
		if (dSipa.nDesignCode != GetCurrentDgnCode()) return;

		CSeismicDesignSiet_Bearing(pProgress).AddProgressStep(kSiet);
		CSeismicDesignSiet_PierBase(pProgress).AddProgressStep(kSiet);
	}
}

BOOL CSeismicDesignSiet::Calculate(CDgnProgressDlg* pProgress)
{
	if (m_pDoc == NULL) return FALSE;

	// Code 단위계 변환
	CUnitConversion unit(D_UNITSYS_FORCE_INDEX_KN, D_UNITSYS_LENGTH_INDEX_M);

	T_KEY_LIST sietKeys;
	m_pDoc->m_pAttrCtrl2->GetQSiet()->GetKeyList(sietKeys);

	for (int idx = 0; idx < sietKeys.GetCount(); idx++)
	{
		T_SIET_K kSiet = sietKeys[idx];

		T_SIET_D dSiet; T_SIPA_D dSipa;
		if (m_pDoc->m_pAttrCtrl2->GetQSiet()->Get(kSiet, dSiet) == FALSE) continue;
		if (m_pDoc->m_pAttrCtrl2->GetQSipa()->Get(dSiet.kSipa, dSipa) == FALSE) continue;
		if (dSipa.nDesignCode != GetCurrentDgnCode()) continue;

		Invoke(pProgress, kSiet);
	}

	return TRUE;
}

void CSeismicDesignSiet::Invoke(CDgnProgressDlg* pProgress, const T_SIET_K& kSiet)
{
	//베어링의 단면력
	CSeismicDesignSiet_Bearing(pProgress).Calculate(kSiet);

	//교각 기부의 단면력 (3파 평균)
	CSeismicDesignSiet_PierBase(pProgress).Calculate(kSiet);
}
#pragma endregion <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

#pragma region >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> CSeismicDesignSiet_Bearing 
CSeismicDesignSiet_Bearing::CSeismicDesignSiet_Bearing(CDgnProgressDlg* pProgress) : CSeismicDesignSiet()
{
	m_pProgress = pProgress;
}

CSeismicDesignSiet_Bearing::~CSeismicDesignSiet_Bearing(void)
{
}

void CSeismicDesignSiet_Bearing::AddProgressStep(const T_SIET_K& kSiet)
{
	if (m_pDoc == nullptr) return;
	if (m_pProgress == nullptr) return;

	T_SIET_D dSiet; T_SIPA_D dSipa; T_SIGR_LOWER_D dSigrLower;
	if (m_pDoc->m_pAttrCtrl2->GetQSiet()->Get(kSiet, dSiet) == FALSE) return;
	if (m_pDoc->m_pAttrCtrl2->GetQSipa()->Get(dSiet.kSipa, dSipa) == FALSE) return;
	if (dSipa.nDesignCode != GetCurrentDgnCode()) return;
	if (m_pDoc->m_pAttrCtrl2->GetQSigrLower()->Get(dSiet.kSigrLower, dSigrLower) == FALSE) return;
	if (dSigrLower.BEAR_D.GetCount() < 1) return;

	m_pProgress->m_nTotalSteps++;
}

BOOL CSeismicDesignSiet_Bearing::Calculate(const T_SIET_K& kSiet)
{
	if (m_pDoc == NULL) return FALSE;

	T_SIET_D dSiet; T_SIPA_D dSipa; T_SIGR_LOWER_D dSigrLower;
	if (m_pDoc->m_pAttrCtrl2->GetQSiet()->Get(kSiet, dSiet) == FALSE) return FALSE;
	if (m_pDoc->m_pAttrCtrl2->GetQSipa()->Get(dSiet.kSipa, dSipa) == FALSE) return FALSE;
	if (dSipa.nDesignCode != GetCurrentDgnCode()) return FALSE;
	if (m_pDoc->m_pAttrCtrl2->GetQSigrLower()->Get(dSiet.kSigrLower, dSigrLower) == FALSE) return FALSE;
	if (dSigrLower.BEAR_D.GetCount() < 1) return FALSE;

	m_kSiet = kSiet;
	m_pSiet = &dSiet;
	m_pSipa = &dSipa;
	m_pSigrLower = &dSigrLower;

	switch (dSiet.nOutTypeBearing)
	{
	case T_SIET_D::kMinMax_Br:
		GetBearingForceMax();
		break;
	case T_SIET_D::kConcurvity_Each_Br:
		GetConcurvityEachBr();
		break;
	case T_SIET_D::kConcurvity_Line_Br:
		GetConcurvityLineBr();
		break;
	default:
		ASSERT(FALSE);
		break;
	}

	if (m_pProgress->NextStep() == FALSE) throw FALSE;

	return TRUE;
}

void CSeismicDesignSiet_Bearing::GetBearingForceMax()
{
	T_SIET_EVAL_BEAR_MINMAX eval;
	eval.kSipa = m_pSiet->kSipa;
	eval.kSigrLower = m_pSiet->kSigrLower;
	eval.nSeismicForceType = m_pSipa->nSeismicForceType;
	eval.nSeismicForceDirection = m_pSipa->nSeismicForceDirection;

	std::map<int, T_SIET_EVAL_BEAR_LINE> mBearLine;

	for (int i = 0; i < m_pSigrLower->BEAR_D.GetCount(); i++)
	{
		const T_SIGR_LOWER_BEAR_D& bear = m_pSigrLower->BEAR_D[i];
		if (bear.nType == T_SIGR_LOWER_BEAR_D::kRigidLink) continue;
		if (bear.aPosition.GetCount() < 2) { ASSERT(bear.aPosition.GetCount() == 2); continue; }

		T_SIET_EVAL_BEAR_GROUP group;
		group.strName = bear.strName;
		group.kNode_i = bear.aPosition[0];
		group.kNode_j = bear.aPosition[1];
		group.nType = bear.nType;
		group.nDirShear = bear.nDirShear;
		group.strComp = _T("--");

		T_SIET_EVAL_NLNK_FORCE avergeMax, avergeMin;

		int nSize = m_pSipa->aSeisThisK.GetCount();
		for (int j = 0; j < nSize; j++)
		{
			const T_THIS_K& kThis = m_pSipa->aSeisThisK[j];

			T_SIET_EVAL_NLNK_FORCE max, min;
			if (GetMinMaxForce(kThis, &eval, &group, &max, &min) == FALSE) continue;

			avergeMax += max;
			avergeMin += min;
		}

		avergeMax /= nSize;
		avergeMin /= nSize;

		group.Force.dHmax = (abs(avergeMax.dHmax) > abs(avergeMin.dHmax)) ? abs(avergeMax.dHmax) : abs(avergeMin.dHmax);
		group.Force.dVmax = (abs(avergeMax.dVmax) > abs(avergeMin.dVmax)) ? avergeMax.dVmax : avergeMin.dVmax;
		group.Force.dUmax = (abs(avergeMax.dUmax) > abs(avergeMin.dUmax)) ? avergeMax.dUmax : avergeMin.dUmax;

		if (mBearLine.find(bear.nLineNum) == mBearLine.end()) {
			T_SIET_EVAL_BEAR_LINE line;
			line.nLineNum = bear.nLineNum;
			line.SumForce.dHmax += group.Force.dHmax;
			line.SumForce.dVmax += group.Force.dVmax;
			line.SumForce.dUmax = group.Force.dUmax;
			line.aGroup.Add(group);
			mBearLine.insert(std::make_pair(bear.nLineNum, line));
		}
		else {
			mBearLine[bear.nLineNum].SumForce.dHmax += group.Force.dHmax;
			mBearLine[bear.nLineNum].SumForce.dVmax += group.Force.dVmax;
			if (group.Force.dUmax > mBearLine[bear.nLineNum].SumForce.dUmax)
				mBearLine[bear.nLineNum].SumForce.dUmax = group.Force.dUmax;
			mBearLine[bear.nLineNum].aGroup.Add(group);
		}
	}

	if (mBearLine.find(T_SIGR_LOWER_BEAR_D::kLine1) != mBearLine.end())
		eval.Line1 = mBearLine[T_SIGR_LOWER_BEAR_D::kLine1];
	if (mBearLine.find(T_SIGR_LOWER_BEAR_D::kLine2) != mBearLine.end())
		eval.Line2 = mBearLine[T_SIGR_LOWER_BEAR_D::kLine2];

	// save result
	CBrdgSeisEvalResult_JP* pResult = m_pDoc->m_pPostCtrl->GetBrdgSeisEvalResult_JP(); if (!pResult) { ASSERT(0); return; }
	pResult->SetResultSietBearMinMax(m_pSiet->kSipa, m_pSiet->kSigrLower, eval);
}

BOOL CSeismicDesignSiet_Bearing::GetMinMaxForce(const T_THIS_K& kThis, const T_SIET_EVAL_BEAR_MINMAX* pEval, const T_SIET_EVAL_BEAR_GROUP* pGroup, OUT T_SIET_EVAL_NLNK_FORCE* pMax, OUT T_SIET_EVAL_NLNK_FORCE* pMin)
{
	int nSeisForceDir = m_pSipa->nSeismicForceDirection;
	int nDir = pGroup->nDirShear;

	const int nHmax = [nSeisForceDir, nDir]()
		{
			int nShearForceDir = (nSeisForceDir == T_SIPA_D::kAxis_Dir) ? nDir : (nDir == seis_jp::kY_Dir ? seis_jp::kZ_Dir : seis_jp::kY_Dir);
			switch (nShearForceDir)
			{
			case seis_jp::kY_Dir: return STEK_SHEAR_Y;
			case seis_jp::kZ_Dir: return STEK_SHEAR_Z;
			default: ASSERT(0); return 2;
			}
		}();

	CArray<T_NLNK_K, T_NLNK_K> nlnkKeys;
	if (m_pDoc->m_pAttrCtrl->GetNlnkKeyListBetween2Nodes(pGroup->kNode_i, pGroup->kNode_j, nlnkKeys) == FALSE) return FALSE;
	if (nlnkKeys.GetCount() < 1) { ASSERT(nlnkKeys.GetCount() == 1); return FALSE; }

	m_pDoc->m_pPostCtrl->SelectThis(kThis);

	T_STEK_D stekMax[2], stekMin[2];
	if (m_pDoc->m_pPostCtrl->GetThisInfo()->GetNlnkWithTime(nlnkKeys[0], &stekMax[0], &stekMin[0], 0, &stekMax[1], &stekMin[1], 0) == FALSE) return FALSE;

	//항상 I,J단의 부재력은 같음
	ASSERT(stekMax[0].dblForce[STEK_POSI_I][nHmax] == stekMax[0].dblForce[STEK_POSI_J][nHmax]);
	ASSERT(stekMin[0].dblForce[STEK_POSI_I][nHmax] == stekMin[0].dblForce[STEK_POSI_J][nHmax]);
	pMax->dHmax = stekMax[0].dblForce[STEK_POSI_I][nHmax];
	pMin->dHmax = stekMin[0].dblForce[STEK_POSI_I][nHmax];

	ASSERT(stekMax[0].dblForce[STEK_POSI_I][STEK_AXIS] == stekMax[0].dblForce[STEK_POSI_J][STEK_AXIS]);
	ASSERT(stekMin[0].dblForce[STEK_POSI_I][STEK_AXIS] == stekMin[0].dblForce[STEK_POSI_J][STEK_AXIS]);
	pMax->dVmax = stekMax[0].dblForce[STEK_POSI_I][STEK_AXIS];
	pMin->dVmax = stekMin[0].dblForce[STEK_POSI_I][STEK_AXIS];

	return GetDispMinMax(nDir, nlnkKeys[0], pMax->dUmax, pMin->dUmax);
}

BOOL CSeismicDesignSiet_Bearing::GetDispMinMax(const int& nDir, const T_NLNK_K& kNlnk, OUT double& dUmax, OUT double& dUmin)
{
	T_DISP_D dispMax[2], dispMin[2];
	if (m_pDoc->m_pPostCtrl->GetNldfHistoryMaxMinAll(kNlnk, &dispMax[0], &dispMin[0], 0, &dispMax[1], &dispMin[1], 0) == FALSE) return FALSE;

	int nSeismicForceDirection = m_pSipa->nSeismicForceDirection;

	const int nUmax = [nSeismicForceDirection, nDir]()
		{
			int nShearForceDir = (nSeismicForceDirection == T_SIPA_D::kAxis_Dir) ? nDir : (nDir == seis_jp::kY_Dir ? seis_jp::kZ_Dir : seis_jp::kY_Dir);
			switch (nShearForceDir)
			{
			case seis_jp::kY_Dir: return DISP_COMP_DY;
			case seis_jp::kZ_Dir: return DISP_COMP_DZ;
			default: ASSERT(0); return 2;
			}
		}();

	dUmax = dispMax[0].dblDisp[nUmax];
	dUmin = dispMin[0].dblDisp[nUmax];

	return TRUE;
}

CString CSeismicDesignSiet_Bearing::GetBearingType(const T_SIPA_D* pSipa, int nType)
{
	CString strType = _T("");
	switch (nType)
	{
	case T_SIGR_LOWER_BEAR_D::kFixed: strType = _T("F"); break;
	case T_SIGR_LOWER_BEAR_D::kMovable:
		if (pSipa->nSeismicForceDirection == T_SIPA_D::kAxis_Dir)
			strType = _T("M");
		else if (pSipa->nSeismicForceDirection == T_SIPA_D::kPerp_Dir)
			strType = _T("F");
		break;
	case T_SIGR_LOWER_BEAR_D::kElastic: strType = _T("E"); break;
	case T_SIGR_LOWER_BEAR_D::kSeismicIsolation: strType = _T("E"); break;
	}
	return strType;
}

void CSeismicDesignSiet_Bearing::GetConcurvityEachBr()
{
	T_SIET_EVAL_BEAR_EACH eval;
	eval.kSipa = m_pSiet->kSipa;
	eval.kSigrLower = m_pSiet->kSigrLower;
	eval.nSeismicForceType = m_pSipa->nSeismicForceType;
	eval.nSeismicForceDirection = m_pSipa->nSeismicForceDirection;

	std::map<int, T_SIET_EVAL_BEAR_LINE_CONCU> mBearLine;

	for (int i = 0; i < m_pSigrLower->BEAR_D.GetCount(); i++)
	{
		const T_SIGR_LOWER_BEAR_D& bear = m_pSigrLower->BEAR_D[i];
		if (bear.nType == T_SIGR_LOWER_BEAR_D::kRigidLink) continue;
		if (bear.aPosition.GetCount() < 2) { ASSERT(bear.aPosition.GetCount() == 2); continue; }

		T_SIET_EVAL_BEAR_GROUP_CONCU group;
		group.strName = bear.strName;
		group.kNode_i = bear.aPosition[0];
		group.kNode_j = bear.aPosition[1];
		group.nType = bear.nType;
		group.nDirShear = bear.nDirShear;

		T_SIET_EVAL_NLNK_FORCE avergeMax, avergeMin;

		int nSize = m_pSipa->aSeisThisK.GetCount();
		for (int j = 0; j < nSize; j++)
		{
			const T_THIS_K& kThis = m_pSipa->aSeisThisK[j];

			T_SIET_EVAL_NLNK_FORCE max, min;
			if (GetConcForceEach(kThis, &group, &max, &min) == FALSE) continue;

			avergeMax += max;
			avergeMin += min;
		}

		avergeMax /= nSize;
		avergeMin /= nSize;

		group.max = avergeMax;
		group.min = avergeMin;

		if (mBearLine.find(bear.nLineNum) == mBearLine.end()) {
			T_SIET_EVAL_BEAR_LINE_CONCU line;
			line.nLineNum = bear.nLineNum;
			line.aGroup.Add(group);
			mBearLine.insert(std::make_pair(line.nLineNum, line));
		}
		else {
			mBearLine[bear.nLineNum].aGroup.Add(group);
		}
	}

	if (mBearLine.find(T_SIGR_LOWER_BEAR_D::kLine1) != mBearLine.end())
		eval.Line1 = mBearLine[T_SIGR_LOWER_BEAR_D::kLine1];
	if (mBearLine.find(T_SIGR_LOWER_BEAR_D::kLine2) != mBearLine.end())
		eval.Line2 = mBearLine[T_SIGR_LOWER_BEAR_D::kLine2];

	// save result
	CBrdgSeisEvalResult_JP* pResult = m_pDoc->m_pPostCtrl->GetBrdgSeisEvalResult_JP(); if (!pResult) { ASSERT(0); return; }
	pResult->SetResultSietBearConcEach(m_pSiet->kSipa, m_pSiet->kSigrLower, eval);
}

BOOL CSeismicDesignSiet_Bearing::GetConcForceEach(const T_THIS_K& kThis, const T_SIET_EVAL_BEAR_GROUP_CONCU* pGroup, OUT T_SIET_EVAL_NLNK_FORCE* pMax, OUT T_SIET_EVAL_NLNK_FORCE* pMin)
{
	int nSeisForceDir = m_pSipa->nSeismicForceDirection;
	int nDir = pGroup->nDirShear;

	const int nHmax = [nSeisForceDir, nDir]()
		{
			int nShearForceDir = (nSeisForceDir == T_SIPA_D::kAxis_Dir) ? nDir : (nDir == seis_jp::kY_Dir ? seis_jp::kZ_Dir : seis_jp::kY_Dir);
			switch (nShearForceDir)
			{
			case seis_jp::kY_Dir: return STEK_SHEAR_Y;
			case seis_jp::kZ_Dir: return STEK_SHEAR_Z;
			default: ASSERT(0); return 2;
			}
		}();

	CArray<T_NLNK_K, T_NLNK_K> nlnkKeys;
	if (m_pDoc->m_pAttrCtrl->GetNlnkKeyListBetween2Nodes(pGroup->kNode_i, pGroup->kNode_j, nlnkKeys) == FALSE) return FALSE;
	if (nlnkKeys.GetCount() < 1) { ASSERT(nlnkKeys.GetCount() == 1); return FALSE; }

	T_LCOM_D LcomD;
	if (!m_pDoc->m_pPostCtrl->MakeLoadComb(D_LOADCASE_HISTORY, kThis, LcomD)) return FALSE;
	m_pDoc->m_pPostCtrl->SelectLoadCombNew(LcomD);

	m_pDoc->m_pPostCtrl->SelectThis(kThis);

	T_STEK_D max[2], min[2];
	if (m_pDoc->m_pPostCtrl->GetThisInfo()->GetNlnkWithTime(nlnkKeys[0], &max[0], &min[0], 0, &max[1], &min[1], 0) == FALSE) return FALSE;

	double dMaxHmaxTimeIdx = 0, dMinHmaxTimeIdx = 0;
	pMax->dHmax = max[0].dblForce[STEK_POSI_I][nHmax];
	pMin->dHmax = min[0].dblForce[STEK_POSI_I][nHmax];
	dMaxHmaxTimeIdx = max[1].dblForce[STEK_POSI_I][nHmax];
	dMinHmaxTimeIdx = min[1].dblForce[STEK_POSI_I][nHmax];

	int nMaxTimeIdx = 0, nMinTimeIdx = 0;
	if (GetTimeIndex(kThis, dMaxHmaxTimeIdx, nMaxTimeIdx) == FALSE) return FALSE;
	if (GetTimeIndex(kThis, dMinHmaxTimeIdx, nMinTimeIdx) == FALSE) return FALSE;

	T_STEK_D stekMax, stekMin;
	m_pDoc->m_pPostCtrl->GetNlfcHistory(nMaxTimeIdx, nlnkKeys[0], stekMax);
	m_pDoc->m_pPostCtrl->GetNlfcHistory(nMinTimeIdx, nlnkKeys[0], stekMin);

	pMax->dVmax = stekMax.dblForce[STEK_POSI_I][STEK_AXIS];
	pMin->dVmax = stekMin.dblForce[STEK_POSI_I][STEK_AXIS];

	return GetDispConcForceEach(nDir, nlnkKeys[0], nMaxTimeIdx, nMinTimeIdx, pMax->dUmax, pMin->dUmax);
}

BOOL CSeismicDesignSiet_Bearing::GetDispConcForceEach(const int& nDir, const T_NLNK_K& kNlnk, const int& nMaxTimeIdx, const int& nMinTimeIdx, OUT double& dUmax, OUT double& dUmin)
{
	T_DISP_D dispMax, dispMin;
	m_pDoc->m_pPostCtrl->GetNldfHistory(nMaxTimeIdx, kNlnk, dispMax);
	m_pDoc->m_pPostCtrl->GetNldfHistory(nMinTimeIdx, kNlnk, dispMin);

	int nSeismicForceDirection = m_pSipa->nSeismicForceDirection;

	const int nUmax = [nSeismicForceDirection, nDir]()
	{
		int nShearForceDir = (nSeismicForceDirection == T_SIPA_D::kAxis_Dir) ? nDir : (nDir == seis_jp::kY_Dir ? seis_jp::kZ_Dir : seis_jp::kY_Dir);
		switch (nShearForceDir)
		{
		case seis_jp::kY_Dir: return DISP_COMP_DY;
		case seis_jp::kZ_Dir: return DISP_COMP_DZ;
		default: ASSERT(0); return 2;
		}
	}();

	dUmax = dispMax.dblDisp[nUmax];
	dUmin = dispMin.dblDisp[nUmax];

	return TRUE;
}

void CSeismicDesignSiet_Bearing::GetConcurvityLineBr()
{
	T_SIET_EVAL_LINE_CONC eval;
	eval.kSipa = m_pSiet->kSipa;
	eval.kSigrLower = m_pSiet->kSigrLower;
	eval.nSeismicForceType = m_pSipa->nSeismicForceType;
	eval.nSeismicForceDirection = m_pSipa->nSeismicForceDirection;

	std::map<int, std::vector<T_SIGR_LOWER_BEAR_D>> mBearLine;
	GetBearingByLine(mBearLine);

	if (mBearLine.find(T_SIGR_LOWER_BEAR_D::kLine1) != mBearLine.end()) {
		eval.Line1.nLineNum = T_SIGR_LOWER_BEAR_D::kLine1;
		std::vector<T_SIGR_LOWER_BEAR_D> bear1 = mBearLine[T_SIGR_LOWER_BEAR_D::kLine1];
		GetConcurvityValue(bear1, &eval.Line1);
	}

	if (mBearLine.find(T_SIGR_LOWER_BEAR_D::kLine2) != mBearLine.end()) {
		eval.Line2.nLineNum = T_SIGR_LOWER_BEAR_D::kLine2;
		std::vector<T_SIGR_LOWER_BEAR_D> bear2 = mBearLine[T_SIGR_LOWER_BEAR_D::kLine2];
		GetConcurvityValue(bear2, &eval.Line2);
	}

	// save result
	CBrdgSeisEvalResult_JP* pResult = m_pDoc->m_pPostCtrl->GetBrdgSeisEvalResult_JP(); if (!pResult) { ASSERT(0); return; }
	pResult->SetResultSietBearLineConc(m_pSiet->kSipa, m_pSiet->kSigrLower, eval);
}

void CSeismicDesignSiet_Bearing::GetBearingByLine(OUT std::map<int, std::vector<T_SIGR_LOWER_BEAR_D>>& mBearLine)
{
	for (int i = 0; i < m_pSigrLower->BEAR_D.GetCount(); i++)
	{
		const T_SIGR_LOWER_BEAR_D& bear = m_pSigrLower->BEAR_D[i];
		if (bear.nType == T_SIGR_LOWER_BEAR_D::kRigidLink) continue;
		if (bear.aPosition.GetCount() < 2) { ASSERT(bear.aPosition.GetCount() == 2); continue; }

		if (mBearLine.find(bear.nLineNum) == mBearLine.end()) {
			std::vector<T_SIGR_LOWER_BEAR_D> list;
			list.push_back(bear);
			mBearLine.insert(std::make_pair(bear.nLineNum, list));
		}
		else {
			mBearLine[bear.nLineNum].push_back(bear);
		}
	}
}

void CSeismicDesignSiet_Bearing::GetConcurvityValue(const std::vector<T_SIGR_LOWER_BEAR_D>& bearList, OUT T_SIET_EVAL_LINE_MINMAX* pLine)
{
	for (auto& bear : bearList)
	{
		T_SIET_EVAL_LINE_GROUP groupMin, groupMax;
		groupMin.strName = bear.strName;
		groupMin.kNode_i = bear.aPosition[0];
		groupMin.kNode_j = bear.aPosition[1];
		groupMin.nType = bear.nType;
		groupMin.nDirShear = bear.nDirShear;
		groupMax = groupMin;

		T_SIET_EVAL_NLNK_FORCE avergeMin, avergeMax;

		int nSize = m_pSipa->aSeisThisK.GetCount();
		for (int j = 0; j < nSize; j++)
		{
			const T_THIS_K& kThis = m_pSipa->aSeisThisK[j];

			ConcBear min, max;
			min.kThis = kThis;
			max.kThis = kThis;
			min.nDirShear = bear.nDirShear;
			max.nDirShear = bear.nDirShear;
			if (GetMinMaxForceAxis(kThis, bear, min, max) == FALSE) continue;

			T_SIET_EVAL_NLNK_FORCE minF, maxF;
			GetConcValueWithTime(bear, min, minF);
			GetConcValueWithTime(bear, max, maxF);

			avergeMin += minF;
			avergeMax += maxF;
		}

		avergeMin /= nSize;
		avergeMax /= nSize;

		groupMin.Force = avergeMin;
		groupMax.Force = avergeMax;

		pLine->aHmin.Add(groupMin);
		pLine->aHmax.Add(groupMax);

		pLine->SumMin.dHmax += avergeMin.dHmax;
		pLine->SumMin.dVmax += avergeMin.dVmax;
		if (abs(avergeMin.dUmax) > abs(pLine->SumMin.dUmax))
			pLine->SumMin.dUmax = avergeMin.dUmax;

		pLine->SumMax.dHmax += avergeMax.dHmax;
		pLine->SumMax.dVmax += avergeMax.dVmax;
		if (abs(avergeMax.dUmax) > abs(pLine->SumMax.dUmax))
			pLine->SumMax.dUmax = avergeMax.dUmax;
	}
}

BOOL CSeismicDesignSiet_Bearing::GetMinMaxForceAxis(const T_THIS_K& kThis, const T_SIGR_LOWER_BEAR_D& bear, OUT ConcBear& min, OUT ConcBear& max)
{
	int nSeisForceDir = m_pSipa->nSeismicForceDirection;
	int nDir = bear.nDirShear;

	const int nHmax = [nSeisForceDir, nDir]()
	{
		int nShearForceDir = (nSeisForceDir == T_SIPA_D::kAxis_Dir) ? nDir : (nDir == seis_jp::kY_Dir ? seis_jp::kZ_Dir : seis_jp::kY_Dir);
		switch (nShearForceDir)
		{
		case seis_jp::kY_Dir: return STEK_SHEAR_Y;
		case seis_jp::kZ_Dir: return STEK_SHEAR_Z;
		default: ASSERT(0); return 2;
		}
	}();

	CArray<T_NLNK_K, T_NLNK_K> nlnkKeys;
	if (m_pDoc->m_pAttrCtrl->GetNlnkKeyListBetween2Nodes(bear.aPosition[0], bear.aPosition[1], nlnkKeys) == FALSE) return FALSE;
	if (nlnkKeys.GetCount() < 1) { ASSERT(nlnkKeys.GetCount() == 1); return FALSE; }

	m_pDoc->m_pPostCtrl->SelectThis(kThis);

	T_STEK_D stekMax[2], stekMin[2];
	if (m_pDoc->m_pPostCtrl->GetThisInfo()->GetNlnkWithTime(nlnkKeys[0], &stekMax[0], &stekMin[0], 0, &stekMax[1], &stekMin[1], 0) == FALSE) return FALSE;

	//항상 I,J단의 부재력은 같음
	max.dHmax = stekMax[0].dblForce[STEK_POSI_I][nHmax];
	min.dHmax = stekMin[0].dblForce[STEK_POSI_I][nHmax];
	max.dTimeCount = stekMax[1].dblForce[STEK_POSI_I][nHmax];
	min.dTimeCount = stekMin[1].dblForce[STEK_POSI_I][nHmax];

	if (GetTimeIndex(kThis, max.dTimeCount, max.nTimeIdx) == FALSE) return FALSE;
	if (GetTimeIndex(kThis, min.dTimeCount, min.nTimeIdx) == FALSE) return FALSE;

	return TRUE;
}

BOOL CSeismicDesignSiet_Bearing::GetConcValueWithTime(const T_SIGR_LOWER_BEAR_D& bear, const ConcBear& bearTime, OUT T_SIET_EVAL_NLNK_FORCE& rForce)
{
	int nSeisForceDir = m_pSipa->nSeismicForceDirection;

	m_pDoc->m_pPostCtrl->SelectThis(bearTime.kThis);

	int nDir = bear.nDirShear;

	const int nHmax = [nSeisForceDir, nDir]()
	{
		int nShearForceDir = (nSeisForceDir == T_SIPA_D::kAxis_Dir) ? nDir : (nDir == seis_jp::kY_Dir ? seis_jp::kZ_Dir : seis_jp::kY_Dir);
		switch (nShearForceDir)
		{
		case seis_jp::kY_Dir: return STEK_SHEAR_Y;
		case seis_jp::kZ_Dir: return STEK_SHEAR_Z;
		default: ASSERT(0); return 2;
		}
	}();

	const int nUmax = [nSeisForceDir, nDir]()
	{
		int nShearForceDir = (nSeisForceDir == T_SIPA_D::kAxis_Dir) ? nDir : (nDir == seis_jp::kY_Dir ? seis_jp::kZ_Dir : seis_jp::kY_Dir);
		switch (nShearForceDir)
		{
		case seis_jp::kY_Dir: return DISP_COMP_DY;
		case seis_jp::kZ_Dir: return DISP_COMP_DZ;
		default: ASSERT(0); return 2;
		}
	}();

	CArray<T_NLNK_K, T_NLNK_K> nlnkKeys;
	if (m_pDoc->m_pAttrCtrl->GetNlnkKeyListBetween2Nodes(bear.aPosition[0], bear.aPosition[1], nlnkKeys) == FALSE) return FALSE;
	if (nlnkKeys.GetCount() < 1) { ASSERT(nlnkKeys.GetCount() == 1); return FALSE; }

	T_STEK_D StekData;
	if (m_pDoc->m_pPostCtrl->GetNlfcHistory(bearTime.nTimeIdx, nlnkKeys[0], StekData) == FALSE) return FALSE;

	T_DISP_D DispData;
	if (m_pDoc->m_pPostCtrl->GetNldfHistory(bearTime.nTimeIdx, nlnkKeys[0], DispData) == FALSE) return FALSE;

	rForce.dHmax = StekData.dblForce[STEK_POSI_I][nHmax];
	rForce.dVmax = StekData.dblForce[STEK_POSI_I][STEK_AXIS];
	rForce.dUmax = DispData.dblDisp[nUmax];

	return TRUE;
}
#pragma endregion <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

#pragma region >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> CSeismicDesignSiet_PierBase 
CSeismicDesignSiet_PierBase::CSeismicDesignSiet_PierBase(CDgnProgressDlg* pProgress) : CSeismicDesignSiet()
{
	m_pProgress = pProgress;
}

CSeismicDesignSiet_PierBase::~CSeismicDesignSiet_PierBase(void)
{
}

void CSeismicDesignSiet_PierBase::AddProgressStep(const T_SIET_K& kSiet)
{
	if (m_pDoc == nullptr) return;
	if (m_pProgress == nullptr) return;

	T_SIET_D dSiet; T_SIPA_D dSipa; T_SIGR_LOWER_D dSigrLower;
	if (m_pDoc->m_pAttrCtrl2->GetQSiet()->Get(kSiet, dSiet) == FALSE) return;
	if (m_pDoc->m_pAttrCtrl2->GetQSipa()->Get(dSiet.kSipa, dSipa) == FALSE) return;
	if (dSipa.nDesignCode != GetCurrentDgnCode()) return;
	if (m_pDoc->m_pAttrCtrl2->GetQSigrLower()->Get(dSiet.kSigrLower, dSigrLower) == FALSE) return;

	switch (dSiet.nOutTypeMember)
	{
	case T_SIET_D::kMinMax_Mb:
		break;
	case T_SIET_D::kConcurvity_Each_Mb:
		break;
	case T_SIET_D::kConcurvity_FootingLow:
		AddProgressStep_FootingLow(&dSipa, &dSigrLower);
		break;
	default:
		ASSERT(FALSE);
		break;
	}

	m_pProgress->m_nTotalSteps++;
}

void CSeismicDesignSiet_PierBase::AddProgressStep_FootingLow(T_SIPA_D* pSipa, T_SIGR_LOWER_D* pSigrLower)
{
	CArray<ConcNode, ConcNode> masters;
	GetConcMasterNode(masters);

	CArray<ConcNode, ConcNode> subNodes;
	GetConcSubNode(subNodes);

	for (int i = 0; i < masters.GetCount(); i++)
	{
		ConcNode& master = masters[i];

		int nSize = pSipa->aSeisThisK.GetCount();
		for (int j = 0; j < nSize; j++)
		{
			const T_THIS_K& kThis = pSipa->aSeisThisK[j];

			AddProgressStep_GetConcurvityForce(kThis, master, subNodes);
		}
	}
}

void CSeismicDesignSiet_PierBase::AddProgressStep_GetConcurvityForce(const T_THIS_K& kThis, ConcNode& master, const CArray<ConcNode, ConcNode>& subNodes)
{
	CArray<ConcNode, ConcNode> concNodes;
	concNodes.Add(master);
	concNodes.Append(subNodes);

	CArray<double, double> aTimeStep;
	m_pDoc->m_pAttrCtrl->GetTimeStep(kThis, aTimeStep);
	int nTimeCount = aTimeStep.GetSize();

	for (int iForce = 0; iForce < ConcNode::kForceNum; iForce++)
	{
		int nMinTime = 0, nMaxTime = 0;
		GetTimeIndex(kThis, master.dTimeCountMin[iForce], nMinTime);
		GetTimeIndex(kThis, master.dTimeCountMax[iForce], nMaxTime);

		for (int i = 0; i < concNodes.GetCount(); i++)
		{
			const ConcNode& target = concNodes[i];

			AddProgressStep_GetTimeKrnlForceResult(kThis, nTimeCount, target, nMinTime, nMaxTime);
		}
	}
}

void CSeismicDesignSiet_PierBase::AddProgressStep_GetTimeKrnlForceResult(const T_THIS_K& kThis,
	const int& nTimeCount,
	const ConcNode& target,
	const int& nMinTime,
	const int& nMaxTime)
{
	auto pThisInfo = m_pDoc->m_pPostCtrl->GetThisInfo();
	if (pThisInfo->IsBeamWithHnge(target.kElem) > 0) {
		return;
	}

	for (int iForce = 0; iForce < ConcNode::kForceNum; iForce++)
	{
		CArray<double, double> arAllStepResult;

		int nSubType = RESULT_TYPE_SELECTION_FORCE;
		int nDBComp = iForce + 1;
		int nComp = pThisInfo->GetComponent4SolverPrntType1(target.kElem, nSubType, target.ePosNodeLow, nDBComp);

		if (!pThisInfo->GetTimeKrnlResult(
			target.kElem, kThis, nTimeCount, 0, TKRNL_TYPE_BEAM, nComp,//결과얻어오기위한변수  
			nSubType, nDBComp, 1,
			arAllStepResult))// 결과데이터 
		{
			return;
		}

		if (nMinTime >= arAllStepResult.GetSize() || nMaxTime >= arAllStepResult.GetSize())
		{
			return;
		}

		m_pProgress->m_nTotalSteps++;
	}
}

BOOL CSeismicDesignSiet_PierBase::Calculate(const T_SIET_K& kSiet)
{
	if (m_pDoc == NULL) return FALSE;

	T_SIET_D dSiet; T_SIPA_D dSipa; T_SIGR_LOWER_D dSigrLower;
	if (m_pDoc->m_pAttrCtrl2->GetQSiet()->Get(kSiet, dSiet) == FALSE) return FALSE;
	if (m_pDoc->m_pAttrCtrl2->GetQSipa()->Get(dSiet.kSipa, dSipa) == FALSE) return FALSE;
	if (dSipa.nDesignCode != GetCurrentDgnCode()) return FALSE;
	if (m_pDoc->m_pAttrCtrl2->GetQSigrLower()->Get(dSiet.kSigrLower, dSigrLower) == FALSE) return FALSE;

	m_kSiet = kSiet;
	m_pSiet = &dSiet;
	m_pSipa = &dSipa;
	m_pSigrLower = &dSigrLower;

	BOOL bResult = FALSE;
	switch (dSiet.nOutTypeMember)
	{
	case T_SIET_D::kMinMax_Mb:
		bResult = GetMemberForceMax();
		break;
	case T_SIET_D::kConcurvity_Each_Mb:
		bResult = GetConcurvityEachMb();
		break;
	case T_SIET_D::kConcurvity_FootingLow:
		bResult = GetConcurvityFootingLow();
		break;
	default:
		ASSERT(FALSE);
		break;
	}

	if (m_pProgress->NextStep() == FALSE) throw FALSE;

	return bResult;
}

BOOL CSeismicDesignSiet_PierBase::GetMemberForceMax()
{
	T_SIET_EVAL_MEMB_MINMAX eval;
	eval.kSipa = m_pSiet->kSipa;
	eval.kSigrLower = m_pSiet->kSigrLower;
	eval.nSeismicForceType = m_pSipa->nSeismicForceType;
	eval.nSeismicForceDirection = m_pSipa->nSeismicForceDirection;

	if (m_pSiet->bPierBase_Sf)
		GetColumnBotForce(&eval);

	if (m_pSiet->bFootingLower_Sf)
		GetFootingBotForce(&eval);

	// save result
	CBrdgSeisEvalResult_JP* pResult = m_pDoc->m_pPostCtrl->GetBrdgSeisEvalResult_JP(); if (!pResult) { ASSERT(0); return FALSE; }
	pResult->SetResultSietMembMinMax(m_pSiet->kSipa, m_pSiet->kSigrLower, eval);

	return TRUE;
}

void CSeismicDesignSiet_PierBase::GetColumnBotForce(OUT T_SIET_EVAL_MEMB_MINMAX* pEval)
{
	for (int i = 0; i < m_pSigrLower->COLM_D.GetCount(); i++)
	{
		const T_SIGR_LOWER_COLM_D& colm = m_pSigrLower->COLM_D[i];

		T_SIET_EVAL_MEMB_GROUP group;
		group.nLocation = T_SIET_EVAL_MEMB_GROUP::kPierBase;
		group.strName = colm.strName;

		T_SIET_EVAL_BEAM_FORCE avergeMax;
		T_SIET_EVAL_BEAM_FORCE avergeMin;

		GetBotNodeElemKey(colm.aInvestigatedElem, group.kElem, group.ePosNodeLow);

		int nSize = m_pSipa->aSeisThisK.GetCount();
		for (int j = 0; j < nSize; j++)
		{
			const T_THIS_K& kThis = m_pSipa->aSeisThisK[j];

			T_SIET_EVAL_BEAM_FORCE max;
			T_SIET_EVAL_BEAM_FORCE min;
			//결과 - 일반 - 결과 테이블 - 보요소 - 단면력 (요소선택, 하중 케이스 선택, 위치선택)
			if (GetMinMaxForce(kThis, &group, &max, &min) == FALSE) continue;

			avergeMax += max;
			avergeMin += min;
		}

		avergeMax /= nSize;
		avergeMin /= nSize;

		group.max = avergeMax;
		group.min = avergeMin;

		pEval->aGroup.Add(group);
	}
}

void CSeismicDesignSiet_PierBase::GetFootingBotForce(OUT T_SIET_EVAL_MEMB_MINMAX* pEval)
{
	for (int i = 0; i < m_pSigrLower->FOOT_D.GetCount(); i++)
	{
		const T_SIGR_LOWER_FOOT_D& foot = m_pSigrLower->FOOT_D[i];

		T_SIET_EVAL_MEMB_GROUP group;
		group.nLocation = T_SIET_EVAL_MEMB_GROUP::kFootingBot;
		group.strName = foot.strName;

		GetBotNodeElemKey(foot.aInvestigatedElem, group.kElem, group.ePosNodeLow);

		T_SIET_EVAL_BEAM_FORCE avergeMax;
		T_SIET_EVAL_BEAM_FORCE avergeMin;

		int nSize = m_pSipa->aSeisThisK.GetCount();
		for (int j = 0; j < nSize; j++)
		{
			const T_THIS_K& kThis = m_pSipa->aSeisThisK[j];

			T_SIET_EVAL_BEAM_FORCE max;
			T_SIET_EVAL_BEAM_FORCE min;
			//결과 - 일반 - 결과 테이블 - 보요소 - 단면력 (요소선택, 하중 케이스 선택, 위치선택)
			if (GetMinMaxForce(kThis, &group, &max, &min) == FALSE) continue;

			avergeMax += max;
			avergeMin += min;
		}

		avergeMax /= nSize;
		avergeMin /= nSize;

		group.max = avergeMax;
		group.min = avergeMin;

		pEval->aGroup.Add(group);
	}
}

void CSeismicDesignSiet_PierBase::GetBotNodeElemKey(const T_ELEM_K_LIST& elemKeys, OUT T_ELEM_K& kElem, OUT int& ePosNodeLow)
{
	double dMinZ = DBL_MAX;
	int elnod_index = 0;
	T_ELEM_K kElemLow = 0;

	int nCount = elemKeys.GetCount();
	for (int i = 0; i < nCount; i++)
	{
		T_ELEM_K kElem = elemKeys[i];

		T_ELEM_D dElem;
		if (m_pDoc->m_pAttrCtrl->GetElem(kElem, dElem) == FALSE)
			continue;

		for (int k = 0; k < D_ELEM_MAXNOD; k++) {
			if (dElem.elnod[k] != 0) {
				T_NODE_K kNode = dElem.elnod[k];
				T_NODE_D dNode;
				if (m_pDoc->m_pAttrCtrl->GetNode(kNode, dNode) == FALSE)
					continue;

				if (dNode.z < dMinZ) {
					dMinZ = dNode.z;
					elnod_index = k;
					kElemLow = kElem;
				}
			}
		}
	}

	kElem = kElemLow;
	ePosNodeLow = elnod_index == 0 ? T_SIET_EVAL_MEMB_GROUP::kI : T_SIET_EVAL_MEMB_GROUP::kJ;
}

BOOL CSeismicDesignSiet_PierBase::GetMinMaxForce(const T_THIS_K& kThis, const T_SIET_EVAL_MEMB_GROUP* pGroup, OUT T_SIET_EVAL_BEAM_FORCE* pMax, OUT T_SIET_EVAL_BEAM_FORCE* pMin)
{
	T_LCOM_D LcomD;
	if (!m_pDoc->m_pPostCtrl->MakeLoadComb(D_LOADCASE_HISTORY, kThis, LcomD)) return FALSE;
	m_pDoc->m_pPostCtrl->SelectLoadCombNew(LcomD);

	T_STRB_D max, min;
	m_pDoc->m_pPostCtrl->GetStrbNew(pGroup->kElem, &max, &min, NULL, TRUE);

	if (pGroup->ePosNodeLow == T_SIET_EVAL_MEMB_GROUP::kI) {
		pMax->dFx = max.dblForce[STRB_POSI_I][STRB_AXIS_I];
		pMax->dFy = max.dblForce[STRB_POSI_I][STRB_SHEAR_Y_I];
		pMax->dFz = max.dblForce[STRB_POSI_I][STRB_SHEAR_Z_I];
		pMax->dMx = max.dblForce[STRB_POSI_I][STRB_TORSION_I];
		pMax->dMy = max.dblForce[STRB_POSI_I][STRB_MOMENT_Y_I];
		pMax->dMz = max.dblForce[STRB_POSI_I][STRB_MOMENT_Z_I];

		pMin->dFx = min.dblForce[STRB_POSI_I][STRB_AXIS_I];
		pMin->dFy = min.dblForce[STRB_POSI_I][STRB_SHEAR_Y_I];
		pMin->dFz = min.dblForce[STRB_POSI_I][STRB_SHEAR_Z_I];
		pMin->dMx = min.dblForce[STRB_POSI_I][STRB_TORSION_I];
		pMin->dMy = min.dblForce[STRB_POSI_I][STRB_MOMENT_Y_I];
		pMin->dMz = min.dblForce[STRB_POSI_I][STRB_MOMENT_Z_I];
	}
	else if (pGroup->ePosNodeLow == T_SIET_EVAL_MEMB_GROUP::kJ) {
		pMax->dFx = max.dblForce[STRB_POSI_J][STRB_AXIS_J];
		pMax->dFy = max.dblForce[STRB_POSI_J][STRB_SHEAR_Y_J];
		pMax->dFz = max.dblForce[STRB_POSI_J][STRB_SHEAR_Z_J];
		pMax->dMx = max.dblForce[STRB_POSI_J][STRB_TORSION_J];
		pMax->dMy = max.dblForce[STRB_POSI_J][STRB_MOMENT_Y_J];
		pMax->dMz = max.dblForce[STRB_POSI_J][STRB_MOMENT_Z_J];

		pMin->dFx = min.dblForce[STRB_POSI_J][STRB_AXIS_J];
		pMin->dFy = min.dblForce[STRB_POSI_J][STRB_SHEAR_Y_J];
		pMin->dFz = min.dblForce[STRB_POSI_J][STRB_SHEAR_Z_J];
		pMin->dMx = min.dblForce[STRB_POSI_J][STRB_TORSION_J];
		pMin->dMy = min.dblForce[STRB_POSI_J][STRB_MOMENT_Y_J];
		pMin->dMz = min.dblForce[STRB_POSI_J][STRB_MOMENT_Z_J];
	}

	return TRUE;
}

BOOL CSeismicDesignSiet_PierBase::GetConcurvityEachMb()
{
	T_SIET_EVAL_MEMB_EACH eval;
	eval.kSipa = m_pSiet->kSipa;
	eval.kSigrLower = m_pSiet->kSigrLower;
	eval.nSeismicForceType = m_pSipa->nSeismicForceType;
	eval.nSeismicForceDirection = m_pSipa->nSeismicForceDirection;

	if (m_pSiet->bPierBase_Sf)
		GetColumnBotEachMb(&eval);

	if (m_pSiet->bFootingLower_Sf)
		GetFootingBotEachMb(&eval);

	// save result
	CBrdgSeisEvalResult_JP* pResult = m_pDoc->m_pPostCtrl->GetBrdgSeisEvalResult_JP(); if (!pResult) { ASSERT(0); return FALSE; }
	pResult->SetResultSietMembConcEach(m_pSiet->kSipa, m_pSiet->kSigrLower, eval);

	return TRUE;
}

void CSeismicDesignSiet_PierBase::GetColumnBotEachMb(OUT T_SIET_EVAL_MEMB_EACH* pEval)
{
	for (int i = 0; i < m_pSigrLower->COLM_D.GetCount(); i++)
	{
		const T_SIGR_LOWER_COLM_D& colm = m_pSigrLower->COLM_D[i];

		T_SIET_EVAL_MEMB_CONCU group;
		group.ePosNodeLow = T_SIET_EVAL_MEMB_GROUP::kPierBase;
		group.strName = colm.strName;

		GetBotNodeElemKey(colm.aInvestigatedElem, group.kElem, group.ePosNodeLow);

		T_SIET_EVAL_BEAM_FORCE avergeMax[T_SIET_EVAL_MEMB_CONCU::kForceNum];
		T_SIET_EVAL_BEAM_FORCE avergeMin[T_SIET_EVAL_MEMB_CONCU::kForceNum];

		int nSize = m_pSipa->aSeisThisK.GetCount();
		for (int j = 0; j < nSize; j++)
		{
			const T_THIS_K& kThis = m_pSipa->aSeisThisK[j];

			T_SIET_EVAL_BEAM_FORCE max[T_SIET_EVAL_MEMB_CONCU::kForceNum];
			T_SIET_EVAL_BEAM_FORCE min[T_SIET_EVAL_MEMB_CONCU::kForceNum];
			//결과 - 일반 - 결과 테이블 - 보요소 - 단면력 (요소선택, 하중 케이스 선택, 위치선택) - 동시성 결과
			if (GetConcurvityEach(kThis, &group, max, min) == FALSE) continue;

			avergeMax[T_SIET_EVAL_MEMB_CONCU::kAxial] += max[T_SIET_EVAL_MEMB_CONCU::kAxial];
			avergeMax[T_SIET_EVAL_MEMB_CONCU::kShearY] += max[T_SIET_EVAL_MEMB_CONCU::kShearY];
			avergeMax[T_SIET_EVAL_MEMB_CONCU::kShearZ] += max[T_SIET_EVAL_MEMB_CONCU::kShearZ];
			avergeMax[T_SIET_EVAL_MEMB_CONCU::kTorsion] += max[T_SIET_EVAL_MEMB_CONCU::kTorsion];
			avergeMax[T_SIET_EVAL_MEMB_CONCU::kMomentY] += max[T_SIET_EVAL_MEMB_CONCU::kMomentY];
			avergeMax[T_SIET_EVAL_MEMB_CONCU::kMomentZ] += max[T_SIET_EVAL_MEMB_CONCU::kMomentZ];

			avergeMin[T_SIET_EVAL_MEMB_CONCU::kAxial] += min[T_SIET_EVAL_MEMB_CONCU::kAxial];
			avergeMin[T_SIET_EVAL_MEMB_CONCU::kShearY] += min[T_SIET_EVAL_MEMB_CONCU::kShearY];
			avergeMin[T_SIET_EVAL_MEMB_CONCU::kShearZ] += min[T_SIET_EVAL_MEMB_CONCU::kShearZ];
			avergeMin[T_SIET_EVAL_MEMB_CONCU::kTorsion] += min[T_SIET_EVAL_MEMB_CONCU::kTorsion];
			avergeMin[T_SIET_EVAL_MEMB_CONCU::kMomentY] += min[T_SIET_EVAL_MEMB_CONCU::kMomentY];
			avergeMin[T_SIET_EVAL_MEMB_CONCU::kMomentZ] += min[T_SIET_EVAL_MEMB_CONCU::kMomentZ];
		}

		avergeMax[T_SIET_EVAL_MEMB_CONCU::kAxial] /= nSize;
		avergeMax[T_SIET_EVAL_MEMB_CONCU::kShearY] /= nSize;
		avergeMax[T_SIET_EVAL_MEMB_CONCU::kShearZ] /= nSize;
		avergeMax[T_SIET_EVAL_MEMB_CONCU::kTorsion] /= nSize;
		avergeMax[T_SIET_EVAL_MEMB_CONCU::kMomentY] /= nSize;
		avergeMax[T_SIET_EVAL_MEMB_CONCU::kMomentZ] /= nSize;

		avergeMin[T_SIET_EVAL_MEMB_CONCU::kAxial] /= nSize;
		avergeMin[T_SIET_EVAL_MEMB_CONCU::kShearY] /= nSize;
		avergeMin[T_SIET_EVAL_MEMB_CONCU::kShearZ] /= nSize;
		avergeMin[T_SIET_EVAL_MEMB_CONCU::kTorsion] /= nSize;
		avergeMin[T_SIET_EVAL_MEMB_CONCU::kMomentY] /= nSize;
		avergeMin[T_SIET_EVAL_MEMB_CONCU::kMomentZ] /= nSize;

		group.max[T_SIET_EVAL_MEMB_CONCU::kAxial] = avergeMax[T_SIET_EVAL_MEMB_CONCU::kAxial];
		group.max[T_SIET_EVAL_MEMB_CONCU::kShearY] = avergeMax[T_SIET_EVAL_MEMB_CONCU::kShearY];
		group.max[T_SIET_EVAL_MEMB_CONCU::kShearZ] = avergeMax[T_SIET_EVAL_MEMB_CONCU::kShearZ];
		group.max[T_SIET_EVAL_MEMB_CONCU::kTorsion] = avergeMax[T_SIET_EVAL_MEMB_CONCU::kTorsion];
		group.max[T_SIET_EVAL_MEMB_CONCU::kMomentY] = avergeMax[T_SIET_EVAL_MEMB_CONCU::kMomentY];
		group.max[T_SIET_EVAL_MEMB_CONCU::kMomentZ] = avergeMax[T_SIET_EVAL_MEMB_CONCU::kMomentZ];

		group.min[T_SIET_EVAL_MEMB_CONCU::kAxial] = avergeMin[T_SIET_EVAL_MEMB_CONCU::kAxial];
		group.min[T_SIET_EVAL_MEMB_CONCU::kShearY] = avergeMin[T_SIET_EVAL_MEMB_CONCU::kShearY];
		group.min[T_SIET_EVAL_MEMB_CONCU::kShearZ] = avergeMin[T_SIET_EVAL_MEMB_CONCU::kShearZ];
		group.min[T_SIET_EVAL_MEMB_CONCU::kTorsion] = avergeMin[T_SIET_EVAL_MEMB_CONCU::kTorsion];
		group.min[T_SIET_EVAL_MEMB_CONCU::kMomentY] = avergeMin[T_SIET_EVAL_MEMB_CONCU::kMomentY];
		group.min[T_SIET_EVAL_MEMB_CONCU::kMomentZ] = avergeMin[T_SIET_EVAL_MEMB_CONCU::kMomentZ];

		pEval->aGroup.Add(group);
	}
}

void CSeismicDesignSiet_PierBase::GetFootingBotEachMb(OUT T_SIET_EVAL_MEMB_EACH* pEval)
{
	for (int i = 0; i < m_pSigrLower->FOOT_D.GetCount(); i++)
	{
		const T_SIGR_LOWER_FOOT_D& foot = m_pSigrLower->FOOT_D[i];

		T_SIET_EVAL_MEMB_CONCU group;
		group.ePosNodeLow = T_SIET_EVAL_MEMB_GROUP::kPierBase;
		group.strName = foot.strName;

		GetBotNodeElemKey(foot.aInvestigatedElem, group.kElem, group.ePosNodeLow);

		T_SIET_EVAL_BEAM_FORCE avergeMax[T_SIET_EVAL_MEMB_CONCU::kForceNum];
		T_SIET_EVAL_BEAM_FORCE avergeMin[T_SIET_EVAL_MEMB_CONCU::kForceNum];

		int nSize = m_pSipa->aSeisThisK.GetCount();
		for (int j = 0; j < nSize; j++)
		{
			const T_THIS_K& kThis = m_pSipa->aSeisThisK[j];

			T_SIET_EVAL_BEAM_FORCE max[T_SIET_EVAL_MEMB_CONCU::kForceNum];
			T_SIET_EVAL_BEAM_FORCE min[T_SIET_EVAL_MEMB_CONCU::kForceNum];
			//결과 - 일반 - 결과 테이블 - 보요소 - 단면력 (요소선택, 하중 케이스 선택, 위치선택) - 동시성 결과
			if (GetConcurvityEach(kThis, &group, max, min) == FALSE) continue;

			avergeMax[T_SIET_EVAL_MEMB_CONCU::kAxial] += max[T_SIET_EVAL_MEMB_CONCU::kAxial];
			avergeMax[T_SIET_EVAL_MEMB_CONCU::kShearY] += max[T_SIET_EVAL_MEMB_CONCU::kShearY];
			avergeMax[T_SIET_EVAL_MEMB_CONCU::kShearZ] += max[T_SIET_EVAL_MEMB_CONCU::kShearZ];
			avergeMax[T_SIET_EVAL_MEMB_CONCU::kTorsion] += max[T_SIET_EVAL_MEMB_CONCU::kTorsion];
			avergeMax[T_SIET_EVAL_MEMB_CONCU::kMomentY] += max[T_SIET_EVAL_MEMB_CONCU::kMomentY];
			avergeMax[T_SIET_EVAL_MEMB_CONCU::kMomentZ] += max[T_SIET_EVAL_MEMB_CONCU::kMomentZ];

			avergeMin[T_SIET_EVAL_MEMB_CONCU::kAxial] += min[T_SIET_EVAL_MEMB_CONCU::kAxial];
			avergeMin[T_SIET_EVAL_MEMB_CONCU::kShearY] += min[T_SIET_EVAL_MEMB_CONCU::kShearY];
			avergeMin[T_SIET_EVAL_MEMB_CONCU::kShearZ] += min[T_SIET_EVAL_MEMB_CONCU::kShearZ];
			avergeMin[T_SIET_EVAL_MEMB_CONCU::kTorsion] += min[T_SIET_EVAL_MEMB_CONCU::kTorsion];
			avergeMin[T_SIET_EVAL_MEMB_CONCU::kMomentY] += min[T_SIET_EVAL_MEMB_CONCU::kMomentY];
			avergeMin[T_SIET_EVAL_MEMB_CONCU::kMomentZ] += min[T_SIET_EVAL_MEMB_CONCU::kMomentZ];
		}

		avergeMax[T_SIET_EVAL_MEMB_CONCU::kAxial] /= nSize;
		avergeMax[T_SIET_EVAL_MEMB_CONCU::kShearY] /= nSize;
		avergeMax[T_SIET_EVAL_MEMB_CONCU::kShearZ] /= nSize;
		avergeMax[T_SIET_EVAL_MEMB_CONCU::kTorsion] /= nSize;
		avergeMax[T_SIET_EVAL_MEMB_CONCU::kMomentY] /= nSize;
		avergeMax[T_SIET_EVAL_MEMB_CONCU::kMomentZ] /= nSize;

		avergeMin[T_SIET_EVAL_MEMB_CONCU::kAxial] /= nSize;
		avergeMin[T_SIET_EVAL_MEMB_CONCU::kShearY] /= nSize;
		avergeMin[T_SIET_EVAL_MEMB_CONCU::kShearZ] /= nSize;
		avergeMin[T_SIET_EVAL_MEMB_CONCU::kTorsion] /= nSize;
		avergeMin[T_SIET_EVAL_MEMB_CONCU::kMomentY] /= nSize;
		avergeMin[T_SIET_EVAL_MEMB_CONCU::kMomentZ] /= nSize;

		group.max[T_SIET_EVAL_MEMB_CONCU::kAxial] = avergeMax[T_SIET_EVAL_MEMB_CONCU::kAxial];
		group.max[T_SIET_EVAL_MEMB_CONCU::kShearY] = avergeMax[T_SIET_EVAL_MEMB_CONCU::kShearY];
		group.max[T_SIET_EVAL_MEMB_CONCU::kShearZ] = avergeMax[T_SIET_EVAL_MEMB_CONCU::kShearZ];
		group.max[T_SIET_EVAL_MEMB_CONCU::kTorsion] = avergeMax[T_SIET_EVAL_MEMB_CONCU::kTorsion];
		group.max[T_SIET_EVAL_MEMB_CONCU::kMomentY] = avergeMax[T_SIET_EVAL_MEMB_CONCU::kMomentY];
		group.max[T_SIET_EVAL_MEMB_CONCU::kMomentZ] = avergeMax[T_SIET_EVAL_MEMB_CONCU::kMomentZ];

		group.min[T_SIET_EVAL_MEMB_CONCU::kAxial] = avergeMin[T_SIET_EVAL_MEMB_CONCU::kAxial];
		group.min[T_SIET_EVAL_MEMB_CONCU::kShearY] = avergeMin[T_SIET_EVAL_MEMB_CONCU::kShearY];
		group.min[T_SIET_EVAL_MEMB_CONCU::kShearZ] = avergeMin[T_SIET_EVAL_MEMB_CONCU::kShearZ];
		group.min[T_SIET_EVAL_MEMB_CONCU::kTorsion] = avergeMin[T_SIET_EVAL_MEMB_CONCU::kTorsion];
		group.min[T_SIET_EVAL_MEMB_CONCU::kMomentY] = avergeMin[T_SIET_EVAL_MEMB_CONCU::kMomentY];
		group.min[T_SIET_EVAL_MEMB_CONCU::kMomentZ] = avergeMin[T_SIET_EVAL_MEMB_CONCU::kMomentZ];

		pEval->aGroup.Add(group);
	}
}

BOOL CSeismicDesignSiet_PierBase::GetConcurvityEach(const T_THIS_K& kThis, const T_SIET_EVAL_MEMB_CONCU* pGroup, T_SIET_EVAL_BEAM_FORCE* pMax, T_SIET_EVAL_BEAM_FORCE* pMin)
{
	T_LCOM_D LcomD;
	if (!m_pDoc->m_pPostCtrl->MakeLoadComb(D_LOADCASE_HISTORY, kThis, LcomD)) return FALSE;
	m_pDoc->m_pPostCtrl->SelectLoadCombNew(LcomD);

	T_SBCF_D max, min;
	m_pDoc->m_pPostCtrl->GetSbcfNew(pGroup->kElem, &max, &min, NULL, FALSE);

	if (pGroup->ePosNodeLow == T_SIET_EVAL_MEMB_GROUP::kI) {
		//Max
		pMax[T_SIET_EVAL_MEMB_CONCU::kAxial].dFx = max.dblForce[SBCF_POSI_I][SBCF_POSI_SUB_I][SBCF_COM_AXIS][SBCF_VAL_AXIS];
		pMax[T_SIET_EVAL_MEMB_CONCU::kAxial].dFy = max.dblForce[SBCF_POSI_I][SBCF_POSI_SUB_I][SBCF_COM_AXIS][SBCF_VAL_SHEAR_Y];
		pMax[T_SIET_EVAL_MEMB_CONCU::kAxial].dFz = max.dblForce[SBCF_POSI_I][SBCF_POSI_SUB_I][SBCF_COM_AXIS][SBCF_VAL_SHEAR_Z];
		pMax[T_SIET_EVAL_MEMB_CONCU::kAxial].dMx = max.dblForce[SBCF_POSI_I][SBCF_POSI_SUB_I][SBCF_COM_AXIS][SBCF_VAL_TORSION];
		pMax[T_SIET_EVAL_MEMB_CONCU::kAxial].dMy = max.dblForce[SBCF_POSI_I][SBCF_POSI_SUB_I][SBCF_COM_AXIS][SBCF_VAL_MOMENT_Y];
		pMax[T_SIET_EVAL_MEMB_CONCU::kAxial].dMz = max.dblForce[SBCF_POSI_I][SBCF_POSI_SUB_I][SBCF_COM_AXIS][SBCF_VAL_MOMENT_Z];

		pMax[T_SIET_EVAL_MEMB_CONCU::kShearY].dFx = max.dblForce[SBCF_POSI_I][SBCF_POSI_SUB_I][SBCF_COM_SHEAR_Y][SBCF_VAL_AXIS];
		pMax[T_SIET_EVAL_MEMB_CONCU::kShearY].dFy = max.dblForce[SBCF_POSI_I][SBCF_POSI_SUB_I][SBCF_COM_SHEAR_Y][SBCF_VAL_SHEAR_Y];
		pMax[T_SIET_EVAL_MEMB_CONCU::kShearY].dFz = max.dblForce[SBCF_POSI_I][SBCF_POSI_SUB_I][SBCF_COM_SHEAR_Y][SBCF_VAL_SHEAR_Z];
		pMax[T_SIET_EVAL_MEMB_CONCU::kShearY].dMx = max.dblForce[SBCF_POSI_I][SBCF_POSI_SUB_I][SBCF_COM_SHEAR_Y][SBCF_VAL_TORSION];
		pMax[T_SIET_EVAL_MEMB_CONCU::kShearY].dMy = max.dblForce[SBCF_POSI_I][SBCF_POSI_SUB_I][SBCF_COM_SHEAR_Y][SBCF_VAL_MOMENT_Y];
		pMax[T_SIET_EVAL_MEMB_CONCU::kShearY].dMz = max.dblForce[SBCF_POSI_I][SBCF_POSI_SUB_I][SBCF_COM_SHEAR_Y][SBCF_VAL_MOMENT_Z];

		pMax[T_SIET_EVAL_MEMB_CONCU::kShearZ].dFx = max.dblForce[SBCF_POSI_I][SBCF_POSI_SUB_I][SBCF_COM_SHEAR_Z][SBCF_VAL_AXIS];
		pMax[T_SIET_EVAL_MEMB_CONCU::kShearZ].dFy = max.dblForce[SBCF_POSI_I][SBCF_POSI_SUB_I][SBCF_COM_SHEAR_Z][SBCF_VAL_SHEAR_Y];
		pMax[T_SIET_EVAL_MEMB_CONCU::kShearZ].dFz = max.dblForce[SBCF_POSI_I][SBCF_POSI_SUB_I][SBCF_COM_SHEAR_Z][SBCF_VAL_SHEAR_Z];
		pMax[T_SIET_EVAL_MEMB_CONCU::kShearZ].dMx = max.dblForce[SBCF_POSI_I][SBCF_POSI_SUB_I][SBCF_COM_SHEAR_Z][SBCF_VAL_TORSION];
		pMax[T_SIET_EVAL_MEMB_CONCU::kShearZ].dMy = max.dblForce[SBCF_POSI_I][SBCF_POSI_SUB_I][SBCF_COM_SHEAR_Z][SBCF_VAL_MOMENT_Y];
		pMax[T_SIET_EVAL_MEMB_CONCU::kShearZ].dMz = max.dblForce[SBCF_POSI_I][SBCF_POSI_SUB_I][SBCF_COM_SHEAR_Z][SBCF_VAL_MOMENT_Z];

		pMax[T_SIET_EVAL_MEMB_CONCU::kTorsion].dFx = max.dblForce[SBCF_POSI_I][SBCF_POSI_SUB_I][SBCF_COM_TORSION][SBCF_VAL_AXIS];
		pMax[T_SIET_EVAL_MEMB_CONCU::kTorsion].dFy = max.dblForce[SBCF_POSI_I][SBCF_POSI_SUB_I][SBCF_COM_TORSION][SBCF_VAL_SHEAR_Y];
		pMax[T_SIET_EVAL_MEMB_CONCU::kTorsion].dFz = max.dblForce[SBCF_POSI_I][SBCF_POSI_SUB_I][SBCF_COM_TORSION][SBCF_VAL_SHEAR_Z];
		pMax[T_SIET_EVAL_MEMB_CONCU::kTorsion].dMx = max.dblForce[SBCF_POSI_I][SBCF_POSI_SUB_I][SBCF_COM_TORSION][SBCF_VAL_TORSION];
		pMax[T_SIET_EVAL_MEMB_CONCU::kTorsion].dMy = max.dblForce[SBCF_POSI_I][SBCF_POSI_SUB_I][SBCF_COM_TORSION][SBCF_VAL_MOMENT_Y];
		pMax[T_SIET_EVAL_MEMB_CONCU::kTorsion].dMz = max.dblForce[SBCF_POSI_I][SBCF_POSI_SUB_I][SBCF_COM_TORSION][SBCF_VAL_MOMENT_Z];

		pMax[T_SIET_EVAL_MEMB_CONCU::kMomentY].dFx = max.dblForce[SBCF_POSI_I][SBCF_POSI_SUB_I][SBCF_COM_MOMENT_Y][SBCF_VAL_AXIS];
		pMax[T_SIET_EVAL_MEMB_CONCU::kMomentY].dFy = max.dblForce[SBCF_POSI_I][SBCF_POSI_SUB_I][SBCF_COM_MOMENT_Y][SBCF_VAL_SHEAR_Y];
		pMax[T_SIET_EVAL_MEMB_CONCU::kMomentY].dFz = max.dblForce[SBCF_POSI_I][SBCF_POSI_SUB_I][SBCF_COM_MOMENT_Y][SBCF_VAL_SHEAR_Z];
		pMax[T_SIET_EVAL_MEMB_CONCU::kMomentY].dMx = max.dblForce[SBCF_POSI_I][SBCF_POSI_SUB_I][SBCF_COM_MOMENT_Y][SBCF_VAL_TORSION];
		pMax[T_SIET_EVAL_MEMB_CONCU::kMomentY].dMy = max.dblForce[SBCF_POSI_I][SBCF_POSI_SUB_I][SBCF_COM_MOMENT_Y][SBCF_VAL_MOMENT_Y];
		pMax[T_SIET_EVAL_MEMB_CONCU::kMomentY].dMz = max.dblForce[SBCF_POSI_I][SBCF_POSI_SUB_I][SBCF_COM_MOMENT_Y][SBCF_VAL_MOMENT_Z];

		pMax[T_SIET_EVAL_MEMB_CONCU::kMomentZ].dFx = max.dblForce[SBCF_POSI_I][SBCF_POSI_SUB_I][SBCF_COM_MOMENT_Z][SBCF_VAL_AXIS];
		pMax[T_SIET_EVAL_MEMB_CONCU::kMomentZ].dFy = max.dblForce[SBCF_POSI_I][SBCF_POSI_SUB_I][SBCF_COM_MOMENT_Z][SBCF_VAL_SHEAR_Y];
		pMax[T_SIET_EVAL_MEMB_CONCU::kMomentZ].dFz = max.dblForce[SBCF_POSI_I][SBCF_POSI_SUB_I][SBCF_COM_MOMENT_Z][SBCF_VAL_SHEAR_Z];
		pMax[T_SIET_EVAL_MEMB_CONCU::kMomentZ].dMx = max.dblForce[SBCF_POSI_I][SBCF_POSI_SUB_I][SBCF_COM_MOMENT_Z][SBCF_VAL_TORSION];
		pMax[T_SIET_EVAL_MEMB_CONCU::kMomentZ].dMy = max.dblForce[SBCF_POSI_I][SBCF_POSI_SUB_I][SBCF_COM_MOMENT_Z][SBCF_VAL_MOMENT_Y];
		pMax[T_SIET_EVAL_MEMB_CONCU::kMomentZ].dMz = max.dblForce[SBCF_POSI_I][SBCF_POSI_SUB_I][SBCF_COM_MOMENT_Z][SBCF_VAL_MOMENT_Z];

		//Min
		pMin[T_SIET_EVAL_MEMB_CONCU::kAxial].dFx = min.dblForce[SBCF_POSI_I][SBCF_POSI_SUB_I][SBCF_COM_AXIS][SBCF_VAL_AXIS];
		pMin[T_SIET_EVAL_MEMB_CONCU::kAxial].dFy = min.dblForce[SBCF_POSI_I][SBCF_POSI_SUB_I][SBCF_COM_AXIS][SBCF_VAL_SHEAR_Y];
		pMin[T_SIET_EVAL_MEMB_CONCU::kAxial].dFz = min.dblForce[SBCF_POSI_I][SBCF_POSI_SUB_I][SBCF_COM_AXIS][SBCF_VAL_SHEAR_Z];
		pMin[T_SIET_EVAL_MEMB_CONCU::kAxial].dMx = min.dblForce[SBCF_POSI_I][SBCF_POSI_SUB_I][SBCF_COM_AXIS][SBCF_VAL_TORSION];
		pMin[T_SIET_EVAL_MEMB_CONCU::kAxial].dMy = min.dblForce[SBCF_POSI_I][SBCF_POSI_SUB_I][SBCF_COM_AXIS][SBCF_VAL_MOMENT_Y];
		pMin[T_SIET_EVAL_MEMB_CONCU::kAxial].dMz = min.dblForce[SBCF_POSI_I][SBCF_POSI_SUB_I][SBCF_COM_AXIS][SBCF_VAL_MOMENT_Z];

		pMin[T_SIET_EVAL_MEMB_CONCU::kShearY].dFx = min.dblForce[SBCF_POSI_I][SBCF_POSI_SUB_I][SBCF_COM_SHEAR_Y][SBCF_VAL_AXIS];
		pMin[T_SIET_EVAL_MEMB_CONCU::kShearY].dFy = min.dblForce[SBCF_POSI_I][SBCF_POSI_SUB_I][SBCF_COM_SHEAR_Y][SBCF_VAL_SHEAR_Y];
		pMin[T_SIET_EVAL_MEMB_CONCU::kShearY].dFz = min.dblForce[SBCF_POSI_I][SBCF_POSI_SUB_I][SBCF_COM_SHEAR_Y][SBCF_VAL_SHEAR_Z];
		pMin[T_SIET_EVAL_MEMB_CONCU::kShearY].dMx = min.dblForce[SBCF_POSI_I][SBCF_POSI_SUB_I][SBCF_COM_SHEAR_Y][SBCF_VAL_TORSION];
		pMin[T_SIET_EVAL_MEMB_CONCU::kShearY].dMy = min.dblForce[SBCF_POSI_I][SBCF_POSI_SUB_I][SBCF_COM_SHEAR_Y][SBCF_VAL_MOMENT_Y];
		pMin[T_SIET_EVAL_MEMB_CONCU::kShearY].dMz = min.dblForce[SBCF_POSI_I][SBCF_POSI_SUB_I][SBCF_COM_SHEAR_Y][SBCF_VAL_MOMENT_Z];

		pMin[T_SIET_EVAL_MEMB_CONCU::kShearZ].dFx = min.dblForce[SBCF_POSI_I][SBCF_POSI_SUB_I][SBCF_COM_SHEAR_Z][SBCF_VAL_AXIS];
		pMin[T_SIET_EVAL_MEMB_CONCU::kShearZ].dFy = min.dblForce[SBCF_POSI_I][SBCF_POSI_SUB_I][SBCF_COM_SHEAR_Z][SBCF_VAL_SHEAR_Y];
		pMin[T_SIET_EVAL_MEMB_CONCU::kShearZ].dFz = min.dblForce[SBCF_POSI_I][SBCF_POSI_SUB_I][SBCF_COM_SHEAR_Z][SBCF_VAL_SHEAR_Z];
		pMin[T_SIET_EVAL_MEMB_CONCU::kShearZ].dMx = min.dblForce[SBCF_POSI_I][SBCF_POSI_SUB_I][SBCF_COM_SHEAR_Z][SBCF_VAL_TORSION];
		pMin[T_SIET_EVAL_MEMB_CONCU::kShearZ].dMy = min.dblForce[SBCF_POSI_I][SBCF_POSI_SUB_I][SBCF_COM_SHEAR_Z][SBCF_VAL_MOMENT_Y];
		pMin[T_SIET_EVAL_MEMB_CONCU::kShearZ].dMz = min.dblForce[SBCF_POSI_I][SBCF_POSI_SUB_I][SBCF_COM_SHEAR_Z][SBCF_VAL_MOMENT_Z];

		pMin[T_SIET_EVAL_MEMB_CONCU::kTorsion].dFx = min.dblForce[SBCF_POSI_I][SBCF_POSI_SUB_I][SBCF_COM_TORSION][SBCF_VAL_AXIS];
		pMin[T_SIET_EVAL_MEMB_CONCU::kTorsion].dFy = min.dblForce[SBCF_POSI_I][SBCF_POSI_SUB_I][SBCF_COM_TORSION][SBCF_VAL_SHEAR_Y];
		pMin[T_SIET_EVAL_MEMB_CONCU::kTorsion].dFz = min.dblForce[SBCF_POSI_I][SBCF_POSI_SUB_I][SBCF_COM_TORSION][SBCF_VAL_SHEAR_Z];
		pMin[T_SIET_EVAL_MEMB_CONCU::kTorsion].dMx = min.dblForce[SBCF_POSI_I][SBCF_POSI_SUB_I][SBCF_COM_TORSION][SBCF_VAL_TORSION];
		pMin[T_SIET_EVAL_MEMB_CONCU::kTorsion].dMy = min.dblForce[SBCF_POSI_I][SBCF_POSI_SUB_I][SBCF_COM_TORSION][SBCF_VAL_MOMENT_Y];
		pMin[T_SIET_EVAL_MEMB_CONCU::kTorsion].dMz = min.dblForce[SBCF_POSI_I][SBCF_POSI_SUB_I][SBCF_COM_TORSION][SBCF_VAL_MOMENT_Z];

		pMin[T_SIET_EVAL_MEMB_CONCU::kMomentY].dFx = min.dblForce[SBCF_POSI_I][SBCF_POSI_SUB_I][SBCF_COM_MOMENT_Y][SBCF_VAL_AXIS];
		pMin[T_SIET_EVAL_MEMB_CONCU::kMomentY].dFy = min.dblForce[SBCF_POSI_I][SBCF_POSI_SUB_I][SBCF_COM_MOMENT_Y][SBCF_VAL_SHEAR_Y];
		pMin[T_SIET_EVAL_MEMB_CONCU::kMomentY].dFz = min.dblForce[SBCF_POSI_I][SBCF_POSI_SUB_I][SBCF_COM_MOMENT_Y][SBCF_VAL_SHEAR_Z];
		pMin[T_SIET_EVAL_MEMB_CONCU::kMomentY].dMx = min.dblForce[SBCF_POSI_I][SBCF_POSI_SUB_I][SBCF_COM_MOMENT_Y][SBCF_VAL_TORSION];
		pMin[T_SIET_EVAL_MEMB_CONCU::kMomentY].dMy = min.dblForce[SBCF_POSI_I][SBCF_POSI_SUB_I][SBCF_COM_MOMENT_Y][SBCF_VAL_MOMENT_Y];
		pMin[T_SIET_EVAL_MEMB_CONCU::kMomentY].dMz = min.dblForce[SBCF_POSI_I][SBCF_POSI_SUB_I][SBCF_COM_MOMENT_Y][SBCF_VAL_MOMENT_Z];

		pMin[T_SIET_EVAL_MEMB_CONCU::kMomentZ].dFx = min.dblForce[SBCF_POSI_I][SBCF_POSI_SUB_I][SBCF_COM_MOMENT_Z][SBCF_VAL_AXIS];
		pMin[T_SIET_EVAL_MEMB_CONCU::kMomentZ].dFy = min.dblForce[SBCF_POSI_I][SBCF_POSI_SUB_I][SBCF_COM_MOMENT_Z][SBCF_VAL_SHEAR_Y];
		pMin[T_SIET_EVAL_MEMB_CONCU::kMomentZ].dFz = min.dblForce[SBCF_POSI_I][SBCF_POSI_SUB_I][SBCF_COM_MOMENT_Z][SBCF_VAL_SHEAR_Z];
		pMin[T_SIET_EVAL_MEMB_CONCU::kMomentZ].dMx = min.dblForce[SBCF_POSI_I][SBCF_POSI_SUB_I][SBCF_COM_MOMENT_Z][SBCF_VAL_TORSION];
		pMin[T_SIET_EVAL_MEMB_CONCU::kMomentZ].dMy = min.dblForce[SBCF_POSI_I][SBCF_POSI_SUB_I][SBCF_COM_MOMENT_Z][SBCF_VAL_MOMENT_Y];
		pMin[T_SIET_EVAL_MEMB_CONCU::kMomentZ].dMz = min.dblForce[SBCF_POSI_I][SBCF_POSI_SUB_I][SBCF_COM_MOMENT_Z][SBCF_VAL_MOMENT_Z];
	}
	else if (pGroup->ePosNodeLow == T_SIET_EVAL_MEMB_GROUP::kJ) {
		pMax[T_SIET_EVAL_MEMB_CONCU::kAxial].dFx = max.dblForce[SBCF_POSI_J][SBCF_POSI_SUB_J][SBCF_COM_AXIS][SBCF_VAL_AXIS];
		pMax[T_SIET_EVAL_MEMB_CONCU::kAxial].dFy = max.dblForce[SBCF_POSI_J][SBCF_POSI_SUB_J][SBCF_COM_AXIS][SBCF_VAL_SHEAR_Y];
		pMax[T_SIET_EVAL_MEMB_CONCU::kAxial].dFz = max.dblForce[SBCF_POSI_J][SBCF_POSI_SUB_J][SBCF_COM_AXIS][SBCF_VAL_SHEAR_Z];
		pMax[T_SIET_EVAL_MEMB_CONCU::kAxial].dMx = max.dblForce[SBCF_POSI_J][SBCF_POSI_SUB_J][SBCF_COM_AXIS][SBCF_VAL_TORSION];
		pMax[T_SIET_EVAL_MEMB_CONCU::kAxial].dMy = max.dblForce[SBCF_POSI_J][SBCF_POSI_SUB_J][SBCF_COM_AXIS][SBCF_VAL_MOMENT_Y];
		pMax[T_SIET_EVAL_MEMB_CONCU::kAxial].dMz = max.dblForce[SBCF_POSI_J][SBCF_POSI_SUB_J][SBCF_COM_AXIS][SBCF_VAL_MOMENT_Z];

		pMax[T_SIET_EVAL_MEMB_CONCU::kShearY].dFx = max.dblForce[SBCF_POSI_J][SBCF_POSI_SUB_J][SBCF_COM_SHEAR_Y][SBCF_VAL_AXIS];
		pMax[T_SIET_EVAL_MEMB_CONCU::kShearY].dFy = max.dblForce[SBCF_POSI_J][SBCF_POSI_SUB_J][SBCF_COM_SHEAR_Y][SBCF_VAL_SHEAR_Y];
		pMax[T_SIET_EVAL_MEMB_CONCU::kShearY].dFz = max.dblForce[SBCF_POSI_J][SBCF_POSI_SUB_J][SBCF_COM_SHEAR_Y][SBCF_VAL_SHEAR_Z];
		pMax[T_SIET_EVAL_MEMB_CONCU::kShearY].dMx = max.dblForce[SBCF_POSI_J][SBCF_POSI_SUB_J][SBCF_COM_SHEAR_Y][SBCF_VAL_TORSION];
		pMax[T_SIET_EVAL_MEMB_CONCU::kShearY].dMy = max.dblForce[SBCF_POSI_J][SBCF_POSI_SUB_J][SBCF_COM_SHEAR_Y][SBCF_VAL_MOMENT_Y];
		pMax[T_SIET_EVAL_MEMB_CONCU::kShearY].dMz = max.dblForce[SBCF_POSI_J][SBCF_POSI_SUB_J][SBCF_COM_SHEAR_Y][SBCF_VAL_MOMENT_Z];

		pMax[T_SIET_EVAL_MEMB_CONCU::kShearZ].dFx = max.dblForce[SBCF_POSI_J][SBCF_POSI_SUB_J][SBCF_COM_SHEAR_Z][SBCF_VAL_AXIS];
		pMax[T_SIET_EVAL_MEMB_CONCU::kShearZ].dFy = max.dblForce[SBCF_POSI_J][SBCF_POSI_SUB_J][SBCF_COM_SHEAR_Z][SBCF_VAL_SHEAR_Y];
		pMax[T_SIET_EVAL_MEMB_CONCU::kShearZ].dFz = max.dblForce[SBCF_POSI_J][SBCF_POSI_SUB_J][SBCF_COM_SHEAR_Z][SBCF_VAL_SHEAR_Z];
		pMax[T_SIET_EVAL_MEMB_CONCU::kShearZ].dMx = max.dblForce[SBCF_POSI_J][SBCF_POSI_SUB_J][SBCF_COM_SHEAR_Z][SBCF_VAL_TORSION];
		pMax[T_SIET_EVAL_MEMB_CONCU::kShearZ].dMy = max.dblForce[SBCF_POSI_J][SBCF_POSI_SUB_J][SBCF_COM_SHEAR_Z][SBCF_VAL_MOMENT_Y];
		pMax[T_SIET_EVAL_MEMB_CONCU::kShearZ].dMz = max.dblForce[SBCF_POSI_J][SBCF_POSI_SUB_J][SBCF_COM_SHEAR_Z][SBCF_VAL_MOMENT_Z];

		pMax[T_SIET_EVAL_MEMB_CONCU::kTorsion].dFx = max.dblForce[SBCF_POSI_J][SBCF_POSI_SUB_J][SBCF_COM_TORSION][SBCF_VAL_AXIS];
		pMax[T_SIET_EVAL_MEMB_CONCU::kTorsion].dFy = max.dblForce[SBCF_POSI_J][SBCF_POSI_SUB_J][SBCF_COM_TORSION][SBCF_VAL_SHEAR_Y];
		pMax[T_SIET_EVAL_MEMB_CONCU::kTorsion].dFz = max.dblForce[SBCF_POSI_J][SBCF_POSI_SUB_J][SBCF_COM_TORSION][SBCF_VAL_SHEAR_Z];
		pMax[T_SIET_EVAL_MEMB_CONCU::kTorsion].dMx = max.dblForce[SBCF_POSI_J][SBCF_POSI_SUB_J][SBCF_COM_TORSION][SBCF_VAL_TORSION];
		pMax[T_SIET_EVAL_MEMB_CONCU::kTorsion].dMy = max.dblForce[SBCF_POSI_J][SBCF_POSI_SUB_J][SBCF_COM_TORSION][SBCF_VAL_MOMENT_Y];
		pMax[T_SIET_EVAL_MEMB_CONCU::kTorsion].dMz = max.dblForce[SBCF_POSI_J][SBCF_POSI_SUB_J][SBCF_COM_TORSION][SBCF_VAL_MOMENT_Z];

		pMax[T_SIET_EVAL_MEMB_CONCU::kMomentY].dFx = max.dblForce[SBCF_POSI_J][SBCF_POSI_SUB_J][SBCF_COM_MOMENT_Y][SBCF_VAL_AXIS];
		pMax[T_SIET_EVAL_MEMB_CONCU::kMomentY].dFy = max.dblForce[SBCF_POSI_J][SBCF_POSI_SUB_J][SBCF_COM_MOMENT_Y][SBCF_VAL_SHEAR_Y];
		pMax[T_SIET_EVAL_MEMB_CONCU::kMomentY].dFz = max.dblForce[SBCF_POSI_J][SBCF_POSI_SUB_J][SBCF_COM_MOMENT_Y][SBCF_VAL_SHEAR_Z];
		pMax[T_SIET_EVAL_MEMB_CONCU::kMomentY].dMx = max.dblForce[SBCF_POSI_J][SBCF_POSI_SUB_J][SBCF_COM_MOMENT_Y][SBCF_VAL_TORSION];
		pMax[T_SIET_EVAL_MEMB_CONCU::kMomentY].dMy = max.dblForce[SBCF_POSI_J][SBCF_POSI_SUB_J][SBCF_COM_MOMENT_Y][SBCF_VAL_MOMENT_Y];
		pMax[T_SIET_EVAL_MEMB_CONCU::kMomentY].dMz = max.dblForce[SBCF_POSI_J][SBCF_POSI_SUB_J][SBCF_COM_MOMENT_Y][SBCF_VAL_MOMENT_Z];

		pMax[T_SIET_EVAL_MEMB_CONCU::kMomentZ].dFx = max.dblForce[SBCF_POSI_J][SBCF_POSI_SUB_J][SBCF_COM_MOMENT_Z][SBCF_VAL_AXIS];
		pMax[T_SIET_EVAL_MEMB_CONCU::kMomentZ].dFy = max.dblForce[SBCF_POSI_J][SBCF_POSI_SUB_J][SBCF_COM_MOMENT_Z][SBCF_VAL_SHEAR_Y];
		pMax[T_SIET_EVAL_MEMB_CONCU::kMomentZ].dFz = max.dblForce[SBCF_POSI_J][SBCF_POSI_SUB_J][SBCF_COM_MOMENT_Z][SBCF_VAL_SHEAR_Z];
		pMax[T_SIET_EVAL_MEMB_CONCU::kMomentZ].dMx = max.dblForce[SBCF_POSI_J][SBCF_POSI_SUB_J][SBCF_COM_MOMENT_Z][SBCF_VAL_TORSION];
		pMax[T_SIET_EVAL_MEMB_CONCU::kMomentZ].dMy = max.dblForce[SBCF_POSI_J][SBCF_POSI_SUB_J][SBCF_COM_MOMENT_Z][SBCF_VAL_MOMENT_Y];
		pMax[T_SIET_EVAL_MEMB_CONCU::kMomentZ].dMz = max.dblForce[SBCF_POSI_J][SBCF_POSI_SUB_J][SBCF_COM_MOMENT_Z][SBCF_VAL_MOMENT_Z];

		//Min
		pMin[T_SIET_EVAL_MEMB_CONCU::kAxial].dFx = min.dblForce[SBCF_POSI_J][SBCF_POSI_SUB_J][SBCF_COM_AXIS][SBCF_VAL_AXIS];
		pMin[T_SIET_EVAL_MEMB_CONCU::kAxial].dFy = min.dblForce[SBCF_POSI_J][SBCF_POSI_SUB_J][SBCF_COM_AXIS][SBCF_VAL_SHEAR_Y];
		pMin[T_SIET_EVAL_MEMB_CONCU::kAxial].dFz = min.dblForce[SBCF_POSI_J][SBCF_POSI_SUB_J][SBCF_COM_AXIS][SBCF_VAL_SHEAR_Z];
		pMin[T_SIET_EVAL_MEMB_CONCU::kAxial].dMx = min.dblForce[SBCF_POSI_J][SBCF_POSI_SUB_J][SBCF_COM_AXIS][SBCF_VAL_TORSION];
		pMin[T_SIET_EVAL_MEMB_CONCU::kAxial].dMy = min.dblForce[SBCF_POSI_J][SBCF_POSI_SUB_J][SBCF_COM_AXIS][SBCF_VAL_MOMENT_Y];
		pMin[T_SIET_EVAL_MEMB_CONCU::kAxial].dMz = min.dblForce[SBCF_POSI_J][SBCF_POSI_SUB_J][SBCF_COM_AXIS][SBCF_VAL_MOMENT_Z];

		pMin[T_SIET_EVAL_MEMB_CONCU::kShearY].dFx = min.dblForce[SBCF_POSI_J][SBCF_POSI_SUB_J][SBCF_COM_SHEAR_Y][SBCF_VAL_AXIS];
		pMin[T_SIET_EVAL_MEMB_CONCU::kShearY].dFy = min.dblForce[SBCF_POSI_J][SBCF_POSI_SUB_J][SBCF_COM_SHEAR_Y][SBCF_VAL_SHEAR_Y];
		pMin[T_SIET_EVAL_MEMB_CONCU::kShearY].dFz = min.dblForce[SBCF_POSI_J][SBCF_POSI_SUB_J][SBCF_COM_SHEAR_Y][SBCF_VAL_SHEAR_Z];
		pMin[T_SIET_EVAL_MEMB_CONCU::kShearY].dMx = min.dblForce[SBCF_POSI_J][SBCF_POSI_SUB_J][SBCF_COM_SHEAR_Y][SBCF_VAL_TORSION];
		pMin[T_SIET_EVAL_MEMB_CONCU::kShearY].dMy = min.dblForce[SBCF_POSI_J][SBCF_POSI_SUB_J][SBCF_COM_SHEAR_Y][SBCF_VAL_MOMENT_Y];
		pMin[T_SIET_EVAL_MEMB_CONCU::kShearY].dMz = min.dblForce[SBCF_POSI_J][SBCF_POSI_SUB_J][SBCF_COM_SHEAR_Y][SBCF_VAL_MOMENT_Z];

		pMin[T_SIET_EVAL_MEMB_CONCU::kShearZ].dFx = min.dblForce[SBCF_POSI_J][SBCF_POSI_SUB_J][SBCF_COM_SHEAR_Z][SBCF_VAL_AXIS];
		pMin[T_SIET_EVAL_MEMB_CONCU::kShearZ].dFy = min.dblForce[SBCF_POSI_J][SBCF_POSI_SUB_J][SBCF_COM_SHEAR_Z][SBCF_VAL_SHEAR_Y];
		pMin[T_SIET_EVAL_MEMB_CONCU::kShearZ].dFz = min.dblForce[SBCF_POSI_J][SBCF_POSI_SUB_J][SBCF_COM_SHEAR_Z][SBCF_VAL_SHEAR_Z];
		pMin[T_SIET_EVAL_MEMB_CONCU::kShearZ].dMx = min.dblForce[SBCF_POSI_J][SBCF_POSI_SUB_J][SBCF_COM_SHEAR_Z][SBCF_VAL_TORSION];
		pMin[T_SIET_EVAL_MEMB_CONCU::kShearZ].dMy = min.dblForce[SBCF_POSI_J][SBCF_POSI_SUB_J][SBCF_COM_SHEAR_Z][SBCF_VAL_MOMENT_Y];
		pMin[T_SIET_EVAL_MEMB_CONCU::kShearZ].dMz = min.dblForce[SBCF_POSI_J][SBCF_POSI_SUB_J][SBCF_COM_SHEAR_Z][SBCF_VAL_MOMENT_Z];

		pMin[T_SIET_EVAL_MEMB_CONCU::kTorsion].dFx = min.dblForce[SBCF_POSI_J][SBCF_POSI_SUB_J][SBCF_COM_TORSION][SBCF_VAL_AXIS];
		pMin[T_SIET_EVAL_MEMB_CONCU::kTorsion].dFy = min.dblForce[SBCF_POSI_J][SBCF_POSI_SUB_J][SBCF_COM_TORSION][SBCF_VAL_SHEAR_Y];
		pMin[T_SIET_EVAL_MEMB_CONCU::kTorsion].dFz = min.dblForce[SBCF_POSI_J][SBCF_POSI_SUB_J][SBCF_COM_TORSION][SBCF_VAL_SHEAR_Z];
		pMin[T_SIET_EVAL_MEMB_CONCU::kTorsion].dMx = min.dblForce[SBCF_POSI_J][SBCF_POSI_SUB_J][SBCF_COM_TORSION][SBCF_VAL_TORSION];
		pMin[T_SIET_EVAL_MEMB_CONCU::kTorsion].dMy = min.dblForce[SBCF_POSI_J][SBCF_POSI_SUB_J][SBCF_COM_TORSION][SBCF_VAL_MOMENT_Y];
		pMin[T_SIET_EVAL_MEMB_CONCU::kTorsion].dMz = min.dblForce[SBCF_POSI_J][SBCF_POSI_SUB_J][SBCF_COM_TORSION][SBCF_VAL_MOMENT_Z];

		pMin[T_SIET_EVAL_MEMB_CONCU::kMomentY].dFx = min.dblForce[SBCF_POSI_J][SBCF_POSI_SUB_J][SBCF_COM_MOMENT_Y][SBCF_VAL_AXIS];
		pMin[T_SIET_EVAL_MEMB_CONCU::kMomentY].dFy = min.dblForce[SBCF_POSI_J][SBCF_POSI_SUB_J][SBCF_COM_MOMENT_Y][SBCF_VAL_SHEAR_Y];
		pMin[T_SIET_EVAL_MEMB_CONCU::kMomentY].dFz = min.dblForce[SBCF_POSI_J][SBCF_POSI_SUB_J][SBCF_COM_MOMENT_Y][SBCF_VAL_SHEAR_Z];
		pMin[T_SIET_EVAL_MEMB_CONCU::kMomentY].dMx = min.dblForce[SBCF_POSI_J][SBCF_POSI_SUB_J][SBCF_COM_MOMENT_Y][SBCF_VAL_TORSION];
		pMin[T_SIET_EVAL_MEMB_CONCU::kMomentY].dMy = min.dblForce[SBCF_POSI_J][SBCF_POSI_SUB_J][SBCF_COM_MOMENT_Y][SBCF_VAL_MOMENT_Y];
		pMin[T_SIET_EVAL_MEMB_CONCU::kMomentY].dMz = min.dblForce[SBCF_POSI_J][SBCF_POSI_SUB_J][SBCF_COM_MOMENT_Y][SBCF_VAL_MOMENT_Z];

		pMin[T_SIET_EVAL_MEMB_CONCU::kMomentZ].dFx = min.dblForce[SBCF_POSI_J][SBCF_POSI_SUB_J][SBCF_COM_MOMENT_Z][SBCF_VAL_AXIS];
		pMin[T_SIET_EVAL_MEMB_CONCU::kMomentZ].dFy = min.dblForce[SBCF_POSI_J][SBCF_POSI_SUB_J][SBCF_COM_MOMENT_Z][SBCF_VAL_SHEAR_Y];
		pMin[T_SIET_EVAL_MEMB_CONCU::kMomentZ].dFz = min.dblForce[SBCF_POSI_J][SBCF_POSI_SUB_J][SBCF_COM_MOMENT_Z][SBCF_VAL_SHEAR_Z];
		pMin[T_SIET_EVAL_MEMB_CONCU::kMomentZ].dMx = min.dblForce[SBCF_POSI_J][SBCF_POSI_SUB_J][SBCF_COM_MOMENT_Z][SBCF_VAL_TORSION];
		pMin[T_SIET_EVAL_MEMB_CONCU::kMomentZ].dMy = min.dblForce[SBCF_POSI_J][SBCF_POSI_SUB_J][SBCF_COM_MOMENT_Z][SBCF_VAL_MOMENT_Y];
		pMin[T_SIET_EVAL_MEMB_CONCU::kMomentZ].dMz = min.dblForce[SBCF_POSI_J][SBCF_POSI_SUB_J][SBCF_COM_MOMENT_Z][SBCF_VAL_MOMENT_Z];
	}

	return TRUE;
}

BOOL CSeismicDesignSiet_PierBase::GetConcurvityFootingLow()
{
	// save result
	CBrdgSeisEvalResult_JP* pResult = m_pDoc->m_pPostCtrl->GetBrdgSeisEvalResult_JP(); if (!pResult) { ASSERT(0); return FALSE; }

	if (m_pSiet->bFootingLower_Sf == FALSE || m_pSiet->bPierBase_Sf == FALSE) return FALSE;

	CArray<ConcNode, ConcNode> masters;
	GetConcMasterNode(masters);

	CArray<ConcNode, ConcNode> subNodes;
	GetConcSubNode(subNodes);

	for (int i = 0; i < masters.GetCount(); i++)
	{
		ConcNode& master = masters[i];

		T_SIET_EVAL_FOOT_MASTER eval;
		eval.kElem = master.kElem;
		eval.kSipa = m_pSiet->kSipa;
		eval.kSigrLower = m_pSiet->kSigrLower;
		eval.nSeismicForceType = m_pSipa->nSeismicForceType;
		eval.nSeismicForceDirection = m_pSipa->nSeismicForceDirection;

		T_SIET_EVAL_FOOT_CONCU averge[T_SIET_EVAL_FOOT_MASTER::kForceNum];

		int nSize = m_pSipa->aSeisThisK.GetCount();
		for (int j = 0; j < nSize; j++)
		{
			const T_THIS_K& kThis = m_pSipa->aSeisThisK[j];
			GetTimeCount(kThis, master);

			T_SIET_EVAL_FOOT_CONCU force[T_SIET_EVAL_FOOT_MASTER::kForceNum];
			// 동시성 결과
			if (GetConcurvityForce(kThis, master, subNodes, force) == FALSE) continue;

			averge[T_SIET_EVAL_MEMB_CONCU::kAxial] += force[T_SIET_EVAL_MEMB_CONCU::kAxial];
			averge[T_SIET_EVAL_MEMB_CONCU::kShearY] += force[T_SIET_EVAL_MEMB_CONCU::kShearY];
			averge[T_SIET_EVAL_MEMB_CONCU::kShearZ] += force[T_SIET_EVAL_MEMB_CONCU::kShearZ];
			averge[T_SIET_EVAL_MEMB_CONCU::kTorsion] += force[T_SIET_EVAL_MEMB_CONCU::kTorsion];
			averge[T_SIET_EVAL_MEMB_CONCU::kMomentY] += force[T_SIET_EVAL_MEMB_CONCU::kMomentY];
			averge[T_SIET_EVAL_MEMB_CONCU::kMomentZ] += force[T_SIET_EVAL_MEMB_CONCU::kMomentZ];
		}

		averge[T_SIET_EVAL_MEMB_CONCU::kAxial] /= nSize;
		averge[T_SIET_EVAL_MEMB_CONCU::kShearY] /= nSize;
		averge[T_SIET_EVAL_MEMB_CONCU::kShearZ] /= nSize;
		averge[T_SIET_EVAL_MEMB_CONCU::kTorsion] /= nSize;
		averge[T_SIET_EVAL_MEMB_CONCU::kMomentY] /= nSize;
		averge[T_SIET_EVAL_MEMB_CONCU::kMomentZ] /= nSize;

		eval.FootConcu[T_SIET_EVAL_MEMB_CONCU::kAxial] = averge[T_SIET_EVAL_MEMB_CONCU::kAxial];
		eval.FootConcu[T_SIET_EVAL_MEMB_CONCU::kShearY] = averge[T_SIET_EVAL_MEMB_CONCU::kShearY];
		eval.FootConcu[T_SIET_EVAL_MEMB_CONCU::kShearZ] = averge[T_SIET_EVAL_MEMB_CONCU::kShearZ];
		eval.FootConcu[T_SIET_EVAL_MEMB_CONCU::kTorsion] = averge[T_SIET_EVAL_MEMB_CONCU::kTorsion];
		eval.FootConcu[T_SIET_EVAL_MEMB_CONCU::kMomentY] = averge[T_SIET_EVAL_MEMB_CONCU::kMomentY];
		eval.FootConcu[T_SIET_EVAL_MEMB_CONCU::kMomentZ] = averge[T_SIET_EVAL_MEMB_CONCU::kMomentZ];

		pResult->SetResultSietConcFootingLow(m_pSiet->kSipa, m_pSiet->kSigrLower, master.kElem, eval);
	}

	return TRUE;
}

void CSeismicDesignSiet_PierBase::GetConcMasterNode(OUT CArray<ConcNode, ConcNode>& masters)
{
	for (int i = 0; i < m_pSigrLower->FOOT_D.GetCount(); i++)
	{
		const T_SIGR_LOWER_FOOT_D& foot = m_pSigrLower->FOOT_D[i];

		T_ELEM_K kElem = 0; T_NODE_K kNode = 0;
		int nPos = T_SIET_EVAL_MEMB_GROUP::kUnKnown;
		GetBotNodeElemKey(foot.aInvestigatedElem, kElem, nPos);

		T_ELEM_D elem;
		if (m_pDoc->m_pAttrCtrl->GetElem(kElem, elem) == FALSE) { ASSERT(FALSE); continue; }

		if (nPos == T_SIET_EVAL_MEMB_GROUP::kI)	kNode = elem.elnod[0];
		else if (nPos == T_SIET_EVAL_MEMB_GROUP::kJ) kNode = elem.elnod[1];

		ConcNode masterNode;
		masterNode.strName = foot.strName;
		masterNode.kElem = kElem;
		masterNode.kNode = kNode;
		masterNode.ePosNodeLow = nPos;
		masters.Add(masterNode);
	}
}

void CSeismicDesignSiet_PierBase::GetTimeCount(const T_THIS_K& kThis, OUT ConcNode& masterNode)
{
	m_pDoc->m_pPostCtrl->SelectThis(kThis);

	T_STRB_D min[2]; T_STRB_D max[2];
	if (m_pDoc->m_pPostCtrl->GetThisInfo()->GetStrbWithTime(masterNode.kElem, &max[0], &min[0], 0, &max[1], &min[1], 0) == FALSE) { ASSERT(FALSE); return; }

	if (masterNode.ePosNodeLow == T_SIET_EVAL_MEMB_GROUP::kI) {
		masterNode.dTimeCountMax[ConcNode::kAxial] = max[1].dblForce[STRB_POSI_I][STRB_AXIS_I];
		masterNode.dTimeCountMax[ConcNode::kShearY] = max[1].dblForce[STRB_POSI_I][STRB_SHEAR_Y_I];
		masterNode.dTimeCountMax[ConcNode::kShearZ] = max[1].dblForce[STRB_POSI_I][STRB_SHEAR_Z_I];
		masterNode.dTimeCountMax[ConcNode::kTorsion] = max[1].dblForce[STRB_POSI_I][STRB_TORSION_I];
		masterNode.dTimeCountMax[ConcNode::kMomentY] = max[1].dblForce[STRB_POSI_I][STRB_MOMENT_Y_I];
		masterNode.dTimeCountMax[ConcNode::kMomentZ] = max[1].dblForce[STRB_POSI_I][STRB_MOMENT_Z_I];

		masterNode.dTimeCountMin[ConcNode::kAxial] = min[1].dblForce[STRB_POSI_I][STRB_AXIS_I];
		masterNode.dTimeCountMin[ConcNode::kShearY] = min[1].dblForce[STRB_POSI_I][STRB_SHEAR_Y_I];
		masterNode.dTimeCountMin[ConcNode::kShearZ] = min[1].dblForce[STRB_POSI_I][STRB_SHEAR_Z_I];
		masterNode.dTimeCountMin[ConcNode::kTorsion] = min[1].dblForce[STRB_POSI_I][STRB_TORSION_I];
		masterNode.dTimeCountMin[ConcNode::kMomentY] = min[1].dblForce[STRB_POSI_I][STRB_MOMENT_Y_I];
		masterNode.dTimeCountMin[ConcNode::kMomentZ] = min[1].dblForce[STRB_POSI_I][STRB_MOMENT_Z_I];
	}
	else if (masterNode.ePosNodeLow == T_SIET_EVAL_MEMB_GROUP::kJ) {
		masterNode.dTimeCountMax[ConcNode::kAxial] = max[1].dblForce[STRB_POSI_J][STRB_AXIS_J];
		masterNode.dTimeCountMax[ConcNode::kShearY] = max[1].dblForce[STRB_POSI_J][STRB_SHEAR_Y_J];
		masterNode.dTimeCountMax[ConcNode::kShearZ] = max[1].dblForce[STRB_POSI_J][STRB_SHEAR_Z_J];
		masterNode.dTimeCountMax[ConcNode::kTorsion] = max[1].dblForce[STRB_POSI_J][STRB_TORSION_J];
		masterNode.dTimeCountMax[ConcNode::kMomentY] = max[1].dblForce[STRB_POSI_J][STRB_MOMENT_Y_J];
		masterNode.dTimeCountMax[ConcNode::kMomentZ] = max[1].dblForce[STRB_POSI_J][STRB_MOMENT_Z_J];

		masterNode.dTimeCountMin[ConcNode::kAxial] = min[1].dblForce[STRB_POSI_J][STRB_AXIS_J];
		masterNode.dTimeCountMin[ConcNode::kShearY] = min[1].dblForce[STRB_POSI_J][STRB_SHEAR_Y_J];
		masterNode.dTimeCountMin[ConcNode::kShearZ] = min[1].dblForce[STRB_POSI_J][STRB_SHEAR_Z_J];
		masterNode.dTimeCountMin[ConcNode::kTorsion] = min[1].dblForce[STRB_POSI_J][STRB_TORSION_J];
		masterNode.dTimeCountMin[ConcNode::kMomentY] = min[1].dblForce[STRB_POSI_J][STRB_MOMENT_Y_J];
		masterNode.dTimeCountMin[ConcNode::kMomentZ] = min[1].dblForce[STRB_POSI_J][STRB_MOMENT_Z_J];
	}
}

void CSeismicDesignSiet_PierBase::GetConcSubNode(OUT CArray<ConcNode, ConcNode>& subNodes)
{
	for (int i = 0; i < m_pSigrLower->COLM_D.GetCount(); i++)
	{
		const T_SIGR_LOWER_COLM_D& colm = m_pSigrLower->COLM_D[i];

		T_ELEM_K kElem = 0; T_NODE_K kNode = 0;
		int nPos = T_SIET_EVAL_MEMB_GROUP::kUnKnown;
		GetBotNodeElemKey(colm.aInvestigatedElem, kElem, nPos);

		T_ELEM_D elem;
		if (m_pDoc->m_pAttrCtrl->GetElem(kElem, elem) == FALSE) { ASSERT(FALSE); continue; }

		if (nPos == T_SIET_EVAL_MEMB_GROUP::kI)	kNode = elem.elnod[0];
		else if (nPos == T_SIET_EVAL_MEMB_GROUP::kJ) kNode = elem.elnod[1];

		ConcNode subNode;
		subNode.strName = colm.strName;
		subNode.kElem = kElem;
		subNode.kNode = kNode;
		subNode.ePosNodeLow = nPos;

		subNodes.Add(subNode);
	}
}

BOOL CSeismicDesignSiet_PierBase::GetConcurvityForce(const T_THIS_K& kThis, ConcNode& master, const CArray<ConcNode, ConcNode>& subNodes, OUT T_SIET_EVAL_FOOT_CONCU* pForce)
{
	CArray<ConcNode, ConcNode> concNodes;
	concNodes.Add(master);
	concNodes.Append(subNodes);

	CArray<double, double> aTimeStep;
	m_pDoc->m_pAttrCtrl->GetTimeStep(kThis, aTimeStep);
	int nTimeCount = aTimeStep.GetSize();

	for (int iForce = 0; iForce < ConcNode::kForceNum; iForce++)
	{
		int nMinTime = 0, nMaxTime = 0;
		GetTimeIndex(kThis, master.dTimeCountMin[iForce], nMinTime);
		GetTimeIndex(kThis, master.dTimeCountMax[iForce], nMaxTime);

		for (int i = 0; i < concNodes.GetCount(); i++)
		{
			const ConcNode& target = concNodes[i];

			T_SIET_EVAL_FOOT_GROUP min, max;
			min.nLocation = (i == 0) ? T_SIET_EVAL_FOOT_GROUP::kFootingBot : T_SIET_EVAL_FOOT_GROUP::kPierBase;
			min.strName = target.strName;
			min.kElem = target.kElem;
			min.ePosNodeLow = target.ePosNodeLow;
			max = min;

			if (GetTimeKrnlForceResult(kThis, nTimeCount, target, nMinTime, nMaxTime, &min, &max) == FALSE)
				return FALSE;

			pForce[iForce].aMax.Add(max);
			pForce[iForce].aMin.Add(min);
		}
	}

	return TRUE;
}

BOOL CSeismicDesignSiet_PierBase::GetTimeKrnlForceResult(const T_THIS_K& kThis, const int& nTimeCount, const ConcNode& target, const int& nMinTime, const int& nMaxTime,
	OUT T_SIET_EVAL_FOOT_GROUP* pMin, OUT T_SIET_EVAL_FOOT_GROUP* pMax)
{
	auto pThisInfo = m_pDoc->m_pPostCtrl->GetThisInfo();

	if (pThisInfo->IsBeamWithHnge(target.kElem) > 0) {
		return GetNlbmHistoryForce(target, nMinTime, nMaxTime, pMin, pMax);
	}

	for (int iForce = 0; iForce < ConcNode::kForceNum; iForce++)
	{
		CArray<double, double> arAllStepResult;

		int nSubType = RESULT_TYPE_SELECTION_FORCE;
		int nDBComp = iForce + 1;
		int nComp = pThisInfo->GetComponent4SolverPrntType1(target.kElem, nSubType, target.ePosNodeLow, nDBComp);

		if (!pThisInfo->GetTimeKrnlResult(
			target.kElem, kThis, nTimeCount, 0, TKRNL_TYPE_BEAM, nComp,//결과얻어오기위한변수  
			nSubType, nDBComp, 1,
			arAllStepResult))// 결과데이터 
		{
			ASSERT(0);
			return FALSE;
		}

		if (nMinTime >= arAllStepResult.GetSize() || nMaxTime >= arAllStepResult.GetSize())
		{
			ASSERT(0);
			return FALSE;
		}

		switch (iForce)
		{
		case ConcNode::kAxial:
			pMin->Force.dFx = arAllStepResult[nMinTime];
			pMax->Force.dFx = arAllStepResult[nMaxTime];
			break;
		case ConcNode::kShearY:
			pMin->Force.dFy = arAllStepResult[nMinTime];
			pMax->Force.dFy = arAllStepResult[nMaxTime];
			break;
		case ConcNode::kShearZ:
			pMin->Force.dFz = arAllStepResult[nMinTime];
			pMax->Force.dFz = arAllStepResult[nMaxTime];
			break;
		case ConcNode::kTorsion:
			pMin->Force.dMx = arAllStepResult[nMinTime];
			pMax->Force.dMx = arAllStepResult[nMaxTime];
			break;
		case ConcNode::kMomentY:
			pMin->Force.dMy = arAllStepResult[nMinTime];
			pMax->Force.dMy = arAllStepResult[nMaxTime];
			break;
		case ConcNode::kMomentZ:
			pMin->Force.dMz = arAllStepResult[nMinTime];
			pMax->Force.dMz = arAllStepResult[nMaxTime];
			break;
		default:
			break;
		}

		if (m_pProgress->NextStep() == FALSE) throw FALSE;
	}

	return TRUE;
}

BOOL CSeismicDesignSiet_PierBase::GetNlbmHistoryForce(const ConcNode& target, const int& nMinTime, const int& nMaxTime, OUT T_SIET_EVAL_FOOT_GROUP* pMin, OUT T_SIET_EVAL_FOOT_GROUP* pMax)
{
	int ePosNodeLow = target.ePosNodeLow;
	int nSeqId = m_pDoc->m_pPostCtrl->GetThisInfo()->IsBeamWithHnge(target.kElem);

	const int nPosIdx = [ePosNodeLow]()
		{
			return (ePosNodeLow == T_SIET_EVAL_MEMB_GROUP::kI) ? STRB_POSI_I : STRB_POSI_J;
		}();

	auto nCompIdx = [ePosNodeLow](const ConcNode::eForceType& eType)
		{
			switch (ePosNodeLow)
			{
			case T_SIET_EVAL_MEMB_GROUP::kI:
				if (eType == ConcNode::kAxial) return STRB_AXIS_I;
				else if (eType == ConcNode::kShearY) return STRB_SHEAR_Y_I;
				else if (eType == ConcNode::kShearZ) return STRB_SHEAR_Z_I;
				else if (eType == ConcNode::kTorsion) return STRB_TORSION_I;
				else if (eType == ConcNode::kMomentY) return STRB_MOMENT_Y_I;
				else if (eType == ConcNode::kMomentZ) return STRB_MOMENT_Z_I;
				break;
			case T_SIET_EVAL_MEMB_GROUP::ePosNodeLow::kJ:
				if (eType == ConcNode::kAxial) return STRB_AXIS_J;
				else if (eType == ConcNode::kShearY) return STRB_SHEAR_Y_J;
				else if (eType == ConcNode::kShearZ) return STRB_SHEAR_Z_J;
				else if (eType == ConcNode::kTorsion) return STRB_TORSION_J;
				else if (eType == ConcNode::kMomentY) return STRB_MOMENT_Y_J;
				else if (eType == ConcNode::kMomentZ) return STRB_MOMENT_Z_J;
				break;
			default:
				break;
			}
			ASSERT(FALSE);
			return 0;
		};

	T_STRB_D StrbD;
	if (m_pDoc->m_pPostCtrl->GetNlbmHistory(nMinTime, nSeqId, StrbD) == FALSE) return FALSE;

	pMin->Force.dFx = StrbD.dblForce[nPosIdx][nCompIdx(ConcNode::kAxial)];
	pMin->Force.dFy = StrbD.dblForce[nPosIdx][nCompIdx(ConcNode::kShearY)];
	pMin->Force.dFz = StrbD.dblForce[nPosIdx][nCompIdx(ConcNode::kShearZ)];
	pMin->Force.dMx = StrbD.dblForce[nPosIdx][nCompIdx(ConcNode::kTorsion)];
	pMin->Force.dMy = StrbD.dblForce[nPosIdx][nCompIdx(ConcNode::kMomentY)];
	pMin->Force.dMz = StrbD.dblForce[nPosIdx][nCompIdx(ConcNode::kMomentZ)];

	StrbD.Initialize();
	if (m_pDoc->m_pPostCtrl->GetNlbmHistory(nMaxTime, nSeqId, StrbD) == FALSE) return FALSE;

	pMax->Force.dFx = StrbD.dblForce[nPosIdx][nCompIdx(ConcNode::kAxial)];
	pMax->Force.dFy = StrbD.dblForce[nPosIdx][nCompIdx(ConcNode::kShearY)];
	pMax->Force.dFz = StrbD.dblForce[nPosIdx][nCompIdx(ConcNode::kShearZ)];
	pMax->Force.dMx = StrbD.dblForce[nPosIdx][nCompIdx(ConcNode::kTorsion)];
	pMax->Force.dMy = StrbD.dblForce[nPosIdx][nCompIdx(ConcNode::kMomentY)];
	pMax->Force.dMz = StrbD.dblForce[nPosIdx][nCompIdx(ConcNode::kMomentZ)];

	return TRUE;
}
#pragma endregion <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<