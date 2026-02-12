// Tendon Property

#ifndef __DB_ST_DT_TDNT_H__
#define __DB_ST_DT_TDNT_H__

#define MIDAS_SDK

#include "DB_ST_DT_TDMF.h"
#include "DB_ST_DT_MATL.h"

#pragma pack(push, 1)

#define T_TDNT_K        unsigned int
#define HASHSIZETDNT    101

enum EN_TDNT_RELAX
{
	TDNT_RELAX_MAGURA                 = 0,	// Magura
	TDNT_RELAX_CEB_FIP                ,	    // CEB-FIP
	TDNT_RELAX_JTG04                  ,	    // JTG04
	TDNT_RELAX_TB05                   ,	    // TB05
	TDNT_RELAX_INDIA_IRC18_2000       ,	    // INDIA(IRC18-2000)
	TDNT_RELAX_EUROPEAN               ,	    // European
	TDNT_RELAX_KSCE_LSD15             ,	    // KSCE-LSD15
	TDNT_RELAX_INDIA_IRC112_2011      ,	    // INDIA(IRC:112-2011)
	TDNT_RELAX_CEB_FIP_1990           ,	    // Not yet, CEB-FIP 1990
	TDNT_RELAX_CEB_FIP_2010           ,	    // CEB-FIP 2010
	TDNT_RELAX_TB10092_17             ,	    // TB10092-17
	TDNT_RELAX_AS_1500_5_2017         ,	    // AS 5100.5-2017
	TDNT_RELAX_QCR9300_18			  ,		// Q/CR 9300_18
	TDNT_RELAX_JTG023_85              ,	    // JTG023-85
	TDNT_RELAX_ABNT_NBR_6118_2023     ,     // ABNT NBR 6118:2023
	TDNT_RELAX_USER_DEFINED           =100,	// User Defined
};

struct T_TDNT_LTSL_965
{
	int       nLongTermLossType; // Not Used
	int       nStrandType;
	double    dEstForce;         // Not Used
	double    dConcAge;
	double    dConcElasticity;
	double    dRelativeHumidity;
	double    dVSRatio;
	double    dAvgPreComp;
	double    dUltCreepCoef;
	double    dTensFactor;
	double    dShrinkageConst;
	BOOL      bAuto;
};

struct T_TDNT_LTSL
{
	int       nLongTermLossType; // Not Used
	int       nStrandType;
	double    dEstForce;         // Not Used
	double    dConcAge;
	double    dConcElasticity;
	double    dRelativeHumidity;
	double    dVSRatio;
	double    dAvgPreComp;
	double    dUltCreepCoef;
	double    dTensFactor;
	double    dShrinkageConst;
	BOOL      bAuto;
	double    dRelaxConst;    // v970
	double    dRelaxReduct;   // v970

	void Initialize()
	{
		nLongTermLossType = 0;
		nStrandType = 0;
		dEstForce = 0.0;
		dConcAge = 15.0;
		dConcElasticity = 0.0;
		dRelativeHumidity = 0.0;
		dVSRatio = 0.0;
		dAvgPreComp = 0.0;
		dUltCreepCoef = 2.0;
		dTensFactor = 0.5;
		dShrinkageConst = 0.0;
		bAuto = FALSE;

		dRelaxConst = 0.0;
		dRelaxReduct = 0.0;
	}

	void Convert965(T_TDNT_LTSL_965& rData)
	{
		nLongTermLossType = rData.nLongTermLossType;
		nStrandType       = rData.nStrandType      ;
		dEstForce         = rData.dEstForce        ;
		dConcAge          = rData.dConcAge         ;
		dConcElasticity   = rData.dConcElasticity  ;
		dRelativeHumidity = rData.dRelativeHumidity;
		dVSRatio          = rData.dVSRatio         ;
		dAvgPreComp       = rData.dAvgPreComp      ;
		dUltCreepCoef     = rData.dUltCreepCoef    ;
		dTensFactor       = rData.dTensFactor      ;
		dShrinkageConst   = rData.dShrinkageConst  ;
		bAuto             = rData.bAuto            ;
		dRelaxConst       = 0.0;
		dRelaxReduct      = 0.0;
	}
};

struct T_TDNT_D
{
	CString   TendonTypeName;
	int       nTendonType;          // 1:Internal, 2:External
	T_MATL_K  MatlKey;
	double    dTendonArea;
	double    dDuctArea;            // Internal-Pre인 경우는 StrandArea로 사용됨
	double    dRelaxCoef;
	double    dFrictionFactor;
	double    dWobbleFactor;
	double    dUltimateStrength;
	double    dYieldStrength;
	int       nLoadType;            // 0=Pre-tension, 1=Post-tension  
	double    dAnchoSlipBegin;
	double    dAnchoSlipEnd;
	unsigned int nTypeId;           // 순번
	int       nBondType;            // 0: Bonded, 1: Unbonded   // 5.7.0 추가
	double    dAlphaExtCable;       // External 텐던 파괴저항 모멘트 증가계수 (6.4.0 추가)
	int       nRelaxMethod;         // TDNT_RELAX_XXXX, ( Magura,CEB-FIP:640 추가 / JTG04:660추가 / TB05,INDIA(IRC:18-2000):750 추가 / European:760 추가 )
	double    dRelaxRatio;          // (6.4.0 추가)
	BOOL      bOverStressRedFactor; // 6.6.0추가 중국 버전에서만 사용
	double    dJTG04Factor;         // 6.7.0추가  : 1.0 or 0.3
	double    dFpk;                 // 6.7.0추가 
	double    dInadvertentChange;   // 7.1.0추가
	T_TDMF_K  TdmfKey;              // 845 추가
	int       nWobbleType;            // 0: K, 1: k // 8.6.0 추가
	double    dWobbleAngle;           // 8.6.0 추가
	BOOL      bLowRelax;              // 8.8.5 추가
	CString   strDgnCode;
	int		  nStrandDia;
	int		  nStrandNum;
	T_TDNT_LTSL LongTermStressLossData;

