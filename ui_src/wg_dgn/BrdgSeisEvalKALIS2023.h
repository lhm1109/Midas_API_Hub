#pragma once
#include "brdgseisevalkistec2019.h"

#include "HeaderPre.h"
class __MY_EXT_CLASS__ CBrdgSeisEvalKALIS2023 : public CBrdgSeisEvalKISTEC2019
{
public:
	CBrdgSeisEvalKALIS2023(void);
	virtual ~CBrdgSeisEvalKALIS2023(void);

protected:
	virtual void   CalcPlainConcCapacity(const UINT& EvgpK, const UINT& iColm, const EN_LOCAL_DIR& enDir, const EN_POSITION& enPos, const EN_FAILURE_MODE& enFalureMode, const T_BRDGEVAL_CALC_PARAM& ParamD, T_BRDGEVAL_EVALUATION& EvalD);
	virtual void   Calc_BearingAnchor_Steel(const T_BREV_K& BrevK, T_BRDGEVAL_BEARING& rResD);
	virtual double Get_VbFactor();
	virtual double Calc_Vbmax(const double& dfck, const double& dca1);
	virtual double Calc_FACC(const double& dVcbg, const double& dPu);
	virtual bool   IsCode4PryoutRebar() { return true; }

	virtual double Get_Vsamax(T_BRDGEVAL_BEARING& rResD);
	virtual double Get_psi_cV(const T_ANEV_D& AnevD);
	virtual double Get_psi_cN(const EN_ANCH_TYPE& enAnchType);
};

#include "HeaderPost.h"