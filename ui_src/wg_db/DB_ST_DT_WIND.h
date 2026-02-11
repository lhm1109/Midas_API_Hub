#ifndef __DB_ST_DT_WIND_H__
#define __DB_ST_DT_WIND_H__

#define MIDAS_SDK

#pragma pack(push, 1)

// [WindCode] 추가시 수정

enum WIND_CODETYPE
{
    KS_W_92         = 1,
    JP_W_87         = 2,
    UBC_W_97        = 3,
    ANSI_W_82       = 4,
    KS_W_2000       = 5,
    IBC_W_2000      = 6,
    EURO_W_1992     = 7,
    BS6399_W_1997   = 8,
    CH_W_2002       = 9,
    JPN_W_2000      = 10,
    NBC_W_1995      = 11,
    IS_W_1987       = 12,
    TAIWAN_W_1986   = 13,
    JP_W_2004       = 14,
    EURO_W_2005     = 15,
    KBC_W_2009      = 16,
    IBC_W_2009      = 17,
    IBC_W_2012      = 18,
    CH_W_2012       = 19,
    NSR_W_2010      = 20,
    KBC_W_2016      = 21,
    IS_W_875_2015   = 22,
    CH_W_2021       = 23,
    KDS_W_2019      = 24,
    KDS_W_2022      = 25,
    DPT_W_2007      = 26,
	USER_W_TYPE		= 27,
	ASCE7_W_2016    = 28,
	ASCE7_W_2022    = 29,
    NSCP_W_2024     = 30,
    RUS_W_2016	    = 31,
    WIND_CODE_END = RUS_W_2016,
    WIND_CODE_NUM = WIND_CODE_END,
};

enum EN_WIND_ECCEN
{
	EN_ECCEN_POSI = 0,
	EN_ECCEN_NEGA,
	EN_ECCEN_NONE
};

#define T_WIND_K T_STLD_K
struct T_WIND_ADDITION_671
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

struct T_WIND_ADDITION_851
{
    T_STOR_K KeyStor;
    double AddX;
    double AddY;
    double AddR;
    void Initialize()
    {
        KeyStor=0;
        AddX=0.;
        AddY=0.;
        AddR=0.;
    }
};

struct T_WIND_ADDITION
{
    T_STOR_K KeyStor;
    double AddX;                    // Along X
    double AddY;                    // Along Y
    double AddX2;                   // Across X
    double AddY2;                   // Across Y
    double AddR;                    // Torsional R / Torsional Rz(x)
    double AddR2;                   // Torsional Rz(y)
    void Initialize()
    {
        KeyStor=0;
        AddX=0.;
        AddY=0.;
        AddX2=0.;
        AddY2=0.;
        AddR=0.;
        AddR2=0.;
    }
};

struct T_WIND_USER
{
	T_STOR_K KeyStor;
	double dElev;
	double dPressX;
	double dPressY;
	double dLoadH;
	double dLoadBX;
	double dLoadBY;
	void Initialize()
	{
		KeyStor = 0;	
		dElev = 0.0;
		dPressX = 0.0;
		dPressY = 0.0;
		dLoadH = 0.0;
		dLoadBX = 0.0;
		dLoadBY = 0.0;
	}
};

struct T_WIND_KS1992
{
    int nExposureCategory;  // 1=A 2=B 3=C
    double dBasicWindSpeed;  //  m/sec
    double dPressureCoefficient;
    void Initialize()
    {
        nExposureCategory=2;
        dBasicWindSpeed=30.;
        dPressureCoefficient=1.4;
    }
};

struct T_WIND_KS2000_570
{
    int nExposureCategory;  // 1=A 2=B 3=C 4=D
    double dBasicWindSpeed;  //  m/sec
    double dGustFactor;
    double dKzt;
    double dHzt;
    double dHRoof;
    double dImportanceFactor;
    void Initialize()
    {
        nExposureCategory=2;
        dBasicWindSpeed=30.;
        dGustFactor=1.;
        dKzt=1.;
        dHzt=0.;
        dHRoof=0.;
        dImportanceFactor=1.;
    }
};

struct T_WIND_KS2000_640
{
    int nExposureCategory;  // 1=A 2=B 3=C 4=D
    double dBasicWindSpeed;  //  m/sec
    double dGustFactor;
    double dKzt;
    double dHzt;
    double dHRoof;
    double dImportanceFactor;
    BOOL   bTopography;          // 5.8.0에서 추가
    BOOL   bUseForceCoef;        // 5.8.0에서 추가
    double dForceCoefficient;    // 5.8.0에서 추가
    int    nRigidity;            // 5.8.0에서 추가  0: Rigid Structure, 1: Flexible Structure
    void Initialize()
    {
        nExposureCategory=2;
        dBasicWindSpeed=30.;
        dGustFactor=2.2;
        dKzt=1.;
        dHzt=0.;
        dHRoof=0.;
        dImportanceFactor=1.;
        bTopography = FALSE;
        bUseForceCoef = FALSE;
        dForceCoefficient = 1.0;
        nRigidity = 0;
    }
};

struct T_WIND_KS2000
{
    int nExposureCategory;  // 1=A 2=B 3=C 4=D
    double dBasicWindSpeed;  //  m/sec
    double dGustFactor;
    double dKzt;
    double dHzt;
    double dHRoof;
    double dImportanceFactor;
    BOOL   bTopography;          // 5.8.0에서 추가
    BOOL   bUseForceCoef;        // 5.8.0에서 추가
    double dForceCoefficient;    // 5.8.0에서 추가
    int    nRigidity;            // 5.8.0에서 추가  0: Rigid Structure, 1: Flexible Structure
    double dGustFactorY;         // 6.5.0에서 추가
    void Initialize()
    {
        nExposureCategory=2;
        dBasicWindSpeed=30.;
        dGustFactor=2.2;
        dKzt=1.;
        dHzt=0.;
        dHRoof=0.;
        dImportanceFactor=1.;
        bTopography = FALSE;
        bUseForceCoef = FALSE;
        dForceCoefficient = 1.0;
        nRigidity = 0;
        dGustFactorY = 2.2;
    }
    void Convert570(T_WIND_KS2000_570& rData)
    {
        nExposureCategory = rData.nExposureCategory;
        dBasicWindSpeed = rData.dBasicWindSpeed;
        dGustFactor = rData.dGustFactor;
        dKzt = rData.dKzt;
        dHzt = rData.dHzt;
        dHRoof = rData.dHRoof;
        dImportanceFactor= rData.dImportanceFactor;
        if ( dKzt > 1.0 && dHzt > 0. ) bTopography = TRUE;  // 5.8.0이전의 호환을 위한 조건
        else bTopography = FALSE;
        bUseForceCoef = FALSE;
        dForceCoefficient = 1.0;
        if ( ( nExposureCategory == 1 && dGustFactor == 2.5 ) ||
            ( nExposureCategory == 2 && dGustFactor == 2.2 ) ||
            ( nExposureCategory == 3 && dGustFactor == 1.9 ) ||
            ( nExposureCategory == 4 && dGustFactor == 1.8 ) )
            nRigidity = 0;    // Rigid Structure
        else nRigidity = 1; // Flexible Structure
        dGustFactorY = rData.dGustFactor;
    }
    void Convert640(T_WIND_KS2000_640& rData)
    {
        nExposureCategory = rData.nExposureCategory;
        dBasicWindSpeed = rData.dBasicWindSpeed;
        dGustFactor = rData.dGustFactor;
        dKzt = rData.dKzt;
        dHzt = rData.dHzt;
        dHRoof = rData.dHRoof;
        dImportanceFactor= rData.dImportanceFactor;
        bTopography = rData.bTopography;
        bUseForceCoef = rData.bUseForceCoef;
        dForceCoefficient = rData.dForceCoefficient;
        nRigidity = rData.nRigidity;
        dGustFactorY = rData.dGustFactor;
    }
};

struct T_WIND_JP1987
{
    double dPressureReductionFactor;
    double dWindPressureCoefficient;
    void Initialize()
    {
        dPressureReductionFactor=0.;
        dWindPressureCoefficient=0.;
    }
};

struct T_WIND_UBC1997_570
{
    int nExposureCategory;   // 1=B  2=C  3=D
    double dBasicWindSpeed;  // mile/h
    double dImportanceFactor;
    double dPressureCoefficient;
    void Initialize()
    {
        nExposureCategory=1;
        dBasicWindSpeed=80.0;
        dImportanceFactor=1.0;
        dPressureCoefficient=1.4;
    }
};

struct T_WIND_UBC1997
{
    int nProcedure;          // 5.8.0에서 추가 1=Projected Area Method, 2=Normal Force Method
    int nExposureCategory;   // 1=B  2=C  3=D
    double dBasicWindSpeed;  // mile/h
    double dImportanceFactor;
    double dPressureCoefficient;
    double dHRoof;
    void Initialize()
    {
        nProcedure = 2;
        nExposureCategory=1;
        dBasicWindSpeed=80.0;
        dImportanceFactor=1.0;
        dPressureCoefficient=1.4;
        dHRoof = 0.;
    }
    void Convert570(T_WIND_UBC1997_570& rData)
    {
        nProcedure = 1;           // 기존의 하중산정은 Projected Area Method를 이용하고 있음
        nExposureCategory   = rData.nExposureCategory;
        dBasicWindSpeed     = rData.dBasicWindSpeed;
        dImportanceFactor   = rData.dImportanceFactor;
        dPressureCoefficient= rData.dPressureCoefficient;
        dHRoof = 0.;
    }
};

struct T_WIND_ANSI1982
{
    int nExposureCategory;   // 1=A  2=B  3=C  4=D
    double dBasicWindSpeed;  // mile/h
    double dImportanceFactor;
    double dWindwardCoefficient;
    double dLewardCoefficient;
    void Initialize()
    {
        nExposureCategory=1;
        dBasicWindSpeed=80.0;
        dImportanceFactor=1.0;
        dWindwardCoefficient=0.8;
        dLewardCoefficient=0.5;
    }
};

struct T_WIND_IBC2000_570
{
    int     nProcedure;         // 1=Simplified, 2=Analytical
    int     nExposureCategory;  // 1=A 2=B 3=C 4=D
    double  dBasicWindSpeed;    // mph
    double  dGustFactorX;
    double  dGustFactorY;
    double  dHRoof;
    double  dTopographicFactor;  // 580부터 쓰이지 않음(삭제)
    double  dImportanceFactor;
    double  dWindDirectionFactor;
    int     nRigidity; // 0 Rigid 1: Flexible
    int     nEnclosure;        // 580부터 쓰이지 않음(삭제)   0: Enclosed, 1: Partially Enclosed
    double  dReductionFactor;  // 580부터 쓰이지 않음(삭제)
    void Initialize()
    {
        nProcedure=2;
        nExposureCategory=1;
        dBasicWindSpeed=85.0;
        dGustFactorX=0.85;
        dGustFactorY=0.85;
        dHRoof=0.;
        dImportanceFactor=1.;
        dTopographicFactor=1.0;
        dWindDirectionFactor= 0.85;
        nRigidity = 0; // 0 Rigid 1: Flexible
        nEnclosure = 0; // 0: Enclosed, 1: Partially Enclosed
        dReductionFactor=0.0;
    }
};

struct T_WIND_IBC2000_790
{
    int     nProcedure;         // 1=Simplified, 2=Analytical
    int     nExposureCategory;  // 1=A 2=B 3=C 4=D
    double  dBasicWindSpeed;    // mph
    double  dGustFactorX;
    double  dGustFactorY;
    double  dHRoof;
    double  dImportanceFactor;
    double  dWindDirectionFactor;
    int     nRigidity; // 0 Rigid 1: Flexible
    BOOL    bUseForceCoef;      // 5.8.0에서 추가
    double  dForceCoefficient;  // 5.8.0에서 추가
    BOOL    bTopography;        // 5.8.0에서 추가  include topography effect
    int     nHillShape;         // 5.8.0에서 추가  0:2-D Ridge or Valley, 1:2-D Escarpment, 2:3-D Axisym Hill
    int     nBldgLocation;      // 5.8.0에서 추가  0:Upwind, 1:Downwind
    double  dHillHeight;        // 5.8.0에서 추가
    double  dHillLength;        // 5.8.0에서 추가
    double  dHillDistance;      // 5.8.0에서 추가  crest-building distance
    void Initialize()
    {
        nProcedure=2;
        nExposureCategory=2;
        dBasicWindSpeed=85.0;
        dGustFactorX=0.85;
        dGustFactorY=0.85;
        dHRoof=0.;
        dImportanceFactor=1.;
        dWindDirectionFactor= 0.85;
        nRigidity = 0; // 0 Rigid 1: Flexible
        bUseForceCoef = FALSE;
        dForceCoefficient = 1.0;
        bTopography   = FALSE;
        nHillShape    = 0;
        nBldgLocation = 0;
        dHillHeight   = 0.;
        dHillLength   = 0.;
        dHillDistance = 0.;
    }
};

struct T_WIND_IBC2000
{
    int     nProcedure;         // 1=Simplified, 2=Analytical
    int     nExposureCategory;  // 1=A 2=B 3=C 4=D
    double  dBasicWindSpeed;    // mph
    double  dGustFactorX;
    double  dGustFactorY;
    double  dHRoof;
    double  dImportanceFactor;
    double  dWindDirectionFactor;
    int     nRigidity; // 0 Rigid 1: Flexible
    BOOL    bUseForceCoef;      // 5.8.0에서 추가
    double  dForceCoefficient;  // 5.8.0에서 추가
    BOOL    bTopography;        // 5.8.0에서 추가  include topography effect
    int     nHillShape;         // 5.8.0에서 추가  0:2-D Ridge or Valley, 1:2-D Escarpment, 2:3-D Axisym Hill
    int     nBldgLocation;      // 5.8.0에서 추가  0:Upwind, 1:Downwind
    double  dHillHeight;        // 5.8.0에서 추가
    double  dHillLength;        // 5.8.0에서 추가
    double  dHillDistance;      // 5.8.0에서 추가  crest-building distance
    BOOL    bAutoCalc;          // 7.9.1에서 추가 // PMS:4223-HSSHIM-20110420
    T_KEY   keyWnat;            // 7.9.1에서 추가 // PMS:4223-HSSHIM-20110420   // Cf 자동 계산용 DB(WNAT) key
    void Initialize()
    {
        nProcedure=2;
        nExposureCategory=2;
        dBasicWindSpeed=85.0;
        dGustFactorX=0.85;
        dGustFactorY=0.85;
        dHRoof=0.;
        dImportanceFactor=1.;
        dWindDirectionFactor= 0.85;
        nRigidity = 0; // 0 Rigid 1: Flexible
        bUseForceCoef = FALSE;
        dForceCoefficient = 1.0;
        bTopography   = FALSE;
        nHillShape    = 0;
        nBldgLocation = 0;
        dHillHeight   = 0.;
        dHillLength   = 0.;
        dHillDistance = 0.;
        bAutoCalc = FALSE;
        keyWnat   = 0;
    }
    void Convert570(T_WIND_IBC2000_570& rData)
    {
        nProcedure = rData.nProcedure;
        nExposureCategory = rData.nExposureCategory;
        dBasicWindSpeed = rData.dBasicWindSpeed;
        dGustFactorX = rData.dGustFactorX;
        dGustFactorY = rData.dGustFactorY;
        dHRoof = rData.dHRoof;
        dImportanceFactor = rData.dImportanceFactor;
        dWindDirectionFactor = rData.dWindDirectionFactor;
        nRigidity = rData.nRigidity; // 0 Rigid 1: Flexible
        bUseForceCoef = FALSE;
        dForceCoefficient = 1.0;
        bTopography   = FALSE;
        nHillShape    = 0;
        nBldgLocation = 0;
        dHillHeight   = 0.;
        dHillLength   = 0.;
        dHillDistance = 0.;
        bAutoCalc = FALSE;
        keyWnat   = 0;
    }
    void Convert790(T_WIND_IBC2000_790& rData)
    {
        nProcedure           = rData.nProcedure;
        nExposureCategory    = rData.nExposureCategory;
        dBasicWindSpeed      = rData.dBasicWindSpeed;
        dGustFactorX         = rData.dGustFactorX;
        dGustFactorY         = rData.dGustFactorY;
        dHRoof               = rData.dHRoof;
        dImportanceFactor    = rData.dImportanceFactor;
        dWindDirectionFactor = rData.dWindDirectionFactor;
        nRigidity            = rData.nRigidity; // 0 Rigid 1: Flexible
        bUseForceCoef        = rData.bUseForceCoef;
        dForceCoefficient    = rData.dForceCoefficient;
        bTopography          = rData.bTopography;
        nHillShape           = rData.nHillShape;
        nBldgLocation        = rData.nBldgLocation;
        dHillHeight          = rData.dHillHeight;
        dHillLength          = rData.dHillLength;
        dHillDistance        = rData.dHillDistance;
        bAutoCalc = FALSE;
        keyWnat   = 0;
    }
};

struct T_WIND_IBC2012
{
    int     nProcedure;         // 1=Simplified, 2=Analytical
    double  dBasicWindSpeed;    // mph
    double  dImportanceFactor;  // IBC2009에서만 사용
    int     nExposureCategory;  // 2=B 3=C 4=D
    double  dHRoof;
    BOOL    bTopography[2];   // [0]=X, [1]=Y
    int     nHillShape[2];   // 0:2-D Ridge or Valley, 1:2-D Escarpment, 2:3-D Axisym Hill
    int     nBldgLocation[2];   // 0:Upwind, 1:Downwind
    double  dHillHeight[2];
    double  dHillLength[2];
    double  dHillDistance[2];   // crest-building distance
    int     nEccen_X;   // 0=Pos, 1=Neg, 2=None
    int     nEccen_Y;   // 0=Pos, 1=Neg, 2=None

    // nProcedure = 0 일경우 사용
    double  dCnetWindward;
    double  dCnetLeeward;

    // nProcedure = 1 일경우 사용
    double  dDirectionFactorX;
    double  dDirectionFactorY;
    int     nRigidity;          // 0 Rigid 1: Flexible
    double  dGustFactorX;
    double  dGustFactorY;
    BOOL    bUseForceCoef;
    double  dForceCoefficient;
    BOOL    bAutoCalc;          // 7.9.1에서 추가 // PMS:4223-HSSHIM-20110420
    T_KEY   keyWnat;            // 7.9.1에서 추가 // PMS:4223-HSSHIM-20110420   // Cf 자동 계산용 DB(WNAT) key

    void Initialize()
    {
        nProcedure        = 2;
        dBasicWindSpeed   = 85.;
        dImportanceFactor = 1.;  // IBC2009에서만 사용
        nExposureCategory = 3;
        dHRoof            = 0.;
        dCnetWindward     = 0.73;
        dCnetLeeward      = -0.51;
        dDirectionFactorX = 0.85;
        dDirectionFactorY = 0.85;
        nRigidity         = 0;             // 0 Rigid 1: Flexible
        dGustFactorX      = 0.85;
        dGustFactorY      = 0.85;
        bUseForceCoef     = FALSE;
        dForceCoefficient = 1.0;
        bAutoCalc         = FALSE;
        keyWnat           = 0;
        nEccen_X          = 2;
        nEccen_Y          = 2;
        for ( int i=0; i<2; i++ )
        {
            bTopography[i] = FALSE;
            nHillShape[i] = 0;
            nBldgLocation[i] = 0;
            dHillHeight[i] = 0.;
            dHillLength[i] = 0.;
            dHillDistance[i] = 0.;
        }
    }
};

struct T_WIND_NSR2010
{
    double  dBasicWindSpeed;    // km/h
    double  dImportanceFactor;  //
    int     nExposureCategory;  // 2=B 3=C 4=D
    double  dHRoof;
    BOOL    bTopography[2];   // [0]=X, [1]=Y
    int     nHillShape[2];   // 0:2-D Ridge or Valley, 1:2-D Escarpment, 2:3-D Axisym Hill
    int     nBldgLocation[2];   // 0:Upwind, 1:Downwind
    double  dHillHeight[2];
    double  dHillLength[2];
    double  dHillDistance[2];   // crest-building distance
    int     nEccen_X;   // 0=Pos, 1=Neg, 2=None
    int     nEccen_Y;   // 0=Pos, 1=Neg, 2=None

    double  dDirectionFactorX;
    double  dDirectionFactorY;
    int     nRigidity;          // 0 Rigid 1: Flexible
    double  dGustFactorX;
    double  dGustFactorY;
    BOOL    bUseForceCoef;
    double  dForceCoefficient;
    BOOL    bAutoCalc;          // 7.9.1에서 추가 // PMS:4223-HSSHIM-20110420
    T_KEY   keyWnat;            // 7.9.1에서 추가 // PMS:4223-HSSHIM-20110420   // Cf 자동 계산용 DB(WNAT) key

    void Initialize()
    {
        dBasicWindSpeed   = 60.;
        dImportanceFactor = 1.;
        nExposureCategory = 2;
        dHRoof            = 0.;
        dDirectionFactorX = 0.85;
        dDirectionFactorY = 0.85;
        nRigidity         = 0;             // 0 Rigid 1: Flexible
        dGustFactorX      = 0.85;
        dGustFactorY      = 0.85;
        bUseForceCoef     = FALSE;
        dForceCoefficient = 1.0;
        bAutoCalc         = FALSE;
        keyWnat           = 0;
        nEccen_X          = 2;
        nEccen_Y          = 2;
        for ( int i=0; i<2; i++ )
        {
            bTopography[i] = FALSE;
            nHillShape[i] = 0;
            nBldgLocation[i] = 0;
            dHillHeight[i] = 0.;
            dHillLength[i] = 0.;
            dHillDistance[i] = 0.;
        }
    }
};

struct T_WIND_EURO1992_580  // added by KYM
{
    int    nProcedure;        // 1=Simplified, 2=Detailed
    int    nRoughCategory;    // 1=1, 2=2, 3=3, 4=4
    double dRefWindSpeed; // m/sec  t
    double dSizeCoef;     // size coefficient
    double dForceCoef;    // force coefficient
    double dFrictionCoef; // friction coefficient
    double dTopographyCoef; // topography coefficient
    double dGustFactor;     // gust response factor
    void Initialize()
    {
        nProcedure = 1;
        nRoughCategory = 1;
        dRefWindSpeed = 24.5;
        dSizeCoef = 1;
        dForceCoef = 1;
        dFrictionCoef = 0.;
        dTopographyCoef = 1;
        dGustFactor = 1;
    }
};

struct T_WIND_EURO1992  // added by KYM
{
    int    nProcedure;       // 1=Simplified, 2=Detailed
    int    nRoughCategory;   // 1=1, 2=2, 3=3, 4=4
    double dRefWindSpeed;    // m/sec  t
    double dForceCoef;       // force coefficient
    double dFrictionCoef;    // friction coefficient
    double dTopographyCoef;  // topography coefficient
    double dGustFactor;      // gust response factor
    double dWindwardCoef;    // 5.9.0에서 추가, windward pressure coefficient
    double dLeewardCoef;     // 5.9.0에서 추가, leeward pressure coefficient
    BOOL   bTopography;      // 5.9.0에서 추가, Include topography effect
    BOOL   bUseForceCoef;    // 5.9.0에서 추가, load evaluation using force coefficient
    double dTopographyRange; // 5.9.0에서 추가, Vertical range for topography coefficient
    void Initialize()
    {
        nProcedure       = 1;
        nRoughCategory   = 1;
        dRefWindSpeed    = 24.5;
        dForceCoef       = 1;
        dFrictionCoef    = 0.;
        dTopographyCoef  = 1;
        dGustFactor      = 1;
        dWindwardCoef    = 0.8;
        dLeewardCoef     = -0.3;
        bTopography      = FALSE;
        bUseForceCoef    = FALSE;
        dTopographyRange = 0.;
    }
    void Convert580(T_WIND_EURO1992_580& rData)
    {
        nProcedure       = rData.nProcedure;
        nRoughCategory   = rData.nRoughCategory;
        dRefWindSpeed    = rData.dRefWindSpeed;
        dForceCoef       = rData.dForceCoef;
        dFrictionCoef    = rData.dFrictionCoef;
        dTopographyCoef  = rData.dTopographyCoef;
        dGustFactor      = rData.dGustFactor;
        dWindwardCoef    = 0.8;
        dLeewardCoef     = -0.3;
        bTopography      = FALSE;
        bUseForceCoef    = TRUE;
        dTopographyRange = 0.;
    }
};

struct T_WIND_BS6399  // added by KYM
{
    int    nProcedure;        // 1=Standard, 2=Directional
    int    nSiteCategory;     // 1 = country, 2 = town
    int    nBldgTypeFactor;   // 1 = 8, 2 = 4, 3 = 2, 4 = 1, 5 = 0.5
    double dBasicWindSpeed;   // Basic Wind Speed : Vb [m/sec]
    double dMeanRoofHeight;   // Mean Roof Height : Ho
    double dBldgSeparation;   // Building Separation : X
    double dFrictionCoef;     // Frictional Drag Coefficient : Cf
    double dSeaDistance;      // Closest Distance to Sea : SD
    double dTownDistance;     // Upwind Distance from Edge of Town to Site : TD
    double dAltitudeFactor;   // Altitude Factor : Sa
    double dDirectionalFactor;// Directional Factor : Sd
    double dSeasonalFactor;   // Seasonal Factor : Ss
    double dProbabilityFactor;// Probability Factor : Sp
    double dTopographicFactor;// Topographic Factor : Sh
    void Initialize()
    {
        nProcedure = 1;
        nSiteCategory = 1;
        nBldgTypeFactor = 4;
        dBasicWindSpeed = 24;
        dMeanRoofHeight = 0;
        dBldgSeparation = 0;
        dFrictionCoef = 0.01;
        dSeaDistance = 0;
        dTownDistance = 0;
        dAltitudeFactor = 1.0;
        dDirectionalFactor = 1.0;
        dSeasonalFactor = 1.0;
        dProbabilityFactor = 1.0;
        dTopographicFactor = 0;
    }
};

struct T_WIND_CH2002_590    // added by KYM
{
    int    nRoughCategory;    // 1=A, 2=B, 3=C
    int    nStructuralMat;    // 1 = Steel, 2 = Steel+Infilled Wall, 3=RC
    double dBasicWindPressure;// Basic Wind Pressure : Wo
    double dModifyFactor;     // Modification factor for basic wind pressure Wo
    double dWindwardCoef;     // Windward pressure coefficient
    double dLeewardCoef;      // Leeward pressure coefficient
    double dPeriod_X;         // Fundamental Period in x direction
    double dPeriod_Y;         // Fundamental Period in y direction
    void Initialize()
    {
        nRoughCategory = 1;
        nStructuralMat = 1;
        dBasicWindPressure = 0.3;
        dModifyFactor = 1.0;
        dWindwardCoef = 0.8;
        dLeewardCoef  = -0.5;
        dPeriod_X = 0;
        dPeriod_Y = 0;
    }
};

struct T_WIND_CH2002_811        // added by KYM
{
    int    nRoughCategory;    // 1=A, 2=B, 3=C
    int    nStructuralMat;    // 1 = Steel, 2 = Steel+Infilled Wall, 3=RC
    double dBasicWindPressure;// Basic Wind Pressure : Wo
    double dModifyFactor;     // Modification factor for basic wind pressure Wo
    double dWindwardCoef;     // Windward pressure coefficient
    double dLeewardCoef;      // Leeward pressure coefficient
    double dPeriod_X;         // Fundamental Period in x direction
    double dPeriod_Y;         // Fundamental Period in y direction
    double dDampingRatio;        // 590추가
    int    nPulseMagnifyMethod;  // 590추가 맥동증대계수 계산방법 0 : Automatic Calculation, 1: User Defined
    double dPulseMagnifyFactorX; // 590추가 X방향 맥동증대계수
    double dPulseMagnifyFactorY; // 590추가 Y방향 맥동증대계수
    void Initialize()
    {
        nRoughCategory       = 1;
        nStructuralMat       = 1;
        dBasicWindPressure   = 0.3;
        dModifyFactor        = 1.0;
        dWindwardCoef        = 0.8;
        dLeewardCoef         = -0.5;
        dPeriod_X            = 0;
        dPeriod_Y            = 0;
        dDampingRatio        = 0.01;
        nPulseMagnifyMethod  = 0;
        dPulseMagnifyFactorX = 1.;
        dPulseMagnifyFactorY = 1.;
    }
};

