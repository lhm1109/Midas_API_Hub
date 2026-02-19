#include "stdafx.h"
#include "SeismicDesignSisr_H14.h"

#include "..\wg_db\DBLib.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_MatlDB.h"
#include "..\wg_db\wg_db_PostCtrl.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_ThisInfo.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl2.h"
#include "..\wg_db\DB_QUERY_MATL.h"
#include "..\wg_db\DB_QUERY_SECT.h"
#include "..\wg_db\DB_QUERY_SEIS_CVL_JP.h"
#include "..\wg_db\BrdgSeisEvalResult_JP.h"

#include "DgnProgressDlg.h"
#include "BrdgSeisEval_JP.h"

#pragma region >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> CSeismicDesignSisr_H14 
CSeismicDesignSisr_H14::CSeismicDesignSisr_H14() : CSeismicDesignBase()
{
}

CSeismicDesignSisr_H14::~CSeismicDesignSisr_H14()
{
}

void CSeismicDesignSisr_H14::AddProgressStep(CDgnProgressDlg* pProgress)
{
	T_KEY_LIST sisrKeys;
	m_pDoc->m_pAttrCtrl2->GetQSisr()->GetKeyList(sisrKeys);

	std::vector<T_SISR_K> vSisrRoad3;
	std::vector<T_SISR_K> vSisrRoad4;
	std::vector<T_SISR_K> vSisrRoad5;

	const int nSize = sisrKeys.GetSize();
	for (int idx = 0; idx < nSize; idx++)
	{
		T_SISR_K kSisr = sisrKeys[idx];
		T_SISR_D dSisr;
		if (m_pDoc->m_pAttrCtrl2->GetQSisr()->Get(kSisr, dSisr) == FALSE) continue;
		T_SIGR_D dSigr;
		if (m_pDoc->m_pAttrCtrl2->GetQSigr()->Get(dSisr.kSigr, dSigr) == FALSE) continue;
		T_SIPA_D dSipa;
		if (m_pDoc->m_pAttrCtrl2->GetQSipa()->Get(dSisr.kSipa, dSipa) == FALSE) continue;
		if (dSipa.nDesignCode != GetCurrentDgnCode()) continue;

		switch (dSisr.AUTO_D.nDesignCriteria)
		{
		case T_SISR_AUTO_D::kRoad3: // 4. 도시 Ⅲ-H14/24 : 상/하부 공통.
			vSisrRoad3.push_back(kSisr);
			break;
		case T_SISR_AUTO_D::kRoad4: // 2.도시 Ⅳ - H14 / 24 : 하부구조 설계편.
			vSisrRoad4.push_back(kSisr);
			break;
		case T_SISR_AUTO_D::kRoad5: // 1. 도시 Ⅴ-H14/24 : 내진설계편 			
			vSisrRoad5.push_back(kSisr);
			break;
		default:
			if (dSigr.nSubType == T_SIGR_D::kLower) {
				vSisrRoad5.push_back(kSisr);
			}
			else { //kUpper, kUser
				vSisrRoad3.push_back(kSisr);
			}
			break;
		}
	}

	CSeismicDesignSisr_H14_Road3 SisrRoad3D;
	SisrRoad3D.SetProgressDlg(pProgress);
	for (auto kSisr : vSisrRoad3)
	{
		SisrRoad3D.AddProgressStep(kSisr);
	}

	CSeismicDesignSisr_H14_Road4 SisrRoad4;
	SisrRoad4.SetProgressDlg(pProgress);
	for (auto kSisr : vSisrRoad4)
	{
		SisrRoad4.AddProgressStep(kSisr);
	}

	CSeismicDesignSisr_H14_Road5 SisrRoad5;
	SisrRoad5.SetProgressDlg(pProgress);
	for (auto kSisr : vSisrRoad5)
	{
		SisrRoad5.AddProgressStep(kSisr);
	}
}

BOOL CSeismicDesignSisr_H14::Calculate(CDgnProgressDlg* pProgress)
{
	if (m_pDoc == NULL) return FALSE;

	T_KEY_LIST sisrKeys;
	m_pDoc->m_pAttrCtrl2->GetQSisr()->GetKeyList(sisrKeys);

	// Code 단위계 변환
	CUnitConversion unit(D_UNITSYS_FORCE_INDEX_N, D_UNITSYS_LENGTH_INDEX_MM);

	m_pDoc->m_pMatlDB->Create_RebarData(D_UNITSYS_LENGTH_INDEX_MM, D_UNITSYS_FORCE_INDEX_N, TRUE);
		
	std::vector<T_SISR_K> vSisrRoad3;
	std::vector<T_SISR_K> vSisrRoad4;
	std::vector<T_SISR_K> vSisrRoad5;
	
	const int nSize = sisrKeys.GetSize();
	for (int idx = 0; idx < nSize; idx++)
	{
		const T_SISR_K& kSisr = sisrKeys[idx];
		
		T_SISR_D dSisr; T_SIPA_D dSipa; T_SIGR_D dSigr;
		if (m_pDoc->m_pAttrCtrl2->GetQSisr()->Get(kSisr, dSisr) == FALSE) continue;
		if (m_pDoc->m_pAttrCtrl2->GetQSipa()->Get(dSisr.kSipa, dSipa) == FALSE) continue;
		if (dSipa.nDesignCode != GetCurrentDgnCode()) continue;
		if (m_pDoc->m_pAttrCtrl2->GetQSigr()->Get(dSisr.kSigr, dSigr) == FALSE) continue;

		switch (dSisr.AUTO_D.nDesignCriteria)
		{
		case T_SISR_AUTO_D::kRoad3: // 4. 도시 Ⅲ-H14/24 : 상/하부 공통.
			vSisrRoad3.push_back(kSisr);
			break;
		case T_SISR_AUTO_D::kRoad4: // 2.도시 Ⅳ - H14 / 24 : 하부구조 설계편.
			vSisrRoad4.push_back(kSisr);
			break;
		case T_SISR_AUTO_D::kRoad5: // 1. 도시 Ⅴ-H14/24 : 내진설계편 			
			vSisrRoad5.push_back(kSisr);
			break;
		default://직접입력일 경우
			if (dSigr.nSubType == T_SIGR_D::kLower) {
				vSisrRoad5.push_back(kSisr);
			}
			else { //kUpper, kUser
				vSisrRoad3.push_back(kSisr);
			}
			break;
		}
	}	

	CSeismicDesignSisr_H14_Road3 SisrRoad3D;
	SisrRoad3D.SetProgressDlg(pProgress);
	for (auto kSisr : vSisrRoad3)
	{
		SisrRoad3D.Calculate(kSisr);
	}

	CSeismicDesignSisr_H14_Road4 SisrRoad4;
	SisrRoad4.SetProgressDlg(pProgress);
	for (auto kSisr : vSisrRoad4)
	{
		SisrRoad4.Calculate(kSisr);
	}
	
	CSeismicDesignSisr_H14_Road5 SisrRoad5;
	SisrRoad5.SetProgressDlg(pProgress);
	for (auto kSisr : vSisrRoad5)
	{
		SisrRoad5.Calculate(kSisr);
	}

	return FALSE;
}
#pragma endregion <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

#pragma region >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> CSeismicDesignSisr_H14_Road3 
CSeismicDesignSisr_H14_Road3::CSeismicDesignSisr_H14_Road3() : CSeismicDesignSisrBase()
{
	Init();
}

CSeismicDesignSisr_H14_Road3::~CSeismicDesignSisr_H14_Road3()
{
}

void CSeismicDesignSisr_H14_Road3::Init()
{
	m_ElemK = 0;
	m_pElem = NULL;
	m_pMatd = NULL;
	m_pSisr = NULL;
	m_pSipa = NULL;
}

void CSeismicDesignSisr_H14_Road3::AddProgressStep(const T_SISR_K& kSisr)
{
	P_INPUT input(kSisr); if (input.IsValidValue() == FALSE) { ASSERT(FALSE); return; }

	T_ELEM_K_LIST kElemList;
	MakeMemberMap(input.kSigr, kElemList);

	const int nElem = kElemList.GetSize();
	for (int i = 0; i < nElem; ++i)
	{
		const T_ELEM_K kElem = kElemList[i];
		if (m_mMemberMap.find(kElem) == m_mMemberMap.end()) { ASSERT(FALSE); continue; }

		m_pProgress->m_nTotalSteps++;
	}
}

