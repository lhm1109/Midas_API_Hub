#include "stdafx.h"
#include "SeismicDesignSisr_H14_Road3_Compress.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_PostCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl2.h"
#include "..\wg_db\DB_QUERY_SEIS_CVL_JP.h"
#include "..\wg_db\BrdgSeisEvalResult_JP.h"
#include "..\wg_db\DB_ST_DT_SISR_EVAL_COMPRESS.h"

CSeismicDesignSisr_H14_Road3_Compress::CSeismicDesignSisr_H14_Road3_Compress(const T_SISR_K& kSisr, const T_ELEM_K& kElem)
{
	m_kSisr = kSisr;
	m_kElem = kElem;
	m_pDoc = CDBDoc::GetDocPoint();
	ASSERT(m_pDoc);
}

CSeismicDesignSisr_H14_Road3_Compress::~CSeismicDesignSisr_H14_Road3_Compress()
{
}

void CSeismicDesignSisr_H14_Road3_Compress::Calculate(const T_SISR_K& kSisr, const T_ELEM_K& kElem)
{
	return CSeismicDesignSisr_H14_Road3_Compress(kSisr, kElem).Invoke();
}

void CSeismicDesignSisr_H14_Road3_Compress::Invoke()
{
	T_SISR_D dSisr;
	if (m_pDoc->m_pAttrCtrl2->GetQSisr()->Get(m_kSisr, dSisr) == FALSE) return;
	T_SIGR_D dSigr;
	if (m_pDoc->m_pAttrCtrl2->GetQSigr()->Get(dSisr.kSigr, dSigr) == FALSE) return;
	//여기는 상부 구조만 대상으로 계산 한다.
	if (dSigr.nSubType != T_SIGR_D::kUpper) return;

	T_SISR_EVAL_COMPRESS eval;
	eval.kElem = m_kElem;

	//전단내력계산
	CalcCapacity(&eval.calcCapa);

	//해석에 의한 전단력
	CalcSeisEvaluation(&eval.analCapa);

	eval.dRatioMin = dgn::SafeDiv(eval.calcCapa.dSuc, eval.analCapa.dAvergeMin);
	double dValMin = dgn::SafeDiv(eval.calcCapa.dSuc, eval.analCapa.dShMin);
	eval.bCheckResultMin = dgn::ME(dValMin, 1.0) ? TRUE : FALSE;

	eval.dRatioMax = dgn::SafeDiv(eval.calcCapa.dSuc, eval.analCapa.dAvergeMax);
	double dValMax = dgn::SafeDiv(eval.calcCapa.dSuc, eval.analCapa.dShMax);
	eval.bCheckResultMax = dgn::ME(dValMax, 1.0) ? TRUE : FALSE;

	// save result
	CBrdgSeisEvalResult_JP* pResult = m_pDoc->m_pPostCtrl->GetBrdgSeisEvalResult_JP(); if (!pResult) { ASSERT(0); return; }
	pResult->SetResultSisrCompress(dSisr.kSigr, m_kSisr, &eval);
}

void CSeismicDesignSisr_H14_Road3_Compress::CalcCapacity(T_SISR_EVAL_COMPRESS_CALC_CAPACITY* pCalc)
{
	//Suc(전단내력(kN)) = τmax * bw * d + Sp
	//Suc	웹 콘크리트의 압축에 대한 내력
	//τmax	콘크리트 평균 전단응력도의 최대치에서 표 4.3.2 의 값을 사용
	//bw	부재단면의 웹 두께(mm)
	//d		부재단면의 유효높이(mm)
	//Sp	(PC강재의 인장력의 전단력 작용방향의 분력)

//	if (하중 - 프리스트레스 - PC강재의 배치형태 == TRUE)
//	{
	//Sp = Ap * σpe * sinα
	//단, 전단력이 작용하는 방향의 두께가 얇은 부재에서는 Sp=0으로 한다.

	//Ap	부재 단면에서의 PC강재의 단면적
	//σpe	부재 단면에서의 PC강재의 유효인장 응력도
	//α	PC강재가 부재각과 이루는 각도
//	}
}

void CSeismicDesignSisr_H14_Road3_Compress::CalcSeisEvaluation(OUT T_SISR_EVAL_COMPRESS_ANAL_CAPACITY* pAnal)
{
	T_SISR_D dSisr;
	if (m_pDoc->m_pAttrCtrl2->GetQSisr()->Get(m_kSisr, dSisr) == FALSE) return;

	T_SIPA_D dSipa;
	if (m_pDoc->m_pAttrCtrl2->GetQSipa()->Get(dSisr.kSipa, dSipa) == FALSE) return;

	//요소 1개당 3개의 하중 케이스에 대한 최대, 최소 전단력 값을 가져오고 평균을 구한다.
	double dSumMin = 0, dSumMax = 0;
	double nCount = dSipa.aSeisThisK.GetCount();
	for (int j = 0; j < nCount; j++)
	{
		T_THIS_K kThis = dSipa.aSeisThisK[j];

		double dSmin = 0, dSmax = 0;
		//GetShearForceMinMax(kElem, kThis, dSmin, dSmax);

		pAnal->aSmin.Add(dSmin);
		pAnal->aSmax.Add(dSmax);

		dSumMin = dSumMin + dSmin;
		dSumMax = dSumMax + dSmax;
	}

	double dAvergeMin = dgn::SafeDiv(dSumMin, nCount);
	double dAvergeMax = dgn::SafeDiv(dSumMax, nCount);

	pAnal->dAvergeMin = dAvergeMin;
	pAnal->dAvergeMax = dAvergeMax;
}