struct T_WIND_CH2002        // added by KYM
{
    int    nRoughCategory;    // 1=A, 2=B, 3=C
    int    nStructuralMat;    // 1 = Steel, 2 = Steel+Infilled Wall, 3=RC
    double dBasicWindPressure;// Basic Wind Pressure : Wo
    double dModifyFactor;     // Modification factor for basic wind pressure Wo
    double dWindwardCoef;     // Windward pressure coefficient
    double dLeewardCoef;      // Leeward pressure coefficient
    double dPeriod_X;         // Fundamental Period in x direction
    double dPeriod_Y;         // Fundamental Period in y direction
    double dDampingRatio;        // 590추가
    int    nPulseMagnifyMethod;  // 590추가 맥동증대계수 계산방법 0 : Automatic Calculation, 1: User Defined
    double dPulseMagnifyFactorX; // 590추가 X방향 맥동증대계수
    double dPulseMagnifyFactorY; // 590추가 Y방향 맥동증대계수
    BOOL   bComfort;            //811  maxiao@midasit.com
    double dCftWindPressure;    //811
    double dCftDampingRatio;    //811
    void Initialize()
    {
        nRoughCategory = 1;
        nStructuralMat = 1;
        dBasicWindPressure = 0.3;
        dModifyFactor = 1.0;
        dWindwardCoef = 0.8;
        dLeewardCoef  = -0.5;
        dPeriod_X = 0;
        dPeriod_Y = 0;
        dDampingRatio = 0.01;
        nPulseMagnifyMethod = 0;
        dPulseMagnifyFactorX = 1.;
        dPulseMagnifyFactorY = 1.;
        bComfort = TRUE;
        dCftWindPressure = 0.3;
        dCftDampingRatio = 0.05;
    }
    void Convert590(T_WIND_CH2002_590& rData)
    {
        nRoughCategory = rData.nRoughCategory;
        nStructuralMat = rData.nStructuralMat;
        dBasicWindPressure = rData.dBasicWindPressure;
        dModifyFactor = rData.dModifyFactor;
        dWindwardCoef = rData.dWindwardCoef;
        dLeewardCoef  = rData.dLeewardCoef;
        dPeriod_X = rData.dPeriod_X;
        dPeriod_Y = rData.dPeriod_Y;
        if ( rData.nStructuralMat == 1 ) dDampingRatio = 0.01;
        else if ( rData.nStructuralMat == 2 ) dDampingRatio = 0.02;
        else if ( rData.nStructuralMat == 3 ) dDampingRatio = 0.05;
        nPulseMagnifyMethod = 0;
        dPulseMagnifyFactorX = 1.;
        dPulseMagnifyFactorY = 1.;
        bComfort = TRUE;
        dCftWindPressure = 0.3;
        dCftDampingRatio = 0.05;
    }
    void Convert811(T_WIND_CH2002_811& rData)
    {
        nRoughCategory       = rData.nRoughCategory;
        nStructuralMat       = rData.nStructuralMat;
        dBasicWindPressure   = rData.dBasicWindPressure;
        dModifyFactor        = rData.dModifyFactor;
        dWindwardCoef        = rData.dWindwardCoef;
        dLeewardCoef         = rData.dLeewardCoef;
        dPeriod_X            = rData.dPeriod_X;
        dPeriod_Y            = rData.dPeriod_Y;
        dDampingRatio        = rData.dDampingRatio;
        nPulseMagnifyMethod  = rData.nPulseMagnifyMethod;
        dPulseMagnifyFactorX = rData.dPulseMagnifyFactorX;
        dPulseMagnifyFactorY = rData.dPulseMagnifyFactorY;
        bComfort             = TRUE;
        dCftWindPressure     = 0.3;
        dCftDampingRatio     = 0.05;
    }
};

struct T_WIND_CH2012_815        // added by maxiao
{
    int    nRoughCategory;    // 1=A, 2=B, 3=C
    double dBasicWindPressure;// Basic Wind Pressure : Wo
    double dModifyFactor;     // Modification factor for basic wind pressure Wo
    double dDampingRatio;     //Damping Ratio
    double dWindwardCoefX;     // Windward pressure coefficient-X
    double dLeewardCoefX;      // Leeward pressure coefficient-X
    double dWindwardCoefY;     // Windward pressure coefficient-Y
    double dLeewardCoefY;      // Leeward pressure coefficient-Y

    BOOL bPartitionCoef;       //Partition ressure coefficient
    BOOL bWindShield;          //WindShield coefficient

    int    nPulseMagnifyMethod;  // 590추가 맥동증대계수 계산방법 0 : Automatic Calculation, 1: User Defined
    double dPeriod_X;         // Fundamental Period in x direction
    double dPeriod_Y;         // Fundamental Period in y direction
    double dPulseMagnifyFactorX; // 590추가 X방향 맥동증대계수
    double dPulseMagnifyFactorY; // 590추가 Y방향 맥동증대계수

    BOOL bAlongVibration;       //Consider Along-wind vibration
    BOOL bCrossVibration;       //Consider cross-wind vibration
    BOOL bCross;
    int  nSectType;  //0 : rectangle 1: Round
    double dCrossPeriod_X;
    double dCrossPeriod_Y;
    BOOL bTorsion;
    double dTorsionPeriod;

    BOOL bWindComfort;          //Wind Comfort
    double dCheckWindPressure; // check Wind Pressure
    double dCheckDampingRatio; //check Damping Ratio

    void Initialize()
    {
        nRoughCategory       = 1;
        dBasicWindPressure   = 0.3;
        dModifyFactor        = 1.0;
        dDampingRatio        = 0.05;
        dWindwardCoefX       = 0.8;
        dWindwardCoefY       = 0.8;
        dLeewardCoefX        = -0.5;
        dLeewardCoefY        = -0.5;
        bPartitionCoef       = FALSE;
        bWindShield          = FALSE;
        nPulseMagnifyMethod  = 0;
        dPeriod_X            = 0;
        dPeriod_Y            = 0;
        dPulseMagnifyFactorX = 1;
        dPulseMagnifyFactorY = 1;
        bAlongVibration      = TRUE;
        bCrossVibration      = FALSE;
        bCross               = TRUE;
        nSectType            = 0;
        dCrossPeriod_X       = 0;
        dCrossPeriod_Y       = 0;
        bTorsion             = TRUE;
        dTorsionPeriod       = 0.5;
        bWindComfort         = TRUE;
        dCheckWindPressure   = 0.3;
        dCheckDampingRatio   = 0.05;
    }
};

struct T_WIND_CH2012_915       // added by tss
{
	int    nRoughCategory;    // 1=A, 2=B, 3=C
	double dBasicWindPressure;// Basic Wind Pressure : Wo
	double dModifyFactor;     // Modification factor for basic wind pressure Wo
	double dDampingRatio;     //Damping Ratio
	double dWindwardCoefX;     // Windward pressure coefficient-X
	double dLeewardCoefX;      // Leeward pressure coefficient-X
	double dWindwardCoefY;     // Windward pressure coefficient-Y
	double dLeewardCoefY;      // Leeward pressure coefficient-Y

	BOOL bPartitionCoef;       //Partition ressure coefficient
	BOOL bWindShield;          //WindShield coefficient

	int    nPulseMagnifyMethod;  // 590추가 맥동증대계수 계산방법 0 : Automatic Calculation, 1: User Defined
	double dPeriod_X;         // Fundamental Period in x direction
	double dPeriod_Y;         // Fundamental Period in y direction
	double dPulseMagnifyFactorX; // 590추가 X방향 맥동증대계수
	double dPulseMagnifyFactorY; // 590추가 Y방향 맥동증대계수

	BOOL bAlongVibration;       //Consider Along-wind vibration
	BOOL bCrossVibration;       //Consider cross-wind vibration
	BOOL bCross;
	int  nSectType;  //0 : rectangle 1: Round
	double dCrossPeriod_X;
	double dCrossPeriod_Y;
	BOOL bTorsion;
	double dTorsionPeriod;

	BOOL bWindComfort;          //Wind Comfort
	double dCheckWindPressure; // check Wind Pressure
	double dCheckDampingRatio; //check Damping Ratio
	int nStructMethod;//0-table(G.0.3)  1-Calculation(8.4.7)
	void Initialize()
	{
		nRoughCategory = 1;
		dBasicWindPressure = 0.3;
		dModifyFactor = 1.0;
		dDampingRatio = 0.05;
		dWindwardCoefX = 0.8;
		dWindwardCoefY = 0.8;
		dLeewardCoefX = -0.5;
		dLeewardCoefY = -0.5;
		bPartitionCoef = FALSE;
		bWindShield = FALSE;
		nPulseMagnifyMethod = 0;
		dPeriod_X = 0;
		dPeriod_Y = 0;
		dPulseMagnifyFactorX = 1;
		dPulseMagnifyFactorY = 1;
		bAlongVibration = TRUE;
		bCrossVibration = FALSE;
		bCross = TRUE;
		nSectType = 0;
		dCrossPeriod_X = 0.0;   // MNET:XXXX-HSSHIM-20130909 // default 값 변경 요청 (from 후효무)
		dCrossPeriod_Y = 0.0;   // MNET:XXXX-HSSHIM-20130909 // default 값 변경 요청 (from 후효무)
		bTorsion = TRUE;
		dTorsionPeriod = 0.5;
		bWindComfort = TRUE;
		dCheckWindPressure = 0.3;
		dCheckDampingRatio = 0.05;
		nStructMethod = 0;
	}
};

struct T_WIND_CH2012        // added by Maxiao(maxiao@midasit.com)
{
    int    nRoughCategory;    // 1=A, 2=B, 3=C
    double dBasicWindPressure;// Basic Wind Pressure : Wo
    double dModifyFactor;     // Modification factor for basic wind pressure Wo
    double dDampingRatio;     //Damping Ratio
    double dWindwardCoefX;     // Windward pressure coefficient-X
    double dLeewardCoefX;      // Leeward pressure coefficient-X
    double dWindwardCoefY;     // Windward pressure coefficient-Y
    double dLeewardCoefY;      // Leeward pressure coefficient-Y

    BOOL bPartitionCoef;       //Partition ressure coefficient
    BOOL bWindShield;          //WindShield coefficient

    int    nPulseMagnifyMethod;  // 590추가 맥동증대계수 계산방법 0 : Automatic Calculation, 1: User Defined
    double dPeriod_X;         // Fundamental Period in x direction
    double dPeriod_Y;         // Fundamental Period in y direction
    double dPulseMagnifyFactorX; // 590추가 X방향 맥동증대계수
    double dPulseMagnifyFactorY; // 590추가 Y방향 맥동증대계수

    BOOL bAlongVibration;       //Consider Along-wind vibration
    BOOL bCrossVibration;       //Consider cross-wind vibration
    BOOL bCross;
    int  nSectType;  //0 : rectangle 1: Round
    double dCrossPeriod_X;
    double dCrossPeriod_Y;
    BOOL bTorsion;
    double dTorsionPeriod;

    BOOL bWindComfort;          //Wind Comfort
    double dCheckWindPressure; // check Wind Pressure
    double dCheckDampingRatio; //check Damping Ratio
    int nStructMethod;//0-table(G.0.3)  1-Calculation(8.4.7)
	int  nAlongType;
    void Initialize()
    {
        nRoughCategory       = 1;
        dBasicWindPressure   = 0.3;
        dModifyFactor        = 1.0;
        dDampingRatio        = 0.05;
        dWindwardCoefX       = 0.8;
        dWindwardCoefY       = 0.8;
        dLeewardCoefX        = -0.5;
        dLeewardCoefY        = -0.5;
        bPartitionCoef       = FALSE;
        bWindShield          = FALSE;
        nPulseMagnifyMethod  = 0;
        dPeriod_X            = 0;
        dPeriod_Y            = 0;
        dPulseMagnifyFactorX = 1;
        dPulseMagnifyFactorY = 1;
        bAlongVibration      = TRUE;
        bCrossVibration      = FALSE;
        bCross               = TRUE;
        nSectType            = 0;
        dCrossPeriod_X       = 0.0;   // MNET:XXXX-HSSHIM-20130909 // default 값 변경 요청 (from 후효무)
        dCrossPeriod_Y       = 0.0;   // MNET:XXXX-HSSHIM-20130909 // default 값 변경 요청 (from 후효무)
        bTorsion             = TRUE;
        dTorsionPeriod       = 0.5;
        bWindComfort         = TRUE;
        dCheckWindPressure   = 0.3;
        dCheckDampingRatio   = 0.05;
        nStructMethod        = 0;
		nAlongType			=	 0;
    }
    void Convert815(T_WIND_CH2012_815& rData)
    {
        nRoughCategory       = rData.nRoughCategory;
        dBasicWindPressure   = rData.dBasicWindPressure;
        dModifyFactor        = rData.dModifyFactor;
        dDampingRatio        = rData.dDampingRatio;
        dWindwardCoefX       = rData.dWindwardCoefX;
        dWindwardCoefY       = rData.dWindwardCoefY;
        dLeewardCoefX        = rData.dLeewardCoefX;
        dLeewardCoefY        = rData.dLeewardCoefY;
        bPartitionCoef       = rData.bPartitionCoef;
        bWindShield          = rData.bWindShield;
        nPulseMagnifyMethod  = rData.nPulseMagnifyMethod;
        dPeriod_X            = rData.dPeriod_X;
        dPeriod_Y            = rData.dPeriod_Y;
        dPulseMagnifyFactorX = rData.dPulseMagnifyFactorX;
        dPulseMagnifyFactorY = rData.dPulseMagnifyFactorY;
        bAlongVibration      = rData.bAlongVibration;
        bCrossVibration      = rData.bCrossVibration;
        bCross               = rData.bCross;
        nSectType            = rData.nSectType;
        dCrossPeriod_X       = rData.dCrossPeriod_X;
        dCrossPeriod_Y       = rData.dCrossPeriod_Y;
        bTorsion             = rData.bTorsion;
        dTorsionPeriod       = rData.dTorsionPeriod;
        bWindComfort         = rData.bWindComfort;
        dCheckWindPressure   = rData.dCheckWindPressure;
        dCheckDampingRatio   = rData.dCheckDampingRatio;
        nStructMethod        = 0;
    }
	void Convert915(const T_WIND_CH2012_915& rData)
	{
		nRoughCategory = rData.nRoughCategory;
		dBasicWindPressure = rData.dBasicWindPressure;
		dModifyFactor = rData.dModifyFactor;
		dDampingRatio = rData.dDampingRatio;
		dWindwardCoefX = rData.dWindwardCoefX;
		dWindwardCoefY = rData.dWindwardCoefY;
		dLeewardCoefX = rData.dLeewardCoefX;
		dLeewardCoefY = rData.dLeewardCoefY;
		bPartitionCoef = rData.bPartitionCoef;
		bWindShield = rData.bWindShield;
		nPulseMagnifyMethod = rData.nPulseMagnifyMethod;
		dPeriod_X = rData.dPeriod_X;
		dPeriod_Y = rData.dPeriod_Y;
		dPulseMagnifyFactorX = rData.dPulseMagnifyFactorX;
		dPulseMagnifyFactorY = rData.dPulseMagnifyFactorY;
		bAlongVibration = rData.bAlongVibration;
		bCrossVibration = rData.bCrossVibration;
		bCross = rData.bCross;
		nSectType = rData.nSectType;
		dCrossPeriod_X = rData.dCrossPeriod_X;
		dCrossPeriod_Y = rData.dCrossPeriod_Y;
		bTorsion = rData.bTorsion;
		dTorsionPeriod = rData.dTorsionPeriod;
		bWindComfort = rData.bWindComfort;
		dCheckWindPressure = rData.dCheckWindPressure;
		dCheckDampingRatio = rData.dCheckDampingRatio;
		nStructMethod = rData.nStructMethod;
		nAlongType = 0;
	}
};

struct T_WIND_CH2019_915
{
	int    nRoughCategory;    // 1=A, 2=B, 3=C
	double dBasicWindPressure;// Basic Wind Pressure : Wo
	double dModifyFactor;     // Modification factor for basic wind pressure Wo
	double dWindDirectionCoe; // Wind direction influence coefficient
	double dDampingRatio;     //Damping Ratio
	double dWindwardCoefX;     // Windward pressure coefficient-X
	double dLeewardCoefX;      // Leeward pressure coefficient-X
	double dWindwardCoefY;     // Windward pressure coefficient-Y
	double dLeewardCoefY;      // Leeward pressure coefficient-Y 

	BOOL bPartitionCoef;       //Partition ressure coefficient
	BOOL bWindShield;          //WindShield coefficient

	int    nPulseMagnifyMethod;  // 590추가 맥동증대계수 계산방법 0 : Automatic Calculation, 1: User Defined
	double dPeriod_X;         // Fundamental Period in x direction
	double dPeriod_Y;         // Fundamental Period in y direction
	double dPulseMagnifyFactorX; // 590추가 X방향 맥동증대계수
	double dPulseMagnifyFactorY; // 590추가 Y방향 맥동증대계수

	BOOL bAlongVibration;       //Consider Along-wind vibration
	BOOL bCrossVibration;       //Consider cross-wind vibration
	BOOL bCross;
	int  nSectType;  //0 : rectangle 1: Round
	double dCrossPeriod_X;
	double dCrossPeriod_Y;
	BOOL bTorsion;
	double dTorsionPeriod;

	BOOL bWindComfort;          //Wind Comfort
	double dCheckWindPressure; // check Wind Pressure
	double dCheckDampingRatio; //check Damping Ratio
	int nStructMethod;//0-table(G.0.3)  1-Calculation(8.4.7)
	void Initialize()
	{
		nRoughCategory = 1;
		dBasicWindPressure = 0.3;
		dModifyFactor = 1.0;
		dWindDirectionCoe = 1.0;
		dDampingRatio = 0.05;
		dWindwardCoefX = 0.8;
		dWindwardCoefY = 0.8;
		dLeewardCoefX = -0.5;
		dLeewardCoefY = -0.5;
		bPartitionCoef = FALSE;
		bWindShield = FALSE;
		nPulseMagnifyMethod = 0;
		dPeriod_X = 0;
		dPeriod_Y = 0;
		dPulseMagnifyFactorX = 1;
		dPulseMagnifyFactorY = 1;
		bAlongVibration = TRUE;
		bCrossVibration = FALSE;
		bCross = TRUE;
		nSectType = 0;
		dCrossPeriod_X = 0.0;   // MNET:XXXX-HSSHIM-20130909 // default 값 변경 요청 (from 후효무)
		dCrossPeriod_Y = 0.0;   // MNET:XXXX-HSSHIM-20130909 // default 값 변경 요청 (from 후효무)
		bTorsion = TRUE;
		dTorsionPeriod = 0.5;
		bWindComfort = TRUE;
		dCheckWindPressure = 0.3;
		dCheckDampingRatio = 0.05;
		nStructMethod = 0;
	}
};

struct T_WIND_CH2019
{
	int    nRoughCategory;    // 1=A, 2=B, 3=C
	double dBasicWindPressure;// Basic Wind Pressure : Wo
	double dModifyFactor;     // Modification factor for basic wind pressure Wo
	double dWindDirectionCoe; // Wind direction influence coefficient
	double dDampingRatio;     //Damping Ratio
	double dWindwardCoefX;     // Windward pressure coefficient-X
	double dLeewardCoefX;      // Leeward pressure coefficient-X
	double dWindwardCoefY;     // Windward pressure coefficient-Y
	double dLeewardCoefY;      // Leeward pressure coefficient-Y

	BOOL bPartitionCoef;       //Partition ressure coefficient
	BOOL bWindShield;          //WindShield coefficient

	int    nPulseMagnifyMethod;  // 590추가 맥동증대계수 계산방법 0 : Automatic Calculation, 1: User Defined
	double dPeriod_X;         // Fundamental Period in x direction
	double dPeriod_Y;         // Fundamental Period in y direction
	double dPulseMagnifyFactorX; // 590추가 X방향 맥동증대계수
	double dPulseMagnifyFactorY; // 590추가 Y방향 맥동증대계수

	BOOL bAlongVibration;       //Consider Along-wind vibration
	int nAlongType;
	BOOL bCrossVibration;       //Consider cross-wind vibration
	BOOL bCross;
	int  nSectType;  //0 : rectangle 1: Round
	double dCrossPeriod_X;
	double dCrossPeriod_Y;
	BOOL bTorsion;
	double dTorsionPeriod;

	BOOL bWindComfort;          //Wind Comfort
	double dCheckWindPressure; // check Wind Pressure
	double dCheckDampingRatio; //check Damping Ratio
	int nStructMethod;//0-table(G.0.3)  1-Calculation(8.4.7)
	void Initialize()
	{
		nRoughCategory = 1;
		dBasicWindPressure = 0.3;
		dModifyFactor = 1.0;
		dWindDirectionCoe = 1.0;
		dDampingRatio = 0.05;
		dWindwardCoefX = 0.8;
		dWindwardCoefY = 0.8;
		dLeewardCoefX = -0.5;
		dLeewardCoefY = -0.5;
		bPartitionCoef = FALSE;
		bWindShield = FALSE;
		nPulseMagnifyMethod = 0;
		dPeriod_X = 0;
		dPeriod_Y = 0;
		dPulseMagnifyFactorX = 1;
		dPulseMagnifyFactorY = 1;
		bAlongVibration = TRUE;
		nAlongType = 0;
		bCrossVibration = FALSE;
		bCross = TRUE;
		nSectType = 0;
		dCrossPeriod_X = 0.0;   // MNET:XXXX-HSSHIM-20130909 // default 값 변경 요청 (from 후효무)
		dCrossPeriod_Y = 0.0;   // MNET:XXXX-HSSHIM-20130909 // default 값 변경 요청 (from 후효무)
		bTorsion = TRUE;
		dTorsionPeriod = 0.5;
		bWindComfort = TRUE;
		dCheckWindPressure = 0.3;
		dCheckDampingRatio = 0.05;
		nStructMethod = 0;
	}
	void Convert915(const T_WIND_CH2019_915& rData)
	{
		nRoughCategory = rData.nRoughCategory;
		dBasicWindPressure = rData.dBasicWindPressure;
		dModifyFactor = rData.dModifyFactor;
		dWindDirectionCoe = rData.dWindDirectionCoe;
		dDampingRatio = rData.dDampingRatio;
		dWindwardCoefX = rData.dWindwardCoefX;
		dWindwardCoefY = rData.dWindwardCoefY;
		dLeewardCoefX = rData.dLeewardCoefX;
		dLeewardCoefY = rData.dLeewardCoefY;
		bPartitionCoef = rData.bPartitionCoef;
		bWindShield = rData.bWindShield;
		nPulseMagnifyMethod = rData.nPulseMagnifyMethod;
		dPeriod_X = rData.dPeriod_X;
		dPeriod_Y = rData.dPeriod_Y;
		dPulseMagnifyFactorX = rData.dPulseMagnifyFactorX;
		dPulseMagnifyFactorY = rData.dPulseMagnifyFactorY;
		bAlongVibration = rData.bAlongVibration;
		bCrossVibration = rData.bCrossVibration;
		bCross = rData.bCross;
		nSectType = rData.nSectType;
		dCrossPeriod_X = rData.dCrossPeriod_X;
		dCrossPeriod_Y = rData.dCrossPeriod_Y;
		bTorsion = rData.bTorsion;
		dTorsionPeriod = rData.dTorsionPeriod;
		bWindComfort = rData.bWindComfort;
		dCheckWindPressure = rData.dCheckWindPressure;
		dCheckDampingRatio = rData.dCheckDampingRatio;
		nStructMethod = rData.nStructMethod;
		nAlongType = 0;
	}
};

struct T_WIND_JPN2000      // added by KYM
{
    int nExposureCategory;   // 1, 2, 3, 4
    double dBasicWindSpeed;  // m/sec
    void Initialize()
    {
        nExposureCategory=3;
        dBasicWindSpeed=34.0;
    }
};

struct T_WIND_NBC1995_570
{
    int     nProcedure;         // 1 = Simple, 2 = Detailed
    double  dWindSpeed;         // reference wind speed
    double  dBuildingHeight;    // building height
    int     nExposureCategory;  // 0=A, 1=B, 2=C
    double  dGustFactor;
    double  dGustFactorX;
    double  dGustFactorY;
    BOOL    bTopography;        // include topography effect
    int     nHillShape;         // 0:2-D Ridge, 1:2-D Escarpment, 2:3-D Axisym Hill
    int     nBldgLocation;      // 0:Upwind, 1:Downwind
    double  dHillHeight;
    double  dHillLength;
    double  dHillDistance;      // crest-building distance
    void Initialize()
    {
        nProcedure = 1;
        dWindSpeed = 30.;
        dBuildingHeight = 0.;
        nExposureCategory = 0;
        dGustFactor   = 2.0;
        dGustFactorX  = 2.0;
        dGustFactorY  = 2.0;
        bTopography   = FALSE;
        nHillShape    = 0;
        nBldgLocation = 0;
        dHillHeight   = 0.;
        dHillLength   = 0.;
        dHillDistance = 0.;
    }
};

struct T_WIND_NBC1995
{
    int     nProcedure;         // 1 = Simple, 2 = Detailed
    double  dWindSpeed;         // reference wind speed
    double  dBuildingHeight;    // building height
    int     nExposureCategory;  // 0=A, 1=B, 2=C
    double  dGustFactor;
    double  dGustFactorX;
    double  dGustFactorY;
    BOOL    bTopography;        // include topography effect
    int     nHillShape;         // 0:2-D Ridge, 1:2-D Escarpment, 2:3-D Axisym Hill
    int     nBldgLocation;      // 0:Upwind, 1:Downwind
    double  dHillHeight;
    double  dHillLength;
    double  dHillDistance;      // crest-building distance
    BOOL    bUseForceCoef;      // 5.8.0에서 추가
    double  dForceCoefficient;  // 5.8.0에서 추가
    void Initialize()
    {
        nProcedure = 1;
        dWindSpeed = 30.;
        dBuildingHeight = 0.;
        nExposureCategory = 0;
        dGustFactor   = 2.0;
        dGustFactorX  = 2.0;
        dGustFactorY  = 2.0;
        bTopography   = FALSE;
        nHillShape    = 0;
        nBldgLocation = 0;
        dHillHeight   = 0.;
        dHillLength   = 0.;
        dHillDistance = 0.;
        bUseForceCoef = FALSE;
        dForceCoefficient = 1.0;
    }
    void Convert570(T_WIND_NBC1995_570& rData)
    {
        nProcedure = rData.nProcedure;
        dWindSpeed = rData.dWindSpeed;
        dBuildingHeight = rData.dBuildingHeight;
        nExposureCategory = rData.nExposureCategory;
        dGustFactor   = rData.dGustFactor;
        dGustFactorX  = rData.dGustFactorX;
        dGustFactorY  = rData.dGustFactorY;
        bTopography   = rData.bTopography;
        nHillShape    = rData.nHillShape;
        nBldgLocation = rData.nBldgLocation;
        dHillHeight   = rData.dHillHeight;
        dHillLength   = rData.dHillLength;
        dHillDistance = rData.dHillDistance;
        bUseForceCoef = FALSE;
        dForceCoefficient = 1.0;
    }
};

