#ifndef __DB_ST_DT_WDPR_H__
#define __DB_ST_DT_WDPR_H__

#define MIDAS_SDK

#pragma pack(push, 1)

//////////////////////////////////////////////////////////////////////////
//
// WDPR = Wind Pressure의 약자 해로운 풍하중 자동계산을 위해 필요한 구조체들을 정의하였다.
//
// Added by yskim.. 풍하중 자동 계산 140311 [4802]
// 
//
// 다음 항목이 있다.(검색하여 바로 가기)
//
// 1. Wind Velocity Pressure.
//    = CCMWindPressureVelocityDefDlg 에서 정의한다.
//
// 2. Wind Pressure.
//    = CCMWindPressureRootDlg 에서 정의... Area, Beam, Nodal, Func 등 4가지.
//
//////////////////////////////////////////////////////////////////////////





//////////////////////////////////////////////////////////////////////////
// 1. Wind Velocity Pressure.
//////////////////////////////////////////////////////////////////////////


#define T_WVEP_K unsigned int

struct T_WVEP_KDS2021
{
	int		nMethod;								// 0: Simplified Method, 1: General Method

	double	dSimpleBasicWindSpeed;	//  m/sec
	double	dSimpleRoofHeight;
	double	dSimpleCe;

	int		nExposureCategory;			// 1=A 2=B 3=C 4=D
	double	dBasicWindSpeed;				//  m/sec
	double	dImportanceFactor;
	double	dRoofHeight;
	BOOL		bTopographicEffect;
	double	dKzt;
	double	dVzt;

	// Vortex Shedding
	BOOL bVortexShedding;
	double dVortexAverageRoof;
	double dVortexDM;							// Diameter of 2H/3 Level (Dm)
	double dVortexDB;							// Diameter of Bottom (DB) 
	double dVortexN;							// Natural Frequency (N)
	double dVortexMass;							// Mass(M)
	double dVortexDamp;

	void Initialize()
	{
		nMethod = 1;

		dSimpleBasicWindSpeed = 28.;
		dSimpleRoofHeight = 0.;
		dSimpleCe = 1.;

		nExposureCategory = 2;
		dBasicWindSpeed = 28.;
		dImportanceFactor = 1.;
		dRoofHeight = 0.;
		bTopographicEffect = FALSE;
		dKzt = 1.;
		dVzt = 0.;
		bVortexShedding = FALSE;
		dVortexAverageRoof = 0.;
		dVortexDM = 0.;
		dVortexDB = 0.;
		dVortexN = 0.;
		dVortexMass = 0.;
		dVortexDamp = 0.;
	}

// 	T_WVEP_KDS2021& operator=(T_WVEP_KDS2021& src)
// 	{
// 		nMethod = src.nMethod;
// 
// 		dSimpleBasicWindSpeed = src.dSimpleBasicWindSpeed;
// 		dSimpleRoofHeight = src.dSimpleRoofHeight;
// 		dSimpleCe = src.dSimpleCe;
// 
// 		nExposureCategory = src.nExposureCategory;
// 		dBasicWindSpeed = src.dBasicWindSpeed;
// 		dImportanceFactor = src.dImportanceFactor;
// 		dRoofHeight = src.dRoofHeight;
// 		bTopographicEffect = src.bTopographicEffect;
// 		dKzt = src.dKzt;
// 		dVzt = src.dVzt;
// 
// 		dVortexAverageRoof = src.dVortexAverageRoof;
// 		dVortexDM = src.dVortexDM;
// 		dVortexDB = src.dVortexDB;
// 		dVortexN = src.dVortexN;
// 		dVortexMass = src.dVortexMass;
// 		dVortexDamp = src.dVortexDamp;
// 
// 		return *this;
// 	}
};

struct T_WVEP_KBC2016
{
	int			nMethod;								// 0: Simplified Method, 1: General Method

	double	dSimpleBasicWindSpeed;	//  m/sec
	double	dSimpleRoofHeight;
	double	dSimpleCe;

	int			nExposureCategory;			// 1=A 2=B 3=C 4=D
	double	dBasicWindSpeed;				//  m/sec
	double	dImportanceFactor;
	double	dRoofHeight;
	BOOL		bTopographicEffect;
	double	dKzt;
	double	dVzt;

	void Initialize()
	{
		nMethod								= 1;

		dSimpleBasicWindSpeed	= 26.;
		dSimpleRoofHeight			= 0.;
		dSimpleCe							= 1.;

		nExposureCategory			= 2;
		dBasicWindSpeed				= 26.;
		dImportanceFactor			= 1.;
		dRoofHeight						= 0.;
		bTopographicEffect		= FALSE;
		dKzt									= 1.;
		dVzt									= 0.;
	}
// 	T_WVEP_KBC2016& operator=(T_WVEP_KBC2016& src)
// 	{
// 		nMethod								= src.nMethod;
// 
// 		dSimpleBasicWindSpeed	= src.dSimpleBasicWindSpeed;
// 		dSimpleRoofHeight			= src.dSimpleRoofHeight;
// 		dSimpleCe							= src.dSimpleCe;
// 
// 		nExposureCategory  = src.nExposureCategory;
// 		dBasicWindSpeed    = src.dBasicWindSpeed;
// 		dImportanceFactor  = src.dImportanceFactor;
// 		dRoofHeight        = src.dRoofHeight;
// 		bTopographicEffect = src.bTopographicEffect;
// 		dKzt               = src.dKzt;
// 		dVzt               = src.dVzt;
// 		return *this;
// 	}
};

struct T_WVEP_KBC2009
{
	int    nExposureCategory ; // 1=A 2=B 3=C 4=D
	double dBasicWindSpeed   ; // m/sec
	double dImportanceFactor ; // 1.0
	double dRoofHeight       ; // Average Roof Height : 0
	BOOL   bTopographicEffect;
	double dKzt              ;
	double dVzt              ;

	void Initialize()
	{
		nExposureCategory  = 2;
		dBasicWindSpeed    = 30.;
		dImportanceFactor  = 1.;
		dRoofHeight        = 0.;
		bTopographicEffect = FALSE;
		dKzt               = 1.;
		dVzt               = 0.;
	}
// 	T_WVEP_KBC2009& operator=(T_WVEP_KBC2009& src)
// 	{
// 		nExposureCategory  = src.nExposureCategory;
// 		dBasicWindSpeed    = src.dBasicWindSpeed;
// 		dImportanceFactor  = src.dImportanceFactor;
// 		dRoofHeight        = src.dRoofHeight;
// 		bTopographicEffect = src.bTopographicEffect;
// 		dKzt               = src.dKzt;
// 		dVzt               = src.dVzt;
// 		return *this;
// 	}
};

struct T_WVEP_CH2019
{
	int    nRoughCategory    ; // 1 = A, 2 = B, 3 = C
	double dBasicWindPressure; // Basic Wind Pressure : Wo // kN/m^2 [단위계 고정]
	double dTopographyCoef   ; // 1.0
	double dWindDirectionCoe; // Wind direction influence coefficient
	void Initialize()
	{
		nRoughCategory     = 1;
		dBasicWindPressure = 0.3;
		dTopographyCoef    = 1.0;
		dWindDirectionCoe  = 1.0;
	}
// 	T_WVEP_CH2019& operator=(T_WVEP_CH2019& src)
// 	{
// 		nRoughCategory     = src.nRoughCategory;
// 		dBasicWindPressure = src.dBasicWindPressure;
// 		dTopographyCoef    = src.dTopographyCoef;
// 		dWindDirectionCoe  = src.dWindDirectionCoe;
// 		return *this;
// 	}
};

struct T_WVEP_CH2012        // added by Maxiao(maxiao@midasit.com)
{
	int    nRoughCategory    ; // 1 = A, 2 = B, 3 = C
	double dBasicWindPressure; // Basic Wind Pressure : Wo // kN/m^2 [단위계 고정]
	double dTopographyCoef   ; // 1.0
	
	void Initialize()
	{
		nRoughCategory     = 1;
		dBasicWindPressure = 0.3;
		dTopographyCoef    = 1.0;
	}
// 	T_WVEP_CH2012& operator=(T_WVEP_CH2012& src)
// 	{
// 		nRoughCategory     = src.nRoughCategory;
// 		dBasicWindPressure = src.dBasicWindPressure;
// 		dTopographyCoef    = src.dTopographyCoef;
// 		return *this;
// 	}
};

struct T_WVEP_CH2001
{
	int    nRoughCategory    ; // 1 = A, 2 = B, 3 = C
	double dBasicWindPressure; // Basic Wind Pressure : Wo // kN/m^2 [단위계 고정]
	double dTopographyCoef   ; // Modify Factor 1.0
	
	void Initialize()
	{
		nRoughCategory     = 1;
		dBasicWindPressure = 0.3;
		dTopographyCoef    = 1.0;
	}
// 	T_WVEP_CH2001& operator=(T_WVEP_CH2001& src)
// 	{
// 		nRoughCategory     = src.nRoughCategory;
// 		dBasicWindPressure = src.dBasicWindPressure;
// 		dTopographyCoef    = src.dTopographyCoef;
// 		return *this;
// 	}
};

struct T_WVEP_ASCE7_16
{
	int		nExposureCategory;			// 2=B 3=C 4=D
	double	dBasicWindSpeed;			// mile/h.
	double	dMeanRoofHeight;
	BOOL	bTopographicEffect;
	double	dKzt;

	void Initialize()
	{
		nExposureCategory = 2;
		dBasicWindSpeed = 85.0;
		dMeanRoofHeight = 0.0;
		bTopographicEffect = FALSE;
		dKzt = 1.0;
	}
};
struct T_WVEP_IS875_2015
{
	double dBasicWindSpeed; // Basic Wind Speed (Vb) in m/s
	int nTerrainCategory; // Dropdown list terrain categories i.e  "I", "II" , "III" and  "IV", Default value will be  "I"
	int nBuildingClass; // Index of Building Class = A
	int nRiskCalcMode; // Risk Cofficient 0: Auto, 1: User  //diff
	int nClassOfStruct; // Class Of StrUCTURE "General buildings","Temporary sheds","Low hazard to life","Important buildings" //
	double dRiskCoefK1; // Risk Coefficient K1, k1 depends on Class of Structure 
	BOOL bIncludeTopo; // Include Topographic Effect
	double dTopoK3; // Topographic Factor K3, 1.0 for no topographic effect // k2(1~1.36) // dtropographicfactor
	double dVertK3; // Vertical Topographic Factor K3, 0.0 for no vertical topographic effect // dtropographivRange
	BOOL bIncludeCyclone; // Include Cyclone Effect // bInclCyclonicRegEffects
	int nImpStructure; // Importance Structure Index,"Post cyclonic structure" , "Industrial structure" ,"Other Structures"
	double dImpFactorK4; // dk4Fact//Importance Factor K4, 1.0 for no importance factor,"Post cyclonic structure = 1.3 " , "Industrial structure =" ,"Other Structures = 1.0"
	void Initialize() {
		dBasicWindSpeed = 33.0; // Basic Wind Speed (Vb) in m/s
		nTerrainCategory = 0; // Index of Terrain Category
		nBuildingClass = 0; // Index of Building Class = A
		nRiskCalcMode = 0; // Risk Cofficient 0: Auto, 1: User
		nClassOfStruct = 0;
		dRiskCoefK1 = 1.0; // - Default value for coefficient  (k1) is "1"
		bIncludeTopo = FALSE; //  Topographic Effect
		dTopoK3 = 1.0; // Topographic Factor K3, 1.0 
		dVertK3 = 0.0;
		bIncludeCyclone = FALSE;
		nImpStructure = 2; // “Other Structures” index
		dImpFactorK4 = 1.0;
		
	}
};

struct T_WVEP_D_856
{
	int nCodeType;
	CString StrName;
	T_WVEP_KBC2009 KBC2009;
	T_WVEP_CH2012  CH2012 ;
	T_WVEP_CH2001  CH2001 ;
};