BOOL CSeismicDesignSisr_H14_Road3::Calculate(const T_SISR_K& SisrK)
{
	CBrdgSeisEvalResult_JP* pResult = m_pDoc->m_pPostCtrl->GetBrdgSeisEvalResult_JP();
	if (!pResult) { ASSERT(0); }

	P_INPUT input(SisrK); if (input.IsValidValue() == FALSE) { ASSERT(FALSE); return FALSE; }

	MakeSisrUserMap(SisrK);

	T_ELEM_K_LIST kElemList;
	MakeMemberMap(input.kSigr, kElemList); //m_mMemberMap 채워야 함.

	const int nElem = kElemList.GetSize();
	for (int i = 0; i < nElem; ++i)
	{
		const T_ELEM_K kElem = kElemList[i];
		if (m_mMemberMap.find(kElem) == m_mMemberMap.end()) { ASSERT(FALSE); continue; }
		if (input.tSisr.AUTO_D.nDesignCriteria == T_SISR_AUTO_D::kRoad0) {//유저입력일 경우
			if (m_pDoc->m_pAttrCtrl2->GetQSisrUser()->ExistElem(input.kSigr, kElem) == FALSE) continue;
		}

		if (m_mMemberMap[kElem].nDirShear == seis_jp::kYZ_Dir) {
			RunEvaluation(&input, kElem, seis_jp::kY_Dir);
			RunEvaluation(&input, kElem, seis_jp::kZ_Dir);
		}
		else {
			seis_jp::eDirShear nDirShear = m_mMemberMap[kElem].nDirShear;
			if (input.tSipa.nSeismicForceDirection == T_SIPA_D::kPerp_Dir) {
				nDirShear = (nDirShear == seis_jp::kY_Dir) ? seis_jp::kZ_Dir : seis_jp::kY_Dir;
			}
			
			RunEvaluation(&input, kElem, nDirShear);
		}

		if (m_pProgress->NextStep() == FALSE) throw FALSE;
	}

	return TRUE;
}

void CSeismicDesignSisr_H14_Road3::RunEvaluation(const P_INPUT* pInput, const T_ELEM_K& kElem, const int& nExamShearDir)
{
	Init();
	T_ELEM_D ElemD;	if (!m_pDoc->m_pAttrCtrl->GetElem(kElem, ElemD)) { return; }
	T_MATD_D MatdD;	if (!m_pDoc->m_pAttrCtrl->GetMatlDesign(ElemD.elmat, MatdD)) { return; }
	T_SISR_D SisrD;	if (!m_pDoc->m_pAttrCtrl2->GetQSisr()->Get(pInput->kSisr, SisrD)) { return; }
	T_SIPA_D SipaD;	if (!m_pDoc->m_pAttrCtrl2->GetQSipa()->Get(SisrD.kSipa, SipaD)) { return; }

	m_ElemK = kElem;
	m_pElem = &ElemD;
	m_pMatd = &MatdD;
	m_pSisr = &SisrD;
	m_pSipa = &SipaD;

	T_SISR_EVAL_ROAD3 eval;
	eval.kSipa = pInput->kSipa;
	eval.kSigr = pInput->kSigr;
	eval.kElem = kElem;
	eval.nExamShearDir = nExamShearDir;

	CSisrParamInput inputD;
	inputD.kElem = kElem;
	inputD.bConsiderRein = FALSE;
	inputD.nSeismicForceDirection = pInput->tSipa.nSeismicForceDirection;
	inputD.nExamShearDir = nExamShearDir;

	CSisrParamOuput outputD;
	CSeismicDesignSisrParam sisrParam(&inputD);
	if (!sisrParam.MakeSisrParam(outputD)) {
		return;
	}

	T_SISR_PARAM_POS aParam[2] = { outputD.RegularI, outputD.RegularJ };
	T_SISR_PARAM_BASE aTndnParam[2] = { outputD.TendonI, outputD.TendonJ };

	CalcForce(kElem, pInput->tSipa.aSeisThisK, nExamShearDir, eval);
	CalcRatio(pInput->kSisr, kElem, pInput->tSipa.aSeisThisK, nExamShearDir, aParam, aTndnParam, eval);

	if (SisrD.AUTO_D.nDesignCriteria == T_SISR_AUTO_D::kRoad0) {//유저입력일 경우
		if (m_pDoc->m_pAttrCtrl2->GetQSisrUser()->ExistElem(pInput->kSisr, kElem) == FALSE) return;
	}

	CBrdgSeisEvalResult_JP* pResult = m_pDoc->m_pPostCtrl->GetBrdgSeisEvalResult_JP();
	if (!pResult) { ASSERT(0); return; }
	pResult->SetResultSisrRoad3(eval);
}

void CSeismicDesignSisr_H14_Road3::CalcRatio(const T_SISR_K& SisrK, const T_ELEM_K& kElem, const T_THIS_K_LIST& aThisK, const int nDir,
	const T_SISR_PARAM_POS aSisrParam[2], const T_SISR_PARAM_BASE aTndnParam[2], OUT T_SISR_EVAL_ROAD3& rEval)
{
	CRoad3Param_H14 aParamMax[2];
	CRoad3Param_H14 aParamMin[2];
	CalcRoad3Param(SisrK, kElem, m_pSipa->nSeismicForceDirection, nDir, aThisK, aSisrParam, aParamMax, aParamMin);

	auto L_CalcCapa = [&](int nPos, const CRoad3Param_H14 aRoad3Param[2], OUT T_SISR_EVAL_ROAD3_BASE& rBase)
	{
		const bool bForcePlus = dgn::ME0(rBase.Forc.dShear);
		T_SISR_USER_SHFO_D ShfoD;
		if (GetSisrUserShfo(kElem, nPos, bForcePlus, ShfoD))
		{
			rBase.bAutoCapa = false;

			T_SISR_EVAL_ROAD3_CAPA_DIAG& rDiag = rBase.CapaDiag;
			rDiag.dSus = ShfoD.dSus;
			rDiag.dRatio = dgn::SafeDivAbs(rBase.Forc.dShear, rDiag.dSus);

			T_SISR_EVAL_ROAD3_CAPA_COMP& rComp = rBase.CapaComp;
			rComp.dSuc = ShfoD.dSuc;
			rComp.dRatio = dgn::SafeDivAbs(rBase.Forc.dShear, rComp.dSuc);
		}
		else
		{
			const CRoad3Param_H14& Param = aRoad3Param[nPos];

			T_SISR_PARAM_BASE SisrParam; SisrParam.Init();
			GetSisrParamBase(aSisrParam[nPos], m_pSipa->nSeismicForceDirection, (seis_jp::eDirShear)nDir, nPos, rBase.Forc.dMoment, SisrParam);

			const T_SISR_PARAM_BASE& TndnParam = aTndnParam[nPos];

			const double dMd = Param.dMd; /// Md = 부재 단면에 작용하는 휨 모멘트(종국 휨모멘트) -> 지진파의 평균값 적용.
			const double dM0 = Param.dM0; /// M0 = 프레스트레스 및 축방향력에 의해 콘크리트의 응력도가 부재 인장연에서서 0이 되는 휨모멘트.
			const double dSp = Param.dSp; /// Sp = PC강재의 인장력의 전단력 작용방향의 분력 -> TP 하중조건의 Fz값으로 적용.
			const double dk = min(1.0 + dgn::SafeDiv(dM0, abs(dMd)), 2.0);
			const double dSigmack = m_pMatd->Data1.Design.C_fc; // 콘크리트의 설계기준강도 (N/mm2)
			const double dSigma_sy = min(m_pMatd->SubRebarData.B_fy, 345.0);// σsy : 기설 띠철근(전단보강철근) 항복강도(N/mm2), 상한치 345N/mm2

			const double db = SisrParam.db;
			const double dd = SisrParam.dd;
			const double dTauc = CalcTauc(dSigmack);
			const double dSc = dk * dTauc * db * dd;

			/// Ss = Σ(Aw * σsy * d * (sinθ+cosθ) / 1.15a)
			/// Sp = Ap * σpe * sinα (단, 전단력이 작용하는 방향의 두께가 얇은 부재에서는 Sp=0으로 한다)
			const double dSinCos = CMathFunc::mathSin(SisrParam.dTheta) + CMathFunc::mathCos(SisrParam.dTheta);
			const double dSs = dgn::SafeDiv(SisrParam.dAw * dSigma_sy * dd * dSinCos, 1.15 * SisrParam.da);

			const double dSigCosp = CMathFunc::mathSin(TndnParam.dTheta) + CMathFunc::mathCos(TndnParam.dTheta);
			const double dSpi = dgn::SafeDiv((dSp + TndnParam.dAw * dSigma_sy) * dd * dSigCosp, 1.15 * TndnParam.da);

			const double dSpApp = (nDir == seis_jp::kY_Dir) ? 0.0 : dgn::EQ0(TndnParam.dAw) ? dSp : dSpi - dSp;

			T_SISR_EVAL_ROAD3_CAPA_DIAG& rDiag = rBase.CapaDiag;
			rDiag.dB = db;
			rDiag.dD = dd;
			rDiag.dM0 = dM0;
			rDiag.dMd = dMd;
			rDiag.dK = dk;
			rDiag.dSc = dSc; /// Sc = 콘크리트가 부담하는 전단내력 = k x τc x bw x d
			rDiag.dSs = dSs; /// Ss = 주방향 설계에 있어 전단력에 대해 배치했다고 인정되는 사인장철근이 부딤하는 전단내력의 합계
			rDiag.dSp = dSpApp;
			rDiag.dSus = dSc + dSs + dSpApp; //Sus(전단내력(kN)) = Sc + Ss + Sp
			rDiag.dRatio = dgn::SafeDivAbs(rBase.Forc.dShear, rDiag.dSus);

			const double dTaumax = CalcTaumax(dSigmack);
			const double dSucw = dTaumax * db * dd;

			T_SISR_EVAL_ROAD3_CAPA_COMP& rComp = rBase.CapaComp;
			rComp.dB = db;
			rComp.dD = dd;
			rComp.dTaumax = dTaumax;  // τmax = 콘크리트 평균 전단응력도의 최대치에서 표 4.3.2 의 값을 사용
			rComp.dSucw = dSucw;      // Sucw = τmax * bw * d
			rComp.dSp = dSpApp;       // Sp	= PC강재의 인장력의 전단력 작용방향의 분력.
			rComp.dSuc = dSucw + dSpApp; // Suc = Sucw + Sp (웹 콘크리트의 압축에 대한 내력)
			rComp.dRatio = dgn::SafeDivAbs(rBase.Forc.dShear, rComp.dSuc);
		}
	};

	L_CalcCapa(k_I, aParamMax, rEval.PosI.RsltMax);
	L_CalcCapa(k_I, aParamMin, rEval.PosI.RsltMin);

	L_CalcCapa(k_J, aParamMax, rEval.PosJ.RsltMax);
	L_CalcCapa(k_J, aParamMin, rEval.PosJ.RsltMin);
}