struct T_WIND_IS1987  // added by KYM
{
    int    nProcedure;         // 1=Standard, 2=Gust Effect
    int    nTerrainCategory;   // 0=I, 1=II, 2=III, 3=IV
    int    nBuildingClass;     // 0=A, 1=B,  2=C
    int    nAutoCalcRiskCoef;  // 0=Auto Calc, 1=User Defined
    int    nStructureClass;    // 0=General Bldg, 1=Temporary Shed, 2=Low hazard to life, 3=Important Bldg
    double dRiskCoef;
    double dBasicWindSpeed;    // Basic Wind Speed : Vb [m/sec], (33, 39, 44, 47, 50)
    BOOL   bUseForceCoef;      //
    double dForceCoef;         //
    double dFrictionCoef;      // Frictional Drag Coefficient : Cf
    BOOL   bTopography;        // Include topography effect
    double dTopographicFactor; // Topographic Factor k2(1~1.36)
    double dTopographyRange;   // Vertical range for topography factor
    double dGustFactor;
    void Initialize()
    {
        nProcedure        = 1;
        nTerrainCategory  = 0;
        nBuildingClass    = 0;
        nAutoCalcRiskCoef = 0;
        nStructureClass   = 0;
        dRiskCoef         = 1.0;
        dBasicWindSpeed   = 33.0;
        bUseForceCoef     = FALSE;
        dForceCoef        = 1.0;
        dFrictionCoef     = 0.01;
        bTopography       = FALSE;
        dTopographicFactor= 1.0;
        dTopographyRange  = 0.0;
        dGustFactor       = 1.0;
    }
};

struct T_WIND_IS875_2015_GUST_940  // added by Pinakin for IS 875 2015 Gust Factor Method
{
	int    nProcedure;         // 1=Gust Effect Calculated, 2=User Defined Gust Factor and FOrce Coefficient
	int    nStrKind;   // 0=Welded Steel, 1=Bolted Steel, 2=Reinforced Concrete, 3=Prestressed Concrete , 4 = User Defined
	double dBeta;
	double dAlongWindPeriod; // Along wind Period sec
	double dAAcrossWindPeriod; // Across wind Period sec

	double dModeShapeFact;      //Editable 0.5,1,1.5,2.3 or user defined
	double dCfs;    // Editable
	int    nMc;   // 0-Maximum 1=Minimum 2=AVerage
	void Initialize()
	{
		nProcedure = 0;
		nStrKind = 0;   // 0=We
		dBeta = 0.01;
		dAlongWindPeriod = 0.;
		dAAcrossWindPeriod = 0.;

		dModeShapeFact = 1.;
		dCfs = 0.01;    // Editabl
		nMc = 1;   // 0-Maximum
	}
};

struct T_WIND_IS875_2015_GUST  // added by Pinakin for IS 875 2015 Gust Factor Method
{
    int    nProcedure;         // 1=Gust Effect Calculated, 2=User Defined Gust Factor and FOrce Coefficient
    int    nStrKind;   // 0=Welded Steel, 1=Bolted Steel, 2=Reinforced Concrete, 3=Prestressed Concrete , 4 = User Defined
    double dBeta;
    double dAlongWindPeriod; // Along wind Period sec
    double dAAcrossWindPeriod; // Across wind Period sec

    double dModeShapeFact;      //Editable 0.5,1,1.5,2.3 or user defined
    double dCfs;    // Editable
    int    nMc;   // 0-Maximum 1=Minimum 2=AVerage
	int m_ifact;
    void Initialize()
    {
        nProcedure = 0;
        nStrKind  = 0;   // 0=We
        dBeta = 0.01;
        dAlongWindPeriod = 0.;
        dAAcrossWindPeriod=0.;

        dModeShapeFact = 1.;
        dCfs = 0.01;    // Editabl
        nMc = 1;   // 0-Maximum
        m_ifact = 0;
    }
    void Convert940(T_WIND_IS875_2015_GUST_940& rData)
    {
		nProcedure = rData.nProcedure;
		nStrKind  = rData.nStrKind;
		dBeta = rData.dBeta;
		dAlongWindPeriod = rData.dAlongWindPeriod;
		dAAcrossWindPeriod = rData.dAAcrossWindPeriod;

		dModeShapeFact = rData.dModeShapeFact;
		dCfs = rData.dCfs;
		nMc = rData.nMc;
        m_ifact = 0;
	}
};

struct T_WIND_IS875_2015_890  // added by Pinakin
{
    int    nProcedure;         // 1=Standard, 2=Gust Effect
    int    nTerrainCategory;   // 0=I, 1=II, 2=III, 3=IV
    int    nBuildingClass;     // 0=A, 1=B,  2=C
    int    nAutoCalcRiskCoef;  // 0=Auto Calc, 1=User Defined
    int    nStructureClass;    // 0=General Bldg, 1=Temporary Shed, 2=Low hazard to life, 3=Important Bldg
    double dRiskCoef;
    double dBasicWindSpeed;    // Basic Wind Speed : Vb [m/sec], (33, 39, 44, 47, 50)
    BOOL   bUseForceCoef;      //
    double dForceCoef;         //
    double dFrictionCoef;      // Frictional Drag Coefficient : Cf
    BOOL   bTopography;        // Include topography effect
    double dTopographicFactor; // Topographic Factor k2(1~1.36)
    double dTopographyRange;   // Vertical range for topography factor
    double dGustFactor;
    BOOL    bInclCyclonicRegEffects;
    double dk4Fact;     //k4 Factor Post Cyclonic - 1.3,Industrial - 1.15,Others 1
    double dkdfact;     //kd factor = 0.9 others, 1.0 Circular
    double dkcfact;     //kc factor = 1.0
    void Initialize()
    {
        nProcedure        = 1;
        nTerrainCategory  = 0;
        nBuildingClass    = 0;
        nAutoCalcRiskCoef = 0;
        nStructureClass   = 0;
        dRiskCoef         = 1.0;
        dBasicWindSpeed   = 33.0;
        bUseForceCoef     = FALSE;
        dForceCoef        = 1.0;
        dFrictionCoef     = 0.01;
        bTopography       = FALSE;
        dTopographicFactor= 1.0;
        dTopographyRange  = 0.0;
        dGustFactor       = 1.0;
        bInclCyclonicRegEffects = FALSE;
        dk4Fact           = 1.0;
        dkdfact           = 0.9;
        dkcfact           = 1.0;
    }
};

struct T_WIND_IS875_2015_940  // added by Pinakin
{
    int    nProcedure;         // 1=Standard, 2=Gust Effect
    int    nTerrainCategory;   // 0=I, 1=II, 2=III, 3=IV
    int    nBuildingClass;     // 0=A, 1=B,  2=C
    int    nAutoCalcRiskCoef;  // 0=Auto Calc, 1=User Defined
    int    nStructureClass;    // 0=General Bldg, 1=Temporary Shed, 2=Low hazard to life, 3=Important Bldg
    double dRiskCoef;
    double dBasicWindSpeed;    // Basic Wind Speed : Vb [m/sec], (33, 39, 44, 47, 50)
    BOOL   bUseForceCoef;      //
    double dForceCoef;         //
    double dFrictionCoef;      // Frictional Drag Coefficient : Cf
    BOOL   bTopography;        // Include topography effect
    double dTopographicFactor; // Topographic Factor k2(1~1.36)
    double dTopographyRange;   // Vertical range for topography factor
    double dGustFactor;
    BOOL    bInclCyclonicRegEffects;
    double dk4Fact;     //k4 Factor Post Cyclonic - 1.3,Industrial - 1.15,Others 1
    double dkdfact;     //kd factor = 0.9 others, 1.0 Circular
    double dkcfact;     //kc factor = 1.0

    T_WIND_IS875_2015_GUST_940 GustFact; // Gust Factor Method
};

struct T_WIND_IS875_2015  // added by Pinakin
{
    int    nProcedure;         // 1=Standard, 2=Gust Effect
    int    nTerrainCategory;   // 0=I, 1=II, 2=III, 3=IV
    int    nBuildingClass;     // 0=A, 1=B,  2=C
    int    nAutoCalcRiskCoef;  // 0=Auto Calc, 1=User Defined
    int    nStructureClass;    // 0=General Bldg, 1=Temporary Shed, 2=Low hazard to life, 3=Important Bldg
    double dRiskCoef;
    double dBasicWindSpeed;    // Basic Wind Speed : Vb [m/sec], (33, 39, 44, 47, 50)
    BOOL   bUseForceCoef;      //
    double dForceCoef;         //
    double dFrictionCoef;      // Frictional Drag Coefficient : Cf
    BOOL   bTopography;        // Include topography effect
    double dTopographicFactor; // Topographic Factor k2(1~1.36)
    double dTopographyRange;   // Vertical range for topography factor
    double dGustFactor;
    BOOL    bInclCyclonicRegEffects;
    double dk4Fact;     //k4 Factor Post Cyclonic - 1.3,Industrial - 1.15,Others 1
    double dkdfact;     //kd factor = 0.9 others, 1.0 Circular
    double dkcfact;     //kc factor = 1.0

    T_WIND_IS875_2015_GUST GustFact; // Gust Factor Method
    void Initialize()
    {
        nProcedure        = 1;
        nTerrainCategory  = 0;
        nBuildingClass    = 0;
        nAutoCalcRiskCoef = 0;
        nStructureClass   = 0;
        dRiskCoef         = 1.0;
        dBasicWindSpeed   = 33.0;
        bUseForceCoef     = FALSE;
        dForceCoef        = 1.0;
        dFrictionCoef     = 0.01;
        bTopography       = FALSE;
        dTopographicFactor= 1.0;
        dTopographyRange  = 0.0;
        dGustFactor       = 1.0;
        bInclCyclonicRegEffects = FALSE;
        dk4Fact           = 1.0;
        dkdfact           = 0.9;
        dkcfact           = 1.0;
        GustFact.Initialize();
    }
    void Convert890(T_WIND_IS875_2015_890& rData)
    {
        nProcedure        = rData.nProcedure;
        nTerrainCategory  = rData.nTerrainCategory;
        nBuildingClass    = rData.nBuildingClass;
        nAutoCalcRiskCoef = rData.nAutoCalcRiskCoef;
        nStructureClass   = rData.nStructureClass;
        dRiskCoef         = rData.dRiskCoef;
        dBasicWindSpeed   = rData.dBasicWindSpeed;
        bUseForceCoef     = rData.bUseForceCoef;
        dForceCoef        = rData.dForceCoef;
        dFrictionCoef     = rData.dFrictionCoef;
        bTopography       = rData.bTopography;
        dTopographicFactor= rData.dTopographicFactor;
        dTopographyRange  = rData.dTopographyRange;
        dGustFactor       = rData.dGustFactor;
        bInclCyclonicRegEffects = rData.bInclCyclonicRegEffects;
        dk4Fact           = rData.dk4Fact;
        dkdfact           = rData.dkdfact;
        dkcfact           = rData.dkcfact;
        GustFact.Initialize();
    }
	void Convert940(T_WIND_IS875_2015_940& rData)
	{
		nProcedure = rData.nProcedure;
		nTerrainCategory = rData.nTerrainCategory;
		nBuildingClass = rData.nBuildingClass;
		nAutoCalcRiskCoef = rData.nAutoCalcRiskCoef;
		nStructureClass = rData.nStructureClass;
		dRiskCoef = rData.dRiskCoef;
		dBasicWindSpeed = rData.dBasicWindSpeed;
		bUseForceCoef = rData.bUseForceCoef;
		dForceCoef = rData.dForceCoef;
		dFrictionCoef = rData.dFrictionCoef;
		bTopography = rData.bTopography;
		dTopographicFactor = rData.dTopographicFactor;
		dTopographyRange = rData.dTopographyRange;
		dGustFactor = rData.dGustFactor;
		bInclCyclonicRegEffects = rData.bInclCyclonicRegEffects;
		dk4Fact = rData.dk4Fact;
		dkdfact = rData.dkdfact;
		dkcfact = rData.dkcfact;
		GustFact.Convert940(rData.GustFact);
	}
};

struct T_WIND_TAIWAN86  // added by KYM
{
    int    nSiteCategory;   // 0=100, 1=150 2=200, 3=250
    double dShapeFactor;
    void Initialize()
    {
        nSiteCategory  = 0;
        dShapeFactor   = 1.0;
    }
};

struct T_WIND_JP2004
{
    int     nProcedure;         // 1 = Simple, 2 = General
    double  dBasicWindSpeed;
    double  dMeanWindSpeed;     // 500년 주기 평균풍속
    double  dBuildingHeight;    // building height
    int     nExposureCategory;  // 0=I, 1=II, 2=III, 3=IV, 4=V
    double  dDirectionFactorX;
    double  dDirectionFactorY;
    double  dEnvironmentFactor;
    int     nForceCoefMethod;   // 0=Automatic, 1=User Defined
    double  dForceCoefficientX;
    double  dForceCoefficientY;
    BOOL    bTopographyX;       // include topographic effect or X-Directional topographic effect
    int     nHillShapeX;        // 0=Escarpment(경사지), 1=Hill(언덕)
    double  dHillHeightX;
    double  dHillLengthX;
    double  dBldgDistanceX;     // crest-building distance
    BOOL    bTopographyY;       // Y-Directional topographic effect
    int     nHillShapeY;        // 0=Escarpment(경사지), 1=Hill(언덕)
    double  dHillHeightY;
    double  dHillLengthY;
    double  dBldgDistanceY;     // crest-building distance
    double  dBreadthX;
    double  dBreadthY;
    double  dFrequencyX;
    double  dFrequencyY;
    double  dFrequencyR;
    double  dDampingX;
    double  dDampingY;
    double  dDampingR;
    double  dModeFactorX;
    double  dModeFactorY;
    double  dModeFactorR;
    double  dGustFactorX;
    double  dGustFactorY;
    double  dLoadFactorX;
    double  dLoadFactorY;
    double  dLoadFactorR;
    int     nWindDirection;   // 0=X-Direction, 1=Y-Direction
    int     nCombType;        // 0=Type1, 1=Type2, 2=Type3
    void Initialize()
    {
        nProcedure = 2;
        dBasicWindSpeed = 36.;
        dMeanWindSpeed  = 40.;
        dBuildingHeight = 0.;
        nExposureCategory = 3;
        dDirectionFactorX = 0.85;
        dDirectionFactorY = 0.85;
        dEnvironmentFactor = 1.0;
        nForceCoefMethod = 0;
        dForceCoefficientX = 1.12;
        dForceCoefficientY = 1.12;
        bTopographyX = FALSE;
        nHillShapeX = 0;
        dHillHeightX = 0.;
        dHillLengthX = 0.;
        dBldgDistanceX = 0.;
        bTopographyY = FALSE;
        nHillShapeY = 0;
        dHillHeightY = 0.;
        dHillLengthY = 0.;
        dBldgDistanceY = 0.;
        dBreadthX = 0.;
        dBreadthY = 0.;
        dFrequencyX = 0.;
        dFrequencyY = 0.;
        dFrequencyR = 0.;
        dDampingX = 0.;
        dDampingY = 0.;
        dDampingR = 0.;
        dModeFactorX = 1.;
        dModeFactorY = 1.;
        dModeFactorR = 1.;
        dGustFactorX = 2.0;
        dGustFactorY = 2.0;
        dLoadFactorX = 1.0;
        dLoadFactorY = 0.4;
        dLoadFactorR = 0.4;
        nWindDirection = 0;
        nCombType = 0;
    }
};

struct T_WIND_EURO2005_940
{
    int    nTerrain;           // Terrain Category : 0=0, 1=I, 2=II, 3=III, 4=IV (Recommended), Structure Type 10=1(Terrain Category 0), 11=2(Terrain Category 2) (Singapore)
    double dCfr;
    double dH;
    double dVb;
    double dCdir;
    double dCseason;
    double dKl;
    int    nCpeMethod;         // 0=Automatic, 1=User Defined
    double dCpe10;
    double dCpe01;
    double dCpeLee;
    int    nLackMethod;        // 0=Automatic, 1=User Defined
    double dLackFactor;
    double dCsCd;
    BOOL   bOrography;
    int    nOroType;           // 0=Cliffs or Escarpment, 1=Hills or Ridges
    int    nBldgLoc;           // 0=Upwind, 1=Downwind
    double dOroH;
    double dOroLu;
    double dOroLd;
    double dOroX;
    BOOL   bNeighbor;
    double dNeiHa;
    double dNeiX;
    double dNeiDl;
    double dNeiHh;
    BOOL   bDisplace;
    double dDisHa;
    double dDisX;
    double dCsB;
    double dCsD;
    double dCsN1;
    double dCsDt;
    BOOL   bUseForceCoef;
    double dForceCoef;
};

struct T_WIND_EURO2005
{
    int    nTerrain;           // Terrain Category : 0=0, 1=I, 2=II, 3=III, 4=IV (Recommended), Structure Type 10=1(Terrain Category 0), 11=2(Terrain Category 2) (Singapore)
    double dCfr;
    double dH;
    double dVb;
    double dCdir;
    double dCseason;
    double dKl;
    int    nCpeMethod;         // 0=Automatic, 1=User Defined
    double dCpe10;
    double dCpe01;
    double dCpeLee;
    int    nLackMethod;        // 0=Automatic, 1=User Defined
    double dLackFactor;
    double dCsCd;
    BOOL   bOrography;
    int    nOroType;           // 0=Cliffs or Escarpment, 1=Hills or Ridges
    int    nBldgLoc;           // 0=Upwind, 1=Downwind
    double dOroH;
    double dOroLu;
    double dOroLd;
    double dOroX;
    BOOL   bNeighbor;
    double dNeiHa;
    double dNeiX;
    double dNeiDl;
    double dNeiHh;
    BOOL   bDisplace;
    double dDisHa;
    double dDisX;
    double dCsB;
    double dCsD;
    double dCsN1;
    double dCsDt;
    BOOL   bUseForceCoef;
    double dForceCoef;
    double dZs;
    void Initialize()
    {
        nTerrain = 2;           // 0=0, 1=I, 2=II, 3=III, 4=IV
        dCfr = 0.0;
        dH = 0.0;
        dVb = 26.;
        dCdir = 1.0;
        dCseason = 1.0;
        dKl = 1.0;
        nCpeMethod = 0;         // 0=Automatic, 1=User Defined
        dCpe10 = 0.8;
        dCpe01 = 1.0;
        dCpeLee = -0.7;
        nLackMethod = 0;        // 0=Automatic, 1=User Defined
        dLackFactor = 1.0;
        dCsCd = 1.0;
        bOrography = FALSE;
        nOroType = 0;           // 0=Cliffs or Escarpment, 1=Hills or Ridges
        nBldgLoc = 0;           // 0=Upwind, 1=Downwind
        dOroH = 0.0;
        dOroLu = 0.0;
        dOroLd = 0.0;
        dOroX = 0.0;
        bNeighbor = FALSE;
        dNeiHa = 0.0;
        dNeiX = 0.0;
        dNeiDl = 0.0;
        dNeiHh = 0.0;
        bDisplace = FALSE;
        dDisHa = 0.0;
        dDisX = 0.0;
        dCsB = 0.0;
        dCsD = 0.0;
        dCsN1 = 0.0;
        dCsDt = 0.05;
        bUseForceCoef = FALSE;
        dForceCoef = 1.0;
        dZs = 0.0;
    }
    void Convert940(T_WIND_EURO2005_940& rData)
    {
		nTerrain = rData.nTerrain;
		dCfr = rData.dCfr;
		dH = rData.dH;
		dVb = rData.dVb;
		dCdir = rData.dCdir;
		dCseason = rData.dCseason;
		dKl = rData.dKl;
		nCpeMethod = rData.nCpeMethod;
		dCpe10 = rData.dCpe10;
		dCpe01 = rData.dCpe01;
		dCpeLee = rData.dCpeLee;
		nLackMethod = rData.nLackMethod;
		dLackFactor = rData.dLackFactor;
		dCsCd = rData.dCsCd;
		bOrography = rData.bOrography;
		nOroType = rData.nOroType;
		nBldgLoc = rData.nBldgLoc;
		dOroH = rData.dOroH;
		dOroLu = rData.dOroLu;
		dOroLd = rData.dOroLd;
		dOroX = rData.dOroX;
		bNeighbor = rData.bNeighbor;
		dNeiHa = rData.dNeiHa;
		dNeiX = rData.dNeiX;
		dNeiDl = rData.dNeiDl;
		dNeiHh = rData.dNeiHh;
		bDisplace = rData.bDisplace;
		dDisHa = rData.dDisHa;
		dDisX = rData.dDisX;
		dCsB = rData.dCsB;
		dCsD = rData.dCsD;
		dCsN1 = rData.dCsN1;
		dCsDt = rData.dCsDt;
		bUseForceCoef = rData.bUseForceCoef;
		dForceCoef = rData.dForceCoef;
		dZs = 0.0;
	}
};

struct T_WIND_KBC2009_790
{
    int    nExposureCategory; // 1=A 2=B 3=C 4=D
    double dBasicWindSpeed;   //  m/sec
    double dImportanceFactor;
    double dRoofHeight;
    BOOL   bTopographicEffect;
    double dKzt;
    double dVzt;  // [사용안함] PMS:4191-HSSHIM-20100610. KBC2009에서는 dVzt를 사용하지 않기로 함.
    int    nRigidity;          // 0: Rigid Structure, 1: Flexible Structure
    double dGustFactorX;
    double dGustFactorY;
    BOOL   bUseForceCoef;
    double dForceCoefficient;

    void Initialize()
    {
        nExposureCategory=2;
        dBasicWindSpeed=30.;
        dImportanceFactor=1.;
        dRoofHeight=0.;
        bTopographicEffect = FALSE;
        dKzt=1.;
        dVzt=0.;
        nRigidity = 0;
        dGustFactorX = 2.2;
        dGustFactorY = 2.2;
        bUseForceCoef = FALSE;
        dForceCoefficient = 1.0;
    }
};

struct T_WIND_KBC2009
{
    int    nExposureCategory; // 1=A 2=B 3=C 4=D
    double dBasicWindSpeed;   //  m/sec
    double dImportanceFactor;
    double dRoofHeight;
    BOOL   bTopographicEffect;
    double dKzt;
    double dVzt;  // [사용안함] PMS:4191-HSSHIM-20100610. KBC2009에서는 dVzt를 사용하지 않기로 함.
    int    nRigidity;          // 0: Rigid Structure, 1: Flexible Structure
    double dGustFactorX;
    double dGustFactorY;
    BOOL   bUseForceCoef;
    double dForceCoefficient;
    BOOL    bAutoCalc;          // 7.9.1에서 추가 // PMS:4223-HSSHIM-20110420
    T_KEY   keyWnat;            // 7.9.1에서 추가 // PMS:4223-HSSHIM-20110420. // Cf 자동 계산용 DB(WNAT) key

    void Initialize()
    {
        nExposureCategory  = 2;
        dBasicWindSpeed    = 30.;
        dImportanceFactor  = 1.;
        dRoofHeight        = 0.;
        bTopographicEffect = FALSE;
        dKzt               = 1.;
        dVzt               = 0.;
        nRigidity          = 0;
        dGustFactorX       = 2.2;
        dGustFactorY       = 2.2;
        bUseForceCoef      = FALSE;
        dForceCoefficient  = 1.0;
        bAutoCalc = FALSE;
        keyWnat   = 0;
    }
    void Convert790(T_WIND_KBC2009_790& rData)
    {
        nExposureCategory  = rData.nExposureCategory;
        dBasicWindSpeed    = rData.dBasicWindSpeed;
        dImportanceFactor  = rData.dImportanceFactor;
        dRoofHeight        = rData.dRoofHeight;
        bTopographicEffect = rData.bTopographicEffect;
        dKzt               = rData.dKzt;
        dVzt               = rData.dVzt;
        nRigidity          = rData.nRigidity;
        dGustFactorX       = rData.dGustFactorX;
        dGustFactorY       = rData.dGustFactorY;
        bUseForceCoef      = rData.bUseForceCoef;
        dForceCoefficient  = rData.dForceCoefficient;
        bAutoCalc = FALSE;
        keyWnat   = 0;
    }
};

struct T_WIND_KBC2015
{
    int     nMethod;                // 0: Simplified Method, 1: General Method

    double  dSimpleBasicWindSpeed;  //  m/sec
    double  dSimpleRoofHeight;
    double  dSimpleCe;

    int     nExposureCategory;      // 1=A 2=B 3=C 4=D
    double  dBasicWindSpeed;        //  m/sec
    double  dImportanceFactor;
    double  dRoofHeight;
    BOOL    bTopographicEffect;
    double  dKzt;
    double  dVzt;
    int     nRigidity;              // 0: Rigid Structure, 1: Flexible Structure
    double  dGustFactorX;
    double  dGustFactorY;
    BOOL    bUseForceCoef;
    double  dForceCoefficient;
    BOOL    bAutoCalc;
    T_KEY   keyWnat;                // Cf 자동 계산용 DB(WNAT) key
    int     nWnatStructType;

    int     nBuildingType;          // 0: Middle Low Rise Building, 1:High Rise Building
    BOOL    bAcrossWind;
    BOOL    bTorsionalWind;
    BOOL    bWindResponse;

    // Parameters of Wind Vibration
    double  dBLy;               // X-Breadth
    double  dBLx;               // Y-Breadth
    double  dNox;               // X-Natural Frequency
    double  dNoy;               // Y-Natural Frequency
    double  dNot;               // Torsional Natural Frequency
    double  dMx1st;             // X-1st vibration Generalized Mass
    double  dMy1st;             // Y-1st vibration Generalized Mass
    double  dMt1st;             // Generalized inertial Moment
    double  dDampingRatio;

    T_KEY   keyWA;              // bAcrossWind 체크시 생성된 WA
    T_KEY   keyWT;              // bTorsionalWind 체크시 생선된 WT

    void Initialize()
    {
        nMethod               = 1;
        dSimpleBasicWindSpeed = 26.;
        dSimpleRoofHeight     = 0.;
        dSimpleCe             = 1.;
        nExposureCategory     = 2;
        dBasicWindSpeed       = 26.;
        dImportanceFactor     = 1.;
        dRoofHeight           = 0.;
        bTopographicEffect    = FALSE;
        dKzt                  = 1.;
        dVzt                  = 0.;
        nRigidity             = 0;
        dGustFactorX          = 2.2;
        dGustFactorY          = 2.2;
        bUseForceCoef         = FALSE;
        dForceCoefficient     = 1.0;
        bAutoCalc             = FALSE;
        keyWnat               = 0;
        nWnatStructType       = 0;
        nBuildingType         = 1;
        bAcrossWind           = FALSE;
        bTorsionalWind        = FALSE;
        bWindResponse         = FALSE;
        dBLy                  = 0.;
        dBLx                  = 0.;
        dNox                  = 0.;
        dNoy                  = 0.;
        dNot                  = 0.;
        dMx1st                = 0.;
        dMy1st                = 0.;
        dMt1st                = 0.;
        dDampingRatio         = 0.;
        keyWA                 = 0;
        keyWT                 = 0;
    }
};

struct T_WIND_KDS2021
{
    int     nMethod;                // 0: Simplified Method, 1: General Method
    BOOL    bVortexShedding;
    double  dSimpleBasicWindSpeed;  //  m/sec
    double  dSimpleRoofHeight;
    double  dSimpleCe;

