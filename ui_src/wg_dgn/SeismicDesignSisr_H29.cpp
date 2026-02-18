#include "stdafx.h"
#include "SeismicDesignSisr_H29.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_MatlDB.h"
#include "..\wg_db\wg_db_PostCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl2.h"
#include "..\wg_db\DB_QUERY_SEIS_CVL_JP.h"
#include "..\wg_db\BrdgSeisEvalResult_JP.h"

#include "DgnProgressDlg.h"
#include "BrdgSeisEval_JP.h"

#pragma region >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> CSeismicDesignSisr_H29 
CSeismicDesignSisr_H29::CSeismicDesignSisr_H29() : CSeismicDesignBase()
{
}

CSeismicDesignSisr_H29::~CSeismicDesignSisr_H29()
{
}

void CSeismicDesignSisr_H29::AddProgressStep(CDgnProgressDlg* pProgress)
{
	if (m_pDoc == NULL) return;

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
		case T_SISR_AUTO_D::kRoad3: // 4. 도시 Ⅲ-H14/24 : 상부 설계편
			vSisrRoad3.push_back(kSisr);
			break;
		case T_SISR_AUTO_D::kRoad4: // 2.도시 Ⅳ - H14 / 24 : 하부구조 설계편
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

	CSeismicDesignSisr_H29_Road3 SisrRoad3;
	SisrRoad3.SetProgressDlg(pProgress);
	for (auto kSisr : vSisrRoad3)
	{
		SisrRoad3.AddProgressStep(kSisr);
	}

	CSeismicDesignSisr_H29_Road4 SisrRoad4;
	SisrRoad4.SetProgressDlg(pProgress);
	for (auto kSisr : vSisrRoad4)
	{
		SisrRoad4.AddProgressStep(kSisr);
	}

	CSeismicDesignSisr_H29_Road5 SisrRoad5;
	SisrRoad5.SetProgressDlg(pProgress);
	for (auto kSisr : vSisrRoad5)
	{
		SisrRoad5.AddProgressStep(kSisr);
	}
}

BOOL CSeismicDesignSisr_H29::Calculate(CDgnProgressDlg* pProgress)
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
		case T_SISR_AUTO_D::kRoad3: // 4. 도시 Ⅲ-H14/24 : 상부 설계편
			vSisrRoad3.push_back(kSisr);
			break;
		case T_SISR_AUTO_D::kRoad4: // 2.도시 Ⅳ - H14 / 24 : 하부구조 설계편
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

	CSeismicDesignSisr_H29_Road3 SisrRoad3;
	SisrRoad3.SetProgressDlg(pProgress);
	for (auto kSisr : vSisrRoad3)
	{
		SisrRoad3.Calculate(kSisr);
	}

	CSeismicDesignSisr_H29_Road4 SisrRoad4;
	SisrRoad4.SetProgressDlg(pProgress);
	for (auto kSisr : vSisrRoad4)
	{
		SisrRoad4.Calculate(kSisr);
	}

	CSeismicDesignSisr_H29_Road5 SisrRoad5;
	SisrRoad5.SetProgressDlg(pProgress);
	for (auto kSisr : vSisrRoad5)
	{
		SisrRoad5.Calculate(kSisr);
	}

	return TRUE;
}

#pragma endregion <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

#pragma region >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> CSeismicDesignSisr_H29_Road3 
CSeismicDesignSisr_H29_Road3::CSeismicDesignSisr_H29_Road3() : CSeismicDesignSisr_H14_Road3()
{
}

CSeismicDesignSisr_H29_Road3::~CSeismicDesignSisr_H29_Road3()
{
}

void CSeismicDesignSisr_H29_Road3::AddProgressStep(const T_SISR_K& kSisr)
{
	CSeismicDesignSisr_H14_Road3::AddProgressStep(kSisr);
}

BOOL CSeismicDesignSisr_H29_Road3::Calculate(const T_SISR_K& SisrK)
{
	return CSeismicDesignSisr_H14_Road3::Calculate(SisrK);
}