void CSeismicDesignSisr_H14_Road3::CalcRoad3Param(const T_SISR_K& SisrK, T_ELEM_K ElemK, int nSeismicForceDirection, int nDir,
	const T_THIS_K_LIST& aThisK, const T_SISR_PARAM_POS aSisrParam[2], OUT CRoad3Param_H14 aParamMax[2], OUT CRoad3Param_H14 aParamMin[2])
{
	T_SISR_D dSisr;
	if (!m_pDoc->m_pAttrCtrl2->GetQSisr()->Get(SisrK, dSisr)) return;

	GetMd(ElemK, nDir, aThisK, &aParamMax[0], &aParamMax[1], &aParamMin[0], &aParamMin[1]);

	GetSp(SisrK, ElemK, nDir, &aParamMax[0], &aParamMax[1], &aParamMin[0], &aParamMin[1]);

	GetMo(ElemK, nSeismicForceDirection, nDir, aThisK, aSisrParam, &aParamMax[0], &aParamMax[1], &aParamMin[0], &aParamMin[1]);
}

void CSeismicDesignSisr_H14_Road3::GetMd(const T_ELEM_K& ElemK, const int& nExamDir, const T_THIS_K_LIST& aThisK, 
										OUT iRoad3Param* pMax_I, OUT iRoad3Param* pMax_J, OUT iRoad3Param* pMin_I, OUT iRoad3Param* pMin_J)
{
	const int nIdxMom = [nExamDir]()
	{
		switch (nExamDir)
		{
		case seis_jp::kY_Dir: return STRB_MOMENT_Z_I;
		case seis_jp::kZ_Dir: return STRB_MOMENT_Y_I;
		default: ASSERT(0); return 2;
		}
	}();

	auto L_CalcAvg = [](const std::vector<double>& vdM)
	{
		const int nSize = vdM.size();
		if (nSize < 1) return 0.0;

		double dSum = 0.0;
		for (int i = 0; i < nSize; ++i)
		{
			dSum += vdM[i];
		}
		return dgn::SafeDiv(dSum, (double)nSize);
	};

	std::vector<double> vdMmaxI;
	std::vector<double> vdMmaxJ;
	std::vector<double> vdMminI;
	std::vector<double> vdMminJ;

	const int nSize = aThisK.GetSize();
	for (int i = 0; i < nSize; ++i)
	{
		const T_THIS_K kThis = aThisK[i];

		T_LCOM_D LcomD;
		if (!m_pDoc->m_pPostCtrl->MakeLoadComb(D_LOADCASE_HISTORY, kThis, LcomD)) { ASSERT(0); continue; }
		m_pDoc->m_pPostCtrl->SelectLoadCombNew(LcomD);

		T_STRB_D StrbMax, StrbMin;
		if (!m_pDoc->m_pPostCtrl->GetStrbNew(ElemK, &StrbMax, &StrbMin, NULL, TRUE)) { ASSERT(0); continue; }

		vdMmaxI.emplace_back(StrbMax.dblForce[STRB_POSI_I][nIdxMom]);
		vdMmaxJ.emplace_back(StrbMax.dblForce[STRB_POSI_J][nIdxMom + 6]);

		vdMminI.emplace_back(StrbMin.dblForce[STRB_POSI_I][nIdxMom]);
		vdMminJ.emplace_back(StrbMin.dblForce[STRB_POSI_J][nIdxMom + 6]);
	}

	pMax_I->dMd = L_CalcAvg(vdMmaxI);
	pMax_J->dMd = L_CalcAvg(vdMmaxJ);
	pMin_I->dMd = L_CalcAvg(vdMminI);
	pMin_J->dMd = L_CalcAvg(vdMminJ);
}

void CSeismicDesignSisr_H14_Road3::GetSp(const T_SISR_K& SisrK, const T_ELEM_K& ElemK, const int& nExamDir, 
	OUT iRoad3Param* pMax_I, OUT iRoad3Param* pMax_J, OUT iRoad3Param* pMin_I, OUT iRoad3Param* pMin_J)
{
	T_STRB_D StrbTP;
	CString strTP = _LS(IDS_DB_SGLD_TP);
	T_SGLD_K SgldTempK = m_pDoc->m_pAttrCtrl->GetSgldKey(strTP);
	T_LCOM_D LcomD;
	m_pDoc->m_pPostCtrl->MakeLoadComb(D_LOADCASE_STAGE, SgldTempK, LcomD);
	m_pDoc->m_pPostCtrl->SelectLoadCombNew(LcomD, 0, 2);
	m_pDoc->m_pPostCtrl->GetStrbNew(ElemK, &StrbTP, NULL, NULL, FALSE);

	const int nIdxShr = [nExamDir]()
	{
		switch (nExamDir)
		{
		case seis_jp::kY_Dir: return STRB_SHEAR_Y_I;
		case seis_jp::kZ_Dir: return STRB_SHEAR_Z_I;
		default: ASSERT(0); return 2;
		}
	}();

	pMax_I->dSp = fabs(StrbTP.dblForce[STRB_POSI_I][nIdxShr]);
	pMax_J->dSp = fabs(StrbTP.dblForce[STRB_POSI_J][nIdxShr + 6]);
	pMin_I->dSp = fabs(StrbTP.dblForce[STRB_POSI_I][nIdxShr]);
	pMin_J->dSp = fabs(StrbTP.dblForce[STRB_POSI_J][nIdxShr + 6]);

	T_SISR_D dSisr;
	if (!m_pDoc->m_pAttrCtrl2->GetQSisr()->Get(SisrK, dSisr)) return;

	double dSigmaceMaxI = 0.0;
	double dSigmaceMaxJ = 0.0;
	double dSigmaceMinI = 0.0;
	double dSigmaceMinJ = 0.0;
	if (dSisr.AUTO_D.bSigmaCe) ///TODDO sigma_ce 고려를 선택한 경우.
	{
		auto L_GetSigmace = [](const T_STRB_D& StrbD, double dMd, bool bPosI)
		{
			if (dMd < 0.0)
			{
				return bPosI ? dgn::AbsMax(StrbD.dblStress[0][16], StrbD.dblStress[0][18])
					: dgn::AbsMax(StrbD.dblStress[3][17], StrbD.dblStress[3][19]);
			}
			else
			{
				return bPosI ? dgn::AbsMax(StrbD.dblStress[0][20], StrbD.dblStress[0][22])
					: dgn::AbsMax(StrbD.dblStress[3][21], StrbD.dblStress[3][23]);
			}
		};

		pMax_I->dSigmace = L_GetSigmace(StrbTP, pMax_I->dMd, true);
		pMax_J->dSigmace = L_GetSigmace(StrbTP, pMax_J->dMd, false);
		pMin_I->dSigmace = L_GetSigmace(StrbTP, pMin_I->dMd, true);
		pMin_J->dSigmace = L_GetSigmace(StrbTP, pMin_J->dMd, false);
	}
}

