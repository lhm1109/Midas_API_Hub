#include "stdafx.h"
#include "DB_ST_DN.h"

#include "UnitCtrl.h"
#include "DBCodeDef.h"

void T_DCON_D::Initialize()
{
	DesignCode.Empty();
	for(int i = 0; i < 5; i++) dPhi[i] = 0.0;

	dRhow=0.;
	dRhoc=0.;    
	dRhor=0.;
	bSpecialSeismic=FALSE;
	nClass=0;		
	dTranLossFactor = 1.0;
	dServLossFactor = 1.0;
	LcomKeyTran = 0;
	StldKeySelf = 0;
	dMrft = 1.0;
	iSlabType = 0;
	dAlpha1 = 0.;
	dAlpha2 = 0.;
	dAlpha2_Colm = 0.;
	iAlphaType = 0;
	iSubCode = 0;
	nPMType = 0;
	dAlphacc = 1.0;
	dConShearStrengthRatio = 0.0;

	dStorColmWeakBeamFactor = 1.3;
	dGamma_rdForBeam = 1.2;
	dGamma_rdForColm = 1.3;
	SecondaryGroupKey = 0;	

	bTorsionDesign = FALSE;
	dTrft = 1.0;

	iFrameType = 0;
	dAua1 = 1.1;
	SplcK = 0;
	dSoilFactor = 1.2;
	dT[0] = 0.15;
	dT[1] = 0.5;
	dT[2] = 2.0;
	dAgR = 0.08;
	dI = 1.0;
	dDampingRat = 5.0;

	//(2009.06.04) Add by Unsang :: 770에서 추가
	dPhi[5] = 0.;
	dMinRhoc=0.0;
	dMaxLimitConc = 0.0;
	dMinLimitConc = 0.0;
	dMaxYieldRbar = 0.0;
	dMinMainRbarD = 0.0;
	dMinSubRbarD  = 0.0; 
	dMaxSubRbarSp = 0.0;
	bBCJointDgn = FALSE;
	nNationalAnnex = dgn::def::enNationalAnnex::Recommended;
	bNTCFlag = TRUE;
	dUf = 0.6;

	dGamma_rdForWall = 1.2;
	dGamma_rdForJoint = 1.1;

	dStrutAngle = 45.0;
	bBehaviorFact = FALSE;
	dBehaviorFactor_q = 1.5;
	dBehaviorFactor_qo = 3.0;

	dA = 0.7;
	dB = 1.1;
	dC = 1.0;
	bAutoC = TRUE;

	nSpecialWall = 0;
	iBndrElemMethod = 0;
	dCd = 4.5;
	dIe = 1.2;

	iExposureFactor = 0;
	iPouringMethod  = 0;

	//aExcludeGroup.RemoveAll();
	//aNotSFSRGroup.RemoveAll();

	bBeam   = TRUE;
	bColumn = TRUE;
	bBrace  = TRUE;
	bWall   = TRUE;
	bSlab   = TRUE;
	bMat    = TRUE;

	bMinRebarBeam   = TRUE;
	bMinRebarColumn = TRUE;
	bMinRebarBrace  = TRUE;
	bMinRebarWall   = TRUE;
	bMinRebarSlab   = TRUE;
	bMinRebarMat    = TRUE;

	bNonSeisSubBeam       = TRUE;
	bNonSeisCantilever    = TRUE;
	bNonSeisUnderGround   = TRUE;

	nSCWBDgnMethod = 0;   // 0: Design Strength,  1: Norminal Strength

	int nMovingType = CProduct::GetMovingType();
	nNTCType = RCS_NTC12;
	if(nMovingType == D_PRODUCT_MOVING_US)
	{
		nNTCType = RCS_NTC18;
		if(CProduct::IsIndia() || CProduct::IsTaiwan()) nNTCType = RCS_NTC12;
	}
	
	bUseVcWall = TRUE;
	bUseVcColm = TRUE;
	bUseVcBeam = TRUE;

	nExposure = 0;
	dRhos=0.0;

	nBridgeType = 1;

	bmb7mb8 = FALSE;
	nmb7mb8Type = 0;
	dmb7 = 0.9;
	dmb8 = 0.9;
	bmb9 = FALSE;
	nmb9Type = 0;
	dmb9 = 0.85;

	bLimitShearStrengthConc = TRUE;
	dFOS		= 3.0;//8.4.0 IS456 SLAB/MATpinakin
	dSubRex	    = 15000.0;//8.4.0 IS456 SLAB/MATpinakin

	dPhi_ef = 2.14;
	bAutoA  = TRUE;
	bChkPM  = FALSE;

	bMinRebarShell   = TRUE;
	dRhominHorzShell = 0.0;  // EC2, RC 휨부재의 최소철근비로 우선 설정.
	dRhominVertShell = 0.0;  // EC2, RC 휨부재의 최소철근비로 우선 설정.
	bStrongColmWeakBeam = FALSE;
	bCheckBeamDeflect   = FALSE;

	NonDissipativeGroupKey = 0;

	bIS3370Sel			 = FALSE;
	bIS3370AnnexA		 = TRUE;
	dIS3370AnnexA_ecs	 = 0.0025;	
	dIS3370AnnexA_etc	 = 0.0025;
	dIS3370AnnexA_t1	 = 30.;
	iIS3370AnnexB_cw	 = 1;

	bConfinedJoint = TRUE;
	bNotConfinedJoint = TRUE;

	nPMCurveMethod = 1;

	nNDDesignMethod = ND_MC;
	dApproximateAlpha = 0.9;

	bShearWallAlphaMax = TRUE;
	bVedPrimaryMemb = FALSE;
	nWallDgnEnvelop = EN_ENVELOP_METHOD_MAX;

	bT1User = FALSE;
	dT1_x = 0.1;
	dT1_y = 0.1;
	nCheckPos = EN_SELECT_CHECK_POS_BOT;
	bUseUGLcom4UGMemb = TRUE;
    nStressStrainType = 0;

    dAggregateSize = 0.016;
    enPL = dgn::def::enProtectionLevelEU::PL1;
    enSL = dgn::def::enServiceLifeEU::L100;
    enSType = dgn::def::enStructTypeEU::Road;
    dkc = 1.0;
    dAlphacc4Shear = 0.0;

    bNotConsiderk1 = FALSE;
    dSCWB_Phic = 0.65;
    bConsiderInteractShear = FALSE; // TWN-USD112 & ACI318-19

#ifdef _ORG
	nBeamRebarConsiderOpt = EN_MCMB_EQUI;
#else
	nBeamRebarConsiderOpt = EN_MCMB_EACH;
#endif // _ORG
	bConsiderBeamElemForce = FALSE;

	dQ = 3.0;
	dR = 1.0;
	bConsiderReduction = FALSE;

	nColumnDgn = 0;
	nBeamDgn = 0;

	nFsMethod4Beam = EN_FSMETHOD_2_3_FY;

	bConsiderPhi = TRUE;
	bStaticallyDeterminable = TRUE;
	bCreep = TRUE;
	bCalculationSeconGroup = TRUE;
	bCosiderSP14_1330_2018 = TRUE;
	bIncludeSeismics = TRUE;
	nIntensityDegree = 0;

	bConsiderBEDesign = FALSE;
	nBEEBarDesignMethod = 1;

	bNonlinearCalculation = FALSE;

	nBeamEndLocRC = 1;
	nColmEndLocRC = 1;
	nBeamEndLocRC_ST = 0;
	nColmEndLocRC_ST = 0;
}