struct T_WVEP_D_890
{
	int nCodeType;
	CString StrName;
	T_WVEP_KBC2016 KBC2016;
	T_WVEP_KBC2009 KBC2009;
	T_WVEP_CH2012  CH2012 ;
	T_WVEP_CH2001  CH2001 ;
	void Initialize()
	{
		nCodeType=0;
		StrName.Empty();
		KBC2016.Initialize();
		KBC2009.Initialize();
		CH2012 .Initialize();
		CH2001 .Initialize();
	}
	T_WVEP_D_890()
	{
		Initialize();
	}
	T_WVEP_D_890(T_WVEP_D_890& src)
	{
		*this = src;
	}
	T_WVEP_D_890& operator=(T_WVEP_D_890& src)
	{
		nCodeType = src.nCodeType;
		StrName   = src.StrName;
		KBC2016   = src.KBC2016;
		KBC2009   = src.KBC2009;
		CH2012    = src.CH2012;
		CH2001    = src.CH2001;
		return *this;
	}
	void Convert856(T_WVEP_D_856& rData)
	{
		nCodeType = rData.nCodeType;
		StrName   = rData.StrName;
		KBC2016.Initialize();
		KBC2009   = rData.KBC2009;
		CH2012    = rData.CH2012;
		CH2001    = rData.CH2001;
	};
};

struct T_WVEP_D_895
{
	int nCodeType;
	CString StrName;
	T_WVEP_KBC2016 KBC2016;
	T_WVEP_KBC2009 KBC2009;
	T_WVEP_CH2012  CH2012 ;
	T_WVEP_CH2001  CH2001 ;
	T_WVEP_CH2019  CH2019 ;
};

struct T_WVEP_D_900
{
	int nCodeType;
	
	CString StrName;
	T_WVEP_KBC2016 KDS2019;
	T_WVEP_KBC2016 KBC2016;
	T_WVEP_KBC2009 KBC2009;
	T_WVEP_CH2012  CH2012;
	T_WVEP_CH2001  CH2001;
	T_WVEP_CH2019  CH2019;
};

union T_WVEP_CODE
{
	T_WVEP_KDS2021 KDS2021;
	T_WVEP_KBC2016 KDS2019;
	T_WVEP_KBC2016 KBC2016;
	T_WVEP_KBC2009 KBC2009;
	T_WVEP_IS875_2015 IS875_2015;
	T_WVEP_CH2012  CH2012;
	T_WVEP_CH2001  CH2001;
	T_WVEP_CH2019  CH2019;
	T_WVEP_ASCE7_16 ASCE7_16;
	T_WVEP_ASCE7_16 ASCE7_22;
	T_WVEP_ASCE7_16 NSCP2024;
	void Initialize(int nCode)
	{
		switch (nCode)
		{
		case CH_W_2002:				CH2001.Initialize();		break;
		case CH_W_2012:				CH2012.Initialize();		break;
		case CH_W_2021:				CH2019.Initialize();		break;
		case KBC_W_2009:			KBC2009.Initialize();		break;
		case KBC_W_2016:			KBC2016.Initialize();		break;
		case KDS_W_2019:			KDS2019.Initialize();		break;
		case KDS_W_2022:      		KDS2021.Initialize();		break;
		case ASCE7_W_2016:			ASCE7_16.Initialize();		break;
		case ASCE7_W_2022:      	ASCE7_22.Initialize();		break;
		case NSCP_W_2024:     	    NSCP2024.Initialize();		break;
		case IS_W_875_2015:			IS875_2015.Initialize();	break;
		}
	}
};
struct T_WVEP_D
{
	int nCodeType;
	// 1=KS.Arch.1992	2=JAPAN.1987	3=UBC.1997		4=ANSI.1982				5=KS.Arch.2000
	// 6=IBC.2000		7=EURO-1(1992)	8=BS6399(1997)	9=CH2002				10=JPN2000
	// 11=NBC1995		12=IS1987		13=TAIWAN86		14=JP2004				15=EURO2005
	// 16=KBC(2009)		17=IBC2009		18=IBC2012		19=CH2012				20=NSR-10
	// 21=KBC(2016)		22=IS875(2015)	23=CH2019		24=KDS(41-10-15:2019)   25=KDS(41-12:2022)
	CString StrName;
	T_WVEP_CODE CodeParam;
	void Initialize()
	{
		nCodeType= KDS_W_2022;
		StrName.Empty();
		CodeParam.Initialize(nCodeType);
	}
	T_WVEP_D()
	{
		Initialize();
	}
	T_WVEP_D(T_WVEP_D& src)
	{
		*this = src;
	}
	T_WVEP_D& operator=(T_WVEP_D& src)
	{
		nCodeType = src.nCodeType;
		StrName   = src.StrName;
		CodeParam = src.CodeParam;
		
		return *this;
	}
	void Convert856(T_WVEP_D_856& rData)
	{
		nCodeType = rData.nCodeType;
		StrName   = rData.StrName;
		switch (nCodeType)
		{
		case CH_W_2002:				CodeParam.CH2001 = rData.CH2001;		break;
		case CH_W_2012:				CodeParam.CH2012 = rData.CH2012;		break;
		case KBC_W_2009:			CodeParam.KBC2009 = rData.KBC2009;		break;
		default:
			break;
		}

	};
	void Convert890(T_WVEP_D_890& rData)
	{
		nCodeType = rData.nCodeType;
		StrName   = rData.StrName;
		switch (nCodeType)
		{
		case CH_W_2002:				CodeParam.CH2001 = rData.CH2001;		break;
		case CH_W_2012:				CodeParam.CH2012 = rData.CH2012;		break;
		case KBC_W_2009:			CodeParam.KBC2009 = rData.KBC2009;		break;
		case KBC_W_2016:			CodeParam.KBC2016 = rData.KBC2016;		break;
		default:
			break;
		}
	};
	void Convert895(T_WVEP_D_895& rData)
	{
		nCodeType	= rData.nCodeType;
		StrName		= rData.StrName;
		switch (nCodeType)
		{
		case CH_W_2002:				CodeParam.CH2001 = rData.CH2001;		break;
		case CH_W_2012:				CodeParam.CH2012 = rData.CH2012;		break;
		case CH_W_2021:				CodeParam.CH2019 = rData.CH2019;		break;
		case KBC_W_2009:			CodeParam.KBC2009 = rData.KBC2009;		break;
		case KBC_W_2016:			CodeParam.KBC2016 = rData.KBC2016;		break;
		default:
			break;
		}
	};
	void Convert900(T_WVEP_D_900& rData)
	{
		nCodeType = rData.nCodeType;
		StrName = rData.StrName;
		switch (nCodeType)
		{
		case CH_W_2002:				CodeParam.CH2001 = rData.CH2001;		break;
		case CH_W_2012:				CodeParam.CH2012 = rData.CH2012;		break;
		case CH_W_2021:				CodeParam.CH2019 = rData.CH2019;		break;
		case KBC_W_2009:			CodeParam.KBC2009 = rData.KBC2009;		break;
		case KBC_W_2016:			CodeParam.KBC2016 = rData.KBC2016;		break;
		case KDS_W_2019:			CodeParam.KDS2019 = rData.KDS2019;		break;

		default:
			break;
		}
	};
	T_WVEP_KBC2016* GetKBC2016()
	{
		switch (nCodeType)
		{
		case KBC_W_2016: return &CodeParam.KBC2016;
		case KDS_W_2019: return &CodeParam.KDS2019;
		default:
			return NULL;
			break;
		}
	}
	T_WVEP_ASCE7_16* GetASCE7Series()
	{
		switch (nCodeType)
		{
		case ASCE7_W_2016: return &CodeParam.ASCE7_16;
		case ASCE7_W_2022: return &CodeParam.ASCE7_22;
		case NSCP_W_2024:  return &CodeParam.NSCP2024;
		default: return NULL;
		}
	}
	T_WVEP_IS875_2015* GetIS875_2015()
	{
		switch (nCodeType)
		{
		case IS_W_875_2015: return &CodeParam.IS875_2015;
		default:
			return NULL;
			break;
		}
	}
};

struct T_WVEP_D_CH_856
{
	int nCodeType;  //아래의 주석은 원래 윈도 로드 구조체 T_WIND_D 에 있던것. 일단 참조하자.
	// 1  =KS.Arch.1992 2  =JAPAN.1987   3  =UBC.1997 4  =ANSI.1982  5  =KS.Arch.2000  6  =IBC.2000
	// 7  =EURO-1(1992) 8  =BS6399(1997) 9  =CH2002   10 =JPN2000    11 =NBC1995       12 =IS1987
	// 13 =TAIWAN86     14 =JP2004       15 =EURO2005 16 =KBC(2009)  17 =IBC2009       18 =IBC2012
	// 19 =CH2012       Code 추가시 wg_main의 ModelTextOut.cpp에 반영할 것
	char StrName[80];
	T_WVEP_KBC2009 KBC2009;
	T_WVEP_CH2012  CH2012 ;
	T_WVEP_CH2001  CH2001 ;

	void ConvertToString(T_WVEP_D_856& rData)
	{
		rData.nCodeType = nCodeType;
		ConvertCharStr(StrName, rData.StrName, sizeof(StrName));
		rData.KBC2009   = KBC2009;
		rData.CH2012    = CH2012;
		rData.CH2001    = CH2001;
	}
};

struct T_WVEP_D_CH_890
{
	int nCodeType;
	char StrName[80];
	T_WVEP_KBC2016 KBC2016;
	T_WVEP_KBC2009 KBC2009;
	T_WVEP_CH2012  CH2012 ;
	T_WVEP_CH2001  CH2001 ;

	void ConvertToString(T_WVEP_D_890& rData)
	{
		rData.nCodeType = nCodeType;
		ConvertCharStr(StrName, rData.StrName, sizeof(StrName));
		rData.KBC2016   = KBC2016;
		rData.KBC2009   = KBC2009;
		rData.CH2012    = CH2012;
		rData.CH2001    = CH2001;
	}
	void ConvertToChar(T_WVEP_D_890& rData)
	{
		nCodeType = rData.nCodeType;
		ConvertStrChar(rData.StrName, StrName, sizeof(StrName));
		KBC2016   = rData.KBC2016;
		KBC2009   = rData.KBC2009;
		CH2012    = rData.CH2012;
		CH2001    = rData.CH2001;
	}
};

struct T_WVEP_D_CH_895
{
	int nCodeType;
	char StrName[80];
	T_WVEP_KBC2016 KBC2016;
	T_WVEP_KBC2009 KBC2009;
	T_WVEP_CH2012  CH2012 ;
	T_WVEP_CH2001  CH2001 ;
	T_WVEP_CH2019  CH2019 ;

	void ConvertToString(T_WVEP_D_895& rData)
	{
		rData.nCodeType = nCodeType;
		ConvertCharStr(StrName, rData.StrName, sizeof(StrName));
		rData.KBC2016   = KBC2016;
		rData.KBC2009   = KBC2009;
		rData.CH2012	= CH2012;
		rData.CH2001	= CH2001;
		rData.CH2019	= CH2019;
	}
	void ConvertToChar(T_WVEP_D_895& rData)
	{
		nCodeType = rData.nCodeType;
		ConvertStrChar(rData.StrName, StrName, sizeof(StrName));
		KBC2016   = rData.KBC2016;
		KBC2009   = rData.KBC2009;
		CH2012    = rData.CH2012;
		CH2001    = rData.CH2001;
		CH2019    = rData.CH2019;
	}
};

struct T_WVEP_D_CH_900
{
	int nCodeType;
	char StrName[80];
	T_WVEP_KBC2016 KDS2019;
	T_WVEP_KBC2016 KBC2016;
	T_WVEP_KBC2009 KBC2009;
	T_WVEP_CH2012  CH2012;
	T_WVEP_CH2001  CH2001;
	T_WVEP_CH2019  CH2019;