void CSeismicDesignSisr_H14_Road3::GetMo(const T_ELEM_K& ElemK, const int& nSeismicForceDir, const int& nShearDir,
										const T_THIS_K_LIST& aThisK, const T_SISR_PARAM_POS aSisrParam[2], 
										OUT iRoad3Param* pMax_I, OUT iRoad3Param* pMax_J, OUT iRoad3Param* pMin_I, OUT iRoad3Param* pMin_J)
{
	T_ELEM_D ElemD;
	if (!m_pDoc->m_pAttrCtrl->GetElem(ElemK, ElemD)) { return; }

	T_SECT_D SectD;
	if (!m_pDoc->m_pPostCtrl->GetSectPost(ElemD.elpro, SectD)) { return; }

	T_SECF_D DataSecf;
	DataSecf.Initialize();
	BOOL bSRC, bTapered;
	int nLyType, nLzType;
	T_SECT_STIFFNESS StiffI, StiffJ;
	m_pDoc->m_pAttrCtrl->GetSectStiffness(ElemD.elpro, SectD, DataSecf, TRUE, bSRC, bTapered, nLyType, nLzType,
		StiffI, StiffJ);

	if ((SectD.nStype == D_SECT_TYPE_TAPERED && DataSecf.bSameIJ == FALSE) == FALSE) {
		StiffJ = StiffI;
	}

	GetN(ElemK, aThisK, pMax_I, pMax_J, pMin_I, pMin_J);

	auto L_CalcM0 = [&](const T_SECT_STIFFNESS& Stiff, const T_SISR_PARAM_POS& paramPos, OUT iRoad3Param* pParam)
	{
		const bool bMyFz = nShearDir == seis_jp::kY_Dir ? false : true;

#if _DEBUG
		if (nSeismicForceDir == T_SIPA_D::kPerp_Dir) {
			int a = 0;
		}
#endif

		//dIc : 부재단면의 도심축에 대한 단면 2차 모멘트
		//dyc : 도심에서 인장측 까지의 거리
		double dIc = bMyFz ? Stiff.Ryy : Stiff.Rzz;
		double dyc = 0.0;
		if (bMyFz == true) {
			dyc = pParam->dMd < 0.0 /*tension at top*/ ? paramPos.zTensTop.dyc : paramPos.zTensBot.dyc;
		}
		else {
			dyc = pParam->dMd < 0.0 /*tension at top*/ ? paramPos.yTensTop.dyc : paramPos.yTensBot.dyc;
		}

		/// 초기하중 존재하는 경우 : 동해석하중 케이스가 사용하는 내부 저장된 초기 단면력 값에는 프레스트레스에 의해 발생한 
		/// Primary force도 포함하고 있으므로 σce = 0 으로 보고 아래 식에서 그냥 N값만 가지고 계산하면 될 것 같습니다. (김경환 이사보님 의견)
		const double dSigTerm = dgn::SafeDiv(pParam->dN, Stiff.Area);
		pParam->dM0 = dgn::SafeDiv(dSigTerm * dIc, dyc);
	};

	L_CalcM0(StiffI, aSisrParam[0], pMax_I);
	L_CalcM0(StiffI, aSisrParam[0], pMin_I);

	L_CalcM0(StiffJ, aSisrParam[1], pMax_J);
	L_CalcM0(StiffJ, aSisrParam[1], pMin_J);
}

void CSeismicDesignSisr_H14_Road3::GetN(const T_ELEM_K& ElemK, const T_THIS_K_LIST& aThisK, 
										OUT iRoad3Param* pMax_I, OUT iRoad3Param* pMax_J, OUT iRoad3Param* pMin_I, OUT iRoad3Param* pMin_J)
{
	double aMaxSum[2] = { 0.0, 0.0 };
	double aMinSum[2] = { 0.0, 0.0 };

	const int nSize = aThisK.GetSize();
	for (int i = 0; i < nSize; ++i)
	{
		const T_THIS_K kThis = aThisK[i];

		T_THIS_D ThisD;
		if (m_pDoc->m_pAttrCtrl->GetThis(kThis, ThisD) == FALSE) continue;

		double aMax[2] = { 0.0, 0.0 };
		double aMin[2] = { 0.0, 0.0 };
		switch (ThisD.nInitLoadMethod)
		{
		case D_THIS_INITMETHOD::D_THIS_INITMETHOD_INITIALLOAD:
			GetN_fromInitialLoad(ElemK, aMax, aMin);
			break;
		case D_THIS_INITMETHOD::D_THIS_INITMETHOD_ORDER_IN_SEQLOAD:
		{
			if (ThisD.nInitCondition == 1) {//초기 단면력 고려 check on
				switch (ThisD.nSubsequentType)
				{
				case 0://Load Case　하중케이스
					GetN_fromLoadCase(ThisD.nSubsequentType, ElemK, ThisD.KeyFinalState, aMax, aMin);
					break;
				case 1://Equilibrium Element Force 초기 단면력(테이블)
					GetN_fromInitElemTable(ElemK, aMax, aMin);
					break;
				case 2://Force for Geometric stiffness 기하 강성용 초기 하중
					ASSERT(FALSE);
					break;
				case 3://Initial Element Forces from Grid Model 격자계산의 초기단면력
					GetN_fromGridModel(ElemK, ThisD.aGilcCase, aMax, aMin);
					break;
				default:
					break;
				}
			}
		}
		break;
		default:
			ASSERT(FALSE);
			break;
		}

		aMaxSum[0] += aMax[0];
		aMinSum[0] += aMin[0];
		aMaxSum[1] += aMax[1];
		aMinSum[1] += aMin[1];
	}

	// -1* 를 하는 이유: Civil 의 축력은 인장력을 표시하며, Mo 를 구하기 위한 식 
	// Mo = (σce + N/Ac) * Ic/y 에서 N은 압축력을 의미하기 때문에 항상 부호를 반대로 해야한다.
	pMax_I->dN = -1 * dgn::SafeDiv(aMaxSum[0], (double)nSize);
	pMin_I->dN = -1 * dgn::SafeDiv(aMinSum[0], (double)nSize);
	pMax_J->dN = -1 * dgn::SafeDiv(aMaxSum[1], (double)nSize);
	pMin_J->dN = -1 * dgn::SafeDiv(aMinSum[1], (double)nSize);
}

void CSeismicDesignSisr_H14_Road3::GetN_fromInitialLoad(const T_ELEM_K& ElemK, OUT double aMax[2], OUT double aMin[2])
{
	T_STRB_D StrbMax, StrbMin;
	StrbMax.Initialize();
	StrbMin.Initialize();

	T_THGC_D ThgcD;
	BOOL bExistThgc = m_pDoc->m_pAttrCtrl2->GetThgc(ThgcD);
	// 2025.12.19 chlim ThgcD.ThisK_DummyInitLoad 의 값이 각 초기하중 케이스의 총합이라 주석처리 함.
// 	for (int i = 0; i < ThgcD.aInitLoad.GetCount(); i++)
// 	{
// 		const T_TGLC_BASE& TglcBase = ThgcD.aInitLoad[i];
// 		//여기를 돌면서 평균값을 구해야 함.
// 		ASSERT_ONCE(FALSE);
// 	}
	//아래는 암시코드
	if (bExistThgc && ThgcD.nInitLoadMethod == 0 && ThgcD.ThisK_DummyInitLoad != 0) {
		const T_THIS_K InitThisK = ThgcD.ThisK_DummyInitLoad;
		m_pDoc->m_pPostCtrl->SelectThis(InitThisK);
		T_STRB_D TimeMax, TimeMin;
		if (m_pDoc->m_pPostCtrl->GetThisInfo()->GetStrbWithTime(ElemK, &StrbMax, &StrbMin, NULL, &TimeMax, &TimeMin, NULL) == FALSE) {
			ASSERT(FALSE);
			return;
		}
	}

	aMax[0] = StrbMax.dblForce[STRB_POSI_I][STRB_AXIS_I];
	aMin[0] = StrbMin.dblForce[STRB_POSI_I][STRB_AXIS_I];
	aMax[1] = StrbMax.dblForce[STRB_POSI_J][STRB_AXIS_J];
	aMin[1] = StrbMin.dblForce[STRB_POSI_J][STRB_AXIS_J];
}

void CSeismicDesignSisr_H14_Road3::GetN_fromLoadCase(const int& nSubsequentType, const T_ELEM_K& ElemK, const T_KEY& kLoadCase, OUT double aMax[2], OUT double aMin[2])
{
	//CThisItemDlg::Dlg2Data() 참고
	T_STRB_D StrbMax; StrbMax.Initialize();
	T_STRB_D StrbMin; StrbMin.Initialize();

	switch (nSubsequentType)
	{
	case 0://ST
	{
		T_LCOM_D LcomD;
		if (!m_pDoc->m_pPostCtrl->MakeLoadComb(D_LOADCASE_STATIC, kLoadCase, LcomD)) { ASSERT(0); return; }
		m_pDoc->m_pPostCtrl->SelectLoadCombNew(LcomD);
		if (!m_pDoc->m_pPostCtrl->GetStrbNew(ElemK, &StrbMax, &StrbMin, NULL, TRUE)) { ASSERT(0); return; }
	}
	break;
	case 1://CS
		ASSERT(FALSE);
		break;
	case 2://TH
	{
		m_pDoc->m_pPostCtrl->SelectThis(kLoadCase);
		T_STRB_D TimeMax, TimeMin;
		if (m_pDoc->m_pPostCtrl->GetThisInfo()->GetStrbWithTime(ElemK, &StrbMax, &StrbMin, NULL, &TimeMax, &TimeMin, NULL) == FALSE) { ASSERT(0); return; }
	}
	break;
	default:
		break;
	}
	aMax[0] = StrbMax.dblForce[STRB_POSI_I][STRB_AXIS_I];
	aMin[0] = StrbMin.dblForce[STRB_POSI_I][STRB_AXIS_I];
	aMax[1] = StrbMax.dblForce[STRB_POSI_J][STRB_AXIS_J];
	aMin[1] = StrbMin.dblForce[STRB_POSI_J][STRB_AXIS_J];
}

