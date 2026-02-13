#include "stdafx.h"
#include "BrdgSeisEvalKISTEC2019.h"

#include "..\wg_db\MathFunc.h"

CBrdgSeisEvalKISTEC2019::CBrdgSeisEvalKISTEC2019(void)
{
    m_CodeUnit.nBase_Length = D_UNITSYS_LENGTH_INDEX_MM;
    m_CodeUnit.nBase_Force  = D_UNITSYS_FORCE_INDEX_N;
}


CBrdgSeisEvalKISTEC2019::~CBrdgSeisEvalKISTEC2019(void)
{
}

void CBrdgSeisEvalKISTEC2019::Get_WallVn(T_BRDGEVAL_CALC_PARAM& ParamD, T_BRDGEVAL_SHR_CAPA& ShearCapaD)
{
    // 벽식교각의 강축방향 - 교축직각 인 경우만 들어온다.
    double dfck  = ParamD.MatlD.dfck_S;
	double dfctk = ParamD.MatlD.dfctk_S;
    double dfyh  = ParamD.MatlD.dfyh;

	// 교직으로 검토하므로 입력된B, H가 바뀐다.
    double dh    = ParamD.SetcD.dB;
    double db    = ParamD.SetcD.dH;
    double ddc   = ParamD.BarD.ddc;
    double dAv   = ParamD.BarD.dAv;
    double drho_sh = ParamD.BarD.drhosh;
    double ds    = ParamD.BarD.ds;
	double dAxialForce_S = ParamD.MatlD.dAxialForce_S;

    double dD1  = dh - ddc;
    
	double dVn1 = 0.0;
    double dVn2 = 0.0;
	double dVn1_s = 0.0;
	double dVn2_s = 0.0;
	double dVn1_c = 0.0;
	double dVn2_c = 0.0;

    double dphi_c = 1.0;
    double dphi_s = 1.0;    
	double drho_slt = ParamD.BarD.drho;
    double dfn    = 0.0;
    double dTheta = 45.0;
    double dv     = 0.0;
    double dk     = 0.0;
    //if(ds > 0.0) // 전단배근이 있는 경우
    {
        double dphi_s = 1.0;
        double dz     = 0.9*dD1;
        double dtan   = tan(dTheta * CMathFunc::m_trrad);
        double dcot   = 1.0 / dtan;
        dVn1_s = dphi_s * dfyh * dAv * dz * dcot / ds;

        dv   = 0.6 * ( 1.0 - dfck/250.0 );
        dVn2_s = dv * dphi_c * dfck * db * dz / ( dcot + dtan );
        
    }
    //else
    {
        dk = 1.0 + sqrt(200.0 / dD1);
        dk = min(dk, 2.0);

        //drho_slt = (ParamD.BarD.dAst/2.0) / (db*dD1);

//         double dfcm  = 0.0;
//         if(dfck < 40.0) dfcm = dfck + 4.0;
//         else if(dfck < 60.0) dfcm = dfck + 6.0;
//         else
//         {
//             dfcm = dfck + (6.0-4.0)/(60.0-40.0) * (dfck-40.0);
//         }
//         double dfctm = 0.30 * pow(dfcm, 2.0/3.0);
//         dfctk = 0.7 * dfctm;

        dfn = fabs(dAxialForce_S/*ParamD.ForceD.dPtop*/) / ParamD.SetcD.dArea;
        dfn = min(dfn, 0.2*dphi_c*dfck);

        dVn1_c = ( 0.85 * dphi_c * dk * pow(( drho_slt * dfck ), (1.0/3.0)) + 0.15*dfn ) * db * dD1;
        dVn2_c = ( 0.4 * dphi_c * dfctk + 0.15*dfn ) * db * dD1;
    }

	int nWallShearType = 0;
	if(min(dVn1_s, dVn2_s) >= max(dVn1_c, dVn2_c))
	{
		nWallShearType = 0;
		dVn1 = dVn1_s;
		dVn2 = dVn2_s;
	}
	else 
	{
		nWallShearType = 1; // 콘크리트
		dVn1 = dVn1_c;
		dVn2 = dVn2_c;
	}


    // Set Result
    ShearCapaD.enPierType = WALL;
    ShearCapaD.dVn1 = dVn1;
    ShearCapaD.dVn2 = dVn2;
    ShearCapaD.dD1  = dD1;
    ShearCapaD.dD2  = ds;
    ShearCapaD.drho_slt = drho_slt;
    ShearCapaD.dfctk= dfctk;
    ShearCapaD.dfn  = dfn;
    ShearCapaD.dtheta = dTheta;
    ShearCapaD.dv     = dv;
    ShearCapaD.dk     = dk;
	ShearCapaD.nWallShearType = nWallShearType;
}

double CBrdgSeisEvalKISTEC2019::Get_WallVn(const T_BRDGEVAL_SHR_CAPA& ShearCapaD)
{
    double dVn = 0.0;
    if(ShearCapaD.nWallShearType==0/*ShearCapaD.dD2 > 0.0*/)    // 전단배근이 있는 경우
    {
        dVn = min(ShearCapaD.dVn1, ShearCapaD.dVn2);
    }
    else // 전단배근이 없는 경우
    {
        dVn = max(ShearCapaD.dVn1, ShearCapaD.dVn2);
    }

    return dVn;
}

void CBrdgSeisEvalKISTEC2019::CalcPlainConcCapacity(const UINT& EvgpK, const UINT& iColm, const EN_LOCAL_DIR& enDir, const EN_POSITION& enPos, const EN_FAILURE_MODE& enFalureMode, const T_BRDGEVAL_CALC_PARAM& ParamD, T_BRDGEVAL_EVALUATION& EvalD)
{
	EvalD.initialize();
	ASSERT(0);
}