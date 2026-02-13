#include "stdafx.h"
#include "BrdgSeisEvalKALIS2023.h"

#include "..\wg_db\MathFunc.h"

CBrdgSeisEvalKALIS2023::CBrdgSeisEvalKALIS2023(void)
{
	m_CodeUnit.nBase_Length = D_UNITSYS_LENGTH_INDEX_MM;
	m_CodeUnit.nBase_Force = D_UNITSYS_FORCE_INDEX_N;
}

CBrdgSeisEvalKALIS2023::~CBrdgSeisEvalKALIS2023(void)
{
}

void CBrdgSeisEvalKALIS2023::CalcPlainConcCapacity(const UINT& EvgpK, const UINT& iColm, const EN_LOCAL_DIR& enDir, const EN_POSITION& enPos, const EN_FAILURE_MODE& enFalureMode, const T_BRDGEVAL_CALC_PARAM& ParamD, T_BRDGEVAL_EVALUATION& EvalD)
{
	EvalD.initialize();

	const double dfck = ParamD.MatlD.dfck;
	const double dPu = ParamD.ForceD.dForce[0]; // compression is positive
	const double dMd = (enDir == LONGI) ? fabs(ParamD.ForceD.dForce[4]) : fabs(ParamD.ForceD.dForce[5]); // 조합탄성지진력
	const double dVd = (enDir == LONGI) ? fabs(ParamD.ForceD.dForce[2]) : fabs(ParamD.ForceD.dForce[1]); // 조합탄성지진력
	const bool   bComp = MT0(dPu);

	// 휨성능
	const double dSm = (enDir == LONGI) ? ParamD.SetcD.dSy : ParamD.SetcD.dSz;
	const double dMn = [&bComp, &dfck, &dSm]()
	{
		if(bComp)
		{
			return 0.42 * sqrt(dfck) * dSm;
		}
		else
		{
			return 0.85 * dfck * dSm;
		}
	}();
	const double dFpcf = SafeDiv(dMn, dMd, _INFI4);

	// 축+휨성능
	const double dAg = ParamD.SetcD.dArea;
	const double dFpcc = [&]()
	{
		if(bComp)
		{
			const double dPn = dfck * dAg;
			const double dAxialR = SafeDiv(dPu, dPn, _INFI4);
			const double dFlexR = SafeDiv(dMd, dMn, _INFI4);
			return SafeDiv(1.0, (dAxialR + dFlexR), _INFI4);
		}
		else
		{
			const double dStr = 0.42 * sqrt(dfck);
			const double dStrS = SafeDiv(dMd, dSm, _INFI4);
			const double dStrA = SafeDiv(dPu, dAg, _INFI4); // tension, axial force is negative.
			return SafeDiv(dStr, (dStrS - dStrA), _INFI4);
		}
	}();

	// 전단성능
	const double dVn = 0.11 * sqrt(dfck) * dAg;
	const double dFpcv = SafeDiv(dVn, dVd, _INFI4);

	// 보유성능
	const double dFpc = min(min(dFpcf, dFpcc), dFpcv);

	// Set result
	EvalD.ModeD.enFalureMode = TYPE4_PLAIN;

	EvalD.bCheck = TRUE;
	EvalD.ModeD.dFpc = dFpc;
	EvalD.EvalD.dFpd = 1.0;

	EvalD.PalinConcD.bPlainConc = true;
	EvalD.PalinConcD.dSm = dSm;
	EvalD.PalinConcD.dMn = dMn;
	EvalD.PalinConcD.dMd = dMd;
	EvalD.PalinConcD.dFpcf = dFpcf;
	EvalD.PalinConcD.dPu = dPu;
	EvalD.PalinConcD.dPn = dfck * dAg;
	EvalD.PalinConcD.dNum = 0.42 * sqrt(dfck);
	EvalD.PalinConcD.dFpcc = dFpcc;
	EvalD.PalinConcD.dAg = dAg;
	EvalD.PalinConcD.dVn = dVn;
	EvalD.PalinConcD.dVd = dVd;
	EvalD.PalinConcD.dFpcv = dFpcv;
	EvalD.PalinConcD.dFpc = dFpc;
}

void CBrdgSeisEvalKALIS2023::Calc_BearingAnchor_Steel(const T_BREV_K& BrevK, T_BRDGEVAL_BEARING& rResD)
{
	const double dmuD = 0.45; // 받침 하부판의 무수축 모르타르 사이의 마찰계수
	const double dD   = 0.88 * rResD.ParamD.ForceD.dPtopBearing; // 고정하중의 수직지진력의 88%
	const double dVsa2 = dmuD * dD;

	const int iTotalBearing = rResD.GetTotalAnchorNum();
	const int iMaxSize = rResD.AnchorParamAll.GetSize();
	for(int i = 0; i < 2; ++i) // Longitudinal, Transverse
	{
		double dVsa1 = 0.0;
		double dVsaParam = 0.0;
		double dAse = 0.0;
		double dnA = 0.0;
		double dfuta = 0.0;
		double dVsa = 0.0;
		double dNumbBP = 0.0;
		double dFBD = rResD.BodyD[i].HorD.dFBD;
		double dnn = 0.0;
		int nB = 0;
		T_BRDGEVAL_BEARING_ANCHOR_FAS& FASD = rResD.AnchorD[i].FASD;
		for(int j = 0; j < iMaxSize; j++)
		{
			T_BRDGEVAL_ANCHOR_PARAM& AnchorParam = rResD.AnchorParamAll[j];

			dVsaParam = (AnchorParam.enAnchType == ANCH_CAST_HEADED_STUD) ? 1.0 : 0.6;
			dAse = AnchorParam.dAse; // 포트받침인 경우 처리

			// 검토 되는 받침(고정단)
			if(i == 0) dnn = AnchorParam.dnBL;
			else        dnn = AnchorParam.dnBT;
			if(dnn == 0.0) continue;

			dnA = AnchorParam.nnL * AnchorParam.nnT;
			dfuta = AnchorParam.dfua;
			dVsa = dnA * dVsaParam * dAse * dfuta;

			dVsa1 += dVsa;
			nB++;
		}

		FASD.dVsa1 = dVsa1;
		FASD.dmuD  = dmuD;
		FASD.dPuD  = rResD.ParamD.ForceD.dPtopBearing;
		FASD.dD    = dD;
		FASD.dVsa2 = dVsa2 * nB / iTotalBearing; // 저항하는 받침만 고려
		FASD.dFASC = max(dVsa1, FASD.dVsa2);
		FASD.dFASD = dFBD;
		FASD.dRat = SafeDiv(FASD.dFASC, FASD.dFASD, _INFI4);
	}
}

double CBrdgSeisEvalKALIS2023::Get_VbFactor()
{
	return 0.66;
}

double CBrdgSeisEvalKALIS2023::Calc_Vbmax(const double& dfck, const double& dca1)
{
	const double dVb = 3.7 * sqrt(dfck) * pow(dca1, 1.5);
	return dVb;
}

double CBrdgSeisEvalKALIS2023::Calc_FACC(const double& dVcbg, const double& dPu)
{
	const double dD = 0.88 * dPu;
	const double dVbrk_ec1 = dVcbg + 0.6 * dD;
	const double dVbrk_ec2 = 1.6*dVcbg;
	const double dVbrk_ec  = min(dVbrk_ec1, dVbrk_ec2);
	return dVbrk_ec;
}