void CSeismicDesignSisr_H29_Road3::CalcRatio(const T_SISR_K& SisrK, const T_ELEM_K& kElem, const T_THIS_K_LIST& aThisK, const int nDir,
	const T_SISR_PARAM_POS aSisrParam[2], const T_SISR_PARAM_BASE aTndnParam[2], OUT T_SISR_EVAL_ROAD3& rEval)
{
#pragma region // Data Get
	T_ELEM_D ElemD;
	if (!m_pDoc->m_pAttrCtrl->GetElem(kElem, ElemD)) { return; }

	T_SECT_D SectD;
	if (!m_pDoc->m_pPostCtrl->GetSectPost(ElemD.elpro, SectD)) { return; }

	T_MATD_D MatdD;
	if (!m_pDoc->m_pAttrCtrl->GetMatlDesign(ElemD.elmat, MatdD)) { return; }

	T_SISR_D SisrD;
	if (!m_pDoc->m_pAttrCtrl2->GetQSisr()->Get(SisrK, SisrD)) { return; }

	T_SIPA_D SipaD;
	if (!m_pDoc->m_pAttrCtrl2->GetQSipa()->Get(SisrD.kSipa, SipaD)) { return; }

	T_SIFA_H29_D facH29;
	if (m_pDoc->m_pAttrCtrl2->GetQSifaH29()->Get(facH29) == FALSE) return;
#pragma endregion

	CRoad3Param_H29 aParamMax[2];
	CRoad3Param_H29 aParamMin[2];
	CalcRoad3Param(SisrK, kElem, SipaD.nSeismicForceDirection, nDir, aThisK, aSisrParam, aParamMax, aParamMin);

	auto L_CalcCapa = [&](int nPos, const CRoad3Param_H29 aRoad3Param[2], OUT T_SISR_EVAL_ROAD3_BASE& rBase)
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
			const CRoad3Param_H29& Param = aRoad3Param[nPos];
			const T_SISR_PARAM_BASE& TndnParam = aTndnParam[nPos];

			T_SISR_PARAM_BASE SisrParam; SisrParam.Init();
			GetSisrParamBase(aSisrParam[nPos], SipaD.nSeismicForceDirection, (seis_jp::eDirShear)nDir, nPos, rBase.Forc.dMoment, SisrParam);

			const double dMd = Param.dMd; // Md = 부재 단면에 작용하는 휨 모멘트(종국 휨모멘트) -> 지진파의 평균값 적용.
			const double dM0 = Param.dM0; // M0 = 프레스트레스 및 축방향력에 의해 콘크리트의 응력도가 부재 인장연에서서 0이 되는 휨모멘트.
			const double dSd = Param.dSd; // Sd = 부재 단면에 작용하는 전단력 (N) -> 지진파의 평균값 적용.
			const double dSp$ = Param.dSp; // Sp = PC강재의 인장력의 전단력 작용방향의 분력 -> TP 하중조건의 Fz값으로 적용

			const double dbw = SisrParam.db;
			const double dd = SisrParam.dd;
			const double dPt = SisrParam.GetPt();
			const double dTau_r = CalcTau_r(kElem, SisrD.kSipa, SisrParam);
			const double dk = facH29.Susd(SipaD.nEarthquakeType)[T_SIFA_RC_Susd::kK];
			// Sc = k x τr x bw x d + Sd x (Mo/Md)
			const double dSc = dk * dTau_r * dbw * dd + dSd * (dM0 / dMd);

			// Ss = Cds x k x (Σ(Aw x σsky x d x (sinθ+cosθ) / 1.15a))
			const double dSigma_sky = min(MatdD.SubRebarData.B_fy, 345.0); /// 전단보강철근의 항복점 (N/mm2), 상한치 345N/mm2	
			const double dSinCos = CMathFunc::mathSin(SisrParam.dTheta) + CMathFunc::mathCos(SisrParam.dTheta);
			const double dSs = SisrParam.dCds * dk * (dgn::SafeDiv(SisrParam.dAw * dSigma_sky * dd * dSinCos, 1.15 * SisrParam.da));

			// Sp = Ap * σpe * sinα (단, 전단력이 작용하는 방향의 두께가 얇은 부재에서는 Sp=0으로 한다)
			const double dSigCosp = CMathFunc::mathSin(TndnParam.dTheta) + CMathFunc::mathCos(TndnParam.dTheta);
			const double dSpi = dgn::SafeDiv((dSp$ + TndnParam.dAw * dSigma_sky) * dd * dSigCosp, 1.15 * TndnParam.da);
			const double dSp = (nDir == seis_jp::kY_Dir) ? 0.0 : dgn::EQ0(TndnParam.dAw) ? dSp$ : dSpi - dSp$;//<-검증필요

			T_SISR_EVAL_ROAD3_CAPA_DIAG& rDiag = rBase.CapaDiag;
			rDiag.dB = dbw;
			rDiag.dD = dd;
			rDiag.dM0 = dM0;
			rDiag.dMd = dMd;
			rDiag.dK = dk;
			rDiag.dSc = dSc; /// Sc = 콘크리트가 부담하는 전단내력 = k x τc x bw x d
			rDiag.dSs = dSs; /// Ss = 주방향 설계에 있어 전단력에 대해 배치했다고 인정되는 사인장철근이 부딤하는 전단내력의 합계
			rDiag.dSp = dSp;
			//Susd(사인장 파괴에 대한 전단력의 한계(N)) = ξ1 x ξ2 x (φuc x Sc + φus x Ss) + ξ1 x ξ2 x φup x Sp
			double dXi1 = facH29.Susd(SipaD.nEarthquakeType)[T_SIFA_RC_Susd::kXi1];
			double dXi2 = facH29.Susd(SipaD.nEarthquakeType)[T_SIFA_RC_Susd::kXi2];
			double dPi_uc = facH29.Susd(SipaD.nEarthquakeType)[T_SIFA_RC_Susd::kPhi_uc_us];
			double dPi_us = facH29.Susd(SipaD.nEarthquakeType)[T_SIFA_RC_Susd::kPhi_uc_us];
			double dXi1_p = facH29.Susd(SipaD.nEarthquakeType)[T_SIFA_RC_Susd::kXi1_p];
			double dXi2_Phi_up = facH29.Susd(SipaD.nEarthquakeType)[T_SIFA_RC_Susd::kXi2_Phi_up];

			rDiag.dSus = dXi1*dXi2*(dPi_uc*dSc + dPi_us*dSs) + dXi1_p*dXi2_Phi_up*dSp;
			rDiag.dRatio = dgn::SafeDivAbs(rBase.Forc.dShear, rDiag.dSus);

			const double dSigmack = MatdD.Data1.Design.C_fc; /// 콘크리트의 설계기준강도 (N/mm2)
			const double dTauMax = CalcTaumax(dSigmack);
			const double dSucw = dTauMax * dbw * dd;

			T_SISR_EVAL_ROAD3_CAPA_COMP& rComp = rBase.CapaComp;
			rComp.dB = dbw;
			rComp.dD = dd;
			rComp.dTaumax = dTauMax;  // τmax = 콘크리트 평균 전단응력도의 최대치에서 표 4.3.2 의 값을 사용
			rComp.dSucw = dSucw;      // Sucw = τmax * bw * d
			rComp.dSp = dSp;       // Sp	= PC강재의 인장력의 전단력 작용방향의 분력.
			// Sucd(웹 콘크리트의 압축에 대한 전단력 제한치) = ξ1 x ξ2 x φucw x Sucw + ξ1 x ξ2 x φup x Sp
			dXi1 = facH29.Sucd(SipaD.nEarthquakeType)[T_SIFA_RC_Sucd::kXi1];
			double dXi2_Phi_ucw = facH29.Sucd(SipaD.nEarthquakeType)[T_SIFA_RC_Sucd::kXi2_Phi_ucw];

			rComp.dSuc = dXi1*dXi2_Phi_ucw*dSucw + dXi1_p*dXi2_Phi_up*dSp;
			rComp.dRatio = dgn::SafeDivAbs(rBase.Forc.dShear, rComp.dSuc);
		}
	};

	L_CalcCapa(k_I, aParamMax, rEval.PosI.RsltMax);
	L_CalcCapa(k_I, aParamMin, rEval.PosI.RsltMin);

	L_CalcCapa(k_J, aParamMax, rEval.PosJ.RsltMax);
	L_CalcCapa(k_J, aParamMin, rEval.PosJ.RsltMin);
}

