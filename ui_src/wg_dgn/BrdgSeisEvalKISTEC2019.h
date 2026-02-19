#pragma once
#include "brdgseisevalkistec2015.h"

#include "HeaderPre.h"
class __MY_EXT_CLASS__ CBrdgSeisEvalKISTEC2019 : public CBrdgSeisEvalKISTEC2015
{
public:
    CBrdgSeisEvalKISTEC2019(void);
    virtual ~CBrdgSeisEvalKISTEC2019(void);

	virtual double Get_WallVn(const T_BRDGEVAL_SHR_CAPA& ShearCapaD) final;
protected:
    virtual void Get_WallVn(T_BRDGEVAL_CALC_PARAM& ParamD, T_BRDGEVAL_SHR_CAPA& ShearCapaD) final;
    virtual void CalcPlainConcCapacity(const UINT& EvgpK, const UINT& iColm, const EN_LOCAL_DIR& enDir, const EN_POSITION& enPos, const EN_FAILURE_MODE& enFalureMode, const T_BRDGEVAL_CALC_PARAM& ParamD, T_BRDGEVAL_EVALUATION& EvalD);
};

#include "HeaderPost.h"
