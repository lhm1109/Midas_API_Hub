#include "stdafx.h"
#include "SeismicDesignSisr_H24.h"

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

#pragma region >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> CSeismicDesignSisr_H24 
CSeismicDesignSisr_H24::CSeismicDesignSisr_H24() : CSeismicDesignBase()
{
}

CSeismicDesignSisr_H24::~CSeismicDesignSisr_H24()
{
}

void CSeismicDesignSisr_H24::AddProgressStep(CDgnProgressDlg* pProgress)
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

	CSeismicDesignSisr_H24_Road3 SisrRoad3D;
	SisrRoad3D.SetProgressDlg(pProgress);
	for (auto kSisr : vSisrRoad3)
	{
		SisrRoad3D.AddProgressStep(kSisr);
	}

	CSeismicDesignSisr_H24_Road4 SisrRoad4;
	SisrRoad4.SetProgressDlg(pProgress);
	for (auto kSisr : vSisrRoad4)
	{
		SisrRoad4.AddProgressStep(kSisr);
	}

	CSeismicDesignSisr_H24_Road5 SisrRoad5;
	SisrRoad5.SetProgressDlg(pProgress);
	for (auto kSisr : vSisrRoad5)
	{
		SisrRoad5.AddProgressStep(kSisr);
	}
}

BOOL CSeismicDesignSisr_H24::Calculate(CDgnProgressDlg* pProgress)
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

	CSeismicDesignSisr_H24_Road3 SisrRoad3D;
	SisrRoad3D.SetProgressDlg(pProgress);
	for (auto kSisr : vSisrRoad3)
	{
		SisrRoad3D.Calculate(kSisr);
	}

	CSeismicDesignSisr_H24_Road4 SisrRoad4;
	SisrRoad4.SetProgressDlg(pProgress);
	for (auto kSisr : vSisrRoad4)
	{
		SisrRoad4.Calculate(kSisr);
	}

	CSeismicDesignSisr_H24_Road5 SisrRoad5;
	SisrRoad5.SetProgressDlg(pProgress);
	for (auto kSisr : vSisrRoad5)
	{
		SisrRoad5.Calculate(kSisr);
	}

	return FALSE;
}
#pragma endregion <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

#pragma region >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> CSeismicDesignSisr_H24_Road3 
CSeismicDesignSisr_H24_Road3::CSeismicDesignSisr_H24_Road3() : CSeismicDesignSisr_H14_Road3()
{
}

CSeismicDesignSisr_H24_Road3::~CSeismicDesignSisr_H24_Road3()
{
}

void CSeismicDesignSisr_H24_Road3::AddProgressStep(const T_SISR_K& kSisr)
{
	CSeismicDesignSisr_H14_Road3::AddProgressStep(kSisr);
}

BOOL CSeismicDesignSisr_H24_Road3::Calculate(const T_SISR_K& SisrK)
{
	return CSeismicDesignSisr_H14_Road3::Calculate(SisrK);
}
#pragma endregion <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

#pragma region >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> CSeismicDesignSisr_H24_Road4 
CSeismicDesignSisr_H24_Road4::CSeismicDesignSisr_H24_Road4() : CSeismicDesignSisr_H14_Road4()
{
}

CSeismicDesignSisr_H24_Road4::~CSeismicDesignSisr_H24_Road4()
{
}

void CSeismicDesignSisr_H24_Road4::AddProgressStep(const T_SISR_K& kSisr)
{
	CSeismicDesignSisr_H14_Road4::AddProgressStep(kSisr);
}

BOOL CSeismicDesignSisr_H24_Road4::Calculate(const T_SISR_K& kSisr)
{
	return CSeismicDesignSisr_H14_Road4::Calculate(kSisr);
}

void CSeismicDesignSisr_H24_Road4::CalcRatio(const T_SISR_K& SisrK, const T_ELEM_K& kElem, const int nDir, const int nSeisForceType,
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
				const double dSigmack = m_pMatd->Data1.Design.C_fc; /// 콘크리트의 설계기준강도 (N/mm2)
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
#pragma endregion <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

#pragma region >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> CSeismicDesignSisr_H24_Road5 
CSeismicDesignSisr_H24_Road5::CSeismicDesignSisr_H24_Road5() : CSeismicDesignSisr_H14_Road5()
{
}

CSeismicDesignSisr_H24_Road5::~CSeismicDesignSisr_H24_Road5()
{
}

void CSeismicDesignSisr_H24_Road5::AddProgressStep(const T_SISR_K& kSisr)
{
	CSeismicDesignSisr_H14_Road5::AddProgressStep(kSisr);
}

BOOL CSeismicDesignSisr_H24_Road5::Calculate(const T_SISR_K& kSisr)
{
	return CSeismicDesignSisr_H14_Road5::Calculate(kSisr);
}

void CSeismicDesignSisr_H24_Road5::CalcRatio(const T_SISR_K& SisrK, const T_ELEM_K& kElem, const int nDir, const int nSeisForceType,
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
				const double dSigmack = m_pMatd->Data1.Design.C_fc; /// 콘크리트의 설계기준강도 (N/mm2)
				const double dSigma_sy = min(m_pMatd->SubRebarData.B_fy, 345.0);// σsy : 기설 띠철근(전단보강철근) 항복강도(N/mm2), 상한치 345N/mm2

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
#pragma endregion <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<