	void ConvertToString(T_WVEP_D_900& rData)
	{
		rData.nCodeType = nCodeType;
		ConvertCharStr(StrName, rData.StrName, sizeof(StrName));
		rData.KDS2019 = KDS2019;
		rData.KBC2016 = KBC2016;
		rData.KBC2009 = KBC2009;
		rData.CH2012 = CH2012;
		rData.CH2001 = CH2001;
		rData.CH2019 = CH2019;
	}
	void ConvertToChar(T_WVEP_D_900& rData)
	{
		nCodeType = rData.nCodeType;
		ConvertStrChar(rData.StrName, StrName, sizeof(StrName));
		KDS2019 = rData.KDS2019;
		KBC2016 = rData.KBC2016;
		KBC2009 = rData.KBC2009;
		CH2012 = rData.CH2012;
		CH2001 = rData.CH2001;
		CH2019 = rData.CH2019;
	}
};

struct T_WVEP_D_CH
{
	int nCodeType;
	char StrName[80];
	T_WVEP_CODE CodeParam;


	void ConvertToString(T_WVEP_D& rData)
	{
		rData.nCodeType = nCodeType;
		ConvertCharStr(StrName, rData.StrName, sizeof(StrName));
		switch (nCodeType)
		{
		case CH_W_2002:	 rData.CodeParam.CH2001  = CodeParam.CH2001;  break;
		case CH_W_2012:	 rData.CodeParam.CH2019  = CodeParam.CH2019;  break;
		case CH_W_2021:	 rData.CodeParam.CH2019  = CodeParam.CH2019;  break;
		case KBC_W_2009: rData.CodeParam.KBC2009 = CodeParam.KBC2009; break;
		case IS_W_875_2015: rData.CodeParam.IS875_2015 = CodeParam.IS875_2015; break; // anshika
		case KBC_W_2016: rData.CodeParam.KBC2016 = CodeParam.KBC2016; break;
		case KDS_W_2019: rData.CodeParam.KDS2019 = CodeParam.KDS2019; break;
		case KDS_W_2022: rData.CodeParam.KDS2021 = CodeParam.KDS2021; break;
		case ASCE7_W_2016: rData.CodeParam.ASCE7_16 = CodeParam.ASCE7_16; break;
		case ASCE7_W_2022: rData.CodeParam.ASCE7_22 = CodeParam.ASCE7_22; break;
		case NSCP_W_2024:  rData.CodeParam.NSCP2024 = CodeParam.NSCP2024; break;
		}
	}
	void ConvertToChar(T_WVEP_D& rData)
	{
		nCodeType = rData.nCodeType;
		ConvertStrChar(rData.StrName, StrName, sizeof(StrName));
		switch (nCodeType)
		{
		case CH_W_2002:	 CodeParam.CH2001  = rData.CodeParam.CH2001;  break;
		case CH_W_2012:	 CodeParam.CH2019  = rData.CodeParam.CH2019;  break;
		case CH_W_2021:	 CodeParam.CH2019  = rData.CodeParam.CH2019;  break;
		case KBC_W_2009: CodeParam.KBC2009 = rData.CodeParam.KBC2009; break;
		case IS_W_875_2015: CodeParam.IS875_2015 = rData.CodeParam.IS875_2015; break;
		case KBC_W_2016: CodeParam.KBC2016 = rData.CodeParam.KBC2016; break;
		case KDS_W_2019: CodeParam.KDS2019 = rData.CodeParam.KDS2019; break;
		case KDS_W_2022: CodeParam.KDS2021 = rData.CodeParam.KDS2021; break;
		case ASCE7_W_2016: CodeParam.ASCE7_16 = rData.CodeParam.ASCE7_16; break;
		case ASCE7_W_2022: CodeParam.ASCE7_22 = rData.CodeParam.ASCE7_22; break;
		case NSCP_W_2024:  CodeParam.NSCP2024 = rData.CodeParam.NSCP2024; break;
		}
	}
};

struct T_WVEP_UDRD_D
{
	T_WVEP_K key;
	T_WVEP_D data;
};
struct T_WVEP_UDRD_D_CH_856
{
	T_WVEP_K key;
	T_WVEP_D_CH_856 data;
};
struct T_WVEP_UDRD_D_CH_890
{
	T_WVEP_K key;
	T_WVEP_D_CH_890 data;
};
struct T_WVEP_UDRD_D_CH_895
{
	T_WVEP_K key;
	T_WVEP_D_CH_895 data;
};

struct T_WVEP_UDRD_D_CH_900
{
	T_WVEP_K key;
	T_WVEP_D_CH_900 data;
};
struct T_WVEP_UDRD_D_CH
{
	T_WVEP_K key;
	T_WVEP_D_CH data;
};

struct T_WVEP_UNIT
{
	T_WVEP_UNIT()
	{
		nCodeType = D_UNITSYS_NONE;
		StrName   = D_UNITSYS_NONE;
		KBC2009_nExposureCategory  = D_UNITSYS_NONE;
		KBC2009_dBasicWindSpeed    = D_UNITSYS_NONE;
		KBC2009_dImportanceFactor  = D_UNITSYS_NONE;
		KBC2009_dRoofHeight        = D_UNITSYS_BASE_LENGTH;
		KBC2009_bTopographicEffect = D_UNITSYS_NONE;
		KBC2009_dKzt = D_UNITSYS_NONE;
		KBC2009_dVzt = D_UNITSYS_BASE_LENGTH;
		KDS2021_dMass = D_UNITSYS_BASE_MASS;
		CH2012_nRoughCategory     = D_UNITSYS_NONE;
		CH2012_dBasicWindPressure = D_UNITSYS_NONE;
		CH2012_dTopographyCoef    = D_UNITSYS_NONE;
		CH2001_nRoughCategory     = D_UNITSYS_NONE;
		CH2001_dBasicWindPressure = D_UNITSYS_NONE;
		CH2001_dTopographyCoef    = D_UNITSYS_NONE;
		CH2019_nRoughCategory     = D_UNITSYS_NONE;
		CH2019_dBasicWindPressure = D_UNITSYS_NONE;
		CH2019_dTopographyCoef    = D_UNITSYS_NONE;
		CH2019_dWindDirectionCoe    = D_UNITSYS_NONE;
	}

	int nCodeType;
	int StrName;
	int KBC2009_nExposureCategory ;
	int KBC2009_dBasicWindSpeed   ;
	int KBC2009_dImportanceFactor ;
	int KBC2009_dRoofHeight       ;
	int KBC2009_bTopographicEffect;
	int KBC2009_dKzt              ;
	int KBC2009_dVzt              ;
	int KDS2021_dMass			  ;
	int CH2012_nRoughCategory     ;
	int CH2012_dBasicWindPressure ;
	int CH2012_dTopographyCoef    ;
	int CH2001_nRoughCategory     ;
	int CH2001_dBasicWindPressure ;
	int CH2001_dTopographyCoef    ;
	int CH2019_nRoughCategory     ;
	int CH2019_dBasicWindPressure ;
	int CH2019_dTopographyCoef    ;
	int CH2019_dWindDirectionCoe  ;
}; 
#define HASHSIZEWVEP 11

//////////////////////////////////////////////////////////////////////////
// 2. Wind Pressure Function
//////////////////////////////////////////////////////////////////////////

enum WDFC_METHOD { WDFC_METHOD_FUNC = 0, WDFC_METHOD_USER };

#define T_WDFC_K unsigned int

struct T_WDFC_D
{
	CString strName;
	int nCoordType; //1:Rectangular, 2:Cylindrical
	CString strEquation;
	CString strDescription;
	int    nFixIdx; //중국요청사항으로 마지막에 계산한 정보 기억하기.
	double dVal1Start; // From
	double dVal1End;   // To
	double dVal1Inc;   // Increment
	double dVal2;
	double dVal3;
	CArray<double, double> caPresure;
	int nMethod; // 0 :Function, 1 : User's Input

	void Initialize()
	{
		strName.Empty();
		nCoordType = 1;	
		strEquation.Empty();
		strDescription.Empty();
		nFixIdx = 0;
		dVal1Start = 0.0;
		dVal1End = 0.0;
		dVal1Inc = 0.0;
		dVal2 = 0.0;
		dVal3 = 0.0;
		caPresure.RemoveAll();
		nMethod = WDFC_METHOD_FUNC;
	}
	T_WDFC_D()
	{
		Initialize();
	}
	T_WDFC_D(T_WDFC_D& src)
	{
		*this = src;
	}
	T_WDFC_D& operator=(T_WDFC_D& src)
	{
		strName        = src.strName;
		nCoordType     = src.nCoordType;
		strEquation    = src.strEquation;
		strDescription = src.strDescription;
		nFixIdx        = src.nFixIdx;
		dVal1Start     = src.dVal1Start;
		dVal1End       = src.dVal1End;
		dVal1Inc       = src.dVal1Inc;
		dVal2          = src.dVal2;
		dVal3          = src.dVal3;
		caPresure.Copy(src.caPresure);
		nMethod = src.nMethod;
		return *this;
	}
};

struct T_WDFC_D_CH_896
{
	char strName[80];
	int nCoordType; //1:Rectangular, 2:Cylindrical
	char strEquation[80];
	char strDescription[80];
};

struct T_WDFC_D_CH_935
{
	char strName[80];
	int nCoordType; //1:Rectangular, 2:Cylindrical
	char strEquation[80];
	char strDescription[80];
	int    nFixIdx;
	double dVal1Start;
	double dVal1End;
	double dVal1Inc;
	double dVal2;
	double dVal3;
	int nCountPressure;
};

struct T_WDFC_D_CH
{
	char strName[80];
	int nCoordType; //1:Rectangular, 2:Cylindrical
	char strEquation[200];
	char strDescription[80];
	int    nFixIdx;
	double dVal1Start;
	double dVal1End;
	double dVal1Inc;
	double dVal2;
	double dVal3;
	CArray<double, double> caPresure;
	int nMethod; // 0 :Function, 1 : User's Input

	void ConvertToString(T_WDFC_D& rData)
	{
		ConvertCharStr(strName, rData.strName, sizeof(strName));
		rData.nCoordType = nCoordType;
		ConvertCharStr(strEquation, rData.strEquation, sizeof(strEquation));
		ConvertCharStr(strDescription, rData.strDescription, sizeof(strDescription));
		rData.nFixIdx = nFixIdx;
		rData.dVal1Start = dVal1Start;
		rData.dVal1End = dVal1End;
		rData.dVal1Inc = dVal1Inc;
		rData.dVal2 = dVal2;
		rData.dVal3 = dVal3;
		rData.caPresure.Copy(caPresure);
		rData.nMethod = nMethod;
	}
	void ConvertToChar(T_WDFC_D& rData)
	{
		ConvertStrChar(rData.strName, strName, sizeof(strName));
		nCoordType = rData.nCoordType;
		ConvertStrChar(rData.strEquation, strEquation, sizeof(strEquation));
		ConvertStrChar(rData.strDescription, strDescription, sizeof(strDescription));
		nFixIdx = rData.nFixIdx;
		dVal1Start = rData.dVal1Start;
		dVal1End = rData.dVal1End;
		dVal1Inc = rData.dVal1Inc;
		dVal2 = rData.dVal2;
		dVal3 = rData.dVal3;
		caPresure.Copy(rData.caPresure);
		nMethod = rData.nMethod;
	}
};

struct T_WDFC_UDRD_D
{
	T_WDFC_K key;
	T_WDFC_D data;
};
struct T_WDFC_UDRD_D_CH_935
{
	T_WDFC_K key;
	T_WDFC_D_CH_935 data;
};
struct T_WDFC_UDRD_D_CH_896
{
	T_WDFC_K key;
	T_WDFC_D_CH_896 data;
};