	void Initialize()
	{
		TendonTypeName.Empty();
		nTendonType = 1;
		MatlKey = 0;
		dTendonArea = 0.0;
		dDuctArea = 0.0;
		dRelaxCoef = 45.0;
		dFrictionFactor = 0.3;
		dWobbleFactor = 0.0066;
		dUltimateStrength = 0.0;
		dYieldStrength = 0.0;
		nLoadType = 1;          // post    
		dAnchoSlipBegin = 0.0;
		dAnchoSlipEnd = 0.0;
		nTypeId=0;
		nBondType = 0;
		dAlphaExtCable = 0.0;
		#if defined(_CH)
		nRelaxMethod = TDNT_RELAX_JTG04; // M1496-SJB-050624
		#else
		nRelaxMethod = TDNT_RELAX_MAGURA;
		#endif
		dRelaxRatio = 0.0;
		bOverStressRedFactor = FALSE;
		dJTG04Factor = 1.0;
		dFpk = 0.;
		dInadvertentChange=0.0;
		TdmfKey = 0;
		nWobbleType=0;
		dWobbleAngle=0.005; // rad/m;
		bLowRelax = TRUE;
		strDgnCode.Empty();
		nStrandDia = 0; 
		nStrandNum = 0;
		LongTermStressLossData.Initialize();
	}

    bool IsInternalPreTension()
    {
        return (nTendonType == 1 && nLoadType == 0);
    }

    bool IsInternalPostTension()
    {
        return (nTendonType == 1 && nLoadType == 1);
    }

    bool IsExternal()
    {
        return (nTendonType == 2);
    }
};

struct T_TDNT_D_CH_560
{
	char      TendonTypeName[20];
	int       nTendonType;          // 1:Internal, 2:External
	T_MATL_K  MatlKey;
	double    dTendonArea;
	double    dDuctArea;
	double    dRelaxCoef;
	double    dFrictionFactor;
	double    dWobbleFactor;
	double    dUltimateStrength;
	double    dYieldStrength;
	int       nLoadType;  // 0=Prestress, 1=Post-stress  
	double    dAnchoSlipBegin;
	double    dAnchoSlipEnd;
	unsigned int nTypeId;    // 순번
};
struct T_TDNT_D_CH_630
{
	char      TendonTypeName[20];
	int       nTendonType;          // 1:Internal, 2:External
	T_MATL_K  MatlKey;
	double    dTendonArea;
	double    dDuctArea;
	double    dRelaxCoef;
	double    dFrictionFactor;
	double    dWobbleFactor;
	double    dUltimateStrength;
	double    dYieldStrength;
	int       nLoadType;  // 0=Prestress, 1=Post-stress  
	double    dAnchoSlipBegin;
	double    dAnchoSlipEnd;
	unsigned int nTypeId;    // 순번
	int       nBondType;  // 0: Bonded, 1: Unbonded   // 5.7.0 추가
};
struct T_TDNT_D_CH_650
{
	char      TendonTypeName[20];
	int       nTendonType;          // 1:Internal, 2:External
	T_MATL_K  MatlKey;
	double    dTendonArea;
	double    dDuctArea;
	double    dRelaxCoef;
	double    dFrictionFactor;
	double    dWobbleFactor;
	double    dUltimateStrength;
	double    dYieldStrength;
	int       nLoadType;  // 0=Prestress, 1=Post-stress  
	double    dAnchoSlipBegin;
	double    dAnchoSlipEnd;
	unsigned int nTypeId;    // 순번
	int       nBondType;  // 0: Bonded, 1: Unbonded   // 5.7.0 추가
	double    dAlphaExtCable; // (6.4.0 추가)
	int       nRelaxMethod;   // (6.4.0 추가)
	double    dRelaxRatio;    // (6.4.0 추가)
};
struct T_TDNT_D_CH_660
{
	char      TendonTypeName[20];
	int       nTendonType;          // 1:Internal, 2:External
	T_MATL_K  MatlKey;
	double    dTendonArea;
	double    dDuctArea;
	double    dRelaxCoef;
	double    dFrictionFactor;
	double    dWobbleFactor;
	double    dUltimateStrength;
	double    dYieldStrength;
	int       nLoadType;  // 0=Prestress, 1=Post-stress  
	double    dAnchoSlipBegin;
	double    dAnchoSlipEnd;
	unsigned int nTypeId;    // 순번
	int       nBondType;  // 0: Bonded, 1: Unbonded   // 5.7.0 추가
	double    dAlphaExtCable; // (6.4.0 추가)
	int       nRelaxMethod;   // (6.4.0 추가)
	double    dRelaxRatio;    // (6.4.0 추가)
	BOOL      bOverStressRedFactor;  // 6.6.0추가 중국 버전에서만 사용
};
struct T_TDNT_D_CH_702
{
	char      TendonTypeName[20];
	int       nTendonType;          // 1:Internal, 2:External
	T_MATL_K  MatlKey;
	double    dTendonArea;
	double    dDuctArea;
	double    dRelaxCoef;
	double    dFrictionFactor;
	double    dWobbleFactor;
	double    dUltimateStrength;
	double    dYieldStrength;
	int       nLoadType;  // 0=Prestress, 1=Post-stress  
	double    dAnchoSlipBegin;
	double    dAnchoSlipEnd;
	unsigned int nTypeId;    // 순번
	int       nBondType;  // 0: Bonded, 1: Unbonded   // 5.7.0 추가
	double    dAlphaExtCable; // (6.4.0 추가)
	int       nRelaxMethod;   // (6.4.0 추가)
	double    dRelaxRatio;    // (6.4.0 추가)
	BOOL      bOverStressRedFactor;  // 6.6.0추가 중국 버전에서만 사용
	double    dJTG04Factor;   // 6.7.0추가  : 1.0 or 0.3
	double    dFpk;           // 6.7.0추가 
};

