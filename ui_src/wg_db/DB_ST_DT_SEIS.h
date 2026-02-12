#ifndef __DB_ST_DT_SEIS_H__
#define __DB_ST_DT_SEIS_H__

#include "DB_ST_DN_ENUM.h"

#define MIDAS_SDK

#pragma pack(push, 1)

// [SeisCode] 추가시 수정

enum SEIS_CODETYPE
{
	KS_E_1992     = 1,
	UBC_E_1991    = 2,
	UBC_E_1997    = 3,
	ATC306_E      = 4,
	KS_E_2000     = 5,
	JIS_E_YY      = 6,
	IBC_E_2000    = 7,
	EURO_E_1996   = 8,
	CH_E_2001     = 9,
	NBC_E_1995    = 10,
	IS_E_2002     = 11,
	TAIWAN_E_1999 = 12,
	KBC_E_2005    = 13,
	CHSH_E_2003   = 14,
	TAIWAN_E_2006 = 15,
	EURO_E_2004   = 16,
	KBC_E_2009    = 17,
	TAIWAN_E_2011 = 18,
	NTC_E_2008    = 19,
	CH_E_2010     = 20,
	IBC_E_2009    = 21,
	IBC_E_2012    = 22,
	NTC_E_2012    = 23,
	NSR_E_2010    = 24,
	KBC_E_2016    = 25,
	P100_E_2013   = 26,
	IS_E_2016     = 27,
	NTC_E_2017	  = 28,	
	KDS_E_2019    = 29, // Gen
	DPT_E_2018    = 30,
	TAIWAN_E_2022 = 31,
	KDS_E_2018    = 32, // Civil
	USER_E_TYPE	  = 33,
	NSCP_E_2024   = 34,
	IS_E_2025     = 35, //GEN
	SEIS_CODE_END = IS_E_2025,
	SEIS_CODE_NUM = SEIS_CODE_END,
};

#define T_SEIS_K T_STLD_K
struct T_SEIS_ADDITION_780
{
	T_STOR_K KeyStor;
	double AddX;
	double AddY;
	void Initialize()
	{
		KeyStor=0;
		AddX=0.;
		AddY=0.;
	}
};

struct T_SEIS_ADDITION
{
	T_STOR_K KeyStor;
	double AddX;
	double AddY;
	double AddR;    // PMS:4183-HSSHIM-20100505. Additional Load에 Rz 추가
	void Initialize()
	{
		KeyStor=0;
		AddX=0.;
		AddY=0.;
		AddR=0.0;
	}
};

struct T_SEIS_USER
{
	T_STOR_K KeyStor;
	double dWeight;
	double dElev;
	double dForceX;
	double dForceY;
	void Initialize()
	{
		KeyStor = 0;
		dWeight = 0.0;
		dElev = 0.0;
		dForceX = 0.0;
		dForceY = 0.0;
	}
};

struct T_SEIS_KS1992
{
	double dZoneFactor;
	double dImportanceFactor;
	double dSoilFactor;
	double dPeriodAnalysis_X;
	double dPeriodAnalysis_Y;
	double dPeriodCode_X;
	double dPeriodCode_Y;
	double dResponseModificationFactor_X;
	double dResponseModificationFactor_Y;
	void Initialize()
	{
		dZoneFactor=0.12;
		dImportanceFactor=1.2;
		dSoilFactor=1.2;
		dPeriodAnalysis_X=0.;
		dPeriodAnalysis_Y=0.;
		dPeriodCode_X=0.;
		dPeriodCode_Y=0.;
		dResponseModificationFactor_X=6.;
		dResponseModificationFactor_Y=6.;
	}
};

struct T_SEIS_KS2000
{
	double dZoneFactor;
	double dImportanceFactor;
	double dSoilFactor;
	double dPeriodAnalysis_X;
	double dPeriodAnalysis_Y;
	double dPeriodCode_X;
	double dPeriodCode_Y;
	double dResponseModificationFactor_X;
	double dResponseModificationFactor_Y;
	void Initialize()
	{
		dZoneFactor=0.11;
		dImportanceFactor=1.2;
		dSoilFactor=1.2;
		dPeriodAnalysis_X=0.;
		dPeriodAnalysis_Y=0.;
		dPeriodCode_X=0.;
		dPeriodCode_Y=0.;
		dResponseModificationFactor_X=6.;
		dResponseModificationFactor_Y=6.;
	}
};

struct T_SEIS_UBC1991
{
	double dZoneFactor;
	double dImportanceFactor;
	double dSoilProfileFactor;
	double dPeriodAnalysis_X;
	double dPeriodAnalysis_Y;
	double dPeriodCode_X;
	double dPeriodCode_Y;
	double dSystemCoefficient_X;
	double dSystemCoefficient_Y;
	void Initialize()
	{
		dZoneFactor=0.;
		dImportanceFactor=1.;
		dSoilProfileFactor=0.;
		dPeriodAnalysis_X=0.;
		dPeriodAnalysis_Y=0.;
		dPeriodCode_X=0.;
		dPeriodCode_Y=0.;
		dSystemCoefficient_X=6.;
		dSystemCoefficient_Y=6.;
	}
};

struct T_SEIS_UBC1997_640
{
	double dSeismicZoneFactor;
	double dImportanceFactor;
	int nSoilProfileType;
	int nSeismicSourceType;
	double dCloseastDistance;
	double dPeriod_X;
	double dPeriod_Y;
	double dResponseModificationFactor_X;
	double dResponseModificationFactor_Y;
	void Initialize()
	{
		dSeismicZoneFactor=0.;
		dImportanceFactor=1.;
		nSoilProfileType=0;
		nSeismicSourceType=0;
		dCloseastDistance=0.;
		dPeriod_X=0.;
		dPeriod_Y=0.;
		dResponseModificationFactor_X=4.5;
		dResponseModificationFactor_Y=4.5;
	}
};

struct T_SEIS_UBC1997
{
	double dSeismicZoneFactor;
	double dImportanceFactor;
	int nSoilProfileType;
	int nSeismicSourceType;
	double dCloseastDistance;
	double dPeriod_X;
	double dPeriod_Y;
	double dResponseModificationFactor_X;
	double dResponseModificationFactor_Y;
	double dPeriodAnalysis_X;   // 6.5.0추가
	double dPeriodAnalysis_Y;   // 6.5.0추가
	void Initialize()
	{
		dSeismicZoneFactor=0.;
		dImportanceFactor=1.;
		nSoilProfileType=0;
		nSeismicSourceType=0;
		dCloseastDistance=0.;
		dPeriod_X=0.;
		dPeriod_Y=0.;
		dResponseModificationFactor_X=4.5;
		dResponseModificationFactor_Y=4.5;
		dPeriodAnalysis_X=0.;
		dPeriodAnalysis_Y=0.;
	}
	void Convert640(T_SEIS_UBC1997_640& rData)
	{
		dSeismicZoneFactor= rData.dSeismicZoneFactor;
		dImportanceFactor=rData.dImportanceFactor;
		nSoilProfileType=rData.nSoilProfileType;
		nSeismicSourceType=rData.nSeismicSourceType;
		dCloseastDistance=rData.dCloseastDistance;
		dPeriod_X=rData.dPeriod_X;
		dPeriod_Y=rData.dPeriod_Y;
		dResponseModificationFactor_X=rData.dResponseModificationFactor_X;
		dResponseModificationFactor_Y=rData.dResponseModificationFactor_Y;
		dPeriodAnalysis_X=rData.dPeriod_X;
		dPeriodAnalysis_Y=rData.dPeriod_Y;
	}
};

struct T_SEIS_ATC306
{
	double dEffectivePeakVelocity;
	double dSoilProfileCoefficient;
	double dPeriodAnalysis_X;
	double dPeriodAnalysis_Y;
	double dPeriodCode_X;
	double dPeriodCode_Y;
	double dResponseModificationFactor_X;
	double dResponseModificationFactor_Y;
	void Initialize()
	{
		dEffectivePeakVelocity=0.;
		dSoilProfileCoefficient=0.;
		dPeriodAnalysis_X=0.;
		dPeriodAnalysis_Y=0.;
		dPeriodCode_X=0.;
		dPeriodCode_Y=0.;
		dResponseModificationFactor_X=0.;
		dResponseModificationFactor_Y=0.;
	}
};

struct T_SEIS_JIS_AI_BASE
{
	T_STOR_K KeyStor;
	double dAiX;
	double dAiY;
	void Initialize()
	{
		KeyStor = 0;
		dAiX = 0.;
		dAiY = 0.;
	}
};

struct T_SEIS_JIS_610
{
	double dZoneFactor;
	double dStdShearFactor;
	double dSoilFactor;
	double dPeriodAnalysis_X;
	double dPeriodAnalysis_Y;
	double dPeriodCode_X;
	double dPeriodCode_Y;
	double dResponseModificationFactor_X;
	double dResponseModificationFactor_Y;
	void Initialize()
	{
		dZoneFactor=1.0;
		dStdShearFactor=0.2;
		dSoilFactor=0.6;
		dPeriodAnalysis_X=0.;
		dPeriodAnalysis_Y=0.;
		dPeriodCode_X=0.;
		dPeriodCode_Y=0.;
		dResponseModificationFactor_X=1.0;
		dResponseModificationFactor_Y=1.0;
	}
};

struct T_SEIS_JIS
{
	double dZoneFactor;
	double dStdShearFactor;
	double dSoilFactor;
	double dPeriodAnalysis_X;
	double dPeriodAnalysis_Y;
	double dPeriodCode_X;
	double dPeriodCode_Y;
	double dResponseModificationFactor_X;
	double dResponseModificationFactor_Y;
	int    nAiMethod;    // 6.2.0에서 추가 : 0=Automatic, 1=User Defined
	CArray<T_SEIS_JIS_AI_BASE, T_SEIS_JIS_AI_BASE&> aSeisJisAi;  // 6.2.0에서 추가
	void Initialize()
	{
		dZoneFactor=1.0;
		dStdShearFactor=0.2;
		dSoilFactor=0.6;
		dPeriodAnalysis_X=0.;
		dPeriodAnalysis_Y=0.;
		dPeriodCode_X=0.;
		dPeriodCode_Y=0.;
		dResponseModificationFactor_X=1.0;
		dResponseModificationFactor_Y=1.0;
		nAiMethod = 0;
		aSeisJisAi.RemoveAll();
	}
	void Convert610(T_SEIS_JIS_610& rData)
	{
		dZoneFactor= rData.dZoneFactor;
		dStdShearFactor=rData.dStdShearFactor;
		dSoilFactor=rData.dSoilFactor;
		dPeriodAnalysis_X=rData.dPeriodAnalysis_X;
		dPeriodAnalysis_Y=rData.dPeriodAnalysis_Y;
		dPeriodCode_X=rData.dPeriodCode_X;
		dPeriodCode_Y=rData.dPeriodCode_Y;
		dResponseModificationFactor_X=rData.dResponseModificationFactor_X;
		dResponseModificationFactor_Y=rData.dResponseModificationFactor_Y;
		nAiMethod = 0;
		aSeisJisAi.RemoveAll();
	}
	T_SEIS_JIS() {}
	T_SEIS_JIS(T_SEIS_JIS& src) { *this = src; }
	T_SEIS_JIS& operator=(const T_SEIS_JIS& src)
	{
		dZoneFactor= src.dZoneFactor;
		dStdShearFactor=src.dStdShearFactor;
		dSoilFactor=src.dSoilFactor;
		dPeriodAnalysis_X=src.dPeriodAnalysis_X;
		dPeriodAnalysis_Y=src.dPeriodAnalysis_Y;
		dPeriodCode_X=src.dPeriodCode_X;
		dPeriodCode_Y=src.dPeriodCode_Y;
		dResponseModificationFactor_X=src.dResponseModificationFactor_X;
		dResponseModificationFactor_Y=src.dResponseModificationFactor_Y;
		nAiMethod = src.nAiMethod;
		aSeisJisAi.Copy(src.aSeisJisAi);
		return *this;
	}
};

struct T_SEIS_JIS_RW
{
	double dZoneFactor;
	double dStdShearFactor;
	double dSoilFactor;
	double dPeriodAnalysis_X;
	double dPeriodAnalysis_Y;
	double dPeriodCode_X;
	double dPeriodCode_Y;
	double dResponseModificationFactor_X;
	double dResponseModificationFactor_Y;
	int    nAiMethod;    // 6.2.0에서 추가 : 0=Automatic, 1=User Defined
	//CArray<T_SEIS_JIS_AI_BASE, T_SEIS_JIS_AI_BASE&> aSeisJisAi;  // 6.2.0에서 추가
	int    nCountSeisJisAi;
	void Get(T_SEIS_JIS& rData)
	{
	#define D_COPY_VAR(x) rData.x = x
		D_COPY_VAR(dZoneFactor);
		D_COPY_VAR(dStdShearFactor);
		D_COPY_VAR(dSoilFactor);
		D_COPY_VAR(dPeriodAnalysis_X);
		D_COPY_VAR(dPeriodAnalysis_Y);
		D_COPY_VAR(dPeriodCode_X);
		D_COPY_VAR(dPeriodCode_Y);
		D_COPY_VAR(dResponseModificationFactor_X);
		D_COPY_VAR(dResponseModificationFactor_Y);
		D_COPY_VAR(nAiMethod);
		rData.aSeisJisAi.RemoveAll();
	#undef D_COPY_VAR
	}
	void Set(const T_SEIS_JIS& rData)
	{
	#define D_COPY_VAR(x) x = rData.x
		D_COPY_VAR(dZoneFactor);
		D_COPY_VAR(dStdShearFactor);
		D_COPY_VAR(dSoilFactor);
		D_COPY_VAR(dPeriodAnalysis_X);
		D_COPY_VAR(dPeriodAnalysis_Y);
		D_COPY_VAR(dPeriodCode_X);
		D_COPY_VAR(dPeriodCode_Y);
		D_COPY_VAR(dResponseModificationFactor_X);
		D_COPY_VAR(dResponseModificationFactor_Y);
		D_COPY_VAR(nAiMethod);
		nCountSeisJisAi = ( int ) rData.aSeisJisAi.GetSize();
	#undef D_COPY_VAR
	}
	void Initialize()
	{
		dZoneFactor=1.0;
		dStdShearFactor=0.2;
		dSoilFactor=0.6;
		dPeriodAnalysis_X=0.;
		dPeriodAnalysis_Y=0.;
		dPeriodCode_X=0.;
		dPeriodCode_Y=0.;
		dResponseModificationFactor_X=1.0;
		dResponseModificationFactor_Y=1.0;
		nAiMethod = 0;
		nCountSeisJisAi = 0;
	}
	void Convert610(T_SEIS_JIS_610& rData)
	{
		dZoneFactor= rData.dZoneFactor;
		dStdShearFactor=rData.dStdShearFactor;
		dSoilFactor=rData.dSoilFactor;
		dPeriodAnalysis_X=rData.dPeriodAnalysis_X;
		dPeriodAnalysis_Y=rData.dPeriodAnalysis_Y;
		dPeriodCode_X=rData.dPeriodCode_X;
		dPeriodCode_Y=rData.dPeriodCode_Y;
		dResponseModificationFactor_X=rData.dResponseModificationFactor_X;
		dResponseModificationFactor_Y=rData.dResponseModificationFactor_Y;
		nAiMethod = 0;
		nCountSeisJisAi = 0;
	}
};

struct T_SEIS_IBC2000
{
	int nSeismicDesignCategory; // A=0, B, C, D, E, F=5
	int nSiteClass;             // A=0, B, C, D, E, F=5
	double dMSRAs;
	double dMSRA1;
	double dImportanceFactor;
	double dPeriodCode_X;
	double dPeriodCode_Y;
	double dPeriodCalculated_X;
	double dPeriodCalculated_Y;
	double dResponseModificationFactor_X;
	double dResponseModificationFactor_Y;
	void Initialize()
	{
		nSeismicDesignCategory=0;
		nSiteClass=0;
		dMSRAs=1.0;
		dMSRA1=0.3;
		dImportanceFactor=1.0;
		dPeriodCode_X=0.;
		dPeriodCode_X=0.;
		dPeriodCalculated_X=0.;
		dPeriodCalculated_Y=0.;
		dResponseModificationFactor_X=6.;
		dResponseModificationFactor_Y=6.;
	}
};

struct T_SEIS_EURO1996      // added by KYM
{
	int nSoilClass;           // 0:A, 1:B, 2:C
	int nBasicBehavior;       // 0:5.0, 1:4.5, 2:4.0, 3:3.5, 4:2.0
	int nDuctility;           // 0:HIGH, 1:MEDIUM, 2:LOW
	int nRegularity;          // 0:REG, 1:IRREG
	double dFailureModeFactor;
	double dAlphaFactor;
	double dPeriod_X;
	double dPeriod_Y;
	void Initialize()
	{
		nSoilClass = 0;
		nBasicBehavior = 0;
		nDuctility = 0;
		nRegularity = 0;
		dFailureModeFactor = 1.0;
		dAlphaFactor = 0.2;
		dPeriod_X = 0;
		dPeriod_Y = 0;
	}
};

struct T_SEIS_CH2002    // added by KYM
{
	int nSeisDgnCategory;     // 0:1, 1:2, 2:3
	int nSeisFortification;   // 0:0.05g, 1:0.10g, 2:0.15g, 3:0.20g, 4:0.30g, 5:0.40g
	int nSiteClass;           // 0:I, 1:II, 2:III, 3:IV
	int nDeltaN;              // 0:RSM, 1:IFM, 2:TSB, 3:ETC
	int nEarthResponse;       // 0:FREQUENT, 1:SCARSE
	BOOL bAlpha1;
	double dDamping;
	double dPeriod_X;
	double dPeriod_Y;
	void Initialize()
	{
		nSeisDgnCategory = 0;
		nSeisFortification = 0;
		nSiteClass = 0;
		nDeltaN = 0;
		nEarthResponse = 0;
		bAlpha1 = FALSE;
		dDamping = 0.05;
		dPeriod_X =0;
		dPeriod_Y =0;
	}
};

struct T_SEIS_NBC1995
{
	int    nAccelerationZone;
	int    nVelocityZone;
	double dZonalVelocityRatio;
	double dImportanceFactor;
	double dFoundationFactor;
	double dPeriodAnalysis_X;
	double dPeriodAnalysis_Y;
	double dPeriodCode_X;
	double dPeriodCode_Y;
	double dForceModificationFactor_X;
	double dForceModificationFactor_Y;
	void Initialize()
	{
		nAccelerationZone = 1;
		nVelocityZone = 1;
		dZonalVelocityRatio = 0.05;
		dImportanceFactor = 1.0;
		dFoundationFactor = 1.0;
		dPeriodAnalysis_X=0.;
		dPeriodAnalysis_Y=0.;
		dPeriodCode_X=0.;
		dPeriodCode_Y=0.;
		dForceModificationFactor_X = 4.0;
		dForceModificationFactor_Y = 4.0;
	}
};

struct T_SEIS_IS2002   // Indian Standard
{
	int    nSeismicZone;   // 0 : II, 1 : III, 2 : IV, 3 : V
	int    nSoilType;      // 0 : I(Rock or Hard Soil), 1 : II(Medium Soil), 2 : III(Soft Soil)
	double dImportanceFactor;   // 1.0, 1.5
	double dDamping;       // Percent(0 ~ 100, default : 5%, 30이상일 때는 30 일때의 값 사용)
	double dPeriod_X;
	double dPeriod_Y;
	double dResponseReductionFactor_X;  // 1.5, 2.5, 3.0, 4.0, 4.5, 5.0,
	double dResponseReductionFactor_Y;
	void Initialize()
	{
		nSeismicZone = 0;
		nSoilType = 0;
		dImportanceFactor = 1.0;
		dDamping = 5.;
		dPeriod_X = 0.;
		dPeriod_Y = 0.;
		dResponseReductionFactor_X = 3.0;
		dResponseReductionFactor_Y = 3.0;
	}
};

struct T_SEIS_IS1893_2016   // Indian Standard
{
	int    nSeismicZone; // 0 : II, 1 : III, 2 : IV, 3 : V
	int    nSoilType; // 0 : I(Rock or Hard Soil), 1 : II(Medium Soil), 2 : III(Soft Soil)
	double dImportanceFactor; // 1.0,1.2, 1.5
	double dDamping; // Percent(0 ~ 100, default : 5%, 30이상일 때는 30 일때의 값 사용)
	double dPeriod_X;
	double dPeriod_Y;
	double dResponseReductionFactor_X; // 1.5, 2.0,2.5, 3.0, 4.0, 4.5, 5.0,
	double dResponseReductionFactor_Y;
	void Initialize()
	{
		nSeismicZone = 0;
		nSoilType = 0;
		dImportanceFactor = 1.0;
		dDamping = 5.;
		dPeriod_X = 0.;
		dPeriod_Y = 0.;
		dResponseReductionFactor_X = 3.0;
		dResponseReductionFactor_Y = 3.0;
	}
};

struct T_SEIS_TAIWAN86   // Taiwan Standard
{
	int    nSeismicZone;   // 0 : ZoneI(0.33g) 1 : ZoneII(0.23g)
	int    nSoilType;      // 0 : I(Rock or Hard Soil), 1 : II(Medium Soil), 2 : III(Soft Soil), 3 : North Basin
	double dImportanceFactor;   // 1.0, 1.25, 1.5
	double dSeisMagnifyFactor;  // 1.2, 1.5
	int    nPeriodMethod;       // 0=Analytical, 1=Approximate
	double dPeriodAnalX;
	double dPeriodAnalY;
	double dPeriodApprX;
	double dPeriodApprY;
	double dPeriodFinalX;
	double dPeriodFinalY;
	double dResponseModifyFactorX;  // 1.6, 2.4, 2.8, 3.2, 3.6, 4.0, 4.4, 4.8
	double dResponseModifyFactorY;
	void Initialize()
	{
		nSeismicZone = 0;
		nSoilType = 0;
		dImportanceFactor = 1.0;
		dSeisMagnifyFactor = 5.;
		nPeriodMethod = 0;
		dPeriodAnalX = 0.;
		dPeriodAnalY = 0.;
		dPeriodApprX = 0.;
		dPeriodApprY = 0.;
		dPeriodFinalX = 0.;
		dPeriodFinalY = 0.;
		dResponseModifyFactorX=1.6;
		dResponseModifyFactorY=1.6;
	}
};

struct T_SEIS_KBC2005      // Korea Building Code 2005
{
	int    nAccCalcMethod;   // 0=Per Code, 1=User Defined
	int    nSeismicZone;     // 0=Zone1(0.11), 1=Zone2(0.07)
	int    nSiteClass;       // 0=Sa, 1=Sb, 2=Sc, 3=Sd, 4=Sd
	double dSds;             // Design Spectral Response Acc. at short periods
	double dSd1;             // Design Spectral Response Acc. at 1 s periods
	int    nSeismicUseGroup; // 0=Special, 1=I, 2=II
	BOOL   bCityPlanRegion;  // 도시계획구역 여부
	double dImportanceFactor;   // 0.8, 1.0, 1.2, 1.5
	int    nPeriodMethod;       // 0=Analytical, 1=Approximate
	double dPeriodAnalX;
	double dPeriodAnalY;
	double dPeriodApprX;
	double dPeriodApprY;
	double dPeriodFinalX;
	double dPeriodFinalY;
	double dResponseModifyFactorX;  // 1.25, 1.5, 2.5, 3, 4.5, 5, 5.5, 6, 6.5, 7, 8
	double dResponseModifyFactorY;
	void Initialize()
	{
		nAccCalcMethod = 0;
		nSeismicZone   = 0;
		nSiteClass     = 3;
		dSds = 0.52668;
		dSd1 = 0.33649;
		nSeismicUseGroup  = 1;
		bCityPlanRegion   = TRUE;
		dImportanceFactor = 1.2;
		nPeriodMethod = 1;
		dPeriodAnalX  = 0.;
		dPeriodAnalY  = 0.;
		dPeriodApprX  = 0.;
		dPeriodApprY  = 0.;
		dPeriodFinalX = 0.;
		dPeriodFinalY = 0.;
		dResponseModifyFactorX = 4.5;
		dResponseModifyFactorY = 4.5;
	}
};

struct T_SEIS_CHSH2003    // added by KYM
{
	int nSeisFortification;   // 0:6(0.05g), 1:7(0.1g), 2:8(0.2g)
	int nSiteClass;           // 0:III, 1:IV
	int nDeltaN;              // 0:RSM, 1:IFM, 2:TSB, 3:ETC
	int nEarthResponse;       // 0:FREQUENT, 1:SCARSE
	BOOL bAlpha1;
	double dDamping;
	double dPeriod_X;
	double dPeriod_Y;
	void Initialize()
	{
		nSeisFortification = 0;
		nSiteClass = 1;
		nDeltaN = 0;
		nEarthResponse = 0;
		bAlpha1 = FALSE;
		dDamping = 0.05;
		dPeriod_X =0;
		dPeriod_Y =0;
	}
};

// MNET:1986-GSJI-20060428
struct T_SEIS_TAIWAN02_701   // Taiwan Standard 2002->2006
{
	int    nSeismicZone;        // 0 : General Zone  1 : Near Fault Zone  2 : Taipai Basin
	double dSds, dSd1;          // General Zone: Spectral Acc. Design. short / 1sec
	double dSms, dSm1;          // General Zone: Spectral Acc. Maximum. short / 1sec
	double dNa, dNv;            // NearFault Zone: Spectral Acc. short / 1sec
	int    nSoilType;           // 0 : TypeI   1 : TypeII, 2 : TypeIII  3 : UserInput
	double dFda, dFdv;          // Site Magify Factor. Design. short / 1sec
	double dFma, dFmv;          // Site Magify Factor. Maximum. short / 1sec
	int    nSubZone;            // 0: TaipaiBasin1 1: TaipaiBasin2 2:TaipaiBasin3 3:TaipaiBasin4
	double dSds_t, dSms_t;      // Taipai Basin: Spectral Acc. short  Design/Maximum
	double dTd0, dTm0;          // Trans전환주기(Taipai Basin only)
	double dImportanceFactor;   // 1.0, 1.25, 1.5
	double dSeisMagnifyFactor;  // 1.2, 1.5
	int    nPeriodMethod;       // 0=Analytical, 1=Approximate
	double dPeriodAnalX;
	double dPeriodAnalY;
	double dPeriodApprX;
	double dPeriodApprY;
	double dPeriodFinalX;
	double dPeriodFinalY;
	double dResponseModifyFactorX;  // 1.6, 2.4, 2.8, 3.2, 3.6, 4.0, 4.4, 4.8
	double dResponseModifyFactorY;
};

struct T_SEIS_TAIWAN02   // Taiwan Standard 2006
{
	int    nSeismicZone;
	double dSds, dSd1;
	double dSms, dSm1;
	double dNda, dNdv;      // 7.0.2에서 이름 변경
	int    nSoilType;
	double dFda, dFdv;
	double dFma, dFmv;
	int    nSubZone;
	double dSds_t, dSms_t;
	double dTd0, dTm0;
	double dImportanceFactor;
	double dSeisMagnifyFactor;
	int    nPeriodMethod;
	double dPeriodAnalX;
	double dPeriodAnalY;
	double dPeriodApprX;
	double dPeriodApprY;
	double dPeriodFinalX;
	double dPeriodFinalY;
	double dResponseModifyFactorX;
	double dResponseModifyFactorY;
	double dNma, dNmv;      // 7.0.2에서 추가 NearFault Zone: Spectral Acc. Maximum short / 1sec
	void Initialize()
	{
		nSeismicZone = 0;
		dSds = 0.5; dSd1 = 0.3;
		dSms = 0.7; dSm1 = 0.45;
		dNda = 1.0; dNdv = 1.0;
		dNma = 1.0; dNmv = 1.0;

		nSoilType = 0;
		dFda = 1.0; dFdv = 1.0;
		dFma = 1.0; dFmv = 1.0;

		nSubZone  = 0;
		dSds_t = 0.6; dSms_t = 0.8;
		dTd0   = 1.6; dTm0   = 1.6;

		dImportanceFactor = 1.0;
		dSeisMagnifyFactor = 1.2;
		nPeriodMethod = 0;
		dPeriodAnalX = 0.;
		dPeriodAnalY = 0.;
		dPeriodApprX = 0.;
		dPeriodApprY = 0.;
		dPeriodFinalX = 0.;
		dPeriodFinalY = 0.;
		dResponseModifyFactorX=1.6;
		dResponseModifyFactorY=1.6;
	}
	void Convert701(T_SEIS_TAIWAN02_701& rData)
	{
		nSeismicZone = rData.nSeismicZone;
		dSds = rData.dSds;      dSd1 = rData.dSd1;
		dSms = rData.dSms;      dSm1 = rData.dSm1;
		dNda = rData.dNa;       dNdv = rData.dNv;
		dNma = rData.dNa;       dNmv = rData.dNv;
		nSoilType = rData.nSoilType;
		dFda = rData.dFda;      dFdv = rData.dFdv;
		dFma = rData.dFma;      dFmv = rData.dFmv;
		nSubZone = rData.nSubZone;
		dSds_t = rData.dSds_t;  dSms_t = rData.dSms_t;
		dTd0 = rData.dTd0;      dTm0 = rData.dTm0;
		dImportanceFactor = rData.dImportanceFactor;
		dSeisMagnifyFactor= rData.dSeisMagnifyFactor;
		nPeriodMethod = rData.nPeriodMethod;
		dPeriodAnalX = rData.dPeriodAnalX;
		dPeriodAnalY = rData.dPeriodAnalY;
		dPeriodApprX = rData.dPeriodApprX;
		dPeriodApprY = rData.dPeriodApprY;
		dPeriodFinalX = rData.dPeriodFinalX;
		dPeriodFinalY = rData.dPeriodFinalY;
		dResponseModifyFactorX = rData.dResponseModifyFactorX;
		dResponseModifyFactorY = rData.dResponseModifyFactorY;
	}
};

struct T_SEIS_TAIWAN22
{
	int    nStructType;
	int    nSeismicZone;
	double dSds, dSd1;
	double dSms, dSm1;
	double dNda, dNdv;      // 7.0.2에서 이름 변경
	int    nSoilType;
	double dFda, dFdv; // Design Spectrum
	double dFsa, dFsv; // Small - Medium Spectrum
	double dFma, dFmv; // Maximum Spectrum
	int    nSubZone;
	double dSds_t, dSms_t;
	double dTd0, dTm0;
	double dImportanceFactor;
	double dSeisMagnifyFactor;
	int    nPeriodMethod;
	double dPeriodAnalX;
	double dPeriodAnalY;
	double dPeriodApprX;
	double dPeriodApprY;
	double dPeriodFinalX;
	double dPeriodFinalY;
	double dResponseModifyFactorX;
	double dResponseModifyFactorY;
	double dNma, dNmv;      // 7.0.2에서 추가 NearFault Zone: Spectral Acc. Maximum short / 1sec
	void Initialize()
	{
		nStructType = 0;
		nSeismicZone = 0;
		dSds = 0.5; dSd1 = 0.3;
		dSms = 0.7; dSm1 = 0.45;
		dNda = 0.80; dNdv = 0.45;
		dNma = 1.00; dNmv = 0.55;

		nSoilType = 0;
		dFda = 1.0; dFdv = 1.0;
		dFsa = 1.0; dFsv = 1.0;
		dFma = 1.0; dFmv = 1.0;

		nSubZone  = 0;
		dSds_t = 0.6; dSms_t = 0.8;
		dTd0   = 1.6; dTm0   = 1.6;

		dImportanceFactor = 1.0;
		dSeisMagnifyFactor = 1.2;
		nPeriodMethod = 0;
		dPeriodAnalX = 0.;
		dPeriodAnalY = 0.;
		dPeriodApprX = 0.;
		dPeriodApprY = 0.;
		dPeriodFinalX = 0.;
		dPeriodFinalY = 0.;
		dResponseModifyFactorX=1.6;
		dResponseModifyFactorY=1.6;
	}
};

struct T_SEIS_EURO2004_945      // added by KYM
{
	int nGroundType;           // 0:A, 1:B, 2:C, 3:D, 4:E, 5:S1, 6:S2 (Recommended) 10:C, 11:D, 12:S1 (Singapore)
	int nSpecType;             // 0:Type1, 1:Type2, 2:User defined
	double dS;                 // Soil Factor
	double dTb;
	double dTc;
	double dTd;
	double dAg;                // Design ground acceleration
	double dQ;                 // Behavior factor
	double dBeta;              // Lower bound factor
	double dImportance;
	double dPeriod_X;
	double dPeriod_Y;
	void Initialize()
	{
		nGroundType = 1;
		nSpecType = 0;
		dS = 1.2;
		dTb = 0.15;
		dTc = 0.5;
		dTd = 2.0;
		dAg = 0.08;
		dQ = 1.5;
		dBeta = 0.2;
		dImportance = 1.0;
		dPeriod_X = 0;
		dPeriod_Y = 0;
	}
};

enum EN_SEIS_NA
{
	EN_RECOMMENDED = 0,
	EN_SINGAPORE,
	EN_MALESIA,
};
enum EN_SEIS_EURO2004_GROUND
{
	EN_GROUND_A = 0,
	EN_GROUND_B,
	EN_GROUND_C,
	EN_GROUND_D,
	EN_GROUND_E,
	EN_GROUND_S1,
	EN_GROUND_S2,
};
enum EN_SEIS_EURO2004_REGION
{
	EN_REGION_PENINSULAR = 0,
	EN_REGION_SABAH,
	EN_REGION_SARAWAK,
};
struct T_SEIS_EURO2004      // added by KYM
{
	int nGroundType;           // 0:A, 1:B, 2:C, 3:D, 4:E, 5:S1, 6:S2 (Recommended) 10:C, 11:D, 12:S1 (Singapore)
	int nSpecType;             // 0:Type1, 1:Type2, 2:User defined
	EN_SEIS_NA nNationalAnnex;
	double dS;                 // Soil Factor
	double dTb;
	double dTc;
	double dTd;
	double dAg;                // Design ground acceleration
	double dQ;                 // Behavior factor
	double dBeta;              // Lower bound factor
	double dImportance;
	double dPeriod_X;
	double dPeriod_Y;
	int nResionType;			// (Malaysia) 0:Peninsular  1:Sabah, 1:Sarawak  
	void Initialize()
	{
		nGroundType = 1;
		nSpecType = 0;
		nNationalAnnex = EN_RECOMMENDED;
		dS = 1.2;
		dTb = 0.15;
		dTc = 0.5;
		dTd = 2.0;
		dAg = 0.08;
		dQ = 1.5;
		dBeta = 0.2;
		dImportance = 1.0;
		dPeriod_X = 0;
		dPeriod_Y = 0;
		nResionType = EN_REGION_PENINSULAR;
	}
	void Convert945(T_SEIS_EURO2004_945& rData)
	{
		nGroundType = rData.nGroundType;
		nSpecType = rData.nSpecType;
		nResionType = EN_REGION_PENINSULAR;
		if (nGroundType >= 10)
		{
			nNationalAnnex = EN_SINGAPORE;
			switch (nGroundType)
			{
				case 10: nGroundType = EN_GROUND_C; break;
				case 11: nGroundType = EN_GROUND_D; break;
				case 12: nGroundType = EN_GROUND_S1; break;
				default: nGroundType = EN_GROUND_C; ASSERT(0);
					break;
			}
		}
		else
			nNationalAnnex = EN_RECOMMENDED;
		dS = rData.dS;
		dTb = rData.dTb;
		dTc = rData.dTc;
		dTd = rData.dTd;
		dAg = rData.dAg;
		dQ = rData.dQ;
		dBeta = rData.dBeta;
		dImportance = rData.dImportance;
		dPeriod_X = rData.dPeriod_X;
		dPeriod_Y = rData.dPeriod_Y;
	}
};
//SANS2010 - South African Code
struct T_SEIS_SANS2010      // added by Pinakin
{
	int nGroundType;           // 0:A, 1:B, 2:C, 3:D, 4:E, 5:S1, 6:S2 (Recommended) 10:C, 11:D, 12:S1 (Singapore)
	int nSpecType;             // 0:Type1, 1:Type2, 2:User defined
	EN_SEIS_NA nNationalAnnex;
	double dS;                 // Soil Factor
	double dTb;
	double dTc;
	double dTd;
	double dAg;                // Design ground acceleration
	double dQ;                 // Behavior factor
	double dBeta;              // Lower bound factor
	double dImportance;
	double dPeriod_X;
	double dPeriod_Y;
	int nResionType;			// (Malaysia) 0:Peninsular  1:Sabah, 1:Sarawak  
	void Initialize()
	{
		nGroundType = 1;
		nSpecType = 0;
		nNationalAnnex = EN_RECOMMENDED;
		dS = 1.2;
		dTb = 0.15;
		dTc = 0.5;
		dTd = 2.0;
		dAg = 0.08;
		dQ = 1.5;
		dBeta = 0.2;
		dImportance = 1.0;
		dPeriod_X = 0;
		dPeriod_Y = 0;
		nResionType = EN_REGION_PENINSULAR;
	}
	
};

// [2011-12-16] Kim, Geun Young (Tel: 2042, gykim@midasit.com)
struct T_SEIS_NTC2008
{
	int nGroundType;           // 0:A, 1:B, 2:C, 3:D, 4:E, 5:S1, 6:S2
	int nSpecType;             // 0:T1, 1:T2, 2:T3, 3:T4, 4:User defined
	double dS;                 // Soil Factor
	double dTb;
	double dTc;
	double dTd;
	double dAg;                // Maximum Horizontal Acc
	double dQ;                 // Structure factor
	double dAmpF;              // Amplification factor
	double dTcStar;            // Period of constant Hori. Acc
	double dPeriod_X;
	double dPeriod_Y;
	void Initialize()
	{
		nGroundType = 1;
		nSpecType = 0;
		dS = 1.2;
		dTb = 0.15;
		dTc = 0.5;
		dTd = 2.0;
		dAg = 0.08;
		dQ = 1.5;
		dAmpF = 2.5;
		dTcStar = 0.3;
		dPeriod_X = 0;
		dPeriod_Y = 0;
	}
};

struct T_SEIS_KBC2009      // Korea Building Code 2008
{
	int    nSeismicZone;     // 0:1, 1:2
	double dZoneFactor;      // Zone Factor
	int    nSiteClass;       // 0:Sa, 1:Sb, 2:Sc, 3:Sd, 4:Se
	double dPeriodCoef;      // The coefficient for upper limit in calculated period, upper limit coefficient
	double dFa;              // Acceleration-based site coefficient
	double dFv;              // Velocity-based site coefficient
	double dSds;             // Design Spectral Response Acc. at short periods
	double dSd1;             // Design Spectral Response Acc. at 1 s periods
	int    nSeismicUseGroup; // 0=Special, 1=I, 2=II
	double dImportanceFactor;   // 1.0, 1.2, 1.5
	int    nPeriodMethod;       // 0=Analytical, 1=Approximate
	double dPeriodAnalX;
	double dPeriodAnalY;
	double dPeriodApprX;
	double dPeriodApprY;
	double dPeriodFinalX;
	double dPeriodFinalY;
	double dResponseModifyFactorX;  // 1.5, 2.5, 3, 3.25, 4.5, 5, 5.5, 6, 6.5, 7, 8
	double dResponseModifyFactorY;  // 1.5, 2.5, 3, 3.25, 4.5, 5, 5.5, 6, 6.5, 7, 8
	void Initialize()
	{
		nSeismicZone   = 0;
		dZoneFactor    = 0.22;
		nSiteClass     = 3;
		dPeriodCoef    = 1.41253;
		dFa            = 1.36;
		dFv            = 1.96;
		dSds           = 0.49867;
		dSd1           = 0.28747;
		nSeismicUseGroup  = 1;
		dImportanceFactor = 1.2;
		nPeriodMethod = 1;
		dPeriodAnalX  = 0.;
		dPeriodAnalY  = 0.;
		dPeriodApprX  = 0.;
		dPeriodApprY  = 0.;
		dPeriodFinalX = 0.;
		dPeriodFinalY = 0.;
		dResponseModifyFactorX = 4.0;
		dResponseModifyFactorY = 4.0;
	}
};

// Add by GAY. PMS:4267. ('12.02.15). [GB50011-10]
struct T_SEIS_CH2010
{
	int nSeisDgnCategory;     // 0:1, 1:2, 2:3
	int nSeisFortification;   // 0:0.05g, 1:0.10g, 2:0.15g, 3:0.20g, 4:0.30g, 5:0.40g
	int nSiteClass;           // 0:I0, 1:I1, 2:II, 3:III, 4:IV
	int nDeltaN;              // 0:RSM, 1:IFM, 2:TSB, 3:ETC
	int nEarthResponse;       // 0:FREQUENT, 1:MIDDLE, 2:SCARSE
	BOOL bAlpha1;
	double dDamping;
	double dPeriod_X;
	double dPeriod_Y;
	void Initialize()
	{
		nSeisDgnCategory = 0;
		nSeisFortification = 0;
		nSiteClass = 0;
		nDeltaN = 0;
		nEarthResponse = 0;
		bAlpha1 = FALSE;
		dDamping = 0.05;
		dPeriod_X =0;
		dPeriod_Y =0;
	}
};

struct T_SEIS_IBC2012
{
	int    nSiteClass;       // 0:A, 1:B, 2:C, 3:D, 4:E
	double dSs;              // 단주기 최대지진스펙트럼가속도
	double dS1;              // 1초주기 최대지진스펙트럼가속도
	double dFa;              // Acceleration-based site coefficient
	double dFv;              // Velocity-based site coefficient
	double dSds;             // Design Spectral Response Acc. at short periods
	double dSd1;             // Design Spectral Response Acc. at 1 s periods
	double dTL;              // Long period transition period
	int    nRiskCategory;    // 2009에서는 Occupancy Category로 사용. 건물 위험도 0:I, 1:II, 2:III, 3:IV
	double dImportanceFactor;// 1.0, 1.25, 1.5
	int    nPeriodMethod;    // 0=Analytical, 1=Approximate
	double dPeriodAnalX;
	double dPeriodAnalY;
	double dPeriodApprX;
	double dPeriodApprY;
	double dPeriodFinalX;
	double dPeriodFinalY;
	double dResponseModifyFactorX;  // 1.5, 2.5, 3, 3.25, 4.5, 5, 5.5, 6, 6.5, 7, 8
	double dResponseModifyFactorY;  // 1.5, 2.5, 3, 3.25, 4.5, 5, 5.5, 6, 6.5, 7, 8
	void Initialize()
	{
		nSiteClass = 3;
		dSs = 0.75;
		dS1 = 0.3;
		dFa = 1.2;
		dFv = 1.8;
		dSds = 0.6;
		dSd1 = 0.36;
		dTL = 4.;
		nRiskCategory = 1;
		dImportanceFactor = 1.0;
		nPeriodMethod = 1;
		dPeriodAnalX  = 0.;
		dPeriodAnalY  = 0.;
		dPeriodApprX  = 0.;
		dPeriodApprY  = 0.;
		dPeriodFinalX = 0.;
		dPeriodFinalY = 0.;
		dResponseModifyFactorX = 4.0;
		dResponseModifyFactorY = 4.0;
	}
};

struct T_SEIS_NSR2010_871
{
	int    nSiteClass;       // 0:A, 1:B, 2:C, 3:D, 4:E
	double dAa;              // Aa
	double dAv;              // Av
	double dFa;             // Fa
	double dFv;             // Fv
	double dCu;              // Cu
	double dImportanceFactor;// 1.0, 1.25, 1.5
	int    nPeriodMethod;    // 0=Analytical, 1=Approximate
	double dPeriodAnalX;
	double dPeriodAnalY;
	double dPeriodApprX;
	double dPeriodApprY;
	double dPeriodFinalX;
	double dPeriodFinalY;
	double dResponseModifyFactorX;  // 1.5, 2.5, 3, 3.25, 4.5, 5, 5.5, 6, 6.5, 7, 8
	double dResponseModifyFactorY;  // 1.5, 2.5, 3, 3.25, 4.5, 5, 5.5, 6, 6.5, 7, 8
	void Initialize()
	{
		nSiteClass = 3;
		dAa = 0.15;
		dAv = 0.15;
		dFa = 1.5;
		dFv = 2.2;
		dCu = 1.75 - ( 1.2 * dAv *dFv );
		dImportanceFactor = 1.0;
		nPeriodMethod = 1;
		dPeriodAnalX  = 0.;
		dPeriodAnalY  = 0.;
		dPeriodApprX  = 0.;
		dPeriodApprY  = 0.;
		dPeriodFinalX = 0.;
		dPeriodFinalY = 0.;
		dResponseModifyFactorX = 4.0;
		dResponseModifyFactorY = 4.0;
	}
};

struct T_SEIS_NSR2010
{
	int    nSiteClass;       // 0:A, 1:B, 2:C, 3:D, 4:E
	double dAa;              // Aa
	double dAv;              // Av
	double dFa;             // Fa
	double dFv;             // Fv
	double dCu;              // Cu
	double dImportanceFactor;// 1.0, 1.25, 1.5
	int    nPeriodMethod;    // 0=Analytical, 1=Approximate
	double dPeriodAnalX;
	double dPeriodAnalY;
	double dPeriodApprX;
	double dPeriodApprY;
	double dPeriodFinalX;
	double dPeriodFinalY;
	double dResponseModifyFactorX;  // 1.5, 2.5, 3, 3.25, 4.5, 5, 5.5, 6, 6.5, 7, 8
	double dResponseModifyFactorY;  // 1.5, 2.5, 3, 3.25, 4.5, 5, 5.5, 6, 6.5, 7, 8
	double dPhiX;
	double dPhiY;
	void Initialize()
	{
		nSiteClass = 3;
		dAa = 0.15;
		dAv = 0.15;
		dFa = 1.5;
		dFv = 2.2;
		dCu = 1.75 - ( 1.2 * dAv *dFv );
		dImportanceFactor = 1.0;
		nPeriodMethod = 1;
		dPeriodAnalX  = 0.;
		dPeriodAnalY  = 0.;
		dPeriodApprX  = 0.;
		dPeriodApprY  = 0.;
		dPeriodFinalX = 0.;
		dPeriodFinalY = 0.;
		dResponseModifyFactorX = 4.0;
		dResponseModifyFactorY = 4.0;
		dPhiX = 1.0;
		dPhiY = 1.0;
	}
	void Convert871(T_SEIS_NSR2010_871& rData)
	{
		nSiteClass             = rData.nSiteClass;
		dAa                    =rData.dAa;
		dAv                    =rData.dAv;
		dFa                    =rData.dFa;
		dFv                    =rData.dFv;
		dCu                    =rData.dCu;
		dImportanceFactor      =rData.dImportanceFactor;
		nPeriodMethod          =rData.nPeriodMethod;
		dPeriodAnalX           =rData.dPeriodAnalX;
		dPeriodAnalY           =rData.dPeriodAnalY;
		dPeriodApprX           =rData.dPeriodApprX;
		dPeriodApprY           =rData.dPeriodApprY;
		dPeriodFinalX          =rData.dPeriodFinalX;
		dPeriodFinalY          =rData.dPeriodFinalY;
		dResponseModifyFactorX =rData.dResponseModifyFactorX;
		dResponseModifyFactorY =rData.dResponseModifyFactorY;
		dPhiX                  =1.0;
		dPhiY                  =1.0;
	}
};

struct T_SEIS_KBC2015      // Korea Building Code 2015
{
	int    nSeismicZone;            // 0:1, 1:2
	double dZoneFactor;             // Zone Factor
	int    nSiteClass;              // 0:Sa, 1:Sb, 2:Sc, 3:Sd, 4:Se
	double dDepth2MR;                   // 보통암까지의 깊이
	double dPeriodCoef;             // The coefficient for upper limit in calculated period, upper limit coefficient
	double dFa;                             // Acceleration-based site coefficient
	double dFv;                             // Velocity-based site coefficient
	double dSds;                            // Design Spectral Response Acc. at short periods
	double dSd1;                            // Design Spectral Response Acc. at 1 s periods
	int    nSeismicUseGroup;    // 0=Special, 1=I, 2=II
	double dImportanceFactor; // 1.0, 1.2, 1.5
	int    nPeriodMethod;     // 0=Analytical, 1=Approximate
	double dPeriodAnalX;
	double dPeriodAnalY;
	double dPeriodApprX;
	double dPeriodApprY;
	double dPeriodFinalX;
	double dPeriodFinalY;
	double dResponseModifyFactorX;  // 1.5, 2.5, 3, 3.25, 4.5, 5, 5.5, 6, 6.5, 7, 8
	double dResponseModifyFactorY;  // 1.5, 2.5, 3, 3.25, 4.5, 5, 5.5, 6, 6.5, 7, 8
	void Initialize()
	{
		nSeismicZone   = 0;
		dZoneFactor    = 0.22;
		nSiteClass     = 3;
		dDepth2MR            = 0.;
		dPeriodCoef    = 1.41253;
		dFa            = 1.46;
		dFv            = 1.58;
		dSds           = 0.53533;
		dSd1           = 0.23173;
		nSeismicUseGroup  = 1;
		dImportanceFactor = 1.2;
		nPeriodMethod = 1;
		dPeriodAnalX  = 0.;
		dPeriodAnalY  = 0.;
		dPeriodApprX  = 0.;
		dPeriodApprY  = 0.;
		dPeriodFinalX = 0.;
		dPeriodFinalY = 0.;
		dResponseModifyFactorX = 4.0;
		dResponseModifyFactorY = 4.0;
	}
};

struct T_SEIS_KDS2019
{
	int    nSeismicZone;        // 0:1, 1:2
	double dZoneFactor;         // Zone Factor
	int    nSiteClass;          // 0:S1, 1:S2, 2:S3, 3:S4, 4:S5, 5:S6
	double dPeriodCoef;         // The coefficient for upper limit in calculated period, upper limit coefficient
	BOOL   bNotConsiderCu;      // Not Consider Period Coef.(Cu)
	double dFa;                 // Acceleration-based site coefficient
	double dFv;                 // Velocity-based site coefficient
	double dSds;                // Design Spectral Response Acc. at short periods
	double dSd1;                // Design Spectral Response Acc. at 1 s periods
	int    nSeismicUseGroup;    // 0=Special, 1=I, 2=II
	double dImportanceFactor;   // 1.0, 1.2, 1.5
	int    nPeriodMethod;       // 0=Analytical, 1=Approximate
	double dPeriodAnalX;
	double dPeriodAnalY;
	double dPeriodApprX;
	double dPeriodApprY;
	double dPeriodFinalX;
	double dPeriodFinalY;
	double dResponseModifyFactorX;  // 1.5, 2.5, 3, 3.25, 4.5, 5, 5.5, 6, 6.5, 7, 8
	double dResponseModifyFactorY;  // 1.5, 2.5, 3, 3.25, 4.5, 5, 5.5, 6, 6.5, 7, 8
	void Initialize()
	{
		nSeismicZone   = 0;
		dZoneFactor    = 0.22;
		nSiteClass     = 1;
		dPeriodCoef    = 1.4976;
		bNotConsiderCu = FALSE;
		dFa            = 1.38;
		dFv            = 1.38;
		dSds           = 0.506;
		dSd1           = 0.2024;
		nSeismicUseGroup  = 1;
		dImportanceFactor = 1.2;
		nPeriodMethod = 1;
		dPeriodAnalX  = 0.;
		dPeriodAnalY  = 0.;
		dPeriodApprX  = 0.;
		dPeriodApprY  = 0.;
		dPeriodFinalX = 0.;
		dPeriodFinalY = 0.;
		dResponseModifyFactorX = 4.0;
		dResponseModifyFactorY = 4.0;
	}
};

enum D_SEIS_P100_2013_TC
{
	D_SEIS_P100_2013_TC07S,  // Tc=0.7s
	D_SEIS_P100_2013_TC10S,  // Tc=1.0s
	D_SEIS_P100_2013_TC16S,  // Tc=1.6s
	D_SEIS_P100_2013_USERD   // User Defined
};

struct T_SEIS_P100_2013      // Romanian building code 2013
{
	int nSpecType;             // D_SEIS_P100_2013_TC
	double dTb;
	double dTc;
	double dTd;
	double dAg;                // Design ground acceleration
	double dQ;                 // Behavior factor
	double dImportance;
	double dPeriod_X;
	double dPeriod_Y;
	void Initialize()
	{
		nSpecType = D_SEIS_P100_2013_TC07S;
		dTb = 0.15;
		dTc = 0.5;
		dTd = 2.0;
		dAg = 0.3;
		dQ  = 4.0;
		dImportance = 1.0;
		dPeriod_X = 0;
		dPeriod_Y = 0;
	}
};

enum EN_DPT_SEIS_REGION
{
	EN_DPT_SEIS_REGION_BANGKOK = 0,
	EN_DPT_SEIS_REGION_NOT_BANGKOK,
};

enum EN_DPT_SEIS_METHOD
{
	EN_DPT_SEIS_METHOD_GRAPH = 0,
	EN_DPT_SEIS_METHOD_TABLE,
};

enum EN_DPT_SEIS_CLASS // Site Class
{
	EN_DPT_SEIS_CLASS_A = 0,
	EN_DPT_SEIS_CLASS_B,
	EN_DPT_SEIS_CLASS_C,
	EN_DPT_SEIS_CLASS_D,
	EN_DPT_SEIS_CLASS_E,
	EN_DPT_SEIS_CLASS_F,
};

enum EN_DPT_SEIS_RISK  // Risk Category
{
	EN_DPT_SEIS_RISK_1 = 0,
	EN_DPT_SEIS_RISK_2,
	EN_DPT_SEIS_RISK_3,
	EN_DPT_SEIS_RISK_4,
};

enum EN_DPT_SEIS_PERIOD // Period Method
{
	EN_DPT_SEIS_PERIOD_ANAL = 0,
	EN_DPT_SEIS_PERIOD_APPR,
};

enum EN_DPT_SEIS_DAMPING
{
	EN_DPT_SEIS_DAMPING_0_025 = 0,
	EN_DPT_SEIS_DAMPING_0_050,
};

struct T_SEIS_DPT2018_PERIOD
{
	double dT_anal;  // analytical period
	double dT_appr;  // approximated period
	double dT_final; // final

	void Init()
	{
		dT_anal  = 0.0;
		dT_appr  = 0.0;
		dT_final = 0.0;
	}
};

struct T_SEIS_DPT2018
{
	EN_DPT_SEIS_REGION unRegion;
	EN_DPT_SEIS_METHOD unMethod;
	int                nSeisZone;
	BOOL               bByCode;             // Check By Code
	EN_DPT_SEIS_CLASS  unSiteClass;
	double             dSs;                 // 단주기 최대지진스펙트럼가속도
	double             dS1;                 // 1초주기 최대지진스펙트럼가속도
	double             dFa;                 // Acceleration-based site coefficient
	double             dFv;                 // Velocity-based site coefficient
	double             dSds;                // Design Spectral Response Acc. at short periods
	double             dSd1;                // Design Spectral Response Acc. at 1 s periods
	EN_DPT_SEIS_RISK   unOccupancyCategory; // I, II, III, IV
	double             dImportanceFactor;   // 1.0, 1.25, 1.5
	EN_DPT_SEIS_PERIOD unPeriodMethod;      // Analytical, Approximate

	T_SEIS_DPT2018_PERIOD Period[2];
	double dR[2];  // Response modification factor
	double dCd[2]; // Deflection amplification factor
	double dDamping;

	void Init()
	{
		unRegion            = EN_DPT_SEIS_REGION_BANGKOK;
		unMethod            = EN_DPT_SEIS_METHOD_TABLE;
		nSeisZone           = 1;
		bByCode             = TRUE;
		unSiteClass         = EN_DPT_SEIS_CLASS_D;
		dSs                 = 0.75;
		dS1                 = 0.30;
		dFa                 = 1.20;
		dFv                 = 1.80;
		dSds                = 0.60 / 0.85; // for 2.5% damping
		dSd1                = 0.36 / 0.85; // for 2.5% damping
		unOccupancyCategory = EN_DPT_SEIS_RISK_2;
		dImportanceFactor   = 1.0;
		unPeriodMethod      = EN_DPT_SEIS_PERIOD_APPR;

		for ( int i = 0; i < 2; ++i )
		{
			Period[i].Init();
			dR[i]  = 3.0;
			dCd[i] = 1.0;
		}

		dDamping = 0.025;
	}
};

enum EN_IS2025_SPEC_TYPE
{
	EN_IS25_SPEC_HOR = 0,
	EN_IS25_SPEC_VER,
};

enum EN_IS2025_EQ_ZONE
{
	EN_IS25_EQ_ZONE_II = 0,
	EN_IS25_EQ_ZONE_III,
	EN_IS25_EQ_ZONE_IV,
	EN_IS25_EQ_ZONE_V,
	EN_IS25_EQ_ZONE_VI,
	EN_IS25_EQ_ZONE_COUNT,
};

enum EN_IS2025_RETURN_PERIOD
{
	EN_IS25_RET_75 = 0,
	EN_IS25_RET_175,
	EN_IS25_RET_275,
	EN_IS25_RET_475,
	EN_IS25_RET_975,
	EN_IS25_RET_1275,
	EN_IS25_RET_2475,
	EN_IS25_RET_4975,
	EN_IS25_RET_9975,
	EN_IS25_RET_COUNT,
};

enum EN_IS25_SITE_CLASS
{
	EN_IS25_SITE_A = 0,
	EN_IS25_SITE_B,
	EN_IS25_SITE_C,
	EN_IS25_SITE_D,
	EN_IS25_SITE_COUNT,
};

struct T_SEIS_IS1893_2025   // Indian Standard
{
	int nSpecType;		   // Spectrum Type
	int nEqZone;           // Seismic Zone
	int nReturnPeriod;     // Soil Class
	double dZoneFactor;    // EQ Zone factor
	int nSiteClass;		   // Site class
	double dDamping;       // Damping(%)
	double dImpFactor;	   // Importance Factor(I)
	double dPeriod_X;
	double dPeriod_Y;
	double dResponseReductionFactor_X;
	double dResponseReductionFactor_Y;
	void Initialize()
	{
		nSpecType = EN_IS25_SPEC_HOR;
		nEqZone = EN_IS25_EQ_ZONE_II;
		nReturnPeriod = EN_IS25_RET_75;
		dZoneFactor = 0.0375;
		nSiteClass = EN_IS25_SITE_A;
		dDamping = 5.0;
		dImpFactor = 1.0;
		dPeriod_X = 0.0;
		dPeriod_Y = 0.0;
		dResponseReductionFactor_X = 5.0;
		dResponseReductionFactor_Y = 5.0;
	}
};

struct T_SEIS_D
{
	int nCodeType; // 1=KS.Arch.1992 2=UBC.1991  3=UBC.1997   4=ATC3-06   5=KS2000      6=JIS          7=IBC2000
				   // 8=EURO1996     9=CH2002   10=NBC1995   11=IS2002   12=TAIWAN86   13=KBC2005     14=CHSH2003
				   // 15=TAIWAN02   16=EURO2004 17 = KBC2009 18=TAIWAN11 19=NTC2008    20=CH2010      21=IBC2009
				   // 22=IBC2012    23=NTC2012  24=NSR2010   25=KBC2015  26=P100_2013  27=IS1893_2016 28=NTC2018
				   // 29=KDS2019    30=DPT2018  31=TAIWAN22  32=KDS2018  33=USER_TYPE
				   // Code 추가시 wg_main의 ModelTextOut.cpp에 반영할 것
	CString Description;
	T_SEIS_KS1992   KS1992;
	T_SEIS_UBC1991  UBC1991;
	T_SEIS_UBC1997  UBC1997;
	T_SEIS_ATC306   ATC306;
	T_SEIS_KS2000   KS2000;
	T_SEIS_JIS      JP;
	T_SEIS_IBC2000  IBC2000;
	T_SEIS_EURO1996 EURO1996;   // added by KYM
	T_SEIS_CH2002   CH2002;     // added by KYM
	T_SEIS_NBC1995  NBC1995;
	T_SEIS_IS2002   IS2002;
	T_SEIS_TAIWAN86 TAIWAN86;
	T_SEIS_KBC2005  KBC2005;
	T_SEIS_CHSH2003 CHSH2003;
	T_SEIS_TAIWAN02 TAIWAN02;
	T_SEIS_EURO2004 EURO2004;
	T_SEIS_KBC2009  KBC2009;
	T_SEIS_TAIWAN02 TAIWAN11;  // PARKHJ-20111007 : TAIWAN02와 같은 구조체 사용
	T_SEIS_NTC2008  NTC2008;
	T_SEIS_NTC2008  NTC2012;   // PMS:DNKIM-20121017 NTC 2008과 같은 구조체 사용
	T_SEIS_CH2010   CH2010;    // Add by GAY. PMS:4267. ('12.02.15).
	T_SEIS_IBC2012  IBC2009;
	T_SEIS_IBC2012  IBC2012;
	T_SEIS_NSR2010  NSR2010;
	T_SEIS_KBC2015  KBC2015;
	T_SEIS_P100_2013 P100_2013;
	T_SEIS_IS1893_2016 IS1893_2016;
	T_SEIS_NTC2008  NTC2018;   // PMS:JWKWON-20171106 NTC 2008과 같은 구조체 사용
	T_SEIS_KDS2019  KDS2019;
	T_SEIS_DPT2018  DPT2018;
	T_SEIS_TAIWAN22 TAIWAN22;
	T_SEIS_IBC2012  NSCP2024;
	T_SEIS_SANS2010 SANS2010;
	T_SEIS_IS1893_2025 IS2025;

	double ScaleFactor_X;
	double ScaleFactor_Y;
	double ScaleFactor_R;
	int nEccen_X;
	int nEccen_Y;
	BOOL bInherentTorsionAmplify;
	BOOL bAccidentTorsionAmplify;
	CArray<T_SEIS_ADDITION, T_SEIS_ADDITION&>arAddition;
	CArray<T_SEIS_USER, T_SEIS_USER&> arUser;
	void Initialize()
	{
		nCodeType=0;
		Description.Empty();

		KS1992.Initialize();
		UBC1991.Initialize();
		UBC1997.Initialize();
		ATC306.Initialize();
		KS2000.Initialize();
		JP.Initialize();
		IBC2000.Initialize();
		EURO1996.Initialize();  // added by KYM
		CH2002.Initialize();    // added by KYM
		NBC1995.Initialize();
		IS2002.Initialize();
		TAIWAN86.Initialize();
		KBC2005.Initialize();
		CHSH2003.Initialize();
		TAIWAN02.Initialize();
		EURO2004.Initialize();
		KBC2009.Initialize();
		TAIWAN11.Initialize();
		NTC2008.Initialize();
		NTC2012.Initialize();
		CH2010.Initialize();
		IBC2009.Initialize();
		IBC2012.Initialize();
		NSR2010.Initialize();
		KBC2015.Initialize();
		P100_2013.Initialize();
		IS1893_2016.Initialize();
		NTC2018.Initialize();
		KDS2019.Initialize();
		DPT2018.Init();
		TAIWAN22.Initialize();
		NSCP2024.Initialize();
		SANS2010.Initialize();
		IS2025.Initialize();

		ScaleFactor_X=0.;
		ScaleFactor_Y=0.;
		ScaleFactor_R=0.;
		nEccen_X = 0;
		nEccen_Y = 0;
		bInherentTorsionAmplify = FALSE;
		bAccidentTorsionAmplify = FALSE;
		arAddition.RemoveAll();
		arUser.RemoveAll();
	}
	T_SEIS_D()
	{
		//Initialize();
	}
	T_SEIS_D(T_SEIS_D& src)
	{
		*this = src;
	}
	T_SEIS_D& operator=(T_SEIS_D& src)
	{
		nCodeType=src.nCodeType;
		Description=src.Description;

		KS1992       = src.KS1992;
		UBC1991      = src.UBC1991;
		UBC1997      = src.UBC1997;
		ATC306       = src.ATC306;
		KS2000       = src.KS2000;
		JP           = src.JP;
		IBC2000      = src.IBC2000;
		EURO1996     = src.EURO1996;   // added by KYM
		CH2002       = src.CH2002;     // added by KYM
		NBC1995      = src.NBC1995;
		IS2002       = src.IS2002;
		TAIWAN86     = src.TAIWAN86;
		KBC2005      = src.KBC2005;
		CHSH2003     = src.CHSH2003;
		TAIWAN02     = src.TAIWAN02;
		EURO2004     = src.EURO2004;
		KBC2009      = src.KBC2009;
		TAIWAN11     = src.TAIWAN11;
		NTC2008      = src.NTC2008;
		NTC2012      = src.NTC2012;
		CH2010       = src.CH2010;
		IBC2009      = src.IBC2009;
		IBC2012      = src.IBC2012;
		NSR2010      = src.NSR2010;
		KBC2015      = src.KBC2015;
		P100_2013    = src.P100_2013;
		IS1893_2016  = src.IS1893_2016;
		NTC2018      = src.NTC2018;
		KDS2019      = src.KDS2019;
		DPT2018      = src.DPT2018;
		TAIWAN22     = src.TAIWAN22;
		NSCP2024     = src.NSCP2024;
		SANS2010 	 = src.SANS2010;
		IS2025		 = src.IS2025;

		ScaleFactor_X=src.ScaleFactor_X;
		ScaleFactor_Y=src.ScaleFactor_Y;
		ScaleFactor_R=src.ScaleFactor_R;
		nEccen_X=src.nEccen_X;
		nEccen_Y=src.nEccen_Y;
		bInherentTorsionAmplify = src.bInherentTorsionAmplify;
		bAccidentTorsionAmplify = src.bAccidentTorsionAmplify;
		arAddition.Copy(src.arAddition);
		arUser.Copy(src.arUser);
		return *this;
	}
};

struct T_SEIS_D_CH
{
	int nCodeType;        // 1=KS.Arch.1992  2=UBC.1991  3=UBC.1997  4=ATC3-06
	char Description[80];

	T_SEIS_KS1992   KS1992;
	T_SEIS_UBC1991  UBC1991;
	T_SEIS_UBC1997  UBC1997;
	T_SEIS_ATC306   ATC306;
	T_SEIS_KS2000   KS2000;
	T_SEIS_JIS      JP;
	T_SEIS_IBC2000  IBC2000;
	T_SEIS_EURO1996 EURO1996;   // added by KYM
	T_SEIS_CH2002   CH2002;     // added by KYM
	T_SEIS_NBC1995  NBC1995;
	T_SEIS_IS2002   IS2002;
	T_SEIS_TAIWAN86 TAIWAN86;
	T_SEIS_KBC2005  KBC2005;
	T_SEIS_CHSH2003 CHSH2003;
	T_SEIS_TAIWAN02 TAIWAN02;
	T_SEIS_EURO2004 EURO2004;
	T_SEIS_KBC2009  KBC2009;
	T_SEIS_TAIWAN02 TAIWAN11;
	T_SEIS_NTC2008  NTC2008;
	T_SEIS_NTC2008  NTC2012;
	T_SEIS_CH2010   CH2010;
	T_SEIS_IBC2012  IBC2009;
	T_SEIS_IBC2012  IBC2012;
	T_SEIS_NSR2010  NSR2010;
	T_SEIS_KBC2015  KBC2015;
	T_SEIS_P100_2013 P100_2013;
	T_SEIS_IS1893_2016 IS1893_2016;
	T_SEIS_NTC2008  NTC2018;
	T_SEIS_KDS2019  KDS2019;
	T_SEIS_DPT2018  DPT2018;
	T_SEIS_TAIWAN22 TAIWAN22;
	T_SEIS_IBC2012  NSCP2024;
	T_SEIS_SANS2010 SANS2010;
	T_SEIS_IS1893_2025 IS2025;

	double ScaleFactor_X;
	double ScaleFactor_Y;
	double ScaleFactor_R;
	int nEccen_X;
	int nEccen_Y;
	BOOL bInherentTorsionAmplify;
	BOOL bAccidentTorsionAmplify;
	CArray<T_SEIS_ADDITION, T_SEIS_ADDITION&>arAddition;
	CArray<T_SEIS_USER, T_SEIS_USER&> arUser;
	void ConvertToString(T_SEIS_D& rData)
	{
		rData.nCodeType=nCodeType;
		ConvertCharStr(Description, rData.Description, sizeof(Description));

		rData.KS1992       = KS1992;
		rData.UBC1991      = UBC1991;
		rData.UBC1997      = UBC1997;
		rData.ATC306       = ATC306;
		rData.KS2000       = KS2000;
		rData.JP           = JP;
		rData.IBC2000      = IBC2000;
		rData.EURO1996     = EURO1996;   // added by KYM
		rData.CH2002       = CH2002;     // added by KYM
		rData.NBC1995      = NBC1995;
		rData.IS2002       = IS2002;
		rData.TAIWAN86     = TAIWAN86;
		rData.KBC2005      = KBC2005;
		rData.CHSH2003     = CHSH2003;
		rData.TAIWAN02     = TAIWAN02;
		rData.EURO2004     = EURO2004;
		rData.KBC2009      = KBC2009;
		rData.TAIWAN11     = TAIWAN11;
		rData.NTC2008      = NTC2008;
		rData.NTC2012      = NTC2012;
		rData.CH2010       = CH2010;
		rData.IBC2009      = IBC2009;
		rData.IBC2012      = IBC2012;
		rData.NSR2010      = NSR2010;
		rData.KBC2015      = KBC2015;
		rData.P100_2013    = P100_2013;
		rData.IS1893_2016  = IS1893_2016;
		rData.NTC2018      = NTC2018;
		rData.KDS2019      = KDS2019;
		rData.DPT2018      = DPT2018;
		rData.TAIWAN22     = TAIWAN22;
		rData.NSCP2024     = NSCP2024;
		rData.SANS2010	   = SANS2010;
		rData.IS2025	   = IS2025;

		rData.ScaleFactor_X=ScaleFactor_X;
		rData.ScaleFactor_Y=ScaleFactor_Y;
		rData.ScaleFactor_R=ScaleFactor_R;
		rData.nEccen_X=nEccen_X;
		rData.nEccen_Y=nEccen_Y;
		rData.bInherentTorsionAmplify = bInherentTorsionAmplify;
		rData.bAccidentTorsionAmplify = bAccidentTorsionAmplify;
		rData.arAddition.Copy(arAddition);
		rData.arUser.Copy(arUser);
	}
	void ConvertToChar(T_SEIS_D& rData)
	{
		nCodeType=rData.nCodeType;
		ConvertStrChar(rData.Description, Description, sizeof(Description));

		KS1992       = rData.KS1992;
		UBC1991      = rData.UBC1991;
		UBC1997      = rData.UBC1997;
		ATC306       = rData.ATC306;
		KS2000       = rData.KS2000;
		JP           = rData.JP;
		IBC2000      = rData.IBC2000;
		EURO1996     = rData.EURO1996; // added by KYM
		CH2002       = rData.CH2002;   // added by KYM
		NBC1995      = rData.NBC1995;
		IS2002       = rData.IS2002;
		TAIWAN86     = rData.TAIWAN86;
		KBC2005      = rData.KBC2005;
		CHSH2003     = rData.CHSH2003;
		TAIWAN02     = rData.TAIWAN02;
		EURO2004     = rData.EURO2004;
		KBC2009      = rData.KBC2009;
		TAIWAN11     = rData.TAIWAN11;
		NTC2008      = rData.NTC2008;
		NTC2012      = rData.NTC2012;
		CH2010       = rData.CH2010;
		IBC2009      = rData.IBC2009;
		IBC2012      = rData.IBC2012;
		NSR2010      = rData.NSR2010;
		KBC2015      = rData.KBC2015;
		P100_2013    = rData.P100_2013;
		IS1893_2016  = rData.IS1893_2016;
		NTC2018      = rData.NTC2018;
		KDS2019      = rData.KDS2019;
		DPT2018      = rData.DPT2018;
		TAIWAN22     = rData.TAIWAN22;
		NSCP2024     = rData.NSCP2024;
		SANS2010	 = rData.SANS2010;
		IS2025		 = rData.IS2025; 

		ScaleFactor_X=rData.ScaleFactor_X;
		ScaleFactor_Y=rData.ScaleFactor_Y;
		ScaleFactor_R=rData.ScaleFactor_R;
		nEccen_X=rData.nEccen_X;
		nEccen_Y=rData.nEccen_Y;
		bInherentTorsionAmplify = rData.bInherentTorsionAmplify;
		bAccidentTorsionAmplify = rData.bAccidentTorsionAmplify;
		arAddition.Copy(rData.arAddition);
		arUser.Copy(rData.arUser);
	}
};

struct T_SEIS_UDRD_D
{
	T_SEIS_K key;
	T_SEIS_D data;
};
struct T_SEIS_UDRD_D_CH
{
	T_SEIS_K key;
	T_SEIS_D_CH data;
};

struct T_SEIS_UDRD_D_CH_RW_412
{
	T_SEIS_K key;
	int nCodeType;        // 1=KS.Arch.1992  2=UBC.1991  3=UBC.1997  4=ATC3-06
	char Description[80];
	T_SEIS_KS1992 KS1992;
	T_SEIS_UBC1991 UBC1991;
	T_SEIS_UBC1997_640 UBC1997;
	T_SEIS_ATC306 ATC306;
	double ScaleFactor_X;
	double ScaleFactor_Y;
	BOOL bEccenPositive_X;
	BOOL bEccenPositive_Y;
	int nCountAddition;
};
struct T_SEIS_UDRD_D_CH_RW_422
{
	T_SEIS_K key;
	int nCodeType;        // 1=KS.Arch.1992  2=UBC.1991  3=UBC.1997  4=ATC3-06  5=KS2000
	char Description[80];
	T_SEIS_KS1992 KS1992;
	T_SEIS_UBC1991 UBC1991;
	T_SEIS_UBC1997_640 UBC1997;
	T_SEIS_ATC306 ATC306;
	T_SEIS_KS2000 KS2000;
	double ScaleFactor_X;
	double ScaleFactor_Y;
	BOOL bEccenPositive_X;
	BOOL bEccenPositive_Y;
	int nCountAddition;
};
struct T_SEIS_UDRD_D_CH_RW_423
{
	T_SEIS_K key;
	int nCodeType;        // 1=KS.Arch.1992  2=UBC.1991  3=UBC.1997  4=ATC3-06  5=KS2000
	char Description[80];
	T_SEIS_KS1992   KS1992;
	T_SEIS_UBC1991  UBC1991;
	T_SEIS_UBC1997_640  UBC1997;
	T_SEIS_ATC306   ATC306;
	T_SEIS_KS2000   KS2000;
	T_SEIS_JIS_610  JP;
	double ScaleFactor_X;
	double ScaleFactor_Y;
	BOOL bEccenPositive_X;
	BOOL bEccenPositive_Y;
	int nCountAddition;
};
struct T_SEIS_UDRD_D_CH_RW_520   // added by KYM
{
	T_SEIS_K key;
	int nCodeType;        // 1=KS.Arch.1992  2=UBC.1991  3=UBC.1997  4=ATC3-06  5=KS2000
	char Description[80];
	T_SEIS_KS1992   KS1992;
	T_SEIS_UBC1991  UBC1991;
	T_SEIS_UBC1997_640  UBC1997;
	T_SEIS_ATC306   ATC306;
	T_SEIS_KS2000   KS2000;
	T_SEIS_JIS_610  JP;
	T_SEIS_IBC2000  IBC2000;
	double ScaleFactor_X;
	double ScaleFactor_Y;
	BOOL bEccenPositive_X;
	BOOL bEccenPositive_Y;
	int nCountAddition;
};
struct T_SEIS_UDRD_D_CH_RW_550   // added by KYM
{
	T_SEIS_K key;
	int nCodeType;
	char Description[80];
	T_SEIS_KS1992   KS1992;
	T_SEIS_UBC1991  UBC1991;
	T_SEIS_UBC1997_640  UBC1997;
	T_SEIS_ATC306   ATC306;
	T_SEIS_KS2000   KS2000;
	T_SEIS_JIS_610  JP;
	T_SEIS_IBC2000  IBC2000;
	T_SEIS_EURO1996 EURO1996;
	T_SEIS_CH2002   CH2002;
	double ScaleFactor_X;
	double ScaleFactor_Y;
	BOOL bEccenPositive_X;
	BOOL bEccenPositive_Y;
	int nCountAddition;
};
struct T_SEIS_UDRD_D_CH_RW_590
{
	T_SEIS_K key;
	int nCodeType;        // 1=KS.Arch.1992  2=UBC.1991  3=UBC.1997  4=ATC3-06  5=KS2000, 6=JIS, 7=IBC2000
	char Description[80];  //8=EURO1996      9=CH2002
	T_SEIS_KS1992   KS1992;
	T_SEIS_UBC1991  UBC1991;
	T_SEIS_UBC1997_640  UBC1997;
	T_SEIS_ATC306   ATC306;
	T_SEIS_KS2000   KS2000;
	T_SEIS_JIS_610  JP;
	T_SEIS_IBC2000  IBC2000;
	T_SEIS_EURO1996 EURO1996;   // added by KYM
	T_SEIS_CH2002   CH2002;     // added by KYM
	T_SEIS_NBC1995  NBC1995;
	double ScaleFactor_X;
	double ScaleFactor_Y;
	BOOL bEccenPositive_X;
	BOOL bEccenPositive_Y;
	int nCountAddition;
};
struct T_SEIS_UDRD_D_CH_RW_600
{
	T_SEIS_K key;
	int nCodeType;        // 1=KS.Arch.1992  2=UBC.1991  3=UBC.1997  4=ATC3-06  5=KS2000, 6=JIS, 7=IBC2000
	char Description[80];  //8=EURO1996      9=CH2002
	T_SEIS_KS1992   KS1992;
	T_SEIS_UBC1991  UBC1991;
	T_SEIS_UBC1997_640  UBC1997;
	T_SEIS_ATC306   ATC306;
	T_SEIS_KS2000   KS2000;
	T_SEIS_JIS_610  JP;
	T_SEIS_IBC2000  IBC2000;
	T_SEIS_EURO1996 EURO1996;   // added by KYM
	T_SEIS_CH2002   CH2002;     // added by KYM
	T_SEIS_NBC1995  NBC1995;
	T_SEIS_IS2002   IS2002;
	double ScaleFactor_X;
	double ScaleFactor_Y;
	int nEccen_X;
	int nEccen_Y;
	int nCountAddition;
};
struct T_SEIS_UDRD_D_CH_RW_610
{
	T_SEIS_K key;
	int nCodeType;        // 1=KS.Arch.1992  2=UBC.1991  3=UBC.1997  4=ATC3-06  5=KS2000, 6=JIS, 7=IBC2000
	char Description[80];  //8=EURO1996      9=CH2002
	T_SEIS_KS1992   KS1992;
	T_SEIS_UBC1991  UBC1991;
	T_SEIS_UBC1997_640  UBC1997;
	T_SEIS_ATC306   ATC306;
	T_SEIS_KS2000   KS2000;
	T_SEIS_JIS_610  JP;
	T_SEIS_IBC2000  IBC2000;
	T_SEIS_EURO1996 EURO1996;   // added by KYM
	T_SEIS_CH2002   CH2002;     // added by KYM
	T_SEIS_NBC1995  NBC1995;
	T_SEIS_IS2002   IS2002;
	T_SEIS_TAIWAN86 TAIWAN86;
	double ScaleFactor_X;
	double ScaleFactor_Y;
	int nEccen_X;
	int nEccen_Y;
	int nCountAddition;
};
struct T_SEIS_UDRD_D_CH_RW_640
{
	T_SEIS_K key;
	int nCodeType;        // 1=KS.Arch.1992  2=UBC.1991  3=UBC.1997  4=ATC3-06  5=KS2000, 6=JIS, 7=IBC2000
	char Description[80];  //8=EURO1996      9=CH2002
	T_SEIS_KS1992   KS1992;
	T_SEIS_UBC1991  UBC1991;
	T_SEIS_UBC1997_640  UBC1997;
	T_SEIS_ATC306   ATC306;
	T_SEIS_KS2000   KS2000;
	T_SEIS_JIS_RW   JP;
	T_SEIS_IBC2000  IBC2000;
	T_SEIS_EURO1996 EURO1996;   // added by KYM
	T_SEIS_CH2002   CH2002;     // added by KYM
	T_SEIS_NBC1995  NBC1995;
	T_SEIS_IS2002   IS2002;
	T_SEIS_TAIWAN86 TAIWAN86;
	double ScaleFactor_X;
	double ScaleFactor_Y;
	int nEccen_X;
	int nEccen_Y;
	int nCountAddition;
};
struct T_SEIS_UDRD_D_CH_RW_670
{
	T_SEIS_K key;
	int nCodeType;        // 1=KS.Arch.1992  2=UBC.1991  3=UBC.1997  4=ATC3-06  5=KS2000, 6=JIS, 7=IBC2000
	char Description[80];  //8=EURO1996      9=CH2002
	T_SEIS_KS1992   KS1992;
	T_SEIS_UBC1991  UBC1991;
	T_SEIS_UBC1997  UBC1997;
	T_SEIS_ATC306   ATC306;
	T_SEIS_KS2000   KS2000;
	T_SEIS_JIS_RW   JP;
	T_SEIS_IBC2000  IBC2000;
	T_SEIS_EURO1996 EURO1996;   // added by KYM
	T_SEIS_CH2002   CH2002;     // added by KYM
	T_SEIS_NBC1995  NBC1995;
	T_SEIS_IS2002   IS2002;
	T_SEIS_TAIWAN86 TAIWAN86;
	double ScaleFactor_X;
	double ScaleFactor_Y;
	int nEccen_X;
	int nEccen_Y;
	int nCountAddition;
};
struct T_SEIS_UDRD_D_CH_RW_671
{
	T_SEIS_K key;
	int nCodeType;        // 1=KS.Arch.1992  2=UBC.1991  3=UBC.1997  4=ATC3-06  5=KS2000, 6=JIS, 7=IBC2000
	char Description[80];  //8=EURO1996      9=CH2002
	T_SEIS_KS1992   KS1992;
	T_SEIS_UBC1991  UBC1991;
	T_SEIS_UBC1997  UBC1997;
	T_SEIS_ATC306   ATC306;
	T_SEIS_KS2000   KS2000;
	T_SEIS_JIS_RW   JP;
	T_SEIS_IBC2000  IBC2000;
	T_SEIS_EURO1996 EURO1996;   // added by KYM
	T_SEIS_CH2002   CH2002;     // added by KYM
	T_SEIS_NBC1995  NBC1995;
	T_SEIS_IS2002   IS2002;
	T_SEIS_TAIWAN86 TAIWAN86;
	T_SEIS_KBC2005  KBC2005;
	double ScaleFactor_X;
	double ScaleFactor_Y;
	int nEccen_X;
	int nEccen_Y;
	BOOL bInherentTorsionAmplify;
	BOOL bAccidentTorsionAmplify;
	int nCountAddition;
};
struct T_SEIS_UDRD_D_CH_RW_700
{
	T_SEIS_K key;
	int nCodeType;        // 1=KS.Arch.1992  2=UBC.1991  3=UBC.1997  4=ATC3-06  5=KS2000, 6=JIS, 7=IBC2000
	char Description[80];  //8=EURO1996      9=CH2002
	T_SEIS_KS1992   KS1992;
	T_SEIS_UBC1991  UBC1991;
	T_SEIS_UBC1997  UBC1997;
	T_SEIS_ATC306   ATC306;
	T_SEIS_KS2000   KS2000;
	T_SEIS_JIS_RW   JP;
	T_SEIS_IBC2000  IBC2000;
	T_SEIS_EURO1996 EURO1996;   // added by KYM
	T_SEIS_CH2002   CH2002;     // added by KYM
	T_SEIS_NBC1995  NBC1995;
	T_SEIS_IS2002   IS2002;
	T_SEIS_TAIWAN86 TAIWAN86;
	T_SEIS_KBC2005  KBC2005;
	T_SEIS_CHSH2003 CHSH2003;
	double ScaleFactor_X;
	double ScaleFactor_Y;
	int nEccen_X;
	int nEccen_Y;
	BOOL bInherentTorsionAmplify;
	BOOL bAccidentTorsionAmplify;
	int nCountAddition;
};
struct T_SEIS_UDRD_D_CH_RW_701
{
	T_SEIS_K key;
	int nCodeType;        // 1=KS.Arch.1992  2=UBC.1991  3=UBC.1997  4=ATC3-06  5=KS2000, 6=JIS, 7=IBC2000
	char Description[80];  //8=EURO1996      9=CH2002
	T_SEIS_KS1992   KS1992;
	T_SEIS_UBC1991  UBC1991;
	T_SEIS_UBC1997  UBC1997;
	T_SEIS_ATC306   ATC306;
	T_SEIS_KS2000   KS2000;
	T_SEIS_JIS_RW   JP;
	T_SEIS_IBC2000  IBC2000;
	T_SEIS_EURO1996 EURO1996;   // added by KYM
	T_SEIS_CH2002   CH2002;     // added by KYM
	T_SEIS_NBC1995  NBC1995;
	T_SEIS_IS2002   IS2002;
	T_SEIS_TAIWAN86 TAIWAN86;
	T_SEIS_KBC2005  KBC2005;
	T_SEIS_CHSH2003 CHSH2003;
	T_SEIS_TAIWAN02_701 TAIWAN02;
	double ScaleFactor_X;
	double ScaleFactor_Y;
	int nEccen_X;
	int nEccen_Y;
	BOOL bInherentTorsionAmplify;
	BOOL bAccidentTorsionAmplify;
	int nCountAddition;
};
struct T_SEIS_UDRD_D_CH_RW_711
{
	T_SEIS_K key;
	int nCodeType;        // 1=KS.Arch.1992  2=UBC.1991  3=UBC.1997  4=ATC3-06  5=KS2000, 6=JIS, 7=IBC2000
	char Description[80];  //8=EURO1996      9=CH2002
	T_SEIS_KS1992   KS1992;
	T_SEIS_UBC1991  UBC1991;
	T_SEIS_UBC1997  UBC1997;
	T_SEIS_ATC306   ATC306;
	T_SEIS_KS2000   KS2000;
	T_SEIS_JIS_RW   JP;
	T_SEIS_IBC2000  IBC2000;
	T_SEIS_EURO1996 EURO1996;   // added by KYM
	T_SEIS_CH2002   CH2002;     // added by KYM
	T_SEIS_NBC1995  NBC1995;
	T_SEIS_IS2002   IS2002;
	T_SEIS_TAIWAN86 TAIWAN86;
	T_SEIS_KBC2005  KBC2005;
	T_SEIS_CHSH2003 CHSH2003;
	T_SEIS_TAIWAN02 TAIWAN02;
	double ScaleFactor_X;
	double ScaleFactor_Y;
	int nEccen_X;
	int nEccen_Y;
	BOOL bInherentTorsionAmplify;
	BOOL bAccidentTorsionAmplify;
	int nCountAddition;
};

struct T_SEIS_UDRD_D_CH_RW_750
{
	T_SEIS_K key;
	int nCodeType;        // 1=KS.Arch.1992  2=UBC.1991  3=UBC.1997  4=ATC3-06  5=KS2000, 6=JIS, 7=IBC2000
	char Description[80];  //8=EURO1996      9=CH2002
	T_SEIS_KS1992   KS1992;
	T_SEIS_UBC1991  UBC1991;
	T_SEIS_UBC1997  UBC1997;
	T_SEIS_ATC306   ATC306;
	T_SEIS_KS2000   KS2000;
	T_SEIS_JIS_RW   JP;
	T_SEIS_IBC2000  IBC2000;
	T_SEIS_EURO1996 EURO1996;   // added by KYM
	T_SEIS_CH2002   CH2002;     // added by KYM
	T_SEIS_NBC1995  NBC1995;
	T_SEIS_IS2002   IS2002;
	T_SEIS_TAIWAN86 TAIWAN86;
	T_SEIS_KBC2005  KBC2005;
	T_SEIS_CHSH2003 CHSH2003;
	T_SEIS_TAIWAN02 TAIWAN02;
	T_SEIS_EURO2004_945 EURO2004;
	double ScaleFactor_X;
	double ScaleFactor_Y;
	int nEccen_X;
	int nEccen_Y;
	BOOL bInherentTorsionAmplify;
	BOOL bAccidentTorsionAmplify;
	int nCountAddition;
};

struct T_SEIS_UDRD_D_CH_RW_780
{
	T_SEIS_K key;
	int nCodeType;        // 1=KS.Arch.1992  2=UBC.1991  3=UBC.1997  4=ATC3-06  5=KS2000, 6=JIS, 7=IBC2000
	char Description[80];  //8=EURO1996      9=CH2002
	T_SEIS_KS1992   KS1992;
	T_SEIS_UBC1991  UBC1991;
	T_SEIS_UBC1997  UBC1997;
	T_SEIS_ATC306   ATC306;
	T_SEIS_KS2000   KS2000;
	T_SEIS_JIS_RW   JP;
	T_SEIS_IBC2000  IBC2000;
	T_SEIS_EURO1996 EURO1996;   // added by KYM
	T_SEIS_CH2002   CH2002;     // added by KYM
	T_SEIS_NBC1995  NBC1995;
	T_SEIS_IS2002   IS2002;
	T_SEIS_TAIWAN86 TAIWAN86;
	T_SEIS_KBC2005  KBC2005;
	T_SEIS_CHSH2003 CHSH2003;
	T_SEIS_TAIWAN02 TAIWAN02;
	T_SEIS_EURO2004_945 EURO2004;
	T_SEIS_KBC2009  KBC2009;
	double ScaleFactor_X;
	double ScaleFactor_Y;
	int nEccen_X;
	int nEccen_Y;
	BOOL bInherentTorsionAmplify;
	BOOL bAccidentTorsionAmplify;
	int nCountAddition;
};

struct T_SEIS_UDRD_D_CH_RW_791
{
	T_SEIS_K key;
	int nCodeType;        // 1=KS.Arch.1992  2=UBC.1991  3=UBC.1997  4=ATC3-06  5=KS2000, 6=JIS, 7=IBC2000
	char Description[80];  //8=EURO1996      9=CH2002
	T_SEIS_KS1992   KS1992;
	T_SEIS_UBC1991  UBC1991;
	T_SEIS_UBC1997  UBC1997;
	T_SEIS_ATC306   ATC306;
	T_SEIS_KS2000   KS2000;
	T_SEIS_JIS_RW   JP;
	T_SEIS_IBC2000  IBC2000;
	T_SEIS_EURO1996 EURO1996;   // added by KYM
	T_SEIS_CH2002   CH2002;     // added by KYM
	T_SEIS_NBC1995  NBC1995;
	T_SEIS_IS2002   IS2002;
	T_SEIS_TAIWAN86 TAIWAN86;
	T_SEIS_KBC2005  KBC2005;
	T_SEIS_CHSH2003 CHSH2003;
	T_SEIS_TAIWAN02 TAIWAN02;
	T_SEIS_EURO2004_945 EURO2004;
	T_SEIS_KBC2009  KBC2009;
	double ScaleFactor_X;
	double ScaleFactor_Y;
	double ScaleFactor_R;
	int nEccen_X;
	int nEccen_Y;
	BOOL bInherentTorsionAmplify;
	BOOL bAccidentTorsionAmplify;
	int nCountAddition;
};

struct T_SEIS_UDRD_D_CH_RW_795
{
	T_SEIS_K key;
	int nCodeType;        // 1=KS.Arch.1992  2=UBC.1991  3=UBC.1997  4=ATC3-06  5=KS2000, 6=JIS, 7=IBC2000
	char Description[80];  //8=EURO1996      9=CH2002
	T_SEIS_KS1992   KS1992;
	T_SEIS_UBC1991  UBC1991;
	T_SEIS_UBC1997  UBC1997;
	T_SEIS_ATC306   ATC306;
	T_SEIS_KS2000   KS2000;
	T_SEIS_JIS_RW   JP;
	T_SEIS_IBC2000  IBC2000;
	T_SEIS_EURO1996 EURO1996;   // added by KYM
	T_SEIS_CH2002   CH2002;     // added by KYM
	T_SEIS_NBC1995  NBC1995;
	T_SEIS_IS2002   IS2002;
	T_SEIS_TAIWAN86 TAIWAN86;
	T_SEIS_KBC2005  KBC2005;
	T_SEIS_CHSH2003 CHSH2003;
	T_SEIS_TAIWAN02 TAIWAN02;
	T_SEIS_EURO2004_945 EURO2004;
	T_SEIS_KBC2009  KBC2009;
	T_SEIS_TAIWAN02 TAIWAN11;
	double ScaleFactor_X;
	double ScaleFactor_Y;
	double ScaleFactor_R;
	int nEccen_X;
	int nEccen_Y;
	BOOL bInherentTorsionAmplify;
	BOOL bAccidentTorsionAmplify;
	int nCountAddition;
};

struct T_SEIS_UDRD_D_CH_RW_796
{
	T_SEIS_K key;
	int nCodeType;        // 1=KS.Arch.1992  2=UBC.1991  3=UBC.1997  4=ATC3-06  5=KS2000, 6=JIS, 7=IBC2000
	char Description[80];  //8=EURO1996      9=CH2002
	T_SEIS_KS1992   KS1992;
	T_SEIS_UBC1991  UBC1991;
	T_SEIS_UBC1997  UBC1997;
	T_SEIS_ATC306   ATC306;
	T_SEIS_KS2000   KS2000;
	T_SEIS_JIS_RW   JP;
	T_SEIS_IBC2000  IBC2000;
	T_SEIS_EURO1996 EURO1996;   // added by KYM
	T_SEIS_CH2002   CH2002;     // added by KYM
	T_SEIS_NBC1995  NBC1995;
	T_SEIS_IS2002   IS2002;
	T_SEIS_TAIWAN86 TAIWAN86;
	T_SEIS_KBC2005  KBC2005;
	T_SEIS_CHSH2003 CHSH2003;
	T_SEIS_TAIWAN02 TAIWAN02;
	T_SEIS_EURO2004_945 EURO2004;
	T_SEIS_KBC2009  KBC2009;
	T_SEIS_TAIWAN02 TAIWAN11;
	T_SEIS_NTC2008  NTC2008;
	double ScaleFactor_X;
	double ScaleFactor_Y;
	double ScaleFactor_R;
	int nEccen_X;
	int nEccen_Y;
	BOOL bInherentTorsionAmplify;
	BOOL bAccidentTorsionAmplify;
	int nCountAddition;
};

struct T_SEIS_UDRD_D_CH_RW_805
{
	T_SEIS_K key;
	int nCodeType;        // 1=KS.Arch.1992  2=UBC.1991  3=UBC.1997  4=ATC3-06  5=KS2000, 6=JIS, 7=IBC2000
	char Description[80];  //8=EURO1996      9=CH2002
	T_SEIS_KS1992   KS1992;
	T_SEIS_UBC1991  UBC1991;
	T_SEIS_UBC1997  UBC1997;
	T_SEIS_ATC306   ATC306;
	T_SEIS_KS2000   KS2000;
	T_SEIS_JIS_RW   JP;
	T_SEIS_IBC2000  IBC2000;
	T_SEIS_EURO1996 EURO1996;   // added by KYM
	T_SEIS_CH2002   CH2002;     // added by KYM
	T_SEIS_NBC1995  NBC1995;
	T_SEIS_IS2002   IS2002;
	T_SEIS_TAIWAN86 TAIWAN86;
	T_SEIS_KBC2005  KBC2005;
	T_SEIS_CHSH2003 CHSH2003;
	T_SEIS_TAIWAN02 TAIWAN02;
	T_SEIS_EURO2004_945 EURO2004;
	T_SEIS_KBC2009  KBC2009;
	T_SEIS_TAIWAN02 TAIWAN11;
	T_SEIS_NTC2008  NTC2008;
	T_SEIS_CH2010   CH2010;
	T_SEIS_IBC2012  IBC2009;
	T_SEIS_IBC2012  IBC2012;
	double ScaleFactor_X;
	double ScaleFactor_Y;
	double ScaleFactor_R;
	int nEccen_X;
	int nEccen_Y;
	BOOL bInherentTorsionAmplify;
	BOOL bAccidentTorsionAmplify;
	int nCountAddition;
};

struct T_SEIS_UDRD_D_CH_RW_845
{
	T_SEIS_K key;
	int nCodeType;        // 1=KS.Arch.1992  2=UBC.1991  3=UBC.1997  4=ATC3-06
	char Description[80];
	T_SEIS_KS1992   KS1992;
	T_SEIS_UBC1991  UBC1991;
	T_SEIS_UBC1997  UBC1997;
	T_SEIS_ATC306   ATC306;
	T_SEIS_KS2000   KS2000;
	T_SEIS_JIS_RW   JP;
	T_SEIS_IBC2000  IBC2000;
	T_SEIS_EURO1996 EURO1996;   // added by KYM
	T_SEIS_CH2002   CH2002;     // added by KYM
	T_SEIS_NBC1995  NBC1995;
	T_SEIS_IS2002   IS2002;
	T_SEIS_TAIWAN86 TAIWAN86;
	T_SEIS_KBC2005  KBC2005;
	T_SEIS_CHSH2003 CHSH2003;
	T_SEIS_TAIWAN02 TAIWAN02;
	T_SEIS_EURO2004_945 EURO2004;
	T_SEIS_KBC2009  KBC2009;
	T_SEIS_TAIWAN02 TAIWAN11;
	T_SEIS_NTC2008  NTC2008;
	T_SEIS_NTC2008  NTC2012;
	T_SEIS_CH2010   CH2010;
	T_SEIS_IBC2012  IBC2009;
	T_SEIS_IBC2012  IBC2012;
	double ScaleFactor_X;
	double ScaleFactor_Y;
	double ScaleFactor_R;
	int nEccen_X;
	int nEccen_Y;
	BOOL bInherentTorsionAmplify;
	BOOL bAccidentTorsionAmplify;
	int nCountAddition;
};

struct T_SEIS_UDRD_D_CH_RW_850
{
	T_SEIS_K key;
	int nCodeType;        // 1=KS.Arch.1992  2=UBC.1991  3=UBC.1997  4=ATC3-06  5=KS2000, 6=JIS, 7=IBC2000
	char Description[80];  //8=EURO1996      9=CH2002
	T_SEIS_KS1992   KS1992;
	T_SEIS_UBC1991  UBC1991;
	T_SEIS_UBC1997  UBC1997;
	T_SEIS_ATC306   ATC306;
	T_SEIS_KS2000   KS2000;
	T_SEIS_JIS_RW   JP;
	T_SEIS_IBC2000  IBC2000;
	T_SEIS_EURO1996 EURO1996;   // added by KYM
	T_SEIS_CH2002   CH2002;     // added by KYM
	T_SEIS_NBC1995  NBC1995;
	T_SEIS_IS2002   IS2002;
	T_SEIS_TAIWAN86 TAIWAN86;
	T_SEIS_KBC2005  KBC2005;
	T_SEIS_CHSH2003 CHSH2003;
	T_SEIS_TAIWAN02 TAIWAN02;
	T_SEIS_EURO2004_945 EURO2004;
	T_SEIS_KBC2009  KBC2009;
	T_SEIS_TAIWAN02 TAIWAN11;
	T_SEIS_NTC2008  NTC2008;
	T_SEIS_NTC2008  NTC2012;
	T_SEIS_CH2010   CH2010;
	T_SEIS_IBC2012  IBC2009;
	T_SEIS_IBC2012  IBC2012;
	T_SEIS_NSR2010_871  NSR2010;
	double ScaleFactor_X;
	double ScaleFactor_Y;
	double ScaleFactor_R;
	int nEccen_X;
	int nEccen_Y;
	BOOL bInherentTorsionAmplify;
	BOOL bAccidentTorsionAmplify;
	int nCountAddition;
};

struct T_SEIS_UDRD_D_CH_RW_860
{
	T_SEIS_K key;
	int nCodeType;        // 1=KS.Arch.1992  2=UBC.1991  3=UBC.1997  4=ATC3-06  5=KS2000, 6=JIS, 7=IBC2000
	char Description[80];  //8=EURO1996      9=CH2002
	T_SEIS_KS1992   KS1992;
	T_SEIS_UBC1991  UBC1991;
	T_SEIS_UBC1997  UBC1997;
	T_SEIS_ATC306   ATC306;
	T_SEIS_KS2000   KS2000;
	T_SEIS_JIS_RW   JP;
	T_SEIS_IBC2000  IBC2000;
	T_SEIS_EURO1996 EURO1996;   // added by KYM
	T_SEIS_CH2002   CH2002;     // added by KYM
	T_SEIS_NBC1995  NBC1995;
	T_SEIS_IS2002   IS2002;
	T_SEIS_TAIWAN86 TAIWAN86;
	T_SEIS_KBC2005  KBC2005;
	T_SEIS_CHSH2003 CHSH2003;
	T_SEIS_TAIWAN02 TAIWAN02;
	T_SEIS_EURO2004_945 EURO2004;
	T_SEIS_KBC2009  KBC2009;
	T_SEIS_TAIWAN02 TAIWAN11;
	T_SEIS_NTC2008  NTC2008;
	T_SEIS_NTC2008  NTC2012;
	T_SEIS_CH2010   CH2010;
	T_SEIS_IBC2012  IBC2009;
	T_SEIS_IBC2012  IBC2012;
	T_SEIS_NSR2010_871  NSR2010;
	T_SEIS_KBC2015  KBC2015;
	double ScaleFactor_X;
	double ScaleFactor_Y;
	double ScaleFactor_R;
	int nEccen_X;
	int nEccen_Y;
	BOOL bInherentTorsionAmplify;
	BOOL bAccidentTorsionAmplify;
	int nCountAddition;
};

struct T_SEIS_UDRD_D_CH_RW_865
{
	T_SEIS_K key;
	int nCodeType;        // 1=KS.Arch.1992  2=UBC.1991  3=UBC.1997  4=ATC3-06  5=KS2000, 6=JIS, 7=IBC2000
	char Description[80];  //8=EURO1996      9=CH2002
	T_SEIS_KS1992   KS1992;
	T_SEIS_UBC1991  UBC1991;
	T_SEIS_UBC1997  UBC1997;
	T_SEIS_ATC306   ATC306;
	T_SEIS_KS2000   KS2000;
	T_SEIS_JIS_RW   JP;
	T_SEIS_IBC2000  IBC2000;
	T_SEIS_EURO1996 EURO1996;   // added by KYM
	T_SEIS_CH2002   CH2002;     // added by KYM
	T_SEIS_NBC1995  NBC1995;
	T_SEIS_IS2002   IS2002;
	T_SEIS_TAIWAN86 TAIWAN86;
	T_SEIS_KBC2005  KBC2005;
	T_SEIS_CHSH2003 CHSH2003;
	T_SEIS_TAIWAN02 TAIWAN02;
	T_SEIS_EURO2004_945 EURO2004;
	T_SEIS_KBC2009  KBC2009;
	T_SEIS_TAIWAN02 TAIWAN11;
	T_SEIS_NTC2008  NTC2008;
	T_SEIS_NTC2008  NTC2012;
	T_SEIS_CH2010   CH2010;
	T_SEIS_IBC2012  IBC2009;
	T_SEIS_IBC2012  IBC2012;
	T_SEIS_NSR2010_871  NSR2010;
	T_SEIS_KBC2015  KBC2015;
	T_SEIS_P100_2013 P100_2013;

	double ScaleFactor_X;
	double ScaleFactor_Y;
	double ScaleFactor_R;
	int nEccen_X;
	int nEccen_Y;
	BOOL bInherentTorsionAmplify;
	BOOL bAccidentTorsionAmplify;
	int nCountAddition;
};

struct T_SEIS_UDRD_D_CH_RW_865_2
{
	T_SEIS_K key;
	int nCodeType;        // 1=KS.Arch.1992  2=UBC.1991  3=UBC.1997  4=ATC3-06  5=KS2000, 6=JIS, 7=IBC2000
	char Description[80];  //8=EURO1996      9=CH2002
	T_SEIS_KS1992   KS1992;
	T_SEIS_UBC1991  UBC1991;
	T_SEIS_UBC1997  UBC1997;
	T_SEIS_ATC306   ATC306;
	T_SEIS_KS2000   KS2000;
	T_SEIS_JIS_RW   JP;
	T_SEIS_IBC2000  IBC2000;
	T_SEIS_EURO1996 EURO1996;   // added by KYM
	T_SEIS_CH2002   CH2002;     // added by KYM
	T_SEIS_NBC1995  NBC1995;
	T_SEIS_IS2002   IS2002;
	T_SEIS_TAIWAN86 TAIWAN86;
	T_SEIS_KBC2005  KBC2005;
	T_SEIS_CHSH2003 CHSH2003;
	T_SEIS_TAIWAN02 TAIWAN02;
	T_SEIS_EURO2004_945 EURO2004;
	T_SEIS_KBC2009  KBC2009;
	T_SEIS_TAIWAN02 TAIWAN11;
	T_SEIS_NTC2008  NTC2008;
	T_SEIS_NTC2008  NTC2012;
	T_SEIS_CH2010   CH2010;
	T_SEIS_IBC2012  IBC2009;
	T_SEIS_IBC2012  IBC2012;
	T_SEIS_NSR2010_871  NSR2010;
	T_SEIS_KBC2015  KBC2015;
	T_SEIS_P100_2013 P100_2013;
	T_SEIS_IS1893_2016 IS1893_2016;

	double ScaleFactor_X;
	double ScaleFactor_Y;
	double ScaleFactor_R;
	int nEccen_X;
	int nEccen_Y;
	BOOL bInherentTorsionAmplify;
	BOOL bAccidentTorsionAmplify;
	int nCountAddition;
};

struct T_SEIS_UDRD_D_CH_RW_871
{
	T_SEIS_K key;
	int nCodeType;        // 1=KS.Arch.1992  2=UBC.1991  3=UBC.1997  4=ATC3-06  5=KS2000, 6=JIS, 7=IBC2000
	// 8=EURO1996      9=CH2002
	char Description[80];
	T_SEIS_KS1992   KS1992;
	T_SEIS_UBC1991  UBC1991;
	T_SEIS_UBC1997  UBC1997;
	T_SEIS_ATC306   ATC306;
	T_SEIS_KS2000   KS2000;
	T_SEIS_JIS_RW   JP;
	T_SEIS_IBC2000  IBC2000;
	T_SEIS_EURO1996 EURO1996;   // added by KYM
	T_SEIS_CH2002   CH2002;     // added by KYM
	T_SEIS_NBC1995  NBC1995;
	T_SEIS_IS2002   IS2002;
	T_SEIS_TAIWAN86 TAIWAN86;
	T_SEIS_KBC2005  KBC2005;
	T_SEIS_CHSH2003 CHSH2003;
	T_SEIS_TAIWAN02 TAIWAN02;
	T_SEIS_EURO2004_945 EURO2004;
	T_SEIS_KBC2009  KBC2009;
	T_SEIS_TAIWAN02 TAIWAN11;
	T_SEIS_NTC2008  NTC2008;
	T_SEIS_NTC2008  NTC2012;
	T_SEIS_CH2010   CH2010;
	T_SEIS_IBC2012  IBC2009;
	T_SEIS_IBC2012  IBC2012;
	T_SEIS_NSR2010_871  NSR2010;
	T_SEIS_KBC2015  KBC2015;
	T_SEIS_P100_2013 P100_2013;
	T_SEIS_IS1893_2016 IS1893_2016;
	T_SEIS_NTC2008  NTC2018;

	double ScaleFactor_X;
	double ScaleFactor_Y;
	double ScaleFactor_R;
	int nEccen_X;
	int nEccen_Y;
	BOOL bInherentTorsionAmplify;
	BOOL bAccidentTorsionAmplify;
	int nCountAddition;
};

struct T_SEIS_UDRD_D_CH_RW_883
{
	T_SEIS_K key;
	int nCodeType;  // 1=KS.Arch.1992  2=UBC.1991  3=UBC.1997  4=ATC3-06  5=KS2000, 6=JIS, 7=IBC2000  8=EURO1996  9=CH2002
	char Description[80];
	T_SEIS_KS1992   KS1992;
	T_SEIS_UBC1991  UBC1991;
	T_SEIS_UBC1997  UBC1997;
	T_SEIS_ATC306   ATC306;
	T_SEIS_KS2000   KS2000;
	T_SEIS_JIS_RW   JP;
	T_SEIS_IBC2000  IBC2000;
	T_SEIS_EURO1996 EURO1996;
	T_SEIS_CH2002   CH2002;
	T_SEIS_NBC1995  NBC1995;
	T_SEIS_IS2002   IS2002;
	T_SEIS_TAIWAN86 TAIWAN86;
	T_SEIS_KBC2005  KBC2005;
	T_SEIS_CHSH2003 CHSH2003;
	T_SEIS_TAIWAN02 TAIWAN02;
	T_SEIS_EURO2004_945 EURO2004;
	T_SEIS_KBC2009  KBC2009;
	T_SEIS_TAIWAN02 TAIWAN11;
	T_SEIS_NTC2008  NTC2008;
	T_SEIS_NTC2008  NTC2012;
	T_SEIS_CH2010   CH2010;
	T_SEIS_IBC2012  IBC2009;
	T_SEIS_IBC2012  IBC2012;
	T_SEIS_NSR2010  NSR2010;
	T_SEIS_KBC2015  KBC2015;
	T_SEIS_P100_2013 P100_2013;
	T_SEIS_IS1893_2016 IS1893_2016;
	T_SEIS_NTC2008  NTC2018;

	double ScaleFactor_X;
	double ScaleFactor_Y;
	double ScaleFactor_R;
	int nEccen_X;
	int nEccen_Y;
	BOOL bInherentTorsionAmplify;
	BOOL bAccidentTorsionAmplify;
	int nCountAddition;
};

struct T_SEIS_UDRD_D_CH_RW_915
{
	T_SEIS_K key;
	int nCodeType;
	char Description[80];
	T_SEIS_KS1992   KS1992;
	T_SEIS_UBC1991  UBC1991;
	T_SEIS_UBC1997  UBC1997;
	T_SEIS_ATC306   ATC306;
	T_SEIS_KS2000   KS2000;
	T_SEIS_JIS_RW   JP;
	T_SEIS_IBC2000  IBC2000;
	T_SEIS_EURO1996 EURO1996;
	T_SEIS_CH2002   CH2002;
	T_SEIS_NBC1995  NBC1995;
	T_SEIS_IS2002   IS2002;
	T_SEIS_TAIWAN86 TAIWAN86;
	T_SEIS_KBC2005  KBC2005;
	T_SEIS_CHSH2003 CHSH2003;
	T_SEIS_TAIWAN02 TAIWAN02;
	T_SEIS_EURO2004_945 EURO2004;
	T_SEIS_KBC2009  KBC2009;
	T_SEIS_TAIWAN02 TAIWAN11;
	T_SEIS_NTC2008  NTC2008;
	T_SEIS_NTC2008  NTC2012;
	T_SEIS_CH2010   CH2010;
	T_SEIS_IBC2012  IBC2009;
	T_SEIS_IBC2012  IBC2012;
	T_SEIS_NSR2010  NSR2010;
	T_SEIS_KBC2015  KBC2015;
	T_SEIS_P100_2013 P100_2013;
	T_SEIS_IS1893_2016 IS1893_2016;
	T_SEIS_NTC2008  NTC2018;
	T_SEIS_KDS2019  KDS2019;
	double ScaleFactor_X;
	double ScaleFactor_Y;
	double ScaleFactor_R;
	int nEccen_X;
	int nEccen_Y;
	BOOL bInherentTorsionAmplify;
	BOOL bAccidentTorsionAmplify;
	int nCountAddition;
};

struct T_SEIS_UDRD_D_CH_RW_920
{
	T_SEIS_K key;
	int nCodeType;
	char Description[80];
	T_SEIS_KS1992   KS1992;
	T_SEIS_UBC1991  UBC1991;
	T_SEIS_UBC1997  UBC1997;
	T_SEIS_ATC306   ATC306;
	T_SEIS_KS2000   KS2000;
	T_SEIS_JIS_RW   JP;
	T_SEIS_IBC2000  IBC2000;
	T_SEIS_EURO1996 EURO1996;
	T_SEIS_CH2002   CH2002;
	T_SEIS_NBC1995  NBC1995;
	T_SEIS_IS2002   IS2002;
	T_SEIS_TAIWAN86 TAIWAN86;
	T_SEIS_KBC2005  KBC2005;
	T_SEIS_CHSH2003 CHSH2003;
	T_SEIS_TAIWAN02 TAIWAN02;
	T_SEIS_EURO2004_945 EURO2004;
	T_SEIS_KBC2009  KBC2009;
	T_SEIS_TAIWAN02 TAIWAN11;
	T_SEIS_NTC2008  NTC2008;
	T_SEIS_NTC2008  NTC2012;
	T_SEIS_CH2010   CH2010;
	T_SEIS_IBC2012  IBC2009;
	T_SEIS_IBC2012  IBC2012;
	T_SEIS_NSR2010  NSR2010;
	T_SEIS_KBC2015  KBC2015;
	T_SEIS_P100_2013 P100_2013;
	T_SEIS_IS1893_2016 IS1893_2016;
	T_SEIS_NTC2008  NTC2018;
	T_SEIS_KDS2019  KDS2019;
	T_SEIS_DPT2018  DPT2018;

	double ScaleFactor_X;
	double ScaleFactor_Y;
	double ScaleFactor_R;
	int nEccen_X;
	int nEccen_Y;
	BOOL bInherentTorsionAmplify;
	BOOL bAccidentTorsionAmplify;
	int nCountAddition;
};

struct T_SEIS_UDRD_D_CH_RW_920_1
{
	T_SEIS_K key;
	int nCodeType;
	char Description[80];
	T_SEIS_KS1992   KS1992;
	T_SEIS_UBC1991  UBC1991;
	T_SEIS_UBC1997  UBC1997;
	T_SEIS_ATC306   ATC306;
	T_SEIS_KS2000   KS2000;
	T_SEIS_JIS_RW   JP;
	T_SEIS_IBC2000  IBC2000;
	T_SEIS_EURO1996 EURO1996;
	T_SEIS_CH2002   CH2002;
	T_SEIS_NBC1995  NBC1995;
	T_SEIS_IS2002   IS2002;
	T_SEIS_TAIWAN86 TAIWAN86;
	T_SEIS_KBC2005  KBC2005;
	T_SEIS_CHSH2003 CHSH2003;
	T_SEIS_TAIWAN02 TAIWAN02;
	T_SEIS_EURO2004_945 EURO2004;
	T_SEIS_KBC2009  KBC2009;
	T_SEIS_TAIWAN02 TAIWAN11;
	T_SEIS_NTC2008  NTC2008;
	T_SEIS_NTC2008  NTC2012;
	T_SEIS_CH2010   CH2010;
	T_SEIS_IBC2012  IBC2009;
	T_SEIS_IBC2012  IBC2012;
	T_SEIS_NSR2010  NSR2010;
	T_SEIS_KBC2015  KBC2015;
	T_SEIS_P100_2013 P100_2013;
	T_SEIS_IS1893_2016 IS1893_2016;
	T_SEIS_NTC2008  NTC2018;
	T_SEIS_KDS2019  KDS2019;
	T_SEIS_DPT2018  DPT2018;
	T_SEIS_TAIWAN02 TAIWAN22;

	double ScaleFactor_X;
	double ScaleFactor_Y;
	double ScaleFactor_R;
	int nEccen_X;
	int nEccen_Y;
	BOOL bInherentTorsionAmplify;
	BOOL bAccidentTorsionAmplify;
	int nCountAddition;
};

struct T_SEIS_UDRD_D_CH_RW_930
{
	T_SEIS_K key;
	int nCodeType;
	char Description[80];
	T_SEIS_KS1992   KS1992;
	T_SEIS_UBC1991  UBC1991;
	T_SEIS_UBC1997  UBC1997;
	T_SEIS_ATC306   ATC306;
	T_SEIS_KS2000   KS2000;
	T_SEIS_JIS_RW   JP;
	T_SEIS_IBC2000  IBC2000;
	T_SEIS_EURO1996 EURO1996;
	T_SEIS_CH2002   CH2002;
	T_SEIS_NBC1995  NBC1995;
	T_SEIS_IS2002   IS2002;
	T_SEIS_TAIWAN86 TAIWAN86;
	T_SEIS_KBC2005  KBC2005;
	T_SEIS_CHSH2003 CHSH2003;
	T_SEIS_TAIWAN02 TAIWAN02;
	T_SEIS_EURO2004_945 EURO2004;
	T_SEIS_KBC2009  KBC2009;
	T_SEIS_TAIWAN02 TAIWAN11;
	T_SEIS_NTC2008  NTC2008;
	T_SEIS_NTC2008  NTC2012;
	T_SEIS_CH2010   CH2010;
	T_SEIS_IBC2012  IBC2009;
	T_SEIS_IBC2012  IBC2012;
	T_SEIS_NSR2010  NSR2010;
	T_SEIS_KBC2015  KBC2015;
	T_SEIS_P100_2013 P100_2013;
	T_SEIS_IS1893_2016 IS1893_2016;
	T_SEIS_NTC2008  NTC2018;
	T_SEIS_KDS2019  KDS2019;
	T_SEIS_DPT2018  DPT2018;
	T_SEIS_TAIWAN22 TAIWAN22;

	double ScaleFactor_X;
	double ScaleFactor_Y;
	double ScaleFactor_R;
	int nEccen_X;
	int nEccen_Y;
	BOOL bInherentTorsionAmplify;
	BOOL bAccidentTorsionAmplify;
	int nCountAddition;
};

struct T_SEIS_UDRD_D_CH_RW_945
{
	T_SEIS_K key;
	int nCodeType;
	char Description[80];
	T_SEIS_KS1992   KS1992;
	T_SEIS_UBC1991  UBC1991;
	T_SEIS_UBC1997  UBC1997;
	T_SEIS_ATC306   ATC306;
	T_SEIS_KS2000   KS2000;
	T_SEIS_JIS_RW   JP;
	T_SEIS_IBC2000  IBC2000;
	T_SEIS_EURO1996 EURO1996;
	T_SEIS_CH2002   CH2002;
	T_SEIS_NBC1995  NBC1995;
	T_SEIS_IS2002   IS2002;
	T_SEIS_TAIWAN86 TAIWAN86;
	T_SEIS_KBC2005  KBC2005;
	T_SEIS_CHSH2003 CHSH2003;
	T_SEIS_TAIWAN02 TAIWAN02;
	T_SEIS_EURO2004_945 EURO2004;
	T_SEIS_KBC2009  KBC2009;
	T_SEIS_TAIWAN02 TAIWAN11;
	T_SEIS_NTC2008  NTC2008;
	T_SEIS_NTC2008  NTC2012;
	T_SEIS_CH2010   CH2010;
	T_SEIS_IBC2012  IBC2009;
	T_SEIS_IBC2012  IBC2012;
	T_SEIS_NSR2010  NSR2010;
	T_SEIS_KBC2015  KBC2015;
	T_SEIS_P100_2013 P100_2013;
	T_SEIS_IS1893_2016 IS1893_2016;
	T_SEIS_NTC2008  NTC2018;
	T_SEIS_KDS2019  KDS2019;
	T_SEIS_DPT2018  DPT2018;
	T_SEIS_TAIWAN22 TAIWAN22;

	double ScaleFactor_X;
	double ScaleFactor_Y;
	double ScaleFactor_R;
	int nEccen_X;
	int nEccen_Y;
	BOOL bInherentTorsionAmplify;
	BOOL bAccidentTorsionAmplify;
	int nCountAddition;
	int nCountUser;
};

struct T_SEIS_UDRD_D_CH_RW_950
{
	T_SEIS_K key;
	int nCodeType;
	char Description[80];
	T_SEIS_KS1992   KS1992;
	T_SEIS_UBC1991  UBC1991;
	T_SEIS_UBC1997  UBC1997;
	T_SEIS_ATC306   ATC306;
	T_SEIS_KS2000   KS2000;
	T_SEIS_JIS_RW   JP;
	T_SEIS_IBC2000  IBC2000;
	T_SEIS_EURO1996 EURO1996;
	T_SEIS_CH2002   CH2002;
	T_SEIS_NBC1995  NBC1995;
	T_SEIS_IS2002   IS2002;
	T_SEIS_TAIWAN86 TAIWAN86;
	T_SEIS_KBC2005  KBC2005;
	T_SEIS_CHSH2003 CHSH2003;
	T_SEIS_TAIWAN02 TAIWAN02;
	T_SEIS_EURO2004 EURO2004;
	T_SEIS_KBC2009  KBC2009;
	T_SEIS_TAIWAN02 TAIWAN11;
	T_SEIS_NTC2008  NTC2008;
	T_SEIS_NTC2008  NTC2012;
	T_SEIS_CH2010   CH2010;
	T_SEIS_IBC2012  IBC2009;
	T_SEIS_IBC2012  IBC2012;
	T_SEIS_NSR2010  NSR2010;
	T_SEIS_KBC2015  KBC2015;
	T_SEIS_P100_2013 P100_2013;
	T_SEIS_IS1893_2016 IS1893_2016;
	T_SEIS_NTC2008  NTC2018;
	T_SEIS_KDS2019  KDS2019;
	T_SEIS_DPT2018  DPT2018;
	T_SEIS_TAIWAN22 TAIWAN22;
	
	double ScaleFactor_X;
	double ScaleFactor_Y;
	double ScaleFactor_R;
	int nEccen_X;
	int nEccen_Y;
	BOOL bInherentTorsionAmplify;
	BOOL bAccidentTorsionAmplify;
	int nCountAddition;
	int nCountUser;
};

struct T_SEIS_UDRD_D_CH_RW_960
{
	T_SEIS_K key;
	int nCodeType;
	char Description[80];
	T_SEIS_KS1992   KS1992;
	T_SEIS_UBC1991  UBC1991;
	T_SEIS_UBC1997  UBC1997;
	T_SEIS_ATC306   ATC306;
	T_SEIS_KS2000   KS2000;
	T_SEIS_JIS_RW   JP;
	T_SEIS_IBC2000  IBC2000;
	T_SEIS_EURO1996 EURO1996;
	T_SEIS_CH2002   CH2002;
	T_SEIS_NBC1995  NBC1995;
	T_SEIS_IS2002   IS2002;
	T_SEIS_TAIWAN86 TAIWAN86;
	T_SEIS_KBC2005  KBC2005;
	T_SEIS_CHSH2003 CHSH2003;
	T_SEIS_TAIWAN02 TAIWAN02;
	T_SEIS_EURO2004 EURO2004;
	T_SEIS_KBC2009  KBC2009;
	T_SEIS_TAIWAN02 TAIWAN11;
	T_SEIS_NTC2008  NTC2008;
	T_SEIS_NTC2008  NTC2012;
	T_SEIS_CH2010   CH2010;
	T_SEIS_IBC2012  IBC2009;
	T_SEIS_IBC2012  IBC2012;
	T_SEIS_NSR2010  NSR2010;
	T_SEIS_KBC2015  KBC2015;
	T_SEIS_P100_2013 P100_2013;
	T_SEIS_IS1893_2016 IS1893_2016;
	T_SEIS_NTC2008  NTC2018;
	T_SEIS_KDS2019  KDS2019;
	T_SEIS_DPT2018  DPT2018;
	T_SEIS_TAIWAN22 TAIWAN22;
	T_SEIS_IBC2012  NSCP2024;
	T_SEIS_SANS2010 SANS2010;

	double ScaleFactor_X;
	double ScaleFactor_Y;
	double ScaleFactor_R;
	int nEccen_X;
	int nEccen_Y;
	BOOL bInherentTorsionAmplify;
	BOOL bAccidentTorsionAmplify;
	int nCountAddition;
	int nCountUser;
};

struct T_SEIS_UDRD_D_CH_RW
{
	T_SEIS_K key;
	int nCodeType;
	char Description[80];
	T_SEIS_KS1992   KS1992;
	T_SEIS_UBC1991  UBC1991;
	T_SEIS_UBC1997  UBC1997;
	T_SEIS_ATC306   ATC306;
	T_SEIS_KS2000   KS2000;
	T_SEIS_JIS_RW   JP;
	T_SEIS_IBC2000  IBC2000;
	T_SEIS_EURO1996 EURO1996;
	T_SEIS_CH2002   CH2002;
	T_SEIS_NBC1995  NBC1995;
	T_SEIS_IS2002   IS2002;
	T_SEIS_TAIWAN86 TAIWAN86;
	T_SEIS_KBC2005  KBC2005;
	T_SEIS_CHSH2003 CHSH2003;
	T_SEIS_TAIWAN02 TAIWAN02;
	T_SEIS_EURO2004 EURO2004;
	T_SEIS_KBC2009  KBC2009;
	T_SEIS_TAIWAN02 TAIWAN11;
	T_SEIS_NTC2008  NTC2008;
	T_SEIS_NTC2008  NTC2012;
	T_SEIS_CH2010   CH2010;
	T_SEIS_IBC2012  IBC2009;
	T_SEIS_IBC2012  IBC2012;
	T_SEIS_NSR2010  NSR2010;
	T_SEIS_KBC2015  KBC2015;
	T_SEIS_P100_2013 P100_2013;
	T_SEIS_IS1893_2016 IS1893_2016;
	T_SEIS_NTC2008  NTC2018;
	T_SEIS_KDS2019  KDS2019;
	T_SEIS_DPT2018  DPT2018;
	T_SEIS_TAIWAN22 TAIWAN22;
	T_SEIS_IBC2012  NSCP2024;
	T_SEIS_SANS2010 SANS2010;
	T_SEIS_IS1893_2025 IS2025;

	double ScaleFactor_X;
	double ScaleFactor_Y;
	double ScaleFactor_R;
	int nEccen_X;
	int nEccen_Y;
	BOOL bInherentTorsionAmplify;
	BOOL bAccidentTorsionAmplify;
	int nCountAddition;
	int nCountUser;
	void Initialize()
	{
		key=0;
		nCodeType=0;
		memset(Description, ' ', 80);

		KS1992.Initialize();
		UBC1991.Initialize();
		UBC1997.Initialize();
		ATC306.Initialize();
		KS2000.Initialize();
		JP.Initialize();
		IBC2000.Initialize();
		EURO1996.Initialize();  // added by KYM
		CH2002.Initialize();    // added by KYM
		NBC1995.Initialize();
		IS2002.Initialize();
		TAIWAN86.Initialize();
		KBC2005.Initialize();
		CHSH2003.Initialize();
		TAIWAN02.Initialize();
		EURO2004.Initialize();
		KBC2009.Initialize();
		TAIWAN11.Initialize();
		NTC2008.Initialize();
		NTC2012.Initialize();
		CH2010.Initialize();
		IBC2009.Initialize();
		IBC2012.Initialize();
		NSR2010.Initialize();
		KBC2015.Initialize();
		P100_2013.Initialize();
		IS1893_2016.Initialize();
		NTC2018.Initialize();
		KDS2019.Initialize();
		DPT2018.Init();
		TAIWAN22.Initialize();
		NSCP2024.Initialize();
		SANS2010.Initialize();
		IS2025.Initialize();

		ScaleFactor_X=0.;
		ScaleFactor_Y=0.;
		ScaleFactor_R=0.;
		nEccen_X = 0;
		nEccen_Y = 0;
		bInherentTorsionAmplify = FALSE;
		bAccidentTorsionAmplify = FALSE;
		nCountAddition=0;
		nCountUser = 0;
	}
	void GetSeis(T_SEIS_K& rKey, T_SEIS_D_CH& rData)
	{
		rKey=key;
		rData.nCodeType=nCodeType;
		memcpy(rData.Description, Description, sizeof(Description));

		rData.KS1992       = KS1992;
		rData.UBC1991      = UBC1991;
		rData.UBC1997      = UBC1997;
		rData.ATC306       = ATC306;
		rData.KS2000       = KS2000;
		JP.Get(rData.JP);
		rData.IBC2000      = IBC2000;
		rData.EURO1996     = EURO1996;   // added by KYM
		rData.CH2002       = CH2002;
		rData.NBC1995      = NBC1995;
		rData.IS2002       = IS2002;
		rData.TAIWAN86     = TAIWAN86;
		rData.KBC2005      = KBC2005;
		rData.CHSH2003     = CHSH2003;
		rData.TAIWAN02     = TAIWAN02;
		rData.EURO2004     = EURO2004;
		rData.KBC2009      = KBC2009;
		rData.TAIWAN11     = TAIWAN11;
		rData.NTC2008      = NTC2008;
		rData.NTC2012      = NTC2012;
		rData.CH2010       = CH2010;
		rData.IBC2009      = IBC2009;
		rData.IBC2012      = IBC2012;
		rData.NSR2010      = NSR2010;
		rData.KBC2015      = KBC2015;
		rData.P100_2013    = P100_2013;
		rData.IS1893_2016  = IS1893_2016;
		rData.NTC2018      = NTC2018;
		rData.KDS2019      = KDS2019;
		rData.DPT2018      = DPT2018;
		rData.TAIWAN22     = TAIWAN22;
		rData.NSCP2024     = NSCP2024;
		rData.SANS2010	   = SANS2010;
		rData.IS2025	   = IS2025;

		rData.ScaleFactor_X=ScaleFactor_X;
		rData.ScaleFactor_Y=ScaleFactor_Y;
		rData.ScaleFactor_R=ScaleFactor_R;
		rData.nEccen_X=nEccen_X;
		rData.nEccen_Y=nEccen_Y;
		rData.bInherentTorsionAmplify = bInherentTorsionAmplify;
		rData.bAccidentTorsionAmplify = bAccidentTorsionAmplify;
	}
	void SetSeis(const T_SEIS_K Key, const T_SEIS_D_CH& rData)
	{
		key=Key;
		nCodeType=rData.nCodeType;
		memcpy(Description, rData.Description, sizeof(Description));

		KS1992       = rData.KS1992;
		UBC1991      = rData.UBC1991;
		UBC1997      = rData.UBC1997;
		ATC306       = rData.ATC306;
		KS2000       = rData.KS2000;
		JP.Set(rData.JP);
		IBC2000      = rData.IBC2000;
		EURO1996     = rData.EURO1996; // added by KYM
		CH2002       = rData.CH2002;   // added by KYM
		NBC1995      = rData.NBC1995;
		IS2002       = rData.IS2002;
		TAIWAN86     = rData.TAIWAN86;
		KBC2005      = rData.KBC2005;
		CHSH2003     = rData.CHSH2003;
		TAIWAN02     = rData.TAIWAN02;
		EURO2004     = rData.EURO2004;
		KBC2009      = rData.KBC2009;
		TAIWAN11     = rData.TAIWAN11;
		NTC2008      = rData.NTC2008;
		NTC2012      = rData.NTC2012;
		CH2010       = rData.CH2010;
		IBC2009      = rData.IBC2009;
		IBC2012      = rData.IBC2012;
		NSR2010      = rData.NSR2010;
		KBC2015      = rData.KBC2015;
		P100_2013    = rData.P100_2013;
		IS1893_2016  = rData.IS1893_2016;
		NTC2018      = rData.NTC2018;
		KDS2019      = rData.KDS2019;
		DPT2018      = rData.DPT2018;
		TAIWAN22     = rData.TAIWAN22;
		NSCP2024     = rData.NSCP2024;
		SANS2010	 = rData.SANS2010;
		IS2025       = rData.IS2025;

		ScaleFactor_X=rData.ScaleFactor_X;
		ScaleFactor_Y=rData.ScaleFactor_Y;
		ScaleFactor_R=rData.ScaleFactor_R;
		nEccen_X=rData.nEccen_X;
		nEccen_Y=rData.nEccen_Y;
		bInherentTorsionAmplify = rData.bInherentTorsionAmplify;
		bAccidentTorsionAmplify = rData.bAccidentTorsionAmplify;
		nCountAddition = (int)rData.arAddition.GetSize();
		nCountUser = (int)rData.arUser.GetSize();
	}
	void Convert412(T_SEIS_UDRD_D_CH_RW_412& rData)
	{
		key=rData.key;
		nCodeType=rData.nCodeType;
		memcpy(Description, rData.Description, sizeof(Description));
		KS1992=rData.KS1992;
		UBC1991=rData.UBC1991;
		UBC1997.Convert640(rData.UBC1997);
		ATC306=rData.ATC306;
		ScaleFactor_X=rData.ScaleFactor_X;
		ScaleFactor_Y=rData.ScaleFactor_Y;
		if ( rData.bEccenPositive_X == TRUE ) nEccen_X = 0;
		else nEccen_X = 1;
		if ( rData.bEccenPositive_Y == TRUE ) nEccen_Y = 0;
		else nEccen_Y = 1;
		bInherentTorsionAmplify = FALSE;
		bAccidentTorsionAmplify = FALSE;
		nCountAddition=rData.nCountAddition;
	}
	void Convert422(T_SEIS_UDRD_D_CH_RW_422& rData)
	{
		Initialize();
		key=rData.key;
		nCodeType=rData.nCodeType;
		memcpy(Description, rData.Description, sizeof(Description));
		KS1992=rData.KS1992;
		UBC1991=rData.UBC1991;
		UBC1997.Convert640(rData.UBC1997);
		ATC306=rData.ATC306;
		KS2000=rData.KS2000;
		ScaleFactor_X=rData.ScaleFactor_X;
		ScaleFactor_Y=rData.ScaleFactor_Y;
		if ( rData.bEccenPositive_X == TRUE ) nEccen_X = 0;
		else nEccen_X = 1;
		if ( rData.bEccenPositive_Y == TRUE ) nEccen_Y = 0;
		else nEccen_Y = 1;
		bInherentTorsionAmplify = FALSE;
		bAccidentTorsionAmplify = FALSE;
		nCountAddition=rData.nCountAddition;
	}
	void Convert423(T_SEIS_UDRD_D_CH_RW_423& rData)
	{
		Initialize();
		key=rData.key;
		nCodeType=rData.nCodeType;
		memcpy(Description, rData.Description, sizeof(Description));
		KS1992=rData.KS1992;
		UBC1991=rData.UBC1991;
		UBC1997.Convert640(rData.UBC1997);
		ATC306=rData.ATC306;
		KS2000=rData.KS2000;
		JP.Convert610(rData.JP);
		ScaleFactor_X=rData.ScaleFactor_X;
		ScaleFactor_Y=rData.ScaleFactor_Y;
		if ( rData.bEccenPositive_X == TRUE ) nEccen_X = 0;
		else nEccen_X = 1;
		if ( rData.bEccenPositive_Y == TRUE ) nEccen_Y = 0;
		else nEccen_Y = 1;
		bInherentTorsionAmplify = FALSE;
		bAccidentTorsionAmplify = FALSE;
		nCountAddition=rData.nCountAddition;
	}
	void Convert520(T_SEIS_UDRD_D_CH_RW_520& rData)  // added by KYM
	{
		Initialize();
		key=rData.key;
		nCodeType=rData.nCodeType;
		memcpy(Description, rData.Description, sizeof(Description));
		KS1992=rData.KS1992;
		UBC1991=rData.UBC1991;
		UBC1997.Convert640(rData.UBC1997);
		ATC306=rData.ATC306;
		KS2000=rData.KS2000;
		JP.Convert610(rData.JP);
		IBC2000=rData.IBC2000;
		ScaleFactor_X=rData.ScaleFactor_X;
		ScaleFactor_Y=rData.ScaleFactor_Y;
		if ( rData.bEccenPositive_X == TRUE ) nEccen_X = 0;
		else nEccen_X = 1;
		if ( rData.bEccenPositive_Y == TRUE ) nEccen_Y = 0;
		else nEccen_Y = 1;
		bInherentTorsionAmplify = FALSE;
		bAccidentTorsionAmplify = FALSE;
		nCountAddition=rData.nCountAddition;
	}
	void Convert550(T_SEIS_UDRD_D_CH_RW_550& rData)  // added by KYM
	{
		Initialize();
		key=rData.key;
		nCodeType=rData.nCodeType;
		memcpy(Description, rData.Description, sizeof(Description));
		KS1992=rData.KS1992;
		UBC1991=rData.UBC1991;
		UBC1997.Convert640(rData.UBC1997);
		ATC306=rData.ATC306;
		KS2000=rData.KS2000;
		JP.Convert610(rData.JP);
		IBC2000=rData.IBC2000;
		EURO1996=rData.EURO1996;
		CH2002=rData.CH2002;
		ScaleFactor_X=rData.ScaleFactor_X;
		ScaleFactor_Y=rData.ScaleFactor_Y;
		if ( rData.bEccenPositive_X == TRUE ) nEccen_X = 0;
		else nEccen_X = 1;
		if ( rData.bEccenPositive_Y == TRUE ) nEccen_Y = 0;
		else nEccen_Y = 1;
		bInherentTorsionAmplify = FALSE;
		bAccidentTorsionAmplify = FALSE;
		nCountAddition=rData.nCountAddition;
	}
	void Convert590(T_SEIS_UDRD_D_CH_RW_590& rData)  // added by KYM
	{
		Initialize();
		key=rData.key;
		nCodeType=rData.nCodeType;
		memcpy(Description, rData.Description, sizeof(Description));
		KS1992=rData.KS1992;
		UBC1991=rData.UBC1991;
		UBC1997.Convert640(rData.UBC1997);
		ATC306=rData.ATC306;
		KS2000=rData.KS2000;
		JP.Convert610(rData.JP);
		IBC2000=rData.IBC2000;
		EURO1996=rData.EURO1996;
		CH2002=rData.CH2002;
		NBC1995 = rData.NBC1995;
		ScaleFactor_X=rData.ScaleFactor_X;
		ScaleFactor_Y=rData.ScaleFactor_Y;
		if ( rData.bEccenPositive_X == TRUE ) nEccen_X = 0;
		else nEccen_X = 1;
		if ( rData.bEccenPositive_Y == TRUE ) nEccen_Y = 0;
		else nEccen_Y = 1;
		bInherentTorsionAmplify = FALSE;
		bAccidentTorsionAmplify = FALSE;
		nCountAddition=rData.nCountAddition;
	}
	void Convert600(T_SEIS_UDRD_D_CH_RW_600& rData)  // added by KYM
	{
		Initialize();
		key=rData.key;
		nCodeType=rData.nCodeType;
		memcpy(Description, rData.Description, sizeof(Description));
		KS1992=rData.KS1992;
		UBC1991=rData.UBC1991;
		UBC1997.Convert640(rData.UBC1997);
		ATC306=rData.ATC306;
		KS2000=rData.KS2000;
		JP.Convert610(rData.JP);
		IBC2000=rData.IBC2000;
		EURO1996=rData.EURO1996;
		CH2002=rData.CH2002;
		NBC1995 = rData.NBC1995;
		IS2002 = rData.IS2002;
		TAIWAN86.Initialize();
		KBC2005.Initialize();
		CHSH2003.Initialize();
		TAIWAN02.Initialize();
		EURO2004.Initialize();
		KBC2009.Initialize();
		CH2010.Initialize();
		NTC2012.Initialize();
		TAIWAN11.Initialize();
		NTC2008.Initialize();
		IBC2009.Initialize();
		IBC2012.Initialize();
		NSR2010.Initialize();
		P100_2013.Initialize();
		IS1893_2016.Initialize();

		ScaleFactor_X=rData.ScaleFactor_X;
		ScaleFactor_Y=rData.ScaleFactor_Y;
		nEccen_X = rData.nEccen_X;
		nEccen_Y = rData.nEccen_Y;
		bInherentTorsionAmplify = FALSE;
		bAccidentTorsionAmplify = FALSE;
		nCountAddition=rData.nCountAddition;
	}
	void Convert610(T_SEIS_UDRD_D_CH_RW_610& rData)  // added by KYM
	{
		Initialize();
		key=rData.key;
		nCodeType=rData.nCodeType;
		memcpy(Description, rData.Description, sizeof(Description));
		KS1992=rData.KS1992;
		UBC1991=rData.UBC1991;
		UBC1997.Convert640(rData.UBC1997);
		ATC306=rData.ATC306;
		KS2000=rData.KS2000;
		JP.Convert610(rData.JP);
		IBC2000=rData.IBC2000;
		EURO1996=rData.EURO1996;
		CH2002=rData.CH2002;
		NBC1995 = rData.NBC1995;
		IS2002 = rData.IS2002;
		TAIWAN86 = rData.TAIWAN86;
		KBC2005.Initialize();
		CHSH2003.Initialize();
		TAIWAN02.Initialize();
		EURO2004.Initialize();
		KBC2009.Initialize();
		CH2010.Initialize();
		NTC2012.Initialize();
		TAIWAN11.Initialize();
		NTC2008.Initialize();
		IBC2009.Initialize();
		IBC2012.Initialize();
		NSR2010.Initialize();
		P100_2013.Initialize();
		IS1893_2016.Initialize();

		ScaleFactor_X=rData.ScaleFactor_X;
		ScaleFactor_Y=rData.ScaleFactor_Y;
		nEccen_X = rData.nEccen_X;
		nEccen_Y = rData.nEccen_Y;
		bInherentTorsionAmplify = FALSE;
		bAccidentTorsionAmplify = FALSE;
		nCountAddition=rData.nCountAddition;
	}
	void Convert640(T_SEIS_UDRD_D_CH_RW_640& rData)  // added by KYM
	{
		Initialize();
		key=rData.key;
		nCodeType=rData.nCodeType;
		memcpy(Description, rData.Description, sizeof(Description));
		KS1992=rData.KS1992;
		UBC1991=rData.UBC1991;
		UBC1997.Convert640(rData.UBC1997);
		ATC306=rData.ATC306;
		KS2000=rData.KS2000;
		JP=rData.JP;
		IBC2000=rData.IBC2000;
		EURO1996=rData.EURO1996;
		CH2002=rData.CH2002;
		NBC1995 = rData.NBC1995;
		IS2002 = rData.IS2002;
		TAIWAN86 = rData.TAIWAN86;
		KBC2005.Initialize();
		CHSH2003.Initialize();
		TAIWAN02.Initialize();
		EURO2004.Initialize();
		KBC2009.Initialize();
		CH2010.Initialize();
		NTC2012.Initialize();
		TAIWAN11.Initialize();
		NTC2008.Initialize();
		IBC2009.Initialize();
		IBC2012.Initialize();
		NSR2010.Initialize();
		P100_2013.Initialize();
		IS1893_2016.Initialize();

		ScaleFactor_X=rData.ScaleFactor_X;
		ScaleFactor_Y=rData.ScaleFactor_Y;
		nEccen_X = rData.nEccen_X;
		nEccen_Y = rData.nEccen_Y;
		bInherentTorsionAmplify = FALSE;
		bAccidentTorsionAmplify = FALSE;
		nCountAddition=rData.nCountAddition;
	}
	void Convert670(T_SEIS_UDRD_D_CH_RW_670& rData)  // added by KYM
	{
		Initialize();
		key=rData.key;
		nCodeType=rData.nCodeType;
		memcpy(Description, rData.Description, sizeof(Description));
		KS1992=rData.KS1992;
		UBC1991=rData.UBC1991;
		UBC1997=rData.UBC1997;
		ATC306=rData.ATC306;
		KS2000=rData.KS2000;
		JP=rData.JP;
		IBC2000=rData.IBC2000;
		EURO1996=rData.EURO1996;
		CH2002=rData.CH2002;
		NBC1995 = rData.NBC1995;
		IS2002 = rData.IS2002;
		TAIWAN86 = rData.TAIWAN86;
		KBC2005.Initialize();
		CHSH2003.Initialize();
		TAIWAN02.Initialize();
		EURO2004.Initialize();
		KBC2009.Initialize();
		CH2010.Initialize();
		NTC2012.Initialize();
		TAIWAN11.Initialize();
		NTC2008.Initialize();
		IBC2009.Initialize();
		IBC2012.Initialize();
		NSR2010.Initialize();
		P100_2013.Initialize();
		IS1893_2016.Initialize();

		ScaleFactor_X=rData.ScaleFactor_X;
		ScaleFactor_Y=rData.ScaleFactor_Y;
		nEccen_X = rData.nEccen_X;
		nEccen_Y = rData.nEccen_Y;
		bInherentTorsionAmplify = FALSE;
		bAccidentTorsionAmplify = FALSE;
		nCountAddition=rData.nCountAddition;
	}
	void Convert671(T_SEIS_UDRD_D_CH_RW_671& rData)
	{
		Initialize();
		key=rData.key;
		nCodeType=rData.nCodeType;
		memcpy(Description, rData.Description, sizeof(Description));
		KS1992=rData.KS1992;
		UBC1991=rData.UBC1991;
		UBC1997=rData.UBC1997;
		ATC306=rData.ATC306;
		KS2000=rData.KS2000;
		JP=rData.JP;
		IBC2000=rData.IBC2000;
		EURO1996=rData.EURO1996;
		CH2002=rData.CH2002;
		NBC1995 = rData.NBC1995;
		IS2002 = rData.IS2002;
		TAIWAN86 = rData.TAIWAN86;
		KBC2005 = rData.KBC2005;
		CHSH2003.Initialize();
		TAIWAN02.Initialize();
		EURO2004.Initialize();
		KBC2009.Initialize();
		CH2010.Initialize();
		NTC2012.Initialize();
		TAIWAN11.Initialize();
		NTC2008.Initialize();
		IBC2009.Initialize();
		IBC2012.Initialize();
		NSR2010.Initialize();
		P100_2013.Initialize();
		IS1893_2016.Initialize();

		ScaleFactor_X=rData.ScaleFactor_X;
		ScaleFactor_Y=rData.ScaleFactor_Y;
		nEccen_X = rData.nEccen_X;
		nEccen_Y = rData.nEccen_Y;
		bInherentTorsionAmplify = rData.bInherentTorsionAmplify;
		bAccidentTorsionAmplify = rData.bAccidentTorsionAmplify;
		nCountAddition=rData.nCountAddition;
	}
	void Convert700(T_SEIS_UDRD_D_CH_RW_700& rData)
	{
		Initialize();
		key=rData.key;
		nCodeType=rData.nCodeType;
		memcpy(Description, rData.Description, sizeof(Description));
		KS1992=rData.KS1992;
		UBC1991=rData.UBC1991;
		UBC1997=rData.UBC1997;
		ATC306=rData.ATC306;
		KS2000=rData.KS2000;
		JP=rData.JP;
		IBC2000=rData.IBC2000;
		EURO1996=rData.EURO1996;
		CH2002=rData.CH2002;
		NBC1995 = rData.NBC1995;
		IS2002 = rData.IS2002;
		TAIWAN86 = rData.TAIWAN86;
		KBC2005 = rData.KBC2005;
		CHSH2003 = rData.CHSH2003;
		TAIWAN02.Initialize();
		EURO2004.Initialize();
		KBC2009.Initialize();
		CH2010.Initialize();
		NTC2012.Initialize();
		TAIWAN11.Initialize();
		NTC2008.Initialize();
		IBC2009.Initialize();
		IBC2012.Initialize();
		NSR2010.Initialize();
		P100_2013.Initialize();
		IS1893_2016.Initialize();

		ScaleFactor_X=rData.ScaleFactor_X;
		ScaleFactor_Y=rData.ScaleFactor_Y;
		nEccen_X = rData.nEccen_X;
		nEccen_Y = rData.nEccen_Y;
		bInherentTorsionAmplify = rData.bInherentTorsionAmplify;
		bAccidentTorsionAmplify = rData.bAccidentTorsionAmplify;
		nCountAddition=rData.nCountAddition;
	}
	void Convert701(T_SEIS_UDRD_D_CH_RW_701& rData)
	{
		Initialize();
		key=rData.key;
		nCodeType=rData.nCodeType;
		memcpy(Description, rData.Description, sizeof(Description));
		KS1992=rData.KS1992;
		UBC1991=rData.UBC1991;
		UBC1997=rData.UBC1997;
		ATC306=rData.ATC306;
		KS2000=rData.KS2000;
		JP=rData.JP;
		IBC2000=rData.IBC2000;
		EURO1996=rData.EURO1996;
		CH2002=rData.CH2002;
		NBC1995 = rData.NBC1995;
		IS2002 = rData.IS2002;
		TAIWAN86 = rData.TAIWAN86;
		KBC2005 = rData.KBC2005;
		CHSH2003 = rData.CHSH2003;
		TAIWAN02.Convert701(rData.TAIWAN02);
		EURO2004.Initialize();
		KBC2009.Initialize();
		CH2010.Initialize();
		NTC2012.Initialize();
		TAIWAN11.Initialize();
		NTC2008.Initialize();
		IBC2009.Initialize();
		IBC2012.Initialize();
		NSR2010.Initialize();
		P100_2013.Initialize();
		IS1893_2016.Initialize();

		ScaleFactor_X=rData.ScaleFactor_X;
		ScaleFactor_Y=rData.ScaleFactor_Y;
		nEccen_X = rData.nEccen_X;
		nEccen_Y = rData.nEccen_Y;
		bInherentTorsionAmplify = rData.bInherentTorsionAmplify;
		bAccidentTorsionAmplify = rData.bAccidentTorsionAmplify;
		nCountAddition=rData.nCountAddition;
	}
	void Convert711(T_SEIS_UDRD_D_CH_RW_711& rData)
	{
		Initialize();
		key=rData.key;
		nCodeType=rData.nCodeType;
		memcpy(Description, rData.Description, sizeof(Description));
		KS1992=rData.KS1992;
		UBC1991=rData.UBC1991;
		UBC1997=rData.UBC1997;
		ATC306=rData.ATC306;
		KS2000=rData.KS2000;
		JP=rData.JP;
		IBC2000=rData.IBC2000;
		EURO1996=rData.EURO1996;
		CH2002=rData.CH2002;
		NBC1995 = rData.NBC1995;
		IS2002 = rData.IS2002;
		TAIWAN86 = rData.TAIWAN86;
		KBC2005 = rData.KBC2005;
		CHSH2003 = rData.CHSH2003;
		TAIWAN02 = rData.TAIWAN02;
		EURO2004.Initialize();
		KBC2009.Initialize();
		CH2010.Initialize();
		NTC2012.Initialize();
		TAIWAN11.Initialize();
		NTC2008.Initialize();
		IBC2009.Initialize();
		IBC2012.Initialize();
		NSR2010.Initialize();
		P100_2013.Initialize();
		IS1893_2016.Initialize();

		ScaleFactor_X=rData.ScaleFactor_X;
		ScaleFactor_Y=rData.ScaleFactor_Y;
		nEccen_X = rData.nEccen_X;
		nEccen_Y = rData.nEccen_Y;
		bInherentTorsionAmplify = rData.bInherentTorsionAmplify;
		bAccidentTorsionAmplify = rData.bAccidentTorsionAmplify;
		nCountAddition=rData.nCountAddition;
	}
	void Convert750(T_SEIS_UDRD_D_CH_RW_750& rData)
	{
		Initialize();
		key=rData.key;
		nCodeType=rData.nCodeType;
		memcpy(Description, rData.Description, sizeof(Description));
		KS1992=rData.KS1992;
		UBC1991=rData.UBC1991;
		UBC1997=rData.UBC1997;
		ATC306=rData.ATC306;
		KS2000=rData.KS2000;
		JP=rData.JP;
		IBC2000=rData.IBC2000;
		EURO1996=rData.EURO1996;
		CH2002=rData.CH2002;
		NBC1995 = rData.NBC1995;
		IS2002 = rData.IS2002;
		TAIWAN86 = rData.TAIWAN86;
		KBC2005 = rData.KBC2005;
		CHSH2003 = rData.CHSH2003;
		TAIWAN02 = rData.TAIWAN02;
		EURO2004.Convert945(rData.EURO2004);
		KBC2009.Initialize();
		CH2010.Initialize();
		NTC2012.Initialize();
		TAIWAN11.Initialize();
		NTC2008.Initialize();
		IBC2009.Initialize();
		IBC2012.Initialize();
		NSR2010.Initialize();
		P100_2013.Initialize();
		IS1893_2016.Initialize();

		ScaleFactor_X=rData.ScaleFactor_X;
		ScaleFactor_Y=rData.ScaleFactor_Y;
		nEccen_X = rData.nEccen_X;
		nEccen_Y = rData.nEccen_Y;
		bInherentTorsionAmplify = rData.bInherentTorsionAmplify;
		bAccidentTorsionAmplify = rData.bAccidentTorsionAmplify;
		nCountAddition=rData.nCountAddition;
	}
	void Convert780(T_SEIS_UDRD_D_CH_RW_780& rData)
	{
		Initialize();
		key=rData.key;
		nCodeType=rData.nCodeType;
		memcpy(Description, rData.Description, sizeof(Description));
		KS1992=rData.KS1992;
		UBC1991=rData.UBC1991;
		UBC1997=rData.UBC1997;
		ATC306=rData.ATC306;
		KS2000=rData.KS2000;
		JP=rData.JP;
		IBC2000=rData.IBC2000;
		EURO1996=rData.EURO1996;
		CH2002=rData.CH2002;
		NBC1995 = rData.NBC1995;
		IS2002 = rData.IS2002;
		TAIWAN86 = rData.TAIWAN86;
		KBC2005 = rData.KBC2005;
		CHSH2003 = rData.CHSH2003;
		TAIWAN02 = rData.TAIWAN02;
		EURO2004.Convert945(rData.EURO2004);
		KBC2009  = rData.KBC2009;
		CH2010.Initialize();
		NTC2012.Initialize();
		TAIWAN11.Initialize();
		NTC2008.Initialize();
		IBC2009.Initialize();
		IBC2012.Initialize();
		NSR2010.Initialize();
		P100_2013.Initialize();
		IS1893_2016.Initialize();

		ScaleFactor_X=rData.ScaleFactor_X;
		ScaleFactor_Y=rData.ScaleFactor_Y;
		ScaleFactor_R=0.0;      // PMS:4183-HSSHIM-20100505
		nEccen_X = rData.nEccen_X;
		nEccen_Y = rData.nEccen_Y;
		bInherentTorsionAmplify = rData.bInherentTorsionAmplify;
		bAccidentTorsionAmplify = rData.bAccidentTorsionAmplify;
		nCountAddition=rData.nCountAddition;
	}
	void Convert791(T_SEIS_UDRD_D_CH_RW_791& rData)
	{
		Initialize();
		key=rData.key;
		nCodeType=rData.nCodeType;
		memcpy(Description, rData.Description, sizeof(Description));
		KS1992=rData.KS1992;
		UBC1991=rData.UBC1991;
		UBC1997=rData.UBC1997;
		ATC306=rData.ATC306;
		KS2000=rData.KS2000;
		JP=rData.JP;
		IBC2000=rData.IBC2000;
		EURO1996=rData.EURO1996;
		CH2002=rData.CH2002;
		NBC1995 = rData.NBC1995;
		IS2002 = rData.IS2002;
		TAIWAN86 = rData.TAIWAN86;
		KBC2005 = rData.KBC2005;
		CHSH2003 = rData.CHSH2003;
		TAIWAN02 = rData.TAIWAN02;
		EURO2004.Convert945(rData.EURO2004);
		KBC2009  = rData.KBC2009;
		TAIWAN11.Initialize();  // PARKHJ-20111007
		CH2010.Initialize();
		NTC2012.Initialize();
		NTC2008.Initialize();
		IBC2009.Initialize();
		IBC2012.Initialize();
		NSR2010.Initialize();
		P100_2013.Initialize();
		IS1893_2016.Initialize();

		ScaleFactor_X=rData.ScaleFactor_X;
		ScaleFactor_Y=rData.ScaleFactor_Y;
		ScaleFactor_R=rData.ScaleFactor_R;
		nEccen_X = rData.nEccen_X;
		nEccen_Y = rData.nEccen_Y;
		bInherentTorsionAmplify = rData.bInherentTorsionAmplify;
		bAccidentTorsionAmplify = rData.bAccidentTorsionAmplify;
		nCountAddition=rData.nCountAddition;
	}
	void Convert795(T_SEIS_UDRD_D_CH_RW_795& rData)
	{
		Initialize();
		key=rData.key;
		nCodeType=rData.nCodeType;
		memcpy(Description, rData.Description, sizeof(Description));
		KS1992=rData.KS1992;
		UBC1991=rData.UBC1991;
		UBC1997=rData.UBC1997;
		ATC306=rData.ATC306;
		KS2000=rData.KS2000;
		JP=rData.JP;
		IBC2000=rData.IBC2000;
		EURO1996=rData.EURO1996;
		CH2002=rData.CH2002;
		NBC1995 = rData.NBC1995;
		IS2002 = rData.IS2002;
		TAIWAN86 = rData.TAIWAN86;
		KBC2005 = rData.KBC2005;
		CHSH2003 = rData.CHSH2003;
		TAIWAN02 = rData.TAIWAN02;
		EURO2004.Convert945(rData.EURO2004);
		KBC2009  = rData.KBC2009;
		TAIWAN11 = rData.TAIWAN11;   // MNET:5432-HSSHIM-20121123. TAIWAN11는 791에 추가된 데이터이므로 795에 당근 포함되어 있다.
		NTC2008.Initialize();
		CH2010.Initialize();
		NTC2012.Initialize();
		IBC2009.Initialize();
		IBC2012.Initialize();
		NSR2010.Initialize();
		P100_2013.Initialize();
		IS1893_2016.Initialize();

		ScaleFactor_X=rData.ScaleFactor_X;
		ScaleFactor_Y=rData.ScaleFactor_Y;
		ScaleFactor_R=rData.ScaleFactor_R;
		nEccen_X = rData.nEccen_X;
		nEccen_Y = rData.nEccen_Y;
		bInherentTorsionAmplify = rData.bInherentTorsionAmplify;
		bAccidentTorsionAmplify = rData.bAccidentTorsionAmplify;
		nCountAddition=rData.nCountAddition;
	}
	void Convert796(T_SEIS_UDRD_D_CH_RW_796& rData)
	{
		Initialize();
		key=rData.key;
		nCodeType=rData.nCodeType;
		memcpy(Description, rData.Description, sizeof(Description));
		KS1992=rData.KS1992;
		UBC1991=rData.UBC1991;
		UBC1997=rData.UBC1997;
		ATC306=rData.ATC306;
		KS2000=rData.KS2000;
		JP=rData.JP;
		IBC2000=rData.IBC2000;
		EURO1996=rData.EURO1996;
		CH2002=rData.CH2002;
		NBC1995 = rData.NBC1995;
		IS2002 = rData.IS2002;
		TAIWAN86 = rData.TAIWAN86;
		KBC2005 = rData.KBC2005;
		CHSH2003 = rData.CHSH2003;
		TAIWAN02 = rData.TAIWAN02;
		EURO2004.Convert945(rData.EURO2004);
		KBC2009  = rData.KBC2009;
		TAIWAN11 = rData.TAIWAN11;
		NTC2008  = rData.NTC2008;
		CH2010.Initialize();
		NTC2012.Initialize();
		IBC2009.Initialize();
		IBC2012.Initialize();
		NSR2010.Initialize();
		P100_2013.Initialize();
		IS1893_2016.Initialize();

		ScaleFactor_X=rData.ScaleFactor_X;
		ScaleFactor_Y=rData.ScaleFactor_Y;
		ScaleFactor_R=rData.ScaleFactor_R;
		nEccen_X = rData.nEccen_X;
		nEccen_Y = rData.nEccen_Y;
		bInherentTorsionAmplify = rData.bInherentTorsionAmplify;
		bAccidentTorsionAmplify = rData.bAccidentTorsionAmplify;
		nCountAddition=rData.nCountAddition;
	}
	void Convert805(T_SEIS_UDRD_D_CH_RW_805& rData)
	{
		Initialize();
		key=rData.key;
		nCodeType=rData.nCodeType;
		memcpy(Description, rData.Description, sizeof(Description));
		KS1992=rData.KS1992;
		UBC1991=rData.UBC1991;
		UBC1997=rData.UBC1997;
		ATC306=rData.ATC306;
		KS2000=rData.KS2000;
		JP=rData.JP;
		IBC2000=rData.IBC2000;
		EURO1996=rData.EURO1996;
		CH2002=rData.CH2002;
		NBC1995 = rData.NBC1995;
		IS2002 = rData.IS2002;
		TAIWAN86 = rData.TAIWAN86;
		KBC2005 = rData.KBC2005;
		CHSH2003 = rData.CHSH2003;
		TAIWAN02 = rData.TAIWAN02;
		EURO2004.Convert945(rData.EURO2004);
		KBC2009  = rData.KBC2009;
		TAIWAN11 = rData.TAIWAN11;
		NTC2008  = rData.NTC2008;
		IBC2009 = rData.IBC2009;
		IBC2012 = rData.IBC2012;
		ScaleFactor_X=rData.ScaleFactor_X;
		ScaleFactor_Y=rData.ScaleFactor_Y;
		ScaleFactor_R=rData.ScaleFactor_R;
		nEccen_X = rData.nEccen_X;
		nEccen_Y = rData.nEccen_Y;
		bInherentTorsionAmplify = rData.bInherentTorsionAmplify;
		bAccidentTorsionAmplify = rData.bAccidentTorsionAmplify;
		nCountAddition=rData.nCountAddition;
	}
	void Convert845(T_SEIS_UDRD_D_CH_RW_845& rData)
	{
		Initialize();
		key=rData.key;
		nCodeType=rData.nCodeType;
		memcpy(Description, rData.Description, sizeof(Description));
		KS1992=rData.KS1992;
		UBC1991=rData.UBC1991;
		UBC1997=rData.UBC1997;
		ATC306=rData.ATC306;
		KS2000=rData.KS2000;
		JP = rData.JP;
		IBC2000=rData.IBC2000;
		EURO1996=rData.EURO1996;
		CH2002=rData.CH2002;
		NBC1995 = rData.NBC1995;
		IS2002 = rData.IS2002;
		TAIWAN86 = rData.TAIWAN86;
		KBC2005 = rData.KBC2005;
		CHSH2003 = rData.CHSH2003;
		TAIWAN02 = rData.TAIWAN02;
		EURO2004.Convert945(rData.EURO2004);
		KBC2009  = rData.KBC2009;
		TAIWAN11 = rData.TAIWAN11;
		NTC2008  = rData.NTC2008;
		IBC2009 = rData.IBC2009;
		IBC2012 = rData.IBC2012;
		CH2010 = rData.CH2010;
		NTC2012 = rData.NTC2012;
		ScaleFactor_X=rData.ScaleFactor_X;
		ScaleFactor_Y=rData.ScaleFactor_Y;
		ScaleFactor_R=rData.ScaleFactor_R;
		nEccen_X = rData.nEccen_X;
		nEccen_Y = rData.nEccen_Y;
		bInherentTorsionAmplify = rData.bInherentTorsionAmplify;
		bAccidentTorsionAmplify = rData.bAccidentTorsionAmplify;
		nCountAddition=rData.nCountAddition;
	}
	void Convert850(T_SEIS_UDRD_D_CH_RW_850& rData)
	{
		Initialize();
		key=rData.key;
		nCodeType=rData.nCodeType;
		memcpy(Description, rData.Description, sizeof(Description));
		KS1992=rData.KS1992;
		UBC1991=rData.UBC1991;
		UBC1997=rData.UBC1997;
		ATC306=rData.ATC306;
		KS2000=rData.KS2000;
		JP = rData.JP;
		IBC2000=rData.IBC2000;
		EURO1996=rData.EURO1996;
		CH2002=rData.CH2002;
		NBC1995 = rData.NBC1995;
		IS2002 = rData.IS2002;
		TAIWAN86 = rData.TAIWAN86;
		KBC2005 = rData.KBC2005;
		CHSH2003 = rData.CHSH2003;
		TAIWAN02 = rData.TAIWAN02;
		EURO2004.Convert945(rData.EURO2004);
		KBC2009 = rData.KBC2009;
		TAIWAN11 = rData.TAIWAN11;
		NTC2008 = rData.NTC2008;
		NTC2012 = rData.NTC2012;
		CH2010 = rData.CH2010;
		IBC2009 = rData.IBC2009;
		IBC2012 = rData.IBC2012;
		NSR2010.Convert871(rData.NSR2010);
		ScaleFactor_X=rData.ScaleFactor_X;
		ScaleFactor_Y=rData.ScaleFactor_Y;
		ScaleFactor_R=rData.ScaleFactor_R;
		nEccen_X = rData.nEccen_X;
		nEccen_Y = rData.nEccen_Y;
		bInherentTorsionAmplify = rData.bInherentTorsionAmplify;
		bAccidentTorsionAmplify = rData.bAccidentTorsionAmplify;
		nCountAddition=rData.nCountAddition;
	}
	void Convert860(T_SEIS_UDRD_D_CH_RW_860& rData)
	{
		Initialize();
		key=rData.key;
		nCodeType=rData.nCodeType;
		memcpy(Description, rData.Description, sizeof(Description));
		KS1992=rData.KS1992;
		UBC1991=rData.UBC1991;
		UBC1997=rData.UBC1997;
		ATC306=rData.ATC306;
		KS2000=rData.KS2000;
		JP = rData.JP;
		IBC2000=rData.IBC2000;
		EURO1996=rData.EURO1996;
		CH2002=rData.CH2002;
		NBC1995 = rData.NBC1995;
		IS2002 = rData.IS2002;
		TAIWAN86 = rData.TAIWAN86;
		KBC2005 = rData.KBC2005;
		CHSH2003 = rData.CHSH2003;
		TAIWAN02 = rData.TAIWAN02;
		EURO2004.Convert945(rData.EURO2004);
		KBC2009 = rData.KBC2009;
		TAIWAN11 = rData.TAIWAN11;
		NTC2008 = rData.NTC2008;
		NTC2012 = rData.NTC2012;
		CH2010 = rData.CH2010;
		IBC2009 = rData.IBC2009;
		IBC2012 = rData.IBC2012;
		NSR2010.Convert871(rData.NSR2010);
		KBC2015 = rData.KBC2015;
		ScaleFactor_X=rData.ScaleFactor_X;
		ScaleFactor_Y=rData.ScaleFactor_Y;
		ScaleFactor_R=rData.ScaleFactor_R;
		nEccen_X = rData.nEccen_X;
		nEccen_Y = rData.nEccen_Y;
		bInherentTorsionAmplify = rData.bInherentTorsionAmplify;
		bAccidentTorsionAmplify = rData.bAccidentTorsionAmplify;
		nCountAddition=rData.nCountAddition;
	}
	void Convert865(T_SEIS_UDRD_D_CH_RW_865& rData)
	{
		Initialize();
		key=rData.key;
		nCodeType=rData.nCodeType;
		memcpy(Description, rData.Description, sizeof(Description));
		KS1992=rData.KS1992;
		UBC1991=rData.UBC1991;
		UBC1997=rData.UBC1997;
		ATC306=rData.ATC306;
		KS2000=rData.KS2000;
		JP = rData.JP;
		IBC2000=rData.IBC2000;
		EURO1996=rData.EURO1996;
		CH2002=rData.CH2002;
		NBC1995 = rData.NBC1995;
		IS2002 = rData.IS2002;
		TAIWAN86 = rData.TAIWAN86;
		KBC2005 = rData.KBC2005;
		CHSH2003 = rData.CHSH2003;
		TAIWAN02 = rData.TAIWAN02;
		EURO2004.Convert945(rData.EURO2004);
		KBC2009 = rData.KBC2009;
		TAIWAN11 = rData.TAIWAN11;
		NTC2008 = rData.NTC2008;
		NTC2012 = rData.NTC2012;
		CH2010 = rData.CH2010;
		IBC2009 = rData.IBC2009;
		IBC2012 = rData.IBC2012;
		NSR2010.Convert871(rData.NSR2010);
		KBC2015 = rData.KBC2015;
		P100_2013 = rData.P100_2013;
		ScaleFactor_X=rData.ScaleFactor_X;
		ScaleFactor_Y=rData.ScaleFactor_Y;
		ScaleFactor_R=rData.ScaleFactor_R;
		nEccen_X = rData.nEccen_X;
		nEccen_Y = rData.nEccen_Y;
		bInherentTorsionAmplify = rData.bInherentTorsionAmplify;
		bAccidentTorsionAmplify = rData.bAccidentTorsionAmplify;
		nCountAddition=rData.nCountAddition;
	}
	void Convert865_2(T_SEIS_UDRD_D_CH_RW_865_2& rData)
	{
		Initialize();
		key=rData.key;
		nCodeType=rData.nCodeType;
		memcpy(Description, rData.Description, sizeof(Description));
		KS1992=rData.KS1992;
		UBC1991=rData.UBC1991;
		UBC1997=rData.UBC1997;
		ATC306=rData.ATC306;
		KS2000=rData.KS2000;
		JP = rData.JP;
		IBC2000=rData.IBC2000;
		EURO1996=rData.EURO1996;
		CH2002=rData.CH2002;
		NBC1995 = rData.NBC1995;
		IS2002 = rData.IS2002;
		TAIWAN86 = rData.TAIWAN86;
		KBC2005 = rData.KBC2005;
		CHSH2003 = rData.CHSH2003;
		TAIWAN02 = rData.TAIWAN02;
		EURO2004.Convert945(rData.EURO2004);
		KBC2009 = rData.KBC2009;
		TAIWAN11 = rData.TAIWAN11;
		NTC2008 = rData.NTC2008;
		NTC2012 = rData.NTC2012;
		CH2010 = rData.CH2010;
		IBC2009 = rData.IBC2009;
		IBC2012 = rData.IBC2012;
		NSR2010.Convert871(rData.NSR2010);
		KBC2015 = rData.KBC2015;
		P100_2013 = rData.P100_2013;
		IS1893_2016 = rData.IS1893_2016;
		ScaleFactor_X=rData.ScaleFactor_X;
		ScaleFactor_Y=rData.ScaleFactor_Y;
		ScaleFactor_R=rData.ScaleFactor_R;
		nEccen_X = rData.nEccen_X;
		nEccen_Y = rData.nEccen_Y;
		bInherentTorsionAmplify = rData.bInherentTorsionAmplify;
		bAccidentTorsionAmplify = rData.bAccidentTorsionAmplify;
		nCountAddition=rData.nCountAddition;
	}
	void Convert871(T_SEIS_UDRD_D_CH_RW_871& rData)
	{
		Initialize();
		key=rData.key;
		nCodeType=rData.nCodeType;
		memcpy(Description, rData.Description, sizeof(Description));
		KS1992=rData.KS1992;
		UBC1991=rData.UBC1991;
		UBC1997=rData.UBC1997;
		ATC306=rData.ATC306;
		KS2000=rData.KS2000;
		JP = rData.JP;
		IBC2000=rData.IBC2000;
		EURO1996=rData.EURO1996;
		CH2002=rData.CH2002;
		NBC1995 = rData.NBC1995;
		IS2002 = rData.IS2002;
		TAIWAN86 = rData.TAIWAN86;
		KBC2005 = rData.KBC2005;
		CHSH2003 = rData.CHSH2003;
		TAIWAN02 = rData.TAIWAN02;
		EURO2004.Convert945(rData.EURO2004);
		KBC2009 = rData.KBC2009;
		TAIWAN11 = rData.TAIWAN11;
		NTC2008 = rData.NTC2008;
		NTC2012 = rData.NTC2012;
		CH2010 = rData.CH2010;
		IBC2009 = rData.IBC2009;
		IBC2012 = rData.IBC2012;
		NSR2010.Convert871(rData.NSR2010);
		KBC2015 = rData.KBC2015;
		P100_2013 = rData.P100_2013;
		IS1893_2016 = rData.IS1893_2016;
		ScaleFactor_X=rData.ScaleFactor_X;
		ScaleFactor_Y=rData.ScaleFactor_Y;
		ScaleFactor_R=rData.ScaleFactor_R;
		nEccen_X = rData.nEccen_X;
		nEccen_Y = rData.nEccen_Y;
		bInherentTorsionAmplify = rData.bInherentTorsionAmplify;
		bAccidentTorsionAmplify = rData.bAccidentTorsionAmplify;
		nCountAddition=rData.nCountAddition;
	}
	void Convert883(T_SEIS_UDRD_D_CH_RW_883& rData)
	{
		Initialize();
		key=rData.key;
		nCodeType=rData.nCodeType;
		memcpy(Description, rData.Description, sizeof(Description));
		KS1992=rData.KS1992;
		UBC1991=rData.UBC1991;
		UBC1997=rData.UBC1997;
		ATC306=rData.ATC306;
		KS2000=rData.KS2000;
		JP = rData.JP;
		IBC2000=rData.IBC2000;
		EURO1996=rData.EURO1996;
		CH2002=rData.CH2002;
		NBC1995 = rData.NBC1995;
		IS2002 = rData.IS2002;
		TAIWAN86 = rData.TAIWAN86;
		KBC2005 = rData.KBC2005;
		CHSH2003 = rData.CHSH2003;
		TAIWAN02 = rData.TAIWAN02;
		EURO2004.Convert945(rData.EURO2004);
		KBC2009 = rData.KBC2009;
		TAIWAN11 = rData.TAIWAN11;
		NTC2008 = rData.NTC2008;
		NTC2012 = rData.NTC2012;
		CH2010 = rData.CH2010;
		IBC2009 = rData.IBC2009;
		IBC2012 = rData.IBC2012;
		NSR2010 = rData.NSR2010;
		KBC2015 = rData.KBC2015;
		P100_2013 = rData.P100_2013;
		IS1893_2016 = rData.IS1893_2016;
		NTC2018.Initialize(); // GEN-615 : 883 이전 버전에서 입력된 NTC2018 값 무시
		ScaleFactor_X=rData.ScaleFactor_X;
		ScaleFactor_Y=rData.ScaleFactor_Y;
		ScaleFactor_R=rData.ScaleFactor_R;
		nEccen_X = rData.nEccen_X;
		nEccen_Y = rData.nEccen_Y;
		bInherentTorsionAmplify = rData.bInherentTorsionAmplify;
		bAccidentTorsionAmplify = rData.bAccidentTorsionAmplify;
		nCountAddition=rData.nCountAddition;
	}
	void Convert915(T_SEIS_UDRD_D_CH_RW_915& rData)
	{
		Initialize();
		key=rData.key;
		nCodeType=rData.nCodeType;
		memcpy(Description, rData.Description, sizeof(Description));

		KS1992       = rData.KS1992;
		UBC1991      = rData.UBC1991;
		UBC1997      = rData.UBC1997;
		ATC306       = rData.ATC306;
		KS2000       = rData.KS2000;
		JP           = rData.JP;
		IBC2000      = rData.IBC2000;
		EURO1996     = rData.EURO1996;
		CH2002       = rData.CH2002;
		NBC1995      = rData.NBC1995;
		IS2002       = rData.IS2002;
		TAIWAN86     = rData.TAIWAN86;
		KBC2005      = rData.KBC2005;
		CHSH2003     = rData.CHSH2003;
		TAIWAN02     = rData.TAIWAN02;
		EURO2004.Convert945(rData.EURO2004);
		KBC2009      = rData.KBC2009;
		TAIWAN11     = rData.TAIWAN11;
		NTC2008      = rData.NTC2008;
		NTC2012      = rData.NTC2012;
		CH2010       = rData.CH2010;
		IBC2009      = rData.IBC2009;
		IBC2012      = rData.IBC2012;
		NSR2010      = rData.NSR2010;
		KBC2015      = rData.KBC2015;
		P100_2013    = rData.P100_2013;
		IS1893_2016  = rData.IS1893_2016;
		NTC2018      = rData.NTC2018;
		KDS2019      = rData.KDS2019;
		DPT2018.Init();

		ScaleFactor_X=rData.ScaleFactor_X;
		ScaleFactor_Y=rData.ScaleFactor_Y;
		ScaleFactor_R=rData.ScaleFactor_R;
		nEccen_X = rData.nEccen_X;
		nEccen_Y = rData.nEccen_Y;
		bInherentTorsionAmplify = rData.bInherentTorsionAmplify;
		bAccidentTorsionAmplify = rData.bAccidentTorsionAmplify;
		nCountAddition=rData.nCountAddition;
	}
	void Convert920_1(T_SEIS_UDRD_D_CH_RW_920_1& rData)
	{
		Initialize();
		key=rData.key;
		nCodeType=rData.nCodeType;
		memcpy(Description, rData.Description, sizeof(Description));
		KS1992       = rData.KS1992;
		UBC1991      = rData.UBC1991;
		UBC1997      = rData.UBC1997;
		ATC306       = rData.ATC306;
		KS2000       = rData.KS2000;
		JP           = rData.JP;
		IBC2000      = rData.IBC2000;
		EURO1996     = rData.EURO1996;
		CH2002       = rData.CH2002;
		NBC1995      = rData.NBC1995;
		IS2002       = rData.IS2002;
		TAIWAN86     = rData.TAIWAN86;
		KBC2005      = rData.KBC2005;
		CHSH2003     = rData.CHSH2003;
		TAIWAN02     = rData.TAIWAN02;
		EURO2004.Convert945(rData.EURO2004);
		KBC2009      = rData.KBC2009;
		TAIWAN11     = rData.TAIWAN11;
		NTC2008      = rData.NTC2008;
		NTC2012      = rData.NTC2012;
		CH2010       = rData.CH2010;
		IBC2009      = rData.IBC2009;
		IBC2012      = rData.IBC2012;
		NSR2010      = rData.NSR2010;
		KBC2015      = rData.KBC2015;
		P100_2013    = rData.P100_2013;
		IS1893_2016  = rData.IS1893_2016;
		NTC2018      = rData.NTC2018;
		KDS2019      = rData.KDS2019;
		DPT2018      = rData.DPT2018;
		TAIWAN22.Initialize();

		ScaleFactor_X=rData.ScaleFactor_X;
		ScaleFactor_Y=rData.ScaleFactor_Y;
		ScaleFactor_R=rData.ScaleFactor_R;
		nEccen_X = rData.nEccen_X;
		nEccen_Y = rData.nEccen_Y;
		bInherentTorsionAmplify = rData.bInherentTorsionAmplify;
		bAccidentTorsionAmplify = rData.bAccidentTorsionAmplify;
		nCountAddition=rData.nCountAddition;
	}
	void Convert920(T_SEIS_UDRD_D_CH_RW_920& rData)
	{
		Initialize();
		key=rData.key;
		nCodeType=rData.nCodeType;
		memcpy(Description, rData.Description, sizeof(Description));

		KS1992       = rData.KS1992;
		UBC1991      = rData.UBC1991;
		UBC1997      = rData.UBC1997;
		ATC306       = rData.ATC306;
		KS2000       = rData.KS2000;
		JP           = rData.JP;
		IBC2000      = rData.IBC2000;
		EURO1996     = rData.EURO1996;
		CH2002       = rData.CH2002;
		NBC1995      = rData.NBC1995;
		IS2002       = rData.IS2002;
		TAIWAN86     = rData.TAIWAN86;
		KBC2005      = rData.KBC2005;
		CHSH2003     = rData.CHSH2003;
		TAIWAN02     = rData.TAIWAN02;
		EURO2004.Convert945(rData.EURO2004);
		KBC2009      = rData.KBC2009;
		TAIWAN11     = rData.TAIWAN11;
		NTC2008      = rData.NTC2008;
		NTC2012      = rData.NTC2012;
		CH2010       = rData.CH2010;
		IBC2009      = rData.IBC2009;
		IBC2012      = rData.IBC2012;
		NSR2010      = rData.NSR2010;
		KBC2015      = rData.KBC2015;
		P100_2013    = rData.P100_2013;
		IS1893_2016  = rData.IS1893_2016;
		NTC2018      = rData.NTC2018;
		KDS2019      = rData.KDS2019;
		DPT2018      = rData.DPT2018;
		TAIWAN22.Initialize();

		ScaleFactor_X=rData.ScaleFactor_X;
		ScaleFactor_Y=rData.ScaleFactor_Y;
		ScaleFactor_R=rData.ScaleFactor_R;
		nEccen_X = rData.nEccen_X;
		nEccen_Y = rData.nEccen_Y;
		bInherentTorsionAmplify = rData.bInherentTorsionAmplify;
		bAccidentTorsionAmplify = rData.bAccidentTorsionAmplify;
		nCountAddition=rData.nCountAddition;
	}
	void Convert930(T_SEIS_UDRD_D_CH_RW_930& rData)
	{
		Initialize();
		key = rData.key;
		nCodeType = rData.nCodeType;
		memcpy(Description, rData.Description, sizeof(Description));

		KS1992 = rData.KS1992;
		UBC1991 = rData.UBC1991;
		UBC1997 = rData.UBC1997;
		ATC306 = rData.ATC306;
		KS2000 = rData.KS2000;
		JP = rData.JP;
		IBC2000 = rData.IBC2000;
		EURO1996 = rData.EURO1996;
		CH2002 = rData.CH2002;
		NBC1995 = rData.NBC1995;
		IS2002 = rData.IS2002;
		TAIWAN86 = rData.TAIWAN86;
		KBC2005 = rData.KBC2005;
		CHSH2003 = rData.CHSH2003;
		TAIWAN02 = rData.TAIWAN02;
		EURO2004.Convert945(rData.EURO2004);
		KBC2009 = rData.KBC2009;
		TAIWAN11 = rData.TAIWAN11;
		NTC2008 = rData.NTC2008;
		NTC2012 = rData.NTC2012;
		CH2010 = rData.CH2010;
		IBC2009 = rData.IBC2009;
		IBC2012 = rData.IBC2012;
		NSR2010 = rData.NSR2010;
		KBC2015 = rData.KBC2015;
		P100_2013 = rData.P100_2013;
		IS1893_2016 = rData.IS1893_2016;
		NTC2018 = rData.NTC2018;
		KDS2019 = rData.KDS2019;
		DPT2018 = rData.DPT2018;
		TAIWAN22 = rData.TAIWAN22;

		ScaleFactor_X = rData.ScaleFactor_X;
		ScaleFactor_Y = rData.ScaleFactor_Y;
		ScaleFactor_R = rData.ScaleFactor_R;
		nEccen_X = rData.nEccen_X;
		nEccen_Y = rData.nEccen_Y;
		bInherentTorsionAmplify = rData.bInherentTorsionAmplify;
		bAccidentTorsionAmplify = rData.bAccidentTorsionAmplify;
		nCountAddition = rData.nCountAddition;
		nCountUser = 0;
	}
	void Convert945(T_SEIS_UDRD_D_CH_RW_945& rData)
	{
		Initialize();
		key = rData.key;
		nCodeType = rData.nCodeType;
		memcpy(Description, rData.Description, sizeof(Description));

		KS1992 = rData.KS1992;
		UBC1991 = rData.UBC1991;
		UBC1997 = rData.UBC1997;
		ATC306 = rData.ATC306;
		KS2000 = rData.KS2000;
		JP = rData.JP;
		IBC2000 = rData.IBC2000;
		EURO1996 = rData.EURO1996;
		CH2002 = rData.CH2002;
		NBC1995 = rData.NBC1995;
		IS2002 = rData.IS2002;
		TAIWAN86 = rData.TAIWAN86;
		KBC2005 = rData.KBC2005;
		CHSH2003 = rData.CHSH2003;
		TAIWAN02 = rData.TAIWAN02;
		EURO2004.Convert945(rData.EURO2004);
		KBC2009 = rData.KBC2009;
		TAIWAN11 = rData.TAIWAN11;
		NTC2008 = rData.NTC2008;
		NTC2012 = rData.NTC2012;
		CH2010 = rData.CH2010;
		IBC2009 = rData.IBC2009;
		IBC2012 = rData.IBC2012;
		NSR2010 = rData.NSR2010;
		KBC2015 = rData.KBC2015;
		P100_2013 = rData.P100_2013;
		IS1893_2016 = rData.IS1893_2016;
		NTC2018 = rData.NTC2018;
		KDS2019 = rData.KDS2019;
		DPT2018 = rData.DPT2018;
		TAIWAN22 = rData.TAIWAN22;

		ScaleFactor_X = rData.ScaleFactor_X;
		ScaleFactor_Y = rData.ScaleFactor_Y;
		ScaleFactor_R = rData.ScaleFactor_R;
		nEccen_X = rData.nEccen_X;
		nEccen_Y = rData.nEccen_Y;
		bInherentTorsionAmplify = rData.bInherentTorsionAmplify;
		bAccidentTorsionAmplify = rData.bAccidentTorsionAmplify;
		nCountAddition = rData.nCountAddition;
		nCountUser = rData.nCountUser;
	}
	void Convert950(T_SEIS_UDRD_D_CH_RW_950& rData)
	{
		Initialize();
		key = rData.key;
		nCodeType = rData.nCodeType;
		memcpy(Description, rData.Description, sizeof(Description));

		KS1992 = rData.KS1992;
		UBC1991 = rData.UBC1991;
		UBC1997 = rData.UBC1997;
		ATC306 = rData.ATC306;
		KS2000 = rData.KS2000;
		JP = rData.JP;
		IBC2000 = rData.IBC2000;
		EURO1996 = rData.EURO1996;
		CH2002 = rData.CH2002;
		NBC1995 = rData.NBC1995;
		IS2002 = rData.IS2002;
		TAIWAN86 = rData.TAIWAN86;
		KBC2005 = rData.KBC2005;
		CHSH2003 = rData.CHSH2003;
		TAIWAN02 = rData.TAIWAN02;
		EURO2004 = rData.EURO2004;
		KBC2009 = rData.KBC2009;
		TAIWAN11 = rData.TAIWAN11;
		NTC2008 = rData.NTC2008;
		NTC2012 = rData.NTC2012;
		CH2010 = rData.CH2010;
		IBC2009 = rData.IBC2009;
		IBC2012 = rData.IBC2012;
		NSR2010 = rData.NSR2010;
		KBC2015 = rData.KBC2015;
		P100_2013 = rData.P100_2013;
		IS1893_2016 = rData.IS1893_2016;
		NTC2018 = rData.NTC2018;
		KDS2019 = rData.KDS2019;
		DPT2018 = rData.DPT2018;
		TAIWAN22 = rData.TAIWAN22;		

		ScaleFactor_X = rData.ScaleFactor_X;
		ScaleFactor_Y = rData.ScaleFactor_Y;
		ScaleFactor_R = rData.ScaleFactor_R;
		nEccen_X = rData.nEccen_X;
		nEccen_Y = rData.nEccen_Y;
		bInherentTorsionAmplify = rData.bInherentTorsionAmplify;
		bAccidentTorsionAmplify = rData.bAccidentTorsionAmplify;
		nCountAddition = rData.nCountAddition;
		nCountUser = rData.nCountUser;
	}
	void Convert960(T_SEIS_UDRD_D_CH_RW_960& rData)
	{
		Initialize();
		key = rData.key;
		nCodeType = rData.nCodeType;
		memcpy(Description, rData.Description, sizeof(Description));

		KS1992 = rData.KS1992;
		UBC1991 = rData.UBC1991;
		UBC1997 = rData.UBC1997;
		ATC306 = rData.ATC306;
		KS2000 = rData.KS2000;
		JP = rData.JP;
		IBC2000 = rData.IBC2000;
		EURO1996 = rData.EURO1996;
		CH2002 = rData.CH2002;
		NBC1995 = rData.NBC1995;
		IS2002 = rData.IS2002;
		TAIWAN86 = rData.TAIWAN86;
		KBC2005 = rData.KBC2005;
		CHSH2003 = rData.CHSH2003;
		TAIWAN02 = rData.TAIWAN02;
		EURO2004 = rData.EURO2004;
		KBC2009 = rData.KBC2009;
		TAIWAN11 = rData.TAIWAN11;
		NTC2008 = rData.NTC2008;
		NTC2012 = rData.NTC2012;
		CH2010 = rData.CH2010;
		IBC2009 = rData.IBC2009;
		IBC2012 = rData.IBC2012;
		NSR2010 = rData.NSR2010;
		KBC2015 = rData.KBC2015;
		P100_2013 = rData.P100_2013;
		IS1893_2016 = rData.IS1893_2016;
		NTC2018 = rData.NTC2018;
		KDS2019 = rData.KDS2019;
		DPT2018 = rData.DPT2018;
		TAIWAN22 = rData.TAIWAN22;
		NSCP2024 = rData.NSCP2024;
		SANS2010 = rData.SANS2010;

		ScaleFactor_X = rData.ScaleFactor_X;
		ScaleFactor_Y = rData.ScaleFactor_Y;
		ScaleFactor_R = rData.ScaleFactor_R;
		nEccen_X = rData.nEccen_X;
		nEccen_Y = rData.nEccen_Y;
		bInherentTorsionAmplify = rData.bInherentTorsionAmplify;
		bAccidentTorsionAmplify = rData.bAccidentTorsionAmplify;
		nCountAddition = rData.nCountAddition;
		nCountUser = rData.nCountUser;
	}

};

struct T_SEIS_UNIT
{
	T_SEIS_UNIT()
	{
		nCodeType=D_UNITSYS_NONE;
		Description=D_UNITSYS_NONE;
		KS1992_dZoneFactor=D_UNITSYS_NONE;
		KS1992_dImportanceFactor=D_UNITSYS_NONE;
		KS1992_dSoilFactor=D_UNITSYS_NONE;
		KS1992_dPeriodAnalysis=D_UNITSYS_NONE;
		KS1992_dPeriodCode=D_UNITSYS_NONE;
		KS1992_dResponseModificationFactor=D_UNITSYS_NONE;
		UBC1991_dZoneFactor=D_UNITSYS_NONE;
		UBC1991_dImportanceFactor=D_UNITSYS_NONE;
		UBC1991_dSoilProfileFactor=D_UNITSYS_NONE;
		UBC1991_dPeriodAnalysis=D_UNITSYS_NONE;
		UBC1991_dPeriodCode=D_UNITSYS_NONE;
		UBC1991_dSystemCoefficient=D_UNITSYS_NONE;
		UBC1997_dSeismicZoneFactor=D_UNITSYS_NONE;
		UBC1997_dImportanceFactor=D_UNITSYS_NONE;
		UBC1997_nSoilProfileType=D_UNITSYS_NONE;
		UBC1997_nSeismicSourceType=D_UNITSYS_NONE;
		UBC1997_dCloseastDistance=D_UNITSYS_NONE;
		UBC1997_dPeriod=D_UNITSYS_NONE;
		UBC1997_dResponseModificationFactor=D_UNITSYS_NONE;
		ATC306_dEffectivePeakVelocity=D_UNITSYS_NONE;
		ATC306_dSoilProfileCoefficient=D_UNITSYS_NONE;
		ATC306_dPeriodAnalysis=D_UNITSYS_NONE;
		ATC306_dPeriodCode=D_UNITSYS_NONE;
		ATC306_dResponseModificationFactor=D_UNITSYS_NONE;
		KS2000_dZoneFactor=D_UNITSYS_NONE;
		KS2000_dImportanceFactor=D_UNITSYS_NONE;
		KS2000_dSoilFactor=D_UNITSYS_NONE;
		KS2000_dPeriodAnalysis=D_UNITSYS_NONE;
		KS2000_dPeriodCode=D_UNITSYS_NONE;
		KS2000_dResponseModificationFactor=D_UNITSYS_NONE;
		JP_dZoneFactor=D_UNITSYS_NONE;
		JP_dStdShearFactor=D_UNITSYS_NONE;
		JP_dSoilFactor=D_UNITSYS_NONE;
		JP_dPeriodAnalysis_X=D_UNITSYS_NONE;
		JP_dPeriodAnalysis_Y=D_UNITSYS_NONE;
		JP_dPeriodCode_X=D_UNITSYS_NONE;
		JP_dPeriodCode_Y=D_UNITSYS_NONE;
		JP_dResponseModificationFactor_X=D_UNITSYS_NONE;
		JP_dResponseModificationFactor_Y=D_UNITSYS_NONE;
		IBC2000_nSeismicDesignCategory=D_UNITSYS_NONE;
		IBC2000_nSiteClass=D_UNITSYS_NONE;
		IBC2000_dMSRAs=D_UNITSYS_NONE;
		IBC2000_dMSRA1=D_UNITSYS_NONE;
		IBC2000_dImportanceFactor=D_UNITSYS_NONE;
		IBC2000_dPeriodCode_X=D_UNITSYS_NONE;
		IBC2000_dPeriodCode_Y=D_UNITSYS_NONE;
		IBC2000_dPeriodCalculated_X=D_UNITSYS_NONE;
		IBC2000_dPeriodCalculated_Y=D_UNITSYS_NONE;
		IBC2000_dResponseModificationFactor_X=D_UNITSYS_NONE;
		IBC2000_dResponseModificationFactor_Y=D_UNITSYS_NONE;
		ScaleFactor_X=D_UNITSYS_NONE;
		ScaleFactor_Y=D_UNITSYS_NONE;
		nEccen_X=D_UNITSYS_NONE;
		nEccen_Y=D_UNITSYS_NONE;
		KeyStor=D_UNITSYS_NONE;
		AddX=D_UNITSYS_BASE_FORCE;
		AddY=D_UNITSYS_BASE_FORCE;
		AddR=D_UNITSYS_BASE_MOMENT;
		Depth2MR=D_UNITSYS_BASE_LENGTH;
		nWeight = D_UNITSYS_BASE_FORCE;
		nElev = D_UNITSYS_BASE_LENGTH;
		nForc = D_UNITSYS_BASE_FORCE;
	}
	int nCodeType;
	int Description;
	int KS1992_dZoneFactor;
	int KS1992_dImportanceFactor;
	int KS1992_dSoilFactor;
	int KS1992_dPeriodAnalysis;
	int KS1992_dPeriodCode;
	int KS1992_dResponseModificationFactor;
	int UBC1991_dZoneFactor;
	int UBC1991_dImportanceFactor;
	int UBC1991_dSoilProfileFactor;
	int UBC1991_dPeriodAnalysis;
	int UBC1991_dPeriodCode;
	int UBC1991_dSystemCoefficient;
	int UBC1997_dSeismicZoneFactor;
	int UBC1997_dImportanceFactor;
	int UBC1997_nSoilProfileType;
	int UBC1997_nSeismicSourceType;
	int UBC1997_dCloseastDistance;
	int UBC1997_dPeriod;
	int UBC1997_dResponseModificationFactor;
	int ATC306_dEffectivePeakVelocity;
	int ATC306_dSoilProfileCoefficient;
	int ATC306_dPeriodAnalysis;
	int ATC306_dPeriodCode;
	int ATC306_dResponseModificationFactor;
	int KS2000_dZoneFactor;
	int KS2000_dImportanceFactor;
	int KS2000_dSoilFactor;
	int KS2000_dPeriodAnalysis;
	int KS2000_dPeriodCode;
	int KS2000_dResponseModificationFactor;
	int JP_dZoneFactor;
	int JP_dStdShearFactor;
	int JP_dSoilFactor;
	int JP_dPeriodAnalysis_X;
	int JP_dPeriodAnalysis_Y;
	int JP_dPeriodCode_X;
	int JP_dPeriodCode_Y;
	int JP_dResponseModificationFactor_X;
	int JP_dResponseModificationFactor_Y;
	int IBC2000_nSeismicDesignCategory;
	int IBC2000_nSiteClass;
	int IBC2000_dMSRAs;
	int IBC2000_dMSRA1;
	int IBC2000_dImportanceFactor;
	int IBC2000_dPeriodCode_X;
	int IBC2000_dPeriodCode_Y;
	int IBC2000_dPeriodCalculated_X;
	int IBC2000_dPeriodCalculated_Y;
	int IBC2000_dResponseModificationFactor_X;
	int IBC2000_dResponseModificationFactor_Y;
	int ScaleFactor_X;
	int ScaleFactor_Y;
	int nEccen_X;
	int nEccen_Y;
	int KeyStor;
	int AddX;
	int AddY;
	int AddR;
	int Depth2MR;
	int nWeight;
	int nElev;
	int nForc;
};
#define HASHSIZESEIS 1

#define T_SSEI_K T_STLD_K
struct T_SSEI_D
{
	CString Description;
	BOOL    bUseGrup;
	UINT    GrupKey;
	CArray<T_ELEM_K, T_ELEM_K> arKeyElem;
	BOOL    bAdditionDeadLoad;
	UINT    StldKey;
	BOOL    bNodalLoad;
	BOOL    bBeamLoad;
	int     nDirectionMtd;                   // 0=Points, 1=Angle
	double  dPoint1[3];
	double  dPoint2[3];
	double  dAngle;
	double  dKh;
	void Initialize()
	{
		Description.Empty();
		bUseGrup = FALSE;
		GrupKey = 0;
		arKeyElem.RemoveAll();
		bAdditionDeadLoad = FALSE;
		StldKey = 0;
		bNodalLoad = TRUE;
		bBeamLoad = TRUE;
		nDirectionMtd = 1;
		for ( int i = 0; i < 3; i++ )
		{
			dPoint1[i] = 0.;
			dPoint2[i] = 0.;
		}
		dAngle = 0.;
		dKh = 0.;
	}
	T_SSEI_D() { }
	T_SSEI_D(T_SSEI_D& src)
	{
		*this = src;
	}
	T_SSEI_D& operator=(T_SSEI_D& src)
	{
		Description   = src.Description;
		bUseGrup      = src.bUseGrup;
		GrupKey       = src.GrupKey;
		arKeyElem.Copy(src.arKeyElem);
		bAdditionDeadLoad = src.bAdditionDeadLoad;
		StldKey           = src.StldKey;
		bNodalLoad        = src.bNodalLoad;
		bBeamLoad         = src.bBeamLoad;
		nDirectionMtd = src.nDirectionMtd;
		for ( int i = 0; i < 3; i++ )
		{
			dPoint1[i]  = src.dPoint1[i];
			dPoint2[i]  = src.dPoint2[i];
		}
		dAngle        = src.dAngle;
		dKh           = src.dKh;
		return *this;
	}
};

struct T_SSEI_D_CH
{
	char    Description[80];
	BOOL    bUseGrup;
	UINT    GrupKey;
	CArray<T_ELEM_K, T_ELEM_K> arKeyElem;
	BOOL    bAdditionDeadLoad;
	UINT    StldKey;
	BOOL    bNodalLoad;
	BOOL    bBeamLoad;
	int     nDirectionMtd;                   // 0=Points, 1=Angle
	double  dPoint1[3];
	double  dPoint2[3];
	double  dAngle;
	double  dKh;
	void ConvertToString(T_SSEI_D& rData)
	{
		ConvertCharStr(Description, rData.Description, sizeof(Description));
		rData.bUseGrup      = bUseGrup;
		rData.GrupKey       = GrupKey;
		rData.arKeyElem.Copy(arKeyElem);
		rData.bAdditionDeadLoad = bAdditionDeadLoad;
		rData.StldKey           = StldKey;
		rData.bNodalLoad        = bNodalLoad;
		rData.bBeamLoad         = bBeamLoad;
		rData.nDirectionMtd = nDirectionMtd;
		for ( int i = 0; i < 3; i++ )
		{
			rData.dPoint1[i]  = dPoint1[i];
			rData.dPoint2[i]  = dPoint2[i];
		}
		rData.dAngle        = dAngle;
		rData.dKh           = dKh;
	}
	void ConvertToChar(T_SSEI_D& rData)
	{
		ConvertStrChar(rData.Description, Description, sizeof(Description));
		bUseGrup      = rData.bUseGrup;
		GrupKey       = rData.GrupKey;
		arKeyElem.Copy(rData.arKeyElem);
		bAdditionDeadLoad = rData.bAdditionDeadLoad;
		StldKey           = rData.StldKey;
		bNodalLoad        = rData.bNodalLoad;
		bBeamLoad         = rData.bBeamLoad;
		nDirectionMtd = rData.nDirectionMtd;
		for ( int i = 0; i < 3; i++ )
		{
			dPoint1[i]  = rData.dPoint1[i];
			dPoint2[i]  = rData.dPoint2[i];
		}
		dAngle        = rData.dAngle;
		dKh           = rData.dKh;
	}
};
struct T_SSEI_UDRD_D
{
	T_SSEI_K key;
	T_SSEI_D data;
};
struct T_SSEI_UDRD_D_CH
{
	T_SSEI_K key;
	T_SSEI_D_CH data;
};
struct T_SSEI_UDRD_D_CH_RW
{
	T_SSEI_K key;
	char    Description[80];
	BOOL    bUseGrup;
	UINT    GrupKey;
	int     nCountKeyElem;
	BOOL    bAdditionDeadLoad;
	UINT    StldKey;
	BOOL    bNodalLoad;
	BOOL    bBeamLoad;
	int     nDirectionMtd;                   // 0=Points, 1=Angle
	double  dPoint1[3];
	double  dPoint2[3];
	double  dAngle;
	double  dKh;
	void GetSsei(T_SSEI_K& rKey, T_SSEI_D_CH& rData)
	{
		rKey = key;
		memcpy(rData.Description, Description, sizeof(Description));
		rData.bUseGrup      = bUseGrup;
		rData.GrupKey       = GrupKey;
		rData.bAdditionDeadLoad = bAdditionDeadLoad;
		rData.StldKey           = StldKey;
		rData.bNodalLoad        = bNodalLoad;
		rData.bBeamLoad         = bBeamLoad;
		rData.nDirectionMtd = nDirectionMtd;
		for ( int i = 0; i < 3; i++ )
		{
			rData.dPoint1[i]  = dPoint1[i];
			rData.dPoint2[i]  = dPoint2[i];
		}
		rData.dAngle        = dAngle;
		rData.dKh           = dKh;
	}
	void SetSsei(const T_SSEI_K Key, const T_SSEI_D_CH& rData)
	{
		key = Key;
		memcpy(Description, rData.Description, sizeof(Description));
		bUseGrup      = rData.bUseGrup;
		GrupKey       = rData.GrupKey;
		nCountKeyElem = ( int ) rData.arKeyElem.GetSize();
		bAdditionDeadLoad = rData.bAdditionDeadLoad;
		StldKey           = rData.StldKey;
		bNodalLoad        = rData.bNodalLoad;
		bBeamLoad         = rData.bBeamLoad;
		nDirectionMtd = rData.nDirectionMtd;
		for ( int i = 0; i < 3; i++ )
		{
			dPoint1[i]  = rData.dPoint1[i];
			dPoint2[i]  = rData.dPoint2[i];
		}
		dAngle        = rData.dAngle;
		dKh           = rData.dKh;
	}
};
struct T_SSEI_UNIT
{
	T_SSEI_UNIT()
	{
		dPoint = D_UNITSYS_BASE_LENGTH;
	}
	int dPoint;
};
#define HASHSIZESSEI 1

enum class SEIS_EVAL_STEP { _PRE, _1ST, _2ND };
enum class SEIS_PFRM_TYPE { Pushover, Timehistory };

enum D_SEME_METHOD
{
	D_SEME_METHOD_KISTEC2013=0,
	D_SEME_METHOD_MOE2018,
	D_SEME_METHOD_MOE2019,
	D_SEME_METHOD_KISTEC2019,
	D_SEME_METHOD_AIK_G_001_2021, 

	D_SEME_METHOD_MAX_PLUS_1 // 새 기준은 이 줄 위로 추가하세요
};
enum D_SEME_PERFORM { D_SEME_PERFORM_IO, D_SEME_PERFORM_LS, D_SEME_PERFORM_CP };
enum D_SEME_RESIST { D_SEME_RESIST_RC_MFRAME, D_SEME_RESIST_RC_WALL, D_SEME_RESIST_ST_MFRAME, D_SEME_RESIST_ST_BFRAME };
enum D_SEME_ZONE { D_SEME_ZONE_1, D_SEME_ZONE_2 };
enum D_SEME_CLASS { D_SEME_CLASS_Sa1, D_SEME_CLASS_Sb2, D_SEME_CLASS_Sc3, D_SEME_CLASS_Sd4, D_SEME_CLASS_Se5, D_SEME_CLASS_S6 };
enum D_SEME_REINF { D_SEME_REINF_135, D_SEME_REINF_90, D_SEME_REINF_OTHER };
enum D_SEME_SHRSTR { D_SEME_SHRSTR_MOE2018, D_SEME_SHRSTR_STANDARD};
enum D_SEME_SHRSTR_DGN { D_SEME_SHRSTR_AIK_2021, D_SEME_SHRSTR_KDS_2022 };
enum D_SEME_EFF_DEPTH { D_SEME_EFF_DEPTH_08H, D_SEME_EFF_DEPTH_AUTO };
enum D_SEME_K1_AUTOUSER { D_SEME_K1_AUTO, D_SEME_K1_USER };
enum D_SEME_PMCM { D_SEME_PMCM_P, D_SEME_PMCM_E };
enum D_SEME_CP_LEVEL_OPT { D_SEME_USERINPUT, D_SEME_INTERMEDIATE};
enum D_SEME_SEISMIC_DETAIL { D_SEME_SEISMIC, D_SEME_NON_SEISMIC, D_SEME_AUTO };
enum D_SEME_SEISMIC_DETAIL_WALL { D_SEME_SPECIAL, D_SEME_RELAXED, D_SEME_NON_SPECIAL};
enum D_SEME_MOMENT_VC_BEAM { D_SEME_MU, D_SEME_MY };

#define T_SEME_K UINT



struct T_SEME_D_930
{
	int nMethod;
	int nPerformLevel;
	int nResistSystem;
	int nSeismicZone;
	int nSiteClass;
	double dMFactor;
	int nReinforcement;
	double dPhi_v;
	int nShearStrengthColumn_ln; // RC 기둥의 전단강도식 : D_SEME_SHRSTR
	int nShearStrengthColumn_mf;
	int nShearStrengthColumn_po;
	int nK1Factor_ln;
	int nK1Factor_mf;
	int nK1Factor_po;
	double dUserK1_ln;
	double dUserK1_mf;
	double dUserK1_po;
	int nEffDepthMethod_Beam_ln; // Linear Evaluation
	int nEffDepthMethod_Beam_mf; // m-factor
	int nEffDepthMethod_Beam_po; // Pushover
	int nEffDepthMethod_Colm_ln; // Linear Evaluation
	int nEffDepthMethod_Colm_mf; // m-factor
	int nEffDepthMethod_Colm_po; // Pushover
	BOOL bColumnHingePosConsiderInfillStrut; // Masonry Infill Strut을 고려하여 인접기둥 면내힌지위치 자동설정
	int nPMMethod_Colm_ln; // Linear Evaluation
	int nPMMethod_Colm_mf; // m-factor(1st)
	int nPMMethod_Colm_po; // Pushover(2nd)
	int nPMMethod_Wall_ln; // Linear Evaluation
	int nPMMethod_Wall_mf; // m-factor(1st)
	int nPMMethod_Wall_po; // Pushover(2nd)
	BOOL bEffectMasonry; // effect of Masonry Infilled Walls(KISTEC2019)
};

struct T_SEME_D_885
{
	int nMethod;
	int nPerformLevel;
	int nResistSystem;
	int nSeismicZone;
	int nSiteClass;
	double dMFactor;
	int nReinforcement;
	double dPhi_v;
	int nShearStrengthColumn_ln; // RC 기둥의 전단강도식 : D_SEME_SHRSTR
	int nShearStrengthColumn_mf;
	int nShearStrengthColumn_po;
	int nK1Factor_ln;
	int nK1Factor_mf;
	int nK1Factor_po;
	double dUserK1_ln;
	double dUserK1_mf;
	double dUserK1_po;
	int nEffDepthMethod_Beam_ln; // Linear Evaluation
	int nEffDepthMethod_Beam_mf; // m-factor
	int nEffDepthMethod_Beam_po; // Pushover
	int nEffDepthMethod_Colm_ln; // Linear Evaluation
	int nEffDepthMethod_Colm_mf; // m-factor
	int nEffDepthMethod_Colm_po; // Pushover
	BOOL bColumnHingePosConsiderInfillStrut; // Masonry Infill Strut을 고려하여 인접기둥 면내힌지위치 자동설정
};

struct T_SEME_D_885_temp
{
	int nMethod;
	int nPerformLevel;
	int nResistSystem;
	int nSeismicZone;
	int nSiteClass;
	double dMFactor;
	int nReinforcement;
	double dPhi_v;
	int nShearStrengthColumn_ln; // RC 기둥의 전단강도식 : D_SEME_SHRSTR
	int nShearStrengthColumn_mf;
	int nShearStrengthColumn_po;
	int nK1Factor_ln;
	int nK1Factor_mf;
	int nK1Factor_po;
	double dUserK1_ln;
	double dUserK1_mf;
	double dUserK1_po;
	int nEffDepthMethod_Beam_ln; // Linear Evaluation
	int nEffDepthMethod_Beam_mf; // m-factor
	int nEffDepthMethod_Beam_po; // Pushover
	int nEffDepthMethod_Colm_ln; // Linear Evaluation
	int nEffDepthMethod_Colm_mf; // m-factor
	int nEffDepthMethod_Colm_po; // Pushover
	BOOL bConsiderk1;
	BOOL bColumnHingePosConsiderInfillStrut; // Masonry Infill Strut을 고려하여 인접기둥 면내힌지위치 자동설정
};

struct T_SEME_D_883
{
	int nMethod;
	int nPerformLevel;
	int nResistSystem;
	int nSeismicZone;
	int nSiteClass;
	double dMFactor;
	int nReinforcement;
};

struct T_SEME_D_875
{
	int nMethod;
	int nPerformLevel;
	int nResistSystem;
	int nSeismicZone;
	int nSiteClass;
	double dMFactor;
};

struct T_SEME_D_870_m // 출시안됨
{
	int nMethod;
	int nPerformLevel;
	int nResistSystem;
	int nSeismicZone;
	int nSiteClass;
};

struct T_SEME_EVAL
{
	int nPerformLevel;
	int nResistSystem;
	int nSeismicZone;
	int nSiteClass;
	double dMFactor;
	int nReinforcement;
	double dPhi_v;
	int nShearStrengthColumn_ln; // RC 기둥의 전단강도식 : D_SEME_SHRSTR
	int nShearStrengthColumn_mf;
	int nShearStrengthColumn_po;
	int nK1Factor_ln;
	int nK1Factor_mf;
	int nK1Factor_po;
	double dUserK1_ln;
	double dUserK1_mf;
	double dUserK1_po;
	int nEffDepthMethod_Beam_ln; // Linear Evaluation
	int nEffDepthMethod_Beam_mf; // m-factor
	int nEffDepthMethod_Beam_po; // Pushover
	int nEffDepthMethod_Colm_ln; // Linear Evaluation
	int nEffDepthMethod_Colm_mf; // m-factor
	int nEffDepthMethod_Colm_po; // Pushover
	BOOL bColumnHingePosConsiderInfillStrut; // Masonry Infill Strut을 고려하여 인접기둥 면내힌지위치 자동설정
	int nPMMethod_Colm_ln; // Linear Evaluation
	int nPMMethod_Colm_mf; // m-factor(1st)
	int nPMMethod_Colm_po; // Pushover(2nd)
	int nPMMethod_Wall_ln; // Linear Evaluation
	int nPMMethod_Wall_mf; // m-factor(1st)
	int nPMMethod_Wall_po; // Pushover(2nd)
	BOOL bEffectMasonry; // effect of Masonry Infilled Walls(KISTEC2019)

	T_SEME_EVAL() { Initialize(); }
	T_SEME_EVAL(T_SEME_EVAL& src)
	{
		*this = src;
	}
	void Initialize()
	{
		nPerformLevel = D_SEME_PERFORM_IO;
		nResistSystem = D_SEME_RESIST_RC_MFRAME;
		nSeismicZone  = D_SEME_ZONE_1;
		nSiteClass    = D_SEME_CLASS_Sd4;
		dMFactor      = 1.0;
		nReinforcement= D_SEME_REINF_135;
		dPhi_v = 0.6;
		nShearStrengthColumn_ln = D_SEME_SHRSTR_MOE2018;
		nShearStrengthColumn_mf = D_SEME_SHRSTR_MOE2018;
		nShearStrengthColumn_po = D_SEME_SHRSTR_MOE2018;
		nK1Factor_ln = D_SEME_K1_AUTO;
		nK1Factor_mf = D_SEME_K1_AUTO;
		nK1Factor_po = D_SEME_K1_AUTO;
		dUserK1_ln = 0.0;
		dUserK1_mf = 0.0;
		dUserK1_po = 0.0;
		nEffDepthMethod_Beam_ln = D_SEME_EFF_DEPTH_AUTO;
		nEffDepthMethod_Beam_mf = D_SEME_EFF_DEPTH_AUTO;
		nEffDepthMethod_Beam_po = D_SEME_EFF_DEPTH_AUTO;
		nEffDepthMethod_Colm_ln = D_SEME_EFF_DEPTH_08H;
		nEffDepthMethod_Colm_mf = D_SEME_EFF_DEPTH_08H;
		nEffDepthMethod_Colm_po = D_SEME_EFF_DEPTH_08H;
		bColumnHingePosConsiderInfillStrut = FALSE;
		nPMMethod_Colm_ln = D_SEME_PMCM_E;
		nPMMethod_Colm_mf = D_SEME_PMCM_P;
		nPMMethod_Colm_po = D_SEME_PMCM_E;
		nPMMethod_Wall_ln = D_SEME_PMCM_E;
		nPMMethod_Wall_mf = D_SEME_PMCM_P;
		nPMMethod_Wall_po = D_SEME_PMCM_E;
		bEffectMasonry = FALSE;
	}
};

struct T_SEME_PFRM
{
	int nPerformLevel;
	int nCPLevelOption;
	double dCPLevelLimit;
	int nSeismicDetailBeam;      // Seismic rebar detail : auto calc (default)
	int nSeismicDetailColm;      // Seismic rebar detail : Seismic Detail (default)
	int nSeismicDetailWall;      // Seismic rebar detail : D_SEME_NON_SPECIAL
	int nShearStrengthColm_PO;   // Pushover     : D_SEME_SHRSTR ( AIK, KDS )
	int nShearStrengthColm_TH;   // Time history : D_SEME_SHRSTR ( AIK, KDS )
	int nEffDepthMethod_Beam_PO; // Pushover     : D_SEME_EFF_DEPTH ( 0.8h, Auto )
	int nEffDepthMethod_Beam_TH; // Time history : D_SEME_EFF_DEPTH ( 0.8h, Auto )
	int nEffDepthMethod_Colm_PO; // Pushover     : D_SEME_EFF_DEPTH ( 0.8h, Auto )
	int nEffDepthMethod_Colm_TH; // Time history : D_SEME_EFF_DEPTH ( 0.8h, Auto )
	int nMomentVcBeamHinge;      // Moment for Vc of Beam Hinge
	double dPlasticHingeLengthFactor;  // Plastic hinge length for fiber wall
	double dShearSpanLs_z_Beam_i; // Shear span of beam (Ls) : i-end
	double dShearSpanLs_z_Beam_j; // Shear span of beam (Ls) : j-end
	double dShearSpanLs_y_Colm_i; // Shear span of column (Ls_y) : i-end
	double dShearSpanLs_y_Colm_j; // Shear span of column (Ls_y) : j-end
	double dShearSpanLs_z_Colm_i; // Shear span of column (Ls_z) : i-end
	double dShearSpanLs_z_Colm_j; // Shear span of column (Ls_z) : j-end
	BOOL bIgnoreYeildRotaFWall;   // Ignore Yield Rotation of Fiber Wall
	double dAllowStoryDriftR_IO;  // Allowable Story Drift Ratio (IO)
	double dAllowStoryDriftR_LS;  // Allowable Story Drift Ratio (LS)
	double dAllowStoryDriftR_CP;  // Allowable Story Drift Ratio (CP)
	BOOL bAutoGenerateTHFile;
	T_SEME_PFRM() { Initialize(); }
	T_SEME_PFRM(T_SEME_PFRM& src)
	{
		*this = src;
	}
	void Initialize()
	{
		nPerformLevel = D_SEME_PERFORM_IO;
		nCPLevelOption = D_SEME_USERINPUT;
		dCPLevelLimit  = 1.0;
		nSeismicDetailBeam = D_SEME_AUTO;
		nSeismicDetailColm = D_SEME_SEISMIC;
		nSeismicDetailWall = D_SEME_NON_SPECIAL;
		nShearStrengthColm_PO = D_SEME_SHRSTR_AIK_2021;
		nShearStrengthColm_TH = D_SEME_SHRSTR_AIK_2021;
		nEffDepthMethod_Beam_PO = D_SEME_EFF_DEPTH_AUTO;
		nEffDepthMethod_Beam_TH = D_SEME_EFF_DEPTH_AUTO;
		nEffDepthMethod_Colm_PO = D_SEME_EFF_DEPTH_AUTO;
		nEffDepthMethod_Colm_TH = D_SEME_EFF_DEPTH_AUTO;
		nMomentVcBeamHinge = D_SEME_MY;
		dPlasticHingeLengthFactor = 0.5;
		dShearSpanLs_z_Beam_i = 0.5;
		dShearSpanLs_z_Beam_j = 0.5;
		dShearSpanLs_y_Colm_i = 0.5;
		dShearSpanLs_y_Colm_j = 0.5;
		dShearSpanLs_z_Colm_i = 0.5;
		dShearSpanLs_z_Colm_j = 0.5;
		bIgnoreYeildRotaFWall = TRUE;
		dAllowStoryDriftR_IO = 0.005;
		dAllowStoryDriftR_LS = 0.015;
		dAllowStoryDriftR_CP = 0.020;
		bAutoGenerateTHFile = FALSE;
	}
};

struct T_SEME_PFRM_940
{
	int nPerformLevel;
	int nCPLevelOption;
	double dCPLevelLimit;
	int nSeismicDetailBeam;      // Seismic rebar detail : auto calc (default)
	int nSeismicDetailColm;      // Seismic rebar detail : Seismic Detail (default)
	int nSeismicDetailWall;      // Seismic rebar detail : D_SEME_NON_SPECIAL
	int nShearStrengthColm_PO;   // Pushover     : D_SEME_SHRSTR ( AIK, KDS )
	int nShearStrengthColm_TH;   // Time history : D_SEME_SHRSTR ( AIK, KDS )
	int nEffDepthMethod_Beam_PO; // Pushover     : D_SEME_EFF_DEPTH ( 0.8h, Auto )
	int nEffDepthMethod_Beam_TH; // Time history : D_SEME_EFF_DEPTH ( 0.8h, Auto )
	int nEffDepthMethod_Colm_PO; // Pushover     : D_SEME_EFF_DEPTH ( 0.8h, Auto )
	int nEffDepthMethod_Colm_TH; // Time history : D_SEME_EFF_DEPTH ( 0.8h, Auto )
	int nMomentVcBeamHinge;      // Moment for Vc of Beam Hinge
	double dPlasticHingeLengthFactor;  // Plastic hinge length for fiber wall
	double dShearSpanLs_z_Beam_i; // Shear span of beam (Ls) : i-end
	double dShearSpanLs_z_Beam_j; // Shear span of beam (Ls) : j-end
	double dShearSpanLs_y_Colm_i; // Shear span of column (Ls_y) : i-end
	double dShearSpanLs_y_Colm_j; // Shear span of column (Ls_y) : j-end
	double dShearSpanLs_z_Colm_i; // Shear span of column (Ls_z) : i-end
	double dShearSpanLs_z_Colm_j; // Shear span of column (Ls_z) : j-end
	BOOL bIgnoreYeildRotaFWall;   // Ignore Yield Rotation of Fiber Wall
	double dAllowStoryDriftR_IO;  // Allowable Story Drift Ratio (IO)
	double dAllowStoryDriftR_LS;  // Allowable Story Drift Ratio (LS)
	double dAllowStoryDriftR_CP;  // Allowable Story Drift Ratio (CP)
};

union T_SEME_DATA_940
{
	T_SEME_EVAL eval;      // 내진성능평가
	T_SEME_PFRM_940 design;    // 성능기반설계

	T_SEME_DATA_940() {}
};

struct T_SEME_D_940
{
	int nMethod;
	T_SEME_DATA_940 data;
};

union T_SEME_DATA
{
	T_SEME_EVAL eval;      // 내진성능평가
	T_SEME_PFRM design;    // 성능기반설계

	T_SEME_DATA() {}
};

struct T_SEME_D
{
	int nMethod;
	T_SEME_DATA data;

	T_SEME_D() { Initialize(); }
	T_SEME_D(T_SEME_D& src)
	{
		*this = src;
	}
	void Initialize()
	{
		Initialize(D_SEME_METHOD_KISTEC2019);
	}
	void Initialize(int Method)
	{
		nMethod = Method;
		switch ( nMethod )
		{
		case D_SEME_METHOD_KISTEC2013:
		case D_SEME_METHOD_MOE2018:
		case D_SEME_METHOD_MOE2019:
		case D_SEME_METHOD_KISTEC2019:
			{
				data.eval.Initialize();
			}
			break;
		case D_SEME_METHOD_AIK_G_001_2021:
			{
				data.design.Initialize();
			}
			break;
		default: ASSERT(0);
		}

		return;
	}

	bool IsEvaluation()
	{
		switch ( nMethod )
		{
		case D_SEME_METHOD_KISTEC2013:
		case D_SEME_METHOD_MOE2018:
		case D_SEME_METHOD_MOE2019:
		case D_SEME_METHOD_KISTEC2019:
			return true;  // 내진성능평가
		case D_SEME_METHOD_AIK_G_001_2021:
			return false; // 성능기반설계
		default: ASSERT(0); return false;
		}
	}

	void Convert940(T_SEME_D_940& rData)
	{
		Initialize(rData.nMethod);
		if (nMethod == D_SEME_METHOD_AIK_G_001_2021)
		{
			data.design.nPerformLevel = rData.data.design.nPerformLevel;
			data.design.nCPLevelOption = rData.data.design.nCPLevelOption;
			data.design.dCPLevelLimit = rData.data.design.dCPLevelLimit;
			data.design.nSeismicDetailBeam = rData.data.design.nSeismicDetailBeam;
			data.design.nSeismicDetailColm = rData.data.design.nSeismicDetailColm;
			data.design.nSeismicDetailWall = rData.data.design.nSeismicDetailWall;
			data.design.nShearStrengthColm_PO = rData.data.design.nShearStrengthColm_PO;
			data.design.nShearStrengthColm_TH = rData.data.design.nShearStrengthColm_TH;
			data.design.nEffDepthMethod_Beam_PO = rData.data.design.nEffDepthMethod_Beam_PO;
			data.design.nEffDepthMethod_Beam_TH = rData.data.design.nEffDepthMethod_Beam_TH;
			data.design.nEffDepthMethod_Colm_PO = rData.data.design.nEffDepthMethod_Colm_PO;
			data.design.nEffDepthMethod_Colm_TH = rData.data.design.nEffDepthMethod_Colm_TH;
			data.design.nMomentVcBeamHinge = rData.data.design.nMomentVcBeamHinge;
			data.design.dPlasticHingeLengthFactor = rData.data.design.dPlasticHingeLengthFactor;
			data.design.dShearSpanLs_z_Beam_i = rData.data.design.dShearSpanLs_z_Beam_i;
			data.design.dShearSpanLs_z_Beam_j = rData.data.design.dShearSpanLs_z_Beam_j;
			data.design.dShearSpanLs_y_Colm_i = rData.data.design.dShearSpanLs_y_Colm_i;
			data.design.dShearSpanLs_y_Colm_j = rData.data.design.dShearSpanLs_y_Colm_j;
			data.design.dShearSpanLs_z_Colm_i = rData.data.design.dShearSpanLs_z_Colm_i;
			data.design.dShearSpanLs_z_Colm_j = rData.data.design.dShearSpanLs_z_Colm_j;
			data.design.bIgnoreYeildRotaFWall = rData.data.design.bIgnoreYeildRotaFWall;
			data.design.dAllowStoryDriftR_IO = rData.data.design.dAllowStoryDriftR_IO;
			data.design.dAllowStoryDriftR_LS = rData.data.design.dAllowStoryDriftR_LS;
			data.design.dAllowStoryDriftR_CP = rData.data.design.dAllowStoryDriftR_CP;
			data.design.bAutoGenerateTHFile = TRUE;
		}
		else data.eval = rData.data.eval;
	}
	void Convert930(T_SEME_D_930& rData)
	{
		Initialize();
		nMethod = rData.nMethod;
		ASSERT(nMethod != D_SEME_METHOD_AIK_G_001_2021);
		data.eval.nPerformLevel = rData.nPerformLevel;
		data.eval.nResistSystem = rData.nResistSystem;
		data.eval.nSeismicZone  = rData.nSeismicZone;
		data.eval.nSiteClass    = rData.nSiteClass;
		data.eval.dMFactor      = rData.dMFactor;
		data.eval.nReinforcement = rData.nReinforcement;
		data.eval.dPhi_v = rData.dPhi_v;
		data.eval.nShearStrengthColumn_ln = rData.nShearStrengthColumn_ln;
		data.eval.nShearStrengthColumn_mf = rData.nShearStrengthColumn_mf;
		data.eval.nShearStrengthColumn_po = rData.nShearStrengthColumn_po;
		data.eval.nK1Factor_ln = rData.nK1Factor_ln;
		data.eval.nK1Factor_mf = rData.nK1Factor_mf;
		data.eval.nK1Factor_po = rData.nK1Factor_po;
		data.eval.dUserK1_ln = rData.dUserK1_ln;
		data.eval.dUserK1_mf = rData.dUserK1_mf;
		data.eval.dUserK1_po = rData.dUserK1_po;
		data.eval.nEffDepthMethod_Beam_ln = rData.nEffDepthMethod_Beam_ln;
		data.eval.nEffDepthMethod_Beam_mf = rData.nEffDepthMethod_Beam_mf;
		data.eval.nEffDepthMethod_Beam_po = rData.nEffDepthMethod_Beam_po;
		data.eval.nEffDepthMethod_Colm_ln = rData.nEffDepthMethod_Colm_ln;
		data.eval.nEffDepthMethod_Colm_mf = rData.nEffDepthMethod_Colm_mf;
		data.eval.nEffDepthMethod_Colm_po = rData.nEffDepthMethod_Colm_po;
		data.eval.bColumnHingePosConsiderInfillStrut = rData.bColumnHingePosConsiderInfillStrut;
		data.eval.nPMMethod_Colm_ln = rData.nPMMethod_Colm_ln;
		data.eval.nPMMethod_Colm_mf = rData.nPMMethod_Colm_mf;
		data.eval.nPMMethod_Colm_po = rData.nPMMethod_Colm_po;
		data.eval.nPMMethod_Wall_ln = rData.nPMMethod_Wall_ln;
		data.eval.nPMMethod_Wall_mf = rData.nPMMethod_Wall_mf;
		data.eval.nPMMethod_Wall_po = rData.nPMMethod_Wall_po;
		data.eval.bEffectMasonry = rData.bEffectMasonry;
	}
	void Convert885(T_SEME_D_885& rData)
	{
		Initialize();
		nMethod         = rData.nMethod;
		data.eval.nPerformLevel   = rData.nPerformLevel;
		data.eval.nResistSystem   = rData.nResistSystem;
		data.eval.nSeismicZone    = rData.nSeismicZone;
		data.eval.nSiteClass      = rData.nSiteClass;
		data.eval.dMFactor        = rData.dMFactor;
		data.eval.nReinforcement  = rData.nReinforcement;
		data.eval.dPhi_v          = rData.dPhi_v;
		data.eval.nShearStrengthColumn_ln = rData.nShearStrengthColumn_ln;
		data.eval.nShearStrengthColumn_mf = rData.nShearStrengthColumn_mf;
		data.eval.nShearStrengthColumn_po = rData.nShearStrengthColumn_po;
		data.eval.nK1Factor_ln = rData.nK1Factor_ln;
		data.eval.nK1Factor_mf = rData.nK1Factor_mf;
		data.eval.nK1Factor_po = rData.nK1Factor_po;
		data.eval.dUserK1_ln = rData.dUserK1_ln;
		data.eval.dUserK1_mf = rData.dUserK1_mf;
		data.eval.dUserK1_po = rData.dUserK1_po;
		data.eval.nEffDepthMethod_Beam_ln = rData.nEffDepthMethod_Beam_ln;
		data.eval.nEffDepthMethod_Beam_mf = rData.nEffDepthMethod_Beam_mf;
		data.eval.nEffDepthMethod_Beam_po = rData.nEffDepthMethod_Beam_po;
		data.eval.nEffDepthMethod_Colm_ln = rData.nEffDepthMethod_Colm_ln;
		data.eval.nEffDepthMethod_Colm_mf = rData.nEffDepthMethod_Colm_mf;
		data.eval.nEffDepthMethod_Colm_po = rData.nEffDepthMethod_Colm_po;
		data.eval.bColumnHingePosConsiderInfillStrut = rData.bColumnHingePosConsiderInfillStrut;
	}
	void Convert885temp(T_SEME_D_885_temp& rData)
	{
		Initialize();
		nMethod       = rData.nMethod;
		data.eval.nPerformLevel = rData.nPerformLevel;
		data.eval.nResistSystem = rData.nResistSystem;
		data.eval.nSeismicZone  = rData.nSeismicZone;
		data.eval.nSiteClass    = rData.nSiteClass;
		data.eval.dMFactor      = rData.dMFactor;
		data.eval.nReinforcement= rData.nReinforcement;
		data.eval.dPhi_v = rData.dPhi_v;
		data.eval.nShearStrengthColumn_ln = rData.nShearStrengthColumn_ln;
		data.eval.nShearStrengthColumn_mf = rData.nShearStrengthColumn_mf;
		data.eval.nShearStrengthColumn_po = rData.nShearStrengthColumn_po;
		data.eval.nK1Factor_ln = rData.nK1Factor_ln;
		data.eval.nK1Factor_mf = rData.nK1Factor_mf;
		data.eval.nK1Factor_po = rData.nK1Factor_po;
		data.eval.dUserK1_ln = rData.dUserK1_ln;
		data.eval.dUserK1_mf = rData.dUserK1_mf;
		data.eval.dUserK1_po = rData.dUserK1_po;
		data.eval.nEffDepthMethod_Beam_ln = rData.nEffDepthMethod_Beam_ln;
		data.eval.nEffDepthMethod_Beam_mf = rData.nEffDepthMethod_Beam_mf;
		data.eval.nEffDepthMethod_Beam_po = rData.nEffDepthMethod_Beam_po;
		data.eval.nEffDepthMethod_Colm_ln = rData.nEffDepthMethod_Colm_ln;
		data.eval.nEffDepthMethod_Colm_mf = rData.nEffDepthMethod_Colm_mf;
		data.eval.nEffDepthMethod_Colm_po = rData.nEffDepthMethod_Colm_po;
	}
	void Convert883(T_SEME_D_883& rData)
	{
		Initialize();
		nMethod       = rData.nMethod;
		data.eval.nPerformLevel = rData.nPerformLevel;
		data.eval.nResistSystem = rData.nResistSystem;
		data.eval.nSeismicZone  = rData.nSeismicZone;
		data.eval.nSiteClass    = rData.nSiteClass;
		data.eval.dMFactor      = rData.dMFactor;
		data.eval.nReinforcement= rData.nReinforcement;
	}
	void Convert875(T_SEME_D_875& rData)
	{
		Initialize();
		nMethod       = rData.nMethod;
		data.eval.nPerformLevel = rData.nPerformLevel;
		data.eval.nResistSystem = rData.nResistSystem;
		data.eval.nSeismicZone  = rData.nSeismicZone;
		data.eval.nSiteClass    = rData.nSiteClass;
		data.eval.dMFactor      = rData.dMFactor;
	}
	void Convert870m(T_SEME_D_870_m& rData)
	{
		Initialize();
		nMethod       = rData.nMethod;
		data.eval.nPerformLevel = rData.nPerformLevel;
		data.eval.nResistSystem = rData.nResistSystem;
		data.eval.nSeismicZone  = rData.nSeismicZone;
		data.eval.nSiteClass    = rData.nSiteClass;
	}
};

struct T_SEME_UDRD_D
{
	T_SEME_D data;
};
#define HASHSIZESEME 1

struct T_SECM_TESTING_D_875
{
	BOOL   bUseDefaultVal;
	double dConcFce; // Default : Preference에 있는 Concrete의 기본 재료강도 X 1.2를 한 값이 보여짐
	double dConcLBSFactor;
	double dRebrMainFye; // Default : Preference에 있는 Rebar의 기본 재료강도 X 1.25를 한 값이 보여짐
	double dRebrSubFye;
	double dRebrLBSFactor;
};

struct T_SECM_DEF_STR_D_910
{
	int    nTimeFrame; // T_SECM_KISTEC2013_DEF_STR_YEAR
};

// Seismic Evaluation Method - Testing
struct T_SECM_TESTING_D
{
	BOOL   bUseDefaultVal;
	double dConcFce; // Default : Preference에 있는 Concrete의 기본 재료강도 X 1.2를 한 값이 보여짐
	double dConcLBSFactor;
	double dRebrMainFye; // Default : Preference에 있는 Rebar의 기본 재료강도 X 1.25를 한 값이 보여짐
	double dRebrSubFye;
	double dRebrLBSFactor;
	double dConcFck;
	double dRebrMainFy;
	double dRebrSubFy;
	void Initialize(int nMethod)
	{
		bUseDefaultVal = TRUE;
		dConcFce       = 0.0;
		dConcLBSFactor = 1.2;
		dRebrMainFye   = 0.0;
		dRebrSubFye    = 0.0;
		dRebrLBSFactor = 1.25;
		dConcFck       = 0.0;
		dRebrMainFy      = 0.0;
		dRebrSubFy       = 0.0;
	}
	void Convert875(T_SECM_TESTING_D_875& rData)
	{
		bUseDefaultVal = rData.bUseDefaultVal;
		dConcFce       = rData.dConcFce;
		dConcLBSFactor = rData.dConcLBSFactor;
		dRebrMainFye   = rData.dRebrMainFye;
		dRebrSubFye    = rData.dRebrSubFye;
		dRebrLBSFactor = rData.dRebrLBSFactor;
		dConcFck       = 0.0;
		dRebrMainFy      = 0.0;
		dRebrSubFy       = 0.0;
	}
	double GetConcFck(int nMethod) { return nMethod==D_SEME_METHOD_KISTEC2013 ? ( dConcLBSFactor>0 ? dConcFce     / dConcLBSFactor : 0.0 ) : dConcFck; }
	double GetRebrMainFy(int nMethod) { return nMethod==D_SEME_METHOD_KISTEC2013 ? ( dRebrLBSFactor>0 ? dRebrMainFye / dRebrLBSFactor : 0.0 ) : dRebrMainFy; }
	double GetRebrSubFy(int nMethod) { return nMethod==D_SEME_METHOD_KISTEC2013 ? ( dRebrLBSFactor>0 ? dRebrSubFye  / dRebrLBSFactor : 0.0 ) : dRebrSubFy; }
};

// Seismic Evaluation Method - Construction Document
enum T_SECM_CONS_DOC_YEAR { T_SECM_CONS_DOC_YEAR_0_9, T_SECM_CONS_DOC_YEAR_10_19, T_SECM_CONS_DOC_YEAR_20_29, T_SECM_CONS_DOC_YEAR_30_ };
enum T_SECM_CONS_DOC_MAT { T_SECM_CONS_DOC_MAT_GOOD, T_SECM_CONS_DOC_MAT_NORMAL, T_SECM_CONS_DOC_MAT_POOR };
struct T_SECM_CONS_DOC_D
{
	BOOL   bUseDefaultVal;
	double dConcFck; // Default : Preference에 있는 기본 재료강도가 보여짐
	double dConcESFactor;
	int    nConcElapsYear; // D_SEMD_YEAR
	int    nConcMatCond; // D_SEMD_MAT
	double dConcRedFactor;
	double dRebrMainFy; // Default : Preference에 있는 기본 재료강도가 보여짐
	double dRebrSubFy;
	double dRebrESFactor;
	void Initialize(int nMethod)
	{
		bUseDefaultVal = TRUE;
		dConcFck        = 0.0;
		dConcESFactor   = 1.2;
		nConcElapsYear  = T_SECM_CONS_DOC_YEAR_0_9;
		nConcMatCond    = T_SECM_CONS_DOC_MAT_GOOD;
		dConcRedFactor  = 1.0;
		dRebrMainFy     = 0.0;
		dRebrSubFy      = 0.0;
		dRebrESFactor   = 1.25;
	}
	double GetConcRedFactor()
	{
		const double aYearFactor[] = { 1.0, 1.0, 0.9, 0.8 };
		const double aMatlFactor[] = { 1.0, 0.9, 0.8 };
		return dConcRedFactor = aYearFactor[nConcElapsYear] * aMatlFactor[nConcMatCond];
	}
	double GetResultFck() { return dConcRedFactor     > 0 ? dConcFck       * dConcRedFactor : 0.0; }
	double GetConcFce() { return dConcESFactor      > 0 ? GetResultFck() * dConcESFactor : 0.0; }
	double GetRebrMainFye() { return dRebrESFactor      > 0 ? dRebrMainFy    * dRebrESFactor : 0.0; }
	double GetRebrSubFye() { return dRebrESFactor      > 0 ? dRebrSubFy     * dRebrESFactor : 0.0; }
};

// Seismic Evaluation Method - Default String
enum T_SECM_KISTEC2013_DEF_STR_YEAR { T_SECM_KISTEC2013_DEF_STR_YEAR_0_1970, T_SECM_KISTEC2013_DEF_STR_YEAR_1971_1987, T_SECM_KISTEC2013_DEF_STR_YEAR_1988_2000, T_SECM_KISTEC2013_DEF_STR_YEAR_2001_ };
enum T_SECM_MOE2018_DEF_STR_YEAR { T_SECM_MOE2018_DEF_STR_YEAR_0_1969, T_SECM_MOE2018_DEF_STR_YEAR_1970_1988, T_SECM_MOE2018_DEF_STR_YEAR_1989_2000, T_SECM_MOE2018_DEF_STR_YEAR_2001_ };
struct T_SECM_DEF_STR_D
{
	int    nTimeFrame; // T_SECM_KISTEC2013_DEF_STR_YEAR
	int    nConcMatCond; // D_SEMD_MAT
	double dConcRedFactor;
	void Initialize(int nMethod)
	{
		nTimeFrame = -1;
		nConcMatCond = T_SECM_CONS_DOC_MAT_GOOD;
		dConcRedFactor = 1.0;

		switch ( nMethod )
		{
		case D_SEME_METHOD_KISTEC2013:
		case D_SEME_METHOD_KISTEC2019:
			nTimeFrame = T_SECM_KISTEC2013_DEF_STR_YEAR_0_1970;
			break;
		case D_SEME_METHOD_MOE2018:
		case D_SEME_METHOD_MOE2019:
			nTimeFrame = T_SECM_MOE2018_DEF_STR_YEAR_0_1969;
			break;
		default:
			ASSERT(0);
			break;
		}
	}

	void Convert910(T_SECM_DEF_STR_D_910& rData)
	{
		nTimeFrame = rData.nTimeFrame;
		nConcMatCond = T_SECM_CONS_DOC_MAT_GOOD;
		dConcRedFactor = 1.0;
	}
};

struct T_SECM_GENERAL_D_875
{
	T_SECM_TESTING_D_875   testing;
	T_SECM_CONS_DOC_D      document;
	T_SECM_DEF_STR_D_910   defStr;
};

struct T_SECM_GENERAL_D_910
{
	T_SECM_TESTING_D        testing;
	T_SECM_CONS_DOC_D       document;
	T_SECM_DEF_STR_D_910    defStr;
};

struct T_SECM_GENERAL_D
{
	T_SECM_TESTING_D   testing;
	T_SECM_CONS_DOC_D  document;
	T_SECM_DEF_STR_D   defStr;
};

union T_SECM_METHOD_D_875
{
	T_SECM_GENERAL_D_875 General;
};

union T_SECM_METHOD_D_910
{
	T_SECM_GENERAL_D_910 General;
};

union T_SECM_METHOD_D
{
	T_SECM_GENERAL_D General;

	void Initialize(int nMethod)
	{
		switch ( nMethod )
		{
		case D_SEME_METHOD_KISTEC2013:
		case D_SEME_METHOD_MOE2018:
		case D_SEME_METHOD_MOE2019:
		case D_SEME_METHOD_KISTEC2019:
		case D_SEME_METHOD_AIK_G_001_2021:
			General.testing.Initialize(nMethod);
			General.document.Initialize(nMethod);
			General.defStr.Initialize(nMethod);
			break;
		default:
			ASSERT(0);
			break;
		}
	}
	void Convert875(T_SECM_METHOD_D_875& rData)
	{
		General.testing.Convert875(rData.General.testing);
		General.document = rData.General.document;
		General.defStr.Convert910(rData.General.defStr);
	}
	void Convert910(T_SECM_METHOD_D_910& rData)
	{
		General.testing = rData.General.testing;
		General.document = rData.General.document;
		General.defStr.Convert910(rData.General.defStr);
	}
	T_SECM_TESTING_D* GetTesting(int nMethod)
	{
		switch ( nMethod )
		{
		case D_SEME_METHOD_KISTEC2013:
		case D_SEME_METHOD_MOE2018:
		case D_SEME_METHOD_MOE2019:
		case D_SEME_METHOD_KISTEC2019:
		case D_SEME_METHOD_AIK_G_001_2021:
			return &General.testing;
		default:
			ASSERT(0);
			break;
		}
		return &General.testing;
	}

	T_SECM_CONS_DOC_D* GetDocument(int nMethod)
	{
		switch ( nMethod )
		{
		case D_SEME_METHOD_KISTEC2013:
		case D_SEME_METHOD_MOE2018:
		case D_SEME_METHOD_MOE2019:
		case D_SEME_METHOD_KISTEC2019:
		case D_SEME_METHOD_AIK_G_001_2021:
			return &General.document;
		default:
			ASSERT(0);
			break;
		}
		return &General.document;
	}

	T_SECM_DEF_STR_D* GetDefault(int nMethod)
	{
		switch ( nMethod )
		{
		case D_SEME_METHOD_KISTEC2013:
		case D_SEME_METHOD_MOE2018:
		case D_SEME_METHOD_MOE2019:
		case D_SEME_METHOD_KISTEC2019:
		case D_SEME_METHOD_AIK_G_001_2021:
			return &General.defStr;
		default:
			ASSERT(0);
			break;
		}
		return &General.defStr;
	}
};

enum D_SECM_METHOD { D_SECM_METHOD_TESTING, D_SECM_METHOD_CONS_DOC, D_SECM_METHOD_DEF_STRENGTH };
enum D_SECM_STRMODE { D_SECM_STRMODE_USER, D_SECM_STRMODE_AUTO };
// Seismic Evaluation - Concrete Material
#define T_SECM_K UINT

struct T_SECM_D_875
{
	BOOL bUseDefaultVal;
	int  nConcPreliminary; // D_SECM_METHOD
	int  nConcTier1;
	int  nConcTier2;
	int  nRebrPreliminary;
	int  nRebrTier1;
	int  nRebrTier2;
	int  nStrengthMode; // D_SECM_STRMODE
	int  nTimeFrame; // T_SECM_KISTEC2013_DEF_STR_YEAR
	T_SECM_METHOD_D_875 method;

	// index = 0:Conc-fck, 1:Conc-fce, 2:Main-fy, 3:Main-fye, 4:Sub-Fy, 5:Sub-fye
	double dResult_Eval1[6];
	double dResult_Eval2[6];
	double dResult_Eval3[6];
};

struct T_SECM_D_870_m
{
	BOOL bUseDefaultVal;
	int nConcPreliminary; // D_SECM_METHOD
	int nConcTier1;
	int nConcTier2;
	int nRebrPreliminary;
	int nRebrTier1;
	int nRebrTier2;
	T_SECM_METHOD_D_875 method;

	// index = 0:Conc-fck, 1:Conc-fce, 2:Main-fy, 3:Main-fye, 4:Sub-Fy, 5:Sub-fye
	double dResult_Eval1[6];
	double dResult_Eval2[6];
	double dResult_Eval3[6];
};

struct T_SECM_D_910
{
	BOOL bUseDefaultVal;
	int  nConcPreliminary; // D_SECM_METHOD
	int  nConcTier1;
	int  nConcTier2;
	int  nRebrPreliminary;
	int  nRebrTier1;
	int  nRebrTier2;
	int  nStrengthMode; // D_SECM_STRMODE
	int  nTimeFrame; // T_SECM_KISTEC2013_DEF_STR_YEAR
	T_SECM_METHOD_D_910 method;

	// index = 0:Conc-fck, 1:Conc-fce, 2:Main-fy, 3:Main-fye, 4:Sub-Fy, 5:Sub-fye
	double dResult_Eval1[6]; // KISTEC2013 : pre, MOE2018 : Linear Eval.
	double dResult_Eval2[6];    // KISTEC2013 : 1st, MOE2018 : m-Factor
	double dResult_Eval3[6];    // KISTEC2013 : 2nd, MOE2018 : Pushover
};

struct T_SECM_D
{
	BOOL bUseDefaultVal;
	int  nConcPreliminary; // D_SECM_METHOD
	int  nConcTier1;
	int  nConcTier2;
	int  nRebrPreliminary;
	int  nRebrTier1;
	int  nRebrTier2;
	int  nStrengthMode; // D_SECM_STRMODE
	int  nTimeFrame; // T_SECM_KISTEC2013_DEF_STR_YEAR
	T_SECM_METHOD_D method;

	// index = 0:Conc-fck, 1:Conc-fce, 2:Main-fy, 3:Main-fye, 4:Sub-Fy, 5:Sub-fye
	double dResult_Eval1[6]; // KISTEC2013 : pre, MOE2018 : Linear Eval.
	double dResult_Eval2[6];    // KISTEC2013 : 1st, MOE2018 : m-Factor
	double dResult_Eval3[6];    // KISTEC2013 : 2nd, MOE2018 : Pushover

	void Initialize(int nMethod)
	{
		bUseDefaultVal   = TRUE;
		nConcPreliminary = D_SECM_METHOD_TESTING;
		nConcTier1       = D_SECM_METHOD_TESTING;
		nConcTier2       = D_SECM_METHOD_TESTING;
		nRebrPreliminary = D_SECM_METHOD_TESTING;
		nRebrTier1       = D_SECM_METHOD_TESTING;
		nRebrTier2       = D_SECM_METHOD_TESTING;
		nStrengthMode    = D_SECM_STRMODE_USER;

		switch ( nMethod )
		{
		case D_SEME_METHOD_KISTEC2013:
		case D_SEME_METHOD_KISTEC2019:
			nTimeFrame = T_SECM_KISTEC2013_DEF_STR_YEAR_0_1970;
			break;
		case D_SEME_METHOD_MOE2018:
		case D_SEME_METHOD_MOE2019:
			nTimeFrame = T_SECM_MOE2018_DEF_STR_YEAR_0_1969;
			break;
		case D_SEME_METHOD_AIK_G_001_2021:
			break;
		default:
			ASSERT(0);
			break;
		}

		method.Initialize(nMethod);

		memset(dResult_Eval1, 0, sizeof(dResult_Eval1));
		memset(dResult_Eval2, 0, sizeof(dResult_Eval2));
		memset(dResult_Eval3, 0, sizeof(dResult_Eval3));
	}
	void Convert870m(T_SECM_D_870_m& rData)
	{
		bUseDefaultVal   = rData.bUseDefaultVal;
		nConcPreliminary = rData.nConcPreliminary;
		nConcTier1       = rData.nConcTier1;
		nConcTier2       = rData.nConcTier2;
		nRebrPreliminary = rData.nRebrPreliminary;
		nRebrTier1       = rData.nRebrTier1;
		nRebrTier2       = rData.nRebrTier2;
		nStrengthMode    = D_SECM_STRMODE_USER;
		nTimeFrame       = T_SECM_KISTEC2013_DEF_STR_YEAR_0_1970;
		method.Convert875(rData.method);

		if ( nConcTier1 == D_SECM_METHOD_CONS_DOC || nConcTier2 == D_SECM_METHOD_CONS_DOC )
			nStrengthMode = D_SECM_STRMODE_USER;

		memcpy(dResult_Eval1, rData.dResult_Eval1, sizeof(dResult_Eval1));
		memcpy(dResult_Eval2, rData.dResult_Eval2, sizeof(dResult_Eval2));
		memcpy(dResult_Eval3, rData.dResult_Eval3, sizeof(dResult_Eval3));
	}
	void Convert875(T_SECM_D_875& rData)
	{
		bUseDefaultVal   = rData.bUseDefaultVal;
		nConcPreliminary = rData.nConcPreliminary;
		nConcTier1       = rData.nConcTier1;
		nConcTier2       = rData.nConcTier2;
		nRebrPreliminary = rData.nRebrPreliminary;
		nRebrTier1       = rData.nRebrTier1;
		nRebrTier2       = rData.nRebrTier2;
		nStrengthMode    = rData.nStrengthMode;
		nTimeFrame       = rData.nTimeFrame;
		method.Convert875(rData.method);

		if ( nConcTier1 == D_SECM_METHOD_CONS_DOC || nConcTier2 == D_SECM_METHOD_CONS_DOC )
			nStrengthMode = D_SECM_STRMODE_USER;

		memcpy(dResult_Eval1, rData.dResult_Eval1, sizeof(dResult_Eval1));
		memcpy(dResult_Eval2, rData.dResult_Eval2, sizeof(dResult_Eval2));
		memcpy(dResult_Eval3, rData.dResult_Eval3, sizeof(dResult_Eval3));
	}
	void Convert910(T_SECM_D_910& rData)
	{
		bUseDefaultVal = rData.bUseDefaultVal;
		nConcPreliminary = rData.nConcPreliminary;
		nConcTier1 = rData.nConcTier1;
		nConcTier2 = rData.nConcTier2;
		nRebrPreliminary = rData.nRebrPreliminary;
		nRebrTier1 = rData.nRebrTier1;
		nRebrTier2 = rData.nRebrTier2;
		nStrengthMode = rData.nStrengthMode;
		nTimeFrame = rData.nTimeFrame;
		method.Convert910(rData.method);

		if ( nConcTier1 == D_SECM_METHOD_CONS_DOC || nConcTier2 == D_SECM_METHOD_CONS_DOC )
			nStrengthMode = D_SECM_STRMODE_USER;

		memcpy(dResult_Eval1, rData.dResult_Eval1, sizeof(dResult_Eval1));
		memcpy(dResult_Eval2, rData.dResult_Eval2, sizeof(dResult_Eval2));
		memcpy(dResult_Eval3, rData.dResult_Eval3, sizeof(dResult_Eval3));
	}

	T_SECM_D& operator= (const T_SECM_D& rData)
	{
		bUseDefaultVal   = rData.bUseDefaultVal;
		nConcPreliminary = rData.nConcPreliminary;
		nConcTier1       = rData.nConcTier1;
		nConcTier2       = rData.nConcTier2;
		nRebrPreliminary = rData.nRebrPreliminary;
		nRebrTier1       = rData.nRebrTier1;
		nRebrTier2       = rData.nRebrTier2;
		nStrengthMode    = rData.nStrengthMode;
		nTimeFrame       = rData.nTimeFrame;
		method           = rData.method;

		memcpy(dResult_Eval1, rData.dResult_Eval1, sizeof(dResult_Eval1));
		memcpy(dResult_Eval2, rData.dResult_Eval2, sizeof(dResult_Eval2));
		memcpy(dResult_Eval3, rData.dResult_Eval3, sizeof(dResult_Eval3));

		return *this;
	}
};

struct T_SECM_UDRD_D_875
{
	T_SECM_K key;
	T_SECM_D_875 data;
};

struct T_SECM_UDRD_D_910
{
	T_SECM_K key;
	T_SECM_D_910 data;
};

struct T_SECM_UDRD_D_870_m
{
	T_SECM_K key;
	T_SECM_D_870_m data;
};

struct T_SECM_UDRD_D
{
	T_SECM_K key;
	T_SECM_D data;
};
#define HASHSIZESECM HASHSIZEMATL

// Performance Based Seismic Design Method - Testing
struct T_PDCM_TESTING_D
{
	BOOL   bUseDefaultVal;
	double dConcFce; // Default : Preference에 있는 Concrete의 기본 재료강도 X 1.2를 한 값이 보여짐
	double dConcLBSFactor;
	double dRebrMainFye; // Default : Preference에 있는 Rebar의 기본 재료강도 X 1.25를 한 값이 보여짐
	double dRebrSubFye;
	double dRebrLBSFactor;
	double dConcFck;
	double dRebrMainFy;
	double dRebrSubFy;
	void Initialize(int nMethod)
	{
		bUseDefaultVal = TRUE;
		dConcFce       = 0.0;
		dConcLBSFactor = 1.2;
		dRebrMainFye   = 0.0;
		dRebrSubFye    = 0.0;
		dRebrLBSFactor = 1.25;
		dConcFck       = 0.0;
		dRebrMainFy      = 0.0;
		dRebrSubFy       = 0.0;
	}
	double GetConcFck(int nMethod) { return nMethod==D_SEME_METHOD_KISTEC2013 ? (dConcLBSFactor>0 ? dConcFce     / dConcLBSFactor : 0.0) : dConcFck; }
	double GetRebrMainFy(int nMethod) { return nMethod==D_SEME_METHOD_KISTEC2013 ? (dRebrLBSFactor>0 ? dRebrMainFye / dRebrLBSFactor : 0.0) : dRebrMainFy; }
	double GetRebrSubFy(int nMethod) { return nMethod==D_SEME_METHOD_KISTEC2013 ? (dRebrLBSFactor>0 ? dRebrSubFye  / dRebrLBSFactor : 0.0) : dRebrSubFy; }
};

// Performance Based Seismic Design Method - Construction Document
enum T_PDCM_CONS_DOC_YEAR { T_PDCM_CONS_DOC_YEAR_0_9, T_PDCM_CONS_DOC_YEAR_10_19, T_PDCM_CONS_DOC_YEAR_20_29, T_PDCM_CONS_DOC_YEAR_30_ };
enum T_PDCM_CONS_DOC_MAT { T_PDCM_CONS_DOC_MAT_GOOD, T_PDCM_CONS_DOC_MAT_NORMAL, T_PDCM_CONS_DOC_MAT_POOR };
struct T_PDCM_CONS_DOC_D
{
	BOOL   bUseDefaultVal;
	double dConcFck; // Default : Preference에 있는 기본 재료강도가 보여짐
	double dConcESFactor;
	int    nConcElapsYear; // D_SEMD_YEAR
	int    nConcMatCond; // D_SEMD_MAT
	double dConcRedFactor;
	double dRebrMainFy; // Default : Preference에 있는 기본 재료강도가 보여짐
	double dRebrSubFy;
	double dRebrESFactor;
	void Initialize(int nMethod)
	{
		bUseDefaultVal = TRUE;
		dConcFck        = 0.0;
		dConcESFactor   = 1.2;
		nConcElapsYear  = T_PDCM_CONS_DOC_YEAR_0_9;
		nConcMatCond    = T_PDCM_CONS_DOC_MAT_GOOD;
		dConcRedFactor  = 1.0;
		dRebrMainFy     = 0.0;
		dRebrSubFy      = 0.0;
		dRebrESFactor   = 1.25;
	}
	double GetConcRedFactor()
	{
		const double aYearFactor[] = { 1.0, 1.0, 0.9, 0.8 };
		const double aMatlFactor[] = { 1.0, 0.9, 0.8 };
		return dConcRedFactor = aYearFactor[nConcElapsYear] * aMatlFactor[nConcMatCond];
	}
	double GetResultFck() { return dConcRedFactor     > 0 ? dConcFck       * dConcRedFactor : 0.0; }
	double GetConcFce() { return dConcESFactor      > 0 ? GetResultFck() * dConcESFactor : 0.0; }
	double GetRebrMainFye() { return dRebrESFactor      > 0 ? dRebrMainFy    * dRebrESFactor : 0.0; }
	double GetRebrSubFye() { return dRebrESFactor      > 0 ? dRebrSubFy     * dRebrESFactor : 0.0; }
};

// Performance Based Seismic Design - Concrete Material
#define T_PDCM_K UINT

struct T_PDCM_D
{
	BOOL bUseDefaultVal;

	// index = 0:Conc-fck, 1:Conc-fce, 2:Main-fy, 3:Main-fye, 4:Sub-Fy, 5:Sub-fye
	double dResult_Eval1[6]; // Strength

	void Initialize(int nMethod)
	{
		bUseDefaultVal   = TRUE;

		memset(dResult_Eval1, 0, sizeof(dResult_Eval1));
	}

	T_PDCM_D& operator= (const T_PDCM_D& rData)
	{
		bUseDefaultVal   = rData.bUseDefaultVal;

		memcpy(dResult_Eval1, rData.dResult_Eval1, sizeof(dResult_Eval1));

		return *this;
	}
};

struct T_PDCM_UDRD_D
{
	T_PDCM_K key;
	T_PDCM_D data;
};

#define HASHSIZEPDCM HASHSIZEMATL

// Seismic Evaluation - Calculation Parameters
enum D_SEPR_ZONE { D_SEPR_ZONE_1, D_SEPR_ZONE_2 };
enum D_SEPR_CLASS { D_SEPR_CLASS_Sa1, D_SEPR_CLASS_Sb2, D_SEPR_CLASS_Sc3, D_SEPR_CLASS_Sd4, D_SEPR_CLASS_Se5, D_SEPR_CLASS_S6 };
enum D_SEPR_EFFWGT { D_SEPR_EFFWGT_AUTO, D_SEPR_EFFWGT_USER };
#define D_SEPR_U_SIZE 100
struct T_SEPR_DUMMY
{
	char c[D_SEPR_U_SIZE];
};

struct T_SEPR_KISTEC_D_885
{
	int     nSeismicZone;
	double  dZoneFactor;
	int     nSiteClass;
	double  dFa;
	double  dFv;
	double  dSds;
	int     nEffWeight;
	double  dDMu;
};

struct T_SEPR_KISTEC_D
{
	int     nSeismicZone;
	double  dZoneFactor;
	int     nSiteClass;
	double  dFa;
	double  dFv;
	double  dSds;
	int     nEffWeight;
	double  dDMu;
	double  dLambdaS;
	double  dSd1;
	double  dImportanceFactor;  // 1.0, 1.2, 1.5
	void Initialize(int nMethod)
	{
		if ( nMethod==D_SEME_METHOD_KISTEC2019 )
		{
			nSeismicZone        = D_SEPR_ZONE_1;
			dZoneFactor         = 0.22;
			nSiteClass          = D_SEPR_CLASS_Sb2;
			dFa                 = 1.38;
			dFv                 = 1.38;
			dSds                = 0.506;
			nEffWeight          = D_SEPR_EFFWGT_AUTO;
			dDMu                = 2.0;
			dLambdaS            = 1.0;
			dSd1                = 0.2024;
			dImportanceFactor   = 1.2;
		}
		else
		{
			nSeismicZone        = D_SEPR_ZONE_1;
			dZoneFactor         = 0.22;
			nSiteClass          = D_SEPR_CLASS_Sa1;
			dFa                 = 0.0;
			dFv                 = 0.0;
			dSds                = 0.0;
			nEffWeight          = D_SEPR_EFFWGT_AUTO;
			dDMu                = 2.0;
			dLambdaS            = 1.0;
			dSd1                = 0.28747;
			dImportanceFactor   = 1.2;
		}
	}
};

union T_SEPR_DATA_D_885
{
	T_SEPR_KISTEC_D_885 General;
};

union T_SEPR_DATA_D
{
	T_SEPR_KISTEC_D General;
	T_SEPR_DUMMY dummy_to_fix_size;
};

#define T_SEPR_K UINT

struct T_SEPR_D_885
{
	T_SEPR_DATA_D_885 data;
};

struct T_SEPR_D
{
	T_SEPR_DATA_D data;

	void Initialize(int nMethod=D_SEME_METHOD_KISTEC2013)
	{
		switch ( nMethod )
		{
		case D_SEME_METHOD_KISTEC2013:
		case D_SEME_METHOD_MOE2018:
		case D_SEME_METHOD_MOE2019:
		case D_SEME_METHOD_KISTEC2019:
		case D_SEME_METHOD_AIK_G_001_2021:
			data.General.Initialize(nMethod);
			break;
		default:
			ASSERT(0);
		}
	}
	void Convert885(T_SEPR_D_885& rData)
	{
		Initialize(D_SEME_METHOD_KISTEC2013);
		data.General.nSeismicZone   = rData.data.General.nSeismicZone;
		data.General.dZoneFactor    = rData.data.General.dZoneFactor;
		data.General.nSiteClass     = rData.data.General.nSiteClass;
		data.General.dFa            = rData.data.General.dFa;
		data.General.dFv            = rData.data.General.dFv;
		data.General.dSds           = rData.data.General.dSds;
		data.General.nEffWeight     = rData.data.General.nEffWeight;
		data.General.dDMu           = rData.data.General.dDMu;
	}
};

struct T_SEPR_UDRD_D
{
	T_SEPR_D data;
};

#define HASHSIZESEPR 1

#define T_PSLL_K T_STLD_K
struct T_PSLL_KISTEC2013_871
{
	int    nSeismicZone;     // 0:1, 1:2
	double dZoneFactor;      // Zone Factor
	int    nSiteClass;       // 0:Sa, 1:Sb, 2:Sc, 3:Sd, 4:Se
	double dPeriodCoef;      // The coefficient for upper limit in calculated period, upper limit coefficient
	double dFa;              // Acceleration-based site coefficient
	double dFv;              // Velocity-based site coefficient
	double dSds;             // Design Spectral Response Acc. at short periods
	double dSd1;             // Design Spectral Response Acc. at 1 s periods
	double dPeriodApprX;
	double dPeriodApprY;
	double dC;
};

struct T_PSLL_KISTEC2013
{
	int    nSeismicZone;     // 0:1, 1:2
	double dZoneFactor;      // Zone Factor
	int    nSiteClass;       // 0:Sa, 1:Sb, 2:Sc, 3:Sd, 4:Se
	double dPeriodCoef;      // The coefficient for upper limit in calculated period, upper limit coefficient
	double dFa;              // Acceleration-based site coefficient
	double dFv;              // Velocity-based site coefficient
	double dSds;             // Design Spectral Response Acc. at short periods
	double dSd1;             // Design Spectral Response Acc. at 1 s periods
	double dPeriodApprX;
	double dPeriodApprY;
	double dC;
	double dJ;               // J값 추가(T_LFCA_D에 있던 값을 여기로 옮기) v875 부터
	void Initialize()
	{
		nSeismicZone   = 0;
		dZoneFactor    = 0.22;
		nSiteClass     = 3;
		dPeriodCoef    = 1.41253;
		dFa            = 1.36;
		dFv            = 1.96;
		dSds           = 0.49867;
		dSd1           = 0.28747;
		dPeriodApprX  = 0.;
		dPeriodApprY  = 0.;
		dC            = 1.0;
		dJ            = 1.0;
	}
	void Convert871(T_PSLL_KISTEC2013_871& rData)
	{
		nSeismicZone   = rData.nSeismicZone;
		dZoneFactor    = rData.dZoneFactor;
		nSiteClass     = rData.nSiteClass;
		dPeriodCoef    = rData.dPeriodCoef;
		dFa            = rData.dFa;
		dFv            = rData.dFv;
		dSds           = rData.dSds;
		dSd1           = rData.dSd1;
		dPeriodApprX   = rData.dPeriodApprX;
		dPeriodApprY   = rData.dPeriodApprY;
		dC             = rData.dC;
		dJ             = 1.0;
	}
};

struct T_PSLL_MOE2018_883
{
	int    nSeismicZone;     // 0:1, 1:2
	double dZoneFactor;      // Zone Factor
	int    nSiteClass;       // 0:Sa, 1:Sb, 2:Sc, 3:Sd, 4:Se
	double dDepth2MR;                // 보통암까지의 깊이
	double dPeriodCoef;      // The coefficient for upper limit in calculated period, upper limit coefficient
	double dFa;              // Acceleration-based site coefficient
	double dFv;              // Velocity-based site coefficient
	double dSds;             // Design Spectral Response Acc. at short periods
	double dSd1;             // Design Spectral Response Acc. at 1 s periods
	double dPeriodApprX;
	double dPeriodApprY;
	double dC;
	double dJ;               // J값 추가(T_LFCA_D에 있던 값을 여기로 옮기) v875 부터
};

struct T_PSLL_MOE2018
{
	int    nSeismicZone;    // 0:1, 1:2
	double dZoneFactor;     // Zone Factor
	int    nSiteClass;      // MOE2018 - 0:Sa, 1:Sb, 2:Sc, 3:Sd, 4:Se, MOE2019 - 0:S1, 1:S2, 2:S3, 3:S4, 4:S5, 5:S6
	double dDepth2MR;       // 보통암까지의 깊이
	double dPeriodCoef;     // The coefficient for upper limit in calculated period, upper limit coefficient
	double dFa;             // Acceleration-based site coefficient
	double dFv;             // Velocity-based site coefficient
	double dSds;            // Design Spectral Response Acc. at short periods
	double dSd1;            // Design Spectral Response Acc. at 1 s periods
	double dPeriodApprX;
	double dPeriodApprY;
	double dC;
	double dJ;              // J값 추가(T_LFCA_D에 있던 값을 여기로 옮기) v875 부터
	double bIgnoreCu;       // Not Consider Cu : MOE2019에서 사용
	void Initialize(int nMethod)
	{
		switch ( nMethod )
		{
		case D_SEME_METHOD_MOE2019:
		case D_SEME_METHOD_KISTEC2019:
			{
				nSeismicZone    = 0;
				dZoneFactor     = 0.22;
				nSiteClass      = 1;
				dDepth2MR       = 0.;
				dPeriodCoef     = 1.4976;
				dFa             = 1.38;
				dFv             = 1.38;
				dSds            = 0.506;
				dSd1            = 0.2024;
				dPeriodApprX    = 0.;
				dPeriodApprY    = 0.;
				dC              = 1.0;
				dJ              = 1.0;
				bIgnoreCu       = FALSE;
			}
			break;
		case D_SEME_METHOD_MOE2018:
		default:
			{
				nSeismicZone    = 0;
				dZoneFactor     = 0.22;
				nSiteClass      = 3;
				dDepth2MR       = 0.;
				dPeriodCoef     = 1.41253;
				dFa             = 1.46;
				dFv             = 1.58;
				dSds            = 0.53533;
				dSd1            = 0.23173;
				dPeriodApprX    = 0.;
				dPeriodApprY    = 0.;
				dC              = 1.0;
				dJ              = 1.0;
				bIgnoreCu       = FALSE;
			}
			break;
		}
	}
	void Convert880(T_PSLL_KISTEC2013& rData)
	{
		nSeismicZone   = rData.nSeismicZone;
		dZoneFactor    = rData.dZoneFactor;
		nSiteClass     = rData.nSiteClass;
		dPeriodCoef    = rData.dPeriodCoef;
		dFa            = rData.dFa;
		dFv            = rData.dFv;
		dSds           = rData.dSds;
		dSd1           = rData.dSd1;
		dPeriodApprX   = rData.dPeriodApprX;
		dPeriodApprY   = rData.dPeriodApprY;
		dC             = rData.dC;
		dJ             = rData.dJ;
		dDepth2MR      = 0.0;
	}
};

struct T_PSLL_KISTEC2019
{
	int    nSeismicZone;    // 0:1, 1:2
	double dZoneFactor;     // Zone Factor
	int    nSiteClass;      // 0:S1, 1:S2, 2:S3, 3:S4, 4:S5, 5:S6
	double dPeriodCoef;     // The coefficient for upper limit in calculated period, upper limit coefficient
	double dFa;             // Acceleration-based site coefficient
	double dFv;             // Velocity-based site coefficient
	double dSds;            // Design Spectral Response Acc. at short periods
	double dSd1;            // Design Spectral Response Acc. at 1 s periods
	double dPeriodApprX;
	double dPeriodApprY;
	double dC;
	double dJ;
	double dX;
	void Initialize()
	{
		nSeismicZone    = 0;
		dZoneFactor     = 0.22;
		nSiteClass      = 1;
		dPeriodCoef     = 1.4976;
		dFa             = 1.38;
		dFv             = 1.38;
		dSds            = 0.506;
		dSd1            = 0.2024;
		dPeriodApprX    = 0.;
		dPeriodApprY    = 0.;
		dC              = 1.0;
		dJ              = 1.0;
		dX              = 1.0;
	}
};

#define D_PSLL_U_SIZE 200
struct T_PSLL_ITEM_DUMMY
{
	// 뭐 하나 추가할 때마다 유니온 사이즈 바뀌어서 그냥 고정값 씀
	char c[D_PSLL_U_SIZE];
};

union T_PSLL_U_871
{
	T_PSLL_KISTEC2013_871 KISTEC2013;
};

union T_PSLL_U_880
{
	T_PSLL_KISTEC2013 KISTEC2013;
};

union T_PSLL_U_883
{
	T_PSLL_KISTEC2013 KISTEC2013;
	T_PSLL_MOE2018_883 Moe2018;
};

union T_PSLL_U
{
	T_PSLL_KISTEC2013 KISTEC2013;
	T_PSLL_MOE2018 Moe2018;
	T_PSLL_KISTEC2019 KISTEC2019;
	T_PSLL_ITEM_DUMMY dummy_to_fix_size;

	void Convert871(int nCodeType, T_PSLL_U_871& rData)
	{
		switch ( nCodeType )
		{
		case D_SEME_METHOD_KISTEC2013:
			KISTEC2013.Convert871(rData.KISTEC2013); break;
		default:
			break;
		}
	}
	void Convert880(int nCodeType, T_PSLL_U_880& rData)
	{
		switch ( nCodeType )
		{
		case D_SEME_METHOD_KISTEC2013:
			KISTEC2013 = rData.KISTEC2013; break;
		case D_SEME_METHOD_MOE2018:
			Moe2018.Convert880(rData.KISTEC2013);
		default:
			break;
		}
	}
};

// 내진성능평가 : 의사지진력. T_SEIS_D와 유사
struct T_PSLL_D
{
	int nCodeType;
	CString Description;
	T_PSLL_U code;

	double ScaleFactor_X;
	double ScaleFactor_Y;
	CArray<T_SEIS_ADDITION, T_SEIS_ADDITION&>arAddition;
	void Initialize()
	{
		nCodeType=D_SEME_METHOD_KISTEC2013;
		Description.Empty();
		code.KISTEC2013.Initialize();
		ScaleFactor_X=0.;
		ScaleFactor_Y=0.;
		arAddition.RemoveAll();
	}
	void Initialize_Code(int nCode)
	{
		Initialize();
		nCodeType = nCode;
		switch ( nCode )
		{
		case D_SEME_METHOD_KISTEC2013:
			code.KISTEC2013.Initialize();
			break;
		case D_SEME_METHOD_MOE2018:
		case D_SEME_METHOD_MOE2019:
			code.Moe2018.Initialize(nCode);
			break;
		case D_SEME_METHOD_KISTEC2019:
			code.KISTEC2019.Initialize();
			break;
		default:
			ASSERT(0);
			break;
		}
	}
	T_PSLL_D()
	{
		Initialize();
	}
	T_PSLL_D(T_PSLL_D& src)
	{
		*this = src;
	}
	T_PSLL_D& operator=(T_PSLL_D& src)
	{
		nCodeType=src.nCodeType;
		Description=src.Description;
		code    = src.code;
		ScaleFactor_X=src.ScaleFactor_X;
		ScaleFactor_Y=src.ScaleFactor_Y;
		arAddition.Copy(src.arAddition);
		return *this;
	}
};

struct T_PSLL_D_CH
{
	int nCodeType;        // 1=KS.Arch.1992  2=UBC.1991  3=UBC.1997  4=ATC3-06
	char Description[80];
	T_PSLL_U code;

	double ScaleFactor_X;
	double ScaleFactor_Y;
	CArray<T_SEIS_ADDITION, T_SEIS_ADDITION&>arAddition;
	void ConvertToString(T_PSLL_D& rData)
	{
		rData.nCodeType=nCodeType;
		ConvertCharStr(Description, rData.Description, sizeof(Description));
		rData.code = code;
		rData.ScaleFactor_X=ScaleFactor_X;
		rData.ScaleFactor_Y=ScaleFactor_Y;
		rData.arAddition.Copy(arAddition);
	}
	void ConvertToChar(T_PSLL_D& rData)
	{
		nCodeType=rData.nCodeType;
		ConvertStrChar(rData.Description, Description, sizeof(Description));
		code  = rData.code;
		ScaleFactor_X=rData.ScaleFactor_X;
		ScaleFactor_Y=rData.ScaleFactor_Y;
		arAddition.Copy(rData.arAddition);
	}
};

struct T_PSLL_UDRD_D
{
	T_PSLL_K key;
	T_PSLL_D data;
};

struct T_PSLL_UDRD_D_CH
{
	T_PSLL_K key;
	T_PSLL_D_CH data;
};

struct T_PSLL_UDRD_D_CH_RW_871
{
	T_PSLL_K key;
	int nCodeType;
	char Description[80];
	T_PSLL_U_871 code;

	double ScaleFactor_X;
	double ScaleFactor_Y;
	int nCountAddition;
};

struct T_PSLL_UDRD_D_CH_RW_880
{
	T_PSLL_K key;
	int nCodeType;
	char Description[80];
	T_PSLL_U_880 code;

	double ScaleFactor_X;
	double ScaleFactor_Y;
	int nCountAddition;
};

struct T_PSLL_UDRD_D_CH_RW_883
{
	T_PSLL_K key;
	int nCodeType;
	char Description[80];
	T_PSLL_U_883 code;

	double ScaleFactor_X;
	double ScaleFactor_Y;
	int nCountAddition;
};

struct T_PSLL_UDRD_D_CH_RW
{
	static_assert( sizeof(T_PSLL_U) == sizeof(T_PSLL_ITEM_DUMMY), "go enlarge size of dummy structure" );

	T_PSLL_K key;
	int nCodeType;
	char Description[80];
	T_PSLL_U code;

	double ScaleFactor_X;
	double ScaleFactor_Y;
	int nCountAddition;
	void Initialize()
	{
		key=0;
		nCodeType=D_SEME_METHOD_KISTEC2013;
		memset(Description, ' ', 80);
		code.KISTEC2013.Initialize();
		ScaleFactor_X=0.;
		ScaleFactor_Y=0.;
		nCountAddition=0;
	}
	void GetPsll(T_PSLL_K& rKey, T_PSLL_D_CH& rData)
	{
		rKey=key;
		rData.nCodeType=nCodeType;
		memcpy(rData.Description, Description, sizeof(Description));
		rData.code = code;
		rData.ScaleFactor_X=ScaleFactor_X;
		rData.ScaleFactor_Y=ScaleFactor_Y;
	}
	void SetPsll(const T_PSLL_K Key, const T_PSLL_D_CH& rData)
	{
		key=Key;
		nCodeType=rData.nCodeType;
		memcpy(Description, rData.Description, sizeof(Description));
		code = rData.code;
		ScaleFactor_X=rData.ScaleFactor_X;
		ScaleFactor_Y=rData.ScaleFactor_Y;
		nCountAddition=( int ) rData.arAddition.GetSize();
	}
	void Convert871(T_PSLL_UDRD_D_CH_RW_871& rData)
	{
		key = rData.key;
		nCodeType=rData.nCodeType;
		memcpy(Description, rData.Description, sizeof(Description));
		code.Convert871(nCodeType, rData.code);
		ScaleFactor_X=rData.ScaleFactor_X;
		ScaleFactor_Y=rData.ScaleFactor_Y;
		nCountAddition=( int ) rData.nCountAddition;
	}
	void Convert880(T_PSLL_UDRD_D_CH_RW_880& rData)
	{
		key = rData.key;
		nCodeType=rData.nCodeType;
		memcpy(Description, rData.Description, sizeof(Description));
		code.Convert880(nCodeType, rData.code);
		ScaleFactor_X=rData.ScaleFactor_X;
		ScaleFactor_Y=rData.ScaleFactor_Y;
		nCountAddition=( int ) rData.nCountAddition;
	}
	void Convert883(T_PSLL_UDRD_D_CH_RW_883& rDataOld)
	{
		key = rDataOld.key;
		nCodeType=rDataOld.nCodeType;
		memcpy(Description, rDataOld.Description, sizeof(Description));
		memcpy(&code, &rDataOld.code, sizeof(rDataOld.code));
		ScaleFactor_X=rDataOld.ScaleFactor_X;
		ScaleFactor_Y=rDataOld.ScaleFactor_Y;
		nCountAddition=( int ) rDataOld.nCountAddition;
	}
	// 기존 struct에 추가된 변수 있을 때 사용
	void InitNewVariable(int nVersion)
	{
		//     switch(nCodeType)
		//     {
		//       // 예문
		//     case D_SEME_METHOD_MOE2019:
		//       {
		//         if(nVersion<=8850)
		//         {
		//           code.Moe2018.ABC = 1.0;
		//         }
		//         if()
		//         {
		//           code.Moe2018.CDF = 1.0;
		//         }
		//       }
		//     }
	}
};

struct T_PSLL_UNIT
{
	T_PSLL_UNIT()
	{
		nCodeType=D_UNITSYS_NONE;
		Description=D_UNITSYS_NONE;
		ScaleFactor_X=D_UNITSYS_NONE;
		ScaleFactor_Y=D_UNITSYS_NONE;
		KeyStor=D_UNITSYS_NONE;
		AddX=D_UNITSYS_BASE_FORCE;
		AddY=D_UNITSYS_BASE_FORCE;
		Depth2MR = D_UNITSYS_BASE_LENGTH;
	}
	int nCodeType;
	int Description;
	int ScaleFactor_X;
	int ScaleFactor_Y;
	int KeyStor;
	int AddX;
	int AddY;
	int Depth2MR;
};

#define HASHSIZEPSLL HASHSIZESEIS

// Seismic Evaluation - Effective Weight
#define T_SEEW_K T_STOR_K
struct T_SEEW_D
{
	double dWeight;
	double dFloorArea;
	void Initialize()
	{
		dWeight    = 0.0;
		dFloorArea = 0.0;
	}
};

struct T_SEEW_UDRD_D
{
	T_SEEW_K key;
	T_SEEW_D data;
};

#define HASHSIZESEEW HASHSIZESTOR

// Seismic Evaluation - Story CDR Method
#define T_SCDR_K UINT
enum D_SCDR_METHOD
{
	D_SCDR_METHOD_LATERAL_STIFF, // Consider lateral stiffness ratio
	D_SCDR_METHOD_MINIMUM,
};

struct T_SCDR_D
{
	unsigned int AnalType;    //D_LOADCASE_??? (Dialog에서 D_LOADCASE_???로 저장해서 변경함:04.06.25)
	unsigned int LoadCaseKey;
	T_STOR_K StorK;
	int nMethod; // D_SCDR_METHOD
	void Initialize()
	{
		AnalType = 0;
		LoadCaseKey = 0;
		StorK = 0;
		nMethod = D_SCDR_METHOD_LATERAL_STIFF;
	}
};

struct T_SCDR_UDRD_D
{
	T_SCDR_K key;
	T_SCDR_D data;
};
#define HASHSIZESCDR HASHSIZESTOR

#define T_LESF_K T_STLD_K
#define T_LESF_K_LIST CArray<T_LESF_K, T_LESF_K>

// LESF(Loadcase for Section Effective Stiffness - Scale Factor)
// 내진성능평가 : 기둥유효강성 계수
struct T_LESF_D
{
	double dScaleFactor;
	void Initialize()
	{
		dScaleFactor = 1.0;
	}
	T_LESF_D() : dScaleFactor(1.0) { }
	T_LESF_D(T_LESF_D& src)
	{
		*this = src;
	}
	T_LESF_D& operator=(T_LESF_D& src)
	{
		dScaleFactor = src.dScaleFactor;
		return *this;
	}
};

struct T_LESF_UDRD_D
{
	T_LESF_K key;
	T_LESF_D data;
};
#define HASHSIZELESF HASHSIZESTLD

// Seismic Evaluation - Automatic Generation of Load Combination
enum D_SELC_OPT { D_SELC_OPT_ADD, D_SELC_OPT_REPLACE };
enum D_SELC_ORTHO { D_SELC_ORTHO_100_30, D_SELC_ORTHO_SRSS };
#define T_SELC_K UINT
struct T_SELC_D
{
	int       nOption; // D_SELC_OPT
	double    dVal_J;
	T_STLD_K  StldK_J;
	BOOL      bConsiderOrthoEff;
	int       nOrthoOpt; // D_SELC_ORTHO
	BOOL      bVerticalForce;

	T_SELC_D() { Initialize(); }
	T_SELC_D(T_SELC_D& src)
	{
		*this = src;
	}
	void Initialize()
	{
		nOption           = D_SELC_OPT_ADD;
		dVal_J            = 1.0;
		StldK_J           = 0;
		bConsiderOrthoEff = FALSE;
		nOrthoOpt         = D_SELC_ORTHO_100_30;
		bVerticalForce    = FALSE;
	}
};

struct T_SELC_UDRD_D
{
	T_SELC_D data;
};
#define HASHSIZESELC 1

#define T_SEMT_K UINT
struct T_SEMT_D
{
	BOOL bUseSeisHook;

	T_SEMT_D() { Initialize(); }
	T_SEMT_D(T_SEMT_D& src)
	{
		*this = src;
	}
	void Initialize()
	{
		bUseSeisHook = FALSE;
	}
	T_SEMT_D& operator=(T_SEMT_D& src)
	{
		bUseSeisHook = src.bUseSeisHook;
		return *this;
	}
	BOOL operator==(const T_SEMT_D& src)
	{
		if ( bUseSeisHook != src.bUseSeisHook ) return FALSE;
		return TRUE;
	}
};

struct T_SEMT_UDRD_D
{
	T_SEMT_D data;
};
#define HASHSIZESEMT 1

// Seismic Evaluation - Clear Height of Column
#define T_SECH_K T_ELEM_K
struct T_SECH_D
{
	double dh0y;
	double dh0z;
	void Initialize()
	{
		dh0y = 0.0;
		dh0y = 0.0;
	}
};

struct T_SECH_UDRD_D
{
	T_SECH_K key;
	T_SECH_D data;
};
#define HASHSIZESECH HASHSIZEELEM

// Seismic Evaluation - Use Seismic Hook
#define T_SEHK_K T_ELEM_K
struct T_SEHK_D
{
	BOOL bUseHook;
	void Initialize()
	{
		bUseHook = FALSE;
	}
};

struct T_SEHK_UDRD_D
{
	T_SEHK_K key;
	T_SEHK_D data;
};
#define HASHSIZESEHK HASHSIZEELEM

//Seismic Component Type
#define T_SCMT_K T_ELEM_K
enum D_SCMT_TYPE { D_SCMT_TYPE_PRIMARY=0, D_SCMT_TYPE_SECONDARY, D_SCMT_TYPE_NONE };
struct T_SCMT_D
{
	int nType;        //0 = Primary Component, 1 = Secondary Component

	T_SCMT_D() { Initialize(); }
	T_SCMT_D(T_SCMT_D& src)
	{
		*this = src;
	}
	void Initialize()
	{
		nType = 0;
	}
	T_SCMT_D& operator=(T_SCMT_D& src)
	{
		nType = src.nType;
		return *this;
	}
	BOOL operator==(const T_SCMT_D& src)
	{
		if ( nType != src.nType ) return FALSE;
		return TRUE;
	}
};

struct T_SCMT_UDRD_D
{
	T_SCMT_K key;
	T_SCMT_D data;
};
#define HASHSIZESCMT HASHSIZEELEM

// Seismic Evaluation Method - Testing (Steel)
struct T_SESM_TESTING_KISTEC2013_D
{
	BOOL   bUseDefaultVal;
	double dSteelFye;         //Default : Material Propery의 항복강도 * 1.25
	double dLBSFactor;
	void Initialize()
	{
		bUseDefaultVal = TRUE;
		dSteelFye = 0.0;
		dLBSFactor = 1.25;
	}
	double GetSteelFy() { return dLBSFactor>0 ? dSteelFye / dLBSFactor : 0.0; }
};

// Seismic Evaluation Method - Construction Document (Steel)
struct T_SESM_DOC_KISTEC2013_D
{
	BOOL   bUseDefaultVal;
	double dSteelFy;
	double dESFactor;
	void Initialize()
	{
		bUseDefaultVal = TRUE;
		dSteelFy = 0.0;
		dESFactor = 1.25;
	}
	double GetSteelFye() { return dSteelFy * dESFactor; }
};

// Seismic Evaluation Method - Default String (Steel)
enum T_SESM_KISTEC2013_DEF_STR_YEAR { T_SESM_KISTEC2013_DEF_STR_YEAR_0_1970, T_SESM_KISTEC2013_DEF_STR_YEAR_1971_1987, T_SESM_KISTEC2013_DEF_STR_YEAR_1988_2000, T_SESM_KISTEC2013_DEF_STR_YEAR_2001_ };
enum T_SESM_MOE2018_DEF_STR_YEAR { T_SESM_MOE2018_DEF_STR_YEAR_0_1969, T_SESM_MOE2018_DEF_STR_YEAR_1970_1988, T_SESM_MOE2018_DEF_STR_YEAR_1989_2000, T_SESM_MOE2018_DEF_STR_YEAR_2001_ };
struct T_SESM_DEF_STR_KISTEC2013_D
{
	int    nTimeFrame; // T_SESM_KISTEC2013_DEF_STR_YEAR
	void Initialize()
	{
		nTimeFrame = T_SESM_KISTEC2013_DEF_STR_YEAR_0_1970;
	}
};

struct T_SESM_KISTEC2013_D
{
	T_SESM_TESTING_KISTEC2013_D     testing;
	T_SESM_DOC_KISTEC2013_D             document;
	T_SESM_DEF_STR_KISTEC2013_D   defStr;
};

// Seismic Evaluation Method - Testing (Steel) MOE2018
struct T_SESM_TESTING_MOE2018_D
{
	BOOL   bUseDefaultVal;
	double dFu;
	double dFue;
	double dFy;
	double dFye;

	void Initialize()
	{
		bUseDefaultVal = TRUE;
		dFu = 0.0;
		dFue = 0.0;
		dFy = 0.0;
		dFye = 0.0;
	}
};

// Seismic Evaluation Method - Document (Steel) MOE2018
struct T_SESM_DOC_MOE2018_D
{
	BOOL   bUseDefaultVal;
	double dFu;
	double dFy1;
	double dFy2;
	double dFy3;
	double dFy4;
	double dFy5;
	double dRt;
	double dRy;

	void Initialize()
	{
		bUseDefaultVal = TRUE;
		dFu = 0.0;
		dFy1 = 0.0;
		dFy2 = 0.0;
		dFy3 = 0.0;
		dFy4 = 0.0;
		dFy5 = 0.0;
		dRt = 1.2;
		dRy = 1.3;
	}
};

struct T_SESM_MOE2018_D
{
	T_SESM_TESTING_MOE2018_D testing;
	T_SESM_DOC_MOE2018_D     document;
};

union T_SESM_METHOD_D_880
{
	T_SESM_KISTEC2013_D KISTEC2013;
};

union T_SESM_METHOD_D
{
	T_SESM_KISTEC2013_D KISTEC2013;
	T_SESM_MOE2018_D MOE2018;

	void Initialize(int nMethod)
	{
		switch ( nMethod )
		{
		case D_SEME_METHOD_KISTEC2013:
			KISTEC2013.testing.Initialize();
			KISTEC2013.document.Initialize();
			KISTEC2013.defStr.Initialize();
			break;
		case D_SEME_METHOD_MOE2018:
		case D_SEME_METHOD_MOE2019:
		case D_SEME_METHOD_KISTEC2019:
			MOE2018.testing.Initialize();
			MOE2018.document.Initialize();
			break;
		default:
			ASSERT(0);
			break;
		}
	}

	void Convert880(T_SESM_METHOD_D_880& rData)
	{
		Initialize(D_SEME_METHOD_MOE2018);
		KISTEC2013 = rData.KISTEC2013;
	}
};

enum D_SESM_METHOD { D_SESM_METHOD_TESTING, D_SESM_METHOD_CONS_DOC, D_SESM_METHOD_DEF_STRENGTH };
enum D_SESM_STRMODE { D_SESM_STRMODE_USER, D_SESM_STRMODE_AUTO };
// Seismic Evaluation - Steel Material
#define T_SESM_K UINT

struct T_SESM_D_880
{
	BOOL bUseDefaultVal;
	int  nSteelEval1; // D_SESM_METHOD
	int  nSteelEval2;
	int  nSteelEval3;
	int  nStrengthMode; // D_SESM_STRMODE
	T_SESM_METHOD_D_880 method;

	// index = 0:Steel-fck, 1:Steel-fce
	double dResult_Eval1[2];
	double dResult_Eval2[2];
	double dResult_Eval3[2];
};

struct T_SESM_D
{
	BOOL bUseDefaultVal;
	int  nSteelEval1; // KISTEC2013 : pre, MOE2018 : Linear Eval.
	int  nSteelEval2; // KISTEC2013 : 1st, MOE2018 : m-Factor
	int  nSteelEval3; // KISTEC2013 : 2nd, MOE2018 : Pushover
	int  nStrengthMode; // D_SESM_STRMODE
	T_SESM_METHOD_D method;

	// KISTEC2013   > 0:Steel-fck, 1:Steel-fce
	// MOE2018      > 0:Fu, 1:Fue, 2:Fy1, 3:Fye1, 4:Fy2, 5:Fye2 ... 10:Fy5, 11:Fye5
	double dResult_Eval1[12];
	double dResult_Eval2[12];
	double dResult_Eval3[12];

	void Initialize(int nMethod)
	{
		bUseDefaultVal= TRUE;
		nSteelEval1     = D_SESM_METHOD_TESTING;
		nSteelEval2     = D_SESM_METHOD_TESTING;
		nSteelEval3     = D_SESM_METHOD_TESTING;
		nStrengthMode   = D_SESM_STRMODE_USER;

		method.Initialize(nMethod);

		memset(dResult_Eval1, 0, sizeof(dResult_Eval1));
		memset(dResult_Eval2, 0, sizeof(dResult_Eval2));
		memset(dResult_Eval3, 0, sizeof(dResult_Eval3));
	}

	T_SESM_D& operator= (const T_SESM_D& rData)
	{
		bUseDefaultVal= rData.bUseDefaultVal;
		nSteelEval1     = rData.nSteelEval1;
		nSteelEval2     = rData.nSteelEval2;
		nSteelEval3     = rData.nSteelEval3;
		nStrengthMode = rData.nStrengthMode;
		method        = rData.method;

		memcpy(dResult_Eval1, rData.dResult_Eval1, sizeof(dResult_Eval1));
		memcpy(dResult_Eval2, rData.dResult_Eval2, sizeof(dResult_Eval2));
		memcpy(dResult_Eval3, rData.dResult_Eval3, sizeof(dResult_Eval3));

		return *this;
	}

	void Convert880(T_SESM_D_880& rData)
	{
		bUseDefaultVal= rData.bUseDefaultVal;
		nSteelEval1     = rData.nSteelEval1;
		nSteelEval2     = rData.nSteelEval2;
		nSteelEval3     = rData.nSteelEval3;
		nStrengthMode = rData.nStrengthMode;
		method.Convert880(rData.method);

		memset(dResult_Eval1, 0, sizeof(dResult_Eval1));
		memset(dResult_Eval2, 0, sizeof(dResult_Eval2));
		memset(dResult_Eval3, 0, sizeof(dResult_Eval3));

		memcpy(dResult_Eval1, rData.dResult_Eval1, sizeof(double)*2);
		memcpy(dResult_Eval2, rData.dResult_Eval2, sizeof(double)*2);
		memcpy(dResult_Eval3, rData.dResult_Eval3, sizeof(double)*2);
	}
};

struct T_SESM_UDRD_D_880
{
	T_SESM_K key;
	T_SESM_D_880 data;
};

struct T_SESM_UDRD_D
{
	T_SESM_K key;
	T_SESM_D data;
};
#define HASHSIZESESM HASHSIZEMATL

struct T_SESM_GOOD_D
{
	BOOL   bUseDefaultVal;
	double Strength;

	void Initialize(int nMethod)
	{
		bUseDefaultVal = TRUE;
		Strength = 0.0;
	}
};

struct T_SESM_NORM_D
{
	BOOL   bUseDefaultVal;
	double Strength;

	void Initialize(int nMethod)
	{
		bUseDefaultVal = TRUE;
		Strength = 0.0;
	}
};

struct T_SESM_POOR_D
{
	BOOL   bUseDefaultVal;
	double Strength;

	void Initialize(int nMethod)
	{
		bUseDefaultVal = TRUE;
		Strength = 0.0;
	}
};

enum D_SEMM_CONDITION { D_SEMM_CONDITION_GOOD, D_SEMM_CONDITION_NORMAL, D_SEMM_CONDITION_POOR };
enum D_SEMM_JOINT { D_SEMM_JOINT_GOOD, D_SEMM_JOINT_POOR };
enum D_SEMM_STRMODE { D_SEMM_STRMODE_USER, D_SEMM_STRMODE_AUTO };
// Seismic Evaluation - Masonry Material
#define T_SEMM_K UINT
struct T_SEMM_D
{
	BOOL bUseDefaultVal;
	int  nMasonryCondition; // D_SEMM_METHOD
	int  nMasonryJoint;
	int  nStrengthMode;         // D_SEMM_STRMODE

	// index = 0:Comp-fm, 1:Comp-fme, 2:Tens-fm, 3:Tens-fme, 4:Shear-fm, 5:Shear-fme
	double dResult_LNEV[6];
	double dResult_MFAC[6];
	double dResult_PSOV[6];

	void Initialize(int nMethod)
	{
		bUseDefaultVal    = TRUE;
		nMasonryCondition = D_SEMM_CONDITION_GOOD;
		nMasonryJoint     = D_SEMM_JOINT_GOOD;
		nStrengthMode     = D_SEMM_STRMODE_USER;

		memset(dResult_LNEV, 0, sizeof(dResult_LNEV));
		memset(dResult_MFAC, 0, sizeof(dResult_MFAC));
		memset(dResult_PSOV, 0, sizeof(dResult_PSOV));
	}

	T_SEMM_D& operator= (const T_SEMM_D& rData)
	{
		bUseDefaultVal    = rData.bUseDefaultVal;
		nMasonryCondition = rData.nMasonryCondition;
		nMasonryJoint     = rData.nMasonryJoint;
		nStrengthMode     = rData.nStrengthMode;

		memcpy(dResult_LNEV, rData.dResult_LNEV, sizeof(dResult_LNEV));
		memcpy(dResult_MFAC, rData.dResult_MFAC, sizeof(dResult_MFAC));
		memcpy(dResult_PSOV, rData.dResult_PSOV, sizeof(dResult_PSOV));

		return *this;
	}
};

struct T_SEMM_UDRD_D
{
	T_SEMM_K key;
	T_SEMM_D data;
};
#define HASHSIZESEMM HASHSIZEMATL

#define T_MFAC_K T_ELEM_K

struct T_MFAC_D_885
{
	double dMFactor1[3];
	double dMFactor2[3];
	double dMFactor3[3];
};

struct T_MFAC_D
{
	// 0:IO, 1:LS, 2:CP
	double dMFactor1[3]; // RcColmM(Tens.), StlColm(Tens.), StlBraceBeam(Tens.), StlBraceBeam(Tens.), RcColm(Flx.y), RcBeam(Flexure), StlBeam(Flexure), RcWall(Flexure/Shear), Masonry(Comp.)
	double dMFactor2[3]; // RcColmM(Comp.), StlColm(Flx.y), StlBraceBeam(Flx.y), StlBraceBeam(Comp.), RcColm(Flx.z)
	double dMFactor3[3]; // RcColmM(Flx.y), StlColm(Flx.z), StlBraceBeam(Flx.z),
	double dMFactor4[3]; // RcColmM(Flx.z)
	void Initialize()
	{
		for ( int i=0; i<3; i++ )
		{
			dMFactor1[i]=1.;
			dMFactor2[i]=1.;
			dMFactor3[i]=1.;
			dMFactor4[i]=1.;
		}
	}
	BOOL operator==(const T_MFAC_D& src) const
	{
		for ( int i=0; i<3; i++ )
		{
			if ( dMFactor1[i] != src.dMFactor1[i] ) return FALSE;
			if ( dMFactor2[i] != src.dMFactor2[i] ) return FALSE;
			if ( dMFactor3[i] != src.dMFactor3[i] ) return FALSE;
			if ( dMFactor4[i] != src.dMFactor4[i] ) return FALSE;
		}
		return TRUE;
	}
	BOOL operator!=(const T_MFAC_D& src) const
	{
		return !( *this == src );
	}
	void Convert885(T_MFAC_D_885& rData)
	{
		Initialize();
		for ( int i=0; i<3; i++ )
		{
			dMFactor1[i] = rData.dMFactor1[i];
			dMFactor2[i] = rData.dMFactor2[i];
			dMFactor3[i] = rData.dMFactor3[i];
		}
	}
};

struct T_MFAC_UDRD_D_885
{
	T_MFAC_K key;
	T_MFAC_D_885 data;
};

struct T_MFAC_UDRD_D
{
	T_MFAC_K key;
	T_MFAC_D data;
};

#define HASHSIZEMFAC HASHSIZEELEM

#define T_XFAC_K T_ELEM_K

struct T_XFAC_D
{
	double dChi;
	double dGamma;
	BOOL bDoNotExeed;
	void Initialize()
	{
		dChi = 1.3;
		dGamma = 1.0;
		bDoNotExeed = FALSE;
	}
	BOOL operator==(const T_XFAC_D& src) const
	{
		if ( dChi != src.dChi ) return FALSE;
		if ( dGamma != src.dGamma ) return FALSE;
		if ( bDoNotExeed != src.bDoNotExeed ) return FALSE;

		return TRUE;
	}
	BOOL operator!=(const T_XFAC_D& src) const
	{
		return !( *this == src );
	}
};

struct T_XFAC_UDRD_D
{
	T_XFAC_K key;
	T_XFAC_D data;
};

#define HASHSIZEXFAC HASHSIZEELEM

// Reduce Infill Strut Stiffness
#define T_RISS_K T_ELEM_K
struct T_RISS_D
{
	BOOL bReduceStiff;

	T_RISS_D() { Initialize(); }
	T_RISS_D(T_RISS_D& src)
	{
		*this = src;
	}
	void Initialize()
	{
		bReduceStiff = FALSE;
	}
	T_RISS_D& operator=(T_RISS_D& src)
	{
		bReduceStiff = src.bReduceStiff;
		return *this;
	}
	BOOL operator==(const T_RISS_D& src)
	{
		if ( bReduceStiff != src.bReduceStiff ) return FALSE;
		return TRUE;
	}
};
struct T_RISS_UDRD_D
{
	T_RISS_K key;
	T_RISS_D data;
};
#define HASHSIZERISS HASHSIZEELEM

// Masonry Infill Strut // JWKWON-20180810 : 조적 채움벽
#define T_MSIS_K unsigned int
typedef CArray<T_MSIS_K, T_MSIS_K> T_MSIS_K_LIST;
enum EN_MSIS_TYPE
{
	EN_MSIS_PARTIAL_HEIGHT=0, EN_MSIS_FULL_HEIGHT
};

struct T_MSIS_D
{
	CString strName;
	CString strDecs;
	double dLm;
	double dHm;
	double dTm;
	double dOpeningArea;
	int nStrutType;   // 0:Window Sill, 1:Full
	double dBending;  // Effective Stiffness Scale Factor
	void Initialize()
	{
		strName.Empty();
		strDecs.Empty();
		dLm = dHm = dTm = 0.;
		dOpeningArea = 0.;
		nStrutType = 0;
		dBending = 1.0;
	}

	T_MSIS_D() {}
	T_MSIS_D& operator=(T_MSIS_D& src)
	{
		strName   = src.strName;
		strDecs   = src.strDecs;
		dLm       = src.dLm;
		dHm       = src.dHm;
		dTm       = src.dTm;
		dOpeningArea    = src.dOpeningArea;
		nStrutType = src.nStrutType;
		dBending  = src.dBending;
		return *this;
	}
};

struct T_MSIS_D_CH_881m //  v881 개발 중간 버전
{
	char   strName[20];
	char   strDecs[20];

	double dLm;
	double dHm;
	double dTm;
	double dOpeningArea;
	int nStrutType; // 0:Window Sill, 1:Full
};

struct T_MSIS_D_CH
{
	char   strName[40];
	char   strDecs[40];

	double dLm;
	double dHm;
	double dTm;
	double dOpeningArea;
	int nStrutType; // 0:Window Sill, 1:Full
	double dBending;

	void ConvertToString(T_MSIS_D& rData)
	{
		ConvertCharStr(strName, rData.strName, sizeof(strName));
		ConvertCharStr(strDecs, rData.strDecs, sizeof(strDecs));

		rData.dLm = dLm;
		rData.dHm = dHm;
		rData.dTm = dTm;
		rData.dOpeningArea = dOpeningArea;
		rData.nStrutType = nStrutType;
		rData.dBending = dBending;
	}
	void ConvertToChar(T_MSIS_D& rData)
	{
		ConvertStrChar(rData.strName, strName, sizeof(strName));
		ConvertStrChar(rData.strDecs, strDecs, sizeof(strDecs));
		dLm = rData.dLm;
		dHm = rData.dHm;
		dTm = rData.dTm;
		dOpeningArea = rData.dOpeningArea;
		nStrutType = rData.nStrutType;
		dBending = rData.dBending;
	}
};

struct T_MSIS_UDRD_D
{
	T_MSIS_K key;
	T_MSIS_D data;
};

struct T_MSIS_UDRD_D_CH
{
	T_MSIS_K key;
	T_MSIS_D_CH data;
};

struct T_MSIS_UDRD_D_CH_RW_881m
{
	T_MSIS_K key;
	char   strName[20];
	char   strDecs[20];

	double dLm;
	double dHm;
	double dTm;
	double dOpeningArea;
	int    nStrutType;
};

struct T_MSIS_UDRD_D_CH_RW_950
{
	T_MSIS_K key;
	char   strName[20];
	char   strDecs[20];

	double dLm;
	double dHm;
	double dTm;
	double dOpeningArea;
	int    nStrutType;
	double dBending;
};

struct T_MSIS_UDRD_D_CH_RW
{
	T_MSIS_K key;
	char   strName[40];
	char   strDecs[40];

	double dLm;
	double dHm;
	double dTm;
	double dOpeningArea;
	int    nStrutType;
	double dBending;

	void Convert950(T_MSIS_UDRD_D_CH_RW_950& rData)
	{
		key = rData.key;
		memcpy(strName, rData.strName, sizeof(strName));
		memcpy(strDecs, rData.strDecs, sizeof(strDecs));
		dLm = rData.dLm;
		dHm = rData.dHm;
		dTm = rData.dTm;
		dOpeningArea = rData.dOpeningArea;
		nStrutType = rData.nStrutType;
		dBending = rData.dBending;
	}
	void Convert881m(T_MSIS_UDRD_D_CH_RW_881m& rData)
	{
		key       = rData.key;
		memcpy(strName, rData.strName, sizeof(strName));
		memcpy(strDecs, rData.strDecs, sizeof(strDecs));
		dLm       = rData.dLm;
		dHm       = rData.dHm;
		dTm       = rData.dTm;
		dOpeningArea = rData.dOpeningArea;
		nStrutType = rData.nStrutType;
		dBending   = 0.35;
	}
	void GetMsis(T_MSIS_K& rKey, T_MSIS_D_CH& rData)
	{
		rKey=key;
		memcpy(rData.strName, strName, sizeof(strName));
		memcpy(rData.strDecs, strDecs, sizeof(strDecs));

		rData.dLm = dLm;
		rData.dHm = dHm;
		rData.dTm = dTm;
		rData.dOpeningArea = dOpeningArea;
		rData.nStrutType = nStrutType;
		rData.dBending = dBending;
	}
	void SetMsis(const T_MSIS_K Key, const T_MSIS_D_CH& rData)
	{
		key=Key;
		memcpy(strName, rData.strName, sizeof(strName));
		memcpy(strDecs, rData.strDecs, sizeof(strDecs));

		dLm = rData.dLm;
		dHm = rData.dHm;
		dTm = rData.dTm;
		dOpeningArea = rData.dOpeningArea;
		nStrutType = rData.nStrutType;
		dBending = rData.dBending;
	}
};

struct T_MSIS_UNIT
{
	T_MSIS_UNIT()
	{
		dLm = D_UNITSYS_BASE_LENGTH;
		dHm = D_UNITSYS_BASE_LENGTH;
		dTm = D_UNITSYS_BASE_LENGTH;
		dOpeningArea = D_UNITSYS_BASE_AREA;
	}
	int dLm;
	int dHm;
	int dTm;
	int dOpeningArea;
};
#define HASHSIZEMSIS 101

// 조적 채움벽 property 계산용
struct T_MSIS_INPUT
{
	int nType;
	double dLm;
	double dHm;   // 조적 채움벽의 순높이
	double dTm;
	double dOa;   // Opening Area
	double dEm;   // Material Data의 Modulus of Elaticity 값
	double dGm;   // Material Data의 Modulus of Elaticity 값과 Poisson's Ratio로 계산 => G=E/{2*(1+v)}
	double dEc;   // 양쪽 기둥의 탄성계수
	double dIc;   // 양쪽 기둥의 단면이차 모멘트의 합
	double dBending;
	void Initialize()
	{
		nType=EN_MSIS_PARTIAL_HEIGHT;
		dLm = 0.;
		dHm = 0.;
		dTm = 0.;
		dOa = 0.;
		dEm = 0.;
		dGm = 0.;
		dEc = 0.;
		dIc = 0.;
		dBending = 0.;
	}
	T_MSIS_INPUT() { Initialize(); }
};

struct T_MSIS_PROP
{
	double dAw;     // 채움벽의 면적
	double dIm;     // 조적채움벽의 단면이차모멘트
	double dDm;     // 조적채움벽의 대각선 길이
	double dThetaM; // 등가 스트럿의 경사각
	double dRop;    // 감소율
	double dLamdaM;
	double dH;      // 모델링상 층 높이
	double dKfm;    // 등가 캔틸레버 기둥의 휨강성
	double dKvm;    // 등가 켄틸레버 기둥의 전단강성
	double dKmi;    // 채움벽 탄성 휨강성
	double dkm;     // 등가 스트럿의 축강성
	double dKms;

	double dBm;     // 등가 스트럿의 폭 Bm  => 전체 채움벽일 때 등가 스트럿 폭
	double dBm2;    // 등가 스트럿의 폭 B'm => 허리벽일 때 사용하는 스트럿 폭

	void Initialize()
	{
		dAw = 0.;
		dIm = 0.;
		dDm = 0.;
		dThetaM = 0.;
		dRop = 0.;
		dKfm = 0.;
		dKvm = 0.;
		dKmi = 0.;
		dkm = 0.;
		dBm = 0.;
		dLamdaM = 0.;
		dH = 0.;
		dKms = 0.;
		dBm2 = 0.;
	}
	T_MSIS_PROP() { Initialize(); }
};

// Masonry Infill Strut Assign // JWKWON-20180810 : 조적 채움벽
#define T_MISA_K unsigned int // Elem key
typedef CArray<T_MISA_K, T_MISA_K> T_MISA_K_LIST;
enum D_MISA_BRACING_TYPE { EN_DIAGONAL=0, EN_X };

struct T_MISA_D_881
{
	T_MSIS_K MsisK;
	void Initialize() { MsisK = 0; }
	T_MISA_D_881() { Initialize(); }
};

struct T_MISA_D
{
	T_MSIS_K MsisK;
	int nBracingType;
	void Initialize() { MsisK = 0;  nBracingType = EN_DIAGONAL; }
	T_MISA_D() { Initialize(); }
	void Convert881(T_MISA_D_881& rData)
	{
		Initialize();
		MsisK       = rData.MsisK;
	}
};

struct T_MISA_UDRD_D_881
{
	T_MISA_K key;
	T_MISA_D_881 data;
};
struct T_MISA_UDRD_D
{
	T_MISA_K key;
	T_MISA_D data;
};

#define HASHSIZEMISA HASHSIZEELEM

// Seismic Evaluation - Transverse Reinforcement Details
#define T_SERD_K T_ELEM_K
struct T_SERD_D
{
	int nReinforcement;
	void Initialize()
	{
		nReinforcement = D_SEME_REINF_135;
	}
};
struct T_SERD_UDRD_D
{
	T_SERD_K key;
	T_SERD_D data;
};
#define HASHSIZESERD HASHSIZEELEM

#define T_EGLD_K UINT
enum D_EGLD_GRUP_TYPE { EN_RC_MOM_FRAME=0, EN_STL_MOM_FRAME, EN_RC_WALL, EN_MASONRY_INFILL, EN_USER_GRUP };

struct T_EGLD_D
{
	UINT    nLoadCaseType;
	UINT    StldKey;
	double  dAngle;
	int     nSystemGrupType;
	CString strUserGroup;
	CArray<T_ELEM_K, T_ELEM_K> arKeyElem;

	void Initialize()
	{
		nLoadCaseType = 0;
		StldKey = 0;
		dAngle  = 0.;
		strUserGroup.Empty();
		nSystemGrupType = EN_RC_MOM_FRAME;
		arKeyElem.RemoveAll();
	}
	T_EGLD_D() { }
	T_EGLD_D(T_EGLD_D& src)
	{
		*this = src;
	}
	T_EGLD_D& operator=(T_EGLD_D& src)
	{
		nLoadCaseType   = src.nLoadCaseType;
		StldKey         = src.StldKey;
		dAngle          = src.dAngle;
		strUserGroup    = src.strUserGroup;
		nSystemGrupType = src.nSystemGrupType;
		arKeyElem.Copy(src.arKeyElem);

		return *this;
	}
};

struct T_EGLD_D_CH
{
	UINT    nLoadCaseType;
	UINT    StldKey;
	double  dAngle;
	char    strUserGroup[80];
	int     nSystemGrupType;
	CArray<T_ELEM_K, T_ELEM_K> arKeyElem;

	void ConvertToString(T_EGLD_D& rData)
	{
		rData.nLoadCaseType   = nLoadCaseType;
		rData.StldKey         = StldKey;
		rData.dAngle          = dAngle;
		rData.nSystemGrupType = nSystemGrupType;
		ConvertCharStr(strUserGroup, rData.strUserGroup, sizeof(strUserGroup));
		rData.arKeyElem.Copy(arKeyElem);
	}
	void ConvertToChar(T_EGLD_D& rData)
	{
		nLoadCaseType   = rData.nLoadCaseType;
		StldKey         = rData.StldKey;
		dAngle          = rData.dAngle;
		nSystemGrupType = rData.nSystemGrupType;
		ConvertStrChar(rData.strUserGroup, strUserGroup, sizeof(strUserGroup));
		arKeyElem.Copy(rData.arKeyElem);
	}
};
struct T_EGLD_UDRD_D
{
	T_EGLD_K key;
	T_EGLD_D data;
};
struct T_EGLD_UDRD_D_CH
{
	T_EGLD_K key;
	T_EGLD_D_CH data;
};
struct T_EGLD_UDRD_D_CH_RW
{
	T_EGLD_K key;
	UINT    nLoadCaseType;
	UINT    StldKey;
	double  dAngle;
	int     nSystemGrupType;
	char    strUserGroup[80];
	int     nCountKeyElem;

	void GetEgld(T_EGLD_K& rKey, T_EGLD_D_CH& rData)
	{
		rKey = key;
		rData.nLoadCaseType   = nLoadCaseType;
		rData.StldKey         = StldKey;
		rData.dAngle          = dAngle;
		rData.nSystemGrupType = nSystemGrupType;

		memcpy(rData.strUserGroup, strUserGroup, sizeof(strUserGroup));
	}
	void SetEgld(const T_EGLD_K Key, const T_EGLD_D_CH& rData)
	{
		key = Key;
		nLoadCaseType   = rData.nLoadCaseType;
		StldKey         = rData.StldKey;
		dAngle          = rData.dAngle;
		nSystemGrupType = rData.nSystemGrupType;
		nCountKeyElem = ( int ) rData.arKeyElem.GetSize();
		memcpy(strUserGroup, rData.strUserGroup, sizeof(strUserGroup));
	}
};

#define HASHSIZEEGLD 1001

// Seismic Evaluation - RC Column with Masonry Infill Type
enum D_RCCM_TYPE
{
	EN_RC_COLUMN_M,
	EN_RC_COLUMN
};

#define T_RCCM_K T_ELEM_K
struct T_RCCM_D
{
	int nLocalYType;
	int nLocalZType;
	void Initialize()
	{
		nLocalYType = EN_RC_COLUMN_M;
		nLocalZType = EN_RC_COLUMN_M;
	}
	int GetType()
	{
		ASSERT(nLocalYType==nLocalZType);
		return nLocalYType;
	}
};
struct T_RCCM_UDRD_D
{
	T_RCCM_K key;
	T_RCCM_D data;
};
#define HASHSIZERCCM HASHSIZEELEM

// Seismic Evaluation - P-M Curve Calculation Method

#define T_PMCM_K T_ELEM_K
struct T_PMCM_D
{
	int nCalcMethod;
	void Initialize()
	{
		nCalcMethod = D_SEME_PMCM_E;
	}
};
struct T_PMCM_UDRD_D
{
	T_PMCM_K key;
	T_PMCM_D data;
};
#define HASHSIZEPMCM HASHSIZEELEM

// Seismic Evaluation - Initialize/Update Hinge Property Option
enum D_IUHP_MATL { D_IUHP_MATL_DESIGN=0, D_IUHP_MATL_SEIS };
enum { D_DOF_FX=0, D_DOF_FY, D_DOF_FZ, D_DOF_MX, D_DOF_MY, D_DOF_MZ, D_DOF_WP };
#define T_IUHP_K UINT

struct T_IUHP_D_930
{
	BOOL bBeam;
	BOOL bBeamDof[6];
	BOOL bColm;
	BOOL bColmDof[6];
	BOOL bWall;
	BOOL bWallDof[6];
	BOOL bStlBeam;
	BOOL bStlBeamDof[6];
	BOOL bStlColm;
	BOOL bStlColmDof[6];
	BOOL bStlBrac;
	BOOL bStlBracBeamDof[6];
	BOOL bStlBracTrssDof[6];
	BOOL bInfillStrut;
	BOOL bInfillStrutDof[6];
	BOOL bUseBraceBeam;
	BOOL bPOStepFound;
	BOOL bRCColmFxInitStiffAsym; // RC기둥 축력성분 초기강성 비대칭(인장측은 철근만 고려)
	T_KEY PolcK;
	int nStep;
	int nUpdateMaterial; // Material for Calculation of Strength : Axial & Moment
};

struct T_IUHP_D
{
	BOOL bBeam;
	BOOL bBeamDof[6];
	BOOL bColm;
	BOOL bColmDof[6];
	BOOL bWall;
	BOOL bWallDof[6];
	BOOL bStlBeam;
	BOOL bStlBeamDof[6];
	BOOL bStlColm;
	BOOL bStlColmDof[6];
	BOOL bStlBrac;
	BOOL bStlBracBeamDof[6];
	BOOL bStlBracTrssDof[6];
	BOOL bInfillStrut;
	BOOL bInfillStrutDof[6];
	BOOL bUseBraceBeam;
	BOOL bPOStepFound;
	BOOL bRCColmFxInitStiffAsym; // RC기둥 축력성분 초기강성 비대칭(인장측은 철근만 고려)
	T_KEY PolcK;
	int nStep;
	int nUpdateMaterial; // Material for Calculation of Strength : Axial & Moment
	double dResidualStrengthRatioBeam;       // Beam hinge update option : Residual strength ratio (c)
	double dResidualStrengthRatioColm;       // Column hinge update option : Residual strength ratio (c)
	double dYieldAndLimitStrengthRatioBeam;  // Beam hinge update option : Ratio of yield Strength & limit strength
	double dYieldAndLimitStrengthRatioColm;  // Column hinge update option : Ratio of yield Strength & limit strength

	T_IUHP_D() { Initialize(); }
	T_IUHP_D(T_IUHP_D& src)
	{
		*this = src;
	}
	void Initialize()
	{
		bBeam=TRUE;
		memset(bBeamDof, 0, sizeof(bBeamDof));
		bBeamDof[D_DOF_FZ]=TRUE;
		bBeamDof[D_DOF_MY]=TRUE;

		bColm=TRUE;
		memset(bColmDof, 0, sizeof(bColmDof));
		bColmDof[D_DOF_FX]=TRUE;
		bColmDof[D_DOF_FY]=TRUE;
		bColmDof[D_DOF_FZ]=TRUE;
		bColmDof[D_DOF_MY]=TRUE;
		bColmDof[D_DOF_MZ]=TRUE;

		bWall=TRUE;
		memset(bWallDof, 0, sizeof(bWallDof));
		bWallDof[D_DOF_FX]=TRUE;
		bWallDof[D_DOF_FZ]=TRUE;
		bWallDof[D_DOF_MY]=TRUE;

		bStlBeam=TRUE;
		memset(bStlBeamDof, 0, sizeof(bStlBeamDof));
		bStlBeamDof[D_DOF_MY]=TRUE;

		bStlColm=TRUE;
		memset(bStlColmDof, 0, sizeof(bStlColmDof));
		bStlColmDof[D_DOF_FX]=TRUE;
		bStlColmDof[D_DOF_MY]=TRUE;
		bStlColmDof[D_DOF_MZ]=TRUE;

		bStlBrac=TRUE;

		memset(bStlBracBeamDof, 0, sizeof(bStlBracBeamDof));
		bStlBracBeamDof[D_DOF_FX]=TRUE;
		bStlBracBeamDof[D_DOF_MY]=TRUE;
		bStlBracBeamDof[D_DOF_MZ]=TRUE;

		memset(bStlBracTrssDof, 0, sizeof(bStlBracTrssDof));
		bStlBracTrssDof[D_DOF_FX]=TRUE;

		bInfillStrut=TRUE;
		memset(bInfillStrutDof, 0, sizeof(bInfillStrutDof));
		bInfillStrutDof[D_DOF_FX]=TRUE;

		bUseBraceBeam=FALSE;

		bPOStepFound=FALSE;
		bRCColmFxInitStiffAsym = FALSE;
		PolcK = 0;
		nStep = 0;
		nUpdateMaterial = D_IUHP_MATL_SEIS;
		dResidualStrengthRatioBeam = 0.0;
		dResidualStrengthRatioColm = 0.0;
		dYieldAndLimitStrengthRatioBeam = 1.0;
		dYieldAndLimitStrengthRatioColm = 1.0;
	}
	void Initialize_AIK_2021()
	{
		Initialize();
		bColmDof[D_DOF_FX]=FALSE;
	}

	void Convert930(T_IUHP_D_930& rData)
	{
		Initialize();
		bBeam = rData.bBeam;
		memcpy(bBeamDof, rData.bBeamDof, sizeof(bBeamDof));
		bColm = rData.bColm;
		memcpy(bColmDof, rData.bColmDof, sizeof(bColmDof));
		bWall = rData.bWall;
		memcpy(bWallDof, rData.bWallDof, sizeof(bWallDof));
		bStlBeam = rData.bStlBeam;
		memcpy(bStlBeamDof, rData.bStlBeamDof, sizeof(bStlBeamDof));
		bStlColm = rData.bStlColm;
		memcpy(bStlColmDof, rData.bStlColmDof, sizeof(bStlColmDof));
		bStlBrac = rData.bStlBrac;
		memcpy(bStlBracBeamDof, rData.bStlBracBeamDof, sizeof(bStlBracBeamDof));
		memcpy(bStlBracTrssDof, rData.bStlBracTrssDof, sizeof(bStlBracTrssDof));
		bInfillStrut = rData.bInfillStrut;
		memcpy(bInfillStrutDof, rData.bInfillStrutDof, sizeof(bInfillStrutDof));
		bUseBraceBeam = rData.bUseBraceBeam;
		bPOStepFound = rData.bPOStepFound;
		bRCColmFxInitStiffAsym = rData.bRCColmFxInitStiffAsym;
		PolcK = rData.PolcK;
		nStep = rData.nStep;
		nUpdateMaterial = rData.nUpdateMaterial;
	}
};
struct T_IUHP_UDRD_D
{
	T_IUHP_D data;
};
#define HASHSIZEIUHP 1

// Seismic Evaluation - Initialize/Update Hinge Property Option
enum D_CESO_METHOD { D_CESO_BY_CODE=0, D_CESO_BY_USER };
#define T_CESO_K UINT
struct T_CESO_D
{
	int nMethod;
	double dTensSign;
	double dAxialPart1;
	double dAxialPart2;
	double dBendingPart1;
	double dBendingPart2;

	T_CESO_D() { Initialize(); }
	T_CESO_D(T_CESO_D& src)
	{
		*this = src;
	}
	void Initialize()
	{
		nMethod = D_CESO_BY_USER;

		dTensSign = -1.0; // 인장(Fx가 양수)이면 Axial Load Ratio에 음수 부호 붙임
		dAxialPart1 = 0.1;
		dAxialPart2 = 0.5;
		dBendingPart1 = 0.3;
		dBendingPart2 = 0.7;
	}
};
struct T_CESO_UDRD_D
{
	T_CESO_D data;
};
#define HASHSIZECESO 1

enum EN_SEBE_DETAIL { EN_SEBE_DETL_SEIS = 0, EN_SEBE_DETL_NONE, EN_SEBE_DETL_AUTO, };

struct T_SEBE_D
{
	UINT nDetail;
	void Initialize()
	{
		nDetail = EN_SEBE_DETL_AUTO;
	}
	T_SEBE_D() { Initialize(); }
};

#define HASHSIZESEBE HASHSIZEELEM
#define T_SEBE_K T_ELEM_K
typedef CArray<T_SEBE_K, T_SEBE_K > T_SEBE_K_LIST;
typedef CArray<T_SEBE_D, T_SEBE_D&> T_SEBE_D_LIST;

struct T_SEBE_UDRD_D
{
	T_SEBE_K key;
	T_SEBE_D data;
};

enum EN_SECO_DETAIL { EN_SECO_DETL_SEIS = 0, EN_SECO_DETL_NONE, };

struct T_SECO_D
{
	UINT nDetail;
	void Initialize()
	{
		nDetail = EN_SECO_DETL_SEIS;
	}
	T_SECO_D() { Initialize(); }
};

#define T_SECO_K T_ELEM_K
#define HASHSIZESECO HASHSIZEELEM
typedef CArray<T_SECO_K, T_SECO_K > T_SECO_K_LIST;
typedef CArray<T_SECO_D, T_SECO_D&> T_SECO_D_LIST;

struct T_SECO_UDRD_D
{
	T_SECO_K key;
	T_SECO_D data;
};

enum EN_SEWA_BNDR_WALL { EN_SEWA_BNDR_SEIS = 0, EN_SEWA_BNDR_MITI, EN_SEWA_BNDR_NONE };

struct T_SEWA_D
{
	UINT nDetail;
	void Initialize()
	{
		nDetail = EN_SEWA_BNDR_NONE;
	}
	T_SEWA_D() { Initialize(); }
};

#define T_SEWA_K T_ELEM_K
#define HASHSIZESEWA HASHSIZEELEM
typedef CArray<T_SEWA_K, T_SEWA_K > T_SEWA_K_LIST;
typedef CArray<T_SEWA_D, T_SEWA_D&> T_SEWA_D_LIST;

struct T_SEWA_UDRD_D
{
	T_SEWA_K key;
	T_SEWA_D data;
};

struct T_HLFW_D
{
	double dLp;
	void Initialize()
	{
		dLp = 0.5;
	}
	T_HLFW_D() { Initialize(); }
};

#define T_HLFW_K T_ELEM_K
#define HASHSIZEHLFW HASHSIZEELEM
typedef CArray<T_HLFW_K, T_HLFW_K > T_HLFW_K_LIST;
typedef CArray<T_HLFW_D, T_HLFW_D&> T_HLFW_D_LIST;

struct T_HLFW_UDRD_D
{
	T_HLFW_K key;
	T_HLFW_D data;
};

struct T_SESP_D
{
	BOOL   bReal; // FALSE : Relative, TRUE : Real Length
	UINT   nMembType;
	double dLsyi;
	double dLsyj;
	double dLszi;
	double dLszj;
	void Initialize()
	{
		bReal = FALSE;
		nMembType = D_MBTP_BEAM;
		dLsyi = 0.5;
		dLsyj = 0.5;
		dLszi = 0.5;
		dLszj = 0.5;
	}
	T_SESP_D() { Initialize(); }
};

#define T_SESP_K T_ELEM_K
#define HASHSIZESESP HASHSIZEELEM
typedef CArray<T_SESP_K, T_SESP_K > T_SESP_K_LIST;
typedef CArray<T_SESP_D, T_SESP_D&> T_SESP_D_LIST;

struct T_SESP_UDRD_D
{
	T_SESP_K key;
	T_SESP_D data;
};

// 성능기반설계 - Hinge Property by Elastic Analsysis Results
enum D_HPEA_MATL { D_HPEA_MATL_DESIGN = 0, D_HPEA_MATL_SEIS };
struct T_HPEA_LOAD
{
	unsigned int LoadCaseKey;
	int nLoadCaseType;
	double dScaleFactor;
	T_HPEA_LOAD() : LoadCaseKey(0), nLoadCaseType(0), dScaleFactor(1.0) {}
	T_HPEA_LOAD(unsigned int key, int type, double fac) : LoadCaseKey(key), nLoadCaseType(type), dScaleFactor(fac) {}
};
#define T_HPEA_K UINT
struct T_HPEA_D
{
	BOOL bBeam;
	BOOL bColumn;
	CArray<T_HPEA_LOAD, T_HPEA_LOAD&> aGravLoad;
	CArray<T_HPEA_LOAD, T_HPEA_LOAD&> aSeisLoad;
	double dOverStrengthFactor;
	// settings
	BOOL bBeamDof[6];
	BOOL bColmDof[6];
	double dBeamResidualRatio;  // Beam hinge update option : Residual strength ratio (c)
	double dBeamYSRatio;		// Column hinge update option : Residual strength ratio (c)
	double dColmResidualRatio;  // Beam hinge update option : Ratio of yield Strength & limit strength
	double dColmYSRatio;		// Column hinge update option : Ratio of yield Strength & limit strength

	T_HPEA_D() { Initialize(); }
	T_HPEA_D(T_HPEA_D& src)
	{
		*this = src;
	}
	void Initialize()
	{
		bBeam = TRUE;
		bColumn = TRUE;
		aGravLoad.RemoveAll();
		aSeisLoad.RemoveAll();
		dOverStrengthFactor = 2.5;
		memset(bBeamDof, 0, sizeof(bBeamDof));
		bBeamDof[D_DOF_FZ]=TRUE;
		bBeamDof[D_DOF_MY]=TRUE;
		memset(bColmDof, 0, sizeof(bColmDof));
		bColmDof[D_DOF_FX]=TRUE;
		bColmDof[D_DOF_FY]=TRUE;
		bColmDof[D_DOF_FZ]=TRUE;
		bColmDof[D_DOF_MY]=TRUE;
		bColmDof[D_DOF_MZ]=TRUE;
		dBeamResidualRatio = 0.0;
		dBeamYSRatio	   = 1.0;
		dColmResidualRatio = 0.0;
		dColmYSRatio	   = 1.0;
	}
	T_HPEA_D& operator=(const T_HPEA_D& src)
	{
		bBeam = src.bBeam;
		bColumn = src.bColumn;
		aGravLoad.Copy(src.aGravLoad);
		aSeisLoad.Copy(src.aSeisLoad);
		dOverStrengthFactor = src.dOverStrengthFactor;
		memcpy(bBeamDof, src.bBeamDof, sizeof(bBeamDof));
		memcpy(bColmDof, src.bColmDof, sizeof(bColmDof));
		dBeamResidualRatio	= src.dBeamResidualRatio;
		dBeamYSRatio = src.dBeamYSRatio;
		dColmResidualRatio = src.dColmResidualRatio;
		dColmYSRatio = src.dColmYSRatio;
		return *this;
	}
};
struct T_HPEA_UDRD_D
{
	T_HPEA_D data;
};

struct T_HPEA_UDRD_D_RW
{
	BOOL bBeam;
	BOOL bColm;
	int nGravLoadCnt;
	int nSeisLoadCnt;
	double dOverStrengthFactor;
	BOOL bBeamDof[6];
	BOOL bColmDof[6];
	double dBeamResidualRatio;
	double dBeamYSRatio;
	double dColmResidualRatio;
	double dColmYSRatio;
	void GetHpea(T_HPEA_D& rData)
	{
		rData.bBeam = bBeam;
		rData.bColumn = bColm;
		rData.dOverStrengthFactor = dOverStrengthFactor;
		memcpy(rData.bBeamDof, bBeamDof, sizeof(bBeamDof));
		memcpy(rData.bColmDof, bColmDof, sizeof(bColmDof));
		rData.dBeamResidualRatio = dBeamResidualRatio;
		rData.dBeamYSRatio = dBeamYSRatio;
		rData.dColmResidualRatio = dColmResidualRatio;
		rData.dColmYSRatio = dColmYSRatio;
	}
	void SetHpea(T_HPEA_D& rData)
	{
		bBeam = rData.bBeam;
		bColm = rData.bColumn;
		nGravLoadCnt = (int)rData.aGravLoad.GetSize();
		nSeisLoadCnt = (int)rData.aSeisLoad.GetSize();
		dOverStrengthFactor = rData.dOverStrengthFactor;
		memcpy(bBeamDof, rData.bBeamDof, sizeof(bBeamDof));
		memcpy(bColmDof, rData.bColmDof, sizeof(bColmDof));
		dBeamResidualRatio = rData.dBeamResidualRatio;
		dBeamYSRatio = rData.dBeamYSRatio;
		dColmResidualRatio = rData.dColmResidualRatio;
		dColmYSRatio = rData.dColmYSRatio;
	}
};

#define HASHSIZEHPEA 1

// 성능기반설계 - Define Axial Strain Criteria

struct T_MATL_STRN_RC
{
	UINT MatlK;
	double dAllowStrain;
	BOOL  bConfined;
	T_MATL_STRN_RC() : MatlK(0), dAllowStrain(0.0), bConfined(FALSE) {}
	T_MATL_STRN_RC(UINT key, double dAllow, BOOL bCon) : MatlK(key), dAllowStrain(dAllow), bConfined(bCon) {}
};
struct T_MATL_STRN_STL_940
{
	UINT MatlK;
	double dAllowStrain;
	T_MATL_STRN_STL_940() : MatlK(0), dAllowStrain(0.0) {}
	T_MATL_STRN_STL_940(UINT key, double dAllow) : MatlK(key), dAllowStrain(dAllow){}
};
struct T_MATL_STRN_STL
{
	UINT MatlK;
	double dAllowStrain;
	BOOL  bSeismic;
	T_MATL_STRN_STL() : MatlK(0), dAllowStrain(0.0), bSeismic(FALSE) {}
	T_MATL_STRN_STL(UINT key, double dAllow, BOOL bSeis) : MatlK(key), dAllowStrain(dAllow), bSeismic(bSeis) {}
	T_MATL_STRN_STL& operator = (const T_MATL_STRN_STL_940& src)
	{
		MatlK = src.MatlK;
		dAllowStrain = src.dAllowStrain;
		bSeismic = FALSE;

		return *this;
	}
};
#define T_DASC_K UINT
struct T_DASC_D
{
	CArray<T_MATL_STRN_RC, T_MATL_STRN_RC&> aMatlConcData;
	CArray<T_MATL_STRN_STL, T_MATL_STRN_STL&> aMatlStlData;
	
	T_DASC_D() { Initialize(); }
	T_DASC_D(T_DASC_D& src)
	{
		*this = src;
	}
	void Initialize()
	{
		aMatlConcData.RemoveAll();
		aMatlStlData.RemoveAll();
	}
	T_DASC_D& operator=(const T_DASC_D& src)
	{
		aMatlConcData.Copy(src.aMatlConcData);
		aMatlStlData.Copy(src.aMatlStlData);
		return *this;
	}
};
struct T_DASC_UDRD_D
{
	T_DASC_D data;
};

struct T_DASC_UDRD_D_RW
{
	int nMatlConcDataCnt;
	int nMatlStlDataCnt;
	void GetDasc(T_DASC_D& rData)
	{
	}
	void SetDasc(T_DASC_D& rData)
	{
		nMatlConcDataCnt = (int)rData.aMatlConcData.GetSize();
		nMatlStlDataCnt = (int)rData.aMatlStlData.GetSize();
	}
};

#define HASHSIZEDASC 1
#pragma pack(pop)

#endif  // __DB_ST_DT_SEIS_H__