void CSeismicDesignSisr_H14_Road3::GetN_fromInitElemTable(const T_ELEM_K& ElemK, OUT double aMax[2], OUT double aMin[2])
{
	POSITION pos = m_pDoc->m_pAttrCtrl->GetStartInmf();
	while (pos != NULL)
	{
		T_INMF_K Key;
		T_INMF_D Data;
		m_pDoc->m_pAttrCtrl->GetNextInmf(pos, Key, Data);

		if (Data.ElemKey != ElemK)
			continue;

		aMax[0] = Data.dElemForce[INMF_POSI_I][INMF_AXIS];
		aMin[0] = Data.dElemForce[INMF_POSI_I][INMF_AXIS];
		aMax[1] = Data.dElemForce[INMF_POSI_J][INMF_AXIS];
		aMin[1] = Data.dElemForce[INMF_POSI_J][INMF_AXIS];
		break;
	}
}

void CSeismicDesignSisr_H14_Road3::GetN_fromGridModel(const T_ELEM_K& ElemK, const CArray<T_GILC_K, T_GILC_K>& gilcKeys, OUT double aMax[2], OUT double aMin[2])
{
	ASSERT(FALSE);
}

double CSeismicDesignSisr_H14_Road3::CalcTauc(double dSigmack)
{
	/// [table-4.3.1] 콘크리트가 부담할 수 있는 평균전단응력, tauc.
	std::vector<std::pair<double, double>> vdTauc;
	vdTauc.emplace_back(std::make_pair(21.0, 0.36));
	vdTauc.emplace_back(std::make_pair(24.0, 0.39));
	vdTauc.emplace_back(std::make_pair(27.0, 0.42));
	vdTauc.emplace_back(std::make_pair(30.0, 0.45));
	vdTauc.emplace_back(std::make_pair(40.0, 0.55));
	vdTauc.emplace_back(std::make_pair(50.0, 0.65));
	vdTauc.emplace_back(std::make_pair(60.0, 0.70));

	return CMathFunc::mathInterpolateLinear(vdTauc, dSigmack);
}

double CSeismicDesignSisr_H14_Road3::CalcTaumax(double dSigmack)
{
	/// [table-4.3.2] 콘크리트의 평균전단응력도의 최대치, tau.max.
	std::vector<std::pair<double, double>> vdTaumax;
	vdTaumax.emplace_back(std::make_pair(21.0, 2.8));
	vdTaumax.emplace_back(std::make_pair(24.0, 3.2));
	vdTaumax.emplace_back(std::make_pair(27.0, 3.6));
	vdTaumax.emplace_back(std::make_pair(30.0, 4.0));
	vdTaumax.emplace_back(std::make_pair(40.0, 5.3));
	vdTaumax.emplace_back(std::make_pair(50.0, 6.0));
	vdTaumax.emplace_back(std::make_pair(60.0, 6.0));

	return CMathFunc::mathInterpolateLinear(vdTaumax, dSigmack);
}
#pragma endregion <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

#pragma region >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> CSeismicDesignSisr_H14_Road4 
CSeismicDesignSisr_H14_Road4::CSeismicDesignSisr_H14_Road4() : CSeismicDesignSisrBase()
{
	Init();
}

CSeismicDesignSisr_H14_Road4::~CSeismicDesignSisr_H14_Road4()
{
}

void CSeismicDesignSisr_H14_Road4::Init()
{
	m_ElemK = 0;
	m_pElem = NULL;
	m_pMatd = NULL;
	m_pSisr = NULL;
	m_pSipa = NULL;
	m_pReinSsm = NULL;
}

void CSeismicDesignSisr_H14_Road4::AddProgressStep(const T_SISR_K& kSisr)
{
	P_INPUT input(kSisr); if (input.IsValidValue() == FALSE) { ASSERT(FALSE); return; }

	T_ELEM_K_LIST kElemList;
	MakeMemberMap(input.kSigr, kElemList);

	const int nElem = kElemList.GetSize();
	for (int i = 0; i < nElem; ++i)
	{
		const T_ELEM_K& kElem = kElemList[i];
		if (m_mMemberMap.find(kElem) == m_mMemberMap.end()) { ASSERT(FALSE); continue; }
		if (m_mMemberMap[kElem].nType != DB_MEMB_TYPE::D_MBTP_COLUMN &&
			m_mMemberMap[kElem].nType != DB_MEMB_TYPE::D_MBTP_BEAM) {
			continue;
		}

		m_pProgress->m_nTotalSteps++;
	}
}

BOOL CSeismicDesignSisr_H14_Road4::Calculate(const T_SISR_K& kSisr)
{
	CBrdgSeisEvalResult_JP* pResult = m_pDoc->m_pPostCtrl->GetBrdgSeisEvalResult_JP();
	if (!pResult) { ASSERT(0); }

	P_INPUT input(kSisr); if (input.IsValidValue() == FALSE) { ASSERT(FALSE); return FALSE; }

	MakeSisrUserMap(kSisr);
	MakeSisrLowerElemParamMap(kSisr);

	T_ELEM_K_LIST kElemList;
	MakeMemberMap(input.kSigr, kElemList); //m_mMemberMap 채워야 함.

	const int nElem = kElemList.GetSize();
	for (int i = 0; i < nElem; ++i)
	{
		const T_ELEM_K& kElem = kElemList[i];
		if (m_mMemberMap.find(kElem) == m_mMemberMap.end()) { ASSERT(FALSE); continue; }
		if (input.tSisr.AUTO_D.nDesignCriteria == T_SISR_AUTO_D::kRoad0) {//유저입력일 경우
			if (m_pDoc->m_pAttrCtrl2->GetQSisrUser()->ExistElem(input.kSisr, kElem) == FALSE) continue;
		}
		
		if (m_mMemberMap[kElem].nType != DB_MEMB_TYPE::D_MBTP_COLUMN &&
			m_mMemberMap[kElem].nType != DB_MEMB_TYPE::D_MBTP_BEAM) {
			continue;
		}

		if (m_mMemberMap[kElem].nDirShear == seis_jp::kYZ_Dir) {
			RunEvaluation(&input, kElem, seis_jp::kY_Dir);
			RunEvaluation(&input, kElem, seis_jp::kZ_Dir);
		}
		else {
			seis_jp::eDirShear nDirShear = m_mMemberMap[kElem].nDirShear;
			if (input.tSipa.nSeismicForceDirection == T_SIPA_D::kPerp_Dir) {
				nDirShear = (nDirShear == seis_jp::kY_Dir) ? seis_jp::kZ_Dir : seis_jp::kY_Dir;
			}

			RunEvaluation(&input, kElem, nDirShear);
		}

		if (m_pProgress->NextStep() == FALSE) throw FALSE;
	}

	SetResultRebarCut(kSisr);

	return TRUE;
}

void CSeismicDesignSisr_H14_Road4::RunEvaluation(const P_INPUT* pInput, const T_ELEM_K& kElem, const int& nExamShearDir)
{
	Init();
	if (m_mMemberMap.find(kElem) == m_mMemberMap.end()) { ASSERT(FALSE); return; }
	T_ELEM_D ElemD;	if (!m_pDoc->m_pAttrCtrl->GetElem(kElem, ElemD)) { ASSERT(FALSE); return; }
	T_MATD_D MatdD;	if (!m_pDoc->m_pAttrCtrl->GetMatlDesign(ElemD.elmat, MatdD)) { return; }
	T_SISR_D SisrD;	if (!m_pDoc->m_pAttrCtrl2->GetQSisr()->Get(pInput->kSisr, SisrD)) { return; }
	T_SIPA_D SipaD;	if (!m_pDoc->m_pAttrCtrl2->GetQSipa()->Get(SisrD.kSipa, SipaD)) { return; }
	T_SECT_SSM_D SectSsmD;
	T_REIN_SSM_D ReinSsmD;
	if (m_pDoc->m_pAttrCtrl2->GetQSectSsm()->Get(kElem, SectSsmD) == TRUE) {
		if (m_pDoc->m_pAttrCtrl2->GetQReinSsm()->Get(SectSsmD.kReinSsm, ReinSsmD) == TRUE)
			m_pReinSsm = &ReinSsmD;
	}
	
	m_ElemK = kElem;
	m_pElem = &ElemD;
	m_pMatd = &MatdD;
	m_pSisr = &SisrD;
	m_pSipa = &SipaD;

	T_SISR_EVAL_LOWER eval;
	eval.kSipa = pInput->kSipa;
	eval.kSigr = pInput->kSigr;
	eval.kElem = kElem;
	eval.nExamShearDir = nExamShearDir;
	eval.nMemberType = m_mMemberMap[eval.kElem].nType;
	eval.nMaterialType = m_mMemberMap[eval.kElem].nMaterialType;

	CSisrParamInput inputD;
	inputD.kElem = kElem;
	inputD.bConsiderRein = (m_pSipa->nExamTarget == T_SIPA_D::kReinforceSection);
	inputD.nSeismicForceDirection = pInput->tSipa.nSeismicForceDirection;
	inputD.nExamShearDir = nExamShearDir;

	CSisrParamOuput outputD;
	CSeismicDesignSisrParam sisrParam(&inputD);
	if (!sisrParam.MakeSisrParam(outputD)) {
		return;
	}

	T_SISR_PARAM_POS aParam[2] = { outputD.RegularI, outputD.RegularJ };

	CalcForce(kElem, pInput->tSipa.aSeisThisK, nExamShearDir, eval);
	CalcRatio(pInput->kSisr, kElem, nExamShearDir, pInput->tSipa.nSeismicForceType, aParam, eval);

	CBrdgSeisEvalResult_JP* pResult = m_pDoc->m_pPostCtrl->GetBrdgSeisEvalResult_JP();
	if (!pResult) { ASSERT(0); return; }
	pResult->SetResultSisrLower(eval);
}