void CSeismicDesignSisr_H29_Road3::CalcRoad3Param(const T_SISR_K& SisrK, T_ELEM_K ElemK, int nSeismicForceDirection, int nDir,
													const T_THIS_K_LIST& aThisK, const T_SISR_PARAM_POS aSisrParam[2],
													OUT CRoad3Param_H29 aParamMax[2], OUT CRoad3Param_H29 aParamMin[2])
{
	T_SISR_D dSisr;
	if (!m_pDoc->m_pAttrCtrl2->GetQSisr()->Get(SisrK, dSisr)) return;

	GetSd(ElemK, nDir, aThisK, aParamMax, aParamMin);
	GetSp(SisrK, ElemK, nDir, &aParamMax[0], &aParamMax[1], &aParamMin[0], &aParamMin[1]);
	GetMd(ElemK, nDir, aThisK, &aParamMax[0], &aParamMax[1], &aParamMin[0], &aParamMin[1]);
	GetMo(ElemK, nSeismicForceDirection, nDir, aThisK, aSisrParam, &aParamMax[0], &aParamMax[1], &aParamMin[0], &aParamMin[1]);
}

void CSeismicDesignSisr_H29_Road3::GetSd(const T_ELEM_K& ElemK, const int& nExamDir, const T_THIS_K_LIST& aThisK,
										OUT CRoad3Param_H29 aParamMax[2], OUT CRoad3Param_H29 aParamMin[2])
{
	const int nIdxShear = [nExamDir]()
	{
		switch (nExamDir)
		{
		case seis_jp::kY_Dir: return STRB_SHEAR_Y_I;
		case seis_jp::kZ_Dir: return STRB_SHEAR_Z_I;
		default: ASSERT(0); return 2;
		}
	}();

	auto L_CalcAvg = [](const std::vector<double>& vdS)
	{
		const int nSize = vdS.size();
		if (nSize < 1) return 0.0;

		double dSum = 0.0;
		for (int i = 0; i < nSize; ++i)
		{
			dSum += vdS[i];
		}
		return dgn::SafeDiv(dSum, (double)nSize);
	};

	std::vector<double> vdSmaxI;
	std::vector<double> vdSmaxJ;
	std::vector<double> vdSminI;
	std::vector<double> vdSminJ;

	const int nSize = aThisK.GetSize();
	for (int i = 0; i < nSize; ++i)
	{
		const T_THIS_K kThis = aThisK[i];

		T_LCOM_D LcomD;
		if (!m_pDoc->m_pPostCtrl->MakeLoadComb(D_LOADCASE_HISTORY, kThis, LcomD)) { ASSERT(0); continue; }
		m_pDoc->m_pPostCtrl->SelectLoadCombNew(LcomD);

		T_STRB_D StrbMax, StrbMin;
		if (!m_pDoc->m_pPostCtrl->GetStrbNew(ElemK, &StrbMax, &StrbMin, NULL, TRUE)) { ASSERT(0); continue; }

		vdSmaxI.emplace_back(StrbMax.dblForce[STRB_POSI_I][nIdxShear]);
		vdSmaxJ.emplace_back(StrbMax.dblForce[STRB_POSI_J][nIdxShear + 6]);

		vdSminI.emplace_back(StrbMin.dblForce[STRB_POSI_I][nIdxShear]);
		vdSminJ.emplace_back(StrbMin.dblForce[STRB_POSI_J][nIdxShear + 6]);
	}

	aParamMax[0].dSd = L_CalcAvg(vdSmaxI);
	aParamMax[1].dSd = L_CalcAvg(vdSmaxJ);
	aParamMin[0].dSd = L_CalcAvg(vdSminI);
	aParamMin[1].dSd = L_CalcAvg(vdSminJ);
}