struct T_WDFC_UDRD_D_CH_RW_955
{
	T_WDFC_K key;
	char strName[80];
	int nCoordType; //1:Rectangular, 2:Cylindrical
	char strEquation[80];
	char strDescription[80];
	int    nFixIdx;
	double dVal1Start;
	double dVal1End;
	double dVal1Inc;
	double dVal2;
	double dVal3;
	int nCountPressure;
	int nMethod; // 0 :Function, 1 : User's Input
};

struct T_WDFC_UDRD_D_CH_RW 
{
	T_WDFC_K key;
	char strName[80];
	int nCoordType; //1:Rectangular, 2:Cylindrical
	char strEquation[200];
	char strDescription[80];
	int    nFixIdx;
	double dVal1Start;
	double dVal1End;
	double dVal1Inc;
	double dVal2;
	double dVal3;
	int nCountPressure;
	int nMethod; // 0 :Function, 1 : User's Input

	void Initialize()
	{
		key = 0;
		memset(strName, ' ', 80);
		nCoordType = 1;
		memset(strEquation, ' ', 200);
		memset(strDescription, ' ', 80);
		nFixIdx = 0;
		dVal1Start = 0.0;
		dVal1End = 0.0;
		dVal1Inc = 0.0;
		dVal2 = 0.0;
		dVal3 = 0.0;
		nCountPressure = 0;
		nMethod = WDFC_METHOD_FUNC;
	}

	void GetWDFC(T_WDFC_K& rKey, T_WDFC_D_CH& rData)
	{
		rKey = key;
		memcpy(rData.strName, strName, sizeof(strName));
		rData.nCoordType = nCoordType;
		memcpy(rData.strEquation, strEquation, sizeof(strEquation));
		memcpy(rData.strDescription, strDescription, sizeof(strDescription));
		rData.nFixIdx = nFixIdx;
		rData.dVal1Start = dVal1Start;
		rData.dVal1End = dVal1End;
		rData.dVal1Inc = dVal1Inc;
		rData.dVal2 = dVal2;
		rData.dVal3 = dVal3;
		rData.nMethod = nMethod;
	}

	void SetWDFC(T_WDFC_K Key, const T_WDFC_D_CH& rData)
	{
		key = Key;
		memcpy(strName, rData.strName, sizeof(strName));
		nCoordType = rData.nCoordType;
		memcpy(strEquation, rData.strEquation, sizeof(strEquation));
		memcpy(strDescription, rData.strDescription, sizeof(strDescription));
		nFixIdx = rData.nFixIdx;
		dVal1Start = rData.dVal1Start;
		dVal1End = rData.dVal1End;
		dVal1Inc = rData.dVal1Inc;
		dVal2 = rData.dVal2;
		dVal3 = rData.dVal3;
		nCountPressure = (int)rData.caPresure.GetSize();
		nMethod = rData.nMethod;
	}
	void Convert955(T_WDFC_UDRD_D_CH_RW_955& rOldData)
	{
		key = rOldData.key;
		memcpy(strName, rOldData.strName, sizeof(strName));
		nCoordType = rOldData.nCoordType;
		memcpy(strEquation, rOldData.strEquation, sizeof(strEquation));
		memcpy(strDescription, rOldData.strDescription, sizeof(strDescription));
		nFixIdx = rOldData.nFixIdx;
		dVal1Start = rOldData.dVal1Start;
		dVal1End = rOldData.dVal1End;
		dVal1Inc = rOldData.dVal1Inc;
		dVal2 = rOldData.dVal2;
		dVal3 = rOldData.dVal3;
		nCountPressure = rOldData.nCountPressure;
		nMethod = rOldData.nMethod;
	}
	void Convert935(T_WDFC_UDRD_D_CH_935& rOldData)
	{
		key = rOldData.key;
		memcpy(strName, rOldData.data.strName, sizeof(strName));
		nCoordType = rOldData.data.nCoordType;
		memcpy(strEquation, rOldData.data.strEquation, sizeof(strEquation));
		memcpy(strDescription, rOldData.data.strDescription, sizeof(strDescription));
		nFixIdx = rOldData.data.nFixIdx;
		dVal1Start = rOldData.data.dVal1Start;
		dVal1End = rOldData.data.dVal1End;
		dVal1Inc = rOldData.data.dVal1Inc;
		dVal2 = rOldData.data.dVal2;
		dVal3 = rOldData.data.dVal3;
		nCountPressure = rOldData.data.nCountPressure;
		nMethod = nCountPressure > 0 ? WDFC_METHOD_USER : WDFC_METHOD_FUNC;
	}
	void Convert896(T_WDFC_UDRD_D_CH_896& rOldData)
	{
		key = rOldData.key;
		memcpy(strName, rOldData.data.strName, sizeof(strName));
		nCoordType = rOldData.data.nCoordType;
		memcpy(strEquation, rOldData.data.strEquation, sizeof(strEquation));
		memcpy(strDescription, rOldData.data.strDescription, sizeof(strDescription));
		nFixIdx = 0;
		dVal1Start = 0.0;
		dVal1End = 0.0;
		dVal1Inc = 0.0;
		dVal2 = 0.0;
		dVal3 = 0.0;
		nCountPressure = 0;
		nMethod = WDFC_METHOD_FUNC;
	}
};

struct T_WDFC_UNIT
{
	T_WDFC_UNIT()
	{
		strName        = D_UNITSYS_NONE;
		nCoordType     = D_UNITSYS_NONE;
		strEquation    = D_UNITSYS_NONE;
		strDescription = D_UNITSYS_NONE;
	}
	int strName;
	int nCoordType;
	int strEquation;
	int strDescription;
}; 
#define HASHSIZEWDFC 11

//////////////////////////////////////////////////////////////////////////
// 2. Wind Pressure.
//////////////////////////////////////////////////////////////////////////

#define T_WDPR_K unsigned int

struct T_WIND_PRESSURE_PROFILE_856
{
	double dLevel;     // 높이
	double dWindWidth; // 바람의 방향의 너비.
	double dWindThick; // 바람의 방향의 두께.
	double dPressure;  // 규준에 따른 풍압.
	double dAdditionalPressure; // 사용자 입력 추가 풍압.
};

struct T_WIND_PRESSURE_PROFILE
{
	double dLevel;     // 높이
	double dWindWidth; // 바람의 방향의 너비.
	double dWindThick; // 바람의 방향의 두께.
	double dPressure;  // 규준에 따른 풍압.
	double dAdditionalPressure; // 사용자 입력 추가 풍압.

	double dPressureA;						// 규준에 따른 풍압. (풍직각)
	double dAdditionalPressureA;	// 사용자 입력 추가 풍압. (풍직각)
	double dPressureT;						// 규준에 따른 풍압. (풍비틀림)
	double dAdditionalPressureT;	// 사용자 입력 추가 풍압. (풍비틀림)

	T_WIND_PRESSURE_PROFILE()
	{
		dLevel = 0.0;     // 높이
		dWindWidth = 0.0; // 바람의 방향의 너비.
		dWindThick = 0.0; // 바람의 방향의 두께.
		dPressure = 0.0;  // 규준에 따른 풍압.
		dAdditionalPressure = 0.0; // 사용자 입력 추가 풍압.

		dPressureA = 0.0;
		dAdditionalPressureA = 0.0;
		dPressureT = 0.0;
		dAdditionalPressureT = 0.0;
	}

	BOOL operator==(const T_WIND_PRESSURE_PROFILE& src) const
	{
		if(fabs(dLevel - src.dLevel) > 1.e-10) return FALSE;
		if(fabs(dWindWidth - src.dWindWidth) > 1.e-10) return FALSE;
		if(fabs(dWindThick - src.dWindThick) > 1.e-10) return FALSE;
		if(fabs(dPressure - src.dPressure) > 1.e-10) return FALSE;
		if(fabs(dAdditionalPressure - src.dAdditionalPressure) > 1.e-10) return FALSE;

		if(fabs(dPressureA - src.dPressureA) > 1.e-10) return FALSE;
		if(fabs(dAdditionalPressureA - src.dAdditionalPressureA) > 1.e-10) return FALSE;
		if(fabs(dPressureT - src.dPressureT) > 1.e-10) return FALSE;
		if(fabs(dAdditionalPressureT - src.dAdditionalPressureT) > 1.e-10) return FALSE;

		return TRUE;
	}

	void Convert856(T_WIND_PRESSURE_PROFILE_856& rData)
	{
		dLevel			= rData.dLevel;
		dWindWidth	= rData.dWindWidth;
		dWindThick	= rData.dWindThick;
		dPressure		= rData.dPressure;
		dAdditionalPressure	= rData.dAdditionalPressure;
	}
};

struct T_WIND_NODAL_STRUCT
{
	int nType; // 상부구조물의 타입 1: Trapezoidal 2: Cylindrical, 3: Spherical, 4: Semi-Spherical
	double dB1;
	double dB2;
	double dB3;
	double dB4;
	double dR1;
	double dR2;
	double dH;
	double dAngleOfB1; //Trapezoidal 일때 방향을 결정.
	void Initialize()
	{
		nType = 1;
		dB1 = 0.0;
		dB2 = 0.0;
		dB3 = 0.0;
		dB4 = 0.0;
		dR1 = 0.0;
		dR2 = 0.0;
		dH  = 0.0;
		dAngleOfB1 = 0.0;
	}
};

struct T_WDPR_CH2001
{
	int    nPulseMagnifyMethod; // 1 : Automatic Calculation, 2: User Defined //590추가 맥동증대계수 계산방법
	double dDampingRatio;       // dPulseMagnifyFactor를 계산하기 위한 항목.(계산서를 위해 저장)
	double dPeriod;             // dPulseMagnifyFactor를 계산하기 위한 항목.(계산서를 위해 저장)
	double dPulseMagnifyFactor;
	double dWindwardCoef;     // Windward pressure coefficient
	double dLeewardCoef;      // Leeward pressure coefficient
	BOOL   bComfort;          // Comfort 811  maxiao@midasit.com
	double dCftWindPressure;  // Comfort 811  KN/m^2 [단위계고정]
	double dCftDampingRatio;  // Comfort 811
	void Initialize()
	{
		nPulseMagnifyMethod = 1;
		dDampingRatio = 0.01;
		dPeriod = 0.0;
		dPulseMagnifyFactor = 1.0;
		dWindwardCoef = 0.8;
		dLeewardCoef  = -0.5;
		bComfort = TRUE;
		dCftWindPressure = 0.3;
		dCftDampingRatio = 0.05;
	}
// 	T_WDPR_CH2001& operator=(const T_WDPR_CH2001& src)
// 	{
// 		nPulseMagnifyMethod = src.nPulseMagnifyMethod;
// 		dDampingRatio = src.dDampingRatio;
// 		dPeriod = src.dPeriod;
// 		dPulseMagnifyFactor = src.dPulseMagnifyFactor;
// 		dWindwardCoef = src.dWindwardCoef;
// 		dLeewardCoef  = src.dLeewardCoef;
// 		bComfort = src.bComfort;
// 		dCftWindPressure = src.dCftWindPressure;
// 		dCftDampingRatio = src.dCftDampingRatio;
// 		return *this;
// 	}
};

struct T_WDPR_CH2012_925
{
	int    nStructMethod;      // 1-table(G.0.3)  2-Calculation(8.4.7)
	double dDampingRatio;      // Damping Ratio
	double dBasicPeriod;       // Basic Period wind along
	double dBasicPeriod_Cross; // Basic Period wind Cross...(bCross)
	double dWindwardCoef;      // Windward pressure coefficient-X (leeward와 x,y방향은 구분 하지 않는다.)
	double dLeewardCoef;       // Leeward pressure coefficient (상부구조일 경우에만)

	BOOL   bConsiderPartition; // Partition Pressure coefficient
	double dPartitionCoef;
	BOOL   bConsiderWindShield;// WindShield coefficient
	double dWindShieldCoef;