void CSeismicDesignSisr_H14_Road4::CalcRatio(const T_SISR_K& SisrK, const T_ELEM_K& kElem, const int nDir, const int nSeisForceType,
	const T_SISR_PARAM_POS aParam[2], OUT T_SISR_EVAL_LOWER& rEval)
{
	//Ps(전단내력(kN)) = Sc(콘크리트가 부담하는 전단내력) + Ss(철근이 부담하는 전단내력)
	//Sc = Cc x Ce x Cpt x CN x τc x b x d
	//Ss = Aw x σsy x d(sinθ+cosθ) / 1.15s
	//
	//Cc	(하중의 정부교반작용의 영향에 관한 보정계수) - 대화창 옵션 사용
	//			타입 I  지진동 - 0.6
	//			타입 II 지진동 - 0.8
	//			단, 교각 장출보, 교대 및 기초에 대해서는 Cc=1.0 으로 한다. - 대화창 옵션 사용
	//Ce	(교각본체단면의 유효높이d 에 대한 보정계수)
	//Cpt	(축방향 인장철근비 Pt에 관한 보정계수)
	//CN	(축방향 압축력에 의한 보정계수, CN = 1+Mo/M 에 의해 산출한다. )
	//
	//Mo	축방향 압축력에 의해 콘크리트의 응력도가 부재 인장연에서서 0이 되는 휨 모멘트
	//		Mo = N/Ac x Ic/y
	//M		부재 단면에 작용하는 휨 모멘트
	//N		부재 단면에 작용하는 축방향 압축력
	//Ic	부재 단면의 도심축에 관한 단면2차 모멘트
	//Ac	부재 단면적
	//y		부재 단면의 도심으로부터 부재 인장연까지의 거리
	//
	//τc	(콘크리트가 부담할 수 있는 평균전단응력)
	//b		(교각 본체 단면의 폭)
	//d		(교각 본체 단면의 유효높이)
	//
	//Aw	(간격 a 및 각도 θ에서 배근된 띠철근의 단면적)
	//σsy	(띠철근의 항복응력, 345(N/mm2)를 상한으로 한다.)
	//θ	(띠철근과 연직축이 이루는 각 (deg))
	//s		(띠철근의 간격)

	const double dCc = IsCcUse10(kElem) ? 1.0 : GetCc(nSeisForceType);
	const BOOL bUseCarbonFiber = (m_pReinSsm != NULL && m_pReinSsm->ReinSectCF.bUse == TRUE);

	auto L_CalcCapa = [&](int nPos, OUT T_SISR_EVAL_LOWER_BASE& rBase)
	{
		const bool bForcePlus = dgn::ME0(rBase.dShear);
		T_SISR_USER_SHFO_D ShfoD;
		if (GetSisrUserShfo(kElem, nPos, bForcePlus, ShfoD))
		{
			rBase.bAutoCapa = false;
			rBase.dPs = ShfoD.dSus;
			rBase.dRatio = dgn::SafeDivAbs(rBase.dShear, rBase.dPs);
		}
		else
		{
			T_SISR_PARAM_BASE SisrParam; SisrParam.Init();
			GetSisrParamBase(aParam[nPos], m_pSipa->nSeismicForceDirection, (seis_jp::eDirShear)nDir, nPos, rBase.dMoment, SisrParam);

			const double db = SisrParam.db;
			const double dd = SisrParam.dd;
			const double dpt = SisrParam.GetPt();
			const double da = SisrParam.da;
			const double dAw = SisrParam.dAw;
			const double dTheta = SisrParam.dTheta;
			const double dSigmack = GetSigmaCk(kElem, SisrParam, nPos); // 콘크리트의 설계기준강도 (N/mm2)
			const double dSigma_sy = min(m_pMatd->SubRebarData.B_fy, 345.0);// σsy : 기설 띠철근(전단보강철근) 항복강도(N/mm2), 상한치 345N/mm2

			const double dCe = CalcCe(dd);
			const double dCpt = CalcCpt(dpt);
			const double dCN = CalcCN();
			const double dTauc = CalcTauc(dSigmack);
			const double dSinCos = CMathFunc::mathSin(dTheta) + CMathFunc::mathCos(dTheta);

			//※ d/1.15보다 교각 높이 hp가 낮은 경우는 d/1.15 대신 교각 높이 hp를 적용한다.
			double dHp = GetColmHeight(kElem);
			double dd115 = dgn::MT0(dHp) ? min(dd / 1.15, dHp) : dd / 1.15;

			double dSc = dCc * dCe * dCpt * dCN * dTauc * db * dd;
			double dSs = dgn::SafeDiv(SisrParam.dAw * dSigma_sy * dd115 * dSinCos, da);

			double dCdc = 1.0;
			double dCds = 1.0;
			double dShearSpan = GetShearSpan(kElem);
			if (dgn::UQ0(dShearSpan))
			{
				const double dadprime = dgn::SafeDiv(dShearSpan, dd);
				dCdc = CalcCdc(dadprime);
				dCds = CalcCds(dadprime);
			}

			double dSf = (bUseCarbonFiber == TRUE) ? CalcSf(nPos, m_pSipa->nSeismicForceDirection, (seis_jp::eDirShear)nDir, kElem, dd) : 0.0;

			rBase.dB = db;
			rBase.dD = dd;
			rBase.dCe = dCe;
			rBase.dCpt = dCpt;
			rBase.dPt = dpt * 100.0; /// save as percent
			rBase.dSc = dCdc * dSc;
			rBase.dSs = dCds * dSs;
			rBase.dCdc = dCdc;
			rBase.dCds = dCds;
			rBase.dPs = rBase.dSc + rBase.dSs;
			if (bUseCarbonFiber == TRUE) rBase.dPs += (dCds * dSf);
			rBase.dRatio = dgn::SafeDivAbs(rBase.dShear, rBase.dPs);
		}
	};

	L_CalcCapa(k_I, rEval.PosI.RsltMax);
	L_CalcCapa(k_I, rEval.PosI.RsltMin);
	rEval.PosI.RsltMin.dPs = -1 * rEval.PosI.RsltMin.dPs;

	L_CalcCapa(k_J, rEval.PosJ.RsltMax);
	L_CalcCapa(k_J, rEval.PosJ.RsltMin);
	rEval.PosJ.RsltMin.dPs = -1 * rEval.PosJ.RsltMin.dPs;
}

double CSeismicDesignSisr_H14_Road4::CalcCe(double dd)
{
	/// [table-4.2.2] 교각 본체 단면의 유효높이 d에 대한 보정계수, Ce.
	std::vector<std::pair<double, double>> vdCe;
	vdCe.emplace_back(std::make_pair(300.0, 1.4));
	vdCe.emplace_back(std::make_pair(1000.0, 1.0));
	vdCe.emplace_back(std::make_pair(3000.0, 0.7));
	vdCe.emplace_back(std::make_pair(5000.0, 0.6));
	vdCe.emplace_back(std::make_pair(10000.0, 0.5));

	return CMathFunc::mathInterpolateLinear(vdCe, dd);
}

double CSeismicDesignSisr_H14_Road4::CalcCpt(double dpt)
{
	const double dptPercent = dpt * 100.0;

	/// [table-4.2.3] 축방향 인장 철근비 pt에 대한 보정계수, Cpt.
	std::vector<std::pair<double, double>> vdCpt;
	vdCpt.emplace_back(std::make_pair(0.0, 0.5)); //Pt가 0.1보다 적은경우, 0.1-0.2간의 기울기로 선형보간함
	vdCpt.emplace_back(std::make_pair(0.1, 0.7));
	vdCpt.emplace_back(std::make_pair(0.2, 0.9));
	vdCpt.emplace_back(std::make_pair(0.3, 1.0));
	vdCpt.emplace_back(std::make_pair(0.5, 1.2));
	vdCpt.emplace_back(std::make_pair(1.0, 1.5));
	return CMathFunc::mathInterpolateLinear(vdCpt, dptPercent);
}