struct T_TDNT_D_CH_842
{
	char      TendonTypeName[20];
	int       nTendonType;          // 1:Internal, 2:External
	T_MATL_K  MatlKey;
	double    dTendonArea;
	double    dDuctArea;
	double    dRelaxCoef;
	double    dFrictionFactor;
	double    dWobbleFactor;
	double    dUltimateStrength;
	double    dYieldStrength;
	int       nLoadType;  // 0=Prestress, 1=Post-stress  
	double    dAnchoSlipBegin;
	double    dAnchoSlipEnd;
	unsigned int nTypeId;    // 순번
	int       nBondType;  // 0: Bonded, 1: Unbonded   // 5.7.0 추가
	double    dAlphaExtCable; // (6.4.0 추가)
	int       nRelaxMethod;   // (6.4.0 추가)
	double    dRelaxRatio;    // (6.4.0 추가)
	BOOL      bOverStressRedFactor;  // 6.6.0추가 중국 버전에서만 사용
	double    dJTG04Factor;   // 6.7.0추가  : 1.0 or 0.3
	double    dFpk;           // 6.7.0추가 
	double    dInadvertentChange;  //7.1.0추가
};

struct T_TDNT_D_CH_856
{
	char      TendonTypeName[20];
	int       nTendonType;          // 1:Internal, 2:External
	T_MATL_K  MatlKey;
	double    dTendonArea;
	double    dDuctArea;
	double    dRelaxCoef;
	double    dFrictionFactor;
	double    dWobbleFactor;
	double    dUltimateStrength;
	double    dYieldStrength;
	int       nLoadType;  // 0=Prestress, 1=Post-stress  
	double    dAnchoSlipBegin;
	double    dAnchoSlipEnd;
	unsigned int nTypeId;    // 순번
	int       nBondType;  // 0: Bonded, 1: Unbonded   // 5.7.0 추가
	double    dAlphaExtCable; // (6.4.0 추가)
	int       nRelaxMethod;   // (6.4.0 추가)
	double    dRelaxRatio;    // (6.4.0 추가)
	BOOL      bOverStressRedFactor;  // 6.6.0추가 중국 버전에서만 사용
	double    dJTG04Factor;   // 6.7.0추가  : 1.0 or 0.3
	double    dFpk;           // 6.7.0추가 
	double    dInadvertentChange;  //7.1.0추가
	T_TDMF_K  TdmfKey;
};

struct T_TDNT_D_CH_883
{
	char      TendonTypeName[20];
	int       nTendonType;          // 1:Internal, 2:External
	T_MATL_K  MatlKey;
	double    dTendonArea;
	double    dDuctArea;
	double    dRelaxCoef;
	double    dFrictionFactor;
	double    dWobbleFactor;
	double    dUltimateStrength;
	double    dYieldStrength;
	int       nLoadType;  // 0=Prestress, 1=Post-stress  
	double    dAnchoSlipBegin;
	double    dAnchoSlipEnd;
	unsigned int nTypeId;    // 순번
	int       nBondType;  // 0: Bonded, 1: Unbonded   // 5.7.0 추가
	double    dAlphaExtCable; // (6.4.0 추가)
	int       nRelaxMethod;   // (6.4.0 추가)
	double    dRelaxRatio;    // (6.4.0 추가)
	BOOL      bOverStressRedFactor;  // 6.6.0추가 중국 버전에서만 사용
	double    dJTG04Factor;   // 6.7.0추가  : 1.0 or 0.3
	double    dFpk;           // 6.7.0추가 
	double    dInadvertentChange;  //7.1.0추가
	T_TDMF_K  TdmfKey;
	int       nWobbleType;
	double    dWobbleAngle;
};

struct T_TDNT_D_CH_955
{
	char      TendonTypeName[20];
	int       nTendonType;          // 1:Internal, 2:External
	T_MATL_K  MatlKey;
	double    dTendonArea;
	double    dDuctArea;
	double    dRelaxCoef;
	double    dFrictionFactor;
	double    dWobbleFactor;
	double    dUltimateStrength;
	double    dYieldStrength;
	int       nLoadType;  // 0=Prestress, 1=Post-stress  
	double    dAnchoSlipBegin;
	double    dAnchoSlipEnd;
	unsigned int nTypeId;    // 순번
	int       nBondType;  // 0: Bonded, 1: Unbonded   // 5.7.0 추가
	double    dAlphaExtCable; // (6.4.0 추가)
	int       nRelaxMethod;   // (6.4.0 추가)
	double    dRelaxRatio;    // (6.4.0 추가)
	BOOL      bOverStressRedFactor;  // 6.6.0추가 중국 버전에서만 사용
	double    dJTG04Factor;   // 6.7.0추가  : 1.0 or 0.3
	double    dFpk;           // 6.7.0추가 
	double    dInadvertentChange;  //7.1.0추가
	T_TDMF_K  TdmfKey;
	int       nWobbleType;
	double    dWobbleAngle;
	BOOL      bLowRelax;              // 8.8.5 추가
};