    int     nExposureCategory;      // 1=A 2=B 3=C 4=D
    double  dBasicWindSpeed;        //  m/sec
    double  dImportanceFactor;
    double  dRoofHeight;
    BOOL    bTopographicEffect;
    double  dKzt;
    double  dVzt;
    double  dDistFactorX;
    double  dDistFactorY;
    int     nRigidity;              // 0: Rigid Structure, 1: Flexible Structure
    double  dGustFactorX;
    double  dGustFactorY;
    BOOL    bUseForceCoef;
    double  dForceCoefficient;
    BOOL    bAutoCalc;
    T_KEY   keyWnat;                // Cf 자동 계산용 DB(WNAT) key
    int     nWnatStructType;

    int     nBuildingType;          // 0: Middle Low Rise Building, 1:High Rise Building
    BOOL    bAcrossWind;
    BOOL    bTorsionalWind;
    BOOL    bWindResponse;
    BOOL    bLShapeBuilding;
    double  dAmFactorAlong[2];
    double  dAmFactorAcross[2];
    double  dAmFactorTorsional[2];
    // Vortex Shedding
    double dVortexAverageRoof;
    double dVortexDM;               // Diameter of 2H/3 Level (Dm)
    double dVortexDB;               // Diameter of Bottom (DB)
    double dVortexN;                // Natural Frequency (N)
    double dVortexMass;             // Mass(M)
    double dVortexDamp;
    // Parameters of Wind Vibration
    double  dBLy;                   // X-Breadth
    double  dBLx;                   // Y-Breadth
    double  dNox;                   // X-Natural Frequency
    double  dNoy;                   // Y-Natural Frequency
    double  dNot;                   // Torsional Natural Frequency
    double  dMx1st;                 // X-1st vibration Generalized Mass
    double  dMy1st;                 // Y-1st vibration Generalized Mass
    double  dMt1st;                 // Generalized inertial Moment
    double  dDampingRatio;
    double  dTotalMass;
    double  dVibrationMode;

    T_KEY   keyWA;                  // bAcrossWind 체크시 생성된 WA
    T_KEY   keyWT;                  // bTorsionalWind 체크시 생선된 WT

    void Initialize()
    {
        nMethod = 1;
        bVortexShedding = FALSE;
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
        nRigidity = 0;
        dDistFactorX = 1.0;
        dDistFactorY = 1.0;
        dGustFactorX = 2.2;
        dGustFactorY = 2.2;
        bUseForceCoef = FALSE;
        dForceCoefficient = 1.0;
        bAutoCalc = FALSE;
        keyWnat = 0;
        nWnatStructType = 0;
        nBuildingType = 1;
        bAcrossWind = FALSE;
        bTorsionalWind = FALSE;
        bWindResponse = FALSE;
        bLShapeBuilding = FALSE;

        dAmFactorAlong[0] = dAmFactorAlong[1] = 1.0;
        dAmFactorAcross[0] = dAmFactorAcross[1] = 1.0;
        dAmFactorTorsional[0] = dAmFactorTorsional[1] = 1.0;

        dVortexAverageRoof = 0.;
        dVortexDM = 0.;
        dVortexDB = 0.;
        dVortexN = 0.;
        dVortexMass = 0.;
        dVortexDamp = 0.;

        dBLy = 0.;
        dBLx = 0.;
        dNox = 0.;
        dNoy = 0.;
        dNot = 0.;
        dMx1st = 0.;
        dMy1st = 0.;
        dMt1st = 0.;
        dDampingRatio = 0.;
        dTotalMass = 0.;
        dVibrationMode = 0.5;
        keyWA = 0;
        keyWT = 0;
    }
};

enum EN_DPT_WIND_METHOD
{
    EN_DPT_WIND_METHOD_SIMPLE = 0,
    EN_DPT_WIND_METHOD_DETAIL,
};

enum EN_DPT_WIND_ZONE
{
    EN_DPT_WIND_ZONE1 = 0,
    EN_DPT_WIND_ZONE2,
    EN_DPT_WIND_ZONE3,
    EN_DPT_WIND_ZONE4A,
    EN_DPT_WIND_ZONE4B,
};

enum EN_DPT_WIND_EXPO // Exposure
{
    EN_DPT_WIND_EXPO_A = 0,
    EN_DPT_WIND_EXPO_B,
    EN_DPT_WIND_EXPO_C,
    EN_DPT_WIND_EXPO_D,
};

enum EN_DPT_WIND_HLSP // Hill Shape
{
    EN_DPT_WIND_HLSP_2D_RIDGEVALLY = 0,
    EN_DPT_WIND_HLSP_2D_ESCARPMENT,
    EN_DPT_WIND_HLSP_3D_AXISYMHILL,
};

enum EN_DPT_WIND_BDLC  // Building Location
{
    EN_DPT_WIND_BDLC_UPWIND = 0,
    EN_DPT_WIND_BDLC_DNWIND,
};

enum EN_DPT_WIND_OPEN_STRT // Structure Open Type
{
    EN_DPT_WIND_OPEN_STRT_ENCL = 0, // Enclosed
    EN_DPT_WIND_OPEN_STRT_OPEN, // Opened or ETC
};

enum EN_DPT_WIND_STRT_STIFF  // Structure Type
{
    EN_DPT_WIND_STRT_STIFF_RIGID = 0,
    EN_DPT_WIND_STRT_STIFF_FLEX,
};

enum EN_DPT_WIND_OPEN_SUB_STRT // Structure Open Sub Type
{
    EN_DPT_WIND_OPEN_SUB_STRT_OPEN = 0, // Opened Structure
    EN_DPT_WIND_OPEN_SUB_STRT_TANK, // Chimney, Tank, and Similar
    EN_DPT_WIND_OPEN_SUB_STRT_TRUS, // Truss Tower
};

enum EN_DPT_WIND_OPEN_CHIMNEY
{
    EN_DPT_WIND_OPEN_CHIMNEY_SQUA_NORM = 0,
    EN_DPT_WIND_OPEN_CHIMNEY_SQUA_DIAG,
    EN_DPT_WIND_OPEN_CHIMNEY_HEXA_OCTA,
    EN_DPT_WIND_OPEN_CHIMNEY_ROUND_SMOOTH,
    EN_DPT_WIND_OPEN_CHIMNEY_ROUND_ROUGH,
    EN_DPT_WIND_OPEN_CHIMNEY_ROUND_VERY_ROUGH,
    EN_DPT_WIND_OPEN_CHIMNEY_CIRCLE,
};

enum EN_DPT_WIND_OPEN_TRS_PLANE // Structure Open Sub Type, TRUSS, Plane Type + Structure Open Sub Type, TRUSS, Member Type
{
    EN_DPT_WIND_OPEN_TRS_PLANE_SQUARE_PIPE = 0, // Square - Steel PIPE
    EN_DPT_WIND_OPEN_TRS_PLANE_SQUARE_RECT, // Square - Rectangle Cross Section
    EN_DPT_WIND_OPEN_TRS_PLANE_SQUARE_ETC,  // Square - Other type
    EN_DPT_WIND_OPEN_TRS_PLANE_TRIANG_PIPE, // Triangle - Steel Rounded
    EN_DPT_WIND_OPEN_TRS_PLANE_TRIANG_RECT, // Triangle - Rectangle Cross Section
    EN_DPT_WIND_OPEN_TRS_PLANE_TRIANG_ETC,  // Triangle - Other type
};

enum EN_DPT_WIND_INT_PRES // Internal Pressure Type
{
    EN_DPT_WIND_INT_PRES_ENCL = 0, // ( Partial ) Enclosed
    EN_DPT_WIND_INT_PRES_PATO, // Partially Opened
    EN_DPT_WIND_INT_PRES_EXTO, // Excellent Opening
};

enum EN_DPT_WIND_ROOF_BEAM_DIR // Roof information Beam Direction
{
    EN_DPT_WIND_ROOF_BEAM_DIR_NORMAL = 0, // Normal to Wind Direction
    EN_DPT_WIND_ROOF_BEAM_DIR_PARALL, // Parallel to Wind Direction
};

enum EN_DPT_WIND_ROOF_LOAD_CASE // Roof Wind Load Case
{
    EN_DPT_WIND_ROOF_LOAD_CASE_A = 0, // A
    EN_DPT_WIND_ROOF_LOAD_CASE_B, // B
};

enum EN_DPT_WIND_ROOF_FLOW // Roof Wind Flow
{
    EN_DPT_WIND_ROOF_FLOW_CLEAR = 0, // A
    EN_DPT_WIND_ROOF_FLOW_OBSTRUCTED, // B
};

enum EN_DPT_WIND_VIBR
{
    EN_DPT_WIND_VIBR_FREQ_X = 0,
    EN_DPT_WIND_VIBR_FREQ_Y,
    EN_DPT_WIND_VIBR_FREQ_T,
    EN_DPT_WIND_VIBR_MASS_X,
    EN_DPT_WIND_VIBR_MASS_Y,
    EN_DPT_WIND_VIBR_MASS_T,
    EN_DPT_WIND_VIBR_DAMP_X,
    EN_DPT_WIND_VIBR_DAMP_Y,
    EN_DPT_WIND_VIBR_DAMP_T,
    EN_DPT_WIND_VIBR_NUM,
};

struct T_WIND_DPT2007_VIBR
{
    BOOL bAcross;
    BOOL bTorsion;
    BOOL bResponse;
    double dParam[EN_DPT_WIND_VIBR_NUM];

    void Init()
    {
        bAcross = FALSE;
        bTorsion = FALSE;
        bResponse = FALSE;
        for ( double& rdVal : dParam )
        {
            rdVal = 0.0;
        }
    }
    void Copy(T_WIND_DPT2007_VIBR& rData)
    {
        rData.bAcross   = bAcross;
        rData.bTorsion  = bTorsion;
        rData.bResponse = bResponse;
        for ( int i = 0; i < EN_DPT_WIND_VIBR_NUM; ++i )
        {
            rData.dParam[i] = dParam[i];
        }
    }
};

struct T_WIND_DPT2007_960
{
    EN_DPT_WIND_METHOD unMethod;
    EN_DPT_WIND_ZONE   unWindZone;
    double             dBasicWindSpeed; // m/sec
    EN_DPT_WIND_EXPO   unExpoCategory;
    double             dImportanceFactor;
    BOOL               bTopographicEffect;
    EN_DPT_WIND_HLSP   unHillShape;
    EN_DPT_WIND_BDLC   unBldgLocation;
    double             dHillHeight;
    double             dHillLength;
    double             dHillDistance;
    BOOL               bMinimumLoad;

    double                     dTopographicEffectLevel;
    EN_DPT_WIND_OPEN_STRT      unOpenStructType;
    EN_DPT_WIND_STRT_STIFF     unOpenStructStiffType;;  // Rigid / Flexible
    BOOL                       bRoof; // consider Roof
    BOOL                       bIntPress; // consider Internal Pressure - ( Partial ) Enclosed Only
    double                     dBreadth[2]; // a, a+90
    EN_DPT_WIND_OPEN_SUB_STRT  unOpenSubStructType;
    EN_DPT_WIND_OPEN_CHIMNEY   unGrossSectType;
    double                     dhD; // h/D - Opened or ETC - Structure Chimney, Tank, and Similar
    EN_DPT_WIND_OPEN_TRS_PLANE unOpenTrussPlane; // Structure Open Sub Type, TRUSS, Plane Type
    double                     dFidelity[2]; // a, a+90, Structure Open Sub Type, TRUSS, 충실률( 유효수압면적 / 외곽 전면적 )

    double  dFrequency[2]; // a, a+90
    double  dDamping[2];   // a, a+90
    EN_DPT_WIND_INT_PRES unIntEnClass; // Internal Pressure Type

    double dRoofSlope;
    EN_DPT_WIND_ROOF_BEAM_DIR  unRoofBeamDir;   // Roof information Beam Direction
    EN_DPT_WIND_ROOF_LOAD_CASE unRoofWindLoadCase;
    EN_DPT_WIND_ROOF_FLOW      unRoofWindFlow;

    double dGf[2];    // Structure Gust Factor
    double dCpe_W[2]; // ( Partial ) Enclosed Structure : Windward
    double dCpe_L[2]; // ( Partial ) Enclosed Structure : Leeward
    double dCpe_S[2]; // ( Partial ) Enclosed Structure : Side
    double dGCpi[2];  // [0] : a, [1] : a+90
    double dCf[2];    // Opened or ETC Structure Cf

    double dRGpe[2];    // Roof Gust Factor
    BOOL   bRCpe_W[2];
    double dRCpe_W[2];
    double dRCpe_L[2];

    T_WIND_DPT2007_VIBR Vibr;
    T_KEY keyWnat; // Cf 자동 계산용 DB(WNAT) key
    T_KEY keyWA; // bAcrossWind 체크시 생성된 WA
    T_KEY keyWT; // bTorsionalWind 체크시 생선된 WT
};

struct T_WIND_DPT2007
{
    EN_DPT_WIND_METHOD unMethod;
    EN_DPT_WIND_ZONE   unWindZone;
    double             dBasicWindSpeed; // m/sec
    EN_DPT_WIND_EXPO   unExpoCategory;
    double             dImportanceFactor;
    BOOL               bTopographicEffect;
    EN_DPT_WIND_HLSP   unHillShape;
    EN_DPT_WIND_BDLC   unBldgLocation;
    double             dHillHeight;
    double             dHillLength;
    double             dHillDistance;
    BOOL               bMinimumLoad;

    double                     dTopographicEffectLevel;
    EN_DPT_WIND_OPEN_STRT      unOpenStructType;
    EN_DPT_WIND_STRT_STIFF     unOpenStructStiffType;;  // Rigid / Flexible
    BOOL                       bRoof; // consider Roof
    BOOL                       bIntPress; // consider Internal Pressure - ( Partial ) Enclosed Only
    double                     dBreadth[2]; // a, a+90
    EN_DPT_WIND_OPEN_SUB_STRT  unOpenSubStructType;
    EN_DPT_WIND_OPEN_CHIMNEY   unGrossSectType;
    double                     dhD; // h/D - Opened or ETC - Structure Chimney, Tank, and Similar
    EN_DPT_WIND_OPEN_TRS_PLANE unOpenTrussPlane; // Structure Open Sub Type, TRUSS, Plane Type
    double                     dFidelity[2]; // a, a+90, Structure Open Sub Type, TRUSS, 충실률( 유효수압면적 / 외곽 전면적 )

    double  dFrequency[2]; // a, a+90
    double  dDamping[2];   // a, a+90
    EN_DPT_WIND_INT_PRES unIntEnClass; // Internal Pressure Type

    double dRoofSlope;
    EN_DPT_WIND_ROOF_BEAM_DIR  unRoofBeamDir;   // Roof information Beam Direction
    EN_DPT_WIND_ROOF_LOAD_CASE unRoofWindLoadCase;
    EN_DPT_WIND_ROOF_FLOW      unRoofWindFlow;

    double dGf[2];    // Structure Gust Factor
    double dCpe_W[2]; // ( Partial ) Enclosed Structure : Windward
    double dCpe_L[2]; // ( Partial ) Enclosed Structure : Leeward
    double dCpe_S[2]; // ( Partial ) Enclosed Structure : Side
    double dGCpi[2];  // [0] : a, [1] : a+90
    double dCf[2];    // Opened or ETC Structure Cf

    double dRGpe[2];    // Roof Gust Factor
    BOOL   bRCpe_W[2];
    double dRCpe_W[2];
    double dRCpe_L[2];

    T_WIND_DPT2007_VIBR Vibr;
    T_KEY keyWnat; // Cf 자동 계산용 DB(WNAT) key
    T_KEY keyWA; // bAcrossWind 체크시 생성된 WA
    T_KEY keyWT; // bTorsionalWind 체크시 생선된 WT

    int     nEccen_X;   // 0=Pos, 1=Neg, 2=None
    int     nEccen_Y;   // 0=Pos, 1=Neg, 2=None

    void Init()
    {
        unMethod           = EN_DPT_WIND_METHOD_DETAIL;
        unWindZone         = EN_DPT_WIND_ZONE3;
        dBasicWindSpeed    = 29.0; // m/sec
        unExpoCategory     = EN_DPT_WIND_EXPO_C;
        dImportanceFactor  = 1.0;
        bTopographicEffect = FALSE;
        unHillShape        = EN_DPT_WIND_HLSP_2D_RIDGEVALLY;
        unBldgLocation     = EN_DPT_WIND_BDLC_UPWIND;
        dHillHeight        = 0.0;
        dHillLength        = 0.0;
        dHillDistance      = 0.0;
        bMinimumLoad       = TRUE;

        dTopographicEffectLevel = 0.0;
        unOpenStructType = EN_DPT_WIND_OPEN_STRT_ENCL;
        unOpenStructStiffType = EN_DPT_WIND_STRT_STIFF_RIGID;
        bRoof = FALSE;
        bIntPress = FALSE;
        unOpenSubStructType = EN_DPT_WIND_OPEN_SUB_STRT_OPEN;
        unGrossSectType = EN_DPT_WIND_OPEN_CHIMNEY_SQUA_NORM;
        dhD = 0.0;
        unOpenTrussPlane = EN_DPT_WIND_OPEN_TRS_PLANE_SQUARE_RECT;
        unIntEnClass  = EN_DPT_WIND_INT_PRES_ENCL;

        dRoofSlope         = 0.0;
        unRoofBeamDir      = EN_DPT_WIND_ROOF_BEAM_DIR_NORMAL;
        unRoofWindLoadCase = EN_DPT_WIND_ROOF_LOAD_CASE_A;
        unRoofWindFlow     = EN_DPT_WIND_ROOF_FLOW_CLEAR;

        for ( int i = 0; i < 2; ++i )
        {
            dBreadth[i]   = 0.0; 
            dFidelity[i]  = 0.0; 
            dFrequency[i] = 0.0; 
            dDamping[i]   = 0.0; 

            dGf[i]    = 2.5;
            dCpe_W[i] = 0.0;
            dCpe_L[i] = 0.0;
            dCpe_S[i] = 0.0;
            dGCpi[i]  = 0.0;
            dCf[i]    = 0.0;

            dRGpe[i]   = 0.0;
            bRCpe_W[i] = FALSE;
            dRCpe_W[i] = 0.0;
            dRCpe_L[i] = 0.0;
        }

        Vibr.Init();
        keyWnat = NULL;
        keyWA = NULL;
        keyWT = NULL;
        nEccen_X = EN_ECCEN_NONE;
        nEccen_Y = EN_ECCEN_NONE;
    }
    void Copy(T_WIND_DPT2007& rData)
    {
        rData.unMethod           = unMethod;
        rData.unWindZone         = unWindZone;
        rData.dBasicWindSpeed    = dBasicWindSpeed;
        rData.unExpoCategory     = unExpoCategory;
        rData.dImportanceFactor  = dImportanceFactor;
        rData.bTopographicEffect = bTopographicEffect;
        rData.unHillShape        = unHillShape;
        rData.unBldgLocation     = unBldgLocation;
        rData.dHillHeight        = dHillHeight;
        rData.dHillLength        = dHillLength;
        rData.dHillDistance      = dHillDistance;
        rData.bMinimumLoad       = bMinimumLoad;

        rData.dTopographicEffectLevel = dTopographicEffectLevel;
        rData.unOpenStructType        = unOpenStructType;
        rData.unOpenStructStiffType   = unOpenStructStiffType;
        rData.bRoof                   = bRoof;
        rData.bIntPress               = bIntPress;
        rData.unOpenSubStructType     = unOpenSubStructType;
        rData.unGrossSectType         = unGrossSectType;
        rData.dhD                     = dhD;
        rData.unOpenTrussPlane        = unOpenTrussPlane;

        rData.unIntEnClass       = unIntEnClass;
        rData.dRoofSlope         = dRoofSlope;
        rData.unRoofBeamDir      = unRoofBeamDir;
        rData.unRoofWindLoadCase = unRoofWindLoadCase;
        rData.unRoofWindFlow     = unRoofWindFlow;

        for ( int i = 0; i < 2; ++i )
        {
            rData.dBreadth[i]   = dBreadth[i];
            rData.dFidelity[i]  = dFidelity[i];
            rData.dFrequency[i] = dFrequency[i];
            rData.dDamping[i]   = dDamping[i];

            rData.dGf[i]    = dGf[i];
            rData.dCpe_W[i] = dCpe_W[i];
            rData.dCpe_L[i] = dCpe_L[i];
            rData.dCpe_S[i] = dCpe_S[i];
            rData.dGCpi[i]  = dGCpi[i];
            rData.dCf[i]    = dCf[i];

            rData.dRGpe[i]   = dRGpe[i];
            rData.bRCpe_W[i] = bRCpe_W[i];
            rData.dRCpe_W[i] = dRCpe_W[i];
            rData.dRCpe_L[i] = dRCpe_L[i];
        }

        Vibr.Copy(rData.Vibr);    
        rData.keyWnat = keyWnat; 
        rData.keyWA   = keyWA;   
        rData.keyWT   = keyWT;
		rData.nEccen_X = nEccen_X;
		rData.nEccen_Y = nEccen_Y;
    }
    void Convert960(const T_WIND_DPT2007_960& rData)
    {
        unMethod           = rData.unMethod;
        unWindZone         = rData.unWindZone;
        dBasicWindSpeed    = rData.dBasicWindSpeed;
        unExpoCategory     = rData.unExpoCategory;
        dImportanceFactor  = rData.dImportanceFactor;
        bTopographicEffect = rData.bTopographicEffect;
        unHillShape        = rData.unHillShape;
        unBldgLocation     = rData.unBldgLocation;
        dHillHeight        = rData.dHillHeight;
        dHillLength        = rData.dHillLength;
        dHillDistance      = rData.dHillDistance;
        bMinimumLoad       = rData.bMinimumLoad;

        dTopographicEffectLevel = rData.dTopographicEffectLevel;
        unOpenStructType        = rData.unOpenStructType;
        unOpenStructStiffType   = rData.unOpenStructStiffType;
        bRoof                   = rData.bRoof;
        bIntPress               = rData.bIntPress;
        unOpenSubStructType     = rData.unOpenSubStructType;
        unGrossSectType         = rData.unGrossSectType;
        dhD                     = rData.dhD;
        unOpenTrussPlane        = rData.unOpenTrussPlane;
        unIntEnClass            = rData.unIntEnClass;
        dRoofSlope              = rData.dRoofSlope;
        unRoofBeamDir           = rData.unRoofBeamDir;
        unRoofWindLoadCase      = rData.unRoofWindLoadCase;
        unRoofWindFlow          = rData.unRoofWindFlow;

        for (int i = 0; i < 2; ++i)
        {
            dBreadth[i]   = rData.dBreadth[i];
            dFidelity[i]  = rData.dFidelity[i];
            dFrequency[i] = rData.dFrequency[i];
            dDamping[i]   = rData.dDamping[i];
            dGf[i]        = rData.dGf[i];
            dCpe_W[i]     = rData.dCpe_W[i];
            dCpe_L[i]     = rData.dCpe_L[i];
            dCpe_S[i]     = rData.dCpe_S[i];
            dGCpi[i]      = rData.dGCpi[i];
            dCf[i]        = rData.dCf[i];
            dRGpe[i]      = rData.dRGpe[i];
            bRCpe_W[i]    = rData.bRCpe_W[i];
            dRCpe_W[i]    = rData.dRCpe_W[i];
            dRCpe_L[i]    = rData.dRCpe_L[i];
        }

        Vibr = rData.Vibr;
        keyWnat = rData.keyWnat;
        keyWA   = rData.keyWA;
        keyWT   = rData.keyWT;
        nEccen_X = EN_ECCEN_NONE;
        nEccen_Y = EN_ECCEN_NONE;
    }
};

enum EN_ASCE7_EXPOSURE
{
	EN_ASCE7_EXPO_B = 2,
	EN_ASCE7_EXPO_C,
	EN_ASCE7_EXPO_D
};

enum EN_ASCE7_STRUCT_RIGIDITY
{
	EN_ASCE7_RIGID = 0,
	EN_ASCE7_FLEXIBLE
};

enum EN_ASCE7_HILL_SHAPE
{
	EN_2D_RIDGE = 0,
	EN_2D_ESCARPMENT,
	EN_3D_AXISYMMETRICAL_HILL
};

enum EN_ASCE7_BLDG_LOCATION
{
	EN_UPWIND_CREST = 0,
	EN_DNWIND_CREST
};

struct T_WIND_ASCE7_16
{
	double  dBasicWindSpeed;    // mph
	int     nExposureCategory;  // EN_ASCE7_EXPOSURE
	double  dMeadRoofHeight;
	BOOL    bTopography[2];   // [0]=X, [1]=Y
	int     nHillShape[2];    // EN_ASCE7_HILL_SHAPE
	int     nBldgLocation[2]; // EN_ASCE7_BLDG_LOCATION
	double  dHillHeight[2];
	double  dHillLength[2];
	double  dHillDistance[2];   // crest-building distance
	int     nEccen_X;   // 0=Pos, 1=Neg, 2=None
	int     nEccen_Y;   // 0=Pos, 1=Neg, 2=None
		
	double  dDirectionFactorX;
	double  dDirectionFactorY;
	double  dGroundElevationFactorX;
	double  dGroundElevationFactorY;
	int     nRigidity;          // EN_ASCE7_RIGIDITY
	double  dGustFactorX;
	double  dGustFactorY;
	BOOL    bUseForceCoef;
	double  dForceCoefficient;
	
	void Initialize(bool bUnitUS)
	{
		dBasicWindSpeed   = bUnitUS ? 85.0/*mile.h*/ : 38.0/*m/s*/;
		nExposureCategory = EN_ASCE7_EXPO_C;
		dMeadRoofHeight   = 0.0;
		dDirectionFactorX = 0.85;
		dDirectionFactorY = 0.85;
		dGroundElevationFactorX = 1.0;
		dGroundElevationFactorY = 1.0;
		nRigidity         = EN_ASCE7_RIGID;
		dGustFactorX      = 0.85;
		dGustFactorY      = 0.85;
		bUseForceCoef     = FALSE;
		dForceCoefficient = 1.0;
		nEccen_X          = EN_ECCEN_NONE;
		nEccen_Y          = EN_ECCEN_NONE;
		for ( int i=0; i<2; i++ )
		{
			bTopography[i] = FALSE;
			nHillShape[i] = EN_2D_RIDGE;
			nBldgLocation[i] = EN_UPWIND_CREST;
			dHillHeight[i] = 0.0;
			dHillLength[i] = 0.0;
			dHillDistance[i] = 0.0;
		}
	}
};

struct T_WIND_RUS_2016
{
    bool  bWindPulsation;   // 
    int   nPulDirect_X;     // Wind Pulsation Direction X
    int   nPulDirect_Y;     // Wind Pulsation Direction Y
    void Initialize()
    {
        bWindPulsation = false;
        nPulDirect_X = 0;
        nPulDirect_Y = 0;
    }
};

union T_WIND_CODE_915
{
    T_WIND_KS1992   KS1992;
    T_WIND_JP1987   JP1987;
    T_WIND_UBC1997  UBC1997;
    T_WIND_ANSI1982 ANSI1982;
    T_WIND_KS2000   KS2000;
    T_WIND_IBC2000  IBC2000;
    T_WIND_EURO1992 EURO1992; // added by KYM
    T_WIND_BS6399   BS6399;   // added by KYM
    T_WIND_CH2002   CH2002;   // added by KYM
    T_WIND_JPN2000  JPN2000;  // added by KYM
    T_WIND_NBC1995  NBC1995;  // added by KYM
    T_WIND_IS1987   IS1987;   // added by KYM
    T_WIND_TAIWAN86 TAIWAN86;
    T_WIND_JP2004   JP2004;
    T_WIND_EURO2005 EURO2005;
    T_WIND_KBC2009  KBC2009;
    T_WIND_IBC2012  IBC2009;
    T_WIND_IBC2012  IBC2012;
    T_WIND_CH2012_915   CH2012;   // v811 added. added by maxiao
    T_WIND_CH2019_915   CH2019;
    T_WIND_NSR2010  NSR2010;
    T_WIND_KBC2015  KBC2015;
    T_WIND_IS875_2015 IS875_2015; //Pinakin v875
    T_WIND_KBC2015  KDS2019;
    T_WIND_KDS2021  KDS2021;
};

