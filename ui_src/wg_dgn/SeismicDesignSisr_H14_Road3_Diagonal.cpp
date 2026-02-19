#include "stdafx.h"
#include "SeismicDesignSisr_H14_Road3_Diagonal.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_PostCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl2.h"
#include "..\wg_db\DB_QUERY_SEIS_CVL_JP.h"
#include "..\wg_db\BrdgSeisEvalResult_JP.h"
#include "..\wg_db\DB_ST_DT_SISR_EVAL_ROAD3.h"

#include "DgnProgressDlg.h"

CSeismicDesignSisr_H14_Road3_Diagonal::CSeismicDesignSisr_H14_Road3_Diagonal() : CSeismicDesignSisrBase()
{
	m_pProgress = nullptr;
}

CSeismicDesignSisr_H14_Road3_Diagonal::~CSeismicDesignSisr_H14_Road3_Diagonal()
{
}

void CSeismicDesignSisr_H14_Road3_Diagonal::AddProgressStep(const T_SISR_K& kSisr, CDgnProgressDlg* pProgress)
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	if (pDoc == NULL) return;

	T_SISR_D dSisr;
	if (!pDoc->m_pAttrCtrl2->GetQSisr()->Get(kSisr, dSisr)) return;

	T_SIGR_UPPER_D SigrUpperD;
	if (!pDoc->m_pAttrCtrl2->GetQSigrUpper()->Get(dSisr.kSigr, SigrUpperD)) return;

	const int nElem = SigrUpperD.aElements.GetSize();
	for (int i = 0; i < nElem; ++i)
	{
		pProgress->m_nTotalSteps += SigrUpperD.aElements.GetSize();
	}	
}

BOOL CSeismicDesignSisr_H14_Road3_Diagonal::Calculate(const T_SISR_K& SisrK)
{
	CBrdgSeisEvalResult_JP* pResult = m_pDoc->m_pPostCtrl->GetBrdgSeisEvalResult_JP();
	if (!pResult) { ASSERT(0); }

	T_SISR_D SisrD;
	if (!m_pDoc->m_pAttrCtrl2->GetQSisr()->Get(SisrK, SisrD)) return FALSE;

	T_SIPA_D SipaD;
	if (!m_pDoc->m_pAttrCtrl2->GetQSipa()->Get(SisrD.kSipa, SipaD)) return FALSE;

	T_SIGR_UPPER_D SigrUpperD;
	if (!m_pDoc->m_pAttrCtrl2->GetQSigrUpper()->Get(SisrD.kSigr, SigrUpperD)) return FALSE;

	const int nElem = SigrUpperD.aElements.GetSize();	
	for (int i = 0; i < nElem; ++i)
	{
		const T_ELEM_K ElemK = SigrUpperD.aElements[i];
		T_ELEM_D ElemD;
		m_pDoc->m_pAttrCtrl->GetElem(ElemK, ElemD);

		T_SISR_EVAL_DIAG eval;
		eval.kSipa = SisrD.kSipa;
		eval.kSigr = SisrD.kSigr;
		eval.kElem = ElemK;

		const int nDir = SigrUpperD.nDir;
		T_SISR_PARAM_POS ParamI, ParamJ;
		CalcBeamShearParam(ElemD.elpro, ParamI, ParamJ);
		//CalcForce(ElemK, SipaD.aSeisThisK, nDir, eval);
		CalcRatio(SisrK, ElemK, nDir, ParamI, ParamJ, eval);

		// save result
		//pResult->SetResultSisrLower(dSisr.kSipa, dSisr.kSigr, ElemK, eval); //TODO
		if (m_pProgress->NextStep() == FALSE) throw FALSE;
	}

	return TRUE;
}