struct T_TDNT_D_CH_965
{
	char      TendonTypeName[20];
	int       nTendonType;          // 1:Internal, 2:External
	T_MATL_K  MatlKey;
	double    dTendonArea;
	double    dDuctArea;
	double    dRelaxCoef;
	double    dFrictionFactor;
	double    dWobbleFactor;
	double    dUltimateStrength;
	double    dYieldStrength;
	int       nLoadType;  // 0=Prestress, 1=Post-stress  
	double    dAnchoSlipBegin;
	double    dAnchoSlipEnd;
	unsigned int nTypeId;    // 순번
	int       nBondType;  // 0: Bonded, 1: Unbonded   // 5.7.0 추가
	double    dAlphaExtCable; // (6.4.0 추가)
	int       nRelaxMethod;   // (6.4.0 추가)
	double    dRelaxRatio;    // (6.4.0 추가)
	BOOL      bOverStressRedFactor;  // 6.6.0추가 중국 버전에서만 사용
	double    dJTG04Factor;   // 6.7.0추가  : 1.0 or 0.3
	double    dFpk;           // 6.7.0추가 
	double    dInadvertentChange;  //7.1.0추가
	T_TDMF_K  TdmfKey;
	int       nWobbleType;
	double    dWobbleAngle;
	BOOL      bLowRelax;              // 8.8.5 추가
	char      strDgnCode[20];
	int       nStrandDia;
	int       nStrandNum;

	T_TDNT_LTSL_965 LongTermStressLossData;
};

struct T_TDNT_D_CH
{
	char      TendonTypeName[20];
	int       nTendonType;          // 1:Internal, 2:External
	T_MATL_K  MatlKey;
	double    dTendonArea;
	double    dDuctArea;
	double    dRelaxCoef;
	double    dFrictionFactor;
	double    dWobbleFactor;
	double    dUltimateStrength;
	double    dYieldStrength;
	int       nLoadType;  // 0=Prestress, 1=Post-stress  
	double    dAnchoSlipBegin;
	double    dAnchoSlipEnd;
	unsigned int nTypeId;    // 순번
	int       nBondType;  // 0: Bonded, 1: Unbonded   // 5.7.0 추가
	double    dAlphaExtCable; // (6.4.0 추가)
	int       nRelaxMethod;   // (6.4.0 추가)
	double    dRelaxRatio;    // (6.4.0 추가)
	BOOL      bOverStressRedFactor;  // 6.6.0추가 중국 버전에서만 사용
	double    dJTG04Factor;   // 6.7.0추가  : 1.0 or 0.3
	double    dFpk;           // 6.7.0추가 
	double    dInadvertentChange;  //7.1.0추가
	T_TDMF_K  TdmfKey;
	int       nWobbleType;
	double    dWobbleAngle;
	BOOL      bLowRelax;              // 8.8.5 추가
	char      strDgnCode[20];
	int       nStrandDia;
	int       nStrandNum;
	T_TDNT_LTSL LongTermStressLossData;