union T_WIND_CODE_960
{
    T_WIND_KS1992   KS1992;
    T_WIND_JP1987   JP1987;
    T_WIND_UBC1997  UBC1997;
    T_WIND_ANSI1982 ANSI1982;
    T_WIND_KS2000   KS2000;
    T_WIND_IBC2000  IBC2000;
    T_WIND_EURO1992 EURO1992; // added by KYM
    T_WIND_BS6399   BS6399;   // added by KYM
    T_WIND_CH2002   CH2002;   // added by KYM
    T_WIND_JPN2000  JPN2000;  // added by KYM
    T_WIND_NBC1995  NBC1995;  // added by KYM
    T_WIND_IS1987   IS1987;   // added by KYM
    T_WIND_TAIWAN86 TAIWAN86;
    T_WIND_JP2004   JP2004;
    T_WIND_EURO2005 EURO2005;
    T_WIND_KBC2009  KBC2009;
    T_WIND_IBC2012  IBC2009;
    T_WIND_IBC2012  IBC2012;
    T_WIND_CH2012   CH2012;   // v811 added. added by maxiao
    T_WIND_CH2019   CH2019;
    T_WIND_NSR2010  NSR2010;
    T_WIND_KBC2015  KBC2015;
    T_WIND_IS875_2015 IS875_2015; //Pinakin v875
    T_WIND_KBC2015  KDS2019;
    T_WIND_KDS2021  KDS2021;
    T_WIND_DPT2007_960  DPT2007;
    T_WIND_ASCE7_16 ASCE7_16;
    T_WIND_ASCE7_16 ASCE7_22;
    T_WIND_ASCE7_16 NSCP2024;
    T_WIND_RUS_2016 RUS2016;
};

union T_WIND_CODE
{
    T_WIND_KS1992   KS1992;
    T_WIND_JP1987   JP1987;
    T_WIND_UBC1997  UBC1997;
    T_WIND_ANSI1982 ANSI1982;
    T_WIND_KS2000   KS2000;
    T_WIND_IBC2000  IBC2000;
    T_WIND_EURO1992 EURO1992; // added by KYM
    T_WIND_BS6399   BS6399;   // added by KYM
    T_WIND_CH2002   CH2002;   // added by KYM
    T_WIND_JPN2000  JPN2000;  // added by KYM
    T_WIND_NBC1995  NBC1995;  // added by KYM
    T_WIND_IS1987   IS1987;   // added by KYM
    T_WIND_TAIWAN86 TAIWAN86;
    T_WIND_JP2004   JP2004;
    T_WIND_EURO2005 EURO2005;
    T_WIND_KBC2009  KBC2009;
    T_WIND_IBC2012  IBC2009;
    T_WIND_IBC2012  IBC2012;
    T_WIND_CH2012   CH2012;   // v811 added. added by maxiao
    T_WIND_CH2019   CH2019;
    T_WIND_NSR2010  NSR2010;
    T_WIND_KBC2015  KBC2015;
    T_WIND_IS875_2015 IS875_2015; //Pinakin v875
    T_WIND_KBC2015  KDS2019;
    T_WIND_KDS2021  KDS2021;
    T_WIND_DPT2007  DPT2007;
    T_WIND_ASCE7_16 ASCE7_16;
    T_WIND_ASCE7_16 ASCE7_22;
    T_WIND_ASCE7_16 NSCP2024;
    T_WIND_RUS_2016 RUS2016;
    void Initialize(int nCode)
    {
        switch ( nCode )
        {
        case KS_W_92:       KS1992.Initialize();        break;
        case JP_W_87:       JP1987.Initialize();        break;
        case UBC_W_97:      UBC1997.Initialize();       break;
        case ANSI_W_82:     ANSI1982.Initialize();      break;
        case KS_W_2000:     KS2000.Initialize();        break;
        case IBC_W_2000:    IBC2000.Initialize();       break;
        case EURO_W_1992:   EURO1992.Initialize();      break;
        case BS6399_W_1997: BS6399.Initialize();        break;
        case CH_W_2002:     CH2002.Initialize();        break;
        case JPN_W_2000:    JPN2000.Initialize();       break;
        case NBC_W_1995:    NBC1995.Initialize();       break;
        case IS_W_1987:     IS1987.Initialize();        break;
        case TAIWAN_W_1986: TAIWAN86.Initialize();      break;
        case JP_W_2004:     JP2004.Initialize();        break;
        case EURO_W_2005:   EURO2005.Initialize();      break;
        case KBC_W_2009:    KBC2009.Initialize();       break;
        case IBC_W_2009:    IBC2009.Initialize();       break;
        case IBC_W_2012:    IBC2012.Initialize();       break;
        case CH_W_2012:     CH2012.Initialize();        break;
        case CH_W_2021:     CH2019.Initialize();        break;
        case NSR_W_2010:    NSR2010.Initialize();       break;
        case KBC_W_2016:    KBC2015.Initialize();       break;
        case IS_W_875_2015: IS875_2015.Initialize();    break;
        case KDS_W_2019:    KDS2019.Initialize();       break;
        case KDS_W_2022:    KDS2021.Initialize();       break;
        case DPT_W_2007:    DPT2007.Init(); break;
        case ASCE7_W_2016:  ASCE7_16.Initialize(true);      break;
        case ASCE7_W_2022:  ASCE7_22.Initialize(true);      break;
        case NSCP_W_2024:   NSCP2024.Initialize(false);      break;
        case RUS_W_2016:    RUS2016.Initialize();       break;
        }
    }
    void Convert915(int nCode, const T_WIND_CODE_915& cData)
    {
        switch ( nCode )
        {
        case KS_W_92:       KS1992     = cData.KS1992;     break;
        case JP_W_87:       JP1987     = cData.JP1987;     break;
        case UBC_W_97:      UBC1997    = cData.UBC1997;    break;
        case ANSI_W_82:     ANSI1982   = cData.ANSI1982;   break;
        case KS_W_2000:     KS2000     = cData.KS2000;     break;
        case IBC_W_2000:    IBC2000    = cData.IBC2000;    break;
        case EURO_W_1992:   EURO1992   = cData.EURO1992;   break;
        case BS6399_W_1997: BS6399     = cData.BS6399;     break;
        case CH_W_2002:     CH2002     = cData.CH2002;     break;
        case JPN_W_2000:    JPN2000    = cData.JPN2000;    break;
        case NBC_W_1995:    NBC1995    = cData.NBC1995;    break;
        case IS_W_1987:     IS1987     = cData.IS1987;     break;
        case TAIWAN_W_1986: TAIWAN86   = cData.TAIWAN86;   break;
        case JP_W_2004:     JP2004     = cData.JP2004;     break;
        case EURO_W_2005:   EURO2005   = cData.EURO2005;   break;
        case KBC_W_2009:    KBC2009    = cData.KBC2009;    break;
        case IBC_W_2009:    IBC2009    = cData.IBC2009;    break;
        case IBC_W_2012:    IBC2012    = cData.IBC2012;    break;
        case CH_W_2012:     CH2012.Convert915(cData.CH2012);     break;
        case CH_W_2021:     CH2019.Convert915(cData.CH2019);     break;
        case NSR_W_2010:    NSR2010    = cData.NSR2010;    break;
        case KBC_W_2016:    KBC2015    = cData.KBC2015;    break;
        case IS_W_875_2015: IS875_2015 = cData.IS875_2015; break;
        case KDS_W_2019:    KDS2019    = cData.KDS2019;    break;
        case KDS_W_2022:    KDS2021    = cData.KDS2021;    break;
        default: Initialize(nCode); break;
        }
    }
    void Convert960(int nCode, const T_WIND_CODE_960& cData)
    {
        switch (nCode)
        {
        case KS_W_92:       KS1992 = cData.KS1992;     break;
        case JP_W_87:       JP1987 = cData.JP1987;     break;
        case UBC_W_97:      UBC1997 = cData.UBC1997;    break;
        case ANSI_W_82:     ANSI1982 = cData.ANSI1982;   break;
        case KS_W_2000:     KS2000 = cData.KS2000;     break;
        case IBC_W_2000:    IBC2000 = cData.IBC2000;    break;
        case EURO_W_1992:   EURO1992 = cData.EURO1992;   break;
        case BS6399_W_1997: BS6399 = cData.BS6399;     break;
        case CH_W_2002:     CH2002 = cData.CH2002;     break;
        case JPN_W_2000:    JPN2000 = cData.JPN2000;    break;
        case NBC_W_1995:    NBC1995 = cData.NBC1995;    break;
        case IS_W_1987:     IS1987 = cData.IS1987;     break;
        case TAIWAN_W_1986: TAIWAN86 = cData.TAIWAN86;   break;
        case JP_W_2004:     JP2004 = cData.JP2004;     break;
        case EURO_W_2005:   EURO2005 = cData.EURO2005;   break;
        case KBC_W_2009:    KBC2009 = cData.KBC2009;    break;
        case IBC_W_2009:    IBC2009 = cData.IBC2009;    break;
        case IBC_W_2012:    IBC2012 = cData.IBC2012;    break;
        case CH_W_2012:     CH2012 = cData.CH2012;     break;
        case CH_W_2021:     CH2019 = cData.CH2019;     break;
        case NSR_W_2010:    NSR2010 = cData.NSR2010;    break;
        case KBC_W_2016:    KBC2015 = cData.KBC2015;    break;
        case IS_W_875_2015: IS875_2015 = cData.IS875_2015; break;
        case KDS_W_2019:    KDS2019 = cData.KDS2019;    break;
        case KDS_W_2022:    KDS2021 = cData.KDS2021;    break;
		case DPT_W_2007:    DPT2007.Convert960(cData.DPT2007); break;
        case ASCE7_W_2016:  ASCE7_16 = cData.ASCE7_16;    break;
        case ASCE7_W_2022:  ASCE7_22 = cData.ASCE7_22;    break;
        case NSCP_W_2024:   NSCP2024 = cData.NSCP2024;    break;
        case RUS_W_2016:    RUS2016 = cData.RUS2016;    break;
        default: Initialize(nCode); break;
        }
    }
};

struct T_WIND_D
{
    int nCodeType;
    //  1 = KS.Arch.1992      2 = JAPAN.1987       3 = UBC.1997           4 = ANSI.1982         5 = KS.Arch.2000
    //  6 = IBC.2000          7 = EURO-1(1992)     8 = BS6399(1997)       9 = CH2002           10 = JPN2000
    // 11 = NBC1995          12 = IS1987          13 = TAIWAN86          14 = JP2004           15 = EURO2005
    // 16 = KBC(2009)        17 = IBC2009         18 = IBC2012           19 = CH2012           20 = NSR-10
    // 21 = KBC(2016)        22 = IS875(2015)     23 = CH2021            24 = KDS(41-10-15:2019)
    // 25 = KDS2021          26 = DPT2007         27 = USER              28 = ASCE7-16         29 = ASCE7-22
    // 30 = NSCP-2024        31 = RUS-2016
    // Code 추가시 wg_main의 ModelTextOut.cpp에 반영할 것
    CString Description;

    T_WIND_CODE CodeParam;
    double ScaleFactor_X;
    double ScaleFactor_Y;
    double ScaleFactor_R;
    CArray<T_WIND_ADDITION, T_WIND_ADDITION&>arAddition;
	CArray<T_WIND_USER, T_WIND_USER&> arUser;
	int nEccen_X;
	int nEccen_Y;

    void Initialize()
    {
        nCodeType= KDS_W_2022;
        Description.Empty();
        CodeParam.Initialize(nCodeType);

        ScaleFactor_X=0.;
        ScaleFactor_Y=0.;
        ScaleFactor_R=0.;
        arAddition.RemoveAll();
		arUser.RemoveAll();
		nEccen_X = 2;
		nEccen_Y = 2;
    }
    T_WIND_D()
    {
    }
    T_WIND_D(T_WIND_D& src)
    {
        *this = src;
    }
    T_WIND_D& operator=(T_WIND_D& src)
    {
        nCodeType=src.nCodeType;
        Description=src.Description;
        CodeParam    = src.CodeParam;
        ScaleFactor_X=src.ScaleFactor_X;
        ScaleFactor_Y=src.ScaleFactor_Y;
        ScaleFactor_R=src.ScaleFactor_R;
        arAddition.Copy(src.arAddition);
		arUser.Copy(src.arUser);
		nEccen_X = src.nEccen_X;
		nEccen_Y = src.nEccen_Y;
        return *this;
    }
    T_WIND_KBC2015* GetKBC2015()
    {
        switch ( nCodeType )
        {
        case KBC_W_2016: return &CodeParam.KBC2015;
        case KDS_W_2019: return &CodeParam.KDS2019;
        default:
            return NULL;
            break;
        }
    }
    T_WIND_ASCE7_16* GetASCE7Series()
	{
		switch (nCodeType)
		{
		case ASCE7_W_2016: return &CodeParam.ASCE7_16;
		case ASCE7_W_2022: return &CodeParam.ASCE7_22;
        case NSCP_W_2024:  return &CodeParam.NSCP2024;
        default: return NULL;			
		}
	}
    T_WIND_IS875_2015* GetIS875_2015()
	{
        switch (nCodeType)
        {
        case IS_W_875_2015: return &CodeParam.IS875_2015;
        default: return NULL;
        }
	}
};

struct T_WIND_D_CH
{
    int nCodeType;
    char Description[80];

    T_WIND_CODE CodeParam;

    double ScaleFactor_X;
    double ScaleFactor_Y;
    double ScaleFactor_R;
    CArray<T_WIND_ADDITION, T_WIND_ADDITION&> arAddition;
	CArray<T_WIND_USER, T_WIND_USER&> arUser;
	int nEccen_X;
	int nEccen_Y;

    void ConvertToString(T_WIND_D& rData)
    {
        rData.nCodeType=nCodeType;
        ConvertCharStr(Description, rData.Description, sizeof(Description));
        rData.CodeParam = CodeParam;

        rData.ScaleFactor_X=ScaleFactor_X;
        rData.ScaleFactor_Y=ScaleFactor_Y;
        rData.ScaleFactor_R=ScaleFactor_R;
        rData.arAddition.Copy(arAddition);
		rData.arUser.Copy(arUser);
		rData.nEccen_X = nEccen_X;
		rData.nEccen_Y = nEccen_Y;
    }
    void ConvertToChar(T_WIND_D& rData)
    {
        nCodeType=rData.nCodeType;
        ConvertStrChar(rData.Description, Description, sizeof(Description));
        CodeParam = rData.CodeParam;

        ScaleFactor_X=rData.ScaleFactor_X;
        ScaleFactor_Y=rData.ScaleFactor_Y;
        ScaleFactor_R=rData.ScaleFactor_R;
        arAddition.Copy(rData.arAddition);
		arUser.Copy(rData.arUser);
		nEccen_X = rData.nEccen_X;
		nEccen_Y = rData.nEccen_Y;
    }
};

struct T_WIND_UDRD_D
{
    T_WIND_K key;
    T_WIND_D data;
};

struct T_WIND_UDRD_D_CH
{
    T_WIND_K key;
    T_WIND_D_CH data;
};

struct T_WIND_UDRD_D_CH_RW_412
{
    T_WIND_K key;
    int nCodeType; // 1=KS.Arch.1992  2=JAPAN.1987  3=UBC.1997  4=ANSI.1982
    char Description[80];
    T_WIND_KS1992      KS1992;
    T_WIND_JP1987      JP1987;
    T_WIND_UBC1997_570 UBC1997;
    T_WIND_ANSI1982    ANSI1982;
    double ScaleFactor_X;
    double ScaleFactor_Y;
    int nCountAddition;
};

struct T_WIND_UDRD_D_CH_RW_423
{
    T_WIND_K key;
    int nCodeType; // 1=KS.Arch.1992  2=JAPAN.1987  3=UBC.1997  4=ANSI.1982  5=KS.Arch.2000
    char Description[80];
    T_WIND_KS1992      KS1992;
    T_WIND_JP1987      JP1987;
    T_WIND_UBC1997_570 UBC1997;
    T_WIND_ANSI1982    ANSI1982;
    T_WIND_KS2000_570  KS2000;
    double ScaleFactor_X;
    double ScaleFactor_Y;
    int nCountAddition;
};

struct T_WIND_UDRD_D_CH_RW_520
{
    T_WIND_K key;
    int nCodeType; // 1=KS.Arch.1992  2=JAPAN.1987  3=UBC.1997  4=ANSI.1982  5=KS.Arch.2000
    char Description[80];
    T_WIND_KS1992      KS1992;
    T_WIND_JP1987      JP1987;
    T_WIND_UBC1997_570 UBC1997;
    T_WIND_ANSI1982    ANSI1982;
    T_WIND_KS2000_570  KS2000;
    T_WIND_IBC2000_570 IBC2000;
    double ScaleFactor_X;
    double ScaleFactor_Y;
    int nCountAddition;
};

struct T_WIND_UDRD_D_CH_RW_550
{
    T_WIND_K key;
    int nCodeType; // 1=KS.Arch.1992  2=JAPAN.1987  3=UBC.1997  4=ANSI.1982  5=KS.Arch.2000  6=IBC2000
                   // 7=EURO-1(1992)  8=BS6399(1997) 9=CH2002   10=JPN2000
    char Description[80];
    T_WIND_KS1992       KS1992;
    T_WIND_JP1987       JP1987;
    T_WIND_UBC1997_570  UBC1997;
    T_WIND_ANSI1982     ANSI1982;
    T_WIND_KS2000_570   KS2000;
    T_WIND_IBC2000_570  IBC2000;
    T_WIND_EURO1992_580 EURO1992;
    T_WIND_BS6399       BS6399;
    T_WIND_CH2002_590   CH2002;
    T_WIND_JPN2000      JPN2000;
    double ScaleFactor_X;
    double ScaleFactor_Y;
    int nCountAddition;
};

struct T_WIND_UDRD_D_CH_RW_570
{
    T_WIND_K key;
    int nCodeType; // 1=KS.Arch.1992  2=JAPAN.1987  3=UBC.1997  4=ANSI.1982  5=KS.Arch.2000  6=IBC2000
                   // 7=EURO-1(1992)  8=BS6399(1997) 9=CH2002   10=JPN2000  11=NBC1995
    char Description[80];
    T_WIND_KS1992       KS1992;
    T_WIND_JP1987       JP1987;
    T_WIND_UBC1997_570  UBC1997;
    T_WIND_ANSI1982     ANSI1982;
    T_WIND_KS2000_570   KS2000;
    T_WIND_IBC2000_570  IBC2000;
    T_WIND_EURO1992_580 EURO1992;
    T_WIND_BS6399       BS6399;
    T_WIND_CH2002_590   CH2002;
    T_WIND_JPN2000      JPN2000;
    T_WIND_NBC1995_570  NBC1995;
    double ScaleFactor_X;
    double ScaleFactor_Y;
    int nCountAddition;
};

struct T_WIND_UDRD_D_CH_RW_580
{
    T_WIND_K key;
    int nCodeType; // 1=KS.Arch.1992  2=JAPAN.1987  3=UBC.1997  4=ANSI.1982  5=KS.Arch.2000  6=IBC2000
                   // 7=EURO-1(1992)  8=BS6399(1997) 9=CH2002   10=JPN2000  11=NBC1995
    char Description[80];
    T_WIND_KS1992       KS1992;
    T_WIND_JP1987       JP1987;
    T_WIND_UBC1997      UBC1997;
    T_WIND_ANSI1982     ANSI1982;
    T_WIND_KS2000_640   KS2000;
    T_WIND_IBC2000_790      IBC2000;
    T_WIND_EURO1992_580 EURO1992; // added by KYM
    T_WIND_BS6399       BS6399;   // added by KYM
    T_WIND_CH2002_590   CH2002;   // added by KYM
    T_WIND_JPN2000      JPN2000;  // added by KYM
    T_WIND_NBC1995      NBC1995;  // added by KYM
    double ScaleFactor_X;
    double ScaleFactor_Y;
    int nCountAddition;
};

struct T_WIND_UDRD_D_CH_RW_590
{
    T_WIND_K key;
    int nCodeType; // 1=KS.Arch.1992  2=JAPAN.1987  3=UBC.1997  4=ANSI.1982  5=KS.Arch.2000  6=IBC2000
                   // 7=EURO-1(1992)  8=BS6399(1997) 9=CH2002   10=JPN2000  11=NBC1995
    char Description[80];
    T_WIND_KS1992     KS1992;
    T_WIND_JP1987     JP1987;
    T_WIND_UBC1997    UBC1997;
    T_WIND_ANSI1982   ANSI1982;
    T_WIND_KS2000_640 KS2000;
    T_WIND_IBC2000_790    IBC2000;
    T_WIND_EURO1992   EURO1992; // added by KYM
    T_WIND_BS6399     BS6399;   // added by KYM
    T_WIND_CH2002_590 CH2002;   // added by KYM
    T_WIND_JPN2000    JPN2000;  // added by KYM
    T_WIND_NBC1995    NBC1995;  // added by KYM
    double ScaleFactor_X;
    double ScaleFactor_Y;
    int nCountAddition;
};

struct T_WIND_UDRD_D_CH_RW_600
{
    T_WIND_K key;
    int nCodeType; // 1=KS.Arch.1992  2=JAPAN.1987  3=UBC.1997  4=ANSI.1982  5=KS.Arch.2000  6=IBC2000
                   // 7=EURO-1(1992)  8=BS6399(1997) 9=CH2002   10=JPN2000  11=NBC1995
    char Description[80];
    T_WIND_KS1992     KS1992;
    T_WIND_JP1987     JP1987;
    T_WIND_UBC1997    UBC1997;
    T_WIND_ANSI1982   ANSI1982;
    T_WIND_KS2000_640 KS2000;
    T_WIND_IBC2000_790    IBC2000;
    T_WIND_EURO1992   EURO1992; // added by KYM
    T_WIND_BS6399     BS6399;   // added by KYM
    T_WIND_CH2002_811     CH2002;   // added by KYM
    T_WIND_JPN2000    JPN2000;  // added by KYM
    T_WIND_NBC1995    NBC1995;  // added by KYM
    T_WIND_IS1987     IS1987;
    double ScaleFactor_X;
    double ScaleFactor_Y;
    int nCountAddition;
};

struct T_WIND_UDRD_D_CH_RW_640
{
    T_WIND_K key;
    int nCodeType; // 1=KS.Arch.1992  2=JAPAN.1987  3=UBC.1997  4=ANSI.1982  5=KS.Arch.2000  6=IBC2000
                   // 7=EURO-1(1992)  8=BS6399(1997) 9=CH2002   10=JPN2000  11=NBC1995
    char Description[80];
    T_WIND_KS1992     KS1992;
    T_WIND_JP1987     JP1987;
    T_WIND_UBC1997    UBC1997;
    T_WIND_ANSI1982   ANSI1982;
    T_WIND_KS2000_640 KS2000;
    T_WIND_IBC2000_790    IBC2000;
    T_WIND_EURO1992   EURO1992; // added by KYM
    T_WIND_BS6399     BS6399;   // added by KYM
    T_WIND_CH2002_811     CH2002;   // added by KYM
    T_WIND_JPN2000    JPN2000;  // added by KYM
    T_WIND_NBC1995    NBC1995;  // added by KYM
    T_WIND_IS1987     IS1987;
    T_WIND_TAIWAN86   TAIWAN86;
    double ScaleFactor_X;
    double ScaleFactor_Y;
    int nCountAddition;
};

struct T_WIND_UDRD_D_CH_RW_671
{
    T_WIND_K key;
    int nCodeType; // 1=KS.Arch.1992  2=JAPAN.1987  3=UBC.1997  4=ANSI.1982  5=KS.Arch.2000  6=IBC2000
                   // 7=EURO-1(1992)  8=BS6399(1997) 9=CH2002   10=JPN2000  11=NBC1995
    char Description[80];
    T_WIND_KS1992   KS1992;
    T_WIND_JP1987   JP1987;
    T_WIND_UBC1997  UBC1997;
    T_WIND_ANSI1982 ANSI1982;
    T_WIND_KS2000   KS2000;
    T_WIND_IBC2000_790  IBC2000;
    T_WIND_EURO1992 EURO1992; // added by KYM
    T_WIND_BS6399   BS6399;   // added by KYM
    T_WIND_CH2002_811   CH2002;   // added by KYM
    T_WIND_JPN2000  JPN2000;  // added by KYM
    T_WIND_NBC1995  NBC1995;  // added by KYM
    T_WIND_IS1987   IS1987;
    T_WIND_TAIWAN86 TAIWAN86;
    double ScaleFactor_X;
    double ScaleFactor_Y;
    int nCountAddition;
};

struct T_WIND_UDRD_D_CH_RW_712
{
    T_WIND_K key;
    int nCodeType; // 1=KS.Arch.1992  2=JAPAN.1987  3=UBC.1997  4=ANSI.1982  5=KS.Arch.2000  6=IBC2000
                   // 7=EURO-1(1992)  8=BS6399(1997) 9=CH2002   10=JPN2000  11=NBC1995
    char Description[80];
    T_WIND_KS1992   KS1992;
    T_WIND_JP1987   JP1987;
    T_WIND_UBC1997  UBC1997;
    T_WIND_ANSI1982 ANSI1982;
    T_WIND_KS2000   KS2000;
    T_WIND_IBC2000_790  IBC2000;
    T_WIND_EURO1992 EURO1992; // added by KYM
    T_WIND_BS6399   BS6399;   // added by KYM
    T_WIND_CH2002_811   CH2002;   // added by KYM
    T_WIND_JPN2000  JPN2000;  // added by KYM
    T_WIND_NBC1995  NBC1995;  // added by KYM
    T_WIND_IS1987   IS1987;
    T_WIND_TAIWAN86 TAIWAN86;
    T_WIND_JP2004   JP2004;
    double ScaleFactor_X;
    double ScaleFactor_Y;
    double ScaleFactor_R;
    int nCountAddition;
};

struct T_WIND_UDRD_D_CH_RW_750
{
    T_WIND_K key;
    int nCodeType; // 1=KS.Arch.1992  2=JAPAN.1987  3=UBC.1997  4=ANSI.1982  5=KS.Arch.2000  6=IBC2000
                   // 7=EURO-1(1992)  8=BS6399(1997) 9=CH2002   10=JPN2000  11=NBC1995
    char Description[80];
    T_WIND_KS1992   KS1992;
    T_WIND_JP1987   JP1987;
    T_WIND_UBC1997  UBC1997;
    T_WIND_ANSI1982 ANSI1982;
    T_WIND_KS2000   KS2000;
    T_WIND_IBC2000_790  IBC2000;
    T_WIND_EURO1992 EURO1992; // added by KYM
    T_WIND_BS6399   BS6399;   // added by KYM
    T_WIND_CH2002_811   CH2002;   // added by KYM
    T_WIND_JPN2000  JPN2000;  // added by KYM
    T_WIND_NBC1995  NBC1995;  // added by KYM
    T_WIND_IS1987   IS1987;
    T_WIND_TAIWAN86 TAIWAN86;
    T_WIND_JP2004   JP2004;
    T_WIND_EURO2005_940 EURO2005;
    double ScaleFactor_X;
    double ScaleFactor_Y;
    double ScaleFactor_R;
    int nCountAddition;
};