double CSeismicDesignSisr_H14_Road4::CalcCN()
{
	/// EQ(4.2.1) CN = 1+M0/M
	return 1.0; /// 교각, 교대 및 footing에 대해서는 CN=1.0으로 한다.
}

double CSeismicDesignSisr_H14_Road4::CalcTauc(double dSigmack)
{
	/// [table-5.2.1] 콘크리트가 부담할 수 있는 평균전단응력, tauc.
	std::vector<std::pair<double, double>> vdTauc;
	vdTauc.emplace_back(std::make_pair(21.0, 0.33));
	vdTauc.emplace_back(std::make_pair(24.0, 0.35));
	vdTauc.emplace_back(std::make_pair(27.0, 0.36));
	vdTauc.emplace_back(std::make_pair(30.0, 0.37));

	return CMathFunc::mathInterpolateLinear(vdTauc, dSigmack);
}
#pragma endregion <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

#pragma region >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> CSeismicDesignSisr_H14_Road5 
CSeismicDesignSisr_H14_Road5::CSeismicDesignSisr_H14_Road5() : CSeismicDesignSisrBase()
{
	Init();
}

CSeismicDesignSisr_H14_Road5::~CSeismicDesignSisr_H14_Road5()
{
}

void CSeismicDesignSisr_H14_Road5::Init()
{
	m_ElemK = 0;
	m_pElem = NULL;
	m_pMatd = NULL;
	m_pSisr = NULL;
	m_pSipa = NULL;
	m_pReinSsm = NULL;
}

void CSeismicDesignSisr_H14_Road5::AddProgressStep(const T_SISR_K& kSisr)
{
	P_INPUT input(kSisr); if (input.IsValidValue() == FALSE) { ASSERT(FALSE); return; }

	T_ELEM_K_LIST kElemList;
	MakeMemberMap(input.kSigr, kElemList);

	const int nElem = kElemList.GetSize();
	for (int i = 0; i < nElem; ++i)
	{
		const T_ELEM_K& kElem = kElemList[i];
		if (m_mMemberMap.find(kElem) == m_mMemberMap.end()) { ASSERT(FALSE); continue; }
		if (m_mMemberMap[kElem].nType != DB_MEMB_TYPE::D_MBTP_COLUMN &&
			m_mMemberMap[kElem].nType != DB_MEMB_TYPE::D_MBTP_BEAM) {
			continue;
		}

		m_pProgress->m_nTotalSteps++;
	}
}

BOOL CSeismicDesignSisr_H14_Road5::Calculate(const T_SISR_K& kSisr)
{
	CBrdgSeisEvalResult_JP* pResult = m_pDoc->m_pPostCtrl->GetBrdgSeisEvalResult_JP();
	if (!pResult) { ASSERT(0); return FALSE; }

	P_INPUT input(kSisr); if (input.IsValidValue() == FALSE) { ASSERT(FALSE); return FALSE; }

	MakeSisrUserMap(kSisr);
	MakeSisrLowerElemParamMap(kSisr);

	T_ELEM_K_LIST kElemList;
	MakeMemberMap(input.kSigr, kElemList); //m_mMemberMap 채워야 함.

	const int nElem = kElemList.GetSize();
	for (int i = 0; i < nElem; ++i)
	{
		const T_ELEM_K& kElem = kElemList[i];
		if (m_mMemberMap.find(kElem) == m_mMemberMap.end()) { ASSERT(FALSE); continue; }
		if (input.tSisr.AUTO_D.nDesignCriteria == T_SISR_AUTO_D::kRoad0) {//유저입력일 경우
			if (m_pDoc->m_pAttrCtrl2->GetQSisrUser()->ExistElem(input.kSisr, kElem) == FALSE)
				continue;
		}

		if (m_mMemberMap[kElem].nType != DB_MEMB_TYPE::D_MBTP_COLUMN &&
			m_mMemberMap[kElem].nType != DB_MEMB_TYPE::D_MBTP_BEAM) {
			continue;
		}

		if (m_mMemberMap[kElem].nDirShear == seis_jp::kYZ_Dir) {
			RunEvaluation(&input, kElem, seis_jp::kY_Dir);
			RunEvaluation(&input, kElem, seis_jp::kZ_Dir);
		}
		else {
			seis_jp::eDirShear nDirShear = m_mMemberMap[kElem].nDirShear;
			if (input.tSipa.nSeismicForceDirection == T_SIPA_D::kPerp_Dir) {
				nDirShear = (nDirShear == seis_jp::kY_Dir) ? seis_jp::kZ_Dir : seis_jp::kY_Dir;
			}

			RunEvaluation(&input, kElem, nDirShear);
		}

		if (m_pProgress->NextStep() == FALSE) throw FALSE;
	}

	SetResultRebarCut(kSisr);

	return TRUE;
}

void CSeismicDesignSisr_H14_Road5::RunEvaluation(const P_INPUT* pInput, const T_ELEM_K& kElem, const int& nExamShearDir)
{
	Init();
	if (m_mMemberMap.find(kElem) == m_mMemberMap.end()) { ASSERT(FALSE); return; }
	T_ELEM_D ElemD;	if (!m_pDoc->m_pAttrCtrl->GetElem(kElem, ElemD)) { ASSERT(FALSE); return; }
	T_MATD_D MatdD;	if (!m_pDoc->m_pAttrCtrl->GetMatlDesign(ElemD.elmat, MatdD)) { return; }
	T_SISR_D SisrD;	if (!m_pDoc->m_pAttrCtrl2->GetQSisr()->Get(pInput->kSisr, SisrD)) { return; }
	T_SIPA_D SipaD;	if (!m_pDoc->m_pAttrCtrl2->GetQSipa()->Get(SisrD.kSipa, SipaD)) { return; }
	T_SECT_SSM_D SectSsmD;
	T_REIN_SSM_D ReinSsmD;
	if (m_pDoc->m_pAttrCtrl2->GetQSectSsm()->Get(kElem, SectSsmD) == TRUE) {
		if (m_pDoc->m_pAttrCtrl2->GetQReinSsm()->Get(SectSsmD.kReinSsm, ReinSsmD) == TRUE)
			m_pReinSsm = &ReinSsmD;
	}
	
	m_ElemK = kElem;
	m_pElem = &ElemD;
	m_pMatd = &MatdD;
	m_pSisr = &SisrD;
	m_pSipa = &SipaD;

	T_SISR_EVAL_LOWER eval;
	eval.kSipa = pInput->kSipa;
	eval.kSigr = pInput->kSigr;
	eval.kElem = kElem;
	eval.nExamShearDir = nExamShearDir;
	eval.nMemberType = m_mMemberMap[eval.kElem].nType;
	eval.nMaterialType = m_mMemberMap[eval.kElem].nMaterialType;

	CSisrParamInput inputD;
	inputD.kElem = kElem;
	inputD.bConsiderRein = (m_pSipa->nExamTarget == T_SIPA_D::kReinforceSection);
	inputD.nSeismicForceDirection = pInput->tSipa.nSeismicForceDirection;
	inputD.nExamShearDir = nExamShearDir;

	CSisrParamOuput outputD;
	CSeismicDesignSisrParam sisrParam(&inputD);
	if (!sisrParam.MakeSisrParam(outputD)) {
		return;
	}

	T_SISR_PARAM_POS aParam[2] = { outputD.RegularI, outputD.RegularJ };

	CalcForce(kElem, pInput->tSipa.aSeisThisK, nExamShearDir, eval);
	CalcRatio(pInput->kSisr, kElem, nExamShearDir, pInput->tSipa.nSeismicForceType, aParam, eval);

	CBrdgSeisEvalResult_JP* pResult = m_pDoc->m_pPostCtrl->GetBrdgSeisEvalResult_JP();
	if (!pResult) { ASSERT(0); return; }
	pResult->SetResultSisrLower(eval);
}