	void ConvertToString(T_TDNT_D& rData)
	{
		ConvertCharStr(TendonTypeName, rData.TendonTypeName, sizeof(TendonTypeName));
		rData.nTendonType = nTendonType;
		rData.MatlKey = MatlKey;
		rData.dTendonArea = dTendonArea;
		rData.dDuctArea = dDuctArea;
		rData.dRelaxCoef = dRelaxCoef;
		rData.dFrictionFactor = dFrictionFactor;
		rData.dWobbleFactor = dWobbleFactor;
		rData.dUltimateStrength = dUltimateStrength;
		rData.dYieldStrength = dYieldStrength;
		rData.nLoadType = nLoadType;    
		rData.dAnchoSlipBegin = dAnchoSlipBegin;
		rData.dAnchoSlipEnd = dAnchoSlipEnd;
		rData.nTypeId=nTypeId;
		rData.nBondType = nBondType;
		rData.dAlphaExtCable = dAlphaExtCable;
		rData.nRelaxMethod   = nRelaxMethod;
		rData.dRelaxRatio    = dRelaxRatio;
		rData.bOverStressRedFactor = bOverStressRedFactor;
		rData.dJTG04Factor = dJTG04Factor;
		rData.dFpk = dFpk;
		rData.dInadvertentChange=dInadvertentChange;
		rData.TdmfKey = TdmfKey;
		rData.nWobbleType=nWobbleType;
		rData.dWobbleAngle=dWobbleAngle;
		rData.bLowRelax = bLowRelax;
		ConvertCharStr(strDgnCode, rData.strDgnCode, sizeof(strDgnCode));
		rData.nStrandDia = nStrandDia;
		rData.nStrandNum = nStrandNum;
		rData.LongTermStressLossData = LongTermStressLossData;
	}
	void ConvertToChar(T_TDNT_D& rData)
	{
		ConvertStrChar(rData.TendonTypeName, TendonTypeName, sizeof(TendonTypeName));
		nTendonType = rData.nTendonType;
		MatlKey = rData.MatlKey;
		dTendonArea = rData.dTendonArea;
		dDuctArea = rData.dDuctArea;
		dRelaxCoef = rData.dRelaxCoef;
		dFrictionFactor = rData.dFrictionFactor;
		dWobbleFactor = rData.dWobbleFactor;
		dUltimateStrength = rData.dUltimateStrength;
		dYieldStrength = rData.dYieldStrength;
		nLoadType = rData.nLoadType;    
		dAnchoSlipBegin = rData.dAnchoSlipBegin;
		dAnchoSlipEnd = rData.dAnchoSlipEnd;
		nTypeId = rData.nTypeId;
		nBondType = rData.nBondType;
		dAlphaExtCable = rData.dAlphaExtCable;
		nRelaxMethod   = rData.nRelaxMethod;
		dRelaxRatio    = rData.dRelaxRatio;
		bOverStressRedFactor = rData.bOverStressRedFactor;
		dJTG04Factor = rData.dJTG04Factor;
		dFpk = rData.dFpk;
		dInadvertentChange=rData.dInadvertentChange;
		TdmfKey = rData.TdmfKey;
		nWobbleType =rData.nWobbleType;
		dWobbleAngle=rData.dWobbleAngle;
		bLowRelax = rData.bLowRelax;
		ConvertStrChar(rData.strDgnCode, strDgnCode, sizeof(strDgnCode));
		nStrandDia = rData.nStrandDia;
		nStrandNum = rData.nStrandNum;
		LongTermStressLossData = rData.LongTermStressLossData;
	}
	void Convert560(T_TDNT_D_CH_560& rData)
	{
		memcpy(TendonTypeName, rData.TendonTypeName, sizeof(TendonTypeName));    
		nTendonType = rData.nTendonType;
		MatlKey = rData.MatlKey;
		dTendonArea = rData.dTendonArea;
		dDuctArea = rData.dDuctArea;
		dRelaxCoef = rData.dRelaxCoef;
		dFrictionFactor = rData.dFrictionFactor;
		dWobbleFactor = rData.dWobbleFactor;
		dUltimateStrength = rData.dUltimateStrength;
		dYieldStrength = rData.dYieldStrength;
		nLoadType = rData.nLoadType;    
		dAnchoSlipBegin = rData.dAnchoSlipBegin;
		dAnchoSlipEnd = rData.dAnchoSlipEnd;
		nTypeId = rData.nTypeId;
		nBondType = 0;    
		dAlphaExtCable = 0.0;
		nRelaxMethod   = 0;
		dRelaxRatio    = 0.0;
		bOverStressRedFactor = FALSE;
		dJTG04Factor = 1.0;
		dFpk = 0.;
		dInadvertentChange=0.0;
		TdmfKey = 0;
		nWobbleType =0;
		dWobbleAngle=0.0;
		bLowRelax = FALSE;
		memset(strDgnCode, 0, sizeof(strDgnCode));
		nStrandDia = 0;
		nStrandNum = 0;
		LongTermStressLossData.Initialize();
	}
	void Convert630(T_TDNT_D_CH_630& rData)
	{
		memcpy(TendonTypeName, rData.TendonTypeName, sizeof(TendonTypeName));
		nTendonType = rData.nTendonType;
		MatlKey = rData.MatlKey;
		dTendonArea = rData.dTendonArea;
		dDuctArea = rData.dDuctArea;
		dRelaxCoef = rData.dRelaxCoef;
		dFrictionFactor = rData.dFrictionFactor;
		dWobbleFactor = rData.dWobbleFactor;
		dUltimateStrength = rData.dUltimateStrength;
		dYieldStrength = rData.dYieldStrength;
		nLoadType = rData.nLoadType;    
		dAnchoSlipBegin = rData.dAnchoSlipBegin;
		dAnchoSlipEnd = rData.dAnchoSlipEnd;
		nTypeId = rData.nTypeId;
		nBondType = rData.nBondType;    
		dAlphaExtCable = 0.0;
		nRelaxMethod   = 0;
		dRelaxRatio    = 0.0;
		bOverStressRedFactor = FALSE;
		dJTG04Factor = 1.0;
		dFpk = 0.;
		dInadvertentChange=0.0;
		TdmfKey = 0;
		nWobbleType =0;
		dWobbleAngle=0.0;
		bLowRelax = FALSE;
		memset(strDgnCode, 0, sizeof(strDgnCode));
		nStrandDia = 0;
		nStrandNum = 0;
		LongTermStressLossData.Initialize();
	}
	void Convert650(T_TDNT_D_CH_650& rData)
	{
		memcpy(TendonTypeName, rData.TendonTypeName, sizeof(TendonTypeName));
		nTendonType = rData.nTendonType;
		MatlKey = rData.MatlKey;
		dTendonArea = rData.dTendonArea;
		dDuctArea = rData.dDuctArea;
		dRelaxCoef = rData.dRelaxCoef;
		dFrictionFactor = rData.dFrictionFactor;
		dWobbleFactor = rData.dWobbleFactor;
		dUltimateStrength = rData.dUltimateStrength;
		dYieldStrength = rData.dYieldStrength;
		nLoadType = rData.nLoadType;    
		dAnchoSlipBegin = rData.dAnchoSlipBegin;
		dAnchoSlipEnd = rData.dAnchoSlipEnd;
		nTypeId = rData.nTypeId;
		nBondType = rData.nBondType;    
		dAlphaExtCable = rData.dAlphaExtCable;
		nRelaxMethod   = rData.nRelaxMethod;
		dRelaxRatio    = rData.dRelaxRatio;
		bOverStressRedFactor = FALSE;
		dJTG04Factor = 1.0;
		dFpk = 0.;
		dInadvertentChange=0.0;
		TdmfKey = 0;
		nWobbleType =0;
		dWobbleAngle=0.0;
		bLowRelax = FALSE;
		memset(strDgnCode, 0, sizeof(strDgnCode));
		nStrandDia = 0;
		nStrandNum = 0;
		LongTermStressLossData.Initialize();
	}
	void Convert660(T_TDNT_D_CH_660& rData)
	{
		memcpy(TendonTypeName, rData.TendonTypeName, sizeof(TendonTypeName));
		nTendonType = rData.nTendonType;
		MatlKey = rData.MatlKey;
		dTendonArea = rData.dTendonArea;
		dDuctArea = rData.dDuctArea;
		dRelaxCoef = rData.dRelaxCoef;
		dFrictionFactor = rData.dFrictionFactor;
		dWobbleFactor = rData.dWobbleFactor;
		dUltimateStrength = rData.dUltimateStrength;
		dYieldStrength = rData.dYieldStrength;
		nLoadType = rData.nLoadType;    
		dAnchoSlipBegin = rData.dAnchoSlipBegin;
		dAnchoSlipEnd = rData.dAnchoSlipEnd;
		nTypeId = rData.nTypeId;
		nBondType = rData.nBondType;    
		dAlphaExtCable = rData.dAlphaExtCable;
		nRelaxMethod   = rData.nRelaxMethod;
		dRelaxRatio    = rData.dRelaxRatio;
		bOverStressRedFactor = rData.bOverStressRedFactor;
		dJTG04Factor = 1.0;
		dFpk = 0.;
		dInadvertentChange=0.0;
		TdmfKey = 0;
		nWobbleType =0;
		dWobbleAngle=0.0;
		bLowRelax = FALSE;
		memset(strDgnCode, 0, sizeof(strDgnCode));
		nStrandDia = 0;
		nStrandNum = 0;
		LongTermStressLossData.Initialize();
	}
	void Convert702(T_TDNT_D_CH_702& rData)
	{
		memcpy(TendonTypeName, rData.TendonTypeName, sizeof(TendonTypeName));
		nTendonType = rData.nTendonType;
		MatlKey = rData.MatlKey;
		dTendonArea = rData.dTendonArea;
		dDuctArea = rData.dDuctArea;
		dRelaxCoef = rData.dRelaxCoef;
		dFrictionFactor = rData.dFrictionFactor;
		dWobbleFactor = rData.dWobbleFactor;
		dUltimateStrength = rData.dUltimateStrength;
		dYieldStrength = rData.dYieldStrength;
		nLoadType = rData.nLoadType;    
		dAnchoSlipBegin = rData.dAnchoSlipBegin;
		dAnchoSlipEnd = rData.dAnchoSlipEnd;
		nTypeId = rData.nTypeId;
		nBondType = rData.nBondType;    
		dAlphaExtCable = rData.dAlphaExtCable;
		nRelaxMethod   = rData.nRelaxMethod;
		dRelaxRatio    = rData.dRelaxRatio;
		bOverStressRedFactor = rData.bOverStressRedFactor;
		dJTG04Factor = rData.dJTG04Factor;
		dFpk = rData.dFpk;
		dInadvertentChange=0.0;
		TdmfKey = 0;
		nWobbleType =0;
		dWobbleAngle=0.0;
		bLowRelax = FALSE;
		memset(strDgnCode, 0, sizeof(strDgnCode));
		nStrandDia = 0;
		nStrandNum = 0;
		LongTermStressLossData.Initialize();
	}
	void Convert842(T_TDNT_D_CH_842& rData)
	{
		memcpy(TendonTypeName, rData.TendonTypeName, sizeof(TendonTypeName));
		nTendonType = rData.nTendonType;
		MatlKey = rData.MatlKey;
		dTendonArea = rData.dTendonArea;
		dDuctArea = rData.dDuctArea;
		dRelaxCoef = rData.dRelaxCoef;
		dFrictionFactor = rData.dFrictionFactor;
		dWobbleFactor = rData.dWobbleFactor;
		dUltimateStrength = rData.dUltimateStrength;
		dYieldStrength = rData.dYieldStrength;
		nLoadType = rData.nLoadType;    
		dAnchoSlipBegin = rData.dAnchoSlipBegin;
		dAnchoSlipEnd = rData.dAnchoSlipEnd;
		nTypeId = rData.nTypeId;
		nBondType = rData.nBondType;    
		dAlphaExtCable = rData.dAlphaExtCable;
		nRelaxMethod   = rData.nRelaxMethod;
		dRelaxRatio    = rData.dRelaxRatio;
		bOverStressRedFactor = rData.bOverStressRedFactor;
		dJTG04Factor = rData.dJTG04Factor;
		dFpk = rData.dFpk;
		dInadvertentChange=rData.dInadvertentChange;
		TdmfKey = 0;
		nWobbleType =0;
		dWobbleAngle=0.0;
		bLowRelax = FALSE;
		memset(strDgnCode, 0, sizeof(strDgnCode));
		nStrandDia = 0;
		nStrandNum = 0;
		LongTermStressLossData.Initialize();
	}
	void Convert856(T_TDNT_D_CH_856& rData)
	{
		memcpy(TendonTypeName, rData.TendonTypeName, sizeof(TendonTypeName));   
		nTendonType = rData.nTendonType;
		MatlKey = rData.MatlKey;
		dTendonArea = rData.dTendonArea;
		dDuctArea = rData.dDuctArea;
		dRelaxCoef = rData.dRelaxCoef;
		dFrictionFactor = rData.dFrictionFactor;
		dWobbleFactor = rData.dWobbleFactor;
		dUltimateStrength = rData.dUltimateStrength;
		dYieldStrength = rData.dYieldStrength;
		nLoadType = rData.nLoadType;    
		dAnchoSlipBegin = rData.dAnchoSlipBegin;
		dAnchoSlipEnd = rData.dAnchoSlipEnd;
		nTypeId = rData.nTypeId;
		nBondType = rData.nBondType;
		dAlphaExtCable = rData.dAlphaExtCable;
		nRelaxMethod   = rData.nRelaxMethod;
		dRelaxRatio    = rData.dRelaxRatio;
		bOverStressRedFactor = rData.bOverStressRedFactor;
		dJTG04Factor = rData.dJTG04Factor;
		dFpk = rData.dFpk;
		dInadvertentChange=rData.dInadvertentChange;
		TdmfKey = rData.TdmfKey;
		nWobbleType =0;
		dWobbleAngle=0.0;
		bLowRelax = FALSE;
		memset(strDgnCode, 0, sizeof(strDgnCode));
		nStrandDia = 0;
		nStrandNum = 0;
		LongTermStressLossData.Initialize();
	}
	void Convert883(T_TDNT_D_CH_883& rData)
	{
		memcpy(TendonTypeName, rData.TendonTypeName, sizeof(TendonTypeName));   
		nTendonType = rData.nTendonType;
		MatlKey = rData.MatlKey;
		dTendonArea = rData.dTendonArea;
		dDuctArea = rData.dDuctArea;
		dRelaxCoef = rData.dRelaxCoef;
		dFrictionFactor = rData.dFrictionFactor;
		dWobbleFactor = rData.dWobbleFactor;
		dUltimateStrength = rData.dUltimateStrength;
		dYieldStrength = rData.dYieldStrength;
		nLoadType = rData.nLoadType;    
		dAnchoSlipBegin = rData.dAnchoSlipBegin;
		dAnchoSlipEnd = rData.dAnchoSlipEnd;
		nTypeId = rData.nTypeId;
		nBondType = rData.nBondType;
		dAlphaExtCable = rData.dAlphaExtCable;
		nRelaxMethod   = rData.nRelaxMethod;
		dRelaxRatio    = rData.dRelaxRatio;
		bOverStressRedFactor = rData.bOverStressRedFactor;
		dJTG04Factor = rData.dJTG04Factor;
		dFpk = rData.dFpk;
		dInadvertentChange=rData.dInadvertentChange;
		TdmfKey = rData.TdmfKey;
		nWobbleType = rData.nWobbleType;
		dWobbleAngle = rData.dWobbleAngle;
		bLowRelax = FALSE;
		memset(strDgnCode, 0, sizeof(strDgnCode));
		nStrandDia = 0;
		nStrandNum = 0;
		LongTermStressLossData.Initialize();
	}
	void Convert955(T_TDNT_D_CH_955& rData)
	{
		memcpy(TendonTypeName, rData.TendonTypeName, sizeof(TendonTypeName));
		nTendonType = rData.nTendonType;
		MatlKey = rData.MatlKey;
		dTendonArea = rData.dTendonArea;
		dDuctArea = rData.dDuctArea;
		dRelaxCoef = rData.dRelaxCoef;
		dFrictionFactor = rData.dFrictionFactor;
		dWobbleFactor = rData.dWobbleFactor;
		dUltimateStrength = rData.dUltimateStrength;
		dYieldStrength = rData.dYieldStrength;
		nLoadType = rData.nLoadType;
		dAnchoSlipBegin = rData.dAnchoSlipBegin;
		dAnchoSlipEnd = rData.dAnchoSlipEnd;
		nTypeId = rData.nTypeId;
		nBondType = rData.nBondType;
		dAlphaExtCable = rData.dAlphaExtCable;
		nRelaxMethod = rData.nRelaxMethod;
		dRelaxRatio = rData.dRelaxRatio;
		bOverStressRedFactor = rData.bOverStressRedFactor;
		dJTG04Factor = rData.dJTG04Factor;
		dFpk = rData.dFpk;
		dInadvertentChange = rData.dInadvertentChange;
		TdmfKey = rData.TdmfKey;
		nWobbleType = rData.nWobbleType;
		dWobbleAngle = rData.dWobbleAngle;
		bLowRelax = rData.bLowRelax;
		memset(strDgnCode, 0, sizeof(strDgnCode));
		nStrandDia = 0;
		nStrandNum = 0;
		LongTermStressLossData.Initialize();
	}
	void Convert965(T_TDNT_D_CH_965& rData)
	{
		memcpy(TendonTypeName, rData.TendonTypeName, sizeof(TendonTypeName));

		nTendonType          = rData.nTendonType;
		MatlKey              = rData.MatlKey;
		dTendonArea          = rData.dTendonArea;
		dDuctArea            = rData.dDuctArea;
		dRelaxCoef           = rData.dRelaxCoef;
		dFrictionFactor      = rData.dFrictionFactor;
		dWobbleFactor        = rData.dWobbleFactor;
		dUltimateStrength    = rData.dUltimateStrength;
		dYieldStrength       = rData.dYieldStrength;
		nLoadType            = rData.nLoadType;    
		dAnchoSlipBegin      = rData.dAnchoSlipBegin;
		dAnchoSlipEnd        = rData.dAnchoSlipEnd;
		nTypeId              = rData.nTypeId;
		nBondType            = rData.nBondType;
		dAlphaExtCable       = rData.dAlphaExtCable;
		nRelaxMethod         = rData.nRelaxMethod;
		dRelaxRatio          = rData.dRelaxRatio;
		bOverStressRedFactor = rData.bOverStressRedFactor;
		dJTG04Factor         = rData.dJTG04Factor;
		dFpk                 = rData.dFpk;
		dInadvertentChange   = rData.dInadvertentChange;
		TdmfKey              = rData.TdmfKey;
		nWobbleType          = rData.nWobbleType;
		dWobbleAngle         = rData.dWobbleAngle;
		bLowRelax            = rData.bLowRelax;
		memcpy(strDgnCode, rData.strDgnCode, sizeof(strDgnCode));
		nStrandDia           = rData.nStrandDia;
		nStrandNum           = rData.nStrandNum;

		LongTermStressLossData.Convert965(rData.LongTermStressLossData);
	}
};