struct T_WIND_UDRD_D_CH_RW_790
{
    T_WIND_K key;
    int nCodeType; // 1=KS.Arch.1992  2=JAPAN.1987  3=UBC.1997  4=ANSI.1982  5=KS.Arch.2000  6=IBC2000
                   // 7=EURO-1(1992)  8=BS6399(1997) 9=CH2002   10=JPN2000  11=NBC1995      12=KBC2009
    char Description[80];
    T_WIND_KS1992       KS1992;
    T_WIND_JP1987       JP1987;
    T_WIND_UBC1997      UBC1997;
    T_WIND_ANSI1982     ANSI1982;
    T_WIND_KS2000       KS2000;
    T_WIND_IBC2000_790  IBC2000;
    T_WIND_EURO1992     EURO1992; // added by KYM
    T_WIND_BS6399       BS6399;   // added by KYM
    T_WIND_CH2002_811   CH2002;   // added by KYM
    T_WIND_JPN2000      JPN2000;  // added by KYM
    T_WIND_NBC1995      NBC1995;  // added by KYM
    T_WIND_IS1987       IS1987;
    T_WIND_TAIWAN86     TAIWAN86;
    T_WIND_JP2004       JP2004;
    T_WIND_EURO2005_940     EURO2005;
    T_WIND_KBC2009_790  KBC2009;
    double ScaleFactor_X;
    double ScaleFactor_Y;
    double ScaleFactor_R;
    int nCountAddition;
};

struct T_WIND_UDRD_D_CH_RW_796
{
    T_WIND_K key;
    int nCodeType; // 1=KS.Arch.1992  2=JAPAN.1987  3=UBC.1997  4=ANSI.1982  5=KS.Arch.2000  6=IBC2000
                   // 7=EURO-1(1992)  8=BS6399(1997) 9=CH2002   10=JPN2000  11=NBC1995      12=KBC2009
    char Description[80];
    T_WIND_KS1992   KS1992;
    T_WIND_JP1987   JP1987;
    T_WIND_UBC1997  UBC1997;
    T_WIND_ANSI1982 ANSI1982;
    T_WIND_KS2000   KS2000;
    T_WIND_IBC2000  IBC2000;
    T_WIND_EURO1992 EURO1992; // added by KYM
    T_WIND_BS6399   BS6399;   // added by KYM
    T_WIND_CH2002_811   CH2002;   // added by KYM
    T_WIND_JPN2000  JPN2000;  // added by KYM
    T_WIND_NBC1995  NBC1995;  // added by KYM
    T_WIND_IS1987   IS1987;
    T_WIND_TAIWAN86 TAIWAN86;
    T_WIND_JP2004   JP2004;
    T_WIND_EURO2005_940 EURO2005;
    T_WIND_KBC2009  KBC2009;
    double ScaleFactor_X;
    double ScaleFactor_Y;
    double ScaleFactor_R;
    int nCountAddition;
};

struct T_WIND_UDRD_D_CH_RW_811
{
    T_WIND_K key;
    int nCodeType; // 1=KS.Arch.1992  2=JAPAN.1987  3=UBC.1997  4=ANSI.1982  5=KS.Arch.2000  6=IBC2000
                   // 7=EURO-1(1992)  8=BS6399(1997) 9=CH2002   10=JPN2000  11=NBC1995      12=KBC2009
    char Description[80];
    T_WIND_KS1992   KS1992;
    T_WIND_JP1987   JP1987;
    T_WIND_UBC1997  UBC1997;
    T_WIND_ANSI1982 ANSI1982;
    T_WIND_KS2000   KS2000;
    T_WIND_IBC2000  IBC2000;
    T_WIND_EURO1992 EURO1992; // added by KYM
    T_WIND_BS6399   BS6399;   // added by KYM
    T_WIND_CH2002_811   CH2002;   // added by KYM
    T_WIND_JPN2000  JPN2000;  // added by KYM
    T_WIND_NBC1995  NBC1995;  // added by KYM
    T_WIND_IS1987   IS1987;
    T_WIND_TAIWAN86 TAIWAN86;
    T_WIND_JP2004   JP2004;
    T_WIND_EURO2005_940 EURO2005;
    T_WIND_KBC2009  KBC2009;
    T_WIND_IBC2012  IBC2009;
    T_WIND_IBC2012  IBC2012;
    double ScaleFactor_X;
    double ScaleFactor_Y;
    double ScaleFactor_R;
    int nCountAddition;
};

struct T_WIND_UDRD_D_CH_RW_815
{
    T_WIND_K key;
    int nCodeType; // 1=KS.Arch.1992  2=JAPAN.1987  3=UBC.1997  4=ANSI.1982  5=KS.Arch.2000  6=IBC2000
                   // 7=EURO-1(1992)  8=BS6399(1997) 9=CH2002   10=JPN2000  11=NBC1995      12=KBC2009
    char Description[80];
    T_WIND_KS1992   KS1992;
    T_WIND_JP1987   JP1987;
    T_WIND_UBC1997  UBC1997;
    T_WIND_ANSI1982 ANSI1982;
    T_WIND_KS2000   KS2000;
    T_WIND_IBC2000  IBC2000;
    T_WIND_EURO1992 EURO1992; // added by KYM
    T_WIND_BS6399   BS6399;   // added by KYM
    T_WIND_CH2002   CH2002;   // added by KYM
    T_WIND_JPN2000  JPN2000;  // added by KYM
    T_WIND_NBC1995  NBC1995;  // added by KYM
    T_WIND_IS1987   IS1987;
    T_WIND_TAIWAN86 TAIWAN86;
    T_WIND_JP2004   JP2004;
    T_WIND_EURO2005_940 EURO2005;
    T_WIND_KBC2009  KBC2009;
    T_WIND_IBC2012  IBC2009;
    T_WIND_IBC2012  IBC2012;
    T_WIND_CH2012_815   CH2012;   // v811 added. added by maxiao
    double ScaleFactor_X;
    double ScaleFactor_Y;
    double ScaleFactor_R;
    int nCountAddition;
};

struct T_WIND_UDRD_D_CH_RW_845
{
    T_WIND_K key;
    int nCodeType; // 1=KS.Arch.1992  2=JAPAN.1987  3=UBC.1997  4=ANSI.1982  5=KS.Arch.2000  6=IBC2000
                   // 7=EURO-1(1992)  8=BS6399(1997) 9=CH2002   10=JPN2000  11=NBC1995      12=KBC2009
    char Description[80];
    T_WIND_KS1992   KS1992;
    T_WIND_JP1987   JP1987;
    T_WIND_UBC1997  UBC1997;
    T_WIND_ANSI1982 ANSI1982;
    T_WIND_KS2000   KS2000;
    T_WIND_IBC2000  IBC2000;
    T_WIND_EURO1992 EURO1992; // added by KYM
    T_WIND_BS6399   BS6399;   // added by KYM
    T_WIND_CH2002   CH2002;   // added by KYM
    T_WIND_JPN2000  JPN2000;  // added by KYM
    T_WIND_NBC1995  NBC1995;  // added by KYM
    T_WIND_IS1987   IS1987;
    T_WIND_TAIWAN86 TAIWAN86;
    T_WIND_JP2004   JP2004;
    T_WIND_EURO2005_940 EURO2005;
    T_WIND_KBC2009  KBC2009;
    T_WIND_IBC2012  IBC2009;
    T_WIND_IBC2012  IBC2012;
    T_WIND_CH2012_915   CH2012;   // v811 added. added by maxiao
    double ScaleFactor_X;
    double ScaleFactor_Y;
    double ScaleFactor_R;
    int nCountAddition;
};

struct T_WIND_UDRD_D_CH_RW_850
{
    T_WIND_K key;
    int nCodeType; // 1=KS.Arch.1992   2=JAPAN.1987    3=UBC.1997  4=ANSI.1982   5=KS.Arch.2000  6=IBC2000
                   // 7=EURO-1(1992)   8=BS6399(1997)  9=CH2002   10=JPN2000    11=NBC1995      12=KBC2009
                   // 13=TAIWAN86     14=JP2004       15=EURO2005 16=KBC(2009)  17=IBC2009      18=IBC2012
                   // 19=CH2012       20=NSR-10
    char Description[80];
    T_WIND_KS1992   KS1992;
    T_WIND_JP1987   JP1987;
    T_WIND_UBC1997  UBC1997;
    T_WIND_ANSI1982 ANSI1982;
    T_WIND_KS2000   KS2000;
    T_WIND_IBC2000  IBC2000;
    T_WIND_EURO1992 EURO1992;
    T_WIND_BS6399   BS6399;
    T_WIND_CH2002   CH2002;
    T_WIND_JPN2000  JPN2000;
    T_WIND_NBC1995  NBC1995;
    T_WIND_IS1987   IS1987;
    T_WIND_TAIWAN86 TAIWAN86;
    T_WIND_JP2004   JP2004;
    T_WIND_EURO2005_940 EURO2005;
    T_WIND_KBC2009  KBC2009;
    T_WIND_IBC2012  IBC2009;
    T_WIND_IBC2012  IBC2012;
    T_WIND_CH2012_915   CH2012;   // v811 added. added by maxiao
    T_WIND_NSR2010  NSR2010;
    double ScaleFactor_X;
    double ScaleFactor_Y;
    double ScaleFactor_R;
    int nCountAddition;
};

struct T_WIND_UDRD_D_CH_RW_870
{
    T_WIND_K key;
    int nCodeType; // 1=KS.Arch.1992  2=JAPAN.1987   3=UBC.1997  4=ANSI.1982   5=KS.Arch.2000  6=IBC2000
                   // 7=EURO-1(1992)  8=BS6399(1997) 9=CH2002   10=JPN2000    11=NBC1995      12=KBC2009
                   // 13=TAIWAN86    14=JP2004      15=EURO2005 16=KBC(2009)  17=IBC2009      18=IBC2012
                   // 19=CH2012      20=NSR-10      21=KBC2015
    char Description[80];
    T_WIND_KS1992   KS1992;
    T_WIND_JP1987   JP1987;
    T_WIND_UBC1997  UBC1997;
    T_WIND_ANSI1982 ANSI1982;
    T_WIND_KS2000   KS2000;
    T_WIND_IBC2000  IBC2000;
    T_WIND_EURO1992 EURO1992;
    T_WIND_BS6399   BS6399;
    T_WIND_CH2002   CH2002;
    T_WIND_JPN2000  JPN2000;
    T_WIND_NBC1995  NBC1995;
    T_WIND_IS1987   IS1987;
    T_WIND_TAIWAN86 TAIWAN86;
    T_WIND_JP2004   JP2004;
    T_WIND_EURO2005_940 EURO2005;
    T_WIND_KBC2009  KBC2009;
    T_WIND_IBC2012  IBC2009;
    T_WIND_IBC2012  IBC2012;
    T_WIND_CH2012_915   CH2012;   // v811 added. added by maxiao
    T_WIND_NSR2010  NSR2010;
    T_WIND_KBC2015  KBC2015;
    double ScaleFactor_X;
    double ScaleFactor_Y;
    double ScaleFactor_R;
    int nCountAddition;
};

struct T_WIND_UDRD_D_CH_RW_890
{
    T_WIND_K key;
    int nCodeType; // 1=KS.Arch.1992  2=JAPAN.1987    3=UBC.1997  4=ANSI.1982     5=KS.Arch.2000  6=IBC2000
                   // 7=EURO-1(1992)  8=BS6399(1997)  9=CH2002   10=JPN2000      11=NBC1995      12=KBC2009
                   // 13=TAIWAN86    14=JP2004       15=EURO2005 16=KBC(2009)    17=IBC2009      18=IBC2012
                   // 19=CH2012      20=NSR-10       21=KBC2015  22=IS875(2015)
    char Description[80];
    T_WIND_KS1992   KS1992;
    T_WIND_JP1987   JP1987;
    T_WIND_UBC1997  UBC1997;
    T_WIND_ANSI1982 ANSI1982;
    T_WIND_KS2000   KS2000;
    T_WIND_IBC2000  IBC2000;
    T_WIND_EURO1992 EURO1992;
    T_WIND_BS6399   BS6399;
    T_WIND_CH2002   CH2002;
    T_WIND_JPN2000  JPN2000;
    T_WIND_NBC1995  NBC1995;
    T_WIND_IS1987   IS1987;
    T_WIND_TAIWAN86 TAIWAN86;
    T_WIND_JP2004   JP2004;
    T_WIND_EURO2005_940 EURO2005;
    T_WIND_KBC2009  KBC2009;
    T_WIND_IBC2012  IBC2009;
    T_WIND_IBC2012  IBC2012;
    T_WIND_CH2012_915   CH2012;   // v811 added. added by maxiao
    T_WIND_NSR2010  NSR2010;
    T_WIND_KBC2015  KBC2015;
    T_WIND_IS875_2015_890 IS875_2015;
    double ScaleFactor_X;
    double ScaleFactor_Y;
    double ScaleFactor_R;
    int nCountAddition;
};

struct T_WIND_UDRD_D_CH_RW_895
{
    T_WIND_K key;
    int nCodeType;
    char Description[80];
    T_WIND_KS1992   KS1992;
    T_WIND_JP1987   JP1987;
    T_WIND_UBC1997  UBC1997;
    T_WIND_ANSI1982 ANSI1982;
    T_WIND_KS2000   KS2000;
    T_WIND_IBC2000  IBC2000;
    T_WIND_EURO1992 EURO1992;
    T_WIND_BS6399   BS6399;
    T_WIND_CH2002   CH2002;
    T_WIND_JPN2000  JPN2000;
    T_WIND_NBC1995  NBC1995;
    T_WIND_IS1987   IS1987;
    T_WIND_TAIWAN86 TAIWAN86;
    T_WIND_JP2004   JP2004;
    T_WIND_EURO2005_940 EURO2005;
    T_WIND_KBC2009  KBC2009;
    T_WIND_IBC2012  IBC2009;
    T_WIND_IBC2012  IBC2012;
    T_WIND_CH2012_915   CH2012;   // v811 added. added by maxiao
    T_WIND_CH2019_915   CH2019;
    T_WIND_NSR2010  NSR2010;
    T_WIND_KBC2015  KBC2015;
    T_WIND_IS875_2015_940 IS875_2015;
    double ScaleFactor_X;
    double ScaleFactor_Y;
    double ScaleFactor_R;
    int nCountAddition;
};

struct T_WIND_UDRD_D_CH_RW_900
{
    T_WIND_K key;
    int nCodeType;  // 1=KS.Arch.1992  2=JAPAN.1987    3=UBC.1997  4=ANSI.1982   5=KS.Arch.2000  6=IBC2000
                    // 7=EURO-1(1992)  8=BS6399(1997)  9=CH2002   10=JPN2000    11=NBC1995      12=KBC2009
                    // 13=TAIWAN86     14=JP2004      15=EURO2005 16=KBC(2009)  17=IBC2009      18=IBC2012
                    // 19=CH2012       20=NSR-10      21=KBC2015  22=IS875(2015)
    char Description[80];
    T_WIND_KS1992   KS1992;
    T_WIND_JP1987   JP1987;
    T_WIND_UBC1997  UBC1997;
    T_WIND_ANSI1982 ANSI1982;
    T_WIND_KS2000   KS2000;
    T_WIND_IBC2000  IBC2000;
    T_WIND_EURO1992 EURO1992;
    T_WIND_BS6399   BS6399;
    T_WIND_CH2002   CH2002;
    T_WIND_JPN2000  JPN2000;
    T_WIND_NBC1995  NBC1995;
    T_WIND_IS1987   IS1987;
    T_WIND_TAIWAN86 TAIWAN86;
    T_WIND_JP2004   JP2004;
    T_WIND_EURO2005_940 EURO2005;
    T_WIND_KBC2009  KBC2009;
    T_WIND_IBC2012  IBC2009;
    T_WIND_IBC2012  IBC2012;
    T_WIND_CH2012_915   CH2012;   // v811 added. added by maxiao
    T_WIND_CH2019_915   CH2019;
    T_WIND_NSR2010  NSR2010;
    T_WIND_KBC2015  KBC2015;
    T_WIND_IS875_2015_940 IS875_2015;
    T_WIND_KBC2015  KDS2019;
    double ScaleFactor_X;
    double ScaleFactor_Y;
    double ScaleFactor_R;
    int nCountAddition;
};

struct T_WIND_UDRD_D_CH_RW_915
{
    T_WIND_K key;
    int nCodeType;
    //  1=KS.Arch.1992  2=JAPAN.1987   3=UBC.1997   4=ANSI.1982      5=KS.Arch.2000  6=IBC2000
    //  7=EURO-1(1992)  8=BS6399(1997) 9=CH2002    10=JPN2000       11=NBC1995      12=KBC2009
    // 13=TAIWAN86     14=JP2004      15=EURO2005  16=KBC(2009)     17=IBC2009      18=IBC2012
    // 19=CH2012       20=NSR-10      21=KBC2015   22=IS875(2015)   23=CH2019
    // 24=KDS2019      25=KDS2021
    char Description[80];
    T_WIND_CODE_915 CodeParam;  //7.1.0 추가

    double ScaleFactor_X;
    double ScaleFactor_Y;
    double ScaleFactor_R;
    int nCountAddition;
};

struct T_WIND_UDRD_D_CH_RW_930
{
	T_WIND_K key;
	int nCodeType;
	//  1=KS.Arch.1992  2=JAPAN.1987   3=UBC.1997   4=ANSI.1982      5=KS.Arch.2000  6=IBC2000
	//  7=EURO-1(1992)  8=BS6399(1997) 9=CH2002    10=JPN2000       11=NBC1995      12=KBC2009
	// 13=TAIWAN86     14=JP2004      15=EURO2005  16=KBC(2009)     17=IBC2009      18=IBC2012
	// 19=CH2012       20=NSR-10      21=KBC2015   22=IS875(2015)   23=CH2019
	// 24=KDS2019      25=KDS2021     26=DPT2007
	char Description[80];
	T_WIND_CODE_960 CodeParam;

	double ScaleFactor_X;
	double ScaleFactor_Y;
	double ScaleFactor_R;
	int nCountAddition;
};

struct T_WIND_UDRD_D_CH_RW_960
{
    T_WIND_K key;
    int nCodeType;
    //  1 = KS.Arch.1992      2 = JAPAN.1987       3 = UBC.1997           4 = ANSI.1982         5 = KS.Arch.2000
    //  6 = IBC.2000          7 = EURO-1(1992)     8 = BS6399(1997)       9 = CH2002           10 = JPN2000
    // 11 = NBC1995          12 = IS1987          13 = TAIWAN86          14 = JP2004           15 = EURO2005
    // 16 = KBC(2009)        17 = IBC2009         18 = IBC2012           19 = CH2012           20 = NSR-10
    // 21 = KBC(2016)        22 = IS875(2015)     23 = CH2021            24 = KDS(41-10-15:2019)
    // 25 = KDS2021          26 = DPT2007         27 = USER              28 = ASCE7-16         29 = ASCE7-22
    // 30 = NSCP-2024        31 = RUS-2016
    char Description[80];
    T_WIND_CODE_960 CodeParam;

    double ScaleFactor_X;
    double ScaleFactor_Y;
    double ScaleFactor_R;
    int nCountAddition;
    int nCountUser;
    int nEccen_X;
    int nEccen_Y;
};

struct T_WIND_UDRD_D_CH_RW
{
    T_WIND_K key;
    int nCodeType;
    //  1 = KS.Arch.1992      2 = JAPAN.1987       3 = UBC.1997           4 = ANSI.1982         5 = KS.Arch.2000
    //  6 = IBC.2000          7 = EURO-1(1992)     8 = BS6399(1997)       9 = CH2002           10 = JPN2000
    // 11 = NBC1995          12 = IS1987          13 = TAIWAN86          14 = JP2004           15 = EURO2005
    // 16 = KBC(2009)        17 = IBC2009         18 = IBC2012           19 = CH2012           20 = NSR-10
    // 21 = KBC(2016)        22 = IS875(2015)     23 = CH2021            24 = KDS(41-10-15:2019)
    // 25 = KDS2021          26 = DPT2007         27 = USER              28 = ASCE7-16         29 = ASCE7-22
    // 30 = NSCP-2024        31 = RUS-2016

    char Description[80];
    T_WIND_CODE CodeParam;  //7.1.0 추가

