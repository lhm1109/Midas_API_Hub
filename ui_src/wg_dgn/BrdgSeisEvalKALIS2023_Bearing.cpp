#include "stdafx.h"
#include "BrdgSeisEvalKALIS2023.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Bearing
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

double CBrdgSeisEvalKALIS2023::Get_Vsamax(T_BRDGEVAL_BEARING& rResD)
{
	if(LE0(rResD.ParamD.ForceD.dPtop)) return DBL_MAX;
	const double dmuD = 0.45; // 받침 하부판과 무수축 모트라트 사이의 마찰계수
	const double dD = 0.88 * rResD.ParamD.ForceD.dPtop; // 받침에 작용하는 유효압축력, 고정하중에 의한 수직력의 88%
	const double dVsamax = dmuD * dD; // 받침 하부판과 무수축 모트라트 사이의 마찰력
	return dVsamax;
}

double CBrdgSeisEvalKALIS2023::Get_psi_cV(const T_ANEV_D& AnevD)
{
	if(AnevD.bShearFactor)
	{
		switch(AnevD.nShearFactorType)
		{
			case 0: return 1.0; // 보조철근이 없거나 D13 미만의 가장자리 보강근이 배치된 균열 콘크리트에 설치된 앵커
			case 1: return 1.2; // 앵커와 가장자리 사이에 D13 이상의 보조철근이 있는 균열 콘크리트에 설치된 앵커
			case 2: return 1.4; // 균열이 발생하지 않는 콘크리트에 설치된 앵커, 또는 앵커와 가장자리 사이에 D13 이상의 보보철근이 있고,
				                // 이 보조철근이 100mm 이하 간격의 스터럽으로 둘러싸인 균열 콘크리트에 설치된 앵커
			default:
			break;
		}
	}
	return 1.0;// 사용자 입력
}

double CBrdgSeisEvalKALIS2023::Get_psi_cN(const EN_ANCH_TYPE& enAnchType)
{
	switch(enAnchType)
	{
		case ANCH_CAST_HEADED_STUD:
		case ANCH_CAST_HEADED_BOLT:
		case ANCH_CAST_HOOKEDBOLTL:
		case ANCH_CAST_HOOKEDBOLTJ:
			// 선설치 앵커
			return 1.25;
		case ANCH_POST_EXPANSIONTC:
		case ANCH_POST_UNDERCUTANC:
		case ANCH_POST_EXPANSIONDC:
		case ANCH_POST_ADHESIVEANC:
			// 후설치 앵커
			return 1.40;
		default:
		ASSERT(0);
		break;
	}
	
	return 1.0;
}