BOOL T_DCON_D::IsSpecialShearWall()
{
	if ( DesignCode == CONCODE_NSR_10 || DesignCode == CONCODE_NTC_DCEC_2017)
	{
		return (nSpecialWall == 0 || nSpecialWall == 1);
	}
	else if (DesignCode == CONCODE_TWN_USD112)
	{
		return nSpecialWall;
	}
	else
	{
		if ( nClass == EN_FRAMETYPE_ORDINARY ) return FALSE;
		return nSpecialWall;
	}
}

void T_CPFP_TYPE::Initialize()
{
	nDetailCategory = 0; 
	dADTT = 0.0;
	dNum = 0.0;
	nWarpType = 0;
	dTopFlange = 0.0;
	dBotFlange = 0.0;
	nGirderType  = 0;
	dWeight      = M_InitValueCurUnit(625000.0, N, M, D_UNITSYS_BASE_FORCE);
	dDesignLife  = 75.0;
	nADTT = 0;
	dNd = 1.0;
	dp  = 0.85;
	bAutoChk = TRUE;

	dADTT_Pr=0.0;
	dNL = 0.0;   // Number of lane
	bTopF_Top = TRUE;
	bTopF_Bot = TRUE;
	bBotF_Top = TRUE;
	bBotF_Bot = TRUE;
	nDetailCategory2 = 0; // 0=A, 1:B, 2:B', 3:C 4:C' 5:D, 6:E, 7:E'
	nDetailCategory3 = 0; // 0=A, 1:B, 2:B', 3:C 4:C' 5:D, 6:E, 7:E'
	nDetailCategory4 = 0; // 0=A, 1:B, 2:B', 3:C 4:C' 5:D, 6:E, 7:E'
	dTopFlangeBot = 0.0;
	dBotFlangeTop = 0.0;
}

