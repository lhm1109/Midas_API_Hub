#include "stdafx.h"
#include "BrdgSeisEvalKEC2012.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\BrdgSeisEvalResult.h"
#include "..\wg_db\wg_db_PostCtrl.h"


CBrdgSeisEvalKEC2012::CBrdgSeisEvalKEC2012(void)
{
    m_CodeUnit.nBase_Length = D_UNITSYS_LENGTH_INDEX_MM;
    m_CodeUnit.nBase_Force  = D_UNITSYS_FORCE_INDEX_N;
}


CBrdgSeisEvalKEC2012::~CBrdgSeisEvalKEC2012(void)
{
}

void CBrdgSeisEvalKEC2012::Calc_BendingCapacity(const SEIS_RES_KEY& SeisResK, const T_BRDGEVAL_CALC_PARAM& ParamD, T_BRDGEVAL_ULT_DISP_DUCT& UltDsipD, T_BRDGEVAL_BEND_CAPA& BendCapaD)
{
    // Unit; kN, m
    SEIS_RES_K Key;
    Key.KeyMap = SeisResK;

    // 4.4.1.2 교각의 휨성능 곡선
    double dHe  = ParamD.OptD.dHe;
    double dD   = 0.0;
    if(ParamD.SetcD.strShape == D_SECT_SHAPE_REG_P || ParamD.SetcD.strShape == D_SECT_SHAPE_REG_SR)
        dD = ParamD.SetcD.dH;
    else
        dD = Key.Key.enDir==LONGI ? ParamD.SetcD.dH : ParamD.SetcD.dB;

    CBrdgSeisEvalResult *pResult = m_pDoc->m_pPostCtrl->GetBrdgSeisEvalResult(); if(!pResult) { ASSERT(0); return; }
    T_BRDGEVAL_MPHI_STATE MPhiStateD;
    if(!pResult->GetMPhiState(SeisResK, MPhiStateD)) return;

    double dMy = MPhiStateD.dMoment[4];
    double dMu = MPhiStateD.dMoment[5];
    double dphiy = MPhiStateD.dCurvature[4];
    double dphiu = MPhiStateD.dCurvature[5];

    double dFy = dMy / dHe; // 식(4.4.1a)
    double dFu = dMu / dHe; // 식(4.4.1b)

    double dmu_phi   = dphiu / dphiy;
    double dmu_delta = 0.35 + 0.375*(dD/dHe) + 0.2*(1.1+(dD/dHe))*dmu_phi;
    double dDeltay = dphiy * pow(dHe, 2.0) / 3.0; // 식(4.4.2a)
    double dDeltau = dDeltay * dmu_delta;

    double dmuDeltac = dDeltau / dDeltay;
    double dmuDeltamax = Get_MaxSupplyDispDuct(ParamD.OptD.nSplice, ParamD.SetcD.strShape, Key.Key.enDir, ParamD.OptD.nBHmethod, ParamD.SetcD.dArea, ParamD.SetcD.dB, ParamD.SetcD.dH, ParamD.OptD.dlsp);
    if(dmuDeltac > dmuDeltamax)
    {
        BendCapaD.dDeltac = dmuDeltamax * dDeltay;
        BendCapaD.dFn     = dFy + (dFu-dFy)/(dDeltau-dDeltay)*(BendCapaD.dDeltac-dDeltay);
    }

    BendCapaD.dFy = dFy;
    BendCapaD.dFu = dFu;
    BendCapaD.dDeltay = dDeltay;
    BendCapaD.dDeltau = dDeltau;

    BendCapaD.dmu_phi   = dmu_phi;
    BendCapaD.dmu_delta = dmu_delta;

    UltDsipD.dDeltay = dDeltay;
    UltDsipD.dDeltau = dDeltau;
    UltDsipD.dDeltac = BendCapaD.dDeltac;
    UltDsipD.dmu_Deltac   = dmuDeltac;
    UltDsipD.dmu_DeltaMax = dmuDeltamax;
    UltDsipD.dFn          = BendCapaD.dFn;
}

////////////////////////////
// 최대공급변위연성도 μΔ,max
double CBrdgSeisEvalKEC2012::Get_MaxSupplyDispDuct(const int& nSplice, const CString& strShape, const EN_LOCAL_DIR& enDir, const int& nBHmethod, const double& dArea, const double& dB, const double& dH, const double& dlsp)
{
    const double dZero = 1.0E-7;
    double dmuDeltaMax = DBL_MAX;
    if(nSplice == 0 || nSplice == 1)  return dmuDeltaMax; // 단일철근 or 50% 이음
    else if(nSplice == 2) // 100% 이음
    {
        if(strShape == D_SECT_SHAPE_REG_P || strShape == D_SECT_SHAPE_REG_SR)
            dmuDeltaMax = 1.80;
        else
            dmuDeltaMax = 1.50;
    }
    else ASSERT(0);

    return dmuDeltaMax;
}