double CSeismicDesignSisr_H29_Road3::CalcTau_r(const T_ELEM_K& kElem, const T_SIPA_K& kSipa, IN T_SISR_PARAM_BASE& param)
{
	T_ELEM_D ElemD;
	if (!m_pDoc->m_pAttrCtrl->GetElem(kElem, ElemD)) { return 0.0; }

	T_MATD_D MatdD;
	if (!m_pDoc->m_pAttrCtrl->GetMatlDesign(ElemD.elmat, MatdD)) { return 0.0; }

	T_SIPA_D SipaD;
	if (!m_pDoc->m_pAttrCtrl2->GetQSipa()->Get(kSipa, SipaD)) { return 0.0; }

	const double dSigmack = MatdD.Data1.Design.C_fc; /// 콘크리트의 설계기준강도 (N/mm2)

	// τr= τc x Ce x Cpt x Cdc x Cc
	double dTau_c = GetTau_c(dSigmack);
	double dCe = GetCe(param.dd);
	double dCpt = GetCpt(param.GetPt());
	double dCdc = GetCdc(kElem, param);
	double dCc = GetCc(SipaD.nSeismicForceType);

	double dTau_r = dTau_c * dCe * dCpt * dCdc * dCc;
	return dTau_r;
}

double CSeismicDesignSisr_H29_Road3::GetTau_c(const double& dSigmack)
{
	// 콘크리트가 부담할 수 있는 전단 응력도의 기본 값, tauc.
	std::vector<std::pair<double, double>> vdTauc;
	vdTauc.emplace_back(std::make_pair(21.0, 0.33));
	vdTauc.emplace_back(std::make_pair(24.0, 0.35));
	vdTauc.emplace_back(std::make_pair(27.0, 0.36));
	vdTauc.emplace_back(std::make_pair(30.0, 0.37));
	vdTauc.emplace_back(std::make_pair(40.0, 0.41));
	vdTauc.emplace_back(std::make_pair(50.0, 0.44));
	vdTauc.emplace_back(std::make_pair(60.0, 0.47));
	vdTauc.emplace_back(std::make_pair(70.0, 0.47));
	vdTauc.emplace_back(std::make_pair(80.0, 0.47));

	return CMathFunc::mathInterpolateLinear(vdTauc, dSigmack);
}