void T_CPFP_TYPE::Convert900(T_CPFP_TYPE_900& rData)
{
	nDetailCategory = rData.nDetailCategory;
	dADTT = rData.dADTT;
	dNum = rData.dNum;
	nWarpType = rData.nWarpType;
	dTopFlange = rData.dTopFlange;
	dBotFlange = rData.dBotFlange;

	nGirderType = rData.nGirderType;
	dWeight = rData.dWeight;
	dDesignLife = rData.dDesignLife;
	nADTT       = rData.nADTT;
	dNd         = rData.dNd;
	dp          = rData.dp;
	bAutoChk    = rData.bAutoChk;

	dADTT_Pr=0.0;
	dNL = 0.0;   // Number of lane
	bTopF_Top = TRUE;
	bTopF_Bot = TRUE;
	bBotF_Top = TRUE;
	bBotF_Bot = TRUE;
	nDetailCategory2 = 0; // 0=A, 1:B, 2:B', 3:C 4:C' 5:D, 6:E, 7:E'
	nDetailCategory3 = 0; // 0=A, 1:B, 2:B', 3:C 4:C' 5:D, 6:E, 7:E'
	nDetailCategory4 = 0; // 0=A, 1:B, 2:B', 3:C 4:C' 5:D, 6:E, 7:E'
	dTopFlangeBot = 0.0;
	dBotFlangeTop = 0.0;
}

void T_CPFP_TYPE::Convert860_0(T_CPFP_TYPE_860_0& rData)
{
	nDetailCategory = rData.nDetailCategory;
	dADTT = rData.dADTT;
	dNum = rData.dNum;
	nWarpType = 0;
	dTopFlange = 0.0;
	dBotFlange = 0.0;

	nGirderType = 0;
	dWeight     = M_InitValueCurUnit(625000.0, N, M, D_UNITSYS_BASE_FORCE);
	dDesignLife = 75.0;
	nADTT       = 0;
	dNd         = 1.0;
	dp          = 0.85;
	bAutoChk    = TRUE;

	dADTT_Pr=0.0;
	dNL = 0.0;   // Number of lane
	bTopF_Top = TRUE;
	bTopF_Bot = TRUE;
	bBotF_Top = TRUE;
	bBotF_Bot = TRUE;
	nDetailCategory2 = 0; // 0=A, 1:B, 2:B', 3:C 4:C' 5:D, 6:E, 7:E'
	nDetailCategory3 = 0; // 0=A, 1:B, 2:B', 3:C 4:C' 5:D, 6:E, 7:E'
	nDetailCategory4 = 0; // 0=A, 1:B, 2:B', 3:C 4:C' 5:D, 6:E, 7:E'
	dTopFlangeBot = 0.0;
	dBotFlangeTop = 0.0;

}

void T_CPFP_TYPE::Convert856_0(T_CPFP_TYPE_856_0& rData)
{
	nDetailCategory = rData.nDetailCategory;
	dADTT = rData.dADTT;
	dNum = rData.dNum;
	nWarpType = 0;
	dTopFlange = 0.0;
	dBotFlange = 0.0;

	nGirderType = 0;
	dWeight     = M_InitValueCurUnit(625000.0, N, M, D_UNITSYS_BASE_FORCE);
	dDesignLife = 75.0;
	nADTT       = 0;
	dNd         = 1.0;
	dp          = 0.85;
	bAutoChk    = TRUE;

	dADTT_Pr=0.0;
	dNL = 0.0;   // Number of lane
	bTopF_Top = TRUE;
	bTopF_Bot = TRUE;
	bBotF_Top = TRUE;
	bBotF_Bot = TRUE;
	nDetailCategory2 = 0; // 0=A, 1:B, 2:B', 3:C 4:C' 5:D, 6:E, 7:E'
	nDetailCategory3 = 0; // 0=A, 1:B, 2:B', 3:C 4:C' 5:D, 6:E, 7:E'
	nDetailCategory4 = 0; // 0=A, 1:B, 2:B', 3:C 4:C' 5:D, 6:E, 7:E'
	dTopFlangeBot = 0.0;
	dBotFlangeTop = 0.0;
}

void T_CPFP_TYPE::Convert835_0(T_CPFP_TYPE_835_0& rData)
{
	nDetailCategory = rData.nDetailCategory;
	dADTT = rData.dADTT;
	dNum = rData.dNum;
	nWarpType = 0;
	dTopFlange = 0.0;
	dBotFlange = 0.0;

	nGirderType = 0;
	dWeight     = M_InitValueCurUnit(625000.0, N, M, D_UNITSYS_BASE_FORCE);
	dDesignLife = 75.0;
	nADTT       = 0;
	dNd         = 1.0;
	dp          = 0.85;
	bAutoChk    = TRUE;

	dADTT_Pr=0.0;
	dNL = 0.0;   // Number of lane
	bTopF_Top = TRUE;
	bTopF_Bot = TRUE;
	bBotF_Top = TRUE;
	bBotF_Bot = TRUE;
	nDetailCategory2 = 0; // 0=A, 1:B, 2:B', 3:C 4:C' 5:D, 6:E, 7:E'
	nDetailCategory3 = 0; // 0=A, 1:B, 2:B', 3:C 4:C' 5:D, 6:E, 7:E'
	nDetailCategory4 = 0; // 0=A, 1:B, 2:B', 3:C 4:C' 5:D, 6:E, 7:E'
	dTopFlangeBot = 0.0;
	dBotFlangeTop = 0.0;
}
