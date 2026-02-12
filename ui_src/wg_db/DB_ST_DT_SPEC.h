#ifndef __DB_ST_DT_SPEC_H__
#define __DB_ST_DT_SPEC_H__

#define MIDAS_SDK

#pragma pack(push, 1)

#include "DB_ST_DT_TIME.h"

///// ---------------  Spectrum Function Data (SPFC)  -----------------------------------------------------------

// 여기 추가시 관계있는 부분
// BOOL CEditData::Checkspfc_DgnCodeData(T_SPFC_D& rData) - 여긴 관계 거의 업슴
// int CETC_DESIGN_SPECT::GetCodeFromItem(int nItem)
// CETC_DESIGN_SPECT::CETC_DESIGN_SPECT(
// BOOL CMgtExport::WriteSpfc_CodeData(T_SPFC_D& data) - Main

#define D_SPFC_CODE_USER           0
#define D_SPFC_CODE_KS             1
#define D_SPFC_CODE_KS2000         2
#define D_SPFC_CODE_KS_BRG         3
#define D_SPFC_CODE_UBC88          4
#define D_SPFC_CODE_UBC97          5
#define D_SPFC_CODE_EURO1996       6
#define D_SPFC_CODE_CH2002         7
#define D_SPFC_CODE_IBC2000        8
#define D_SPFC_CODE_JPN2000        9
#define D_SPFC_CODE_CH_BRG89       10
#define D_SPFC_CODE_CH_GBJ111_87   11
#define D_SPFC_CODE_NBC95          12
#define D_SPFC_CODE_EURO1996_ELA   13
#define D_SPFC_CODE_IS2002         14
#define D_SPFC_CODE_TAIWAN99H      15
#define D_SPFC_CODE_TAIWAN99V      16
#define D_SPFC_CODE_TAIWAN89H_BRG  17
#define D_SPFC_CODE_TAIWAN89V_BRG  18
#define D_SPFC_CODE_JP_BRG2002     19
#define D_SPFC_CODE_KBC2005        20
#define D_SPFC_CODE_CHSH2003       21
#define D_SPFC_CODE_EURO2004       22
#define D_SPFC_CODE_TAIWAN06       23
#define D_SPFC_CODE_GB50111_2006   24
#define D_SPFC_CODE_JTJXXX_2008    25
#define D_SPFC_CODE_KBC2009        26
#define D_SPFC_CODE_USER_TYPE      27
#define D_SPFC_CODE_TAIWANBRG98    28
#define D_SPFC_CODE_CH2010         29  // Add by GAY. PMS:4267. ('11.10.11). 중국 신하중기준 추가.
#define D_SPFC_CODE_CJJ166_2011    30  // Add by maxiao for China.2012-2-28
#define D_SPFC_CODE_IBC2009        31
#define D_SPFC_CODE_IBC2012        32
#define D_SPFC_CODE_AASHTO_LRFD12  33
#define D_SPFC_CODE_NSR2010        34
#define D_SPFC_CODE_KBC2015        35
#define D_SPFC_CODE_P100_2013      36
#define D_SPFC_CODE_IS1893_2016    37
#define D_SPFC_CODE_KISTEC2013     38  // 현재 사용은 안되고 있음.
#define D_SPFC_CODE_KSCE2019       39
#define D_SPFC_CODE_NTC2018        40
#define D_SPFC_CODE_DPWH_LRFD_2013 41
#define D_SPFC_CODE_KDS2019        42
#define D_SPFC_CODE_AS_5100_2_2017 43
#define D_SPFC_CODE_CH2019         44 // 현재 사용은 안되고 있음.
#define D_SPFC_CODE_IRCSP114_2018  45
#define D_SPFC_CODE_JTG_T_2231_01_2020 46
#define D_SPFC_CODE_JP_BRG2012     47
#define D_SPFC_CODE_JP_BRG2017     48
#define D_SPFC_CODE_DPT2018        49
#define D_SPFC_CODE_CH_GB_T_51408_2021 50
#define D_SPFC_CODE_TAIWAN22       51
#define D_SPFC_CODE_RUS_SP2016     52
#define D_SPFC_CODE_NSCP_2024      53
#define D_SPFC_CODE_SANS	       54
#define D_SPFC_CODE_RUS_SP14_2018  55
#define D_SPFC_CODE_IS1893_2025    56
#define D_SPFC_CODE_COUNT          D_SPFC_CODE_IS1893_2025
#define D_SPFC_CODE_UNDEFINED     99

#define PRD_ST 0.0
#define PRD_ED 6.0
#define PRD_NUM 100

#if defined(_CIVIL_RUS_LOCAL) || defined(_MGEN_RUS_LOCAL)
	#define SPFC_RUS_SP_268_1325800	_T("СП 268.1325800.2016")
	#define SPFC_RUS_SP_14_13330	_T("СП 14.13330.2018")
#else
	#define SPFC_RUS_SP_268_1325800	_T("SP 268.1325800.2016")
	#define SPFC_RUS_SP_14_13330	_T("SP 14.13330.2018")
#endif

enum EN_SPEC_TYPE
{
	EN_HORI_ELASTIC, EN_VERT_ELASTIC, EN_HORI_DESIGN, EN_VERT_DESIGN
};

enum EN_SPEC_TYPE_SINGAPORE
{
	EN_HORI_ELASTIC_SIN=10, EN_HORI_DESIGN_SIN=11
};

struct T_SPFC_CH_BRG89      // China(JTJ004-89)
{
	int     nSFI;               // Seismic Fortification Intensity
	double  dKh;                // Horizontal Seismic Coef.(Kh)
	int     nSc;                // Site Class [ 0:I, 1:II, 2:III, 3:IV ]
	double  dTg;                // Design Characteristic Period of Ground Motion
	double  dCi;                // Importance Factor(Ci)
	double  dCz;                // General Effect Factor(Cz)
	double  dMaxPeriod;         // Max. Period
	void Initialize()
	{
		nSFI = 0;
		dKh  = 0.1;
		nSc  = 0;
		dTg  = 0.2;
		dCi  = 1.0;
		dCz  = 0.20;
		dMaxPeriod  = 6.0;
	}
};

struct T_SPFC_CH_GBJ111_87    // China(GBJ11-87)
{
	int     nSFI;               // Seismic Fortification Intensity
	double  dKh;                // Horizontal Seismic Coef.(Kh)
	int     nSc;                // Site Class [ 0:I, 1:II, 2:III, 3:IV ]
	double  dTg1;               // Design Characteristic Period of Ground Motion
	double  dTg2;               // Design Characteristic Period of Ground Motion
	double  dCz;                // General Effect Factor(Cz)
	double  dMaxPeriod;         // Max. Period
	void Initialize()
	{
		nSFI = 0;
		dKh  = 0.1;
		nSc  = 0;
		dTg1  = 0.2;
		dTg2  = 1.0;
		dCz  = 0.20;
		dMaxPeriod  = 6.0;
	}
};

struct T_SPFC_CH2002_835    // China(GBJ111-87) -> China(GB50011-01)
{
	int nSeisDgnCategory;     // 0:1, 1:2, 2:3
	int nSeisFortification;   // 0:0.05g, 1:0.10g, 2:0.15g, 3:0.20g, 4:0.30g, 5:0.40g
	int nSiteClass;           // 0:I, 1:II, 2:III, 3:IV
	int nEarthResponse;       // 0:FREQUENT, 1:SCARSE
	double dTg;               // Dgn Characteristic Period of Ground Motion
	double dDamping;
	//double dMaxPeriod;        // 데이터가 있긴하나 DB에서 Ctrl하지 않아도 됨.
};

struct T_SPFC_CH2002        // China(GBJ111-87) -> China(GB50011-01)
{
	int     nSeisDgnCategory; // 0:1, 1:2, 2:3
	int     nSeisFortification; // 0:0.05g, 1:0.10g, 2:0.15g, 3:0.20g, 4:0.30g, 5:0.40g
	int     nSiteClass; // 0:I, 1:II, 2:III, 3:IV
	int     nEarthResponse; // 0:FREQUENT, 1:SCARSE
	double  dTg; // Dgn Characteristic Period of Ground Motion
	double  dDamping;
	//double dMaxPeriod;        // 데이터가 있긴하나 DB에서 Ctrl하지 않아도 됨.
	double  dMaxEQE; // Maximum Effect Coefficient
	double dMaxPeriod; // 9.5.5
	void Initialize()
	{
		nSeisDgnCategory = 0;
		nSeisFortification = 0;
		nSiteClass = 0;
		nEarthResponse = 0;
		dTg      = 0.25;
		dDamping = 0.05;
		dMaxEQE  = 0.04;
		dMaxPeriod = 6.0;
	}
};

struct T_SPFC_CHSH2003_835  // China Shanghai(DGJ08-9-2003)
{
	int nSeisFortification;   // 0:6(0.05g), 1:7(0.1g), 2:8(0.2g)
	int nSiteClass;           // 0:III, 1:IV
	int nEarthResponse;       // 0:FREQUENT, 1:SCARSE
	double dTg;               // Dgn Characteristic Period of Ground Motion
	double dDamping;
};

struct T_SPFC_CHSH2003      // China Shanghai(DGJ08-9-2003)
{
	int     nSeisFortification; // 0:6(0.05g), 1:7(0.1g), 2:8(0.2g)
	int     nSiteClass; // 0:III, 1:IV
	int     nEarthResponse; // 0:FREQUENT, 1:SCARSE
	double  dTg; // Dgn Characteristic Period of Ground Motion
	double  dDamping;
	double  dMaxEQE; // Maximum Effect Coefficient
	double dMaxPeriod; // 9.5.5

	void Initialize()
	{
		nSeisFortification = 0;
		nSiteClass = 0;
		nEarthResponse = 0;
		dTg      = 0.65;
		dDamping = 0.05;
		dMaxEQE  = 0.04;
		dMaxPeriod = 6.0;
	}
};

// MNET:2744 20070706 mylee 7.3.0
struct T_SPFC_GB50111_2006    // China(GB50111_2006)
{
	int nDivision;            // 0:1, 1:2, 2:3
	int nSiteClass;           // 0:I, 1:II, 2:III, 3:IV
	int nSeisFortification;   // 0:0.05g, 1:0.10g, 2:0.15g, 3:0.20g, 4:0.30g, 5:0.40g
	int nEarthResponse;       // 0:FREQUENT, 1:DESIGN, 2:SCARSE
	double dTg;               // Dgn Characteristic Period of Ground Motion
	double dG;                // Basic horizontal seismic acceleration alpha(g)
	BOOL bImportantBridge;    // important bridge
	double dMaxPeriod; // 9.5.5

	void Initialize()
	{
		nDivision = 0;
		nSiteClass = 0;
		nSeisFortification = 0;
		nEarthResponse = 0;
		dTg      = 0.25;
		dG       = 0.02;
		bImportantBridge = FALSE;
		dMaxPeriod = 6.0;
	}
};

// MNET:3615 20080818 - 7.6.0
struct T_SPFC_JTJXXX_2008_770    // China(JTJXXX_2008) -> China(JTG/T B02-01-2008)
{
	// Input Data
	int nBridgeType;         // 0:A, 1:B, 2:C, 3:D
	int nZoningMap;          // 0:0.35, 1:0.4, 2:0.45
	int nSiteType;           // 0:1, 1:2, 2:3, 3:4
	int nSeisminIntensity;   // 0:6(0.05g), 1:7(0.10g), 2:7(0.15g), 3:8(0.20g), 4:8(0.30g), 5:9(0.40g)
	int nSeisminCriterion;   // 0:E1, 1:E2

	double dDampingRatio;

	BOOL bLargeBridge;

	// Output Data
	double dTg;
	double dCi;
	double dCs;
	double dCd;
	double dEPA;
	double dSMax;
	double dMaxPeriod;
};

struct T_SPFC_JTJXXX_2008    // China(JTJXXX_2008) -> China(JTG/T B02-01-2008)
{
	// Input Data
	int nBridgeType;         // 0:A, 1:B, 2:C, 3:D
	int nZoningMap;          // 0:0.35, 1:0.4, 2:0.45
	int nSiteType;           // 0:1, 1:2, 2:3, 3:4
	int nSeisminIntensity;   // 0:6(0.05g), 1:7(0.10g), 2:7(0.15g), 3:8(0.20g), 4:8(0.30g), 5:9(0.40g)
	int nSeisminCriterion;   // 0:E1, 1:E2

	double dDampingRatio;

	BOOL bLargeBridge;
	BOOL bVertAccSpec;       // 수직방향 가속도 스펙트럼
	int  nVertAccSpecType;   // 0:염층 1:토층

	// Output Data
	double dTg;
	double dCi;
	double dCs;
	double dCd;
	double dEPA;
	double dSMax;
	double dMaxPeriod;

	void Initialize()
	{
		nBridgeType = 0;
		nZoningMap = 0;
		nSiteType = 0;
		nSeisminIntensity =0;
		nSeisminCriterion =0;

		dDampingRatio = 0.05;

		bLargeBridge = FALSE;
		bVertAccSpec = FALSE;
		nVertAccSpecType = 0;

		dTg = 0.25;
		dCi = 1.0;
		dCs = 1.20;
		dCd = 1.0;
		dEPA = 0.05;
		dSMax = 0.135;
		dMaxPeriod = 6.0;
	}
	void Convert770(T_SPFC_JTJXXX_2008_770& rData)
	{
		nBridgeType = rData.nBridgeType;
		nZoningMap = rData.nZoningMap;
		nSiteType = rData.nSiteType;
		nSeisminIntensity = rData.nSeisminIntensity;
		nSeisminCriterion = rData.nSeisminCriterion;

		dDampingRatio = rData.dDampingRatio;

		bLargeBridge = rData.bLargeBridge;
		bVertAccSpec = FALSE;
		nVertAccSpecType = 0;

		dTg = rData.dTg;
		dCi = rData.dCi;
		dCs = rData.dCs;
		dCd = rData.dCd;
		dEPA = rData.dEPA;
		dSMax = rData.dSMax;
		dMaxPeriod = rData.dMaxPeriod;
	}
};

struct T_SPFC_JTG_T_2231_01_2020    // China(JTJXXX_2008) -> China(JTG/T B02-01-2008)
{
	// Input Data
	int nBridgeType;         // 0:A, 1:B, 2:C, 3:D
	int nZoningMap;          // 0:0.35, 1:0.4, 2:0.45
	int nSiteType;           // 0:1(0), 1:1(1), 2:2, 3:3, 4:4
	int nSeisminIntensity;   // 0:6(0.05g), 1:7(0.10g), 2:7(0.15g), 3:8(0.20g), 4:8(0.30g), 5:9(0.40g)
	int nSeisminCriterion;   // 0:E1, 1:E2

	double dDampingRatio;

	BOOL bLargeBridge;
	BOOL bVertResSpec;       // 수직방향 가속도 스펙트럼

	// Output Data
	double dTg;
	double dCi;
	double dCs;
	double dCd;
	double dA;
	double dSMax;
	double dMaxPeriod;

	void Initialize()
	{
		nBridgeType = 0;
		nZoningMap = 0;
		nSiteType = 0;
		nSeisminIntensity = 0;
		nSeisminCriterion = 0;

		dDampingRatio = 0.05;

		bLargeBridge = FALSE;
		bVertResSpec = FALSE;

		dTg = 0.20;
		dCi = 1.0;
		dCs = 0.72;
		dCd = 1.0;
		dA = 0.05;
		dSMax = 0.135;
		dMaxPeriod = 6.0;
	}
};

struct T_SPFC_TAIWAN89   // Taiwan89
{
	int    nSeisZone;    //주석달기
	int    nSoilType;
	double dIe;
	double dAlpha;
	double dMaxPeriod;
	double dCoef;		// 9.5.5
	double dDamping;	// 9.5.5

	void Initialize()
	{
		nSeisZone = 0;
		nSoilType = 0;
		dIe       = 1.0;
		dAlpha    = 1.2;
		dMaxPeriod = 6.0;
		dCoef = 1.0;
		dDamping = 0.05;
	}
};

struct T_SPFC_TAIWAN06_871   // Taiwan2006
{
	// MNET:2138-GSJI-20060525
	//union T_SPFC_CODE 의 크기때문에 임시로 구조체만 만듬.
	int    nSeismicZone;    //주석달기
	double dSds, dSd1;
	double dSms, dSm1;
	double dNda, dNdv;
	double dNma, dNmv;
	int    nSoilType;
	double dFda, dFdv;
	double dFma, dFmv;
	int    nSubZone;
	double dSds_t, dSms_t;
	double dTd0_t, dTm0_t;
	double dImportanceFactor;
	double dSeisMagnifyFactor;
	double dResponseModifyFactor;
};

struct T_SPFC_TAIWAN06   // Taiwan2006
{
	// union T_SPFC_CODE 의 크기때문에 임시로 구조체만 만듬.
	int    nSpectrumType;   // Spectrum Type
	int    nSeismicZone;    // Seismic Zone
	int    nSpectrumUsed;   // Spectrum Used;

	// spectral Response Acceleration
	double dSds, dSd1;
	double dSms, dSm1;

	// Near Source Factor
	double dNda, dNdv;
	double dNma, dNmv;

	// Site Magnify Factor
	int    nSoilType;
	double dFda, dFdv;
	double dFma, dFmv;

	int    nSubZone;
	double dSds_t, dSms_t;  // Spectral Response Acceleration
	double dTd0_t, dTm0_t;

	double dImportanceFactor;
	double dSeisMagnifyFactor;
	double dResponseModifyFactor;
	double dDampingRatio;
	double dMaxPeriod;

	void Initialize()
	{
		nSpectrumType = 0;
		nSeismicZone  = 0;
		nSpectrumUsed = 0;
		dSds    = 0.5;          dSd1  = 0.3;
		dSms    = 0.7;          dSm1  = 0.4;
		dNda    = 1.0;          dNdv  = 1.0;
		dNma    = 1.0;          dNmv  = 1.0;
		nSoilType = 0;
		dFda    = 1.0;          dFdv  = 1.0;
		dFma    = 1.0;          dFmv  = 1.0;
		nSubZone  = 0;
		dSds_t  = 0.6;          dSms_t = 0.8;
		dTd0_t  = 1.6;          dTm0_t = 1.6;
		dImportanceFactor     = 1.0;
		dSeisMagnifyFactor    = 1.2;
		dResponseModifyFactor = 1.6;
		dDampingRatio         = 5.0;
		dMaxPeriod            = 6.0;
	}
	void POInitialize(BOOL bBridge=FALSE)
	{
		nSpectrumType = 0;
		nSeismicZone  = 0;
		nSpectrumUsed = 0;
		dSds    = 0.5;          dSd1  = 0.3;
		dSms    = 0.7;          dSm1  = 0.45;
		dNda    = 1.0;          dNdv  = 1.0;
		dNma    = 1.0;          dNmv  = 1.0;
		nSoilType = 0;
		dFda    = 1.0;          dFdv  = 1.0;
		dFma    = 1.0;          dFmv  = 1.0;
		nSubZone  = 0;
		dSds_t  = 0.6;          dSms_t = 0.8;
		dTd0_t  = 1.6;          dTm0_t = 1.6;
		dImportanceFactor     = 1.0;
		dSeisMagnifyFactor    = ( bBridge ) ? 1.0 : 1.6;
		dResponseModifyFactor = 1.0;
		dDampingRatio         = 5.0;
		dMaxPeriod            = 6.0;
	}
	void Convert871(T_SPFC_TAIWAN06_871& rData)
	{
		nSpectrumType = 0;
		nSeismicZone  = rData.nSeismicZone;
		nSpectrumUsed = 0;

		dSds      = rData.dSds;
		dSd1      = rData.dSd1;
		dSms      = rData.dSms;
		dSm1      = rData.dSm1;
		dNda      = rData.dNda;
		dNdv      = rData.dNdv;
		dNma      = rData.dNma;
		dNmv      = rData.dNmv;
		nSoilType = rData.nSoilType;
		dFda      = rData.dFda;
		dFdv      = rData.dFdv;
		dFma      = rData.dFma;
		dFmv      = rData.dFmv;
		nSubZone  = rData.nSubZone;
		dSds_t    = rData.dSds_t;
		dSms_t    = rData.dSms_t;
		dTd0_t    = rData.dTd0_t;
		dTm0_t    = rData.dTm0_t;

		dImportanceFactor     = rData.dImportanceFactor;
		dSeisMagnifyFactor    = rData.dSeisMagnifyFactor;
		dResponseModifyFactor = rData.dResponseModifyFactor;
		dDampingRatio         = 5.0;
		dMaxPeriod            = 6.0;
	}
};

struct T_SPFC_TAIWAN22
{
	int nSpectrumType;   // Spectrum Type
	int nSeismicZone;    // Seismic Zone
	int nSpectrumUsed;   // Spectrum Used;

	// spectral Response Acceleration
	double dSds, dSd1;
	double dSms, dSm1;

	// Near Source Factor
	double dNda, dNdv;
	double dNma, dNmv;

	// Site Magnify Factor
	int    nSoilType;
	double dFda, dFdv;
	double dFma, dFmv;

	int    nSubZone;
	double dSds_t, dSms_t;  // Spectral Response Acceleration
	double dTd0_t, dTm0_t;

	double dImportanceFactor;
	double dSeisMagnifyFactor;
	double dResponseModifyFactor;
	double dDampingRatio;
	double dMaxPeriod;
	double dFundamentPeriod;

	void Initialize()
	{
		nSpectrumType = 0;
		nSeismicZone  = 0;
		nSpectrumUsed = 0;
		dSds = 0.50; dSd1 = 0.30;
		dSms = 0.70; dSm1 = 0.40;
		dNda = 0.80; dNdv = 0.45;
		dNma = 1.00; dNmv = 0.55;

		nSoilType = 0;
		dFda = 1.0; dFdv = 1.0;
		dFma = 1.0; dFmv = 1.0;

		nSubZone  = 0;
		dSds_t = 0.6; dSms_t = 0.8;
		dTd0_t = 1.6; dTm0_t = 1.6;

		dImportanceFactor     = 1.0;
		dSeisMagnifyFactor    = 1.2;
		dResponseModifyFactor = 1.6;
		dDampingRatio         = 5.0;
		dMaxPeriod            = 6.0;
		dFundamentPeriod      = 0.0;
	}
	void POInitialize(BOOL bBridge=FALSE)
	{
		nSpectrumType = 0;
		nSeismicZone  = 0;
		nSpectrumUsed = 0;
		dSds = 0.50; dSd1 = 0.30;
		dSms = 0.70; dSm1 = 0.45;
		dNda = 0.80; dNdv = 0.45;
		dNma = 1.00; dNmv = 0.55;

		nSoilType = 0;
		dFda = 1.0; dFdv = 1.0;
		dFma = 1.0; dFmv = 1.0;

		nSubZone = 0;
		dSds_t = 0.6; dSms_t = 0.8;
		dTd0_t = 1.6; dTm0_t = 1.6;

		dImportanceFactor     = 1.0;
		dSeisMagnifyFactor    = ( bBridge ) ? 1.0 : 1.6;
		dResponseModifyFactor = 1.0;
		dDampingRatio         = 5.0;
		dMaxPeriod            = 6.0;
		dFundamentPeriod      = 0.0;
	}
};

struct T_SPFC_EURO1996    // Euro1996
{
	int nSc;       // Soil Class(S)
	int nKd;       // Ductility Class(Kd)
	int nKr;       // Structural Regularity in Elevation(Kr)
	double dQ0;    // Basic Behavior Factor(Q0)
	double dKw;    // Failure Mode Factor (Kw)
	double dAlpha; // Ratio of design ground acceleration to gravity acceleration(alpha)
	double dMaxPeriod;    // Max. Period

	void Initialize()
	{
		nSc   = 0;
		nKd   = 0;
		nKr   = 0;
		dQ0   = 5.0;
		dKw   = 1.0;
		dAlpha= 0.2;

		dMaxPeriod      = 6.0;
	}
	void POInitialize()
	{
		nSc   = 0;
		nKd   = 0;
		nKr   = 0;
		dQ0   = 1.0;
		dKw   = 1.0;
		dAlpha= 0.2;

		dMaxPeriod      = 6.0;
	}
};

struct T_SPFC_EURO1996_ELA    // Euro1996_ELA
{
	int nSc;            // Soil Class(S)
	double dAg;         // Design Ground Acceleration(g)
	double dDamping;    // Viscous Damping Ratio(%)
	double dMaxPeriod;    // Max. Period

	void Initialize()
	{
		nSc        = 0;
		dAg        = 0.2;
		dDamping   = 5.0;
		dMaxPeriod = 6.0;
	}
};

enum EN_SPFC_NA
{
	EN_SPFC_RECOMMENDED = 0,
	EN_SPFC_SINGAPORE,
	EN_SPFC_MALESIA,
};
enum EN_SPFC_EURO2004_GROUND
{
	EN_SPFC_GROUND_A = 0,
	EN_SPFC_GROUND_B,
	EN_SPFC_GROUND_C,
	EN_SPFC_GROUND_D,
	EN_SPFC_GROUND_E,
	EN_SPFC_GROUND_S1,
	EN_SPFC_GROUND_S2,
// 	EN_SPFC_GROUND_R,
// 	EN_SPFC_GROUND_SS,
// 	EN_SPFC_GROUND_FS,
};
enum EN_SPFC_EURO2004_REGION
{
	EN_SPFC_REGION_PENINSULAR = 0,
	EN_SPFC_REGION_SABAH,
	EN_SPFC_REGION_SARAWAK,
};

// MNET:XXXX 20090407 sshan 7.7.0
struct T_SPFC_EURO2004    // Euro2004
{
	int nSpectrumType;    // 0:Horizontal Elastic, 1:Vertical Elastic, 2:Horizontal Design, 3:Vertical Design(Recommended), 10:Horizontal Elastic, 11:Horizontal Design (Singapore)
	int nGroundType;      // (Recommended)  0:A, 1:B, 2:C, 3:D, 4:E, 5:S1, 6:S2
						  // (Singapore)   10:C, 11:D, 12:S1
						  // (Malaysia)    13:Rock(R), 14:Stiff Soil(SS), 15:Flexible Soil(FS)
	int nParameterType;   // 0:Type1, 1:Type2, 2:User Defined
	double dSoilFactor;   // SoilFactor
	double dTb;           // Tb
	double dTc;           // Tc
	double dTd;           // Td
	double dAgr;          // Design Ground Acc. (AgR)
	double dI;            // Importance Factor(I)
	double dXi;           // Viscous Damping Ratio(Xi)
	double dQ;            // Behavior Factor(q)
	double dB;            // Lower Bound Factor(h)
	double dMaxPeriod;    // Max. Period
	double dTs;           // (Malaysia) Site Natural Period
	int nResionType;      // (Malaysia) 0:Peninsular  1:Sabah, 1:Sarawak  
	EN_SPFC_NA nNationalAnnex;
	void Initialize()
	{
		nSpectrumType   = 0;
		nGroundType     = EN_SPFC_GROUND_B;
		nParameterType  = 0;
		dSoilFactor     = 1.2;
		dTb             = 0.15;
		dTc             = 0.5;
		dTd             = 2.0;
		dAgr            = 0.08;
		dI              = 1.0;
		dXi             = 5.0;
		dQ              = 1.5;
		dB              = 0.2;
		dMaxPeriod      = 6.0;
		dTs             = 0.5;
		nResionType     = 0;
		nNationalAnnex = EN_SPFC_RECOMMENDED;
	}
	void POInitialize()
	{
		nSpectrumType   = 0;
		nGroundType     = EN_SPFC_GROUND_B;
		nParameterType  = 0;
		dSoilFactor     = 1.2;
		dTb             = 0.15;
		dTc             = 0.5;
		dTd             = 2.0;
		dAgr            = 0.08;
		dI              = 1.0;
		dXi             = 5.0;
		dQ              = 1.0;
		dB              = 0.2;
		dMaxPeriod      = 6.0;
		dTs             = 0.5;
		nResionType     = 0;
		nNationalAnnex = EN_SPFC_RECOMMENDED;
	}
};

struct T_SPFC_IBC2000    // IBC2000
{
	int nSc;       // Site Class
	double dSs;    // Mapped Spectral Response Acceleration at Short Periods(Ss)
	double dS1;    // Mapped Spectral Response Acceleration at 1sec Period(S1)
	double dMaxPeriod;    // Max. Period

	void Initialize()
	{
		nSc   = 0;
		dSs   = 1.0;
		dS1   = 0.4;

		dMaxPeriod      = 6.0;
	}
};

struct T_SPFC_IBC2012    // IBC2012
{
	int nSc;       // Site Class
	double dSs;    // Spectral Acceleration(Ss)
	double dS1;    // Spectral Acceleration(S1)
	double dFa;
	double dFv;
	double dSds;
	double dSd1;
	double dIe;    // Importance Factor (Ie)
	double dCoef;  // Response Modification Coef. (R)
	double dTL;    // Long Tran. Period(TL)

	double dMaxPeriod;    // Max. Period