struct T_TDNT_UDRD_D
{
	T_TDNT_K key;
	T_TDNT_D data;
};

struct T_TDNT_UDRD_D_CH_560
{
	T_TDNT_K key;
	T_TDNT_D_CH_560 data;
};

struct T_TDNT_UDRD_D_CH_630
{
	T_TDNT_K key;
	T_TDNT_D_CH_630 data;
};

struct T_TDNT_UDRD_D_CH_650
{
	T_TDNT_K key;
	T_TDNT_D_CH_650 data;
};

struct T_TDNT_UDRD_D_CH_660
{
	T_TDNT_K key;
	T_TDNT_D_CH_660 data;
};

struct T_TDNT_UDRD_D_CH_702
{
	T_TDNT_K key;
	T_TDNT_D_CH_702 data;
};

struct T_TDNT_UDRD_D_CH_842
{
	T_TDNT_K key;
	T_TDNT_D_CH_842 data;
};

struct T_TDNT_UDRD_D_CH_856
{
	T_TDNT_K key;
	T_TDNT_D_CH_856 data;
};

struct T_TDNT_UDRD_D_CH_883
{
	T_TDNT_K key;
	T_TDNT_D_CH_883 data;
};

struct T_TDNT_UDRD_D_CH_955
{
	T_TDNT_K key;
	T_TDNT_D_CH_955 data;
};