	BOOL   bAlongVibration;    //Consider Along-wind vibration 
	BOOL   bCrossVibration;    //Consider cross-wind vibration, torsion
	BOOL   bCross;
	int    nSectType;          //1 : rectangle 2: Round
	double dCrossPeriod;
	BOOL   bTorsion;
	double dTorsionPeriod;

	BOOL   bWindComfort;       // Wind Comfort
	double dCftWindPressure;   // check Wind Pressure KN/m^2 [단위계고정]
	double dCftDampingRatio;   // check Damping Ratio
};

struct T_WDPR_CH2012
{
	int    nStructMethod;      // 1-table(G.0.3)  2-Calculation(8.4.7)
	double dDampingRatio;      // Damping Ratio
	double dBasicPeriod;       // Basic Period wind along
	double dBasicPeriod_Cross; // Basic Period wind Cross...(bCross)
	double dWindwardCoef;      // Windward pressure coefficient-X (leeward와 x,y방향은 구분 하지 않는다.)
	double dLeewardCoef;       // Leeward pressure coefficient (상부구조일 경우에만)

	BOOL   bConsiderPartition; // Partition Pressure coefficient
	double dPartitionCoef;
	BOOL   bConsiderWindShield;// WindShield coefficient
	double dWindShieldCoef;

	BOOL   bAlongVibration;    //Consider Along-wind vibration 
	BOOL   bCrossVibration;    //Consider cross-wind vibration, torsion
	BOOL   bCross;
	int    nSectType;          //1 : rectangle 2: Round
	double dCrossPeriod;	
	BOOL   bTorsion ;
	double dTorsionPeriod;
	int		nAlongType;

	BOOL   bWindComfort;       // Wind Comfort
	double dCftWindPressure;   // check Wind Pressure KN/m^2 [단위계고정]
	double dCftDampingRatio;   // check Damping Ratio	

	void Initialize()
	{
		nStructMethod        = 1;
		dDampingRatio        = 0.05;
		dBasicPeriod         = 0.0;
		dBasicPeriod_Cross   = 0.0;
		dWindwardCoef        = 0.8;
		dLeewardCoef         = -0.5;
		bConsiderPartition   = FALSE;
		dPartitionCoef       = 0.8;
		bConsiderWindShield  = FALSE;
		dWindShieldCoef      = 1.0;
		bAlongVibration      = TRUE;
		bCrossVibration      = FALSE;
		bCross               = TRUE;
		nSectType            = 1;
		dCrossPeriod         = 0.0;
		bTorsion             = TRUE;
		dTorsionPeriod       = 0.5;
		bWindComfort         = FALSE;
		dCftWindPressure     = 0.3;
		dCftDampingRatio     = 0.05;
		nAlongType = 0;
	}
	void Convert925(T_WDPR_CH2012_925& rOldData)
	{
		nStructMethod = rOldData.nStructMethod;
		dDampingRatio = rOldData.dDampingRatio;
		dBasicPeriod = rOldData.dBasicPeriod;
		dBasicPeriod_Cross = rOldData.dBasicPeriod_Cross;
		dWindwardCoef = rOldData.dWindwardCoef;
		dLeewardCoef = rOldData.dLeewardCoef;
		bConsiderPartition = rOldData.bConsiderPartition;
		dPartitionCoef = rOldData.dPartitionCoef;
		bConsiderWindShield = rOldData.bConsiderWindShield;
		dWindShieldCoef = rOldData.dWindShieldCoef;
		bAlongVibration = rOldData.bAlongVibration;
		bCrossVibration = rOldData.bCrossVibration;
		bCross = rOldData.bCross;
		nSectType = rOldData.nSectType;
		dCrossPeriod = rOldData.dCrossPeriod;
		bTorsion = rOldData.bTorsion;
		dTorsionPeriod = rOldData.dTorsionPeriod;
		bWindComfort = rOldData.bWindComfort;
		dCftWindPressure = rOldData.dCftWindPressure;
		dCftDampingRatio = rOldData.dCftDampingRatio;
		nAlongType = 0;
	}
};

struct T_WDPR_CH2019
{
	int    nStructMethod;      // 1-table(G.0.3)  2-Calculation(8.4.7)
	double dDampingRatio;      // Damping Ratio
	double dBasicPeriod;       // Basic Period wind along
	double dBasicPeriod_Cross; // Basic Period wind Cross...(bCross)
	double dWindwardCoef;      // Windward pressure coefficient-X (leeward와 x,y방향은 구분 하지 않는다.)
	double dLeewardCoef;       // Leeward pressure coefficient (상부구조일 경우에만)

	BOOL   bConsiderPartition; // Partition Pressure coefficient
	double dPartitionCoef;
	BOOL   bConsiderWindShield;// WindShield coefficient
	double dWindShieldCoef;

	BOOL   bAlongVibration;    //Consider Along-wind vibration 
	int    nAlongType;
	BOOL   bCrossVibration;    //Consider cross-wind vibration, torsion
	BOOL   bCross;
	int    nSectType;          //1 : rectangle 2: Round
	double dCrossPeriod;	
	BOOL   bTorsion ;
	double dTorsionPeriod;

	BOOL   bWindComfort;       // Wind Comfort
	double dCftWindPressure;   // check Wind Pressure KN/m^2 [단위계고정]
	double dCftDampingRatio;   // check Damping Ratio	

	void Initialize()
	{
		nStructMethod        = 1;
		dDampingRatio        = 0.05;
		dBasicPeriod         = 0.0;
		dBasicPeriod_Cross   = 0.0;
		dWindwardCoef        = 0.8;
		dLeewardCoef         = -0.5;
		bConsiderPartition   = FALSE;
		dPartitionCoef       = 0.8;
		bConsiderWindShield  = FALSE;
		dWindShieldCoef      = 1.0;
		bAlongVibration      = TRUE;
		bCrossVibration      = FALSE;
		bCross               = TRUE;
		nSectType            = 1;
		dCrossPeriod         = 0.0;
		bTorsion             = TRUE;
		dTorsionPeriod       = 0.5;
		bWindComfort         = FALSE;
		dCftWindPressure     = 0.3;
		dCftDampingRatio     = 0.05;
		nAlongType = 0;
	}
};

struct T_WDPR_ASCE7_16
{
	double dGfExternal;
	double dGfInternal;
	double dCfExternal;
	double dCfInternal;
	int    nRigidity;     // 1: Rigid Structure, 2: Flexible Structure
	double dBreath;
	double dNaturalFreq;  // Hz
	double dDampingRatio;
	double dKd;   // Directional Factor
	double dKe;   // ground elevation factor.

	BOOL   bAutoForceCoef;
	int	   nAutoForceCoefStructType;

	void Initialize()
	{
		dGfExternal = 2.2;
		dGfInternal = 0.0;
		dCfExternal = 0.8;
		dCfInternal = 0.0;
		nRigidity = 1;
		dBreath = 0.0;
		dNaturalFreq = 0.0;
		dDampingRatio = 0.0;
		dKd = 0.85;
		dKe = 1.0;

		bAutoForceCoef = FALSE;
		nAutoForceCoefStructType = 0;
	}
};

struct T_WDPR_KBC_AUTOCALC_856
{
	double dLevel;   //Level.
	int nStructType;	// 0: chimneys, Tanks, and similar structures
						// 1: solid signs
						// 2: open signs and lattice framework
						// 3: truss towers
	int nSubType1;	// nStructType: 0 -> 0: Square(Wind normal to face), 1: Square(Wind along diagonal), 2: Hexagonal or Octagonal, 3: Round
					// nStructType: 1 -> 0: At Ground Level, 1: Above Ground Level
					// nStructType: 2 -> 0: Flat-Sided Members, 1: Rounded Members
					// nStructType: 3 -> 0: Square Plan, 1: Triangle Plan
	int nSubType2;	// 0: Steel Rounded members, 1: Square cross-section members, 2: Other Type
					// (nStructType이 truss towers일때만 사용.)
	/*
		nStructType이 2,3(open signs and lattice framework, truss towers)일 때만,
		dXDir[0], dYDir[0]는 단위계를 가지지 않는다.
	*/
	double dHc;
	double dDirVal1; //dDir[0]
	double dDirVal2; //dDir[1]
	
	double dCoefFactor; //위의 값으로 계산해낸 결과.
};
struct T_WDPR_KBC_AUTOCALC_900
{
	double dLevel;   //Level.

	int nStructType;	// 0: chimneys, Tanks, and similar structures
										// 1: solid signs
										// 2: open signs and lattice framework
										// 3: truss towers
										// 4: circle plan
										// 5: solid walls and solid signs <= [PMS2015] 현재 Gen에서 제공하기 어려울 것 같으므로 제외함
										// 6: lattice towers
										// 7: roof structures
										// 8: fences on ground

	int nSubType1;		// nStructType: 0 -> 0: Square(Wind normal to face), 1: Square(Wind along diagonal), 2: Hexagonal or Octagonal, 3: Round
										// nStructType: 1 -> 0: At Ground Level, 1: Above Ground Level
										// nStructType: 2 -> 0: Flat-Sided Members, 1: Rounded Members
										// nStructType: 3 -> 0: Square Plan, 1: Triangle Plan
										// nStructType: 5 -> 0: Case A, 1: Case B, 2: Case C
										// nStructType: 6 -> 0: Square Plan, 1: Triangle Plan
										// nStructType: 7 -> 0: Square(Wind normal to face), 1: Square(Wind along diagonal), 2: Hexagonal or Octagonal, 3: Round

	int nSubType2;		// nStructType: 3 -> 0: Steel Rounded members, 1: Square cross-section members, 2: Other Type
										// nStructType: 6 -> 0: Angle members, 1: Steel Round members

	int nSubType3;		// nStructType: 6 -> 0: 90, 1: 45

	/*
		nStructType이 2,3,4,6,8 : dDir[0]는 단위가 없다.
		nStructType이 5					: dDir[3]는 단위가 없다.
	*/
	double dHc;
	double dDirVal1; //dDir[0]
	double dDirVal2; //dDir[1]
	double dDirVal3; //dDir[2]
	double dDirVal4; //dDir[3]

	double dCoefFactor; //위의 값으로 계산해낸 결과.
};

struct T_WDPR_KBC_AUTOCALC
{
	double dLevel;   //Level.

	int nStructType;	// 0: chimneys, Tanks, and similar structures
	                    // 1: solid signs
	                    // 2: open signs and lattice framework
	                    // 3: truss towers
	                    // 4: circle plan
	                    // 5: solid walls and solid signs <= [PMS2015] 현재 Gen에서 제공하기 어려울 것 같으므로 제외함
	                    // 6: lattice towers
	                    // 7: roof structures
	                    // 8: fences on ground

	int nSubType1;		// nStructType: 0 -> 0: Square(Wind normal to face), 1: Square(Wind along diagonal), 2: Hexagonal or Octagonal, 3: Round
						// nStructType: 1 -> 0: At Ground Level, 1: Above Ground Level
						// nStructType: 2 -> 0: Flat-Sided Members, 1: Rounded Members
						// nStructType: 3 -> 0: Square Plan, 1: Triangle Plan
						// nStructType: 5 -> 0: Case A, 1: Case B, 2: Case C
						// nStructType: 6 -> 0: Square Plan, 1: Triangle Plan
						// nStructType: 7 -> 0: Square(Wind normal to face), 1: Square(Wind along diagonal), 2: Hexagonal or Octagonal, 3: Round

	int nSubType2;		// nStructType: 3 -> 0: Steel Rounded members, 1: Square cross-section members, 2: Other Type
						// nStructType: 6 -> 0: Angle members, 1: Steel Round members
					
	int nSubType3;		// nStructType: 6 -> 0: 90, 1: 45

	/*
		nStructType이 2,3,4,6,8 : dDir[0]는 단위가 없다.
		nStructType이 5					: dDir[3]는 단위가 없다.
	*/
	double dHc;
	double dDirVal1; //dDir[0]
	double dDirVal2; //dDir[1]
	double dDirVal3; //dDir[2]
	double dDirVal4; //dDir[3]
	