double CSeismicDesignSisr_H29_Road3::GetCe(const double& dD)
{
	// Ce：부재 단면의 유효 높이 d에 관한 보정 계수
	std::vector<std::pair<double, double>> vdCe;
	vdCe.emplace_back(std::make_pair(300, 1.4));
	vdCe.emplace_back(std::make_pair(1000, 1.0));
	vdCe.emplace_back(std::make_pair(3000, 0.7));
	vdCe.emplace_back(std::make_pair(5000, 0.6));
	vdCe.emplace_back(std::make_pair(10000, 0.5));

	return CMathFunc::mathInterpolateLinear(vdCe, dD);
}

double CSeismicDesignSisr_H29_Road3::GetCpt(const double& dPt)
{
	const double dptPercent = dPt * 100.0;

	// Cpt：축 방향으로 배치 된 인장 측 철근 또는 PC 강재 비 Pt 에 관한 보정계수
	std::vector<std::pair<double, double>> vdCpt;
	vdCpt.emplace_back(std::make_pair(0.1, 0.7));
	vdCpt.emplace_back(std::make_pair(0.2, 0.9));
	vdCpt.emplace_back(std::make_pair(0.3, 1.0));
	vdCpt.emplace_back(std::make_pair(0.5, 1.2));
	vdCpt.emplace_back(std::make_pair(1.0, 1.5));
	vdCpt.emplace_back(std::make_pair(2.0, 1.9));
	vdCpt.emplace_back(std::make_pair(3.0, 2.2));

	return CMathFunc::mathInterpolateLinear(vdCpt, dptPercent);
}

double CSeismicDesignSisr_H29_Road3::GetCdc(const T_ELEM_K& kElem, OUT T_SISR_PARAM_BASE& param)
{
	param.dCdc = 1.0;
	double dShearSpan = GetShearSpan(kElem);
	if (dgn::UQ0(dShearSpan)) {
		const double dadprime = dgn::SafeDiv(dShearSpan, param.dd);

		param.dCdc = CalcCdc(dadprime);
		param.dCds = CalcCds(dadprime);
	}
	return param.dCdc;
}