void CSeismicDesignSisr_H14_Road5::CalcRatio(const T_SISR_K& SisrK, const T_ELEM_K& kElem, const int nDir, const int nSeisForceType,
	const T_SISR_PARAM_POS aParam[2], OUT T_SISR_EVAL_LOWER& rEval)
{
	//Ps(전단내력(kN)) = Sc(콘크리트가 부담하는 전단내력) + Ss(철근이 부담하는 전단내력)
	//Sc = Cc x Ce x Cpt x τc x b x d
	//Ss = Aw x σsy x d(sinθ+cosθ) / 1.15a
	//
	//Cc	(하중의 정부교반작용의 영향에 관한 보정계수) - 대화창 옵션 사용
	//			타입 I  지진동 - 0.6
	//			타입 II 지진동 - 0.8
	//			단, 교각 장출보, 교대 및 기초에 대해서는 Cc=1.0 으로 한다. - 대화창 옵션 사용
	//Ce	(교각본체단면의 유효높이d 에 대한 보정계수)
	//Cpt	(축방향 인장철근비 Pt에 관한 보정계수)
	//τc	(콘크리트가 부담할 수 있는 평균전단응력)
	//b		(교각 본체 단면의 폭)
	//d		(교각 본체 단면의 유효높이)
	//Aw	(간격 a 및 각도 θ에서 배근된 띠철근의 단면적)
	//σsy	(띠철근의 항복응력, 345(N/mm2)를 상한으로 한다.)
	//θ	(띠철근과 연직축이 이루는 각 (deg))
	//a		(띠철근의 간격)

	const double dCc = IsCcUse10(kElem) ? 1.0 : GetCc(nSeisForceType);
	const BOOL bConsiderRein = (m_pSipa->nExamTarget == T_SIPA_D::kReinforceSection);
	const BOOL bUseCarbonFiber = bConsiderRein ? (m_pReinSsm != NULL && m_pReinSsm->ReinSectCF.bUse == TRUE) : FALSE;

	auto L_CalcCapa = [&](int nPos, OUT T_SISR_EVAL_LOWER_BASE& rBase)
	{
		const bool bForcePlus = dgn::ME0(rBase.dShear);
		T_SISR_USER_SHFO_D ShfoD;
		if (GetSisrUserShfo(kElem, nPos, bForcePlus, ShfoD))
		{
			rBase.bAutoCapa = false;
			rBase.dPs = ShfoD.dSus;
			rBase.dRatio = dgn::SafeDivAbs(rBase.dShear, rBase.dPs);
		}
		else
		{
			T_SISR_PARAM_BASE SisrParam; SisrParam.Init();
			GetSisrParamBase(aParam[nPos], m_pSipa->nSeismicForceDirection, (seis_jp::eDirShear)nDir, nPos, rBase.dMoment, SisrParam);

			const double db = SisrParam.db;
			const double dd = SisrParam.dd;
			const double dpt = SisrParam.GetPt();
			const double da = SisrParam.da;
			const double dAw = SisrParam.dAw;
			const double dTheta = SisrParam.dTheta;
			const double dSigmack = bConsiderRein ? GetSigmaCk(kElem, SisrParam, nPos) : m_pMatd->Data1.Design.C_fc; // 콘크리트의 설계기준강도 (N/mm2)
			double dSigma_sy = bConsiderRein ? GetSigmaSy(kElem) : min(m_pMatd->SubRebarData.B_fy, 345.0);// σsy : 기설 띠철근(전단보강철근) 항복강도(N/mm2), 상한치 345N/mm2

			double dCe = CalcCe(dd);
			double dCpt = CalcCpt(dpt);
			double dTauc = CalcTauc(dSigmack);
			double dSinCos = CMathFunc::mathSin(dTheta) + CMathFunc::mathCos(dTheta);

			//※ d/1.15보다 교각 높이 hp가 낮은 경우는 d/1.15 대신 교각 높이 hp를 적용한다.
			double dHp = GetColmHeight(kElem);
			double dd115 = dgn::MT0(dHp) ? min(dd / 1.15, dHp) : dd / 1.15;

			double dSc = dCc * dCe * dCpt * dTauc * db * dd;
			double dSs = dgn::SafeDiv(SisrParam.dAw * dSigma_sy * dd115 * dSinCos, da);

			double dCdc = 1.0, dCds = 1.0;
			double dShearSpan = GetShearSpan(kElem);
			if (dgn::UQ0(dShearSpan))
			{
				const double dadprime = dgn::SafeDiv(dShearSpan, dd);
				dCdc = CalcCdc(dadprime);
				dCds = CalcCds(dadprime);
			}

			double dSf = (bUseCarbonFiber == TRUE) ? CalcSf(nPos, m_pSipa->nSeismicForceDirection, (seis_jp::eDirShear)nDir, kElem, dd) : 0.0;

			rBase.dB = db;
			rBase.dD = dd;
			rBase.dCe = dCe;
			rBase.dCpt = dCpt;
			rBase.dPt = dpt * 100.0; /// save as percent
			rBase.dSc = dCdc * dSc;
			rBase.dSs = dCds * dSs;
			rBase.dCdc = dCdc;
			rBase.dCds = dCds;
			rBase.dPs = rBase.dSc + rBase.dSs;
			if (bUseCarbonFiber == TRUE) rBase.dPs += (dCds * dSf);
			rBase.dRatio = dgn::SafeDivAbs(rBase.dShear, rBase.dPs);
		}
	};

	L_CalcCapa(k_I, rEval.PosI.RsltMax);
	L_CalcCapa(k_I, rEval.PosI.RsltMin);
	rEval.PosI.RsltMin.dPs = -1 * rEval.PosI.RsltMin.dPs;

	L_CalcCapa(k_J, rEval.PosJ.RsltMax);
	L_CalcCapa(k_J, rEval.PosJ.RsltMin);
	rEval.PosJ.RsltMin.dPs = -1 * rEval.PosJ.RsltMin.dPs;
}

double CSeismicDesignSisr_H14_Road5::CalcCe(double dd)
{
	/// [table-10.5.2] 교각 본체 단면의 유효높이 d에 대한 보정계수, Ce.
	std::vector<std::pair<double, double>> vdCe;
	vdCe.emplace_back(std::make_pair(1000.0, 1.0));
	vdCe.emplace_back(std::make_pair(3000.0, 0.7));
	vdCe.emplace_back(std::make_pair(5000.0, 0.6));
	vdCe.emplace_back(std::make_pair(10000.0, 0.5));

	return CMathFunc::mathInterpolateLinear(vdCe, dd);
}

double CSeismicDesignSisr_H14_Road5::CalcCpt(double dpt)
{
	const double dptPercent = dpt * 100.0;

	if (dptPercent < 0.1) {
		double dVal = 0.7 - 2 * (0.1 - dptPercent);
		return dVal;
	}

	/// [table-10.5.3] 축방향 인장 철근비 pt에 대한 보정계수, Cpt.
	std::vector<std::pair<double, double>> vdCpt;
	vdCpt.emplace_back(std::make_pair(0.1, 0.7));
	vdCpt.emplace_back(std::make_pair(0.2, 0.9));
	vdCpt.emplace_back(std::make_pair(0.3, 1.0));
	vdCpt.emplace_back(std::make_pair(0.5, 1.2));
	vdCpt.emplace_back(std::make_pair(1.0, 1.5));

	return CMathFunc::mathInterpolateLinear(vdCpt, dptPercent);
}

#define NormalZero 1.e-15

double mathInverseInterpolateLinear(double dX1, double dX2, double dY1, double dY2, double dY)
{
	if (fabs(dY - dY1) < NormalZero)
		return dX1;
	if (fabs(dY - dY2) < NormalZero)
		return dX2;
	if (fabs(dY2 - dY1) < NormalZero)
		return (dX1 + dX2) / 2.0; // 수평선 보간

	return dX1 + (dX2 - dX1) / (dY2 - dY1) * (dY - dY1);
}

double mathInverseInterpolateLinear(const std::vector<std::pair<double, double>>& vdXY, double dY)
{
	const size_t nSize = vdXY.size();
	if (nSize == 0)
		return 0.0;

	if (nSize == 1)
		return vdXY[0].first;

	for (int i = 0; i < nSize - 1; ++i)
	{
		const double dX1 = vdXY[i].first;
		const double dY1 = vdXY[i].second;

		const double dX2 = vdXY[i + 1].first;
		const double dY2 = vdXY[i + 1].second;

		// dY가 두 점 사이에 있을 경우만 처리
		if ((dY > dY1 || fabs(dY - dY1) < NormalZero) &&
			(dY < dY2 || fabs(dY - dY2) < NormalZero) ||
			(dY < dY1 || fabs(dY - dY1) < NormalZero) &&
			(dY > dY2 || fabs(dY - dY2) < NormalZero))
		{
			return mathInverseInterpolateLinear(dX1, dX2, dY1, dY2, dY);
		}
	}

	// dY가 범위 밖이면 양 끝값 X 리턴
	if (dY < vdXY.front().second)
		return vdXY.front().first;
	if (dY > vdXY.back().second)
		return vdXY.back().first;

	return 0.0;
}

double CSeismicDesignSisr_H14_Road5::CalcTauc(double dSigmack)
{
	/// [table-10.5.1] 콘크리트가 부담할 수 있는 평균전단응력, tauc.
	std::vector<std::pair<double, double>> vdTauc;
	vdTauc.emplace_back(std::make_pair(21.0, 0.33));
	vdTauc.emplace_back(std::make_pair(24.0, 0.35));
	vdTauc.emplace_back(std::make_pair(27.0, 0.36));
	vdTauc.emplace_back(std::make_pair(30.0, 0.37));
	vdTauc.emplace_back(std::make_pair(40.0, 0.41));

	double dTauc = CMathFunc::mathInterpolateLinear(vdTauc, dSigmack);
#ifdef _DEBUG
	double dCk = mathInverseInterpolateLinear(vdTauc, dTauc);
	ASSERT_ONCE(dgn::EQ(dSigmack, dCk) == true);
#endif // _DEBUG
	return dTauc;
}
#pragma endregion <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<