	void Initialize()
	{
		nSc   = 3;
		dSs   = 0.75;
		dS1   = 0.3;
		dFa   = 1.2;
		dFv   = 1.8;
		dSds  = 0.6;
		dSd1  = 0.36;
		dCoef = 4.0;
		dIe   = 1.0;
		dTL   = 4.0;

		dMaxPeriod = 6.0;
	}

	void POInitialize()
	{
		nSc   = 3;
		dSs   = 0.75;
		dS1   = 0.3;
		dFa   = 1.2;
		dFv   = 1.8;
		dSds  = 0.6;
		dSd1  = 0.36;
		dCoef = 1.0;
		dIe   = 1.0;
		dTL   = 4.0;

		dMaxPeriod = 6.0;
	}
};

struct T_SPFC_AASHTO_LRFD12
{
	int nSiteClass;
	double dPGA;
	double dSs;
	double dS1;
	double dR;
	double dMaxPeriod;
	void Initialize()
	{
		nSiteClass   = 0;
		dPGA   = 0.1;
		dSs   = 0.25;
		dS1   = 0.1;
		dR    = 1.0;

		dMaxPeriod = 6.0;
	}
};

struct T_SPFC_NBC95    // NBC1995
{
	double dV;      // Zonal Velocity Ratio(V)
	int nZa;        // Acceleration Zone(Za)
	int nZv;        // Velocity Zone(Zv)
	double dMaxPeriod;    // Max. Period

	void Initialize()
	{
		dV   = 0.05;
		nZa   = 1;
		nZv   = 1;

		dMaxPeriod      = 6.0;
	}
};

// Add by GAY. PMS:4267. ('11.10.11). 중국 신하중기준 추가.
struct T_SPFC_CH2010_835   // China(GB50011-10)
{
	int nSeisDgnCategory;    // 0:1, 1:2, 2:3
	int nSeisFortification;  // 0:0.05g, 1:0.10g, 2:0.15g, 3:0.20g, 4:0.30g, 5:0.40g
	int nSiteClass;          // 0:I0, 1:I1, 2:II, 3:III, 4:IV
	int nEarthResponse;      // 0:Frequent, 1:Mid, 2:Scarce
	double dTg;              // Design Characteristic Period of Ground Motion.
	double dDamping;         // Damping Ratio.
	int nLForce;                         // 0:층전단력차, 1:관성력
};

// Add by GAY. PMS:4267. ('11.10.11). 중국 신하중기준 추가.
struct T_SPFC_CH2010  // China(GB50011-10)
{
	int     nSeisDgnCategory; // 0:1, 1:2, 2:3
	int     nSeisFortification; // 0:0.05g, 1:0.10g, 2:0.15g, 3:0.20g, 4:0.30g, 5:0.40g
	int     nSiteClass; // 0:I0, 1:I1, 2:II, 3:III, 4:IV
	int     nEarthResponse; // 0:Frequent, 1:Mid, 2:Scarce
	double  dTg; // Design Characteristic Period of Ground Motion.
	double  dDamping; // Damping Ratio.
	int     nLForce;    // 0:층전단력차, 1:관성력
	double  dMaxEQE; // Maximum Effect Coefficient
	double dMaxPeriod; // 9.5.5

	void Initialize()
	{
		nSeisDgnCategory   = 0;
		nSeisFortification = 0;
		nSiteClass         = 0;
		nEarthResponse     = 0;
		dTg      = 0.2;
		dDamping = 0.05;
		nLForce = 0;
		dMaxEQE = 0.04;
		dMaxPeriod = 6.0;
		CalcTg();
	}
	void CalcTg()
	{
		dTg = GetTg(nSeisDgnCategory, nSiteClass, nEarthResponse);
	}
	double GetTg(int nSeisDgnCategory, int nSiteClass, int nEarthResponse)
	{
		double dTg=0.;
		switch ( nSeisDgnCategory )
		{
		case 0: // 제1조.
			if ( nSiteClass == 0 ) dTg = 0.20; // I0.
			else if ( nSiteClass == 1 ) dTg = 0.25; // I1.
			else if ( nSiteClass == 2 ) dTg = 0.35; // II.
			else if ( nSiteClass == 3 ) dTg = 0.45; // III.
			else if ( nSiteClass == 4 ) dTg = 0.65; // IV.
			else ASSERT(0);
			break;
		case 1: // 제1조.
			if ( nSiteClass == 0 ) dTg = 0.25; // I0.
			else if ( nSiteClass == 1 ) dTg = 0.30; // I1.
			else if ( nSiteClass == 2 ) dTg = 0.40; // II.
			else if ( nSiteClass == 3 ) dTg = 0.55; // III.
			else if ( nSiteClass == 4 ) dTg = 0.75; // IV.
			else ASSERT(0);
			break;
		case 2: // 제1조.
			if ( nSiteClass == 0 ) dTg = 0.30; // I0.
			else if ( nSiteClass == 1 ) dTg = 0.35; // I1.
			else if ( nSiteClass == 2 ) dTg = 0.45; // II.
			else if ( nSiteClass == 3 ) dTg = 0.65; // III.
			else if ( nSiteClass == 4 ) dTg = 0.90; // IV.
			else ASSERT(0);
			break;
		}

		// 한우지진일때 특정 주기 Tg는 0.05sec 증가
		if ( nEarthResponse==2 ) dTg = dTg + 0.05;

		return dTg;
	}
};


struct T_SPFC_CH_GB_T_51408_2021
{
	int     nSeisDgnCategory; // 0:1, 1:2, 2:3
	int     nSeisFortification; // 0:0.05g, 1:0.10g, 2:0.15g, 3:0.20g, 4:0.30g, 5:0.40g
	int     nSiteClass; // 0:I0, 1:I1, 2:II, 3:III, 4:IV
	int     nEarthResponse; // 0:Mid, 1:Scarce, 2:Very Scarce
	double  dTg; // Design Characteristic Period of Ground Motion.
	double  dDamping; // Damping Ratio.
	int     nLForce;    // 0:층전단력차, 1:관성력
	double  dMaxEQE; // Maximum Effect Coefficient
	double dMaxPeriod; // 9.5.5
	void Initialize()
	{
		nSeisDgnCategory = 0;
		nSeisFortification = 0;
		nSiteClass = 0;
		nEarthResponse = 0;
		dTg = 0.2;
		dDamping = 0.05;
		nLForce = 0;
		dMaxEQE = 0.12;
		dMaxPeriod = 6.0;
		CalcTg();
	}
	void CalcTg()
	{
		dTg = GetTg(nSeisDgnCategory, nSiteClass, nEarthResponse);
	}
	double GetTg(int nSeisDgnCategory, int nSiteClass, int nEarthResponse)
	{
		double dTg = 0.;
		switch (nSeisDgnCategory)
		{
		case 0: // 제1조.
			if (nSiteClass == 0) dTg = 0.20; // I0.
			else if (nSiteClass == 1) dTg = 0.25; // I1.
			else if (nSiteClass == 2) dTg = 0.35; // II.
			else if (nSiteClass == 3) dTg = 0.45; // III.
			else if (nSiteClass == 4) dTg = 0.65; // IV.
			else ASSERT(0);
			break;
		case 1: // 제1조.
			if (nSiteClass == 0) dTg = 0.25; // I0.
			else if (nSiteClass == 1) dTg = 0.30; // I1.
			else if (nSiteClass == 2) dTg = 0.40; // II.
			else if (nSiteClass == 3) dTg = 0.55; // III.
			else if (nSiteClass == 4) dTg = 0.75; // IV.
			else ASSERT(0);
			break;
		case 2: // 제1조.
			if (nSiteClass == 0) dTg = 0.30; // I0.
			else if (nSiteClass == 1) dTg = 0.35; // I1.
			else if (nSiteClass == 2) dTg = 0.45; // II.
			else if (nSiteClass == 3) dTg = 0.65; // III.
			else if (nSiteClass == 4) dTg = 0.90; // IV.
			else ASSERT(0);
			break;
		}
		if (nEarthResponse == 1) dTg = dTg + 0.05;
		else if (nEarthResponse == 2) dTg = dTg + 0.1;

		return dTg;
	}
};

struct T_SPFC_CH2019
{
	int     nSeisDgnCategory; // 0:1, 1:2, 2:3
	int     nSeisFortification; // 0:0.05g, 1:0.10g, 2:0.15g, 3:0.20g, 4:0.30g, 5:0.40g
	int     nSiteClass; // 0:I0, 1:I1, 2:II, 3:III, 4:IV
	int     nEarthResponse; // 0:Frequent, 1:Mid, 2:Scarce
	double  dTg; // Design Characteristic Period of Ground Motion.
	double  dDamping; // Damping Ratio.
	int     nLForce;    //
	double  dMaxEQE; // Maximum Effect Coefficient
	double dMaxPeriod; // 9.5.5
	void Initialize()
	{
		nSeisDgnCategory   = 0;
		nSeisFortification = 0;
		nSiteClass         = 0;
		nEarthResponse     = 0;
		dTg      = 0.2;
		dDamping = 0.05;
		nLForce = 0;
		dMaxEQE = 0.05;
		dMaxPeriod = 6.0;
		CalcTg();
	}
	void CalcTg()
	{
		dTg = GetTg(nSeisDgnCategory, nSiteClass, nEarthResponse);
	}
	double GetTg(int nSeisDgnCategory, int nSiteClass, int nEarthResponse)
	{
		double dTg=0.;
		switch ( nSeisDgnCategory )
		{
		case 0: // 제1조.
			if ( nSiteClass == 0 ) dTg = 0.20; // I0.
			else if ( nSiteClass == 1 ) dTg = 0.25; // I1.
			else if ( nSiteClass == 2 ) dTg = 0.35; // II.
			else if ( nSiteClass == 3 ) dTg = 0.45; // III.
			else if ( nSiteClass == 4 ) dTg = 0.65; // IV.
			else ASSERT(0);
			break;
		case 1: // 제1조.
			if ( nSiteClass == 0 ) dTg = 0.25; // I0.
			else if ( nSiteClass == 1 ) dTg = 0.30; // I1.
			else if ( nSiteClass == 2 ) dTg = 0.40; // II.
			else if ( nSiteClass == 3 ) dTg = 0.55; // III.
			else if ( nSiteClass == 4 ) dTg = 0.75; // IV.
			else ASSERT(0);
			break;
		case 2: // 제1조.
			if ( nSiteClass == 0 ) dTg = 0.30; // I0.
			else if ( nSiteClass == 1 ) dTg = 0.35; // I1.
			else if ( nSiteClass == 2 ) dTg = 0.45; // II.
			else if ( nSiteClass == 3 ) dTg = 0.65; // III.
			else if ( nSiteClass == 4 ) dTg = 0.90; // IV.
			else ASSERT(0);
			break;
		}

		// 한우지진일때 특정 주기 Tg는 0.05sec 증가
		if ( nEarthResponse==2 ) dTg = dTg + 0.05;

		return dTg;
	}
};

// Add by Maxiao 2012-2-28
struct T_SPFC_CJJ166_2011  // China(CJJ166_2011)
{
	int nSeisType;          // 0:1, 1:2, 2:3
	int nZoningMap;          // 0:0.35, 1:0.4, 2:0.45
	int nSiteClass;          // 0:I0, 1:I1, 2:II, 3:III, 4:IV
	int nSeisFortification;  // 0:0.05g, 1:0.10g, 2:0.15g, 3:0.20g, 4:0.30g, 5:0.40g
	int nSeisE;              // 0:E1, 1:E2
	double dDamping;         // Damping Ratio.
	BOOL bVertAccSpec;       // VertAccSpec
	BOOL bE2_005;            // Think E2 0.05
	// Output Data
	double dTg;
	double dEta1;
	double dEta2;
	double dGamma;
	double dCi;
	double dA;
	double dSMax;
	double dMaxPeriod;

	void Initialize()
	{
		nSeisType          = 0;
		nZoningMap         = 0;
		nSiteClass         = 0;
		nSeisFortification = 0;
		nSeisE             = 0;

		bE2_005            = FALSE;
		bVertAccSpec       = FALSE;

		dDamping = 0.05;
		dTg      = 0.25;
		dEta1 = 0.02;
		dEta2 = 1.0;
		dGamma = 0.9;
		dCi    = 0.61;
		dA = 0.05;
		dSMax = 0.13;
		dMaxPeriod = 6.0;
	}
};

struct T_SPFC_JPN2000 // Japan(Arch.2000)
{
	int nZ;         //Seismic Zone Factor(Z)
	int nSoilClass; // Soil Class
	double dCo;     // Base Shear Factor(Co)
	double dMaxPeriod;
	void Initialize()
	{
		nZ         = 0;
		nSoilClass = 0;
		dCo        = 0.2;
		dMaxPeriod = 6.0;
	}
};

struct T_SPFC_JP_BRG2002 // Japan(Bridge2002)
{
	int nMethod;         // Earthquake Input Method
	int nSeisZone;       // Seismic Zone
	int nSoilClass;      // Soil Class
	double dCz;          // Modification Factor(Cz)
	double dDamping;     // Damping Ratio
	double dMaxPeriod;
	void Initialize()
	{
		nMethod    = 0;
		nSeisZone  = 0;
		nSoilClass = 0;
		dCz        = 1.0;
		dDamping   = 0.05;
		dMaxPeriod = 6.0;
	}
};

struct T_SPFC_IS1893_2016 // IS1893(2002)
{
	int nSeisZone;         // Seismic Zone
	int nSoilType;         // Soil Class
	double dDamping;       // Damping(%)
	double dDampingFactor; // Damping Multiplying Factor
	double dIe;            // Importance Factor(I)
	double dCoef;          // Response reduction Factor(R)
	double dMaxPeriod;
	void Initialize()
	{
		nSeisZone      = 0;
		nSoilType      = 0;
		dDamping       = 5.0;
		dDampingFactor = 1.0;
		dIe            = 1.0;
		dCoef          = 1.0;
		dMaxPeriod = 6.0;
	}
};

struct T_SPFC_IRCSP114_2018 // IS1893(2002)
{
	int nSeisZone;         // Seismic Zone
	int nSoilType;         // Soil Class
	double dDamping;       // Damping(%)
	double dDampingFactor; // Damping Multiplying Factor
	double dIe;            // Importance Factor(I)
	double dCoef;          // Response reduction Factor(R)
	double dUsrDefSeismicZone; // User Defined Seismic Zone for nSeisZone = 4
	double dMaxPeriod;
	void Initialize()
	{
		nSeisZone      = 0;
		nSoilType      = 0;
		dDamping       = 5.0;
		dDampingFactor = 1.0;
		dIe            = 1.0;
		dCoef          = 1.0;
		dUsrDefSeismicZone = 0.;
		dMaxPeriod = 6.0;
	}
};

struct T_SPFC_KSCE2019
{
	int nSeisZone;      // Seismic Zone (Z)
	int nSiteClass;     // Site Class(S1~S6)
	double dIe;         // Risk Factor (I )
	double dMaxPeriod;  // Max. Period
	double dFa;
	double dFv;

	void Initialize()
	{
		nSeisZone   = 0;
		nSiteClass  = 0;
		dIe         = 1.0;
		dMaxPeriod  = 6.0;
		dFa         = 0.0;
		dFv         = 0.0;
	}
	void POInitialize()
	{
		nSeisZone   = 0;
		nSiteClass  = 0;
		dIe         = 1.0;
		dMaxPeriod  = 6.0;
		dFa         = 0.0;
		dFv         = 0.0;
	}
};

struct T_SPFC_NTC2018    // NTC2018
{
	int nSpectrumType;    // 0:Horizontal Elastic, 1:Vertical Elastic, 2:Horizontal Design, 3:Vertical Design(Recommended), 10:Horizontal Elastic, 11:Horizontal Design (Singapore)
	int nGroundType;      // 0:A, 1:B, 2:C, 3:D, 4:E
	int nParameterType;   // 0:T1, 1:T2, 2:T3, 3:T4, 4:User Defined
	double dSoilFactor;   // SoilFactor
	double dTb;           // Tb
	double dTc;           // Tc
	double dTd;           // Td
	double dAg;                     // Maximum Horizontal Acc. (Ag)
	double dFo;           // Amplification Factor (Fo)
	double dTcStar;             // Period of constant Hor. Acc. (Tc*)
	double dXi;           // Viscous Damping Ratio (Xi)
	double dQ;            // Behavior Factor (q)
	double dMaxPeriod;    // Max. Period

	void Initialize()
	{
		nSpectrumType   = 0;
		nGroundType     = 1;
		nParameterType  = 0;
		dSoilFactor     = 1.2;
		dTb             = 0.15;
		dTc             = 0.5;
		dTd             = 2.0;
		dAg                         = 0.08;
		dFo                         = 2.5;
		dTcStar                 = 0.3;
		dXi             = 5.0;
		dQ              = 1.5;
		dMaxPeriod      = 2.5;
	}
	void POInitialize()
	{
		nSpectrumType   = 0;
		nGroundType     = 1;
		nParameterType  = 0;
		dSoilFactor     = 1.2;
		dTb             = 0.15;
		dTc             = 0.5;
		dTd             = 2.0;
		dAg                         = 0.08;
		dFo                         = 2.5;
		dTcStar                 = 0.3;
		dXi             = 5.0;
		dQ              = 1.0;
		dMaxPeriod      = 2.5;
	}
};

struct T_SPFC_DPWH_LRFD_2013
{
	int nGroundType;
	double dPGA;
	double dSs;
	double dS1;
	double dR;
	double dMaxPeriod;
	void Initialize()
	{
		nGroundType = 0;
		dPGA = 0.5;
		dSs  = 1.1;
		dS1  = 0.6;
		dR   = 1.0;

		dMaxPeriod = 6.0;
	}
};

struct T_SPFC_KDS2019
{
	int nSeisZone;      // Seismic Zone
	int nSiteClass;     // Site Class(S1, S2, S3, S4, S5, S6)

	double dZoneFactor; // Zone Factor(S)
	double dFa;         // Fa
	double dFv;         // Fv
	double dSds;        // Sds
	double dSd1;        // Sd1
	double dIe;         // Importance Factor (Ie)
	double dCoef;       // Response Modification Coef. (R)
	double dMaxPeriod;  // Max. Period
	void Initialize()
	{
		nSeisZone   = 0;
		nSiteClass  = 1;
		dZoneFactor = 0.22;
		dFa         = 1.38;
		dFv         = 1.38;
		dSds        = 0.506;
		dSd1        = 0.2024;
		dIe         = 1.2;
		dCoef       = 4.0;
		dMaxPeriod  = 6.0;
	}
	void POInitialize()
	{
		nSeisZone   = 0;
		nSiteClass  = 1;
		dZoneFactor = 0.22;
		dFa         = 1.38;
		dFv         = 1.38;
		dSds        = 0.506;
		dSd1        = 0.2024;
		dIe         = 1.2;
		dCoef       = 4.0;
		dMaxPeriod  = 6.0;
	}
};

struct T_SPFC_AS_5100_2_2017
{
	int nSoilClass;
	double dkp;
	double dZ;
	double dMu;
	double dMaxPeriod;
	void Initialize()
	{
		nSoilClass = 0;
		dkp = 1.3;
		dZ  = 0.08;
		dMu = 1.5;

		dMaxPeriod = 6.0;
	}
};

enum D_SPFC_P100_2013_SP
{
	D_SPFC_P100_2013_SP_HE, // Horizontal Elastic
	D_SPFC_P100_2013_SP_VE, // Vertical Elastic
	D_SPFC_P100_2013_SP_HD, // Horizontal Design
	D_SPFC_P100_2013_SP_VD  // Vertical Design
};

enum D_SPFC_P100_2013_TC
{
	D_SPFC_P100_2013_TC07S, // Tc=0.7s
	D_SPFC_P100_2013_TC10S, // Tc=1.0s
	D_SPFC_P100_2013_TC16S, // Tc=1.6s
	D_SPFC_P100_2013_USERD  // User Defined
};

struct T_SPFC_P100_2013 // P100-1(2013)
{
	int nSpectrumType;    // D_SPFC_P100_2013_SP
	int nParameterType;   // D_SPFC_P100_2013_TC
	double dTb;           // Tb
	double dTc;           // Tc
	double dTd;           // Td
	double dAgr;          // Design Ground Acc. (AgR)
	double dI;            // Importance Factor(I)
	double dXi;           // Viscous Damping Ratio(Xi)
	double dQ;            // Behavior Factor(q)
	double dMaxPeriod;    // Max. Period

	void Initialize()
	{
		nSpectrumType   = D_SPFC_P100_2013_SP_HE;
		nParameterType  = D_SPFC_P100_2013_TC07S;
		dTb             = 0.15;
		dTc             = 0.5;
		dTd             = 2.0;
		dAgr            = 0.08;
		dI              = 1.0;
		dXi             = 5.0;
		dQ              = 1.5;
		dMaxPeriod      = 6.0;
	}
};

struct T_SPFC_KS // KS(Korea(Arch.1992))
{
	double dSoil;       // Soil Profile Type
	double dEPA;        // Earthquake Area
	double dIe;         // Importance Factor (Ie)
	double dCoef;       // Response Modification Coef. (R)
	double dMaxPeriod;  // Max. Period
	void Initialize()
	{
		dSoil       = 1.0;
		dEPA        = 0.08;
		dIe         = 1.0;
		dCoef       = 1.0;
		dMaxPeriod  = 6.0;
	}
};

struct T_SPFC_KS2000 // KS(Korea(Arch.2000))
{
	double dSoil;       // Soil Profile Type
	double dEPA;        // Earthquake Area
	double dIe;         // Importance Factor (Ie)
	double dCoef;       // Response Modification Coef. (R)
	double dMaxPeriod;  // Max. Period
	void Initialize()
	{
		dSoil       = 1.0;
		dEPA        = 0.11;
		dIe         = 1.0;
		dCoef       = 1.0;
		dMaxPeriod  = 6.0;
	}
};

struct T_SPFC_KS_BRG // KS(Korea(Bridge))
{
	double dSoil;       // Soil Profile Type
	double dEPA;        // Earthquake Area
	double dIe;         // Importance Factor (Ie)
	double dCoef;       // Response Modification Coef. (R)
	double dMaxPeriod;  // Max. Period
	void Initialize()
	{
		dSoil       = 1.0;
		dEPA        = 0.11;
		dIe         = 1.0;
		dCoef       = 1.0;
		dMaxPeriod  = 6.0;
	}
};

struct T_SPFC_UBC88   // UBC 88-94
{
	double dSoil;       // Soil Profile Type
	double dSeis;       // Seismic Zone Factor
	double dIe;         // Importance Factor (Ie)
	double dCoef;       // Response Modification Coef. (R)
	double dMaxPeriod;  // Max. Period
	void Initialize()
	{
		dSoil       = 1.0;
		dSeis       = 0.075;
		dIe         = 1.0;
		dCoef       = 1.0;
		dMaxPeriod  = 6.0;
	}
};

struct T_SPFC_UBC97   // UBC(1997)
{
	int nOption;        // Seismic Coefficients Calculation Option
	int nSoilType;      // Soil Profile Type
	int nSeisZone;      // Seismic Zone Factor(Z)
	int nSourceType;    // Seismic Source Type
	double dClosestDist;// Closest Distance to Known Seismic Source(km)
	double dCa;         // Seismic Coefficient(Ca)
	double dCv;         // Seismic Coefficient(Cv)
	double dIe;         // Importance Factor (Ie)
	double dCoef;       // Response Modification Coef. (R)
	double dMaxPeriod;  // Max. Period
	void Initialize()
	{
		nOption      = 0;
		nSoilType    = 0;
		nSeisZone    = 0;
		nSourceType  = 0;
		dClosestDist = 10.0;
		dCa          = 0.06;
		dCv          = 0.06;
		dIe          = 1.0;
		dCoef        = 1.0;
		dMaxPeriod   = 6.0;
	}
};

struct T_SPFC_KBC_2005 // KBC 2005
{
	int nAccMethod;     //
	int nSeisZone;      // Seismic Zone
	int nSiteClass;     // Site Class(Sa, Sb, Sc, Sd, Se)
	BOOL bRelieve;      //
	double dSds;        // Sds
	double dSd1;        // Sd1
	double dIe;         // Importance Factor (Ie)
	double dCoef;       // Response Modification Coef. (R)
	double dMaxPeriod;  // Max. Period
	void Initialize()
	{
		nAccMethod  = 0;
		nSeisZone   = 0;
		nSiteClass  = 3;
		bRelieve = FALSE;
		dSds        = 0.53533;
		dSd1        = 0.23173;
		dIe         = 1.0;
		dCoef       = 1.0;
		dMaxPeriod  = 6.0;
	}
	void POInitialize()
	{
		nAccMethod  = 0;
		nSeisZone   = 0;
		nSiteClass  = 3;
		bRelieve = FALSE;
		dSds        = 0.52668;
		dSd1        = 0.33649;
		dIe         = 1.0;
		dCoef       = 1.0;
		dMaxPeriod  = 6.0;
	}
};

struct T_SPFC_KBC_2009 // KBC 2009
{
	int nSeisZone;      // Seismic Zone
	int nSiteClass;     // Site Class(Sa, Sb, Sc, Sd, Se)

	double dZoneFactor; // Zone Factor(S)
	double dFa;         // Fa
	double dFv;         // Fv
	double dSds;        // Sds
	double dSd1;        // Sd1
	double dIe;         // Importance Factor (Ie)
	double dCoef;       // Response Modification Coef. (R)
	double dMaxPeriod;  // Max. Period
	void Initialize()
	{
		nSeisZone   = 0;
		nSiteClass  = 3;
		dZoneFactor = 0.22;
		dFa         = 1.46;
		dFv         = 1.58;
		dSds        = 0.53533;
		dSd1        = 0.23173;
		dIe         = 1.0;
		dCoef       = 1.0;
		dMaxPeriod  = 6.0;
	}
	void POInitialize()
	{
		nSeisZone   = 0;
		nSiteClass  = 3;
		dZoneFactor = 0.22;
		dFa         = 1.36;
		dFv         = 1.96;
		dSds        = 0.52668;
		dSd1        = 0.33649;
		dIe         = 1.0;
		dCoef       = 1.0;
		dMaxPeriod  = 6.0;
	}
};

struct T_SPFC_KBC_2016 // KBC 2016
{
	int nSeisZone;      // Seismic Zone
	int nSiteClass;     // Site Class(Sa, Sb, Sc, Sd, Se)

	double dZoneFactor; // Zone Factor(S)
	double dDepthMR;    // Depth to MR
	double dFa;         // Fa
	double dFv;         // Fv
	double dSds;        // Sds
	double dSd1;        // Sd1
	double dIe;         // Importance Factor (Ie)
	double dCoef;       // Response Modification Coef. (R)
	double dMaxPeriod;  // Max. Period
	void Initialize()
	{
		nSeisZone   = 0;
		nSiteClass  = 3;
		dZoneFactor = 0.22;
		dDepthMR    = 0.0;
		dFa         = 1.46;
		dFv         = 1.58;
		dSds        = 0.53533;
		dSd1        = 0.23173;
		dIe         = 1.0;
		dCoef       = 1.0;
		dMaxPeriod  = 6.0;
	}
	void POInitialize()
	{
		nSeisZone   = 0;
		nSiteClass  = 3;
		dZoneFactor = 0.22;
		dDepthMR    = 0.0;
		dFa         = 1.46;
		dFv         = 1.58;
		dSds        = 0.53533;
		dSd1        = 0.23173;
		dIe         = 1.0;
		dCoef       = 1.0;
		dMaxPeriod  = 6.0;
	}
};

struct T_SPFC_NSR_10  // NSR-10
{
	int nSiteClass;     // Site Class(A, B, C, D, E)
	double dAa;         // Effective Peak Acc.
	double dAv;         // Effective Peak Vel.
	double dFa;         // Site Coeff. at Short Period(Fa)
	double dFv;         // Site Coeff. at 1sec Period(Fv)
	double dIe;         // Importance Factor (Ie)
	double dCoef;       // Basic Ductility Factor. (R0)
	double dPhi;        // Phi
	double dMaxPeriod;  // Max. Period
	void Initialize()
	{
		nSiteClass  = 3;
		dAa         = 0.15;
		dAv         = 0.15;
		dFa         = 1.5;
		dFv         = 2.0;
		dIe         = 1.0;
		dCoef       = 1.0;
		dPhi        = 1.0;
		dMaxPeriod  = 6.0;
	}
};

struct T_SPFC_DPT2018 
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
	double             dResModFactor;       // Response Modification Factor
	double             dImportFactor;       // 1.0, 1.25, 1.5
	double             dDampingRatio;
	double             dMaxPeriod;

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
		dResModFactor       = 4.0;
		dImportFactor       = 1.0;
		dDampingRatio       = 0.025;
		dMaxPeriod          = 6.0;
	}
};

struct T_SPFC_RUS_SP2016
{
	int     nRegion;    // 0:7, 1:8,  2:9,  3:10    - Region Seismicity
	int     nSoil;      // 0:I, 1:II, 2:III         - Soil Category