	double dCoefFactor; //위의 값으로 계산해낸 결과.

	double dHeight;
	double dArea;

	T_WDPR_KBC_AUTOCALC()
	{
		Initialize();
	}
	void Initialize()
	{
		dLevel      = 0.0;
		nStructType = 0;
		nSubType1   = 0;
		nSubType2   = 0;
		nSubType3	= 0;
		dHc			= 0.0;
		dDirVal1    = 0.0;
		dDirVal2    = 0.0;
		dDirVal3    = 0.0;
		dDirVal4    = 0.0;
		dCoefFactor = 0.0;
		dHeight = 0.0;
		dArea = 0.0;

	}

	void Convert856(T_WDPR_KBC_AUTOCALC_856& rData)
	{
		Initialize();
		dLevel      = rData.dLevel      ;
		nStructType = rData.nStructType ;
		nSubType1   = rData.nSubType1   ;
		nSubType2   = rData.nSubType2   ;
		dHc			= rData.dHc			;
		dDirVal1    = rData.dDirVal1    ;
		dDirVal2    = rData.dDirVal2    ;
		dCoefFactor = rData.dCoefFactor ;
	}
	void Convert900(T_WDPR_KBC_AUTOCALC_900& rData)
	{
		Initialize();
		dLevel = rData.dLevel;
		nStructType = rData.nStructType;
		nSubType1 = rData.nSubType1;
		nSubType2 = rData.nSubType2;
		nSubType3 = rData.nSubType3;
		dHc		  = rData.dHc;
		dDirVal1 = rData.dDirVal1;
		dDirVal2 = rData.dDirVal2;
		dDirVal3 = rData.dDirVal3;
		dDirVal4 = rData.dDirVal4;

		dCoefFactor = rData.dCoefFactor;
	}
};

struct T_WDPR_KDS2021
{
	double dGfExternal;
	double dGfInternal;
	double dCfExternal;
	double dCfInternal;
	int    nRigidity;     // 1: Rigid Structure, 2: Flexible Structure
	double dBreath;
	double dNaturalFreq;  // Hz
	double dDampingRatio;
	double dKd;				// Directional Factor

	//Cf 자동계산... 기존에는 DB에 따로 저장하였으나, 그냥 이곳에 함께 저장한다.
	BOOL   bAutoForceCoef;
	int	nAutoForceCoefStructType;

	int			nBuildingType;					// 0: Middle Low Rise Building, 1:High Rise Building
	BOOL		bAlongWind;
	BOOL		bAcrossWind;
	BOOL		bTorsionalWind;
	double   dCenterPnt[3];

	// Parameters of Wind Vibration
	double	dBLy;	// X-Breadth
	double	dBLx;	// Y-Breadth
	double	dNoa;	// Natural Frequency
	double	dNot;	// Torsional Natural Frequency
	double	dZf;	// DampingRatio
	double  dTotalMass;
	double	dMx1st;									// X-1st vibration Generalized Mass
	double	dMy1st;									// Y-1st vibration Generalized Mass
	double  dVibrationMode;


	void Initialize()
	{
		dGfExternal = 2.2;
		dGfInternal = 0.0;
		dCfExternal = 0.8;
		dCfInternal = 0.0;
		nRigidity = 1;
		dBreath = 0.0;
		dNaturalFreq = 0.0;
		dDampingRatio = 0.0;
		bAutoForceCoef = FALSE;
		nAutoForceCoefStructType = 0;
		nBuildingType = 1;
		bAlongWind = TRUE;
		bAcrossWind = FALSE;
		bTorsionalWind = FALSE;
		dCenterPnt[0] = 0.0;
		dCenterPnt[1] = 0.0;
		dCenterPnt[2] = 0.0;
		dBLy = 0.;
		dBLx = 0.;
		dNoa = 0.;
		dNot = 0.;
		dZf = 0.;
		dMx1st = 0.;
		dMy1st = 0.;
		dTotalMass = 0.;
		dVibrationMode = 0.5;
		dKd = 1.0;
	}
	
};
struct T_WDPR_KBC2016
{
	double dGfExternal;
	double dGfInternal;
	double dCfExternal;
	double dCfInternal;
	int    nRigidity;     // 1: Rigid Structure, 2: Flexible Structure
	double dBreath;
	double dNaturalFreq;  // Hz
	double dDampingRatio;

	//Cf 자동계산... 기존에는 DB에 따로 저장하였으나, 그냥 이곳에 함께 저장한다.
	BOOL   bAutoForceCoef;
	int	nAutoForceCoefStructType;

	int			nBuildingType;					// 0: Middle Low Rise Building, 1:High Rise Building
	BOOL		bAlongWind;
	BOOL		bAcrossWind;
	BOOL		bTorsionalWind;
	double   dCenterPnt[3];

	// Parameters of Wind Vibration
	double	dBLy;	// X-Breadth
	double	dBLx;	// Y-Breadth
	double	dNoa;	// Natural Frequency
	double	dNot;	// Torsional Natural Frequency
	double	dZf;	// DampingRatio

	void Initialize()
	{
		dGfExternal = 2.2;
		dGfInternal = 0.0;
		dCfExternal = 0.8;
		dCfInternal = 0.0;
		nRigidity      = 1;
		dBreath        = 0.0;
		dNaturalFreq   = 0.0;
		dDampingRatio = 0.0;
		bAutoForceCoef = FALSE;
		nAutoForceCoefStructType = 0;
		nBuildingType		= 1;
		bAlongWind			= TRUE;
		bAcrossWind			= FALSE;
		bTorsionalWind	= FALSE;
		dCenterPnt[0] = 0.0;
		dCenterPnt[1] = 0.0;
		dCenterPnt[2] = 0.0;
		dBLy						= 0.;
		dBLx						= 0.;
		dNoa						= 0.;
		dNot						= 0.;
		dZf							= 0.;
	}
// 	T_WDPR_KBC2016& operator=(const T_WDPR_KBC2016& src)
// 	{
// 		dGfExternal = src.dGfExternal;
// 		dGfInternal = src.dGfInternal;
// 		dCfExternal = src.dCfExternal;
// 		dCfInternal = src.dCfInternal;
// 		nRigidity      = src.nRigidity;
// 		dBreath        = src.dBreath;
// 		dNaturalFreq   = src.dNaturalFreq;
// 		dDampingRatio = src.dDampingRatio;
// 		bAutoForceCoef = src.bAutoForceCoef;
// 		nAutoForceCoefStructType = src.nAutoForceCoefStructType;
// 		nBuildingType		= src.nBuildingType;
// 		bAlongWind			= src.bAlongWind;
// 		bAcrossWind			= src.bAcrossWind;
// 		bTorsionalWind	= src.bTorsionalWind;
// 		dCenterPnt[0]		= src.dCenterPnt[0];
// 		dCenterPnt[1]		= src.dCenterPnt[1];
// 		dCenterPnt[2]		= src.dCenterPnt[2];
// 		dBLy						= src.dBLy;
// 		dBLx						= src.dBLx;
// 		dNoa						= src.dNoa;
// 		dNot						= src.dNot;
// 		dZf							= src.dZf;
// 		return *this;
// 	}
};
struct T_WDPR_KBC2009
{
	double dGfExternal;
	double dGfInternal;
	double dCfExternal;
	double dCfInternal;
	int    nRigidity;     // 1: Rigid Structure, 2: Flexible Structure
	double dBreath;
	double dNaturalFreq;  // Hz
	double dDampingRatio;

	//Cf 자동계산... 기존에는 DB에 따로 저장하였으나, 그냥 이곳에 함께 저장한다.
	BOOL   bAutoForceCoef;

	void Initialize()
	{
	  dGfExternal = 2.2;
	  dGfInternal = 0.0;
	  dCfExternal = 0.8;
	  dCfInternal = 0.0;
	  nRigidity      = 1;
	  dBreath        = 0.0;
	  dNaturalFreq   = 0.0;
	  dDampingRatio = 0.0;
	  bAutoForceCoef = FALSE;
	}
	// 	T_WDPR_KBC2009& operator=(const T_WDPR_KBC2009& src)
	// 	{
	// 	  dGfExternal = src.dGfExternal;
	// 	  dGfInternal = src.dGfInternal;
	// 	  dCfExternal = src.dCfExternal;
	// 	  dCfInternal = src.dCfInternal;
	// 	  nRigidity      = src.nRigidity;
	// 	  dBreath        = src.dBreath;
	// 	  dNaturalFreq   = src.dNaturalFreq;
	// 	  dDampingRatio = src.dDampingRatio;
	// 	  bAutoForceCoef = src.bAutoForceCoef;
	// 	  return *this;
	// 	}
};
//---------------------
// IS875:2015 Wind Pressure Parameters
//---------------------
struct T_WDPR_IS875_2015
{
	double dkdfact;
	double dkcfact;     //kc factor = 1.0
	int    nPressureCoefMode; //0 = pressure Calc, 1 = force Defined
	double dCpe;   // External Pressure Coefficient
	double dCpi;   // Internal Pressure Coefficient
	double dForceCoef;
	BOOL bAutoKaCalc;	  // Auto calculate Ka factor
	double dKafact;		  // Area averaging factor ka factor