    double ScaleFactor_X;
    double ScaleFactor_Y;
    double ScaleFactor_R;
    int nCountAddition;
	int nCountUser;
	int nEccen_X;
	int nEccen_Y;
    void Initialize()
    {
        key= 0;
        nCodeType= KDS_W_2022;
        memset(Description, ' ', 80);
        CodeParam.Initialize(nCodeType);

        ScaleFactor_X=0.;
        ScaleFactor_Y=0.;
        ScaleFactor_R=0.;
        nCountAddition=0;
		nCountUser=0;
		nEccen_X = 0;
		nEccen_Y = 0;
    }
    void GetWind(T_WIND_K& rKey, T_WIND_D_CH& rData)
    {
        rKey=key;
        rData.nCodeType=nCodeType;
        memcpy(rData.Description, Description, sizeof(Description));
        rData.CodeParam = CodeParam;

        rData.ScaleFactor_X=ScaleFactor_X;
        rData.ScaleFactor_Y=ScaleFactor_Y;
        rData.ScaleFactor_R=ScaleFactor_R;
		rData.nEccen_X = nEccen_X;
		rData.nEccen_Y = nEccen_Y;
    }
    void SetWind(const T_WIND_K Key, const T_WIND_D_CH& rData)
    {
        key=Key;
        nCodeType=rData.nCodeType;
        memcpy(Description, rData.Description, sizeof(Description));
        CodeParam = rData.CodeParam;

        ScaleFactor_X=rData.ScaleFactor_X;
        ScaleFactor_Y=rData.ScaleFactor_Y;
        ScaleFactor_R=rData.ScaleFactor_R;
        nCountAddition=( int ) rData.arAddition.GetSize();
		nCountUser = (int)rData.arUser.GetSize();
		nEccen_X = rData.nEccen_X;
		nEccen_Y = rData.nEccen_Y;
    }
    void Convert412(T_WIND_UDRD_D_CH_RW_412& rData)
    {
        Initialize();
        key = rData.key;
        nCodeType=rData.nCodeType;

        memcpy(Description, rData.Description, sizeof(Description));
        CodeParam.Initialize(nCodeType);

        switch ( rData.nCodeType )
        {
        case KS_W_92: CodeParam.KS1992 = rData.KS1992; break;
        case JP_W_87: CodeParam.JP1987 = rData.JP1987; break;
        case UBC_W_97: CodeParam.UBC1997.Convert570(rData.UBC1997); break;
        case ANSI_W_82: CodeParam.ANSI1982 = rData.ANSI1982; break;
        default: break;
        }
        ScaleFactor_X=rData.ScaleFactor_X;
        ScaleFactor_Y=rData.ScaleFactor_Y;
        ScaleFactor_R=0.;
        nCountAddition=rData.nCountAddition;
    }
    void Convert423(T_WIND_UDRD_D_CH_RW_423& rData)
    {
        Initialize();
        key = rData.key;
        nCodeType = rData.nCodeType;

        memcpy(Description, rData.Description, sizeof(Description));
        CodeParam.Initialize(nCodeType);

        switch ( rData.nCodeType )
        {
        case KS_W_92: CodeParam.KS1992 = rData.KS1992; break;
        case JP_W_87: CodeParam.JP1987 = rData.JP1987; break;
        case UBC_W_97: CodeParam.UBC1997.Convert570(rData.UBC1997); break;
        case ANSI_W_82: CodeParam.ANSI1982 = rData.ANSI1982; break;
        case KS_W_2000: CodeParam.KS2000.Convert570(rData.KS2000); break;
        default: break;
        }

        ScaleFactor_X=rData.ScaleFactor_X;
        ScaleFactor_Y=rData.ScaleFactor_Y;
        ScaleFactor_R=0.;
        nCountAddition=rData.nCountAddition;
    }
    void Convert520(T_WIND_UDRD_D_CH_RW_520& rData)   // this function is added by KYM
    {
        Initialize();
        key = rData.key;
        nCodeType = rData.nCodeType;
        memcpy(Description, rData.Description, sizeof(Description));
        CodeParam.Initialize(nCodeType);

        switch ( rData.nCodeType )
        {
        case KS_W_92: CodeParam.KS1992 = rData.KS1992; break;
        case JP_W_87: CodeParam.JP1987 = rData.JP1987; break;
        case UBC_W_97: CodeParam.UBC1997.Convert570(rData.UBC1997); break;
        case ANSI_W_82: CodeParam.ANSI1982 = rData.ANSI1982; break;
        case KS_W_2000: CodeParam.KS2000.Convert570(rData.KS2000); break;
        case IBC_W_2000: CodeParam.IBC2000.Convert570(rData.IBC2000); break;
        default: break;
        }

        ScaleFactor_X=rData.ScaleFactor_X;
        ScaleFactor_Y=rData.ScaleFactor_Y;
        ScaleFactor_R=0.;
        nCountAddition=rData.nCountAddition;
    }
    void Convert550(T_WIND_UDRD_D_CH_RW_550& rData)   // this function is added by KYM
    {
        Initialize();
        key = rData.key;
        nCodeType = rData.nCodeType;

        memcpy(Description, rData.Description, sizeof(Description));
        CodeParam.Initialize(nCodeType);

        switch ( rData.nCodeType )
        {
        case KS_W_92: CodeParam.KS1992 = rData.KS1992; break;
        case JP_W_87: CodeParam.JP1987 = rData.JP1987; break;
        case UBC_W_97: CodeParam.UBC1997.Convert570(rData.UBC1997); break;
        case ANSI_W_82: CodeParam.ANSI1982 = rData.ANSI1982; break;
        case KS_W_2000: CodeParam.KS2000.Convert570(rData.KS2000); break;
        case IBC_W_2000: CodeParam.IBC2000.Convert570(rData.IBC2000); break;
        case EURO_W_1992: CodeParam.EURO1992.Convert580(rData.EURO1992); break;
        case BS6399_W_1997: CodeParam.BS6399 = rData.BS6399; break;
        case CH_W_2002: CodeParam.CH2002.Convert590(rData.CH2002); break;
        case JPN_W_2000: CodeParam.JPN2000 = rData.JPN2000; break;
        default: break;
        }

        ScaleFactor_X=rData.ScaleFactor_X;
        ScaleFactor_Y=rData.ScaleFactor_Y;
        ScaleFactor_R=0.;
        nCountAddition=rData.nCountAddition;
    }
    void Convert570(T_WIND_UDRD_D_CH_RW_570& rData)   // this function is added by KYM
    {
        Initialize();
        key = rData.key;
        nCodeType = rData.nCodeType;

        memcpy(Description, rData.Description, sizeof(Description));
        CodeParam.Initialize(nCodeType);

        switch ( rData.nCodeType )
        {
        case KS_W_92: CodeParam.KS1992 = rData.KS1992; break;
        case JP_W_87: CodeParam.JP1987 = rData.JP1987; break;
        case UBC_W_97: CodeParam.UBC1997.Convert570(rData.UBC1997); break;
        case ANSI_W_82: CodeParam.ANSI1982 = rData.ANSI1982; break;
        case KS_W_2000: CodeParam.KS2000.Convert570(rData.KS2000); break;
        case IBC_W_2000: CodeParam.IBC2000.Convert570(rData.IBC2000); break;
        case EURO_W_1992: CodeParam.EURO1992.Convert580(rData.EURO1992); break;
        case BS6399_W_1997: CodeParam.BS6399 = rData.BS6399; break;
        case CH_W_2002: CodeParam.CH2002.Convert590(rData.CH2002); break;
        case JPN_W_2000: CodeParam.JPN2000 = rData.JPN2000; break;
        case NBC_W_1995: CodeParam.NBC1995.Convert570(rData.NBC1995); break;
        default: break;
        }

        ScaleFactor_X=rData.ScaleFactor_X;
        ScaleFactor_Y=rData.ScaleFactor_Y;
        ScaleFactor_R=0.;
        nCountAddition=rData.nCountAddition;
    }
    void Convert580(T_WIND_UDRD_D_CH_RW_580& rData)   // this function is added by KYM
    {
        Initialize();
        key = rData.key;
        nCodeType = rData.nCodeType;

        memcpy(Description, rData.Description, sizeof(Description));
        CodeParam.Initialize(nCodeType);

        switch ( rData.nCodeType )
        {
        case KS_W_92: CodeParam.KS1992 = rData.KS1992; break;
        case JP_W_87: CodeParam.JP1987 = rData.JP1987; break;
        case UBC_W_97: CodeParam.UBC1997 = rData.UBC1997; break;
        case ANSI_W_82: CodeParam.ANSI1982 = rData.ANSI1982; break;
        case KS_W_2000: CodeParam.KS2000.Convert640(rData.KS2000); break;
        case IBC_W_2000: CodeParam.IBC2000.Convert790(rData.IBC2000); break;
        case EURO_W_1992: CodeParam.EURO1992.Convert580(rData.EURO1992); break;
        case BS6399_W_1997: CodeParam.BS6399 = rData.BS6399; break;
        case CH_W_2002: CodeParam.CH2002.Convert590(rData.CH2002); break;
        case JPN_W_2000: CodeParam.JPN2000 = rData.JPN2000; break;
        case NBC_W_1995: CodeParam.NBC1995 = rData.NBC1995; break;
        default: break;
        }

        ScaleFactor_X=rData.ScaleFactor_X;
        ScaleFactor_Y=rData.ScaleFactor_Y;
        ScaleFactor_R=0.;
        nCountAddition=rData.nCountAddition;
    }
    void Convert590(T_WIND_UDRD_D_CH_RW_590& rData)   // this function is added by KYM
    {
        Initialize();
        key = rData.key;
        nCodeType = rData.nCodeType;

        memcpy(Description, rData.Description, sizeof(Description));
        CodeParam.Initialize(nCodeType);

        switch ( rData.nCodeType )
        {
        case KS_W_92: CodeParam.KS1992 = rData.KS1992; break;
        case JP_W_87: CodeParam.JP1987 = rData.JP1987; break;
        case UBC_W_97: CodeParam.UBC1997 = rData.UBC1997; break;
        case ANSI_W_82: CodeParam.ANSI1982 = rData.ANSI1982; break;
        case KS_W_2000: CodeParam.KS2000.Convert640(rData.KS2000); break;
        case IBC_W_2000: CodeParam.IBC2000.Convert790(rData.IBC2000); break;
        case EURO_W_1992: CodeParam.EURO1992 = rData.EURO1992; break;
        case BS6399_W_1997: CodeParam.BS6399 = rData.BS6399; break;
        case CH_W_2002: CodeParam.CH2002.Convert590(rData.CH2002); break;
        case JPN_W_2000: CodeParam.JPN2000 = rData.JPN2000; break;
        case NBC_W_1995: CodeParam.NBC1995 = rData.NBC1995; break;
        default: break;
        }

        ScaleFactor_X=rData.ScaleFactor_X;
        ScaleFactor_Y=rData.ScaleFactor_Y;
        ScaleFactor_R=0.;
        nCountAddition=rData.nCountAddition;
    }
    void Convert600(T_WIND_UDRD_D_CH_RW_600& rData)   // this function is added by KYM
    {
        Initialize();
        key = rData.key;
        nCodeType = rData.nCodeType;

        memcpy(Description, rData.Description, sizeof(Description));
        CodeParam.Initialize(nCodeType);

        switch ( rData.nCodeType )
        {
        case KS_W_92: CodeParam.KS1992 = rData.KS1992; break;
        case JP_W_87: CodeParam.JP1987 = rData.JP1987; break;
        case UBC_W_97: CodeParam.UBC1997 = rData.UBC1997; break;
        case ANSI_W_82: CodeParam.ANSI1982 = rData.ANSI1982; break;
        case KS_W_2000: CodeParam.KS2000.Convert640(rData.KS2000); break;
        case IBC_W_2000: CodeParam.IBC2000.Convert790(rData.IBC2000); break;
        case EURO_W_1992: CodeParam.EURO1992 = rData.EURO1992; break;
        case BS6399_W_1997: CodeParam.BS6399 = rData.BS6399; break;
        case CH_W_2002: CodeParam.CH2002.Convert811(rData.CH2002); break;
        case JPN_W_2000: CodeParam.JPN2000 = rData.JPN2000; break;
        case NBC_W_1995: CodeParam.NBC1995 = rData.NBC1995; break;
        case IS_W_1987: CodeParam.IS1987 = rData.IS1987; break;
        default: break;
        }

        ScaleFactor_X=rData.ScaleFactor_X;
        ScaleFactor_Y=rData.ScaleFactor_Y;
        ScaleFactor_R=0.;
        nCountAddition=rData.nCountAddition;
    }
    void Convert640(T_WIND_UDRD_D_CH_RW_640& rData)   // this function is added by KYM
    {
        Initialize();
        key = rData.key;
        nCodeType = rData.nCodeType;

        memcpy(Description, rData.Description, sizeof(Description));
        CodeParam.Initialize(nCodeType);

        switch ( rData.nCodeType )
        {
        case KS_W_92: CodeParam.KS1992 = rData.KS1992; break;
        case JP_W_87: CodeParam.JP1987 = rData.JP1987; break;
        case UBC_W_97: CodeParam.UBC1997 = rData.UBC1997; break;
        case ANSI_W_82: CodeParam.ANSI1982 = rData.ANSI1982; break;
        case KS_W_2000: CodeParam.KS2000.Convert640(rData.KS2000); break;
        case IBC_W_2000: CodeParam.IBC2000.Convert790(rData.IBC2000); break;
        case EURO_W_1992: CodeParam.EURO1992 = rData.EURO1992; break;
        case BS6399_W_1997: CodeParam.BS6399 = rData.BS6399; break;
        case CH_W_2002: CodeParam.CH2002.Convert811(rData.CH2002); break;
        case JPN_W_2000: CodeParam.JPN2000 = rData.JPN2000; break;
        case NBC_W_1995: CodeParam.NBC1995 = rData.NBC1995; break;
        case IS_W_1987: CodeParam.IS1987 = rData.IS1987; break;
        case TAIWAN_W_1986: CodeParam.TAIWAN86 = rData.TAIWAN86; break;
        default: break;
        }

        ScaleFactor_X=rData.ScaleFactor_X;
        ScaleFactor_Y=rData.ScaleFactor_Y;
        ScaleFactor_R=0.;
        nCountAddition=rData.nCountAddition;
    }
    void Convert671(T_WIND_UDRD_D_CH_RW_671& rData)
    {
        Initialize();
        key = rData.key;
        nCodeType = rData.nCodeType;

        memcpy(Description, rData.Description, sizeof(Description));
        CodeParam.Initialize(nCodeType);

        switch ( rData.nCodeType )
        {
        case KS_W_92: CodeParam.KS1992 = rData.KS1992; break;
        case JP_W_87: CodeParam.JP1987 = rData.JP1987; break;
        case UBC_W_97: CodeParam.UBC1997 = rData.UBC1997; break;
        case ANSI_W_82: CodeParam.ANSI1982 = rData.ANSI1982; break;
        case KS_W_2000: CodeParam.KS2000 = rData.KS2000; break;
        case IBC_W_2000: CodeParam.IBC2000.Convert790(rData.IBC2000); break;
        case EURO_W_1992: CodeParam.EURO1992 = rData.EURO1992; break;
        case BS6399_W_1997: CodeParam.BS6399 = rData.BS6399; break;
        case CH_W_2002: CodeParam.CH2002.Convert811(rData.CH2002); break;
        case JPN_W_2000: CodeParam.JPN2000 = rData.JPN2000; break;
        case NBC_W_1995: CodeParam.NBC1995 = rData.NBC1995; break;
        case IS_W_1987: CodeParam.IS1987 = rData.IS1987; break;
        case TAIWAN_W_1986: CodeParam.TAIWAN86 = rData.TAIWAN86; break;
        default: break;
        }

        ScaleFactor_X=rData.ScaleFactor_X;
        ScaleFactor_Y=rData.ScaleFactor_Y;
        ScaleFactor_R=0.;
        nCountAddition=rData.nCountAddition;
    }
    void Convert712(T_WIND_UDRD_D_CH_RW_712& rData)
    {
        Initialize();
        key = rData.key;
        nCodeType = rData.nCodeType;

        memcpy(Description, rData.Description, sizeof(Description));
        CodeParam.Initialize(nCodeType);

        switch ( rData.nCodeType )
        {
        case KS_W_92: CodeParam.KS1992 = rData.KS1992; break;
        case JP_W_87: CodeParam.JP1987 = rData.JP1987; break;
        case UBC_W_97: CodeParam.UBC1997 = rData.UBC1997; break;
        case ANSI_W_82: CodeParam.ANSI1982 = rData.ANSI1982; break;
        case KS_W_2000: CodeParam.KS2000 = rData.KS2000; break;
        case IBC_W_2000: CodeParam.IBC2000.Convert790(rData.IBC2000); break;
        case EURO_W_1992: CodeParam.EURO1992 = rData.EURO1992; break;
        case BS6399_W_1997: CodeParam.BS6399 = rData.BS6399; break;
        case CH_W_2002: CodeParam.CH2002.Convert811(rData.CH2002); break;
        case JPN_W_2000: CodeParam.JPN2000 = rData.JPN2000; break;
        case NBC_W_1995: CodeParam.NBC1995 = rData.NBC1995; break;
        case IS_W_1987: CodeParam.IS1987 = rData.IS1987; break;
        case TAIWAN_W_1986: CodeParam.TAIWAN86 = rData.TAIWAN86; break;
        case JP_W_2004: CodeParam.JP2004 = rData.JP2004; break;
        default: break;
        }

        ScaleFactor_X=rData.ScaleFactor_X;
        ScaleFactor_Y=rData.ScaleFactor_Y;
        ScaleFactor_R=0.;
        nCountAddition=rData.nCountAddition;
    }
    void Convert750(T_WIND_UDRD_D_CH_RW_750& rData)
    {
        Initialize();
        key = rData.key;
        nCodeType = rData.nCodeType;

        memcpy(Description, rData.Description, sizeof(Description));
        CodeParam.Initialize(nCodeType);

        switch ( rData.nCodeType )
        {
        case KS_W_92: CodeParam.KS1992 = rData.KS1992; break;
        case JP_W_87: CodeParam.JP1987 = rData.JP1987; break;
        case UBC_W_97: CodeParam.UBC1997 = rData.UBC1997; break;
        case ANSI_W_82: CodeParam.ANSI1982 = rData.ANSI1982; break;
        case KS_W_2000: CodeParam.KS2000 = rData.KS2000; break;
        case IBC_W_2000: CodeParam.IBC2000.Convert790(rData.IBC2000); break;
        case EURO_W_1992: CodeParam.EURO1992 = rData.EURO1992; break;
        case BS6399_W_1997: CodeParam.BS6399 = rData.BS6399; break;
        case CH_W_2002: CodeParam.CH2002.Convert811(rData.CH2002); break;
        case JPN_W_2000: CodeParam.JPN2000 = rData.JPN2000; break;
        case NBC_W_1995: CodeParam.NBC1995 = rData.NBC1995; break;
        case IS_W_1987: CodeParam.IS1987 = rData.IS1987; break;
        case TAIWAN_W_1986: CodeParam.TAIWAN86 = rData.TAIWAN86; break;
        case JP_W_2004: CodeParam.JP2004 = rData.JP2004; break;
        case EURO_W_2005: CodeParam.EURO2005.Convert940(rData.EURO2005); break;
        default: break;
        }

        ScaleFactor_X=rData.ScaleFactor_X;
        ScaleFactor_Y=rData.ScaleFactor_Y;
        ScaleFactor_R=0.;
        nCountAddition=rData.nCountAddition;
    }
    void Convert790(T_WIND_UDRD_D_CH_RW_790& rData)
    {
        Initialize();
        key = rData.key;
        nCodeType = rData.nCodeType;

        memcpy(Description, rData.Description, sizeof(Description));
        CodeParam.Initialize(nCodeType);

        switch ( rData.nCodeType )
        {
        case KS_W_92: CodeParam.KS1992 = rData.KS1992; break;
        case JP_W_87: CodeParam.JP1987 = rData.JP1987; break;
        case UBC_W_97: CodeParam.UBC1997 = rData.UBC1997; break;
        case ANSI_W_82: CodeParam.ANSI1982 = rData.ANSI1982; break;
        case KS_W_2000: CodeParam.KS2000 = rData.KS2000; break;
        case IBC_W_2000: CodeParam.IBC2000.Convert790(rData.IBC2000); break;
        case EURO_W_1992: CodeParam.EURO1992 = rData.EURO1992; break;
        case BS6399_W_1997: CodeParam.BS6399 = rData.BS6399; break;
        case CH_W_2002: CodeParam.CH2002.Convert811(rData.CH2002); break;
        case JPN_W_2000: CodeParam.JPN2000 = rData.JPN2000; break;
        case NBC_W_1995: CodeParam.NBC1995 = rData.NBC1995; break;
        case IS_W_1987: CodeParam.IS1987 = rData.IS1987; break;
        case TAIWAN_W_1986: CodeParam.TAIWAN86 = rData.TAIWAN86; break;
        case JP_W_2004: CodeParam.JP2004 = rData.JP2004; break;
        case EURO_W_2005: CodeParam.EURO2005.Convert940(rData.EURO2005); break;
        case KBC_W_2009: CodeParam.KBC2009.Convert790(rData.KBC2009); break;
        default: break;
        }

        ScaleFactor_X=rData.ScaleFactor_X;
        ScaleFactor_Y=rData.ScaleFactor_Y;
        ScaleFactor_R=rData.ScaleFactor_R;
        nCountAddition=rData.nCountAddition;
    }
    void Convert796(T_WIND_UDRD_D_CH_RW_796& rData)
    {
        Initialize();
        key = rData.key;
        nCodeType = rData.nCodeType;

        memcpy(Description, rData.Description, sizeof(Description));
        CodeParam.Initialize(nCodeType);

        switch ( rData.nCodeType )
        {
        case KS_W_92: CodeParam.KS1992 = rData.KS1992; break;
        case JP_W_87: CodeParam.JP1987 = rData.JP1987; break;
        case UBC_W_97: CodeParam.UBC1997 = rData.UBC1997; break;
        case ANSI_W_82: CodeParam.ANSI1982 = rData.ANSI1982; break;
        case KS_W_2000: CodeParam.KS2000 = rData.KS2000; break;
        case IBC_W_2000: CodeParam.IBC2000 = rData.IBC2000; break;
        case EURO_W_1992: CodeParam.EURO1992 = rData.EURO1992; break;
        case BS6399_W_1997: CodeParam.BS6399 = rData.BS6399; break;
        case CH_W_2002: CodeParam.CH2002.Convert811(rData.CH2002); break;
        case JPN_W_2000: CodeParam.JPN2000 = rData.JPN2000; break;
        case NBC_W_1995: CodeParam.NBC1995 = rData.NBC1995; break;
        case IS_W_1987: CodeParam.IS1987 = rData.IS1987; break;
        case TAIWAN_W_1986: CodeParam.TAIWAN86 = rData.TAIWAN86; break;
        case JP_W_2004: CodeParam.JP2004 = rData.JP2004; break;
        case EURO_W_2005: CodeParam.EURO2005.Convert940(rData.EURO2005); break;
        case KBC_W_2009: CodeParam.KBC2009 = rData.KBC2009; break;
        default: break;
        }

        ScaleFactor_X=rData.ScaleFactor_X;
        ScaleFactor_Y=rData.ScaleFactor_Y;
        ScaleFactor_R=rData.ScaleFactor_R;
        nCountAddition=rData.nCountAddition;
    }
    void Convert811(T_WIND_UDRD_D_CH_RW_811& rData)
    {
        Initialize();
        key = rData.key;
        nCodeType = rData.nCodeType;

        memcpy(Description, rData.Description, sizeof(Description));
        CodeParam.Initialize(nCodeType);

        switch ( rData.nCodeType )
        {
        case KS_W_92: CodeParam.KS1992 = rData.KS1992; break;
        case JP_W_87: CodeParam.JP1987 = rData.JP1987; break;
        case UBC_W_97: CodeParam.UBC1997 = rData.UBC1997; break;
        case ANSI_W_82: CodeParam.ANSI1982 = rData.ANSI1982; break;
        case KS_W_2000: CodeParam.KS2000 = rData.KS2000; break;
        case IBC_W_2000: CodeParam.IBC2000 = rData.IBC2000; break;
        case EURO_W_1992: CodeParam.EURO1992 = rData.EURO1992; break;
        case BS6399_W_1997: CodeParam.BS6399 = rData.BS6399; break;
        case CH_W_2002: CodeParam.CH2002.Convert811(rData.CH2002); break;
        case JPN_W_2000: CodeParam.JPN2000 = rData.JPN2000; break;
        case NBC_W_1995: CodeParam.NBC1995 = rData.NBC1995; break;
        case IS_W_1987: CodeParam.IS1987 = rData.IS1987; break;
        case TAIWAN_W_1986: CodeParam.TAIWAN86 = rData.TAIWAN86; break;
        case JP_W_2004: CodeParam.JP2004 = rData.JP2004; break;
        case EURO_W_2005: CodeParam.EURO2005.Convert940(rData.EURO2005); break;
        case KBC_W_2009: CodeParam.KBC2009 = rData.KBC2009; break;
        case IBC_W_2009: CodeParam.IBC2009 = rData.IBC2009; break;
        case IBC_W_2012: CodeParam.IBC2012 = rData.IBC2012; break;
        default: break;
        }

        ScaleFactor_X=rData.ScaleFactor_X;
        ScaleFactor_Y=rData.ScaleFactor_Y;
        ScaleFactor_R=rData.ScaleFactor_R;
        nCountAddition=rData.nCountAddition;
    }
    void Convert815(T_WIND_UDRD_D_CH_RW_815& rData)
    {
        Initialize();
        key = rData.key;
        nCodeType = rData.nCodeType;

        memcpy(Description, rData.Description, sizeof(Description));
        CodeParam.Initialize(nCodeType);

        switch ( rData.nCodeType )
        {
        case KS_W_92: CodeParam.KS1992 = rData.KS1992; break;
        case JP_W_87: CodeParam.JP1987 = rData.JP1987; break;
        case UBC_W_97: CodeParam.UBC1997 = rData.UBC1997; break;
        case ANSI_W_82: CodeParam.ANSI1982 = rData.ANSI1982; break;
        case KS_W_2000: CodeParam.KS2000 = rData.KS2000; break;
        case IBC_W_2000: CodeParam.IBC2000 = rData.IBC2000; break;
        case EURO_W_1992: CodeParam.EURO1992 = rData.EURO1992; break;
        case BS6399_W_1997: CodeParam.BS6399 = rData.BS6399; break;
        case CH_W_2002: CodeParam.CH2002 = rData.CH2002; break;
        case JPN_W_2000: CodeParam.JPN2000 = rData.JPN2000; break;
        case NBC_W_1995: CodeParam.NBC1995 = rData.NBC1995; break;
        case IS_W_1987: CodeParam.IS1987 = rData.IS1987; break;
        case TAIWAN_W_1986: CodeParam.TAIWAN86 = rData.TAIWAN86; break;
        case JP_W_2004: CodeParam.JP2004 = rData.JP2004; break;
        case EURO_W_2005: CodeParam.EURO2005.Convert940(rData.EURO2005); break;
        case KBC_W_2009: CodeParam.KBC2009 = rData.KBC2009; break;
        case IBC_W_2009: CodeParam.IBC2009 = rData.IBC2009; break;
        case IBC_W_2012: CodeParam.IBC2012 = rData.IBC2012; break;
        case CH_W_2012: CodeParam.CH2012.Convert815(rData.CH2012); break;
        default: break;
        }

        ScaleFactor_X=rData.ScaleFactor_X;
        ScaleFactor_Y=rData.ScaleFactor_Y;
        ScaleFactor_R=rData.ScaleFactor_R;
        nCountAddition=rData.nCountAddition;
    }
    void Convert845(T_WIND_UDRD_D_CH_RW_845& rData)
    {
        Initialize();
        key = rData.key;
        nCodeType = rData.nCodeType;

        memcpy(Description, rData.Description, sizeof(Description));
        CodeParam.Initialize(nCodeType);

        switch ( rData.nCodeType )
        {
        case KS_W_92: CodeParam.KS1992 = rData.KS1992; break;
        case JP_W_87: CodeParam.JP1987 = rData.JP1987; break;
        case UBC_W_97: CodeParam.UBC1997 = rData.UBC1997; break;
        case ANSI_W_82: CodeParam.ANSI1982 = rData.ANSI1982; break;
        case KS_W_2000: CodeParam.KS2000 = rData.KS2000; break;
        case IBC_W_2000: CodeParam.IBC2000 = rData.IBC2000; break;
        case EURO_W_1992: CodeParam.EURO1992 = rData.EURO1992; break;
        case BS6399_W_1997: CodeParam.BS6399 = rData.BS6399; break;
        case CH_W_2002: CodeParam.CH2002 = rData.CH2002; break;
        case JPN_W_2000: CodeParam.JPN2000 = rData.JPN2000; break;
        case NBC_W_1995: CodeParam.NBC1995 = rData.NBC1995; break;
        case IS_W_1987: CodeParam.IS1987 = rData.IS1987; break;
        case TAIWAN_W_1986: CodeParam.TAIWAN86 = rData.TAIWAN86; break;
        case JP_W_2004: CodeParam.JP2004 = rData.JP2004; break;
        case EURO_W_2005: CodeParam.EURO2005.Convert940(rData.EURO2005); break;
        case KBC_W_2009: CodeParam.KBC2009 = rData.KBC2009; break;
        case IBC_W_2009: CodeParam.IBC2009 = rData.IBC2009; break;
        case IBC_W_2012: CodeParam.IBC2012 = rData.IBC2012; break;
        case CH_W_2012: CodeParam.CH2012.Convert915(rData.CH2012); break;
        default: break;
        }

        ScaleFactor_X=rData.ScaleFactor_X;
        ScaleFactor_Y=rData.ScaleFactor_Y;
        ScaleFactor_R=rData.ScaleFactor_R;
        nCountAddition=rData.nCountAddition;
    }
    void Convert850(T_WIND_UDRD_D_CH_RW_850& rData)
    {
        Initialize();
        key = rData.key;
        nCodeType = rData.nCodeType;

        memcpy(Description, rData.Description, sizeof(Description));
        CodeParam.Initialize(nCodeType);

        switch ( rData.nCodeType )
        {
        case KS_W_92: CodeParam.KS1992 = rData.KS1992; break;
        case JP_W_87: CodeParam.JP1987 = rData.JP1987; break;
        case UBC_W_97: CodeParam.UBC1997 = rData.UBC1997; break;
        case ANSI_W_82: CodeParam.ANSI1982 = rData.ANSI1982; break;
        case KS_W_2000: CodeParam.KS2000 = rData.KS2000; break;
        case IBC_W_2000: CodeParam.IBC2000 = rData.IBC2000; break;
        case EURO_W_1992: CodeParam.EURO1992 = rData.EURO1992; break;
        case BS6399_W_1997: CodeParam.BS6399 = rData.BS6399; break;
        case CH_W_2002: CodeParam.CH2002 = rData.CH2002; break;
        case JPN_W_2000: CodeParam.JPN2000 = rData.JPN2000; break;
        case NBC_W_1995: CodeParam.NBC1995 = rData.NBC1995; break;
        case IS_W_1987: CodeParam.IS1987 = rData.IS1987; break;
        case TAIWAN_W_1986: CodeParam.TAIWAN86 = rData.TAIWAN86; break;
        case JP_W_2004: CodeParam.JP2004 = rData.JP2004; break;
        case EURO_W_2005: CodeParam.EURO2005.Convert940(rData.EURO2005); break;
        case KBC_W_2009: CodeParam.KBC2009 = rData.KBC2009; break;
        case IBC_W_2009: CodeParam.IBC2009 = rData.IBC2009; break;
        case IBC_W_2012: CodeParam.IBC2012 = rData.IBC2012; break;
        case CH_W_2012: CodeParam.CH2012.Convert915(rData.CH2012); break;
        case NSR_W_2010: CodeParam.NSR2010 = rData.NSR2010; break;
        default: break;
        }

        ScaleFactor_X=rData.ScaleFactor_X;
        ScaleFactor_Y=rData.ScaleFactor_Y;
        ScaleFactor_R=rData.ScaleFactor_R;
        nCountAddition=rData.nCountAddition;
    }
    void Convert870(T_WIND_UDRD_D_CH_RW_870& rData)
    {
        Initialize();
        key = rData.key;
        nCodeType = rData.nCodeType;

        memcpy(Description, rData.Description, sizeof(Description));
        CodeParam.Initialize(nCodeType);

        switch ( rData.nCodeType )
        {
        case KS_W_92: CodeParam.KS1992 = rData.KS1992; break;
        case JP_W_87: CodeParam.JP1987 = rData.JP1987; break;
        case UBC_W_97: CodeParam.UBC1997 = rData.UBC1997; break;
        case ANSI_W_82: CodeParam.ANSI1982 = rData.ANSI1982; break;
        case KS_W_2000: CodeParam.KS2000 = rData.KS2000; break;
        case IBC_W_2000: CodeParam.IBC2000 = rData.IBC2000; break;
        case EURO_W_1992: CodeParam.EURO1992 = rData.EURO1992; break;
        case BS6399_W_1997: CodeParam.BS6399 = rData.BS6399; break;
        case CH_W_2002: CodeParam.CH2002 = rData.CH2002; break;
        case JPN_W_2000: CodeParam.JPN2000 = rData.JPN2000; break;
        case NBC_W_1995: CodeParam.NBC1995 = rData.NBC1995; break;
        case IS_W_1987: CodeParam.IS1987 = rData.IS1987; break;
        case TAIWAN_W_1986: CodeParam.TAIWAN86 = rData.TAIWAN86; break;
        case JP_W_2004: CodeParam.JP2004 = rData.JP2004; break;
        case EURO_W_2005:CodeParam.EURO2005.Convert940(rData.EURO2005); break;
        case KBC_W_2009: CodeParam.KBC2009 = rData.KBC2009; break;
        case IBC_W_2009: CodeParam.IBC2009 = rData.IBC2009; break;
        case IBC_W_2012: CodeParam.IBC2012 = rData.IBC2012; break;
        case CH_W_2012: CodeParam.CH2012.Convert915(rData.CH2012); break;
        case NSR_W_2010: CodeParam.NSR2010 = rData.NSR2010; break;
        case KBC_W_2016: CodeParam.KBC2015 = rData.KBC2015; break;
        default: break;
        }

        ScaleFactor_X=rData.ScaleFactor_X;
        ScaleFactor_Y=rData.ScaleFactor_Y;
        ScaleFactor_R=rData.ScaleFactor_R;
        nCountAddition=rData.nCountAddition;
    }
    void Convert890(T_WIND_UDRD_D_CH_RW_890& rData)
    {
        Initialize();
        key = rData.key;
        nCodeType = rData.nCodeType;

        memcpy(Description, rData.Description, sizeof(Description));
        CodeParam.Initialize(nCodeType);

        switch ( rData.nCodeType )
        {
        case KS_W_92: CodeParam.KS1992 = rData.KS1992; break;
        case JP_W_87: CodeParam.JP1987 = rData.JP1987; break;
        case UBC_W_97: CodeParam.UBC1997 = rData.UBC1997; break;
        case ANSI_W_82: CodeParam.ANSI1982 = rData.ANSI1982; break;
        case KS_W_2000: CodeParam.KS2000 = rData.KS2000; break;
        case IBC_W_2000: CodeParam.IBC2000 = rData.IBC2000; break;
        case EURO_W_1992: CodeParam.EURO1992 = rData.EURO1992; break;
        case BS6399_W_1997: CodeParam.BS6399 = rData.BS6399; break;
        case CH_W_2002: CodeParam.CH2002 = rData.CH2002; break;
        case JPN_W_2000: CodeParam.JPN2000 = rData.JPN2000; break;
        case NBC_W_1995: CodeParam.NBC1995 = rData.NBC1995; break;
        case IS_W_1987: CodeParam.IS1987 = rData.IS1987; break;
        case TAIWAN_W_1986: CodeParam.TAIWAN86 = rData.TAIWAN86; break;
        case JP_W_2004: CodeParam.JP2004 = rData.JP2004; break;
        case EURO_W_2005: CodeParam.EURO2005.Convert940(rData.EURO2005); break;
        case KBC_W_2009: CodeParam.KBC2009 = rData.KBC2009; break;
        case IBC_W_2009: CodeParam.IBC2009 = rData.IBC2009; break;
        case IBC_W_2012: CodeParam.IBC2012 = rData.IBC2012; break;
        case CH_W_2012: CodeParam.CH2012.Convert915(rData.CH2012); break;
        case NSR_W_2010: CodeParam.NSR2010 = rData.NSR2010; break;
        case KBC_W_2016: CodeParam.KBC2015 = rData.KBC2015; break;
        case IS_W_875_2015: CodeParam.IS875_2015.Convert890(rData.IS875_2015); break;
        default: break;
        }

        ScaleFactor_X=rData.ScaleFactor_X;
        ScaleFactor_Y=rData.ScaleFactor_Y;
        ScaleFactor_R=rData.ScaleFactor_R;
        nCountAddition=rData.nCountAddition;
    }
    void Convert895(T_WIND_UDRD_D_CH_RW_895& rData)
    {
        Initialize();
        key = rData.key;
        nCodeType = rData.nCodeType;

        memcpy(Description, rData.Description, sizeof(Description));
        CodeParam.Initialize(nCodeType);

        switch ( rData.nCodeType )
        {
        case KS_W_92: CodeParam.KS1992 = rData.KS1992; break;
        case JP_W_87: CodeParam.JP1987 = rData.JP1987; break;
        case UBC_W_97: CodeParam.UBC1997 = rData.UBC1997; break;
        case ANSI_W_82: CodeParam.ANSI1982 = rData.ANSI1982; break;
        case KS_W_2000: CodeParam.KS2000 = rData.KS2000; break;
        case IBC_W_2000: CodeParam.IBC2000 = rData.IBC2000; break;
        case EURO_W_1992: CodeParam.EURO1992 = rData.EURO1992; break;
        case BS6399_W_1997: CodeParam.BS6399 = rData.BS6399; break;
        case CH_W_2002: CodeParam.CH2002 = rData.CH2002; break;
        case JPN_W_2000: CodeParam.JPN2000 = rData.JPN2000; break;
        case NBC_W_1995: CodeParam.NBC1995 = rData.NBC1995; break;
        case IS_W_1987: CodeParam.IS1987 = rData.IS1987; break;
        case TAIWAN_W_1986: CodeParam.TAIWAN86 = rData.TAIWAN86; break;
        case JP_W_2004: CodeParam.JP2004 = rData.JP2004; break;
        case EURO_W_2005: CodeParam.EURO2005.Convert940(rData.EURO2005); break;
        case KBC_W_2009: CodeParam.KBC2009 = rData.KBC2009; break;
        case IBC_W_2009: CodeParam.IBC2009 = rData.IBC2009; break;
        case IBC_W_2012: CodeParam.IBC2012 = rData.IBC2012; break;
        case CH_W_2012: CodeParam.CH2012.Convert915(rData.CH2012); break;
        case NSR_W_2010: CodeParam.NSR2010 = rData.NSR2010; break;
        case KBC_W_2016: CodeParam.KBC2015 = rData.KBC2015; break;
        case IS_W_875_2015: CodeParam.IS875_2015.Convert940(rData.IS875_2015); break;
        default: break;
        }

        ScaleFactor_X=rData.ScaleFactor_X;
        ScaleFactor_Y=rData.ScaleFactor_Y;
        ScaleFactor_R=rData.ScaleFactor_R;
        nCountAddition=rData.nCountAddition;
    }
    void Convert900(T_WIND_UDRD_D_CH_RW_900& rData)
    {
        Initialize();
        key = rData.key;
        nCodeType = rData.nCodeType;

        memcpy(Description, rData.Description, sizeof(Description));
        CodeParam.Initialize(nCodeType);
        switch ( rData.nCodeType )
        {
        case KS_W_92: CodeParam.KS1992 = rData.KS1992; break;
        case JP_W_87: CodeParam.JP1987 = rData.JP1987; break;
        case UBC_W_97: CodeParam.UBC1997 = rData.UBC1997; break;
        case ANSI_W_82: CodeParam.ANSI1982 = rData.ANSI1982; break;
        case KS_W_2000: CodeParam.KS2000 = rData.KS2000; break;
        case IBC_W_2000: CodeParam.IBC2000 = rData.IBC2000; break;
        case EURO_W_1992: CodeParam.EURO1992 = rData.EURO1992; break;
        case BS6399_W_1997: CodeParam.BS6399 = rData.BS6399; break;
        case CH_W_2002: CodeParam.CH2002 = rData.CH2002; break;
        case JPN_W_2000: CodeParam.JPN2000 = rData.JPN2000; break;
        case NBC_W_1995: CodeParam.NBC1995 = rData.NBC1995; break;
        case IS_W_1987: CodeParam.IS1987 = rData.IS1987; break;
        case TAIWAN_W_1986: CodeParam.TAIWAN86 = rData.TAIWAN86; break;
        case JP_W_2004: CodeParam.JP2004 = rData.JP2004; break;
        case EURO_W_2005: CodeParam.EURO2005.Convert940(rData.EURO2005); break;
        case KBC_W_2009: CodeParam.KBC2009 = rData.KBC2009; break;
        case IBC_W_2009: CodeParam.IBC2009 = rData.IBC2009; break;
        case IBC_W_2012: CodeParam.IBC2012 = rData.IBC2012; break;
        case CH_W_2012: CodeParam.CH2012.Convert915(rData.CH2012); break;
        case NSR_W_2010: CodeParam.NSR2010 = rData.NSR2010; break;
        case KBC_W_2016: CodeParam.KBC2015 = rData.KBC2015; break;
        case IS_W_875_2015: CodeParam.IS875_2015.Convert940(rData.IS875_2015); break;
        case CH_W_2021: CodeParam.CH2019.Convert915(rData.CH2019); break;
        case KDS_W_2019: CodeParam.KDS2019 = rData.KDS2019; break;
        default: break;
        }

        ScaleFactor_X = rData.ScaleFactor_X;
        ScaleFactor_Y = rData.ScaleFactor_Y;
        ScaleFactor_R = rData.ScaleFactor_R;
        nCountAddition = rData.nCountAddition;
    }
    void Convert915(T_WIND_UDRD_D_CH_RW_915& rData)
    {
        Initialize();
        key = rData.key;
        nCodeType = rData.nCodeType;
        memcpy(Description, rData.Description, sizeof(Description));

        CodeParam.Convert915(nCodeType, rData.CodeParam);

        ScaleFactor_X  = rData.ScaleFactor_X;
        ScaleFactor_Y  = rData.ScaleFactor_Y;
        ScaleFactor_R  = rData.ScaleFactor_R;
        nCountAddition = rData.nCountAddition;
    }
	void Convert930(T_WIND_UDRD_D_CH_RW_930& rData)
	{
		Initialize();
		key = rData.key;
		nCodeType = rData.nCodeType;
		memcpy(Description, rData.Description, sizeof(Description));

		CodeParam.Convert960(nCodeType, rData.CodeParam);

		ScaleFactor_X = rData.ScaleFactor_X;
		ScaleFactor_Y = rData.ScaleFactor_Y;
		ScaleFactor_R = rData.ScaleFactor_R;
		nCountAddition = rData.nCountAddition;
		nCountUser = 0;
		nEccen_X = 0;
		nEccen_Y = 0;
	}
    void Convert960(T_WIND_UDRD_D_CH_RW_960& rData)
    {
        Initialize();
        key = rData.key;
        nCodeType = rData.nCodeType;
        memcpy(Description, rData.Description, sizeof(Description));

        CodeParam.Convert960(nCodeType, rData.CodeParam);
        ScaleFactor_X = rData.ScaleFactor_X;
        ScaleFactor_Y = rData.ScaleFactor_Y;
        ScaleFactor_R = rData.ScaleFactor_R;
        nCountAddition = rData.nCountAddition;
        nCountUser = rData.nCountUser;
        nEccen_X = rData.nEccen_X;
        nEccen_Y = rData.nEccen_Y;
    }
};