	double dK1;
	double dK2;
	double dK3;
	double dK4;
	double dKPsi;

	double dMaxPeriod;  // Max. Period
	void Initialize()
	{
		nRegion = 0;
		nSoil   = 0;
		dK1 = 0.5;
		dK2 = 1.0;
		dK3 = 1.0;
		dK4 = 1.0;
		dKPsi = 1.0;

		dMaxPeriod = 6.0;
	}
};

struct T_SPFC_RUS_SP14_2018
{
	int  nSoil;
	BOOL bNLDS; // Nonlinear deformation of soils
	double dK0;
	double dK1;
	double dKPsi;
	double dAcce;
	double dMaxPeriod;  // Max. Period

	void Initialize()
	{
		nSoil = 0;
		bNLDS = false;
		dK0 = 1.00;
		dK1 = 0.12;
		dKPsi = 1.30;
		dAcce = 1.00;
		dMaxPeriod = 6.0;
	}
};


// Pinakin 9.5.5
struct T_SPFC_SANS2010    // SANS10160_4_2010:
{
	int nSpectrumType;    // 0:Horizontal Elastic, 1:Vertical Elastic, 2:Horizontal Design, 3:Vertical Design(Recommended), 10:Horizontal Elastic, 11:Horizontal Design (Singapore)
	int nGroundType;      // (Recommended)  0:A, 1:B, 2:C, 3:D, 4:E, 5:S1, 6:S2
						  // (Singapore)   10:C, 11:D, 12:S1
						  // (Malaysia)    13:Rock(R), 14:Stiff Soil(SS), 15:Flexible Soil(FS)
	int nParameterType;   // 0:Type1, 1:Type2, 2:User Defined
	double dSoilFactor;   // SoilFactor
	double dTb;           // Tb
	double dTc;           // Tc
	double dTd;           // Td
	double dAgr;          // Design Ground Acc. (AgR)
	double dI;            // Importance Factor(I)
	double dXi;           // Viscous Damping Ratio(Xi)
	double dQ;            // Behavior Factor(q)
	double dB;            // Lower Bound Factor(h)
	double dMaxPeriod;    // Max. Period
	double dTs;           // (Malaysia) Site Natural Period
	int nResionType;      // (Malaysia) 0:Peninsular  1:Sabah, 1:Sarawak  
	EN_SPFC_NA nNationalAnnex;
	void Initialize()
	{
		nSpectrumType = 0;
		nGroundType = EN_SPFC_GROUND_B;
		nParameterType = 0;
		dSoilFactor = 1.0;
		dTb = 0.15;
		dTc = 0.4;
		dTd = 2.0;
		dAgr = 0.1;
		dI = 1.0;
		dXi = 5.0;
		dQ = 1.0;
		dB = 0.2;
		dMaxPeriod = 6.0;
		dTs = 0.5;
		nResionType = 0;
		nNationalAnnex = EN_SPFC_RECOMMENDED;
	}
	void POInitialize()
	{
		nSpectrumType = 0;
		nGroundType = EN_SPFC_GROUND_B;
		nParameterType = 0;
		dSoilFactor = 1.0;
		dTb = 0.15;
		dTc = 0.4;
		dTd = 2.0;
		dAgr = 0.1;
		dI = 1.0;
		dXi = 5.0;
		dQ = 1.0;
		dB = 0.2;
		dMaxPeriod = 6.0;
		dTs = 0.5;
		nResionType = 0;
		nNationalAnnex = EN_SPFC_RECOMMENDED;
	}
};


enum EN_IS1893_2025_SPEC_TYPE
{
	EN_IS2025_SPEC_HOR = 0,
	EN_IS2025_SPEC_VER,
};

enum EN_IS1893_2025_EQ_ZONE
{
	EN_IS2025_EQ_ZONE_II = 0,
	EN_IS2025_EQ_ZONE_III,
	EN_IS2025_EQ_ZONE_IV,
	EN_IS2025_EQ_ZONE_V,
	EN_IS2025_EQ_ZONE_VI,
	EN_IS2025_EQ_ZONE_COUNT,
};

enum EN_IS1893_2025_RETURN_PERIOD
{
	EN_IS2025_RET_75 = 0,
	EN_IS2025_RET_175,
	EN_IS2025_RET_275,
	EN_IS2025_RET_475,
	EN_IS2025_RET_975,
	EN_IS2025_RET_1275,
	EN_IS2025_RET_2475,
	EN_IS2025_RET_4975,
	EN_IS2025_RET_9975,
	EN_IS2025_RET_COUNT,
};

enum EN_IS1893_2025_SITE_CLASS
{
	EN_IS2025_SITE_A = 0,
	EN_IS2025_SITE_B,
	EN_IS2025_SITE_C,
	EN_IS2025_SITE_D,
	EN_IS2025_SITE_COUNT,
};

struct T_SPFC_IS1893_2025 // IS1893(2025)
{
	int nSpecType;		   // Spectrum Type
	int nEqZone;           // Seismic Zone
	int nReturnPeriod;     // Soil Class
	double dZoneFactor;    // EQ Zone factor
	int nSiteClass;		   // Site class
	double dDamping;       // Damping(%)
	double dImpFactor;	   // Importance Factor(I)
	double dRespRedFact;   // Response reduction Factor(R)
	double dMaxPeriod;
	void Initialize()
	{
		nSpecType = EN_IS2025_SPEC_HOR;
		nEqZone = EN_IS2025_EQ_ZONE_II;
		nReturnPeriod = EN_IS2025_RET_75;
		dZoneFactor = 0.0375;
		nSiteClass = EN_IS2025_SITE_A;
		dDamping = 5.0;
		dImpFactor = 1.0;
		dRespRedFact = 5.0;
		dMaxPeriod = 10.0;
	}
};

union T_SPFC_CODE_770         // 7.1.0
{
	T_SPFC_CH2002_835      CH2002;
	T_SPFC_CHSH2003_835    CHSH2003;
	T_SPFC_TAIWAN06_871    TAIWAN06;
	T_SPFC_GB50111_2006    GB50111_2006; // 7.3.0
	T_SPFC_JTJXXX_2008_770 JTJXXX_2008;  // 7.6.0
	T_SPFC_EURO2004        EURO2004; //770
};

union T_SPFC_CODE_835
{
	T_SPFC_CH2002_835   CH2002;
	T_SPFC_CHSH2003_835 CHSH2003;
	T_SPFC_TAIWAN06_871 TAIWAN06;
	T_SPFC_GB50111_2006 GB50111_2006; // 7.3.0
	T_SPFC_JTJXXX_2008  JTJXXX_2008;  // 7.6.0
	T_SPFC_EURO2004     EURO2004; //770
	T_SPFC_CH2010_835   CH2010;
	T_SPFC_CJJ166_2011  CJJ166_2011;
};

union T_SPFC_CODE_871
{
	T_SPFC_CH2002       CH2002;
	T_SPFC_CHSH2003     CHSH2003;
	T_SPFC_TAIWAN06_871 TAIWAN06;     // 8.7.1
	T_SPFC_GB50111_2006 GB50111_2006;
	T_SPFC_JTJXXX_2008  JTJXXX_2008;
	T_SPFC_EURO2004     EURO2004;
	T_SPFC_CH2010       CH2010;
	T_SPFC_CJJ166_2011  CJJ166_2011;
	T_SPFC_P100_2013    P100_2013;
};

union T_SPFC_CODE_881
{
	T_SPFC_CH_BRG89     CHBRG89;
	T_SPFC_CH_GBJ111_87 CHGBJ111_87;
	T_SPFC_CH2002       CH2002;
	T_SPFC_CHSH2003     CHSH2003;
	T_SPFC_TAIWAN89     TAIWAN99V;
	T_SPFC_TAIWAN89     TAIWAN99H;
	T_SPFC_TAIWAN89     TAIWAN89V;
	T_SPFC_TAIWAN89     TAIWAN89H;
	T_SPFC_TAIWAN06     TAIWAN06;
	T_SPFC_TAIWAN06     TAIWANBRG98;
	T_SPFC_GB50111_2006 GB50111_2006;
	T_SPFC_JTJXXX_2008  JTJXXX_2008;
	T_SPFC_EURO1996     EURO1996;
	T_SPFC_EURO1996_ELA EURO1996_ELA;
	T_SPFC_EURO2004     EURO2004;
	T_SPFC_CH2010       CH2010;
	T_SPFC_CJJ166_2011  CJJ166_2011;
	T_SPFC_P100_2013    P100_2013;
	T_SPFC_JPN2000      JP2000;
	T_SPFC_JP_BRG2002   JPBRG2002;
	T_SPFC_IS1893_2016  IS1893;
	T_SPFC_KS           KS;
	T_SPFC_KS2000       KS2000;
	T_SPFC_KS_BRG       KSBRG;
	T_SPFC_UBC88        UBC88;
	T_SPFC_UBC97        UBC97;
	T_SPFC_KBC_2005     KBC2005;
	T_SPFC_KBC_2009     KBC2009;
	T_SPFC_KBC_2016     KBC2016;
	T_SPFC_IBC2000      IBC2000;
	T_SPFC_IBC2012      IBC2009;
	T_SPFC_IBC2012      IBC2012;
	T_SPFC_AASHTO_LRFD12 AASHTO_LRFD12;
	T_SPFC_NBC95        NBC95;
	T_SPFC_NSR_10       NSR10;
	T_SPFC_IS1893_2016  IS2016;
};

union T_SPFC_CODE_883
{
	T_SPFC_CH_BRG89       CHBRG89;
	T_SPFC_CH_GBJ111_87   CHGBJ111_87;
	T_SPFC_CH2002         CH2002;
	T_SPFC_CHSH2003       CHSH2003;
	T_SPFC_TAIWAN89       TAIWAN99V;
	T_SPFC_TAIWAN89       TAIWAN99H;
	T_SPFC_TAIWAN89       TAIWAN89V;
	T_SPFC_TAIWAN89       TAIWAN89H;
	T_SPFC_TAIWAN06       TAIWAN06;
	T_SPFC_TAIWAN06       TAIWANBRG98;
	T_SPFC_GB50111_2006   GB50111_2006;
	T_SPFC_JTJXXX_2008    JTJXXX_2008;
	T_SPFC_EURO1996       EURO1996;
	T_SPFC_EURO1996_ELA   EURO1996_ELA;
	T_SPFC_EURO2004       EURO2004;
	T_SPFC_CH2010         CH2010;
	T_SPFC_CJJ166_2011    CJJ166_2011;
	T_SPFC_P100_2013      P100_2013;
	T_SPFC_JPN2000        JP2000;
	T_SPFC_JP_BRG2002     JPBRG2002;
	T_SPFC_IS1893_2016    IS1893;
	T_SPFC_KS             KS;
	T_SPFC_KS2000         KS2000;
	T_SPFC_KS_BRG         KSBRG;
	T_SPFC_UBC88          UBC88;
	T_SPFC_UBC97          UBC97;
	T_SPFC_KBC_2005       KBC2005;
	T_SPFC_KBC_2009       KBC2009;
	T_SPFC_KBC_2016       KBC2016;
	T_SPFC_IBC2000        IBC2000;
	T_SPFC_IBC2012        IBC2009;
	T_SPFC_IBC2012        IBC2012;
	T_SPFC_AASHTO_LRFD12  AASHTO_LRFD12;
	T_SPFC_NBC95          NBC95;
	T_SPFC_NSR_10         NSR10;
	T_SPFC_IS1893_2016    IS2016;
	T_SPFC_KSCE2019       KSCE2019;
	T_SPFC_NTC2018        NTC2018;
	T_SPFC_DPWH_LRFD_2013 DPWHLRFD2013;
};

union T_SPFC_CODE_895                 // 8.9.5
{
	T_SPFC_CH_BRG89       CHBRG89;
	T_SPFC_CH_GBJ111_87   CHGBJ111_87;
	T_SPFC_CH2002         CH2002;
	T_SPFC_CHSH2003       CHSH2003;
	T_SPFC_TAIWAN89       TAIWAN99V;
	T_SPFC_TAIWAN89       TAIWAN99H;
	T_SPFC_TAIWAN89       TAIWAN89V;
	T_SPFC_TAIWAN89       TAIWAN89H;
	T_SPFC_TAIWAN06       TAIWAN06;
	T_SPFC_TAIWAN06       TAIWANBRG98;
	T_SPFC_GB50111_2006   GB50111_2006;
	T_SPFC_JTJXXX_2008    JTJXXX_2008;
	T_SPFC_EURO1996       EURO1996;
	T_SPFC_EURO1996_ELA   EURO1996_ELA;
	T_SPFC_EURO2004       EURO2004;
	T_SPFC_CH2010         CH2010;
	T_SPFC_CH2019         CH2019;
	T_SPFC_CJJ166_2011    CJJ166_2011;
	T_SPFC_P100_2013      P100_2013;
	T_SPFC_JPN2000        JP2000;
	T_SPFC_JP_BRG2002     JPBRG2002;
	T_SPFC_IS1893_2016    IS1893;
	T_SPFC_KS             KS;
	T_SPFC_KS2000         KS2000;
	T_SPFC_KS_BRG         KSBRG;
	T_SPFC_UBC88          UBC88;
	T_SPFC_UBC97          UBC97;
	T_SPFC_KBC_2005       KBC2005;
	T_SPFC_KBC_2009       KBC2009;
	T_SPFC_KBC_2016       KBC2016;
	T_SPFC_IBC2000        IBC2000;
	T_SPFC_IBC2012        IBC2009;
	T_SPFC_IBC2012        IBC2012;
	T_SPFC_AASHTO_LRFD12  AASHTO_LRFD12;
	T_SPFC_NBC95          NBC95;
	T_SPFC_NSR_10         NSR10;
	T_SPFC_IS1893_2016    IS2016;
	T_SPFC_KSCE2019       KSCE2019;
	T_SPFC_NTC2018        NTC2018;
	T_SPFC_DPWH_LRFD_2013 DPWHLRFD2013;
	T_SPFC_KDS2019        KDS2019;
	T_SPFC_AS_5100_2_2017 AS510022017;
	T_SPFC_JTG_T_2231_01_2020 JTG_T_2231_01_2020;
};

union T_SPFC_CODE_915
{
	T_SPFC_CH_BRG89       CHBRG89;
	T_SPFC_CH_GBJ111_87   CHGBJ111_87;
	T_SPFC_CH2002         CH2002;
	T_SPFC_CHSH2003       CHSH2003;
	T_SPFC_TAIWAN89       TAIWAN99V;
	T_SPFC_TAIWAN89       TAIWAN99H;
	T_SPFC_TAIWAN89       TAIWAN89V;
	T_SPFC_TAIWAN89       TAIWAN89H;
	T_SPFC_TAIWAN06       TAIWAN06;
	T_SPFC_TAIWAN06       TAIWANBRG98;
	T_SPFC_GB50111_2006   GB50111_2006;
	T_SPFC_JTJXXX_2008    JTJXXX_2008;
	T_SPFC_EURO1996       EURO1996;
	T_SPFC_EURO1996_ELA   EURO1996_ELA;
	T_SPFC_EURO2004       EURO2004;
	T_SPFC_CH2010         CH2010;
	T_SPFC_CH2019         CH2019;
	T_SPFC_CJJ166_2011    CJJ166_2011;
	T_SPFC_P100_2013      P100_2013;
	T_SPFC_JPN2000        JP2000;
	T_SPFC_JP_BRG2002     JPBRG2002;
	T_SPFC_IS1893_2016    IS1893;
	T_SPFC_KS             KS;
	T_SPFC_KS2000         KS2000;
	T_SPFC_KS_BRG         KSBRG;
	T_SPFC_UBC88          UBC88;
	T_SPFC_UBC97          UBC97;
	T_SPFC_KBC_2005       KBC2005;
	T_SPFC_KBC_2009       KBC2009;
	T_SPFC_KBC_2016       KBC2016;
	T_SPFC_IBC2000        IBC2000;
	T_SPFC_IBC2012        IBC2009;
	T_SPFC_IBC2012        IBC2012;
	T_SPFC_AASHTO_LRFD12  AASHTO_LRFD12;
	T_SPFC_NBC95          NBC95;
	T_SPFC_NSR_10         NSR10;
	T_SPFC_IS1893_2016    IS2016;
	T_SPFC_KSCE2019       KSCE2019;
	T_SPFC_NTC2018        NTC2018;
	T_SPFC_DPWH_LRFD_2013 DPWHLRFD2013;
	T_SPFC_KDS2019        KDS2019;
	T_SPFC_AS_5100_2_2017 AS510022017;
	T_SPFC_IRCSP114_2018  IRCSP114;
	T_SPFC_JTG_T_2231_01_2020 JTG_T_2231_01_2020;
	T_SPFC_JP_BRG2002     JPBRG2012;    // 905 추가
	T_SPFC_JP_BRG2002     JPBRG2017;    // 905 추가
};

union T_SPFC_CODE_920
{
	T_SPFC_CH_BRG89       CHBRG89;
	T_SPFC_CH_GBJ111_87   CHGBJ111_87;
	T_SPFC_CH2002         CH2002;
	T_SPFC_CHSH2003       CHSH2003;
	T_SPFC_TAIWAN89       TAIWAN99V;
	T_SPFC_TAIWAN89       TAIWAN99H;
	T_SPFC_TAIWAN89       TAIWAN89V;
	T_SPFC_TAIWAN89       TAIWAN89H;
	T_SPFC_TAIWAN06       TAIWAN06;
	T_SPFC_TAIWAN06       TAIWANBRG98;
	T_SPFC_GB50111_2006   GB50111_2006;
	T_SPFC_JTJXXX_2008    JTJXXX_2008;
	T_SPFC_EURO1996       EURO1996;
	T_SPFC_EURO1996_ELA   EURO1996_ELA;
	T_SPFC_EURO2004       EURO2004;
	T_SPFC_CH2010         CH2010;
	T_SPFC_CH2019         CH2019;
	T_SPFC_CJJ166_2011    CJJ166_2011;
	T_SPFC_P100_2013      P100_2013;
	T_SPFC_JPN2000        JP2000;
	T_SPFC_JP_BRG2002     JPBRG2002;
	T_SPFC_IS1893_2016    IS1893;
	T_SPFC_KS             KS;
	T_SPFC_KS2000         KS2000;
	T_SPFC_KS_BRG         KSBRG;
	T_SPFC_UBC88          UBC88;
	T_SPFC_UBC97          UBC97;
	T_SPFC_KBC_2005       KBC2005;
	T_SPFC_KBC_2009       KBC2009;
	T_SPFC_KBC_2016       KBC2016;
	T_SPFC_IBC2000        IBC2000;
	T_SPFC_IBC2012        IBC2009;
	T_SPFC_IBC2012        IBC2012;
	T_SPFC_AASHTO_LRFD12  AASHTO_LRFD12;
	T_SPFC_NBC95          NBC95;
	T_SPFC_NSR_10         NSR10;
	T_SPFC_IS1893_2016    IS2016;
	T_SPFC_KSCE2019       KSCE2019;
	T_SPFC_NTC2018        NTC2018;
	T_SPFC_DPWH_LRFD_2013 DPWHLRFD2013;
	T_SPFC_KDS2019        KDS2019;
	T_SPFC_AS_5100_2_2017 AS510022017;
	T_SPFC_IRCSP114_2018  IRCSP114;
	T_SPFC_JTG_T_2231_01_2020 JTG_T_2231_01_2020;
	T_SPFC_JP_BRG2002     JPBRG2012;    // 905 추가
	T_SPFC_JP_BRG2002     JPBRG2017;    // 905 추가
	T_SPFC_DPT2018        DPT2018;	//915
};

union T_SPFC_CODE_925
{
	T_SPFC_CH_BRG89       CHBRG89;
	T_SPFC_CH_GBJ111_87   CHGBJ111_87;
	T_SPFC_CH2002         CH2002;
	T_SPFC_CHSH2003       CHSH2003;
	T_SPFC_TAIWAN89       TAIWAN99V;
	T_SPFC_TAIWAN89       TAIWAN99H;
	T_SPFC_TAIWAN89       TAIWAN89V;
	T_SPFC_TAIWAN89       TAIWAN89H;
	T_SPFC_TAIWAN06       TAIWAN06;
	T_SPFC_TAIWAN06       TAIWANBRG98;
	T_SPFC_GB50111_2006   GB50111_2006;
	T_SPFC_JTJXXX_2008    JTJXXX_2008;
	T_SPFC_EURO1996       EURO1996;
	T_SPFC_EURO1996_ELA   EURO1996_ELA;
	T_SPFC_EURO2004       EURO2004;
	T_SPFC_CH2010         CH2010;
	T_SPFC_CH2019         CH2019;
	T_SPFC_CJJ166_2011    CJJ166_2011;
	T_SPFC_P100_2013      P100_2013;
	T_SPFC_JPN2000        JP2000;
	T_SPFC_JP_BRG2002     JPBRG2002;
	T_SPFC_IS1893_2016    IS1893;
	T_SPFC_KS             KS;
	T_SPFC_KS2000         KS2000;
	T_SPFC_KS_BRG         KSBRG;
	T_SPFC_UBC88          UBC88;
	T_SPFC_UBC97          UBC97;
	T_SPFC_KBC_2005       KBC2005;
	T_SPFC_KBC_2009       KBC2009;
	T_SPFC_KBC_2016       KBC2016;
	T_SPFC_IBC2000        IBC2000;
	T_SPFC_IBC2012        IBC2009;
	T_SPFC_IBC2012        IBC2012;
	T_SPFC_AASHTO_LRFD12  AASHTO_LRFD12;
	T_SPFC_NBC95          NBC95;
	T_SPFC_NSR_10         NSR10;
	T_SPFC_IS1893_2016    IS2016;
	T_SPFC_KSCE2019       KSCE2019;
	T_SPFC_NTC2018        NTC2018;
	T_SPFC_DPWH_LRFD_2013 DPWHLRFD2013;
	T_SPFC_KDS2019        KDS2019;
	T_SPFC_AS_5100_2_2017 AS510022017;
	T_SPFC_IRCSP114_2018  IRCSP114;
	T_SPFC_JTG_T_2231_01_2020 JTG_T_2231_01_2020;
	T_SPFC_JP_BRG2002     JPBRG2012;    // 905 추가
	T_SPFC_JP_BRG2002     JPBRG2017;    // 905 추가
	T_SPFC_DPT2018        DPT2018;
	T_SPFC_CH_GB_T_51408_2021 GB_T_51408_2021;
};

union T_SPFC_CODE_955
{
	T_SPFC_CH_BRG89       CHBRG89;
	T_SPFC_CH_GBJ111_87   CHGBJ111_87;
	T_SPFC_CH2002         CH2002;
	T_SPFC_CHSH2003       CHSH2003;
	T_SPFC_TAIWAN89       TAIWAN99V;
	T_SPFC_TAIWAN89       TAIWAN99H;
	T_SPFC_TAIWAN89       TAIWAN89V;
	T_SPFC_TAIWAN89       TAIWAN89H;
	T_SPFC_TAIWAN06       TAIWAN06;
	T_SPFC_TAIWAN06       TAIWANBRG98;
	T_SPFC_GB50111_2006   GB50111_2006;
	T_SPFC_JTJXXX_2008    JTJXXX_2008;
	T_SPFC_EURO1996       EURO1996;
	T_SPFC_EURO1996_ELA   EURO1996_ELA;
	T_SPFC_EURO2004       EURO2004;
	T_SPFC_CH2010         CH2010;
	T_SPFC_CH2019         CH2019;
	T_SPFC_CJJ166_2011    CJJ166_2011;
	T_SPFC_P100_2013      P100_2013;
	T_SPFC_JPN2000        JP2000;
	T_SPFC_JP_BRG2002     JPBRG2002;
	T_SPFC_IS1893_2016    IS1893;
	T_SPFC_KS             KS;
	T_SPFC_KS2000         KS2000;
	T_SPFC_KS_BRG         KSBRG;
	T_SPFC_UBC88          UBC88;
	T_SPFC_UBC97          UBC97;
	T_SPFC_KBC_2005       KBC2005;
	T_SPFC_KBC_2009       KBC2009;
	T_SPFC_KBC_2016       KBC2016;
	T_SPFC_IBC2000        IBC2000;
	T_SPFC_IBC2012        IBC2009;
	T_SPFC_IBC2012        IBC2012;
	T_SPFC_AASHTO_LRFD12  AASHTO_LRFD12;
	T_SPFC_NBC95          NBC95;
	T_SPFC_NSR_10         NSR10;
	T_SPFC_IS1893_2016    IS2016;
	T_SPFC_KSCE2019       KSCE2019;
	T_SPFC_NTC2018        NTC2018;
	T_SPFC_DPWH_LRFD_2013 DPWHLRFD2013;
	T_SPFC_KDS2019        KDS2019;
	T_SPFC_AS_5100_2_2017 AS510022017;
	T_SPFC_IRCSP114_2018  IRCSP114;
	T_SPFC_JTG_T_2231_01_2020 JTG_T_2231_01_2020;
	T_SPFC_JP_BRG2002     JPBRG2012;    // 905 추가
	T_SPFC_JP_BRG2002     JPBRG2017;    // 905 추가
	T_SPFC_DPT2018        DPT2018;
	T_SPFC_CH_GB_T_51408_2021 GB_T_51408_2021;
	T_SPFC_SANS2010       SANS2010;
};

union T_SPFC_CODE
{
	T_SPFC_CH_BRG89       CHBRG89;
	T_SPFC_CH_GBJ111_87   CHGBJ111_87;
	T_SPFC_CH2002         CH2002;
	T_SPFC_CHSH2003       CHSH2003;
	T_SPFC_TAIWAN89       TAIWAN99V;
	T_SPFC_TAIWAN89       TAIWAN99H;
	T_SPFC_TAIWAN89       TAIWAN89V;
	T_SPFC_TAIWAN89       TAIWAN89H;
	T_SPFC_TAIWAN06       TAIWAN06;
	T_SPFC_TAIWAN06       TAIWANBRG98;
	T_SPFC_TAIWAN22       TAIWAN22;
	T_SPFC_GB50111_2006   GB50111_2006;
	T_SPFC_JTJXXX_2008    JTJXXX_2008;
	T_SPFC_EURO1996       EURO1996;
	T_SPFC_EURO1996_ELA   EURO1996_ELA;
	T_SPFC_EURO2004       EURO2004;
	T_SPFC_CH2010         CH2010;
	T_SPFC_CH2019         CH2019;
	T_SPFC_CJJ166_2011    CJJ166_2011;
	T_SPFC_P100_2013      P100_2013;
	T_SPFC_JPN2000        JP2000;
	T_SPFC_JP_BRG2002     JPBRG2002;
	T_SPFC_IS1893_2016    IS1893;
	T_SPFC_KS             KS;
	T_SPFC_KS2000         KS2000;
	T_SPFC_KS_BRG         KSBRG;
	T_SPFC_UBC88          UBC88;
	T_SPFC_UBC97          UBC97;
	T_SPFC_KBC_2005       KBC2005;
	T_SPFC_KBC_2009       KBC2009;
	T_SPFC_KBC_2016       KBC2016;
	T_SPFC_IBC2000        IBC2000;
	T_SPFC_IBC2012        IBC2009;
	T_SPFC_IBC2012        IBC2012;
	T_SPFC_AASHTO_LRFD12  AASHTO_LRFD12;
	T_SPFC_NBC95          NBC95;
	T_SPFC_NSR_10         NSR10;
	T_SPFC_IS1893_2016    IS2016;
	T_SPFC_KSCE2019       KSCE2019;
	T_SPFC_NTC2018        NTC2018;
	T_SPFC_DPWH_LRFD_2013 DPWHLRFD2013;
	T_SPFC_KDS2019        KDS2019;
	T_SPFC_AS_5100_2_2017 AS510022017;
	T_SPFC_IRCSP114_2018  IRCSP114;
	T_SPFC_JTG_T_2231_01_2020 JTG_T_2231_01_2020;
	T_SPFC_JP_BRG2002     JPBRG2012;    // 905 추가
	T_SPFC_JP_BRG2002     JPBRG2017;    // 905 추가
	T_SPFC_DPT2018        DPT2018;
	T_SPFC_CH_GB_T_51408_2021 GB_T_51408_2021;
	T_SPFC_RUS_SP2016     SP2016;       // 945 추가
	T_SPFC_IBC2012        NSCP2024;
	T_SPFC_SANS2010 	  SANS2010;		//955 추가
	T_SPFC_RUS_SP14_2018  SP14_2018;
	T_SPFC_IS1893_2025    IS2025;