void CSeismicDesignSisr_H14_Road3_Diagonal::CalcRatio(const T_SISR_K& SisrK, const T_ELEM_K& kElem, const int nDir,
	const T_SISR_PARAM_POS& ParamI, const T_SISR_PARAM_POS& ParamJ, OUT T_SISR_EVAL_DIAG& rEval)
{
	//Sus(전단내력(kN)) = Sc + Ss + Sp
	//Sc	(콘크리트가 부담하는 전단내력)
	//Ss	(주방향 설계에 있어 전단력에 대해 배치했다고 인정되는 사인장철근이 부딤하는 전단내력의 합계)
	//Sp	(PC강재의 인장력의 전단력 작용방향의 분력)

	//Sc = k x τc x bw x d
	//k = 1 + Mo/Md <= 2

	//Ss = Σ(Aw * σsy * d * (sinθ+cosθ) / 1.15a)

	//Sp = 0;
//	if (하중 - 프리스트레스 - PC강재의 배치형태 == TRUE)
//	{
	//Sp = Ap * σpe * sinα
	//단, 전단력이 작용하는 방향의 두께가 얇은 부재에서는 Sp=0으로 한다.

	//τc	콘크리트가 부담할 수 있는 평균전단응력
	//bw	부재 단면의 웹 두께
	//d		부재 단면의 유효높이

	//Md	부재 단면에 작용하는 휨 모멘트(종극휨 모멘트)
	//Mo	프레스트레스 및 축방향력에 의해 콘크리트의 응력도가 부재 인장연에서서 0이 되는 휨 모멘트
	//		Mo = (σce + N/Ac) x Ic/y
	//σce	유효 프레스트레스력에 의한 부재 인장연의 응력도
	//N		부재 단면에 작용하는 축방향력(사하중 작용시), 단, 프레스트레스력은 포함되지 않는다.
	//Ic	부재 단면의 도심축에 관한 단면2차 모멘트
	//Ac	부재 단면적
	//y		부재 단면의 도심으로부터 부재 인장연까지의 거리

	//Aw	간격 a 및 각도 θ로 배근된 사인장철근의 단면적
	//σsy	사인장철근의 항복응력, 345(N/mm2)를 상한으로 한다.
	//θ	사인장철근과 부재축이 이루는 각 (deg)
	//a		사인장철근의 간격

	//Ap	부재 단면에서의 PC강재의 단면적
	//σpe	부재 단면에서의 PC강재의 유효인장 응력도
	//α	PC강재가 부재각과 이루는 각도

	T_ELEM_D ElemD;
	if (!m_pDoc->m_pAttrCtrl->GetElem(kElem, ElemD)) { return; }

	T_MATD_D MatdD;
	if (!m_pDoc->m_pAttrCtrl->GetMatlDesign(ElemD.elmat, MatdD)) { return; }

	const double dSigmack = MatdD.Data1.Design.C_fc; /// 콘크리트의 설계기준강도 (N/mm2)
	const double dSigmasy = min(MatdD.SubRebarData.B_fy, 345.0); /// 전단보강철근의 항복점 (N/mm2), 상한치 345N/mm2		

	auto L_CalcCapa = [&](const T_SISR_PARAM_BASE& SisrParam, OUT T_SISR_EVAL_DIAG_BASE& rBase)
	{		
		const double db = SisrParam.db;
		const double dd = SisrParam.dd;
		const double dpt = SisrParam.GetPt();
		const double da = SisrParam.da;
		const double dAw = SisrParam.dAw;
		const double dTheta = SisrParam.dTheta;
		const double dAlpha = 0.0; //TODO.
		const double dTauc = CalcTauc(dSigmack);
		const double dSinCos = CMathFunc::mathSin(dTheta) + CMathFunc::mathCos(dTheta);
				
		const double dM0 = 0.0; //TODO.
		const double dMd = 0.0; //TODO.
		const double dk = min(1.0 + dgn::SafeDiv(dM0, dMd), 2.0);
		const double dSc = dk * dTauc * db * dd;

		const double dSs = dgn::SafeDiv(dAw * dSigmasy  * dd * dSinCos, 1.15 * da);

		const double dAp = 0.0; //TODO.
		const double dSigmape = 0.0; //TODO.
		const double dSp = dAp * dSigmape * CMathFunc::mathSin(dAlpha);
		
		rBase.dB = db;
		rBase.dD = dd;
		rBase.dM0 = dM0;
		rBase.dMd = dMd;
		rBase.dK  = dk;
		rBase.dSc = dSc;
		rBase.dSs = dSs;
		rBase.dSp = dSp;
		rBase.dSus = dSc + dSs + dSp;		
		rBase.dRatio = dgn::SafeDivAbs(rBase.dShear, rBase.dSus);
	};

	switch (nDir)
	{
	case 1:
		{
			L_CalcCapa(dgn::ME0(rEval.PosI.RsltMax.dShear) ? ParamI.yTensTop : ParamI.yTensBot, rEval.PosI.RsltMax);
			L_CalcCapa(dgn::ME0(rEval.PosI.RsltMin.dShear) ? ParamI.yTensTop : ParamI.yTensBot, rEval.PosI.RsltMin);

			L_CalcCapa(dgn::ME0(rEval.PosJ.RsltMax.dShear) ? ParamJ.yTensBot : ParamJ.yTensTop, rEval.PosJ.RsltMax);
			L_CalcCapa(dgn::ME0(rEval.PosJ.RsltMin.dShear) ? ParamJ.yTensBot : ParamJ.yTensTop, rEval.PosJ.RsltMin);
		}
		break;
	case 2:
		{
			L_CalcCapa(dgn::ME0(rEval.PosI.RsltMax.dShear) ? ParamI.zTensTop : ParamI.zTensBot, rEval.PosI.RsltMax);
			L_CalcCapa(dgn::ME0(rEval.PosI.RsltMin.dShear) ? ParamI.zTensTop : ParamI.zTensBot, rEval.PosI.RsltMin);

			L_CalcCapa(dgn::ME0(rEval.PosJ.RsltMax.dShear) ? ParamJ.zTensBot : ParamJ.zTensTop, rEval.PosJ.RsltMax);
			L_CalcCapa(dgn::ME0(rEval.PosJ.RsltMin.dShear) ? ParamJ.zTensBot : ParamJ.zTensTop, rEval.PosJ.RsltMin);
		}
		break;
	default:
		ASSERT(0);
		break;
	}
}

double CSeismicDesignSisr_H14_Road3_Diagonal::CalcTauc(double dSigmack)
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

void CSeismicDesignSisr_H14_Road3_Diagonal::CalcSh()
{
	//	if (변단면이면) //부재 유효높이 변화에 따른 설계 전단력 보정값을 계산
	//Sh = S - M/d(tanβ + tanγ)
	//Sh	(부재의 유효높이 변화의 영향을 고려한 설계 전단력
	//S		(부재단면에 작용하는 전단력)
	//M		(부재단면에 작용하는 모멘트)
	//d		(부재다념의 유효 높이)
	//β	(부재압축연이 부재축이 되는 각도)
	//γ	(인장강재가 부재축이 되는 각도) - 교량은 한쪽면만 좁아지는 변단면이라 γ는 고려하지 않음
}