	void Initialize()
	{
		dkdfact = 1.0; // Default value for kdfact
		dkcfact = 1.0;
		nPressureCoefMode = 0; // Default to Pressure Coefficient
		dCpe = 0.8;
		dCpi = 0.0;
		dForceCoef = 0.8;
		bAutoKaCalc = TRUE;
		dKafact = 1.0;
	}
};
union T_WDPR_CODE
{
	T_WDPR_KDS2021 KDS2021;
	T_WDPR_KBC2016 KDS2019;
	T_WDPR_KBC2016 KBC2016;
	T_WDPR_KBC2009 KBC2009;
	T_WDPR_IS875_2015 IS875_2015; 
	T_WDPR_CH2012  CH2012;
	T_WDPR_CH2001  CH2001;
	T_WDPR_CH2019  CH2019;
	T_WDPR_ASCE7_16 ASCE7_16;
	T_WDPR_ASCE7_16 ASCE7_22;
	T_WDPR_ASCE7_16 NSCP2024;
	void Initialize(int nCode)
	{
		switch (nCode)
		{
		case CH_W_2002:				CH2001.Initialize();		break;
		case CH_W_2012:				CH2012.Initialize();		break;
		case CH_W_2021:				CH2019.Initialize();		break;
		case KBC_W_2009:			KBC2009.Initialize();		break;
		case IS_W_875_2015:         IS875_2015.Initialize();    break;  
		case KBC_W_2016:			KBC2016.Initialize();		break;
		case KDS_W_2019:			KDS2019.Initialize();		break;
		case KDS_W_2022:      		KDS2021.Initialize();		break;
		case ASCE7_W_2016:			ASCE7_16.Initialize();		break;
		case ASCE7_W_2022:      	ASCE7_22.Initialize();		break;
		case NSCP_W_2024:           NSCP2024.Initialize();      break;
		}
	}
};
struct T_WDPR_D
{
	T_LDGR_K KeyLdgr; //Load Group
	T_STLD_K KeyStld; //Load Case Name
	int      nDirection;   //1=X-Y, 2=Normal, 3=local y+, 4=local y-, 5=local z+, 6=local z-
	int      nType;        //1:Area, 2:Beam, 3:Nodal, 4:Function
	double   dAngle;
	double   dInnerPnt[3];   // 공간구조의 경우 Nomal을 구하기위한 좌표.
	double   dConcenPt[3];   // 상부구조의 경우 상부구조의 집중하중이 들어갈 좌표.
	double   dWindArea;      // 상부구조의 경우 상부구조의 영풍면 넓이.
	double   dCenterPnt[3];  // 함수 풍하중의 경우 원통형 일때 원통의 중심을 따로 입력받는다.
	double   dScaleFactor;
	int      nCodeType;   // 1=KS.Arch.1992  2=JAPAN.1987   3=UBC.1997  4=ANSI.1982  5=KS.Arch.2000   6=IBC.2000
	// 7=EURO-1(1992)  8=BS6399(1997) 9=CH2002    10=JPN2000   11=NBC1995       12=IS1987
	// 13=TAIWAN86     14=JP2004      15=EURO2005 16=KBC(2009) 17=IBC2009       18=IBC2012
	// 19=CH2012 Code 추가시 wg_main의 ModelTextOut.cpp에 반영할 것
	T_WVEP_K KeyWvep;     // Wind Velocity Pressure
	T_WDFC_K KeyWdfc;     // Wind Pressure Function... 함수.
	T_WDPR_CODE CodeParam;
	int      nSelection;    // 1=Group, 2=Element, 3=Node
	//T_LAGR_K KeyLagr;     // Loading Area Group
	T_LAPL_K KeyLapl;       // 결국 DB에는 Lapl단위로 저장이 된다....
	T_WIND_NODAL_STRUCT nodalStruct;
	CArray<T_ELEM_K, T_ELEM_K> arKeysDefineElem;
	CArray<T_NODE_K, T_NODE_K> arKeysDefineNode;
	CArray<T_WIND_PRESSURE_PROFILE, T_WIND_PRESSURE_PROFILE&> arPresProfile;
	CArray<T_WDPR_KBC_AUTOCALC, T_WDPR_KBC_AUTOCALC&> arKBC2009ForceCoef; //KBC2009 의 경우 높이별로 저장하기 위함.
// 	CArray<T_WDPR_KBC_AUTOCALC, T_WDPR_KBC_AUTOCALC&> arKBC2016ForceCoef;
// 	CArray<T_WDPR_KBC_AUTOCALC, T_WDPR_KBC_AUTOCALC&> arKDS2019ForceCoef;
	void Initialize()
	{
		KeyLdgr = 0;
		KeyStld = 0;
		nDirection = 1;
		nType = 1;
		dAngle = 0.0;
		dInnerPnt[0] = 0.0;
		dInnerPnt[1] = 0.0;
		dInnerPnt[2] = 0.0;
		dConcenPt[0] = 0.0;
		dConcenPt[1] = 0.0;
		dConcenPt[2] = 0.0;
		dWindArea = 0.0;
		dCenterPnt[0] = 0.0;
		dCenterPnt[1] = 0.0;
		dCenterPnt[2] = 0.0;
		dScaleFactor = 1.0;
		nCodeType = KDS_W_2022;
		KeyWvep = 0;
		KeyWdfc = 0;
		CodeParam.Initialize(nCodeType);
		nSelection = 1;
		//KeyLagr = 0;
		KeyLapl = 0;
		nodalStruct.Initialize();
		arKeysDefineElem.RemoveAll();
		arKeysDefineNode.RemoveAll();
		arPresProfile.RemoveAll();
		arKBC2009ForceCoef.RemoveAll();
	}
	T_WDPR_D()
	{
		Initialize();
	}
	T_WDPR_D(T_WDPR_D& src)
	{
		*this = src;
	}
	T_WDPR_D& operator=(const T_WDPR_D& src)
	{
		KeyLdgr=src.KeyLdgr;
		KeyStld=src.KeyStld;
		nDirection=src.nDirection;
		nType=src.nType;
		dAngle=src.dAngle;
		dInnerPnt[0]=src.dInnerPnt[0];
		dInnerPnt[1]=src.dInnerPnt[1];
		dInnerPnt[2]=src.dInnerPnt[2];
		dConcenPt[0] = src.dConcenPt[0];
		dConcenPt[1] = src.dConcenPt[1];
		dConcenPt[2] = src.dConcenPt[2];
		dWindArea = src.dWindArea;
		dCenterPnt[0] = src.dCenterPnt[0];
		dCenterPnt[1] = src.dCenterPnt[1];
		dCenterPnt[2] = src.dCenterPnt[2];
		dScaleFactor=src.dScaleFactor;
		nCodeType=src.nCodeType;
		KeyWvep=src.KeyWvep;
		KeyWdfc=src.KeyWdfc;
		CodeParam=src.CodeParam;
		nSelection=src.nSelection;
		//KeyLagr=src.KeyLagr;
		KeyLapl=src.KeyLapl;
		nodalStruct=src.nodalStruct;
		arKeysDefineElem.Copy(src.arKeysDefineElem);
		arKeysDefineNode.Copy(src.arKeysDefineNode);
		arPresProfile.Copy(src.arPresProfile);
		arKBC2009ForceCoef.Copy(src.arKBC2009ForceCoef);
// 		arKBC2016ForceCoef.Copy(src.arKBC2016ForceCoef);
// 		arKDS2019ForceCoef.Copy(src.arKDS2019ForceCoef);
		return *this;
	}
	T_WDPR_KBC2016* GetKBC2016()
	{
		switch (nCodeType)
		{
		case KBC_W_2016: return &CodeParam.KBC2016;
		case KDS_W_2019: return &CodeParam.KDS2019;
		default:
			return NULL;
			break;
		}
	}
	T_WDPR_ASCE7_16* GetASCE7Series()
	{
		switch (nCodeType)
		{
		case ASCE7_W_2016: return &CodeParam.ASCE7_16;
		case ASCE7_W_2022: return &CodeParam.ASCE7_22;
		case NSCP_W_2024:  return &CodeParam.NSCP2024;
		default: return NULL;
		}
	}
	const T_WDPR_ASCE7_16* GetASCE7Series() const
	{
		switch (nCodeType)
		{
		case ASCE7_W_2016: return &CodeParam.ASCE7_16;
		case ASCE7_W_2022: return &CodeParam.ASCE7_22;
		case NSCP_W_2024:  return &CodeParam.NSCP2024;
		default: return NULL;
		}
	}
	T_WDPR_IS875_2015* GetIS875_2015()
	{
		switch (nCodeType)
		{
		case IS_W_875_2015: return &CodeParam.IS875_2015;
		default: return NULL;
		}
	}
	const T_WDPR_IS875_2015* GetIS875_2015() const
	{
		switch (nCodeType)
		{
		case IS_W_875_2015: return &CodeParam.IS875_2015;
		default: return NULL;
		}
	}
};

struct T_WDPR_UDRD_D
{
	T_WDPR_K key;
	T_WDPR_D data;
};

struct T_WDPR_UDRD_D_RW_856
{
	T_WIND_K key;

	T_LDGR_K KeyLdgr; //Load Group
	T_STLD_K KeyStld; //Load Case Name
	int      nDirection;   //1=X-Y, 2=Normal
	int      nType;        //1:Area, 2:Beam, 3:Nodal, 4:Function
	double   dAngle;
	double   dInnerPnt[3];
	double   dConcenPt[3];   // 상부구조의 경우 상부구조의 집중하중이 들어갈 좌표.
	double   dWindArea;      // 상부구조의 경우 상부구조의 영풍면 넓이.
	double   dCenterPnt[3];
	double   dScaleFactor;
	int      nCodeType;
	T_WVEP_K KeyWvep;     //Wind Velocity Pressure
	T_WVEP_K KeyWdfc;     //Wind Pressure Function
	T_WDPR_KBC2009 KBC2009;
	T_WDPR_CH2012_925  CH2012;
	T_WDPR_CH2001  CH2001;
	int      nSelection;     //1=Group, 2=Element
	T_LAPL_K KeyLapl;       // 결국 DB에는 Lapl단위로 저장이 된다....
	T_WIND_NODAL_STRUCT nodalStruct;
	int arKeysDefineElem;
	int arKeysDefineNode;
	int arPresProfile;
	int arKBC2009ForceCoef; //KBC2009 저장용.
};

struct T_WDPR_UDRD_D_RW_895
{
	T_WIND_K key;

	T_LDGR_K KeyLdgr; //Load Group
	T_STLD_K KeyStld; //Load Case Name
	int      nDirection;   //1=X-Y, 2=Normal
	int      nType;        //1:Area, 2:Beam, 3:Nodal, 4:Function
	double   dAngle;
	double   dInnerPnt[3];
	double   dConcenPt[3];   // 상부구조의 경우 상부구조의 집중하중이 들어갈 좌표.
	double   dWindArea;      // 상부구조의 경우 상부구조의 영풍면 넓이.
	double   dCenterPnt[3];
	double   dScaleFactor;
	int      nCodeType;
	T_WVEP_K KeyWvep;     //Wind Velocity Pressure
	T_WVEP_K KeyWdfc;     //Wind Pressure Function
	T_WDPR_KBC2016 KBC2016;
	T_WDPR_KBC2009 KBC2009;
	T_WDPR_CH2012_925  CH2012;
	T_WDPR_CH2001  CH2001;
	int      nSelection;     //1=Group, 2=Element
	T_LAPL_K KeyLapl;       // 결국 DB에는 Lapl단위로 저장이 된다....
	T_WIND_NODAL_STRUCT nodalStruct;
	int arKeysDefineElem;
	int arKeysDefineNode;
	int arPresProfile;
	int arKBC2009ForceCoef; //KBC2009 저장용.
	int arKBC2016ForceCoef; //KBC2016 저장용.
};

struct T_WDPR_UDRD_D_RW_900
{
	T_WIND_K key;

	T_LDGR_K KeyLdgr; //Load Group
	T_STLD_K KeyStld; //Load Case Name
	int      nDirection;   //1=X-Y, 2=Normal
	int      nType;        //1:Area, 2:Beam, 3:Nodal, 4:Function
	double   dAngle;
	double   dInnerPnt[3];
	double   dConcenPt[3];   // 상부구조의 경우 상부구조의 집중하중이 들어갈 좌표.
	double   dWindArea;      // 상부구조의 경우 상부구조의 영풍면 넓이.
	double   dCenterPnt[3];
	double   dScaleFactor;
	int      nCodeType;
	T_WVEP_K KeyWvep;     //Wind Velocity Pressure
	T_WVEP_K KeyWdfc;     //Wind Pressure Function
	T_WDPR_KBC2016 KDS2019;
	T_WDPR_KBC2016 KBC2016;
	T_WDPR_KBC2009 KBC2009;
	T_WDPR_CH2012_925  CH2012;
	T_WDPR_CH2001  CH2001;
	int      nSelection;     //1=Group, 2=Element
	T_LAPL_K KeyLapl;       // 결국 DB에는 Lapl단위로 저장이 된다....
	T_WIND_NODAL_STRUCT nodalStruct;
	int arKeysDefineElem;
	int arKeysDefineNode;
	int arPresProfile;
	int arKBC2009ForceCoef; //KBC2009 저장용.
	int arKBC2016ForceCoef; //KBC2016 저장용.
	int arKDS2019ForceCoef;
};
struct T_WDPR_UDRD_D_RW
{
	T_WIND_K key;

	T_LDGR_K KeyLdgr; //Load Group
	T_STLD_K KeyStld; //Load Case Name
	int      nDirection;   //1=X-Y, 2=Normal
	int      nType;        //1:Area, 2:Beam, 3:Nodal, 4:Function
	double   dAngle;
	double   dInnerPnt[3];
	double   dConcenPt[3];   // 상부구조의 경우 상부구조의 집중하중이 들어갈 좌표.
	double   dWindArea;      // 상부구조의 경우 상부구조의 영풍면 넓이.
	double   dCenterPnt[3];
	double   dScaleFactor;
	int      nCodeType;
	T_WVEP_K KeyWvep;     //Wind Velocity Pressure
	T_WVEP_K KeyWdfc;     //Wind Pressure Function
	T_WDPR_CODE CodeParam;