	void Initialize(int nCode)
	{
		switch ( nCode )
		{
		case D_SPFC_CODE_KS:                 KS.Initialize();                 break;
		case D_SPFC_CODE_KS2000:             KS2000.Initialize();             break;
		case D_SPFC_CODE_KS_BRG:             KSBRG.Initialize();              break;
		case D_SPFC_CODE_UBC88:              UBC88.Initialize();              break;
		case D_SPFC_CODE_UBC97:              UBC97.Initialize();              break;
		case D_SPFC_CODE_EURO1996:           EURO1996.Initialize();           break;
		case D_SPFC_CODE_CH2002:             CH2002.Initialize();             break;
		case D_SPFC_CODE_IBC2000:            IBC2000.Initialize();            break;
		case D_SPFC_CODE_JPN2000:            JP2000.Initialize();             break;
		case D_SPFC_CODE_CH_BRG89:           CHBRG89.Initialize();            break;
		case D_SPFC_CODE_CH_GBJ111_87:       CHGBJ111_87.Initialize();        break;
		case D_SPFC_CODE_NBC95:              NBC95.Initialize();              break;
		case D_SPFC_CODE_EURO1996_ELA:       EURO1996_ELA.Initialize();       break;
		case D_SPFC_CODE_IS2002:             IS1893.Initialize();             break;
		case D_SPFC_CODE_TAIWAN99H:          TAIWAN99H.Initialize();          break;
		case D_SPFC_CODE_TAIWAN99V:          TAIWAN99V.Initialize();          break;
		case D_SPFC_CODE_TAIWAN89H_BRG:      TAIWAN89H.Initialize();          break;
		case D_SPFC_CODE_TAIWAN89V_BRG:      TAIWAN89V.Initialize();          break;
		case D_SPFC_CODE_JP_BRG2002:         JPBRG2002.Initialize();          break;
		case D_SPFC_CODE_KBC2005:            KBC2005.Initialize();            break;
		case D_SPFC_CODE_CHSH2003:           CHSH2003.Initialize();           break;
		case D_SPFC_CODE_EURO2004:           EURO2004.Initialize();           break;
		case D_SPFC_CODE_TAIWAN06:           TAIWAN06.Initialize();           break;
		case D_SPFC_CODE_TAIWAN22:           TAIWAN22.Initialize();           break;
		case D_SPFC_CODE_GB50111_2006:       GB50111_2006.Initialize();       break;
		case D_SPFC_CODE_JTJXXX_2008:        JTJXXX_2008.Initialize();        break;
		case D_SPFC_CODE_KBC2009:            KBC2009.Initialize();            break;
		case D_SPFC_CODE_TAIWANBRG98:        TAIWANBRG98.Initialize();        break;
		case D_SPFC_CODE_CH2010:             CH2010.Initialize();             break;
		case D_SPFC_CODE_CH2019:             CH2019.Initialize();             break;
		case D_SPFC_CODE_CJJ166_2011:        CJJ166_2011.Initialize();        break;
		case D_SPFC_CODE_IBC2009:            IBC2009.Initialize();            break;
		case D_SPFC_CODE_IBC2012:            IBC2012.Initialize();            break;
		case D_SPFC_CODE_AASHTO_LRFD12:      AASHTO_LRFD12.Initialize();      break;
		case D_SPFC_CODE_NSR2010:            NSR10.Initialize();              break;
		case D_SPFC_CODE_KBC2015:            KBC2016.Initialize();            break;
		case D_SPFC_CODE_P100_2013:          P100_2013.Initialize();          break;
		case D_SPFC_CODE_IS1893_2016:        IS2016.Initialize();             break;
		case D_SPFC_CODE_KSCE2019:           KSCE2019.Initialize();           break;
		case D_SPFC_CODE_NTC2018:            NTC2018.Initialize();            break;
		case D_SPFC_CODE_DPWH_LRFD_2013:     DPWHLRFD2013.Initialize();       break;
		case D_SPFC_CODE_KDS2019:            KDS2019.Initialize();            break;
		case D_SPFC_CODE_AS_5100_2_2017:     AS510022017.Initialize();        break;
		case D_SPFC_CODE_IRCSP114_2018:      IRCSP114.Initialize();           break;
		case D_SPFC_CODE_JTG_T_2231_01_2020: JTG_T_2231_01_2020.Initialize(); break;
		case D_SPFC_CODE_JP_BRG2012:         JPBRG2012.Initialize();          break;
		case D_SPFC_CODE_JP_BRG2017:         JPBRG2017.Initialize();          break;
		case D_SPFC_CODE_DPT2018:            DPT2018.Init();                  break;
		case D_SPFC_CODE_CH_GB_T_51408_2021: GB_T_51408_2021.Initialize();    break;
		case D_SPFC_CODE_RUS_SP2016:         SP2016.Initialize();             break;
		case D_SPFC_CODE_RUS_SP14_2018:      SP14_2018.Initialize();          break;
		case D_SPFC_CODE_NSCP_2024:          NSCP2024.Initialize();           break;
		case D_SPFC_CODE_SANS:				 SANS2010.Initialize();           break;
		case D_SPFC_CODE_IS1893_2025:		 IS2025.Initialize();             break;
		}
	}
	void POInitialize(int nCode)
	{
		switch ( nCode )
		{
		case D_SPFC_CODE_KS:                 KS.Initialize();                 break;
		case D_SPFC_CODE_KS2000:             KS2000.Initialize();             break;
		case D_SPFC_CODE_KS_BRG:             KSBRG.Initialize();              break;
		case D_SPFC_CODE_UBC88:              UBC88.Initialize();              break;
		case D_SPFC_CODE_UBC97:              UBC97.Initialize();              break;
		case D_SPFC_CODE_EURO1996:           EURO1996.POInitialize();         break;
		case D_SPFC_CODE_CH2002:             CH2002.Initialize();             break;
		case D_SPFC_CODE_IBC2000:            IBC2000.Initialize();            break;
		case D_SPFC_CODE_JPN2000:            JP2000.Initialize();             break;
		case D_SPFC_CODE_CH_BRG89:           CHBRG89.Initialize();            break;
		case D_SPFC_CODE_CH_GBJ111_87:       CHGBJ111_87.Initialize();        break;
		case D_SPFC_CODE_NBC95:              NBC95.Initialize();              break;
		case D_SPFC_CODE_EURO1996_ELA:       EURO1996_ELA.Initialize();       break;
		case D_SPFC_CODE_IS2002:             IS1893.Initialize();             break;
		case D_SPFC_CODE_TAIWAN99H:          TAIWAN99H.Initialize();          break;
		case D_SPFC_CODE_TAIWAN99V:          TAIWAN99V.Initialize();          break;
		case D_SPFC_CODE_TAIWAN89H_BRG:      TAIWAN89H.Initialize();          break;
		case D_SPFC_CODE_TAIWAN89V_BRG:      TAIWAN89V.Initialize();          break;
		case D_SPFC_CODE_JP_BRG2002:         JPBRG2002.Initialize();          break;
		case D_SPFC_CODE_KBC2005:            KBC2005.POInitialize();          break;
		case D_SPFC_CODE_CHSH2003:           CHSH2003.Initialize();           break;
		case D_SPFC_CODE_EURO2004:           EURO2004.POInitialize();         break;
		case D_SPFC_CODE_TAIWAN06:           TAIWAN06.POInitialize();         break;
		case D_SPFC_CODE_TAIWAN22:           TAIWAN22.POInitialize();         break;
		case D_SPFC_CODE_GB50111_2006:       GB50111_2006.Initialize();       break;
		case D_SPFC_CODE_JTJXXX_2008:        JTJXXX_2008.Initialize();        break;
		case D_SPFC_CODE_KBC2009:            KBC2009.POInitialize();          break;
		case D_SPFC_CODE_TAIWANBRG98:        TAIWANBRG98.POInitialize(TRUE);  break;
		case D_SPFC_CODE_CH2010:             CH2010.Initialize();             break;
		case D_SPFC_CODE_CJJ166_2011:        CJJ166_2011.Initialize();        break;
		case D_SPFC_CODE_IBC2009:            IBC2009.POInitialize();          break;
		case D_SPFC_CODE_IBC2012:            IBC2012.POInitialize();          break;
		case D_SPFC_CODE_AASHTO_LRFD12:      AASHTO_LRFD12.Initialize();      break;
		case D_SPFC_CODE_NSR2010:            NSR10.Initialize();              break;
		case D_SPFC_CODE_KBC2015:            KBC2016.POInitialize();          break;
		case D_SPFC_CODE_P100_2013:          P100_2013.Initialize();          break;
		case D_SPFC_CODE_IS1893_2016:        IS2016.Initialize();             break;
		case D_SPFC_CODE_KSCE2019:           KSCE2019.POInitialize();         break;
		case D_SPFC_CODE_NTC2018:            NTC2018.Initialize();            break;
		case D_SPFC_CODE_DPWH_LRFD_2013:     DPWHLRFD2013.Initialize();       break;
		case D_SPFC_CODE_KDS2019:            KDS2019.Initialize();            break;
		case D_SPFC_CODE_AS_5100_2_2017:     AS510022017.Initialize();        break;
		case D_SPFC_CODE_IRCSP114_2018:      IRCSP114.Initialize();           break;
		case D_SPFC_CODE_JTG_T_2231_01_2020: JTG_T_2231_01_2020.Initialize(); break;
		case D_SPFC_CODE_JP_BRG2012:         JPBRG2012.Initialize();          break;
		case D_SPFC_CODE_JP_BRG2017:         JPBRG2017.Initialize();          break;
		case D_SPFC_CODE_DPT2018:            DPT2018.Init();                  break;
		case D_SPFC_CODE_CH_GB_T_51408_2021: GB_T_51408_2021.Initialize();	  break;
		case D_SPFC_CODE_RUS_SP2016:         SP2016.Initialize();             break;
		case D_SPFC_CODE_RUS_SP14_2018:      SP14_2018.Initialize();          break;
		case D_SPFC_CODE_NSCP_2024:          NSCP2024.Initialize();           break;
		case D_SPFC_CODE_SANS:				 SANS2010.Initialize();           break;
		case D_SPFC_CODE_IS1893_2025:		 IS2025.Initialize();             break;
		}
	}
	void Convert770(int nCode, T_SPFC_CODE_770& rData)
	{
		switch ( nCode )
		{
		case D_SPFC_CODE_CH2002:
			{
				CH2002.nSeisDgnCategory   = rData.CH2002.nSeisDgnCategory;
				CH2002.nSeisFortification = rData.CH2002.nSeisFortification;
				CH2002.nSiteClass         = rData.CH2002.nSiteClass;
				CH2002.nEarthResponse     = rData.CH2002.nEarthResponse;
				CH2002.dTg                = rData.CH2002.dTg;
				CH2002.dDamping           = rData.CH2002.dDamping;
				CH2002.dMaxEQE            = 0.04;
			}
			break;
		case D_SPFC_CODE_CHSH2003:
			{
				CHSH2003.nSeisFortification = rData.CHSH2003.nSeisFortification;
				CHSH2003.nSiteClass         = rData.CHSH2003.nSiteClass;
				CHSH2003.nEarthResponse     = rData.CHSH2003.nEarthResponse;
				CHSH2003.dTg                = rData.CHSH2003.dTg;
				CHSH2003.dDamping           = rData.CHSH2003.dDamping;
				CHSH2003.dMaxEQE            = 0.04;
			}
			break;
		case D_SPFC_CODE_TAIWAN06:
			{
				TAIWAN06.Convert871(rData.TAIWAN06);
			}
			break;
		case D_SPFC_CODE_GB50111_2006:
			{
				GB50111_2006 = rData.GB50111_2006;
			}
			break;
		case D_SPFC_CODE_JTJXXX_2008:
			{
				JTJXXX_2008.nBridgeType = rData.JTJXXX_2008.nBridgeType;
				JTJXXX_2008.nZoningMap = rData.JTJXXX_2008.nZoningMap;
				JTJXXX_2008.nSiteType = rData.JTJXXX_2008.nSiteType;
				JTJXXX_2008.nSeisminIntensity = rData.JTJXXX_2008.nSeisminIntensity;
				JTJXXX_2008.nSeisminCriterion = rData.JTJXXX_2008.nSeisminCriterion;
				JTJXXX_2008.dDampingRatio = rData.JTJXXX_2008.dDampingRatio;
				JTJXXX_2008.bLargeBridge = rData.JTJXXX_2008.bLargeBridge;
				JTJXXX_2008.bVertAccSpec = FALSE;
				JTJXXX_2008.nVertAccSpecType = 0;
				JTJXXX_2008.dTg = rData.JTJXXX_2008.dTg;
				JTJXXX_2008.dCi = rData.JTJXXX_2008.dCi;
				JTJXXX_2008.dCs = rData.JTJXXX_2008.dCs;
				JTJXXX_2008.dCd = rData.JTJXXX_2008.dCd;
				JTJXXX_2008.dEPA = rData.JTJXXX_2008.dEPA;
				JTJXXX_2008.dSMax = rData.JTJXXX_2008.dSMax;
				JTJXXX_2008.dMaxPeriod = rData.JTJXXX_2008.dMaxPeriod;
			}
			break;
		case D_SPFC_CODE_EURO2004:
			EURO2004 = rData.EURO2004;
			break;
		default: Initialize(nCode); break;
		}
	}
	void Convert835(int nCode, T_SPFC_CODE_835& rData)
	{
		switch ( nCode )
		{
		case D_SPFC_CODE_CH2002:
			{
				CH2002.nSeisDgnCategory   = rData.CH2002.nSeisDgnCategory;
				CH2002.nSeisFortification = rData.CH2002.nSeisFortification;
				CH2002.nSiteClass         = rData.CH2002.nSiteClass;
				CH2002.nEarthResponse     = rData.CH2002.nEarthResponse;
				CH2002.dTg                = rData.CH2002.dTg;
				CH2002.dDamping           = rData.CH2002.dDamping;
				CH2002.dMaxEQE            = 0.04;
			}
			break;
		case D_SPFC_CODE_CHSH2003:
			{
				CHSH2003.nSeisFortification = rData.CHSH2003.nSeisFortification;
				CHSH2003.nSiteClass         = rData.CHSH2003.nSiteClass;
				CHSH2003.nEarthResponse     = rData.CHSH2003.nEarthResponse;
				CHSH2003.dTg                = rData.CHSH2003.dTg;
				CHSH2003.dDamping           = rData.CHSH2003.dDamping;
				CHSH2003.dMaxEQE            = 0.04;
			}
			break;
		case D_SPFC_CODE_CH2010:
			{
				CH2010.nSeisDgnCategory   = rData.CH2010.nSeisDgnCategory;
				CH2010.nSeisFortification = rData.CH2010.nSeisFortification;
				CH2010.nSiteClass         = rData.CH2010.nSiteClass;
				CH2010.nEarthResponse     = rData.CH2010.nEarthResponse;
				CH2010.dTg                = rData.CH2010.dTg;
				CH2010.dDamping           = rData.CH2010.dDamping;
				CH2010.nLForce            = rData.CH2010.nLForce;
				CH2010.dMaxEQE            = 0.04;
			}
			break;
		case D_SPFC_CODE_TAIWAN06:
			{
				TAIWAN06.Convert871(rData.TAIWAN06);
			}
			break;
		case D_SPFC_CODE_GB50111_2006:
			{
				GB50111_2006 = rData.GB50111_2006;
			}
			break;
		case D_SPFC_CODE_JTJXXX_2008:
			{
				JTJXXX_2008 = rData.JTJXXX_2008;
			}
			break;
		case D_SPFC_CODE_EURO2004:
			{
				EURO2004 = rData.EURO2004;
			}
			break;
		case D_SPFC_CODE_CJJ166_2011:
			{
				CJJ166_2011 = rData.CJJ166_2011;
			}
			break;
		default: Initialize(nCode); break;
		}
	}
	void Convert871(int nCode, T_SPFC_CODE_871& rData)
	{
		switch ( nCode )
		{
		case D_SPFC_CODE_CH2002:        CH2002 = rData.CH2002;                break;
		case D_SPFC_CODE_CHSH2003:      CHSH2003 = rData.CHSH2003;            break;
		case D_SPFC_CODE_CH2010:        CH2010 = rData.CH2010;                break;
		case D_SPFC_CODE_TAIWAN06:      TAIWAN06.Convert871(rData.TAIWAN06);  break;
		case D_SPFC_CODE_GB50111_2006:  GB50111_2006 = rData.GB50111_2006;    break;
		case D_SPFC_CODE_JTJXXX_2008:   JTJXXX_2008 = rData.JTJXXX_2008;      break;
		case D_SPFC_CODE_EURO2004:      EURO2004 = rData.EURO2004;            break;
		case D_SPFC_CODE_CJJ166_2011:   CJJ166_2011 = rData.CJJ166_2011;      break;
		case D_SPFC_CODE_P100_2013:     P100_2013 = rData.P100_2013;          break;
		default: Initialize(nCode); break;
		}
	}
	void Convert881(int nCode, T_SPFC_CODE_881& rData)
	{
		switch ( nCode )
		{
		case D_SPFC_CODE_KS:            KS                = rData.KS; break;
		case D_SPFC_CODE_KS2000:        KS2000            = rData.KS2000; break;
		case D_SPFC_CODE_KS_BRG:        KSBRG             = rData.KSBRG; break;
		case D_SPFC_CODE_UBC88:         UBC88             = rData.UBC88; break;
		case D_SPFC_CODE_UBC97:         UBC97             = rData.UBC97; break;
		case D_SPFC_CODE_EURO1996:      EURO1996          = rData.EURO1996; break;
		case D_SPFC_CODE_CH2002:        CH2002            = rData.CH2002; break;
		case D_SPFC_CODE_IBC2000:       IBC2000           = rData.IBC2000; break;
		case D_SPFC_CODE_JPN2000:       JP2000            = rData.JP2000; break;
		case D_SPFC_CODE_CH_BRG89:      CHBRG89           = rData.CHBRG89; break;
		case D_SPFC_CODE_CH_GBJ111_87:  CHGBJ111_87       = rData.CHGBJ111_87; break;
		case D_SPFC_CODE_NBC95:         NBC95             = rData.NBC95; break;
		case D_SPFC_CODE_EURO1996_ELA:  EURO1996_ELA      = rData.EURO1996_ELA; break;
		case D_SPFC_CODE_IS2002:        IS1893            = rData.IS1893; break;
		case D_SPFC_CODE_TAIWAN99H:     TAIWAN99H         = rData.TAIWAN99H; break;
		case D_SPFC_CODE_TAIWAN99V:     TAIWAN99V         = rData.TAIWAN99V; break;
		case D_SPFC_CODE_TAIWAN89H_BRG: TAIWAN89H         = rData.TAIWAN89H; break;
		case D_SPFC_CODE_TAIWAN89V_BRG: TAIWAN89V         = rData.TAIWAN89V; break;
		case D_SPFC_CODE_JP_BRG2002:    JPBRG2002         = rData.JPBRG2002; break;
		case D_SPFC_CODE_KBC2005:       KBC2005           = rData.KBC2005; break;
		case D_SPFC_CODE_CHSH2003:      CHSH2003          = rData.CHSH2003; break;
		case D_SPFC_CODE_EURO2004:      EURO2004          = rData.EURO2004; break;
		case D_SPFC_CODE_TAIWAN06:      TAIWAN06          = rData.TAIWAN06; break;
		case D_SPFC_CODE_GB50111_2006:  GB50111_2006      = rData.GB50111_2006; break;
		case D_SPFC_CODE_JTJXXX_2008:   JTJXXX_2008       = rData.JTJXXX_2008;  break;
		case D_SPFC_CODE_KBC2009:       KBC2009           = rData.KBC2009; break;
		case D_SPFC_CODE_TAIWANBRG98:   TAIWANBRG98       = rData.TAIWANBRG98; break;
		case D_SPFC_CODE_CH2010:        CH2010            = rData.CH2010; break;
		case D_SPFC_CODE_CJJ166_2011:   CJJ166_2011       = rData.CJJ166_2011; break;
		case D_SPFC_CODE_IBC2009:       IBC2009           = rData.IBC2009; break;
		case D_SPFC_CODE_IBC2012:       IBC2012           = rData.IBC2012; break;
		case D_SPFC_CODE_AASHTO_LRFD12: AASHTO_LRFD12     = rData.AASHTO_LRFD12; break;
		case D_SPFC_CODE_NSR2010:       NSR10             = rData.NSR10; break;
		case D_SPFC_CODE_KBC2015:       KBC2016           = rData.KBC2016; break;
		case D_SPFC_CODE_P100_2013:     P100_2013         = rData.P100_2013; break;
		case D_SPFC_CODE_IS1893_2016:   IS2016            = rData.IS2016; break;
		default: Initialize(nCode); break;
		}
	}
	void Convert883(int nCode, T_SPFC_CODE_883& rData)
	{
		switch ( nCode )
		{
		case D_SPFC_CODE_KS:             KS               = rData.KS; break;
		case D_SPFC_CODE_KS2000:         KS2000           = rData.KS2000; break;
		case D_SPFC_CODE_KS_BRG:         KSBRG            = rData.KSBRG; break;
		case D_SPFC_CODE_UBC88:          UBC88            = rData.UBC88; break;
		case D_SPFC_CODE_UBC97:          UBC97            = rData.UBC97; break;
		case D_SPFC_CODE_EURO1996:       EURO1996         = rData.EURO1996; break;
		case D_SPFC_CODE_CH2002:         CH2002           = rData.CH2002; break;
		case D_SPFC_CODE_IBC2000:        IBC2000          = rData.IBC2000; break;
		case D_SPFC_CODE_JPN2000:        JP2000           = rData.JP2000; break;
		case D_SPFC_CODE_CH_BRG89:       CHBRG89          = rData.CHBRG89; break;
		case D_SPFC_CODE_CH_GBJ111_87:   CHGBJ111_87      = rData.CHGBJ111_87; break;
		case D_SPFC_CODE_NBC95:          NBC95            = rData.NBC95; break;
		case D_SPFC_CODE_EURO1996_ELA:   EURO1996_ELA     = rData.EURO1996_ELA; break;
		case D_SPFC_CODE_IS2002:         IS1893           = rData.IS1893; break;
		case D_SPFC_CODE_TAIWAN99H:      TAIWAN99H        = rData.TAIWAN99H; break;
		case D_SPFC_CODE_TAIWAN99V:      TAIWAN99V        = rData.TAIWAN99V; break;
		case D_SPFC_CODE_TAIWAN89H_BRG:  TAIWAN89H        = rData.TAIWAN89H; break;
		case D_SPFC_CODE_TAIWAN89V_BRG:  TAIWAN89V        = rData.TAIWAN89V; break;
		case D_SPFC_CODE_JP_BRG2002:     JPBRG2002        = rData.JPBRG2002; break;
		case D_SPFC_CODE_KBC2005:        KBC2005          = rData.KBC2005; break;
		case D_SPFC_CODE_CHSH2003:       CHSH2003         = rData.CHSH2003; break;
		case D_SPFC_CODE_EURO2004:       EURO2004         = rData.EURO2004; break;
		case D_SPFC_CODE_TAIWAN06:       TAIWAN06         = rData.TAIWAN06; break;
		case D_SPFC_CODE_GB50111_2006:   GB50111_2006     = rData.GB50111_2006; break;
		case D_SPFC_CODE_JTJXXX_2008:    JTJXXX_2008      = rData.JTJXXX_2008; break;
		case D_SPFC_CODE_KBC2009:        KBC2009          = rData.KBC2009; break;
		case D_SPFC_CODE_TAIWANBRG98:    TAIWANBRG98      = rData.TAIWANBRG98; break;
		case D_SPFC_CODE_CH2010:         CH2010           = rData.CH2010; break;
		case D_SPFC_CODE_CJJ166_2011:    CJJ166_2011      = rData.CJJ166_2011; break;
		case D_SPFC_CODE_IBC2009:        IBC2009          = rData.IBC2009; break;
		case D_SPFC_CODE_IBC2012:        IBC2012          = rData.IBC2012; break;
		case D_SPFC_CODE_AASHTO_LRFD12:  AASHTO_LRFD12    = rData.AASHTO_LRFD12; break;
		case D_SPFC_CODE_NSR2010:        NSR10            = rData.NSR10; break;
		case D_SPFC_CODE_KBC2015:        KBC2016          = rData.KBC2016; break;
		case D_SPFC_CODE_P100_2013:      P100_2013        = rData.P100_2013; break;
		case D_SPFC_CODE_IS1893_2016:    IS2016           = rData.IS2016; break;
		case D_SPFC_CODE_KSCE2019:       KSCE2019         = rData.KSCE2019; break;
		case D_SPFC_CODE_NTC2018:        NTC2018          = rData.NTC2018; break;
		case D_SPFC_CODE_DPWH_LRFD_2013: DPWHLRFD2013     = rData.DPWHLRFD2013; break;
		default: Initialize(nCode); break;
		}
	}
	void Convert895(int nCode, T_SPFC_CODE_895& rData)
	{
		switch ( nCode )
		{
		case D_SPFC_CODE_KS:             KS               = rData.KS; break;
		case D_SPFC_CODE_KS2000:         KS2000           = rData.KS2000; break;
		case D_SPFC_CODE_KS_BRG:         KSBRG            = rData.KSBRG; break;
		case D_SPFC_CODE_UBC88:          UBC88            = rData.UBC88; break;
		case D_SPFC_CODE_UBC97:          UBC97            = rData.UBC97; break;
		case D_SPFC_CODE_EURO1996:       EURO1996         = rData.EURO1996; break;
		case D_SPFC_CODE_CH2002:         CH2002             = rData.CH2002; break;
		case D_SPFC_CODE_IBC2000:        IBC2000          = rData.IBC2000; break;
		case D_SPFC_CODE_JPN2000:        JP2000           = rData.JP2000; break;
		case D_SPFC_CODE_CH_BRG89:       CHBRG89          = rData.CHBRG89; break;
		case D_SPFC_CODE_CH_GBJ111_87:   CHGBJ111_87      = rData.CHGBJ111_87; break;
		case D_SPFC_CODE_NBC95:          NBC95            = rData.NBC95; break;
		case D_SPFC_CODE_EURO1996_ELA:   EURO1996_ELA     = rData.EURO1996_ELA; break;
		case D_SPFC_CODE_IS2002:         IS1893           = rData.IS1893; break;
		case D_SPFC_CODE_TAIWAN99H:      TAIWAN99H        = rData.TAIWAN99H; break;
		case D_SPFC_CODE_TAIWAN99V:      TAIWAN99V        = rData.TAIWAN99V; break;
		case D_SPFC_CODE_TAIWAN89H_BRG:  TAIWAN89H        = rData.TAIWAN89H; break;
		case D_SPFC_CODE_TAIWAN89V_BRG:  TAIWAN89V        = rData.TAIWAN89V; break;
		case D_SPFC_CODE_JP_BRG2002:     JPBRG2002        = rData.JPBRG2002; break;
		case D_SPFC_CODE_KBC2005:        KBC2005          = rData.KBC2005; break;
		case D_SPFC_CODE_CHSH2003:       CHSH2003         = rData.CHSH2003; break;
		case D_SPFC_CODE_EURO2004:       EURO2004         = rData.EURO2004; break;
		case D_SPFC_CODE_TAIWAN06:       TAIWAN06         = rData.TAIWAN06; break;
		case D_SPFC_CODE_GB50111_2006:   GB50111_2006     = rData.GB50111_2006; break;
		case D_SPFC_CODE_JTJXXX_2008:    JTJXXX_2008      = rData.JTJXXX_2008; break;
		case D_SPFC_CODE_KBC2009:        KBC2009          = rData.KBC2009; break;
		case D_SPFC_CODE_TAIWANBRG98:    TAIWANBRG98      = rData.TAIWANBRG98; break;
		case D_SPFC_CODE_CH2010:         CH2010           = rData.CH2010; break;
		case D_SPFC_CODE_CJJ166_2011:    CJJ166_2011      = rData.CJJ166_2011; break;
		case D_SPFC_CODE_IBC2009:        IBC2009          = rData.IBC2009; break;
		case D_SPFC_CODE_IBC2012:        IBC2012          = rData.IBC2012; break;
		case D_SPFC_CODE_AASHTO_LRFD12:  AASHTO_LRFD12    = rData.AASHTO_LRFD12; break;
		case D_SPFC_CODE_NSR2010:        NSR10            = rData.NSR10; break;
		case D_SPFC_CODE_KBC2015:        KBC2016          = rData.KBC2016; break;
		case D_SPFC_CODE_P100_2013:      P100_2013        = rData.P100_2013; break;
		case D_SPFC_CODE_IS1893_2016:    IS2016           = rData.IS2016; break;
		case D_SPFC_CODE_KSCE2019:       KSCE2019         = rData.KSCE2019; break;
		case D_SPFC_CODE_NTC2018:        NTC2018          = rData.NTC2018; break;
		case D_SPFC_CODE_DPWH_LRFD_2013: DPWHLRFD2013     = rData.DPWHLRFD2013; break;
		case D_SPFC_CODE_KDS2019:        KDS2019          = rData.KDS2019; break;
		case D_SPFC_CODE_AS_5100_2_2017: AS510022017      = rData.AS510022017; break;
		default: Initialize(nCode); break;
		}
	}
	void Convert915(int nCode, const T_SPFC_CODE_915& rData)
	{
		switch ( nCode )
		{
		case D_SPFC_CODE_KS:             KS            = rData.KS; break;
		case D_SPFC_CODE_KS2000:         KS2000        = rData.KS2000; break;
		case D_SPFC_CODE_KS_BRG:         KSBRG         = rData.KSBRG; break;
		case D_SPFC_CODE_UBC88:          UBC88         = rData.UBC88; break;
		case D_SPFC_CODE_UBC97:          UBC97         = rData.UBC97; break;
		case D_SPFC_CODE_EURO1996:       EURO1996      = rData.EURO1996; break;
		case D_SPFC_CODE_CH2002:         CH2002        = rData.CH2002; break;
		case D_SPFC_CODE_IBC2000:        IBC2000       = rData.IBC2000; break;
		case D_SPFC_CODE_JPN2000:        JP2000        = rData.JP2000; break;
		case D_SPFC_CODE_CH_BRG89:       CHBRG89       = rData.CHBRG89; break;
		case D_SPFC_CODE_CH_GBJ111_87:   CHGBJ111_87   = rData.CHGBJ111_87; break;
		case D_SPFC_CODE_NBC95:          NBC95         = rData.NBC95; break;
		case D_SPFC_CODE_EURO1996_ELA:   EURO1996_ELA  = rData.EURO1996_ELA; break;
		case D_SPFC_CODE_IS2002:         IS1893        = rData.IS1893; break;
		case D_SPFC_CODE_TAIWAN99H:      TAIWAN99H     = rData.TAIWAN99H; break;
		case D_SPFC_CODE_TAIWAN99V:      TAIWAN99V     = rData.TAIWAN99V; break;
		case D_SPFC_CODE_TAIWAN89H_BRG:  TAIWAN89H     = rData.TAIWAN89H; break;
		case D_SPFC_CODE_TAIWAN89V_BRG:  TAIWAN89V     = rData.TAIWAN89V; break;
		case D_SPFC_CODE_JP_BRG2002:     JPBRG2002     = rData.JPBRG2002; break;
		case D_SPFC_CODE_KBC2005:        KBC2005       = rData.KBC2005; break;
		case D_SPFC_CODE_CHSH2003:       CHSH2003      = rData.CHSH2003; break;
		case D_SPFC_CODE_EURO2004:       EURO2004      = rData.EURO2004; break;
		case D_SPFC_CODE_TAIWAN06:       TAIWAN06      = rData.TAIWAN06; break;
		case D_SPFC_CODE_GB50111_2006:   GB50111_2006  = rData.GB50111_2006; break;
		case D_SPFC_CODE_JTJXXX_2008:    JTJXXX_2008   = rData.JTJXXX_2008; break;
		case D_SPFC_CODE_KBC2009:        KBC2009       = rData.KBC2009; break;
		case D_SPFC_CODE_TAIWANBRG98:    TAIWANBRG98   = rData.TAIWANBRG98; break;
		case D_SPFC_CODE_CH2010:         CH2010        = rData.CH2010; break;
		case D_SPFC_CODE_CJJ166_2011:    CJJ166_2011   = rData.CJJ166_2011; break;
		case D_SPFC_CODE_IBC2009:        IBC2009       = rData.IBC2009; break;
		case D_SPFC_CODE_IBC2012:        IBC2012       = rData.IBC2012; break;
		case D_SPFC_CODE_AASHTO_LRFD12:  AASHTO_LRFD12 = rData.AASHTO_LRFD12; break;
		case D_SPFC_CODE_NSR2010:        NSR10         = rData.NSR10; break;
		case D_SPFC_CODE_KBC2015:        KBC2016       = rData.KBC2016; break;
		case D_SPFC_CODE_P100_2013:      P100_2013     = rData.P100_2013; break;
		case D_SPFC_CODE_IS1893_2016:    IS2016        = rData.IS2016; break;
		case D_SPFC_CODE_KSCE2019:       KSCE2019      = rData.KSCE2019; break;
		case D_SPFC_CODE_NTC2018:        NTC2018       = rData.NTC2018; break;
		case D_SPFC_CODE_DPWH_LRFD_2013: DPWHLRFD2013  = rData.DPWHLRFD2013; break;
		case D_SPFC_CODE_KDS2019:        KDS2019       = rData.KDS2019; break;
		case D_SPFC_CODE_AS_5100_2_2017: AS510022017   = rData.AS510022017; break;
		case D_SPFC_CODE_JP_BRG2017:     JPBRG2017     = rData.JPBRG2017; break;
		default: Initialize(nCode); break;
		}
	}
	void Convert920(int nCode, const T_SPFC_CODE_920& rData)
	{
		switch (nCode)
		{
		case D_SPFC_CODE_KS:             KS = rData.KS; break;
		case D_SPFC_CODE_KS2000:         KS2000 = rData.KS2000; break;
		case D_SPFC_CODE_KS_BRG:         KSBRG = rData.KSBRG; break;
		case D_SPFC_CODE_UBC88:          UBC88 = rData.UBC88; break;
		case D_SPFC_CODE_UBC97:          UBC97 = rData.UBC97; break;
		case D_SPFC_CODE_EURO1996:       EURO1996 = rData.EURO1996; break;
		case D_SPFC_CODE_CH2002:         CH2002 = rData.CH2002; break;
		case D_SPFC_CODE_IBC2000:        IBC2000 = rData.IBC2000; break;
		case D_SPFC_CODE_JPN2000:        JP2000 = rData.JP2000; break;
		case D_SPFC_CODE_CH_BRG89:       CHBRG89 = rData.CHBRG89; break;
		case D_SPFC_CODE_CH_GBJ111_87:   CHGBJ111_87 = rData.CHGBJ111_87; break;
		case D_SPFC_CODE_NBC95:          NBC95 = rData.NBC95; break;
		case D_SPFC_CODE_EURO1996_ELA:   EURO1996_ELA = rData.EURO1996_ELA; break;
		case D_SPFC_CODE_IS2002:         IS1893 = rData.IS1893; break;
		case D_SPFC_CODE_TAIWAN99H:      TAIWAN99H = rData.TAIWAN99H; break;
		case D_SPFC_CODE_TAIWAN99V:      TAIWAN99V = rData.TAIWAN99V; break;
		case D_SPFC_CODE_TAIWAN89H_BRG:  TAIWAN89H = rData.TAIWAN89H; break;
		case D_SPFC_CODE_TAIWAN89V_BRG:  TAIWAN89V = rData.TAIWAN89V; break;
		case D_SPFC_CODE_JP_BRG2002:     JPBRG2002 = rData.JPBRG2002; break;
		case D_SPFC_CODE_KBC2005:        KBC2005 = rData.KBC2005; break;
		case D_SPFC_CODE_CHSH2003:       CHSH2003 = rData.CHSH2003; break;
		case D_SPFC_CODE_EURO2004:       EURO2004 = rData.EURO2004; break;
		case D_SPFC_CODE_TAIWAN06:       TAIWAN06 = rData.TAIWAN06; break;
		case D_SPFC_CODE_GB50111_2006:   GB50111_2006 = rData.GB50111_2006; break;
		case D_SPFC_CODE_JTJXXX_2008:    JTJXXX_2008 = rData.JTJXXX_2008; break;
		case D_SPFC_CODE_KBC2009:        KBC2009 = rData.KBC2009; break;
		case D_SPFC_CODE_TAIWANBRG98:    TAIWANBRG98 = rData.TAIWANBRG98; break;
		case D_SPFC_CODE_CH2010:         CH2010 = rData.CH2010; break;
		case D_SPFC_CODE_CJJ166_2011:    CJJ166_2011 = rData.CJJ166_2011; break;
		case D_SPFC_CODE_IBC2009:        IBC2009 = rData.IBC2009; break;
		case D_SPFC_CODE_IBC2012:        IBC2012 = rData.IBC2012; break;
		case D_SPFC_CODE_AASHTO_LRFD12:  AASHTO_LRFD12 = rData.AASHTO_LRFD12; break;
		case D_SPFC_CODE_NSR2010:        NSR10 = rData.NSR10; break;
		case D_SPFC_CODE_KBC2015:        KBC2016 = rData.KBC2016; break;
		case D_SPFC_CODE_P100_2013:      P100_2013 = rData.P100_2013; break;
		case D_SPFC_CODE_IS1893_2016:    IS2016 = rData.IS2016; break;
		case D_SPFC_CODE_KSCE2019:       KSCE2019 = rData.KSCE2019; break;
		case D_SPFC_CODE_NTC2018:        NTC2018 = rData.NTC2018; break;
		case D_SPFC_CODE_DPWH_LRFD_2013: DPWHLRFD2013 = rData.DPWHLRFD2013; break;
		case D_SPFC_CODE_KDS2019:        KDS2019 = rData.KDS2019; break;
		case D_SPFC_CODE_AS_5100_2_2017: AS510022017 = rData.AS510022017; break;
		case D_SPFC_CODE_JP_BRG2017:     JPBRG2017.Initialize(); break;
		case D_SPFC_CODE_DPT2018:        DPT2018.Init(); break;
		default: Initialize(nCode); break;
		}
	}
	void Convert925(int nCode, const T_SPFC_CODE_925& rData)
	{
		switch ( nCode )
		{
		case D_SPFC_CODE_KS:             KS = rData.KS; break;
		case D_SPFC_CODE_KS2000:         KS2000 = rData.KS2000; break;
		case D_SPFC_CODE_KS_BRG:         KSBRG = rData.KSBRG; break;
		case D_SPFC_CODE_UBC88:          UBC88 = rData.UBC88; break;
		case D_SPFC_CODE_UBC97:          UBC97 = rData.UBC97; break;
		case D_SPFC_CODE_EURO1996:       EURO1996 = rData.EURO1996; break;
		case D_SPFC_CODE_CH2002:         CH2002 = rData.CH2002; break;
		case D_SPFC_CODE_IBC2000:        IBC2000 = rData.IBC2000; break;
		case D_SPFC_CODE_JPN2000:        JP2000 = rData.JP2000; break;
		case D_SPFC_CODE_CH_BRG89:       CHBRG89 = rData.CHBRG89; break;
		case D_SPFC_CODE_CH_GBJ111_87:   CHGBJ111_87 = rData.CHGBJ111_87; break;
		case D_SPFC_CODE_NBC95:          NBC95 = rData.NBC95; break;
		case D_SPFC_CODE_EURO1996_ELA:   EURO1996_ELA = rData.EURO1996_ELA; break;
		case D_SPFC_CODE_IS2002:         IS1893 = rData.IS1893; break;
		case D_SPFC_CODE_TAIWAN99H:      TAIWAN99H = rData.TAIWAN99H; break;
		case D_SPFC_CODE_TAIWAN99V:      TAIWAN99V = rData.TAIWAN99V; break;
		case D_SPFC_CODE_TAIWAN89H_BRG:  TAIWAN89H = rData.TAIWAN89H; break;
		case D_SPFC_CODE_TAIWAN89V_BRG:  TAIWAN89V = rData.TAIWAN89V; break;
		case D_SPFC_CODE_JP_BRG2002:     JPBRG2002 = rData.JPBRG2002; break;
		case D_SPFC_CODE_KBC2005:        KBC2005 = rData.KBC2005; break;
		case D_SPFC_CODE_CHSH2003:       CHSH2003 = rData.CHSH2003; break;
		case D_SPFC_CODE_EURO2004:       EURO2004 = rData.EURO2004; break;
		case D_SPFC_CODE_TAIWAN06:       TAIWAN06 = rData.TAIWAN06; break;
		case D_SPFC_CODE_GB50111_2006:   GB50111_2006 = rData.GB50111_2006; break;
		case D_SPFC_CODE_JTJXXX_2008:    JTJXXX_2008 = rData.JTJXXX_2008; break;
		case D_SPFC_CODE_KBC2009:        KBC2009 = rData.KBC2009; break;
		case D_SPFC_CODE_TAIWANBRG98:    TAIWANBRG98 = rData.TAIWANBRG98; break;
		case D_SPFC_CODE_CH2010:         CH2010 = rData.CH2010; break;
		case D_SPFC_CODE_CJJ166_2011:    CJJ166_2011 = rData.CJJ166_2011; break;
		case D_SPFC_CODE_IBC2009:        IBC2009 = rData.IBC2009; break;
		case D_SPFC_CODE_IBC2012:        IBC2012 = rData.IBC2012; break;
		case D_SPFC_CODE_AASHTO_LRFD12:  AASHTO_LRFD12 = rData.AASHTO_LRFD12; break;
		case D_SPFC_CODE_NSR2010:        NSR10 = rData.NSR10; break;
		case D_SPFC_CODE_KBC2015:        KBC2016 = rData.KBC2016; break;
		case D_SPFC_CODE_P100_2013:      P100_2013 = rData.P100_2013; break;
		case D_SPFC_CODE_IS1893_2016:    IS2016 = rData.IS2016; break;
		case D_SPFC_CODE_KSCE2019:       KSCE2019 = rData.KSCE2019; break;
		case D_SPFC_CODE_NTC2018:        NTC2018 = rData.NTC2018; break;
		case D_SPFC_CODE_DPWH_LRFD_2013: DPWHLRFD2013 = rData.DPWHLRFD2013; break;
		case D_SPFC_CODE_KDS2019:        KDS2019 = rData.KDS2019; break;
		case D_SPFC_CODE_AS_5100_2_2017: AS510022017 = rData.AS510022017; break;
		case D_SPFC_CODE_JP_BRG2017:     JPBRG2017.Initialize(); break;
		case D_SPFC_CODE_DPT2018:        DPT2018.Init(); break;
		default: Initialize(nCode); break;
		}
	}
	void Convert955(int nCode, const T_SPFC_CODE_955& rData)
	{
		switch (nCode)
		{
		case D_SPFC_CODE_KS:             KS = rData.KS; break;
		case D_SPFC_CODE_KS2000:         KS2000 = rData.KS2000; break;
		case D_SPFC_CODE_KS_BRG:         KSBRG = rData.KSBRG; break;
		case D_SPFC_CODE_UBC88:          UBC88 = rData.UBC88; break;
		case D_SPFC_CODE_UBC97:          UBC97 = rData.UBC97; break;
		case D_SPFC_CODE_EURO1996:       EURO1996 = rData.EURO1996; break;
		case D_SPFC_CODE_CH2002:         CH2002 = rData.CH2002; break;
		case D_SPFC_CODE_IBC2000:        IBC2000 = rData.IBC2000; break;
		case D_SPFC_CODE_JPN2000:        JP2000 = rData.JP2000; break;
		case D_SPFC_CODE_CH_BRG89:       CHBRG89 = rData.CHBRG89; break;
		case D_SPFC_CODE_CH_GBJ111_87:   CHGBJ111_87 = rData.CHGBJ111_87; break;
		case D_SPFC_CODE_NBC95:          NBC95 = rData.NBC95; break;
		case D_SPFC_CODE_EURO1996_ELA:   EURO1996_ELA = rData.EURO1996_ELA; break;
		case D_SPFC_CODE_IS2002:         IS1893 = rData.IS1893; break;
		case D_SPFC_CODE_TAIWAN99H:      TAIWAN99H = rData.TAIWAN99H; break;
		case D_SPFC_CODE_TAIWAN99V:      TAIWAN99V = rData.TAIWAN99V; break;
		case D_SPFC_CODE_TAIWAN89H_BRG:  TAIWAN89H = rData.TAIWAN89H; break;
		case D_SPFC_CODE_TAIWAN89V_BRG:  TAIWAN89V = rData.TAIWAN89V; break;
		case D_SPFC_CODE_JP_BRG2002:     JPBRG2002 = rData.JPBRG2002; break;
		case D_SPFC_CODE_KBC2005:        KBC2005 = rData.KBC2005; break;
		case D_SPFC_CODE_CHSH2003:       CHSH2003 = rData.CHSH2003; break;
		case D_SPFC_CODE_EURO2004:       EURO2004 = rData.EURO2004; break;
		case D_SPFC_CODE_TAIWAN06:       TAIWAN06 = rData.TAIWAN06; break;
		case D_SPFC_CODE_GB50111_2006:   GB50111_2006 = rData.GB50111_2006; break;
		case D_SPFC_CODE_JTJXXX_2008:    JTJXXX_2008 = rData.JTJXXX_2008; break;
		case D_SPFC_CODE_KBC2009:        KBC2009 = rData.KBC2009; break;
		case D_SPFC_CODE_TAIWANBRG98:    TAIWANBRG98 = rData.TAIWANBRG98; break;
		case D_SPFC_CODE_CH2010:         CH2010 = rData.CH2010; break;
		case D_SPFC_CODE_CJJ166_2011:    CJJ166_2011 = rData.CJJ166_2011; break;
		case D_SPFC_CODE_IBC2009:        IBC2009 = rData.IBC2009; break;
		case D_SPFC_CODE_IBC2012:        IBC2012 = rData.IBC2012; break;
		case D_SPFC_CODE_AASHTO_LRFD12:  AASHTO_LRFD12 = rData.AASHTO_LRFD12; break;
		case D_SPFC_CODE_NSR2010:        NSR10 = rData.NSR10; break;
		case D_SPFC_CODE_KBC2015:        KBC2016 = rData.KBC2016; break;
		case D_SPFC_CODE_P100_2013:      P100_2013 = rData.P100_2013; break;
		case D_SPFC_CODE_IS1893_2016:    IS2016 = rData.IS2016; break;
		case D_SPFC_CODE_KSCE2019:       KSCE2019 = rData.KSCE2019; break;
		case D_SPFC_CODE_NTC2018:        NTC2018 = rData.NTC2018; break;
		case D_SPFC_CODE_DPWH_LRFD_2013: DPWHLRFD2013 = rData.DPWHLRFD2013; break;
		case D_SPFC_CODE_KDS2019:        KDS2019 = rData.KDS2019; break;
		case D_SPFC_CODE_AS_5100_2_2017: AS510022017 = rData.AS510022017; break;
		case D_SPFC_CODE_JP_BRG2017:     JPBRG2017=rData.JPBRG2017; break;
		case D_SPFC_CODE_DPT2018:        DPT2018=rData.DPT2018; break;
		case D_SPFC_CODE_SANS:			 SANS2010.Initialize(); break;
		default: Initialize(nCode); break;
		}
	}
};