double CSeismicDesignSisr_H29_Road3::GetCc(int nSeisForcType)
{
	// 하중의 (+)(-) 반복 작용의 영향에 대한 보정 계수. 소성화를 기대하지 않는 부재는 1.0 을 표준으로한다.
	return 1.0;
}
#pragma endregion <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

#pragma region >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> CSeismicDesignSisr_H29_Road4 
CSeismicDesignSisr_H29_Road4::CSeismicDesignSisr_H29_Road4() : CSeismicDesignSisr_H29_Road3()
{
}

CSeismicDesignSisr_H29_Road4::~CSeismicDesignSisr_H29_Road4()
{
}

void CSeismicDesignSisr_H29_Road4::AddProgressStep(const T_SISR_K& kSisr)
{
	CSeismicDesignSisr_H29_Road3::AddProgressStep(kSisr);
}

BOOL CSeismicDesignSisr_H29_Road4::Calculate(const T_SISR_K& kSisr)
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

		if (m_mMemberMap[kElem].nDirShear == seis_jp::eDirShear::kYZ_Dir) {
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

void CSeismicDesignSisr_H29_Road4::RunEvaluation(const P_INPUT* pInput, const T_ELEM_K& kElem, const int& nExamShearDir)
{
	T_ELEM_D ElemD;
	if (!m_pDoc->m_pAttrCtrl->GetElem(kElem, ElemD)) { ASSERT(FALSE); return; }
	if (m_mMemberMap.find(kElem) == m_mMemberMap.end()) { ASSERT(FALSE); return; }

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

	CalcForce(kElem, pInput->tSipa.aSeisThisK, nExamShearDir, eval);
	CalcRatio(pInput, kElem, nExamShearDir, aParam, eval);

	CBrdgSeisEvalResult_JP* pResult = m_pDoc->m_pPostCtrl->GetBrdgSeisEvalResult_JP();
	if (!pResult) { ASSERT(0); return; }
	pResult->SetResultSisrRoad3(eval);
}


void CSeismicDesignSisr_H29_Road4::CalcRatio(const P_INPUT* pInput, const T_ELEM_K& kElem, const int nDir,
											T_SISR_PARAM_POS aParam[2], OUT T_SISR_EVAL_ROAD3& rEval)
{
#pragma region // Data Get
	T_ELEM_D ElemD;
	if (!m_pDoc->m_pAttrCtrl->GetElem(kElem, ElemD)) { return; }

	T_SECT_D SectD;
	if (!m_pDoc->m_pPostCtrl->GetSectPost(ElemD.elpro, SectD)) { return; }

	T_MATD_D MatdD;
	if (!m_pDoc->m_pAttrCtrl->GetMatlDesign(ElemD.elmat, MatdD)) { return; }

	T_SISR_D SisrD;
	if (!m_pDoc->m_pAttrCtrl2->GetQSisr()->Get(pInput->kSisr, SisrD)) { return; }

	T_SIPA_D SipaD;
	if (!m_pDoc->m_pAttrCtrl2->GetQSipa()->Get(SisrD.kSipa, SipaD)) { return; }

	T_SIFA_H29_D facH29;
	if (m_pDoc->m_pAttrCtrl2->GetQSifaH29()->Get(facH29) == FALSE) return;
#pragma endregion

	CRoad3Param_H29 aParamMax[2];
	CRoad3Param_H29 aParamMin[2];
	CalcRoad3Param(pInput->kSisr, kElem, SipaD.nSeismicForceDirection, nDir, pInput->tSipa.aSeisThisK, aParam, aParamMax, aParamMin);

	auto L_CalcCapa = [&](int nPos, const CRoad3Param_H29& Param, OUT T_SISR_EVAL_ROAD3_BASE& rBase)
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
			T_SISR_PARAM_BASE tensParam; tensParam.Init();
			GetSisrParamBase(aParam[nPos], SipaD.nSeismicForceDirection, (seis_jp::eDirShear)nDir, nPos, rBase.Forc.dMoment, tensParam);

			const double dMd = Param.dMd; // Md = 부재 단면에 작용하는 휨 모멘트(종국 휨모멘트) -> 지진파의 평균값 적용.
			const double dM0 = Param.dM0; // M0 = 프레스트레스 및 축방향력에 의해 콘크리트의 응력도가 부재 인장연에서서 0이 되는 휨모멘트.
			const double dSd = Param.dSd; // Sd = 부재 단면에 작용하는 전단력 (N) -> 지진파의 평균값 적용.
			const double dSp = Param.dSp; // Sp = PC강재의 인장력의 전단력 작용방향의 분력 -> TP 하중조건의 Fz값으로 적용

			const double dbw = tensParam.db;
			const double dd = tensParam.dd;
			const double dPt = tensParam.GetPt();
			const double dTau_r = CalcTau_r(kElem, SisrD.kSipa, tensParam);
			const double dk = facH29.Susd(SipaD.nEarthquakeType)[T_SIFA_RC_Susd::kK];
			// Sc = k x τr x bw x d + Sd x (Mo/Md)
			const double dSc = dk * dTau_r * dbw * dd + dSd * (dM0 / dMd);

			// Ss = Cds x k x (Σ(Aw x σsky x d x (sinθ+cosθ) / 1.15a))
			const double dSigma_sky = min(MatdD.SubRebarData.B_fy, 345.0); /// 전단보강철근의 항복점 (N/mm2), 상한치 345N/mm2	
			const double dSinCos = CMathFunc::mathSin(tensParam.dTheta) + CMathFunc::mathCos(tensParam.dTheta);
			const double dSs = tensParam.dCds * dk * (dgn::SafeDiv(tensParam.dAw * dSigma_sky * dd * dSinCos, 1.15 * tensParam.da));

			T_SISR_EVAL_ROAD3_CAPA_DIAG& rDiag = rBase.CapaDiag;
			rDiag.dB = dbw;
			rDiag.dD = dd;
			rDiag.dM0 = dM0;
			rDiag.dMd = dMd;
			rDiag.dK = dk;
			rDiag.dSc = dSc; /// Sc = 콘크리트가 부담하는 전단내력 = k x τc x bw x d
			rDiag.dSs = dSs; /// Ss = 주방향 설계에 있어 전단력에 대해 배치했다고 인정되는 사인장철근이 부딤하는 전단내력의 합계
			rDiag.dSp = dSp;
			//Susd(사인장 파괴에 대한 전단력의 한계(N)) = ξ1 x ξ2 x (φuc x Sc + φus x Ss) + ξ1 x ξ2 x φup x Sp
			double dXi1 = facH29.Susd(SipaD.nEarthquakeType)[T_SIFA_RC_Susd::kXi1];
			double dXi2 = facH29.Susd(SipaD.nEarthquakeType)[T_SIFA_RC_Susd::kXi2];
			double dPi_uc = facH29.Susd(SipaD.nEarthquakeType)[T_SIFA_RC_Susd::kPhi_uc_us];
			double dPi_us = facH29.Susd(SipaD.nEarthquakeType)[T_SIFA_RC_Susd::kPhi_uc_us];
			double dXi1_p = facH29.Susd(SipaD.nEarthquakeType)[T_SIFA_RC_Susd::kXi1_p];
			double dXi2_Phi_up = facH29.Susd(SipaD.nEarthquakeType)[T_SIFA_RC_Susd::kXi2_Phi_up];

			rDiag.dSus = dXi1 * dXi2 * (dPi_uc * dSc + dPi_us * dSs) + dXi1_p * dXi2_Phi_up * dSp;
			rDiag.dRatio = dgn::SafeDivAbs(rBase.Forc.dShear, rDiag.dSus);
		}
	};

	L_CalcCapa(k_I, aParamMax[0], rEval.PosI.RsltMax);
	L_CalcCapa(k_I, aParamMin[0], rEval.PosI.RsltMin);

	L_CalcCapa(k_J, aParamMax[1], rEval.PosJ.RsltMax);
	L_CalcCapa(k_J, aParamMin[1], rEval.PosJ.RsltMin);
}