struct T_WIND_UNIT
{
    T_WIND_UNIT()
    {
        nCodeType=D_UNITSYS_NONE;
        Description=D_UNITSYS_NONE;

        KS1992_nExposureCategory    = D_UNITSYS_NONE;
        KS1992_dBasicWindSpeed      = D_UNITSYS_NONE;
        KS1992_dPressureCoefficient = D_UNITSYS_NONE;

        JP1987_dPressureReductionFactor = D_UNITSYS_NONE;
        JP1987_dWindPressureCoefficient = D_UNITSYS_NONE;

        UBC1997_nExposureCategory    = D_UNITSYS_NONE;
        UBC1997_dBasicWindSpeed      = D_UNITSYS_NONE;
        UBC1997_dImportanceFactor    = D_UNITSYS_NONE;
        UBC1997_dPressureCoefficient = D_UNITSYS_NONE;
        UBC1997_dHRoof               = D_UNITSYS_BASE_LENGTH;

        ANSI1982_nExposureCategory    = D_UNITSYS_NONE;
        ANSI1982_dBasicWindSpeed      = D_UNITSYS_NONE;
        ANSI1982_dImportanceFactor    = D_UNITSYS_NONE;
        ANSI1982_dWindwardCoefficient = D_UNITSYS_NONE;
        ANSI1982_dLewardCoefficient   = D_UNITSYS_NONE;

        KS2000_nExposureCategory = D_UNITSYS_NONE;
        KS2000_dBasicWindSpeed   = D_UNITSYS_NONE;
        KS2000_dGustFactor       = D_UNITSYS_NONE;
        KS2000_dKzt              = D_UNITSYS_NONE;
        KS2000_dHzt              = D_UNITSYS_BASE_LENGTH;
        KS2000_dHRoof            = D_UNITSYS_BASE_LENGTH;
        KS2000_dImportanceFactor = D_UNITSYS_NONE;

        IBC2000_nProcedure           = D_UNITSYS_NONE;
        IBC2000_nExposureCategory    = D_UNITSYS_NONE;
        IBC2000_dBasicWindSpeed      = D_UNITSYS_NONE;
        IBC2000_dGustFactorX         = D_UNITSYS_NONE;
        IBC2000_dGustFactorY         = D_UNITSYS_NONE;
        IBC2000_dHRoof               = D_UNITSYS_BASE_LENGTH;
        IBC2000_dImportanceFactor    = D_UNITSYS_NONE;
        IBC2000_dWindDirectionFactor = D_UNITSYS_NONE;
        IBC2000_nRigidity            = D_UNITSYS_NONE;
        IBC2000_dHillHeight          = D_UNITSYS_BASE_LENGTH;
        IBC2000_dHillLength          = D_UNITSYS_BASE_LENGTH;
        IBC2000_dHillDistance        = D_UNITSYS_BASE_LENGTH;

        BS6399_dMeanRoofHeight = D_UNITSYS_BASE_LENGTH; // added by KYM (단위변환되는 것만 추가)
        BS6399_dBldgSeparation = D_UNITSYS_BASE_LENGTH; // added by KYM (단위변환되는 것만 추가)

        NBC1995_dBuildingHeight = D_UNITSYS_BASE_LENGTH;
        NBC1995_dHillHeight     = D_UNITSYS_BASE_LENGTH;
        NBC1995_dHillLength     = D_UNITSYS_BASE_LENGTH;
        NBC1995_dHillDistance   = D_UNITSYS_BASE_LENGTH;

        JP2004_dBuildingHeight  = D_UNITSYS_BASE_LENGTH;
        JP2004_dBreadthX        = D_UNITSYS_BASE_LENGTH;
        JP2004_dBreadthY        = D_UNITSYS_BASE_LENGTH;
        JP2004_dHillHeight      = D_UNITSYS_BASE_LENGTH;
        JP2004_dHillLength      = D_UNITSYS_BASE_LENGTH;
        JP2004_dBldgDistance    = D_UNITSYS_BASE_LENGTH;

        EURO1992_dTopographyRange = D_UNITSYS_BASE_LENGTH;
        IS1987_dTopographyRange   = D_UNITSYS_BASE_LENGTH;

        EURO2005_dH     = D_UNITSYS_BASE_LENGTH;
        EURO2005_dOroH  = D_UNITSYS_BASE_LENGTH;
        EURO2005_dOroLu = D_UNITSYS_BASE_LENGTH;
        EURO2005_dOroLd = D_UNITSYS_BASE_LENGTH;
        EURO2005_dOroX  = D_UNITSYS_BASE_LENGTH;
        EURO2005_dNeiHa = D_UNITSYS_BASE_LENGTH;
        EURO2005_dNeiX  = D_UNITSYS_BASE_LENGTH;
        EURO2005_dNeiDl = D_UNITSYS_BASE_LENGTH;
        EURO2005_dNeiHh = D_UNITSYS_BASE_LENGTH;
        EURO2005_dDisHa = D_UNITSYS_BASE_LENGTH;
        EURO2005_dDisX  = D_UNITSYS_BASE_LENGTH;
        EURO2005_dCsB   = D_UNITSYS_BASE_LENGTH;
        EURO2005_dCsD   = D_UNITSYS_BASE_LENGTH;
        EURO2005_dZs    = D_UNITSYS_BASE_LENGTH;

        KBC2009_dBasicWindSpeed = D_UNITSYS_NONE;
        KBC2009_dRoofHeight     = D_UNITSYS_BASE_LENGTH;
        KBC2009_dVzt            = D_UNITSYS_BASE_LENGTH;

        IBC2012_dHRoof        = D_UNITSYS_BASE_LENGTH;
        IBC2012_dHillHeight   = D_UNITSYS_BASE_LENGTH;
        IBC2012_dHillLength   = D_UNITSYS_BASE_LENGTH;
        IBC2012_dHillDistance = D_UNITSYS_BASE_LENGTH;

        KBC2015_dBLx   = D_UNITSYS_BASE_LENGTH;
        KBC2015_dBLy   = D_UNITSYS_BASE_LENGTH;
        KBC2015_dMx1st = D_UNITSYS_BASE_MASS;
        KBC2015_dMy1st = D_UNITSYS_BASE_MASS;
        KBC2015_dMt1st = D_UNITSYS_BASE_ROMASS;

        DPT2007_dHillHeight   = D_UNITSYS_BASE_LENGTH;
        DPT2007_dHillLength   = D_UNITSYS_BASE_LENGTH;
        DPT2007_dHillDistance = D_UNITSYS_BASE_LENGTH;
        DPT2007_dTopographic  = D_UNITSYS_BASE_LENGTH;
        DPT2007_dBreadth      = D_UNITSYS_BASE_LENGTH;
        DPT2007_dVibr[EN_DPT_WIND_VIBR_FREQ_X] = D_UNITSYS_NONE;
        DPT2007_dVibr[EN_DPT_WIND_VIBR_FREQ_Y] = D_UNITSYS_NONE;
        DPT2007_dVibr[EN_DPT_WIND_VIBR_FREQ_T] = D_UNITSYS_NONE;
        DPT2007_dVibr[EN_DPT_WIND_VIBR_MASS_X] = D_UNITSYS_BASE_MASS;
        DPT2007_dVibr[EN_DPT_WIND_VIBR_MASS_Y] = D_UNITSYS_BASE_MASS;
        DPT2007_dVibr[EN_DPT_WIND_VIBR_MASS_T] = D_UNITSYS_BASE_ROMASS;
        DPT2007_dVibr[EN_DPT_WIND_VIBR_DAMP_Y] = D_UNITSYS_NONE;
        DPT2007_dVibr[EN_DPT_WIND_VIBR_DAMP_T] = D_UNITSYS_NONE;

        ScaleFactor_X=D_UNITSYS_NONE;
        ScaleFactor_Y=D_UNITSYS_NONE;
        ScaleFactor_R=D_UNITSYS_NONE;
        KeyStor=D_UNITSYS_NONE;
        AddX=D_UNITSYS_BASE_FORCE;
        AddY=D_UNITSYS_BASE_FORCE;
        AddR=D_UNITSYS_BASE_MOMENT;

		nPress = D_UNITSYS_BASE_PRESSURE;
		nElev = D_UNITSYS_BASE_LENGTH;
		nLoadH = D_UNITSYS_BASE_LENGTH;
		nLoadB = D_UNITSYS_BASE_LENGTH;
    }
    int nCodeType;
    int Description;

    int KS1992_nExposureCategory;
    int KS1992_dBasicWindSpeed;
    int KS1992_dPressureCoefficient;

    int JP1987_dPressureReductionFactor;
    int JP1987_dWindPressureCoefficient;

    int UBC1997_nExposureCategory;
    int UBC1997_dBasicWindSpeed;
    int UBC1997_dImportanceFactor;
    int UBC1997_dPressureCoefficient;
    int UBC1997_dHRoof;

    int ANSI1982_nExposureCategory;
    int ANSI1982_dBasicWindSpeed;
    int ANSI1982_dImportanceFactor;
    int ANSI1982_dWindwardCoefficient;
    int ANSI1982_dLewardCoefficient;

    int KS2000_nExposureCategory;
    int KS2000_dBasicWindSpeed;
    int KS2000_dGustFactor;
    int KS2000_dKzt;
    int KS2000_dHzt;
    int KS2000_dHRoof;
    int KS2000_dImportanceFactor;

    int IBC2000_nProcedure;
    int IBC2000_nExposureCategory;
    int IBC2000_dBasicWindSpeed;
    int IBC2000_dGustFactorX;
    int IBC2000_dGustFactorY;
    int IBC2000_dHRoof;
    int IBC2000_dImportanceFactor;
    int IBC2000_dWindDirectionFactor;
    int IBC2000_nRigidity;
    int IBC2000_dHillHeight;
    int IBC2000_dHillLength;
    int IBC2000_dHillDistance;

    int BS6399_dMeanRoofHeight;     // added by KYM(단위변환되는 것만 추가)
    int BS6399_dBldgSeparation;     // added by KYM(단위변환되는 것만 추가)

    int NBC1995_dBuildingHeight;
    int NBC1995_dHillHeight;
    int NBC1995_dHillLength;
    int NBC1995_dHillDistance;

    int EURO1992_dTopographyRange;
    int IS1987_dTopographyRange;

    int JP2004_dBuildingHeight;
    int JP2004_dBreadthX;
    int JP2004_dBreadthY;
    int JP2004_dHillHeight;
    int JP2004_dHillLength;
    int JP2004_dBldgDistance;

    int EURO2005_dH;
    int EURO2005_dOroH;
    int EURO2005_dOroLu;
    int EURO2005_dOroLd;
    int EURO2005_dOroX;
    int EURO2005_dNeiHa;
    int EURO2005_dNeiX;
    int EURO2005_dNeiDl;
    int EURO2005_dNeiHh;
    int EURO2005_dDisHa;
    int EURO2005_dDisX;
    int EURO2005_dCsB;
    int EURO2005_dCsD;
    int EURO2005_dZs;

    int KBC2009_dBasicWindSpeed;
    int KBC2009_dRoofHeight;
    int KBC2009_dVzt;

    int IBC2012_dHRoof;
    int IBC2012_dHillHeight;
    int IBC2012_dHillLength;
    int IBC2012_dHillDistance;
    int KBC2015_dBLx;
    int KBC2015_dBLy;
    int KBC2015_dMx1st;
    int KBC2015_dMy1st;
    int KBC2015_dMt1st;

    int DPT2007_dHillHeight;
    int DPT2007_dHillLength;
    int DPT2007_dHillDistance;
    int DPT2007_dTopographic; // Topographical Effect Level
    int DPT2007_dBreadth;
    int DPT2007_dVibr[EN_DPT_WIND_VIBR_NUM];

    int ScaleFactor_X;
    int ScaleFactor_Y;
    int ScaleFactor_R;
    int KeyStor;
    int AddX;
    int AddY;
    int AddR;

	int nPress;
	int nElev;
	int nLoadH;
	int nLoadB;
};

#define HASHSIZEWIND 7


#define T_WNPS_K unsigned int
// Wind Pulsation

enum EN_WNPS_CONSTRUCTION_TYPE
{
	EN_WNPS_TYPE_TOWERS = 0, EN_WNPS_TYPE_OTHERS
};

enum EN_WNPS_TERRAIN_TYPE
{
	EN_WNPS_TERRAIN_A = 0, EN_WNPS_TERRAIN_B, EN_WNPS_TERRAIN_C
};

struct T_WNPS_D
{
	
    EN_WNPS_CONSTRUCTION_TYPE enConstructionType;   // Type of construction (Y5)
    EN_WNPS_TERRAIN_TYPE enTerrainType;             // Type of Terrain (Y7)


	double dLogDecrement; // Logarithmic Decrement of Oscillations (Y8)
	double dWindPressure; // Normative value of wind pressure - wo (kPa) (Y6)

	// Dimensions (X and Y Directions)
	double dDimXPlan; // X2 - According to Plan (X-Direction)
	double dDimYPlan; // X3 - According to Plan (Y-Direction)
	double dDimXWind; // X4 - Along Wind Front (X-Direction)
	double dDimYWind; // X4_1 - Along Wind Front (Y-Direction)

	// Direction Factors (X and Y Directions)
	double dFactorX; // KPX - X-Direction
	double dFactorY; // KPY - Y-Direction

	// Sum of Modal Mass Percentage
	double dModalMassX; // Sum of modal mass percentage in X
	double dModalMassY; // Sum of modal mass percentage in Y

	// Mode shapes selection for X and Y directions
	CArray<bool, bool> arModeShapesX;   // Modes in X-Direction
	CArray<bool, bool> arModeShapesY;   // Modes in Y-Direction

    BOOL bCheckModeShape;
	void Initialize()
	{
		enConstructionType = EN_WNPS_TYPE_TOWERS;
		enTerrainType = EN_WNPS_TERRAIN_A;
		dLogDecrement = 0.3;
		dWindPressure = 0.23;
		dDimXPlan = 11.0;
		dDimYPlan = 11.0;
		dDimXWind = 12.0;
		dDimYWind = 10.0;
		dFactorX = 1.0;
		dFactorY = 1.0;
		dModalMassX = 0.0;
		dModalMassY = 0.0;
		arModeShapesX.RemoveAll();
		arModeShapesY.RemoveAll();
        bCheckModeShape = TRUE;
	}
	T_WNPS_D()
	{
		Initialize();
	}
	T_WNPS_D(T_WNPS_D& src)
	{
		*this = src;
	}
	T_WNPS_D& operator=(const T_WNPS_D& src)
	{
		enConstructionType = src.enConstructionType;
        enTerrainType = src.enTerrainType;
        dLogDecrement = src.dLogDecrement;
        dWindPressure = src.dWindPressure;
        dDimXPlan = src.dDimXPlan;
        dDimYPlan = src.dDimYPlan;
        dDimXWind = src.dDimXWind;
        dDimYWind = src.dDimYWind;
        dFactorX = src.dFactorX;
        dFactorY = src.dFactorY;
        dModalMassX = src.dModalMassX;
        dModalMassY = src.dModalMassY;
		arModeShapesX.Copy(src.arModeShapesX);
		arModeShapesY.Copy(src.arModeShapesY);
        bCheckModeShape = src.bCheckModeShape;
		return *this;
	}
};

struct T_WNPS_UDRD_D_RW
{
	T_WNPS_K key;
	
    EN_WNPS_CONSTRUCTION_TYPE enConstructionType;   // Type of construction (Y5)
	EN_WNPS_TERRAIN_TYPE enTerrainType;             // Type of Terrain (Y7)


	double dLogDecrement; // Logarithmic Decrement of Oscillations (Y8)
	double dWindPressure; // Normative value of wind pressure - wo (kPa) (Y6)

	// Dimensions (X and Y Directions)
	double dDimXPlan; // X2 - According to Plan (X-Direction)
	double dDimYPlan; // X3 - According to Plan (Y-Direction)
	double dDimXWind; // X4 - Along Wind Front (X-Direction)
	double dDimYWind; // X4_1 - Along Wind Front (Y-Direction)

	// Direction Factors (X and Y Directions)
	double dFactorX; // KPX - X-Direction
	double dFactorY; // KPY - Y-Direction

	// Sum of Modal Mass Percentage
	double dModalMassX; // Sum of modal mass percentage in X
	double dModalMassY; // Sum of modal mass percentage in Y

	// Mode shapes selection for X and Y directions
	int arModeShapesX;   // Modes in X-Direction
	int arModeShapesY;   // Modes in Y-Direction
    BOOL bCheckModeShape;
	void GetWnps(T_WNPS_K& rKey, T_WNPS_D& rData)
	{
		rKey = key;
		rData.enConstructionType = enConstructionType;
        rData.enTerrainType = enTerrainType;
        rData.dLogDecrement = dLogDecrement;
        rData.dWindPressure = dWindPressure;
        rData.dDimXPlan = dDimXPlan;
        rData.dDimYPlan = dDimYPlan;
        rData.dDimXWind = dDimXWind;
        rData.dDimYWind = dDimYWind;
        rData.dFactorX = dFactorX;
        rData.dFactorY = dFactorY;
        rData.dModalMassX = dModalMassX;
        rData.dModalMassY = dModalMassY;
        rData.arModeShapesX.SetSize(arModeShapesX);
        rData.arModeShapesY.SetSize(arModeShapesY);
        rData.bCheckModeShape = bCheckModeShape;
	}
	void SetWnps(const T_WNPS_K tKey, const T_WNPS_D& rData)
	{
		key = tKey;
		enConstructionType = rData.enConstructionType;
		enTerrainType = rData.enTerrainType;
		dLogDecrement = rData.dLogDecrement;
		dWindPressure = rData.dWindPressure;
		dDimXPlan = rData.dDimXPlan;
		dDimYPlan = rData.dDimYPlan;
		dDimXWind = rData.dDimXWind;
		dDimYWind = rData.dDimYWind;
		dFactorX = rData.dFactorX;
		dFactorY = rData.dFactorY;
		dModalMassX = rData.dModalMassX;
		dModalMassY = rData.dModalMassY;
		arModeShapesX = rData.arModeShapesX.GetSize();
		arModeShapesY = rData.arModeShapesY.GetSize();
        bCheckModeShape = rData.bCheckModeShape;
	}
};

struct T_WNPS_UDRD_D
{
	T_WNPS_K key;
	T_WNPS_D data;
};

struct T_WNPS_UNIT
{
	T_WNPS_UNIT()
	{
		dDimXPlan = D_UNITSYS_BASE_LENGTH;
		dDimYPlan = D_UNITSYS_BASE_LENGTH;
		dDimXWind = D_UNITSYS_BASE_LENGTH;
		dDimYWind = D_UNITSYS_BASE_LENGTH;
	}

	int	dDimXPlan;
	int	dDimYPlan;
	int	dDimXWind;
	int dDimYWind;
};
#define HASHSIZEWNPS 7

#pragma pack(pop)

#endif  // __DB_ST_DT_WIND_H__