struct T_TDNT_UDRD_D_CH_965
{
	T_TDNT_K key;
	T_TDNT_D_CH_965 data;
};

struct T_TDNT_UDRD_D_CH
{
	T_TDNT_K key;
	T_TDNT_D_CH data;
};

struct T_TDNT_LTSL_UNIT
{
	T_TDNT_LTSL_UNIT()
	{
		dEstForce         = D_UNITSYS_BASE_FORCE;
		dConcAge          = D_UNITSYS_NONE;
		dConcElasticity   = D_UNITSYS_BASE_STRESS;
		dRelativeHumidity = D_UNITSYS_BASE_PERCENT;
		dVSRatio          = D_UNITSYS_BASE_LENGTH;
		dAvgPreComp       = D_UNITSYS_BASE_STRESS;
		dUltCreepCoef     = D_UNITSYS_NONE;
		dTensFactor       = D_UNITSYS_NONE;
		dShrinkageConst   = D_UNITSYS_NONE;
		dRelaxConst       = D_UNITSYS_BASE_STRESS;
	}
	int dEstForce;
	int dConcAge;
	int dConcElasticity;
	int dRelativeHumidity;
	int dVSRatio;
	int dAvgPreComp;
	int dUltCreepCoef;
	int dTensFactor;
	int dShrinkageConst;
	int dRelaxConst;
};