void CSeismicDesignSisr_H29_Road4::CalcRoad3Param(const T_SISR_K& SisrK, T_ELEM_K ElemK, int nSeismicForceDirection, int nDir,
	const T_THIS_K_LIST& aThisK, const T_SISR_PARAM_POS aSisrParam[2], OUT CRoad3Param_H29 aParamMax[2], OUT CRoad3Param_H29 aParamMin[2])
{
	T_SISR_D dSisr;
	if (!m_pDoc->m_pAttrCtrl2->GetQSisr()->Get(SisrK, dSisr)) return;

	GetSd(ElemK, nDir, aThisK, aParamMax, aParamMin);
	GetSp(SisrK, ElemK, nDir, &aParamMax[0], &aParamMax[1], &aParamMin[0], &aParamMin[1]);
	GetMd(ElemK, nDir, aThisK, &aParamMax[0], &aParamMax[1], &aParamMin[0], &aParamMin[1]);
	GetMo(ElemK, nSeismicForceDirection, nDir, aThisK, aSisrParam , &aParamMax[0], &aParamMax[1], &aParamMin[0], &aParamMin[1]);
}

void CSeismicDesignSisr_H29_Road4::GetMo(const T_ELEM_K& ElemK, 
										 const int& nSeismicForceDir, 
										 const int& nShearDir, 
										 const T_THIS_K_LIST& aThisK,
										 const T_SISR_PARAM_POS aSisrParam[2],
										 OUT iRoad3Param* pMax_I, 
										 OUT iRoad3Param* pMax_J, 
										 OUT iRoad3Param* pMin_I, 
										 OUT iRoad3Param* pMin_J)
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

		//dyc : 도심에서 인장측 까지의 거리
		//dIc : 부재단면의 도심축에 대한 단면 2차 모멘트
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