	int      nSelection;     //1=Group, 2=Element
	T_LAPL_K KeyLapl;       // 결국 DB에는 Lapl단위로 저장이 된다....
	T_WIND_NODAL_STRUCT nodalStruct;
	int arKeysDefineElem;
	int arKeysDefineNode;
	int arPresProfile;
	int arKBC2009ForceCoef; //KBC2009 저장용.
// 	int arKBC2016ForceCoef; //KBC2016 저장용.
// 	int arKDS2019ForceCoef;

	void GetWdpr(T_WDPR_K& rKey, T_WDPR_D& rData)
	{
		rKey = key;
		rData.KeyLdgr = KeyLdgr;
		rData.KeyStld = KeyStld;
		rData.nDirection = nDirection;
		rData.nType = nType;
		rData.dAngle= dAngle;
		rData.dInnerPnt[0] = dInnerPnt[0];
		rData.dInnerPnt[1] = dInnerPnt[1];
		rData.dInnerPnt[2] = dInnerPnt[2];
		rData.dConcenPt[0] = dConcenPt[0];
		rData.dConcenPt[1] = dConcenPt[1];
		rData.dConcenPt[2] = dConcenPt[2];
		rData.dWindArea = dWindArea;
		rData.dCenterPnt[0] = dCenterPnt[0];
		rData.dCenterPnt[1] = dCenterPnt[1];
		rData.dCenterPnt[2] = dCenterPnt[2];
		rData.dScaleFactor = dScaleFactor;
		rData.nCodeType = nCodeType;   
		rData.KeyWvep = KeyWvep;   
		rData.KeyWdfc = KeyWdfc;
		rData.CodeParam = CodeParam;
		
// 		rData.KDS2019 = KDS2019;
// 		rData.KBC2016 = KBC2016;  
// 		rData.KBC2009 = KBC2009;
// 		rData.CH2012 = CH2012;
// 		rData.CH2001 = CH2001;
		rData.nSelection = nSelection;
		rData.KeyLapl = KeyLapl;
		rData.nodalStruct=nodalStruct;
	}
	void SetWdpr(const T_WDPR_K tKey, const T_WDPR_D& rData)
	{
		key = tKey;
		KeyLdgr = rData.KeyLdgr;
		KeyStld = rData.KeyStld;
		nDirection = rData.nDirection;
		nType = rData.nType;
		dAngle= rData.dAngle;
		dInnerPnt[0] = rData.dInnerPnt[0];
		dInnerPnt[1] = rData.dInnerPnt[1];
		dInnerPnt[2] = rData.dInnerPnt[2];
		dConcenPt[0] = rData.dConcenPt[0];
		dConcenPt[1] = rData.dConcenPt[1];
		dConcenPt[2] = rData.dConcenPt[2];
		dWindArea = rData.dWindArea;
		dCenterPnt[0] = rData.dCenterPnt[0];
		dCenterPnt[1] = rData.dCenterPnt[1];
		dCenterPnt[2] = rData.dCenterPnt[2];    
		dScaleFactor = rData.dScaleFactor;
		nCodeType = rData.nCodeType;   
		KeyWvep = rData.KeyWvep;   
		KeyWdfc = rData.KeyWdfc;
		CodeParam = rData.CodeParam;
// 		KDS2019 = rData.KDS2019;
// 		KBC2016 = rData.KBC2016;
// 		KBC2009 = rData.KBC2009;
// 		CH2012 = rData.CH2012;
// 		CH2001 = rData.CH2001;
		nSelection = rData.nSelection;
		KeyLapl = rData.KeyLapl;
		nodalStruct=rData.nodalStruct;
		arKeysDefineElem = (int)rData.arKeysDefineElem.GetSize();
		arKeysDefineNode = (int)rData.arKeysDefineNode.GetSize();
		arPresProfile = (int)rData.arPresProfile.GetSize();
		arKBC2009ForceCoef = (int)rData.arKBC2009ForceCoef.GetSize();
// 		arKBC2016ForceCoef = (int)rData.arKBC2016ForceCoef.GetSize();
// 		arKDS2019ForceCoef = (int)rData.arKDS2019ForceCoef.GetSize();
	}
	void Convert900(T_WDPR_UDRD_D_RW_900& rData)
	{
		key = rData.key;
		KeyLdgr = rData.KeyLdgr;
		KeyStld = rData.KeyStld;
		nDirection = rData.nDirection;
		nType = rData.nType;
		dAngle = rData.dAngle;
		dInnerPnt[0] = rData.dInnerPnt[0];
		dInnerPnt[1] = rData.dInnerPnt[1];
		dInnerPnt[2] = rData.dInnerPnt[2];
		dConcenPt[0] = rData.dConcenPt[0];
		dConcenPt[1] = rData.dConcenPt[1];
		dConcenPt[2] = rData.dConcenPt[2];
		dWindArea = rData.dWindArea;
		dCenterPnt[0] = rData.dCenterPnt[0];
		dCenterPnt[1] = rData.dCenterPnt[1];
		dCenterPnt[2] = rData.dCenterPnt[2];
		dScaleFactor = rData.dScaleFactor;
		nCodeType = rData.nCodeType;
		KeyWvep = rData.KeyWvep;
		KeyWdfc = rData.KeyWdfc;
		switch (nCodeType)
		{
		case CH_W_2002:				CodeParam.CH2001 = rData.CH2001;		break;
		case CH_W_2012:				CodeParam.CH2012.Convert925(rData.CH2012);		break;
		case KBC_W_2009:			CodeParam.KBC2009 = rData.KBC2009;	arKBC2009ForceCoef = rData.arKBC2009ForceCoef; break;
		case KBC_W_2016:			CodeParam.KBC2016 = rData.KBC2016;	arKBC2009ForceCoef = rData.arKBC2016ForceCoef; break;
		case KDS_W_2019:			CodeParam.KDS2019 = rData.KDS2019;	arKBC2009ForceCoef = rData.arKDS2019ForceCoef; break;

		default:
			break;
		}
		nSelection = rData.nSelection;
		KeyLapl = rData.KeyLapl;
		nodalStruct = rData.nodalStruct;
		arKeysDefineElem = rData.arKeysDefineElem;
		arKeysDefineNode = rData.arKeysDefineNode;
		arPresProfile = rData.arPresProfile;
	}

	void Convert895(T_WDPR_UDRD_D_RW_895& rData)
	{
		key = rData.key;
		KeyLdgr = rData.KeyLdgr;
		KeyStld = rData.KeyStld;
		nDirection = rData.nDirection;
		nType = rData.nType;
		dAngle= rData.dAngle;
		dInnerPnt[0] = rData.dInnerPnt[0];
		dInnerPnt[1] = rData.dInnerPnt[1];
		dInnerPnt[2] = rData.dInnerPnt[2];
		dConcenPt[0] = rData.dConcenPt[0];
		dConcenPt[1] = rData.dConcenPt[1];
		dConcenPt[2] = rData.dConcenPt[2];
		dWindArea = rData.dWindArea;
		dCenterPnt[0] = rData.dCenterPnt[0];
		dCenterPnt[1] = rData.dCenterPnt[1];
		dCenterPnt[2] = rData.dCenterPnt[2];    
		dScaleFactor = rData.dScaleFactor;
		nCodeType = rData.nCodeType;   
		KeyWvep = rData.KeyWvep;   
		KeyWdfc = rData.KeyWdfc;
		switch (nCodeType)
		{
		case CH_W_2002:				CodeParam.CH2001 = rData.CH2001;		break;
		case CH_W_2012:				CodeParam.CH2012.Convert925(rData.CH2012);		break;
		case KBC_W_2009:			CodeParam.KBC2009 = rData.KBC2009;	arKBC2009ForceCoef = rData.arKBC2009ForceCoef; break;
		case KBC_W_2016:			CodeParam.KBC2016 = rData.KBC2016;	arKBC2009ForceCoef = rData.arKBC2016ForceCoef; break;
		default:
			break;
		}
		nSelection = rData.nSelection;
		KeyLapl = rData.KeyLapl;
		nodalStruct=rData.nodalStruct;
		arKeysDefineElem = rData.arKeysDefineElem;
		arKeysDefineNode =rData.arKeysDefineNode;
		arPresProfile = rData.arPresProfile;
	}
	void Convert856(T_WDPR_UDRD_D_RW_856& rData)
	{
		key = rData.key;
		KeyLdgr = rData.KeyLdgr;
		KeyStld = rData.KeyStld;
		nDirection = rData.nDirection;
		nType = rData.nType;
		dAngle= rData.dAngle;
		dInnerPnt[0] = rData.dInnerPnt[0];
		dInnerPnt[1] = rData.dInnerPnt[1];
		dInnerPnt[2] = rData.dInnerPnt[2];
		dConcenPt[0] = rData.dConcenPt[0];
		dConcenPt[1] = rData.dConcenPt[1];
		dConcenPt[2] = rData.dConcenPt[2];
		dWindArea = rData.dWindArea;
		dCenterPnt[0] = rData.dCenterPnt[0];
		dCenterPnt[1] = rData.dCenterPnt[1];
		dCenterPnt[2] = rData.dCenterPnt[2];    
		dScaleFactor = rData.dScaleFactor;
		nCodeType = rData.nCodeType;   
		KeyWvep = rData.KeyWvep;   
		KeyWdfc = rData.KeyWdfc;
		switch (nCodeType)
		{
		case CH_W_2002:				CodeParam.CH2001 = rData.CH2001;			break;
		case CH_W_2012:				CodeParam.CH2012.Convert925(rData.CH2012);	break;
		case KBC_W_2009:			CodeParam.KBC2009 = rData.KBC2009;	arKBC2009ForceCoef = rData.arKBC2009ForceCoef; break;
		default:
			break;
		}
		nSelection = rData.nSelection;
		KeyLapl = rData.KeyLapl;
		nodalStruct=rData.nodalStruct;
		arKeysDefineElem = rData.arKeysDefineElem;
		arKeysDefineNode =rData.arKeysDefineNode;
		arPresProfile = rData.arPresProfile;

	}
};

struct T_WDPR_UNIT
{
	T_WDPR_UNIT()
	{
		dInnerPnt=D_UNITSYS_BASE_LENGTH;
		dCenterPnt=D_UNITSYS_BASE_LENGTH;
		dConcenPt=D_UNITSYS_BASE_LENGTH;
		dWindArea=D_UNITSYS_BASE_AREA;
		arPresProfile_dLevel=D_UNITSYS_BASE_LENGTH;
		arPresProfile_dWindWidth=D_UNITSYS_BASE_LENGTH;
		arPresProfile_dWindThick=D_UNITSYS_BASE_LENGTH;
		arPresProfile_dPressure=D_UNITSYS_BASE_PRESSURE;
		arPresProfile_dAdditionalPressure=D_UNITSYS_BASE_PRESSURE;
		CH2001_dCftWindPressure=D_UNITSYS_NONE;
		CH2012_dCftWindPressure=D_UNITSYS_NONE;
		KBC2009_dBreath = D_UNITSYS_BASE_LENGTH;
		KBC2009_COEFAUTOCALC_dDirVal = D_UNITSYS_BASE_LENGTH;
		KDS_Mass = D_UNITSYS_BASE_MASS;
	}

	int dInnerPnt;
	int dCenterPnt;
	int dConcenPt;
	int dWindArea;
	int arPresProfile_dLevel;     // 높이
	int arPresProfile_dWindWidth; // 바람의 방향의 너비
	int arPresProfile_dWindThick; // 바람의 방향의 너비
	int arPresProfile_dPressure;  // 규준에 따른 풍압.
	int arPresProfile_dAdditionalPressure; // 사용자 입력 추가 풍압.
	int CH2001_dCftWindPressure;  //Pressure KN/m^2 [단위계고정]
	int CH2012_dCftWindPressure;  //Pressure KN/m^2 [단위계고정]
	int KBC2009_dBreath;
	int KBC2009_COEFAUTOCALC_dDirVal;
	int KDS_Mass;
}; 

#define HASHSIZEWDPR 101

#pragma pack(pop)

#endif  // __DB_ST_DT_WDPR_H__