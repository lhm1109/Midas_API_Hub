#include "stdafx.h"
#include "SeisEvalCalcStruct.h"

#include "..\wg_db\SeisEvalStruct.h"


void T_VFRAME::Initialize()
{
    ElemK = 0;
    dVny = 0.0;
    dVnz = 0.0;
}

T_VFRAME::T_VFRAME()
{
    Initialize();
}

T_VFRAME::T_VFRAME(T_ELEM_K Key, double dVy, double dVz)
{
    ElemK = Key;
    dVny  = dVy;
    dVnz  = dVz;
}

T_CON_SHR_PARAM::T_CON_SHR_PARAM()
{
    Initialize();
}

void T_CON_SHR_PARAM::Initialize()
{
    dFck = 0.0;
    dFys = 0.0;
    dLambda = 0.0;
    dBv  = 0.0;
    dDv  = 0.0;
    dAsv = 0.0;
    dSv  = 0.0;
}

double T_CON_SHR_PARAM::CalcRhoIndex() const
{
    return fabs(dBv*dSv) < cEvalZero ? 0.0 : dAsv/(dBv*dSv);
}

bool T_CON_SHR_PARAM::IsSpaceGT05Dv() const
{
    return dSv > dDv/2.0 ? true : false;
}

void T_BEAM_SHR_PARAM::Initialize()
{
    ParamPos.Initialize();
    ParamNeg.Initialize();
	nShrMethod = D_SEME_SHRSTR_AIK_2021;
}

T_CON_SHR_PARAM T_BEAM_SHR_PARAM::GetParam(BOOL bConsiderM, double dMu/*=0.0*/) const /// 보의 전단유효깊이는 대상하중조합의 모멘트 부호에 따라 결정 (same to 설계 @선형해석평가)
{
    if (bConsiderM)
    {
        return dMu < 0.0 ? ParamNeg : ParamPos;
    }

    T_CON_SHR_PARAM Shr = ParamPos;
    Shr.dDv = min(ParamPos.dDv, ParamNeg.dDv); /// 1단계, 2단계에서는 모멘트 부호를 고려하지 않았음.
    return Shr;
}

void T_COLM_SHR_PARAM::Initialize()
{
    Comm.Initialize();
    nShrMethod = D_SEME_SHRSTR_MOE2018;
    bAutoK1 = FALSE;
    dUserK1 = 0.0;
    dAg = 0.0;
    dP  = 0.0;
    dV  = 0.0;
    dM  = 0.0;
}

T_WALL_SHR_PARAM::T_WALL_SHR_PARAM()
{
    Initialize();
}

void T_WALL_SHR_PARAM::Initialize()
{
    Comm.Initialize();
    bVnmaxEQ = false;
    dTotalHw = 0.0;
    dLw  = 0.0;
    dP   = 0.0;
    dV   = 0.0;   
    dM   = 0.0;
}

void T_WALL_SHR_PARAM::SetForc(double dPu, double dVu, double dMu)
{
    dP = dPu;
    dV = dVu;
    dM = dMu;
}

void T_MEMBMFACT_TABLE_2::Initialize()
{
    dA1 = 0.0;
    dA2 = 0.0;
    dB1 = 0.0;
    dB2 = 0.0;

    for ( int i=0; i<3; ++i )
    {
        adTable_A1_B1[i] = 0.0;
        adTable_A1_B2[i] = 0.0;
        adTable_A2_B1[i] = 0.0;
        adTable_A2_B2[i] = 0.0;
    }    
}

void T_MEMBMFACT_TABLE_3::Initialize()
{
    dA1 = 0.0;
    dA2 = 0.0;
    dB1 = 0.0;
    dB2 = 0.0;
    dC1 = 0.0;
    dC2 = 0.0;

    for ( int i=0; i<3; ++i )
    {
        adTable_A1_B1_C1[i] = 0.0;
        adTable_A1_B1_C2[i] = 0.0;
        adTable_A1_B2_C1[i] = 0.0;
        adTable_A1_B2_C2[i] = 0.0;
        adTable_A2_B1_C1[i] = 0.0;
        adTable_A2_B1_C2[i] = 0.0;
        adTable_A2_B2_C1[i] = 0.0;
        adTable_A2_B2_C2[i] = 0.0;
    }   
}

void T_MEMBPARAM_TABLE_2::Initialize()
{
    dA1 = 0.0;
    dA2 = 0.0;
    dB1 = 0.0;
    dB2 = 0.0;

    for ( int i=0; i<6; ++i )
    {
        adTable_A1_B1[i] = 0.0;
        adTable_A1_B2[i] = 0.0;
        adTable_A2_B1[i] = 0.0;
        adTable_A2_B2[i] = 0.0;
    }    
}

void T_MEMBPARAM_TABLE_3::Initialize()
{
    dA1 = 0.0;
    dA2 = 0.0;
    dB1 = 0.0;
    dB2 = 0.0;
    dC1 = 0.0;
    dC2 = 0.0;

    for ( int i=0; i<6; ++i )
    {
        adTable_A1_B1_C1[i] = 0.0;
        adTable_A1_B1_C2[i] = 0.0;
        adTable_A1_B2_C1[i] = 0.0;
        adTable_A1_B2_C2[i] = 0.0;
        adTable_A2_B1_C1[i] = 0.0;
        adTable_A2_B1_C2[i] = 0.0;
        adTable_A2_B2_C1[i] = 0.0;
        adTable_A2_B2_C2[i] = 0.0;
    }   
}