double CSeismicDesignSisr_H29_Road4::GetCpt(const double& dPt)
{
	const double dptPercent = dPt * 100.0;

	// Cpt：축 방향으로 배치 된 인장 측 철근 또는 PC 강재 비 Pt 에 관한 보정계수
	std::vector<std::pair<double, double>> vdCpt;
	vdCpt.emplace_back(std::make_pair(0.1, 0.7));
	vdCpt.emplace_back(std::make_pair(0.2, 0.9));
	vdCpt.emplace_back(std::make_pair(0.3, 1.0));
	vdCpt.emplace_back(std::make_pair(0.5, 1.2));
	vdCpt.emplace_back(std::make_pair(1.0, 1.5));

	return CMathFunc::mathInterpolateLinear(vdCpt, dptPercent);
}
#pragma endregion <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

#pragma region >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> CSeismicDesignSisr_H29_Road5 
CSeismicDesignSisr_H29_Road5::CSeismicDesignSisr_H29_Road5() : CSeismicDesignSisr_H29_Road4()
{
}

CSeismicDesignSisr_H29_Road5::~CSeismicDesignSisr_H29_Road5()
{
}

void CSeismicDesignSisr_H29_Road5::AddProgressStep(const T_SISR_K& kSisr)
{
	CSeismicDesignSisr_H29_Road4::AddProgressStep(kSisr);
}

BOOL CSeismicDesignSisr_H29_Road5::Calculate(const T_SISR_K& SisrK)
{
	return CSeismicDesignSisr_H29_Road4::Calculate(SisrK);
}

void CSeismicDesignSisr_H29_Road5::CalcRoad3Param(const T_SISR_K& SisrK, T_ELEM_K ElemK, int nSeismicForceDirection, int nDir,
	const T_THIS_K_LIST& aThisK, const T_SISR_PARAM_POS aSisrParam[2], OUT CRoad3Param_H29 aParamMax[2], OUT CRoad3Param_H29 aParamMin[2])
{
	CSeismicDesignSisr_H29_Road4::CalcRoad3Param(SisrK, ElemK, nSeismicForceDirection, nDir, aThisK, aSisrParam, aParamMax, aParamMin);

	// 콘크리트가 부담하는 전단력의 특성치 Sc 계산시
	// 축방향 압축력에 의한 콘크리트의 전단력 증가를 고려하지 않기위해
	// Sd 값을 전부 0 으로 설정한다.
	aParamMax[0].dSd = 0.0;
	aParamMax[1].dSd = 0.0;
	aParamMin[0].dSd = 0.0;
	aParamMin[1].dSd = 0.0;
}
#pragma endregion <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<