///// ---------------  Spectrum Function Data (SPFC)  -----------------------------------------------------------
struct T_SPFC_BASE
{
	double dblPeriod;
	double dblValue;
	void Initialize()
	{
		dblPeriod=0.;
		dblValue=0.;
	}
};
#define T_SPFC_DT CArray<T_SPFC_BASE, T_SPFC_BASE&>
#define T_SPFC_K unsigned int

struct T_SPFC_D
{
	CString SpecFuncName;
	int nSpecFuncType;    // =1 Normalized Acceleration =2 Acceleration =3 Velocity =4 Displacement
	double ScaleFactor;
	double Gravity;  // Normalized Acceleration일때만 사용
	unsigned int nSpecFuncId;    // 순번
	T_SPFC_DT arSpecFuncData;
	CString Description;
	int nScaleMethod;     // 6.4.0추가 0=Scale Factor, 1=Maximum Value
	double dMaxValue;     // 6.4.0추가
	double dDampingRatio; // 6.5.0추가

	int nSpecCodeType;      // 0: User, 1: D_SPFC_CODE_??
	T_SPFC_CODE CodeParam;
	double dResModCoef;     // 7.7.0 추가     Response Modification Coef.
	double dC;              // 870 추가

	void Initialize()
	{
		SpecFuncName.Empty();
		nSpecFuncType=0;
		ScaleFactor=0.;
		Gravity=0.;
		nSpecFuncId=0;
		arSpecFuncData.RemoveAll();
		Description.Empty();
		nScaleMethod = 0;
		dMaxValue = 0;
		dDampingRatio = 0.05;
		nSpecCodeType = D_SPFC_CODE_UNDEFINED;
		dResModCoef = 1.0;
		dC = 1.0;
	}
	T_SPFC_D()
	{
	}
	T_SPFC_D(T_SPFC_D& src)
	{
		*this = src;
	}
	T_SPFC_D& operator=(T_SPFC_D& src)
	{
		SpecFuncName=src.SpecFuncName;
		nSpecFuncType=src.nSpecFuncType;
		ScaleFactor=src.ScaleFactor;
		Gravity=src.Gravity;
		nSpecFuncId=src.nSpecFuncId;
		arSpecFuncData.Copy(src.arSpecFuncData);
		Description=src.Description;
		nScaleMethod = src.nScaleMethod;
		dMaxValue = src.dMaxValue;
		dDampingRatio = src.dDampingRatio;
		nSpecCodeType = src.nSpecCodeType;    // MNET:2138-GSJI-20060529
		CodeParam     = src.CodeParam;
		dResModCoef   = src.dResModCoef;
		dC            = src.dC;
		return *this;
	}
};

struct T_SPFC_D_CH
{
	char SpecFuncName[64];  // 9.3.0 : 28 -> 64
	int nSpecFuncType;      // =1 Acceleration =2 Velocity =3 Displacement
	double ScaleFactor;
	double Gravity;
	unsigned int nSpecFuncId;    // 순번
	T_SPFC_DT arSpecFuncData;
	char Description[256];  // 9.3.0 : 100 -> 256
	int nScaleMethod;       // 6.4.0추가 0=Scale Factor, 1=Maximum Value
	double dMaxValue;       // 6.4.0추가
	double dDampingRatio;   // 6.5.0추가
	int nSpecCodeType;      // 0: User, 1~: D_SPFC_CODE_??    7.1.0 추가
	T_SPFC_CODE CodeParam;  //7.1.0 추가
	double dResModCoef;        // 7.7.0 추가     Response Modification Coef.
	double dC; // 870

	void ConvertToString(T_SPFC_D& rData)
	{
		ConvertCharStr(SpecFuncName, rData.SpecFuncName, sizeof(SpecFuncName));
		rData.nSpecFuncType = nSpecFuncType;
		rData.ScaleFactor = ScaleFactor;
		rData.Gravity = Gravity;
		rData.nSpecFuncId = nSpecFuncId;
		rData.arSpecFuncData.Copy(arSpecFuncData);
		ConvertCharStr(Description, rData.Description, sizeof(Description));
		rData.nScaleMethod = nScaleMethod;
		rData.dMaxValue = dMaxValue;
		rData.dDampingRatio = dDampingRatio;
		rData.nSpecCodeType = nSpecCodeType;
		rData.CodeParam     = CodeParam;
		rData.dResModCoef   = dResModCoef;
		rData.dC            = dC;
	}
	void ConvertToChar(T_SPFC_D& rData)
	{
		ConvertStrChar(rData.SpecFuncName, SpecFuncName, sizeof(SpecFuncName));
		nSpecFuncType = rData.nSpecFuncType;
		ScaleFactor = rData.ScaleFactor;
		Gravity = rData.Gravity;
		nSpecFuncId = rData.nSpecFuncId;
		arSpecFuncData.Copy(rData.arSpecFuncData);
		ConvertStrChar(rData.Description, Description, sizeof(Description));
		nScaleMethod = rData.nScaleMethod;
		dMaxValue = rData.dMaxValue;
		dDampingRatio = rData.dDampingRatio;
		nSpecCodeType = rData.nSpecCodeType;
		CodeParam     = rData.CodeParam;
		dResModCoef   = rData.dResModCoef;
		dC            = rData.dC;
	}
};

struct T_SPFC_UDRD_D
{
	T_SPFC_K key;
	T_SPFC_D data;
};

struct T_SPFC_UDRD_D_CH
{
	T_SPFC_K key;
	T_SPFC_D_CH data;
};

struct T_SPFC_UDRD_D_CH_RW_431
{
	T_SPFC_K key;
	char SpecFuncName[20];
	int nSpecFuncType;    // =1 Acceleration =2 Velocity =3 Displacement
	double ScaleFactor;
	double Gravity;
	unsigned int nSpecFuncId;    // 순번
};

struct T_SPFC_UDRD_D_CH_RW_632
{
	T_SPFC_K key;
	char SpecFuncName[20];
	int nSpecFuncType;    // =1 Acceleration =2 Velocity =3 Displacement
	double ScaleFactor;
	double Gravity;
	unsigned int nSpecFuncId;    // 순번
	char Description[80];
};

struct T_SPFC_UDRD_D_CH_RW_640
{
	T_SPFC_K key;
	char SpecFuncName[20];
	int nSpecFuncType;    // =1 Acceleration =2 Velocity =3 Displacement
	double ScaleFactor;
	double Gravity;
	unsigned int nSpecFuncId;    // 순번
	char Description[80];
	int nScaleMethod;     // 0=Scale Factor, 1=Maximum Value
	double dMaxValue;
};

struct T_SPFC_UDRD_D_CH_RW_641
{
	T_SPFC_K key;
	char SpecFuncName[20];
	int nSpecFuncType;    // =1 Acceleration =2 Velocity =3 Displacement
	double ScaleFactor;
	double Gravity;
	unsigned int nSpecFuncId;    // 순번
	char Description[80];
	int nScaleMethod;     // 0=Scale Factor, 1=Maximum Value
	double dMaxValue;
	double dDampingRatio; // 6.5.0추가
};

struct T_SPFC_UDRD_D_CH_RW_702
{
	T_SPFC_K key;
	char SpecFuncName[28];  // 6.5.0에서 28로 증가
	int nSpecFuncType;    // =1 Acceleration =2 Velocity =3 Displacement
	double ScaleFactor;
	double Gravity;
	unsigned int nSpecFuncId;    // 순번
	char Description[80];
	int nScaleMethod;     // 0=Scale Factor, 1=Maximum Value
	double dMaxValue;
	double dDampingRatio; // 6.5.0추가
	//#//int nSpecCodeType;    // 0: User, 1: D_SPFC_CODE_??  //7.1.0 추가
	//#//T_SPFC_CODE CodeParam;//7.1.0 추가
};

struct T_SPFC_UDRD_D_CH_RW_761
{
	T_SPFC_K key;
	char SpecFuncName[28];  // 6.5.0에서 28로 증가
	int nSpecFuncType;      // =1 Acceleration =2 Velocity =3 Displacement
	double ScaleFactor;
	double Gravity;
	unsigned int nSpecFuncId;    // 순번
	char Description[80];
	int nScaleMethod;       // 0=Scale Factor, 1=Maximum Value
	double dMaxValue;
	double dDampingRatio;   // 6.5.0추가
	int nSpecCodeType;      //7.1.0 추가
	T_SPFC_CODE_770 CodeParam;  //7.1.0 추가
};

struct T_SPFC_UDRD_D_CH_RW_770
{
	T_SPFC_K key;
	char SpecFuncName[28];  // 6.5.0에서 28로 증가
	int nSpecFuncType;      // =1 Acceleration =2 Velocity =3 Displacement
	double ScaleFactor;
	double Gravity;
	unsigned int nSpecFuncId;    // 순번
	char Description[80];
	int nScaleMethod;       // 0=Scale Factor, 1=Maximum Value
	double dMaxValue;
	double dDampingRatio;   // 6.5.0추가
	int nSpecCodeType;      //7.1.0 추가
	T_SPFC_CODE_770 CodeParam;  //7.1.0 추가
	double dResModCoef;        // 7.7.0 추가     Response Modification Coef.
};

struct T_SPFC_UDRD_D_CH_RW_835
{
	T_SPFC_K key;
	char SpecFuncName[28];
	int nSpecFuncType;
	double ScaleFactor;
	double Gravity;
	unsigned int nSpecFuncId;
	char Description[80];
	int nScaleMethod;
	double dMaxValue;
	double dDampingRatio;
	int nSpecCodeType;
	T_SPFC_CODE_835 CodeParam;
	double dResModCoef;
};

struct T_SPFC_UDRD_D_CH_RW_851
{
	T_SPFC_K key;
	char SpecFuncName[28];  // 6.5.0에서 28로 증가
	int nSpecFuncType;      // =1 Acceleration =2 Velocity =3 Displacement
	double ScaleFactor;
	double Gravity;
	unsigned int nSpecFuncId;    // 순번
	char Description[80];
	int nScaleMethod;       // 0=Scale Factor, 1=Maximum Value
	double dMaxValue;
	double dDampingRatio;   // 6.5.0추가
	int nSpecCodeType;      //7.1.0 추가
	T_SPFC_CODE_871 CodeParam;
	double dResModCoef;        // 7.7.0 추가     Response Modification Coef.
};

struct T_SPFC_UDRD_D_CH_RW_865
{
	T_SPFC_K key;
	char SpecFuncName[28];  // 6.5.0에서 28로 증가
	int nSpecFuncType;      // =1 Acceleration =2 Velocity =3 Displacement
	double ScaleFactor;
	double Gravity;
	unsigned int nSpecFuncId;    // 순번
	char Description[100];
	int nScaleMethod;       // 0=Scale Factor, 1=Maximum Value
	double dMaxValue;
	double dDampingRatio;   // 6.5.0추가
	int nSpecCodeType;      //7.1.0 추가
	T_SPFC_CODE_871 CodeParam;  //7.1.0 추가
	double dResModCoef;        // 7.7.0 추가     Response Modification Coef.
};

