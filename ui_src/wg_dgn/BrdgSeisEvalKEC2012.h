#pragma once
#include "brdgseisevalkistec2015.h"

#include "HeaderPre.h"
class __MY_EXT_CLASS__ CBrdgSeisEvalKEC2012 : public CBrdgSeisEvalKISTEC2015
{
public:
    CBrdgSeisEvalKEC2012(void);
    virtual ~CBrdgSeisEvalKEC2012(void);

protected:
    void   Calc_BendingCapacity(const SEIS_RES_KEY& SeisResK, const T_BRDGEVAL_CALC_PARAM& ParamD, T_BRDGEVAL_ULT_DISP_DUCT& UltDsipD, T_BRDGEVAL_BEND_CAPA& BendCapaD);
    double Get_MaxSupplyDispDuct(const int& nSplice, const CString& strShape, const EN_LOCAL_DIR& enDir, const int& nBHmethod, const double& dArea, const double& dB, const double& dH, const double& dlsp);

};
#include "HeaderPost.h"