struct T_TDNT_UNIT
{
	T_TDNT_UNIT()
	{
		dTendonArea = D_UNITSYS_BASE_AREA;
		dDuctArea = D_UNITSYS_BASE_LENGTH;
		dWobbleFactor = D_UNITSYS_BASE_PERLENGTH;
		dUltimateStrength = D_UNITSYS_BASE_STRESS;
		dYieldStrength = D_UNITSYS_BASE_STRESS;
		dAnchoSlipBegin = D_UNITSYS_BASE_LENGTH;
		dAnchoSlipEnd = D_UNITSYS_BASE_LENGTH;
		dAlphaExtCable = D_UNITSYS_BASE_STRESS;
		dFpk = D_UNITSYS_BASE_STRESS;
		dInadvertentChange = D_UNITSYS_BASE_RADIAN;
		dWobbleAngle = D_UNITSYS_BASE_RADIAN_L;
	}
	int dTendonArea;
	int dDuctArea;
	int dWobbleFactor;
	int dUltimateStrength;
	int dYieldStrength;
	int dAnchoSlipBegin;
	int dAnchoSlipEnd;
	int dAlphaExtCable;
	int dFpk;
	int dInadvertentChange;
	int dWobbleAngle;
	T_TDNT_LTSL_UNIT LongTermStressLossData;
};

#pragma pack(pop)

#endif