struct T_SPFC_UDRD_D_CH_RW_871
{
	T_SPFC_K key;
	char SpecFuncName[28];  // 6.5.0에서 28로 증가
	int nSpecFuncType;      // =1 Acceleration =2 Velocity =3 Displacement
	double ScaleFactor;
	double Gravity;
	unsigned int nSpecFuncId;    // 순번
	char Description[100];
	int nScaleMethod;       // 0=Scale Factor, 1=Maximum Value
	double dMaxValue;
	double dDampingRatio;   // 6.5.0추가
	int nSpecCodeType;      //7.1.0 추가
	T_SPFC_CODE_871 CodeParam;  //8.7.1 추가
	double dResModCoef;        // 7.7.0 추가     Response Modification Coef.
	double dC;
};

struct T_SPFC_UDRD_D_CH_RW_881
{
	T_SPFC_K key;
	char SpecFuncName[28];  // 6.5.0에서 28로 증가
	int nSpecFuncType;      // =1 Acceleration =2 Velocity =3 Displacement
	double ScaleFactor;
	double Gravity;
	unsigned int nSpecFuncId;    // 순번
	char Description[100];
	int nScaleMethod;       // 0=Scale Factor, 1=Maximum Value
	double dMaxValue;
	double dDampingRatio;   // 6.5.0추가
	int nSpecCodeType;      //7.1.0 추가
	T_SPFC_CODE_881 CodeParam;  //7.1.0 추가
	double dResModCoef;        // 7.7.0 추가     Response Modification Coef.
	double dC;
};

struct T_SPFC_UDRD_D_CH_RW_883
{
	T_SPFC_K key;
	char SpecFuncName[28];  // 6.5.0에서 28로 증가
	int nSpecFuncType;      // =1 Acceleration =2 Velocity =3 Displacement
	double ScaleFactor;
	double Gravity;
	unsigned int nSpecFuncId;    // 순번
	char Description[100];
	int nScaleMethod;       // 0=Scale Factor, 1=Maximum Value
	double dMaxValue;
	double dDampingRatio;   // 6.5.0추가
	int nSpecCodeType;      //7.1.0 추가
	T_SPFC_CODE_883 CodeParam;
	double dResModCoef;        // Response Modification Coef.
	double dC;
};

struct T_SPFC_UDRD_D_CH_RW_895
{
	T_SPFC_K key;
	char SpecFuncName[28];  // 6.5.0에서 28로 증가
	int nSpecFuncType;      // =1 Acceleration =2 Velocity =3 Displacement
	double ScaleFactor;
	double Gravity;
	unsigned int nSpecFuncId;    // 순번
	char Description[100];
	int nScaleMethod;       // 0=Scale Factor, 1=Maximum Value
	double dMaxValue;
	double dDampingRatio;   // 6.5.0추가
	int nSpecCodeType;      //7.1.0 추가
	T_SPFC_CODE_895 CodeParam;  //7.1.0 추가
	double dResModCoef;        // 7.7.0 추가     Response Modification Coef.
	double dC;
};

struct T_SPFC_UDRD_D_CH_RW_925_1
{
	T_SPFC_K key;
	char SpecFuncName[28];  // 6.5.0에서 28로 증가
	int nSpecFuncType;      // =1 Acceleration =2 Velocity =3 Displacement
	double ScaleFactor;
	double Gravity;
	unsigned int nSpecFuncId; // 순번
	char Description[100];
	int nScaleMethod;       // 0=Scale Factor, 1=Maximum Value
	double dMaxValue;
	double dDampingRatio;   // 6.5.0 추가
	int nSpecCodeType;      // 7.1.0 추가
	T_SPFC_CODE CodeParam;  // 7.1.0 추가
	double dResModCoef;     // 7.7.0 추가 Response Modification Coefficient.
	double dC;
};

struct T_SPFC_UDRD_D_CH_RW_925
{
	T_SPFC_K key;
	char SpecFuncName[28];  // 6.5.0에서 28로 증가
	int nSpecFuncType;      // =1 Acceleration =2 Velocity =3 Displacement
	double ScaleFactor;
	double Gravity;
	unsigned int nSpecFuncId; // 순번
	char Description[100];
	int nScaleMethod;       // 0=Scale Factor, 1=Maximum Value
	double dMaxValue;
	double dDampingRatio;   // 6.5.0 추가
	int nSpecCodeType;      // 7.1.0 추가
	T_SPFC_CODE_925 CodeParam;  // 7.1.0 추가
	double dResModCoef;     // 7.7.0 추가 Response Modification Coefficient.
	double dC;
};

struct T_SPFC_UDRD_D_CH_RW_955
{
	T_SPFC_K key;
	char SpecFuncName[28];  // 6.5.0에서 28로 증가
	int nSpecFuncType;      // =1 Acceleration =2 Velocity =3 Displacement
	double ScaleFactor;
	double Gravity;
	unsigned int nSpecFuncId; // 순번
	char Description[100];
	int nScaleMethod;       // 0=Scale Factor, 1=Maximum Value
	double dMaxValue;
	double dDampingRatio;   // 6.5.0 추가
	int nSpecCodeType;      // 7.1.0 추가
	T_SPFC_CODE_955 CodeParam;  // 7.1.0 추가
	double dResModCoef;     // 7.7.0 추가 Response Modification Coefficient.
	double dC;
};

struct T_SPFC_UDRD_D_CH_RW
{
	T_SPFC_K key;
	char SpecFuncName[64];  // 9.3.0 : 28 -> 64
	int nSpecFuncType;      // =1 Acceleration =2 Velocity =3 Displacement
	double ScaleFactor;
	double Gravity;
	unsigned int nSpecFuncId; // 순번
	char Description[256];  // 9.3.0 : 100 -> 256
	int nScaleMethod;       // 0=Scale Factor, 1=Maximum Value
	double dMaxValue;
	double dDampingRatio;   // 6.5.0 추가
	int nSpecCodeType;      // 7.1.0 추가
	T_SPFC_CODE CodeParam;  // 7.1.0 추가
	double dResModCoef;     // 7.7.0 추가 Response Modification Coefficient.
	double dC;

	void GetSpfc(T_SPFC_K& rKey, T_SPFC_D_CH& rData)
	{
		rKey=key;
		memcpy(rData.SpecFuncName, SpecFuncName, sizeof(SpecFuncName));
		rData.nSpecFuncType = nSpecFuncType;
		rData.ScaleFactor = ScaleFactor;
		rData.Gravity = Gravity;
		rData.nSpecFuncId = nSpecFuncId;
		memcpy(rData.Description, Description, sizeof(Description));
		rData.nScaleMethod = nScaleMethod;
		rData.dMaxValue = dMaxValue;
		rData.dDampingRatio = dDampingRatio;
		rData.nSpecCodeType = nSpecCodeType;
		rData.CodeParam     = CodeParam;
		rData.dResModCoef   = dResModCoef;
		rData.dC            = dC;
	}
	void SetSpfc(const T_SPFC_K Key, const T_SPFC_D_CH& rData)
	{
		key = Key;
		memcpy(SpecFuncName, rData.SpecFuncName, sizeof(SpecFuncName));
		nSpecFuncType = rData.nSpecFuncType; 
		ScaleFactor   = rData.ScaleFactor;   
		Gravity       = rData.Gravity;       
		nSpecFuncId   = rData.nSpecFuncId;   
		memcpy(Description, rData.Description, sizeof(Description));
		nScaleMethod  = rData.nScaleMethod;  
		dMaxValue     = rData.dMaxValue;     
		dDampingRatio = rData.dDampingRatio; 
		nSpecCodeType = rData.nSpecCodeType; 
		CodeParam     = rData.CodeParam;     
		dResModCoef   = rData.dResModCoef;   
		dC            = rData.dC;            
	}
	void Convert955(const T_SPFC_UDRD_D_CH_RW_955& rData)
	{
		memset(SpecFuncName, 0, sizeof(SpecFuncName));
		memset(Description, 0, sizeof(Description));
		key = rData.key;
		memcpy(SpecFuncName, rData.SpecFuncName, sizeof(rData.SpecFuncName));
		memcpy(Description, rData.Description, sizeof(rData.Description));
		nSpecFuncType = rData.nSpecFuncType;
		ScaleFactor = rData.ScaleFactor;
		Gravity = rData.Gravity;
		nSpecFuncId = rData.nSpecFuncId;
		nScaleMethod = rData.nScaleMethod;
		dMaxValue = rData.dMaxValue;
		dDampingRatio = rData.dDampingRatio;
		nSpecCodeType = rData.nSpecCodeType;
		CodeParam.Convert955(nSpecCodeType, rData.CodeParam);
		dResModCoef = rData.dResModCoef;
		dC = rData.dC;
	}
	void Convert925(const T_SPFC_UDRD_D_CH_RW_925& rData)
	{
		memset(SpecFuncName, 0, sizeof(SpecFuncName));
		memset(Description, 0, sizeof(Description));
		key = rData.key;
		memcpy(SpecFuncName, rData.SpecFuncName, sizeof(rData.SpecFuncName));
		memcpy(Description, rData.Description, sizeof(rData.Description));
		nSpecFuncType = rData.nSpecFuncType;
		ScaleFactor   = rData.ScaleFactor;
		Gravity       = rData.Gravity;
		nSpecFuncId   = rData.nSpecFuncId;
		nScaleMethod  = rData.nScaleMethod;
		dMaxValue     = rData.dMaxValue;
		dDampingRatio = rData.dDampingRatio;
		nSpecCodeType = rData.nSpecCodeType;
		CodeParam.Convert925(nSpecCodeType, rData.CodeParam);
		dResModCoef = rData.dResModCoef;
		dC          = rData.dC;
	}
	void Convert925_1(const T_SPFC_UDRD_D_CH_RW_925_1& rData)
	{
		memset(SpecFuncName, 0, sizeof(SpecFuncName));
		memset(Description, 0, sizeof(Description));
		key = rData.key;
		memcpy(SpecFuncName, rData.SpecFuncName, sizeof(rData.SpecFuncName));
		memcpy(Description, rData.Description, sizeof(rData.Description));
		nSpecFuncType = rData.nSpecFuncType;
		ScaleFactor   = rData.ScaleFactor;
		Gravity       = rData.Gravity;
		nSpecFuncId   = rData.nSpecFuncId;
		nScaleMethod  = rData.nScaleMethod;
		dMaxValue     = rData.dMaxValue;
		dDampingRatio = rData.dDampingRatio;
		nSpecCodeType = rData.nSpecCodeType;
		CodeParam     = rData.CodeParam;
		dResModCoef   = rData.dResModCoef;
		dC            = rData.dC;
	}
	void Convert895(T_SPFC_UDRD_D_CH_RW_895& rData)
	{
		key = rData.key;
		memcpy(SpecFuncName, rData.SpecFuncName, sizeof(rData.SpecFuncName));
		nSpecFuncType = rData.nSpecFuncType;
		ScaleFactor = rData.ScaleFactor;
		Gravity = rData.Gravity;
		nSpecFuncId = rData.nSpecFuncId;
		memcpy(Description, rData.Description, sizeof(rData.Description));
		nScaleMethod = rData.nScaleMethod;
		dMaxValue = rData.dMaxValue;
		dDampingRatio = rData.dDampingRatio;
		nSpecCodeType = rData.nSpecCodeType;
		CodeParam.Convert895(nSpecCodeType, rData.CodeParam);
		dResModCoef = rData.dResModCoef;
		dC          = rData.dC;
	}
	void Convert883(T_SPFC_UDRD_D_CH_RW_883& rData)
	{
		key = rData.key;
		memcpy(SpecFuncName, rData.SpecFuncName, sizeof(rData.SpecFuncName));
		nSpecFuncType = rData.nSpecFuncType;
		ScaleFactor = rData.ScaleFactor;
		Gravity = rData.Gravity;
		nSpecFuncId = rData.nSpecFuncId;
		memcpy(Description, rData.Description, sizeof(rData.Description));
		nScaleMethod = rData.nScaleMethod;
		dMaxValue = rData.dMaxValue;
		dDampingRatio = rData.dDampingRatio;
		nSpecCodeType = rData.nSpecCodeType;
		CodeParam.Convert883(nSpecCodeType, rData.CodeParam);
		dResModCoef = rData.dResModCoef;
		dC          = rData.dC;
	}
	void Convert881(T_SPFC_UDRD_D_CH_RW_881& rData)
	{
		key = rData.key;
		memcpy(SpecFuncName, rData.SpecFuncName, sizeof(rData.SpecFuncName));
		nSpecFuncType = rData.nSpecFuncType;
		ScaleFactor = rData.ScaleFactor;
		Gravity = rData.Gravity;
		nSpecFuncId = rData.nSpecFuncId;
		memcpy(Description, rData.Description, sizeof(rData.Description));
		nScaleMethod = rData.nScaleMethod;
		dMaxValue = rData.dMaxValue;
		dDampingRatio = rData.dDampingRatio;
		nSpecCodeType = rData.nSpecCodeType;
		CodeParam.Convert881(nSpecCodeType, rData.CodeParam);
		dResModCoef = rData.dResModCoef;
		dC          = rData.dC;
	}
	void Convert871(T_SPFC_UDRD_D_CH_RW_871& rData)
	{
		key = rData.key;
		memcpy(SpecFuncName, rData.SpecFuncName, sizeof(rData.SpecFuncName));
		nSpecFuncType = rData.nSpecFuncType;
		ScaleFactor = rData.ScaleFactor;
		Gravity = rData.Gravity;
		nSpecFuncId = rData.nSpecFuncId;
		memcpy(Description, rData.Description, sizeof(rData.Description));
		nScaleMethod = rData.nScaleMethod;
		dMaxValue = rData.dMaxValue;
		dDampingRatio = rData.dDampingRatio;
		nSpecCodeType = rData.nSpecCodeType;
		CodeParam.Convert871(nSpecCodeType, rData.CodeParam);
		dResModCoef = rData.dResModCoef;
		dC          = rData.dC;
	}
	void Convert865(T_SPFC_UDRD_D_CH_RW_865& rData)
	{
		key = rData.key;
		memcpy(SpecFuncName, rData.SpecFuncName, sizeof(rData.SpecFuncName));
		nSpecFuncType = rData.nSpecFuncType;
		ScaleFactor = rData.ScaleFactor;
		Gravity = rData.Gravity;
		nSpecFuncId = rData.nSpecFuncId;
		memcpy(Description, rData.Description, sizeof(rData.Description));
		nScaleMethod = rData.nScaleMethod;
		dMaxValue = rData.dMaxValue;
		dDampingRatio = rData.dDampingRatio;
		nSpecCodeType = rData.nSpecCodeType;
		CodeParam.Convert871(nSpecCodeType, rData.CodeParam);
		dResModCoef = rData.dResModCoef;
		dC          = 1.0;
	}
	void Convert851(T_SPFC_UDRD_D_CH_RW_851& rData)
	{
		key = rData.key;
		memset(SpecFuncName, 0, sizeof(SpecFuncName));
		memcpy(SpecFuncName, rData.SpecFuncName, sizeof(rData.SpecFuncName));
		nSpecFuncType = rData.nSpecFuncType;
		ScaleFactor = rData.ScaleFactor;
		Gravity = rData.Gravity;
		nSpecFuncId = rData.nSpecFuncId;
		memset(Description, 0, sizeof(Description));
		memcpy(Description, rData.Description, sizeof(rData.Description));
		nScaleMethod = rData.nScaleMethod;
		dMaxValue = rData.dMaxValue;
		dDampingRatio = rData.dDampingRatio;
		nSpecCodeType = rData.nSpecCodeType;
		CodeParam.Convert871(nSpecCodeType, rData.CodeParam);
		dResModCoef = rData.dResModCoef;
		dC          = 1.0;
	}
	void Convert835(T_SPFC_UDRD_D_CH_RW_835& rData)
	{
		key = rData.key;
		memset(SpecFuncName, 0, sizeof(SpecFuncName));
		memcpy(SpecFuncName, rData.SpecFuncName, sizeof(rData.SpecFuncName));
		nSpecFuncType = rData.nSpecFuncType;
		ScaleFactor = rData.ScaleFactor;
		Gravity = rData.Gravity;
		nSpecFuncId = rData.nSpecFuncId;
		memcpy(Description, rData.Description, sizeof(Description));
		nScaleMethod = rData.nScaleMethod;
		dMaxValue = rData.dMaxValue;
		dDampingRatio = rData.dDampingRatio;
		nSpecCodeType = rData.nSpecCodeType;
		CodeParam.Convert835(rData.nSpecCodeType, rData.CodeParam);
		dResModCoef = rData.dResModCoef;
		dC          = 1.0;
	}
	void Convert770(T_SPFC_UDRD_D_CH_RW_770& rData)
	{
		key = rData.key;
		memset(SpecFuncName, 0, sizeof(SpecFuncName));
		memcpy(SpecFuncName, rData.SpecFuncName, sizeof(rData.SpecFuncName));
		nSpecFuncType = rData.nSpecFuncType;
		ScaleFactor = rData.ScaleFactor;
		Gravity = rData.Gravity;
		nSpecFuncId = rData.nSpecFuncId;
		memcpy(Description, rData.Description, sizeof(Description));
		nScaleMethod = rData.nScaleMethod;
		dMaxValue = rData.dMaxValue;
		dDampingRatio = rData.dDampingRatio;
		nSpecCodeType = rData.nSpecCodeType;
		CodeParam.Convert770(rData.nSpecCodeType, rData.CodeParam);
		dResModCoef = rData.dResModCoef;
		dC          = 1.0;
	}
	void Convert761(T_SPFC_UDRD_D_CH_RW_761& rData)
	{
		key = rData.key;
		memset(SpecFuncName, 0, sizeof(SpecFuncName));
		memcpy(SpecFuncName, rData.SpecFuncName, sizeof(rData.SpecFuncName));
		nSpecFuncType = rData.nSpecFuncType;
		ScaleFactor = rData.ScaleFactor;
		Gravity = rData.Gravity;
		nSpecFuncId = rData.nSpecFuncId;
		memcpy(Description, rData.Description, sizeof(Description));
		nScaleMethod = rData.nScaleMethod;
		dMaxValue = rData.dMaxValue;
		dDampingRatio = rData.dDampingRatio;
		nSpecCodeType = rData.nSpecCodeType;
		CodeParam.Convert770(rData.nSpecCodeType, rData.CodeParam);
		dResModCoef = 1.0;
		dC          = 1.0;
	}
	void Convert702(T_SPFC_UDRD_D_CH_RW_702& rData)
	{
		key = rData.key;
		memset(SpecFuncName, 0, sizeof(SpecFuncName));
		memcpy(SpecFuncName, rData.SpecFuncName, sizeof(rData.SpecFuncName));
		nSpecFuncType = rData.nSpecFuncType;
		ScaleFactor = rData.ScaleFactor;
		Gravity = rData.Gravity;
		nSpecFuncId = rData.nSpecFuncId;
		memcpy(Description, rData.Description, sizeof(Description));
		nScaleMethod = rData.nScaleMethod;
		dMaxValue = rData.dMaxValue;
		dDampingRatio = rData.dDampingRatio;
		nSpecCodeType = D_SPFC_CODE_UNDEFINED;
		dResModCoef = 1.0;
		dC          = 1.0;
	}
	void Convert641(T_SPFC_UDRD_D_CH_RW_641& rData)
	{
		key = rData.key;
		memset(SpecFuncName, 0, sizeof(SpecFuncName));
		memcpy(SpecFuncName, rData.SpecFuncName, sizeof(rData.SpecFuncName));
		nSpecFuncType = rData.nSpecFuncType;
		ScaleFactor = rData.ScaleFactor;
		Gravity = rData.Gravity;
		nSpecFuncId = rData.nSpecFuncId;
		memcpy(Description, rData.Description, sizeof(Description));
		nScaleMethod = rData.nScaleMethod;
		dMaxValue = rData.dMaxValue;
		dDampingRatio = rData.dDampingRatio;
		nSpecCodeType = D_SPFC_CODE_UNDEFINED;
		dResModCoef = 1.0;
		dC          = 1.0;
	}
	void Convert640(T_SPFC_UDRD_D_CH_RW_640& rData)
	{
		key = rData.key;
		memset(SpecFuncName, 0, sizeof(SpecFuncName));
		memcpy(SpecFuncName, rData.SpecFuncName, sizeof(rData.SpecFuncName));
		nSpecFuncType = rData.nSpecFuncType;
		ScaleFactor = rData.ScaleFactor;
		Gravity = rData.Gravity;
		nSpecFuncId = rData.nSpecFuncId;
		memcpy(Description, rData.Description, sizeof(Description));
		nScaleMethod = rData.nScaleMethod;
		dMaxValue = rData.dMaxValue;
		dDampingRatio = 0.;
		nSpecCodeType = D_SPFC_CODE_UNDEFINED;
		dResModCoef = 1.0;
		dC          = 1.0;
	}
	void Convert632(T_SPFC_UDRD_D_CH_RW_632& rData)
	{
		key = rData.key;
		memset(SpecFuncName, 0, sizeof(SpecFuncName));
		memcpy(SpecFuncName, rData.SpecFuncName, sizeof(rData.SpecFuncName));
		nSpecFuncType = rData.nSpecFuncType;
		ScaleFactor = rData.ScaleFactor;
		Gravity = rData.Gravity;
		nSpecFuncId = rData.nSpecFuncId;
		memcpy(Description, rData.Description, sizeof(Description));
		nScaleMethod = 0;
		dMaxValue = 0;
		dDampingRatio = 0.;
		nSpecCodeType = D_SPFC_CODE_UNDEFINED;
		dResModCoef = 1.0;
		dC          = 1.0;
	}
	void Convert431(T_SPFC_UDRD_D_CH_RW_431& rData)
	{
		key = rData.key;
		memset(SpecFuncName, 0, sizeof(SpecFuncName));
		memcpy(SpecFuncName, rData.SpecFuncName, sizeof(rData.SpecFuncName));
		nSpecFuncType = rData.nSpecFuncType;
		ScaleFactor = rData.ScaleFactor;
		Gravity = rData.Gravity;
		nSpecFuncId = rData.nSpecFuncId;
		memset(Description, 0, sizeof(Description));
		nScaleMethod = 0;
		dMaxValue = 0;
		dDampingRatio = 0.;
		nSpecCodeType = D_SPFC_CODE_UNDEFINED;
		dResModCoef = 1.0;
		dC          = 1.0;
	}
};

struct T_SPFC_UNIT
{
	T_SPFC_UNIT()
	{
		nCount=10;
		nUnit[0]=D_UNITSYS_NONE;
		nUnit[1]=D_UNITSYS_NONE;
		nUnit[2]=D_UNITSYS_NONE;
		nUnit[3]=D_UNITSYS_BASE_ACCELERATION;
		nUnit[4]=D_UNITSYS_NONE;
		nUnit[5]=D_UNITSYS_BASE_TIME;
		nUnit[6]=D_UNITSYS_NONE;
		nUnit[7]=D_UNITSYS_BASE_ACCELERATION;
		nUnit[8]=D_UNITSYS_BASE_VELOCITY;
		nUnit[9]=D_UNITSYS_BASE_LENGTH;
		SpecFuncName=nUnit[0];
		nSpecFuncType=nUnit[1];
		ScaleFactor=nUnit[2];
		Gravity=nUnit[3];
		nSpecFuncId=nUnit[4];
		dblPeriod=nUnit[5];
		dblValue[0]=D_UNITSYS_NONE;                // if nSpecFuncType ==1;
		dblValue[1]=D_UNITSYS_BASE_ACCELERATION;   // if nSpecFuncType ==2;
		dblValue[2]=D_UNITSYS_BASE_VELOCITY;       // if nSpecFuncType ==3;
		dblValue[3]=D_UNITSYS_BASE_LENGTH;         // if nSpecFuncType ==4;
		dResModCoef = D_UNITSYS_NONE;
	}
	int SpecFuncName;
	int nSpecFuncType;
	int ScaleFactor;
	int Gravity;
	int nSpecFuncId;
	int dblPeriod;
	int dblValue[4];
	int nCount;
	int nUnit[10];
	int dResModCoef;
};

#define HASHSIZESPFC 21
#define D_SPFC_MINNUM       1
#define D_SPFC_MAXNUM      20

///// ---------------  Spectrum Analysis Control Data -----------------------------------------------------------
#define T_SPCT_K unsigned int
struct T_SPCT_D_413
{
	int nSpecCombType;      // 1=SRSS 2=CQC 3=ABS
	double dblDamping;      // nSpecCombType == 2 인 경우에만 사용
};

struct T_SPCT_D_414
{
	int nSpecCombType;      // 1=SRSS 2=CQC 3=ABS
	double dblDamping;      // nSpecCombType == 2 인 경우에만 사용
	BOOL bResultSign;
};

struct T_SPCT_D
{
	int nSpecCombType;      // 1=SRSS 2=CQC 3=ABS
	double dblDamping;      // nSpecCombType == 2 인 경우에만 사용
	BOOL bResultSign;
	int nSignType;        // 0=Major Mode, 1=Maximum Value
	void Initialize()
	{
		nSpecCombType=1;
	#if defined(_CIVIL)
	#if defined(_JP)
		nSpecCombType = 2;
	#endif
	#endif
		dblDamping=0.;
		bResultSign=FALSE;
		nSignType=0;
	}
	void Convert413(T_SPCT_D_413& rData)
	{
		nSpecCombType=rData.nSpecCombType;
		dblDamping=rData.dblDamping;
		bResultSign=FALSE;
		nSignType=0;
	}
	void Convert414(T_SPCT_D_414& rData)
	{
		nSpecCombType=rData.nSpecCombType;
		dblDamping=rData.dblDamping;
		bResultSign=rData.bResultSign;
		nSignType=0;
	}
};

struct T_SPCT_UDRD_D_413
{
	T_SPCT_D_413 data;
};

struct T_SPCT_UDRD_D_414
{
	T_SPCT_D_414 data;
};

struct T_SPCT_UDRD_D
{
	T_SPCT_D data;
};

struct T_SPCT_UNIT
{
	T_SPCT_UNIT()
	{
		nSpecCombType=D_UNITSYS_NONE;
		dblDamping=D_UNITSYS_NONE;
		nResultSign=D_UNITSYS_NONE;
		nSignType=D_UNITSYS_NONE;
	}
	int nSpecCombType;
	int dblDamping;
	int nResultSign;
	int nSignType;
};
#define HASHSIZESPCT 1

///// ---------------  Spectrum Load Case (SPLC) ----------------------------------------------------------------
struct T_SPLC_ECCEN
{
	T_STOR_K KeyStor;
	double dEccentricity;
	void Initialize()
	{
		KeyStor=0;
		dEccentricity=0.;
	}
};

#define T_SPLC_K unsigned int
typedef CArray<T_SPLC_K, T_SPLC_K> T_SPLC_K_LIST;
struct T_SPLC_D_590
{
	CString LoadCaseName;
	CString Description;
	unsigned int nSpecFuncKey;
	int nDirection;  // 1=X-Y  2=Z
	double dblAngle; // nDirection == 1 인 경우에만 사용
	double dblFactor;
	unsigned int LoadCaseId;    // 순번
	void Initialize()
	{
		LoadCaseName.Empty();
		Description.Empty();
		nSpecFuncKey=0;
		nDirection=0;
		dblAngle=0;
		dblFactor=0;
		LoadCaseId=0;
	}
};

struct T_SPLC_D
{
	CString LoadCaseName;
	CString Description;
	//  unsigned int nSpecFuncKey;   // 6.6.0부터 사용되지 않음
	int nDirection;  // 1=X-Y  2=Z
	double dblAngle; // nDirection == 1 인 경우에만 사용
	double dblFactor;
	unsigned int LoadCaseId;    // 순번
	double dPeriodModifyFactor; // 6.0.0에서 추가
	BOOL bConsiderEccen;        // 6.0.0에서 추가  Consider Accidental Eccentricity
	int  nEccenMethod;          // 6.0.0에서 추가  0: Linear Sum, 1: SRSS
	int  nAutomatic;            // 6.0.0에서 추가  0: Automatic,  1: User Defined
	double dEccenPercent;       // 6.0.0에서 추가
	CArray<T_SPLC_ECCEN, T_SPLC_ECCEN&> aSplcEccen;  // 6.0.0에서 추가
	BOOL bCorrectDamping;   // 6.5.0에서 추가
	int nDampingType;       // 6.5.0에서 추가 : 1=Direct Modal, 2=Mass and Stiffness Proportional, 3=Group
	double dDampAll;        // 6.5.0에서 추가 : nDampingType == Direct Modal인 경우 Damping Ratio for All Modes
	T_THIS_DT aDampData;    // 6.5.0에서 추가 : nDampingType == Direct Modal인 경우 Damping Data
	T_THIS_PROPDAMPING PropDamping;   // 6.5.0에서 추가 : nDampingType == Mass and Stiffness Proportional인 경우 Damping Data
	int  nInterpolateType;  // 6.5.0에서 추가 : 0=Linear, 1=Logarithm
	CArray<T_SPFC_K, T_SPFC_K> aSpfcKey;  // 6.6.0에서 추가: 기존의 SPFC key하나만 받는 것에서 여러개 받는 것으로 변경
	BOOL bApplyDampingMethod;  // 6.7.0 추가
	int nSpecCombType;      // 7.0.2 추가 0=SRSS 1=CQC 2=ABS 3=Linear
	BOOL bResultSign;       // 7.0.2 추가
	int nSignType;        // 7.0.2 추가 0=Major Mode, 1=Maximum Value
	BOOL bSelectUse;     //7.0.2 추가
	CArray<BOOL, BOOL> aUseMode;    // 7.0.2 추가 각 모드의 사용여부
	CArray<double, double> aMSFactor;   // 7.0.2 추가 각 모드의 Mode Shape Factor

	BOOL bAutoSearchAngle;  // 7.3.0 추가
	int nAutoType;          // 7.3.0 추가
	BOOL bUseGL;            // 7.8.0 추가

	CArray<T_SPLC_ECCEN, T_SPLC_ECCEN&> aSplcEccen_AL;  // 8.3.0에서 추가
	BOOL bConsEccMnt; // 8.3.0에서 추가

	BOOL bNonDissipative;   // 8.8.5 추가
	double dNonDissipative; // 8.8.5 추가

	int nAngleType;     // 0:Excitation Angle 1:Directional Combination(CQC3)
	double dCQCRatio;

	void Initialize()
	{
		LoadCaseName.Empty();
		Description.Empty();
		//    nSpecFuncKey=0;
		nDirection=0;
		dblAngle=0;
		dblFactor=0;
		LoadCaseId=0;
		dPeriodModifyFactor = 1.0;
		bConsiderEccen = FALSE;
		nEccenMethod = 0;
		nAutomatic = 0;
		dEccenPercent = 5.;
		aSplcEccen.RemoveAll();
		bApplyDampingMethod = FALSE;
		bCorrectDamping = FALSE;
		nDampingType = 1;
	#if defined(_JP)
		nDampingType = 3;
		bApplyDampingMethod = TRUE;
	#endif
		dDampAll = 0.05;
		aDampData.RemoveAll();
		PropDamping.Initialize();
		nInterpolateType = 1;
		aSpfcKey.RemoveAll();

		nSpecCombType=0;
	#if defined(_CIVIL) || defined(_JP)
		nSpecCombType = 1;
	#endif
	#if defined(_MGEN) || defined(_CH)
		nSpecCombType = 1;
	#endif
		bResultSign=FALSE;
		nSignType=0;
		bSelectUse=TRUE;
		aUseMode.RemoveAll();
		aMSFactor.RemoveAll();
		bAutoSearchAngle = FALSE;
		nAutoType = 0;
		bUseGL = TRUE;
		aSplcEccen_AL.RemoveAll();
	#if defined(_MGEN) && defined(_CH)
		bConsEccMnt = TRUE;
	#else
		bConsEccMnt = FALSE;
	#endif

		bNonDissipative = FALSE;
		dNonDissipative = 1.2;

		nAngleType = 0;
		dCQCRatio = 0.3;
	}
	T_SPLC_D()
	{
	}
	T_SPLC_D(T_SPLC_D& src)
	{
		*this = src;
	}
	T_SPLC_D& operator=(T_SPLC_D& src)
	{
		LoadCaseName = src.LoadCaseName;
		Description  = src.Description;
		//    nSpecFuncKey = src.nSpecFuncKey;
		nDirection   = src.nDirection;
		dblAngle     = src.dblAngle;
		dblFactor    = src.dblFactor;
		LoadCaseId   = src.LoadCaseId;
		dPeriodModifyFactor = src.dPeriodModifyFactor;
		bConsiderEccen = src.bConsiderEccen;
		nEccenMethod = src.nEccenMethod;
		nAutomatic   = src.nAutomatic;
		dEccenPercent = src.dEccenPercent;
		aSplcEccen.Copy(src.aSplcEccen);
		bCorrectDamping = src.bCorrectDamping;
		nDampingType = src.nDampingType;
		dDampAll     = src.dDampAll;
		aDampData.Copy(src.aDampData);
		PropDamping  = src.PropDamping;
		nInterpolateType = src.nInterpolateType;
		aSpfcKey.Copy(src.aSpfcKey);
		bApplyDampingMethod = src.bApplyDampingMethod;
		nSpecCombType= src.nSpecCombType;
		bResultSign=src.bResultSign;
		nSignType=src.nSignType;
		bSelectUse=src.bSelectUse;
		aUseMode.Copy(src.aUseMode);
		aMSFactor.Copy(src.aMSFactor);
		bAutoSearchAngle = src.bAutoSearchAngle;
		nAutoType = src.nAutoType;
		bUseGL = src.bUseGL;
		aSplcEccen_AL.Copy(src.aSplcEccen_AL);
		bConsEccMnt = src.bConsEccMnt;
		bNonDissipative = src.bNonDissipative;
		dNonDissipative = src.dNonDissipative;
		nAngleType = src.nAngleType;
		dCQCRatio  = src.dCQCRatio;
		return *this;
	}
	void Convert590(T_SPLC_D_590& rData) // MGT에서 사용
	{
		Initialize();
		LoadCaseName = rData.LoadCaseName;
		Description  = rData.Description;
		//    nSpecFuncKey = rData.nSpecFuncKey;
		nDirection   = rData.nDirection;
		dblAngle     = rData.dblAngle;
		dblFactor    = rData.dblFactor;
		LoadCaseId   = rData.LoadCaseId;
		dPeriodModifyFactor = 1.0;
		bConsiderEccen = FALSE;
		nEccenMethod = 0;
		nAutomatic   = 0;
		dEccenPercent = 5.0;
		aSplcEccen.RemoveAll();
		bCorrectDamping = FALSE;
		nDampingType = 1;
		dDampAll = 0.05;
		aDampData.RemoveAll();
		PropDamping.Initialize();
		nInterpolateType = 0;
		aSpfcKey.RemoveAll();
		aSpfcKey.Add(rData.nSpecFuncKey);
	}
};

struct T_SPLC_D_CH
{
	char LoadCaseName[20];
	char Description[80];
	//  unsigned int nSpecFuncKey;
	int nDirection;  // 1=X-Y  2=Z
	double dblAngle; // nDirection == 1 인 경우에만 사용
	double dblFactor;
	unsigned int LoadCaseId;    // 순번
	double dPeriodModifyFactor;  // 6.0.0에서 추가
	BOOL bConsiderEccen;        // 6.0.0에서 추가  Consider Accidental Eccentricity
	int  nEccenMethod;          // 6.0.0에서 추가  0: Linear Sum, 1: SRSS
	int  nAutomatic;            // 6.0.0에서 추가  0: Automatic,  1: User Defined
	double dEccenPercent;        // 6.0.0에서 추가
	CArray<T_SPLC_ECCEN, T_SPLC_ECCEN&> aSplcEccen;  // 6.0.0에서 추가
	BOOL bCorrectDamping;   // 6.5.0에서 추가
	int nDampingType;       // 6.5.0에서 추가 : 1=Direct Modal, 2=Mass and Stiffness Proportional, 3=Group
	double dDampAll;        // 6.5.0에서 추가 : nDampingType == Direct Modal인 경우 Damping Ratio for All Modes
	T_THIS_DT aDampData;    // 6.5.0에서 추가 : nDampingType == Direct Modal인 경우 Damping Data
	T_THIS_PROPDAMPING PropDamping;   // 6.5.0에서 추가 : nDampingType == Mass and Stiffness Proportional인 경우 Damping Data
	int  nInterpolateType;  // 6.5.0에서 추가 : 0=Linear, 1=Logarithm
	CArray<T_SPFC_K, T_SPFC_K> aSpfcKey;  // 6.6.0에서 추가: 기존의 SPFC key하나만 받는 것에서 여러개 받는 것으로 변경
	BOOL bApplyDampingMethod;

	int nSpecCombType;      // 7.0.2 추가 0=SRSS 1=CQC 2=ABS 3=Linear
	BOOL bResultSign;       // 7.0.2 추가
	int nSignType;        // 7.0.2 추가 0=Major Mode, 1=Maximum Value
	BOOL bSelectUse;      //7.0.2 추가
	CArray<BOOL, BOOL> aUseMode;    // 7.0.2 추가 각 모드의 사용여부
	CArray<double, double> aMSFactor;   // 7.0.2 추가 각 모드의 Mode Shape Factor

	BOOL bAutoSearchAngle;  // 7.3.0 추가
	int nAutoType;          // 7.3.0 추가
	BOOL bUseGL;            // 7.8.0 추가

	CArray<T_SPLC_ECCEN, T_SPLC_ECCEN&> aSplcEccen_AL;  // 8.3.0에서 추가
	BOOL bConsEccMnt; // 8.3.0에서 추가

	BOOL bNonDissipative;   // 8.8.3 추가
	double dNonDissipative; // 8.8.3 추가

	int nAngleType;
	double dCQCRatio;

	void ConvertToString(T_SPLC_D& rData)
	{
		ConvertCharStr(LoadCaseName, rData.LoadCaseName, sizeof(LoadCaseName));
		ConvertCharStr(Description, rData.Description, sizeof(Description));
		//    rData.nSpecFuncKey=nSpecFuncKey;
		rData.nDirection=nDirection;
		rData.dblAngle=dblAngle;
		rData.dblFactor=dblFactor;
		rData.LoadCaseId=LoadCaseId;
		rData.dPeriodModifyFactor = dPeriodModifyFactor;
		rData.bConsiderEccen = bConsiderEccen;
		rData.nEccenMethod = nEccenMethod;
		rData.nAutomatic = nAutomatic;
		rData.dEccenPercent = dEccenPercent;
		rData.aSplcEccen.Copy(aSplcEccen);
		rData.bCorrectDamping = bCorrectDamping;
		rData.nDampingType = nDampingType;
		rData.dDampAll     = dDampAll;
		rData.aDampData.Copy(aDampData);
		rData.PropDamping  = PropDamping;
		rData.nInterpolateType = nInterpolateType;
		rData.aSpfcKey.Copy(aSpfcKey);
		rData.bApplyDampingMethod = bApplyDampingMethod;

		rData.nSpecCombType=nSpecCombType;
		rData.bResultSign=bResultSign;
		rData.nSignType=nSignType;
		rData.bSelectUse=bSelectUse;
		rData.aUseMode.Copy(aUseMode);
		rData.aMSFactor.Copy(aMSFactor);

		rData.bAutoSearchAngle=bAutoSearchAngle;
		rData.nAutoType=nAutoType;
		rData.bUseGL = bUseGL;
		rData.aSplcEccen_AL.Copy(aSplcEccen_AL);
		rData.bConsEccMnt = bConsEccMnt;

		rData.bNonDissipative = bNonDissipative;
		rData.dNonDissipative = dNonDissipative;

		rData.nAngleType = nAngleType;
		rData.dCQCRatio = dCQCRatio;
	}
	void ConvertToChar(T_SPLC_D& rData)
	{
		ConvertStrChar(rData.LoadCaseName, LoadCaseName, sizeof(LoadCaseName));
		ConvertStrChar(rData.Description, Description, sizeof(Description));
		//    nSpecFuncKey=rData.nSpecFuncKey;
		nDirection=rData.nDirection;
		dblAngle=rData.dblAngle;
		dblFactor=rData.dblFactor;
		LoadCaseId=rData.LoadCaseId;
		dPeriodModifyFactor = rData.dPeriodModifyFactor;
		bConsiderEccen = rData.bConsiderEccen;
		nEccenMethod   = rData.nEccenMethod;
		nAutomatic     = rData.nAutomatic;
		dEccenPercent  = rData.dEccenPercent;
		aSplcEccen.Copy(rData.aSplcEccen);
		bCorrectDamping = rData.bCorrectDamping;
		nDampingType = rData.nDampingType;
		dDampAll     = rData.dDampAll;
		aDampData.Copy(rData.aDampData);
		PropDamping  = rData.PropDamping;
		nInterpolateType = rData.nInterpolateType;
		aSpfcKey.Copy(rData.aSpfcKey);
		bApplyDampingMethod = rData.bApplyDampingMethod;

		nSpecCombType=rData.nSpecCombType;
		bResultSign=rData.bResultSign;
		nSignType=rData.nSignType;
		bSelectUse=rData.bSelectUse;
		aUseMode.Copy(rData.aUseMode);
		aMSFactor.Copy(rData.aMSFactor);

		bAutoSearchAngle=rData.bAutoSearchAngle;
		nAutoType=rData.nAutoType;
		bUseGL=rData.bUseGL;
		aSplcEccen_AL.Copy(rData.aSplcEccen_AL);
		bConsEccMnt=rData.bConsEccMnt;

		bNonDissipative=rData.bNonDissipative;
		dNonDissipative=rData.dNonDissipative;

		nAngleType = rData.nAngleType;
		dCQCRatio = rData.dCQCRatio;
	}
};

struct T_SPLC_UDRD_D
{
	T_SPLC_K key;
	T_SPLC_D data;
};

struct T_SPLC_UDRD_D_CH
{
	T_SPLC_K key;
	T_SPLC_D_CH data;
};

struct T_SPLC_UDRD_D_CH_RW_590
{
	T_SPLC_K key;
	char LoadCaseName[20];
	char Description[80];
	unsigned int nSpecFuncKey;
	int nDirection;  // 1=X-Y  2=Z
	double dblAngle; // nDirection == 1 인 경우에만 사용
	double dblFactor;
	unsigned int LoadCaseId;    // 순번
};

struct T_SPLC_UDRD_D_CH_RW_640
{
	T_SPLC_K key;
	char LoadCaseName[20];
	char Description[80];
	unsigned int nSpecFuncKey;
	int nDirection;  // 1=X-Y  2=Z
	double dblAngle; // nDirection == 1 인 경우에만 사용
	double dblFactor;
	unsigned int LoadCaseId;    // 순번
	double dPeriodModifyFactor;  // 6.0.0에서 추가
	BOOL bConsiderEccen;        // 6.0.0에서 추가  Consider Accidental Eccentricity
	int  nEccenMethod;          // 6.0.0에서 추가  0: Linear Sum, 1: SRSS
	int  nAutomatic;            // 6.0.0에서 추가  0: Automatic,  1: User Defined
	double dEccenPercent;        // 6.0.0에서 추가
	int nCountEccen;
};

struct T_SPLC_UDRD_D_CH_RW_650
{
	T_SPLC_K key;
	char LoadCaseName[20];
	char Description[80];
	unsigned int nSpecFuncKey;
	int nDirection;  // 1=X-Y  2=Z
	double dblAngle; // nDirection == 1 인 경우에만 사용
	double dblFactor;
	unsigned int LoadCaseId;    // 순번
	double dPeriodModifyFactor;  // 6.0.0에서 추가
	BOOL bConsiderEccen;        // 6.0.0에서 추가  Consider Accidental Eccentricity
	int  nEccenMethod;          // 6.0.0에서 추가  0: Linear Sum, 1: SRSS
	int  nAutomatic;            // 6.0.0에서 추가  0: Automatic,  1: User Defined
	double dEccenPercent;        // 6.0.0에서 추가
	int nCountEccen;
	BOOL bCorrectDamping;   // 6.5.0에서 추가
	int nDampingType;       // 6.5.0에서 추가 : 1=Direct Modal, 2=Mass and Stiffness Proportional, 3=Group
	double dDampAll;        // 6.5.0에서 추가 : nDampingType == Direct Modal인 경우 Damping Ratio for All Modes
	int nCountDamping;      // 6.5.0에서 추가 : nDampingType == Direct Modal인 경우 Damping Data
	T_THIS_PROPDAMPING PropDamping;   // 6.5.0에서 추가 : nDampingType == Mass and Stiffness Proportional인 경우 Damping Data
	int  nInterpolateType;  // 6.5.0에서 추가 : 0=Linear, 1=Logarithm
};

struct T_SPLC_UDRD_D_CH_RW_660
{
	T_SPLC_K key;
	char LoadCaseName[20];
	char Description[80];
	unsigned int nSpecFuncKey;
	int nDirection;  // 1=X-Y  2=Z
	double dblAngle; // nDirection == 1 인 경우에만 사용
	double dblFactor;
	unsigned int LoadCaseId;    // 순번
	double dPeriodModifyFactor;  // 6.0.0에서 추가
	BOOL bConsiderEccen;        // 6.0.0에서 추가  Consider Accidental Eccentricity
	int  nEccenMethod;          // 6.0.0에서 추가  0: Linear Sum, 1: SRSS
	int  nAutomatic;            // 6.0.0에서 추가  0: Automatic,  1: User Defined
	double dEccenPercent;        // 6.0.0에서 추가
	int nCountEccen;
	BOOL bCorrectDamping;   // 6.5.0에서 추가
	int nDampingType;       // 6.5.0에서 추가 : 1=Direct Modal, 2=Mass and Stiffness Proportional, 3=Group
	double dDampAll;        // 6.5.0에서 추가 : nDampingType == Direct Modal인 경우 Damping Ratio for All Modes
	int nCountDamping;      // 6.5.0에서 추가 : nDampingType == Direct Modal인 경우 Damping Data
	T_THIS_PROPDAMPING PropDamping;   // 6.5.0에서 추가 : nDampingType == Mass and Stiffness Proportional인 경우 Damping Data
	int  nInterpolateType;  // 6.5.0에서 추가 : 0=Linear, 1=Logarithm
	int  nCountSpfcKey;     // 6.6.0에서 추가
};

struct T_SPLC_UDRD_D_CH_RW_700
{
	T_SPLC_K key;
	char LoadCaseName[20];
	char Description[80];
	unsigned int nSpecFuncKey;
	int nDirection;  // 1=X-Y  2=Z
	double dblAngle; // nDirection == 1 인 경우에만 사용
	double dblFactor;
	unsigned int LoadCaseId;    // 순번
	double dPeriodModifyFactor;  // 6.0.0에서 추가
	BOOL bConsiderEccen;        // 6.0.0에서 추가  Consider Accidental Eccentricity
	int  nEccenMethod;          // 6.0.0에서 추가  0: Linear Sum, 1: SRSS
	int  nAutomatic;            // 6.0.0에서 추가  0: Automatic,  1: User Defined
	double dEccenPercent;        // 6.0.0에서 추가
	int nCountEccen;
	BOOL bCorrectDamping;   // 6.5.0에서 추가
	int nDampingType;       // 6.5.0에서 추가 : 1=Direct Modal, 2=Mass and Stiffness Proportional, 3=Group
	double dDampAll;        // 6.5.0에서 추가 : nDampingType == Direct Modal인 경우 Damping Ratio for All Modes
	int nCountDamping;      // 6.5.0에서 추가 : nDampingType == Direct Modal인 경우 Damping Data
	T_THIS_PROPDAMPING PropDamping;   // 6.5.0에서 추가 : nDampingType == Mass and Stiffness Proportional인 경우 Damping Data
	int  nInterpolateType;  // 6.5.0에서 추가 : 0=Linear, 1=Logarithm
	int  nCountSpfcKey;     // 6.6.0에서 추가
	BOOL bApplyDampingMethod;  // 6.7.0 추가
};

struct T_SPLC_UDRD_D_CH_RW_721
{
	T_SPLC_K key;
	char LoadCaseName[20];
	char Description[80];
	unsigned int nSpecFuncKey;
	int nDirection;  // 1=X-Y  2=Z
	double dblAngle; // nDirection == 1 인 경우에만 사용
	double dblFactor;
	unsigned int LoadCaseId;    // 순번
	double dPeriodModifyFactor;  // 6.0.0에서 추가
	BOOL bConsiderEccen;        // 6.0.0에서 추가  Consider Accidental Eccentricity
	int  nEccenMethod;          // 6.0.0에서 추가  0: Linear Sum, 1: SRSS
	int  nAutomatic;            // 6.0.0에서 추가  0: Automatic,  1: User Defined
	double dEccenPercent;        // 6.0.0에서 추가
	int nCountEccen;
	BOOL bCorrectDamping;   // 6.5.0에서 추가
	int nDampingType;       // 6.5.0에서 추가 : 1=Direct Modal, 2=Mass and Stiffness Proportional, 3=Group
	double dDampAll;        // 6.5.0에서 추가 : nDampingType == Direct Modal인 경우 Damping Ratio for All Modes
	int nCountDamping;      // 6.5.0에서 추가 : nDampingType == Direct Modal인 경우 Damping Data
	T_THIS_PROPDAMPING PropDamping;   // 6.5.0에서 추가 : nDampingType == Mass and Stiffness Proportional인 경우 Damping Data
	int  nInterpolateType;  // 6.5.0에서 추가 : 0=Linear, 1=Logarithm
	int  nCountSpfcKey;     // 6.6.0에서 추가
	BOOL bApplyDampingMethod;  // 6.7.0 추가
	int nSpecCombType;      // 7.0.2 추가 0=SRSS 1=CQC 2=ABS 3=Linear
	BOOL bResultSign;       // 7.0.2 추가
	int nSignType;        // 7.0.2 추가 0=Major Mode, 1=Maximum Value
	int nCountUseMode;    // 7.0.2 추가 각 모드의 사용여부
	int bSelectUse;       // 7.0.2 추가
	int nCountMSFactor;   // 7.0.2 추가 각 모드의 Mode Shape Factor
};

struct T_SPLC_UDRD_D_CH_RW_770
{
	T_SPLC_K key;
	char LoadCaseName[20];
	char Description[80];
	unsigned int nSpecFuncKey;
	int nDirection;  // 1=X-Y  2=Z
	double dblAngle; // nDirection == 1 인 경우에만 사용
	double dblFactor;
	unsigned int LoadCaseId;    // 순번
	double dPeriodModifyFactor;  // 6.0.0에서 추가
	BOOL bConsiderEccen;        // 6.0.0에서 추가  Consider Accidental Eccentricity
	int  nEccenMethod;          // 6.0.0에서 추가  0: Linear Sum, 1: SRSS
	int  nAutomatic;            // 6.0.0에서 추가  0: Automatic,  1: User Defined
	double dEccenPercent;        // 6.0.0에서 추가
	int nCountEccen;
	BOOL bCorrectDamping;   // 6.5.0에서 추가
	int nDampingType;       // 6.5.0에서 추가 : 1=Direct Modal, 2=Mass and Stiffness Proportional, 3=Group
	double dDampAll;        // 6.5.0에서 추가 : nDampingType == Direct Modal인 경우 Damping Ratio for All Modes
	int nCountDamping;      // 6.5.0에서 추가 : nDampingType == Direct Modal인 경우 Damping Data
	T_THIS_PROPDAMPING PropDamping;   // 6.5.0에서 추가 : nDampingType == Mass and Stiffness Proportional인 경우 Damping Data
	int  nInterpolateType;  // 6.5.0에서 추가 : 0=Linear, 1=Logarithm
	int  nCountSpfcKey;     // 6.6.0에서 추가
	BOOL bApplyDampingMethod; // 6.7.0에서 추가
	int nSpecCombType;      // 7.0.2 추가 0=SRSS 1=CQC 2=ABS 3=Linear
	BOOL bResultSign;       // 7.0.2 추가
	int nSignType;        // 7.0.2 추가 0=Major Mode, 1=Maximum Value
	int nCountUseMode;    // 7.0.2 추가 각 모드의 사용여부
	int bSelectUse;       // 7.0.2 추가
	int nCountMSFactor;   // 7.0.2 추가 각 모드의 Mode Shape Factor

	BOOL bAutoSearchAngle;  // 7.3.0 추가
	int nAutoType;          // 7.3.0 추가
};

struct T_SPLC_UDRD_D_CH_RW_830
{
	T_SPLC_K key;
	char LoadCaseName[20];
	char Description[80];
	unsigned int nSpecFuncKey;
	int nDirection;  // 1=X-Y  2=Z
	double dblAngle; // nDirection == 1 인 경우에만 사용
	double dblFactor;
	unsigned int LoadCaseId;    // 순번
	double dPeriodModifyFactor;  // 6.0.0에서 추가
	BOOL bConsiderEccen;        // 6.0.0에서 추가  Consider Accidental Eccentricity
	int  nEccenMethod;          // 6.0.0에서 추가  0: Linear Sum, 1: SRSS
	int  nAutomatic;            // 6.0.0에서 추가  0: Automatic,  1: User Defined
	double dEccenPercent;        // 6.0.0에서 추가
	int nCountEccen;
	BOOL bCorrectDamping;   // 6.5.0에서 추가
	int nDampingType;       // 6.5.0에서 추가 : 1=Direct Modal, 2=Mass and Stiffness Proportional, 3=Group
	double dDampAll;        // 6.5.0에서 추가 : nDampingType == Direct Modal인 경우 Damping Ratio for All Modes
	int nCountDamping;      // 6.5.0에서 추가 : nDampingType == Direct Modal인 경우 Damping Data
	T_THIS_PROPDAMPING PropDamping;   // 6.5.0에서 추가 : nDampingType == Mass and Stiffness Proportional인 경우 Damping Data
	int  nInterpolateType;  // 6.5.0에서 추가 : 0=Linear, 1=Logarithm
	int  nCountSpfcKey;     // 6.6.0에서 추가
	BOOL bApplyDampingMethod; // 6.7.0에서 추가
	int nSpecCombType;      // 7.0.2 추가 0=SRSS 1=CQC 2=ABS 3=Linear
	BOOL bResultSign;       // 7.0.2 추가
	int nSignType;        // 7.0.2 추가 0=Major Mode, 1=Maximum Value
	int nCountUseMode;    // 7.0.2 추가 각 모드의 사용여부
	int bSelectUse;       // 7.0.2 추가
	int nCountMSFactor;   // 7.0.2 추가 각 모드의 Mode Shape Factor

	BOOL bAutoSearchAngle;  // 7.3.0 추가
	int nAutoType;          // 7.3.0 추가
	BOOL bUseGL;            // 7.8.0 추가
};

struct T_SPLC_UDRD_D_CH_RW_883
{
	T_SPLC_K key;
	char LoadCaseName[20];
	char Description[80];
	unsigned int nSpecFuncKey;
	int nDirection;  // 1=X-Y  2=Z
	double dblAngle; // nDirection == 1 인 경우에만 사용
	double dblFactor;
	unsigned int LoadCaseId;    // 순번
	double dPeriodModifyFactor;  // 6.0.0에서 추가
	BOOL bConsiderEccen;        // 6.0.0에서 추가  Consider Accidental Eccentricity
	int  nEccenMethod;          // 6.0.0에서 추가  0: Linear Sum, 1: SRSS
	int  nAutomatic;            // 6.0.0에서 추가  0: Automatic,  1: User Defined
	double dEccenPercent;        // 6.0.0에서 추가
	int nCountEccen;
	BOOL bCorrectDamping;   // 6.5.0에서 추가
	int nDampingType;       // 6.5.0에서 추가 : 1=Direct Modal, 2=Mass and Stiffness Proportional, 3=Group
	double dDampAll;        // 6.5.0에서 추가 : nDampingType == Direct Modal인 경우 Damping Ratio for All Modes
	int nCountDamping;      // 6.5.0에서 추가 : nDampingType == Direct Modal인 경우 Damping Data
	T_THIS_PROPDAMPING PropDamping;   // 6.5.0에서 추가 : nDampingType == Mass and Stiffness Proportional인 경우 Damping Data
	int  nInterpolateType;  // 6.5.0에서 추가 : 0=Linear, 1=Logarithm
	int  nCountSpfcKey;     // 6.6.0에서 추가
	BOOL bApplyDampingMethod; // 6.7.0에서 추가
	int nSpecCombType;      // 7.0.2 추가 0=SRSS 1=CQC 2=ABS 3=Linear
	BOOL bResultSign;       // 7.0.2 추가
	int nSignType;        // 7.0.2 추가 0=Major Mode, 1=Maximum Value
	int nCountUseMode;    // 7.0.2 추가 각 모드의 사용여부
	int bSelectUse;       // 7.0.2 추가
	int nCountMSFactor;   // 7.0.2 추가 각 모드의 Mode Shape Factor

	BOOL bAutoSearchAngle;  // 7.3.0 추가
	int nAutoType;          // 7.3.0 추가
	BOOL bUseGL;            // 7.8.0 추가

	int nCountEccen_AL;     //8.3.0 추가
	BOOL bConsEccMnt;
};


struct T_SPLC_UDRD_D_CH_RW_950
{
	T_SPLC_K key;
	char LoadCaseName[20];
	char Description[80];
	unsigned int nSpecFuncKey;
	int nDirection;  // 1=X-Y  2=Z
	double dblAngle; // nDirection == 1 인 경우에만 사용
	double dblFactor;
	unsigned int LoadCaseId;    // 순번
	double dPeriodModifyFactor;  // 6.0.0에서 추가
	BOOL bConsiderEccen;        // 6.0.0에서 추가  Consider Accidental Eccentricity
	int  nEccenMethod;          // 6.0.0에서 추가  0: Linear Sum, 1: SRSS
	int  nAutomatic;            // 6.0.0에서 추가  0: Automatic,  1: User Defined
	double dEccenPercent;        // 6.0.0에서 추가
	int nCountEccen;
	BOOL bCorrectDamping;   // 6.5.0에서 추가
	int nDampingType;       // 6.5.0에서 추가 : 1=Direct Modal, 2=Mass and Stiffness Proportional, 3=Group
	double dDampAll;        // 6.5.0에서 추가 : nDampingType == Direct Modal인 경우 Damping Ratio for All Modes
	int nCountDamping;      // 6.5.0에서 추가 : nDampingType == Direct Modal인 경우 Damping Data
	T_THIS_PROPDAMPING PropDamping;   // 6.5.0에서 추가 : nDampingType == Mass and Stiffness Proportional인 경우 Damping Data
	int  nInterpolateType;  // 6.5.0에서 추가 : 0=Linear, 1=Logarithm
	int  nCountSpfcKey;     // 6.6.0에서 추가
	BOOL bApplyDampingMethod; // 6.7.0에서 추가
	int nSpecCombType;      // 7.0.2 추가 0=SRSS 1=CQC 2=ABS 3=Linear
	BOOL bResultSign;       // 7.0.2 추가
	int nSignType;        // 7.0.2 추가 0=Major Mode, 1=Maximum Value
	int nCountUseMode;    // 7.0.2 추가 각 모드의 사용여부
	int bSelectUse;       // 7.0.2 추가
	int nCountMSFactor;   // 7.0.2 추가 각 모드의 Mode Shape Factor

	BOOL bAutoSearchAngle;  // 7.3.0 추가
	int nAutoType;          // 7.3.0 추가
	BOOL bUseGL;            // 7.8.0 추가

	int nCountEccen_AL;     //8.3.0 추가
	BOOL bConsEccMnt;

	BOOL bNonDissipative;   // 8.8.3 추가
	double dNonDissipative; // 8.8.3 추가

};

struct T_SPLC_UDRD_D_CH_RW
{
	T_SPLC_K key;
	char LoadCaseName[20];
	char Description[80];
	unsigned int nSpecFuncKey;
	int nDirection;  // 1=X-Y  2=Z
	double dblAngle; // nDirection == 1 인 경우에만 사용
	double dblFactor;
	unsigned int LoadCaseId;    // 순번
	double dPeriodModifyFactor;  // 6.0.0에서 추가
	BOOL bConsiderEccen;        // 6.0.0에서 추가  Consider Accidental Eccentricity
	int  nEccenMethod;          // 6.0.0에서 추가  0: Linear Sum, 1: SRSS
	int  nAutomatic;            // 6.0.0에서 추가  0: Automatic,  1: User Defined
	double dEccenPercent;        // 6.0.0에서 추가
	int nCountEccen;
	BOOL bCorrectDamping;   // 6.5.0에서 추가
	int nDampingType;       // 6.5.0에서 추가 : 1=Direct Modal, 2=Mass and Stiffness Proportional, 3=Group
	double dDampAll;        // 6.5.0에서 추가 : nDampingType == Direct Modal인 경우 Damping Ratio for All Modes
	int nCountDamping;      // 6.5.0에서 추가 : nDampingType == Direct Modal인 경우 Damping Data
	T_THIS_PROPDAMPING PropDamping;   // 6.5.0에서 추가 : nDampingType == Mass and Stiffness Proportional인 경우 Damping Data
	int  nInterpolateType;  // 6.5.0에서 추가 : 0=Linear, 1=Logarithm
	int  nCountSpfcKey;     // 6.6.0에서 추가
	BOOL bApplyDampingMethod; // 6.7.0에서 추가
	int nSpecCombType;      // 7.0.2 추가 0=SRSS 1=CQC 2=ABS 3=Linear
	BOOL bResultSign;       // 7.0.2 추가
	int nSignType;        // 7.0.2 추가 0=Major Mode, 1=Maximum Value
	int nCountUseMode;    // 7.0.2 추가 각 모드의 사용여부
	int bSelectUse;       // 7.0.2 추가
	int nCountMSFactor;   // 7.0.2 추가 각 모드의 Mode Shape Factor

	BOOL bAutoSearchAngle;  // 7.3.0 추가
	int nAutoType;          // 7.3.0 추가
	BOOL bUseGL;            // 7.8.0 추가

	int nCountEccen_AL;     //8.3.0 추가
	BOOL bConsEccMnt;

	BOOL bNonDissipative;   // 8.8.3 추가
	double dNonDissipative; // 8.8.3 추가

	int nAngleType; 
	double dCQCRatio; 

	void Initialize()
	{
		key=0;
		memset(LoadCaseName, 0, sizeof(LoadCaseName));
		memset(Description, 0, sizeof(Description));
		nSpecFuncKey = 0;
		nDirection = 0;
		dblAngle = 0;
		dblFactor = 0;
		LoadCaseId = 0;
		dPeriodModifyFactor = 1.0;
		bConsiderEccen = FALSE;
		nEccenMethod = 0;
		nAutomatic = 0;
		dEccenPercent = 0.0;
		nCountEccen = 0;
		nDampingType = 1;
		dDampAll = 0.05;
		nCountDamping = 0;
		PropDamping.Initialize();
		nInterpolateType = 0;
		nCountSpfcKey = 1;
		bCorrectDamping = FALSE;
		bApplyDampingMethod = FALSE;

		nSpecCombType=0;
	#if defined(_CIVIL)
	#if defined(_JP)
		nSpecCombType = 1;
	#endif
	#endif
		bResultSign = FALSE;
		nSignType = 0;
		bSelectUse = TRUE;
		nCountUseMode = 0;
		nCountMSFactor = 0;

		bAutoSearchAngle = FALSE;
		nAutoType = 0;
		bUseGL = FALSE;

		nCountEccen_AL = 0;
		bConsEccMnt = FALSE;

		bNonDissipative = FALSE;
		dNonDissipative = 1.2;

		nAngleType = 0;
		dCQCRatio = 0.3;
	}

	void GetSplc(T_SPLC_K& rKey, T_SPLC_D_CH& rData)
	{
		rKey=key;
		memcpy(rData.LoadCaseName, LoadCaseName, sizeof(LoadCaseName));
		memcpy(rData.Description, Description, sizeof(Description));
		//    rData.nSpecFuncKey = nSpecFuncKey;
		rData.nDirection   = nDirection;
		rData.dblAngle     = dblAngle;
		rData.dblFactor    = dblFactor;
		rData.LoadCaseId   = LoadCaseId;
		rData.dPeriodModifyFactor = dPeriodModifyFactor;
		rData.bConsiderEccen = bConsiderEccen;
		rData.nEccenMethod = nEccenMethod;
		rData.nAutomatic   = nAutomatic;
		rData.dEccenPercent = dEccenPercent;
		rData.bCorrectDamping = bCorrectDamping;
		rData.nDampingType = nDampingType;
		rData.dDampAll     = dDampAll;
		rData.PropDamping  = PropDamping;
		rData.nInterpolateType = nInterpolateType;
		rData.bApplyDampingMethod = bApplyDampingMethod;

		rData.nSpecCombType=nSpecCombType;
		rData.bSelectUse=bSelectUse;
		rData.bResultSign=bResultSign;
		rData.nSignType=nSignType;

		rData.bAutoSearchAngle = bAutoSearchAngle;
		rData.nAutoType = nAutoType;
		rData.bUseGL = bUseGL;
		rData.bConsEccMnt = bConsEccMnt;

		rData.bNonDissipative = bNonDissipative;
		rData.dNonDissipative = dNonDissipative;

		rData.nAngleType = nAngleType;
		rData.dCQCRatio = dCQCRatio;
	}
	void SetSplc(const T_SPLC_K Key, const T_SPLC_D_CH& rData)
	{
		key=Key;
		memcpy(LoadCaseName, rData.LoadCaseName, sizeof(LoadCaseName));
		memcpy(Description, rData.Description, sizeof(Description));
		//    nSpecFuncKey  = rData.nSpecFuncKey;
		nDirection    = rData.nDirection;
		dblAngle      = rData.dblAngle;
		dblFactor     = rData.dblFactor;
		LoadCaseId    = rData.LoadCaseId;
		dPeriodModifyFactor = rData.dPeriodModifyFactor;
		bConsiderEccen = rData.bConsiderEccen;
		nEccenMethod   = rData.nEccenMethod;
		nAutomatic     = rData.nAutomatic;
		dEccenPercent  = rData.dEccenPercent;
		nCountEccen    = ( int ) rData.aSplcEccen.GetSize();
		bCorrectDamping = rData.bCorrectDamping;
		nDampingType   = rData.nDampingType;
		dDampAll       = rData.dDampAll;
		nCountDamping  = ( int ) rData.aDampData.GetSize();
		PropDamping    = rData.PropDamping;
		nInterpolateType = rData.nInterpolateType;
		nCountSpfcKey  = ( int ) rData.aSpfcKey.GetSize();
		bApplyDampingMethod = rData.bApplyDampingMethod;

		nSpecCombType=rData.nSpecCombType;
		bResultSign=rData.bResultSign;
		nSignType=rData.nSignType;
		bSelectUse=rData.bSelectUse;
		nCountUseMode = ( int ) rData.aUseMode.GetSize();
		nCountMSFactor = ( int ) rData.aMSFactor.GetSize();

		bAutoSearchAngle = rData.bAutoSearchAngle;
		nAutoType = rData.nAutoType;
		bUseGL = rData.bUseGL;
		nCountEccen_AL    = ( int ) rData.aSplcEccen_AL.GetSize();
		bConsEccMnt = rData.bConsEccMnt;

		bNonDissipative = rData.bNonDissipative;
		dNonDissipative = rData.dNonDissipative;

		nAngleType = rData.nAngleType;
		dCQCRatio = rData.dCQCRatio;
	}
	void Convert590(T_SPLC_UDRD_D_CH_RW_590& rData)
	{
		Initialize();
		key=rData.key;
		memcpy(LoadCaseName, rData.LoadCaseName, sizeof(LoadCaseName));
		memcpy(Description, rData.Description, sizeof(Description));
		nSpecFuncKey  = rData.nSpecFuncKey;
		nDirection    = rData.nDirection;
		dblAngle      = rData.dblAngle;
		dblFactor     = rData.dblFactor;
		LoadCaseId    = rData.LoadCaseId;
	}
	void Convert640(T_SPLC_UDRD_D_CH_RW_640& rData)
	{
		Initialize();
		key=rData.key;
		memcpy(LoadCaseName, rData.LoadCaseName, sizeof(LoadCaseName));
		memcpy(Description, rData.Description, sizeof(Description));
		nSpecFuncKey  = rData.nSpecFuncKey;
		nDirection    = rData.nDirection;
		dblAngle      = rData.dblAngle;
		dblFactor     = rData.dblFactor;
		LoadCaseId    = rData.LoadCaseId;
		dPeriodModifyFactor = rData.dPeriodModifyFactor;
		bConsiderEccen = rData.bConsiderEccen;
		nEccenMethod = rData.nEccenMethod;
		nAutomatic   = rData.nAutomatic;
		dEccenPercent = rData.dEccenPercent;
		nCountEccen   = rData.nCountEccen;
		nCountEccen_AL = rData.nCountEccen;
	}
	void Convert650(T_SPLC_UDRD_D_CH_RW_650& rData)
	{
		Initialize();
		key=rData.key;
		memcpy(LoadCaseName, rData.LoadCaseName, sizeof(LoadCaseName));
		memcpy(Description, rData.Description, sizeof(Description));
		nSpecFuncKey  = rData.nSpecFuncKey;
		nDirection    = rData.nDirection;
		dblAngle      = rData.dblAngle;
		dblFactor     = rData.dblFactor;
		LoadCaseId    = rData.LoadCaseId;
		dPeriodModifyFactor = rData.dPeriodModifyFactor;
		bConsiderEccen = rData.bConsiderEccen;
		nEccenMethod = rData.nEccenMethod;
		nAutomatic   = rData.nAutomatic;
		dEccenPercent = rData.dEccenPercent;
		nCountEccen   = rData.nCountEccen;
		bCorrectDamping = rData.bCorrectDamping;
		nDampingType = rData.nDampingType;
		dDampAll = rData.dDampAll;
		nCountDamping = rData.nCountDamping;
		PropDamping = rData.PropDamping;
		nInterpolateType = rData.nInterpolateType;

		if ( rData.bCorrectDamping ) bApplyDampingMethod = TRUE;
		else bApplyDampingMethod = FALSE;

		nCountEccen_AL = rData.nCountEccen;
	}
	void Convert660(T_SPLC_UDRD_D_CH_RW_660& rData)
	{
		Initialize();
		key=rData.key;
		memcpy(LoadCaseName, rData.LoadCaseName, sizeof(LoadCaseName));
		memcpy(Description, rData.Description, sizeof(Description));
		nSpecFuncKey  = rData.nSpecFuncKey;
		nDirection    = rData.nDirection;
		dblAngle      = rData.dblAngle;
		dblFactor     = rData.dblFactor;
		LoadCaseId    = rData.LoadCaseId;
		dPeriodModifyFactor = rData.dPeriodModifyFactor;
		bConsiderEccen = rData.bConsiderEccen;
		nEccenMethod = rData.nEccenMethod;
		nAutomatic   = rData.nAutomatic;
		dEccenPercent = rData.dEccenPercent;
		nCountEccen   = rData.nCountEccen;
		bCorrectDamping = rData.bCorrectDamping;
		nDampingType = rData.nDampingType;
		dDampAll = rData.dDampAll;
		nCountDamping = rData.nCountDamping;
		PropDamping = rData.PropDamping;
		nInterpolateType = rData.nInterpolateType;
		nCountSpfcKey = rData.nCountSpfcKey;
		bApplyDampingMethod = FALSE;
		if ( rData.nCountSpfcKey >= 2 )
		{
			if ( rData.bCorrectDamping )
			{
				bApplyDampingMethod = TRUE;
				bCorrectDamping = FALSE;
			}
		}
		else
		{
			if ( rData.bCorrectDamping )
			{
				bApplyDampingMethod = TRUE;
				bCorrectDamping = TRUE;
			}
			else
			{
				bApplyDampingMethod = FALSE;
				bCorrectDamping = FALSE;
			}
		}

		nCountEccen_AL = rData.nCountEccen;
	}
	void Convert700(T_SPLC_UDRD_D_CH_RW_700& rData)
	{
		Initialize();
		key=rData.key;
		memcpy(LoadCaseName, rData.LoadCaseName, sizeof(LoadCaseName));
		memcpy(Description, rData.Description, sizeof(Description));
		nSpecFuncKey  = rData.nSpecFuncKey;
		nDirection    = rData.nDirection;
		dblAngle      = rData.dblAngle;
		dblFactor     = rData.dblFactor;
		LoadCaseId    = rData.LoadCaseId;
		dPeriodModifyFactor = rData.dPeriodModifyFactor;
		bConsiderEccen = rData.bConsiderEccen;
		nEccenMethod = rData.nEccenMethod;
		nAutomatic   = rData.nAutomatic;
		dEccenPercent = rData.dEccenPercent;
		nCountEccen   = rData.nCountEccen;
		bCorrectDamping = rData.bCorrectDamping;
		nDampingType = rData.nDampingType;
		dDampAll = rData.dDampAll;
		nCountDamping = rData.nCountDamping;
		PropDamping = rData.PropDamping;
		nInterpolateType = rData.nInterpolateType;
		nCountSpfcKey = rData.nCountSpfcKey;
		bApplyDampingMethod = rData.bApplyDampingMethod;

		nCountEccen_AL = rData.nCountEccen;
	}
	void Convert721(T_SPLC_UDRD_D_CH_RW_721& rData)
	{
		Initialize();
		key=rData.key;
		memcpy(LoadCaseName, rData.LoadCaseName, sizeof(LoadCaseName));
		memcpy(Description, rData.Description, sizeof(Description));
		nSpecFuncKey  = rData.nSpecFuncKey;
		nDirection    = rData.nDirection;
		dblAngle      = rData.dblAngle;
		dblFactor     = rData.dblFactor;
		LoadCaseId    = rData.LoadCaseId;
		dPeriodModifyFactor = rData.dPeriodModifyFactor;
		bConsiderEccen = rData.bConsiderEccen;
		nEccenMethod = rData.nEccenMethod;
		nAutomatic   = rData.nAutomatic;
		dEccenPercent = rData.dEccenPercent;
		nCountEccen   = rData.nCountEccen;
		bCorrectDamping = rData.bCorrectDamping;
		nDampingType = rData.nDampingType;
		dDampAll = rData.dDampAll;
		nCountDamping = rData.nCountDamping;
		PropDamping = rData.PropDamping;
		nInterpolateType = rData.nInterpolateType;
		nCountSpfcKey = rData.nCountSpfcKey;
		bApplyDampingMethod = rData.bApplyDampingMethod;

		nSpecCombType=rData.nSpecCombType;
		bResultSign = rData.bResultSign;
		nSignType = rData.nSignType;
		bSelectUse = rData.bSelectUse;
		nCountUseMode = rData.nCountUseMode;
		nCountMSFactor = rData.nCountMSFactor;

		nCountEccen_AL = rData.nCountEccen;
	}
	void Convert770(T_SPLC_UDRD_D_CH_RW_770& rData)
	{
		Initialize();
		key=rData.key;
		memcpy(LoadCaseName, rData.LoadCaseName, sizeof(LoadCaseName));
		memcpy(Description, rData.Description, sizeof(Description));
		nSpecFuncKey  = rData.nSpecFuncKey;
		nDirection    = rData.nDirection;
		dblAngle      = rData.dblAngle;
		dblFactor     = rData.dblFactor;
		LoadCaseId    = rData.LoadCaseId;
		dPeriodModifyFactor = rData.dPeriodModifyFactor;
		bConsiderEccen = rData.bConsiderEccen;
		nEccenMethod = rData.nEccenMethod;
		nAutomatic   = rData.nAutomatic;
		dEccenPercent = rData.dEccenPercent;
		nCountEccen   = rData.nCountEccen;
		bCorrectDamping = rData.bCorrectDamping;
		nDampingType = rData.nDampingType;
		dDampAll = rData.dDampAll;
		nCountDamping = rData.nCountDamping;
		PropDamping = rData.PropDamping;
		nInterpolateType = rData.nInterpolateType;
		nCountSpfcKey = rData.nCountSpfcKey;
		bApplyDampingMethod = rData.bApplyDampingMethod;

		nSpecCombType=rData.nSpecCombType;
		bResultSign = rData.bResultSign;
		nSignType = rData.nSignType;
		bSelectUse = rData.bSelectUse;
		nCountUseMode = rData.nCountUseMode;
		nCountMSFactor = rData.nCountMSFactor;

		bAutoSearchAngle = rData.bAutoSearchAngle;
		nAutoType = rData.nAutoType;

		nCountEccen_AL = rData.nCountEccen;
	}
	void Convert830(T_SPLC_UDRD_D_CH_RW_830& rData)
	{
		Initialize();
		key=rData.key;
		memcpy(LoadCaseName, rData.LoadCaseName, sizeof(LoadCaseName));
		memcpy(Description, rData.Description, sizeof(Description));
		nSpecFuncKey  = rData.nSpecFuncKey;
		nDirection    = rData.nDirection;
		dblAngle      = rData.dblAngle;
		dblFactor     = rData.dblFactor;
		LoadCaseId    = rData.LoadCaseId;
		dPeriodModifyFactor = rData.dPeriodModifyFactor;
		bConsiderEccen = rData.bConsiderEccen;
		nEccenMethod = rData.nEccenMethod;
		nAutomatic   = rData.nAutomatic;
		dEccenPercent = rData.dEccenPercent;
		nCountEccen   = rData.nCountEccen;
		bCorrectDamping = rData.bCorrectDamping;
		nDampingType = rData.nDampingType;
		dDampAll = rData.dDampAll;
		nCountDamping = rData.nCountDamping;
		PropDamping = rData.PropDamping;
		nInterpolateType = rData.nInterpolateType;
		nCountSpfcKey = rData.nCountSpfcKey;
		bApplyDampingMethod = rData.bApplyDampingMethod;

		nSpecCombType=rData.nSpecCombType;
		bResultSign = rData.bResultSign;
		nSignType = rData.nSignType;
		bSelectUse = rData.bSelectUse;
		nCountUseMode = rData.nCountUseMode;
		nCountMSFactor = rData.nCountMSFactor;

		bAutoSearchAngle = rData.bAutoSearchAngle;
		nAutoType = rData.nAutoType;

		nCountEccen_AL = rData.nCountEccen;
	}
	void Convert883(T_SPLC_UDRD_D_CH_RW_883& rData)
	{
		Initialize();
		key=rData.key;
		memcpy(LoadCaseName, rData.LoadCaseName, sizeof(LoadCaseName));
		memcpy(Description, rData.Description, sizeof(Description));
		nSpecFuncKey  = rData.nSpecFuncKey;
		nDirection    = rData.nDirection;
		dblAngle      = rData.dblAngle;
		dblFactor     = rData.dblFactor;
		LoadCaseId    = rData.LoadCaseId;
		dPeriodModifyFactor = rData.dPeriodModifyFactor;
		bConsiderEccen = rData.bConsiderEccen;
		nEccenMethod = rData.nEccenMethod;
		nAutomatic   = rData.nAutomatic;
		dEccenPercent = rData.dEccenPercent;
		nCountEccen   = rData.nCountEccen;
		bCorrectDamping = rData.bCorrectDamping;
		nDampingType = rData.nDampingType;
		dDampAll = rData.dDampAll;
		nCountDamping = rData.nCountDamping;
		PropDamping = rData.PropDamping;
		nInterpolateType = rData.nInterpolateType;
		nCountSpfcKey = rData.nCountSpfcKey;
		bApplyDampingMethod = rData.bApplyDampingMethod;

		nSpecCombType =rData.nSpecCombType;
		bResultSign = rData.bResultSign;
		nSignType = rData.nSignType;
		bSelectUse = rData.bSelectUse;
		nCountUseMode = rData.nCountUseMode;
		nCountMSFactor = rData.nCountMSFactor;

		bAutoSearchAngle = rData.bAutoSearchAngle;
		nAutoType = rData.nAutoType;
		bUseGL = rData.bUseGL;

		nCountEccen_AL = rData.nCountEccen;
		bConsEccMnt = rData.bConsEccMnt;
	}
	void Convert950(T_SPLC_UDRD_D_CH_RW_950& rData)
	{
		Initialize();
		key = rData.key;
		memcpy(LoadCaseName, rData.LoadCaseName, sizeof(LoadCaseName));
		memcpy(Description, rData.Description, sizeof(Description));
		nSpecFuncKey = rData.nSpecFuncKey;
		nDirection = rData.nDirection;
		dblAngle = rData.dblAngle;
		dblFactor = rData.dblFactor;
		LoadCaseId = rData.LoadCaseId;
		dPeriodModifyFactor = rData.dPeriodModifyFactor;
		bConsiderEccen = rData.bConsiderEccen;
		nEccenMethod = rData.nEccenMethod;
		nAutomatic = rData.nAutomatic;
		dEccenPercent = rData.dEccenPercent;
		nCountEccen = rData.nCountEccen;
		bCorrectDamping = rData.bCorrectDamping;
		nDampingType = rData.nDampingType;
		dDampAll = rData.dDampAll;
		nCountDamping = rData.nCountDamping;
		PropDamping = rData.PropDamping;
		nInterpolateType = rData.nInterpolateType;
		nCountSpfcKey = rData.nCountSpfcKey;
		bApplyDampingMethod = rData.bApplyDampingMethod;

		nSpecCombType = rData.nSpecCombType;
		bResultSign = rData.bResultSign;
		nSignType = rData.nSignType;
		bSelectUse = rData.bSelectUse;
		nCountUseMode = rData.nCountUseMode;
		nCountMSFactor = rData.nCountMSFactor;

		bAutoSearchAngle = rData.bAutoSearchAngle;
		nAutoType = rData.nAutoType;
		bUseGL = rData.bUseGL;

		nCountEccen_AL = rData.nCountEccen_AL;
		bConsEccMnt    = rData.bConsEccMnt;

		bNonDissipative = rData.bNonDissipative;
		dNonDissipative = rData.dNonDissipative;
	}
};

struct T_SPLC_UNIT
{
	T_SPLC_UNIT()
	{
		LoadCaseName=D_UNITSYS_NONE;
		Description=D_UNITSYS_NONE;
		nSpecFuncKey=D_UNITSYS_NONE;
		nDirection=D_UNITSYS_NONE;
		dblAngle=D_UNITSYS_BASE_DEGREE;
		dblFactor=D_UNITSYS_NONE;
		LoadCaseId=D_UNITSYS_NONE;
		dEccentricity=D_UNITSYS_BASE_LENGTH;
	}
	int LoadCaseName;
	int Description;
	int nSpecFuncKey;
	int nDirection;
	int dblAngle;
	int dblFactor;
	int LoadCaseId;
	int dEccentricity;
};

#define HASHSIZESPLC 10
#define D_SPLC_MINNUM       1   // 입력가능한 최소 응답스펙트럼조건번호
#define D_SPLC_MAXNUM      10   // 입력가능한 최대 응답스펙트럼조건번호

#define T_NSPC_K T_SPLC_K
#define T_NSPC_D T_SPLC_D
#define HASHSIZENSPC HASHSIZESPLC

#define T_ESPC_K T_SPLC_K
#define T_ESPC_D T_SPLC_D
#define HASHSIZEESPC HASHSIZESPLC

// Response Spectrum - Modification Factor
#define T_RSMF_K T_SPLC_K
struct T_RSMF_D_871
{
	double dC; // 최대 비탄성변위와 선형탄성변위의 보정을 위한 수정계수
};

struct T_RSMF_D_885
{
	double dC; // 최대 비탄성변위와 선형탄성변위의 보정을 위한 수정계수
	double dJ; // 하중 전달계수
};

struct T_RSMF_D
{
	double dC; // 최대 비탄성변위와 선형탄성변위의 보정을 위한 수정계수
	double dJ; // 하중 전달계수
	double dX; // 성능수준에 따른 보정계수
	T_RSMF_D() : dC(1.0), dJ(1.0), dX(1.0) { }
	T_RSMF_D(T_RSMF_D& src)
	{
		*this = src;
	}
	T_RSMF_D& operator=(T_RSMF_D& src)
	{
		dC = src.dC;
		dJ = src.dJ;
		dX = src.dX;
		return *this;
	}
	void Convert(T_RSMF_D_871& rData)
	{
		dC = rData.dC;
		dJ = 1.0;
	}
	void Convert(T_RSMF_D_885& rData)
	{
		dC = rData.dC;
		dJ = rData.dJ;
		dX = 1.0;
	}
};

struct T_RSMF_UDRD_D_871
{
	T_RSMF_K key;
	T_RSMF_D_871 data;
};

struct T_RSMF_UDRD_D_885
{
	T_RSMF_K key;
	T_RSMF_D_885 data;
};

struct T_RSMF_UDRD_D
{
	T_RSMF_K key;
	T_RSMF_D data;
	void Convert871(T_RSMF_UDRD_D_871& rData)
	{
		key = rData.key;
		data.Convert(rData.data);
	}
	void Convert885(T_RSMF_UDRD_D_885& rData)
	{
		key = rData.key;
		data.Convert(rData.data);
	}
};

struct T_RSMF_UNIT
{
	T_RSMF_UNIT()
	{
		nNoUnit = D_UNITSYS_NONE;
	}
	int nNoUnit;
};
#define HASHSIZERSMF HASHSIZESPLC

// Load Case for Force Controlled Action
enum D_LFCA_TYPE { D_LFCA_TYPE_PSLL=1, D_LFCA_TYPE_SPLC };
#define T_LFCA_KEY unsigned int

union T_LFCA_K
{
	T_LFCA_KEY keymap;
	struct
	{
		unsigned int type : 4;  // 1:Pseudo Seismic Force, 2:Response Spectrum
		unsigned int objK : 28; // type==1->T_PSLL_K, type==2->T_SPLC_K
	}key;
};

struct T_LFCA_D
{
	double dC; // 사용 안 함
	double dJ; // 하중전달 감소계수
	T_LFCA_D() : dC(1.0), dJ(1.0) { }
	T_LFCA_D(T_LFCA_D& src)
	{
		*this = src;
	}
	T_LFCA_D& operator=(T_LFCA_D& src)
	{
		dC = src.dC;
		dJ = src.dJ;
		return *this;
	}
};

struct T_LFCA_UDRD_D
{
	T_LFCA_K key;
	T_LFCA_D data;
};

struct T_LFCA_UNIT
{
	T_LFCA_UNIT()
	{
		nNoUnit = D_UNITSYS_NONE;
	}
	int nNoUnit;
};

#define HASHSIZELFCA HASHSIZESPLC+HASHSIZESEIS

#pragma pack(pop)

#endif  // __DB_ST_DT_SPEC_H__
